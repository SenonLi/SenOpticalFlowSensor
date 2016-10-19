/*******************************************************************************
* File Name: SenTimer.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SenTimer
*  component
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

uint8 SenTimer_initVar = 0u;


/*******************************************************************************
* Function Name: SenTimer_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default SenTimer configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (SenTimer__QUAD == SenTimer_CONFIG)
        SenTimer_CONTROL_REG = SenTimer_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        SenTimer_TRIG_CONTROL1_REG  = SenTimer_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        SenTimer_SetInterruptMode(SenTimer_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        SenTimer_SetCounterMode(SenTimer_COUNT_DOWN);
        SenTimer_WritePeriod(SenTimer_QUAD_PERIOD_INIT_VALUE);
        SenTimer_WriteCounter(SenTimer_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (SenTimer__QUAD == SenTimer_CONFIG) */

    #if (SenTimer__TIMER == SenTimer_CONFIG)
        SenTimer_CONTROL_REG = SenTimer_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        SenTimer_TRIG_CONTROL1_REG  = SenTimer_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        SenTimer_SetInterruptMode(SenTimer_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        SenTimer_WritePeriod(SenTimer_TC_PERIOD_VALUE );

        #if (SenTimer__COMPARE == SenTimer_TC_COMP_CAP_MODE)
            SenTimer_WriteCompare(SenTimer_TC_COMPARE_VALUE);

            #if (1u == SenTimer_TC_COMPARE_SWAP)
                SenTimer_SetCompareSwap(1u);
                SenTimer_WriteCompareBuf(SenTimer_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == SenTimer_TC_COMPARE_SWAP) */
        #endif  /* (SenTimer__COMPARE == SenTimer_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (SenTimer_CY_TCPWM_V2 && SenTimer_TIMER_UPDOWN_CNT_USED && !SenTimer_CY_TCPWM_4000)
            SenTimer_WriteCounter(1u);
        #elif(SenTimer__COUNT_DOWN == SenTimer_TC_COUNTER_MODE)
            SenTimer_WriteCounter(SenTimer_TC_PERIOD_VALUE);
        #else
            SenTimer_WriteCounter(0u);
        #endif /* (SenTimer_CY_TCPWM_V2 && SenTimer_TIMER_UPDOWN_CNT_USED && !SenTimer_CY_TCPWM_4000) */
    #endif  /* (SenTimer__TIMER == SenTimer_CONFIG) */

    #if (SenTimer__PWM_SEL == SenTimer_CONFIG)
        SenTimer_CONTROL_REG = SenTimer_CTRL_PWM_BASE_CONFIG;

        #if (SenTimer__PWM_PR == SenTimer_PWM_MODE)
            SenTimer_CONTROL_REG |= SenTimer_CTRL_PWM_RUN_MODE;
            SenTimer_WriteCounter(SenTimer_PWM_PR_INIT_VALUE);
        #else
            SenTimer_CONTROL_REG |= SenTimer_CTRL_PWM_ALIGN | SenTimer_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (SenTimer_CY_TCPWM_V2 && SenTimer_PWM_UPDOWN_CNT_USED && !SenTimer_CY_TCPWM_4000)
                SenTimer_WriteCounter(1u);
            #elif (SenTimer__RIGHT == SenTimer_PWM_ALIGN)
                SenTimer_WriteCounter(SenTimer_PWM_PERIOD_VALUE);
            #else 
                SenTimer_WriteCounter(0u);
            #endif  /* (SenTimer_CY_TCPWM_V2 && SenTimer_PWM_UPDOWN_CNT_USED && !SenTimer_CY_TCPWM_4000) */
        #endif  /* (SenTimer__PWM_PR == SenTimer_PWM_MODE) */

        #if (SenTimer__PWM_DT == SenTimer_PWM_MODE)
            SenTimer_CONTROL_REG |= SenTimer_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (SenTimer__PWM_DT == SenTimer_PWM_MODE) */

        #if (SenTimer__PWM == SenTimer_PWM_MODE)
            SenTimer_CONTROL_REG |= SenTimer_CTRL_PWM_PRESCALER;
        #endif  /* (SenTimer__PWM == SenTimer_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        SenTimer_TRIG_CONTROL1_REG  = SenTimer_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        SenTimer_SetInterruptMode(SenTimer_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (SenTimer__PWM_PR == SenTimer_PWM_MODE)
            SenTimer_TRIG_CONTROL2_REG =
                    (SenTimer_CC_MATCH_NO_CHANGE    |
                    SenTimer_OVERLOW_NO_CHANGE      |
                    SenTimer_UNDERFLOW_NO_CHANGE);
        #else
            #if (SenTimer__LEFT == SenTimer_PWM_ALIGN)
                SenTimer_TRIG_CONTROL2_REG = SenTimer_PWM_MODE_LEFT;
            #endif  /* ( SenTimer_PWM_LEFT == SenTimer_PWM_ALIGN) */

            #if (SenTimer__RIGHT == SenTimer_PWM_ALIGN)
                SenTimer_TRIG_CONTROL2_REG = SenTimer_PWM_MODE_RIGHT;
            #endif  /* ( SenTimer_PWM_RIGHT == SenTimer_PWM_ALIGN) */

            #if (SenTimer__CENTER == SenTimer_PWM_ALIGN)
                SenTimer_TRIG_CONTROL2_REG = SenTimer_PWM_MODE_CENTER;
            #endif  /* ( SenTimer_PWM_CENTER == SenTimer_PWM_ALIGN) */

            #if (SenTimer__ASYMMETRIC == SenTimer_PWM_ALIGN)
                SenTimer_TRIG_CONTROL2_REG = SenTimer_PWM_MODE_ASYM;
            #endif  /* (SenTimer__ASYMMETRIC == SenTimer_PWM_ALIGN) */
        #endif  /* (SenTimer__PWM_PR == SenTimer_PWM_MODE) */

        /* Set other values from customizer */
        SenTimer_WritePeriod(SenTimer_PWM_PERIOD_VALUE );
        SenTimer_WriteCompare(SenTimer_PWM_COMPARE_VALUE);

        #if (1u == SenTimer_PWM_COMPARE_SWAP)
            SenTimer_SetCompareSwap(1u);
            SenTimer_WriteCompareBuf(SenTimer_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == SenTimer_PWM_COMPARE_SWAP) */

        #if (1u == SenTimer_PWM_PERIOD_SWAP)
            SenTimer_SetPeriodSwap(1u);
            SenTimer_WritePeriodBuf(SenTimer_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == SenTimer_PWM_PERIOD_SWAP) */
    #endif  /* (SenTimer__PWM_SEL == SenTimer_CONFIG) */
    
}


