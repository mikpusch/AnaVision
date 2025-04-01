#pragma once
#if !defined(AFX_DIAGIO_H__F4938522_18EE_49B2_9BDC_6C57BF9A45EF__INCLUDED_)
#define AFX_DIAGIO_H__F4938522_18EE_49B2_9BDC_6C57BF9A45EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagIO.h : header file
//
#pragma once
/////////////////////////////////////////////////////////////////////////////
// DiagIO dialog

class DiagIO : public CDialog
{
// Construction
public:
	DiagIO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DiagIO)
	enum { IDD = IDD_DIAGIO };
	CString	m_number;
	CString	m_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiagIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DiagIO)
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

#endif // !defined(AFX_DIAGIO_H__F4938522_18EE_49B2_9BDC_6C57BF9A45EF__INCLUDED_)


