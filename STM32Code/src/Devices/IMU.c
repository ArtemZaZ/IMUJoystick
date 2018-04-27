#include "IMU.h"

void I2CInitDelay(void)
{
  for(uint32_t i = 0; i < I2C_INIT_DELAY_TIME; i++);
}

static void I2CInit(void)
{
	// TODO: Переделать GPIO под LL
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // разрешаем тактирование на порт B
  I2CInitDelay();
	//RCC -> APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // делаем ресет шины I2C1
	RCC -> APB1ENR1 |= RCC_APB1ENR1_I2C1EN;	// разрешаем тактирование I2C1
  I2CInitDelay();

	// насройка SCL линии 
	GPIOB -> AFR[0] &= ~(0xFU << 24); // Очищаем AFSEL6  
	GPIOB -> AFR[0] |= (0x4U << 24); //Записываем значение 4(табл. стр 55(datasheet) PB6 - I2C_SCL - AF4) в поле альтернативных ф-ий AFRL
	GPIOB -> MODER &= ~(0x3U << 12); // Очищаем MODE6
	GPIOB -> MODER |= (0x2U << 12);	// Включаем режим Alternate function mode
	GPIOB -> OSPEEDR |= (0x3U << 12); // very high speed (мб поменьше надо)
	GPIOB -> OTYPER |= (1 << 6); // режим Open-drain
	GPIOB -> PUPDR &= ~(0x3 << 12); // очищаем значение PUPD6
	GPIOB -> PUPDR |= (1 << 12); // устанавливаем режим pull-up
  I2CInitDelay();
	
	// насройка SDA линии 
	GPIOB -> AFR[0] &= ~(0xFU << 28); // Очищаем AFSEL7
	GPIOB -> AFR[0] |= (0x4U << 28); //Записываем значение 4(табл. стр 55(datasheet) PB7 - I2C_SDA - AF4) в поле альтернативных ф-ий AFRL
	GPIOB -> MODER &= ~(0x3U << 14); // Очищаем MODE7
	GPIOB -> MODER |= (0x2U << 14);	// Включаем режим Alternate function mode
	GPIOB -> OSPEEDR |= (0x3U << 14); // very high speed (мб поменьше надо)
	GPIOB -> OTYPER |= (1 << 7); // режим Open-drain
	GPIOB -> PUPDR &= ~(0x3 << 14); // очищаем значение PUPD7
	GPIOB -> PUPDR |= (1 << 14); // устанавливаем режим pull-up
  I2CInitDelay();
	
	LL_I2C_Disable(I2Cx);	// отключаем I2C
	LL_I2C_SetMode(I2Cx, LL_I2C_MODE_I2C);	// режим I2C - не SMBus
	LL_I2C_EnableAnalogFilter(I2Cx);	// включаем аналоговый фильтр
	LL_I2C_SetTiming(I2Cx, I2Cx_TIMING);	 // Устанавливаем всякие временные параметры
	LL_I2C_Enable(I2Cx);	// включаем I2C
}

static uint8_t TransmitWithoutStop(uint32_t slaveAddr, uint8_t* data, uint32_t size)  // ф-ия отправки данных без отправки стопа(для приема)
{
	LL_I2C_SetTransferSize(I2Cx, size);	// размер, который нужно отправить
	LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_WRITE);	// указываем, что будем отправлять
	LL_I2C_SetSlaveAddr(I2Cx, (slaveAddr << 1));	// указываем адресс ведомого
  LL_I2C_ClearFlag_NACK(I2Cx); // очищаем флаг Nack
	LL_I2C_ClearFlag_TXE(I2Cx); // очищаем регистр данных
	LL_I2C_GenerateStartCondition(I2Cx);	// Сигнал Старт
  I2CInitDelay();
	do 
	{
		if(LL_I2C_IsActiveFlag_NACK(I2Cx)) // если активен флаг Nack(отправка не удалась)
		{
			LL_I2C_GenerateStopCondition(I2Cx); // Генерим Стоп
      I2CInitDelay();
			LL_I2C_ClearFlag_NACK(I2Cx); // очищаем флаг Nack
			LL_I2C_ClearFlag_TXE(I2Cx); // очищаем регистр данных
			return 0; // возвращаем, что отправка не удалась
		}
		if(LL_I2C_IsActiveFlag_TXE(I2Cx)) // если активен флаг TXE(регистр данных пуст и готов для записи в него данных) 
		{
			LL_I2C_TransmitData8(I2Cx, *(data));	// записать байт данных
      data++;
			size--;
		}
	} while(size); // пока не закончатся данные

	return 1;	// отправка успешна
}

static uint8_t Transmit(uint32_t slaveAddr, uint8_t* data, uint32_t size)
{
  uint8_t ret = TransmitWithoutStop(slaveAddr, data, size);  	
	LL_I2C_GenerateStopCondition(I2Cx); // Генерим Стоп  
  I2CInitDelay();
  return ret;
}