/*******************************************************************************
* Function Name: SenTimer_Enable
********************************************************************************
*
* Summary:
*  Enables the SenTimer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    SenTimer_BLOCK_CONTROL_REG |= SenTimer_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (SenTimer__PWM_SEL == SenTimer_CONFIG)
        #if (0u == SenTimer_PWM_START_SIGNAL_PRESENT)
            SenTimer_TriggerCommand(SenTimer_MASK, SenTimer_CMD_START);
        #endif /* (0u == SenTimer_PWM_START_SIGNAL_PRESENT) */
    #endif /* (SenTimer__PWM_SEL == SenTimer_CONFIG) */

    #if (SenTimer__TIMER == SenTimer_CONFIG)
        #if (0u == SenTimer_TC_START_SIGNAL_PRESENT)
            SenTimer_TriggerCommand(SenTimer_MASK, SenTimer_CMD_START);
        #endif /* (0u == SenTimer_TC_START_SIGNAL_PRESENT) */
    #endif /* (SenTimer__TIMER == SenTimer_CONFIG) */
}


/*******************************************************************************
* Function Name: SenTimer_Start
********************************************************************************
*
* Summary:
*  Initializes the SenTimer with default customizer
*  values when called the first time and enables the SenTimer.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SenTimer_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time SenTimer_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the SenTimer_Start() routine.
*
*******************************************************************************/
void SenTimer_Start(void)
{
    if (0u == SenTimer_initVar)
    {
        SenTimer_Init();
        SenTimer_initVar = 1u;
    }

    SenTimer_Enable();
}


