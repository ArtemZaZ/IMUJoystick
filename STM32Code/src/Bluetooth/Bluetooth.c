#include "Bluetooth.h"

void BluetoothInitialize(void)
{
	UART_TX_Buffer[0] = (uint8_t)'\0';
	UART_RX_Buffer[0] = (uint8_t)'\0';
}

void sendMsg(SendData sd)
{
	strncat((char*)UART_TX_Buffer, (char*)(sd.message), sd.size);	// добавляем сообщение в конец строки
}

void recvMsg(RecData* recMsgBuf, uint8_t* recMsgBufSize)
{
	*recMsgBufSize = 0;
	uint8_t* token;	// временный указатель на строку для парсинга
	uint8_t stateFlag = 0; // флаг вхождения в сообщение
	while(UART_RX_BufferCounter != (uint8_t)'\0')	// пока данные не закончились
	{
		if(*UART_RX_BufferCounter == (uint8_t)'<')
		{
			token = UART_RX_Buffer;	// ставим указатель на начало комманды
			stateFlag = 1;
		}
		if((*UART_RX_BufferCounter == (uint8_t)'>') && stateFlag)	// если был пойман символ конца комманды и при этом было вхождение в нее
		{
			recMsgBuf[*recMsgBufSize] = parsing((char*)token, (uint8_t)(UART_RX_BufferCounter - token));	// парсим комманду и записываем ее в выходной массив
			(*recMsgBufSize)++;
			stateFlag = 0;			
		}
		UART_RX_BufferCounter++;
	}
	UART_RX_Buffer[0] = (uint8_t)'\0';	// очищаем буффер
	UART_RX_BufferCounter = UART_RX_Buffer;	// ставим указатель в начало массива
}
