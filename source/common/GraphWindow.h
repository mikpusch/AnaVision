#pragma once

/////////////////////////////////////////////////////////////////////////////
// GraphWnd window
#define ID_GRAPHMENU_HELLO				143256
#define ID_AXISMENU_AUTOSCALE			(ID_GRAPHMENU_HELLO				+ 1)
#define ID_AXISMENU_SHOWTITLE			(ID_AXISMENU_AUTOSCALE			+ 1)
#define ID_AXISMENU_LOGSCALE			(ID_AXISMENU_SHOWTITLE			+ 1)
#define ID_AXISMENU_SHOWTICKVALUES		(ID_AXISMENU_LOGSCALE			+ 1)
#define ID_AXISMENU_NTICKS				(ID_AXISMENU_SHOWTICKVALUES		+ 1)
#define ID_MINX_TEXT_BOX				(ID_AXISMENU_NTICKS				+ 1)
#define ID_MAXX_TEXT_BOX				(ID_MINX_TEXT_BOX				+ 1)
#define ID_MINY_TEXT_BOX				(ID_MAXX_TEXT_BOX				+ 1)
#define ID_MAXY_TEXT_BOX				(ID_MINY_TEXT_BOX				+ 1)
#define ID_GRAPHMENU_EXPORTCLIPBOARD	(ID_MAXY_TEXT_BOX				+ 1)
#define ID_GRAPHMENU_EXPORTFILE			(ID_GRAPHMENU_EXPORTCLIPBOARD	+ 1)
#define ID_GRAPHMENU_EXPORTGEPULSEFILE	(ID_GRAPHMENU_EXPORTFILE		+ 1)
#define ID_GRAPHMENU_INTERRUPTBUTTON	(ID_GRAPHMENU_EXPORTGEPULSEFILE	+ 1)

//#define GRAPH_WINDOW_DATA_TYPE double
#define GRAPH_WINDOW_DATA_TYPE float


class Text;
class Group; // each graph may be stored as GePulse file; this has to be done by the user;
class GeStimulation;
class GeSeries;

class GraphAxis{
public:
	GraphAxis();
	~GraphAxis();

	int type; // 0:x, 1:y
	bool Autoscale;
	GRAPH_WINDOW_DATA_TYPE DrawMinValue, DrawMaxValue;
	GRAPH_WINDOW_DATA_TYPE DataMinValue, DataMaxValue;
	GRAPH_WINDOW_DATA_TYPE ManualMinValue, ManualMaxValue;

	bool logscale; // else linear
	unsigned __int32 NTicks;
	CString Title;
	bool ShowTitle;
	CString Unit;
	bool ShowUnit;
//	CFont VertFont;

	bool ShowTickValues;

	int Minxy, Maxxy;
	int Minyx, Maxyx;
	int ScaleDataValue(GRAPH_WINDOW_DATA_TYPE data);
	GRAPH_WINDOW_DATA_TYPE GetDataValue(int screendata);

	void Draw(CDC * pDC);

	bool PointIsCloseToAxis(CPoint p);

	int ScaleDataPoint(GRAPH_WINDOW_DATA_TYPE d);

	CPen *GrayPen;

// to be supplied by the user!!

	Text * MinBox;
	Text * MaxBox;

	CFont Vert;
	bool FirstTime;
};

class DataPointClass{
public:
	GRAPH_WINDOW_DATA_TYPE data[3];

};

class MyPlotClass{
public:
	MyPlotClass();
	~MyPlotClass();

	static void Cleanup(); // 

	static CPen  *** MyPens; //[4][256];
	static CPen  *** MyThickPens; //[4][256];
	static COLORREF  ** MyColors; //[4][256];
	static bool PensCreated;

//	RGB ColorOfPlot;

public:
	vector<DataPointClass> data; // 0: x, 1: y; 2: error bar

	int m_typeplot; // 0: lines; 1: symbols; 2: symbols and error bar
	int m_symboltype; // 0: circle; 1: square; 2: triangle
	int m_color; // 0 : black; 1: red; 2: green; 3: blue
	int m_intensity_color; //0...255;
	CPen * pm_Pen;
	CPen * pm_ThickPen;
	CString m_OverMouseText;
	
//	CToolTipCtrl * pm_ToolTip;

	void InitPen(); // Creates the pens according to color and intensity color

};



class GraphWnd : public CWnd

//class GraphWnd
{
// Construction
public:
	GraphWnd(CWnd * owner, CRect r);
public:
	void SetAxisTitle(int axis, CString title); // 0: x, 1: y
	
	CMenu m_FileMenu;

	bool ShowWaitCursor;
	bool ShowLastValue;

	bool MouseIsOverLine;
	int OverLineIndex;
	bool CheckIfToExitMouseOverLine(CPoint point);
	bool CheckIfToEnterMouseOverLine(CPoint point);
	void GetSmallLineCoordinates(int lineindex, int & x1, int & x2, int & y);
	void GetMouseOverRect(int lineindex, CRect & r);


	CString Title;
	bool ShowTitle;
	CButton * InterruptButton;
	bool InterruptClicked;
	bool ShowInterruptButton;
	bool FirstTime;
	void CreateButtons();
	void MoveButtons();

	Group * pGroup;
//	GeSeries * series;
	
	GraphAxis * xaxis;
	GraphAxis * yaxis;
	GraphAxis * selectedAxis;

	Text * MinXTextBox;
	Text * MaxXTextBox;
	
	Text * MinYTextBox;
	Text * MaxYTextBox;

	CPen * Dotted, * SolidBlack, * BluePen, * RedPen;
	CPen * GreenPen, *YellowPen, *GrayPen; 

//	bool IsCalculating;

	vector<MyPlotClass> plots;

	void OnMenuAxis(GraphAxis * axis, UINT nFlags, CPoint point);

	void ClearPlots();
	void AddPlot(int typeplot, int symboltype, int color, int ColorIntensity = 255,
					CString OverMouseText = "");
	void AddPoint(int plotnumber, GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y);
	void AddPoint(int plotnumber, GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y, GRAPH_WINDOW_DATA_TYPE error);
	void AddPointToLastPlot(GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y);
	void AddPointToLastPlot(GRAPH_WINDOW_DATA_TYPE x, GRAPH_WINDOW_DATA_TYPE y, GRAPH_WINDOW_DATA_TYPE error);

	void GetExportString(CString & c);

	void AddSweep(			int NChannels,
							bool NewSeries,
							int sweepcount,
							GeStimulation * pStim,
							bool WillHaveLeak,
							double VHold);
	void FillSeries();
//	void MyEnhancedPolyLine(vector<CPoint> & points, CDC *pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TraceDrawWnd)
	protected:
		void CalcMinAndMaxValues();

	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GraphWnd();

	// Generated message map functions
protected:
public:
	//{{AFX_MSG(GraphWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAxisMenuShowTitle();
	afx_msg void OnAxisAutoScale();
	afx_msg void OnAxisLogScale();
	afx_msg void OnAxisTickValues();
	afx_msg void OnAxisNTicks();
	afx_msg void OnXAxisMinChanged();
	afx_msg void OnXAxisMaxChanged();
	afx_msg void OnYAxisMinChanged();
	afx_msg void OnYAxisMaxChanged();
	afx_msg void OnExportToFile();
	afx_msg void OnExportToClipboard();
	afx_msg void OnExportGepulse();
	afx_msg void OnClickInterruptButton();


	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
