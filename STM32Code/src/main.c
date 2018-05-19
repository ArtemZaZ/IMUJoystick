#include "Logic/logic.h"
#define WAIT_TICKS  10000    // задержка конечного автомата
/* следующие определения определяют, какая конфигурация будет у проекта */
// MPU6050 -- не доделано --
// GY85
// MOTOR
// FIRST_BUTTON
// SECOND_BUTTON
// THIRD_BUTTON
/* ставятся в настройках проекта */

Timer IMUTimer;
Timer actionsTimer;

void FSM(void)
{
  static enum {
    STOP,	// Джойстик не работает - начальное состояние конечной машины
    REINIT, // Переинициализация Джойстика
    START, 	// Джойстик ничинает работать - нормальный режим
    WAIT,	// Ожидание 
    GET_IMU_DATA,	// Чтение данных с I2C и фильтрация
    UPDATE, // если нужно что-то обновить
    FILTRATION_BUTTON, // Фильтрация кнопок 
    READ_UART, // Чтение данных с UART
    ACTION,	// Сделать какое-нибудь действие
    WRITE_UART	// Отправка по UART
  } State;
  
  switch(State)
  {
    case STOP:
      set_joystickWorkFlag(0); // ставим флаг, что джойстик не работает
      State = READ_UART;	// Пока не придет сигнал START с UART	переходим к чтению
      return;
		
    case REINIT:      
      reInitAll(); 
      resetTimer(&IMUTimer);
      resetTimer(&actionsTimer);
      getDeltaTime(&IMUTimer);
      getDeltaTime(&actionsTimer);
      State = START;
      return;

    case START:
      sendSTART();  // отправляем сигнал, что джойстик начал работу
      set_joystickWorkFlag(1); // ставим флаг, что джойстик работает
      State = GET_IMU_DATA;
      return;

    case WAIT:
      for(uint32_t i = 0; i < WAIT_TICKS; i++);
      State = GET_IMU_DATA; 
      return;

    case GET_IMU_DATA:      
      updateIMUData(getDeltaTime(&IMUTimer));
      if(is_timeToSendIMUDataFlag())  // если активен флаг, что пора отправлять данные с IMU
      {
        sendIMUData();  // отправляем данные с IMU датчика
      }
      State = FILTRATION_BUTTON;
      return;
		  
    case UPDATE:
      updateActions(getDeltaTime(&actionsTimer));   
      State = WRITE_UART;
      return;
			
    case FILTRATION_BUTTON:
      filtrationAndSendChangedButtons();
      State = UPDATE;
      return;
			
    case READ_UART:
      if(is_bluetoothReceiveCompleteFlag())  // если стоит флаг того, что можно читать сообщения
      {
        readMessages();
      }
      if(is_recieveSTARTFlag() && !is_joystickWorkFlag())
      {        
        State = REINIT; // Если пришел сигнал START и не активен флаг работы джойстика
      }
      else if(is_recieveSTOPFlag() || !is_joystickWorkFlag()) // Если пришел сигнал STOP или неактивен флаг работы джойстика
      {
        if(is_joystickWorkFlag())
        {
          sendSTOP(); // Если джойстик до этого работал - отправить стоп
          BTransmit();
        }
        State = STOP; 
      }
      else if(is_recieveACTIONFlag())	State = ACTION;	// Если пришел сигнал действия
      else  State = WAIT; // Если ничего из этого
      return;
		
    case ACTION:
      act();
      State = WAIT;
      return;
		
    case WRITE_UART:
      BTransmit(); 
      State = READ_UART;
      return;
  }		
}


int main(void)       		   
{  
  InitializeAll();
  while(1)
  { 
    FSM();
  }
}
