// BufferedDiffusionDoc.h : interface of the CBufferedDiffusionDoc class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#if !defined(AFX_BUFFEREDDIFFUSIONDOC_H__593A52E9_06BB_44CD_B6BE_228226047C99__INCLUDED_)
#define AFX_BUFFEREDDIFFUSIONDOC_H__593A52E9_06BB_44CD_B6BE_228226047C99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Diffusion;

class Params{
public:
	Params();
	~Params();

	BOOL SaveOnFile(CFile * fp);
	BOOL LoadFromFile(CFile * fp);

	BOOL SaveOnFile2(CFile * fp);
	BOOL LoadFromFile2(CFile * fp);

	BOOL SaveOnFile3(CFile * fp);
	BOOL LoadFromFile3(CFile * fp);

	int MaxIter;
	int UpdateEvery;
	double RangepH;
	BOOL WriteOutFiles;
	CString OutFileName;

	BOOL ReadStartingFile;

	UINT NSecondsOut;

};


class CBufferedDiffusionDoc : public CDocument
{
protected: // create from serialization only
	CBufferedDiffusionDoc();
	DECLARE_DYNCREATE(CBufferedDiffusionDoc)

// Attributes
public:

	Diffusion d;
	Params	  p;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBufferedDiffusionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBufferedDiffusionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBufferedDiffusionDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUFFEREDDIFFUSIONDOC_H__593A52E9_06BB_44CD_B6BE_228226047C99__INCLUDED_)
