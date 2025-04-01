#include "stdafx.h"
#include "BufferedDiffusion.h"


SeriesSimulation::SeriesSimulation(){
	NValues = 0;
	for (int i=0; i<MAX_NVALUES_SERIES_SIMULATION; i++){
		dt[i]=dr[i]=norm[i]=0;
	}
}

SeriesSimulation::~SeriesSimulation(){
}

void SeriesSimulation::MakeToString(CString & c){
	c="";
	for (int i=0; i<NValues; i++){
		char s[40];
		_gcvt(dr[i], 10, s);
		c += CString(s)+"  ";
		_gcvt(dt[i], 10, s);
		c += CString(s)+"\r\n";
	}
}

bool SeriesSimulation::WriteOnFile(CFile &f){
	CFile * fp = &(f);
	WriByNS(NValues);
	for (int i=0; i<NValues; i++){
		WriByNS(	dt[i]);
		WriByNS(	dr[i]);
		WriByNS(	norm[i]);
	}
	return true;
}
bool SeriesSimulation::LoadFromFile(CFile &f){
	CFile * fp = &(f);
	ReByNS(NValues);
	if (NValues<0) NValues = 0;
	for (int i=0; i<NValues; i++){
		ReByNS(	dt[i]);
		ReByNS(	dr[i]);
		ReByNS(	norm[i]);
	}
	return true;
}


void SeriesSimulation::AddString(CString & c){
	double x, y;
	if (NValues>=MAX_NVALUES_SERIES_SIMULATION){
		return;
	}
	if (ReadTwoDoublesInString(c, x, y)){
		dr[NValues]=x;
		dt[NValues]=y;
		NValues++;
	}
}

int SeriesSimulation::GetMinIndex(){
	if (NValues<2){
		return 0;
	}
	int MinIndex = 0;
	double mindr = dr[0];
	double mindt = dt[0];
	for (int i=1; i<NValues; i++){
		if ((dr[i]<=mindr) && (dt[i]<=mindt)){
			MinIndex = i;
			mindr=dr[i];
			mindt=dt[i];
		}
	}
	return MinIndex;
}

