#ifndef _MIK_HIDDEN_MARKOV

#define _MIK_HIDDEN_MARKOV


#include "../common/utils.h"
//#include "../StateEditor/Model.h"
#include <math.h>
#include <windows.h>
#include <stdio.h>

#include "..\common\nrutil.h"
#include "..\common\simplex.h"
#include "../common/FunctionParser.h"
#include "../common/SSMarkov.h"
#include "../common/SparseMarkov.h"
#include "../common/Powell.h"

#include "../common/DefMarkov.h"

#include "../StateEditor/Model.h"

#define SIZELUTTABLE	65536
#define OFFSETLUTTABLE	32768

int MyPowHM(int A, int B);

//class Markov;
class Model;

class StateVector {
public:
	BufferClass <double> v;
	StateVector &operator=( StateVector & );  // Right side is the argument.
};

typedef double LookupTable[SIZELUTTABLE];

template <class T>
class HiddenMarkov{
public:

	HiddenMarkov();
	~HiddenMarkov();
//	bool Init(Model * pointerM, double v, int NData, __int16 * data, double gain, double SampleTime, int NFilterCoeffs = 0,
//					 double * FilterCoeffs = NULL);
	bool Init(Model * pointerM, double v, int NData, T * data, double gain, double SampleTime, int NFilterCoeffs = 0,
					 double * FilterCoeffs = NULL);

//	bool Init(Model * pointerM, int NT, double * v, int * NData, __int16 ** data, double * gain, double SampleTime, int NFilterCoeffs = 0,
//					 double * FilterCoeffs = NULL);
	bool Init(Model * pointerM, int NT, double * v, int * NData, T ** data, double * gain, double SampleTime, int NFilterCoeffs = 0,
					 double * FilterCoeffs = NULL);

	bool CalcAlphaAndLogLikelihood(double & Likelihood, bool SaveAlpha = false);

	bool CalcAlphaAndLogLikelihood(int trace, double & Likelihood, bool SaveAlpha = false);

	bool Fit(int MaxIter, double StepSize, double & Likelihood, CDC *pDC = NULL);

//	bool Idealize(__int16 * ideal, double & Likelihood);
	bool Idealize(T * ideal, double & Likelihood);

//	bool Idealize(int trace, __int16 * ideal, double & Likelihood);
	bool Idealize(int trace, T * ideal, double & Likelihood);


private:
//	Markov * pm;
	Model * pm;

	bool initialized;
	bool AlphaAndLutAllocated;
	CString TempFile;
	bool MemoryError;
	bool FileCreated;
	double GetEmissionProb(int trace, int j, int Y);
	double SampleTime;
	double * gain;
	double * V;
	void WriteAlpha(CFile & file, double * a, int nalpha = 1);
	void ReadAlpha(int t, CFile * pfile, double * a, int nalpha = 1);
	CFile * pInFile;


//	__int16 ** Y; // the data 
	T ** Y; // the data 
	int * TTT;

	int MaxT;

	int NTraces;

	double * currents;
	double * sigmas;
	int rmid;
	double scale;

	int NS;

	bool UseFilter;
	int NCoeffs;
	double * COEFFS;
	int R, M;

	int ** StateArray;
	double * LargeCurrents;
	double * TempAlpha;
	double * TempBeta;
	double * TempRead;


	double ** LargeA;
	double ** SmallA;
	void CalcLargeA();
	void AddOne(int m, int row);



//	double * SIGMA;

//	BufferClass <StateVector> b;
//	BufferClass <StateVector> alpha;
//	BufferClass <StateVector> beta;
	double ** alpha;
//	double ** beta;
	LookupTable * LUT;
//	BufferClass <LookupTable> LUT;
	BufferClass <double> SIGMA;
	BufferClass <double> L;

//	StateVector mu;
//	StateVector sigma;

	void PutParamsInModel(double * Params);


	void DeAlloc();
	void Alloc();
	bool UpdateBuffers();
	void DellocAlphaBeta();
	int GetMaxIndex(int t, double *beta );
	void AllocTraceBuffers();
	void DeAllocTraceBuffers();



	static void PowellFitFunc(double * f, double * x, int * stuff);

