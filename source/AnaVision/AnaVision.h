
// AnaVision.h : main header file for the ANAVISION application
//

#if !defined(AFX_ANAVISION_H__2368E044_B0EA_48A0_A739_C4EBD62F5507__INCLUDED_)
#define AFX_ANAVISION_H__2368E044_B0EA_48A0_A739_C4EBD62F5507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define uint64 unsigned __int64

#define UPDATEVIEW_MESSAGE	(WM_USER + 2)

#define MAX_SERIES_SELECTED 1000
#define MAX_POLYGON_SELECTED 14

#define SIMPLE_IMAGE_CORE  // Comment this out if not going to be used

//#define MAX_X_SIZEIMAGE	800
//#define MAX_Y_SIZEIMAGE	500


#define ImageDataType float
#define ImageDataTypeFloat

//#define ImageDataType int
//#define ImageDataTypeInt

//#define ImageDataType double
//#define ImageDataTypeDouble
#include "resource.h"       // main symbols
#include <math.h>

#include "../common/simplex.h"
#include "../common/praxis.h"
#include "../common/utils.h"
#include "../common/nrutil.h"
#include "../common/MTRand.h"
#include "../common/RungeKutta.h"
#include "../common/ListObject.h"
#include "Phasor.h"

class ImageData{
public:
	~ImageData();
	ImageDataType ** data;
	ImageDataType ** refdata;
	ImageDataType ** meanrefdata;
	bool refcalculated;
	int xsize, ysize;

	void Alloc(int xsize, int ysize);
	void DeAlloc();

	ImageDataType ** AllocOneImage(int xsize, int ysize);
	void DeallocOneImage (ImageDataType ** image, int xsize, int ysize);

};

/*
#define RectangleSection	"RectangleSection"
#define P1RectItemx			"P1RectItemx"
#define P2RectItemx			"P2RectItemx"
#define P3RectItemx			"P3RectItemx"
#define P4RectItemx			"P4RectItemx"
#define P1RectItemy			"P1RectItemy"
#define P2RectItemy			"P2RectItemy"
#define P3RectItemy			"P3RectItemy"
#define P4RectItemy			"P4RectItemy"

#define ConusSection			"ConusSection"
#define P1ConusItemx			"P1ConusItemx"
#define P2ConusItemx			"P2ConusItemx"
#define P3ConusItemx			"P3ConusItemx"
#define P4ConusItemx			"P4ConusItemx"
#define P1ConusItemy			"P1ConusItemy"
#define P2ConusItemy			"P2ConusItemy"
#define P3ConusItemy			"P3ConusItemy"
#define P4ConusItemy			"P4ConusItemy"
*/

#define AutoScaleSection	"AutoScaleSection"
#define AutoScaleItem		"AutoScaleItem"

#define NZOOMBUTTONS 4


#define ID_PREVIOUSFILE_BUTTON			10000
#define ID_NEXTFILE_BUTTON				10001
#define ID_FILENO_FIELD					10002
#define ID_FIRSTFILE_FIELD				10003
#define ID_LASTFILE_FIELD				10004
#define ID_DELTAIMAGE_FIELD				10005
#define ID_TEXT_DELTAIMAGE_FIELD		10006
#define ID_TEXT_MINSCALE_FIELD			10007
#define ID_TEXT_MAXSCALE_FIELD			10008
#define ID_RELOAD_BUTTON				10009
#define ID_COPYTHISTOGENERAL_BUTTON		10010
#define ID_COPYGENERALTOTHIS_BUTTON		10011
#define ID_ZOOM_BUTTON1					10012
#define ID_ZOOM_BUTTON2					10013
#define ID_ZOOM_BUTTON3					10014
#define ID_ZOOM_BUTTON4					10015

#define ID_OFFSET_X_PLUS_10_BUTTON		10016
#define ID_OFFSET_X_PLUS_1_BUTTON		10017
#define ID_OFFSET_X_MINUS_10_BUTTON		10018
#define ID_OFFSET_X_MINUS_1_BUTTON		10019
#define ID_OFFSET_Y_PLUS_10_BUTTON		10020
#define ID_OFFSET_Y_PLUS_1_BUTTON		10021
#define ID_OFFSET_Y_MINUS_10_BUTTON		10022
#define ID_OFFSET_Y_MINUS_1_BUTTON		10023

#define ID_OFFSET_X_TEXT				10024
#define ID_OFFSET_Y_TEXT				10025

#define ID_TRACK_THIS_BUTTON				10026

#define ID_OFFSET_X_PDCORIGIN				10027
#define ID_OFFSET_Y_PDCORIGIN				10028
#define ID_OFFSET_TEXT_X_PDCORIGIN			10029
#define ID_OFFSET_TEXT_Y_PDCORIGIN			10030

