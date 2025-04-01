// BufferedDiffusionView.cpp : implementation of the CBufferedDiffusionView class
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
#include <math.h>

//#include "BufferedDiffusionDoc.h"
//#include "BufferedDiffusionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT CBufferedDiffusionView::TI = 0;

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionView

IMPLEMENT_DYNCREATE(CBufferedDiffusionView, CView)

BEGIN_MESSAGE_MAP(CBufferedDiffusionView, CView)
	//{{AFX_MSG_MAP(CBufferedDiffusionView)
	ON_COMMAND(ID_TEST_TEST, OnTestTest)
	ON_COMMAND(ID_EDIT_PARAMETERS, OnEditParameters)
	ON_COMMAND(ID_GO_GO, OnGoGo)
	ON_COMMAND(ID_GO_LINEARIZED, OnGoLinearized)
	ON_COMMAND(ID_GO_CLEARWINDIOW, OnGoClearwindiow)
	ON_COMMAND(ID_GO_SECONDLINEARIZED, OnGoSecondlinearized)
	ON_COMMAND(ID_GO_REDRAWOLDSIMULATION, OnGoRedrawoldsimulation)
	ON_COMMAND(ID_GO_SERIESOFSIMULATIONS, OnGoSeriesofsimulations)
	ON_COMMAND(ID_STOP_STOPSERIESSIMULATION, OnStopStopseriessimulation)
	ON_COMMAND(ID_FILE_SAVESIMULATIONSERIESONFILE, OnFileSavesimulationseriesonfile)
	ON_COMMAND(ID_FILE_LOADSIMULATIONSERIESFROMFILE, OnFileLoadsimulationseriesfromfile)
	ON_COMMAND(ID_STOP_SLOWALMOSTPAUSESIMULATION, OnStopSlowalmostpausesimulation)
	ON_COMMAND(ID_STOP_RESTART, OnStopRestart)
	ON_COMMAND(ID_STOP_SAVEENERGY, OnStopSaveenergy)
	ON_COMMAND(ID_STOP_CHANGENOOFSECONDSOUT, OnStopChangenoofsecondsout)
	ON_COMMAND(ID_GO_CONTINUESERIESSIMULATION, OnGoContinueseriessimulation)
	ON_COMMAND(ID_FILE_READDRDTTABLE, OnFileReaddrdttable)
	ON_COMMAND(ID_FILE_WRITEDRDTTABLE, OnFileWritedrdttable)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionView construction/destruction



CBufferedDiffusionView::CBufferedDiffusionView()
{
	// TODO: add construction code here

	UseTimerForEnergySave = false;
	firsttime = true;
	TSimulation = .40;
	TIntermediateSimulation = .05;
	NCurvesSimulation = 4;
	IndexMinParams = 0;
	UseTwoTimePoints = true;
	RefIndex = 0;
	ShowRef=true;
	SecondCount =0;



	int i=0;


/*
	ss.dr[i]= 20e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=2e-5;
	i++;
*/
	ss.dr[i]=20e-5;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=1e-6;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=2e-6;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=5e-6;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=1e-5;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=2e-5;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=5e-5;
	i++;

	ss.dr[i]= 20e-5;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 10e-5;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 5e-5;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 2e-5;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 10e-6;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 5e-6;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 2e-6;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 10e-7;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 5e-7;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 2e-7;
	ss.dt[i]=1e-4;
	i++;

	ss.dr[i]= 1e-7;
	ss.dt[i]=1e-4;
	i++;

	ss.NValues = i;
	

	Hi = Bfreei = Btotali = Ht = Bfreet = Btotalt = NULL;
	SolidBlack = new CPen(PS_SOLID, 1, RGB(0,0,0));
	WhitePen = new CPen(PS_SOLID, 1, RGB(255,255,255));
	BluePen = new CPen(PS_SOLID, 1, RGB(0,0,255));
	RedPen = new CPen(PS_SOLID, 1, RGB(255,0,0));
	GrayPen = new CPen(PS_SOLID, 1, RGB(200,200,200));
	NSlice = 0;
	Nt = 0;
	SimMem.Create(5);

	RedrawIndex = -1;
	RedrawColor = 0;

#define MAX_REDRAW_COLOR 10

	Colors = new CPen * [MAX_REDRAW_COLOR];

	Colors [0] = new CPen(PS_SOLID, 1,  RGB(200,200,200));
	Colors [1] = new CPen(PS_SOLID, 1,  RGB(0,0,0));
	Colors [2] = new CPen(PS_SOLID, 1,  RGB(255,0,0));
	Colors [3] = new CPen(PS_SOLID, 1,  RGB(0,255,0));
	Colors [4] = new CPen(PS_SOLID, 1,  RGB(0,0,255));
	Colors [5] = new CPen(PS_SOLID, 1,  RGB(255,255,0));
	Colors [6] = new CPen(PS_SOLID, 1,  RGB(255,165,00));
	Colors [7] = new CPen(PS_SOLID, 1,  RGB(255,20,147));
	Colors [8] = new CPen(PS_SOLID, 1,  RGB(139,35,35));
	Colors [9] = new CPen(PS_SOLID, 1,  RGB(135,206,250));


}

CBufferedDiffusionView::~CBufferedDiffusionView()
{
	DeAllocSpace();
	DeAllocTime();
	delete SolidBlack;
	delete BluePen;
	delete GrayPen;
	delete WhitePen;
	for (int i=0; i<MAX_REDRAW_COLOR; i++){
		delete Colors[i];
	}
	delete [] Colors;

}
void CBufferedDiffusionView::DeAllocSpace(){
	if (Hi) delete [] Hi;
	if (Bfreei) delete [] Bfreei;
	if (Btotali) delete [] Btotali;
	Hi = Bfreei = Btotali = NULL;
}

void CBufferedDiffusionView::DeAllocTime(){
	if (Ht) delete [] Ht;
	if (Bfreet) delete [] Bfreet;
	if (Btotalt) delete [] Btotalt;
	Ht = Bfreet = Btotalt = NULL;
}

void CBufferedDiffusionView::Alloc(){
	CBufferedDiffusionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int NNewSlice = pDoc->d.GetNumberOfSlices()+5;
	if (NNewSlice>NSlice) {
		DeAllocSpace();
		NSlice = NNewSlice;
//		ShowFloat(NSlice, "NSlice");
		Hi = new CPoint[NSlice];
		Bfreei = new CPoint[NSlice];
		Btotali = new CPoint[NSlice];
	}

//	int NSlice = pDoc->d.GetNumberOfSlices()+5;


//	int NSlice = 2*(2*pset->N)+5;
	int NNewt = 2*(pDoc->p.MaxIter*2/pDoc->p.UpdateEvery)+5;

	if (NNewt>Nt){
		DeAllocTime();
		Nt = NNewt;
//		ShowFloat(Nt, "Nt");

		Ht = new CPoint[Nt];
		Bfreet = new CPoint[Nt];
		Btotalt = new CPoint[Nt];
	}
}

