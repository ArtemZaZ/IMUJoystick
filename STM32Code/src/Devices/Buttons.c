#include "Buttons.h"

void ButtonsInitialize(void)
{
#ifdef FIRST_BUTTON		
	buttonList[butCounter].buttonsGPIO = GPIOA;
	buttonList[butCounter].num = 1;
	buttonList[butCounter].pin = 3;
	buttonList[butCounter].isPressed = 0;
	buttonList[butCounter].filteredVar = 0.f;	
	buttonList[butCounter].hysteresis = 1;
	butCounter++;
#endif

#ifdef SECOND_BUTTON	
	buttonList[butCounter].buttonsGPIO = GPIOA;
	buttonList[butCounter].num = 2;
	buttonList[butCounter].pin = 4;
	buttonList[butCounter].isPressed = 0;
	buttonList[butCounter].filteredVar = 0.f;	
	buttonList[butCounter].hysteresis = 1;
	butCounter++;
#endif

#ifdef THIRD_BUTTON
	buttonList[butCounter].buttonsGPIO = GPIOA;
	buttonList[butCounter].num = 3;
	buttonList[butCounter].pin = 5;
	buttonList[butCounter].isPressed = 0;
	buttonList[butCounter].filteredVar = 0.f;	
	buttonList[butCounter].hysteresis = 1;
	butCounter++;
#endif
}

static inline uint8_t PushPress(Button* but)	// возвращаемое значение - изменилось ли состояние кнопки
{
	if((*but).hysteresis & LEVELUP((*but).filteredVar))	// петля вверх + пересечение уровня петли
	{
		(*but).isPressed = 1;
		(*but).hysteresis = 0;	// меняем направление петли
		return 1;
	}
	else if(!(*but).hysteresis & LEVELDOWN((*but).filteredVar))
	{
		(*but).isPressed = 0;
		(*but).hysteresis = 1;
		return 1;
	}
	else return 0;
}

void checkAndFiltrateButtons(Button* buttons, uint8_t* size)
{
	*size = 0;
	for(uint8_t i = 0; i < butCounter; i++)
	{
		HPFilterIterator((float)(((buttonList[i].buttonsGPIO -> ODR) & (1 << buttonList[i].pin)) >> buttonList[i].pin), &(buttonList[i].filteredVar), KP);	// фильтрация кнопки
		if(PushPress(&buttonList[i]))
		{
			*buttons++ = buttonList[i];
			(*size)++;
		}
	}
}
