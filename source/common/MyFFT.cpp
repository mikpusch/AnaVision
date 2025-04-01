// This file implements the cg_descent class


#include "stdafx.h"
#include "../common/MyFFT.h"
#include <complex.h>


MyFFT::MyFFT(){
}
MyFFT::~MyFFT(){
}
void MyFFT::Oldfft(double *real, double *imag, int N) {
    int i, j, k, m;
    double temp_real, temp_imag, angle, cos_angle, sin_angle;
    
    // Bit reversal permutation  
    for (i = 1, j = 0; i < N; i++) {
        for (m = N / 2; m > (j ^= m); m /= 2);
        if (i < j) {
            temp_real = real[i];
            temp_imag = imag[i];
            real[i] = real[j];
            imag[i] = imag[j];
            real[j] = temp_real;
            imag[j] = temp_imag;
        }
    }

    // FFT computation (Cooley-Tukey)
    for (m = 2; m <= N; m *= 2) {
        angle = -2.0 * PI_GENERAL_FILTER / m;
        cos_angle = cos(angle);
        sin_angle = sin(angle);
        for (i = 0; i < N; i += m) {
            for (k = 0; k < m / 2; k++) {
                temp_real = real[i + k + m / 2] * cos_angle - imag[i + k + m / 2] * sin_angle;
                temp_imag = real[i + k + m / 2] * sin_angle + imag[i + k + m / 2] * cos_angle;
                real[i + k + m / 2] = real[i + k] - temp_real;
                imag[i + k + m / 2] = imag[i + k] - temp_imag;
                real[i + k] += temp_real;
                imag[i + k] += temp_imag;
            }
        }
    }
}

void MyFFT::Oldifft(double *real, double *imag, int N) {
    // Inverse FFT is just the complex conjugate of the FFT, with a normalization factor
    for (int i = 0; i < N; i++) {
        imag[i] = -imag[i];
    }
    fft(real, imag, N);
    for (int i = 0; i < N; i++) {
        real[i] /= N;
        imag[i] /= N;
    }
}


void MyFFT::fft(double *realpart, double *imagpart, int len) {

//void fft( complex *v, int n, complex *tmp )
//{
	if (len<2) return;

	double * Xr = new double[len];
	double * Xi = new double[len];
    for (int k = 0; k < len; k++) {
        Xr[k] = 0;
        Xi[k] = 0;
		double mult = 2.0 * PI_GENERAL_FILTER * double(k) / double(len);
		for (int n = 0; n < len; n++) {
			double m2 = mult * double(n);
			double c = cos(m2);
			double s = sin(m2);
			double r = realpart[n];
			double i = imagpart[n];
			Xr[k] = Xr[k] + r*c + i*s;
			Xi[k] = Xi[k] + i*c - r*s;

//            Xr[k] = (Xr[k] + xn[n] * cos(2.0 * PI_GENERAL_FILTER * double(k) * double(n) / double(N));
//            Xi[k] = (Xi[k] - xn[n] * sin(2.0 * PI_GENERAL_FILTER * double(k) * double(n) / double(N));
        }
 
//        printf("(%f) + j(%f)\n", Xr[k], Xi[k]);
    }
    for (int k = 0; k < len; k++) {
        realpart[k] = Xr[k];
        imagpart[k] = Xi[k];
	}

	delete [] Xi;
	delete [] Xr;

/*	Complex * tmp = new Complex[n];
	Complex * v = new Complex[n];
	for (int ll=0; ll<n; ll++){
		v[ll]._Val[0] = realpart[ll];
		v[ll]._Val[1] = imagpart[ll];
	}
	Dofft(v, n, tmp);
	delete [] tmp;
	delete [] v;
*/
}

