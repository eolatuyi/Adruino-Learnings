/*
 * usart.h
 *
 *  Created on: Feb 28, 2017
 *      Author: ebenezer
 */

#include <stdint.h>

#ifndef USART_H_
#define USART_H_

enum usart_state_t
{
    RECEIVING,
    PROCESSING,
    TRANSMITTING
};

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define USART_TX_RX_BIT_LENGTH       (9)  // 8-N-1
#define USART_TX_RX_BUFF_SIZE        (2)
#define USART_BUFF_EMPTYING_MS_DELAY (USART_TX_RX_BIT_LENGTH * USART_TX_RX_BUFF_SIZE * 1000 / USART_BAUDRATE)

#define USART_MS_TIMER_RES             (0.1)
//#define USART_INTERVAL_MS_TIMEOUT      (700)
#define USART_INTERVAL_TIMEOUT_COUNT   (7000) //(USART_INTERVAL_MS_TIMEOUT/USART_MS_TIMER_RES)

#define USART_RET_PACKET_PARSED  1
#define USART_RET_RX_TIMEOUT     2
#define USART_RET_PACKET_PARSING 0

void usart_init(void);
void usart_byte_tx(uint8_t const *byte_tx);
uint8_t usart_byte_rx(uint8_t *byte_rx, uint8_t const rx_packet_size);

#endif /* USART_H_ */
