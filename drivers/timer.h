/**
 * @file timer.h
 * @brief Timer utilities for AM335x (TRM 8).
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "hw_types.h"

void timer_setup(void);
void delay(uint32_t us); // in seconds

extern vuint32_t flag_timer;
void timerIntConfig(void);
void DMTimerIsr(void);

#endif /* TIMERS_H_ */