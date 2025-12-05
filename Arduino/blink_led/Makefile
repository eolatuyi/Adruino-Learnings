## gmake avr
## gmake clean

# AVR Libc Reference Manual Options for C compiler avr-gcc has options definition

# Configuration: adjust COM port and baud rate as needed for your environment
COM_PORT ?= COM3
BAUD_RATE ?= 115200
MCU ?= atmega328p
F_CPU ?= 16000000L

clean:
	rm -f blink_led.o blink_led.elf blink_led.hex blink_led.s19 blink_led_upload.bat

avr:
	# Compile and assemble but do not link
	avr-gcc -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c -o blink_led.o blink_led.c
	
	# Link to create elf file
	avr-gcc -mmcu=$(MCU) blink_led.o -oblink_led.elf
	
	# Convert elf program to ihex Application File format and remove .eeprom section from the output
	avr-objcopy -O ihex -R .eeprom blink_led.elf blink_led.hex
	avr-objcopy -O srec -R .eeprom blink_led.elf blink_led.s19
	
	echo "REM Uploaded from Target" > blink_led_upload.bat
	echo "avrdude -F -V -c arduino -p ATMEGA328P -P $(COM_PORT) -b$(BAUD_RATE) -U flash:r:blink_edit_bak_app.hex:i" >> blink_led_upload.bat
	
	echo "REM download to target specifying -p <PART \#> -c <programmer-id> -F: skip device signature ping" >> blink_led_upload.bat
	echo "avrdude -F -V -c arduino -p ATMEGA328P -P $(COM_PORT) -b$(BAUD_RATE) -U flash:w:blink_led.hex:i" >> blink_led_upload.bat

.PHONY: clean
