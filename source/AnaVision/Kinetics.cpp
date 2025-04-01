

#include "stdafx.h"
#include "AnaVision.h"
#include <math.h>


Kinetics::Kinetics(){
	AssociatedIndexes = NULL;

	a=0.5e-3; // default size of oocyte

	DeltaShow = 1;

	pixelsize = 9.9e-6/10.0; // default pixelsize / objective magnific.
	beta = 1000.0; // change total [H] / change of free [H]  

	Current = 1e-6;
	FitCurrent = true;

	NCoeffOffFocusCorrection = 10;
	HalfWidthOffFocusCorrectionInMeters = 326*1e-6; // micrometers;
	HalfWidthOffFocusCorrectionInMeters = 300*1e-6; // micrometers;
	UseOffFocusCorrection = true;
	CalcCoeffeOffFocusCorrection(500e-6);

	D=5e-10;
	HasBeenFitted = false;
	HasBeenConstructed = false; 
	IndexRMin=0;
	IndexRMax=1;		// index of radii to be analyzed: the absolute value of the radius is
		// r = a + indexr*pixelsize;
	DeltaRIndex = 1;


	IndexTimeMin=0;
	IndexTimeMax=1;
	DeltaTimeIndex=1;
	
	dt=0.1; // seconds

	BackgroundTimeIndex0=0;
	BackgroundTimeIndex1=1;

	TrackDrift = false;

	ShowAsFunctionOfTime=true; // else ShowAsFunctionOfRadius

	data=NULL;
	fit=NULL;
	tempfit=NULL;
	SizeR = SizeT = 0;
	NDataR = NDataT = 0;
	ConvertToDeltaHTotal = true;
	FitD=false;

	FitTotalFree=false;
	FitAmountFixed = true;
	FitKFree=false;
	FitKFixed = true;

	DH = 9.3e-9;
	AmountFixed = 1e-12; // moles
	KFixed = 1e-7;
	ThicknessFixedBufferVolume = 10e-6;

	DR=0.5e-6;
	DT0=1e-6;
	factdt=1.001;
	DTMAX=1e-4;
	RMaxSimulation = 1.2e-3;

	MaxIterSimulFit = 100;
	StepSize = 0.2;

	FitThicknessFixedBufferVolume = false;



}


void Kinetics::Alloc(){
	if ((NDataR<=SizeR) && (NDataT<=SizeT)){
		//Alert0("hi");
		return;
	}
	DeAlloc();
	SizeT = NDataT;
	SizeR = NDataR;
	data = dmatrix(0, SizeT-1, 0, SizeR-1);
	fit = dmatrix(0, SizeT-1, 0, SizeR-1);
	tempfit = new double[SizeT];


}
void Kinetics::DeAlloc(){
//	Alert0("hi");
	if (tempfit){
		delete [] tempfit;
	}
	if (fit){
		free_dmatrix(fit, 0, SizeT-1, 0, SizeR-1);
	}
	if (data){
		free_dmatrix(data, 0, SizeT-1, 0, SizeR-1);
	}
	data=fit=NULL;
	tempfit=NULL;
}
void Kinetics::ClearArrays(){
	data=fit=NULL;
	tempfit=NULL;
	d.ClearArrays();
}
Kinetics::~Kinetics(){
	DeAlloc();
}

