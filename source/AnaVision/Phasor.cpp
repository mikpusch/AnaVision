#include "stdafx.h"
#include "Phasor.h"
#include "AnaVision.h"
#include "../common/simplex.h"

Phasor::Phasor(){
	G=.5; S=0.5;
};
Phasor::Phasor(double g, double s){
	G=g;
	S=s;
}
Phasor::~Phasor(){
};
void Phasor::CalcTauMPhi(double fLaser){
	double omega = TWOPI_GENERAL_FILTER*fLaser;
	tau_in_ns = 1e9*S/G/omega;
	m = Length();
	phi = atan(S/G);
};

double Phasor::Convert(int ImageValue){
	return double(ImageValue)/32768.0 - 1.0;
}
Phasor Phasor::Add(Phasor A, Phasor B){
	return Phasor(A.G+B.G, A.S+B.S);
}
Phasor Phasor::Subtract(Phasor A, Phasor B){
	return Phasor(A.G-B.G, A.S-B.S);
}
double Phasor::Length(){
	return Xsqrt(G*G + S*S);
}
Phasor Phasor::ScalarMult(double lambda, Phasor A){
	return Phasor(lambda*A.G, lambda*A.S);
}

bool Phasor::IsInCircle(Phasor Midpoint, double RSquare){
	double x=this->G-Midpoint.G;
	double y=this->S-Midpoint.S;
	return ((x*x+y*y)<=RSquare);
}


NumberList::NumberList(){
};
NumberList::~NumberList(){
};
int NumberList::ReadFromFile(std::ifstream & f){
	numbers.resize(0);
	std::string c;
	while (std::getline(f, c)){
		extractIntegerWords(c, numbers);
//		while (true){//(ScanDoubleFromStringNoComma(c, d)){
			//	while (ReadDoubleFromASCIIFile(d, f)){
//			extractIntegerWords(c, numbers)
//			numbers.push_back(d);
//		}
	}
	return numbers.size();
};

PhasorImage::PhasorImage(double fLaser){
	FLASER = fLaser;
	sizex= sizey=0;
	initialized = false;
};
void PhasorImage::SetLaserFreq(double fLaser){
	FLASER = fLaser;
};

PhasorImage::~PhasorImage(){
}

bool PhasorImage::GetImagesFromBuffer(int sx, int sy, vector <double> & G, vector <double> & S){
	initialized = false;
	sizex = sx;
	sizey = sy;
	imagedata.resize(sy);
	for (int iy=0; iy<sy; iy++){
		imagedata[iy].resize(sx);
	}
	int i=0;
	for (int iy=0; iy<sy; iy++){
		for (int ix=0; ix<sx; ix++){
			imagedata[iy][ix].G = G[i];
			imagedata[iy][ix].S = S[i];
			imagedata[iy][ix].CalcTauMPhi(FLASER);
			i++;
		}
	}
	initialized = true;
	return true;
}

bool PhasorImage::ReadFromTextFiles(int sx, int sy,
									std::ifstream & freal, std::ifstream &fimag){
	NumberList reallist;
	int NReal = reallist.ReadFromFile(freal);
	//ShowFloat(NReal, "Nreal");
	if (NReal<=0){
		Alert0("0 real numbers");
		return false;
	}
	NumberList imaglist;
	int NImag = imaglist.ReadFromFile(fimag);
//	ShowFloat(NImag, "NImag");
	if (NImag<=0){
		Alert0("0 imaginary numbers");
		return false;
	}
	if (NImag != NReal){
		Alert0("real and imaginary numbers do not coincide");
		return false;
	}
	int NMin = sx*sy;
	if (NReal < NMin){
		Alert0("not enough numbers");
		return false;
	}
	if ((NReal % NMin) != 0){
		Alert0("Number is not multiple of sizex*sizey");
		return false;
	}
	int NImages = NReal / NMin;
	//ShowFloat(NImages, "images");
	int iimage = 0;
	if (NImages>1){
		io myio;
		char s[20];
		_itoa(NImages, s, 10);
		Alert0("There are" + CString(s) + " images");
		if (myio.InInt(iimage, "Which image? (0 = first, 1 = second...") != IDOK){
			return false;
		}
		if (iimage<0){
			return false;
		}
		if (iimage>=NImages){
			return false;
		}
	}
	int Offset = NMin*iimage;
	int i=0;
	sizex = sx;
	sizey = sy;
	imagedata.resize(sy);
	for (int iy=0; iy<sy; iy++){
		imagedata[iy].resize(sx);
	}
	int N32G=0;
	int N32S=0;
	for (int iy=0; iy<sy; iy++){
		for (int ix=0; ix<sx; ix++){
			imagedata[iy][ix].G = Phasor::Convert(reallist.numbers[i]);
			imagedata[iy][ix].S = Phasor::Convert(imaglist.numbers[i]); ///2.0;
			imagedata[iy][ix].CalcTauMPhi(FLASER);
			i++;
		}
	}
	initialized = true;
	return true;
}

