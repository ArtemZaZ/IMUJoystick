/* файл с ф-иями для мотора*/
#ifndef MOTOR_H_
#define MOTOR_H_
#include "LL/stm32l4xx_ll_gpio.h"

#define PORT_MOTOR 0xA
#define PIN_MOTOR 1
#define VIBRO_TIME 3.f // 3 секунды

#if 	(PORT_MOTOR == 0xA)
	#define GPIO_PORT_MOTOR		GPIOA
#elif (PORT_MOTOR == 0xB)
	#define GPIO_PORT_MOTOR		GPIOB
#elif	(PORT_MOTOR == 0xC)
	#define	GPIO_PORT_MOTOR		GPIOC
#else 
	#error "Motor port not selected"
#endif

#ifndef PIN_MOTOR
	#error "Motor pin not selected"
#endif

static volatile uint8_t isVibrate = 0x0; // флаг обозначающий должен ли мотор крутиться или нет
static volatile float time = 0.f; // время прошедшее с начала вибрации

void motorInitialize(void);   // Ф-ия инициализации мотора
void vibrate(void);	// установка флага вибрации
void update(float dt);	// обновляет состояние мотора // dt в секундах
#endif /* MOTOR_H_ */
