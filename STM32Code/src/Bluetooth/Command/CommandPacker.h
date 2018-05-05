/* Упаковщик команд для отправки() */
#ifndef COMMANDPACKER_H_
#define COMMANDPACKER_H_
#include <stdint.h>   
#include <string.h>

#define MAX_SENDMESSAGE_LEN 12

typedef enum enumSendCommand  // приходящие комманды
{
  SSTART, // формат: <START 0000>
  SSTOP,  // формат: <STOP 0000>
  SPRD, // Формат: <PRD 001A>  2 байта по 1 байту на ось pitch и roll
  SBUT, // Формат: <BUT 0101>  2 байта 1й байт - номер кнопки, второй байт - значение
  SERR  // Формат: <ERROR 0000>
} SendCommand;

typedef struct  // структура приходящих данных
{
  uint8_t message[MAX_SENDMESSAGE_LEN];
  uint8_t size;
} SendData;

SendData packing(SendCommand command, uint16_t data);
static uint8_t convertNumberToSymbol(uint8_t num);  //num only 0..9
static void convertNumberToString(uint8_t* ret, uint16_t num);  // перевод числа в строку, не библиотечная, т.к. нужно дополнять строку 0

#endif /* COMMANDPACKER_H_ */ 
