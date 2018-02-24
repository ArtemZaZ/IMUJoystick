#include "CMSIS/stm32f10x.h"
#include "Filters/MajvikFilter.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
/* следующие определения определяют, какая конигурация будет у проекта */
//#define MPU6050
#define GY85
#define MOTOR
#include "Devices/devices.h"

float yaw, pitch, roll;
volatile char* a;
volatile uint8_t temp[20];
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
	//convertNumberToString((uint8_t*)temp, 0xA1CD);
	while(1)
	{
		;		
	}
	

}
