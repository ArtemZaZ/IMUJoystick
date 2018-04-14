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

int main(void)       		   
{  

	while(1)
	{
		//FSM();	
    //sendMsg((SendData){"message\n", 8});
    //while(!BTransmit());
    //readIMUData(rawData);
	}
}
