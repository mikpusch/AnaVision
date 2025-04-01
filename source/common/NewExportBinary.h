
class NewExportBinary 
{

public:
	NewExportBinary();
	~NewExportBinary();

public:

	int Version;
	int NTraces;
	int NPoints;

	BOOL Compressed;
	double CompressionFactor;

	double SampleTime; // in secs
	double VHold; // in mV
	double StimInterval; // in secs
	int TimeOffset; // in sampletimes

	double conc;
	double spare[10];

	int NSegments;

#define MAXSEGMENTSNEWEXPORTBINARY 10

	double V[MAXSEGMENTSNEWEXPORTBINARY];
	double DV[MAXSEGMENTSNEWEXPORTBINARY];
	double T[MAXSEGMENTSNEWEXPORTBINARY];
	double DT[MAXSEGMENTSNEWEXPORTBINARY];

	BOOL   fitted;
	double ** data;
	double ** fit;

	double * xdata;


// Operations
public:
	void ShowHeader();
	double GetMax();
	BOOL WriteHeader(CFile & f);
	BOOL WriteData(CFile & f);
	BOOL LoadHeader(CFile & f);
	BOOL LoadData(CFile & f);
	BOOL LoadCompressed(CFile & f, double CompressionFactor);

	BOOL DoLoadData(CFile & f, BOOL Compress, double CompressionFactor);




};

/////////////////////////////////////////////////////////////////////////////
