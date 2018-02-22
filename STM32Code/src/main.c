#include "CMSIS/stm32f10x.h"
#include "Filters/MajvikFilter.h"

float yaw, pitch, roll;

int main(void)       		   
{											
	MajvikFilter(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.01f);
	getEulerAngle(&yaw, &pitch, &roll);
	while(1)
	{
		;		
	}
	

}
