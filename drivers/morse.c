#include "morse.h"
#include "gpio.h"
#include "timer.h"
#include "uart_io.h"

#define TICKS_MS         10   // granularidade do loop principal (heartbeat)
//#define LETTER_TICKS_MS  600  // silêncio necessário para fechar uma letra
#define COMBO_GRACE_MS   40   // janela de espera pra confirmar combo (2 botões quase juntos)

// par letra + código morse
struct MorseCode
{
    char letter;
    const char *code;
};

// tabela de busca: serve tanto pra decodificar quanto pra codificar
static const MorseCode morse_table[] = {
    {'A', ".-"},    {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},  {'E', "."},
    {'F', "..-."},  {'G', "--."},  {'H', "...."}, {'I', ".."},   {'J', ".---"},
    {'K', "-.-"},   {'L', ".-.."}, {'M', "--"},   {'N', "-."},   {'O', "---"},
    {'P', ".--."},  {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
    {'U', "..-"},   {'V', "...-"}, {'W', ".--"},  {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."},

    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
    {'8', "---.."}, {'9', "----."}
};

// quantidade de entradas, calculada automaticamente pelo tamanho do array
#define MORSE_TABLE_SIZE (sizeof(morse_table) / sizeof(morse_table[0]))

// converte um buffer de pontos/traços na letra correspondente
char decode_letter(const char *symbols, uint8_t len){
    for(uint8_t i = 0; i < MORSE_TABLE_SIZE; i++){
        uint8_t j = 0;

        // avança enquanto o código da tabela bate com o que foi digitado
        while (morse_table[i].code[j] != '\0' && j < len && morse_table[i].code[j] == symbols[j]){
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
    uint8_t symbol_len = 0;   // quantos símbolos já estão em symbol_buf

    char message_buf[128]; // frase decodificada, montada letra por letra
    message_buf[0] = '\0';
    uint8_t message_len = 0;

    uint32_t idle_ms = 0;      // tempo desde o último clique
    uint8_t combo_counter = 0; // combos seguidos sem nenhum ponto/traço no meio

    char pending_symbol = 0;  // último clique solo, aguardando confirmar se é combo
    uint32_t pending_ms = 0;  // tempo que pending_symbol está esperando

    printString("\n--- MODO MORSE -> TEXTO ---\n", 29);
    printString("Digite os codigos\n. Pressione AMBOS os botoes juntos 3x para dar ENTER.\n\n", 73);

    while(1){
        // combo "no mesmo tick": os dois botões já chegaram juntos
        uint8_t is_combo = (button_down_pressed && button_up_pressed) ||
                           (button_down_pressed && Pin_Read(BUTTON_UP)) ||
                           (button_up_pressed && Pin_Read(BUTTON_DOWN));

        // combo "atrasado": o parceiro chegou depois, dentro da janela de tolerância
        if (!is_combo && pending_symbol == '.' && button_up_pressed)   is_combo = 1;
        if (!is_combo && pending_symbol == '-' && button_down_pressed) is_combo = 1;

        if(is_combo){
            button_down_pressed = false;
            button_up_pressed = false;
            pending_symbol = 0; // descarta: nunca foi clique solo de verdade

            // fecha a letra que estava sendo digitada antes do combo
            if(symbol_len > 0){
                char letter = decode_letter(symbol_buf, symbol_len);
                if(letter != '?'){
                    message_buf[message_len++] = letter;
                }
                symbol_len = 0;
            }
            combo_counter++; // mais um combo na sequência

            if(combo_counter < 3){
                // 1º ou 2º combo: separa palavras com espaço
                if(message_len > 0 && message_buf[message_len - 1] != ' '){
                    message_buf[message_len++] = ' ';
                    printChar(' ');
                }
                idle_ms = 0;
            }
            else{
                // 3º combo seguido: fim de transmissão
                while(message_len > 0 && message_buf[message_len -1] == ' '){
                    message_len--; // remove espaços sobrando no final
                }
                message_buf[message_len] = '\0';

                printString("\n\n[Transmissao Encerrada via 3x Combo]", 37);
                printString("\nFrase Completa Decodificada: ", 30);
                printString(message_buf, message_len);
                printString("\n---------------------------------------\n", 41);

                return; // sai da função, volta pro menu
            }
        }
        else if (button_down_pressed || button_up_pressed) {
            // clique solo: ainda não sabemos se vai virar combo
            char c = button_down_pressed ? '.' : '-';
            button_down_pressed = false;
            button_up_pressed   = false;

            if (pending_symbol != 0) {
                // já tinha um pendente e não virou combo: confirma como símbolo real
                if (symbol_len < 7) {
                    symbol_buf[symbol_len++] = pending_symbol;
                    printChar(pending_symbol);
                }
                combo_counter = 0; // quebrou a sequência de combos
            }
            pending_symbol = c; // o clique atual passa a ser o novo pendente
            pending_ms = 0;
            idle_ms = 0;
        }

        DMTimer_Delay(TICKS_MS); // heartbeat de 10 ms
        idle_ms += TICKS_MS;
        pending_ms += TICKS_MS;

        // janela de tolerância acabou sem parceiro chegar: confirma como solo
        if (pending_symbol != 0 && pending_ms >= COMBO_GRACE_MS) {
            if (symbol_len < 7) {
                symbol_buf[symbol_len++] = pending_symbol;
                printChar(pending_symbol);
            }
            combo_counter = 0;
            pending_symbol = 0;
            idle_ms = 0;
        }
    }
}