void CBufferedDiffusionView::AllocReDraw(int index){
	CBufferedDiffusionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int NNewSlice = SimMem.GetNumberOfPoints(index)+1;
	if (NNewSlice>NSlice) {
		DeAllocSpace();
		NSlice = NNewSlice;
//		ShowFloat(NSlice, "NSlice");
		Hi = new CPoint[NSlice];
		Bfreei = new CPoint[NSlice];
		Btotali = new CPoint[NSlice];
	}

//	int NSlice = pDoc->d.GetNumberOfSlices()+5;


//	int NSlice = 2*(2*pset->N)+5;
	int NNewt = 2*SimMem.GetNumberOfCurves(index)+5;

	if (NNewt>Nt){
		DeAllocTime();
		Nt = NNewt;
//		ShowFloat(Nt, "Nt");

		Ht = new CPoint[Nt];
		Bfreet = new CPoint[Nt];
		Btotalt = new CPoint[Nt];
	}
}

BOOL CBufferedDiffusionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionView drawing

void CBufferedDiffusionView::OnDraw(CDC* pDC)
{
	CBufferedDiffusionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (firsttime){
		firsttime = false;
		CChildFrame * pf = GetChildFrame();
		pf->SetTimer(ID_TIMER_SAVE_ENERGY+TI, 1000, SaveEnergyTimer);
		TI++;

	}
	// TODO: add draw code for native data here
	DrawCircle(pDC);
}

void CBufferedDiffusionView::SaveEnergyTimer(
							HWND hWnd,      // handle of CWnd that called SetTimer
							UINT nMsg,      // WM_TIMER
							UINT nIDEvent,   // timer identification
							DWORD dwTime    // system time
							){

/*	if (nIDEvent != ID_TIMER_ACQUIRE){
		return;
	}
*/

	CWnd * Win = CWnd::FromHandle(hWnd);
	
	if (!Win){
		return;
	}
	Win->SendMessage(SAVEENEGRYTIMERMESSAGE, SAVEENEGRYTIMERMESSAGE, SAVEENEGRYTIMERMESSAGE); 
}




/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionView printing

BOOL CBufferedDiffusionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBufferedDiffusionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBufferedDiffusionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionView diagnostics

#ifdef _DEBUG
void CBufferedDiffusionView::AssertValid() const
{
	CView::AssertValid();
}

void CBufferedDiffusionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBufferedDiffusionDoc* CBufferedDiffusionView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBufferedDiffusionDoc)));
	return (CBufferedDiffusionDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionView message handlers

double ssx;
double ssy;
int ymax;

int Scallx(double r){
	return int(r*ssx);
}

int Scally(double pH){
//	ShowFloat(pH, "pH");
//	int result =ymax - int((pH-7.0)*ssy);
//	ShowFloat(result, "result");
	return ymax - int((pH-5.5)*ssy);
}

double Solu(double J, double D, double a, double r, double t){
	double h=1.0/a;

	double argr=r-a;
	double Dt = D*t;
	double root = sqrt(Dt);

	double arg1=argr/2.0/root;
	double erf1=erfcc(arg1);
	double arg2=arg1+h*root;
	double erf2=erfcc(arg2);

	double result = a*a*J/D/r*(erf1-erf2*exp(h*argr+Dt*h*h));
//	ShowFloat(r, "r");
//	ShowFloat(t,"t");
//	ShowFloat(erf1, "erf1");
//	ShowFloat(erf2, "erf2");

//	ShowFloat(result, "result");
//	ShowFloat(J/D, "J/D");
//	ShowFloat(J/D/r, "J/D/r");
	return result;

}

void CBufferedDiffusionView::DoTest(){
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}
	double H0=1e-7;
	double K=1e-7;
	double T=1e-3;
	double DB=5e-10;
	double DH=1e-8;
	double B0=K*T/(K+H0);
	double beta = B0/(K+H0);

	double lambda=B0/(H0+K);
	double D=(DH+lambda*DB)/(1.0+lambda);

	double a=0.5e-3;

	double J=10e-6; // uA
	J /= 96000; // moles/seconds
	J /= (4.0*3.14*a*a); // moles/seconds/square meter
	J /= 1000; // to go molar

//	ShowFloat(erfcc(0.0001), "erfcc(0.0001)");

	double rmax = 1e-3; // 5 m2
	double dr=(rmax-a)/1000.0;

	double dt = 0.2;

	int NIter = 100;

	CDC * pDC = GetDC();

	GetClientRect(&rect);

//	pDC->FillRect( &rect, pDC->GetCurrentBrush());

	ssx=double(rect.right)/rmax;
	ssy=double(rect.bottom-rect.top)/4.0;
	ymax = int(0.5*double(rect.bottom+rect.top));

	double t=dt/1000.0;
	for (int i=0; i<NIter; i++){
//		ShowFloat(t, "t");
		int j=0;
		double r=a; //+1e-6;
		while (r<=rmax){

			int x=Scallx(r);
//			ShowFloat(x, "x");
			double c=Solu(J/beta, D, a, r, t);

			double H=H0+c;
//			ShowFloat(H0, "H0");
//			ShowFloat(c, "c");

			double pH=-log10(H);
//			ShowFloat(pH, "pH");

			int y=Scally(pH);
//			ShowFloat(y, "y");

			if (j==0){
				pDC->MoveTo(x, y);
			}
			else{
				pDC->LineTo(x, y);
			}
			j++;


			r += dr;
		}

		
		
		t += dt;
	}






}

