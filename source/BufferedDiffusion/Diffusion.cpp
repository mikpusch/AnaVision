
#include "stdafx.h"
#include "BufferedDiffusion.h"
#include <math.h>
#include "../common/simplex.h"
#include "../common/DCompl.h"


double * Diffusion::roots	= NULL;
double * Diffusion::denum	= NULL;
double * Diffusion::alphan	= NULL;



Diffusion::Diffusion(){

	UseFluxCorrectionInLinearApproximation = false;
	UsePredictorInWagnerKeitzerForNonLinearTerms =  false;

	NRootsUsed = 10;

	r0 = 0.5e-3; // half a mm

	rmax = 10e-3; // the outer radius treated as bulk

	DH=9.3e-9; // diffusion constant of protons in m^2/s
	DB = 5e-10; // diffusion constant of buffer (bound and unbound!)

	pHbulk = 7; // bulk pH

	T=1e-3;		// total constant buffer concentration in molar

	KD=1e-7; // is = beta/alpha (in molar)

	alpha = 1e13; // in molar^-2 sec^-1
	beta = KD*alpha;  // in molar^-1 sec^-1

	alpha = 1e10; // in molar^-2 sec^-1
	beta = KD*alpha;  // in molar^-1 sec^-1

	IndependentReactionParameters = 2;	// = 0: alpha and beta
										// = 1: alpha and KD
										// = 2: beta and KD

	M = 100; // 
	DR0 = 20e-6; // r0+M*DR=rmax;
	DRMax = 0.01; // 1 cm
	FactDR = 1;

	ModeToDetermineSliceNumber = 1;		// = 0: fix n, calc. DR;
										// = 1: fix DR, calc. n;

	IH=1e-6; // in Amperers
	IH=0; //1e-6; // in Amperers
	IH=1e-6; // in Amperers

	DT0 = 1e-4;
	DT0 = 1e-5;
	DT0 = 1e-1;
	DTMax=1;
	DTMax=DT0;
	FactDT=1;

	UseWagnerKeizer = true;
	SimpleWagnerKeizer = false;

	ClearArrays();
	ArrayDimension = 0;

	ProtocolType = 0;
	TimeCurrentOn = 0.2;
	TimeCurrentOff = 0.2;
	NPulsesInTrain= 0; // if <=0 : indefinite

	DoRenormalize = false;

	TotalBleft = 1E-3; //  1 mM Buffer left
	TotalBright = 1e-3; // 
	pHleft=1e-7;
	pHright=1e-7;

	AutoCalcBeta = true; // beta = delta total [H] / delta free [H] = 1+KT/(H0+K)^2
	buffercapacextern=1000;
	factorcorrection = 1.0;

	IncludeFixedSurfaceBuffer = false;
	KDFixedSurfaceBuffer = 1e-7;
	AmountFixedSurfaceBufferInMoles=1e-12;
	ThicknessFixedVolume = 10e-6;



}


double RootFunc(double x){
	return x*cot(x)-1.0;
}

double GetRoot(double x1, double x2){
	double y1=RootFunc(x1);
	double y2=RootFunc(x2);
	for (int i=0; i<200; i++){
		double x3=0.5*(x1+x2);
		double y3=RootFunc(x3);
		if (y1*y3>0.0){
			x1=x3;
		}
		else{
			x2=x3;
		}
//		ShowFloat((x1+x2)/2.0, "root");
	}
	return (x1+x2)/2.0;
	
}

void Diffusion::InitRoots(){
	if (roots) return;
	roots = new double [N_MAX_ROOTS];
	alphan =  new double [N_MAX_ROOTS];;
	denum = new double [N_MAX_ROOTS];

	double x=1;

	double dxstep=0.001;
//	double dxstep=1;

	double y=RootFunc(x);

	bool take = false;

	CWaitCursor dummy;

	int r=0;
	roots[r]=0;

	while (true){
		double xnew = x + dxstep;
		double ynew =RootFunc(xnew);

		if ( (y*ynew)<0.0){ // changed sign!
			if (take){
				r++;
				if (r>=N_MAX_ROOTS){
					break;
				}

				roots[r]=GetRoot(x,xnew);
	
			}
			y=ynew;
			take = !take;
		}
		x=xnew;

	}

	alphan[0]=0;
	denum[0]=1;

	for (int i=1; i<N_MAX_ROOTS; i++){
		denum[i] = 1.0/(roots[i]*roots[i]*sin(roots[i]));
	}
}

