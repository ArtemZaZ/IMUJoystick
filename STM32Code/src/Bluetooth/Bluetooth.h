#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include "Command/CommandPacker.h"
#include "Command/CommandParser.h"
#include <string.h>
#include "LL/stm32l4xx_ll_usart.h"

#define MAX_UART_TX_BUFFER_LEN 255
#define MAX_UART_RX_BUFFER_LEN 255

static uint8_t UART_TX_Buffer[MAX_UART_TX_BUFFER_LEN];
static uint8_t UART_RX_Buffer[MAX_UART_RX_BUFFER_LEN];

static uint8_t* UART_TX_BufferCounter;	// каретка
static uint8_t* UART_RX_BufferCounter;	// каретка

#define UARTx USART1

void BluetoothInitialize(void);	// инициализация Bluetooth
void BluetoothReInitialize(void); // реинициализация Bluetooth
void sendMsg(SendData sd);	// добавляет сообщение в конец массива
void recvMsg(RecData* recDataBuf, uint8_t* size);	// возвращает массив пришедших комманд с данными
static void UART_Initialize(void);
uint8_t BTransmit(void);
uint8_t BReceive(void);
#endif /* BLUETOOTH_H_ */
