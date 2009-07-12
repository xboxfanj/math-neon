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

const float __expf_rng[2] = {
	1.442695041f,
	0.693147180f
};

//const float __expf_range =  0.693147180f;
//const float __expf_invrange = 1.4426950f;

const float __expf_lut[4] = {
	0.1627058233704877f,	//p3
	1.0025726735069924f,	//p1
	0.5193612362562018f,	//p2
	0.9989971156433657f		//p0
};

float expf_c(float x)
{
	float a, b, xx;
	int m;
	
	union {
		float   f;
		int 	i;
	} r;
		
	//Range Reduction:
	m = (int) (x * __expf_rng[0]);
	x = x - ((float) m) * __expf_rng[1];
	
	//Taylor Polynomial (Estrins)
	a = (__expf_lut[0] * x) + (__expf_lut[2]);
	b = (__expf_lut[1] * x) + (__expf_lut[3]);
	xx = x * x;
	r.f = b + a * xx;
	
	printf("x=%f \t r=%f \t e=%f \t dr=%f \n", x, r.f, expf(x), 100 * (r.f - expf(x)) / expf(x) );

	m = m << 23;
	r.i = r.i + m;

	return r.f;
}

float expf_neon(float x)
{
#ifdef __MATH_NEON
	float r;
	volatile asm (
	);
	return r;
#else
	return expf_c(x);
#endif
}
