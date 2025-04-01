
/* The Powell  class is a wrapper and application class for the Powell minimization method described in Numerical Recuipes in C
*/


#ifndef _MIK_POWELL

#define _MIK_POWELL


// The following function definition includes an additional int * parameter. This is intended to pass a pointer
//

typedef void (*powell_costfunction_type)(double *, double *, int * );

/*

  The following example illustrates the use of the Powell class using the default parameters of the algorithm:
  
	Powell pw;

	int * stuff = (int *)(this);		// This pointer to the instance is stored in the Powell class and passed 
										// as a parameter in the cost function 

    if (!pw.Init(DIMENS, MyFunc, stuff)){
		Alert("Error init Powell");
		return;
	}

	double x[DIMENS];
	for (int i=0; i<DIMENS; i++){
		x[i] = 1.0;
	}
	double Cost;

	CWaitCursor dummy;

	cw.MinimizeCost(x, &Cost);  // x is the starting point and the final result. Cost is the final cost.


The static!!! member functions, MyFunc  of the calling class can use the parameters of the instance of the class by 
using the stuff parameter as illustrated below:


  void MyClass::MyFunc(double * value, double * x, int * stuff){

	MyClass * p = (MyClass *)(stuff);

	...
  }



If the default parameters of the Powell algorithm shall be changed, that has to be done after calling Init and before 
calling MinimizeCost:

	Powell pw;

	int * stuff = (int *)(this);		// This pointer to the instance is stored in the cg_descent class and passed 
										// as a parameter in the cost function

    if (!cg.Init(DIMENS, MyFunc, MyGrad, stuff)){
		Alert("Error init cg");
		return;
	}

// Change defaults:  For example


   pw.ftol = 1e-3;
   pw.ITMAX = 100;


*/


class Powell{
public:

	Powell();
	~Powell();

	bool Init(int n, // dimension
		powell_costfunction_type CostFunction, // this must be supplied
		int * stuff
		);

	bool MinimizeCost(double * x, // start value and on return end value
					double * CostAtEnd);


	double ftol;
	int ITMAX;


private:

	int * STUFF;

	void DeleteArrays();
	void AllocArrays(int d);


	int ActualSize;


	void InitValues();

	int DIMENS;

	bool Initialized;

	powell_costfunction_type COSTFUNCTION;


	double CalcValue(double * x);



//	int ncom;
	double *xt;

	double *pcom,*xicom; //,(*nrfunc)(double []);

	double *pt,*ptt,*xit;
	double **xi;


	void linmin(double p[], double xi[], int n, double *fret); //, double (*func)(double []))

	void mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb, double *fc);
//		double (*func)(double))

//	double brent(double ax, double bx, double cx, double (*f)(double), double tol,
	double brent(double ax, double bx, double cx, double tol, double *xmin);

	void powell(double p[], int n, double ftol, int *iter, double *fret);
//		double (*func)(float []))

	//	double f1dim(double x);

	inline 	double f1dim(double x){
		int j;
		for (j=1;j<=DIMENS;j++){
			xt[j] = pcom[j] + x*xicom[j];
		}
		return CalcValue(xt);
	};
};

#endif