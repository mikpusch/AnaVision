
#include "stdafx.h"
#include "AnaVision.h"
#include <math.h>

Params::Params(){
	LaserFlimFreq = 80e6;
	SizeOfMembraneInPixels=3;
	NumberOfGradientPoints = 10;
	ROISAreClosedInMembraneAnalysis = true;

	xsizeimage = 640; // default 640
	ysizeimage = 480; // default 480

	FRETBTDonor=0.5645;
	FRETBTAcceptor=0.082;
	ThresholdFret = 1.0;
	CalcNormalizedFret = true;

	BackgroundA=188.0; // for ratio: A/B
	BackgroundB=53.0;
	NSurroundForAverage=0;

	MultiplierImages = 1.0; 
	OffsetImages = 0.0;

	SubtractInitialValuesForDraw = false;

//	AutoScale = false;
	AutoScale = true;
	AutoScaleSecondImage = true;
	MinManScale=0;
	MaxManScale= 4096;
	MinManScaleSecondImage=MinManScale;
	MaxManScaleSecondImage=MaxManScale;
	DeltaImage = 1;
	ConvertTopH = false; //true;
	WavelengthCombination = 0;
	ScaleRatio = 2000.0;

	FactorRatioIntoDeltaH = -73.125;

	MakeGreyScale(0);
	MakeGreyScale(1);

	LookupTableName = "";
	LookupTableNameSecondImage = LookupTableName;
	NBitsColorTable = 8;
	NBitsColorTableSecondImage = NBitsColorTable;

	A1_490_450  = 1.0286;
	A2_490_450  = 5.0008;
	x0_490_450  = 6.7569;
	dx_490_450  = 0.3844;

	A1_490_440  = 1.4491;
	A2_490_440  = 9.3006;
	x0_490_440  = 6.8665;
	dx_490_440  = 0.3827;

	A1_490_440  = 1.7978;
	A2_490_440  = 10.2078;
	x0_490_440  = 7.048;
	dx_490_440  = 0.4787;

	dt = 0.1; // 100 ms
	PixelSizeInMicrom = 9.9;
	Magnification = 10;

	linearconversionfactor=0.625;
	protonconversionmode=1;// 0 = theoretcal; 1 : linear; 2: ratio into DELTAH
    theoretical_pK=7.5;
	totalbuffermM=2;

	StepByOne = false;

	curspos[0] = 0.6;
	curspos[1] = 0.7;
	curspos[2] = 0.1;
	TrackDrift = false;
	InitialFrame0 = InitialFrame1= FrameToAnalyze0= FrameToAnalyze1=0;

	CPoint P1R = CPoint(100,100);
	CPoint P2R = CPoint(140,100);
	CPoint P3R = CPoint(140,140);
	CPoint P4R = CPoint(100,140);

	CPoint P1C = CPoint(80,140);
	CPoint P2C = CPoint(160,140);
	CPoint P3C = CPoint(140,100);
	CPoint P4C = CPoint(100,100);

	RectOb.Define(0, P1R, P2R, P3R, P4R, xsizeimage, ysizeimage);
	ConusOb.Define(1,  P1C, P2C, P3C, P4C, xsizeimage, ysizeimage );

	ViewType = 1; //	drawob = &(ConusOb);
	ViewType = 2; //


}
AreaObject * Params::drawob(){
	if (ViewType == 0){
		return &(RectOb);
	}
	return &(ConusOb);
}

void Params::MakeGreyScale(int WhichImage){
	COLORREF * c1 = &(ColorTable256[0]);
	COLORREF * c2 = &(ColorTable65536[0]);
	if (WhichImage == 1){
		c1 = &(ColorTable256SecondImage[0]);
		c2 = &(ColorTable65536SecondImage[0]);
	}
	int i;
	for (i=0; i<255; i++){
		c1[i]=RGB(i,i,i);
	}
	for (i=0; i<65536; i++){
		c2[i]=RGB(i/256,i/256,i/256);
	}
}

