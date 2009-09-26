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

#ifndef __MATH_NEON_H__ 
#define __MATH_NEON_H__ 

#if !defined(__i386__) && defined(__arm__)
//if defined neon ASM routines are used, otherwise all calls to *_neon 
//functions are rerouted to their equivalent *_c function.
#define __MATH_NEON			

//Floating Point value ABI 0=softfp, 1=hardfp. Only effects *_neon routines.
#define __MATH_FPABI 	0	

#endif

#ifdef GCC
#define ALIGN(A) __attribute__ ((aligned (A))
#else
#define ALIGN(A)
#endif

#ifndef _MATH_H
#define M_PI		3.14159265358979323846	/* pi */
#define M_PI_2		1.57079632679489661923	/* pi/2 */
#define M_PI_4		0.78539816339744830962	/* pi/4 */
#define M_E			2.7182818284590452354	/* e */
#define M_LOG2E		1.4426950408889634074	/* log_2 e */
#define M_LOG10E	0.43429448190325182765	/* log_10 e */
#define M_LN2		0.69314718055994530942	/* log_e 2 */
#define M_LN10		2.30258509299404568402	/* log_e 10 */
#define M_1_PI		0.31830988618379067154	/* 1/pi */
#define M_2_PI		0.63661977236758134308	/* 2/pi */
#define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */
#endif 

/* 
function:	enable_runfast
			this function enables the floating point runfast mode on the 
			ARM Cortex A8.  	
*/
void		enable_runfast();

/* 
function:	matmul2
arguments:  m0 2x2 matrix, m1 2x2 matrix
return: 	d 2x2 matrix
expression: d = m0 * m1
*/
void		matmul2_c(float m0[4], float m1[4], float d[4]);
void		matmul2_neon(float m0[4], float m1[4], float d[4]);

/* 
function:	matmul3
arguments:  m0 3x3 matrix, m1 3x3 matrix
return: 	d 3x3 matrix
expression: d = m0 * m1
*/
void		matmul3_c(float m0[9], float m1[9], float d[9]);
void		matmul3_neon(float m0[9], float m1[9], float d[9]);

/* 
function:	matmul4
arguments:  m0 4x4 matrix, m1 4x4 matrix
return: 	d 4x4 matrix
expression: d = m0 * m1
*/
void		matmul4_c(float m0[16], float m1[16], float d[16]);
void		matmul4_neon(float m0[16], float m1[16], float d[16]);
\
/* 
function:	matvec2
arguments:  m 2x2 matrix, v 2 element vector
return: 	d 2x2 matrix
expression: d = m * v
*/
void		matvec2_c(float m[4], float v[2], float d[2]);
void		matvec2_neon(float m[4], float v[2], float d[2]);

/* 
function:	matvec3
arguments:  m 3x3 matrix, v 3 element vector
return: 	d 3x3 matrix
expression: d = m * v
*/
void		matvec3_c(float m[9], float v[3], float d[3]);
void		matvec3_neon(float m[9], float v[3], float d[3]);

/* 
function:	matvec4
arguments:  m 4x4 matrix, v 4 element vector
return: 	d 4x4 matrix
expression: d = m * v
*/
void		matvec4_c(float m[16], float v[4], float d[4]);
void		matvec4_neon(float m[16], float v[4], float d[4]);

/* 
function:	sinf
arguments:  x radians
return: 	the sine function evaluated at x radians.	
expression: r = sin(x) 	
*/
float 		sinf_c(float x);
float 		sinf_neon(float x);

/* 
function:	cosf
arguments:  x radians
return: 	the cosine function evaluated at x radians.	
expression: r = cos(x) 	
notes:		computed using cos(x) = sin(x + pi/2)
*/
float 		cosf_c(float x);
float 		cosf_neon(float x);

/* 
function:	sincosf
arguments:  x radians, r[2] result array.
return: 	both the sine and the cosine evaluated at x radians.	
expression: r = {sin(x), cos(x)} 	
notes:		faster than evaluating seperately.
*/
void		sincosf_c(float x, float r[2]);
void		sincosf_neon(float x, float r[2]);

/* 
function:	sinfv
return: 	the sine function evaluated at x[i] radians 	
expression: r[i] = sin(x[i])	
notes:		faster than evaluating individually.
			r and x can be the same memory location.
*/
void		sinfv_c(float *x, int n, float *r);
void  		sinfv_neon(float *x, int n, float *r);

