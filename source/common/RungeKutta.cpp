// Implementation of the Runge-Kutta class
#include "stdafx.h"


#include "../common/RungeKutta.h"
#include "../common/nrutil.h"
#include "../common/utils.h"

#include <math.h>

#ifdef GEPULSEMODELFRAME
	int		RungeKutta::MAXSTP	=  1000;
	double	RungeKutta::TINY	= 1.0e-7;
	#include "../GePulse/GePulse.h"

#else
	int		RungeKutta::MAXSTP	=  20000;
	
//double	RungeKutta::TINY	= 1.0e-30;
//double	RungeKutta::TINY	= 1.0e-20;
	double	RungeKutta::TINY	= 1.0e-12;
#endif

//int		RungeKutta::MAXSTP	=  1000000;


RungeKutta::RungeKutta(){
	Init();
	SwitchOffErrorAfterFirst = true;
//	ShowError = false;
}
void RungeKutta::CheckError(CString msg){

#ifdef GEPULSEMODELFRAME
	return;
#endif

	if (!ShowError) return;
	Alert0(msg);
	if (SwitchOffErrorAfterFirst){
		ShowError = false;
	}

}

void RungeKutta::Init(){
	ShowError = true;
	SwitchOffErrorAfterFirst = true;

	nvar = 0;
	kmax = 0;
	xp = NULL;
	yp = NULL;
	dxsav = 0.0;
	eps = 0.0;
	h1 = 0.0;
	hmin = 0.0;

	yscal = y = dydx = NULL;
	yerr = ytemp = NULL;
	ak2 = ak3 = ak4 = ak5 = ak6 = ytemp2 = NULL;

}

void RungeKutta::Dealloc(){

	if (yscal) delete [] yscal;
	if (y) delete [] y;
	if (dydx) delete [] dydx;
	if (yerr) delete [] yerr;
	if (ytemp) delete [] ytemp;
	if (ytemp2) delete [] ytemp2;
	if (ak2) delete [] ak2;
	if (ak3) delete [] ak3;
	if (ak4) delete [] ak4;
	if (ak5) delete [] ak5;
	if (ak6) delete [] ak6;
	Init();
}

void RungeKutta::Alloc(){

	ak6 = new double[nvar];
	ak5 = new double[nvar];
	ak4 = new double[nvar];
	ak3 = new double[nvar];
	ak2 = new double[nvar];
	ytemp2 = new double[nvar];
	ytemp = new double[nvar];
	yerr = new double[nvar];
	dydx = new double[nvar];
	yscal = new double[nvar];
	y = new double[nvar];

}

RungeKutta::~RungeKutta(){
	Dealloc();
}

BOOL RungeKutta::Initialize(	int NumberOfEquations,

				int KMAX,				// the maximum number of intermediate 
										// steps stored. Can be zero.
				double * XP,			// the x-value array of the intermediate results
										// if kmax>0 this must be valid
				double ** YP,			// this is the matrix of y-values of intermediate
										// results. If kmax>0 this must be valid.
				int * KOUNT,			// This is set after each call to "Integrate"
										// to the number of effective intermediate results.
										// Storage has to be provided a priori by the user for
										// xp and yp!!
				double DXSAV,			// Intermediate results are stored at approximate 
										// intervals definded by dxsav

				double EPS,				// Global accuracy
				double H1,				// guessed first step-size
				double HMIN,			// minimum allowed step-size, can be zero
				derivfunctiontype DERIVS,

				int * stuff

//				void * derivs(double, double [], double [])
				){

#ifdef GEPULSEMODELFRAME
	CGePulseApp* pApp = (CGePulseApp * )(AfxGetApp());
	CMainFrame * frame = (CMainFrame *) pApp->m_pMainWnd;

	MAXSTP	=  frame->pSet->RungeKutta_MAXSTEP;
	TINY	= frame->pSet->RungeKutta_TINY;
#else
//	MAXSTP	=  20000;
//	TINY	= 1.0e-12;
#endif
	if (NumberOfEquations<=0){
		Dealloc();
		return false;
	}
	if (NumberOfEquations!=nvar){
		Dealloc();
		nvar = NumberOfEquations;
		Alloc();
	}
	nvar = NumberOfEquations;

	STUFF = stuff;

	xp = XP;
	yp = YP;
	dxsav = DXSAV;
	kount = KOUNT;
	eps = EPS;
//	ShowFloat(eps, "EPS");
	h1 = H1;
	hmin = HMIN;
	derivs = DERIVS;
	return true;
}




