// AnaVisionView.cpp : implementation of the CAnaVisionView class
//

#include "stdafx.h"
#include "AnaVision.h"
#include <math.h>
#include "AnaVisionView.h"
#include "DiagFret.h"
#include "../cdib/cdib.h"
//#include "AnaVisionDoc.h"
//#include "AnaVisionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionView

IMPLEMENT_DYNCREATE(CAnaVisionView, CView)

BEGIN_MESSAGE_MAP(CAnaVisionView, CView)
	//{{AFX_MSG_MAP(CAnaVisionView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_TONE_TEST, OnToneTest)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_EN_KILLFOCUS( ID_TEXT_MINSCALE_FIELD, OnMinScaleChanged)
	ON_EN_KILLFOCUS( ID_TEXT_MAXSCALE_FIELD, OnMaxScaleChanged)

	ON_WM_RBUTTONDOWN()
//	ON_COMMAND(ID_REGIONOFINTEREST_CALCSUMVALUES, &CAnaVisionView::OnRegionofinterestCalcsumvalues)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionView construction/destruction

CAnaVisionView::CAnaVisionView()
{
	// TODO: add construction code here

	firsttime = true;
	PhasorSizeX = PhasorSizeY = 512;
	PhasorCircleG=0.5;
	PhasorCircleS=0.25;
	PhasorCircleR=0.05;
	OnMovingPhasorCircle = OnSizingPhasorCircle = false;
	GMin=-0.1;
	GMax=1.1;
	SMin=-0.1;
	SMax=0.6;


	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		SeriesSelected[i]=false;
	}
	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		PolygonSelected[i]=true;
	}
	BackgroundPolygon = -1;
	ConcatenateImages = false;

	pDoc = NULL;
	ViewType = 0;
	ViewType = 2;
//	InitialFrame0 = InitialFrame1= FrameToAnalyze0= FrameToAnalyze1=0;
	MeanInArea=0;
	ShowMode = 0;
	NMeanImages = 0;
	MeanImages = NULL;
	IndexMeanImages = NULL;

	SD=0;

	pSimFitThread = NULL;

	LastAssignedValue = 0;

}

CAnaVisionView::~CAnaVisionView()
{

	if (pSimFitThread){
//		pSimFitThread-> = NULL;
		delete pSimFitThread;
	}
	pSimFitThread = NULL;


	DeAlloc();
	DeAllocTimeArrays();
	if (IndexMeanImages){
		delete [] IndexMeanImages;
		delete [] MeanImages;
	}

	delete [] KineticPens;


}
/*
void CAnaVisionView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Beep(1000, 100);
	if (nChar == VK_ESCAPE){
		Beep(500, 20);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);


}
*/
/*
CChildFrame * CAnaVisionView::GetChildFrame(){
	return (CChildFrame *)(this->GetParent()->GetParent()->GetParent());
}
*/
BOOL CAnaVisionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionView drawing

void CAnaVisionView::Init(){
	NData=0;
	SizeArrays = 1000;
	DoAlloc();
	frame = (CMainFrame *)(AfxGetMainWnd( ));
	pDoc = (CAnaVisionDoc * ) GetDocument();


	NTimeData=0;
	SizeTimeArrays = 1000;
	DoAllocTimeArrays();

	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	blackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	greenPen.CreatePen(PS_SOLID, 1, RGB(0,255,0));

	LeftButtonDown = false;

	KineticPens = new CPen[NKINETICPENS];
	COLORREF refs[7];
	refs [0] = RGB(0,0,0);
	refs [1] = RGB(255,0,0);
	refs [2] = RGB(0,255,0);
	refs [3] = RGB(0,0,255);
	refs [4] = RGB(0,255,255);
	refs [5] = RGB(255,0,255);
	refs [6] = RGB(255,255,0);
	for (int j=0; j<NKINETICPENS; j++){
		KineticPens[j].CreatePen(PS_SOLID, 2, refs[j]);
	}

	int T=1;

	for (int i=0; i<NCHANNELSKINETICPENS; i++){
		for (int j=0; j<NKINETICPENS; j++){
			RoiPens[i][j].CreatePen(PS_SOLID, T, refs[j]);
		}
		T +=2;
	}
//			COLORREF c0 = RGB(0,0,0);

//		CPen RoiPens[NCHANNELSKINETICPENS][NKINETICPENS];


}

void CAnaVisionView::DoAlloc(){
	Mean = new double [SizeArrays];
	NValues = new int [SizeArrays];
	MeanInitialpH = new double [SizeArrays];
	RefTrace = new double [SizeArrays];
}
void CAnaVisionView::DeAlloc(){
	if (RefTrace) delete [] RefTrace;
	if (MeanInitialpH) delete [] MeanInitialpH;
	if (NValues)	delete [] NValues;
	if (Mean) delete [] Mean  ;

	NValues = 0;
	Mean = 0;
	MeanInitialpH = NULL;
}

void CAnaVisionView::AllocAnalyisArrays(int ND){

	if (ND<=SizeArrays){
		return;
	}

	DeAlloc();
	SizeArrays = ND;

	DoAlloc();

}


void CAnaVisionView::DoAllocTimeArrays(){
	time = new double [SizeTimeArrays];
	deltaH = new double [SizeTimeArrays];
}
void CAnaVisionView::DeAllocTimeArrays(){
	if (time)	delete [] time;
	if (deltaH) delete [] deltaH  ;

	time = NULL;
	deltaH = NULL;
}

void CAnaVisionView::AllocTimeAnalyisArrays(int ND){

	if (ND<SizeTimeArrays){
		return;
	}
	DeAllocTimeArrays();
	SizeTimeArrays = ND;
	DoAllocTimeArrays();
}



int CAnaVisionView::Scalex(int x){
	return offx + int (double(x)*scalex);
}
int CAnaVisionView::Scaley(double y){
	return bottom - int ((y-Min)*scaley);
}


int CAnaVisionView::ScalexTime(int x){
	return offx + int (double(x)*scalextime);
}
int CAnaVisionView::ScaleyDeltaH(double y){
	return bottom - int ((y-MinChangeProton)*scaleydeltah);
}


void CAnaVisionView::CreateButtons(){
	CRect r;

	CAnaVisionDoc* pDoc = (CAnaVisionDoc *) GetDocument();



	MinScaleField = new Text(ID_TEXT_MINSCALE_FIELD, pDoc->dat.p.MinManScale,
					0.0, 10.0, true, r, this);

	MaxScaleField = new Text(ID_TEXT_MAXSCALE_FIELD, pDoc->dat.p.MaxManScale,
					0.0, 10.0, true, r, this);

}
void CAnaVisionView::MoveButtons(CDC * pDC, double Min, double Max, bool autoscale){

	MinScaleField->SetFloat(Min);
	MaxScaleField->SetFloat(Max);


	if ((ShowMode == 0) && ((ViewType!=3) && (ViewType!=4) )){
		MinScaleField->ShowWindow(SW_SHOW);
		MaxScaleField->ShowWindow(SW_SHOW);
	}
	else{
		MinScaleField->ShowWindow(SW_HIDE);
		MaxScaleField->ShowWindow(SW_HIDE);
	}



	CRect cr;
	GetClientRect(&cr);

	CRect r = cr;
	r.left = 1;
	r.right = r.left + 100;

	r.top = 1;
	r.bottom = r.top + 20;

	MaxScaleField->MoveWindow(&r);

	r.top = cr.bottom - 20;
	r.bottom = cr.bottom;
	
	MinScaleField->MoveWindow(&r);

	MinScaleField->DoEnable(!autoscale);
	MaxScaleField->DoEnable(!autoscale);
}

void CAnaVisionView::OnDraw(CDC* pDC)
{
	if (firsttime){

		firsttime = false;
		DataFile & d = pDoc->dat;
		if (d.NImages>0){
			pDoc->ReadFile(d.images[d.ImageIndex].CurrentIndex);
		}
		Init();
		CreateButtons();
		OnAnalyzeThisimage();
//		CheckViewMenu();

		DWORD dwStyle = ES_LEFT | ES_MULTILINE | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL  | WS_VSCROLL;
		ResultsEdit.Create(dwStyle, EditRect, this, 110);
		ResultsEdit.ShowWindow(SW_SHOW);
		pcf = (CChildFrame *)(this->GetParent()->GetParent()->GetParent());
		//pTool = new CToolTipCtrl();
		//pTool->Create(this);

	}

	GetClientRect(&EditRect);

	CheckViewMenu();
//	pDoc->UsePhasorMask = false;
	CRect cr;
	GetClientRect(&cr);
	this->bottomPhasor = cr.bottom;
	int sizex = cr.right-cr.left+1;
	if (sizex<10) return;
	int sizey = bottomPhasor-cr.top+1;
	if (sizey<10) return;

	OffsetXPhasor=1;
	OffsetYPhasor=1;
	while ((sizex-2*OffsetXPhasor)<1){
		OffsetXPhasor--;
	}
	while ((sizey-2*OffsetYPhasor)<1){
		OffsetYPhasor--;
	}
	int sizeXPhasorRect = cr.right-OffsetXPhasor;
	int sizeYPhasorRect = bottomPhasor-OffsetYPhasor;

	NSizeGPhasor = 8;
	while (2*NSizeGPhasor<sizeXPhasorRect){
		NSizeGPhasor *= 2;
	}
	NSizeSPhasor = 8;
	while (2*NSizeSPhasor<sizeYPhasorRect){
		NSizeSPhasor *= 2;
	}

	NPixelGPhasor = NSizeGPhasor;
	while (NPixelGPhasor>256){
		NPixelGPhasor /=2;
	}
	NPixelSPhasor = NSizeSPhasor;
	while (NPixelSPhasor>128){
		NPixelSPhasor /=2;
	}
	fG=NSizeGPhasor/NPixelGPhasor;
	fS=NSizeSPhasor/NPixelSPhasor;

	ScaleXPhasor = NSizeGPhasor/(GMax-GMin);
	ScaleYPhasor = NSizeSPhasor/(SMax-SMin);
//	ShowFloat(NSizeGPhasor, "NG");
//	ShowFloat(NSizeSPhasor, "NS");

	if (ViewType != 7){
		OnMovingPhasorCircle = OnSizingPhasorCircle= false;
	}
	CAnaVisionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->IsLoadingFile){
		return;
	}

	//	ShowFloat(ViewType, "VT");
	if (ShowMode == 0){
		ResultsEdit.ShowWindow(SW_HIDE);

	}
	else{
		ResultsEdit.ShowWindow(SW_SHOW);
	}

	if (ViewType == 1){
//		frame->p.AutoScale = TRUE;
		DrawDeltaH(pDC);
//		return;
	}
	if (ViewType == 0){
//		frame->p.AutoScale = TRUE;
		DrawGradient(pDC, Mean, &blackPen);
//		return;
	}
	if (ViewType == 2){ // Mean values
		DrawMeanValues(pDC);
//		return;
	}
	if (ViewType == 3){ // Kinetics as function of time
		DrawKinetics(pDC, true, SD);
//		return;
	}
	if (ViewType == 4){ // Kinetics as function of radius
		DrawKinetics(pDC, false, SD);
//		return;
	}
	if (ViewType == 5){ // mean values of ROI's
		DrawMeanROIS(pDC);
	}
	if (ViewType == 6){ // fill area
		DrawFillArea(pDC);
	}
	if (ViewType == 7){ // fill area
		DrawPhasors(pDC);
	}
	if (ShowMode == 0){
//		CChildFrame * pcf = GetChildFrame();
		if (pcf->IsSimFitting){
			CPen * OldPen = pDC->SelectObject(&redPen);
			pDC->Ellipse(20-4,60-4, 20+4,60+4);
			pDC->Ellipse(20-6,60-6,20+6,60+6);
			pDC->Ellipse(20-4,60-4, 20+4,60+4);
			pDC->SelectObject(OldPen);
		}

	}

}

void CAnaVisionView::DrawMeanValues(CDC * pDC){
	if (ShowMode==1){
		DrawMeanValuesText(pDC);
		return;
	}
	return;
}

void CAnaVisionView::DrawMeanValuesText(CDC * pDC){
	if (NMeanImages <1) return;
	if (MeanImages == NULL) return;
	if (IndexMeanImages==NULL) return;

	CRect rect;
	GetWindowRect(&rect);
	ResultsEdit.GetClientRect(&EditRect);
	EditRect.left = 0;
	EditRect.right = rect.right-rect.left;
	EditRect.top=0;//20+ 7*textMetric.tmHeight;
	EditRect.bottom = rect.bottom-rect.top;
	
	ResultsEdit.MoveWindow(&EditRect, true);
	CString EditText = CString("");


	for (int i=0; i<NMeanImages; i++){
		char s[20];
		_itoa(IndexMeanImages[i], s, 10);
		CString c = CString(s)+CString("\t");
		_gcvt(MeanImages[i], 6, s);
		c += CString(s)+CString("\t");
		EditText += c+CString("\r\n");
	}
		
	ResultsEdit.SetWindowText(EditText);
	ResultsEdit.LineScroll( 0);
	MoveButtons(pDC, Min, Max, pDoc->dat.p.AutoScale == true);


}


void CAnaVisionView::DrawGradientText(CDC * pDC, double * Mean){

	CRect rect;
	GetWindowRect(&rect);
	ResultsEdit.GetClientRect(&EditRect);
	EditRect.left = 0;
	EditRect.right = rect.right-rect.left;
	EditRect.top=0;//20+ 7*textMetric.tmHeight;
	EditRect.bottom = rect.bottom-rect.top;
	
	ResultsEdit.MoveWindow(&EditRect, true);
	CString EditText = CString("");

	double length = pDoc->dat.p.drawob()->Length();
	double R1, R2;

	pDoc->dat.p.drawob()->GetMidPointAndRadius(Vect(0,0), R1, R2);

	R1 *= pDoc->dat.p.PixelSizeInMicrom/pDoc->dat.p.Magnification*1e-6;
	R2 *= pDoc->dat.p.PixelSizeInMicrom/pDoc->dat.p.Magnification*1e-6;

	double DR = (R2-R1)/double(NData);

	double r=R1;

	for (int i=0; i<NData; i++){
		char s[20];
		_itoa(i, s, 10);
		CString c = CString(s)+CString("\t");
		_gcvt(r, 6, s);
		c += CString(s)+CString("\t");
		_gcvt(Mean[i], 6, s);
		c += CString(s)+CString("\r\n");
		EditText += c;
		r += DR;
	}
		
	ResultsEdit.SetWindowText(EditText);
	ResultsEdit.LineScroll( 0);
	MoveButtons(pDC, Min, Max, pDoc->dat.p.AutoScale == true);

}
void CAnaVisionView::DrawCurs(CDC * pDC, CPen & pen, int x){
	CRect rect;
	GetClientRect(&rect);
//	CPen * OldPen = pDC->SelectObject(&redPen);
	CPen * OldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(x, rect.top);
	pDC->LineTo(x, rect.bottom);
	pDC->SelectObject(OldPen);
}

void CAnaVisionView::DrawGradient(CDC * pDC, double * Mean, CPen * ppen){

	if (ShowMode==1){
		DrawGradientText(pDC, Mean);
		return;
	}

	if (NData<2){
		pDC->TextOut(10, 10, "NData < 2");
		return ;
	}


	CalcScales(pDoc->dat.p.AutoScale == true, Mean);
	
	CRect rect;

	GetClientRect(&rect);

	offx = 100;
	int	offy = 5;

	bottom = rect.bottom - offy;

	scalex = double(rect.right-rect.left-offx)/double(NData+2);

	scaley = double(rect.bottom-rect.top-offy)/(Max-Min);

	MoveButtons(pDC, Min, Max, pDoc->dat.p.AutoScale == true);

	int i;

	char s[20];
	_gcvt(MeanInArea, 8, s);
	pDC->TextOut(150, 1, CString("Mean = ")+CString(s));

//	ShowFloat(Max, "Max");
	pDC->MoveTo(Scalex(0), Scaley(Min));
	pDC->LineTo(Scalex(0), Scaley(Max));
	pDC->MoveTo(Scalex(0), Scaley(Min));
	pDC->LineTo(Scalex(NData-1), Scaley(Min));

//	_gcvt(Min, 5, s);
//	pDC->TextOut(1,rect.bottom-20, CString(s));

//	_gcvt(Max, 5, s);
//	pDC->TextOut(1,rect.top+2, CString(s));

	pDC->MoveTo(Scalex(0), Scaley(Mean[0]));

	CPen * OldPen = pDC->SelectObject(ppen);

	for (i=1; i<NData; i++){
//		ShowFloat(Scaley(Mean[i]), "mean");
		pDC->LineTo(Scalex(i), Scaley(Mean[i]));
	}

	pDC->SelectObject(OldPen);

	for (i=0; i<3; i++){
		CPen * pen = &redPen;
		if (i>1){
			pen = &bluePen;
		}
		DrawCurs(pDC, *pen, Scalex( int (pDoc->dat.p.curspos[i]*double(NData-1)) ));
	}
}

int CAnaVisionView::Scalexk(double x){
//	ShowFloat(x, "x");
//	ShowFloat(offsetxk, "offsetxk");
//	ShowFloat(minxk, "minxk");
//	ShowFloat(scalexk, "scalexk");

	return offsetxk + int(  scalexk*(x-minxk) );
}
int CAnaVisionView::Scaleyk(double y){
	return offsetyk - int ( scaleyk*(y-minyk) );
}

