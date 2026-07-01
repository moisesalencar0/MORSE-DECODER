/**
 * @file  gpio.h
 * @brief GPIO (General Purpose Input/Output) driver implementations
 */

#ifndef GPIO_H
#define GPIO_H

#include "soc_AM335x.h"
#include "hw_regs.h"
#include "hw_types.h"

/* ============================================================================
 * BOARD PIN MASKS
 * ========================================================================= */

#define BUTTON_DOWN                  (1 << 6)
#define BUTTON_UP                    (1 << 7)
#define BUTTON_MODE                  (1 << 22)
#define BUZZER                       (1 << 17)
#define DEBUG_LED                    (1 << 21) // first internal led
#define USER_LED                     (1 << 28) // red led

/* ============================================================================
 * GPIO API
 * ========================================================================= */

#define Led_On(pin)                   HWREG(GPIO1_SETDATAOUT) = (pin)
#define Led_Off(pin)                  HWREG(GPIO1_CLEARDATAOUT) = (pin)
#define Buzzer_On                     HWREG(GPIO1_SETDATAOUT) = BUZZER
#define Buzzer_Off                    HWREG(GPIO1_CLEARDATAOUT) = BUZZER

#define GPIO_Pin_Output_Enable(pin)   HWREG(GPIO1_OE) &= ~(pin)
#define Pin_Read(pin)                (HWREG(GPIO2_DATAIN) & (pin))
#define GPIO_Pin_Input_Enable(pin)    HWREG(GPIO2_OE) |= (pin)


void GPIO_Init(void);

extern vuint32_t button_down_pressed;
extern vuint32_t button_up_pressed;
extern vuint32_t mode;

void GPIO_IntConfig(void);

void GPIO_ISR(void);

#endif /* GPIO_H */