void MyFFT::ifft(double *realpart, double *imagpart, int len) {
	if (len<2) return;

	double * Xr = new double[len];
	double * Xi = new double[len];
    for (int k = 0; k < len; k++) {
        Xr[k] = 0;
        Xi[k] = 0;
		double mult = 2.0 * PI_GENERAL_FILTER * double(k) / double(len);
		for (int n = 0; n < len; n++) {
			double m2 = mult * double(n);
			double c = cos(m2);
			double s = sin(m2);
			double r = realpart[n];
			double i = imagpart[n];
			Xr[k] = Xr[k] + r*c - i*s;
			Xi[k] = Xi[k] + i*c + r*s;

//            Xr[k] = (Xr[k] + xn[n] * cos(2.0 * PI_GENERAL_FILTER * double(k) * double(n) / double(N));
//            Xi[k] = (Xi[k] - xn[n] * sin(2.0 * PI_GENERAL_FILTER * double(k) * double(n) / double(N));
        }
 
//        printf("(%f) + j(%f)\n", Xr[k], Xi[k]);
    }
	double scale = 1.0/double(len);
    for (int k = 0; k < len; k++) {
        realpart[k] = Xr[k]*scale;
        imagpart[k] = Xi[k]*scale;
	}

	delete [] Xi;
	delete [] Xr;

//void fft( complex *v, int n, complex *tmp )
//{
/*	if (n<2) return;
	Complex * tmp = new Complex[n];
	Complex * v = new Complex[n];
	for (int ll=0; ll<n; ll++){
		v[ll]._Val[0] = realpart[ll];
		v[ll]._Val[1] = imagpart[ll];
	}
	Doifft(v, n, tmp);
	delete [] tmp;
	delete [] v;
*/
}

