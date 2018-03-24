#include "HLPFilters.h"

void LPFilterIterator(float rawIn, float oldrawIn, float* oldFilteredVal, float dt, float RC)
{
	float a = RC/(RC + dt);
	*oldFilteredVal = a**oldFilteredVal + a*(rawIn - oldrawIn);
}

void HPFilterIterator(float rawIn, float* oldFilteredVal, float Kp)
{
	*oldFilteredVal = (1.f - Kp)**oldFilteredVal + Kp*rawIn;
}
