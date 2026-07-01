/**
 * @file  uart_io.c
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

void printChar(uint8_t c) {
    if(c == '\n'){
        while(!(HWREG(UART0_LSR) & (1<<5)));
        HWREG(UART0_THR) = '\r';
    }
    while(!(HWREG(UART0_LSR) & (1<<5)));
    HWREG(UART0_THR) = c;
}

uint8_t scanChar(void) {
    while(!(HWREG(UART0_LSR) & (1<<0)));
    return (uint8_t)HWREG(UART0_RHR);
}

uint32_t printString(char *str, uint32_t length) {
    for(uint32_t i = 0; i < length; i++) {
        printChar(str[i]);
    }
    return(length);
}

uint8_t scanChar_Non_Blocking(char *buf) {
    if (!(HWREG(UART0_LSR) & (1<<0))) return 0;
    *buf = (uint8_t)HWREG(UART0_RHR);
    return 1;
}