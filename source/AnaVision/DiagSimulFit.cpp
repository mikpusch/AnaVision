// DiagSimulFit.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "DiagSimulFit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiagSimulFit dialog


DiagSimulFit::DiagSimulFit(CWnd* pParent /*=NULL*/)
	: CDialog(DiagSimulFit::IDD, pParent)
{
	//{{AFX_DATA_INIT(DiagSimulFit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DiagSimulFit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DiagSimulFit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_KD_FREE, k.KFree);
	DDX_Text(pDX, IDC_CONC_FREE, k.TotalFree);
	

	DDX_Text(pDX, IDC_KD_FIXED, k.KFixed);
	DDX_Text(pDX, IDC_AMOUNT_FIXED, k.AmountFixed);
	
	DDX_Check(pDX, IDC_FIT_KDFREE, k.FitKFree);
	DDX_Check(pDX, IDC_FIT_CONCFREE, k.FitTotalFree);
	DDX_Check(pDX, IDC_FIT_KDFIXED, k.FitKFixed);
	DDX_Check(pDX, IDC_FIT_AMOUNTFIXED, k.FitAmountFixed);

	DDX_Text(pDX, IDC_DR, k.DR);
	DDX_Text(pDX, IDC_DT0, k.DT0);
	DDX_Text(pDX, IDC_FACT_T, k.factdt);
	DDX_Text(pDX, IDC_DT_MAX, k.DTMAX);


	DDX_Check(pDX, IDC_CHECKFITD, k.FitD);
	DDX_Text(pDX, IDC_DIFFCOEFF, k.D);
	DDX_Text(pDX, IDC_CURRENT, k.Current);

	DDX_Text(pDX, IDC_NITERATIONS_SIMULFIT, k.MaxIterSimulFit);

	DDX_Check(pDX, IDC_CHECKFITCURRENT, k.FitCurrent);
	DDX_Text(pDX, IDC_STEPSIZE, k.StepSize);
	

	DDX_Check(pDX, IDC_FIT_THICKNESSFIXED, k.FitThicknessFixedBufferVolume);
	DDX_Text(pDX, IDC_THICKNESS_FIXED, k.ThicknessFixedBufferVolume);
	
	DDX_Text(pDX, IDC_RMAXSIMUL, k.RMaxSimulation);
	

}


BEGIN_MESSAGE_MAP(DiagSimulFit, CDialog)
	//{{AFX_MSG_MAP(DiagSimulFit)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiagSimulFit message handlers
