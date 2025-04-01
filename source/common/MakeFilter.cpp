#include "stdafx.h"
#include "../common/MakeFilter.h"
//#include "../common/DComplex.h"

#include <complex>
#include <math.h>
#include "utils.h"


complex <double> MakeFilter::bessel_poles[30]={
    ( -1.000000e+00,  0.000000e+00 ),	 (-8.660254e-01, -5.000000e-01 ),    ( -9.416000e-01,	0.000000e+00 ),
  ( -7.456404e-01, -7.113666e-01 ),	 ( -9.047588e-01, -2.709187e-01 ),    ( -6.572112e-01, -8.301614e-01 ),
  ( -9.264421e-01,  0.000000e+00 ),	 ( -8.515536e-01, -4.427175e-01 ),    ( -5.905759e-01, -9.072068e-01 ),
    ( -9.093907e-01, -1.856964e-01 ),	 ( -7.996542e-01, -5.621717e-01 ),    ( -5.385527e-01, -9.616877e-01 ),
  ( -9.194872e-01,  0.000000e+00 ),	 ( -8.800029e-01, -3.216653e-01 ),    ( -7.527355e-01, -6.504696e-01 ),
   ( -4.966917e-01, -1.002509e+00 ),	 ( -9.096832e-01, -1.412438e-01 ),    ( -8.473251e-01, -4.259018e-01 ),
   ( -7.111382e-01, -7.186517e-01 ),	 ( -4.621740e-01, -1.034389e+00 ),    ( -9.154958e-01,	0.000000e+00 ),
   ( -8.911217e-01, -2.526581e-01 ),	 ( -8.148021e-01, -5.085816e-01 ),    ( -6.743623e-01, -7.730546e-01 ),
  ( -4.331416e-01, -1.060074e+00 ),	 ( -9.091347e-01, -1.139583e-01 ),    ( -8.688460e-01, -3.430008e-01 ),
  ( -7.837694e-01, -5.759148e-01 ),	 ( -6.417514e-01, -8.175836e-01 ),    ( -4.083221e-01, -1.081275e+00 ),
 };

MakeFilter::MakeFilter(){
	IsInitialized = false;
}
MakeFilter::~MakeFilter(){
}