void CAnaVisionView::DrawKinetics(CDC * pDC, bool ShowAsFunctionOfTime, double SD){

	Kinetics & k = pDoc->dat.k;
	if (!k.HasBeenConstructed){
		pDC->TextOut(100, 100, "Please construct the kinetics first");
		return;
	}

	k.ShowAsFunctionOfTime = ShowAsFunctionOfTime;

	MoveButtons(pDC, Min, Max, pDoc->dat.p.AutoScale == true);

//		double scalexk, scaleyk;
//	int offsetxk, offsetyk;

	k.GetMaxMin(minxk, maxxk, minyk, maxyk);
//	ShowFloat(minxk, "minxk");
//	ShowFloat(maxxk, "maxxk");
//	ShowFloat(minyk, "minyk");
//	ShowFloat(maxyk, "maxyk");


	CRect r;
	GetClientRect(&r);
	
	offsetxk = 49;
	scalexk = double(r.right - 54)/(maxxk - minxk);

	offsetyk = r.bottom-20;
	scaleyk =  double (r.bottom - 24)/(maxyk - minyk);

//	ShowFloat(Scalexk(minxk), "Scalex(minx)");
///	ShowFloat(offsetxk, "offsetx");
	int iminxk = Scalexk(minxk);
	int imaxxk = Scalexk(maxxk);
	int iminyk = Scaleyk(minyk);
	int imaxyk = Scaleyk(maxyk);
	
	pDC->MoveTo(iminxk, int(minyk));
	pDC->LineTo(iminxk, int(maxyk));
	pDC->LineTo(imaxxk, int(maxyk));
	pDC->LineTo(imaxxk, int(minyk));
	pDC->LineTo(iminxk, int(minyk));

	if ((minxk<0) && (maxxk>0)){
		pDC->MoveTo(Scalexk(0), Scaleyk(minyk));
		pDC->LineTo(Scalexk(0), Scaleyk(maxyk));
	}
	if ((minyk<0) && (maxyk>0)){
		pDC->MoveTo(Scalexk(minxk), Scaleyk(0));
		pDC->LineTo(Scalexk(maxxk), Scaleyk(0));
	}

	CPen * OldPen = pDC->SelectObject(&KineticPens[0]);

	int ncurves = k.NumberOfCurves();
//	ShowFloat(ncurves, "nc");
	int npointspercurve=k.NPointsPerCurve();
//	ShowFloat(npointspercurve, "np");

	int curve;
	int ipen = 0;
	for (curve=0; curve<ncurves; curve++){
		pDC->SelectObject(&KineticPens[ipen]);
		ipen = (ipen + 1) % NKINETICPENS;

		for (int point=0; point<npointspercurve; point++){
			double x, y;
			k.GetPoint(curve, point, x, y, ShowAsFunctionOfTime, false);
//			ShowFloat(x, "x");
//			ShowFloat(y, "y");

			int ix=Scalexk(x);
			int iy=Scaleyk(y);
			int sizec=2;
			pDC->Ellipse(ix-sizec, iy-sizec, ix+sizec,iy+sizec);
		}
	}

	ipen = 0;
	if (k.HasBeenFitted){
		for (curve=0; curve<ncurves; curve++){
			pDC->SelectObject(&KineticPens[ipen]);
			ipen = (ipen + 1) % NKINETICPENS;
			double x, y;
			int ix, iy;
			k.GetPoint(curve, 0, x, y, ShowAsFunctionOfTime, true);
			ix=Scalexk(x);
			iy=Scaleyk(y);
			pDC->MoveTo(ix, iy);
			for (int point=1; point<(npointspercurve); point++){
				k.GetPoint(curve, point, x, y, ShowAsFunctionOfTime, true);
				ix=Scalexk(x);
				iy=Scaleyk(y);
				pDC->LineTo(ix, iy);
			}
		}
		char s[20];
		_gcvt(k.Current*1e6, 8, s);
		CString c = CString("Current ")+CString(s)+CString(" microA");
		pDC->TextOut(500,50,c);
		_gcvt(k.D, 8, s);
		c = CString("D       ")+CString(s)+CString(" m^2/s");
		pDC->TextOut(500,70,c);
		_gcvt(SD, 8, s);
		c = CString("SD       ")+CString(s);
		pDC->TextOut(500,90,c);
		if (k.FitType>=2){
			_gcvt(k.AmountFixed, 8, s);
			c = CString("AmountFixed   ")+CString(s)+CString(" moles");
			pDC->TextOut(500,110,c);
			_gcvt(k.KFixed, 8, s);
			c = CString("KFixed ")+CString(s)+CString(" M");
			pDC->TextOut(500,130,c);
		}
	}
	else{
//		Alert("not fitted");
	}
	char s[20];
	_gcvt(minxk, 5, s);
	pDC->TextOut(iminxk+100, iminyk, CString(s));
	_gcvt(minyk, 5, s);
	pDC->TextOut(iminxk, iminyk, CString(s));
	_gcvt(maxxk, 5, s);
	pDC->TextOut(imaxxk-100, iminyk, CString(s));
	_gcvt(maxyk, 5, s);
	pDC->TextOut(iminxk, imaxyk, CString(s));


}

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionView printing

BOOL CAnaVisionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAnaVisionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAnaVisionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionView diagnostics

#ifdef _DEBUG
void CAnaVisionView::AssertValid() const
{
	CView::AssertValid();
}

void CAnaVisionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnaVisionDoc* CAnaVisionView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnaVisionDoc)));
	return (CAnaVisionDoc*)m_pDocument;
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CAnaVisionView message handlers

void CAnaVisionView::AnalyzeImage(double & CursorMean){
	ThisMean = 0;
	CursorMean = 0;
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	if (!pDoc) return;
//	if (!pDoc->currentimage){
	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
//		Alert("no image");
		return;
	}

	AreaObject * drawob = pDoc->dat.p.drawob();

	NData = int(drawob->Length());

	int i1curs = int(pDoc->dat.p.curspos[0]*double(NData-1));
	int i2curs = int(pDoc->dat.p.curspos[1]*double(NData-1));
	int i3curs = int(pDoc->dat.p.curspos[2]*double(NData-1));

	AllocAnalyisArrays(NData);

	int i;
	for (i=0; i<NData; i++){
		Mean[i]=0;
		NValues[i] = 0;
	}

	CPoint p;

	int index;


//	int XX= pDoc->dat.p.xsizeimage;
//	int YY= pDoc->dat.p.ysizeimage;

//	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
//	ImageData & id = pApp->id;

	int XX= pDoc->id.xsize;//dat.p.xsizeimage;
	int YY= pDoc->id.ysize; //dat.p.ysizeimage;

//	int XX= id.xsize;//dat.p.xsizeimage;
//	int YY= id.ysize; //dat.p.ysizeimage;

//	double ** im = pDoc->currentimage;
	ImageDataType ** im = pDoc->id.data;//currentimage;

	int NDataTotal = 0;
	bool Initialize = true;
//	while (false){
/*
	while (pDoc->dat.p.drawob()->GeneratePoints(Initialize, p, index)){
		if ((p.x<0) || (p.x>639)|| (p.y<0) || (p.y>479)|| (index<0) || (index>=SizeArrays)){
			ShowFloat(index, "index");
			ShowFloat(NDataTotal, "NDataTotal");
			ShowFloat(p.x, "x");
			ShowFloat(p.y, "y");
		}
		NDataTotal ++;
	}
	ShowFloat(NDataTotal, "NDT");
*/
	NDataTotal = 0;
	Initialize = true;

	while (pDoc->dat.p.drawob()->GeneratePoints(Initialize, p, index)){

/*
		if ((p.x<0) || (p.x>639)|| (p.y<0) || (p.y>479)|| (index<0) || (index>=SizeArrays)){
			ShowFloat(index, "index");
			ShowFloat(NDataTotal, "NDataTotal");
			ShowFloat(p.x, "x");
			ShowFloat(p.y, "y");
		}
*/
		Mean[index] += im[p.x][p.y];
		NValues[index] ++;
		ThisMean +=  im[p.x][p.y];
		NDataTotal ++;
	}


	if (NDataTotal>0){
		ThisMean /= double(NDataTotal);
	}

	MeanInArea = 0.0;


	int nmean=0;

	int ncursmean = 0;

	for (i=0; i<NData; i++){
		int nv = NValues[i];
		if (nv>0){
			double r = Mean[i];

			MeanInArea += r;
			nmean += nv;
			r /= double(nv);
			Mean[i] = r;
			if (i>=i1curs){
				if (i<=i2curs){
					CursorMean += r;
					ncursmean++;
				}
			}
		}
	}
	if (nmean) MeanInArea /= double(nmean);
	if (ncursmean) CursorMean /= double(ncursmean);

}


void CAnaVisionView::OnAnalyzeThisimage(){
	double CursorMean;
	AnalyzeImage(CursorMean);
//	ViewType = 0;
//	CheckViewMenu();
	MyInvalidate();
}

void CAnaVisionView::CalcScales(bool autoscale, double * Mean){
	int i;
	if (autoscale){
		Max = -1e300;
		Min = 1e300;

		for (i=0; i<NData; i++){
			if (NValues[i]>0){
				double r = Mean[i];
				if (r > Max) Max = r;
				if (r < Min) Min = r;
			}
		}
//		ShowFloat(Min, "Min");
	}
	else{
		Min = pDoc->dat.p.MinManScale;
		Max = pDoc->dat.p.MaxManScale;
	}
}

void CAnaVisionView::CalcTimeScales(){
	MaxChangeProton = MaxDouble(deltaH, NTimeData);
	MinChangeProton = MinDouble(deltaH, NTimeData);
}


void CAnaVisionView::OnMinScaleChanged(){
	pDoc->dat.p.MinManScale = MinScaleField->GetFloat();
	MyInvalidate();
}
void CAnaVisionView::OnMaxScaleChanged(){
	pDoc->dat.p.MaxManScale = MaxScaleField->GetFloat();
	MyInvalidate();
}

void CAnaVisionView::OnAnalyzeGetnumberofprotons() 
{
	// TODO: Add your command handler code here
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (!pDoc) return;

//	if (!pDoc->currentimage){
	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
		Alert("No data. Please open an image file.");
		return;
	}


	GetFramesDiag diag;
	diag.p = pDoc->dat.p;

	if (diag.DoModal() != IDOK){
		return;
	}
	pDoc->dat.p = diag.p;
	CheckFrames();

	AnalyzeProtonAccumulation();

	ViewType = 1;
	CheckViewMenu();

	MyInvalidate();

}
void CAnaVisionView::OldAnalyzeProtonAccumulation(){
	CWaitCursor dummy;

	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	DataFile & d = pDoc->dat;
	int oldindex = d.images[d.ImageIndex].CurrentIndex;

	NTimeData = pDoc->dat.p.FrameToAnalyze1 - pDoc->dat.p.FrameToAnalyze0 + 1;

	AllocTimeAnalyisArrays(NTimeData);
    
	int i, j;

	for (i=0; i<NData; i++) {
		MeanInitialpH[i]=0;
	}


	CDC * pDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, pDC->GetCurrentBrush());

//	CWaitCursor dummy;

	double MeanInitialCursorMean = 0;
//	pDoc->dat.p.drawob()->CalcGeneratorList();
	for (i=pDoc->dat.p.InitialFrame0; i<=pDoc->dat.p.InitialFrame1; i++){
//		Sleep(100);
		if (!pDoc->ReadFile(i)){
			Alert(" could not read file");
			return;	
		}
//		ShowFloat(i, "i");
		double CursorMean;
		AnalyzeImage(CursorMean);
		MeanInitialCursorMean += CursorMean;

//		ShowFloat(i, "i");
		for ( j=0; j<NData; j++){
			if (NValues[j]>0){
				MeanInitialpH[j] += Mean[j];
			}
		}
		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
	}
}

void CAnaVisionView::AnalyzeProtonAccumulation(){

	CWaitCursor dummy;

	int delta = pDoc->dat.p.DeltaImage;
	if (pDoc->dat.p.StepByOne){
		delta = 1;
	}

	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	DataFile & d = pDoc->dat;

	int oldindex = d.images[d.ImageIndex].CurrentIndex;

	NTimeData = (pDoc->dat.p.FrameToAnalyze1 - pDoc->dat.p.FrameToAnalyze0 + 1)/delta;

	AllocTimeAnalyisArrays(NTimeData);
    
	int i;

	for (i=0; i<NData; i++) {
		MeanInitialpH[i]=0;
	}


	CDC * pDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, pDC->GetCurrentBrush());
	pDC->TextOut(1, 100, CString("ESC TO STOP"));

//	CWaitCursor dummy;

	double MeanInitialCursorMean = 0;
//	pDoc->dat.p.drawob()->CalcGeneratorList();
	for (i=pDoc->dat.p.InitialFrame0; i<=pDoc->dat.p.InitialFrame1; i++){
		if (InKey()==ESCAPEKEY){
			ReleaseDC(pDC);
			return;
		}

		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
		if (!pDoc->ReadFile(i)){			// Reads and converts the frame according to the BCECF calibration
			Alert(" could not read file");
			return;	
		}
		double CursorMean;
		AnalyzeImage(CursorMean);	// This calculates the gradient of this frame and puts the
								// result in double * Mean!
		MeanInitialCursorMean += CursorMean;

		DrawGradient(pDC, Mean, &blackPen);

		for (int j=0; j<NData; j++){
			if (NValues[j]>0){
				MeanInitialpH[j] += Mean[j];
			}
		}
	}
	double scale = pDoc->dat.p.InitialFrame1 - pDoc->dat.p.InitialFrame0 + 1;
	for (int j=0; j<NData; j++){
		MeanInitialpH[j] /= scale;
	}
	MeanInitialCursorMean /= scale;

	
	double length = pDoc->dat.p.drawob()->Length();
	double R1, R2;

	pDoc->dat.p.drawob()->GetMidPointAndRadius(Vect(0,0), R1, R2);

	R1 *= pDoc->dat.p.PixelSizeInMicrom/pDoc->dat.p.Magnification*1e-6;
	R2 *= pDoc->dat.p.PixelSizeInMicrom/pDoc->dat.p.Magnification*1e-6;

	double DR = (R2-R1)/double(NData);

	pDC->TextOut(10, 50, CString("                 "));
	for (i=pDoc->dat.p.FrameToAnalyze0; i<=pDoc->dat.p.FrameToAnalyze1; i += delta){
		if (InKey()==ESCAPEKEY){
			ReleaseDC(pDC);
			return;
		}

		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
		int integertime = i-pDoc->dat.p.FrameToAnalyze0;
		int timeindex = integertime/delta;
		time[timeindex]=integertime*pDoc->dat.p.dt; // in seconds
		
		if (!pDoc->ReadFile(i)){			// Reads and converts the frame according to the BCECF calibration
			Alert(" could not read file"); 
			return;	
		}

		double CursorMean;
		AnalyzeImage(CursorMean);			// This calculates the gradient of this frame and puts the
								// result in double * Mean!

		if (pDoc->dat.p.TrackDrift){
			double difference = MeanInitialCursorMean - CursorMean;
			for (int j=0; j<NData; j++){
				Mean[j] += difference;
			}
		

		}
		DrawGradient(pDC, MeanInitialpH, &redPen);
		DrawGradient(pDC, Mean, &blackPen);


		// Now integrate the protons over the shells

		double r1 = R1;			// this is the current "smaller" radius
		double Vol1 = r1*r1*r1;	// and it's correspondig sphere volume.Scaling is done at the end
		double DH = 0;			// The cumulative change in proton number

		int i1curs = int(pDoc->dat.p.curspos[0]*double(NData-1));
//		ShowFloat(i1curs, "i1curs");
		for (int j=0; j<i1curs; j++){
		
			double r2 = r1 + DR;	// the current larger radius
			double Vol2 = r2*r2*r2;	// and it's volume
			double value0 = MeanInitialpH[j];	// the starting pH in this shell
			if (NValues[j]>0){					// Use only if there are data!
				if (fabs(value0)>0.1){			// Dont use strange initial pH
//				if (true){			// Dont use strange initial pH
					double value1 = Mean[j];
					if (fabs(value1)>0.1){		// Same for data
//					if (true){		// Same for data
						DH += (Vol2 - Vol1)*pDoc->dat.p.ConvertpHToProton(value0, value1);
//						if (i==(pDoc->dat.p.FrameToAnalyze0+delta)) ShowFloat(value1-value0, "v1-v0");
							// concentration change * Vol = quantity!
					}
				}
			}

			r1 = r2;
			Vol1 = Vol2;
		}
		deltaH[timeindex]=	DH* 4.0*PIGRECO*FARADAY/3.0; // in Coulomb

	}

	pDoc->ReadFile(oldindex);
	pDC->TextOut(1, 100, CString("           "));

	ReleaseDC(pDC);
}


void CAnaVisionView::DrawDeltaHText(CDC * pDC){

	CRect rect;
	GetWindowRect(&rect);
	ResultsEdit.GetClientRect(&EditRect);
	EditRect.left = 0;
	EditRect.right = rect.right-rect.left;
	EditRect.top=0;//20+ 7*textMetric.tmHeight;
	EditRect.bottom = rect.bottom-rect.top;
	
	ResultsEdit.MoveWindow(&EditRect, true);
	CString EditText = CString("");

	for (int i=0; i<NTimeData; i++){
		char s[20];
		_itoa(i, s, 10);
		CString c = CString(s)+CString("\t");
		_gcvt(time[i], 6, s);
		c += CString(s)+CString("\t");
		_gcvt(deltaH[i], 6, s);
		c += CString(s)+CString("\r\n");
		EditText += c;
	}
		
	ResultsEdit.SetWindowText(EditText);
	ResultsEdit.LineScroll( 0);
	MoveButtons(pDC, MinChangeProton, MaxChangeProton, true);

}

void CAnaVisionView::DrawDeltaH(CDC * pDC){
	if (ShowMode == 1){
		DrawDeltaHText(pDC);
		return;
	}


	if (NTimeData<1){
		pDC->TextOut(10, 10, "NTimeData < 1");
		return ;
	}

	CalcTimeScales();

	CRect rect;

	GetClientRect(&rect);

	offx = 100;
	int	offy = 5;

	bottom = rect.bottom - offy;

	scalextime = double(rect.right-rect.left-offx)/double(NTimeData);

	scaleydeltah = double(rect.bottom-rect.top-offy)/(MaxChangeProton-MinChangeProton);

	MoveButtons(pDC, MinChangeProton, MaxChangeProton, true);

//	ShowFloat(Max, "Max");
	pDC->MoveTo(ScalexTime(0), ScaleyDeltaH(MinChangeProton));
	pDC->LineTo(ScalexTime(0), ScaleyDeltaH(MaxChangeProton));
	pDC->MoveTo(ScalexTime(0), ScaleyDeltaH(MinChangeProton));
	pDC->LineTo(ScalexTime(NTimeData-1), ScaleyDeltaH(MinChangeProton));

//	char s[20];
//	_gcvt(Min, 5, s);
//	pDC->TextOut(1,rect.bottom-20, CString(s));

//	_gcvt(Max, 5, s);
//	pDC->TextOut(1,rect.top+2, CString(s));

	if (NTimeData == 1){
		int x=ScalexTime(0);
		int y = ScaleyDeltaH(deltaH[0]);
		pDC->Ellipse(x-5, y-5, x+5, y+5);
	}
	else{
		pDC->MoveTo(ScalexTime(0), ScaleyDeltaH(deltaH[0]));
		int i;
		for (i=1; i<NTimeData; i++){
	//		ShowFloat(Scaley(Mean[i]), "mean");
			pDC->LineTo(ScalexTime(i), ScaleyDeltaH(deltaH[i]));
		}
	}
}


void CAnaVisionView::OnAnalyzeOverlaygradientseveralframes(Traces * pT, bool exportimages, bool exportgradients ){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	if (!pDoc) return;

//	if (!pDoc->currentimage){
	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
		Alert("No data. Please open an image file.");
		return;
	}

	CString templatestring = pDoc->GetPathName()+CString("0000.txt");

	if (exportimages){
		CFileDialog fdiag( false, NULL, templatestring, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);
		fdiag.m_ofn.lpstrTitle = "Export images on ASCII text files";
		if (fdiag.DoModal()!= IDOK){
			return;
		}
		templatestring = fdiag.GetPathName();
		templatestring = templatestring.Left(templatestring.GetLength()-8);
	}

	CString templatestringgradients = pDoc->GetPathName()+CString("0000.asc");

	if (exportgradients){
		CFileDialog fdiag( false, NULL, templatestringgradients, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);
		fdiag.m_ofn.lpstrTitle = "Export gradients on ASCII text files";
		if (fdiag.DoModal()!= IDOK){
			return;
		}
		templatestringgradients = fdiag.GetPathName();
		templatestringgradients = templatestringgradients.Left(templatestringgradients.GetLength()-8);
	}
	

