#include "stdafx.h"
#include "AnaVision.h"
#include <afxdlgs.h>

#include "AnaVisionTif.h"
#include "AnaVisionImageFile.h"



MeanROIArray::MeanROIArray(){
}
MeanROIArray::~MeanROIArray(){
}
//	BufferClass<double> values;
bool MeanROIArray::SaveOnFile(CFile * fp, int & byteswritten){
	int NP=values.size();
	MyWriteVariable(NP);
	if (NP<1) return true;
//	double * buff = (double *)(values.buff);
	for (int i=0; i<NP; i++){
		MyWriteVariable(values[i]);
	}
	return true;
}
bool MeanROIArray::LoadFromFile(CFile * fp, int & bytesavailable ){
	int NP;
	MyReadVariable(NP);
//	ShowFloat(NP, "NP");
	if (NP<0){
		NP=0;
	}
	values.resize(NP);
	if (NP<1){
		return true;
	}
//	double * buff = (double *)values.buff;
	for (int i=0; i<NP; i++){
		MyReadVariable(values[i]);
	}
	return true;
}
void MeanROIArray::CalcMinMax(double & Min, double & Max){
	int NP=values.size();
	if (NP<1) return;
	double * buff = &values[0];
	double mm=MinDouble(buff, NP);
	if (mm<Min) Min = mm;
	mm=MaxDouble(buff, NP);
	if (mm>Max) Max = mm;
}
int MeanROIArray::GetMaxImages(){
	return values.size();
}

SeveralMeanROI::SeveralMeanROI(){
}
SeveralMeanROI::~SeveralMeanROI(){
}
//	BufferClass<MeanROIArray> ListOfArrays;
bool SeveralMeanROI::SaveOnFile(CFile * fp, int & byteswritten){
	int NP=ListOfArrays.GetSize();
	MyWriteVariable(NP);
	if (NP<1) return true;
	MeanROIArray * buff = (MeanROIArray *)(ListOfArrays.buff);
	for (int i=0; i<NP; i++){
		buff[i].SaveOnFile(fp, byteswritten);
	}
	return true;
}
bool SeveralMeanROI::LoadFromFile(CFile * fp, int & bytesavailable ){
	int NP;
	MyReadVariable(NP);
//	ShowFloat(NP, "NP");
	if (NP<0){
		NP=0;
	}
	ListOfArrays.UpdateBuffer(NP);
	if (NP<1){
		return true;
	}
	MeanROIArray * buff = (MeanROIArray *)ListOfArrays.buff;
	for (int i=0; i<NP; i++){
		if (!buff[i].LoadFromFile(fp, bytesavailable)){
			return false;
		}
	}
	return true;
}
void SeveralMeanROI::CalcMinMax(double & Min, double & Max){
	int NP=ListOfArrays.GetSize();
	if (NP<1) return;
	MeanROIArray * buff = (MeanROIArray *)ListOfArrays.buff;
	for (int i=0; i<NP; i++){
		buff[i].CalcMinMax(Min, Max);
	}
}
int SeveralMeanROI::GetMaxImages(){
	int NP = ListOfArrays.GetSize();
	if (NP<1) return 0;
	int MaxImages = 0;
	MeanROIArray * buff = (MeanROIArray  *)ListOfArrays.buff;
	for (int i=0; i<NP; i++){
		int n=buff[i].GetMaxImages();
		if (n>MaxImages) MaxImages=n;
	}
	return MaxImages;
}
AllROIValues::AllROIValues(){
}
AllROIValues::~AllROIValues(){
}
//	BufferClass<SeveralMeanROI> SeriesOfROIValues; 
bool AllROIValues::SaveOnFile(CFile * fp, int & byteswritten){
	int NP=SeriesOfROIValues.GetSize();
	MyWriteVariable(NP);
	if (NP<1) return true;
	SeveralMeanROI * buff = (SeveralMeanROI *)(SeriesOfROIValues.buff);
	for (int i=0; i<NP; i++){
		buff[i].SaveOnFile(fp, byteswritten);
	}
	return true;
}
bool AllROIValues::LoadFromFile(CFile * fp, int & bytesavailable ){
	int NP;
	MyReadVariable(NP);
//	ShowFloat(NP, "NP");
	if (NP<0){
		NP=0;
	}
	SeriesOfROIValues.UpdateBuffer(NP);
	if (NP<1){
		return true;
	}
	SeveralMeanROI * buff = (SeveralMeanROI  *)SeriesOfROIValues.buff;
	for (int i=0; i<NP; i++){
		if (!buff[i].LoadFromFile(fp, bytesavailable)){
			return false;
		}
	}
	return true;
}
void AllROIValues::CalcMinMax(double & Min, double & Max){
	Min = 1e300;
	Max = -1e300;
	int NP = this->SeriesOfROIValues.GetSize();
	if (NP<1) return;
	SeveralMeanROI * buff = (SeveralMeanROI  *)SeriesOfROIValues.buff;
	for (int i=0; i<NP; i++){
		buff[i].CalcMinMax(Min, Max);
	}
}
int AllROIValues::GetMaxImages(){
	int NP = this->SeriesOfROIValues.GetSize();
	if (NP<1) return 0;
	int MaxImages = 0;
	SeveralMeanROI * buff = (SeveralMeanROI  *)SeriesOfROIValues.buff;
	for (int i=0; i<NP; i++){
		int n=buff[i].GetMaxImages();
		if (n>MaxImages) MaxImages=n;
	}
	return MaxImages;
}


ImageSequence::ImageSequence(){
	CurrentIndex = 0;
	this->Sizex = 640;
	this->Sizey = 480;

#if defined SIMPLE_IMAGE_CORE
	SISStack = NULL;
#endif

	ptrTiff = NULL;
	TempImageData = NULL;

}
ImageSequence::~ImageSequence(){
//	if (SISStack){
	if (false){
		CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

#if defined SIMPLE_IMAGE_CORE
		pApp->Core->closeImageStack(SISStack);
		SISStack = NULL;
#endif

	}
	if (TempImageData){
		delete [] TempImageData;
	}

}


DataFile::DataFile(){
//	FileMode = 0; // ASCII

	SetVersion();
	FileType = 0;

	Converted = true; 
	images = NULL;
	NImages = 0;
	ImageIndex = 0;
	FileNamesAreRelative = true;

#if defined SIMPLE_IMAGE_CORE
	SISStack = NULL;
#endif

	MaxNumberOfImages = 0;
	OffsetX = NULL;
	OffsetY = NULL;
	SaveOffsetX = NULL;
	SaveOffsetY = NULL;

	lines.AreLines = true;
}

DataFile::~DataFile(){
#if defined SIMPLE_IMAGE_CORE
	if (SISStack){
		CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
		pApp->Core->closeImageStack(SISStack);
		SISStack = NULL;
	}
#endif

	DeAllocOffsetArrays();
}
void DataFile::DeAllocOffsetArrays(){
	if (OffsetX) delete [] OffsetX;
	if (OffsetY) delete [] OffsetY;
	if (SaveOffsetX) delete [] SaveOffsetX;
	if (SaveOffsetY) delete [] SaveOffsetY;
}
void DataFile::AllocOffsetArrays(){
	SaveOffsetY = new int [MaxNumberOfImages];
	SaveOffsetX = new int [MaxNumberOfImages];
	OffsetY = new int [MaxNumberOfImages];
	OffsetX = new int [MaxNumberOfImages];
}
void DataFile::SaveOffsetArrays(){
	if (MaxNumberOfImages<=0) return;
	for (int i=0; i<MaxNumberOfImages; i++){
		SaveOffsetX[i] = OffsetX[i];
		SaveOffsetY[i] = OffsetY[i];
	}

}
void DataFile::RetrievOffsetArrays(){
	if (MaxNumberOfImages<=0) return;
	for (int i=0; i<MaxNumberOfImages; i++){
		OffsetX[i] = SaveOffsetX[i];
		OffsetY[i] = SaveOffsetY[i];
	}
}

void DataFile::SetVersion(){
	Version1 = 2;
	Version1 = 3;
	Version1 = 4;
	Version1 = 5;
	Version1 = 6;
	Version1 = 7;
	Version1 = 8;
	Version1 = 9;
	Version1 = 10;
	Version1 = 11;
	Version1 = 12;



	Version2 = 0;
	Version3 = 0;


//#define CURRENT_DATFILE_VERSION 8
//#define CURRENT_DATFILE_VERSION 9
//#define CURRENT_DATFILE_VERSION 10
#define CURRENT_DATFILE_VERSION 12
}

bool DataFile::IsAnavisionFile(CFile *fp) { // Tests only if the first thing in the file is the string
									// ANAVISION
	if (!fp) return false;
	CFile & file = *fp;
	file.SeekToBegin();
	char s[10];
	if (file.GetLength()<9){
		return false;
	}
	file.Read(s, 9);
	s[9]=0;
	CString c = CString(s);
//	Alert0(c);
	CString comp = CString("ANAVISION");
	if (c.Compare(comp)!=0){
		return false;
	}

	return true;
}

