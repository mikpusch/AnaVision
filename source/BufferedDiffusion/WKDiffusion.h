
#define MAXMINBUFFEREDDIFFUSION 10000000 // 10 million
//#define FARADAY  96000.0
#define FARADAY  96485.309


class WKDiffusion{ // Wagner - Keizer Biophysical journal

public:

	WKDiffusion();
	~WKDiffusion();

	void CopyParams(Diffusion & d);

	BOOL SaveOnFile(CFile * fp);
	BOOL LoadFromFile(CFile * fp);

	void ClearArrays();

	int GetNumberOfSlices();

	void GetValues(int j, double & H, double & B, double & BH);



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
	double DR; // r0+M*DR=rmax;

	int ModeToDetermineSliceNumber;		// = 0: fix n, calc. DR;
										// = 1: fix DR, calc. n;

// At r0 a possibly time-dependent proton current I pumps protons out.
// A fixed proton current parameter I is used for a constant proton current
// a procedure is used for more complicated situtations

	double IH; // in Amperers

	double GetProtonCurrent(double t /* in seconds */ ); // in Amperes; usually returns just IH.


// Integration occurs with time step DT

	double DT;


// The following dynamic arrays keep the proton concentration, the free buffer concentration

//	These are used to read out after each step of integration.

	double * H, * B;


// The following arrays are used for the integration


	double *r, *h, *b, * ah, * bh, * ch, * ab, * bb, * cb, * hnew, * hold, * bnew, * bold;


// The following procedure sets up the integration, freeing possibly old vectors,
// allocating space, initializing all arrays etc.

	void Setup();

// These procedures perform one integration step

	void Step(double t); // Integrates one time step;
	void Step(double t, int n);  // Integrates n time steps;

private:

	int ArrayDimension; // this keps track of the last dimension of all arrays.
	void DeAlloc();
	void Alloc();


	double Hbulk;
	double Bbulk;
	double V0;

	double d2;// = 1.0/(DR*DR);
	double d2half; //= d2/2.0;
	double it; //= 1.0/DT;
	double bbb; // = beta*T;

	double phi;

	double GetFreeBuffer(double ch ); // calc free buffer from proton concentration

	void CalcRightHandSides(); // i.e. at time 0 or for time indepednent current!
	void CalcRightHandSides(double t);
	 // Uses arrays H and B to calc the resepctive right hand sides for the integration
};