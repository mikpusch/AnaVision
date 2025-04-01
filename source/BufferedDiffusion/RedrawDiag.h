#if !defined(AFX_REDRAWDIAG_H__88A268B3_7F58_430A_BE17_EA56E34B6BA8__INCLUDED_)
#define AFX_REDRAWDIAG_H__88A268B3_7F58_430A_BE17_EA56E34B6BA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RedrawDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RedrawDiag dialog
class MyEditCtrl;
class SimulationMemory;

class RedrawDiag : public CDialog
{
// Construction
public:
	RedrawDiag(CWnd* pParent = NULL);   // standard constructor

	int RedrawIndex;
	int Color;
	int NSimul;

	MyEditCtrl * pC;

	SimulationMemory * pMem;

// Dialog Data
	//{{AFX_DATA(RedrawDiag)
	enum { IDD = IDD_REDRAWDIAG };
	int		m_RefIndex;
	BOOL	m_ShowRef;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RedrawDiag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RedrawDiag)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDRAWDIAG_H__88A268B3_7F58_430A_BE17_EA56E34B6BA8__INCLUDED_)
