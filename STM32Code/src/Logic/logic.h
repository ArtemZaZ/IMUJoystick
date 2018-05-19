#ifndef LOGIC_H_
#define LOGIC_H_
#include "Bluetooth/Bluetooth.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
#include "Filters/MajvikFilter.h"
#include "Devices/devices.h"
#include "Timer/timer.h"

void reInitAll(void); // реинициализация всего, что можно 
void sendSTART(void); // отправляет старт
void sendSTOP(void);  // отправляет стоп
void sendERROR(void); // отправка ошибки
void readMessages(void);  // читаем все полученные сообщения и ставим флаги
void filtrationAndSendChangedButtons(void);  // отправляет кнопки, изменившие состояние
void act(void); // выполняет действия
void updateIMUData(float time); // обновляет данные с IMU датчика
void updateActions(float time); // обновляем действия
void sendIMUData(void); // отправляем данные с IMU датчика
uint8_t is_joystickWorkFlag(void);  // проверяет активен ли флаг joystickWorkFlag
uint8_t is_buttonPressedFlag(void);  // проверяет активен ли флаг buttonPressedFlag
uint8_t is_timeToSendIMUDataFlag(void);  // проверяет активен ли флаг timeToSendIMUDataFlag
uint8_t is_recieveSTOPFlag(void); // проверяет активен ли флаг recieveSTOPFlag
uint8_t is_recieveSTARTFlag(void);  // проверяет активен ли флаг recieveSTARTFlag
uint8_t is_recieveACTIONFlag(void);  // проверяет активен ли флаг recieveACTIONFlag
void set_joystickWorkFlag(uint8_t b);  // установка флагов
void set_buttonPressedFlag(uint8_t b);
void set_timeToSendIMUDataFlag(uint8_t b);
void set_recieveSTOPFlag(uint8_t b);
void set_recieveSTARTFlag(uint8_t b);
void set_recieveACTIONFlag(uint8_t b);

void InitializeAll(void); 



#endif /* LOGIC_H_ */
