// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "AnaVision.h"
#include "ChildFrm.h"
#include "AnaVisionTif.h"
#include "../../GnuWin32/include/tiffio.h"
#include "DiagFillParams.h"

#include <vector>

//#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_COMMAND(ID_TONE_A, OnToneA)
	ON_COMMAND(ID_TONE_B, OnToneB)
	ON_COMMAND(ID_TONE_C, OnToneC)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_VIEW_SETMODEMOVE, OnViewSetmodemove)
	ON_COMMAND(ID_VIEW_SETMODESIZE, OnViewSetmodesize)
	ON_COMMAND(ID_VIEW_SETMODEROTATE, OnViewSetmoderotate)
	ON_COMMAND(ID_VIEW_SHOWRECTANGLE, OnViewShowrectangle)
	ON_COMMAND(ID_VIEW_SHOWCONUS, OnViewShowconus)
	ON_COMMAND(ID_GENERATE_INIT, OnGenerateInit)
	ON_COMMAND(ID_GENERATE_NEXT, OnGenerateNext)
	ON_COMMAND(ID_ANALYZE_THISIMAGE, OnAnalyzeThisimage)
	ON_COMMAND(ID_FILE_LOADPARAMS, OnFileLoadparams)
	ON_COMMAND(ID_FILE_SAVEPARAMS, OnFileSaveparams)
	ON_COMMAND(ID_EDIT_PARAMS, OnEditParams)
	ON_COMMAND(ID_AUTO_SCALE, OnAutoScale)
	ON_COMMAND(ID_MAN_SCALE, OnManScale)
	ON_COMMAND(ID_ANALYZE_GETNUMBEROFPROTONS, OnAnalyzeGetnumberofprotons)
	ON_COMMAND(ID_ANALYZE_OVERLAYGRADIENTSEVERALFRAMES, OnAnalyzeOverlaygradientseveralframes)
	ON_COMMAND(ID_VIEW_VIEWGRADIENTTHISIMAGE, OnViewViewgradientthisimage)
	ON_COMMAND(ID_VIEW_VIENUMBEROFPROTONS, OnViewVienumberofprotons)
	ON_COMMAND(ID_VIEW_SHOWGRAPH, OnViewShowgraph)
	ON_COMMAND(ID_VIEW_SHOWTEXT, OnViewShowtext)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_FILE_EXPORTGRADIENTS, OnFileExportgradients)
	ON_COMMAND(ID_GENERATE_MACRO, OnGenerateMacro)
	ON_COMMAND(ID_GENERATE_MACRONORMALIZEIMAGES, OnGenerateMacronormalizeimages)
	ON_COMMAND(ID_ANALYZE_SHOWMEANVALUESINTABLE, OnAnalyzeShowmeanvaluesintable)
	ON_COMMAND(ID_VIEW_VIEWMEANVALUES, OnViewViewmeanvalues)
	ON_COMMAND(ID_MODIFYDATA_SUBTRACT, OnModifydataSubtract)
	ON_COMMAND(ID_KINETICS_CONSTRUCT, OnKineticsConstruct)
	ON_COMMAND(ID_VIEW_VIEWKINETICS, OnViewViewkinetics)
	ON_COMMAND(ID_VIEW_VIEWKINETICSASFUNCTIONOFRADIUS, OnViewViewkineticsasfunctionofradius)
	ON_COMMAND(ID_KINETICS_FIT, OnKineticsFit)
	ON_COMMAND(ID_KINETICS_FITSIMULATED, OnKineticsFitsimulated)
	ON_COMMAND(ID_KINETICS_STOPFIT, OnKineticsStopfit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MODIFYDATA_ASSIGNAVALUETOTHISIMAGE, OnModifydataAssignavaluetothisimage)
	ON_COMMAND(ID_MODIFYDATA_RATIO, OnModifydataRatio)
	ON_COMMAND(ID_ANALYZE_EXPORTIMAGES, OnAnalyzeExportimages)
	ON_COMMAND(ID_ANALYZE_EXPORTGRADIENTS, OnAnalyzeExportgradients)
	ON_COMMAND(ID_GENERATE_NEWMACROS, OnGenerateNewmacros)
	ON_COMMAND(ID_KINETICS_EXPORTASAFUNCTIONOFRADIUS, OnKineticsExportasafunctionofradius)
	ON_COMMAND(ID_KINETICS_EXPORTASAFUNCTIONOFTIME, OnKineticsExportasafunctionoftime)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_SHOWPOLYGONS, &CChildFrame::OnViewShowpolygons)
	ON_COMMAND(ID_CREATEAPOLYGON, &CChildFrame::OnCreateapolygon)
	ON_COMMAND(ID_REGIONOFINTEREST_DELETESELECTEDPOLYGON, &CChildFrame::OnRegionofinterestDeleteselectedpolygon)
	ON_COMMAND(ID_VIEW_VIEWMEANVALUESINPOLYGONS, &CChildFrame::OnViewViewmeanvaluesinpolygons)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCMEANVALUES, &CChildFrame::OnRegionofinterestCalcmeanvalues)
	ON_COMMAND(ID_HELP_TEST, &CChildFrame::OnHelpTest)
	ON_COMMAND(ID_FILE_OPENTIFF, &CChildFrame::OnFileOpentiff)
	ON_COMMAND(ID_OFFSET_OFFSETX, &CChildFrame::OnOffsetOffsetx)
	ON_COMMAND(ID_OFFSET_OFFSETX32832, &CChildFrame::OnOffsetOffsetx32832)
	ON_COMMAND(ID_OFFSET_OFFSETY, &CChildFrame::OnOffsetOffsety)
	ON_COMMAND(ID_OFFSET_OFFSETY32834, &CChildFrame::OnOffsetOffsety32834)
	ON_COMMAND(ID_OFFSET_CLEARALLOFFSETS, &CChildFrame::OnOffsetClearalloffsets)
	ON_COMMAND(ID_OFFSET_TRACK, &CChildFrame::OnOffsetTrack)
	ON_COMMAND(ID_OFFSET_TRACKALL, &CChildFrame::OnOffsetTrackall)
	ON_COMMAND(ID_HELP_TESTEDGE, &CChildFrame::OnHelpTestedge)
	ON_COMMAND(ID_REGIONOFINTEREST_DELETEALLPLYGONS, &CChildFrame::OnRegionofinterestDeleteallplygons)
	ON_COMMAND(ID_HELP_TESTVECTREF, &CChildFrame::OnHelpTestvectref)
	ON_COMMAND(ID_FILE_OPENTIFFZ, &CChildFrame::OnFileOpentiffz)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCULATEMEMBRANE, &CChildFrame::OnRegionofinterestCalculatemembrane)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCSUMVALUES, &CChildFrame::OnRegionofinterestCalcsumvalues)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCULATEMEMBRANESUMS, &CChildFrame::OnRegionofinterestCalculatemembranesums)
	ON_COMMAND(ID_REGIONOFINTEREST_MAKEOPPOSINGCLOSEDROISFORMALINEARROI, &CChildFrame::OnRegionofinterestMakeopposingclosedroisformalinearroi)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCULATEGRADIENTFORALINEALPOLY, &CChildFrame::OnRegionofinterestCalculategradientforalinealpoly)
	ON_COMMAND(ID_REGIONOFINTEREST_COPYSELECTEDPOLYGON32850, &CChildFrame::OnRegionofinterestCopyselectedpolygon32850)
	ON_COMMAND(ID_REGIONOFINTEREST_PASTECOPIEDPOLYGON, &CChildFrame::OnRegionofinterestPastecopiedpolygon)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCAREA, &CChildFrame::OnRegionofinterestCalcarea)
	ON_COMMAND(ID_HELP_FILL, &CChildFrame::OnHelpFill)
	ON_COMMAND(ID_HELP_STARTFILL, &CChildFrame::OnHelpStartfill)
	ON_COMMAND(ID_FILL_DEFINETHRESHOLD, &CChildFrame::OnFillDefinethreshold)
	ON_COMMAND(ID_FILL_STARTSELECTPOINTS, &CChildFrame::OnFillStartselectpoints)
	ON_COMMAND(ID_FILL_STOPSELECTPOINTS, &CChildFrame::OnFillStopselectpoints)
	ON_COMMAND(ID_FILL_CLEAR, &CChildFrame::OnFillClear)
	ON_COMMAND(ID_FILL_UNDOLAST, &CChildFrame::OnFillUndolast)
	ON_COMMAND(ID_FILL_TOGGLEHIDE, &CChildFrame::OnFillTogglehide)
	ON_COMMAND(ID_FILL_SAVEONFILE, &CChildFrame::OnFillSaveonfile)
	ON_COMMAND(ID_FILL_LOADFROMFILE, &CChildFrame::OnFillLoadfromfile)
	ON_COMMAND(ID_FILL_FILLCURRENTLYSELECTEDPOLYGON, &CChildFrame::OnFillFillcurrentlyselectedpolygon)
	ON_COMMAND(ID_VIEW_VIEWFILLEDAREA, &CChildFrame::OnViewViewfilledarea)
	ON_COMMAND(ID_FILE_OPENSTANDARDTIFF, &CChildFrame::OnFileOpenstandardtiff)
	ON_COMMAND(ID_REGIONOFINTEREST_SELECTPOLYGONBYINDEX, &CChildFrame::OnRegionofinterestSelectpolygonbyindex)
	ON_COMMAND(ID_LINE_DEFINENEWLINE, &CChildFrame::OnLineDefinenewline)
	ON_COMMAND(ID_LINE_DELETESELECTEDLINE, &CChildFrame::OnLineDeleteselectedline)
	ON_COMMAND(ID_LINE_DELETEALLLINES, &CChildFrame::OnLineDeletealllines)
	ON_COMMAND(ID_LINE_SELECTLINEBYINDEX, &CChildFrame::OnLineSelectlinebyindex)
	ON_COMMAND(ID_LINE_MAKEPOLYGONAROUNDSELECTEDLINE, &CChildFrame::OnLineMakepolygonaroundselectedline)
	ON_COMMAND(ID_LINE_COPYSELECTEDLINE, &CChildFrame::OnLineCopyselectedline)
	ON_COMMAND(ID_LINE_PASTECOPIEDLINE, &CChildFrame::OnLinePastecopiedline)
	ON_COMMAND(ID_REGIONOFINTEREST_MOVESELECTEDPOLYWITHARROWKEYS, &CChildFrame::OnRegionofinterestMoveselectedpolywitharrowkeys)
	ON_COMMAND(ID_LINE_MOVESELECTEDPOLYWITHARROWKEYS, &CChildFrame::OnLineMoveselectedpolywitharrowkeys)
	ON_COMMAND(ID_FILE_SAVEIMAGESINANAVISIONFORMAT, &CChildFrame::OnFileSaveimagesinanavisionformat)
	ON_COMMAND(ID_FILE_OPENANAVISIPONFORMAT, &CChildFrame::OnFileOpenanavisionformat)
	ON_COMMAND(ID_ANALYZE_FRET, &CChildFrame::OnAnalyzeFret)
	ON_COMMAND(ID_REGIONOFINTEREST_CONVERTTOLINE, &CChildFrame::OnRegionofinterestConverttoline)
	ON_COMMAND(ID_LINE_CONVERTSELECTEDLINETOPOLYGON, &CChildFrame::OnLineConvertselectedlinetopolygon)
	ON_COMMAND(ID_LINE_CALCLENGTH, &CChildFrame::OnLineCalclength)
	ON_COMMAND(ID_REGIONOFINTEREST_CALCCIRCUMFERENCEOFSELECTEDPOLYGON, &CChildFrame::OnRegionofinterestCalccircumferenceofselectedpolygon)
	ON_COMMAND(ID_FILL_STARTSELECTPOINTSINSELECTEDPOLYGON, &CChildFrame::OnFillStartselectpointsinselectedpolygon)
	ON_COMMAND(ID_COUNT_START, &CChildFrame::OnCountStart)
	ON_COMMAND(ID_FILL_STOPSELECTPOINTSINSELECTEDPOLYGON, &CChildFrame::OnFillStopselectpointsinselectedpolygon)
	ON_COMMAND(ID_COUNT_STOP, &CChildFrame::OnCountStop)
	ON_COMMAND(ID_COUNT_CLEAR, &CChildFrame::OnCountClear)
	ON_COMMAND(ID_COUNT_SETSIZE, &CChildFrame::OnCountSetsize)
	ON_COMMAND(ID_REGIONOFINTEREST_CREATENPOLYGONSINSIDE, &CChildFrame::OnRegionofinterestCreatenpolygonsinside)
	ON_COMMAND(ID_FILE_SAVETHISSTACKINANAVISIONFORMAT, &CChildFrame::OnFileSavethisstackinanavisionformat)
	ON_COMMAND(ID_TRACKCELLS_DETECTCELLS, &CChildFrame::OnTrackcellsDetectcells)
	ON_COMMAND(ID_TRACKCELLS_COUNTCELLS, &CChildFrame::OnTrackcellsCountcells)
	ON_COMMAND(ID_TRACKCELLS_STARTTRACKINGCELLSMODE, &CChildFrame::OnTrackcellsStarttrackingcellsmode)
	ON_COMMAND(ID_TRACKCELLS_STOPTRACKINGCELLSMODED, &CChildFrame::OnTrackcellsStoptrackingcellsmoded)
	ON_COMMAND(ID_VIEW_VIEWPHASORS, &CChildFrame::OnViewViewphasors)
	ON_COMMAND(ID_PHASOR_LOADPHASORTXTFILES, &CChildFrame::OnPhasorLoadphasortxtfiles)
	ON_COMMAND(ID_PHASOR_LOADPHASORTIFFILES, &CChildFrame::OnPhasorLoadphasortiffiles)
	ON_COMMAND(ID_REGIONOFINTEREST_COPYALLPOLYGONS, &CChildFrame::OnRegionofinterestCopyallpolygons)
	ON_COMMAND(ID_FILE_SAVEFLIMFRETPOJECT, &CChildFrame::OnFileSaveflimfretpoject)
	ON_COMMAND(ID_FILE_LOADFLIMFRETPROJECT, &CChildFrame::OnFileLoadflimfretproject)
	ON_COMMAND(ID_LOCALIZATION_CALCCOEFFICIENTS, &CChildFrame::OnLocalizationCalccoefficients)
	ON_COMMAND(ID_LOCALIZATION_CALCMANDERS1, &CChildFrame::OnLocalizationCalcmanders1)
	ON_COMMAND(ID_LOCALIZATION_CALCMANDERS2, &CChildFrame::OnLocalizationCalcmanders2)
	ON_COMMAND(ID_COUNT_STARTVESICLECOUNT, &CChildFrame::OnCountStartvesiclecount)
	ON_COMMAND(ID_COUNT_SETRIMSIZE, &CChildFrame::OnCountSetrimsize)
	ON_COMMAND(ID_COUNT_WRITEVESICLESTOCLIPBOARD, &CChildFrame::OnCountWritevesiclestoclipboard)
	ON_COMMAND(ID_COUNT_READVESICLESFROMCLIPBOARD, &CChildFrame::OnCountReadvesiclesfromclipboard)
	ON_COMMAND(ID_COUNT_GETVESICLESINPOLY, &CChildFrame::OnCountGetvesiclesinpoly)
	ON_COMMAND(ID_COUNT_SETCUTOFFSD, &CChildFrame::OnCountSetcutoffsd)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	IsSimFitting = false;
