#pragma once


// DiagMembranePolys dialog

class DiagMembranePolys : public CDialog
{
	DECLARE_DYNAMIC(DiagMembranePolys)

public:
	DiagMembranePolys(CWnd* pParent = NULL);   // standard constructor
	virtual ~DiagMembranePolys();

// Dialog Data
	enum { IDD = IDD_DIAGMEMBRANEPOLYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_IsCircular;
	int m_MembraneSize;

};
