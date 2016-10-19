/*******************************************************************************
* File Name: SenTimer.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the SenTimer
*  component.
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

#if !defined(CY_TCPWM_SenTimer_H)
#define CY_TCPWM_SenTimer_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} SenTimer_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  SenTimer_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define SenTimer_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define SenTimer_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define SenTimer_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define SenTimer_QUAD_ENCODING_MODES            (0lu)

/* Signal modes */
#define SenTimer_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define SenTimer_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define SenTimer_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define SenTimer_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define SenTimer_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define SenTimer_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define SenTimer_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define SenTimer_TC_RUN_MODE                    (0lu)
#define SenTimer_TC_COUNTER_MODE                (1lu)
#define SenTimer_TC_COMP_CAP_MODE               (2lu)
#define SenTimer_TC_PRESCALER                   (0lu)

/* Signal modes */
#define SenTimer_TC_RELOAD_SIGNAL_MODE          (0lu)
#define SenTimer_TC_COUNT_SIGNAL_MODE           (3lu)
#define SenTimer_TC_START_SIGNAL_MODE           (0lu)
#define SenTimer_TC_STOP_SIGNAL_MODE            (0lu)
#define SenTimer_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define SenTimer_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define SenTimer_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define SenTimer_TC_START_SIGNAL_PRESENT        (0lu)
#define SenTimer_TC_STOP_SIGNAL_PRESENT         (0lu)
#define SenTimer_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define SenTimer_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define SenTimer_PWM_KILL_EVENT                 (0lu)
#define SenTimer_PWM_STOP_EVENT                 (0lu)
#define SenTimer_PWM_MODE                       (4lu)
#define SenTimer_PWM_OUT_N_INVERT               (0lu)
#define SenTimer_PWM_OUT_INVERT                 (0lu)
#define SenTimer_PWM_ALIGN                      (0lu)
#define SenTimer_PWM_RUN_MODE                   (0lu)
#define SenTimer_PWM_DEAD_TIME_CYCLE            (0lu)
#define SenTimer_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define SenTimer_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define SenTimer_PWM_COUNT_SIGNAL_MODE          (3lu)
#define SenTimer_PWM_START_SIGNAL_MODE          (0lu)
#define SenTimer_PWM_STOP_SIGNAL_MODE           (0lu)
#define SenTimer_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define SenTimer_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define SenTimer_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define SenTimer_PWM_START_SIGNAL_PRESENT       (0lu)
#define SenTimer_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define SenTimer_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define SenTimer_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define SenTimer_TC_PERIOD_VALUE                (500lu)
#define SenTimer_TC_COMPARE_VALUE               (65535lu)
#define SenTimer_TC_COMPARE_BUF_VALUE           (65535lu)
#define SenTimer_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define SenTimer_PWM_PERIOD_VALUE               (65535lu)
#define SenTimer_PWM_PERIOD_BUF_VALUE           (65535lu)
#define SenTimer_PWM_PERIOD_SWAP                (0lu)
#define SenTimer_PWM_COMPARE_VALUE              (65535lu)
#define SenTimer_PWM_COMPARE_BUF_VALUE          (65535lu)
#define SenTimer_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define SenTimer__LEFT 0
#define SenTimer__RIGHT 1
#define SenTimer__CENTER 2
#define SenTimer__ASYMMETRIC 3

#define SenTimer__X1 0
#define SenTimer__X2 1
#define SenTimer__X4 2

#define SenTimer__PWM 4
#define SenTimer__PWM_DT 5
#define SenTimer__PWM_PR 6

#define SenTimer__INVERSE 1
#define SenTimer__DIRECT 0

#define SenTimer__CAPTURE 2
#define SenTimer__COMPARE 0

#define SenTimer__TRIG_LEVEL 3
#define SenTimer__TRIG_RISING 0
#define SenTimer__TRIG_FALLING 1
#define SenTimer__TRIG_BOTH 2

#define SenTimer__INTR_MASK_TC 1
#define SenTimer__INTR_MASK_CC_MATCH 2
#define SenTimer__INTR_MASK_NONE 0
#define SenTimer__INTR_MASK_TC_CC 3

