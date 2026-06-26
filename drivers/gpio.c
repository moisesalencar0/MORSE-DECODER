/**
 * @file gpio.c
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#include "gpio.h"

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"
#include "uart_io.h"

#include "timer.h"


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


/** @brief Global flag for button event. */
vuint32_t button_down_pressed = false;
vuint32_t button_up_pressed = false;


/** BREAK: decidir quais pinos gpio usar pra botões
 * @brief Configures the button interrupt on GPIO2 pin 6.
 *
 * Enables debounce for pressing hardware'
 * Unmasks line 32 (GPIOINT2A) in the INTC_MIR_CLEAR1 register.
 * Enables the interrupt on pin 6/7 via GPIO2_IRQSTATUS_SET_0.
 * Configures rising edge detection via GPIO2_RISINGDETECT.
 */
void GPIO_IntConfig(void) {
   HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR1) |= (1 << 0);
   HWREG(GPIO2_DEBOUNCINGTIME)   = 0xFF; // ~8ms

   HWREG(GPIO2_DEBOUNCENABLE)   |= BUTTON_DOWN | BUTTON_UP;
   HWREG(GPIO2_IRQSTATUS_SET_0) |= BUTTON_DOWN | BUTTON_UP;

   HWREG(GPIO2_RISINGDETECT)    |= BUTTON_DOWN | BUTTON_UP;
   
   HWREG(GPIO2_IRQSTATUS_0) = BUTTON_DOWN | BUTTON_UP;
   button_down_pressed = false;
   button_up_pressed   = false;
}


/** BREAK: fazer essa função ser reaproveitável
 * @brief GPIO1 interrupt service routine.
 *
 * Clears the interrupt flag for pin 6/7 in hardware and signals the event
 * to @c main() through @c flag_gpio.
 */
void GPIO_ISR(void) {
   uint32_t status =  HWREG(GPIO2_IRQSTATUS_0);
   HWREG(GPIO2_IRQSTATUS_0) = status & (BUTTON_DOWN | BUTTON_UP); 
   
   if(status & BUTTON_DOWN){
      button_down_pressed = true;
   }
   if(status & BUTTON_UP){
     button_up_pressed = true;
   }
}