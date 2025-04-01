/*

This class encapsulates the Praxis optimization algorithm


**/
#ifndef _MIK_PRAXIS

#define _MIK_PRAXIS

#define C_FLOAT64 double
#define C_INT int
static C_INT c__1 = 1;
static C_INT c__2 = 2;
static bool c_false = false;
static C_INT c__4 = 4;
static bool c_true = true;
static C_INT c__3 = 3;
static C_INT c__0 = 0;



// The following function definition includes an additional int * parameter. This is intended to pass a pointer
// to a class instant
// Thge first argument is the x array, the second is the function value
//

typedef void ( *praxis_costfunction_type)(double *, double *, int * );


/* In order to allow the praxis optimizatio run in a thread that allows the GUI to remian active, a pointer to a function
that calls an updtate draw (or anythig similar) is also provided. If this is NULL, no update is called. The argument is ageain 
the class instant.


*/
//typedef double (CALLBACK* doubleFuncdoubleintPointer)(double, int *); // this is the function template for the voltage-function for the numerical integration;

typedef void (*praxis_message_function_type)(int *);

/*

  The following example illustrates the use of the Praxs class using the default parameters of the algorithm:
  
	Praxis pr;

	int * stuff = (int *)(this);		// This pointer to the instance is stored in the Praxis class and passed 
										// as a parameter in the cost function 

	praxis_message_function_type UpdateFunction = NULL;
//or
	UpdateFunction = ...

    if (!pr.InitPraxis(DIMENS, MyFunc, stuff, UpdateFunction)){
		Alert("Error init Praxis");
		return;
	}

	double x[DIMENS];
	for (int i=0; i<DIMENS; i++){
		x[i] = 1.0;
	}
	double Cost;

	CWaitCursor dummy;

	pr.Minimize(x, &Cost);  // x is the starting point and the final result. Cost is the final cost.


The static!!! member functions, MyFunc  of the calling class can use the parameters of the instance of the class by 
using the stuff parameter as illustrated below:


  void MyClass::MyFunc(double * value, double * x, int * stuff){

	MyClass * p = (MyClass *)(stuff);

	...
  }



If the default parameters of the Praxis algorithm shall be changed, that has to be done after calling Init and before 
calling Minimize:

	Praxis pr;

	int * stuff = (int *)(this);		// This pointer to the instance is stored in the cg_descent class and passed 
										// as a parameter in the cost function
    if (!pr.Init(DIMENS, MyFunc, MyGrad, stuff)){
		Alert("Error init cg");
		return;
	}

// Change defaults:  For example


....

*/


class Praxis{

	 struct Global
  {
    C_FLOAT64 fx, ldt, dmin__;
    C_INT nf, nl;
  };

  struct Q
  {
    C_FLOAT64 v[10000] /* was [100][100] */, q0[100], q1[100], qa, qb, qc, qd0,
    qd1, qf1;
  };

public:

  Praxis();
  ~Praxis();

   void InitPraxis( int n,    // number of vars
					praxis_costfunction_type f, // Functor for function under investigation
					double Tolerance, 
                    int * STUFF = NULL,   // The class instance used to calc the cost function, in general
					int MaxIter = -1, 
					int * StatusInt = NULL,
					double * CurrentBestParams = NULL, // can be used if conjunctance with the messaging to store the currently best params
					praxis_message_function_type  msgfunc = NULL,
					int * MessageStuff = NULL);  // if you want to send messages

  void Minimize(double Tolerance, double MaxStepSize,  double * xarray, double & Fmin);

public:
//  CRandom * mpRandom;

	bool Initialized;
  Global global_1;
  Q q_1;

  double EvalFunc(double * xarray);
  void CopyBestParams(double result, double * xarray);

  praxis_costfunction_type  COSTFUNC;

  bool WeHaveToStop;
  int MAXITER;
  int * STUFF;
  int * MESSAGESTUFF;
  int * STATUSINT;
  int iter;
  double * CURRENTBESTPARAMS;
  double BestResult;
  praxis_message_function_type  MSGFUNC;
  int DIMENS;
  double MACHEP;
  double T0; // Tolerance



public:

  C_FLOAT64 praxis_(C_FLOAT64 *t0,
                    C_FLOAT64 *machep,
                    C_FLOAT64 *h0,
                    C_INT *n,
                    C_INT *prin,
                    C_FLOAT64 *x,
//                    FPraxis *f, // Functor for function under investigation
                    praxis_costfunction_type f, // Functor for function under investigation
                    C_FLOAT64 *fmin);

  int min_(C_INT *, C_INT *, C_INT *,
           C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *, bool *,praxis_costfunction_type  ,
		   //FPraxis *f,
           C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *);
  C_FLOAT64 flin_(C_INT *, C_INT *, C_FLOAT64 *, praxis_costfunction_type , C_FLOAT64 *, C_INT *);
//  C_FLOAT64 flin_(C_INT *, C_INT *, C_FLOAT64 *, FPraxis *, C_FLOAT64 *, C_INT *);
  int quad_(C_INT *, praxis_costfunction_type f, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *);
//  int quad_(C_INT *, FPraxis *f, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *);
  int print_(C_INT *n, C_FLOAT64 *x, C_INT *prin, C_FLOAT64 *fmin);
  int minfit_(C_INT *, C_INT *, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *, C_FLOAT64 *);

};


#endif