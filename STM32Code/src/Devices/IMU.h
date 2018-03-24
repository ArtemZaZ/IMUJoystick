#ifndef IMU_H_
#define IMU_H_
#include <stdint.h>
#include "stm32l4xx.h"
#include "LL/stm32l4xx_ll_i2c.h"
#include "LL/stm32l4xx_ll_gpio.h"

#define I2Cx	I2C1	//  будем работать с I2C1
#define I2Cx_TIMING	0x10420F13U	// см. предыдущие коммиты, где я это все ручками настравал

#ifdef MPU6050
	#include "RegisterMaps/MPU6050RM.h"
	#define MPU6050_A_SENSETIVE		16384.f // Пока так, потом сделать проверку из регистра
	#define MPU6050_G_SENSETIVE		131.f
	#define IMUINFO "MPU6050 IMU sensor" 
#elif defined(GY85)
	#include "RegisterMaps/ADXL345RM.h"
	#include "RegisterMaps/ITG3205RM.h"
	#define GY85_A_SENSETIVE		128.f   // Пока так, потом сделать проверку из регистра
	#define GY85_G_SENSETIVE		14.375f
	#define IMUINFO "GY85 IMU sensor"
#endif  

static float goffx = 0.f; // TODO: пока статичная, сделать калибровку
static float goffy = 0.f;
static float goffz = 0.f;

void IMUInitialize(void);
void readIMUData(int32_t* buf);

static void I2CInit(void);
static uint8_t Transmit(uint32_t slaveAddr, uint8_t* data, uint32_t size);
static uint8_t Receive(uint32_t slaveAddr, uint8_t regAddr, uint8_t* buf, uint32_t size);


#endif /* IMU_H_ */
