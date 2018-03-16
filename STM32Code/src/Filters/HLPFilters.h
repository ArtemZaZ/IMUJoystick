/* Фильтры LP и HP */
#ifndef HLPFILTERS_H_
#define HLPFILTERS_H_

#define RC 0.9f
#define KP 0.8f

// параметр oldFilteredVal является выходом по совместительству
void HPFilterIterator(float rawIn, float oldrawIn, float* oldFilteredVal, float dt);
void LPFilterIterator(float rawIn, float* oldFilteredVal);
#endif /* HLPFILTERS_H_ */

