/*******************************************************************************
* File Name: SenTimer_PM.c
* Version 2.0
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SenTimer.h"

static SenTimer_BACKUP_STRUCT SenTimer_backup;


/*******************************************************************************
* Function Name: SenTimer_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: SenTimer_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_Sleep(void)
{
    if(0u != (SenTimer_BLOCK_CONTROL_REG & SenTimer_MASK))
    {
        SenTimer_backup.enableState = 1u;
    }
    else
    {
        SenTimer_backup.enableState = 0u;
    }

    SenTimer_Stop();
    SenTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: SenTimer_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: SenTimer_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_Wakeup(void)
{
    SenTimer_RestoreConfig();

    if(0u != SenTimer_backup.enableState)
    {
        SenTimer_Enable();
    }
}


/* [] END OF FILE */
