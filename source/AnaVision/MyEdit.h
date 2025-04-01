#if !defined(AFX_MYEDIT_H__924918EF_53B2_4DDE_A1FF_414669903201__INCLUDED_)
#define AFX_MYEDIT_H__924918EF_53B2_4DDE_A1FF_414669903201__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyEdit window

class MyEdit : public CEdit
{
// Construction
public:
	MyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(MyEdit)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__924918EF_53B2_4DDE_A1FF_414669903201__INCLUDED_)
