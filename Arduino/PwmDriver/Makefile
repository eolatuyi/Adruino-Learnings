BUILD_OUTPUT_NAME  := pwm_driver_testbed

# Path to testbed directory containing shared source files.
# Default to ../installer (sibling directory). Override on command line:
# make TESTBED_DIR=../path/to/testbed
TESTBED_DIR ?= ../installer

BUILD_MODULE_NAMES :=                                    \
$(TESTBED_DIR)/usart.c                                   \
$(TESTBED_DIR)/main_testbed.c                            \
pwm_driver_testbed.c                                     \
pwm_driver.c                           

BUILD_HEADERS_NAME :=                                    \
$(TESTBED_DIR)/blink_led.h                               \
$(TESTBED_DIR)/usart.h                                   \
$(TESTBED_DIR)/main_testbed.h                            \
pwm_driver.h

BUILD_SOURCES := $($(BUILD_MODULE_NAMES) $(BUILD_HEADERS_NAME))

PRG = $(BUILD_OUTPUT_NAME)
OBJ =                                                    \
$(PRG).o                                                 \
usart.o                                                  \
main_testbed.o                                           \
pwm_driver.o

MCU_TARGET = atmega328p
OPTIMIZE = -O2 

DEFS =
LIBS =

#download variants
COM_PORT := com3
BAUD_RATE := 115200


# You should not have to change anything below here.

CC = avr-gcc
# Override is only needed by avr-lib build system.
override CFLAGS = -g -Wall $(OPTIMIZE) -DF_CPU=16000000L -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS = -Wl,-Map,$(PRG).map
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

avr: $(PRG)_upload.bat $(PRG).elf lst text eeprom

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	
$(PRG)_upload.bat: $(PRG).hex 
	echo "REM Uploaded from Target" > $(PRG)_upload.bat
	echo "avrdude -F -V -c arduino -p ATMEGA328P -P $(COM_PORT) -b$(BAUD_RATE) -U flash:r:$(PRG)_bak_app.hex:i" >> $(PRG)_upload.bat
	
	echo "REM download to target specifying -p <PART \#> -c <programmer-id> -F: skip device signature ping" >> $(PRG)_upload.bat
	echo "avrdude -F -V -c arduino -p ATMEGA328P -P $(COM_PORT) -b$(BAUD_RATE) -U flash:w:$(PRG).hex:i" >> $(PRG)_upload.bat
	
# dependency:
$(OBJ): $(BUILD_SOURCES)

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
	
lst: $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
	
# Rules for building the .text rom images

text: hex bin srec

hex: $(PRG).hex
bin: $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images
eeprom: ehex ebin esrec

ehex: $(PRG)_eeprom.hex
ebin: $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }
	
%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	|| { echo empty $@ not generated; exit 0; }

# Every thing below here is used by avr-libcâ€™s build system and can be ignored
# by the casual user.
FIG2DEV = fig2dev
EXTRA_CLEAN_FILES = *.hex *.bin *.srec *.bat

dox: eps png pdf

eps: $(PRG).eps
png: $(PRG).png
pdf: $(PRG).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@
	
%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@
	
%.png: %.fig
	$(FIG2DEV) -L png $< $@

.PHONY: clean
