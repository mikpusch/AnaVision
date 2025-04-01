#include "stdafx.h"

//#include "c:\msdev\projects\3States\TestMarkov.h"
#include <math.h>
#include "..\common\utils.h"
#include "..\common\nrutil.h"

#include "..\common\SparseMarkov.h"

int SparseMarkov::DEFAULT_MAX_ITER_SPARSE_MARKOV = 5000;
//double SparseMakov::DEFAULT_MAX_ITER_SPARSE_MARKOV = 1000;
//double SparseMakov::DEFAULT_MIN_NORM_SPARSE_MARKOV = 1e-50;
double SparseMarkov::DEFAULT_MIN_NORM_SPARSE_MARKOV = 1e-100;


SparseMarkov::SparseMarkov(){
	NS = 0;
	NRATES = 0;
	pinf = NULL;
	rsforcheck = NULL;
	rsa = NULL;
	isa = NULL;
}

SparseMarkov::~SparseMarkov(){
	//ShowInt(NS, "enter del SSMarkov class");
	DeleteSparseMarkov();
}


void SparseMarkov::SetNStatesAndNRatesMarkov(int NStates, int NRates){
	if (NStates<=1){
		Alert0(_T("NStates < 2 in SparseMarkov::SetNStatesAndNRatesMarkov"));
		return;
	}
	if (NRates<1){
		Alert0(_T("NRates < 1 in SparseMarkov::SetNStatesAndNRatesMarkov"));
		return;
	}
	if ((NStates<=NS) && (NRates<=NRATES)){ // no additional space to allocate
		NS = NStates;
		NRATES = NRates;
		return;
	}
	if (NStates<=NS){		// more rates to alloc
		if (rsa) delete [] rsa;
		if (isa) delete [] isa;
		NS = NStates;
		NRATES = NRates;
		rsa = new double [NRATES +2*NS];
		isa = new int [NRATES +2*NS];
		return;
	}
	if (NRates<=NRATES){		// more states to alloc
		if (pinf){
			delete [] pinf;
			delete [] rsforcheck;
		}
		NS = NStates;
		NRATES = NRates;
		pinf = new double [NS];
		rsforcheck = new double [NS];
		return;
	}
	// all newc
	DeleteSparseMarkov();

	NS = NStates;
	NRATES = NRates;
	rsa = new double [NRATES +2*NS];
	isa = new int [NRATES +2*NS];
	pinf = new double [NS];
	rsforcheck = new double [NS];
}


void SparseMarkov::DeleteSparseMarkov(){

	if (pinf) delete [] pinf;
	if (rsforcheck) delete [] rsforcheck;
	if (rsa) delete [] rsa;
	if (isa) delete [] isa;
	pinf = NULL;
	rsforcheck = NULL;
	rsa = NULL;
	isa = NULL;
}


void SparseMarkov::CalcPinf(){
	CalcPinf(DEFAULT_MIN_NORM_SPARSE_MARKOV, DEFAULT_MAX_ITER_SPARSE_MARKOV);
}
void SparseMarkov::CalcPinf(double MinNorm){
	CalcPinf(MinNorm, DEFAULT_MAX_ITER_SPARSE_MARKOV);
}
void SparseMarkov::CalcPinf(int MaxIter){
	CalcPinf(DEFAULT_MIN_NORM_SPARSE_MARKOV, MaxIter);
}
void SparseMarkov::CalcPinf(double MinNorm, int MaxIter){
	ProbabilitySparseGaussSeidel(NS, rsa, isa, NULL, pinf, rsforcheck, MaxIter, MinNorm);

}


void SparseMarkov::SetRates(int NStates, double ** R){

	// this assigns space  and determines also NRATES and assigns the values
	// in the sparse matrix, and calls "calcpinf"

	int nrates = 0;

	int i, j;

	for (i=0; i<NStates; i++){
		R[i][i] = 0;
		for (j=0; j<NStates; j++){
			if (i!=j){
				if ( R[i][j] != 0.0) {
//					ShowFloat(i, "i");
//					ShowFloat(j, "j");
//					ShowFloat(R[i][j], "rij");
					nrates++;
				}
				R[i][i] -= R[i][j]; //CCCHHHEEECCCKKK
//				R[i][i] -= R[j][i]; //CCCHHHEEECCCKKK
			}
		}
	}

	SetNStatesAndNRatesMarkov(NStates, nrates);

//	if (!sprsin(R, NS, 0.0, NRATES +2*NS, rsa, isa)){
//		Alert0("Error calling sprsin in SparseMarkov::SetRates");
//		return;
//	}
	if (!sprsTRANSPOSEin(R, NS, 0.0, NRATES +2*NS, rsa, isa)){
		Alert0(_T("Error calling sprsin in SparseMarkov::SetRates"));
		return;
	}

	CalcPinf();

}