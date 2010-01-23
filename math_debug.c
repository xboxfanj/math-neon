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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif

#define randf()	(rand() / (RAND_MAX + 1.0f))



struct	test1_s {
	const char*	name;
	float 		(*func)(float);	//the function
	float 		(*bench)(float);	//the function to benchmark against.
	float 		rng0, rng1;
	int			num;
	float 		emaxabs;
	float 		xmaxabs;
	float 		emaxrel;
	float 		xmaxrel;
	float 		erms;
	int			time;				//time to execute num functions;
};

struct	test2_s {
	const char*	name;
	float 		(*func)(float, float);	//the function
	float 		(*bench)(float, float);	//the function to benchmark against.
	float 		rng0, rng1;
	int			num;
	float 		emaxabs;
	float 		xmaxabs;
	float 		emaxrel;
	float 		xmaxrel;
	float 		erms;
	int			time;				//time to execute num functions;
};


float invsqrtf(float x){
	return (1.0f / sqrtf(x));
}

typedef struct test1_s test1_t;
typedef struct test2_s test2_t;

test1_t test1[51] = 
{
	{"sinf       ", 	sinf, 		sinf, 	-M_PI, 		M_PI, 	500000},
	{"sinf_c     ", 	sinf_c, 	sinf, 	-M_PI, 		M_PI, 	500000},
	{"sinf_neon  ", 	sinf_neon, 	sinf, 	-M_PI, 		M_PI, 	500000},
	
	{"cosf       ", 	cosf, 		cosf, 	-M_PI, 		M_PI, 	500000},
	{"cosf_c     ", 	cosf_c, 	cosf, 	-M_PI, 		M_PI, 	500000},
	{"cosf_neon  ", 	cosf_neon, 	cosf, 	-M_PI, 		M_PI, 	500000},

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

	{"tanhf       ", 	tanhf, 		tanhf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"tanhf_c     ", 	tanhf_c, 	tanhf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},
	{"tanhf_neon  ", 	tanhf_neon, tanhf, 	-M_PI, 		M_PI, 	500000, 0, 0, 0},

	{"expf       ", 	expf, 		expf, 	0, 			10, 	500000, 0, 0, 0},
	{"expf_c     ", 	expf_c, 	expf, 	0, 			10, 	500000, 0, 0, 0},
	{"expf_neon  ",		expf_neon, 	expf, 	0, 			10, 	500000, 0, 0, 0},
	
	{"logf       ", 	logf, 		logf, 	1, 			1000, 	500000, 0, 0, 0},
	{"logf_c     ", 	logf_c, 	logf, 	1, 			1000, 	500000, 0, 0, 0},
	{"logf_neon  ",		logf_neon, 	logf, 	1, 			1000, 	500000, 0, 0, 0},

	{"log10f       ", 	log10f, 	log10f, 1, 			1000, 	500000, 0, 0, 0},
	{"log10f_c     ", 	log10f_c, 	log10f, 1, 			1000, 	500000, 0, 0, 0},
	{"log10f_neon  ",	log10f_neon,log10f, 1, 			1000, 	500000, 0, 0, 0},

	{"floorf     ", 	floorf, 	floorf, 1, 			1000, 	5000000, 0, 0, 0},
	{"floorf_c   ", 	floorf_c, 	floorf, 1, 			1000, 	5000000, 0, 0, 0},
	{"floorf_neon",		floorf_neon,floorf, 1, 			1000, 	5000000, 0, 0, 0},

	{"ceilf     ", 		ceilf, 		ceilf, 	1, 			1000, 	5000000, 0, 0, 0},
	{"ceilf_c   ", 		ceilf_c, 	ceilf, 	1, 			1000, 	5000000, 0, 0, 0},
	{"ceilf_neon",		ceilf_neon,	ceilf, 	1, 			1000, 	5000000, 0, 0, 0},

	{"fabsf     ", 		fabsf, 		fabsf, 	1, 			1000, 	5000000, 0, 0, 0},
	{"fabsf_c   ", 		fabsf_c, 	fabsf, 	1, 			1000, 	5000000, 0, 0, 0},
	{"fabsf_neon",		fabsf_neon,	fabsf, 	1, 			1000, 	5000000, 0, 0, 0},

	{"sqrtf      ", 	sqrtf, 		sqrtf, 	1, 			1000, 	500000, 0, 0, 0},
	{"sqrtf_c    ", 	sqrtf_c, 	sqrtf, 	1, 			1000, 	500000, 0, 0, 0},
	{"sqrtf_neon ",		sqrtf_neon,	sqrtf, 	1, 			1000, 	500000, 0, 0, 0},

	{"invsqrtf      ", 	invsqrtf, 		invsqrtf, 	1, 	1000, 	500000, 0, 0, 0},
	{"invsqrtf_c    ", 	invsqrtf_c, 	invsqrtf, 	1, 	1000, 	500000, 0, 0, 0},
	{"invsqrtf_neon ",	invsqrtf_neon,	invsqrtf, 	1, 	1000, 	500000, 0, 0, 0},
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
	float dx = (tst->rng1 - tst->rng0) / ((float)tst->num);
#ifndef WIN32
	struct rusage ru;
#endif

	tst->emaxabs = tst->xmaxabs = 0;
	tst->emaxrel = tst->xmaxrel = 0;
	tst->erms = 0;
	for(x = tst->rng0; x < tst->rng1 ; x += dx){	
		float r = (tst->func)((float)x);
		float rr = (tst->bench)((float)x);
		float dr = fabs(r - rr);
		float drr = dr * (100.0f / rr);
		tst->erms += dr*dr;
		if (dr > tst->emaxabs){
			tst->emaxabs = dr;
			tst->xmaxabs = x;
		}
		if (drr > tst->emaxrel){
			tst->emaxrel = drr;
			tst->xmaxrel = x;
		}
	}
	tst->erms = sqrt(tst->erms / ((float) tst->num));
	
#ifdef WIN32
	tst->time = (1000 * clock()) / (CLOCKS_PER_SEC / 1000);
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
#endif

	for(x = tst->rng0; x < tst->rng1 ; x += dx){	
		(tst->func)((float)x);
	}

#ifdef WIN32
	tst->time = (1000 * clock()) / (CLOCKS_PER_SEC / 1000) - tst->time;
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
#ifndef WIN32
	struct rusage ru;
#endif

	tst->emaxabs = tst->xmaxabs = 0;
	tst->emaxrel = tst->xmaxrel = 0;
	for(y = (tst->rng0); y < (tst->rng1) ; y += d){	
		for(x = (tst->rng0); x < (tst->rng1); x += d){	
			float r = (tst->func)((float)x, y);
			float rr = (tst->bench)((float)x, y);
			float dr = fabs(r - rr);
			float drr = dr * (100.0f / rr);
			if (dr > tst->emaxabs){
				tst->emaxabs = dr;
				tst->xmaxabs = x;
			}
			if (drr > tst->emaxrel && fabsf(rr) > 0.0001){
				tst->emaxrel = drr;
				tst->xmaxrel = x;
			}
		}
	}
	
#ifdef WIN32
	tst->time = (1000 * clock()) / (CLOCKS_PER_SEC / 1000) ;
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
#endif

	for(y = tst->rng0; y < tst->rng1 ; y += d){	
		for(x = tst->rng0; x < tst->rng1 ; x += d){	
			(tst->func)((float)x, (float)y);
		}
	}

#ifdef WIN32
	tst->time = (1000 * clock()) / (CLOCKS_PER_SEC / 1000) - tst->time;
#else
	getrusage(RUSAGE_SELF, &ru);	
	tst->time = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec - tst->time;
#endif

}


