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
Based on x ^ n = exp(n * log(x))

Test func : powf(x, n)
Test Range: (1,1) < (x, n) < (10, 10)
Peak Error:	~0.0010%
RMS  Error: ~0.0002%
*/

#include "math.h"
#include "math_neon.h"

const float __powf_rng[2] = {
	1.442695041f,
	0.693147180f
};

const float __powf_lut[16] = {
	-2.295614848256274, 	//p0	log
	-2.470711633419806, 	//p4
	-5.686926051100417, 	//p2
	-0.165253547131978, 	//p6
	+5.175912446351073, 	//p1
	+0.844006986174912, 	//p5
	+4.584458825456749, 	//p3
	+0.014127821926000,		//p7
	0.9999999916728642,		//p0	exp
	0.04165989275009526, 	//p4
	0.5000006143673624, 	//p2
	0.0014122663401803872, 	//p6
	1.000000059694879, 		//p1
	0.008336936973260111, 	//p5
	0.16666570253074878, 	//p3
	0.00019578093328483123	//p7
};

float powf_c(float x, float n)
{
	float a, b, c, d, xx;
	int m;
	
	union {
		float   f;
		int 	i;
	} r;
	
	//extract exponent
	r.f = x;
	m = (r.i - 0x3F800000) >> 23;
	r.i = (r.i & 0x007FFFFF) | 0x3F800000;
	
	//Taylor Polynomial (Estrins)
	xx = r.f * r.f;
	a = (__powf_lut[4] * r.f) + (__powf_lut[0]);
	b = (__powf_lut[6] * r.f) + (__powf_lut[2]);
	c = (__powf_lut[5] * r.f) + (__powf_lut[1]);
	d = (__powf_lut[7] * r.f) + (__powf_lut[3]);
	a = a + b * xx;
	c = c + d * xx;
	xx = xx * xx;
	r.f = a + c * xx;

	//add exponent
	r.f = r.f + ((float) m) * __powf_rng[1];

	r.f = r.f * n;


	//Range Reduction:
	m = (int) (r.f * __powf_rng[0]);
	r.f = r.f - ((float) m) * __powf_rng[1];	
	
	//Taylor Polynomial (Estrins)
	a = (__powf_lut[12] * r.f) + (__powf_lut[8]);
	b = (__powf_lut[14] * r.f) + (__powf_lut[10]);
	c = (__powf_lut[13] * r.f) + (__powf_lut[9]);
	d = (__powf_lut[15] * r.f) + (__powf_lut[11]);
	xx = r.f * r.f;
	a = a + b * xx; 
	c = c + d * xx;
	xx = xx* xx;
	r.f = a + c * xx; 
	
	//multiply by 2 ^ m 
	m = m << 23;
	r.i = r.i + m;

	return r.f;
}

float powf_neon(float x)
{
#ifdef __MATH_NEON
	float r;
	asm volatile (""
	);
	return r;
#else
	return expf_c(x);
#endif
}
