/*******************************************************************************
* File Name: Reset.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Reset_H) /* Pins Reset_H */
#define CY_PINS_Reset_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Reset_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Reset_Write(uint8 value) ;
void    Reset_SetDriveMode(uint8 mode) ;
uint8   Reset_ReadDataReg(void) ;
uint8   Reset_Read(void) ;
uint8   Reset_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Reset_DRIVE_MODE_BITS        (3)
#define Reset_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Reset_DRIVE_MODE_BITS))

#define Reset_DM_ALG_HIZ         (0x00u)
#define Reset_DM_DIG_HIZ         (0x01u)
#define Reset_DM_RES_UP          (0x02u)
#define Reset_DM_RES_DWN         (0x03u)
#define Reset_DM_OD_LO           (0x04u)
#define Reset_DM_OD_HI           (0x05u)
#define Reset_DM_STRONG          (0x06u)
#define Reset_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Reset_MASK               Reset__MASK
#define Reset_SHIFT              Reset__SHIFT
#define Reset_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Reset_PS                     (* (reg32 *) Reset__PS)
/* Port Configuration */
#define Reset_PC                     (* (reg32 *) Reset__PC)
/* Data Register */
#define Reset_DR                     (* (reg32 *) Reset__DR)
/* Input Buffer Disable Override */
#define Reset_INP_DIS                (* (reg32 *) Reset__PC2)


#if defined(Reset__INTSTAT)  /* Interrupt Registers */

    #define Reset_INTSTAT                (* (reg32 *) Reset__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Reset_DRIVE_MODE_SHIFT       (0x00u)
#define Reset_DRIVE_MODE_MASK        (0x07u << Reset_DRIVE_MODE_SHIFT)


#endif /* End Pins Reset_H */


/* [] END OF FILE */