void CBufferedDiffusionView::OnTestTest() 
{
	// TODO: Add your command handler code here


	ShowInt( -3 % 2,"mod");
	return;
	double rrr=1;
	double s=1;
	double tt=1;

	dcomplex roots[10];

	SolveCubic(rrr, s, tt, roots);

	int i;
	for (i=0; i<3; i++){
		ShowFloat(roots[i].r, "r");
		ShowFloat(roots[i].i, "i");
	}

	return;



	double B=1;
	double A=10;

	io myio;

	static double r=10;

	if (myio.InDouble(r, "r") != IDOK) return;

	double b[5000];

	for (i=0; i<5000; i++) b[i]=0;

	b[1]=-B/A;

	for (i=3; i<100; i++){
		b[i] = double(i-2)*double(i-1)*b[i-2]/A;
//		ShowFloat(b[i], "b");
	}

	CString c;
	double pr=r;
	double sum=0;
	for (i=1; i<80; i++){
		char s[20];
		sum += b[i]/pr;
		pr *= r;
		if ( (i%2)==1){
			_gcvt(sum, 6, s);
			c+= CString(s)+CString(", ");
		}

	}
	Alert0(c);








return;
	static double t=1;


	if (myio.InDouble(t, "t")!= IDOK){
		return;
	}

	double Erfcc = erfcc(t);//le x); //Returns the complementary error function erfc(x) with fractional error everywhere less than 1.2 × 10-7.
	ShowFloat(Erfcc, "Erfcc");

	double Erffc1=erffc1(t);
	ShowFloat(Erffc1, "Erffc1");


	double Erff=erff(t);//(double x);
	ShowFloat(Erff, "Erff");
	double Erff1=erff1(t);//(double x);
	ShowFloat(Erff1, "Erff1");
 return;



	

	DoTest();
	return;
	double a[100], bb[100], cc[100], rr[100], u[100];


	for (i=0; i<100; i++){
		a[i]=bb[i]=cc[i]=rr[i]=0;
	}
	for (i=0; i<100; i++){
		rr[i]=bb[i]=1;
	}

	cc[0]=0.5;
	a[1]=0.5;
	a[2]=0.5;
	cc[1]=0.5;

	tridiag(a,bb,cc,rr,u,3);

	for (i=0; i<3; i++){
		ShowFloat(u[i], "u[i]");
	}


	
}

void CBufferedDiffusionView::OnEditParameters() 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}
	CBufferedDiffusionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Diffusion & d = pDoc->d;

	d.Setup();

//	return; 


	ParamsDiag diag;


	d.CopyParams(diag.d);
	diag.d.ClearArrays();

	diag.p = pDoc->p;


	if (diag.DoModal()!= IDOK){
		return;
	}


	diag.d.CopyParams(d);
	pDoc->p = diag.p;

	return;


}


void CBufferedDiffusionView::DrawCurves(CDC * pDC, int ni, int nt, BOOL gray, BOOL start){

	CBufferedDiffusionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (start){
//		pDC->FillRect( &rect, pDC->GetCurrentBrush());
	}
	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;

	char s[20];

	double time = d.time;
	_gcvt(time, 4, s);
//	_itoa(nt*pset, s, 10);
	CString c = CString(s);

	pDC->TextOut(1,10, CString("                                            "));
	pDC->TextOut(1,10, c);

	_gcvt(LastMean, 4, s);
	c=CString(s);
	pDC->TextOut(sx-100, sy-20, CString("                                            "));
	pDC->TextOut(sx-100, sy-20, c);

	if (start){
		_gcvt(d.r[d.M]/1e-6, 4, s);
		c=CString(s);
		
		pDC->TextOut(offxt-40, sy-20, CString("                                            "));
		pDC->TextOut(offxt-40, sy-20, c);

		CPen * OldPen = pDC->SelectObject(BluePen);

		pDC->MoveTo(0, sy/3);
		pDC->LineTo(sx, sy/3);

		pDC->MoveTo(0, 2*sy/3);
		pDC->LineTo(sx, 2*sy/3);

		pDC->MoveTo(offxt, 0);
		pDC->LineTo(offxt, sy);

/* QUESTION
	int x2 = pset->r[pset->index2]*scalexi;
	pDC->MoveTo(x2, sy);
	pDC->LineTo(x2, 0);

	_gcvt(pset->r[pset->index2]/1e-6, 4, s);
	c=CString(s);
	pDC->TextOut(x2, sy-20, c);
*/
		pDC->SelectObject(OldPen);
	}

	if (ni<0) return;
	if (nt<0) return;

	int i;

	CPen * OldPen;
	if (gray){
		OldPen = pDC->SelectObject(GrayPen);
	}


	pDC->MoveTo(Hi[0]);
	for (i=1; i<=ni; i++){
//		ShowFloat(Hi[i].x, "x");
//		ShowFloat(Hi[i].y, "y");
		pDC->LineTo(Hi[i]);
	}
	pDC->MoveTo(Bfreei[0]);
	for (i=1; i<=ni; i++){
		pDC->LineTo(Bfreei[i]);
	}
	pDC->MoveTo(Btotali[0]);
	for (i=1; i<=ni; i++){
		pDC->LineTo(Btotali[i]);
	}

	if (!gray){
		pDC->MoveTo(Ht[0]);
		for (i=1; i<=nt; i++){
			pDC->LineTo(Ht[i]);
		}
		pDC->MoveTo(Bfreet[0]);
		for (i=1; i<=nt; i++){
			pDC->LineTo(Bfreet[i]);
		}
/*
		pDC->MoveTo(Ht[0]);
		for (i=1; i<=nt; i++){
			pDC->LineTo(Btotalt[i]);
		}
*/
	}
	if (gray){
		pDC->SelectObject(OldPen);
	}


}
void CBufferedDiffusionView::ReDrawCurves(CDC * pDC, int ni){

	CBufferedDiffusionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (ni<0) return;

	int i;

	CPen * OldPen;

	bool Release = false;
	if (!pDC){
		Release = true;
		pDC = GetDC();
	}

	OldPen = pDC->SelectObject(Colors[RedrawColor]);


	pDC->MoveTo(Hi[0]);
	for (i=1; i<ni; i++){
		pDC->LineTo(Hi[i]);
	}
	pDC->MoveTo(Bfreei[0]);
	for (i=1; i<ni; i++){
		pDC->LineTo(Bfreei[i]);
	}
/*
	pDC->MoveTo(Btotali[0]);
	for (i=1; i<=ni; i++){
		pDC->LineTo(Btotali[i]);
	}
*/
	pDC->SelectObject(OldPen);

	double dr = SimMem.Getdr(RedrawIndex);
	double dt = SimMem.Getdt(RedrawIndex);
	double Norm = SimMem.GetNorm(RedrawIndex);

	char s[20];

	_gcvt(dr, 4, s);
	CString c = CString(" dr = ") + CString(s);

	pDC->TextOut(20,20, CString("                                            "));
	pDC->TextOut(20,20, c);

	_gcvt(dt, 4, s);
	c = CString(" dt = ") + CString(s);

	pDC->TextOut(20,40, CString("                                            "));
	pDC->TextOut(20,40, c);

	_gcvt(Norm, 4, s);
	c = CString(" Norm= ") + CString(s);

	pDC->TextOut(20,60, CString("                                            "));
	pDC->TextOut(20,60, c);

	if (Release){
		ReleaseDC(pDC);
	}

}

