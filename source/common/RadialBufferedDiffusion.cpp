#include "stdafx.h"
#include "../common/RadialBufferedDiffusion.h"
#include "../common/utils.h"
#include "../common/nrutil.h"

//CRITICAL_SECTION CritSect;

RadialDiffusion::RadialDiffusion(){
}
RadialDiffusion::~RadialDiffusion(){
}
void RadialDiffusion::Init(int NComps, bool LINEAR, double R0, double RMAX,
						   double DR, double DT, vector<double> & Ds,
						   vector<double> & BULK,
		  vector<int> & LEFTBOUNDARYCONDITIONS, vector<double> & LEFTBOUNDARYVALUES,
		  int * STUFF){
	linear = LINEAR;
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
	gam.resize(NComponents);
	newconc.resize(NComponents);
//	localconc.resize(NComponents);

	for (int i=0; i<NComponents; i++){
		DiffusionCoeff[i]=Ds[i];
		LeftBoundaryConditions[i]=LEFTBOUNDARYCONDITIONS[i];
		LeftBoundaryValues[i]=LEFTBOUNDARYVALUES[i];
		BulkValues[i] = BULK[i];
	}

	M=int((rmax-r0)/dr)+1;

	r.resize(M);

	for (int j=0; j<NComponents; j++){

		gam[j].resize(M);
		newconc[j].resize(M);
		
		RS[j].resize(M);
		e1[j].resize(M);
		e2[j].resize(M);
		e3[j].resize(M);
	}

	for (int i=0; i<M; i++){
		r[i] = r0+double(i)*dr;
	}
}

void RadialDiffusion::OneStepOld(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
							  vector<vector<double> > & newconcentrations, bool DoShow){
	if (linear){
		OneStepLinear(t, reaction, concentrations,
							  newconcentrations, DoShow);
		return;
	}
//		ShowFloat(concentrations[0][0], "concentrations[0][0]");
//		ShowFloat(concentrations[1][0], "concentrations[1][0]");
//		ShowFloat(concentrations[2][0], "concentrations[2][0]");
	double localconc[1000];
	for (int i=0; i<NComponents; i++){
//		double bb=0.0; // for the linear case
		double D = DiffusionCoeff[i];
		double b=D*dt/dr;

		vector <double> & g1 = e1[i];
		vector <double> & g2 = e2[i];
		vector <double> & g3 = e3[i];
		vector <double> & R = RS[i];
		vector <double> & f = concentrations[i];

		double a=0.5*D*dt/dr/dr;
//		ShowFloat(a, "a");
//		ShowFloat(dr, "dr");
//		if (!linear){
//			double b=D*dt/dr;
		double bb=b/r0;
//		}

		g1[0]=0.0;
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][0];
//				ShowFloat(localconc[j], "lc");
		}
		double LinPart = 0.0;
//		R[0] = dt*reaction(i, localconc, LinPart, t, stuff);
		R[0] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
