/**
 * @file  gpio.c
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#include "gpio.h"

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"
#include "uart_io.h"

#include "timer.h"

#define SETTING  ((1<<18) | 2)


void GPIO_Init(void){
   HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |= SETTING;
   while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) & SETTING) == 0){};
    
   HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |= SETTING;
   while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) & SETTING) == 0){};
}


vuint32_t button_down_pressed = false;
vuint32_t button_up_pressed = false;
vuint32_t mode = 0;


void GPIO_IntConfig(void) {
   HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR1) |= (1 << 0);
   HWREG(GPIO2_DEBOUNCINGTIME)   = 0xFF; // ~8ms

   HWREG(GPIO2_DEBOUNCENABLE)   |= BUTTON_DOWN | BUTTON_UP | BUTTON_MODE;
   HWREG(GPIO2_IRQSTATUS_SET_0) |= BUTTON_DOWN | BUTTON_UP | BUTTON_MODE;

   HWREG(GPIO2_RISINGDETECT)    |= BUTTON_DOWN | BUTTON_UP | BUTTON_MODE;
   
   HWREG(GPIO2_IRQSTATUS_0) = BUTTON_DOWN | BUTTON_UP | BUTTON_MODE;
   button_down_pressed = false;
   button_up_pressed   = false;
   mode   = 0;
}


void GPIO_ISR(void) {
   uint32_t status =  HWREG(GPIO2_IRQSTATUS_0);
   HWREG(GPIO2_IRQSTATUS_0) = status & (BUTTON_DOWN | BUTTON_UP | BUTTON_MODE); 
   
   if(status & BUTTON_DOWN) button_down_pressed = true;
   if(status & BUTTON_UP)   button_up_pressed = true;
   if(status & BUTTON_MODE) mode ^= 1;
}