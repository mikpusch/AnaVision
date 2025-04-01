/// RungeKutta.h

/// Implements the RungeKutta class.

/**************** Copyright Michael Pusch, pusch@ge.ibf.cnr.it, 2005 ***************/

/// Based on numerical recipes in C, this class implements the integration of a
/// a set of n ordinary differential equations

/// The code uses the (slightly modified) routines odeint, rkck, and rkqs from
/// numerical recipes in C

/// The usage is as follows.

/// Step 1: declare an object of type RungeKutta that must remain viable throughout
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
//#ifndef _MIK_RUNGE_KUTTA
//#define _MIK_RUNGE_KUTTA

typedef void (*derivfunctiontype)(double, double [], double [], int *);

// The last int * is used to pass user data, e.g. a pointer to a specific class instance


class RungeKutta{
public:
	RungeKutta();
	~RungeKutta();

	// The following two parameters can be changed from their default value
	static int MAXSTP; //= 1000000
	static double TINY; //= 1.0e-20

	bool ShowError;
	bool SwitchOffErrorAfterFirst;


	BOOL Initialize(	int NumberOfEquations,

				int kmax,				// the maximum number of intermediate 
										// steps stored. Can be zero.
				double * xp,			// the x-value array of the intermediate results
										// if kmax>0 this must be valid
				double ** yp,			// this is the matrix of y-values of intermediate
										// results. If kmax>0 this must be valid.
				int * kount,			// This is set after each call to "Integrate"
										// to the number of effective intermediate results.
										// Storage has to be provided a priori by the user for
										// xp and yp!!
				double dxsav,			// Intermediate results are stored at approximate 
										// intervals definded by dxsav

				double eps,				// Global accuracy
				double h1,				// guessed first step-size
				double hmin,			// minimum allowed step-size, can be zero

				derivfunctiontype derivs,

				int * stuff = NULL			// used e.g. to pass pointer to a class isntance
										// can be NULL if not used

//				void * derivs(double, double [], double [])
				);

	void Integrate(double * ystart, double x1, double x2, int & nok, int & nbad);



private:
	void Init(); // sets all to 0
	void Alloc();
	void Dealloc();

	int * STUFF;
	int nvar;
	int kmax;
	int * kount;
	double * xp;
	double ** yp;
	double dxsav;
	double eps;
	double h1;
	double hmin;

	derivfunctiontype derivs;

	double *yscal,*y,*dydx;
	double *yerr,*ytemp;

	double *ak2,*ak3,*ak4,*ak5,*ak6,*ytemp2;

//	void * DERIVS(double, double [], double []);


	void rkqs(double y[], double dydx[], int n, double *x, double htry, double eps,
		double yscal[], double *hdid, double *hnext);

	void rkck(double y[], double dydx[], int n, double x, double h, double yout[],
		double yerr[]);
	
	void CheckError(CString msg);
};

//#endif
