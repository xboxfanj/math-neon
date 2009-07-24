/*
Math-NEON:  Neon Optimised Math Library based on cmath
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

/*
Based on: 

		log(x) = log((1+m) * (2^n))
		log(x) = n * log(2) + log(1 + m)
		log(1+m) = Poly(1+m)
		
		where Poly(x) is the Minimax approximation of log(x) over the 
		range [1, 2]

Test func : logf(x)
Test Range: 1 < x < 10000
Peak Error:	~0.000601%
RMS  Error: ~0.000005%
*/


#include "math.h"
#include "math_neon.h"


const float __logf_rng =  0.693147180f;

const float __logf_lut[8] = {
	-2.295614848256274, 	//p0
	-2.470711633419806, 	//p4
	-5.686926051100417, 	//p2
	-0.165253547131978, 	//p6
	+5.175912446351073, 	//p1
	+0.844006986174912, 	//p5
	+4.584458825456749, 	//p3
	+0.014127821926000		//p7
};

float logf_c(float x)
{
	float a, b, c, d, xx;
	int m;
	
	union {
		float   f;
		int 	i;
	} r;
	
	//extract exponent
	r.f = x;
	m = (r.i - 0x3F800000) >> 23;
	r.i = (r.i & 0x007FFFFF) | 0x3F800000;
	
	//Taylor Polynomial (Estrins)
	xx = r.f * r.f;
	a = (__logf_lut[4] * r.f) + (__logf_lut[0]);
	b = (__logf_lut[6] * r.f) + (__logf_lut[2]);
	c = (__logf_lut[5] * r.f) + (__logf_lut[1]);
	d = (__logf_lut[7] * r.f) + (__logf_lut[3]);
	a = a + b * xx;
	c = c + d * xx;
	xx = xx * xx;
	r.f = a + c * xx;

	//add exponent
	r.f = r.f + ((float) m) * __logf_rng;

	return r.f;
}

float logf_neon(float x)
{
#ifdef __MATH_NEON
	float 	r;
	int   	tmp0, tmp1;
	asm volatile (
	
	//extract exponent
	"ldr			%1, =0x3F800000			\n\t"	//tmp0 = 0x3F800000
	"sub			%2, %3, %1				\n\t"	//tmp1 = x - tmp0
	"lsr			%2, %2, #23				\n\t"	//tmp1 = tmp1 >> 23
	"vdup.32 		d6, %2					\n\t"	//d6 = {tmp1, tmp1}
	"ldr			%2, =0x007FFFFF			\n\t"	//tmp1 = 0x007FFFFF
	"and			%3, %3, %2				\n\t"	//x = x & tmp1
	"orr			%3, %3, %1				\n\t"	//x = x | tmp0
	"vdup.32 		d0, %3					\n\t"	//d0 = {x, x}

	//polynomial:
	"vmul.f32 		d1, d0, d0				\n\t"	//d1 = d0*d0 = {x^2, x^2}	
	"vld1.32 		{d2, d3, d4, d5}, [%5]	\n\t"	//q1 = {p0, p4, p2, p6}, q2 = {p1, p5, p3, p7} ;
	"vmla.f32 		q1, q2, d0[0]			\n\t"	//q1 = q1 + q2 * d0[0]		
	"vmla.f32 		d2, d3, d1[0]			\n\t"	//d2 = d2 + d3 * d1[0]		
	"vmul.f32 		d1, d1, d1				\n\t"	//d1 = d1 * d1 = {x^4, x^4}	
	"vmla.f32 		d2, d1, d2[1]			\n\t"	//d2 = d2 + d1 * d2[1]		

	//add exponent 	
	"vdup.32 		d7, %4					\n\t"	//d7 = {rng, rng}
	"vcvt.f32.s32 	d6, d6					\n\t"	//d6 = (float) d6
	"vmla.f32 		d2, d6, d7				\n\t"	//d2 = d2 + d6 * d7		

	"vmov.f32 		%0, s4					\n\t"	//r = s4
	
	: "=r"(r), "+r"(tmp0), "+r"(tmp1), "+r"(x)
	: "r"(__logf_rng), "r"(__logf_lut) 
    : "d0", "d1", "q1", "q2", "d6", "d7"
	);
	return r;
#else
	return logf_c(x);
#endif
}
