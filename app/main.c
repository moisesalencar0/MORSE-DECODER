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
#include "uart_io.h"

/**
 * @brief Application entry point.
 *
 * Initializes the board and runs the main loop:
 * [...]
 */
/*
void _main(void){
    Board_Init();
    
    while(1){
        if (button_up_pressed || button_down_pressed) {

            DMTimer_Delay(50);
            
            if (button_up_pressed && button_down_pressed) {
                button_up_pressed   = false;
                button_down_pressed = false;

                printString("both\r\n", 6);

            } else if (button_up_pressed) {
                button_up_pressed = false;

                printString("up\r\n", 4);

            } else {
                button_down_pressed = false;

                printString("down\r\n", 6);
            }
        }
    }

    while (Pin_Read(BUTTON_UP) || Pin_Read(BUTTON_DOWN)){
        DMTimer_Delay(100);
    }
    button_up_pressed   = false;
    button_down_pressed = false;
}
*/

// teste ttm

#include "morse.h"

int _main(void) {
    Board_Init();
    
    //Text_To_Morse();
    while (1){
        //text_to_morse();
        run_morse_to_text();
    }

}