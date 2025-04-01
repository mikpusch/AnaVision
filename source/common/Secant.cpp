#include "stdafx.h"

#include "Secant.h"
#include "utils.h"
#include <math.h>

Secant::Secant(){
	Initialized = false;
}
Secant::~Secant(){
}
double Secant::InitSecant(double x1, double x2, double MaxError, int MaxIter ){
	X1=x1;
	X2=x2;
	j=-2;
	X=x1;
	MAXERROR = MaxError;
	MAXITER = MaxIter;
	Initialized = true;
	return X;
}
bool Secant::NextSecant(double fx, double & XNew){ // User has to calculate fx from the last xhere// returns last best value;
	if (!Initialized){
		Alert0("Secant not initialized");
		return true;
	}
	if (j==(-2)){
		Y1=fx;
		X=X2;
		j=-1;
		XNew = X;
		return true;
	}
	if (j==(-1)){
		Y2=fx;
		if ((Y1*Y2)>0.0){
			Alert0("Bad Starting values in Secant");
			return false; // "Converged"
		}
	}
	if (j>=0){
		// test for new:
		double Y0=fx;
		if ((Y0*Y1)>0.0){ // allora Y0 e Y2
			X1=X;
			Y1=Y0;
		}
		else{
			X2=X;
			Y2=Y0;
		}
	}
	j++;
	if ( (fabs(fx)<MAXERROR) || (j>MAXITER) ){
		XNew = X;
		return false;
	}
//	ShowFloat(j, "j");
//	ShowFloat(MAXITER, "MAXITER");
	double m=(Y2-Y1)/(X2-X1);
	double b=Y1-m*X1;
	X=-b/m;
	XNew = X;
	return true;
}
