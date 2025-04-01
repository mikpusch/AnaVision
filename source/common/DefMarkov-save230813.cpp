#include "stdafx.h"

//#include "c:\msdev\projects\3States\TestMarkov.h"
#include <math.h>
#include "..\common\utils.h"
#include "..\common\nrutil.h"
#include "..\common\complexEV.h"


#include "..\common\DefMarkov.h"

BOOL Markov::UseGaussSeidel = FALSE;

Markov::Markov(){
	NS = 0;
	DoShow = FALSE;
	ComplexEigenValueFound = false;
}

Markov::~Markov(){
	//ShowInt(NS, "enter del Markov class");
	if (NS<=1) return;
	DeleteMarkov();
}

bool Markov::FoundComplexEigenValue(){
	return ComplexEigenValueFound;
}

void Markov::ClearRates(double ** r){
	int i, j;
	if (NS<1) return;
	for (i=0; i<NS; i++){
		for (j=0; j<NS; j++){
			r[i][j] = 0;
		}
	}
}


void Markov::Taus(double * t){
	if (NS<2) return;
	CalcTauAndEV(NS, R, pinf, tau, ev, iw, EVIsReal);

	for (int i=0; i<(NS-1); i++){
		t[i]=tau[i];
	}
}



void Markov::TausAndCoeffs(double * t, double ** coeff){
	if (NS<2) return;
//	MakeE(NS, R, p0, pinf, tau, e);
	int i, j;
	for ( i=0; i<(NS-1); i++){
		t[i]=tau[i];
	}
			
	for ( i=0; i<(NS-1); i++){
		for ( j=0; j<(NS-1); j++){
			coeff[i][j] = e[i][j];
		}
		coeff[i][NS-1]=pinf[i];
	}
	for ( j=0; j<(NS-1); j++){
		double sum=0.0;
		for ( i=0; i<(NS-1); i++){
			sum += coeff[i][j];
		}
		coeff[NS-1][j]= -sum;
	}
	coeff[NS-1][NS-1] = pinf[NS-1];

}

void Markov::PrepareJump(double * p0){
	if (NS<2) return;
	MakeE(NS, R, p0, pinf, tau, e);
}


void Markov::CopyPinftauE(double * copypinf, double * copytau, double ** copye){
	int i, j;
	for (i=0; i<NS; i++){
		copypinf[i]=pinf[i];
		for (j=0; j<NS; j++){
			copye[i][j] = e[i][j];
		}
	}
	for (i=0; i<(NS-1); i++){
		copytau[i]=tau[i];
	}
}


void Markov::LoadPinftauE(double * copypinf, double * copytau, double ** copye){
	int i, j;
	for (i=0; i<NS; i++){
		pinf[i]=copypinf[i];
		for (j=0; j<NS; j++){
			e[i][j] = copye[i][j];
		}
	}
	for (i=0; i<(NS-1); i++){
		tau[i]=copytau[i];
	}
}


void Markov::Calc(double t, double * p){
	if (NS<2) return;
	EvalE (NS, t, pinf, e, tau, p);
}


void Markov::InitMarkov(int NStates){
	ComplexEigenValueFound = false;
	if (NStates<=1){
		Alert0(_T("less than 2 states in InitMarkov"));
		return;
	}

	if (NStates>MARKOV_MAX_STATES){
//		char ss[20];
		ShowFloat(MARKOV_MAX_STATES, _T("this is max number of states handled by Markov: error"));
		return;
	}

	if (NS==NStates) return;

	if (NS!=0){
		DeleteMarkov();
	}
	NS = NStates;
	R = dmatrix(0, NS-1, 0, NS-1);
	Q = dmatrix(0, NS-1, 0, NS-1);
	AW = dmatrix(0, NS-1, 0, NS-1);
	IW = dmatrix(0, NS-1, 0, NS-1);
	W = dmatrix(0, NS-1, 0, NS-1);
	e = dmatrix(0, NS-1, 0, NS-1);
	evt = dmatrix(0, NS-1, 0, NS-1);

	rs = dvector(0, NS-1);
	pinf = dvector(0, NS-1);
	EVIsReal = new bool[NS];
	p = dvector(0, NS-1);
	p0 = dvector(0, NS-1);
	tau = dvector(0, NS-1);
	lambda = dvector(0, NS-1);
	indx=imatrix(0, NS-1, 0, NS-1);

	w=dmatrix(0, NS-1, 0, NS-1);
	ww=dmatrix(0, NS-1, 0, NS-1);
	iw=dmatrix(0, NS-1, 0, NS-1);
	QM=dmatrix(0, NS-1, 0, NS-1);
	RM=dmatrix(0, NS-1, 0, NS-1);
	coeffs=dmatrix(0, NS-1, 0, NS-1);

	ev = dmatrix(0, NS-1, 0, NS-1);
	iev = dmatrix(0, NS-1, 0, NS-1);
	complex_ev = cmatrix(0, NS-1, 0, NS-1);
	complex_iev = cmatrix(0, NS-1, 0, NS-1);
	complex_e = cmatrix(0, NS-1, 0, NS-1);
	complex_evt = cmatrix(0, NS-1, 0, NS-1);
	complex_AW = cmatrix(0, NS-1, 0, NS-1);
	ratesautocov = dvector(0, NS-1);
    AAutoCov=dmatrix(0, NS-1, 0, NS-1);
    BAutoCov=dmatrix(0, NS-1, 0, NS-1);
}

void Markov::DeleteMarkov(){
	free_dmatrix( BAutoCov, 0, NS-1, 0, NS-1);
	free_dmatrix( AAutoCov, 0, NS-1, 0, NS-1);
	free_dvector( ratesautocov, 0, NS-1);
	free_cmatrix(complex_AW, 0, NS-1, 0, NS-1);
	free_cmatrix(complex_evt, 0, NS-1, 0, NS-1);
	free_cmatrix(complex_e, 0, NS-1, 0, NS-1);
	free_cmatrix(complex_iev, 0, NS-1, 0, NS-1);
	free_cmatrix(complex_ev, 0, NS-1, 0, NS-1);
	free_dmatrix(iev, 0, NS-1, 0, NS-1);
	free_dmatrix(ev, 0, NS-1, 0, NS-1);
	free_dmatrix(coeffs, 0, NS-1, 0, NS-1);
	free_dmatrix(RM, 0, NS-1, 0, NS-1);
	free_dmatrix(QM, 0, NS-1, 0, NS-1);
	free_dmatrix(iw, 0, NS-1, 0, NS-1);
	free_dmatrix(ww, 0, NS-1, 0, NS-1);
	free_dmatrix(w, 0, NS-1, 0, NS-1);
	free_imatrix(indx, 0, NS-1, 0, NS-1);
	free_dvector(tau, 0, NS-1);
	free_dvector(lambda, 0, NS-1);
	free_dvector(p0, 0, NS-1);
	free_dvector(p, 0, NS-1);
	delete [] EVIsReal;
	free_dvector(pinf, 0, NS-1);
	free_dvector(rs, 0, NS-1);
	free_dmatrix(evt, 0, NS-1, 0, NS-1);
	free_dmatrix(e, 0, NS-1, 0, NS-1);
	free_dmatrix(W, 0, NS-1, 0, NS-1);
	free_dmatrix(IW, 0, NS-1, 0, NS-1);
	free_dmatrix(AW, 0, NS-1, 0, NS-1);
	free_dmatrix(R, 0, NS-1, 0, NS-1);
	free_dmatrix(Q, 0, NS-1, 0, NS-1);
}

