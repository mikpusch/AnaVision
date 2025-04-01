#if !defined(AFX_BUTTON_H__693460AA_BED2_4BB0_8326_963CA0AC5F4E__INCLUDED_)
#define AFX_BUTTON_H__693460AA_BED2_4BB0_8326_963CA0AC5F4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Button.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Button window

class Button : public CButton
{
// Construction
public:
//	Button();
// Construction
public:

	Button(int id, BOOL OWNERDRAW, CString Text, CRect CR, CWnd * wnd);
	
	Button(int id, CString Text, CRect CR,
				BOOL CheckBox /* otherwise radio */, BOOL FirstOfGroup, CWnd * wnd);




// Attributes
public:
	int ID;

	BOOL OwnerDraw;

	BOOL Active;


#define PUSHBUTTONTYPE	0
#define CHECKBOXTYPE	1
#define RADIOBUTTONTYPE	2

	int ButtonType;
	BOOL IsCheckBox;
	BOOL IsFirstOfGroup;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Button)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Button();

	// Generated message map functions
protected:
	//{{AFX_MSG(Button)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTON_H__693460AA_BED2_4BB0_8326_963CA0AC5F4E__INCLUDED_)
