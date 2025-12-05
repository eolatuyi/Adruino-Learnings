/*
 * boot.c
 *
 *  Created on: Feb 23, 2017
 *      Author: ebenezer
 */


#include <inttypes.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "boot.h"
#include "usart.h"

void boot_program_page (uint32_t page, uint8_t *buf);
serial_download (uint32_t page, uint8_t *program_data);
void main(void);

/*
 * Name       : main
 * Description: main loop
 */
unit8_t main(void)
{
    uint8_t program_data[SPM_PAGESIZE];
    uint16_t idx;
    uint32_t page_addr = 0; // Fix Me

    usart_init ();

    while(1)
    {

        for(idx = 0; idx < SPM_PAGESIZE; idx++)
        {
            usart_byte_rx((uint8_t*)(&program_data[0] + idx));

        }


        serial_download (page_addr,(uint8_t*)(&program_data[0]));

    }

    return 0;
}


/*
 * Name       : serial_download
 * Description: Downloads UART packets to program flash
 */
void serial_download (uint32_t page, uint8_t *program_data)
{


    /*
     *  SERIAL PROGRAMMING
    // Power-up sequence
    // Set RESET and SCK to "0" and apply power between VCC and GND
    // Pulse RESET at least 2 CPU Clck Cycles duration after SCK has been set low
    // to ensure SCK is low during power up

    // Delay 20 ms and Send Enable Serial Programming Command

    // If 2nd byte of command doesn't echo back when issuing the 3rd byte
    // then give RESET a positive pulse and issue a new Enable Serial Programming Command
    // This is to ensure synch
     *
     */

    // Get Serial data to program flash on the go
    // Disable interrupt while programming flash: would serial communication still be functional?
    // Ref of Serial Communication: http://jamesgregson.blogspot.com/2012/07/sample-code-for-atmega328p-serial.html

    // The flash is programmed one page at a time
    // if polling (RDY/BSY) is not used, wait > tWD_FLASH before issuing
    // the next page... accessing the serial programming interface before
    // the Flash write completes can result in incorrect programming

    boot_program_page (page, program_data);

    // RESET can be set high to commence normal op

    // Power-off sequence (as needed)
    // Set RESET to "1" and Turn VCC Power off


}

/*
 * Name       : boot_program_page
 * Description: Programs a page based on provided data
 * Inarg      : page - base address of specific page to program
 * Inarg      : buf  - data for page  (== SPM_PAGESIZE) with collection of little endian 2 byte words
 */
void boot_program_page (uint32_t page, uint8_t *buf)
{
    uint16_t i;
    uint8_t sreg;

    // Disable interrupts.
    sreg = SREG;
    cli();

    eeprom_busy_wait ();
    boot_spm_busy_wait ();

    // Erase the flash page... page is a byte address in flash, not a word address
    // Wait until the memory is erased (spm instruction is busy doing erasure).
    boot_page_erase (page);
    boot_spm_busy_wait ();

    // Fill a page buffer one word at a time
    for (i=0; i<SPM_PAGESIZE; i+=2) // SPM_PAGESIZE is in byte and a word is 2 bytes
    {
        // Set up little-endian word... a word is 2 bytes
        uint16_t w = *buf++;
        w += (*buf++) << 8;

        // Fill the bootloader temporary page buffer for flash address with data word.
        boot_page_fill (page + i, w);
    }

    // Write the bootloader temporary page buffer to flash page.
    // Wait until the memory is written (spm instruction is busy doing writing).
    boot_page_write (page);
    boot_spm_busy_wait();

    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
    boot_rww_enable ();

    // Re-enable interrupts (if they were ever enabled).
    SREG = sreg;
}