//	IsSimFitting = true;
	StatusSimFitThread = 0;
	AreaSelected = 0;
}

CChildFrame::~CChildFrame()
{
}
/*
void CChildFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		Beep(1000, 100);
	if (nChar == VK_ESCAPE){
		Beep(500, 20);
	}
	CMDIChildWnd::OnKeyDown(nChar, nRepCnt, nFlags);


}
*/
BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{
//	return m_wndSplitter.Create( this,
//		2, 2,                 // TODO: adjust the number of rows, columns
//		CSize( 10, 10 ),      // TODO: adjust the minimum pane size
//		pContext );
//}



	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return false;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,

		RUNTIME_CLASS(Tree), CSize(160, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return false;
	}
	m_wndSplitter.SetColumnInfo( 0, 250, 150);//int row, int cyIdeal, int cyMin );


	// add the second splitter pane - which is a nested splitter with 2 rows
	if (!m_wndSplitter2.CreateStatic(
		&m_wndSplitter,     // our parent window is the first splitter
		2, 1,               // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
		m_wndSplitter.IdFromRowCol(0, 1)
			// new splitter is in the first row, 2nd column of first splitter
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return false;
	}

	// now create the two views inside the nested splitter
//	int cyText = max(lpcs->cy - 10, 20);    // height of text pane
	int cyText = 1000;

	if (!m_wndSplitter2.CreateView(0, 0,

		pContext->m_pNewViewClass, CSize(0, cyText), pContext))
//		pContext->m_pNewViewClass, CSize(10, 10), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return false;
	}
	if (!m_wndSplitter2.CreateView(1, 0,
//		RUNTIME_CLASS(Traces), CSize(100, 100), pContext))
//		RUNTIME_CLASS(Traces), CSize(100, 100), pContext))
		RUNTIME_CLASS(Traces), CSize(200, 100), pContext))
	{
		TRACE0("Failed to create third pane\n");
		return false;
	}

//	m_wndSplitter2.SetRowInfo( 0, 100, 30);//int row, int cyIdeal, int cyMin );
//	m_wndSplitter2.SetRowInfo( 0, 200, 30);//int row, int cyIdeal, int cyMin );
//	m_wndSplitter2.SetRowInfo( 0, 350, 190);//int row, int cyIdeal, int cyMin );
//	m_wndSplitter2.SetRowInfo( 0, 1050, 190);//int row, int cyIdeal, int cyMin );

	m_wndSplitter2.SetRowInfo( 0, 350, 1);//int row, int cyIdeal, int cyMin );

	// it all worked, we now have two splitter windows which contain
	//  three different views

	return true;
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return false;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	return true;
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Modify this function to change how the frame is activated.

	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnToneA() 
{
	// TODO: Add your command handler code here
	Beep(220, 500);
}

void CChildFrame::OnToneB() 
{
	// TODO: Add your command handler code here
	Beep(260, 500);
	
}

void CChildFrame::OnToneC() 
{
	// TODO: Add your command handler code here
	
	Beep(500, 50);
}


Tree * CChildFrame::GetTree(){
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pFirstView = pDoc->GetNextView( pos );
	if (pos==NULL){
		Alert("pos = NULL, after 1st");
	}
	Tree * pTree = (Tree *)(pFirstView);
	return pTree;
}


Traces * CChildFrame::GetTraces(){
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pFirstView = pDoc->GetNextView( pos );
	if (pos==NULL){
		Alert("pos = NULL, after 1st");
	}
	pFirstView = pDoc->GetNextView( pos );
	if (pos==NULL){
		Alert("pos = NULL, after 2nd");
	}
	pFirstView = pDoc->GetNextView( pos );

	Traces * pTraces = (Traces *)(pFirstView);

	return pTraces;
}

CAnaVisionView * CChildFrame::GetCAnaVisionView(){
	CView* pV = GetActiveView( );

	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pFirstView = pDoc->GetNextView( pos );
	pFirstView = pDoc->GetNextView( pos );
	CAnaVisionView * pCAna = (CAnaVisionView *)(pFirstView);
	return pCAna;
}



void CChildFrame::OnEditUndo() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnEditUndo();
}

