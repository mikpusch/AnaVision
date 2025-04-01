#pragma once

#include <complex>
#include <math.h>
#include "utils.h"

using namespace std;

#define global
#define unless(x) if(!(x))
#define until(x)  while(!(x))

#define VERSION_MAKEFILTER   "4.6"
#undef  PI_MAKEFILTER
// Microsoft C++ does not define M_PI !
#define PI_MAKEFILTER        3.14159265358979323846
#define TWOPI_MAKEFILTER     (2.0 * PI)
#define EPS_MAKEFILTER       1e-10
#define MAXORDER_MAKEFILTER  10
#define MAXPZ_MAKEFILTER     512
// .ge. 2*MAXORDER, to allow for doubling of poles in BP filter;
// high values needed for FIR filters
#define MAXSTRING_MAKEFILTER   256

typedef void (*proc)();
//typedef unsigned int uint;

extern const char *progname;
extern void readdata(char*, double&, int&, double*, int&, double*);

#define opt_be_MAKEFILTER 0x00001		 /* -Be Bessel characteristic          */
#define opt_bu_MAKEFILTER 0x00002		 /* -Bu Butterworth characteristic     */
#define opt_ch_MAKEFILTER 0x00004		 /* -Ch Chebyshev characteristic       */
#define opt_re_MAKEFILTER 0x00008		 /* -Re Resonator                      */
#define opt_pi_MAKEFILTER 0x00010		 /* -Pi proportional-integral          */

#define opt_lp_MAKEFILTER 0x00020		 /* -Lp lowpass                        */
#define opt_hp_MAKEFILTER 0x00040		 /* -Hp highpass                       */
#define opt_bp_MAKEFILTER 0x00080		 /* -Bp bandpass                       */
#define opt_bs_MAKEFILTER 0x00100		 /* -Bs bandstop                       */
#define opt_ap_MAKEFILTER 0x00200		 /* -Ap allpass                        */

#define opt_a_MAKEFILTER  0x00400		 /* -a alpha value                     */
#define opt_l_MAKEFILTER  0x00800		 /* -l just list filter parameters     */
#define opt_o_MAKEFILTER  0x01000		 /* -o order of filter                 */
#define opt_p_MAKEFILTER  0x02000		 /* -p specified poles only            */
#define opt_w_MAKEFILTER  0x04000		 /* -w don't pre-warp                  */
#define opt_z_MAKEFILTER  0x08000		 /* -z use matched z-transform         */
#define opt_Z_MAKEFILTER  0x10000		 /* -Z additional zero                 */

struct pzrep
{
	complex<double> poles[MAXPZ_MAKEFILTER];
	complex<double> zeros[MAXPZ_MAKEFILTER];
	int numpoles, numzeros;
};

class MakeFilter{
public:

	MakeFilter();
	~MakeFilter();
	void Define(int Characteristic, //0: Bessel; 1: butterw.; 2: cheb.
				int Lowhiband, //0: low pass; 1: hipass; 2: bandpass
				int Order,
				double F1,
				double F2=0.0,
				double Chebrip=-0.1);

	void Test();

	void DoFilter(double * in, double * out, int NP);

	bool IsInitialized;

	pzrep splane, zplane;
	int order;
	double raw_alpha1, raw_alpha2, raw_alphaz;
	complex<double> dc_gain, fc_gain, hf_gain;
	uint options;
	double warped_alpha1, warped_alpha2, chebrip, qfactor;
	bool infq;
	uint polemask;
	double xcoeffs[MAXPZ_MAKEFILTER+1], ycoeffs[MAXPZ_MAKEFILTER+1];

	static complex <double> bessel_poles[30];

};


