/*******************************************************************************
* File Name: SPI_Master_PINS.h
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_SPI_Master_H)
#define CY_SCB_PINS_SPI_Master_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define SPI_Master_REMOVE_RX_WAKE_SDA_MOSI_PIN  (1u)
#define SPI_Master_REMOVE_RX_SDA_MOSI_PIN      (1u)
#define SPI_Master_REMOVE_TX_SCL_MISO_PIN      (1u)
#define SPI_Master_REMOVE_CTS_SCLK_PIN      (1u)
#define SPI_Master_REMOVE_RTS_SS0_PIN      (1u)
#define SPI_Master_REMOVE_SS1_PIN                 (1u)
#define SPI_Master_REMOVE_SS2_PIN                 (1u)
#define SPI_Master_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define SPI_Master_REMOVE_I2C_PINS                (1u)
#define SPI_Master_REMOVE_SPI_MASTER_PINS         (0u)
#define SPI_Master_REMOVE_SPI_MASTER_SCLK_PIN     (0u)
#define SPI_Master_REMOVE_SPI_MASTER_MOSI_PIN     (0u)
#define SPI_Master_REMOVE_SPI_MASTER_MISO_PIN     (0u)
#define SPI_Master_REMOVE_SPI_MASTER_SS0_PIN      (0u)
#define SPI_Master_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define SPI_Master_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define SPI_Master_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define SPI_Master_REMOVE_SPI_SLAVE_PINS          (1u)
#define SPI_Master_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define SPI_Master_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define SPI_Master_REMOVE_UART_TX_PIN             (1u)
#define SPI_Master_REMOVE_UART_RX_TX_PIN          (1u)
#define SPI_Master_REMOVE_UART_RX_PIN             (1u)
#define SPI_Master_REMOVE_UART_RX_WAKE_PIN        (1u)
#define SPI_Master_REMOVE_UART_RTS_PIN            (1u)
#define SPI_Master_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define SPI_Master_RX_WAKE_SDA_MOSI_PIN (0u == SPI_Master_REMOVE_RX_WAKE_SDA_MOSI_PIN)
#define SPI_Master_RX_SDA_MOSI_PIN     (0u == SPI_Master_REMOVE_RX_SDA_MOSI_PIN)
#define SPI_Master_TX_SCL_MISO_PIN     (0u == SPI_Master_REMOVE_TX_SCL_MISO_PIN)
#define SPI_Master_CTS_SCLK_PIN     (0u == SPI_Master_REMOVE_CTS_SCLK_PIN)
#define SPI_Master_RTS_SS0_PIN     (0u == SPI_Master_REMOVE_RTS_SS0_PIN)
#define SPI_Master_SS1_PIN                (0u == SPI_Master_REMOVE_SS1_PIN)
#define SPI_Master_SS2_PIN                (0u == SPI_Master_REMOVE_SS2_PIN)
#define SPI_Master_SS3_PIN                (0u == SPI_Master_REMOVE_SS3_PIN)

/* Mode defined pins */
#define SPI_Master_I2C_PINS               (0u == SPI_Master_REMOVE_I2C_PINS)
#define SPI_Master_SPI_MASTER_PINS        (0u == SPI_Master_REMOVE_SPI_MASTER_PINS)
#define SPI_Master_SPI_MASTER_SCLK_PIN    (0u == SPI_Master_REMOVE_SPI_MASTER_SCLK_PIN)
#define SPI_Master_SPI_MASTER_MOSI_PIN    (0u == SPI_Master_REMOVE_SPI_MASTER_MOSI_PIN)
#define SPI_Master_SPI_MASTER_MISO_PIN    (0u == SPI_Master_REMOVE_SPI_MASTER_MISO_PIN)
#define SPI_Master_SPI_MASTER_SS0_PIN     (0u == SPI_Master_REMOVE_SPI_MASTER_SS0_PIN)
#define SPI_Master_SPI_MASTER_SS1_PIN     (0u == SPI_Master_REMOVE_SPI_MASTER_SS1_PIN)
#define SPI_Master_SPI_MASTER_SS2_PIN     (0u == SPI_Master_REMOVE_SPI_MASTER_SS2_PIN)
#define SPI_Master_SPI_MASTER_SS3_PIN     (0u == SPI_Master_REMOVE_SPI_MASTER_SS3_PIN)
#define SPI_Master_SPI_SLAVE_PINS         (0u == SPI_Master_REMOVE_SPI_SLAVE_PINS)
#define SPI_Master_SPI_SLAVE_MOSI_PIN     (0u == SPI_Master_REMOVE_SPI_SLAVE_MOSI_PIN)
#define SPI_Master_SPI_SLAVE_MISO_PIN     (0u == SPI_Master_REMOVE_SPI_SLAVE_MISO_PIN)
#define SPI_Master_UART_TX_PIN            (0u == SPI_Master_REMOVE_UART_TX_PIN)
#define SPI_Master_UART_RX_TX_PIN         (0u == SPI_Master_REMOVE_UART_RX_TX_PIN)
#define SPI_Master_UART_RX_PIN            (0u == SPI_Master_REMOVE_UART_RX_PIN)
#define SPI_Master_UART_RX_WAKE_PIN       (0u == SPI_Master_REMOVE_UART_RX_WAKE_PIN)
#define SPI_Master_UART_RTS_PIN           (0u == SPI_Master_REMOVE_UART_RTS_PIN)
#define SPI_Master_UART_CTS_PIN           (0u == SPI_Master_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (SPI_Master_RX_WAKE_SDA_MOSI_PIN)
    #include "SPI_Master_uart_rx_wake_i2c_sda_spi_mosi.h"