void Kinetics::CalcCoeffeOffFocusCorrection(double radiusinmeters){
	if (NCoeffOffFocusCorrection<1){
		CoeffsOffFocusCorrection[0] = 1;
		return;
	}

	int i;
	double sumcoeff = 0;
	for (i=0; i<=NCoeffOffFocusCorrection; i++){
		double r = double(i)*radiusinmeters/double(NCoeffOffFocusCorrection);
		double tp1=r/HalfWidthOffFocusCorrectionInMeters;
		CoeffsOffFocusCorrection[i]=exp(-tp1*tp1);
		sumcoeff += CoeffsOffFocusCorrection[i];
		if (i>0){
			sumcoeff += CoeffsOffFocusCorrection[i];
		}
	}
	CString c;
	char s[20];
	for (i=0; i<=NCoeffOffFocusCorrection; i++){
		CoeffsOffFocusCorrection[i]/= sumcoeff;
//		ShowFloat(i, "i");
//		ShowFloat(CoeffsOffFocusCorrection[i], "CoeffsOffFocusCorrection[i]");
		_gcvt(CoeffsOffFocusCorrection[i], 8, s);
		c += s;
		c += " ";
	}
//	Alert0(c);


}
bool Kinetics::ReadFromFile(CFile * fp, int bytesavailable){
	BOOL CheckBytesAvailable=true;
	if (bytesavailable<1){
		bytesavailable = 90000000;
		CheckBytesAvailable = false;
	}

//	ShowFloat(bytesavailable, "bytesavailable");
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	AssociatedIndexes = NULL;
	data = NULL;
	fit = NULL;
	SizeR=SizeT=0;


	MyReadVariable(beta);
	MyReadVariable(Current);
	MyReadVariable(FitCurrent);
	MyReadVariable(D);
	MyReadVariable(FitD);

	MyReadVariable(HasBeenFitted);
	MyReadVariable(FitType);
	MyReadVariable(HasBeenConstructed);

	MyReadVariable(a);
	MyReadVariable(pixelsize); 

	MyReadVariable(IndexRMin);
	MyReadVariable(IndexRMax);
	MyReadVariable(DeltaRIndex);

	MyReadVariable(IndexTimeMin);
	MyReadVariable(IndexTimeMax);
	MyReadVariable(DeltaTimeIndex);
	MyReadVariable(dt);
	MyReadVariable(BackgroundTimeIndex0);
	MyReadVariable(BackgroundTimeIndex1);

	MyReadVariable(DeltaShow);
	MyReadVariable(TrackDrift);

	MyReadVariable(ShowAsFunctionOfTime);

	MyReadVariable(ConvertToDeltaHTotal);

	MyReadVariable(NDataR);
	MyReadVariable(NDataT);

//	MyReadVariable(SizeR);
//	MyReadVariable(SizeT);

	MyReadVariable(InitialpH);
	MyReadVariable(DH);
	MyReadVariable(TotalFree);
	MyReadVariable(AmountFixed);
	MyReadVariable(KFree);
	MyReadVariable(KFixed);
	MyReadVariable(FitTotalFree);
	MyReadVariable(FitAmountFixed);
	MyReadVariable(FitKFree);
	MyReadVariable(FitKFixed);

	MyReadVariable(MaxIterSimulFit);
	MyReadVariable(NParamsSimulFit);
	MyReadVariable(SDSimulFit);
	MyReadVariable(BestSDSimulFit);
	int i;
	for (i=0; i<100; i++){
		MyReadVariable(ParamsSimulFit[i]);
	}
	for (i=0; i<100; i++){
		MyReadVariable(BestParamsSimulFit[i]);
	}
	MyReadVariable(StepSize);


	MyReadVariable(DR);
	MyReadVariable(DT0);
	MyReadVariable(factdt);
	MyReadVariable(DTMAX);


	if ( (NDataR>0) && (NDataT>0)){
		Alloc();
//		SizeT = NDataT;
//		SizeR = NDataR;
//		data = dmatrix(0, SizeT-1, 0, SizeR-1);
//		fit = dmatrix(0, SizeT-1, 0, SizeR-1);

		for (int it=0; it<NDataT; it++){
			for (int ir=0; ir<NDataR; ir++){
				MyReadVariable(data[it][ir]);
				MyReadVariable(fit[it][ir]);
			}
		}
	}
	MyReadVariableTrue(ThicknessFixedBufferVolume);
	MyReadVariable(FitThicknessFixedBufferVolume);
	MyReadVariable(RMaxSimulation);

	if (bytesavailable==0) return true;
	MyReadVariable(NCoeffOffFocusCorrection);

	if (bytesavailable==0) return true;
	MyReadVariable(HalfWidthOffFocusCorrectionInMeters);

	if (bytesavailable==0) return true;
	MyReadVariable(UseOffFocusCorrection);

	if (CheckBytesAvailable){
		while (bytesavailable>0){
			char c;
			MyReadVariable(c);
		}
	}
	return true;
}

