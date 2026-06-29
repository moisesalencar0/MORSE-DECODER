#ifndef _MORSE_
#define _MORSE_
#include "hw_types.h"

typedef struct MorseCode MorseCode;



/*função responsável por transformar os pontos e traços em uma letra*/
char decode_letter(const char *symbols, uint32_t len);

/*traduz o conteudo dos botões para texto e o tratamento de enter*/
void run_morse_to_text(void);

/*recebe string na uart e converte em transmissão de código morse*/
void Text_To_Morse(void);



#endif
