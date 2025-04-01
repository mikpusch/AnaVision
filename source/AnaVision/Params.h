#include "../common/DrawObject.h"
//#include "DataFile.h"


// Class Params
#define SettingsSection "AnaVisionSettingsSection"
#define SettingsItem	"AnaVisionSettingsItem"

#define NCURSORS	3

class Params{
public:
	Params();
	~Params();

	int SaveParamsFile();
	int LoadParamsFile(bool ask);


	bool ReadFromFile(CFile * fp);
	bool ReadFromFile(CFile * fp, int bytesavailable);

	int SaveOnFile(CFile * fp); // returns the number of bytes written


	AreaObject RectOb, ConusOb;
	int ViewType;	// 0 = Rect
					// 1 = Conus
					// 2 = polygons

	AreaObject * drawob();

	double LaserFlimFreq;
	int SizeOfMembraneInPixels;
	BOOL ROISAreClosedInMembraneAnalysis;
	int NumberOfGradientPoints;

	int xsizeimage; // default 640
	int ysizeimage; // default 480
	double dt;		// Timed difference between images (in seconds)

	double curspos[NCURSORS];
	void CheckCursors();

	BOOL TrackDrift;

	double PixelSizeInMicrom;
	double Magnification;

    int protonconversionmode;// 0 = theoretcal; 1 : linear; 2: convert ratio into deltaH
    double theoretical_pK;
	double totalbuffermM;
	double linearconversionfactor; 
	double FactorRatioIntoDeltaH;
	
	double ConvertpHToProton (double oldpH, double newpH);

	BOOL StepByOne; // otherwise use delta image

	BOOL AutoScale;
	BOOL AutoScaleSecondImage;

	BOOL SubtractInitialValuesForDraw;

	double MultiplierImages; 
	double OffsetImages;

	double MinManScale;
	double MaxManScale;

	double MinManScaleSecondImage;
	double MaxManScaleSecondImage;

	int DeltaImage;

	double BackgroundA; // for ratio: A/B
	double BackgroundB;
	int NSurroundForAverage;

	bool ConvertTopH;

	double A1_490_450, A2_490_450, x0_490_450, dx_490_450; 
	double A1_490_440, A2_490_440, x0_490_440, dx_490_440; 

	int WavelengthCombination;	// 0 : 490/450
								// 1 : 490/440

	int InitialFrame0, InitialFrame1, FrameToAnalyze0, FrameToAnalyze1;

	double ScaleRatio;

	double FRETBTDonor;
	double FRETBTAcceptor;
	double ThresholdFret;
	BOOL CalcNormalizedFret; // else just fret



//	double Convert(double value);
	void Convert(ImageDataType ** image, int xsize, int ysize);

	int NBitsColorTable; // 8 or 16
	COLORREF ColorTable256[256];
	COLORREF ColorTable65536[65536];

	int NBitsColorTableSecondImage; // 8 or 16
	COLORREF ColorTable256SecondImage[256];
	COLORREF ColorTable65536SecondImage[65536];

	CString LookupTableName;
	CString LookupTableNameSecondImage;
//	CString LastGoodLookupTableName;

	bool DoLoadLookupTable(CFile & file, int WhichImage = 0);
	bool TryLoadLookupTable( int WhichImage = 0);

	void MakeGreyScale(int WhichImage = 0);
	void MakeGreenScale(int WhichImage = 0);
	void MakeRedScale(int WhichImage = 0);
	void MakeBlueScale(int WhichImage = 0);

	// Not changes on file, just to read the data
	int offsetx, offsety;


private:

	bool LoadASCIITable(CFile & file,  int WhichImage = 0);
	double TheoreticalConvertpHToProton (double oldpH, double newpH);
	double LinearConvertpHToProton (double oldpH, double newpH);
	double RatioToProtonChange (double oldratio, double newratio);
};