void CChildFrame::OnEditRedo() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnEditRedo();
}

void CChildFrame::OnViewSetmodemove() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnViewSetmodemove();	
}

void CChildFrame::OnViewSetmodesize() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnViewSetmodesize();	
}

void CChildFrame::OnViewSetmoderotate() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnViewSetmoderotate();	
}

void CChildFrame::OnViewShowrectangle() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnViewShowrectangle();	
	
}

void CChildFrame::OnViewShowconus() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnViewShowconus();	
	
}

void CChildFrame::OnGenerateInit() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnGenerateInit();	
	
}

void CChildFrame::OnGenerateNext() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnGenerateNext();	
	
}

void CChildFrame::OnAnalyzeThisimage() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->Invalidate();
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnAnalyzeThisimage();

}

void CChildFrame::OnFileLoadparams() 
{
	// TODO: Add your command handler code here
	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	if (!pDoc) {
//		Alert0("no doc");
		return;
	}

	pDoc->dat.p.LoadParamsFile(true);
	pDoc->dat.p.RectOb.SetSize(pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage);
	pDoc->dat.p.ConusOb.SetSize(pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage);


	pDoc->UpdateAllViews(NULL);
}

void CChildFrame::OnFileSaveparams() 
{
	// TODO: Add your command handler code here
	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());

	pDoc->dat.p.SaveParamsFile();
