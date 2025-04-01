#if !defined(AFX_RATIODIAG_H__D2949B91_C23A_458B_9381_8597B659E1F8__INCLUDED_)
#define AFX_RATIODIAG_H__D2949B91_C23A_458B_9381_8597B659E1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatioDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RatioDiag dialog

class RatioDiag : public CDialog
{
// Construction
public:
	RatioDiag(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RatioDiag)
	enum { IDD = IDD_DIAG_RATIO };
	CString	m_ComboResult;
	CString	m_ListA;
	CString	m_ListB;
	double	m_BackgroundA;
	double	m_BackgroundB;
	int		m_NSurroundForAverage;
	//}}AFX_DATA

public:
	int NStringsAB;
	bool InvalidSelection;
	int indexa, indexb, indexc;
	CString NewResultString;

	CString * StringsAB;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RatioDiag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RatioDiag)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RATIODIAG_H__D2949B91_C23A_458B_9381_8597B659E1F8__INCLUDED_)
