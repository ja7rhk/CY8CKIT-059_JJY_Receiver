/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs following functions:
*  1: 
*  2: 
*  3:
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

/* PSoC5 */
#include <device.h>
#include "stdio.h"
/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* API */
#include "I2C_CLCD.h"

/* Project Defines */
#define FALSE  0
#define TRUE   1

/***************************************************************************
*	Resources for FreeRTOS
****************************************************************************/

#define TASK_STACK_TONE   512
#define TASK_STACK_JJY   1024
#define TASK_STACK_CLCD   1024

xQueueHandle xQueue_TONE;    /* Queue Data of tone output from Task_JJY */
xQueueHandle xQueue_JJY_Bit;     /* Queue Data of JJY bit from Task_JJY */
xQueueHandle xQueue_Pule_Width;     /* Pulse width data from ISR */

#define TASK_PRIORITY_TONE (tskIDLE_PRIORITY + 1)
#define TASK_PRIORITY_JJY (tskIDLE_PRIORITY + 3)
#define TASK_PRIORITY_CLCD (tskIDLE_PRIORITY + 2)

void Task_TONE (void *pvParameters);
void Task_JJY (void *pvParameters);
void Task_CLCD (void *pvParameters);

xSemaphoreHandle xMutex_CLCD;       /* Mutex for CLCD Port */

/***************************************************************************
*	JJY Parameters
****************************************************************************/

enum rec_state {
   IDLE,
   PULSE_WIDHT_DET,
   PULSE_WAIT,
   JJY_TIME_OUT		/* assumed as receiving JJY call sign. */
};

enum bit_state {
   BIN_0,		/* binary 0 */
   BIN_1,		/* binary 1 */
   M,			/* Marker */
   PM,			/* Position Marker */
   UNKNOWN,
};

enum tone_state {
   TONE_E,
   TONE_N,
   TONE_THROUGH_ON,
   TONE_THROUGH_OFF,
   TONE_IDLE
};

#define nBlock	6
#define nBit	10

struct JJY_data
{
	uint16	nMinute;
	uint16	nHour;
	uint16	nDay;
	uint16	nYear;
	uint16	nDay_of_week;
	uint16	month;
	uint16	day;
	uint16   day_of_week[3];
};

// Pulse Width (usec)
#define PW_M	200000
#define PW_1	500000
#define PW_0	800000
#define PVAL	100000		// pulse width should be (PW_x +/- PVAL)


/*---------------------------------------------------------------------------*/
void prvHardwareSetup( void )
{
    /* Port layer functions that need to be copied into the vector table. */
    extern void xPortPendSVHandler( void );
    extern void xPortSysTickHandler( void );
    extern void vPortSVCHandler( void );
    extern cyisraddress CyRamVectors[];

	/* Install the OS Interrupt Handlers. */
	CyRamVectors[ 11 ] = ( cyisraddress ) vPortSVCHandler;
	CyRamVectors[ 14 ] = ( cyisraddress ) xPortPendSVHandler;
	CyRamVectors[ 15 ] = ( cyisraddress ) xPortSysTickHandler;
    
}
/*---------------------------------------------------------------------------*/
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* The stack space has been execeeded for a task, considering allocating more. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*---------------------------------------------------------------------------*/
void vApplicationMallocFailedHook( void )
{
	/* The heap space has been execeeded. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*---------------------------------------------------------------------------*/
    
/*******************************
*  Watchdog Timer ISR
********************************/
uint8 tFlag;

CY_ISR(timerISR_1_Routine)
{
    tFlag = Timer_1_ReadStatusRegister();
    /* Clear WDT */
    CyWdtClear();
}
/*---------------------------------------------------------------------------*/

/***************************************************************************
*	main()
****************************************************************************/
int main()
{
    prvHardwareSetup();
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Watchdog Timer Enable. */
    timerISR_1_StartEx(timerISR_1_Routine);
    Timer_1_Start();
    CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_NOCHANGE);

    /* Initializes the I2C CLCD. */
    I2C_1_Start();
    I2C_CLCD_Start();
    
    /* Create Tasks */
    xMutex_CLCD = xSemaphoreCreateMutex();

    CyDelay(100u);
    
    I2C_CLCD_Position(0u,0u);
    I2C_CLCD_PrintString("JJY Receiver");
    I2C_CLCD_Position(1u,0u);
    I2C_CLCD_PrintString("getting started");

    CyDelay(1000u);
    
    xTaskCreate(Task_TONE, "TONE", TASK_STACK_TONE, NULL, TASK_PRIORITY_TONE, NULL);
    xTaskCreate(Task_JJY, "JJY", TASK_STACK_JJY, NULL, TASK_PRIORITY_JJY, NULL);
    xTaskCreate(Task_CLCD, "CLCD", TASK_STACK_CLCD, NULL, TASK_PRIORITY_CLCD, NULL);

    xQueue_TONE    = xQueueCreate(4, 1);     /* 4 x 1 byte */
    xQueue_JJY_Bit = xQueueCreate(4, 1);     /* 4 x 1 byte */
    xQueue_Pule_Width = xQueueCreate(4, 4);  /* 4 x 8 byte */

    /* Start RTOS Kernel */
    vTaskStartScheduler();

    for(;;)
    {
        /* Place your application code here. */
    }
}

