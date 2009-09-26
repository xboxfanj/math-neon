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
math_mat4mul_c(float m0[16], float m1[16], float d[16]){

}


//matrix vector multiplication. d = m * v
void
math_matvec4mul_c(float m[16], float v[4], float d[4])
{
	d[0] = m[0]*v[0] + m[4]*v[1] + m[8]*v[2] + m[12]*v[3];
	d[1] = m[1]*v[0] + m[5]*v[1] + m[9]*v[2] + m[13]*v[3];
	d[2] = m[2]*v[0] + m[6]*v[1] + m[10]*v[2] + m[14]*v[3];
	d[3] = m[3]*v[0] + m[7]*v[1] + m[11]*v[2] + m[15]*v[3];
}


void
math_matvec4mul_neon(float m[16], float v[4], float d[4])
{
#ifdef __MATH_NEON
	asm volatile (
	"vld1.32 		{d0, d1}, [%1]			\n\t"	//Q0 = v
	"vld1.32 		{d18, d19}, [%0]!		\n\t"	//Q1 = m
	"vld1.32 		{d20, d21}, [%0]!		\n\t"	//Q2 = m+4
	"vld1.32 		{d22, d23}, [%0]!		\n\t"	//Q3 = m+8
	"vld1.32 		{d24, d25}, [%0]!		\n\t"	//Q4 = m+12	
	
	"vmul.f32 		q13, q9, d0[0]			\n\t"	//Q5 = Q1*Q0[0]
	"vmla.f32 		q13, q10, d0[1]			\n\t"	//Q5 += Q1*Q0[1] 
	"vmla.f32 		q13, q11, d1[0]			\n\t"	//Q5 += Q2*Q0[2] 
	"vmla.f32 		q13, q12, d1[1]			\n\t"	//Q5 += Q3*Q0[3]
	
	"vst1.32 		{d26, d27}, [%2] 		\n\t"	//Q4 = m+12	
	: 
	: "r"(m), "r"(v), "r"(d) 
    : "q0", "q9", "q10","q11", "q12", "q13"
	);	
#else
	math_matvec4mul_c(m, v, d);
#endif
}





