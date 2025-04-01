class Diffusion;
class Simplex;

#define MAXCOEFFOUTOFFOCUSCORRRECTION 200


class Kinetics{
public:
	Kinetics();
	~Kinetics();

	bool ReadFromFile(CFile * fp, int bytesavailable);

	int SaveOnFile(CFile * fp); // returns the number of bytes written

	void Alloc();

	void DeAlloc();

	void ClearArrays();

	void GetMaxMin(double & minx, double & maxx, double & miny, double & maxy);

	int NumberOfCurves();
	int NPointsPerCurve();
	void GetPoint(int curve, int point, double & x, double & y, bool DoShowAsFunctionOfTime, bool FitPoint);
	void GetPoint(int curve, int point, double & x, double & y, bool FitPoint);

	double beta; // change total [H] / change of free [H]  

	double Current; // in Amperes
	BOOL FitCurrent;

	double D;

	BOOL FitD;

	bool HasBeenFitted;
	int FitType; // 0 : linear, Total H
				 // 1 : linear, pH
				 // 2 : simulated
	bool HasBeenConstructed;

	double a; // radius of oocyte in m
	double pixelsize; // in m

	int IndexRMin, IndexRMax;		// index of radii to be analyzed: the absolute value of the radius is
		// r = a + indexr*pixelsize;
	int DeltaRIndex;


	int IndexTimeMin, IndexTimeMax, DeltaTimeIndex;
	double dt; // seconds

	int BackgroundTimeIndex0, BackgroundTimeIndex1;

	int DeltaShow;

	BOOL TrackDrift;


	BOOL ShowAsFunctionOfTime; // else ShowAsFunctionOfRadius

	BOOL ConvertToDeltaHTotal;

	int NDataR;
	int NDataT;

	int SizeR;
	int SizeT;
	double ** data; ///  data [time][radius]  
	double ** fit;
	double * tempfit;

	void FitLinear();

	double InitialpH;
	double DH, TotalFree, AmountFixed, KFree, KFixed, ThicknessFixedBufferVolume;
	BOOL FitTotalFree, FitAmountFixed, FitKFree, FitKFixed, FitThicknessFixedBufferVolume;

	int MaxIterSimulFit;
	int NParamsSimulFit;
	double SDSimulFit;
	double BestSDSimulFit;
	double ParamsSimulFit[100];
	double BestParamsSimulFit[100];
	Simplex simpSimulFit;
	double StepSize;

	int PutParamsInArraySimulFit(double * params);
	int PutArrayInParamsSimulFit(double * params);
	double DoCalcSimulFit();
	void GetBestParamsSimulationFit(double & SD);


	int * AssociatedIndexes ;

	double DR, DT0, factdt, DTMAX, RMaxSimulation;

	void SetupSimulationFit();

	BOOL StepSimulationFit(double & SD);

	int iter;

	int NCoeffOffFocusCorrection;
	double HalfWidthOffFocusCorrectionInMeters;
	BOOL UseOffFocusCorrection;
	double CoeffsOffFocusCorrection[MAXCOEFFOUTOFFOCUSCORRRECTION];
	void CalcCoeffeOffFocusCorrection(double rInMeters);

	Diffusion d;

private:

	void CalcRadiusAndTimeDependence();
	double GetCurrentFromLinearFit(double & SD);
	void CalcTimeDependence(double r, double DeltaT);
};