BOOL CBufferedDiffusionView::MakeFileName(int fileno, CString & filename){
	CBufferedDiffusionDoc* pDoc = GetDocument();
	filename = pDoc->p.OutFileName;
	if (filename.GetLength()<1) return FALSE;

	char s[20];
	_itoa(fileno, s, 10);
	CString a = CString(s);
	if (a.GetLength()<3){
		a = '0' + a;
	}
	if (a.GetLength()<3){
		a = '0' + a;
	}
	filename += a + ".asc";
//	Alert0(filename);
	return TRUE;
}

void CBufferedDiffusionView::OnGoGoOld() {
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}

	OnDoGoGo(1);
}

void CBufferedDiffusionView::OnGoGo() {
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}

	OnDoGoGo(1);
}

void CBufferedDiffusionView::SetupDiffusion(){
	CBufferedDiffusionDoc* pDoc = GetDocument();
	Diffusion & d = pDoc->d;
	Params & pset = pDoc->p;
	d.Setup();
	SimMem.AddSimulation(d.M+1, pset.MaxIter/pset.UpdateEvery + 1, d.pHbulk, d.Bbulk, d.DR0, d.DT0, d.r);
	MemorizeSimulation = true;
}

void CBufferedDiffusionView::OnDoGoGoOld(int mode) 
{
	// TODO: Add your command handler code here

	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;


	MemorizeSimulation = false;

	switch (mode){
		case 0 : d.SetupLinearized(); break;
		case 1 : SetupDiffusion();			break;
		case 2 : d.SetupLinearized2(); break;
		default : SetupDiffusion();			break;
	}


/*	if (linear){
		d.SetupLinearized();
	}
	else{
		d.Setup();
	}
*/
	Alloc();

	CDC * pDC = GetDC();


	GetClientRect(&rect);

//	pDC->FillRect( &rect, pDC->GetCurrentBrush());

	sx=rect.right;
	sy=rect.bottom;


	scalexi = double(sx)/1.1/double(d.r[d.M]-d.r[0]);
//	ShowFloat(d.r[d.M], "r[M]");
//	ShowFloat(d.r[d.M], "rmax");
//	scalext = double(sx)/2.0/double(pset.MaxIter+1);
	scalext = double(sx)/20.0/d.GetMaxTime(pset.MaxIter);

//	ShowFloat(scalext, "scalext");
	Hbulk = pow(10.0, -d.pHbulk);
//	ShowFloat(Hbulk, "Hbulk");

	scaleyH = -double(sy)/3.0/2.0/Hbulk;

	scaleyH = -double(sy)/2/pset.RangepH;
//		ShowFloat(scaleyH, "scaleyH");
//	scaleyB = -double(sy)/3.0/d.T;
	scaleyB = -double(sy)/3.0/(2*d.Bbulk);

	offxi = 0;
	
	offxt = int(double(sx)/1.1);

	offyH = sy-2*sy/6+sy/10;
//	ShowFloat(offyH, "offyH");

	offyBfree = 2*sy/3;
	offyBtotal = sy/3;

	int i;
	CWaitCursor dummy;

	int idraw=1;
	int fileno=0;
	double t=0;
	double nprot = 0;
	for (i=0; i<pset.MaxIter; i++){

		iter = i;
		t=d.time;

//		char s[20];
//		_itoa(i, s, 10);
//		CString c = CString(s);
//		pDC->TextOut(1,10, c);

		idraw++;
		bool actuallyused = false;
		if ( (idraw==(pset.UpdateEvery)) || (idraw==(2*pset.UpdateEvery-1))){
			actuallyused = true;
		}
		if ( (iter==0) || (idraw==(pset.UpdateEvery+1)) || (idraw==(2*pset.UpdateEvery))){
//			if (iter==0){
//				pDC->FillRect( &rect, pDC->GetCurrentBrush());
//			}

			int index = (i+1)/pset.UpdateEvery;
			if (iter>1){
				DrawCurves(pDC, d.M, index, TRUE, FALSE);
			}
			BOOL DoWrite = FALSE;
			CFile outfile;
			CString filename;
			if (pDoc->p.WriteOutFiles){
				if (MakeFileName(fileno, filename)){
					if (outfile.Open(filename, CFile::modeCreate | CFile::modeWrite)){
//						Alert0("hi");
						DoWrite = TRUE;
						fileno++;
					}
					else{
						Alert0("COULD NOT OPEN FILE");
					}
				}
			}

			if (MemorizeSimulation){
				SimMem.AddCurve(d.H, d.B);
			}
			double H, B, BH;
			double meanH=0;
			double meanB=0;

			double nprotthis = 0;
			int j;
			for (j=0; j<=d.M; j++){
				d.GetValues(j, H, B, BH);
				double r1=pDoc->d.r[j];
				double r2;
				if (j<d.M){
					r2=pDoc->d.r[j+1];
				}
				else{
					r2=2*pDoc->d.r[d.M] -pDoc->d.r[d.M-1];
				}

				double Vol = 4.0*PIGRECO/3.0*(r2*r2*r2-r1*r1*r1)*1000.0; // in liters

				
				nprotthis += Vol*pDoc->d.GetAddedProtonConcInMolar(H); 
			}

			nprot += nprotthis;

			if (DoWrite){
//				Alert0("0");
				WriteShortDoubleOnFile(d.time, outfile);
//				Alert0("1");
				WriteShortDoubleOnFile(nprotthis, outfile);
				WriteShortDoubleOnFile(nprotthis/d.time*FARADAY, outfile);
				WriteEndOfLine(outfile);
			}

			for (j=0; j<=d.M; j++){
				d.GetValues(j, H, B, BH);

				if (DoWrite){
//					WriteShortDoubleOnFile(pDoc->d.r[j], outfile);
//					WriteShortDoubleOnFile(H, outfile);
//					WriteShortDoubleOnFile(B, outfile);
					WriteDoubleOnFile(pDoc->d.r[j], outfile);
					WriteDoubleOnFile(H, outfile);
					WriteDoubleOnFile(B, outfile);
					WriteEndOfLine(outfile);
				}


				H =- log10(H)-d.pHbulk;
				meanH += H;
				meanB += B;
//				ShowFloat(j, "j");
//				ShowFloat(H, "H");
				Hi[j].x = Bfreei[j].x = Btotali[j].x = offxi + scalexi*(d.r[j]-d.r[0]);
				Hi[j].y = offyH + scaleyH*H;

				Bfreei[j].y = offyBfree + scaleyB*B;
				Btotali[j].y = offyBtotal + scaleyB*(B+BH)/2;
//				ShowFloat(j, "j");
//				ShowFloat(Hi[j].x, "Hi[j].x");
//				ShowFloat(Hi[j].y, "Hi[j].y");
			}

			if (DoWrite){
				outfile.Close();
			}

			d.GetValues(5, H, B, BH);

			H =- log10(H)-d.pHbulk;
			H=meanH/double(d.M+1);
			B=meanB/double(d.M+1);
//			ShowFloat(H, "H");
			LastMean = H+d.pHbulk;
//		ShowFloat(H, "H");
//			ShowFloat(index, "index");
//			ShowFloat(i, "i");
//			Ht[index].x = Bfreet[index].x = Btotalt[index].x = offxt + scalext*double(i);
			Ht[index].x = Bfreet[index].x = Btotalt[index].x = offxt + scalext*d.time;
//		ShowFloat(Ht[i].x, "Ht[i].x");
			Ht[index].y = offyH + scaleyH*H;
//			ShowFloat(Ht[index].y, "Ht[index].y");

			Bfreet[index].y = offyBfree + scaleyB*B;
			Btotalt[index].y = offyBtotal + scaleyB*(B+BH)/2;
			DrawCurves(pDC, d.M, index, FALSE, iter==0);
			if (DoWrite){
				pDC->TextOut(80,10, filename);
			}
		}
		if (idraw==(2*pset.UpdateEvery)){
			idraw=1;
		}

		if (InKey()==STOPKEY){
			ReleaseDC(pDC);
			return;
		}

//		_itoa(i, s, 10);
//		c = CString(s);
//		pDC->TextOut(1,10, c);

//		ShowFloat(d.H[5], "H[5]");
		d.Step(actuallyused);
		actuallyused = false;
	}

	ReleaseDC(pDC);


	
}
void CBufferedDiffusionView::OnDoGoGo(int mode) 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}

	UseTimerForEnergySave = false;
	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;


	MemorizeSimulation = false;

	switch (mode){
		case 0 : d.SetupLinearized(); break;
		case 1 : SetupDiffusion();			break;
		case 2 : d.SetupLinearized2(); break;
		default : SetupDiffusion();			break;
	}