#define SenTimer__UNCONFIG 8
#define SenTimer__TIMER 1
#define SenTimer__QUAD 3
#define SenTimer__PWM_SEL 7

#define SenTimer__COUNT_UP 0
#define SenTimer__COUNT_DOWN 1
#define SenTimer__COUNT_UPDOWN0 2
#define SenTimer__COUNT_UPDOWN1 3


/* Prescaler */
#define SenTimer_PRESCALE_DIVBY1                ((uint32)(0u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY2                ((uint32)(1u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY4                ((uint32)(2u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY8                ((uint32)(3u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY16               ((uint32)(4u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY32               ((uint32)(5u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY64               ((uint32)(6u << SenTimer_PRESCALER_SHIFT))
#define SenTimer_PRESCALE_DIVBY128              ((uint32)(7u << SenTimer_PRESCALER_SHIFT))

/* TCPWM set modes */
#define SenTimer_MODE_TIMER_COMPARE             ((uint32)(SenTimer__COMPARE         <<  \
                                                                  SenTimer_MODE_SHIFT))
#define SenTimer_MODE_TIMER_CAPTURE             ((uint32)(SenTimer__CAPTURE         <<  \
                                                                  SenTimer_MODE_SHIFT))
#define SenTimer_MODE_QUAD                      ((uint32)(SenTimer__QUAD            <<  \
                                                                  SenTimer_MODE_SHIFT))
#define SenTimer_MODE_PWM                       ((uint32)(SenTimer__PWM             <<  \
                                                                  SenTimer_MODE_SHIFT))
#define SenTimer_MODE_PWM_DT                    ((uint32)(SenTimer__PWM_DT          <<  \
                                                                  SenTimer_MODE_SHIFT))
#define SenTimer_MODE_PWM_PR                    ((uint32)(SenTimer__PWM_PR          <<  \
                                                                  SenTimer_MODE_SHIFT))

/* Quad Modes */
#define SenTimer_MODE_X1                        ((uint32)(SenTimer__X1              <<  \
                                                                  SenTimer_QUAD_MODE_SHIFT))
#define SenTimer_MODE_X2                        ((uint32)(SenTimer__X2              <<  \
                                                                  SenTimer_QUAD_MODE_SHIFT))
#define SenTimer_MODE_X4                        ((uint32)(SenTimer__X4              <<  \
                                                                  SenTimer_QUAD_MODE_SHIFT))

/* Counter modes */
#define SenTimer_COUNT_UP                       ((uint32)(SenTimer__COUNT_UP        <<  \
                                                                  SenTimer_UPDOWN_SHIFT))
#define SenTimer_COUNT_DOWN                     ((uint32)(SenTimer__COUNT_DOWN      <<  \
                                                                  SenTimer_UPDOWN_SHIFT))
#define SenTimer_COUNT_UPDOWN0                  ((uint32)(SenTimer__COUNT_UPDOWN0   <<  \
                                                                  SenTimer_UPDOWN_SHIFT))
#define SenTimer_COUNT_UPDOWN1                  ((uint32)(SenTimer__COUNT_UPDOWN1   <<  \
                                                                  SenTimer_UPDOWN_SHIFT))

/* PWM output invert */
#define SenTimer_INVERT_LINE                    ((uint32)(SenTimer__INVERSE         <<  \
                                                                  SenTimer_INV_OUT_SHIFT))
#define SenTimer_INVERT_LINE_N                  ((uint32)(SenTimer__INVERSE         <<  \
                                                                  SenTimer_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define SenTimer_TRIG_RISING                    ((uint32)SenTimer__TRIG_RISING)
#define SenTimer_TRIG_FALLING                   ((uint32)SenTimer__TRIG_FALLING)
#define SenTimer_TRIG_BOTH                      ((uint32)SenTimer__TRIG_BOTH)
#define SenTimer_TRIG_LEVEL                     ((uint32)SenTimer__TRIG_LEVEL)

