// AnaVisionDoc.cpp : implementation of the CAnaVisionDoc class
//

#include "stdafx.h"
#include "AnaVision.h"
#include "ChildFrm.h"
//#include <windows.h>
//#include "AnaVisionDoc.h"
//#include <stdio.h>
//#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionDoc

IMPLEMENT_DYNCREATE(CAnaVisionDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnaVisionDoc, CDocument)
	//{{AFX_MSG_MAP(CAnaVisionDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionDoc construction/destruction

CheckLoadingFile::CheckLoadingFile(bool * pLoadCheck){
	pLOADCHECK = pLoadCheck;
	if (pLOADCHECK){
		*pLOADCHECK = true;
	}
}
CheckLoadingFile::~CheckLoadingFile(){
	if (pLOADCHECK){
		*pLOADCHECK = false;
	}
}


CAnaVisionDoc::CAnaVisionDoc()
{
	// TODO: add one-time construction code here
//	currentimage = NULL;
/*
	Cabinet::CCompress        i_Compress;
	Cabinet::CExtract         i_ExtrDecrypt;
	Cabinet::CExtractResource i_ExtrResource;
*/


	UsePhasorMask[0] = UsePhasorMask[1] = false;
	DataPresent = false;
	IsLoadingFile = false;
	UseCurrentlySelectedPolygonForPasorPlot = false;

	this->pActualCells = NULL;
	id.data = NULL;
	id.refdata = NULL;
	id.meanrefdata = NULL;
	id.refcalculated = false;


	AllocTimeData = 1000;
	time = new double [AllocTimeData];
	CoulombsProtonAdded= new double [AllocTimeData];
	NTimeData = 0;


//	FileMode = 0; // ASCII

}

void CAnaVisionDoc::AllocTime(){
	if (NTimeData<AllocTimeData){
		return;
	}
	DeAllocTime();
	AllocTimeData = NTimeData;
	time = new double [AllocTimeData];
	CoulombsProtonAdded = new double [AllocTimeData];
}

void CAnaVisionDoc::DeAllocTime(){
	delete [] time;
	delete [] CoulombsProtonAdded;
}

void CAnaVisionDoc::Alloc(){

	DataPresent = true;
	int xsize = dat.p.xsizeimage;
	int ysize = dat.p.ysizeimage;
//	ShowFloat(xsize, "xsize");
//	ShowFloat(ysize, "ysize");

	id.Alloc(xsize, ysize);


}

void CAnaVisionDoc::DeAlloc(){

//	id.DeAlloc();

}

CAnaVisionDoc::~CAnaVisionDoc()
{
	DeAlloc();
	DeAllocTime();
}

BOOL CAnaVisionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return false;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionDoc serialization

