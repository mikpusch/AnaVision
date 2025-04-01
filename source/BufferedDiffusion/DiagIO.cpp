// DiagIO.cpp : implementation file
//

#include "stdafx.h"
#include "BufferedDiffusion.h"
//#include "DiagIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiagIO dialog


DiagIO::DiagIO(CWnd* pParent /*=NULL*/)
	: CDialog(DiagIO::IDD, pParent)
{
	//{{AFX_DATA_INIT(DiagIO)
	m_number = _T("");
	m_string = _T("");
	//}}AFX_DATA_INIT
}


void DiagIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DiagIO)
	DDX_Text(pDX, IDC_NUMBER, m_number);
	DDX_Text(pDX, IDC_STRING, m_string);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DiagIO, CDialog)
	//{{AFX_MSG_MAP(DiagIO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiagIO message handlers

BOOL DiagIO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GotoDlgCtrl( GetDlgItem(IDC_NUMBER) );
//	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	return FALSE;
}




int io::ReadString(){
	DiagIO dlg(AfxGetMainWnd());
//	CMainFrame* frame = (CMainFrame *)(AfxGetMainWnd( ));
//    ASSERT_VALID(frame);


	// Copy the most current data into the dialog box.
    dlg.m_string = lf;
	dlg.m_number = ls;

	// Display the dialog box.
	int result = dlg.DoModal();

	// If the user exited the dialog box with the OK button...
	if (result == IDOK)
	{
	    // Copy the dialog's data into this class's data members.
	   ls = dlg.m_number;
	 	   // Force the window to show the new data.
//        Invalidate();
   }

	return result;

}

int io::InInt(int & i, CString text){
	char s[20];
	_itoa(i, s, 10);
    CString c(s);
	ls = c;
	lf = text;
	int result = ReadString();
    i =  atoi(ls);
	return result;
}

void io::ShowDouble(double  value, CString text)
{
 char s1[20];
_gcvt( value, 10,  s1 );

MessageBox(NULL,  CString(s1), text, MB_OK);
}

int io::InDouble(double & i, CString text){
	char s[20];
	_gcvt( i, 10,  s );
    CString c(s);
	ls = c;
	lf = text;
	int result = ReadString();
    i=atof( ls );
	return result;
}

int io::InString(CString & i, CString text){
	ls = i;
	lf = text;
	int result = ReadString();
    i=ls;
	return result;
}
