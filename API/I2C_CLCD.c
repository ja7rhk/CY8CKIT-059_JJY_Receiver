/*******************************************************************************
* File Name: I2C_CLCD.c
* Version 1.0
*
* Description:
*  This file provides source code for the I2C CLCD component's API.
*  I2C_CLCD.c is midified LCD.c to use PCF8574 based I2C module for CLCD.
*  In this file, the original 'LCD_' is changed to 'I2C_CLCD_'
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/*******************************************************************************
* Copyright 2016, JA7RHK.   All rights reserved.
*******************************************************************************/

#include "CyLib.h"
#include "I2C_CLCD.h"


static void I2C_CLCD_WrDatNib(uint8 nibble) ;
static void I2C_CLCD_WrCntrlNib(uint8 nibble) ;

/* Stores state of component. Indicates whether component is or not
* in the enable state.
*/
uint8 I2C_CLCD_enableState = 0u;

uint8 I2C_CLCD_initVar = 0u;

/* Need to have I2C_CLCD_buffIndex of different size to avoid MISRA
* warning when SCB based I2C Master is used. 
*/

/* Variable stores the I2C address */
/* 7-bit slave address. */
uint8 slaveAddress = I2C_LCD_DEFAULT_I2C_ADDRESS;
uint8 I2C_CLCD_BackLight = I2C_CLCD_BL;


/*******************************************************************************
* Function Name: I2C_CLCD_Init
********************************************************************************
*
* Summary:
*  Perform initialization required for the components normal work.
*  This function initializes the LCD hardware module as follows:
*        Enables a 4-bit interface
*        Clears the display
*        Enables the auto cursor increment
*        Resets the cursor to start position
*  Also, it loads a custom character set to the LCD if it was defined in the customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_CLCD_Init(void) 
{
    /* INIT CODE */
    CyDelay(40u);                                                        /* Delay 40 ms */
    I2C_CLCD_WrCntrlNib(I2C_CLCD_DISPLAY_8_BIT_INIT);    /* Selects 8-bit mode */
    CyDelay(5u);                                                         /* Delay 5 ms */
    I2C_CLCD_WrCntrlNib(I2C_CLCD_DISPLAY_8_BIT_INIT);    /* Selects 8-bit mode */
    CyDelay(15u);                                                        /* Delay 15 ms */
    I2C_CLCD_WrCntrlNib(I2C_CLCD_DISPLAY_8_BIT_INIT);    /* Selects 8-bit mode */
    CyDelay(1u);                                                         /* Delay 1 ms */
    I2C_CLCD_WrCntrlNib(I2C_CLCD_DISPLAY_4_BIT_INIT);    /* Selects 4-bit mode */
    CyDelay(5u);                                                         /* Delay 5 ms */

    I2C_CLCD_WriteControl(I2C_CLCD_CURSOR_AUTO_INCR_ON);    /* Incr Cursor After Writes */
    I2C_CLCD_WriteControl(I2C_CLCD_DISPLAY_CURSOR_ON);      /* Turn Display, Cursor ON */
    I2C_CLCD_WriteControl(I2C_CLCD_DISPLAY_2_LINES_5x10);   /* 2 Lines by 5x10 Characters */
    I2C_CLCD_WriteControl(I2C_CLCD_DISPLAY_CURSOR_OFF);     /* Turn Display, Cursor OFF */
    I2C_CLCD_WriteControl(I2C_CLCD_CLEAR_DISPLAY);          /* Clear LCD Screen */
    I2C_CLCD_WriteControl(I2C_CLCD_DISPLAY_ON_CURSOR_OFF);  /* Turn Display ON, Cursor OFF */
    I2C_CLCD_WriteControl(I2C_CLCD_RESET_CURSOR_POSITION);  /* Set Cursor to 0,0 */
    CyDelay(5u);
}


/*******************************************************************************
* Function Name: I2C_CLCD_Enable
********************************************************************************
*
* Summary:
*  Turns on the display.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Theory:
*  This function has no effect when it is called the first time as
*  I2C_CLCD_Init() turns on the LCD.
*
*******************************************************************************/
void I2C_CLCD_Enable(void) 
{
    I2C_CLCD_DisplayOn();
    I2C_CLCD_enableState = 1u;
}