void DataFile::SaveOnFile(CFile & f){

	// SAVE FIRST AN ID!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// First save p and then the rest
//	ShowFloat(FileType, "FileType");
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	CFile * fp = &(f);
	char s[9]; 
	s[0]='A';	s[1]='N';	s[2]='A';	s[3]='V';	s[4]='I';	s[5]='S';
	s[6]='I';	s[7]='O';	s[8]='N';

	f.Write(s, 9);
	int byteswritten = 9;

	MyWriteVariable(Version1);
	MyWriteVariable(Version2);
	MyWriteVariable(Version3);

	int nbytesparams = p.SaveOnFile(NULL); // setting the arg to NULL does not write

	MyWriteVariable(nbytesparams);

	byteswritten += p.SaveOnFile(fp);

	MyWriteVariable(FileType);


	if (FileType==0){

		pApp->MySaveString(byteswritten, fp, FirstFileName);

		pApp->MySaveString(byteswritten, fp, Extension);
		pApp->MySaveString(byteswritten, fp, rootpath);

		MyWriteVariable(FileNamesAreRelative);
		MyWriteVariable(NImages);

		for (unsigned int i=0; i<NImages; i++){
				MyWriteVariable(images[i].FirstIndex);
				MyWriteVariable(images[i].LastIndex);
				MyWriteVariable(images[i].FileMode);
				MyWriteVariable(images[i].Sizex);
				MyWriteVariable(images[i].Sizey);
				pApp->MySaveString(byteswritten, fp, images[i].FileName);
				pApp->MySaveString(byteswritten, fp, images[i].Title);
		}
	}

	if ((FileType==1) || (FileType==2)){

		MyWriteVariable(FileOffset);
		if (Version1>8){
			MyWriteVariable(AnaTif.ImageOrderMethod);
//			ShowFloat(AnaTif.ImageOrderMethod, "AnaTif.ImageOrderMethod");
		}
		if (Version1>9){
			MyWriteVariable(AnaTif.IsStandardTif);
//			ShowFloat(AnaTif.ImageOrderMethod, "AnaTif.ImageOrderMethod");
		}


		pApp->MySaveString(byteswritten, fp, FirstFileName);

		pApp->MySaveString(byteswritten, fp, Extension);
		pApp->MySaveString(byteswritten, fp, rootpath);

		MyWriteVariable(FileNamesAreRelative);
		MyWriteVariable(NImages);

		for (unsigned int i=0; i<NImages; i++){
				MyWriteVariable(images[i].FirstIndex);
				MyWriteVariable(images[i].LastIndex);
				MyWriteVariable(images[i].FileMode);
				MyWriteVariable(images[i].Sizex);
				MyWriteVariable(images[i].Sizey);
				pApp->MySaveString(byteswritten, fp, images[i].FileName);
				pApp->MySaveString(byteswritten, fp, images[i].Title);
		}
	}
	if (FileType==5){
		this->HS.WriteHyperStackToFile(byteswritten, f);
	}


	polys.SaveOnFile(fp, byteswritten, Version1);
	lines.SaveOnFile(fp, byteswritten, Version1);
	roivalues.SaveOnFile(fp, byteswritten);

	MyWriteVariable(MaxNumberOfImages);

	for (int i=0; i<MaxNumberOfImages; i++){
		MyWriteVariable(OffsetX[i]);
		MyWriteVariable(OffsetY[i]);
	}

	int nbyteskinetics;

	nbyteskinetics = k.SaveOnFile(NULL); // setting the arg to NULL does not write
//	ShowFloat(nbyteskinetics , "nbyteskinetics ");

	MyWriteVariable(nbyteskinetics);

	byteswritten += k.SaveOnFile(fp);

	BytesWrittenToFile = byteswritten;
//	polys.SaveOnFile(fp, byteswritten);

//	ShowFloat(byteswritten, "byteswritten");
//	ShowFloat(byteswritten, "byteswritten");

}

bool DataFile::LoadFromFile(CFile & f){
	ImageIndex = 0;
	MaxNumberOfImages = 0;

//	CurrentIndex = 0;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	CFile * fp = &(f);

	CString AnaVisionString = "ANAVISION";

	unsigned __int64 l = f.GetLength();
	int bytesavailable = int(l);

	if (l<11){
		return false;
	}
	char s[20];
	f.SeekToBegin();
	f.Read(s, 9);
	s[9]=0;
	if (AnaVisionString.Compare(CString(s))!= 0){
		return false;
	}

	MyReadVariable(Version1);
	if (Version1<2){
		f.SeekToBegin();
		return OldLoadFromFile(f);
	}
	if (Version1>CURRENT_DATFILE_VERSION){
		Alert0("This file was created by a more recent version of AnaVision than this. Probably, it cannot be loaded");
	}

//	ShowFloat(Version1, "Version1");
	MyReadVariable(Version2);
//	ShowFloat(Version2, "Version2");
	MyReadVariable(Version3);

	int nbytesparams;
	MyReadVariable(nbytesparams);

	if (!p.ReadFromFile(&f, nbytesparams)){
		return false;
	}
	bytesavailable -= nbytesparams;
	
	if (Version1>6){
		MyReadVariable(FileType);
	}
	else{
		FileType = 0; // vws file converted
	}

//	ShowFloat(FileType, "FileType");
	if (FileType==0){

		pApp->MyLoadString(bytesavailable, fp, FirstFileName);
		pApp->MyLoadString(bytesavailable, fp, Extension);
		pApp->MyLoadString(bytesavailable, fp, rootpath);

//Alert0(FirstFileName);
//	Alert0(Extension);
//	Alert0(rootpath);

		MyReadVariable(FileNamesAreRelative);
		if (FileNamesAreRelative){
			if (Extension.Compare("VWS") == 0){

//			FirstFileName = f.GetFilePath();
				int slashposFirstFileName1 = FirstFileName.ReverseFind('\\');
				int slashposFirstFileName2 = FirstFileName.ReverseFind('/');

				int slashposFirstFileName = slashposFirstFileName1;
				if (slashposFirstFileName2 > slashposFirstFileName){
					slashposFirstFileName = slashposFirstFileName2;
				}
				if (slashposFirstFileName != -1){
					FirstFileName = FirstFileName.Right(FirstFileName.GetLength()-slashposFirstFileName-1);
				}
				CString pstdir=FirstFileName;
				int dotpos=pstdir.Find('.');
				if (dotpos != -1){
					pstdir = pstdir.Left(dotpos);
				
				}
				pstdir += ".pst/";
//			Alert0(pstdir);


				rootpath = f.GetFilePath();
				int slashposrootpath1 = rootpath.ReverseFind('\\');
				int slashposrootpath2 = rootpath.ReverseFind('/');
	
				int slashposrootpath = slashposrootpath1;
				if (slashposrootpath2 > slashposrootpath){
					slashposrootpath = slashposrootpath2;
				}
				if (slashposrootpath != -1){
					rootpath = rootpath.Left(slashposrootpath+1);
				}
				rootpath += pstdir;
	
			
//			rootpath = rootpath.Left(rootpath.GetLength()-8);
//			rootpath = rootpath + "pst/";

//			Alert0(rootpath);
			}
		}
		MyReadVariable(NImages);

		if (NImages == 0){
			return false;
		}
		images = new ImageSequence[NImages];

		for (unsigned int i=0; i<NImages; i++){
				MyReadVariable(images[i].FirstIndex);
				MyReadVariable(images[i].LastIndex);
				MyReadVariable(images[i].FileMode);
				if (images[i].FileMode<0){
					return false;
				}
				if (images[i].FileMode>1){
					return false;
				}
				if (Version1  > 3){
					MyReadVariable(images[i].Sizex);
					if (images[i].Sizex<1){
						return false;
					}
					MyReadVariable(images[i].Sizey);
					if (images[i].Sizey<1){
						return false;
					}
				}
				else{
					images[i].Sizex = p.xsizeimage;
					images[i].Sizey = p.ysizeimage;
				}

				pApp->MyLoadString(bytesavailable, fp, images[i].FileName);
				pApp->MyLoadString(bytesavailable, fp, images[i].Title);
				images[i].FileNamesAreRelative = FileNamesAreRelative;
				images[i].RootName = rootpath;
				images[i].initialized = images[i].CheckAvailabilityDataFile();
		}
	}
	if (FileType==1){
		MyReadVariable(FileOffset);
		bool IsNewSISFile = false;
		if (FileOffset==-999){
//			Alert0("new SIS");
			IsNewSISFile = true;
		}
		else{
			IsNewSISFile = false;

		}

		pApp->MyLoadString(bytesavailable, fp, FirstFileName);
		pApp->MyLoadString(bytesavailable, fp, Extension);
		pApp->MyLoadString(bytesavailable, fp, rootpath);

		if (!IsNewSISFile){
			CString FileName = FirstFileName;
			GetFileName(FileName, this->FirstFileName);
			GetPath(FileName, this->rootpath);
			IsNewSISFile = true;
		}

//	Alert0(FirstFileName);
//	Alert0(Extension);
//	Alert0(rootpath);
		MyReadVariable(FileNamesAreRelative);
		MyReadVariable(NImages);

		if (NImages == 0){
			return false;
		}
		images = new ImageSequence[NImages];

#if defined SIMPLE_IMAGE_CORE
		CString SISFileName;
		arivis::simpleimagecore::SimpleImageStack * imageStack = NULL;

		if (IsNewSISFile){
			CString FileName = f.GetFilePath();
			GetPath(FileName, rootpath);
			SISFileName = rootpath + FirstFileName;
	//		Alert0(SISFileName);
			CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
			imageStack=	pApp->Core->openImageStack(SISFileName);
			SISStack = imageStack;
			if (!imageStack){
				Alert0("could not open image stack");
				return false;
			}
		}
#endif

#if defined SIMPLE_IMAGE_CORE
		for (unsigned int i=0; i<NImages; i++){
			images[i].SISStack  = imageStack;
			MyReadVariable(images[i].FirstIndex);
			MyReadVariable(images[i].LastIndex);
			MyReadVariable(images[i].FileMode);
			if (images[i].FileMode<0){
				return false;
			}
			if (images[i].FileMode!=2){
				Alert0("images[i].FileMode!=2");
				return false;
			}
			MyReadVariable(images[i].Sizex);
			if (images[i].Sizex<1){
				return false;
			}
			MyReadVariable(images[i].Sizey);
			if (images[i].Sizey<1){
				return false;
			}

			pApp->MyLoadString(bytesavailable, fp, images[i].FileName);
			pApp->MyLoadString(bytesavailable, fp, images[i].Title);
			images[i].FileNamesAreRelative = FileNamesAreRelative;
			images[i].RootName = rootpath;
			images[i].initialized = images[i].CheckAvailabilityDataFile(fp);
			if (!images[i].initialized){
				ShowFloat(i, "image not initialzied");
			}
		}
#endif
	}

	if (FileType==2){
		MyReadVariable(FileOffset);
//		ShowFloat(FileOffset, "oFF");
		bool IsNewTIFFile = false;
		if (FileOffset==-9999){
//			Alert0("new TIF");
			IsNewTIFFile = true;
		}
		else{
			IsNewTIFFile = false;

		}
		if (this->Version1>8){
			MyReadVariable(AnaTif.ImageOrderMethod);
		}
		if (Version1>9){
			MyReadVariable(AnaTif.IsStandardTif);
//			ShowFloat(AnaTif.ImageOrderMethod, "AnaTif.ImageOrderMethod");
		}

		pApp->MyLoadString(bytesavailable, fp, FirstFileName);
		pApp->MyLoadString(bytesavailable, fp, Extension);
		pApp->MyLoadString(bytesavailable, fp, rootpath);

		if (!IsNewTIFFile){
			CString FileName = FirstFileName;
			GetFileName(FileName, this->FirstFileName);
			GetPath(FileName, this->rootpath);
			IsNewTIFFile = true;
		}

//	Alert0(FirstFileName);
//	Alert0(Extension);
//	Alert0(rootpath);
		MyReadVariable(FileNamesAreRelative);
		MyReadVariable(NImages);

		if (NImages == 0){
			return false;
		}
		images = new ImageSequence[NImages];

		CString TIFFileName;

		if (IsNewTIFFile){
			CString FileName = f.GetFilePath();
			GetPath(FileName, rootpath);
			TIFFileName = rootpath + FirstFileName;
//			Alert0(TIFFileName);
			CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
//			ShowFloat( AnaTif.ImageOrderMethod, "imageorder");
			bool result = AnaTif.OpenFile(TIFFileName);
			if (!result){
				Alert0("Could not open TIFF");
				return false;
			}
		}

		for (unsigned int i=0; i<NImages; i++){
			images[i].ptrTiff = &(AnaTif);
			MyReadVariable(images[i].FirstIndex);
			MyReadVariable(images[i].LastIndex);
			MyReadVariable(images[i].FileMode);
			if (images[i].FileMode<0){
				return false;
			}
			if (images[i].FileMode!=3){
				Alert0("images[i].FileMode!=3");
				return false;
			}
			MyReadVariable(images[i].Sizex);
			if (images[i].Sizex<1){
				return false;
			}
			MyReadVariable(images[i].Sizey);
			if (images[i].Sizey<1){
				return false;
			}

			pApp->MyLoadString(bytesavailable, fp, images[i].FileName);
//			Alert0(images[i].FileName);
			pApp->MyLoadString(bytesavailable, fp, images[i].Title);
			images[i].FileNamesAreRelative = FileNamesAreRelative;
			images[i].RootName = rootpath;
			images[i].initialized = images[i].CheckAvailabilityDataFile(fp);
			if (!images[i].initialized){
				ShowFloat(i, "image not initialzied");
			}
		}
	}

	if (FileType==5){
		if (!this->ReadAnaVisionFileFormat(bytesavailable, f)){
			Alert0("could not read anavision file");
		}
	}
	if (Version1<3){
		SetVersion();
		return true;
	}
	if (Version1>4){
		if (bytesavailable<2){
			//Beep(1000,10);
		}
		if (!polys.LoadFromFile(&f, bytesavailable, Version1)){
			return false;
		}
		PolygonObject * polyd = polys.GetPoly(0);
		//ShowFloat(polyd->pointlist[0].x, "xd");
		//ShowFloat(polys.NPoly(), "NPloy in load from file");
	}
	if (Version1>10){
		if (!lines.LoadFromFile(&f, bytesavailable, Version1)){
			return false;
		}
	}

	if (Version1>5){
		if (bytesavailable<2){
			//Beep(1000,10);
		}
		if (!roivalues.LoadFromFile(&f, bytesavailable)){
			return false;
		}
	}

//	ShowFloat(Version1, "version1");
	if (Version1>7){
		MyReadVariable(MaxNumberOfImages);
		AllocOffsetArrays();

		for (int i=0; i<MaxNumberOfImages; i++){
			MyReadVariable(OffsetX[i]);
//			if (i==0) ShowFloat(OffsetX[i], "ox");
			MyReadVariable(OffsetY[i]);
		}
	}
	else{
		CalcMaxNumberOfImages();
	}

	int nbyteskinetics=0;
	MyReadVariable(nbyteskinetics);
//	ShowFloat(nbyteskinetics, "nbyteskinetics");

	if (nbyteskinetics<=0){
		//Beep(1000,10);
//		return true;
	}
	else{

		if (!k.ReadFromFile(&f, nbyteskinetics)){
			return false;
		}
		bytesavailable -= nbyteskinetics;
	}

	//ShowFloat(bytesavailable, "bytesavailable");
//ReadPolygons:
//	ShowFloat(bytesavailable, "bytesavailable");

/*
if (Version1>4){
		if (bytesavailable<2){
			Beep(1000,10);
		}
		if (!polys.LoadFromFile(&f, bytesavailable)){
			return false;
		}
	}
*/


	SetVersion();

	return true;
}
bool DataFile::OldLoadFromFile(CFile & f){
//	CurrentIndex = 0;
	ImageIndex = 0;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	CFile * fp = &(f);

	CString AnaVisionString = "ANAVISION";

	unsigned __int64 l = f.GetLength();
	int bytesavailable = int(l);

	if (l<11){
		return false;
	}
	char s[20];
	f.SeekToBegin();
	f.Read(s, 9);
	s[9]=0;
	if (AnaVisionString.Compare(CString(s))!= 0){
		return false;
	}

	MyReadVariable(Version1);
//	ShowFloat(Version1, "Version1");
	MyReadVariable(Version2);
//	ShowFloat(Version2, "Version2");
	MyReadVariable(Version3);

	int nbytesparams;
	MyReadVariable(nbytesparams);

	if (!p.ReadFromFile(&f, nbytesparams)){
		return false;
	}
	bytesavailable -= nbytesparams;
	

	NImages=1;
	images = new ImageSequence[NImages];


	pApp->MyLoadString(bytesavailable, fp, FirstFileName);
	images[0].FileName = FirstFileName;

	char tit[1000];
	GetFileTitle(FirstFileName, tit, 1000);
	images[0].Title = CString(tit);
	

	pApp->MyLoadString(bytesavailable, fp, Extension);

	MyReadVariable(images[0].FirstIndex);
	MyReadVariable(images[0].LastIndex);
	MyReadVariable(images[0].FileMode);
	if (images[0].FileMode<0){
		return false;
	}
	if (images[0].FileMode>1){
		return false;
	}

//	initialized  = CheckAvailabilityDataFile();

//	return initialized;
	FileNamesAreRelative = false;

	SetVersion();

	rootpath ="";
	images[0].RootName = rootpath;
	images[0].FileNamesAreRelative = FileNamesAreRelative ;

	return images[0].CheckAvailabilityDataFile();
}

