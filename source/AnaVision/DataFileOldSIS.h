#ifndef DAtaFileAnaVision
#define DAtaFileAnaVision

//#include "Params.h"

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
	BufferClass<double> values;
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

class arivis::simpleimagecore::SimpleImageStack;

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

	CString FileName;
	CString Title;

	int CurrentIndex;
	bool initialized;
	bool imageready;
//	bool ReadImage(int index, Params & p, double ** data);
	bool ReadImage(int index, Params & p, ImageData & id, bool RefImage,
		unsigned __int64 fileoffset = 0, UINT NChannels=1, UINT ChannelNumber = 0);

	bool CheckAvailabilityDataFile(CFile * fp = NULL);

	//The following 3 are copied from the root or only used in tree. Not stored on file!
	bool FileNamesAreRelative;
	CString RootName;
	int treeid;
	
	arivis::simpleimagecore::SimpleImageStack* SISStack;

	Params * pp;
	CString GetFileName();
	CString GetNewFileName(CString name);

	bool AssignValue(__int16 value);


private:
	bool DoReadTxtFile(FILE * f, ImageDataType ** currentimage);
	bool ReadPstFile(int index, CFile * fp, ImageDataType ** currentimage);

	bool OldReadSisFile(int index, CFile * fp, ImageDataType ** currentimage, unsigned __int64 fileoffset, UINT NChannels, UINT ChannelNumber);
	bool ReadSisFile(	int index, ImageDataType ** currentimage, UINT ChannelNumber);
	CString MakeFileName(CString Name, int index, CString ext);
	bool CheckAvailabilityTextFiles();
	bool CheckAvailabilityPst(CString rootpath, bool FileNamesAreRelative);
	bool FindTheFile(CString & Name);
	bool AssignFileNameAndIndexesTxtFiles(CString Name);
	bool CheckAvailabilitySis(CFile * fp);

};




class DataFile{
public:

// ********************* DATA HANDLING
	DataFile();
	~DataFile();



	void SaveOnFile(CFile & f);
	bool LoadFromFile(CFile & f);

	int FileType; // 0=Converted vws file

					// 1: converetd SIS file
	arivis::simpleimagecore::SimpleImageStack* SISStack;
//********************** DATA

	Params p;
	Kinetics k;
	ArrayOfPolygons polys;
	AllROIValues roivalues;

	CString FirstFileName;
	CString Extension;
	CString rootpath;

	UINT ImageIndex;
	UINT NImages;
	ImageSequence * images;

	unsigned __int64 FileOffset;


	// Not saved on file:
	bool Converted; 
	bool imageready;

	
// ************************** procedures

	bool AssignFileNamesAndIndexes(CString Name, CFile & file);
	bool ReadImage(int index, ImageData &id, bool RefImage);
	bool IsAnavisionFile(CFile & file); // Tests only if the first thing in the file is the string
									// ANAVISION


	bool CopyImage(int index); // puts the image at the end
	bool DoAverage(int indexa, int indexb, int indexc, CString newtitle);


	void AssignValue(__int16 value);

//private:
	void SetVersion();
	void OldSaveOnFile(CFile & f);
	bool OldLoadFromFile(CFile & f);
	int Version1, Version2, Version3;

	bool FileNamesAreRelative;
	bool ReadVwsFile(CFile & file);
	bool OldReadSisFile(CFile & file);
//	bool ReadSisFile(CFile & file);
	bool OpenSisFile(CString & FileName);
	
	bool FindNextStringForward(CFile & file, CString c, uint64 & pos);
	bool FindNextStringBackward(CFile & file, CString c, uint64 & pos);
	int ScanVwsFile(CFile & file, bool storevalues);


	bool GetWidthAndHeightFromInfFile(CString PSTFileName, int & width, int & height);
	void DoCalcRatio(__int16 * da, __int16 * db, int Sizex, int Sizey, Params & p, __int16 *dc);





};

#endif