	double FitWithPowell(int NPARAMS, double * Params, int MaxIter);

//	double InitialParams[NEWMAXPARAMS];
	double InitialParams[1000];
	int NPARAMS;

	int MEM_NS, MEM_T;

	int NTracesAllocated;
};

template <class T>
HiddenMarkov<T>::HiddenMarkov(){
	FileCreated = false;
	scale = 1.0/sqrt(2.0*PIGRECO);
	srand( (unsigned)time( NULL ) );
	currents = new double [MAXSTATESMODEL];
	sigmas = new double [MAXSTATESMODEL];
	pm = NULL;
	initialized = false;
	TTT = 0;
	SampleTime = 20e-6;
	gain = NULL;//1e-16;
	V = NULL;
	Y=NULL;
	TTT = NULL;
	LargeA = NULL;
	SmallA = NULL;
	alpha = NULL;
//	beta = NULL;
	LUT = NULL;
	MEM_NS=MEM_T = 0;
	NTracesAllocated = 0;

	NTraces = 0;

	UseFilter = false;
	NCoeffs = 0;
	COEFFS = NULL;

	StateArray = NULL;
	LargeCurrents = NULL;
	TempAlpha= NULL;
	TempBeta= NULL;
	TempRead = NULL;

	char s[1000];
	DWORD temp = GetEnvironmentVariable("TEMP", s, 1000);
	CString Title = "HiddenMarkov";


	TempFile = CString(s) + "/" + Title;
	_itoa(rand(), s,10);
	TempFile += s;
//	Alert0(TempFile);

}


template <class T>
HiddenMarkov<T>::~HiddenMarkov(){
	DeAlloc();
	DellocAlphaBeta();
	delete [] COEFFS;
	delete [] sigmas;
	delete [] currents;
	DeAllocTraceBuffers();

	if (FileCreated){
		remove(TempFile);
	}
}
template <class T>
void HiddenMarkov<T>::DeAllocTraceBuffers(){
	if (Y){
		delete [] Y;
	}
	if (V){
		delete [] V;
	}
	if (gain){
		delete [] gain;
	}
	if (TTT){
		delete [] TTT;
	}
	TTT = NULL;
	Y=NULL;
	V= NULL;
	gain = NULL;
}

template <class T>
void HiddenMarkov<T>::AllocTraceBuffers(){
	if (NTraces<=NTracesAllocated){
		return;
	}
	DeAllocTraceBuffers();
	NTracesAllocated = NTraces;
	TTT = new int [NTraces];
	Y = new T*[NTraces];
	V = new double [NTraces];
	gain = new double [NTraces];
}

template <class T>
bool HiddenMarkov<T>::Init(Model * pointerM, double v, int ArgNData, T * Argdata,
						   double Arggain, double ArgSampleTime,
						int NFilterCoeffs, double * FilterCoeffs){
	if (!pointerM){
		return false;
	}
	if (ArgNData<1){
		return false;
	}
	if (!Argdata){
		return false;
	}
	if (pointerM->NStates()<1) return false;

	pm = pointerM;

	NTraces = 1;

	AllocTraceBuffers();

	TTT[0] = ArgNData;
	MaxT = TTT[0];
	Y[0] = Argdata;
	gain[0] = Arggain;
	SampleTime = ArgSampleTime;
	V[0]=v;

	if (COEFFS) delete [] COEFFS;
	COEFFS = NULL;

	if (NFilterCoeffs<1){
		NCoeffs = 0;
		UseFilter=false;
	}
	else{
		NCoeffs = NFilterCoeffs;
		UseFilter = true;
		COEFFS = new double[NCoeffs];
		for (int i=0; i<NCoeffs; i++){
			COEFFS[i] = FilterCoeffs[i];
		}
	}


	initialized = true;

	if (!UpdateBuffers()){
		initialized = false;
		return false;
	}

	return true;
}

//bool HiddenMarkov::Init(Model * pointerM, double v, int ArgNData, __int16 * Argdata, double Arggain, double ArgSampleTime,
//							int NFilterCoeffs, double * FilterCoeffs){

