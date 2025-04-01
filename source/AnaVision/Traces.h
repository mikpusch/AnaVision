#if !defined(AFX_TRACES_H__683D6AA9_6679_4345_80FB_09E8F97A8ECC__INCLUDED_)
#define AFX_TRACES_H__683D6AA9_6679_4345_80FB_09E8F97A8ECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Traces.h : header file
//

#include "../common/DrawObject.h"
#include "../common/ConnectedComponent.h"
#include "ZoomButton.h"

/////////////////////////////////////////////////////////////////////////////
// Traces view
class CMainFrame;
class CChildFrame;
class CAnaVisionDoc;
//class ConnectedComponent;


class Traces : public CScrollView
{
protected:
	Traces();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(Traces)

// Attributes
public:

//	COLORREF MyTable[256];

//	BufferClass<int> BitMapBuffer;

	void SaveCurrentImageAsTiff();
	void SaveMaskedPartOfImageAsTiff(int mask);
	ConnectedComponent comp;
	bool OnTrackingCells;
	void OnTrackcellsStarttrackingcellsmode();
	void OnRButtonTrackingCells(UINT nFlags, CPoint point);
	void OnTrackcellsStoptrackingcellsmoded();

	int * MyBitMapBuffer;
	__int64 * Hidden64Buff;
	void UpdateBitmapBuffer(int size);
	int SizeBitmapBuffer;

	float lowThreshold, highThreshold;
	UINT MaxDistPolyPoint;
	int MaxPolyPoints;
	float gaussianKernelRadius, gaussianKernelWidth ;


	CPoint OriginDC;
	void CorrectClickedPointForOrigin(CPoint & point);
	void UnCorrectClickedPointForOrigin(CPoint & point);
	void ShiftOriginDC(CDC * pDC);
	void ResetOriginDC(CDC * pDC);
	void AdjustRectOrigin(CRect & r);

	void OnLButtonControl(CPoint point); 
	void OnLButtonShift(CPoint point);
	void OnCountStart();
	bool ClickingIsTracked(bool ShowWarning = false);
	bool IsCounting;
	vector<CPoint> Counts;
	void AddPointToCount(CPoint NewPoint);
	void OnCountStop();
	void OnCountClear();
	void OnCountSetsize();
	void OnRegionofinterestCreatenpolygonsinside();
	void OnRButtonDownCounting(UINT nFlags, CPoint point);
	void OnTrackcellsDetectcells();
	void CountCells( );
//	void NewOnTrackcellsDetectcells();
//	void ExploreForCells(int x, int y, ImageDataType ** im, int xsize, int ysize, PointsAssignedClass & PointsAssigned, CellsClass & cells);
//	bool CheckAllAssigned(PointsAssignedClass & PointsAssigned, int xsize, int ysize);
//	void AddPoints(int x, int y, int dx, int dy, ImageDataType ** im, int xsize, int ysize, PointsAssignedClass & PointsAssigned, CellPoints * Cell);

	NeighborsPixelsClass GetNeighbors(int x, int y, ImageDataType ** im, int xsize, int ysize, ComponentLabelsClass & Labels);
	void CheckPixel(int & StartLabel, int x, int y, ImageDataType ** im, int xsize, int ysize, ComponentLabelsClass & Labels);
	Button * ReloadButton;
	Button * ResetButton;

	Button * OffsetXPlus10Button;
	Button * OffsetXPlus1Button;
	Button * OffsetXMinus10Button;
	Button * OffsetXMinus1Button;

	Button * OffsetYPlus10Button;
	Button * OffsetYPlus1Button;
	Button * OffsetYMinus10Button;
	Button * OffsetYMinus1Button;

	Text * OffsetxField;
	Text * OffsetyField;

	Text * OriginDCxField;
	Text * OriginDCyField;
	Text * TextOriginDCxField;
	Text * TextOriginDCyField;

	Text * MinScaleField;
	Text * MinScaleTextField;
	Text * MaxScaleField;
	Text * MaxScaleTextField;

	Button * TrackThisButton;

	Button * CopyGeneralSettingsToThisFileButton;
	Button * CopyThisSettingsToGeneralButton;
	CButton AutoScaleButton;
	CSliderCtrl MinSlider;
	CSliderCtrl MaxSlider;


	bool firsttime;
	CChildFrame * child;

	CPen redPen, bluePen, blackPen, whitePen;

	OverlayClass overlay;
	BOOL Overlay;
	CMainFrame* frame;
	bool InitRectGenerator ;
	bool InitConusGenerator ;
	bool * InitGenerator;


	int Zoom; // 1, 2=0.5 ecc
	int ZoomIndex;
//	double RealZoom; // if Zoom == 0: use RealZoom
	bool ZoomIn;
	UINT ZoomTable[10];
	ZoomButton ZoomButtons[NZOOMBUTTONS];
	ZoomButton ZoomButtonsReal[NZOOMBUTTONS];

	int SelectMode; // 0 = move
					// 1 = size
					// 2 = rotate

	int indexselected;

	bool MouseIsDown;

	bool OnShiftingOrigin;
	CPoint StartPointShiftingOrigin;

	bool OnMovingPolyWithArrowKey;
	bool OnMovingLineWithArrowKey;
	bool OnCreatingPolygon;
	bool OnMovePolygon;
	bool OnMoveLine;
	bool OnCreatingLine;