/*******************************************************************************
* Function Name: I2C_CLCD_Start
********************************************************************************
*
* Summary:
*  Perform initialization required for the components normal work.
*  This function initializes the LCD hardware module as follows:
*        Enables 4-bit interface
*        Clears the display
*        Enables auto cursor increment
*        Resets the cursor to start position
*  Also, it loads a custom character set to the LCD if it was defined in the customizer.
*  If it was not the first call in this project, then it just turns on the
*  display
*
*
* Parameters:
*  I2C_CLCD_initVar - global variable.
*
* Return:
*  I2C_CLCD_initVar - global variable.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_CLCD_Start(void) 
{
    /* If not initialized, perform initialization */
    if(I2C_CLCD_initVar == 0u)
    {
        I2C_CLCD_Init();
        I2C_CLCD_initVar = 1u;
    }

    /* Turn on the LCD */
    I2C_CLCD_Enable();
}


/*******************************************************************************
* Function Name: I2C_CLCD_Stop
********************************************************************************
*
* Summary:
*  Turns off the display of the LCD screen.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2C_CLCD_Stop(void) 
{
    /* Calls LCD Off Macro */
    I2C_CLCD_DisplayOff();
    I2C_CLCD_enableState = 0u;
}


/*******************************************************************************
*  Function Name: I2C_CLCD_Position
********************************************************************************
*
* Summary:
*  Moves the active cursor location to a point specified by the input arguments
*
* Parameters:
*  row:    Specific row of LCD module to be written
*  column: Column of LCD module to be written
*
* Return:
*  None.
*
* Note:
*  This only applies for LCD displays that use the 2X40 address mode.
*  In this case Row 2 starts with 0x28 offset from Row 1.
*  When there are more than 2 rows, each row must be fewer than 20 characters.
*
*******************************************************************************/
void I2C_CLCD_Position(uint8 row, uint8 column) 
{
    switch (row)
    {
        case (uint8)0:
            I2C_CLCD_WriteControl(I2C_CLCD_ROW_0_START + column);
            break;
        case (uint8) 1:
            I2C_CLCD_WriteControl(I2C_CLCD_ROW_1_START + column);
            break;
        case (uint8) 2:
            I2C_CLCD_WriteControl(I2C_CLCD_ROW_2_START + column);
            break;
        case (uint8) 3:
            I2C_CLCD_WriteControl(I2C_CLCD_ROW_3_START + column);
            break;
        default:
            /* if default case is hit, invalid row argument was passed.*/
            break;
    }
}


/*******************************************************************************
* Function Name: I2C_CLCD_PrintString
********************************************************************************
*
* Summary:
*  Writes a zero terminated string to the LCD.
*
* Parameters:
*  string: Pointer to head of char8 array to be written to the LCD module
*
* Return:
*  None.
*
*******************************************************************************/
void I2C_CLCD_PrintString(char8 const string[]) 
{
    uint8 indexU8 = 1u;
    char8 current = *string;

    /* Until null is reached, print next character */
    while((char8) '\0' != current)
    {
        I2C_CLCD_WriteData((uint8)current);
        current = string[indexU8];
        indexU8++;
    }
}


/*******************************************************************************
*  Function Name: I2C_CLCD_PutChar
********************************************************************************
*
* Summary:
*  Writes a single character to the current cursor position of the LCD module.
*  Custom character names (_CUSTOM_0 through
*  _CUSTOM_7) are acceptable as inputs.
*
* Parameters:
*  character: Character to be written to LCD
*
* Return:
*  None.
*
*******************************************************************************/
void I2C_CLCD_PutChar(char8 character) 
{
    I2C_CLCD_WriteData((uint8)character);
}


/*******************************************************************************
*  Function Name: I2C_CLCD_WriteData
********************************************************************************
*
* Summary:
*  Writes a data byte to the LCD module's Data Display RAM.
*
* Parameters:
*  dByte: Byte to be written to the LCD module
*
* Return:
*  None.
*
*******************************************************************************/
void I2C_CLCD_WriteData(uint8 dByte) 
{
    uint8 nibble;

    I2C_CLCD_IsReady();
    nibble = dByte >> I2C_CLCD_NIBBLE_SHIFT;

    /* Write high nibble */
    I2C_CLCD_WrDatNib(nibble);

    nibble = dByte & I2C_CLCD_NIBBLE_MASK;
    /* Write low nibble */
    I2C_CLCD_WrDatNib(nibble);
}