#endif /* (SPI_Master_RX_SDA_MOSI) */

#if (SPI_Master_RX_SDA_MOSI_PIN)
    #include "SPI_Master_uart_rx_i2c_sda_spi_mosi.h"
#endif /* (SPI_Master_RX_SDA_MOSI) */

#if (SPI_Master_TX_SCL_MISO_PIN)
    #include "SPI_Master_uart_tx_i2c_scl_spi_miso.h"
#endif /* (SPI_Master_TX_SCL_MISO) */

#if (SPI_Master_CTS_SCLK_PIN)
    #include "SPI_Master_uart_cts_spi_sclk.h"
#endif /* (SPI_Master_CTS_SCLK) */

#if (SPI_Master_RTS_SS0_PIN)
    #include "SPI_Master_uart_rts_spi_ss0.h"
#endif /* (SPI_Master_RTS_SS0_PIN) */

#if (SPI_Master_SS1_PIN)
    #include "SPI_Master_spi_ss1.h"
#endif /* (SPI_Master_SS1_PIN) */

#if (SPI_Master_SS2_PIN)
    #include "SPI_Master_spi_ss2.h"
#endif /* (SPI_Master_SS2_PIN) */

#if (SPI_Master_SS3_PIN)
    #include "SPI_Master_spi_ss3.h"
#endif /* (SPI_Master_SS3_PIN) */

#if (SPI_Master_I2C_PINS)
    #include "SPI_Master_scl.h"
    #include "SPI_Master_sda.h"
#endif /* (SPI_Master_I2C_PINS) */

#if (SPI_Master_SPI_MASTER_PINS)
#if (SPI_Master_SPI_MASTER_SCLK_PIN)
    #include "SPI_Master_sclk_m.h"
#endif /* (SPI_Master_SPI_MASTER_SCLK_PIN) */

#if (SPI_Master_SPI_MASTER_MOSI_PIN)
    #include "SPI_Master_mosi_m.h"
#endif /* (SPI_Master_SPI_MASTER_MOSI_PIN) */

#if (SPI_Master_SPI_MASTER_MISO_PIN)
    #include "SPI_Master_miso_m.h"
#endif /*(SPI_Master_SPI_MASTER_MISO_PIN) */
#endif /* (SPI_Master_SPI_MASTER_PINS) */

#if (SPI_Master_SPI_SLAVE_PINS)
    #include "SPI_Master_sclk_s.h"
    #include "SPI_Master_ss_s.h"

#if (SPI_Master_SPI_SLAVE_MOSI_PIN)
    #include "SPI_Master_mosi_s.h"
#endif /* (SPI_Master_SPI_SLAVE_MOSI_PIN) */

#if (SPI_Master_SPI_SLAVE_MISO_PIN)
    #include "SPI_Master_miso_s.h"
#endif /*(SPI_Master_SPI_SLAVE_MISO_PIN) */
#endif /* (SPI_Master_SPI_SLAVE_PINS) */

#if (SPI_Master_SPI_MASTER_SS0_PIN)
    #include "SPI_Master_ss0_m.h"
#endif /* (SPI_Master_SPI_MASTER_SS0_PIN) */

#if (SPI_Master_SPI_MASTER_SS1_PIN)
    #include "SPI_Master_ss1_m.h"
#endif /* (SPI_Master_SPI_MASTER_SS1_PIN) */

