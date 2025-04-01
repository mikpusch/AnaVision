// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "BufferedDiffusion.h"

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
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	StatusSimThread = 0;
	IsSimulating = FALSE;
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
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

LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message){

		case MESSAGE_GO_GO	:		return MessageGoGo();
											break;
		case MESSAGE_FINISHED	:	return MessageFinished();
											break;
		case MESSAGE_DRAWCURVES	:	return MessageDrawCurves();
											break;
		case SAVEENEGRYTIMERMESSAGE:	return MessageSaveEnergyTimer();
											break;
		case MESSAGE_GO_FINISHED:	return MessageGoFinished();
											break;
		default							: 	return CMDIChildWnd::WindowProc(message, wParam, lParam);
											break;
	}
}


LRESULT CChildFrame::MessageGoGo(){
	CBufferedDiffusionView * pV = (CBufferedDiffusionView *) GetActiveView( );
	pV->MessageRedraw();
	return 0;
}
LRESULT CChildFrame::MessageSaveEnergyTimer(){
	CBufferedDiffusionView * pV = (CBufferedDiffusionView *) GetActiveView( );
	pV->MessageEnergySave();
	return 0;
}
LRESULT CChildFrame::MessageFinished(){
	CBufferedDiffusionView * pV = (CBufferedDiffusionView *) GetActiveView( );
	pV->SimulationFinished();
	return 0;
}

LRESULT CChildFrame::MessageDrawCurves(){
	CBufferedDiffusionView * pV = (CBufferedDiffusionView *) GetActiveView( );
	pV->MessageDrawCurves(index);
	return 0;
}

LRESULT CChildFrame::MessageGoFinished(){
	CBufferedDiffusionView * pV = (CBufferedDiffusionView *) GetActiveView( );
	pV->MessageGoFinished();
	return 0;}

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (IsSimulating){
		Alert0("Wait until simulation is finished or stop it");
//		CBufferedDiffusionView * pV = (CBufferedDiffusionView *) GetActiveView( );
//		pV->OnStopStopseriessimulation();
		return;
	}
	CMDIChildWnd::OnClose();
}



