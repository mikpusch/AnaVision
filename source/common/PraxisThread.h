#pragma once

#include "../common/Praxis.h"

// PraxisThread

class PraxisThread : public CWinThread
{
	DECLARE_DYNCREATE(PraxisThread)

protected:
public:
	PraxisThread( );

	PraxisThread(int n,    // number of vars
					praxis_costfunction_type f, // Functor for function under investigation
					double * xarray,
					double & BestValue, 
					double Tolerance, 
					double MaxStepSize,
                    int * stuff,   // The class instance used to calc the cost function, in general
					int * statusindex,
					UINT MsgStop,
					UINT MsgBetter,
					HWND hwnd,
					int MaxIter = -1, // Set to neg for indefinite
					double * CurrentBestParams = NULL // can be used if conjunctance with the messaging to store the currently best params
					);  
	virtual ~PraxisThread();

public:
	bool Initialized;
	int * STUFF;
	int * STATUSINDEX;
	double * XARRAY;
	double  BESTVALUE;
	HWND Frame;
	double TOL;
	double MAXSTEPSIZE;
	praxis_costfunction_type COSTFUNC;
	int DIMENS;
	int MAXITER;
	double * BestParams;

	int * MessageStuff;

	UINT MessageStop;
	UINT MessageBetter;



public:
	static void StaticSendMessage(int * stuff);

	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void DoSendMessage(UINT msg);

protected:
	DECLARE_MESSAGE_MAP()
};


