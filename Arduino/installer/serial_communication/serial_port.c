/*
 * serial_port.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Ebenezer Olatuyi
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "serial_port.h"

#define READ_TIMEOUT      1000      // milliseconds
#define WRITE_TIMEOUT     1000

static void serial_port_read_handler(uint8_t *buffer, DWORD NoBytesRead)
{
    uint8_t i = 0;

    printf("Completed Receiving %lu bytes\n", NoBytesRead);
    printf("Received Data: ");
    for (i = 0; i < NoBytesRead; i++) {
        printf("%d,", *(buffer + i));
    }
    printf("\n******** END OF SERIAL PORT ********\n ");
    return;
}

static void serial_port_clean_up(HANDLE *hComm, OVERLAPPED *asyn_io_monitor)
{
    CloseHandle(*hComm);
    CloseHandle(asyn_io_monitor->hEvent);
    return;
}

/*
static uint8_t serial_port_asyn_op_monitor(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, DWORD wait_time_ms, DWORD *num_of_bytes_tx_rx)
{
    DWORD operation_termination_condition;
    DWORD start_time_ms;
    uint8_t execute_once = 1;

    if (GetLastError() != ERROR_IO_PENDING) {
        printf("Error in communications after issuing IO operation \n");
        return 1;
    } else {
        // operation_termination_condition = WaitForSingleObject(asyn_io_monitor->hEvent, wait_time_ms);

        start_time_ms = GetTickCount();
        while((GetTickCount() - start_time_ms) >= READ_TIMEOUT)
        {
        operation_termination_condition =
                GetOverlappedResult(*hComm, asyn_io_monitor, num_of_bytes_tx_rx, FALSE);

        if (!operation_termination_condition && GetLastError() == ERROR_IO_INCOMPLETE)
        {
            if (execute_once)
            {
                printf("GetOverlappedResult I/O Incomplete...\n");
                execute_once = 0;
            }

        }
        else
        {
            break;
            if (!CancelIo(*hComm)) {
                                printf("Error in canceling IO operation after Rx timeout\n");
                            }
        }
        }

        switch (operation_termination_condition) {
        case WAIT_OBJECT_0:
            if (!GetOverlappedResult(*hComm, asyn_io_monitor, num_of_bytes_tx_rx, FALSE)) {
                printf("Error in communications after performing an IO\n "
                        "operation and receiving a \"completed\" signal.\n");
                return 2;
            }

            break;
        case WAIT_TIMEOUT:
            if (!GetOverlappedResult(*hComm, asyn_io_monitor, num_of_bytes_tx_rx, FALSE) &&
                 GetLastError() == ERROR_IO_INCOMPLETE) {
                // This is a good time to do some background work but i choose to cancel IO operation.
                if (!CancelIo(*hComm)) {
                    printf("Error in canceling IO operation after Rx timeout\n");
                }
            }
            break;
        default:
            printf("Error in the WaitForSingleObject performing an IO operation:\n "
                   "Possibly problem with the OVERLAPPED structure's event handle \n");
            return 3;
            break;
        }

    }
    return 0;
}
*/

/*
 * Name       : main
 * Description: sends messages over a windows serial COM port
 */

