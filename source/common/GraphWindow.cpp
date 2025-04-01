
#include "stdafx.h"
#include "math.h"
//#include "MainFrm.h"
//#include "../StateEditor/StateEditor.h"
//#include "GePulse.h"
#include "GraphWindow.h"

#include "../StateEditor/Text.h"
#include "../GePulse/stim.h"
#include "../GePulse/Pulsed.h"
#include "../MarkovEditor/MarkovEditor/iodiag.h"
#include "DrawObject.h"

/////////////////////////////////////////////////////////////////////////////
// GraphWnd window
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool MyPlotClass::PensCreated  = false;
CPen *** MyPlotClass::MyPens = NULL ;
CPen *** MyPlotClass::MyThickPens = NULL ;
COLORREF ** MyPlotClass::MyColors = NULL ;

GraphAxis::GraphAxis(){
	ShowTickValues = false;
	MinBox = NULL;
	MaxBox = NULL;
	NTicks = 3;
	type=0; // 0:x, 1:y
	Autoscale=true;
//	Autoscale=false;
	ManualMinValue=-1;
	ManualMaxValue=1;
	DataMinValue=-1;
	DataMaxValue=1;
	GrayPen= new CPen(PS_SOLID, 1, RGB(150,150,150));

	logscale=false; // else linear
	Title="";
	ShowTitle=false;
	Unit="";
	ShowUnit=false;

	Minxy=0; Maxxy=50;
	Minyx = 0; Maxyx = 50;

}
GraphAxis::~GraphAxis(){

	delete GrayPen;
	MinBox=NULL;
	MaxBox=NULL;

	if (!FirstTime && (this->type==1)){
//		VertFont.DeleteObject();
	}
}

