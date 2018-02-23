/* Упаковщик команд для отправки */
#ifndef COMMANDPACKER_H_
#define COMMANDPACKER_H_

typedef enum enumSendCommand   // приходящие комманды
{
	START,                // формат: <START 0000>
	STOP,                 // формат: <STOP 0000>
	PRD,               		// Формат: <PRD 001A>  2 байта по 1 байту на ось pitch и roll
	ERR                   // Формат: <ERROR 0000>
} SendCommand;

#endif /* COMMANDPACKER_H_ */ 
