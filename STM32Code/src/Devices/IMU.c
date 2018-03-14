#include "IMU.h"
#include "stm32l4xx.h"

/*
void I2CInit(void)	// настройка по диаграмме на странице 1022 и таблице на странице 1046. Настройка I2C1
{
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // разрешаем тактирование на порт B
	//RCC -> APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // делаем ресет шины I2C1
	RCC -> APB1ENR1 |= RCC_APB1ENR1_I2C1EN;	// разрешаем тактирование I2C1

	// насройка SCL линии 
	GPIOB -> AFR[1] &= ~(0xFU << 24); // Очищаем AFSEL6
	GPIOB -> AFR[1] |= (0x4U << 24); //Записываем значение 4(табл. стр 55(datasheet) PB6 - I2C_SCL - AF4) в поле альтернативных ф-ий AFRL
	GPIOB -> MODER &= ~(0x3U << 12); // Очищаем MODE6
	GPIOB -> MODER |= (0x2U << 12);	// Включаем режим Alternate function mode
	GPIOB -> OSPEEDR |= (0x3U << 12); // very high speed (мб поменьше надо)
	GPIOB -> OTYPER |= (1 << 6); // режим Open-drain
	GPIOB -> PUPDR &= ~(0x3 << 12); // очищаем значение PUPD6
	GPIOB -> PUPDR |= (1 << 12); // устанавливаем режим pull-up
	
	// насройка SDA линии 
	GPIOB -> AFR[1] &= ~(0xFU << 28); // Очищаем AFSEL7
	GPIOB -> AFR[1] |= (0x4U << 28); //Записываем значение 4(табл. стр 55(datasheet) PB7 - I2C_SDA - AF4) в поле альтернативных ф-ий AFRL
	GPIOB -> MODER &= ~(0x3U << 14); // Очищаем MODE7
	GPIOB -> MODER |= (0x2U << 14);	// Включаем режим Alternate function mode
	GPIOB -> OSPEEDR |= (0x3U << 14); // very high speed (мб поменьше надо)
	GPIOB -> OTYPER |= (1 << 7); // режим Open-drain
	GPIOB -> PUPDR &= ~(0x3 << 14); // очищаем значение PUPD7
	GPIOB -> PUPDR |= (1 << 14); // устанавливаем режим pull-up
	
	// Настройка прерываний 
	NVIC_SetPriority(I2C1_ER_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 1)); // устанавливаем для прерывания I2C1_ER_IRQn группу 0, подгруппу 1(как я понимаю устанавливаем через упаковщик, чтоб не произошло конфликтов между группами)
	NVIC_EnableIRQ(I2C1_ER_IRQn); // разрешаем прерывание 
	NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 2)); // устанавливаем для прерывания I2C1_EV_IRQn группу 0, подгруппу 2(как я понимаю устанавливаем через упаковщик, чтоб не произошло конфликтов между группами)
	NVIC_EnableIRQ(I2C1_EV_IRQn); // разрешаем прерывание

  // Настройка I2C 
	I2C1 -> CR1 &= ~I2C_CR1_PE; // выключаем линии SDA и SCL
	I2C1 -> CR2 |= I2C_CR2_AUTOEND; // включаем генерацию сигнала стоп, когда все данные переданы
	I2C1 -> CR1 |= I2C_CR1_NOSTRETCH;	// выключаем растяжение часов(не знаю зачем, но так написано в даташите)
	
	I2C1 -> CR1 &= ~I2C_CR1_ANFOFF;	// включаем аналоговый фильтр ... пока так
	I2C1 -> CR1 &= ~(0xFU << 8);	// включаем дискретный фильтр ... пока так
	I2C1 -> CR1 |= (1 << 8);	// на время 1*Ti2cclc ... пока так
	I2C1 -> TIMINGR &= ~I2C_TIMINGR_PRESC; // очищаем prescaler
	I2C1 -> TIMINGR |= (1 << 28); // устанавливаем prescaler значение 1 (100кГ) при 8 мГц  тактирования
	I2C1 -> TIMINGR |= 0x13U; // устанавливаем SCLL
	I2C1 -> TIMINGR |= (0xFU << 8); // устанавливаем SCLH
	I2C1 -> TIMINGR &= ~I2C_TIMINGR_SCLDEL; // обнуляем
	I2C1 -> TIMINGR |= (0x4U << 20); // устанавливаем значение
	I2C1 -> TIMINGR &= ~I2C_TIMINGR_SDADEL; // обнуляем
	I2C1 -> TIMINGR |= (0x2U << 16);		// задержка в 2 такта i2c
	
	I2C1 -> CR1 |= I2C_CR1_PE; // включаем I2C
}

// запись до 255 байт 
void Transmit(uint32_t slaveAddr, uint8_t* data, uint32_t size) // построено по диаграмме на странице 1039
{
	TX_counter = data;	// ставим указаель на начало передаваемых данных
	TX_size = size;	// указываем размер данных
	MODIFY_REG(I2C1 -> CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_STOP), \
	(slaveAddr << 1) | (size << 16) | I2C_CR2_START);
	//
	I2C1 -> CR2 &= ~I2C_CR2_NBYTES;	// обнуляем количество байт, которые нужно отправить
	I2C1 -> CR2 |= (size << 16); // записываем их в регистр
	I2C1 -> CR2 &= ~I2C_CR2_SADD;	// обнуляем адрес slave
	I2C1 -> CR2 |= (slaveAddr << 1); // записываем адрес slave в 1-7 биты
	I2C1 -> CR2 |= I2C_CR2_START;	// формируем сигнал СТАРТ
	
	
	// разрешаем следующие прерывания 
	I2C1 -> CR1 |= I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_TXIE;


	
	do
	{
		uint32_t temp = 0;
		if((I2C1 -> ISR) & I2C_ISR_NACKF) break;
		while(!((I2C1 -> ISR) & I2C_ISR_NACKF))	
		{
			if((I2C1 -> ISR) & I2C_ISR_TXIS)
			{
				I2C1 -> TXDR |= data[temp++];
				if(temp == size)	break;
			}
		}
	} while(((I2C1 -> ISR) & I2C_ISR_TC));
	I2C1 -> CR2 |= I2C_CR2_STOP;	// формируем сигнал СТОП
	
}

void Receive(uint32_t slaveAddr, uint8_t* buf, uint32_t size) // прием данных
{
	RX_counter = buf;
	RX_size = size;
	MODIFY_REG(I2C1 -> CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_STOP), \
	(slaveAddr << 1) | I2C_CR2_RD_WRN  | (size << 16) | I2C_CR2_START);
	I2C1 -> CR1 |= I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_RXIE | I2C_CR1_TXIE;
}


void I2C1_EV_IRQHandler(void)
{
	if((I2C1 -> ISR) & I2C_ISR_NACKF) // если событие NACKF
	{
		I2C1 -> ICR |= I2C_ICR_NACKCF; //очищаем флаг NACKF в регистре ISR
		if((I2C1 -> ISR) & I2C_ISR_TXIS) I2C1 -> TXDR &= ~I2C_TXDR_TXDATA;	// очищаем TXDATA в регистре TXDR
		if(!((I2C1 -> ISR) & I2C_ISR_TXE)) I2C1 -> ISR |= I2C_ISR_TXE; // Из даташита: This bit can be written to ‘1’ by software in order to flush the transmit data register I2C_TXDR(возможно, тогда не нужно делать предыдущую операцию).
	}
	else if((I2C1 -> ISR) & I2C_ISR_TXIS)	// если событие TXI(готовность записи)
	{
		I2C1 -> TXDR |= *(TX_counter++);	// записываем первый байт из буффера передачи в регистр отправки данных, сдвигаем указатель на 1 байт
		TX_size--;	// уменьшаемм размер буффера
	}
	else if((I2C1 -> ISR) & I2C_ISR_TC) // если данные отправлены
	{
		I2C1 -> CR2 |= I2C_CR2_STOP;	// формируем сигнал СТОП
		//TX_counter = TX_buf;	// возвращаем указатель на начальный элемент массива
	}	
	else if((I2C1 -> ISR) & I2C_ISR_RXNE)	// если регистр данных готов к чтению
	{
		*RX_counter = I2C1 -> RXDR;
		RX_counter++;
		RX_size++;
	}
}
*/

