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
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif

struct	test1_s {
	const char*	name;
	float 		(*func)(float);	//the function
	float 		(*bench)(float);	//the function to benchmark against.
	float 		rng0, rng1;
	int			num;
	float 		emax;				//maximum error
	float 		xmax;				//location of maximum error
	int			time;				//time to execute num functions;
};

struct	test2_s {
	const char*	name;
	float 		(*func)(float, float);	//the function
	float 		(*bench)(float, float);	//the function to benchmark against.
	float 		rng0, rng1;
	int			num;
	float 		emax;				//maximum error
	float 		xmax;				//location of maximum error
	int			time;				//time to execute num functions;
};


typedef struct test1_s test1_t;
typedef struct test2_s test2_t;

test1_t test1[48] = 
{
	{"sinf       ", 	sinf, 		sinf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"sinf_c     ", 	sinf_c, 	sinf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"sinf_neon  ", 	sinf_neon, 	sinf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	
	{"cosf       ", 	cosf, 		cosf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"cosf_c     ", 	cosf_c, 	cosf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"cosf_neon  ", 	cosf_neon, 	cosf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},

	{"tanf       ", 	tanf, 		tanf, 	-M_PI_4, 	M_PI_4, 500000, 0, 0, 0},
	{"tanf_c     ", 	tanf_c, 	tanf, 	-M_PI_4, 	M_PI_4, 500000, 0, 0, 0},
	{"tanf_neon  ", 	tanf_neon, 	tanf, 	-M_PI_4, 	M_PI_4, 500000, 0, 0, 0},

	{"asinf      ", 	asinf, 		asinf, 	-1, 		1, 		500000, 0, 0, 0},
	{"asinf_c    ", 	asinf_c, 	asinf, 	-1, 		1,	 	500000, 0, 0, 0},
	{"asinf_neon ",		asinf_neon,	asinf, 	-1, 		1, 		500000, 0, 0, 0},
	
	{"acosf      ", 	acosf, 		acosf, 	-1, 		1, 		500000, 0, 0, 0},
	{"acosf_c    ", 	acosf_c, 	acosf, 	-1, 		1,	 	500000, 0, 0, 0},
	{"acosf_neon ",		acosf_neon,	acosf, 	-1, 		1, 		500000, 0, 0, 0},
	
	{"atanf      ", 	atanf, 		atanf, 	-1, 		1, 		500000, 0, 0, 0},
	{"atanf_c    ", 	atanf_c, 	atanf, 	-1, 		1,	 	500000, 0, 0, 0},
	{"atanf_neon ",		atanf_neon,	atanf, 	-1, 		1, 		500000, 0, 0, 0},

	{"sinhf       ", 	sinhf, 		sinhf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"sinhf_c     ", 	sinhf_c, 	sinhf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"sinhf_neon  ", 	sinhf_neon, sinhf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	
	{"coshf       ", 	coshf, 		coshf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"coshf_c     ", 	coshf_c, 	coshf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"coshf_neon  ", 	coshf_neon, coshf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},

	{"tanhf       ", 	tanhf, 		tanhf, 	-M_PI_4, 	M_PI_4, 500000, 0, 0, 0},
	{"tanhf_c     ", 	tanhf_c, 	tanhf, 	-M_PI_4, 	M_PI_4, 500000, 0, 0, 0},
	{"tanhf_neon  ", 	tanhf_neon, tanhf, 	-M_PI_4, 	M_PI_4, 500000, 0, 0, 0},

	{"expf       ", 	expf, 		expf, 	0, 			50, 	500000, 0, 0, 0},
	{"expf_c     ", 	expf_c, 	expf, 	0, 			50, 	500000, 0, 0, 0},
	{"expf_neon  ",		expf_neon, 	expf, 	0, 			50, 	500000, 0, 0, 0},
	
	{"logf       ", 	logf, 		logf, 	1, 			1000, 	500000, 0, 0, 0},
	{"logf_c     ", 	logf_c, 	logf, 	1, 			1000, 	500000, 0, 0, 0},
	{"logf_neon  ",		logf_neon, 	logf, 	1, 			1000, 	500000, 0, 0, 0},

	{"log10f       ", 	log10f, 	log10f, 1, 			1000, 	500000, 0, 0, 0},
	{"log10f_c     ", 	log10f_c, 	log10f, 1, 			1000, 	500000, 0, 0, 0},
	{"log10f_neon  ",	log10f_neon,log10f, 1, 			1000, 	500000, 0, 0, 0},

	{"floorf     ", 	floorf, 	floorf, 1, 			1000, 	500000, 0, 0, 0},
	{"floorf_c   ", 	floorf_c, 	floorf, 1, 			1000, 	500000, 0, 0, 0},
	{"floorf_neon",		floorf_neon,floorf, 1, 			1000, 	500000, 0, 0, 0},

	{"ceilf     ", 		ceilf, 		ceilf, 	1, 			1000, 	500000, 0, 0, 0},
	{"ceilf_c   ", 		ceilf_c, 	ceilf, 	1, 			1000, 	500000, 0, 0, 0},
	{"ceilf_neon",		ceilf_neon,	ceilf, 	1, 			1000, 	500000, 0, 0, 0},

	{"fabsf     ", 		fabsf, 		fabsf, 	1, 			1000, 	500000, 0, 0, 0},
	{"fabsf_c   ", 		fabsf_c, 	fabsf, 	1, 			1000, 	500000, 0, 0, 0},
	{"fabsf_neon",		fabsf_neon,	fabsf, 	1, 			1000, 	500000, 0, 0, 0},

	
	{"sqrtf      ", 	sqrtf, 		sqrtf, 	1, 			1000, 	500000, 0, 0, 0},
	{"sqrtf_c    ", 	sqrtf_c, 	sqrtf, 	1, 			1000, 	500000, 0, 0, 0},
	{"sqrtf_neon ",		sqrtf_neon,	sqrtf, 	1, 			1000, 	500000, 0, 0, 0},
};

test2_t test2[9] = 
{
	{"atan2f       ", 	atan2f, 	atan2f, 0.1, 		10, 	10000, 0, 0, 0},
	{"atan2f_c     ", 	atan2f_c, 	atan2f, 0.1, 		10, 	10000, 0, 0, 0},
	{"atan2f_neon  ", 	atan2f_neon,atan2f, 0.1, 		10, 	10000, 0, 0, 0},
	
	{"powf       ", 	powf, 		powf, 	1, 			10, 	10000, 0, 0, 0},
	{"powf_c     ", 	powf_c, 	powf, 	1, 			10, 	10000, 0, 0, 0},
	{"powf_neon  ", 	powf_neon, 	powf, 	1, 			10, 	10000, 0, 0, 0},

	{"fmodf       ", 	fmodf, 		fmodf, 	1, 			10, 	10000, 0, 0, 0},
	{"fmodf_c     ", 	fmodf_c, 	fmodf, 	1, 			10, 	10000, 0, 0, 0},
	{"fmodf_neon  ", 	fmodf_neon, fmodf, 	1, 			10, 	10000, 0, 0, 0},

};


void 
test_mathfunc1(test1_t *tst)
{

	float x;
	float dx = (tst->rng1 - tst->rng0) / (tst->num);

	tst->emax = 0;
	tst->xmax = 0;
	for(x = tst->rng0; x < tst->rng1 ; x += dx){	
		float r = (tst->func)(x);
		float rr = (tst->bench)(x);
		float dr = fabs(r - rr) * (100 / rr);
		if (dr > tst->emax && rr > 0.001){
			tst->emax = dr;
			tst->xmax = x;
		}
	}
	
#ifdef WIN32
	tst->time = GetTickCount()*1000 ;
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
#endif

	for(x = tst->rng0; x < tst->rng1 ; x += dx){	
		(tst->func)(x);
	}

#ifdef WIN32
	tst->time = GetTickCount()*1000 - tst->time;
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec - tst->time;
#endif

}

void
test_mathfunc2(test2_t *tst)
{
	float x, y;
	float rng = tst->rng1 - tst->rng0;
	float d = (rng * rng) / ((float) tst->num);

	tst->emax = 0;
	tst->xmax = 0;
	
	for(y = (tst->rng0); y < (tst->rng1) ; y += d){	
		for(x = (tst->rng0); x < (tst->rng1); x += d){	
			float r = (tst->func)(x, y);
			float rr = (tst->bench)(x, y);
			float dr = fabs(r - rr) * (100 / rr);
			if (dr > tst->emax && rr > 0.001){
				tst->emax = dr;
				tst->xmax = x;
			}
		}
	}
	
#ifdef WIN32
	tst->time = GetTickCount()*1000 ;
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
#endif

	for(y = tst->rng0; y < tst->rng1 ; y += d){	
		for(x = tst->rng0; x < tst->rng1 ; x += d){	
			(tst->func)(x, y);
		}
	}

#ifdef WIN32
	tst->time = GetTickCount()*1000 - tst->time;
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec - tst->time;
#endif

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

	int i, ii;

#ifndef WIN32
	if (argc > 1 && strcmp(argv[1], "-norunfast") == 0){
		printf("RUNFAST: Disabled \n");
	}else {
		printf("RUNFAST: Enabled \n");
		enable_runfast();
	}
#endif	

#if 1

	//test single argument functions:
	printf("Function\tRange\t\tNumber\tMax Error\tRate\n");	
	printf("-------------------------------------------------------------------------\n");	
	for(i = 0; i < 48; i++){
		test_mathfunc1(&test1[i]);	
		
		ii = i - (i % 3);
		
		printf("%s\t[%.2f, %.2f]\t%i\t%.2e%%\t%.2f%%\n", test1[i].name,  test1[i].rng0, test1[i].rng1, 
			test1[i].num, test1[i].emax, 100.0f * (float)test1[ii].time / test1[i].time);
	}
	for(i = 0; i < 9; i++){
		test_mathfunc2(&test2[i]);
	
		ii = i - (i % 3);
		
		printf("%s\t[%.2f, %.2f]\t%i\t%.2e%%\t%.2f%%\n", test2[i].name,  test2[i].rng0, test2[i].rng1, 
			test2[i].num, test2[i].emax, 100.0f * (float)test2[ii].time / test2[i].time);
	}
	printf("--------------------------------------------------------------------------\n");	

#else

	float x = 0;
	for(x = -M_PI_2; x < M_PI_2; x+= 0.01)
	{
		printf("x=%.2f\t in=%.2f\t neon=%.2f \n", x, tanf(x), tanf_neon(x));
	
	}
	
#endif
	
	return 0;
} 