void GraphAxis::Draw(CDC * pDC){
	if (FirstTime && (this->type==1)){
/*
VertFont.CreateFont(14, 0, 900, 900,
						0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
*/
/*		CFont* pf = pDC->GetCurrentFont( );
		LOGFONT LogFont;
		ZeroMemory (&LogFont, sizeof(LOGFONT));
		pf->GetLogFont(&LogFont );
		LogFont.lfEscapement = 900;
		LogFont.lfOrientation = 0;
		strcpy(LogFont.lfFaceName, "Arial");
//			LogFont.lfHeight = 20;
		//	CFont Vert;
		Vert.CreateFontIndirect(&LogFont );
		//CFont * OldFont = pDC->SelectObject(&Vert);
		//pDC->TextOut(Minyx -20, Minxy + (Maxxy-Minxy)/3, Titletoshow);
		//pDC->SelectObject(OldFont);
		//Vert.DeleteObject();
*/
		FirstTime = false;
	}
	if (this->Autoscale){
		DrawMinValue = DataMinValue;
		DrawMaxValue = DataMaxValue;
	}
	else{
		DrawMinValue = ManualMinValue;
		DrawMaxValue = ManualMaxValue;
	}
//	return;
	char s[20];
	CString MinString;
	_gcvt(this->DrawMinValue, 4,s);
	MinString  = s;
	CString MaxString;
	_gcvt(this->DrawMaxValue, 4, s);
	MaxString  = s;

	CString Titletoshow = Title;
	if (this->logscale){
		Titletoshow += " (log scale)";
	}
	int ticklength = 5; int NTicksDraw = NTicks+1 ; //4;
	if (type==0){
		pDC->MoveTo(Minxy, Minyx);
		pDC->LineTo(Maxxy, Minyx);
		pDC->MoveTo(Minxy, Maxyx);
		pDC->LineTo(Maxxy, Maxyx);

		for (int i=1; i<NTicksDraw; i++){
			int tickx = Minxy + int(double(i)/double(NTicksDraw)*double(Maxxy-Minxy));
			pDC->MoveTo(tickx, Minyx-ticklength);
			pDC->LineTo(tickx, Minyx+ticklength);
			if (ShowTickValues){
				float value = DrawMinValue+ double(i)*(DrawMaxValue-DrawMinValue)/double(NTicksDraw);
				char s[80];
//			_fcvt(value, 2, s);
				sprintf(s, "%4.2f", value);
				pDC->TextOutA(tickx-25, Minyx+8, CString(s));
			}

		}

		if (ShowTitle){

//			pDC->TextOut(Minxy+(Maxxy-Minxy)/3, Minyx+5, Titletoshow);
			pDC->TextOut(Minxy+(Maxxy-Minxy)/3, Minyx+20, Titletoshow);
		}
		if (MinBox){
//		if (false){
			CRect r;
			r.left = Minxy-0;
			r.bottom = Minyx+20;
			r.right = r.left + 40;
			r.top = r.bottom - 20;

			MinBox->EnableWindow(!this->Autoscale);

			MinBox->SetFloat(DrawMinValue);
			MinBox->MoveWindow(&r);
		}
		else{
			pDC->TextOut(Minxy-5, Minyx+5, MinString);
		}
		if (MaxBox){
//		if (false){
			CRect r;
			r.left = Maxxy-50;
			r.left = Maxxy-90;
			r.bottom = Minyx+20;
			r.right = r.left + 90;
			r.top = r.bottom - 20;

			MaxBox->EnableWindow(!this->Autoscale);

			MaxBox->SetFloat(DrawMaxValue);
			MaxBox->MoveWindow(&r);
		}
		else{
			pDC->TextOut(Maxxy-30, Minyx+5, MaxString);
		}

		if ( (DrawMinValue<0) && (DrawMaxValue>0)){
			CPen * black = pDC->SelectObject(GrayPen);
			int x0=ScaleDataPoint(0);
			pDC->MoveTo(x0, Minyx);
			pDC->LineTo(x0, Maxyx);
			pDC->SelectObject(black);
		}
	} 
	else{ // Y-AXIS
		pDC->MoveTo(Minyx, Minxy);
		pDC->LineTo(Minyx, Maxxy);
		pDC->MoveTo(Maxyx, Minxy);
		pDC->LineTo(Maxyx, Maxxy);

		for (int i=1; i<NTicksDraw; i++){
			int ticky = Minxy + int(double(i)/double(NTicksDraw)*double(Maxxy-Minxy));
			pDC->MoveTo(Minyx-ticklength, ticky);
			pDC->LineTo(Minyx+ticklength, ticky);
			if (ShowTickValues){
				float value = DrawMinValue+ double(i)*(DrawMaxValue-DrawMinValue)/double(NTicksDraw);
				char s[80];
//			_fcvt(value, 2, s);
				sprintf(s, "%4.2f", value);
				pDC->TextOutA(Minyx-50, ticky-8, CString(s));
			}
		}

		if (ShowTitle){

/*			CFont* pf = pDC->GetCurrentFont( );
			LOGFONT LogFont;
			ZeroMemory (&LogFont, sizeof(LOGFONT));
			pf->GetLogFont(&LogFont );
			LogFont.lfEscapement = 900;
			LogFont.lfOrientation = 0;
			strcpy(LogFont.lfFaceName, "Arial");
//			LogFont.lfHeight = 20;
			CFont Vert;
			Vert.CreateFontIndirect(&LogFont );
*/
		CFont Font;
		Font.CreateFont(14, 0, 900, 900,
						0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
			CFont* pFont = pDC->SelectObject(&Font);

////////			CFont* pFont = pDC->SelectObject(&VertFont);
//			CRect rectClient;
//			GetClientRect(rectClient);
//			pDC->DrawText("Vertical Text", rectClient,
//						DT_SINGLELINE | DT_CENTER | DT_VCENTER);
//			pDC->TextOut(Minyx -20, Minxy + (Maxxy-Minxy)/3, Titletoshow);
			pDC->TextOut(Minyx -70, Minxy + (Maxxy-Minxy)/3, Titletoshow);
			pDC->SelectObject(pFont);
			Font.DeleteObject();

//			CFont * OldFont = pDC->SelectObject(&Vert);
//			pDC->TextOut(Minyx -20, Minxy + (Maxxy-Minxy)/3, Titletoshow);
//			pDC->SelectObject(OldFont);
//			Vert.DeleteObject();
		}

		int sizeTExtBox = Minyx;;
		if (MinBox){
//		if (false){
			CRect r;
			r.left = Minyx-sizeTExtBox;
			r.bottom = Minxy-1;
			r.right = r.left + sizeTExtBox;
			r.top = r.bottom - 20;

			MinBox->EnableWindow(!this->Autoscale);

			MinBox->SetFloat(DrawMinValue);
			MinBox->MoveWindow(&r);
		}
		else{
			pDC->TextOut(Minyx-20, Minxy-10, MinString);
		}
		if (MaxBox){
//		if (false){
			CRect r;
			r.left = Minyx-sizeTExtBox;
			r.bottom = Maxxy+20;
			r.right = r.left + sizeTExtBox;
			r.top = r.bottom - 20;

			MaxBox->EnableWindow(!this->Autoscale);

			MaxBox->SetFloat(DrawMaxValue);
			MaxBox->MoveWindow(&r);
		}
		else{
			pDC->TextOut(Minyx-20, Maxxy, MaxString);
		}
		if ( (DrawMinValue<0) && (DrawMaxValue>0)){
			CPen * black = pDC->SelectObject(GrayPen);
			int y0=ScaleDataPoint(0);
			pDC->MoveTo(Minyx, y0);
			pDC->LineTo(Maxyx, y0);
			pDC->SelectObject(black);
		}
	}
}
bool GraphAxis::PointIsCloseToAxis(CPoint p){
	CRect r;
	int x1, y1, x2, y2;
	if (type==0){
		x1=Minxy; y1=Minyx; x2=Maxxy; y2=Minyx;
	}
	else{
		x1=Minyx; y1=Minxy; x2=Minyx; y2=Maxxy;

	}
	int delta = 4;
	if (p.x<(x1-delta)) return false;
	if (p.x>(x2+delta)) return false;
	if (p.y>(y1+delta)) return false;
	if (p.y<(y2-delta)) return false;
	return true;
}

int GraphAxis::ScaleDataPoint(GRAPH_WINDOW_DATA_TYPE d){
	GRAPH_WINDOW_DATA_TYPE min = this->DrawMinValue;
	GRAPH_WINDOW_DATA_TYPE max = this->DrawMaxValue;
	GRAPH_WINDOW_DATA_TYPE scale;
	if (this->logscale){
		scale = (log(d)-log(min))/((log(max)-log(min)))*GRAPH_WINDOW_DATA_TYPE(Maxxy-Minxy);
	}
	else{
		scale = (d-min)/(max-min)*GRAPH_WINDOW_DATA_TYPE(Maxxy-Minxy);
	}
	int result = (Minxy + int(scale));
	int lower = Minxy;
	int upper = Maxxy;

	if (type == 1){
		lower = Maxxy;
		upper = Minxy;
	}
	if (result < lower) result = lower;
	if (result > upper) result = upper;
	return result;
}


MyPlotClass::MyPlotClass(){
	m_typeplot = 0; // 0: lines; 1: symbols; 2: symbols and error bar
	m_symboltype = 0; // 0: circle; 1: square; 2: triangle
	m_color = 0; // 0 : black; 1: red; 2: green; 3: blue
	pm_Pen = NULL;
	if (!PensCreated){
		MyPens = new CPen **[4];
		for (int i=0; i<4; i++){
			MyPens[i] = new CPen * [256];
		}
		MyThickPens = new CPen **[4];
		for (int i=0; i<4; i++){
			MyThickPens[i] = new CPen * [256];
		}

		MyColors = new COLORREF *[4];
		for (int i=0; i<4; i++){
			MyColors[i] = new COLORREF [256];
		}

		int PenStyle = PS_SOLID;
		int PenWidth = 1;
		int ThickPenWidth = 4;
		for (int i=0; i<256; i++){
/*			for (int j=0; j<4; j++){
				MyPens[j][i] = new CPen;
				MyThickPens[j][i] = new CPen;
			}
*/
			MyColors[0][i] = RGB(255-i,255-i,255-i);
			MyColors[1][i] = RGB(i,0,0);
			MyColors[2][i] = RGB(0,i,0);
			MyColors[3][i] = RGB(0,0,i);
//			MyColors[1][i] = RGB(255-i,0,0);
//			MyColors[2][i] = RGB(0,255-i,0);
//			MyColors[3][i] = RGB(0,0,255-i);
			for (int k=0; k<4; k++){
				MyPens[k][i] = new CPen;
				MyThickPens[k][i] = new CPen;
				MyPens[k][i]->CreatePen(PenStyle, PenWidth, MyColors[k][i]);
				MyThickPens[k][i]->CreatePen(PenStyle, ThickPenWidth, MyColors[k][i]);
			}
		}
		PensCreated = true;
	}
//	pm_ToolTip = NULL;
}
void MyPlotClass::Cleanup(){
//	for (int i=0; i<4; i++){
//		for (int j=0; j<256; j++){
//			ShowFloat(i, "i");
//			ShowFloat(j, "j");
//			if (MyPens[i][j]){
//				Beep(1000.0, j);
//			}
//			delete MyPens[i][j]; //->DeleteObject();
//			MyPens[i][j] = NULL;
//
//		}
//		delete [] MyPens[i];
//	}
//	delete [] MyPens;
}
MyPlotClass::~MyPlotClass(){
//	if (m_Pen) delete m_Pen;
	pm_Pen = NULL;
	pm_ThickPen = NULL;
//	if (pm_ToolTip){
//		delete pm_ToolTip;
//	}

}
void MyPlotClass::InitPen(){
//	if (m_Pen) delete m_Pen;
//	m_Pen = NULL;
	//m_Pen = new CPen;
//	int PenStyle = PS_SOLID;
//	int PenWidth = 1;
//	m_intensity_color /= 2;
//	ShowFloat(m_color, "m_Color");
	pm_Pen = MyPens[m_color][m_intensity_color];
	pm_ThickPen = MyThickPens[m_color][m_intensity_color];
	this->m_color = this->MyColors[m_color][m_intensity_color];
	

/*
CPen m_Pen;
	switch ( m_color){
		//black
		case 0 : m_Pen.CreatePen(PenStyle, PenWidth, RGB(255-m_intensity_color,255-m_intensity_color,255-m_intensity_color));
				break;
		// red
		case 1 : m_Pen.CreatePen(PenStyle, PenWidth, RGB(m_intensity_color,0,0));
				break;
		// green
		case 2 : m_Pen.CreatePen(PenStyle, PenWidth, RGB(0,m_intensity_color,0));
				break;
		// blue
		case 3 : m_Pen.CreatePen(PenStyle, PenWidth, RGB(0,0,m_intensity_color));
				break;
		default: m_Pen.CreatePen(PenStyle, PenWidth, RGB(255-m_intensity_color,255-m_intensity_color,255-m_intensity_color));
				break;
	}
*/
}


GraphWnd::GraphWnd(CWnd * owner, CRect r){

//	IsCalculating = false;
//	MyDC = NULL;
	MouseIsOverLine = false;
	InterruptClicked = false;
	ShowInterruptButton = false;
	FirstTime = true;
	InterruptButton = NULL;

	ShowWaitCursor = false; //true;
	ShowLastValue = false;
	pGroup = NULL;
//	series = NULL;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE  ;

	this->Create(NULL, NULL, dwStyle, r, owner, 0, NULL);
//	MyWindowRect = r;


	Dotted = new CPen(PS_DOT, 1, RGB(255,0,0));
	SolidBlack = new CPen(PS_SOLID, 1, RGB(0,0,0));
	BluePen = new CPen(PS_SOLID, 1, RGB(0,0,255));

	RedPen= new CPen(PS_SOLID, 1, RGB(255,0,0));;
	GreenPen= new CPen(PS_SOLID, 1, RGB(0,255,0));;
	YellowPen= new CPen(PS_SOLID, 1, RGB(255,255,0));;
	GrayPen= new CPen(PS_SOLID, 1, RGB(150,150,150));

	ShowTitle = true;
	
	CRect r6;

	xaxis = new GraphAxis();
	xaxis->type = 0;
	xaxis->ShowTitle = true;
	xaxis->Title = "Title";

	yaxis = new GraphAxis();
	yaxis->type = 1;
	yaxis->ShowTitle = true;
	yaxis->Title = "Title";

		
	MinXTextBox = new Text(ID_MINX_TEXT_BOX, 0.0, -1e18, 1e18, true,	r6, this);
	xaxis->MinBox = MinXTextBox;

	MaxXTextBox = new Text(ID_MAXX_TEXT_BOX, 0.0, -1e18, 1e18, true,	r6, this);
	xaxis->MaxBox = MaxXTextBox;

	MinYTextBox = new Text(ID_MINY_TEXT_BOX, 0.0, -1e18, 1e18, true,	r6, this);
	yaxis->MinBox = MinYTextBox;

	MaxYTextBox = new Text(ID_MAXY_TEXT_BOX, 0.0, -1e18, 1e18, true,	r6, this);
	yaxis->MaxBox = MaxYTextBox;

}

GraphWnd::~GraphWnd()
{
//	delete xaxis;
//	delete yaxis;
	if (MinXTextBox){
//		delete MinXTextBox; // = NULL;
		MinXTextBox = NULL;
	}
	if (MaxXTextBox){
		MaxXTextBox = NULL;
	}
	if (MinYTextBox){
//		delete MinXTextBox; // = NULL;
		MinYTextBox = NULL;
	}
	if (MaxYTextBox){
		MaxYTextBox = NULL;
	}
	delete Dotted;
	delete SolidBlack;
	delete RedPen;
	delete GreenPen;
	delete BluePen;
	delete YellowPen;
	delete GrayPen;
	delete yaxis;
	delete xaxis;
	if (pGroup){
		delete pGroup;
	}
	if (InterruptButton){
		delete InterruptButton;
	}

}
BEGIN_MESSAGE_MAP(GraphWnd, CWnd)
	//{{AFX_MSG_MAP(GraphWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_GRAPHMENU_EXPORTFILE, OnExportToFile)
	ON_COMMAND(ID_GRAPHMENU_EXPORTCLIPBOARD, OnExportToClipboard)
	ON_COMMAND(ID_AXISMENU_SHOWTITLE, OnAxisMenuShowTitle)
	ON_COMMAND(ID_AXISMENU_AUTOSCALE, OnAxisAutoScale)
	ON_COMMAND(ID_AXISMENU_LOGSCALE, OnAxisLogScale)
	ON_COMMAND(ID_AXISMENU_SHOWTICKVALUES, OnAxisTickValues)
	ON_COMMAND(ID_AXISMENU_NTICKS, OnAxisNTicks)
	ON_COMMAND(ID_GRAPHMENU_EXPORTGEPULSEFILE, OnExportGepulse)
	ON_COMMAND(ID_GRAPHMENU_INTERRUPTBUTTON, OnClickInterruptButton)

	ON_EN_KILLFOCUS( ID_MINX_TEXT_BOX, OnXAxisMinChanged)
	ON_EN_KILLFOCUS( ID_MAXX_TEXT_BOX, OnXAxisMaxChanged)
	ON_EN_KILLFOCUS( ID_MINY_TEXT_BOX, OnYAxisMinChanged)
	ON_EN_KILLFOCUS( ID_MAXY_TEXT_BOX, OnYAxisMaxChanged)
	
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



void GraphWnd::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CWnd::PostNcDestroy();
	//delete this;
}

