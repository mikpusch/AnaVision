// AnaVisionView.h : interface of the CAnaVisionView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANAVISIONVIEW_H__53DF4A51_594B_4DEC_B641_72C1B762B104__INCLUDED_)
#define AFX_ANAVISIONVIEW_H__53DF4A51_594B_4DEC_B641_72C1B762B104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NKINETICPENS 7
#define NCHANNELSKINETICPENS 4

//class CChildFrame;

class CAnaVisionView : public CView
{
protected: // create from serialization only
	CAnaVisionView();
	DECLARE_DYNCREATE(CAnaVisionView)

//	vector < vector < bool>> WhiteMask;
	// Attributes
public:

	bool SaveFlimFret(CString Name);
	bool DoSaveFlimFret(CFile & file, unsigned __int32 length);
//	bool LoadFlimFret(CString Name, unsigned __int64 BytesAlreadyReadFromFile);
	bool LoadFlimFret(CFile & file);
	BOOL SeriesSelected[MAX_SERIES_SELECTED];
	BOOL PolygonSelected[MAX_POLYGON_SELECTED];
	
	int BackgroundPolygon;
	BOOL ConcatenateImages;

	Traces * GetTraces();

	void CopyFramePhasorToClipboard();

	bool FirstMean;

	PhasorImage phasor;
	double SMin, SMax, GMin, GMax;
//	double PhasorCircleG[3], PhasorCircleS[3], PhasorCircleR[3];
	Phasor PhasorCircle[4]; //0: pink, 1: blue, 2: background; 3: fret efficieny
	double PhasorCircleR[4];
	double LifeTime[2];
	bool ShowPinkCircle, ShowBlueCircle;
	bool UseLogScalePhasor;
	bool PhasorBackGroundDefined;
	double PhasorFraction;
	double PhasorPlotFretEfficiency;

	double GetFretEfficiency();
	void CalcNewEfficiencyAfterMouseMove(CPoint p);
	//double CutOffPhasorG, CutOffPhasorS, CutOffPhasorR;
	int PhasorSizeX, PhasorSizeY;
	vector < vector < int > > PhasorN;
	int CalcPhasorN();
	bool GetCoordinatesAndNumberList(vector <Phasor> & coords, vector<int> & N); 
	int OffsetXPhasor, OffsetYPhasor, NSizeGPhasor, NSizeSPhasor;
	int bottomPhasor, NPixelGPhasor, NPixelSPhasor, fG, fS;
	double ScaleXPhasor, ScaleYPhasor;
	int PhasorNMin;
	bool PhasorIsTransposed;
	void GetPixelFromPhasor(Phasor P, LONG & px, LONG & py);
	CPoint GetPixelFromPhasor(Phasor P);
	Phasor GetPhasorFromPixel(int x, int y); // here using NPixels
	Phasor GetPhasorFromCPoint(CPoint p); // here using size
	void GetPixelFromCPoint(CPoint p, int &x, int & y);
	void InitPhasorMask();

	void GetEllipseCenterAndRadius(int CircleIndex /* 0 or 1 */, CPoint & center, CPoint & radius);
	double GetDistanceFromPhasorEllipse(int CircleIndex /* 0 or 1 */, CPoint p);
	void DrawPhasorCircles(CDC * pDC = NULL);
	bool PointIsInPhasorCircle(int & CircleIndex, CPoint p, bool & OnBorder);
	bool OnMovingPhasorCircle;
	bool OnSizingPhasorCircle;
	int MovingCircleIndex;
	CPoint PhasorRButtonDownPoint;
	CPoint MousePhasorPoint;
	void OnLButtonUpPhasor(UINT nFlags, CPoint point);
	void OnMouseMovePhasor(UINT nFlags, CPoint point);
	void SetSliders();

	// coordinates are in terms of 

	// AnalyisMode: 0 regular mean
	//				1 membrane value
	//				2 inside cell, excluding membrane (assumed to be cicular!

	//	CalcType: 0: Mean, 1 : Sum
	void AnalyzeROISOfSeries(bool & first, int indexseries, int indexs,
	int AnalysisMode = 0, int CalcType = 0 );

	void AnalyzeGradientROISOfSeries(bool & first, int indexseries, int indexs,
	int AnalysisMode = 0, int CalcType = 0, bool circularROI = true );

	CPen * KineticPens;
	CPen RoiPens[NCHANNELSKINETICPENS][NKINETICPENS];
	CPen greenPen;
	CPen pinkPen;
	CPen grayPen[100];
	CPen yellowPen;
	CBrush grayBrush;
	CBrush yellowBrush;

	CChildFrame * pcf;

	__int16 LastAssignedValue;

	int ViewType; // 0 = this image
					// 1 ; deltah
	CAnaVisionDoc* GetDocument();
//	CChildFrame * GetChildFrame();

	CMainFrame * frame;
	CAnaVisionDoc * pDoc;

	SimFitThread * pSimFitThread;

//	Kinetics k;

