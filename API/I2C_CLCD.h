/*******************************************************************************
* File Name: I2C_CLCD.h
* Version 1.0
*
* Description:
*  This header file contains registers and constants associated with the
*  I2C based Character LCD component.
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

#if !defined(CY_CHARI2C_CLCD_I2C_CLCD_H)
#define CY_CHARI2C_CLCD_I2C_CLCD_H

#include "cytypes.h"
#include "cyfitter.h"

/* I2C master module is supposed to use "I2C_1" */
#include "I2C_1.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define I2C_CLCD_CONVERSION_ROUTINES     (1u)
#define I2C_CLCD_CUSTOM_CHAR_SET         (0u)

/* Custom character set types */
#define I2C_CLCD_NONE                     (0u)    /* No Custom Fonts      */
#define I2C_CLCD_HORIZONTAL_BG            (1u)    /* Horizontal Bar Graph */
#define I2C_CLCD_VERTICAL_BG              (2u)    /* Vertical Bar Graph   */
#define I2C_CLCD_USER_DEFINED             (3u)    /* User Defined Fonts   */


/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} I2C_CLCD_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void I2C_CLCD_Init(void) ;
void I2C_CLCD_Enable(void) ;
void I2C_CLCD_Start(void) ;
void I2C_CLCD_Stop(void) ;
void I2C_CLCD_WriteControl(uint8 cByte) ;
void I2C_CLCD_WriteData(uint8 dByte) ;
void I2C_CLCD_PrintString(char8 const string[]) ;
void I2C_CLCD_Position(uint8 row, uint8 column) ;
void I2C_CLCD_PutChar(char8 character) ;
void I2C_CLCD_IsReady(void) ;
void I2C_CLCD_SaveConfig(void) ;
void I2C_CLCD_RestoreConfig(void) ;
void I2C_CLCD_Sleep(void) ;
void I2C_CLCD_Wakeup(void) ;

#if(I2C_CLCD_CONVERSION_ROUTINES == 1u)

    /* ASCII Conversion Routines */
    void I2C_CLCD_PrintInt8(uint8 value) ;
    void I2C_CLCD_PrintInt16(uint16 value) ;
    void I2C_CLCD_PrintInt32(uint32 value) ;
    void I2C_CLCD_PrintNumber(uint16 value) ; 
    void I2C_CLCD_PrintU32Number(uint32 value) ;
    
#endif /* I2C_CLCD_CONVERSION_ROUTINES == 1u */

/***************************************
*        Macros
***************************************/

/* Clear Macro */
#define I2C_CLCD_ClearDisplay() I2C_CLCD_WriteControl(I2C_CLCD_CLEAR_DISPLAY)

/* Off Macro */
#define I2C_CLCD_DisplayOff() I2C_CLCD_WriteControl(I2C_CLCD_DISPLAY_CURSOR_OFF)

/* On Macro */
#define I2C_CLCD_DisplayOn() I2C_CLCD_WriteControl(I2C_CLCD_DISPLAY_ON_CURSOR_OFF)

#define I2C_CLCD_PrintNumber(value) I2C_CLCD_PrintU32Number((uint16) (value))

/* Function-like macros for handling I2C MAster API */
#define I2C_CLCD_MasterSendStart(slaveAddress, R_nW)    \
                                        I2C_1_MasterSendStart(slaveAddress, R_nW)
#define I2C_CLCD_MasterWriteByte(value)	I2C_1_MasterWriteByte((uint8) (value)) 
#define I2C_CLCD_MasterSendStop()       I2C_1_MasterSendStop()

/***************************************
*           Global Variables
***************************************/

extern uint8 I2C_CLCD_initVar;
extern uint8 I2C_CLCD_enableState;


/***************************************
*           API Constants
***************************************/

/* Default I2C address in 7-bit mode */
#define I2C_LCD_DEFAULT_I2C_ADDRESS     (0x27u)

/* Full Byte Commands Sent as Two Nibbles */
#define I2C_CLCD_DISPLAY_8_BIT_INIT       (0x03u)
#define I2C_CLCD_DISPLAY_4_BIT_INIT       (0x02u)
#define I2C_CLCD_DISPLAY_CURSOR_OFF       (0x08u)
#define I2C_CLCD_CLEAR_DISPLAY            (0x01u)
#define I2C_CLCD_CURSOR_AUTO_INCR_ON      (0x06u)
#define I2C_CLCD_DISPLAY_CURSOR_ON        (0x0Eu)
#define I2C_CLCD_DISPLAY_2_LINES_5x10     (0x2Cu)
#define I2C_CLCD_DISPLAY_ON_CURSOR_OFF    (0x0Cu)

