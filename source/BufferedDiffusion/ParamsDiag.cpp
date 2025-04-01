// ParamsDiag.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"

#include <math.h>
//#include "ParamsDiag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParamsDiag dialog


ParamsDiag::ParamsDiag(CWnd* pParent /*=NULL*/)
	: CDialog(ParamsDiag::IDD, pParent)
{
	//{{AFX_DATA_INIT(ParamsDiag)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ParamsDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ParamsDiag)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_DT, d.DT0);
	DDX_Text(pDX, IDC_DT2, d.DTMax);
	DDX_Text(pDX, IDC_DT3, d.FactDT);
	DDX_Text(pDX, IDC_R0, d.r0);
	DDX_Text(pDX, IDC_RMAX, d.rmax);
	DDX_Text(pDX, IDC_DR, d.DR0);
	DDX_Text(pDX, IDC_MAXDR, d.DRMax);
	DDX_Text(pDX, IDC_FACTDR, d.FactDR);
	DDX_Text(pDX, IDC_NSLICES, d.M);
	DDX_Text(pDX, IDC_DH, d.DH);
	DDX_Text(pDX, IDC_DB, d.DB);
	DDX_Text(pDX, IDC_ALPHA, d.alpha);
	DDX_Text(pDX, IDC_BETA, d.beta);
	DDX_Text(pDX, IDC_KD, d.KD);
	DDX_Text(pDX, IDC_PHBULK, d.pHbulk);
	DDX_Text(pDX, IDC_TOTALBUFFER, d.T);
	DDX_Text(pDX, IDC_IH, d.IH);
	DDX_Text(pDX, IDC_RANGEPH, p.RangepH);
	DDX_Text(pDX, IDC_FACTORCORRECTION, d.factorcorrection);

	DDX_Text(pDX, IDC_NITER, p.MaxIter);
	DDX_Text(pDX, IDC_NUPDATE, p.UpdateEvery);

	DDX_Check(pDX, IDC_USEWAGNERKEIZER, d.UseWagnerKeizer);
	DDX_Check(pDX, IDC_SIMPLEWAGNERKEIZER, d.SimpleWagnerKeizer);

	DDX_Check(pDX, IDC_WRITEFILES, p.WriteOutFiles);
	DDX_Text(pDX, IDC_OUTFILENAMES, p.OutFileName);

	DDX_Text(pDX, IDC_TIMECURRENTON, d.TimeCurrentOn);
	DDX_Text(pDX, IDC_TIMECURRENTOFF, d.TimeCurrentOff);
	DDX_Text(pDX, IDC_NPULSESTRAIN, d.NPulsesInTrain);

	DDX_Check(pDX, IDC_RENORMALIZE, d.DoRenormalize);

	DDX_Text(pDX, IDC_NROOTS, d.NRootsUsed);

	DDX_Check(pDX, IDC_AUTOCALCBUFFERCAPAC, d.AutoCalcBeta);

	DDX_Text(pDX, IDC_EXTERNALLYSUPPLEIDBUFFERCAPAC, d.buffercapacextern);

	DDX_Check(pDX, IDC_USE_FLUX_CORRECTION, d.UseFluxCorrectionInLinearApproximation);

	
	DDX_Text(pDX, IDC_INTERNALBUFFERCAPAC, InternalCapac);

	DDX_Check(pDX, IDC_CHECK_INCLUDE_FIXED_SURFACE_BUFFER, d.IncludeFixedSurfaceBuffer);
	DDX_Text(pDX, IDC_AMOUNT_FIXED_SURFACEBUFFER, d.AmountFixedSurfaceBufferInMoles);
	DDX_Text(pDX, IDC_KD_FIXED_SURFACEBUFFER, d.KDFixedSurfaceBuffer);
	

	DDX_Text(pDX, IDC_AMOUNT_FIXED_THICKNESSFIXEDSURFACE, d.ThicknessFixedVolume);
	
	DDX_Check(pDX, IDC_READFILE, p.ReadStartingFile);

	DDX_Text(pDX, IDC_NSECONDSOUT, p.NSecondsOut);

	DDX_Check(pDX, IDC_USE_PREDICTORIN_WAGNERKEIZER, d.UsePredictorInWagnerKeitzerForNonLinearTerms);
	
		


}




