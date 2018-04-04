#include "Bluetooth.h"

void BluetoothInitialize(void)
{
  UART_Initialize();
	BluetoothReInitialize();  
}

void BluetoothReInitialize(void)
{
  UART_TX_Buffer[0] = (uint8_t)'\0';
	UART_RX_Buffer[0] = (uint8_t)'\0';
  UART_TX_BufferCounter = UART_TX_Buffer;
  UART_RX_BufferCounter = UART_RX_Buffer;
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

static void UART_Initialize(void)
{
  // TODO: Переделать GPIO под LL
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // разрешаем тактирование на порт A
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;	// разрешаем тактирование UART
  
  /* настройка линии TX */
  GPIOA -> AFR[1] &= ~(0xFU << 4); // Очищаем AFSEL9
	GPIOA -> AFR[1] |= (0x7U << 4); //Записываем значение 7(табл. стр 55(datasheet) PA9 - USART1_TX - AF7) в поле альтернативных ф-ий AFRH
	GPIOA -> MODER &= ~(0x3U << 18); // Очищаем MODE9
	GPIOA -> MODER |= (0x2U << 18);	// Включаем режим Alternate function mode
	GPIOA -> OSPEEDR |= (0x3U << 18); // very high speed (мб поменьше надо)
	GPIOA -> OTYPER |= (1 << 9); // режим Open-drain
	GPIOA -> PUPDR &= ~(0x3 << 18); // очищаем значение PUPD6
	GPIOA -> PUPDR |= (1 << 18); // устанавливаем режим pull-up
  
  /* настройка линии RX */
  GPIOA -> AFR[1] &= ~(0xFU << 8); // Очищаем AFSEL10
	GPIOA -> AFR[1] |= (0x7U << 8); //Записываем значение 7(табл. стр 55(datasheet) PA10 - USART1_RX - AF7) в поле альтернативных ф-ий AFRH
	GPIOA -> MODER &= ~(0x3U << 20); // Очищаем MODE10
	GPIOA -> MODER |= (0x2U << 20);	// Включаем режим Alternate function mode
	GPIOA -> OSPEEDR |= (0x3U << 20); // very high speed (мб поменьше надо)
	GPIOA -> OTYPER |= (1 << 10); // режим Open-drain
	GPIOA -> PUPDR &= ~(0x3 << 20); // очищаем значение PUPD6
	GPIOA -> PUPDR |= (1 << 20); // устанавливаем режим pull-up
  
  LL_USART_Disable(UARTx);  // отключаем UART
  LL_USART_EnableDirectionRx(UARTx); // разрешаем прием
  LL_USART_EnableDirectionTx(UARTx);  // разрешаем передачу
  LL_USART_SetBaudRate(UARTx, 4000000U, 8, 9600);
  LL_USART_Enable(UARTx); // включаем UART 
}

uint8_t BTransmit(void)
{
  uint32_t tempSize = 0;
  do
  {
    if(tempSize > MAX_UART_TX_BUFFER_LEN)   // переполнение
    {
      BluetoothReInitialize();
      return 0;
    }
    if(LL_USART_IsActiveFlag_TXE(UARTx))  // если буффер данных пуст
    {
      LL_USART_TransmitData8(UARTx, *(UART_TX_BufferCounter++));  //отправляем байт
      tempSize++;      
    }
    
  } while(*UART_TX_BufferCounter != (uint8_t)'\0'); // пока есть, что передавать
  return 1;
}

static uint8_t BReceive(void)
{
  
}