bool CAnaVisionDoc::OpenTif(CString FileName){

	CheckLoadingFile CheckFile(&IsLoadingFile);

	CWaitCursor dummy;

	DeAlloc();
	dat.AnaTif.ImageOrderMethod = ORDER_BY_TIME;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	dat.p = pApp->p; // Use the previously active params as default for a newly opened file


//	if (dat.DoOpenTifFile(FileName)){
	if (dat.AssignFileNamesAndIndexes(FileName, NULL)){


		if (dat.p.xsizeimage<1){
			Alert0("Error : xsizeimage <1 ");
			return false;
		}

		if (dat.p.ysizeimage<1){
			dat.p.xsizeimage = dat.p.ysizeimage = 0;
			Alert0("Error : ysizeimage <1 ");
			return false;
		}
	
		Alloc();

			
		this->NTimeData = (dat.images[dat.ImageIndex].LastIndex - dat.images[dat.ImageIndex].FirstIndex + 1);
//			ShowFloat(this->NTimeData, "this->NTimeData");
		this->AllocTime();
//			ShowFloat(NTimeData, "NTime");
//			ReadFile(dat.images[dat.ImageIndex].CurrentIndex);
		this->m_strPathName = FileName;
		return true;
	}
	else{
		Alert0("Error in reading file - open tif");
		return false;
	}
}
bool CAnaVisionDoc::OpenTifZStack(CString FileName){

	CheckLoadingFile CheckFile(&IsLoadingFile);

	CWaitCursor dummy;

	DeAlloc();

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	dat.p = pApp->p; // Use the previously active params as default for a newly opened file

	dat.AnaTif.ImageOrderMethod = ORDER_BY_ZSTACK;

//	if (dat.DoOpenTifFile(FileName)){
	if (dat.AssignFileNamesAndIndexes(FileName, NULL)){


		if (dat.p.xsizeimage<1){
			Alert0("Error : xsizeimage <1 ");
			return false;
		}

		if (dat.p.ysizeimage<1){
			dat.p.xsizeimage = dat.p.ysizeimage = 0;
			Alert0("Error : ysizeimage <1 ");
			return false;
		}
		Alloc();
			
		this->NTimeData = (dat.images[dat.ImageIndex].LastIndex - dat.images[dat.ImageIndex].FirstIndex + 1);
//			ShowFloat(this->NTimeData, "this->NTimeData");
		this->AllocTime();
//			ShowFloat(NTimeData, "NTime");
//			ReadFile(dat.images[dat.ImageIndex].CurrentIndex);
		this->m_strPathName = FileName;
		return true;
	}
	else{
		Alert0("Error in reading file");
		return false;
	}
}
void CAnaVisionDoc::DoLoad(CFile * fp){
	CWaitCursor dummy;
	CheckLoadingFile CheckFile(&IsLoadingFile);
	DeAlloc();

//	Alert0("hi1");
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	dat.p = pApp->p; // Use the previously active params as default for a newly opened file

	CString Name = fp->GetFilePath( );
/*		CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd( ));
		CChildFrame * pChild = (CChildFrame *)frame->MDIGetActive();
		if (pChild->DoOpenAnvisionFormat(Name)){
			return;
		}
*/
	if (dat.AssignFileNamesAndIndexes(Name, fp)){
		if (dat.p.xsizeimage<1){
				Alert0("Error : xsizeimage <1 ");
				return;
			}

			if (dat.p.ysizeimage<1){
				dat.p.xsizeimage = dat.p.ysizeimage = 0;
				Alert0("Error : ysizeimage <1 ");
				return;
			}
			Alloc();
			//ShowFloat(dat.p.xsizeimage, "dat.p.xsizeimage");

			//ShowFloat(id.xsize, "id . xsixe in pdoc.dat.ass");
			
			this->NTimeData = (dat.images[dat.ImageIndex].LastIndex - dat.images[dat.ImageIndex].FirstIndex + 1);
//			ShowFloat(this->NTimeData, "this->NTimeData");
			this->AllocTime();
//			ShowFloat(NTimeData, "NTime");
//			ReadFile(dat.images[dat.ImageIndex].CurrentIndex);
		}
		else{
			Alert0("Error in reading file");
		}
	//Alert0("hi2");
		dat.BytesReadFromFile = fp->GetLength();
}

void CAnaVisionDoc::Serialize(CArchive& ar)
{
	CWaitCursor dummy;

	CFile* fp = ar.GetFile();

	if (ar.IsStoring())
	{
		dat.SaveOnFile(*fp);
		//Alert0("hi");
	}
	else
	{
		DoLoad(fp);

/*		CheckLoadingFile CheckFile(&IsLoadingFile);
		DeAlloc();

		CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
		dat.p = pApp->p; // Use the previously active params as default for a newly opened file

		CString Name = fp->GetFilePath( );
		if (dat.AssignFileNamesAndIndexes(Name, fp)){

			if (dat.p.xsizeimage<1){
				Alert0("Error : xsizeimage <1 ");
				return;
			}

			if (dat.p.ysizeimage<1){
				dat.p.xsizeimage = dat.p.ysizeimage = 0;
				Alert0("Error : ysizeimage <1 ");
				return;
			}
			Alloc();
			//ShowFloat(dat.p.xsizeimage, "dat.p.xsizeimage");

			//ShowFloat(id.xsize, "id . xsixe in pdoc.dat.ass");
			
			this->NTimeData = (dat.images[dat.ImageIndex].LastIndex - dat.images[dat.ImageIndex].FirstIndex + 1);
//			ShowFloat(this->NTimeData, "this->NTimeData");
			this->AllocTime();
//			ShowFloat(NTimeData, "NTime");
//			ReadFile(dat.images[dat.ImageIndex].CurrentIndex);
		}
		else{
			Alert0("Error in reading file");
		}
		dat.BytesReadFromFile = fp->GetLength();
//		Alert0("end read");
*/
	}

}


