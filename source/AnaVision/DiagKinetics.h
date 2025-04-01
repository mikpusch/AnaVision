#if !defined(AFX_DIAGKINETICS_H__7AB53D39_3869_42BF_81C5_92ABF2D1B4A3__INCLUDED_)
#define AFX_DIAGKINETICS_H__7AB53D39_3869_42BF_81C5_92ABF2D1B4A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagKinetics.h : header file
//
//#include "Kinetics.h"

/////////////////////////////////////////////////////////////////////////////
// DiagKinetics dialog

class DiagKinetics : public CDialog
{
// Construction
public:
	DiagKinetics(CWnd* pParent = NULL);   // standard constructor

	Kinetics k;
	double GaussWidth;
// Dialog Data
	//{{AFX_DATA(DiagKinetics)
	enum { IDD = IDD_DIAG_KINETICS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiagKinetics)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DiagKinetics)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGKINETICS_H__7AB53D39_3869_42BF_81C5_92ABF2D1B4A3__INCLUDED_)
