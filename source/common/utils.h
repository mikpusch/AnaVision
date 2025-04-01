#pragma once
#include <vector>
using namespace std ;
#include <iostream>

//#ifndef _MIKUTILS

//#define _MIKUTILS

#define USEMYMEM false

#include <math.h>
#include <complex.h>
#include <complex>


//#define USEMYMEM TRUE
#define uint	    unsigned int

#define PI_GENERAL_FILTER	    3.14159265358979323846
#define TWOPI_GENERAL_FILTER	    (2.0 * PI_GENERAL_FILTER)
#define EPS_GENERAL_FILTER	    1e-10
#define MAXORDER_GENERAL_FILTER    10
#define MAXPOLES_GENERAL_FILTER    (2*MAXORDER_GENERAL_FILTER)    /* to allow for doubling of poles in BP filter */

#define MaxCoeffInDoFilter 1000

//#define PIGRECO 3.14159265358979323846264338327950288419716939937510
#define PIGRECO 3.141592653589793
#define FARADAY 96485.30929 
#define AVOGADRO 6.0221415e23
#define ELEMENTARYCHARGE 1.60217646e-19

#define MAX_BESSEL_COEFFS	200

#define STOPKEY			115
#define CLEARKEY		99
#define ASKKEY			133
#define SMALLERKEY		60
#define BACKSLASH		92
#define NUMBERKEY		35
#define YESKEY			121
#define NOKEY			110
#define QUITKEY			113
#define AUTOMATICKEY	97
#define RUSHKEY			114
#define BACKKEY			98
#define GOODKEY			103
#define SPACEKEY		32
#define INKEY			105
#define F12KEY			VK_F12
#define F11KEY			VK_F11
#define F10KEY			VK_F10
#define ESCAPEKEY		VK_ESCAPE

#define ReBy(xxx) if (!ReadBytes(fp, pos, sizeof(xxx), &(xxx))) return false;
#define ReBy1(xxx) if (!ReadBytes(fp, pos, sizeof(xxx), &(xxx))) return -1;
#define ReBy2(xxx) if (!ReadBytes(fp, pos, sizeof(xxx), &(xxx))) return -2;
#define WriBy(xxx) WriteBytes(fp, pos, sizeof(xxx), &(xxx));

#define WriByNS(xxx) WriteBytesNoSeek(fp, sizeof(xxx), &(xxx));
#define ReByNS(xxx) if (!ReadBytesNoSeek(fp, sizeof(xxx), &(xxx))) return false;
#define ReByTRUENS(xxx) if (!ReadBytesNoSeek(fp, sizeof(xxx), &(xxx))) return true;

#define Alert(s) MessageBox(s, _T("Sorry"), MB_OK)

#define Confirm(s) (IDYES==MessageBox(s, _T("Please confirm"), MB_ICONQUESTION | MB_YESNO))
#define Confirm0(s) (IDYES==MessageBox(NULL, s, _T("Please confirm"), MB_ICONQUESTION | MB_YESNO))


#define MyWriteVariable(xxx)		if (fp) { fp->Write (&(xxx) , sizeof(xxx)); }\
									byteswritten += sizeof(xxx);

#define MyReadVariable(xxx)	if (bytesavailable<sizeof(xxx)){\
								char s;\
								for (int j=0; j<bytesavailable; j++){\
									fp->Read(&s, 1);\
								}\
								return false;\
							}\
							else{\
								fp->Read(&(xxx), sizeof(xxx));\
								bytesavailable -= sizeof(xxx);\
							}
#define MyReadVariableTrue(xxx) if (bytesavailable<sizeof(xxx)){\
								char s;\
								for (int j=0; j<bytesavailable; j++){\
									fp->Read(&s, 1);\
								}\
								return false;\
							}\
							else{\
								fp->Read(&(xxx), sizeof(xxx));\
								bytesavailable -= sizeof(xxx);\
								return true;\
							}



#define BF_TYPE 0x4D42             /* "MB" */

//BOOL UseGaussSeidel;

template <class T>
complex <T> cconj(complex <T> z){// struct complex coeffs[]; int np; struct complex z;
	complex <T> x = complex <T> (z.real(), -z.imag());
	return x;
}

class MyCFile : public CFile{
public:
	void Write( const void* lpBuf,unsigned __int64 nCount );
};

bool CStringsAreIdentical(CString & c1, CString & c2);
bool StdStringsAreIdentical(std::string & c1, std::string & c2);

std::string RightString(std::string & s, unsigned __int32 n);
std::string LeftString(std::string & s, unsigned __int32 n);

class EnsembleIDGeneNamePair{
public:
	EnsembleIDGeneNamePair();
	~EnsembleIDGeneNamePair();
	std::string SEnsembleID;
	std::string SGeneName;
	//CString EnsembleID;
	//CString GeneName;
	//CString Tissue;
	std::string STissue;
	//CString Cancer;
	std::string SCancer;
	//CString PatientID;
	std::string SPatientID;
	int IntPatientID;
	//CString Expression;
	std::string SExpression;
	double DoubleExpression;

	int NForAverage;

