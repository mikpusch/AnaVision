// AnaVisionDoc.h : interface of the CAnaVisionDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANAVISIONDOC_H__D71B0DCA_F434_45D0_9217_D8D652233DF6__INCLUDED_)
#define AFX_ANAVISIONDOC_H__D71B0DCA_F434_45D0_9217_D8D652233DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define MAX_X_SIZEIMAGE	1500
//#define MAX_Y_SIZEIMAGE	1500

#define MAX_X_SIZEIMAGE	800
#define MAX_Y_SIZEIMAGE	500


class CheckLoadingFile{
public:
	CheckLoadingFile(bool * pLoadCheck);
	~CheckLoadingFile();
private:
	bool * pLOADCHECK;

};

class CAnaVisionDoc : public CDocument
{
protected: // create from serialization only
	CAnaVisionDoc();
	DECLARE_DYNCREATE(CAnaVisionDoc)

// Attributes
public:


	vector < vector < bool>> WhiteMask[2];
	bool UseCurrentlySelectedPolygonForPasorPlot;
	bool UsePhasorMask[2];
	bool OpenTif(CString Name);
	bool OpenTifZStack(CString Name);
	bool OpenStandardTif(CString Name);
	bool Openanavisionformat(CString FileName);

	void UpdatePolygonDataPoints();


	DataFile dat;
	bool IsLoadingFile;

//	double ** currentimage;

	void OnAnalyzeFret();

	ImageData id;
	CellsClass * pActualCells;
	CellsClass DummyCells;
	bool DataPresent;

	double * time;					// in sec
	double * CoulombsProtonAdded;
	int NTimeData;
	int AllocTimeData;
	bool ReadFile(int index);
	void CalculateRef();
	void GetOffset(int & offsetx, int & offsety);
	void IncOffset(int offsetx, int offsety);
	void ClearAllOffsets();


// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnaVisionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnaVisionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void SaveFlimFretProject(bool ask);
//	void LoadFlimFretProject(bool ask);
	void DoLoadFlimFretProject(CFile & file);
	void DoLoad(CFile * fp);

private:

	void DeAlloc();
	void Alloc();
	void AllocTime();
	void DeAllocTime();


	bool DoReadFile(FILE * f);
	void DoFileSave(bool ask);

// Generated message map functions
protected:
	//{{AFX_MSG(CAnaVisionDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANAVISIONDOC_H__D71B0DCA_F434_45D0_9217_D8D652233DF6__INCLUDED_)