/*
	if (exportgradients){
		CFileDialog fdiaggradient( FALSE, NULL, GradientExportName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);
	
		fdiaggradient.m_ofn.lpstrTitle = "Export gradients on ASCII file";
				
		if (fdiaggradient.DoModal()!= IDOK){
			return;
		}
		GradientExportName = fdiaggradient.GetPathName();
		gradientfile.Open(GradientExportName, CFile::modeCreate | CFile::modeWrite);
	}
*/

	GetFramesDiag diag;
//	diag.DisableInitialFrames();
	diag.p = pDoc->dat.p;
	{
		for ( int j=0; j<NData; j++){
				MeanInitialpH[j] = 0;
		}
	}



	if (diag.DoModal() != IDOK){
		return;
	}

	pDoc->dat.p = diag.p;
	CheckFrames();

	DataFile & d = pDoc->dat;
	int oldindex = d.images[d.ImageIndex].CurrentIndex;
//	int oldindex = pDoc->dat.CurrentIndex;

	CDC * pDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, pDC->GetCurrentBrush());
	pDC->TextOut(1, 100, CString("ESC TO STOP"));

	CWaitCursor dummy;

	int i;

	double MeanInitialCursorMean = 0;
	pDoc->id.refcalculated = false;

	ImageDataType ** dd = pDoc->id.data;

//	pDoc->dat.p.drawob()->CalcGeneratorList();

	if (pDoc->dat.p.TrackDrift){
//		Alert0("hi");
		ImageDataType ** meanref = pDoc->id.meanrefdata;
		for (i=pDoc->dat.p.InitialFrame0; i<=pDoc->dat.p.InitialFrame1; i++){
//		Sleep(100);
			if (!pDoc->ReadFile(i)){
				Alert(" could not read file");
				return;	
			}
			if (pDoc->dat.p.SubtractInitialValuesForDraw){
				if (i==pDoc->dat.p.InitialFrame0){
					for (int ix=0; ix<pDoc->id.xsize; ix++){
						for (int iy=0; iy<pDoc->id.ysize; iy++){
							meanref[ix][iy] = 0.0;
						}
					}
				}
				for (int ix=0; ix<pDoc->id.xsize; ix++){
					for (int iy=0; iy<pDoc->id.ysize; iy++){
						meanref[ix][iy] += dd[ix][iy] ;
					}
				}
			}

//			ShowFloat(i, "i");
			double CursorMean;
			AnalyzeImage(CursorMean);
			MeanInitialCursorMean += CursorMean;

//			ShowFloat(i, "i");
			for ( int j=0; j<NData; j++){
				if (NValues[j]>0){
						MeanInitialpH[j] += Mean[j];
				}
			}
			char s[20];
			_itoa(i, s, 10);
			pDC->TextOut(10, 50, CString(s));
		}

		double navg=pDoc->dat.p.InitialFrame1-pDoc->dat.p.InitialFrame0+1;
		for ( int j=0; j<NData; j++){
				MeanInitialpH[j] /= navg;
//				ShowFloat(MeanInitialpH[j], "mean ini");
		}

		MeanInitialCursorMean /= navg;

		if (pDoc->dat.p.SubtractInitialValuesForDraw){
			for (int ix=0; ix<pDoc->id.xsize; ix++){
				for (int iy=0; iy<pDoc->id.ysize; iy++){
					meanref[ix][iy] /= float(navg) ;
				}
			}
			pDoc->id.refcalculated = true;
		}
	}


	int delta = pDoc->dat.p.DeltaImage;
	if (pDoc->dat.p.StepByOne){
		delta = 1;
	}
	int imageindex=0;

	for (i=pDoc->dat.p.FrameToAnalyze0; i<=pDoc->dat.p.FrameToAnalyze1; i += delta){
		if (InKey()==ESCAPEKEY){
			ReleaseDC(pDC);
			return;
		}
		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
		if (!pDoc->ReadFile(i)){
			Alert(" could not read file");
			ReleaseDC(pDC);
			return;	
		}
		double CursorMean;
		AnalyzeImage(CursorMean);
		if (pDoc->dat.p.TrackDrift){
			double difference = MeanInitialCursorMean - CursorMean;
			for (int j=0; j<NData; j++){
				Mean[j] += difference;
			}
		}

		double * gradient = Mean;
		if (pDoc->dat.p.SubtractInitialValuesForDraw){
			for (int j=0; j<NData; j++){
				RefTrace[j] = Mean[j] - MeanInitialpH[j];
			}
			DrawGradient(pDC, RefTrace, &blackPen);
			gradient = RefTrace;

		}
		else{
			if (pDoc->dat.p.TrackDrift) DrawGradient(pDC, MeanInitialpH, &redPen);
			DrawGradient(pDC, Mean, &blackPen);
			gradient = Mean;
		}
/*
		if (exportgradients){
			ExportGradient(gradientfile, RefTrace);
		}
*/
		if (exportgradients){
			for (int j=0; j<NData; j++){
				gradient[j] = gradient[j]*pDoc->dat.p.MultiplierImages+pDoc->dat.p.OffsetImages;
			}

			CString filename = templatestringgradients;
			char sss[20];
			_itoa(imageindex, sss, 10);
			CString extend=CString(sss);
			int l=extend.GetLength();
			if (l<2){
				extend = "000"+extend;
			}
			else{
				if (l<3){
					extend = "00"+extend;
				}
				else{
					if (l<4){
						extend = "0"+extend;
					}
				}
			}

			filename += extend + ".asc";
//			Alert(filename);

			CFile file;
			file.Open(filename, CFile::modeCreate | CFile::modeWrite);
			ExportGradient(file, gradient);
		}

		if (exportimages){
			CString filename = templatestring;
			char sss[20];
			_itoa(imageindex, sss, 10);
			CString extend=CString(sss);
			int l=extend.GetLength();
			if (l<2){
				extend = "000"+extend;
			}
			else{
				if (l<3){
					extend = "00"+extend;
				}
				else{
					if (l<4){
						extend = "0"+extend;
					}
				}
			}

			filename += extend + ".txt";
//			Alert(filename);

			CFile file;
			file.Open(filename, CFile::modeCreate | CFile::modeWrite);
//			pT->DrawCell(NULL, true, &file);
			pT->DrawCellTest(NULL, true, &file);
		}
		else{
//			if (!exportgradients){
//				pT->DrawCell(NULL, false, NULL);
				pT->DrawCellTest(NULL, false, NULL);
//			}

		}
		imageindex ++;


	}

	pDC->TextOut(1, 100, CString("                       "));
 
	ReleaseDC(pDC);

	pDoc->ReadFile(oldindex);

}

void CAnaVisionView::OnViewViewgradientthisimage(){
	ViewType = 0;
	CheckViewMenu();
	MyInvalidate();
}
void CAnaVisionView::OnViewVienumberofprotons(){
	ViewType = 1;
	CheckViewMenu();
	MyInvalidate();
}



void CAnaVisionView::CheckViewMenu(){

	if (firsttime) return;

    CMainFrame * f = (CMainFrame *)(AfxGetMainWnd());

	CMenu * pmenu = f->GetMenu();

	if (pmenu==NULL){
		return;
	}

	CMDIChildWnd * pC1 = f->MDIGetActive();
	CMDIChildWnd * pC2 = (CMDIChildWnd * )(this->GetParent()->GetParent()->GetParent());

	if (pC1 != pC2){
//		return;
	}

	DoCheckMenuItem(ViewType==0, pmenu, ID_VIEW_VIEWGRADIENTTHISIMAGE);
	DoCheckMenuItem(ViewType==1, pmenu, ID_VIEW_VIENUMBEROFPROTONS);
	DoCheckMenuItem(ViewType==2, pmenu, ID_VIEW_VIEWMEANVALUES);
	DoCheckMenuItem(ViewType==3, pmenu, ID_VIEW_VIEWKINETICS);
	DoCheckMenuItem(ViewType==4, pmenu, ID_VIEW_VIEWKINETICSASFUNCTIONOFRADIUS);
	DoCheckMenuItem(ViewType==5, pmenu, ID_VIEW_VIEWMEANVALUESINPOLYGONS);
	DoCheckMenuItem(ViewType==6, pmenu, ID_VIEW_VIEWFILLEDAREA);
	DoCheckMenuItem(ViewType==7, pmenu, ID_VIEW_VIEWPHASORS);

	DoCheckMenuItem(ShowMode==0, pmenu, ID_VIEW_SHOWGRAPH);
	DoCheckMenuItem(ShowMode==1, pmenu, ID_VIEW_SHOWTEXT);
}


void CAnaVisionView::OnViewShowtext() 
{
	// TODO: Add your command handler code here
	if (this->ShowMode != 1){
		ShowMode = 1;
		MyInvalidate();
	}
	CheckViewMenu();
	
}

void CAnaVisionView::OnViewShowgraph(){
	if (this->ShowMode != 0){
		ShowMode = 0;
		MyInvalidate();
	}
	CheckViewMenu();
}

bool CAnaVisionView::CursorClicked(int curs, CPoint p){
	if (ShowMode != 0){
		return false;
	}
	if (ViewType != 0){
		return false;
	}
	int x = Scalex( int (pDoc->dat.p.curspos[curs]*double(NData-1)) );
	if (abs(p.x-x)<4){
		return true;
	}
	return false;

}

void CAnaVisionView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	if (ViewType == 7){
		OnLButtonDownPhasor(nFlags, point);
		return;
	}
	int i;
	for (i=0; i<3; i++){
		if (CursorClicked(i, point)){
			LeftButtonDown = true;
			CursorToMove = i;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CAnaVisionView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (ViewType == 7){
		OnMouseMovePhasor(nFlags, point);
		return;
	}
	CView::OnMouseMove(nFlags, point);
	if (!LeftButtonDown){
		return;
	}
	CDC * pDC = GetDC();
	CPen * pen = &redPen;
	if (CursorToMove>1){
		pen = &bluePen;
	}
	DrawCurs(pDC, *pen, point.x);
	ReleaseDC(pDC);
}

void CAnaVisionView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (ViewType==7){
		OnLButtonUpPhasor(nFlags, point);
		return;
	}
	CView::OnLButtonUp(nFlags, point);
	if (!LeftButtonDown){
		return;
	}
	LeftButtonDown = false;
	if (!pDoc) return;

	double x =  double (point.x-offx)/scalex/double(NData-1);
	pDoc->dat.p.curspos[CursorToMove] = x;

	pDoc->dat.p.CheckCursors();

	MyInvalidate();

}

void CAnaVisionView::CheckFrames(){

	DataFile & d = pDoc->dat;
	Params & p = d.p;


//	int ntotal = pDoc->dat.LastIndex-pDoc->dat.FirstIndex+1;
	int ntotal = d.images[d.ImageIndex].LastIndex-d.images[d.ImageIndex].FirstIndex+1;

	if (p.InitialFrame0>=ntotal){
		p.InitialFrame0=ntotal-1;

//		Alert("InitialFrame0>=ntotal");
//		return;
	}
	if (p.InitialFrame1>=ntotal){
		p.InitialFrame1=ntotal-1;
//		Alert("InitialFrame1>=ntotal");
//		return;
	}
	if (p.FrameToAnalyze0>=ntotal){
		p.FrameToAnalyze0=ntotal-1;
//		Alert("AnalysisFrame0>=ntotal");
//		return;
	}
	if (p.FrameToAnalyze1>=ntotal){
		p.FrameToAnalyze1=ntotal-1;
//		Alert("AnalysisFrame1>=ntotal");
//		return;
	}

	if (p.InitialFrame0<0){
		p.InitialFrame0 = 0;
//		Alert("InitialFrame0<0");
	}
	if (p.InitialFrame1<0){
		p.InitialFrame1 = 0;
//		Alert("InitialFrame1<0");
	}
	if (p.InitialFrame0>p.InitialFrame1){
		p.InitialFrame0=p.InitialFrame1;
	}
	if (p.FrameToAnalyze0<0){
		p.FrameToAnalyze0 = 0;
//		Alert("AnalysisFrame0<0");
//		return;
	}
	if (p.FrameToAnalyze1<0){
		p.FrameToAnalyze1 = 0;
//		Alert("AnalysisFrame1<0");
//		return;
	}

	if (p.FrameToAnalyze0>p.FrameToAnalyze1){
		p.FrameToAnalyze0=p.FrameToAnalyze1;
//		Alert("AnalysisFrame0>diag.m_AnalysisFrame1");
//		return;
	}

	if (p.DeltaImage<1){
		p.DeltaImage = 1;
	}



}

void CAnaVisionView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

//	frame->p = pDoc->dat.p;
	
}
void CAnaVisionView::MyInvalidate(){
//	frame->p = pDoc->dat.p;
	Invalidate();
}

void CAnaVisionView::OnFileExportgradients(){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

//	if (!pDoc->currentimage){
	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
		Alert("No data. Please open an image file.");
		return;
	}

	GetFramesDiag diag;
	diag.DisableInitialFrames();
	diag.p = pDoc->dat.p;

	if (diag.DoModal() != IDOK){
		return;
	}

	pDoc->dat.p = diag.p;
	CheckFrames();

	DataFile & d = pDoc->dat;
	int oldindex = d.images[d.ImageIndex].CurrentIndex;
//	int oldindex = pDoc->dat.CurrentIndex;

	CDC * pDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, pDC->GetCurrentBrush());
	pDC->TextOut(1, 100, CString("ESC TO STOP"));

	int i;
	int delta = pDoc->dat.p.DeltaImage;
	if (pDoc->dat.p.StepByOne){
		delta = 1;
	}
	CString Name = pDoc->GetPathName();
	Name += "_.asc";
	
	CFileDialog fdiag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);
	
	fdiag.m_ofn.lpstrTitle = "Export on ASCII file";
				
	if (fdiag.DoModal()!= IDOK){
		return;
	}
	Name = fdiag.GetPathName();
	CFile file;
	file.Open(Name, CFile::modeCreate | CFile::modeWrite);

	CWaitCursor dummy;
//	pDoc->dat.p.drawob()->CalcGeneratorList();
	double MeanInitialCursorMean = 0;
	if (pDoc->dat.p.TrackDrift){
		for (i=pDoc->dat.p.InitialFrame0; i<=pDoc->dat.p.InitialFrame1; i++){
//		Sleep(100);
			if (!pDoc->ReadFile(i)){
				Alert(" could not read file");
				return;	
			}
//		ShowFloat(i, "i");
		double CursorMean;
			AnalyzeImage(CursorMean);
			MeanInitialCursorMean += CursorMean;

//		ShowFloat(i, "i");
			for ( int j=0; j<NData; j++){
				if (NValues[j]>0){
					MeanInitialpH[j] += Mean[j];
				}
			}
			char s[20];
			_itoa(i, s, 10);
			pDC->TextOut(10, 50, CString(s));
		}
	}


	for (i=pDoc->dat.p.FrameToAnalyze0; i<=pDoc->dat.p.FrameToAnalyze1; i += delta){
		if (InKey()==ESCAPEKEY){
			ReleaseDC(pDC);
			return;
		}
		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
		if (!pDoc->ReadFile(i)){
			Alert(" could not read file");
			ReleaseDC(pDC);
			return;	
		}
		double CursorMean;
		AnalyzeImage(CursorMean);
		if (pDoc->dat.p.TrackDrift){
			double difference = MeanInitialCursorMean - CursorMean;
			for (int j=0; j<NData; j++){
				Mean[j] += difference;
			}
		}

		ExportGradient(file, Mean);
	}

	pDC->TextOut(1, 100, CString("                       "));
 
	ReleaseDC(pDC);

	pDoc->ReadFile(oldindex);

}
void CAnaVisionView::ExportGradient(CFile & f, double * Mean){

	CString EditText = CString("");

	double length = pDoc->dat.p.drawob()->Length();
	double R1, R2;

	pDoc->dat.p.drawob()->GetMidPointAndRadius(Vect(0,0), R1, R2);

	R1 *= pDoc->dat.p.PixelSizeInMicrom/pDoc->dat.p.Magnification*1e-6;
	R2 *= pDoc->dat.p.PixelSizeInMicrom/pDoc->dat.p.Magnification*1e-6;

	double DR = (R2-R1)/double(NData);

	double r=R1;

	for (int i=0; i<NData; i++){
		char s[20];
		_itoa(i, s, 10);
		CString c = CString(s)+CString("\t");
		_gcvt(r, 6, s);
		c += CString(s)+CString("\t");
		_gcvt(Mean[i], 6, s);
		c += CString(s)+CString("\r\n");
		EditText += c;
		r += DR;
	}
	int l = EditText.GetLength();
	f.Write(EditText.GetBuffer(l), l);

}

#define AAXX(xx) 	WriteStringOnFile(CString(xx), f); WriteEndOfLine(f);

