#pragma once
#include "afxcmn.h"


// DiagSelectSeriesAndPolygons dialog

class DiagSelectSeriesAndPolygons : public CDialog
{
	DECLARE_DYNAMIC(DiagSelectSeriesAndPolygons)

public:
	DiagSelectSeriesAndPolygons(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagSelectSeriesAndPolygons();

// Dialog Data
	enum { IDD = IDD_ANALYZE_ROIS };

	BOOL SeriesSelected[MAX_SERIES_SELECTED];
	BOOL PolygonSelected[MAX_POLYGON_SELECTED];
	int BackgroundPolygon;
	BOOL ConcatenateImages;
	BOOL UseSeries; // Else use imaegs of 1 series
	int NSeries;
	int NPoly;

	CString SeriesNames[MAX_SERIES_SELECTED];
	void SetCheck(int ID, int value);
	void GetCheck(int ID, int value);
	void SetText(int ID, int index);
	void Inactivate(int ID);
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CListCtrl ListCtrl;
	BOOL m_ClosedLoop;
	int m_SizeMembraneInPixels;
	int m_NumberOfGradientPoints;
};
