#pragma once



// DiagFillParams dialog

class DiagFillParams : public CDialog
{
	DECLARE_DYNAMIC(DiagFillParams)

public:
	DiagFillParams(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagFillParams();

// Dialog Data
	enum { IDD = IDD_DIAG_FILLTHRESHOLD };

	BOOL UseGloabMean;
	double Threshold;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
