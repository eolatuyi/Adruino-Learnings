# Blinking LED Project

A minimal baremetal example toggling an LED (PB5) on and off using the ATmega328P GPIO interface.

## Pin Configuration

### Register Overview

- **DDxn (DDRx)**: Data Direction Register
  - `1` = Output Pin direction
  - `0` = Input Pin direction
- **PORTxn (PORTx)**: Pin state
  - `1` = Set output high
  - `0` = Set output low
- **PINxn (PINx)**: Read/toggle
  - Read: Get current pin state
  - Write: Toggle pin irrespective of DDxn state

### 8-bit Addressing

```c
// Set PB5 as output
(uint8_t *)address(DDRB)  |=   1 << bit_number(DDB5);

// Set PB5 high
(uint8_t *)address(PORTB) |=   1 << bit_number(PORTB5);

// Set PB5 low
(uint8_t *)address(PORTB) &= ~(1 << bit_number(PORTB5));
```

## Build and Flash

### Compile

```bash
make avr
```

Or manually:

```bash
# Compile and assemble (do not link)
avr-gcc -Os -DF_CPU=16000000L -mmcu=atmega328p -c -o blink_led.o blink_led.c

# Link to create elf file
avr-gcc -mmcu=atmega328p blink_led.o -o blink_led.elf

# Convert elf to ihex format (Intel HEX)
avr-objcopy -O ihex -R .eeprom blink_led.elf blink_led.hex
```

### Flash to Device

```bash
# Read current firmware (backup)
avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b115200 -U flash:r:blink_led_backup.hex:i

# Write new firmware
avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b115200 -U flash:w:blink_led.hex:i
```

Adjust `COM3` and `115200` as needed for your board and environment.

## References

- [Arduino Platform Specification](https://code.google.com/archive/p/arduino/wikis/Platforms.wiki)
- **BOARD.build.mcu**: e.g., "atmega328" or "atmega1280"
- **BOARD.build.f_cpu**: e.g., "16000000L" (16 MHz) or "8000000L" (8 MHz internal)
- **BOARD.build.core**: Location of core files to link against (e.g., "arduino")