//		ShowFloat(R[0], "R[0]");
		LinPart = 0.5*dt*LinPart;
		if (LeftBoundaryConditions[i]==0){ // Value = 
//			ShowFloat(i, "i");
			g2[0]=1.0+2.0*a-bb;
			g3[0]=-a;
			double A = LeftBoundaryValues[i];

			R[0] += 2.0*A*(a-bb)+f[0]*(1.0-2.0*a+bb)+f[1]*a;
//			R[0] += f[0]*(1.0-2.0*a+b/r0)+f[1]*a+2.0*LeftBoundaryValues[i]*(a-b/r0);
//			ShowFloat(R[0], "R[0]");
//			ShowFloat(f[0], "f[0]");
			R[0] += 2.0*LinPart*A;
		}

		else{ // Gradient =
			g2[0]= 1.0 + a - LinPart;
			g3[0]=-a;
			if (DoShow){
//				ShowFloat(R[0], "R[0] start");
			}
			R[0] += f[0]*(1.0-a)+f[1]*a;
			R[0] += LeftBoundaryValues[i]*dr*2.0*( -a + bb);
//			ShowFloat(LeftBoundaryValues[i], "LeftBoundaryValues[i]");
			R[0] += LinPart*f[0];
			if (DoShow){
//				ShowFloat(R[0], "R[0] after");
			}
		}

		for (int k=1; k<(M-1); k++){
			double rr=r[k];
//			if (!linear){
//				double b=D*dt/dr;
				bb=b/rr;
//			}
			g1[k]=-a + bb;
			g2[k]=1.0 + 2.0*a - bb;
			g3[k]=-a;

			for (int j=0; j<NComponents; j++){
				localconc[j] = concentrations[j][k];
//				ShowFloat(localconc[j], "lc");
			}
			double LinPart = 0.0;
			R[k] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
			LinPart = LinPart*dt*0.5;
			if (false){
//			if (i==2){
				if (k==1){
					ShowFloat(reaction(i, &(localconc[0]), LinPart, t, stuff), "reaction(i, localconc, t, stuff);");
				}
			}
			if (DoShow){
				if (k==2 && i==1){
					ShowFloat(R[k], "k=2, R[0] start");
				}
			}

			R[k] += f[k-1]*(a-bb) + f[k]*(1.0-2.0*a+bb)+f[k+1]*a;

			R[k] += LinPart*f[k];
			g2[k] -= LinPart;

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
//		if (!linear){
			bb=b/rmax;
//		}
		g1[M-1]=-a + bb;
		g2[M-1]=1.0+2.0*a-bb;
		g3[M-1]=0.0;
		LinPart = 0.0;
		R[M-1] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
		LinPart *= dt*0.5;

		R[M-1] += 2.0*a*BulkValues[i] + f[k-1]*(a-bb)+f[k]*(1.0-2.0*a+bb);

		R[M-1] += LinPart*f[M-1];
		g2[M-1] -= LinPart;

		tridiag2(g1, g2, g3, R, newconc[i], M, gam[i]);
		for (int k=0; k<M; k++){
			newconcentrations[i][k] = newconc[i][k];
		}
	//	ShowFloat(newconcentrations[0][0], "concentrations[0][0]");
	//	ShowFloat(newconcentrations[1][0], "concentrations[1][0]");
	//	ShowFloat(newconcentrations[2][0], "concentrations[2][0]");

	}

	t += dt;
}


void RadialDiffusion::OneStepNoThreads(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
							  vector<vector<double> > & newconcentrations, bool DoShow){
/*	Pt_t = &(t);
	Pt_reaction = &(reaction);
	Pt_concentrations = &(concentrations);
	Pt_newconcentrations = &(newconcentrations );
	Pt_DoShow = &(DoShow);
*/
	if (linear){
		OneStepLinear(t, reaction, concentrations,
							  newconcentrations, DoShow);
		return;
	}

//		ShowFloat(concentrations[0][0], "concentrations[0][0]");
//		ShowFloat(concentrations[1][0], "concentrations[1][0]");
//		ShowFloat(concentrations[2][0], "concentrations[2][0]");
	double localconc[1000];
	for (int i=0; i<NComponents; i++){

		double D = DiffusionCoeff[i];
//		ShowFloat(D, "D");
		double b=D*dt/dr;

		vector <double> & g1 = e1[i];
		vector <double> & g2 = e2[i];
		vector <double> & g3 = e3[i];
		vector <double> & R = RS[i];
		vector <double> & f = concentrations[i];

		for (int l=0; l<M; l++){
			g1[l]=g2[l]=g3[l]=R[l]=0.0;
		}

		double eps=0.5*D*dt/dr/dr;
//		ShowFloat(a, "a");
//		ShowFloat(dr, "dr");
//		if (!linear){
//			double b=D*dt/dr;
		double bb=b/r0;
//		}

		g1[0]=0.0;
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][0];
//				ShowFloat(localconc[j], "lc");
		}
		double LinPart = 0.0;
		R[0] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