	double scalexk, scaleyk, minxk, maxxk, minyk, maxyk;
	int offsetxk, offsetyk;
	int Scalexk(double x);
	int Scaleyk(double y);


	CPen redPen, bluePen, blackPen;

	double ThisMean;
	bool LeftButtonDown;
	int CursorToMove;
	bool CursorClicked(int curs, CPoint p);
	void DrawCurs(CDC * pDC, CPen & pen, int x);

	bool firsttime;

	int NData;
	int SizeArrays;

	double * Mean;
	int * NValues;
	double * MeanInitialpH;
	double * RefTrace;
	double MeanInArea;

	double Max, Min;

	int NMeanImages;
	double * MeanImages;
	int * IndexMeanImages;

	MyEdit ResultsEdit;
	CRect EditRect;
	
	double MaxChangeProton;
	double MinChangeProton;
	double scalextime;
	double scaleydeltah;

	void CalcTimeScales();
	int ScalexTime(int x);
	int ScaleyDeltaH(double y);

	int offx;
	int bottom;
	double scalex;
	double scaley;
	int PhasorLineThickness;

	int Scalex(int x);
	int Scaley(double y);

	void CreateButtons();
	void MoveButtons(CDC * pDC, double Min, double Max, bool autoscale);
	void MovePhasorButtons(CDC * pDC);

	CBrush PinkBrush, BlueBrush;
	Text * MinScaleField;
	Text * MaxScaleField;

	Text * LifetTimeTextField;
	Text * LifeTimeField;
	Text * PhasorRTextField;
	Text * PhasorRField;
	Text * PhasorGTextField;
	Text * PhasorGField;
	Text * PhasorSTextField;
	Text * PhasorSField;

	Text * LifetTime2TextField;
	Text * LifeTime2Field;
	Text * PhasorR2TextField;
	Text * PhasorR2Field;
	Text * PhasorG2TextField;
	Text * PhasorG2Field;
	Text * PhasorS2TextField;
	Text * PhasorS2Field;

	Text * PhasorNMinTextField;
	Text * PhasorNMinField;

	Text * FretEfficienyTextField;
	Text * FretEfficienyField;

	Text * PhasorFractionTextField;
	Text * PhasorFractionField;

	Text * PhasorPlotFretEfficiencyTextField;
	Text * PhasorPlotFretEfficiencyField;

	Text * PhasorPlotLineThicknessField;
	Text * PhasorPlotLineThicknessTextField;

	Button * ButtonUsePhasorMask;
	Button * ButtonUsePhasorMask2;
	Button * ButtonUsePhasorLogScale;
	Button * ButtonTransposeRead;

	CButton * CopyPinkButton;
	CButton * PastePinkButton;
	CButton * CopyBlueButton;
	CButton * PasteBlueButton;

	CButton * FitPinkButton;
	CButton * FitBlueButton;
	CButton * FitFracButton;

	CSliderCtrl PhasorFractionSlider;
	void OnPhasorFractionSliderScrolled(UINT nSBCode, UINT nPos);

	CSliderCtrl FretEfficiencySlider;
	void OnFretEfficiencySliderScrolled(UINT nSBCode, UINT nPos);

	int ShowMode; // 0 = graph, 1 = text
	void DrawGradient(CDC * pDC, double * Mean, CPen * ppen);
	void DrawGradientText(CDC * pDC, double * Mean);
	void DrawMeanValues(CDC * pDC);
	void DrawMeanValuesText(CDC * pDC);
	void DrawMeanROISCurves(CDC * pDC);

	// CalcType 0: Mean
	//			1: Sum
	void CalcMeanOrSumValues(int CalcType);

//	int InitialFrame0, InitialFrame1, FrameToAnalyze0, FrameToAnalyze1;

	double * time; // in seconds
	double * deltaH; // in moles
	int NTimeData;
	int SizeTimeArrays;

	void AnalyzeImage(double & CursorMean);
	void DrawDeltaH(CDC * pDC);
	void DrawDeltaHText(CDC * pDC);
	void DrawKinetics(CDC * pDC, bool ShowAsFunctionOfTime, double SD);

	void CheckFrames();

	void AnalyzeMeans();

	void OnModifydataAssignavaluetothisimage() ;

	void OnRegionofinterestCalcmeanvalues();
	void OnRegionofinterestCalculatemembrane();

	void OnRButtonDownROIS(UINT nFlags, CPoint point);

//	CToolTipCtrl * pTool ;

// Operations
public:

	double SD;
	double ThresholdManders[2];

	void MyInvalidate();

	void CheckViewMenu();

	void ConvertPhasorCircleToString(CString & c, int index);
	void ConvertE_FRac_Eph_ToString(CString & c);

	void OnLocalizationCalccoefficients(int CalcType); //0: Peason, 1: Manders 1; 2* Manders2
	void OnAnalyzeOverlaygradientseveralframes(Traces * pT, bool exportimages, bool exportgradients ) ;
	void OnAnalyzeShowmeanvaluesintable();
	void OnViewShowtext();
	void OnViewShowgraph();
	void OnViewViewkinetics();
	void OnViewViewkineticsasfunctionofradius();
	void OnViewViewfilledarea();
	void OnViewViewphasors();
	void OnPhasorLoadphasortxtfiles();
	void OnPhasorLoadphasortiffiles();
	void OnAnalyzeFret(int selectdpolygon); // polygon used as background

