/*
 * serial_com_testbed.c
 *
 *  Created on: Feb 28, 2017
 *      Author: ebenezer
 */
#include<string.h>
#include <avr/io.h>
#include <stdint.h>
#include "blink_led.h"
#include "shift_register.h"
#include "usart.h"

#define BUFF_LENGTH (64)
#define LED_ON_PERIOD (1000)
#define ACTIVE_HIGH 1
#define ACTIVE_LOW  0

// 2 - srclr - PD2 - active_low
// 4 - srclk - PD4
// 7 - rclk  - PD7
// 8 - oe    - PB0 - active_low
// 12- ser   - PB4


#define DEFINE_SET_DIGITAL_OUTPUT(port, port_number, active_high)      \
void set_##port##port_number(shift_register_output_state_t state)      \
{                                                                      \
    if (state == ON && (active_high))                                  \
    {                                                                  \
        PORT##port |= _BV(PORT##port##port_number);                    \
    }                                                                  \
    else if (state == OFF && (active_high))                            \
    {                                                                  \
        PORT##port &= ~_BV(PORT##port##port_number);                   \
    }                                                                  \
    else if (state == ON && !(active_high))                            \
    {                                                                  \
        PORT##port &= ~_BV(PORT##port##port_number);                   \
    }                                                                  \
    else if (state == OFF && !(active_high))                           \
    {                                                                  \
        PORT##port |= _BV(PORT##port##port_number);                    \
    }                                                                  \
}

#define SET_DIGITAL_OUTPUT(port, port_number)                      \
set_##port##port_number

#define CONFIGURE_PIN_AS_DIGITAL_OUTPUT(port, port_number)         \
DDR##port  |= _BV(DD##port##port_number)

int main(void);
void main_init(void);
void process_usart_input(uint8_t* input_buffer , uint8_t num_of_input);
void init_device_under_test();


DEFINE_SET_DIGITAL_OUTPUT(D, 2, ACTIVE_LOW)
DEFINE_SET_DIGITAL_OUTPUT(D, 4, ACTIVE_HIGH)
DEFINE_SET_DIGITAL_OUTPUT(D, 7, ACTIVE_HIGH)
DEFINE_SET_DIGITAL_OUTPUT(B, 0, ACTIVE_LOW)
DEFINE_SET_DIGITAL_OUTPUT(B, 4, ACTIVE_HIGH)

shift_register_cblk_t shift_register_cblk = {.data = 0,.latched_output = 0, .latch_enable = 0, .clear_data = 0,
        .digital_input =
        {SET_DIGITAL_OUTPUT(B,4),SET_DIGITAL_OUTPUT(D,2),SET_DIGITAL_OUTPUT(B,0),SET_DIGITAL_OUTPUT(D,4),SET_DIGITAL_OUTPUT(D,7)}};


/*
 * Name       : main
 * Description: You may turn on shift register pins by running serial_port <valid pin numbers> an invalid pin number
 *              will turn off the pin
 */
int main(void)
{

    uint8_t received_byte,
            transmit_byte,
                    i = 0,
            packet_status = 0;
    enum usart_state_t usart_state = RECEIVING;
    uint8_t tx_rx_buff[BUFF_LENGTH];
    uint8_t tx_packet_size = 0;
    uint8_t rx_packet_size = 0;

    main_init();

    blink_led(LED_ON_PERIOD);

    while(1)
    {
        blink_led(LED_ON_PERIOD);
        switch (usart_state)
        {
            case RECEIVING:
                rx_packet_size = 0;
                packet_status = USART_RET_PACKET_PARSING;

                // Disable transmitter
                 UCSR0B |= (0<<TXEN0);

                // receive byte and detect packet end
                while(packet_status == USART_RET_PACKET_PARSING)
                {
                    packet_status = usart_byte_rx((uint8_t*)(&received_byte), rx_packet_size);
                    if (packet_status == USART_RET_PACKET_PARSING)
                    {
                        tx_rx_buff[rx_packet_size++] = received_byte;
                    }

                }

                if(packet_status == USART_RET_PACKET_PARSED)
                {
                    usart_state = PROCESSING;

                    // Enable transmitter
                    UCSR0B |= (1<<TXEN0);

                }else if (packet_status == USART_RET_RX_TIMEOUT) {
                    // Stay in Receive State
                }


                break;
            case PROCESSING:
                // process packet... process is echo
                tx_packet_size = rx_packet_size;

                usart_state = TRANSMITTING;

                process_usart_input(tx_rx_buff,tx_packet_size);
                break;
            case TRANSMITTING:
                // send response packey
                for (i = 0; i < tx_packet_size; i++){
                    transmit_byte = tx_rx_buff[i];
                    usart_byte_tx((uint8_t*)(&transmit_byte));
                }

                tx_packet_size = 0;

                blink_led(LED_ON_PERIOD);

                usart_state = RECEIVING;
                break;

        }


    }


    return 0;
}

void main_init()
{

    init_device_under_test();

    blink_led_init();

    usart_init();

    return;
}

void init_device_under_test()
{
    // Init pin direction
    CONFIGURE_PIN_AS_DIGITAL_OUTPUT(D, 2);    // Output port direction
    CONFIGURE_PIN_AS_DIGITAL_OUTPUT(D, 4);    // Output port direction
    CONFIGURE_PIN_AS_DIGITAL_OUTPUT(D, 7);    // Output port direction
    CONFIGURE_PIN_AS_DIGITAL_OUTPUT(B, 0);    // Output port direction
    CONFIGURE_PIN_AS_DIGITAL_OUTPUT(B, 4);    // Output port direction

    shift_register_init(&shift_register_cblk);
}

void process_usart_input(uint8_t* input_buffer , uint8_t num_of_input)
{
    int8_t idx;

    for (idx = 0; idx < num_of_input; idx++ )
    {
        switch (input_buffer[idx])
        {
            case '0':
                shift_register_set_output(&shift_register_cblk, 0, ON);
                break;
            case '1':
                shift_register_set_output(&shift_register_cblk, 1, ON);
                break;
            case '2':
                shift_register_set_output(&shift_register_cblk, 2, ON);
                break;
            case '3':
                shift_register_set_output(&shift_register_cblk, 3, ON);
                break;
            case '4':
                shift_register_set_output(&shift_register_cblk, 4, ON);
                break;
            case '5':
                shift_register_set_output(&shift_register_cblk, 5, ON);
                break;
            case '6':
                shift_register_set_output(&shift_register_cblk, 6, ON);
                break;
            case '7':
                shift_register_set_output(&shift_register_cblk, 7, ON);
                break;

            default:
                init_device_under_test();
                idx = num_of_input;
                break;
        }
    }
    return;
}
