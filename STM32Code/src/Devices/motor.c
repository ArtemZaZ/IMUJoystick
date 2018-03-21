#include "motor.h"

void motorInitialize(void)
{
#if 	(PORT_MOTOR == 0xA)
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
#elif (PORT_MOTOR == 0xB)
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
#elif (PORT_MOTOR == 0xC)
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
#endif
	
	GPIO_PORT_MOTOR -> MODER &= ~(0x3 << PIN_MOTOR*2); // очищаем MODER для нужного пина
	GPIO_PORT_MOTOR -> MODER |= (0x1 << PIN_MOTOR*2);	// устанавливаем режим output
	GPIO_PORT_MOTOR -> OTYPER &= ~(0x1 << PIN_MOTOR); //очищаем OTTYPER - push-pull
	GPIO_PORT_MOTOR -> OSPEEDR &= ~(0x3 << PIN_MOTOR*2); // low speed
	GPIO_PORT_MOTOR -> PUPDR &= ~(0x3 << PIN_MOTOR*2);	// очищаем PUPDR
	GPIO_PORT_MOTOR -> PUPDR |= (0x1 << PIN_MOTOR*2); // pull-up
}

void vibrate(void)
{
	isVibrate = 0x1;
}

void update(float dt)	
{
	time += dt;
	if(VIBRO_TIME < time)	// если время прошедшее с начала вибрации больше нужного времени
	{
		isVibrate = 0x0;
		time = 0.f;
	}
	GPIO_PORT_MOTOR -> ODR |= (isVibrate << PIN_MOTOR);		// включаем или выключаем мотор
	GPIO_PORT_MOTOR -> ODR &= ~(!isVibrate << PIN_MOTOR);
}