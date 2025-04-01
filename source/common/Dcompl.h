// dcompl.h
#pragma once

#define M_PI      3.141592653589793
#define M_2_PI  6.283185307179586

typedef struct sdcomplex {
 double r;
 double i;
 } tagsdcomplex;
typedef struct sdcomplex dcomplex;

dcomplex MakeComplex (double r, double i);

dcomplex Cadd( dcomplex a, dcomplex b);

dcomplex Csub( dcomplex a, dcomplex b);

dcomplex Cmul( dcomplex a, dcomplex b);

dcomplex RCmul( double a, dcomplex b);

dcomplex Cconj(dcomplex a);

double Cabs(dcomplex a);

dcomplex Cdiv( dcomplex a, dcomplex b);

dcomplex Csqrt( dcomplex a);

void SolveCubic(double r, double s, double t, dcomplex *roots);

void SolveQuartic ( double a, double b, double c, double d, double e,
		    dcomplex *roots);

