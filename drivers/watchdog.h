/**
 * @file  watchdog.h
 * @brief Watchdog timer driver interface.
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/**
 * @brief Disables the watchdog timer.
 *
 * Writes the required unlock sequence to stop the watchdog
 * and waits for write completion after each step.
 */
void Watchdog_Disable(void);

/**
 * @brief Triggers (kicks) the watchdog to prevent reset.
 *
 * Writes alternating values to the watchdog trigger register
 * and waits for write completion.
 */
void Watchdog_Trigger(void);

#endif /* WATCHDOG_H_ */