	int pointsadded;
	int selectedpolygon;
	int selectedline;
	int cornerclicked;
	void MovePolygon(UINT nFlags, CPoint point);
	void MoveLine(UINT nFlags, CPoint point);
	PolygonObject * ActualPoly;
	void AddPointToPolygonOrLine(UINT nFlags, CPoint point, bool IsLine);
	void OnRegionofinterestDeleteselectedpolygon(bool AskDelete = true);
	void OnRbuttonDownPolygons(UINT nFlags, CPoint point);
	void OnHelpTestedge();
	void OnRegionofinterestSelectpolygonbyindex();
	void OnLineDeleteselectedline(bool AskDelete = true);
	void OnLineDeletealllines();
	void OnLineSelectlinebyindex();
	void OnLineMakepolygonaroundselectedline();
	void MakeMembranePolygons();
	void OnRegionofinterestMakeopposingclosedroisformalinearroi();
	bool OnRegionofinterestCopyselectedpolygon32850();
	bool OnRegionofinterestCopyAllPolygons();
	void OnRegionofinterestPastecopiedpolygon();
	bool OnLineCopyselectedline();
	void OnLinePastecopiedline();
	void OnRegionofinterestMoveselectedpolywitharrowkeys();
	void OnLineMoveselectedpolywitharrowkeys();
	void MoveSelectedPolyOrLine(int dx, int dy);

	void OnLineDefinenewline();
	void OnRegionofinterestConverttoline();
	void OnLineConvertselectedlinetopolygon();
	void OnLineCalclength();
	void OnRegionofinterestCalccircumferenceofselectedpolygon();
	void GetMinMax(double & Min, double & Max);
//	void AddPointToLine(UINT nFlags, CPoint point);
	PolygonObject * ActualLine;

	CAnaVisionDoc* pDoc;

//	COLORREF GreyColorTable256[256];

// Operations
public:
	
	void GetOffset(int & offsetx, int & offsety);

	void DrawCell(CDC* pDC, bool Export, CFile * fp);
	void DrawCellTest(CDC* pDC, bool Export, CFile * fp);
	void DrawCellBad(CDC* pDC, bool Export, CFile * fp);

	void MyInvalidate();
	void OnLButtonDownMove(UINT nFlags, CPoint point);
	void OnLButtonDownSize( UINT nFlags, CPoint point);
	void OnLButtonDownRotate( UINT nFlags, CPoint point);

	BOOL PreTranslateMessage(MSG *pMsg);

	void OnEditUndo(); 
	void OnEditRedo(); 

	void Init();

	void OnViewSetmodemove() ;

	void OnViewSetmodesize() ;

	void OnViewSetmoderotate() ;

	void OnViewShowrectangle();

	void OnViewShowconus();
	void OnViewShowpolygons();
	void OnCreateapolygon();
	void OnFillStartselectpoints();
	void OnFillStopselectpoints();
	void OnFillClear();

	void OnGenerateInit();
	void OnGenerateNext();

	void CreateButtons();
	void MoveButtons(CDC * pDC);

	void OnOffsetClearalloffsets();
	void OnOffsetTrack();
	void DoOffsetTrack(int index);
	void OnOffsetTrackall();
	void OnRegionofinterestDeleteallplygons();

	vector<bool> Selected;
	bool SelectedInitialized;
	void InitSelected(int w, int h);
	int CurrentFillWidth, CurrentFillHeight;
	bool FillStarted;
	bool SelectingFillPoints;
	bool SelectingFillPointsInSelectedPolygon;
	
	void OnHelpStartFill();
	void OnHelpFill();
	void AddFillArea(CPoint point, bool UseOnlySelectedPolygon = false);
	void OnFillUndolast();
	void OnFillTogglehide();
	BOOL ShowFill;
	void OnFillSaveonfile();
	void OnFillLoadfromfile();
	void OnFillFillcurrentlyselectedpolygon();
	bool OnFillStartselectpointsinselectedpolygon();
	void OnFillStopselectpointsinselectedpolygon();

	void OnMinSliderScrolled(UINT nSBCode, UINT nPos);
	void OnMaxSliderScrolled(UINT nSBCode, UINT nPos);

	double GetMeanInObjectArea();
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Traces)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~Traces();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
public:
	//{{AFX_MSG(Traces)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg	void ReloadButtonClicked();
	afx_msg	void ResetButtonClicked();
	afx_msg	void CopyThisToGeneralButtonClicked();
	afx_msg	void CopyGeneralToThisButtonClicked();

	afx_msg	void OnOffsetXPlus10Clicked();
	afx_msg	void OnOffsetXPlus1Clicked();
	afx_msg	void OnOffsetXMinus10Clicked();
	afx_msg	void OnOffsetXMinus1Clicked();

	afx_msg	void OnOffsetYPlus10Clicked();
	afx_msg	void OnOffsetYPlus1Clicked();
	afx_msg	void OnOffsetYMinus10Clicked();
	afx_msg	void OnOffsetYMinus1Clicked();
	afx_msg	void OnTrackThisButtonClicked();
	afx_msg	void OnImageOffsetxChanged();
	afx_msg	void OnImageOffsetyChanged();
	afx_msg	void OnAutoscaleButtonClicked();
	afx_msg	void OnMinChanged();
	afx_msg	void OnMaxChanged();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACES_H__683D6AA9_6679_4345_80FB_09E8F97A8ECC__INCLUDED_)