/*---------------------------------------------------------------------------*/


/***************************************************************************
*	Task_TONE
****************************************************************************/
void Task_TONE(void *pvParameters)
{
    const portTickType xDelay    = 1u   / portTICK_RATE_MS;
    const portTickType dotDelay  = 100u / portTICK_RATE_MS;
    const portTickType dashDelay = 400u / portTICK_RATE_MS;

    enum tone_state tone_flag;

    /* start the tone generator */
	TONE_Control_Reg_Write(0u);    /* Tone control OFF */
    WaveDAC8_1_Start();

    for(;;)
    {
        /* Get tone generate request from Task_JJT */
        if (uxQueueMessagesWaiting(xQueue_TONE) > 0)
        {
            xQueueReceive(xQueue_TONE, &tone_flag, 0);

            switch(tone_flag)
            {
                case TONE_E:     /* 100ms */
                	TONE_Control_Reg_Write(1u);
                    vTaskDelay(dotDelay);
                	TONE_Control_Reg_Write(0u);
                    break;
                case TONE_N:     /* 400ms + 100ms */
                	TONE_Control_Reg_Write(1u);
                    vTaskDelay(dashDelay);
                	TONE_Control_Reg_Write(0u);
                    vTaskDelay(dotDelay);
                	TONE_Control_Reg_Write(1u);
                    vTaskDelay(dotDelay);
                	TONE_Control_Reg_Write(0u);
                    break;
                case TONE_THROUGH_ON:
                    /* Control_Reg_1 bit1 = 1 */
                	TONE_Control_Reg_Write(2u);
                    break;
                case TONE_THROUGH_OFF:
                	TONE_Control_Reg_Write(0u);
                    break;
                default:
                    /* do nothing */
                    break;
            }
            tone_flag = TONE_IDLE;
        }       
        vTaskDelay(xDelay);
    }
}


/***************************************************************************
*	Task_JJY
****************************************************************************/
/*********************************
*	Interrupt Handler
**********************************/

CY_ISR(tc_pulse_det_ISR)
{
    LED_Write(1u);
    Counter_1_WriteCounter(0u);
}

CY_ISR(tc_pulse_end_ISR)
{
    uint32 pulse_width;
    
    LED_Write(0u);
    /* Send the Data to Queue */
    pulse_width = Counter_1_ReadCounter();
    xQueueSendFromISR(xQueue_Pule_Width, &pulse_width, NULL);

    /* Clear the Counter */
    Counter_1_WriteCounter(0u);
}

/*---------------------------------------------------------------------------*/
void Task_JJY(void *pvParameters)
{
  	enum bit_state last_bit = UNKNOWN;	/* Last received bit */
    uint32 pulse_width;         /* pulse width (usec) */
    
    const portTickType xDelay = 1u / portTICK_RATE_MS;

    Counter_1_Start();
    isr_TC_PULSE_DET_StartEx(tc_pulse_det_ISR);
    isr_TC_PULSE_END_StartEx(tc_pulse_end_ISR);

    Counter_1_WriteCounter(0u);     /* Clear the Counter */

    for(;;)
    {
    	/********************************************
    	*  Falling edge of a pulse is detected
    	*********************************************/
        if (uxQueueMessagesWaiting(xQueue_Pule_Width) > 0)
        {
            xQueueReceive(xQueue_Pule_Width, &pulse_width, 0);        

            /* Marker or Position Marker */
    		if ((pulse_width > PW_M - PVAL) && (pulse_width < PW_M + PVAL))
			{
                /* Are there two continuous Markers?　*/
				if (last_bit == PM)
					last_bit = M;
				else
					last_bit = PM;
			}
    		else if ((pulse_width > PW_1 - PVAL) && (pulse_width < PW_1 + PVAL))
				last_bit = BIN_1;

            else if ((pulse_width > PW_0 - PVAL) && (pulse_width < PW_0 + PVAL))
				last_bit = BIN_0;
			else
				last_bit = UNKNOWN;

			/* Send the received data to Task_CLCD */
			xQueueSend(xQueue_JJY_Bit, &last_bit, 0);
		}
        vTaskDelay(xDelay);
    }
}


