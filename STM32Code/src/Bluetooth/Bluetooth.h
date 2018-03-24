#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include "Command/CommandPacker.h"
#include "Command/CommandParser.h"
#include <string.h>

#define MAX_UART_TX_BUFFER_LEN 255
#define MAX_UART_RX_BUFFER_LEN 255

static uint8_t UART_TX_Buffer[MAX_UART_TX_BUFFER_LEN];
static uint8_t UART_RX_Buffer[MAX_UART_RX_BUFFER_LEN];

static uint8_t* UART_TX_BufferCounter = UART_TX_Buffer;	// каретка
static uint8_t* UART_RX_BufferCounter = UART_RX_Buffer;	// каретка


void BluetoothInitialize(void);	// инициализация Bluetooth
void sendMsg(SendData sd);	// добавляет сообщение в конец массива
RecData recvMsg(void);	// берет данные из начала массива 

#endif /* BLUETOOTH_H_ */
