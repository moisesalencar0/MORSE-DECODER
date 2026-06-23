/**
 * @file pinmux.c
 * @brief Control module pin multiplexing (TRM 9).
 */

#include "pinmux.h"
#include "hw_types.h"
#include "soc_AM335x.h"

uint32_t mode_output = 0x7;
uint32_t mode_input = 0x27;

/**
 * @brief Configures a pin as GPIO output.
 *
 * @param REGISTER Control module register offset for the pin.
 */
void PINMUX_GPIO_OUTPUT(uint32_t REGISTER){
    HWREG(SOC_CONTROL_REGS + REGISTER) |= mode_output;
}

/**
 * @brief Configures a pin as GPIO input.
 *
 * @param REGISTER Control module register offset for the pin.
 */
void PINMUX_GPIO_INPUT(uint32_t REGISTER){
    HWREG(SOC_CONTROL_REGS + REGISTER) |= mode_input;
}