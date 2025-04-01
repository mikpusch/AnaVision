#include "stdafx.h"
#include "BufferedDiffusion.h"
#include <math.h>

SimParams::SimParams(){
	Init();
}

void SimParams::Init(){

	m_NumberOfPoints  = 0;
	m_MaxNumberOfCurves = 0;
	m_NumberOfCurves = 0;
	m_Hinitial = 1e-7;
	m_Binitial = 1e-3;
	m_Radius = NULL;
	m_H = NULL;
	m_B = NULL;
	m_dr=1e-5;
	m_dt=1e-5;
	m_Norm = 0;
}


SimParams::~SimParams(){
	DeAlloc();
}



void SimParams::Alloc(){
	m_Radius = new double [m_NumberOfPoints];

	m_H = new double * [m_MaxNumberOfCurves];

	m_B = new double * [m_MaxNumberOfCurves];

	for (int i=0; i<m_MaxNumberOfCurves;i++){
		m_H[i] = new double [m_NumberOfPoints];

		m_B[i] = new double [m_NumberOfPoints];
	}
}

void SimParams::DeAlloc(){
	for (int i=0; i<m_MaxNumberOfCurves;i++){
		
		if (m_B[i]){
			delete [] m_B[i];
		}
		m_B[i] = NULL;

		if (m_H[i]){
			delete [] m_H[i];
		}
		m_H[i] = NULL;
	}

	delete [] m_B;
	m_B = NULL;

	delete [] m_H;
	m_H = NULL;

	if (m_Radius){
		delete [] m_Radius;
	}
	m_Radius=NULL;

}


**********************************************************************

SimulationMemory::SimulationMemory(){

	initialized = false;

	m_maxnumSimulations = 3;

	Sims = NULL;

	m_NumberOfSimulations = 0;

	DoShow = false;

}

SimulationMemory::~SimulationMemory(){
	DestroyAll();
}

bool SimulationMemory::SaveOnFile(CFile & f){
	CString c="SIMULATIONMEMORY";
	CFile * fp = &f;

	f.Write(c, c.GetLength());

	WriByNS(this->m_NumberOfSimulations);

	for (int i=0; i<this->m_NumberOfSimulations; i++){
		WriByNS(this->Sims[i].m_Binitial);
		WriByNS(this->Sims[i].m_dr);
		WriByNS(this->Sims[i].m_dt);
		WriByNS(this->Sims[i].m_Hinitial);
		WriByNS(this->Sims[i].m_MaxNumberOfCurves);
		WriByNS(this->Sims[i].m_Norm);
		WriByNS(this->Sims[i].m_NumberOfCurves);
		WriByNS(this->Sims[i].m_NumberOfPoints);
		for (int j=0; j<this->Sims[i].m_NumberOfPoints; j++){
			WriByNS(this->Sims[i].m_Radius[j]);
		}
		for (int k=0; k<this->Sims[i].m_NumberOfCurves; k++){
			for (int j=0; j<this->Sims[i].m_NumberOfPoints; j++){
				WriByNS(this->Sims[i].m_H[k][j]);
				WriByNS(this->Sims[i].m_B[k][j]);
				if (j==0){
//					ShowFloat(this->Sims[i].m_H[k][j], "ss.Sims[i].m_H[k][j]");
				}
				if (j==(this->Sims[i].m_NumberOfPoints-1)){
//					ShowFloat(this->Sims[i].m_H[k][j], "ss.Sims[i].m_H[k][j]");
				}

			}
		}
	}


	return true;
}

