#include "stdafx.h"
#include "../common/PraxisThread.h"
#include "../common/utils.h"

IMPLEMENT_DYNCREATE(PraxisThread, CWinThread)

PraxisThread::PraxisThread( ){
	Initialized = false;
}
PraxisThread::~PraxisThread(){
}

void PraxisThread::DoSendMessage(UINT msg){
	if (Frame){
		SendMessage(Frame, msg, msg, msg);
	}
}

void PraxisThread::StaticSendMessage(int * stuff){
	if (!stuff){
		return;
	}

	PraxisThread * pP = (PraxisThread *)(stuff);

	UINT Message = pP->MessageBetter;
	if ( (*pP->STATUSINDEX)==1){
		Message = pP->MessageStop;
	}

	pP->DoSendMessage(Message);

}

PraxisThread::PraxisThread(int n,    // number of vars
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
					int MaxIter, // Set to neg for indefinite
					double * CurrentBestParams // can be used if conjunctance with the messaging to store the currently best params
					){
	Initialized = false;
	STUFF = stuff;
	XARRAY = xarray;
	BESTVALUE = BestValue;
	STATUSINDEX = statusindex;
	Frame = hwnd;
	TOL=Tolerance;
	COSTFUNC=f;
//	ShowInt(int(f), "F");
	DIMENS=n;
	MAXITER=MaxIter;
	BestParams= CurrentBestParams;
	MAXSTEPSIZE = MaxStepSize;
	MessageStop = MsgStop;
	MessageBetter = MsgBetter;
	Initialized = true;

}

BOOL PraxisThread::InitInstance(){
	if (!Initialized){
		return true;
	}

	MessageStuff = (int *)(this);

	Praxis p;

	praxis_message_function_type MsgFunc = (praxis_message_function_type) (this->StaticSendMessage);

	p.InitPraxis(DIMENS, COSTFUNC,
		TOL, STUFF, MAXITER, STATUSINDEX, BestParams, MsgFunc, MessageStuff);
//	Alert0("Init ok");

	p.Minimize(TOL, MAXSTEPSIZE, XARRAY, BESTVALUE);

	DoSendMessage(MessageStop);

	return true;
}
int PraxisThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(PraxisThread, CWinThread)
END_MESSAGE_MAP()