void CAnaVisionView::OnGenerateNewmacrosZmSuc(){
	CWaitCursor dummy;
	// Make Move Files
	int NFiles = 89;
//	CString templatestr = "C:/Documents and Settings/mik/Desktop/transfer/2007/0407/270407/r2x/hclc-5-270407-2.vws_r2x.ana00";
//	CString macrostr = "hclc-5-270407-2.vws_r2x.ana00";
	CString templatestr = "C:/Documents and Settings/mik/Desktop/transfer/2007/0307/150307/images/zm-suc-150307-4.vws_.ana00";
	CString macrostr = "zm-suc-150307-4.vws_.ana00";

	CString macrodir = "c:/programmi/imagej/macros/";
	double data[90][500];
	double r[90][500];
	int npoints = 0;

	int i;
	CString imagefiles[89];
	CString gradientfiles[89];
	CString movefilesgray[89];
	CString movefilespink[89];
	CString macrofiles[89];

	for (i=1; i<=NFiles; i++){
		char s[20];
		_itoa(i, s, 10);
		CString extend= CString(s);
		if (i<10){
			extend = "0"+extend;
		}
		imagefiles[i-1]=templatestr+extend+".txt";
//		Alert(imagefiles[i-1]);
		gradientfiles[i-1]=templatestr+extend+".asc";
//		movefiles[i-1]=templatestr+extend+"_mov.txt";
//		macrofiles[i-1]=templatestr+extend+"_m.txt";
		movefilesgray[i-1]=macrostr+extend+"_mov_gray.txt";
		movefilespink[i-1]=macrostr+extend+"_mov_pink.txt";
		macrofiles[i-1]=macrostr+extend+"_m.txt";

		FILE * ff = fopen(gradientfiles[i-1], "r");
		int j=0;
		while (true){
			double d;
			if (!ReadDouble(d, ff)) break;
			if (!ReadDouble(r[i][j], ff)) break;;
			if (!ReadDouble(data[i][j], ff)) break;;
			j++;
		}
		npoints = j+1;
//		if (i==NFiles) ShowFloat(npoints, "NP");
//		Alert(gradientfiles[i-1]);
	}

	double MinDH=-60;
	double MaxDH=5;

	double x0=60;
	double x1=630;

	double y0=20;
	double y1=180;

	double scalex=(x1-x0)/double(npoints);

	double scaley=(y1-y0)/(MaxDH-MinDH);

	for (i=1; i<=NFiles; i++){
		CFile f;
		CString movestr=macrodir+movefilesgray[i-1];
		f.Open(movestr, CFile::modeCreate | CFile::modeWrite);
		AAXX("setColor(110,110,110);");
		AAXX("setLineWidth(2);");
		for (int j=0; j<(npoints-1); j++){

			double d = (-data[i][j]-MinDH)/(MaxDH-MinDH);
//			if (d<0) d=0;
//			if (d>1) d=1;
			double x = x0 + (x1-x0)*double(j)/double(npoints);
			double y = y1 - d*(y1-y0);
			int ix=int(x);
			int iy=int(y);
			char s[20];
			_itoa(ix, s, 10);
			CString c = CString(s);
			_itoa(iy, s, 10);
			c += ","+CString(s)+CString(");");
			if (j==0){
				c = CString("moveTo(") + c;
			}
			else{
				c = CString("lineTo(") + c;
			}
			AAXX(c);
		}
	}
	for (i=1; i<=NFiles; i++){
		CFile f;
		CString movestr=macrodir+movefilespink[i-1];
		f.Open(movestr, CFile::modeCreate | CFile::modeWrite);
		AAXX("setColor(200,0,200);");
		AAXX("setLineWidth(4);");
		for (int j=0; j<(npoints-1); j++){

			double d = (-data[i][j]-MinDH)/(MaxDH-MinDH);
//			if (d<0) d=0;
//			if (d>1) d=1;
			double x = x0 + (x1-x0)*double(j)/double(npoints);
			double y = y1 - d*(y1-y0);
			int ix=int(x);
			int iy=int(y);
			char s[20];
			_itoa(ix, s, 10);
			CString c = CString(s);
			_itoa(iy, s, 10);
			c += ","+CString(s)+CString(");");
			if (j==0){
				c = CString("moveTo(") + c;
			}
			else{
				c = CString("lineTo(") + c;
			}
			AAXX(c);
		}
	}
	for (i=1; i<=NFiles; i++){
		CString macrostr=macrodir+macrofiles[i-1];
		CFile f;
		f.Open(macrostr, CFile::modeCreate | CFile::modeWrite);

		CString c = "run(\"Text Image... \", \"open=[";
		c += imagefiles[i-1];
		c += "]\")";
		AAXX(c);

//		AAXX("setMinAndMax(0, 12000);");
		AAXX("setMinAndMax(0, 6000);");
		AAXX("run(\"brgbcmyw\");");

//		AAXX("makeOval(-180,340,960,800);");
		AAXX("makeOval(-185,330,960,800);");
		AAXX("setColor(10,10,10);");
		AAXX("fill();");

		AAXX("makeRectangle(0,0,640,200);");
		AAXX("setColor(255,255,255);");
		AAXX("fill();");

		AAXX("makeRectangle(0,200,640,10);");
		AAXX("setColor(0,255,0);");
		AAXX("fill();");


		AAXX("moveTo(20, 205);");
		AAXX("setLineWidth(10);");
		if ((i<10) || (i>=60)){
			AAXX("setColor(0,0,255);");
		}
		else{
			AAXX("setColor(255,0,0);");
		}


		AAXX("setColor(0,0,255);");
		int i1=0;	int i2=0; 	int i3=0;
		if (i<10){
			i1=i;
		}
		else{
			i1=10;
			if (i<60){
				i2=i;
			}
			else{
				i2=60;
				i3=i;
			}
		}
		double lengthline1 = 600.0*double(i1-1)/double(NFiles);
		double lengthline2 = 600.0*double(i2-1)/double(NFiles);
		double lengthline3 = 600.0*double(i3-1)/double(NFiles);

		int ll=int(lengthline1);
		char s[20];
		_itoa(ll,s,10);
		c = CString("lineTo(20+")+CString(s)+CString(", 205);");
		AAXX(c);
		if (i>10){
			AAXX("setColor(255,0,0);");
			int ll=int(lengthline2);
			char s[20];
			_itoa(ll,s,10);
			c = CString("lineTo(20+")+CString(s)+CString(", 205);");
			AAXX(c);
			if (i>60){
				AAXX("setColor(0,0,255);");
				int ll=int(lengthline3);
				char s[20];
				_itoa(ll,s,10);
				c = CString("lineTo(20+")+CString(s)+CString(", 205);");
				AAXX(c);
			}
		}
		_itoa(int(x0), s, 10);
		CString cx0=CString(s);
		_itoa(int(x1), s, 10);
		CString cx1=CString(s);
		_itoa(int(y0), s, 10);
		CString cy0=CString(s);
		_itoa(int(y1), s, 10);
		CString cy1=CString(s);

		AAXX("setColor(0,0,0);");
		AAXX("setLineWidth(3);");

		c=CString("moveTo(")+cx0+","+cy0+");";
		AAXX(c);
		c=CString("lineTo(")+cx1+","+cy0+");";
		AAXX(c);

		c=CString("moveTo(")+cx0+","+cy0+");";
		AAXX(c);
		c=CString("lineTo(")+cx0+","+cy1+");";
		AAXX(c);

	//	AAXX("setColor(200,200,200);");
	//	AAXX("setLineWidth(2);");

		for (int j=1; j<i; j++){
			CString c = CString("runMacro(\"")+movefilesgray[j-1]+CString( "\");");
			AAXX(c);
		}
	//	AAXX("setLineWidth(10);");
	//	AAXX("setColor(255,0,0);");
		c = CString("runMacro(\"")+movefilespink[i-1]+CString("\");");
		AAXX(c);


		AAXX("drawString(\"total [H+]\", 2, 90);");
//		AAXX("drawString(\"120 �M\", 12, 180);");
		AAXX("drawString(\"60 �M\", 12, 180);");
//		AAXX("drawString(\"260 �m\", 580, 15);");
		AAXX("drawString(\"270 �m\", 580, 15);");
		AAXX("drawString(\"0 �m\", 40, 15);");
		
		c = imagefiles[i-1]+"_.bmp";
		c="saveAs(\"bmp\",\"" + c + "\");";

		AAXX(c);

		AAXX("close();");

	}
	CString fname = "C:/Documents and Settings/mik/Desktop/transfer/2007/0307/150307/images/000.txt";

//	CString fname = "C:/Documents and Settings/mik/Desktop/transfer/2007/0407/270407/r2x/000.txt";

	CFile f;

	f.Open(fname, CFile::modeCreate | CFile::modeWrite);

	for (i=1; i<=NFiles; i++){
		CString macrostr=macrofiles[i-1];
		CString c = "runMacro(\""+macrostr+"\");";
		AAXX(c);
	}


}
void CAnaVisionView::OnGenerateNewmacros(){
	CWaitCursor dummy;
	// Make Move Files
	int NFiles = 89;
	CString templatestr = "C:/Documents and Settings/mik/Desktop/transfer/2007/0407/270407/r2x/hclc-5-270407-2.vws_r2x.ana00";
	CString macrostr = "hclc-5-270407-2.vws_r2x.ana00";
//	CString templatestr = "C:/Documents and Settings/mik/Desktop/transfer/2007/0307/150307/images/zm-suc-150307-4.vws_.ana00";
//	CString macrostr = "zm-suc-150307-4.vws_.ana00";

	CString macrodir = "c:/programmi/imagej/macros/";
	double data[90][500];
	double r[90][500];
	int npoints = 0;

	int i;
	CString imagefiles[89];
	CString gradientfiles[89];
	CString movefilesgray[89];
	CString movefilespink[89];
	CString macrofiles[89];

	for (i=1; i<=NFiles; i++){
		char s[20];
		_itoa(i, s, 10);
		CString extend= CString(s);
		if (i<10){
			extend = "0"+extend;
		}
		imagefiles[i-1]=templatestr+extend+".txt";
//		Alert(imagefiles[i-1]);
		gradientfiles[i-1]=templatestr+extend+".asc";
//		movefiles[i-1]=templatestr+extend+"_mov.txt";
//		macrofiles[i-1]=templatestr+extend+"_m.txt";
		movefilesgray[i-1]=macrostr+extend+"_mov_gray.txt";
		movefilespink[i-1]=macrostr+extend+"_mov_pink.txt";
		macrofiles[i-1]=macrostr+extend+"_m.txt";

		FILE * ff = fopen(gradientfiles[i-1], "r");
		int j=0;
		while (true){
			double d;
			if (!ReadDouble(d, ff)) break;
			if (!ReadDouble(r[i][j], ff)) break;;
			if (!ReadDouble(data[i][j], ff)) break;;
			j++;
		}
		npoints = j+1;
//		if (i==NFiles) ShowFloat(npoints, "NP");
//		Alert(gradientfiles[i-1]);
	}

//	double MinDH=-60;
	double MinDH=120;
//	double MaxDH=5;
	double MaxDH=-5;

	double x0=60;
	double x1=630;

	double y0=20-15;
	double y1=180-15;

	double scalex=(x1-x0)/double(npoints);

	double scaley=(y1-y0)/(MaxDH-MinDH);

	for (i=1; i<=NFiles; i++){
		CFile f;
		CString movestr=macrodir+movefilesgray[i-1];
		f.Open(movestr, CFile::modeCreate | CFile::modeWrite);
		AAXX("setColor(110,110,110);");
		AAXX("setLineWidth(2);");
		for (int j=0; j<(npoints-1); j++){

			double d = (-data[i][j]-MinDH)/(MaxDH-MinDH);
//			if (d<0) d=0;
//			if (d>1) d=1;
			double x = x0 + (x1-x0)*double(j)/double(npoints);
			double y = y0 + d*(y1-y0);
			int ix=int(x);
			int iy=int(y);
			char s[20];
			_itoa(ix, s, 10);
			CString c = CString(s);
			_itoa(iy, s, 10);
			c += ","+CString(s)+CString(");");
			if (j==0){
				c = CString("moveTo(") + c;
			}
			else{
				c = CString("lineTo(") + c;
			}
			AAXX(c);
		}
	}
	for (i=1; i<=NFiles; i++){
		CFile f;
		CString movestr=macrodir+movefilespink[i-1];
		f.Open(movestr, CFile::modeCreate | CFile::modeWrite);
		AAXX("setColor(200,0,200);");
		AAXX("setLineWidth(4);");
		for (int j=0; j<(npoints-1); j++){

			double d = (-data[i][j]-MinDH)/(MaxDH-MinDH);
//			if (d<0) d=0;
//			if (d>1) d=1;
			double x = x0 + (x1-x0)*double(j)/double(npoints);
			double y = y0 + d*(y1-y0);
			int ix=int(x);
			int iy=int(y);
			char s[20];
			_itoa(ix, s, 10);
			CString c = CString(s);
			_itoa(iy, s, 10);
			c += ","+CString(s)+CString(");");
			if (j==0){
				c = CString("moveTo(") + c;
			}
			else{
				c = CString("lineTo(") + c;
			}
			AAXX(c);
		}
	}
	for (i=1; i<=NFiles; i++){
		CString macrostr=macrodir+macrofiles[i-1];
		CFile f;
		f.Open(macrostr, CFile::modeCreate | CFile::modeWrite);

		CString c = "run(\"Text Image... \", \"open=[";
		c += imagefiles[i-1];
		c += "]\")";
		AAXX(c);

//		AAXX("setMinAndMax(0, 12000);");
		AAXX("setMinAndMax(0, 8000);");
		AAXX("run(\"brgbcmyw\");");

//		AAXX("makeOval(-180,340,960,800);");
		AAXX("makeOval(-185,330,960,800);");
		AAXX("setColor(10,10,10);");
		AAXX("fill();");

		AAXX("makeRectangle(0,0,640,200);");
		AAXX("setColor(255,255,255);");
		AAXX("fill();");

		AAXX("makeRectangle(0,200,640,10);");
		AAXX("setColor(0,255,0);");
		AAXX("fill();");


		AAXX("moveTo(20, 205);");
		AAXX("setLineWidth(10);");
		if ((i<10) || (i>=60)){
			AAXX("setColor(0,0,255);");
		}
		else{
			AAXX("setColor(255,0,0);");
		}


		AAXX("setColor(0,0,255);");
		int i1=0;	int i2=0; 	int i3=0;
		if (i<10){
			i1=i;
		}
		else{
			i1=10;
//			if (i<60){
			if (i<40){
				i2=i;
			}
			else{
				i2=40;
				i3=i;
			}
		}
		double lengthline1 = 600.0*double(i1-1)/double(NFiles);
		double lengthline2 = 600.0*double(i2-1)/double(NFiles);
		double lengthline3 = 600.0*double(i3-1)/double(NFiles);

		int ll=int(lengthline1);
		char s[20];
		_itoa(ll,s,10);
		c = CString("lineTo(20+")+CString(s)+CString(", 205);");
		AAXX(c);
		if (i>10){
			AAXX("setColor(255,0,0);");
			int ll=int(lengthline2);
			char s[20];
			_itoa(ll,s,10);
			c = CString("lineTo(20+")+CString(s)+CString(", 205);");
			AAXX(c);
			if (i>40){
				AAXX("setColor(0,0,255);");
				int ll=int(lengthline3);
				char s[20];
				_itoa(ll,s,10);
				c = CString("lineTo(20+")+CString(s)+CString(", 205);");
				AAXX(c);
			}
		}
		_itoa(int(x0), s, 10);
		CString cx0=CString(s);
		_itoa(int(x1), s, 10);
		CString cx1=CString(s);
		_itoa(int(y0), s, 10);
		CString cy0=CString(s);
		_itoa(int(y1), s, 10);
		CString cy1=CString(s);

		AAXX("setColor(0,0,0);");
		AAXX("setLineWidth(3);");

		c=CString("moveTo(")+cx0+","+cy1+");";
		AAXX(c);
		c=CString("lineTo(")+cx1+","+cy1+");";
		AAXX(c);

		c=CString("moveTo(")+cx0+","+cy1+");";
		AAXX(c);
		c=CString("lineTo(")+cx0+","+cy0+");";
		AAXX(c);

	//	AAXX("setColor(200,200,200);");
	//	AAXX("setLineWidth(2);");

		for (int j=1; j<i; j++){
			CString c = CString("runMacro(\"")+movefilesgray[j-1]+CString( "\");");
			AAXX(c);
		}
	//	AAXX("setLineWidth(10);");
	//	AAXX("setColor(255,0,0);");
		c = CString("runMacro(\"")+movefilespink[i-1]+CString("\");");
		AAXX(c);


		AAXX("drawString(\"total [H+]\", 2, 90);");
		AAXX("drawString(\"120 �M\", 12, 18);");
//		AAXX("drawString(\"60 �M\", 12, 180);");
		AAXX("drawString(\"260 �m\", 580, 185);");
//		AAXX("drawString(\"270 �m\", 580, 15);");
		AAXX("drawString(\"0 �m\", 40, 185);");
		
		c = imagefiles[i-1]+"_.bmp";
		c="saveAs(\"bmp\",\"" + c + "\");";

		AAXX(c);

		AAXX("close();");

	}
//	CString fname = "C:/Documents and Settings/mik/Desktop/transfer/2007/0307/150307/images/000.txt";

	CString fname = "C:/Documents and Settings/mik/Desktop/transfer/2007/0407/270407/r2x/000.txt";

	CFile f;

	f.Open(fname, CFile::modeCreate | CFile::modeWrite);

	for (i=1; i<=NFiles; i++){
		CString macrostr=macrofiles[i-1];
		CString c = "runMacro(\""+macrostr+"\");";
		AAXX(c);
	}


}