void MyFFT::Dofft(Complex * v, int n, Complex * tmp){
	if (n<2) return;
    int k,m;
	Complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    Dofft( ve, n/2, v );		///* FFT on even-indexed elements of v[]
    Dofft( vo, n/2, v );		///* FFT on odd-indexed elements of v[] 
    for(m=0; m<n/2; m++) {
      w._Val[0] = cos(2*PI_GENERAL_FILTER*m/(double)n);
      w._Val[1] = -sin(2*PI_GENERAL_FILTER*m/(double)n);
      z._Val[0] = w._Val[0]*vo[m]._Val[0] - w._Val[1]*vo[m]._Val[1];	///* Re(w*vo[m]) 
      z._Val[1] = w._Val[0]*vo[m]._Val[1] + w._Val[1]*vo[m]._Val[0];	///* Im(w*vo[m]) 
      v[  m  ]._Val[0] = ve[m]._Val[0] + z._Val[0];
      v[  m  ]._Val[1] = ve[m]._Val[1] + z._Val[1];
      v[m+n/2]._Val[0] = ve[m]._Val[0] - z._Val[0];
      v[m+n/2]._Val[1] = ve[m]._Val[1] - z._Val[1];
    }

}
void MyFFT::Doifft(Complex * v, int n, Complex * tmp){
//void ifft( complex *v, int n, complex *tmp )
	if (n<2) return;
    int k,m;    Complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    Doifft( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
    Doifft( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
    for(m=0; m<n/2; m++) {
      w._Val[0] = cos(2*PI_GENERAL_FILTER*m/(double)n);
      w._Val[1] = sin(2*PI_GENERAL_FILTER*m/(double)n);
      z._Val[0] = w._Val[0]*vo[m]._Val[0] - w._Val[1]*vo[m]._Val[1];	/* Re(w*vo[m]) */
      z._Val[1] = w._Val[0]*vo[m]._Val[1] + w._Val[1]*vo[m]._Val[0];	/* Im(w*vo[m]) */
      v[  m  ]._Val[0] = ve[m]._Val[0] + z._Val[0];
      v[  m  ]._Val[1] = ve[m]._Val[1] + z._Val[1];
      v[m+n/2]._Val[0] = ve[m]._Val[0] - z._Val[0];
      v[m+n/2]._Val[1] = ve[m]._Val[1] - z._Val[1];
    }
}


void MyFFT::wiener_filter(double *y_real, double *y_imag, double *H_real, double *H_imag, double *h_real, double *h_imag, double noise_variance, double signal_variance, int N) {
    for (int i = 0; i < N; i++) {
        // H* / (|H|^2 + noise_variance / signal_variance)
        double real_H = H_real[i];
        double imag_H = H_imag[i];
        double abs_H2 = real_H * real_H + imag_H * imag_H;

        // Wiener filter in frequency domain
        double real_Y = y_real[i];
        double imag_Y = y_imag[i];
        
        double numerator_real = real_H * real_Y + imag_H * imag_Y;
        double numerator_imag = imag_H * real_Y - real_H * imag_Y;
        double denominator = abs_H2 + noise_variance / signal_variance;

        // Apply Wiener filter
        h_real[i] = numerator_real / denominator;
        h_imag[i] = numerator_imag / denominator;
    }
}

// Function to add noise to the signal
void MyFFT::add_noise(double *signal, double *noisy_signal, int N, double noise_level) {
    for (int i = 0; i < N; i++) {
        noisy_signal[i] = signal[i] + noise_level * ((double)rand() / RAND_MAX - 0.5);
    }
}
void MyFFT::rc_filter_impulse_response(double *h, double dt, double tau, int N) {
//    double tau = R * C;  // Time constant
    for (int i = 0; i < N; i++) {
        h[i] = (1.0 / tau) * exp(-i * dt / tau);  // Impulse response formula
    }
}


bool MyFFT::Deconvolute(double * input, double dt, double tau, int N){
//	ShowFloat(N, "N");
//	ShowFloat(dt, "dt");
//	ShowFloat(tau, "tau");
    double *signal = (double*)malloc(N * sizeof(double));
    double *noisy_signal = (double*)malloc(N * sizeof(double));
    double *impulse_response = (double*)malloc(N * sizeof(double));
    
    double *y_real = (double*)malloc(N * sizeof(double));  // Observed signal real part
    double *y_imag = (double*)malloc(N * sizeof(double));  // Observed signal imaginary part
    double *H_real = (double*)malloc(N * sizeof(double));  // Filter transfer function real part
    double *H_imag = (double*)malloc(N * sizeof(double));  // Filter transfer function imaginary part
    double *h_real = (double*)malloc(N * sizeof(double));  // Wiener filter output real part
    double *h_imag = (double*)malloc(N * sizeof(double));  // Wiener filter output imaginary part

    // Simulate a step input signal (simple test case)
    for (int i = 0; i < N; i++) {
        signal[i] = input[i];
    }
    // Compute the RC filter impulse response
    rc_filter_impulse_response(impulse_response, dt, tau, N);
    for (int i = 0; i < N; i++) {
        noisy_signal[i] = signal[i];
	}

/*
// Convolve the signal with the RC filter (in the time domain)
    for (int i = 0; i < N; i++) {
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
	for (int i = 0; i < N; i++) {
        y_real[i] = noisy_signal[i];
        y_imag[i] = 0.0;
        H_real[i] = impulse_response[i];
        H_imag[i] = 0.0;
    }
    fft(y_real, y_imag, N);
    fft(H_real, H_imag, N);


    // Perform Wiener filtering in the frequency domain
    double noise_variance = 0.1;
    double signal_variance = 0.1; //1.0;
    wiener_filter(y_real, y_imag, H_real, H_imag, h_real, h_imag, noise_variance, signal_variance, N);

    // Perform IFFT to get the estimated signal in the time domain
    ifft(h_real, h_imag, N);

    // Print the estimated signal (for simplicity, printing first few values)
/*    printf("Estimated Signal (Wiener Filter):\n");
    for (int i = 0; i < 10; i++) {
        printf("%lf\n", h_real[i]);
    }
*/
    for (int i = 0; i < N; i++) {
       input[i] = h_real[i];
    }

    // Clean up
    free(signal);
    free(noisy_signal);
    free(impulse_response);
    free(y_real);
    free(y_imag);
    free(H_real);
    free(H_imag);
    free(h_real);
    free(h_imag);


	return true;
}