void Diffusion::ClearArrays(){
	H = NULL; B=NULL;
	r=rightside=b=ah=bh=ch=ab=bb=cb=hnew=bnew=gam=Hold=NULL;
//	Alert0("clear");
}
int Diffusion::GetNumberOfSlices(){
	return GetNumberOfSlices(false);
}
int Diffusion::GetNumberOfSlices(BOOL AssignRVector){
	double rr=r0;
	if (AssignRVector){
		r[0] = rr;
	}
	M=0;
	if (FactDR<1.0) FactDR = 1.0;
	if (FactDT<1.0) FactDT = 1.0;
	double dr=DR0;
	while (true){
		rr += dr;
		if (AssignRVector){
			r[M+1] = rr;
		}
		M++;
		if (rr>=rmax){
			break;
		}
		if (dr<DRMax){
			dr *= FactDR;
		}
	}

/*
	if (ModeToDetermineSliceNumber==1){ //= 1: fix DR, calc. M;
//		Alert0("Fix dr");
		double dm=(rmax-r0)/DR;
		if (dm<1.0) dm=1.1;
		if (dm>MAXMINBUFFEREDDIFFUSION){
			dm = MAXMINBUFFEREDDIFFUSION;
		}
		M=int(dm);
	}

	if (M<1) M=1;
	if (M>MAXMINBUFFEREDDIFFUSION) M=MAXMINBUFFEREDDIFFUSION;
*/
	return M;
}

Diffusion::~Diffusion(){
//	Alert0("delete, call dealloc ");
	DeAlloc();
}
void Diffusion::Shutdown(){
	if (roots){
		delete [] roots;
		roots = NULL;
		delete [] alphan;
		alphan = NULL;
		delete [] denum;
		denum = NULL;
	}
}


void Diffusion::DeAlloc(){

	if (ArrayDimension<1){
//		Alert0("ArrayDimension <1 ");
		return;
	}
	if (H){
//		Alert0("delete H");
		delete [] H;
	}
	if (B) delete [] B;
	if (r) delete [] r;
	if (rightside) delete [] rightside;
	if (b) delete [] b;
	if (ah) delete [] ah;
	if (bh) delete [] bh;
	if (ch) delete [] ch;
	if (ab) delete [] ab;
	if (bb) delete [] bb;
	if (cb) delete [] cb;
	if (hnew) delete [] hnew;
	if (gam) delete [] gam;
	if (Hold) delete [] Hold;
	if (bnew) delete [] bnew;
//	if (bold) delete [] bold;
	ClearArrays();
}

void Diffusion::Alloc(){
//	ShowFloat(ArrayDimension, "ArrayDimension in alloc");
//	bold=new double [ArrayDimension];

	bnew=new double [ArrayDimension];
//	hold=new double [ArrayDimension];
	hnew=new double [ArrayDimension];
	gam=new double [ArrayDimension];
	Hold=new double [ArrayDimension];
	cb=new double [ArrayDimension];
	bb=new double [ArrayDimension];
	ab=new double [ArrayDimension];
	ch=new double [ArrayDimension];
	bh=new double [ArrayDimension];
	ah=new double [ArrayDimension];
	b=new double [ArrayDimension];
	rightside=new double [ArrayDimension];
	r=new double [ArrayDimension];
	B=new double [ArrayDimension];
	H=new double [ArrayDimension];
}
void Diffusion::GetValues(int j, double & Hv, double & Bv, double & BHv){

	if ((j<0) || (j>M) || (j>=ArrayDimension)){
		Hv=this->Hbulk;
		Bv=this->Bbulk;
		BHv=T-Bv;
		return;
	}

	Hv=H[j];
	Bv=B[j];
	BHv=T-Bv;

}

void Diffusion::CopyParams(Diffusion & d){
	d.r0=r0;
	d.rmax = rmax;
	d.DH = DH;
	d.DB = DB;
	d.pHbulk = pHbulk;
	d.T = T;
	d.alpha = alpha;
	d.beta = beta;
	d.KD = KD;
	d.IndependentReactionParameters = IndependentReactionParameters;
	d.M = M; 
	d.DR0 = DR0;
	d.DRMax = DRMax;
	d.FactDR = FactDR;
	d.ModeToDetermineSliceNumber = ModeToDetermineSliceNumber;
	d.IH = IH;
	d.DT0 = DT0;
	d.DTMax = DTMax;
	d.FactDT = FactDT;
	d.UseWagnerKeizer = this->UseWagnerKeizer;
	d.SimpleWagnerKeizer = SimpleWagnerKeizer;

	d.ProtocolType = ProtocolType ;
	d.TimeCurrentOn=TimeCurrentOn;
	d.TimeCurrentOff=TimeCurrentOff ;
	d.NPulsesInTrain=NPulsesInTrain;

	d.DoRenormalize = DoRenormalize;

	d.TotalBleft=TotalBleft; // Buffer left
	d.TotalBright=TotalBright; // 
	d.pHleft=pHleft;
	d.pHright=pHright;
	d.NRootsUsed = NRootsUsed;
	d.AutoCalcBeta = AutoCalcBeta;
	d.buffercapacextern = buffercapacextern;
	d.UseFluxCorrectionInLinearApproximation=UseFluxCorrectionInLinearApproximation;
	d.factorcorrection = this->factorcorrection;

	d.IncludeFixedSurfaceBuffer = IncludeFixedSurfaceBuffer;
	d.KDFixedSurfaceBuffer = KDFixedSurfaceBuffer;
	d.AmountFixedSurfaceBufferInMoles = AmountFixedSurfaceBufferInMoles;
	d.ThicknessFixedVolume = ThicknessFixedVolume;
	d.UsePredictorInWagnerKeitzerForNonLinearTerms = UsePredictorInWagnerKeitzerForNonLinearTerms;


}


