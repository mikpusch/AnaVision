// RedrawDiag.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
//#include "RedrawDiag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RedrawDiag dialog


RedrawDiag::RedrawDiag(CWnd* pParent /*=NULL*/)
	: CDialog(RedrawDiag::IDD, pParent)
{
	//{{AFX_DATA_INIT(RedrawDiag)
	m_RefIndex = 0;
	m_ShowRef = FALSE;
	//}}AFX_DATA_INIT
	pMem = NULL;
}


void RedrawDiag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RedrawDiag)
	DDX_Text(pDX, IDC_REFINDEX, m_RefIndex);
	DDX_Check(pDX, IDC_SHOWREF, m_ShowRef);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_REDRAW_INDEX, RedrawIndex);
	DDX_Text(pDX, IDC_NSIMUL, NSimul);

	
}


BEGIN_MESSAGE_MAP(RedrawDiag, CDialog)
	//{{AFX_MSG_MAP(RedrawDiag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RedrawDiag message handlers

BOOL RedrawDiag::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CButton* pB1 = (CButton*)GetDlgItem(IDC_RADIO_GRAY);
    CButton* pB2 = (CButton*)GetDlgItem(IDC_RADIO_BLACK);
    CButton* pB3 = (CButton*)GetDlgItem(IDC_RADIO_RED);
    CButton* pB4 = (CButton*)GetDlgItem(IDC_RADIO_GREEN);
    CButton* pB5 = (CButton*)GetDlgItem(IDC_RADIO_BLUE);
    CButton* pB6 = (CButton*)GetDlgItem(IDC_RADIO_YELLOW);
    CButton* pB7 = (CButton*)GetDlgItem(IDC_RADIO_ORANGE);
    CButton* pB8 = (CButton*)GetDlgItem(IDC_RADIO_PINK);
    CButton* pB9 = (CButton*)GetDlgItem(IDC_RADIO_BROWN);
    CButton* pB10 = (CButton*)GetDlgItem(IDC_RADIO_LIGHTBLUE);


	pB1->SetCheck(0);
	pB2->SetCheck(0);
	pB3->SetCheck(0);
	pB4->SetCheck(0);
	pB5->SetCheck(0);
	pB6->SetCheck(0);
	pB7->SetCheck(0);
	pB8->SetCheck(0);
	pB9->SetCheck(0);
	pB10->SetCheck(0);

	switch (Color) {
		case 0	: pB1->SetCheck(1); break;
		case 1	: pB2->SetCheck(1); break;
		case 2	: pB3->SetCheck(1); break;
		case 3	: pB4->SetCheck(1); break;
		case 4	: pB5->SetCheck(1); break;
		case 5	: pB6->SetCheck(1); break;
		case 6	: pB7->SetCheck(1); break;
		case 7	: pB8->SetCheck(1); break;
		case 8	: pB9->SetCheck(1); break;
		case 9	: pB10->SetCheck(1); break;
		default	: pB1->SetCheck(1); break;
	}

	
	pC=new MyEditCtrl(this);

	if (pMem){
		CString c;
		for (int i=0; i<pMem->GetNumberOfSimulations(); i++){
			char s[20];

			_itoa(-i, s, 10);
			c += CString(s)+ "\t";

			_gcvt(pMem->Getdr(-i), 5, s);
			c += CString(s)+"\t";

			_gcvt(pMem->Getdt(-i), 5, s);
			c += CString(s)+"\t";

			_gcvt(pMem->GetNorm(-i), 5, s);
			c += CString(s)+"";

			c += "\r\n";
		}
		pC->SetWindowText(c);

	}

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void RedrawDiag::OnOK() 
{
	// TODO: Add extra validation here
	CButton* pB1 = (CButton*)GetDlgItem(IDC_RADIO_GRAY);
    CButton* pB2 = (CButton*)GetDlgItem(IDC_RADIO_BLACK);
    CButton* pB3 = (CButton*)GetDlgItem(IDC_RADIO_RED);
    CButton* pB4 = (CButton*)GetDlgItem(IDC_RADIO_GREEN);
    CButton* pB5 = (CButton*)GetDlgItem(IDC_RADIO_BLUE);
    CButton* pB6 = (CButton*)GetDlgItem(IDC_RADIO_YELLOW);
    CButton* pB7 = (CButton*)GetDlgItem(IDC_RADIO_ORANGE);
    CButton* pB8 = (CButton*)GetDlgItem(IDC_RADIO_PINK);
    CButton* pB9 = (CButton*)GetDlgItem(IDC_RADIO_BROWN);
    CButton* pB10 = (CButton*)GetDlgItem(IDC_RADIO_LIGHTBLUE);

	if (pB1->GetCheck()) Color = 0;
	if (pB2->GetCheck()) Color = 1;
	if (pB3->GetCheck()) Color = 2;
	if (pB4->GetCheck()) Color = 3;
	if (pB5->GetCheck()) Color = 4;
	if (pB6->GetCheck()) Color = 5;
	if (pB7->GetCheck()) Color = 6;
	if (pB8->GetCheck()) Color = 7;
	if (pB9->GetCheck()) Color = 8;
	if (pB10->GetCheck()) Color = 9;

	
	CDialog::OnOK();
}
