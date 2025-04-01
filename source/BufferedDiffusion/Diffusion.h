#pragma once
#define MAXMINBUFFEREDDIFFUSION 10000000 // 10 million
//#define FARADAY  96485.309

#define N_MAX_ROOTS 1000




class Diffusion{

public:

	Diffusion();
	~Diffusion();

	static double * roots;
	static double * alphan;
	static double * denum;
	static void InitRoots();
	static void Shutdown();

	void CopyParams(Diffusion & d);

	BOOL SaveOnFile(CFile * fp);
	BOOL LoadFromFile(CFile * fp);
	BOOL LoadFromFile2(CFile * fp);
	BOOL SaveOnFile2(CFile * fp);
	BOOL LoadFromFile3(CFile * fp);
	BOOL SaveOnFile3(CFile * fp);


	void ClearArrays();


	void GetValues(int j, double & H, double & B, double & BH);

	int NRootsUsed;

//This class contains all mathematical parameters and procedures that describe the diffusion.

//  The situation is as follows:

//  We treat the diffusion outside of a spherical cell of radius r0 (in m)

	double r0;

	double rmax; // the outer radius treated as bulk


//  We treat protons and a buffer with respective diffusion constants, bulk concentrations
//  (in molar = moles / liter)

	double DH; // diffusion constant of protons in m^2/s
	double DB; // diffusion constant of buffer (bound and unbound!)

	double pHbulk; // bulk pH

	double T;		// total constant buffer concentration in molar


//  buffer and protons react with forward reaction rate (association) alpha
//	and backward reaction rate beta

	double alpha; // in molar^-2 sec^-1
	double beta;  // in molar^-1 sec^-1

	double KD; // is = beta/alpha (in molar)

	int IndependentReactionParameters;	// = 0: alpha and beta
										// = 1: alpha and KD
										// = 2: beta and KD


// The region is sliced into n spherical slices. Either the slice thickness 
// or the number of slices can be defined.

	int M; // 
	double DR0; // r0+M*DR=rmax;
	double DRMax;
	double FactDR;

	int ModeToDetermineSliceNumber;		// = 0: fix n, calc. DR;
										// = 1: fix DR, calc. n;

// At r0 a possibly time-dependent proton current I pumps protons out.
// A fixed proton current parameter I is used for a constant proton current
// a procedure is used for more complicated situtations

	double IH; // in Amperers

	int ProtocolType;
		//	0 : constant
		//	1 : train
	double TimeCurrentOn;
	double TimeCurrentOff;
	int NPulsesInTrain; // if <=0 : indefinite

	BOOL DoRenormalize;

	BOOL AutoCalcBeta; // beta = delta total [H] / delta free [H] = 1+KT/(H0+K)^2
	double buffercapacextern;
	double factorcorrection;

	BOOL IncludeFixedSurfaceBuffer;
	double KDFixedSurfaceBuffer;
	double AmountFixedSurfaceBufferInMoles;
	double ThicknessFixedVolume; // im m
	double HSurface;
	double GetBufferedPhiFxedSurfaceBuffer(double DELTA);

	double GetProtonCurrent(double t /* in seconds */ ); // in Amperes; usually returns just IH.


// Integration occurs with time step DT

	double DT0;
	double DTMax;
	double FactDT;

	BOOL UseWagnerKeizer; // assumes implicitely equlibrium between buffer and protons
	BOOL SimpleWagnerKeizer;
// The following dynamic arrays keep the proton concentration, the free buffer concentration

//	These are used to read out after each step of integration.

	double * H, * B;

	double time;

	BOOL UseFluxCorrectionInLinearApproximation;

	BOOL UsePredictorInWagnerKeitzerForNonLinearTerms;


// The following arrays are used for the integration


	double *r, *rightside, *b, * ah, * bh, * ch, * ab, * bb, * cb, * hnew, * bnew, * gam, * Hold;


// The following procedure sets up the integration, freeing possibly old vectors,
// allocating space, initializing all arrays etc.

	void Setup();
	void SetupLinearized();
	void SetupLinearized2();

// These procedures perform one integration step

	void Step(bool actuallyused); // Integrates one time step;
//	void Step(int n);  // Integrates n time steps;

	int GetNumberOfSlices();
	double GetMaxTime(int iter);


// Parameters that describe the capillary diffusion problem:
	double TotalBleft; // Buffer left
	double TotalBright; // 
	double pHleft;
	double pHright;
// the spacial and temporal parameters are taken from the other parameters

	double GetAddedProtonConcInMolar(double H);
// for a given change of Hbulk to H and given T, calc. the conc. of added (or subracted)
// protons



	void DoSetup(int mode); // 0 = lin; 1 = regular; 2 = lin2
	void StepLinearized();
	BOOL UseLinearized;
	BOOL UseLinearized2;
	double CalcLinearized(double t, double r);


	int ArrayDimension; // this keps track of the last dimension of all arrays.
	void DeAlloc();
	void Alloc();


	double DT;
	double Hbulk;
	double Bbulk;
	double V0;

//	double d2;// = 1.0/(DR*DR);
//	double d2half; //= d2/2.0;
	double it; //= 1.0/DT;
	double bbb; // = beta*T;

	double J, beta0;

	double phi;

	double ChargeTransferred, TotalInitialProtons;

	double GetFreeBuffer(double ch ); // calc free buffer from proton concentration

	double CalcTotalProtons(double alpha);
	double GetBufferedPhi(double phi);
	double GetBufferedPhiWithFixedSurfaceBuffer(double phi);
	void RenormalizeH();
	double Solu2(double J, double beta0, double D, double a, double r, double t);



	// For explicit treatment of buffer reaction
	void CalcRightHandSides(); 
	 // Uses arrays H and B to calc the resepctive right hand sides for the integration


	void CalcRightSideAndMatrixWagnerKeizer();
	void CalcRightSideAndMatrixLinearized2();

	void StepWagnerKeizer();
	void SimpleStepWagnerKeizer();
	void StepLinearized2();


	int GetNumberOfSlices(BOOL AssignRVector);

// other parameters
	double Deff, prefactor;
	int NSum;
//	double FreeFixedBuffer; // = concentration of fixed, free buffer in molar; changes with time
	double TotalFixedBuffer; // in molar; fixed
	double VolSurf;

	void CalcRightSideAndMatrixWagnerKeizerIncludePredictor(bool UseOld);



	
};