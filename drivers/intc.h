/**
 * @file menu.h
 * @brief Displays LED delay menu for selection of timing.
 */

#ifndef IRQ_H_
#define IRQ_H_

#include "hw_types.h"

/**
 * @brief Displays standby message for the menu system.
 *    ->  "Press 'm' to open the menu"
 * Shows initial system status before entering interactive mode.
 */
void MENU_STANDBY(void);

/**
 * @brief Displays LED delay menu and returns selected option.
 *
 * Reads user input via UART and returns a predefined delay value.
 * If the input is invalid, returns the current delay.
 *
 * @param delay Current delay value.
 *
 * @return Selected delay constant (FAST_DELAY, MEDIUM_DELAY, SLOW_DELAY)
 *         or the original value if input is invalid.
 */
uint32_t MENU(uint32_t delay);

void IntAINTCInit(void);

#endif /* MENU_H_ */