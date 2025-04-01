// Traces.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "Traces.h"
#include "../common/CannyEdge.h"

#include "DiagEdgeDetect.h"
#include "DiagMembranePolys.h"
#include "../common/ConnectedComponent.h"
#include "../cdib/cdib.h"
#include "DiagSelectPoly.h"


#include <vector>

//using namespace std ;

//#include "Traces.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Traces

IMPLEMENT_DYNCREATE(Traces, CView)


Traces::Traces(){
//	for (int i=0; i<255; i++){
//		GreyColorTable256[i]=RGB(i,i,i);
//	}


//	comp = NULL;
	OnTrackingCells = false;
	IsCounting = false;;

	SelectedInitialized = false;
	ShowFill = true;

	MyBitMapBuffer = NULL;
	SizeBitmapBuffer = 0;
	Hidden64Buff = NULL;

	UpdateBitmapBuffer(10000);

	OnShiftingOrigin = false;
	OriginDC = CPoint(-200, -200);
	OriginDC = CPoint(0, 0);
	MaxPolyPoints = 20;

	CannyEdgeDetector ch(1, 1);

	lowThreshold = ch.lowThreshold;
	highThreshold = ch.highThreshold;
	gaussianKernelRadius = ch.gaussianKernelRadius;
	gaussianKernelWidth = ch.gaussianKernelWidth;
	MaxDistPolyPoint = 8;

	Zoom = 1;
	Overlay = false;
	ZoomIndex = 0;
	Zoom = 1;
	ZoomIn = false;
//	RealZoom = 0.5;
	UINT z=1;
	for (int i=0; i<10; i++){
		ZoomTable[i]=z;
		z*=2;
	}
	firsttime = true;
//	PercentageAcceptanceFill = 1.0;
	OnCreatingPolygon = false;
	OnMovePolygon = false;
	OnMoveLine = false;
	OnCreatingLine = false;
	OnMovingPolyWithArrowKey = false;
	OnMovingLineWithArrowKey = false;

	SelectingFillPoints = false;
	SelectingFillPointsInSelectedPolygon = false;
	selectedpolygon = 0;
	selectedline = 0;
	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	blackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	whitePen.CreatePen(PS_SOLID, 1, RGB(255,255,255));

/*
switch (pDoc->dat.p.ViewType){
		case 0 : OnViewShowrectangle(); break;
		case 1 : OnViewShowconus(); break;
		default : OnViewShowpolygons(); break;
	}
*/
}

Traces::~Traces(){
//	if (comp) delete comp;
	if (Hidden64Buff){
		delete [] Hidden64Buff;
	}
}

void Traces::UpdateBitmapBuffer(int size){
	if (size<SizeBitmapBuffer){
		return;
	}
	if (Hidden64Buff){
		delete [] Hidden64Buff;
	}

	Hidden64Buff = new __int64[size];
	SizeBitmapBuffer = size;
	MyBitMapBuffer = (int *) Hidden64Buff;
}


BEGIN_MESSAGE_MAP(Traces, CView)
	//{{AFX_MSG_MAP(Traces)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS( ID_OFFSET_X_PDCORIGIN, OnImageOffsetxChanged)
	ON_EN_KILLFOCUS( ID_OFFSET_Y_PDCORIGIN, OnImageOffsetyChanged)

	ON_EN_KILLFOCUS( ID_MINSCALE_FIELD, OnMinChanged)
	ON_EN_KILLFOCUS( ID_MAXSCALE_FIELD, OnMaxChanged)

	ON_BN_CLICKED( ID_RELOAD_BUTTON, ReloadButtonClicked)
	ON_BN_CLICKED( ID_RESET_BUTTON, ResetButtonClicked)
	ON_BN_CLICKED( ID_COPYTHISTOGENERAL_BUTTON, CopyThisToGeneralButtonClicked)
	ON_BN_CLICKED( ID_COPYGENERALTOTHIS_BUTTON, CopyGeneralToThisButtonClicked)

	ON_BN_CLICKED( ID_OFFSET_X_PLUS_10_BUTTON,  OnOffsetXPlus10Clicked)
	ON_BN_CLICKED( ID_OFFSET_X_PLUS_1_BUTTON,   OnOffsetXPlus1Clicked)
	ON_BN_CLICKED( ID_OFFSET_X_MINUS_10_BUTTON, OnOffsetXMinus10Clicked)
	ON_BN_CLICKED( ID_OFFSET_X_MINUS_1_BUTTON,  OnOffsetXMinus1Clicked)

	ON_BN_CLICKED( ID_OFFSET_Y_PLUS_10_BUTTON,  OnOffsetYPlus10Clicked)
	ON_BN_CLICKED( ID_OFFSET_Y_PLUS_1_BUTTON,   OnOffsetYPlus1Clicked)
	ON_BN_CLICKED( ID_OFFSET_Y_MINUS_10_BUTTON, OnOffsetYMinus10Clicked)
	ON_BN_CLICKED( ID_OFFSET_Y_MINUS_1_BUTTON,  OnOffsetYMinus1Clicked)

	ON_BN_CLICKED( ID_TRACK_THIS_BUTTON, OnTrackThisButtonClicked)

	ON_BN_CLICKED( ID_AUTOSCALE_BUTTON, OnAutoscaleButtonClicked)
//	ON_WM_VSCROLL(ID_MIN_SLIDERCTRL, OnMinSliderSelected)
//	ON_WM_LBUTTONDOWN(ID_MIN_SLIDERCTRL, OnMinSliderSelected)

	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Traces drawing

void Traces::Init(){
	frame = (CMainFrame *)(AfxGetMainWnd( ));
	child = (CChildFrame *)(GetParent()->GetParent()->GetParent());
	pDoc = (CAnaVisionDoc *) GetDocument();

	InitGenerator = &(InitConusGenerator);

	SelectMode = 0; // Move
//	SelectMode = 1; // Size
	MouseIsDown = false;
	InitRectGenerator = true;
	InitConusGenerator = true;

}
void Traces::CreateButtons(){
	
	CRect r;

	ReloadButton = new Button(ID_RELOAD_BUTTON, false, CString("Reload"), r, this);
	ResetButton = new Button(ID_RESET_BUTTON, false, CString("Reset"), r, this);

	OffsetXPlus10Button  = new Button(ID_OFFSET_X_PLUS_10_BUTTON,  false, CString(">10"), r, this);
	OffsetXPlus1Button   = new Button(ID_OFFSET_X_PLUS_1_BUTTON,   false, CString(">1"),  r, this);
	{ 
		//AutoScaleButton = new CButton();
		CRect rect;
		AutoScaleButton.Create("Autosc.",
			BS_CHECKBOX , //| WS_VISIBLE | WS_CHILD | WS_BORDER |BS_OWNERDRAW,
//			BS_AUTOCHECKBOX | WS_VISIBLE | WS_CHILD | WS_BORDER |BS_OWNERDRAW,
//				BS_CHECKBOX | WS_VISIBLE | WS_CHILD | WS_BORDER |BS_OWNERDRAW,
				rect, this, ID_AUTOSCALE_BUTTON);
		AutoScaleButton.SetCheck(pDoc->dat.p.AutoScale);
		AutoScaleButton.ShowWindow(SW_SHOW);

		MinSlider.Create(TBS_NOTICKS | TBS_VERT  ,rect, this, ID_MIN_SLIDERCTRL);
		MinSlider.ShowWindow(SW_SHOW);
		MinSlider.SetRange(0, 100);
		MinSlider.SetPos(100);

		MaxSlider.Create(TBS_NOTICKS | TBS_VERT ,rect, this, ID_MAX_SLIDERCTRL);
		MaxSlider.ShowWindow(SW_SHOW);
		MaxSlider.SetRange(0, 100);
		MaxSlider.SetPos(0);

	}
	OffsetXMinus10Button = new Button(ID_OFFSET_X_MINUS_10_BUTTON, false, CString("<10"), r, this);
	OffsetXMinus1Button  = new Button(ID_OFFSET_X_MINUS_1_BUTTON,  false, CString("<1"),  r, this);

	OffsetYPlus10Button  = new Button(ID_OFFSET_Y_PLUS_10_BUTTON,  false, CString("v10"), r, this);
	OffsetYPlus1Button   = new Button(ID_OFFSET_Y_PLUS_1_BUTTON,   false, CString("v1"),  r, this);
	OffsetYMinus10Button = new Button(ID_OFFSET_Y_MINUS_10_BUTTON, false, CString("^10"), r, this);
	OffsetYMinus1Button  = new Button(ID_OFFSET_Y_MINUS_1_BUTTON,  false, CString("^1"),  r, this);

	OffsetxField = new Text(ID_OFFSET_X_TEXT, 0, -20000, 20000, false, r, this);
	OffsetyField = new Text(ID_OFFSET_Y_TEXT, 0, -20000, 20000, false, r, this);

//	OriginDCxField = new Text(ID_OFFSET_X_PDCORIGIN, 0, -20000, 20000, FALSE, r, this);
//	OriginDCyField = new Text(ID_OFFSET_Y_PDCORIGIN, 0, -20000, 20000, FALSE, r, this);
	OriginDCxField = new Text(ID_OFFSET_X_PDCORIGIN, 0, -20000, 20000, true, r, this);
	OriginDCyField = new Text(ID_OFFSET_Y_PDCORIGIN, 0, -20000, 20000, true, r, this);

	TextOriginDCxField = new Text(ID_OFFSET_TEXT_X_PDCORIGIN, "Im Off x", false, r, this);
	TextOriginDCyField = new Text(ID_OFFSET_TEXT_Y_PDCORIGIN, "Im Off y", false, r, this);

	MinScaleField = new Text(ID_MINSCALE_FIELD, 0, -2140000000, 2140000000, true, r, this);
	MinScaleTextField  = new Text(ID_MINSCALE_TEXT_FIELD, "Min", false, r, this);
	MaxScaleField = new Text(ID_MAXSCALE_FIELD, 0, -2140000000, 2140000000, true, r, this);
	MaxScaleTextField  = new Text(ID_MAXSCALE_TEXT_FIELD, "Max", false, r, this);

	TrackThisButton = new Button(ID_TRACK_THIS_BUTTON, false, CString("Track this"), r, this);

//	CopyGeneralSettingsToThisFileButton =
//		new Button(ID_COPYGENERALTOTHIS_BUTTON, FALSE, CString("Copy General settings to This"), r, this);
//	CopyThisSettingsToGeneralButton=
//		new Button(ID_COPYTHISTOGENERAL_BUTTON, FALSE, CString("Copy This settings to General"), r, this);
	int z=2;
	double realz = 1.0;
	for (int i = 0; i<NZOOMBUTTONS; i++){
	    char s[10];
		_gcvt(realz, 6, s);
		ZoomButtons[i].Create(CString(s), BS_RADIOBUTTON, // | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_MAXIMIZE, 
		r, 	this, ID_ZOOM_BUTTON1+i);
		ZoomButtonsReal[i].ZoomIn = false;
		ZoomButtons[i].parent = this;
		ZoomButtons[i].index=i;
		ZoomButtons[i].ZoomFactor = ZoomTable[i];

		realz /= 2.0;
		ZoomButtons[i].ShowWindow(SW_SHOW);
	}
	for (int i = 0; i<NZOOMBUTTONS; i++){
	    char s[10];
		_itoa(z, s, 10);
//		_itoa(z, s, 10);
		ZoomButtonsReal[i].Create(CString(s), BS_RADIOBUTTON, // | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_MAXIMIZE, 
		r, 	this, ID_ZOOM_BUTTON_REAL1+i);
		ZoomButtonsReal[i].ZoomIn = true;
//		ZoomButtonsReal[i].IsDoubleZoom = true;
		ZoomButtonsReal[i].parent = this;
		ZoomButtonsReal[i].index=i;
		ZoomButtonsReal[i].ZoomFactor = ZoomTable[i+1];
		ZoomButtonsReal[i].ShowWindow(SW_SHOW);
		z *= 2;
	}

}
void Traces::MoveButtons(CDC * pDC){
	CRect r;
	GetClientRect(&r);

	CRect r2 = r;
	r2.right -= 5;
	r2.left = r2.right - 60;
	r2.top = 10;
	r2.bottom = r2.top + 30;

	CRect r234 = r2;
	ResetButton->MoveWindow(&r2);

	r234.right = r234.left-1;
	r234.left = r234.right-60;
	ReloadButton->MoveWindow(&r234);

//	r.left = 645;
//	r.right = r.left + 60;

//	r.top = 10;
//	r.bottom = r.top + 30;

//	ReloadButton->MoveWindow(&r2);

	r2.left -=70;
	r2.right -= (30+30);
	r2.top = r2.bottom+3;
	r2.bottom = r2.top+20;
	ZoomButtons[0].MoveWindow(&r2);

	CRect r2r=r2;
	r2r.left = r2.right+1;
	r2r.right = r2r.left+40;
	ZoomButtonsReal[0].MoveWindow(&r2r);


	r2.top = r2.bottom+2;
	r2.bottom = r2.top+20;
	ZoomButtons[1].MoveWindow(&r2);

	r2r=r2;
	r2r.left = r2.right+1;
	r2r.right = r2r.left+40;
	ZoomButtonsReal[1].MoveWindow(&r2r);

	r2.top = r2.bottom+2;
	r2.bottom = r2.top+20;
	ZoomButtons[2].MoveWindow(&r2);
	r2r=r2;
	r2r.left = r2.right+1;
	r2r.right = r2r.left+40;
	ZoomButtonsReal[2].MoveWindow(&r2r);

	r2.top = r2.bottom+2;
	r2.bottom = r2.top+20;
	ZoomButtons[3].MoveWindow(&r2);
	r2r=r2;
	r2r.left = r2.right+1;
	r2r.right = r2r.left+40;
	ZoomButtonsReal[3].MoveWindow(&r2r);

	for (int i=0; i<NZOOMBUTTONS; i++){
		if ((ZoomIndex==i) && (!ZoomIn)){
			ZoomButtons[i].SetCheck(1);
		}
		else{
			ZoomButtons[i].SetCheck(0);
		}
		if ((ZoomIndex==i) && (ZoomIn)){
			ZoomButtonsReal[i].SetCheck(1);
		}
		else{
			ZoomButtonsReal[i].SetCheck(0);
		}
	}

	r2.top = r2.bottom+2;
	r2.bottom = r2.top+20;
	r2.left = r2.left - 20;
	r2.right = r2.left + 40;

	OffsetXPlus10Button->MoveWindow(&r2);

	CRect r3 = r2;

	r3.left = r2.right + 1;
	r3.right = r3.left +40;
	OffsetXPlus1Button->MoveWindow(&r3);
	
	{
		CRect rectAuto = r3;
		rectAuto.left = r3.right + 1;
		rectAuto.right = rectAuto.left + 70;
		AutoScaleButton.MoveWindow(&rectAuto);
		
		rectAuto.top =rectAuto.bottom + 2;
		rectAuto.bottom = rectAuto.top + 20;
		MinScaleTextField->MoveWindow(&rectAuto);
		rectAuto.top = rectAuto.bottom+1;
		rectAuto.bottom = rectAuto.top + 20;
		MinScaleField->MoveWindow(&rectAuto);

		rectAuto.top =rectAuto.bottom + 1;
		rectAuto.bottom = rectAuto.top + 60;
		MinSlider.MoveWindow(&rectAuto);

		rectAuto.top =rectAuto.bottom + 8;
		rectAuto.bottom = rectAuto.top + 20;
		MaxScaleTextField->MoveWindow(&rectAuto);
		rectAuto.top = rectAuto.bottom+1;
		rectAuto.bottom = rectAuto.top + 20;
		MaxScaleField->MoveWindow(&rectAuto);

		rectAuto.top =rectAuto.bottom + 1;
		rectAuto.bottom = rectAuto.top + 60;
		MaxSlider.MoveWindow(&rectAuto);

	}

	r2.top = r2.bottom+2;
	r2.bottom = r2.top+20;
	r3.top = r3.bottom+2;
	r3.bottom = r3.top+20;
	OffsetXMinus10Button->MoveWindow(&r2);
	OffsetXMinus1Button->MoveWindow(&r3);

	r2.top = r2.bottom+2;
	r2.bottom = r2.top+20;
	r3.top = r3.bottom+2;
	r3.bottom = r3.top+20;

	CRect r4 = r2;
	r4.right = r4.right + 40;
	OffsetxField->MoveWindow(&r4);

	r2.top = r2.bottom+5; 	r2.bottom = r2.top+20;
	r3.top = r3.bottom+5;	r3.bottom = r3.top+20;
	r4.top = r4.bottom+5;	r4.bottom = r4.top+20;


	OffsetYPlus10Button->MoveWindow(&r2);
	OffsetYPlus1Button->MoveWindow(&r3);

	r2.top = r2.bottom+2; 	r2.bottom = r2.top+20;
	r3.top = r3.bottom+2;	r3.bottom = r3.top+20;
	r4.top = r4.bottom+2;	r4.bottom = r4.top+20;

	OffsetYMinus10Button->MoveWindow(&r2);
	OffsetYMinus1Button->MoveWindow(&r3);

	r2.top = r2.bottom+2; 	r2.bottom = r2.top+20;
	r3.top = r3.bottom+2;	r3.bottom = r3.top+20;
	r4.top = r4.bottom+2;	r4.bottom = r4.top+20;
	OffsetyField->MoveWindow(&r4);

	r2.top = r2.bottom+2; 	r2.bottom = r2.top+20;
	r3.top = r3.bottom+2;	r3.bottom = r3.top+20;
	r4.top = r4.bottom+2;	r4.bottom = r4.top+20;

	TrackThisButton->MoveWindow(&r4);

	CRect r5 = r4;

	r5.top = r5.bottom + 10;
	r5.bottom = r5.top + 20;

	TextOriginDCxField->MoveWindow(&r5);
	r5.top = r5.bottom+1;
	r5.bottom = r5.top + 20;
	OriginDCxField->MoveWindow(&r5);

	r5.top = r5.bottom+5;
	r5.bottom = r5.top + 20;
	TextOriginDCyField->MoveWindow(&r5);
	r5.top = r5.bottom+1;
	r5.bottom = r5.top + 20;
	OriginDCyField->MoveWindow(&r5);

}