void Markov::SetRates(double ** rates){
	int i, j;
	double sum, tp1;//, det;
	int nm=NS-1;
	for (i=0; i<NS; i++)
		for (j=0; j<NS; j++)
			R[i][j] = rates[i][j];


	for (i=0; i<NS; i++){
		sum=0.0;
		for (j=0; j<NS; j++){
			if (i!=j){
				sum -= R[i][j];
				AW[i][j] = R[j][i];
			}
		}
		AW[i][i] = sum;
	}

	for (i = 0; i<nm; i++){
		tp1 = -AW[i][nm];
	    rs[i] = tp1;
		for (j=0; j<nm; j++){
			IW[i][j] = AW[i][j] + tp1;
		}
	}

	if (UseGaussSeidel){
		int MaxIter=1000;
		double eps = 1e-20;
		for (i=0; i<NS; i++){
			rs[i]=0;
		}
//		SimpleGaussSeidel(nm, IW, rs, pinf, MaxIter, eps);
//		SimpleSOR(nm, IW, rs, pinf, MaxIter, eps, 0.1);
		ProbabilityGaussSeidel(NS, AW, rs, pinf, MaxIter, eps);

/*
		double sump = 0.0;

		for (i=0; i<nm; i++){
			sump += pinf[i]; 
		}
		pinf[nm]=1.0 - sump;
*/
	}
	else{

//		if (InvertDoubleMat(IW, W, nm, &det)<0){
		if (InvertDoubleMat(IW, W, nm)<0){

//		if (det==0.0){
//		Alert0("Error in Invert Mat in SetRates");
			return;
		}

		double sump = 0.0;

		for (i=0; i<nm; i++){
			sum = 0.0;
			for (j=0; j<nm; j++){
				sum += W[i][j]*rs[j];
			}
			sump += sum;
			pinf[i] = sum;
		}
		pinf[nm]=1.0 - sump;
	}
}

void Markov::Pinf(double * p){
	if (NS<=1) return;
	for (int i=0; i<NS; i++)
		p[i]=pinf[i];
}


void AssV (int n, double *v1, double *v2)
{
 /* v1 := v2 */
 int i;
 for (i=0;i<n;i++)
    v1[i] = v2[i];
}

void Markov::CalcE (int n, double *p0, double *pss, double **ev, 
					double **evt, double **e)
{
 double lambda[MARKOV_MAX_STATES];
 int i, j;
 double sum;

 for (i=0; i<(n-1); i++)
  {
    sum = 0;
    for(j = 0; j<(n-1); j++)
     {
        sum += (p0[j] - pss[j]) * evt[j][i];
     }
    lambda[i] = sum;
  }

 for (i=0; i<(n-1); i++)
  {
    for (j=0; j<(n-1); j++)
     {
        e[i][j] = lambda[j] * ev[j][i];
     }
  }
}




void Markov::SolveEVEq (int n1, int ns, double **m, double **XX)
{
int n;

//int **indx;
int Zero[MARKOV_MAX_STATES], Bigx[MARKOV_MAX_STATES];
int i, j, i0, nn, bigi, bigj, xxx, k0, j0, k, nfound;
double Big, absbig, tp1, cp1, cp2, cp3, sum;

n=n1-1;

//indx=imatrix(0, n-1, 0, n-1);

for (i=0; i<n1; i++)
 {
  indx[n][i] = i;
  for (j=0; j<n1; j++)
    {
     XX[i][j] = 0;
    }
 }

for (i0=0; i0<=(n-ns); i0++)
/*  FOR i0% = 0 TO n - ns%'???????  */
 {
    nn = n - i0;

/*    GOSUB GetBig
     GOSUB dividebig

      GetBig: ' nn% is current index
*/

    Big = m[0][indx[nn][0]];
    absbig = fabs(Big);
    bigi = 0;
    bigj = 0;

    for (i=0; i<=nn; i++)
     {
      for (j=0; j<=nn; j++)
       {
	tp1 = fabs(m[i][indx[nn][j]]);
	if (tp1>absbig)
	 {
	   bigi = i;
	   bigj = j;
	   absbig = tp1;
	   Big = m[i][indx[nn][j]];
	 }
       }
     }
/*
dividebig:
*/

    absbig = fabs(Big);
    for (i=0; i<=nn; i++)
     {
      xxx = indx[nn][i];
      m[bigi][xxx] = m[bigi][xxx] / Big;
     }

    if (bigi != nn)
     {
      for (i=0; i<=nn; i++)
       {
	xxx = indx[nn][i];
	cp1 = m[bigi][xxx];
	m[bigi][xxx] = m[nn][xxx];
	m[nn][xxx] = cp1;
       }
      bigi = nn;
     }

    for (i=0; i<nn; i++)
     {
      cp1 = m[i][indx[nn][bigj]];
      if ( fabs(cp1) > 0)
       {
	for (j=0; j<=nn; j++)
	 {
	  xxx = indx[nn][j];
	  cp2 = cp1 * m[nn][xxx];
	  cp3 = m[i][xxx];
	  m[i][xxx] = cp3 - cp2;
	 }
       }
     }

    Bigx[nn] = bigj;
    if (nn>0)
     {
       k0 = 0;
       for (j0=0; j0<=nn; j0++)
	{
	 if (j0!=bigj)
	  {
	   indx[nn - 1][k0] = indx[nn][j0];
	   k0++;
	  }
	}
     }
 } /* next i0 */

/* 'zerofound:  */

  nfound = ns;

//if (DoShow) ShowLongInt(nfound, "nfound");
  /* 'nn% = n - i0% */
  nn = nfound - 1;

/* 'PRINT nn% */
  for (j=0; j<nfound; j++)
   {
    Zero[j] = indx[nn][j];
//ShowLongInt(j, "j");
//ShowLongInt(Zero[j], "zero[j]");
    XX[j][Zero[j]] = 1;
   }

/* 'CALL WaitKey(tp6$) */

  for (k=0; k<nfound; k++)
   {
    for (i=nfound; i<=n; i++)
     {
      nn = i - 1;
      xxx = indx[i][Bigx[i]];
      XX[k][xxx] = 0;
      for (j=0; j<i; j++)
       {
	cp1 = m[i][indx[nn][j]] * XX[k][indx[nn][j]];
	XX[k][xxx] = XX[k][xxx] - cp1;
       }
     }
   } /* for k */

  for (i=0; i<ns; i++)
   {
    sum = 0;
    for (j=0; j<=n; j++)
     {
      tp1 = XX[i][j];
      sum +=  tp1 * tp1;
     }
    sum = sqrt(sum);
    for (j=0; j<=n; j++)
     {
      tp1 = XX[i][j];
      XX[i][j] = XX[i][j] / sum;
     }
   }
//free_imatrix(indx, 0, n-1, 0);

} /* EXIT SUB */


