// This file implements the AnaVisionTif class
// created 10.07.2011
// Copyright Michael Pusch, pusch@ge.ibf.cnr.it

// Purpose is to wrap around the libtiff library to read Till Photonics new
// data files.

#include "stdafx.h"
#include "AnaVisionTif.h"
#include "../common/utils.h"

MySingleTiffileInfo::MySingleTiffileInfo(){
	FileIsOpen = false;
	stdtiffr=NULL;

	IsStandardTif = false;
	SetPointersToNull();
}
MySingleTiffileInfo::~MySingleTiffileInfo(){
	if (FileIsOpen){
		this->Tiffile.Close();
	}
	DeAlloc();
	if (stdtiffr){
//		TinyTIFFReader_close(stdtiffr); 
	}

}
void MySingleTiffileInfo::SetPointersToNull(){
	ifds = NULL;
	tags = NULL;
	NIfds = 0;
}

void MySingleTiffileInfo::Alloc(){
}

void MySingleTiffileInfo::DeAlloc(){
	if (IsStandardTif) return;
	if (NIfds<1) return;
	for (uint32 i=0; i<NIfds; i++){
		delete [] tags[i];
	}
	delete [] tags;
	delete [] ifds;
}

bool MySingleTiffileInfo::OpenFile(CString FileName, int ImageOrderMethod, bool ReadFileInfo, bool isstandardtif){
//	CWaitCursor dummy;
	Name = FileName;
	if (isstandardtif){
		IsStandardTif = true;
		stdtiffr=TinyTIFFReader_open(Name); 
	    if (!stdtiffr) { 
			Alert0("ERROR reading (not existent, not accessible or no TIFF file)");
			return false;
		}
		NIfds=TinyTIFFReader_countFrames(stdtiffr); 
		width=TinyTIFFReader_getWidth(stdtiffr); 
        height=TinyTIFFReader_getHeight(stdtiffr); 
	    TinyTIFFReader_close(stdtiffr); 

	}
	else{
		if (!Tiffile.Open(FileName, CFile::modeRead )){
			return false;
		}
		FileIsOpen = true;
		if (Tiffile.GetLength()<100){
			return false;
		}

		Tiffile.Seek(4,0);

		uint32 OffsetFirstIFD;
		Tiffile.Read(&OffsetFirstIFD, 4);
//	ShowFloat(OffsetFirstIFD, "first offset");

		uint32 lastoffset = OffsetFirstIFD;

		NIfds = 1;
		while (true){
			Tiffile.Seek(lastoffset, 0);
			uint16 NumberOfTags;
			Tiffile.Read(&NumberOfTags, 2);
		
			lastoffset += 2 + NumberOfTags*12;
			Tiffile.Seek(lastoffset, 0);
			uint32 NextIFDOffset;
			Tiffile.Read(&NextIFDOffset, 4);
			if (NextIFDOffset == 0){
				break;
			}
			NIfds++;
			lastoffset = NextIFDOffset;
		}
		if (NIfds<1){
			Alert0("no image found (NIfds<1)");
			return false;
		}

//	ShowFloat(NIfds, "NIFDS");
	
		ifds = new MyTiffIFDStruct[NIfds];
		tags = new MyTiffTagStruct* [NIfds];

		lastoffset = OffsetFirstIFD;

		for (uint32 i=0; i<NIfds; i++){
			ifds[i].OffsetThisIFD = lastoffset;
			Tiffile.Seek(lastoffset, 0);
			Tiffile.Read(&ifds[i].NumberOfTags, 2);
//		ShowFloat(ifds[i].NumberOfTags, "NTags");
			tags[i] = new MyTiffTagStructTag[ifds[i].NumberOfTags];

			for (uint32 itag=0; itag<ifds[i].NumberOfTags; itag++){
				Tiffile.Read(&tags[i][itag], 12);
			}
			Tiffile.Read(&ifds[i].OffsetToNextIFD, 4);
			lastoffset = ifds[i].OffsetToNextIFD;
		}
		this->FileIsOpen = true;


//	Alert0("hi");
//	if (!IsStandardTif
		for (int i=0; i<ifds[0].NumberOfTags; i++){
			if ((tags[0][i].TagCode == 270) && ReadFileInfo){
//			ShowUInt(tags[0][i].DataType, "DataType??");
//			char * s = new char [tags[0][i].NValues+100];
				Tiffile.Seek(tags[0][i].Data, 0);
				char c;
				FileInfo = "";
				for (uint32 ic=0; ic<tags[0][i].NValues; ic++){
					Tiffile.Read(&c, 1);
					FileInfo += CString(c);
//				s[ic]=c;
				}
			}
			if (tags[0][i].TagCode == 256){
				this->width = tags[0][i].Data;
//			ShowFloat(width, "width");
			}
			if (tags[0][i].TagCode == 257){
				this->height = tags[0][i].Data;
//			ShowFloat(height, "height");
			}


		}

		for (int l=0; l<this->NIfds; l++){
			ifds[l].StripDataOffset = -1;
			for (int i=0; i<ifds[l].NumberOfTags; i++){
				if (tags[l][i].TagCode == 273){
					ifds[l].StripDataOffset = tags[l][i].Data;
					//ShowLongInt(ifds[l].StripDataOffset, "StripDataOffset");
				}
			}
		}
//	Alert0("hi");

		if (ReadFileInfo){
//		Alert0("anapyze");
			AnalyzeIfds(ImageOrderMethod);
		}

		return true;
	}
}

