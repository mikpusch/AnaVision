#ifndef MIK_ZOOM_BUTTON
#define MIK_ZOOM_BUTTON

class Traces;


class ZoomButton : public CButton
{
// Construction
public:
	ZoomButton();

// Attributes
public:
	int index;
	int ZoomFactor; // inverse
//	double DZoomFactor; // not used now
//	bool IsDoubleZoom;
	bool ZoomIn;
	BOOL Selected;

	Traces * parent;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RelSegButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ZoomButton();

	// Generated message map functions
	// Generated message map functions
protected:
	//{{AFX_MSG(RelSegButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


#endif
