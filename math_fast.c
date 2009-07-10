
#include "math.h"
#include <stdio.h>

const float __log2f_A = 1.0f / 8388608.0f;
const float __log2f_B = 127.0f;

float log2f_fast(const float x){
	
	union {
		float f;
		int i;
	} sx;
	
	sx.f = fabsf(x);	
	sx.f = ((float) sx.i) * __log2f_A - __log2f_B;
	
	return sx.f;
}

const float __logf_A = 0.69314718055f;

float logf_fast(const float x){
	return log2f_fast(x) * __logf_A;
}

const float __expf_A = 8388608 / M_LN2;
const float __expf_B = 1065353216 - 366393;

float expf_fast(const float x){

	union {
		float f;
		int i;
	} sx;

	sx.f = x;
	sx.i = (__expf_A * x + __expf_B);
	return sx.f;
}

const double __expd_A = 1048576 / M_LN2;
const double __expd_B = 1072693248 - 60801;

double expd_fast(const double x){

	union {
		double d;
		struct {
			int i, j;
		} n;
	} sx;

	sx.d = x;
	sx.n.j = __expd_A * x + __expd_B;
	return sx.d;
}

float powf_fast(const float x, const float y){
	return expf_fast(y * logf_fast(x));
}
