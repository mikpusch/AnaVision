#if !defined(AFX_DIAGLINEARFIT_H__6ED26FB8_DB5E_4195_8FC7_0B442743403A__INCLUDED_)
#define AFX_DIAGLINEARFIT_H__6ED26FB8_DB5E_4195_8FC7_0B442743403A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagLinearFit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DiagLinearFit dialog

class DiagLinearFit : public CDialog
{
// Construction
public:
	DiagLinearFit(CWnd* pParent = NULL);   // standard constructor

	Kinetics k;
// Dialog Data
	//{{AFX_DATA(DiagLinearFit)
	enum { IDD = IDD_DIAG_LINFIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiagLinearFit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	double GaussWidth;
	// Generated message map functions
	//{{AFX_MSG(DiagLinearFit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGLINEARFIT_H__6ED26FB8_DB5E_4195_8FC7_0B442743403A__INCLUDED_)