/*
printm:


PRINT
FOR i% = 0 TO n
    FOR j% = 0 TO n
        tp1 = m(i%, j%)
        PRINT tp1;
    NEXT j%
    PRINT
NEXT i%
CALL WaitKEy(tp6$)
RETURN


END SUB
*/



bool Markov::CalcTauAndEV (int n, double **Rates, double *pss,
		   double *tau, double **e, double ** ie, bool * EvIsReal)
{
 int nm;
 // double **w, **ww, **iw, **QM, **RM;
 double rs[MARKOV_MAX_STATES];
 double uniquetaus[MARKOV_MAX_STATES];
 int ntau[MARKOV_MAX_STATES];
 double sum, sump, tp1;
 int i, j,  toput, differenttau, tp11, tp2;
// double det;

 nm = n - 1;



for (i=0; i<n; i++)
 {
  sum = 0;
  for (j = 0; j<n; j++)
   {
	if (i!=j){
	    sum -= Rates[i][j];
		w[i][j] = Rates[j][i];
	}
   }
  w[i][i] = sum;
 }
for (i=0; i<nm; i++)
{
 tp1 = -w[i][nm];
 rs[i] = tp1;
 for (j=0; j<nm; j++)
  {
    iw[i][j] = w[i][j] + tp1;
  }
}

for (i=0; i<nm; i++)
 {
  for (j=0; j<nm; j++)
   {
     ww[i][j] = iw[i][j];
   }
 }

//if (InvertDoubleMat(iw, w, nm, &det)<0){
if (InvertDoubleMat(iw, w, nm)<0){

//if (det == 0) {
//  Alert0("Error in Invert Mat");
 }

sump = 0;
for (i=0; i<nm; i++)
 {
  sum = 0;
  for (j=0; j<nm; j++)
   {
     sum +=  w[i][j]* rs[j];
   }
  sump += sum;
  pss[i] = sum; /*  ' steady state p */
 }
pss[nm] = 1.0 - sump;



//CalcEigenValues(nm, iw, tau);
//for (int i=0; i<nm; i++){
//	ShowFloat(tau[i], "taui");
//}
ComplexEV ev;
ev.CalcEigenValuesAndVectors(iw, nm, tau, e, ie, EVIsReal);

bool result = true;

for (int i=0; i<nm; i++){
	if (EVIsReal[i]){
//		Alert0("Real");
	}
	else{
		result = false;
//		Alert0("imag");
	}
//	ShowFloat(tau[i], "taui");
//	for (int j=0; j<nm; j++){
//		ShowFloat(e[i][j], "eeij");
//	}

}

/* ' Now calc Eigenvectors for each eigenvalue
   ' order taus
*/

#define DoShowEV 0
for (i=0; i<nm; i++){
	tau[i]=1.0/fabs(tau[i]);
	if (DoShowEV) ShowFloat(tau[i], _T("tau"));
}
if (!result){
	ComplexEigenValueFound = true;
}
return result;

Order(tau, nm, false);

/* 'Now: smallest tau first
   ' Check for equal taus
*/

toput = 0;
uniquetaus[0] = tau[0];
ntau[0] = 1;

for (i=1; i<nm; i++)
 {
  if ( ((tau[i] - uniquetaus[toput]) / tau[i]) < 1.0E-10)
   {
    ntau[toput] += 1;
   }
  else
   {
    toput++;
    uniquetaus[toput] = tau[i];
    ntau[toput] = 1;
   }
 }

differenttau = toput + 1;

toput = 0;

//if (DoShow) ShowLongInt(differenttau, "differenttau");


for (i=0; i<differenttau; i++)
 {
	//if (DoShow) ShowLongInt(i, "i");
	//if (DoShow) ShowFloat(uniquetaus[i], " unique tau");

  for (tp11 = 0; tp11<nm; tp11++)
   {
     for (tp2= 0; tp2<nm; tp2++)
      {
       iw[tp11][tp2] = ww[tp11][tp2];
      }
     iw[tp11][tp11] +=  1.0 / uniquetaus[i];
   }

  //if (DoShow) ShowLongInt(ntau[i], "ntau");
  SolveEVEq(nm, ntau[i], iw, QM);

  for (tp11 = 0; tp11<ntau[i]; tp11++)
   {
    for (tp2 = 0; tp2<nm; tp2++)
     {
       e[toput][tp2] = QM[tp11][tp2];
     }
    toput++;
   }
 }
//for (int i=0; i<nm; i++){
//	for (int j=0; j<nm; j++){
//		ShowFloat(e[i][j], "eeij");
//	}
//}

//free_matrix(RM, 0, nm-1, 0);
//free_matrix(QM, 0, nm-1, 0);
//free_matrix(iw, 0, nm-1, 0);
//free_matrix(ww, 0, nm-1, 0);
//free_matrix(w, 0, nm-1, 0);

}



inline void Markov::EvalE (int n, double t, double *pss, double **e, double *tau, double *p)
{
double exps[MARKOV_MAX_STATES];   /* DIM exps(n% - 2) */
int k;
double sump, sum;

int i=0;
while (true){
	if (i>(n-2)){
		break;
	}
	if (EVIsReal[i]){
	  exps[i] = exp(-t / tau[i]);
	  i++;
	}
	else{
		double exppp = exp(-t / tau[i]);
		double omega = -1.0/tau[i+1];
		exps[i] = exppp*cos(omega*t);
		exps[i+1] = exppp*sin(omega*t);
		i += 2;
	}
}
//for (i=0; i<=(n-2); i++) {
//  exps[i] = exp(-t / tau[i]);
// }

sump = 0;
for (i=0; i<=(n-2); i++)
 {
   sum = pss[i];
   for (k=0; k<=(n-2); k++)
    {
      sum += e[i][k] * exps[k];
    }
   p[i] = sum;
   sump += sum;
 }

p[n - 1] = 1.0 - sump;

}


