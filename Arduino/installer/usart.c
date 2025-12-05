/*
 * usart.c
 *
 *  Created on: Feb 28, 2017
 *      Author: ebenezer
 *      Ref   : https://sites.google.com/site/qeewiki/books/avr-guide/usart
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "usart.h"
/*
 * Name       : usart_byte_tx
 * Description: Wait until port is ready to be written to transmit byte
 */
void usart_byte_tx(uint8_t const *byte_tx)
{
    // wait until the port is ready to be written to
    //_delay_ms(USART_BUFF_EMPTYING_MS_DELAY);

    while( !( UCSR0A & ( 1 << UDRE0 ) ) )
    {

    }

    // write the byte to the serial port
    UDR0 = *byte_tx;
}

/*
 * Name       : usart_byte_rx
 * Description: Wait until byte is received to return received byte
 */
uint8_t usart_byte_rx(uint8_t *byte_rx, uint8_t const rx_packet_size)
{
    uint16_t ms_counter = 0;


    // wait until a byte is ready to read
    while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 )
    {
        // If counter > 100 ms i.e. interval timeout is larger than byte rx period based on baud rate
        _delay_ms(USART_MS_TIMER_RES);
        if(ms_counter++ > USART_INTERVAL_TIMEOUT_COUNT){

            if(rx_packet_size > 0)
            {
                return USART_RET_PACKET_PARSED; // packet parsed
            }else{
                return USART_RET_RX_TIMEOUT; // rx_timeout
            }

        }
    }

    // grab the byte from the serial port
    *byte_rx = UDR0;

    return USART_RET_PACKET_PARSING; // packet parsing
}

/*
 * Name       : usart_init
 * Description: Init USART peripheral for serial TX, RX
 */
void usart_init()
{
    // Enable receiver and transmitter
     UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);

     // Frame Format: 8 data bits
     UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);

     // UBRR0H Baud rate register... upper 4 bits
     // UBRR0L Baud rate register... lower 8 bits
     UBRR0H  = (BAUD_PRESCALE >> 8);
     UBRR0L  = BAUD_PRESCALE;
}
