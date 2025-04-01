// DiagFitD.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "DiagFitD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiagFitD dialog


DiagFitD::DiagFitD(CWnd* pParent /*=NULL*/)
	: CDialog(DiagFitD::IDD, pParent)
{
	//{{AFX_DATA_INIT(DiagFitD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DiagFitD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DiagFitD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Check(pDX, IDC_CHECKFITD, k.FitD);
	DDX_Text(pDX, IDC_DIFFCOEFF, k.D);


}


BEGIN_MESSAGE_MAP(DiagFitD, CDialog)
	//{{AFX_MSG_MAP(DiagFitD)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiagFitD message handlers