/*	if (linear){
		d.SetupLinearized();
	}
	else{
		d.Setup();
	}
*/
//		Alert("Hi2");
	Alloc();

//	CDC * pDC = GetDC();


	GetClientRect(&rect);

//	pDC->FillRect( &rect, pDC->GetCurrentBrush());

	sx=rect.right;
	sy=rect.bottom;


	scalexi = double(sx)/1.1/double(d.r[d.M]-d.r[0]);
//	ShowFloat(d.r[d.M], "r[M]");
//	ShowFloat(d.r[d.M], "rmax");
//	scalext = double(sx)/2.0/double(pset.MaxIter+1);
	scalext = double(sx)/20.0/d.GetMaxTime(pset.MaxIter);

//	ShowFloat(scalext, "scalext");
	Hbulk = pow(10.0, -d.pHbulk);
//	ShowFloat(Hbulk, "Hbulk");

	scaleyH = -double(sy)/3.0/2.0/Hbulk;

	scaleyH = -double(sy)/2/pset.RangepH;
//		ShowFloat(scaleyH, "scaleyH");
//	scaleyB = -double(sy)/3.0/d.T;
	scaleyB = -double(sy)/3.0/(2*d.Bbulk);

	offxi = 0;
	
	offxt = int(double(sx)/1.1);

	offyH = sy-2*sy/6+sy/10;
//	ShowFloat(offyH, "offyH");

	offyBfree = 2*sy/3;
	offyBtotal = sy/3;

	if (mode == 1){
		if (pset.ReadStartingFile){
			DoReadStartingFile();
		}
	}

	CWaitCursor dummy;

	idraw=1;
	fileno=0;
	t=0;
	nprot = 0;
//	Alert("Hi0");
	pGoThread = new GoThread(&pf->StatusSimThread, &pf->index, pf->m_hWnd, &d, &pset, &ss, &SimMem);
	
	KindOfThread = 1;

	pGoThread->m_pThreadParams = NULL;
	pGoThread->m_bAutoDelete = TRUE;
	if (!pGoThread->CreateThread(CREATE_SUSPENDED)){
		return;
	}
	VERIFY(pGoThread->SetThreadPriority(THREAD_PRIORITY_NORMAL));

	pf->StatusSimThread = 1;
	pf->IsSimulating = TRUE;
	
//	Alert("Hi");
	pGoThread->ResumeThread();

	SimMem.DoShow = false;

	DrawCircle(NULL);
}

void CBufferedDiffusionView::DoReadStartingFile(){
	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;

	CString InFile;
	CFileDialog
		diag( TRUE, NULL, InFile, OFN_FILEMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return ;
	}

	InFile = diag.GetPathName();

	FILE * f;

	f=fopen(InFile, "rb");

	if (!f){
		Alert("could not open file");
		return;
	}

	CWaitCursor dummy;

	int j;
	double value;
	if (!ReadDouble(value, f)){
		Alert("could not read time value in first row");
	}

	d.time = value;


	for (j=0; j<2; j++){
		if (!ReadDouble(value, f)){
			Alert("could not read value in first row");
			fclose(f);
			return;
		}
	}
	for (j=0; j<=d.M; j++){
		double r, H, B;
		if (!ReadDouble(r, f)){
			fclose(f);
			ShowFloat(j, "Error reading r j");
			return;
		}
		if (!ReadDouble(H, f)){
			fclose(f);
			ShowFloat(j, "Error reading H j");
			return;
		}
		if (!ReadDouble(B, f)){
			fclose(f);
			ShowFloat(j, "Error reading B j");
			return;
		}
		d.hnew[j]=H+d.KD;
		d.H[j]=H;
		d.B[j]=B;
//		ShowFloat(d.hnew[j], "hnew");

	}
	fclose(f);

}

