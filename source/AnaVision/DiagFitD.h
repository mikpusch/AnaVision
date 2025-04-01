#if !defined(AFX_DIAGFITD_H__21AA995C_C492_4942_8CB8_0C7C8CCB7D97__INCLUDED_)
#define AFX_DIAGFITD_H__21AA995C_C492_4942_8CB8_0C7C8CCB7D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagFitD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DiagFitD dialog

class DiagFitD : public CDialog
{
// Construction
public:
	DiagFitD(CWnd* pParent = NULL);   // standard constructor

	Kinetics k;

// Dialog Data
	//{{AFX_DATA(DiagFitD)
	enum { IDD = IDD_DIAGFITD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiagFitD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DiagFitD)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGFITD_H__21AA995C_C492_4942_8CB8_0C7C8CCB7D97__INCLUDED_)