int PhasorImage::CalcNumberOfPhasorsPerPixel(double GMin, double GMax, 
											 double SMin, double SMax,
											int sizeXRect, int sizeYRect,
											bool UsePolygon, PolygonObject * poly,
											vector < vector <int > > & N){
	for (int ry=0; ry<sizeYRect; ry++){
		for (int rx=0; rx<sizeXRect; rx++){
			N[ry][rx] = 0;
		}
	}
	if (!initialized){
		return 0;
	}
	if (!poly){
		UsePolygon = false;
	}
	double scaleX = double(sizeXRect)/(GMax-GMin);
	double scaleY = double(sizeYRect)/(SMax-SMin);

	double SSMin = SMin;
	double GGMin = GMin;
	for (int iy=0; iy<sizey; iy++){
		for (int ix=0; ix<sizex; ix++){
			bool use = true;
			if (UsePolygon){
				if (!poly->IsInside(ix, iy)){
					use = false;
				}
			}
			if (use){
				double S = imagedata[iy][ix].S;
				double G = imagedata[iy][ix].G;

				if ((S>=SSMin) && (S<=SMax) && (G>GGMin) && (G<=GMax) ){
					int rx = int((imagedata[iy][ix].G-GMin)*scaleX);
					int ry = int((imagedata[iy][ix].S-SMin)*scaleY);
					N[ry][rx]++;
				}
			}
			else{
				//ShowFloat(ix, "ix");
				//ShowFloat(iy, "iy");
			}
		}
	}
	int NMax = 0;
	for (int ry=0; ry<sizeYRect; ry++){
		for (int rx=0; rx<sizeXRect; rx++){
			if (N[ry][rx]>NMax){
				NMax = N[ry][rx];
			}
		}
	}
	return NMax;
}
 
void PhasorImage::WriteMask(Phasor p0, double R,
							vector < vector < bool>> &WhiteMask){

	if (!initialized) return;
//	Phasor p0(G0, S0);
	double R2 = R*R;
	for (int iy=0; iy<sizey; iy++){
		for (int ix=0; ix<sizex; ix++){
			double G = imagedata[iy][ix].G;
			double S = imagedata[iy][ix].S;
//			double dg = G-G0;
//			double ds = S-S0;
			WhiteMask[iy][ix] = (Phasor::SquareDistPhasor(p0, Phasor(G,S)) <= R2);
		}
	}
}

