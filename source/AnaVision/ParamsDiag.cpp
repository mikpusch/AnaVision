// ParamsDiag.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "ParamsDiag.h"
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

	DDX_Text(pDX, IDC_SIZE_X, p.xsizeimage);
	DDX_Text(pDX, IDC_SIZE_Y, p.ysizeimage);
	DDX_Text(pDX, IDC_MANUAL_MIN, p.MinManScale);
	DDX_Text(pDX, IDC_MANUAL_MAX, p.MaxManScale);
	DDX_Text(pDX, IDC_DELTA_IMAGE, p.DeltaImage);
	DDX_Text(pDX, IDC_EDIT_SCALERATIO, p.ScaleRatio);
	DDX_Text(pDX, IDC_FLIM_LASER_FREQ, p.LaserFlimFreq);

	DDX_Text(pDX, IDC_IMAGE_SCALE, p.MultiplierImages);
	DDX_Text(pDX, IDC_IMAGE_OFFSET, p.OffsetImages);

	DDX_Text(pDX, IDC_LOOKUPNAME, p.LookupTableName);
	
	
	DDX_Check(pDX, IDC_CHECK_AUTOSCALE, p.AutoScale);

	DDX_Check(pDX, IDC_CHECK_SUBTRACTREFERNCE, p.SubtractInitialValuesForDraw);
	
 

	DDX_Text(pDX, 	IDC_A1_490_450, p.A1_490_450);
	DDX_Text(pDX, 	IDC_A2_490_450, p.A2_490_450);
	DDX_Text(pDX, 	IDC_DX_490_450, p.dx_490_450);
	DDX_Text(pDX, 	IDC_X0_490_450, p.x0_490_450);

	DDX_Text(pDX, 	IDC_A1_490_440, p.A1_490_440);
	DDX_Text(pDX, 	IDC_A2_490_440, p.A2_490_440);
	DDX_Text(pDX, 	IDC_DX_490_440, p.dx_490_440);
	DDX_Text(pDX, 	IDC_X0_490_440, p.x0_490_440);

	DDX_Text(pDX, 	IDC_TIME_DIFFERENCE, p.dt);
	DDX_Text(pDX, 	IDC_PIXELSIZE, p.PixelSizeInMicrom);
	DDX_Text(pDX, 	IDC_MAGNIFICATION, p.Magnification);
	DDX_Text(pDX, 	IDC_THEORETICAL_PK, p.theoretical_pK);
	DDX_Text(pDX, 	IDC_TOTALAMOUNT, p.totalbuffermM);
	DDX_Text(pDX, 	IDC_LINEARCONVERSIONFACTOR, p.linearconversionfactor);

	DDX_Text(pDX, 	IDC_CONVERSION_RATIO_TO_DELTA_H, p.FactorRatioIntoDeltaH);

}


BEGIN_MESSAGE_MAP(ParamsDiag, CDialog)
	//{{AFX_MSG_MAP(ParamsDiag)
	ON_BN_CLICKED(IDC_LOAD_LOOKUPTABLE, OnLoadLookuptable)
	ON_BN_CLICKED(IDC_MAKE_GREY, OnMakeGrey)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MAKE_RED, &ParamsDiag::OnBnClickedMakeRed)
	ON_BN_CLICKED(IDC_MAKE_GREEN, &ParamsDiag::OnBnClickedMakeGreen)
	ON_BN_CLICKED(IDC_MAKE_BLUE, &ParamsDiag::OnBnClickedMakeBlue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParamsDiag message handlers

void ParamsDiag::OnOK() 
{
	// TODO: Add extra validation here
	CButton * pR1 = (CButton *)(this->GetDlgItem(IDC_RADIO_RAW));
	CButton * pR2 = (CButton *)(this->GetDlgItem(IDC_RADIO_CONVERT));

	if (pR1->GetCheck()){
		p.ConvertTopH = false;
	}

	if (pR2->GetCheck()){
		p.ConvertTopH = true;
	}

	pR1 = (CButton *)(this->GetDlgItem(IDC_RADIO_490450));
	pR2 = (CButton *)(this->GetDlgItem(IDC_RADIO_490440));

	if (pR1->GetCheck()){
		p.WavelengthCombination = 0;
	}
	if (pR2->GetCheck()){
		p.WavelengthCombination = 1;
	}

	pR1 = (CButton *)(this->GetDlgItem(IDC_THEORETICAL));
	pR2 = (CButton *)(this->GetDlgItem(IDC_LINEAR));
	CButton * pR3 = (CButton *)(this->GetDlgItem(IDC_RADIO_CONVERT_RATIO_TO_DH));

	if (pR1->GetCheck()){
		p.protonconversionmode = 0;
	}
	if (pR2->GetCheck()){
		p.protonconversionmode = 1;
	}
	if (pR3->GetCheck()){
		p.protonconversionmode = 2;
	}



	
	CDialog::OnOK();
}

BOOL ParamsDiag::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CButton * pR1 = (CButton *)(this->GetDlgItem(IDC_RADIO_RAW));
	CButton * pR2 = (CButton *)(this->GetDlgItem(IDC_RADIO_CONVERT));

	pR1->SetCheck(0);
	pR2->SetCheck(0);

	if (p.ConvertTopH){
		pR2->SetCheck(1);
	}
	else{
		pR1->SetCheck(1);

	}


	pR1 = (CButton *)(this->GetDlgItem(IDC_RADIO_490450));
	pR2 = (CButton *)(this->GetDlgItem(IDC_RADIO_490440));

	pR1->SetCheck(0);
	pR2->SetCheck(0);

	switch (p.WavelengthCombination){
		case 0 : pR1->SetCheck(1); break;
		case 1 : pR2->SetCheck(1); break;
		default: pR1->SetCheck(1); break;
	}

	pR1 = (CButton *)(this->GetDlgItem(IDC_THEORETICAL));
	pR2 = (CButton *)(this->GetDlgItem(IDC_LINEAR));
	CButton * pR3 = (CButton *)(this->GetDlgItem(IDC_RADIO_CONVERT_RATIO_TO_DH));

	pR1->SetCheck(0);
	pR2->SetCheck(0);
	pR3->SetCheck(0);

	switch (p.protonconversionmode){
		case 0 : pR1->SetCheck(1); break;
		case 1 : pR2->SetCheck(1); break;
		case 2 : pR3->SetCheck(1); break;
		default: pR1->SetCheck(1); break;
	}
	

	return true;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}

void ParamsDiag::OnLoadLookuptable() 
{
	// TODO: Add your control notification handler code here

	this->UpdateData(true);

	CString InFile = p.LookupTableName;

	CFileDialog diag( true, NULL, InFile, OFN_FILEMUSTEXIST, NULL, this);
	
	if (diag.DoModal()==IDOK){
		InFile = diag.GetPathName();
	}
	else{
		return;
	}

	p.LookupTableName = InFile;

	p.TryLoadLookupTable(0);

	this->UpdateData(false);

//	Alert0(p.LookupTableName);
}

void ParamsDiag::OnMakeGrey() 
{
	// TODO: Add your control notification handler code here
	p.MakeGreyScale(0);
}

void ParamsDiag::OnBnClickedMakeRed()
{
	// TODO: Add your control notification handler code here
	p.MakeBlueScale(0);
}

void ParamsDiag::OnBnClickedMakeGreen()
{
	// TODO: Add your control notification handler code here
	p.MakeGreenScale(0);
}

void ParamsDiag::OnBnClickedMakeBlue()
{
	// TODO: Add your control notification handler code here
	p.MakeRedScale(0);
}