//		ShowFloat(R[0], "R[0]");
		LinPart = 0.5*dt*LinPart;
		if (LeftBoundaryConditions[i]==0){ // Value = 
//			ShowFloat(i, "i");
			g2[0]=1.0+2.0*eps+bb-LinPart;
			g3[0]=-eps-bb;
			double A = LeftBoundaryValues[i];

			R[0] += 2.0*A*eps;
			R[0] += f[0]*(1.0 -2.0*eps -bb + LinPart);
			R[0] += f[1]*(eps + bb);
		}

		else{ // Gradient =
			g2[0]= 1.0 + eps + bb - LinPart;
			g3[0]= -eps - bb;
			if (DoShow){
//				ShowFloat(R[0], "R[0] start");
			}
			double G = LeftBoundaryValues[i];
			R[0] += f[0]*(1.0 - eps - bb + LinPart);
			R[0] += f[1]*(eps + bb);
			R[0] -= 2.0*eps*G*dr;
			if (DoShow){
//				ShowFloat(R[0], "R[0] after");
			}
		}

		for (int k=1; k<(M-1); k++){
			double rr=r[k];
//			if (!linear){
//				double b=D*dt/dr;
				bb=b/rr;
//			}
			for (int j=0; j<NComponents; j++){
				localconc[j] = concentrations[j][k];
//				ShowFloat(localconc[j], "lc");
			}
			double LinPart = 0.0;
			R[k] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
			LinPart = LinPart*dt*0.5;
			if (false){
				if (i==2 && k==1){
					ShowFloat(R[k], "R1");
					ShowFloat(LinPart, "LinPart");
				}
			}

			g1[k]= -eps;
			g2[k]= 1.0 + 2.0*eps + bb - LinPart;
			g3[k]= -eps - bb;

			if (false){
//			if (i==2){
				if (k==1){
					ShowFloat(reaction(i, &(localconc[0]), LinPart, t, stuff), "reaction(i, localconc, t, stuff);");
				}
			}
			if (DoShow){
				if (k==2 && i==1){
					ShowFloat(R[k], "k=2, R[0] start");
				}
			}

			R[k] += f[k-1]*eps;
			R[k] += f[k]*(1.0 - 2.0*eps - bb + LinPart);
			R[k] += f[k+1]*(eps + bb);

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
		bb=b/r[k];

		LinPart = 0.0;
		R[M-1] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
		LinPart *= dt*0.5;

		g1[M-1] = -eps;
		g2[M-1] = 1.0 + 2.0*eps + bb - LinPart;
		g3[M-1] = 0.0;

		R[M-1] += BulkValues[i]*(2.0*eps + 2.0*bb);
		R[M-1] += f[k-1]*eps;
		R[M-1] += f[k]*(1.0 - 2.0*eps - bb + LinPart);

		tridiag2(g1, g2, g3, R, newconc[i], M, gam[i]);
		for (int k=0; k<M; k++){
			newconcentrations[i][k] = newconc[i][k];
		}
	//	ShowFloat(newconcentrations[0][0], "concentrations[0][0]");
	//	ShowFloat(newconcentrations[1][0], "concentrations[1][0]");
	//	ShowFloat(newconcentrations[2][0], "concentrations[2][0]");

	}

	t += dt;
}


