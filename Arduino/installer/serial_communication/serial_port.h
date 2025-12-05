/*
 * serial_port.h
 *
 *  Created on: Mar 1, 2017
 *      Author: ebenezer
 */

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

int serial_port_init(HANDLE *hComm, int baud_rate, DCB *dcbSerialParams, COMMTIMEOUTS *timeouts);
int serial_port_tx(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, const uint8_t *buffer, const DWORD tx_data_size);
int serial_port_rx(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, uint8_t *buffer, const DWORD rx_data_size);

#define SERIAL_PORT_TX_LENGTH     64
#define SERIAL_PORT_RX_LENGTH     64

#define SERIAL_PORT_USART_BUFF_LENGTH     2
#define SERIAL_PORT_MAX_TX_BYTE_LENGTH SERIAL_PORT_USART_BUFF_LENGTH
#define SERIAL_PORT_WRITE_INTERVAL_MS  200

#define SERIAL_PORT_BYTES_TIME(NUM_OF_BYTES, BAUD_RATE) (NUM_OF_BYTES * 8 * 1000 / BAUD_RATE + 1)

#define MIN(A,B) (((A)<=(B))?(A):(B))

#endif /* SERIAL_PORT_H_ */
