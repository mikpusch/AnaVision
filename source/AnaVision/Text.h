#if !defined(AFX_TEXT_H__7E02BAA5_4811_4483_A8FC_06C91E5B8D54__INCLUDED_)
#define AFX_TEXT_H__7E02BAA5_4811_4483_A8FC_06C91E5B8D54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Text.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Text window

#define TYPE_TEXT_INT		1
#define TYPE_TEXT_FLOAT		2
#define TYPE_TEXT_STRING	3
#define TYPE_TEXT_WITHUNITS	4



/////////////////////////////////////////////////////////////////////////////
// Text window

class Text : public CEdit
{
// Construction
public:
	Text(int id, CString & inits, BOOL modifiable,
		   CRect CR, CWnd * wnd);	//  type = String

	Text(int id, CString  inits, BOOL modifiable,
		   CRect CR, CWnd * wnd);	//  type = String

	Text(int id, int i, int mini, int maxi, BOOL modifiable,
		CRect CR, CWnd * wnd); //  type = int

	Text(int id, double f, double minf, double maxf, BOOL modifiable,
		CRect CR, CWnd * wnd); //  type = float

	Text(int id, double f, double minf, double maxf, CString Unit, BOOL modifiable,
		CRect CR, CWnd * wnd); //  type = float with units

private:
	CRect rect;
	CString c;
	CString SaveString;
	int VI;
	int MINI, MAXI;
	double VF;
	double MINF, MAXF;
	BOOL Modifiable;
	CString UNIT;

	int ID;
	int Type; // 

// Operations
public:
	void DoEnable(BOOL Enable);
	void SetUnit(CString & unit);
	void SetInt(int i);
	void SetFloat(double g);
	void SetString(CString & c);

	int GetInt();
	double GetFloat();
	void GetString(CString & x);

	void UpdateField();

	void IntToString();
	void FloatToString();

	void GetCR(CRect & cr);

	BOOL IsModifiable();
	BOOL DoMakeFloat(double & f);

private:
	BOOL MakeInt(int & i);
	BOOL MakeFloat(double & f);
	BOOL MakeString(CString & x);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Text)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Text();

	// Generated message map functions
protected:
	//{{AFX_MSG(Text)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg	BOOL PreTranslateMessage( MSG* pMsg );

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT_H__7E02BAA5_4811_4483_A8FC_06C91E5B8D54__INCLUDED_)