void RungeKutta::Integrate(double * ystart, double x1, double x2, int & nok, int & nbad){

/*
void odeint(float ystart[], int nvar, float x1, float x2, float eps, float h1,
	float hmin, int *nok, int *nbad,
	void (*derivs)(float, float [], float []),
	void (*rkqs)(float [], float [], int, float *, float, float, float [],
	float *, float *, void (*)(float, float [], float [])))
*/

//	ShowFloat(MAXSTP, "MAXSTEP");
//	ShowFloat(TINY, "tiny");

	int nstp,i;
	double xsav,x,hnext,hdid,h;

	x=x1;
	h=SIGN(h1,x2-x1);
	nok = nbad = (*kount) = 0;
	for (i=0;i<nvar;i++) y[i]=ystart[i];
	if (kmax > 0) xsav=x-dxsav*2.0;
	for (nstp=0;nstp<MAXSTP;nstp++) {
		(*derivs)(x,y,dydx, STUFF);
		//ShowFloat(x, "x");
		//ShowFloat(dydx[0], "deriv");
		for (i=0;i<nvar;i++)
			yscal[i]=fabs(y[i])+fabs(dydx[i]*h)+TINY;

		//CHECK NEXT LINE: IS  (*kount) < (kmax-2) CORRECT?????
		if ( (kmax > 0) && ((*kount) < (kmax-2)) && (fabs(x-xsav) > fabs(dxsav))) {
			xp[++(*kount)-1]=x;
			for (i=0;i<nvar;i++) yp[i][(*kount)-1]=y[i];
			xsav=x;
		}
		if ((x+h-x2)*(x+h-x1) > 0.0) h=x2-x;

		rkqs(y,dydx,nvar,&x,h,eps,yscal,&hdid,&hnext);//,derivs);

		if (hdid == h){
			++(nok);
//			Alert0("step ok");
		}
		else{
			++(nbad);
//			Alert0("step bad");
		}
//		ShowFloat(x, "x");
		if ((x-x2)*(x2-x1) >= 0.0) {
			for (i=0;i<nvar;i++) ystart[i]=y[i];
			if (kmax) {
				xp[++(*kount)-1]=x;
				for (i=0;i<nvar;i++) yp[i][(*kount)-1]=y[i];
			}
			return;
		}
		if (fabs(hnext) <= hmin) CheckError(_T("Step size too small in odeint"));
		h=hnext;
	}
	CheckError(_T("Too many steps in routine odeint"));
}



#define SAFETY 0.9
#define PGROW -0.2
#define PSHRNK -0.25
#define ERRCON 1.89e-4

void RungeKutta::rkqs(double y[], double dydx[], int n, double *x, double htry, double eps,
					double yscal[], double *hdid, double *hnext){

	int i;
	double errmax,h,htemp,xnew;//,*yerr,*ytemp;

	h=htry;
//	ShowFloat(h, "h");
	for (;;) {
		rkck(y,dydx,n,*x,h,ytemp,yerr);
		errmax=0.0;
		for (i=0;i<n;i++) errmax=DMAX(errmax,fabs(yerr[i]/yscal[i]));
//		ShowFloat(errmax, "errmax");
		errmax /= eps;
		if (errmax <= 1.0) break;
		htemp=SAFETY*h*pow(errmax,PSHRNK);
//		ShowFloat(h, "h");
		h=(h >= 0.0 ? DMAX(htemp,0.1*h) : DMIN(htemp,0.1*h));
		//ShowFloat(h, "h");
		xnew=(*x)+h;
		if (xnew == *x) CheckError(_T("stepsize underflow in rkqs"));
	}
	if (errmax > ERRCON) *hnext=SAFETY*h*pow(errmax,PGROW);
	else *hnext=5.0*h;
	*x += (*hdid=h);
//	ShowFloat(*x, "*x");
	for (i=0;i<n;i++) y[i]=ytemp[i];

}
#undef SAFETY
#undef PGROW
#undef PSHRNK
#undef ERRCON

void RungeKutta::rkck(double y[], double dydx[], int n, double x, double h, double yout[],
					  double yerr[]){

//void rkck(float y[], float dydx[], int n, float x, float h, float yout[],
//	float yerr[], void (*derivs)(float, float [], float []))

	int i;
	static double a2=0.2,a3=0.3,a4=0.6,a5=1.0,a6=0.875,b21=0.2,
		b31=3.0/40.0,b32=9.0/40.0,b41=0.3,b42 = -0.9,b43=1.2,
		b51 = -11.0/54.0, b52=2.5,b53 = -70.0/27.0,b54=35.0/27.0,
		b61=1631.0/55296.0,b62=175.0/512.0,b63=575.0/13824.0,
		b64=44275.0/110592.0,b65=253.0/4096.0,c1=37.0/378.0,
		c3=250.0/621.0,c4=125.0/594.0,c6=512.0/1771.0,
		dc5 = -277.00/14336.0;
	double dc1=c1-2825.0/27648.0,dc3=c3-18575.0/48384.0,
		dc4=c4-13525.0/55296.0,dc6=c6-0.25;

	for (i=0;i<n;i++)
		ytemp2[i]=y[i]+b21*h*dydx[i];

	(*derivs)(x+a2*h,ytemp2,ak2,STUFF);
	for (i=0;i<n;i++)
		ytemp2[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);

	(*derivs)(x+a3*h,ytemp2,ak3,STUFF);
	for (i=0;i<n;i++)
		ytemp2[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);

	(*derivs)(x+a4*h,ytemp2,ak4,STUFF);
	for (i=0;i<n;i++)
		ytemp2[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);

	(*derivs)(x+a5*h,ytemp2,ak5, STUFF);
	for (i=0;i<n;i++)
		ytemp2[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);

	(*derivs)(x+a6*h,ytemp2,ak6, STUFF);
	for (i=0;i<n;i++)
		yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);


	for (i=0;i<n;i++)
		yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);

}
