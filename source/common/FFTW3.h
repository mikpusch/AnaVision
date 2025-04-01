
/* This is a wrapepr around the FFTW3 library
*/

#ifndef _MIK_FFT3

#define _MIK_FFT3

#include "..\..\FFTW3\fftw-3.3.5-dll32\fftw3.h"

//#define C_FUNC __stdcall

typedef void (__stdcall * Func_fftw_cleanup)();
typedef fftw_complex * (__stdcall * Func_fftw_malloc)(UINT); 


class FFTW3{
public:

	FFTW3();
	~FFTW3();

	void wiener_filter(fftw_complex *y, fftw_complex *H, fftw_complex *h, double noise_variance, double signal_variance, int N);
	void add_noise(double *signal, double *noisy_signal, int N, double noise_level);
	void rc_filter_impulse_response(double *h, double dt, double tau, int N) ;

	bool Deconvolute(double * input, double dt, double tau, int N);

	static bool Initialized;
	bool Initialize();
	static HINSTANCE hDLL;
	bool CheckProcedureAddress(DWORD adr, CString name, HINSTANCE hDll);

	static Func_fftw_cleanup fftw_cleanup;
	static Func_fftw_malloc fftw_malloc;

};

#endif