void test_matrixfunc()
{
	float m2a[4], m2b[4], m2c[4];
	float m3a[9], m3b[9], m3c[9];
	float m4a[16], m4b[16], m4c[16];
	int m2t[3], m3t[3], m4t[3];
	
	int i;
	int testnum = 1000000;
	struct rusage ru;
	
	m2a[0] = 10.0f * randf(); m2a[2] = 10.0f * randf(); 
	m2a[1] = 10.0f * randf(); m2a[3] = 10.0f * randf();
	
	m2b[0] = 6.329; m2b[2] = 2.742; 
	m2b[1] = 1.453; m2b[3] = 9.386;
	
	getrusage(RUSAGE_SELF, &ru);	
	m2t[0] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	for(i = 0; i < testnum; i++){
		matmul2_c(m2a, m2b, m2c);	
	}
	getrusage(RUSAGE_SELF, &ru);	
	m2t[1] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	for(i = 0; i < testnum; i++){
		matmul2_neon(m2a, m2b, m2c);
	}
	getrusage(RUSAGE_SELF, &ru);	
	m2t[2] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;

	memset(m2c, 0, 4*sizeof(float));
	matmul2_c(m2a, m2b, m2c);	
	printf("matmul2_c = \n");
	printf("\t\t\t|%.2f, %.2f|\n", m2c[0], m2c[2]);
	printf("\t\t\t|%.2f, %.2f|\n", m2c[1], m2c[3]);

	memset(m2c, 0, 4*sizeof(float));
	matmul2_neon(m2a, m2b, m2c);	
	printf("matmul2_neon = \n");
	printf("\t\t\t|%.2f, %.2f|\n", m2c[0], m2c[2]);
	printf("\t\t\t|%.2f, %.2f|\n", m2c[1], m2c[3]);
	
	printf("matmul2: c=%i \t neon=%i \t rate=%.2f \n", m2t[1] - m2t[0], m2t[2] - m2t[1], 
		(float)(m2t[1] - m2t[0]) / (float)(m2t[2] - m2t[1]));


	//MAT3
	m3a[0] = 10.0f * randf(); m3a[3] = 10.0f * randf(); m3a[6] = 10.0f * randf();
	m3a[1] = 10.0f * randf(); m3a[4] = 10.0f * randf(); m3a[7] = 10.0f * randf();
	m3a[2] = 10.0f * randf(); m3a[5] = 10.0f * randf(); m3a[8] = 10.0f * randf();
	
	m3b[0] = 10.0f * randf(); m3b[3] = 10.0f * randf(); m3b[6] = 10.0f * randf();
	m3b[1] = 10.0f * randf(); m3b[4] = 10.0f * randf(); m3b[7] = 10.0f * randf();
	m3b[2] = 10.0f * randf(); m3b[5] = 10.0f * randf(); m3b[8] = 10.0f * randf();
	
	getrusage(RUSAGE_SELF, &ru);	
	m3t[0] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	for(i = 0; i < testnum; i++){
		matmul3_c(m3a, m3b, m3c);	
	}
	getrusage(RUSAGE_SELF, &ru);	
	m3t[1] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	for(i = 0; i < testnum; i++){
		matmul3_neon(m3a, m3b, m3c);
	}
	getrusage(RUSAGE_SELF, &ru);	
	m3t[2] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;

	memset(m3c, 0, 9*sizeof(float));
	matmul3_c(m3a, m3b, m3c);	
	printf("matmul3_c =\n");
	printf("\t\t\t|%.2f, %.2f, %.2f|\n", m3c[0], m3c[3], m3c[6]);
	printf("\t\t\t|%.2f, %.2f, %.2f|\n", m3c[1], m3c[4], m3c[7]);
	printf("\t\t\t|%.2f, %.2f, %.2f|\n", m3c[2], m3c[5], m3c[8]);
	
	memset(m3c, 0, 9*sizeof(float));
	matmul3_neon(m3a, m3b, m3c);	
	printf("matmul3_neon =\n");
	printf("\t\t\t|%.2f, %.2f, %.2f|\n", m3c[0], m3c[3], m3c[6]);
	printf("\t\t\t|%.2f, %.2f, %.2f|\n", m3c[1], m3c[4], m3c[7]);
	printf("\t\t\t|%.2f, %.2f, %.2f|\n", m3c[2], m3c[5], m3c[8]);
	
	printf("matmul3: c=%i \t neon=%i \t rate=%.2f \n", m3t[1] - m3t[0], m3t[2] - m3t[1], 
		(float)(m3t[1] - m3t[0]) / (float)(m3t[2] - m3t[1]));


	//MAT4
	m4a[0] = 10.0f * randf(); m4a[4] = 10.0f * randf(); m4a[8] = 10.0f * randf();  m4a[12] = 10.0f * randf();
	m4a[1] = 10.0f * randf(); m4a[5] = 10.0f * randf(); m4a[9] = 10.0f * randf();  m4a[13] = 10.0f * randf();
	m4a[2] = 10.0f * randf(); m4a[6] = 10.0f * randf(); m4a[10] = 10.0f * randf();  m4a[14] = 10.0f * randf();
	m4a[3] = 10.0f * randf(); m4a[7] = 10.0f * randf(); m4a[11] = 10.0f * randf();  m4a[15] = 10.0f * randf();
	
	m4b[0] = 10.0f * randf(); m4b[4] = 10.0f * randf(); m4b[8] = 10.0f * randf();  m4b[12] = 10.0f * randf();
	m4b[1] = 10.0f * randf(); m4b[5] = 10.0f * randf(); m4b[9] = 10.0f * randf();  m4b[13] = 10.0f * randf();
	m4b[2] = 10.0f * randf(); m4b[6] = 10.0f * randf(); m4b[10] = 10.0f * randf();  m4b[14] = 10.0f * randf();
	m4b[3] = 10.0f * randf(); m4b[7] = 10.0f * randf(); m4b[11] = 10.0f * randf();  m4b[15] = 10.0f * randf();
		
	getrusage(RUSAGE_SELF, &ru);	
	m4t[0] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	for(i = 0; i < testnum; i++){
		matmul4_c(m4a, m4b, m4c);	
	}
	getrusage(RUSAGE_SELF, &ru);	
	m4t[1] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;
	for(i = 0; i < testnum; i++){
		matmul4_neon(m4a, m4b, m4c);
	}
	getrusage(RUSAGE_SELF, &ru);	
	m4t[2] = ru.ru_utime.tv_sec * 1000000 + ru.ru_utime.tv_usec;

	memset(m4c, 0, 16*sizeof(float));
	matmul4_c(m4a, m4b, m4c);	
	printf("matmul4_c =\n");
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[0], m4c[4], m4c[8], m4c[12]);
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[1], m4c[5], m4c[9], m4c[13]);
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[2], m4c[6], m4c[10], m4c[14]);
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[3], m4c[7], m4c[11], m4c[15]);
	
	memset(m4c, 0, 16*sizeof(float));
	matmul4_neon(m4a, m4b, m4c);	
	printf("matmul4_neon =\n");
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[0], m4c[4], m4c[8], m4c[12]);
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[1], m4c[5], m4c[9], m4c[13]);
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[2], m4c[6], m4c[10], m4c[14]);
	printf("\t\t\t|%.2f, %.2f, %.2f, %.2f|\n", m4c[3], m4c[7], m4c[11], m4c[15]);
	
	printf("matmul4: c=%i \t neon=%i \t rate=%.2f \n", m4t[1] - m4t[0], m4t[2] - m4t[1], 
		(float)(m4t[1] - m4t[0]) / (float)(m4t[2] - m4t[1]));

}

