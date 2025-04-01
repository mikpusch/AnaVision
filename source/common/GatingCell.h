#ifndef MIK_GATING_CELL
#define MIK_GATING_CELL


class GatingCellDataEntry{
public:
	GatingCellDataEntry();
	~GatingCellDataEntry();
	void DeAlloc();
	void Alloc();

	bool SaveOnFile(CFile * fp, bool WriteData = false);
	bool LoadFromFile(CFile * fp, bool ReadData = false);

	void PutParamsInArray(int & NParams, double * Params);
	void PutArrayInParams(int & NParams, double * Params);


	void CopyTo(GatingCellDataEntry & to);

	void GetMaxMinData(double & max, double & min);

	bool DataPresent;

	int IVType; //0: direct IV, 1: tail;
	double Cm, Rs, Rm;
	BOOL FitCm, FitRs, FitRm;
	BOOL PerformLeakSubtraction;
	int series, firstentry, NTraces, offsetindexfromstart, OffsetForFit, NPointsPerTrace;
	double ** Data;
	double ** Fit;
	double gaindata;
	double SD;
//	double VHold, VTest, tTest, DeltaV; 
	double VHold, VStart, DeltaV; 
	double dt;

	double VHoldP4, VTestP4;
};

class GatingCellOnAndOffData{
public:
	GatingCellOnAndOffData();
	~GatingCellOnAndOffData();
	bool SaveOnFile(CFile * fp);
	bool LoadFromFile(CFile * fp);
	void ExportDataToClibboard();

	GatingCellDataEntry Entries[2]; // 0: direct IV, 1:tail;

	double AbsMax;


};

class RungeKutta;
class Model;

class GatingCell{
public:
	GatingCell();
	~GatingCell();

	bool SaveOnFile(CFile * fp);
	bool LoadFromFile(CFile * fp);
	void ExportDataToClibboard();


	GatingCellOnAndOffData OnOffData;

	GatingCellDataEntry GeneralData;

	void Predict();
	void InitPrediction();
	bool Step();

	void PredictMarkov(Model * pm);
	void InitPredictionMarkov(Model * pm);
	static int * MarkovStuff[2];
//	bool StepMarkov(Model * pm);

	void Alloc();
	void Dealloc();
	void NullPointers();

	void CopyTo(GatingCell & to);

	//  Stimulation parameters

	double tTest, VTest;

	// params of transporter
	double Qmax; // in Coulomb
	BOOL FitQmax;

	///
	// alpha = alpha0*exp(v*zalpha/25)
	// beta =  beta0*exp(-v*zbeta/25)

	double alpha0, zalpha, beta0, zTotal, zbeta, zK, K;
	double zon, koff;
//	double RatioConcK;
	BOOL Fitalpha0, Fitzalpha, Fitbeta0, FitzTotal, FitzK, FitK;
	BOOL UseKVeryOld;
	int ModelTypeUseK;//, UseKalpha, UseKbeta; 
	// 0: NotUseK
	// 1: UseKalpha
	// 2: UseKbeta

	// 4: Scheme II
	// 5: scheme III

	double * time, * predictedV, * predictedI, *leakI; 
	UINT NPoints;
	UINT SizeBuffer;

	RungeKutta * prk;
	UINT istep;
	double y[200];

	double ka, kb, sumk, KA, CinFDiv1000, phialpha, phibeta, phikappa, INVCinFDiv1000;
	double phion, phioff, zoff;

	static void CalcDerivs(double t, double * y, double * yp, int * STUFF);
	void DoCalcDerivs(double t, double * y, double * yp);

	static void CalcDerivs3StateModel(double t, double * y, double * yp, int * STUFF);
	void DoCalcDerivs3StateModel(double t, double * y, double * yp);

	static void CalcDerivsMarkov(double t, double * y, double * yp, int * STUFF);
	void DoCalcDerivsMarkov(double t, double * y, double * yp, Model * pm);



	double CalcQ(double v);
	void CalcSteadyState3StateModel(double v, double * y);
	void CalcW(double v, double * w);

	double CalcAlpha(double v);
	double CalcBeta(double v);
	double CalcVmSteadyState(double v);



	// Fitting parameters for data


	int MaxIter;
	int NParams;
	double StepSize;
	double BestSD;
	double Params[100];
	double BestParams[100];
 
	int PutParamsInArray(double * Params);
	int PutArrayInParams(double * Params);

	void CalcFit();
	double CalcSD();

	int PutParamsInArrayIVAndTail(double * Params);
	int PutArrayInParamsIVAndTail(double * Params);

	void CalcFitIVAndTail();
	double CalcSDIVAndTail();
	

	void PerformFit(int IVType,
					int NTraces,
					int NPointsPerTrace,
					int Offset, // green cursor-blue cursor
					double SampleTimeInSeconds,
					double VHold,
					double VStart,
					double DeltaV,
					double ** Data,
					double ** Fit); // first, all the parameters have to be assigned!!
	void FitIVAndTail();


	int PutParamsInArrayIVAndTailMarkovModel(Model * pm, double * Params);
	int PutArrayInParamsIVAndTailMarkovModel(Model * pm, double * Params);
	void CalcFitIVAndTailMarkovModel(Model * pm);
	double CalcSDIVAndTailMarkovModel(Model * pm);
	
	void FitIVAndTailWithMarkovModel(Model * pm);

};


class CollectionOfGatingCells{
public:
	CollectionOfGatingCells();
	~CollectionOfGatingCells();

	void CopyTo(CollectionOfGatingCells & to);

	int ModelType; // 0 : Cl dependendnce in K
					// 1: Cl dependence in alpha (K not used)
					// 2: Cl dependence in beta (K not used)
					// 3: Cl dependence after charge displacement

	double BestSD, StepSize;
	int MaxIter;

	BOOL NormalizeEachFile;

	int NCells;

//	GatingCellOnAndOffData TheCells[1000];
	GatingCell TheCells[200];
	int IndexCl[200];// The chloride concentrations of the files
	double Cl[200]; 
	CString GatFileName[200];
	CString rootname;
	int ConcentrationIndex;

	void Fit();
	void Fit3StateModel();

	void FitMarkov(Model * PM);

	void ReadFiles(CString & NameFile);
	void SaveData();
	bool LoadData(int NTestCells);

	bool SaveThis(CFile * fp);
	bool LoadThis(CFile * fp);
	void Copyparamnstoclipboard();



	double alpha0[100], zalpha, beta0[100], zTotal, zK, K, zon, koff;
	BOOL Fitalpha0, Fitzalpha, Fitbeta0, FitzTotal, FitzK, FitK, Fitzon, Fitkoff;


	//GatingCell gc;

	double Params[1000];
	double BestParams[1000];
	int NParams;
 
	int PutParamsInArray(double * Params);
	int PutArrayInParams(double * Params);

	int PutParamsInArrayMarkov(double * Params);
	int PutArrayInParamsMarkov(double * Params);

//	void CalcFit();
	double CalcSD();
	double CalcSD3States();

	double CalcSDMarkov();


private:
	CString MakeFileName(CString file);

	Model * pm;
};

#endif
