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

### Windows Setup for Arduino (ATmega328P) Cross Compilation

This project requires three main components for Windows development:

#### 1. AVR Toolchain

Install the AVR 8-bit toolchain from Microchip for compiling code targeting ATmega328P and other AVR chips.

- **Download**: [Microchip AVR 8-bit GCC Compiler](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)
- **Install**: Follow the Microchip installer; default installation path is `C:\AVR\avr8-gnu-toolchain`
- **Required Tools**: Includes avr-gcc, avr-libc, and binutils for AVR

#### 2. AVRDUDE (Programmer)

AVRDUDE is required to upload compiled firmware to your Arduino board.

- **Download**: [avrdude releases](https://github.com/mariusgreuel/avrdude/releases) (e.g., `avrdude-v7.1-windows-x64.zip`)
- **Extract**: Extract to a location like `C:\AVR\avrdude`
- **Purpose**: Programs the ATmega328P via USART bootloader or ICSP

#### 3. GNU Make

The AVR toolchain requires `make` to build projects. **MinGW is recommended** to avoid compatibility issues common with other Windows `make` implementations (Chocolatey, GnuWin32, etc.).

**MinGW Installation (Windows 10/11)**:
1. Download the [MinGW installer](https://sourceforge.net/projects/mingw/files/) (`mingw-get-setup.exe`)
2. Run the installer
3. In the **Basic Setup** tab, select for installation:
   - `mingw32-base`
   - `mingw32-gcc-g++`
   - `msys-base`
4. Click **Apply Changes** to install
5. Default installation path: `C:\MinGW`

#### Update System PATH

For me, I added the following directories to Windows PATH environment variable (in order):
- `C:\AVR\avrdude`
- `C:\AVR\avr8-gnu-toolchain\bin`
- `C:\MinGW\bin`
- `C:\MinGW\msys\1.0\bin`

**To update PATH on Windows**:
1. Open **Environment Variables** (press `Win+X`, select "System", then "Advanced system settings")
2. Click **Environment Variables**
3. Under **System variables**, select **Path** and click **Edit**
4. Click **New** and add each path above
5. Click **OK** and restart your terminal/IDE

**Verification**: Open a new Command Prompt or PowerShell and verify:
```bash
avr-gcc --version      # Should show AVR GCC version
avrdude -v             # Should show AVRDUDE version
make --version         # Should show GNU Make version
```

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

