#include "stdafx.h"
#include <math.h>
#include "GatingCell.h"
#include "RungeKutta.h"
#include "utils.h"
#include "nrutil.h"
#include "simplex.h"
#include "../StateEditor/Model.h"

int * GatingCell::MarkovStuff[2]={NULL, NULL};

GatingCellDataEntry::GatingCellDataEntry(){
	Data = Fit = NULL;
	DataPresent = false;

	IVType=0; // 0: direct IV, 1: tail
	PerformLeakSubtraction=true;
	VHoldP4=0;
	VTestP4=-100;

	VHold=0;
	VStart=200;
	dt=10e-6;  // Voltages are in mV, times in seconds

	/// Parameters
	Rs=2;
	Cm=30;
	Rm=200000; // cell parameters

	FitRs = FitCm = FitRm = false;

}
GatingCellDataEntry::~GatingCellDataEntry(){
	if (!DataPresent) return;

	DeAlloc();
}

void GatingCellDataEntry::DeAlloc(){
	if (!DataPresent) return;
	free_dmatrix(Data, 0, NTraces-1, 0, NPointsPerTrace-1);
	free_dmatrix(Fit, 0, NTraces-1, 0, NPointsPerTrace-1);
	DataPresent = false;
}
void GatingCellDataEntry::Alloc(){
	Data = dmatrix(0, NTraces-1, 0, NPointsPerTrace-1);
	Fit = dmatrix(0, NTraces-1, 0, NPointsPerTrace-1);
	DataPresent = true;
}
void GatingCellDataEntry::CopyTo(GatingCellDataEntry & to){
	to.Cm = Cm;
	to.Rs = Rs;
	to.Rm = Rm;
	to.FitCm = FitCm;
	to.FitRm = FitRm;
	to.FitRs = FitRs;
	to.IVType = IVType;

	to.OffsetForFit = OffsetForFit;
	to.offsetindexfromstart = offsetindexfromstart;
	to.series = series;
	to.firstentry=firstentry;

	to.NTraces = NTraces;
	to.NPointsPerTrace = NPointsPerTrace;

	to.VHold = VHold;
	to.VStart = VStart;
	to.DeltaV = DeltaV;

	to.VHoldP4 = VHoldP4;
	to.VTestP4 = VTestP4;

	to.PerformLeakSubtraction = this->PerformLeakSubtraction;

	to.dt = dt;


}
void GatingCellDataEntry::GetMaxMinData(double & max, double & min){
	max=-1e300;
	min=1e300;
	if (!DataPresent){
		Alert0("no data");
		return;
	}
	for (int it=0; it<this->NTraces; it++){
		for (int i=0; i<this->NPointsPerTrace; i++){
			double d=this->Data[it][i];
			if (d>max) max = d;
			if (d<min) min = d;
		}
	}
//	ShowFloat(max, "max");
//	ShowFloat(min, "min");

}

void GatingCellDataEntry::PutParamsInArray(int & NP, double * Params){
	if (FitRs){
		Params[NP]=Rs;
		NP++;
	}
	if (FitRm){
		Params[NP]=Rm;
		NP++;
	}
	if (FitCm){
		Params[NP]=Cm;
		NP++;
	}

}
void GatingCellDataEntry::PutArrayInParams(int & NP, double * Params){
	if (FitRs){
		Rs=Params[NP];
		NP++;
	}
	if (FitRm){
		Rm=Params[NP];
		NP++;
	}
	if (FitCm){
		Cm=Params[NP];
		NP++;
	}
}
bool GatingCellDataEntry::SaveOnFile(CFile * fp, bool WriteData){

	WriByNS(IVType); //0: direct IV, 1: tail;
	WriByNS(Cm);
	WriByNS(Rs);
	WriByNS(Rm);
	WriByNS(FitCm);
	WriByNS(FitRs);
	WriByNS(FitRm);
	WriByNS(PerformLeakSubtraction);
	WriByNS(series);
	WriByNS(firstentry);
	WriByNS(NTraces);
	WriByNS(offsetindexfromstart);
	WriByNS(OffsetForFit);
	WriByNS(NPointsPerTrace);
	WriByNS(gaindata);
	WriByNS(SD);
	WriByNS(VHold);
	WriByNS(VStart);
	WriByNS(DeltaV); 
	WriByNS(dt);
	WriByNS(VHoldP4);
	WriByNS(VTestP4);
	WriByNS(DataPresent);
	if (WriteData){
		if (DataPresent){
			for (int it=0; it<NTraces; it++){
				for (int i=0; i<NPointsPerTrace; i++){
					WriByNS(Data[it][i]);
					WriByNS(Fit[it][i]);
				}
			}
		}
	}

	return true;
}
bool GatingCellDataEntry::LoadFromFile(CFile * fp, bool ReadData){
	ReByNS(IVType); //0: direct IV, 1: tail;
	ReByNS(Cm);
	ReByNS(Rs);
	ReByNS(Rm);
	ReByNS(FitCm);
	ReByNS(FitRs);
	ReByNS(FitRm);
	ReByNS(PerformLeakSubtraction);
	ReByNS(series);
	ReByNS(firstentry);
	ReByNS(NTraces);
	ReByNS(offsetindexfromstart);
	ReByNS(OffsetForFit);
	ReByNS(NPointsPerTrace);
	ReByNS(gaindata);
	ReByNS(SD);
	ReByNS(VHold);
	ReByNS(VStart);
	ReByNS(DeltaV); 
	ReByNS(dt);
	ReByNS(VHoldP4);
	ReByNS(VTestP4);
	if (DataPresent){
		DeAlloc();
		Data = Fit = NULL;
	}
	ReByNS(DataPresent);
	if (ReadData){
		if (DataPresent){
			Alloc();
			for (int it=0; it<NTraces; it++){
				for (int i=0; i<NPointsPerTrace; i++){
					ReByNS(Data[it][i]);
					ReByNS(Fit[it][i]);
				}
			}
		}
	}

	return true;
}

GatingCellOnAndOffData::GatingCellOnAndOffData(){
}
GatingCellOnAndOffData::~GatingCellOnAndOffData(){
}
bool GatingCellOnAndOffData::SaveOnFile(CFile * fp){
	for (int ee=0; ee<2; ee++){
		if (!Entries[ee].SaveOnFile(fp, true)){
			return false;
		}
	}
	return true;
}
void GatingCellOnAndOffData::ExportDataToClibboard(){
	CWaitCursor dummy;

	int NMax = 0;

	CString c;
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & e = Entries[ee];
		if (e.NPointsPerTrace>NMax){
			NMax = e.NPointsPerTrace;
		}
	}
	BOOL first = true;

	for (int i=0; i<NMax; i++){
		for (int ee=0; ee<2; ee++){
			GatingCellDataEntry & e = Entries[ee];
			for (int dataorfit=0; dataorfit<2; dataorfit++){
				double time = 0.0;
				if (i<e.NPointsPerTrace){
					time = double(i)*e.dt*1e3; // in ms
				}
				AddDoubleToString(first, time, c);

				for (int t=0; t<e.NTraces; t++){
					double data=0.0;
					if (i<e.NPointsPerTrace){
						if (dataorfit==0){
							data = e.Data[t][i];
						}
						else{
							data = e.Fit[t][i];
						}
					}
					AddDoubleToString(first, data, c);
				}
			}
		}
		c += "\r\n"; first = true;
	}
	CopyTextToClipboard(c);


}
bool GatingCellOnAndOffData::LoadFromFile(CFile * fp){
	for (int ee=0; ee<2; ee++){
		if (!Entries[ee].LoadFromFile(fp, true)){
			return false;
		}
	}
	AbsMax = 1e-300;
	for (int ee=0; ee<2; ee++){
		double ma, mi;
		Entries[ee].GetMaxMinData(ma, mi);
		if (ma>AbsMax) AbsMax = fabs(ma);
		if (fabs(mi)>AbsMax) AbsMax = fabs(mi);
	}
	return true;
}