void Markov::MakeE (int n, double **Rates, double *p0, double *pss, double *tau,
	   double **e)
{
//double **ev, **iev;
//double det;

/* DIM ev(n%, n%), iev(n%, n%) */

//ev=matrix(0, n-1, 0, n-1);
//iev=matrix(0, n-1, 0, n-1);


bool AllEVsReal = CalcTauAndEV(n, Rates, pss, tau, ev, iev, EVIsReal);
//XXXXXXXXXXXXXXXXXXXXXXXXX
if (AllEVsReal){
//	InvertDoubleMat(ev, iev, n - 1, &det);
	InvertDoubleMat(ev, iev, n - 1);
/*	Transpose(ev, w, n-1);
	Transpose(iev, ww, n-1);
	MulMat(e, ww, iw, n-1, n-1, n-1);
	MulMat(evt, e, w, n-1, n-1, n-1);
	for (int i=0; i<(n-1); i++){
		for (int j=0; j<(n-1); j++){
			char s[20];
			_itoa(i, s, 10);
			CString c=s;
			_itoa(j,s,10);
			c += "; "+CString(s)+CString(": ");
			_gcvt(evt[i][j], 5, s);
			c += CString(s);
			Alert0(c);
		}
	}
*/
	CalcE(n, p0, pss, ev, iev, e);
}
else{
	int i=0;
	while (true){
		if (EVIsReal[i]){
			for (int j=0; j<(n-1); j++){
				complex_ev[i][j] = ev[i][j];
			}
			i++;
		}
		else{
			for (int j=0; j<(n-1); j++){
				complex_ev[i][j].real(ev[i][j]);
				complex_ev[i][j].imag(ev[i+1][j]);

				complex_ev[i+1][j].real(complex_ev[i][j].real());
				complex_ev[i+1][j].imag(-complex_ev[i][j].imag());
			}
			i +=2;
		}
		if (i>=(n-1)) break;
	}

//	complex<double> cdet=0;
//	InvertComplexMat(complex_ev, complex_iev, n - 1, &cdet);
	InvertComplexMat(complex_ev, complex_iev, n - 1);
/*
for (int i=0; i<(n-1); i++){
		ShowFloat(i, "i");
		for (int j=0; j<(n-1); j++){
			ShowFloat(j, "j");
			ShowFloat(complex_ev[i][j].real(), "real");
			ShowFloat(complex_ev[i][j].imag(), "imag");
		}
	}
*/

/*
ShowFloat(cdet.real(), "cdet");
	for (int i=0; i<(n-1); i++){
		for (int j=0; j<(n-1); j++){
			complex <double> sum = 0;
			for (int k=0; k<(n-1); k++){
				sum += complex_ev[i][k]*complex_iev[k][j];
			}
			ShowFloat(i, "i");
			ShowFloat(j, "j");
			ShowFloat(sum.real(), "real");
			ShowFloat(sum.imag(), "imag");
		}
	}

*/

/*
for (int i=0; i<(n-1); i++){
		for (int j=0; j<(n-1); j++){
			complex_AW[i][j] = iw[i][j];
		}
	}

	Transpose(complex_ev, complex_e, n-1);
	Transpose(complex_iev, complex_evt, n-1);

	MulMat(complex_ev, complex_evt, complex_AW, n-1, n-1, n-1);
	MulMat(complex_iev, complex_ev, complex_e, n-1, n-1, n-1);
	for (int i=0; i<(n-1); i++){
		for (int j=0; j<(n-1); j++){
			char s[20];
			_itoa(i, s, 10);
			CString c=s;
			_itoa(j,s,10);
			c += "; "+CString(s)+CString(": ");
			_gcvt(complex_iev[i][j].real(), 5, s);
			c += CString(s)+" i* ";
			_gcvt(complex_iev[i][j].imag(), 5, s);
			c += CString(s);
//			Alert0(c);
		}
	}
*/
	{
		 complex <double> lambda[MARKOV_MAX_STATES];
		 int i, j;
		 complex <double> sum;

		for (i=0; i<(n-1); i++)	  {
		    sum = 0;
		    for( j = 0; j<(n-1); j++)     {
		        sum += (p0[j] - pss[j]) * complex_iev[j][i];
		     }
		    lambda[i] = sum;
//			ShowFloat(lambda[i].real(), "li real");
		  }

		 for (i=0; i<(n-1); i++){
		    for (j=0; j<(n-1); j++)     {
		        complex_e[i][j] = lambda[j] * complex_ev[j][i];
//				ShowFloat(complex_e[i][j].real(), "eij");
		     }
		}
		 for (i=0; i<(n-1); i++){
			 int k=0;
			 while (true){
				 if (k>=(n-1)){
					 break;
				 }
				 if (EVIsReal[k]){
					 e[i][k] = complex_e[i][k].real();
					 k++;
				 }	
				 else{
					 e[i][k] = complex_e[i][k].real()+complex_e[i][k+1].real();
					 e[i][k+1] = complex_e[i][k].imag()-complex_e[i][k+1].imag();
					 k += 2;
				 }
			 }
// 		    for (j=0; j<(n-1); j++)     {
//				ShowFloat(e[i][j], "eij");
//		     }

		}

	}


}

//free_matrix(iev, 0, n-1, 0);
//free_matrix(ev, 0, n-1, 0);

}



bool Markov::CalcEigenValues(int n, double ** m, double * ev){


	ComplexEV ev_calculator;
	ev_calculator.CalcEigenValues(m, n,  ev, EVIsReal);

	bool result = true;

	for (int i=0; i<n; i++){
		if (EVIsReal[i]){
//		Alert0("Real");
		}
		else{
			result = false;
//		Alert0("imag");
		}
//	ShowFloat(tau[i], "taui");
//	for (int j=0; j<nm; j++){
//		ShowFloat(e[i][j], "eeij");
//	}

	}

	
	if (!result){
		ComplexEigenValueFound = true;
	}
	return result;

}



