
#include "stdafx.h"
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
//#include <complex.h>

#include "utils.h"
#include "nrutil.h"
#include "simplex.h"
#include "../common/MTRand.h"
#include "ComplexEV.h"

#include "../alglib370/cpp/src/ap.h"
#include "../alglib370/cpp/src/alglibinternal.h"
#include "../alglib370/cpp/src/alglibmisc.h"
#include "../alglib370/cpp/src/linalg.h"

#include <complex>


#define MaxLogBinomN 10000

#define errormessage(x) Alert0(x);

static bool UtilInitialized = false;

double sumlog[MaxLogBinomN];

HWND MessageWindow=NULL;

void InitUtil(){
	if (UtilInitialized) return;
	MessageWindow = NULL;
	double sum=0.0;
	sumlog[0]=0;
	for (int i=1; i<MaxLogBinomN; i++){
		sum += log(double(i));
		sumlog[i] = sum;
	}
	Randomize();
	MTRand::init_genrand(rand());
}

double my_dmax (double d1, double d2){
	if (d1>d2) return d1;
	return d2;
}
double my_dmin (double d1, double d2){
	if (d1<d2) return d1;
	return d2;
}

void ClearMemory(__int8 * x, __int64 s){
	__int8 * y = (__int8 *)(x);
	for (__int64 i=0; i<s; i++){
		y[i]=0;
	}
}

void SetMessageWindow(HWND mw){
	MessageWindow=mw;
}

double cot(double x){
	return cos(x)/sin(x);
}
double LogFac(int N){
	if (N<1) return 0.0;
	if (!UtilInitialized){
		InitUtil();
	}
	if (N<MaxLogBinomN) return sumlog[N];
	double sum=0.0;
	for (int i=1; i<=N; i++){
		sum += log(double(i));
	}
	return sum;
}

double TrueLogBinom(int N, int k){
	double res = 0;
	int i;
    if (k<(N/2)){
		k=N-k;
	}
	for (i=(k+1); i<=N; i++){
		res += log(double(i));
	}
	for (i=2; i<=(N-k); i++){
		res -= log(double(i));
	}
	return res;
}


double LogBinom2(int N, int k){
	if (!UtilInitialized){
		InitUtil();
	}

	return (sumlog[N]-sumlog[k]-sumlog[N-k]);
}


double LogBinom(int N, int k){
	if ((N<1) | (k>=N) | (k<1)) return 0.0;
	if (N<MaxLogBinomN) return LogBinom2(N, k);
#define halflog2pi .9189385332046727

	double t1 = N;
	double t2 = k;
	double t3 = N - k;

	return ( (t1 + .5) * log(t1) - halflog2pi -
		     (t2 + .5) * log(t2) -
			 (t3 + .5) * log(t3));
}

double TrueLogTrinom(int N, int k1, int k2){
	int maxk=k1;
	int mink=k2;
	int midk=N-k1-k2;
	int save;
	int notok=1;

	while (notok){
		notok=0;
		if (maxk<midk){
			save=midk;
			midk=maxk;
			maxk=save;
			notok=1;
		}
		if (midk<mink){
			save=mink;
			mink=midk;
			midk=save;
			notok=1;
		}
	}

	double res = 0;
	int i;

	for (i=(maxk+1); i<=N; i++){
		res += log(double(i));
	}
	for (i=2; i<=mink; i++){
		res -= log(double(i));
	}
	for (i=2; i<=midk; i++){
		res -= log(double(i));
	}
	return res;
}



double LogTrinom2(int N, int k1, int k2){
	if (!UtilInitialized){
		InitUtil();
	}

	return (sumlog[N]-sumlog[k1]-sumlog[k2]-sumlog[N-k1-k2]);
}


double LogTrinom(int N, int k1, int k2){
	if ((N<1) | (k1<0) | (k2<0) | ((k1+k2)>N)) return 0.0;
	if (N<MaxLogBinomN) return LogTrinom2(N, k1, k2);
#define log2pi 1.837877066409345

	double t1 = N;
	double t2 = k1;
	double t3 = k2;
	double t4 = N-k1-k2;
	return ( (t1 + .5) * log(t1) - log2pi -
		     (t2 + .5) * log(t2) -
		     (t3 + .5) * log(t3) -
			 (t4 + .5) * log(t4));
}




bool BitIsIn(UINT bit, UINT word){
	if (bit>31) return false;
	if (UINT(word/UINT(pow(2.0,double(bit))) % 2))
		return true;
	else
		return false;
}
bool BitIsIn(UINT bit, BYTE byte){
	if (bit>8) return false;
	if (UINT(byte/UINT(pow(2.0,double(bit))) % 2))
		return true;
	else
		return false;
}

void AddBit(UINT bit, UINT & word){
	if (bit>31) return;
	if (BitIsIn(bit, word)) return;
	word += UINT(pow(2.0,double(bit)));
}


void GetStopValue(CString question, int & stopvalue){
	stopvalue = 0;
	int result = MessageBox(NULL, question, _T("Yes or no"), MB_ICONSTOP | MB_OK);
	stopvalue = 1;
}


int ConfirmOld(CString question){
	// returns 0 if not confirm

	int result = MessageBox(NULL, question, _T("Please confirm"), MB_ICONQUESTION | MB_YESNO);

	if (result==IDYES){
		return 1;
	}
	else{
		return 0;
	}
}

void Alert0(CString message){
	// returns 0 if not confirm

//	int result = MessageBox(NULL, message, "Sorry", MB_OK);
	int result = MessageBox(NULL, message, _T("Alert"), MB_OK);
}
void Alert0_Std(std::string message){
	// returns 0 if not confirm

//	int result = MessageBox(NULL, message, "Sorry", MB_OK);
	int result = MessageBoxA(NULL, message.c_str(), _T("Alert"), MB_OK);
}

void ShowString(CString st, CString text){
 MessageBox(NULL, st, text, MB_OK);
}


void ShowLongInt(__int32 value, CString text)
{
 char s1[20];

 _itoa( value, s1, 10 );

MessageBox(NULL, CString(s1), text, MB_OK);
}

void ShowVeryLongInt(long int value, CString text)
{
 char s1[20];

 _itoa( value, s1, 10 );

MessageBox(NULL, CString(s1), text, MB_OK);
}

void ShowUInt(unsigned __int32 value, CString text){
 char s1[20];

 _itoa( value, s1, 10 );

//MessageBox(NULL, CString(s1), text, MB_OK);
MessageBox(MessageWindow, CString(s1), text, MB_OK);
}


void ShowInt(int value, CString text)
{
 char s1[20];

 _itoa( value, s1, 10 );

//MessageBox(NULL, CString(s1), text, MB_OK);
MessageBox(MessageWindow, CString(s1), text, MB_OK);
}

void ShowFloat(unsigned __int64 value, CString text){
	ShowFloat(double(value), text);
}

void ShowFloat(int value, CString text){
	ShowFloat(double(value), text);
}
void ShowFloat(UINT value, CString text){
	ShowFloat(double(value), text);
}

void ShowFloat(double value, CString text)
{
 char s1[20];

// _gcvt( value, 15, s1);
 _gcvt( value, 6, s1);

//MessageBox(NULL, CString(s1), text, MB_OK);
MessageBox(MessageWindow, CString(s1), text, MB_OK);
}


void ShowDouble(double value, CString text)
{
	ShowFloat(value, text);
}

void FlushKeyboard(int number){
	for (int i=0; i<number; i++)
		InKey();
}


void Wait(double seconds){
	SYSTEMTIME t;
	GetSystemTime(&t);
	double dt1=t.wSecond + t.wMinute*60.0 + t.wMilliseconds/1000.0+
				t.wHour*3600.0;
	while(1){
		GetSystemTime(&t);
		double dt2=t.wSecond + t.wMinute*60.0 + t.wMilliseconds/1000.0+
					t.wHour*3600.0;
		if ((dt2-dt1)>=seconds) return;
	}
}

int InKey(){
	if ( GetAsyncKeyState( VkKeyScan(INKEY)))
		return INKEY;
	if ( GetAsyncKeyState( VkKeyScan(CLEARKEY)))
		return CLEARKEY;
	if ( GetAsyncKeyState(VkKeyScan(STOPKEY)))
		return STOPKEY;
	if ( GetAsyncKeyState(F12KEY))
		return F12KEY;
	if ( GetAsyncKeyState(F11KEY))
		return F11KEY;
	if ( GetAsyncKeyState(F10KEY)){
	//	Alert("F10");
		return F10KEY;
	}
	if ( GetAsyncKeyState( VkKeyScan(YESKEY)))
		return YESKEY;
	if ( GetAsyncKeyState( VkKeyScan(NOKEY)))
		return NOKEY;
	if ( GetAsyncKeyState( VkKeyScan(QUITKEY)))
		return QUITKEY;
	if ( GetAsyncKeyState( VkKeyScan(AUTOMATICKEY)))
		return AUTOMATICKEY;
	if ( GetAsyncKeyState( VkKeyScan(RUSHKEY)))
		return RUSHKEY;
	if ( GetAsyncKeyState( VkKeyScan(BACKKEY)))
		return BACKKEY;
	if ( GetAsyncKeyState( VkKeyScan(GOODKEY)))
		return GOODKEY;
	if ( GetAsyncKeyState( VkKeyScan(SPACEKEY)))
		return SPACEKEY;
	if ( GetAsyncKeyState( VkKeyScan(ESCAPEKEY)))
		return ESCAPEKEY;
	return 0;
}

int WaitKey(){

	int result=0;
	//Confirm("enter Wait Key");
	while(1){
		if ( GetAsyncKeyState(VkKeyScan(STOPKEY))){
			result = STOPKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(YESKEY))){
			result =  YESKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(NOKEY))){
			result =  NOKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(QUITKEY))){
			result =  QUITKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(AUTOMATICKEY))){
			result =  AUTOMATICKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(RUSHKEY))){
			result =  RUSHKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(RUSHKEY))){
			result =  RUSHKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(BACKKEY))){
			result =  BACKKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(GOODKEY))){
			result =  GOODKEY; break; }
		if ( GetAsyncKeyState( VkKeyScan(SPACEKEY))){
			result =  SPACEKEY; break; }
	}
	Wait(0.2);
	return result;
}

//#define MOD % // just to port better the modula2 code. Is undefined at the end
//#undef MOD


void MakeUnits(double & value, CString Basic, CString & result){
	double myvalue, absv, ratio;
	char s[100];
	CString pre;
	absv=fabs(value);
	myvalue = 1.0;
	if (absv<1.0E-21){
		myvalue = value/1.0E-24;
		pre = CString(_T("s"));
	}
	else
	if (absv<1.0E-18){
		myvalue = value/1.0E-21;
		pre = CString(_T("r"));
	}
	else
	if (absv<1.0E-15){
		myvalue = value/1.0E-18;
		pre = CString(_T("a"));
	}
	else
	if (absv<1.0E-12){
		myvalue = value/1.0E-15;
		pre = CString(_T("f"));
	}
	else
	if (absv<1.0E-9){
		myvalue = value/1.0E-12;
		pre = CString(_T("p"));
	}
	else
	if (absv<1.0E-6){
		myvalue = value/1.0E-9;
		pre = CString(_T("n"));
	}
	else
	if (absv<1.0E-3){
		myvalue = value/1.0E-6;
		pre = CString(_T("µ"));
	}
	else
	if (absv<1.0){
		myvalue = value/1.0E-3;
		pre = CString(_T("m"));
	}
	else
	if (absv<1.0E3){
		myvalue = value/1.0;
		pre = CString(" ");
	}
	else
	if (absv<1.0E6){
		myvalue = value/1.0E3;
		pre = CString("k");
	}
	else
	if (absv<1.0E9){
		myvalue = value/1.0E6;
		pre = CString("M");
	}
	else
	if (absv<1.0E12){
		myvalue = value/1.0E9;
		pre = CString("G");
	}
	else
	if (absv<1.0E15){
		myvalue = value/1.0E12;
		pre = CString("T");
	}
	ratio = value/myvalue;
	if ((myvalue>=1.0) | (myvalue<=999.99)){
		if (myvalue<2.0) myvalue = 1.0;
		else if (myvalue<3.1) myvalue=2.0;
//		else if (myvalue<4.1) myvalue=4.0;
//		else if (myvalue<5.1) myvalue=5.0;
		else if (myvalue<7.1) myvalue=5.0;
//		else if (myvalue<8.1) myvalue=8.0;
		else if (myvalue<16.0) myvalue=10.0;
//		else if (myvalue<16.0) myvalue=15.0;
		else if (myvalue<41.0) myvalue=20.0;
//		else if (myvalue<31.0) myvalue=30.0;
//		else if (myvalue<41.0) myvalue=40.0;
		else if (myvalue<74.0) myvalue=50.0;
//		else if (myvalue<81.0) myvalue=80.0;
		else if (myvalue<80.0) myvalue=75.0;
		else if (myvalue<151.0) myvalue=100.0;
//		else if (myvalue<160.0) myvalue=150.0;
		else if (myvalue<401.0) myvalue=200.0;
//		else if (myvalue<310.0) myvalue=300.0;
//		else if (myvalue<410.0) myvalue=400.0;
		else if (myvalue<751.0) myvalue=500.0;
//		else if (myvalue<510.0) myvalue=500.0;
//		else if (myvalue<810.0) myvalue=800.0;
//		else myvalue = 900.0;
		else myvalue = 750.0;
		int t=int(myvalue);
		_itoa(t, s, 10);
	}
	else{
		_gcvt( myvalue, 10, s );
	}
	value = myvalue*ratio;
	result = CString(s)+CString(" ")+pre+Basic;
}





void CalcExData (int nexp, int ndata,
		 float *xdata, float *rates, float **expdata)
{
int i, k;
float tp1, tp0;
double tp1d;

for (i = 0; i<nexp; i++)
 for (k=0; k<ndata; k++)
  {
    tp0 = -rates[i] * xdata[k];
    if (fabs(tp0) > 70.0)
     {
      tp1d = 0.0;
     }
    else
     {
      tp1d = exp(tp0);
     }
    tp1 = float(tp1d);
    expdata[i][k] = tp1;
  }
}


int InvertMat (float **B, float **A, int n1, float *determ)
{

/*
 PROCEDURE InvertMat( A    : MatrixPointer ;
  'n1   : INTEGER ;                      VAR Det : Complex.COMPLEX) ; (*
  'Invert matrix A and return determinant. Det=0.0indicates
  'a singular matrix. Note that A is replaced by(A)^-1.
  'Method: Standard Gauss-Jordan.
*/
/*
  A and B are assumed to be in the numerical recipes format with
  starting indices 0,0
*/
	return AlgLibInvertFloatMat(B, A, n1, determ);
}


int InvertDoubleMat (double **B, double **A, int n1, double *determ)
{

/*
 PROCEDURE InvertMat( A    : MatrixPointer ;
  'n1   : INTEGER ;                      VAR Det : Complex.COMPLEX) ; (*
  'Invert matrix A and return determinant. Det=0.0indicates
  'a singular matrix. Note that A is replaced by(A)^-1.
  'Method: Standard Gauss-Jordan.
*/
/*
  A and B are assumed to be in the numerical recipes format with
  starting indices 0,0
*/

	return AlgLibInvertRealMat(B, A, n1, determ);
}

double MyCAbs2(complex<double> x){
	double rr=x.real();
	double ii=x.imag();
	return rr*rr+ii*ii;
}
int InvertComplexMat (complex<double> **B, complex<double> **A,
					   int n1, complex<double> *determ)
{

/*
 PROCEDURE InvertMat( A    : MatrixPointer ;
  'n1   : INTEGER ;                      VAR Det : Complex.COMPLEX) ; (*
  'Invert matrix A and return determinant. Det=0.0indicates
  'a singular matrix. Note that A is replaced by(A)^-1.
  'Method: Standard Gauss-Jordan.
*/
/*
  A and B are assumed to be in the numerical recipes format with
  starting indices 0,0
*/
	return AlgLibInvertComplexMat(B, A, n1, determ);

}

double Det (float **x, int n)
   /* x is a matrix as defined in numerical recipes with
     starting indices 0,0 */
{
 float **cx;
 double result;

 if (n < 1)
   return 0.0;

 if (n == 1)
   return x[0][0];

 if ( n == 2 )
   return (x[0][0] * x[1][1] - x[0][1] * x[1][0]);

cx = matrix(0, n-1, 0, n-1);

float det;
InvertMat(x, cx, n, &det);
result = det;

free_matrix(cx, 0, n-1, 0, n-1);

return result;

}


void OldFitPoly (int npoly, int ndata, double *xdata,
	      double *ydata, double *params)
{
#define NMAXPOLY 20
double xsums[NMAXPOLY*2+1], ysums[NMAXPOLY+1];
double work [NMAXPOLY+1][NMAXPOLY+2];
int i, k, l;
double tp1;


if (npoly > NMAXPOLY)
 {
//  errormessage("Too many polynomial coeffiecients in FitPoly");
  return;
 }

if (npoly < 0)
 {
//  errormessage("npoly<0 in FitPoly");
  return;
 }

if (ndata <= npoly)
 {
//  errormessage("ndata<=npoly in FitPoly");
  return;
 }

 for (i=0; i<=npoly; i++)
  {
    xsums[i] = 0.0;
    xsums[2 * i] = 0.0;
    ysums[i] = 0;
    xsums[2*i+1]=0;
  }

 for (k=0; k<ndata; k++)
  {
   tp1 = 1.0;
   for (i=0; i<=npoly; i++)
    {
     ysums[i] = ysums[i] + ydata[k] * tp1;
     xsums[i] = xsums[i] + tp1;
     tp1 = tp1 * xdata[k];
    }
   for (i=(npoly+1); i<=2*npoly; i++)
    {
     xsums[i] = xsums[i] + tp1;
     tp1 = tp1 * xdata[k];
    }
  }
 for (i=0; i<=npoly; i++)
  {
   work[i][npoly + 1] = ysums[i];
   for (k=0; k<=npoly; k++)
    {
     work[i][k] = xsums[i + k];
    }
  }

 for (i=0; i<npoly; i++)
  {
   if (work[i][i] == 0)
    {
      Alert0(_T(" Division by zero in FitPoly "));
//      return;
    }

   for (k=(i+1); k<=(npoly+1); k++)
    {
     work[i][k] = work[i][k] / work[i][i];
    }
   for (l=(i+1); l<=npoly; l++)
    {
     for (k=(i+1); k<=(npoly+1); k++)
      {
       work[l][k] = work[l][k] - work[l][i] * work[i][k];
      }
    }
  }

if (work[npoly][npoly] == 0)
 {
  Alert0 (_T("Division by zero in FitPoly (2)"));
//  return;
 }

params[npoly] = work[npoly][npoly + 1] / work[npoly][npoly];

for (i=(npoly-1); i>=0; i--)
 {
  params[i] = work[i][npoly + 1];
  for (k=(i+1); k<=npoly; k++)
   {
    params[i] = params[i] - params[k] * work[i][k];
   }
 }
}


void FitPoly (int npoly, int ndata, double *xdata,
	      double *ydata, double *params)
{
#define NNEWMAXPOLY 100
double xsums[NNEWMAXPOLY*2+1], ysums[NNEWMAXPOLY+1];
//double work [NMAXPOLY+1][NMAXPOLY+2];
double ** work;
double ** invwork;

int i;
double tp1;

if (npoly > NNEWMAXPOLY) {
//  errormessage("Too many polynomial coeffiecients in FitPoly");
  return;
}

if (npoly < 0) {
//  errormessage("npoly<0 in FitPoly");
  return;
}

if (ndata < npoly) {
//  errormessage("ndata<=npoly in FitPoly");
  return;
}

work=dmatrix(0, npoly-1, 0, npoly-1);
invwork=dmatrix(0, npoly-1, 0, npoly-1);

for (i=0; i<npoly; i++)  {
    xsums[i] = 0.0;
    xsums[2 * i] = 0.0;
    ysums[i] = 0;
    xsums[2*i+1]=0;
}

for (int k=0; k<ndata; k++)  {
   tp1 = 1.0;
   for (i=0; i<npoly; i++)    {
     ysums[i] = ysums[i] + ydata[k] * tp1;
     xsums[i] = xsums[i] + tp1;
     tp1 = tp1 * xdata[k];
   }
   for (i=(npoly); i<(2*npoly); i++)    {
     xsums[i] = xsums[i] + tp1;
     tp1 = tp1 * xdata[k];
   }
}
for (i=0; i<npoly; i++)  {
   for (int k=0; k<npoly; k++)    {
     work[i][k] = xsums[i + k];
   }
}

//double determ;
//InvertDoubleMat (work, invwork, npoly, &determ);
InvertDoubleMat (work, invwork, npoly);

for (i=0; i<npoly; i++){
	 double tp1 = 0;
	 for (int j=0; j<npoly; j++){
		 tp1 += ysums[j]*invwork[i][j];
	 }
	 params[i]=tp1;
 }

free_dmatrix(invwork, 0, npoly-1, 0, npoly-1);
free_dmatrix(work, 0, npoly-1, 0, npoly-1);


}
void DMatTimesVect (double *A, double **B, double *c,
	     int n, int m)
{
/*    A= B * C */

int i,  k;
double temp;

if ((n < 1) | (m < 1))
 {
  return;
 }

for (i=0; i<n; i++)
 {
    temp = 0.0;
     for (k=0; k<m; k++) {
		temp += B[i][k] * c[k];
     }
     A[i] = temp;
  }

}

void DVectTimesMat (double *A, double *B, double **c,
	     int m, int l)
{
/*    A= B * C */

int  j, k;
double temp;

if ((m < 1) | (l < 1))
 {
  return;
 }

  for (j=0; j<l; j++)
   {
     temp = 0.0;
     for (k=0; k<m; k++)
      {
		temp += B[k] * c[k][j];
      }
     A[j] = temp;
   }
}