GatingCell::GatingCell(){
	MaxIter = 100;
	StepSize = .2;
	zTotal = 1.3;


	prk = new RungeKutta;
	// params of transporter
	Qmax = 1e-12; // in Coulomb

	alpha0=100;
	zalpha=0.5;
	beta0=1e4;
	zbeta=0.5;
	SizeBuffer = 0;
	NullPointers();
	FitQmax = true;
	Fitalpha0 = Fitzalpha = Fitbeta0 = true;
	Fitalpha0 = Fitzalpha = Fitbeta0 = false;
	FitzTotal = false;
	UseKVeryOld = true;
	ModelTypeUseK = 0;
	FitzK = true;
	FitK = true;
	zK=0.4;
	K=1;
	tTest = 5e-3;
	VTest = 100.0;
	

}
GatingCell::~GatingCell(){
	delete prk;
	Dealloc();
}
void GatingCell::Alloc(){
	if (NPoints>SizeBuffer){
		Dealloc();
		time = new double [NPoints];
		predictedV = new double [NPoints];
		predictedI = new double [NPoints];
		leakI = new double [NPoints];
		SizeBuffer = NPoints;
	}
}
void GatingCell::Dealloc(){
	if (time) delete [] time;
	if (predictedI) delete [] predictedI;
	if (predictedV) delete [] predictedV;
	if (leakI) delete [] leakI;
	NullPointers();
}
void GatingCell::NullPointers(){
	leakI = NULL;
	time = NULL;
	predictedI = NULL;
	predictedV = NULL;
}


void GatingCell::CopyTo(GatingCell & to){
	GatingCell & f = *(this);

	GeneralData.CopyTo(to.GeneralData);

	to.zon = zon;
	to.koff = koff;
	to.BestSD = BestSD;
	to.UseKVeryOld = UseKVeryOld;
	to.ModelTypeUseK = ModelTypeUseK;

	to.FitzK = FitzK;
	to.FitK = FitK;
	to.zK = zK;
	to.K = K;
	to.zTotal = f.zTotal;
	to.MaxIter = f.MaxIter;
	to.StepSize = f.StepSize;

	to.FitQmax = f.FitQmax;
	to.Fitalpha0 = f.Fitalpha0;
	to.Fitzalpha = f.Fitzalpha;
	to.Fitbeta0 = f.Fitbeta0;
	to.FitzTotal = f.FitzTotal;
	to.Qmax=f.Qmax;
	to.alpha0=f.alpha0;
	to.zalpha = f.zalpha;
	to.beta0 = f.beta0;
	to.zbeta = f.zbeta;

	to.tTest = this->tTest;
	to.VTest = VTest;


}
void GatingCell::CalcSteadyState3StateModel(double v, double * y){
	// y[1] = P1
	// y[2] = P2
	double w[4];
	CalcW(v, w);
	double phi1=w[3]/w[2];
	double phi2=w[1]/w[0];  // K/Cl

	if (ModelTypeUseK==4){ // scheme II
		double denom = 1 + phi1 + phi1*phi2;
		y[2]=1.0/denom;
		y[1]=phi1*y[2];
	}
	if (ModelTypeUseK==5){// scheme III
		double denom = 1 + phi2 + phi1*phi2;
		y[2]=1.0/denom;
		y[1]=phi2*y[2];
	}
//	ShowFloat(v, "v");
//	ShowFloat(y[1], "y1");
//	ShowFloat(y[2], "y2");

}
void GatingCell::CalcW(double v, double * w){
	// w0: cl*kon
	// w1: koff
	// w2: alpha
	// w3: beta
//	ShowFloat(v, "v");
//	ShowFloat(K, "K");
	double KV=K*exp(phikappa*v);  // Cl/K(V)
	w[1] = koff*exp(phioff*v);
	w[0] = w[1]*KV;
	w[2] = alpha0*exp(v*phialpha);
	w[3] = beta0*exp(v*phibeta);
//	ShowFloat(w[0], "w[0]");
//	ShowFloat(w[1], "w[1]");
//	ShowFloat(w[2], "w[2]");
//	ShowFloat(w[3], "w[3]");
}

double GatingCell::CalcQ(double v){
//	ShowFloat(v, "v");
	double a=CalcAlpha(v);
	double b=CalcBeta(v);
//	ShowFloat(a, "a");

//	ShowFloat(b, "b");
	return a*Qmax/(a+b);
}
inline double GatingCell::CalcAlpha(double v){
//	if (UseKOld){
	if  (ModelTypeUseK==1){
//		Alert0("effect on alpha");
		double KK=K*exp(v*phikappa);
		return KK*alpha0*exp(v*phialpha)/(1.0+KK);
	}
	else{
		return alpha0*exp(v*phialpha);
	}
}
inline double GatingCell::CalcBeta(double v){
//	if (UseKOld){
	if (ModelTypeUseK==2){
		double KK=K*exp(v*phikappa);
//		Alert0("effect on beta");
		return beta0*exp(v*phibeta)/(1.0+KK);
	}
	else{
		return beta0*exp(v*phibeta);
	}
}
double GatingCell::CalcVmSteadyState(double v){
	return v*GeneralData.Rm/(GeneralData.Rm+GeneralData.Rs);
}
void GatingCell::CalcDerivs(double t, double * y, double * yp, int * STUFF){
	GatingCell & gc = *( (GatingCell *) (STUFF));
	gc.DoCalcDerivs(t, y, yp);
}

void GatingCell::DoCalcDerivs(double t, double * y, double * yp){

	double v=y[0];
//	double Q=y[1];
	double a=CalcAlpha(v);
//	ShowFloat(v, "v");
//	ShowFloat(Q, "Q");
//	ShowFloat(a, "a");
//	double b=CalcBeta(v);

	yp[1]=a*Qmax-(a+CalcBeta(v))*y[1];

	yp[0] = KA - v*sumk - yp[1]*INVCinFDiv1000; // dQ/dt is in C/s; dV/dt is in mV/s

}

void GatingCell::InitPrediction(){
	zbeta = zTotal-zalpha;
	zoff = zK - zon*zK;
	ka=1.0/(GeneralData.Rs*GeneralData.Cm*1e-6);
	KA=VTest*ka;
	kb=1.0/(GeneralData.Rm*GeneralData.Cm*1e-6);
	sumk = ka+kb;
	CinFDiv1000 = GeneralData.Cm*1e-15;
	INVCinFDiv1000 = 1.0/CinFDiv1000;
	phialpha = zalpha/25.0;
	phibeta = -zbeta/25.0;
	phikappa = zK/25.0;
	phion = zon*zK/25.0;
	phioff = -zoff/25.0;
	NPoints = GeneralData.NPointsPerTrace; //UINT  (tTest / GeneralData.dt);

//	ShowFloat(VTest, "Vtest");
//	ShowFloat(NPoints, "NP");

	Alloc();
	RungeKutta & rk = *prk;

//	double x[2], xp[2];

	int * STUFF = (int * )(this);
	double dxsav = GeneralData.dt/10.0;
	double eps = 1e-7;
	double h1=dxsav;
	double hmin=0;
	int kount;
	int NEquation = 2;
	if ((ModelTypeUseK==4) || (ModelTypeUseK==5)){
		NEquation = 3;
		rk.Initialize(NEquation, 0, NULL, NULL, &kount, dxsav, eps, h1, hmin, CalcDerivs3StateModel, STUFF);
	}
	else{
		rk.Initialize(NEquation, 0, NULL, NULL, &kount, dxsav, eps, h1, hmin, CalcDerivs, STUFF);
	}

	time[0]=0;
	istep=0;

	y[0]=CalcVmSteadyState(GeneralData.VHold);
//	ShowFloat(GeneralData.VHold, "GeneralData.VHold");
//	ShowFloat(y[0], "Vm0");
	if ((ModelTypeUseK==4) || (ModelTypeUseK==5)){
		CalcSteadyState3StateModel(y[0], y);
	}
	else{
		y[1]=CalcQ(y[0]);
	}
//	ShowFloat(y[1], "Q0");

}
bool GatingCell::Step(){
	if (istep>=NPoints){
		return false;
	}
//	ShowFloat(istep, "istep");
	predictedV[istep]=y[0];
	predictedI[istep]=(VTest-predictedV[istep])/1e9/GeneralData.Rs;
	int nok, nbad;
	RungeKutta & rk = *prk;
	double t=time[istep];
	rk.Integrate(y, t, t+GeneralData.dt, nok, nbad);
	istep++;
	if (istep<NPoints){
		time[istep] = time[istep-1] + GeneralData.dt;
	}
//	ShowFloat(y[0], "v");
	return true;
}


