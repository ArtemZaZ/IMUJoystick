#include "stm32l4xx.h"
#include "Filters/MajvikFilter.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
#include "Bluetooth/Bluetooth.h"
#include "Logic/logic.h"
/* следующие определения определяют, какая конфигурация будет у проекта */
// MPU6050
// GY85
// MOTOR
// FIRST_BUTTON
// SECOND_BUTTON
// THIRD_BUTTON
/* ставятся в настройках проекта */
#include "Devices/devices.h"

int32_t rawData[7];
volatile uint8_t temp[2];

int main(void)       		   
{  
  IMUInitialize();
  
	while(1)
	{
		//FSM();	
    //sendMsg((SendData){"message\n", 8});
    //while(!BTransmit());
    //while(!Receive(ITG3205_ADDR, 0x00, (uint8_t*)temp, 1));
    readIMUData(rawData);
    temp[0] = temp[1];
	}
}
