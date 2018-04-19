#ifndef TIMER_H_
#define TIMER_H_
#include "stm32l4xx.h"

#define FREQ  500 // Hz
#define PERIOD  TIM_ARR_ARR  // ставим период обновления счетчиа на максимум
static volatile uint32_t allTime = 0;  // время в периодах таймера, прошедшее с начала включения

void timerInitialize(void);
void timerReInitialize(void);
void TIM2_IRQHandler(void); // обработчик прерываний
float getAllTime(void);  // возвращает все время с начала работы в секундах




#endif /* TIMER_H_ */

