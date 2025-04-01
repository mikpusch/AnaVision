#include "stdafx.h"
#include "AnaVisionImageFile.h"
#include "AnaVision.h"

AnaVisionImageFile::AnaVisionImageFile(){

	Width = Height = 0;
}

AnaVisionImageFile::~AnaVisionImageFile(){
}

/*
AnaVisionImageFileDataType AnaVisionImageFile::GetPixel(int x, int y){
//    return data[y*this->Width + x];
	return data[y*this->Width + x];
//	int value = data[y*this->Width + x];
  //  return data[y*this->Width + x];
//	if ((x==20) && (y==20)) ShowFloat(value , "value inn getpixel");

 //   return data[y*this->Width + x];
//	return value;

	//return rand();
}
*/

AnaVisionImageFileDataType AnaVisionImageFile::GetMeanEnvironment(int x, int y, unsigned int PixelDepth){
	if (PixelDepth<1){
		return GetPixel(x, y);
	}
	int x1=x-PixelDepth;
	if (x1<0){
		x1=0;
	}
	int x2=x+PixelDepth;
	if (x2>(this->Width-1)){
		x2=this->Width-1;
	}
	int y1=y-PixelDepth;
	if (y1<0){
		y1=0;
	}
	int y2=y+PixelDepth;
	if (y2>(this->Height-1)){
		y2=this->Height-1;
	}
	int n=0;
	double mean = 0.0;
	for (int ix=x1; ix<=x2; ix++){
		for (int iy=y1; iy<=y2; iy++){
			mean += double(GetPixel(ix, iy));
			n++;
		}
	}

	return AnaVisionImageFileDataType( mean/double(n));

}
void AnaVisionImageFile::SetPixel(AnaVisionImageFileDataType value, int x, int y){
	data[y*this->Width + x] = value;
}

int AnaVisionImageFile::GetNBytesImage(){
	return 4*this->Width*this->Height;
}

bool AnaVisionImageFile::ReadImage(int & bytesavailable, CFile & file, int Version){
	int NBytes = GetNBytesImage();
	int NPixel = this->Width*this->Height;
	data.resize(NPixel);
	int NRead = file.Read(&(data[0]), NBytes);
	if (NBytes != NRead){
		Alert0("could not read image");
	}
	bytesavailable -= NRead;
	return (NBytes == NRead);
}
bool AnaVisionImageFile::WriteImage(int & byteswritten, CFile & file){
	int NBytes = GetNBytesImage();
	file.Write(&(data[0]), NBytes);
	byteswritten += NBytes;

	return true;
}


AnaVisionImageFileStack::AnaVisionImageFileStack(){
	Width = Height = 0;
}
AnaVisionImageFileStack::~AnaVisionImageFileStack(){
}

bool AnaVisionImageFileStack::AppendImage(AnaVisionImageFile & image){ // adds to this stack
//	ShowFloat(image.Width*image.Height, "image.Width*image.Height");
//	ShowFloat(this->Width, "this->Width");
	if (this->Images.size()<1){
		this->Width = image.Width;
		this->Height = image.Height;
	}
	else{

		if (image.Width != this->Width){
			Alert0("width not equal in AnaVisionImageFileStack::AppendImage");
			return false;
		}
		if (image.Height != this->Height){
			Alert0("height not equal in AnaVisionImageFileStack::AppendImage");
			return false;
		}
	}
//	ShowFloat(image.data.size(), "image.data.size()");
	if (image.data.size() != (image.Height*image.Width)){
		Alert0("image size not correct in AnaVisionImageFileStack::AppendImage");
		return false;
	}
	this->Images.push_back(image);
	return true;
}


bool AnaVisionImageFileStack::ReadStack(int & bytesavailable, CFile & file, int Version){
	CFile * fp = &(file);
	unsigned __int32 NImages;
	MyReadVariable(NImages);
//	ShowFloat(NImages, "NI");
//	int result = ReadBytesNoSeek(&file, 4, &NImages);
//	if (!result){
//		Alert0("could not read number of images of stack in AnaVisionImageFileStack::ReadStack");
//		return false;
//	}

	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	pApp->MyLoadString(bytesavailable, fp, this->Title);
//	Alert0(Title);

//	if (!LoadString(this->Title, file)){
//		Alert0("could not read title in AnaVisionImageFileStack::ReadStack");
//		return false;
//	}

	for (int i=0; i<NImages; i++){
		AnaVisionImageFile image;
		image.Width = this->Width;
		image.Height = this->Height;
		if (!image.ReadImage(bytesavailable, file, Version)){
			Alert0("could not read an image in AnaVisionImageFileStack::ReadStack");
			return false;
		}
		this->Images.push_back(image);
	}
	return true;
}

