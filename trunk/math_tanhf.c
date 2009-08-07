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
 
float tanhf_c(float x)
{
	float a, b;
	int m;
	union{
		float 	f;
		int 	i;
	} xx;
	
	x = 2.0f * x;
	a = expf_c(x);
	a = a + 1.0f;
		
	//reciporical approx.
	xx.f = a;
	m = 0x3F800000 - (xx.i & 0x7F800000);
	xx.i = xx.i + m;
	xx.f = 1.41176471f - 0.47058824f * xx.f;
	xx.i = xx.i + m;
	b = 2.0 - xx.f * a;
	xx.f = xx.f * b;	
	b = 2.0 - xx.f * a;
	xx.f = xx.f * b;

	xx.f = 2.0f * xx.f;
	a = 1.0;
	a = a - xx.f;
	return a;
}


float tanhf_neon(float x)
{
#ifdef __MATH_NEON
	asm volatile (""
	);
#else
	return tanhf_c(x);
#endif
}

