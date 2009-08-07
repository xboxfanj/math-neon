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
 
float coshf_c(float x)
{
	float a, b, xx;
	xx = -x;
	a = expf_c(x);
	b = expf_c(xx);
	a = a * 0.5f;
	a = a + 0.5f * b;
	return a;
}


float coshf_neon(float x)
{
#ifdef __MATH_NEON
	asm volatile (""
	);
#else
	return coshf_c(x);
#endif
}