void GraphWnd::CreateButtons(){
	MakeButton(InterruptButton, "Interrupt", this, ID_GRAPHMENU_INTERRUPTBUTTON);

}
void GraphWnd::MoveButtons(){
	if (ShowInterruptButton){
		CRect rect;
		rect.left = 370;
		rect.right = rect.left+60;
		rect.top =5;
		rect.bottom = rect.top +20;
		InterruptButton->MoveWindow(&rect);
		InterruptButton->ShowWindow(SW_SHOW);
//		Beep(1000,100);
	}
	else{
		InterruptButton->ShowWindow(SW_HIDE);
//		Beep(500,50);
	}
}
void GraphWnd::GetSmallLineCoordinates(int lineindex, int & x1, int & x2, int & y1){
	CRect rect;
	this->GetClientRect(rect);
	x1=rect.right-50;
	x2=x1+40;
	int y=rect.top;
	y1=y+10+lineindex*10;
}

void GraphWnd::OnPaint() 
{
	bool UseNewDrawStyle = true;

//	Beep(1000,100);
/*
if (IsCalculating) {
		return;
	}
*/
	CWaitCursor * pdummy = NULL;
	if (ShowWaitCursor){
		pdummy = new CWaitCursor;
	}
//	CalcMinAndMaxValues();
	CPaintDC dc(this); // device context for painting
	CDC * pDC = &(dc);
	CRect r;
	GetClientRect(r);
	if (FirstTime){
		FirstTime = false;
		CreateButtons();
	}
//	ShowFloat(r.right, "r");

	CBrush * pb = pDC->GetCurrentBrush();
	pDC->FillRect(&r, pb);
//	pDC->TextOut(10,10, "Hi");


	CalcMinAndMaxValues();

	xaxis->Minxy = r.left+80;
	xaxis->Maxxy = r.right-2;
	xaxis->Minyx = r.bottom-20;
	xaxis->Minyx = r.bottom-40;
	xaxis->Maxyx = r.top+5;
	xaxis->Draw(pDC);

	yaxis->Minyx = xaxis->Minxy;
	yaxis->Maxyx = xaxis->Maxxy;
	yaxis->Minxy = xaxis->Minyx;
	yaxis->Maxxy = xaxis->Maxyx;
	yaxis->Draw(pDC);

//	CPen * OldPen = pDC->SelectObject(SolidBlack);
//	int imax =plots.size();
//	if (imax>10) imax = 10;
	if (ShowTitle){
//		dc.TextOut(r.left+55, r.top+5, Title);
//		pDC->TextOut(xaxis->Minyx+1, r.top+7, Title);
		pDC->TextOut(r.left+100, r.top+7, Title);
	}

	if (plots.size()<1){
		dc.TextOutA(100, 100, "No plots yet");
		return;
	}
	vector<CPoint> PointsVector;

	for (int i=0; i<plots.size(); i++){
		PointsVector.resize(0);
		CPen * OldPen ;
//	for (int i=0; i<imax; i++){
		//CDC * pDC = GetDC();

		if (true){
//			if (plots[i].m_Pen){
			if (true){
//				OldPen = pDC->SelectObject(&(plots[i].m_Pen));
//				OldPen = pDC->SelectObject(plots[i].pm_Pen);
				if (MouseIsOverLine && (OverLineIndex == i)){
/*					CFont Font;
					Font.CreateFont(20, 0, 0, 0,
						10, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");

					CFont* pFont = pDC->SelectObject(&Font);
					
					pDC->SetTextColor(this->plots[i].m_color);
					pDC->TextOut(200, 25, this->plots[i].m_OverMouseText);
					Font.DeleteObject();
*/
					OldPen = pDC->SelectObject(plots[i].pm_ThickPen);

				}
				else{
					OldPen = pDC->SelectObject(plots[i].pm_Pen);
				}
			}
			else{
				switch (plots[i].m_color){// 0 : black; 1: red; 2: green; 3: blue
					case 0 : OldPen = pDC->SelectObject(SolidBlack); break;
					case 1 : OldPen = pDC->SelectObject(RedPen); break;
					case 2 : OldPen = pDC->SelectObject(GreenPen); break;
					case 3 : OldPen = pDC->SelectObject(BluePen); break;
					default: OldPen = pDC->SelectObject(SolidBlack); break;
				}
			}
		}

		int NPointsPlot = plots[i].data.size();
			
		CRect rc;
		GetWindowRect(rc);
		if (NPointsPlot>(2*(rc.right-rc.left))){
			UseNewDrawStyle = true;
		}
		else{
			UseNewDrawStyle = false;
		}		
		
		for (int j=0; j<NPointsPlot; j++){
			DataPointClass d = plots[i].data[j];
			int x = xaxis->ScaleDataPoint(d.data[0]);
				//ShowFloat(d.data[0], "x");
				//ShowFloat(x, "ix");
			int y = yaxis->ScaleDataPoint(d.data[1]);
			int y1=y;
			int y2=y;
			if (plots[i].m_typeplot == 2){
				y1 = yaxis->ScaleDataPoint(d.data[1]+d.data[2]);
				y2 = yaxis->ScaleDataPoint(d.data[1]-d.data[2]);
			}

			if (plots[i].m_typeplot==0){
				if (UseNewDrawStyle){
					PointsVector.push_back(CPoint(x, y));
					if (j==(NPointsPlot-1)){
//						MyEnhancedPolyLine(PointsVector, NPointsPlot, pDC);
						MyEnhancedPolyLine(PointsVector, pDC);
					}
				}
				else{
					if (j==0){
						pDC->MoveTo(x, y);
					}
					else{
						pDC->LineTo(x, y);
					}
				}
				if (j==(NPointsPlot-1)){
//				if ((this->plots.size()>1) && (j==(NPointsPlot-1))){
					int x1, x2, y1;
					GetSmallLineCoordinates(i, x1, x2, y1);
/*
					CRect rect;
					this->GetClientRect(rect);
					int x1=rect.right-50;
					int x2=x1+40;
					int y=rect.top;
					int y1=y+10+i*10;
*/
					pDC->MoveTo(x1,y1);
					pDC->LineTo(x2,y1);
				}

			}
			if (plots[i].m_typeplot>0){
				int size = 4;
				DrawSymbol(plots[i].m_symboltype, x, y, size, pDC);
			}
			if (ShowLastValue && (i==plots.size()-1) && (j==(NPointsPlot-1))){
				DataPointClass d = plots[i].data[j];
				
				char s[20];
				_gcvt(d.data[1], 6, s);
				pDC->TextOutA(100, 100, CString("                 "));
				pDC->TextOutA(100, 100, CString(s));
				if (ShowInterruptButton){
					DrawFilledGradientArrow(
					   pDC,
						CPoint(x, y), 
						CPoint (x+30, y),
						0, 0, 255, //0,255,0,
						255, 0, 0,
						5, //Thickness
						10, //Widtharrow
						15); // SizeTipe
				}

			}
		}
		if (true){
			pDC->SelectObject(OldPen);
		}
		//ReleaseDC(pDC);
	}
	MoveButtons();
	if (MouseIsOverLine && (OverLineIndex < this->plots.size())){
		CFont Font;
		Font.CreateFont(20, 0, 0, 0,
			FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");

		CFont* pFont = pDC->SelectObject(&Font);
					
		pDC->SetTextColor(this->plots[OverLineIndex].m_color);
		pDC->TextOut(100, 25, this->plots[OverLineIndex].m_OverMouseText);
		Font.DeleteObject();

//		OldPen = pDC->SelectObject(plots[i].pm_ThickPen);

	}

	pDC->SelectObject(GetStockObject(DC_PEN));
	if (ShowWaitCursor){
		delete pdummy;
	}

	//pDC->SelectObject(OldPen);
//	ReleaseDC(pDC);
//	delete pDC;
}

//void GraphWnd::UpDateDC(){
//	if (MyDC) ReleaseDC(MyDC);
//	MyDC = GetDC();
//}

void GraphWnd::OnMenuAxis(GraphAxis * axis, UINT nFlags, CPoint point){
	CMenu m_FileMenu;
	VERIFY(m_FileMenu.CreatePopupMenu());
	m_FileMenu.AppendMenu(MF_STRING, ID_AXISMENU_SHOWTITLE, _T("Toggle show title"));
	m_FileMenu.AppendMenu(MF_STRING, ID_AXISMENU_AUTOSCALE, _T("Toggle autoscale"));
	m_FileMenu.AppendMenu(MF_STRING, ID_AXISMENU_LOGSCALE, _T("Toggle log scale"));
	m_FileMenu.AppendMenu(MF_STRING, ID_AXISMENU_SHOWTICKVALUES, _T("Toggle show tick values"));
	m_FileMenu.AppendMenu(MF_STRING, ID_AXISMENU_NTICKS, _T("Set number of ticks"));
	
	
	CRect rc;
	GetWindowRect(rc);

	int x=point.x+rc.left;
	int y=point.y+rc.top;

	selectedAxis = axis;
	m_FileMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, this);

}

void GraphWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnRButtonDown(nFlags, point);
	if (xaxis->PointIsCloseToAxis(point)){
		OnMenuAxis(xaxis, nFlags, point);
		return;
	}
	if (yaxis->PointIsCloseToAxis(point)){
		OnMenuAxis(yaxis, nFlags, point);
		return;
	}

	CMenu m_FileMenu;
	VERIFY(m_FileMenu.CreatePopupMenu());
	m_FileMenu.AppendMenu(MF_STRING, ID_GRAPHMENU_EXPORTCLIPBOARD, _T("Export to clipboard"));
	m_FileMenu.AppendMenu(MF_STRING, ID_GRAPHMENU_EXPORTFILE, _T("Export to text file"));
	if (pGroup){
		m_FileMenu.AppendMenu(MF_STRING, ID_GRAPHMENU_EXPORTGEPULSEFILE, _T("Export to GePulse file"));
	}

	CRect rc;
	GetWindowRect(rc);

	int x=point.x+rc.left;
	int y=point.y+rc.top;

	m_FileMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, this);

}

void GraphWnd::OnAxisMenuShowTitle(){
	selectedAxis->ShowTitle = !selectedAxis->ShowTitle;
	Invalidate();
}
void GraphWnd::OnAxisAutoScale(){
	selectedAxis->Autoscale = !selectedAxis->Autoscale;
	Invalidate();
}
void GraphWnd::OnAxisLogScale(){
	selectedAxis->logscale = !selectedAxis->logscale;
	Invalidate();
}
void GraphWnd::OnAxisTickValues(){
	selectedAxis->ShowTickValues = !selectedAxis->ShowTickValues;
	Invalidate();
}
void GraphWnd::OnAxisNTicks(){
	int NTicksOld = selectedAxis->NTicks;
	int NTicksNew = NTicksOld;
	io myio;
	if (myio.InInt(NTicksNew, "Number of ticks") != IDOK){
		return;
	}
	if (NTicksNew<0){
		NTicksNew = 0;
	}
	if (NTicksNew == NTicksOld){
		return;
	}
	selectedAxis->NTicks = NTicksNew;
	Invalidate();
}
void GraphWnd::OnXAxisMinChanged(){

	//Alert0("hi");
	xaxis->ManualMinValue = MinXTextBox->GetFloat();
	MinXTextBox->SetFloat(xaxis->ManualMinValue);
	Invalidate();
}
void GraphWnd::OnXAxisMaxChanged(){

	//Alert0("hi");
	xaxis->ManualMaxValue = MaxXTextBox->GetFloat();
	MaxXTextBox->SetFloat(xaxis->ManualMaxValue);
	Invalidate();
}
void GraphWnd::OnYAxisMinChanged(){

	//Alert0("hi");
	yaxis->ManualMinValue = MinYTextBox->GetFloat();
	MinYTextBox->SetFloat(yaxis->ManualMinValue);
	Invalidate();
}
void GraphWnd::OnYAxisMaxChanged(){

	//Alert0("hi");
	yaxis->ManualMaxValue = MaxYTextBox->GetFloat();
	MaxYTextBox->SetFloat(yaxis->ManualMaxValue);
	Invalidate();
}
void GraphWnd::ClearPlots(){
	plots.resize(0);
	CalcMinAndMaxValues();
	if (pGroup){
		delete pGroup;
		pGroup = NULL;
	}
}
void GraphWnd::AddPlot(int typeplot, int symboltype, int color, int ColorIntensity,
					   CString OverMouseText){

	MyPlotClass plot;
	plot.m_typeplot = typeplot;
	plot.m_symboltype = symboltype;
	plot.m_color = color;
	plot.m_intensity_color = ColorIntensity;
	plot.m_OverMouseText = OverMouseText;
	plots.push_back(plot);
	plots[plots.size()-1].InitPen();
//	plots[plots.size()-1].pm_ToolTip = new CToolTipCtrl;
//	plots[plots.size()-1].pm_ToolTip->Create(this);
//	plots[plots.size()-1].pm_ToolTip->UpdateTipText(OverMouseText, this);



}

