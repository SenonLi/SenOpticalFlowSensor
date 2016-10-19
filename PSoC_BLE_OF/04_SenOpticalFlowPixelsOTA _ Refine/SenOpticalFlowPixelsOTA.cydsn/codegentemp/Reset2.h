/*******************************************************************************
* File Name: Reset2.h  
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

#if !defined(CY_PINS_Reset2_H) /* Pins Reset2_H */
#define CY_PINS_Reset2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Reset2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Reset2_Write(uint8 value) ;
void    Reset2_SetDriveMode(uint8 mode) ;
uint8   Reset2_ReadDataReg(void) ;
uint8   Reset2_Read(void) ;
uint8   Reset2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Reset2_DRIVE_MODE_BITS        (3)
#define Reset2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Reset2_DRIVE_MODE_BITS))

#define Reset2_DM_ALG_HIZ         (0x00u)
#define Reset2_DM_DIG_HIZ         (0x01u)
#define Reset2_DM_RES_UP          (0x02u)
#define Reset2_DM_RES_DWN         (0x03u)
#define Reset2_DM_OD_LO           (0x04u)
#define Reset2_DM_OD_HI           (0x05u)
#define Reset2_DM_STRONG          (0x06u)
#define Reset2_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define Reset2_MASK               Reset2__MASK
#define Reset2_SHIFT              Reset2__SHIFT
#define Reset2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Reset2_PS                     (* (reg32 *) Reset2__PS)
/* Port Configuration */
#define Reset2_PC                     (* (reg32 *) Reset2__PC)
/* Data Register */
#define Reset2_DR                     (* (reg32 *) Reset2__DR)
/* Input Buffer Disable Override */
#define Reset2_INP_DIS                (* (reg32 *) Reset2__PC2)


#if defined(Reset2__INTSTAT)  /* Interrupt Registers */

    #define Reset2_INTSTAT                (* (reg32 *) Reset2__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define Reset2_DRIVE_MODE_SHIFT       (0x00u)
#define Reset2_DRIVE_MODE_MASK        (0x07u << Reset2_DRIVE_MODE_SHIFT)


#endif /* End Pins Reset2_H */


/* [] END OF FILE */
