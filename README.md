# Morse Code to Text and Text to Morse Code Translator

## About the Project

### What does this project implement?

This project implements a bidirectional translator between Morse code and plain text using the BeagleBone Black.

### Objective

The objective of this project was to apply, in practice, the fundamental concepts of microcontroller programming covered throughout the course by developing a complete embedded system involving drivers, interrupts, timers, and onboard peripherals.

### Course

**Programming Techniques for Embedded Systems**


## Demo

[![Morse Translator Demo](https://www.youtube.com/watch?v=qoMjnd7s96E)
---

## Features

The system provides two operating modes.

### Mode 1 — Morse Code → Text

The interface is available through UART (via Minicom), where a selection menu is displayed.

Morse code is entered using two buttons on the BeagleBone Black:

- **Down Button** → dot (`.`)
- **Up Button** → dash (`-`)

Pressing both buttons simultaneously (**combo**) performs special actions:

- **1 press:** confirms the current letter;
- **2 consecutive presses:** inserts a space between words (`/`);
- **3 consecutive presses:** ends the message and starts the decoding process.

Sequences that are not part of the International Morse Code table are ignored.

The input buffer supports up to **128 characters**.

### Mode 2 — Text → Morse Code

The user sends a message through the UART interface.

The system automatically converts each character into Morse code and represents it using:

- LED flashes;
- Buzzer beeps.

---

## Hardware

- BeagleBone Black
- Texas Instruments Sitara AM335x Processor

### Peripherals Used

- GPIO
- DMTimer7
- Interrupt Controller (INTC)
- UART
- Watchdog Timer

---

## Project Structure

For educational purposes, the project was organized into layers.

Each hardware peripheral has its own driver implementation, while the application logic is separated into independent modules, reducing coupling between hardware and application code.

MORSE-DECODER/
├── app/
│   ├── main.c
│   ├── morse.c
│   └── morse.h
├── board/
│   ├── board.c
│   └── board.h
├── drivers/
│   ├── gpio.c / gpio.h
│   ├── intc.c / intc.h
│   ├── timer.c / timer.h
│   ├── uart_io.c / uart_io.h
│   └── watchdog.c / watchdog.h

---

## Compilation

The project must be compiled using an ARM cross-compilation toolchain (arm-none-eabi-gcc)

After compilation, the generated binary is transferred to the BeagleBone Black using **TFTP**.

A PDF document included in the repository (pratica00) explains the complete environment setup and compilation process in detail.

---

## Running the Project

After loading the binary onto the board:

1. Start the application.
2. Connect to the UART interface using Minicom.
3. The main menu will be displayed.
4. Select one of the available operating modes:
   - Morse Code → Text
   - Text → Morse Code

### Inputs

- Physical buttons on the BeagleBone Black;
- Messages sent through UART.

### Outputs

- Decoded text displayed on the UART terminal;
- LED flashes;
- Buzzer beeps.

---

## Internal Architecture

The application's execution flow is coordinated by `main.c`.

Hardware initialization is centralized in `board.c` through the `Board_Init()` function, which configures and initializes all required drivers.

Application modules interact exclusively with these drivers, maintaining a clear separation between application logic and hardware control.

---

## Limitations

- The implementation supports only the basic International Morse Code table and does not include special symbols such as `@`, `$`, `%`, and others.
- The input buffer has a maximum capacity of **128 characters**.
- Morse code input was adapted to use two buttons (dot and dash) due to the physical limitations and precision of the available hardware buttons.
- Although software debouncing and timing tolerances are implemented, reproducing the traditional single-button Morse input method would require greater timing precision.

---

## Authors

- **Moisés Alencar** — Federal University of Ceará (UFC) — GitHub: `@moisesalencar0`
- **Rafael de Castro** — Federal University of Ceará (UFC) — GitHub: `@rafaelccastro007`