void GetExCoeffs (int nexps,
				  int ndata, 
				  //float *xdata,
		          __int16  *ydata,
				  double *params,
				  double *coeffs,
				  int CalcSD,
		          double *sd)
{
#define MAX_COEFFS_GET_FLOAT_EX_COEFFS 200
int KeepOldWork=0;
float  rates[MAX_COEFFS_GET_FLOAT_EX_COEFFS+1], ysums[MAX_COEFFS_GET_FLOAT_EX_COEFFS+1];

//static double work[12][12];
double work[MAX_COEFFS_GET_FLOAT_EX_COEFFS+2][MAX_COEFFS_GET_FLOAT_EX_COEFFS+2];

float **wwork, **pinvwork;

//static float invwork[10][10];
float invwork[MAX_COEFFS_GET_FLOAT_EX_COEFFS][MAX_COEFFS_GET_FLOAT_EX_COEFFS];

int i, j, k;
float tp1;
double tp0d, tp1d, tp2d, tp3d;

//float determ;

if (nexps > MAX_COEFFS_GET_FLOAT_EX_COEFFS)
 {
//  errormessage("nexps>10 in GetExCoeffs");
  return;
 }

if (ndata < 1)
 {
//  errormessage("ndata<1 in GetExCoeffs");
  return;
 }

if (nexps < 1)
 {
//  errormessage("nexps<1 in GetExCoeffs");
  return;
 }

if (KeepOldWork==0)
 {
  wwork=matrix(0, nexps-1, 0, nexps-1);
  pinvwork=matrix(0, nexps-1, 0, nexps-1);
 }

for (i=0; i<nexps; i++)
   rates[i] = float(params[i]);

for (i=0; i<=nexps; i++)
 {
  ysums[i] = 0.0;
  if (KeepOldWork == 0)
   {
    for (j=0; j<=nexps; j++)
     {
       work[i][j] = 0;
     }
   }
  }

for (i=0; i<nexps; i++)
 {
  for (k=0; k<ndata; k++)
   {
    tp0d= -rates[i] * k; //xdata[k];
    if (fabs(tp0d) > 70.0)
     {
      tp1d = 0.0;
     }
    else
     {
      tp1d = exp(tp0d);
     }

    ysums[i] += float(ydata[k] * tp1d);

    if (KeepOldWork == 0)
     {
      for (j=0; j<nexps; j++)
       {
	tp2d = -rates[j] *  k ; // xdata[k];
	if (fabs(tp2d + tp0d) > 70.0)
	 {
	   tp3d = 0;
	 }
	else
	 {
	   tp3d = exp(tp2d + tp0d);
	 }
	work[i][j] += tp3d;
       }
     }
   }
 }

if (KeepOldWork==0)
 {
  for (i=0; i<nexps; i++)
   {
    for (j=0; j<nexps; j++)
     {
      wwork[i][j] = float(work[i][j]);
     }
    }
//  InvertMat(wwork, pinvwork, nexps, &determ);
  InvertMat(wwork, pinvwork, nexps);
  for (i=0; i<nexps; i++)
   {
    for (j=0; j<nexps; j++)
     {
      invwork[i][j] = pinvwork[i][j];
     }
    }
  free_matrix(wwork, 0, nexps-1, 0, nexps-1);
  free_matrix(pinvwork, 0, nexps-1, 0, nexps-1);
 }

for (i=0; i<nexps; i++)
 {
  tp1 = 0;
  for (j=0; j<nexps; j++)
   {
     tp1 += invwork[i][j] * ysums[j];
   }
   coeffs[i] = tp1;
 }

if (CalcSD == 0)
 return;

*sd = 0;

for (k=0; k<ndata; k++)
 {
  tp1 = 0;
  for (i=0; i<nexps; i++)
   {
     tp1 += float(coeffs[i] * exp(-rates[i] *  k )); //xdata[k]);
   }
   tp1 = tp1 - ydata[k];
   *sd += tp1 * tp1;
 }
}


void GetDoubleExCoeffs (int nexps,
					  int ndata, 
				  //float *xdata,
					double   *ydata,
				  double *params,
				  double *coeffs,
				  int CalcSD,
		          double *sd)
{
#define MAX_COEFFS_GET_DOUBLE_EX_COEFFS 200
int KeepOldWork=0;
	double rates[MAX_COEFFS_GET_DOUBLE_EX_COEFFS+1], ysums[MAX_COEFFS_GET_DOUBLE_EX_COEFFS+1];

//static double work[12][12];
double work[MAX_COEFFS_GET_DOUBLE_EX_COEFFS+2][MAX_COEFFS_GET_DOUBLE_EX_COEFFS+2];

double **wwork, **pinvwork;

//static float invwork[10][10];
double  invwork[MAX_COEFFS_GET_DOUBLE_EX_COEFFS][MAX_COEFFS_GET_DOUBLE_EX_COEFFS];

int i, j, k;
double tp1;
double tp0d, tp1d, tp2d, tp3d; //, determ;

if (nexps > MAX_COEFFS_GET_DOUBLE_EX_COEFFS)
 {
  errormessage(_T("nexps>10 in GetExCoeffs"));
  return;
 }

if (ndata < 1)
 {
  errormessage(_T("ndata<1 in GetExCoeffs"));
  return;
 }

if (nexps < 1)
 {
  errormessage(_T("nexps<1 in GetExCoeffs"));
  return;
 }

if (KeepOldWork==0)
 {
  wwork=dmatrix(0, nexps-1, 0, nexps-1);
  pinvwork=dmatrix(0, nexps-1, 0, nexps-1);
 }

for (i=0; i<nexps; i++)
   rates[i] = params[i];

for (i=0; i<=nexps; i++)
 {
  ysums[i] = 0.0;
  if (KeepOldWork == 0)
   {
    for (j=0; j<=nexps; j++)
     {
       work[i][j] = 0;
     }
   }
  }

for (i=0; i<nexps; i++)
 {
  for (k=0; k<ndata; k++)
   {
    tp0d= -rates[i] * k; //xdata[k];
    if (fabs(tp0d) > 70.0)
     {
      tp1d = 0.0;
     }
    else
     {
      tp1d = exp(tp0d);
     }

    ysums[i] += ydata[k] * tp1d;

    if (KeepOldWork == 0)
     {
      for (j=0; j<nexps; j++)
       {
	tp2d = -rates[j] *  k ; // xdata[k];
	if (fabs(tp2d + tp0d) > 70.0)
	 {
	   tp3d = 0;
	 }
	else
	 {
	   tp3d = exp(tp2d + tp0d);
	 }
	work[i][j] += tp3d;
       }
     }
   }
 }

if (KeepOldWork==0)
 {
  for (i=0; i<nexps; i++)
   {
    for (j=0; j<nexps; j++)
     {
      wwork[i][j] = work[i][j];
     }
    }
//  InvertDoubleMat(wwork, pinvwork, nexps, &determ);
  InvertDoubleMat(wwork, pinvwork, nexps);
  for (i=0; i<nexps; i++)
   {
    for (j=0; j<nexps; j++)
     {
      invwork[i][j] = pinvwork[i][j];
     }
    }
  free_dmatrix(wwork, 0, nexps-1, 0, nexps-1);
  free_dmatrix(pinvwork, 0, nexps-1, 0, nexps-1);
 }

for (i=0; i<nexps; i++)
 {
  tp1 = 0;
  for (j=0; j<nexps; j++)
   {
     tp1 += invwork[i][j] * ysums[j];
   }
   coeffs[i] = tp1;
 }

if (CalcSD == 0)
 return;

*sd = 0;

for (k=0; k<ndata; k++)
 {
  tp1 = 0;
  for (i=0; i<nexps; i++)
   {
     tp1 += coeffs[i] * exp(-rates[i] *  k ); //xdata[k]);
   }
   tp1 = tp1 - ydata[k];
   *sd += tp1 * tp1;
 }
}

void GetFuncCoeffs (int nfunc, int ndata, float **funcs,
		    float *ydata, float *params,
		    int CalcSD, double *sd, float * fit )
{
#define MAXFUNC_GET_FUNC_COEFFS 200

	int KeepOldWork=0;
float ysums[MAXFUNC_GET_FUNC_COEFFS+1];
//static float work[11][12];
float work[MAXFUNC_GET_FUNC_COEFFS+1][MAXFUNC_GET_FUNC_COEFFS+2];
float **wwork, **pinvwork;

//static float invwork[11][11];
float invwork[MAXFUNC_GET_FUNC_COEFFS+1][MAXFUNC_GET_FUNC_COEFFS+1];

int i, j, k;
float tp1;
//double  determ;
//float determ;


if (nfunc > MAXFUNC_GET_FUNC_COEFFS)
 {
//  errormessage("nfunc>10 in GetFuncCoeffs");
  return;
 }

if (ndata < 1)
 {
//  errormessage("ndata<1 in GetFuncCoeffs");
  return;
 }

if (nfunc < 1)
 {
//  errormessage("nfunc<1 in GetFuncCoeffs");
  return;
 }

if (KeepOldWork==0)
 {
  wwork=matrix(0, nfunc-1, 0, nfunc-1);
  pinvwork=matrix(0, nfunc-1, 0, nfunc-1);
 }

for (i=0; i<=nfunc; i++)
 {
  ysums[i] = 0.0;
  if (KeepOldWork == 0)
   {
    for (j=0; j<=nfunc; j++)
     {
       work[i][j] = 0;
     }
   }
  }

for (i=0; i<nfunc; i++)
 {
  for (k=0; k<ndata; k++)
   {
     tp1 = funcs[i][k];
     ysums[i] += ydata[k] * tp1;
     if (KeepOldWork == 0)
      {
       for (j=0; j<nfunc; j++)
	{
	 work[i][j] += funcs[j][k] * tp1;
	}
      }
   }
 }

if (KeepOldWork == 0)
 {
  for (i=0; i<nfunc; i++)
   {
    for (j=0; j<nfunc; j++)
     {
       wwork[i][j] = work[i][j];
     }
   }

//  InvertMat(wwork, pinvwork, nfunc, &determ);
  InvertMat(wwork, pinvwork, nfunc);
  for (i=0; i<nfunc; i++)
    {
     for (j=0; j<nfunc; j++)
      {
       invwork[i][j] = pinvwork[i][j];
      }
     }
   free_matrix(wwork, 0, nfunc-1, 0, nfunc-1);
   free_matrix(pinvwork, 0, nfunc-1, 0, nfunc-1);
  }

for (i=0; i<nfunc; i++)
 {
  tp1 = 0;
  for (j=0; j<nfunc; j++)
   {
     tp1 += invwork[i][j] * ysums[j];
   }
   params[i] = tp1;
 }

if (CalcSD == 0)
 return;

*sd = 0;

for (k=0; k<ndata; k++)
 {
  tp1=0;
  for (i=0; i<nfunc; i++)
   {
    tp1 += params[i]* funcs[i][k];
   }
   tp1 = tp1 - ydata[k];
   *sd += tp1 * tp1;
 }
}
#undef MAXFUNC_GET_FUNC_COEFFS

void GetFuncCoeffs (int nfunc, int ndata, double **funcs,
		    double *ydata, double *params,
		    int CalcSD, double *sd, double * fit)
{
#define MAXFUNC_GET_FUNC_COEFFS 200

	int KeepOldWork=0;
double ysums[MAXFUNC_GET_FUNC_COEFFS+1];
//static float work[11][12];
double work[MAXFUNC_GET_FUNC_COEFFS+1][MAXFUNC_GET_FUNC_COEFFS+2];
double **wwork, **pinvwork;

//static float invwork[11][11];
double invwork[MAXFUNC_GET_FUNC_COEFFS+1][MAXFUNC_GET_FUNC_COEFFS+1];

int i, j, k;
double tp1;
//double  determ;
//float determ;


if (nfunc > MAXFUNC_GET_FUNC_COEFFS)
 {
//  errormessage("nfunc>10 in GetFuncCoeffs");
  return;
 }

if (ndata < 1)
 {
//  errormessage("ndata<1 in GetFuncCoeffs");
  return;
 }

if (nfunc < 1)
 {
//  errormessage("nfunc<1 in GetFuncCoeffs");
  return;
 }

if (KeepOldWork==0)
 {
  wwork=dmatrix(0, nfunc-1, 0, nfunc-1);
  pinvwork=dmatrix(0, nfunc-1, 0, nfunc-1);
 }

for (i=0; i<=nfunc; i++)
 {
  ysums[i] = 0.0;
  if (KeepOldWork == 0)
   {
    for (j=0; j<=nfunc; j++)
     {
       work[i][j] = 0;
     }
   }
  }

for (i=0; i<nfunc; i++)
 {
  for (k=0; k<ndata; k++)
   {
     tp1 = funcs[i][k];
     ysums[i] += ydata[k] * tp1;
     if (KeepOldWork == 0)
      {
       for (j=0; j<nfunc; j++)
	{
	 work[i][j] += funcs[j][k] * tp1;
	}
      }
   }
 }

if (KeepOldWork == 0)
 {
  for (i=0; i<nfunc; i++)
   {
    for (j=0; j<nfunc; j++)
     {
       wwork[i][j] = work[i][j];
     }
   }

//  InvertMat(wwork, pinvwork, nfunc, &determ);
  InvertDoubleMat(wwork, pinvwork, nfunc);
  for (i=0; i<nfunc; i++)
    {
     for (j=0; j<nfunc; j++)
      {
       invwork[i][j] = pinvwork[i][j];
      }
     }
   free_dmatrix(wwork, 0, nfunc-1, 0, nfunc-1);
   free_dmatrix(pinvwork, 0, nfunc-1, 0, nfunc-1);
  }

for (i=0; i<nfunc; i++)
 {
  tp1 = 0;
  for (j=0; j<nfunc; j++)
   {
     tp1 += invwork[i][j] * ysums[j];
   }
   params[i] = tp1;
 }

if (CalcSD == 0)
 return;

*sd = 0;

for (k=0; k<ndata; k++)
 {
  tp1=0;
  for (i=0; i<nfunc; i++)
   {
    tp1 += params[i]* funcs[i][k];
   }
  if (fit){
	  fit[k]=tp1;
  }
  tp1 = tp1 - ydata[k];
   *sd += tp1 * tp1;
 }
}
#undef MAXFUNC_GET_FUNC_COEFFS


double MaxDouble (double * d, int np){ //returns  max of an array
	if (np<=0) return 0.0;
	double res=d[0];
	for (int i=1; i<np; i++)
		if (d[i]>res) res=d[i];
	return res;
}

double AbsMinDouble (double * d, int np, int & minindex){ //returns  absolute min of an array
	if (np<=0) return 0.0;
	minindex = 0;
	double res=fabs(d[0]);
	for (int i=1; i<np; i++)
		if (fabs(d[i])<res){
			res=fabs(d[i]);
			minindex = i;
		}
	return res;
}

double AbsMaxDouble (double * d, int np){ //returns  max of an array
	if (np<=0) return 0.0;
	double res=fabs(d[0]);
	for (int i=1; i<np; i++)
		res = GetLargerAbsValue(res, d[i]);
//		if (fabs(d[i])>res) res=fabs(d[i]);
	return res;
}

double MinDouble (double * d, int np){ //returns  min of an array
	if (np<=0) return 0.0;
	double res=d[0];
	for (int i=1; i<np; i++)
		if (d[i]<res) res=d[i];
	return res;
}

double MeanDouble (double * d, int np){ //returns  mean of an array
	if (np<=0) return 0.0;
	double res=d[0];
	for (int i=1; i<np; i++)
		res += d[i];

	return res/double(np);
}

int MaxInt (int * d, int np){ //returns  max of an array
	if (np<=0) return 0;
	int res=d[0];
	for (int i=1; i<np; i++)
		if (d[i]>res) res=d[i];
	return res;
}

int MinInt (int * d, int np){ //returns  min of an array
	if (np<=0) return 0;
	int res=d[0];
	for (int i=1; i<np; i++)
		if (d[i]<res) res=d[i];
	return res;
}

int MeanInt (int * d, int np){ //returns  mean of an array
	if (np<=0) return 0;
	double res=d[0];
	for (int i=1; i<np; i++)
		res += double(d[i]);

	res /= np;
	int ires=int(res);
	return ires;
}

void CopyInt16(__int16 * from, __int16 * to, int N){
	for (int i=0; i<N; i++)
		to[i]=from[i];
}


void WriteStringOnFile(CString s, CFile & fp){
	fp.Write(s, s.GetLength());
}

void WriteStringOnFileWithPos(UINT & pos, CString s, CFile & fp);

void WriteCharOnFile(char c, CFile & fp){
	fp.Write(&c, 1);
}


void WriteIntOnFile(int value, CFile & fp){
	char s1[20];
	 _itoa( value, s1, 10 );
	CString cs = CString(s1);
	fp.Write(s1, cs.GetLength());
	cs = CString("  ");
	fp.Write(cs.GetBuffer(2), cs.GetLength());
}

void WriteDoubleOnFile(double value, CFile & fp){

	char s1[40];
    _gcvt( value, 10, s1);
	CString cs = CString(s1);
	unsigned __int64 Pos=fp.GetPosition();
//	ShowFloat(Pos, _T("Pos"));
	fp.Write(s1, cs.GetLength());
//	fp.SetPos(Pos+cs.GetLength());
	cs = CString(" ");
	fp.Write(cs.GetBuffer(1), cs.GetLength());
//	fp.SetPos(Pos+1);
//	ShowFloat(value, _T("value"));

}

void WriteShortDoubleOnFile(double value, CFile & fp){

	char s1[40];
    _gcvt( value, 5, s1);
	CString cs = CString(s1);
	fp.Write(s1, cs.GetLength());
	cs = CString("  ");
	fp.Write(cs.GetBuffer(2), cs.GetLength());
//	fp.Write(cs.GetBuffer(2), 2);
}



void WriteEndOfLine(CFile & fp){
	char s1[1];
	s1[0]=13;
	fp.Write(s1, 1);
	s1[0]=10;
	fp.Write(s1, 1);
}


#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void FFT(double data[], UINT nn, int isign)
{
	UINT n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	double tempr,tempi;

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}
#undef SWAP


void WaitOK(){
   CWinApp* pApp = AfxGetApp();
   pApp->DoMessageBox(CString(""), MB_OK, MB_APPLMODAL);
}

  
void SaveDouble(double value, CFile & file){
	file.Write(&value, sizeof(double));
}

void SaveFloat(float value, CFile & file){
	file.Write(&value, sizeof(float));
}

void SaveInt(int value, CFile & file){
	//ShowInt(value, "value");
	file.Write(&value, sizeof(int));
}

void SaveBool(BOOL value, CFile & file){
	file.Write(&value, sizeof(BOOL));
}


void SaveString(CString value, CFile & file){
	__int32 l=value.GetLength();
	SaveInt(l, file);
//	ShowInt(l, "l");
	TCHAR t;
//	__int8 t;

	for (int i=0; i<l; i++){
		t=value.GetAt( i ) ;
		file.Write(&t, sizeof(t));
	}
}
void SaveStringWithPos(unsigned __int32 & pos, CString s, CFile & fp){

	unsigned __int64 pos64 = pos;
	SaveStringWithPos(pos64, s, fp);
	pos = (unsigned __int32) pos64;
}

void SaveStringWithPos(unsigned __int64 & pos, CString s, CFile & fp){
	pos += s.GetLength()+sizeof(int);
	SaveString(s, fp);
}


bool LoadDouble(double & value, CFile & file){
	UINT s=sizeof(double);
	UINT read;
	read=file.Read(&value, s);
	return (read==s);
}
bool LoadFloat(float & value, CFile & file){
	UINT s=sizeof(float);
	UINT read;
	read=file.Read(&value, s);
	return (read==s);
}
bool LoadInt16(__int16 & value, CFile & file){
	UINT s=sizeof(__int16);
	UINT read;
	read=file.Read(&value, s);
//	ShowInt(value, "value");
	return (read==s);
}
bool LoadInt(int & value, CFile & file){
	UINT s=sizeof(int);
	UINT read;
	read=file.Read(&value, s);
//	ShowInt(value, "value");
	return (read==s);
}

bool LoadBool(BOOL & value, CFile & file){
	UINT s=sizeof(BOOL);
	UINT read;
	read=file.Read(&value, s);
	return (read==s);
}


bool LoadString(CString & value, CFile & file){
	__int32 l;
	if (!LoadInt(l, file)){
		return false;
	}
//	ShowFloat(l, _T("l"));
	if (l>0){
	    value=CString(_T(" "), l);
	}
	else{
		value = CString(_T(""));
		l=0;
		return true;
	}
//	__int8 t;
	TCHAR t;
	for (int i=0; i<l; i++){
		if (file.Read(&t, sizeof(t))!=sizeof(t)){
			return false;
		}
//		value.SetAt(i, t);
//		char c = (char) t;
//		TCHAR tt=(TCHAR)c;
		TCHAR tt=t;
//		value += CString(tt);
		value.SetAt(i, t);
	}
	
	return true;
}

bool LoadString(CString & value, CFile & file, int MaxLength){
	int l;
	if (!LoadInt(l, file)){
		return false;
	}
    value=CString("");
	TCHAR t;
	if (l>MaxLength) return false;
	for (int i=0; i<l; i++){
		if (file.Read(&t, sizeof(TCHAR))!=sizeof(TCHAR)){
			return false;
		}
		value.SetAt(i, t);
//		value += CString(t);
	}
	return true;
}

bool LoadString(BOOL DoShow, CString & value, CFile & file){
	int l;
	if (!LoadInt(l, file)){
		return false;
	}
	if (DoShow){
		ShowInt(l, _T("length"));
	}
    value=CString("");
	TCHAR t;
	for (int i=0; i<l; i++){
		if (file.Read(&t, sizeof(TCHAR))!=sizeof(TCHAR)){
			return false;
		}
		value += CString(t);
	}
	return true;
}


bool LoadStringWithPos(unsigned __int32 & pos, CString & value, CFile & file){
	unsigned __int64 pos64 = pos;

	bool result = LoadStringWithPos(pos64, value, file);
	pos = (unsigned __int32) pos64;
	return result;
}

bool LoadStringWithPos(unsigned __int64 & pos, CString & value, CFile & file){
	if (!LoadString(value, file)){
		return false;
	}
	pos += value.GetLength()+sizeof(int);
	return true;
}


bool ReadString(CString & c, FILE * f){
	char s[1000];
	if (fscanf(f, "%s", &s) >= 0){
		c = CString(s);
		return true;
	}
	else{
		return false;
	}

}

bool ReadInt(int & i, FILE * f){
	if (fscanf(f, "%d", &i) > 0){
		return true;
	}
	else{
		return false;
	}

}

bool ReadDouble(double & d, FILE * f){
	if (fscanf(f, "%lf", &d) > 0){
		return true;
	}
	else{
		return false;
	}

}

void ConvertDoubleToFixPoint(double d , CString & c){
	char s[256];
	sprintf(s, "%f", d);
	c = CString(s);
	int l=c.GetLength();
	int dotpos=c.Find(_T("."));
	if (dotpos>=(l-1)){
		return;
	}
	if (dotpos<0) return;
	while (true){
		int l=c.GetLength();
		if (l<=dotpos) break;
		if (l<1) break;
		CString cc=c.Mid(l-1,1);
		if (cc.Compare(_T("0"))==0){
			c=c.Left(l-1);
		}
		else{
			break;
		}
	}
//	Alert0(c);
//	ShowFloat(dotpos, "sot");
}


void Randomize(){
	srand( (unsigned)time( NULL ) );
}

double Rand(){
	return MTRand::genrand_real1();
//	return 	double(rand())/double(RAND_MAX);
}

