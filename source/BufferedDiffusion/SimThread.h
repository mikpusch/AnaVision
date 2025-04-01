#if !defined(AFX_SIMTHREAD_H__6F44E840_E9CE_4EDA_A519_076F91C6CF64__INCLUDED_)
#define AFX_SIMTHREAD_H__6F44E840_E9CE_4EDA_A519_076F91C6CF64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// SimThread thread

class SimThread : public CWinThread
{
	DECLARE_DYNCREATE(SimThread)
public:
	SimThread();           // protected constructor used by dynamic creation

// Attributes
public:

	SimThread(int * StatusSimThread, HWND hWnd, Diffusion * pd, Params * pset, 	SeriesSimulation * pS,
			double Time, int FIRSTINDEX, int NCurves, SimulationMemory * pSimMem);

	SimThread(int * StatusSimThread, HWND hWnd, Diffusion * pd, Params * pset, 	SeriesSimulation * pS,
			double Time, int FIRSTINDEX, double IntermediateTime, SimulationMemory * pSimMem);

	Diffusion * pD;
	int * StatusInt;
	SeriesSimulation * pSS;
	Params * pSet;
	int FirstIndexToUse;
	HWND Frame;
	double SimTime;
	double IntTime;
	bool UseIntTime;
	int NumberCurves;
	SimulationMemory * pSIMMEM;

	bool paused;

	void DoSendMessage(UINT msg);
	void OnGoGo() ;
	void MakeTempFileNames();
	CString TempTableFileName;
	CString TempSimFileName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SimThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SimThread();

	// Generated message map functions
	//{{AFX_MSG(SimThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMTHREAD_H__6F44E840_E9CE_4EDA_A519_076F91C6CF64__INCLUDED_)
