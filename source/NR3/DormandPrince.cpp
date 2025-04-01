// Implementation of the Runge-Kutta class
#include "stdafx.h"


#include "DormandPrince.h"
#include "../common/nrutil.h"
#include "../common/utils.h"

#include <math.h>

int DormandPrince::MAXSTEP = 100;


rhs_DormanPrince::rhs_DormanPrince(){
	Y=DY=NULL;
}

void rhs_DormanPrince::Init(int * stuff, derivfunctiontype derivs, int nvar){
	STUFF = stuff;
	DERIVS = derivs;
	NVAR = nvar;

	Y = new double[NVAR];
	DY = new double[NVAR];
}

void rhs_DormanPrince::operator () (const Doub x, VecDoub_I & y, VecDoub_O & dydx){
	for (int i=0; i<NVAR; i++){
		Y[i] = y[i];
	}
	DERIVS(x, Y, DY, STUFF);
	for (int i=0; i<NVAR; i++){
		dydx[i] = DY[i];
	}
}

	
rhs_DormanPrince::~rhs_DormanPrince(){
	if (DY) delete [] DY;
	if (Y) delete [] Y;
}


DormandPrince::DormandPrince(){
	Init();
	SwitchOffErrorAfterFirst = true;
//	ShowError = false;
}
void DormandPrince::CheckError(CString msg){

#ifdef GEPULSEMODELFRAME
	return;
#endif

	if (!ShowError) return;
	Alert0(msg);
	if (SwitchOffErrorAfterFirst){
		ShowError = false;
	}

}

void DormandPrince::Init(){
	ShowError = true;
	SwitchOffErrorAfterFirst = true;

	nvar = 0;
	eps = 0.0;
	h1 = 0.0;
	hmin = 0.0;


//	y = NULL;
//	dydx = NULL;

}

void DormandPrince::Dealloc(){

//	if (yscal) delete [] yscal;
//	if (y) delete [] y;
//	if (dydx) delete [] dydx;
//	if (yerr) delete [] yerr;
//	if (ytemp) delete [] ytemp;
//	if (ytemp2) delete [] ytemp2;
//	if (ak2) delete [] ak2;
//	if (ak3) delete [] ak3;
//	if (ak4) delete [] ak4;
//	if (ak5) delete [] ak5;
//	if (ak6) delete [] ak6;
	Init();
}

void DormandPrince::Alloc(){

//	ak6 = new double[nvar];
//	ak5 = new double[nvar];
//	ak4 = new double[nvar];
//	ak3 = new double[nvar];
//	ak2 = new double[nvar];
//	ytemp2 = new double[nvar];
//	ytemp = new double[nvar];
//	yerr = new double[nvar];
//	dydx = new double[nvar];
//	yscal = new double[nvar];
//	y = new double[nvar];

}

DormandPrince::~DormandPrince(){
	Dealloc();
}

bool DormandPrince::Initialize(	int NumberOfEquations,

				double EPS,				// Global accuracy
				double H1,				// guessed first step-size
				double HMIN,			// minimum allowed step-size, can be zero
				derivfunctiontype DERIVS,

				int * stuff

				){

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

//	xp = XP;
//	yp = YP;
//	dxsav = DXSAV;
//	kount = KOUNT;
	eps = EPS;
//	ShowFloat(eps, "EPS");
	h1 = H1;
	hmin = HMIN;
	derivs = DERIVS;

	YSTART.resize(nvar);

	rhsdp.Init(stuff, derivs, nvar);

	return true;
}

void DormandPrince::Integrate(double * ystart, double x1, double x2, int & nok, int & nbad){

	for (int i=0; i<nvar; i++){
		YSTART[i] = ystart[i];
	}

//	CWaitCursor dummy;
	Odeint<StepperDopr853<rhs_DormanPrince> > ode(YSTART, x1, x2, eps, eps, h1, hmin, out, rhsdp);
	ode.integrate();
	for (int i=0; i<nvar; i++){
		ystart[i] = YSTART[i];
	}
	nok = ode.nok;
	nbad = ode.nbad;


}



