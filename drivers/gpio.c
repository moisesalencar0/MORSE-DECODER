/**
 * @file gpio.c
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#include "gpio.h"

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"


#define SETTING  ((1<<18) | 2)

/**
 * @brief Activates GPIO1 and GPIO2 module clocks.
 *
 * Enables the clock signals required by the GPIO1 and GPIO2 peripherals
 * and blocks execution until both modules report that the clocks are enabled.
 */
void GPIO_Init(void){
        HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |= SETTING;
        while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) & SETTING) == 0){};
    
        HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |= SETTING;
        while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) & SETTING) == 0){};
}


/**
 * @brief Global flag for button event.
 *
 * Set to @c true by the ISR when the button is pressed.
 * Read and reset by @c main().
 */
vuint32_t flag_gpio = false;


/** BREAK: decidir quais pinos gpio usar pra botões
 * @brief Configures the button interrupt on GPIO2 pin 6.
 *
 * Enables debounce for pressing hardware'
 * Unmasks line 32 (GPIOINT2A) in the INTC_MIR_CLEAR1 register.
 * Enables the interrupt on pin 6 via GPIO2_IRQSTATUS_SET_0.
 * Configures rising edge detection via GPIO2_RISINGDETECT.
 */
 void GPIO_Interrupt_Config(void) {
    HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR1) |= (1 << 0);
    
    HWREG(GPIO2_DEBOUNCINGTIME)  = 0xFF;       // ~8ms
    HWREG(GPIO2_DEBOUNCENABLE)  |= (1 << 6);
    
    HWREG(GPIO2_IRQSTATUS_SET_0)  |= (1 << 6);
    HWREG(GPIO2_RISINGDETECT)     |= (1 << 6);
}


/** BREAK: fazer essa função ser reaproveitável
 * @brief GPIO1 interrupt service routine.
 *
 * Clears the interrupt flag for pin 13 in hardware and signals the event to @c main() through @c flag_gpio.
 */
 void GPIO_ISR(void) {
    HWREG(GPIO2_IRQSTATUS_0) = (1 << 13);
    flag_gpio = true;
}