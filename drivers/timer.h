/**
 * @file timer.h
 * @brief Timer utilities for AM335x (TRM 8).
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "hw_types.h"

void DMTimer_Delay(uint32_t us); // in seconds

void DMTimer_Init(void);
void DMTimer_IntConfig(void);
void DMTimer_ISR(void);

extern vuint32_t timer_overflow;

#endif /* TIMER_H_ */