BOOL Diffusion::LoadFromFile(CFile * fp){
	ReByNS(r0);
	ReByNS(rmax);
	ReByNS(DH);
	ReByNS(DB);
	ReByNS(pHbulk);
	ReByNS(T);
	ReByNS(alpha);
	ReByNS(beta);
	ReByNS(KD);
	ReByNS(IndependentReactionParameters);
	ReByNS(M );
	ReByNS(DR0);
	ReByNS(ModeToDetermineSliceNumber);
	ReByNS(IH);
	ReByNS(DT0);
	ReByNS(UseWagnerKeizer);
	ReByNS(SimpleWagnerKeizer);
	ReByTRUENS(ProtocolType);
	ReByTRUENS(TimeCurrentOn);
	ReByTRUENS(TimeCurrentOff);
	ReByTRUENS(NPulsesInTrain); // if <=0 : indefinite
	ReByTRUENS(DoRenormalize);
	ReByTRUENS(DRMax);
	ReByTRUENS(FactDR);
	ReByTRUENS(DTMax);
	ReByTRUENS(FactDT);

	ReByTRUENS(TotalBleft);
	ReByTRUENS(TotalBright);
	ReByTRUENS(pHleft);
	ReByTRUENS(pHright);
	ReByTRUENS(NRootsUsed);
	ReByTRUENS(AutoCalcBeta);
	ReByTRUENS(buffercapacextern);
	ReByTRUENS(UseFluxCorrectionInLinearApproximation);

	ReByTRUENS(factorcorrection);

	ReByTRUENS(IncludeFixedSurfaceBuffer); 
	ReByTRUENS(KDFixedSurfaceBuffer); 
	ReByTRUENS(AmountFixedSurfaceBufferInMoles );
	ReByTRUENS(ThicknessFixedVolume );

	return true;
}
BOOL Diffusion::SaveOnFile(CFile * fp){
	WriByNS(r0);
	WriByNS(rmax);
	WriByNS(DH);
	WriByNS(DB);
	WriByNS(pHbulk);
	WriByNS(T);
	WriByNS(alpha);
	WriByNS(beta);
	WriByNS(KD);
	WriByNS(IndependentReactionParameters);
	WriByNS(M);
	WriByNS(DR0);
	WriByNS(ModeToDetermineSliceNumber);
	WriByNS(IH);
	WriByNS(DT0);
	WriByNS(UseWagnerKeizer);
	WriByNS(SimpleWagnerKeizer);
	WriByNS(ProtocolType);
	WriByNS(TimeCurrentOn);
	WriByNS(TimeCurrentOff);
	WriByNS(NPulsesInTrain); // if <=0 : indefinite
	WriByNS(DoRenormalize);	
	WriByNS(DRMax);
	WriByNS(FactDR);
	WriByNS(DTMax);
	WriByNS(FactDT);
	WriByNS(TotalBleft);
	WriByNS(TotalBright);
	WriByNS(pHleft);
	WriByNS(pHright);
	WriByNS(NRootsUsed);
	WriByNS(AutoCalcBeta);
	WriByNS(buffercapacextern);
	WriByNS(UseFluxCorrectionInLinearApproximation);

	WriByNS(factorcorrection);

	WriByNS(IncludeFixedSurfaceBuffer); 
	WriByNS(KDFixedSurfaceBuffer); 
	WriByNS(AmountFixedSurfaceBufferInMoles );
	WriByNS(ThicknessFixedVolume );

	return true;
}
BOOL Diffusion::LoadFromFile2(CFile * fp){
	ReByNS(factorcorrection);
	ReByNS(IncludeFixedSurfaceBuffer);
	ReByNS(KDFixedSurfaceBuffer);
	ReByNS(AmountFixedSurfaceBufferInMoles);
	return true;
}
BOOL Diffusion::SaveOnFile2(CFile * fp){
	WriByNS(factorcorrection);
	WriByNS(IncludeFixedSurfaceBuffer);
	WriByNS(KDFixedSurfaceBuffer);
	WriByNS(AmountFixedSurfaceBufferInMoles);
	return true;
}

BOOL Diffusion::LoadFromFile3(CFile * fp){
	ReByNS(UsePredictorInWagnerKeitzerForNonLinearTerms);
	return true;
}
BOOL Diffusion::SaveOnFile3(CFile * fp){
	WriByNS(UsePredictorInWagnerKeitzerForNonLinearTerms);
	return true;
}


double Diffusion::GetProtonCurrent(double t /* in seconds */ ){ // in Amperes; usually returns just IH.
	if (this->ProtocolType==0){
		return IH;
	}
	double tot=TimeCurrentOn+TimeCurrentOff;
	int n = int(t/tot);
	if (this->NPulsesInTrain>0){
		if (n>=this->NPulsesInTrain){
//			ShowFloat(t, "t0");
			return 0.0;
		}
	}
	double start=t-double(n)*tot;
//	if (start<TimeCurrentOn){
	if (start<TimeCurrentOn){
		return 0.0;
	}
	return IH;
}



