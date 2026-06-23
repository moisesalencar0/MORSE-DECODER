/**
 * @file board.c
 * @brief Board initialization for AM335x GPIO setup.
 */

#include "board.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include "gpio.h"


void BBB_Pinmux(void){
    uint32_t mode_output = 0x7;
    uint32_t mode_input = 0x27;

    HWREG(SOC_CONTROL_REGS + CM_CONF_GPMC_A5)   |= mode_output;
    HWREG(SOC_CONTROL_REGS + CM_CONF_GPMC_BEN1) |= mode_output;

    HWREG(SOC_CONTROL_REGS + CM_CONF_LCD_DATA0) |= mode_input;
    HWREG(SOC_CONTROL_REGS + CM_CONF_LCD_DATA1) |= mode_input;
}

/**
 * @brief Initializes board clocks, pinmux and GPIO directions.
 *
 * Configures GPIO clocks, sets pin multiplexing for LEDs and buttons,
 * and defines GPIO directions (output for LEDs, input for buttons).
 */
void Board_Init(void) {
    BBB_Pinmux();
    GPIO_Init;
    
}

