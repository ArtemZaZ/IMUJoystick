#include "HLPFilters.h"

void HPFilterIterator(float rawIn, float oldrawIn, float* oldFilteredVal, float dt)
{
	float a = RC/(RC + dt);
	*oldFilteredVal = a**oldFilteredVal + a*(rawIn - oldrawIn);
}

void LPFilterIterator(float rawIn, float* oldFilteredVal)
{
	*oldFilteredVal = (KP - 1.f)**oldFilteredVal + KP*rawIn;
}
