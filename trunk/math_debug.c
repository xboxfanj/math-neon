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

int main(int argc, char** argv){
		
	double x, p;
	int n = 0;
	double emax = 0;
	double erms = 0;
	double xmax = 0;
	
	for(x = -10; x < 10; x += 0.01){
		
		float r;
		float rr;
		float dr;
		double e;
		
		r = sinf_c((float)x);
		rr = sinf((float)x);
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
		//printf("x=%f \t r=%f \t rr=%f \t e=%f \n", x, r, rr, e);
	}
	printf("\n");
	printf("Max Error %f at x = %f \n", emax, xmax);
	printf("RMS Error %f \n", sqrtf(erms / n));
	return 0;
} 