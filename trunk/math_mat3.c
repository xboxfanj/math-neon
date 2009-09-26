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
Matrices are specified in row major format:

| x0 x2 |
| x1 x3 |

therefore m[2] = x2

*/

#include "math_neon.h"

//matrix matrix multipication. d = m0 * m1;
void
matmul3_c(float m0[9], float m1[9], float d[9])
{
	d[0] = m0[0]*m1[0] + m0[3]*m1[1] + m0[6]*m1[2];
	d[1] = m0[1]*m1[0] + m0[4]*m1[1] + m0[7]*m1[2];
	d[2] = m0[2]*m1[0] + m0[5]*m1[1] + m0[8]*m1[2];
	d[3] = m0[0]*m1[3] + m0[3]*m1[4] + m0[6]*m1[5];
	d[4] = m0[1]*m1[3] + m0[4]*m1[4] + m0[7]*m1[5];
	d[5] = m0[2]*m1[3] + m0[5]*m1[4] + m0[8]*m1[5];
	d[6] = m0[0]*m1[6] + m0[3]*m1[7] + m0[6]*m1[8];
	d[7] = m0[1]*m1[6] + m0[4]*m1[7] + m0[7]*m1[8];
	d[8] = m0[2]*m1[6] + m0[5]*m1[7] + m0[8]*m1[8];
}


//matrix vector multiplication. d = m * v
void
matvec3_c(float m[9], float v[3], float d[3])
{
	d[0] = m[0]*v[0] + m[3]*v[1] + m[6]*v[2];
	d[1] = m[1]*v[0] + m[4]*v[1] + m[7]*v[2];
	d[2] = m[2]*v[0] + m[5]*v[1] + m[8]*v[2];
}

void
matvec3_neon(float m[9], float v[3], float d[3])
{
#ifdef __MATH_NEON
	int tmp;
	asm volatile (
	"mov 			%3, #3					\n\t"	//r3 = 3
	"vld1.32 		{d0, d1}, [%1]			\n\t"	//Q0 = v
	"vld1.32 		{d18, d19}, [%0], %3	\n\t"	//Q1 = m
	"vld1.32 		{d20, d21}, [%0], %3	\n\t"	//Q2 = m+3
	"vld1.32 		{d22, d23}, [%0], %3	\n\t"	//Q3 = m+6
	
	"vmul.f32 		q12, q9, d0[0]			\n\t"	//Q5 = Q1*Q0[0]
	"vmla.f32 		q12, q10, d0[1]			\n\t"	//Q5 += Q1*Q0[1] 
	"vmla.f32 		q12, q11, d1[0]			\n\t"	//Q5 += Q2*Q0[2] 
	
	"vst1.32 		d24, [%2]! 				\n\t"	//r2 = D24	
	"vst1.32 		d25[0], [%2]  			\n\t"	//r2 = D25[0]	
	: 
	: "r"(m), "r"(v), "r"(d), "r"(tmp)
    : "q0", "q9", "q10","q11", "q12", "q13"
	);	
#else
	matvec3_c(m, v, d);
#endif
}