double Diffusion::GetFreeBuffer(double ch ){ // calc free buffer from proton concentration
	return KD*T/(ch+KD);
}

double Diffusion::CalcTotalProtons(double alpha){
	double sum = 0.0;
	int i;
	for (i=0; i<M; i++){
		double r1=r[i];
		double r2=r[i+1];
		double V=4188.79020479*( r2*r2*r2-r1*r1*r1 ); // in liters
		double h=H[i];
		double hh = h + alpha*(h-Hbulk);
		double bb = GetFreeBuffer(hh);
		sum += (hh+(T-bb))*V; 
	}
	return sum;
}
double Diffusion::GetMaxTime(int iter){
	double t=0;
	double dt = DT0;
	for (int i=0; i<iter; i++){
		t += dt;
		if (dt<DTMax){
			dt *= FactDT;
		}
	}

	return t;
}
void Diffusion::Setup(){
	DoSetup(1);
}

void Diffusion::SetupLinearized2(){
	DoSetup(2);
	int i;
	for (i=0; i<=M; i++){
		hnew[i] = (Hbulk + KD)/r[i];
	}

}

void Diffusion::DoSetup(int mode){
	UseLinearized = false;
	UseLinearized2 = false;
	if (mode==0){
		UseLinearized = true;
	}
	if (mode==2){
		UseLinearized2 = true;
	}
//	UseLinearized = linear;
	int i;
//	if (UseLinearized2){
//		Alert0("hi0");
//	}

	time = 0.0;
	DT = DT0;
	ChargeTransferred = 0.0;

	M=this->GetNumberOfSlices(false);
//	DR=(this->rmax-this->r0)/double(M);


//	ShowFloat(M, "M");
//	ShowFloat(ArrayDimension, "ArrayDimension");
	if (M>=ArrayDimension){
//		Alert0("Deallloc kkkddkkkkd");
		DeAlloc();
		ArrayDimension = M+1;
		Alloc();
	}
	GetNumberOfSlices(true);

	switch (IndependentReactionParameters) {
		case 0	: KD = beta/alpha; break;
		case 1	: beta = alpha*KD; break; // = 1: alpha and KD
		case 2	: alpha = beta/KD; break; // = 2: beta and KD
		default	: KD = beta/alpha; break;
	}

	Hbulk = pow(10.0, -pHbulk);
	Bbulk = GetFreeBuffer(Hbulk);
//	ShowFloat(Bbulk, "Bbulk");
	double r1=r[1];

//	V0 = 4.0*PIGRECO*DR*(r0*r0+r0*DR+DR*DR/3.0)*1000.0 ; // in liters and not m^3
	V0 = 4.0*PIGRECO/3.0*(r1*r1*r1-r0*r0*r0)*1000.0 ; // in liters and not m^3
//	ShowFloat(PIGRECO, "PIGRECO");
	for (i=0; i<=M; i++){
		H[i] = hnew[i] = Hbulk;
		B[i] = bnew[i] = Bbulk;
		rightside[i]=b[i]=ah[i]=bh[i]=ch[i]=ab[i]=bb[i]=cb[i]=0.0; 
//		r[i] = r0 + double(i)*DR - DR/2.0;
//		r[i] = r0 + double(i)*DR ; //+ DR;
//		ShowFloat(r[i], "r[i]");
	}

	if (IncludeFixedSurfaceBuffer){
		double r1=r0+this->ThicknessFixedVolume;
		VolSurf = 4.0*PIGRECO/3.0*(r1*r1*r1-r0*r0*r0)*1000.0 ; // in liters and not m^3
		TotalFixedBuffer = this->AmountFixedSurfaceBufferInMoles/VolSurf; // in molar
//		FreeFixedBuffer = KDFixedSurfaceBuffer*TotalFixedBuffer/(Hbulk+KDFixedSurfaceBuffer);
		HSurface = Hbulk;
		//ShowFloat(TotalFixedBuffer, "TotalFixedBuffer");
		//ShowFloat(FreeFixedBuffer, "FreeFixedBuffer");

	}



	TotalInitialProtons = CalcTotalProtons(0.0);

	if ((this->UseWagnerKeizer) || UseLinearized || UseLinearized2){
		for (i=0; i<=M; i++){
			hnew[i] = Hbulk + KD;
		}

//		Alert0("hi");
//		ShowFloat(hnew[0], "h0");
		//CalcRightSideAndMatrixWagnerKeizer(0.0);
		return ;
	}


	it = 1.0/DT;
	bbb = beta*T;


	for (i=0; i<=M; i++){

		double DR;

		if (i==0){
			DR = DR0;
		}
		else{
			DR = r[i]-r[i-1];
		}

		double d2 = 1.0/(DR*DR);
		double d2half = d2/2.0;

		double dm = 1.0/(DR*r[i]);
		double d3 = d2     + dm;
		double d4 = d2half + dm;

		if (i==0){
			bh[i]=it + DH*d4; 
			bb[i]=it + DB*d4;
		}
		else{
			ah[i] = -DH*d2half;
			ab[i] = -DB*d2half;
			bh[i] = it + DH*d3; 
			bb[i] = it + DB*d3;
		}
		if (i<M){
			ch[i] = -DH*d4;
			cb[i] = -DB*d4;
		}
	}

	CalcRightHandSides();
}


