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
#include <stdio.h>
#include <math.h>

void enable_runfast()
{
#ifdef __arm__
	static const unsigned int x = 0x04086060;
	static const unsigned int y = 0x03000000;
	int r;
	asm volatile (
		"fmrx	%0, fpscr			\n\t"	//r0 = FPSCR
		"and	%0, %0, %1			\n\t"	//r0 = r0 & 0x04086060
		"orr	%0, %0, %2			\n\t"	//r0 = r0 | 0x03000000
		"fmxr	fpscr, %0			\n\t"	//FPSCR = r0
		: "=r"(r)
		: "r"(x), "r"(y)
	);
#endif
}

int main(int argc, char** argv){
		
	float x, y, p;
	int n = 0;
	float emax = 0;
	float erms = 0;
	float xmax = 0;
	for(x = -1; x < 1; x += 0.01){
		float r;
		float rr;
		float dr;
		float e;
		
		r = tanhf_c(x);
		rr = tanhf(x);
		dr = fabsf(rr - r);
		if (fabs(rr) > 0.0){
			e = (100 * dr) / rr;
			if (e>emax) {
				emax = e;
				xmax = x;
			}
			n++;
			erms += e * e;
		}else{
			e = 0;
		}
		printf("x=%f \t r=%f \t rr=%f \t e=%f \n", x, r, rr, e);
	}
	printf("\n");
	printf("Max Error %f at x = %f \n", emax, xmax);
	printf("RMS Error %f \n", sqrtf(erms / n));
	return 0;
} 

