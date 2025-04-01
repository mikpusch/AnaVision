#if !defined(AFX_MYEDITCTRL_H__F22113B0_314D_4F52_AC04_A608263F94F1__INCLUDED_)
#define AFX_MYEDITCTRL_H__F22113B0_314D_4F52_AC04_A608263F94F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyEditCtrl window

class MyEditCtrl : public CRichEditCtrl
{
// Construction
public:
	MyEditCtrl(CWnd * parent);

// Attributes


// Operations
public:

	void DoShowWindow();
	void DoHideWindow();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyEditCtrl)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyEditCtrl();

	// Generated message map functions
protected:
public:
	//{{AFX_MSG(MyEditCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnMenuSelectAll();
	afx_msg void OnMenuCopy();
	afx_msg void OnMenuCut();
	afx_msg void OnMenuPaste();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDITCTRL_H__F22113B0_314D_4F52_AC04_A608263F94F1__INCLUDED_)
