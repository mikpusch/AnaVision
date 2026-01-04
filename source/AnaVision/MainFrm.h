// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8297BB69_267D_4D31_8B51_B83995A96ABE__INCLUDED_)
#define AFX_MAINFRM_H__8297BB69_267D_4D31_8B51_B83995A96ABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

	Phasor PinkPhasor;
	double PinkPhasorRadius;
//	bool ReadPhasorFilesTransposed;
	Phasor PastedPhasor;
	double PastedPhasorRadius;

	Phasor PhasorCircle[4]; //0: pink, 1: blue, 2: background; 3: fret efficieny
	double PhasorCircleR[4];
	double LifeTime[2];
	bool ShowPinkCircle, ShowBlueCircle;
	bool UseLogScalePhasor;
	bool PhasorBackGroundDefined;
	double PhasorFraction;
	double PhasorPlotFretEfficiency;


	double Background;
	int MinSizeCellInPixles;

	int SizeCountCircle;
	int VesicleRimSize;
	double CutoffSDVesicleFit;
	unsigned int NPolyInside;
	unsigned int MaxRadiusVesicles;

	double PercentageAcceptanceFill;
	BOOL UseGlobalMeanForFill;
	ArrayOfPolygons CopiedPolygons;
	ArrayOfPolygons CopiedLines;

	CBrush * RedBrush, * BlueBrush;

	bool FirstFont;
	CFont SeqFont;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8297BB69_267D_4D31_8B51_B83995A96ABE__INCLUDED_)
