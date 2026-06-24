/**
 * @file   main.c
 * @brief  [...]
 * @author Moises Alencar (UFC) @moisesalencar0 / [...]
 * @date   2026-06-22
 */

#include "board.h"
#include "gpio.h"

/**
 * @brief Application entry point.
 *
 * Initializes the board and runs the main loop:
 * [..;]
 */
void _main(void){
    Board_Init();

    while(1){
        Led_On(DEBUG_LED);
        Led_On(USER_LED);
    }
}
