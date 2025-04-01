#if !defined(AFX_PARAMSDIAG_H__1A215D35_0167_40C2_A7C4_CF96DCB05C0A__INCLUDED_)
#define AFX_PARAMSDIAG_H__1A215D35_0167_40C2_A7C4_CF96DCB05C0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamsDiag.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ParamsDiag dialog

#include "BufferedDiffusionDoc.h"

class Diffusion;
class Params;


class ParamsDiag : public CDialog
{
// Construction
public:
	ParamsDiag(CWnd* pParent = NULL);   // standard constructor

	Diffusion	d;
	Params		p;

	double InternalCapac;

// Dialog Data
	//{{AFX_DATA(ParamsDiag)
	enum { IDD = IDD_DIAGPARAMS };
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
	afx_msg void OnChangefilename();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSDIAG_H__1A215D35_0167_40C2_A7C4_CF96DCB05C0A__INCLUDED_)