bool ImageSequence::CheckAvailabilityDataFile(CFile * fp){
//	ShowFloat(FileMode, "fm");
	if (FileMode == 0){
		return CheckAvailabilityTextFiles();
	}
	if (FileMode == 1){
		return CheckAvailabilityPst(RootName, FileNamesAreRelative);
	}
	if (FileMode == 2){
		return CheckAvailabilitySis(fp);
	}
	if (FileMode == 3){
		return CheckAvailabilityTif(fp);
	}
	return false;

}
bool ImageSequence::CheckAvailabilitySis(CFile * fp){
	if (!fp) return false;
	CString  path;
	GetPath(fp->GetFilePath(), path);
	CString Name = path+this->FileName;
//	Alert0(Name);
	return FileExists(Name);
}
bool ImageSequence::CheckAvailabilityTif(CFile * fp){
	if (!fp) return false;
	CString  path;
	GetPath(fp->GetFilePath(), path);
	CString Name = path+this->FileName;
//	Alert0(Name);
	return FileExists(Name);
}
bool ImageSequence::FindTheFile(CString & Name){
	CFileDialog diag( true, NULL,Name, OFN_FILEMUSTEXIST, NULL, AfxGetMainWnd());
	diag.m_ofn.lpstrTitle = "IMAGE DATA FILE NOT FOUND: SELECT THE IMAGE DATA FILE NAME";
//	diag.SetWindowText("IMAGE DATA FILE NOT FOUND: SELECT THE IMAGE DATA FILE NAME");
	if (diag.DoModal()!=IDOK){
		return false;
	}
	Name = diag.GetPathName();
	return true;
}

bool ImageSequence::CheckAvailabilityTextFiles(){
	/*

	CString Name = MakeFileName(FirstFileName, FirstIndex, Extension);
	FILE * file = fopen(Name, "r");
	if (!file){
		if (!FindTheFile(Name)){
			return false;
		}
		return AssignFileNameAndIndexesTxtFiles(Name);
	}
	*/
	return true;
}
bool ImageSequence::CheckAvailabilityPst(CString rootpath, bool FileNamesAreRelative){
	CFile file;
	CString name = FileName;
	if (FileNamesAreRelative){
		name = rootpath + name;
//		Alert0(name);
	}
	if (!file.Open(name, CFile::modeRead)){
		if (FileNamesAreRelative){
			Alert0("The directory or files of the image data was not found");
			return false;
		}
		else{
			if (!FindTheFile(FileName)){
				return false;
			}
		}
	}
	return true;
}


CString ImageSequence::GetFileName(){
	CString name = FileName;
	if (this->FileNamesAreRelative){
		name = RootName + FileName;
	}
	return name;
}

bool ImageSequence::ReadPstFile(int index, CFile * fp, ImageDataType ** currentimage){

//	ShowFloat(this->FirstIndex, "this->FirstIndex");
//	ShowFloat(this->LastIndex, "this->LastIndex");

	if (!initialized){
//		Alert0("not initialized in ReadPstFile");
		return false;
	}
	if (index<FirstIndex){
		Alert0("index<FirstIndex in ReadPstFile");
		return false;
	}
//	ShowFloat(LastIndex, "last index");
	if (index>LastIndex){
//		Alert0("index>LastIndex in ReadPstFile");
		return false;
	}
	CFile file;

	CFile * fp2;
	if (fp){
		fp2=fp;
	}
	else{
		CString name = GetFileName();
//		if (this->FileNamesAreRelative){
//			name = RootName + FileName;
//		}
		if (!file.Open(name, CFile::modeRead)){
//			Alert0(FileName);
			Alert0("could not open file for read");
			return false;
		}
		fp2=&file;
	}

//	UINT offset = index*2*pp->xsizeimage*pp->ysizeimage;
	uint64 offset = index*2*this->Sizex*this->Sizey;

	fp2->Seek(offset, 0L);


//	for (unsigned __int32 y=0; y<pp->ysizeimage; y++){


	__int16 * arr = new __int16[Sizex*Sizey];

	BYTE * b = (BYTE *)&(arr[0]);

	uint64 toread = 2*Sizex*Sizey;
	
	uint64 actuallyread = fp2->Read(b, UINT(toread) );

	if (actuallyread < toread){
		Alert0("could not read 2 image");
		return false;
	}
//	Alert0("hi");
	int iter=0;
	for (__int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (__int32 x=0; x<this->Sizex; x++){
			currentimage[x][y] = arr[iter];
			iter++;
		}
	}

	delete [] arr;


/*
	for (unsigned __int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (unsigned __int32 x=0; x<this->Sizex; x++){
			__int16 d;
			int result = fp2->Read(&d, 2);
			if (result<2){
				Alert0("could not read 2 bytes");
				return false;
			}
			currentimage[x][y] = d;
		}
	}
*/
//	pp->Convert(currentimage, pp->xsizeimage, pp->ysizeimage);
	pp->Convert(currentimage, this->Sizex, this->Sizey);
	imageready = true;

	return true;
}

