#pragma once


// DiagEdgeDetect dialog

class DiagEdgeDetect : public CDialog
{
	DECLARE_DYNAMIC(DiagEdgeDetect)

public:
	DiagEdgeDetect(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagEdgeDetect();

// Dialog Data
	enum { IDD = IDD_DIALG_EDGE_DETECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float LowThreshold;
public:
	float HighThreshold;
public:
	UINT MaxDistPolyPoints;
public:
	int MaxPolyPoints;
public:
	float GaussianKernelRadius;
public:
	float gaussianKernelWidth;
};