void GraphWnd::AddPointToLastPlot(GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y){
	AddPointToLastPlot(x, y, 0.0);
}
void GraphWnd::AddPointToLastPlot(GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y, GRAPH_WINDOW_DATA_TYPE error){
	int n=plots.size();
	if (n<1) return;
	AddPoint(n-1, x, y, error);
}
void GraphWnd::AddPoint(int plotnumber, GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y){
	AddPoint(plotnumber, x, y, 0.0);
}
void GraphWnd::AddPoint(int plotnumber, GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y, GRAPH_WINDOW_DATA_TYPE error){
	int n=plots.size();
	if (plotnumber>=n) return;
	if (plotnumber<0) return;
	DataPointClass d;
	d.data[0] = x;
	d.data[1] = y;
	d.data[2] = error;
	plots[plotnumber].data.push_back(d);
//	CalcMinAndMaxValues();
}

void GraphWnd::CalcMinAndMaxValues(){
	for (int k=0; k<1; k++){
		if (k==0){
			xaxis->DataMaxValue = yaxis->DataMaxValue = -1e20;
			xaxis->DataMinValue = yaxis->DataMinValue = 1e20;
		}
		for (int i=0; i<plots.size(); i++){
			for (int j=0; j<plots[i].data.size(); j++){
				GRAPH_WINDOW_DATA_TYPE x=plots[i].data[j].data[0];
				GRAPH_WINDOW_DATA_TYPE y=plots[i].data[j].data[1];
				GRAPH_WINDOW_DATA_TYPE e = 0;
				if (plots[i].m_typeplot == 2){
					e = plots[i].data[j].data[2];
				}
				GRAPH_WINDOW_DATA_TYPE y1=y+e;
				GRAPH_WINDOW_DATA_TYPE y2=y-e;
				if (x<xaxis->DataMinValue) xaxis->DataMinValue = x;
				if (x>xaxis->DataMaxValue) xaxis->DataMaxValue = x;
				bool Checky = true;
			
				if (!xaxis->Autoscale){
					if (x<xaxis->ManualMinValue){
						Checky=false;
					}
					if (x>xaxis->ManualMaxValue){
						Checky=false;
					}
				}
				if (Checky){
					if (y1<yaxis->DataMinValue) yaxis->DataMinValue = y1;
					if (y1>yaxis->DataMaxValue) yaxis->DataMaxValue = y1;
					if (y2<yaxis->DataMinValue) yaxis->DataMinValue = y2;
					if (y2>yaxis->DataMaxValue) yaxis->DataMaxValue = y2;
				}
			}
		}
	}
}
void GraphWnd::SetAxisTitle(int axis, CString title){ // 0: x, 1: y
	GraphAxis * paxis = xaxis;
	if (axis != 0){
		paxis = yaxis;
	}
	paxis->Title = title;
	Invalidate();

}
void  GraphWnd::GetExportString(CString & c){
	int MaxPoints = 0;
	int NPlots = this->plots.size();
	for (int i=0; i<NPlots; i++){
		int NPoints = plots[i].data.size();
		if (NPoints>MaxPoints){
			MaxPoints = NPoints;
		}
	}
	for (int p=0; p<MaxPoints; p++){
		bool StartLine = true;
		for (int i=0; i<NPlots; i++){
			MyPlotClass & plot = plots[i];
			bool UseError = (plot.m_typeplot == 2);
			if (!StartLine){
				c += "\t";
			}
			StartLine = false;
			if (p<plot.data.size()){
				char s[20];
				_gcvt(plot.data[p].data[0], 6, s); //x
				c += CString(s)+"\t";

				_gcvt(plot.data[p].data[1], 6, s); //y
				c += CString(s);
				if (UseError){
					c += "\t";
					_gcvt(plot.data[p].data[2], 6, s); //error
					c += CString(s);
				}
			}
			else{
				c += CString("0")+"\t"+CString("0");
				if (UseError){
					c += "\t";
					c += CString("0");
				}
			}
		}
		if (p<(MaxPoints-1)){
			c += "\r\n";
		}
	}
}