UINT Permute(UINT ChannelNumber, UINT NChannels){
	if (NChannels == 1){
		return ChannelNumber;
	}
	if (NChannels == 2){
		return ChannelNumber;
	}
	switch (ChannelNumber){
		case 0 : return 5; break;
		case 1 : return 2; break;
		case 2 : return 0; break;
		case 3 : return 3; break;
		case 4 : return 4; break;
		case 5 : return 1; break;
		default : return ChannelNumber; break;
	}
}
bool ImageSequence::ReadSisFile(int index, ImageDataType ** currentimage, UINT ChannelNumber){
#if defined SIMPLE_IMAGE_CORE
	if (!initialized){
//		Alert0("not initialized in ReadPstFile");
		return false;
	}
	if (index<FirstIndex){
		Alert0("index<FirstIndex in ReadPstFile");
		return false;
	}
//	ShowFloat(LastIndex, "last index");
	if (index>LastIndex){
//		Alert0("index>LastIndex in ReadPstFile");
		return false;
	}
	if (!SISStack) {
		Alert0("no sis stack open");
		return false;
	}

	arivis::simpleimagecore::Rectangle rect = 	SISStack->getBounds();
	unsigned __int16 * arr = new unsigned __int16[Sizex*Sizey];
	int plane = 0;

	SISStack->read16bitChannelData(rect, 1.0, arr, ChannelNumber, plane, index);

	unsigned __int64 iter = 0;
	for (__int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (__int32 x=0; x<this->Sizex; x++){
			currentimage[x][y] = arr[iter];
			iter++;
		}
	}

	delete [] arr;

	pp->Convert(currentimage, this->Sizex, this->Sizey);
	imageready = true;

	return true;
#else
	return false;
#endif

}

bool ImageSequence::ReadAnaVisionImage(int index, ImageDataType ** currentimage, UINT ChannelNumber){
//	Beep(500,1000);
	if (!initialized){
		Alert0("not initialized in ReadAnaVisionImage");
		return false;
	}
	if (index<FirstIndex){
		Alert0("index<FirstIndex in ReadAnaVisionImage");
		return false;
	}
	if (index>LastIndex){
		Alert0("index>LastIndex in ReadAnaVisionImage");
		return false;
	}

	AnaVisionImageFile & AF = this->AS->Images[index];
	this->Sizex = AF.Width;
	this->Sizey = AF.Height;
	

//	ShowFloat(AF.Width, "W in  ReadAnaVisionImage");
//	ShowFloat(index, "index in ReadAnaVisionImage");
//	for (__int32 y=0; y<this->Sizey; y++){
	int d=0;
	for (__int32 y=0; y<AF.Height; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
//		for (__int32 x=0; x<this->Sizex; x++){
		for (__int32 x=0; x<AF.Width; x++){
			currentimage[x][y] = AF.data[d];
			d++;
//			currentimage[x][y] = AF.GetPixel(x,y);
		}
	}
//	Alert0("hi");
	pp->Convert(currentimage, this->Sizex, this->Sizey);
	imageready = true;

	return true;
}

bool ImageSequence::ReadTifFile(int index, ImageDataType ** currentimage, UINT ChannelNumber){
//	ShowFloat(index, "index");
	if (!initialized){
		Alert0("not initialized in ReadTifFile");
		return false;
	}
	if (index<FirstIndex){
		Alert0("index<FirstIndex in ReadTifFile");
		return false;
	}
	if (index>LastIndex){
		Alert0("index>LastIndex in ReadTifFile");
		return false;
	}
//	ShowFloat(LastIndex, "last index");
	if (index>LastIndex){
//		Alert0("index>LastIndex in ReadPstFile");
		return false;
	}

//	arivis::simpleimagecore::Rectangle rect = 	SISStack->getBounds();
	if (!TempImageData){
		TempImageData = new unsigned __int16[Sizex*Sizey];
	}
	
	unsigned __int16 * arr = TempImageData; //new unsigned __int16[Sizex*Sizey];

//	if (index>96){
	if (false){
		ShowFloat(index, "index");
		ShowFloat(ChannelNumber, "ChannelNumber");
	}

	if (!this->ptrTiff->ReadImage(index, ChannelNumber, arr)){
		Alert0("Error reading tiff");
	}

//	SISStack->read16bitChannelData(rect, 1.0, arr, ChannelNumber, plane, index);

	unsigned __int64 iter = 0;
	for (__int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (__int32 x=0; x<this->Sizex; x++){
			currentimage[x][y] = arr[iter];
			iter++;
		}
	}

//	delete [] arr;

	pp->Convert(currentimage, this->Sizex, this->Sizey);
	imageready = true;

	return true;
}


CString ImageSequence::MakeFileName(CString Name, int index, CString ext){
	CString result = Name + CString("_");
	char s[20];
	_itoa(index, s, 10);
	return result+CString(s)+ext;
}

bool DataFile::ReadImage(int index, ImageData & id,   CellsClass * & celllist, bool RefImage, bool DoShow){
//	imageready = images[ImageIndex].ReadImage(index, p, data);
//	bool ReadImage(int index, Params & p, ImageData & id, bool RefImage, UINT fileoffset = 0, UINT NChannels=1, UINT ChannelNumber = 0);

	if (DoShow){
		ShowFloat(ImageIndex, "ImageIndex");
		ShowFloat(index, "index");
	}

	int indexminusoffset = index-images[ImageIndex].FirstIndex;
	p.offsetx = this->OffsetX[indexminusoffset ];
	p.offsety = this->OffsetY[indexminusoffset ];

	imageready = images[ImageIndex].ReadImage(index, p, id, RefImage, FileOffset, NImages, ImageIndex, DoShow);
//	ShowFloat(index, "index");
//	ShowFloat(ImageIndex, "ImageIndex");
	if (!imageready){
		Alert0("image not ready in DataFile::ReadImage");
	}
	if (imageready){
		celllist = &(images[ImageIndex].celllistarray[index]);
//		celllist=NULL;
	}

//	Alert0("hi");
	return imageready;
}

void DataFile::AssignValue(__int16 value){
	if (!images){
		return;
	}
	images[ImageIndex].AssignValue(value);
}

bool ImageSequence::AssignValue( __int16 value){
	int index = CurrentIndex;
	if (index<0) return false;

	if (!initialized){
//		Alert0("not initialized in ReadPstFile");
		return false;
	}
	if (index<FirstIndex){
		Alert0("index<FirstIndex in AssignValue");
		return false;
	}
	if (index>LastIndex){
		Alert0("index>LastIndex in AssignValue");
		return false;
	}

//	if (index>=NImages) return false;

	CString oldfilename = GetFileName();

//	CString c="pause hi \r\n copy "+oldfilename+" "+newfilename+"\r\n\ pause hi";
	CFile f1;
	if (!f1.Open(oldfilename, CFile::modeRead)){
		Alert0("The directory or files of the image data was not found");
		return false;
	}

	FileName = GetNewFileName(FileName);
	CString newfilename = GetFileName();

	CFile f2;
//	Alert0(newfilename);
	if (!f2.Open(newfilename, CFile::modeCreate | CFile::modeWrite)){
		Alert0("could not open outfile");
		return false;
	}


	BYTE block[10000];

	unsigned __int64 rest=f1.GetLength();
	unsigned __int64 read = 10000;

	while (rest>0){
		if (read>rest) read = rest;
		f1.Read(block, UINT(read));
		f2.Write(block, UINT(read));
		rest -= read;
	}

	f1.Close();

	uint64 offset = index*2*this->Sizex*this->Sizey;

	f2.Seek(offset, 0L);


//	for (unsigned __int32 y=0; y<pp->ysizeimage; y++){
	for (__int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (__int32 x=0; x<this->Sizex; x++){
			f2.Write(&value, 2);
		}
	}
//	pp->Convert(currentimage, pp->xsizeimage, pp->ysizeimage);
	return true;

}

bool ImageSequence::ReadImage(int index, Params & p, ImageData & id, bool RefImage,
							  unsigned __int64 fileoffset, UINT NChannels,  UINT ChannelNumber, bool DoShow){
//bool ImageSequence::ReadImage(int index, Params & p, double ** data){
	pp=&(p);

	p.xsizeimage = this->Sizex;
	p.ysizeimage = this->Sizey;

//	ShowFloat(id.xsize, "id.xsize in ImageSequence::ReadImage");

	id.xsize = this->Sizex;
	id.ysize = this->Sizey;

//	ShowFloat(id.xsize, "id.xsize in ImageSequence::ReadImage");

	p.RectOb.SetSize(p.xsizeimage, p.ysizeimage);
	p.ConusOb.SetSize(p.xsizeimage, p.ysizeimage);


	ImageDataType ** d = id.data;
	if (RefImage){
		d = id.refdata;
	}
//	ShowFloat(FileMode, "FileMode");
	if (FileMode == 5){
		bool result = ReadAnaVisionImage(index, d, ChannelNumber);
//		ShowFloat(index, "index");
		if (!result){
			Alert0("could not read AnaVisionImage");
		}
		id.xsize = p.xsizeimage = this->Sizex;
		id.ysize = p.ysizeimage = this->Sizey;
		p.RectOb.SetSize(p.xsizeimage, p.ysizeimage);
		p.ConusOb.SetSize(p.xsizeimage, p.ysizeimage);
//		Beep(1000,100);
//		ShowFloat(this->Sizey, "this->Sizey in ImageSequence::ReadImage");
		return result;
	}

	if (FileMode == 1){
		//ShowFloat(index, "index");
		return ReadPstFile(index, NULL, d);
	}
	if (FileMode == 2){
//		return ReadSisFile(index, NULL, d, fileoffset, NChannels, ChannelNumber);
		return ReadSisFile(index, d, ChannelNumber);
	}
	if (FileMode == 3){
		if (DoShow){
			ShowFloat(index, "index");
			ShowFloat(ChannelNumber, "ChannelNumber");
		}
//		return ReadSisFile(index, NULL, d, fileoffset, NChannels, ChannelNumber);
		return ReadTifFile(index, d, ChannelNumber);
	}
	return false;
/*

	if (!initialized){
//		Alert0("not initialized");
		return false;
	}
	if (index<FirstIndex){
		Alert0("index<FirstIndex");
		return false;
	}
	if (index>LastIndex){
		Alert0("index>LastIndex");
		return false;
	}

	CString Name = MakeFileName(FirstFileName, index, Extension);

	FILE * file = fopen(Name, "r");
	if (!file){
		Alert0(CString("could not open file")+Name);
		return false;
	}
	imageready = false;
	if (!DoReadTxtFile(file, data)){
		Alert0(CString("Could not read file ")+Name);
		fclose(file);
		return false;
	}
	fclose(file);
	imageready = true;
	return true;
*/
}

bool ImageSequence::DoReadTxtFile(FILE * f, ImageDataType ** currentimage){

	return false;
/*
	for (unsigned __int32 y=0; y<p.ysizeimage; y++){
		for (unsigned __int32 x=0; x<p.xsizeimage; x++){
			if (!ReadDouble(currentimage[x][y], f)){
				return false;
			}
//			currentimage[x][y] = p.Convert(d);
		}
	}
	p.Convert(currentimage, p.xsizeimage, p.ysizeimage);

	return true;
*/
}


