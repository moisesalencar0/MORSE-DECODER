#ifndef _MORSE_
#define _MORSE_
#include "hw_types.h"

typedef struct MorseCode MorseCode;

/** @brief Decodes a dot/dash buffer into its corresponding character. */
char decode_letter(const char *symbols, uint32_t len);

/** @brief Reads Morse button input and decodes it into text. */
void morse_to_text(void);

/** @brief Reads a UART string and transmits it as Morse code. */
void text_to_morse(void);

#endif
