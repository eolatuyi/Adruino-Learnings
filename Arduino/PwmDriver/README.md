# PWM Driver Project

PWM (Pulse-Width Modulation) driver for the ATmega328P using Timer/Counter peripherals.

## Overview

This project abstracts the Timer/Counter peripheral interface to provide a platform-independent PWM driver. The implementation is optimized for use with RGB LED control and other PWM-based applications.

## Hardware Pins and Timer Mapping

### Timer/Counter0 (8-bit)

- **OC0A / PB3** (Port B, Bit 3)
  - Output Compare Match A
  - Configured as output via DDB3
  - PWM mode timer output

- **OC0B / PD5** (Port D, Bit 5)
  - Output Compare Match B
  - Configured as output via DDD5
  - PWM mode timer output

### Timer/Counter2 (8-bit)

- **OC2A / PB3** (Port B, Bit 3)
  - Output Compare Match A
  - Configured as output via DDB3
  - PWM mode timer output

## Driver Architecture

The PWM driver is designed with the following principles:

1. **Platform Independence**: Separate platform-specific register operations from the core driver logic
2. **Configurability**: Runtime duty cycle and frequency control via OCR (Output Compare Register) writes
3. **Abstraction**: Hide timer/counter complexity behind simple API functions

### Core Functions

```c
void pwm_driver_init(void);
// Initializes timer/counter peripherals and GPIO pins for PWM output
```

See `pwm_driver.h` for the public API.

## Build

```bash
make TESTBED_DIR=../installer avr
```

The testbed (`pwm_driver_testbed.c`) demonstrates RGB LED PWM control via USART commands.

## Future Improvements

- Dynamic frequency control (prescaler adjustment)
- Module-level API for pin abstraction (pin aliasing)
- Support for 16-bit Timer/Counter1