void CBufferedDiffusionView::MessageDrawCurves(int index){
	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;
	
	CDC * pDC = GetDC();

//	ShowFloat(index, "index");
	DrawCurves(pDC, d.M, index, TRUE, FALSE);
	BOOL DoWrite = FALSE;
	CFile outfile;
	CString filename;
	if (pDoc->p.WriteOutFiles){
		if (MakeFileName(fileno, filename)){
			if (outfile.Open(filename, CFile::modeCreate | CFile::modeWrite)){
//				Alert0("hi");
				DoWrite = TRUE;
				fileno++;
			}
			else{
				Alert0("COULD NOT OPEN FILE");
			}
		}
	}
	if (MemorizeSimulation){
		SimMem.AddCurve(d.H, d.B);
	}
	double H, B, BH;
	double meanH=0;
	double meanB=0;
	double nprotthis = 0;
	int j;
	for (j=0; j<=d.M; j++){
		d.GetValues(j, H, B, BH);
		double r1=pDoc->d.r[j];
		double r2;
		if (j<d.M){
			r2=pDoc->d.r[j+1];
		}
		else{
			r2=2*pDoc->d.r[d.M] -pDoc->d.r[d.M-1];
		}
		double Vol = 4.0*PIGRECO/3.0*(r2*r2*r2-r1*r1*r1)*1000.0; // in liters
		nprotthis += Vol*pDoc->d.GetAddedProtonConcInMolar(H); 
	}

	nprot += nprotthis;

	if (DoWrite){
//				Alert0("0");
		WriteShortDoubleOnFile(d.time, outfile);
//				Alert0("1");
		WriteShortDoubleOnFile(nprotthis, outfile);
		WriteShortDoubleOnFile(nprotthis/d.time*FARADAY, outfile);
		WriteEndOfLine(outfile);
	}

	for (j=0; j<=d.M; j++){
		d.GetValues(j, H, B, BH);

		if (DoWrite){
//				WriteShortDoubleOnFile(pDoc->d.r[j], outfile);
//					WriteShortDoubleOnFile(H, outfile);
//					WriteShortDoubleOnFile(B, outfile);
			WriteDoubleOnFile(pDoc->d.r[j], outfile);
			WriteDoubleOnFile(H, outfile);
			WriteDoubleOnFile(B, outfile);
			WriteEndOfLine(outfile);
		}
		H =- log10(H)-d.pHbulk;
		meanH += H;
		meanB += B;
//				ShowFloat(j, "j");
//				ShowFloat(H, "H");
		Hi[j].x = Bfreei[j].x = Btotali[j].x = offxi + scalexi*(d.r[j]-d.r[0]);
		Hi[j].y = offyH + scaleyH*H;

		Bfreei[j].y = offyBfree + scaleyB*B;
		Btotali[j].y = offyBtotal + scaleyB*(B+BH)/2;
//				ShowFloat(j, "j");
//				ShowFloat(Hi[j].x, "Hi[j].x");
//				ShowFloat(Hi[j].y, "Hi[j].y");
	}

	if (DoWrite){
		outfile.Close();
	}

	d.GetValues(5, H, B, BH);

	H =- log10(H)-d.pHbulk;
	H=meanH/double(d.M+1);
	B=meanB/double(d.M+1);
//			ShowFloat(H, "H");
	LastMean = H+d.pHbulk;
//		ShowFloat(H, "H");
//			ShowFloat(index, "index");
//			ShowFloat(i, "i");
//			Ht[index].x = Bfreet[index].x = Btotalt[index].x = offxt + scalext*double(i);
	Ht[index].x = Bfreet[index].x = Btotalt[index].x = offxt + scalext*d.time;
//		ShowFloat(Ht[i].x, "Ht[i].x");
	Ht[index].y = offyH + scaleyH*H;
//			ShowFloat(Ht[index].y, "Ht[index].y");

	Bfreet[index].y = offyBfree + scaleyB*B;
	Btotalt[index].y = offyBtotal + scaleyB*(B+BH)/2;
	DrawCurves(pDC, d.M, index, FALSE, iter==0);
	iter++;
	if (DoWrite){
		pDC->TextOut(80,10, filename);
	}


	ReleaseDC(pDC);
}

void CBufferedDiffusionView::MessageGoFinished(){
	CChildFrame * pf = GetChildFrame();
	pf->IsSimulating = FALSE;
	pf->StatusSimThread = 0;
//	delete pSimThread;
	pGoThread=NULL;
	DrawCircle(NULL);
}

void CBufferedDiffusionView::OnGoLinearized() 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}
	OnDoGoGo(0);

}


void CBufferedDiffusionView::OnGoClearwindiow() 
{
	// TODO: Add your command handler code here
	CDC * pDC = GetDC();
	CRect r;
	GetClientRect(&r);
	pDC->FillRect( &r, pDC->GetCurrentBrush());
	DrawCircle(pDC);
	ReleaseDC(pDC);	
}

void CBufferedDiffusionView::OnGoSecondlinearized() 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}
	OnDoGoGo(2);
	
}

void CBufferedDiffusionView::OnGoRedrawoldsimulation() 
{
	// TODO: Add your command handler code here


	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;



	RedrawDiag diag;
	diag.RedrawIndex = RedrawIndex;
	diag.Color = RedrawColor;
	diag.NSimul = SimMem.GetNumberOfSimulations();
	diag.pMem = &(SimMem);
	diag.m_RefIndex = RefIndex;
	diag.m_ShowRef = ShowRef;

	if (diag.DoModal() != IDOK){
		return;
	}

	RefIndex = diag.m_RefIndex;
	ShowRef = diag.m_ShowRef;
	if (ShowRef){
		RedrawIndex = RefIndex;
		RedrawColor = 0;
		DoRedraw();
	}
	RedrawIndex = diag.RedrawIndex;
	RedrawColor = diag.Color;
	DoRedraw();
}

void CBufferedDiffusionView::DrawCircle(CDC * pDC){
	bool Release = false;
	if (!pDC){
		Release = true;
		pDC = GetDC();
	}

	GetClientRect(&rect);
	
	CChildFrame * pf = GetChildFrame();
	CPen * OldPen;
	if (pf->IsSimulating){
		if (this->KindOfThread==0){
			OldPen = pDC->SelectObject(BluePen);
		}
		else{
			OldPen = pDC->SelectObject(RedPen);
		}
	}
	else{
		OldPen = pDC->SelectObject(WhitePen);
	}

	int x=300;
	int y=20;
	int s1=5;
	int s2=10;

	pDC->Ellipse(x-s2,y-s2,x+s2,y+s2);
	pDC->Ellipse(x-s1,y-s1,x+s1,y+s1);
	
	pDC->SelectObject(OldPen);

	if (Release){
		ReleaseDC(pDC);
	}
}