bool CAnaVisionDoc::ReadFile(int index){
//	bool result = dat.ReadImage(index, currentimage);;
//	if (!result){
//		Alert0("error reading file");
//	}
//	return dat.ReadImage(index, currentimage);
//	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	//ShowFloat(index, "index");
	return dat.ReadImage(index, id, pActualCells, false);

}
void CAnaVisionDoc::CalculateRef(){

//	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
//	ImageData & id = pApp->id;

	int nref=0;
	ImageDataType ** d = id.meanrefdata;
	ImageDataType ** r = id.refdata;
	int i;
	{
		for (int ix=0; ix<id.xsize; ix++){
			for (int iy=0; iy<id.ysize; iy++){
				d[ix][iy] = 0.0;
			}
		} 
	}
	for (i=dat.p.InitialFrame0; i<=dat.p.InitialFrame1; i++){
		if (!dat.ReadImage(i, id, pActualCells, true)){
			Alert0(" could not read file");
			return;	
		}
		for (int ix=0; ix<id.xsize; ix++){
			for (int iy=0; iy<id.ysize; iy++){
				d[ix][iy] += r[ix][iy] ;
			}
		}
		nref++;
	}
	if (nref<2) return;
	float xx = nref;
	for (int ix=0; ix<id.xsize; ix++){
		for (int iy=0; iy<id.ysize; iy++){
			d[ix][iy] /= xx;
		}
	}


}


/////////////////////////////////////////////////////////////////////////////
// CAnaVisionDoc diagnostics

#ifdef _DEBUG
void CAnaVisionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnaVisionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionDoc commands


void CAnaVisionDoc::OnFileSave() 
{
	DoFileSave(false);
}
void CAnaVisionDoc::OnFileSaveAs() {
	DoFileSave(true);
}

void CAnaVisionDoc::DoFileSave(bool ask){
	CString Name = GetPathName();
	if (dat.Converted){
		Name += "_.ana";
		CString a = "The file was not opened as an ANAVISION file.\r\n ";
		CString b = "Overwriting of the data is not possible.\r\n ";
		CString c = "\r\nSelecting \"Yes (Si)\" will save the file as\r\n\r\n";
		CString d = "\r\n\r\nSelecting \"No\" will not save anything.";

		if (!Confirm0(a+b+c+Name+d)){
			return ;
		}
		if (!ask){
			CFile file;
			if (file.Open(Name, CFile::modeRead)){
				if (!Confirm0("The file\r\n" + Name + "\r\n exists already. Sure to overwrite?")){
					return;
				}
				file.Close();
			}
		}
	}
	if (ask){
	   CFileDialog
		   diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
		diag.m_ofn.lpstrTitle = "Save file as";
		if (diag.DoModal()!= IDOK){
			return;
		}
		Name = diag.GetPathName();
	}

	
	SetPathName(Name, true);
	OnSaveDocument(Name);
	dat.Converted = false;
	this->SetModifiedFlag(false);
}


void CAnaVisionDoc::GetOffset(int & offsetx, int & offsety){
	dat.GetOffset(offsetx, offsety);
}

void  CAnaVisionDoc::IncOffset(int offsetx, int offsety){
	dat.IncOffset(offsetx, offsety);
}

void CAnaVisionDoc::ClearAllOffsets(){
	dat.ClearAllOffsets();
}



bool CAnaVisionDoc::OpenStandardTif(CString FileName){

	CheckLoadingFile CheckFile(&IsLoadingFile);

	CWaitCursor dummy;

	DeAlloc();
	dat.AnaTif.ImageOrderMethod = ORDER_BY_TIME;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	dat.p = pApp->p; // Use the previously active params as default for a newly opened file

//	if (dat.DoOpenTifFile(FileName)){
	if (dat.AssignFileNamesAndIndexes(FileName, NULL, true)){
		if (dat.p.xsizeimage<1){
			Alert0("Error : xsizeimage <1 ");
			return false;
		}

		if (dat.p.ysizeimage<1){
			dat.p.xsizeimage = dat.p.ysizeimage = 0;
			Alert0("Error : ysizeimage <1 ");
			return false;
		}
//		ShowFloat(dat.p.ysizeimage, "dat.p.ysizeimage");
		Alloc();
			
		this->NTimeData = (dat.images[dat.ImageIndex].LastIndex - dat.images[dat.ImageIndex].FirstIndex + 1);
//			ShowFloat(this->NTimeData, "this->NTimeData");
		this->AllocTime();
//			ShowFloat(NTimeData, "NTime");
//			ReadFile(dat.images[dat.ImageIndex].CurrentIndex);
		this->m_strPathName = FileName;
		return true;
	}
	else{
		Alert0("Error in reading file standard tiff");
		return false;
	}
}


