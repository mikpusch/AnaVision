// DiagSelectSeriesAndPolygons.cpp : implementation file
//

#include "stdafx.h"
#include "AnaVision.h"
#include "DiagSelectSeriesAndPolygons.h"
//#include "DiagSelectSeriesAndPolygons.h"


// DiagSelectSeriesAndPolygons dialog

IMPLEMENT_DYNAMIC(DiagSelectSeriesAndPolygons, CDialog)

DiagSelectSeriesAndPolygons::DiagSelectSeriesAndPolygons(CWnd* pParent /*=NULL*/)
	: CDialog(DiagSelectSeriesAndPolygons::IDD, pParent)
	, m_ClosedLoop(false)
	, m_SizeMembraneInPixels(0)
{
	NSeries = 0;
	NPoly = 0;
	UseSeries = true;

}

DiagSelectSeriesAndPolygons::~DiagSelectSeriesAndPolygons()
{
}

void DiagSelectSeriesAndPolygons::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_POLYGON0, PolygonSelected[0]);
	DDX_Check(pDX, IDC_POLYGON1, PolygonSelected[1]);
	DDX_Check(pDX, IDC_POLYGON2, PolygonSelected[2]);
	DDX_Check(pDX, IDC_POLYGON3, PolygonSelected[3]);
	DDX_Check(pDX, IDC_POLYGON4, PolygonSelected[4]);
	DDX_Check(pDX, IDC_POLYGON5, PolygonSelected[5]);
	DDX_Check(pDX, IDC_POLYGON6, PolygonSelected[6]);
	DDX_Check(pDX, IDC_POLYGON7, PolygonSelected[7]);
	DDX_Check(pDX, IDC_POLYGON8, PolygonSelected[8]);
	DDX_Check(pDX, IDC_POLYGON9, PolygonSelected[9]);
	DDX_Check(pDX, IDC_POLYGON10, PolygonSelected[10]);
	DDX_Check(pDX, IDC_POLYGON11, PolygonSelected[11]);
	DDX_Check(pDX, IDC_POLYGON12, PolygonSelected[12]);
	DDX_Check(pDX, IDC_POLYGON13, PolygonSelected[13]);

	DDX_Check(pDX, IDC_CONCAT_IMAGES, ConcatenateImages);
	DDX_Check(pDX, IDC_ROIS_ARE_CIRCULAR, this->m_ClosedLoop);

	DDX_Text(pDX, IDC_SIZE_MEMBRANE, this->m_SizeMembraneInPixels);
	DDX_Text(pDX, IDC_NUMBER_GRADIENTPOINTS, this->m_NumberOfGradientPoints);

	
	DDX_Control(pDX, IDC_LIST_SERIES, ListCtrl);
}


BEGIN_MESSAGE_MAP(DiagSelectSeriesAndPolygons, CDialog)
END_MESSAGE_MAP()


// DiagSelectSeriesAndPolygons message handlers

void DiagSelectSeriesAndPolygons::SetCheck(int ID, int value){
    CButton* pB = (CButton*)GetDlgItem(ID);
	if (BackgroundPolygon==value){
		pB->SetCheck(1);
	}
	else{
		pB->SetCheck(0);
	}
}
void DiagSelectSeriesAndPolygons::GetCheck(int ID, int value){
    CButton* pB = (CButton*)GetDlgItem(ID);
	if (pB->GetCheck()){
		BackgroundPolygon=value;
	}
}


void DiagSelectSeriesAndPolygons::SetText(int ID, int index){
	CEdit * pE = (CEdit *)GetDlgItem(ID);
	pE->SetWindowTextA(SeriesNames[index]);
	
}
void DiagSelectSeriesAndPolygons::Inactivate(int ID){
    CButton* pB = (CButton*)GetDlgItem(ID);
	pB->EnableWindow(false);
}
	