// These procedures perform one integration step

void Diffusion::Step(bool actuallyused){; // Integrates one time step;

	double cur = GetProtonCurrent(time);
	double charge = cur*DT;
	ChargeTransferred += charge;

	phi = charge/FARADAY;
	if (UseWagnerKeizer){
		if (IncludeFixedSurfaceBuffer){
			double phistar = phi/VolSurf;
			phistar = GetBufferedPhiFxedSurfaceBuffer(phistar);
			phi = phistar * VolSurf;
		}
	}

	phi /= V0;


//	phi = charge/FARADAY/V0;

//	if (UseLinearized2){
//		Alert0("hi");
//	}
	if (UseLinearized){
		if (actuallyused){
			StepLinearized();
		}
	}
	else{
		if (UseLinearized2){
			phi = GetBufferedPhi(phi);

			StepLinearized2();
		}
		else{
			if (UseWagnerKeizer){
				phi = GetBufferedPhi(phi); //+KD+H[0];
				if (SimpleWagnerKeizer){
					SimpleStepWagnerKeizer();
				}
				else{
					StepWagnerKeizer();
				}
			}
			else{
					CalcRightHandSides();
					tridiag(ah, bh, ch, rightside, hnew, M+1);
					tridiag(ab, bb, cb, b, bnew, M+1);
					hnew[0] += phi;

					for (int i=0; i<=M; i++){
						H[i] = hnew[i];
						B[i] = bnew[i];
					}
				}
		}
	}

	time += DT;
//	ShowFloat(time, "time");
//	ShowFloat(DT, "DT");
	if (DT<DTMax){
		DT *= FactDT;
	}
}

//void Diffusion::Step( int n){
//	for (int i=0; i<n; i++){
//		Step();
//	}
//}

	
void Diffusion::CalcRightHandSides(){
	int i;
 

//	double A=4*PIGRECO*r0*r0;
//	double hzeroprime = cur/A/DH/FARADAY/1000.0;

	for (i=0; i<=M; i++){

		double DR;

		if (i==0){
			DR = DR0;
		}
		else{
			DR = r[i]-r[i-1];
		}

		double d2 = 1.0/(DR*DR);
		double d2half = d2/2.0;


		double dm = 1.0/(DR*r[i]);
		double d3 = dm + d2;
		double d4 = d2half + dm;

		double Bim1, Him1;

		if (i==0){
			Him1=H[0];
//			Him1=H[0]+hzeroprime*DR;
			Bim1=B[0];
		}
		else{
			Him1=H[i-1];
			Bim1=B[i-1];
		}

		double Hip1, Bip1;

		if (i==M){
			Hip1=Hbulk;
			Bip1=Bbulk;
		}
		else{
			Hip1=H[i+1];
			Bip1=B[i+1];
		}

		rightside[i] = Him1*DH*d2half + H[i]*(it-DH*d3-alpha*B[i]) + Hip1*DH*d4 + beta*(T-B[i]);
		b[i] = Bim1*DB*d2half + B[i]*(it-DB*d3-alpha*H[i]) + Bip1*DB*d4 + beta*(T-B[i]);

//		if (i==0){
//			rightside[i] += phi;
//		}
		if (i==M){
			rightside[i] += DH*Hbulk*d4;
			b[i] += DB*Bbulk*d4;
		}

	}
}


void Diffusion::StepWagnerKeizer(){

	int i;

	if (this->UsePredictorInWagnerKeitzerForNonLinearTerms){
		for (i=0; i<=M; i++){
			Hold[i]=H[i];
		}
		CalcRightSideAndMatrixWagnerKeizerIncludePredictor(false);
		tridiag2(ah, bh, ch, rightside, hnew, M+1, gam);
//		hnew[0] += phi;
		CalcRightSideAndMatrixWagnerKeizerIncludePredictor(true);
		tridiag2(ah, bh, ch, rightside, hnew, M+1, gam);
		hnew[0] += phi;
	}
	else{
		CalcRightSideAndMatrixWagnerKeizer();
		tridiag2(ah, bh, ch, rightside, hnew, M+1, gam);
		double H0=hnew[0]-KD;
		H[0]=H0;
//	ShowFloat(H0, "H0");
		double myphi = phi;
	
//	myphi = GetBufferedPhi(phi);

		hnew[0] += myphi;
	}

//	tridiag(ah, bh, ch, rightside, hnew, M+1);

	for (i=0; i<=M; i++){
		H[i] = hnew[i]-KD;
		if (H[i]<0){
//			ShowFloat(H[i], "H <0");
			H[i]=0;
		}

		B[i] = GetFreeBuffer(H[i]);
	}

	if (DoRenormalize){
		if (ChargeTransferred != 0.0){
			double ChangeInMoleProton = CalcTotalProtons(0.0)-TotalInitialProtons;
			double ChargeProton = ChangeInMoleProton*FARADAY;
			double scale = ChargeProton / ChargeTransferred;
			if (fabs(scale-1.0)>0.001){
				RenormalizeH();
			}
			//	for (i=0; i<=M; i++)
		}
	}
}
void Diffusion::StepLinearized2(){

	CalcRightSideAndMatrixLinearized2();

	tridiag(ah, bh, ch, rightside, hnew, M+1);

	int i;

	for (i=0; i<=M; i++){
		H[i] = hnew[i]*r[i]-KD;
		if (H[i]<0){
//			ShowFloat(H[i], "H <0");
			H[i]=0;
		}

		B[i] = GetFreeBuffer(H[i]);
	}


	H[0] += phi;
	hnew[0] = (H[0]+KD)/r[0];

	B[0] = GetFreeBuffer(H[0]);

}

