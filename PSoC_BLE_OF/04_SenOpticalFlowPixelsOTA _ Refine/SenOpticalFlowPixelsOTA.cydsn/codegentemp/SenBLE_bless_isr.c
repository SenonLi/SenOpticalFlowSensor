/*******************************************************************************
* File Name: SenBLE_bless_isr.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <SenBLE_bless_isr.h>
#include "cyapicallbacks.h"

#if !defined(SenBLE_bless_isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START SenBLE_bless_isr_intc` */

/* `#END` */

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: SenBLE_bless_isr_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_Start(void)
{
    /* For all we know the interrupt is active. */
    SenBLE_bless_isr_Disable();

    /* Set the ISR to point to the SenBLE_bless_isr Interrupt. */
    SenBLE_bless_isr_SetVector(&SenBLE_bless_isr_Interrupt);

    /* Set the priority. */
    SenBLE_bless_isr_SetPriority((uint8)SenBLE_bless_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    SenBLE_bless_isr_Enable();
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    SenBLE_bless_isr_Disable();

    /* Set the ISR to point to the SenBLE_bless_isr Interrupt. */
    SenBLE_bless_isr_SetVector(address);

    /* Set the priority. */
    SenBLE_bless_isr_SetPriority((uint8)SenBLE_bless_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    SenBLE_bless_isr_Enable();
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_Stop(void)
{
    /* Disable this interrupt. */
    SenBLE_bless_isr_Disable();

    /* Set the ISR to point to the passive one. */
    SenBLE_bless_isr_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for SenBLE_bless_isr.
*
*   Add custom code between the START and END comments to keep the next version
*   of this file from over-writing your code.
*
*   Note You may use either the default ISR by using this API, or you may define
*   your own separate ISR through ISR_StartEx().
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(SenBLE_bless_isr_Interrupt)
{
    #ifdef SenBLE_bless_isr_INTERRUPT_INTERRUPT_CALLBACK
        SenBLE_bless_isr_Interrupt_InterruptCallback();
    #endif /* SenBLE_bless_isr_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START SenBLE_bless_isr_Interrupt` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling SenBLE_bless_isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use SenBLE_bless_isr_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_SetVector(cyisraddress address)
{
    CyRamVectors[CYINT_IRQ_BASE + SenBLE_bless_isr__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress SenBLE_bless_isr_GetVector(void)
{
    return CyRamVectors[CYINT_IRQ_BASE + SenBLE_bless_isr__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling SenBLE_bless_isr_Start or SenBLE_bless_isr_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after SenBLE_bless_isr_Start or SenBLE_bless_isr_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_SetPriority(uint8 priority)
{
	uint8 interruptState;
    uint32 priorityOffset = ((SenBLE_bless_isr__INTC_NUMBER % 4u) * 8u) + 6u;
    
	interruptState = CyEnterCriticalSection();
    *SenBLE_bless_isr_INTC_PRIOR = (*SenBLE_bless_isr_INTC_PRIOR & (uint32)(~SenBLE_bless_isr__INTC_PRIOR_MASK)) |
                                    ((uint32)priority << priorityOffset);
	CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 SenBLE_bless_isr_GetPriority(void)
{
    uint32 priority;
	uint32 priorityOffset = ((SenBLE_bless_isr__INTC_NUMBER % 4u) * 8u) + 6u;

    priority = (*SenBLE_bless_isr_INTC_PRIOR & SenBLE_bless_isr__INTC_PRIOR_MASK) >> priorityOffset;

    return (uint8)priority;
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_Enable(void)
{
    /* Enable the general interrupt. */
    *SenBLE_bless_isr_INTC_SET_EN = SenBLE_bless_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 SenBLE_bless_isr_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*SenBLE_bless_isr_INTC_SET_EN & (uint32)SenBLE_bless_isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_Disable(void)
{
    /* Disable the general interrupt. */
    *SenBLE_bless_isr_INTC_CLR_EN = SenBLE_bless_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void SenBLE_bless_isr_SetPending(void)
{
    *SenBLE_bless_isr_INTC_SET_PD = SenBLE_bless_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: SenBLE_bless_isr_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void SenBLE_bless_isr_ClearPending(void)
{
    *SenBLE_bless_isr_INTC_CLR_PD = SenBLE_bless_isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