bool PhasorImage::ReadFromTiffFiles(int &sx, int &sy,
									CString FileNameG, CString FileNameS){
	AnaVisionTif GTif;
	if (!GTif.OpenStandardTif(FileNameG)){
		Alert0("could not open G Tif file");
		return false;
	}
	int NChan = GTif.GetNumberOfChannels();
	//ShowFloat(NChan, "NChan");
	int NImagesPerChannel = GTif.GetNumberOfImagesPerChannel();

	int w, h;
	GTif.GetImageSize(w, h);

	AnaVisionTif STif;
	if (!STif.OpenStandardTif(FileNameS)){
		Alert0("could not open S Tif file");
		return false;
	}

	if (STif.GetNumberOfChannels() != NChan){
		Alert0("different number of channels");
		return false;
	}		
	if (STif.GetNumberOfImagesPerChannel() != NImagesPerChannel){
		Alert0("different number of images per channel");
		return false;
	}		
	int ws, hs;
	STif.GetImageSize(ws, hs);
	if ((ws!=w) || (hs!=h)){
		Alert0("different image sizes");
	}
	int SIZEXY = w*h;
	vector <unsigned __int16> GNumbers, SNumbers;
	GNumbers.resize(SIZEXY);
	if (!GTif.ReadImage(0,0, &GNumbers[0])){
		Alert0("could not read G Tif");
		return false;
	}
	SNumbers.resize(SIZEXY);
	if (!STif.ReadImage(0,0, &SNumbers[0])){
		Alert0("could not read S Tif");
		return false;
	}
	sizex = sx = w;
	sizey = sy = h;
	imagedata.resize(sy);
	for (int iy=0; iy<sy; iy++){
		imagedata[iy].resize(sx);
	}
	int i=0;
	for (int iy=0; iy<sy; iy++){
		for (int ix=0; ix<sx; ix++){
			imagedata[iy][ix].G = Phasor::Convert(GNumbers[i]);
			imagedata[iy][ix].S = Phasor::Convert(SNumbers[i]); ///2.0;
			imagedata[iy][ix].CalcTauMPhi(FLASER);
			i++;
		}
	}
	initialized = true;
	return true;
}

void PhasorImage::Transpose(){
	if (!initialized){
		return;
	}
	int NNumbers = sizex*sizey;

	vector<double> GVect;
	vector<double> SVect;

	GVect.resize(NNumbers);
	SVect.resize(NNumbers);

	int i=0;
	for (int iy=0; iy<sizey; iy++){
		for (int ix=0; ix<sizex; ix++){
			GVect[i]= imagedata[iy][ix].G;
			SVect[i]= imagedata[iy][ix].S;
			i++;
		}
	}

	if (sizex != sizey){
		for (int iy=0; iy<sizey; iy++){
			imagedata[iy].resize(0);
		}
		int temp=sizex;
		sizex=sizey;
		sizey=temp;
		imagedata.resize(sizey);
		for (int iy=0; iy<sizey; iy++){
			imagedata[iy].resize(sizex);
		}
	}

	i=0;
		for (int ix=0; ix<sizex; ix++){
	for (int iy=0; iy<sizey; iy++){
			imagedata[iy][ix].G = GVect[i];
			imagedata[iy][ix].S = SVect[i];
			imagedata[iy][ix].CalcTauMPhi(FLASER);
			i++;
		}
	}
}
double Phasor::SquareDistPhasor(Phasor A, Phasor B){
	double dg=A.G-B.G;
	double ds=A.S-B.S;
	return dg*dg+ds*ds;
}

double Phasor::DistPhasor(Phasor A, Phasor B){
	return sqrt(SquareDistPhasor(A, B));
}

Phasor Phasor::GetIntersection(Phasor BG, Phasor PD){
	double m = (PD.S-BG.S)/(PD.G-BG.G);
	double b = BG.S - m*BG.G;
	double a = 1.0+m*m;
	double p=(1.0-2.0*m*b)/2.0/a;
	double q = b*b/a;
	double r=Xsqrt(p*p-q);
	double x1=p-r;
	double y1=m*x1+b;
	double x2=p+r;
	double y2=m*x2+b;

	if (y1<0){
		return Phasor(x2, y2);
	}
	if (y2<0){
		return Phasor(x1, y1);
	}
	double d1=SquareDistPhasor(Phasor(x1,y1), PD);
	double d2=SquareDistPhasor(Phasor(x2,y2), PD);

	if (d1<d2){
		return Phasor(x1, y1);
	}
	else{
		return Phasor(x2, y2);
	}
}

