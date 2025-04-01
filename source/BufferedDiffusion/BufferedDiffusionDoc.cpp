// BufferedDiffusionDoc.cpp : implementation of the CBufferedDiffusionDoc class
//

#include "stdafx.h"
#include "BufferedDiffusion.h"

//#include "BufferedDiffusionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionDoc

IMPLEMENT_DYNCREATE(CBufferedDiffusionDoc, CDocument)

BEGIN_MESSAGE_MAP(CBufferedDiffusionDoc, CDocument)
	//{{AFX_MSG_MAP(CBufferedDiffusionDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionDoc construction/destruction

CBufferedDiffusionDoc::CBufferedDiffusionDoc()
{
	// TODO: add one-time construction code here

}

CBufferedDiffusionDoc::~CBufferedDiffusionDoc()
{
}

BOOL CBufferedDiffusionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionDoc serialization

void CBufferedDiffusionDoc::Serialize(CArchive& ar)
{
	CFile * fp  = ar.GetFile();
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		if (!p.SaveOnFile(fp)){
			Alert0("Error saving parameters");
		}
		if (!d.SaveOnFile(fp)){
			Alert0("Error saving diffusion parameters");
		}
		if (!p.SaveOnFile2(fp)){
			Alert0("Error saving parameters");
		}
		if (!d.SaveOnFile2(fp)){
			Alert0("Error saving diffusion parameters");
		}
		if (!p.SaveOnFile3(fp)){
			Alert0("Error saving parameters");
		}
		if (!d.SaveOnFile3(fp)){
			Alert0("Error saving diffusion parameters");
		}
	}
	else
	{
		// TODO: add loading code here
		if (!p.LoadFromFile(fp)){
			Alert0("Error loading parameters");
		}
		if (!d.LoadFromFile(fp)){
			Alert0("Error  loadingdiffusion parameters");
		}
		if (!p.LoadFromFile2(fp)){
			return;
//			Alert0("Error loading parameters");
		}
		if (!d.LoadFromFile2(fp)){
			return;
//			Alert0("Error loading diffusion parameters");
		}
		if (!p.LoadFromFile3(fp)){
			return;
//			Alert0("Error loading parameters");
		}
		if (!d.LoadFromFile3(fp)){
			return;
//			Alert0("Error loading diffusion parameters");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionDoc diagnostics

#ifdef _DEBUG
void CBufferedDiffusionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBufferedDiffusionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionDoc commands


Params::Params(){
	MaxIter = 100000;
	UpdateEvery = 1000;
	WriteOutFiles = FALSE;
	OutFileName="c:/temp/diffusion";

	MaxIter = 1000;
	UpdateEvery = 1;
	WriteOutFiles = FALSE;
	OutFileName="c:/temp/diffusion";

	RangepH = 5;

	ReadStartingFile = FALSE;
	NSecondsOut = 0;

}

Params::~Params(){
}

BOOL Params::SaveOnFile(CFile * fp){
	WriByNS(MaxIter);
	WriByNS(UpdateEvery);
	WriByNS(WriteOutFiles);
	SaveString(OutFileName, *fp);

	return TRUE;
}
BOOL Params::LoadFromFile(CFile * fp){
	ReByNS(MaxIter);
	ReByNS(UpdateEvery);
	ReByTRUENS(WriteOutFiles);
//	return TRUE;
	if (!LoadString(OutFileName, *fp)){
		return TRUE;
	}

	return TRUE;
}

BOOL Params::SaveOnFile2(CFile * fp){
	WriByNS(RangepH);

	return TRUE;
}
BOOL Params::LoadFromFile2(CFile * fp){
	ReByTRUENS(RangepH);

//	RangepH = 100;
	return TRUE;
}
BOOL Params::SaveOnFile3(CFile * fp){

	WriByNS(ReadStartingFile);
	WriByNS(NSecondsOut);
	return TRUE;
}
BOOL Params::LoadFromFile3(CFile * fp){

	ReByTRUENS(ReadStartingFile);
	ReByTRUENS(NSecondsOut);
//	RangepH = 100;
	return TRUE;
}


