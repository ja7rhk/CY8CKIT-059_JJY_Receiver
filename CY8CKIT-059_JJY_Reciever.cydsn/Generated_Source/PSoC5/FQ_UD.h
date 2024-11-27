/*******************************************************************************
* File Name: FQ_UD.h  
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

#if !defined(CY_PINS_FQ_UD_H) /* Pins FQ_UD_H */
#define CY_PINS_FQ_UD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "FQ_UD_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 FQ_UD__PORT == 15 && ((FQ_UD__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    FQ_UD_Write(uint8 value);
void    FQ_UD_SetDriveMode(uint8 mode);
uint8   FQ_UD_ReadDataReg(void);
uint8   FQ_UD_Read(void);
void    FQ_UD_SetInterruptMode(uint16 position, uint16 mode);
uint8   FQ_UD_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the FQ_UD_SetDriveMode() function.
     *  @{
     */
        #define FQ_UD_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define FQ_UD_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define FQ_UD_DM_RES_UP          PIN_DM_RES_UP
        #define FQ_UD_DM_RES_DWN         PIN_DM_RES_DWN
        #define FQ_UD_DM_OD_LO           PIN_DM_OD_LO
        #define FQ_UD_DM_OD_HI           PIN_DM_OD_HI
        #define FQ_UD_DM_STRONG          PIN_DM_STRONG
        #define FQ_UD_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define FQ_UD_MASK               FQ_UD__MASK
#define FQ_UD_SHIFT              FQ_UD__SHIFT
#define FQ_UD_WIDTH              1u

/* Interrupt constants */
#if defined(FQ_UD__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FQ_UD_SetInterruptMode() function.
     *  @{
     */
        #define FQ_UD_INTR_NONE      (uint16)(0x0000u)
        #define FQ_UD_INTR_RISING    (uint16)(0x0001u)
        #define FQ_UD_INTR_FALLING   (uint16)(0x0002u)
        #define FQ_UD_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define FQ_UD_INTR_MASK      (0x01u) 
#endif /* (FQ_UD__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define FQ_UD_PS                     (* (reg8 *) FQ_UD__PS)
/* Data Register */
#define FQ_UD_DR                     (* (reg8 *) FQ_UD__DR)
/* Port Number */
#define FQ_UD_PRT_NUM                (* (reg8 *) FQ_UD__PRT) 
/* Connect to Analog Globals */                                                  
#define FQ_UD_AG                     (* (reg8 *) FQ_UD__AG)                       
/* Analog MUX bux enable */
#define FQ_UD_AMUX                   (* (reg8 *) FQ_UD__AMUX) 
/* Bidirectional Enable */                                                        
#define FQ_UD_BIE                    (* (reg8 *) FQ_UD__BIE)
/* Bit-mask for Aliased Register Access */
#define FQ_UD_BIT_MASK               (* (reg8 *) FQ_UD__BIT_MASK)
/* Bypass Enable */
#define FQ_UD_BYP                    (* (reg8 *) FQ_UD__BYP)
/* Port wide control signals */                                                   
#define FQ_UD_CTL                    (* (reg8 *) FQ_UD__CTL)
/* Drive Modes */
#define FQ_UD_DM0                    (* (reg8 *) FQ_UD__DM0) 
#define FQ_UD_DM1                    (* (reg8 *) FQ_UD__DM1)
#define FQ_UD_DM2                    (* (reg8 *) FQ_UD__DM2) 
/* Input Buffer Disable Override */
#define FQ_UD_INP_DIS                (* (reg8 *) FQ_UD__INP_DIS)
/* LCD Common or Segment Drive */
#define FQ_UD_LCD_COM_SEG            (* (reg8 *) FQ_UD__LCD_COM_SEG)
/* Enable Segment LCD */
#define FQ_UD_LCD_EN                 (* (reg8 *) FQ_UD__LCD_EN)
/* Slew Rate Control */
#define FQ_UD_SLW                    (* (reg8 *) FQ_UD__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define FQ_UD_PRTDSI__CAPS_SEL       (* (reg8 *) FQ_UD__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define FQ_UD_PRTDSI__DBL_SYNC_IN    (* (reg8 *) FQ_UD__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define FQ_UD_PRTDSI__OE_SEL0        (* (reg8 *) FQ_UD__PRTDSI__OE_SEL0) 
#define FQ_UD_PRTDSI__OE_SEL1        (* (reg8 *) FQ_UD__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define FQ_UD_PRTDSI__OUT_SEL0       (* (reg8 *) FQ_UD__PRTDSI__OUT_SEL0) 
#define FQ_UD_PRTDSI__OUT_SEL1       (* (reg8 *) FQ_UD__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define FQ_UD_PRTDSI__SYNC_OUT       (* (reg8 *) FQ_UD__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(FQ_UD__SIO_CFG)
    #define FQ_UD_SIO_HYST_EN        (* (reg8 *) FQ_UD__SIO_HYST_EN)
    #define FQ_UD_SIO_REG_HIFREQ     (* (reg8 *) FQ_UD__SIO_REG_HIFREQ)
    #define FQ_UD_SIO_CFG            (* (reg8 *) FQ_UD__SIO_CFG)
    #define FQ_UD_SIO_DIFF           (* (reg8 *) FQ_UD__SIO_DIFF)
#endif /* (FQ_UD__SIO_CFG) */

/* Interrupt Registers */
#if defined(FQ_UD__INTSTAT)
    #define FQ_UD_INTSTAT            (* (reg8 *) FQ_UD__INTSTAT)
    #define FQ_UD_SNAP               (* (reg8 *) FQ_UD__SNAP)
    
	#define FQ_UD_0_INTTYPE_REG 		(* (reg8 *) FQ_UD__0__INTTYPE)
#endif /* (FQ_UD__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_FQ_UD_H */


/* [] END OF FILE */
