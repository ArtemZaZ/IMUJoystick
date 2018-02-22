/* файл конфигурации подключенных устройств*/
#ifndef DEVICES_H_
#define DEVICES_H_

/* конфигурация IMU*/
#if defined(MPU6050) || defined(GY85)
#define IMU

#ifdef MPU6050
#include "MPU6050RM.h"
#define IMUDOC "MPU6050 IMU sensor"

#elif defined(GY85)
#include "ADXL345RM.h"
#include "ITG3205RM.h"
#define IMUDOC "GY85 IMU sensor"
#endif 

#endif
/* * * * */


/* конфигурация мотора */
#ifdef MOTOR 
#include "motor.h"
#endif
/* * * * */



#endif /* DEVICES_H_ */