	//bool GetIDGeneNamePairFromString(CString & c);
	bool GetIDGeneNamePairFromString(std::string & c);
	//bool GetIDGeneNameTissueExpressionFromString(CString & c);
	bool GetIDGeneNameTissueExpressionFromString(std::string & c);
	//bool GetGeneNamePatientCancerExpressionFromString(CString & c);
	bool GetGeneNamePatientCancerExpressionFromString(std::string & c);
	//bool GetGeneNameCancerExpressionFromString(CString & c);
	bool GetGeneNameCancerExpressionFromString(std::string & c);
	//static bool GetFirstTabulatedWord(CString & line, CString & result);
	static bool GetFirstTabulatedWord(std::string & line, std::string & result);
	//static bool SubstituteFirstTabulatedWord(CString & line, CString & subst);
	static bool SubstituteFirstTabulatedWord(std::string & line, std::string & subst);
	//static bool GetnthTabulatedWord(int n, CString & line, CString & result); // start from 1
	static bool GetnthTabulatedWord(int n, std::string  & line, std::string  & result); // start from 1
	//static bool SubstitutenthTabulatedWord(int n, CString & line, CString & subst);
	static bool SubstitutenthTabulatedWord(int n, std::string& line, std::string & subst);
};

void InitUtil();

double cnorm(complex <double> x);
complex <double> cexp(complex <double> x);
complex <double> eval(complex <double> * coeffs, int np, complex <double> z);
complex<double> evaluate(complex <double> * topco, complex <double> * botco, int np, complex <double> z);
complex <double> csqrt(complex <double> x);// struct complex x;


double TrimDecimals(double value, int digits);

char * GetStringArray(CString & c);

double my_dmax (double d1, double d2);
double my_dmin (double d1, double d2);

double CorrectRS(double valueInA, double RSInOhm, double VoltageInV, double ER);

void MyEnhancedPolyLine(vector<CPoint> & points, CDC *pDC);

void ClearMemory(__int8 * x, __int64 s);

double cot(double x);
double erfcc(double x); //Returns the complementary error function erfc(x) with fractional error everywhere less than 1.2 × 10-7.

double erff(double x);

double gammln(double xx);

double erff1(double x);
//Returns the error function erf(x).
double erffc1(double x);
//erf1 and erfc1 are highly accurate but more time consuming

void GetBaseAndExponentBase10(double value, float & base, int & exponent); // base 10!

//void MiksEnhancedPolyLine(vector <CPoint> & points, int N, CDC *pDC);

void SetMessageWindow(HWND mw);

double LogFac(int N);

double TrueLogBinom(int N, int k);

double LogBinom2(int N, int k);

double LogBinom(int N, int k);

double TrueLogTrinom(int N, int k1, int k2);

double LogTrinom2(int N, int k1, int k2);

double LogTrinom(int N, int k1, int k2);

bool FindNextStringForward(CFile & file, CString c, unsigned __int64 & pos);
bool FindNextStringBackward(CFile & file, CString c, unsigned __int64 & pos);

bool FindNextStringForward(CString & instring, CString c, unsigned __int64 & pos);
bool FindNextStringBackward(CString & instring, CString c, unsigned __int64 & pos);


bool BitIsIn(UINT bit, UINT word);
void AddBit(UINT bit, UINT & word);

int ConfirmOld(CString question);
	// returns 0 if not confirm

void Alert0(CString message);
void Alert0_Std(std::string message);

void ShowLongInt(__int32 value, CString text);

void ShowInt(int value, CString text);

void ShowUInt(unsigned __int32 value, CString text);

void ShowFloat(double value, CString text);

void ShowFloat(int value, CString text);

void ShowFloat(UINT value, CString text);

void ShowFloat(unsigned __int64 value, CString text);

void ShowDouble(double value, CString text);

void ShowString(CString st, CString text);

void FlushKeyboard(int number);

void Wait(double secs);

int InKey();

int WaitKey();
int WaitKeyWithSleep(int ms);


void MakeUnits(double & value, CString Basic, CString & result);

void CalcExData (int nexp, int ndata,
			 float *xdata, float *rates, float **expdata);

int InvertMat (float **B, float **A, int n1, float *determ = NULL);
	// >0: OK; <0 : singular

int InvertDoubleMat (double **B, double **A, int n1, double *determ = NULL);
	// >0: OK; <0 : singular

int InvertComplexMat (complex<double> **B, complex<double> **A, int n1,
					   complex<double> *determ = NULL);
	// >0: OK; <0 : singular

int AlgLibInvertFloatMat(float ** A, float ** B, int n, float * det = NULL);
	// returns the return code of alglib
int AlgLibInvertRealMat(double ** A, double ** B, int n, double * det = NULL);
	// returns the return code of alglib
int AlgLibInvertComplexMat(complex<double> ** A, complex<double> ** B, int n,
						   complex<double> * det = NULL);
	// returns the return code of alglib


void FitPoly (int npoly, int ndata, double *xdata,
	      double *ydata, double *params); // returns sd

template <class T>
void Transpose(T** A, T** AT, int n){
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			AT[i][j]=A[j][i];
		}
	}

}

template <class T>
void MulMat (T **A, T **B, T **c,
			 int n, int m, int l){

///    A= B * C 

	int i, j, k;
	T temp;

	if ((n < 1) | (m < 1) | (l < 1)) {
		return;
	}

	for (i=0; i<n; i++){
		for (j=0; j<l; j++){
			temp = 0.0;
			for (k=0; k<m; k++) {
				temp += B[i][k] * c[k][j];
			}
			A[i][j] = temp;
		}
	}

}

/*
void DMulMat (double **A, double **B, double **c,
	     int n, int m, int l);
*/
void DVectTimesMat (double *A, double *B, double **c,
	     int m, int l);