void I2CInit(void)
{
	
}

void IMUInitialize(void)
{
#ifdef MPU6050
	// TODO: когда придет плата  инициализация MPU6050
#elif defined(GY85)
	// TODO: когда придет плата  инициализация GY85
#endif	
}

void readIMUData(int32_t* data)
{	 
#ifdef MPU6050
	uint8_t rawData[14];
	// TODO: когда придет плата  чтение данных с MPU6050
	data[0] = (int32_t)(rawdata[0] << 8 | rawdata[1])/MPU6050_A_SENSETIVE;
  data[1] = (int32_t)(rawdata[2] << 8 | rawdata[3])/MPU6050_A_SENSETIVE;
  data[2] = (int32_t)(rawdata[4] << 8 | rawdata[5])/MPU6050_A_SENSETIVE;
  data[3] = (int32_t)(rawdata[8] << 8 | rawdata[9])/MPU6050_G_SENSETIVE + goffx;
  data[4] = (int32_t)(rawdata[10] << 8 | rawdata[11])/MPU6050_G_SENSETIVE + goffy;
  data[5] = (int32_t)(rawdata[12] << 8 | rawdata[13])/MPU6050_G_SENSETIVE + goffz;
#elif defined(GY85)
	uint8_t rawData[6];
	// TODO: когда придет плата  чтение данных c ADXL345
	data[0] = (int32_t)((buff[1] << 8) | buff[0])/GY85_A_SENSETIVE;
  data[1] = (int32_t)((buff[3] << 8) | buff[2])/GY85_A_SENSETIVE;
  data[2] = (int32_t)((buff[5] << 8) | buff[4])/GY85_A_SENSETIVE;  
	// TODO: когда придет плата  чтение данных c ITG3205
	data[0] = (int32_t)((buff[0] << 8) | buff[1])/GY85_G_SENSETIVE + goffx;
  data[1] = (int32_t)((buff[2] << 8) | buff[3])/GY85_G_SENSETIVE + goffy;
  data[2] = (int32_t)((buff[4] << 8) | buff[5])/GY85_G_SENSETIVE + goffz;
#endif

}
