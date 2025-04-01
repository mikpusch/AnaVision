#pragma once

//#include "dcompl.h"
#include <complex.h>

class ComplexEV{
public:
  ComplexEV();
  ~ComplexEV();
	static double my_cabs(complex<double> c);
  // output matrices and vectors have to be allocated by user 
  void CalcEigenValues(double ** matrix, int n,
						complex<double> * EigenValues,
						bool * EigenValueIsReal);

  void CalcEigenValuesAndVectors
	  (double ** matrix, int n,
	  complex<double> * EigenValues,
	  complex<double> ** EigenVectors,
	  bool * EigenValueIsReal);

  void CalcEigenValues(double ** matrix, int n,
						double * EigenValues,
						bool * EigenValueIsReal);
  // here,if EV is not real: EigenValue[i] = real part,EigenValue[i+1] = imag part, 

  void CalcEigenValuesAndVectors
	  (double ** matrix, int n,
	  double * EigenValues,
	  double ** EigenVectors,
	  bool * EigenValueIsReal);

  void CalcEigenValuesAndVectors
	  (double ** matrix, int n,
	  double * EigenValues,
	  double ** REigenVectors,
	  double ** LEigenVectors,
	  bool * EigenValueIsReal);
  // here,if EV is not real: EigenValue[i] = real part,EigenValue[i+1] = imag part, 

  bool MatrixIsDefective(double ** mat, int n);

  
  bool CheckDefective(int n, int istart, int iend, complex<double> ** EigenVectors);

private:

	void DoCalcEigenValuesAndVectors
	  (bool CalcEigenVecs,
	  double ** mat, int n,
	  complex<double> * EigenValues,
	  complex<double> ** EigenVectors,
	  bool * EigenValueIsReal);

	void DoCalcEigenValuesAndVectors
	  (bool CalcEigenVecs,
	  bool CalcAlsoLeftEV, 
	  double ** mat, int n,
	  double * EigenValues,
	  double ** REigenVectors,
	  double ** LEigenVectors,
	  bool * EigenValueIsReal);

//	bool CheckDefective(int n, int istart, int iend, complex<double> ** EigenVectors);

	complex<double> my_conj(complex<double> c);

};
