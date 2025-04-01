// SimFitThread.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "SimFitThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimFitThread

IMPLEMENT_DYNCREATE(SimFitThread, CWinThread)

SimFitThread::SimFitThread(){
	StatusPointer = NULL;
	FrameHandle = NULL;
	K=NULL;
}

SimFitThread::SimFitThread(__int32 * status, HWND FrameHANDLE, Kinetics * k, double * psd){
	StatusPointer = status;
	FrameHandle = FrameHANDLE;
	K=k;
	pSD = psd;
}

SimFitThread::~SimFitThread()
{


}

inline void SimFitThread::DoSendMessage(UINT msg){
	if (FrameHandle){
		SendMessage(FrameHandle, msg, msg, msg);
	}
}


BOOL SimFitThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	while (true){
		BOOL result = false;
		if (K){
//			Beep(1000,100);
//			ShowFloat(K->d.hnew[0], "h0");
			result = K->StepSimulationFit(*pSD);
//			ShowFloat(K->d.hnew[0], "h0");
		}
		if (result){
			if (*StatusPointer == 2){ // Stopped
				K->GetBestParamsSimulationFit(*pSD);
				*StatusPointer = 3;
				DoSendMessage(UPDATEVIEW_MESSAGE);
				AfxEndThread(0);
				return true;
			}
		}
//		Beep(2000,200);
		DoSendMessage(UPDATEVIEW_MESSAGE);
		if (!result){
			*StatusPointer = 3;
			DoSendMessage(UPDATEVIEW_MESSAGE);
			AfxEndThread(0);
			return true;
		}
	}
}

int SimFitThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(SimFitThread, CWinThread)
	//{{AFX_MSG_MAP(SimFitThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimFitThread message handlers