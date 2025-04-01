// Text.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "Text.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Text

/*Text::Text()
{
}
*/

Text::~Text()
{
}


BEGIN_MESSAGE_MAP(Text, CEdit)
	//{{AFX_MSG_MAP(Text)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Text message handlers

void Text::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CEdit::PostNcDestroy();

	delete this;
}

void Text::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//Beep(1000,1000);
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	if (nChar == VK_ESCAPE){
		CWnd * pw = this->GetParent();
		pw->SetFocus();
//		Alert("VK escape");
	}
}


Text::Text(int id, CString  inits, BOOL modifiable, 
		   CRect CR, CWnd * wnd){	//  type = String
	CMainFrame * frame = (CMainFrame *) (AfxGetMainWnd());

	rect = CR;
	ID=id;
	c=inits;
//	c="Rosso";
	Type = TYPE_TEXT_STRING;
	VI=0;
	VF=0;
	Modifiable = modifiable;
	UNIT = CString("V");

//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER ,
//		CR, wnd, ID);

	DWORD style = ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER;
	if (!Modifiable){
		style = style | ES_READONLY  ;
	}
//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER ,
	Create(	style,	CR, wnd, ID);

	SetString(c);

}

Text::Text(int id, int i, int mini, int maxi, BOOL modifiable,
		   CRect CR, CWnd * wnd){ //  type = int
	CMainFrame * frame = (CMainFrame *) (AfxGetMainWnd());
	rect = CR;
	ID=id;
	Type = TYPE_TEXT_INT;
	VI=i;
	MINI=mini;
	MAXI=maxi;
	UNIT = CString("V");

	VF=0;
	Modifiable = modifiable;

	DWORD style = ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER;
	if (!Modifiable){
		style = style | ES_READONLY  ;
	}
//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER ,
	Create(	style,	CR, wnd, ID);
//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER ,
//		CR, wnd, ID);

	SetInt(VI);

}

Text::Text(int id, double f, double minf, double maxf, BOOL modifiable,
		   CRect CR, CWnd * wnd){ //  type = float
	CMainFrame * frame = (CMainFrame *) (AfxGetMainWnd());
	rect = CR;
	ID=id;
	Type = TYPE_TEXT_FLOAT;
	VI=0;
	VF=f;
	MINF=minf;
	MAXF=maxf;
	Modifiable = modifiable;
	UNIT = CString("V");

	DWORD style = ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER;
	if (!Modifiable){
		style = style | ES_READONLY  ;
	}
//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER ,
	Create(	style,	CR, wnd, ID);

	SetFloat(VF);
}

Text::Text(int id, double f, double minf, double maxf, CString unit, BOOL modifiable,
		   CRect CR, CWnd * wnd){ //  type = float with units
	CMainFrame * frame = (CMainFrame *) (AfxGetMainWnd());
	rect = CR;
	ID=id;
	Type = TYPE_TEXT_WITHUNITS;
	VI=0;
	VF=f;
	MINF=minf;
	MAXF=maxf;
	Modifiable = modifiable;
	UNIT = unit;

	DWORD style = ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER;
	if (!Modifiable){
		style = style | ES_READONLY  ;
	}
//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER ,
//	Create(	ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER , //| WS_OWNERDRAW ,
//		CR, wnd, ID);
	Create(	style,	CR, wnd, ID);
	SetFloat(VF);
}





/////////////////////////////////////////////////////////////////////////////
// Text message handlers
BOOL Text::MakeInt(int & i){
	if (!Modifiable) return false;
	char * s;
	i = strtol( c, &s, 10);
	if (i<MINI) return false;
	if (i>MAXI) return false;
	return true;
}

void Text::IntToString(){
	char s[100];
	_itoa( VI, s, 10 );
	c=CString(s);
	SetWindowText(c);
}

BOOL Text::MakeFloat(double & f){
	if (!Modifiable) return false;
//	char * s;
//	MyFloat  x;
//	x = strtod( c, &s);
	if (Type == TYPE_TEXT_FLOAT){

		f=atof(c.GetBuffer(c.GetLength()));

	}
	else{
		if (!StringWithUnitToDouble(UNIT, c, f)){
//			Alert("error convert to double");
			return false;
		}
//		ShowFloat(f, "f");
	}

	double g=f;
	if (g<MINF) return false;
	if (g>MAXF) return false;

	return true;
}

BOOL Text::MakeString(CString & x){
	if (!Modifiable) return false;
	x=c;
	return true;
}

void Text::FloatToString(){

	if (Type == TYPE_TEXT_FLOAT){
		char s[20];
		_gcvt(VF, 6, s);
	
		c=CString(s);
	}
	else{
		//Assume with units
		DoubleToStringWithUnit(VF, UNIT, c);
	}

	SetWindowText(c);
	Invalidate();


//	Alert(c);  ///XXXXXXXXXXXXXXXXXX
}


void Text::SetInt(int i){
	VI=i;
	IntToString();
}
void Text::SetFloat(double g){
	VF=g;
	FloatToString();
}
void Text::SetString(CString & x){

	c=x;

	SetWindowText(c);
}


int Text::GetInt(){
	UpdateField();
	return VI;
}
double Text::GetFloat(){
	UpdateField();
	return VF;
}

void Text::GetString(CString & x){
	UpdateField();
	x=c;
}





void Text::GetCR(CRect & cr){
	cr = rect;
}

BOOL Text::IsModifiable(){
	return Modifiable;
}


void Text::UpdateField(){
	if (Modifiable) GetWindowText(c);
	if (Type==TYPE_TEXT_INT){
		int i;
		if (MakeInt(i)){
			VI=i;
		}
		IntToString();
	}
	if ((Type==TYPE_TEXT_FLOAT) | (Type==TYPE_TEXT_WITHUNITS)){
		double f;
		if (MakeFloat(f)){
			VF=f;
		}
		FloatToString();
//		ShowFloat(VF, "VF");
	}
	if (Type==TYPE_TEXT_STRING){
		if (!MakeString(c)){
			c=SaveString;
		}
	}
	SetWindowText(c);

}

void Text::DoEnable(BOOL Enable){
	EnableWindow(Enable);
	Modifiable= Enable;
}

void Text::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	ShowFloat(nChar, "c");
	if (nChar == VK_RETURN)    {
		Beep(1000,1000);
		Alert0("hi");
    }
}

BOOL Text::PreTranslateMessage( MSG* pMsg ){
// TODO: Add your specialized code here and/or call the base class
//	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ){
//		Alert0("key");
//	}
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )	{
//		int idCtrl= this->GetFocus()->GetDlgCtrlID();
//		if ( idCtrl == IDC_EDIT1 )
		this->GetOwner()->SetFocus();
//		UpdateWindow();
//		Invalidate();
//		SendMessage(EN_KILLFOCUS, 0, 0);
//		parent->Invalidate();
//		    Alert0("Got it!" );
		return true;
	}
	else{

		return CEdit::PreTranslateMessage( pMsg );
	}
}