uint8_t Receive(uint32_t slaveAddr, uint8_t regAddr, uint8_t* buf, uint32_t size)
{	
	while(!TransmitWithoutStop(slaveAddr, (uint8_t*)(&regAddr), 1)){;} // передаем адресс регистра
	LL_I2C_SetTransferSize(I2Cx, size); // размер, который нужно принять
	LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_READ); // указываем, что будем читать
	LL_I2C_SetSlaveAddr(I2Cx, (slaveAddr << 1)); // указываем адресс ведомого
  I2CInitDelay();
	LL_I2C_GenerateStartCondition(I2Cx);	// Сигнал Старт
  I2CInitDelay();
	do
	{
		if(LL_I2C_IsActiveFlag_NACK(I2Cx)) // если активен флаг Nack(отправка адреса не удалась, я хз, как это еще проверить - даташит adxl345 чтение множества байт)
		{
			LL_I2C_GenerateStopCondition(I2Cx); // Генерим Стоп
      I2CInitDelay();
			LL_I2C_ClearFlag_NACK(I2Cx); // очищаем флаг Nack
			return 0; // возвращаем, что отправка не удалась
		}
		if(LL_I2C_IsActiveFlag_RXNE(I2Cx))
		{
			*buf++ = LL_I2C_ReceiveData8(I2Cx); // принимаем байт
			size--;
		}		
	} while(size);
	LL_I2C_GenerateStopCondition(I2Cx);
  I2CInitDelay();
	return 1;
}

/*
uint8_t ReceiveWithStartAfterStop(uint32_t slaveAddr, uint8_t regAddr, uint8_t* buf, uint32_t size)
{	
	while(!Transmit(slaveAddr, (uint8_t*)(&regAddr), 1)){;} // передаем адресс регистра
	LL_I2C_SetTransferSize(I2Cx, size); // размер, который нужно принять
	LL_I2C_SetTransferRequest(I2Cx, LL_I2C_REQUEST_READ); // указываем, что будем читать
	LL_I2C_SetSlaveAddr(I2Cx, (slaveAddr << 1)); // указываем адресс ведомого
	LL_I2C_GenerateStartCondition(I2Cx);	// Сигнал Старт
  I2CInitDelay();
	do
	{
		if(LL_I2C_IsActiveFlag_NACK(I2Cx)) // если активен флаг Nack(отправка адреса не удалась, я хз, как это еще проверить - даташит adxl345 чтение множества байт)
		{
			LL_I2C_GenerateStopCondition(I2Cx); // Генерим Стоп
      I2CInitDelay();
			LL_I2C_ClearFlag_NACK(I2Cx); // очищаем флаг Nack
			return 0; // возвращаем, что отправка не удалась
		}
		if(LL_I2C_IsActiveFlag_RXNE(I2Cx))
		{
			*buf++ = LL_I2C_ReceiveData8(I2Cx); // принимаем байт
			size--;
		}		
	} while(size);
	LL_I2C_GenerateStopCondition(I2Cx);
	return 1;
}
*/

void IMUInitialize(void)
{
	I2CInit();
#ifdef MPU6050
	// TODO: когда придет плата  инициализация MPU6050
#elif defined(GY85)
	uint8_t tempBuf[2];
	
	// Инициализация акселерометра
	tempBuf[0] = ADXL345_POWER_CTL;
	tempBuf[1] = 0x00;
	while(!Transmit(ADXL345_ADDR, tempBuf, 3)); // Записываем в регистр ADXL345_POWER_CTL значение 8
  I2CInitDelay();
  tempBuf[0] = ADXL345_POWER_CTL;
	tempBuf[1] = 0x16;
	while(!Transmit(ADXL345_ADDR, tempBuf, 3)); // Записываем в регистр ADXL345_POWER_CTL значение 8
  I2CInitDelay();
  tempBuf[0] = ADXL345_POWER_CTL;
	tempBuf[1] = 0x08;
	while(!Transmit(ADXL345_ADDR, tempBuf, 3)); // Записываем в регистр ADXL345_POWER_CTL значение 8
  I2CInitDelay();
  
	tempBuf[0] = ADXL345_DATA_FORMAT;
	tempBuf[1] = 0x01;
	while(!Transmit(ADXL345_ADDR, tempBuf, 3)); // записывае в регистр ADXL345_DATA_FORMAT значение 1
	
	// Инициализация гироскопа
	tempBuf[0] = ITG3205_PWR_MGM;
	tempBuf[1] = 0x00;
	while(!Transmit(ITG3205_ADDR, tempBuf, 3)); // Записываем в регистр ITG3205_PWR_MGM значение 0
	tempBuf[0] = ITG3205_DLPF_FS;
	tempBuf[1] = 0x1E;
	while(!Transmit(ITG3205_ADDR, tempBuf, 3)); // Записываем в регистр ITG3205_DLPF_FS значение 0x1E
#endif	
}

void readIMUData(int32_t* data)
{	 
#ifdef MPU6050
  uint8_t rawData[14];
  // TODO: когда придет плата  чтение данных с MPU6050
  data[0] = (int32_t)(rawdata[0] << 8 | rawdata[1]);
  data[1] = (int32_t)(rawdata[2] << 8 | rawdata[3]);
  data[2] = (int32_t)(rawdata[4] << 8 | rawdata[5]);
  data[3] = (int32_t)(rawdata[8] << 8 | rawdata[9]);
  data[4] = (int32_t)(rawdata[10] << 8 | rawdata[11]);
  data[5] = (int32_t)(rawdata[12] << 8 | rawdata[13]);
#elif defined(GY85)
  uint8_t rawData[6];
  while(!Receive(ADXL345_ADDR, ADXL345_DATAX0, rawData, 6));
  data[0] = (int32_t)((rawData[1] << 8) | rawData[0]);
  data[1] = (int32_t)((rawData[3] << 8) | rawData[2]);
  data[2] = (int32_t)((rawData[5] << 8) | rawData[4]);  
  
  while(!Receive(ITG3205_ADDR, ITG3205_GYRO_XOUT_H, rawData, 6));
  data[3] = (int32_t)((rawData[0] << 8) | rawData[1]);
  data[4] = (int32_t)((rawData[2] << 8) | rawData[3]);
  data[5] = (int32_t)((rawData[4] << 8) | rawData[5]);
#endif
}
