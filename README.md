# Adruino-Learnings

Learnings from tinkering with baremetal Arduino (ATmega328P) using C and avr-gcc.

## Overview

This repository contains three main embedded use-cases demonstrating core microcontroller concepts:

- **`Arduino/blink_led`** – Minimal GPIO example toggling an LED on/off; foundation for embedded I/O.
- **`Arduino/PwmDriver`** – PWM driver using Timer/Counter peripherals for RGB LED control and duty-cycle modulation.
- **`Arduino/installer`** – Advanced testbed featuring shift-register control, USART serial communication, and bootloader self-programming.

Each folder includes:
- Baremetal C source code and headers
- Portable Makefile with configurable build parameters
- Comprehensive `README.md` with hardware pinouts, build instructions, and API documentation

## Testing & Confidence

**Local Development Status**: 
- **`blink_led`** and **`PwmDriver`** have been tested and validated on real ATmega328P hardware.
- **`installer`** (shift-register testbed and USART driver) have been tested; however, **`boot.c` is in development and has not been exercised on hardware** yet.

**Public Release Status**: This port to a public repository is **yet to be fully re-tested** after refactoring (path removal, Makefile sanitization, and documentation conversion). However, the **core firmware and driver logic remain unchanged**, providing strong confidence in hardware compatibility for tested components.

**Why You Should Trust This Code**:
- **GPIO & PWM**: Demonstrates proven embedded C patterns and working examples tested on real hardware
- **USART & Shift-Register**: Functional serial communication and digital I/O control
- **Bootloader (boot.c)**: Early-stage implementation; use as reference or educational material, not yet production-ready
- **Makefiles**: Portable and tested with MSYS2 and standard avr-gcc toolchain
- **Overall**: Suitable as educational material and production reference for Windows-based AVR development (excluding bootloader)

## Prerequisites

Currently targets **Windows development environment**. You'll need:

- **AVR toolchain**: avr-gcc, avr-libc, avrdude
  - **Option A (Recommended):** Use [MSYS2](https://www.msys2.org/) with `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-avr-gcc mingw-w64-x86_64-avr-libc mingw-w64-x86_64-avrdude`
  - **Option B:** Use [WinAVR](http://winavr.sourceforge.net/) (older but self-contained)
  - **Option C:** Use WSL2 with Linux toolchain
- **GNU Make** (usually bundled with above)
- **Serial port access** to your Arduino (via Device Manager or similar)

## Quick Start

1. Navigate to a use-case:
   ```
   cd Arduino/installer
   ```

2. Build with Make:
   ```
   make avr
   ```
   To override COM port or baud rate:
   ```
   make avr COM_PORT=COM4 BAUD_RATE=115200
   ```

3. Flash the device using the generated `*_upload.bat` or:
   ```
   avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b115200 -U flash:w:shift_register_testbed.hex:i
   ```

## Project Structure

```
Arduino/
├── blink_led/          # Simple GPIO toggle example
│   ├── blink_led.c     # Source
│   ├── Makefile        # Build configuration
│   └── README.md       # Full documentation
│
├── PwmDriver/          # PWM/Timer-Counter driver
│   ├── pwm_driver.c    # Core driver
│   ├── pwm_driver.h    # Public API
│   ├── pwm_driver_testbed.c
│   ├── Makefile
│   └── README.md
│
└── installer/          # Advanced: shift-register + USART + bootloader
    ├── shift_register.c/.h
    ├── usart.c/.h
    ├── shift_register_testbed.c
    ├── serial_communication/
    │   ├── serial_port.c/.h    # Windows serial I/O
    │   └── Makefile
    ├── boot_loader/
    │   ├── boot.c/.h           # Flash self-programming
    │   └── Makefile
    ├── Makefile
    └── README.md
```

## Notes

- All source files and Makefiles use portable paths (no absolute system paths).
- Makefiles employ `?=` for defaults (e.g., `COM_PORT ?= COM3`), allowing easy command-line overrides.
- Compiled binaries, object files and IDE workspace files are excluded from the repo (see `.gitignore`).
- Each project folder has its own `README.md` with hardware pinouts, detailed APIs, and build instructions.

## Future Work

- **Dockerize the build:** Add Docker/Docker Compose configurations to build and test on both Windows (via WSL2) and Linux without requiring local AVR toolchain installation.
- **CI/CD integration:** GitHub Actions workflow to automatically build all projects on push.
- **Top-level build script: Unified Makefile or script to build all three use-cases at once.
- **Re-test on multiple machines:** Validate the ported code on different Windows AVR setups to ensure robustness and platform stability.

## Contributing

Contributions, bug reports, and improvements are welcome. Please ensure:
- Code follows the existing C style (baremetal, minimal abstractions)
- Makefiles remain portable and Windows-friendly
- Documentation stays up-to-date with code changes