void Traces::OnDraw(CDC* pDC)
{

	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (pDoc->IsLoadingFile){
		return;
	}

//	if (!BitmapsInitialized){
//		if (!
	if (firsttime){
		{

			FillStarted = false;
			pDC->SetMapMode(MM_TEXT);
			CRect rect;
			GetClientRect(&rect);
			SIZE s;

//			s.cx = 2*rect.right;
//			s.cy = 2*rect.bottom;
			s.cx = rect.right;
			s.cy = rect.bottom;
//			SetScaleToFitSize(s);
			SetScrollSizes(MM_TEXT, s);
		}
		firsttime = false;
		Init();
		CreateButtons();
//		pDoc->ReadFile(pDoc->dat.CurrentIndex);
		switch (pDoc->dat.p.ViewType){
			case 0 : OnViewShowrectangle(); break;
			case 1 : OnViewShowconus(); break;
			default : OnViewShowpolygons(); break;
		}


	}
	MoveButtons(pDC);
	ShiftOriginDC(pDC);
//	pDC->SetViewportOrg(OriginDC.x, OriginDC.y);
	DrawCellTest(pDC, false, NULL);
//	pDC->SetViewportOrg(0, 0);
	ResetOriginDC(pDC);
	int offsetx, offsety;
	pDoc->GetOffset(offsetx, offsety);
	OffsetxField->SetInt(offsetx);
	OffsetyField->SetInt(offsety);

	OriginDCxField->SetInt(this->OriginDC.x);
	OriginDCyField->SetInt(this->OriginDC.y);

	MinScaleField->SetInt(pDoc->dat.p.MinManScale);
	MaxScaleField->SetInt(pDoc->dat.p.MaxManScale);

	CRect rect;
	GetClientRect(&rect);
	int tx = rect.right-200;
	int ty=rect.bottom-50;
	if 	(OnMovingPolyWithArrowKey){
		pDC->TextOut(tx, ty, "Moving Poly - ESC to stop");
	}
	if (OnMovingLineWithArrowKey){
		pDC->TextOut(tx, ty, "Moving Line - ESC to stop");
	}
	if (IsCounting){
		pDC->TextOut(tx, ty, "Counting - ESC to stop");
	}

	if (OnCreatingPolygon){
		pDC->TextOut(tx, ty, "OnCreatingPolygon  - ESC to stop");
	}
	if (OnCreatingLine){
		pDC->TextOut(tx, ty, "OnCreatingLine  - ESC to stop");
	}
	if (SelectingFillPoints){
		pDC->TextOut(tx, ty, "SelectingFillPoints  - ESC to stop");
	}
	if (SelectingFillPointsInSelectedPolygon){
		pDC->TextOut(tx, ty, "SelectingFillPointsInSelectedPolygon  - ESC to stop");
	}


	int NCounts = Counts.size();
	if (NCounts>0){
		//int tx = rect.right-200;
		ty -= 20;
		char s[20];
		_itoa(NCounts, s, 10);
		pDC->TextOut(tx, ty, "Counts: " + CString(s));
//		int offsetx, offsety;
//		GetOffset(offsetx, offsety);
//		_itoa(this->OriginDC.x, s, 10);
//		pDC->TextOut(tx, ty-20, "ox: " + CString(s));

		for (int i=0; i<NCounts; i++){
			ObjectPoint op;
			op.SetPos(Counts[i].x+this->OriginDC.x, Counts[i].y+this->OriginDC.y);
			CPoint dp = op.GetPos(Zoom,  ZoomIn);
			int sizecircle = DivZoom(frame->SizeCountCircle, Zoom, ZoomIn);

			int x1=dp.x-sizecircle ;
			int x2=x1+2*sizecircle ;
			int y1=dp.y-sizecircle ;
			int y2=y1+2*sizecircle ;
			pDC->Ellipse(x1, y1, x2, y2);
		}
	}



/*	bool OnCreatingPolygon;
	bool OnMovePolygon;
	bool OnMoveLine;
	bool OnCreatingLine;
*/
}

void Traces::DrawCell(CDC* pDC, bool Export, CFile * fp){

	//Alert0("hi");
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	bool releasedc = false;

	if (!pDC){
		releasedc = true;
		pDC = GetDC();
	}


	CWaitCursor dummy;

	// TODO: add draw code here

	CDC * mDC = new CDC;
	mDC->CreateCompatibleDC(pDC);
	if (!mDC){
		Alert("could not create compatible DC");
		return;
	}

	CRect r;
	GetClientRect(&r);
	AdjustRectOrigin(r);

	CBitmap * pcBM = pDC->GetCurrentBitmap();
	BITMAP PBM;//, OBM;
	pcBM->GetBitmap(&PBM);
	PBM.bmWidth = r.right;
	PBM.bmHeight = r.bottom;


//	OBM.bmWidth = r.right;
//	OBM.bmHeight = r.bottom;
	CBitmap CBM;
	CBM.CreateBitmap(r.right, r.bottom, 1, GetDeviceCaps(pDC->m_hDC, BITSPIXEL), NULL);

	mDC->SelectObject(&CBM);
	pDC->SelectObject(pDC->GetCurrentBitmap());

	if (!mDC->BitBlt(0, 0, r.right-1, r.bottom-1, pDC, 0,0,SRCCOPY)){
		Alert("could not copy");
	}

//	pDoc->dat.p.drawob()->DrawObject(pDC, r);

	CRect rect;

	GetClientRect(&rect);
	this->AdjustRectOrigin(rect);

	__int32 y; 
	__int32 x; 
	double Min = 0;
	double Max = 0;
	double dmax = 1;
	COLORREF * pref ;

	int MaxBit;

	if (pDoc->dat.p.NBitsColorTable == 8){
		pref = pDoc->dat.p.ColorTable256;
//		pDoc->dat.p.MakeGreyScale();
//		MaxBit = 255;
		MaxBit = 254;

//		Alert0("255");
	}
	else{
//		Alert0("65534");
		pref = pDoc->dat.p.ColorTable65536;
//		MaxBit = 65535;
		MaxBit = 65534;
	}

//	pref = &(GreyColorTable256[0]);

//	ShowFloat(MaxBit, "MaxBit");
//	double ** im = pDoc->currentimage;
	ImageDataType ** im = pDoc->id.data;
	ImageDataType ** ref = pDoc->id.meanrefdata;

	bool Subtract = pDoc->dat.p.SubtractInitialValuesForDraw == true;
	double scaleimage = pDoc->dat.p.MultiplierImages;
	double offsetimage = pDoc->dat.p.OffsetImages;

	if (!im){
//		Alert0("!im");
		goto enddrawOld;
	}

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		if (!pDoc->id.refcalculated){
			pDoc->CalculateRef();
		}
		else{
//			Alert("REF CALC");
		}
	}


	Min = im[0][0];
	Max = im[0][0];

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		Min -= ref[0][0];
		Max -= ref[0][0];
	}

	if (pDoc->dat.p.AutoScale){
		for (y=0; y<pDoc->id.ysize; y++){
			for (x=0; x<pDoc->id.xsize; x++){
				int d = int(im[x][y]);
				if (pDoc->dat.p.SubtractInitialValuesForDraw){
					d -= int(ref[x][y]);
				}
				if (d<Min) Min=d;
				if (d>Max) Max = d;
			}
		}
	}
	else{
		Min = pDoc->dat.p.MinManScale;
		Max = pDoc->dat.p.MaxManScale;
	}
//	ShowFloat(Max, "Max");

//	dmax = double(MaxBit)/(Max-Min+0.0000000000000001);
	dmax = double(MaxBit)/(Max-Min+0.0000000001);
//	dmax = 1.677e7/(Max-Min+0.0000000000000001); // 2^24 
//Alert0("hi");
	int YSIZE = pDoc->id.ysize;
	if ((YSIZE>MultZoom(rect.bottom, Zoom, ZoomIn)) && (!Export)){
		YSIZE = MultZoom(rect.bottom, Zoom, ZoomIn);
	}
	int XSIZE = pDoc->id.xsize;
	if ((XSIZE>MultZoom(rect.right, Zoom, ZoomIn)) && (!Export)){
		XSIZE = MultZoom(rect.right, Zoom, ZoomIn);
	}

//	{
		CBitmap * mBitmap = mDC->GetCurrentBitmap();

//		int * buffer = new int[r.right*r.bottom];

//		BitMapBuffer.UpdateBuffer(r.right*r.bottom);
		UpdateBitmapBuffer(r.right*r.bottom);

//		int * buffer = (int *) BitMapBuffer.buff;
		int * buffer = MyBitMapBuffer;
//		for (int i=0; i<BitMapBuffer.GetSize(); i++){
//			buffer[i]=0;
//		}

		DWORD Copied = mBitmap->GetBitmapBits(r.right*r.bottom*4, buffer);

//		ShowFloat(4*r.right*r.bottom, "4*r.right*r.bottom");
//		ShowFloat(int(Copied), "Copied");

		BITMAP BitmapStructure;

		mBitmap->GetBitmap(&BitmapStructure);


//		ShowFloat(BitmapStructure.bmType, "type");
//		ShowFloat(BitmapStructure.bmWidthBytes, "bmWidthBytes");
//		ShowFloat(BitmapStructure.bmPlanes, "bmPlanes");
//		ShowFloat(BitmapStructure.bmBitsPixel, "bmBitsPixel");
//		ShowFloat(BitmapStructure.bmBits, "bmBits");
/*
bmWidthBytes
    The number of bytes in each scan line. This value must be divisible by 2, because the system assumes that the bit values of a bitmap form an array that is word aligned.
bmPlanes
    The count of color planes.
bmBitsPixel
    The number of bits required to indicate the color of a pixel.
bmBits
    A pointer to the location of the bit values for the bitmap. The bmBits member must be a pointer to an array of character 
*/

//		ShowFloat(Zoom, "Zoom");
	int yy=0;
	for (y=0; y<YSIZE; y+=Zoom){
		int xx=0;
		int offset = yy*BitmapStructure.bmWidth;
		int offsetfine = y*XSIZE;
		for (x=0; x<XSIZE; x+=Zoom){

//			double f= (im[x][y]-Min)*dmax;
			double value = im[x][y];
			
			if (Subtract){
				value -= ref[x][y];
			}
			value = value*scaleimage + offsetimage;
			if (Export){
				int ivalue = int(value);
				char s[20];
				_itoa(ivalue,s, 10);
				CString str = CString(s)+"\t";
				fp->Write(str.GetBuffer(str.GetLength()), str.GetLength());
			}

			__int64 intf = __int64 ((value-Min)*dmax);
//			int intf = int ((value-Min)*dmax);
			if (intf<0){
				intf = 0;
			}
			if (intf > MaxBit){
//				Alert0(">");
				intf = MaxBit;
			}
//			mDC->SetPixel(xx, yy, pref[intf]); 
/*
COLORREF A = pref[intf];
			int red = GetRValue(A);
			int green = GetGValue(A);
			int blue = GetBValue(A);
			int col = blue + 256*(green + 256*red);

//			buffer[xx+yy*BitmapStructure.bmWidth]=col;
			buffer[xx+offset]=col;
*/
			buffer[xx+offset]=pref[intf];
			if (SelectedInitialized && ShowFill){
//			if (FillStarted){
				bool PointIsSelected = Selected[x+offsetfine];
				if (PointIsSelected){
					buffer[xx+offset] = RGB(0,255,0);//pref[0]; //RGB(0,255,0);
				}
			}
//			pDC->SetPixel(xx, yy, pref[intf]); 
//			mDC->SetPixel(xx, yy, 0); 


//			Bits[ xx*4 + 4*yy*BitmapStructure.bmWidth] = 0xFF;

//			pData[ (y * bitmap_width * 4) + (x * 4) + 0 ] = 0x00;  // blue component  *
//pData[ (y * bitmap_width * 4) + (x * 4) + 1 ] = 0xFF;  // green component
//pData[ (y * bitmap_width * 4) + (x * 4) + 2 ] = 0x00;  // red component  

//Bits[xx+yy*BitmapStructure.bmWidth] = pref[intf];

			xx++;
		}
		if (Export){
			if (y<(pDoc->id.ysize-1)){
				CString str=CString("\r\n");
				fp->Write(str.GetBuffer(str.GetLength()), str.GetLength());
			}
		}
		yy++;

	}
	mBitmap->SetBitmapBits(r.right*r.bottom*4, buffer);
//	delete [] buffer;


	mDC->SelectObject(mDC->GetCurrentBitmap());
	pDC->SelectObject(pDC->GetCurrentBitmap());


	if (!pDC->BitBlt(0, 0, r.right-1, r.bottom-1, mDC, 0, 0, SRCCOPY)){
//	if (!pDC->BitBlt(1, 1, XSIZE, YSIZE, mDC, 0, 0, SRCCOPY)){
//	if (!pDC->BitBlt(-200, -200, r.right-1, r.bottom-1, mDC, 0, 0, SRCCOPY)){
		Alert("could not retrieve");
//		return false;
	}
	else{
//		Alert0("Hi");
	}


