#pragma once
/*

Created 08/01/21

AnaVisionImageFile.h defines a C++ class of hyperstack images
Images are fully in RAM,as in Ana.

All images are in monocolor AnaVisionImageFileDataType(now float) format, i.e. each pixel has values form 0 to about 32000.
Negative values are allowed but for now not considered.

ALL images in the hyperstack have the same width and height

That's practically all to say!


*/
#define AnaVisionImageFileDataType float

#define VERSION_ANAVISION_IMAGE_FILE_FORMAT 1

class AnaVisionImageFile{ // this is in memory
	public:
	
	AnaVisionImageFile();
 	~AnaVisionImageFile();  // deallocates if allocated
	//bool IsAllocated;
	
	int Width, Height;
	
	//vector <__int32> data; 
	vector <AnaVisionImageFileDataType> data;
	
	bool ReadImage(int & bytesavailable, CFile & file, int Version);
	bool WriteImage(int & byteswritten, CFile & file);
	
	inline AnaVisionImageFileDataType GetPixel(int x, int y){
		return data[y*this->Width + x];
	}

	// for PixelDepth = 0 this returns the pixel value ecc.
	AnaVisionImageFileDataType GetMeanEnvironment(int x, int y, unsigned int PixelDepth);

	void SetPixel(AnaVisionImageFileDataType value, int x, int y);

	int GetNBytesImage();

};

class AnaVisionImageFileStack{ //  in memory
	public:
	AnaVisionImageFileStack();
	~AnaVisionImageFileStack();
	
	int Width, Height;
	
	CString Title;
	
	vector <AnaVisionImageFile> Images;
			
	bool AppendImage(AnaVisionImageFile & image); // adds to this stack

	bool ReadStack(int & bytesavailable, CFile & file, int Version);
	bool WriteStack(int & byteswritten, CFile & file);
};

class AnaVisionImageFileHyperStack{ //  in memory
	public:
	AnaVisionImageFileHyperStack();
	~AnaVisionImageFileHyperStack();
	
	int Version;
	int Width, Height;

	void SetVersion(); // after reading file sets to current version

	vector <AnaVisionImageFileStack> Stacks;
		
	bool AppendStack(AnaVisionImageFileStack & stack); // adds to this hyperstack

	bool ReadHyperStackFromFile(int & bytesavailable, CFile & file);
	bool WriteHyperStackToFile(int & byteswritten, CFile & file);

	static bool IsAnavisionHyperStack(int & bytesavailable, CFile & file); // test to read the first characters 

};

