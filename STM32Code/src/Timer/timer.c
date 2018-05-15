#include "timer.h"

void timerInitialize(void)
{
  RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // тактируем таймер
  NVIC_EnableIRQ(TIM2_IRQn);  // разрешаем прерывание по таймеру
  TIM2 -> CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);  // очищаем поля DIR и CMS: таймер идет вверх
  TIM2 -> ARR = PERIOD;  
  TIM2 -> PSC = (uint32_t)(SystemCoreClock / FREQ) - 1; // устанавливаем частоту тика таймера в FREQ
  TIM2 -> DIER |= TIM_DIER_UIE; // разрешаем прерывание по обновлению данных
  TIM2 -> CR1 |= TIM_CR1_CEN; // разрешаем считать
}

void timerReInitialize(void)
{
  TIM2 -> CNT = 0;
  allTime = 0;  
}

void TIM2_IRQHandler(void)
{
  if((TIM2 -> SR) & 1)
  {
    allTime++;  // увеличиваем счетчик периодов 
    TIM2 -> SR &= ~1; // очищаем флаг переполнения таймера
  }
}

float getAllTime()
{
  NVIC_DisableIRQ(TIM2_IRQn); // запрещаем прерывания
  uint32_t temp = TIM2 -> CNT;  // Измеряем тики
  uint32_t temp2 = TIM2 -> SR;  //
  temp2 = temp2 & 1;  // Измеряем статус 
  temp2 = temp2 & (uint32_t)!(TIM2 -> CNT < temp); // Смотрим было ли переключение таймера до получения статуса и корректируем его, если нужно
  float temp3 = ((float)((allTime + temp2)*PERIOD + temp))/FREQ;
  NVIC_EnableIRQ(TIM2_IRQn); // разрешаем прерывания
  return temp3;
}

float getDeltaTime(Timer* t)
{
  t -> newTime = getAllTime();
  float temp = t -> oldTime;
  t -> oldTime = t -> newTime;
  return (t -> newTime) - temp;
}

void resetTimer(Timer *t)
{
  t -> oldTime = getAllTime();
  t -> newTime = t -> oldTime;
}