BEGIN_MESSAGE_MAP(ParamsDiag, CDialog)
	//{{AFX_MSG_MAP(ParamsDiag)
	ON_BN_CLICKED(IDC_CHANGEFILENAME, OnChangefilename)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParamsDiag message handlers

void ParamsDiag::OnOK() 
{
	// TODO: Add extra validation here

    CButton* pB1 = (CButton*)GetDlgItem(IDC_FIXDR);
    CButton* pB2 = (CButton*)GetDlgItem(IDC_FIXM);

	if (pB2->GetCheck()){
		d.ModeToDetermineSliceNumber = 0;
	}
	else {
		if (pB1->GetCheck()){
			d.ModeToDetermineSliceNumber = 1;
		}
	}


	pB1 = (CButton*)GetDlgItem(IDC_CALCALPHA);
	pB2 = (CButton*)GetDlgItem(IDC_CALCBETA);
	CButton* pB3 = (CButton*)GetDlgItem(IDC_CALCKD);

	if (pB1->GetCheck()){
		d.IndependentReactionParameters = 2;
	}
	else{
		if (pB2->GetCheck()){
			d.IndependentReactionParameters = 1;
		}
		else{
			if (pB3->GetCheck()){
				d.IndependentReactionParameters = 0;
			}
		}
	}
	pB1 = (CButton*)GetDlgItem(IDC_CONSTANTCURRENT);
	pB2 = (CButton*)GetDlgItem(IDC_TRAIN);
	if (pB1->GetCheck()){
		d.ProtocolType = 0;
	}
	else {
		if (pB2->GetCheck()){
			d.ProtocolType = 1;
		}
	}
	CDialog::OnOK();
}

BOOL ParamsDiag::OnInitDialog() 
{
	double Hbulk = pow(10, -d.pHbulk);
	InternalCapac = d.KD*d.T/pow(d.KD+Hbulk, 2.0);
	//ShowFloat(InternalCapac, "IC");
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


    CButton* pB1 = (CButton*)GetDlgItem(IDC_FIXDR);
	pB1->SetCheck(0);
    CButton* pB2 = (CButton*)GetDlgItem(IDC_FIXM);
	pB2->SetCheck(0);

	switch (d.ModeToDetermineSliceNumber) {
		case 0		: pB2->SetCheck(1);break;
		case 1		: pB1->SetCheck(1);break;
		default		: pB2->SetCheck(1);break;
	}


	pB1 = (CButton*)GetDlgItem(IDC_CALCALPHA);
	pB1->SetCheck(0);
	pB2 = (CButton*)GetDlgItem(IDC_CALCBETA);
	pB2->SetCheck(0);
	CButton* pB3 = (CButton*)GetDlgItem(IDC_CALCKD);
	pB3->SetCheck(0);

	switch (d.IndependentReactionParameters) {
		case 0		: pB3->SetCheck(1);break;
		case 1		: pB2->SetCheck(1);break;
		case 2		: pB1->SetCheck(1);break;
		default		: pB1->SetCheck(1);break;
	}

	pB1 = (CButton*)GetDlgItem(IDC_CONSTANTCURRENT);
	pB1->SetCheck(0);
	pB2 = (CButton*)GetDlgItem(IDC_TRAIN);
	pB2->SetCheck(0);

	switch (d.ProtocolType) {
		case 0		: pB1->SetCheck(1);break;
		case 1		: pB2->SetCheck(1);break;
		default		: pB1->SetCheck(1);break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ParamsDiag::OnChangefilename() 
{
	// TODO: Add your control notification handler code here

	CString dir = p.OutFileName;
	int l1 = dir.ReverseFind('/');
	int l2 = dir.ReverseFind('\\');
	if (l1>l2){
		l2=l1;
	}

//	dir = dir.Left(l2);
//	Alert0(dir);
	CString name = dir.Right(dir.GetLength()-l2-1);
//	Alert0(dir);
/*
	if (l2>=0){
		cf.m_ofn.lpstrTitle = dir; //dir.Right(dir.GetLength()-l2-1);
		Alert0(dir);
	}
*/
	CFileDialog cf( FALSE, NULL, name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

//	CFileDialog cf( FALSE, NULL, p.OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		NULL, NULL); //this);

	if (l2>=0){
		cf.m_ofn.lpstrInitialDir = dir.Left(l2);
	}

	if (cf.DoModal() != IDOK){
		return;
	}

	p.OutFileName = cf.GetPathName();
	UpdateData(FALSE);

}

