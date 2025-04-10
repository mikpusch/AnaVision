// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__F4E34A92_47D4_406F_8D25_A9F55DC0B61D__INCLUDED_)
#define AFX_CHILDFRM_H__F4E34A92_47D4_406F_8D25_A9F55DC0B61D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_TIMER_SAVE_ENERGY			500


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

	int StatusSimThread;
	BOOL IsSimulating;

// Operations
public:

	LRESULT MessageFinished();
	LRESULT MessageGoGo();
	LRESULT MessageDrawCurves();
	LRESULT MessageGoFinished();
	LRESULT MessageSaveEnergyTimer();

	int index;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__F4E34A92_47D4_406F_8D25_A9F55DC0B61D__INCLUDED_)
