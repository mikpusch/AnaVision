// SimThread.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
//#include "SimThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimThread

IMPLEMENT_DYNCREATE(SimThread, CWinThread)

SimThread::SimThread()
{
	paused = false;
}


SimThread::SimThread(int * StatusSimThread, HWND hWnd, Diffusion * pd, Params * pset, 	SeriesSimulation * pS,
					 double Time, int FIRSTINDEX, int NCurves, SimulationMemory * pSimMem){
	pD = pd;
	StatusInt = StatusSimThread;
	pSS = pS;
	pSet = pset;
	Frame = hWnd;
	SimTime = Time;
	NumberCurves = NCurves;
	pSIMMEM = pSimMem;
	UseIntTime = false;
	FirstIndexToUse = FIRSTINDEX;
	if (FirstIndexToUse<0) 	FirstIndexToUse = 0;
	MakeTempFileNames();
}


SimThread::SimThread(int * StatusSimThread, HWND hWnd, Diffusion * pd, Params * pset, 	SeriesSimulation * pS,
					 double Time, int FIRSTINDEX, double IntermediateTime, SimulationMemory * pSimMem){
	pD = pd;
	StatusInt = StatusSimThread;
	pSS = pS;
	pSet = pset;
	Frame = hWnd;
	SimTime = Time;
	pSIMMEM = pSimMem;
	UseIntTime = true;
	IntTime = IntermediateTime;
	NumberCurves = 4;
	FirstIndexToUse = FIRSTINDEX;
	if (FirstIndexToUse<0) 	FirstIndexToUse = 0;
	MakeTempFileNames();
}

void SimThread::MakeTempFileNames(){
	TempTableFileName = "";
	TempSimFileName = "";
	CString tempdir;
	
	char s[1000];
	DWORD temp = GetEnvironmentVariable("TEMP", s, 1000);
	TempTableFileName = CString(s) + "/" + "TABLE_DR_DT_BUFF.TAB";
	TempSimFileName = CString(s) + "/" + "SIM_BUFF.SIM";
	
}

SimThread::~SimThread()
{
}
inline void SimThread::DoSendMessage(UINT msg){
	if (Frame){
		SendMessage(Frame, msg, msg, msg);
	}
}

BOOL SimThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	paused = false;
	for (int i=FirstIndexToUse; i<pSS->NValues; i++){
		if (*StatusInt == 2){ // stopped
			break;
		}
		pD->DR0 = pSS->dr[i];
		pD->DT0 = pSS->dt[i];
		pD->FactDR = 1;
		pD->FactDT = 1;

		pSet->MaxIter = int(SimTime/pD->DT0)+1;
		pSet->UpdateEvery = pSet->MaxIter/NumberCurves;

		OnGoGo();

		CFile ftable;
		if (ftable.Open(TempTableFileName, CFile::modeCreate | CFile::modeWrite)){
			pSS->WriteOnFile(ftable);
		}
		CFile fsim;
		if (fsim.Open(TempSimFileName, CFile::modeCreate | CFile::modeWrite)){
			pSIMMEM->SaveOnFile(fsim);
		}

		if (*StatusInt == 2){ // stopped
			break;
		}
		DoSendMessage(MESSAGE_GO_GO);
	}
	DoSendMessage(MESSAGE_FINISHED);
	AfxEndThread(0);

	return TRUE;
}

int SimThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(SimThread, CWinThread)
	//{{AFX_MSG_MAP(SimThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimThread message handlers


void SimThread::OnGoGo() 
{
	// TODO: Add your command handler code here

	Params & pset = *pSet;
	Diffusion & d = *pD;
	SimulationMemory & SimMem = *pSIMMEM;

	d.Setup();
	if (UseIntTime){
		SimMem.AddSimulation(d.M+1, 4, d.pHbulk, d.Bbulk, d.DR0, d.DT0, d.r);
	}
	else{
		SimMem.AddSimulation(d.M+1, pset.MaxIter/pset.UpdateEvery + 1, d.pHbulk, d.Bbulk, d.DR0, d.DT0, d.r);
	}

	int idraw=0;
//	int iter;
	bool IntermediateToDo = true;
	for (int i=0; i<pset.MaxIter; i++){
		if (*StatusInt == 2){ // stopped
			return;
		}
//		iter = i;
		if (UseIntTime){
			if (i==0){
				SimMem.AddCurve(d.H, d.B);
			}
			else{
				if (i==(pset.MaxIter-1)){
	//				Alert0("Last");
					SimMem.AddCurve(d.H, d.B);
				}
				else{
					if (IntermediateToDo){
						if ( (i*d.DT0)>=this->IntTime){
							SimMem.AddCurve(d.H, d.B);
							IntermediateToDo=false;
	//						Alert0("inter");
						}
					}
				}
			}
		}

		else{
//			Alert0("hi");
			idraw++;

			if ( (i==0) || (idraw==(pset.UpdateEvery+1)) || (idraw==(2*pset.UpdateEvery))){
				SimMem.AddCurve(d.H, d.B);
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

		d.Step(true);
	}
}
