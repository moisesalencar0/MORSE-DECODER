/**
 * @file  intc.c
 * @brief ARM interrupt controller (AINTC) initialization and dispatcher.
 */

#include "intc.h"

#include "hw_regs.h"
#include "hw_types.h"
#include "soc_AM335x.h"

#include "timer.h"
#include "gpio.h"

/** @brief Interrupt line number for DMTimer7. */
#define SYS_INT_DMTIMER7   95

/** @brief Interrupt line number for GPIO2 group A. */
#define SYS_INT_GPIOINT2A  32

/**
 * @brief Initializes the ARM interrupt controller (AINTC).
 *
 * Performs a soft-reset, waits for completion and sets
 * the priority threshold to allow all interrupt lines.
 */
void INTC_Init(void) {
    HWREG(SOC_AINTC_REGS + INTC_SYSCONFIG) = INTC_SYSCONFIG_SOFTRESET;

    while ((HWREG(SOC_AINTC_REGS + INTC_SYSSTATUS)
            & INTC_SYSSTATUS_RESETDONE) != INTC_SYSSTATUS_RESETDONE);

    HWREG(SOC_AINTC_REGS + INTC_THRESHOLD) = 0xFF;
}


/**
 * @brief Central interrupt dispatcher.
 *
 * Reads the active interrupt line from @c INTC_SIR_IRQ,
 * dispatches to the corresponding ISR and acknowledges the interrupt to the INTC.
 */
void INTC_ISRHandler(void) {
    switch (HWREG(SOC_AINTC_REGS + INTC_SIR_IRQ) & 0x7F) {
        case SYS_INT_DMTIMER7:
            DMTimer_ISR();
            break;
        case SYS_INT_GPIOINT2A:
            GPIO_ISR();
            break;
        default:
            break;
    }

    HWREG(SOC_AINTC_REGS + INTC_CONTROL) |= 0x1;
}
