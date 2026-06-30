/**
 * @file uart_io.c
 * @brief UART driver implementations. (TRM 19) 
 */

#include "uart_io.h"

#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_regs.h"

/**
 * @brief Checks if UART0 has received data available.
 *
 * Evaluates the Line Status Register (LSR) bit 0,
 * which indicates data ready to be read.
 */
#define UART_HAS_DATA() \
    (HWREG(UART0_LSR) & (1 << 0))

/**
 * @brief Transmits a character through UART0.
 *
 * @param c Character to be transmitted.
 */
void printChar(uint8_t c) {
    if(c == '\n'){
        while(!(HWREG(UART0_LSR) & (1<<5)));
        HWREG(UART0_THR) = '\r';
    }
    while(!(HWREG(UART0_LSR) & (1<<5)));
    HWREG(UART0_THR) = c;
}

/**
 * @brief Receives a character from UART0.
 *
 * @return Received character.
 */
uint8_t scanChar(void) {
    while(!(HWREG(UART0_LSR) & (1<<0)));
    return (uint8_t)HWREG(UART0_RHR);
}

/**
 * @brief Transmits a string through UART0.
 *
 * @param str Pointer to the string to be transmitted.
 * @param length Number of characters to transmit.
 *
 * @return Number of transmitted characters.
 */
uint32_t printString(char *str, uint32_t length) {
    for(uint32_t i = 0; i < length; i++) {
        printChar(str[i]);
    }
    return(length);
}

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
uint32_t scanString(char *buf) {
    uint32_t i = 0;

    for (i = 0; i < 100; i++){
        char c = scanChar();
        if (c == '\r' || c == '\n') break;
        buf[i] = c;
        printChar(c);
    }
    
    buf[i] = '\0';
    return i;
}