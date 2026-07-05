/**
 * @file   main.c
 * @brief  Logical abstraction for managing key driver functions.
 * @author Moises Alencar (UFC) @moisesalencar0 / Rafael de Castro (UFC) @rafaelccastro007
 * @date   2026-06-22
 */

#include "board.h"
#include "morse.h"
#include "gpio.h"
#include "timer.h"
#include "watchdog.h"
#include "uart_io.h"

/**
 * @brief Application entry point.
 * Initializes the board and runs the main loop
 */
int _main(void) {
    Board_Init();
    Menu_Morse();
    while (1){
        if(mode == 0){
            morse_to_text();
        } else {
            text_to_morse();
        }
    }
}