void MySingleTiffileInfo::AnalyzeIfds(int ImageOrderMethod){
//	ShowFloat(NIfds, "NIfds");
	if (IsStandardTif){
	}
	else{
		for (int l=0; l<this->NIfds; l++){
			this->ifds[l].IndexChannel = -1;
			this->ifds[l].IndexTime = -1;
			this->ifds[l].IndexZ = -1;
		}
		unsigned __int64 pos=0;
		unsigned __int64 oldpos = pos;
		while (true){
			oldpos = pos;
			int ifdvalue;
//		ShowFloat(ifdvalue, "IFD before");
			if (!AnaVisionTif::GetXMLValue(this->FileInfo, "OME:TiffData",  "IFD", ifdvalue, pos)){
				break;
			}
//		ShowFloat(ifdvalue, "IFD afetr");

			pos = oldpos;
			int cvalue;
			if (!AnaVisionTif::GetXMLValue(this->FileInfo, "OME:TiffData",  "FirstC", cvalue, pos)){
					break;
			}
//		ShowFloat(cvalue, "firstc");

	
			pos = oldpos;
			int zvalue;
			if (!AnaVisionTif::GetXMLValue(this->FileInfo, "OME:TiffData",  "FirstZ", zvalue, pos)){
				break;
			}
//		ShowFloat(zvalue, "firstz");

			pos = oldpos;
			int tvalue;
			if (!AnaVisionTif::GetXMLValue(this->FileInfo, "OME:TiffData",  "FirstT", tvalue, pos)){
				break;
			}
//		ShowFloat(ImageOrderMethod, "ImageOrderMethod"); // = ORDER_BY_ZSTACK;
//		ImageOrderMethod = ORDER_BY_TIME;
//		ShowFloat(ImageOrderMethod, "ImageOrderMethod"); // = ORDER_BY_ZSTACK;
//		ShowFloat(tvalue, "firstt");
			if ((ifdvalue>=0) && (ifdvalue<this->NIfds)){
				if (ImageOrderMethod == ORDER_BY_TIME){
					ifds[ifdvalue].IndexChannel = cvalue;
					ifds[ifdvalue].IndexTime = tvalue;
					ifds[ifdvalue].IndexZ = zvalue;
				}
				if (ImageOrderMethod == ORDER_BY_ZSTACK){
					ifds[ifdvalue].IndexChannel = tvalue;
					ifds[ifdvalue].IndexTime = zvalue;
					ifds[ifdvalue].IndexZ = cvalue;
				}
			}

		}
	}


}



AnaVisionTif::~AnaVisionTif(){
	DeAlloc();
}