/***************************************************************************
*	Task_CLCD
****************************************************************************/
void BCD_decode_data (uint8 block);
uint32 BCD2time (uint8 block);
void display_rx_data (void);
void display_time (uint8 block);
void nDay_to_calendar (void);

struct JJY_data	ct;     /* current JJY decoded data */
enum bit_state tmp_buf[nBlock][nBit];	   /* 1 frame temp data */

void Task_CLCD(void *pvParameters)
{
  	enum bit_state last_bit = UNKNOWN;	/* Last received bit */
    enum tone_state tone_flag;

    uint8 CLCD_sync = FALSE; 
    uint8 CLCD_block = 0;
    uint8 CLCD_bit = 0;
    uint8 JJY_call_count = 0;
	char8 buf[32];           /* CLCD */

    const portTickType xDelay = 1u      / portTICK_RATE_MS;

    I2C_CLCD_WriteControl(I2C_CLCD_CLEAR_DISPLAY);

    ct.nMinute = 0;
	ct.nHour = 0;
	ct.nDay = 0;
	ct.nYear = 0;
	ct.nDay_of_week = 0;
	ct.month = 0;
	ct.day = 0;

    for(;;)
    {
        if (uxQueueMessagesWaiting(xQueue_JJY_Bit) > 0)
        {
            xQueueReceive(xQueue_JJY_Bit, &last_bit, 0);
            
            if (JJY_Control_Reg_Read()) /* JJY Call? */
            {
                JJY_call_count++;
                /* JJY call is "JJY JJY" in Morse code 
                *--------------------------------------------------------------------------
                * 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
                *  .  -  -  -  .  -  -  -  -  .  -  -  .  -  -  -  .  -  -  -  -  .  -  -
                *--------------------------------------------------------------------------
                *          (J)         (J)         (Y)         (J)          (J)        (Y)
                */
                switch (JJY_call_count)
                {
                    case 1: /* dot of the 1st 'J' */
                		I2C_CLCD_Position(0, 0);
            			I2C_CLCD_PrintString("(4)             ");
                    	I2C_CLCD_Position(0, CLCD_bit + 4);
                        break;
                    case 4:
                    case 8:
                    case 16:
                    case 20:
                        I2C_CLCD_PutChar('J');
                        CLCD_bit++;
                        break;
                    case 12:
                        I2C_CLCD_PutChar('Y');
                        CLCD_bit++;
                        break;
                    case 13:
                        I2C_CLCD_PutChar(' ');
                        CLCD_bit++;
                        break;
                    case 24: /* - of 1st 'Y' */
                        I2C_CLCD_PutChar('Y');
                        I2C_CLCD_PutChar(' ');
                        CLCD_bit += 2;
                        JJY_Control_Reg_Write(FALSE);      /* reset through mode */
                        break;
                }
                continue;   /* skip the followings */
            }

        	if (last_bit == M)
        	{
        		CLCD_sync = TRUE;
        		CLCD_block = 0;
        		CLCD_bit = 0;
                JJY_call_count = 0;
        	}
        	/* character position */
        	if (CLCD_bit == 0)
        	{
        		I2C_CLCD_Position(0, 0);
        		if (CLCD_sync)
        		{
        			sprintf(buf, "(%d)             ", CLCD_block);
        			I2C_CLCD_PrintString(buf);
        		}
        		else
        			I2C_CLCD_PrintString("(x)             ");
        	}

        	/* print received data */
        	I2C_CLCD_Position(0, CLCD_bit + 4);
        	if (last_bit == M)
        		I2C_CLCD_PutChar('M');
            else if (last_bit == PM)
        		I2C_CLCD_PutChar('P');
        	else if (last_bit == BIN_0)
        		I2C_CLCD_PutChar('0');
        	else if (last_bit == BIN_1)
        		I2C_CLCD_PutChar('1');
        	else if (last_bit == UNKNOWN)
        	{
        		I2C_CLCD_PutChar('?');
        		CLCD_sync = FALSE;
        	}

            tmp_buf[CLCD_block][CLCD_bit] = last_bit;

        	CLCD_bit++;
        	if (last_bit == M)
        	{
                /* Tone Signal */
				if (ct.nMinute == 59)      /* if the last time was 59 min, */
                    tone_flag = TONE_N;   /* this marker is for 0 min period. */
                else
                    tone_flag = TONE_E;
                xQueueSend(xQueue_TONE, &tone_flag, 0);
            }
            if (last_bit == PM)
        	{
          		/* Display the current time on the 2nd line */
                if (CLCD_sync)
                {
                    BCD_decode_data(CLCD_block);
            		display_time(CLCD_block);
                }
                /* JJY Call check the next block is JJY */
    			if (((ct.nMinute == 15) || (ct.nMinute == 45)) && (CLCD_block == 3))
                    JJY_Control_Reg_Write(TRUE);      /* set through mode */
                else
                    JJY_Control_Reg_Write(FALSE);      /* reset through mode */

                /* next block */
                CLCD_block++;
                    if (CLCD_block > 5)
                    {
                        CLCD_sync = FALSE; 
                        CLCD_block = 0;
                    }
        		CLCD_bit = 0;
                }
        }
        vTaskDelay(xDelay);
    }
}