bool DataFile::AssignFileNamesAndIndexes(CString Name, CFile * fp, bool IsStandardTif){
//	Alert0(Name);
	CMainFrame* frame = (CMainFrame *)(AfxGetMainWnd( ));

	int l = Name.GetLength(); 
	if (l<7){ // must be at least "x_0.txt"
		Alert0("Too short file name");
		return false;
	}

	int FileMode = 0;
	Converted = true;
	
	CString Ext = Name.Right(3);

	CString EXT = Ext;
	EXT.MakeUpper();

//	initialized = false;

	if (fp) fp->SeekToBegin();

	if (IsAnavisionFile(fp)){
//		Alert0("Is anabv");
		FileMode = 2;
		Converted = false;
		bool success = LoadFromFile(*fp);
		if (success){
			AllocCellLists();
		}
		return success;
//		return LoadFromFile(*fp);
	}
	if (fp) fp->SeekToBegin();
	if (fp)
	{
		CFile & file = *fp;
//					Alert0("hi");
		int bytesavailable = file.GetLength();

			
		if (AnaVisionImageFileHyperStack::IsAnavisionHyperStack(bytesavailable, file)){
//			Alert0("the file is a AnaVisionImageFileHyperStack");
			
			file.SeekToBegin();

			if (ReadAnaVisionFileFormat(bytesavailable, file)){

//				Alert0("hi");
				if (p.xsizeimage<1){
					Alert0("Error : xsizeimage <1 ");
					return false;
				}

				if (p.ysizeimage<1){
					p.xsizeimage = p.ysizeimage = 0;
					Alert0("Error : ysizeimage <1 ");
					return false;
				}
			}
			AllocCellLists();
			return true;
		}
		file.SeekToBegin();
	}


	if (fp) fp->SeekToBegin();
/*	int bytesavailable;
	if (AnaVisionImageFileHyperStack::IsAnavisionHyperStack(bytesavailable, *fp)){
		fp->SeekToBegin();
		FileMode = 5;
		if (!HS.ReadHyperStackFromFile(bytesavailable, *fp)){
			Alert0("could not read anavision hyperstack");
			return false;
		}
		Beep(1000,100); //Alert0("Read file");
		this->NImages = HS.Stacks.size();
	
		images = new ImageSequence[NImages];
		for (int i=0; i<this->NImages; i++){
			images[i].FileNamesAreRelative = FileNamesAreRelative;
			images[i].RootName = rootpath;
			images[i].AS = &(HS.Stacks[i]);
			images[i].FileMode = 5;
			images[i].CurrentIndex = 0;
			images[i].FileName =  fp->GetFileName();
			images[i].FirstIndex = 0;
			images[i].LastIndex = HS.Stacks[i].Images.size()-1;
		}//
		this->ImageIndex = 0;

		p.xsizeimage = HS.Width;
		p.ysizeimage = HS.Height;

		CalcMaxNumberOfImages();
		if (false){
			images = NULL;
			this->NImages = 0;
			return false;
		}
		return true;
	}
	if (fp) fp->SeekToBegin();
*/

	if (EXT.Compare("PST") == 0){
		FileNamesAreRelative = false;
		FileMode = 1;
		NImages = 1;
		images = new ImageSequence[NImages];
		uint64 lengthfile = fp->GetLength();
		ImageSequence & IS = images[0];
	
		IS.CurrentIndex = 0;
		FirstFileName = Name;
		IS.FileName = Name;
		char tit[1000];
		GetFileTitle(Name, tit, 1000);
		IS.Title = CString(tit);

//		Alert0(IS.FileName);
		ImageIndex = 0;
//		Alert0(FirstFileName);
//		IS.Extension = Ext;
		IS.FirstIndex = 0;
		IS.FileMode = 1;
		int width, height;


		if (GetWidthAndHeightFromInfFile(IS.FileName, width, height)){
			p.xsizeimage = width;
			p.ysizeimage = height;
			images[0].Sizex =  width;
			images[0].Sizey = height;
		}
		else{
		}

		uint64 lengthonefile = p.xsizeimage*p.ysizeimage*2;
		uint64 nimage = lengthfile/lengthonefile;
		IS.LastIndex = int(nimage-1);
		IS.RootName = "";
		IS.FileNamesAreRelative=false;

		p.TryLoadLookupTable(0);

		IS.initialized = true;
		CalcMaxNumberOfImages();
		AllocCellLists();
		return true;
	}
	if (EXT.Compare("VWS") == 0){
		bool result = ReadVwsFile(*fp);
		if (result){
			CalcMaxNumberOfImages();
			AllocCellLists();
		}
		return result;

	}
	if (EXT.Compare("SIS") == 0){
		fp->Close();
		ImageIndex = 0;

		bool result = OpenSisFile(Name);
		if (!result) return false;
		if (this->NImages<1) return false;
		p.xsizeimage = images[0].Sizex;
		p.ysizeimage = images[0].Sizey;

		CalcMaxNumberOfImages();
		AllocCellLists();
		return true;

	}
	if ((EXT.Compare("TIF") == 0)|| (EXT.Compare("TIFF") == 0)){
		if (fp){
			Alert0("Please open a tiff file with the command File->Open Tiff or File-> Open Standard Tiff");
			return false;
		}
		if (fp) fp->Close();
//		return false;
		ImageIndex = 0;
//		Alert0(Name);
		bool result = false;
		if (IsStandardTif){
			result = OpenStandardTifFile(Name);
		}
		else{
			result = OpenTifFile(Name);
		}
		if (!result) return false;


		if (this->NImages<1) return false;
		p.xsizeimage = images[0].Sizex;
		p.ysizeimage = images[0].Sizey;

//		AnaTif.CloseFirstFileTemporarily();
//		file.Open(Name, CFile::modeRead);
		CalcMaxNumberOfImages();
		AllocCellLists();
		return true;
	}

	return false;
//	return AssignFileNameAndIndexesTxtFiles(Name);
}
void DataFile::AllocCellLists(){
	for (int i=0; i<this->NImages; i++){
		images[i].celllistarray.resize(images[i].LastIndex-images[i].FirstIndex+1);
	}
}

void DataFile::CalcMaxNumberOfImages(){
	MaxNumberOfImages = 0;
	for (int i=0; i<this->NImages; i++){
		int i1 = this->images[i].FirstIndex;
		int i2 = this->images[i].LastIndex;
		int NIM = i2-i1+1;
		if (NIM>MaxNumberOfImages){
			MaxNumberOfImages = NIM;
		}
	}
//	ShowFloat(MaxNumberOfImages, "MAXN");

	AllocOffsetArrays();

	for (int i=0; i<MaxNumberOfImages; i++){
		OffsetX[i]=OffsetY[i]=0;
	}
}

int DataFile::ScanVwsFile(CFile & file, bool storevalues){
	int NI=0;
	while (true){
		uint64 IMPULSpos;
		if (!FindNextStringForward(file, "IMPULS", IMPULSpos)){
			break;
		}
		file.Seek(IMPULSpos+1, CFile::begin);
		uint64 SlashPos;
//		ShowFloat(IMPULSpos, "IMPULSpos");
		if (!FindNextStringForward(file, "\\", SlashPos)){
			break;
		}
//		file.Seek(SlashPos+1, CFile::begin);
//		ShowFloat(SlashPos, "slashpos");
/*
		char filx[10000];
		file.Read(&filx[0], 5);
		filx[5]=0;
		CString xxname = CString(filx);
		Alert0(xxname);
*/
		uint64 Null1Pos;
		char nullchar = 0;
//		CString FindStrn
		if (!FindNextStringForward(file, nullchar, Null1Pos)){
			break;
		}
//		ShowFloat(Null1Pos, "Null1Pos");
		file.Seek(SlashPos+1, CFile::begin);
		char fil[10000];
		int l = int(Null1Pos-SlashPos-1);
		if (l>=10000){
			break;
		}
		if (l>0){
//		ShowFloat(l, "l");
		file.Read(&fil[0], l);
		fil[l]=0;
		CString name = CString(fil);

	//	Alert0(name);
		uint64 stylespos;
		if (!FindNextStringBackward(file, "Styles", stylespos)){
			break;
		}
		stylespos -= 3;
		file.Seek(stylespos-1, CFile::begin);
		uint64 StartTitlePos;
		if (!FindNextStringBackward(file, nullchar, StartTitlePos)){
			break;
		}
		file.Seek(StartTitlePos+2, CFile::begin);
		l=int(stylespos-StartTitlePos-2);
		if(l<1) l=1;
		if (l>=10000){
			break;
		}
//		ShowFloat(l, "l");
		file.Read(&fil[0], l);
		fil[l]=0;
		CString title = CString(fil);
//		Alert0(title);
		file.Seek(Null1Pos, CFile::begin);
		if (storevalues){
			ImageSequence & IS = images[NI];
			IS.FileName = name;
			IS.Title = title;

			CString FileName = IS.RootName+IS.FileName;
//			Alert0(FileName);
			CFileStatus rStatus;
			if (!CFile::GetStatus( FileName, rStatus )){
//				break;
				Alert0("Coul;d not get status");
			}
			uint64 lengthfile = rStatus.m_size;

			IS.CurrentIndex = 0;
			IS.FirstIndex = 0;
			IS.FileMode = 1;

			int width, height;

			if (GetWidthAndHeightFromInfFile(FileName, width, height)){
				p.xsizeimage = width;
				p.ysizeimage = height;
				IS.Sizex =  width;
				IS.Sizey = height;
			}
			else{
				IS.Sizex =  p.xsizeimage;
				IS.Sizey = p.ysizeimage;

			}

//			UINT lengthonefile = p.xsizeimage*p.ysizeimage*2;
			uint64 lengthonefile = IS.Sizex*IS.Sizey*2;
			uint64 nimage = lengthfile/lengthonefile;
			IS.LastIndex = int(nimage-1);
			IS.initialized = true;

		}

		NI++;
		} // end if l>0

	}
	return NI;
}


bool DataFile::ReadVwsFile(CFile & file){

	file.SeekToBegin();

	this->ImageIndex = 0;
//	this->FirstFileName=file.GetFilePath();
	this->FirstFileName=file.GetFileTitle();
//	Alert0(FirstFileName);
//	this->rootpath = FirstFileName;
	this->rootpath = file.GetFilePath();
	this->Extension = "VWS";
	rootpath = rootpath.Left(rootpath.GetLength()-3)+"pst/";
	this->FileNamesAreRelative = true;

	this->NImages = ScanVwsFile(file, false);

//	ShowFloat(NImages, "NI");
	if (NImages==0){
		return false;
	}

	images = new ImageSequence[NImages];
	for (UINT i=0; i<NImages; i++){
		images[i].FileNamesAreRelative = FileNamesAreRelative;
		images[i].RootName = rootpath;
		images[i].FileMode = 1;

	}

	file.SeekToBegin();
	ScanVwsFile(file, true);
	FileType=0;

	return true;

}