/* Interrupt mask */
#define SenTimer_INTR_MASK_TC                   ((uint32)SenTimer__INTR_MASK_TC)
#define SenTimer_INTR_MASK_CC_MATCH             ((uint32)SenTimer__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define SenTimer_CC_MATCH_SET                   (0x00u)
#define SenTimer_CC_MATCH_CLEAR                 (0x01u)
#define SenTimer_CC_MATCH_INVERT                (0x02u)
#define SenTimer_CC_MATCH_NO_CHANGE             (0x03u)
#define SenTimer_OVERLOW_SET                    (0x00u)
#define SenTimer_OVERLOW_CLEAR                  (0x04u)
#define SenTimer_OVERLOW_INVERT                 (0x08u)
#define SenTimer_OVERLOW_NO_CHANGE              (0x0Cu)
#define SenTimer_UNDERFLOW_SET                  (0x00u)
#define SenTimer_UNDERFLOW_CLEAR                (0x10u)
#define SenTimer_UNDERFLOW_INVERT               (0x20u)
#define SenTimer_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define SenTimer_PWM_MODE_LEFT                  (SenTimer_CC_MATCH_CLEAR        |   \
                                                         SenTimer_OVERLOW_SET           |   \
                                                         SenTimer_UNDERFLOW_NO_CHANGE)
#define SenTimer_PWM_MODE_RIGHT                 (SenTimer_CC_MATCH_SET          |   \
                                                         SenTimer_OVERLOW_NO_CHANGE     |   \
                                                         SenTimer_UNDERFLOW_CLEAR)
#define SenTimer_PWM_MODE_ASYM                  (SenTimer_CC_MATCH_INVERT       |   \
                                                         SenTimer_OVERLOW_SET           |   \
                                                         SenTimer_UNDERFLOW_CLEAR)

#if (SenTimer_CY_TCPWM_V2)
    #if(SenTimer_CY_TCPWM_4000)
        #define SenTimer_PWM_MODE_CENTER                (SenTimer_CC_MATCH_INVERT       |   \
                                                                 SenTimer_OVERLOW_NO_CHANGE     |   \
                                                                 SenTimer_UNDERFLOW_CLEAR)
    #else
        #define SenTimer_PWM_MODE_CENTER                (SenTimer_CC_MATCH_INVERT       |   \
                                                                 SenTimer_OVERLOW_SET           |   \
                                                                 SenTimer_UNDERFLOW_CLEAR)
    #endif /* (SenTimer_CY_TCPWM_4000) */
#else
    #define SenTimer_PWM_MODE_CENTER                (SenTimer_CC_MATCH_INVERT       |   \
                                                             SenTimer_OVERLOW_NO_CHANGE     |   \
                                                             SenTimer_UNDERFLOW_CLEAR)
#endif /* (SenTimer_CY_TCPWM_NEW) */

/* Command operations without condition */
#define SenTimer_CMD_CAPTURE                    (0u)
#define SenTimer_CMD_RELOAD                     (8u)
#define SenTimer_CMD_STOP                       (16u)
#define SenTimer_CMD_START                      (24u)

/* Status */
#define SenTimer_STATUS_DOWN                    (1u)
#define SenTimer_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   SenTimer_Init(void);
void   SenTimer_Enable(void);
void   SenTimer_Start(void);
void   SenTimer_Stop(void);

void   SenTimer_SetMode(uint32 mode);
void   SenTimer_SetCounterMode(uint32 counterMode);
void   SenTimer_SetPWMMode(uint32 modeMask);
void   SenTimer_SetQDMode(uint32 qdMode);

void   SenTimer_SetPrescaler(uint32 prescaler);
void   SenTimer_TriggerCommand(uint32 mask, uint32 command);
void   SenTimer_SetOneShot(uint32 oneShotEnable);
uint32 SenTimer_ReadStatus(void);

void   SenTimer_SetPWMSyncKill(uint32 syncKillEnable);
void   SenTimer_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   SenTimer_SetPWMDeadTime(uint32 deadTime);
void   SenTimer_SetPWMInvert(uint32 mask);

