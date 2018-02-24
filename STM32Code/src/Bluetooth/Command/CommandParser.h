/* Парсер команд, приходящих с bluetooth */
#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_
#include <stdint.h>
#include <string.h>

#define MAX_RECMESSAGE_LEN 14   // максимальная длина сообщения
typedef enum enumRecCommand   // приходящие комманды
{
	RSTART,
	RSTOP,
	RVIBRATE,                 // ФОРМАТ КОММАНДЫ: <COMMAND DATA>: <VIBRATE 001A>
	RERR
} RecCommand;

typedef struct           // структура приходящих данных
{
	RecCommand command;
	uint16_t data;
} RecData;

typedef enum enumFormat{
	BIGEN,															// порядок хранения данных. Порядок от старшего к младшему (англ. big-endian — большим концом)
	LITTLEN															// Порядок от младшего к старшему (англ. little-endian)
} Format;

static RecCommand commandDefiner(char *str);     		// определяет приходящие комманды
static uint8_t convertSymbolToNumber(char sym);		// перевод символа(шестнадцатиричного) в число
static uint8_t convertStringToNumber(char* str, uint8_t* ret, uint8_t size, Format format);   // перевод строки в число(запись в память)
RecData parsing(char* str, uint8_t size);
#endif /* COMMANDPARSER_H_ */
