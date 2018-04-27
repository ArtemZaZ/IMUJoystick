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
uint8_t temp[10];
RecData data[4];
uint8_t size;
float t;
uint32_t tem = 0;
Button but[5];

int main(void)       		   
{  
  ButtonsInitialize();
  //IMUInitialize();
  //BluetoothInitialize();
  //timerInitialize();
  //recvMsg(data, &size);
	while(1)
	{
    checkAndFiltrateButtons(but, &size);
    if(size != 0)
    {
      tem++;
    }
    //t = getAllTime();
    //tem = get();
		//FSM();	
    //sendMsg((SendData){"message\n", 8});
    //while(!BTransmit());
    //while(!Receive(ADXL345_ADDR, ADXL345_POWER_CTL, (uint8_t*)&size, 1));
    //while(!Receive(ITG3205_ADDR, 0x00, (uint8_t*)&size, 1));
    //readIMUData(rawData);
    I2CInitDelay();
    I2CInitDelay();
    //temp[0] = temp[1];
	}
}