template <class T>
bool HiddenMarkov<T>::Init(Model * pointerM, int NT, double * v, int * NData, T ** data, double * g,
						double ArgSampleTime, int NFilterCoeffs, double * FilterCoeffs){

	if (!pointerM){
		Alert0("!pointerM");
		return false;
	}
	if (NData==NULL){
		Alert0("NData==NULL");
		return false;
	}
	if (NT<1){
		Alert0("NT<1");
		return false;
	}
	int i;
	for (i=0; i<NT; i++){
		if (NData[i]<1){
			Alert0("NData[i]<1");
			return false;
		}
	}

	if (data == NULL){
		Alert0("data == NULL");
		return false;
	}
	for (i=0; i<NT; i++){
		if (data[i]==NULL){
			Alert0("data[i]==NULL");
			return false;
		}
	}

	if (g == NULL){
		Alert0("gain == NULL");
		return false;
	}
	
	if (pointerM->NStates()<1){

		Alert0("pointerM->NStates()<1");
		return false;
	}

	pm = pointerM;

	NTraces = NT;

	AllocTraceBuffers();

	int MaxT = 0;
	for (i=0; i<NT; i++){
		TTT[i] = NData[i];
		if (TTT[i]>MaxT){
			MaxT = TTT[i];
		}
		Y[i] = data[i];
		gain[i] = g[i];
		SampleTime = ArgSampleTime;
		V[i]=v[i];

		if (COEFFS) delete [] COEFFS;
		COEFFS = NULL;

		if (NFilterCoeffs<1){
			NCoeffs = 0;
			UseFilter=false;
		}
		else{
			NCoeffs = NFilterCoeffs;
			UseFilter = true;
			COEFFS = new double[NCoeffs];
			for (int ii=0; ii<NCoeffs; ii++){
				COEFFS[ii] = FilterCoeffs[ii];
			}
		}
	}


	initialized = true;

	if (!UpdateBuffers()){
		initialized = false;
		return false;
	}

	return true;
}

template <class T>
void HiddenMarkov<T>::DellocAlphaBeta(){
	if (alpha){
		free_dmatrix(alpha, 0, MEM_NS-1, 0, MEM_T-1);
	}
	alpha = NULL;
/*
	if (beta){
		free_dmatrix(beta, 0, MEM_NS-1, 0, MEM_T-1);
	}
	beta = NULL;
*/
	if (LUT){
		delete [] LUT;
	}
	LUT = NULL;

	if (LargeCurrents){
		delete [] LargeCurrents;
	}
	LargeCurrents = NULL;

	if (TempAlpha){
		delete [] TempAlpha;
	}
	TempAlpha = NULL;
	if (TempBeta){
		delete [] TempBeta;
	}
	TempBeta = NULL;
	
	if (TempRead){
		delete [] TempRead;
	}
	TempRead = NULL;
}

template <class T>
void HiddenMarkov<T>::DeAlloc(){
	if (SmallA){
		free_dmatrix(SmallA, 0, NS-1, 0, NS-1);
	}
	SmallA = NULL;
	if (LargeA){
		free_dmatrix(LargeA, 0, M-1, 0, M-1);
	}
	LargeA = NULL;

	if (StateArray){
		free_imatrix(StateArray, 0, M-1, 0, R-1);
	}
	StateArray = NULL;

}

template <class T>
void HiddenMarkov<T>::Alloc(){
	SmallA = dmatrix(0, NS-1, 0, NS-1);
	LargeA = dmatrix(0, M-1, 0, M-1);
	StateArray = imatrix(0, M-1, 0, R-1);

	//	if ((NS>MEM_NS) || (T>MEM_T)){
	if ((M>MEM_NS) || (MaxT>MEM_T)){
		DellocAlphaBeta();
//		MEM_NS = NS;
		MEM_NS = M;
		MEM_T = MaxT;
		LargeCurrents = new double [MEM_NS];
		TempAlpha = new double [MEM_NS];
		TempBeta = new double [MEM_NS];
		TempRead = new double [MEM_NS];
		
		__int64 memtoalloc = __int64(MEM_NS)*__int64(MEM_T)*__int64(sizeof(double));


//		MEMORYSTATUSEX statex;
		MEMORYSTATUS statex;
		statex.dwLength = sizeof (statex);
//		GlobalMemoryStatusEx (&statex);
		GlobalMemoryStatus (&statex);
		MemoryError = false;
		__int64 Available = __int64(statex.dwTotalPhys);

		if (memtoalloc> (Available/3)){
//		if (memtoalloc> 200){
			Alert0("too much");
			ShowFloat(double(memtoalloc), "bytes");
			alpha = NULL;
			AlphaAndLutAllocated = false;
//			FileWritten = false;
			alpha = dmatrix(0, MEM_NS-1, 0, MEM_T-1);
		}
		else{
//			Alert0("ok ");
			alpha = dmatrix(0, MEM_NS-1, 0, MEM_T-1);
			AlphaAndLutAllocated = true;
			LUT = new LookupTable[MEM_NS];
		}
		if (memtoalloc > 4294967296){ // 4 G
			Alert0("ERROR : >4 GB requested: ERROR");
			MemoryError = true;
		}


//		beta = dmatrix(0, MEM_NS-1, 0, MEM_T-1);
	}
}