void CAnaVisionView::OnGenerateMacro(){
	CWaitCursor dummy;
//	CString InFile = "c:/transfer/data/data.asc";
	CString InFile = "c:/mich/pp/ventura-2006/data.asc";
	FILE * ff = fopen(InFile, "r");
	if (!ff) return;

#define NTRACES 100
#define NDATARADIUS 142
	double data[NTRACES][NDATARADIUS];

	{
		for (int i=0; i<NTRACES; i++){
			for (int j=0; j<NDATARADIUS; j++){
				double d;
				ReadDouble(d, ff);
				ReadDouble(d, ff);
				ReadDouble(data[i][j], ff);
			}
		}
	}
	fclose(ff);



	int x0=50;
	int x1=590;
	int y0=20;
	int y1=150;

	double pHMin = 7.1;
	double pHMax = 7.3;

	CString fnames[NTRACES];

	for (int ii=0; ii<NTRACES; ii++){
		fnames[ii] = "c:/mich/pp/ventura-2006/tif-norm/m";
		fnames[ii] = "m";
		char s[20];
		_itoa(ii, s, 10);
		fnames[ii] += CString(s)+".txt";
		CString name = "c:/programmi/imagej/macros/"+fnames[ii];
		{
			CFile f;
			f.Open(name, CFile::modeCreate | CFile::modeWrite);
			AAXX("strii = getArgument();");
			AAXX("i=parseInt(strii);");
			AAXX("if (i==255){ setColor(255, 255, 255); setLineWidth(2);}else{setColor(150,150,150);}");

				
//			AAXX("setColor(i,i,i);");
			for (int j=0; j<NDATARADIUS; j++){
				double d = (data[ii][j]-pHMin)/(pHMax-pHMin);
				if (d<0) d=0;
				if (d>1) d=1;
				double x = double(x0) + double(x1-x0)*double(j)/double(NDATARADIUS);
				double y = double(y1) - d*double(y1-y0);
				int ix=int(x);
				int iy=int(y);
				char s[20];
				_itoa(ix, s, 10);
				CString c = CString(s);
			_itoa(iy, s, 10);
				c += ","+CString(s)+CString(");");
				if (j==0){
					c = CString("moveTo(") + c;
				}
				else{
					c = CString("lineTo(") + c;
				}
				AAXX(c);
			}

		}
	}


	CString macrofile = "c:/mich/pp/ventura-2006/tif-norm/aaatest.txt";
	CFile f;
	f.Open(macrofile, CFile::modeCreate | CFile::modeWrite);

    AAXX("myreds = newArray(256);") 
    AAXX("mygreens = newArray(256);" )
    AAXX("myblues = newArray(256);")
	AAXX("getLut(myreds, mygreens, myblues);")
	AAXX("getLut(reds, greens, blues);")

	AAXX("print (reds[100]);")

	AAXX("for (i=0; i<256; i++){")
	AAXX("  myreds[i]=reds[i];")
	AAXX("	mygreens[i]=greens[i];")
	AAXX("  myblues[i]=blues[i];")
	AAXX("}")

	int i;
	AAXX("setFont(\"SansSerif\", 16, \"bold\");")
	for (i=0; i<100; i = i+1){
		AAXX("	setColor(0,0,0);")

		CString str = "\"c:/mich/pp/ventura-2006/tif-norm/a3";
		if (i<99) {
			str = str + "0";
		};
		if (i<9){
			str= str + "0";
		}

		char s[20];
		_itoa(i+1, s, 10);
		str += CString(s);
		str += ".tif\"";

		str = "str = " + str;

		AAXX(str);
		AAXX(" print (str);")
		AAXX(" open(str);")
		AAXX("  setLut(myreds, mygreens, myblues);")
		AAXX("  l=lengthOf(str);")

//		AAXX(" setBatchMode(true);")
		AAXX(" autoUpdate(false);")
		AAXX(" for (x=0; x<640; x++){")
//		AAXX("    print (x);")
		AAXX("     for (y=0; y<480; y++){")
		AAXX("	v = 10*(getPixel(x,y)-12000);")
		AAXX("	if (v<0){")
		AAXX("		 v=0;")
		AAXX("	}")
		AAXX("	setPixel(x, y, v);")
		AAXX("     }")
		AAXX("  }")
		AAXX(" updateDisplay();")
		AAXX("  str = substring(str, 0, l-3)+\"bmp\";")
		AAXX("  print (str);")
//		AAXX("  y0=200;")
		AAXX("  y0=250;")
		AAXX("  x0=50;")
		AAXX("  fillRect(0,0,640,y0);")
		AAXX("  fillRect(0,y0,x0,480-y0);")
		AAXX("  fillRect(640-x0,y0,x0,480-y0);")


		AAXX("  x0=50;")
		AAXX("  x1=590;")
		AAXX("  y0=20;")
		AAXX("  y1=150;")

		AAXX("  setColor(255, 255, 255);")
		AAXX("  moveTo(x0, y0);")
		AAXX("  lineTo(x0, y1);")
		AAXX("  lineTo(x1, y1);")

//		AAXX("  drawString(\"7.1\", x0-20, y1);");
//		AAXX("  drawString(\"7.3\", x0-20, y0+10);");
		AAXX("  drawString(\"7.1\", x0-30, y1);");
		AAXX("  drawString(\"7.3\", x0-30, y0+10);");
		AAXX("  drawString(\"pH\", x0-30, (y1+y0)/2);");

//		AAXX("  drawString(\"0.42 mm\", x0, y1+20);");
//		AAXX("  drawString(\"0.56 mm\", x1-20, y1+20);");
		AAXX("  drawString(\"0 �m\", x0, y1+30);");
		AAXX("  drawString(\"140 �m\", x1-20, y1+30);");
		AAXX("  drawString(\"distance\", (x0+x1)/2-50, y1+30);");

		double time = double(i+1)*0.1;
		double maxtime = double(NTRACES+1)*0.1;
		
		int lengthtimebar = int ( double(x1-x0)*time/maxtime);

//		AAXX("  setColor(0, 0, 255);")
		AAXX("  setColor(0, 255, 0);")
		AAXX("  setLineWidth(10);");
		_itoa(lengthtimebar, s, 10);
		CString a = s;
		CString b = s;
		a = "drawLine(x0+10, y1+50, x0 + 10 + " + a + ", y1+50);";
		AAXX(a);

		_gcvt(time, 4, s);
		a = CString(s) + " s";

//		a = "drawString(\"" + a + "\", x0 - 34  " +  ", y1 + 37);";
		a = "drawString(\"" + a + "\", x0 - 40  " +  ", y1 + 57);";
		AAXX(a);



		AAXX("  setLineWidth(1);");
		AAXX("  setColor(150, 150, 150);")
		for (int ii=0; ii<=i; ii++){
			CString aa="\"150\"";
			aa="\"150\"";
			aa="\"150\"";
			if (ii==i){
				aa="\"255\"";
				aa="\"255\"";
//				AAXX("  setColor(255, 255, 255);")
			}
			CString c = fnames[ii];
			c = "runMacro(\"" + c + "\", " + aa +  ");";
			AAXX(c);
		}

		AAXX("  updateDisplay();")

	    AAXX("	save(str);")
		AAXX("	close();")
	}


}


void CAnaVisionView::OnGenerateMacronormalizeimages(){
	CWaitCursor dummy;


	int i;

	CString macrofile = "c:/mich/pp/ventura-2006/tif-norm/aaanorm.txt";
	CFile f;
	if (!f.Open(macrofile, CFile::modeCreate | CFile::modeWrite)){
		Alert0("could not open");
		return;
	}

	AAXX("background = 0;");
	for (i=0; i<5; i = i+1){
		CString str = "\"c:/mich/pp/ventura-2006/tif-norm/n3";
		if (i<99) {
			str = str + "0";
		};
		if (i<9){
			str= str + "0";
		}

		char s[20];
		_itoa(i+1, s, 10);
		str += CString(s);
		str += ".tif\"";

		str = "str = " + str;

		AAXX(str);
		AAXX(" print (str);")
		AAXX(" open(str);")
		AAXX(" for (x=0; x<640; x++){")
		AAXX("     for (y=0; y<5; y++){")
		AAXX("	        background = background + getPixel(x,y);")
		AAXX("     }")
		AAXX("  }")
		AAXX("close();");
	}
	AAXX(" background = background/640.0/5.0/5.0;");
	for (i=0; i<100; i = i+1){
		CString str = "\"c:/mich/pp/ventura-2006/tif-norm/n3";
		if (i<99) {
			str = str + "0";
		};
		if (i<9){
			str= str + "0";
		}

		char s[20];
		_itoa(i+1, s, 10);
		str += CString(s);
		str += ".tif\"";

		str = "str = " + str;

		AAXX("b=0;");
		AAXX(str);
		AAXX(" print (str);")
		AAXX(" open(str);")
		AAXX(" for (x=0; x<640; x++){")
		AAXX("     for (y=0; y<5; y++){")
		AAXX("	        b = b + getPixel(x,y);")
		AAXX("     }")
		AAXX("  }")
		AAXX("b = b/640.0/5.0;");
		AAXX(" for (x=0; x<640; x++){")
		AAXX("     for (y=0; y<480; y++){")
		AAXX("	        setPixel(x,y,getPixel(x,y)-b+background);");
		AAXX("     }")
		AAXX("  }")

		str = "\"c:/mich/pp/ventura-2006/tif-norm/a3";
		if (i<99) {
			str = str + "0";
		};
		if (i<9){
			str= str + "0";
		}

		_itoa(i+1, s, 10);
		str += CString(s);
		str += ".tif\"";

		str = "str = " + str;

		AAXX(str);
		AAXX(" print (str);")

		AAXX("save(str);");
		AAXX("close();");
	}

}

void CAnaVisionView::OnViewViewmeanvalues()  {
	ViewType = 2;
	CheckViewMenu();
	MyInvalidate();

}
void CAnaVisionView::OnViewViewmeanvaluesinpolygons(){
	ViewType = 5;
	CheckViewMenu();
	MyInvalidate();
}
void CAnaVisionView::OnAnalyzeShowmeanvaluesintable(){
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
//	AreaObject * drawob = pDoc->dat.p.drawob();
//	drawob->CalcGeneratorList();

//	if (!pDoc->currentimage){
	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
		Alert("No data. Please open an image file.");
		return;
	}


	GetFramesDiag diag;
	diag.p = pDoc->dat.p;

	if (diag.DoModal() != IDOK){
		return;
	}
	pDoc->dat.p = diag.p;
	CheckFrames();

	AnalyzeMeans();

	ViewType = 2;
	ShowMode = 1;
	CheckViewMenu();
//	Alert0("hi");

	MyInvalidate();


}

void CAnaVisionView::AnalyzeMeans(){
	CWaitCursor dummy;

	int delta = pDoc->dat.p.DeltaImage;
	if (pDoc->dat.p.StepByOne){
		delta = 1;
	}

	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

	DataFile & d = pDoc->dat;

	int oldindex = d.images[d.ImageIndex].CurrentIndex;

	NMeanImages = (pDoc->dat.p.FrameToAnalyze1 - pDoc->dat.p.FrameToAnalyze0 + 1)/delta;

	if (NMeanImages<1){
		return;
	}
	if (IndexMeanImages){
		delete [] IndexMeanImages;
		delete [] MeanImages;
	}

	IndexMeanImages = new int [NMeanImages];
	MeanImages = new double [NMeanImages];

    
	int i;

	CRect rect;
	GetClientRect(&rect);

	CDC * pDC = GetDC();
	pDC->FillRect(&rect, pDC->GetCurrentBrush());
	pDC->TextOut(1, 100, CString("ESC TO STOP"));

//	CWaitCursor dummy;

	int indeximage = 0;
//	pDoc->dat.p.drawob()->CalcGeneratorList();

	for (i=pDoc->dat.p.FrameToAnalyze0; i<=pDoc->dat.p.FrameToAnalyze1; i += delta){
//		ShowFloat(i, "i");
		IndexMeanImages[indeximage]=i;
		;
		if (InKey()==ESCAPEKEY){
			ReleaseDC(pDC);
			return;
		}

		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
		
		if (!pDoc->ReadFile(i)){			// Reads and converts the frame according to the BCECF calibration
			Alert(" could not read file"); 
			return;	
		}

		double CursorMean;
		AnalyzeImage(CursorMean);			// This calculates the gradient of this frame and puts the
								// result in double * Mean!

		MeanImages[indeximage] = ThisMean;
		indeximage++;
		if (indeximage>=NMeanImages){
			return;
		}
//		ShowFloat(ThisMean, "ThisMean");

	}

	pDoc->ReadFile(oldindex);
	pDC->TextOut(1, 100, CString("           "));

	ReleaseDC(pDC);
//	Alert0("hi");

}


void CAnaVisionView::OnKineticsConstruct(){

	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();

//	if (!pDoc->currentimage){
	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
		Alert("No data. Please open an image file.");
		return;
	}


	DataFile & d = pDoc->dat;
	Kinetics & k = d.k;
	
	if (!k.HasBeenConstructed){
		k.pixelsize = d.p.PixelSizeInMicrom/1e6/d.p.Magnification;
		k.IndexTimeMin = d.p.FrameToAnalyze0;
		k.IndexTimeMax = d.p.FrameToAnalyze1;
		k.DeltaTimeIndex = d.p.DeltaImage;
		if (d.p.StepByOne){
			k.DeltaTimeIndex = 1;
		}
		k.BackgroundTimeIndex0 = d.p.InitialFrame0;
		k.BackgroundTimeIndex1 = d.p.InitialFrame1;

		k.TrackDrift = d.p.TrackDrift;
		k.TotalFree = d.p.totalbuffermM/1000.0;
		k.KFree = pow(10.0, -d.p.theoretical_pK);

		k.beta = d.p.linearconversionfactor*3.862e3;
//		ShowFloat(d.p.linearconversionfactor, "d.p.linearconversionfactor");

	}
//		ShowFloat(d.p.linearconversionfactor, "d.p.linearconversionfactor");
	
	AreaObject * drawob = pDoc->dat.p.drawob();

	NData = int(drawob->Length());
	int i1curs = int(d.p.curspos[0]*double(NData-1)); // left red cursor; construct up to here
	int i2curs = int(d.p.curspos[1]*double(NData-1)); // right red cursor (end of region to use for tracking drift
	int i3curs = int(d.p.curspos[2]*double(NData-1)); // blue cursor: start of fit

	k.IndexRMin = i3curs;
	k.IndexRMax = i1curs;

	Vect M; double r1, r2;
	drawob->GetMidPointAndRadius(M, r1, r2);  // These are the iner and outer rdii of the conus

	k.a=r1*k.pixelsize;						// Radius of oocyte in meters

	DiagKinetics diag;

	diag.k = k;

//	int diagresult = diag.DoModal();

	if (diag.DoModal()!=IDOK){
		diag.k.ClearArrays();
		return;
	}

	k=diag.k;
	diag.k.ClearArrays();

	k.HasBeenFitted = false;

	CWaitCursor dummy;

	CDC * pDC = GetDC();

	k.NDataT = ((k.IndexTimeMax - k.IndexTimeMin)/k.DeltaTimeIndex) +1;
	if (k.NDataT<=0){
		Alert("NDataT <= 0");
		return;
	}

	k.NDataR = ((k.IndexRMax - k.IndexRMin)/k.DeltaRIndex) + 1;
	if (k.NDataR<=0){
		Alert("NDataR <= 0");
		return;
	}

	k.Alloc();

	double * Background = new double[k.NDataR];

	int ir;

	for (ir = 0; ir<k.NDataR; ir++){
		Background[ir]=0;
	}

	double MeanInitialCursorMean = 0;
	int i;
	
	BOOL SaveConvertTopH = pDoc->dat.p.ConvertTopH;

	if (k.ConvertToDeltaHTotal){
		pDoc->dat.p.ConvertTopH = false;
	}

	k.InitialpH = 0;
	int ninitph = 0;
	for (i=k.BackgroundTimeIndex0; i<=k.BackgroundTimeIndex1; i++){
		if (!pDoc->ReadFile(i)){
			Alert(" could not read file");
			pDoc->dat.p.ConvertTopH = SaveConvertTopH == true;
			return;	
		}
		double CursorMean;
		AnalyzeImage(CursorMean);
		k.InitialpH += ThisMean;
		ninitph ++;
		MeanInitialCursorMean += CursorMean;

		for (ir = 0; ir<k.NDataR; ir++){
			int j = k.IndexRMin + ir*k.DeltaRIndex;
			if (NValues[j]>0){
				Background[ir] += Mean[j];
			}
		}
	}

	k.InitialpH /= double(ninitph);

//	ShowFloat(k.InitialpH, "iniph");

	double NBack = k.BackgroundTimeIndex1 - k.BackgroundTimeIndex0 +  1;
	MeanInitialCursorMean /= NBack;
	for (ir = 0; ir<k.NDataR; ir++){
		Background[ir] /= NBack;
	}

	
	int delta = k.DeltaTimeIndex;
	int it = 0;

	for (i=k.IndexTimeMin; i<=k.IndexTimeMax; i += delta){

		if (InKey()==ESCAPEKEY){
			delete [] Background;
			ReleaseDC(pDC);
			pDoc->dat.p.ConvertTopH = (SaveConvertTopH==true);
			return;
		}

		char s[20];
		_itoa(i, s, 10);
		pDC->TextOut(10, 50, CString(s));
		if (!pDoc->ReadFile(i)){
			Alert(" could not read file");
			ReleaseDC(pDC);
			delete [] Background;
			pDoc->dat.p.ConvertTopH = (SaveConvertTopH==true);
			return;	
		}

		double CursorMean;

		AnalyzeImage(CursorMean);
		
		if (k.TrackDrift){
			double difference = MeanInitialCursorMean - CursorMean;
			for (int j=0; j<NData; j++){
				Mean[j] += difference;
			}
		}

		for (int ir=0; ir<k.NDataR; ir++){
			int j=k.IndexRMin + ir*k.DeltaRIndex;
			k.data[it][ir] = Mean[j]-Background[ir];
		}


//		DrawGradient(pDC);
		DrawGradient(pDC, Mean, &blackPen);
		it++;
	}

	k.HasBeenConstructed = true;

	delete [] Background;
	ReleaseDC(pDC);
	pDoc->dat.p.ConvertTopH = (SaveConvertTopH==true);

//	ShowFloat(d.p.FactorRatioIntoDeltaH, "d.p.FactorRatioIntoDeltaH");
	if (k.ConvertToDeltaHTotal){
		double factor = d.p.FactorRatioIntoDeltaH/d.p.ScaleRatio;
//		ShowFloat(factor, "factor");
		for (int ir=0; ir<k.NDataR; ir++){
			for (int it=0; it<k.NDataT; it++){
				k.data[it][ir] *= factor; 
			}
		}
	}

	OnViewViewkineticsasfunctionofradius();

	
}

void CAnaVisionView::OnViewViewkinetics(){
	ViewType = 3;
	CheckViewMenu();
	MyInvalidate();
}

void CAnaVisionView::OnViewViewkineticsasfunctionofradius(){
	ViewType = 4;
	CheckViewMenu();
	MyInvalidate();

}

void CAnaVisionView::OnKineticsFit(){
	Kinetics & k = pDoc->dat.k;

	if (!k.HasBeenConstructed){
		return;
	}

	DiagLinearFit diag;

	diag.k = k;

	if (diag.DoModal() != IDOK){
		diag.k.ClearArrays();
		return;
	}

	k=diag.k;
	diag.k.ClearArrays();
		
	k.FitLinear();
	k.FitType=0;
	if (!k.ConvertToDeltaHTotal){
		k.FitType=1;
	}
	Invalidate();
}


void CAnaVisionView::OnKineticsFitsimulatedOld(){
	Kinetics & k = pDoc->dat.k;
	if (!k.HasBeenConstructed){
		return;
	}

	DiagSimulFit diag;

	diag.k = k;

	if (diag.DoModal() != IDOK){
		diag.k.ClearArrays();
		return;
	}

	k=diag.k;

	diag.k.ClearArrays();

	k.SetupSimulationFit();

	CDC * pDC = GetDC();

	CRect rect;
	GetClientRect(&rect);

	k.HasBeenFitted = true;
	k.FitType = 2;
	while (k.StepSimulationFit(SD)){
		pDC->FillRect(&rect, pDC->GetCurrentBrush());
		this->DrawKinetics(pDC, false, SD);
	}

	ReleaseDC(pDC);

	Invalidate();
}

void CAnaVisionView::OnKineticsFitsimulated(){

	Kinetics & k = pDoc->dat.k;
	if (!k.HasBeenConstructed){
		return;
	}

	DiagSimulFit diag;

	diag.k = k;

	if (diag.DoModal() != IDOK){
		diag.k.ClearArrays();
		return;
	}

	k=diag.k;

	diag.k.ClearArrays();

	k.SetupSimulationFit();

	
	k.HasBeenFitted = true;
	k.FitType = 2;


//	CChildFrame * pf = GetChildFrame();
	CChildFrame * pf = pcf;
	pSimFitThread = new SimFitThread(&pf->StatusSimFitThread, pf->m_hWnd, &k, &SD);
	pSimFitThread->m_pThreadParams = NULL;
	pSimFitThread->m_bAutoDelete = true;
	if (!pSimFitThread->CreateThread(CREATE_SUSPENDED)){
		return;
	}
	VERIFY(pSimFitThread->SetThreadPriority(THREAD_PRIORITY_NORMAL));
	pf->StatusSimFitThread = 1;
	pf->IsSimFitting = true;
	pSimFitThread->ResumeThread();
	Invalidate();
}

void CAnaVisionView::OnUpdateViewMessageFromSimFit(){
//	CChildFrame * pcf = GetChildFrame();
	if (pcf->StatusSimFitThread==3){ // Finished
		pcf->IsSimFitting = false;
		pSimFitThread = NULL;
	}
	Invalidate();

}