/***************************************************
*  Decode the time data
****************************************************/
void BCD_decode_data (uint8 index)
{
	switch(index)
	{
		case 0:
			ct.nMinute = BCD2time(index);
			break;
		case 1:
			ct.nHour = BCD2time(index);
			break;
		case 3:
			ct.nDay = BCD2time(index - 1) + BCD2time(index);
			break;
		case 4:
			if ((ct.nMinute != 15) && (ct.nMinute != 45))
				ct.nYear = BCD2time(index);
			break;
		case 5:
			ct.nDay_of_week = BCD2time(index);
			break;
	}
}

/***************************************************
*  BCD code to Time data
****************************************************/
uint32 BCD2time (uint8 block)
{
        /* BCD code table */
    const uint8 BCD_table [nBlock][nBit] =
    {
    	{ 0, 40, 20, 10,  0, 8, 4, 2, 1, 0 },		// minute
    	{ 0,  0, 20, 10,  0, 8, 4, 2, 1, 0 },		// hour
    	{ 0,  0, 200, 100, 0, 80, 40, 20, 10, 0 },	// upper day
    	{ 8,  4,  2,  1,  0, 0, 0, 0, 0, 0 },		// lower day
    	{ 0, 80, 40, 20, 10, 8, 4, 2, 1, 0 },		// lower year (+2000)
    	{ 4,  2,  1,  0,  0, 0, 0, 0, 0, 0 }		// days of the week
    };
    
	uint32 time_code = 0;
	uint8 bit;

	for (bit = 0; bit < nBit; bit++)
	{
		if (tmp_buf[block][bit] == BIN_1)
			time_code += BCD_table[block][bit];
	}
	return time_code;
}

/***************************************************
*  Display the time on CLCD
****************************************************/
/*
* display format is "20YY/MM/DD HH:MM"
*                    0000000000111111
*                    0123456789012345  
*/
void display_time (uint8 block)
{
	char buf[32];	/* buffer */

	switch (block)
	{
		case 0:	/* minute */
			I2C_CLCD_Position(1, 14);
			sprintf(buf, "%02d", ct.nMinute);
			I2C_CLCD_PrintString(buf);
			break;
		case 1: /* hour */
			I2C_CLCD_Position(1, 11);
			sprintf(buf, "%02d:", ct.nHour);
			I2C_CLCD_PrintString(buf);
			break;
		case 3: /* calendar */
			nDay_to_calendar();
			I2C_CLCD_Position(1, 5);
			sprintf(buf, "%02d/%02d ", ct.month, ct.day);
			I2C_CLCD_PrintString(buf);
			break;
		case 4: /* year */
			I2C_CLCD_Position(1, 0);
			sprintf(buf, "20%02d/", ct.nYear);
			I2C_CLCD_PrintString(buf);
			break;
		case 5: /* day of the week */
			/* there is no space to use */
			break;
	}
}

/***************************************************
*  The day number in the year to calendar
****************************************************/
void nDay_to_calendar (void)
{
    //const char week[7][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    const unsigned int norm_month_Days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    const unsigned int leap_month_Days[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int i;
	int sum_of_month_days = 0;
	int leap_year = FALSE;
    unsigned int monthDays[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	if ((ct.nYear % 4) == 0)
		leap_year = TRUE;

    for (i = 1; i < 13; i++)
    {
        if (leap_year == FALSE)
            monthDays[i] = norm_month_Days[i];
        else
            monthDays[i] = leap_month_Days[i];
    }
    
	for(i = 1; i < 13; i++)
	{
		sum_of_month_days += monthDays[i];
		if( ct.nDay <= sum_of_month_days )
		{
			ct.month = i;
			ct.day = ct.nDay - (sum_of_month_days - monthDays[i]);
			break;
		}
	}
}


/* [] END OF FILE */