void DMatTimesVect (double *A, double **B, double *c,
	     int n, int m);

void GetExCoeffs (int nexps,
				  int ndata, 
				  //float *xdata,
		          __int16 *ydata,
				  double *params,
				  double *coeffs,
				  int CalcSD,
				  double *sd);
//				  int KeepOldWork);

void GetDoubleExCoeffs (int nexps,
					  int ndata, 
				  //float *xdata,
					double   *ydata,
				  double *params,
				  double *coeffs,
				  int CalcSD,
		          double *sd);

//void GetFuncCoeffs (int nfunc, int ndata, float **funcs,
//		    float *ydata, float *params,
//		    int CalcSD, double *sd, int KeepOldWork);
void GetFuncCoeffs (int nfunc, int ndata, float **funcs,
		    float *ydata, float *params,
		    int CalcSD, double *sd, float * fit=NULL);

void GetFuncCoeffs (int nfunc, int ndata, double **funcs,
		    double *ydata, double *params,
		    int CalcSD, double *sd, double * fit=NULL);

double MaxDouble (double * d, int np); //returns  max of an array
double AbsMaxDouble (double * d, int np); //returns  absolute max of an array
double AbsMinDouble (double * d, int np,  int & minindex); //returns  absolute min of an array
double MinDouble (double * d, int np); //returns  min of an array
double MeanDouble (double * d, int np); //returns  mean of an array

template <class T>
T MeanArray(T * d, int np){
	if (np<=0) return T(0);
	double res=double(d[0]);
	for (int i=1; i<np; i++)
		res += double(d[i]);

	return T(res/double(np));
}


int MaxInt (int * d, int np);
int MinInt (int * d, int np);
int MeanInt (int * d, int np);
int AbsMaxInt (int * d, int np);


__int16 MeanInt16(__int16 * d, int np);
__int16 MaxInt16(__int16 * d, int np);
__int16 MinInt16(__int16 * d, int np);
__int16 AbsMaxInt16(__int16 * d, int np);

void CopyInt16(__int16 * from, __int16 * to, int N);


void WriteIntOnFile(int value, CFile & fp);
void WriteDoubleOnFile(double value, CFile & fp);
void WriteShortDoubleOnFile(double value, CFile & fp);
void WriteStringOnFile(CString s, CFile & fp);
void WriteCharOnFile(char c, CFile & fp);
void WriteEndOfLine(CFile & fp);

void ReadDoubleFromFile(double & value, CFile & fp);
void ReadEndOfLine(CFile & fp);
bool ReadDoubleFromASCIIFile(double & d, CFile & f , bool WithComma = false);
bool ReadDoubleFromASCIIString(int & pos, double & d, CString & c , bool WithComma);

bool ScanDoubleFromStringNoComma(CString & c, double & d);

bool ReadString(CString & c, FILE * fp);
bool ReadInt(int & i, FILE * f);
bool ReadDouble(double & d, FILE * f);


void FFT(double * inoutdata, UINT n, int isign);
// WARNING: inoutdata starts with index 1
// TAKEN FROM: NUMERICAL RECIPES IN C
// inoutdata[2*i+1] : realpart
// inoutdata[2*i+2] : imaginary part


void WaitOK();

void GetStopValue(CString question, int & stopvalue);

int WriteBytes(CFile * fp, unsigned __int64 & pos, unsigned __int64 size, void * buff);

int WriteBytesNoSeek(CFile * fp, unsigned __int64 size, void * buff);

int ReadBytes(CFile * fp, unsigned __int64 & pos, unsigned __int64 size, void * buff);

int ReadBytesNoSeek(CFile * fp, unsigned __int64 size, void * buff);

void SaveDouble(double value, CFile & file);

void SaveFloat(float value, CFile & file);

void SaveInt(int value, CFile & file);

void SaveBool(BOOL value, CFile & file);

void SaveString(CString value, CFile & file);
void SaveStringWithPos(unsigned __int32 & pos, CString s, CFile & fp);
void SaveStringWithPos(unsigned __int64 & pos, CString s, CFile & fp);

bool LoadDouble(double & value, CFile & file);
bool LoadFloat(float & value, CFile & file);

bool LoadInt(int & value, CFile & file);
bool LoadInt16(__int16 & value, CFile & file);

bool LoadBool(BOOL & value, CFile & file);

bool LoadString(CString & value, CFile & file);
bool LoadString(BOOL DoShow, CString & value, CFile & file);
bool LoadString(CString & value, CFile & file, int MaxLength);
bool LoadStringWithPos(unsigned __int32 & pose, CString & value, CFile & file);
bool LoadStringWithPos(unsigned __int64 & pose, CString & value, CFile & file);

void Randomize();
double Rand();
double RandLargerZero();
double GaussRand();

void dft(int npt, int inv, double * realin, double * imin,
							double * realout, double * imout);
void DoFilterDFT(double f, __int16 * in, __int16* out, int NData);
void DoFilterDFT(double f, __int32 * in, __int32* out, int NData);

bool ReadDouble(FILE * fp, double & x);

double loggamma(double x);

double beta (double x, double y);


double BetaDensity (double x, double a, double b);

double GaussDensity (double x, double sigma);

//double ConvolutedBeta (double x, double a, double b, double sigma, int N);

//double CalcFittedBeta(double xinpA, double * p);

void MakeBetaTrace(double a, double b, double ymin, double ymax, int N, double * buffer);