//	pApp->p.SaveParamsFile();
	
}

void CChildFrame::OnEditParams() 
{
	// TODO: Add your command handler code here
//	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());

	ParamsDiag diag;

//	diag.p = frame->p;
	diag.p = pDoc->dat.p;

	if (diag.DoModal() != IDOK){
		return;
	}

//	frame->p = diag.p;
	pDoc->dat.p = diag.p ;

//	frame->p.RectOb.SetSize(frame->p.xsizeimage, frame->p.ysizeimage);
//	frame->p.ConusOb.SetSize(frame->p.xsizeimage, frame->p.ysizeimage);
	pDoc->dat.p.RectOb.SetSize(pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage);
	pDoc->dat.p.ConusOb.SetSize(pDoc->dat.p.xsizeimage, pDoc->dat.p.ysizeimage);


//	CView* pV = GetActiveView( );
//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	pDoc->UpdateAllViews(NULL);
	
}

void CChildFrame::OnAutoScale() 
{
	// TODO: Add your command handler code here
//	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
	CView* pV = GetActiveView( );
	if (!pV) return;
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());

//	if (frame->p.AutoScale){
	if (pDoc->dat.p.AutoScale){
		return;
	}

//	frame->p.AutoScale = true;
	pDoc->dat.p.AutoScale = true;
//	CView* pV = GetActiveView( );
//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	pDoc->UpdateAllViews(NULL);



	
}

void CChildFrame::OnManScale() 
{
	// TODO: Add your command handler code here
//	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());

//	if (!frame->p.AutoScale){
	if (!pDoc->dat.p.AutoScale){
		return;
	}

//	frame->p.AutoScale = false;
	pDoc->dat.p.AutoScale = false;
//	CView* pV = GetActiveView( );
//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	pDoc->UpdateAllViews(NULL);



}


void CChildFrame::OnAnalyzeGetnumberofprotons() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnAnalyzeGetnumberofprotons();

}