void GraphWnd::OnExportToFile(){
	CString c;
	GetExportString(c);
	if (c.GetLength()<1) return;
	CString Name;
	CFileDialog
    diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());

	if (diag.DoModal()!=IDOK){
		return;
	}

	CString OutFileName = diag.GetPathName();
	CFile file;
	file.Open ( OutFileName, CFile::modeCreate | CFile::modeWrite);
	file.Write(c.GetBuffer(), c.GetLength());
}
void GraphWnd::OnExportToClipboard(){
	CString c;
	GetExportString(c);
	if (c.GetLength()<1) return;
	CopyTextToClipboard(c);

}

void GraphWnd::OnExportGepulse(){
	if (!pGroup){
		return;
	}
	CString Name;
	CFileDialog
    diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());

	if (diag.DoModal()!=IDOK){
		return;
	}

	CString OutFileName = diag.GetPathName();
	CFile file;
	file.Open ( OutFileName, CFile::modeCreate | CFile::modeWrite);

	pGroup->WriteOnFile(&file, CURRENT_GEPULSE_PULSEANDSTIM_VERSION);
	
}

void GraphWnd::AddSweep( int NChannels,
						 bool NewSeries,
						 int sweepcount,
						GeStimulation * pStim,
						bool WillHaveLeak,
						double VHold
								){

	if (!pGroup){
		pGroup = new Group;
	}

	if (NewSeries){

		GeSeries * series = new GeSeries;

		series->NumberOfSweeps = sweepcount+1;

		series->NumberOfChannels = NChannels;

		series->Stim = GeStimulation::CopySingle(pStim);

		if (!WillHaveLeak){
			series->Stim->LeakCount = 0;
		}

		series->SweepType = SweepTypePulsed;
		series->gfParams = NULL;
		series->BackgroundNoise =0.0;
		series->Comment = CString("Markov Editor generated");
		series->RecordingMode = RecordingModeWholeCell;
		series->VHold = VHold;

		if (!series->Stim){
			Alert0("failed copy stim");
		}
//		ShowFloat(series->Stim->Segments->Duration, "t0");

		pGroup->AddSeries(series);
	}
	GeSeries * series = (GeSeries * ) ListObject::Last(pGroup->series);
//	ShowFloat(series->Stim->Segments->Duration, "t0");

	Sweep * sweep = new Sweep;

	sweep->StimCount = sweepcount+1;
	sweep->SweepCount = sweepcount+1;

	sweep->AverageCount = 1;
	sweep->Leak = WillHaveLeak; //false;
	series->Stim->GetNPoints(sweepcount+1, sweep->DataPoints);
//	ShowFloat(sweep->DataPoints, "sweep->DataPoints");
	sweep->NChannels = NChannels;

	UINT datasize = 2;
	UINT write = datasize*sweep->DataPoints;

	for (int chan=0; chan<NChannels; chan++){
		BYTE * d  = new BYTE[write];
		sweep->DataPointer[chan] = d;
		if (sweep->Leak){
			BYTE * d  = new BYTE[write];
			sweep->P4Pointer[chan] = d;
		}
		sweep->DataSizeInBytes = datasize;
	}
	
	series->AddSweep(sweep);
}

 void GraphWnd::FillSeries(){
	// Alert0("hi");
	 if (!pGroup) {
		 Alert0("no group");
		 return;
	 }
	GeSeries * series = (GeSeries * ) ListObject::Last(pGroup->series);
	if (!series){
		Alert0("no series");
		return;
	}

	int NSweeps = ListObject::LengthList(series->sweeps);
//	ShowFloat(NSweeps, "nswee");
	int NChannels = series->NumberOfChannels;
//	ShowFloat(NChannels, "NChann");
	int NTraces = NSweeps*NChannels;
//	ShowFloat(NTraces, "NTr");
	int NPlots = plots.size();
//	ShowFloat(NPlots, "NPLots");
	if (NSweeps<1){
		Alert0("no sweeps");
		return;
	}
	if (NTraces>NPlots){
		Alert0("not enough plots in add series");
		return;
	}
	CalcMinAndMaxValues();
	GRAPH_WINDOW_DATA_TYPE AbsMax = fabs(yaxis->DataMaxValue);
	if (fabs(yaxis->DataMinValue)>AbsMax){
		AbsMax = fabs(yaxis->DataMinValue);
	}

	GRAPH_WINDOW_DATA_TYPE scaleCurrent=32767.0/AbsMax;
	for (int chan=0; chan<NChannels; chan++){
		series->DataFactor[chan] = 1.0/scaleCurrent;
		series->DataFactor[chan] *= 1.0e-12; // in pA

	}
	Sweep * sweep = series->sweeps;
	for (int i=0; i<NSweeps; i++){
		int iplot0 = NPlots-NTraces+i*NChannels;
		for (int chan=0; chan<NChannels; chan++){
			int iplot = iplot0 + chan;
			int NData = plots[iplot].data.size();
//			ShowFloat(NData, "NData");
			int NDataInSweep = sweep->DataPoints;
			if (NDataInSweep<NData){
				NData = NDataInSweep;
			}
//			ShowFloat(NData, "NData");
//			ShowFloat(NDataInSweep, "NDataInSweep");
			__int16 * pdata = (__int16 * )sweep->DataPointer[chan];
			for (int k=0; k<NData; k++){
				pdata[k] = __int16(scaleCurrent*plots[iplot].data[k].data[1]);
			}
		}
		sweep = (Sweep *)sweep->Next;
	}
}

 void GraphWnd::OnClickInterruptButton(){

	 InterruptClicked = true;
 }
 void GraphWnd::OnMouseMove(UINT nFlags, CPoint point)
 {
	 // TODO: Add your message handler code here and/or call default

	 

	if ( !(nFlags && MK_CONTROL ) && //
		 !(nFlags && MK_LBUTTON ) && //Set if the left mouse button is down.
		 !(nFlags && MK_MBUTTON ) && //Set if the middle mouse button is down.
		 !(nFlags && MK_RBUTTON ) && //Set if the right mouse button is down.
		 !(nFlags && MK_SHIFT )  //Set if the SHIFT key is down.
		){
			if (MouseIsOverLine){
				if (CheckIfToExitMouseOverLine(point)){
//					Beep(500, 100);
//					this->plots[OverLineIndex].pm_ToolTip->Activate(false);

					MouseIsOverLine = false;
					Invalidate();
				}
			}
			else{
				if (CheckIfToEnterMouseOverLine(point)){
					MouseIsOverLine = true;
//					Beep(1000,10);
					Invalidate();
				}
			}
//			Beep(1000,1);
		}
		else{
			if (MouseIsOverLine){
				MouseIsOverLine = false;
//				Beep(500, 100);
				Invalidate();
			}
		}
		CWnd::OnMouseMove(nFlags, point);
 }