void Phasor::OldCalcTrajectoryPoint(Phasor BG, Phasor PD, double E, double frac, Phasor & R){
	Phasor P0 = GetIntersection(BG, PD);
	Phasor BGandpD((P0.G+BG.G)/2.0, (P0.S+BG.S)/2.0);
//	R=P0;
//	double frac=Phasor::DistPhasor(BG, P0);
	double taud=P0.S/P0.G;
	//taud = PD.S/PD.G;
	double tauq=(1.0-E)*taud;
//	PQ.S/PQ.G = tauq;9
	double temp = 1.0/(1.0 + tauq*tauq);
	Phasor P1(temp, tauq*temp);

//	double fracg = (PD.G-P1.G)/(BG.G-P1.G);
//	double fracs = (PD.S-P1.S)/(BG.S-P1.S);

	double temp1 = (1.0-frac)*(1.0-E);
	double Nenner = temp1+frac;
	R.G = (temp1*P1.G + frac*BG.G)/Nenner;
	R.S = (temp1*P1.S + frac*BG.S)/Nenner;

	R.G = (temp1*P1.G + frac*BGandpD.G)/Nenner;
	R.S = (temp1*P1.S + frac*BGandpD.S)/Nenner;

	R.G=(1.0-E)*PD.G+E*(frac*P1.G + (1.0-frac)*PD.G);
	R.S=(1.0-E)*PD.S+E*(frac*P1.S + (1.0-frac)*PD.S);

}

Phasor Phasor::CalcTrajectoryPoint(Phasor BG, Phasor PD, double E, double frac){
	Phasor P0= GetIntersection(BG, PD);
	Phasor P0PD = Phasor::Subtract(P0, PD);
	Phasor P0BG = Phasor::Subtract(P0, BG);
	double FractionNonBG = P0PD.Length()/P0BG.Length();

	double taud=P0.S/P0.G;

	double tauq=(1.0-E)*taud;
	double temp = 1.0/(1.0 + tauq*tauq);

	Phasor PQ(temp, tauq*temp);

	double Nenner = (1.0-FractionNonBG)*(1.0-E) + FractionNonBG;

	Phasor P2 = Phasor::ScalarMult((1.0-FractionNonBG)*(1.0-E)/Nenner, PQ);
	Phasor P3 = Phasor::ScalarMult(FractionNonBG/Nenner, BG);
	Phasor P4 = Phasor::Add(P2, P3);
	return Phasor::Add(ScalarMult(frac, P4), ScalarMult((1.0-frac), PD));

}
Phasor Phasor::CalcPasqua24TrajectoryPoint1(Phasor BG, Phasor PD, double E, double frac){
	Phasor P0= GetIntersection(BG, PD);
	Phasor P0PD = Phasor::Subtract(P0, PD);
	Phasor P0BG = Phasor::Subtract(P0, BG);
	double FractionBG = P0PD.Length()/P0BG.Length();
	//ShowFloat(FractionBG , "FractionBG ");

	double taud=P0.S/P0.G;

	double tauq=(1.0-E)*taud;
	double temp = 1.0/(1.0 + tauq*tauq);

	Phasor PQ(temp, tauq*temp);

	double FractionBGFret = FractionBG/(FractionBG+(1.0-E)*(1.0-FractionBG));
	Phasor P2 = Phasor::ScalarMult((1.0-FractionBGFret), PQ);
	Phasor P3 = Phasor::ScalarMult(FractionBGFret, BG);
	Phasor P4 = Phasor::Add(P2, P3);

	double NDdivNB = (1.0-FractionBG)/FractionBG;
	double tp1=(1.0-frac)*(NDdivNB + 1);
//	double tp2=frac*(NDdivNB*(1.0-E)+ FractionBGFret);
	double tp2=frac*(NDdivNB*(1.0-E)+ 1);
	double Nenner = tp1 + tp2;
	return Phasor::Add(ScalarMult(tp1/Nenner, PD), ScalarMult(tp2/Nenner, P4));

}