void Markov::CalcHistogram(UINT StateCode, double * rates, double * coeffs, bool * EVIsReal){
	int nsubstates = 0;
	int selected[MARKOV_MAX_STATES];
//	double uniquerates[MARKOV_MAX_STATES];
	double PHI[MARKOV_MAX_STATES], pinf[MARKOV_MAX_STATES];
//	int nrates[MARKOV_MAX_STATES];

	int i, j;

	for (i=0; i<NS; i++){
		if (BitIsIn(i, StateCode)){
			selected[i]=1;
			nsubstates ++;
//			ShowFloat(i, "state selected");
		}
		else{
			selected[i]=0;
		}
	}

//	ShowFloat(nsubstates, "states selected");
	if (nsubstates==0){
		Alert0(_T("No state selected"));
		return;
	}
	if (nsubstates>=NS){
		Alert0(_T("Too many states selected"));
		return;
	}

//	ShowInt(nsubstates, "nsubstates");
//	ShowInt(StateCode, "StateCode");
//	return;

	if (nsubstates==1){
		int state;
		for (i=0; i<NS; i++){
			if (selected[i]){
				state=i;
			}
		}

		rates[0]=0.0;
		coeffs[0]=1.0;
		for (i=0; i<NS; i++){
			rates[0] += R[state][i];
		}
		return;
	}

	double ** QEE = dmatrix(0, nsubstates-1, 0, nsubstates-1);

	int I, J, l;
	for (I=0; I<nsubstates; I++)
		for (J=0; J<nsubstates; J++)
			QEE[I][J]=0.0;

	I=0;
	double sum;
	for (i=0; i<NS; i++){
		J=0;
		if (selected[i]){
			for (j=0; j<NS; j++){
				if (selected[j]){
					if (i!=j){
						QEE[I][J] = R[j][i];
//						QEE[I][J] = R[i][j];
					}
					else{
						sum = 0.0;
						for (l=0; l<NS; l++){
							sum -= R[i][l];
						}
						QEE[I][J] = sum;
					}
					J++;
				}
			}
			I++;
		}
	}

//	CalcEigenValues(nsubstates, QEE, rates);

//	for (int kkl=0; kkl<nsubstates; kkl++){
//		ShowFloat(rates[kkl], "rate ");
//	}


/*	int ordered;
	double save;
	do
	 {
	ordered = 1;
	for (i=0; i<(nsubstates-1); i++)
	{
	    if (rates[i] < rates[i + 1])
		 {
		ordered = 0;
		save = rates[i];
		rates[i] = rates[i + 1];
		rates[i + 1] = save;
	     }
	   }
	 }
	while (ordered == 0);
*/

/*
	int toput = 0;

	uniquerates[0] = rates[0];
	nrates[0] = 1;

    for (i=1; i<nsubstates; i++)
	 {
	if ( ((rates[i] - uniquerates[toput]) / rates[i]) < 1.0E-10)
	   {
		nrates[toput] += 1;
	   }
	  else
	   {
		toput++;
		uniquerates[toput] = rates[i];
		nrates[toput] = 1;
	   }
	 }

	int differentrates = toput + 1;

//	ShowInt(differentrates, "diff rates");
	toput = 0;

if (DoShow) ShowLongInt(differentrates, _T("differentrates"));
*/

/*	int tp11, tp2;
	for (i=0; i<differentrates; i++)
	 {
		//if (DoShow) ShowLongInt(i, "i");
		//if (DoShow) ShowFloat(uniquerates[i], " unique rates");
		//ShowFloat(uniquerates[i], " unique rates");

	  for (tp11 = 0; tp11<nsubstates; tp11++)
	   {
		 for (tp2= 0; tp2<nsubstates; tp2++)
	      {
		   iw[tp11][tp2] = QEE[tp11][tp2];
	      }
		    iw[tp11][tp11] -=  uniquerates[i];
	   }

  //if (DoShow) ShowLongInt(nrates[i], "nrates");
  
	  SolveEVEq(nsubstates, nrates[i], iw, QM);

	  for (tp11 = 0; tp11<nrates[i]; tp11++)
	   {
		  sum =0;
	    for (tp2 = 0; tp2<nsubstates; tp2++)
		 {
	      e[tp2][toput] = QM[tp11][tp2];
//	   v1[tp2]=QM[0][tp2];
		 }
	    toput++;
	   }

	 }
*/
/*
for (int i=0; i<nsubstates; i++){
		uniquerates[i] = rates[i]; 
		nrates[i] = 1;
	}
	int differentrates = nsubstates;
*/
	ComplexEV ev;
	ev.CalcEigenValuesAndVectors(QEE, nsubstates, rates, e, evt, EVIsReal);
	complex <double> complex_rates[MARKOV_MAX_STATES];
    bool AllEVReal = true;
	for (int i=0; i<nsubstates; i++){
		if (!EVIsReal[i]){
			AllEVReal = false;
		}
	}
	if (!AllEVReal){
		ComplexEigenValueFound = true;
		int i=0;
		while (true){
			if (EVIsReal[i]){
				for (int j=0; j<nsubstates; j++){
					complex_evt[i][j] = e[i][j];
				}
				complex_rates[i] = rates[i];
				i++;
			}
			else{
				complex_rates[i].real(rates[i]);
				complex_rates[i].imag(rates[i+1]);
				complex_rates[i+1].real(complex_rates[i].real());
				complex_rates[i+1].imag(-complex_rates[i].imag());

				for (int j=0; j<nsubstates; j++){
					complex_evt[i][j].real(e[i][j]);
					complex_evt[i][j].imag(e[i+1][j]);

					complex_evt[i+1][j].real(complex_evt[i][j].real());
					complex_evt[i+1][j].imag(-complex_evt[i][j].imag());
				}
				i +=2;
			}
			if (i>=nsubstates) break;
		}
		for (int i=0; i<nsubstates; i++){
			for (int j=0; j<nsubstates; j++){
				complex_e[i][j] = complex_evt[j][i];
			}
		}
	}
	else{
		for (int i=0; i<nsubstates; i++){
			 for (int j=0; j<nsubstates; j++){
				 evt[i][j]=e[j][i];
			 }
		}
		for (int i=0; i<nsubstates; i++){
			for (int j=0; j<nsubstates; j++){
				 e[i][j]=evt[i][j];
			 }
		}
	}


	Pinf(pinf);
	double sumtot = 0;
	I=0;
	for (i=0; i<NS; i++){
		if (selected[i]){
			 sum = 0.0;
			 for (j=0; j<NS; j++){
				if (!selected[j]){
				 sum += R[j][i]*pinf[j];
				 }
			}
			sumtot += sum;
			PHI[I] = sum;
			I++;
		}
	 }
	for (I=0; I<nsubstates; I++){
		PHI[I] /= sumtot;
	}
	if (AllEVReal){
//		double det;

//		InvertDoubleMat(e, evt, nsubstates, &det);
		InvertDoubleMat(e, evt, nsubstates);


		double Norm = 0.0;
		for (i=0; i<nsubstates; i++){
			coeffs[i] = 0.0;
			for (j=0; j<nsubstates; j++){
				for (int k=0; k<nsubstates; k++){
					W[j][k] = e[j][i]*evt[i][k];
				}
			}

			MulMat(IW, W, QEE, nsubstates, nsubstates, nsubstates);

			for (j=0; j<nsubstates; j++){
				for (int k=0; k<nsubstates; k++){
					coeffs[i] -= PHI[k]*IW[j][k];
				}
			}
			rates[i] = -rates[i];
//			ShowFloat(rates[i], "rates[i]");
			coeffs[i] /= rates[i];
			Norm += coeffs[i];
		}
		for (i=0; i<nsubstates; i++){
			coeffs[i] /= Norm;
		}
	}
	else{
//		complex <double> cdet;
		complex <double> complex_coeffs[MARKOV_MAX_STATES];


//		InvertComplexMat(complex_e, complex_evt, nsubstates, &cdet);
		InvertComplexMat(complex_e, complex_evt, nsubstates);

//		InvertDoubleMat(e, evt, nsubstates, &det);


		for (i=0; i<nsubstates; i++){
			complex_coeffs[i] = 0.0;
			for (j=0; j<nsubstates; j++){
				for (int k=0; k<nsubstates; k++){
					complex_AW[j][k] = complex_e[j][i]*complex_evt[i][k];
				}
			}

			for (int ii=0; ii<nsubstates; ii++){
				for (int jj=0; jj<nsubstates; jj++){
					complex <double> sum = 0.0;
					for (int kk=0; kk<nsubstates; kk++){
						sum += complex_AW[ii][kk]*QEE[kk][jj];
					}
					complex_ev[ii][jj] = sum;

				}
			}
//			MulMat(IW, complex_AW, QEE, nsubstates, nsubstates, nsubstates);

			for (j=0; j<nsubstates; j++){
				for (int k=0; k<nsubstates; k++){
					complex_coeffs[i] -= PHI[k]*complex_ev[j][k];
//					coeffs[i] -= PHI[k]*IW[j][k];
				}
			}
		}
		complex<double> NormCoeffs = 0;
		for (int i=0; i<nsubstates; i++){
			complex_coeffs[i] = complex_coeffs[i] / complex_rates[i];
			NormCoeffs += complex_coeffs[i];
		}
		for (int i=0; i<nsubstates; i++){
			complex_coeffs[i] = complex_coeffs[i] / NormCoeffs;
		}
		int i=0;
		double RealNorm = 0.0;
		while (true){
			if (i>=nsubstates){
				break;
			}
			if (EVIsReal[i]){
				coeffs[i] = complex_coeffs[i].real();
				RealNorm += coeffs[i];
				rates[i]=-rates[i];
				i++;
			}
			else{
				coeffs[i] = complex_coeffs[i].real()+complex_coeffs[i+1].real();
				RealNorm += coeffs[i];
				coeffs[i+1] = -complex_coeffs[i].imag() + complex_coeffs[i+1].imag();
				rates[i]=-rates[i];
				i +=2 ;
			}
			
		}
	//	ShowFloat(RealNorm, "RealNorm");

	}




free_dmatrix(QEE, 0, nsubstates-1, 0, nsubstates-1);

}