bool SimulationMemory::DoRead(SimulationMemory & ss, CFile & f){
	CFile * fp = &f;

	CString c="SIMULATIONMEMORY";
	char s[100];
	int l=c.GetLength();
	if (f.GetLength()<l){
		return false;
	}
	f.Read(&s[0], l);
	s[l]=0;
	CString c2=CString(s);
	if (c2.Compare(c) != 0){
		Alert0(CString("File does not begin with ")+c );
		return false;
	}

	ReByNS(ss.m_NumberOfSimulations);
	if (ss.m_NumberOfSimulations<=0) return false;

	ss.m_maxnumSimulations = ss.m_NumberOfSimulations;
	ss.Create(ss.m_NumberOfSimulations);
	ss.m_NumberOfSimulations = ss.m_maxnumSimulations;
	ss.m_ActualSimulation = 0;

//	ShowFloat(ss.m_NumberOfSimulations, "NS");

	for (int i=0; i<ss.m_NumberOfSimulations; i++){
		ReByNS(ss.Sims[i].m_Binitial);
		ReByNS(ss.Sims[i].m_dr);
//		ShowFloat(ss.Sims[i].m_dr, "dr");
		ReByNS(ss.Sims[i].m_dt);
		ReByNS(ss.Sims[i].m_Hinitial);
		ReByNS(ss.Sims[i].m_MaxNumberOfCurves);
		ReByNS(ss.Sims[i].m_Norm);
		ReByNS(ss.Sims[i].m_NumberOfCurves);
		ReByNS(ss.Sims[i].m_NumberOfPoints);
//		ShowFloat(ss.Sims[i].m_NumberOfPoints, "NP");
//		ShowFloat(ss.Sims[i].m_MaxNumberOfCurves, "m_MaxNumberOfCurves");
		if (ss.Sims[i].m_NumberOfCurves<=0){
			return false;
		}
		if (ss.Sims[i].m_NumberOfPoints<=0){
			return false;
		}
		if (ss.Sims[i].m_MaxNumberOfCurves<=0){
			return false;
		}
		if (ss.Sims[i].m_MaxNumberOfCurves<ss.Sims[i].m_NumberOfCurves){
			return false;
		}

		ss.Sims[i].Alloc();

		for (int j=0; j<ss.Sims[i].m_NumberOfPoints; j++){
			ReByNS(ss.Sims[i].m_Radius[j]);
//			if (j==(ss.Sims[i].m_NumberOfPoints-1)){
//				ShowFloat(ss.Sims[i].m_Radius[j], "ss.Sims[i].m_Radius[j]");
//			}
		}
//		ShowFloat(ss.Sims[i].m_NumberOfCurves, "m_NumberOfCurves");
		for (int k=0; k<ss.Sims[i].m_NumberOfCurves; k++){
//			ShowFloat(ss.Sims[i].m_NumberOfPoints, "m_NumberOfPoints");
			for (int j=0; j<ss.Sims[i].m_NumberOfPoints; j++){
				ReByNS(ss.Sims[i].m_H[k][j]);
				ReByNS(ss.Sims[i].m_B[k][j]);
				if (j==0){
//					ShowFloat(ss.Sims[i].m_H[k][j], "ss.Sims[i].m_H[k][j]");
				}
				if (j==(ss.Sims[i].m_NumberOfPoints-1)){
//					ShowFloat(ss.Sims[i].m_H[k][j], "ss.Sims[i].m_H[k][j]");
				}
			}
		}
	}



	return true;
}
	

bool SimulationMemory::LoadFromFile(CFile & f){
	SimulationMemory ss;

	if (!DoRead(ss, f)){
		Alert0("Error reading file");
		return false;
	}

	f.SeekToBegin();

	DestroyAll();

	DoRead(*this, f);

	return true;
}

void SimulationMemory::Create(int maxnumSimulations){

	if (initialized){
		DestroyAll();
	}

	initialized = true;

	m_maxnumSimulations = maxnumSimulations;

	Sims = new SimParams[m_maxnumSimulations];

	m_ActualSimulation = -1;
	m_NumberOfSimulations = 0;

}

void SimulationMemory::DestroyAll(){

	if(!initialized)return;

	if (Sims){
		delete [] Sims;
	}
	initialized = false;

}