enddrawOld:
	if (pDoc->dat.p.ViewType<2){
		pDoc->dat.p.drawob()->DrawObject(pDC, rect, 3, pDoc->dat.p.curspos, Zoom, ZoomIn);
	}
	if (pDoc->dat.p.ViewType==2){
/*
char s[10];
		_itoa(pDoc->dat.polys.NPoly(), s, 10);
		pDC->TextOutA(10,10, CString("NPOLY ")+CString(s));
		_itoa(pDoc->dat.Version1, s, 10);
		pDC->TextOutA(10,30, CString("Version ")+CString(s));
*/
		int offsetx, offsety;
		GetOffset(offsetx, offsety);

		pDoc->dat.polys.DrawPolys(pDC, selectedpolygon, Zoom, offsetx, offsety, ZoomIn);
	} 

	ReleaseDC(mDC);
	delete mDC;

	if (releasedc){
		ReleaseDC(pDC);
	}

}

void Traces::DrawCellBad(CDC* pDC, bool Export, CFile * fp){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	bool releasedc = false;

	if (!pDC){
		releasedc = true;
		pDC = GetDC();
	}

	CRect rect;
	GetClientRect(&rect);

//	return;

	CWaitCursor dummy;

	// TODO: add draw code here

	__int32 y; 
	__int32 x; 
	double Min = 0;
	double Max = 0;
	double dmax = 1;
	COLORREF * pref ;

	int MaxBit;

	if (pDoc->dat.p.NBitsColorTable == 8){
		pref = pDoc->dat.p.ColorTable256;
		MaxBit = 255;
	}
	else{
		pref = pDoc->dat.p.ColorTable65536;
		MaxBit = 65535;
	}

//	double ** im = pDoc->currentimage;
	ImageDataType ** im = pDoc->id.data;
	ImageDataType ** ref = pDoc->id.meanrefdata;

	bool Subtract = pDoc->dat.p.SubtractInitialValuesForDraw == true;
	double scaleimage = pDoc->dat.p.MultiplierImages;
	double offsetimage = pDoc->dat.p.OffsetImages;

	if (!im){
		goto enddraw;
	}

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		if (!pDoc->id.refcalculated){
			pDoc->CalculateRef();
		}
		else{
//			Alert("REF CALC");
		}
	}

	Min = im[0][0];
	Max = im[0][0];

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		Min -= ref[0][0];
		Max -= ref[0][0];
	}

	if (pDoc->dat.p.AutoScale){
//		for (y=0; y<pDoc->dat.p.ysizeimage; y++){
//			for (x=0; x<pDoc->dat.p.xsizeimage; x++){
//		ShowFloat(pDoc->id.ysize, "pDoc->id.ysize");
		for (y=0; y<pDoc->id.ysize; y++){
			for (x=0; x<pDoc->id.xsize; x++){
				int d = int(im[x][y]);
				if (pDoc->dat.p.SubtractInitialValuesForDraw){
					d -= int(ref[x][y]);
				}
				if (d<Min) Min=d;
				if (d>Max) Max = d;
			}
		}
	}
	else{
		Min = pDoc->dat.p.MinManScale;
		Max = pDoc->dat.p.MaxManScale;
	}

	dmax = double(MaxBit)/(Max-Min+0.0000000000000001);
//	dmax = 1.677e7/(Max-Min+0.0000000000000001); // 2^24 

//	for (y=0; y<pDoc->dat.p.ysizeimage; y++){
//		for (x=0; x<pDoc->dat.p.xsizeimage; x++){

	for (y=0; y<pDoc->id.ysize; y++){
		for (x=0; x<pDoc->id.xsize; x++){

//			double f= (im[x][y]-Min)*dmax;
			double value = im[x][y];
			if (Subtract){
				value -= ref[x][y];
			}
			value = value*scaleimage + offsetimage;
			if (Export){
				int ivalue = int(value);
				char s[20];
				_itoa(ivalue,s, 10);
				CString str = CString(s)+"\t";
				fp->Write(str.GetBuffer(str.GetLength()), str.GetLength());
			}

			int intf = int ((value-Min)*dmax);
			if (intf<0){
				intf = 0;
			}
			if (intf > MaxBit){
				intf = MaxBit;
			}
			pDC->SetPixel(x, y, pref[intf]); 
		}
		if (Export){
			if (y<(pDoc->id.ysize-1)){
				CString str=CString("\r\n");
				fp->Write(str.GetBuffer(str.GetLength()), str.GetLength());
			}
		}

	}



enddraw:
	if (pDoc->dat.p.ViewType<2){
		pDoc->dat.p.drawob()->DrawObject(pDC, rect, 3, pDoc->dat.p.curspos, Zoom, ZoomIn);
	}
	if (pDoc->dat.p.ViewType==2){

		int offsetx, offsety;
		GetOffset(offsetx, offsety);
		pDoc->dat.polys.DrawPolys(pDC, selectedpolygon, Zoom, offsetx, offsety, ZoomIn);
	}


	if (releasedc){
		ReleaseDC(pDC);
	}

}

/////////////////////////////////////////////////////////////////////////////
// Traces diagnostics

#ifdef _DEBUG
void Traces::AssertValid() const
{
	CView::AssertValid();
}

void Traces::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Traces message handlers

void Traces::CorrectClickedPointForOrigin(CPoint & point){
	point.x -= DivZoom(OriginDC.x, Zoom, ZoomIn);
	point.y -= DivZoom(OriginDC.y, Zoom, ZoomIn);
}
void Traces::UnCorrectClickedPointForOrigin(CPoint & point){
	point.x += DivZoom(OriginDC.x, Zoom, ZoomIn);
	point.y += DivZoom(OriginDC.y, Zoom, ZoomIn);
}
void Traces::AdjustRectOrigin(CRect & r){
	r.right -= DivZoom(OriginDC.x, Zoom, ZoomIn);
	r.bottom -= DivZoom(OriginDC.y, Zoom, ZoomIn);
}

void Traces::ShiftOriginDC(CDC * pDC){
//	CSize OriginalSize = pDC->GetViewportExt();
//	CSize mysize = OriginalSize;
//	ShowFloat(mysize.cx, "cx");
	char s[20];
	//_itoa(DivZoom(OriginDC.x, Zoom, ZoomIn), s, 10);
	//pDC->TextOutA(900, 200, CString(s));
//	ShowFloat(DivZoom(OriginDC.x, Zoom, ZoomIn), "DivZoom(OriginDC.x, Zoom, ZoomIn)");
	pDC->SetViewportOrg(DivZoom(OriginDC.x, Zoom, ZoomIn), DivZoom(OriginDC.y, Zoom, ZoomIn));
//	pDC->SetWindowOrg(-OriginDC.x, -OriginDC.y);
//	pDC->SetViewportExt(mysize.cx-OriginDC.x, mysize.cy-OriginDC.y);
//	pDC->SetViewportExt(2000, 2000);
//	CSize NewSize = pDC->GetViewportExt();
//	ShowFloat(NewSize.cx, "cxnew");
}

void Traces::ResetOriginDC(CDC * pDC){
	pDC->SetViewportOrg(0, 0);
}

void Traces::OnLButtonControl(CPoint point) {
	if (ZoomIn){
		if (ZoomIndex == 3){
			//Alert0("Max");
			return;
		}
		OriginDC.x = OriginDC.x - int (double(point.x)/2.0/double(Zoom));
		OriginDC.y = OriginDC.y - int (double(point.y)/2.0/double(Zoom));
		ZoomIndex++;
		Zoom *=2;
		MyInvalidate();
		return;
	}
	if (ZoomIndex == 0){ // Zoom out
		OriginDC.x = OriginDC.x - int (double(point.x)/2.0/double(Zoom));
		OriginDC.y = OriginDC.y - int (double(point.y)/2.0/double(Zoom));
		ZoomIndex = 0;
		ZoomIn = true;
		Zoom = 2;
		MyInvalidate();
		return;
	}
	else{
		OriginDC.x = OriginDC.x - int (double(point.x)/2.0*double(Zoom));
		OriginDC.y = OriginDC.y - int (double(point.y)/2.0*double(Zoom));
		ZoomIndex -= 1;
//		ZoomIn = true;
		Zoom /= 2;
		MyInvalidate();
		return;
	}

}
void Traces::OnLButtonShift(CPoint point) {
	if (ZoomIn){
		if (ZoomIndex >0){
			//Alert0("Max");
			OriginDC.x = OriginDC.x + int (double(point.x)*2.0/double(Zoom));
			OriginDC.y = OriginDC.y + int (double(point.y)*2.0/double(Zoom));
			ZoomIndex--;
			Zoom /=2;
			MyInvalidate();
			return;
		}
		else{ // Zoom = 2
			OriginDC.x = OriginDC.x + int (double(point.x)*2.0/double(Zoom));
			OriginDC.y = OriginDC.y + int (double(point.y)*2.0/double(Zoom));
			ZoomIndex = 0;
			Zoom /=2;
			ZoomIn = false;
			MyInvalidate();
			return;
		}
	}
	if (ZoomIndex >=3 ) return;

	OriginDC.x = OriginDC.x + int (double(point.x)/2.0*double(Zoom));
	OriginDC.y = OriginDC.y + int (double(point.y)/2.0*double(Zoom));
	ZoomIndex++;
	Zoom *=2;
	MyInvalidate();

}

void Traces::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (firsttime){
		return;
	}
	if (OnMovingPolyWithArrowKey){
		return;
	}
	if (OnMovingLineWithArrowKey){
		return;
	}
	if (nFlags & MK_CONTROL){
		OnLButtonControl(point); 
		return;
	}
	if (nFlags & MK_SHIFT){
		OnLButtonShift(point); 
		return;
	}

	// TODO: Add your message handler code here and/or call default
	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);

	NewPoint.x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	NewPoint.y = MultZoom(NewPoint.y, Zoom, ZoomIn);

	if (IsCounting){
		AddPointToCount(NewPoint);
	}

	if (OnCreatingPolygon){
		//Beep(500,20);
		AddPointToPolygonOrLine(nFlags, NewPoint, false);
		return;
	}
	if (OnCreatingLine){
		//Beep(500,20);
		AddPointToPolygonOrLine(nFlags, NewPoint, true);
		return;
	}

	if (SelectingFillPoints){
		AddFillArea(NewPoint);
		return;
	}
	if (SelectingFillPointsInSelectedPolygon){
		AddFillArea(NewPoint, true);
		return;
	}
	
	if (pDoc->dat.p.ViewType == 2){
		int offsetx, offsety;
		GetOffset(offsetx, offsety);
		NewPoint.x -= offsetx;
		NewPoint.y -= offsety;

		int result = pDoc->dat.polys.ClickedOnAPoint(NewPoint, cornerclicked);
		if (result >= 0){
//			Beep(1000,20);
			int olds=selectedpolygon;
			selectedpolygon = result;
			OnMovePolygon = true;
			return;
		}
		result = pDoc->dat.lines.ClickedOnAPoint(NewPoint, cornerclicked);
		if (result >= 0){
//			Beep(1000,20);
			int olds=selectedline;
			selectedline = result;
			OnMoveLine = true;
			return;
		}



		//Alert0("shift");
		OnShiftingOrigin = true;
		StartPointShiftingOrigin = point;
		return;
	}

	indexselected = pDoc->dat.p.drawob()->IsClicked(NewPoint);
//		ShowFloat(indexselected, "index");
//		return;

	if ( indexselected>=0){
		AreaObject * drawob = pDoc->dat.p.drawob();
		MouseIsDown = true;
		point = drawob->P[indexselected].GetPos(1,1);
		int x=point.x;
		int y=point.y; 
		CRect rect;
		GetWindowRect(&rect);

		
//		BOOL result = SetCursorPos(rect.left+x, rect.top+y);

		switch (SelectMode){
			case 0 :  break;
			case 1 : drawob->StartSize(indexselected); break;
			case 2 : drawob->StartRotate(indexselected); break;
			default : Alert0("ERROR IN TRACES::ONLBUTTONDOWN"); break;
		}
/*
		switch (SelectMode){
			case 0 : OnLButtonDownMove( nFlags, point); break;
			case 1 : OnLButtonDownSize( nFlags, point); break;
			case 2 : OnLButtonDownRotate( nFlags, point); break;
			default  : OnLButtonDownMove( nFlags, point); break;
		};
*/
	}
	else{

		MouseIsDown = false;
	}

	CView::OnLButtonDown(nFlags, point);
}

void Traces::OnLButtonDownMove( UINT nFlags, CPoint point) {
	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);

	NewPoint.x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	NewPoint.y = MultZoom(NewPoint.y, Zoom, ZoomIn);
	pDoc->dat.p.drawob()->MovePoint( indexselected, NewPoint);
	CDC * pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);

//	ShiftOriginDC(pDC);
	pDoc->dat.p.drawob()->DrawObject(pDC, rect, Zoom, ZoomIn);
//	ResetOriginDC(pDC);
	ReleaseDC(pDC);
//	MouseIsDown = FALSE;
}

void Traces::OnLButtonDownSize( UINT nFlags, CPoint point) {
	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);

//	NewPoint.x *= Zoom;
//	NewPoint.y *= Zoom;
	NewPoint.x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	NewPoint.y = MultZoom(NewPoint.y, Zoom, ZoomIn);
	pDoc->dat.p.drawob()->SizePoint( indexselected, NewPoint);
	CDC * pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	ShiftOriginDC(pDC);
	pDoc->dat.p.drawob()->DrawObject(pDC, rect, Zoom, ZoomIn);
	ResetOriginDC(pDC);
	ReleaseDC(pDC);
}
void Traces::OnLButtonDownRotate(UINT nFlags, CPoint point) {
	CDC * pDC = GetDC();
	CRect rect;

	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);
	NewPoint.x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	NewPoint.y = MultZoom(NewPoint.y, Zoom, ZoomIn);

	pDoc->dat.p.drawob()->RotatePoint(indexselected, NewPoint);

	GetClientRect(&rect);
	ShiftOriginDC(pDC);
	pDoc->dat.p.drawob()->DrawObject(pDC, rect, Zoom, ZoomIn);
	ReleaseDC(pDC);
	ResetOriginDC(pDC);

	GetWindowRect(&rect);


	int x=pDoc->dat.p.drawob()->P[indexselected].Getx(Zoom, ZoomIn);
	int y=pDoc->dat.p.drawob()->P[indexselected].Gety(Zoom, ZoomIn);
//		y=pDoc->dat.p.drawob()->P[indexselected].Gety()/Zoom;
	if (pDoc->dat.p.drawob()->FixMouse()){
		BOOL result = SetCursorPos(rect.left+x, rect.top+y);
	}

}
void Traces::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (firsttime){
		return;
	}
	if (OnMovePolygon){
		OnMovePolygon = false;
		MyInvalidate();
		return;
	}
	if (OnMoveLine){
		OnMoveLine = false;
		MyInvalidate();
		return;
	}
	if (OnShiftingOrigin){
		OnShiftingOrigin = false;
		MyInvalidate();
		return;
	}

	if (!MouseIsDown){
		return;
	}
	MouseIsDown = false;
	pDoc->dat.p.drawob()->SetChanged();
//	frame->p = pDoc->dat.p;

	MyInvalidate();

	CView::OnLButtonUp(nFlags, point);
}
void Traces::MovePolygon(UINT nFlags, CPoint point){
	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);
	NewPoint.x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	NewPoint.y = MultZoom(NewPoint.y, Zoom, ZoomIn);

//	NewPoint.x *= Zoom;
//	NewPoint.y *= Zoom;
	CDC * pDC = GetDC();
	int offsetx, offsety;
	GetOffset(offsetx, offsety);
	
	NewPoint.x -= offsetx;
	NewPoint.y -= offsety;

	ShiftOriginDC(pDC);
	pDoc->dat.polys.MovePolygon(selectedpolygon, cornerclicked, NewPoint, pDC, Zoom, offsetx, offsety, ZoomIn);
	ReleaseDC(pDC);

}
void Traces::MoveLine(UINT nFlags, CPoint point){
	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);
	NewPoint.x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	NewPoint.y = MultZoom(NewPoint.y, Zoom, ZoomIn);

