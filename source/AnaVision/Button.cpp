// Button.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "Button.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Button

/*
Button::Button()
{
}
*/

Button::~Button()
{
}


Button::Button(int id,  BOOL OWNERDRAW, CString Text, CRect CR, CWnd * wnd){
	Active = false;
	ID = id;

//	pb = NULL;

	OwnerDraw = OWNERDRAW;

	if (OwnerDraw){
		Create(Text,
				WS_VISIBLE | WS_CHILD | WS_BORDER |BS_OWNERDRAW   ,
				CR, wnd, ID);//item);
	}
	else{
		Create(Text,
				WS_VISIBLE | WS_CHILD | WS_BORDER ,  
				CR, wnd, ID);//item);
	}
	ButtonType = PUSHBUTTONTYPE;
}

Button::Button(int id, CString Text, CRect CR, BOOL CheckBox /* otherwise radio */,
			   BOOL FirstOfGroup, CWnd * wnd){

	Active = false;
	if (CheckBox){
		ButtonType = CHECKBOXTYPE;
	}
	else{
		ButtonType = RADIOBUTTONTYPE;
	}
	OwnerDraw = false;
	ID = id;
	IsFirstOfGroup = FirstOfGroup;

	if (ButtonType == CHECKBOXTYPE){
		Create(Text, 
				WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTOCHECKBOX,  
				CR, wnd, ID);//item);
	}
	else{
		if (FirstOfGroup){
			Create(Text, 
					WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTORADIOBUTTON | WS_GROUP,  
					CR, wnd, ID);//item);
		}
		else{
			Create(Text, 
					WS_VISIBLE | WS_CHILD | WS_BORDER | BS_AUTORADIOBUTTON,  
					CR, wnd, ID);//item);
		}
	}



}



BEGIN_MESSAGE_MAP(Button, CButton)
	//{{AFX_MSG_MAP(Button)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Button message handlers

void Button::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if (!OwnerDraw){
		CButton::DrawItem(lpDrawItemStruct);
		return;
	}

	CMainFrame * f =  (CMainFrame *) (AfxGetMainWnd());
	CBrush * pB;
	if (Active){
		pB = f->RedBrush;
	}
	else{
		pB = f->BlueBrush;
	}

	CString c;
	GetWindowText(c);
//	CDC * pDC = GetDC();
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect r;
	GetClientRect(&r);
	CBrush * OldBrush = pDC->SelectObject(pB);
	pDC->FillRect(r, pB);
			//	Alert("red");		
	pDC->SelectObject(OldBrush);

	if (f->FirstFont){
		f->SeqFont.CreatePointFont( 80, "times", pDC );
		f->FirstFont = false;
	}
	CFont * OldFont = pDC->SelectObject( &f->SeqFont );

	pDC->TextOut(1,5,c);
	pDC->SelectObject( OldFont );

	
}

void Button::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PostNcDestroy();

	delete this;
}