double GaussRand(){
	static int iset=0;
	static double gset;
	double fac, rsq, v1, v2;
	if (iset==0){
		do{
			v1=2.0*Rand()-1.0;
			v2=2.0*Rand()-1.0;
			rsq = v1*v1 + v2*v2;
		}
		while((rsq>=1.0) | (rsq==0.0));
		fac = sqrt(-2.0*log(rsq)/rsq);
		gset = v1*fac;
		iset = 1;
		return v2*fac;
	}
	else{
		iset = 0;
		return gset;
	}
}


double RandLargerZero(){
	while (1){
		double r=Rand();
		if (r>0) return r;
	}
}




void dft(int npt, int inv, double * realin, double * imin,
							double * realout, double * imout){
		if (npt<2) return;
        int k,n;
	//	ShowFloat(npt, "npt");
        double WN, wk, c, s;
		double pi = 3.141592653589793;
        WN=2*pi/npt;
        if(inv==1) WN=-WN;

	//	int ii=npt/10;
	//	int iii=0;
		double * cc = new double [npt+1];
		double * ss = new double [npt+1];
        for (k=0;k<npt;k++){
			wk=k*WN;
			cc[k]=cos(wk);
			ss[k]=sin(wk);
		}


        for (k=0;k<npt;k++){
	//		if (iii==ii){
	//			ShowFloat(k, "k");
	//			iii=0;
	//		}
	//		iii++;
			realout[k]=0;
			imout[k]=0;
//			wk=k*WN;
            for (n=0; n<npt;n++){
//               c=cos(double(n)*wk);
//                s=sin(double(n)*wk);
				c=cc[n*k % npt];
				s=ss[n*k % npt];
                realout[k] += realin[n]*c+imin[n]*s;
                imout[k] = imout[k]-realin[n]*s+imin[n]*c;
            }
            if (inv==1){       /* divide by N for IDFT */
	            realout[k] /= (0.5*double(npt));
		        imout[k] /= (0.5*double(npt));
            }
       }
		delete [] ss;
		delete [] cc;
}




void DoFilterDFT(double f, __int16 * in, __int16* out, int NData){
	if (NData<2) return;
	if (f<=0) return;
	if (f>=1) return;
	double * realin = new double [NData];
	double * realout = new double [NData];
	double * imin = new double [NData];
	double * imout = new double [NData];
	int k;
	for (k=0; k<NData; k++){
		realin[k]= double(in[k]);
		imin[k]=0;
	}
	double Mean = MeanDouble (realin, NData);
	for (k=0; k<NData; k++){
		realin[k] -= Mean;
	}
	
	dft(NData, 0, realin, imin, realout, imout);
	int NMax=int (double(NData)*f);
	for (k=0; k<NData; k++){
		double temp=double(k)/double(NMax);
		temp *= temp;
		temp *= temp;
//		temp *= temp;
		realout[k] *= 1/(1+temp);
		imout[k] *= 1/(1+temp);
	}

/*

	for (k=NMax; k<NData; k++){
		realout[k]= 0;
		imout[k]=0;
	}
*/
	dft(NData, 1, realout, imout, realin, imin);
	for (k=0; k<NData; k++){
		out[k]= __int16(realin[k]+Mean);
	}
	delete [] imout;
	delete [] imin;
	delete [] realout;
	delete [] realin;

}
void DoFilterDFT(double f, __int32 * in, __int32* out, int NData){
	if (NData<2) return;
	if (f<=0) return;
	if (f>=1) return;
	double * realin = new double [NData];
	double * realout = new double [NData];
	double * imin = new double [NData];
	double * imout = new double [NData];
	int k;
	for (k=0; k<NData; k++){
		realin[k]= double(in[k]);
		imin[k]=0;
	}
	double Mean = MeanDouble (realin, NData);
	for (k=0; k<NData; k++){
		realin[k] -= Mean;
	}
	
	dft(NData, 0, realin, imin, realout, imout);
	int NMax=int (double(NData)*f);
	for (k=0; k<NData; k++){
		double temp=double(k)/double(NMax);
		temp *= temp;
		temp *= temp;
//		temp *= temp;
		realout[k] *= 1/(1+temp);
		imout[k] *= 1/(1+temp);
	}


	dft(NData, 1, realout, imout, realin, imin);
	for (k=0; k<NData; k++){
		out[k]= __int32(realin[k]+Mean);
	}
	delete [] imout;
	delete [] imin;
	delete [] realout;
	delete [] realin;

}

__int16 MeanInt16(__int16 * d, int np){
	if (np==0) return 0;
	double res=d[0];
	for (int i=1; i<np; i++)
		res += d[i];

	res /= np;
	__int16 ires=__int16(res);
	return ires;

}
__int16 MaxInt16(__int16 * d, int np){
	if (np==0) return 0;
	__int16 res=d[0];
	for (int i=1; i<np; i++)
		if (d[i]>res){
		res=d[i];
	}
	return res;
}

__int16 MinInt16(__int16 * d, int np){
	if (np==0) return 0;
	__int16 res=d[0];
	for (int i=1; i<np; i++)
		if (d[i]<res){
		res=d[i];
	}
	return res;
}

__int16 AbsMaxInt16(__int16 * d, int np){
	if (np==0) return 0;
	__int16 res=abs(d[0]);
	for (int i=1; i<np; i++)
		if (abs(d[i])>res){
		res=abs(d[i]);
	}
	return res;
}
int AbsMaxInt(int * d, int np){
	if (np==0) return 0;
	int res=abs(d[0]);
	for (int i=1; i<np; i++)
		if (abs(d[i])>res){
		res=abs(d[i]);
	}
	return res;
}

bool ReadDouble(FILE * fp, double & x){
	if (fscanf(fp, "%lf", &x) == 1)
		return true;
	else
		return false;
}


double loggamma(double xx){
	
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091, -1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;
	double x, y, tmp, ser;

	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0; j<=5; j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);

}

double beta (double x, double w){
	
	return exp(loggamma(x)+loggamma(w)-loggamma(x+w));

}

double mypow(double x, double y){
	if (x<=0) return 0;

	if (y>0){
		return pow(x,y);
	}
	if (y==0){
		return 1;
	}
	return  1/pow(x, -y);
}



double BetaDensity (double x, double a, double b){

	double w=mypow(x, a-1);
	double z=mypow(1-x, b-1);

	return w*z/beta(a,b);

}



double GaussDensity (double x, double sigma){

	static double c= 0.3989422804014327;
	double y= x/sigma;

	return c/sigma*exp(-y*y/2);

}

/*

double ConvolutedBeta (double x, double a, double b, double sigma, int N){
//	return BetaDensity(x,a,b)/beta(a,b);
	double sum=0;
	double y=0;
	if (N<2){
		N=2;
	}
	double dy=1/double(N);
//	ShowFloat(dy, "dy");
	int i=0;
	for (i=0; i<=N;i++){
		y=double(i)*dy;
		sum += BetaDensity(y,a,b)*GaussDensity(x-y, sigma);
//		sum += BetaDensity(y,a,b);//*GaussDensity(x-y, sigma);
//		sum += GaussDensity(x-y, sigma);
//		ShowFloat(BetaDensity(y,a,b), "beta");
//		ShowFloat(GaussDensity(x-y, sigma), "gaus");
//		y += dy;
	}
	return sum/double (N+1)/beta (a,b);
}


double CalcFittedBeta(double xinpA, double * p){
	double a= p[0];
	double b= p[1];
	double Amp=p[2];

	double i=p[4];
	double i0=p[5];
	double Iopen=i-i0;
	double SigmaNorm= fabs(p[3]/Iopen);
	int N= p[6];

//	ShowFloat(xinpA, "x in pa");
//	ShowFloat(a, "a");
//	ShowFloat(b, "b");
//	ShowFloat(Amp, "Amp");
//	ShowFloat(i, "i");
//	ShowFloat(i0, "i0");
//	ShowFloat(Iopen, "Iopen");
//	ShowFloat(SigmaNorm, "s");
//	ShowFloat(N, "N");

	double inorm = (xinpA-i0)/Iopen;

//	double f = ConvolutedBeta( inorm, a, b, SigmaNorm, N);
//	if ((inorm>0.9) & (inorm<1)){
//		ShowFloat(inorm, "inorm");
//		ShowFloat(f, "f");
//	}
	return Amp*ConvolutedBeta( inorm, a, b, SigmaNorm, N);
}

*/

void MakeBetaTrace(double a, double b, double ymin, double ymax, int N, double * buffer){
	double dy=(ymax-ymin)/double(N);
	//ShowFloat(ymin, "ymin");
	//ShowFloat(ymax, "ymax");
	//ShowFloat(N, "N");
	//owFloat(dy, "dy");
	for (int i=0; i<N; i++){
		double y = ymin+double(i)*dy;
		//ShowFloat(y, "y");
		buffer[i]=BetaDensity(y, a, b);
	}
}

void FilterBetaTrace(double sigmanorm, double ymin, double ymax, double * bufin, double * bufout, int N){
	double dy=(ymax-ymin)/double(N); 

	double freq=fabs(0.1325*dy/sigmanorm);

//	DoFilterDouble(freq, bufin, bufout, N);
	DoFilter<double>(freq, bufin, bufout, N);

}

void MyCFile::Write( const void* lpBuf,unsigned __int64 nCount ){
	UINT NMax = UINT(nCount/unsigned __int64 (UINT_MAX));
//	ShowFloat(NMax, "NMax");
	char * MyBuff = (char *) lpBuf;
	for (UINT i=0; i<NMax; i++){
		CFile::Write(MyBuff, UINT_MAX);
		nCount -= unsigned __int64(UINT_MAX);
		MyBuff += unsigned __int64(UINT_MAX);
	}
	UINT Rest = UINT(nCount);
//	ShowFloat(Rest, "Rest");
	CFile::Write(MyBuff, Rest);

}
		

int WriteBytesNoSeek(CFile * fp, unsigned __int64 size, void * buff){
    fp->Write (buff , size);
	return 1;
}

int WriteBytes(CFile * fp, unsigned __int64 & pos, unsigned __int64 size, void * buff){
    fp->Seek (pos, SEEK_SET);
    fp->Write (buff , size);
	
	pos += size;
	return 1;
}

int ReadBytes(CFile * fp, unsigned __int64 & pos, unsigned __int64 size, void * buff){
	unsigned __int64 read;
    fp->Seek (pos, SEEK_SET);
    read=fp->Read (buff , size);
    if (read != size) {
//	    MessageBox(NULL, "Could read enough bytes", "ERROR LOADING FILE", MB_OK);
		return 0;
	}
	pos += size;
	return 1;
}

int ReadBytesNoSeek(CFile * fp, unsigned __int64 size, void * buff){
	unsigned __int64 read;
    read=fp->Read (buff , size);
    if (read != size) {
//	    MessageBox(NULL, "Could read enough bytes", "ERROR LOADING FILE", MB_OK);
		return 0;
	}
	return 1;
}


double FitKd(double * c, double * data, double * fit, int ndata,
			 BOOL FitMax, double & KD, double & IMax, bool BlockType){
	if (ndata<1) return 0;
	if (!data) return 9;
	if (!c) return 0;
	double params[2];
	KD=0;
	int i;
	for (i=0; i<ndata; i++){
		KD += c[i];
	}
	KD /= double(ndata);
//	ShowFloat(KD, "ENTER: KD");
	params[0] = KD;
	double Best = 1e200;
	double BestKD, BestIMax;
	Simplex Simp;
	Simp.InitSimplex(0.5, params, 1);
	int iter;
	for (iter=0; iter<500; iter++){
		KD=fabs(params[0]);
		for (i=0; i<ndata; i++){
			fit[i]=1/(1+c[i]/KD);
			if (!BlockType){
				fit[i] *= c[i]/KD;
			}
		}
		if (FitMax){
			double sum1=0;
			double sum2=0;
			for (i=0; i<ndata; i++){
				double f=fit[i];
				sum1 += f*f;
				sum2 += f*data[i];
			}
			IMax = sum2/sum1;
		}
		else{
			IMax = 1;
		}
		double SD=0;
		for (i=0; i<ndata; i++){
			double f=IMax*fit[i] - data[i];
			SD += f*f;
		}
		if (SD<Best){
			Best=SD;
			BestKD=KD;
//			ShowFloat(KD, "better KD");
//			ShowFloat(SD, "SD");
			BestIMax = IMax;
		}
		Simp.NextSimp(params, SD);
	}
	KD=BestKD;
	IMax=BestIMax;
	for (i=0; i<ndata; i++){
		fit[i]=IMax/(1+c[i]/KD);
		if (!BlockType){
			fit[i]*=c[i]/KD;
		}
	}
	return Best;
}

bool ReadOneDoubleInLine(FILE * fp, double & x){
	if (fscanf(fp, "%lf", &x) == 1)
		return true;
	else
		return false;
}

bool ReadTwoDoublesInLine(FILE * fp, double & x, double & y){
	if (!ReadOneDoubleInLine(fp, x)) return false;
	return ReadOneDoubleInLine(fp, y);
//	if (fscanf(fp, "%lf%lf",&x, &y) == 1)
//		return true;
//	else
//		return false;
}

bool GetLineFromString(CString & c, CString & line){
	int length=c.GetLength();
	line=CString (' ', 81);
	line="";
	int count=0;
	char b;
	int pos=0;
	while(true){
		if (pos>=length){
			goto EndGetLineFromString;
		}
		b = c.GetAt(pos);
		pos++;
		if (b==10){
			// EOL
			goto EndGetLineFromString;
		}
		if (b==13){
			// check if next char is 10
			if (pos==length){
				goto EndGetLineFromString;
			}

		    b=c.GetAt(pos); //read=f.Read (&b, 1);
			pos++;

			if (b==10 ){	// EOL
				goto EndGetLineFromString;
			}
			// so next character is good, have to step back one:
			pos--;
			goto EndGetLineFromString;
		}
		count ++;
		line += b;
	}
EndGetLineFromString:
	c = c.Right(length-pos);
	if (count>0){
		return true;
	}
	else{
		return false;
	}

}
bool GetLineFromStringWithPos(CString & c, CString & line, int & pos){
	int length=c.GetLength();
	line=CString (' ', 81);
	line="";
	int count=0;
	char b;
//	int pos=0;
	while(true){
		if (pos>=length){
			goto EndGetLineFromString;
		}
		b = c.GetAt(pos);
		pos++;
		if (b==10){
			// EOL
			goto EndGetLineFromString;
		}
		if (b==13){
			// check if next char is 10
			if (pos==length){
				goto EndGetLineFromString;
			}

		    b=c.GetAt(pos); //read=f.Read (&b, 1);
			pos++;

			if (b==10 ){	// EOL
				goto EndGetLineFromString;
			}
			// so next character is good, have to step back one:
			pos--;
			goto EndGetLineFromString;
		}
		count ++;
		line += b;
	}
EndGetLineFromString:
//	c = c.Right(length-pos);
	if (count>0){
		return true;
	}
	else{
		return false;
	}

}

bool GetLineFromStringWithPos(std::string & c, std::string & line, int & pos){
//	return false;
//	CString c;
//	CString line;
	int length=c.length();

	line=std::string(' ', 81);
	line="";
	int count=0;
	char b;
//	int pos=0;
	while(true){
		if (pos>=length){
			goto EndGetLineFromStringstd;
		}
		b = c.at(pos);
		pos++;
		if (b==10){
			// EOL
			goto EndGetLineFromStringstd;
		}
		if (b==13){
			// check if next char is 10
			if (pos==length){
				goto EndGetLineFromStringstd;
			}

		    b=c.at(pos); //read=f.Read (&b, 1);
			pos++;

			if (b==10 ){	// EOL
				goto EndGetLineFromStringstd;
			}
			// so next character is good, have to step back one:
			pos--;
			goto EndGetLineFromStringstd;
		}
		count ++;
		line += b;
	}
EndGetLineFromStringstd:
//	c = c.Right(length-pos);
	if (count>0){
		return true;
	}
	else{
		return false;
	}

}

bool ReadNDoublesInString(int N, CString & cc, double * x){
	if (N==1){
		double value;
		bool Result = ReadOneDoubleInString(cc, value);
		x[0] = value;
		return Result;
	}
	return false;


}

bool ReadOneDoubleInString(CString & cc, double & x){
	CString c = cc.TrimLeft();
	int l=c.GetLength();
	if (l<3) return false;

	CString space = CString(_T(" "));
	int ispace = -1;
	for (int i=1; i<l; i++){
		int j=c.GetAt(i);
		if (j<33){
			ispace=i;
			break;
		}
	}
//	int ispace = c.Find(_T(" "));
//	int ispace = c.Find(space);
//	Alert0(c);
//	ShowFloat(ispace, _T("isp"));

	
	if (ispace>=(l-1)){
//		Alert0(_T(">="));
		ispace = l-1;
//		return false;
	}
	if (ispace <= 0){
//		Alert0(_T("<="));
		ispace = l-1;
//		return false;
	}

	CString cx = c.Left(ispace+1);
	CString cy = c.Right(l-ispace);

//	Alert0(cx);
	x=atof(CStringA(cx));
//	y=atof(CStringA(cy));
//	ShowFloat(x, _T("x"));
	cc = cy;
	return true;
}
bool ReadOneDoubleInString(std::string & ccc, double & x){
	return false;
	CString cc;
	CString c = cc.TrimLeft();
	int l=c.GetLength();
	if (l<3) return false;

	CString space = CString(_T(" "));
	int ispace = -1;
	for (int i=1; i<l; i++){
		int j=c.GetAt(i);
		if (j<33){
			ispace=i;
			break;
		}
	}
//	int ispace = c.Find(_T(" "));
//	int ispace = c.Find(space);
//	Alert0(c);
//	ShowFloat(ispace, _T("isp"));

	
	if (ispace>=(l-1)){
//		Alert0(_T(">="));
		ispace = l-1;
//		return false;
	}
	if (ispace <= 0){
//		Alert0(_T("<="));
		ispace = l-1;
//		return false;
	}

	CString cx = c.Left(ispace+1);
	CString cy = c.Right(l-ispace);

//	Alert0(cx);
	x=atof(CStringA(cx));
//	y=atof(CStringA(cy));
//	ShowFloat(x, _T("x"));
	cc = cy;
	return true;
}

bool ReadOneIntInString(CString & cc, int & x){
	CString c = cc.TrimLeft();
	int l=c.GetLength();
	if (l<1) return false;

//	CString space = CString(_T(" "));
	int ispace = -1;
	for (int i=1; i<l; i++){
		int j=c.GetAt(i);
		if (j<33){
			ispace=i;
			break;
		}
	}
//	int ispace = c.Find(_T(" "));
//	int ispace = c.Find(space);
//	Alert0(c);
//	ShowFloat(ispace, _T("isp"));

	
	if (ispace>=(l-1)){
//		Alert0(_T(">="));
		ispace = l-1;
//		return false;
	}
	CString cx, cy;
	if (ispace <= 0){
		cx=c; 
		cy="";
//		Alert0(_T("<="));
//		ispace = l-1;
//		return false;
	}
	else{

		cx = c.Left(ispace);
		cy = c.Right(l-ispace);
	}

	x=atoi(CStringA(cx));
//	y=atof(CStringA(cy));
//	ShowFloat(x, _T("x"));
	cc = cy;
	return true;
}

bool ReadTwoDoublesInString(CString & cc, double & x, double & y){
//	if (!ReadOneDoubleInString(cc, x)){
//		return false;
//	}
//	return ReadOneDoubleInString(cc, y);
	CString c = cc.TrimLeft();
	int l=c.GetLength();
	if (l<3) return false;

	CString space = CString(_T(" "));
	int ispace = -1;
	for (int i=1; i<l; i++){
		int j=c.GetAt(i);
		if (j<33){
			ispace=i;
			break;
		}
	}
//	int ispace = c.Find(_T(" "));
//	int ispace = c.Find(space);
//	Alert0(c);
//	ShowFloat(ispace, _T("isp"));

	
	if (ispace>=(l-1)){
//		Alert0(_T(">="));
		return false;
	}
	if (ispace <= 0){
//		Alert0(_T("<="));
		return false;
	}

	CString cx = c.Left(ispace);
	CString cy = c.Right(l-ispace);

	x=atof(CStringA(cx));
	y=atof(CStringA(cy));
//	ShowFloat(x, _T("x"));
	return true;
}
bool ReadThreeDoublesInString(CString & cc, double & x, double & y, double & z){
//	if (!ReadTwoDoublesInString(c, x, y)){
//		return false;
//	}
//	return ReadOneDoubleInString(c, z);
	CString c = cc.TrimLeft();
	int l=c.GetLength();
	if (l<3) return false;

	CString space = CString(_T(" "));
	int ispace = -1;
	for (int i=1; i<l; i++){
		int j=c.GetAt(i);
		if (j<33){
			ispace=i;
			break;
		}
	}
//	int ispace = c.Find(_T(" "));
//	int ispace = c.Find(space);
//	Alert0(c);
//	ShowFloat(ispace, _T("isp"));

	
	if (ispace>=(l-1)){
//		Alert0(_T(">="));
		return false;
	}
	if (ispace <= 0){
//		Alert0(_T("<="));
		return false;
	}

	CString cx = c.Left(ispace);
	CString cy = c.Right(l-ispace);

	x=atof(CStringA(cx));
	ReadTwoDoublesInString(cy, y, z);
//	y=atof(CStringA(cy));
//	ShowFloat(x, _T("x"));
	return true;
}

bool ReadThreeDoublesInLine(FILE * fp, double & x, double & y, double & z){
	if (!ReadTwoDoublesInLine(fp, x, y)) return false;
	return ReadOneDoubleInLine(fp, z);
//	if (fscanf(fp, "%lf%lf%lf", &x, &y, &z) == 1)
//		return true;
//	else
//		return false;
}


bool ReadLine(CFile & f, CString & c, int EstimatedLength){ // removes all eol chararcters!
	if (EstimatedLength<81) EstimatedLength = 81;
	c=CString (' ', EstimatedLength);
	c="";
	int read;
	int count=0;
	char b;
	while(true){
	    read=f.Read (&b, 1);
	    if (read != 1){
			// EOF
			return (count != 0);
		}
		if (b==10){
			// EOL
			return true;
		}
		if (b==13){
			// check if next char is 10
		    read=f.Read (&b, 1);

			if  (read != 1){
				// EOF
				return (count != 0);
			}
			if (b==10 ){
					// EOL
				return true;
			}
			// so next character is good, have to step back one:
			unsigned __int64 pos = f.GetPosition( );
			f.Seek(pos-1, SEEK_SET);
			return (count!=0);
		}
		count ++;
		c += b;
	}

}