template <class T>
bool HiddenMarkov<T>::UpdateBuffers(){

	if (!initialized) return false;
	if (MaxT<1) return false;
	if (!pm) return false;
	int localNS = pm->NStates();
	if (localNS<1) return false;

	DeAlloc();
	NS = localNS;
	M = NS;
	R = 0;

	if (UseFilter){
		R = 2*(NCoeffs-1)+1;
//		M = pow(double(NS), double(R));
		M = MyPowHM(NS, R);
	}

	Alloc();

	SIGMA.UpdateBuffer(MaxT);
	L.UpdateBuffer(MaxT);

	return true;
}



template <class T>
void HiddenMarkov<T>::AddOne(int m, int row){

	StateArray[m][row]++;
	if (StateArray[m][row] >= NS){
		StateArray[m][row] = 0;
		AddOne(m, row+1);
	}
}

template <class T>
void HiddenMarkov<T>::CalcLargeA(){
	/*
		StateArray = imatrix(0, M-1, 0, R-1);
	000
	100
	010
	*/

	int m, n, r;

	for (r=0; r<R; r++){
		StateArray[0][r]=0;
	}

	for (m=1; m<M; m++){
		for (r=0; r<R; r++){
			StateArray[m][r] = StateArray[m-1][r];
		}
		AddOne(m, 0);

	}

	for (m=0; m<M; m++){
		for (n=0; n<M; n++){
			bool Connected = true;
			for (r=0; r<(R-1); r++){
				if (StateArray[m][r+1] != StateArray[n][r]){
					Connected = false;
				}
			}
			if (Connected){
				int from = StateArray[m][R-1];
				int to = StateArray[n][R-1];
				LargeA[m][n] = SmallA[from][to];
			}
			else{
				LargeA[m][n] = 0.0;
			}
/*
			CString c = "from ";
			for (r=0; r<R; r++){
					char s[20];
					_itoa(StateArray[m][r], s, 10);
					c += CString(s);
			}
			c += " to ";
			for (r=0; r<R; r++){
					char s[20];
					_itoa(StateArray[n][r], s, 10);
					c += CString(s);
			}
			char s[20];
			_gcvt(LargeA[m][n], 10, s);
			c += ": " + CString(s);
			Alert0(c);
*/
		}
	}



}

template <class T>
double HiddenMarkov<T>::GetEmissionProb(int trace, int m, int Y){

	if (AlphaAndLutAllocated){
		return LUT[m][Y];
	}


	double ss;

	if (UseFilter){
		ss = sigmas[StateArray[m][rmid]];
	}
	else{
		ss = sigmas[m];
	}

	double mm=LargeCurrents[m];
	double tp3 = -1.0/ss/ss/2.0;
	double tp4 = scale/ss;
	int jj=Y-OFFSETLUTTABLE;
	double tp1=double(jj)*gain[trace] - mm;
	return tp4 * exp(tp1*tp1*tp3);
}

template <class T>
void HiddenMarkov<T>::WriteAlpha(CFile & file, double * a, int nalpha){
	file.Write(a, nalpha*M*sizeof(double));
}

template <class T>
void HiddenMarkov<T>::ReadAlpha(int t, CFile * file, double * a, int nalpha){
	int ss=M*sizeof(double);
	file->Seek(t*ss, CFile::begin);
	file->Read(a, nalpha*ss);
}

