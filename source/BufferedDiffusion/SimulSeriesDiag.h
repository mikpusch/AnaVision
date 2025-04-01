#if !defined(AFX_SIMULSERIESDIAG_H__09B2A6A8_AEE9_4960_B6D7_499106A40635__INCLUDED_)
#define AFX_SIMULSERIESDIAG_H__09B2A6A8_AEE9_4960_B6D7_499106A40635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimulSeriesDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SimulSeriesDiag dialog
class MyEditCtrl;

class SimulSeriesDiag : public CDialog
{
// Construction
public:
	SimulSeriesDiag(CWnd* pParent = NULL);   // standard constructor

	MyEditCtrl * pC;
	SeriesSimulation ss;


// Dialog Data
	//{{AFX_DATA(SimulSeriesDiag)
	enum { IDD = IDD_DIAG_SIMUL_SERIES };
	int		m_NCurves;
	double	m_Time;
	double	m_IntermediateTime;
	BOOL	m_UseTwoTimePoints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SimulSeriesDiag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SimulSeriesDiag)
	virtual BOOL OnInitDialog();
	afx_msg void OnMyok();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULSERIESDIAG_H__09B2A6A8_AEE9_4960_B6D7_499106A40635__INCLUDED_)