void Markov::CalcSpectrum(double * currents, double * fc, double * coeffs){
	int i, j, k, l;
	double sum; //, det;
	int nm=NS-1;
	double pss[MARKOV_MAX_STATES], p0[MARKOV_MAX_STATES];

	double *** PI = d3tensor(0, nm, 0, nm, 0, nm);
	
	CalcTauAndEV(NS, R, pss, tau, ev, iev, EVIsReal);
//	InvertDoubleMat(ev, iev, nm, &det);
	InvertDoubleMat(ev, iev, nm);

	for (i=0; i<NS; i++){
		for (j=0; j<NS; j++){
			p0[j]=0;
		}
		p0[i]=1;
		CalcE(NS, p0, pss, ev, iev, e);
		for (l=0; l<nm; l++){
			sum = 0.0;
			for (k=0; k<nm; k++){
				PI[i][k][l] = e[k][l];
				sum -= e[k][l];
			}
			PI[i][nm][l] = sum;
		}
	}

// PI(i,k)(t) = pinf(k) + sum l PI[i][k][l]*exp(-t/tau(l));
				 


	for (l=0; l<nm; l++){
		sum=0;
		for (i=0; i<NS; i++){
			for (k=0; k<NS; k++){
				sum += currents[i]*currents[k]*pss[i]*PI[i][k][l];
//				ShowFloat(PI[i][k][l], "PI");
			}
		}
		coeffs[l]=4.0*sum*tau[l];
		fc[l] = 1.0/tau[l]/6.28318530718;
	}

	free_d3tensor(PI, 0, nm, 0, nm, 0, nm);

}

double Markov::EvalAutoCovariance(double t, double tau){
	int k, l;
	double sum = 0.0;
	double tp1;
	double rt[MARKOV_MAX_STATES];
	double rtau[MARKOV_MAX_STATES];
	for (k=0; k<NS; k++){
		rt[k] = exp(-ratesautocov[k]*t);
		rtau[k] = exp(-ratesautocov[k]*tau);
	}
	for (k=0; k<NS; k++){
		tp1 = rt[k];
		for (l=0; l<NS; l++){
			sum += tp1*rtau[l]*(AAutoCov[k][l]-BAutoCov[k][l]*rt[l]);
		}
	}
	return sum;
}