void GatingCell::Predict(){
	InitPrediction();
	while (Step()){
	}
}


int GatingCell::PutParamsInArray(double * Params){
	int NP=0;

	GeneralData.PutParamsInArray(NP, Params);

	if (FitQmax){
		Params[NP]=Qmax;
		NP++;
	}
	if (Fitalpha0){
		Params[NP]=alpha0;
		NP++;
	}
	if (Fitzalpha){
		Params[NP]=zalpha;
		NP++;
	}
	if (Fitbeta0){
		Params[NP]=beta0;
		NP++;
	}
	if (FitzTotal){
		Params[NP]=zTotal;
		NP++;
	}

//	if (ModelTypeUseK==1){
//	if (UseKOld){
	if (ModelTypeUseK>0){
		if (FitzK){
			Params[NP] = zK;
			NP++;
		}
		if (FitK){
			Params[NP] = K;
			NP++;
		}
	}


	return NP;
}
int GatingCell::PutArrayInParams(double * Params){
	int NP=0;

	GeneralData.PutArrayInParams(NP, Params);

	if (FitQmax){
		Qmax=Params[NP];
//		ShowFloat(Qmax, "Qmax");
		NP++;
	}
	if (Fitalpha0){
		alpha0=Params[NP];
		NP++;
	}
	if (Fitzalpha){
		zalpha=Params[NP];
		NP++;
	}
	if (Fitbeta0){
		beta0=Params[NP];
		NP++;
	}
	if (FitzTotal){
		zTotal=Params[NP];
		NP++;
	}
//	if (UseKOld){
	if (ModelTypeUseK>0){
		if (FitzK){
			zK = Params[NP];
			NP++;
		}
		if (FitK){
			K = Params[NP];
			NP++;
		}
	}

	return NP;
}

void GatingCell::PerformFit(int lIVType,
					int lNTraces,
					int lNPointsPerTrace,
					int lOffset, // green cursor-blue cursor
					double SampleTimeInSeconds,
					double lVHold,
					double lVStart,
					double lDeltaV,
					double ** lData,
					double ** lFit){ // first, all the parameters have to be assigned!!
	if (lData == NULL){
		Alert0("Data == 0 in GatingCell::PerformFit()");
		return;
	}
	if (lFit == NULL){
		Alert0("Fit == 0 in GatingCell::PerformFit()");
		return;
	}

	if (lNPointsPerTrace<2){
		Alert0("NPointsPerTrace<2 in GatingCell::PerformFit()");
		return;
	}
	GeneralData.Data = lData;
	GeneralData.Fit = lFit;
	GeneralData.NPointsPerTrace=lNPointsPerTrace;
	GeneralData.dt = SampleTimeInSeconds;
	GeneralData.IVType = lIVType;
	GeneralData.NTraces=lNTraces;
//	ShowFloat(lNTraces, "lNTraces");
	GeneralData.OffsetForFit= lOffset;
	GeneralData.VHold = lVHold;
	GeneralData.VStart = lVStart;
	GeneralData.DeltaV = lDeltaV;


	tTest = double(GeneralData.NPointsPerTrace)*GeneralData.dt;

	NPoints = GeneralData.NPointsPerTrace;
	Alloc();

	NParams = PutParamsInArray(Params);
	NParams = PutParamsInArray(BestParams);
//	ShowFloat(NParams, "NP");
	BestSD = CalcSD();
//	ShowFloat(BestSD, "BestSD");

	Simplex simp;

	RungeKutta & rk = *prk;
	rk.ShowError = true;

	if (NParams>0){
		simp.InitSimplex(StepSize, Params, NParams);
		for (int iter=0; iter<MaxIter; iter++){
			if (InKey()==STOPKEY){
				goto EndFitGatingCell;
			}
			PutArrayInParams(Params);
			double SD = CalcSD();
//			ShowFloat(SD, "SD");
			if (SD<BestSD){
				BestSD = SD;
//				ShowFloat(BestSD, "BestSD");
				simp.CopyParams(Params, BestParams);
			}
			simp.NextSimp(Params, SD);
		}
	}
EndFitGatingCell:
	if (NParams>0) simp.CopyParams(BestParams, Params);
	PutArrayInParams(BestParams);
	CalcSD();
}


double GatingCell::CalcSD(){
	if (zalpha>zTotal){
		return 1e300;
	}
	CalcFit();
	double SD = 0;
	for (int t=0; t<GeneralData.NTraces; t++){
		double * d = GeneralData.Data[t];
		double * f = GeneralData.Fit[t];
		for (int i=GeneralData.OffsetForFit; i<(GeneralData.NPointsPerTrace-1); i++){
			double tp1 = d[i]- f[i];
			SD += tp1*tp1;
		}
	}
	return SD;
}

void GatingCell::CalcFit(){
//	Alert0("hi");
	if (GeneralData.PerformLeakSubtraction){
		VTest = GeneralData.VTestP4;
		double SaveVHold = GeneralData.VHold;
		GeneralData.VHold = GeneralData.VHoldP4;
		
		Predict();
		GeneralData.VHold = SaveVHold;
		for (UINT i=0; i<NPoints; i++){
			leakI[i] = predictedI[i];
		}
	}
//	ShowFloat(GeneralData.GeneralData.NTraces, "Ntraces");
	for (int itrace=0; itrace<GeneralData.NTraces; itrace++){
		VTest = GeneralData.VStart+double(itrace)*GeneralData.DeltaV;
//		ShowFloat(VTest, "Vtest");
		Predict();
		for (UINT i=0; i<NPoints; i++){
//			ShowFloat(NPoints, "NP");
//			ShowFloat(predictedI[i], "p");
		}
		if (GeneralData.PerformLeakSubtraction){
			double scaleleak = -(VTest-GeneralData.VHold)/(GeneralData.VTestP4-GeneralData.VHoldP4);
			for (UINT i=0; i<NPoints; i++){
				predictedI[i] = predictedI[i] + scaleleak*leakI[i];
			}
		}
		for (UINT i=0; i<NPoints; i++){
			GeneralData.Fit[itrace][i] = predictedI[i];
		}

	}

}

void GatingCell::FitIVAndTail(){
	for (int ee=0; ee<2; ee++){
		if (!OnOffData.Entries[ee].DataPresent){
			Alert0("Not both IVand tail present  - no action");
			return;
		}
	}

	double NPMax = 0;
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & de = OnOffData.Entries[ee];
		if (de.NPointsPerTrace>NPMax){
			NPMax = de.NPointsPerTrace;
		}
	}

	NPoints = UINT(NPMax);
	Alloc();


	NParams = PutParamsInArrayIVAndTail(Params);
	NParams = PutParamsInArrayIVAndTail(BestParams);