int main(int argc, char *argv[])
{
    // open serial port and close upon error detection
    int baud_rate = 9600;
    uint8_t tx_buffer[SERIAL_PORT_TX_LENGTH];
    uint8_t rx_buffer[SERIAL_PORT_RX_LENGTH];
    DWORD tx_data_size;
    DWORD rx_data_size;
    DWORD i;
    HANDLE hComm;
    OVERLAPPED tx_asyn_io_monitor = {0};
    OVERLAPPED rx_asyn_io_monitor = {0};
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;

    printf("\nTransmit Buffer is  ");

    for(i = 1; i < argc && (i-1) < SERIAL_PORT_TX_LENGTH; i++ )
    {
        tx_buffer[i-1] = atoi(argv[i]);

        printf("%d ", tx_buffer[i-1]);
    }

    tx_data_size = argc - 1;
    rx_data_size = tx_data_size;

    printf("\nSerial port Parsing Complete\n");

    // Create the overlapped event. Must be closed before exiting
    // to avoid a handle leak.
    tx_asyn_io_monitor.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    rx_asyn_io_monitor.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (tx_asyn_io_monitor.hEvent == NULL || rx_asyn_io_monitor.hEvent == NULL)
    {
        // Error creating overlapped event; abort.
        printf("Error creating overlapped event; abort...\n");
        return 1;
    }

    /*
     * FIXME: COM port should be a variant
     */
    hComm = CreateFile("COM3",                      //port name ... From COM1 to 9
                      GENERIC_READ | GENERIC_WRITE, //Read/Write
                      0,                            // No Sharing
                      NULL,                         // No Security
                      OPEN_EXISTING,                // Open existing port only
                      FILE_FLAG_OVERLAPPED,
                      NULL);                        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("Error in opening serial port...\n");
        return 2;
    }
    else
    {
        printf("opening serial port successful...\n");
    }

    if(serial_port_init(&hComm, baud_rate, &dcbSerialParams, &timeouts)    ||
       serial_port_tx(&hComm, &tx_asyn_io_monitor, tx_buffer, tx_data_size)||
       serial_port_rx(&hComm, &rx_asyn_io_monitor, rx_buffer, rx_data_size))
    {
        serial_port_clean_up(&hComm, &tx_asyn_io_monitor);
        serial_port_clean_up(&hComm, &rx_asyn_io_monitor);

        return 3;
    }

    serial_port_clean_up(&hComm, &tx_asyn_io_monitor);
    serial_port_clean_up(&hComm, &rx_asyn_io_monitor);
    	return 0;
}

/*
 * Name       : serial_port_tx
 * Description: serial data generator: https://batchloaf.wordpress.com/2013/02/13/writing-bytes-to-a-serial-port-in-c/
 *              https://msdn.microsoft.com/en-us/library/ff802693.aspx
 *              http://xanthium.in/Serial-Port-Programming-using-Win32-API
 */
int serial_port_tx(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, const uint8_t *buffer, const DWORD tx_data_size)
{
    // Send specified data in buffer`
    DWORD num_of_bytes_tx_rx = 0;
    DWORD i;
    DWORD start_time;
    BOOL write_operation_completed;

    /*
     * FIXME: Throttling here is not compliant for a baud rate based port driver. Throttling
     * or buffering should be handled on MCU for baud rate compliance.
     */
    for (i = 1;i <= tx_data_size ; i = i + SERIAL_PORT_MAX_TX_BYTE_LENGTH)
    {
        start_time = GetTickCount();

        printf("Sending bytes...");

        write_operation_completed = WriteFile(*hComm,
                                             (uint8_t*)(buffer+i-1),
            MIN(SERIAL_PORT_MAX_TX_BYTE_LENGTH, tx_data_size-(i-1)),
                                                   NULL,
                                            asyn_io_monitor);

        if(!write_operation_completed && GetLastError() == ERROR_IO_PENDING)
        {
            // Asyn operation started

            // poll for completion of Async operation
            while (!GetOverlappedResult(*hComm, asyn_io_monitor, &num_of_bytes_tx_rx, FALSE) &&
                    GetLastError() == ERROR_IO_INCOMPLETE &&
                   (GetTickCount() - start_time) < (DWORD)WRITE_TIMEOUT)
            {
                // Async incomplete: Free to do background task
            }

            CancelIo(*hComm);
        }
        else if (write_operation_completed)
        {
            num_of_bytes_tx_rx = asyn_io_monitor->InternalHigh;
        }
        else
        {
            printf("Error...\n");
            printf("%lu bytes written\n", num_of_bytes_tx_rx);
            return 1;
        }


        printf("%lu bytes written\n", num_of_bytes_tx_rx);
        Sleep(SERIAL_PORT_WRITE_INTERVAL_MS);
    }

    return 0;
}


/*
 * Name       : serial_port_rx
 * Description: serial data monitor: https://msdn.microsoft.com/en-us/library/ff802693.aspx
 *              http://xanthium.in/Serial-Port-Programming-using-Win32-API
 *
 * Outarg     : serial_buffer - Buffer for storing Rxed Data
 *
 */