void FilterBetaTrace(double sigmanorm, double ymin, double ymax, double * bufin, double * bufout, int N);

double FitKd(double * c, double * data, double * fit, int ndata,
			 BOOL FitMax, double & KD, double & IMax, bool BlockType = true);

 // returns SD


bool ReadOneDoubleInLine(FILE * fp, double & x);
bool ReadTwoDoublesInLine(FILE * fp, double & x, double & y);
bool ReadThreeDoublesInLine(FILE * fp, double & x, double & y, double & z);

bool ReadOneDoubleInString(CString & c, double & x);
bool ReadOneIntInString(CString & c, int & x);
bool ReadTwoDoublesInString(CString & c, double & x, double & y);
bool ReadThreeDoublesInString(CString & c, double & x, double & y, double & z);
bool GetLineFromString(CString & c, CString & line);
bool GetLineFromStringWithPos(CString & c, CString & line, int & pos);


bool ReadLine(CFile & f, CString & c, int EstimatedLength = 81); // removes all eol chararcters!

void TimeToString(double UnixTime, CString & timestring);

bool DoFindFile(CString Name, CString dir, CString & CopyName, UINT & Length);
bool DoFindFile(CString Name, CString dir, CString & CopyName, unsigned __int64 & Length);


//void ChangeColorInBitMap(HDC hdcBM, LPBITMAP lpBM,
  //                                   COLORREF rgbOld, COLORREF rgbNew);

HBITMAP FAR PASCAL CopyBitmap(HBITMAP hbmSrc);
//CBitmap * CopyBitmap( CBitmap * hbmSrc );

void SwapBitmapColors (CBitmap * hBmSource, COLORREF cr1,
                                        COLORREF cr2, HPALETTE hPal);

//void ChangeBitmapColor (HDC hDC, CBitmap * hbmSrc, COLORREF rgbOld,
//                                   COLORREF rgbNew, HPALETTE hPal);
void ChangeBitmapColor (BITMAP * hbmSrc, COLORREF rgbOld,
                                   COLORREF rgbNew, HPALETTE hPal);
void ChangeBitmapColorDC(CDC *, LPBITMAP lpBM,
                                     COLORREF rgbOld, COLORREF rgbNew);

//void ChangeBitmapColorDC(HDC hdcBM, LPBITMAP lpBM,
//                                     COLORREF rgbOld, COLORREF rgbNew);


bool ReadLineFromBuffer(char * buffer, UINT & start, UINT sizebuffer, CString & c);
bool ReadLineFromString(CString & source, int & start, CString & c);

void DoubleToStringWithUnit(double d, CString unit, CString & result);

bool StringWithUnitToDouble(CString unit, CString string, double & d);

bool WriteSystemTimeOnFile(SYSTEMTIME * pt, CFile * fp);

bool ReadSystemTimeFromFile(SYSTEMTIME * pt, CFile * fp);

__int32 SwapInt32(__int32 i);
__int16 SwapInt16(__int16 i);

double SwapDouble(double x);

double SystemTimeDifferenceInSeconds(SYSTEMTIME & st1, SYSTEMTIME & st2);
// works only for time-differneces < 23.99 hours

void WriteLineOnBinaryFile(CString & value, CFile & file);
void WriteLineOnBinaryFile(std::string & value, std::ofstream & file);

void PutCStringInCharArray(CString & c, char * s, int lengtharray);

void Swapfloat (float *rr);

__int16 SwapInt(__int16 i);

__int32 SwapLong(__int32 i);

void SwapInts(__int16 * ip, int n);
void Swap32Ints(__int32 * ip, int n);
double Norm(double * vect);
void Normalize(double * vect);

void DoRot(double ** RotaMat, double * vect);

void CopyVect(double * from, double * to);
void AddVect(double *a, double *b, double *c);
void ScalarMult(double a, double *b, double *c); // c=a*b

void SubVect(double *a, double *b, double *c); // c=a-b

double DotProduct(double *a, double * b);
void CrossProduct(double *a, double * b, double * c);
double findAngle(double * u, double *v);
/*
    Calculates the angle (degrees) between two vectors (as 1-d arrays) using
    dot product.
*/

double CrossDotProduct(double * a, double * b, double * c); // (axb).c

void MakeRotaMat(double angle, double * PolarVect, double ** RotaMat);

bool CheckSigns(double * InitialParams, double * Params, int NPARAMS);

bool PermutationsAreEquivalent(int Length, int * p1, int * p2);
// e.g. (1,2,3) is equivalent to (2,3,1) and to (3,1,2)

bool PermutationsAreReverseEquivalent(int Length, int * p1, int * p2);
// e.g. (1,2,3) is reverse equivalent to (3,2,1) and to (2,1,3)

int	* GetSequenceofNumbers(CString & c, int & length);

char *  LoadDIBitmap(CFile & file, /* I - File to load */
             BITMAPINFO & info);    /* O - Bitmap information */
	// return bitmap data


bool SaveDIBitmap(CFile  & file, /* I - File to load */
					BITMAPINFO & info,     /* I - Bitmap information */
					char     *bits);    /* I - Bitmap data */


bool SimpleGaussSeidel(int n, double ** A, double * b,
					   double * x, int Maxiter, double eps);
/* Solves the equation Ax = b using the iterative Gauss-Seidel method

  ASSUMING THAT ALL DIAGONAL ELEMENTS OF A ARE NONZERO 

  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS OF A IS ZERO

  n is the dimension of the matrix A assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm

  */