void CAnaVisionDoc::UpdatePolygonDataPoints(){
	for (int j=0; j<dat.polys.NPoly(); j++){
		dat.polys.GetPoly(j)->UpdateDataPoints();
	}
}

bool CAnaVisionDoc::Openanavisionformat(CString FileName){
	CheckLoadingFile CheckFile(&IsLoadingFile);

	CWaitCursor dummy;

	DeAlloc();

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	dat.p = pApp->p; // Use the previously active params as default for a newly opened file


//	if (dat.DoOpenTifFile(FileName)){

	CFile file;
	if (!file.Open(FileName, CFile::modeRead)){
		Alert0("cannot open file in DataFile::ReadAnaVisionFileFormat");
		return false;
	}

	int bytesavailable = file.GetLength();

	if (!AnaVisionImageFileHyperStack::IsAnavisionHyperStack(bytesavailable, file)){
		Alert0("the file is not a AnaVisionImageFileHyperStack");
		return false;
	}
	file.SeekToBegin();
	bytesavailable = file.GetLength();

	if (dat.ReadAnaVisionFileFormat(bytesavailable, file)){

		if (dat.p.xsizeimage<1){
			Alert0("Error : xsizeimage <1 ");
			return false;
		}

		if (dat.p.ysizeimage<1){
			dat.p.xsizeimage = dat.p.ysizeimage = 0;
			Alert0("Error : ysizeimage <1 ");
			return false;
		}
	
		Alloc();

			
		this->NTimeData = (dat.images[dat.ImageIndex].LastIndex - dat.images[dat.ImageIndex].FirstIndex + 1);
//			ShowFloat(this->NTimeData, "this->NTimeData");
		this->AllocTime();
//			ShowFloat(NTimeData, "NTime");
//			ReadFile(dat.images[dat.ImageIndex].CurrentIndex);
		this->m_strPathName = FileName;
		return true;
	}
	else{
		Alert0("Error in reading file");
		return false;
	}
}

void CAnaVisionDoc::SaveFlimFretProject(bool ask){
	CString Name = GetPathName();
	if (dat.Converted){
		Name += "_.ana_flim";
		CString a = "The file was not opened as an ANAVISION file.\r\n ";
		CString b = "Overwriting of the data is not possible.\r\n ";
		CString c = "\r\nSelecting \"Yes (Si)\" will save the file as\r\n\r\n";
		CString d = "\r\n\r\nSelecting \"No\" will not save anything.";

		if (!Confirm0(a+b+c+Name+d)){
			return ;
		}
		if (!ask){
			CFile file;
			if (file.Open(Name, CFile::modeRead)){
				if (!Confirm0("The file\r\n" + Name + "\r\n exists already. Sure to overwrite?")){
					return;
				}
				file.Close();
			}
		}
	}
	if (ask){
	   CFileDialog
		   diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
		diag.m_ofn.lpstrTitle = "Save file as";
		if (diag.DoModal()!= IDOK){
			return;
		}
		Name = diag.GetPathName();
	}

	
	SetPathName(Name, true);
	OnSaveDocument(Name);
	dat.Converted = false;
	CMainFrame * f = (CMainFrame * )AfxGetMainWnd();
	CChildFrame * pChild = (CChildFrame *) f->MDIGetActive();
	CAnaVisionView * pV = pChild->GetCAnaVisionView();
	pV->SaveFlimFret(Name);//, dat.BytesWrittenToFile);
	this->SetModifiedFlag(false);
}

void CAnaVisionDoc::DoLoadFlimFretProject(CFile & file){
	DoLoad(&file);

}