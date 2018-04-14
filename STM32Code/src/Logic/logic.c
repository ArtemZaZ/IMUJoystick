#include "logic.h"

void FSM(void)
{
	switch(State)
	{
		case FSM_STOP:
      setFlag_jWF(0);
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
				sendSTART();
        setFlag_jWF(1);
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
			UpdateIMUData(0.f);
			if(!isActiveFlag_tTSIMUDF())
      {
        State = FSM_READ_UART; // переход к чтению данных с UART, если еще прошло недостаточно времени для отправки новых данных
      }
			else State = FSM_WRITE_UART;
			return;
		
		case FSM_UPDATE:
			UpdateActions(0.f);   
			State = FSM_FILTRATION_BUTTON;
			return;
			
		case FSM_FILTRATION_BUTTON:
			FiltrationAndSendChangedButtons();
			if(isActiveFlag_bPF()) // Если нажата какая-то кнопка
			{
				State = FSM_WRITE_UART;	// Отправляем в UART
			}
			else State = FSM_ACTION;
			return;
			
		case FSM_READ_UART:
			// TODO: Читаем данные с UART
      if(isActiveFlag_rSTOPF() || !isActiveFlag_jWF()) State = FSM_STOP; // TODO: Если пришел сигнал STOP или неактивен флаг работы джойстика
			else if(isActiveFlag_rSTARTF()) State = FSM_REINIT; // TODO: Если пришел сигнал START
			else if(isActiveFlag_rACTIONF())	State = FSM_ACTION;	// TODO: Если пришел сигнал действия
			else  State = FSM_FILTRATION_BUTTON; // TODO: Если ничего из этого
			return;
		
		case FSM_ACTION:
			// TODO: Действия
			State = FSM_GET_IMU_DATA;
			return;
		
		case FSM_WRITE_UART:
			// TODO: пишем данные
			State = FSM_READ_UART;
			return;
	}		
}

void InitializeAll(void)
{
#ifdef IMU
	IMUInitialize();	
#endif
#ifdef MOTOR
	motorInitialize();
#endif
#ifdef BUTTONS
	ButtonsInitialize();
#endif	
	BluetoothInitialize();
}

void FiltrationAndSendChangedButtons(void)
{
  uint8_t size = 0;
#ifdef BUTTONS  
  checkAndFiltrateButtons(buttons, &size);
  for(uint8_t i = 0; i < size; i++) // проходим по всем кнопкам, изменившим значение
  {
    uint16_t data = (uint16_t)((buttons[i].num << 8) | buttons[i].isPressed); // первый байт - номер кнопки, второй - нажата или отжата
    sendMsg(packing(SBUT, data)); // упаковываем и отправляем  
  }  
#endif
  if(size != 0) setFlag_bPF(1);
  else setFlag_bPF(0);
}

void UpdateIMUData(float time)
{
#ifdef IMU
  OldSendIMUDataTime += time; // обновляем время прошедшее с предыдущей отправки
  if(OldSendIMUDataTime > IMU_DATA_TIME_SEND) setFlag_tTSIMUDF(1);  // если времени с предыдущей отправки прошло достаточно  
  float rD[6];
  readIMUData(rD);
  MajvikFilter(rD[0], rD[1], rD[2], rD[3], rD[4], rD[5], time); // обновляем данные  
#endif   
}

void sendIMUData(void)
{
#ifdef IMU
  float yaw, pitch, roll;
  getEulerAngle(&yaw, &pitch, &roll);
  uint8_t spitch = (uint8_t)((float)(((uint32_t)pitch % 360)/360) * 256); // урезаем углы до целых, делаем множество кольцом, помещаем их в промежуток от 0 до 1(float), потом в промежуток от 0 до 255(float), урезаем до целых
  uint8_t sroll = (uint8_t)((float)(((uint32_t)roll % 360)/360) * 256);
  uint16_t data = (uint16_t)((spitch << 8) | sroll);
  sendMsg(packing(SPRD, data));
  
  OldSendIMUDataTime = 0.f; // обнуляем время прошедшее с предыдущей отправки
  setFlag_tTSIMUDF(0); 
#endif 
}

void UpdateActions(float time)
{
#ifdef MOTOR
  updateMotor(time);
#endif  
}

void sendSTART(void) { sendMsg(packing(SSTART, 0)); }
void sendSTOP(void) { sendMsg(packing(SSTOP, 0)); }

uint8_t isActiveFlag_jWF(void) { return joystickWorkFlag; }
uint8_t isActiveFlag_bPF(void) { return buttonPressedFlag; }
uint8_t isActiveFlag_tTSIMUDF(void) { return timeToSendIMUDataFlag; }
uint8_t isActiveFlag_rSTOPF(void) { return recieveSTOPFlag; }
uint8_t isActiveFlag_rSTARTF(void) { return recieveSTARTFlag; }
uint8_t isActiveFlag_rACTIONF(void) { return recieveACTIONFlag; }

void setFlag_jWF(uint8_t b) { joystickWorkFlag = b; }
void setFlag_bPF(uint8_t b) { buttonPressedFlag = b; }
void setFlag_tTSIMUDF(uint8_t b) { timeToSendIMUDataFlag = b; }
void setFlag_rSTOPF(uint8_t b) { recieveSTOPFlag = b; }
void setFlag_rSTARTF(uint8_t b) { recieveSTARTFlag = b; }
void setFlag_rACTIONF(uint8_t b) { recieveACTIONFlag = b; }


