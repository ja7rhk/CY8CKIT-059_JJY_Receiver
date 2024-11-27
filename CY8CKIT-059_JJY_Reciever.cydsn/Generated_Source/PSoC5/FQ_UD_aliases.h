/*******************************************************************************
* File Name: FQ_UD.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_FQ_UD_ALIASES_H) /* Pins FQ_UD_ALIASES_H */
#define CY_PINS_FQ_UD_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define FQ_UD_0			(FQ_UD__0__PC)
#define FQ_UD_0_INTR	((uint16)((uint16)0x0001u << FQ_UD__0__SHIFT))

#define FQ_UD_INTR_ALL	 ((uint16)(FQ_UD_0_INTR))

#endif /* End Pins FQ_UD_ALIASES_H */


/* [] END OF FILE */
