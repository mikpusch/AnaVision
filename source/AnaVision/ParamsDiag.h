#if !defined(AFX_PARAMSDIAG_H__2B04434A_54B2_4E5E_9D96_01CC06715C6A__INCLUDED_)
#define AFX_PARAMSDIAG_H__2B04434A_54B2_4E5E_9D96_01CC06715C6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamsDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ParamsDiag dialog

class ParamsDiag : public CDialog
{
// Construction
public:
	ParamsDiag(CWnd* pParent = NULL);   // standard constructor


	Params p;
// Dialog Data
	//{{AFX_DATA(ParamsDiag)
	enum { IDD = IDD_DIAGSET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ParamsDiag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ParamsDiag)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLoadLookuptable();
	afx_msg void OnMakeGrey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMakeRed();
	afx_msg void OnBnClickedMakeGreen();
	afx_msg void OnBnClickedMakeBlue();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSDIAG_H__2B04434A_54B2_4E5E_9D96_01CC06715C6A__INCLUDED_)
