// RelSegButton.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RelSegButton

ZoomButton::ZoomButton()
{
	ZoomIn = false;
//	IsDoubleZoom = false;
}

ZoomButton::~ZoomButton()
{
}
BEGIN_MESSAGE_MAP(ZoomButton, CButton)
	//{{AFX_MSG_MAP(RelSegButton)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// RelSegButton message handlers
void ZoomButton::OnLButtonDown(UINT nFlags, CPoint point){
	parent->ZoomIndex = index;
	parent->Zoom = ZoomFactor; //parent->ZoomTable[index];
	parent->ZoomIn = ZoomIn;
//	ShowFloat(parent->Zoom, "zoom");

/*
	if (!ZoomIn){
		parent->Zoom = parent->ZoomTable[index];
	}
	else{
		parent->Zoom = 0;
		if (index==0){
			parent->RealZoom = 0.5;
		}
		if (index==1){
			parent->RealZoom = 0.25;
		}
		if (index==2){
			parent->RealZoom = 0.125;
		}
		if (index==3){
			parent->RealZoom = 0.0625;
		}
	}
*/
	parent->Invalidate();
//	CMainFrame* frame = (CMainFrame *)(AfxGetMainWnd( ));
//	ASSERT_VALID(frame);
//	frame->setTheSet.RelSeg=index;
//	frame->SaveASet();
//	frame->Invalidate();

}

