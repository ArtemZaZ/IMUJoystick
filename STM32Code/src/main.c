#include "Logic/logic.h"
#define WAIT_TICKS  10000    // задержка конечного автомата
/* следующие определения определяют, какая конфигурация будет у проекта */
// MPU6050
// GY85
// MOTOR
// FIRST_BUTTON
// SECOND_BUTTON
// THIRD_BUTTON
/* ставятся в настройках проекта */

Timer IMUTimer;
Timer actionsTimer;


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
  FSM_WRITE_UART	// Отправка по UART
} State;


void FSM(void)
{
  switch(State)
  {
    case FSM_STOP:
      setFlag_jWF(0); // ставим флаг, что джойстик не работает
      State = FSM_READ_UART;	// Пока не придет сигнал START с UART	переходим к чтению
      return;
		
    case FSM_REINIT:      
      reInitAll(); 
      resetTimer(&IMUTimer);
      resetTimer(&actionsTimer);
      getDeltaTime(&IMUTimer);
      getDeltaTime(&actionsTimer);
      State = FSM_START;
      return;

    case FSM_START:
      sendSTART();  // отправляем сигнал, что джойстик начал работу
      setFlag_jWF(1); // ставим флаг, что джойстик работает
      State = FSM_GET_IMU_DATA;
      return;

    case FSM_WAIT:
      for(uint32_t i = 0; i < WAIT_TICKS; i++);
      State = FSM_GET_IMU_DATA; 
      return;

    case FSM_GET_IMU_DATA:      
      updateIMUData(getDeltaTime(&IMUTimer));
      if(isActiveFlag_tTSIMUDF())  // если активен флаг, что пора отправлять данные с IMU
      {
        sendIMUData();  // отправляем данные с IMU датчика
      }
      State = FSM_FILTRATION_BUTTON;
      return;
		  
    case FSM_UPDATE:
      updateActions(getDeltaTime(&actionsTimer));   
      State = FSM_WRITE_UART;
      return;
			
    case FSM_FILTRATION_BUTTON:
      filtrationAndSendChangedButtons();
      State = FSM_UPDATE;
      return;
			
    case FSM_READ_UART:
      if(isActiveFlag_bRC())  // если стоит флаг того, что можно читать сообщения
      {
        readMessages();
      }
      if(isActiveFlag_rSTARTF() && !isActiveFlag_jWF())
      {        
        State = FSM_REINIT; // Если пришел сигнал START и не активен флаг работы джойстика
      }
      else if(isActiveFlag_rSTOPF() || !isActiveFlag_jWF()) // Если пришел сигнал STOP или неактивен флаг работы джойстика
      {
        if(isActiveFlag_jWF())
        {
          sendSTOP(); // Если джойстик до этого работал - отправить стоп
          BTransmit();
        }
        State = FSM_STOP; 
      }
      else if(isActiveFlag_rACTIONF())	State = FSM_ACTION;	// Если пришел сигнал действия
      else  State = FSM_WAIT; // Если ничего из этого
      return;
		
    case FSM_ACTION:
      act();
      State = FSM_WAIT;
      return;
		
    case FSM_WRITE_UART:
      BTransmit(); 
      State = FSM_READ_UART;
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
