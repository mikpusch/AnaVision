// DiagFillParams.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagFillParams.h"


// DiagFillParams dialog

IMPLEMENT_DYNAMIC(DiagFillParams, CDialog)

DiagFillParams::DiagFillParams(CWnd* pParent /*=NULL*/)
	: CDialog(DiagFillParams::IDD, pParent)
{

}

DiagFillParams::~DiagFillParams()
{
}

void DiagFillParams::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USE_GLOBALMEAN, UseGloabMean);
	DDX_Text(pDX, IDC_FILLTHRESHOLD, Threshold);
	BOOL UseGloabMean;
	double Threshold;
}


BEGIN_MESSAGE_MAP(DiagFillParams, CDialog)
END_MESSAGE_MAP()


// DiagFillParams message handlers