void   SenTimer_SetInterruptMode(uint32 interruptMask);
uint32 SenTimer_GetInterruptSourceMasked(void);
uint32 SenTimer_GetInterruptSource(void);
void   SenTimer_ClearInterrupt(uint32 interruptMask);
void   SenTimer_SetInterrupt(uint32 interruptMask);

void   SenTimer_WriteCounter(uint32 count);
uint32 SenTimer_ReadCounter(void);

uint32 SenTimer_ReadCapture(void);
uint32 SenTimer_ReadCaptureBuf(void);

void   SenTimer_WritePeriod(uint32 period);
uint32 SenTimer_ReadPeriod(void);
void   SenTimer_WritePeriodBuf(uint32 periodBuf);
uint32 SenTimer_ReadPeriodBuf(void);

void   SenTimer_WriteCompare(uint32 compare);
uint32 SenTimer_ReadCompare(void);
void   SenTimer_WriteCompareBuf(uint32 compareBuf);
uint32 SenTimer_ReadCompareBuf(void);

void   SenTimer_SetPeriodSwap(uint32 swapEnable);
void   SenTimer_SetCompareSwap(uint32 swapEnable);

void   SenTimer_SetCaptureMode(uint32 triggerMode);
void   SenTimer_SetReloadMode(uint32 triggerMode);
void   SenTimer_SetStartMode(uint32 triggerMode);
void   SenTimer_SetStopMode(uint32 triggerMode);
void   SenTimer_SetCountMode(uint32 triggerMode);

void   SenTimer_SaveConfig(void);
void   SenTimer_RestoreConfig(void);
void   SenTimer_Sleep(void);
void   SenTimer_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define SenTimer_BLOCK_CONTROL_REG              (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define SenTimer_BLOCK_CONTROL_PTR              ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define SenTimer_COMMAND_REG                    (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define SenTimer_COMMAND_PTR                    ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define SenTimer_INTRRUPT_CAUSE_REG             (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define SenTimer_INTRRUPT_CAUSE_PTR             ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define SenTimer_CONTROL_REG                    (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__CTRL )
#define SenTimer_CONTROL_PTR                    ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__CTRL )
#define SenTimer_STATUS_REG                     (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__STATUS )
#define SenTimer_STATUS_PTR                     ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__STATUS )
#define SenTimer_COUNTER_REG                    (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__COUNTER )
#define SenTimer_COUNTER_PTR                    ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__COUNTER )
#define SenTimer_COMP_CAP_REG                   (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__CC )
#define SenTimer_COMP_CAP_PTR                   ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__CC )
#define SenTimer_COMP_CAP_BUF_REG               (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__CC_BUFF )
#define SenTimer_COMP_CAP_BUF_PTR               ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__CC_BUFF )
#define SenTimer_PERIOD_REG                     (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__PERIOD )
#define SenTimer_PERIOD_PTR                     ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__PERIOD )
#define SenTimer_PERIOD_BUF_REG                 (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define SenTimer_PERIOD_BUF_PTR                 ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define SenTimer_TRIG_CONTROL0_REG              (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define SenTimer_TRIG_CONTROL0_PTR              ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define SenTimer_TRIG_CONTROL1_REG              (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define SenTimer_TRIG_CONTROL1_PTR              ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define SenTimer_TRIG_CONTROL2_REG              (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define SenTimer_TRIG_CONTROL2_PTR              ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define SenTimer_INTERRUPT_REQ_REG              (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR )
#define SenTimer_INTERRUPT_REQ_PTR              ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR )
#define SenTimer_INTERRUPT_SET_REG              (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR_SET )
#define SenTimer_INTERRUPT_SET_PTR              ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR_SET )
#define SenTimer_INTERRUPT_MASK_REG             (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR_MASK )
#define SenTimer_INTERRUPT_MASK_PTR             ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR_MASK )
#define SenTimer_INTERRUPT_MASKED_REG           (*(reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR_MASKED )
#define SenTimer_INTERRUPT_MASKED_PTR           ( (reg32 *) SenTimer_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define SenTimer_MASK                           ((uint32)SenTimer_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define SenTimer_RELOAD_CC_SHIFT                (0u)
#define SenTimer_RELOAD_PERIOD_SHIFT            (1u)
#define SenTimer_PWM_SYNC_KILL_SHIFT            (2u)
#define SenTimer_PWM_STOP_KILL_SHIFT            (3u)
#define SenTimer_PRESCALER_SHIFT                (8u)
#define SenTimer_UPDOWN_SHIFT                   (16u)
#define SenTimer_ONESHOT_SHIFT                  (18u)
#define SenTimer_QUAD_MODE_SHIFT                (20u)
#define SenTimer_INV_OUT_SHIFT                  (20u)
#define SenTimer_INV_COMPL_OUT_SHIFT            (21u)
#define SenTimer_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define SenTimer_RELOAD_CC_MASK                 ((uint32)(SenTimer_1BIT_MASK        <<  \
                                                                            SenTimer_RELOAD_CC_SHIFT))
#define SenTimer_RELOAD_PERIOD_MASK             ((uint32)(SenTimer_1BIT_MASK        <<  \
                                                                            SenTimer_RELOAD_PERIOD_SHIFT))
