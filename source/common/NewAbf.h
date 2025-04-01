#ifndef _MIKNEWABF

#define _MIKNEWABF

#include "..\NewAbf\AbfFiles.h"


#define C_FUNC __stdcall

typedef BOOL (C_FUNC *  FuncABF_Close)(int, int *);
//int32 __CFUNC DAQmxGetDevAOPhysicalChans(const char device[], char *data, uInt32 bufferSize);
//BOOL WINAPI ABF_Close(int nFile, int *pnError);
//typedef __int32 (C_FUNC *  FuncDAQmxGetDevAOPhysicalChans)(const char [], char *, UINT);
//int32 __CFUNC DAQmxGetDevAOPhysicalChans(const char device[], char *data, uInt32 bufferSize);

typedef BOOL (C_FUNC *  FuncABF_ReadOpen)( LPCSTR, int *, UINT, ABCFFileHeader *, UINT *, DWORD *, int * );
//BOOL WINAPI ABF_ReadOpen( LPCSTR szFileName, int *phFile, UINT fFlags, ABFFileHeader *pFH, 
 //                         UINT *puMaxSamples, DWORD *pdwMaxEpi, int *pnError );

typedef BOOL (C_FUNC *  FuncABF_ReadChannel)(int, const ABCFFileHeader *, int, DWORD, float *, UINT *, int *);

//BOOL WINAPI ABCF_ReadChannel(int nFile, const ABCFFileHeader *pFH, int nChannel, DWORD dwEpisode, 
 //                           float *pfBuffer, UINT *puNumSamples, int *pnError);

typedef BOOL (C_FUNC *  FuncABF_GetNumSamples)(int, const ABCFFileHeader *, DWORD, UINT *, int *);

//BOOL WINAPI ABCF_GetNumSamples(int nFile, const ABCFFileHeader *pFH, DWORD dwEpisode, 
//                              UINT *puNumSamples, int *pnError);

typedef BOOL (C_FUNC *  FuncABF_GetEpisodeDuration)(int, const ABCFFileHeader *, DWORD, double *, int *);

//BOOL WINAPI ABCF_GetEpisodeDuration(int nFile, const ABCFFileHeader *pFH, DWORD dwEpisode, 
//                                 double *pdDuration, int *pnError);



class NewAbf{
public:
	// Constructor - either read or write
static bool InitNewAbf(bool ShowWarning = false); // load DLL
static void ShutDown();	  // Unload DLL

	NewAbf(bool OpenForRead); // this creates the object
	~NewAbf(); // destructor

	static bool IsInitialized(bool ShowWarning = false);

	bool OpenFile(CString & FileName);
	bool Close();
	bool ReadChannel(int sweep, // 0-based
					int channel, // logical; from 0 to NChannelsvalable-1
					float * data);

	bool GetComment(char * s, int MaxChar);
	int GetMaxSamples();



	int GetNumberOfChannels();
	int GetNumberOfSweeps();
	UINT GetNumberOfPointsOfSweep(DWORD sweep);
	double GetSampleTime();
	double GetVHold();
	double GetGain(int channel, double & factorgain);
	int GetNoOfStimSegs();
	int GetNoOfStimSegs(int & channel);
	void GetSegment(int i, double & A, double & T, double & SV, double & ST);
	short GetOperationMode();
	int GetActualAcqLength();

	bool SetComment(char * s, int MaxChar);
	bool SetNumberOfChannels(UINT nchan);
	bool SetNumberOfSweeps(UINT nsweep);
	bool SetNumberOfPointsOfSweeps(UINT * puNPoints);
	bool SetSampleTime(double stInSeconds);
	bool SetVHold(double vhInVolts);


private :
	void GetADCtoUUFactors( int nChannel, float *pfADCToUUFactor, float *pfADCToUUShift );

	static bool Initialized;
	static 	HINSTANCE hDLL;
	bool FileOpen;
	bool OPENFORREAD;

	static bool CheckProcedureAddress(DWORD adr, CString name);

	bool CheckRead();
	bool CheckWrite();
	void ShowInfo();

	int hFile;
	UINT fFlags;
	ABCFFileHeader FH;
	UINT uMaxSamples;
	DWORD dwMaxEpi;
	int nError;

	static FuncABF_Close				MyABF_Close;
	static FuncABF_ReadOpen				MyABF_ReadOpen;
	static FuncABF_ReadChannel			MyABF_ReadChannel;
	static FuncABF_GetNumSamples		MyABF_GetNumSamples;
	static FuncABF_GetEpisodeDuration	MyABF_GetEpisodeDuration;


};


#endif