template <class T>
bool HiddenMarkov<T>::CalcAlphaAndLogLikelihood(double & Likelihood, bool SaveAlpha){
	double L=0;
	Likelihood = 0;
	int t;
	for (t=0; t<NTraces; t++){
		if (!CalcAlphaAndLogLikelihood(t, L, SaveAlpha)){
			return false;
		}
		Likelihood += L;
	}
	return true;

			;
}

template <class T>
bool HiddenMarkov<T>::CalcAlphaAndLogLikelihood(int trace, double & Likelihood, bool SaveAlpha){
	CFile OutFile;
	if (SaveAlpha){
		if (MemoryError) {
			Alert0("MEMORY ERROR IN CalcAlphaAndLogLikelihood");
			return false;
		}
		else{
			if (!AlphaAndLutAllocated){
				if (!OutFile.Open ( TempFile, CFile::modeCreate | CFile::modeWrite)){
					Alert0("Could not create temp file");
					return false;
				}
				FileCreated = true;
			}
		}
	}

	CWaitCursor dummy;

	if (!initialized) return false;

	int NS = pm->NStates();

	pm->Init(V[trace]);

	if (NS<2) return false;

	int i, j, t, m;

	pm->GetTransitionMatrix(SampleTime, SmallA);

	if (UseFilter){
		CalcLargeA();
	}
	else{
		for (i=0; i<M; i++){
			for (j=0; j<M; j++){
				LargeA[i][j] = SmallA[i][j];
			}
		}
	}

	for (i=0; i<NS; i++){
		State state;
		if (!pm->GetState(i, state)){
			Alert0("could not get state in HHM");
			return false;
		}
		currents[i] = state.current*1e-12;
		sigmas[i] = state.NewSigma*1e-12;
	}

	rmid = (R-1)/2;   // R=3; rmid = 1

	if (UseFilter){
		for (m=0; m<M; m++){
			double mm=0;
			for (int r=0; r<R; r++){
				int s = StateArray[m][r];
				double cr = currents[s];
				int icoeff = abs(rmid-r);
				mm += COEFFS[icoeff]*cr;
			}
			LargeCurrents[m] = mm;
		}
	}
	else{
		for (i=0; i<NS; i++){
			LargeCurrents[i] = currents[i];
		}
	}

	if (AlphaAndLutAllocated){
		double gg= gain[trace];
		if (UseFilter){
			for (m=0; m<M; m++){
				double mm=LargeCurrents[m];
				double * table = LUT[m];
				double ss = sigmas[StateArray[m][rmid]];
				double tp3 = -1.0/ss/ss/2.0;
				double tp4 = scale/ss;
				for (j=0; j<SIZELUTTABLE; j++){
					int jj=j-OFFSETLUTTABLE;
					double tp1=double(jj)*gg - mm;
					table[j]= tp4 * exp(tp1*tp1*tp3);
				}
			}
		}
		else{
			for (i=0; i<NS; i++){
				double * table = LUT[i];
				double mm = currents[i];
				double ss = sigmas[i];
				double tp3 = -1.0/ss/ss/2.0;
				double tp4 = scale/ss;
				for (j=0; j<SIZELUTTABLE; j++){
					int jj=j-OFFSETLUTTABLE;
					double tp1=double(jj)*gg - mm;	
					table[j]= tp4 * exp(tp1*tp1*tp3);
				}
			}
		}
	}

	double p0[MAXSTATESMODEL];
	double current;

//	pm->Init(f->p.vhold);
	pm->PinfAllStates(current, p0);

	int T1 = 0;
	int T2 = TTT[trace]-1;
/*
	if (UseFilter){
		T1 += (NCoeffs-1);
		T2 -= (NCoeffs-1);
	}
*/
	if (UseFilter){
		for (m=0; m<M; m++){
			TempBeta[m] = p0[StateArray[m][rmid]];
		}
	}
	else{
		for (i=0; i<NS; i++){
			TempBeta[i] = p0[i];
		}
	}
	if (SaveAlpha){
		if (AlphaAndLutAllocated){
			for (m=0; m<M; m++){
				alpha[m][T1] = TempBeta[m];
			}
		}
		else{
			WriteAlpha(OutFile, TempBeta);
		}
	}

	L.buff[0] = 0;

	Likelihood = 0;


	T * YY = Y[trace];
	for (t=T1; t<T2; t++){
//	for (t=0; t<(T-1); t++){
//		ShowFloat(Y[t]*gain, "Y");
		int t1=t+1;
		double & sigma = SIGMA.buff[t];
//		int index = abs(Y[t1]);
		int index = YY[t1];
		index += OFFSETLUTTABLE;
		if (index >= SIZELUTTABLE){
			index = SIZELUTTABLE-1;
		}
		double sumpj = 0;
		for (j=0; j<M; j++){
			double pj=0;
			for (i=0; i<M; i++){
				double b=GetEmissionProb(trace, j, index);
//				pj += alpha[i][t]*A[i][j]*LUT[j][index];
//				pj += alpha[i][t]*LargeA[j][i]*LUT[j][index];
//				pj += alpha[i][t]*LargeA[j][i]*b;
				pj += TempBeta[i]*LargeA[j][i]*b;
			}
			TempAlpha[j] = pj;
			sumpj += pj;
		}
		sigma = sumpj;
		L.buff[t1] = L.buff[t] + log(sigma);

		Likelihood += L.buff[t1];

		for (i=0; i<M; i++){
//			alpha[i][t1] = TempAlpha[i]/sigma;
			TempBeta[i] = TempAlpha[i]/sigma;
		}
		if (SaveAlpha){
			if (AlphaAndLutAllocated){
				for (m=0; m<M; m++){
					alpha[m][t1] = TempBeta[m];
				}
			}
			else{
				WriteAlpha(OutFile, TempBeta);
			}
		}
	}


//	Likelihood = L.buff[T-1];

	SIGMA.buff[TTT[trace]-1]=1;

	return true;
}

