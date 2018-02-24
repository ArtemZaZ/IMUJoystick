#include "CommandParser.h"

static RecCommand commandDefiner(char *str)          // определитель приходящих комманд
{
	if(!strncmp(str, "START", 5))	return RSTART;
	if(!strncmp(str, "STOP", 4))	return RSTOP;
	if(!strncmp(str, "VIBRATE", 7))	return RVIBRATE;
	return RERR;	
}

static uint8_t convertSymbolToNumber(char sym)			// перевод символа(шестнадцатиричного(Только верхний регистр)), в число
{
	if((sym>=(uint8_t)'0')&&(sym<=(uint8_t)'9')) return (uint8_t)sym - (uint8_t)'0';					// Для цифр
	if((sym>=(uint8_t)'A')&&(sym<=(uint8_t)'F')) return ((uint8_t)sym - (uint8_t)'A') + 10;		// для букв
	else return 0;																																						// 
}

static uint8_t convertStringToNumber(char* str, uint8_t* ret, uint8_t size, Format format)					// Переводит шестнадцатиричную символьную строку в числовую строку и записывает ее в область памяти, на которую указывает ret 
{
	switch(format)
	{
		case LITTLEN:
			for(uint8_t i = 0; i < size/2; i++)
			{
				ret[size/2-i-1] = convertSymbolToNumber(str[2*i])*16 + convertSymbolToNumber(str[2*i+1]);
			}
			return 1;
		
		case BIGEN:
			for(uint8_t i = 0; i < size/2; i++)
			{
				ret[i] = convertSymbolToNumber(str[2*i])*16 + convertSymbolToNumber(str[2*i+1]);
			}
			return 1;
		
		default:
			return 0;
	}
}

RecData parsing(char* str, uint8_t size)
{
	RecData ret;
	while((*str != '<')&&(size > 0)) 					// поиск вхождения(проверка скобок)
	{																					// от начала к концу
		str++;
		size--;
	}
	uint8_t tempSize = size;
	while(tempSize > 0)												// от конца к началу
	{	
		if(str[tempSize-1] == '>')	break;
		tempSize--;
	}
	if((size == 0)||(tempSize == 0)||(tempSize > MAX_RECMESSAGE_LEN))					// если нет хоть какой-нибудь скобки или сообщение слишком длинное - вернуть ошибку
	{
		ret.command = RERR;
		return ret;
	}	
	str++; 	                                  // избавляемся от скобочки(первой)
	
	char* token = strtok(str, " ");						// разбиваем строку на лексемы
  ret.command = commandDefiner(token);
	token = strtok(NULL, " >");							// токен - данные
	uint8_t tempBuffer[2];                  // временный буффер
	convertStringToNumber(token, tempBuffer, 4, LITTLEN);	// переводим в число с записью сразу в память
	ret.data = *(uint16_t*)tempBuffer;	    // записываем в переменную
	return ret;
}