void CChildFrame::OnAnalyzeOverlaygradientseveralframes() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnAnalyzeOverlaygradientseveralframes(pT, false, false) ;
}

void CChildFrame::OnViewViewgradientthisimage() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewgradientthisimage();	
}

void CChildFrame::OnViewVienumberofprotons() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewVienumberofprotons();
}

void CChildFrame::OnViewShowgraph() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->	OnViewShowgraph();
}

void CChildFrame::OnViewShowtext() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->	OnViewShowtext();
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CView* pV = GetActiveView( );
	if (!pV) return;

	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->CheckViewMenu();
	
}

void CChildFrame::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CView* pActive = GetActiveView( ) ;
	CAnaVisionView * pV = this->GetCAnaVisionView();

	if (pV==pActive){
		if (pV->ShowMode == 1){
			int nStartChar, nEndChar;
			pV->ResultsEdit.GetSel( nStartChar, nEndChar ) ;
			if (nEndChar>nStartChar){
				pV->ResultsEdit.Copy();
				return;
			}
		}
	}
}


void CChildFrame::OnFileExportgradients() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnFileExportgradients() ;
	
}

void CChildFrame::OnGenerateMacro() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnGenerateMacro() ;	
}

void CChildFrame::OnGenerateMacronormalizeimages() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnGenerateMacronormalizeimages() ;		
}

void CChildFrame::OnAnalyzeShowmeanvaluesintable() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnAnalyzeShowmeanvaluesintable() ;		
	
}

void CChildFrame::OnViewViewmeanvalues() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewmeanvalues() ;
}

void CChildFrame::OnModifydataSubtract() 
{
	// TODO: Add your command handler code here
	Tree * tree = GetTree();
	tree->OnModifydataSubtract();
}

void CChildFrame::OnKineticsConstruct() 
{
	// TODO: Add your command handler code here
	if (IsSimFitting){
		Alert0("Currently fitting");
		return;
	}
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnKineticsConstruct()  ;

}

void CChildFrame::OnViewViewkinetics() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewkinetics();	
	
}

void CChildFrame::OnViewViewkineticsasfunctionofradius() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewkineticsasfunctionofradius();	
	
}

void CChildFrame::OnKineticsFit() 
{
	// TODO: Add your command handler code here
	if (IsSimFitting){
		Alert0("Currently fitting");
		return;
	}
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnKineticsFit();	
	
}

void CChildFrame::OnKineticsFitsimulated() 
{
	// TODO: Add your command handler code here
	if (IsSimFitting){
		Alert0("Currently fitting");
		return;
	}
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnKineticsFitsimulated();	
	
}

LRESULT CChildFrame::OnUpdateViewMessageFromSimFit(){
	CAnaVisionView * pCAna = this->GetCAnaVisionView();

	pCAna->OnUpdateViewMessageFromSimFit();

	return 0;
}

LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message){

		case UPDATEVIEW_MESSAGE	:		return OnUpdateViewMessageFromSimFit();
											break;
		default							: 	return CMDIChildWnd::WindowProc(message, wParam, lParam);
											break;
	}

}


void CChildFrame::OnKineticsStopfit() 
{
	// TODO: Add your command handler code here
	if (!IsSimFitting){
		Alert0("Not currently fitting");
		return;
	}

	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnKineticsStopfit();	
	
}

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (IsSimFitting){
		Alert0("Wait until fit is finished");
		OnKineticsStopfit();
		return;
	}
	CMDIChildWnd::OnClose();
}

void CChildFrame::OnModifydataAssignavaluetothisimage() 
{
	// TODO: Add your command handler code here
	if (IsSimFitting){
		Alert0("Currently fitting");
		return;
	}	
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnModifydataAssignavaluetothisimage() ;	
}

void CChildFrame::OnModifydataRatio() 
{
	// TODO: Add your command handler code here
	if (IsSimFitting){
		Alert0("Currently fitting");
		return;
	}	
	Tree * t = GetTree();
	t->OnModifydataRatio() ;	
}

void CChildFrame::OnAnalyzeExportimages() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnAnalyzeOverlaygradientseveralframes(pT, true, false) ;

}

void CChildFrame::OnAnalyzeExportgradients() 
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnAnalyzeOverlaygradientseveralframes(pT, false, true) ;
	
}

void CChildFrame::OnGenerateNewmacros() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnGenerateNewmacros();
}

void CChildFrame::OnKineticsExportasafunctionofradius() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnKineticsExport(false) ;
	
}

void CChildFrame::OnKineticsExportasafunctionoftime() 
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnKineticsExport(true) ;	
}

void CChildFrame::OnViewShowpolygons()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnViewShowpolygons();

}

void CChildFrame::OnCreateapolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCreateapolygon();
}

void CChildFrame::OnRegionofinterestDeleteselectedpolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestDeleteselectedpolygon();
}

void CChildFrame::OnViewViewmeanvaluesinpolygons()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewmeanvaluesinpolygons() ;
}

void CChildFrame::OnRegionofinterestCalcmeanvalues()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnRegionofinterestCalcmeanvalues() ;

}

