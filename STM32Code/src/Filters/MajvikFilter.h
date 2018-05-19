/* Фильтр Маджвика */
#ifndef MAJVIKFILER_H_
#define MAJVIKFILER_H_
#include <stdint.h>
#include <math.h>

#define PI 3.1415926f
#define RAD_TO_DEGREE (180.f / PI)
#define DEGREE_TO_RAD (PI / 180.f)

static inline float fastInvSqrt(float x);		// ф-ия извлечения быстрого обратного корня
void MajvikFilter(float ax, float ay, float az, float wx, float wy, float wz, float time);	// Фильтр Маджвика
void getEulerAngle(float* yaw, float* pitch, float* roll);  // получение углов Эйлера из квартерниона
void resetMajvikFilter(void);
#endif /* MAJVIKFILER_H_ */