void Markov::NonStationaryAutoCovariance(
	double * currents, // INPUT
	double * InitP, // INPUT
	double * rates,
	double ** A, double ** B){
  // the single channel current for each state has to be specfied

  // Cov(t, tau) = Sum over k,l=0,...,N-1 of
  //				exp(-rk*t-rl*tau)*(A(k,l) - B(k,l)*exp(-rl*t))
  // r(N-1) = 0!
 

	int i, j, k, l;
	double tp1, tp2;
	int nm=NS-1;
	double p0[MARKOV_MAX_STATES], taus[MARKOV_MAX_STATES];

	double *** PI = d3tensor(0, nm, 0, nm, 0, nm);
	double ** coeff = dmatrix(0, nm, 0, nm);

	PrepareJump(InitP);
	TausAndCoeffs(rates, coeff);
	for (i=0; i<nm; i++){
		rates[i] = 1.0/rates[i];
	}
	rates[nm] = 0.0;

	for (i=0; i<NS; i++){
		for (j=0; j<NS; j++){
			p0[j] = 0.0;
		}
		p0[i] = 1.0;
		PrepareJump(p0);
		TausAndCoeffs(taus, PI[i]);
	}

	for (k=0; k<NS; k++){
		for (l=0; l<NS; l++){
			A[k][l] = 0.0;
			B[k][l] = 0.0;
			for (i=0; i<NS; i++){
				for (j=0; j<NS; j++){
					tp1 = currents[i]*currents[j];
					tp2= tp1*coeff[i][k];
					A[k][l] += tp2*PI[i][j][l];
					B[k][l] += tp2 * coeff[j][l];
				}
			}
		}
	}

	for (i=0; i<NS; i++){
		ratesautocov[i] = rates[i];
		for (j=0; j<NS; j++){
			AAutoCov[i][j] = A[i][j];
			BAutoCov[i][j] = B[i][j];
		}
	}
	free_dmatrix(coeff, 0, nm, 0, nm);
	free_d3tensor(PI, 0, nm, 0, nm, 0, nm);
}

 void Markov::CalcBurstProbs(UINT StateCode, double * rates, double *** coeffs){
  // if n states are within the burst (i.e. StateCode) then
  // Prob(i,k,t)=Prob to be in state k at time t, given in state i at time 0
  // where i and k count from 0 to n-1
  // is given by
  // sum over j=0 to n-1 of coeff(i,k,j)*exp(-rates[j]*t)
	int nsubstates = 0;
	int selected[MARKOV_MAX_STATES];
	double uniquerates[MARKOV_MAX_STATES];
	double PHI[MARKOV_MAX_STATES], pinf[MARKOV_MAX_STATES];
	int nrates[MARKOV_MAX_STATES];

	int i, j;

	for (i=0; i<NS; i++){
		if (BitIsIn(i, StateCode)){
			selected[i]=1;
			nsubstates ++;
		}
		else{
			selected[i]=0;
		}
	}
	if (nsubstates==0){
		Alert0(_T("No state selected"));
		return;
	}
	if (nsubstates>=NS){
		Alert0(_T("Too many states selected"));
		return;
	}

//	ShowInt(nsubstates, "nsubstates");
//	ShowInt(StateCode, "StateCode");
//	return;

	if (nsubstates==1){
		int state;
		for (i=0; i<NS; i++){
			if (selected[i]){
				state=i;
			}
		}

		rates[0]=0.0;
		coeffs[0][0][0]=1.0;
		for (i=0; i<NS; i++){
			rates[0] += R[state][i];
		}
		return;
	}

	double ** QEE = dmatrix(0, nsubstates-1, 0, nsubstates-1);

	int I, J, l;
	for (I=0; I<nsubstates; I++)
		for (J=0; J<nsubstates; J++)
			QEE[I][J]=0.0;

	I=0;
	double sum;
	for (i=0; i<NS; i++){
		J=0;
		if (selected[i]){
			for (j=0; j<NS; j++){
				if (selected[j]){
					if (i!=j){
						QEE[I][J] = R[j][i];
//						QEE[I][J] = R[i][j];
					}
					else{
						sum = 0.0;
						for (l=0; l<NS; l++){
							sum -= R[i][l];
						}
						QEE[I][J] = sum;
					}
					J++;
				}
			}
			I++;
		}
	}

	CalcEigenValues(nsubstates, QEE, rates);
	//ShowFloat(rates[0], "rate 0");
	//ShowFloat(rates[1], "rate 1");

	int ordered;
	double save;
	do
	 {
	ordered = 1;
	for (i=0; i<(nsubstates-1); i++)
	{
	    if (rates[i] < rates[i + 1])
		 {
		ordered = 0;
		save = rates[i];
		rates[i] = rates[i + 1];
		rates[i + 1] = save;
	     }
	   }
	 }
	while (ordered == 0);

	int toput = 0;

	uniquerates[0] = rates[0];
	nrates[0] = 1;

    for (i=1; i<nsubstates; i++)
	 {
	if ( ((rates[i] - uniquerates[toput]) / rates[i]) < 1.0E-10)
	   {
		nrates[toput] += 1;
	   }
	  else
	   {
		toput++;
		uniquerates[toput] = rates[i];
		nrates[toput] = 1;
	   }
	 }

	int differentrates = toput + 1;

//	ShowInt(differentrates, "diff rates");
	toput = 0;

if (DoShow) ShowLongInt(differentrates, _T("differentrates"));


int tp11, tp2;
for (i=0; i<differentrates; i++)
 {
	//if (DoShow) ShowLongInt(i, "i");
	//if (DoShow) ShowFloat(uniquerates[i], " unique rates");
	//ShowFloat(uniquerates[i], " unique rates");

  for (tp11 = 0; tp11<nsubstates; tp11++)
   {
     for (tp2= 0; tp2<nsubstates; tp2++)
      {
       iw[tp11][tp2] = QEE[tp11][tp2];
      }
     iw[tp11][tp11] -=  uniquerates[i];
   }

  //if (DoShow) ShowLongInt(nrates[i], "nrates");
  
  SolveEVEq(nsubstates, nrates[i], iw, QM);

  for (tp11 = 0; tp11<nrates[i]; tp11++)
   {
	  sum =0;
    for (tp2 = 0; tp2<nsubstates; tp2++)
     {
       //e[tp2][toput] = QM[tp11][tp2];
       e[toput][tp2] = QM[tp11][tp2];
     }
    toput++;
   }
 }

// double det;

// InvertDoubleMat(e, evt, nsubstates, &det);
 InvertDoubleMat(e, evt, nsubstates);
// DMulMat(W, QEE, e, nsubstates, nsubstates, nsubstates);
// DMulMat(IW, evt, W, nsubstates, nsubstates, nsubstates);
 for (i=0; i<nsubstates; i++){
	 for (j=0; j<nsubstates; j++){
//		 ShowInt(i, "i");
//		 ShowInt(j, "j");
//		 ShowFloat(IW[i][j], "IW");
	 }
 }

 
 for (i=0; i<nsubstates; i++){
	 for (j=0; j<nsubstates; j++){
		 PHI[j] = 0.0;
		 pinf[j] = 0.0;
	 }
	 PHI[i]=1.0;
	 CalcE(nsubstates+1, PHI, pinf, e, evt, coeffs[i]);
 }



//	 void Markov::CalcE (int n, double *p0, double *pss, double *tau, double **ev,
//		   double **evt, double **e)


//for (I=0; I<nsubstates; I++){
//ShowFloat(PHI[I], "PHI");
//	ShowFloat(-coeffs[I]/rates[I], "ai");
//}

free_dmatrix(QEE, 0, nsubstates-1, 0, nsubstates-1);


 }