bool SimpleSOR(int n, double ** A, double * b,
					   double * x, int Maxiter, double eps, double omega);
/* Solves the equation Ax = b using the iterative SOR method

  ASSUMING THAT ALL DIAGONAL ELEMENTS OF A ARE NONZERO 

  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS OF A IS ZERO

  n is the dimension of the matrix A assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm

  */


bool ProbabilityGaussSeidel(int n, double ** A, double * b,
					   double * x, int Maxiter, double eps);
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

bool sprsin(double ** A, int n, double thresh, int nmax, double * sa, int * ija);

/* Converts a square matrix A[0,...,n-1][0, ..., n-1] into row-indexes 
   sparse storage mode. Only elements with magnitude > thresh are retained. Thus 0 is ok as thresh!
   Output is in two linear arrays with dimension nmax. Procedure returns FALSE if
   not enough space was allocated. 
   sa[0, ...] contains array values, indexed by ija[0,...].
*/

bool sprsTRANSPOSEin(double ** A, int n, double thresh, int nmax, double * sa, int * ija);

/* Converts a square matrix A[0,...,n-1][0, ..., n-1] into row-indexes 
   sparse storage mode and first transposes A. Only elements with magnitude > thresh are retained. Thus 0 is ok as thresh!
   Output is in two linear arrays with dimension nmax. Procedure returns FALSE if
   not enough space was allocated. 
   sa[0, ...] contains array values, indexed by ija[0,...].
*/

bool sprsax(double * sa, int * ija, double * x, double * b, int n);
/* Multiply a matrix in row-index sparse storage, stored in sa and ija, by a vector
   x[0,...] giving b[0,...]
   n is the dimension of the arrays/matrix
*/



bool ProbabilitySparseGaussSeidel(int n, double *sa, int * ija, double * b,
					   double * x,  double * rsforcheck,
					   int Maxiter, double eps);

/* Solves the equation Ax = b using the iterative Gauss-Seidel method
	where A is in row-indexed   sparse storage mode in vectors sa and ija

   if b==NULL assumes b[i]=0 for all i

   rsforcheck is an array to be allocated by the user for temporary use for calculation of
   the norm,

  ASSUMES that all xi are >=0 and that sum xi = 1.
  Normalizes the xi on each iteration such.

  ASSUMING THAT ALL DIAGONAL ELEMENTS ARE NONZERO 

  RETURNS FALSE IF ONE OF THE DIAGONAL ELEMENTS IS ZERO

  n is the dimension of the non-sparse matrix assumed to go from 0 ... n-1

  iteration stops if Maxiter is exceeded or if ||Ax-b||<eps where ||.|| is the
  maximum norm

  */

double GetSparseij(int i, int j, double * rsa, int * isa);
//Get the value of a sparse matrix

double MaxNorm(int n, double * x);
// Calculate the maximum norm of vector x with dimension n: x=(x[0], ..., x[n-1])

double MaxNorm(int n, double * x, double * b);
// Calculate the maximum norm of the difference of
//   vectors x and b, each with dimension n: x=(x[0], ..., x[n-1])
// b can be NULL. In this case the norm of x ist calculated.

/*
void IntToString(__int16 i, char * s);
void IntToString(__int32 i, char * s);
void IntToString(unsigned __int16 i, char * s);
void IntToString(unsigned __int32 i, char * s);
*/
void IntToString(int i, char * s);

void IntToString(__int64 i, char * s);
void IntToString(unsigned __int64 i, char * s);


void tridiag(double a[], double b[], double c[], double r[], double u[], int n);

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
void tridiag2(double a[], double b[], double c[], double r[], double u[], int n, double gam[]);
	// in this version the working vector gam is povided by user

void tridiag2(vector<double> & a, vector<double> & b, vector<double> & c, vector<double> & r,
			  vector<double> & u, int n, vector<double> & gam);
	// in this version the working vector gam is povided by user


#define SYMBOL_CIRCLE	0
#define SYMBOL_SQUARE	1
#define SYMBOL_TRIANGLE	2
#define SYMBOL_CROSS	3
#define SYMBOL_FILLED_CIRCLE 4

void DrawSymbol(int Symbol, int x, int y, int size, CDC * pD);

//double LogGamma(double x);
double RealLogBinom(double N, double k);

__int64 Binom(int N, int k);

void AddDoubleToString(BOOL & first, double d, CString & c);
void AddIntToString(BOOL & first, int d, CString & c);

void ConvertDoubleToFixPoint(double d , CString & c);

CString IncrementOrDecrementDirName(CString name, bool Increment);

CString IncrementFileName(CString name);
CString DecrementFileName(CString name);

CString CopyCString(CString * pc);

int Calci2(int i1, double prod);
// special function used in Export binary to get the next index

void CopyTextToClipboard(CString & source);
void GetTextFromClipboard(CString & source);

void CalcBesselPolynomialCoefficients(int order, double * coeffs);
void CalcReverseBesselPolynomialCoefficients(int order, double * coeffs);
void CalcPowerSpectrumBesselCoeffs(int order, double * coeffs);

///  Derivative - From nrc
double dfridr(double (*func)(double), double x, double h, double *err);


