/* $Id: math.h,v 1.2 2004/10/01 19:13:22 GrosbaJ Exp $ */
#ifndef __MATH_H
#define __MATH_H

typedef float float_t;
typedef float double_t;

#define HUGE_VAL 6.3e38
#define HUGE_VALL 6.3e38
#define HUGE_VALF 6.3e38

float fabs (float x);
float ldexp (float x, int n);
float exp (float f);
float sqrt (float x);
float asin (float x);
float acos (float x);
float atan2 (float y, float x);
float atan (float x);
float sin (float x);
float cos (float x);
float tan (float x);
float sinh (float x);
float cosh (float x);
float tanh (float x);
float frexp (float x, int *pexp);
float log10 (float x);
float log (float x);
float pow (float x, float y);
float ceil (float x);
float floor (float x);
float modf (float x, float *ipart);
float fmod (float x, float y);

float mchptoieee (unsigned long v);
unsigned long ieeetomchp (float v);

#endif
