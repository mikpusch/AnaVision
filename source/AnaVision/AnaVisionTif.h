// This file defines the AnaVisionTif class
// created 10.07.2011
// Copyright Michael Pusch, pusch@ge.ibf.cnr.it

// Purpose is to read Till Photonics new
// data files.
// Initially I intended to use libtiff, but now the reading is done by my own code.

#pragma once

typedef unsigned __int16 uint16 ;
typedef unsigned __int32 uint32 ;
typedef __int16  int16 ;
typedef __int32 int32 ;

#include "../common/tinytiffreader.h"
#include "../common/tinytiffwriter.h"

/*
The single member of this structure that is within the scope of this discussion is the third, the offset to the first IFD (Image File Directory). This IFD can be located anywhere in the file. Every 'page' in a multi-page TIFF is represented by exactly one IFD. Here's a more detailed view of this IFD...
Offset	Datatype	Value
0	Word	Number of tags in IFD
2+x*12	Tag structure	Tag data
2+(number of tags in IFD)*12	Unsigned Long	Offset to next IFD, if there is a next IFD
0 otherwise

The tags in this IFD should be sorted by code. Every tag takes up exactly 12 bytes, and looks like this...
Offset	Datatype	Value
0	Word	Tag identifying code
2	Word	Datatype of tag data
4	Unsigned Long	Number of values
8	x * Tag data datatype
or Unsigned Long offset
see below	Tag data
or offset to tag data
see below
*/

typedef struct MyRGBStructTag{
	unsigned __int8 r;
	unsigned __int8 g;
	unsigned __int8 b;
	unsigned __int8 unused;
}MyRGBStruct;

typedef struct MyRGBOnlyStructTag{
	unsigned __int8 r;
	unsigned __int8 g;
	unsigned __int8 b;
}MyRGBOnlyStruct;

typedef struct MyTiffHeaderStructTag{
	uint16 ByteOrder;
	uint16 VersionNumber;
	uint32 OffsetToFirstIFD;
}MyTiffHeaderStruct;

typedef struct MyTiffIFDStructTag{
	uint32 OffsetThisIFD;
	uint16 NumberOfTags;
	uint32 OffsetToNextIFD;
	int IndexTime, IndexChannel, IndexZ;
	uint32 StripDataOffset;
}MyTiffIFDStruct;

typedef struct MyTiffTagStructTag{
	uint16 TagCode;
	uint16 DataType;
	uint32 NValues;
	uint32 Data;
}MyTiffTagStruct;


class MySingleTiffileInfo{
public:
	MySingleTiffileInfo();
	~MySingleTiffileInfo();

	bool OpenStandardTif(CString FileName);
	bool OpenFile(CString FileName, int ImageOrderMethod, bool ReadFileInfo = false, bool IsStandardTif = false);

	CString Name;
	CFile Tiffile;
	bool FileIsOpen;

	bool IsStandardTif;
	TinyTIFFReaderFile* stdtiffr;


	CString FileInfo;

	MyTiffHeaderStruct Header;

	uint32 NIfds;
	MyTiffIFDStruct * ifds;

	uint32 * NTags;
	MyTiffTagStruct ** tags;

	int width, height;

	void SetPointersToNull();
	void Alloc();
	void DeAlloc();

	void AnalyzeIfds(int ImageOrderMethod);


};

// for each [TimeIndex][ChannelIndex] the following struct provides the relevant
// file index and ifd index, in order to read in the correct inmage
typedef struct MyFileChannelRefTag{
	uint32 FileIndex;
	uint32 IfdIndex;
}MyFileChannelRef;

class AnaVisionTif{
public:
	AnaVisionTif();
	~AnaVisionTif();

	bool OpenFileOld(CString FileName);
	bool OpenFile(CString FileName, bool IsStandardTiff = false);
	bool OpenStandardTif(CString FileName);

	static bool SaveStandard16BitTiff(CString FileName, int sizex, int sizey, vector <unsigned __int16> & numbers); 

//	static bool SaveRGBTiff(CString Name, int sizex, int sizey, vector <COLORREF> &Pixels); 
	static bool SaveRGBTiff(CString Name, int sizex, int sizey, vector <MyRGBOnlyStruct> &Pixels); 

	void CloseFirstFileTemporarily();

	void GetImageSize(int & width, int & height);

	int GetNumberOfChannels();
	int GetNumberOfImagesPerChannel();

	bool ReadImage(int timeindex, int channel, unsigned __int16 * data);
	
	bool IsStandardTif;

static	bool GetXMLValue(CString & instring, CString target1, CString target2,
		int & value,
		unsigned __int64 & pos);
	/*
	Examples:
	With instring= " <OME:TiffData IFD="16" FirstZ="0" FirstT="2" FirstC="4" />
	and target1="OME:TiffData", target2="FirstZ"
	value would be 0
	*/

	int ImageOrderMethod;
#define ORDER_BY_TIME 0
#define ORDER_BY_ZSTACK 1

private:


	bool Initialized;
	bool FileIsOpen;
	bool FirstFileClosedTemporarily;

	CString FirstTiffFileName;

	CFile Tiffile;

	int NTotalIFDs;
	int NFiles;
	int NImagesPerChannel;
	int NChannels;

	MyFileChannelRef ** FileChannelRefs;


	int width, height;

	MySingleTiffileInfo * files;
	bool OpenNextTiff(CString LastName, CString & NewName, bool ReadFileInfo = false);

	void SetPointersToNull();
	void Alloc();
	void DeAlloc();

	bool GetNewName(CString LastName, CString & NewName);
};