void TimeToString(double UnixTime, CString & timestring){
	time_t unixtime= time_t(UnixTime);
//	timestring = CString(ctime(&(unixtime)));

	struct tm * ptm = gmtime( &unixtime );

	ptm->tm_year += 20;

//	time_t corrected = mktime(ptm);

//	struct tm * ptm2 = gmtime( &unixtime );

	timestring = CString(asctime(ptm));

	timestring = timestring.Left(timestring.GetLength()-1);
	timestring = timestring.Right(timestring.GetLength()-3);

//					ShowDouble(ptm->tm_year, 600, position, pDC);
//					ShowDouble(unixtime, 600, position, pDC);

}

bool DoFindFile(CString Name, CString dir, CString & CopyName, UINT & LengthCopy){
	unsigned __int64 LongLengthCopy = unsigned __int64 (LengthCopy);
	bool result = DoFindFile(Name, dir, CopyName, LongLengthCopy);
	LengthCopy = UINT(LongLengthCopy);
	return result;
}

bool DoFindFile(CString Name, CString dir, CString & CopyName, unsigned __int64 & LengthCopy){
	CFileFind ff;
	LengthCopy = 0;
	Name.MakeUpper();

	CString start = dir + CString("/*.*");
	bool result = ff.FindFile(start, 0);
	if (!result) return false;

	while (true){
		int ffresult = ff.FindNextFile();
		CString c=ff.GetFilePath();
		CString cbad=c.Right(1);
		CString cbad2=c.Right(2);
		if ((cbad.Compare(CString("."))!=0) & (cbad2.Compare(CString(".."))!=0)){

			if (ff.IsDirectory()){// &
				if (DoFindFile(Name, c, CopyName, LengthCopy)){
					return true;
				}
			}
			else{
				CString NameOrig=ff.GetFileName();
				NameOrig.MakeUpper();

				if (NameOrig.Compare(Name)==0){
					CopyName=ff.GetFilePath();
					LengthCopy = ff.GetLength();
					return true;
				}
			}
		}
		if (ffresult==0) break;
	}
	return false;
}

// BITMAP FUNCTIONS:


#define DSa       0x008800C6L
#define DSo       0x00EE0086L
#define DSx       0x00660045L
#define DSPDxax   0x00E20746L

#define RGBBLACK     RGB(0,0,0)
#define RGBWHITE     RGB(255,255,255)

/*************************************************************************
 *
 * CopyBitmap()
 *
 * This function makes a copy of a bitmap.
 *
 * Parameters:
 *
 *    HBITMAP hbmSrc - Handle to source bitmap
 *
 * Return value:
 *
 *    Handle to the new bitmap if successful, or NULL if not.
 *
 * History:   
 *            
 *    Date      Author             Reason        
 *    6/10/91   Charlie Kindel     Created
 *    3/26/92   Mark Bader         Fixed allocated size, commented
 *
 *************************************************************************/

HBITMAP FAR PASCAL CopyBitmap(HBITMAP hbmSrc)
{
   HBITMAP hbmDest = NULL;
   BITMAP BM;
   LPSTR lpBits;
   DWORD dwNumBytes;
   GLOBALHANDLE hMem;

   if (!hbmSrc)
      return NULL;

   // Get BITMAP structure from source bitmap
   GetObject(hbmSrc, sizeof(BITMAP), (LPSTR)&BM);

   // Figure out how much space we need for the
   // bitmap bits, and alloc the space

   dwNumBytes = (DWORD)BM.bmHeight * BM.bmWidthBytes;

   if (hMem = GlobalAlloc(GHND, dwNumBytes))
   {
      // Make a new bitmap the same as the source (except this call
      // does not copy the bits, we have to do that ourselves)

      if (hbmDest = CreateBitmapIndirect(&BM))
      {
         if (!(lpBits = (LPSTR)GlobalLock(hMem)))
         {
            DeleteObject(hbmDest);
            return NULL;
         }

         // Now copy the bits

         GetBitmapBits(hbmSrc, dwNumBytes, lpBits);
         SetBitmapBits(hbmDest, dwNumBytes, lpBits);

         GlobalUnlock(hMem);
         GlobalFree(hMem);
      }
      else
      {
         GlobalFree(hMem);
         return NULL;
      }
   }
   else
      return NULL;

   return hbmDest;
}                  /* CopyBitmap()  */


/*************************************************************************
 *
 * ChangeBitmapColorDC()
 *
 * This function makes all pixels in the given DC that have the
 * color rgbOld have the color rgbNew.  This function is used by
 * ChangeBitmapColor().
 *
 * Parameters:
 *
 * HDC hdcBM        - Memory DC containing bitmap
 * LPBITMAP lpBM    - Long pointer to bitmap structure from hdcBM
 * COLORREF rgbOld  - Source color
 * COLORREF rgbNew  - Destination color
 *
 * Return value: none.
 *
 * History:   Date      Author      Reason
 *            6/10/91   CKindel     Created
 *            1/23/92   MarkBad     Added big nifty comments which explain
 *                                  how this works, split bitmap graying 
 *                                  code out
 *
 *************************************************************************/

void ChangeBitmapColorDC(CDC * hdcBM, LPBITMAP lpBM,
                                     COLORREF rgbOld, COLORREF rgbNew)

//VOID FAR PASCAL ChangeBitmapColorDC (HDC hdcBM, LPBITMAP lpBM,
//                                     COLORREF rgbOld, COLORREF rgbNew)
{
//   HDC hdcMask;
   CDC hdcMask;
//   HBITMAP hbmMask, hbmOld;
   CBitmap  hbmMask;
   CBitmap *  hbmOld;


   HBRUSH hbrOld;

   if (!lpBM)
      return;

   //
   // if the bitmap is mono we have nothing to do
   //

   if (lpBM->bmPlanes == 1 && lpBM->bmBitsPixel == 1){
	   Alert0(_T(" is mono"));
      return;
	}

   //
   // To perform the color switching, we need to create a monochrome 
   // "mask" which is the same size as our color bitmap, but has all 
   // pixels which match the old color (rgbOld) in the bitmap set to 1.  
   // 
   // We then use the ROP code "DSPDxax" to Blt our monochrome 
   // bitmap to the color bitmap.  "D" is the Destination color 
   // bitmap, "S" is the source monochrome bitmap, and "P" is the 
   // selected brush (which is set to the replacement color (rgbNew)).  
   // "x" and "a" represent the XOR and AND operators, respectively.
   // 
   // The DSPDxax ROP code can be explained as having the following 
   // effect:
   // 
   // "Every place the Source bitmap is 1, we want to replace the 
   // same location in our color bitmap with the new color.  All 
   // other colors we leave as is."
   // 
   // The truth table for DSPDxax is as follows:
   // 
   //       D S P Result
   //       - - - ------
   //       0 0 0   0
   //       0 0 1   0
   //       0 1 0   0
   //       0 1 1   1
   //       1 0 0   1
   //       1 0 1   1
   //       1 1 0   0
   //       1 1 1   1
   // 
   // (Even though the table is assuming monochrome D (Destination color), 
   // S (Source color), & P's (Pattern color), the results apply to color 
   // bitmaps also). 
   //
   // By examining the table, every place that the Source is 1 
   // (source bitmap contains a 1), the result is equal to the 
   // Pattern at that location.  Where S is zero, the result equals 
   // the Destination.  
   // 
   // See Section 11.2 (page 11-4) of the "Reference -- Volume 2" for more
   // information on the Termary Raster Operation codes.
   //

//   hbmMask = new CBitmap;
//   if (hbmMask = CreateBitmap(lpBM->bmWidth, lpBM->bmHeight, 1, 1, NULL))

//   ShowFloat(lpBM->bmWidth, "lpBM->bmWidth");
   if (hbmMask.CreateBitmap(lpBM->bmWidth, lpBM->bmHeight, 1, 1, NULL))
   {
//      if (hdcMask = CreateCompatibleDC(hdcBM))
      if (hdcMask.CreateCompatibleDC(hdcBM))
      {
      //
      // Select th mask bitmap into the mono DC
      //
         hbmOld = (CBitmap * )( SelectObject(hdcMask, hbmMask));

         // 
         // Create the brush and select it into the source color DC --
         // this is our "Pattern" or "P" color in our DSPDxax ROP.
         //

         hbrOld = (HBRUSH ) hdcBM->SelectObject(CreateSolidBrush(rgbNew));

         // 
         // To create the mask, we will use a feature of BitBlt -- when
         // converting from Color to Mono bitmaps, all Pixels of the
         // background colors are set to WHITE (1), and all other pixels
         // are set to BLACK (0).  So all pixels in our bitmap that are
         // rgbOld color, we set to 1.
         //

         hdcBM->SetBkColor(rgbOld);
         hdcMask.BitBlt(0, 0, lpBM->bmWidth, lpBM->bmHeight,
                hdcBM, 0, 0, SRCCOPY);

         //
         // Where the mask is 1, lay down the brush, where it is 0, leave
         // the destination.
         //

         hdcBM->SetBkColor( RGBWHITE);
         hdcBM->SetTextColor(RGBBLACK);

         hdcBM->BitBlt( 0, 0, lpBM->bmWidth, lpBM->bmHeight,
                &hdcMask, 0, 0, DSPDxax);

         SelectObject(hdcMask, hbmOld);

         hbrOld = (HBRUSH ) (hdcBM->SelectObject(hbrOld));
         DeleteObject(hbrOld);

         DeleteDC(hdcMask);
      }
      else{
		  Alert0(_T("could not create compatible dc"));
         return;
	  }

      DeleteObject(hbmMask);
   }
   else{
	   Alert0(_T("Could not create bitmap"));
      return;
	}
}                  /* ChangeBitmapColorDC()  */




/*************************************************************************
 *
 * ChangeBitmapColor()
 *
 *    This function translates the colors in a bitmap from one
 *    to another.
 *
 * Parameters:
 *
 *    HBITMAP hbmSrc  - Bitmap to process
 *    COLORREF rgbOld - Source color
 *    COLORREF rgbNew - Destination color
 *
 * Return value: none.
 *
 * History:   Date      Author      Reason
 *            6/10/91   CKindel     Created
 *            1/28/92   MarkBad     Split "Graying" functionality out into
 *                                    separate API, added palette param
 *
 *************************************************************************/

/*

void ChangeBitmapColor ( BITMAP * hbmSrc, COLORREF rgbOld,
                                   COLORREF rgbNew, HPALETTE hPal)
{
//   HDC hDC=pDC->m_hDC;
	HDC hDC;
   HDC hdcMem;
   BITMAP bmBits;

   if (hDC = GetDC(NULL)){
//   if (hDC){
      if (hdcMem = CreateCompatibleDC(hDC))
      {
      //
      // Get the bitmap struct needed by ChangeBitmapColorDC()
      //
         GetObject(hbmSrc, sizeof(BITMAP), (LPSTR)&bmBits);

         //
         // Select our bitmap into the memory DC
         //
         hbmSrc= (BITMAP * ) SelectObject(hdcMem, hbmSrc);

         // Select in our palette so our RGB references will come
         // out correctly

         if (hPal) {
            SelectPalette(hdcMem, hPal, FALSE);
            RealizePalette(hdcMem);
            }

         //
         // Translate the sucker
         //
         ChangeBitmapColorDC(hdcMem, &bmBits, rgbOld, rgbNew);

         //
         // Unselect our bitmap before deleting the DC
         //
         hbmSrc = (BITMAP *)SelectObject(hdcMem, hbmSrc);

         DeleteDC(hdcMem);
      }

      ReleaseDC(NULL, hDC);
   }
}                 // ChangeBitmapColor()  



*/

//**************************************************************************
//
// SwapBitmapColors()
//
// Purpose:
//   
//   Exchanges all colors in bitmap with color "cr1" with color "cr2".
//   
// Parameters:
//
// HDC hdcBM        - Memory DC containing bitmap
// LPBITMAP lpBM    - Long pointer to bitmap structure from hdcBM
// COLORREF rgbOld  - Source color
// COLORREF rgbNew  - Destination color
//
// Return value: none.
//
// Comments:
//
// The way this function works is by creating 2 monochrome "masks" 
// which will be used to "cut out" the first and second colors:
//
// 1. Our first mask will have all pixels of Color #1 turned into 
// WHITE (1) in the mask, and all other pixels turned to BLACK (0).  
//
// 2. Our second mask will have all pixels of Color #2 turned into 
// WHITE (1) in the mask, and all other pixels turned to BLACK (0).
//
// With the above 2 masks, we call BitBlt with the DSPDxax ROP code
// twice to do the work.  For an explanation of the DSPDxax ROP code
// and how it works, see the function ChangeBitmapColorDC() above.
//
// History:    Date       Author        Comment
//             3/26/92    Mark Bader    Created it!
//
//**************************************************************************

//void SwapBitmapColors (HBITMAP hBmSource, COLORREF cr1,
void SwapBitmapColors (CBitmap * hBmSource, COLORREF cr1,
                                    COLORREF cr2, HPALETTE hPal) {

  HDC hDCMask1,          // hDC for Mask #1
      hDCMask2,          // hDC for Mask #2
      hDCSource;         // hDC which contains original bitmap

  HPALETTE hOldPal;      // Keeps track of old palette

  CBitmap hBmMask1,      // Bitmap for Mask 1
          hBmMask2,      // Bitmap for Mask 2
          * hBmOldSource,  // Old bitmaps
          * hBmOld1,
          * hBmOld2;

  int bmW, bmH;          // Bitmap Width & Height
  HDC hDC;
  BITMAP bm;

  HBRUSH hBr1, hBr2, hBrOld;

  // Create DCs for each of our bitmaps we are going to manipulate

  hDC = GetDC(NULL);
  hDCMask1 = CreateCompatibleDC(hDC);
  hDCSource = CreateCompatibleDC(hDC);
  hDCMask2 = CreateCompatibleDC(hDC);

  // hDCSource will just contain our original bitmap.

  hBmOldSource = (CBitmap *)SelectObject(hDCSource, hBmSource);

     // Select in our palette so our RGB references will come
     // out correctly

  if (hPal) {
        hOldPal = SelectPalette(hDCSource, hPal, false);
        RealizePalette(hDCSource);
  }


  GetObject(hBmSource, sizeof(BITMAP), &bm);

  bmW = bm.bmWidth;
  bmH = bm.bmHeight;

  // Create MONO bitmaps for each of the masks

//  hBmMask1 = new CBitmap;

  hBmMask1.CreateBitmap(bmW, bmH, 1, 1, NULL);
  hBmMask2.CreateBitmap(bmW, bmH, 1, 1, NULL);

  hBmOld1 = (CBitmap * )SelectObject(hDCMask1, hBmMask1);
  hBmOld2 = (CBitmap * )SelectObject(hDCMask2, hBmMask2);

  ReleaseDC(NULL, hDC);

  // To create the masks, we will use a feature of BitBlt -- when
  // converting from Color to Mono bitmaps, all Pixels of the
  // background colors are set to WHITE (1), and all other pixels
  // are set to BLACK (0).  This is how we will create the two
  // masks.

  // Transform into a Mono bitmap with all pixels of color "cr1"
  // in the color bitmap being WHITE in the new bitmap (all else is black)

  SetBkColor(hDCSource, cr1);
  BitBlt(hDCMask1, 0, 0, bmW, bmH, hDCSource, 0, 0, SRCCOPY);

  // Get second mono mask same way, but with color 2

  SetBkColor(hDCSource, cr2);
  BitBlt(hDCMask2, 0, 0, bmW, bmH, hDCSource, 0, 0, SRCCOPY);


  // Now apply the magic ROP code

  hBr2 = CreateSolidBrush(cr2);

  hBrOld = (HBRUSH)SelectObject(hDCSource, hBr2);

  SetBkColor(hDCSource, RGBWHITE);
  SetTextColor(hDCSource, RGBBLACK);

  BitBlt(hDCSource, 0, 0, bmW, bmH, hDCMask1, 0, 0, DSPDxax);

  hBr1 = CreateSolidBrush(cr1);
  SelectObject(hDCSource, hBr1);

  BitBlt(hDCSource, 0, 0, bmW, bmH, hDCMask2, 0, 0, DSPDxax);

  // Clean up

  SelectObject(hDCSource, hBrOld);
  DeleteObject(hBr1);
  DeleteObject(hBr2);

  SelectObject(hDCMask1, hBmOld1);
//  DeleteObject(hBmMask1); // IS NOW ON STACK

  SelectObject(hDCMask2, hBmOld2);
//  DeleteObject(hBmMask2); // IS NOW ON STACK

  SelectObject(hDCSource, hBmOldSource);
  // Don't Delete hBmSource!

  SelectPalette(hDCSource, hOldPal, false);
  // Don't delete hPal!

  DeleteDC(hDCMask1);
  DeleteDC(hDCMask2);
  DeleteDC(hDCSource);
     
}
bool ReadLineFromString(CString & source, int & start, CString & c){
	int sizebuffer = source.GetLength();
	c=CString (' ', 100);
	c="";
	UINT count=0;
	char b;
	while(true){
		if (start >= sizebuffer){
			return (count>0);
		}

//	    read=f.Read (&b, 1);
	    b = source.GetAt(start);
		start++;

		if (b==10){
			// EOL
			return (true);
		}
		if (b==13){
			if (start == sizebuffer){
				return (true);
			}
			b=source.GetAt(start);
			// check if next char is 10

			if (b==10 ){
					// EOL
				start++;
				return true;
			}
			// so next character is good, have to step back one:
			return true;
		}
		count ++;
		c += b;
	}
}


bool ReadLineFromBuffer(char * buffer, UINT & start, UINT sizebuffer, CString & c){
	c=CString (' ', 100);
	c="";
	UINT count=0;
	char b;
	while(true){
		if (start == sizebuffer){
			return (count>0);
		}

//	    read=f.Read (&b, 1);
	    b = buffer[start];
		start++;

		if (b==10){
			// EOL
			return (true);
		}
		if (b==13){
			if (start == sizebuffer){
				return (true);
			}
			b=buffer[start];
			// check if next char is 10

			if (b==10 ){
					// EOL
				start++;
				return (true);
			}
			// so next character is good, have to step back one:
			return (true);
		}
		count ++;
		c += b;
	}
}


void DoubleToStringWithUnit(double d, CString unit, CString & result){
	result=CString("");
	char s[20];
	CString u=CString(unit);

	double dd;

	double sign = 1.0;

	if (d<0.0) sign = -1.0;

	d=fabs(d);

	CString g;

	
	if (d >= 1.0e12){
		dd = 1.0e12;
		g='T';
//		Alert0("Tera");
//		ShowFloat(d, "d");
	}
	else
	if (d>=1e9){
		dd = 1e9;
		g='G';
	}
	else
	if (d==0.0){
		dd = 1;
		g=' ';
	}
	else
	if (d>=1e6){
		dd = 1e6;
		g='M';
	}
	else
	if (d>=1e3){
		dd = 1e3;
		g='k';
	}
	else
	if (d>=1){
		dd = 1;
		g=CString("");
	}
	else
	if (d>=1e-3){
		dd = 1e-3;
		g='m';
	}
	else
	if (d>=1e-6){
		dd = 1e-6;
		g='µ';
	}
	else
	if (d>=1e-9){
		dd = 1e-9;
		g='n';
	}
	else
	if (d>=1e-12){
		dd = 1e-12;
		g='p';
	}
	else
	if (d>=1e-15){
		dd = 1e-15;
		g='f';
	}
	else
	if (d>=1e-18){
		dd = 1e-18;
		g='a';
	}
	else{
		dd = 1e-18;
		g='a';
	}
	d /= dd;
	d *= sign;
	_gcvt(d, 4, s);
//	_gcvt(d, 8, s);
	result = CString(s)+CString(" ")+g+u;
//	Alert0(result);


}

BOOL IsCifra(CString & c){
	if (c.Compare(_T("0"))==0) return true;
	if (c.Compare(_T("1"))==0) return true;
	if (c.Compare(_T("2"))==0) return true;
	if (c.Compare(_T("3"))==0) return true;
	if (c.Compare(_T("4"))==0) return true;
	if (c.Compare(_T("5"))==0) return true;
	if (c.Compare(_T("6"))==0) return true;
	if (c.Compare(_T("7"))==0) return true;
	if (c.Compare(_T("8"))==0) return true;
	if (c.Compare(_T("9"))==0) return true;
	if (c.Compare(_T("0"))==0) return true;
	return false;
}

BOOL IsNumeric(CString & c){
	if (IsCifra(c)) return true;
	if (c.Compare(_T("."))==0) return true;
	if (c.Compare(_T("+"))==0) return true;
	if (c.Compare(_T("-"))==0) return true;
	if (c.Compare(_T("e"))==0) return true;
	if (c.Compare(_T("E"))==0) return true;
	return false;
}

BOOL GetMult(CString c, double & mult){
	if (c.Compare(_T("T"))==0){
		mult = 1e12;
		return true;
	}
	if (c.Compare(_T("G"))==0){
		mult = 1e9;
		return true;
	}
	if (c.Compare(_T("M"))==0){
		mult = 1e6;
		return true;
	}
	if (c.Compare(_T("k"))==0){
		mult = 1e3;
		return true;
	}
	if (c.Compare(_T("m"))==0){
		mult = 1e-3;
		return true;
	}
	if (c.Compare(_T("µ"))==0){
		mult = 1e-6;
		return true;
	}
	if (c.Compare(_T("u"))==0){
		mult = 1e-6;
		return true;
	}
	if (c.Compare(_T("n"))==0){
		mult = 1e-9;
		return true;
	}
	if (c.Compare(_T("p"))==0){
		mult = 1e-12;
		return true;
	}
	if (c.Compare(_T("f"))==0){
		mult = 1e-15;
		return true;
	}
	if (c.Compare(_T("a"))==0){
		mult = 1e-18;
		return true;
	}
	return false;
}

bool StringWithUnitToDouble(CString unit, CString c, double & d){

	c.Remove(' ');			// remove all spaces
	int l=c.GetLength();
	if (l<1) return false;
	int lastnum=0;
	CString num=CString("");
	while (true){
		if (lastnum>=l){
			d=atof(CStringA(c));
			return true;
		}
		CString n=c.Mid(lastnum, 1);
		if (IsNumeric(n)){
			num += n;
			lastnum++;
		}
		else{
			break;
		}
	}

//	Alert0(num);

	double nd;
	if (lastnum==0){  // e.g. just " kHz" -> "1 kHz"
		nd=1.0;
	}
	else{
		nd=atof(CStringA(num));
	}
//	ShowFloat(nd, "nd");

	CString right = c.Right(l-lastnum);
//	Alert0(right);

	if (right.GetLength()==0){
		d=nd;
		return true;
	}

	double mult = 1.0;
	int upos=right.Find(unit, 0);
	if (upos==(-1)){ // not found
		// no unit, assume just something like p, m, M ...., or nothing
		if (right.GetLength()>1) return false;
		if (!GetMult(right, mult)){
			return false;
		}
		d = mult*nd;
		return true;
	}

	if (right.GetLength()==unit.GetLength()){
		d=nd;
		return true;
	}
	if (right.GetLength()!=(1+unit.GetLength())){
		return false;
	}
	if (!GetMult(right.Left(1), mult)) return false;
	d=nd*mult;
	return true;
}