AnaVisionTif::AnaVisionTif(){
//	FileIsOpen = false;
	Initialized = false;
	NTotalIFDs = 0;
	NFiles=0;
	NImagesPerChannel = 0;
	NChannels = 0;

	IsStandardTif = false;
	FirstFileClosedTemporarily = false;


	SetPointersToNull();
}
void AnaVisionTif::Alloc(){
}
void AnaVisionTif::DeAlloc(){
	if (NTotalIFDs<1) return;
	if (files){
		delete [] files;
		files = NULL;
	}
	if (FileChannelRefs){
		for (int i=0; i<NImagesPerChannel; i++){
			delete [] FileChannelRefs[i];
		}
		delete [] FileChannelRefs;
	}
	FileChannelRefs = NULL;

}


void AnaVisionTif::SetPointersToNull(){
	files = NULL;
	FileChannelRefs = NULL;
}


bool AnaVisionTif::GetNewName(CString LastName, CString & NewName){
	CString Path;
	CString Title;

	int NFull = LastName.GetLength();

	GetPath(LastName, Path);

	int NPath = Path.GetLength();

//	Alert0(Path);

	if (NPath<2) return false;

	Title = LastName.Right(NFull-NPath);
//	Alert0(Title);

	CString Path1=Path.Left(NPath-1);
	CString Path2=Path1;
	Path2.MakeUpper();

	int LenTitle = Title.GetLength();
	CString TitleUpper = Title;
	TitleUpper.MakeUpper();
	
	//	int ppos=Path2.ReverseFind('P');
	int ppos=TitleUpper.ReverseFind('P');
	if (ppos<0){
//		Alert0("Not found ppos");
		return false;
	}

	int NDigit = LenTitle-ppos-5; 
//	ShowFloat(NDigit, "NDigit");

//	CString Num=Title.Right(LenTitle-ppos-2);

	CString Num0 = Title.Left(LenTitle-4);
	CString Num = Num0.Right(NDigit);
//	Alert0(CString("Num:")+Num);
	UINT inum=atoi(Num)+1;
	char s[20];
	_itoa(inum, s, 10);
	CString num1=CString(s);
	CString IncDir = Path1.Left(ppos-1)+CString("_P")+num1+CString("\\");


//	Alert0(IncDir);

//	NewName = IncDir+Title;
	NewName = Path +"\/"+Title.Left(ppos+1)+num1+".tif";
	return true;
}
bool AnaVisionTif::OpenNextTiff(CString LastName, CString & NewName, bool ReadFileInfo){

	CString Path;
	CString Title;

	int NFull = LastName.GetLength();

	GetPath(LastName, Path);

	int NPath = Path.GetLength();

//	Alert0(Path);

	if (NPath<2) return false;

	Title = LastName.Right(NFull-NPath);
//	Alert0(Title);

	CString Path1=Path.Left(NPath-1);
	CString Path2=Path1;
	Path2.MakeUpper();
	int ppos=Path2.ReverseFind('P');
	if (ppos<0){
		Alert0("Not found ppos");
		return false;
	}
	CString Num=Path1.Right(NPath-ppos-2);
//	Alert0(CString("Num:")+Num);
	UINT inum=atoi(Num)+1;
	char s[20];
	_itoa(inum, s, 10);
	CString num1=CString(s);
	CString IncDir = Path1.Left(ppos-1)+CString("_P")+num1+CString("\/");

//	Alert0(IncDir);

	NewName = IncDir+Title;
	MySingleTiffileInfo TestTiff;

	if (TestTiff.OpenFile(NewName, ImageOrderMethod, ReadFileInfo)){
//	if (TestTiff.OpenFile(NewName)){
		return true;
	}

	return false;
}
bool AnaVisionTif::OpenFileOld(CString FileName){
	NFiles = 0;
	{
		MySingleTiffileInfo TestTiff;
		this->NTotalIFDs = 0;
		if (TestTiff.OpenFile(FileName, ImageOrderMethod)){
			this->NTotalIFDs += TestTiff.NIfds;
			NFiles++;
			this->width = TestTiff.width;
			this->height = TestTiff.height;
		}
		else{
			return false;
		}
	}
	FirstTiffFileName = FileName;

	CString LastName = FileName;
	while (true){
		CString NewName;
		if (OpenNextTiff(LastName, NewName)){
			this->NTotalIFDs++;
			NFiles++;
		}
		else{
			break;
		}
		LastName = NewName;
	}


	files = new MySingleTiffileInfo[NFiles];

	LastName = FileName;
	for (int ifile=0; ifile<NFiles; ifile++){
		if (!files[ifile].OpenFile(LastName, ImageOrderMethod, true)){
			Alert0("ERROR IN READING TIFFS");
			return false;
		}

//		ShowFloat(this->NImagesPerChannel, "NImagesPerchannel");
		CString NewName;
		if (ifile<(NFiles-1)){
			if (!OpenNextTiff(LastName, NewName)){
				Alert0("ERROR IN READING TIFFS");
				return false;
			}
		}
		LastName = NewName;
	}

	NTotalIFDs = 0;
	for (int f=0; f<NFiles; f++){
		NTotalIFDs += files[f].NIfds;
	}

//	ShowFloat(NFiles, "NFiles");
//	ShowFloat(this->NTotalIFDs, "Nframes");

	if (NFiles<1) return false;
	NChannels = 0;
	this->NImagesPerChannel = 0;
	for (int f=0; f<NFiles; f++){
//		ShowFloat(files[f].NIfds, "files[f].NIfds");
		for (int ifd=0; ifd<files[f].NIfds; ifd++){

			if (files[f].ifds[ifd].IndexChannel>=NChannels){
				NChannels++;
			}
			if (files[f].ifds[ifd].IndexTime >= NImagesPerChannel){
				NImagesPerChannel = files[f].ifds[ifd].IndexTime+1;
			}
		}
	}
//	ShowFloat(NChannels, "NChannels");
//	ShowFloat(NImagesPerChannel, "NImagesPerChannels");
	while (NTotalIFDs<NChannels*NImagesPerChannel){
		NImagesPerChannel--;
	}
//	ShowFloat(NChannels, "NChannels");
//	ShowFloat(NImagesPerChannel, "NImagesPerChannels");

	FileChannelRefs = new MyFileChannelRef * [NImagesPerChannel];
	for (int i=0; i<NImagesPerChannel; i++){
		FileChannelRefs[i] =  new MyFileChannelRef [NChannels];
		for (int j=0; j<NChannels; j++){
			FileChannelRefs[i][j].FileIndex = FileChannelRefs[i][j].IfdIndex = -1;
		}
	}

	for (int f=0; f<this->NFiles; f++){
		for (int i=0; i<files[f].NIfds; i++){
			int it = files[f].ifds[i].IndexTime;
			int ic = files[f].ifds[i].IndexChannel;
			if (false){
//					if ( (ic==2) && (it>94)){
					ShowFloat(it, "it");
					ShowFloat(i, "FileChannelRefs[it][ic].IfdIndex");
			}
			if ((it>=0) && (it<NImagesPerChannel)){
				if ((ic>=0) && (ic<NChannels)){
					FileChannelRefs[it][ic].FileIndex = f;
					FileChannelRefs[it][ic].IfdIndex = i;
					if (false){
//					if ( (ic==2) && (it>94)){
						ShowFloat(it, "it");
						ShowFloat(i, "FileChannelRefs[it][ic].IfdIndex");
					}

				}
			}
		}
	}
	for (int it=0; it<NImagesPerChannel; it++){
		for (int ic=0; ic<NChannels; ic++){
			if (FileChannelRefs[it][ic].FileIndex<0){
				Alert0("Not all indexes assigned - f ");
				return false;
			}
			if (FileChannelRefs[it][ic].IfdIndex<0){
				Alert0("Not all indexes assigned - ifd");
				return false;
			}
		}
	}
	Initialized = true;
	return true;

}

