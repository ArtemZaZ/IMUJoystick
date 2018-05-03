#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_
#include "Command/CommandPacker.h"
#include "Command/CommandParser.h"
#include <string.h>
#include "LL/stm32l4xx_ll_usart.h"

#define MAX_UART_TX_BUFFER_LEN 255
#define MAX_UART_RX_BUFFER_LEN 255
#define END_RECEIVE_COUNTER 1000   // максимальное кол-во циклов, до ответа от передатчика

static volatile uint8_t UART_TX_Buffer[MAX_UART_TX_BUFFER_LEN];
static volatile uint8_t UART_RX_Buffer[MAX_UART_RX_BUFFER_LEN];

static volatile uint8_t* UART_TX_BufferCounter;	// каретка
static volatile uint8_t* UART_RX_BufferCounter;	// каретка

static volatile uint8_t bluetoothReceiveComplete = 0;  // флаг, означающий можно ли читать сообщения

#define UARTx USART1

void BluetoothInitialize(void);	// инициализация Bluetooth
void BluetoothReInitialize(void); // реинициализация Bluetooth
void sendMsg(SendData sd);	// добавляет сообщение в конец массива
void recvMsg(RecData* recDataBuf, uint8_t* size);	// возвращает массив пришедших комманд с данными
static void UART_Initialize(void);
void USART1_IRQHandler(void);
uint8_t BTransmit(void);
uint8_t isActiveFlag_bRC(void);
void setFlag_bRC(uint8_t b);
#endif /* BLUETOOTH_H_ */
