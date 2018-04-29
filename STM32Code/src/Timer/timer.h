#ifndef TIMER_H_
#define TIMER_H_
#include "stm32l4xx.h"

#define FREQ  200U // Hz
#define PERIOD  0x0000FFFFU//0xFFFFFFFFU//TIM_ARR_ARR  // ставим период обновления счетчиа на максимум
static volatile uint32_t allTime = 0;  // время в периодах таймера, прошедшее с начала включения

typedef struct timer
{
  float oldTime;
  float newTime;  
} Timer;

void timerInitialize(void);
void timerReInitialize(void);
void TIM2_IRQHandler(void); // обработчик прерываний
float getAllTime(void);  // возвращает все время с начала работы в секундах
float getDeltaTime(Timer *t); // возвращает промежуток времени с предыдущего запроса
void resetTimer(Timer *t);  // сбрасывает таймер

#endif /* TIMER_H_ */

