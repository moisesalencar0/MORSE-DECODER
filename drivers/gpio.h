/**
 * @file gpio.h
 * @brief GPIO (General-Purpose Input/Output) driver interface.
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

/**
 * @brief Activates GPIO1 and GPIO2 module clocks.
 *
 * Enables the clock signals required by the GPIO1 and GPIO2 peripherals
 * and blocks execution until both modules report that the clocks are enabled.
 */
void GPIO_Init(void);

/** @brief Global flag for button event. */
extern vuint32_t button_down_pressed;
extern vuint32_t button_up_pressed;
extern vuint32_t mode;

/** 
 * @brief Configures the button interrupt on GPIO2 pin 6.
 *
 * Enables debounce for pressing hardware'
 * Unmasks line 32 (GPIOINT2A) in the INTC_MIR_CLEAR1 register.
 * Enables the interrupt on pin 6/7/22 via GPIO2_IRQSTATUS_SET_0.
 * Configures rising edge detection via GPIO2_RISINGDETECT.
 */
void GPIO_IntConfig(void);

/** 
 * @brief GPIO1 interrupt service routine.
 *
 * Clears the interrupt flag for pin 6/7/22 in hardware and signals the event
 * to @c main() through @c flag_gpio.
 */
void GPIO_ISR(void);

#endif /* GPIO_H */