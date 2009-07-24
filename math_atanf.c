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

#include "math.h"
#include "math_neon.h"

const float __atanf_lut[4] = {
	-0.0443265554792128,	//p7
	-0.3258083974640975,	//p3
	+0.1555786518463281,	//p5
	+0.9997878412794807  	//p1
}; 
 
const float __atanf_pi_2 = M_PI_2;
    
float atanf_c(float x){

	float a, b, r = 0, xx;
	int m;
	
	union {
		float f;
		int i;
	} xinv, ax;

	//branch on ARM (abs(x) < 1.0):
	ax.f = x;
	ax.i = ax.i & (~0x80000000);
	if (ax.i < 0x3F800000) goto label1;
	
	//fast inverse approximation (2x newton)
	xinv.f = ax.f;
	m = 0x3F800000 - (xinv.i & 0x7F800000);
	xinv.i = xinv.i + m;
	xinv.f = 1.41176471f - 0.47058824f * xinv.f;
	xinv.i = xinv.i + m;
	b = 2.0 - xinv.f * ax.f;
	xinv.f = xinv.f * b;	
	b = 2.0 - xinv.f * ax.f;
	xinv.f = xinv.f * b;
	ax.f = -xinv.f;
	r = M_PI_2;

label1:
	
	//polynomial evaluation
	xx = ax.f * ax.f;	
	a = (__atanf_lut[0] * ax.f) * xx + (__atanf_lut[2] * ax.f);
	b = (__atanf_lut[1] * ax.f) * xx + (__atanf_lut[3] * ax.f);
	xx = xx * xx;
	r = r + a * xx; 
	r = r + b;

	//branch on ARM:
	ax.f = x;
	if (ax.i > 0) goto label2;
	r = -r;

label2:



	return r;
}


float atanf_neon(float x)
{
#ifdef __MATH_NEON
	float 	r;
	int volatile tmp0, tmp1;
	asm volatile (
	
	//Branch 1
	"bic	 		%1, %3, #0x80000000		\n\t"	//tmp0 = x & ~(1 << 31)
	"vdup.32 		d0, %1					\n\t"	//d0 = {ax, ax}
	"ldr	 		%2, =0x3F800000			\n\t"	//tmp1 = 0x3f800000
	"cmp	 		%1, %2					\n\t"	//flags = cmp(tmp0, tmp1)
	"vmov.i32		d5, #0.0				\n\t"	//d5[1] = 0
	"blt	 		1f						\n\t"	//if (lt) goto 1

	//fast reciporical approximation
	"vrecpe.f32		d1, d0					\n\t"	//d1 = ~ 1 / d0; 
	"vrecps.f32		d2, d1, d0				\n\t"	//d2 = 2.0 - d1 * d0; 
	"vmul.f32		d1, d1, d2				\n\t"	//d1 = d1 * d2; 
	"vrecps.f32		d2, d1, d0				\n\t"	//d2 = 2.0 - d1 * d0; 
	"vmul.f32		d1, d1, d2				\n\t"	//d1 = d1 * d2; 
	"vneg.f32		d1, d1					\n\t"	//d1 = -d1; 
	"vmov.f32		s11, %5					\n\t"	//d5[1] = pi / 2; 	

	"1:										\n\t"	//label 1
	
	//polynomial:
	"vmul.f32 		d2, d1, d1				\n\t"	//d2 = d1*d1 = {ax^2, ax^2}	
	"vld1.32 		{d4, d5}, [%4]			\n\t"	//d4 = {p7, p3}, d5 = {p5, p1}
	"vmul.f32 		d3, d2, d2				\n\t"	//d3 = d2*d2 = {x^4, x^4}		
	"vmul.f32 		q0, q2, d1[0]			\n\t"	//q0 = q2 * d1[0] = {p7x, p3x, p5x, p1x}
	"vmla.f32 		d1, d0, d2[0]			\n\t"	//d1 = d1 + d0*d2 = {p5x + p7x^3, p1x + p3x^3}		
	"vmla.f32 		d1, d3, d1[0]			\n\t"	//d1 = d1 + d3*d0 = {..., p1x + p3x^3 + p5x^5 + p7x^7}		
	"vadd.f32 		d1, d1, d5				\n\t"	//d1 = d1 + d5		
	
	"cmp	 		%3, #0					\n\t"	//flags = cmp(x, 0)	
	"bgt	 		2f						\n\t"	//if (gt) goto 2
	"vneg.f32		d1, d1					\n\t"	//d1 = -d1

	"2:										\n\t"	//label 2
		
	"vmov.f32 		%0, s3					\n\t"	//r = d1[1]
	
	: "=r"(r), "+r"(tmp0), "+r"(tmp1)
	: "r"(x), "r"(__atanf_lut),  "r"(__atanf_pi_2) 
    : "d0", "d1", "d2", "d3", "d4", "d5"
	);

	return r;

#else
	return atanf_c(x);
#endif
}