void Params::MakeGreenScale(int WhichImage){
	COLORREF * c1 = &(ColorTable256[0]);
	COLORREF * c2 = &(ColorTable65536[0]);
	if (WhichImage == 1){
		c1 = &(ColorTable256SecondImage[0]);
		c2 = &(ColorTable65536SecondImage[0]);
	}
	int i;
	for (i=0; i<255; i++){
		c1[i]=RGB(0,i,0);
	}
	for (i=0; i<65536; i++){
		c2[i]=RGB(0,i/256,0);
	}
}
void Params::MakeRedScale(int WhichImage){
	COLORREF * c1 = &(ColorTable256[0]);
	COLORREF * c2 = &(ColorTable65536[0]);
	if (WhichImage == 1){
		c1 = &(ColorTable256SecondImage[0]);
		c2 = &(ColorTable65536SecondImage[0]);
	}
	int i;
	for (i=0; i<255; i++){
		c1[i]=RGB(i,0,0);
	}
	for (i=0; i<65536; i++){
		c2[i]=RGB(i/256,0,0);
	}
}
void Params::MakeBlueScale(int WhichImage){
	COLORREF * c1 = &(ColorTable256[0]);
	COLORREF * c2 = &(ColorTable65536[0]);
	if (WhichImage == 1){
		c1 = &(ColorTable256SecondImage[0]);
		c2 = &(ColorTable65536SecondImage[0]);
	}
	int i;
	for (i=0; i<255; i++){
		c1[i]=RGB(0,0,i);
	}
	for (i=0; i<65536; i++){
		c2[i]=RGB(0,0,i/256);
	}
}


Params::~Params(){
}



bool Params::LoadASCIITable(CFile & file, int WhichImage){
	COLORREF * c1 = &(ColorTable256[0]);
	COLORREF * c2 = &(ColorTable65536[0]);
	if (WhichImage == 1){
		c1 = &(ColorTable256SecondImage[0]);
		c2 = &(ColorTable65536SecondImage[0]);
	}
	CString Name = file.GetFilePath();
	file.Close();
	FILE * f = fopen(Name, "r");
	if (!f){
		return false;
	}

	char s[200];
	fread( s, 1, 20, f );

	for (int i=0; i<255; i++){
		int index, red, green, blue;
		if (!ReadInt(index, f)){
			fclose(f);
			return false;
		}
//		ShowFloat(index, "index");
		if (!ReadInt(red, f)){
			fclose(f);
			return false;
		}
//		ShowFloat(red, "red");
		if (!ReadInt(green, f)){
			fclose(f);
			return false;
		}
//		ShowFloat(green, "gree");
		if (!ReadInt(blue, f)){
			fclose(f);
			return false;
		}
//		ShowFloat(blue, "blue");
		if (index<0){
			fclose(f);
			return false;
		}
		if (index>255){
			fclose(f);
			return false;
		}
		BYTE r = red;
		BYTE g = green;
		BYTE b = blue;
//		c1[index]=RGB(r, g, b);
		c1[index]=RGB(b, g, r);
//		ColorTable256[index]=RGB(r, g, b);
	}
	fclose(f);
	return true;
}

bool Params::DoLoadLookupTable(CFile & file, int WhichImage){
//	Alert0("hi do load lookuptabel");
	COLORREF * c1 = &(ColorTable256[0]);
	COLORREF * c2 = &(ColorTable65536[0]);
	if (WhichImage == 1){
		c1 = &(ColorTable256SecondImage[0]);
		c2 = &(ColorTable65536SecondImage[0]);
	}

	unsigned __int64 l=file.GetLength();
	if (l<(256*3)){
		return false;
	}
	file.SeekToBegin();
	char s[21];
	file.Read(s, 20);
	s[20]=0;

	CString c = CString(s);
	CString left = c.Left(5);

	if (left.Compare("Index") == 0){
		bool result = LoadASCIITable(file, WhichImage);
		if (!result){
			Alert0("Error loading ASCII color table");
		}
		else{
//			Alert0("Loaded ASCII");
		}
		return result;
	}
	file.SeekToBegin();

	int i;
	BYTE x[256][3];
	file.Read(&(x[0][0]), 3*256);
	for (i=0; i<255; i++){
//		ShowFloat(x[0], "x[0]");
//		ShowFloat(x[1], "x[1]");
//		ShowFloat(x[2], "x[2]");//

//		ColorTable256[i] = RGB(x[i][0], x[i][1], x[i][2]);
		c1[i] = RGB(x[i][0], x[i][1], x[i][2]);
	}
	return true;
}

bool Params::TryLoadLookupTable(int WhichImage){
//	Alert0("hi try do load lookuptabel");
	CFile f;
//	this->MakeGreyScale(WhichImage);
	if (!f.Open(LookupTableName, CFile::modeRead)){
//		Alert0("could not open file");
//		this->MakeGreyScale(WhichImage);
		return false;
	}

	if (!DoLoadLookupTable(f, WhichImage)){
//		this->MakeGreyScale(WhichImage);
		return false;
	}

//	Alert0("succeed");

	return true;

}
bool Params::ReadFromFile(CFile * fp){
	return ReadFromFile(fp, -90000000);
//	return ReadFromFile(fp, 8);
}



