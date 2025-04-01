#include "stdafx.h"

#include <math.h>
#include "utils.h"
#include "nrutil.h"
#include "ComplexEV.h"

#include "../alglib370/cpp/src/ap.h"
#include "../alglib370/cpp/src/alglibinternal.h"
#include "../alglib370/cpp/src/alglibmisc.h"
#include "../alglib370/cpp/src/linalg.h"

#define eps_defective  1e-10


ComplexEV::ComplexEV(){
}

ComplexEV::~ComplexEV(){
}



void ComplexEV::CalcEigenValues(double ** matrix, int n,
						complex<double> * EigenValues,
						bool * EigenValueIsReal){
   DoCalcEigenValuesAndVectors(false,
	   matrix, n, EigenValues, NULL, EigenValueIsReal);
}


void ComplexEV::CalcEigenValuesAndVectors
	  (double ** mat, int n,
	  complex<double> * EigenValues,
	  complex<double> ** EigenVectors,
	  bool * EigenValueIsReal){

   DoCalcEigenValuesAndVectors(true,
	   mat, n, EigenValues, EigenVectors, EigenValueIsReal);
}

bool CheckEigenValueIsReal(complex<double> c){
	#define MIN_EV_IMAG 1.0e-12
//	ShowFloat(c.real(), "r");
//	ShowFloat(c.imag(), "i");

	return (fabs( c.imag() ) <= fabs( c.real()*MIN_EV_IMAG) );

}

void ComplexEV::DoCalcEigenValuesAndVectors	  (bool CalcEigenVecs,
											  double ** mat, int n,
											  complex<double> * EigenValues,
											  complex<double> ** EigenVectors,
											  bool * EigenValueIsReal){

	alglib::real_2d_array a;
	a.allocate_own(n, n, alglib_impl::DT_REAL);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			a[i][j] = mat[i][j];
		}
	}


	alglib::real_1d_array wr;
	wr.allocate_own(n, alglib_impl::DT_REAL);

	alglib::real_1d_array wi;
	wi.allocate_own(n, alglib_impl::DT_REAL);

	alglib::real_2d_array vl;
	alglib::real_2d_array vr;

    int vneeded;
	if (CalcEigenVecs){
		vneeded = 3;
		vl.allocate_own(n, n, alglib_impl::DT_REAL);
		vr.allocate_own(n, n, alglib_impl::DT_REAL);
	}
	else{
		vneeded = 0;
	}

	bool result = alglib::rmatrixevd(
						a,
					    n,
					    vneeded,
					    wr,
					    wi,
					    vl,
					    vr);

	if (!result){
		Alert0("ERROR IN alglib::rmatrixevd");
	}
	for (int i=0; i<n; i++){
		EigenValues[i].real(wr[i]);
		EigenValues[i].imag(wi[i]);

		EigenValueIsReal[i] = CheckEigenValueIsReal(EigenValues[i]);
//		ShowFloat(wr[i], "wr");
//		ShowFloat(wi[i], "wi");

	}

//	if (CalcEigenVecs){
//		for (int i=0; i<n; i++){
//			for (int j=0; j<n; j++){
//				ShowFloat(vr[j][i], "vr[j][i]");
//			}
//		}
//	}

	if (CalcEigenVecs){
		int i=0;
		while (i<n){
			if (EigenValueIsReal[i]){
				for (int j=0; j<n; j++){
					EigenVectors[i][j].imag(0.0);
					EigenVectors[i][j].real(vr[j][i]);
//					ShowFloat(EigenVectors[i][j].real(), "realEV");
				}
				i++;
			}
			else{
				for (int j=0; j<n; j++){

					EigenVectors[i][j].real(vr[j][i]);
					EigenVectors[i][j].imag(vr[j][i+1]);

					EigenVectors[i+1][j].real(EigenVectors[i][j].real());
					EigenVectors[i+1][j].imag(-EigenVectors[i][j].imag());
				}
				i += 2;
			}

		}
		for (int i=0; i<n; i++){ // normalize them
			double norm = 0;
			for (int j=0; j<n; j++){
				double rr=EigenVectors[i][j].real();
				double ii=EigenVectors[i][j].imag();
				norm += rr*rr+ii*ii;
			}
			norm = 1.0/sqrt(norm);
			for (int j=0; j<n; j++){
				EigenVectors[i][j] *= norm;
			}
		}

	}

}

void ComplexEV::CalcEigenValues(double ** matrix, int n,
						double * EigenValues,
						bool * EigenValueIsReal){
   DoCalcEigenValuesAndVectors(false, false,
	   matrix, n, EigenValues, NULL, NULL, EigenValueIsReal);
}