int Kinetics::SaveOnFile(CFile * fp){ // returns the number of bytes written
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	int byteswritten = 0;

	MyWriteVariable(beta);
	MyWriteVariable(Current);
	MyWriteVariable(FitCurrent);
	MyWriteVariable(D);
	MyWriteVariable(FitD);

	MyWriteVariable(HasBeenFitted);
	MyWriteVariable(FitType);
	MyWriteVariable(HasBeenConstructed);

	MyWriteVariable(a);
	MyWriteVariable(pixelsize); 

	MyWriteVariable(IndexRMin);
	MyWriteVariable(IndexRMax);
	MyWriteVariable(DeltaRIndex);

	MyWriteVariable(IndexTimeMin);
	MyWriteVariable(IndexTimeMax);
	MyWriteVariable(DeltaTimeIndex);
	MyWriteVariable(dt);
	MyWriteVariable(BackgroundTimeIndex0);
	MyWriteVariable(BackgroundTimeIndex1);

	MyWriteVariable(DeltaShow);
	MyWriteVariable(TrackDrift);

	MyWriteVariable(ShowAsFunctionOfTime);

	MyWriteVariable(ConvertToDeltaHTotal);

	MyWriteVariable(NDataR);
	MyWriteVariable(NDataT);

//	MyWriteVariable(SizeR);
//	MyWriteVariable(SizeT);

	MyWriteVariable(InitialpH);
	MyWriteVariable(DH);
	MyWriteVariable(TotalFree);
	MyWriteVariable(AmountFixed);
	MyWriteVariable(KFree);
	MyWriteVariable(KFixed);
	MyWriteVariable(FitTotalFree);
	MyWriteVariable(FitAmountFixed);
	MyWriteVariable(FitKFree);
	MyWriteVariable(FitKFixed);

	MyWriteVariable(MaxIterSimulFit);
	MyWriteVariable(NParamsSimulFit);
	MyWriteVariable(SDSimulFit);
	MyWriteVariable(BestSDSimulFit);
	int i;
	for (i=0; i<100; i++){
		MyWriteVariable(ParamsSimulFit[i]);
	}
	for (i=0; i<100; i++){
		MyWriteVariable(BestParamsSimulFit[i]);
	}
	MyWriteVariable(StepSize);


	MyWriteVariable(DR);
	MyWriteVariable(DT0);
	MyWriteVariable(factdt);
	MyWriteVariable(DTMAX);

	if ( (NDataR>0) && (NDataT>0)){

		for (int it=0; it<NDataT; it++){
			for (int ir=0; ir<NDataR; ir++){
				MyWriteVariable(data[it][ir]);
				MyWriteVariable(fit[it][ir]);
			}
		}
	}
	MyWriteVariable(ThicknessFixedBufferVolume);
	MyWriteVariable(FitThicknessFixedBufferVolume);
	MyWriteVariable(RMaxSimulation);

	MyWriteVariable(NCoeffOffFocusCorrection);
	MyWriteVariable(HalfWidthOffFocusCorrectionInMeters);
	MyWriteVariable(UseOffFocusCorrection);


	return byteswritten;

}



void Kinetics::GetMaxMin(double & minx, double & maxx, double & miny, double & maxy){
	if (NDataR<=0){
		return;
	}
	if (NDataT<=0){
		return;
	}
	if (!data){
		return;
	}

	if (ShowAsFunctionOfTime){
		minx=0;
		maxx=double(NDataT)*dt*double(DeltaTimeIndex);
	}
	else{
		minx = a + double(IndexRMin)*pixelsize;
		maxx = minx + double(NDataR-1)*pixelsize*double(DeltaRIndex);
	}


	miny=1e300;
	maxy=-1e300;
	int it, ir;
	for (it=0; it<NDataT; it++){
		for (ir=0; ir<NDataR; ir++){
			double d=data[it][ir];
			if (d>maxy) maxy=d;
			if (d<miny) miny=d;
		}
	}

}