/*******************************************************************************
*  Function Name: I2C_CLCD_WriteControl
********************************************************************************
*
* Summary:
*  Writes a command byte to the LCD module.
*
* Parameters:
*  cByte:  The byte to be written to theLCD module
* 
* Return:
*  None.
*
*******************************************************************************/
void I2C_CLCD_WriteControl(uint8 cByte) 
{
    uint8 nibble;

    I2C_CLCD_IsReady();

    nibble = cByte >> I2C_CLCD_NIBBLE_SHIFT;
    
    /* WrCntrlNib(High Nibble) */
    I2C_CLCD_WrCntrlNib(nibble);
    nibble = cByte & I2C_CLCD_NIBBLE_MASK;

    /* WrCntrlNib(Low Nibble) */
    I2C_CLCD_WrCntrlNib(nibble);
}


/*******************************************************************************
* Function Name: I2C_CLCD_IsReady
********************************************************************************
*
* Summary:
*  Polls the LCD until the ready bit is set.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void I2C_CLCD_IsReady(void) 
{
    /*
    *  No need to use the delay routines since the time taken to write takes
    *  longer that what is needed both for toggling and enable pin an to execute
    *  the command.
    */
    CyDelayUs(1u);
}


/*******************************************************************************
*  Function Name: I2C_CLCD_WrDatNib
********************************************************************************
*
* Summary:
*  Writes a data nibble to the LCD module.
*
* Parameters:
*  nibble:  Byte containing nibble in least significant nibble to be written
*           to LCD module.
*
* Return:
*  None.
*
*******************************************************************************/
static void I2C_CLCD_WrDatNib(uint8 nibble) 
{
	uint8 data = I2C_CLCD_BackLight;

    I2C_CLCD_MasterSendStart(slaveAddress, I2C_WRITE);
    CyDelayUs(1u);

    /* RS should be low to select data register */
    data |= I2C_CLCD_RS;
	I2C_CLCD_MasterWriteByte(data);

    /* Reset RW for write operation */
    data &= ((uint8)(~I2C_CLCD_RW));
	I2C_CLCD_MasterWriteByte(data);

    /* Guaranteed delay between Setting RS and RW and setting E bits */
    CyDelayUs(0u);
    
    /* Clear data pins */
    data &= ((uint8)(~I2C_CLCD_DATA_MASK));
	I2C_CLCD_MasterWriteByte(data);

    /* Write data, bring E high */
    /* for FC-113 data position is 0xF0 */
    data |= (I2C_CLCD_E | (nibble << I2C_CLCD_4BIT_DATA_SHIFT));
 	I2C_CLCD_MasterWriteByte(data);

    /* Minimum of 230 ns delay */
    CyDelayUs(1u);

    data &= ((uint8)(~I2C_CLCD_E));
	I2C_CLCD_MasterWriteByte(data);

    CyDelayUs(1u);
    I2C_1_MasterSendStop();
}


/*******************************************************************************
*  Function Name: I2C_CLCD_WrCntrlNib
********************************************************************************
*
* Summary:
*  Writes a control nibble to the LCD module.
*
* Parameters:
*  nibble: The byte containing a nibble in the four least significant bits.????
*
* Return:
*  None.
*
*******************************************************************************/
static void I2C_CLCD_WrCntrlNib(uint8 nibble) 
{
	uint8 data = I2C_CLCD_BackLight;
    
    I2C_CLCD_MasterSendStart(slaveAddress, I2C_WRITE);
    CyDelayUs(1u);

    /* RS and RW should be low to select instruction register and  write operation respectively */
    data |= ((uint8)(~(I2C_CLCD_RS | I2C_CLCD_RW)));
	I2C_CLCD_MasterWriteByte(data);

    /* Two following lines of code will give 40ns delay */
    /* Clear data pins */
    data &= ((uint8)(~I2C_CLCD_DATA_MASK));
	I2C_CLCD_MasterWriteByte(data);

    /* for FC-113 data position is 0xF0 */
    data |= (I2C_CLCD_E | (nibble << I2C_CLCD_4BIT_DATA_SHIFT));
	I2C_CLCD_MasterWriteByte(data);

    /* Minimum of 230 ns delay */
    CyDelayUs(1u);

    data &= ((uint8)(~I2C_CLCD_E));
	I2C_CLCD_MasterWriteByte(data);
    
    CyDelayUs(1u);
    I2C_1_MasterSendStop();
}

