#include "stdafx.h"
#include "../common/GeneralFilter.h"
//#include "../common/DComplex.h"

#include <complex>
#include <math.h>
#include "utils.h"
//#include <complex>
//#include <iostream>
/*

complex <double> csqrt(complex <double> x)// struct complex x;
  { 
	  double rr = x.real();
	 double r = _hypot(x.imag(), rr);
	complex <double> z (Xsqrt(0.5 * (r + rr)), Xsqrt(0.5 * (r - rr)));
//	z.real = Xsqrt(0.5 * (r + x.real()));
//	z.imag = Xsqrt(0.5 * (r - x.real()));
	if (x.imag() < 0.0){
		z = cconj(z);

	}
    return z;
  }
complex <double> eval(complex <double> * coeffs, int np, complex <double> z){// struct complex coeffs[]; int np; struct complex z;
//  { /* evaluate polynomial in z, substituting for z */
//		complex <double> sum (0,0);
//    sum = czero;
//    for (int i=np; i >= 0; i--) sum = cadd(cmul(sum, z), coeffs[i]);
//		for (int i=np; i >= 0; i--){
//			sum = sum*z + coeffs[i];
//			sum = cadd(cmul(sum, z), coeffs[i]);
//		}
//    return sum;
//  }


//template <class T>
//complex<double> evaluate(complex <double> * topco, complex <double> * botco, int np, complex <double> z)
// struct complex topco[], botco[]; int np; struct complex z;
//  { /* evaluate response, substituting for z */
//	  return eval(topco, np, z)/eval(botco, np, z);
//    return cdiv(eval(topco, np, z), eval(botco, np, z));
//  }

//complex <double> cexp(complex <double> x) {
//	complex <double> z;
//	double r = exp(x.real());
//	double i = x.imag();
//	complex <double> z (r * cos(i), r * sin(i));
//    z.real = r * cos(x.imag());
 //   z.imag = r * sin(x.imag());
 //   return z;
 // }

//double cnorm(complex <double> x){
//	return sqrt(x.real()*x.real()+x.imag()*x.imag());
//}
//cmone = complex <double>( -1.0, 0.0 );
//*/
