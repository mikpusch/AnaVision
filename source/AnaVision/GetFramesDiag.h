#if !defined(AFX_GETFRAMESDIAG_H__D581385F_69CF_42F0_8EF0_46F1A320E18F__INCLUDED_)
#define AFX_GETFRAMESDIAG_H__D581385F_69CF_42F0_8EF0_46F1A320E18F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetFramesDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GetFramesDiag dialog

class GetFramesDiag : public CDialog
{
// Construction
public:
	GetFramesDiag(CWnd* pParent = NULL);   // standard constructor

	Params p;


// Dialog Data
	//{{AFX_DATA(GetFramesDiag)
	enum { IDD = IDD_DIAG_GET_FRAMES_TO_ANALYZE };
	//}}AFX_DATA

	void DisableInitialFrames();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetFramesDiag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	bool InitialFramesDisabled;

	// Generated message map functions
	//{{AFX_MSG(GetFramesDiag)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETFRAMESDIAG_H__D581385F_69CF_42F0_8EF0_46F1A320E18F__INCLUDED_)
