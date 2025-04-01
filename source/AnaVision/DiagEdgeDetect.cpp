// DiagEdgeDetect.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagEdgeDetect.h"


// DiagEdgeDetect dialog

IMPLEMENT_DYNAMIC(DiagEdgeDetect, CDialog)

DiagEdgeDetect::DiagEdgeDetect(CWnd* pParent /*=NULL*/)
	: CDialog(DiagEdgeDetect::IDD, pParent)
	, LowThreshold(0)
	, HighThreshold(0)
	, MaxDistPolyPoints(0)
	, MaxPolyPoints(0)
	, GaussianKernelRadius(0)
	, gaussianKernelWidth(0)
{

}

DiagEdgeDetect::~DiagEdgeDetect()
{
}

void DiagEdgeDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOW_THRESHOLD, LowThreshold);
	DDX_Text(pDX, IDC_EDIT_HIGH_THRESHOLD, HighThreshold);
	DDX_Text(pDX, IDC_EDIT_MAXDISTPOLYPOINTS, MaxDistPolyPoints);
	DDX_Text(pDX, IDC_EDIT_MAXPOINTSEACHPOLY, MaxPolyPoints);
	DDX_Text(pDX, IDC_EDITGAUSRADIUS, GaussianKernelRadius);
	DDX_Text(pDX, IDC_EDITGAUSWIDTH, gaussianKernelWidth);
}


BEGIN_MESSAGE_MAP(DiagEdgeDetect, CDialog)
END_MESSAGE_MAP()


// DiagEdgeDetect message handlers