void CBufferedDiffusionView::DoRedraw(){

	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;

	AllocReDraw(RedrawIndex);

	GetClientRect(&rect);

	sx=rect.right;
	sy=rect.bottom;

	int NP=SimMem.GetNumberOfPoints(RedrawIndex);
	if (NP<2){
		return;
	}
	int NCurves = SimMem.GetNumberOfCurves(RedrawIndex);
	if (NCurves<1){
		return;
	}

	scalexi = double(sx)/1.1/(SimMem.GetRadius(RedrawIndex, NP-1)-SimMem.GetRadius(RedrawIndex, 0));

//	ShowFloat(scalext, "scalext");

	Hbulk = SimMem.GetpHBulk(RedrawIndex);

//	ShowFloat(Hbulk, "Hbulk");

	scaleyH = -double(sy)/2/pset.RangepH;
	scaleyB = -double(sy)/3.0/(2*SimMem.GetInitialFreeBuffer(RedrawIndex));

	offxi = 0;
	

	offyH = sy-2*sy/6+sy/10;
	offyBfree = 2*sy/3;
//	offyBtotal = sy/3;

	int i;
	CWaitCursor dummy;

//	ShowFloat(d.pHbulk, "d.pHbulk");
	int fileno=0;
	double t=0;
	double nprot = 0;
	for (i=0; i<NCurves; i++){
/*			BOOL DoWrite = FALSE;
			CFile outfile;
			CString filename;
			if (pDoc->p.WriteOutFiles){
				if (MakeFileName(fileno, filename)){
					if (outfile.Open(filename, CFile::modeCreate | CFile::modeWrite)){
//						Alert0("hi");
						DoWrite = TRUE;
						fileno++;
					}
					else{
						Alert0("COULD NOT OPEN FILE");
					}
				}
			}
*/
			double H, B; //, BH;

/*
			if (DoWrite){
//				Alert0("0");
				WriteShortDoubleOnFile(d.time, outfile);
//				Alert0("1");
				WriteShortDoubleOnFile(nprotthis, outfile);
				WriteShortDoubleOnFile(nprotthis/d.time*FARADAY, outfile);
				WriteEndOfLine(outfile);
			}
*/
			for (int j=0; j<NP; j++){
				SimMem.GetValues(RedrawIndex, i, j, H, B);

/*
				if (DoWrite){
//					WriteShortDoubleOnFile(pDoc->d.r[j], outfile);
//					WriteShortDoubleOnFile(H, outfile);
//					WriteShortDoubleOnFile(B, outfile);
					WriteDoubleOnFile(pDoc->d.r[j], outfile);
					WriteDoubleOnFile(H, outfile);
					WriteDoubleOnFile(B, outfile);
					WriteEndOfLine(outfile);
				}
*/

				H =- log10(H)-SimMem.GetpHBulk(RedrawIndex);
				Hi[j].x = Bfreei[j].x = Btotali[j].x = offxi + scalexi*(SimMem.GetRadius(RedrawIndex, j)-SimMem.GetRadius(RedrawIndex, 0));
				Hi[j].y = offyH + scaleyH*H;

				Bfreei[j].y = offyBfree + scaleyB*B;
				Btotali[j].y = 0;
			}
			ReDrawCurves(NULL, NP);

/*
			if (DoWrite){
				outfile.Close();
			}
*/
		}

}

CChildFrame * CBufferedDiffusionView::GetChildFrame(){
	return (CChildFrame *)(this->GetParent());
//	return (CChildFrame *)(this->GetParent()->GetParent()->GetParent());
}

void CBufferedDiffusionView::OnGoSeriesofsimulations() {
	DoGoSeriesofsimulations(false);
}
void CBufferedDiffusionView::DoGoSeriesofsimulations(bool restart) 
{
	// TODO: Add your command handler code here
	
	CChildFrame * pf = GetChildFrame();
	if (pf->IsSimulating){
		Alert("Is simulating");
		return;
	}

	int FirstIndex = 0;

	SimulSeriesDiag diag;

	diag.ss = ss;
	diag.m_NCurves = NCurvesSimulation;
	diag.m_Time = TSimulation;
	diag.m_IntermediateTime = TIntermediateSimulation;
	diag.m_UseTwoTimePoints = UseTwoTimePoints;

	if (diag.DoModal() != 1){
		return;
	}

	if (diag.m_Time<=0.0){
		return;
	}

	if (diag.m_IntermediateTime<=0.0){
		diag.m_IntermediateTime = diag.m_Time/2;
	}

	if (diag.m_NCurves<0){
		return;
	}


	ss=diag.ss;
	NCurvesSimulation = diag.m_NCurves;
	TSimulation = diag.m_Time;
	TIntermediateSimulation = diag.m_IntermediateTime;
	if (TIntermediateSimulation>=TSimulation){
		TIntermediateSimulation = TSimulation/2.0;
	}

	UseTwoTimePoints = diag.m_UseTwoTimePoints;

	IndexMinParams = ss.GetMinIndex();
//	ShowFloat(IndexMinParams, "min");
///	ShowFloat(ss.dr[IndexMinParams], "dr");
//	ShowFloat(ss.dt[IndexMinParams], "dt");

	if (ss.NValues<1){
		return;
	}

	if (restart){
		FirstIndex = SimMem.GetNumberOfSimulations();
		if (FirstIndex == 0){
			restart = false;
		}
		if (FirstIndex>ss.NValues){
			Alert("No free dr/dt pairs");
			return;
		}
	}
	if (!restart){
		SimMem.Create(ss.NValues);
	}

	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;


	d.CopyParams(SaveD);

	SaveSet = pset;


//****************************


	if (UseTwoTimePoints){
		pSimThread = new SimThread(&pf->StatusSimThread, pf->m_hWnd, &d, &pset, &ss, TSimulation, FirstIndex, TIntermediateSimulation, &SimMem);
	}
	else{
		pSimThread = new SimThread(&pf->StatusSimThread, pf->m_hWnd, &d, &pset, &ss, TSimulation, FirstIndex, NCurvesSimulation, &SimMem);
	}

	KindOfThread = 0;


	pSimThread->m_pThreadParams = NULL;
	pSimThread->m_bAutoDelete = TRUE;
	if (!pSimThread->CreateThread(CREATE_SUSPENDED)){
		return;
	}
	VERIFY(pSimThread->SetThreadPriority(THREAD_PRIORITY_NORMAL));

	pf->StatusSimThread = 1;
	pf->IsSimulating = TRUE;
	
	pSimThread->ResumeThread();

	SimMem.DoShow = false;

	DrawCircle(NULL);

}

void CBufferedDiffusionView::OnStopStopseriessimulation() 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (!pf->IsSimulating){
		return;
	}
	pf->StatusSimThread = 2;
}