#define SenTimer_PWM_SYNC_KILL_MASK             ((uint32)(SenTimer_1BIT_MASK        <<  \
                                                                            SenTimer_PWM_SYNC_KILL_SHIFT))
#define SenTimer_PWM_STOP_KILL_MASK             ((uint32)(SenTimer_1BIT_MASK        <<  \
                                                                            SenTimer_PWM_STOP_KILL_SHIFT))
#define SenTimer_PRESCALER_MASK                 ((uint32)(SenTimer_8BIT_MASK        <<  \
                                                                            SenTimer_PRESCALER_SHIFT))
#define SenTimer_UPDOWN_MASK                    ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                            SenTimer_UPDOWN_SHIFT))
#define SenTimer_ONESHOT_MASK                   ((uint32)(SenTimer_1BIT_MASK        <<  \
                                                                            SenTimer_ONESHOT_SHIFT))
#define SenTimer_QUAD_MODE_MASK                 ((uint32)(SenTimer_3BIT_MASK        <<  \
                                                                            SenTimer_QUAD_MODE_SHIFT))
#define SenTimer_INV_OUT_MASK                   ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                            SenTimer_INV_OUT_SHIFT))
#define SenTimer_MODE_MASK                      ((uint32)(SenTimer_3BIT_MASK        <<  \
                                                                            SenTimer_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define SenTimer_CAPTURE_SHIFT                  (0u)
#define SenTimer_COUNT_SHIFT                    (2u)
#define SenTimer_RELOAD_SHIFT                   (4u)
#define SenTimer_STOP_SHIFT                     (6u)
#define SenTimer_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define SenTimer_CAPTURE_MASK                   ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                  SenTimer_CAPTURE_SHIFT))
#define SenTimer_COUNT_MASK                     ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                  SenTimer_COUNT_SHIFT))
#define SenTimer_RELOAD_MASK                    ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                  SenTimer_RELOAD_SHIFT))
#define SenTimer_STOP_MASK                      ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                  SenTimer_STOP_SHIFT))
#define SenTimer_START_MASK                     ((uint32)(SenTimer_2BIT_MASK        <<  \
                                                                  SenTimer_START_SHIFT))

/* MASK */
#define SenTimer_1BIT_MASK                      ((uint32)0x01u)
#define SenTimer_2BIT_MASK                      ((uint32)0x03u)
#define SenTimer_3BIT_MASK                      ((uint32)0x07u)
#define SenTimer_6BIT_MASK                      ((uint32)0x3Fu)
#define SenTimer_8BIT_MASK                      ((uint32)0xFFu)
#define SenTimer_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define SenTimer_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define SenTimer_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(SenTimer_QUAD_ENCODING_MODES     << SenTimer_QUAD_MODE_SHIFT))       |\
         ((uint32)(SenTimer_CONFIG                  << SenTimer_MODE_SHIFT)))

#define SenTimer_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(SenTimer_PWM_STOP_EVENT          << SenTimer_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(SenTimer_PWM_OUT_INVERT          << SenTimer_INV_OUT_SHIFT))         |\
         ((uint32)(SenTimer_PWM_OUT_N_INVERT        << SenTimer_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(SenTimer_PWM_MODE                << SenTimer_MODE_SHIFT)))

