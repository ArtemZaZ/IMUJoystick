#ifndef TIMER_H_
#define TIMER_H_
#include "stm32l4xx.h"

typedef struct timer
{
  float oldTime;
  float newTime;  
} Timer;

void timerInitialize(void);
void timerReInitialize(void);
float getAllTime(void);  // возвращает все время с начала работы в секундах
float getDeltaTime(Timer *t); // возвращает промежуток времени с предыдущего запроса
void resetTimer(Timer *t);  // сбрасывает таймер

#endif /* TIMER_H_ */