//	NewPoint.x *= Zoom;
//	NewPoint.y *= Zoom;
	CDC * pDC = GetDC();
	int offsetx, offsety;
	GetOffset(offsetx, offsety);
	
	NewPoint.x -= offsetx;
	NewPoint.y -= offsety;

	ShiftOriginDC(pDC);
	pDoc->dat.lines.MovePolygon(selectedline, cornerclicked, NewPoint, pDC, Zoom, offsetx, offsety, ZoomIn);
	ReleaseDC(pDC);

}

void Traces::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (firsttime){
		return;
	}
	if (OnMovePolygon){
		MovePolygon(nFlags, point);
		return;
	}
	if (OnMoveLine){
		MoveLine(nFlags, point);
		return;
	}
	if (OnShiftingOrigin){
//		int dx = (point.x - StartPointShiftingOrigin.x)*Zoom;
		int dx = MultZoom((point.x - StartPointShiftingOrigin.x), Zoom, ZoomIn);
		int NewOrigx = this->OriginDC.x + dx;
		if (NewOrigx>0){
			NewOrigx=0;
//			return;
		}
		int XSIZE = pDoc->id.xsize;
		if (NewOrigx<(-XSIZE)){
			NewOrigx = (-XSIZE);
//			return;
		}
//		int dy = (point.y - StartPointShiftingOrigin.y)*Zoom;
		int dy = MultZoom((point.y - StartPointShiftingOrigin.y), Zoom, ZoomIn);
		int NewOrigy = this->OriginDC.y + dy;
		if (NewOrigy>0){
			NewOrigy=0;
//			return;
		}
		int YSIZE = pDoc->id.ysize;
		if (NewOrigy<(-YSIZE)){
			NewOrigy = -YSIZE;
//			return;
		}
		this->OriginDC.x = NewOrigx;
		this->OriginDC.y = NewOrigy;
		StartPointShiftingOrigin = point;
		MyInvalidate();
		return;
//		Beep(1000, 100);

	}
	if (this->MouseIsDown){
		switch (SelectMode){
				case 0 : OnLButtonDownMove( nFlags, point); break;
				case 1 : OnLButtonDownSize( nFlags, point); break;
				case 2 : OnLButtonDownRotate( nFlags, point); break;
				default  : OnLButtonDownMove( nFlags, point); break;
		};
	}

	
	CView::OnMouseMove(nFlags, point);
}


void Traces::OnEditUndo(){
	pDoc->dat.p.drawob()->Undo();

	MyInvalidate();
}
void Traces::OnEditRedo(){
	pDoc->dat.p.drawob()->Redo();
	MyInvalidate();
}

void Traces::OnViewSetmodemove() {
	SelectMode = 0;
}

void Traces::OnViewSetmodesize() {
	SelectMode = 1;
}

void Traces::OnViewSetmoderotate() {
	SelectMode = 2;
}


void Traces::OnViewShowrectangle(){
	pDoc->dat.p.ViewType = 0;
//	pDoc->dat.p.drawob() = &(pDoc->dat.p.RectOb);
    CMainFrame * f = (CMainFrame *)(AfxGetMainWnd());
	CMenu * pmenu = f->GetMenu();
	DoCheckMenuItem(false, pmenu, ID_VIEW_SHOWPOLYGONS);
	DoCheckMenuItem(false, pmenu, ID_VIEW_SHOWCONUS);
	DoCheckMenuItem(true, pmenu, ID_VIEW_SHOWRECTANGLE);
	InitGenerator = &(InitRectGenerator);
	MyInvalidate();
}
void Traces::OnViewShowpolygons(){
	pDoc->dat.p.ViewType = 2;
    CMainFrame * f = (CMainFrame *)(AfxGetMainWnd());
	CMenu * pmenu = f->GetMenu();
	DoCheckMenuItem(true, pmenu, ID_VIEW_SHOWPOLYGONS);
	DoCheckMenuItem(false, pmenu, ID_VIEW_SHOWCONUS);
	DoCheckMenuItem(false, pmenu, ID_VIEW_SHOWRECTANGLE);
	MyInvalidate();
}


void Traces::OnViewShowconus(){
	pDoc->dat.p.ViewType = 1;
//	pDoc->dat.p.drawob() = &(pDoc->dat.p.ConusOb);
	InitGenerator = &(InitConusGenerator);

	CMainFrame * f = (CMainFrame *)(AfxGetMainWnd());
	CMenu * pmenu = f->GetMenu();
	DoCheckMenuItem(true, pmenu, ID_VIEW_SHOWCONUS);
	DoCheckMenuItem(false, pmenu, ID_VIEW_SHOWPOLYGONS);
	DoCheckMenuItem(false, pmenu, ID_VIEW_SHOWRECTANGLE);

	MyInvalidate();

}

double Traces::GetMeanInObjectArea(){
	double mean = 0;
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

//	if (!pDoc->currentimage){
	if (!pDoc->id.data){
//		Alert("no image");
		return mean;
	}

	AreaObject * drawob = pDoc->dat.p.drawob();

	CPoint p;

	int index;

	bool Initialize = true;

//	int XX= pDoc->dat.p.xsizeimage;
//	int YY= pDoc->dat.p.ysizeimage;

//	double ** im = pDoc->currentimage;
	ImageDataType ** im = pDoc->id.data;

	if (!im) return mean;

	int N=0;

	while (pDoc->dat.p.drawob()->GeneratePoints(Initialize, p, index)){
		mean += double(im[p.x][p.y]);
		N ++;
		
	}
	if (N>0){
		mean /= double(N);
	}
	return mean;
}
void Traces::OnRbuttonDownPolygons(UINT nFlags, CPoint point){
	CPoint NewPoint1 = point;
	CorrectClickedPointForOrigin(NewPoint1);

	NewPoint1.x = MultZoom(NewPoint1.x, Zoom, ZoomIn);
	NewPoint1.y = MultZoom(NewPoint1.y, Zoom, ZoomIn);
//	NewPoint1.x *= Zoom;
//	NewPoint1.y *= Zoom;

	int offsetx, offsety;
	GetOffset(offsetx, offsety);

	CPoint NewPoint = NewPoint1;
	
	NewPoint.x -= offsetx;
	NewPoint.y -= offsety;

	CDC * pDC = GetDC();
	ShiftOriginDC(pDC);
	char s[20];
	CString c;

	pDC->TextOut(300, 20, "                                             ");

	ImageDataType ** im = pDoc->id.data;
	if (im){
		if ( (NewPoint.x<pDoc->id.xsize) & (NewPoint.y<pDoc->id.ysize) ){
			double v=im[NewPoint.x][NewPoint.y];
			_gcvt(v, 5, s);
			c += CString("v = ")+CString(s);
		}
		int offsetx, offsety;
		GetOffset(offsetx, offsety);

		double v=pDoc->dat.polys.CalcMeanValue(0, selectedpolygon,im,pDoc->dat.p.xsizeimage,
												pDoc->dat.p.ysizeimage, offsetx, offsety,
												0);
		_gcvt(v, 5, s);
		c += CString("mean = ")+CString(s);
	}

	CPoint px = CPoint(300,20);
//	this->UnCorrectClickedPointForOrigin(px);
	this->CorrectClickedPointForOrigin(px);
//	pDC->TextOut(300, 20, c);
	pDC->TextOut(px.x, px.y, c);


	if (pDoc->dat.polys.IsInside(NewPoint, selectedpolygon)){
//		pDC->TextOut(300,60, "inside     ");
		pDC->TextOut(px.x, px.y+40, "inside     ");
	}
	else{
//		pDC->TextOut(300,60, "outside    ");
		pDC->TextOut(px.x,px.y+40, "outside    ");
	}
	ReleaseDC(pDC);
	
	int result = pDoc->dat.polys.ClickedOnAPoint(NewPoint, cornerclicked);
//		ShowFloat(result, "res");
	if (result >= 0){
		int olds=selectedpolygon;
		selectedpolygon = result;
		if (selectedpolygon != olds){
			Invalidate();
		}
	}

}
void Traces::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//xxxyyy
	if (firsttime){
		return;
	}
	if (OnTrackingCells){
		OnRButtonTrackingCells(nFlags, point);
		return;
	}
	if (pDoc->IsLoadingFile){
		return;
	}
	if (IsCounting){
		OnRButtonDownCounting(nFlags, point);
		return;
	}
	if (pDoc->dat.p.ViewType==2){
		OnRbuttonDownPolygons(nFlags, point);
		return;
	}
	CDC * pDC = GetDC();
	ShiftOriginDC(pDC);
	char s[20];

	double l = pDoc->dat.p.drawob()->Length();
	_gcvt(l, 6, s);
	CString c=CString(s);
	pDC->TextOut(300, 20, "                                             ");

///	double ** im = pDoc->currentimage;
	ImageDataType ** im = pDoc->id.data;
	if (im){
//		if ( (point.x<pDoc->dat.p.xsizeimage) & (point.y<pDoc->dat.p.ysizeimage) ){
		if ( (point.x<pDoc->id.xsize) & (point.y<pDoc->id.ysize) ){
			double v=im[point.x][point.y];
			_gcvt(v, 5, s);
			c += CString("v = ")+CString(s);
		}
	}

	pDC->TextOut(300, 20, c);

	
	int index = pDoc->dat.p.drawob()->GetRelevantIndex(point);
	_itoa(index, s, 10);
	c = CString(s);

	Vect v;
	double r1, r2;
	pDoc->dat.p.drawob()->GetMidPointAndRadius(v, r1, r2);
	double r=r1 + (r2-r1)*double(index)/l;
	_gcvt(r, 6, s);
	c += CString("; r = ") + CString(s);

	pDC->TextOut(300,40, "                                 ");
	pDC->TextOut(300,40, c);

	if (pDoc->dat.p.drawob()->PointIsInside(point)){
		pDC->TextOut(300,60, "inside     ");
	}
	else{
		pDC->TextOut(300,60, "outside    ");
	}


	_itoa(point.x, s, 10);
	c = CString(s)+ " ";
	_itoa(point.y, s, 10);
	c += s;
	pDC->TextOut(300,80, c);

	_gcvt(GetMeanInObjectArea(), 7, s);

	c="Mean " + CString(s);
	pDC->TextOut(300,100, c);


	ReleaseDC(pDC);


	CView::OnRButtonDown(nFlags, point);
}


void Traces::OnGenerateInit(){
	*InitGenerator = true;
//	OnGenerateNext();
}

void Traces::OnGenerateNext(){
	CPoint p;
	CDC * pDC = GetDC();
	ShiftOriginDC(pDC);
	int index;
	
	while (	pDoc->dat.p.drawob()->GeneratePoints(*InitGenerator, p, index)){
		pDC->SetPixel(p, RGB(0,0,0)); 
//		ShowFloat(index, "index");
	}

//	ShowFloat(p.x, "x");

	ReleaseDC(pDC);
}

void Traces::ReloadButtonClicked(){

	CWaitCursor dummy;

//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *) GetDocument();
	DataFile & d = pDoc->dat;
//	if (!pDoc->ReadFile(pDoc->dat.CurrentIndex)){
//		if (!pDoc->dat.initialized){
	if (!d.images) return;
	if (!pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex)){
		if (!d.images[d.ImageIndex].initialized){
			return;
		}
		Alert("could not read file; maybe no file opened!");
		return;
	}
//	CChildFrame * child = (CChildFrame *)(GetParent()->GetParent()->GetParent());

	child->OnAnalyzeThisimage();
	
}

void Traces::CopyThisToGeneralButtonClicked(){
//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *) GetDocument();
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	pApp->p = pDoc->dat.p;

}
void Traces::CopyGeneralToThisButtonClicked(){

//	Alert(pDoc->dat.p.LookupTableName);
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	pDoc->dat.p = pApp->p;
	pDoc->dat.p.RectOb.SetSize(pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage);
	pDoc->dat.p.ConusOb.SetSize(pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage);

//	Alert(frame->p.LookupTableName);
//	Alert(pDoc->dat.p.LookupTableName);
	pDoc->UpdateAllViews(NULL);
//	MyInvalidate();
}


void Traces::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
//	frame->p = pDoc->dat.p;

//	Beep(1000,10);
	
}

void Traces::MyInvalidate(){
//	frame->p = pDoc->dat.p;
	if (OriginDC.x>0){
		OriginDC.x = 0;
	}
	if (OriginDC.y>0){
		OriginDC.y = 0;
	}

	pDoc->UpdatePolygonDataPoints();

	Invalidate();
}

bool Traces::ClickingIsTracked(bool ShowWarning){
	if (OnCreatingPolygon){
		if (ShowWarning) Alert("OnCreatingPolygon");
		return true;
	}
	if (OnCreatingLine){
		if (ShowWarning) Alert("OnCreatingLine");
		return true;
	}
	if (SelectingFillPoints){
		if (ShowWarning) Alert("SelectingFillPoints");
		return true;
	}
	if (SelectingFillPointsInSelectedPolygon){
		if (ShowWarning)  Alert("SelectingFillPointsInSelectedPolygon");
		return true;
	}
	if (OnMovingPolyWithArrowKey){
		if (ShowWarning) Alert("OnMovingPolyWithArrowKey");
		return true;
	}
	if (OnMovingLineWithArrowKey){
		if (ShowWarning) Alert("OnMovingLineWithArrowKey");
		return true;
	}
	if (SelectingFillPoints){
		if (ShowWarning) Alert("already selected start fill");
		return true;
	}
	if (IsCounting){
		if (ShowWarning) Alert("is counting");
		return true;
	}
	return false;

}

void Traces::OnCreateapolygon(){
	if (ClickingIsTracked(true)){
		return;
	}

	if (pDoc->dat.p.ViewType != 2){
		pDoc->dat.p.ViewType = 2;
		Invalidate();
	}
	pointsadded = 0;
	OnCreatingPolygon = true;
	Invalidate();
}
void Traces::OnFillStartselectpoints(){
	if (ClickingIsTracked(true)){
		return;
	}
	DataFile & d = pDoc->dat;
	if (!d.images) {
		Alert("no image");
		return;
	}
	SelectingFillPoints = true;
	Invalidate();
}
void Traces::OnFillStopselectpoints(){
	SelectingFillPoints = false;
	SelectingFillPointsInSelectedPolygon = false;
	Invalidate();
}

void Traces::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if ((!OnCreatingPolygon) && (!OnCreatingLine)){
		return;
	}
	OnCreatingPolygon = false;
	OnCreatingLine = false;
/*
if (ActualPoly.NPoints()>0){
		pDoc->dat.polys.AddPolygon(ActualPoly);
	}
*/
	Invalidate();
	
//	CView::OnLButtonDblClk(nFlags, point);
}

