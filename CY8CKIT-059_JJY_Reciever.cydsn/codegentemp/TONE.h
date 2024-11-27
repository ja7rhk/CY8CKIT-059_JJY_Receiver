/*******************************************************************************
* File Name: TONE.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_TONE_H) /* Pins TONE_H */
#define CY_PINS_TONE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TONE_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TONE__PORT == 15 && ((TONE__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TONE_Write(uint8 value);
void    TONE_SetDriveMode(uint8 mode);
uint8   TONE_ReadDataReg(void);
uint8   TONE_Read(void);
void    TONE_SetInterruptMode(uint16 position, uint16 mode);
uint8   TONE_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TONE_SetDriveMode() function.
     *  @{
     */
        #define TONE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TONE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TONE_DM_RES_UP          PIN_DM_RES_UP
        #define TONE_DM_RES_DWN         PIN_DM_RES_DWN
        #define TONE_DM_OD_LO           PIN_DM_OD_LO
        #define TONE_DM_OD_HI           PIN_DM_OD_HI
        #define TONE_DM_STRONG          PIN_DM_STRONG
        #define TONE_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TONE_MASK               TONE__MASK
#define TONE_SHIFT              TONE__SHIFT
#define TONE_WIDTH              1u

/* Interrupt constants */
#if defined(TONE__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TONE_SetInterruptMode() function.
     *  @{
     */
        #define TONE_INTR_NONE      (uint16)(0x0000u)
        #define TONE_INTR_RISING    (uint16)(0x0001u)
        #define TONE_INTR_FALLING   (uint16)(0x0002u)
        #define TONE_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TONE_INTR_MASK      (0x01u) 
#endif /* (TONE__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TONE_PS                     (* (reg8 *) TONE__PS)
/* Data Register */
#define TONE_DR                     (* (reg8 *) TONE__DR)
/* Port Number */
#define TONE_PRT_NUM                (* (reg8 *) TONE__PRT) 
/* Connect to Analog Globals */                                                  
#define TONE_AG                     (* (reg8 *) TONE__AG)                       
/* Analog MUX bux enable */
#define TONE_AMUX                   (* (reg8 *) TONE__AMUX) 
/* Bidirectional Enable */                                                        
#define TONE_BIE                    (* (reg8 *) TONE__BIE)
/* Bit-mask for Aliased Register Access */
#define TONE_BIT_MASK               (* (reg8 *) TONE__BIT_MASK)
/* Bypass Enable */
#define TONE_BYP                    (* (reg8 *) TONE__BYP)
/* Port wide control signals */                                                   
#define TONE_CTL                    (* (reg8 *) TONE__CTL)
/* Drive Modes */
#define TONE_DM0                    (* (reg8 *) TONE__DM0) 
#define TONE_DM1                    (* (reg8 *) TONE__DM1)
#define TONE_DM2                    (* (reg8 *) TONE__DM2) 
/* Input Buffer Disable Override */
#define TONE_INP_DIS                (* (reg8 *) TONE__INP_DIS)
/* LCD Common or Segment Drive */
#define TONE_LCD_COM_SEG            (* (reg8 *) TONE__LCD_COM_SEG)
/* Enable Segment LCD */
#define TONE_LCD_EN                 (* (reg8 *) TONE__LCD_EN)
/* Slew Rate Control */
#define TONE_SLW                    (* (reg8 *) TONE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TONE_PRTDSI__CAPS_SEL       (* (reg8 *) TONE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TONE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TONE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TONE_PRTDSI__OE_SEL0        (* (reg8 *) TONE__PRTDSI__OE_SEL0) 
#define TONE_PRTDSI__OE_SEL1        (* (reg8 *) TONE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TONE_PRTDSI__OUT_SEL0       (* (reg8 *) TONE__PRTDSI__OUT_SEL0) 
#define TONE_PRTDSI__OUT_SEL1       (* (reg8 *) TONE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TONE_PRTDSI__SYNC_OUT       (* (reg8 *) TONE__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TONE__SIO_CFG)
    #define TONE_SIO_HYST_EN        (* (reg8 *) TONE__SIO_HYST_EN)
    #define TONE_SIO_REG_HIFREQ     (* (reg8 *) TONE__SIO_REG_HIFREQ)
    #define TONE_SIO_CFG            (* (reg8 *) TONE__SIO_CFG)
    #define TONE_SIO_DIFF           (* (reg8 *) TONE__SIO_DIFF)
#endif /* (TONE__SIO_CFG) */

/* Interrupt Registers */
#if defined(TONE__INTSTAT)
    #define TONE_INTSTAT            (* (reg8 *) TONE__INTSTAT)
    #define TONE_SNAP               (* (reg8 *) TONE__SNAP)
    
	#define TONE_0_INTTYPE_REG 		(* (reg8 *) TONE__0__INTTYPE)
#endif /* (TONE__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TONE_H */


/* [] END OF FILE */
