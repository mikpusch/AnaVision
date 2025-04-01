
/* The cg_descent  class is a wrapper and application class for the conjugated gradient methof of
   Hager and Zhang (2005)
*/

//The file f2c.h as used here will be needed.

// ATTENTION: The abs macro of the original f2c.h file has been modified to f2c_abs in order to avoid conflicts with the C abns function
// that works only on integers.

// created by Michael Pusch, 20/12/2007, pusch@ge.ibf.cnr.it

#ifndef _MIK_CG_DESCENT

#define _MIK_CG_DESCENT

#include "../common/f2c.h"

// The following function definitions include an additional int * parameter. This is intended to pass a pointer
//to the class instance of the calling class. If this is not needed, the pointer can be NULL.
//

typedef void (*cg_descent_costfunction_type)(double *, double *, int * );
typedef void (*cg_descent_costfunction_gradient_type)(double *, double *, int * );

/*

  The following example illustrates the use of the cg_descent class using the default parameters of the algorithm:
  
	cg_descent cg;

	int * stuff = (int *)(this);		// This pointer to the instance is stored in the cg_descent class and passed 
										// as a parameter in the cost function and gradient function

    if (!cg.Init(DIMENS, MyFunc, MyGrad, stuff)){
		Alert("Error init cg");
		return;
	}

	double x[DIMENS];
	for (int i=0; i<DIMENS; i++){
		x[i] = 1.0;
	}
	double Cost;

	CWaitCursor dummy;

	cg.MinimizeCost(x, &Cost);  // x is the starting point and the final result. Cost is the final cost.


The static!!! member functions, MyFunc and MyGrad, of the calling class can use the parameters of the instance of the class by 
using the stuff parameter as illustrated below:


  void MyClass::MyGrad(double * g, double * x, int * stuff){

	MyClass * p = (MyClass *)(stuff);

	...
  }


If no explicit function for the gradient is available, the gradient function can be passed as NULL. In this 
case the gradient will be calculated numerically using the Numerical Recipes dfridr function.


If the default parameters of the cg_descent algorithm shall be changed, that has to be done after calling Init and before 
calling MinimizeCost:

	cg_descent cg;

	int * stuff = (int *)(this);		// This pointer to the instance is stored in the cg_descent class and passed 
										// as a parameter in the cost function and gradient function

    if (!cg.Init(DIMENS, MyFunc, MyGrad, stuff)){
		Alert("Error init cg");
		return;
	}

// Change defaults:  For example

   cg.cgparms_.eps = 1.0e-3;

The parameters are described in the original article and are accessible as described in the class definition in the class 
definition.


*/


class cg_descent{
public:

	cg_descent();
	~cg_descent();

	bool Init(int n, // dimension
		cg_descent_costfunction_type CostFunction, // this must be supplied
		cg_descent_costfunction_gradient_type Gradient, // is this is NULL, the gradient will be computed numerically
		int * stuff
		);

	bool MinimizeCost(double * x, // start value and on return end value
					double * CostAtEnd);

	integer status;

// The following parametes assume default values, but can be changed 
struct {
    doublereal delta, sigma, eps, gamma, rho, tol, eta, fpert, f0, ck, qdecay,
	     wolfe_hi__, wolfe_lo__, awolfe_hi__, quadcutoff, stopfac, 
	    awolfefac, zero, feps, psi0, psi1, psi2;
    integer n, n5, n6, nf, ng, info, nrestart, nexpand, nsecant, maxit;
    logical pertrule, quadok, quadstep, printlevel, printfinal, stoprule, 
	    awolfe, step, debug;
} cgparms_;



#define cgparms_1 cgparms_

// The following functions and variables are hidden and implement the original algorithm, created by the f2c program from the 
// original FORTRAN code.

private:

	int * STUFF;

	void DeleteArrays();
	void AllocArrays(int d);

	void DoCalcGradientNumerically(double * g, double * x);
	double dfridr(int index, double * x, double h, double *err);


	int ActualSize;

	double gnorm;   /* --if the parameter Step in cg_descent.parm is  | */
/*     |                   .true., then gnorm contains the initial step | */
/*     |                   used at iteration 0 in the line search       | */
/*     |                                                                | */
	double * d;

/*     |(double) d       --direction (work array, length n)             | */
/*     |                                                                | */
/*     |(double) g       --gradient (work array, length n)              | */

	double * g;

/*     |                                                                | */
/*     |(double) xtemp   --work array (work array, length n)            | */

	double * xtemp;

                                                             
/*     |(double) gtemp   --work array (work array, length n)   */

   double * gtemp;

   double * xp;
   double * xm;
   double ** amatrix;


	void InitValues();

	int DIMENS;

	bool Initialized;

	cg_descent_costfunction_type COSTFUNCTION;

	bool CalcGradientNumerically;

	cg_descent_costfunction_gradient_type GRADIENT;

	void CalcValue(double * f, double * x);
	void CalcGradient(double * g, double * x);



int cg_descent__(doublereal *grad_tol__, doublereal *x, 
	int *dim, S_fp cg_value__, S_fp cg_grad__, integer *status, 
	doublereal *gnorm, doublereal *f, integer *iter, integer *nfunc, 
	integer *ngrad, doublereal *d__, doublereal *g, doublereal *xtemp, 
	doublereal *gtemp);

int cg_init__(doublereal *grad_tol__, int *dim);

int cg_step__(doublereal *xtemp, doublereal *x, doublereal *
	d__, doublereal *alpha);

int cg_line__(doublereal *alpha, doublereal *phi, doublereal 
	*dphi, doublereal *dphi0, doublereal *x, doublereal *xtemp, 
	doublereal *d__, doublereal *gtemp, S_fp cg_value__, S_fp cg_grad__);

int cg_linew__(doublereal *alpha, doublereal *phi, 
	doublereal *dphi, doublereal *dphi0, doublereal *x, doublereal *xtemp,
	 doublereal *d__, doublereal *gtemp, S_fp cg_value__, S_fp cg_grad__);

integer cg_updatew__(doublereal *a, doublereal *dpsia, doublereal *b, 
	doublereal *dpsib, doublereal *alpha, doublereal *phi, doublereal *
	dphi, doublereal *dpsi, doublereal *x, doublereal *xtemp, doublereal *
	d__, doublereal *gtemp, S_fp cg_value__, S_fp cg_grad__);


integer cg_update__(doublereal *a, doublereal *dphia, doublereal *b, 
	doublereal *dphib, doublereal *alpha, doublereal *phi, doublereal *
	dphi, doublereal *x, doublereal *xtemp, doublereal *d__, doublereal *
	gtemp, S_fp cg_value__, S_fp cg_grad__);


logical cg_wolfe__(doublereal *alpha, doublereal *f, doublereal *dphi);

logical cg_tol__(doublereal *f, doublereal *gnorm);

doublereal cg_dot__(doublereal *x, doublereal *y);

};

#endif