int Kinetics::NumberOfCurves(){
	if (NDataR<=0){
		return 0;
	}
	if (NDataT<=0){
		return 0;
	}
	if (!data){
		return 0;
	}
	int result;
	if (ShowAsFunctionOfTime){
		result = NDataR;
	}
	else{
		result = NDataT;
	}
	return result / DeltaShow;

}
int Kinetics::NPointsPerCurve(){
	if (NDataR<=0){
		return 0;
	}
	if (NDataT<=0){
		return 0;
	}
	if (!data){
		return 0;
	}
	if (ShowAsFunctionOfTime){
		return NDataT;
	}
	else{
		return NDataR;
	}
}
void Kinetics::GetPoint(int curve, int point, double & x, double & y, bool FitPoint){
	GetPoint(curve, point, x, y, ShowAsFunctionOfTime, BOOL(FitPoint));
}
void Kinetics::GetPoint(int curve, int point, double & x, double & y, bool ShowAsFunctionOfTime, bool FitPoint){
	if (NDataR<=0){
		return ;
	}
	if (NDataT<=0){
		return ;
	}
	if (!data){
		return ;
	}
	int it, ir;

	if (ShowAsFunctionOfTime){
		x = double(point)*dt*double(DeltaTimeIndex);
		it = point;
		ir = curve*DeltaShow;
	}
	else{
		x = a + double(IndexRMin + point*DeltaRIndex)*pixelsize;
		it = curve*DeltaShow;
		ir = point;
	}

	if (FitPoint){
		y=fit[it][ir];
	}
	else{
		y=data[it][ir];
	}

}


double Kinetics::GetCurrentFromLinearFit(double & SD){
	double InitialH = pow(10.0, -InitialpH);
	SD = 0;
	double sum1=0; double sum2=0;
	int it, ir;
	for (ir=0; ir<NDataR; ir++){
		for (it=0; it<NDataT; it++){
			double f=fit[it][ir];
			if (ConvertToDeltaHTotal){
				sum1 += data[it][ir]*f;
//				ShowFloat(data[it][ir], "DH");
			}
			else{
				double DeltaH = 1e6*(pow(10.0, -data[it][ir]-InitialpH)-InitialH); // in microM
//				ShowFloat(DeltaH, "DH");
				sum1 += DeltaH*f;
			}
			sum2 += f*f;
		}
	}

	double A = sum1/sum2;

	for (ir=0; ir<NDataR; ir++){
		for (it=0; it<NDataT; it++){
			fit[it][ir] *= A;
			double temp;
			if (ConvertToDeltaHTotal){
				temp = fit[it][ir] - data[it][ir];
			}
			else{
				double DeltaH = 1E6*(pow(10.0, -data[it][ir]-InitialpH)-InitialH);
				temp = fit[it][ir] - DeltaH;
//				fit[it][ir] /= 1e6; // in molarDELTAH in uM;
				fit[it][ir] = -log10( (fit[it][ir]/1e6+InitialH))-InitialpH;
			}
			SD += temp*temp;
		}
	}

	// A = J*a*a/D or A=J*a*a/D/beta

	if (!ConvertToDeltaHTotal){
//		Alert0("A *= beta");
		A *= beta;
	}

	// now A = J*a*a/D


	A *= D; // now A=J*a*a

	return A*1.21247e3; // because J is in micromoles / secs / square meter; F is in C / moles and Htot is in moles / liter

}

