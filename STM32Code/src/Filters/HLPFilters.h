/* Фильтры LP и HP */
#ifndef HLPFILTERS_H_
#define HLPFILTERS_H_

//#define RC 1.f
//#define KP 0.0001f

// параметр oldFilteredVal является выходом по совместительству
void HPFilterIterator(float rawIn, float oldrawIn, float* oldFilteredVal, float dt, float RC);
void LPFilterIterator(float rawIn, float* oldFilteredVal, float Kp);
#endif /* HLPFILTERS_H_ */

