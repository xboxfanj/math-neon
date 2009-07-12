

#include "math_neon.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv){
		
	double x;
	int n = 0;
	double emax = 0;
	double erms = 0;
	double xmax = 0;
	for(x = -M_PI; x < M_PI;x += 0.001){
		
		double r = sinf_c((float)x);
		double rr = sinf((float)x);
		
		double e = fabsf(r - rr);
		//if (fabs(rr) > 1E-4){
			if (e>emax) {
				emax = e;
				xmax = x;
			}
			n++;
			erms += e * e;
		//}
		printf("x=%f \t r=%f \t rr=%f \t e=%f \n", x, r, rr, e);
	}

	printf("\n", emax, xmax);
	printf("Max Error %f at x = %f \n", emax, xmax);
	printf("RMS Error %f \n", sqrt(erms / n));
	return 0;
} 