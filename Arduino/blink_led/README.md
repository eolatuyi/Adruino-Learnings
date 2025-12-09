# Blinking LED Project

A minimal baremetal example toggling an LED (PB5) on and off using the ATmega328P GPIO interface.

**Tested on Hardware:** This example has been tested on real ATmega328P-based Arduino hardware (e.g. Arduino Uno). The onboard LED on digital pin 13 (PB5) toggles as expected.

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

### Upload Script (Windows)

An upload helper batch file `blink_led_upload.bat` is provided/generated to simplify flashing on Windows. It calls `avrdude` with the typical Arduino bootloader settings. Edit the `COM` port and baud rate as needed.

Example `blink_led_upload.bat` contents:

```bat
@echo off
rem Update COM port as necessary (COM3 used as example)
set COMPORT=COM3
set BAUD=115200

rem Write Intel HEX to flash
avrdude -F -V -c arduino -p ATMEGA328P -P %COMPORT% -b%BAUD% -U flash:w:blink_led.hex:i
pause
```

Run the batch file from the `Arduino/blink_led` directory after building:

```powershell
.\blink_led_upload.bat
```

### S19 (Motorola S-record) Output

An S19 file is also generated to demonstrate the compact size of the final firmware. To produce an S-record from the ELF:

```bash
avr-objcopy -O srec -R .eeprom blink_led.elf blink_led.s19
```

You can inspect the S19 or HEX file sizes to get a quick impression of how small the firmware is:

```powershell
Get-ChildItem blink_led.hex, blink_led.s19 | Select-Object Name, Length
```

## References

- [Arduino Platform Specification](https://code.google.com/archive/p/arduino/wikis/Platforms.wiki)
- **BOARD.build.mcu**: e.g., "atmega328" or "atmega1280"
- **BOARD.build.f_cpu**: e.g., "16000000L" (16 MHz) or "8000000L" (8 MHz internal)
- **BOARD.build.core**: Location of core files to link against (e.g., "arduino")