void CChildFrame::OnHelpTest()
{
	// TODO: Add your command handler code here
	CString FileName;

   	CFileDialog
	diag( true, NULL, FileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	FileName = diag.GetPathName();

	AnaVisionTif AnaTif;

	if (!AnaTif.OpenFile(FileName)){
		Alert0("could not open");
		return;
	}



//	return;

//	TIFF* tif = TIFFOpen(FileName, "r");

//	if (!tif){
//		Alert0("cannot open tiff");
//		return;
//	}
	__int32 w, h;

	AnaTif.GetImageSize(w, h);

//	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
//	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
	ShowFloat(w, "width");
	ShowFloat(h, "height");

	ShowFloat(AnaTif.GetNumberOfImagesPerChannel(), "NImages");


//	int info;

//	TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &info);

//	ShowFloat(info, "TIFFTAG_PLANARCONFIG");

	unsigned __int32 npixels = w * h;
//	unsigned __int32 * raster = new unsigned __int16 [(uint32*) _TIFFmalloc(npixels * sizeof (uint32));
	unsigned __int16 * raster = new  unsigned __int16 [npixels];

	int channel=0;
	int time = 0;
	io myio;
	while (true){
		if (myio.InInt(channel, "channel")!= IDOK) return;
		if (myio.InInt(time, "time")!= IDOK) return;
		if (!AnaTif.ReadImage(time, channel, raster)){
			Alert0("could not read image");
			return;
		}
		unsigned __int16 MaxData = 0;
		for (int i=0; i<npixels; i++){
			if (raster[i]>MaxData) MaxData = raster[i];
		}


//	if (!TIFFReadRGBAImage(tif, w, h, raster, 0)) {
//		Alert0("Could not Read rgb raster");
		//goto EndTifTest;
//	}

		CDC * pDC = GetDC();
	//	pDC->SetGraphicsMode(GM_ADVANCED);
		pDC->MoveTo(10,10);
		pDC->LineTo(800, 400);
		CDC * mDC = new CDC;
		mDC->CreateCompatibleDC(pDC);
		if (!mDC){
			Alert("could not create compatible DC");
			return;
		}

		CRect r;
		GetClientRect(&r);

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

		CRect rect;

		GetClientRect(&rect);

		

		for (int i=0; i<h; i++){
			int ss=i*w;
			for (int j=0; j<w; j++){
//				unsigned __int32 p=raster[ss+j];
//				MyRGBStruct * ff = (MyRGBStruct *)(&(p));
				int d = int( double(raster[ss+j])/double(MaxData)*255.0);
//				unsigned __int16 p=raster[ss+j];
//				MyRGBStruct * ff = (MyRGBStruct *)(&(p));


				CPoint point;
				point.x = j;
				point.y = i;

//				mDC->SetPixel(point,RGB(ff->r, ff->g, ff->b));
				mDC->SetPixel(point,RGB(d, d, d));
			}
		}

		mDC->SelectObject(mDC->GetCurrentBitmap());
		pDC->SelectObject(pDC->GetCurrentBitmap());

		if (!pDC->BitBlt(0, 0, r.right-1, r.bottom-1, mDC, 0, 0, SRCCOPY)){
			Alert("could not retrieve");
//		return false;
		}
		else{
//			Alert0("Hi");
		}
		ReleaseDC(mDC);
		delete mDC;

		ReleaseDC(pDC);
	}

	delete [] raster;

}

bool CChildFrame::DoOpenTif(CString FileName){
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	if (!pDoc->OpenTif(FileName)){
		return false;
	}
	Tree * tree = GetTree();
	tree->DeleteTreeView();
	tree->CreateTreeView();
	tree->CreateFileNumberButtons();
//	tree->CreateButtons();

	tree->Invalidate();


	this->SetWindowTextA(FileName);
	pDoc->SetPathName(FileName);

	pV->Invalidate();

	Traces * pT = GetTraces();
	pT->Invalidate();


	return true;
	//ShowFloat(pDoc->dat.NImages, "NI");

}

void CChildFrame::OnFileOpentiff()
{
	// TODO: Add your command handler code here
	CString FileName;

//	Alert0("Hi");
   	CFileDialog
	diag( true, NULL, FileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	CWaitCursor dummy;

	FileName = diag.GetPathName();

	DoOpenTif(FileName);

	return;

	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	if (!pDoc->OpenTif(FileName)){
		return;
	}
	Tree * tree = GetTree();
	tree->DeleteTreeView();
	tree->CreateTreeView();
	tree->CreateFileNumberButtons();
//	tree->CreateButtons();

	tree->Invalidate();


	this->SetWindowTextA(FileName);


	//ShowFloat(pDoc->dat.NImages, "NI");

}

void CChildFrame::OnOffsetOffsetx()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetXPlus10Clicked();
}

void CChildFrame::OnOffsetOffsetx32832() // -10
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetXMinus10Clicked();
}

void CChildFrame::OnOffsetOffsety()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetYPlus10Clicked();
}

void CChildFrame::OnOffsetOffsety32834()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetYMinus10Clicked();
}

void CChildFrame::OnOffsetClearalloffsets()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetClearalloffsets();
}

void CChildFrame::OnOffsetTrack()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetTrack();
}

void CChildFrame::OnOffsetTrackall()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnOffsetTrackall();
}

void CChildFrame::OnHelpTestedge()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnHelpTestedge();
}

void CChildFrame::OnRegionofinterestDeleteallplygons()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestDeleteallplygons();
}

bool TestVectCall(std::vector<int> & data, int i){
	data[100]=0;
	if (i>0) data[100]=0;
	return false;
}
bool TestVectCall(int * data, int i){
	data[100]=0;
	if (i>0) data[100]=0;
	return false;
}
void CChildFrame::OnHelpTestvectref()
{
	// TODO: Add your command handler code here
	std::vector<int> data;
	data.resize(200000);
	CWaitCursor dummy;
	int * dd = new int[2000];
	for (int i=0; i<3000000000; i++){
		TestVectCall(data, i);
//		TestVectCall(dd, i);
	}
}

