// This file implements the cg_descent class


#include "stdafx.h"
#include "../common/FFTW3.h"
#include "..\..\FFTW3\fftw-3.3.5-dll32/fftw3.h"


bool		FFTW3::Initialized = false;
HINSTANCE	FFTW3::hDLL = NULL;

#define GET_FFT_FUNC(FUNCSTRING) \
	FUNCSTRING = (Func_##FUNCSTRING)GetProcAddress(hDLL, #FUNCSTRING); \
	if  (!CheckProcedureAddress(DWORD(FUNCSTRING), #FUNCSTRING, hDLL)){ \
		return false; \
	} 

#define NULL_FFT_FUNC(FUNCSTRING)  Func_##FUNCSTRING FFTW3::##FUNCSTRING = NULL ;

NULL_FFT_FUNC(fftw_cleanup);
NULL_FFT_FUNC(fftw_malloc);



FFTW3::FFTW3(){
	if (!Initialized){
		Initialize();
	}
}
FFTW3::~FFTW3(){
	if (!Initialized){
		return;
	}
	FreeLibrary(hDLL);
	hDLL = NULL;
	Initialized = false;
}

bool FFTW3::Initialize(){
	if (Initialized){
		return true;
	}
	HINSTANCE hDLL=LoadLibrary( "libfftw3-3.dll");
	if (!hDLL){
		Alert0("RROR: could not find libfftw3-3.dll");
		return false;
	}
	GET_FFT_FUNC(fftw_cleanup);
	GET_FFT_FUNC(fftw_malloc);
	Initialized = true;
//	Alert0("init");
	return true;
}

bool FFTW3::CheckProcedureAddress(DWORD adr, CString name, HINSTANCE hDll){
	if (adr) return true;
	CString errorstring=CString("Error in getting address of ");
	errorstring += name;
	DWORD err=GetLastError();
	char s[20];
	_itoa(err, s, 10);
	errorstring += CString("; error: ")+CString(s);
	Alert0(errorstring);
//	Beep(1000,30);
	FreeLibrary(hDll);
	return false;
}


void FFTW3::rc_filter_impulse_response(double *h, double dt, double tau, int N) {
//    double tau = R * C;  // Time constant
    for (int i = 0; i < N; i++) {
        h[i] = (1.0 / tau) * exp(-i * dt / tau);  // Impulse response formula
    }
}

// Wiener Filter function
void FFTW3::wiener_filter(fftw_complex *y, fftw_complex *H, fftw_complex *h, double noise_variance, double signal_variance, int N) {
    for (int i = 0; i < N; i++) {
        // H* / (|H|^2 + noise_variance / signal_variance)
        double real_H = H[i][0];
        double imag_H = H[i][1];
        double abs_H2 = real_H * real_H + imag_H * imag_H;

        // Wiener filter in frequency domain
        double real_Y = y[i][0];
        double imag_Y = y[i][1];
        
        double numerator_real = real_H * real_Y + imag_H * imag_Y;
        double numerator_imag = imag_H * real_Y - real_H * imag_Y;
        double denominator = abs_H2 + noise_variance / signal_variance;

        // Apply Wiener filter
        h[i][0] = numerator_real / denominator;
        h[i][1] = numerator_imag / denominator;
    }
}

bool FFTW3::Deconvolute(double * input, double dt, double tau, int N){

    double *signal = (double*)malloc(N * sizeof(double));
    double *noisy_signal = (double*)malloc(N * sizeof(double));
    double *impulse_response = (double*)malloc(N * sizeof(double));

	fftw_complex *y = (fftw_complex*)fftw_malloc(N * sizeof(fftw_complex));  // Observed signal in frequency domain
    fftw_complex *H = (fftw_complex*)fftw_malloc(N * sizeof(fftw_complex));  // RC filter transfer function in frequency domain
    fftw_complex *h = (fftw_complex*)fftw_malloc(N * sizeof(fftw_complex));  // Output of Wiener filter (estimated signal)

    // Simulate a step input signal (simple test case)
    for (int i = 0; i < N; i++) {
        signal[i] = input[i];
    }

    // Compute the RC filter impulse response
    rc_filter_impulse_response(impulse_response, dt, tau, N);

    for (int i = 0; i < N; i++) {
        noisy_signal[i] = signal[i];
	}
    // Convolve the signal with the RC filter (in the time domain)
/*    for (int i = 0; i < N; i++) {
        noisy_signal[i] = 0;
        for (int j = 0; j <= i; j++) {
            noisy_signal[i] += signal[j] * impulse_response[i - j];
        }
    }

    // Add noise to the observed signal
    double noise_level = 0.1;
    add_noise(noisy_signal, noisy_signal, N, noise_level);
*/

    // Perform FFT on the noisy observed signal and the filter impulse response
/*
    fftw_plan plan_Y = fftw_plan_dft_r2c_1d(N, noisy_signal, y, FFTW_ESTIMATE);
    fftw_plan plan_H = fftw_plan_dft_r2c_1d(N, impulse_response, H, FFTW_ESTIMATE);

    fftw_execute(plan_Y);
    fftw_execute(plan_H);

    // Perform Wiener filtering in the frequency domain
    double noise_variance = 0.1;
    double signal_variance = 1.0;
    wiener_filter(y, H, h, noise_variance, signal_variance, N);

    // Perform IFFT to get the estimated signal in the time domain
    fftw_plan plan_ifft = fftw_plan_dft_c2r_1d(N, h, noisy_signal, FFTW_ESTIMATE);
    fftw_execute(plan_ifft);

    // Normalize the estimated signal
    for (int i = 0; i < N; i++) {
        noisy_signal[i] /= N;
    }

    // Print the estimated signal (for simplicity, printing first few values)
//    printf("Estimated Signal (Wiener Filter):\n");
//    for (int i = 0; i < 10; i++) {
//        printf("%lf\n", noisy_signal[i]);
//    }

    // Clean up
    free(signal);
    free(noisy_signal);
    free(impulse_response);
    fftw_free(y);
    fftw_free(H);
    fftw_free(h);
*/
    fftw_cleanup();

	return true;
}