bool Params::ReadFromFile(CFile * fp, int bytesavailable){
	bool CheckBytesAvailable=true;
	if (bytesavailable<1){
		bytesavailable = 90000000;
		CheckBytesAvailable = false;
	}

//	ShowFloat(bytesavailable, "bytesavailable");
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	MyReadVariable(xsizeimage);
	MyReadVariable(ysizeimage);
	MyReadVariable(AutoScale);
	MyReadVariable(MinManScale);
	MyReadVariable(MaxManScale);
	MyReadVariable(DeltaImage);
	MyReadVariable(ConvertTopH);
	MyReadVariable(WavelengthCombination);
	MyReadVariable(ScaleRatio);
	if (!pApp->MyLoadString(bytesavailable, fp, LookupTableName)){
		return true;
	}

	if (!TryLoadLookupTable()){
	}

	MyReadVariable(A1_490_450);
	MyReadVariable(A2_490_450);
	MyReadVariable(x0_490_450);
	MyReadVariable(dx_490_450); 

	MyReadVariable(A1_490_440);
	MyReadVariable(A2_490_440);
	MyReadVariable(x0_490_440);
	MyReadVariable(dx_490_440); 

	MyReadVariable(dt); 

	MyReadVariable(PixelSizeInMicrom);
	MyReadVariable(Magnification);


    MyReadVariable(theoretical_pK);
	MyReadVariable(totalbuffermM);
	MyReadVariable(linearconversionfactor);
	MyReadVariable(protonconversionmode);// 0 = theoretcal; 1 : linear
    MyReadVariable(StepByOne);

	int i;
	for (i=0; i<NCURSORS; i++){
		MyReadVariable(curspos[i]);
	}
	MyReadVariable(TrackDrift);

	MyReadVariable(InitialFrame0);
	MyReadVariable(InitialFrame1);
	MyReadVariable(FrameToAnalyze0);
	MyReadVariable(FrameToAnalyze1);

	for (i=0; i<4; i++){
		MyReadVariable(RectOb.P[i].pos.x);
		MyReadVariable(RectOb.P[i].pos.y);
	}
	MyReadVariable(RectOb.ObjectType);
	for (i=0; i<4; i++){
		MyReadVariable(ConusOb.P[i].pos.x);
		MyReadVariable(ConusOb.P[i].pos.y);
	}
	MyReadVariable(ConusOb.ObjectType);

	MyReadVariable(ViewType);

	if (bytesavailable==0) return true;

	MyReadVariable(FactorRatioIntoDeltaH);

	if (bytesavailable==0) return true;

	RectOb.CalcGeneratorList();
	ConusOb.CalcGeneratorList();

	MyReadVariable(BackgroundA);

	if (bytesavailable==0) return true;
	MyReadVariable(BackgroundB);

	if (bytesavailable==0) return true;
	MyReadVariable(NSurroundForAverage);

	if (bytesavailable==0) return true;
	MyReadVariable(SubtractInitialValuesForDraw);

	if (bytesavailable==0) return true;
	MyReadVariable(MultiplierImages);
	
	if (bytesavailable==0) return true;
	MyReadVariable(OffsetImages);

	if (bytesavailable==0) return true;
	MyReadVariable(AutoScaleSecondImage);

	if (bytesavailable==0) return true;
	MyReadVariable(MinManScaleSecondImage);

	if (bytesavailable==0) return true;
	MyReadVariable(MaxManScaleSecondImage);

	if (bytesavailable==0) return true;
	if (!pApp->MyLoadString(bytesavailable, fp, LookupTableNameSecondImage)){
		return true;
	}

	if (bytesavailable==0) return true;

	MyReadVariable(FRETBTDonor);
	if (bytesavailable==0) return true;

	MyReadVariable(FRETBTAcceptor);

	if (bytesavailable==0) return true;

	MyReadVariable(ThresholdFret);

	if (bytesavailable==0) return true;

	MyReadVariable(CalcNormalizedFret);

	if (bytesavailable==0) return true;

	MyReadVariable(LaserFlimFreq);

	if (CheckBytesAvailable){
		while (bytesavailable>0){
			char c;
			MyReadVariable(c);
		}
	}

//	ShowFloat(bytesavailable, "bytesavailable");
	return true;
}



