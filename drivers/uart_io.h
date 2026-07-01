/**
 * @file  uart_io.h
 * @brief UART input/output interface.
 *
 * Provides functions for transmitting and receiving data through UART0,
 * including blocking and non-blocking character reception.
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
 * @brief Transmits a string through UART0.
 *
 * @param str Pointer to the string to be transmitted.
 * @param length Number of characters to transmit.
 *
 * @return Number of transmitted characters.
 */
uint32_t printString(char *str, uint32_t len);

/**
 * @brief Receives a string from UART0 until Enter is pressed.
 *
 * Reads characters one by one, echoing each back to the terminal,
 * until '\r' or '\n' is received or the buffer is full.
 *
 * @param buf    Buffer to store the received string.
 *
 * @return Number of characters received (excluding '\0').
 */
uint8_t scanChar_Non_Blocking(char *buf);

#endif /* UART_IO_H_ */