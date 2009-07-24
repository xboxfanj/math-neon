

#ifndef __MATH_NEON_H__ 
#define __MATH_NEON_H__ 

#if !defined(__i386__) && defined(__arm__)
#define __MATH_NEON
#endif


#ifdef GCC
#define ALIGN(A) __attribute__ ((aligned (A))
#else
#define ALIGN(A)
#endif


float 	sinf_c(float x);
float 	cosf_c(float x);
float 	tanf_c(float x);
float 	expf_c(float x);
float 	logf_c(float x);
float 	powf_c(float x, float n);
float 	atanf_c(float x);
void	sincosf_c(float x, float r[2]);

float 	sinf_neon(float x);
float 	cosf_neon(float x);
float 	tanf_neon(float x);

void	sinfv_c(float *x, int n);
void  	sinfv_neon(float *x, int n);


float 	logf_fast(const float x);
float 	expf_fast(const float x);
float 	log2f_fast(const float x);
float 	powf_fast(const float x, const float y);

float _floorf(const float x);
float _fmodf(const float x);
float _frexpf(const float x);
float _ldexpf(const float x);
float _log10f(const float x);
float _modf(const float x, const float p[2]);
float _sinhf(const float x);
float _tanf(const float x);
float _tanhf(const float x);
float _acosf(const float x);
float _asinf(const float x);
float _atanf(const float x);
float _atan2f(const float x);
float _ceilf(const float x);
float _cosf(const float x);
float _coshf(const float x);

#endif