void RadialDiffusion::OneStepLinear(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
							  vector<vector<double> > & newconcentrations, bool DoShow){
		double localconc[1000];
		for (int i=0; i<NComponents; i++){
		double D = DiffusionCoeff[i];
		vector <double> & g1 = e1[i];
		vector <double> & g2 = e2[i];
		vector <double> & g3 = e3[i];
		vector <double> & R = RS[i];
		vector <double> & f = concentrations[i];

		double a=0.5*D*dt/dr/dr;
//		ShowFloat(a, "a");
//		ShowFloat(dr, "dr");
//		double b=D*dt/dr;
//		double bb=b/r0;

		g1[0]=0.0;
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][0];
//				ShowFloat(localconc[j], "lc");
		}
		double LinPart = 0.0;
		R[0] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
		LinPart *= 0.5*dt;
		
		if (LeftBoundaryConditions[i]==0){ // Value = 
//			ShowFloat(i, "i");
			g2[0] = 1.0+2.0*a ;
			g3[0] = -a;
			double A = LeftBoundaryValues[i];
			R[0] += 2.0*A*a+f[0]*(1.0-2.0*a)+f[1]*a;
			R[0] += 2.0*LinPart*A;
		}

		else{ // Gradient =
			g2[0]=1.0 + a - LinPart;
			g3[0]=-a;
			if (DoShow){
//				ShowFloat(R[0], "R[0] start");
			}
			R[0] += f[0]*(1.0-a)+f[1]*a;
			R[0] -= LeftBoundaryValues[i]*dr*2.0*a;
			R[0] += LinPart*f[0];
//			ShowFloat(LeftBoundaryValues[i], "LeftBoundaryValues[i]");
			if (DoShow){
//				ShowFloat(R[0], "R[0] after");
			}
		}

		for (int k=1; k<(M-1); k++){
			double rr=r[k];
//			double bb=b/rr;
			g1[k]=-a;
			g2[k]=1.0 + 2.0*a;
			g3[k]=-a;

			for (int j=0; j<NComponents; j++){
				localconc[j] = concentrations[j][k];
//				ShowFloat(localconc[j], "lc");
			}
			double LinPart = 0.0;
			R[k] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
			LinPart *= dt*0.5;
			R[k] += LinPart*f[k];
			g2[k] -= LinPart;

			if (false){
//			if (i==2){
				if (k==1){
					ShowFloat(reaction(i, &(localconc[0]), LinPart, t, stuff), "reaction(i, localconc, t, stuff);");
				}
			}
			if (DoShow){
				if (k==2 && i==1){
					ShowFloat(R[k], "k=2, R[0] start");
				}
			}

			R[k] += f[k-1]*a + f[k]*(1.0-2.0*a)+f[k+1]*a;
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
//		bb=b/rmax;
		g1[M-1]=-a;
		g2[M-1]=1.0+2.0*a;
		g3[M-1]=0.0;
		LinPart = 0.0;
		R[M-1] = dt*reaction(i, &(localconc[0]), LinPart, t, stuff);
		LinPart = LinPart*dt*0.5;

		R[M-1] += 2.0*a*BulkValues[i] + f[k-1]*a + f[k]*(1.0-2.0*a);

		R[M-1] += LinPart*f[M-1];
		g2[M-1] -= LinPart;

		tridiag2(g1, g2, g3, R, newconc[i], M, gam[i]);
		for (int k=0; k<M; k++){
			newconcentrations[i][k] = newconc[i][k];
		}
	}

	t += dt;
}



void RadialDiffusion::OneStep(double & t, RadialReaction_type reaction, vector<vector<double> > & concentrations,
							  vector<vector<double> > & newconcentrations, bool DoShow){

//this->OneStepNoThreads(t, reaction, concentrations, newconcentrations, DoShow);
//return;

	if (linear){
		OneStepLinear(t, reaction, concentrations,
							  newconcentrations, DoShow);
		return;
	}
	for (int i=0; i<NComponents; i++){
		ThreadFinished[i] = false;
	}

	ThreadParams[0].IndexComponent  = 0;
	ThreadParams[0].Pt_t = &(t);
	ThreadParams[0].Pt_reaction = &(reaction);
	ThreadParams[0].Pt_concentrations = &(concentrations);
	ThreadParams[0].Pt_newconcentrations = &(newconcentrations );
	ThreadParams[0].Pt_DoShow = &(DoShow);
	ThreadParams[0].AddressOfRadialDiffusionClass = this;

//	Pt_t = &(t);
//	Pt_reaction = &(reaction);
//	Pt_concentrations = &(concentrations);
//	Pt_newconcentrations = &(newconcentrations );
//	Pt_DoShow = &(DoShow);

	for (int i=1; i<NComponents; i++){
		ThreadParams[i] = ThreadParams[0];
		ThreadParams[i].IndexComponent  = i;
	}
//		ShowFloat(concentrations[0][0], "concentrations[0][0]");
//		ShowFloat(concentrations[1][0], "concentrations[1][0]");
//		ShowFloat(concentrations[2][0], "concentrations[2][0]");
//    InitializeCriticalSection(&CritSect);

	for (int i=0; i<NComponents; i++){
		if (i<(NComponents-1)){
			AfxBeginThread(GlobalOneStepOneComponent, LPVOID(&(ThreadParams[i])));
		}
		else{
			OneStepOneComponent(i, &(ThreadParams[i]));
//		    EnterCriticalSection(&CritSect);
			ThreadFinished[i] = true;
//		    LeaveCriticalSection(&CritSect);
		}
	}
	while (true){
//		Sleep(1);
		bool finished = true;
//	    EnterCriticalSection(&CritSect);
		for (int i=0; i<NComponents; i++){
			finished = finished && ThreadFinished[i];
		}
//	    LeaveCriticalSection(&CritSect);
		if (finished) break;
	}
	t += dt;
//	DeleteCriticalSection(&CritSect);
}

