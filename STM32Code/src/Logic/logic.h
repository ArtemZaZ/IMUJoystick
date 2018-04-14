#ifndef LOGIC_H_
#define LOGIC_H_
#include "Bluetooth/Bluetooth.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
#include "Filters/MajvikFilter.h"
#include "Devices/devices.h"

#define IMU_DATA_TIME_SEND  0.1f // максимальное время задержки в отправке сообщений с IMU

static Button buttons[MAX_AMOUNT_BUTTONS]; // массив кнопок меняющих значение
static volatile float OldSendIMUDataTime = 0.f; // время прошедшее с предыдущей отправки сообщения

static volatile uint8_t joystickWorkFlag = 0; // флаг, работает ли джойстик
static volatile uint8_t buttonPressedFlag = 0;  // флаг, что хоть одна кнопка была нажата
static volatile uint8_t timeToSendIMUDataFlag = 0;  // флаг, что пора отправлять данные с IMU
static volatile uint8_t recieveSTOPFlag = 0;  // флаг, что пришел сигнал STOP
static volatile uint8_t recieveSTARTFlag = 0; // флаг, что пришел сигнал START
static volatile uint8_t recieveACTIONFlag = 0; // флаг, что пришел сигнал действия


static enum {
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


void sendSTART(void); // отправляет старт
void sendSTOP(void);  // отправляет стоп
void FiltrationAndSendChangedButtons(void);  // отправляет кнопки, изменившие состояние
void UpdateIMUData(float time); // обновляет данные с IMU датчика
void UpdateActions(float time); // обновляем действия
void sendIMUData(void); // отправляем данные с IMU датчика
uint8_t isActiveFlag_jWF(void);  // проверяет активен ли флаг joystickWorkFlag
uint8_t isActiveFlag_bPF(void);  // проверяет активен ли флаг buttonPressedFlag
uint8_t isActiveFlag_tTSIMUDF(void);  // проверяет активен ли флаг timeToSendIMUDataFlag
uint8_t isActiveFlag_rSTOPF(void); // проверяет активен ли флаг recieveSTOPFlag
uint8_t isActiveFlag_rSTARTF(void);  // проверяет активен ли флаг recieveSTARTFlag
uint8_t isActiveFlag_rACTIONF(void);  // проверяет активен ли флаг recieveACTIONFlag
void setFlag_jWF(uint8_t b);  // установка флагов
void setFlag_bPF(uint8_t b);
void setFlag_tTSIMUDF(uint8_t b);
void setFlag_rSTOPF(uint8_t b);
void setFlag_rSTARTF(uint8_t b);
void setFlag_rACTIONF(uint8_t b);

void InitializeAll(void); 
void FSM(void); // конечный автомат


#endif /* LOGIC_H_ */
