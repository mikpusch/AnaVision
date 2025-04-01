#pragma once
#include <vector>
using namespace std ;

//
//
// This class calculates radial diffusion on several components in a time dependent manner. 
// Components can be chemically coupled
// RadialReaction (int component, vector<double> & concentrations,
//                  double & LinearPart, double t, int * STUFF);
//typedef double ( *RadialReaction_type)(int, vector<double> &, double &, double, int *);
typedef double ( *RadialReaction_type)(int, double *, double &, double, int *);

// this user supplied function describes the chemical terms of the equations and depends on the local values only:
//  dc/dt = Diffusion + Reaction;

// at r0 for each component it must be specified if the value is fixed or if the gradient is fixed;
// value = fixed: LeftBoundary = 0
// gradient = fixed: LeftBoundary = 1

class ThreadParamsClassOneStepOneComponent{
public:
	int IndexComponent;
	double *Pt_t;
	RadialReaction_type * Pt_reaction;
	vector<vector<double> > * Pt_concentrations;
	vector<vector<double> > * Pt_newconcentrations;
	bool * Pt_DoShow;
	LPVOID AddressOfRadialDiffusionClass;
};

class RadialDiffusion{
public:
	
	RadialDiffusion();
	~RadialDiffusion();

	bool linear; // else: radial

//	double *Pt_t;
//	RadialReaction_type * Pt_reaction;
//	vector<vector<double> > * Pt_concentrations;
//	vector<vector<double> > * Pt_newconcentrations;
//	bool * Pt_DoShow;

	bool ThreadFinished[1000]; // One for ecah componeent
	ThreadParamsClassOneStepOneComponent ThreadParams[1000];
	static UINT __cdecl GlobalOneStepOneComponent( LPVOID pParam );
	void OneStepOneComponent (int i, ThreadParamsClassOneStepOneComponent * ThreadParams) ; // i is the component


	// pParam pouints to the follwoing structure of addresses:
	// address of ThreadParasnClass

	int NComponents;
	vector <double> DiffusionCoeff; // diffusion coefficients in m^2/s
	vector <double> BulkValues; // at rmax

	vector <int> LeftBoundaryConditions; //0: valuie fixed; 1: gradient fixed
	vector <double> LeftBoundaryValues;

	double r0; // Minimum radius in meters
	double rmax; // in meters 
	double dr;   // in meters
	double dt; // in seconds

	int M; // points go from 0 to M-1; 0=r0; M-1=rmax

	void Init(int NComps, bool LINEAR, double R0, double RMAX, double DR, double DT,
				vector<double> & Ds, vector<double> & BULK,
				vector<int> & LEFTBOUNDARYCONDITIONS, vector<double> & LEFTBOUNDARYVALUES,
				int * STUFF);

	void OneStep(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
						vector<vector<double> > & newconcentrations, bool DoShow = false);
	void OneStepOld(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
						vector<vector<double> > & newconcentrations, bool DoShow = false);
	void OneStepNoThreads(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
						vector<vector<double> > & newconcentrations, bool DoShow = false);
	
	void OneStepLinear(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
						vector<vector<double> > & newconcentrations, bool DoShow = false);

private:
	vector<vector<double> > RS;
	vector<vector<double> > e1;
	vector<vector<double> > e2;
	vector<vector<double> > e3;

	vector<vector<double> > gam;
	vector<vector<double> > newconc;
//	vector<vector<double> > localconc;
	vector<double> r;

	int * stuff;


};