double Diffusion::GetBufferedPhiWithFixedSurfaceBuffer(double DELTA){
	double H0=H[0];
	double a=H0+KD;
	double A = KD*T/a;
	double b=H0+KDFixedSurfaceBuffer;
	double B = KDFixedSurfaceBuffer * TotalFixedBuffer/b;

	double r= a+b+A+B-DELTA;
	double s= a*b+A*b+a*B-DELTA*(a+b);
	double t = -DELTA*a*b;

	dcomplex roots[10];

	SolveCubic(r, s, t, roots);

	BOOL RealNeg[3];
	int i;
	bool foundpos = false;
	int firstpos = 0;
	for (i=0; i<3; i++){
		RealNeg[i]=false;
		if (roots[i].r<0.0){
			RealNeg[i]=true;
		}
		else{
			if (!foundpos){
				foundpos = true;
				firstpos = i;
			}
		}
	}

	if (!foundpos){
		for (i=0; i<3; i++){
			RealNeg[i]=false;
		}
		firstpos=0;
	}

	int minr=firstpos;
	double minimag=fabs(roots[firstpos].i);
	if (firstpos<2){
		for (int i=(firstpos+1); i<3; i++){
			double tp1 = fabs(roots[i].i);
			if (!RealNeg[i]){
				if (tp1<minimag){
					minimag = tp1;
					minr = i;
				}
			}
		}
	}
	double result = roots[minr].r;

//	ShowFloat(DELTA, "DELTA");
//	ShowFloat(result, "delta");
	return result;
}
double Diffusion::GetBufferedPhiFxedSurfaceBuffer(double phi){
//	ShowFloat(phi, "phi");
//	ShowFloat(HSurface, "HSurface");
	double H0=HSurface;

//	ShowFloat(-log10(H0), "HSurface");
	double h0=H0+KDFixedSurfaceBuffer;
	double B0=KDFixedSurfaceBuffer*TotalFixedBuffer/h0;

	double A=phi-B0;
	double p=(h0-A)/2.0;
	double q=KDFixedSurfaceBuffer*TotalFixedBuffer+A*h0;

	double x=sqrt(p*p + q)- p;
	HSurface += x;
//	ShowFloat(x, "x");
	return x;
}


double Diffusion::GetBufferedPhi(double phi){
//	if (this->IncludeFixedSurfaceBuffer){
//		phi = GetBufferedPhiFxedSurfaceBuffer(phi);
//		return GetBufferedPhiWithFixedSurfaceBuffer(phi);
//	}
	double H0=H[0];
	double h0=H0+KD;
	double B0=GetFreeBuffer(H0);
	double A=phi-B0;
	double p=(h0-A)/2.0;
	double q=KD*T+A*h0;

	double x=sqrt(p*p + q)- p;
//	ShowFloat(phi, "phi");
//	ShowFloat(x, "x");
	return x;
}

