#if !defined(AFX_IODIAG_H__8550A87C_4F94_4295_B3FC_EB0920E46768__INCLUDED_)
#define AFX_IODIAG_H__8550A87C_4F94_4295_B3FC_EB0920E46768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IODiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IODiag dialog

class IODiag : public CDialog
{
// Construction
public:
	IODiag(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(IODiag)
	enum { IDD = IDD_DIAGIO };
	CString	m_number;
	CString	m_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IODiag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IODiag)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class io
{
public:
	CString ls;
	CString lf;
	int ReadString();
	int InInt(int & i, CString text);
	int InDouble(double & i, CString text);
	int InString(CString & i, CString text);
	void ShowDouble(double  i, CString text);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IODIAG_H__8550A87C_4F94_4295_B3FC_EB0920E46768__INCLUDED_)