int Params::SaveOnFile(CFile * fp){
	CAnaVisionApp * pApp = (CAnaVisionApp *) AfxGetApp();

	int byteswritten = 0;
	MyWriteVariable(xsizeimage);
	MyWriteVariable(ysizeimage);
	MyWriteVariable(AutoScale);

	MyWriteVariable(MinManScale);
	MyWriteVariable(MaxManScale);
	MyWriteVariable(DeltaImage);
	MyWriteVariable(ConvertTopH);
	MyWriteVariable(WavelengthCombination);
	MyWriteVariable(ScaleRatio);
	pApp->MySaveString(byteswritten, fp, LookupTableName);
	MyWriteVariable(A1_490_450);
	MyWriteVariable(A2_490_450);
	MyWriteVariable(x0_490_450);
	MyWriteVariable(dx_490_450); 
	MyWriteVariable(A1_490_440);
	MyWriteVariable(A2_490_440);
	MyWriteVariable(x0_490_440);
	MyWriteVariable(dx_490_440); 
	MyWriteVariable(dt); 
	MyWriteVariable(PixelSizeInMicrom);
	MyWriteVariable(Magnification);
	MyWriteVariable(theoretical_pK);
	MyWriteVariable(totalbuffermM);
	MyWriteVariable(linearconversionfactor);
	MyWriteVariable(protonconversionmode);// 0 = theoretcal; 1 : linear
    MyWriteVariable(StepByOne);
	int i;
	for (i=0; i<NCURSORS; i++){
		MyWriteVariable(curspos[i]);
	}
	MyWriteVariable(TrackDrift);
	MyWriteVariable(InitialFrame0);
	MyWriteVariable(InitialFrame1);
	MyWriteVariable(FrameToAnalyze0);
	MyWriteVariable(FrameToAnalyze1);

	for (i=0; i<4; i++){
		MyWriteVariable(RectOb.P[i].pos.x);
		MyWriteVariable(RectOb.P[i].pos.y);
	}
	MyWriteVariable(RectOb.ObjectType);
	for (i=0; i<4; i++){
		MyWriteVariable(ConusOb.P[i].pos.x);
		MyWriteVariable(ConusOb.P[i].pos.y);
	}
	MyWriteVariable(ConusOb.ObjectType);

	MyWriteVariable(ViewType);
	MyWriteVariable(FactorRatioIntoDeltaH);

	MyWriteVariable(BackgroundA);
	MyWriteVariable(BackgroundB);
	MyWriteVariable(NSurroundForAverage);
	MyWriteVariable(SubtractInitialValuesForDraw);
	MyWriteVariable(MultiplierImages);
	MyWriteVariable(OffsetImages);

	MyWriteVariable(AutoScaleSecondImage);
	MyWriteVariable(MinManScaleSecondImage);
	MyWriteVariable(MaxManScaleSecondImage);
	pApp->MySaveString(byteswritten, fp, LookupTableNameSecondImage);

	MyWriteVariable(FRETBTDonor);
	MyWriteVariable(FRETBTAcceptor);
	MyWriteVariable(ThresholdFret);
	MyWriteVariable(CalcNormalizedFret);
	MyWriteVariable(LaserFlimFreq);

//	ShowFloat(byteswritten, "byteswritten");
	return byteswritten;
}


void Params::Convert(ImageDataType ** image, int xsize, int ysize){
//	if ( (offsetx != 0) || (offsety != 0)){
	if ( false ){
		ImageDataType * CopyImage = new ImageDataType[xsize*ysize];
		for (int x=0; x<xsize; x++){
			int offx=x*ysize;
			for (int y=0; y<ysize; y++){
				CopyImage[y+offx] = image[x][y];
				image[x][y]=0;
			}
		}
		for (int x=0; x<xsize; x++){
			int xx = x+offsetx; 
			if ((xx>=0) && (xx<xsize)){
				int offx=x*ysize;
				for (int y=0; y<ysize; y++){
					int yy =y+offsety;
					if ((yy>=0) && (yy<ysize)) {
						image[xx][yy] = CopyImage[y+offx];
					}
				}
			}
		}
		delete [] CopyImage;
	}


	if (!ConvertTopH) return ;

	double A1, A2, x0, dx;

	switch (WavelengthCombination){
		case 0 : A1 = A1_490_450; A2 = A2_490_450; x0 = x0_490_450; dx = dx_490_450; break;
		case 1 : A1 = A1_490_440; A2 = A2_490_440; x0 = x0_490_440; dx = dx_490_440; break;
		default : A1 = A1_490_450; A2 = A2_490_450; x0 = x0_490_450; dx = dx_490_450; break;
	}


	for (int x=0; x<xsize; x++){
		for (int y=0; y<ysize; y++){
			double d = image[x][y]/ScaleRatio;
			if (d<=A1){
				image[x][y] = 5.0;
			}
			else{
				if (d>=A2){
					image[x][y] = 7.8;
				}
				else{
					image[x][y] = x0 + dx*log(fabs(( d - A1)/ (A2 - d)));
				}
			}
		}
	}

}