void SimulationMemory::AddSimulation(int NumberOfPoints, int MaxNumberOfCurves, double InitialpH, double InitialB, 
											double dr, double dt, double *Radius){

	if(!initialized)return;

	m_ActualSimulation++;

	if (m_NumberOfSimulations< m_maxnumSimulations){
		m_NumberOfSimulations++ ;
	}


	if(m_ActualSimulation >= m_maxnumSimulations){

		m_ActualSimulation = 0;

	}

	SimParams & Sim = Sims[m_ActualSimulation];

	Sim.DeAlloc();

	Sim.m_NumberOfPoints = NumberOfPoints;
	Sim.m_MaxNumberOfCurves = MaxNumberOfCurves;
	Sim.m_Hinitial =  pow(10.0, -InitialpH);
	Sim.m_Binitial = InitialB;
	Sim.m_dr = dr;
	Sim.m_dt = dt;

	Sim.Alloc();


	for(int i=0;i<NumberOfPoints;i++){

		Sim.m_Radius[i] = Radius[i];

		for(int j=0;j<MaxNumberOfCurves;j++){

			Sim.m_H[j][i] = Sim.m_Hinitial;

			Sim.m_B[j][i] = Sim.m_Binitial;

		}

	}
	Sim.m_NumberOfCurves = 0;

	
}

void SimulationMemory::AddCurve(double *H, double *B){

	if(!initialized)return;

	SimParams & Sim = Sims[m_ActualSimulation];

	int Curve =Sim.m_NumberOfCurves;

	if (Curve >= Sim.m_MaxNumberOfCurves) return;
	
	for(int i=0;i<Sim.m_NumberOfPoints;i++){
	
		Sim.m_H[Curve][i] = H[i];

		Sim.m_B[Curve][i] = B[i];

	}

	Sim.m_NumberOfCurves++;
}

int SimulationMemory::GetAbsoluteSimulationIndex(int relindex){
	relindex = (m_ActualSimulation + relindex) % m_maxnumSimulations ;
	return (m_maxnumSimulations + relindex) % m_maxnumSimulations ;
}

void SimulationMemory::GetCurve(int index, int curve, double *H, double *B){

	if(!initialized)return;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	if(curve>= Sim.m_MaxNumberOfCurves)return;

	for(int i=0;i<Sim.m_NumberOfPoints;i++){

		H[i] = Sim.m_H[curve][i];

		B[i] = Sim.m_B[curve][i];

	}

}


int SimulationMemory::GetNumberOfPoints(int index){

	if(!initialized)return 0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return Sim.m_NumberOfPoints;

}


int SimulationMemory::GetNumberOfCurves(int index){

	if(!initialized)return 0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return Sim.m_NumberOfCurves;
}

double SimulationMemory::GetRadius(int index, int j){

	if(!initialized)return 0.0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	if(j>=Sim.m_NumberOfPoints)return 0.0;

	return Sim.m_Radius[j];

}

void SimulationMemory::GetValues(int index, int curve, int j, double & H, double & B){
	if(!initialized)return ;

	if (DoShow) ShowFloat(GetAbsoluteSimulationIndex(index), "real ind");

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

//	ShowFloat(curve, "curve");

	if (curve >= Sim.m_NumberOfCurves){
		return;
	}
	if (j>=Sim.m_NumberOfPoints){
		return;
	}

	H=Sim.m_H[curve][j];
	B=Sim.m_B[curve][j];
	
}

double SimulationMemory::GetpHBulk(int index){
	if(!initialized)return 0.0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return -log10(Sim.m_Hinitial);

}


double SimulationMemory::GetInitialFreeBuffer(int index){
	if(!initialized)return 0.0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return Sim.m_Binitial;

}
double SimulationMemory::Getdt(int index){
	if(!initialized)return 0.0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return Sim.m_dt;

}

double SimulationMemory::Getdr(int index){
	if(!initialized)return 0.0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return Sim.m_dr;

}

int SimulationMemory::GetMaxNumberOfSimulations(){
	return this->m_maxnumSimulations;
}
int SimulationMemory::GetNumberOfSimulations(){
	return m_NumberOfSimulations;
}

void SimulationMemory::SetNorm(int index, double Norm){
	if(!initialized)return ;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	Sim.m_Norm = Norm;
}

double SimulationMemory::GetNorm(int index){
	if(!initialized)return 0.0;

	SimParams & Sim = Sims[GetAbsoluteSimulationIndex(index)];

	return Sim.m_Norm;
}


