/* файл с ф-иями для мотора*/
#ifndef MOTOR_H_
#define MOTOR_H_
#include "LL/stm32l4xx_ll_gpio.h"

void motorInitialize(void); // Ф-ия инициализации мотора
void motorReInitialize(void); // реинициализация мотора
void vibrate(float time);	// установка флага вибрации
void updateMotor(float dt);	// обновляет состояние мотора // dt в секундах
#endif /* MOTOR_H_ */