double Params::ConvertpHToProton(double oldpH, double newpH){
	
	switch (protonconversionmode){
		case 0 : return TheoreticalConvertpHToProton(oldpH, newpH);
		case 1 :  return LinearConvertpHToProton(oldpH, newpH);
		case 2 :  return RatioToProtonChange(oldpH, newpH);
		default :  return TheoreticalConvertpHToProton(oldpH, newpH);
	}
}

double Params::RatioToProtonChange (double oldratio, double newratio){
//	ShowFloat(oldratio, "ratio");
	return (oldratio-newratio)*FactorRatioIntoDeltaH/1000.0/ScaleRatio; // from microM to millimolar
}

double Params::TheoreticalConvertpHToProton(double oldpH, double newpH){

	double K=pow(10,-theoretical_pK)*1000;	// in mM

	double H0=pow(10,-oldpH)*1000.0;		// in mM
	double BH0= H0*totalbuffermM/(K+H0);	// in mM
	
	double H1=pow(10,-newpH)*1000.0;		// in mM
	double BH1= H1*totalbuffermM/(K+H1);	// in mM
	// calcoli ....

	return (H1+BH1-H0-BH0);					// in mM



}
double Params::LinearConvertpHToProton(double oldpH, double newpH){
	return linearconversionfactor*(oldpH-newpH);
}



int Params::SaveParamsFile(){
   CWinApp* pApp = AfxGetApp();

   CString strSection       = SettingsSection;
   CString strStringItem    = SettingsItem;

   CString OutFile;
   OutFile=pApp->GetProfileString(strSection, strStringItem);

//   static char BASED_CODE szFilter[] = "Set Files (*.set) | All Files (*.*) | *.* ||";

	CString Filter = CString("Set Files (*.set) |*.set| All Files (*.*) |*.*||");

   CFileDialog
    diag( false, CString(".set"), OutFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, Filter, AfxGetMainWnd());

   if (diag.DoModal()==IDOK){
  	 OutFile = diag.GetPathName();
     CFile file ( OutFile, CFile::modeCreate | CFile::modeWrite);
     pApp->WriteProfileString(strSection, strStringItem, OutFile);
     SaveOnFile(&file);
     file.Close();
	 return 1;
   }
   return -1;
}

int Params::LoadParamsFile(bool ask){
   CWinApp* pApp = AfxGetApp();

   CString strSection       = SettingsSection;
   CString strStringItem    = SettingsItem;

   CString InFile;
   InFile=pApp->GetProfileString(strSection, strStringItem);

  
//static char BASED_CODE szFilter[] = "Set Files (*.set) | *.set | All Files (*.*) | *.* ||";

   if (ask){
  
		CString Filter = CString("Set Files (*.set) |*.set| All Files (*.*) |*.*||");
		CFileDialog diag( true, CString("*.set"),
						InFile, OFN_FILEMUSTEXIST, Filter, AfxGetMainWnd());
		if (diag.DoModal()==IDOK){
			InFile = diag.GetPathName();
		}
		else{
			return -1;
		}
   }
   CFile file ;
//	Alert0(InFile);
   
   if (!file.Open( InFile, CFile::modeRead)){
	   return -1;
   }

   bool result=ReadFromFile(&file);
   if (!result){
//		AfxThrowArchiveException(CArchiveException::badIndex);
	    return -1;
	}
	pApp->WriteProfileString(strSection, strStringItem, InFile);
     file.Close();
	 return 1;
 
	return -1;

}

void Params::CheckCursors(){
	for (int i=0; i<2; i++){
		if (curspos[i]<0) curspos[i]=0;
		if (curspos[i]>1) curspos[i]=1;
	}

	if (curspos[0]>curspos[1]){
		double x=curspos[0];
		curspos[0]=curspos[1];
		curspos[1]=x;
	}
}


