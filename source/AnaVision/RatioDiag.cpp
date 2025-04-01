// RatioDiag.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
//#include "RatioDiag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RatioDiag dialog


RatioDiag::RatioDiag(CWnd* pParent /*=NULL*/)
	: CDialog(RatioDiag::IDD, pParent)
{
	NStringsAB = 0;
	indexa=indexb=indexc=-1;

	StringsAB = NULL;
	//{{AFX_DATA_INIT(RatioDiag)
	m_ComboResult = _T("");
	m_ListA = _T("");
	m_ListB = _T("");
	m_BackgroundA = 0.0;
	m_BackgroundB = 0.0;
	m_NSurroundForAverage = 0;
	//}}AFX_DATA_INIT
}


void RatioDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RatioDiag)
	DDX_CBString(pDX, IDC_COMBO_RESULT, m_ComboResult);
	DDX_LBString(pDX, IDC_LIST_A, m_ListA);
	DDX_LBString(pDX, IDC_LIST_B, m_ListB);
	DDX_Text(pDX, IDC_BACKGROUND_A, m_BackgroundA);
	DDX_Text(pDX, IDC_BACKGROUND_B, m_BackgroundB);
	DDX_Text(pDX, IDC_N_NEIGHBORS, m_NSurroundForAverage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RatioDiag, CDialog)
	//{{AFX_MSG_MAP(RatioDiag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RatioDiag message handlers

BOOL RatioDiag::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CListBox * la = (CListBox *) (this->GetDlgItem(IDC_LIST_A));
	CListBox * lb = (CListBox *) (this->GetDlgItem(IDC_LIST_B));
	CComboBox * lc = (CComboBox *) (this->GetDlgItem(IDC_COMBO_RESULT));
	for (int i=0; i<NStringsAB; i++){
		la->AddString(StringsAB[i]);
		lb->AddString(StringsAB[i]);
		lc->AddString(StringsAB[i]);
	}

	if (indexa>=0){
		la->SetCurSel(indexa);
	}
	if (indexb>=0){
		lb->SetCurSel(indexa);
	}
	if (indexb>=0){
		lc->SetCurSel(indexa);
	}


	
	return true;  // return true unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return false
}

void RatioDiag::OnOK() 
{
	// TODO: Add extra validation here
	CListBox * la = (CListBox *) (this->GetDlgItem(IDC_LIST_A));
	CListBox * lb = (CListBox *) (this->GetDlgItem(IDC_LIST_B));
	CComboBox * lc = (CComboBox *) (this->GetDlgItem(IDC_COMBO_RESULT));

	

	InvalidSelection = false;

	indexa=la->GetCurSel();
//	ShowFloat(indexa, "indexa");
	if (indexa==LB_ERR){
		InvalidSelection = true;
	}
	indexb=lb->GetCurSel();
//	ShowFloat(indexb, "indexb");

	if (indexb==LB_ERR){
		InvalidSelection = true;
	}

	indexc=lc->GetCurSel();
//	ShowFloat(indexc, "indexc");

	if (indexc==CB_ERR){
		lc->GetWindowText(NewResultString);
//		Alert0(NewResultString);
		if (NewResultString.GetLength()==0){
			InvalidSelection = true;
		}
	}



	CDialog::OnOK();
}