void CAnaVisionView::OnKineticsStopfit(){
//	CChildFrame * pcf = GetChildFrame();
	pcf->StatusSimFitThread = 2;
//	pSimFitThread = NULL;

}

void CAnaVisionView::OnModifydataAssignavaluetothisimage() {
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();


	if (!pDoc->DataPresent){
//	if (!pDoc->id.data){
//		Alert("no image");
		return;
	}

	io myio;

	int Last=LastAssignedValue;
	if (myio.InInt(Last, "Value to assign") != IDOK){
		return;
	}

	LastAssignedValue = Last;

	CWaitCursor(dummy);
	pDoc->dat.AssignValue(LastAssignedValue);

	Invalidate();

}



void CAnaVisionView::OnToneTest() 
{
	// TODO: Add your command handler code here
	CString Name = "psf.tif";
	
	CFileDialog diag( true, NULL, "", OFN_FILEMUSTEXIST, NULL, AfxGetMainWnd());

	if (diag.DoModal()!=IDOK){
		return;
	}
	CString FileName = diag.GetPathName();

	Alert(FileName);

	CFile f;
	if (!f.Open(FileName, CFile::modeRead)){
		Alert("could not open file");
		return;
	}

	UINT l=UINT(f.GetLength());

	ShowFloat(l, "l");

	char * buf = new char[l];

	{
		CWaitCursor dummy;
		f.Read(buf, l);
	}

	Alert0("hi");

	int sizex=126;
	int sizey=2640;

	int start0 = l-sizex*sizey;
	ShowFloat(start0, "start0");
	int off = 0;

	int x=sizex/2;
	int y=sizey/2;

	while (true){
		io myio;

		if (myio.InInt(off, "off") != IDOK){
			return;
		}
		int start = start0+off;

		if (start<0) return;
		if (start>start0) return;


		if (myio.InInt(x, "x") != IDOK){
			return;
		}
		if (x<0) return;
		if (x>=sizex) return;

		if (myio.InInt(y, "y") != IDOK){
			return;
		}
		if (y<0) return;
		if (y>=sizey) return;

		int index = start+sizex*y+x;

		char value = buf[index];
		int ivalue = value;
		if (ivalue<0) ivalue += 256;
		ShowInt(ivalue, "value");
	}
}



void CAnaVisionView::OnKineticsExport(bool ShowAsFunctionOfTime) {
	Kinetics & k = pDoc->dat.k;

	if (!k.HasBeenConstructed){
		Alert("Please construct the kinetics first");
		return;
	}

	CWaitCursor dummy;

	k.ShowAsFunctionOfTime = ShowAsFunctionOfTime;

	int ncurves = k.NumberOfCurves();
//	ShowFloat(ncurves, "nc");
	int npointspercurve=k.NPointsPerCurve();
//	ShowFloat(npointspercurve, "np");

	CString Source;
	int curve, point;
	char s[20];

	for (point=0; point<npointspercurve; point++){
		CString c;

		for (curve=0; curve<ncurves; curve++){
			double x, y;
			k.GetPoint(curve, point, x, y, ShowAsFunctionOfTime, false);
			_gcvt(x, 10, s);
			c += CString(s)+"\t";
			_gcvt(y, 10, s);
			c += CString(s);
			if (k.HasBeenFitted){
				c += "\t";
				double fx, fy;
				k.GetPoint(curve, point, fx, fy, ShowAsFunctionOfTime, true);
				_gcvt(fx, 10, s);
				c += CString(s)+"\t";
				_gcvt(fy, 10, s);
				c += CString(s);
				if (curve==(ncurves-1)){
					if (point==0){
						_gcvt(k.Current, 10, s);
						c += "\t"+CString(s);
					}
					else{
						if (point==1){
							_gcvt(k.D, 10, s);
							c += "\t"+CString(s);
						}
						else{
							if (point==2){
								_gcvt(SD, 10, s);
								c += "\t"+CString(s);
							}
						}
					}
				}
			}
			if (curve==(ncurves-1)){
				c+="\r\n";
			}
			else{
				c += "\t";
			}
		}
		Source += c;
	}


	CopyTextToClipboard(Source);
}


// AnalyisMode: 0 regular mean or sum
//				1 membrane value
//				2 inside cell, excluding membrane (assumed to be cicular!

// CalcType: 0 : Mean; 1 : Sum; 2: Area
void CAnaVisionView::AnalyzeROISOfSeries(bool & first, int indexseries, int indexs,
											int AnalysisMode, int CalcType){
//	ShowFloat(indexs, "is");
//	ShowFloat(AnalysisMode, "AnalysisMode");
//	ShowFloat(CalcType, "CalcType");
	DataFile & d = pDoc->dat;

	int NPolySelected=0;
	int MaxPoly = MAX_POLYGON_SELECTED;
	if (d.polys.NPoly()<MaxPoly){
		MaxPoly = d.polys.NPoly();
	}
	for (int i=0; i<MaxPoly; i++){
		if (PolygonSelected[i]){
			if (i!=BackgroundPolygon){
				NPolySelected++;
			}
		}
	}

	SeveralMeanROI * buffSeveral = (SeveralMeanROI *)	d.roivalues.SeriesOfROIValues.buff;

	SeveralMeanROI & smroi = buffSeveral[indexs];

	smroi.ListOfArrays.UpdateBuffer(NPolySelected);

	ImageSequence & image = d.images[indexseries];
	int nimages = image.LastIndex-image.FirstIndex+1;


	MeanROIArray * buff = (MeanROIArray *)smroi.ListOfArrays.buff;
	int NStart=0;
	bool copy = false;
	if (ConcatenateImages){
		if (!first){
			NStart += buff[0].values.size();
			copy = true;
		}
	}
	first = false;


	for (int i=0; i<NPolySelected; i++){
//		buff[i].values.UpdateBuffer(nimages+NStart, copy);
		buff[i].values.resize(nimages+NStart);
	}
	int SaveIndex = d.ImageIndex;
	d.ImageIndex = indexseries;

	CString imageindexstring;
	//XXX
	BOOL ffirst = true;
	AddIntToString(ffirst, indexseries, imageindexstring);
	CDC * pDC = GetDC();

	CellsClass * Cells;
	for (int i=0; i<nimages; i++){
		CString c;
		BOOL first = true;
		AddIntToString(first, i, c);
		pDC->TextOutA(20,20, imageindexstring+CString("   ")+c);

//		if (i>90ShowFloat(i+image.FirstIndex, "i+image.FirstIndex");
		bool DoShow = i>90;
		DoShow = false;
		d.ReadImage(i+image.FirstIndex, pDoc->id, Cells, false, DoShow);

		int offsetx, offsety;
		offsetx = d.OffsetX[i];
		offsety = d.OffsetY[i];

		double Back=0.0;
		if (BackgroundPolygon>=0){
			if (CalcType != 2){ // area
						Back = d.polys.CalcMeanValue(0.0, BackgroundPolygon, pDoc->id.data,
							pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage, offsetx, offsety, 0);
			}
		}
//		ShowFloat(Back, "Back");
		int p=0;
		for (int ip=0; ip<MaxPoly; ip++){
			if (PolygonSelected[ip] && (ip != BackgroundPolygon)){
//				double * values = (double *)buff[p].values.buff;
				double * values = &(buff[p].values[0]);
				switch (AnalysisMode){
					case 0 : values[i+NStart] =
								 d.polys.CalcMeanValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
								pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
						// if (i==0) ShowFloat(values[i+NStart], "values[i+NStart]");
							//values[i+NStart] -= Back;
							break;
					case 1 : values[i+NStart] =
								 d.polys.CalcMembraneValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
								 pDoc->dat.p.ysizeimage, offsetx, offsety,
								 pDoc->dat.p.SizeOfMembraneInPixels,
								 pDoc->dat.p.ROISAreClosedInMembraneAnalysis, CalcType
								 );
							//values[i+NStart] -= Back;
							break;
					case 2 : double mean =  1.0; //d.polys.CalcMeanValue(0, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
//								pDoc->dat.p.ysizeimage, offsetx, offsety, 2);
							double sum  =  d.polys.CalcMeanValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
								pDoc->dat.p.ysizeimage, offsetx, offsety, 2);
						values[i+NStart] = sum/mean;
//								 d.polys.CalcMeanValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
//								pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
							//values[i+NStart] -= Back;
							break;
				}
							
					p++;
			}
		}
		//pDoc->dat
	}


	d.ImageIndex = SaveIndex;

	ReleaseDC(pDC);

	

}
void CAnaVisionView::DrawMeanROIS(CDC * pDC){
	if (ShowMode==1){
		DrawMeanROISText(pDC);
		return;
	}
	else{
		DrawMeanROISCurves(pDC);
		return;
	}
	return;
}
void CAnaVisionView::DrawMeanROISText(CDC * pDC){
	CRect rect;
	GetWindowRect(&rect);
	ResultsEdit.GetClientRect(&EditRect);
	DataFile & d = pDoc->dat;

	EditRect.left = 0;
	EditRect.right = rect.right-rect.left;
	EditRect.top=0;//20+ 7*textMetric.tmHeight;
	EditRect.bottom = rect.bottom-rect.top;
	
	ResultsEdit.MoveWindow(&EditRect, true);
	CString EditText = CString("");


	SeveralMeanROI * buffSeveral = (SeveralMeanROI *)	d.roivalues.SeriesOfROIValues.buff;

	int NSeries = d.roivalues.SeriesOfROIValues.GetSize();

	if (NSeries<1){
//		Alert("NSeries = 0");
		return;
	}

	SeveralMeanROI & smroi = buffSeveral[0];

	int NPoly = smroi.ListOfArrays.GetSize();
	if (NPoly<1){
//		Alert("NPoly=0");
		return;
	}

	int MaxImage = 0;

	for (int indexs=0; indexs<NSeries; indexs++){
		SeveralMeanROI & smroi = buffSeveral[indexs];
	 	MeanROIArray * buff = (MeanROIArray *)smroi.ListOfArrays.buff;
		int ni=buff[0].values.size();
		if (ni>MaxImage) MaxImage=ni;
	}
	if (MaxImage<1){
//		Alert("MaxImage=0");
		return;
	}

	for (int i=0; i<MaxImage; i++){
		BOOL first=true;
		AddIntToString(first, i, EditText);

		for (int indexs=0; indexs<NSeries; indexs++){
			SeveralMeanROI & smroi = buffSeveral[indexs];
		 	MeanROIArray * buff = (MeanROIArray *)smroi.ListOfArrays.buff;
			for (int p=0; p<NPoly; p++){
				int NI=buff[p].values.size();
				if (i<NI){
//					double * values = (double *)(buff[p].values.buff);
					double * values = &(buff[p].values[0]);
					AddDoubleToString(first, values[i], EditText);
				}
				else{
					EditText += CString(" - \t");
				}
			}
		}
		EditText += "\r\n";
	}


	ResultsEdit.SetWindowText(EditText);
	ResultsEdit.LineScroll( 0);
	MoveButtons(pDC, Min, Max, pDoc->dat.p.AutoScale==true);
}

void CAnaVisionView::DrawMeanROISCurves(CDC * pDC){

	DataFile & d = pDoc->dat;

	Min = d.p.MinManScale;
	Max = d.p.MaxManScale;
	if (d.p.AutoScale){
		d.roivalues.CalcMinMax(Min, Max);
	}
	NData = d.roivalues.GetMaxImages();
	
	CRect rect;

	GetClientRect(&rect);

	offx = 100;
	int	offy = 5;

	bottom = rect.bottom - offy;

	scalex = double(rect.right-rect.left-offx)/double(NData+2);

	scaley = double(rect.bottom-rect.top-offy)/(Max-Min);

	MoveButtons(pDC, Min, Max, d.p.AutoScale==true);

	SeveralMeanROI * buffSeveral = (SeveralMeanROI *)	d.roivalues.SeriesOfROIValues.buff;
	int NSeries = d.roivalues.SeriesOfROIValues.GetSize();

	if (NSeries<1){
//		Alert("NSeries = 0");
		return;
	}

	SeveralMeanROI & smroi = buffSeveral[0];

	int NPoly = smroi.ListOfArrays.GetSize();
	if (NPoly<1){
//		Alert("NPoly=0");
		return;
	}

	pDC->MoveTo(Scalex(0), Scaley(Min));
	pDC->LineTo(Scalex(0), Scaley(Max));
	pDC->MoveTo(Scalex(0), Scaley(Min));
	pDC->LineTo(Scalex(NData-1), Scaley(Min));

	CPen * OldPen = pDC->SelectObject(&(KineticPens[0]));

	int ipen = 0;
	for (int indexs=0; indexs<NSeries; indexs++){
		SeveralMeanROI & smroi = buffSeveral[indexs];
	 	MeanROIArray * buff = (MeanROIArray *)smroi.ListOfArrays.buff;
		int iipen=0;
		for (int p=0; p<NPoly; p++){
//			pDC->SelectObject(&(KineticPens[ipen]));
			pDC->SelectObject(&(RoiPens[iipen][ipen]));
//			ipen = (ipen + 1) % NKINETICPENS;
			iipen  = (iipen+1) % NCHANNELSKINETICPENS;
			int NI=buff[p].values.size();
			if (NI>1){
//				double * data = (double *)buff[p].values.buff;
				double * data = &(buff[p].values[0]);
				pDC->MoveTo(Scalex(0), Scaley(data[0]));
				for (int i=1; i<NI; i++){
					pDC->LineTo(Scalex(i), Scaley(data[i]));
				}
			}
		}
		ipen = (ipen + 1) % NKINETICPENS;
	}
	{
		int ipen = 0;
		int x0=5;
		int y0=40;
		for (int indexs=0; indexs<NSeries; indexs++){
			char s[20];
			_itoa(indexs+1, s, 10);
			pDC->TextOutA(x0, y0, CString(s));
			pDC->SelectObject(&(RoiPens[0][ipen]));
			ipen = (ipen + 1) % NKINETICPENS;
			pDC->MoveTo(x0+20, y0+8);
			pDC->LineTo(x0+50, y0+8);
			y0 += 20;
		}
	}

	pDC->SelectObject(OldPen);



}
void CAnaVisionView::OnRButtonDownROIS(UINT nFlags, CPoint point){
//	Alert0("hi");

		int ix = point.x;
		int x = int(double(ix-offx)/scalex);
		int iy=point.y;
//		return bottom - int ((y-Min)*scaley);
		double y = -double(iy-bottom)/scaley+Min;

		char s[20];
		_itoa(x,s,10);
//		_gcvt(x, 6, s);
		CString c="x: "+CString(s);
		_gcvt(y, 6, s);
		c += CString("; y: ")+CString(s);

		CDC * pDC = GetDC();

		pDC->TextOutA(ix, iy, c);

		ReleaseDC(pDC);

/*

CRect rect; 
		rect.left = ix-10;
		rect.right = rect.left + 100;
		rect.bottom = iy+50;
		rect.top = rect.bottom -100;


		pTool->SetDelayTime(2000);
//		CToolTipCtrl * pTool = new CToolTipCtrl();
//		pTool->Create(this);

	pTool->MoveWindow(&rect);
		pTool->SetWindowText(c);
		pTool->ShowWindow(SW_SHOW);

*/
}
void CAnaVisionView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (ViewType == 5){ // mean values of ROI's
		OnRButtonDownROIS(nFlags, point);
	}


//	CView::OnRButtonDown(nFlags, point);
}


void CAnaVisionView::OnRegionofinterestCalcmeanvalues(){

	CalcMeanOrSumValues(0); // Mean
}

void CAnaVisionView::OnRegionofinterestCalculatemembrane(){
	if (pDoc->IsLoadingFile){
		return;
	}

	if (pDoc->dat.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}

	DiagSelectSeriesAndPolygons diag;

	DataFile & d = pDoc->dat;

	if (d.NImages<1){
		Alert("less than 1 image");
		return;
	}

	int MaxSeries = d.NImages;
	if (MaxSeries>MAX_SERIES_SELECTED){
		MaxSeries = MAX_SERIES_SELECTED;
	}
	for (int i=0; i<MaxSeries; i++){
		diag.SeriesNames[i] = d.images[i].Title; 
	}

	if (MaxSeries==1){
		SeriesSelected[0]=true;
	}

	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		diag.SeriesSelected[i] = SeriesSelected[i];
	}

	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		diag.PolygonSelected[i]=PolygonSelected[i];
	}
	diag.BackgroundPolygon = BackgroundPolygon;
	diag.ConcatenateImages = ConcatenateImages;


	diag.NSeries = d.NImages;
	if (d.NImages>MAX_SERIES_SELECTED){
		diag.NSeries = MAX_SERIES_SELECTED;
	}
	diag.NPoly = pDoc->dat.polys.NPoly();
	if (diag.NPoly>MAX_POLYGON_SELECTED){
		diag.NPoly = MAX_POLYGON_SELECTED;
	}

	diag.m_ClosedLoop = d.p.ROISAreClosedInMembraneAnalysis;
	diag.m_SizeMembraneInPixels = d.p.SizeOfMembraneInPixels;
//	ShowFloat(d.p.ROISAreClosedInMembraneAnalysis, "d.p.ROISAreClosedInMembraneAnalysis");
//	ShowFloat(d.p.SizeOfMembraneInPixels, "d.p.SizeOfMembraneInPixels");

	if (diag.DoModal() != IDOK){
		return;
	}
	d.p.ROISAreClosedInMembraneAnalysis = diag.m_ClosedLoop;
	d.p.SizeOfMembraneInPixels=diag.m_SizeMembraneInPixels;
//	ShowFloat(d.p.ROISAreClosedInMembraneAnalysis, "d.p.ROISAreClosedInMembraneAnalysis");
//	ShowFloat(d.p.SizeOfMembraneInPixels, "d.p.SizeOfMembraneInPixels");

	ConcatenateImages = diag.ConcatenateImages;

	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		SeriesSelected[i] = diag.SeriesSelected[i];
	}
	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		PolygonSelected[i] = diag.PolygonSelected[i];
	}
	BackgroundPolygon = diag.BackgroundPolygon;

	int NSeriesSelected = 0;
/*
int MaxSeries =  MAX_SERIES_SELECTED;
	if (d.NImages<MaxSeries){
		MaxSeries = d.NImages;
	}
*/
	for (int i=0; i<MaxSeries; i++){
		if (SeriesSelected[i]) NSeriesSelected++;
	}
	if (NSeriesSelected<1){
		Alert("No series selected");
		return;
	}
	int NPolySelected=0;
	int MaxPoly = MAX_POLYGON_SELECTED;
	if (pDoc->dat.polys.NPoly()<MaxPoly){
		MaxPoly = pDoc->dat.polys.NPoly();
	}
	for (int i=0; i<MaxPoly; i++){
		if (PolygonSelected[i]){
			if (i!=BackgroundPolygon){
				NPolySelected++;
			}
		}
	}
	if (NPolySelected<1){
		Alert("No Poly Selected");
		return;
	}

	CWaitCursor dummy;
	if (ConcatenateImages){
		pDoc->dat.roivalues.SeriesOfROIValues.UpdateBuffer(1);
	}
	else{
		pDoc->dat.roivalues.SeriesOfROIValues.UpdateBuffer(NSeriesSelected);
	}
	int s=0;
	bool first = true;
	for (int i=0; i<MaxSeries; i++){
		if (SeriesSelected[i]){
//			AnalyzeROISOfSeries(first, i, s, 0, 1);
			int CalcMode = 1; // Sum
			int AnalysisMode = 1 ; // MembraneValue
			AnalyzeROISOfSeries(first, i, s, AnalysisMode, CalcMode);
			if (!ConcatenateImages){
				s++;
			}
		}
	}

	ViewType = 5;
	Invalidate();

}


	// CalcType 0: Mean
	//			1: Sum
	//			2: area