/*******************************************************************************
* Function Name: SenTimer_Stop
********************************************************************************
*
* Summary:
*  Disables the SenTimer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_BLOCK_CONTROL_REG &= (uint32)~SenTimer_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the SenTimer. This function is used when
*  configured as a generic SenTimer and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the SenTimer to operate in
*   Values:
*   - SenTimer_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - SenTimer_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - SenTimer_MODE_QUAD - Quadrature decoder
*         - SenTimer_MODE_PWM - PWM
*         - SenTimer_MODE_PWM_DT - PWM with dead time
*         - SenTimer_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_MODE_MASK;
    SenTimer_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - SenTimer_MODE_X1 - Counts on phi 1 rising
*         - SenTimer_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - SenTimer_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_QUAD_MODE_MASK;
    SenTimer_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - SenTimer_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - SenTimer_PRESCALE_DIVBY2    - Divide by 2
*         - SenTimer_PRESCALE_DIVBY4    - Divide by 4
*         - SenTimer_PRESCALE_DIVBY8    - Divide by 8
*         - SenTimer_PRESCALE_DIVBY16   - Divide by 16
*         - SenTimer_PRESCALE_DIVBY32   - Divide by 32
*         - SenTimer_PRESCALE_DIVBY64   - Divide by 64
*         - SenTimer_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_PRESCALER_MASK;
    SenTimer_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the SenTimer runs
*  continuously or stops when terminal count is reached.  By default the
*  SenTimer operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_ONESHOT_MASK;
    SenTimer_CONTROL_REG |= ((uint32)((oneShotEnable & SenTimer_1BIT_MASK) <<
                                                               SenTimer_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPWMMode(uint32 modeMask)
{
    SenTimer_TRIG_CONTROL2_REG = (modeMask & SenTimer_6BIT_MASK);
}



/*******************************************************************************
* Function Name: SenTimer_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_PWM_SYNC_KILL_MASK;
    SenTimer_CONTROL_REG |= ((uint32)((syncKillEnable & SenTimer_1BIT_MASK)  <<
                                               SenTimer_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_PWM_STOP_KILL_MASK;
    SenTimer_CONTROL_REG |= ((uint32)((stopOnKillEnable & SenTimer_1BIT_MASK)  <<
                                                         SenTimer_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_PRESCALER_MASK;
    SenTimer_CONTROL_REG |= ((uint32)((deadTime & SenTimer_8BIT_MASK) <<
                                                          SenTimer_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - SenTimer_INVERT_LINE   - Inverts the line output
*         - SenTimer_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_INV_OUT_MASK;
    SenTimer_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: SenTimer_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_WriteCounter(uint32 count)
{
    SenTimer_COUNTER_REG = (count & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 SenTimer_ReadCounter(void)
{
    return (SenTimer_COUNTER_REG & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - SenTimer_COUNT_UP       - Counts up
*     - SenTimer_COUNT_DOWN     - Counts down
*     - SenTimer_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - SenTimer_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_UPDOWN_MASK;
    SenTimer_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_WritePeriod(uint32 period)
{
    SenTimer_PERIOD_REG = (period & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 SenTimer_ReadPeriod(void)
{
    return (SenTimer_PERIOD_REG & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_RELOAD_CC_MASK;
    SenTimer_CONTROL_REG |= (swapEnable & SenTimer_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_WritePeriodBuf(uint32 periodBuf)
{
    SenTimer_PERIOD_BUF_REG = (periodBuf & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 SenTimer_ReadPeriodBuf(void)
{
    return (SenTimer_PERIOD_BUF_REG & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_CONTROL_REG &= (uint32)~SenTimer_RELOAD_PERIOD_MASK;
    SenTimer_CONTROL_REG |= ((uint32)((swapEnable & SenTimer_1BIT_MASK) <<
                                                            SenTimer_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_WriteCompare(uint32 compare)
{
    #if (SenTimer_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (SenTimer_CY_TCPWM_4000) */

    #if (SenTimer_CY_TCPWM_4000)
        currentMode = ((SenTimer_CONTROL_REG & SenTimer_UPDOWN_MASK) >> SenTimer_UPDOWN_SHIFT);

        if (((uint32)SenTimer__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)SenTimer__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (SenTimer_CY_TCPWM_4000) */
    
    SenTimer_COMP_CAP_REG = (compare & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
*******************************************************************************/
uint32 SenTimer_ReadCompare(void)
{
    #if (SenTimer_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (SenTimer_CY_TCPWM_4000) */

    #if (SenTimer_CY_TCPWM_4000)
        currentMode = ((SenTimer_CONTROL_REG & SenTimer_UPDOWN_MASK) >> SenTimer_UPDOWN_SHIFT);
        
        regVal = SenTimer_COMP_CAP_REG;
        
        if (((uint32)SenTimer__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)SenTimer__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & SenTimer_16BIT_MASK);
    #else
        return (SenTimer_COMP_CAP_REG & SenTimer_16BIT_MASK);
    #endif /* (SenTimer_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: SenTimer_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_WriteCompareBuf(uint32 compareBuf)
{
    #if (SenTimer_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (SenTimer_CY_TCPWM_4000) */

    #if (SenTimer_CY_TCPWM_4000)
        currentMode = ((SenTimer_CONTROL_REG & SenTimer_UPDOWN_MASK) >> SenTimer_UPDOWN_SHIFT);

        if (((uint32)SenTimer__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)SenTimer__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (SenTimer_CY_TCPWM_4000) */
    
    SenTimer_COMP_CAP_BUF_REG = (compareBuf & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
*******************************************************************************/
uint32 SenTimer_ReadCompareBuf(void)
{
    #if (SenTimer_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (SenTimer_CY_TCPWM_4000) */

    #if (SenTimer_CY_TCPWM_4000)
        currentMode = ((SenTimer_CONTROL_REG & SenTimer_UPDOWN_MASK) >> SenTimer_UPDOWN_SHIFT);

        regVal = SenTimer_COMP_CAP_BUF_REG;
        
        if (((uint32)SenTimer__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)SenTimer__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & SenTimer_16BIT_MASK);
    #else
        return (SenTimer_COMP_CAP_BUF_REG & SenTimer_16BIT_MASK);
    #endif /* (SenTimer_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: SenTimer_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 SenTimer_ReadCapture(void)
{
    return (SenTimer_COMP_CAP_REG & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 SenTimer_ReadCaptureBuf(void)
{
    return (SenTimer_COMP_CAP_BUF_REG & SenTimer_16BIT_MASK);
}


/*******************************************************************************
* Function Name: SenTimer_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - SenTimer_TRIG_LEVEL     - Level
*     - SenTimer_TRIG_RISING    - Rising edge
*     - SenTimer_TRIG_FALLING   - Falling edge
*     - SenTimer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_TRIG_CONTROL1_REG &= (uint32)~SenTimer_CAPTURE_MASK;
    SenTimer_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - SenTimer_TRIG_LEVEL     - Level
*     - SenTimer_TRIG_RISING    - Rising edge
*     - SenTimer_TRIG_FALLING   - Falling edge
*     - SenTimer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_TRIG_CONTROL1_REG &= (uint32)~SenTimer_RELOAD_MASK;
    SenTimer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << SenTimer_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - SenTimer_TRIG_LEVEL     - Level
*     - SenTimer_TRIG_RISING    - Rising edge
*     - SenTimer_TRIG_FALLING   - Falling edge
*     - SenTimer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_TRIG_CONTROL1_REG &= (uint32)~SenTimer_START_MASK;
    SenTimer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << SenTimer_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - SenTimer_TRIG_LEVEL     - Level
*     - SenTimer_TRIG_RISING    - Rising edge
*     - SenTimer_TRIG_FALLING   - Falling edge
*     - SenTimer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_TRIG_CONTROL1_REG &= (uint32)~SenTimer_STOP_MASK;
    SenTimer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << SenTimer_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - SenTimer_TRIG_LEVEL     - Level
*     - SenTimer_TRIG_RISING    - Rising edge
*     - SenTimer_TRIG_FALLING   - Falling edge
*     - SenTimer_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_TRIG_CONTROL1_REG &= (uint32)~SenTimer_COUNT_MASK;
    SenTimer_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << SenTimer_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - SenTimer_CMD_CAPTURE    - Trigger Capture command
*     - SenTimer_CMD_RELOAD     - Trigger Reload command
*     - SenTimer_CMD_STOP       - Trigger Stop command
*     - SenTimer_CMD_START      - Trigger Start command
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    SenTimer_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: SenTimer_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the SenTimer.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - SenTimer_STATUS_DOWN    - Set if counting down
*     - SenTimer_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 SenTimer_ReadStatus(void)
{
    return ((SenTimer_STATUS_REG >> SenTimer_RUNNING_STATUS_SHIFT) |
            (SenTimer_STATUS_REG & SenTimer_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: SenTimer_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - SenTimer_INTR_MASK_TC       - Terminal count mask
*     - SenTimer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetInterruptMode(uint32 interruptMask)
{
    SenTimer_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: SenTimer_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - SenTimer_INTR_MASK_TC       - Terminal count mask
*     - SenTimer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 SenTimer_GetInterruptSourceMasked(void)
{
    return (SenTimer_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: SenTimer_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - SenTimer_INTR_MASK_TC       - Terminal count mask
*     - SenTimer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 SenTimer_GetInterruptSource(void)
{
    return (SenTimer_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: SenTimer_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - SenTimer_INTR_MASK_TC       - Terminal count mask
*     - SenTimer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_ClearInterrupt(uint32 interruptMask)
{
    SenTimer_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: SenTimer_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - SenTimer_INTR_MASK_TC       - Terminal count mask
*     - SenTimer_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void SenTimer_SetInterrupt(uint32 interruptMask)
{
    SenTimer_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
