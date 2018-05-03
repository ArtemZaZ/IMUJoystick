#include "MajvikFilter.h"

static inline float fastInvSqrt(float x)
{
	float xhalf = 0.5f * x;
	uint32_t i = *(uint32_t*)&x;
	i = 0x5F3759DF - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - xhalf*x*x);
	return x;
}               

void MajvikFilter(float ax, float ay, float az, float wx, float wy, float wz, float dt)
{	
	float norm;	// временная переменная для вычисления нормы
	float qHatDotw, qHatDotx, qHatDoty, qHatDotz; // составляющие квартерниона компенсации
	float qDotOmegaw, qDotOmegax, qDotOmegay, qDotOmegaz;	// составляющее квартерниона скорости изменения ориентации
	float temp1, temp2; 	// переменные оптимизации
	if(ax && ay && az) 
	{
		norm = fastInvSqrt(ax*ax + ay*ay + az*az);
		ax = ax*norm;
		ay = ay*norm;
		az = az*norm;

		temp1 = qx*qx + qy*qy;		//magic variable
		temp2 = qz*qz + qw*qw - 1.f + 2.f*temp1 + az;
		
		qHatDotw = 4.f*qw*temp1 + 2.f*(qy*ax - qx*ay);	// проверить
		qHatDotx = 4.f*qx*temp2 - 2.f*(qz*ax + qw*ay);
		qHatDoty = 4.f*qy*temp2 + 2.f*(qw*ax - qz*ay);
		qHatDotz = 4.f*qz*temp1 - 2.f*(qx*ax + qy*ay);
		
		norm = fastInvSqrt(qHatDotw*qHatDotw + qHatDotx*qHatDotx + qHatDoty*qHatDoty + qHatDotz*qHatDotz);
		qHatDotw *= norm;
		qHatDotx *= norm;
		qHatDoty *= norm;
		qHatDotz *= norm;
		
		qDotOmegaw = -0.5f*(qx*wx + qy*wy + qz*wz);  // снова проверить
		qDotOmegax = 0.5f*(qw*wx + qy*wz - qz*wy);
		qDotOmegay = 0.5f*(qw*wy - qx*wz + qz*wx);
		qDotOmegaz = 0.5f*(qw*wz + qx*wy - qy*wx);
		
		qw += (qDotOmegaw - BETA*qHatDotw)*dt;
		qx += (qDotOmegax - BETA*qHatDotx)*dt;
		qy += (qDotOmegay - BETA*qHatDoty)*dt;
		qz += (qDotOmegaz - BETA*qHatDotz)*dt;
		
		norm = fastInvSqrt(qw*qw + qx*qx + qy*qy + qz*qz);
		qw *= norm;
		qx *= norm;
		qy *= norm;
		qz *= norm;		
	}      	
}

void getEulerAngle(float* yaw, float* pitch, float* roll)
{
	*yaw = atan2f(2.f*(qw*qz + qx*qy), qx*qx + qw*qw - qz*qz - qy*qy)*RAD_TO_DEGREE;
	*pitch = asinf(2.f*(qx*qz - qw*qy))*RAD_TO_DEGREE;
	*roll = atan2f(2.f*(qw*qx + qy*qz), qz*qz - qy*qy - qx*qx + qw*qw)*RAD_TO_DEGREE;
}

void resetMajvikFilter(void)
{
  qw = 1.f;
  qx = 0.f;
  qy = 0.f;
  qz = 0.f;
}