void CChildFrame::OnFileOpentiffz()
{
	// TODO: Add your command handler code here
	CString FileName;

//	Alert0("Hi");
   	CFileDialog
	diag( true, NULL, FileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	CWaitCursor dummy;

	FileName = diag.GetPathName();

	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	if (!pDoc->OpenTifZStack(FileName)){
		return;
	}
	Tree * tree = GetTree();
	tree->DeleteTreeView();
	tree->CreateTreeView();
	tree->CreateFileNumberButtons();
//	tree->CreateButtons();

	tree->Invalidate();


	this->SetWindowTextA(FileName);


	//ShowFloat(pDoc->dat.NImages, "NI");

}

void CChildFrame::OnRegionofinterestCalculatemembrane()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
//	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pT->MakeMembranePolygons();

}

void CChildFrame::OnRegionofinterestCalcsumvalues()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnRegionofinterestCalcsumvalues();
}

void CChildFrame::OnRegionofinterestCalculatemembranesums()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnRegionofinterestCalculateCircularGradient(true);
}

void CChildFrame::OnRegionofinterestMakeopposingclosedroisformalinearroi()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
//	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pT->OnRegionofinterestMakeopposingclosedroisformalinearroi();

}

void CChildFrame::OnRegionofinterestCalculategradientforalinealpoly()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnRegionofinterestCalculateCircularGradient(false);
}

void CChildFrame::OnRegionofinterestCopyselectedpolygon32850()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestCopyselectedpolygon32850();
}

void CChildFrame::OnRegionofinterestPastecopiedpolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestPastecopiedpolygon();
}

void CChildFrame::OnRegionofinterestCalcarea()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnRegionofinterestCalcarea();
}

void CChildFrame::OnHelpFill()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnHelpFill();
}

void CChildFrame::OnHelpStartfill()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnHelpStartFill();
}

void CChildFrame::OnFillDefinethreshold()
{
	// TODO: Add your command handler code here
	DiagFillParams diag;
	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
	diag.UseGloabMean = frame->UseGlobalMeanForFill;
	diag.Threshold = frame->PercentageAcceptanceFill;

	if (diag.DoModal()!=IDOK){
		return;
	}
	frame->PercentageAcceptanceFill = diag.Threshold;
	frame->UseGlobalMeanForFill=diag.UseGloabMean;

}

void CChildFrame::OnFillStartselectpoints()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillStartselectpoints();
}

void CChildFrame::OnFillStopselectpoints()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillStopselectpoints();
}


void CChildFrame::OnFillClear()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillClear();
}

void CChildFrame::OnFillUndolast()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillUndolast();
}

void CChildFrame::OnFillTogglehide()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillTogglehide();
}

void CChildFrame::OnFillSaveonfile()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillSaveonfile();
}

void CChildFrame::OnFillLoadfromfile()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillLoadfromfile();
}

void CChildFrame::OnFillFillcurrentlyselectedpolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillFillcurrentlyselectedpolygon();
}

void CChildFrame::OnViewViewfilledarea()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewfilledarea() ;
}

void CChildFrame::OnFileOpenstandardtiff()
{
	// TODO: Add your command handler code here
	CString FileName;

//	Alert0("Hi");
   	CFileDialog
	diag( true, NULL, FileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	CWaitCursor dummy;

	FileName = diag.GetPathName();

	DoOpenStandardTif(FileName);

	return;

}

bool CChildFrame::DoOpenStandardTif(CString FileName){
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	if (!pDoc->OpenStandardTif(FileName)){
		return false;
	}
	Tree * tree = GetTree();
	tree->DeleteTreeView();
	tree->CreateTreeView();
	tree->CreateFileNumberButtons();
//	tree->CreateButtons();

	tree->Invalidate();


	this->SetWindowTextA(FileName);
	pDoc->SetPathName(FileName);

	pV->Invalidate();

	Traces * pT = GetTraces();
	pT->Invalidate();


	return true;
	//ShowFloat(pDoc->dat.NImages, "NI");

}

void CChildFrame::OnRegionofinterestSelectpolygonbyindex()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestSelectpolygonbyindex() ;
}

void CChildFrame::OnLineDefinenewline()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineDefinenewline();
}

void CChildFrame::OnLineDeleteselectedline()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineDeleteselectedline();
}

void CChildFrame::OnLineDeletealllines()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineDeletealllines();
}

void CChildFrame::OnLineSelectlinebyindex()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineSelectlinebyindex();
}

void CChildFrame::OnLineMakepolygonaroundselectedline()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineMakepolygonaroundselectedline();
}

void CChildFrame::OnLineCopyselectedline()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineCopyselectedline();
}

void CChildFrame::OnLinePastecopiedline()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLinePastecopiedline();
}

void CChildFrame::OnRegionofinterestMoveselectedpolywitharrowkeys()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestMoveselectedpolywitharrowkeys();
}

void CChildFrame::OnLineMoveselectedpolywitharrowkeys()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineMoveselectedpolywitharrowkeys();
}

void CChildFrame::OnFileSaveimagesinanavisionformat()
{
	// TODO: Add your command handler code here

	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	int byteswritten = 0;
	int from=0;
	int to = pDoc->dat.NImages-1;
	io myio;
	if (myio.InInt(from, "from image")!= IDOK){
		return;
	}
	if (myio.InInt(to, "to image")!= IDOK){
		return;
	}
	pDoc->dat.OnFileSaveimagesinanavisionformat(byteswritten, pDoc->id);
}
void CChildFrame::OnFileOpenanavisionformat(){
	// TODO: Add your command handler code here

	CString FileName;

//	Alert0("Hi");
   	CFileDialog
	diag( true, NULL, FileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	CWaitCursor dummy;

	FileName = diag.GetPathName();
	DoOpenAnvisionFormat(FileName);
}

bool CChildFrame::DoOpenAnvisionFormat(CString FileName){
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());

	if (!pDoc->Openanavisionformat(FileName)){
		return false;
	}
	Tree * tree = GetTree();
	tree->DeleteTreeView();
	tree->CreateTreeView();
	tree->CreateFileNumberButtons();
//	tree->CreateButtons();

	tree->Invalidate();
	this->SetWindowTextA(FileName);
	pDoc->SetPathName(FileName);

	pV->Invalidate();

	Traces * pT = GetTraces();
	pT->Invalidate();


	return true;

}

