// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__F91D4C95_D28E_4234_86D6_D9207692545D__INCLUDED_)
#define AFX_CHILDFRM_H__F91D4C95_D28E_4234_86D6_D9207692545D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "Tree.h"
//#include "Traces.h"
//#include "AnaVisionView.h"
class Tree;
class Traces;
class CAnaVisionView;

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
//protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;
public:
	

	BOOL IsSimFitting;
	__int32 StatusSimFitThread;


	int AreaSelected;

	bool DoOpenTif(CString Name);
	bool DoOpenStandardTif(CString FileName);
	bool DoOpenAnvisionFormat(CString FileName);
// Operations
public:


	Tree * GetTree();
	Traces * GetTraces();
	CAnaVisionView * GetCAnaVisionView();

	LRESULT OnUpdateViewMessageFromSimFit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions

protected:
public:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnToneA();
	afx_msg void OnToneB();
	afx_msg void OnToneC();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnViewSetmodemove();
	afx_msg void OnViewSetmodesize();
	afx_msg void OnViewSetmoderotate();
	afx_msg void OnViewShowrectangle();
	afx_msg void OnViewShowconus();
	afx_msg void OnGenerateInit();
	afx_msg void OnGenerateNext();
	afx_msg void OnAnalyzeThisimage();
	afx_msg void OnFileLoadparams();
	afx_msg void OnFileSaveparams();
	afx_msg void OnEditParams();
	afx_msg void OnAutoScale();
	afx_msg void OnManScale();
	afx_msg void OnAnalyzeGetnumberofprotons();
	afx_msg void OnAnalyzeOverlaygradientseveralframes();
	afx_msg void OnViewViewgradientthisimage();
	afx_msg void OnViewVienumberofprotons();
	afx_msg void OnViewShowgraph();
	afx_msg void OnViewShowtext();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEditCopy();
	afx_msg void OnFileExportgradients();
	afx_msg void OnGenerateMacro();
	afx_msg void OnGenerateMacronormalizeimages();
	afx_msg void OnAnalyzeShowmeanvaluesintable();
	afx_msg void OnViewViewmeanvalues();
	afx_msg void OnModifydataSubtract();
	afx_msg void OnKineticsConstruct();
	afx_msg void OnViewViewkinetics();
	afx_msg void OnViewViewkineticsasfunctionofradius();
	afx_msg void OnKineticsFit();
	afx_msg void OnKineticsFitsimulated();
	afx_msg void OnKineticsStopfit();
	afx_msg void OnClose();
	afx_msg void OnModifydataAssignavaluetothisimage();
	afx_msg void OnModifydataRatio();
	afx_msg void OnAnalyzeExportimages();
	afx_msg void OnAnalyzeExportgradients();
	afx_msg void OnGenerateNewmacros();
	afx_msg void OnKineticsExportasafunctionofradius();
	afx_msg void OnKineticsExportasafunctionoftime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewShowpolygons();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnCreateapolygon();
public:
	afx_msg void OnRegionofinterestDeleteselectedpolygon();
public:
	afx_msg void OnViewViewmeanvaluesinpolygons();
public:
	afx_msg void OnRegionofinterestCalcmeanvalues();
public:
	afx_msg void OnHelpTest();
public:
	afx_msg void OnFileOpentiff();
public:
	afx_msg void OnOffsetOffsetx();
public:
	afx_msg void OnOffsetOffsetx32832();
public:
	afx_msg void OnOffsetOffsety();
public:
	afx_msg void OnOffsetOffsety32834();
public:
	afx_msg void OnOffsetClearalloffsets();
public:
	afx_msg void OnOffsetTrack();
public:
	afx_msg void OnOffsetTrackall();
public:
	afx_msg void OnHelpTestedge();
public:
	afx_msg void OnRegionofinterestDeleteallplygons();
