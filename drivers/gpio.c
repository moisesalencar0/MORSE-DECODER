/**
 * @file gpio.c
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#include "gpio.h"

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"

/**
 * @brief Global flag for button event.
 *
 * Set to @c true by the ISR when the button is pressed.
 * Read and reset by @c main().
 */
vuint32_t flag_gpio = false;

/** BREAK: decidir quais pinos gpio usar pra botões
 * @brief Configures the button interrupt on GPIO1 pin 13.
 *
 * Enables debounce for pressing hardware'
 * Unmasks line 96 (GPIOINT1A) in the INTC_MIR_CLEAR3 register.
 * Enables the interrupt on pin 13 via GPIO1_IRQSTATUS_SET_0.
 * Configures rising edge detection via GPIO1_RISINGDETECT.
 void gpioIntConfig(void) {
    HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR3) |= (1 << 2);
    
    HWREG(GPIO1_DEBOUNCINGTIME)  = 0xFF;       // ~8ms
    HWREG(GPIO1_DEBOUNCENABLE)  |= (1 << 13);
    
    HWREG(GPIO1_IRQSTATUS_SET_0)            |= (1 << 13);
    HWREG(GPIO1_RISINGDETECT)               |= (1 << 13);
}
*/


/** BREAK: fazer essa função ser reaproveitável
 * @brief GPIO1 interrupt service routine.
 *
 * Clears the interrupt flag for pin 13 in hardware and signals the event to @c main() through @c flag_gpio.
 void gpioIsr(void) {
    HWREG(GPIO1_IRQSTATUS_0)  = (1 << 13);
    flag_gpio = true;
}
*/