void Kinetics::CalcTimeDependence(double r, double DeltaT){
	double argr=r-a;
	double s=1.0/a;

	for (int it=0; it<NDataT; it++){
		double t = double(it)*dt*double(DeltaTimeIndex)+DeltaT;
//		ShowFloat(t, "t");
		double Dt = D*t;
		double rootDt = sqrt(Dt);
		double tp1=argr/2.0/rootDt;
		tempfit[it] = (erfcc(tp1)-exp(s*(argr + Dt*s))*erfcc( tp1 + s*rootDt))/r;
	}

}
void Kinetics::CalcRadiusAndTimeDependence(){
	double s=1.0/a;
	double DeltaT=0.0;
	DeltaT=0.0;
	bool old = false;
	for (int ir=0; ir<NDataR; ir++){
		double r = a + double(IndexRMin + ir*DeltaRIndex)*pixelsize;
//		ShowFloat(r, "r");
		double argr=r-a;
//		ShowFloat(argr, "argr");
		int NN = NCoeffOffFocusCorrection;
		if (NN<0) NN=0;
		if (!UseOffFocusCorrection) NN=0;
		double dd=0;
		if (NN>0){
			dd = a/double(NN);
		}
		for (int it=0; it<NDataT; it++){
			fit[it][ir] = 0;
		}
		int indexoff = 0;
//		double sumscale =0;
		for (indexoff = 0; indexoff<=NN; indexoff++){
			double d=double(indexoff)*dd;
			double rnew;
			if (old){

				double seno  = d/a;

				if (seno<0.0) seno = 0.0;
				if (seno>=0.999999) seno = 0.999999;

				rnew = r + a*(1.0 - cos(asin(seno)));

//			ShowFloat(indexoff,"indexoff");
//			ShowFloat(1.0 - cos(asin(seno)), "1-..");
			}
			else{
				rnew=sqrt(r*r+d*d);
			}

			CalcTimeDependence(rnew, DeltaT);


			double scale;
			if (NN==0){
				scale = 1.0;
			}
			else{
				scale = CoeffsOffFocusCorrection[indexoff];
				if (indexoff>0){
					scale *= 2.0;
				}
			}
//			sumscale += scale;
			for (int it=0; it<NDataT; it++){
				fit[it][ir] += scale*tempfit[it];
			}
		}
//		ShowFloat(sumscale, "ss");
/*
		CalcTimeDependence(r, DeltaT)
		for (int it=0; it<NDataT; it++){
			fit[it][ir] = TempFit[it];
		}
*/
	}
}
void Kinetics::FitLinear(){
	CWaitCursor dummy;
	if (NDataR<=0){
		return ;
	}
	if (NDataT<=0){
		return ;
	}
	if (!data){
		return ;
	}
	HasBeenFitted = true;

	if (UseOffFocusCorrection){
		CalcCoeffeOffFocusCorrection(a);
	}

	double SD;
	if (!FitD){
		CalcRadiusAndTimeDependence();
		Current = GetCurrentFromLinearFit(SD);
		return;
	}


	Simplex simp;

	double Params[5];
	double BestParams[5];
	int NParams = 1;
	Params[0]=D;

	int MaxIter = 200;
	double StepSize = 0.2;
	double BestSD = 1e300;

	simp.InitSimplex(StepSize, Params, NParams);

	int i;

	for (i=0; i<MaxIter; i++){
		D = Params[0];
		CalcRadiusAndTimeDependence();
		Current = GetCurrentFromLinearFit(SD);
		if (SD<BestSD){
			simp.CopyParams(Params, BestParams);
			BestSD = SD;
		}
		simp.NextSimp(Params, SD);
	}

	D = BestParams[0];
	CalcRadiusAndTimeDependence();
	Current = GetCurrentFromLinearFit(SD);

}