//	ShowFloat(NParams, "NP");
	BestSD = CalcSDIVAndTail();
//	ShowFloat(BestSD, "BestSD");

	Simplex simp;

	RungeKutta & rk = *prk;
	rk.ShowError = true;

	if (NParams>0){
		simp.InitSimplex(StepSize, Params, NParams);
		for (int iter=0; iter<MaxIter; iter++){
			if (InKey()==STOPKEY){
				goto EndFitGatingCellIVAndTail;
			}
			PutArrayInParamsIVAndTail(Params);
			double SD = CalcSDIVAndTail();
//			ShowFloat(SD, "SD");
			if (SD<BestSD){
				BestSD = SD;
//				ShowFloat(BestSD, "BestSD");
				simp.CopyParams(Params, BestParams);
			}
			simp.NextSimp(Params, SD);
		}
	}
EndFitGatingCellIVAndTail:
	if (NParams>0)	simp.CopyParams(BestParams, Params);
	PutArrayInParamsIVAndTail(BestParams);
	CalcSDIVAndTail();

}

double GatingCell::CalcSDIVAndTail(){
	if (zalpha>zTotal){
		return 1e300;
	}
//	if ( (ModelTypeUseK==4) || (ModelTypeUseK==5) ){
//		if (zon>zK){
//			return 1e300;
//		}
//	}
	CalcFitIVAndTail();
	double SD = 0;
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & de = OnOffData.Entries[ee];
		for (int t=0; t<de.NTraces; t++){
			double * d = de.Data[t];
			double * f = de.Fit[t];
			for (int i=de.OffsetForFit; i<(de.NPointsPerTrace-1); i++){
				double tp1 = d[i]- f[i];
				SD += tp1*tp1;
			}
		}
	}
	return SD;
}

void GatingCell::CalcFitIVAndTail(){
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & de = OnOffData.Entries[ee];

		de.CopyTo(this->GeneralData);

		if (de.PerformLeakSubtraction){
			VTest = de.VTestP4;
			double SaveVHold = de.VHold;
			de.VHold = de.VHoldP4;
			GeneralData.VHold = de.VHold;
			Predict();
			de.VHold = SaveVHold;
//			ShowFloat(de.VHoldP4, "de.VHoldP4");
//			ShowFloat(de.VTestP4, "de.VTestP4");
			for (int i=0; i<de.NPointsPerTrace; i++){
				leakI[i] = predictedI[i];
			}
		}
		for (int itrace=0; itrace<de.NTraces; itrace++){
			VTest = de.VStart+double(itrace)*de.DeltaV;
			GeneralData.VHold = de.VHold;

//			ShowFloat(VTest, "Vtest");
			Predict();
			if (de.PerformLeakSubtraction){
				double scaleleak = -(VTest-de.VHold)/(de.VTestP4-de.VHoldP4);
//				ShowFloat(scaleleak, "scaleleak");
				for (int i=0; i<de.NPointsPerTrace; i++){
					predictedI[i] += scaleleak*leakI[i];
				}
			}
			for (int i=0; i<de.NPointsPerTrace; i++){
				de.Fit[itrace][i] = predictedI[i];
			}
		}
	}

}

int GatingCell::PutParamsInArrayIVAndTail(double * Params){
	int NP=0;

	for (int ee=0; ee<2; ee++){
		OnOffData.Entries[ee].PutParamsInArray(NP, Params);
	}

	if (FitQmax){
		Params[NP]=Qmax;
		NP++;
	}
	if (Fitalpha0){
		Params[NP]=alpha0;
		NP++;
	}
	if (Fitzalpha){
		Params[NP]=zalpha;
		NP++;
	}
	if (Fitbeta0){
		Params[NP]=beta0;
		NP++;
	}
	if (FitzTotal){
		Params[NP]=zTotal;
		NP++;
	}
	if (ModelTypeUseK>0){
//	if (UseKOld){
		if (FitzK){
			Params[NP] = zK;
			NP++;
		}
		if (FitK){
			Params[NP] = K;
			NP++;
		}
	}

	return NP;
}
int GatingCell::PutArrayInParamsIVAndTail(double * Params){
	int NP=0;

	for (int ee=0; ee<2; ee++){
		OnOffData.Entries[ee].PutArrayInParams(NP, Params);
	}

	if (FitQmax){
		Qmax=Params[NP];
//		ShowFloat(Qmax, "Qmax");
		NP++;
	}
	if (Fitalpha0){
		alpha0=Params[NP];
		NP++;
	}
	if (Fitzalpha){
		zalpha=Params[NP];
		NP++;
	}
	if (Fitbeta0){
		beta0=Params[NP];
		NP++;
	}
	if (FitzTotal){
		zTotal=Params[NP];
		NP++;
	}
//	if (UseKOld){
	if (ModelTypeUseK>0){
		if (FitzK){
			zK = Params[NP];
			NP++;
		}
		if (FitK){
			K = Params[NP];
			NP++;
		}
	}

	return NP;
}

bool GatingCell::SaveOnFile(CFile * fp){
	WriByNS(StepSize);
	WriByNS(MaxIter);

	WriByNS(tTest);
	WriByNS(VTest);

	WriByNS(Qmax);
	WriByNS(FitQmax);
	WriByNS(alpha0);
	WriByNS(zalpha);
	WriByNS(beta0);
	WriByNS(zTotal);
	WriByNS(zbeta);
	WriByNS(Fitalpha0);
	WriByNS(Fitzalpha);
	WriByNS(Fitbeta0);
	WriByNS(FitzTotal);

	if (!OnOffData.SaveOnFile(fp)){
		return false;
	}
	if (!GeneralData.SaveOnFile(fp)){
		return false;
	}
	WriByNS(UseKVeryOld);
//	ShowFloat(ModelTypeUseK, "ModelTypeUseK");
	WriByNS(ModelTypeUseK);
	WriByNS(FitzK);
	WriByNS(FitK);
	WriByNS(zK);
	WriByNS(K);
	WriByNS(BestSD);
	return true;
}
void GatingCell::ExportDataToClibboard(){
	OnOffData.ExportDataToClibboard();
}

bool GatingCell::LoadFromFile(CFile * fp){
	ReByNS(StepSize);
	ReByNS(MaxIter);
	ReByNS(tTest);
	ReByNS(VTest);

	ReByNS(Qmax);
	ReByNS(FitQmax);
	ReByNS(alpha0);
	ReByNS(zalpha);
	ReByNS(beta0);
	ReByNS(zTotal);
	ReByNS(zbeta);
	ReByNS(Fitalpha0);
	ReByNS(Fitzalpha);
	ReByNS(Fitbeta0);
	ReByNS(FitzTotal);

	if (!OnOffData.LoadFromFile(fp)){
		Alert0("could not load OnOff");
		return false;
	}
	if (!GeneralData.LoadFromFile(fp)){
		Alert0("could not load General Data");
		return false;
	}
//	ReByTRUENS(UseKOld);

//	ShowFloat(ModelTypeUseK, "ModelTypeUseK");


//ModelTypeUseK = 0;
	BOOL UseK;
	ReByTRUENS(UseK);
	if (UseK){
//		ModelTypeUseK = 1;
//		Alert0("Usek");
	}
	else{
//		Alert0("not Usek");
	}

	UseKVeryOld = UseK;
//	ByNS(ModelTypeUseK);
	ReByTRUENS(ModelTypeUseK);



	ReByTRUENS(FitzK);
	ReByTRUENS(FitK);
	ReByTRUENS(zK);
	ReByTRUENS(K);
	ReByTRUENS(BestSD);

	return true;
}


