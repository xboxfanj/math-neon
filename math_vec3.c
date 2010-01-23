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


#include "math_neon.h"

//vec4 scalar product
float 
dot3_c(float v0[3], float v1[3])
{
	float r;
	r = v0[0]*v1[0];
	r += v0[1]*v1[1];
	r += v0[2]*v1[2]; 
	return r;
}

void
cross3_c(float v0[3], float v1[3], float d[3])
{
	d[0] = v0[1]*v1[2] - v0[2]*v1[1];
	d[1] = v0[2]*v1[0] - v0[0]*v1[2];
	d[2] = v0[0]*v1[1] - v0[1]*v1[0];
}

void 
normalize3_c(float v[3], float d[3])
{
	float b, c, x;
	union {
		float 	f;
		int 	i;
	} a;
	
	x = v[0]*v[0];
	x += v[1]*v[1];
	x += v[2]*v[2];

	//fast invsqrt approx
	a.f = x;
	a.i = 0x5F3759DF - (a.i >> 1);		//VRSQRTE
	c = x * a.f;
	b = (3.0f - c * a.f) * 0.5;		//VRSQRTS
	a.f = a.f * b;		
	c = x * a.f;
	b = (3.0f - c * a.f) * 0.5;
    a.f = a.f * b;	

	d[0] = v[0]*a.f;
	d[1] = v[1]*a.f;
	d[2] = v[2]*a.f;
}


float 
dot3_neon_hfp(float v0[3], float v1[3])
{
#ifdef __MATH_NEON
	asm volatile (
	"vld1.32 		{d2}, [%0]			\n\t"	//d2={x0,y0}
	"flds 			s6, [%0, #8]		\n\t"	//d3[0]={z0}
	"vld1.32 		{d4}, [%1]			\n\t"	//d4={x1,y1}
	"flds 			s10, [%1, #8]	\n\t"	//d5[0]={z1}

	"vmul.f32 		d0, d2, d4			\n\t"	//d0= d2*d4
	"vpadd.f32 		d0, d0, d0			\n\t"	//d0 = d[0] + d[1]
	"vmla.f32 		d0, d3, d5			\n\t"	//d0 = d0 + d3*d5 
	:: "r"(v0), "r"(v1) 
    : 
	);	
#endif
}

float 
dot3_neon_sfp(float v0[3], float v1[3])
{
#ifdef __MATH_NEON
	dot3_neon_hfp(v0, v1);
	asm volatile ("vmov.f32 r0, s0 		\n\t");
#else
	return dot3_c(v0, v1);
#endif
};


float 
cross3_neon(float v0[3], float v1[3], float d[3])
{
#ifdef __MATH_NEON
	asm volatile (
	"flds 			s4, [%0]			\n\t"	//d2[0]={x0}
	"vld1.32 		{d0}, [%0, #4]		\n\t"	//d0={y0,z0}
	"vmov.f32 		s3, s4		 		\n\t"	//d1[1]={x0}
	"vmov.f32 		s2, s1		 		\n\t"	//d1[0]={z0}
	"vmov.f32 		s5, s0		 		\n\t"	//d2[1]={y0}

	"flds 			s6, [%1, #8]		\n\t"	//d3[0]={z1}
	"flds 			s8, [%1]			\n\t"	//d4[0]={x1}
	"flds 			s10, [%1, #4]		\n\t"	//d5[0]={y1}
	"fnegs	 		s11, s6				\n\t"	//d5[1]=-d3[0]
	"fnegs 			s9, s8				\n\t"	//d4[1]=-d4[0]
	"fnegs 			s7, s10				\n\t"	//d3[1]=-d5[0]
	
	"vmul.f32 		d16, d0, d3			\n\t"	//d16=d0*d3
	"vmul.f32 		d17, d1, d4			\n\t"	//d17=d1*d4
	"vmul.f32 		d18, d2, d5			\n\t"	//d18=d2*d5
	"vpadd.f32 		d0, d16, d17		\n\t"	//d0[0]=d16[0]+d16[1], d0[1]=d17[0]+d17[1], 
	"vpadd.f32 		d1, d18				\n\t"	//d2[0]=d18[0]+d18[1]
	
	"vst1.32 		d0, [%2]			\n\t"	//
	"fsts 			s2, [%2, #8]		\n\t"	//
	
	:: "r"(v0), "r"(v1), "r"(d)
    : 
	);	
#else
	cross3_c(v0,v1,d);
#endif
}

void 
normalize3_neon(float v[3], float d[3])
{
#ifdef __MATH_NEON
	asm volatile (
	"vld1.32 		{d4}, [%0]				\n\t"	//d4={x0,y0}
	"flds 			s10, [%0, #8]			\n\t"	//d5[0]={z0}

	"vmul.f32 		d0, d4, d4				\n\t"	//d0= d4*d4
	"vpadd.f32 		d0, d0					\n\t"	//d0 = d[0] + d[1]
	"vmla.f32 		d0, d5, d5				\n\t"	//d0 = d0 + d5*d5 
	
	"vmov.f32 		d1, d0					\n\t"	//d1 = d0
	"vrsqrte.f32 	d0, d0					\n\t"	//d0 = ~ 1.0 / sqrt(d0)
	"vmul.f32 		d2, d0, d1				\n\t"	//d2 = d0 * d1
	"vrsqrts.f32 	d3, d2, d0				\n\t"	//d3 = (3 - d0 * d2) / 2 	
	"vmul.f32 		d0, d0, d3				\n\t"	//d0 = d0 * d3
	"vmul.f32 		d2, d0, d1				\n\t"	//d2 = d0 * d1	
	"vrsqrts.f32 	d3, d2, d0				\n\t"	//d4 = (3 - d0 * d3) / 2	
	"vmul.f32 		d0, d0, d3				\n\t"	//d0 = d0 * d4	

	"vmul.f32 		q2, q2, d0[0]			\n\t"	//d0= d2*d4
	"vst1.32 		{d4}, [%1]				\n\t"	//
	"fsts 			s10, [%1, #8]			\n\t"	//
	
	:: "r"(v), "r"(d) 
    : "d0", "d1", "d2", "d3", "d4", "d5"
	);	
#else
	normalize3_c(v, d);
#endif

}


