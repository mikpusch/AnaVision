#pragma once

//#ifndef _DEFMARKOV
//#define _DEFMARKOV

#define MARKOV_MAX_STATES 5000

class Markov
{
public:
  Markov();
  ~Markov();

  bool FoundComplexEigenValue();

  void InitMarkov(int NStates);
  void SetRates(double ** rates);
  bool IsDefective();
  void Pinf(double * p);
  void Taus(double * t);
  void TausAndCoeffs(double * t, double ** coeff);
		// Coeff(i,j) : pi : j=0: tau0, j=N-1: a0
  
  void PrepareJump(double * p0);
  void Calc(double t, double * p);
  double ** R;  // rates
  double ** Q;  // Q
  double * p;
  double * p0;
  double * tau;
  double * lambda;
  double ** coeffs;
  BOOL DoShow;
  void MakeTransitionMatrix (double dt, double ** A);

  void ClearRates(double ** r);

  void CalcHistogram(bool UsePCode, UINT PCode, vector<int> & StateCode, double * rates, double * coeffs, bool * IsRealEV);

  void CalcSpectrum(double * currents, double * fc, double * coeffs);
  // the single channel current for each state has to be specfied
  // spectrum = sum coeffs[i]/(1+(f/fc[i])^2)

  bool CalcFullOmega(double *** omega, bool * EigenValueIsReal, double * EVR, double * EVI);
    // this matrix gives the greens solution:  index runs from 0 to N-1; the third index runs from 0 to N-2
  //    for i=0,...,N-1:   pi(t)-piss = sum(j=0,N-1)sum(k=0,N-2) omega(i,j,k)*exp(-t/tau[k])
  // returns true if al EVs are real, otherwise false

  bool OldCalcFullOmega(double *** omega, bool * EigenValueIsReal, double * EVR, double * EVI);
    // this matrix gives the greens solution:  index runs from 0 to N-1; the third index runs from 0 to N-2
  //    for i=0,...,N-1:   pi(t)-piss = sum(j=0,N-1)sum(k=0,N-2) omega(i,j,k)*exp(-t/tau[k])
  // returns true if al EVs are real, otherwise false

   void NonStationaryAutoCovariance(
	double * currents, // INPUT
	double * InitP, // INPUT
	double * rates, // OUTPUT
	double ** A, double ** B); // OUTPUT
  // the single channel current and initial
  // probability for each state has to be specfied

  // Cov(t, tau) = Sum over k,l=0,...,N-1 of
  //				exp(-rk*t-rl*tau)*(A(k,l) - B(k,l)*exp(-rl*t))
  // r0 = 0!
  double EvalAutoCovariance(double t, double tau);


void CalcBurstProbs(UINT StateCode, double * rates, double *** coeffs);
  // if n states are within the burst (i.e. StateCode) then
  // Prob(i,k,t)=Prob to be in state k at time t, given in state i at time 0
  // where i and k count from 0 to n-1
  // is given by
  // sum over j=0 to n-1 of coeff(i,k,j)*exp(-rates[j]*t)

int NS;

void CopyPinftauE(double * copypinf, double * copytau, double ** copye);
void LoadPinftauE(double * copypinf, double * copytau, double ** copye);

static BOOL UseGaussSeidel;

//private:
  bool ComplexEigenValueFound;
  double ** W;
  double ** AW;
  complex<double> ** complex_AW;
  double ** IW;
  double ** e;
  double ** evt;
  complex<double> ** complex_e;
  complex<double> ** complex_evt;
  double * pinf;
  bool * EVIsReal;
  double * rs;

  int ** indx;
  double ** w;
  double ** ww;
  double ** iw;
  double ** QM;
  double ** RM;

  double ** ev;
  double ** iev;

  complex <double> * complex_eigenvalues;
  complex <double> * dummy_complex_vector;
  complex <double> ** complex_ev;
  complex <double> ** complex_iev;
  double * ratesautocov;
  double ** AAutoCov;
  double ** BAutoCov;


  void DeleteMarkov();

  void CalcE (int n, double *p0, double *pss, double **ev,
		   double **evt, double **e);

  void SolveEVEq (int n1, int ns, double **m, double **XX);

  bool CalcTauAndEV (int n, double **Rates, double *pss,
		   double *tau, double **e, double ** ie, bool * EvIsReal);
  // if ev is not real: tau[i]= real part, tai[i+1]=imaginary part, same for e
  // return true if all eivs are real

  inline void EvalE (int n, double t, double *pss, double **e, double *tau, double *p);

  void MakeE (int n, double **Rates, double *p0, double *pss, double * tau,
	   double **e);

 bool CalcEigenValues(int n, double ** m, double * ev);
 // returns true if all Eigenvalues are real


};

//#endif