void CBufferedDiffusionView::SimulationFinished(){
	CChildFrame * pf = GetChildFrame();
	pf->IsSimulating = FALSE;
	pf->StatusSimThread = 0;
//	delete pSimThread;
	pSimThread=NULL;


	int NSimul=SimMem.GetNumberOfSimulations();
//	ShowFloat(NSimul, "NSimul");

	if (NSimul<1) {
		DrawCircle(NULL);
		return;
	}

	int indexmin = -NSimul+IndexMinParams+1;
	int NMaxp=SimMem.GetNumberOfPoints(indexmin);
//	ShowFloat(NMaxp, "NMaxp");
	if (NMaxp<1) {
		DrawCircle(NULL);
		return;
	}

	{ 
		for (int i=0; i<NSimul; i++){
			int N=SimMem.GetNumberOfPoints(-i);
			if (N>NMaxp){
				NMaxp=N;
			}
		}
	}

	int ncurve = SimMem.GetNumberOfCurves(indexmin);

//	ShowFloat(ncurve, "ncurve");
	if (ncurve<1){
		return ;
	}

	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	Diffusion & d = pDoc->d;

	
	double * HMin = new double[NMaxp];
	double * Htest = new double[NMaxp];
	double * Htestconv = new double[NMaxp];

	double B;

	double Norm = 0;
	double Hbulk = pow(10.0, -SimMem.GetpHBulk(indexmin));
	for (int j=0; j<NMaxp; j++){
		SimMem.GetValues(indexmin, ncurve-1, j, HMin[j], B);
		double tp1=HMin[j]-Hbulk;
		Norm += tp1*tp1;
	}
//	Alert0("hi");
//	SimMem.DoShow = true;

	for (int i=0; i<ss.NValues; i++){
		int NP=SimMem.GetNumberOfPoints(-i);
		if (NP>0){
			int ncurve = SimMem.GetNumberOfCurves(-i);
//			ShowFloat(NP, "NP");
			int j;
			for (j=0; j<NP; j++){
				SimMem.GetValues(-i, ncurve-1, j, Htest[j], B);
			}
			double r=double(NMaxp)/double(NP);
			for (j=0; j<NMaxp; j++){
				int k= int(double(j)/r);
//			ShowFloat(k, "k");
				if (k>=NP){
					k=NP-1;
				}
				Htestconv[j]=Htest[k];
			}
			double SD=0;
			for (j=0; j<NMaxp; j++){
				double tp1 = Htestconv[j]-HMin[j];
				SD += tp1*tp1;
			}
			ss.norm[ss.NValues-i-1]=SD/Norm;
			SimMem.SetNorm(-i, SD/Norm);
		}
			//		ShowFloat(SD/Norm, "N");
	}


	SaveD.CopyParams(d);
	pset = SaveSet;

	delete [] Htest;
	delete [] HMin;
	delete [] Htestconv;

	DrawCircle(NULL);

}

void CBufferedDiffusionView::MessageRedraw(){
	RedrawIndex = 0;
	RedrawColor++;
	if (RedrawColor>=MAX_REDRAW_COLOR){
		RedrawColor = 0;
	}

	DoRedraw();
}

void CBufferedDiffusionView::OnFileSavesimulationseriesonfile() 
{
	// TODO: Add your command handler code here

	CString OutFile;
	CFileDialog
		diag( FALSE, NULL, OutFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

	if (diag.DoModal()!=IDOK){
		return ;
	}
	OutFile = diag.GetPathName();
	CFile file ( OutFile, CFile::modeCreate | CFile::modeWrite);
	CWaitCursor dummy;
	SimMem.SaveOnFile(file);

}

void CBufferedDiffusionView::OnFileLoadsimulationseriesfromfile() 
{
	// TODO: Add your command handler code here
	CString InFile;
	CFileDialog
		diag( TRUE, NULL, InFile, OFN_FILEMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return ;
	}
	InFile = diag.GetPathName();
	CFile file ( InFile, CFile::modeRead);
	CWaitCursor dummy;
	SimMem.LoadFromFile(file);
	
}




void CBufferedDiffusionView::OnStopSlowalmostpausesimulation() 
{
	// TODO: Add your command handler code here

	CChildFrame * pf = GetChildFrame();
	if (!pf->IsSimulating){
		return;
	}
	pf->StatusSimThread = 3;
	UseTimerForEnergySave = false;
	
}

void CBufferedDiffusionView::OnStopRestart() 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (!pf->IsSimulating){
		return;
	}
	pf->StatusSimThread = 4;
	UseTimerForEnergySave = false;
	
}

void CBufferedDiffusionView::OnStopSaveenergy() 
{
	// TODO: Add your command handler code here
	CChildFrame * pf = GetChildFrame();
	if (!pf->IsSimulating){
		return;
	}
	UseTimerForEnergySave = true;
	
}

void  CBufferedDiffusionView::MessageEnergySave(){
	CChildFrame * pf = GetChildFrame();
	if (!pf->IsSimulating){
		return;
	}
	if (!UseTimerForEnergySave){
		return;
	}
//	Beep(1000,10);

	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;

	SecondCount++;
	if (SecondCount>pset.NSecondsOut){
		SecondCount = 0;
	}
	if (SecondCount){
		pf->StatusSimThread = 3;		// very slow for all values SecondCount>0
	}
	else{
		pf->StatusSimThread = 4;		// normal for SecondCound==0
	}



}


void CBufferedDiffusionView::OnStopChangenoofsecondsout() 
{
	// TODO: Add your command handler code here
	CBufferedDiffusionDoc* pDoc = GetDocument();

	Params & pset = pDoc->p;
	int no = pset.NSecondsOut;
	io myio;
	if (myio.InInt(no, "Number of seconds out")!=IDOK) return;
	if (no<0) no=0;
	pset.NSecondsOut = no;

}

void CBufferedDiffusionView::OnGoContinueseriessimulation() 
{
	// TODO: Add your command handler code here
	DoGoSeriesofsimulations(true);
	
}

void CBufferedDiffusionView::OnFileReaddrdttable() 
{
	// TODO: Add your command handler code here
	CString InFile;
	CFileDialog
		diag( TRUE, NULL, InFile, OFN_FILEMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return ;
	}
	InFile = diag.GetPathName();
	CFile file ( InFile, CFile::modeRead);
	CWaitCursor dummy;
	this->ss.LoadFromFile(file);

}


void CBufferedDiffusionView::OnFileWritedrdttable() 
{
	// TODO: Add your command handler code here
	CString OutFile;
	CFileDialog
		diag( FALSE, NULL, OutFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

	if (diag.DoModal()!=IDOK){
		return ;
	}
	OutFile = diag.GetPathName();
	CFile file ( OutFile, CFile::modeCreate | CFile::modeWrite);
	CWaitCursor dummy;
	ss.WriteOnFile(file);
}
