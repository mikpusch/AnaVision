#ifndef DAtaFileAnaVision
#define DAtaFileAnaVision

//#include "Params.h"

#include "AnaVisionTif.h"
#include "AnaVisionImageFile.h"

class BorderPoint{
public:
	int x;
	int y;
	int Corner; //1: upper right, 2: upper left; 3: lower left; 4: lower right
};

class BorderPoints{
public:
	vector<BorderPoint> points;
	bool CornerIsInThis(BorderPoint & tp);
};

class CellPointClass{
public:
	int cellindex;
	CPoint point;
};

class BlobPoints{
public:
//	BlobPoints &BlobPoints::operator =(BlobPoints  );
	int NCells;
	vector < CellPointClass > points; // point[x][y]
//	PolygonObject BorderBlob;
//	vector < PolygonObject > BorderCells; // the first is of the blob for each cell
	BorderPoints BlobBorder; // of the blob
	vector < BorderPoints > BorderCells; // 

	void CalculateBlobBorder(int SX, int SY);
	bool PointIsInBlob(int x, int y);
	bool PointIsInCell(int index, int x, int y);
	CPoint FindBorderPointOfBlob(int SX, int SY);
	int GetNNeighbors(int i, int SX, int SY);
	void FillNeighbours(int x, int y, bool (*array)[3][3]);

};
class CellsClass{
public:
	vector<BlobPoints> celllist;
};


class ComponentLabelsClass{
public:
	vector <vector <int> > Labels;
};

class NeighborPixelClass{
public:
	int x;
	int y;
	int value;
	int Label;
};
class NeighborsPixelsClass{
public:
	int NNeighbors;
	NeighborPixelClass Neighbors[10];
};




class OverlayClass{
public:
	OverlayClass(); 
	~OverlayClass();

	int NChannelsToOverlay;  //
	BOOL OverlayThis[1000]; // Hope there are not more than 1000
	CPoint OffsetOfThis[1000];
};


class MeanROIArray{
public:
	MeanROIArray();
	~MeanROIArray();
	vector <double> values;
	bool SaveOnFile(CFile * fp, int & byteswritten); // return #bytes written
	bool LoadFromFile(CFile * fp, int & bytesavailable ); // return #bytes written
	void CalcMinMax(double & Min, double & Max);
	int GetMaxImages();

};

class SeveralMeanROI{
public:
	SeveralMeanROI();
	~SeveralMeanROI();
	BufferClass<MeanROIArray> ListOfArrays;
	bool SaveOnFile(CFile * fp, int & byteswritten); // return #bytes written
	bool LoadFromFile(CFile * fp, int & bytesavailable ); // return #bytes written
	void CalcMinMax(double & Min, double & Max);
	int GetMaxImages();
};

class AllROIValues{
public:
	AllROIValues();
	~AllROIValues();
	BufferClass<SeveralMeanROI> SeriesOfROIValues; 
	bool SaveOnFile(CFile * fp, int & byteswritten); // return #bytes written
	bool LoadFromFile(CFile * fp, int & bytesavailable ); // return #bytes written
	void CalcMinMax(double & Min, double & Max);
	int GetMaxImages();
};

#if defined SIMPLE_IMAGE_CORE
	class arivis::simpleimagecore::SimpleImageStack;
#endif

class ImageSequence{
public:
	ImageSequence();
	~ImageSequence();

	int Sizex, Sizey;

	int FirstIndex;
	int LastIndex;
	int FileMode;	// 0 = several txt files
					// 1 = one pst file
					// 2 = SIS file
					// 3 = TIFF from Till
					// 5 = AnaVisionFile

	CString FileName;
	CString Title;

	int CurrentIndex;
	bool initialized;
	bool imageready;
//	bool ReadImage(int index, Params & p, double ** data);
	bool ReadImage(int index, Params & p, ImageData & id, bool RefImage,
		unsigned __int64 fileoffset = 0, UINT NChannels=1, UINT ChannelNumber = 0, bool DoShow = false);

	bool CheckAvailabilityDataFile(CFile * fp = NULL);

	//The following 3 are copied from the root or only used in tree. Not stored on file!
	bool FileNamesAreRelative;
	CString RootName;
	int treeid;

#if defined SIMPLE_IMAGE_CORE	
	arivis::simpleimagecore::SimpleImageStack* SISStack;
#endif

	unsigned __int16 * TempImageData;

	AnaVisionTif * ptrTiff;

	Params * pp;
	CString GetFileName();
	CString GetNewFileName(CString name);