BOOL DiagSelectSeriesAndPolygons::OnInitDialog()
{
	CDialog::OnInitDialog();
	ListCtrl.InsertColumn(0, _T("Item Name"), LVCFMT_LEFT, 200);
	ListCtrl.SetExtendedStyle(LVS_EX_CHECKBOXES);

	LVITEM lvi;
	CString strItem;
//	ShowFloat(NSeries, "NS");
	for (int i = 0; i < NSeries; i++)
	{
// Insert the first item
		lvi.mask =  LVIF_TEXT;
		if (UseSeries){
			strItem = "Series ";
		}
		else{
			strItem = "Images ";
		}
		BOOL first = true;
		AddIntToString(first, i, strItem);
		if (UseSeries){
			strItem += CString("  ")+SeriesNames[i];
		}
		else{
		}
//		strItem.Format(_T("Series %i")+SeriesNames[i], i);
//		strItem += SeriesNames[i];
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		ListCtrl.InsertItem(&lvi);
	}

	if (NPoly<14){
		Inactivate(IDC_RADDIO_BACK_13);
		Inactivate(IDC_POLYGON13);
	}
	if (NPoly<13){
		Inactivate(IDC_RADDIO_BACK_12);
		Inactivate(IDC_POLYGON12);
	}
	if (NPoly<12){
		Inactivate(IDC_RADDIO_BACK_11);
		Inactivate(IDC_POLYGON11);
	}
	if (NPoly<11){
		Inactivate(IDC_RADDIO_BACK_10);
		Inactivate(IDC_POLYGON10);
	}
	if (NPoly<10){
		Inactivate(IDC_RADDIO_BACK_9);
		Inactivate(IDC_POLYGON9);
	}
	if (NPoly<9){
		Inactivate(IDC_RADDIO_BACK_8);
		Inactivate(IDC_POLYGON8);
	}
	if (NPoly<8){
		Inactivate(IDC_RADDIO_BACK_7);
		Inactivate(IDC_POLYGON7);
	}
	if (NPoly<7){
		Inactivate(IDC_RADDIO_BACK_6);
		Inactivate(IDC_POLYGON6);
	}
	if (NPoly<6){
		Inactivate(IDC_RADDIO_BACK_5);
		Inactivate(IDC_POLYGON5);
	}
	if (NPoly<5){
		Inactivate(IDC_RADDIO_BACK_4);
		Inactivate(IDC_POLYGON4);
	}
	if (NPoly<4){
		Inactivate(IDC_RADDIO_BACK_3);
		Inactivate(IDC_POLYGON3);
	}
	if (NPoly<3){
		Inactivate(IDC_RADDIO_BACK_2);
		Inactivate(IDC_POLYGON2);
	}
	if (NPoly<2){
		Inactivate(IDC_RADDIO_BACK_1);
		Inactivate(IDC_POLYGON1);
	}
	if (NPoly<1){
		Inactivate(IDC_RADDIO_BACK_0);
		Inactivate(IDC_POLYGON0);
	}
	if (BackgroundPolygon>=NPoly){
		BackgroundPolygon = -1;
	}
	for (int i=NSeries; i<MAX_SERIES_SELECTED; i++){
		SeriesSelected[i]=false;
	}



	SetCheck(IDC_RADDIO_BACK_NOBACK, -1);
	SetCheck(IDC_RADDIO_BACK_0, 0);
	SetCheck(IDC_RADDIO_BACK_1, 1);
	SetCheck(IDC_RADDIO_BACK_2, 2);
	SetCheck(IDC_RADDIO_BACK_3, 3);
	SetCheck(IDC_RADDIO_BACK_4, 4);
	SetCheck(IDC_RADDIO_BACK_5, 5);
	SetCheck(IDC_RADDIO_BACK_6, 6);
	SetCheck(IDC_RADDIO_BACK_7, 7);
	SetCheck(IDC_RADDIO_BACK_8, 8);
	SetCheck(IDC_RADDIO_BACK_9, 9);
	SetCheck(IDC_RADDIO_BACK_10, 10);
	SetCheck(IDC_RADDIO_BACK_11, 11);
	SetCheck(IDC_RADDIO_BACK_12, 12);
	SetCheck(IDC_RADDIO_BACK_13, 13);

	for (int i=0; i<NSeries; i++){
//		ListCtrl.SetCheck(SeriesSelected[i]);
		if (SeriesSelected[i]){
			ListCtrl.SetCheck(i);
//			ShowFloat(i, "i");
		}
	}

	// TODO:  Add extra initialization here
;

	return true;  // return true unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return false
}

void DiagSelectSeriesAndPolygons::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();

	GetCheck(IDC_RADDIO_BACK_NOBACK, -1);
	GetCheck(IDC_RADDIO_BACK_0, 0);
	GetCheck(IDC_RADDIO_BACK_1, 1);
	GetCheck(IDC_RADDIO_BACK_2, 2);
	GetCheck(IDC_RADDIO_BACK_3, 3);
	GetCheck(IDC_RADDIO_BACK_4, 4);
	GetCheck(IDC_RADDIO_BACK_5, 5);
	GetCheck(IDC_RADDIO_BACK_6, 6);
	GetCheck(IDC_RADDIO_BACK_7, 7);
	GetCheck(IDC_RADDIO_BACK_8, 8);
	GetCheck(IDC_RADDIO_BACK_9, 9);
	GetCheck(IDC_RADDIO_BACK_10, 10);
	GetCheck(IDC_RADDIO_BACK_11, 11);
	GetCheck(IDC_RADDIO_BACK_12, 12);
	GetCheck(IDC_RADDIO_BACK_13, 13);

	for (int i=0; i<NSeries; i++){
		SeriesSelected[i] = ListCtrl.GetCheck(i);
		if (SeriesSelected[i]){
//			ShowFloat(i, "i");
		}
	}

}