CString ImageSequence::GetNewFileName(CString name){
	if (name.GetLength()<5) {
		name = name + "XXXX";
	}
	while (true){
		CString name1 = name;
		name1 = name1.Left(name1.GetLength()-4) + "_c1.pst";
		{	
			CFile f1;
			CString filename = name1;
			if (this->FileNamesAreRelative){
				filename = RootName+name1;
			}
//			Alert0(filename);
			if (!f1.Open(filename, CFile::modeRead)){
				return name1;
			}
		}
		name = name1;
	}
}

bool DataFile::CopyImage(int index){ // puts the image at the end
	if (index<0) return false;
	if (index>=int(NImages)) return false;

	ImageSequence * newimages = new ImageSequence[NImages+1];

	unsigned int i;
	for (i=0; i<NImages; i++){
		newimages[i] = images[i];
	}

	newimages[NImages] = images[index];

	for (i=0; i<NImages; i++){
		images[i].pp = NULL;
	}

	delete [] images;

	images = newimages;
	NImages++;

	ImageSequence & im = images[NImages-1];
	CString oldfilename = im.GetFileName();

	im.FileName = im.GetNewFileName(im.FileName);
	CString newfilename = im.GetFileName();

//	CString c="pause hi \r\n copy "+oldfilename+" "+newfilename+"\r\n\ pause hi";
	CFile f1;
	if (!f1.Open(oldfilename, CFile::modeRead)){
		Alert0("The directory or files of the image data was not found");
		return false;
	}
	CFile f2;
//	Alert0(newfilename);
	if (!f2.Open(newfilename, CFile::modeCreate | CFile::modeWrite)){
		Alert0("could not open outfile");
		return false;
	}

	BYTE block[10000];

	unsigned __int64 rest=f1.GetLength();
	unsigned __int64 read = 10000;

	while (rest>0){
		if (read>rest) read = rest;
		f1.Read(block, UINT(read));
		f2.Write(block, UINT(read));
		rest -= read;
	}
//	CString c="copy "+oldfilename+" "+newfilename;
//	ShowFloat(system(c), "sys");;
//	Alert0(c);

	return true;
}

bool ImageSequence::AssignFileNameAndIndexesTxtFiles(CString Name){
	return false;
}



bool DataFile::GetWidthAndHeightFromInfFile(CString PSTFileName, int & width, int & height){
	CString InfFileName = PSTFileName;
	int dotpos = InfFileName.ReverseFind('.');
	if (dotpos>=0){
		InfFileName = InfFileName.Left(dotpos);
	}
	InfFileName += ".inf";

	CString lpAppName = "Info";
	CString lpDefault = "0";
	CString lpKeyName ="Width";

	char s[1000];
	s[0] = 0;
	DWORD nbytes = GetPrivateProfileString(
				lpAppName,        // points to section name
				lpKeyName,        // points to key name
				lpDefault,        // points to default string
				s,  // points to destination buffer
				1000,               // size of destination buffer
				InfFileName        // points to initialization filename
	);
//	Alert0(CString(s));
	if (nbytes < 1 ){
		return false;
	}
	width = atoi(s);

	if (width<1) return false;

	s[0] = 0;
	lpKeyName ="Height";
	nbytes = GetPrivateProfileString(
				lpAppName,        // points to section name
				lpKeyName,        // points to key name
				lpDefault,        // points to default string
				s,  // points to destination buffer
				1000,               // size of destination buffer
				InfFileName        // points to initialization filename
	);
	if (nbytes < 1 ){
		return false;
	}
	height = atoi(s);

	if (height<1) return false;
	
//	Alert0(CString(s));

	return true;
}

void DataFile::DoCalcRatio(__int16 * da, __int16 * db, int Sizex, int Sizey, Params & p, __int16 *dc){
	for (int ix=0; ix<Sizex; ix++){
		for (int iy=0; iy<Sizey; iy++){
//			__int64 i=iy*Sizex+ix;
//			double Valuea=da[i]-p.BackgroundA;
//			double Valueb=db[i]-p.BackgroundB;
			double Valuea=0.0;
			double Valueb=0.0;
			int Nav=0;
			int jxa=ix-p.NSurroundForAverage;
			if (jxa<0){
				jxa=0;
			}
			int jxb=ix+p.NSurroundForAverage;
			if (jxb>=Sizex){
				jxb=Sizex-1;
			}
			int jya=iy-p.NSurroundForAverage;
			if (jya<0){
				jya=0;
			}
			int jyb=iy+p.NSurroundForAverage;
			if (jyb>=Sizey){
				jyb=Sizey-1;
			}
			for (int jx=jxa; jx<=jxb; jx++){
				for (int jy=jya; jy<=jyb; jy++){
					Nav++;
					__int64 j=jy*Sizex+jx;
					Valuea += (double(da[j])-p.BackgroundA);
					Valueb += (double(db[j])-p.BackgroundB);
				}
			}
			if (Nav>0){
				dc[iy*Sizex+ix]=__int16 ( Valuea*p.ScaleRatio/Valueb );
			}
		}

	}

}

bool DataFile::DoAverage(int indexa, int indexb, int indexc, CString newtitle){
	CWaitCursor dummy;

//	ShowFloat(indexc, "indexc");
//	ShowFloat(indexa, "indexa");
//	ShowFloat(indexb, "indexb");
//	ShowFloat(NImages, "NImages");

	if (indexa<0){
//		Alert0("Hi1");
		return false;
	}
	if (indexa>=int(NImages)){
//		Alert0("Hi2");
		return false;
	}
	if (indexb<0){
//		Alert0("Hi3");
		return false;
	}
	if (indexb>=int(NImages)){
//		Alert0("Hi4");
		return false;
	}
	if ( (indexc>0) && (indexc>=int(NImages))){
//		Alert0("Hi5");
		return false;
	}

	if (indexc==indexa){
//		Alert0("Hi6");
		return false;
	}
	if (indexc==indexb){
//		Alert0("Hi7");
		return false;
	}

	int nimagetoratioa = images[indexa].LastIndex-images[indexa].FirstIndex+1;
	int nimagetoratiob = images[indexb].LastIndex-images[indexb].FirstIndex+1;
	int nimagetoratio;
//	Alert0("Hi0");

	if (nimagetoratioa>nimagetoratiob){
	  nimagetoratio = nimagetoratiob;
	}
	else{
	  nimagetoratio = nimagetoratioa;
	}
	if (nimagetoratio <1){
		Alert0("nimagetoratio <1");
		return false;
	}

	if (indexc<0){
//		Alert0("hi");
		ImageSequence * newimages = new ImageSequence[NImages+1];

		UINT i;
		for (i=0; i<NImages; i++){
			newimages[i] = images[i];
		}

		newimages[NImages] = images[indexa];

		for (i=0; i<NImages; i++){
			images[i].pp = NULL;
		}

		delete [] images;

		images = newimages;
		indexc = NImages;
		NImages++;
		images[indexc].Title = newtitle;
		images[indexc].FileName = images[indexa].FileName;

	}
	else{

	}

	images[indexc].Sizex = images[indexa].Sizex;
	images[indexc].Sizey = images[indexa].Sizey;

	images[indexc].FirstIndex = 0;
	images[indexc].LastIndex = nimagetoratio-1;
	images[indexc].FileMode = 1;	// 0 = several txt files
					// 1 = one pst file

	CString FileName;

	images[indexc].CurrentIndex = 0;
	images[indexc].initialized = true;
	images[indexc].imageready = false;
	images[indexc].FileNamesAreRelative = images[indexa].FileNamesAreRelative;
	images[indexc].RootName = images[indexa].RootName;;
//	int treeid; ???????????
	images[indexc].pp = images[indexa].pp;
	
	ImageSequence & ima = images[indexa];
	ImageSequence & imb = images[indexb];
	ImageSequence & imc = images[indexc];

//	ShowFloat(p.NSurroundForAverage, "ima.pp->NSurroundForAverage");


	CString filenamea = ima.GetFileName();
	CString filenameb = imb.GetFileName();

	CFile fa;
	if (!fa.Open(filenamea, CFile::modeRead)){
		Alert0("The directory or files of the image data was not found - A");
		return false;
	}
	CFile fb;
	if (!fb.Open(filenameb, CFile::modeRead)){
		Alert0("The directory or files of the image data was not found - B");
		return false;
	}
	CString oldfilename = imc.FileName;

	imc.FileName = imc.GetNewFileName(imc.FileName);

	CString filenamec = imc.GetFileName();

	CFile fc;

//	Alert0(filenamec);


//	Alert0(newfilename);
	if (!fc.Open(filenamec, CFile::modeCreate | CFile::modeWrite)){
		Alert0("could not open outfile");
		imc.FileName = oldfilename;
		return false;
	}

	int sizeimage = ima.Sizex*ima.Sizey;

	__int16 * da = new __int16[sizeimage];
	__int16 * db = new __int16[sizeimage];
	__int16 * dc = new __int16[sizeimage];

	for (int i=0; i<nimagetoratio; i++){
		int start = i*sizeimage*2;
		fa.Seek(start, CFile::begin);
		fa.Read(da, 2*sizeimage);
		fb.Seek(start, CFile::begin);
		fb.Read(db, 2*sizeimage);
		DoCalcRatio(da, db, ima.Sizex, ima.Sizey, p, dc);
		fc.Write(dc, 2*sizeimage);
	}


	delete [] dc;
	delete [] db;
	delete [] da;

	return true;
}


bool DataFile::OpenSisFile(CString & FileName){
#if defined SIMPLE_IMAGE_CORE
//	bool DoShow = true;
	bool DoShow = false;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	arivis::simpleimagecore::SimpleImageStack * imageStack=	pApp->Core->openImageStack(FileName);
	SISStack = imageStack;

	if (!imageStack){
		Alert0("could not open stack");
		return false;
	}

	int channelcount = imageStack->getChannelCount();

	arivis::simpleimagecore::Rectangle rect = 	imageStack->getBounds();
//	ShowFloat(rect.height, "height");
//	ShowFloat(rect.width, "width");

	int NFrames = imageStack->getFrameCount();
//	ShowFloat(NFrames, "NFrames");

	int NPlanes = imageStack->getPlaneCount();
//	ShowFloat(NPlanes, "NPlanes");

	arivis::simpleimagecore::PixelType pT = imageStack->getPixelType();
//	ShowFloat(pT, "pT");

	uint64 NChan = channelcount;
	
	if (DoShow) ShowFloat(NChan, "NChan");

	uint64 NTick = NFrames ;
	if (DoShow) ShowFloat(NTick, "NTick");

	uint64 width = rect.width;
	if (DoShow) ShowFloat(width, "width");

	uint64 height = rect.height;
	FileType = 1;


	if (NChan<1) return false;

	this->ImageIndex = 0;
	GetFileName(FileName, this->FirstFileName);
	GetPath(FileName, this->rootpath);

//	Alert0(this->rootpath);
	this->Extension = "SIS";
//	rootpath = rootpath.Left(rootpath.GetLength()-3)+"pst/";
	this->FileNamesAreRelative = true;
	NImages = UINT(NChan);

	images = new ImageSequence[NImages];
	for (unsigned int i=0; i<NImages; i++){
		images[i].SISStack = imageStack;
		images[i].FileName = this->FirstFileName;
		images[i].FileNamesAreRelative = true;
		images[i].RootName = rootpath;
		images[i].FileMode = 2; // SIS file
		images[i].Sizex = int(width);
		images[i].Sizey = int(height);
		images[i].FirstIndex = 0;
		images[i].LastIndex = int(NTick)-1;
	}
	this->FileOffset = -999; // to indicate that the sis file is read with arivis lib
	return true;
#else
	return false;
#endif
}

