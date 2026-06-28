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



























/*
#include "morse.h"
#include "watchdog.h"
#include "gpio.h"
#include "timer.h"
#include "intc.h"
#include "uart_io.h"


int _main(void) {
    Watchdog_Disable();

    GPIO_Init();
    DMTimer_Init();
    INTC_Init();

    GPIO_Pin_Input_Enable(BUTTON_DOWN | BUTTON_UP);
    GPIO_IntConfig();
    DMTimer_IntConfig();

    // falta habilitar IRQ na CPU (bit no CPSR) -- mesma função/asm que você
    // já usa no lab do LED, não está nos arquivos que você me mandou

   

    while (1){
         run_morse_to_text();
    } // trava aqui de propósito, só pra esse teste único
}



*/