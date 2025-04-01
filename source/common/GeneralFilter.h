#ifndef MIK_GENERAL_FILTER
#define MIK_GENERAL_FILTER

#include <complex>
#include <math.h>
#include "utils.h"

using namespace std;

///*
//#define uint	    unsigned int


//#define PI_GENERAL_FILTER	    3.14159265358979323846
//#define TWOPI_GENERAL_FILTER	    (2.0 * PI_GENERAL_FILTER)
//#define EPS_GENERAL_FILTER	    1e-10
//#define MAXORDER_GENERAL_FILTER    10
//#define MAXPOLES_GENERAL_FILTER    (2*MAXORDER_GENERAL_FILTER)    /* to allow for doubling of poles in BP filter */
//*/


#define opt_be 0x0001	/* -Be		Bessel cheracteristic	       */
#define opt_bu 0x0002	/* -Bu		Butterworth characteristic     */
#define opt_ch 0x0004	/* -Ch		Chebyshev characteristic       */

#define opt_lp 0x0008	/* -Lp		low-pass		       */
#define opt_hp 0x0010	/* -Hp		high-pass		       */
#define opt_bp 0x0020	/* -Bp		band-pass		       */

#define opt_a  0x0040	/* -a		alpha value		       */
#define opt_e  0x0100	/* -e		execute filter		       */
#define opt_l  0x0200	/* -l		just list filter parameters    */
#define opt_o  0x0400	/* -o		order of filter		       */
#define opt_p  0x0800	/* -p		specified poles only	       */
#define opt_w  0x1000	/* -w		don't pre-warp		       */

/*
double cnorm(complex <double> x);
complex <double> cexp(complex <double> x);
complex <double> eval(complex <double> * coeffs, int np, complex <double> z);
complex<double> evaluate(complex <double> * topco, complex <double> * botco, int np, complex <double> z);
complex <double> csqrt(complex <double> x);// struct complex x;
*/

template <class T>
class GeneralFilter{
public:
	static void Init();

	GeneralFilter();
	~GeneralFilter();
	void Define(int Characteristic, //0: Bessel; 1: butterw.; 2: cheb.
				int Lowhiband, //0: low pass; 1: hipass; 2: bandpass
				int Order,
				double F1,
				double F2=0.0,
				double Chebrip=-0.1);

	GeneralFilter(int Characteristic, //0: Bessel; 1: butterw.; 2: cheb.
				int Lowhiband, //0: low pass; 1: hipass; 2: bandpass
				int Order,
				double F1,
				double F2=0.0,
				double Chebrip=-0.1);

	void Test();

	void DoFilter(T * in, T * out, int NP);

	double xcoeffs[MAXPOLES_GENERAL_FILTER+1], ycoeffs[MAXPOLES_GENERAL_FILTER+1];


private:
	static bool Initialized;
	int order, numpoles;
	int characteristic;
	int lohiband;
public:
	double raw_alpha1, raw_alpha2;
private:
	static complex <double> bessel_poles[30]; 
	complex <double> dc_gain, fc_gain, hf_gain;
	static uint opts;	/* option flag bits */

	double warped_alpha1, warped_alpha2, chebrip;
	uint polemask;

	bool optsok;

	complex <double> spoles[MAXPOLES_GENERAL_FILTER];
	complex <double>  zpoles[MAXPOLES_GENERAL_FILTER], zzeros[MAXPOLES_GENERAL_FILTER];

	void Compute_s();
	void choosepole(complex <double> z);
    void normalize();
    void compute_z();
    void expandpoly();
	void expand(complex <double> * pz, complex <double> * coeffs);
	void multin(complex <double> w, complex <double> *coeffs);

};


