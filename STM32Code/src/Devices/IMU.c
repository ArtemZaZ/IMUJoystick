#include "IMU.h"

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
