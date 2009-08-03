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

//Floating Point value ABI 0=softfp, 1=hardfp. Only effects ASM routines.
#define __MATH_FPABI 	1	

#endif

#ifdef GCC
#define ALIGN(A) __attribute__ ((aligned (A))
#else
#define ALIGN(A)
#endif

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
void		sincosf_c(float r[2], float x);
void		sincosf_neon(float r[2], float x);

/* 
function:	sinfv
return: 	the sine function evaluated at x[i] radians 	
expression: r[i] = sin(x[i])	
notes:		faster than evaluating individually.
			r and x can be the same memory location.
*/
void		sinfv_c(float *r, float *x, int n);
void  		sinfv_neon( float *r, float *x, int n);

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
notes:		assumes |x| >= 0
*/
float 		logf_c(float x);
float 		logf_neon(float x);

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

float _frexpf(const float x);
float _log10f(const float x);
float _sinhf(const float x);
float _tanhf(const float x);
float _acosf(const float x);
float _asinf(const float x);
float _atan2f(const float x);
float _coshf(const float x);

#endif
