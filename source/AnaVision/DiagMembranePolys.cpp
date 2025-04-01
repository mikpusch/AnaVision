// DiagMembranePolys.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagMembranePolys.h"


// DiagMembranePolys dialog

IMPLEMENT_DYNAMIC(DiagMembranePolys, CDialog)

DiagMembranePolys::DiagMembranePolys(CWnd* pParent /*=NULL*/)
	: CDialog(DiagMembranePolys::IDD, pParent)
	, m_IsCircular(0)
	, m_MembraneSize(0)
{

}

DiagMembranePolys::~DiagMembranePolys()
{
}

void DiagMembranePolys::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ROIS_ARE_CIRCULAR, this->m_IsCircular);

	DDX_Text(pDX, IDC_SIZE_MEMBRANE, this->m_MembraneSize);



}


BEGIN_MESSAGE_MAP(DiagMembranePolys, CDialog)
END_MESSAGE_MAP()


// DiagMembranePolys message handlers