Phasor Phasor::CalcPasqua24TrajectoryPointBis1(Phasor BG, Phasor PD, double E, double frac){
	Phasor P0= GetIntersection(BG, PD);
	Phasor P0PD = Phasor::Subtract(P0, PD);
	Phasor P0BG = Phasor::Subtract(P0, BG);
	double FractionBG = P0PD.Length()/P0BG.Length();
	//ShowFloat(FractionBG , "FractionBG ");

	double taud=P0.S/P0.G;

	double tauq=(1.0-E)*taud;
	double temp = 1.0/(1.0 + tauq*tauq);

	Phasor PQ(temp, tauq*temp);

	double FractionBGFret = FractionBG/(FractionBG+(1.0-E)*(1.0-FractionBG));
	Phasor P2 = Phasor::ScalarMult((1.0-FractionBGFret), PQ);
	Phasor P3 = Phasor::ScalarMult(FractionBGFret, BG);
	Phasor P4 = Phasor::Add(P2, P3);

	double NDdivNB = (1.0-FractionBG)/FractionBG;
	double tp1=(1.0-frac)*NDdivNB;
	double tp2=1;
	double tp3=frac*NDdivNB*(1.0-E);
	double Nenner = tp1 + tp2 +  tp3;
	return Phasor((BG.G*tp2 + P0.G*tp1 + PQ.G*tp3)/Nenner, (BG.S*tp2 + P0.S*tp1 + PQ.S*tp3)/Nenner);

//	Phasor temp1 = Phasor::Add(ScalarMult(tp1/Nenner, P0), ScalarMult(tp3/Nenner, PQ));

//	return Phasor::Add(ScalarMult(tp2/Nenner, BG), temp1);

}


Phasor Phasor::CalcTryLeicaTrajectoryPoint(Phasor BG, Phasor PD, double E, double frac){
	Phasor P0= GetIntersection(BG, PD);
	Phasor P0PD = Phasor::Subtract(P0, PD);
	Phasor P0BG = Phasor::Subtract(P0, BG);
	double FractionNonBG = P0PD.Length()/P0BG.Length();
//	ShowFloat(FractionNonBG, "FractionNonBG");

	double taud=P0.S/P0.G;

	double tauq=(1.0-E)*taud;
	double temp = 1.0/(1.0 + tauq*tauq);

	Phasor PQ(temp, tauq*temp);

	double Nenner = (1.0-FractionNonBG)*(1.0-E) + FractionNonBG;

	Phasor P2 = Phasor::ScalarMult((1.0-FractionNonBG)*(1.0-E)/Nenner, PQ);
	Phasor P3 = Phasor::ScalarMult(FractionNonBG/Nenner, BG);
	Phasor P4 = Phasor::Add(P2, P3);
	
//	P2 = Phasor::ScalarMult(1.0-E, PQ);
//	P3 = Phasor::ScalarMult(FractionNonBG, BG);
//	P4 = Phasor::Add(P2, P3);
//	return Phasor::Add(ScalarMult((1.0-E)*frac, P4), ScalarMult(E*(1.0-frac), PD));

	return Phasor::Add(ScalarMult(frac, P4), ScalarMult((1.0-frac), PD));

}


double PhasorImage::CalcSD(double * Params){
	Phasor P0(Params[0], Params[1]);
	if (Phasor::SquareDistPhasor(FitPhasorCircle, P0)>FitRadiusSquare){
		return 1e299;
	}
	double variance = Params[2];
	if (variance<=0.0){
		return 1e299;
	}

	double sum1=0;
	double sum2=0;
	for (int i=0; i<PointsUsedInFit.size(); i++){
		Phasor P = PointsUsedInFit[i];
		double dist = Phasor::SquareDistPhasor(P, P0);
		FuncValues[i] = exp(-dist/variance);
		
		sum1 += FuncValues[i]*FuncValues[i];
		sum2 += FuncValues[i]*NData[i];
	}
	double lambda = sum2/sum1;
	double SD=0;
	for (int i=0; i<PointsUsedInFit.size(); i++){
		double tp1=lambda*FuncValues[i] - NData[i];
		SD += tp1*tp1;
	}
	return SD;
}