template <class T>
complex <double> GeneralFilter<T>::bessel_poles[30]={
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



template <class T>
void GeneralFilter<T>::DoFilter(T* in, T * out, int NP){
	double ybuff[MAXORDER_GENERAL_FILTER];
	double xbuff[MAXORDER_GENERAL_FILTER];
	if (NP<order) return;

	double gain = cnorm(dc_gain);
	double meanfirst = double(MeanArray(in, order));
//	ShowFloat(meanfirst, "mf");

	int orderp=order+1;
	for (int i=0; i<order; i++){
		xbuff[i] = meanfirst;
		ybuff[i] = meanfirst*gain;
	}
	xbuff[order] = meanfirst;
	int posx = 0;
	int posy = 0;
	for (int i=0; i<NP; i++){
		double temp=0.0;
		for (int j=0; j<order; j++){
			int kx=(posx+j) % orderp;
			int ky=(posy+j) % order;
			temp += xbuff[kx]*xcoeffs[j];
			temp += ybuff[ky]*ycoeffs[j];
		}
		int kx=(posx+order) % orderp;
		temp += xbuff[kx]*xcoeffs[order];
		out[i]= T(temp/gain);
		if (i<(NP-1)){
			xbuff[posx]=in[i+1];
			ybuff[posy]=temp;
		}
		posx = (posx + 1 ) % orderp;
		posy = (posy + 1 ) % order;
	}
}

template <class T>
bool	GeneralFilter<T>::Initialized = false;

//using namespace std;


//complex <double> cmone ( -1.0, 0.0 );
//complex <double> czero (	 0.0, 0.0 );
//complex <double> cone  (	 1.0, 0.0 );
//complex <double> ctwo  (	 2.0, 0.0 );
//complex <double> chalf (	 0.5, 0.0);

//complex <double> bessel_poles[33];
//	bessel_poles[0] = complex <double>( -1.000000e+00,  0.000000e+00 );



//complex <double> bessel_poles[30]; //= {    complex <double>( -1.000000e+00,  0.000000e+00 )}

//bessel_poles[0]=complex <double>( -1.000000e+00,  0.000000e+00 );
	/*

    ( -1.000000e+00,  0.000000e+00 ),	 { -8.660254e-01, -5.000000e-01 ),    { -9.416000e-01,	0.000000e+00 ),
  ( -7.456404e-01, -7.113666e-01 ),	 ( -9.047588e-01, -2.709187e-01 ),    ( -6.572112e-01, -8.301614e-01 ),
  ( -9.264421e-01,  0.000000e+00 ),	 ( -8.515536e-01, -4.427175e-01 ),    ( -5.905759e-01, -9.072068e-01 ),
    ( -9.093907e-01, -1.856964e-01 ),	 ( -7.996542e-01, -5.621717e-01 ),    ( -5.385527e-01, -9.616877e-01 ),
  ( -9.194872e-01,  0.000000e+00 ),	 ( -8.800029e-01, -3.216653e-01 ),    ( -7.527355e-01, -6.504696e-01 ),
   ( -4.966917e-01, -1.002509e+00 ),	 ( -9.096832e-01, -1.412438e-01 ),    ( -8.473251e-01, -4.259018e-01 )
   ( -7.111382e-01, -7.186517e-01 ),	 ( -4.621740e-01, -1.034389e+00 ),    ( -9.154958e-01,	0.000000e+00 ),
   ( -8.911217e-01, -2.526581e-01 ),	 ( -8.148021e-01, -5.085816e-01 ),    ( -6.743623e-01, -7.730546e-01 ),
  ( -4.331416e-01, -1.060074e+00 ),	 ( -9.091347e-01, -1.139583e-01 ),    ( -8.688460e-01, -3.430008e-01 ),
  ( -7.837694e-01, -5.759148e-01 ),	 ( -6.417514e-01, -8.175836e-01 ),    ( -4.083221e-01, -1.081275e+00 ),
 };
 */



#define unless(x)   if(!(x))
#define until(x)    while(!(x))
#define ifix(x)	    (int) (((x) >= 0.0) ? (x) + 0.5 : (x) - 0.5)
#define seq(s1,s2)  (strcmp(s1,s2) == 0)

template <class T>
void GeneralFilter<T>::Init(){

	bessel_poles[0] = complex <double>( -1.000000e+00,  0.000000e+00 );
	bessel_poles[1] = complex <double> ( -8.660254e-01, -5.000000e-01 );
	bessel_poles[2] = complex <double> ( -9.416000e-01,	0.000000e+00 );

	bessel_poles[3] = complex <double> ( -7.456404e-01, -7.113666e-01 );
	bessel_poles[4] = complex <double> ( -9.047588e-01, -2.709187e-01 );
	bessel_poles[5] = complex <double> ( -6.572112e-01, -8.301614e-01 );

	bessel_poles[6] = complex <double>  ( -9.264421e-01,  0.000000e+00 );
	bessel_poles[7] = complex <double>( -8.515536e-01, -4.427175e-01 );
	bessel_poles[8] = complex <double>( -5.905759e-01, -9.072068e-01 );

	bessel_poles[9] = complex <double>  ( -9.093907e-01, -1.856964e-01 );
	bessel_poles[10] = complex <double>( -7.996542e-01, -5.621717e-01 );
	bessel_poles[11] = complex <double>( -5.385527e-01, -9.616877e-01 );

	bessel_poles[12] = complex <double> ( -9.194872e-01,  0.000000e+00 );
	bessel_poles[13] = complex <double>( -8.800029e-01, -3.216653e-01 );
	bessel_poles[14] = complex <double>( -7.527355e-01, -6.504696e-01 );

	bessel_poles[15] = complex <double>  ( -4.966917e-01, -1.002509e+00 );
	bessel_poles[16] = complex <double>( -9.096832e-01, -1.412438e-01 );
	bessel_poles[17] = complex <double>( -8.473251e-01, -4.259018e-01 );

	bessel_poles[18] = complex <double>   ( -7.111382e-01, -7.186517e-01 );
	bessel_poles[19] = complex <double>( -4.621740e-01, -1.034389e+00 );
	bessel_poles[20] = complex <double>( -9.154958e-01,	0.000000e+00 );

	bessel_poles[21] = complex <double>  ( -8.911217e-01, -2.526581e-01 );
	bessel_poles[22] = complex <double>( -8.148021e-01, -5.085816e-01 );
	bessel_poles[23] = complex <double>( -6.743623e-01, -7.730546e-01 );

	bessel_poles[24] = complex <double>  ( -4.331416e-01, -1.060074e+00 );
	bessel_poles[25] = complex <double>( -9.091347e-01, -1.139583e-01 );
	bessel_poles[26] = complex <double>( -8.688460e-01, -3.430008e-01 );

	bessel_poles[27] = complex <double>  ( -7.837694e-01, -5.759148e-01 );
	bessel_poles[28] = complex <double>( -6.417514e-01, -8.175836e-01 );
	bessel_poles[29] = complex <double>( -4.083221e-01, -1.081275e+00 );
}

template <class T>
void GeneralFilter<T>::Test(){
   // The first constructor specifies real & imaginary parts
   complex <double> c1 ( 0.0 , PI_GENERAL_FILTER);
   complex <double> x =cexp(c1);
   ShowFloat(x.real(), "r");
   ShowFloat(x.imag(), "imag");
}
template <class T>
GeneralFilter<T>::~GeneralFilter(){
}
template <class T>
GeneralFilter<T>::GeneralFilter(){
	if (!Initialized){
		Init();
		Initialized = true;
	}
}
template <class T>
GeneralFilter<T>::GeneralFilter(int Characteristic, //0: Bessel; 1: butterw.; 2: cheb.
				int Lowhiband, //0: low pass; 1: hipass; 2: cheb
				int Order,
				double F1,
				double F2,
				double Chebrip){
		if (!Initialized){
			Init();
			Initialized = true;
		}
		Define(Characteristic, Lowhiband, Order, F1, F2, Chebrip);
}

template <class T>
void GeneralFilter<T>::Define(int Characteristic, //0: Bessel; 1: butterw.; 2: cheb.
				int Lowhiband, //0: low pass; 1: hipass; 2: cheb
				int Order,
				double F1,
				double F2,
				double Chebrip){

		order = polemask = 0;
		polemask=-1;
		order = Order;
		if (order<1) order =1;
		if (order>10) order = 10;
		characteristic=Characteristic;
		if (characteristic<0) characteristic = 0; 
		if (characteristic>2) characteristic = 2; 
		lohiband = Lowhiband;
		if (lohiband<0) lohiband = 0;
		if (lohiband>2) lohiband = 2;
		raw_alpha1=F1;
		raw_alpha2=F2;
		if (lohiband!=2){
			raw_alpha2=raw_alpha1;
		}
		chebrip = Chebrip;
		if (chebrip>0.0){
			chebrip = -0.1;
		}
		polemask = -1;

		Compute_s();
	    normalize();
		compute_z();
		expandpoly();

//		ShowFloat(this->warped_alpha1, "wa1");
//		ShowFloat(this->warped_alpha2, "wa2");

//		ShowFloat(cnorm(dc_gain), "dcnorm");



}

template <class T>
void GeneralFilter<T>::Compute_s(){
   numpoles = 0;
   if (characteristic==0)
      { /* Bessel filter */
	int i;
	int p = (order*order)/4; /* ptr into table */
	if (order & 1) choosepole(bessel_poles[p++]);
	for (i=0; i < order/2; i++)
	  { choosepole(bessel_poles[p]);
	    choosepole(cconj(bessel_poles[p]));
	    p++;
	  }
    }

   if ( ( characteristic==1)  || (characteristic==2))
      { /* Butterworth filter */
	int i;
	for (i=0; i < 2*order; i++)
	  { complex <double> s(0, 
	  (order & 1) ? (i*PI_GENERAL_FILTER) / order : ((i+0.5)*PI_GENERAL_FILTER) / order);
//		complex <double> zz = cexp(s);
	    choosepole(cexp(s));
	  }
    }
    if ((characteristic==2))
      { /* modify for Chebyshev (p. 136 DeFatta et al.) */
	double rip, eps, y; int i;
/*
if (chebrip >= 0.0)
	  { fprintf(stderr, "mkfilter: Chebyshev ripple is %g dB; must be .lt. 0.0\n", chebrip);
	    exit(1);
	  }
*/
	rip = pow(10.0, -chebrip / 10.0);
	eps = sqrt(rip - 1.0);
	y = asinh(1.0 / eps) / (double) order;
	if (y <= 0.0)
	  { fprintf(stderr, "mkfilter: bug: Chebyshev y=%g; must be .gt. 0.0\n", y);
	    exit(1);
	  }
	for (i=0; i < numpoles; i++)
	  { 
		  double rr=spoles[i].real();//.real;
		  double ii=spoles[i].imag();
//		  spoles[i].real = spoles[i].real()*sinh(y);
		  spoles[i]=complex<double>(rr* sinh(y), ii*cosh(y));
//		  spoles[i].imag = spoles[i].imag()*cosh(y);
		 // spoles[i].real *= sinh(y);
		   // spoles[i].imag *= cosh(y);
	  }
      }
}

template <class T>
void GeneralFilter<T>::choosepole(complex <double> z){
  if (z.real() < 0.0)
      { if (polemask & 1) spoles[numpoles++] = z;
			polemask >>= 1;
   }
}



template <class T>
void GeneralFilter<T>::normalize(){
   int i;
    /* for bilinear transform, perform pre-warp on alpha values */
//    if (opts & opt_w)
    if (false)
//    if (true)
      { warped_alpha1 = raw_alpha1;
	warped_alpha2 = raw_alpha2;
      }
    else
      { warped_alpha1 = tan(PI_GENERAL_FILTER * raw_alpha1) / PI_GENERAL_FILTER;
	warped_alpha2 = tan(PI_GENERAL_FILTER * raw_alpha2) / PI_GENERAL_FILTER;
      }
	complex <double> w1 (TWOPI_GENERAL_FILTER * warped_alpha1, 0.0);
    complex <double> w2 (TWOPI_GENERAL_FILTER * warped_alpha2, 0.0);
    /* transform prototype into appropriate filter type (lp/hp/bp) */
//	ShowFloat(numpoles, "numpoles");
    switch (lohiband)
      { case 0:
	    for (i=0; i < numpoles; i++) spoles[i] = spoles[i]* w1;
//		Alert0("hi");
	    break;

	case 1:
	    for (i=0; i < numpoles; i++) spoles[i] = w1 / spoles[i];
	    /* also N zeros at (0,0) */
	    break;

	case 2:
	  { complex <double> w0, bw;
	    w0 = csqrt(w1*w2);
	    bw = w2-w1;
	    for (i=0; i < numpoles; i++)
	      { complex <double> hba, temp;
		complex <double> chalf (	 0.5, 0.0);
		hba = chalf* spoles[i]* bw;
		temp = w0 / hba;
		complex <double> cone  (	 1.0, 0.0 );
		temp = csqrt(cone- temp* temp);
		spoles[i] = hba*(cone+ temp);
		spoles[numpoles+i] = hba *(cone- temp);
	      }
	    /* also N zeros at (0,0) */
	    numpoles *= 2;
	    break;
	  }
     }

}
template <class T>
void GeneralFilter<T>::compute_z(){
   int i;
    for (i=0; i < numpoles; i++)
      { /* use bilinear transform */
//	struct complex top, bot;
	complex <double> ctwo  (	 2.0, 0.0 );
	complex <double> top = ctwo + spoles[i];
	complex <double> bot = ctwo - spoles[i];
	zpoles[i] = top / bot;
	complex <double> cmone ( -1.0, 0.0 );

	complex <double> cone  (	 1.0, 0.0 );
	switch (lohiband)
	  { case 0:    zzeros[i] = cmone; break;
	    case 1:    zzeros[i] = cone; break;
	    case 2:    zzeros[i] = (i & 1) ? cone : cmone; break;
	  }
	}
}


template <class T>
void GeneralFilter<T>::expandpoly(){
	complex <double> topcoeffs[MAXPOLES_GENERAL_FILTER+1];
	complex <double> botcoeffs[MAXPOLES_GENERAL_FILTER+1];
	int i;
    expand(zzeros, topcoeffs);
    expand(zpoles, botcoeffs);
	complex <double> cone  (	 1.0, 0.0 );
    dc_gain = evaluate(topcoeffs, botcoeffs, numpoles, cone);
    complex <double> st(0.0, TWOPI_GENERAL_FILTER * 0.5 * (raw_alpha1 + raw_alpha2)); /* "jwT" for centre freq. */
    complex <double> zfc = cexp(st);
    fc_gain = evaluate(topcoeffs, botcoeffs, numpoles, zfc);
	complex <double> cmone ( -1.0, 0.0 );

    hf_gain = evaluate(topcoeffs, botcoeffs, numpoles, cmone);
    for (i=0; i <= numpoles; i++)
      { xcoeffs[i] = topcoeffs[i].real() / botcoeffs[numpoles].real();
	ycoeffs[i] = -(botcoeffs[i].real() / botcoeffs[numpoles].real());
      }

}

template <class T>
void GeneralFilter<T>::multin(complex <double> w, complex <double> * coeffs)
  { /* multiply factor (z-w) into coeffs */
    int i;
	complex <double> czero (	 0.0, 0.0 );
    complex <double> nw = czero-w;
	
    for (i=numpoles; i >= 1; i--)
      coeffs[i] = nw * coeffs[i]+ coeffs[i-1];
    coeffs[0] = nw * coeffs[0];
  }

template <class T>
void GeneralFilter<T>::expand(complex <double> * pz, complex <double> * coeffs){
   /* compute product of poles or zeros as a polynomial of z */
    int i;
	complex <double> cone  (	 1.0, 0.0 );
    coeffs[0] = cone;
	complex <double> czero (	 0.0, 0.0 );
    for (i=0; i < numpoles; i++) coeffs[i+1] = czero;
    for (i=0; i < numpoles; i++) multin(pz[i], coeffs);
    /* check computed coeffs of z^k are all real */
    for (i=0; i < (numpoles+1); i++)
      { if (fabs(coeffs[i].imag()) > EPS_GENERAL_FILTER)
	  { fprintf(stderr, "mkfilter: coeff of z^%d is not real; poles are not complex conjugates\n", i);
	    exit(1);
	  }
      }
  }



#endif
