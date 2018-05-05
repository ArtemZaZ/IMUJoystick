#ifndef LOGIC_H_
#define LOGIC_H_
#include "Bluetooth/Bluetooth.h"
#include "Bluetooth/Command/CommandParser.h"
#include "Bluetooth/Command/CommandPacker.h"
#include "Filters/MajvikFilter.h"
#include "Devices/devices.h"
#include "Timer/timer.h"

#define IMU_DATA_TIME_SEND  0.1f // максимальное время задержки в отправке сообщений с IMU
#define MAX_AMOUNT_MESSAGES (int)(MAX_UART_RX_BUFFER_LEN / MAX_RECMESSAGE_LEN) + 5 // максимальное кол-во действий - длина буффера UART/длину максимального сообщения

static Button buttons[MAX_AMOUNT_BUTTONS]; // массив кнопок, меняющих значение
static RecData messages[MAX_AMOUNT_MESSAGES]; // массив всех сообщений, приходящих с UART(вспомогательный массив)
static RecData actions[MAX_AMOUNT_MESSAGES]; // массив действий, приходящих с UART
static uint8_t actionsAmount = 0; // количество действий, пришедших с UART

static volatile float OldSendIMUDataTime = 0.f; // время прошедшее с предыдущей отправки сообщения

static volatile uint8_t joystickWorkFlag = 0; // флаг, работает ли джойстик
static volatile uint8_t buttonPressedFlag = 0;  // флаг, что хоть одна кнопка была нажата, флаг очищается только при его чтении
static volatile uint8_t timeToSendIMUDataFlag = 0;  // флаг, что пора отправлять данные с IMU
static volatile uint8_t recieveSTOPFlag = 0;  // флаг, что пришел сигнал STOP, флаг очищается только при его чтении
static volatile uint8_t recieveSTARTFlag = 0; // флаг, что пришел сигнал START, флаг очищается только при его чтении
static volatile uint8_t recieveACTIONFlag = 0; // флаг, что пришел сигнал действия, флаг очищается только при его чтении
//static volatile uint8_t recieveVIBRATEFlag = 0; // флаг, что пришел сигнал действия - включить вибромотор, флаг очищается только при его чтении

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
uint8_t isActiveFlag_jWF(void);  // проверяет активен ли флаг joystickWorkFlag
uint8_t isActiveFlag_bPF(void);  // проверяет активен ли флаг buttonPressedFlag
uint8_t isActiveFlag_tTSIMUDF(void);  // проверяет активен ли флаг timeToSendIMUDataFlag
uint8_t isActiveFlag_rSTOPF(void); // проверяет активен ли флаг recieveSTOPFlag
uint8_t isActiveFlag_rSTARTF(void);  // проверяет активен ли флаг recieveSTARTFlag
uint8_t isActiveFlag_rACTIONF(void);  // проверяет активен ли флаг recieveACTIONFlag
//uint8_t isActiveFlag_rVIBRATEF(void);  // проверяет активен ли флаг recieveVIBRATEFlag
void setFlag_jWF(uint8_t b);  // установка флагов
void setFlag_bPF(uint8_t b);
void setFlag_tTSIMUDF(uint8_t b);
void setFlag_rSTOPF(uint8_t b);
void setFlag_rSTARTF(uint8_t b);
void setFlag_rACTIONF(uint8_t b);
//void setFlag_rVIBRATEF(uint8_t b);

void InitializeAll(void); 
//void FSM(void); // конечный автомат


#endif /* LOGIC_H_ */
