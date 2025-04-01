// BufferedDiffusionView.h : interface of the CBufferedDiffusionView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFEREDDIFFUSIONVIEW_H__1AC390F9_335E_4008_B049_E006820487CD__INCLUDED_)
#define AFX_BUFFEREDDIFFUSIONVIEW_H__1AC390F9_335E_4008_B049_E006820487CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SimThread;
class GoThread;

class CBufferedDiffusionView : public CView
{
protected: // create from serialization only
	CBufferedDiffusionView();
	DECLARE_DYNCREATE(CBufferedDiffusionView)

// Attributes
public:

	void MessageEnergySave();
	bool UseTimerForEnergySave;
	bool firsttime;
	int SecondCount;
	
	static UINT TI;

	int KindOfThread;				// 0 simThread, 1 go thread
	CBufferedDiffusionDoc* GetDocument();
	double scalexi, scalext;

	SeriesSimulation ss;
	double TSimulation;
	double TIntermediateSimulation;
	int NCurvesSimulation;
	int IndexMinParams;
	bool UseTwoTimePoints;
	int RefIndex;
	BOOL ShowRef;

	int NSlice, Nt;

	double scaleyH, scaleyB;

	int offxi, offxt;

	int offyH, offyBfree, offyBtotal;

	CPoint * Hi, * Bfreei, * Btotali, * Ht, * Bfreet, * Btotalt;

	int iter;

	int RedrawIndex;
	int RedrawColor;

	double LastMean;

	CRect rect;

	Params SaveSet;
	Diffusion SaveD;


	int sx, sy;

	int fileno;
	double t;
	int nprot;
	int idraw;

	CPen * 	SolidBlack;
	CPen * WhitePen;
	CPen * BluePen;
	CPen * RedPen;
	CPen * GrayPen;
	CPen ** Colors;


	double Hbulk ;

	SimulationMemory SimMem;
	bool MemorizeSimulation;
	void AllocReDraw(int index);
	void ReDrawCurves(CDC * pDC, int ni);
	SimThread * pSimThread ;
	CChildFrame * GetChildFrame();
	void SimulationFinished();
	void DoRedraw();
	void MessageRedraw();
	void DrawCircle(CDC * pDC);

	void DoReadStartingFile();
	void DoGoSeriesofsimulations(bool restart) ;


	GoThread * pGoThread;

// Operations
public:
	void DeAllocSpace();
	void DeAllocTime();
	void Alloc();
	void SetupDiffusion();

	void DrawCurves(CDC * pDC, int ni, int nt, BOOL gray, BOOL start);
	BOOL MakeFileName(int fileno, CString & filename);

	static void CALLBACK EXPORT SaveEnergyTimer(
							HWND hWnd,      // handle of CWnd that called SetTimer
							UINT nMsg,      // WM_TIMER
							UINT nIDEvent,   // timer identification
							DWORD dwTime    // system time
							);

// Operations
public:

	void OnDoGoGo(int mode) ;

	void OnGoGoOld();

	void DoTest();

	void OnDoGoGoOld(int mode);

	void MessageDrawCurves(int index);
	void MessageGoFinished();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBufferedDiffusionView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBufferedDiffusionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBufferedDiffusionView)
	afx_msg void OnTestTest();
	afx_msg void OnEditParameters();
	afx_msg void OnGoGo();
	afx_msg void OnGoLinearized();
	afx_msg void OnGoClearwindiow();
	afx_msg void OnGoSecondlinearized();
	afx_msg void OnGoRedrawoldsimulation();
	afx_msg void OnGoSeriesofsimulations();
	afx_msg void OnStopStopseriessimulation();
	afx_msg void OnFileSavesimulationseriesonfile();
	afx_msg void OnFileLoadsimulationseriesfromfile();
	afx_msg void OnStopSlowalmostpausesimulation();
	afx_msg void OnStopRestart();
	afx_msg void OnStopSaveenergy();
	afx_msg void OnStopChangenoofsecondsout();
	afx_msg void OnGoContinueseriessimulation();
	afx_msg void OnFileReaddrdttable();
	afx_msg void OnFileWritedrdttable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BufferedDiffusionView.cpp
inline CBufferedDiffusionDoc* CBufferedDiffusionView::GetDocument()
   { return (CBufferedDiffusionDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUFFEREDDIFFUSIONVIEW_H__1AC390F9_335E_4008_B049_E006820487CD__INCLUDED_)
