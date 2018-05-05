#ifndef IMU_H_
#define IMU_H_
#include <stdint.h>
#include "stm32l4xx.h"
#include "LL/stm32l4xx_ll_i2c.h"
#include "LL/stm32l4xx_ll_gpio.h"

#define I2Cx	I2C1	//  будем работать с I2C1
#define I2Cx_TIMING	0x00420F13U	// см. предыдущие коммиты, где я это все ручками настравал
//#define I2Cx_TIMING	0x0042C3C7U
#define I2C_INIT_DELAY_TIME 100 // в попугаях

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

static float goffx = 7.4101f; // TODO: пока статичная, сделать калибровку
static float goffy = 1.74f;
static float goffz = 0.21f;

void IMUInitialize(void);   
void readIMUData(int16_t* buf);

static void I2CInit(void);
static uint8_t Transmit(uint32_t slaveAddr, uint8_t* data, uint32_t size);
static uint8_t TransmitWithoutStop(uint32_t slaveAddr, uint8_t* data, uint32_t size);
uint8_t Receive(uint32_t slaveAddr, uint8_t regAddr, uint8_t* buf, uint32_t size);
//uint8_t ReceiveWithStartAfterStop(uint32_t slaveAddr, uint8_t regAddr, uint8_t* buf, uint32_t size);
void I2CInitDelay(void); // ф-ия задержки инициализации переферии I2C


#endif /* IMU_H_ */
