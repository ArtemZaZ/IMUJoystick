#include "logic.h"

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
  timerInitialize();
}

void filtrationAndSendChangedButtons(void)
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
  if(size != 0) setFlag_bPF(1); // если хотя бы одна кнопка изменила положение, то ставим флаг
  //else setFlag_bPF(0);
}

void readMessages(void)
{
  actionsAmount = 0;
  uint8_t size;
  recvMsg(messages, &size);
  for(uint8_t i = 0; i < size; i++)
  {
    switch(messages[i].command)
    {
      case RSTART:
        setFlag_rSTARTF(1);
        break;
      
      case RSTOP:
        setFlag_rSTOPF(1);
        break;
      
      case RVIBRATE:
        setFlag_rACTIONF(1);  // ставим флаг, что пришло действие
        actions[actionsAmount] = messages[i]; // копируем сообщение в массив действий
        actionsAmount++;
        break;
      
      default:
        break;      
    }
  }
}

void updateIMUData(float time)
{
#ifdef IMU
  OldSendIMUDataTime += time; // обновляем время прошедшее с предыдущей отправки
  if(OldSendIMUDataTime > IMU_DATA_TIME_SEND) setFlag_tTSIMUDF(1);  // если времени с предыдущей отправки прошло достаточно  
  int16_t rD[6];
  readIMUData(rD);
  float ax, ay, az, wx, wy, wz;
#ifdef GY85
  ax = rD[0]/GY85_A_SENSETIVE;
  ay = rD[1]/GY85_A_SENSETIVE;
  az = rD[2]/GY85_A_SENSETIVE;
  wx = DEGREE_TO_RAD*(rD[3]/GY85_G_SENSETIVE + goffx);
  wy = DEGREE_TO_RAD*(rD[4]/GY85_G_SENSETIVE + goffy);
  wz = DEGREE_TO_RAD*(rD[5]/GY85_G_SENSETIVE + goffz);  
#elif defined(MPU6050)
  ax = rD[0]/MPU6050_A_SENSETIVE;
  ay = rD[1]/MPU6050_A_SENSETIVE;
  az = rD[2]/MPU6050_A_SENSETIVE;
  wx = DEGREE_TO_RAD*(rD[3]/MPU6050_G_SENSETIVE + goffx);
  wy = DEGREE_TO_RAD*(rD[4]/MPU6050_G_SENSETIVE + goffy);
  wz = DEGREE_TO_RAD*(rD[5]/MPU6050_G_SENSETIVE + goffz);  
#endif  /* GY85 */
  MajvikFilter(ax, ay, az, wx, wy, wz, time); // обновляем данные  
#endif  /* IMU */
}

void act(void)
{
  for(uint8_t i = 0; i < actionsAmount; i++)
  {
    switch(actions[i].command)
    {
      case RVIBRATE:
#ifdef MOTOR
        vibrate((float)actions[i].data);
#endif
        break;
      
      default:
        break;
    }
  }
}

void sendIMUData(void)
{
#ifdef IMU
  float yaw, pitch, roll;
  getEulerAngle(&yaw, &pitch, &roll);
  uint8_t spitch = (uint8_t)(int8_t)(float)(((float)((int32_t)pitch % 360)/360.f)*256.f); // урезаем углы до целых, делаем множество кольцом, помещаем их в промежуток от 0 до 1(float), потом в промежуток от 0 до 255(float), урезаем до целых
  uint8_t sroll = (uint8_t)(int8_t)(float)(((float)((int32_t)roll % 360)/360.f)*256.f);
  uint16_t data = (uint16_t)((spitch << 8) | sroll);
  sendMsg(packing(SPRD, data));
  
  OldSendIMUDataTime = 0.f; // обнуляем время прошедшее с предыдущей отправки
  setFlag_tTSIMUDF(0); 
#endif 
}

void updateActions(float time)
{
#ifdef MOTOR
  updateMotor(time);
#endif  
}

void sendSTART(void) { sendMsg(packing(SSTART, 0)); }
void sendSTOP(void) { sendMsg(packing(SSTOP, 0)); }
void sendERROR(void) { sendMsg(packing(SERR, 0)); }

uint8_t isActiveFlag_jWF(void) { return joystickWorkFlag; }
uint8_t isActiveFlag_tTSIMUDF(void) { return timeToSendIMUDataFlag; }

uint8_t isActiveFlag_bPF(void)  // флаг очищается при чтении
{ 
  uint8_t temp = buttonPressedFlag;
  setFlag_bPF(0);
  return temp;
}

uint8_t isActiveFlag_rSTOPF(void)
{ 
  uint8_t temp = recieveSTOPFlag;
  setFlag_rSTOPF(0);
  return temp; 
}

uint8_t isActiveFlag_rSTARTF(void) 
{ 
  uint8_t temp = recieveSTARTFlag;
  setFlag_rSTARTF(0);
  return temp;
}

uint8_t isActiveFlag_rACTIONF(void) 
{
  uint8_t temp = recieveACTIONFlag;
  setFlag_rACTIONF(0);
  return temp;
}
/*
uint8_t isActiveFlag_rVIBRATEF(void) 
{
  uint8_t temp = recieveVIBRATEFlag;
  setFlag_rVIBRATEF(0);
  return temp;
}
*/

void reInitAll(void)
{
  BluetoothReInitialize();
#ifdef MOTOR
  motorReInitialize();
#endif
  timerReInitialize();
  resetMajvikFilter();
  setFlag_bPF(0); // очищаем флаги
  setFlag_tTSIMUDF(0);
  setFlag_rSTOPF(0);
  setFlag_rSTARTF(0);
  setFlag_rACTIONF(0);
  OldSendIMUDataTime = 0.f;
  actionsAmount = 0;
}

void setFlag_jWF(uint8_t b) { joystickWorkFlag = b; }
void setFlag_bPF(uint8_t b) { buttonPressedFlag = b; }
void setFlag_tTSIMUDF(uint8_t b) { timeToSendIMUDataFlag = b; }
void setFlag_rSTOPF(uint8_t b) { recieveSTOPFlag = b; }
void setFlag_rSTARTF(uint8_t b) { recieveSTARTFlag = b; }
void setFlag_rACTIONF(uint8_t b) { recieveACTIONFlag = b; }
//void setFlag_rVIBRATEF(uint8_t b) { recieveVIBRATEFlag = b; }


