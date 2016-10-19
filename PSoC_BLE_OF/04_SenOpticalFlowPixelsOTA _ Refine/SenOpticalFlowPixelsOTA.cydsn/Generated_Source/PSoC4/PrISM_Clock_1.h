/*******************************************************************************
* File Name: PrISM_Clock_1.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_PrISM_Clock_1_H)
#define CY_CLOCK_PrISM_Clock_1_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void PrISM_Clock_1_StartEx(uint32 alignClkDiv);
#define PrISM_Clock_1_Start() \
    PrISM_Clock_1_StartEx(PrISM_Clock_1__PA_DIV_ID)

#else

void PrISM_Clock_1_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void PrISM_Clock_1_Stop(void);

void PrISM_Clock_1_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 PrISM_Clock_1_GetDividerRegister(void);
uint8  PrISM_Clock_1_GetFractionalDividerRegister(void);

#define PrISM_Clock_1_Enable()                         PrISM_Clock_1_Start()
#define PrISM_Clock_1_Disable()                        PrISM_Clock_1_Stop()
#define PrISM_Clock_1_SetDividerRegister(clkDivider, reset)  \
    PrISM_Clock_1_SetFractionalDividerRegister((clkDivider), 0u)
#define PrISM_Clock_1_SetDivider(clkDivider)           PrISM_Clock_1_SetDividerRegister((clkDivider), 1u)
#define PrISM_Clock_1_SetDividerValue(clkDivider)      PrISM_Clock_1_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define PrISM_Clock_1_DIV_ID     PrISM_Clock_1__DIV_ID

#define PrISM_Clock_1_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define PrISM_Clock_1_CTRL_REG   (*(reg32 *)PrISM_Clock_1__CTRL_REGISTER)
#define PrISM_Clock_1_DIV_REG    (*(reg32 *)PrISM_Clock_1__DIV_REGISTER)

#define PrISM_Clock_1_CMD_DIV_SHIFT          (0u)
#define PrISM_Clock_1_CMD_PA_DIV_SHIFT       (8u)
#define PrISM_Clock_1_CMD_DISABLE_SHIFT      (30u)
#define PrISM_Clock_1_CMD_ENABLE_SHIFT       (31u)

#define PrISM_Clock_1_CMD_DISABLE_MASK       ((uint32)((uint32)1u << PrISM_Clock_1_CMD_DISABLE_SHIFT))
#define PrISM_Clock_1_CMD_ENABLE_MASK        ((uint32)((uint32)1u << PrISM_Clock_1_CMD_ENABLE_SHIFT))

#define PrISM_Clock_1_DIV_FRAC_MASK  (0x000000F8u)
#define PrISM_Clock_1_DIV_FRAC_SHIFT (3u)
#define PrISM_Clock_1_DIV_INT_MASK   (0xFFFFFF00u)
#define PrISM_Clock_1_DIV_INT_SHIFT  (8u)

#else 

#define PrISM_Clock_1_DIV_REG        (*(reg32 *)PrISM_Clock_1__REGISTER)
#define PrISM_Clock_1_ENABLE_REG     PrISM_Clock_1_DIV_REG
#define PrISM_Clock_1_DIV_FRAC_MASK  PrISM_Clock_1__FRAC_MASK
#define PrISM_Clock_1_DIV_FRAC_SHIFT (16u)
#define PrISM_Clock_1_DIV_INT_MASK   PrISM_Clock_1__DIVIDER_MASK
#define PrISM_Clock_1_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_PrISM_Clock_1_H) */

/* [] END OF FILE */