UINT RadialDiffusion::GlobalOneStepOneComponent( LPVOID pParam ){
//	Alert0("hi hi");

	ThreadParamsClassOneStepOneComponent * Param = (ThreadParamsClassOneStepOneComponent *) pParam;
	int IndexComponent = Param->IndexComponent;
//	ShowFloat(IndexComponent, "IndexComponent");
//	if (IndexComponent == 0){
//		ShowFloat(IndexComponent, "IndexComponent");
//	}
	RadialDiffusion * pDiff = (RadialDiffusion *) Param->AddressOfRadialDiffusionClass;
//	pDiff->ThreadFinished[IndexComponent] = false;
	pDiff->OneStepOneComponent(IndexComponent, Param);
//    EnterCriticalSection(&CritSect);
	pDiff->ThreadFinished[IndexComponent] = true;
//	LeaveCriticalSection(&CritSect);
	return 0;
}
void RadialDiffusion::OneStepOneComponent (int i, ThreadParamsClassOneStepOneComponent * ThreadParams) { // i is the component
//	ShowFloat(i, "i");

	double localconc[1000];

	double & t = *ThreadParams->Pt_t;
	RadialReaction_type &   reaction = *ThreadParams->Pt_reaction;
	vector<vector<double> > & concentrations = *ThreadParams->Pt_concentrations;
	vector<vector<double> > & newconcentrations = *ThreadParams->Pt_newconcentrations;
//	bool & DoShow = *ThreadParams->Pt_DoShow;

//	double & t = *Pt_t;
//	RadialReaction_type &   reaction = *Pt_reaction;
//	vector<vector<double> > & concentrations = *Pt_concentrations;
//	vector<vector<double> > & newconcentrations = *Pt_newconcentrations;
//	bool & DoShow = *Pt_DoShow;

		double D = DiffusionCoeff[i];
//		ShowFloat(D, "D");
		double b=D*dt/dr;

		vector <double> & g1 = e1[i];
		vector <double> & g2 = e2[i];
		vector <double> & g3 = e3[i];
		vector <double> & R = RS[i];
		vector <double> & f = concentrations[i];

		for (int l=0; l<M; l++){
			g1[l]=g2[l]=g3[l]=R[l]=0.0;
		}

		double eps=0.5*D*dt/dr/dr;
//		ShowFloat(a, "a");
//		ShowFloat(dr, "dr");
//		if (!linear){
//			double b=D*dt/dr;
		double bb=b/r0;
//		}

		g1[0]=0.0;
//	    EnterCriticalSection(&CritSect);
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][0];
//				ShowFloat(localconc[j], "lc");
		}
		double LinPart = 0.0;
		R[0] = dt*reaction(i, localconc, LinPart, t, stuff);
//		LeaveCriticalSection(&CritSect);

