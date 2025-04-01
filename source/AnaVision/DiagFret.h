#pragma once


// DiagFret dialog

class DiagFret : public CDialog
{
	DECLARE_DYNAMIC(DiagFret)

public:
	DiagFret(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagFret();
	Params p;

// Dialog Data
	enum { IDD = IDD_DIAG_FRET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