void CAnaVisionView::CalcMeanOrSumValues(int CalcType){

	if (pDoc->IsLoadingFile){
		return;
	}

	if (pDoc->dat.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}
	pDoc->UpdatePolygonDataPoints();


	DiagSelectSeriesAndPolygons diag;

	DataFile & d = pDoc->dat;

	if (d.NImages<1){
		Alert("less than 1 image");
		return;
	}

	int MaxSeries = d.NImages;
	if (MaxSeries>MAX_SERIES_SELECTED){
		MaxSeries = MAX_SERIES_SELECTED;
	}
	for (int i=0; i<MaxSeries; i++){
		diag.SeriesNames[i] = d.images[i].Title; 
	}

	if (MaxSeries==1){
		SeriesSelected[0]=true;
	}

	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		diag.SeriesSelected[i] = SeriesSelected[i];
	}
	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		diag.PolygonSelected[i]=PolygonSelected[i];
	}
	diag.BackgroundPolygon = BackgroundPolygon;
	diag.ConcatenateImages = ConcatenateImages;


	diag.NSeries = d.NImages;
	if (d.NImages>MAX_SERIES_SELECTED){
		diag.NSeries = MAX_SERIES_SELECTED;
	}
	diag.NPoly = pDoc->dat.polys.NPoly();
	if (diag.NPoly>MAX_POLYGON_SELECTED){
		diag.NPoly = MAX_POLYGON_SELECTED;
	}



	if (diag.DoModal() != IDOK){
		return;
	}


	ConcatenateImages = diag.ConcatenateImages;

	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		SeriesSelected[i] = diag.SeriesSelected[i];
	}
	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		PolygonSelected[i] = diag.PolygonSelected[i];
	}
	BackgroundPolygon = diag.BackgroundPolygon;

	int NSeriesSelected = 0;
/*
int MaxSeries =  MAX_SERIES_SELECTED;
	if (d.NImages<MaxSeries){
		MaxSeries = d.NImages;
	}
*/
	for (int i=0; i<MaxSeries; i++){
		if (SeriesSelected[i]) NSeriesSelected++;
	}
	if (NSeriesSelected<1){
		Alert("No series selected");
		return;
	}
	int NPolySelected=0;
	int MaxPoly = MAX_POLYGON_SELECTED;
	if (pDoc->dat.polys.NPoly()<MaxPoly){
		MaxPoly = pDoc->dat.polys.NPoly();
	}
	for (int i=0; i<MaxPoly; i++){
		if (PolygonSelected[i]){
			if (i!=BackgroundPolygon){
				NPolySelected++;
			}
		}
	}
	if (NPolySelected<1){
		Alert("No Poly Selected");
		return;
	}

	CWaitCursor dummy;
	if (ConcatenateImages){
		pDoc->dat.roivalues.SeriesOfROIValues.UpdateBuffer(1);
	}
	else{
		pDoc->dat.roivalues.SeriesOfROIValues.UpdateBuffer(NSeriesSelected);
	}
	int s=0;
	bool first = true;
	for (int i=0; i<MaxSeries; i++){
		if (SeriesSelected[i]){
			AnalyzeROISOfSeries(first, i, s, 0, CalcType);
			if (!ConcatenateImages){
				s++;
			}
		}
	}

	ViewType = 5;
	Invalidate();

}


void CAnaVisionView::OnRegionofinterestCalcsumvalues()
{
	// TODO: Add your command handler code here
//	Alert0("hi");
	CalcMeanOrSumValues(1) ; // sum
}


void CAnaVisionView::OnRegionofinterestCalculateCircularGradient(bool circularROI){
	if (pDoc->IsLoadingFile){
		return;
	}

	if (pDoc->dat.polys.NPoly()<1){
		Alert("no polygons defined");
		return;
	}

	DiagSelectSeriesAndPolygons diag;

	DataFile & d = pDoc->dat;

	if (d.NImages<1){
		Alert("less than 1 image");
		return;
	}

	int MaxSeries = d.NImages;
	if (MaxSeries>MAX_SERIES_SELECTED){
		MaxSeries = MAX_SERIES_SELECTED;
	}
	for (int i=0; i<MaxSeries; i++){
		diag.SeriesNames[i] = d.images[i].Title; 
	}
	if (MaxSeries==1){
		SeriesSelected[0]=true;
	}

	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		diag.SeriesSelected[i] = SeriesSelected[i];
	}
	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		diag.PolygonSelected[i]=PolygonSelected[i];
	}
	diag.BackgroundPolygon = BackgroundPolygon;
	diag.ConcatenateImages = ConcatenateImages;


	diag.NSeries = d.NImages;
	if (d.NImages>MAX_SERIES_SELECTED){
		diag.NSeries = MAX_SERIES_SELECTED;
	}
	diag.NPoly = pDoc->dat.polys.NPoly();
	if (diag.NPoly>MAX_POLYGON_SELECTED){
		diag.NPoly = MAX_POLYGON_SELECTED;
	}

	diag.m_ClosedLoop = d.p.ROISAreClosedInMembraneAnalysis;
	diag.m_SizeMembraneInPixels = d.p.SizeOfMembraneInPixels;
	diag.m_NumberOfGradientPoints = d.p.NumberOfGradientPoints;
//	ShowFloat(d.p.ROISAreClosedInMembraneAnalysis, "d.p.ROISAreClosedInMembraneAnalysis");
//	ShowFloat(d.p.SizeOfMembraneInPixels, "d.p.SizeOfMembraneInPixels");
	diag.m_NumberOfGradientPoints = d.p.NumberOfGradientPoints;
	if (diag.DoModal() != IDOK){
		return;
	}
	d.p.NumberOfGradientPoints = diag.m_NumberOfGradientPoints;
	d.p.ROISAreClosedInMembraneAnalysis = diag.m_ClosedLoop;
	d.p.SizeOfMembraneInPixels=diag.m_SizeMembraneInPixels;