bool AnaVisionTif::OpenFile(CString FileName, bool isstandardtif){
	if (isstandardtif){
		IsStandardTif = true;
	}
	int MaxFiles = 1000;
	files = new MySingleTiffileInfo[MaxFiles];

	NFiles = 0;
/*
{
		MySingleTiffileInfo TestTiff;
		this->NTotalIFDs = 0;
		if (TestTiff.OpenFile(FileName)){
			this->NTotalIFDs += TestTiff.NIfds;
			NFiles++;
			this->width = TestTiff.width;
			this->height = TestTiff.height;
		}
		else{
			return false;
		}
	}
*/
	FirstTiffFileName = FileName;

/*
CString LastName = FileName;
	while (true){
		CString NewName;
		if (OpenNextTiff(LastName, NewName)){
			this->NTotalIFDs++;
			NFiles++;
		}
		else{
			break;
		}
		LastName = NewName;
	}


	files = new MySingleTiffileInfo[NFiles];
*/
	CString LastName = FileName;
	int ifile = 0;
	while (true){
		if (ifile>=MaxFiles ){
			char s[20];
			_itoa(MaxFiles, s, 10);
			Alert0(CString("More than ")+CString(s)+CString(" files"));
			break;
		}
//		ShowFloat(ifile, "ifile");
//	for (int ifile=0; ifile<NFiles; ifile++){
		if (!files[ifile].OpenFile(LastName, this->ImageOrderMethod, true, IsStandardTif)){
			break;
//			Alert0("ERROR IN READING TIFFS");
//			return false;
		}

		ifile++;
		CString NewName;
		if (IsStandardTif){
			break;
		}
		if (!this->GetNewName(LastName, NewName)){
//			Alert0(LastName + CString("could bnot get ")+NewName);
			break;
		}
//		Alert0(LastName+CString("NEW")+NewName),
		LastName = NewName;

/*
if (ifile<(NFiles-1)){
			if (!OpenNextTiff(LastName, NewName)){
				Alert0("ERROR IN READING TIFFS");
				return false;
			}
		}
		LastName = NewName;
	}
*/
	}

	NFiles = ifile;

	if (NFiles<1){
		Alert0("Nfiles < 1; could not open a single tiff");
		return false;
	}
	this->width = files[0].width;
	this->height = files[0].height;
//	ShowFloat(width, "w");

	NTotalIFDs = 0;
	for (int f=0; f<NFiles; f++){
		NTotalIFDs += files[f].NIfds;
	}

//	ShowFloat(NFiles, "NFiles");
//	ShowFloat(this->NTotalIFDs, "Nframes");


	if (IsStandardTif){
			NChannels = 1;
			this->NImagesPerChannel = this->NTotalIFDs;
	}
	else{

		NChannels = 0;
		this->NImagesPerChannel = 0;
		for (int f=0; f<NFiles; f++){
	//	ShowFloat(files[f].NIfds, "files[f].NIfds");
			for (int ifd=0; ifd<files[f].NIfds; ifd++){

	//		ShowFloat(files[f].ifds[ifd].IndexChannel, "files[f].ifds[ifd].IndexChannel");
				if (files[f].ifds[ifd].IndexChannel>=NChannels){
					NChannels++;
				}
	//		ShowFloat(files[f].ifds[ifd].IndexTime, "files[f].ifds[ifd].IndexTime");
				if (files[f].ifds[ifd].IndexTime >= NImagesPerChannel){
					NImagesPerChannel = files[f].ifds[ifd].IndexTime+1;
				}
			}
		}
	}
//	ShowFloat(NChannels, "NChannels");
//	ShowFloat(NImagesPerChannel, "NImagesPerChannels");

	if (!IsStandardTif){
		while (NTotalIFDs<NChannels*NImagesPerChannel){
			NImagesPerChannel--;
		}
	}
//	ShowFloat(NChannels, "NChannels");
//	ShowFloat(NImagesPerChannel, "NImagesPerChannels");

	if (!IsStandardTif){
		FileChannelRefs = new MyFileChannelRef * [NImagesPerChannel];
		for (int i=0; i<NImagesPerChannel; i++){
			FileChannelRefs[i] =  new MyFileChannelRef [NChannels];
			for (int j=0; j<NChannels; j++){
				FileChannelRefs[i][j].FileIndex = FileChannelRefs[i][j].IfdIndex = -1;
			}
		}

		for (int f=0; f<this->NFiles; f++){
			for (int i=0; i<files[f].NIfds; i++){
				int it = files[f].ifds[i].IndexTime;
				int ic = files[f].ifds[i].IndexChannel;
				if (IsStandardTif){
					it=0;
					ic=0;
				}
				if (false){
							ShowFloat(it, "it");
							ShowFloat(ic, "ic");
							ShowFloat(i, "FileChannelRefs[it][ic].IfdIndex");
				}
				if ((it>=0) && (it<NImagesPerChannel)){
					if ((ic>=0) && (ic<NChannels)){
						FileChannelRefs[it][ic].FileIndex = f;
						FileChannelRefs[it][ic].IfdIndex = i;
						if (false){
//					if ( (ic==2) && (it>94)){
							ShowFloat(it, "it");
							ShowFloat(i, "FileChannelRefs[it][ic].IfdIndex");
						}
	
					}
				}
			}
		}
		for (int it=0; it<NImagesPerChannel; it++){
			for (int ic=0; ic<NChannels; ic++){
				if (FileChannelRefs[it][ic].FileIndex<0){
					Alert0("Not all indexes assigned - f ");
					return false;
				}
//			ShowFloat(FileChannelRefs[it][ic].IfdIndex, "FileChannelRefs[it][ic].IfdIndex");
				if (FileChannelRefs[it][ic].IfdIndex<0){
					Alert0("Not all indexes assigned - ifd");
					return false;
				}
			}
		}
	}
//	Alert0("hi");
	Initialized = true;
	return true;

}

