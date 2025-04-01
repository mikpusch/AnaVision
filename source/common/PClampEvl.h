// Created 05.04.08 /// Michael Pusch
// A class to represent Pclamp EVL Files - read - write - generate based on idealized trace
// based on the Axon ASC file EVL.ASC, downloaded from MD

#ifndef PCLAMPEVLCLASS
#define PCLAMPEVLCLASS

class PClampEvlHeader{
public:

	PClampEvlHeader();
	~PClampEvlHeader();

	bool ReadFromFile(CFile & f, bool show = false);
	bool WriteToFile(CFile & f);

	bool IsClampex;

	char FileID[8];
	__int16 FFVersion;
	char DataName[12];
	char ADCUnits[8];
	char Comment[128];
	float TimePerPointInMicroSecs;
	__int16 InterpolationFactor;
	float AnalysisFilter;
	__int16 EpisodeSize;
	__int16 TriggerStart;
	float Holding;
	float AcquistionFilter;
	float Gaps;
	__int16 TriggerEnd;
	__int16 NumLevels;
	char Reserved[68];
};

class PClampEvlEvent{ // 24 or 26 bytes
public:

	PClampEvlEvent();
	~PClampEvlEvent();

	bool ReadFromFile(CFile & f, bool Clampex, int e, bool ShowValues = false);
	bool WriteToFile(CFile & f);

	bool IsClampex; // this defines if the initial "EventEpi" is valid or not


	__int16 EventEpi; // optional
	__int32 LevelStart;
	float	Amplitude;
	__int32 LevelLength;
	float	EventsStandardDev;
	__int16	EventCurrentLevel;
	__int16 EventNotes;
	float	BaseLine;
};

class PClampEvl{
public:
	PClampEvl();
	~PClampEvl();

	bool IsClampex;

	bool ReadFromFile(CFile & f);
	bool WriteToFile(CFile & f);

	bool MakeEventFile(__int16 * data, int NData, double gain, double SampleTime);

	PClampEvlHeader header;

	int NEvents;
	int NAllocs;

	PClampEvlEvent * events;

private:
	void DeAlloc();
	void Alloc();


};



#endif
