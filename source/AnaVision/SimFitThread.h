#if !defined(AFX_SIMFITTHREAD_H__7A36F393_5D9F_4EAB_871A_AF74BD57A04F__INCLUDED_)
#define AFX_SIMFITTHREAD_H__7A36F393_5D9F_4EAB_871A_AF74BD57A04F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimFitThread.h : header file
//


class Kinetics;

/////////////////////////////////////////////////////////////////////////////
// SimFitThread thread

class SimFitThread : public CWinThread
{
	DECLARE_DYNCREATE(SimFitThread)
public:
	SimFitThread();           // protected constructor used by dynamic creation
	SimFitThread(__int32 * status, HWND FrameHANDLE, Kinetics * k, double * psd);           // protected constructor used by dynamic creation

// Attributes
public:

	__int32 * StatusPointer;
	HWND FrameHandle;
	Kinetics * K;
	double * pSD;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SimFitThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SimFitThread();

	void DoSendMessage(UINT msg);

	// Generated message map functions
	//{{AFX_MSG(SimFitThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMFITTHREAD_H__7A36F393_5D9F_4EAB_871A_AF74BD57A04F__INCLUDED_)
