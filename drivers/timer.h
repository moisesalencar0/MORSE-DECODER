/**
 * @file timer.h
 * @brief Timer utilities for AM335x (TRM 8).
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "hw_types.h"

void DMTimer_Delay(uint32_t us); // in seconds

/**
 * @brief Configures DMTimer7 for delay generation.
 *
 * Activates clock.
 * Enables posted mode, stops the timer and resets
 * counter and load registers to a clean state
 */
void DMTimer_Init(void);

/** @brief Unmasks DMTimer7 interrupt line (95) in the INTC. */
void DMTimer_IntConfig(void);

/**
 * @brief DMTimer7 overflow interrupt service routine.
 *
 * Clears the overflow flag in hardware and signals
 * the delay function through @c timer_overflow.
 */
void DMTimer_ISR(void);

extern vuint32_t timer_overflow;

#endif /* TIMER_H_ */