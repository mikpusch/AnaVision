
#define MAXSEGSINWRTEABF 10
#define MAXCHANNELSINWRTEABF 16

class WriteAbf{
public:
	WriteAbf();

	// How to use this converter:

	// 1. Fill all the relevant variables (i.e. the member variables);
	// 2. Call WriteHeader()
	// 3. for (i=0; i<NSweeps; i++){
	//		WriteSweep(i);
	//    }

	static void Initialize(int ResouceID);
	static void Shutdown();

	void WriteHeader(CFile & f);

	// the __int16 ** data in "WriteSweep" is for the case of more than 1 channel!!
	// i.e. "data[0][0]" is the 1st data point of the first channel
	// i.e. "data[1][99]" is the 100st data point of the second channel
	void WriteSweep(CFile & f, int sweep, __int16 ** data, int NPointsInThisSweep);


	int FileType;		// 0 = pulsed
						// 1 = continuous

	double VHold;		// in V
	double SampleTime;	//in sec
	double Gain[MAXCHANNELSINWRTEABF]; 

	int NChannels;
	int NSweeps;		// must be = 1 for continuous file type
	int NPointsPerSweep;

	int NSegs;

	int ADCResolution; // e.g. for a 16 bit ADC converter this is 2^15=32768


	double Volt[MAXSEGSINWRTEABF]; // in V
	double DeltaVolt[MAXSEGSINWRTEABF]; // in V
	double Time[MAXSEGSINWRTEABF]; // in secs
	double DeltaTime[MAXSEGSINWRTEABF]; // in secs

private:
	static char * abfh; // contains header; present once!
	static BOOL Initialized;
	static HGLOBAL HeaderResourcePointer;

	static BOOL DoLoadResource(int ResourceID);

};