	void OnViewViewgradientthisimage();
	void OnViewVienumberofprotons();
	void OnViewViewmeanvalues() ;
	void OnViewViewmeanvaluesinpolygons();
	void OnFileExportgradients();
	void OnGenerateMacro();
	void OnGenerateMacronormalizeimages();
	void OnKineticsConstruct() ;
	void OnKineticsFit();
	void OnKineticsFitsimulated();
	void OnKineticsFitsimulatedOld();
	void OnKineticsExport(bool ShowAsFunctionOfTime) ;

	void OnGenerateNewmacros();
	void OnGenerateNewmacrosZmSuc();

	void OnUpdateViewMessageFromSimFit();

	void OnAnalyzeGetnumberofprotons();
	void DoAlloc();
	void DeAlloc();

	void AllocAnalyisArrays(int ND);

	void DoAllocTimeArrays();
	void DeAllocTimeArrays();
	void AllocTimeAnalyisArrays(int ND);


	void DrawMeanROIS(CDC * pDC);
	void DrawMeanROISText(CDC * pDC);

	void DrawFillArea(CDC * pDC);
	void DrawFillAreaText(CDC * pDC);
	void DrawFillAreaCurves(CDC * pDC);

	void DrawPhasors(CDC * pDC);

	void OnAnalyzeThisimage();
	void Init();
	void CalcScales(bool autoscale, double * Mean);
	void AnalyzeProtonAccumulation();
	void OldAnalyzeProtonAccumulation();
	void ExportGradient(CFile & f, double * Array);
	void OnKineticsStopfit();

	void OnRegionofinterestCalcsumvalues();
	void OnRegionofinterestCalcarea();

	void OnRegionofinterestCalculateCircularGradient(bool CircularROI = true);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnaVisionView)
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
	virtual ~CAnaVisionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAnaVisionView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnToneTest();
	//}}AFX_MSG
	afx_msg	void OnMinScaleChanged();
	afx_msg	void OnMaxScaleChanged();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);


	DECLARE_MESSAGE_MAP()
public:
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonDownPhasor(UINT nFlags, CPoint point);

	afx_msg void UsePhasorMaskClicked();
	afx_msg void UsePhasorMask2Clicked();
	
	afx_msg void UsePhasorLogScaleClicked();

	afx_msg void OnPhasorRChanged();
	afx_msg void OnPhasorGChanged();
	afx_msg void OnPhasorSChanged();
	afx_msg void OnPhasorR2Changed();
	afx_msg void OnPhasorG2Changed();
	afx_msg void OnPhasorS2Changed();
	afx_msg void ButtonPhasorTransposedReadClicked();
	afx_msg void OnPhasorNMinRChanged();
	afx_msg void OnPhasorLineThicknessChanged();

	afx_msg void OnPopupphasorShown();
	afx_msg void OnPopupphasorShow();
	afx_msg void OnPopupphasorSetasbluecirclecenter();
	afx_msg void OnPopupphasorSetasbackgroundreference();
	afx_msg void OnPopupphasorExportg();
	afx_msg void OnPopupphasorRemovebackgroundreeference();
	afx_msg void OnPhasorFractionChanged();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPhasorFretEfficiencyChanged();
	afx_msg void OnPopupphasorSavelowerimageastiff();
	afx_msg void OnPopupphasorSaveonlybluepartofimageastif();
	afx_msg void OnPopupphasorToggleusecurrentlyselectedpolygon();

	afx_msg void CopyPinkButtonClicked();
	afx_msg void PastePinkButtonClicked();
	afx_msg void CopyBlueButtonClicked();
	afx_msg void PasteBlueButtonClicked();

	afx_msg void FitPinkButtonClicked();
	afx_msg void FitBlueButtonClicked();
	afx_msg void FitFracButtonClicked();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);


	afx_msg void OnPopupphasorCopypinkcirclecoordinates();
	afx_msg void OnPopupphasorPastepinkcirclecoordinates();
	afx_msg void OnPopupphasorSavethisastiff();
	afx_msg void OnPopupphasorShow32913();
	afx_msg void OnPopupphasorShow32914();
	afx_msg void OnPopupphasorCopypinkvaluestoclipboard();
	afx_msg void OnPopupphasorCopybluevaluestoclipboard();
	afx_msg void OnPopupphasorCopyall();
	afx_msg void OnPopupphasorPasteall();
};

#ifndef _DEBUG  // debug version in AnaVisionView.cpp
inline CAnaVisionDoc* CAnaVisionView::GetDocument()
   { return (CAnaVisionDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANAVISIONVIEW_H__53DF4A51_594B_4DEC_B641_72C1B762B104__INCLUDED_)
