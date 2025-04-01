// DiagKinetics.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "DiagKinetics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiagKinetics dialog


DiagKinetics::DiagKinetics(CWnd* pParent /*=NULL*/)
	: CDialog(DiagKinetics::IDD, pParent)
{
	//{{AFX_DATA_INIT(DiagKinetics)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/*
DiagKinetics::~DiagKinetics(){
	k.ClearArrays();
}
*/

void DiagKinetics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DiagKinetics)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP


//	DDX_Check(pDX, IDC_CHECKTIMEFUNCTION, k.ShowAsFunctionOfTime);
	DDX_Check(pDX, IDC_CHECKTRACKDRIFT, k.TrackDrift);

	DDX_Check(pDX, IDC_CHECKCONVERTTODELTAHTOTAL, k.ConvertToDeltaHTotal);

	DDX_Check(pDX, IDC_CHECKFITD, k.FitD);

	DDX_Text(pDX, IDC_RMIN, k.IndexRMin);
	DDX_Text(pDX, IDC_RMAX, k.IndexRMax);
	DDX_Text(pDX, IDC_DELTAR, k.DeltaRIndex);
	DDX_Text(pDX, IDC_TIMEMIN, k.IndexTimeMin);
	DDX_Text(pDX, IDC_TIMEMAX, k.IndexTimeMax);
	DDX_Text(pDX, IDC_DELTATIME, k.DeltaTimeIndex);
	DDX_Text(pDX, IDC_TIMEBETWEENIMAGES, k.dt);
	DDX_Text(pDX, IDC_PIXELSIZE, k.pixelsize);
	DDX_Text(pDX, IDC_ROOCYTE, k.a);
	DDX_Text(pDX, IDC_CURRENT, k.Current);
	DDX_Text(pDX, IDC_DIFFCOEFF, k.D);
	DDX_Text(pDX, IDC_BUFFERCAP, k.beta);
	DDX_Text(pDX, IDC_FIRSTSTART, k.BackgroundTimeIndex0);
	DDX_Text(pDX, IDC_LASTSTART, k.BackgroundTimeIndex1);

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

	DDX_Text(pDX, IDC_NITERATIONS_SIMULFIT, k.MaxIterSimulFit);

	DDX_Check(pDX, IDC_CHECKFITCURRENT, k.FitCurrent);
	DDX_Text(pDX, IDC_STEPSIZE, k.StepSize);
	

	DDX_Check(pDX, IDC_FIT_THICKNESSFIXED, k.FitThicknessFixedBufferVolume);
	DDX_Text(pDX, IDC_THICKNESS_FIXED, k.ThicknessFixedBufferVolume);
	DDX_Text(pDX, IDC_RMAXSIMUL, k.RMaxSimulation);

	DDX_Check(pDX, IDC_CHECK_OFFFOCUS, k.UseOffFocusCorrection);

	DDX_Text(pDX, IDC_N_OFF_FOCUS_COEFFS, k.NCoeffOffFocusCorrection);
	DDX_Text(pDX, IDC_GAUSSIAN_WIDTH, GaussWidth);

}


BEGIN_MESSAGE_MAP(DiagKinetics, CDialog)
	//{{AFX_MSG_MAP(DiagKinetics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiagKinetics message handlers

void DiagKinetics::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	k.HalfWidthOffFocusCorrectionInMeters = GaussWidth * 1e-6;
}

BOOL DiagKinetics::OnInitDialog() 
{
	GaussWidth = k.HalfWidthOffFocusCorrectionInMeters * 1e6;
//	ShowFloat(GaussWidth, "Gausswidth");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return true;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}