void GatingCell::FitIVAndTailWithMarkovModel(Model * pm){
	for (int ee=0; ee<2; ee++){
		if (!OnOffData.Entries[ee].DataPresent){
			Alert0("Not both IVand tail present  - no action");
			return;
		}
	}

	double NPMax = 0;
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & de = OnOffData.Entries[ee];
		if (de.NPointsPerTrace>NPMax){
			NPMax = de.NPointsPerTrace;
		}
	}

	NPoints = UINT(NPMax);
	Alloc();


	NParams = PutParamsInArrayIVAndTailMarkovModel(pm, Params);
	NParams = PutParamsInArrayIVAndTailMarkovModel(pm, BestParams);
//	ShowFloat(NParams, "NP");
	BestSD = CalcSDIVAndTailMarkovModel(pm);
//	ShowFloat(BestSD, "BestSD");

	Simplex simp;

	RungeKutta & rk = *prk;
	rk.ShowError = true;

	if (NParams>0){
		simp.InitSimplex(StepSize, Params, NParams);
		for (int iter=0; iter<MaxIter; iter++){
			if (InKey()==STOPKEY){
				goto EndFitGatingCellIVAndTailMarkov;
			}
			PutArrayInParamsIVAndTailMarkovModel(pm, Params);
			double SD = CalcSDIVAndTailMarkovModel(pm);
//			ShowFloat(SD, "SD");
			if (SD<BestSD){
				BestSD = SD;
//				ShowFloat(BestSD, "BestSD");
				simp.CopyParams(Params, BestParams);
			}
			simp.NextSimp(Params, SD);
		}
	}
EndFitGatingCellIVAndTailMarkov:
	if (NParams>0) simp.CopyParams(BestParams, Params);
	PutArrayInParamsIVAndTailMarkovModel(pm, BestParams);
	CalcSDIVAndTailMarkovModel(pm);

}

int GatingCell::PutParamsInArrayIVAndTailMarkovModel(Model * pm, double * Params){
	int NP=0;
	for (int ee=0; ee<2; ee++){
		OnOffData.Entries[ee].PutParamsInArray(NP, Params);
	}
	if (this->FitQmax){
		Params[NP] = Qmax;
		NP++;
	}
	for (int a=0; a<NEWMAXPARAMS; a++){
		if (pm->DoFitParam[a]){
			Params[NP] = pm->A[a];
			NP++;
		}
	}
	return NP;
}
int GatingCell::PutArrayInParamsIVAndTailMarkovModel(Model * pm, double * Params){
	int NP=0;
	for (int ee=0; ee<2; ee++){
		OnOffData.Entries[ee].PutArrayInParams(NP, Params);
	}
	if (this->FitQmax){
		Qmax = Params[NP];
		NP++;
	}
	for (int a=0; a<NEWMAXPARAMS; a++){
		if (pm->DoFitParam[a]){
			pm->A[a] = Params[NP];
			NP++;
		}
	}
	return NP;
}
double GatingCell::CalcSDIVAndTailMarkovModel(Model * pm){
	CalcFitIVAndTailMarkovModel(pm);
//	Alert0("Calc fit");
	double SD = 0;
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & de = OnOffData.Entries[ee];
		for (int t=0; t<de.NTraces; t++){
			double * d = de.Data[t];
			double * f = de.Fit[t];
			for (int i=de.OffsetForFit; i<(de.NPointsPerTrace-1); i++){
				double tp1 = d[i]- f[i];
				SD += tp1*tp1;
			}
		}
	}
//	ShowFloat(SD, "SD");
	return SD;
}

void GatingCell::CalcFitIVAndTailMarkovModel(Model * pm){
	for (int ee=0; ee<2; ee++){
		GatingCellDataEntry & de = OnOffData.Entries[ee];

		de.CopyTo(this->GeneralData);

		if (de.PerformLeakSubtraction){
			VTest = de.VTestP4;
			double SaveVHold = de.VHold;
			de.VHold = de.VHoldP4;
			GeneralData.VHold = de.VHold;
			PredictMarkov(pm);
			de.VHold = SaveVHold;
//			ShowFloat(de.VHoldP4, "de.VHoldP4");
//			ShowFloat(de.VTestP4, "de.VTestP4");
			for (int i=0; i<de.NPointsPerTrace; i++){
				leakI[i] = predictedI[i];
			}
		}
//		Alert0("leak0");
		for (int itrace=0; itrace<de.NTraces; itrace++){
			VTest = de.VStart+double(itrace)*de.DeltaV;
			GeneralData.VHold = de.VHold;

//			ShowFloat(VTest, "Vtest");
			PredictMarkov(pm);
			if (de.PerformLeakSubtraction){
				double scaleleak = -(VTest-de.VHold)/(de.VTestP4-de.VHoldP4);
//				ShowFloat(scaleleak, "scaleleak");
				for (int i=0; i<de.NPointsPerTrace; i++){
					predictedI[i] += scaleleak*leakI[i];
				}
			}
			for (int i=0; i<de.NPointsPerTrace; i++){
				de.Fit[itrace][i] = predictedI[i];
			}
//			Alert0("test");
		}
	}

}

void GatingCell::PredictMarkov(Model * pm){
	InitPredictionMarkov(pm);
	while (Step()){
	}
}

void GatingCell::InitPredictionMarkov(Model * pm){
	ka=1.0/(GeneralData.Rs*GeneralData.Cm*1e-6);
	KA=VTest*ka;
	kb=1.0/(GeneralData.Rm*GeneralData.Cm*1e-6);
	sumk = ka+kb;
	CinFDiv1000 = GeneralData.Cm*1e-15;
	INVCinFDiv1000 = 1.0/CinFDiv1000;
	NPoints = GeneralData.NPointsPerTrace+100; //UINT  (tTest / GeneralData.dt);
	Alloc();
	NPoints-=100;

	RungeKutta & rk = *prk;

//	double x[2], xp[2];

	MarkovStuff[0] = (int * )(this);
	MarkovStuff[1] = (int * ) (pm);

	int * STUFF = (int * )(&MarkovStuff[0]);

	double dxsav = GeneralData.dt/10.0;
	double eps = 1e-7;
	double h1=dxsav;
	double hmin=0;
	int kount;
	int NStates = pm->NEffectiveStates();
//	ShowFloat(NStates, "NStates");

	rk.Initialize(1+NStates, 0, NULL, NULL, &kount, dxsav, eps, h1, hmin, CalcDerivsMarkov, STUFF);

	time[0]=0;
	istep=0;

	y[0]=CalcVmSteadyState(GeneralData.VHold);
//	ShowFloat(GeneralData.VHold, "GeneralData.VHold");
//	ShowFloat(y[0], "Vm0");
	pm->Init(y[0]);
	double pinf[1000];
	double current;
	pm->PinfAllStates(current, pinf);
	for (int i=0; i<NStates; i++){
		y[i+1]=pinf[i];

	}
//	ShowFloat(y[1], "Q0");
}
void GatingCell::CalcDerivsMarkov(double t, double * y, double * yp, int * STUFF){
	int ** MarkovStuff = (int **) (STUFF);
	GatingCell & gc = *( (GatingCell *) (MarkovStuff[0]));
//	ShowFloat(gc.alpha0, "alpha0");
	Model * pm = (Model * )(MarkovStuff[1]);
//	int NStates = pm->NEffectiveStates();
//	ShowFloat(NStates, "NStates");
	gc.DoCalcDerivsMarkov(t, y, yp, pm);
}
void GatingCell::DoCalcDerivsMarkov(double t, double * y, double * yp, Model * pm){
	double v=y[0];
/*	for (int i=1; i<=pm->NS; i++){
		yp[i]=0;
	}
*/
	pm->DoCalcDerivs(t, y+1, yp+1, v);

	double current = Qmax*pm->EvaluateSpecialCurrentFunction(v, y+1, pm->m.R);
//	ShowFloat(current, "current");

	yp[0] = KA - v*sumk - current*INVCinFDiv1000; // dQ/dt is in C/s; dV/dt is in mV/s

}