public:
	afx_msg void OnHelpTestvectref();
	afx_msg void OnFileOpentiffz();
	afx_msg void OnRegionofinterestCalculatemembrane();
	afx_msg void OnRegionofinterestCalcsumvalues();
	afx_msg void OnRegionofinterestCalculatemembranesums();
	afx_msg void OnRegionofinterestMakeopposingclosedroisformalinearroi();
	afx_msg void OnRegionofinterestCalculategradientforalinealpoly();
	afx_msg void OnRegionofinterestCopyselectedpolygon32850();
	afx_msg void OnRegionofinterestPastecopiedpolygon();
	afx_msg void OnRegionofinterestCalcarea();
	afx_msg void OnHelpFill();
	afx_msg void OnHelpStartfill();
	afx_msg void OnFillDefinethreshold();
	afx_msg void OnFillStartselectpoints();
	afx_msg void OnFillStopselectpoints();
	afx_msg void OnFillClear();
	afx_msg void OnFillUndolast();
	afx_msg void OnFillTogglehide();
	afx_msg void OnFillSaveonfile();
	afx_msg void OnFillLoadfromfile();
	afx_msg void OnFillFillcurrentlyselectedpolygon();
	afx_msg void OnViewViewfilledarea();
	afx_msg void OnFileOpenstandardtiff();
	afx_msg void OnRegionofinterestSelectpolygonbyindex();
	afx_msg void OnLineDefinenewline();
	afx_msg void OnLineDeleteselectedline();
	afx_msg void OnLineDeletealllines();
	afx_msg void OnLineSelectlinebyindex();
	afx_msg void OnLineMakepolygonaroundselectedline();
	afx_msg void OnLineCopyselectedline();
	afx_msg void OnLinePastecopiedline();
	afx_msg void OnRegionofinterestMoveselectedpolywitharrowkeys();
	afx_msg void OnLineMoveselectedpolywitharrowkeys();
	afx_msg void OnFileSaveimagesinanavisionformat();
	afx_msg void OnFileOpenanavisionformat();
	afx_msg void OnAnalyzeFret();
	afx_msg void OnRegionofinterestConverttoline();
	afx_msg void OnLineConvertselectedlinetopolygon();
	afx_msg void OnLineCalclength();
	afx_msg void OnRegionofinterestCalccircumferenceofselectedpolygon();
	afx_msg void OnFillStartselectpointsinselectedpolygon();
	afx_msg void OnCountStart();
	afx_msg void OnFillStopselectpointsinselectedpolygon();
	afx_msg void OnCountStop();
	afx_msg void OnCountClear();
	afx_msg void OnCountSetsize();
	afx_msg void OnRegionofinterestCreatenpolygonsinside();
	afx_msg void OnFileSavethisstackinanavisionformat();
	afx_msg void OnTrackcellsDetectcells();
	afx_msg void OnTrackcellsCountcells();
	afx_msg void OnTrackcellsStarttrackingcellsmode();
	afx_msg void OnTrackcellsStoptrackingcellsmoded();
	afx_msg void OnViewViewphasors();
	afx_msg void OnPhasorLoadphasortxtfiles();
	afx_msg void OnPhasorLoadphasortiffiles();
	afx_msg void OnRegionofinterestCopyallpolygons();
	afx_msg void OnFileSaveflimfretpoject();
	afx_msg void OnFileLoadflimfretproject();
	afx_msg void OnLocalizationCalccoefficients();
	afx_msg void OnLocalizationCalcmanders1();
	afx_msg void OnLocalizationCalcmanders2();
	afx_msg void OnCountStartvesiclecount();
	afx_msg void OnCountSetrimsize();
	afx_msg void OnCountWritevesiclestoclipboard();
	afx_msg void OnCountReadvesiclesfromclipboard();
	afx_msg void OnCountGetvesiclesinpoly();
	afx_msg void OnCountSetcutoffsd();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__F91D4C95_D28E_4234_86D6_D9207692545D__INCLUDED_)