#define SenTimer_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(SenTimer_PWM_RUN_MODE         << SenTimer_ONESHOT_SHIFT))
            
#define SenTimer_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(SenTimer_PWM_ALIGN            << SenTimer_UPDOWN_SHIFT))

#define SenTimer_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(SenTimer_PWM_KILL_EVENT      << SenTimer_PWM_SYNC_KILL_SHIFT))

#define SenTimer_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(SenTimer_PWM_DEAD_TIME_CYCLE  << SenTimer_PRESCALER_SHIFT))

#define SenTimer_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(SenTimer_PWM_PRESCALER        << SenTimer_PRESCALER_SHIFT))

#define SenTimer_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(SenTimer_TC_PRESCALER            << SenTimer_PRESCALER_SHIFT))       |\
         ((uint32)(SenTimer_TC_COUNTER_MODE         << SenTimer_UPDOWN_SHIFT))          |\
         ((uint32)(SenTimer_TC_RUN_MODE             << SenTimer_ONESHOT_SHIFT))         |\
         ((uint32)(SenTimer_TC_COMP_CAP_MODE        << SenTimer_MODE_SHIFT)))
        
#define SenTimer_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(SenTimer_QUAD_PHIA_SIGNAL_MODE   << SenTimer_COUNT_SHIFT))           |\
         ((uint32)(SenTimer_QUAD_INDEX_SIGNAL_MODE  << SenTimer_RELOAD_SHIFT))          |\
         ((uint32)(SenTimer_QUAD_STOP_SIGNAL_MODE   << SenTimer_STOP_SHIFT))            |\
         ((uint32)(SenTimer_QUAD_PHIB_SIGNAL_MODE   << SenTimer_START_SHIFT)))

#define SenTimer_PWM_SIGNALS_MODES                                                              \
        (((uint32)(SenTimer_PWM_SWITCH_SIGNAL_MODE  << SenTimer_CAPTURE_SHIFT))         |\
         ((uint32)(SenTimer_PWM_COUNT_SIGNAL_MODE   << SenTimer_COUNT_SHIFT))           |\
         ((uint32)(SenTimer_PWM_RELOAD_SIGNAL_MODE  << SenTimer_RELOAD_SHIFT))          |\
         ((uint32)(SenTimer_PWM_STOP_SIGNAL_MODE    << SenTimer_STOP_SHIFT))            |\
         ((uint32)(SenTimer_PWM_START_SIGNAL_MODE   << SenTimer_START_SHIFT)))

#define SenTimer_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(SenTimer_TC_CAPTURE_SIGNAL_MODE  << SenTimer_CAPTURE_SHIFT))         |\
         ((uint32)(SenTimer_TC_COUNT_SIGNAL_MODE    << SenTimer_COUNT_SHIFT))           |\
         ((uint32)(SenTimer_TC_RELOAD_SIGNAL_MODE   << SenTimer_RELOAD_SHIFT))          |\
         ((uint32)(SenTimer_TC_STOP_SIGNAL_MODE     << SenTimer_STOP_SHIFT))            |\
         ((uint32)(SenTimer_TC_START_SIGNAL_MODE    << SenTimer_START_SHIFT)))
        
#define SenTimer_TIMER_UPDOWN_CNT_USED                                                          \
                ((SenTimer__COUNT_UPDOWN0 == SenTimer_TC_COUNTER_MODE)                  ||\
                 (SenTimer__COUNT_UPDOWN1 == SenTimer_TC_COUNTER_MODE))

#define SenTimer_PWM_UPDOWN_CNT_USED                                                            \
                ((SenTimer__CENTER == SenTimer_PWM_ALIGN)                               ||\
                 (SenTimer__ASYMMETRIC == SenTimer_PWM_ALIGN))               
        
#define SenTimer_PWM_PR_INIT_VALUE              (1u)
#define SenTimer_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_SenTimer_H */

/* [] END OF FILE */