void Diffusion::CalcRightSideAndMatrixWagnerKeizer(){
	int i;


//	double A=4*PIGRECO*r0*r0;

//	double hzeroprime = cur/A/DH/FARADAY/1000.0;

	double kk=KD*T;
	double DK = kk*DB;

//	for (i=0; i<=M; i++){
//		hnew[i] = H[i] + KD;
//	}

	for (i=0; i<=M; i++){

		double DR;

		if (i==0){
			DR = DR0;
		}
		else{
			DR = r[i]-r[i-1];
		}

		double idr = 1.0/DR;
		double sc1 = DT/DR/DR;
		double sc2 = DT/DR/2.0;


		double hprime;
		if (i<M){
//			if (i<0){
//				hprime = -hzeroprime;
//			}
//			else{
				hprime = (H[i+1]-H[i])*idr;
//			}
		}
		else{
			hprime = (Hbulk-H[M])*idr;
		}

		double tp1=hnew[i];
		double tp2=tp1*tp1;
		double tp3 = 1.0/(kk+ tp2);
		double ee = (DH*tp2+DK)*tp3;
		double ff = -2.0*DK*hprime*tp3/tp1;
		double gg = 2.0*ee/r[i] + ff;
		ee *= sc1;
		gg *= sc2;

		double ehalf = ee/2.0;


		if (i==0){
			ah[i]=0;
			bh[i]=1.0 + gg + ehalf; 
//			bh[i]=1.0 + gg + ee; 
		}
		else{
			ah[i] = -ehalf;
			bh[i] = 1.0+ ee +gg; 
		}
		if (i<M){
			ch[i] = -gg-ehalf;
		}
		else{
			ch[i] = 0;
		}


		double Him1;
		double Hi=hnew[i];
		rightside[i] = 0;

		if (i==0){
//			rightside[0] = phi;
//			ShowFloat(phi, "phi");
//			Him1=hnew[0]+hzeroprime*DR;
			Him1=hnew[0];
//			Him1=phi;
//			Hi=phi;
//			rightside[i] = hzeroprime*DR*ehalf;

		}
		else{
			Him1=hnew[i-1];
//			if (i==1){
//				Him1 = phi;
//			}
		}

		double Hip1;

		if (i==M){
			Hip1=Hbulk+KD;
			rightside[i] += (Hbulk+KD)*(ehalf+gg);
		}
		else{
			Hip1=hnew[i+1];
//			if (i==0){
//				Hip1 += phi;
//			}
		}

		rightside[i] += Hi*(1.0-ee-gg) + Hip1*(ehalf+gg) + Him1*ehalf ;
	}
}


void Diffusion::CalcRightSideAndMatrixWagnerKeizerIncludePredictor(bool UseOld){
}



void Diffusion::CalcRightSideAndMatrixLinearized2(){
	int i;

//	for (i=0; i<=M; i++){
//		hnew[i] = (H[i] + KD)/r[i];
//	}

	double rmax = r[M];

	for (i=0; i<=M; i++){

		double DR;

		if (i==0){
			DR = DR0;
		}
		else{
			DR = r[i]-r[i-1];
		}


		double sc3 = DB*DT/DR/DR;
		double sc4 = -sc3/2.0;

		bh[i]= 1.0 + sc3; 

		if (i==0){
			ah[i]=0;
			bh[i]= 1.0 - sc4; 
		}
		else{
			ah[i] = sc4;
		}

		if (i<M){
			ch[i] = sc4;
		}
		else{
			ch[i] = 0;
		}


		double Him1;

		double Hi=hnew[i];

		rightside[i] = 0;

		if (i==0){
			Him1=hnew[0];
//			rightside[i] += (Hbulk+KD)/r[0]*sc3/2;
//			Him1=0;
		}
		else{
			Him1=hnew[i-1];
		}

		double Hip1;

		if (i==M){
			Hip1=(Hbulk+KD)/rmax;
			rightside[i] += (Hbulk+KD)/rmax*sc3/2;
		}
		else{
			Hip1=hnew[i+1];
		}

		rightside[i] += (  Hi*(1.0-sc3) - (Hip1+ Him1)*sc4)  ;
	}
}

void Diffusion::SimpleStepWagnerKeizer(){
	int i;

//	double A=4*PIGRECO*r0*r0;
//	double hzeroprime = cur/A/DH/FARADAY/1000.0;

	double kk=KD*T;
	double DK = kk*DB;

	for (i=0; i<=M; i++){
		hnew[i] = H[i] + KD;
	}


	for (i=0; i<=M; i++){

		double DR;

		if (i==0){
			DR = DR0;
		}
		else{
			DR = r[i]-r[i-1];
		}

		double idr = 1.0/DR;
		double sc1 = DT/DR/DR;
		double sc2 = DT/DR;

		double hprime;
		if (i<M){
			hprime = (H[i+1]-H[i])*idr;
		}
		else{
			hprime = (Hbulk-H[M])*idr;
		}

		double tp1=hnew[i];
		double tp2=tp1*tp1;
		double tp3 = 1.0/(kk+ tp2);
		double ee = (DH*tp2+DK)*tp3;
		double ff = -2.0*DK*hprime*tp3/tp1;
		double gg = 2.0*ee/r[i] + ff;
		ee *= sc1;
		gg *= sc2;

		double Him1;

		rightside[i] = 0;
		if (i==0){
//			rightside[i] = phi;
//			rightside[i] = 0;
			Him1=hnew[0]; //+hzeroprime*DR;
		}
		else{
			Him1=hnew[i-1];
		}

		double Hip1;

		if (i==M){
			Hip1=Hbulk+KD;
		}
		else{
			Hip1=hnew[i+1];
		}

		rightside[i] += hnew[i]*(1-2.0*ee-gg) + Hip1*(ee+gg) + Him1*ee ;
	}
	rightside[0] += phi;

	for (i=0; i<=M; i++){
		H[i] = rightside[i]-KD;

		B[i] = GetFreeBuffer(H[i]);
	}



}


