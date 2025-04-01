// GetFramesDiag.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "GetFramesDiag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GetFramesDiag dialog


GetFramesDiag::GetFramesDiag(CWnd* pParent /*=NULL*/)
	: CDialog(GetFramesDiag::IDD, pParent)
{
	InitialFramesDisabled = false;
	//{{AFX_DATA_INIT(GetFramesDiag)
	//}}AFX_DATA_INIT
}

void GetFramesDiag::DisableInitialFrames(){
	InitialFramesDisabled = true;
}
void GetFramesDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetFramesDiag)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_TRACK_DRIFT, p.TrackDrift);
	DDX_Text(pDX, IDC_INITIALFRAME0, p.InitialFrame0);
	DDX_Text(pDX, IDC_INITIALFRAME1, p.InitialFrame1);
	DDX_Text(pDX, IDC_ANALYSISFRAME0, p.FrameToAnalyze0);
	DDX_Text(pDX, IDC_ANALYSISFRAME1, p.FrameToAnalyze1);
	DDX_Text(pDX, IDC_STEP, p.DeltaImage);
}


BEGIN_MESSAGE_MAP(GetFramesDiag, CDialog)
	//{{AFX_MSG_MAP(GetFramesDiag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetFramesDiag message handlers

BOOL GetFramesDiag::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (InitialFramesDisabled){
		CEdit * pE = (CEdit *)(this->GetDlgItem(IDC_INITIALFRAME0));
		pE->EnableWindow(false);
		pE = (CEdit *)(this->GetDlgItem(IDC_INITIALFRAME1));
		pE->EnableWindow(false);

	}
	CButton * pB1 = (CButton *)(this->GetDlgItem(IDC_STEP_BY_ONE));
	CButton * pB2 = (CButton *)(this->GetDlgItem(IDC_USE_LARGER_STEP));
	pB1->SetCheck(0);
	pB2->SetCheck(0);

	if (p.StepByOne){
		pB1->SetCheck(1);
	}
	else{
		pB2->SetCheck(1);
	}


	
	return true;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}

void GetFramesDiag::OnOK() 
{
	// TODO: Add extra validation here
	CButton * pB1 = (CButton *)(this->GetDlgItem(IDC_STEP_BY_ONE));
//	CButton * pB2 = (CButton *)(this->GetDlgItem(IDC_USE_LARGER_STEP));
	p.StepByOne = pB1->GetCheck();
	CDialog::OnOK();
}