template <class T>
int HiddenMarkov<T>::GetMaxIndex(int t, double *beta ){
	int m;
	if (AlphaAndLutAllocated){
		for (m=0; m<M; m++){
			TempRead[m] = alpha[m][t];
		}
	}
	else{
		ReadAlpha(t, pInFile, TempRead);
	}


	int imax=0;
//	double vmax = beta[0]*alpha[0][t];
	double vmax = beta[0]*TempRead[0];
	for (m=1; m<M; m++){
//		double r=beta[m]*alpha[m][t];
		double r=beta[m]*TempRead[m];
		if (r>vmax){
			imax = m;
			vmax = r;
		}
	}
	return imax;
}

template <class T>
bool HiddenMarkov<T>::Idealize(T * ideal, double & Likelihood){
	double L;
	int t;
	Likelihood = 0;
	for (t=0; t<NTraces; t++){
		if (!Idealize(t, ideal, L)){
			return false;
		}
		Likelihood += L;
	}
	return true;
//	return Idealize(0, ideal, Likelihood);
}
//	double Likelihood;

template <class T>
bool HiddenMarkov<T>::Idealize(int trace, T * ideal, double & Likelihood){

	if (!CalcAlphaAndLogLikelihood(trace, Likelihood, true)){
		Alert0("!CalcAlphaAndLogLikelihood(trace, Likelihood, true)");
		return false;
	}
	CFile InFile;

	if (!AlphaAndLutAllocated){
		if (!InFile.Open(TempFile,  CFile::modeRead )){
			Alert0("could not open temp file");
			return false;
		}
		pInFile = &(InFile);
	}

	int T1 = 0;
	int T2 = TTT[trace]-1;
/*
	if (UseFilter){
		T1 += (NCoeffs-1);
		T2 -= (NCoeffs-1);
	}
*/
	int i, j, t, m;

	T currents[MAXSTATESMODEL];


	for (i=0; i<NS; i++){
		State state;
		if (!pm->GetState(i, state)){
			Alert0("could not get state in HHM");
			return false;
		}
		double mm = state.current*1e-12/gain[trace];
//		ShowFloat(mm, "mm");
		currents[i] = T(mm);
//		ShowFloat(currents[i] , "currents[i] ");
	}


	for (m=0; m<M; m++){
		TempBeta[m] = 1.0/double(M);
	}

	rmid = (R-1)/2;   // R=3; rmid = 1

	if (UseFilter){
		int stateindex = GetMaxIndex(T2, TempBeta);
		int midstate = StateArray[stateindex][rmid];
//		ShowFloat(midstate, "midstate");
		ideal[T2] = currents[midstate];
	}
	else{
		double cc = LargeCurrents[GetMaxIndex(T2, TempBeta)];
		ideal[T2] = T(cc/gain[trace]);
	}


//		ideal[T2] = currents[GetMaxIndex(T2, beta)];

/*
	if (UseFilter){
		for (int t=(T2+1); t<T; t++){
			ideal[t]= ideal[T2];
		}
	}
*/

	T * YY = Y[trace];

//	for (t=(T-2); t>=0; t--){
	for (t=(T2-1); t>=T1; t--){
//		int index = abs(Y[t+1]);
		int index = YY[t+1];
		index += OFFSETLUTTABLE;
		if (index >= SIZELUTTABLE){
			index = SIZELUTTABLE-1;
		}
		for (j=0; j<M; j++){
			double pj=0;
			for (i=0; i<M; i++){
				double b=GetEmissionProb(trace, j, index);
				pj += TempBeta[i]*LargeA[i][j]*b;
//				pj += TempBeta[i]*LargeA[i][j]*LUT[j][index];
//				pj += beta[i]*A[j][i]*LUT[j][index];
			}
			TempAlpha[j] = pj;
		}

		double sigma = SIGMA.buff[t];
		for (i=0; i<M; i++){
			TempBeta[i] = TempAlpha[i]/sigma;
		}
		if (UseFilter){
			int stateindex = GetMaxIndex(t, TempBeta);
			int midstate = StateArray[stateindex][rmid];
			ideal[t] = currents[midstate];
//			ShowFloat(midstate, "midstate");
		}
		else{
			double cc = LargeCurrents[GetMaxIndex(t, TempBeta)];
			ideal[t] = T(cc/gain[trace]);
		}



	}

/*
	if (UseFilter){
		for (t=0; t<T1; t++){
			ideal[t] = ideal[T1];
		}
	}
*/
	return true;

}