void Kinetics::SetupSimulationFit(){
	if (NDataR<=0){
		return ;
	}
	if (NDataT<=0){
		return ;
	}
	if (!data){
		return ;
	}
	iter = 0;
	d.r0 = a;
	double 	rmax = a + double(IndexRMin + (NDataR-1)*DeltaRIndex)*pixelsize;
//	d.rmax = a + double(IndexRMin + (NDataR-1)*DeltaRIndex)*pixelsize;
	if (rmax < this->RMaxSimulation){
		rmax = this->RMaxSimulation;
	}
	d.rmax = rmax;
 

	d.DH = DH; // diffusion constant of protons in m^2/s
	d.DB = D; // diffusion constant of buffer (bound and unbound!)

	d.pHbulk=InitialpH; // bulk pH
//	ShowFloat(InitialpH, "InitialpH");
	d.T = TotalFree;		// total constant buffer concentration in molar
//	ShowFloat(d.T,"T");


	d.alpha=1.0;
	d.beta=1.0;

	d.KD = KFree; // is = beta/alpha (in molar)

	d.IndependentReactionParameters=1;

// The region is sliced into n spherical slices. Either the slice thickness 
// or the number of slices can be defined.

//	int M; // 
	d.DR0 = DR; // r0+M*DR=rmax;
	d.DRMax = DR;
	d.FactDR = 1.0;

	d.ModeToDetermineSliceNumber=1;		// = 0: fix n, calc. DR;
										// = 1: fix DR, calc. n;


	d.IH=Current; // in Amperers

	d.ProtocolType=0;

	d.DoRenormalize=false;

	d.AutoCalcBeta=true; // beta = delta total [H] / delta free [H] = 1+KT/(H0+K)^2

	d.IncludeFixedSurfaceBuffer=true;
	d.KDFixedSurfaceBuffer = KFixed;
	d.AmountFixedSurfaceBufferInMoles = AmountFixed;
	d.ThicknessFixedVolume = ThicknessFixedBufferVolume;

	d.DT0=DT0;
	d.DTMax=DTMAX;
	d.FactDT = factdt;

	d.UseWagnerKeizer = true; 
	d.SimpleWagnerKeizer=false;

	d.UseFluxCorrectionInLinearApproximation=false;

	NParamsSimulFit = PutParamsInArraySimulFit(ParamsSimulFit);


	if (AssociatedIndexes){
		delete [] AssociatedIndexes;
		AssociatedIndexes = NULL;
	}

	AssociatedIndexes = new int [NDataR];


//	ShowFloat(d.M, "d.M");
//	ShowFloat(NDataR, "NDataR");
	int ir;
	for (ir=0; ir<NDataR; ir++){
		int index = int (double(IndexRMin + ir*DeltaRIndex)*pixelsize/DR );
//		ShowFloat(index, "index");
		if (index>d.M){
			index=d.M;
		}
		if (index<0){
			index=0;
		}
		AssociatedIndexes[ir] = index;
	}
	for (ir=0; ir<NDataR; ir++){
		for (int it=0; it<NDataT; it++){
			fit[it][ir] = 0;
		}
	}

	d.Setup();

	if (NParamsSimulFit<=0){
		SDSimulFit = DoCalcSimulFit();
//		SD = SDSimulFit;
		return;
	}

	BestSDSimulFit = 1E300;

	simpSimulFit.InitSimplex(StepSize, ParamsSimulFit, NParamsSimulFit);
	simpSimulFit.CopyParams(ParamsSimulFit, BestParamsSimulFit);



}

