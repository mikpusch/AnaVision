#include "stdafx.h"
#include "../common/RadialBufferedDiffusion.h"
#include "../common/utils.h"
#include "../common/nrutil.h"

RadialDiffusion::RadialDiffusion(){
}
RadialDiffusion::~RadialDiffusion(){
}
void RadialDiffusion::Init(int NComps, double R0, double RMAX, double DR, double DT, vector<double> & Ds,
						   vector<double> & BULK,
		  vector<int> & LEFTBOUNDARYCONDITIONS, vector<double> & LEFTBOUNDARYVALUES,
		  int * STUFF){
	stuff = STUFF;
	r0=R0;
	rmax = RMAX;
	dr=DR;
	dt=DT;
	NComponents = NComps;
	
	DiffusionCoeff.resize(NComponents);
	LeftBoundaryConditions.resize(NComponents); //0: valuie fixed; 1: gradient fixed
	LeftBoundaryValues.resize(NComponents);
	BulkValues.resize(NComponents);
	RS.resize(NComponents);
	e1.resize(NComponents);
	e2.resize(NComponents);
	e3.resize(NComponents);
	localconc.resize(NComponents);

	for (int i=0; i<NComponents; i++){
		DiffusionCoeff[i]=Ds[i];
		LeftBoundaryConditions[i]=LEFTBOUNDARYCONDITIONS[i];
		LeftBoundaryValues[i]=LEFTBOUNDARYVALUES[i];
		BulkValues[i] = BULK[i];
	}

	M=int((rmax-r0)/dr)+1;

	r.resize(M);
	gam.resize(M);
	newconc.resize(M);


	for (int j=0; j<NComponents; j++){
		RS[j].resize(M);
		e1[j].resize(M);
		e2[j].resize(M);
		e3[j].resize(M);
	}

	for (int i=0; i<M; i++){
		r[i] = r0+double(i)*dr;
	}
}

void RadialDiffusion::OneStep(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
							  vector<vector<double> > & newconcentrations, bool DoShow){
	for (int i=0; i<NComponents; i++){
		double D = DiffusionCoeff[i];
		vector <double> g1 = e1[i];
		vector <double> g2 = e2[i];
		vector <double> g3 = e3[i];
		vector <double> R = RS[i];
		vector <double> & f = concentrations[i];

		double a=0.5*D*dt/dr/dr;
//		ShowFloat(a, "a");
//		ShowFloat(dr, "dr");
		double b=D*dt/dr;
		double bb=b/r0;

		g1[0]=0.0;
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][0];
//				ShowFloat(localconc[j], "lc");
		}
		R[0] = dt*reaction(i, localconc, t, stuff);
		if (LeftBoundaryConditions[i]==0){ // Value = 
//			ShowFloat(i, "i");
			g2[0]=1.0+2.0*a-bb;
			g3[0]=-a;
			double A = LeftBoundaryValues[i];

			R[0] += 2.0*A*(a-bb)+f[0]*(1.0-2.0*a+bb)+f[1]*a;
//			R[0] += f[0]*(1.0-2.0*a+b/r0)+f[1]*a+2.0*LeftBoundaryValues[i]*(a-b/r0);
//			ShowFloat(R[0], "R[0]");
//			ShowFloat(f[0], "f[0]");
		}

		else{ // Gradient =
			g2[0]=(1.0+a);
			g3[0]=-a;
			if (DoShow){
//				ShowFloat(R[0], "R[0] start");
			}
			R[0] += f[0]*(1.0-a)+f[1]*a;
			R[0] += LeftBoundaryValues[i]*dr*2.0*( -a + b/r0);
			if (DoShow){
//				ShowFloat(R[0], "R[0] after");
			}
		}

		for (int k=1; k<(M-1); k++){
			double rr=r[k];
			double bb=b/rr;
			g1[k]=-a + bb;
			g2[k]=1.0 + 2.0*a - bb;
			g3[k]=-a;

			for (int j=0; j<NComponents; j++){
				localconc[j] = concentrations[j][k];
//				ShowFloat(localconc[j], "lc");
			}
			R[k] = dt*reaction(i, localconc, t, stuff);
			if (DoShow){
				if (k==2 && i==1){
					ShowFloat(R[k], "k=2, R[0] start");
				}
			}

			R[k] += f[k-1]*(a-bb) + f[k]*(1.0-2.0*a+bb)+f[k+1]*a;
			if (DoShow){
				if (k==2 && i==1){
					ShowFloat(R[k], "k=2, R[0] after");
				}
			}
		}
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][M-1];
//				ShowFloat(localconc[j], "lc");
		}
		int k=M-1;
		bb=b/rmax;
		g1[M-1]=-a + bb;
		g2[M-1]=1.0+2.0*a-bb;
		g3[M-1]=0.0;
		R[M-1] = dt*reaction(i, localconc, t, stuff);
		R[M-1] += 2.0*a*BulkValues[i] + f[k-1]*(a-bb)+f[k]*(1.0-2.0*a+bb);

		tridiag2(g1, g2, g3, R, newconc, M, gam);
		for (int k=0; k<M; k++){
			newconcentrations[i][k] = newconc[k];
		}

	}

	t += dt;
}



