/**
 * @file  morse.c
 * @brief Morse code translation implementation.
 *
 * Implements the text-to-Morse and Morse-to-text translation modes,
 * including character encoding, decoding, and user interaction.
 */

#include "morse.h"

#include "gpio.h"
#include "timer.h"
#include "uart_io.h"
#include "ctype.h"

#define TICKS_MS   10   
#define COMBO_MS   40   

/**
 * @brief Letter-to-Morse mapping entry.
 */
struct MorseCode {
    char letter;
    const char *code;
};

static const MorseCode morse_table[] = {
    {'A', ".-"},    {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},  {'E', "."},
    {'F', "..-."},  {'G', "--."},  {'H', "...."}, {'I', ".."},   {'J', ".---"},
    {'K', "-.-"},   {'L', ".-.."}, {'M', "--"},   {'N', "-."},   {'O', "---"},
    {'P', ".--."},  {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
    {'U', "..-"},   {'V', "...-"}, {'W', ".--"},  {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."},  {' ', "/"},

    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
    {'8', "---.."}, {'9', "----."}
};

#define MORSE_TABLE_SIZE (sizeof(morse_table) / sizeof(morse_table[0]))

/* ============================================================================
 * MORSE -> TEXT RELATED FUNCTIONS
 * ========================================================================= */

/**
 * @brief Decodes a dot/dash buffer into its corresponding character.
 * @param symbols Buffer containing the dot/dash sequence.
 * @param len     Number of symbols in the buffer.
 * @return Decoded character, or '?' if no match is found.
 */
char decode_letter(const char *symbols, uint32_t len){
    for(uint32_t i = 0; i < MORSE_TABLE_SIZE; i++){
        uint32_t j = 0;
        while (morse_table[i].code[j] != '\0' &&
               j < len &&
               morse_table[i].code[j] == symbols[j]){
            j++;
        }
        if(morse_table[i].code[j] == '\0' && j == len){
            return morse_table[i].letter;
        }
    }
    return '?';
}

void morse_to_text(void){
    char symbol_buf[8] = {0};
    uint32_t symbol_len = 0;

    char message_buf[128];
    message_buf[0] = '\0';
    uint32_t message_len = 0;

    uint32_t combo_counter = 0;
    char pending_symbol = 0;
    uint32_t pending_ms = 0;

    printString("\r\n", 2);
    printString("╔══════════════════════════════════════╗\r\n", 122);
    printString("║       MODO: MORSE  ->  TEXTO         ║\r\n", 46);
    printString("╠══════════════════════════════════════╣\r\n", 122);
    printString("║ combo: 1x: letra, 2x: [ ], 3x: enter ║\r\n", 46);
    printString("╚══════════════════════════════════════╝\r\n", 122);
    printString("> ", 2);

    volatile uint32_t entry_mode = mode;
    while(1){
        if (mode != entry_mode) return;

        uint32_t is_combo = 0;
        if (button_up_pressed || button_down_pressed) {
            DMTimer_Delay(100);
            is_combo = (button_down_pressed && button_up_pressed);
        }

        if(is_combo) {
            button_down_pressed = false;
            button_up_pressed = false;
            pending_symbol = 0;

            if(symbol_len > 0) {
                char letter = decode_letter(symbol_buf, symbol_len);
                if(letter != '?')
                    message_buf[message_len++] = letter;
                symbol_len = 0;
                combo_counter = 0;
                printChar(' ');
            }
            
            else {
                combo_counter++;
                if(combo_counter == 1){
                    if(message_len > 0 && message_buf[message_len - 1] != ' '){
                        message_buf[message_len++] = ' ';
                        printChar('/');
                    }
                }
                else {
                    while(message_len > 0 && message_buf[message_len - 1] == ' '){
                        message_len--;
                    }
                    message_buf[message_len] = '\0';
                    printString("\r\nDecodificado: ", 15);
                    printString(message_buf, message_len);
                    printString("\r\n", 2);
                    return;
                }
            }
        }
        else if (button_down_pressed || button_up_pressed) {
            char c = button_down_pressed ? '.' : '-';
            button_down_pressed = false;
            button_up_pressed   = false;

            if (pending_symbol != 0) {
                if (symbol_len < 7) {
                    symbol_buf[symbol_len++] = pending_symbol;
                    printChar(pending_symbol);
                }
                combo_counter = 0;
            }
            pending_symbol = c;
            pending_ms = 0;
        }

        DMTimer_Delay(TICKS_MS);
        pending_ms += TICKS_MS;

        if (pending_symbol != 0 && pending_ms >= COMBO_MS) {
            if (symbol_len < 7) {
                symbol_buf[symbol_len++] = pending_symbol;
                printChar(pending_symbol);
            }
            combo_counter = 0;
            pending_symbol = 0;
        }
    }
}

/* ============================================================================
 * TEXT -> MORSE RELATED FUNCTIONS
 * ========================================================================= */

/**
 * @brief Transmits a character as Morse code via LED and buzzer.
 *
 * Dot = 150 ms, dash = 600 ms, 200 ms gap between symbols.
 * Aborts early if mode changes. Lowercase is auto-converted to uppercase.
 *
 * @param c          Character to transmit.
 * @param entry_mode Mode value at call time; used to detect mode changes.
 */
void transmit_morse(char c, uint32_t entry_mode){
    char target_char = (c >= 'a' && c <= 'z') ? c - 32 : c;

    for(uint32_t i = 0; i < MORSE_TABLE_SIZE; i++){
        if(target_char == morse_table[i].letter){
            const char *code = morse_table[i].code;
    
            for(uint32_t j = 0; code[j] != '\0'; j++){
                if (mode != entry_mode) return;

                if (code[j] == '.') {
                    Led_On(USER_LED);
                    Buzzer_On;
                    DMTimer_Delay(150);
                    Led_Off(USER_LED);
                    Buzzer_Off;
                } else {
                    Led_On(USER_LED);
                    Buzzer_On;
                    DMTimer_Delay(600);
                    Led_Off(USER_LED);
                    Buzzer_Off;
                }
                DMTimer_Delay(200);
            }
            break;
        }
    }
}


void text_to_morse(void){
    printString("\r\n", 2);
    printString("╔══════════════════════════════════════╗\r\n", 122);
    printString("║       MODO: TEXTO  ->  MORSE         ║\r\n", 46);
    printString("╠══════════════════════════════════════╣\r\n", 122);
    printString("║  Digite a frase e pressione Enter    ║\r\n", 46);
    printString("║  [SWITCH] para trocar de modo        ║\r\n", 46);
    printString("╚══════════════════════════════════════╝\r\n", 122);
    printString("\r\n> ", 4);

    char input_buffer[101];
    uint32_t idx = 0;
    uint32_t entry_mode = mode;

    while(idx < 100) {
        if (mode != entry_mode) return;

        char c;
        if (!scanChar_Non_Blocking(&c)) { DMTimer_Delay(10); continue; }
        if (c == '\r' || c == '\n') break;
        if ((c == '\x7f' || c == '\b') && idx > 0) { idx--; printString("\b \b", 3); continue; }
        input_buffer[idx++] = c;
        printChar(c);
    }
    input_buffer[idx] = '\0';

    printString("\r[Transmitindo...]\r\n", 20);

    int32_t i = 0;
    while(input_buffer[i] != '\0'){
        if (mode != entry_mode) return;

        if(input_buffer[i] == ' '){
            DMTimer_Delay(1200);
            i++;
            continue;
        }
        transmit_morse(input_buffer[i], entry_mode);
        i++;
    }
}