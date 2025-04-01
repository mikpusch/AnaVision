// DiagSelectPoly.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagSelectPoly.h"


// DiagSelectPoly dialog

IMPLEMENT_DYNAMIC(DiagSelectPoly, CDialog)

DiagSelectPoly::DiagSelectPoly(CWnd* pParent /*=NULL*/)
	: CDialog(DiagSelectPoly::IDD, pParent)
{

}

DiagSelectPoly::~DiagSelectPoly()
{
}

void DiagSelectPoly::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DiagSelectPoly, CDialog)
END_MESSAGE_MAP()


// DiagSelectPoly message handlers
int DiagSelectPoly::GetID(int i){
	switch (i){
		case 0: return IDC_RADIO_SELECT_POLY_0; break;
		case 1: return IDC_RADIO_SELECT_POLY_1; break;
		case 2: return IDC_RADIO_SELECT_POLY_2; break;
		case 3: return IDC_RADIO_SELECT_POLY_3; break;
		case 4: return IDC_RADIO_SELECT_POLY_4; break;
		case 5: return IDC_RADIO_SELECT_POLY_5; break;
		case 6: return IDC_RADIO_SELECT_POLY_6; break;
		case 7: return IDC_RADIO_SELECT_POLY_7; break;
		case 8: return IDC_RADIO_SELECT_POLY_8; break;
		case 9: return IDC_RADIO_SELECT_POLY_9; break;
		case 10: return IDC_RADIO_SELECT_POLY_10; break;
		case 11: return IDC_RADIO_SELECT_POLY_11; break;
		case 12: return IDC_RADIO_SELECT_POLY_12; break;
		case 13: return IDC_RADIO_SELECT_POLY_13; break;
		case 14: return IDC_RADIO_SELECT_POLY_14; break;
		case 15: return IDC_RADIO_SELECT_POLY_15; break;
		case 16: return IDC_RADIO_SELECT_POLY_16; break;
		case 17: return IDC_RADIO_SELECT_POLY_17; break;
		default : return IDC_RADIO_SELECT_POLY_0; break;
	}
}

BOOL DiagSelectPoly::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	if (NValidPolys>18){
		NValidPolys = 18;
	}
	for (int i=0; i<18; i++){
		int ID = GetID(i);
		CButton * pB = (CButton *) GetDlgItem(ID);
		if (i>=NValidPolys){
			pB->SetCheck(0);
			pB->EnableWindow(false);
		}
		else{
			pB->SetCheck(i==selected);
		}
	}


	return true;  // return true unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return false
}

void DiagSelectPoly::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	for (int i=0; i<NValidPolys; i++){
		int ID = GetID(i);
		CButton * pB = (CButton *) GetDlgItem(ID);
		if (pB->GetCheck()){
			selected = i;
			break;
		}
	}

	CDialog::OnOK();
}