void ComplexEV::CalcEigenValuesAndVectors
	  (double ** mat, int n,
	  double * EigenValues,
	  double ** EigenVectors,
	  bool * EigenValueIsReal){

   DoCalcEigenValuesAndVectors(true, false,
	   mat, n, EigenValues, EigenVectors, NULL, EigenValueIsReal);
}
void ComplexEV::CalcEigenValuesAndVectors
	  (double ** mat, int n,
	  double * EigenValues,
	  double ** EigenVectors,
	  double ** LEigenVectors,
	  bool * EigenValueIsReal){

   DoCalcEigenValuesAndVectors(true, true,
	   mat, n, EigenValues, EigenVectors, LEigenVectors, EigenValueIsReal);
}

void ComplexEV::DoCalcEigenValuesAndVectors	  (bool CalcEigenVecs,
										   	  bool CalcAlsoLeftEV, 
											  double ** mat, int n,
											  double * EigenValues,
											  double ** EigenVectors,
											  double ** LEigenVectors,
											  bool * EigenValueIsReal){

	alglib::real_2d_array a;
	a.allocate_own(n, n, alglib_impl::DT_REAL);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			a[i][j] = mat[i][j];
		}
	}


	alglib::real_1d_array wr;
	wr.allocate_own(n, alglib_impl::DT_REAL);

	alglib::real_1d_array wi;
	wi.allocate_own(n, alglib_impl::DT_REAL);

	alglib::real_2d_array vl;
	alglib::real_2d_array vr;

    int vneeded;
	if (CalcEigenVecs){
		vneeded = 3;
		vl.allocate_own(n, n, alglib_impl::DT_REAL);
		vr.allocate_own(n, n, alglib_impl::DT_REAL);
	}
	else{
		vneeded = 0;
	}

	bool result = alglib::rmatrixevd(
						a,
					    n,
					    vneeded,
					    wr,
					    wi,
					    vl,
					    vr);

	if (!result){
		Alert0("ERROR IN alglib::rmatrixevd");
	}
	int i=0;
	while (true){
		EigenValues[i]=wr[i];

		complex<double> c(wr[i], wi[i]);

		EigenValueIsReal[i] = CheckEigenValueIsReal(c);

		if (!EigenValueIsReal[i]){
			EigenValueIsReal[i+1] = false;
			EigenValues[i+1]=wi[i];
			i += 2;
		}
		else{
			i++;
		}
		if (i>=n) break;
	}


	if (CalcEigenVecs){
		int i=0;
		while (i<n){
			double temp = 0.0;
			if (EigenValueIsReal[i]){
				for (int j=0; j<n; j++){
					double rr = vr[j][i];
					EigenVectors[i][j]=rr;
					temp += rr*rr;
//					ShowFloat(EigenVectors[i][j].real(), "realEV");
				}
				temp = 1.0/sqrt(temp);
				for (int j=0; j<n; j++){
					EigenVectors[i][j] *= temp;
				}
				if (CalcAlsoLeftEV){
					double temp = 0.0;
					if (EigenValueIsReal[i]){
						for (int j=0; j<n; j++){
							double rr = vl[j][i];
							LEigenVectors[i][j]=rr;
							temp += rr*rr;
//					ShowFloat(EigenVectors[i][j].real(), "realEV");
						}
						temp = 1.0/sqrt(temp);
						for (int j=0; j<n; j++){
							LEigenVectors[i][j] *= temp;
						}
					}
				}

				i++;
			}
			else{
				temp=0.0;
				for (int j=0; j<n; j++){
					double rr=vr[j][i];
					double ii=vr[j][i+1];

					EigenVectors[i][j] = rr;
					EigenVectors[i+1][j] = ii;

					temp += rr*rr + ii*ii;

				}
				temp = 1.0/sqrt(temp);
				for (int j=0; j<n; j++){
					EigenVectors[i][j] *= temp;
					EigenVectors[i+1][j] *= temp;
				}
				if (CalcAlsoLeftEV){

					double temp=0.0;
					for (int j=0; j<n; j++){
						double rr=vl[j][i];
						double ii=vl[j][i+1];

						LEigenVectors[i][j] = rr;
						LEigenVectors[i+1][j] = ii;
	
						temp += rr*rr + ii*ii;

					}
					temp = 1.0/sqrt(temp);
					for (int j=0; j<n; j++){
						LEigenVectors[i][j] *= temp;
						LEigenVectors[i+1][j] *= temp;
					}

				}
				i += 2;
			}

		}

	}

}

void ShowComplex(complex<double> * cs, int n, CString show){
	CString c="(;";
	for (int i=0; i<n; i++){
		char s [20];
		_gcvt(cs[i].real(), 5, s);
		c += "("+CString(s)+CString("i*");
		_gcvt(cs[i].imag(), 5, s);
		if (i==(n-1)){	
			c += CString(s)+")";
		}
		else{
			c += "; ";
		}
	}

	MessageBox(NULL, c, show, MB_OK);
}
void ShowComplex(complex<double> cc, CString show){
	CString c;
	char s [20];
	_gcvt(cc.real(), 5, s);
	c += "("+CString(s)+CString("i*");
	_gcvt(cc.imag(), 5, s);
	c += CString(s)+")";

	MessageBox(NULL, c, show, MB_OK);
}

