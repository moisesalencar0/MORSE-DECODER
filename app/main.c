/**
 * @file   main.c
 * @brief  [...]
 * @author Moises Alencar (UFC) @moisesalencar0 / Rafael de Castro (UFC) @rafaelccastro007
 * @date   2026-06-22
 */

#include "board.h"
#include "gpio.h"
#include "timer.h"
#include "watchdog.h"
#include "i2c.h"

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
            Buzzer_On;
            DMTimer_Delay(2);
        }
        else Buzzer_Off;
    }
}

