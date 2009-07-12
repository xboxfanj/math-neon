/*
math-neon:  ARM Neon VFP Optimised Math Library based on cmath
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

const float __logf_range =  0.693147180f;
const float __logf_invrange = 1.4426950f;

const float __logf_lut[4] = {
	0.14154254546192570,	//p3
	2.30888151926361070,	//p1
	-0.8681945310048997,	//p2
	-1.5822295331515284 	//p0
};

float logf_c(float x)
{
	float a, b;
	int m;
	
	union {
		float   f;
		int 	i;
	} r;
			
	r.f = x;
	m = (r.i >> 23) - 127;
	r.i = (r.i & 0x007FFFFF) | 0x3F800000; // 127 << 23
	
	//Taylor Polynomial (Estrins)
	a = (__logf_lut[0] * r.f) + (__logf_lut[2]);
	b = (__logf_lut[1] * r.f) + (__logf_lut[3]);
	r.f = r.f * r.f;
	r.f = b + a * r.f;
	r.f += ((float) m) * __logf_range;

	return r.f;
}

float logf_neon(float x)
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
