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
#include "uart_io.h"
/**
 * @brief Application entry point.
 *
 * Initializes the board and runs the main loop:
 * [..;]
 */
void _main(void){
    Board_Init();
    
    while(1){
        if (button_up_pressed || button_down_pressed) {
        
            DMTimer_Delay(50); // ~50ms, aguarda o segundo botão
            
            if (button_up_pressed && button_down_pressed) {
                button_up_pressed   = false;
                button_down_pressed = false;

                printString("both\n", 5);

            } else if (button_up_pressed) {
                button_up_pressed = false;

                printString("up\n", 3);

            } else {
                button_down_pressed = false;

                printString("down\n", 5);

            }
        }
    }

    while (Pin_Read(BUTTON_UP) || Pin_Read(BUTTON_DOWN)){
        DMTimer_Delay(100);
    }
    button_up_pressed   = false;
    button_down_pressed = false;
}