int AnaVisionTif::GetNumberOfChannels(){
	return this->NChannels;
}
int AnaVisionTif::GetNumberOfImagesPerChannel(){
	return this->NImagesPerChannel;
}

bool AnaVisionTif::GetXMLValue(CString & instring, CString target1, CString target2,
		int & value,
		unsigned __int64 & pos){
	/*
	Examples:
	With instring= " <OME:TiffData IFD="16" FirstZ="0" FirstT="2" FirstC="4" />
	and target1="OME:TiffData, target2="FirstZ"
	value would be 0
	*/


	target1 = "<" + target1;

	unsigned __int64 oldpos = pos;
	if (!FindNextStringForward(instring, target1, pos)){
		//Alert0(CString("Could not find ")+target1);
		return false;
	}
	target2 += CString("=\"");
	if (!FindNextStringForward(instring, target2, pos)){
		//Alert0(CString("Could not find ")+target2);
		return false;
	}
	unsigned __int64 startpos = pos;
	target2 ="\"";
	if (!FindNextStringForward(instring, target2, pos)){
		//Alert0(CString("Could not find ")+target2);
		return false;
	}
	char s[20000];
	for (unsigned __int64 k=startpos-1; k<pos-1; k++){
		s[k-startpos+1]=instring.GetAt(k);
	}
	s[pos-startpos]=0;
//	Alert0(CString(s));

//	value = atof(s);
	value = atoi(s);

	return true;

}