#if (SPI_Master_SPI_MASTER_SS2_PIN)
    #include "SPI_Master_ss2_m.h"
#endif /* (SPI_Master_SPI_MASTER_SS2_PIN) */

#if (SPI_Master_SPI_MASTER_SS3_PIN)
    #include "SPI_Master_ss3_m.h"
#endif /* (SPI_Master_SPI_MASTER_SS3_PIN) */

#if (SPI_Master_UART_TX_PIN)
    #include "SPI_Master_tx.h"
#endif /* (SPI_Master_UART_TX_PIN) */

#if (SPI_Master_UART_RX_TX_PIN)
    #include "SPI_Master_rx_tx.h"
#endif /* (SPI_Master_UART_RX_TX_PIN) */

#if (SPI_Master_UART_RX_PIN)
    #include "SPI_Master_rx.h"
#endif /* (SPI_Master_UART_RX_PIN) */

#if (SPI_Master_UART_RX_WAKE_PIN)
    #include "SPI_Master_rx_wake.h"
#endif /* (SPI_Master_UART_RX_WAKE_PIN) */

#if (SPI_Master_UART_RTS_PIN)
    #include "SPI_Master_rts.h"
#endif /* (SPI_Master_UART_RTS_PIN) */

#if (SPI_Master_UART_CTS_PIN)
    #include "SPI_Master_cts.h"
