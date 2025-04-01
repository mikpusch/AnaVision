#include "stdafx.h"

//#include "c:\msdev\projects\3States\TestMarkov.h"
#include <math.h>
#include "..\common\utils.h"
#include "..\common\nrutil.h"


#include "..\common\SSMarkov.h"

SSMarkov::SSMarkov(){
	NS = 0;
}

SSMarkov::~SSMarkov(){
	//ShowInt(NS, "enter del SSMarkov class");
	if (NS<=1) return;
	DeleteSSMarkov();
}

void SSMarkov::InitMarkov(int NStates){
	if (NStates<=1){
		Alert0("NStates < 2 in SSMarkov::InitMarkov");
		return;
	}

	if (NS==NStates) return;

	if (NStates>SS_MARKOV_MAX_STATES){
		ShowFloat(SS_MARKOV_MAX_STATES, "This is the max number of states handled by SSMarkov: ERROR");
		return;
	}

	if (NS!=0){
		DeleteSSMarkov();
	}
	NS = NStates;
	R = matrix(0, NS-1, 0, NS-1);
	AW = matrix(0, NS-1, 0, NS-1);
	IW = matrix(0, NS-1, 0, NS-1);
	pinf = dvector(0, NS-1);
	rs = dvector(0, NS-1);
}

void SSMarkov::DeleteSSMarkov(){
	free_dvector(rs, 0, NS-1);
	free_dvector(pinf, 0, NS-1);
	free_matrix(IW, 0, NS-1, 0, NS-1);
	free_matrix(AW, 0, NS-1, 0, NS-1);
	free_matrix(R, 0, NS-1, 0, NS-1);
}

void SSMarkov::ClearRates(float ** r){
	int i, j;
	if (NS<1) return;
	for (i=0; i<NS; i++){
		for (j=0; j<NS; j++){
			r[i][j] = 0;
		}
	}
}

void SSMarkov::SetRates(float ** rates){
	int i, j;
	double sum, tp1;
	float det;
	int nm=NS-1;
	for (i=0; i<NS; i++)
		for (j=0; j<NS; j++)
			R[i][j] = rates[i][j];


	for (i=0; i<NS; i++){
		sum=0.0;
		for (j=0; j<NS; j++){
			if (i!=j){
				sum -= R[i][j];
				AW[i][j] = R[j][i];
			}
		}
		AW[i][i] = sum;
	}

	for (i = 0; i<nm; i++){
		tp1 = -AW[i][nm];
	    rs[i] = tp1;
		for (j=0; j<nm; j++){
			IW[i][j] = AW[i][j] + tp1;
		}
	}

	InvertMat(IW, AW, nm, &det);

	if (det==0.0){
//		Alert0("Error in Invert Mat in SetRates");
		return;
	}

	double sump = 0.0;

	for (i=0; i<nm; i++){
		sum = 0.0;
		for (j=0; j<nm; j++){
			sum += AW[i][j]*rs[j];
		}
		sump += sum;
		pinf[i] = sum;
	}
	pinf[nm]=1.0 - sump;
}



