#if !defined(AFX_GOTHREAD_H__EEE98928_91E1_49F3_A7A0_6F709FC08864__INCLUDED_)
#define AFX_GOTHREAD_H__EEE98928_91E1_49F3_A7A0_6F709FC08864__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GoThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// GoThread thread

class GoThread : public CWinThread
{
	DECLARE_DYNCREATE(GoThread)
protected:
public:
	GoThread();           // protected constructor used by dynamic creation
	GoThread(int * StatusGoThread, int * Index, HWND hWnd, Diffusion * pd, Params * pset, SeriesSimulation * pS,
			SimulationMemory * pSimMem);

// Attributes
public:

	Diffusion * pD;
	int * StatusInt;

	int * index; 
	SeriesSimulation * pSS;
	Params * pSet;
	HWND Frame;
//	double SimTime;
//	double IntTime;
//	bool UseIntTime;
//	int NumberCurves;

	SimulationMemory * pSIMMEM;

	bool paused;

	void DoSendMessage(UINT msg);
	void OnGoGo() ;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GoThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~GoThread();

	// Generated message map functions
	//{{AFX_MSG(GoThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTHREAD_H__EEE98928_91E1_49F3_A7A0_6F709FC08864__INCLUDED_)