CollectionOfGatingCells::CollectionOfGatingCells(){
	BestSD = 0;
	NormalizeEachFile = true;
	ModelType = 0;
	NCells = 0;
	Cl[0]=9;
	Cl[1]=35;
	Cl[2]=140;
	alpha0[0]=20;
	alpha0[1]=50;
	alpha0[2]=100;
	beta0[0]=beta0[1]=beta0[2]=100000;
	zalpha=0.4;
	zTotal = 1;
	zK=0.4;
	K=100.0;
	zon=0.2;
	koff=10000;
	Fitalpha0 = Fitzalpha = Fitbeta0 = FitzTotal = FitzK = FitK = Fitzon = Fitkoff = false;
	MaxIter = 100;
	StepSize = 0.1;
	BestSD = 1e300;
	ConcentrationIndex = 31;

	K=18673.2336973926;
	koff=exp(10.45);
	zK=0.971293907328077;
	zon=0.470403951771716;
	double r0=465.263497458824;
	alpha0[0]=exp(5.79);
	beta0[0]=r0*alpha0[0];
	zTotal=0.778538508405141;
	zalpha=0.169098308490662;

	K=383.0850331;
	koff=exp(10.178);
	zK=0.228290777459587;
	zon=0.522375204247428;
	r0=376.958559297334;
	alpha0[0]=exp(5.09);
	beta0[0]=r0*alpha0[0];
	zTotal=0.950586620830286;
	zalpha=0.382242172054162;
}
CollectionOfGatingCells::~CollectionOfGatingCells(){
}


bool CollectionOfGatingCells::SaveThis(CFile * fp){
//	WriByNS(NCells);
	for (int i=0; i<100; i++){
		WriByNS(alpha0[i]);
		WriByNS(beta0[i]);
//		WriByNS(IndexCl[i]);
//		WriByNS(GatFileName[i]);
	}
	WriByNS(BestSD);
	WriByNS(Fitalpha0);
	WriByNS(Fitbeta0);
	WriByNS(FitK);
	WriByNS(Fitzalpha);
	WriByNS(FitzK);
	WriByNS(FitzTotal);
	WriByNS(K);
	WriByNS(MaxIter);
	WriByNS(ModelType);
	WriByNS(StepSize);
	WriByNS(zalpha);
	WriByNS(zK);
	WriByNS(zTotal);
	SaveString(rootname, *fp);
	WriByNS(NormalizeEachFile);
	WriByNS(ConcentrationIndex);
	WriByNS(zon);
	WriByNS(koff);
	WriByNS(Fitzon);
	WriByNS(Fitkoff);
	return true;

}
bool CollectionOfGatingCells::LoadThis(CFile * fp){
//	ReByNS(NCells);
	for (int i=0; i<100; i++){
		ReByNS(alpha0[i]);
		ReByNS(beta0[i]);
//		ReByNS(IndexCl[i]);
//		ReByNS(GatFileName[i]);
	}
	ReByNS(BestSD);
	ReByNS(Fitalpha0);
	ReByNS(Fitbeta0);
	ReByNS(FitK);
	ReByNS(Fitzalpha);
	ReByNS(FitzK);
	ReByNS(FitzTotal);
	ReByNS(K);
	ReByNS(MaxIter);
	ReByNS(ModelType);
	ReByNS(StepSize);
	ReByNS(zalpha);
	ReByNS(zK);
	ReByNS(zTotal);

	if (!LoadString(rootname, *fp)){
		return true;
	}
	ReByTRUENS(NormalizeEachFile);
	ReByTRUENS(ConcentrationIndex);
	ReByTRUENS(zon);
	ReByTRUENS(koff);
	ReByTRUENS(Fitzon);
	ReByTRUENS(Fitkoff);

	return true;

}
void CollectionOfGatingCells::CopyTo(CollectionOfGatingCells & to){
	for (int i=0; i<100; i++){
		to.alpha0[i] = alpha0[i];
		to.beta0[i] = beta0[i];
//		to.IndexCl[i] = IndexCl[i];
	//	to.GatFileName[i] = GatFileName[i];
	}
	to.ConcentrationIndex = ConcentrationIndex;
	to.BestSD = BestSD;
	to.Fitalpha0 = Fitalpha0;
	to.Fitbeta0 = Fitbeta0;
	to.FitK = FitK;
	to.Fitzalpha = Fitzalpha;
	to.FitzK = FitzK;
	to.FitzTotal = FitzTotal;
	to.K = K;
	to.MaxIter = MaxIter;
	to.ModelType = ModelType;
	to.StepSize = StepSize;
	to.zalpha = zalpha;
	to.zK = zK;
	to.zTotal = zTotal;
	to.Fitkoff = Fitkoff;
	to.koff = koff;
	to.Fitzon = Fitzon;
	to.zon = zon;

	to.NormalizeEachFile = NormalizeEachFile;

//	to.rootname = rootname;


}

void CollectionOfGatingCells::ReadFiles(CString & NameFile){
	CFile InFile;
	if (!InFile.Open(NameFile, CFile::modeRead)){
		Alert0(_T("Could not open"));
		return;
	}
	GetPath(NameFile, rootname);
	int NTestCells = 0;
	CString Lines[200];
	while (true){
		if (NTestCells>190){
			Alert0("more than 190 lines are not accepted");
			break;
		}
		if (!ReadLine(InFile, Lines[NTestCells])){
			break;
		}
		CString & l = Lines[NTestCells];
		// Analyze: first file name, relative to this dir, then Cl conc, separated by ;
		int semcolonpos = l.Find(';');
		if (semcolonpos<2){
			Alert0("could not find a ; in line");
			break;
		}
		CString name = l.Left(semcolonpos);
		CString FileName = MakeFileName(name);
		if (!FileExists(FileName)){
			CString a = FileName + CString(" does not exist. "); 
			Alert0(a);
			break;
		}
		GatFileName[NTestCells] = name;

		int length = l.GetLength();

		CString ClString = l.Right(length-semcolonpos-1);
//		Alert0(ClString);
		
		double Cl = atof(ClString);
		if (Cl>100.0){
			IndexCl[NTestCells] = 2;
		}
		else{
			if (Cl>20.0){
				IndexCl[NTestCells] = 1;
			}
			else{
				IndexCl[NTestCells] = 0;
			}
		}
//		ShowFloat(IndexCl[NTestCells], "index");
//		Alert0(GatFileName[NTestCells] );

		NTestCells++;
	}

	if (!LoadData(NTestCells)){
		NCells = 0;
	}
	else{
		NCells = NTestCells;
	}



//	bool FileExists(CString Name);


}

CString CollectionOfGatingCells::MakeFileName(CString file){
//	return (rootname + CString("\\") + file);
	return (rootname + file);
}