bool DataFile::OpenTifFile(CString & FileName){
//	bool DoShow = true;
	bool DoShow = false;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	if (!AnaTif.OpenFile(FileName)){
		return false;
	}

	int channelcount = AnaTif.GetNumberOfChannels();

//	arivis::simpleimagecore::Rectangle rect = 	imageStack->getBounds();
//	ShowFloat(rect.height, "height");
//	ShowFloat(rect.width, "width");

	__int32 w, h;
	AnaTif.GetImageSize(w, h);

	int NFrames = AnaTif.GetNumberOfImagesPerChannel();
//	ShowFloat(NFrames, "NFrames");

	uint64 NChan = channelcount;
	
	if (DoShow) ShowFloat(NChan, "NChan");

	uint64 NTick = NFrames ;
	if (DoShow) ShowFloat(NTick, "NTick");

	uint64 width = w;
	if (DoShow) ShowFloat(width, "width");

	uint64 height = h;
	FileType = 2;

	if (NChan<1) return false;

	this->ImageIndex = 0;
	GetFileName(FileName, this->FirstFileName);
	GetPath(FileName, this->rootpath);

//	Alert0(this->rootpath);
	this->Extension = "TIF";
//	rootpath = rootpath.Left(rootpath.GetLength()-3)+"pst/";
	this->FileNamesAreRelative = true;
	NImages = UINT(NChan);

	images = new ImageSequence[NImages];
	for (unsigned int i=0; i<NImages; i++){

		images[i].ptrTiff = &(AnaTif);
		images[i].FileName = this->FirstFileName;
		images[i].FileNamesAreRelative = true;
		images[i].RootName = rootpath;
		images[i].FileMode = 3; // TIFF file
		images[i].Sizex = int(width);
		images[i].Sizey = int(height);
		images[i].FirstIndex = 0;
		images[i].LastIndex = int(NTick)-1;
		images[i].initialized = true;
	}
	this->FileOffset = -9999; // to indicate that the sis file is read with AnaTif
//	Alert0("hi");
	return true;
}


OverlayClass::OverlayClass(){
	NChannelsToOverlay = 0;
	for (int i=0; i<1000; i++){
		OverlayThis[i]=false;
		OffsetOfThis[i].x=OffsetOfThis[i].y=0;
	}
}

OverlayClass::~OverlayClass(){
}

bool DataFile::DoOpenTifFile(CString Name){
//	Alert0(Name);
	CMainFrame* frame = (CMainFrame *)(AfxGetMainWnd( ));

	int l = Name.GetLength(); 

	Converted = true;

	
	CString Ext = Name.Right(3);

	CString EXT = Ext;
	EXT.MakeUpper();

//	initialized = false;

	ImageIndex = 0;
//	Alert0(Name);
	bool result = OpenTifFile(Name);
	if (!result) return false;


	if (this->NImages<1) return false;
	p.xsizeimage = images[0].Sizex;
	p.ysizeimage = images[0].Sizey;
//	initialized = true;

//	Alert0("Hi");
	return true;

}


void DataFile::GetOffset(int & offsetx, int & offsety){
	offsetx = offsety = 0;
	if (!images) return;
	int index = images[ImageIndex].CurrentIndex;
	//ShowFloat(index, "index");
	offsetx = OffsetX[index];
	offsety = OffsetY[index];
	//ShowFloat(offsetx, "x");
//	ShowFloat(offsety, "y"); 
}

void DataFile::IncOffset(int offsetx, int offsety){
	if (!images) return;
	int index = images[ImageIndex].CurrentIndex;
	IncOffset(index, offsetx, offsety);
}
void DataFile::IncOffset(int startindex, int offsetx, int offsety){
	if (!images) return;
	for (int i=startindex; i<MaxNumberOfImages; i++){
		OffsetX[i] += offsetx;
		OffsetY[i] += offsety;
	}
}

void DataFile::ClearAllOffsets(){
	if (!images) return;
	for (int i=0; i<MaxNumberOfImages; i++){
		OffsetX[i] = 0;
		OffsetY[i] = 0;
	}
}

bool DataFile::OpenStandardTifFile(CString & FileName){
//	bool DoShow = true;
	bool DoShow = false;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	if (!AnaTif.OpenStandardTif(FileName)){
		return false;
	}

	int channelcount = AnaTif.GetNumberOfChannels();

//	arivis::simpleimagecore::Rectangle rect = 	imageStack->getBounds();
//	ShowFloat(rect.height, "height");
//	ShowFloat(rect.width, "width");

	__int32 w, h;
	AnaTif.GetImageSize(w, h);
//	ShowFloat(w, "w");
//	ShowFloat(h, "h");

	int NFrames = AnaTif.GetNumberOfImagesPerChannel();
//	ShowFloat(NFrames, "NFrames");

	uint64 NChan = channelcount;
	
	if (DoShow) ShowFloat(NChan, "NChan");

	uint64 NTick = NFrames ;
	if (DoShow) ShowFloat(NTick, "NTick");

	uint64 width = w;
	if (DoShow) ShowFloat(width, "width");

	uint64 height = h;
	FileType = 2;

	if (NChan<1) return false;

	this->ImageIndex = 0;
	GetFileName(FileName, this->FirstFileName);
	GetPath(FileName, this->rootpath);

//	Alert0(this->rootpath);
	this->Extension = "TIF";
//	rootpath = rootpath.Left(rootpath.GetLength()-3)+"pst/";
	this->FileNamesAreRelative = true;
	NImages = UINT(NChan);

	images = new ImageSequence[NImages];
	for (unsigned int i=0; i<NImages; i++){

		images[i].ptrTiff = &(AnaTif);
		images[i].FileName = this->FirstFileName;
		images[i].FileNamesAreRelative = true;
		images[i].RootName = rootpath;
		images[i].FileMode = 3; // TIFF file
		images[i].Sizex = int(width);
		images[i].Sizey = int(height);
		images[i].FirstIndex = 0;
		images[i].LastIndex = int(NTick)-1;
		images[i].initialized = true;
	}
	this->FileOffset = -9999; // to indicate that the sis file is read with AnaTif
//	Alert0("hi");
	return true;
}


void DataFile::OnFileSaveimagesinanavisionformat(int & byteswritten, ImageData & id){

	if (!images){
		Alert0("no data in DataFile::OnFileSaveimagesinanavisionformat");
		return;
	}
	
	CString Name = images->FileName+".AnaVision";

	CFileDialog
		   diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
	diag.m_ofn.lpstrTitle = "Save images in Anavision format";
	if (diag.DoModal()!= IDOK){
		return;
	}
	Name = diag.GetPathName();
//	Alert0(Name);

	AnaVisionImageFileHyperStack HS;
	HS.Width = images[0].Sizex;
	HS.Height = images[0].Sizey;

	for (int i=1; i<this->NImages; i++){
		if (HS.Width != images[i].Sizex){
			Alert0("not all stacks have the same size in DataFile::OnFileSaveimagesinanavisionformat");
			return;
		}
		if (HS.Height != images[i].Sizey){
			Alert0("not all stacks have the same size in DataFile::OnFileSaveimagesinanavisionformat");
			return;
		}
	}
	CWaitCursor dummy;
	int SaveImageIndex = ImageIndex;
//	ShowFloat(this->NImages, "this->NImages");
	for (int i=0; i<this->NImages; i++){
		this->ImageIndex = i;
		AnaVisionImageFileStack AS;
		AS.Width = HS.Width;
		AS.Height = HS.Height;
		ImageSequence & image = images[i];
		int SaveCurrentIndex = image.CurrentIndex;
		AS.Title = image.Title;
//		ShowFloat(image.FirstIndex, "image.FirstIndex");
//		ShowFloat(image.LastIndex, "image.LastIndex");
		for (int j=image.FirstIndex; j<=image.LastIndex; j++){
//			ShowFloat(j, "j");
			AnaVisionImageFile AF;
			AF.Width=AS.Width;
			AF.Height=AS.Height;
//			ShowFloat(AF.Height, "h");
			AF.data.resize(AF.Width*AF.Height);
			CellsClass * Cells;
			this->ReadImage(j, id, Cells, false, false);
			int d=0;
			for (int y=0; y<AF.Height; y++){
				for (int x=0; x<AF.Width; x++){
					AF.data[d]= id.data[x][y];
					//if ((x==20) && (y==20)) ShowFloat(AF.data[d] , "AF.data[d]");
					d++;
				}
			}
			AS.AppendImage(AF);
		}
		image.CurrentIndex=SaveCurrentIndex;
		HS.AppendStack(AS);
	}
	ImageIndex = SaveImageIndex;
	CellsClass * Cells;
	this->ReadImage(ImageIndex, id, Cells, false, false);

	CFile file;
	file.Open(Name, CFile::modeCreate | CFile::modeWrite);
	
	if (!HS.WriteHyperStackToFile(byteswritten, file)){
		Alert0("error writing file");
	}
}