bool AnaVisionImageFileStack::WriteStack(int & byteswritten, CFile & file){
	CFile * fp = &(file);
	unsigned __int32 NImages = this->Images.size();
	MyWriteVariable(NImages);
//	WriteBytesNoSeek(&file, 4, &NImages);//
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();
	pApp->MySaveString(byteswritten, fp, this->Title);
	//SaveString(this->Title, file);

	for (int i=0; i<NImages; i++){
		if (!this->Images[i].WriteImage(byteswritten, file)){
			Alert0("could not write an image in AnaVisionImageFileStack::WriteStack");
			return false;
		}
	}
	return true;
}

AnaVisionImageFileHyperStack::AnaVisionImageFileHyperStack(){
	Width=Height=0;
	SetVersion();
}
AnaVisionImageFileHyperStack::~AnaVisionImageFileHyperStack(){
}


bool AnaVisionImageFileHyperStack::AppendStack(AnaVisionImageFileStack & stack){ // adds to this hyperstack
	if (this->Stacks.size()<1){
		this->Width = stack.Width;
		this->Height = stack.Height;
	}
	else{
		if (stack.Width != this->Width){
			Alert0("width not equal in AnaVisionImageFileHyperStack::AppendStack");
			return false;
		}
		if (stack.Height != this->Height){
			Alert0("height not equal in AnaVisionImageFileHyperStack::AppendStack");
			return false;
		}
	}
	this->Stacks.push_back(stack);
	return true;

}

bool AnaVisionImageFileHyperStack::ReadHyperStackFromFile(int & bytesavailable, CFile & file){
	CFile * fp = &(file);
	if (!IsAnavisionHyperStack(bytesavailable, file)){

/*	char s[29]; s[28]=0;
	if (file.GetLength()<(28+4+4+4)){
		Alert0("file too short");
		return false;
	}
	file.Read(s, 28);
	CString c = CString(s);

	if (c.Compare("AnaVisionImageFileHyperStack") != 0){

*/
		Alert0("file does not start with 'AnaVisionImageFileHyperStack'");
		return false;
	}

	MyReadVariable(this->Version);
	MyReadVariable(this->Width);
//	ShowFloat(Width, "W");
/*	int result = ReadBytesNoSeek(&file, 4, &this->Width);
	if (!result){
		Alert0("could not read Width in AnaVisionImageFileHyperStack::ReadHyperStackFromFile");
		return false;
	}
*/
	MyReadVariable(this->Height);
//	ShowFloat(Height, "H");
/*	result = ReadBytesNoSeek(&file, 4, &this->Height);
	if (!result){
		Alert0("could not read Height in AnaVisionImageFileHyperStack::ReadHyperStackFromFile");
		return false;
	}
*/
	unsigned __int32 NStacks;
	MyReadVariable(NStacks);
//	ShowFloat(NStacks, "NS");
/*
result = ReadBytesNoSeek(&file, 4, &NStacks);
	if (!result){
		Alert0("could not read number of stacks in AnaVisionImageFileHyperStack::ReadHyperStackFromFile");
		return false;
	}
*/
	for (int i=0; i<NStacks; i++){
		AnaVisionImageFileStack stack;
		stack.Width = this->Width;
		stack.Height = this->Height;
		if (!stack.ReadStack(bytesavailable, file, this->Version)){
			Alert0("could not read a stack in AnaVisionImageFileHyperStack::ReadHyperStackFromFile");
			return false;
		}
		this->Stacks.push_back(stack);
	}
	SetVersion();
	return true;	

}
bool AnaVisionImageFileHyperStack::WriteHyperStackToFile(int & byteswritten, CFile & file){
	CFile * fp = &(file);
	CString c ="%AnaVisionImageFileHyperStack";
	WriteBytesNoSeek(&file, 29, c.GetBuffer());
	byteswritten += 29;

	MyWriteVariable(this->Version);
	MyWriteVariable(this->Width);
//	WriteBytesNoSeek(&file, 4, &this->Width);
//	WriteBytesNoSeek(&file, 4, &this->Height);
	MyWriteVariable(this->Height);
	int NStacks = this->Stacks.size();
//	WriteBytesNoSeek(&file, 4, &NStacks);
	MyWriteVariable(NStacks);
	for (int i=0; i<this->Stacks.size(); i++){
		if (!this->Stacks[i].WriteStack(byteswritten, file)){
			Alert0("Could not write stack in AnaVisionImageFileHyperStack::WriteHyperStackToFile");
			return false;
		}
	}
	return true;
}

bool AnaVisionImageFileHyperStack::IsAnavisionHyperStack(int & bytesavailable, CFile & file){ // test to read the first characters 
	char s[30]; s[29]=0;
	if (file.GetLength()<(28+4+4+4)){
//		Alert0("file too short");
		return false;
	}
	file.Read(s, 29);
	CString c = CString(s);
	bytesavailable -= 29;
	if (c.Compare("%AnaVisionImageFileHyperStack") != 0){
//		Alert0("not compare");
		return false;
//		Alert0("file does not start with 'AnaVisionImageFileHyperStack'");

	}
//	Alert0("hi");
	return true;

}

void AnaVisionImageFileHyperStack::SetVersion(){
	Version = VERSION_ANAVISION_IMAGE_FILE_FORMAT;
}