bool WriteSystemTimeOnFile(SYSTEMTIME * pt, CFile * fp){
	WriByNS(pt->wDay);
	WriByNS(pt->wDayOfWeek);
	WriByNS(pt->wHour);
	WriByNS(pt->wMilliseconds);
	WriByNS(pt->wMinute);
	WriByNS(pt->wMinute);
	WriByNS(pt->wMonth);
	WriByNS(pt->wSecond);
	WriByNS(pt->wYear);
	return true;
}

bool ReadSystemTimeFromFile(SYSTEMTIME * pt, CFile * fp){
	ReByNS(pt->wDay);
	ReByNS(pt->wDayOfWeek);
	ReByNS(pt->wHour);
	ReByNS(pt->wMilliseconds);
	ReByNS(pt->wMinute);
	ReByNS(pt->wMinute);
	ReByNS(pt->wMonth);
	ReByNS(pt->wSecond);
	ReByNS(pt->wYear);
	return true;
}

__int32 SwapInt32(__int32 i)
{
 typedef struct structintstruct {
  char c1, c2, c3, c4;
 } intstruct;
// typedef struct structintstruct intstruct;
 intstruct *iptr;

 char C1, C2;
/*  printf("\n SwapLONGCARD: before %u", i); */
 iptr=(intstruct *)(&i);
 C1=iptr->c1;
 C2=iptr->c2;
 iptr->c1=iptr->c4;
 iptr->c2=iptr->c3;
 iptr->c3=C2;
 iptr->c4=C1;
/*  printf(" after %u\n", i); */
 return i;
}

double SwapDouble(double x){
	typedef struct structdoublestruct {
	char c1, c2, c3, c4, c5, c6, c7, c8;
	} doublestruct;
// typedef struct structdoublestruct doublestruct;
	doublestruct *xptr;

	char C1, C2, C3, C4;
/*  printf("\n SwapLONGCARD: before %u", i); */
	xptr=(doublestruct *)(&x);
	C1=xptr->c1;
	C2=xptr->c2;
	C3=xptr->c3;
	C4=xptr->c4;
	xptr->c1=xptr->c8;
	xptr->c2=xptr->c7;
	xptr->c3=xptr->c6;
	xptr->c4=xptr->c5;
	xptr->c5=C4;
	xptr->c6=C3;
	xptr->c7=C2;
	xptr->c8=C1;
/*  printf(" after %u\n", i); */
	 return x;
}

float SwapFloat(float var){
	__int32 * iptr = (__int32 *)  &var;
	__int32 swapped = SwapInt32(*iptr);
	float * floatptr = (float *) &swapped;
	return *floatptr;
}


double SystemTimeDifferenceInSeconds(SYSTEMTIME & st1, SYSTEMTIME & st2){
	// works only for time-differences < 23.99 hours

	bool t2GTt1 = true;
	if (st1.wYear>st2.wYear){
		t2GTt1=false;
	}
	if (st1.wMonth>st2.wMonth){
		t2GTt1=false;
	}
	if (st1.wDay>st2.wDay){
		t2GTt1=false;
	}

	double mult = -1.0;
	if (t2GTt1){
		mult = 1.0;
	}

	double t1=st1.wSecond + st1.wMinute*60.0 + st1.wMilliseconds/1000.0+
				st1.wHour*3600.0;

	double t2=st2.wSecond + st2.wMinute*60.0 + st2.wMilliseconds/1000.0+
					st2.wHour*3600.0;

	return (t2-t1)*mult;

}

void WriteLineOnBinaryFile(CString & value, CFile & file){
	int l=value.GetLength();
	TCHAR t;
	for (int i=0; i<l; i++){
		t=value.GetAt( i ) ;
		file.Write(&t, sizeof(TCHAR));
	}
	WriteEndOfLine(file);
}
void WriteLineOnBinaryFile(std::string & value, std::ofstream & file){
	if (value.length()>0){
//		file.write(value, value.length());
//		file.write(endl, 1);
//		file.put('\n');
//		file << endl;
		file << value << endl;
	}
}

void PutCStringInCharArray(CString & c, char * s, int lengtharray){
	int i;

	int l=c.GetLength();
	if (l<lengtharray){
		s[lengtharray-1]=0;
	}
	else{
		l = lengtharray;
	}
	for (i=0; i<l; i++){
		s[i] = c.GetAt(i);
	}
}

void Swapfloat (float *rr)
{
 char C1, C2;
 char r[4];
 char * rp1;
 float * fp;
 int i;

 rp1 = (char *)(rr);
 for (i=0; i<4; i++) r[i]=(rp1)[i];

 C1=r[0];  C2=r[1];
 r[0]=r[3];  r[1]=r[2];
 r[2]=C2;  r[3]=C1;

 fp =(float *)(&r);
 *rr=*fp;
 }

__int16 SwapInt(__int16 i)
{
 typedef struct structintstruct {
  char c1, c2;
 }intstruct;
// typedef struct structintstruct intstruct;
 intstruct *iptr;
 char c;
 iptr= (intstruct *) (&i);
 c=iptr->c1;
 iptr->c1=(*iptr).c2;
 iptr->c2=c;
 return i;
}
__int16 SwapInt16(__int16 i)
{
 typedef struct structintstruct {
  char c1, c2;
 }intstruct;
// typedef struct structintstruct intstruct;
 intstruct *iptr;
 char c;
 iptr= (intstruct *) (&i);
 c=iptr->c1;
 iptr->c1=(*iptr).c2;
 iptr->c2=c;
 return i;
}


__int32 SwapLong(__int32 i)
{
 typedef struct structintstruct {
  char c1, c2, c3, c4;
 }intstruct;
// typedef struct structintstruct intstruct;
 intstruct *iptr;

 char C1, C2;
/*  printf("\n Swaplong: before %d", i); */
 iptr=(intstruct*)(&i);
 C1=iptr->c1;
 C2=iptr->c2;
 iptr->c1=iptr->c4;
 iptr->c2=iptr->c3;
 iptr->c3=C2;
 iptr->c4=C1;
/*  printf(" after %d\n", i); */
 return i;
}

void SwapInts(__int16 * ip, int n){
	for (int k=0; k<n; k++){
		(*ip) = SwapInt((*ip));
		ip++;
	}
}

void Swap32Ints(__int32 * ip, int n){
	for (int k=0; k<n; k++){
		(*ip) = SwapLong((*ip));
		ip++;
	}
}

double Norm(double * vect) {
	double result =0 ;
	for (int i=0; i<3; i++){
		double temp=vect[i];
		result += temp*temp;
	}
	return sqrt(result);
}

void Normalize(double * vect){
	double norm=Norm(vect);
	if (norm<=0) return;
	for (int i=0; i<3; i++){
		vect[i] /= norm;
	}
}

void CopyVect(double * from, double * to){
	for (int i=0; i<3; i++){
		to[i] = from[i];
	}
}


void DoRot(double ** RotaMat, double * vect){
	double r[3];
	int i;
	for (i=0; i<3; i++){
		r[i]=0;
		for (int j=0; j<3; j++){
			r[i] += RotaMat[i][j]*vect[j];
		}
	}
	for (i=0; i<3; i++){
		vect[i]=r[i];
	}
}


void AddVect(double *a, double *b, double *c){ // c=a+b
	for (int i=0; i<3; i++){
		c[i]=a[i]+b[i];
	}
}
void ScalarMult(double a, double *b, double *c){ // c=a*b
	for (int i=0; i<3; i++){
		c[i]=a*b[i];
	}
}

void SubVect(double *a, double *b, double *c){ // c=a-b
	for (int i=0; i<3; i++){
		c[i]=a[i]-b[i];
	}
}

double DotProduct(double *a, double * b){
	double r=0;
	for (int i=0; i<3; i++){
		r += a[i]*b[i];
	}
	return r;
}

double findAngle(double * u, double *v){
/*
    Calculates the angle (degrees) between two vectors (as 1-d arrays) using
    dot product.
*/

    double mag_u = Norm(u); 
    double mag_v = Norm(v);

	double result = acos(DotProduct(u,v)/(mag_u*mag_v));
	return result;
//    return 180/pi * acos(dotProduct(u,v)/(mag_u*mag_v))
}

void CrossProduct(double *a, double * b, double * c){
	//(a2b3 - a3b2)i + (a3b1 - a1b3)j + (a1b2 - a2b1)k 
	c[0]=a[1]*b[2]-a[2]*b[1];
	c[1]=a[2]*b[0]-a[0]*b[2];
	c[2]=a[0]*b[1]-a[1]*b[0];
}

double CrossDotProduct(double * a, double * b, double * c){ // (axb).c
	double cc[3];
	CrossProduct(a, b, cc);
	return DotProduct(cc, c);
}

void MakeRotaMat(double angle, double * PolarVect, double ** RotaMat){
	double s = sin(angle);
	double c = cos(angle);
	double v=1-c;

	double kx=PolarVect[0];
	double ky=PolarVect[1];
	double kz=PolarVect[2];

	RotaMat[0][0]=kx*kx*v+c;
	RotaMat[0][1]=kx*ky*v-kz*s;
	RotaMat[0][2]=kx*kz*v+ky*s;

	RotaMat[1][0]=ky*kx*v+kz*s;
	RotaMat[1][1]=ky*ky*v+c;
	RotaMat[1][2]=ky*kz*v-kx*s;

	RotaMat[2][0]=kz*kx*v-ky*s;
	RotaMat[2][1]=kz*ky*v+kx*s;
	RotaMat[2][2]=kz*kz*v+c;

/*

	RotaMat[0][0]=kx*kx*v+c;
	RotaMat[1][0]=kx*ky*v-kz*s;
	RotaMat[2][0]=kx*kz*v+ky*s;

	RotaMat[0][1]=ky*kx*v+kz*s;
	RotaMat[1][1]=ky*ky*v+c;
	RotaMat[2][1]=ky*kz*v-kx*s;

	RotaMat[0][2]=kz*kx*v-ky*s;
	RotaMat[1][2]=kz*ky*v+kx*s;
	RotaMat[2][2]=kz*kz*v+c;
*/
}



bool CheckSigns(double * InitialParams, double * Params, int NPARAMS){
	for (int i=0; i<NPARAMS; i++){
		if (InitialParams[i]*Params[i]<0.0){
			return false;
		}
	}
	return true;
}


bool PermutationsAreEquivalent(int Length, int * p1, int * p2){
// e.g. (1,2,3) is equivalent to (2,3,1) and to (3,1,2)
	int i;
	for (i=0; i<Length; i++){
		bool Equal = true;
		for (int j=0; j<Length; j++){
			if (p1[j] != p2[ (j+i) % Length]){
				Equal = false;
			}
		}
		if (Equal) return true;
	}
	return false;
}

bool PermutationsAreReverseEquivalent(int Length, int * p1, int * p2){
// e.g. (1,2,3) is reverse equivalent to (3,2,1) and to (2,1,3)
	int i;
	for (i=0; i<Length; i++){
		bool Equal = true;
		for (int j=0; j<Length; j++){
			if (p1[j] != p2[ Length - ((j+i) % Length) - 1]){
				Equal = false;
			}
		}
		if (Equal) return true;
	}
	return false;
}

int	* GetSequenceofNumbers(CString & c, int & length){
	int ls = c.GetLength();

	int nnumbers = 0;
	bool IsInNumber = false;
	int i=0;
	while (i<c.GetLength()){
		CString  c1 = CString(c.GetAt(i));
		bool IsNumber = IsCifra(c1);
		if (IsNumber){
			if (!IsInNumber){
				nnumbers ++;
				IsInNumber = true;
			}
		}
		else{
			IsInNumber = false;
		}
		i++;
	}

	if (nnumbers<1) return false;
	length = nnumbers;

	int * p = new int[length];

	nnumbers = 0;
	IsInNumber = false;
	i=0;
	CString c2;
	while (i<c.GetLength()){
		CString  c1 = CString(c.GetAt(i));
		bool IsNumber = IsCifra(c1);
//		Alert0(CString("c1: ")+c1);
//		Alert0(CString("c2: ")+c2);
		if (IsNumber){
//			Alert0("is number");
			c2 += c1;
			IsInNumber = true;
		}
		else{
//			Alert0("is not number");
			if (IsInNumber){
//				Alert0("is in number");
//				Alert0(c2);
				p[nnumbers] = atoi(CStringA(c2));
				nnumbers ++;
				c2 = CString("");
			}
			else{
//				Alert0("not is in number");
			}

			IsInNumber = false;
		}
		i++;
	}
	if (IsInNumber){
		p[nnumbers] = atoi(CStringA(c2));
	}

	return p;
}


char *                          /* O - Bitmap data */
LoadDIBitmap(CFile & file, /* I - File to load */
             BITMAPINFO & info){    /* O - Bitmap information */

	CFile * fp = &(file);

    char             *bits;        /* Bitmap pixel bits */
//    char             *ptr;         /* Pointer into bitmap */
//    char             temp;         /* Temporary variable to swap red and blue */
//    int              x, y;         /* X and Y position in image */
//    int              length;       /* Line length */
    int              bitsize;      /* Size of bitmap */
    int              infosize;     /* Size of header information */
    BITMAPFILEHEADER header;       /* File header */


    /* Read the file header and any following bitmap information... */
	ReByNS(header.bfType);
	ReByNS(header.bfSize);
	ReByNS(header.bfReserved1);
	ReByNS(header.bfReserved2);
	ReByNS(header.bfOffBits);
//	ShowFloat(header.bfOffBits, "offbits");

//    header.bfType      = read_word(fp);
//    header.bfType      = read_dword(fp);
//    header.bfReserved1 = read_word(fp);
//    header.bfReserved2 = read_word(fp);
//    header.bfOffBits   = read_dword(fp);

    if (header.bfType != BF_TYPE) /* Check for BM reversed... */
        {
        /* Not a bitmap file - return NULL... */
 //       fclose(fp);
        return (NULL);
        }

    infosize = header.bfOffBits - 18;

	ReByNS(info.bmiHeader.biSize );
	ReByNS(info.bmiHeader.biWidth);
	ReByNS(info.bmiHeader.biHeight);
	ReByNS(info.bmiHeader.biPlanes);
	ReByNS(info.bmiHeader.biBitCount);
	ReByNS(info.bmiHeader.biCompression);
	ReByNS(info.bmiHeader.biSizeImage);
	ReByNS(info.bmiHeader.biXPelsPerMeter);
	ReByNS(info.bmiHeader.biClrUsed);
	ReByNS(info.bmiHeader.biClrImportant);

/*
    (*info)->bmiHeader.biSize          = read_dword(fp);
    (*info)->bmiHeader.biWidth         = read_long(fp);
    (*info)->bmiHeader.biHeight        = read_long(fp);
    (*info)->bmiHeader.biPlanes        = read_word(fp);
    (*info)->bmiHeader.biBitCount      = read_word(fp);
    (*info)->bmiHeader.biCompression   = read_dword(fp);
    (*info)->bmiHeader.biSizeImage     = read_dword(fp);
    (*info)->bmiHeader.biXPelsPerMeter = read_long(fp);
    (*info)->bmiHeader.biYPelsPerMeter = read_long(fp);
    (*info)->bmiHeader.biClrUsed       = read_dword(fp);
    (*info)->bmiHeader.biClrImportant  = read_dword(fp);
*/
    if (infosize > 40){
		int result = fp->Read(info.bmiColors, infosize-40);
		if (result<(infosize-40)){
			return NULL;
		}
	}
/*
	if (fread((*info)->bmiColors, infosize - 40, 1, fp) < 1)
            {
            free(*info);
            fclose(fp);
            return (NULL);
            }
*/

//	ShowFloat(info.bmiHeader.biBitCount, "bitcount");
    /* Now that we have all the header info read in, allocate memory for *
     * the bitmap and read *it* in...                                    */
    if ((bitsize = info.bmiHeader.biSizeImage) == 0){
//		Alert0("size == 0");
		int rowsize = (info.bmiHeader.biWidth * 3 + 3) & ~3;
//		int rowsize = (info.bmiHeader.biWidth * info.bmiHeader.biBitCount + 7) / 8;

		bitsize = info.bmiHeader.biHeight*rowsize;

/*
        bitsize = (info.bmiHeader.biWidth *
                   info.bmiHeader.biBitCount + 7) / 8 *
  	           abs(info.bmiHeader.biHeight);
			*/
	}

	bits = (char * )(malloc(bitsize));
    if ( bits == NULL){
        /* Couldn't allocate memory - return NULL! */
        return (NULL);
    }

	fp->Seek(header.bfOffBits, 0);

	int result = fp->Read(bits, bitsize);
	if (result<bitsize){
//    if (fread(bits, 1, bitsize, fp) < bitsize)
//        {
        Alert0(_T(" Couldn't read bitmap - free memory and return NULL! "));
		delete bits;
		bits = NULL;
 
//		free(*info);
//        free(bits);
//        fclose(fp);
        return (NULL);
	}

    /* Swap red and blue */
/*
   length = (info.bmiHeader.biWidth * 3 + 3) & ~3;

 for (y = 0; y < info.bmiHeader.biHeight; y ++)
        for (ptr = bits + y * length, x = info.bmiHeader.biWidth;
             x > 0;
	     x --, ptr += 3)
	    {
	    temp   = ptr[0];
	    ptr[0] = ptr[2];
	    ptr[2] = temp;
	    }
*/
    /* OK, everything went fine - return the allocated bitmap... */
//    fclose(fp);

    return (bits);
  }


/*
 * 'SaveDIBitmap()' - Save a DIB/BMP file to disk.
 *
 * Returns 0 on success or -1 on failure...
 */

bool SaveDIBitmap(CFile & file, /* I - File to load */
					BITMAPINFO & info,     /* I - Bitmap information */
					char     *bits) {    /* I - Bitmap data */

	CFile * fp = &(file);
    int  size,                     /* Size of file */
         infosize,                 /* Size of bitmap info */
         bitsize;                  /* Size of bitmap pixels */

//   BITMAPFILEHEADER  header = info.bmiHeader;       /* File header */



    /* Figure out the bitmap size */
    if (info.bmiHeader.biSizeImage == 0){
//		Alert0("size = 0");
		int rowsize = (info.bmiHeader.biWidth * 3 + 3) & ~3;
//		int rowsize = (info.bmiHeader.biWidth * info.bmiHeader.biBitCount + 7) / 8;

		bitsize = info.bmiHeader.biHeight*rowsize;

//	bitsize = (info.bmiHeader.biWidth *
  //      	   info.bmiHeader.biBitCount + 7) / 8 *
//		  abs(info.bmiHeader.biHeight);
	}
    else{
		bitsize = info.bmiHeader.biSizeImage;
	}

    /* Figure out the header size */
    infosize = sizeof(BITMAPINFOHEADER);
//	ShowFloat(infosize, "infosize");
//	ShowFloat(info.bmiHeader.biCompression, "info.bmiHeader.biCompression");
    switch (info.bmiHeader.biCompression)
	{
	case 0 :break; //Alert0("0");break;
	case BI_BITFIELDS :
            infosize += 12; /* Add 3 RGB doubleword masks */
            if (info.bmiHeader.biClrUsed == 0)
	      break;
/*
	case BI_RGB :
            if (info.bmiHeader.biBitCount > 8 &&
        	info.bmiHeader.biClrUsed == 0)
	      break;
		*/
	case BI_RLE8 :
	case BI_RLE4 :
            if (info.bmiHeader.biClrUsed == 0)
              infosize += (1 << info.bmiHeader.biBitCount) * 4;
	    else
              infosize += info.bmiHeader.biClrUsed * 4;
	    break;
	}

    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;
//	ShowFloat(infosize, "infosize");

    /* Write the file header, bitmap information, and bitmap pixel data... */

	__int16 iii= BF_TYPE;
	WriByNS(iii);
	WriByNS(size);
	iii = 0;
	WriByNS(iii);
	WriByNS(iii);

	int bfOffBits = 18 + infosize-4;
	WriByNS(bfOffBits);
//	ShowFloat(bfOffBits, "offbits");

	WriByNS(info.bmiHeader.biSize);
	WriByNS(info.bmiHeader.biWidth);
	WriByNS(info.bmiHeader.biHeight);
	WriByNS(info.bmiHeader.biPlanes);
	WriByNS(info.bmiHeader.biBitCount);
	WriByNS(info.bmiHeader.biCompression);
	WriByNS(info.bmiHeader.biSizeImage);
	WriByNS(info.bmiHeader.biXPelsPerMeter);
	WriByNS(info.bmiHeader.biYPelsPerMeter);
	WriByNS(info.bmiHeader.biClrUsed);
	WriByNS(info.bmiHeader.biClrImportant);

//    write_word(fp, BF_TYPE);        /* bfType */
//    write_dword(fp, size);          /* bfSize */
//    write_word(fp, 0);              /* bfReserved1 */
//    write_word(fp, 0);              /* bfReserved2 */
 //   write_dword(fp, 18 + infosize); /* bfOffBits */

//    write_dword(fp, info->bmiHeader.biSize);
//    write_long(fp, info->bmiHeader.biWidth);
//    write_long(fp, info->bmiHeader.biHeight);
//    write_word(fp, info->bmiHeader.biPlanes);
//    write_word(fp, info->bmiHeader.biBitCount);
//    write_dword(fp, info->bmiHeader.biCompression);
//    write_dword(fp, info->bmiHeader.biSizeImage);
//    write_long(fp, info->bmiHeader.biXPelsPerMeter);
//    write_long(fp, info->bmiHeader.biYPelsPerMeter);
//    write_dword(fp, info->bmiHeader.biClrUsed);
//    write_dword(fp, info->bmiHeader.biClrImportant);
//	ShowFloat(bitsize, "btsize");
    if (infosize > 40){
		Alert0(_T("infosize>40"));
		fp->Write(info.bmiColors, infosize - 40);
/*
		if (fwrite(info->bmiColors, infosize - 40, 1, fp) < 1)
		        {
				fclose(fp);
	            return (-1);
		        }
	}
*/
	}
	fp->Seek(bfOffBits, 0);
	fp->Write(bits, bitsize);
	return true;
/*
    if (fwrite(bits, 1, bitsize, fp) < bitsize)
        {
        fclose(fp);
        return (-1);
        }

    fclose(fp);
    return (0);
    }
*/
}