bool DataFile::ReadAnaVisionFileFormat(int & bytesavailable, CFile & file){
//	Alert0(Name);
	CMainFrame* frame = (CMainFrame *)(AfxGetMainWnd( ));

/*	CFile file;
	if (!file.Open(FileName, CFile::modeRead)){
		Alert0("cannot open file in DataFile::ReadAnaVisionFileFormat");
		return false;
	}

	bytesavailable = file.GetLength();

	if (!AnaVisionImageFileHyperStack::IsAnavisionHyperStack(bytesavailable, file)){
		Alert0("the file is not a AnaVisionImageFileHyperStack");
		return false;
	}
	file.SeekToBegin();
	bytesavailable = file.GetLength();
*/
//	FileMode = 5;
	if (!HS.ReadHyperStackFromFile(bytesavailable, file)){
		Alert0("could not read anavision hyperstack");
		return false;
	}
//	Beep(1000,100); //Alert0("Read file");
	this->NImages = HS.Stacks.size();
	
	images = new ImageSequence[NImages];
	for (int i=0; i<this->NImages; i++){
		images[i].Sizex = HS.Width;
		images[i].Sizey = HS.Height;
		images[i].Title = HS.Stacks[i].Title;
		images[i].FileNamesAreRelative = FileNamesAreRelative;
		images[i].RootName = rootpath;
		images[i].AS = &(HS.Stacks[i]);
		images[i].FileMode = 5;
		images[i].CurrentIndex = 0;
		images[i].FileName =  file.GetFileName();
		images[i].FirstIndex = 0;
		images[i].LastIndex = HS.Stacks[i].Images.size()-1;
		images[i].initialized = true;

	}//
	this->ImageIndex = 0;
	p.xsizeimage = HS.Width;
	p.ysizeimage = HS.Height;

	CalcMaxNumberOfImages();
	if (false){
		images = NULL;
		this->NImages = 0;
		return false;
	}
	ClearAllOffsets();
//	ShowFloat(OffsetX[0], "OffsetX[0]");
	this->FileType = 5;
	this->AllocCellLists();

	
	return true;
}


void DataFile::OnFileSavethisstackinanavisionformat(int & byteswritten,
										  ImageData & id,
										  int stack, int  from, int to){
	
	if (stack<0){
		Alert0("stack<0 in DataFile::OnFileSaveimagesinanavisionformat");
		return;
	}
	
	if (stack>=this->NImages){
		Alert0("stack>=this->NImages in DataFile::OnFileSaveimagesinanavisionformat");
		return;
	}

	if (!images){
		Alert0("no data in DataFile::OnFileSaveimagesinanavisionformat");
		return;
	}
	
	CString Name = images->FileName+".AnaVision";

	CFileDialog
		   diag( false, NULL, Name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
	diag.m_ofn.lpstrTitle = "Save images in Anavision format";
	if (diag.DoModal()!= IDOK){
		return;
	}
	Name = diag.GetPathName();
//	Alert0(Name);

	AnaVisionImageFileHyperStack HS;
	HS.Width = images[stack].Sizex;
	HS.Height = images[stack].Sizey;

	CWaitCursor dummy;
	int SaveImageIndex = ImageIndex;
//	ShowFloat(this->NImages, "this->NImages");
	for (int i=stack; i<=stack; i++){
		this->ImageIndex = i;
		AnaVisionImageFileStack AS;
		AS.Width = HS.Width;
		AS.Height = HS.Height;
		ImageSequence & image = images[i];
		int SaveCurrentIndex = image.CurrentIndex;
		AS.Title = image.Title;
//		ShowFloat(image.FirstIndex, "image.FirstIndex");
//		ShowFloat(image.LastIndex, "image.LastIndex");
//		ShowFloat(image.FirstIndex, "image.FirstIndex");
//		ShowFloat(image.LastIndex, "image.LastIndex");
		if (from<image.FirstIndex){
			Alert0("from<image.FirstIndex in DataFile::OnFileSaveimagesinanavisionformat");
		}
		if (to>image.LastIndex){
			Alert0("to>image.LastIndex in DataFile::OnFileSaveimagesinanavisionformat");
		}
		if (from>to){
			Alert0("from>to in DataFile::OnFileSaveimagesinanavisionformat");
		}
//		for (int j=image.FirstIndex; j<=image.LastIndex; j++){
		for (int j=from; j<=to; j++){
//			ShowFloat(j, "j");
			AnaVisionImageFile AF;
			AF.Width=AS.Width;
			AF.Height=AS.Height;
//			ShowFloat(AF.Height, "h");
			AF.data.resize(AF.Width*AF.Height);
			CellsClass * Cells;
			this->ReadImage(j, id, Cells, false, false);
			int d=0;
			for (int y=0; y<AF.Height; y++){
				for (int x=0; x<AF.Width; x++){
					AF.data[d]= id.data[x][y];
					//if ((x==20) && (y==20)) ShowFloat(AF.data[d] , "AF.data[d]");
					d++;
				}
			}
			AS.AppendImage(AF);
		}
		image.CurrentIndex=SaveCurrentIndex;
		HS.AppendStack(AS);
	}
	ImageIndex = SaveImageIndex;
	CellsClass * Cells;
	this->ReadImage(ImageIndex, id, Cells, false, false);

	CFile file;
	file.Open(Name, CFile::modeCreate | CFile::modeWrite);
	
	if (!HS.WriteHyperStackToFile(byteswritten, file)){
		Alert0("error writing file");
	}
}

bool BlobPoints::PointIsInBlob(int x, int y){
	for (int i=0; i<points.size(); i++){
		CPoint & point = points[i].point;
		if ((x==point.x) && (y==point.y)){
			return true;
		}
	}
	return false;
}
bool BlobPoints::PointIsInCell(int index, int x, int y){
	if (NCells<2){
		return PointIsInBlob(x, y);
	}
	for (int i=0; i<points.size(); i++){
		CellPointClass & point = points[i];
		if ((index == point.cellindex) && (x==point.point.x) && (y==point.point.y)){
			return true;
		}
	}
	return false;

}
int BlobPoints::GetNNeighbors(int i, int SX, int SY){
	int n = 0;
	CPoint & point = points[i].point;
	int x = point.x;
	int y = point.y;
	for (int dx=-1; dx<2; dx++){
		for (int dy=-1; dy<2; dy++){
			if ((dx != 0) || (dy !=0)){
				int newx = x+dx;
				int newy = y+ dy;
				if ( (newx>=0) && (newx<SX) && (newy>=0) && (newy<SY) ){
					if (PointIsInBlob(newx, newy)){
						n++;
					}
				}
			}
		}
	}
	//ShowFloat(n, "n");
	return n;
}


CPoint BlobPoints::FindBorderPointOfBlob(int SX, int SY){
	for (int i=0; i<points.size(); i++){
		if (GetNNeighbors(i, SX, SY)<8){
			return points[i].point;
		}
	}
	Alert0("no border point found");
	return CPoint(0,0);

}

bool BorderPoints::CornerIsInThis(BorderPoint & tp){
	for (int i=0; i<points.size(); i++){
		BorderPoint & p = points[i];
//		ShowFloat(p.Corner,"p.C");
//		ShowFloat(tp.Corner,"tp.C");
		if ((p.x == tp.x) && (p.y==tp.y) && (p.Corner==tp.Corner)){
			return true;
		}
	}
	return false;
}


void BlobPoints::FillNeighbours(int x, int y, bool (*nb) [3][3]){
	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){
			(*nb)[i][j] = !PointIsInBlob(x+i-1, y+j-1);
/*			char s[20];
			CString c;
			_itoa(i, s, 10);
			c =  " i "+ CString(s);
			_itoa(j, s, 10);
			c +=  "  "+ CString(s);
			if ((*nb)[i][j]){
				c += " empty";
			}
			else{
				c += " occ ";
			}
			Alert0(c);
*/
		}
	}
}

#define PUSH_CORNER  { /* ShowFloat(p.Corner, "push C");*/ if (BlobBorder.CornerIsInThis(p)){  /* Alert0("finish"); */ return; 	} ; points.push_back(p);}

void BlobPoints::CalculateBlobBorder(int SX, int SY){

	CPoint FirstPoint = FindBorderPointOfBlob(SX, SY);
//	ShowFloat(BorderPoint.x, "x");
	vector <BorderPoint> & points = this->BlobBorder.points;
//	points.resize(1);
	BorderPoint p; 
	p.x = FirstPoint.x;
	p.y = FirstPoint.y;

	int x=p.x;
	int y=p.y;

	bool nb[3][3]; // = {false, false, false, false, false, false,false, false, false};
//	Alert0("first point");
	FillNeighbours(x, y, &nb);
	
	if (nb[1][2]) { /// top middle empty
		p.Corner=2;
		PUSH_CORNER
	}
	else{
		if (nb[2][1]){
			p.Corner = 1;
			PUSH_CORNER;
		}
		else{
			if (nb[1][0]){
				p.Corner = 4;
				PUSH_CORNER
			}
			else{
				if (nb[0][1]){
					p.Corner = 3;
					PUSH_CORNER;
				}
				else{
					if (nb[2][2]){
						p.x=x+1;
						p.Corner=2;
						PUSH_CORNER
					}
					else{
						if (nb[2][0]){
							p.x=x+1;
							p.Corner = 1;
							PUSH_CORNER
						}
						else{
							if (nb[0][0]){
								p.y = y-1;
								p.Corner = 4;
								PUSH_CORNER
							}
							else{
								if (nb[0][2]){
									p.y = y+1;
									p.Corner = 3;
									PUSH_CORNER
								}
								else{
									Alert0("ERROR IN GETTING BORDER");
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	while (true){
		BorderPoint p = points[points.size()-1];
		int x=p.x;
		int y=p.y;
/*		CString str = "enter loop, x=";
		char s[20];
		_itoa(x, s, 10);
		str += CString(s) +  ", y = ";
		_itoa(y, s, 10);
		str += CString(s) +  ", C = ";
		_itoa(p.Corner, s, 10);
		str += CString(s);
		Alert0(str);
*/		
		bool nb[3][3];
		FillNeighbours(x, y, &nb);
		int a, a2, b, c, d, C1, C2, C3, x1, y1, x2, y2;
		if (p.Corner==1){
			a=2; a2 = 2; b=1; c= 0; d=0; C1 = 4; C2 = 1; x1=x; y1=y-1; x2=x+1; y2 = y-1; C3 = 2;
		}
		if (p.Corner==2){
			a=1; a2 = 2; b=2; c=2; d =1; C1 = 1; C2 = 2; x1=x+1; y1=y; x2=x+1; y2 = y+1; C3 = 3;
		}
		if (p.Corner==3){
			a=0; a2 = 0; b=1; c=2; d =2; C1 = 2; C2 = 3; x1=x; y1=y+1; x2=x-1; y2 = y+1; C3 = 4;
		}
		if (p.Corner==4){
			a=1; a2 = 0; b=0; c=0; d =1; C1 = 3; C2 = 4; x1=x-1; y1=y; x2=x-1; y2 = y-1; C3 = 1;
		}
		if (!nb[a][b]){
			Alert0("ERROR INCONSISTENCY 1");
			return;
		}
		if (nb[a2][c]){
			if (nb[b][d]){
				p.Corner = C1;
				PUSH_CORNER
			}
			else{
				p.x = x1;
				p.y = y1;
				p.Corner = C2;
				PUSH_CORNER
			}
		}
		else{
			p.x = x2;
			p.y = y2;
			p.Corner = C3;
			PUSH_CORNER
		}
	}
}
