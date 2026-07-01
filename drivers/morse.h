/**
 * @file  morse.h
 * @brief Morse code translation interface.
 *
 * Declares the functions that implement bidirectional translation
 * between text and Morse code.
 */

#ifndef _MORSE_H
#define _MORSE_H
#include "hw_types.h"

typedef struct MorseCode MorseCode;

/** @brief Decodes a dot/dash buffer into its corresponding character. */
char decode_letter(const char *symbols, uint32_t len);

/**
 * @brief Reads Morse button input and decodes it into text.
 *
 * Combo rules: 1 combo closes the current letter; 2 consecutive empty
 * combos insert a word space; 3 trigger end-of-transmission and print
 * the decoded message.
 */
void morse_to_text(void);

/**
 * @brief Reads a text line from UART and transmits it as Morse code.
 *
 * Accepts input until Enter or 100 characters. Supports backspace.
 * Spaces produce a 1200 ms gap. Aborts if mode changes.
 */
void text_to_morse(void);

#endif /* MORSE_H_ */
