/**
 * @file uart_io.h
 * @brief UART driver implementations. (TRM 19) 
 */

#ifndef UART_IO_H_
#define UART_IO_H_

#include "hw_types.h"
#include "hw_regs.h"

/**
 * @brief Reads a single character from UART.
 * @return Character received.
 */
uint8_t scanChar(void);

/**
 * @brief Sends a single character via UART.
 * @param c Character to send.
 */
void printChar(uint8_t c);

/**
 * @brief Sends a string via UART.
 * @param str String to send.
 * @param len Length of the string.
 */
uint32_t printString(char *str, uint32_t len);

#endif /* UART_IO_H_ */