int serial_port_rx(HANDLE *hComm, OVERLAPPED *asyn_io_monitor, uint8_t *buffer, const DWORD rx_data_size)
{
    DWORD num_of_bytes_tx_rx = 0;
    DWORD total_num_of_bytes_tx_rx = 0;
    BOOL  read_operation_completed;
    DWORD start_time;
    BOOL async_op_status = FALSE;

    start_time = GetTickCount();

    printf("Receiving bytes...");

    memset(buffer,0xff,rx_data_size); // For debugging purpose

    while (total_num_of_bytes_tx_rx < rx_data_size) {

        SetLastError(0);

        read_operation_completed = ReadFile(*hComm,
                (buffer + asyn_io_monitor->Offset),
                                      rx_data_size,
                                              NULL,
                                   asyn_io_monitor);

        if (!read_operation_completed && GetLastError() == ERROR_IO_PENDING)
        {
            // Asyn operation started

            // poll for completion of Async operation
            while (!GetOverlappedResult(*hComm, asyn_io_monitor,&num_of_bytes_tx_rx, FALSE))
            {
                // Async incomplete: Free to do background task
                if (GetLastError() == ERROR_IO_INCOMPLETE)
                {
                    if (async_op_status == FALSE) {
                        printf("I/O pending: ERROR_IO_INCOMPLETE...\n");
                        async_op_status = TRUE;
                    }
                } else if (GetLastError() == ERROR_HANDLE_EOF) {
                    printf("I/O pending: ERROR_HANDLE_EOF.\n");
                    break;
                } else {
                    printf("GetOverlappedResult failed with error:%d\n", GetLastError());
                    break;
                }

            }

            if ((GetTickCount() - start_time) >= (DWORD)READ_TIMEOUT){
                CancelIo(*hComm);
                printf("Timeout detected...");
                break;
            }

        } else if (read_operation_completed) {
            // read completed immediately
        } else {
            printf("Error...\n");
            total_num_of_bytes_tx_rx = total_num_of_bytes_tx_rx + asyn_io_monitor->InternalHigh;
            printf("%lu bytes read\n", total_num_of_bytes_tx_rx);
            return 1;
        }

        total_num_of_bytes_tx_rx = total_num_of_bytes_tx_rx + asyn_io_monitor->InternalHigh;

        asyn_io_monitor->Offset = total_num_of_bytes_tx_rx;

    //    printf("\nReadFile operation completed for %lu bytes",total_num_of_bytes_tx_rx);
    }

    serial_port_read_handler(buffer, total_num_of_bytes_tx_rx);

    return 0;
}


/*
 * Name       : serial_port_init
 * Description: initialize serial port
 *
 */
int serial_port_init(HANDLE *hComm, int baud_rate, DCB *dcbSerialParams, COMMTIMEOUTS *timeouts)
{
    printf("Initializing serial port with baud rate %d...", baud_rate);

    memset(dcbSerialParams,0,sizeof(DCB));
    memset(timeouts,0,sizeof(COMMTIMEOUTS));

    dcbSerialParams->DCBlength = sizeof(dcbSerialParams);

    if (GetCommState(*hComm, dcbSerialParams) == 0)
    {
        printf("Error in getting serial port configuration \n");
        CloseHandle(*hComm);
        return 1;
    }
    dcbSerialParams->BaudRate = baud_rate;
    dcbSerialParams->ByteSize = 8;                     // Setting ByteSize = 8
    dcbSerialParams->StopBits = ONESTOPBIT;            // Setting StopBits = 1
    dcbSerialParams->Parity   = NOPARITY;              // Setting Parity = None
    if (SetCommState(*hComm, dcbSerialParams) == 0)
    {
        printf("Error in setting uart configuration \n");
        CloseHandle(*hComm);
        return 2;
    }


    timeouts->ReadIntervalTimeout         =  10;//SERIAL_PORT_BYTES_TIME(2, baud_rate);  // in milliseconds
    timeouts->ReadTotalTimeoutConstant    =  50;//SERIAL_PORT_BYTES_TIME(1, baud_rate);  // in milliseconds
    timeouts->ReadTotalTimeoutMultiplier  =  10;//SERIAL_PORT_BYTES_TIME(2, baud_rate);  // in milliseconds
    timeouts->WriteTotalTimeoutConstant   =  50;//SERIAL_PORT_BYTES_TIME(2, baud_rate);  // in milliseconds
    timeouts->WriteTotalTimeoutMultiplier =  10;//SERIAL_PORT_BYTES_TIME(1, baud_rate);  // in milliseconds
    if(SetCommTimeouts(*hComm, timeouts) == 0)
    {
        printf("Error in setting serial port timeout configuration \n");
        CloseHandle(*hComm);
        return 3;
    }

    printf("Initialization Complete\n");
    return 0;
}
