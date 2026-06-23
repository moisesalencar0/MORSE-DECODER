/**
 * @file gpio.h
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#ifndef GPIO_H
#define GPIO_H

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"

/* ──────────────────────────────────────────────── */
/* GPIO1 API                                        */
/* ──────────────────────────────────────────────── */

/**
 * @brief Sets GPIO1 pins high.
 */
#define GPIO1PinWriteHigh(pin)      HWREG(GPIO1_SETDATAOUT) = (pin)

/**
 * @brief Sets GPIO1 pins low.
 */
#define GPIO1PinWriteLow(pin)       HWREG(GPIO1_CLEARDATAOUT) = (pin)

/**
 * @brief Configures GPIO1 pins as outputs.
 */
#define GPIO1PinOutputEnable(pin)   HWREG(GPIO1_OE) &= ~(pin)

/* ──────────────────────────────────────────────── */
/* GPIO2 API                                        */
/* ──────────────────────────────────────────────── */

/**
 * @brief Reads GPIO2 pin state.
 *
 * @return Non-zero if pin is high.
 */
#define GPIO2PinRead(pin)          (HWREG(GPIO2_DATAIN) & (pin))

/**
 * @brief Configures GPIO2 pins as inputs.
 */
#define GPIO2PinInputEnable(pin)    HWREG(GPIO2_OE) |= (pin)

extern vuint32_t flag_gpio;

void GPIOIntConfig(void);

void GPIOIsr(void);

#endif /* GPIO_H */