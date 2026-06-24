/**
 * @file board.c
 * @brief Board initialization for AM335x GPIO setup.
 */

#include "board.h"

#include "hw_types.h"
#include "soc_AM335x.h"

#include "watchdog.h"
#include "gpio.h"
#include "timer.h"
#include "intc.h"

/**
 * @brief Configures pin multiplexing and GPIO directions used by the application.
 *
 * Sets the LED pins to GPIO output mode and configures the button pins
 * as GPIO inputs on the BeagleBone Black.
 */
void BBB_Pinmux(void){
    uint32_t mode_output = 0x7;
    uint32_t mode_input = 0x27;

    HWREG(SOC_CONTROL_REGS + CM_CONF_GPMC_A5)   |= mode_output;
    HWREG(GPIO1_OE) &= ~(DEBUG_LED);

    HWREG(SOC_CONTROL_REGS + CM_CONF_GPMC_BEN1) |= mode_output;
    HWREG(GPIO1_OE) &= ~(USER_LED);

    HWREG(SOC_CONTROL_REGS + CM_CONF_LCD_DATA0) |= mode_input;
    HWREG(GPIO2_OE) |= (BUTTON_DOWN);

    HWREG(SOC_CONTROL_REGS + CM_CONF_LCD_DATA1) |= mode_input;
    HWREG(GPIO2_OE) |= (BUTTON_UP);
}

/**
 * @brief Initializes board clocks, pinmux and GPIO directions.
 *
 * Configures GPIO clocks, sets pin multiplexing for LEDs and buttons,
 * and defines GPIO directions (output for LEDs, input for buttons).
 */
void Board_Init(void) {
    Watchdog_Disable();
    BBB_Pinmux();
    INTC_Init();
    DMTimer_Init();
    DMTimer_IntConfig();
    GPIO_Init();
    GPIO_IntConfig();
}