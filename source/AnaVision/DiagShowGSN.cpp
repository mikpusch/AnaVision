// DiagShowGSN.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagShowGSN.h"


// DiagShowGSN dialog

IMPLEMENT_DYNAMIC(DiagShowGSN, CDialog)

DiagShowGSN::DiagShowGSN(CWnd* pParent /*=NULL*/)
	: CDialog(DiagShowGSN::IDD, pParent)
{

}

DiagShowGSN::~DiagShowGSN()
{
}

void DiagShowGSN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_S_POINT, S);
	DDX_Text(pDX, IDC_EDIT_G_POINT, G);
	DDX_Text(pDX, IDC_EDIT_N_POINT, N);

}


BEGIN_MESSAGE_MAP(DiagShowGSN, CDialog)
END_MESSAGE_MAP()


// DiagShowGSN message handlers
