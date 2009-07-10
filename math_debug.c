

#include "math_neon.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv){
		
	float x;
	int n = 0;
	float emax = 0;
	float erms = 0;
	float xmax = 0;
	for(x = -100; x < 100;x += 0.1){
		
		float r = tanf_c(x);
		float rr = tanf(x);
		
		float e = 100 * fabsf(r - rr) / rr;
		if (e>emax) {
			emax = e;
			xmax = x;
		}
		erms += e * e;
		printf("x=%f \t r=%f \t rr=%f \t e=%f%% \n", x, r, rr, e);
		n++;
	}

	printf("\n", emax, xmax);
	printf("Max Error %f%% at x = %f \n", emax, xmax);
	printf("RMS Error %f%% \n", sqrt(erms / n));
	return 0;
} 