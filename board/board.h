/**
 * @file  board.h
 * @brief Board definitions and initialization interface.
 */

#ifndef BOARD_H
#define BOARD_H

/**
 * @brief Initializes board clocks, pinmux and GPIO directions.
 *
 * Configures GPIO clocks, sets pin multiplexing for LEDs and buttons,
 * and defines GPIO directions (output for LEDs, input for buttons).
 */
void Board_Init(void);

#endif /* BOARD_H */