bool SimpleGaussSeidel(int n, double ** A, double * b, double * x, int MaxIter, double eps){
/* Solves the equation Ax = b using the iterative Gauss-Seidel method

  ASSUMING THAT ALL DIAGONAL ELEMENTS OF A ARE NONZERO 

  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS OF A IS ZERO

  n is the dimension of the matrix A assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm

  */

	if (n<1) return false;

	int i, j;

	for (i=0; i<n; i++){
		if (A[i][i] == 0.0){
			return false;
		}
	}
	
	for (i=0; i<n; i++){
//		x[i]=1.0/double(n);
		x[i] = 0.0;
	}


	int checknorm = MaxIter/20;
	
	int iter =0;
	int normiter = 0;

	while (true){
		if (iter>MaxIter){
//			Alert0("iter>MAxIter");
			return true;
		}
		normiter++;
		if (normiter > checknorm){
			normiter = 0;
			double norm = 0.0;

			for (i=0; i<n; i++){
				double sum = 0.0;
				for (j=0; j<n; j++){
					sum += A[i][j]*x[j];
				}
				sum -= b[i];
				if (fabs(sum)>norm){
					norm = fabs(sum);
				}
			}

			if (norm<eps){
	//			ShowFloat(norm, "norm");
	//			Alert0("norm <eps");
				return true;
			}
		}

		for (i=0; i<n; i++){
			double sum = 0.0;
			for (int j=0; j<n; j++){
				if (i!=j){
					sum += A[i][j]*x[j];
				}
			}
			x[i] = (b[i] - sum) / A[i][i];
		}
		iter++;
	}
	return true;
}


bool SimpleSOR(int n, double ** A, double * b,
			   double * x, int MaxIter, double eps, double omega){
/* Solves the equation Ax = b using the iterative SOR method

  ASSUMING THAT ALL DIAGONAL ELEMENTS OF A ARE NONZERO 

  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS OF A IS ZERO

  n is the dimension of the matrix A assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm

  */
	if (n<1) return false;

	int i, j;

	for (i=0; i<n; i++){
		if (A[i][i] == 0.0){
			return false;
		}
	}
	
	for (i=0; i<n; i++){
//		x[i]=1.0/double(n);
		x[i] = 0.0;
	}


	int checknorm = MaxIter/20;
	
	int iter =0;
	int normiter = 0;

	while (true){
		if (iter>MaxIter){
//			Alert0("iter>MAxIter");
			return true;
		}
		normiter++;
		if (normiter > checknorm){
			normiter = 0;
			double norm = 0.0;

			for (i=0; i<n; i++){
				double sum = 0.0;
				for (j=0; j<n; j++){
					sum += A[i][j]*x[j];
				}
				sum -= b[i];
				if (fabs(sum)>norm){
					norm = fabs(sum);
				}
			}

			if (norm<eps){
	//			ShowFloat(norm, "norm");
	//			Alert0("norm <eps");
				return true;
			}
		}

		for (i=0; i<n; i++){
			double sum0 = (1.0-omega)*x[i];
			double sum =0.0;

			for (int j=0; j<n; j++){
				if (i!=j){
					sum += A[i][j]*x[j];
				}
			}
			x[i] = sum0 + omega*(b[i] - sum) / A[i][i];
		}
		iter++;
	}
	return true;
}

bool ProbabilityGaussSeidel(int n, double ** A, double * b,
							double * x, int MaxIter, double eps){
/* Solves the equation Ax = b using the iterative Gauss-Seidel method
   if b==NULL assumes b[i]=0 for all i

  ASSUMES that all xi are >=0 and that sum xi = 1.
  Normalizes the xi on each iteration such.

  ASSUMING THAT ALL DIAGONAL ELEMENTS OF A ARE NONZERO 

  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS OF A IS ZERO

  n is the dimension of the matrix A assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm

  */
	if (n<1) return false;

	int i, j;

	for (i=0; i<n; i++){
		if (A[i][i] == 0.0){
			return false;
		}
	}
	
	for (i=0; i<n; i++){
		x[i]=1.0/double(n);
	}


	int checknorm = MaxIter/20;
	
	int iter =0;
	int normiter = 0;

	while (true){
		if (iter>MaxIter){
//			Alert0("iter>MAxIter");
			return true;
		}

		normiter++;
		if (normiter > checknorm){
			normiter = 0;
			if (eps>0.0){
				double norm = 0.0;
	
				for (i=0; i<n; i++){
					double sum = 0.0;
					for (j=0; j<n; j++){
						sum += A[i][j]*x[j];
					}
					if (b) sum -= b[i];		
					if (fabs(sum)>norm){
						norm = fabs(sum);
					}
				}

				if (norm<eps){
		//			ShowFloat(norm, "norm");
		//			Alert0("norm <eps");
					return true;
				}
			}
		}

		for (i=0; i<n; i++){
			double sum = 0.0;
			for (int j=0; j<n; j++){
				if (i!=j){
					sum += A[i][j]*x[j];
				}
			}
			if (b)
				x[i] = (b[i] - sum) / A[i][i];
			else
				x[i] = (0.0 - sum) / A[i][i];

		}
		
		for (i=0; i<n; i++){
			if (x[i]<0) x[i]=0;
		}

		double sump=0;

		for (i=0; i<n; i++){
			sump += x[i];
		}
		for (i=0; i<n; i++){
			x[i] /= sump;
		}

		iter++;
	}
	return true;
}


bool sprsin(double ** A, int n, double thresh, int nmax, double * sa, int * ija){

/* Converts a square matrix A[0,...,n-1][0, ..., n-1] into row-indexes 
   sparse storage mode. Only elements with magnitude > thresh are retained. Thus 0 is ok as thresh!
   Output is in two linear arrays with dimension nmax. Procedure returns FALSE if
   not enough space was allocated. 
   sa[0, ...] contains array values, indexed by ija[0,...].
*/

	int i, j, k;

	if (n<1) return false;
	if (nmax<n) return false;

	for (i=0; i<n; i++){
		sa[i] = A[i][i]; // diagonal elements are stored also if 0
	}

	ija[0] = n+1; // From this you can get the dimension of the matrix

	k=n;

	for (i=0; i<n; i++){
		for (j=0; j<n; j++){
			if ( (i != j) && (fabs(A[i][j])>thresh) ) {
				k++;
				if (k>=nmax){
					return false; // Not enough space allocated
				}
				sa[k] = A[i][j];
				ija[k] = j;
			}
		}
		ija[i+1] = k+1;
	} 
	return true; // End of sprsin
}

bool sprsTRANSPOSEin(double ** A, int n, double thresh, int nmax, double * sa, int * ija){

/* Converts a square matrix A[0,...,n-1][0, ..., n-1] into row-indexes 
   sparse storage mode and first transposes A. Only elements with magnitude > thresh are retained. Thus 0 is ok as thresh!
   Output is in two linear arrays with dimension nmax. Procedure returns FALSE if
   not enough space was allocated. 
   sa[0, ...] contains array values, indexed by ija[0,...].
*/

	int i, j, k;

	if (n<1) return false;
	if (nmax<n) return false;

	for (i=0; i<n; i++){
		sa[i] = A[i][i]; // diagonal elements are stored also if 0
	}

	ija[0] = n+1; // From this you can get the dimension of the matrix

	k=n;

	for (i=0; i<n; i++){
		for (j=0; j<n; j++){
			if ( (i != j) && (fabs(A[j][i])>thresh) ) {
				k++;
				if (k>=nmax){
					return false; // Not enough space allocated
				}
				sa[k] = A[j][i];
				ija[k] = j;
			}
		}
		ija[i+1] = k+1;
	} 
	return true; // End of sprsin
}


bool sprsax(double * sa, int * ija, double * x, double * b, int n){
/* Multiply a matrix in row-index sparse storage, stored in sa and ija, by a vector
   x[0,...] giving b[0,...]
   n is the dimension of the arrays/matrix
*/
	int i, k;
	if (ija[0] != (n+1)){
		return false;
	}
	for (i=0; i<n; i++){
		b[i] = sa[i]*x[i]; // the diagonal term
		for (k=ija[i]; k<ija[i+1]; k++){
			b[i] += sa[k]*x[ija[k]];
		}
	}
	return true;
}

double MaxNorm(int n, double * x){
	return MaxNorm(n, x, NULL);
}
double MaxNorm(int n, double * x, double * b){
	double norm = 0.0;
	for (int i=0; i<n; i++){
		double sum = x[i];
		if (b) sum -= b[i];		
		if (fabs(sum)>norm){
			norm = fabs(sum);
		}
	}
	return norm;
}

bool ProbabilitySparseGaussSeidel(int n, double *sa, int * ija, double * b,
								  double * x, double * rsforcheck,
								  int MaxIter, double eps){
/* Solves the equation Ax = b using the iterative Gauss-Seidel method
	where A is in row-indexed   sparse storage mode in vectors sa and ija
   if b==NULL assumes b[i]=0 for all i
  ASSUMES that all xi are >=0 and that sum xi = 1.
  Normalizes the xi on each iteration such.
  ASSUMING THAT ALL DIAGONAL ELEMENTS ARE NONZERO 
  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS IS ZERO
  n is the dimension of the non-sparse matrix assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm  */


////
	if (n<1) return false;
	int i;
	for (i=0; i<n; i++){
		if (sa[i] == 0.0){
			return false;
		}
	}
	for (i=0; i<n; i++){
		x[i]=1.0/double(n);
	}
	int checknorm = 1000;
	int iter =0;
	int normiter = 0;
	double oldnorm = 0.0;

	while (true){
		if (iter>MaxIter){
//			Alert0("iter>MAxIter");
			return true;
		}

		normiter++;
		if (normiter > checknorm){
			normiter = 0;
			if (eps>0.0){

				sprsax(sa, ija, x, rsforcheck,  n);

				double norm1 = MaxNorm(n, rsforcheck, b);
				double norm2 = MaxNorm(n, x);
				double norm = norm1/norm2;
//				ShowFloat(norm, "norm");
				if (norm<eps){
	//				ShowFloat(norm, "norm");
	//				Alert0("norm <eps");
					return true;
				}
				if (norm==oldnorm){
					return true;
				}
			}
		}

		// THIS IS TO CHECK
		for (i=0; i<n; i++){
			double sum = 0.0;
			for (int k=ija[i]; k<ija[i+1]; k++){
				sum += sa[k]*x[ija[k]];
			}
			if (b)
				x[i] = (b[i] - sum) / sa[i];
			else
				x[i] = (0.0 - sum) / sa[i];
		}
		
		for (i=0; i<n; i++){
			if (x[i]<0) x[i]=0;
		}

		double sump=0;

		for (i=0; i<n; i++){
			sump += x[i];
		}
		for (i=0; i<n; i++){
			x[i] /= sump;
		}

		iter++;
	}
	return true;
}

double GetSparseij(int i, int j, double * rsa, int * isa){
//Get the value of a sparse matrix
	
//	if (i<0) return 0.0;
//	if (j<0) return 0.0;
//	int ns = isa[0]-1;
//	if (i>=ns) return 0.0;
//	if (j>=ns) return 0.0;

	if (i==j) return rsa[i];

	for (int k=isa[i]; k<isa[i+1]; k++){
		if (j==isa[k]){
			return rsa[k];
		}
	}
	return 0.0;
}

/*

void IntToString(__int16 i, char * s){
	__int32 j = i;
	IntToString(j, s);
}
void IntToString(__int32 i, char * s){
	_itoa(i, s, 10);
}

void IntToString(unsigned __int16 i, char * s){
	unsigned __int32 j = i;
	IntToString(j, s);
}
void IntToString(unsigned __int32 i, char * s){
	_itoa(i, s, 10);
}
*/

void IntToString(int i, char * s){
	_itoa(i, s, 10);
}

void IntToString(__int64 i, char * s){
	_i64toa(i, s, 10);
}
void IntToString(unsigned __int64 i, char * s){
	_ui64toa(i, s, 10);
}


void tridiag(double a[], double b[], double c[], double r[], double u[], int n){

/* Solves for a vector u[0, ..., n-1] the tridiagonal linear set

b0  c0   0 ...                         |  u0    = r0
a1  b1  c1 0 ...                       |  u1    = r1
0   a2  b2  c2 0 ...                   |
                                       |
  ....................                 |
                  a(n-2) b(n-2) c(n-2) |  u(n-2) = r(n-2)
				   0     a(n-1) b(n-1) |  u(n-1) = r(n-1)


Vectors a , b, c, r  are not modified!

 */
//	ShowFloat(n,"n");

	if (n<2){
		Alert0(_T("ERROR: n<2 in trdiag"));
		return;
	}

	int j;
	double bet, * gam;

	gam = dvector(0, n-1);

	if (b[0]==0.0){
		Alert0(_T("Error: b[0] = 0 in tridiag!"));
		free_dvector(gam, 0, n-1);
		return;
	}
	u[0]=r[0]/(bet=b[0]);

	for (j=1; j<n; j++){
		gam[j]=c[j-1]/bet;
		bet=b[j]-a[j]*gam[j];
		if (bet == 0.0){
			Alert0(_T("Error: bet = 0 in tridiag!"));
			free_dvector(gam, 0, n-1);
			return;
		}
		u[j]=(r[j]-a[j]*u[j-1])/bet;
	}
	for (j=(n-2); j>=0; j--){
//			ShowFloat(j,"j");
		u[j] -= gam[j+1]*u[j+1];
	}

	free_dvector(gam, 0, n-1);
}


void tridiag2(double a[], double b[], double c[], double r[], double u[], int n, double gam[]){
	// in this version the working vector is povided by user

/* Solves for a vector u[0, ..., n-1] the tridiagonal linear set

b0  c0   0 ...                         |  u0    = r0
a1  b1  c1 0 ...                       |  u1    = r1
0   a2  b2  c2 0 ...                   |
                                       |
  ....................                 |
                  a(n-2) b(n-2) c(n-2) |  u(n-2) = r(n-2)
				   0     a(n-1) b(n-1) |  u(n-1) = r(n-1)


Vectors a , b, c, r  are not modified!

 */
//	ShowFloat(n,"n");

	if (n<2){
		Alert0(_T("ERROR: n<2 in trdiag"));
		return;
	}

	int j;
	double bet;

//	gam = dvector(0, n-1);

	if (b[0]==0.0){
		Alert0(_T("Error: b[0] = 0 in tridiag!"));
//		free_dvector(gam, 0, n-1);
		return;
	}
	u[0]=r[0]/(bet=b[0]);

	for (j=1; j<n; j++){
		gam[j]=c[j-1]/bet;
		bet=b[j]-a[j]*gam[j];
		if (bet == 0.0){
			Alert0(_T("Error: bet = 0 in tridiag!"));
//			free_dvector(gam, 0, n-1);
			return;
		}
		u[j]=(r[j]-a[j]*u[j-1])/bet;
	}
	for (j=(n-2); j>=0; j--){
//			ShowFloat(j,"j");
		u[j] -= gam[j+1]*u[j+1];
	}

//	free_dvector(gam, 0, n-1);
}


void tridiag2(vector<double> & a, vector<double> & b, vector<double> & c, vector<double> & r,
			  vector<double> & u, int n, vector<double> & gam){
	// in this version the working vector is povided by user

/* Solves for a vector u[0, ..., n-1] the tridiagonal linear set

b0  c0   0 ...                         |  u0    = r0
a1  b1  c1 0 ...                       |  u1    = r1
0   a2  b2  c2 0 ...                   |
                                       |
  ....................                 |
                  a(n-2) b(n-2) c(n-2) |  u(n-2) = r(n-2)
				   0     a(n-1) b(n-1) |  u(n-1) = r(n-1)


Vectors a , b, c, r  are not modified!

 */
//	ShowFloat(n,"n");

	if (n<2){
		Alert0(_T("ERROR: n<2 in trdiag"));
		return;
	}

	int j;
	double bet;

//	gam = dvector(0, n-1);

	if (b[0]==0.0){
		Alert0(_T("Error: b[0] = 0 in tridiag!"));
//		free_dvector(gam, 0, n-1);
		return;
	}
	u[0]=r[0]/(bet=b[0]);

	for (j=1; j<n; j++){
		gam[j]=c[j-1]/bet;
		bet=b[j]-a[j]*gam[j];
		if (bet == 0.0){
			Alert0(_T("Error: bet = 0 in tridiag!"));
//			free_dvector(gam, 0, n-1);
			return;
		}
		u[j]=(r[j]-a[j]*u[j-1])/bet;
	}
	for (j=(n-2); j>=0; j--){
//			ShowFloat(j,"j");
		u[j] -= gam[j+1]*u[j+1];
	}

//	free_dvector(gam, 0, n-1);
}

void DrawSymbol(int Symbol, int x, int y, int size, CDC * pDC){
	if (Symbol == SYMBOL_CIRCLE){
//		Alert0("hi");
		int dd=size; ///2;
		pDC->Ellipse( x-dd, y-dd, x+dd, y+dd);
		return;
	}
	if (Symbol == SYMBOL_SQUARE){
		pDC->MoveTo(x-size,y-size);
		pDC->LineTo(x+size,y-size);
		pDC->LineTo(x+size,y+size);
		pDC->LineTo(x-size,y+size);
		pDC->LineTo(x-size,y-size);
		return;
	}
	if (Symbol == SYMBOL_TRIANGLE){
		pDC->MoveTo(x-size,y+size);
		pDC->LineTo(x+size,y+size);
		pDC->LineTo(x,y-size);
//		pDC->LineTo(x-size,y+size);
		pDC->LineTo(x-size,y+size);
		return;
	}
	if (Symbol == SYMBOL_CROSS){
		pDC->MoveTo(x-size,y-size);
		pDC->LineTo(x+size,y+size);
		pDC->MoveTo(x-size,y+size);
		pDC->LineTo(x+size,y-size);
		return;
	}
	if (Symbol == SYMBOL_FILLED_CIRCLE){
//		Alert0("hi");
		for (int dd=size; dd>=1; dd--){
//		int dd=size; ///2;
			pDC->Ellipse( x-dd, y-dd, x+dd, y+dd);
		}
		return;
	}
}
/*

double LogGamma(double xx)
//Returns the value ln[Ã(xx)] for xx > 0.
{
//Internal arithmetic will be done in double precision, a nicety that you can omit if five-figure
//accuracy is good enough.
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
						24.01409824083091,-1.231739572450155,
						0.1208650973866179e-2,-0.5395239384953e-5};
	int j;
	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;

	return -tmp+log(2.5066282746310005*ser/x);
}
*/
/*

double LogGamma(double x){
//	return 1.0;
	int n1=int(x);
	int n2=n1+1;
	double rest = x-n1;
	double l1=LogFac(n1-1);
//	if (rest<1e-20){
//		return l1;
//	}
	double l2=LogFac(n2-1);
	return l1 + rest*(l2-l1);
	
	return LogFac(x-1);
}
*/
double RealLogBinom(double N, double k){
///	return LogGamma(N+1.0) - LogGamma(N-k+1.0) - LogGamma(k+1.0);
	return loggamma(N+1.0) - loggamma(N-k+1.0) - loggamma(k+1.0);
/*
	return LogBinom(N, k);
	return 1.0;
*/
}

void AddDoubleToString(BOOL & first, double d, CString & c){
	char s[100];
	_gcvt(d, 10, s);
	if (!first){
		c += CString("\t");
	}
	else{
		first = false;
	}
	c += CString(s)+ CString(" ");
}

void AddIntToString(BOOL & first, int i, CString & c){
	char s[100];
	_itoa(i, s, 10);
	if (!first){
		c += CString("\t");
	}
	else{
		first = false;
	}
	c += CString(s);
}


double erfcc(double x)
//Returns the complementary error function erfc(x) with fractional error everywhere less than 1.2 × 10-7.
{
double t,z,ans;
z=fabs(x);
t=1.0/(1.0+0.5*z);
ans=t*exp(-z*z-1.26551223+t*(1.00002368+t*(0.37409196+t*(0.09678418+
t*(-0.18628806+t*(0.27886807+t*(-1.13520398+t*(1.48851587+
t*(-0.82215223+t*0.17087277)))))))));
return x >= 0.0 ? ans : 2.0-ans;
}

double erff(double x){
	return (1.0-erfcc(x));
}

double gammln(double xx)
//Returns the value ln[Ã(xx)] for xx > 0.
{
	//Internal arithmetic will be done in double precision, a nicety that you can omit if five-figure
	//accuracy is good enough.
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;
	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}



#define ITMAXGSERGCF 400
//#define EPS 3.0e-7
#define EPSGSERGCF 3.0e-20

void gser(double *gamser, double a, double x, double *gln)
//Returns the incomplete gamma function P(a, x) evaluated by its series representation as gamser.
//Also returns ln Ã(a) as gln.
{
	double gammln(double xx);
//	void nrerror(char error_text[]);
	int n;
	double sum,del,ap;
	*gln=gammln(a);
	if (x <= 0.0) {
		if (x < 0.0) return ; //nrerror("x less than 0 in routine gser");
		*gamser=0.0;
		return;
	} else {
		ap=a;
		del=sum=1.0/a;
		for (n=1;n<=ITMAXGSERGCF;n++) {
			++ap;
			del *= x/ap;
			sum += del;
			if (fabs(del) < fabs(sum)*EPSGSERGCF) {
				*gamser=sum*exp(-x+a*log(x)-(*gln));
				return;
			}
		}
//		nrerror("a too large, ITMAX too small in routine gser");
		return;
	}
}
//#define FPMIN 1.0e-30 Number near the smallest representable floating-point number.
#define FPMINGSERGCF 1.0e-100 //Number near the smallest representable floating-point number.

void gcf(double *gammcf, double a, double x, double *gln)
//Returns the incomplete gamma function Q(a, x) evaluated by its continued fraction representation
//as gammcf. Also returns lnÃ(a) as gln.
{
	double gammln(double xx);
//	void nrerror(char error_text[]);
	int i;
	double an,b,c,d,del,h;
	*gln=gammln(a);
	b=x+1.0-a; //Set up for evaluating continued fraction by modified Lentz’s method (§5.2)	with b0 = 0.
	c=1.0/FPMINGSERGCF;
	d=1.0/b;
	h=d;
	for (i=1;i<=ITMAXGSERGCF;i++) { //Iterate to convergence.
		an = -i*(i-a);
		b += 2.0;
		d=an*d+b;
		if (fabs(d) < FPMINGSERGCF) d=FPMINGSERGCF;
		c=b+an/c;
		if (fabs(c) < FPMINGSERGCF) c=FPMINGSERGCF;
		d=1.0/d;
		del=d*c;
		h *= del;
		if (fabs(del-1.0) < EPSGSERGCF) break;
	}
//	if (i > ITMAXGSERGCF) nrerror("a too large, ITMAX too small in gcf");
	*gammcf=exp(-x+a*log(x)-(*gln))*h; //Put factors in front.
}

