

#ifndef _MIK_FFT

#define _MIK_FFT


class MyFFT{
public:

	MyFFT();
	~MyFFT();

//	void wiener_filter(fftw_complex *y, fftw_complex *H, fftw_complex *h, double noise_variance, double signal_variance, int N);
	void add_noise(double *signal, double *noisy_signal, int N, double noise_level);
	void rc_filter_impulse_response(double *h, double dt, double tau, int N) ;

	void fft(double *real, double *imag, int N);
	void ifft(double *real, double *imag, int N);
	void Dofft(Complex * v, int n, Complex * temp);
	void Doifft(Complex * v, int n, Complex * temp);
	void Oldfft(double *real, double *imag, int N);
	void Oldifft(double *real, double *imag, int N);
	void wiener_filter(double *y_real, double *y_imag,
		double *H_real, double *H_imag, double *h_real, double *h_imag, double noise_variance, double signal_variance, int N);

	bool Deconvolute(double * input, double dt, double tau, int N);

};

#endif