int Kinetics::PutParamsInArraySimulFit(double * params){
	int NP=0;
	if (FitCurrent){
		params[NP]=log(Current);
		NP++;
	}
	if (FitD){
		params[NP]=log(D);
		NP++;
	}
	if (FitKFree){
		params[NP]=log(KFree);
		NP++;
	}
	if (FitTotalFree){
		params[NP]=log(TotalFree);
		NP++;
	}
	if (FitKFixed){
		params[NP]=log(KFixed);
		NP++;
	}
	if (FitAmountFixed){
		params[NP]=log(AmountFixed);
		NP++;
	}
	if (FitThicknessFixedBufferVolume){
		params[NP]=log(ThicknessFixedBufferVolume);
		NP++;
	}

	return NP;

}
int Kinetics::PutArrayInParamsSimulFit(double * params){
	int NP=0;
	if (FitCurrent){
		Current = exp(params[NP]);
		NP++;
	}
	if (FitD){
		D = exp(params[NP]);;
		NP++;
	}
	if (FitKFree){
		KFree = exp(params[NP]);
		NP++;
	}
	if (FitTotalFree){
		TotalFree = exp(params[NP]);
		NP++;
	}
	if (FitKFixed){
		KFixed = exp(params[NP]);
		NP++;
	}
	if (FitAmountFixed){
		AmountFixed = exp(params[NP]);
		NP++;
	}
	if (FitThicknessFixedBufferVolume){
		ThicknessFixedBufferVolume = exp(params[NP]);
		NP++;
	}

	d.DB = D;
	d.T = TotalFree;
	d.KD = KFree;
	d.IH=Current;
	d.KDFixedSurfaceBuffer = KFixed;
	d.AmountFixedSurfaceBufferInMoles = AmountFixed;
	d.ThicknessFixedVolume = ThicknessFixedBufferVolume;

	return NP;
}



BOOL Kinetics::StepSimulationFit(double & SD){

	CWaitCursor dummy;

	if (NParamsSimulFit<=0){
		return false;
	}

//	if ((iter >MaxIterSimulFit)	||	 (InKey()==STOPKEY)){
//	if ((iter >MaxIterSimulFit)	||	 (InKey()==F12KEY)){
	if (iter >MaxIterSimulFit){
//		ShowFloat(MaxIterSimulFit, "MaxIterSimulFit");
//		ShowFloat(iter, "iter");
		simpSimulFit.CopyParams(BestParamsSimulFit, ParamsSimulFit);
		PutArrayInParamsSimulFit(BestParamsSimulFit);
		SDSimulFit = DoCalcSimulFit();
		SD = SDSimulFit;
		return false;
	}
	iter ++;

	PutArrayInParamsSimulFit(ParamsSimulFit);
//	ShowFloat(Current, "Current");
	SDSimulFit = DoCalcSimulFit();
//	ShowFloat(SDSimulFit, "SD");
	if (SDSimulFit<BestSDSimulFit){
//		Alert0("Better");
		BestSDSimulFit = SDSimulFit;
		simpSimulFit.CopyParams(ParamsSimulFit, BestParamsSimulFit);
	}

	simpSimulFit.NextSimp(ParamsSimulFit, SDSimulFit);
	SD = SDSimulFit;

	return true;
}

void Kinetics::GetBestParamsSimulationFit(double & SD){
		simpSimulFit.CopyParams(BestParamsSimulFit, ParamsSimulFit);
		PutArrayInParamsSimulFit(BestParamsSimulFit);
		SDSimulFit = DoCalcSimulFit();
		SD = SDSimulFit;
}



double Kinetics::DoCalcSimulFit(){
	double SD = 0.0;

	d.Setup();

	double DT = dt*double(DeltaTimeIndex); 
	double tmax = double(NDataT-1)*DT;

	int ir;

	for (ir=0; ir<NDataR-1; ir++){
		fit[0][ir] = 0;
	}

	double NextTime = DT;

	int it = 1;
//	ShowFloat(InitialpH, "InitialpH")
	while (true){
		if (d.time>=NextTime){
			for (int ir=0; ir<NDataR; ir++){
				int index = AssociatedIndexes[ir];
				fit[it][ir]=-log10(d.H[index])-InitialpH;
//				fit[it][ir]=-log10(d.hnew[index])-InitialpH;
//				ShowFloat(data[it][ir], "data");
//				ShowFloat(fit[it][ir], "fit");
				
//				ShowFloat(d.hnew[index], "fit");
			}

			NextTime += DT;
			it++;
			if (it>=NDataT){
				break;
			}
		}

		d.Step(true);
	}

	for (ir=0; ir<NDataR; ir++){
		for (int it=0; it<NDataT; it++){
			double temp = fit[it][ir] - data[it][ir];
			SD += temp*temp;
		}
	}

	return SD;
}