//		ShowFloat(R[0], "R[0]");
		LinPart = 0.5*dt*LinPart;
		if (LeftBoundaryConditions[i]==0){ // Value = 
//			ShowFloat(i, "i");
			g2[0]=1.0+2.0*eps+bb-LinPart;
			g3[0]=-eps-bb;
			double A = LeftBoundaryValues[i];

			R[0] += 2.0*A*eps;
			R[0] += f[0]*(1.0 -2.0*eps -bb + LinPart);
			R[0] += f[1]*(eps + bb);
		}

		else{ // Gradient =
			g2[0]= 1.0 + eps + bb - LinPart;
			g3[0]= -eps - bb;
//			if (DoShow){
//				ShowFloat(R[0], "R[0] start");
//			}
			double G = LeftBoundaryValues[i];
			R[0] += f[0]*(1.0 - eps - bb + LinPart);
			R[0] += f[1]*(eps + bb);
			R[0] -= 2.0*eps*G*dr;
//			if (DoShow){
//				ShowFloat(R[0], "R[0] after");
//			}
		}

		for (int k=1; k<(M-1); k++){
			double rr=r[k];
//			if (!linear){
//				double b=D*dt/dr;
				bb=b/rr;
//			}

//			EnterCriticalSection(&CritSect);
			for (int j=0; j<NComponents; j++){
				localconc[j] = concentrations[j][k];
//				ShowFloat(localconc[j], "lc");
			}
			double LinPart = 0.0;
			R[k] = dt*reaction(i, localconc, LinPart, t, stuff);
//			LeaveCriticalSection(&CritSect);

			LinPart = LinPart*dt*0.5;
//			if (false){
//				if (i==2 && k==1){
//					ShowFloat(R[k], "R1");
//					ShowFloat(LinPart, "LinPart");
//				}
//			}

			g1[k]= -eps;
			g2[k]= 1.0 + 2.0*eps + bb - LinPart;
			g3[k]= -eps - bb;

//			if (false){
//			if (i==2){
//				if (k==1){
//					ShowFloat(reaction(i, localconc, LinPart, t, stuff), "reaction(i, localconc, t, stuff);");
//				}
//			}
//			if (DoShow){
//				if (k==2 && i==1){
//					ShowFloat(R[k], "k=2, R[0] start");
//				}
//			}

			R[k] += f[k-1]*eps;
			R[k] += f[k]*(1.0 - 2.0*eps - bb + LinPart);
			R[k] += f[k+1]*(eps + bb);

//			if (DoShow){
//				if (k==2 && i==1){
//					ShowFloat(R[k], "k=2, R[0] after");
//				}
//			}
		}
//	    EnterCriticalSection(&CritSect);
		for (int j=0; j<NComponents; j++){
			localconc[j] = concentrations[j][M-1];
//				ShowFloat(localconc[j], "lc");
		}
//	    LeaveCriticalSection(&CritSect);
		int k=M-1;
		bb=b/r[k];

		LinPart = 0.0;
//	    EnterCriticalSection(&CritSect);
		R[M-1] = dt*reaction(i, localconc, LinPart, t, stuff);
//	    LeaveCriticalSection(&CritSect);
		LinPart *= dt*0.5;

		g1[M-1] = -eps;
		g2[M-1] = 1.0 + 2.0*eps + bb - LinPart;
		g3[M-1] = 0.0;

		R[M-1] += BulkValues[i]*(2.0*eps + 2.0*bb);
		R[M-1] += f[k-1]*eps;
		R[M-1] += f[k]*(1.0 - 2.0*eps - bb + LinPart);

//		tridiag2(g1, g2, g3, R, newconc[i], M, gam[i]);
		tridiag2(g1, g2, g3, R, newconcentrations[i], M, gam[i]);
//	    EnterCriticalSection(&CritSect);
//		for (int k=0; k<M; k++){
//			newconcentrations[i][k] = newconc[i][k];
//		}
//	    LeaveCriticalSection(&CritSect);
		//	ShowFloat(newconcentrations[0][0], "concentrations[0][0]");
	//	ShowFloat(newconcentrations[1][0], "concentrations[1][0]");
	//	ShowFloat(newconcentrations[2][0], "concentrations[2][0]");


}
