#include "stdafx.h"
#include "AnaVision.h"
#include <afxdlgs.h>

MeanROIArray::MeanROIArray(){
}
MeanROIArray::~MeanROIArray(){
}
//	BufferClass<double> values;
bool MeanROIArray::SaveOnFile(CFile * fp, int & byteswritten){
	int NP=values.GetSize();
	MyWriteVariable(NP);
	if (NP<1) return true;
	double * buff = (double *)(values.buff);
	for (int i=0; i<NP; i++){
		MyWriteVariable(buff[i]);
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
	values.UpdateBuffer(NP);
	if (NP<1){
		return true;
	}
	double * buff = (double *)values.buff;
	for (int i=0; i<NP; i++){
		MyReadVariable(buff[i]);
	}
	return true;
}
void MeanROIArray::CalcMinMax(double & Min, double & Max){
	int NP=values.GetSize();
	if (NP<1) return;
	double * buff = (double *)(values.buff);
	double mm=MinDouble(buff, NP);
	if (mm<Min) Min = mm;
	mm=MaxDouble(buff, NP);
	if (mm>Max) Max = mm;
}
int MeanROIArray::GetMaxImages(){
	return values.GetSize();
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
	SISStack = NULL;
}
ImageSequence::~ImageSequence(){
//	if (SISStack){
	if (false){
		CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
		pApp->Core->closeImageStack(SISStack);
		SISStack = NULL;
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
	SISStack = NULL;


}

DataFile::~DataFile(){
	if (SISStack){
		CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
		pApp->Core->closeImageStack(SISStack);
		SISStack = NULL;
	}
}

void DataFile::SetVersion(){
	Version1 = 2;
	Version1 = 3;
	Version1 = 4;
	Version1 = 5;
	Version1 = 6;
	Version1 = 7;



	Version2 = 0;
	Version3 = 0;

#define CURRENT_DATFILE_VERSION 7
}

bool DataFile::IsAnavisionFile(CFile & file) { // Tests only if the first thing in the file is the string
									// ANAVISION
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

		for (int i=0; i<NImages; i++){
				MyWriteVariable(images[i].FirstIndex);
				MyWriteVariable(images[i].LastIndex);
				MyWriteVariable(images[i].FileMode);
				MyWriteVariable(images[i].Sizex);
				MyWriteVariable(images[i].Sizey);
				pApp->MySaveString(byteswritten, fp, images[i].FileName);
				pApp->MySaveString(byteswritten, fp, images[i].Title);
		}
	}

	if (FileType==1){

		MyWriteVariable(FileOffset);

		pApp->MySaveString(byteswritten, fp, FirstFileName);

		pApp->MySaveString(byteswritten, fp, Extension);
		pApp->MySaveString(byteswritten, fp, rootpath);

		MyWriteVariable(FileNamesAreRelative);
		MyWriteVariable(NImages);

		for (int i=0; i<NImages; i++){
				MyWriteVariable(images[i].FirstIndex);
				MyWriteVariable(images[i].LastIndex);
				MyWriteVariable(images[i].FileMode);
				MyWriteVariable(images[i].Sizex);
				MyWriteVariable(images[i].Sizey);
				pApp->MySaveString(byteswritten, fp, images[i].FileName);
				pApp->MySaveString(byteswritten, fp, images[i].Title);
		}
	}

	polys.SaveOnFile(fp, byteswritten);
	roivalues.SaveOnFile(fp, byteswritten);

	int nbyteskinetics;

	nbyteskinetics = k.SaveOnFile(NULL); // setting the arg to NULL does not write
//	ShowFloat(nbyteskinetics , "nbyteskinetics ");

	MyWriteVariable(nbyteskinetics);

	byteswritten += k.SaveOnFile(fp);

//	polys.SaveOnFile(fp, byteswritten);

//	ShowFloat(byteswritten, "byteswritten");
//	ShowFloat(byteswritten, "byteswritten");

}

bool DataFile::LoadFromFile(CFile & f){
	ImageIndex = 0;
//	CurrentIndex = 0;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	CFile * fp = &(f);

	CString AnaVisionString = "ANAVISION";

	int l = f.GetLength();
	int bytesavailable = l;

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

	if (FileType==0){

		pApp->MyLoadString(bytesavailable, fp, FirstFileName);
		pApp->MyLoadString(bytesavailable, fp, Extension);
		pApp->MyLoadString(bytesavailable, fp, rootpath);

//	Alert0(FirstFileName);
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

		for (int i=0; i<NImages; i++){
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

		for (int i=0; i<NImages; i++){
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
	}

	if (Version1<3){
		SetVersion();
		return true;
	}
	if (Version1>4){
		if (bytesavailable<2){
			//Beep(1000,10);
		}
		if (!polys.LoadFromFile(&f, bytesavailable)){
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
void DataFile::OldSaveOnFile(CFile & f){

	// SAVE FIRST AN ID!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// First save p and then the rest
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

	pApp->MySaveString(byteswritten, fp, FirstFileName);

	pApp->MySaveString(byteswritten, fp, Extension);

	MyWriteVariable(images[0].FirstIndex);
	MyWriteVariable(images[0].LastIndex);
	MyWriteVariable(images[0].FileMode);
}

bool DataFile::OldLoadFromFile(CFile & f){
//	CurrentIndex = 0;
	ImageIndex = 0;

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	CFile * fp = &(f);

	CString AnaVisionString = "ANAVISION";

	int l = f.GetLength();
	int bytesavailable = l;

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
bool ImageSequence::FindTheFile(CString & Name){
	CFileDialog diag( TRUE, NULL,Name, OFN_FILEMUSTEXIST, NULL, AfxGetMainWnd());
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
	
	uint64 actuallyread = fp2->Read(b, toread );

	if (actuallyread < toread){
		Alert0("could not read 2 image");
		return false;
	}
//	Alert0("hi");
	int iter=0;
	for (unsigned __int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (unsigned __int32 x=0; x<this->Sizex; x++){
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
bool ImageSequence::OldReadSisFile(int index, CFile * fp, ImageDataType ** currentimage,
								unsigned __int64 fileoffset, UINT NChannels, UINT ChannelNumber){
	ChannelNumber = Permute(ChannelNumber, NChannels);
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

	CString name = GetFileName();
//	Alert0(name);
	if (!file.Open(name, CFile::modeRead)){
			Alert0(FileName);
			Alert0("could not open file for read");
			return false;
		}
	fp2=&file;

	unsigned __int64 size1 = 2*this->Sizex*this->Sizey;
	unsigned __int64 offset = fileoffset + uint64(size1*( NChannels*index + ChannelNumber));

	bool DoShow = false;
	char s[100];
	_itoa(fileoffset, s, 10);

	if (false) Alert0(CString(s)+CString(" fileoffset")); 
	if (DoShow) ShowFloat(offset, "offset");
	if (DoShow) ShowFloat(fp2->GetLength(), "Length");

	fp2->Seek(offset, 0L);

	__int16 * arr = new __int16[Sizex*Sizey];

	BYTE * b = (BYTE *)&(arr[0]);

	uint64 toread = 2*Sizex*Sizey;
	
	uint64 actuallyread = fp2->Read(b, toread );

	if (actuallyread < toread){
		Alert0("could not read 2 image");
		return false;
	}
	if (DoShow) Alert0("hi");
	int iter=0;
	for (unsigned __int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (unsigned __int32 x=0; x<this->Sizex; x++){
			currentimage[x][y] = arr[iter];
			iter++;
		}
	}

	delete [] arr;

	pp->Convert(currentimage, this->Sizex, this->Sizey);
	imageready = true;

	return true;
}
bool ImageSequence::ReadSisFile(int index, ImageDataType ** currentimage, UINT ChannelNumber){
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
	for (unsigned __int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (unsigned __int32 x=0; x<this->Sizex; x++){
			currentimage[x][y] = arr[iter];
			iter++;
		}
	}

	delete [] arr;

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

bool DataFile::ReadImage(int index, ImageData & id,  bool RefImage){
//	imageready = images[ImageIndex].ReadImage(index, p, data);
//	bool ReadImage(int index, Params & p, ImageData & id, bool RefImage, UINT fileoffset = 0, UINT NChannels=1, UINT ChannelNumber = 0);

	imageready = images[ImageIndex].ReadImage(index, p, id, RefImage, FileOffset, NImages, ImageIndex );
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

	int rest=f1.GetLength();
	int read = 10000;

	while (rest>0){
		if (read>rest) read = rest;
		f1.Read(block, read);
		f2.Write(block, read);
		rest -= read;
	}

	f1.Close();

	uint64 offset = index*2*this->Sizex*this->Sizey;

	f2.Seek(offset, 0L);


//	for (unsigned __int32 y=0; y<pp->ysizeimage; y++){
	for (unsigned __int32 y=0; y<this->Sizey; y++){
//		for (unsigned __int32 x=0; x<pp->xsizeimage; x++){
		for (unsigned __int32 x=0; x<this->Sizex; x++){
			f2.Write(&value, 2);
		}
	}
//	pp->Convert(currentimage, pp->xsizeimage, pp->ysizeimage);
	return true;

}

bool ImageSequence::ReadImage(int index, Params & p, ImageData & id, bool RefImage,
							  unsigned __int64 fileoffset, UINT NChannels,  UINT ChannelNumber){
//bool ImageSequence::ReadImage(int index, Params & p, double ** data){
	pp=&(p);

	p.xsizeimage = this->Sizex;
	p.ysizeimage = this->Sizey;

	id.xsize = this->Sizex;
	id.ysize = this->Sizey;

	p.RectOb.SetSize(p.xsizeimage, p.ysizeimage);
	p.ConusOb.SetSize(p.xsizeimage, p.ysizeimage);


	ImageDataType ** d = id.data;
	if (RefImage){
		d = id.refdata;
	}
	if (FileMode == 1){
		return ReadPstFile(index, NULL, d);
	}
	if (FileMode == 2){
//		return ReadSisFile(index, NULL, d, fileoffset, NChannels, ChannelNumber);
		return ReadSisFile(index, d, ChannelNumber);
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


bool DataFile::AssignFileNamesAndIndexes(CString Name, CFile & file){
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

	file.SeekToBegin();

	if (IsAnavisionFile(file)){
//		Alert0("Is anabv");
		FileMode = 2;
		Converted = false;
		return LoadFromFile(file);
	}

	file.SeekToBegin();
	if (EXT.Compare("PST") == 0){
		FileNamesAreRelative = false;
		FileMode = 1;
		NImages = 1;
		images = new ImageSequence[NImages];
		uint64 lengthfile = file.GetLength();
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
		IS.LastIndex = nimage-1;
		IS.RootName = "";
		IS.FileNamesAreRelative=false;

		p.TryLoadLookupTable(0);

		IS.initialized = true;
		
		return true;
	}
	if (EXT.Compare("VWS") == 0){
		return ReadVwsFile(file);
	}
	if (EXT.Compare("SIS") == 0){
		file.Close();
		ImageIndex = 0;
//		bool result = ReadSisFile(file);
		bool result = OpenSisFile(Name);
		if (!result) return false;
		if (this->NImages<1) return false;
		p.xsizeimage = images[0].Sizex;
		p.ysizeimage = images[0].Sizey;

		return true;

	}

	return false;
//	return AssignFileNameAndIndexesTxtFiles(Name);
}
bool DataFile::FindNextStringForward(CFile & file, CString c, uint64 & pos){
	char s1[100000];
	char s;
	pos = file.GetPosition();
	uint64 l = c.GetLength();
	uint64 endpos = file.GetLength();
	s1[l]=0;
	char first = c.GetAt(0);
	while (true){
		if ( (pos+l)>endpos){
			return false;
		}
		file.Read(&s, 1);
		if (s==first){
			if (l>1){
				file.Read(&s1[1], l-1);
				file.Seek(pos+1, CFile::begin);
			}
			s1[0]=s;
			CString c2=CString(s1);
			if (c2.Compare(c)==0){
				return true;
			}
		}
		pos++;
//		file.Seek(pos, CFile::begin);
//		ShowFloat(pos, "pos");
	}
}

bool DataFile::FindNextStringBackward(CFile & file, CString c, uint64 & pos){
	char s1[100000];
	char s;
	pos = file.GetPosition();
	uint64 l = c.GetLength();
	uint64 endpos = file.GetLength();
	s1[l]=0;
	while ((pos+l)>endpos){
		if (pos==0) return false;
		pos--;
	}
	char first = c.GetAt(0);

	while (true){

		file.Read(&s, 1);
		if (s==first){
			if (l>1){
				file.Read(&s1[1], l-1);
//				file.Seek(pos+1, CFile::begin);
			}
			s1[0]=s;
			CString c2=CString(s1);
			if (c2.Compare(c)==0){
				return true;
			}
		}
		if (pos==0) return false;
		pos--;
		file.Seek(pos, CFile::begin);
	}
	return false;
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
		if (!FindNextStringForward(file, nullchar, Null1Pos)){
			break;
		}
//		ShowFloat(Null1Pos, "Null1Pos");
		file.Seek(SlashPos+1, CFile::begin);
		char fil[10000];
		int l = Null1Pos-SlashPos-1;
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
		l=stylespos-StartTitlePos-2;
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
			IS.LastIndex = nimage-1;
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
	for (int i=0; i<NImages; i++){
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
	if (index>=NImages) return false;

	ImageSequence * newimages = new ImageSequence[NImages+1];

	int i;
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

	int rest=f1.GetLength();
	int read = 10000;

	while (rest>0){
		if (read>rest) read = rest;
		f1.Read(block, read);
		f2.Write(block, read);
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
	if (indexa>=NImages){
//		Alert0("Hi2");
		return false;
	}
	if (indexb<0){
//		Alert0("Hi3");
		return false;
	}
	if (indexb>=NImages){
//		Alert0("Hi4");
		return false;
	}
	if ( (indexc>0) && (indexc>=NImages)){
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

		int i;
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



bool DataFile::OldReadSisFile(CFile & file){
//	bool DoShow = true;
	bool DoShow = false;
	char filx[10000];

	uint64 NChanpos1;
	uint64 NChanpos2;

	if (!FindNextStringForward(file, "Channels number=", NChanpos1)){
		return false;
	}
	file.Seek(NChanpos1, CFile::begin);
	if (!FindNextStringForward(file, "\"", NChanpos1)){
		return false;
	}
	file.Seek(NChanpos1+1, CFile::begin);
	if (!FindNextStringForward(file, "\"", NChanpos2)){
		return false;
	}
	int nc=NChanpos2-NChanpos1;
	file.Seek(NChanpos1+1, CFile::begin);
	file.Read(&filx[0], nc);
	filx[nc]=0;
//	Alert0(CString(filx));
	uint64 NChan = atoi(CString(filx));
	
	if (DoShow) ShowFloat(NChan, "NChan");

	uint64 NTickpos1;
	uint64 NTickpos2;

	if (!FindNextStringForward(file, "Ticks number=", NTickpos1)){
		return false;
	}
	file.Seek(NTickpos1, CFile::begin);
	if (!FindNextStringForward(file, "\"", NTickpos1)){
		return false;
	}
	file.Seek(NTickpos1+1, CFile::begin);
	if (!FindNextStringForward(file, "\"", NTickpos2)){
		return false;
	}
	int nt=NTickpos2-NTickpos1;
	file.Seek(NTickpos1+1, CFile::begin);
	file.Read(&filx[0], nt);
	filx[nt]=0;
//	Alert0(CString(filx));
	uint64 NTick = atoi(CString(filx));
	if (DoShow) ShowFloat(NTick, "NTick");

	uint64 widthpos1;
	uint64 widthpos2;

	if (!FindNextStringForward(file, "DefaultPlane width=", widthpos1)){
		return false;
	}
	file.Seek(widthpos1, CFile::begin);

	if (!FindNextStringForward(file, "\"", widthpos1)){
		return false;
	}
	file.Seek(widthpos1+1, CFile::begin);
	if (!FindNextStringForward(file, "\"", widthpos2)){
		return false;
	}
	int nw=widthpos2-widthpos1;
	file.Seek(widthpos1+1, CFile::begin);
	file.Read(&filx[0], nw);
	filx[nw]=0;
//	Alert0(CString(filx));
	uint64 width = atoi(CString(filx));
	if (DoShow) ShowFloat(width, "width");

	uint64 heightpos1;
	uint64 heightpos2;
	if (!FindNextStringForward(file, "height=", heightpos1)){
		return false;
	}

	file.Seek(heightpos1, CFile::begin);

	if (!FindNextStringForward(file, "\"", heightpos1)){
		return false;
	}
	file.Seek(heightpos1+1, CFile::begin);

	if (!FindNextStringForward(file, "\"", heightpos2)){
		return false;
	}
	int nh=heightpos2-heightpos1;
	file.Seek(heightpos1+1, CFile::begin);
	file.Read(&filx[0], heightpos2-heightpos1);
	filx[nh]=0;
//	ShowFloat(nh, "nh");
//	Alert0(CString(filx));
	uint64 height = atoi(CString(filx));
	if (DoShow) ShowFloat(height, "height");

	uint64 HeaderEndPos;
	file.Seek(heightpos2+1, CFile::begin);
	if (!FindNextStringForward(file, "Header>", HeaderEndPos)){
		return false;
	}
	if (DoShow) ShowFloat(HeaderEndPos, "HeaderEndPos");

	uint64 filesize = file.GetLength();

	uint64 DataSize=2*NChan*NTick*width*height;// = unsigned __int64
	while (DataSize>filesize){
		NTick -= 1;
		if (DoShow) ShowFloat(NTick, "NTick");
		DataSize=2*NChan*NTick*width*height;// = unsigned __int64
	}
	uint64 MinOffset = HeaderEndPos;

	uint64 size1image = 2*width*height;
	uint64 Offset = filesize-DataSize;
	while (Offset>(size1image+MinOffset)){
		Offset -= size1image;
	}

	char s[30];
	_itoa(filesize, s, 10);
	if (DoShow) Alert0(CString(s)+CString("filesize"));
	if (DoShow) ShowFloat(filesize, "filesize");

	_itoa(DataSize, s, 10);
	if (DoShow) Alert0(CString(s)+ CString("DataSize"));
	if (DoShow) ShowFloat(DataSize, "DataSize");

	_itoa(Offset, s, 10);
	if (DoShow) Alert0(CString(s)+ CString(" Offset"));
	if (DoShow) ShowFloat(Offset, "Offset");
//	if (true) ShowFloat(Offset, "Offset");

	FileType = 1;
	FileOffset = Offset;

/*
char * ccc = new char[FileOffset+10];
file.SeekToBegin();
	file.Read(ccc, FileOffset+10);
	CFile OutFile = CFile("c:/Offset.tmp", CFile::modeCreate | CFile::modeWrite);
	OutFile.Write(ccc, FileOffset+10);
*/

	if (NChan<1) return false;

	this->ImageIndex = 0;
	this->FirstFileName=file.GetFileTitle();
	this->rootpath = file.GetFilePath();

	GetPath(file.GetFilePath(), this->rootpath);

//	Alert0(this->rootpath);
	this->Extension = "SIS";
//	rootpath = rootpath.Left(rootpath.GetLength()-3)+"pst/";
	this->FileNamesAreRelative = true;
	NImages = NChan;

	images = new ImageSequence[NImages];
	for (int i=0; i<NImages; i++){
		images[i].FileName = this->FirstFileName;
		images[i].FileNamesAreRelative = true;
		images[i].RootName = rootpath;
		images[i].FileMode = 2; // SIS file
		images[i].Sizex = width;
		images[i].Sizey = height;
		images[i].FirstIndex = 0;
		images[i].LastIndex = NTick-1;

	}
	return true;
}
bool DataFile::OpenSisFile(CString & FileName){
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
	NImages = NChan;

	images = new ImageSequence[NImages];
	for (int i=0; i<NImages; i++){
		images[i].SISStack = imageStack;
		images[i].FileName = this->FirstFileName;
		images[i].FileNamesAreRelative = true;
		images[i].RootName = rootpath;
		images[i].FileMode = 2; // SIS file
		images[i].Sizex = width;
		images[i].Sizey = height;
		images[i].FirstIndex = 0;
		images[i].LastIndex = NTick-1;
	}
	this->FileOffset = -999; // to indicate that the sis file is read with arivis lib
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