Phasor PhasorImage::FitGaussian(Phasor PhasorCircle, double PhasorCircleR, vector <Phasor> & Coordinates,  vector <int > & N){
	if (!initialized){
		return Phasor(0.5, 0.25);
	}
	if (Coordinates.size()<2){
//		Alert0("size<2");
		return Phasor(0.5, 0.25);
	}
	if (N.size()<2){
//		Alert0("size<2");
		return Phasor(0.5, 0.25);
	}
	if (Coordinates.size()!=N.size()){
//		Alert0("unequal size");
		return Phasor(0.5, 0.25);
	}

	FitPhasorCircle = PhasorCircle;
	FitRadiusSquare = PhasorCircleR*PhasorCircleR;
	PointsUsedInFit.resize(0);
	FuncValues.resize(0);
	NData.resize(0);
	for (int k=0; k<N.size(); k++){
		Phasor P = Coordinates[k];
		if (P.IsInCircle(FitPhasorCircle, FitRadiusSquare)){
			PointsUsedInFit.push_back(P);
			NData.push_back(double(N[k]));
			FuncValues.push_back(0.0);
		}
	}
	double Params[3]; // x, y, var
	double BestParams[3];
	double BestSD=1e300;
	Params[0]=FitPhasorCircle.G;
	Params[1]=FitPhasorCircle.S;
	Params[2]=FitRadiusSquare/4.0;
	int MaxStep = 4000;
	double stepsize =0.1;
	Simplex simp;
	simp.InitSimplex(stepsize, Params, 3);
	for (int nstep=0; nstep<MaxStep; nstep++){
		double SD = CalcSD(Params);
		if (SD<BestSD){
			BestSD=SD;
			for (int i=0; i<3; i++){
				BestParams[i]=Params[i];
			}
		}
		simp.NextSimp(Params, SD);
	}

	return Phasor(BestParams[0], BestParams[1]);
}

bool PhasorImage::SaveOnFile(CFile & file){
	CFile * fp = &(file);
	WriByNS(FLASER);
//	ShowFloat(FLASER, "FLASER");
	WriByNS(sizex);
	WriByNS(sizey);
	WriByNS(initialized);
	unsigned __int32 size1 = imagedata.size();
	WriByNS(size1);
	for (int i=0; i<imagedata.size(); i++){
		unsigned __int32 size2 = imagedata[i].size();
		WriByNS(size2);
		for (int j=0; j<imagedata[i].size(); j++){
			WriByNS(imagedata[i][j].G);
			WriByNS(imagedata[i][j].S);
			WriByNS(imagedata[i][j].tau_in_ns);
			WriByNS(imagedata[i][j].m);
			WriByNS(imagedata[i][j].phi);
		}
	}
	return true;
}

bool PhasorImage::LoadFromFile(CFile & file){
	CFile * fp = &(file);

	ReByNS(FLASER);
//		ShowFloat(FLASER, "FLASER");
	ReByNS(sizex);
	ReByNS(sizey);
	ReByNS(initialized);

	unsigned __int32 size1;
	ReByNS(size1);
	imagedata.resize(size1);
//	ShowFloat(size1, "size1");

	for (int i=0; i<imagedata.size(); i++){
		unsigned __int32 size2;
		ReByNS(size2);
//		if (i==0) 	ShowFloat(size2, "size2");
		imagedata[i].resize(size2);
		for (int j=0; j<imagedata[i].size(); j++){
			ReByNS(imagedata[i][j].G);
			ReByNS(imagedata[i][j].S);
			ReByNS(imagedata[i][j].tau_in_ns);
			ReByNS(imagedata[i][j].m);
			ReByNS(imagedata[i][j].phi);
		}
	}
//	Alert0("hi3");
	return true;

}

double Phasor::CalcSDFitFraction(Phasor PFret, Phasor BG, Phasor PD, double PhasorFraction, double & BestE ){
	if (PhasorFraction<0.0) return 1e300;
	if (PhasorFraction>1.0) return 1e300;
	double MinDist = 1e300;
	for (int index=0; index<=100; index++){
		double E = double(index)/100.0;
		Phasor P = Phasor::CalcPasqua24TrajectoryPoint1(BG, PD, E, PhasorFraction);
		double SD = Phasor::SquareDistPhasor(P, PFret);
		if (SD<MinDist){
			MinDist = SD;
			BestE = E;
		}
	}
	return MinDist;
}

double Phasor::FitBestFraction(Phasor PFret, Phasor BG, Phasor PD, double & BestE){
	double Params[1], Frac;
	Params[0] = Frac = 0.5;
	double BestSD = 1e200;
	Simplex simp;
	double StepSize = 0.1;
	simp.InitSimplex(StepSize, Params, 1);
	for (int i=0; i<200; i++){
		double E;
		double SD = Phasor::CalcSDFitFraction(PFret, BG, PD, Params[0], E);
		if (SD<BestSD){
			BestSD = SD;
			Frac = Params[0];
			BestE = E;
		}
		simp.NextSimp(Params, SD);
	}
	return Frac;
}