void Diffusion::RenormalizeH(){
	Simplex simp;
	int MaxIter = 100;
	double StepSize = 0.2;
	double SD;
	double BestSD = 1e300;
	double Params[2], BestParams[2];
	Params[0] = BestParams[0] = 0.1;
	simp.InitSimplex(StepSize, Params, 1);
	for (int iter=0; iter<MaxIter; iter++){
		double ChangeInMoleProton = CalcTotalProtons(Params[0])-TotalInitialProtons;
		double ChargeProton = ChangeInMoleProton*FARADAY;
		double scale = (ChargeProton - ChargeTransferred)/ChargeTransferred;
		SD = scale*scale;

		if (SD<BestSD){
			BestParams[0] = Params[0];
			BestSD = SD;
		}

		simp.NextSimp(Params, SD);
	}

	double alpha = BestParams[0];
//	ShowFloat(alpha, "alpha");

	for (int i=0; i<=M; i++){
		H[i] = H[i] + alpha*(H[i]-Hbulk);
		B[i] = GetFreeBuffer(H[i]);
	}

}


double Diffusion::GetAddedProtonConcInMolar(double H){
// for a given change of Hbulk to H and given T, calc. the conc. of added (or subracted)
// protons

	return (H-Hbulk+KD*T*(1.0/(Hbulk+KD)-1.0/(H+KD)));
}


void Diffusion::SetupLinearized(){
	DoSetup(0);

//	if (!roots){
//		this->InitRoots();
//	}
//	int i;
//	for (i=1; i<N_MAX_ROOTS; i++){
//		alphan[i] = roots[i]/r0;
//	}

	double H0 = Hbulk;
	double B0 = GetFreeBuffer(H0);
	double lambda = B0/(H0 + KD);
	Deff = (DH+lambda*DB)/(1+lambda);

//	ShowFloat(Deff, "Deff");

	double cur = IH; // in A
	double f= cur/FARADAY; // in moles / sec
	double A= 4*PIGRECO*r0*r0; // surface area in m^2
	J = f/A; // in moles/sec/m^2

//	ShowFloat(J*r0*r0, "J*r0*r0");

//	prefactor = J*r0*r0/Deff; // in moles/cubic meter
//	prefactor /= 1000.0; // in moles / liter

	beta0 = this->buffercapacextern;

	if (this->AutoCalcBeta){
		beta0 = 1.0 + KD*T/(H0+KD)/(H0+KD);
	}

//	prefactor /= buffercapac; // in moles / liter


//	ShowFloat(buffercapac, "buffercapac");

//	ShowFloat(prefactor, "pf");


}

double Diffusion::CalcLinearized(double t, double r){
	double sum = 0.0;
	for (int i=1; i<NSum; i++){
		double alpha = alphan[i];
		double tp1=sin(alpha*r)*exp(-Deff*alpha*alpha*t)*denum[i];;
//		ShowFloat(tp1, "tp1");
		sum += sin(alpha*r)*exp(-Deff*alpha*alpha*t)*denum[i];
	}
	double a2=r0*r0;

	return prefactor*(3*Deff*t/a2 + r*r/2.0/a2 - 0.3 - 2.0*r0*sum/r);
}

double Diffusion::Solu2(double J, double beta0, double D, double a, double r, double t){
	double s=1.0/a;
//	ShowFloat(s, "s");
	double prefactor = J*a/D; // in moles/cubic meter
	prefactor /= (1000.0*beta0); // in moles / liter

	if (this->UseFluxCorrectionInLinearApproximation){
//		Beep(1000,100);
//		double tp1 = 2.0*J*(Hbulk+KD)/KD/T/1000/D;
		double tp1 = factorcorrection*2.0*J*(Hbulk+KD)/KD/T/1000/D;
		//		double tp1 = 1.0*J*(Hbulk+KD)/KD/T/1000/D;
//		ShowFloat(a*tp1, "a*tp1");
		s -= tp1;
	}

	prefactor /= s;


	double argr=r-a;
	double Dt = D*t;
	double root = sqrt(Dt);

	double arg1=argr/2.0/root;
	double erf1;

#define SIMPLEERF true
	if (SIMPLEERF){
		erf1=erfcc(arg1);
	}
	else{
		erf1= erffc1(arg1);
	}


	double arg2=arg1+s*root;
	double erf2;

	if (SIMPLEERF){
		erf2=erfcc(arg2);
	}
	else{
		erf2=erffc1(arg2);
	}

	double result = prefactor/r*(erf1-erf2*exp(s*argr+Dt*s*s));
//	ShowFloat(r, "r");
//	ShowFloat(t,"t");
//	ShowFloat(erf1, "erf1");
//	ShowFloat(erf2, "erf2");

//	ShowFloat(result, "result");
//	ShowFloat(J/D, "J/D");
//	ShowFloat(J/D/r, "J/D/r");
	return result;

}

void Diffusion::StepLinearized(){

	int i;

	double t= time;
//	ShowFloat(t, "t");
	double a=r0;

	for (i=0; i<=M; i++){

		H[i] = this->Hbulk + Solu2(J, beta0, Deff, a, r[i], t);
//		ShowFloat(r[i], "r");
//		ShowFloat(CalcLinearized(t, r[i]), "lin");

		B[i] = GetFreeBuffer(H[i]);
	}



}