#endif /* (SPI_Master_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (SPI_Master_RX_WAKE_SDA_MOSI_PIN)
    #define SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG \
                            (*(reg32 *) SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    #define SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_PTR \
                            ( (reg32 *) SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    #define SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_MASK \
                            (SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_POS \
                            (SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_SHIFT)

    #define SPI_Master_RX_WAKE_SDA_MOSI_INTCFG_REG \
                            (*(reg32 *) SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define SPI_Master_RX_WAKE_SDA_MOSI_INTCFG_PTR \
                            ( (reg32 *) SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define SPI_Master_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS  (SPI_Master_uart_rx_wake_i2c_sda_spi_mosi__SHIFT)
    #define SPI_Master_RX_WAKE_SDA_MOSI_INTCFG_TYPE_MASK \
                            ((uint32) SPI_Master_INTCFG_TYPE_MASK << \
                                      SPI_Master_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS)
#endif /* (SPI_Master_RX_WAKE_SDA_MOSI_PIN) */

#if (SPI_Master_RX_SDA_MOSI_PIN)
    #define SPI_Master_RX_SDA_MOSI_HSIOM_REG   (*(reg32 *) SPI_Master_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    #define SPI_Master_RX_SDA_MOSI_HSIOM_PTR   ( (reg32 *) SPI_Master_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    #define SPI_Master_RX_SDA_MOSI_HSIOM_MASK  (SPI_Master_uart_rx_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define SPI_Master_RX_SDA_MOSI_HSIOM_POS   (SPI_Master_uart_rx_i2c_sda_spi_mosi__0__HSIOM_SHIFT)
#endif /* (SPI_Master_RX_SDA_MOSI_PIN) */

#if (SPI_Master_TX_SCL_MISO_PIN)
    #define SPI_Master_TX_SCL_MISO_HSIOM_REG   (*(reg32 *) SPI_Master_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    #define SPI_Master_TX_SCL_MISO_HSIOM_PTR   ( (reg32 *) SPI_Master_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    #define SPI_Master_TX_SCL_MISO_HSIOM_MASK  (SPI_Master_uart_tx_i2c_scl_spi_miso__0__HSIOM_MASK)
    #define SPI_Master_TX_SCL_MISO_HSIOM_POS   (SPI_Master_uart_tx_i2c_scl_spi_miso__0__HSIOM_SHIFT)
#endif /* (SPI_Master_TX_SCL_MISO_PIN) */

#if (SPI_Master_CTS_SCLK_PIN)
    #define SPI_Master_CTS_SCLK_HSIOM_REG   (*(reg32 *) SPI_Master_uart_cts_spi_sclk__0__HSIOM)
    #define SPI_Master_CTS_SCLK_HSIOM_PTR   ( (reg32 *) SPI_Master_uart_cts_spi_sclk__0__HSIOM)
    #define SPI_Master_CTS_SCLK_HSIOM_MASK  (SPI_Master_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define SPI_Master_CTS_SCLK_HSIOM_POS   (SPI_Master_uart_cts_spi_sclk__0__HSIOM_SHIFT)
#endif /* (SPI_Master_CTS_SCLK_PIN) */

#if (SPI_Master_RTS_SS0_PIN)
    #define SPI_Master_RTS_SS0_HSIOM_REG   (*(reg32 *) SPI_Master_uart_rts_spi_ss0__0__HSIOM)
    #define SPI_Master_RTS_SS0_HSIOM_PTR   ( (reg32 *) SPI_Master_uart_rts_spi_ss0__0__HSIOM)
    #define SPI_Master_RTS_SS0_HSIOM_MASK  (SPI_Master_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define SPI_Master_RTS_SS0_HSIOM_POS   (SPI_Master_uart_rts_spi_ss0__0__HSIOM_SHIFT)
#endif /* (SPI_Master_RTS_SS0_PIN) */

#if (SPI_Master_SS1_PIN)
    #define SPI_Master_SS1_HSIOM_REG      (*(reg32 *) SPI_Master_spi_ss1__0__HSIOM)
    #define SPI_Master_SS1_HSIOM_PTR      ( (reg32 *) SPI_Master_spi_ss1__0__HSIOM)
    #define SPI_Master_SS1_HSIOM_MASK     (SPI_Master_spi_ss1__0__HSIOM_MASK)
    #define SPI_Master_SS1_HSIOM_POS      (SPI_Master_spi_ss1__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SS1_PIN) */

#if (SPI_Master_SS2_PIN)
    #define SPI_Master_SS2_HSIOM_REG     (*(reg32 *) SPI_Master_spi_ss2__0__HSIOM)
    #define SPI_Master_SS2_HSIOM_PTR     ( (reg32 *) SPI_Master_spi_ss2__0__HSIOM)
    #define SPI_Master_SS2_HSIOM_MASK    (SPI_Master_spi_ss2__0__HSIOM_MASK)
    #define SPI_Master_SS2_HSIOM_POS     (SPI_Master_spi_ss2__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SS2_PIN) */

#if (SPI_Master_SS3_PIN)
    #define SPI_Master_SS3_HSIOM_REG     (*(reg32 *) SPI_Master_spi_ss3__0__HSIOM)
    #define SPI_Master_SS3_HSIOM_PTR     ( (reg32 *) SPI_Master_spi_ss3__0__HSIOM)
    #define SPI_Master_SS3_HSIOM_MASK    (SPI_Master_spi_ss3__0__HSIOM_MASK)
    #define SPI_Master_SS3_HSIOM_POS     (SPI_Master_spi_ss3__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SS3_PIN) */

#if (SPI_Master_I2C_PINS)
    #define SPI_Master_SCL_HSIOM_REG     (*(reg32 *) SPI_Master_scl__0__HSIOM)
    #define SPI_Master_SCL_HSIOM_PTR     ( (reg32 *) SPI_Master_scl__0__HSIOM)
    #define SPI_Master_SCL_HSIOM_MASK    (SPI_Master_scl__0__HSIOM_MASK)
    #define SPI_Master_SCL_HSIOM_POS     (SPI_Master_scl__0__HSIOM_SHIFT)

    #define SPI_Master_SDA_HSIOM_REG     (*(reg32 *) SPI_Master_sda__0__HSIOM)
    #define SPI_Master_SDA_HSIOM_PTR     ( (reg32 *) SPI_Master_sda__0__HSIOM)
    #define SPI_Master_SDA_HSIOM_MASK    (SPI_Master_sda__0__HSIOM_MASK)
    #define SPI_Master_SDA_HSIOM_POS     (SPI_Master_sda__0__HSIOM_SHIFT)
#endif /* (SPI_Master_I2C_PINS) */

#if (SPI_Master_SPI_MASTER_SCLK_PIN)
    #define SPI_Master_SCLK_M_HSIOM_REG   (*(reg32 *) SPI_Master_sclk_m__0__HSIOM)
    #define SPI_Master_SCLK_M_HSIOM_PTR   ( (reg32 *) SPI_Master_sclk_m__0__HSIOM)
    #define SPI_Master_SCLK_M_HSIOM_MASK  (SPI_Master_sclk_m__0__HSIOM_MASK)
    #define SPI_Master_SCLK_M_HSIOM_POS   (SPI_Master_sclk_m__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SPI_MASTER_SCLK_PIN) */

#if (SPI_Master_SPI_MASTER_SS0_PIN)
    #define SPI_Master_SS0_M_HSIOM_REG    (*(reg32 *) SPI_Master_ss0_m__0__HSIOM)
    #define SPI_Master_SS0_M_HSIOM_PTR    ( (reg32 *) SPI_Master_ss0_m__0__HSIOM)
    #define SPI_Master_SS0_M_HSIOM_MASK   (SPI_Master_ss0_m__0__HSIOM_MASK)
    #define SPI_Master_SS0_M_HSIOM_POS    (SPI_Master_ss0_m__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SPI_MASTER_SS0_PIN) */

#if (SPI_Master_SPI_MASTER_SS1_PIN)
    #define SPI_Master_SS1_M_HSIOM_REG    (*(reg32 *) SPI_Master_ss1_m__0__HSIOM)
    #define SPI_Master_SS1_M_HSIOM_PTR    ( (reg32 *) SPI_Master_ss1_m__0__HSIOM)
    #define SPI_Master_SS1_M_HSIOM_MASK   (SPI_Master_ss1_m__0__HSIOM_MASK)
    #define SPI_Master_SS1_M_HSIOM_POS    (SPI_Master_ss1_m__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SPI_MASTER_SS1_PIN) */

#if (SPI_Master_SPI_MASTER_SS2_PIN)
    #define SPI_Master_SS2_M_HSIOM_REG    (*(reg32 *) SPI_Master_ss2_m__0__HSIOM)
    #define SPI_Master_SS2_M_HSIOM_PTR    ( (reg32 *) SPI_Master_ss2_m__0__HSIOM)
    #define SPI_Master_SS2_M_HSIOM_MASK   (SPI_Master_ss2_m__0__HSIOM_MASK)
    #define SPI_Master_SS2_M_HSIOM_POS    (SPI_Master_ss2_m__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SPI_MASTER_SS2_PIN) */

#if (SPI_Master_SPI_MASTER_SS3_PIN)
    #define SPI_Master_SS3_M_HSIOM_REG    (*(reg32 *) SPI_Master_ss3_m__0__HSIOM)
    #define SPI_Master_SS3_M_HSIOM_PTR    ( (reg32 *) SPI_Master_ss3_m__0__HSIOM)
    #define SPI_Master_SS3_M_HSIOM_MASK   (SPI_Master_ss3_m__0__HSIOM_MASK)
    #define SPI_Master_SS3_M_HSIOM_POS    (SPI_Master_ss3_m__0__HSIOM_SHIFT)
#endif /* (SPI_Master_SPI_MASTER_SS3_PIN) */

#if (SPI_Master_UART_TX_PIN)
    #define SPI_Master_TX_HSIOM_REG   (*(reg32 *) SPI_Master_tx__0__HSIOM)
    #define SPI_Master_TX_HSIOM_PTR   ( (reg32 *) SPI_Master_tx_0__HSIOM)
    #define SPI_Master_TX_HSIOM_MASK  (SPI_Master_tx__0__HSIOM_MASK)
    #define SPI_Master_TX_HSIOM_POS   (SPI_Master_tx__0__HSIOM_SHIFT)
#endif /* (SPI_Master_UART_TX_PIN) */

#if (SPI_Master_UART_RTS_PIN)
    #define SPI_Master_RTS_HSIOM_REG  (*(reg32 *) SPI_Master_rts__0__HSIOM)
    #define SPI_Master_RTS_HSIOM_PTR  ( (reg32 *) SPI_Master_rts__0__HSIOM)
    #define SPI_Master_RTS_HSIOM_MASK (SPI_Master_rts__0__HSIOM_MASK)
    #define SPI_Master_RTS_HSIOM_POS  (SPI_Master_rts__0__HSIOM_SHIFT)
#endif /* (SPI_Master_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constants */
#define SPI_Master_HSIOM_DEF_SEL      (0x00u)
#define SPI_Master_HSIOM_GPIO_SEL     (0x00u)
#define SPI_Master_HSIOM_UART_SEL     (0x09u)
#define SPI_Master_HSIOM_I2C_SEL      (0x0Eu)
#define SPI_Master_HSIOM_SPI_SEL      (0x0Fu)

#define SPI_Master_RX_WAKE_SDA_MOSI_PIN_INDEX   (0u)
#define SPI_Master_RX_SDA_MOSI_PIN_INDEX       (0u)
#define SPI_Master_TX_SCL_MISO_PIN_INDEX       (1u)
#define SPI_Master_CTS_SCLK_PIN_INDEX       (2u)
#define SPI_Master_RTS_SS0_PIN_INDEX       (3u)
#define SPI_Master_SS1_PIN_INDEX                  (4u)
#define SPI_Master_SS2_PIN_INDEX                  (5u)
#define SPI_Master_SS3_PIN_INDEX                  (6u)

#define SPI_Master_RX_WAKE_SDA_MOSI_PIN_MASK ((uint32) 0x01u << SPI_Master_RX_WAKE_SDA_MOSI_PIN_INDEX)
#define SPI_Master_RX_SDA_MOSI_PIN_MASK     ((uint32) 0x01u << SPI_Master_RX_SDA_MOSI_PIN_INDEX)
#define SPI_Master_TX_SCL_MISO_PIN_MASK     ((uint32) 0x01u << SPI_Master_TX_SCL_MISO_PIN_INDEX)
#define SPI_Master_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << SPI_Master_CTS_SCLK_PIN_INDEX)
#define SPI_Master_RTS_SS0_PIN_MASK     ((uint32) 0x01u << SPI_Master_RTS_SS0_PIN_INDEX)
#define SPI_Master_SS1_PIN_MASK                ((uint32) 0x01u << SPI_Master_SS1_PIN_INDEX)
#define SPI_Master_SS2_PIN_MASK                ((uint32) 0x01u << SPI_Master_SS2_PIN_INDEX)
#define SPI_Master_SS3_PIN_MASK                ((uint32) 0x01u << SPI_Master_SS3_PIN_INDEX)

/* Pin interrupt constants */
#define SPI_Master_INTCFG_TYPE_MASK           (0x03u)
#define SPI_Master_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants */
#define SPI_Master_PIN_DM_ALG_HIZ  (0u)
#define SPI_Master_PIN_DM_DIG_HIZ  (1u)
#define SPI_Master_PIN_DM_OD_LO    (4u)
#define SPI_Master_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define SPI_Master_DM_MASK    (0x7u)
#define SPI_Master_DM_SIZE    (3)
#define SPI_Master_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) SPI_Master_DM_MASK << (SPI_Master_DM_SIZE * (pos)))) >> \
                                                              (SPI_Master_DM_SIZE * (pos)) )

#if (SPI_Master_TX_SCL_MISO_PIN)
    #define SPI_Master_CHECK_TX_SCL_MISO_PIN_USED \
                (SPI_Master_PIN_DM_ALG_HIZ != \
                    SPI_Master_GET_P4_PIN_DM(SPI_Master_uart_tx_i2c_scl_spi_miso_PC, \
                                                   SPI_Master_uart_tx_i2c_scl_spi_miso_SHIFT))
#endif /* (SPI_Master_TX_SCL_MISO_PIN) */

#if (SPI_Master_RTS_SS0_PIN)
    #define SPI_Master_CHECK_RTS_SS0_PIN_USED \
                (SPI_Master_PIN_DM_ALG_HIZ != \
                    SPI_Master_GET_P4_PIN_DM(SPI_Master_uart_rts_spi_ss0_PC, \
                                                   SPI_Master_uart_rts_spi_ss0_SHIFT))
#endif /* (SPI_Master_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define SPI_Master_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define SPI_Master_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define SPI_Master_SET_HSIOM_SEL(reg, mask, pos, sel) SPI_Master_SET_REGISTER_BITS(reg, mask, pos, sel)
#define SPI_Master_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        SPI_Master_SET_REGISTER_BITS(reg, mask, pos, intType)
#define SPI_Master_SET_INP_DIS(reg, mask, val) SPI_Master_SET_REGISTER_BIT(reg, mask, val)

/* SPI_Master_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  SPI_Master_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (SPI_Master_CY_SCBIP_V0)
#if (SPI_Master_I2C_PINS)
    #define SPI_Master_SET_I2C_SCL_DR(val) SPI_Master_scl_Write(val)

    #define SPI_Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                          SPI_Master_SET_HSIOM_SEL(SPI_Master_SCL_HSIOM_REG,  \
                                                         SPI_Master_SCL_HSIOM_MASK, \
                                                         SPI_Master_SCL_HSIOM_POS,  \
                                                         (sel))
    #define SPI_Master_WAIT_SCL_SET_HIGH  (0u == SPI_Master_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (SPI_Master_RX_WAKE_SDA_MOSI_PIN)
    #define SPI_Master_SET_I2C_SCL_DR(val) \
                            SPI_Master_uart_rx_wake_i2c_sda_spi_mosi_Write(val)

    #define SPI_Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                    SPI_Master_SET_HSIOM_SEL(SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG,  \
                                                   SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_MASK, \
                                                   SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define SPI_Master_WAIT_SCL_SET_HIGH  (0u == SPI_Master_uart_rx_wake_i2c_sda_spi_mosi_Read())

#elif (SPI_Master_RX_SDA_MOSI_PIN)
    #define SPI_Master_SET_I2C_SCL_DR(val) \
                            SPI_Master_uart_rx_i2c_sda_spi_mosi_Write(val)


    #define SPI_Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                            SPI_Master_SET_HSIOM_SEL(SPI_Master_RX_SDA_MOSI_HSIOM_REG,  \
                                                           SPI_Master_RX_SDA_MOSI_HSIOM_MASK, \
                                                           SPI_Master_RX_SDA_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define SPI_Master_WAIT_SCL_SET_HIGH  (0u == SPI_Master_uart_rx_i2c_sda_spi_mosi_Read())

#else
    #define SPI_Master_SET_I2C_SCL_DR(val) \
                                                    do{ /* Does nothing */ }while(0)
    #define SPI_Master_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                    do{ /* Does nothing */ }while(0)

    #define SPI_Master_WAIT_SCL_SET_HIGH  (0u)
#endif /* (SPI_Master_I2C_PINS) */

/* SCB I2C: sda signal */
#if (SPI_Master_I2C_PINS)
    #define SPI_Master_WAIT_SDA_SET_HIGH  (0u == SPI_Master_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (SPI_Master_TX_SCL_MISO_PIN)
    #define SPI_Master_WAIT_SDA_SET_HIGH  (0u == SPI_Master_uart_tx_i2c_scl_spi_miso_Read())
#else
    #define SPI_Master_WAIT_SDA_SET_HIGH  (0u)
#endif /* (SPI_Master_MOSI_SCL_RX_PIN) */
#endif /* (SPI_Master_CY_SCBIP_V0) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define SPI_Master_REMOVE_MOSI_SCL_RX_WAKE_PIN    SPI_Master_REMOVE_RX_WAKE_SDA_MOSI_PIN
#define SPI_Master_REMOVE_MOSI_SCL_RX_PIN         SPI_Master_REMOVE_RX_SDA_MOSI_PIN
#define SPI_Master_REMOVE_MISO_SDA_TX_PIN         SPI_Master_REMOVE_TX_SCL_MISO_PIN
#ifndef SPI_Master_REMOVE_SCLK_PIN
#define SPI_Master_REMOVE_SCLK_PIN                SPI_Master_REMOVE_CTS_SCLK_PIN
#endif /* SPI_Master_REMOVE_SCLK_PIN */
#ifndef SPI_Master_REMOVE_SS0_PIN
#define SPI_Master_REMOVE_SS0_PIN                 SPI_Master_REMOVE_RTS_SS0_PIN
#endif /* SPI_Master_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define SPI_Master_MOSI_SCL_RX_WAKE_PIN   SPI_Master_RX_WAKE_SDA_MOSI_PIN
#define SPI_Master_MOSI_SCL_RX_PIN        SPI_Master_RX_SDA_MOSI_PIN
#define SPI_Master_MISO_SDA_TX_PIN        SPI_Master_TX_SCL_MISO_PIN
#ifndef SPI_Master_SCLK_PIN
#define SPI_Master_SCLK_PIN               SPI_Master_CTS_SCLK_PIN
#endif /* SPI_Master_SCLK_PIN */
#ifndef SPI_Master_SS0_PIN
#define SPI_Master_SS0_PIN                SPI_Master_RTS_SS0_PIN
#endif /* SPI_Master_SS0_PIN */

#if (SPI_Master_MOSI_SCL_RX_WAKE_PIN)
    #define SPI_Master_MOSI_SCL_RX_WAKE_HSIOM_REG     SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define SPI_Master_MOSI_SCL_RX_WAKE_HSIOM_PTR     SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define SPI_Master_MOSI_SCL_RX_WAKE_HSIOM_MASK    SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define SPI_Master_MOSI_SCL_RX_WAKE_HSIOM_POS     SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define SPI_Master_MOSI_SCL_RX_WAKE_INTCFG_REG    SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define SPI_Master_MOSI_SCL_RX_WAKE_INTCFG_PTR    SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define SPI_Master_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define SPI_Master_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  SPI_Master_RX_WAKE_SDA_MOSI_HSIOM_REG
#endif /* (SPI_Master_RX_WAKE_SDA_MOSI_PIN) */

#if (SPI_Master_MOSI_SCL_RX_PIN)
    #define SPI_Master_MOSI_SCL_RX_HSIOM_REG      SPI_Master_RX_SDA_MOSI_HSIOM_REG
    #define SPI_Master_MOSI_SCL_RX_HSIOM_PTR      SPI_Master_RX_SDA_MOSI_HSIOM_PTR
    #define SPI_Master_MOSI_SCL_RX_HSIOM_MASK     SPI_Master_RX_SDA_MOSI_HSIOM_MASK
    #define SPI_Master_MOSI_SCL_RX_HSIOM_POS      SPI_Master_RX_SDA_MOSI_HSIOM_POS
#endif /* (SPI_Master_MOSI_SCL_RX_PIN) */

#if (SPI_Master_MISO_SDA_TX_PIN)
    #define SPI_Master_MISO_SDA_TX_HSIOM_REG      SPI_Master_TX_SCL_MISO_HSIOM_REG
    #define SPI_Master_MISO_SDA_TX_HSIOM_PTR      SPI_Master_TX_SCL_MISO_HSIOM_REG
    #define SPI_Master_MISO_SDA_TX_HSIOM_MASK     SPI_Master_TX_SCL_MISO_HSIOM_REG
    #define SPI_Master_MISO_SDA_TX_HSIOM_POS      SPI_Master_TX_SCL_MISO_HSIOM_REG
#endif /* (SPI_Master_MISO_SDA_TX_PIN_PIN) */

#if (SPI_Master_SCLK_PIN)
    #ifndef SPI_Master_SCLK_HSIOM_REG
    #define SPI_Master_SCLK_HSIOM_REG     SPI_Master_CTS_SCLK_HSIOM_REG
    #define SPI_Master_SCLK_HSIOM_PTR     SPI_Master_CTS_SCLK_HSIOM_PTR
    #define SPI_Master_SCLK_HSIOM_MASK    SPI_Master_CTS_SCLK_HSIOM_MASK
    #define SPI_Master_SCLK_HSIOM_POS     SPI_Master_CTS_SCLK_HSIOM_POS
    #endif /* SPI_Master_SCLK_HSIOM_REG */
#endif /* (SPI_Master_SCLK_PIN) */

#if (SPI_Master_SS0_PIN)
    #ifndef SPI_Master_SS0_HSIOM_REG
    #define SPI_Master_SS0_HSIOM_REG      SPI_Master_RTS_SS0_HSIOM_REG
    #define SPI_Master_SS0_HSIOM_PTR      SPI_Master_RTS_SS0_HSIOM_PTR
    #define SPI_Master_SS0_HSIOM_MASK     SPI_Master_RTS_SS0_HSIOM_MASK
    #define SPI_Master_SS0_HSIOM_POS      SPI_Master_RTS_SS0_HSIOM_POS
    #endif /* SPI_Master_SS0_HSIOM_REG */
#endif /* (SPI_Master_SS0_PIN) */

#define SPI_Master_MOSI_SCL_RX_WAKE_PIN_INDEX SPI_Master_RX_WAKE_SDA_MOSI_PIN_INDEX
#define SPI_Master_MOSI_SCL_RX_PIN_INDEX      SPI_Master_RX_SDA_MOSI_PIN_INDEX
#define SPI_Master_MISO_SDA_TX_PIN_INDEX      SPI_Master_TX_SCL_MISO_PIN_INDEX
#ifndef SPI_Master_SCLK_PIN_INDEX
#define SPI_Master_SCLK_PIN_INDEX             SPI_Master_CTS_SCLK_PIN_INDEX
#endif /* SPI_Master_SCLK_PIN_INDEX */
#ifndef SPI_Master_SS0_PIN_INDEX
#define SPI_Master_SS0_PIN_INDEX              SPI_Master_RTS_SS0_PIN_INDEX
#endif /* SPI_Master_SS0_PIN_INDEX */

#define SPI_Master_MOSI_SCL_RX_WAKE_PIN_MASK SPI_Master_RX_WAKE_SDA_MOSI_PIN_MASK
#define SPI_Master_MOSI_SCL_RX_PIN_MASK      SPI_Master_RX_SDA_MOSI_PIN_MASK
#define SPI_Master_MISO_SDA_TX_PIN_MASK      SPI_Master_TX_SCL_MISO_PIN_MASK
#ifndef SPI_Master_SCLK_PIN_MASK
#define SPI_Master_SCLK_PIN_MASK             SPI_Master_CTS_SCLK_PIN_MASK
#endif /* SPI_Master_SCLK_PIN_MASK */
#ifndef SPI_Master_SS0_PIN_MASK
#define SPI_Master_SS0_PIN_MASK              SPI_Master_RTS_SS0_PIN_MASK
#endif /* SPI_Master_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_SPI_Master_H) */


/* [] END OF FILE */
