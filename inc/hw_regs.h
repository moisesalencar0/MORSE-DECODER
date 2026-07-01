/**
 * @file    bbb_regs.h
 * @brief   AM335x / BeagleBone Black register definitions and board pin masks.
 */

#ifndef BBB_REGS_H_
#define BBB_REGS_H_

#include "soc_AM335x.h"

/* ============================================================================
 * BBB PINMUX REGISTES
 * ========================================================================= */

#define CM_CONF_GPMC_A1                  (0x844) // GPIO1_17
#define CM_CONF_GPMC_A5                  (0x854) // GPIO1_21
#define CM_CONF_GPMC_BEN1                (0x878) // GPIO1_28

#define CM_CONF_LCD_DATA0                (0x8A0) // GPIO2_6
#define CM_CONF_LCD_DATA1                (0x8A4) // GPIO2_7
#define CM_CONF_LCD_VSYNC                (0x8E0) // GPIO2_22

/* ============================================================================
 * CLOCK MANAGEMENT REGISTERS
 * ========================================================================= */

#define CM_PER_GPIO1_CLKCTRL             (0x0AC)
#define CM_PER_GPIO2_CLKCTRL             (0x0B0)
#define CM_PER_TIMER7_CLKCTRL            (0x07C)

/* ============================================================================
 * GPIO REGISTERS
 * ========================================================================= */

#define GPIO1_OE                         (SOC_GPIO_1_REGS + 0x134)
#define GPIO1_CLEARDATAOUT               (SOC_GPIO_1_REGS + 0x190)
#define GPIO1_SETDATAOUT                 (SOC_GPIO_1_REGS + 0x194)

#define GPIO2_OE                         (SOC_GPIO_2_REGS + 0x134)
#define GPIO2_DATAIN                     (SOC_GPIO_2_REGS + 0x138)

#define GPIO2_IRQSTATUS_0                (SOC_GPIO_2_REGS + 0x02C)
#define GPIO2_IRQSTATUS_SET_0            (SOC_GPIO_2_REGS + 0x034)
#define GPIO2_IRQSTATUS_CLR_0            (SOC_GPIO_2_REGS + 0x03C)
#define GPIO2_RISINGDETECT               (SOC_GPIO_2_REGS + 0x148)

#define GPIO2_DEBOUNCENABLE              (SOC_GPIO_2_REGS + 0x150)
#define GPIO2_DEBOUNCINGTIME             (SOC_GPIO_2_REGS + 0x154)

/* ============================================================================
 * UART0 REGISTERS
 * ========================================================================= */

#define UART0_RHR                        (SOC_UART_0_REGS + 0x000)
#define UART0_THR                        (SOC_UART_0_REGS + 0x000)
#define UART0_LSR                        (SOC_UART_0_REGS + 0x014)

/**
 * @brief Returns non-zero if UART RX buffer contains data.
 */
#define UART_DATA_IN() \
    (HWREG(UART0_LSR) & (1 << 0))

/* ============================================================================
 * DMTIMER REGISTERS
 * ========================================================================= */

#define DMTIMER_TCLR                     (0x038)
#define DMTIMER_TCRR                     (0x03C)
#define DMTIMER_TLDR                     (0x040)
#define DMTIMER_TWPS                     (0x048)
#define DMTIMER_TSICR                    (0x054)
#define DMTIMER_TSICR_POSTED             (1 << 2)
#define DMTIMER_TCLR_ST                  (1 << 0)

#define DMTIMER_TWPS_W_PEND_TCLR         (1 << 0)
#define DMTIMER_TWPS_W_PEND_TCRR         (1 << 1)

#define DMTIMER_WRITE_POST_TCLR          (1 << 0)
#define DMTIMER_WRITE_POST_TCRR          (1 << 2)

#define DMTIMER_IRQSTATUS                (0x028)
#define DMTIMER_IRQENABLE_SET            (0x02C)
#define DMTIMER_IRQENABLE_CLR            (0x030)

/* ============================================================================
 * WATCHDOG REGISTERS
 * ========================================================================= */

#define WDT1_WTGR                        (SOC_WDT_1_REGS + 0x030) // Trigger Register
#define WDT1_WWPS                        (SOC_WDT_1_REGS + 0x034) // Write Posting Bits Register
#define WDT1_WSPR                        (SOC_WDT_1_REGS + 0x048) // Start/Stop Register
#define WWPS_PEND_WTGR                   (1 << 3)
#define WWPS_PEND_WSPR                   (1 << 4)

/* ============================================================================
 * INTC REGISTERS
 * ========================================================================= */

#define INTC_SYSCONFIG                   (0x010)
#define INTC_SYSSTATUS                   (0x014)
#define INTC_THRESHOLD                   (0x068)
#define INTC_SYSCONFIG_SOFTRESET         (1 << 1)
#define INTC_SYSSTATUS_RESETDONE         (1 << 0)
#define INTC_THRESHOLD_PRIORITYTHRESHOLD (0xFF)

#define INTC_MIR_CLEAR1                  (0x0A8)
#define INTC_MIR_CLEAR2                  (0x0C8)
#define INTC_MIR_CLEAR3                  (0x0E8)

#define IRQSTATUS_SET_0                  (0x034)
#define RISINGDETECT                     (0x148)

#define INTC_SIR_IRQ                     (0x040)
#define INTC_CONTROL                     (0x048)

#endif