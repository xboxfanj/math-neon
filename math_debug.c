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
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>


struct	test_s {
	const char*	name;
	float 		(*func)(float);	//the function
	float 		(*bench)(float);	//the function to benchmark against.
	float 		rng0, rng1;
	int			num;
	float 		emax;				//maximum error
	float 		xmax;				//location of maximum error
	int			time;				//time to execute num functions;
};

typedef struct test_s test_t;

test_t test[32] = {
	{"sinf       ", 	sinf, 		sinf, 	-M_PI, 		M_PI, 	1000000, 0, 0, 0},
	{"sinf_c     ", 	sinf_c, 	sinf, 	-M_PI, 		M_PI, 	1000000, 0, 0, 0},
	{"sinf_neon  ", 	sinf_neon, 	sinf, 	-M_PI, 		M_PI, 	1000000, 0, 0, 0},
	{"tanf       ", 	tanf, 		tanf, 	-M_PI_2, 	M_PI_2, 1000000, 0, 0, 0},
	{"tanf_c     ", 	tanf_c, 	tanf, 	-M_PI_2, 	M_PI_2, 1000000, 0, 0, 0},
	{"tanf_neon  ", 	tanf_neon, 	tanf, 	-M_PI_2, 	M_PI_2, 1000000, 0, 0, 0},
	{"expf       ", 	expf, 		expf, 	0, 			50, 	1000000, 0, 0, 0},
	{"expf_c     ", 	expf_c, 	expf, 	0, 			50, 	1000000, 0, 0, 0},
	{"expf_neon  ",		expf_neon, 	expf, 	0, 			50, 	1000000, 0, 0, 0},
	{"logf       ", 	logf, 		logf, 	1, 			10000, 	1000000, 0, 0, 0},
	{"logf_c     ", 	logf_c, 	logf, 	1, 			10000, 	1000000, 0, 0, 0},
	{"logf_neon  ",		logf_neon, 	logf, 	1, 			10000, 	1000000, 0, 0, 0},
	{"floorf     ", 	floorf, 	floorf, 1, 			10000, 	1000000, 0, 0, 0},
	{"floorf_c   ", 	floorf_c, 	floorf, 1, 			10000, 	1000000, 0, 0, 0},
	{"floorf_neon",		floorf_neon,floorf, 1, 			10000, 	1000000, 0, 0, 0},
	{"sqrtf      ", 	sqrtf, 		sqrtf, 	1, 			10000, 	1000000, 0, 0, 0},
	{"sqrtf_c    ", 	sqrtf_c, 	sqrtf, 	1, 			10000, 	1000000, 0, 0, 0},
	{"sqrtf_neon ",		sqrtf_neon,	sqrtf, 	1, 			10000, 	1000000, 0, 0, 0},
	{"atanf      ", 	atanf, 		atanf, 	-1, 		1, 		1000000, 0, 0, 0},
	{"atanf_c    ", 	atanf_c, 	atanf, 	-1, 		1,	 	1000000, 0, 0, 0},
	{"atanf_neon ",		atanf_neon,	atanf, 	-1, 		1, 		1000000, 0, 0, 0},
};


void 
test_mathfunc(test_t *tst){

	struct rusage ru;
	float x;
	float dx = (tst->rng1 - tst->rng0) / (tst->num);


	tst->emax = 0;
	tst->xmax = 0;
	for(x = tst->rng0; x < tst->rng1 ; x += dx){	
		float r = (*tst->func)(x);
		float rr = (*tst->bench)(x);
		float dr = fabs(r - rr) * (100 / rr);
		if (dr > tst->emax && rr > 0.001){
			tst->emax = dr;
			tst->xmax = x;
		}
	}

	getrusage(RUSAGE_SELF, &ru);	
	tst->time = -ru.ru_utime.tv_sec * 1000000 - ru.ru_utime.tv_usec;

	for(x = tst->rng0; x < tst->rng1 ; x += dx){	
		(*tst->func)(x);
	}

	getrusage(RUSAGE_SELF, &ru);	
	tst->time += ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	
}


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

int main(int argc, char** argv)
{

	int i;
	
	if (argc > 1 && strcmp(argv[1], "-norunfast") == 0){
		printf("RUNFAST: Disabled \n");
	}else {
		printf("RUNFAST: Enabled \n");
		enable_runfast();
	}
	
	printf("Function\tRange\t\tNumber\tMax Error\tTime\n");	
	printf("----------------------------------------------------------------\n");	
	for(i = 0; i < 21; i++){
		test_mathfunc(&test[i]);	
		printf("%s\t[%.2f, %.2f]\t%i\t%.2e\t%i\n", test[i].name,  test[i].rng0, test[i].rng1, 
			test[i].num, test[i].emax,   test[i].time);
	}
	printf("----------------------------------------------------------------\n");	
	
	return 0;
} 

