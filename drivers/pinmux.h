/**
 * @file pinmux.h
 * @brief Control module pin multiplexing (TRM 9).
 */

#include "hw_types.h"
#include "soc_AM335x.h"

#ifndef PINMUX_H_
#define PINMUX_H_

/**
 * @brief Configures a GPIO pin as output/input via pin multiplexing.
 *
 * @param pin GPIO pin identifier.
 */
void PINMUX_GPIO_OUTPUT(uint32_t pin);
void PINMUX_GPIO_INPUT(uint32_t pin);

#endif /* PINMUX_H_ */