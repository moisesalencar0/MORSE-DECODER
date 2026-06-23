/**
 * @file gpio.h
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#ifndef GPIO_H
#define GPIO_H

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"


/* ============================================================================
 * BOARD PIN MASKS
 * =========================================================================

#define BUTTON_UP                  (1 << 13)
#define BUTTON_DOWN                (1 << 7)
#define BBB_LED0                   (1 << 21)
#define BBB_LED1                   (1 << 22)
#define BBB_LED2                   (1 << 23)
#define BBB_LED3                   (1 << 24)
#define USER_LED                   (1 << 28)

/* ──────────────────────────────────────────────── */
/* GPIO1 API                                        */
/* ──────────────────────────────────────────────── */

#define ledOn(pin)                  HWREG(GPIO1_SETDATAOUT) = (pin)
#define ledOff(pin)                 HWREG(GPIO1_CLEARDATAOUT) = (pin)
#define GPIO1PinOutputEnable(pin)   HWREG(GPIO1_OE) &= ~(pin)

/* ──────────────────────────────────────────────── */
/* GPIO2 API                                        */
/* ──────────────────────────────────────────────── */

#define pinRead(pin)               (HWREG(GPIO2_DATAIN) & (pin))
#define GPIO2PinInputEnable(pin)    HWREG(GPIO2_OE) |= (pin)

extern vuint32_t flag_gpio;

void GPIOIntConfig(void);

void GPIOIsr(void);

#endif /* GPIO_H */