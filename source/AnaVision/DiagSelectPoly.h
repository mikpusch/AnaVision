#pragma once


// DiagSelectPoly dialog

class DiagSelectPoly : public CDialog
{
	DECLARE_DYNAMIC(DiagSelectPoly)

public:
	DiagSelectPoly(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagSelectPoly();

	int NValidPolys;
	int selected;
	int GetID(int i);
// Dialog Data
	enum { IDD = IDD_DIAGSELECT_POLY_BY_INDEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
