#include "morse.h"
#include "gpio.h"
#include "timer.h"
#include "uart_io.h"
#include "ctype.h"

#define TICKS_MS         10   
#define COMBO_GRACE_MS   40   

/* @brief letra, equivalente em código morse 
 * junto com a tabela de conversão
 */
struct MorseCode
{
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

/* quantidade de entradas, calculada automaticamente pelo tamanho do array */ 
#define MORSE_TABLE_SIZE (sizeof(morse_table) / sizeof(morse_table[0]))

// converte um buffer de pontos/traços na letra correspondente
char decode_letter(const char *symbols, uint32_t len){
    for(uint32_t i = 0; i < MORSE_TABLE_SIZE; i++){
        uint32_t j = 0;

        // avança enquanto o código da tabela bate com o que foi digitado
        while (morse_table[i].code[j] != '\0' &&
               j < len &&
               morse_table[i].code[j] ==
               symbols[j]){
            j++;
        }
        // só é match se os dois terminaram juntos (mesmo tamanho)
        if(morse_table[i].code[j] == '\0' && j == len){
            return morse_table[i].letter;
        }
    }
    // nenhuma entrada bateu: código inválido
    return '?';
}

void run_morse_to_text(void){

    char symbol_buf[8] = {0}; // pontos/traços da letra sendo digitada agora
    uint32_t symbol_len = 0;   // quantos símbolos já estão em symbol_buf

    char message_buf[128]; // frase decodificada, montada letra por letra
    message_buf[0] = '\0';
    uint32_t message_len = 0;

    uint32_t combo_counter = 0; // combos VAZIOS seguidos (sem nenhum ponto/traço no meio)

    char pending_symbol = 0;  // último clique solo, aguardando confirmar se é combo
    uint32_t pending_ms = 0;  // tempo que pending_symbol está esperando

    printString("\n--- MODO: MORSE -> TEXTO ---\n", 30);
    printString("Digite os codigos. Combo (os 2 botoes juntos) fecha a letra atual.\n", 67);
    printString("Combo SEM letra pendente = espaco. Combo vazio 2x seguido = ENTER.\n\n", 68);

    while(1){
        // combo "no mesmo tick": os dois botões já chegaram juntos
        uint32_t is_combo = (button_down_pressed && button_up_pressed) ||
                           (button_down_pressed && Pin_Read(BUTTON_UP)) ||
                           (button_up_pressed && Pin_Read(BUTTON_DOWN));

        // combo "atrasado": o parceiro chegou depois, dentro da janela de tolerância
        if (!is_combo && pending_symbol == '.' && button_up_pressed)   is_combo = 1;
        if (!is_combo && pending_symbol == '-' && button_down_pressed) is_combo = 1;

        if(is_combo){
            button_down_pressed = false;
            button_up_pressed = false;
            pending_symbol = 0; // descarta: nunca foi clique solo de verdade

            if(symbol_len > 0){
                // tem letra pendente: este combo so FECHA ela, sem espaço no texto
                char letter = decode_letter(symbol_buf, symbol_len);
                if(letter != '?'){
                    message_buf[message_len++] = letter;
                }
                symbol_len = 0;
                combo_counter = 0; // combo "gastou a função" fechando letra, não conta pra separador
                printChar(' '); // eco cru: espaço = separador de LETRA no morse escrito
            }
            else{
                // sem letra pendente: combo vazio = usuario quer separador
                combo_counter++;

                if(combo_counter == 1){
                    // primeiro combo vazio = espaço (fim de palavra) no texto decodificado
                    if(message_len > 0 && message_buf[message_len - 1] != ' '){
                        message_buf[message_len++] = ' ';
                        printChar('/'); // eco cru: "/" = separador de PALAVRA no morse escrito
                    }
                }
                else{
                    // segundo combo combo vazio seguido = fim de transmissão
                    while(message_len > 0 && message_buf[message_len - 1] == ' '){
                        message_len--; // remove espaços sobrando no final
                    }
                    message_buf[message_len] = '\0';

                    printString("\n\n[Transmissao Encerrada via Combo Vazio 2x]", 44);
                    printString("\nFrase Completa Decodificada: ", 30);
                    printString(message_buf, message_len);
                    printString("\n---------------------------------------\n", 41);

                    return; // sai da função, volta pro menu
                }
            }
        }
        else if (button_down_pressed || button_up_pressed) {
            // clique solo: ainda não sabe se vai virar combo
            char c = button_down_pressed ? '.' : '-';
            button_down_pressed = false;
            button_up_pressed   = false;

            if (pending_symbol != 0) {
                // ja tinha um pendente e não virou combo: confirma como símbolo real
                if (symbol_len < 7) {
                    symbol_buf[symbol_len++] = pending_symbol;
                    printChar(pending_symbol);
                }
                combo_counter = 0; // quebrou a sequência de combos vazios
            }
            pending_symbol = c; // o clique atual passa a ser o novo pendente
            pending_ms = 0;
        }

        DMTimer_Delay(TICKS_MS); // heartbeat de 10 ms
        pending_ms += TICKS_MS;

        // janela de tolerância acabou sem parceiro chegar: confirma como solo
        if (pending_symbol != 0 && pending_ms >= COMBO_GRACE_MS) {
            if (symbol_len < 7) {
                symbol_buf[symbol_len++] = pending_symbol;
                printChar(pending_symbol);
            }
            combo_counter = 0;
            pending_symbol = 0;
        }
    }
}

/* dont touch above */

void Transmit_Morse(char c){
    char target_char = (c >= 'a' && c <= 'z') ? c - 32 : c;

    for(uint32_t i = 0; i < MORSE_TABLE_SIZE; i++){
        if(target_char == morse_table[i].letter){
            const char *code = morse_table[i].code;
    
            for(uint32_t j = 0; code[j] != '\0'; j++){
                if (code[j] == '.') {
                    Led_On(USER_LED);
                    Buzzer_On;
                    DMTimer_Delay(200);
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

void Text_To_Morse(void){
    printString("\r\n", 2);
    printString("╔══════════════════════════════════════╗\r\n", 42);
    printString("║       MODO: TEXTO  ->  MORSE         ║\r\n", 42);
    printString("╠══════════════════════════════════════╣\r\n", 42);
    printString("║  Digite a frase e pressione Enter    ║\r\n", 42);
    printString("║  [SWITCH] para trocar de modo        ║\r\n", 42);
    printString("╚══════════════════════════════════════╝\r\n", 42);
    printString("\r\n> ", 3);

    char input_buffer[101];
    scanString(input_buffer);

    printString("\r[Transmitindo...]\r\n", 20);

    int32_t i = 0;
    while(input_buffer[i] != '\0'){
        if(input_buffer[i] == ' '){
            DMTimer_Delay(1200);
            i++;
            continue;
        }
        Transmit_Morse(input_buffer[i]);
        i++;
    }
}