double gammp(double a, double x)
//Returns the incomplete gamma function P(a, x).
{
//	void gcf(double *gammcf, double a, double x, double *gln);
//	void gser(double *gamser, double a, double x, double *gln);
//	void nrerror(char error_text[]);
	double gamser,gammcf,gln;
	if (x < 0.0 || a <= 0.0) nrerror("Invalid arguments in routine gammp");
	if (x < (a+1.0)) { //Use the series representation.
		gser(&gamser,a,x,&gln);
		return gamser;
	} else { //Use the continued fraction representation
		gcf(&gammcf,a,x,&gln);
		return 1.0-gammcf;// and take its complement.
	}
}

double gammq(double a, double x)
//Returns the incomplete gamma function Q(a, x) ß 1 . P(a, x).
{
//	void gcf(double *gammcf, double a, double x, double *gln);
//	void gser(double *gamser, double a, double x, double *gln);
//	void nrerror(char error_text[]);
	double gamser,gammcf,gln;
	if (x < 0.0 || a <= 0.0) return 0;//nrerror("Invalid arguments in routine gammq");
	if (x < (a+1.0)) { //Use the series representation
		gser(&gamser,a,x,&gln);
		return 1.0-gamser;// and take its complement.
	} else { //Use the continued fraction representation.
		gcf(&gammcf,a,x,&gln);
	return gammcf;
	}
}


double erff1(double x)
//Returns the error function erf(x).
{
//double gammp(double a, double x);
	return x < 0.0 ? -gammp(0.5,x*x) : gammp(0.5,x*x);
}

double erffc1(double x)
//Returns the complementary error function erfc(x).
{
//double gammp(double a, double x);
//double gammq(double a, double x);
	return x < 0.0 ? 1.0+gammp(0.5,x*x) : gammq(0.5,x*x);
}

void GetPreNumPost(CString name, CString & pre, CString & num, CString & post){
	CString n = name;
	int l=n.GetLength();
	if (l<1){
		pre = name;
		num=_T("");
		post = _T("");
		return;
	}
//	Alert0(n);
	int dotpos = n.ReverseFind('.');
	int slashpos = n.ReverseFind('/');
	int backslashpos = n.ReverseFind('\\');
	int spos=-1;
	if (slashpos>=0){
		spos = slashpos;
		if (backslashpos>spos){
			spos = backslashpos;
		}
	}
	else{
		if (backslashpos>=0){
			spos = backslashpos;
			if (slashpos>spos){
				spos = slashpos;
			}
		}
	}

	
//	ShowFloat(dotpos, "dotpos");
//	ShowFloat(slashpos, "slpos");
//	ShowFloat(backslashpos, "bslpos");
//	ShowFloat(l, "length");

	bool dotfound = true;

	if (dotpos<0){
//		Alert0("no dotfound");
		dotfound = false;
	}
	else{
		if (spos>dotpos){
//			Alert0("no dotfound");
			dotfound = false;
		}
	}

	pre = n;
	post = _T("");

	if (dotfound){
		pre = n.Left(dotpos);
		post = n.Right(l-dotpos);
	}

	l = pre.GetLength();

	bool numfound = false;

	num=_T("");

	int index = l;

	while (true){
		index--;
		if (index<0){
			break;
		}
		char c = pre.GetAt(index);
		if ((c>='0') && (c<='9')){
			numfound = true;
			num = CString(c)+num;
			pre = pre.Left(pre.GetLength()-1);
		}
		else{
			break;
		}
	}
	int lnum = num.GetLength();
	int nzero=0;
	bool initzero = false;
	if (lnum>0){
		if (num.GetAt(0)=='0'){
			initzero = true;
			nzero=1;
		}
	}
	bool goon = initzero;
	while (goon){
		if (nzero>=lnum){
			break;
		}
		if (num.GetAt(nzero)=='0'){
			nzero++;
		}
		else{
			break;
		}
	}

	if (nzero>0){
		int ntrans = nzero;
		if (nzero==lnum){
			ntrans--;
		}
		if (ntrans>0){
			for (int i=0; i<ntrans; i++){
				pre = pre + _T("0");
			}
			num = num.Right(lnum-ntrans);
		}
	}
			


//	Alert0(num);

//	Alert0(pre);
//	Alert0(post);
}


CString IncrementOrDecrementFileName(CString name, bool Increment){
	CString pre, num, post;

	GetPreNumPost(name, pre, num, post);

	if (num.GetLength()<1){
		return pre+CString("-1")+post;
	}

	int ToAdd = 1;
	if (!Increment) ToAdd = -1;
	int inum = atoi(CStringA(num)) + ToAdd;

	char s[50];

	_itoa(inum, s, 10);

	CString AddZero;
	int NZero = 0;
	if (pre.GetLength()==0){  // We have to catch stupid names like 0.dat, 00.dat, 00234.dat etc.
		int lnum = num.GetLength();
		bool TryZero = true;
		while (TryZero){
			if (NZero<=lnum){
				CString x = CString(num.GetAt(NZero));
				int izero = atoi(CStringA(x));
				if (izero==0){						// we have a "0"
					if (Increment && (inum==1) && (NZero>=(lnum-1))){	// be aware of names of all zeros (like 0000.dat)
						TryZero = false;
					}
					else{
						if (!Increment && (inum==(-1)) && (NZero>=(lnum-1))){	// be aware of names of all zeros (like 0000.dat)
							TryZero = false;
						}
						else{
							NZero++;
							AddZero += CString("0");
						}
					}
				}
				else{
					TryZero = false;  // no more zeros
				}
			}
			else{
				TryZero = false; // beyond the length of num-string
			}
		}
	}
	return AddZero + pre + CString(s) + post;

}
CString IncrementFileName(CString name){
	return IncrementOrDecrementFileName(name, true);
}
CString DecrementFileName(CString name){
	return IncrementOrDecrementFileName(name, false);
}


CString CopyCString(CString * pc){
	if (!pc){
		return CString("");
	}

	int l=pc->GetLength();
	if (l<1) {
		return CString("");
	}
	CString r = CString(' ', l);
	r=CString("");
	for (int i=0; i<l; i++){
		r += pc->GetAt(i);
	}
	return r;
}

int Calci2(int i1, double prod){
	int i2=i1;
	//if (i1>4000){
	//	ShowFloat(i1, "i1 in calc i2");
	//	ShowFloat(prod, "prod ");
//	}
	do{
//			ShowFloat(i2, "i2");
		if ((i2+2)>prod){
			return i2;
		}
		i2++;
	}
	while(1);
}
void GetTextFromClipboard(CString & source){
	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
//		char * buffer;
//		EmptyClipboard();
		clipbuffer = GetClipboardData( CF_TEXT);
		if (!clipbuffer){
			//Alert0(_T("no txt"));
			CloseClipboard();
			return;
		}

		char * cc = (char *)clipbuffer;
		while (*cc!=0){
//			Alert0(CString(*cc));
			source += CString(*cc);
			cc++;
		}

//		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		
/*
buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, (LPCSTR)CStringA(source));
		strcpy(buffer, (LPCSTR)CStringA(source));
//		GlobalUnlock(clipbuffer);
//		SetClipboardData(CF_TEXT,clipbuffer);
*/
		CloseClipboard();
	}
}

void GetTextFromClipboard(std::string & source){
	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
//		char * buffer;
//		EmptyClipboard();
		clipbuffer = GetClipboardData( CF_TEXT);
		if (!clipbuffer){
			//Alert0(_T("no txt"));
			CloseClipboard();
			return;
		}

		char * cc = (char *)clipbuffer;
		while (*cc!=0){
//			Alert0(CString(*cc));
			source += *cc;
			cc++;
		}

//		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		
/*
buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, (LPCSTR)CStringA(source));
		strcpy(buffer, (LPCSTR)CStringA(source));
//		GlobalUnlock(clipbuffer);
//		SetClipboardData(CF_TEXT,clipbuffer);
*/
		CloseClipboard();
	}
}

void CopyTextToClipboard(CString & source){ 
//put your text in source
	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);
/*
for (int i=0; i<source.GetLength(); i++){
			buffer[i] = source.GetAt(i);
		}
		buffer[source.GetLength()]=0;
*/
		strcpy(buffer, (LPCSTR)CStringA(source));
//		strcpy_s(buffer, source.GetLength(), source);
/*
CString orig("Hello, World!");
    wcout << orig << " (CString)" << endl;

    // Convert to a char*
    const size_t newsize = 100;
    char nstring[newsize];

    strcpy_s(nstring, orig);
*/
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
}

void CalcBesselPolynomialCoefficients(int n, double * coeffs){
	if (n<1) return;
	if (!coeffs) return;
	int k;
	double rcoeffs[MAX_BESSEL_COEFFS];
	double * rr;
	if (n>MAX_BESSEL_COEFFS){
		rr=new double[n];
	}
	else{
		rr=&rcoeffs[0];
	}
	
	CalcReverseBesselPolynomialCoefficients(n, rr);

	for (k=0; k<=n; k++){
		coeffs[n-k] = rr[k]; //exp (LogFac(double(2*n-k))-LogFac(double(n-k)) - LogFac(double(k)) - double(n-k)*log(2.0) );
	}
	if (n>MAX_BESSEL_COEFFS){
		delete [] rr;
	}

}

void CalcReverseBesselPolynomialCoefficients(int n, double * coeffs){
	if (n<1) return;
	if (!coeffs) return;

	int k;
	for (k=0; k<=n; k++){
//		coeffs[n-k] = exp (LogFac(double(2*n-k))-LogFac(double(n-k)) - LogFac(double(k)) - double(n-k)*log(2.0) );
		coeffs[n-k] = exp (LogFac(2*n-k)-LogFac(n-k) - LogFac(k) - double(n-k)*log(2.0) );
	}


}

void CalcPowerSpectrumBesselCoeffs(int n, double * coeffs){
	if (n<1) return;
	if (!coeffs) return;
	double realcoeffs[MAX_BESSEL_COEFFS];
	double * rc;
	double imagcoeffs[MAX_BESSEL_COEFFS];
	double * ic;
	if (n>MAX_BESSEL_COEFFS){
		rc=new double[n];
		ic=new double[n];
	}
	else{
		rc=&realcoeffs[0];
		ic=&imagcoeffs[0];
	}

//	CalcReverseBesselPolynomialCoefficients(n, coeffs);
	CalcBesselPolynomialCoefficients(n, coeffs);

	int i;
	for (i=0; i<=n; i++){
		rc[i]=ic[i]=0.0;
	}
	for (i=0; i<=n; i++){
//		ShowFloat(i%2, "i%2");
//		ShowFloat(i%4, "i%4");

		if ( (i % 2) != 0 ){ // uneven i
			if ((i % 4) == 1){
				ic[i] = coeffs[i];
			}
			else{
				ic[i] = -coeffs[i];
			}
		}
		else{				// even i
			if ((i % 4 ) == 0){
				rc[i] = coeffs[i];
			}
			else{
				rc[i] = -coeffs[i];
			}
		}
	}
	int j;
	for (i=0; i<=n; i++){
//		ShowFloat(i,"i");
//		ShowFloat(rc[i],"rci");
//		ShowFloat(ic[i],"ici");
	}
	for (j=0; j<=2*n; j += 2){
		i = j/2;
		coeffs[i] = rc[i]*rc[i] + ic[i]*ic[i];
		for (int k=0; k<=i; k++){
			if (k<=n){
				int l=j-k;
				if (l<=n){
					if (k!=l){
						coeffs[i] += 2.0*rc[k]*rc[l]+2.0*ic[k]*ic[l];
					}
				}
			}
		}
//		coeffs[i]=rc[i];
	}
	if (n>MAX_BESSEL_COEFFS){
		delete [] rc;
		delete [] ic;
	}
}


//#include <math.h>
//#define NRANSI
//#include "nrutil.h"
#define CON_dfridr 1.4
#define CON2_dfridr (CON_dfridr*CON_dfridr)
#define BIG_dfridr 1.0e100
#define NTAB_dfridr 10
#define SAFE_dfridr 2.0

double dfridr(double (*func)(double), double x, double h, double *err)
{
	int i,j;
	double errt,fac,hh,**a,ans;

	if (h == 0.0) nrerror("h must be nonzero in dfridr.");
	a=dmatrix(1,NTAB_dfridr,1,NTAB_dfridr);
	hh=h;
	a[1][1]=((*func)(x+hh)-(*func)(x-hh))/(2.0*hh);
	*err=BIG_dfridr;
	for (i=2;i<=NTAB_dfridr;i++) {
		hh /= CON_dfridr;
		a[1][i]=((*func)(x+hh)-(*func)(x-hh))/(2.0*hh);
		fac=CON2_dfridr;
		for (j=2;j<=i;j++) {
			a[j][i]=(a[j-1][i]*fac-a[j-1][i-1])/(fac-1.0);
			fac=CON2_dfridr*fac;
//			errt=FMAX(fabs(a[j][i]-a[j-1][i]),fabs(a[j][i]-a[j-1][i-1]));
			errt=DMAX(fabs(a[j][i]-a[j-1][i]),fabs(a[j][i]-a[j-1][i-1]));
			if (errt <= *err) {
				*err=errt;
				ans=a[j][i];
			}
		}
		if (fabs(a[i][i]-a[i-1][i-1]) >= SAFE_dfridr*(*err)) break;
	}
	free_dmatrix(a,1,NTAB_dfridr,1,NTAB_dfridr);
	return ans;
}
#undef CON_dfridr
#undef CON2_dfridr
#undef BIG_dfridr
#undef NTAB_dfridr
#undef SAFE_dfridr
//#undef NRANSI



bool DirOrFileNamesAreEquivalent(CString f1, CString f2){
// Deals only with upper/lower case and slash/backslash -> not very reliable

	int l1=f1.GetLength();
	int l2=f2.GetLength();
	if (l1 != l2 ) return false;

	f1.MakeUpper();
	f2.MakeUpper();

	for (int i=0; i<l1; i++){
		char c1=f1.GetAt(i);
		char c2=f2.GetAt(i);
		if (c1!=c2){
			if ( (c1 == '/') || (c1 == '\\')) {
				if (!( (c2 == '/') || (c2 == '\\')) ){
					return false;
				}
			}
			else{
				return false;
			}
		}
	}

	return true;
}

void GetFileName(CString FullNameWithPath, CString & FileName){
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];	
	char ext[_MAX_EXT];

	_splitpath(FullNameWithPath, drive, dir, fname, ext);
	CString Ext = CString(ext);
//	Ext = Ext.Right(Ext.GetLength()-1); // Remove the dot
	FileName = CString(fname)+Ext;
}
void GetPath(CString FullNameWithPath, CString & Path){
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];	
	char ext[_MAX_EXT];

	_splitpath(FullNameWithPath, drive, dir, fname, ext);

	Path = CString(drive)+CString(dir);

}
bool FileExists(CString Name){
	CFileFind finder;
	return(finder.FindFile(Name) != 0);
}



UINT DoCheckMenuItem(bool DoCheck, CMenu * pMenu, int ID){
	UINT result;
	if (DoCheck){
		result = pMenu->CheckMenuItem(ID, MF_CHECKED);
	}
	else{
		result = pMenu->CheckMenuItem(ID, MF_UNCHECKED);
	}
	return result;
}
double Xsqrt(double x)// double x;
  { /* because of deficiencies in hypot on Sparc, it's possible for arg of Xsqrt to be small and -ve,
       which logically it can't be (since r >= |x.re|).	 Take it as 0. */
    return (x >= 0.0) ? sqrt(x) : 0.0;
  }

bool FindNextStringForward(CString & instring, CString c, unsigned __int64 & pos){
	char s1[100000];
	char s;
	if (pos>=instring.GetLength()){
		return false;
	}
//	CString instringshow = instring.Right(instring.GetLength()-pos);
//	Alert0(instringshow.Left(80));
//	pos = instring.
//	pos = file.GetPosition();

	unsigned __int64 l = c.GetLength();
//	unsigned __int64 endpos = file.GetLength();
	unsigned __int64 endpos = instring.GetLength();
	s1[l]=0;
	char first = c.GetAt(0);
	while (true){
		if ( (pos+l)>endpos){
			return false;
		}
		s=instring.GetAt(pos); pos++;
		if (s==first){
			unsigned __int64 ipos=pos;
			if (l>1){
				for (unsigned __int64 k=0; k<(l-1); k++){
					s1[1+k]=instring.GetAt(ipos); ipos++;
				}
				ipos++;
			}
			s1[0]=s;
			CString c2=CString(s1);
			if (c2.Compare(c)==0){
				pos = ipos;
				return true;
			}
		}
	}
}



bool FindNextStringForward(CFile & file, CString c, unsigned __int64 & pos){
	char s1[100000];
	char s;
	pos = file.GetPosition();
	unsigned __int64 l = c.GetLength();
	unsigned __int64 endpos = file.GetLength();
	s1[l]=0;
	char first = c.GetAt(0);
	while (true){
		if ( (pos+l)>endpos){
			return false;
		}
		file.Read(&s, 1);
		if (s==first){
			if (l>1){
				file.Read(&s1[1], UINT(l-1));
				file.Seek(pos+1, CFile::begin);
			}
			s1[0]=s;
			CString c2=CString(s1);
			if (c2.Compare(c)==0){
				return true;
			}
		}
		pos++;
//		file.Seek(pos, CFile::begin);
//		ShowFloat(pos, "pos");
	}
}

bool FindNextStringBackward(CFile & file, CString c, unsigned __int64 & pos){
	char s1[100000];
	char s;
	pos = file.GetPosition();
	unsigned __int64 l = c.GetLength();
	unsigned __int64 endpos = file.GetLength();
	s1[l]=0;
	while ((pos+l)>endpos){
		if (pos==0) return false;
		pos--;
	}
	char first = c.GetAt(0);

	while (true){

		file.Read(&s, 1);
		if (s==first){
			if (l>1){
				file.Read(&s1[1], UINT(l-1));
//				file.Seek(pos+1, CFile::begin);
			}
			s1[0]=s;
			CString c2=CString(s1);
			if (c2.Compare(c)==0){
				return true;
			}
		}
		if (pos==0) return false;
		pos--;
		file.Seek(pos, CFile::begin);
	}
	return false;
}


bool TestSettingsParameter(double Test, double Min, double Max, CString Text){
	if ((Test<Min) || (Test>Max)){
		CString c;
		char s[20];
		_gcvt(Test, 8, s);
		c = CString("Warning: Parameter ")+Text+CString("has a value of ")+CString(s)+CString(" - This seems to be incorrect");
		c += CString("\r\n Do you want to continue anyway to load this setting file?");
		return Confirm0(c);
	}
	return true;
}
bool TestSettingsParameter(int Test, int Min, int Max, CString Text){
	if ((Test<Min) || (Test>Max)){
		CString c;
		char s[20];
		_itoa(Test, s, 10);
		c = CString("Warning: Parameter ")+Text+CString("has a value of ")+CString(s)+CString(" - This seems to be incorrect");
		c += CString("\r\n Do you want to continue anyway to load this setting file?");
		return Confirm0(c);
	}
	return true;
}

__int64 Binom(int N, int k){
	__int64 result = 1;
	if (N<1) return result;
	if (k<=0) return result;
	if (k>=N) return result;
	for (int i=(k+1); i<=N; i++){
		result *= i;
	}
	for (int i=1; i<=(N-k); i++){
		result /= i;
	}
	return result;
}

bool ReadDoubleFromASCIIFile(double & d, CFile & f , bool WithComma){
	CString c1, c2;
	unsigned __int64 pos;
	if (WithComma){
		char s;
		bool cont = true;
		while (cont){
			if (f.Read(&s, 1)<1){
				return false;
			}
			if (s==','){
				cont = false;
			}
			else{
				c2 += CString(s);
			}
		}
		d = atof(c2);
		return true;
	}
	else{

		if (!ReadLine(f, c1)){
			return false;
		}
	
		pos = f.GetPosition();

		if (!ReadLine(f, c2)){
			f.Seek(pos, CFile::begin );
			d = atof(c1);
//		ShowFloat(d, "d");
			return true;
		}
	}

//	Alert0(c2);

	CString space =" ";
	if (space.Compare(c2.Left(1))==0){
//		Alert0(c1);
//		Alert0(c2);
		CString minus = "-";
		c2.TrimLeft();
//		Alert0(c2);
		if (minus.Compare(c2.Left(1))==0){
//			Alert0("minus");
			f.Seek(pos, CFile::begin );
			d = atof(c1);
//			ShowFloat(d, "d 0");
			return true;
		}
		c1.TrimRight();
		d = atof(c1+c2);
//		ShowFloat(d, "d1");
		return true;
	}
	f.Seek(pos, CFile::begin );
	d = atof(c1);
//	ShowFloat(d, "d");
	return true;

}


bool ReadDoubleFromASCIIString(int & pos, double & d, CString & source , bool WithComma){
	CString c1, c2;
//	unsigned __int64 pos;
	int len = source.GetLength();
	if (pos>=len){
		return false;
	}

	if (WithComma){
		char s;
		bool cont = true;
		while (cont){
			if (pos>=len){
				cont = false;
				break;
			}
			s = source.GetAt(pos);
			pos++;
			if (s==','){
				cont = false;
			}
			else{
				c2 += CString(s);
			}
		}
		d = atof(c2);
		return true;
	}
	else{

		if (!ReadLineFromString(source, pos, c1)){
			return false;
		}
	//	Alert0(c1);

		d = atof(c1);
		return true;

	
//		pos = f.GetPosition();
		int SavePos = pos;

		if (!ReadLineFromString(source, pos, c2)){
			pos = SavePos;
//			f.Seek(pos, CFile::begin );
			d = atof(c1);
		ShowFloat(d, "d");
			return true;
		}

	}
	

//	Alert0(c2);

//	c2 = c1;
	CString space =" ";
	if (space.Compare(c2.Left(1))==0){
//		Alert0(c1);
//		Alert0(c2);
		CString minus = "-";
		c2.TrimLeft();
//		Alert0(c2);
		if (minus.Compare(c2.Left(1))==0){
//			Alert0("minus");
//			f.Seek(pos, CFile::begin );
			d = atof(c1);
//			ShowFloat(d, "d 0");
			return true;
		}
		c1.TrimRight();
		d = atof(c1+c2);
//		ShowFloat(d, "d1");
		return true;
	}
//	f.Seek(pos, CFile::begin );
	d = atof(c1);
//	ShowFloat(d, "d");
	return true;

}

void GetBaseAndExponentBase10(double value, float & base, int & exponent){
	double absvalue=fabs(value);
	if (absvalue==0.0){
		base = 0;
		exponent = 0;
		return;
	}
	exponent = int(log10(absvalue));
	double divid = pow(10.0, exponent);
	base = float(value/divid);
	if (value<0.0) base *= -1.0;
}