template <class T>
class BufferClass {
	public:
		T * buff;
		BufferClass();
		~BufferClass();
		int SizeBuffer();
		void UpdateBuffer(int newsize, bool copy = true);
		int GetSize();
		BufferClass &operator=( BufferClass & );  // Right side is the argument.

	private:
		int BufferSize;
		int FullBufferSize;
		void Dealloc();
};

template <class T>
BufferClass<T>::BufferClass(){
	buff = NULL;
	BufferSize = FullBufferSize = 0;
};

template <class T>
BufferClass<T>::~BufferClass(){
	Dealloc();
};

template <class T>
void BufferClass<T>::Dealloc(){
	if (buff){
		delete [] buff;
	}
	buff = NULL;
};

template <class T>
int BufferClass<T>::GetSize(){
	return BufferSize;
}

template <class T>
void BufferClass<T>::UpdateBuffer(int newsize, bool copy){
	if (newsize<=FullBufferSize){
		BufferSize = newsize;
		return;
	}
	T * newbuffer = new T[newsize];

	if (copy){
		for (int i=0; i<BufferSize; i++){
			newbuffer[i] = buff[i];
		}
	}
	Dealloc();
	buff = newbuffer;
	BufferSize = newsize;
	FullBufferSize = newsize;
};

template <class T>
BufferClass<T> &BufferClass<T>::operator=( BufferClass &ptRHS ){
	UpdateBuffer(ptRHS.GetSize());
	if (BufferSize>0){
		for (int i=0; i<BufferSize; i++){
			buff[i] = ptRHS.buff[i];
		}
	}
	return *this;
}


template <class T>
void Order(T * data, int NData, bool LargestFirst){
	bool ordered;

	do{
		ordered = true;
		for (int i=0; i<(NData-1); i++) {
			bool exchange = false;
			if (LargestFirst){
				if (data[i] < data[i + 1]){
					exchange = true;
				}
			}
			else{
				if (data[i] > data[i + 1]){
					exchange = true;
				}
			}
			if (exchange){
//			if (data[i] > data[i + 1]){
				ordered = false;
				T save = data[i];
				data[i] = data[i + 1];
				data[i + 1] = save;
			}
		}
	}
	while (!ordered);
}

template <class T>
void Order(vector<T> & data, int NData, bool LargestFirst){
	bool ordered;

	do{
		ordered = true;
		for (int i=0; i<(NData-1); i++) {
			bool exchange = false;
			if (LargestFirst){
				if (data[i] < data[i + 1]){
					exchange = true;
				}
			}
			else{
				if (data[i] > data[i + 1]){
					exchange = true;
				}
			}
			if (exchange){
//			if (data[i] > data[i + 1]){
				ordered = false;
				T save = data[i];
				data[i] = data[i + 1];
				data[i + 1] = save;
			}
		}
	}
	while (!ordered);
}


void GetFileName(CString FullNameWithPath, CString & FileName);
void GetPath(CString FullNameWithPath, CString & Path);

bool DirOrFileNamesAreEquivalent(CString f1, CString f2);
// Deals only with upper/lower case and slash/backslash -> not very reliable

bool FileExists(CString Name);

UINT DoCheckMenuItem(bool DoCheck, CMenu * pMenu, int ID);