void CChildFrame::OnAnalyzeFret()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	CAnaVisionView * pCAna = this->GetCAnaVisionView();

//	ShowFloat(pT->selectedpolygon, "pT->selectedpolygon");
	pCAna->OnAnalyzeFret(pT->selectedpolygon);

}

void CChildFrame::OnRegionofinterestConverttoline()
{
	// TODO: Add your command handler code here
		Traces * pT = GetTraces();
		pT->OnRegionofinterestConverttoline();
}

void CChildFrame::OnLineConvertselectedlinetopolygon()
{
	// TODO: Add your command handler code here
		Traces * pT = GetTraces();
		pT->OnLineConvertselectedlinetopolygon();
}

void CChildFrame::OnLineCalclength()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnLineCalclength();
}

void CChildFrame::OnRegionofinterestCalccircumferenceofselectedpolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestCalccircumferenceofselectedpolygon();
}

void CChildFrame::OnFillStartselectpointsinselectedpolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillStartselectpointsinselectedpolygon();
}

void CChildFrame::OnCountStart()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountStart();
}

void CChildFrame::OnFillStopselectpointsinselectedpolygon()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnFillStopselectpointsinselectedpolygon();
}

void CChildFrame::OnCountStop()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountStop();
}

void CChildFrame::OnCountClear()
{
	Traces * pT = GetTraces();
	pT->OnCountClear();
	// TODO: Add your command handler code here
}

void CChildFrame::OnCountSetsize()
{
	Traces * pT = GetTraces();
	pT->OnCountSetsize();
	// TODO: Add your command handler code here
}

void CChildFrame::OnRegionofinterestCreatenpolygonsinside()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestCreatenpolygonsinside();
}

void CChildFrame::OnFileSavethisstackinanavisionformat()
{
	// TODO: Add your command handler code here

	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	int byteswritten = 0;
	int from=0;
	int to = pDoc->dat.images[pDoc->dat.ImageIndex].LastIndex;
	io myio;
	if (myio.InInt(from, "from image")!= IDOK){
		return;
	}
	if (myio.InInt(to, "to image")!= IDOK){
		return;
	}
//	ShowFloat(pDoc->dat.ImageIndex, "pDoc->dat.ImageIndex");
	pDoc->dat.OnFileSavethisstackinanavisionformat(byteswritten, pDoc->id, pDoc->dat.ImageIndex, from, to);
}



void CChildFrame::OnTrackcellsDetectcells()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
//	CView* pV = GetActiveView( );
//	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
//	pDoc->dat.celllist.celllist.resize(0);

	pT->OnTrackcellsDetectcells();
}

void CChildFrame::OnTrackcellsCountcells()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->CountCells( );
}

void CChildFrame::OnTrackcellsStarttrackingcellsmode()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnTrackcellsStarttrackingcellsmode();
}

void CChildFrame::OnTrackcellsStoptrackingcellsmoded()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnTrackcellsStoptrackingcellsmoded();
}

void CChildFrame::OnViewViewphasors()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnViewViewphasors() ;
}

void CChildFrame::OnPhasorLoadphasortxtfiles()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnPhasorLoadphasortxtfiles();
}

void CChildFrame::OnPhasorLoadphasortiffiles()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnPhasorLoadphasortiffiles();
}

void CChildFrame::OnRegionofinterestCopyallpolygons()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnRegionofinterestCopyAllPolygons();
}

void CChildFrame::OnFileSaveflimfretpoject()
{
	// TODO: Add your command handler code here
	CView* pV = GetActiveView( );
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	pDoc->SaveFlimFretProject(true);
}

void CChildFrame::OnFileLoadflimfretproject()
{
	// TODO: Add your command handler code here
	CString FileName;

//	Alert0("Hi");
   	CFileDialog
	diag( true, NULL, FileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, this);

	if (diag.DoModal()!=IDOK){
		return;
	}
	CWaitCursor dummy;

	FileName = diag.GetPathName();
	
	CFile file;
	if (!file.Open(FileName, CFile::modeRead)){
		Alert0("could not open file for read");
		return;
	}


	CAnaVisionView * pV = GetCAnaVisionView();
	CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
	pDoc->DoLoadFlimFretProject(file);
//		Alert0("hi2");
	pV->LoadFlimFret(file);
	Traces * pT = GetTraces();
	pT->Invalidate();
	

}

void CChildFrame::OnLocalizationCalccoefficients()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnLocalizationCalccoefficients(0) ;

}

void CChildFrame::OnLocalizationCalcmanders1()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnLocalizationCalccoefficients(1) ;
}

void CChildFrame::OnLocalizationCalcmanders2()
{
	// TODO: Add your command handler code here
	CAnaVisionView * pCAna = this->GetCAnaVisionView();
	pCAna->OnLocalizationCalccoefficients(2) ;
}

void CChildFrame::OnCountStartvesiclecount()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountStartvesiclecount();
}

void CChildFrame::OnCountSetrimsize()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountSetrimsize();
}

void CChildFrame::OnCountWritevesiclestoclipboard()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountWritevesiclestoclipboard();
}

void CChildFrame::OnCountReadvesiclesfromclipboard()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountReadvesiclesfromclipboard();
}

void CChildFrame::OnCountGetvesiclesinpoly()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountGetvesiclesinpoly();
}

void CChildFrame::OnCountSetcutoffsd()
{
	// TODO: Add your command handler code here
	Traces * pT = GetTraces();
	pT->OnCountSetcutoffsd();
}