template <class T>
bool HiddenMarkov<T>::Fit(int MaxIter, double StepSize, double & LH, CDC *pDC){
	if (!initialized){
		Alert0("not init");
		return false;
	}
//	ShowFloat(this->NCoeffs, "NCoeffs");
	if (UseFilter){
//		Alert0("Use Fiulter");
	}
	else{
//		Alert0("not use filter");
	}

	double Params[NEWMAXPARAMS], BestParams[NEWMAXPARAMS];
//	double InitialParams[NEWMAXPARAMS],
	double InitialArray[NEWMAXPARAMS];
	
	double BestSD, SD; 

	int i;
	
	NPARAMS=0;

	for (i=0; i<NEWMAXPARAMS; i++){
		InitialArray[i] = pm->A[i];
		if (pm->DoFitParam[i]){
			InitialParams[NPARAMS] = pm->A[i];
			NPARAMS++;
		}
	}

//	ShowInt(NPARAMS, "NPARAMS");

	if (NPARAMS<1){
		Alert0("no param to fit");
		return false;
	}
	for (i=0; i<NPARAMS; i++){
		Params[i] = InitialParams[i];
		BestParams[i] = InitialParams[i];
	}
//	ShowFloat(Params[0], "p0");

	Simplex Simp;
	Simp.InitSimplex(StepSize, Params, NPARAMS);
	BestSD = 1E299;

	CWaitCursor dummy;

	int iter = 0;
	char numbstr[100];
	CString c;

	if (pDC) pDC->TextOut(10,10, CString("     "));

	bool SimplexDecreased = false;
	bool PowellDecreased = false;
	bool StepChanged = false;
	bool OnStepIncrease = false;
	double InitialStepSize = StepSize;

	do{
		int inkey = InKey();
		if ( inkey == ESCAPEKEY){
			Simp.CopyParams(BestParams, Params);
			break;
		}
		if (iter>MaxIter){
			iter = 0;
			Simp.CopyParams(BestParams, Params);

//			double pwsd = FitWithPowell(NPARAMS, Params, MaxIter);
			double pwsd = 1e200; //FitWithPowell(NPARAMS, Params, MaxIter);
			PowellDecreased = false;


//		if (pwsd<BestSD){
		if (false){
//				ShowFloat(fabs((pwsd-BestSD)/BestSD), " ");
				if (fabs((pwsd-BestSD)/BestSD) < 1e-15){
					PowellDecreased = true;
				}
				BestSD = pwsd;
				Simp.CopyParams(Params, BestParams);
				if (pDC){
					Beep(1000,10);
					_itoa(iter, numbstr, 10);
					c = CString(numbstr)+CString(" SD ");
					_gcvt(SD, 10, numbstr);
					c += CString(numbstr)+CString("  ESC to stop                   ");
					pDC->TextOut(10,10, c);
					_gcvt(Params[0], 10, numbstr);
					c = CString(numbstr)+CString(" p0 ");
					_gcvt(SD, 5, numbstr);
					pDC->TextOut(10,40, c);
				}
			}
			else{
				PowellDecreased = false;
//				Beep(100,100);
				Simp.CopyParams(BestParams, Params);
			}


			if ( (!SimplexDecreased) && (!PowellDecreased)){
//				Alert0("hi");
				StepSize *= 1.5;
				StepChanged = true;
				OnStepIncrease = true;
			}
			else{
				if (SimplexDecreased && OnStepIncrease){
					StepSize = InitialStepSize;
					OnStepIncrease = false;
				}
//				Alert0("1 decr");
			}
			Simp.InitSimplex(StepSize, Params, NPARAMS);
			SimplexDecreased = false;
			PowellDecreased = false;

//			BestSD = 1E299;
		}

		if (CheckSigns(InitialParams, Params, NPARAMS)){
			PutParamsInModel(Params);
//			double LH;
//			pm->Init(V[trace]);
			CalcAlphaAndLogLikelihood(LH);
			SD = - LH;
//			SD = LH;
		}
		else{
			SD = 1E300;
//			Alert0("check signs bad");
		}
		if ((SD<BestSD) || StepChanged){
			StepChanged = false;
			if (SD<BestSD) {
//				ShowFloat( fabs((SD-BestSD)/BestSD), "SD ratio");
				SimplexDecreased = true;
				BestSD = SD;
				Simp.CopyParams(Params, BestParams);
			}
			if (pDC){
				_itoa(iter, numbstr, 10);
				c = CString(numbstr)+CString(" SD ");
				_gcvt(SD, 10, numbstr);
				c += CString(numbstr)+CString("  ESC to stop                   ");
				pDC->TextOut(10,10, c);
				_gcvt(Params[0], 10, numbstr);
				c = CString(numbstr)+CString(" p0 ");
				_gcvt(StepSize, 5, numbstr);
				c += CString(numbstr)+CString(" StepSize ");
				pDC->TextOut(10,40, c);
			}
		}
		Simp.NextSimp(Params, SD);
//		ShowFloat(SD, "SD");
//		ShowFloat(Params[0], "p0");

		iter ++;
	}
	while (true);

	PutParamsInModel(BestParams);
	return true;
}
template <class T>
void HiddenMarkov<T>::PutParamsInModel(double * Params){
	int j, i;
	j=0;
	for (i=0; i<NEWMAXPARAMS; i++){
		if (pm->DoFitParam[i]){
			pm->A[i] = Params[j];
			j++;
		}
	}
}
template <class T>
void HiddenMarkov<T>::PowellFitFunc(double * SD, double * x, int * stuff){
	HiddenMarkov * pH = (HiddenMarkov *)(stuff);
	HiddenMarkov & hm = *pH;

	if (CheckSigns(hm.InitialParams, x, hm.NPARAMS)){
		hm.PutParamsInModel(x);
		double LH;
//		hm.pm->Init(hm.V);
		hm.CalcAlphaAndLogLikelihood(LH);
		*SD = - LH;
	}
	else{
		*SD = 1e300;
	}
}

template <class T>
double HiddenMarkov<T>::FitWithPowell(int NPARAMS, double * Params, int MaxIter){
	Powell pw;

	pw.Init(NPARAMS,  PowellFitFunc, (int *)(this));

	pw.ITMAX = MaxIter;

	double f;

	pw.MinimizeCost(Params, &f);

	return f;

}






#endif