//	ShowFloat(d.p.ROISAreClosedInMembraneAnalysis, "d.p.ROISAreClosedInMembraneAnalysis");
//	ShowFloat(d.p.SizeOfMembraneInPixels, "d.p.SizeOfMembraneInPixels");

	ConcatenateImages = diag.ConcatenateImages;

	for (int i=0; i<MAX_SERIES_SELECTED; i++){
		SeriesSelected[i] = diag.SeriesSelected[i];
	}
	for (int i=0; i<MAX_POLYGON_SELECTED; i++){
		PolygonSelected[i] = diag.PolygonSelected[i];
	}
	BackgroundPolygon = diag.BackgroundPolygon;

	int NSeriesSelected = 0;
	for (int i=0; i<MaxSeries; i++){
		if (SeriesSelected[i]) NSeriesSelected++;
	}
	if (NSeriesSelected<1){
		Alert("No series selected");
		return;
	}
	int NPolySelected=0;
	int MaxPoly = MAX_POLYGON_SELECTED;
	if (pDoc->dat.polys.NPoly()<MaxPoly){
		MaxPoly = pDoc->dat.polys.NPoly();
	}
	int polyselected;
	for (int i=0; i<MaxPoly; i++){
		if (PolygonSelected[i]){
			if (i!=BackgroundPolygon){
				NPolySelected++;
				polyselected = i;
			}
		}
	}
	if (NPolySelected<1){
		Alert("No Poly Selected");
		return;
	}
	if (NPolySelected>1){
		Alert("Only one poly can be analyzed");
		return;
	}

	CWaitCursor dummy;
	if (ConcatenateImages){
		pDoc->dat.roivalues.SeriesOfROIValues.UpdateBuffer(1);
	}
	else{
		pDoc->dat.roivalues.SeriesOfROIValues.UpdateBuffer(NSeriesSelected);
	}
	int s=0;
	bool first = true;
	for (int i=0; i<MaxSeries; i++){
		if (SeriesSelected[i]){
//			AnalyzeROISOfSeries(first, i, s, 0, 1);
			int CalcMode = 1; // Sum
			int AnalysisMode = 3 ; // Analysis Mode
			AnalyzeGradientROISOfSeries(first, i, s, AnalysisMode, CalcMode, circularROI);
			if (!ConcatenateImages){
				s++;
			}
		}
	}

	ViewType = 5;
	Invalidate();

}
void CAnaVisionView::AnalyzeGradientROISOfSeries(bool & first, int indexseries, int indexs,
											int AnalysisMode, int CalcType, bool circularROI){
//	ShowFloat(indexs, "is");
//	ShowFloat(AnalysisMode, "AnalysisMode");
//	ShowFloat(CalcType, "CalcType");
	DataFile & d = pDoc->dat;

//	int NPolySelected=0;
	int MaxPoly = MAX_POLYGON_SELECTED;
	if (d.polys.NPoly()<MaxPoly){
		MaxPoly = d.polys.NPoly();
	}
	int polyselected = -1;
	int backgroundpoly = -1;
	for (int i=0; i<MaxPoly; i++){
		if (PolygonSelected[i]){
			if (i!=BackgroundPolygon){
				polyselected = i;
			}
			else{
				backgroundpoly = i;
			}
		}
	}
	if (polyselected<0){
		Alert("no poly selected");
		return;
	}
	int NGradient = d.p.NumberOfGradientPoints;
	int NDataPoints = 2*NGradient;
	if (NDataPoints<1){
		Alert0("too few gradient points");
		return;
	}

	SeveralMeanROI * buffSeveral = (SeveralMeanROI *)	d.roivalues.SeriesOfROIValues.buff;

	SeveralMeanROI & smroi = buffSeveral[indexs];

	smroi.ListOfArrays.UpdateBuffer(NDataPoints);

	ImageSequence & image = d.images[indexseries];
	int nimages = image.LastIndex-image.FirstIndex+1;

	MeanROIArray * buff = (MeanROIArray *)smroi.ListOfArrays.buff;
	int NStart=0;
	bool copy = false;
	if (ConcatenateImages){
		if (!first){
			NStart += buff[0].values.size();
			copy = true;
		}
	}
	first = false;

	for (int i=0; i<NDataPoints; i++){
//		buff[i].values.UpdateBuffer(nimages+NStart, copy);
		buff[i].values.resize(nimages+NStart);
	}
	int SaveIndex = d.ImageIndex;
	d.ImageIndex = indexseries;

	CString imageindexstring;
	BOOL ffirst = true;
	AddIntToString(ffirst, indexseries, imageindexstring);
	CDC * pDC = GetDC();

	for (int i=0; i<nimages; i++){
		CString c;
		BOOL first = true;
		AddIntToString(first, i, c);
		pDC->TextOutA(20,20, imageindexstring+CString("   ")+c);

//		if (i>90ShowFloat(i+image.FirstIndex, "i+image.FirstIndex");
		bool DoShow = i>90;
		DoShow = false;
		d.ReadImage(i+image.FirstIndex, pDoc->id, pDoc->pActualCells, false, DoShow);

		int offsetx, offsety;
		offsetx = d.OffsetX[i];
		offsety = d.OffsetY[i];

		double Back=0.0;
		if (backgroundpoly>=0){
			int CalcType = 0; // Mean
			Back = d.polys.CalcMeanValue(0.0, backgroundpoly, pDoc->id.data,
				pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
		}
		int CalcType = 1; // Sum
		double ZeroValue = 0.0;
		if (circularROI){
			ZeroValue = 
				d.polys.CalcMeanValue(Back, polyselected, pDoc->id.data, pDoc->dat.p.xsizeimage,
									pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
		}

//		ShowFloat(ZeroValue, "ZeroValue");

		vector<double> SumsOfNewPolylines;
		int NOldPolylines = d.polys.NPoly();
		for (int igradient=0; igradient<NGradient; igradient++){
			int MembraneSize = (igradient+1)*d.p.SizeOfMembraneInPixels;
	//		ShowFloat(MembraneSize, "MembraneSize");
			bool IsLinearRoi = true;
			if (circularROI){
				IsLinearRoi = false;
			}
			d.polys.MakeMembranePolys(polyselected, MembraneSize, circularROI, IsLinearRoi);
			int CalcType = 1; // Sum
			double SumExt = 
				d.polys.CalcMeanValue(Back, NOldPolylines, pDoc->id.data, pDoc->dat.p.xsizeimage,
							pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
//			ShowFloat(SumExt, "SumExt");
			SumsOfNewPolylines.push_back(SumExt);
			double SumInt = 
				d.polys.CalcMeanValue(Back, NOldPolylines+1, pDoc->id.data, pDoc->dat.p.xsizeimage,
							pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
//			ShowFloat(SumInt, "SumInt");
			SumsOfNewPolylines.push_back(SumInt);
			d.polys.DeletePolygon(NOldPolylines+1);
			d.polys.DeletePolygon(NOldPolylines);

		}


		int idat = 0;
//		double * values = (double *)buff[idat].values.buff;
//		double * values = &(buff[idat].values[0]);
		for (int igradient=0; igradient<(NGradient-1); igradient++){
//			values = (double *)buff[idat].values.buff;
			buff[idat].values[i+NStart] = SumsOfNewPolylines[2*(NGradient-igradient-1)]-SumsOfNewPolylines[2*(NGradient-igradient-2)];
//			ShowFloat(values[i+NStart], "values[i+NStart]");
			idat++;
		}
//		values = (double *)buff[idat].values.buff;
		buff[idat].values[i+NStart] = SumsOfNewPolylines[0]-ZeroValue;
//		ShowFloat(values[i+NStart], "values[i+NStart] first ext");
		idat++;
//		values = (double *)buff[idat].values.buff;
//		values[i+NStart] = ZeroValue - SumsOfNewPolylines[1];
		buff[idat].values[i+NStart] = ZeroValue - SumsOfNewPolylines[1];
		if (!circularROI){
			//values[i+NStart] *= -1;
			buff[idat].values[i+NStart] *= -1;
		}
//		ShowFloat(values[i+NStart], "values[i+NStart] first int");
		idat++;
		for (int igradient=0; igradient<(NGradient-1); igradient++){
//			values = (double *)buff[idat].values.buff;
//			values[i+NStart] = SumsOfNewPolylines[2*igradient+1]-SumsOfNewPolylines[2*igradient+3];
			buff[idat].values[i+NStart] = SumsOfNewPolylines[2*igradient+1]-SumsOfNewPolylines[2*igradient+3];
			if (!circularROI){
//				values[i+NStart] *= -1;
				buff[idat].values[i+NStart] *= -1;
			}
//			ShowFloat(values[i+NStart], "values[i+NStart]");
			idat++;
		}
//		for (int ip=0; ip<MaxPoly; ip++){
//			if (PolygonSelected[ip] && (ip != BackgroundPolygon)){
//				double * values = (double *)buff[p].values.buff;
//				switch (AnalysisMode){
//					case 0 : values[i+NStart] =
//								 d.polys.CalcMeanValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
//								pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
//						//ShowFloat(values[i+NStart], "values[i+NStart]");
//							//values[i+NStart] -= Back;
//							break;
//					case 1 : values[i+NStart] =
//								 d.polys.CalcMembraneValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
//								 pDoc->dat.p.ysizeimage, offsetx, offsety,
//								 pDoc->dat.p.SizeOfMembraneInPixels,
///								 pDoc->dat.p.ROISAreClosedInMembraneAnalysis, CalcType
///								 );
//							//values[i+NStart] -= Back;
//							break;
//					case 2 : values[i+NStart] =
//								 d.polys.CalcMeanValue(Back, ip, pDoc->id.data, pDoc->dat.p.xsizeimage,
//								pDoc->dat.p.ysizeimage, offsetx, offsety, CalcType);
//							//values[i+NStart] -= Back;
//							break;
//				}
//							
//					p++;
//			}
//		}
		//pDoc->dat


	}


	d.ImageIndex = SaveIndex;

	ReleaseDC(pDC);



}
void CAnaVisionView::OnRegionofinterestCalcarea(){

	// TODO: Add your command handler code here
//	Alert0("hi");
	CalcMeanOrSumValues(2) ; // area
}

void CAnaVisionView::OnViewViewfilledarea(){
	ViewType = 6;

	CheckViewMenu();

	MyInvalidate();
}

void CAnaVisionView::DrawFillArea(CDC * pDC){
	if (ShowMode==1){
		DrawFillAreaText(pDC);
		return;
	}
	else{
		DrawFillAreaCurves(pDC);
		return;
	}
	return;
}

void CAnaVisionView::DrawFillAreaText(CDC * pDC){
	CChildFrame * pf = pcf;
	char s[20];
	_itoa(pf->AreaSelected, s, 10);


	ResultsEdit.SetWindowText(CString(s));
	ResultsEdit.LineScroll( 0);


}
void CAnaVisionView::DrawFillAreaCurves(CDC * pDC){
	CChildFrame * pf = pcf;
	char s[20];
	_itoa(pf->AreaSelected, s, 10);

	pDC->TextOut(10, 10, CString(s));

}


void CAnaVisionView::OnAnalyzeFret(int selectedpoly){
//	Alert0("hi");
	CAnaVisionDoc * pDoc = (CAnaVisionDoc * ) GetDocument();
	if (!pDoc) return;

	if (!pDoc->DataPresent){
		Alert("No data. Please open an image file.");
		return;
	}

	if (selectedpoly<0) return;
	DataFile & d = pDoc->dat;
	if (selectedpoly>=d.polys.NPoly()){
		Alert("index of polygon larger than n polys present.");
		return;
	}
	ImageSequence & image  = d.images[d.ImageIndex];
//	ShowFloat(d.ImageIndex, "d.ImageIndex");
	int oldindex = image.CurrentIndex;

	int NImages = image.LastIndex - image.FirstIndex + 1;
	if ((NImages % 3) != 0){
		Alert("Number of images is not multiple of 3");
		return;
	}
//	ShowFloat(NImages, "NIm");

	DiagFret diag;
	diag.p = d.p;

	if (diag.DoModal() != IDOK){
		return;
	}
	d.p = diag.p;

	DataFile * pfret = new DataFile;
	DataFile & fret = *pfret;
	fret.SetVersion();

//	DataFile fret;
	fret.FileType=5; // 0=Converted vws file
	fret.p = d.p;
	for (int i=0; i<d.polys.NPoly(); i++){
		PolygonObject * pPol = d.polys.GetPoly(i);
		PolygonObject poly = *pPol;
		//d.polys.GetPoly(i)->Copy(poly);
		fret.polys.AddPolygon(poly);
	}
/*	PolygonObject * polyd = d.polys.GetPoly(0);
	ShowFloat(polyd->pointlist[0].x, "xd");
	PolygonObject * polyf = fret.polys.GetPoly(0);
	ShowFloat(polyf->pointlist[0].x, "xf");
*/	
	for (int i=0; i<d.lines.NPoly(); i++){
		PolygonObject poly;
		d.lines.GetPoly(i)->Copy(poly);
		fret.lines.AddPolygon(poly);
	}
//	ShowFloat(fret.polys.NPoly(), " Npoly");

//	ArrayOfPolygons polys;
//	ArrayOfPolygons lines;
// not copied	AllROIValues roivalues;

	fret.FirstFileName = d.FirstFileName;
	fret.Extension = d.Extension;
	fret.rootpath = d.rootpath;
	fret.ImageIndex=0;
	fret.NImages=1;
	fret.FileOffset=0;
	ImageSequence * pFretStack = new ImageSequence;

	fret.images = pFretStack;
	
	ImageSequence & FImage = *pFretStack;
	
	FImage.Sizex = image.Sizex;
	FImage.Sizey = image.Sizey;
	FImage.FirstIndex = 0;
	FImage.LastIndex = NImages/3-1;
	FImage.FileMode = 5;
	FImage.FileName = image.FileName;
	FImage.Title = image.Title;
	FImage.CurrentIndex = 0;
	FImage.initialized = false;
	FImage.imageready = false;

	AnaVisionImageFileStack fretstack;
	fretstack.Title = image.Title;
	//fretstack.Width = image.Sizex;
	//fretstack.Height = image.Sizey;
	
	AnaVisionImageFile ims[3];
	int NPixel = image.Sizex*image.Sizey;
	for (int k=0; k<3; k++){
		ims[k].Width = image.Sizex;
		ims[k].Height = image.Sizey;
		ims[k].data.resize(NPixel);
	}

	double back[3];

	CWaitCursor dummy;
	int offx = d.p.offsetx;
	int offy = d.p.offsety;
	int CalcType = 0;  // Mean
//	ShowFloat(image.Sizex, "sx");
//	ShowFloat(image.Sizey, "sy");


	for (int i=0; i<NImages/3; i++){
		AnaVisionImageFile fretimage;
		fretimage.data.resize(NPixel);
		fretimage.Width = image.Sizex;
		fretimage.Height = image.Sizey;
		for (int k=0; k<3; k++){
			image.CurrentIndex = 3*i+k;
			CellsClass * DummyCells;
			d.ReadImage(image.CurrentIndex, pDoc->id, DummyCells, false);

			back[k] = d.polys.CalcMeanValue(0.0, selectedpoly, pDoc->id.data,
							image.Sizex, image.Sizey, offx, offy, 0);
			//ShowFloat(back[k], "back[k]");
			for (int x=0; x<fretimage.Width; x++){
				for (int y=0;y<fretimage.Height; y++){
					//ims[k].SetPixel(pDoc->id.data[x][y]-back[k], x, y) ;
					ims[k].SetPixel(pDoc->id.data[x][y], x, y) ;

				}

			}
		}
		int ifret = 1;
		int idonor = 2;
		int iacceptor = 0;
		float BackFret = back[ifret];
		float BackDon = back[idonor];
		float BackAcc = back[iacceptor];
		float threshold = d.p.ThresholdFret;
//		ShowFloat(threshold, "threshold");
		float NThresh = sqrt(BackDon*BackAcc)*threshold;
		for (int x=0; x<fretimage.Width; x++){
			for (int y=0;y<fretimage.Height; y++){
				float values[3];
				float env[3];
				for (int k=0; k<3; k++){
					values[k] = ims[k].GetPixel(x, y);
					env[k] = ims[k].GetMeanEnvironment(x, y, 1);
				}
				float ff  = values[ifret]-BackFret;
				float don = values[idonor]-BackDon;
				float acc = values[iacceptor]-BackAcc;
				float envff = env[ifret];
				float envdon = env[idonor];
				float envacc = env[iacceptor];

				float fret=0.0;
				if ((envdon>BackDon*threshold) && (envacc>BackAcc*threshold) && (envff>BackFret*threshold)){
//					 && (don>BackDon*threshold)&& (acc>BackAcc*threshold)&& (ff>BackFret*threshold)){ 
					bool show = false;
					if (ff>200) show = true;
					fret = ff-d.p.FRETBTDonor*don - d.p.FRETBTAcceptor*acc;
					if (d.p.CalcNormalizedFret){
						fret = fret*32000.0/fabs(acc);
					}
					//fret = fret*100.0/sqrt(acc*acc + don*don);
					if (false){//(show){
						ShowFloat(ff, "fret");ShowFloat(don, "don");ShowFloat(acc, "acc");
						ShowFloat(values[0], "0");ShowFloat(values[1], "1");ShowFloat(values[2], "2");
						ShowFloat(fret, "cfret");
					}
					float nt = sqrt(fabs(don*acc));
					if (nt<=NThresh){
						fret = 0.0;
					}
					if (fret<0.0) fret = 0.0;
				}
				else{
					//fret=0.0;
//					ShowFloat(fret, "fret");
				}
				fretimage.SetPixel(fret, x, y);

			}
		}
		fretstack.AppendImage(fretimage);
	}


	
//	AH.Width = image.Sizex;
//	AH.Height = image.Sizey;
	fret.HS.AppendStack(fretstack);
	fret.CalcMaxNumberOfImages();
	
	image.CurrentIndex = oldindex;

	CString FileName = pDoc->GetPathName();
	FileName += ".FRET";
	CFileDialog fdiag( false, NULL, FileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);
	fdiag.m_ofn.lpstrTitle = "Export images on ASCII text files";
	if (fdiag.DoModal()!= IDOK){
		return;
	}
	FileName = fdiag.GetPathName();

	CFile file;
	file.Open(FileName, CFile::modeCreate | CFile::modeWrite);
	fret.SaveOnFile(file);


}

void CAnaVisionView::OnViewViewphasors(){
	ViewType = 7;

	CheckViewMenu();

	MyInvalidate();
}
void CAnaVisionView::GetPixelFromPhasor(double G, double S, CPoint & p){
	GetPixelFromPhasor(G, S, p.x, p.y);
}


void CAnaVisionView::GetPixelFromPhasor(double G, double S, LONG & px, LONG & py){
//	int OffsetXPhasor, OffsetYPhasor, NSizeGPhasor, NSizeSPhasor;
//	int bottomPhasor, NPixelGPhasor, NPixelSPhasor, fG, fS;
	if (G<=GMin){
		px=OffsetXPhasor;
	}
	else{
		if (G>=GMax){
			px=OffsetXPhasor + NSizeGPhasor;
		}
		else{
			px = OffsetXPhasor + int ((G-GMin)*ScaleXPhasor);
		}
	}
	if (S<=SMin){
		py=bottomPhasor;
	}
	else{
		if (S>=SMax){
			py = bottomPhasor - NSizeSPhasor;
		}
		else{
			py = bottomPhasor - int ((S-SMin)*ScaleYPhasor);
		}
	}
}

void CAnaVisionView::DrawPhasors(CDC * pDC){
	CWaitCursor dummy;
	CDC * mDC = new CDC;
	mDC->CreateCompatibleDC(pDC);
	if (!mDC){
		Alert("could not create compatible DC");
		return;
	}
	CDib * m_pDib =  NULL;

	if (phasor.initialized){
		phasor.WriteMask(PhasorCircleG, PhasorCircleS, PhasorCircleR, pDoc->WhiteMask);
		//pDoc->UsePhasorMask=true;

//	ShowFloat(radius.x, "rx");
//	ShowFloat(radius.y, "ry");

		vector < vector < int > > N;
		N.resize(NPixelSPhasor);
		for (int iy=0; iy<NPixelSPhasor; iy++){
			N[iy].resize(NPixelGPhasor);
		}

		int NMMax = phasor.CalcNumberOfPhasorsPerPixel(GMin, GMax, SMin, SMax, NPixelGPhasor, NPixelSPhasor, N);
		int NMMin = NMMax;
		for (int ix=0; ix<NPixelGPhasor; ix++){
			for (int iy=0; iy<NPixelSPhasor; iy++){
				int n=N[iy][ix];
				if ((n>0) && (n<NMMin)){
					NMMin = n;
				}
			}
		}
//	ShowFloat(NMMax, "NMax");
//	ShowFloat(NMMin, "NMMin");
		double logmin = log(double(NMMin));
		double logmax = log(double(NMMax));
		double logscale = 255.0/(logmax - logmin);
//	ShowFloat(N3Max, "N3Max");

		CRect r;
		GetClientRect(&r);

		int BitMapYSize= r.bottom-r.top;
		int BitMapXSize = r.right-r.left;

		m_pDib = new CDib(CSize(BitMapXSize,BitMapYSize), // size of the bitmap
					  24);            // bits per pixel
		VERIFY(m_pDib->CreateSection(mDC));
		BYTE* dibits = m_pDib->m_lpImage;
		for (int iy=0; iy<BitMapYSize; iy++){
			for (int ix=0; ix<BitMapXSize; ix++){
				int BitMapValue = RGB(255,255,255);
				RGBQUAD rr;
				rr.rgbBlue = GetBValue(BitMapValue);
				rr.rgbGreen = GetGValue(BitMapValue);
				rr.rgbRed = GetRValue(BitMapValue);
				m_pDib->WritePixel(ix, iy, rr);
			}
		}
		for (int ix=0; ix<NPixelGPhasor; ix++){
			for (int iy=0; iy<NPixelSPhasor; iy++){
				int n=N[iy][ix];
				if (n>0){
					int px = ix*fG+OffsetXPhasor;
					int py= bottomPhasor - OffsetYPhasor - iy*fS;
					double logn = log(double(n));
					BYTE b = BYTE( (logn-logmin)*logscale );
					int BitMapValue = RGB(b, 0, 0);
					RGBQUAD rr;
					rr.rgbBlue = GetRValue(BitMapValue);
					rr.rgbGreen = GetBValue(BitMapValue);
					rr.rgbRed = GetGValue(BitMapValue);
	
					for (int iix=0; iix<fG; iix++){
						for (int iiy=0; iiy<fS; iiy++){
//				pDC->SetPixel(px, py, RGB(n*255/NMax, 255, 255));
//				pDC->SetPixel(px, py, RGB(n*255/NMax, 0, 0));
							m_pDib->WritePixel(px+iix, py-iiy, rr);		
//						pDC->SetPixelV(px+iix, py-iiy, RGB(b, 0, 0));
//						pDC->SetPixel(px+iix, py-iiy, RGB(n*255/NMax, 0, 0));
//						pDC->SetPixelV(px+iix, py-iiy, RGB(n*255/NMax, 0, n*255/NMax));
						}
					}
				}
			}

//		Alert("Before");

			m_pDib->Draw(pDC, CPoint(0,0), CSize(BitMapXSize, BitMapYSize));
//		Alert("After");

		}
		pDoc->UsePhasorMask = true;
		GetTraces()->Invalidate();
	}

	CPoint Origin;
	GetPixelFromPhasor(0.0, 0.0, Origin);
	CPoint End;
	GetPixelFromPhasor(1.0, 0.0, End);
	pDC->MoveTo(Origin);
	for (int ix=0; ix<=NSizeGPhasor; ix++){
		double g=double(ix)/double(NSizeGPhasor);
		double s= sqrt(0.25 - (g-0.5)*(g-0.5));
		CPoint p;
		GetPixelFromPhasor(g, s, p);
		pDC->LineTo(p);
	}

//	pDC->MoveTo(UpperPoint);
//	pDC->LineTo(LowerPoint);
	pDC->MoveTo(Origin);
	pDC->LineTo(End);
//	pDC->Ellipse(Origin.px, UpperPont.y, Origin.px, -UpperPont.y); 
	DrawPhasorCircle(pDC);

	if (!phasor.initialized){
		return;
	}

	CPen * OldPen = pDC->SelectObject(&bluePen);
	pDC->MoveTo(Origin);
	for (int ix=0; ix<=NSizeGPhasor; ix++){
		double g=double(ix)/double(NSizeGPhasor);
		double s= sqrt(0.25 - (g-0.5)*(g-0.5));
		CPoint p;
		GetPixelFromPhasor(g, s, p);
		pDC->LineTo(p);
	}
	pDC->SelectObject(OldPen);
	DrawPhasorCircle(pDC);

	ReleaseDC(mDC);
	mDC->DeleteDC();
	delete mDC;
	if (m_pDib){
		m_pDib->Empty();
		delete m_pDib;
	}
	
	


//	pDC->TextOut(100,100, CString("Hello"));
}

void CAnaVisionView::GetEllipseCenterAndRadius(CPoint & center, CPoint & radius){
	this->GetPixelFromPhasor(PhasorCircleG, PhasorCircleS, center);
	this->GetPixelFromPhasor(PhasorCircleG+PhasorCircleR, PhasorCircleS+PhasorCircleR, radius);
	radius.x -= center.x;
	radius.y = abs(radius.y-center.y);
//	ShowFloat(center.x, "center.x");
//	ShowFloat(center.y, "center.y");
//	ShowFloat(radius.x, "r.x");
//	ShowFloat(radius.y, "r.y");
}


void CAnaVisionView::DrawPhasorCircle(CDC * pDC){
	CPoint center, radius;
//	this->GetPixelFromPhasor(PhasorCircleG, PhasorCircleS, center.x, center.y);
//	this->GetPixelFromPhasor(PhasorCircleG+PhasorCircleR, PhasorCircleS+PhasorCircleR, radius.x, radius.y);
//	radius.x; // center.x;
//	radius.y = abs(radius.y+center.y);
	GetEllipseCenterAndRadius(center, radius);
//	ShowFloat(radius.x, "rx");
//	ShowFloat(radius.y, "ry");
	pDC->SelectObject(GetStockObject(HOLLOW_BRUSH));
	CPen * OldPen = pDC->SelectObject(&greenPen);
	pDC->Ellipse(center.x-radius.x, center.y-radius.y, center.x+radius.x, center.y+radius.y);
	pDC->SelectObject(OldPen);
}
double CAnaVisionView::GetDistanceFromPhasorEllipse(CPoint p){
	CPoint center, radius;
	GetEllipseCenterAndRadius(center, radius);
	double temp1 = p.x-center.x;
	double temp2 = p.y-center.y;
	double temp3 = radius.x*radius.x;
	double temp4 = radius.y*radius.y;
	double temp5 = temp3*temp4;
	double diff = (temp1*temp1*temp4 + temp2*temp2*temp3 - temp5)/temp5;
	return diff;
}

bool CAnaVisionView::PointIsInPhasorCircle(CPoint p, bool & OnBorder){
	double diff = GetDistanceFromPhasorEllipse(p);
//	ShowFloat(diff, "diff");
	OnBorder = (fabs(diff)<=.2);//5e-2);
	return (diff<=0.0);
}

void CAnaVisionView::OnPhasorLoadphasortxtfiles(){
	CFileDialog diag( true, NULL, "Real Part", OFN_FILEMUSTEXIST, NULL, AfxGetMainWnd());

	if (diag.DoModal()!=IDOK){
		return;
	}
	CString FileName = diag.GetPathName();

	std::ifstream freal ( FileName);
/*
CFile freal;
	if (!freal.Open(FileName, CFile::modeRead)){
		Alert("could not open real file");
		return;
	}
*/
	CFileDialog diag2( true, NULL, "Imaginary Part", OFN_FILEMUSTEXIST, NULL, AfxGetMainWnd());

	if (diag2.DoModal()!=IDOK){
		return;
	}
	FileName = diag2.GetPathName();

//	CFile fimag;
	std::ifstream fimag ( FileName);
/*	if (!fimag.Open(FileName, CFile::modeRead)){
		Alert("could not open imaginary file");
		return;
	}
*/
	io myio;
	int px = PhasorSizeX;
	if (myio.InInt(px, "Size x") != IDOK){
		return;
	}
	if (px<=0) return;
	int py = PhasorSizeY;
	if (myio.InInt(py, "Size y") != IDOK){
		return;
	}
	if (py<=0) return;

	PhasorSizeX = px;
	PhasorSizeY = py;

	CWaitCursor dummy;
	phasor.SetLaserFreq(pDoc->dat.p.LaserFlimFreq);
	if (!phasor.ReadFromTextFiles(px, py, freal, fimag)){
		Alert0("could not read phasor files");
		return;
	}
	for (int i=0; i<pDoc->WhiteMask.size(); i++){
		pDoc->WhiteMask[i].resize(0);
	}
	pDoc->WhiteMask.resize(PhasorSizeY);
	for (int i=0; i<PhasorSizeY; i++){
		pDoc->WhiteMask[i].resize(PhasorSizeX);
	}
	this->OnViewViewphasors();

	Invalidate();

}
void CAnaVisionView::OnLButtonDownPhasor(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (ViewType == 7){ // mean values of ROI's
		bool OnBorder;
		bool InCircle = PointIsInPhasorCircle(point, OnBorder);
		if (OnBorder){
			OnSizingPhasorCircle = true;
			MousePhasorPoint = point;
			return;
		}
		if (InCircle){
			OnMovingPhasorCircle = true;
			MousePhasorPoint = point;
			return;
		}
	}
}

void CAnaVisionView::OnLButtonUpPhasor(UINT nFlags, CPoint point) {
	OnMovingPhasorCircle = OnSizingPhasorCircle = false;
	Invalidate();
}
void CAnaVisionView::OnMouseMovePhasor(UINT nFlags, CPoint point){
	if (OnMovingPhasorCircle){
		int dx = point.x - MousePhasorPoint.x;
		int dy = point.y - MousePhasorPoint.y;
		double dG = double(dx)/ScaleXPhasor;
		double dS = -double(dy)/ScaleYPhasor;
		this->PhasorCircleG += dG;
		this->PhasorCircleS += dS;
		MousePhasorPoint = point;
		Invalidate();
	}
	if (OnSizingPhasorCircle){
		CPoint center, radius;
		GetEllipseCenterAndRadius(center, radius);
		double rx = double(point.x-center.x)/ScaleXPhasor;
		double ry = double(point.y-center.y)/ScaleYPhasor;
		this->PhasorCircleR = sqrt(rx*rx+ry*ry);
		Invalidate();

/*		int ry = point.y-center.y;

		int dx = point.x - MousePhasorPoint.x;
		int dy = point.y - MousePhasorPoint.y;
		double dRG = double(dx)/double(NSizeGPhasor);
		double dRS = -double(dy)/double(NSizeSPhasor)/2.0;
		this->PhasorCircleG += dG;
		this->PhasorCircleS += dS;
		MousePhasorPoint = point;
		Invalidate();
*/
	}
}

Traces * CAnaVisionView::GetTraces(){
	CChildFrame * c = (CChildFrame *) this->GetParentFrame();
	return c->GetTraces();
}
