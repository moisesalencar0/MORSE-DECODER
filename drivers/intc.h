/**
 * @file  intc.h
 * @brief ARM interrupt controller (AINTC) initialization and dispatcher.
 */

#ifndef INTC_H_
#define INTC_H_

/**
 * @brief Initializes the ARM interrupt controller (AINTC).
 *
 * Performs a soft-reset, waits for completion and sets
 * the priority threshold to allow all interrupt lines.
 */
void INTC_Init(void);

#endif /* INTC_H_ */