#define I2C_CLCD_RESET_CURSOR_POSITION    (0x03u)
#define I2C_CLCD_CURSOR_WINK              (0x0Du)
#define I2C_CLCD_CURSOR_BLINK             (0x0Fu)
#define I2C_CLCD_CURSOR_SH_LEFT           (0x10u)
#define I2C_CLCD_CURSOR_SH_RIGHT          (0x14u)
#define I2C_CLCD_DISPLAY_SCRL_LEFT        (0x18u)
#define I2C_CLCD_DISPLAY_SCRL_RIGHT       (0x1Eu)
#define I2C_CLCD_CURSOR_HOME              (0x02u)
#define I2C_CLCD_CURSOR_LEFT              (0x04u)
#define I2C_CLCD_CURSOR_RIGHT             (0x06u)

/* Point to Character Generator Ram 0 */
#define I2C_CLCD_CGRAM_0                  (0x40u)

/* Point to Display Data Ram 0 */
#define I2C_CLCD_DDRAM_0                  (0x80u)

/* LCD Characteristics */
#define I2C_CLCD_CHARACTER_WIDTH          (0x05u)
#define I2C_CLCD_CHARACTER_HEIGHT         (0x08u)

/* Nibble Offset and Mask */
#define I2C_CLCD_NIBBLE_SHIFT             (0x04u)
#define I2C_CLCD_NIBBLE_MASK              (0x0Fu)

#if(I2C_CLCD_CONVERSION_ROUTINES == 1u)
    #define I2C_CLCD_NUMBER_OF_REMAINDERS_U32 (0x0Au)
    #define I2C_CLCD_TEN                      (0x0Au)
    #define I2C_CLCD_8_BIT_SHIFT              (8u)
    #define I2C_CLCD_32_BIT_SHIFT             (32u)
    #define I2C_CLCD_ZERO_CHAR_ASCII          (48u)
#endif /* I2C_CLCD_CONVERSION_ROUTINES == 1u */

/* LCD Module Address Constants */
#define I2C_CLCD_ROW_0_START              (0x80u)
#define I2C_CLCD_ROW_1_START              (0xC0u)
#define I2C_CLCD_ROW_2_START              (0x94u)
#define I2C_CLCD_ROW_3_START              (0xD4u)

/* Custom Character References */
#define I2C_CLCD_CUSTOM_0                 (0x00u)
#define I2C_CLCD_CUSTOM_1                 (0x01u)
#define I2C_CLCD_CUSTOM_2                 (0x02u)
#define I2C_CLCD_CUSTOM_3                 (0x03u)
#define I2C_CLCD_CUSTOM_4                 (0x04u)
#define I2C_CLCD_CUSTOM_5                 (0x05u)
#define I2C_CLCD_CUSTOM_6                 (0x06u)
#define I2C_CLCD_CUSTOM_7                 (0x07u)

/* Other constants */
#define I2C_CLCD_BYTE_UPPER_NIBBLE_SHIFT  (0x04u)
#define I2C_CLCD_BYTE_LOWER_NIBBLE_MASK   (0x0Fu)
#define I2C_CLCD_U16_UPPER_BYTE_SHIFT     (0x08u)
#define I2C_CLCD_U16_LOWER_BYTE_MASK      (0xFFu)

#define I2C_CLCD_PORT_MASK                (0xFFu)


/***************************************
*       I2C Port Constants
***************************************/

#define I2C_WRITE                         (0x0u)
#define I2C_READ                          (0x1u)

/* The followings are the bit position of FC-113 I2C module */
#define I2C_CLCD_RS                       (0x01u)
#define I2C_CLCD_RW                       (0x02u)
#define I2C_CLCD_E                        (0x04u)
#define I2C_CLCD_BL                       (0x08u)
#define I2C_CLCD_READY_BIT                (0x80u)
#define I2C_CLCD_DATA_MASK                (0xF0u)
/* Set Data or Command on D4-D7 position */
#define I2C_CLCD_4BIT_DATA_SHIFT          (0x04u)


#endif /* CY_CHARI2C_CLCD_I2C_CLCD_H */


/* [] END OF FILE */
