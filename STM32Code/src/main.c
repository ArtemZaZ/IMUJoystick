#include "stm32l4xx.h"
#include "Filters/MajvikFilter.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
#include "Bluetooth/Bluetooth.h"
/* следующие определения определяют, какая конфигурация будет у проекта */
// MPU6050
// GY85
// MOTOR
// FIRST_BUTTON
// SECOND_BUTTON
// THIRD_BUTTON
/* ставятся в настройках проекта */
#include "Devices/devices.h"

enum {
	FSM_STOP,	// Джойстик не работает - начальное состояние конечной машины
	FSM_REINIT, // Переинициализация Джойстика
	FSM_START, 	// Джойстик ничинает работать - нормальный режим
	FSM_WAIT,	// Ожидание 
	FSM_GET_IMU_DATA,	// Чтение данных с I2C и фильтрация
	FSM_UPDATE, // если нужно что-то обновить
	FSM_FILTRATION_BUTTON, // Фильтрация кнопок 
	FSM_READ_UART, // Чтение данных с UART
	FSM_ACTION,	// Сделать какое-нибудь действие
	FSM_WRITE_UART	// Упаковка данных для передачи по UART и отправка по UART
}	State;



void FSM(void)
{
	switch(State)
	{
		case FSM_STOP:
			State = FSM_READ_UART;	// TODO: Пока не придет сигнал START с UART	переходим к чтению	 	
			return;
		
		case FSM_REINIT:
			if(!0)	// TODO: Дополнительные возможности переинициализации
			{
				
			}
			State = FSM_START;
			return;
			
		case FSM_START:
			if(!0)	// TODO: Еще что-нибудь
			{
				
			}
			State = FSM_GET_IMU_DATA;
			return;
			
		case FSM_WAIT:
			while(!1) // TODO: Сделать ожидание
			{
				
			}
			State = FSM_START; // TODO: Сделвть переход куда нужно
			return;
		
		case FSM_GET_IMU_DATA:
			// TODO: Получение данных с I2C и их фильтрация
			if(!0) State = FSM_READ_UART; // TODO: переход к чтению данных с UART, если еще прошло недостаточно времени для отправки новых данных
			else State = FSM_WRITE_UART;
			return;
		
		case FSM_UPDATE:
			// TODO: Оюновление данных
			State = FSM_FILTRATION_BUTTON;
			return;
			
		case FSM_FILTRATION_BUTTON:
			// TODO: Фильтрация кнопок
			if(!0) // TODO: Если нажата какая-то кнопка
			{
				// TODO: Записываем все что хотим отправить в UART
				State = FSM_WRITE_UART;	// TODO: Отправляем в UART
			}
			else State = FSM_ACTION;
			return;
			
		case FSM_READ_UART:
			// TODO: Читаем данные с UART
			if(!0) State = FSM_STOP; // TODO: Если пришел сигнал STOP
			else if(!0) State = FSM_REINIT; // TODO: Если пришел сигнал START
			else if(!0)	State = FSM_ACTION;	// TODO: Если пришел сигнал действия
			else  State = FSM_FILTRATION_BUTTON; // TODO: Если ничего из этого
			return;
		
		case FSM_ACTION:
			// TODO: Действия
			State = FSM_GET_IMU_DATA;
			return;
		
		case FSM_WRITE_UART:
			// TODO: Читаем данные
			State = FSM_READ_UART;
			return;
	}		
}



int main(void)       		   
{	
	IMUInitialize();	
	motorInitialize();
	ButtonsInitialize();	
	BluetoothInitialize();
	
	while(1)
	{
		FSM();	
	}
}
