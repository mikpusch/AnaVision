#if !defined(AFX_DIAGSIMULFIT_H__38FB8C46_BFDF_4EA6_AD61_6F959CCF9D76__INCLUDED_)
#define AFX_DIAGSIMULFIT_H__38FB8C46_BFDF_4EA6_AD61_6F959CCF9D76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagSimulFit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DiagSimulFit dialog

class DiagSimulFit : public CDialog
{
// Construction
public:
	DiagSimulFit(CWnd* pParent = NULL);   // standard constructor

	Kinetics k;

// Dialog Data
	//{{AFX_DATA(DiagSimulFit)
	enum { IDD = IDD_DIAG_SIMULATED_FIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiagSimulFit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DiagSimulFit)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGSIMULFIT_H__38FB8C46_BFDF_4EA6_AD61_6F959CCF9D76__INCLUDED_)
