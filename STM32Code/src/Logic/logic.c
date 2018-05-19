#include "logic.h"

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
  if(size != 0) set_buttonPressedFlag(1); // если хотя бы одна кнопка изменила положение, то ставим флаг
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
        set_recieveSTARTFlag(1);
        break;
      
      case RSTOP:
        set_recieveSTOPFlag(1);
        break;
      
      case RVIBRATE:
        set_recieveACTIONFlag(1);  // ставим флаг, что пришло действие
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
  if(OldSendIMUDataTime > IMU_DATA_TIME_SEND) set_timeToSendIMUDataFlag(1);  // если времени с предыдущей отправки прошло достаточно  
  int16_t rD[6];
  readIMUData(rD);
  float ax, ay, az, wx, wy, wz;
#ifdef GY85
  ax = rD[0]/GY85_A_SENSETIVE;
  ay = rD[1]/GY85_A_SENSETIVE;
  az = rD[2]/GY85_A_SENSETIVE;
  wx = DEGREE_TO_RAD*(rD[3]/GY85_G_SENSETIVE + GY85_X_OFFSET);
  wy = DEGREE_TO_RAD*(rD[4]/GY85_G_SENSETIVE + GY85_Y_OFFSET);
  wz = DEGREE_TO_RAD*(rD[5]/GY85_G_SENSETIVE + GY85_Z_OFFSET);  
#elif defined(MPU6050)
  ax = rD[0]/MPU6050_A_SENSETIVE;
  ay = rD[1]/MPU6050_A_SENSETIVE;
  az = rD[2]/MPU6050_A_SENSETIVE;
  wx = DEGREE_TO_RAD*(rD[3]/MPU6050_G_SENSETIVE);
  wy = DEGREE_TO_RAD*(rD[4]/MPU6050_G_SENSETIVE);
  wz = DEGREE_TO_RAD*(rD[5]/MPU6050_G_SENSETIVE);  
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
  set_timeToSendIMUDataFlag(0); 
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

uint8_t is_joystickWorkFlag(void) { return joystickWorkFlag; }
uint8_t is_timeToSendIMUDataFlag(void) { return timeToSendIMUDataFlag; }

uint8_t is_buttonPressedFlag(void)  // флаг очищается при чтении
{ 
  uint8_t temp = buttonPressedFlag;
  set_buttonPressedFlag(0);
  return temp;
}

uint8_t is_recieveSTOPFlag(void)
{ 
  uint8_t temp = recieveSTOPFlag;
  set_recieveSTOPFlag(0);
  return temp; 
}

uint8_t is_recieveSTARTFlag(void) 
{ 
  uint8_t temp = recieveSTARTFlag;
  set_recieveSTARTFlag(0);
  return temp;
}

uint8_t is_recieveACTIONFlag(void) 
{
  uint8_t temp = recieveACTIONFlag;
  set_recieveACTIONFlag(0);
  return temp;
}

void reInitAll(void)
{
  BluetoothReInitialize();
#ifdef MOTOR
  motorReInitialize();
#endif
  timerReInitialize();
  resetMajvikFilter();
  set_buttonPressedFlag(0); // очищаем флаги
  set_timeToSendIMUDataFlag(0);
  set_recieveSTOPFlag(0);
  set_recieveSTARTFlag(0);
  set_recieveACTIONFlag(0);
  OldSendIMUDataTime = 0.f;
  actionsAmount = 0;
}

void set_joystickWorkFlag(uint8_t b) { joystickWorkFlag = b; }
void set_buttonPressedFlag(uint8_t b) { buttonPressedFlag = b; }
void set_timeToSendIMUDataFlag(uint8_t b) { timeToSendIMUDataFlag = b; }
void set_recieveSTOPFlag(uint8_t b) { recieveSTOPFlag = b; }
void set_recieveSTARTFlag(uint8_t b) { recieveSTARTFlag = b; }
void set_recieveACTIONFlag(uint8_t b) { recieveACTIONFlag = b; }
//void setFlag_rVIBRATEF(uint8_t b) { recieveVIBRATEFlag = b; }