void Markov::MakeTransitionMatrix (double dt, double ** A){
	//double **ev, **iev;
	CString tau;
	CString ntau;
	CString nm;

//	double det;

	int n= NS;

	if (NS<2) return;

	int i, j;
	for (i=0; i<n; i++) {
		double sum = 0;
		for (j = 0; j<n; j++){
			if (i!=j){
				sum -= R[i][j];
				w[i][j] = R[j][i];
			}
		}
		w[i][i] = sum;
	}

	double rates[MARKOV_MAX_STATES];
	double uniquerates[MARKOV_MAX_STATES];
	int nrates[MARKOV_MAX_STATES];

	CalcEigenValues(n, w, rates);

	int ordered = 0;
	do{
		ordered = 1;
		for (i=0; i<(n-1); i++)   {
			if (rates[i] < rates[i + 1]){
				ordered = 0;
				double save = rates[i];
				rates[i] = rates[i + 1];
				rates[i + 1] = save;
			}
		}
	}
	while (ordered == 0);

// 'Now: largest rate first ;  Check for equal rates

	int toput = 0;
	uniquerates[0] = rates[0];
	nrates[0] = 1;
	int differentrate = n;

	for (i=1; i<n; i++){
		if ( fabs((rates[i] - uniquerates[toput]) / rates[i]) < 1.0E-60){
			nrates[toput] += 1;
		}
		else{
			toput++;
			uniquerates[toput] = rates[i];
			nrates[toput] = 1;
		}
	}

	differentrate = toput + 1;

	toput = 0;

//if (DoShow) ShowLongInt(differenttau, "differenttau");


	for (i=0; i<differentrate; i++){
	//if (DoShow) ShowLongInt(i, "i");
	//if (DoShow) ShowFloat(uniquetaus[i], " unique tau");
		int tp11;

	  for (tp11 = 0; tp11<n; tp11++){
		  for (int tp2= 0; tp2<n; tp2++){
			  iw[tp11][tp2] = w[tp11][tp2];
		  }
		  iw[tp11][tp11] -=  rates[i];
		}

  //if (DoShow) ShowLongInt(ntau[i], "ntau");
		SolveEVEq(n, nrates[i], iw, QM);

	  for (tp11 = 0; tp11<nrates[i]; tp11++)   {
		  for (int tp2 = 0; tp2<n; tp2++){
			  e[toput][tp2] = QM[tp11][tp2];
		  }
		  toput++;
	   }
	}

//	InvertDoubleMat(e, iev, n, &det);
	InvertDoubleMat(e, iev, n);

	double exps[MARKOV_MAX_STATES];   /* DIM exps(n% - 2) */

	for (i=0; i<n; i++){
		exps[i] = exp( dt *rates[i]);
	}


	for (i=0; i<n; i++){
		for (int k=0; k<n; k++){
			A[i][k] = 0.0;
			for (int l=0; l<n; l++){
				A[i][k] += iev[i][l]*exps[l]*e[l][k];
			}
		}
	}

}


void Markov::CalcFullOmega(double *** omega, bool * EigenValueIsReal){
  // this matrix gives the greens solution:  index runs from 0 to N-1; the third index runs from 0 to N-2
  //    for i=0,...,N-1:   pi(t)-piss = sum(j=0,N-1)sum(k=0,N-2) omega(i,j,k)*exp(-t/tau[k])

//	double ** Q = dmatrix(0, NS-1, 0, NS-1);;
	double ** EV = dmatrix(0, NS-1, 0, NS-1);;
	double ** IEV = dmatrix(0, NS-1, 0, NS-1);;
	double ** iw = dmatrix(0, NS-1, 0, NS-1);;
	double ** ww = dmatrix(0, NS-1, 0, NS-1);;
//	double lambda[MARKOV_MAX_STATES];
	double uniquelambda[MARKOV_MAX_STATES];
	int nlambda[MARKOV_MAX_STATES];

	for (int i=0; i<NS; i++){
		double temp=0.0;
		for (int j=0; j<NS; j++){
			if (i!=j){
				Q[i][j] = R[j][i];
				temp -= R[i][j];
			}
			Q[i][i] = temp;
		}
	}

	double p0[MARKOV_MAX_STATES];

	p0[0]=1;
	for (int i=1; i<NS; i++){
		p0[i]=0.0;
	}
	PrepareJump(p0);
	// Now we have the inverse eigenvalues in the arrau tau 

	int nm = NS-1;

	for (int i=0; i<nm; i++){
		lambda[i]=1.0/tau[i];
//		ShowFloat(lambda[i], "lambda[i]");
	}

	Order(lambda, nm, true);

/* 'Now: largest lambda first
   ' Check for equal lambdas
*/

	int toput = 0;
	uniquelambda[0] = lambda[0];
	nlambda[0] = 1;

	for (int i=1; i<nm; i++){
			if ( ((-lambda[i] + uniquelambda[toput]) / lambda[i]) < 1.0E-30)  {
//			ShowFloat((lambda[i] - uniquelambda[toput]), "(lambda[i] - uniquelambda[toput])");
//			ShowFloat(((lambda[i] - uniquelambda[toput]) / lambda[i]), "((lambda[i] - uniquelambda[toput]) / lambda[i])");
			nlambda[toput] += 1;
//			Alert0("equal");
		   }
		else {
//		Alert0("differen");
			toput++;
			uniquelambda[toput] = lambda[i];
			nlambda[toput] = 1;
		}
	}

	int differentlambda = toput + 1;

	toput = 0;

//if (DoShow) ShowLongInt(differenttau, "differenttau");

//	ShowFloat(NS, "NS");
//	ShowFloat(differentlambda, "diff lambda");

	
	for (int i=0; i<differentlambda; i++){
	  for (int tp11 = 0; tp11<NS; tp11++){
		  for (int tp2= 0; tp2<NS; tp2++)  {
			  iw[tp11][tp2] = Q[tp11][tp2];
		  }
		  iw[tp11][tp11] +=  uniquelambda[i];
	  }

  //if (DoShow) ShowLongInt(ntau[i], "ntau");
	  SolveEVEq(NS, nlambda[i], iw, ww);

	  for (int tp11 = 0; tp11<nlambda[i]; tp11++){
		  for (int tp2 = 0; tp2<NS; tp2++){
			  EV[toput][tp2] = ww[tp11][tp2];

		  }
		  if (false){
			ShowFloat(lambda[i], "lambda i");
			  double temp=0;
			  for (int m=0; m<NS; m++){
				  ShowFloat(EV[toput][m], "EV");
				  double mult=0;
				  for (int r=0; r<NS; r++){
					  mult += Q[m][r]*EV[toput][r];
				  }
				  ShowFloat(mult/lambda[i], "mult/lambda[i]");
			  }
		  }

	  }
	  toput++;
	}


	for (int tp2 = 0; tp2<NS; tp2++){
		  EV[NS-1][tp2] = pinf[tp2];
	}

	lambda[NS-1]=0;
//	ShowFloat(toput+1, "NL");

//	double det;
//	InvertDoubleMat(EV, IEV, NS, &det);
	InvertDoubleMat(EV, IEV, NS);
//	ShowFloat(det, "det");
	for (int i=0; i<nm; i++){
		tau[i]=1.0/lambda[i];
	}


	for (int i=0; i<NS; i++){
		for (int j=0; j<NS; j++){
//			for (int k=0; k<(NS-1); k++){
			for (int k=0; k<NS; k++){
//				omega[i][j][k] = IEV[i][k]*EV[k][j];
				omega[i][j][k] = IEV[j][k]*EV[k][i];
//				omega[i][j][k] = EV[i][k]*IEV[k][j];
			}
		}
	}




	free_dmatrix(ww, 0, NS-1, 0, NS-1);;
	free_dmatrix(iw, 0, NS-1, 0, NS-1);;
	free_dmatrix(IEV, 0, NS-1, 0, NS-1);;
	free_dmatrix(EV, 0, NS-1, 0, NS-1);;

}