template <class T>
int DoFilter(bool Causal, double f, T * in, T* out, int NData, int & NCoeffs,
			  int MaxNCoeffs= MaxCoeffInDoFilter, double * coeffs = NULL, bool GetOnlyNCoeff = false){
//void DoFilter(double f, __int16 * in, __int16* out, int NData){
  MaxNCoeffs--;
  double SI = 0.1325 / f;
//  #define MaxCoeffInDoFilter 1000
  double CCoeffs[MaxCoeffInDoFilter+2];
  double XXin[2*MaxCoeffInDoFilter+5];

  double * Coeffs;
  double * Xin;
  bool Alloc = false;
//  ShowFloat(MaxNCoeffs, "MaxNCoeffs");

  if (!GetOnlyNCoeff){
	  if (MaxNCoeffs<=MaxCoeffInDoFilter){
			  Coeffs = &CCoeffs[0];
			  Xin = &XXin[0];
	  }
	  else{
		  Alloc = true;
		  Coeffs = new double [MaxNCoeffs+2];
		  Xin = new double [2*MaxNCoeffs+5];
	  }
  }

  int NC;
  double SM, tp1, T0, T1, Sum, tp2;
  int i, k, j, ForMod, l, l1, i1, i2;
  T value;

  if (SI<0.61){
  	  NC=1;
	  if (GetOnlyNCoeff) return NC;
	  Coeffs[1] = SI*SI/2.0;
	  Coeffs[0] = 1.0 - 2.0 * Coeffs[1];

  }
  else{
	  NC = int(4.0*SI);
	  if (NC>MaxNCoeffs){
		  NC = MaxNCoeffs;
	  }
	  if (GetOnlyNCoeff) return NC;
	  T0 = -1.0 / (2.0 * SI * SI);
	  Coeffs[0] = 1.0;
      SM = 0.5;
      for (i = 1; i<=NC; i++){
		 tp1 = i;
         T1 = exp(tp1 * tp1 * T0);
         Coeffs[i] = T1;
         SM += T1;
	  }

	  SM *= 2.0;
      for (i= 0; i<=NC; i++){
		  Coeffs[i] /= SM;
	  }
  }

  NCoeffs =NC+1;
  if (Causal){
      for (i= 1; i<=NC; i++){
		  Coeffs[i] *= 2;
	  }
  }

  //ShowFloat(NC, "NC");
  if (coeffs){
      for (i= 0; i<=NC; i++){
		  coeffs[i] = Coeffs[i];
	  }
  }


  k = 0;
  l1=0;

  ForMod = 2 * NC + 1;
  for (i = 0; i<= 2 * MaxCoeffInDoFilter + 1; i++){
      Xin[i] = 0.0;
  }

  int NMax = NC;
  if (NMax>NData) NMax = NData;
  for (i = 0; i<NMax; i++){
	  Xin[i]=in[i];
  }
//  Position& = Position& + 2

  j = 0;

  for (i = NMax; i<NData; i++){
      l = (j + NC) % ForMod;
	  Xin[l]=in[i];
      Sum = Xin[j];
      Sum *= Coeffs[0];
	  for (l= 1; l<=NC; l++){
		  i1 = (j + l) % ForMod;
          i2 = (j - l + ForMod) % ForMod;
          tp1 = Xin[i1];
          tp2 = Xin[i2];
		  if (Causal){
	          Sum += Coeffs[l] * ( tp2);
		  }
		  else{
	          Sum += Coeffs[l] * (tp1 + tp2);
		  }


		  }
      k = 0;
      value = T(Sum);
	  out[l1]=T(Sum);
	  l1++;
      j++;
      j = j % ForMod;
  }

  for (i = 1; i<=NC; i++){
//      REM l% = (j% + NC%) MOD ForMod%
//      REM GET #File%, Position&, Xin%(l%)
//      REM Position& = Position& + 2
      l = (j - NC + ForMod) % ForMod;
      Xin[l] = 0.0;
	  k++;
      Sum = Coeffs[0]* Xin[j];
      for (l = 1; l<= NC; l++){
        i1 = (j + l) % ForMod;
        i2 = (j - l + ForMod) % ForMod;
        tp1 = Xin[i1];
        tp2 = Xin[i2];
		  if (Causal){
		     Sum += Coeffs[l] * ( tp2);
		  }
		  else{
		     Sum += Coeffs[l] * (tp1 + tp2);
		  }
	  }
      k = 0;
	  value = T(Sum);
	  if (l1<NData) out[l1]=value;
//	  out[l1]=value;
	  l1++;
      j = (j + 1) % ForMod;
  }
  if (Alloc){
	  delete [] Coeffs;
	  delete [] Xin;
  }
  return NC;
}


template <class T>
int DoFilter(double f, T * in, T* out, int NData, bool GetOnlyNCoeff = false){
	int NCoeffs;
	return DoFilter(false, f, in, out, NData, NCoeffs, MaxCoeffInDoFilter, NULL, GetOnlyNCoeff);
}

template <class T>
int DoFilterSubtractMean(double f, T * in, T* out, int NData){
//	int NCoeffs;
	double mean = 0.0;
	for (int i=0; i<NData; i++){
		mean += double(in[i]);
	}
	mean = mean/double(NData);
	T meanT = T (mean);
	for (int i=0; i<NData; i++){
		out[i] = in[i] - meanT;
	}

	int result = DoFilter(f, out, out, NData);

	for (int i=0; i<NData; i++){
		out[i] += meanT;
	}
	return result;
}


template <class T>
int DoFilter(double f, T * in, T * out, int NData, int & NCoeffs,
			  int MaxNCoeffs= MaxCoeffInDoFilter, double * coeffs = NULL, bool GetOnlyNCoeff = false){
	return DoFilter(false, f, in, out, NData, NCoeffs, MaxNCoeffs, coeffs, GetOnlyNCoeff);
}

template <class T>
void FilterBessel(double f, T * in, T * out, int NData, int BesselOrder, double * coeffs = NULL){

  double CCoeffs[MaxCoeffInDoFilter+2];
  double XXin[2*MaxCoeffInDoFilter+5];

  if (NData<=BesselOrder){
	  return;
  }
//ShowFloat(f, "f");
  double * Coeffs;
  double * Xin;
  bool Alloc = false;

  if (BesselOrder<MaxCoeffInDoFilter){
	  Coeffs = &CCoeffs[0];
	  Xin = &XXin[0];
  }
  else{
	  Alloc = true;
	  Coeffs = new double [BesselOrder+2];
	  Xin = new double [2*BesselOrder+5];
  }

// CalcBesselPolynomialCoefficients(BesselOrder, Coeffs);
  CalcReverseBesselPolynomialCoefficients(BesselOrder, Coeffs);

  int i, j;

  for (i=0; i<=BesselOrder; i++){
//	  ShowFloat(Coeffs[i], _T("i"));
//	0  Coeffs[i] /= Coeffs[0]; 
 }

  double z=1;
  for (i=0; i<=BesselOrder; i++){
	  Xin[i] = double(in[i]);
	  out[i]=in[i];
	  Coeffs[i] *= z;
	  z *= f;
  }
  for (i=0; i<=BesselOrder; i++){
//	  ShowFloat(Coeffs[i], _T("i"));
//	  Coeffs[i] /= Coeffs[0]; 
 }

  int l=BesselOrder;

  int ToMod = BesselOrder+1;

  for (i=(BesselOrder+1); i<NData; i++){

	  double sum = in[(l-3) % ToMod];

	  sum *= Coeffs[BesselOrder];

	  for (j=1; j<=BesselOrder; j++){
		  int k=(l-j) % ToMod;
		  double yk=Xin[k]*Coeffs[j];
		  sum -= yk;
	  }
	  Xin[l] = sum;
	  out[i] = T(sum);
	  l++;
	  l = l % ToMod;
  }

  if (Alloc){
	  delete [] Coeffs;
	  delete [] Xin;
  }

}