void MiksEnhancedPolyLine(vector <CPoint> & points, int N, CDC *pDC){
	if (N<1) return;
	int x=points[0].x;
	int max=points[0].y;
	int min=max;
	int oldx=x;
	int oldmean=min;
	int oldmin=min;
	int oldmax = max;
	
	pDC->MoveTo(x, min);

	for (int i=1; i<N; i++){

		if (points[i].x>x){		// at least 1 pixel advanced
			if (min==max){
//				pDC->MoveTo(oldx, oldmean);
				pDC->LineTo(x, min);
				pDC->LineTo(points[i].x, points[i].y);
			}
			else{
				if (min>oldmax){
					pDC->LineTo(x,min);
				}
				else{
					if (max<oldmin){
						pDC->LineTo(x,max);
					}
					else{
//						if ( (x-oldx)>1){
						if ( (x-oldx)>0){

							pDC->MoveTo(oldx, oldmean);
							pDC->LineTo(x, (min+max)/2);
						}
					}
				}
				if (i==(N-1)){
					pDC->LineTo(points[i].x, points[i].y);
				}
				else{
					pDC->MoveTo(x, min);
					pDC->LineTo(x, max);
				}


			}
			oldx=x;
			oldmean = (min+max)/2;
			oldmin = min;
			oldmax = max;
			x=points[i].x;
			min=max=points[i].y;
		}
		else{
			int y=points[i].y;
			if (y>max) max = y;
			if (y<min) min = y;
		}
	}

}

bool GetNextNonEmptyString(int & index, int NStrings, CString * stringlist){
	while (index<NStrings){
		CString c = stringlist[index];
		c.TrimLeft();
		c.TrimRight();
//		Alert0(c);
		if (c.GetLength()>0){
			return true;
		}
		index ++;
	}
	return false;
}

bool GetNextNonEmptyString(int & index, vector <CString> & stringlist){
	int NStrings = stringlist.size();
	while (index<NStrings){
		CString c = stringlist[index];
		c.TrimLeft();
		c.TrimRight();
//		Alert0(c);
		if (c.GetLength()>0){
			return true;
		}
		index ++;
	}
	return false;
}
int AlgLibInvertFloatMat(float ** A, float ** B, int n, float * det){
	// returns the return code of alglib
	alglib::real_2d_array a;
	a.allocate_own(n, n, alglib_impl::DT_REAL);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			a[i][j] = A[i][j];
		}
	}
	int info;
	alglib::matinvreport rep;
	alglib::rmatrixinverse( a, n, info, rep);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			B[i][j] = a[i][j];
		}
	}
	if (det){
		*det =0;
		if (info>0){
			for (int i=0; i<n; i++){
				for (int j=0; j<n; j++){
					a[i][j] = A[i][j];
				}
			}
			*det = alglib::rmatrixdet(a, n);
		}
	}
	return info; // 1 ok, -3: singular
}


int AlgLibInvertRealMat(double ** A, double ** B, int n, double * det){
	// returns the return code of algli
	alglib::real_2d_array a;
	a.allocate_own(n, n, alglib_impl::DT_REAL);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			a[i][j] = A[i][j];
		}
	}
	int info;
	alglib::matinvreport rep;
	alglib::rmatrixinverse( a, n, info, rep);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			B[i][j] = a[i][j];
		}
	}
	if (det){
		*det =0;
		if (info>0){
			for (int i=0; i<n; i++){
				for (int j=0; j<n; j++){
					a[i][j] = A[i][j];
				}
			}
			*det = alglib::rmatrixdet(a, n);
		}
	}
	return info; // 1 ok, -3: singular
}
int AlgLibInvertComplexMat(complex<double> ** A, complex<double> ** B, int n,
						   complex<double> * det){
	// returns the return code of alglib
	alglib::complex_2d_array a;
	a.allocate_own(n, n, alglib_impl::DT_COMPLEX);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){

			alglib::complex c;
			c.x = A[i][j].real();
			c.y = A[i][j].imag();
			a[i][j] = c;

//			a[i][j] = (A[i][j].real(), A[i][j].imag());
		}
	}
	int info;
	alglib::matinvreport rep;
	alglib::cmatrixinverse( a, n, info, rep);
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			alglib::complex c = a[i][j];
			B[i][j].real(c.x);
			B[i][j].imag(c.y);
		}
	}
	if (det){
		*det =0;
		if (info>0){
			for (int i=0; i<n; i++){
				for (int j=0; j<n; j++){
					alglib::complex c;
					c.x = A[i][j].real();
					c.y = A[i][j].imag();
					a[i][j] = c;
	//				a[i][j] = (A[i][j].real(), A[i][j].imag());
				}
			}
			alglib::complex algdet = alglib::cmatrixdet(a, n);
			det->real(algdet.x);
			det->imag(algdet.y);
		}
	}
	return info; // 1 ok, -3: singular
}


double TrimDecimals(double value, int digits){
	double p = pow(10.0, digits);
	
	return ((int)(value * p + .5) / p);

}


void MyEnhancedPolyLine(POINT * points, int N, CDC *pDC){
//	MyNewEnhancedPolyLine(points, N, pDC);
//	return;
	int x=points[0].x;
	int max=points[0].y;
	int min=max;
	int oldx=x;
	int oldmean=min;
	int oldmin=min;
	int oldmax = max;
	
	pDC->MoveTo(x, min);

	for (int i=1; i<N; i++){
		if (points[i].x>x){		// at least 1 pixel advanced
			if (min==max){

				pDC->MoveTo(oldx, oldmean);
				pDC->LineTo(x, min);
				pDC->LineTo(points[i].x, points[i].y);
			}
			else{
				if (min>oldmax){

					pDC->LineTo(x,min);
				}
				else{
					if (max<oldmin){
						pDC->LineTo(x,max);
					}
					else{
						if ( (x-oldx)>1){

							pDC->MoveTo(oldx, oldmean);
							pDC->LineTo(x, (min+max)/2);
						}
					}
				}
				pDC->MoveTo(x, min);
				pDC->LineTo(x, max);
			}
			oldx=x;
			oldmean = (min+max)/2;
			oldmin = min;
			oldmax = max;
			x=points[i].x;
			max=points[i].y;
			min=max;
		}
		else{
			int y=points[i].y;
			if (y>max) max = y;
			if (y<min) min = y;
		}
	}
}

complex <double> csqrt(complex <double> x)// struct complex x;
  { 
	  double rr = x.real();
	 double r = _hypot(x.imag(), rr);
	complex <double> z (Xsqrt(0.5 * (r + rr)), Xsqrt(0.5 * (r - rr)));
//	z.real = Xsqrt(0.5 * (r + x.real()));
//	z.imag = Xsqrt(0.5 * (r - x.real()));
	if (x.imag() < 0.0){
		z = cconj(z);

	}
    return z;
  }
complex <double> eval(complex <double> * coeffs, int np, complex <double> z){// struct complex coeffs[]; int np; struct complex z;
//  { /* evaluate polynomial in z, substituting for z */
		complex <double> sum (0,0);
//    sum = czero;
//    for (int i=np; i >= 0; i--) sum = cadd(cmul(sum, z), coeffs[i]);
		for (int i=np; i >= 0; i--){
			sum = sum*z + coeffs[i];
//			sum = cadd(cmul(sum, z), coeffs[i]);
		}
    return sum;
  }


//template <class T>
complex<double> evaluate(complex <double> * topco, complex <double> * botco, int np, complex <double> z)
// struct complex topco[], botco[]; int np; struct complex z;
  { /* evaluate response, substituting for z */
	  return eval(topco, np, z)/eval(botco, np, z);
//    return cdiv(eval(topco, np, z), eval(botco, np, z));
  }

complex <double> cexp(complex <double> x) {
//	complex <double> z;
	double r = exp(x.real());
	double i = x.imag();
	complex <double> z (r * cos(i), r * sin(i));
//    z.real = r * cos(x.imag());
 //   z.imag = r * sin(x.imag());
    return z;
  }

double cnorm(complex <double> x){
	return sqrt(x.real()*x.real()+x.imag()*x.imag());
}
void MyEnhancedPolyLine(vector<CPoint> & points, CDC *pDC){
	 int N=points.size();
	 if (N<2){
		 return;
	 }
	 if (!pDC) return;
	int x=points[0].x;
	int max=points[0].y;
	int min=max;
	int oldx=x;
	int oldmean=min;
	int oldmin=min;
	int oldmax = max;
	
	pDC->MoveTo(x, min);

	for (int i=1; i<N; i++){
		if (points[i].x>x){		// at least 1 pixel advanced
			if (min==max){
//				pDC->MoveTo(oldx, oldmean);
				pDC->LineTo(x, min);
				pDC->LineTo(points[i].x, points[i].y);
			}
			else{
				if (min>oldmax){
					pDC->LineTo(x,min);
				}
				else{
					if (max<oldmin){
						pDC->LineTo(x,max);
					}
					else{
						if ( (x-oldx)>1){

							pDC->MoveTo(oldx, oldmean);
							pDC->LineTo(x, (min+max)/2);
						}
					}
				}
				pDC->MoveTo(x, min);
				pDC->LineTo(x, max);
			}
			oldx=x;
			oldmean = (min+max)/2;
			oldmin = min;
			oldmax = max;
			x=points[i].x;
			min=max=points[i].y;
		}
		else{
			int y=points[i].y;
			if (y>max) max = y;
			if (y<min) min = y;
		}
	}
}


bool ScanDoubleFromStringNoComma(CString & c, double & d){
	c.TrimLeft();
	CString g;
	int l = c.GetLength();
	int pos = 0;

	while (true){
		if (c.GetLength()<1){
			break;
		}
		char s = c.GetAt(0);
		if (s==' '){
			c.TrimLeft();
			break;
		}
		if (s=='\t'){
			c = c.Right(c.GetLength()-1);
			break;
		}
		if (s=='\r'){
			c = c.Right(c.GetLength()-1);
			break;
		}
		g += s;
		c = c.Right(c.GetLength()-1);
	}
	g.TrimLeft();
	g.TrimRight();
	if (g.GetLength()<1){
		return false;
	}
	d  =atof(g);
	return true;
}

void MakeButton(CButton * & b, CString title,  CWnd * pW, UINT ID){
	DWORD style = WS_CHILD | WS_VISIBLE   ;
	CRect brect;
	brect.left = 0;
	brect.right=10;
	brect.top =1;
	brect.bottom = 10;
	b = new CButton;
	b->Create(title, style, brect, pW, ID );
}

std::string RightString(std::string & s, unsigned __int32 n){
	std::string result;
	unsigned __int32 l = s.length();
	if (n<l){
		result = s.substr(l - n);
	}
	return result;
}
std::string LeftString(std::string & s, unsigned __int32 n){
	std::string result;
	//Alert0_Std("Input:" + s);
	//ShowFloat(n, "n");
	unsigned __int32 l = s.length();
	if (n<=l){
		result = s.substr(0,n);
	}
	else{
		result = s;
	}
	//Alert0_Std("result:" + result);
	return result;
}


/*
bool EnsembleIDGeneNamePair::GetIDGeneNamePairFromString(CString & c){
	if (c.GetLength()<2){
		return false;
	}
	int postab = c.Find('\t');
	//ShowFloat(postab, "postab");
	this->EnsembleID = c.Left(postab);
	this->GeneName = c.Right(c.GetLength()-postab-1);
	return true;
};
*/
bool EnsembleIDGeneNamePair::GetIDGeneNamePairFromString(std::string & c){
	if (c.length()<2){
		return false;
	}
	int postab = c.find('\t');
	//ShowFloat(postab, "postab");
	this->SEnsembleID = LeftString(c, postab);
	this->SGeneName = RightString(c, c.length()-postab-1);
	return true;
};

/*
bool EnsembleIDGeneNamePair::GetIDGeneNameTissueExpressionFromString(CString & c){
	if (c.GetLength()<2){
		return false;
	}
	CString word;
	if (!this->GetnthTabulatedWord(1, c, word)){
		return false;
	}
	this->EnsembleID = word;
	if (!this->GetnthTabulatedWord(2, c, word)){
		return false;
	}
	this->GeneName = word;
	if (!this->GetnthTabulatedWord(3, c, word)){
		return false;
	}
	this->Tissue = word;
	if (!this->GetnthTabulatedWord(4, c, word)){
		return false;
	}

	this->Expression = word;
	this->DoubleExpression = atof(word);
	return true;
}
*/
bool EnsembleIDGeneNamePair::GetIDGeneNameTissueExpressionFromString(std::string & c){
	if (c.length()<2){
		return false;
	}
	std::string word;
	if (!this->GetnthTabulatedWord(1, c, word)){
		return false;
	}
	this->SEnsembleID = word;
	if (!this->GetnthTabulatedWord(2, c, word)){
		return false;
	}
	this->SGeneName = word;
	if (!this->GetnthTabulatedWord(3, c, word)){
		return false;
	}
	this->STissue = word;
	if (!this->GetnthTabulatedWord(4, c, word)){
		return false;
	}

	this->SExpression = word;
	this->DoubleExpression = atof(word.c_str());
	return true;
}

/*
bool EnsembleIDGeneNamePair::GetFirstTabulatedWord(CString & line, CString & result){
	if (line.GetLength()<1){
		return false;
	}
	int postab = line.Find('\t');
	//ShowFloat(postab, "postab");
	result = line.Left(postab);
	return true;
}
*/

bool EnsembleIDGeneNamePair::GetFirstTabulatedWord(std::string & line, std::string & result){
	if (line.length()<1){
		return false;
	}
	int postab = line.find('\t');
	//ShowFloat(postab, "postab");
	result = LeftString(line, postab);
	return true;
}


/*
bool EnsembleIDGeneNamePair::GetnthTabulatedWord(int n, CString & line, CString & result){ // start from 1
	if (line.GetLength()<1){
		return false;
	}

	CString CopyLine = line;
	//ShowFloat(postab, "postab");
	int i=1;

	while (true){
		int postab = CopyLine.Find('\t');
		if ((postab<0) || (postab>CopyLine.GetLength())){
			if (CopyLine.GetLength()<1){
				return false;
			}
			postab = CopyLine.GetLength();
		}

		if (i==n){
			result = CopyLine.Left(postab);
			return true;
		}
		i++;
		CopyLine = CopyLine.Right(CopyLine.GetLength()-postab-1);

	}
}
*/

bool EnsembleIDGeneNamePair::GetnthTabulatedWord(int n, std::string & line, std::string & result){ // start from 1
	if (line.length()<1){
		return false;
	}

	std::string CopyLine = line;
	//ShowFloat(postab, "postab");
	int i=1;

	while (true){
		int postab = CopyLine.find('\t');
		if ((postab<0) || (postab>CopyLine.length())){
			if (CopyLine.length()<1){
				return false;
			}
			postab = CopyLine.length();
		}

		if (i==n){
			result = LeftString(CopyLine,postab);
			return true;
		}
		i++;
		CopyLine = RightString(CopyLine,CopyLine.length()-postab-1);

	}
}


/*
bool EnsembleIDGeneNamePair::SubstitutenthTabulatedWord(int n, CString & line, CString & subst){
	if (line.GetLength()<1){
		return false;
	}
//	CString CopyLine = line;
	//ShowFloat(postab, "postab");
	int i=1;

	int lastpos = 0;
	while (true){
		int postab = line.Find('\t', lastpos);
		if ((postab<0) || (postab>line.GetLength())){
			postab = line.GetLength();
			if (i<n){
				return false;
			}
		}
		if (i==n){
			line = line.Left(lastpos)+subst+line.Right(line.GetLength()-postab);
			return true;
		}
		lastpos=postab+1;
		i++;
	}
}
*/

bool EnsembleIDGeneNamePair::SubstitutenthTabulatedWord(int n, std::string & line, std::string  & subst){
	if (line.length()<1){
		return false;
	}
//	CString CopyLine = line;
	//ShowFloat(postab, "postab");
	int i=1;

	int lastpos = 0;
	while (true){
		int postab = line.find('\t', lastpos);
		if ((postab<0) || (postab>line.length())){
			postab = line.length();
			if (i<n){
				return false;
			}
		}
		if (i==n){
			line = LeftString(line, lastpos)+subst+RightString(line,line.length()-postab);
			return true;
		}
		lastpos=postab+1;
		i++;
	}
}

EnsembleIDGeneNamePair::EnsembleIDGeneNamePair(){
	DoubleExpression = 0.0;
	IntPatientID = 0;
	NForAverage = 0;
}
EnsembleIDGeneNamePair::~EnsembleIDGeneNamePair(){
}

/*
 bool EnsembleIDGeneNamePair::SubstituteFirstTabulatedWord(CString & line, CString & subst){
	if (line.GetLength()<1){
		return false;
	}
	int postab = line.Find('\t');
	//ShowFloat(postab, "postab");
	line = subst+line.Right(line.GetLength()-postab);
	return true;
}
*/

bool EnsembleIDGeneNamePair::SubstituteFirstTabulatedWord(std::string & line, std::string & subst){
	if (line.length()<1){
		return false;
	}
	int postab = line.find('\t');
	//ShowFloat(postab, "postab");
	line = subst+RightString(line, line.length()-postab);
	return true;
}


/*
bool EnsembleIDGeneNamePair::GetGeneNamePatientCancerExpressionFromString(CString & c){
	if (c.GetLength()<2){
		return false;
	}
	CString word;
	if (!this->GetnthTabulatedWord(1, c, word)){
		return false;
	}
	this->GeneName = word;


	if (!this->GetnthTabulatedWord(2, c, word)){
		return false;
	}
	this->PatientID = word;
	this->IntPatientID= atoi(word);

	if (!this->GetnthTabulatedWord(3, c, word)){
		return false;
	}
	this->Cancer = word;


	if (!this->GetnthTabulatedWord(4, c, word)){
		return false;
	}

	this->Expression = word;
	this->DoubleExpression = atof(word);
	return true;
}
*/

bool EnsembleIDGeneNamePair::GetGeneNamePatientCancerExpressionFromString(std::string & c){
	if (c.length()<2){
		return false;
	}
	std::string word;
	if (!this->GetnthTabulatedWord(1, c, word)){
		return false;
	}
	this->SGeneName = word;


	if (!this->GetnthTabulatedWord(2, c, word)){
		return false;
	}
	this->SPatientID = word;
	this->IntPatientID= atoi(word.c_str());

	if (!this->GetnthTabulatedWord(3, c, word)){
		return false;
	}
	this->SCancer = word;


	if (!this->GetnthTabulatedWord(4, c, word)){
		return false;
	}

	this->SExpression = word;
	this->DoubleExpression = atof(word.c_str());
	return true;
}

bool CStringsAreIdentical(CString & c1, CString & c2){
	unsigned __int64 l1 = c1.GetLength();
	unsigned __int64 l2 = c2.GetLength();
	if (l1!=l2) return false;
	if (l1==0) return true;
	for (unsigned __int64 i=0; i<l1; i++){
		if (c1.GetAt(i)!=c2.GetAt(i)){
			return false;
		}
	}
	return true;
}
bool StdStringsAreIdentical(std::string & c1, std::string & c2){
	unsigned __int64 l1 = c1.length();
	unsigned __int64 l2 = c2.length();
	if (l1!=l2) return false;
	if (l1==0) return true;
	for (unsigned __int64 i=0; i<l1; i++){
		if (c1.at(i)!=c2.at(i)){
			return false;
		}
	}
	return true;
}


/*
bool EnsembleIDGeneNamePair::GetGeneNameCancerExpressionFromString(CString & c){
	if (c.GetLength()<2){
		return false;
	}
	CString word;
	if (!this->GetnthTabulatedWord(1, c, word)){
		return false;
	}
	this->GeneName = word;

	if (!this->GetnthTabulatedWord(2, c, word)){
		return false;
	}
	this->Cancer = word;


	if (!this->GetnthTabulatedWord(3, c, word)){
		return false;
	}

	this->Expression = word;
	this->DoubleExpression = atof(word);
	return true;
}
*/

bool EnsembleIDGeneNamePair::GetGeneNameCancerExpressionFromString(std::string & c){
	if (c.length()<2){
		return false;
	}
	std::string word;
	if (!this->GetnthTabulatedWord(1, c, word)){
		return false;
	}
	this->SGeneName = word;

	if (!this->GetnthTabulatedWord(2, c, word)){
		return false;
	}
	this->SCancer = word;

	if (!this->GetnthTabulatedWord(3, c, word)){
		return false;
	}

	this->SExpression = word;
	this->DoubleExpression = atof(word.c_str());
	return true;
}

double CorrectRS(double valueInA, double RSInOhm, double VoltageInV, double ER){
	double VoltageError = valueInA*RSInOhm;
	//ShowFloat(VoltageError, "ERRR");
	double TrueVoltage = VoltageInV - VoltageError;
	double m = valueInA/(TrueVoltage-ER);
	double CorrectedCurrent = (VoltageInV - ER) * m;
	return CorrectedCurrent;
}

void extractIntegerWords(std::string & str, vector <int> & numbers){
	std::stringstream ss;
 
    /* Storing the whole string into string stream */
    ss << str;
 
    /* Running loop till the end of the stream */
	std::string temp;
    int found;
    while (!ss.eof()) {
 
        /* extracting word by word from stream */
        ss >> temp;
 
        /* Checking the given word is integer or not */
		if (stringstream(temp) >> found){
//			ShowInt(found, "found");
			numbers.push_back(found);
//            cout << found << " ";
		}
 
        /* To save from space at the end of string */
        temp = "";
    }
}

bool GetNextWordInLine(CString & line, CString & Word){
	Word ="";
//	Alert0(CString("enter ")+line);
	line.TrimLeft();
	line.TrimRight();
	if (line.GetLength()<1) return false;

	CString backt="\t";
	CString space=" ";
	int nback=backt.GetAt(0);
	int nsp = space.GetAt(0);
	while (true){
		//Alert0(line);
		
		char c = line.GetAt(0);
//		Alert0(line.Left(1));
		int l= line.GetLength();
		if (c==nback || c==nsp){
//			Alert0("Is b");
			line = line.Right(l-1);
			return (Word.GetLength()>0);
		}
		Word = Word + c;
		if (l>1){
			line = line.Right(l-1);
		}
		else{
			line ="";
			return true;
		}
	}


	return false;
}

double GetLargerAbsValue(double d1, double d2){
	if (fabs(d1)>fabs(d2)){
		return fabs(d1);
	}
	else{
		return fabs(d2);
	}
}
int ReadDoublesInLine(std::string line, vector<double> & result){
	std::istringstream iss(line);

    // Iterate the istringstream
    // using do-while loop
	while(true){
		std::string substring;

        // Get the word from the istringstream
        iss >> substring;
//		CString cc=CString(substring.c_str());
//		Alert0(CString(substring.c_str()));
	
		if (substring.length()>0){
//			result.push_back(substring.at.stod());
			result.push_back(atof(substring.c_str()));
		}
		else{
			break;
		}
		if (!iss){
			break;
		}
    }
	return result.size();
}

bool StringsAreEqualNotRegardingCase(CString c1, CString c2){
	if (c1.CompareNoCase(c2)==0){
		return true;
	}
	return false;
}