void CollectionOfGatingCells::SaveData(){
	for (int i=0; i<NCells; i++){
		CString Name = MakeFileName(GatFileName[i]);
//		Alert0(Name);
		CFile OutFile;
		if (OutFile.Open(Name, CFile::modeCreate | CFile::modeWrite)){
			TheCells[i].SaveOnFile(&OutFile);
		}
		else{
			Alert0(CString("Cannot create output file: ") + Name);
		}

//		CFile file ( MakeFileName(GatFileName[i]), CFile::modeCreate | CFile::modeWrite);
//		TheCells[i].SaveOnFile(&file);
	}
}
bool CollectionOfGatingCells::LoadData(int NTestCells){
	for (int i=0; i<NTestCells; i++){
		CFile InFile;
		CString Name = MakeFileName(GatFileName[i]);
//		Alert0(Name);
		if (!InFile.Open(Name, CFile::modeRead)){
			Alert0(_T("Could not open") + Name);
			return false;
		}
		if (!TheCells[i].LoadFromFile(&InFile)){
			Alert0(CString("Could not load data from file ")+Name);
			return false;
		}
		else{
//			ShowFloat(TheCells[i].OnOffData.Entries[0].NTraces, "0 NT");
//			ShowFloat(TheCells[i].OnOffData.Entries[0].NPointsPerTrace, " 0 NP");
//			ShowFloat(TheCells[i].OnOffData.Entries[1].NTraces, "1 NT");
//			ShowFloat(TheCells[i].OnOffData.Entries[1].NPointsPerTrace, " 1 NP");
		}
	}
	return true;
}


int CollectionOfGatingCells::PutParamsInArray(double * Params){
	int NP=0;
	for (int i=0; i<NCells; i++){
		Params[NP] = TheCells[i].Qmax;
		NP++;
	}
	if (Fitzalpha){
		Params[NP]=zalpha;
		NP++;
	}
	if (FitzTotal){
		Params[NP]=zTotal;
		NP++;
	}
	if (Fitalpha0){
		Params[NP] = alpha0[0];
		NP++;
	}
	if (Fitbeta0){
		Params[NP]=beta0[0];
		NP++;
	}

//	if (ModelType == 0){
	if ((ModelType == 0) || (ModelType == 3)){
		if (FitzK){
			Params[NP] = zK;
			NP++;
		}
		if (FitK){
			Params[NP] = log(K);
			NP++;
		}
	}
	if (ModelType == 1){
		if (Fitalpha0){
			Params[NP] = alpha0[1];
			NP++;
			Params[NP] = alpha0[2];
			NP++;
		}
	}
	if (ModelType == 2){
		if (Fitbeta0){
			Params[NP]=beta0[1];
			NP++;
			Params[NP]=beta0[2];
			NP++;
		}
	}
	if ((ModelType == 4) || (ModelType == 5)){
		if (FitzK){
			Params[NP] = zK;
			NP++;
		}
		if (FitK){
			Params[NP] = log(K);
			NP++;
		}
		if (Fitzon){
			Params[NP] = zon;
			NP++;
		}
		if (Fitkoff){
			Params[NP] = log(koff);
			NP++;
		}

	}

	return NP;
}
int CollectionOfGatingCells::PutArrayInParams(double * Params){
	int NP=0;
	for (int i=0; i<NCells; i++){
		TheCells[i].Qmax = Params[NP];
		NP++;
	}
//	ShowFloat(TheCells[0].Qmax, "TheCell[0].Qmax");
	if (Fitzalpha){
		zalpha = Params[NP];
		NP++;
	}
	if (FitzTotal){
		zTotal = Params[NP];
		NP++;
	}
	if (Fitalpha0){
		alpha0[0] = Params[NP];
		NP++;
	}
	if (Fitbeta0){
		beta0[0] = Params[NP];
		NP++;
	}
//	if (ModelType == 0){
	if ((ModelType == 0) || (ModelType == 3)){
		if (FitzK){
			zK = Params[NP];
			NP++;
		}
		if (FitK){
			K = exp(Params[NP]);
			NP++;
		}
	}
	if (ModelType == 1){
		if (Fitalpha0){
			alpha0[1] = Params[NP];
			NP++;
			alpha0[2] = Params[NP];
			NP++;
		}
	}
	if (ModelType == 2){
		if (Fitbeta0){
			beta0[1] = Params[NP];
			NP++;
			beta0[2] = Params[NP];
			NP++;
		}
	}
	if ((ModelType == 4) || (ModelType == 5)){
		if (FitzK){
			zK = Params[NP];
			NP++;
		}
		if (FitK){
			K=exp(Params[NP]);
			NP++;
		}
		if (Fitzon){
			zon=Params[NP];
			NP++;
		}
		if (Fitkoff){
			koff=exp(Params[NP]);
			NP++;
		}
	}

//	ShowFloat(NP, "NP");
	return NP;
}
void CollectionOfGatingCells::Fit(){
	if (NCells<1){
		return;
	}
	double NPMax = 0;
	for (int i=0; i<NCells; i++){
		for (int ee=0; ee<2; ee++){
			GatingCellDataEntry & de = TheCells[i].OnOffData.Entries[ee];
			if (de.NPointsPerTrace>NPMax){
				NPMax = de.NPointsPerTrace;
			}
		}
	}

	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		gc.NPoints = UINT(NPMax);
		gc.Alloc();
	}


	NParams = PutParamsInArray(Params);
	NParams = PutParamsInArray(BestParams);
//	ShowFloat(NParams, "NP");
	BestSD = CalcSD();
//	ShowFloat(BestSD, "BestSD");

	Simplex simp;

	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		RungeKutta & rk = *gc.prk;
		rk.ShowError = true;
	}

	simp.InitSimplex(StepSize, Params, NParams);
	for (int iter=0; iter<MaxIter; iter++){
		if (InKey()==STOPKEY){
			goto EndFitCollectionCells;
		}
		PutArrayInParams(Params);
		double SD = CalcSD();
//			ShowFloat(SD, "SD");
		if (SD<BestSD){
			BestSD = SD;
//				ShowFloat(BestSD, "BestSD");
			simp.CopyParams(Params, BestParams);
		}
		simp.NextSimp(Params, SD);
	}
EndFitCollectionCells:
	if (NParams>0)	simp.CopyParams(BestParams, Params);
	PutArrayInParams(BestParams);
	CalcSD();


}
void CollectionOfGatingCells::Copyparamnstoclipboard(){
	BOOL first;
	CString c;
	
	first = true;
	AddDoubleToString(first, zTotal, c);
	c += "\r\n";

	first = true;
	AddDoubleToString(first, zalpha, c);
	c += "\r\n";

	first = true;
	AddDoubleToString(first, alpha0[0], c);
	c += "\r\n";

	first = true;
	AddDoubleToString(first, beta0[0], c);
	c += "\r\n";

	first = true;
	AddDoubleToString(first, zK, c);
	c += "\r\n";

	first = true;
	AddDoubleToString(first, K, c);
	
	CopyTextToClipboard(c);
}

double CollectionOfGatingCells::CalcSD(){
	double SD = 0;
//	ShowFloat(NCells, "NCells");
	for (int i=0; i<NCells; i++){
//		ShowFloat(i, "i");
//		ShowFloat(IndexCl[i], "IndexCl[i]");
		GatingCell & gc = TheCells[i];

		// Copy Params here!
		gc.zTotal = zTotal;
		gc.zalpha = zalpha;
//		ShowFloat(zalpha, "zalpha");
		gc.ModelTypeUseK = 0;
//		gc.UseKOld = 0;
		if ((ModelType == 0) || (ModelType == 3)){

			if (ModelType == 0){
				gc.ModelTypeUseK = 1;
			}
			if (ModelType == 3){
				gc.ModelTypeUseK = 2;
			}

//			gc.UseKOld = true;
			gc.alpha0 = alpha0[0];
			gc.beta0 = beta0[0];
			gc.K = Cl[IndexCl[i]]/K;
			gc.zK = zK;
		}
		else{
			gc.ModelTypeUseK = 0;
//			gc.UseKOld = false;
//			gc.UseK = false;
			if (ModelType == 1){
				gc.alpha0 = alpha0[IndexCl[i]];
				gc.beta0 = beta0[0];
			}
			else{
				gc.alpha0 = alpha0[0];
				gc.beta0 = beta0[IndexCl[i]];
			}
		}
		double SDD = gc.CalcSDIVAndTail();
		if (NormalizeEachFile){
			double m= gc.OnOffData.AbsMax;
			m *= m;
			SDD /= m;
		}
//		SD += gc.CalcSDIVAndTail();
		SD += SDD;
	}
	return SD;

};

