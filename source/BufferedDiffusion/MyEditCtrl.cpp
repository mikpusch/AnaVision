// MyEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
//#include "MyEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyEditCtrl

MyEditCtrl::MyEditCtrl(	CWnd * parent)
{
	CRect rect;
	CRect clientrect;
	parent->GetClientRect(&clientrect);
	rect.left = 20;
	rect.top = 70;
	rect.right = rect.left+400;
	rect.bottom = rect.top + 500;
		DWORD style = 
			  ES_AUTOHSCROLL 
			| ES_AUTOVSCROLL  
			| ES_LEFT 
			| ES_MULTILINE 
			| WS_VISIBLE 
			| WS_CHILD
//			| WS_DLGFRAME;
//			| WS_CAPTION
			| WS_HSCROLL
			| WS_VSCROLL 
			| WS_THICKFRAME
//			| WS_SYSMENU 
//			| WS_OVERLAPPEDWINDOW 
			| WS_BORDER
//			| WS_MAXIMIZE
;
	this->Create(style, rect, parent, 9999);
}



MyEditCtrl::~MyEditCtrl()
{
}


BEGIN_MESSAGE_MAP(MyEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(MyEditCtrl)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDITCTRLL_SELECTALL, OnMenuSelectAll)
	ON_COMMAND(ID_EDITCTRLL_COPY, OnMenuCopy)
	ON_COMMAND(ID_EDITCTRLL_CUT, OnMenuCut)
	ON_COMMAND(ID_EDITCTRLL_PASTE, OnMenuPaste)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyEditCtrl message handlers
void MyEditCtrl::DoShowWindow(){
	ShowWindow(SW_SHOW);
}


void MyEditCtrl::DoHideWindow(){
	ShowWindow(SW_HIDE);

}

void MyEditCtrl::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CRichEditCtrl::PostNcDestroy();
}

void MyEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_POP2));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CRect rc;
	GetWindowRect(rc);

	int x=point.x+rc.left;
	int y=point.y+rc.top;

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, this);
	
//	CRichEditCtrl::OnRButtonDown(nFlags, point);
}


void MyEditCtrl::OnMenuSelectAll(){
	// TODO: Add your command handler code here
//	Beep(1000,100);
		SetSel(0, -1);
	
}

void MyEditCtrl::OnMenuPaste(){
	Paste();
}
void MyEditCtrl::OnMenuCopy(){
	Copy();
}
void MyEditCtrl::OnMenuCut(){
	Cut();
}


