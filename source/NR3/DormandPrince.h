/// DormandPrince.h

/// Implements the DormandPrince class.

/**************** Copyright Michael Pusch, pusch@ge.ibf.cnr.it, 2005 ***************/

/// Based on numerical recipes in C, this class implements the integration of a
/// a set of n ordinary differential equations

/// The code uses the (slightly modified) routines odeint, rkck, and rkqs from
/// numerical recipes in C

/// The usage is as follows.

/// Step 1: declare an object of type DormandPrince that must remain viable throughout
///			integration that may involve several steps.

/// Step 2: Initialize the object. This defines several parameters like the number of
///			equations, the routine for calculating the derivatives ...

/// Step 3: Call (repeatedly) "::Integrate(double * ystart, double x1, double x2,
///											int & nok, int & nbad);
///								

/// This will integrate from the starting vector y[] (at value x1) to x2. The final 
/// vector will be stored again in y[]. Intermediate results will be stored on demand 
/// in a matrix provided in the initialization (see Numerical Recipes in C and below 
/// for details).

/// At difference to Numerical Recipes vector indexes are 0-based, i.e. vectors are
/// y[0], ..., y[n-1]
/// Another difference is that routines are in double precision


#pragma once

#include "../NR3/odeint.h"
#include "../NR3/StepperDopr853.h"


typedef void (*derivfunctiontype)(double, double [], double [], int *);

// The last int * is used to pass user data, e.g. a pointer to a specific class instance

struct rhs_DormanPrince{
	int * STUFF;
	derivfunctiontype DERIVS;
	int NVAR;
//	VecDoub Y;
	double * Y;
	double * DY;



	rhs_DormanPrince();
	~rhs_DormanPrince();

	void Init(int * stuff, derivfunctiontype deriv, int nvar);
	void operator () (const Doub x, VecDoub_I & y, VecDoub_O & dydx);
};

class DormandPrince{
public:
	DormandPrince();
	~DormandPrince();

	static int MAXSTEP;

	// The following two parameters can be changed from their default value
	static int MAXSTP; //= 1000000
	static double TINY; //= 1.0e-20

	bool ShowError;
	bool SwitchOffErrorAfterFirst;


	bool Initialize(	int NumberOfEquations,

				double eps,				// Global accuracy
				double h1,				// guessed first step-size
				double hmin,			// minimum allowed step-size, can be zero

				derivfunctiontype derivs,

				int * stuff = NULL			// used e.g. to pass pointer to a class instance
										// can be NULL if not used

				);

	void Integrate(double * ystart, double x1, double x2, int & nok, int & nbad);



private:
	void Init(); // sets all to 0
	void Alloc();
	void Dealloc();

	int * STUFF;
	int nvar;
	double dxsav;
	double eps;
	double h1;
	double hmin;

	rhs_DormanPrince rhsdp;

	VecDoub YSTART;
	Output out;


	derivfunctiontype derivs;

//	double *y,*dydx;
//	double *yerr,*ytemp;

//	double *ak2,*ak3,*ak4,*ak5,*ak6,*ytemp2;


	void CheckError(CString msg);

};


