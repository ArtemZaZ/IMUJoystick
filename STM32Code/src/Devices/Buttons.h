/* файл с ф-иями для кнопок*/
#ifndef BUTTONS_H_
#define BUTTONS_H_
#include <stdint.h>
#include "stm32l432xx.h"
#include "Filters/HLPFilters.h"
#include "Bluetooth/Command/CommandPacker.h"

#define MAX_AMOUNT_BUTTONS 5	// максимальное кол-во кнопок

typedef struct button
{
  GPIO_TypeDef* buttonsGPIO;	//	порт, на котором висит кнопка
  uint8_t num;	// номер кнопки
  uint8_t pin;	// пин, на котором висит кнопка
  uint8_t isPressed;	// флаг - нажата ли кнопка
  float filteredVar;	// СЛУЖЕБНОЕ: фильтрованное значение	
  uint8_t hysteresis;	// СЛУЖЕБНОЕ: направление петли гистерезиса 1 - вверх; 0 - вниз;
} Button;

static inline uint8_t PushPress(Button* but);	// определение нажата ли кнопка по уровню фильрации
void ButtonsInitialize(void);		// инициализация кнопок
void checkAndFiltrateButtons(Button* buttons, uint8_t* size);		// возвращает массив кнопок изменивших положение и их количество



#endif /* BUTTONS_H_ */
