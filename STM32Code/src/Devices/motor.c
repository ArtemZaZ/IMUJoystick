#include "motor.h"

#define PORT_MOTOR 0xB
#define PIN_MOTOR 3
//#define VIBRO_TIME 3.f // 3 секунды

#if (PORT_MOTOR == 0xA)
  #define GPIO_PORT_MOTOR GPIOA
#elif (PORT_MOTOR == 0xB)
  #define GPIO_PORT_MOTOR GPIOB
#elif (PORT_MOTOR == 0xC)
  #define	GPIO_PORT_MOTOR GPIOC
#else 
  #error "Motor port not selected"
#endif

#ifndef PIN_MOTOR
  #error "Motor pin not selected"
#endif

static volatile uint8_t isVibrate = 0x0; // флаг обозначающий должен ли мотор крутиться или нет
static volatile float vibroTime = 0.f; // время, которое должен вибрировать мотор
static volatile float time = 0.f; // время прошедшее с начала вибрации

void motorInitialize(void)
{
#if 	(PORT_MOTOR == 0xA)
  RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
#elif (PORT_MOTOR == 0xB)
  RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
#elif (PORT_MOTOR == 0xC)
  RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
#endif
  GPIO_PORT_MOTOR -> MODER &= ~(0x3 << PIN_MOTOR*2);  // очищаем MODER для нужного пина
  GPIO_PORT_MOTOR -> MODER |= (0x1 << PIN_MOTOR*2); // устанавливаем режим output
  GPIO_PORT_MOTOR -> OTYPER &= ~(0x1 << PIN_MOTOR); //очищаем OTTYPER - push-pull
  GPIO_PORT_MOTOR -> OSPEEDR &= ~(0x3 << PIN_MOTOR*2);  // low speed
  //GPIO_PORT_MOTOR -> PUPDR &= ~(0x3 << PIN_MOTOR*2);  // очищаем PUPDR
  //GPIO_PORT_MOTOR -> PUPDR |= (0x1 << PIN_MOTOR*2); // pull-up
}

void motorReInitialize(void)
{
  isVibrate = 0x0;
  time = 0.f;
  vibroTime = 0.f;
  GPIO_PORT_MOTOR -> ODR &= ~(1 << PIN_MOTOR);
}

void vibrate(float time)
{
  isVibrate = 0x1;
  vibroTime += time;  // время вибрации добавляется к предыдущему
}

void updateMotor(float dt)	
{
  if(isVibrate)
  {
    time += dt;
    if(vibroTime < time) // если время прошедшее с начала вибрации больше нужного времени
    {
      isVibrate = 0x0;
      vibroTime = 0.f;
      time = 0.f;
    }
    GPIO_PORT_MOTOR -> ODR |= (isVibrate << PIN_MOTOR); // включаем или выключаем мотор
    GPIO_PORT_MOTOR -> ODR &= ~((!isVibrate) << PIN_MOTOR);
  }
}
