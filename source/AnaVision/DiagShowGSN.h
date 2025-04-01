#pragma once


// DiagShowGSN dialog

class DiagShowGSN : public CDialog
{
	DECLARE_DYNAMIC(DiagShowGSN)

public:
	DiagShowGSN(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagShowGSN();

// Dialog Data
	enum { IDD = ID_DIAG_SHOW_GSN };
	double G, S;
	int N;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
