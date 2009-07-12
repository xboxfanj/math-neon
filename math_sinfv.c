/*
math-neon:  ARM Neon VFP Optimised Math Library based on cmath
Contact:    lachlan.ts@gmail.com
Copyright (C) 2009  Lachlan Tychsen - Smith aka Adventus

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "math.h"
#include "math_neon.h"

const float __sinfv_rng[2] = {
	2.0 / M_PI,
	M_PI / 2.0, 
};

const float __sinfv_lut[4] = {
	-0.00018365f,	//p7
	-0.16664831f,	//p3
	+0.00830636f,	//p5
	+0.99999661f,	//p1
};

void sinfv_c(float *x, int num)
{
	union {
		float 	f;
		int 	i;
	} ax;
	
	float a, b, xx;
	int m, n;
	
	int ii = num & 0x1;
	int i = num ^ ii;
	
	while(i > 0){
		
		ax.f = fabsf(x[i]);

		//Range Reduction:
		m = (int) (ax.f * __sinfv_rng[0]);	
		ax.f = ax.f - (((float)m) * __sinfv_rng[1]);

		//Test Quadrant
		n = m & 1;
		ax.f = ax.f - n * __sinfv_rng[1];
		m = (m&2) >> 1;
		ax.i = ax.i ^ ((n ^ m ^ (x[i] < 0)) << 31);
			
		//Taylor Polynomial (Estrins)
		xx = ax.f * ax.f;	
		a = (__sinfv_lut[0] * ax.f) * xx + (__sinfv_lut[2] * ax.f);
		b = (__sinfv_lut[1] * ax.f) * xx + (__sinfv_lut[3] * ax.f);
		xx = xx * xx;
		x[i] = b + a * xx;
		i -= 1;
		
		ax.f = fabsf(x[i]);

		//Range Reduction:
		m = (int) (ax.f * __sinfv_rng[0]);	
		ax.f = ax.f - (((float)m) * __sinfv_rng[1]);

		//Test Quadrant
		n = m & 1;
		ax.f = ax.f - n * __sinfv_rng[1];
		m = (m&2) >> 1;
		ax.i = ax.i ^ ((n ^ m ^ (x[i] < 0)) << 31);
			
		//Taylor Polynomial (Estrins)
		xx = ax.f * ax.f;	
		a = (__sinfv_lut[0] * ax.f) * xx + (__sinfv_lut[2] * ax.f);
		b = (__sinfv_lut[1] * ax.f) * xx + (__sinfv_lut[3] * ax.f);
		xx = xx * xx;
		x[i] = b + a * xx;
		i -= 1;
	}
	
	if (ii) {
		x[num - 1] = sinf_c(x[num-1]);
	}
	
	
	return x;
}

void sinfv_neon(float *x, int n)
{
#ifdef __MATH_NEON
	volatile asm (
	);
	return x;
#else
	return sinfv_c(x, n);
#endif
}
