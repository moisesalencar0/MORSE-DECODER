/**
 * @file   main.c
 * @brief  [...]
 * @author Moises Alencar (UFC) @moisesalencar0 / [...]
 * @date   2026-06-22
 */

#include "board.h"
#include "gpio.h"
#include "timer.h"
#include "watchdog.h"

/**
 * @brief Application entry point.
 *
 * Initializes the board and runs the main loop:
 * [..;]
 */
void _main(void){
    Board_Init();
    
    while(1){
        if(button_pressed){
            button_pressed = false;
            Led_On(DEBUG_LED);
            DMTimer_Delay(2);
        }
        else Led_Off(DEBUG_LED);//
    }
}

