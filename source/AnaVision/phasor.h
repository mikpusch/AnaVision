#pragma once
#include "../common/DrawObject.h"


class Phasor{
public:
	Phasor();
	Phasor(double g, double s);
	~Phasor();

	double G, S;
	double tau_in_ns, m, phi;
	void CalcTauMPhi(double fLaser);
	double Length();

	bool IsInCircle(Phasor Midpoint, double RSquare);

	static double Convert(int ImageValue);
	static void OldCalcTrajectoryPoint(Phasor BG, Phasor PD, double E, double frac, Phasor & Result);
	static Phasor CalcTrajectoryPoint(Phasor BG, Phasor PD, double E, double frac);
	static Phasor CalcPasqua24TrajectoryPoint1(Phasor BG, Phasor PD, double E, double frac);
	static Phasor CalcPasqua24TrajectoryPointBis1(Phasor BG, Phasor PD, double E, double frac);
	static Phasor CalcTryLeicaTrajectoryPoint(Phasor BG, Phasor PD, double E, double frac);
	static double FitBestFraction(Phasor PFret, Phasor BG, Phasor PD, double & BestE); 
	static double CalcSDFitFraction(Phasor PFret, Phasor BG, Phasor PD, double PhasorFraction, double & BestE);
	static Phasor GetIntersection(Phasor BG, Phasor PD);
	static double SquareDistPhasor(Phasor A, Phasor B); 
	static double DistPhasor(Phasor A, Phasor B); 
	static Phasor Add(Phasor A, Phasor B);
	static Phasor Subtract(Phasor A, Phasor B);
	static Phasor ScalarMult(double lambda, Phasor A);

};

class NumberList{ // read ascii file
public:
	NumberList();
	~NumberList();
	vector<int> numbers;
	int ReadFromFile(std::ifstream & ifile);
};

class PhasorImage{
public:
	PhasorImage(double fLaser = 80e6);
	~PhasorImage();
	bool SaveOnFile(CFile & file);
	bool LoadFromFile(CFile & file);
	void SetLaserFreq(double fLaser);
	void Transpose();
	double FLASER;
	int sizex;
	int sizey;
	bool initialized;
	vector < vector <Phasor> > imagedata;
	bool ReadFromTextFiles(int sx, int sy,
		std::ifstream & Gfile, std::ifstream & SFile);
	bool ReadFromTiffFiles(int &sx, int &sy,
		CString FileNameG, CString FileNameS);
	bool GetImagesFromBuffer(int sx, int sy, vector <double> & G, vector <double> & S);


	int CalcNumberOfPhasorsPerPixel(double GMin, double GMax, double SMin, double SMax,
			int sizeXRect, int sizeYRect, bool UsePolygon, PolygonObject * poly, vector < vector <int > > & N);
//	int CalcNumberOfPhasorsPerPixel(double GMin, double GMax, double SMin, double SMax,
//			int sizeXRect, int sizeYRect, bool UsePolygon, PolygonObject & poly, vector < vector <int > > & N);

	void WriteMask(Phasor PhasorCircle, 
		           double PhasorCircleR, 	vector < vector < bool>> &WhiteMask);

	Phasor FitGaussian(Phasor PhasorCircle, double PhasorCircleR, vector <Phasor> & Coordinates,  vector <int > & N);

private:
	double CalcSD(double * Params);
	vector <Phasor> PointsUsedInFit;
	vector <double> FuncValues;
	vector <double> NData;
	Phasor FitPhasorCircle;
	double FitRadiusSquare;


};