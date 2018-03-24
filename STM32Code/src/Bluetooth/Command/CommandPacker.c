#include "CommandPacker.h"

static uint8_t convertNumberToSymbol(uint8_t num)    //num only 0..15
{
	if(num < 10) return (uint8_t)'0' + num;
	if(num < 15) return (uint8_t)'A' + num - 10;
	return 0;
}

static void convertNumberToString(uint8_t* ret, uint16_t num)
{
	ret[0] = convertNumberToSymbol((uint8_t)(num >> 12));
	ret[1] = convertNumberToSymbol((uint8_t)(0x0F & (num >> 8)));
	ret[2] = convertNumberToSymbol((uint8_t)(0x0F & (num >> 4)));
	ret[3] = convertNumberToSymbol((uint8_t)(0x0F & num));
}

SendData packing(SendCommand command, uint16_t data)
{
	SendData send;
	send.message[0] = (uint8_t)'<';
	switch(command)
	{
		case SSTART:
			strcpy((char*)(send.message + 1), "START ");
			convertNumberToString(send.message + 7, data);	// записываем сразу в структуру без доп переменных, !!! еси что-то менять, нужно менять и смещение	
      send.message[11] = (uint8_t)'>';
			send.size = 12;
			break;
		
		case SSTOP: 
			strcpy((char*)(send.message + 1), "STOP ");
			convertNumberToString(send.message + 6, data);	// записываем сразу в структуру без доп переменных, !!! еси что-то менять, нужно менять и смещение			
			send.message[10] = (uint8_t)'>';
			send.size = 11;
			break;
		
		case SPRD: 
			strcpy((char*)(send.message + 1), "PRD ");
			convertNumberToString(send.message + 5, data);	// записываем сразу в структуру без доп переменных, !!! еси что-то менять, нужно менять и смещение			
			send.message[9] = (uint8_t)'>';
			send.size = 10;
			break;
		
		case SBUT:
			strcpy((char*)(send.message + 1), "BUT ");
			convertNumberToString(send.message + 5, data);	// записываем сразу в структуру без доп переменных, !!! еси что-то менять, нужно менять и смещение
			send.message[9] = (uint8_t)'>';
			send.size = 10;
			break;
		
		default:
			strcpy((char*)(send.message + 1), "ERROR ");
			convertNumberToString(send.message + 7, data);	// записываем сразу в структуру без доп переменных, !!! еси что-то менять, нужно менять и смещение			
			send.message[11] = (uint8_t)'>';
			send.size = 12;
			break;   	
	}
	return send;
}

