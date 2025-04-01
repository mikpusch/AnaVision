// DiagLinearFit.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "DiagLinearFit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiagLinearFit dialog


DiagLinearFit::DiagLinearFit(CWnd* pParent /*=NULL*/)
	: CDialog(DiagLinearFit::IDD, pParent)
{
	//{{AFX_DATA_INIT(DiagLinearFit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DiagLinearFit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DiagLinearFit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Check(pDX, IDC_CHECKFITD, k.FitD);
	DDX_Text(pDX, IDC_DIFFCOEFF, k.D);

	DDX_Check(pDX, IDC_CHECK_OFFFOCUS, k.UseOffFocusCorrection);

	DDX_Text(pDX, IDC_N_OFF_FOCUS_COEFFS, k.NCoeffOffFocusCorrection);
	DDX_Text(pDX, IDC_GAUSSIAN_WIDTH, GaussWidth);

}


BEGIN_MESSAGE_MAP(DiagLinearFit, CDialog)
	//{{AFX_MSG_MAP(DiagLinearFit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiagLinearFit message handlers

BOOL DiagLinearFit::OnInitDialog() 
{
	GaussWidth = k.HalfWidthOffFocusCorrectionInMeters * 1e6;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return true;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}

void DiagLinearFit::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	k.HalfWidthOffFocusCorrectionInMeters = GaussWidth * 1e-6;
}


