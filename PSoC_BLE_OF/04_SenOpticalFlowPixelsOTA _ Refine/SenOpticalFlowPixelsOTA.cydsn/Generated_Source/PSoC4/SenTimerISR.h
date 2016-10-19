/*******************************************************************************
* File Name: SenTimerISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_SenTimerISR_H)
#define CY_ISR_SenTimerISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void SenTimerISR_Start(void);
void SenTimerISR_StartEx(cyisraddress address);
void SenTimerISR_Stop(void);

CY_ISR_PROTO(SenTimerISR_Interrupt);

void SenTimerISR_SetVector(cyisraddress address);
cyisraddress SenTimerISR_GetVector(void);

void SenTimerISR_SetPriority(uint8 priority);
uint8 SenTimerISR_GetPriority(void);

void SenTimerISR_Enable(void);
uint8 SenTimerISR_GetState(void);
void SenTimerISR_Disable(void);

void SenTimerISR_SetPending(void);
void SenTimerISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the SenTimerISR ISR. */
#define SenTimerISR_INTC_VECTOR            ((reg32 *) SenTimerISR__INTC_VECT)

/* Address of the SenTimerISR ISR priority. */
#define SenTimerISR_INTC_PRIOR             ((reg32 *) SenTimerISR__INTC_PRIOR_REG)

/* Priority of the SenTimerISR interrupt. */
#define SenTimerISR_INTC_PRIOR_NUMBER      SenTimerISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable SenTimerISR interrupt. */
#define SenTimerISR_INTC_SET_EN            ((reg32 *) SenTimerISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the SenTimerISR interrupt. */
#define SenTimerISR_INTC_CLR_EN            ((reg32 *) SenTimerISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the SenTimerISR interrupt state to pending. */
#define SenTimerISR_INTC_SET_PD            ((reg32 *) SenTimerISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the SenTimerISR interrupt. */
#define SenTimerISR_INTC_CLR_PD            ((reg32 *) SenTimerISR__INTC_CLR_PD_REG)



#endif /* CY_ISR_SenTimerISR_H */


/* [] END OF FILE */