void Traces::AddPointToPolygonOrLine(UINT nFlags, CPoint point, bool IsLine){
	CDC * pDC = GetDC();
	UnCorrectClickedPointForOrigin(point);
	if (pointsadded == 0){
		if (!IsLine){
			selectedpolygon = pDoc->dat.polys.NPoly();
//			pDoc->dat.polys.polygons.UpdateBuffer(selectedpolygon+1, true);
			pDoc->dat.polys.polygons.resize(selectedpolygon+1);

//			PolygonObject * ps = (PolygonObject *)(pDoc->dat.polys.polygons.buff);
//			ActualPoly = &(ps[selectedpolygon]);
			ActualPoly = &(pDoc->dat.polys.polygons[selectedpolygon]);

		}
		else{
			selectedline = pDoc->dat.lines.NPoly();
//			pDoc->dat.lines.polygons.UpdateBuffer(selectedline+1, true);
			pDoc->dat.lines.polygons.resize(selectedline+1);
//			PolygonObject * ps = (PolygonObject *)(pDoc->dat.lines.polygons.buff);
//			ActualPoly = &(ps[selectedline]);
			ActualPoly = &(pDoc->dat.lines.polygons[selectedline]);
		}
	}
	pointsadded++;
	CPen * OldPen = pDC->SelectObject(&whitePen);
//	ShowFloat(Zoom, "Zoom  a  a ");

	int offsetx, offsety;
	GetOffset(offsetx, offsety);

	ShiftOriginDC(pDC);
	ActualPoly->Draw(pDC, offsetx, offsety, -1, Zoom, ZoomIn, true);
	pDC->SelectObject(&redPen);


//	ShowFloat(offsetx, "offsetx");
//	ShowFloat(offsety, "offsety");
	CPoint PointToAdd = point;
	CorrectClickedPointForOrigin(PointToAdd);
	PointToAdd.x -= offsetx;
	PointToAdd.y -= offsety;

	bool DoAddPoint = true;
	
	if (pointsadded>1){

//		ShowFloat(pointsadded, "pa");
		CPoint LastPoint = ActualPoly->pointlist[pointsadded-2];
//		ShowFloat(LastPoint.x, "lx");
//		ShowFloat(LastPoint.y, "ly");
//		ShowFloat(PointToAdd.x, "ax");
//		ShowFloat(PointToAdd.y, "ay");


		if ((PointToAdd.x==LastPoint.x) && (PointToAdd.y==LastPoint.y)){
//			Alert0("no add");
			pointsadded--;
			DoAddPoint = false;
//			Beep(1000,100);
		}
	}

	if (DoAddPoint)	{
		ActualPoly->AddPoint(PointToAdd);
		ActualPoly->IsLine = IsLine;
	}

	ActualPoly->Draw(pDC, offsetx, offsety, -1, Zoom, ZoomIn, true);

	ReleaseDC(pDC);
	
}
void Traces::OnRegionofinterestDeleteselectedpolygon(bool AskDelete){
	if (pDoc->dat.p.ViewType != 2){
		return;
	}
	if (AskDelete){
		if (!Confirm("Are you sure to delete the selected polygon?")){
			return;
		}
	}

	pDoc->dat.polys.DeletePolygon(selectedpolygon);
	selectedpolygon=0;
	MyInvalidate();	



}

void Traces::OnLineDeleteselectedline(bool AskDelete){
	if (pDoc->dat.p.ViewType != 2){
		return;
	}
	if (AskDelete){
		if (!Confirm("Are you sure to delete the selected line?")){
			return;
		}
	}

	pDoc->dat.lines.DeletePolygon(selectedline);
	selectedline=0;
	MyInvalidate();	
}
/*
void Traces::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CScrollView::OnPaint() for painting messages
	{
		CRect rect;
		GetClientRect(&rect);
		SIZE s;
		s.cx = 2*rect.right;
		s.cy = 2*rect.bottom;
//		SetScaleToFitSize(s);
		//SetScrollSizes(MM_TEXT, s);
	}
//	CScrollView::OnPaint();
	CView::OnPaint();

}
*/
void Traces::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo){
		pDC->SetMapMode(MM_TEXT);
		CRect rect;
		GetClientRect(&rect);
		SIZE s;
		s.cx = 2*rect.right;
		s.cy = 2*rect.bottom;
		s.cx = rect.right;
		s.cy = rect.bottom;
//		SetScaleToFitSize(s);
		SetScrollSizes(MM_TEXT, s);
		CScrollView::OnPrepareDC(pDC, pInfo);
}

void Traces::GetOffset(int & offsetx, int & offsety){
	pDoc->GetOffset(offsetx, offsety);
}

void Traces::OnOffsetClearalloffsets(){
	if (!Confirm("Are you sure to clear all offsets?")){
		return;
	}
	pDoc->ClearAllOffsets();
	ReloadButtonClicked();
}


void Traces::OnOffsetXPlus10Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(10,0);
	ReloadButtonClicked();
}
void Traces::OnOffsetXPlus1Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(1,0);
	ReloadButtonClicked();
}
void Traces::OnOffsetXMinus10Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(-10,0);
	ReloadButtonClicked();
}
void Traces::OnOffsetXMinus1Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(-1,0);
	ReloadButtonClicked();
}

void Traces::OnOffsetYPlus10Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(0,10);
	ReloadButtonClicked();
}
void Traces::OnOffsetYPlus1Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(0,1);
	ReloadButtonClicked();
}
void Traces::OnOffsetYMinus10Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(0,-10);
	ReloadButtonClicked();
}
void Traces::OnOffsetYMinus1Clicked(){
	CWaitCursor dummy;
	pDoc->IncOffset(0,-1);
	ReloadButtonClicked();
}


void Traces::OnOffsetTrack(){

	if (pDoc->IsLoadingFile){
		return;
	}
	DataFile & d = pDoc->dat;

	if (d.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}

	if (this->selectedpolygon>=d.polys.NPoly()){
		Alert("selected poly>=NPoly"); return;
	}
	if (this->selectedpolygon<0){
		Alert("selected poly<0"); return;
	}

	if (d.NImages<1){
		Alert("less than 1 image");
		return;
	}

	ImageSequence & image = d.images[d.ImageIndex];

	int index = d.images[d.ImageIndex].CurrentIndex;

	if (index==0) {
		Alert("This is first index");
		return;
	}

	DoOffsetTrack(index);
	
	ReloadButtonClicked();

}
void Traces::DoOffsetTrack(int index){
	CWaitCursor dummy;

	if (pDoc->IsLoadingFile){
		return;
	}
	DataFile & d = pDoc->dat;

	if (d.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}

	if (this->selectedpolygon>=d.polys.NPoly()){
		Alert("selected poly>=NPoly"); return;
	}
	if (this->selectedpolygon<0){
		Alert("selected poly<0"); return;
	}

	if (d.NImages<1){
		Alert("less than 1 image");
		return;
	}

	ImageSequence & image = d.images[d.ImageIndex];

//	int index = d.images[d.ImageIndex].CurrentIndex;

	if (index==0) {
		Alert("This is first index");
		return;
	}
	if (index>=image.LastIndex){
		Alert0("Image > LastIndex");
		return;
	}


	bool DoShow = false;
	int xsize = pDoc->dat.p.xsizeimage;
	int ysize = pDoc->dat.p.ysizeimage;

	ImageData RefImage;
	RefImage.Alloc(xsize, ysize);

//	CellsClass celllist;
	d.ReadImage(index-1+image.FirstIndex, RefImage, pDoc->pActualCells, false, DoShow);
	d.ReadImage(index+image.FirstIndex, pDoc->id, pDoc->pActualCells, false, DoShow);

	int offsetx, offsety;
	offsetx = d.OffsetX[index];
	offsety = d.OffsetY[index];

	int offsetxref, offsetyref;
	offsetxref = d.OffsetX[index-1];
	offsetyref = d.OffsetY[index-1];

	int SaveOffsetx = offsetx;
	int SaveOffsety = offsety;


//	ShowFloat(offsetxref, "offsetxref");
//	ShowFloat(offsetyref, "offsetyref");
//	ShowFloat(offsetx, "offsetx");
//	ShowFloat(offsety, "offsety");

	d.polys.Track(0, selectedpolygon, RefImage.data, pDoc->id.data, xsize, ysize, offsetxref, offsetyref, offsetx, offsety);
//	d.polys.Track(selectedpolygon, pDoc->id.data, pDoc->id.data, xsize, ysize, offsetx, offsety);

//	ShowFloat(offsetx, "offsetx");
//	ShowFloat(offsety, "offsety");

	d.IncOffset(index, offsetx, offsety);
//	pDoc->IncOffset(offsetx-SaveOffsetx, offsety-SaveOffsety);

}


void Traces::OnOffsetTrackall(){
	if (pDoc->IsLoadingFile){
		return;
	}
	DataFile & d = pDoc->dat;

	if (d.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}

	if (this->selectedpolygon>=d.polys.NPoly()){
		Alert("selected poly>=NPoly"); return;
	}
	if (this->selectedpolygon<0){
		Alert("selected poly<0"); return;
	}

	if (d.NImages<1){
		Alert("less than 1 image");
		return;
	}

	ImageSequence & image = d.images[d.ImageIndex];

	int index = d.images[d.ImageIndex].CurrentIndex;

	if (index==0) {
		index = 1;
	}

	CDC * pDC = GetDC();


	while (true){
		if (index>=d.images[d.ImageIndex].LastIndex){
			break;
		}
		char s[20];
		_itoa(index, s, 10);
		pDC->TextOutA(20, 20, "                       ");
		pDC->TextOutA(20, 20, CString(s)+CString(" ... ESC to stop"));
		
		DoOffsetTrack(index);
		index++;
		if (InKey()==ESCAPEKEY){
			break;
		}

	}
	
	ReleaseDC(pDC);
	ReloadButtonClicked();

}


void Traces::OnTrackThisButtonClicked(){
	OnOffsetTrack();
}


int FindClosest(int index, int & NewIndex, vector<EdgePoint<int>> & edges){
	__int64 MinDist = 1000000000000000000;
	bool found = false;
	int x = edges[index].point.x;
	int y = edges[index].point.y;
	for (int i=0; i<edges.size(); i++){
		EdgePoint<int> & edge = edges[i];
		if (edge.NotYetUsedInPolygon){
			__int64 DX = x-edge.point.x;
			__int64 DY = y-edge.point.y;
			__int64 dist = DX*DX+DY*DY;
			if (dist<MinDist){
				found = true;
				MinDist = dist;
				NewIndex = i;
			}
		}
	}
	if (found){
		edges[NewIndex].NotYetUsedInPolygon = false;
		return MinDist;
	}
	else{
		return -1;
	}
}

//bool FindUnused(int w, int h, int * data, bool * NotUsed, int & x, int & y){
bool FindUnused(int w, int h, vector<int> & data, vector<bool> & NotUsed, int & x, int & y){
	int a=0;
	bool found = false;
	for (int iy=0; iy<h; iy++){
		for (int ix=0; ix<w; ix++){
			if ( data[a]>0 && NotUsed[a]){
//			if ( data[a]>0 ){
				x = ix;
				y = iy;
				NotUsed[a] = false;
				return true;
			}
			a++;
		}
	}
	return false;
	
}
void Traces::OnHelpTestedge(){

//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *) GetDocument();
	DataFile & d = pDoc->dat;
//	if (!pDoc->ReadFile(pDoc->dat.CurrentIndex)){
//		if (!pDoc->dat.initialized){
	if (!d.images) return;
	if (!pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex)){
		if (!d.images[d.ImageIndex].initialized){
			return;
		}
		Alert("could not read file; maybe no file opened!");
		return;
	}
//	CChildFrame * child = (CChildFrame *)(GetParent()->GetParent()->GetParent());

	child->OnAnalyzeThisimage();

	int w = d.images[d.ImageIndex].Sizex;
	int h = d.images[d.ImageIndex].Sizey;

	int picsize = w*h;

//	int * data = new int [picsize];

	std::vector<int> data;
	data.resize(picsize);

	CannyEdgeDetector ch(w, h);

	int i=0;
	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			data[i] = pDoc->id.data[x][y];
			i++;
		}
	}

//	Alert0("process");

	DiagEdgeDetect diag;

	diag.LowThreshold = lowThreshold;
	diag.HighThreshold = highThreshold;
	diag.MaxDistPolyPoints = MaxDistPolyPoint;
	diag.MaxPolyPoints = MaxPolyPoints;

	diag.GaussianKernelRadius = gaussianKernelRadius;
	diag.gaussianKernelWidth = gaussianKernelWidth;

	if (diag.DoModal()!=IDOK){
		return;
	}
	lowThreshold = ch.lowThreshold = diag.LowThreshold ;
	highThreshold = ch.highThreshold = diag.HighThreshold;
	MaxDistPolyPoint = diag.MaxDistPolyPoints;
	MaxPolyPoints = diag.MaxPolyPoints;

	gaussianKernelRadius = ch.gaussianKernelRadius =diag.GaussianKernelRadius ;
	gaussianKernelWidth = ch.gaussianKernelWidth = diag.gaussianKernelWidth;

	CWaitCursor dummy;
	ch.process(data);
//	Alert0("end process");

	i=0;
	int NPointsFound=0;
	vector<EdgePoint<int>> edges;
	EdgePoint<int> edge;

	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			pDoc->id.data[x][y] = data[i];
			if (data[i] != 0){
				edge.point = CPoint(x, y);
				edge.value = data[i];
				edges.push_back(edge);
				NPointsFound++;
			}
			i++;
		}
	}
	if (NPointsFound<1){
		Invalidate();
		return;
	}

	bool found = false;

	//PolygonObject NewPoly;
	int NPAdd=0;
	int index = 0;

	selectedpolygon = pDoc->dat.polys.NPoly();
	selectedline = pDoc->dat.lines.NPoly();
	int startpoly = selectedpolygon;
	int startline = selectedline;
	int NPolyFound = 1;

StartNewPolyGon:
//		Alert0("start new poly");
//		pDoc->dat.polys.polygons.UpdateBuffer(selectedpolygon+1, true);
		pDoc->dat.polys.polygons.resize(selectedpolygon+1);
//		PolygonObject * ps = (PolygonObject *)(pDoc->dat.polys.polygons.buff);
//		ActualPoly = &(ps[selectedpolygon]);
		ActualPoly = &(pDoc->dat.polys.polygons[selectedpolygon]);
		edges[index].NotYetUsedInPolygon = false;
//		ActualPoly->AddPoint(CPoint(x, y));
		ActualPoly->AddPoint(edges[index].point);

	int MinSquqreDist = MaxDistPolyPoint*MaxDistPolyPoint;
	while (true){
		int newindex;

//		int MinDist = FindClosest(w, h, x, y, xp, yp, data, NotUsed);
		int MinDist = FindClosest(index, newindex, edges);
		if (MinDist<0){ // all points used up
			goto EndLoopFindNeighbor;
		}
		if (MinDist<MinSquqreDist){ // continue this poly
			goto FoundNeighbor;
		}
		// Start new polygon:
		index = newindex;
		selectedpolygon++;
		NPolyFound++;
		goto StartNewPolyGon;
FoundNeighbor:
//		Alert0("found");
//		NewPoly.AddPoint(CPoint(xp, yp));
		ActualPoly->AddPoint(edges[newindex].point);
		NPAdd++;
		index = newindex;
		found = true;
	} // while

EndLoopFindNeighbor:

	ShowFloat(NPolyFound, "NPolyFound");
//	Invalidate();

	for (int i=0; i<NPolyFound; i++){
		int ipoly = startpoly + i;
//		PolygonObject * ps = (PolygonObject *)(pDoc->dat.polys.polygons.buff);
//		ActualPoly = &(ps[ipoly]);
		ActualPoly = &pDoc->dat.polys.polygons[ipoly];
		int NP = ActualPoly->GetNumberOfVertices();
		if (NP>MaxPolyPoints ){
			vector <CPoint> & buff =ActualPoly->pointlist;
			for (int i=0; i<MaxPolyPoints; i++){
				int j = int( double(i*NP)/double(MaxPolyPoints) );
//		ShowFloat(j, "j");
//			ShowFloat(NP, "NP");
				buff[i] = buff[j];
			}
			ActualPoly->pointlist.resize(MaxPolyPoints);
		}
	}
	Invalidate();

}
void Traces::OnRegionofinterestDeleteallplygons(){
		if (pDoc->dat.p.ViewType != 2){
		return;
	}
	if (!Confirm("Are you sure to delete ALL POLYGONS?")){
		return;
	}


	selectedpolygon=pDoc->dat.polys.NPoly()-1;
//	int NP = pDoc->dat.polys.NPoly();
//	for (int i=0; i<NP; i++){
	while (selectedpolygon>=0){
//		pDoc->dat.polys.DeletePolygon(NP-i-1);
		pDoc->dat.polys.DeletePolygon(selectedpolygon);
		selectedpolygon--;
	}
	selectedpolygon = 0;
	MyInvalidate();	

}

