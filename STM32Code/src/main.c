#include "stm32l4xx.h"
#include "Filters/MajvikFilter.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
/* следующие определения определяют, какая конигурация будет у проекта */
#define MPU6050
//#define GY85
#define MOTOR
#include "Devices/devices.h"

float yaw, pitch, roll;
volatile char* a;
volatile uint8_t temp[20] = {1,0,0,1};
RecData b;
SendData c;
volatile char* data0 = "   <START 0001>123456";
volatile char* data1 = "  <STOP 0004>  ";
volatile char* data3 = "  <VIBRATE 0010>   ";
int main(void)       		   
{											
	MajvikFilter(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.01f);
	getEulerAngle(&yaw, &pitch, &roll);
  
	b = parsing((char*)data3, strlen((const char*)data3));
	c = packing(SSTART, 0xA1CD);
	//RCC -> AHB2ENR |= (1<<1);	// Даем питание на порт B
	//GPIOB -> MODER &= ~(1<<7);
	//GPIOB -> MODER |= (1<<6);	// устанавливаем режим - выход
	//GPIOB -> OTYPER &= (1<<3); //устанавливаем режим выхода push-pull
	//GPIOB -> PUPDR &= ~(1<<7);
	//GPIOB -> PUPDR |= (1<<6);
	I2CInit();
	
	Transmit((uint8_t)0x010101, (uint8_t*)temp, 4);
	
	//convertNumberToString((uint8_t*)temp, 0xA1CD);
	while(1)
	{
		;		
	}
	

}
