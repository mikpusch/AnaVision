// Simplex.h
#pragma once
//#ifndef SIMPLEX_MIK
//#define SIMPLEX_MIK

#define  SimpSize  50

class Simplex
{
public:
	  Simplex();
	  void InitSimplex(double StepS, double *Params, int NParams);
//	  void InitSimplex(double StepS, double *Params, int NParams);
	
	  void NextSimp (double *Params, double Residuum);
	  void GetBestParams(double * Params);
	  void CopyParams(double *  from, double * to);

	private:

	double Vertices[SimpSize][SimpSize+1];
	int SimpFlag;
	int SimpNParams;
	int Best;
	int Worst;
	double BestResid;

	void Contract (double *Params, int Best, int Worst);
	void Expand (double *Params, int Best, int Worst);
	void Order();
	void Reflect (double *Params, int Best, int Worst);
	void Shrink (int Best);
};

//#endif



