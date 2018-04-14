#include "timer.h"

void timerInitialize(void)
{
  RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // тактируем таймер
  NVIC_EnableIRQ(TIM2_IRQn);  // разрешаем прерывание по таймеру
  TIM2 -> CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);  // очищаем поля DIR и CMS: таймер идет вверх
  TIM2 -> ARR |= PERIOD;  
  TIM2 -> PSC = (uint32_t)(SystemCoreClock / FREQ) - 1; // устанавливаем частоту тика таймера в FREQ
  TIM2 -> DIER |= TIM_DIER_UIE; // разрешаем прерывание по обновлению данных
  TIM2 -> CR1 |= TIM_CR1_CEN; // разрешаем считать
}

void TIM2_IRQHandler(void)
{
  allTime++;  // увеличиваем счетчик периодов 
}

float getAllTime()
{
  return (float)((allTime*PERIOD + (TIM2 -> CNT))/SystemCoreClock);
}

