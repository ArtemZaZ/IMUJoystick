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

void MajvikFilter(float ax, float ay, float az, float wx, float wy, float wz, float time)
{	
	float norm;	// временная переменная для вычисления нормы
	float qHatDotw, qHatDotx, qHatDoty, qHatDotz; // составляющие квартерниона компенсации
	float qDotOmegaw, qDotOmegax, qDotOmegay, qDotOmegaz;	// составляющее квартерниона скорости изменения ориентации
	//float f1, f2, f3; // составляющие целевой ф-и. !подр(https://habrahabr.ru/post/255661/)
	//float _2qxqx, _2qyqy, _2qw, _2qx, _2qy, _2qz, _4qx, _4qy;		// комбинированные компоненты - оптимизация
	float temp1, temp2; 	// переменные оптимизации
	if(ax && ay && az) 
	{
		norm = fastInvSqrt(ax*ax + ay*ay + az*az);
		ax = ax*norm;
		ay = ay*norm;
		az = az*norm;
		/*
		_2qxqx = 2*qx*qx;
		_2qyqy = 2*qy*qy;
		_2qw = 2*qw;
		_2qx = 2*qx;
		_2qy = 2*qy;
		_2qz = 2*qz;
		_4qx = 4*qx;
		_4qy = 4*qy;		
		
		qHatDotw = 
		qHatDotw = _2qw *_2qyqy + _2qy*ax + _2qw *_2qxqx - _2qx*ay;
		qHatDotx = _4qx*qz*qz + 4*qw*qw*qx - 4*qx + 2*
		*/
		temp1 = qx*qx + qy*qy;												//magic variable
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
		
		qw += (qDotOmegaw - BETA*qHatDotw)*time;
		qx += (qDotOmegax - BETA*qHatDotx)*time;
		qy += (qDotOmegay - BETA*qHatDoty)*time;
		qz += (qDotOmegaz - BETA*qHatDotz)*time;
		
		norm = fastInvSqrt(qw*qw + qx*qx + qy*qy + qz*qz);
		qw *= norm;
		qx *= norm;
		qy *= norm;
		qz *= norm;		
	}      	
}

void getEulerAngle(float* yaw, float* pitch, float* roll)
{
	*yaw = atan2(2.f*(qw*qz + qx*qy), qx*qx + qw*qw - qz*qz - qy*qy);
	*pitch = asin(2.f*(qx*qz - qw*qy));
	*roll = atan2(2.f*(qw*qx + qy*qz), qz*qz - qy*qy - qx*qx + qw*qw);
}
