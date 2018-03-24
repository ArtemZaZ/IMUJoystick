/* файл конфигурации подключенных устройств*/
#ifndef DEVICES_H_
#define DEVICES_H_

/* конфигурация IMU*/
#if defined(MPU6050) || defined(GY85)    
	#if (defined(MPU6050) && defined(GY85))
		#error "Too many IMU selected"
	#endif	
	#define IMU       
	#include "IMU.h"  
#else
	#warning "No IMU selected"
#endif
/* Конец конфигурации IMU */


/* конфигурация мотора */
#ifdef MOTOR 
	#include "motor.h"
#else 
	#warning "No motor selected"
#endif
/* Конец конфигурации мотора */

/* конфигурация кнопок */
#if defined(FIRST_BUTTON) || defined(SECOND_BUTTON) || defined(THIRD_BUTTON)
	#include "Buttons.h"
#else
	#warning "No Buttons selected"
#endif
/* Конец конфигурации кнопок */

#endif /* DEVICES_H_ */