void Traces::MakeMembranePolygons(){
	if (pDoc->dat.p.ViewType != 2){
		return;
	}

	DiagMembranePolys diag;

	diag.m_IsCircular = pDoc->dat.p.ROISAreClosedInMembraneAnalysis;
	diag.m_MembraneSize = pDoc->dat.p.SizeOfMembraneInPixels;


	if (diag.DoModal()!=IDOK){
		return;
	}
	pDoc->dat.p.ROISAreClosedInMembraneAnalysis = diag.m_IsCircular;
	pDoc->dat.p.SizeOfMembraneInPixels = diag.m_MembraneSize;

	
	pDoc->dat.polys.MakeMembranePolys(selectedpolygon, pDoc->dat.p.SizeOfMembraneInPixels, pDoc->dat.p.ROISAreClosedInMembraneAnalysis);
//	selectedpolygon=0;
	MyInvalidate();	


}

void Traces::OnRegionofinterestMakeopposingclosedroisformalinearroi(){
	if (pDoc->dat.p.ViewType != 2){
		return;
	}

	DiagMembranePolys diag;

	diag.m_IsCircular = pDoc->dat.p.ROISAreClosedInMembraneAnalysis;
	diag.m_MembraneSize = pDoc->dat.p.SizeOfMembraneInPixels;


	if (diag.DoModal()!=IDOK){
		return;
	}
	pDoc->dat.p.ROISAreClosedInMembraneAnalysis = diag.m_IsCircular;
	pDoc->dat.p.SizeOfMembraneInPixels = diag.m_MembraneSize;

	
	pDoc->dat.polys.MakeMembranePolys(selectedpolygon, pDoc->dat.p.SizeOfMembraneInPixels, pDoc->dat.p.ROISAreClosedInMembraneAnalysis, true);
//	selectedpolygon=0;
	MyInvalidate();	


}
bool Traces::OnRegionofinterestCopyselectedpolygon32850(){
	if (pDoc->dat.p.ViewType != 2){
		return false;
	}

	DataFile & d = pDoc->dat;
	if (d.polys.NPoly()<1){
		Alert("No polygon");
		return false;
	}

//	frame->CopiedPolygons.polygons.UpdateBuffer(1);
	frame->CopiedPolygons.polygons.resize(1);
//	PolygonObject & p = d.polys.polygons.buff[selectedpolygon];
	PolygonObject & p = d.polys.polygons[selectedpolygon];
//	PolygonObject & pToCopyIn = frame->CopiedPolygons.polygons.buff[0];
	PolygonObject & pToCopyIn = frame->CopiedPolygons.polygons[0];
	p.Copy(pToCopyIn);
	return true;
}
void Traces::OnRegionofinterestPastecopiedpolygon(){
	int NCopiedPolygons = frame->CopiedPolygons.NPoly();
	if (NCopiedPolygons <1){
		Alert("Nothing to copy");
		return;
	}
//	PolygonObject & pToPasteFrom = frame->CopiedPolygons.polygons.buff[0];
	PolygonObject & pToPasteFrom = frame->CopiedPolygons.polygons[0];

	DataFile & d = pDoc->dat;
	d.polys.AddPolygon(pToPasteFrom);
	selectedpolygon = d.polys.NPoly()-1;

	MyInvalidate();	
}
void Traces::OnHelpStartFill(){
	DataFile & d = pDoc->dat;

	if (!d.images) {
		Alert("no image");
		return;
	}
	if (!pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex)){
		if (!d.images[d.ImageIndex].initialized){
			return;
		}
		Alert("could not read file; maybe no file opened!");
		return;
	}

	int w = d.images[d.ImageIndex].Sizex;
	int h = d.images[d.ImageIndex].Sizey;

//	io myio;
//	if (myio.InDouble(PercentageAcceptanceFill, "PercentageAcceptanceFill")!=IDOK){
//		return;
//	}
//	Alert("atart fill selcted");
	Selected.resize(w*h);
//	ShowFloat(w, "w");
//	ShowFloat(h, "h");
	for (int i=0; i<(w*h); i++){
		Selected[i] = false;
	}
//	Alert("end fill selcted");
	CurrentFillWidth = w;
	CurrentFillHeight = h;

	FillStarted  = true;

//	Alert("check comp");
//	if (comp){
//		Alert("delete comp");
//		delete comp;
//	}
//	Alert("start create");
	comp.Init(w, h);
//	Alert("created");

	for (int y=0; y<h; y+=1){//Zoom){
		int offset = y*w;
		for (int x=0; x<w; x+=1){//Zoom){
			if (FillStarted){
				if ((y>100) && (y<200)){
					if ((x>300) && (x<700)){
						//Selected[x+offset] = true;
					}
				}
			}
		}
	}
//	Alert("end");
//	ShowFloat(comp.width, "comp.width");
}

void Traces::OnHelpFill(){
//	Alert("start");
//	ShowFloat(comp.width, "comp.width");
	DataFile & d = pDoc->dat;
	if (!d.images) {
		Alert("no image");
		return;
	}
	if (!FillStarted){
		Alert("Fill not started");
		return;
	}
//	ShowFloat(ListObject::LengthList(comp.Points), "ListObject::LengthList(comp->Points)");

	if (!pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex)){
		if (!d.images[d.ImageIndex].initialized){
			return;
		}
		Alert("could not read file; maybe no file opened!");
		return;
	}
	child->OnAnalyzeThisimage();

	int w = d.images[d.ImageIndex].Sizex;
	int h = d.images[d.ImageIndex].Sizey;

	int picsize = w*h;

	if (w!=CurrentFillWidth){
		Alert("w!=CurrentFillWidth");
		return;
	}
	if (h!=CurrentFillHeight){
		Alert("w!=CurrentFillHeight");
		return;
	}

	std::vector<int> data;
	data.resize(picsize);


	int i=0;
	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			data[i] = pDoc->id.data[x][y];
			i++;
		}
	}

	if (d.polys.NPoly()<1){
		Alert("no polygon");
		return;
	}
//	PolygonObject * buff = (PolygonObject *)d.polys.polygons.buff;
//	CPoint StartPoint = buff[selectedpolygon].DataPoints[0];
	CPoint StartPoint = d.polys.polygons[selectedpolygon].DataPoints[0];

//	ShowFloat(StartPoint.x, "x0");
//	ShowFloat(StartPoint.y, "y0");


//	ShowFloat(ListObject::LengthList(comp.Points), "ListObject::LengthList(comp->Points)");
//	ShowFloat(comp.width, "comp.width");
	CWaitCursor dummy;
	CDC * pDC = GetDC();
	comp.Init(w,h);
	comp.Scan(StartPoint, frame->PercentageAcceptanceFill, Selected, data, pDC);
	ReleaseDC(pDC);
	ShowFloat(ListObject::LengthList(comp.Points), "ListObject::LengthList(comp->Points)");
	comp.CopyToSelected(Selected);

}
void Traces::OnFillClear(){
	DataFile & d = pDoc->dat;
	if (!d.images) {
		Alert("no image");
		return;
	}
	if (!pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex)){
		if (!d.images[d.ImageIndex].initialized){
			return;
		}
		Alert("could not read file; maybe no file opened!");
		return;
	}
	child->OnAnalyzeThisimage();

	int w = d.images[d.ImageIndex].Sizex;
	int h = d.images[d.ImageIndex].Sizey;

	Selected.resize(w*h);
	for (int i=0; i<(w*h); i++){
		Selected[i] = false;
	}
	CurrentFillWidth = w;
	CurrentFillHeight = h;
	MyInvalidate();
	child->OnViewViewfilledarea();

}
void Traces::InitSelected(int w, int h){
	CurrentFillWidth = w;
	CurrentFillHeight = h;
	Selected.resize(w*h);
	for (int i=0; i<w*h; i++){
		Selected[i] = false;
	}
	SelectedInitialized = true;
}


void Traces::AddFillArea(CPoint point, bool UseOnlySelectedPolygon){
	DataFile & d = pDoc->dat;
	if (!d.images) {
		Alert("no image");
		return;
	}
	PolygonObject * poly = NULL;
//	int NPoly = d.polys.polygons.GetSize();
	int NPoly = d.polys.polygons.size();
	if (UseOnlySelectedPolygon){
		if (NPoly<1) return;
//		poly = &d.polys.polygons.buff[selectedpolygon];
		poly = &d.polys.polygons[selectedpolygon];
		if (!poly->IsInside(point)){
			return;
		}
	}

//	ShowFloat(ListObject::LengthList(comp.Points), "ListObject::LengthList(comp->Points)");

	if (!pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex)){
		if (!d.images[d.ImageIndex].initialized){
			return;
		}
		Alert("could not read file; maybe no file opened!");
		return;
	}
	child->OnAnalyzeThisimage();

	int w = d.images[d.ImageIndex].Sizex;
	int h = d.images[d.ImageIndex].Sizey;

	int picsize = w*h;

	if (!SelectedInitialized){
		InitSelected(w, h);
	}
	std::vector<int> data;
	data.resize(picsize);

	int i=0;
	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			data[i] = pDoc->id.data[x][y];
			i++;
		}
	}


	CWaitCursor dummy;
	CDC * pDC = GetDC();
	comp.Init(w,h);
	int NumPointsSelected = 0;
	bool PointAlreadySeleted = false;
	i=0;
	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			if (Selected[i]){
				NumPointsSelected++;
				if ((x==point.x) && (y==point.y)){
					PointAlreadySeleted = true;
				}
			}
			i++;
		}
	}
	if (PointAlreadySeleted){
		//Alert("Point already selected");
		return;
	}
//	bool ExpandInitialPoint = (NumPointsSelected == 0);
	double mean = 0.0;
	if (NumPointsSelected>0){
		for (int i=0; i<w*h; i++){
			if (Selected[i]){
				mean += data[i];
			}
		}
		mean /= double(NumPointsSelected);
	}
	
	comp.ScanArea(point, frame->PercentageAcceptanceFill,
				NumPointsSelected, mean, frame->UseGlobalMeanForFill,
				Selected, data, pDC, UseOnlySelectedPolygon, poly);
	ReleaseDC(pDC);
//	ShowFloat(ListObject::LengthList(comp.Points), "ListObject::LengthList(comp->Points)");
	comp.CopyToSelected(Selected);

	NumPointsSelected = 0;
	i=0;
	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++){
			if (Selected[i]){
				NumPointsSelected++;
			}
			i++;
		}
	}
//	ShowFloat(NumPointsSelected, "NumPointsSelected");

	MyInvalidate();
	child->OnViewViewfilledarea();

}

void Traces::OnFillUndolast(){
	if (!SelectedInitialized){
		return;
	}
	comp.RemoveFromSelection(Selected);
	MyInvalidate();

}

void Traces::OnFillTogglehide(){
	ShowFill = !ShowFill;
	MyInvalidate();
}

void Traces::GetMinMax(double & Min, double & Max){
	CWaitCursor dummy;

	ImageDataType ** im = pDoc->id.data;

	if (!im){
		Min = Max = 0.0;
		return;
	}
	ImageDataType ** ref = pDoc->id.meanrefdata;
	Min = 1e33;
	Max = -1e33;

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		if (!pDoc->id.refcalculated){
			pDoc->CalculateRef();
		}
	}

	Min = im[0][0];
	Max = im[0][0];

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		Min -= ref[0][0];
		Max -= ref[0][0];
	}

	for (int y=0; y<pDoc->id.ysize; y++){
		for (int x=0; x<pDoc->id.xsize; x++){
			int d = int(im[x][y]);
			if (pDoc->dat.p.SubtractInitialValuesForDraw){
				d -= int(ref[x][y]);
			}
			if (d<Min) Min=d;
			if (d>Max) Max = d;
		}
	}

}
void Traces::DrawCellTest(CDC* pDC, bool Export, CFile * fp){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	bool releasedc = false;

	if (!pDC){
		releasedc = true;
		pDC = GetDC();
	}


	CWaitCursor dummy;

	if (SelectedInitialized){
		child->AreaSelected = 0;
		for (int i=0; i<CurrentFillWidth*CurrentFillHeight; i++){
			if (Selected[i]){
				child->AreaSelected ++;
			}
		}
	}

	CDC * mDC = new CDC;
	mDC->CreateCompatibleDC(pDC);
	if (!mDC){
		Alert("could not create compatible DC");
		return;
	}

	CRect r;
	GetClientRect(&r);
	AdjustRectOrigin(r);

	CRect rect;

	GetClientRect(&rect);
	this->AdjustRectOrigin(rect);

	__int32 y; 
	__int32 x; 
	double Min = 0;
	double Max = 0;
	double dmax = 1;
	COLORREF * pref ;

	int MaxBit;

	if (pDoc->dat.p.NBitsColorTable == 8){
		pref = pDoc->dat.p.ColorTable256;
		MaxBit = 254;
	}
	else{
		pref = pDoc->dat.p.ColorTable65536;
		MaxBit = 65534;
	}

	ImageDataType ** im = pDoc->id.data;
	ImageDataType ** ref = pDoc->id.meanrefdata;

	bool Subtract = pDoc->dat.p.SubtractInitialValuesForDraw == true;
	double scaleimage = pDoc->dat.p.MultiplierImages;
	double offsetimage = pDoc->dat.p.OffsetImages;

	if (!im){
		goto enddrawOldTest;
	}
	CDib * m_pDib = NULL;

	if (pDoc->pActualCells != NULL){
		CellsClass & cells = *(pDoc->pActualCells);
		int NCells = cells.celllist.size();
		if (NCells>0){
			for (int i=0; i<NCells; i++){
				int color = Rand()*255.0;
				for (int j=0; j<cells.celllist[i].points.size(); j++){
					int x = cells.celllist[i].points[j].point.x;
					int y = cells.celllist[i].points[j].point.y;
					im[x][y] = color;
				}
			}
		}
	}

	
	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		if (!pDoc->id.refcalculated){
			pDoc->CalculateRef();
		}
		else{
		}
	}



/*	Min = im[0][0];
	Max = im[0][0];

	if (pDoc->dat.p.SubtractInitialValuesForDraw){
		Min -= ref[0][0];
		Max -= ref[0][0];
	}
*/

	if (pDoc->dat.p.AutoScale){
		GetMinMax(Min, Max);
		//ShowFloat(pDoc->id.xsize, "pDoc->id.xsize");
		//ShowFloat(pDoc->id.ysize, "pDoc->id.ysize");
/*		for (y=0; y<pDoc->id.ysize; y++){
			for (x=0; x<pDoc->id.xsize; x++){
				int d = int(im[x][y]);
				if (pDoc->dat.p.SubtractInitialValuesForDraw){
					d -= int(ref[x][y]);
				}
				if (d<Min) Min=d;
				if (d>Max) Max = d;
			}
		}
*/
	}
	else{
		Min = pDoc->dat.p.MinManScale;
		Max = pDoc->dat.p.MaxManScale;
	}
//	ShowFloat(Max, "Max");

	dmax = double(MaxBit)/(Max-Min+0.0000000001);
	int SizeImageY =  pDoc->id.ysize;
	int SizeImageX =  pDoc->id.xsize;
//	Alert("hi 1");
	bool UsePhasorMask = pDoc->UsePhasorMask;
	if (UsePhasorMask){
		if (pDoc->WhiteMask.size()<1){
			UsePhasorMask = false;
		}
		else{
			if (pDoc->WhiteMask.size()<SizeImageY){
				UsePhasorMask = false;
			}
			else{
				if (pDoc->WhiteMask[0].size()<SizeImageX){
					UsePhasorMask = false;			
				}
			}
		}
	}