#define ID_ZOOM_BUTTON_REAL1					10031
#define ID_ZOOM_BUTTON_REAL2					10032
#define ID_ZOOM_BUTTON_REAL3					10033
#define ID_ZOOM_BUTTON_REAL4					10034
#define ID_RESET_BUTTON							10035
#define ID_AUTOSCALE_BUTTON						10036
#define ID_MINSCALE_FIELD						10037
#define ID_MINSCALE_TEXT_FIELD					10038
#define ID_MAXSCALE_FIELD						10039
#define ID_MAXSCALE_TEXT_FIELD					10040
#define ID_MIN_SLIDERCTRL						10041
#define ID_MAX_SLIDERCTRL						10042
#define ID_USE_PHASOR_BUTTON					10043
#define	ID_TEXT_LIFETIME_TXT					10044
#define	ID_TEXT_LIFETIME						10045
#define ID_USE_PHASOR_LOGSCALE					10046
#define ID_PHASOR_R_TEXT_FIELD			10047
#define ID_PHASOR_R_FIELD				10048
#define ID_PHASOR_TRANSPOSEREAD					10049
#define ID_PHASOR_NMIN_TEXT_FIELD				10050
#define ID_PHASOR_NMIN_FIELD					10051
#define ID_PHASOR_G_TEXT_FIELD			10052
#define ID_PHASOR_G_FIELD				10053
#define ID_PHASOR_S_TEXT_FIELD			10054
#define ID_PHASOR_S_FIELD				10055

#define ID_PHASOR_R2_TEXT_FIELD			10056
#define ID_PHASOR_R2_FIELD				10057
#define ID_PHASOR_G2_TEXT_FIELD			10058
#define ID_PHASOR_G2_FIELD				10059
#define ID_PHASOR_S2_TEXT_FIELD			10060
#define ID_PHASOR_S2_FIELD				10061
#define	ID_TEXT_LIFETIME2_TXT			10062
#define	ID_TEXT_LIFETIME2				10063
#define ID_USE_PHASOR_BUTTON2			10064
#define ID_TEXT_FRET_EFFICIENCY_TEXT_FIELD	10066
#define ID_TEXT_FRET_EFFICIENCY_FIELD	10067
#define ID_TEXT_PHASOR_FRACTION_TEXT_FIELD	10068
#define ID_TEXT_PHASOR_FRACTION_FIELD	10069
#define ID_SLIDER_PHASOR_FRACTION_SLIDER 10070
#define ID_SLIDER_FRET_EFFICIENCY_SLIDER 10071
#define ID_TEXT_PHASOR_FRET_EFFICIENCY_TEXT_FIELD	10072
#define ID_TEXT_PHASOR_FRET_EFFICIENCY_FIELD	10073
#define ID_COPY_PINK_PHASOR_BUTTON				10074
#define ID_PASTE_PINK_PHASOR_BUTTON				10075
#define ID_COPY_BLUE_PHASOR_BUTTON				10076
#define ID_PASTE_BLUE_PHASOR_BUTTON				10077
#define ID_FIT_PINK_PHASOR_BUTTON				10078
#define ID_FIT_BLUE_PHASOR_BUTTON				10079
#define ID_FIT_FRAC_PHASOR_BUTTON				10080
#define ID_PHASORPLOT_LINETHICKNESS				10081
#define ID_PHASORPLOT_LINETHICKNESS_TEXT		10082


#include "resource.h"       // main symbols

#include "../common/nrutil.h"
#include "../common/utils.h"
#include "../common/simplex.h"
#include "../common/DCompl.h"
#include "../BufferedDiffusion/Diffusion.h"

#if defined SIMPLE_IMAGE_CORE
	#include "../simpleImageCore/include/arivis/SimpleImageCore.h"
	#include "../simpleImageCore/include/arivis/SimpleImageStack.h"
#endif

#include "SimFitThread.h"

#include "Params.h"
#include "Kinetics.h"
#include "DataFile.h"
#include "MyEdit.h"
#include "DiagSimulFit.h"
#include "ParamsDiag.h"
#include "RatioDiag.h"
#include "DiagLinearFit.h"
#include "DiagSelectSeriesAndPolygons.h"
#include "DiagFitD.h"
#include "DiagKinetics.h"
#include "Button.h"
#include "Text.h"
#include "IODiag.h"
#include "GetFramesDiag.h"
#include "Tree.h"
#include "Traces.h"
#include "ZoomButton.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "AnaVisionDoc.h"
#include "AnaVisionView.h"


/////////////////////////////////////////////////////////////////////////////
// CAnaVisionApp:
// See AnaVision.cpp for the implementation of this class
//

class CAnaVisionApp : public CWinApp
{
public:
	CAnaVisionApp();
	bool MyLoadString(int & bytesavailable, CFile * fp, CString & value);
	void MySaveString(int & byteswritten, CFile * fp, CString & value);

	CDocument* OpenDocumentFile( LPCTSTR lpszFileName );

	Params p;

#if defined SIMPLE_IMAGE_CORE
	arivis::simpleimagecore::SimpleImageCore* Core;
#endif

//	ImageData id;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnaVisionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAnaVisionApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnMyFileOpen();
	afx_msg void MyOnFileNew();

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANAVISION_H__2368E044_B0EA_48A0_A739_C4EBD62F5507__INCLUDED_)