void GraphWnd::GetMouseOverRect(int lineindex, CRect & r){
	int x1, x2, y1;
	GetSmallLineCoordinates(lineindex, x1, x2, y1);
	int SizeRect = 4;
	r.left = x1-SizeRect;
	r.right = x2 + SizeRect;
	r.top = y1 - SizeRect;
	r.bottom = y1 + SizeRect;
}

 bool GraphWnd::CheckIfToExitMouseOverLine(CPoint point){
	 int NPlots = this->plots.size();
	 CRect r;
	 GetMouseOverRect(OverLineIndex, r);
	 if (point.x<r.left){
		 return true;
	 }
	 if (point.x>r.right){
		 return true;
	 }
	 if (point.y<r.top){
		 return true;
	 }
	 if (point.y>r.bottom){
		 return true;
	 }
	 return false;
 }
 bool GraphWnd::CheckIfToEnterMouseOverLine(CPoint point){
	 int NPlots = this->plots.size();

	 for (int i=0; i<NPlots; i++){
		 CRect r;
		 GetMouseOverRect(i, r);
		 if (point.x>=r.left){
			 if (point.x<=r.right){
				 if (point.y>=r.top){
					 if (point.y<=r.bottom){
						 OverLineIndex = i;

//						 this->plots[i].pm_ToolTip->Activate(true);
						 
						 return true;
					 }
				 }
			 }
		 }
	 }
	 return false;
 }