//	ShowFloat(SizeImageX, "SizeImageX");

	if (!SelectedInitialized){
		InitSelected(SizeImageX, SizeImageY);
	}

	int XSIZe = SizeImageX;
	int YSIZe = SizeImageY;

	if ((YSIZe>MultZoom(rect.bottom, Zoom, ZoomIn)) && (!Export)){
		YSIZe = MultZoom(rect.bottom, Zoom, ZoomIn);
	}
	int BitMapYSize= DivZoom(YSIZe, Zoom, ZoomIn);

	if ((XSIZe>MultZoom(rect.right-100, Zoom, ZoomIn)) && (!Export)){
		XSIZe = MultZoom(rect.right-100, Zoom, ZoomIn);
	}
	int BitMapXSize = DivZoom(XSIZe, Zoom, ZoomIn);

//	ShowFloat(BitMapXSize  , "BitMapXSize  ");
	//ShowFloat(XSIZe , "XSIZe ");

//	int BitMapXSize = XSIZe/Zoom;
	//ShowFloat(BitMapXSize  , "BitMapXSize  ");

	m_pDib = new CDib(CSize(BitMapXSize,BitMapYSize), // size of the bitmap
					  24);            // bits per pixel
	VERIFY(m_pDib->CreateSection(mDC));
//	UpdateBitmapBuffer(BitMapXSize*BitMapYSize);

//	ShowFloat(Zoom, "zooom");
//	int * buffer = MyBitMapBuffer;
	BYTE* dibits = m_pDib->m_lpImage;
	for (int iy=0; iy<BitMapYSize; iy++){
		int y=MultZoom(iy, Zoom, ZoomIn);
		for (int ix=0; ix<BitMapXSize; ix++){
//	for (int iy=0; iy<BitMapYSize; iy++){
			int x=MultZoom(ix, Zoom, ZoomIn);

			double value = im[x][y];
			if (Subtract){
				value -= ref[x][y];
			}
			value = value*scaleimage + offsetimage;
			if (Export){
				int ivalue = int(value);
				char s[20];
				_itoa(ivalue,s, 10);
				CString str = CString(s)+"\t";
				fp->Write(str.GetBuffer(str.GetLength()), str.GetLength());
			}

			__int64 intf = __int64 ((value-Min)*dmax);
			if (intf<0){
				intf = 0;
			}
			if (intf > MaxBit){
				intf = MaxBit;
			}
			int BitMapValue = pref[intf];
			int posSelected = x + SizeImageX*y;
			if (SelectedInitialized && ShowFill){
//				bool PointIsSelected = Selected[x+offsetfine];
				bool PointIsSelected = Selected[posSelected];
				if (PointIsSelected){
					BitMapValue = RGB(0,255,0);//pref[0]; //RGB(0,255,0);
				}
			}

//			Alert("Hi 2");
//			if (false)
			if (UsePhasorMask){
//				Alert("mask");
//				if (false)

				if (pDoc->WhiteMask[y][x]){
					BitMapValue = RGB(255,0,255);
				}
			}


//			*buffer = BitMapValue;
//			buffer++;
			RGBQUAD rr;
			rr.rgbBlue = GetBValue(BitMapValue);
			rr.rgbGreen = GetGValue(BitMapValue);
			rr.rgbRed = GetRValue(BitMapValue);
			m_pDib->WritePixel(ix, iy, rr);
			if (false){
				*(dibits++) = GetRValue(BitMapValue);
				*(dibits++) = GetGValue(BitMapValue);
				*(dibits++) = GetBValue(BitMapValue);
			}
		}
		if (Export){
			if (y<(pDoc->id.ysize-1)){
				CString str=CString("\r\n");
				fp->Write(str.GetBuffer(str.GetLength()), str.GetLength());
			}
		}
	}


//	m_pDib->Draw(pDC,rect.TopLeft(),rect.Size());
	m_pDib->Draw(pDC, CPoint(0,0), CSize(BitMapXSize, BitMapYSize));
//	m_pDib->Draw(pDC,BMRect.TopLeft(),BMRect.Size());
//	m_pDib->Draw(pDC, CPoint(0,0), rect.Size());

enddrawOldTest:
	if (pDoc->dat.p.ViewType<2){
		pDoc->dat.p.drawob()->DrawObject(pDC, rect, 3, pDoc->dat.p.curspos, Zoom, ZoomIn);
	}
	if (pDoc->dat.p.ViewType==2){
		int offsetx, offsety;
		GetOffset(offsetx, offsety);

		pDoc->dat.polys.DrawPolys(pDC, selectedpolygon, Zoom, offsetx, offsety, ZoomIn);
		pDoc->dat.lines.DrawPolys(pDC, selectedline, Zoom, offsetx, offsety, ZoomIn);
	} 

 
if (false)
if (m_pDib){
		if (m_pDib->GetSizeImage()>0){
		m_pDib->Empty();
		delete m_pDib;
		}
	}

	ReleaseDC(mDC);
	delete mDC;

	if (releasedc){
		ReleaseDC(pDC);
	}

}

void Traces::OnFillSaveonfile(){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (!this->SelectedInitialized){
		Alert("Selected not initialized");
		return;
	}
	CString Name = pDoc->GetPathName()+CString("_SEL");
//	Alert (Name);
	
	CFileDialog
		   diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
		diag.m_ofn.lpstrTitle = "Save selection as";
	if (diag.DoModal()!= IDOK){
		return;
	}
	Name = diag.GetPathName();
	CFile file;
	file.Open(Name, CFile::modeCreate | CFile::modeWrite);

//	ShowFloat(Selected.size(), "Selected.size()");
//	ShowFloat(this->CurrentFillWidth,"w");
//	ShowFloat(CurrentFillHeight, "h");

	CWaitCursor dummy;
	for (int i=0; i<Selected.size(); i++){
		bool b = Selected[i];
		file.Write(&b, 1);
//		if (Selected[i]){
//			ShowFloat(i, "i");
//		}
	}
//	file.Write(&(Selected[0]), 1);
	file.Close();

}

void Traces::OnFillLoadfromfile(){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (!this->SelectedInitialized){
		Alert("Selected not initialized");
		return;
	}
	CString Name = pDoc->GetPathName()+CString("_SEL");


   	CFileDialog
	diag( true, NULL, Name, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	Name = diag.GetPathName();
	CFile file;
	if (!file.Open(Name, CFile::modeRead)){
		Alert("could not open file for read");
		return;
	}
	if (file.GetLength()*sizeof(Selected[0]) < Selected.size()){
		Alert("File is too small");
		return;
	}

	CWaitCursor dummy;
	for (int i=0; i<Selected.size(); i++){
		bool b=false;
		file.Read(&(b), 1);
		Selected[i] = b;
	}
	file.Close();
	child->OnViewViewfilledarea();

	MyInvalidate();
}

void Traces::OnFillFillcurrentlyselectedpolygon(){
	if (!this->SelectedInitialized){
		Alert("Selected not initialized");
		return;
	}

	int i=0;
	for (int y=0; y<this->CurrentFillHeight; y++){
		for (int x=0; x<this->CurrentFillWidth; x++){
			if (pDoc->dat.polys.IsInside(CPoint(x, y), selectedpolygon)){
				Selected[i] = true;
			}
			i++;
		}
	}
	child->OnViewViewfilledarea();
	MyInvalidate();	

}

void Traces::OnImageOffsetxChanged(){
	OriginDC.x = OriginDCxField->GetInt();
	if (OriginDC.x>0){
		OriginDC.x = 0;
	}
	int XSIZE = pDoc->id.xsize;
	if (OriginDC.x<(-XSIZE)){
			OriginDC.x = (-XSIZE);
	}
	OriginDCxField->SetInt(OriginDC.x);
	Invalidate();
}
void Traces::OnImageOffsetyChanged(){
	OriginDC.y = OriginDCyField->GetInt();
	if (OriginDC.y>0){
		OriginDC.y = 0;
	}

	int YSIZE = pDoc->id.ysize;
	if (OriginDC.y<(-YSIZE)){
		OriginDC.y = -YSIZE;
	}
	OriginDCyField->SetInt(OriginDC.y);
	Invalidate();
}

void Traces::ResetButtonClicked(){
	OriginDC.x = OriginDC.y = 0;
	Zoom = 1;
	ZoomIndex = 0;
	ZoomIn = false;
	MyInvalidate();
}


void Traces::OnRegionofinterestSelectpolygonbyindex(){

	if (pDoc->IsLoadingFile){
		return;
	}

	if (pDoc->dat.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}


	DiagSelectPoly diag;
	diag.NValidPolys = pDoc->dat.polys.NPoly();
	diag.selected = this->selectedpolygon;

	if (diag.DoModal() != IDOK){
		return;
	}
	selectedpolygon = diag.selected ;
	Invalidate();



	
}
BOOL Traces::PreTranslateMessage(MSG *pMsg){
	if( pMsg->message == WM_KEYDOWN ) {
        if( pMsg->wParam == VK_ESCAPE ) {
			//Beep(500,1000);

            OnKeyDown(VK_ESCAPE, 1, 0);
			return true;
        }
	}

	return CScrollView::PreTranslateMessage(pMsg);
}

void Traces::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
//	Beep(2000,200);
	if (nChar == VK_ESCAPE){
		if (OnCreatingPolygon){
			Beep(500, 20);
			OnCreatingPolygon = false;
			Invalidate();
			return;
		}
		if (OnCreatingLine){
			Beep(1000,20);
			OnCreatingLine = false;
			Invalidate();
			return;
		}
		if (OnMovingPolyWithArrowKey){
			OnMovingPolyWithArrowKey = false;
			Invalidate();
			return;
		}
		if (OnMovingLineWithArrowKey){
			OnMovingLineWithArrowKey = false;
			Invalidate();
			return;
		}
		if (IsCounting){
			IsCounting = false;
			Invalidate();
			return;
		}
		if (SelectingFillPoints){
			SelectingFillPoints = false;
			Invalidate();
			return;
		}
		if (SelectingFillPointsInSelectedPolygon){
			SelectingFillPointsInSelectedPolygon=false;
			Invalidate();
			return;
		}
	}
	if (OnMovingPolyWithArrowKey || OnMovingLineWithArrowKey){
		int dx=0;
		int dy=0;
		if (nChar == VK_LEFT){
			dx=-1;
		}
		if (nChar == VK_RIGHT){
			dx=1;
		}
		if (nChar == VK_UP){
			dy=-1;
		}
		if (nChar == VK_DOWN){
			dy=1;
		}
		if ( (dx!=0) || (dy!=0)){
			MoveSelectedPolyOrLine(dx, dy);
		}
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Traces::OnLineDefinenewline(){
	if (ClickingIsTracked(true)){
		return;
	}

	if (pDoc->dat.p.ViewType != 2){
		pDoc->dat.p.ViewType = 2;
		Invalidate();
	}
	pointsadded = 0;
	OnCreatingLine = true;
	Invalidate();
}


void Traces::OnLineDeletealllines(){
	if (pDoc->dat.p.ViewType != 2){
		return;
	}
	if (!Confirm("Are you sure to delete ALL lines?")){
		return;
	}
	selectedline=pDoc->dat.lines.NPoly()-1;
	while (selectedline>=0){
//		pDoc->dat.polys.DeletePolygon(NP-i-1);
		pDoc->dat.lines.DeletePolygon(selectedline);
		selectedline--;
	}
	selectedline = 0;
	MyInvalidate();	
}


void Traces::OnLineSelectlinebyindex(){
	if (pDoc->IsLoadingFile){
		return;
	}

	if (pDoc->dat.lines.NPoly()<1){
		Alert("no lines defined");
		return;
	}

	DiagSelectPoly diag;
	diag.NValidPolys = pDoc->dat.lines.NPoly();
	diag.selected = this->selectedline;
	if (diag.DoModal() != IDOK){
		return;
	}
	selectedline = diag.selected ;
	Invalidate();
}

void Traces::OnLineMakepolygonaroundselectedline(){
	if (pDoc->dat.p.ViewType != 2){
		return;
	}

	if (pDoc->dat.lines.NPoly()<1){
		return;
	}

	DiagMembranePolys diag;

	diag.m_IsCircular = pDoc->dat.p.ROISAreClosedInMembraneAnalysis;
	diag.m_MembraneSize = pDoc->dat.p.SizeOfMembraneInPixels;

	if (diag.DoModal()!=IDOK){
		return;
	}
	pDoc->dat.p.ROISAreClosedInMembraneAnalysis = diag.m_IsCircular;
	pDoc->dat.p.SizeOfMembraneInPixels = diag.m_MembraneSize;

	
	pDoc->dat.lines.MakePolygonAroundLine(selectedline,
		pDoc->dat.p.SizeOfMembraneInPixels, pDoc->dat.polys);

	selectedpolygon=pDoc->dat.polys.NPoly()-1;
	MyInvalidate();	


}

bool Traces::OnLineCopyselectedline(){
	if (pDoc->dat.p.ViewType != 2){
		return false;
	}

	DataFile & d = pDoc->dat;
	if (d.lines.NPoly()<1){
		Alert("No lines");
		return false;
	}

//	frame->CopiedLines.polygons.UpdateBuffer(1);
	frame->CopiedLines.polygons.resize(1);

//	PolygonObject & p = d.lines.polygons.buff[selectedline];
//	PolygonObject & pToCopyIn = frame->CopiedLines.polygons.buff[0];
	PolygonObject & p = d.lines.polygons[selectedline];
	PolygonObject & pToCopyIn = frame->CopiedLines.polygons[0];
	p.Copy(pToCopyIn);
	return true;
}
void Traces::OnLinePastecopiedline(){
	int NCopiedLines = frame->CopiedLines.NPoly();
	if (NCopiedLines <1){
		Alert("Nothing to copy");
		return;
	}
//	PolygonObject & pToPasteFrom = frame->CopiedLines.polygons.buff[0];
	PolygonObject & pToPasteFrom = frame->CopiedLines.polygons[0];

	DataFile & d = pDoc->dat;
	d.lines.AddPolygon(pToPasteFrom);
	selectedline = d.lines.NPoly()-1;

	MyInvalidate();	
}


void Traces::OnRegionofinterestMoveselectedpolywitharrowkeys(){
	if (ClickingIsTracked(true)){
		return;
	}

	if (pDoc->dat.p.ViewType != 2){
		pDoc->dat.p.ViewType = 2;
		Invalidate();
	}
	DataFile & d = pDoc->dat;
	if (d.polys.NPoly()<1){
		return;
	}
	OnMovingPolyWithArrowKey = true;
	Invalidate();

}
void Traces::OnLineMoveselectedpolywitharrowkeys(){
	if (ClickingIsTracked(true)){
		return;
	}
	if (pDoc->dat.p.ViewType != 2){
		pDoc->dat.p.ViewType = 2;
		Invalidate();
	}
	DataFile & d = pDoc->dat;
	if (d.lines.NPoly()<1){
		return;
	}
	OnMovingLineWithArrowKey = true;
	Invalidate();

}


void Traces::MoveSelectedPolyOrLine(int dx, int dy){
	PolygonObject * pPoly = NULL;
	DataFile & d = pDoc->dat;
	if (OnMovingPolyWithArrowKey){
//		pPoly = &(d.polys.polygons.buff[selectedpolygon]);
		pPoly = &(d.polys.polygons[selectedpolygon]);
	}
	if (OnMovingLineWithArrowKey){
		pPoly = &(d.lines.polygons[selectedline]);
//		pPoly = &(d.lines.polygons.buff[selectedline]);
	}
	if (!pPoly){
		return;
	}
	pPoly->ShiftPoints(dx, dy);
	Invalidate();

}

void Traces::OnRegionofinterestConverttoline(){
	DataFile & d = pDoc->dat;
	if (!OnRegionofinterestCopyselectedpolygon32850()){
		return;
	}
	OnRegionofinterestDeleteselectedpolygon(false);

//	int NLines = d.lines.polygons.GetSize();
	int NLines = d.lines.polygons.size();
	
	//	PolygonObject & p = d.polygons.polygons.buff[selectedpolygon];
//	PolygonObject & p = frame->CopiedPolygons.polygons.buff[0];
	PolygonObject & p = frame->CopiedPolygons.polygons[0];

	p.IsLine=true;
	d.lines.AddPolygon(p);
	p.IsLine=false;
	
	Invalidate();
}

void Traces::OnLineConvertselectedlinetopolygon(){
	DataFile & d = pDoc->dat;
	if (!OnLineCopyselectedline()){
		return;
	}
//	PolygonObject & p = frame->CopiedLines.polygons.buff[0];
	PolygonObject & p = frame->CopiedLines.polygons[0];
	OnLineDeleteselectedline(false);
	p.IsLine=false;
	d.polys.AddPolygon(p);
	p.IsLine=true;
	
	Invalidate();
}

void Traces::OnLineCalclength(){
	DataFile & d = pDoc->dat;
//	int NLines = d.lines.polygons.GetSize();
	int NLines = d.lines.polygons.size();
	if (NLines<1){
		Alert0("no lines");
		return;
	}
//	PolygonObject & p = d.lines.polygons.buff[selectedline];
	PolygonObject & p = d.lines.polygons[selectedline];
	ShowFloat(p.CalcLength(), "length");
}

void Traces::OnRegionofinterestCalccircumferenceofselectedpolygon(){
	DataFile & d = pDoc->dat;
//	int NPoly = d.polys.polygons.GetSize();
	int NPoly = d.polys.polygons.size();
	if (NPoly<1){
		Alert0("no polygons");
		return;
	}
//	PolygonObject & p = d.polys.polygons.buff[selectedpolygon];
	PolygonObject & p = d.polys.polygons[selectedpolygon];
	ShowFloat(p.CalcLength(), "length");

}

bool Traces::OnFillStartselectpointsinselectedpolygon(){

	if (ClickingIsTracked(true)){
		return false;
	}

	DataFile & d = pDoc->dat;
	if (!d.images) {
		Alert("no image");
		return false;
	}
	
//	int NPoly = d.polys.polygons.GetSize();
	int NPoly = d.polys.polygons.size();
	if (NPoly<1){
		Alert0("no polygons");
		return false;
	}
	SelectingFillPointsInSelectedPolygon = true;
}

void Traces::OnCountStart(){
	if (ClickingIsTracked(true)){
		return;
	}
	IsCounting = true;
}

void Traces::OnFillStopselectpointsinselectedpolygon(){
	SelectingFillPointsInSelectedPolygon = false;
}

void Traces::AddPointToCount(CPoint NewPoint){
	Counts.push_back(NewPoint);
	Invalidate();
}

void Traces::OnCountStop(){
	IsCounting = false;
}
	
void Traces::OnCountClear(){
	if (Counts.size()>0){
		if (Confirm("Sure to clear?")){
			Counts.resize(0);
			Invalidate();
		}
	}
}
void Traces::OnCountSetsize(){
	io myio;
	int OldSize = frame->SizeCountCircle;
	if (myio.InInt(OldSize, "Size of circle")!= IDOK){
		return;
	}
	frame->SizeCountCircle = OldSize;
	if (Counts.size()>0){
		Invalidate();
	}
}

void Traces::OnRegionofinterestCreatenpolygonsinside(){
	if (ClickingIsTracked(true)){
		return;
	}
	DataFile & d = pDoc->dat;

//	int NPoly = d.polys.polygons.GetSize();
	int NPoly = d.polys.polygons.size();
	if (NPoly<1){
		Alert0("no polygons");
		return;
	}
	io myio;
	int NOld = frame->NPolyInside;
	if (myio.InInt(NOld, "Number of polygons")!= IDOK){
		return;
	}
	frame->NPolyInside = abs(NOld);

	CWaitCursor dummy;
	d.polys.Createnpolygonsinside(selectedpolygon, frame->NPolyInside);
	Invalidate();

}


void Traces::OnRButtonDownCounting(UINT nFlags, CPoint point){
	if (!IsCounting){
		return;
	}
	bool found = false;
	int index = 0;
	CPoint NewPoint = point;
	CorrectClickedPointForOrigin(NewPoint);
	int x = MultZoom(NewPoint.x, Zoom, ZoomIn);
	int y = MultZoom(NewPoint.y, Zoom, ZoomIn);

	for (int i=0; i<this->Counts.size(); i++){
		CPoint p = Counts[i];
		if (abs(p.x-x)<frame->SizeCountCircle){
			if (abs(p.y-y)<frame->SizeCountCircle){
				found = true;
				index = i;
				break;
			}
		}
	}
	if (!found) return;
	if (Counts.size()<2){
		Counts.resize(0);
	}
	else{
		if (index<(Counts.size()-1)){
			Counts[index] = Counts[Counts.size()-1];
		}
		Counts.resize(Counts.size()-1);
	}
	Invalidate();
}

void Traces::OnAutoscaleButtonClicked(){
	pDoc->dat.p.AutoScale = !pDoc->dat.p.AutoScale;
	AutoScaleButton.SetCheck(pDoc->dat.p.AutoScale);
	
	Invalidate();
}

void Traces::OnMinChanged(){
	pDoc->dat.p.MinManScale = MinScaleField->GetInt();
	if (!pDoc->dat.p.AutoScale){
		Invalidate();
	}
}
void Traces::OnMaxChanged(){
	pDoc->dat.p.MaxManScale = MaxScaleField->GetInt();
	if (!pDoc->dat.p.AutoScale){
		Invalidate();
	}
}

void Traces::OnMinSliderScrolled(UINT nSBCode, UINT nPos){
//	Alert("hi");
//	ShowFloat(nPos, "pos Min");
	nPos  = MinSlider.GetPos();
	double Min, Max;
	this->GetMinMax(Min, Max);
	if (Min>=Max)return;
	double Range = Max - Min;
	double Pos = (100.0-nPos)*Range/100.0 + Min;
	pDoc->dat.p.MinManScale = Pos;
	this->MinScaleField->SetInt(Pos);
	if (pDoc->dat.p.MaxManScale < pDoc->dat.p.MinManScale){
		pDoc->dat.p.MaxManScale = pDoc->dat.p.MinManScale + 1;
		this->MaxScaleField->SetInt(pDoc->dat.p.MaxManScale);
		MaxSlider.SetPos(100.0 - 100.0*(pDoc->dat.p.MaxManScale-Min)/Range);
	}
	if (!pDoc->dat.p.AutoScale){
		Invalidate();
	}
}
void Traces::OnMaxSliderScrolled(UINT nSBCode, UINT nPos){
//	Alert("hi");
//	ShowFloat(nPos, "pos Max");
	nPos  = MaxSlider.GetPos();
	double Min, Max;
	this->GetMinMax(Min, Max);
	if (Min>=Max)return;
	double Range = Max - Min;
	double Pos = (100.0-nPos)*Range/100.0 + Min;
	pDoc->dat.p.MaxManScale = Pos;
	this->MaxScaleField->SetInt(Pos);
	if (pDoc->dat.p.MaxManScale < pDoc->dat.p.MinManScale){
		pDoc->dat.p.MinManScale = pDoc->dat.p.MaxManScale - 1;
		if (pDoc->dat.p.MinManScale<0) pDoc->dat.p.MinManScale = 0;
		this->MinScaleField->SetInt(pDoc->dat.p.MinManScale);
		MinSlider.SetPos(100.0 - 100.0*(pDoc->dat.p.MinManScale-Min)/Range);
	}
	if (!pDoc->dat.p.AutoScale){
		Invalidate();
	}
}

void Traces::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar==( (CScrollBar*)(&MinSlider))){
		OnMinSliderScrolled(nSBCode, nPos);
//		OnScrollBar(nSBCode, nPos); 
	}
	if (pScrollBar==( (CScrollBar*)(&MaxSlider))){
		OnMaxSliderScrolled(nSBCode, nPos);
//		OnScrollBar(nSBCode, nPos); 
	}
	//OnVScroll(nSBCode, nPos, pScrollBar);
}