void AnaVisionTif::GetImageSize(int & Width, int & Height){
	Width = this->width;
	Height = this->height;
}

bool AnaVisionTif::ReadImage(int timeindex, int channel, unsigned __int16 * data){
//	ShowFloat(timeindex, "index");
	if (!Initialized){
		Alert0("not initialized");
		return false;
	}
	if (timeindex<0){
		Alert0("timeindex < 0");
		return false;
	}
//	ShowFloat(this->NImagesPerChannel, "this->NImagesPerChannel");
	if (timeindex>=this->NImagesPerChannel){
		Alert0("timeindex>=this->NImagesPerChannel");
		return false;
	}
	if (channel<0){
		Alert0("channel < 0");
		return false;
	}
	if (channel>=this->NChannels){
		Alert0("channel>=NChannels");
		return false;
	}
	if (IsStandardTif){
		MySingleTiffileInfo & File = files[0];
		File.stdtiffr=TinyTIFFReader_open(File.Name); 
	    if (!File.stdtiffr) { 
			Alert0("ERROR reading (not existent, not accessible or no TIFF file)");
			return false;
		}
		int NIfds=TinyTIFFReader_countFrames(File.stdtiffr); 
		int width=TinyTIFFReader_getWidth(File.stdtiffr); 
        int height=TinyTIFFReader_getHeight(File.stdtiffr); 

        uint32_t frame=0; 
        do { 
//			ShowFloat(frame, "frame");
			std::string ss = TinyTIFFReader_getImageDescription(File.stdtiffr);
			
//			Alert0(ss.c_str());
			int BitsPerSample = TinyTIFFReader_getBitsPerSample(File.stdtiffr);
//			ShowFloat(BitsPerSample, "BitsPerSample");
            uint32_t width=TinyTIFFReader_getWidth(File.stdtiffr); 
            uint32_t height=TinyTIFFReader_getHeight(File.stdtiffr); 
//			ShowFloat(width, "w");
			uint32_t wwidth = width;
			bool IsOdd = false;
			if ( (width % 2) != 0){ // odd
//				Alert0("odd");
				IsOdd = true;
				wwidth = width +1;
			}
//			ShowFloat(wwidth, "wwidth");
			bool ok=true;
			if (width>0 && height>0){
		//		std::cout<<"    size of frame "<<frame<<": "<<width<<"x"<<height<<"\n"; 
			}

            else {
				Alert0("    ERROR IN FRAME ");
			    TinyTIFFReader_close(File.stdtiffr); 
				return false;
//					<<frame<<": size too small "<<width<<"x"<<height<<"\n"; ok=false;
			} 

            uint32_t* image=(uint32_t*)calloc((width+1)*height, sizeof(uint32_t));  
            TinyTIFFReader_getSampleData(File.stdtiffr, image, 0); 
            if (TinyTIFFReader_wasError(File.stdtiffr)) {
				Alert0("error reading frame");
			    TinyTIFFReader_close(File.stdtiffr); 
	            free(image); 
				return false;
				//ok=false; std::cout<<"   ERROR:"<<TinyTIFFReader_getLastError(tiffr)<<"\n"; } 
			}
			
			if (frame == timeindex){
				if (BitsPerSample == 32){
					uint32 MaxData = 0;
					for (int i=0; i<(width*height); i++){
						if (image[i]>MaxData){
							MaxData = image[i];
						}
					}
					bool DoScale = false;
					double Scale;
					if (MaxData>65535){
						DoScale = true;
						Scale = double(65535)/double(MaxData);
					}
//					if (DoScale){
					if (false){
						Alert0("Do Scale");
					}

					for (int i=0; i<(width*height); i++){
						if (DoScale){
							data[i] = unsigned __int16 (double(image[i])*Scale);
						}
						else{
							data[i] = image[i];
						}
					}
				}
				if (BitsPerSample == 16){
					uint16 * datptr = (uint16 *)(image);
					for (int i=0; i<(width*height); i++){
						data[i] = datptr[i];
						if (data[i]>65535){
							data[i] = 65535;
						}
					}
/*
					int k=0;
					int w=0;
					BYTE * byteptr = (BYTE *) (image);
					for (int i=0; i<height; i++){
						for (int j=0; j<width; j++){
							unsigned __int16 * datptr = (unsigned __int16 * ) byteptr;
							data[k] = *datptr;
							k++;
							byteptr += 2;
							if ((j==(width-1)) && IsOdd){
								byteptr -=2;
							}
						}
					}
*/
				}
				if (BitsPerSample == 8){
					char * datptr = (char *)(image);
					for (int i=0; i<(width*height); i++){
						data[i] = 0;
						data[i] = int(datptr[i]);
						if (data[i]>255){
							data[i] = 255;
						}
					}
				}

			   free(image); 
			    TinyTIFFReader_close(File.stdtiffr); 
				return true;
			}
            frame++; 
			if (frame>timeindex){
				Alert0("ERROR frame>=timeindex");
				return false;
			}

        } while (TinyTIFFReader_readNext(File.stdtiffr)); // iterate over all frames
	}
	else{
		int f = FileChannelRefs[timeindex][channel].FileIndex;
		int ifdindex = FileChannelRefs[timeindex][channel].IfdIndex;
//	ShowFloat(f, "f");
//	ShowFloat(ifdindex, "ifdindex");
		if (ifdindex<0){
			Alert0("ifdindex<0");
			return false;
		}

		MySingleTiffileInfo & File = files[f];

		if (!File.FileIsOpen){
			if ((f==0) && FirstFileClosedTemporarily){
				if (!File.Tiffile.Open(FirstTiffFileName, CFile::modeRead)){
					Alert0("!File.Tiffile.Open(FirstTiffFileName, CFile::modeRead))");
					return false;
				}
				FirstFileClosedTemporarily = false;
			}
			else{
				return false;
			}
		}
		unsigned __int32 Offset = File.ifds[ifdindex].StripDataOffset;
		unsigned __int32 BytesToRead = this->height*this->width*2; // bytes

//	if (timeindex>95){
		if (false){
			ShowFloat(f, "f");
			ShowFloat(ifdindex, "ifdindex");
			ShowLongInt(Offset, "Offset");
			ShowLongInt(File.Tiffile.GetLength(), "Length");
			ShowLongInt(BytesToRead, "BytesToRead");
			ShowLongInt(File.Tiffile.GetLength()-Offset, "Length-offset");
			ShowLongInt(File.Tiffile.GetLength()-Offset-BytesToRead, "Length-offset-BytesToRead");
		}


		File.Tiffile.Seek(Offset, 0);
		unsigned __int32 read = File.Tiffile.Read(data, BytesToRead);

		if (read<BytesToRead){
			return false;
		}
		return true;
	}
	
}
void AnaVisionTif::CloseFirstFileTemporarily(){
	if (!Initialized){
		return;
	}
	MySingleTiffileInfo & File = files[0];
	if (!File.FileIsOpen) return;
	FirstFileClosedTemporarily = true;
	File.Tiffile.Close();
	File.FileIsOpen = false;


}
bool AnaVisionTif::OpenStandardTif(CString FileName) {
	bool IsStandardTif = true;
	return OpenFile(FileName, IsStandardTif);
}

	
bool AnaVisionTif::SaveStandard16BitTiff(CString FileName, int sizex, int sizey, vector <unsigned __int16> & numbers){
	const char* s = FileName.GetBuffer();
	uint16_t bitsPerSample = 16;
	TinyTIFFWriterSampleFormat sampleFormat = TinyTIFFWriter_UInt;
	uint32_t width = sizex;
	uint32_t height = sizey;
	TinyTIFFWriterSampleInterpretation sampleInterpretation =  TinyTIFFWriter_Greyscale;
	uint16_t samples = 0;
	TinyTIFFWriterFile* tiff = TinyTIFFWriter_open(s,
										bitsPerSample,
										sampleFormat,
										samples,
										width, height,
										sampleInterpretation);
	if (!tiff){
		return false;
	}
	const void * data = &(numbers[0]);
    int n = TinyTIFFWriter_writeImage(tiff, data);
 
//	ShowFloat(n, "n");
	TinyTIFFWriter_close(tiff);
	return true;
}

bool AnaVisionTif::SaveRGBTiff(CString Name, int sizex, int sizey, vector <MyRGBOnlyStruct> &Pixels){
	const char* s = Name.GetBuffer();
	uint16_t bitsPerSample = 8;
//	ShowFloat(sizex, "sizex");
//	ShowFloat(sizey, "sizeY");
	TinyTIFFWriterSampleFormat sampleFormat = TinyTIFFWriter_UInt;
	uint32_t width = sizex;
	uint32_t height = sizey;
//	width = sizey;
//	height = sizex;
	TinyTIFFWriterSampleInterpretation sampleInterpretation =  TinyTIFFWriter_RGB;
	uint16_t samples = 3;
	TinyTIFFWriterFile* tiff = TinyTIFFWriter_open(s,
										bitsPerSample,
										sampleFormat,
										samples,
										width, height,
										sampleInterpretation);
	if (!tiff){
		return false;
	}
	const void * data = &(Pixels[0]);
    bool result = TinyTIFFWriter_writeImage(tiff, data);
 
//	ShowFloat(n, "n");
	TinyTIFFWriter_close(tiff);
	return result;
}