/* 
function:	tanf
return: 	the tangent evaluated at x radians.	
expression: r = tan(x) 	
notes:		computed using tan(x) = sin(x) / cos(x)
*/
float 		tanf_c(float x);
float 		tanf_neon(float x);

/* 
function:	atanf
return: 	the arctangent evaluated at x.	
expression: r = atan(x) 	
*/
float 		atanf_c(float x);
float 		atanf_neon(float x);

/* 
function:	atanf
return: 	the arctangent evaluated at x.	
expression: r = atan(x) 	
*/
float 		atan2f_c(float y, float x);
float 		atan2f_neon(float y, float x);

/* 
function:	asinf
return: 	the arcsine evaluated at x.	
expression: r = asin(x) 	
*/
float 		asinf_c(float x);
float 		asinf_neon(float x);

/* 
function:	acosf
return: 	the arcsine evaluated at x.	
expression: r = asin(x) 	
*/
float 		acosf_c(float x);
float 		acosf_neon(float x);

/* 
function:	sinhf
return: 	the arcsine evaluated at x.	
expression: r = asin(x) 	
*/
float 		sinhf_c(float x);
float 		sinhf_neon(float x);

/* 
function:	coshf
return: 	the arcsine evaluated at x.	
expression: r = asin(x) 	
*/
float 		coshf_c(float x);
float 		coshf_neon(float x);

/* 
function:	tanhf
return: 	the arcsine evaluated at x.	
expression: r = asin(x) 	
*/
float 		tanhf_c(float x);
float 		tanhf_neon(float x);

/* 
function:	expf
return: 	the natural exponential evaluated at x.	
expression: r = e ** x	
*/
float 		expf_c(float x);
float 		expf_neon(float x);

/* 
function:	logf
return: 	the value of the natural logarithm of x.	
expression: r = ln(x)	
notes:		assumes x > 0
*/
float 		logf_c(float x);
float 		logf_neon(float x);

/* 
function:	log10f
return: 	the value of the power 10 logarithm of x.	
expression: r = log10(x)	
notes:		assumes x > 0
*/
float 		log10f_c(float x);
float 		log10f_neon(float x);

/* 
function:	powf
return: 	x raised to the power of n, x ** n.
expression: r = x ** y	
notes:		computed using e ** (y * ln(x))
*/
float 		powf_c(float x, float n);
float 		powf_neon(float x, float n);

/* 
function:	floorf
return: 	x rounded down (towards negative infinity) to its nearest 
			integer value.	
notes:		assumes |x| < 2 ** 31
*/
float 		floorf_c(float x);
float 		floorf_neon(float x);

/* 
function:	ceilf
return: 	x rounded up (towards positive infinity) to its nearest 
			integer value.	
notes:		assumes |x| < 2 ** 31
*/
float 		ceilf_c(float x);
float 		ceilf_neon(float x);

/* 
function:	fabsf
return: 	absolute vvalue of x	
notes:		assumes |x| < 2 ** 31
*/
float 		fabsf_c(float x);
float 		fabsf_neon(float x);


/* 
function:	ldexpf
return: 	the value of m multiplied by 2 to the power of e. 
expression: r = m * (2 ** e)
*/
float 		ldexpf_c(float m, int e);
float 		ldexpf_neon(float m, int e);

/* 
function:	frexpf
return: 	the exponent and mantissa of x 
*/
float 		frexpf_c(float x, int *e);
float 		frexpf_neon(float x, int *e);

/* 
function:	fmodf
return: 	the remainder of x divided by y, x % y	
expression: r = x - floor(x / y) * y;
notes:		assumes that |x / y| < 2 ** 31 
*/
float 		fmodf_c(float x, float y);
float 		fmodf_neon(float x, float y);

/* 
function:	modf
return: 	breaks x into the integer (i) and fractional part (return)
notes:		assumes that |x| < 2 ** 31 
*/
float 		modf_c(float x, int *i);
float 		modf_neon(float x, int *i);

/* 
function:	modf
return: 	breaks x into the integer (i) and fractional part (return)
notes:		assumes that |x| < 2 ** 31 
*/
float 		sqrtf_c(float x);
float 		sqrtf_neon(float x);


#endif