NeighborsPixelsClass Traces::GetNeighbors(int x, int y, ImageDataType ** im, int xsize, int ysize, ComponentLabelsClass & Labels){
	NeighborsPixelsClass n;
	n.NNeighbors = 0;
	for (int dx=-1; dx<2; dx++){
		for (int dy=-1; dy<2; dy++){
			if ((dx != 0) || (dy !=0)){
				int newx = x+dx;
				int newy = y+ dy;
				if ( (newx>=0) && (newx<xsize) && (newy>=0) && (newy<ysize) ){
					if (im[newx][newy] != 0){
						n.Neighbors[n.NNeighbors].x = newx;
						n.Neighbors[n.NNeighbors].y = newy;
						n.Neighbors[n.NNeighbors].value = im[newx][newy];
						n.Neighbors[n.NNeighbors].Label = Labels.Labels[newx][newy];
						n.NNeighbors++;
					}
				}
			}
		}
	}
	return n;
}

void Traces::CheckPixel(int & StartLabel, int x, int y, ImageDataType ** im, int SX, int SY,
						ComponentLabelsClass & Labels){
	int value = im[x][y];
	if (value==0){
		return;
	}
	NeighborsPixelsClass n = GetNeighbors(x, y, im, SX, SY, Labels);
	if (n.NNeighbors == 0){
		if (Labels.Labels[x][y]<0){
			Labels.Labels[x][y] = StartLabel;
			StartLabel++;
		}
		return;
	}

	bool LabelFound = false;
	int LowestLabel = 2000000000;
	if (Labels.Labels[x][y]>=0){
		LabelFound = true;
		LowestLabel = Labels.Labels[x][y];
	}
	for (int i=0; i<n.NNeighbors; i++){
		int nx = n.Neighbors[i].x;
		int ny = n.Neighbors[i].y;
		int label = Labels.Labels[nx][ny] ;
		if (label>=0){
			if (label<LowestLabel){
				LowestLabel = label;
				LabelFound = true;
			}
		}
	}
	if (!LabelFound){
		LowestLabel = StartLabel;
		StartLabel++;
	}
	Labels.Labels[x][y] = LowestLabel;
	for (int i=0; i<n.NNeighbors; i++){
		int nx = n.Neighbors[0].x;
		int ny = n.Neighbors[0].y;
		Labels.Labels[nx][ny] = LowestLabel ;
	}
}


void Traces::OnTrackcellsDetectcells(){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (!pDoc) return;

	if (!pDoc->DataPresent){
		Alert("No data. Please open an image file.");
		return;
	}
	ImageDataType ** im = pDoc->id.data;
	CellsClass & cells = *(pDoc->pActualCells);
	int SY =  pDoc->id.ysize;
	int SX =  pDoc->id.xsize;

	ComponentLabelsClass Labels;
	Labels.Labels.resize(SX);
	for (int x=0; x<SX; x++){
		Labels.Labels[x].resize(SY);
		for (int y=0;y<SY; y++){
			Labels.Labels[x][y] = -1;
		}
	}
	CWaitCursor dummy;
	int StartLabel = 1;
	for (int round=0; round<1; round++){

		for (int x=0; x<SX; x++){
			for (int y=0;y<SY; y++){
				CheckPixel(StartLabel, x, y, im, SX, SY, Labels);

			}
		}
		for (int y=0;y<SY; y++){
			for (int x=0; x<SX; x++){
				CheckPixel(StartLabel, x, y, im, SX, SY, Labels);

			}
		}

		for (int x=0; x<SX; x++){
			for (int y=0;y<SY; y++){
				CheckPixel(StartLabel, SX-x-1, y, im, SX, SY, Labels);

			}
		}
		for (int y=0;y<SY; y++){
			for (int x=0; x<SX; x++){
				CheckPixel(StartLabel, SX-x - 1, y, im, SX, SY, Labels);

			}
		}
		for (int x=0; x<SX; x++){
			for (int y=0;y<SY; y++){
				CheckPixel(StartLabel, x, SY-y-1, im, SX, SY, Labels);

			}
		}
		for (int y=0;y<SY; y++){
			for (int x=0; x<SX; x++){
				CheckPixel(StartLabel, x , SY-y-1, im, SX, SY, Labels);

			}
		}
		for (int x=0; x<SX; x++){
			for (int y=0;y<SY; y++){
				CheckPixel(StartLabel, SX-x-1, SY-y-1, im, SX, SY, Labels);

			}
		}
		for (int y=0;y<SY; y++){
			for (int x=0; x<SX; x++){
				CheckPixel(StartLabel, SX-x-1 , SY-y-1, im, SX, SY, Labels);

			}
		}

		//ShowFloat(StartLabel, "StartLabel");
	}

	vector<int> UsedLabels;
	for (int x=0; x<SX; x++){
		for (int y=0;y<SY; y++){
			int label = Labels.Labels[x][y];
			if (label>=0){
				bool LabelFound = false;
				for (int i=0; i<UsedLabels.size(); i++){
					if (label == UsedLabels[i]){
						LabelFound = true;
						break;
					}
				}
				if (!LabelFound){
					UsedLabels.push_back(label);
				}
			}
		}
	}
	int NLabels = UsedLabels.size();

	for (int i=0; i<NLabels; i++){
		BlobPoints NewBlob;
		for (int x=0; x<SX; x++){
			for (int y=0;y<SY; y++){
				int label = Labels.Labels[x][y];
				if (label==UsedLabels[i]){
					CellPointClass cpc;
					cpc.point = CPoint(x, y);
					cpc.cellindex = 0;
					NewBlob.points.push_back(cpc);
				}
			}
		}
		cells.celllist.push_back(NewBlob);
	}

	int NCells = cells.celllist.size();

	ShowFloat(NCells, "NCells ");

	CWaitCursor dummy2;
	CDC * pDC = GetDC();
	for (int i=0; i<NCells; i++){
		char s[20];
		_itoa(i, s, 10);
		//pDC->TextOut(100, 100, "                      ");
		pDC->TextOut(100, 100, CString(s));

		cells.celllist[i].CalculateBlobBorder(SX, SY);
		int NPointsInBorder = cells.celllist[i].BlobBorder.points.size();
//		ShowFloat(NPointsInBorder, "NPointsInBorder");
		PolygonObject poly;
		vector <BorderPoint> & points = cells.celllist[i].BlobBorder.points;
		for (int i=0; i<NPointsInBorder; i++){
			//if ((i==0) || ((i>0) && ((points[i-1].x != points[i].x) || (points[i-1].y != points[i].y)))){
				CPoint p;
				p.x = points[i].x; p.y = points[i].y;
				poly.AddPoint(p);
			//}
		}
		pDoc->dat.polys.AddPolygon(poly);
	}
	ReleaseDC(pDC);

}
void Traces::CountCells( ){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (!pDoc) return;

	if (!pDoc->DataPresent){
		Alert("No data. Please open an image file.");
		return;
	}
	if (!pDoc->pActualCells){
		Alert("No cells");
		return;
	}
		
	CellsClass & cells = *(pDoc->pActualCells);
	io myio;
	if (myio.InInt(frame->MinSizeCellInPixles, "Min number of pixels per cell") != IDOK){
		return;
	}
	if (frame->MinSizeCellInPixles<1){
		Alert("must be > 0 ");
		frame->MinSizeCellInPixles=1;
		return;
	}
	CWaitCursor dummy;
	int NCells = 0;
	for (int i=0; i<cells.celllist.size(); i++){
		if (cells.celllist[i].points.size()>=frame->MinSizeCellInPixles){
			NCells++;
		}
	}
	ShowFloat(NCells, "NCells");
}


void Traces::OnTrackcellsStarttrackingcellsmode(){
	if (OnTrackingCells){
		return;
	}
	IsCounting=false;
	pDoc->dat.p.ViewType=0;
	OnTrackingCells = true;

}
void Traces::OnTrackcellsStoptrackingcellsmoded(){
	if (!OnTrackingCells){
		return;
	}
	OnTrackingCells = false;

}

void Traces::OnRButtonTrackingCells(UINT nFlags, CPoint point){
}