void CollectionOfGatingCells::FitMarkov(Model * PM){
	pm = PM;

	if (!pm) return;
	if (!pm->IsReady()) return;

	if (NCells<1){
		return;
	}
	double NPMax = 0;
	for (int i=0; i<NCells; i++){
		for (int ee=0; ee<2; ee++){
			GatingCellDataEntry & de = TheCells[i].OnOffData.Entries[ee];
			if (de.NPointsPerTrace>NPMax){
				NPMax = de.NPointsPerTrace;
			}
		}
	}

	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		gc.NPoints = UINT(NPMax);
		gc.Alloc();
	}


	NParams = PutParamsInArrayMarkov(Params);
	NParams = PutParamsInArrayMarkov(BestParams);
//	ShowFloat(NParams, "NP");
	BestSD = CalcSDMarkov();
//	ShowFloat(BestSD, "BestSD");

	Simplex simp;

	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		RungeKutta & rk = *gc.prk;
		rk.ShowError = true;
	}

	simp.InitSimplex(StepSize, Params, NParams);
	for (int iter=0; iter<MaxIter; iter++){
		if (InKey()==STOPKEY){
			goto EndFitCollectionCellsMarkov;
		}
		PutArrayInParamsMarkov(Params);
		double SD = CalcSDMarkov();
//			ShowFloat(SD, "SD");
		if (SD<BestSD){
			BestSD = SD;
//				ShowFloat(BestSD, "BestSD");
			simp.CopyParams(Params, BestParams);
		}
		simp.NextSimp(Params, SD);
	}
EndFitCollectionCellsMarkov:
	if (NParams>0)	simp.CopyParams(BestParams, Params);
	PutArrayInParamsMarkov(BestParams);
	CalcSDMarkov();


}
double CollectionOfGatingCells::CalcSDMarkov(){
	double SD = 0;
//	ShowFloat(NCells, "NCells");
	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		pm->A[ConcentrationIndex] = Cl[IndexCl[i]];

		double SDD = gc.CalcSDIVAndTailMarkovModel(pm);
		if (NormalizeEachFile){
			double m= gc.OnOffData.AbsMax;
			m *= m;
			SDD /= m;
		}
//		SD += gc.CalcSDIVAndTail();
		SD += SDD;
	}
	return SD;

};

int CollectionOfGatingCells::PutParamsInArrayMarkov(double * Params){
	int NP=0;
	for (int i=0; i<NCells; i++){
		Params[NP] = TheCells[i].Qmax;
		NP++;
	}
	for (int a=0; a<NEWMAXPARAMS; a++){
		if (pm->DoFitParam[a]){
			Params[NP] = pm->A[a];
			NP++;
		}
	}

	return NP;
}
int CollectionOfGatingCells::PutArrayInParamsMarkov(double * Params){
	int NP=0;
	for (int i=0; i<NCells; i++){
		TheCells[i].Qmax = Params[NP];
		NP++;
	}
	for (int a=0; a<NEWMAXPARAMS; a++){
		if (pm->DoFitParam[a]){
			pm->A[a] = Params[NP];
			NP++;
		}
	}
	return NP;
}

void CollectionOfGatingCells::Fit3StateModel(){
	if ((ModelType != 4) && (ModelType != 5)){
		Alert0("Model type must be 4 or 5");
		return;
	}

	if (NCells<1){
		return;
	}
	double NPMax = 0;
	for (int i=0; i<NCells; i++){
		for (int ee=0; ee<2; ee++){
			GatingCellDataEntry & de = TheCells[i].OnOffData.Entries[ee];
			if (de.NPointsPerTrace>NPMax){
				NPMax = de.NPointsPerTrace;
			}
		}
	}

	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		gc.NPoints = UINT(NPMax);
		gc.Alloc();
	}


	NParams = PutParamsInArray(Params);
	NParams = PutParamsInArray(BestParams);
//	ShowFloat(NParams, "NP");
	BestSD = CalcSD3States();
//	ShowFloat(BestSD, "BestSD");

	Simplex simp;

	for (int i=0; i<NCells; i++){
		GatingCell & gc = TheCells[i];
		RungeKutta & rk = *gc.prk;
		rk.ShowError = true;
	}

	simp.InitSimplex(StepSize, Params, NParams);
	for (int iter=0; iter<MaxIter; iter++){
		if (InKey()==STOPKEY){
			goto EndFitCollectionCells3States;
		}
		PutArrayInParams(Params);
		double SD = CalcSD3States();
//			ShowFloat(SD, "SD");
		if (SD<BestSD){
			BestSD = SD;
//				ShowFloat(BestSD, "BestSD");
			simp.CopyParams(Params, BestParams);
		}
		simp.NextSimp(Params, SD);
	}
EndFitCollectionCells3States:
	if (NParams>0)	simp.CopyParams(BestParams, Params);
	PutArrayInParams(BestParams);
	CalcSD3States();


}
double CollectionOfGatingCells::CalcSD3States(){
	double SD = 0;
//	ShowFloat(NCells, "NCells");
	for (int i=0; i<NCells; i++){
//		ShowFloat(i, "i");
//		ShowFloat(IndexCl[i], "IndexCl[i]");
		GatingCell & gc = TheCells[i];

		// Copy Params here!

		gc.ModelTypeUseK = this->ModelType;

		gc.zTotal = zTotal;
		gc.zalpha = zalpha;
		gc.zon = zon;
		gc.koff = koff;
		gc.alpha0 = alpha0[0];
		gc.beta0 = beta0[0];
		gc.K = Cl[IndexCl[i]]/K;
		gc.zK = zK;

		double SDD = gc.CalcSDIVAndTail();

		if (NormalizeEachFile){
			double m= gc.OnOffData.AbsMax;
			m *= m;
			SDD /= m;
		}
		SD += SDD;
	}
	return SD;
}



void GatingCell::CalcDerivs3StateModel(double t, double * y, double * yp, int * STUFF){
	GatingCell & gc = *( (GatingCell *) (STUFF));
	gc.DoCalcDerivs3StateModel(t, y, yp);
}
void GatingCell::DoCalcDerivs3StateModel(double t, double * y, double * yp){
	double v=y[0];
	double w[4];
	CalcW(v, w);

	double y1=y[1];
	double y2=y[2];
	double y0=1.0-y1-y2;
//	ShowFloat(y[0], "y[0]");
//	ShowFloat(y[1], "y[1]");
//	ShowFloat(y[2], "y[2]");
	double current;

	if (ModelTypeUseK==4){
		yp[1]=w[0]+(w[3]-w[0])*y2-(w[1]+w[2]+w[0])*y1;
		yp[2]=w[2]*y1-w[3]*y2;
		current = y0*w[0]*zK+y1*(w[2]*zTotal-w[1]*zK)-y2*w[3]*zTotal;
	}
	if (ModelTypeUseK==5){
		yp[1]=w[2]+(w[1]-w[2])*y2-(w[3]+w[2]+w[0])*y1;
		yp[2]=w[0]*y1-w[1]*y2;
		current = y0*w[2]*zTotal+y1*(w[0]*zK-w[3]*zTotal)-y2*w[1]*zK;
	}
	current *= this->Qmax;

	yp[0] = KA - v*sumk - current*INVCinFDiv1000; // dQ/dt is in C/s; dV/dt is in mV/s

}

