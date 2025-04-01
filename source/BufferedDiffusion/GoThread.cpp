// GoThread.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
//#include "GoThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GoThread

IMPLEMENT_DYNCREATE(GoThread, CWinThread)

GoThread::GoThread()
{
}

GoThread::GoThread(int * StatusSimThread, int * Index, HWND hWnd, Diffusion * pd, Params * pset, 	SeriesSimulation * pS,
					 SimulationMemory * pSimMem){
	pD = pd;
	StatusInt = StatusSimThread;
	pSS = pS;
	pSet = pset;
	Frame = hWnd;
	index = Index;
//	SimTime = Time;
//	NumberCurves = NCurves;
	pSIMMEM = pSimMem;
//	UseIntTime = false;

}

GoThread::~GoThread()
{
}
inline void GoThread::DoSendMessage(UINT msg){
	if (Frame){
		SendMessage(Frame, msg, msg, msg);
	}
}
BOOL GoThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	paused = false;
	OnGoGo();

//	DoSendMessage(MESSAGE_GO_GO);

	DoSendMessage(MESSAGE_GO_FINISHED);
	
	AfxEndThread(0);

	return TRUE;
}


int GoThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(GoThread, CWinThread)
	//{{AFX_MSG_MAP(GoThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GoThread message handlers

void GoThread::OnGoGo() 
{
	// TODO: Add your command handler code here

	Params & pset = *pSet;
	Diffusion & d = *pD;
	SimulationMemory & SimMem = *pSIMMEM;

//	d.Setup();
//	SimMem.AddSimulation(d.M+1, 4, d.pHbulk, d.Bbulk, d.DR0, d.DT0, d.r);
	
	int iter;
	int idraw = 0;

	for (int i=0; i<pset.MaxIter; i++){
		if (*StatusInt == 2){ // stopped
			return;
		}

		iter = i;
//		t=d.time;

		idraw++;
		bool actuallyused = false;
		if ( (idraw==(pset.UpdateEvery)) || (idraw==(2*pset.UpdateEvery-1))){
			actuallyused = true;
		}
		if ( (iter==0) || (idraw==(pset.UpdateEvery+1)) || (idraw==(2*pset.UpdateEvery))){
//			if (iter==0){
//				pDC->FillRect( &rect, pDC->GetCurrentBrush());
//			}

			*index = (i+1)/pset.UpdateEvery;
//			if (iter>1){
			if (true){
				DoSendMessage(MESSAGE_DRAWCURVES);
//				DrawCurves(pDC, d.M, index, TRUE, FALSE);
			}
			if (idraw==(2*pset.UpdateEvery)){
				idraw=1;
			}
		}
		if (paused){
//			Beep(1000,10);
			Sleep(100);
		}


		if (*StatusInt == 3){ // paused
//				Alert0("hi");
			paused = true;
		}
		else{
			paused = false;
		}

		d.Step(actuallyused);
		actuallyused = false;
	}

}