int x, y, z;

int main(int argc, char** argv)
{

	z = x / y;
	

	int i, ii;
	if (argc > 1 && strcmp(argv[1], "-norunfast") == 0){
		printf("RUNFAST: Disabled \n");
	}else {
		printf("RUNFAST: Enabled \n");
		enable_runfast();
	}

#if 0
	//test single argument functions:
	printf("------------------------------------------------------------------------------------------------------\n");	
	printf("MATRIX FUNCTION TESTS \n");	
	printf("------------------------------------------------------------------------------------------------------\n");	
	
	test_matrixfunc();

	printf("------------------------------------------------------------------------------------------------------\n");	
	printf("CMATH FUNCTION TESTS \n");	
	printf("------------------------------------------------------------------------------------------------------\n");	
	printf("Function\tRange\t\tNumber\tABS Max Error\tREL Max Error\tRMS Error\tTime\tRate\n");	
	printf("------------------------------------------------------------------------------------------------------\n");	
	for(i = 0; i < 51; i++){
		test_mathfunc1(&test1[i]);	
		
		ii = i - (i % 3);
		printf("%s\t", test1[i].name);
		printf("[%.2f, %.2f]\t", test1[i].rng0, test1[i].rng1);
		printf("%i\t", test1[i].num);
		printf("%.2e\t", test1[i].emaxabs);
		printf("%.2e%%\t", test1[i].emaxrel);
		printf("%.2e\t", test1[i].erms);
		printf("%i\t", test1[i].time);
		printf("x%.2f\t", (float)test1[ii].time / test1[i].time);
		printf("\n");
	}
	for(i = 0; i < 9; i++){
		test_mathfunc2(&test2[i]);
	
		ii = i - (i % 3);
		
		printf("%s\t", test2[i].name);
		printf("[%.2f, %.2f]\t", test2[i].rng0, test2[i].rng1);
		printf("%i\t", test2[i].num);
		printf("%.2e\t", test2[i].emaxabs);
		printf("%.2e%%\t", test2[i].emaxrel);
		printf("%.2e\t", test2[i].erms);
		printf("%i\t", test2[i].time);
		printf("x%.2f\t", (float)test2[ii].time / test2[i].time);
		printf("\n");
	}
	
#else


	float x = 0;
	for(x = -M_PI_2; x < M_PI_2; x+= 0.01)
	{
		printf("x=%.2f\t in=%.2f\t c=%.2f\t neon=%.2f \n", x, sinhf(x), sinhf_c(x), sinhf_neon(x));
	}

#endif
	
	return 0;
} 