template <class T>
void FilterRC(double RC, T * in, T * out, int NData){
	// RC is in units of the sample time
	double alpha = 1.0/(1.0+RC);   // var real alpha := dt / (RC + dt)
	double alpha1 = 1.0-alpha;
	if (NData<1) return;
	out[0]=in[0];
	for (int i=1; i<NData; i++){
		out[i]=  T( alpha * double(in[i]) + alpha1*double(out[i-1])    );
	}
}


double Xsqrt(double x);// double x;
 /* because of deficiencies in hypot on Sparc, it's possible for arg of Xsqrt to be small and -ve,
       which logically it can't be (since r >= |x.re|).	 Take it as 0. */


bool TestSettingsParameter(double Test, double Min, double Max, CString Text);
bool TestSettingsParameter(int Test, int Min, int Max, CString Text);

class ReferenceCountForSmartPointers
{
    private:
    int count; // Reference count

    public:
    void AddRef()
    {
        // Increment the reference count
        count++;
    }

    int Release()
    {
        // Decrement the reference count and
        // return the reference count.
        return --count;
    }
};

template < typename T > class S_P
{
private:
    T*    pData;       // pointer
    ReferenceCountForSmartPointers* reference; // Reference count

public:
    S_P() : pData(0), reference(0) 
    {
        // Create a new reference 
        reference = new ReferenceCountForSmartPointers();
        // Increment the reference count
        reference->AddRef();
    }

    S_P(T* pValue) : pData(pValue), reference(0)
    {
        // Create a new reference 
        reference = new ReferenceCountForSmartPointers();
        // Increment the reference count
        reference->AddRef();
    }

    S_P(const S_P<T>& sp) : pData(sp.pData), reference(sp.reference)
    {
        // Copy constructor
        // Copy the data and reference pointer
        // and increment the reference count
        reference->AddRef();
    }

    ~S_P()
    {
        // Destructor
        // Decrement the reference count
        // if reference become zero delete the data
        if(reference->Release() == 0)
        {
            delete pData;
            delete reference;
        }
    }

    T& operator* ()
    {
        return *pData;
    }

    T* operator-> ()
    {
        return pData;
    }
    
    S_P<T>& operator = (const S_P<T>& sp)
    {
        // Assignment operator
        if (this != &sp) // Avoid self assignment
        {
            // Decrement the old reference count
            // if reference become zero delete the old data
            if(reference->Release() == 0)
            {
                delete pData;
                delete reference;
            }

            // Copy the data and reference pointer
            // and increment the reference count
            pData = sp.pData;
            reference = sp.reference;
            reference->AddRef();
        }
        return *this;
    }
};

/* USAGE */
/*
void main()
{
    S_P<PERSON> p(new Person("Scott", 25));
    p->Display();
    {
        S_P<PERSON> q = p;
        q->Display();
        // Destructor of q will be called here..

        S_P<PERSON> r;
        r = p;
        r->Display();
        // Destructor of r will be called here..
    }
    p->Display();
    // Destructor of p will be called here 
    // and person pointer will be deleted
}
*/
//#endif


bool GetNextNonEmptyString(int & index, int NStrings, CString * stringlist);
bool GetNextNonEmptyString(int & index, vector <CString> & stringlist);

void MyEnhancedPolyLine(POINT * points, int N, CDC *pDC);

template <class T>

void SavitzkyGolayDerivative(vector<T> & input, vector<T> & output){
	int n=input.size();
	double coeffs[9];
	coeffs[0]=86; 	coeffs[1]=-142; 	coeffs[2]=-193; 	coeffs[3]=-126;
	coeffs[4]=0;	coeffs[5]=126;  	coeffs[6]=193;  	coeffs[7]=142; 	coeffs[8]=-86;
	for (int i=0; i<9; i++){
		coeffs[i] /= 1188.0;
	}
	double circ[9];
	for (int i=0; i<9; i++){
		circ[i]=0.0;
	}
	for (int i=0; i<5; i++){
		if (i<n){
			circ[i+4] = double(input[i]);
		}
	}
	for (int i=0; i<n; i++){
		output[i]=0;
		for (int j=0; j<9; j++){
			output[i] += T(circ[j]*coeffs[j]);
		}
		for (int j=0; j<8; j++){
			circ[j] = circ[j+1];
		}
		if ((i+4)<n){
			circ[8] = double(input[i+4]);
		}
		else{
			circ[8] = 0;
		}
	}

}

inline double sqr(double x) { return x*x; }
inline bool seq(const char *s1, const char *s2) { return strcmp(s1,s2) == 0; }
inline bool onebit(uint m) { return (m != 0) && ((m & (m-1)) == 0); }

inline double asinh(double x)
{	// Microsoft C++ does not define
	return log(x + sqrt(1.0 + sqr(x)));
}

inline double fix(double x)
{	// nearest integer
	return (x >= 0.0) ? floor(0.5+x) : -floor(0.5-x);
}

void MakeButton(CButton * & b, CString title, CWnd * pW, UINT ID);

void extractIntegerWords(std::string & str, vector <int> & numbers);

// s is altered
bool GetNextWordInLine(CString & s, CString & w);