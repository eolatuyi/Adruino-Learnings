# Installer / Testbed Project

A comprehensive testbed for shift-register control, USART serial communication, and bootloader functionality on the ATmega328P.

## Overview

This project contains:

- **Shift Register Driver** (`shift_register.c/.h`): 74HC595-style shift register abstraction with serial communication
- **USART Driver** (`usart.c/.h`): Serial communication (TX/RX) for command/response protocols
- **Serial Communication Tools** (`serial_port.c/.h`): Windows-based serial port interface (C#/.NET-style API)
- **Bootloader** (`boot_loader/boot.c/.h`): Self-programming flash memory via serial
- **Testbed** (`shift_register_testbed.c`): Main application integrating all drivers

## Building

### Prerequisites

- AVR toolchain (avr-gcc, avr-libc, avrdude)
- GNU make

### Compile

```bash
make avr
```

Or with custom settings:

```bash
make MCU_TARGET=atmega328p COM_PORT=COM4 BAUD_RATE=115200 avr
```

### Clean

```bash
make clean
```

## Shift Register Hardware Setup

### Pin Mapping

| Signal | AVR Pin | Direction | Active Level |
|--------|---------|-----------|--------------|
| SER (Data) | PB4 | Output | High |
| SRCLK (Shift Clock) | PD4 | Output | High |
| RCLK (Latch Clock) | PD7 | Output | High |
| SRCLR (Clear) | PD2 | Output | Low |
| OE (Output Enable) | PB0 | Output | Low |

### Driver API

```c
void shift_register_init(shift_register_cblk_t *cblk);
void shift_register_set_output(shift_register_cblk_t *cblk, uint8_t output_num, shift_register_output_state_t state);
void shift_register_clear_register(shift_register_cblk_t *cblk);
void shift_register_latch_data_to_output(shift_register_cblk_t *cblk);
```

## USART Communication

### Configuration

- **Baud Rate**: 9600 bps (configurable)
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None
- **Frame Format**: 8-N-1

### Driver API

```c
void usart_init(void);
void usart_byte_tx(uint8_t const *byte_tx);
uint8_t usart_byte_rx(uint8_t *byte_rx, uint8_t const rx_packet_size);
```

## Flash and Program

### Using avrdude

```bash
# Backup current firmware
avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b115200 -U flash:r:backup.hex:i

# Program with testbed
avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b115200 -U flash:w:shift_register_testbed.hex:i
```

### Using Generated Script

```bash
./shift_register_testbed_upload.bat
```

Adjust COM port and baud rate in the Makefile or environment as needed.

## Bootloader Notes

The bootloader implementation demonstrates:

1. **Flash Programming**: Filling page buffers and executing Self-Programming Memory (SPM) instructions
2. **Serial Protocol**: Receiving binary data over USART
3. **RWW Section Management**: Enabling Read-While-Write sections after programming

See `boot_loader/boot.c` for full implementation. Reference ATmega328P datasheet (Section 27, Self-Programming) for details.

## Serial Communication (Windows)

The `serial_port.c` module provides asynchronous Windows serial port I/O using overlapped I/O:

```c
int serial_port_init(HANDLE *hComm, int baud_rate, DCB *dcbSerialParams, COMMTIMEOUTS *timeouts);
int serial_port_tx(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, const uint8_t *buffer, const DWORD tx_data_size);
int serial_port_rx(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, uint8_t *buffer, const DWORD rx_data_size);
```

This is a host-side utility for sending commands to the testbed firmware.

## References

- ATmega328P Datasheet
  - Section 18: Timer/Counter0
  - Section 27: Self-Programming Memory (Bootloader)
  - Section 32: USART
- Arduino Platform Specification: [Platforms.wiki](https://code.google.com/archive/p/arduino/wikis/Platforms.wiki)
