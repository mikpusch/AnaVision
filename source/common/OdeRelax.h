/*
class OdeRelax 

is a wrapper class for using the numerical recipes in C function 

solvede

which implements the relaxation method for boundary value ode problems.

*/

#pragma once

#include <vector>

using namespace std ;

/*
This is the orginal definition:
void difeq(int k, int k1, int k2, int jsf, int is1, int isf,
		int indexv[], int ne, float **s, float **y);

Here this is amplified to use also x, and a STUFF *
void difeq(int k, int k1, int k2, int jsf, int is1, int isf,
		int indexv[], int ne, double * x, float **s, float **y, int * STUFF);

void difeq(int k, int k1, int k2, int jsf, int is1, int isf, int * indexv, double * x, double **s, double **y);
*/
typedef void ( *OdeRelax_DiffEqFunction_type)(int, int, int , int, int, int, int *, double *, double **, double **, int *);
/* The other parameters are known to the class! */


/*
USAGE: 
	First: Call
		Init(int NumberOfEquations, int NumberOfPoints, int NumberOfLeftBoundaryConditions);

	this will allocate the matrices.

	Then assign values for:

		difeq: The equation that defines the ode

		double * x //  NO DEFAULT!
		double ** y  // y[i][k]: value of eq. i at x[k] for initial guess;  NO DEFAULT

		
		double conv;// default: 5e-6
		double slowc; // default 1.0
		double * scalv; : 1...NE: default : 1.0
		int * indexv;	: 1...NE: default : i

		all of which are members of this class

		Then Call solvde(Maxiter, difeq, stuff);

*/


class OdeRelax{
public:

	OdeRelax();
	~OdeRelax();

	void Init(int NumberOfEquations, int NumberOfPoints, int NumberOfLeftBoundaryConditions);
	void solvde(int MaxIter, OdeRelax_DiffEqFunction_type equation, int * stuff);

	void ZeroYArrays();

	int ne, m;  // ne: Number of equations; m: number of space points

	int nb; // number of boundary conditions at "left" edge

	int itmax;
	double conv;
	double slowc;
	double * scalv;
	int * indexv;
	double ** y, ** s;
	double *** c;

	double * x;



	OdeRelax_DiffEqFunction_type difeq;


private:
	int * STUFF;
	int * kmax;
	double * ermax;

	bool Initialized;

	void DeAlloc();
	void DoDifeq(int k, int k1, int k2, int jsf, int is1, int isf, int * indexv, double * x, double **s, double **y, int * stuff);


};
