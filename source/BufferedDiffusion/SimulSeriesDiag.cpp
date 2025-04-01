// SimulSeriesDiag.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
//#include "SimulSeriesDiag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SimulSeriesDiag dialog


SimulSeriesDiag::SimulSeriesDiag(CWnd* pParent /*=NULL*/)
	: CDialog(SimulSeriesDiag::IDD, pParent)
{
	//{{AFX_DATA_INIT(SimulSeriesDiag)
	m_NCurves = 0;
	m_Time = 0.0;
	m_IntermediateTime = 0.0;
	m_UseTwoTimePoints = FALSE;
	//}}AFX_DATA_INIT

}


void SimulSeriesDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SimulSeriesDiag)
	DDX_Text(pDX, IDC_EDIT_NUMBERCURVES, m_NCurves);
	DDX_Text(pDX, IDC_EDIT_TOTALTIME, m_Time);
	DDX_Text(pDX, IDC_EDIT_INTERMEDIATETIME, m_IntermediateTime);
	DDX_Check(pDX, IDC_USE_TWO_TIMES_POINTS, m_UseTwoTimePoints);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(SimulSeriesDiag, CDialog)
	//{{AFX_MSG_MAP(SimulSeriesDiag)
	ON_BN_CLICKED(IDMYOK, OnMyok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SimulSeriesDiag message handlers




BOOL SimulSeriesDiag::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	Alert0("init");

	pC=new MyEditCtrl(this);

	CString c;
//	ShowFloat(ss.dt[0], "in oninitdiag dt0");


	ss.MakeToString(c);
//	Alert0(c);

	pC->SetWindowText(c);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SimulSeriesDiag::OnMyok() 
{
	// TODO: Add your control notification handler code here

	int length = pC->GetLineCount();

	ss.NValues = 0;

	if (length>0){
		for (int i=0; i<length; i++){
			char s[10000];
			for (int jj=0; jj<10000; jj++){
				s[jj]=0;
			}
			pC->GetLine(i, s, 10000);
			ss.AddString(CString(s));
		}
	}

	OnOK();
}