	bool AssignValue(__int16 value);
	AnaVisionImageFileStack * AS;
	vector < CellsClass > celllistarray;


private:
	bool DoReadTxtFile(FILE * f, ImageDataType ** currentimage);
	bool ReadPstFile(int index, CFile * fp, ImageDataType ** currentimage);

//	bool OldReadSisFile(int index, CFile * fp, ImageDataType ** currentimage, unsigned __int64 fileoffset, UINT NChannels, UINT ChannelNumber);
	bool ReadSisFile(	int index, ImageDataType ** currentimage, UINT ChannelNumber);
	bool ReadTifFile(	int index, ImageDataType ** currentimage, UINT ChannelNumber);
	bool ReadAnaVisionImage(	int index, ImageDataType ** currentimage, UINT ChannelNumber);

	CString MakeFileName(CString Name, int index, CString ext);
	bool CheckAvailabilityTextFiles();
	bool CheckAvailabilityPst(CString rootpath, bool FileNamesAreRelative);
	bool FindTheFile(CString & Name);
	bool AssignFileNameAndIndexesTxtFiles(CString Name);
	bool CheckAvailabilitySis(CFile * fp);
	bool CheckAvailabilityTif(CFile * fp);

};




class DataFile{
public:

// ********************* DATA HANDLING
	DataFile();
	~DataFile();

	void AllocOffsetArrays();
	void DeAllocOffsetArrays();
	void SaveOffsetArrays();
	void RetrievOffsetArrays();
	void GetOffset(int & offsetx, int & offsety);
	void IncOffset(int offsetx, int offsety);
	void IncOffset(int startindex, int offsetx, int offsety);
	void ClearAllOffsets();

	AnaVisionTif AnaTif;

	AnaVisionImageFileHyperStack HS;

	void SaveOnFile(CFile & f);
	bool LoadFromFile(CFile & f);

	unsigned __int64 BytesWrittenToFile;
	unsigned __int64 BytesReadFromFile;


	int FileType; // 0=Converted vws file

					// 1: converetd SIS file
					// 2: converted TIFF
					// 5: AnavisionImageFile

#if defined SIMPLE_IMAGE_CORE
	arivis::simpleimagecore::SimpleImageStack* SISStack;
#endif

//********************** DATA


	Params p;
	Kinetics k;
	ArrayOfPolygons polys;
	ArrayOfPolygons lines;
	AllROIValues roivalues;

	CString FirstFileName;
	CString Extension;
	CString rootpath;

	UINT ImageIndex;
	UINT NImages;
	ImageSequence * images;

	unsigned __int64 FileOffset;

	//Added in Version 8: relative offset of x and y. Applied to all .

	int MaxNumberOfImages;
	int * OffsetX;
	int * OffsetY;
	int * SaveOffsetX;
	int * SaveOffsetY;


	// Not saved on file:
	bool Converted; 
	bool imageready;

	
// ************************** procedures

	void CalcMaxNumberOfImages();
	void AllocCellLists();

	bool AssignFileNamesAndIndexes(CString Name, CFile * fp, bool IsStandardTiff = false);
	bool ReadImage(int index, ImageData &id, CellsClass * & celllist, bool RefImage, bool DoShow = false);
	bool IsAnavisionFile(CFile *fp); // Tests only if the first thing in the file is the string
									// ANAVISION


	bool CopyImage(int index); // puts the image at the end
	bool DoAverage(int indexa, int indexb, int indexc, CString newtitle);


	void AssignValue(__int16 value);

//private:
	void SetVersion();
//	void OldSaveOnFile(CFile & f);
	bool OldLoadFromFile(CFile & f);
	int Version1, Version2, Version3;

	bool FileNamesAreRelative;
	bool ReadVwsFile(CFile & file);
//	bool OldReadSisFile(CFile & file);
//	bool ReadSisFile(CFile & file);
	bool OpenSisFile(CString & FileName);
	bool DoOpenTifFile(CString FileName);
	bool OpenTifFile(CString & FileName);

	bool DoOpenStandardTifFile(CString FileName);
	bool OpenStandardTifFile(CString & FileName);
	bool ReadAnaVisionFileFormat(int & bytesavailable, CFile & file);

//	bool FindNextStringForward(CFile & file, CString c, uint64 & pos);
//	bool FindNextStringBackward(CFile & file, CString c, uint64 & pos);
	int ScanVwsFile(CFile & file, bool storevalues);


	bool GetWidthAndHeightFromInfFile(CString PSTFileName, int & width, int & height);
	void DoCalcRatio(__int16 * da, __int16 * db, int Sizex, int Sizey, Params & p, __int16 *dc);

	void OnFileSaveimagesinanavisionformat(int & byteswritten, ImageData & id);
	void OnFileSavethisstackinanavisionformat(int & byteswritten, ImageData & id, int stack, int  from, int to);

};

#endif
