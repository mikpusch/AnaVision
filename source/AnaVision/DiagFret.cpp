// DiagFret.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagFret.h"


// DiagFret dialog

IMPLEMENT_DYNAMIC(DiagFret, CDialog)

DiagFret::DiagFret(CWnd* pParent /*=NULL*/)
	: CDialog(DiagFret::IDD, pParent)
{

}

DiagFret::~DiagFret()
{
}

void DiagFret::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BTDON, p.FRETBTDonor);
	DDX_Text(pDX, IDC_EDIT_BTACC, p.FRETBTAcceptor);
	DDX_Text(pDX, IDC_EDIT_THRESHOLDFRET, p.ThresholdFret);
	DDX_Check(pDX, IDC_CHECK_NORMALIZED_FRET, p.CalcNormalizedFret);
}


BEGIN_MESSAGE_MAP(DiagFret, CDialog)
END_MESSAGE_MAP()


// DiagFret message handlers