bool ComplexEV::CheckDefective(int n, int istart, int iend, complex<double> ** EigenVectors){
	if (istart==iend) return false;
//	complex <double> * factors = new complex<double>[n];
//	ShowFloat(istart, "istart");
//	ShowFloat(iend, "iend");
	
	for (int i=(istart+1); i<=iend; i++){
		complex<double> sumreal=0.0;
		complex<double> sumimag=0.0;
		complex<double> sumbb=0.0;
		for (int j=0; j<n; j++){
//			complex<double> a = EigenVectors[j][istart];
//			complex<double> b = EigenVectors[j][i];
			complex<double> a = EigenVectors[istart][j];
			complex<double> b = EigenVectors[i][j];
			complex<double> ax = my_conj(a);
			complex<double> bx = my_conj(b);

			complex <double> tp1 = b*ax;
			complex <double> tp2 = bx*a;

			sumreal += tp1+tp2;
			sumimag += tp1-tp2;
			sumbb += b*bx;
		}

		complex <double> lx = 0.5*sumreal/sumbb;
		complex <double> ly = 0.5*sumimag/sumbb;
		complex <double> factor;
		factor.real(lx.real());
		factor.imag(-ly.imag());
//		ShowFloat(factors[i].real(), "real");
//		ShowFloat(factors[i].imag(), "imag");

		if (false){
			char s[20];
			CString c =" a =(";
			for (int j=0; j<n; j++){
//			complex<double> a = EigenVectors[j][istart];
				complex<double> a = EigenVectors[istart][j];
				_gcvt(a.real(), 6,s);
				c +=CString(s)+" +i* ";
				_gcvt(a.imag(), 6,s);
				c +=CString(s);
				if (j<(n-1)){
					c += ";";
				}
				else{
					c += ")";
				}
			}
	
			c +=" b =(";
			for (int j=0; j<n; j++){
//			complex<double> b = EigenVectors[j][i];
				complex<double> b = EigenVectors[i][j];
				_gcvt(b.real(), 6,s);
				c +=CString(s)+" +i* ";
				_gcvt(b.imag(), 6,s);
				c +=CString(s);
				if (j<(n-1)){
					c += ";";
				}
				else{
					c += ")";
				}
			}
			c +=" c =(";
			for (int j=0; j<n; j++){
//				complex<double> m = factors[i]*EigenVectors[j][i];
				complex<double> m = factor*EigenVectors[i][j];
				_gcvt(m.real(), 6,s);
				c +=CString(s)+" +i* ";
				_gcvt(m.imag(), 6,s);
				c +=CString(s);
				if (j<(n-1)){
					c += ";";
				}
				else{
					c += ")";
				}
			}


			Alert0(c);
		}

		double sum = 0.0;
		for (int j=0; j<n; j++){
				complex<double> a = EigenVectors[istart][j];
				complex<double> b = EigenVectors[i][j];
				complex<double> m = factor*EigenVectors[i][j];

				complex<double> tp1 = a-m;
				complex<double> tp2 = tp1*my_conj(tp1);
				sum += tp2.real();
		}
//		ShowFloat(i, "i");
//		ShowFloat(sum, "sum");
		if (sum<eps_defective){
			return true;
		}

	}
			

//	delete [] factors;

	return CheckDefective(n, istart+1, iend, EigenVectors);
}
double ComplexEV::my_cabs(complex<double> c){
	double r = c.real();
	double i = c.imag();
	return sqrt(r*r+i*i);
}
complex<double> ComplexEV::my_conj(complex<double> c){
	complex<double> result;
	result.real(c.real());
	result.imag(-c.imag());
	return result;
}

bool ComplexEV::MatrixIsDefective(double ** mat, int n){

  complex<double> * EigenValues = new complex<double> [n];
  complex<double> ** EigenVectors = cmatrix(0, n-1, 0, n-1);
  bool * EigenValueIsReal = new bool [n];

  CalcEigenValuesAndVectors(mat, n,
							EigenValues,
							EigenVectors,
							EigenValueIsReal);

  // scan for equal EV's
  int istart = 0;
  complex<double> c = EigenValues[istart];
  int iend = 0;
  int i=0;
  bool FoundDefective = false;
  while (true){
	  if (i>=n) break;
	  if (i==(n-1)){
	//	  Alert0("i=iend");
		  iend = i;
		  FoundDefective= CheckDefective(n, istart, iend, EigenVectors);
		  break;
	  }
	  i++;
	  if  (my_cabs(c-EigenValues[i])> eps_defective){
	//	  Alert0(">>eps");
		  iend = i-1;
		  FoundDefective= CheckDefective(n, istart, iend, EigenVectors);
		  if (FoundDefective){
			  break;
		  }
		  istart = i;
		  c = EigenValues[istart];
	  }
	  else{
	//	  Alert0("<eps");
		  i++;
	  }
  }


  delete [] EigenValueIsReal;
  free_cmatrix(EigenVectors, 0, n-1, 0, n-1);
  delete [] EigenValues;

  return FoundDefective;
}