#if(I2C_CLCD_CONVERSION_ROUTINES == 1u)

    /*******************************************************************************
    *  Function Name: I2C_CLCD_PrintInt8
    ********************************************************************************
    *
    * Summary:
    *  Print a byte as two ASCII characters.
    *
    * Parameters:
    *  value: The byte to be printed out as ASCII characters.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void I2C_CLCD_PrintInt8(uint8 value) 
    {
        static char8 const CYCODE I2C_CLCD_hex[16u] = "0123456789ABCDEF";
        
        I2C_CLCD_PutChar((char8) I2C_CLCD_hex[value >> I2C_CLCD_BYTE_UPPER_NIBBLE_SHIFT]);
        I2C_CLCD_PutChar((char8) I2C_CLCD_hex[value & I2C_CLCD_BYTE_LOWER_NIBBLE_MASK]);
    }


    /*******************************************************************************
    *  Function Name: I2C_CLCD_PrintInt16
    ********************************************************************************
    *
    * Summary:
    *  Print a uint16 as four ASCII characters.
    *
    * Parameters:
    *  value: The uint16 to be printed out as ASCII characters.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void I2C_CLCD_PrintInt16(uint16 value) 
    {
        I2C_CLCD_PrintInt8((uint8)(value >> I2C_CLCD_U16_UPPER_BYTE_SHIFT));
        I2C_CLCD_PrintInt8((uint8)(value & I2C_CLCD_U16_LOWER_BYTE_MASK));
    }

    
    /*******************************************************************************
    *  Function Name: I2C_CLCD_PrintInt32
    ********************************************************************************
    *
    * Summary:
    *  Print a uint32 hexadecimal number as eight ASCII characters.
    *
    * Parameters:
    *  value: The uint32 to be printed out as ASCII characters.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void I2C_CLCD_PrintInt32(uint32 value) 
    {
        uint8 shift = I2C_CLCD_32_BIT_SHIFT;
        
        while (shift != 0u)
        {
            /* "shift" var should be subtracted by 8 prior shifting. This implements 
            * shifting by 24, 16, 8 and 0u. 
            */
            shift -= I2C_CLCD_8_BIT_SHIFT;
            
            /* Print 8 bits of uint32 hex number */
            I2C_CLCD_PrintInt8((uint8) ((uint32) (value >> shift)));
        }
    }
    
    
    /*******************************************************************************
    *  Function Name: I2C_CLCD_PrintNumber
    ********************************************************************************
    *
    * Summary:
    *  Print an uint16 value as a left-justified decimal value.
    *
    * Parameters:
    *  value: A 16-bit value to be printed in ASCII characters as a decimal number
    *
    * Return:
    *  None.
    *
    * Note:
    *  This function is implemented as a macro.
    *
    *******************************************************************************/

    
    /*******************************************************************************
    *  Function Name: I2C_CLCD_PrintU32Number
    ********************************************************************************
    *
    * Summary:
    *  Print an uint32 value as a left-justified decimal value.
    *
    * Parameters:
    *  value: A 32-bit value to be printed in ASCII characters as a decimal number
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void I2C_CLCD_PrintU32Number(uint32 value) 
    {
        uint8 tmpDigit;
        char8 number[I2C_CLCD_NUMBER_OF_REMAINDERS_U32 + 1u];
        uint8 digIndex = I2C_CLCD_NUMBER_OF_REMAINDERS_U32;
        
        /* This API will output a decimal number as a string and that string will be 
        * filled from end to start. Set Null termination character first.
        */
        number[digIndex] = (char8) '\0';
        digIndex--;
        
        /* Load these in reverse order */
        while(value >= I2C_CLCD_TEN)
        {
            /* Extract decimal digit, indexed by 'digIndex', from 'value' and
            * convert it to ASCII character.
            */
            tmpDigit = (uint8) (((uint8) (value % I2C_CLCD_TEN)) + (uint8) I2C_CLCD_ZERO_CHAR_ASCII);
            
            /* Temporary variable 'tmpDigit' is used to avoid Violation of MISRA rule 
            * #10.3.
            */
            number[digIndex] = (char8) tmpDigit;
            value /= I2C_CLCD_TEN;
            digIndex--;
        }
        
        /* Extract last decimal digit 'digIndex', from the 'value' and convert it
        * to ASCII character.
        */
        tmpDigit = (uint8) (((uint8)(value % I2C_CLCD_TEN)) + (uint8) I2C_CLCD_ZERO_CHAR_ASCII);
        number[digIndex] = (char8) tmpDigit;

        /* Print out number */
        I2C_CLCD_PrintString(&number[digIndex]);
    }
    
#endif /* I2C_CLCD_CONVERSION_ROUTINES == 1u */

/* [] END OF FILE */
