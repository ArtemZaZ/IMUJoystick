/* файл конфигурации подключенных устройств*/
#ifndef DEVICES_H_
#define DEVICES_H_

/* конфигурация IMU*/
#if defined(MPU6050) || defined(GY85)     
#define IMU       
#include "IMU.h"  
#endif
/* Конец конфигурации IMU */


/* конфигурация мотора */
#ifdef MOTOR 
#include "motor.h"
#endif
/* Конец конфигурации мотора */

#endif /* DEVICES_H_ */
