// implements the pclamp evl class
#include "stdafx.h"
#include "../common/PClampEvl.h"
#include "../common/utils.h"
#include <math.h>

PClampEvlHeader::PClampEvlHeader(){
	int i;
	FileID[0]='E';
	FileID[1]='V';
	FileID[2]='N';
	FileID[3]='T';
	FileID[4]='L';
	FileID[5]='I';
	FileID[6]='S';
	FileID[7]='T';
//	Alert0(FileID);
	IsClampex = false;

	FFVersion = 1;
	DataName[0]='d';
	DataName[1]='a';
	DataName[2]='t';
	DataName[3]='a';
	DataName[4]='1';
	DataName[5]='2';
	DataName[6]='3';
	DataName[7]='4';
	DataName[8]='.';
	DataName[9]='d';
	DataName[10]='a';
	DataName[11]='t';
	for (i=0; i<8; i++){
		ADCUnits[i]=0;
	}
	ADCUnits[0]='p';
	ADCUnits[1]='A';
	for (i=0; i<128; i++){
		Comment[i]=0;
	}
	for (i=0; i<68; i++){
		Reserved[i]=0;
	}

	TimePerPointInMicroSecs = 100;
	InterpolationFactor = 1;
	AnalysisFilter = 1000;
	EpisodeSize = 1;
	TriggerStart = 0;
	Holding = 0;
	AcquistionFilter = 1000;
	Gaps = 0.0;
	TriggerEnd = 0;
	NumLevels = 1;
}

PClampEvlHeader::~PClampEvlHeader(){
}



bool PClampEvlHeader::ReadFromFile(CFile & f, bool show){
	show = true;
	CFile * fp = &(f);
	UINT toread = 8;
	char s[9];
	UINT read = f.Read(s, toread);
	if (read<toread){
		return false;
	}
	s[8]=0;
	CString c1=CString(s);
	if (c1.Compare(CString("EVNTLIST")) != 0){
		c1 = CString("ERROR IN EVL HEADER: ")+c1;
		Alert0(c1);
		return false;
	}
	if (show)	Alert0(c1);
	{	for (int i=0; i<8; i++){
			FileID[i]=s[i];
		}
	}

	ReByNS(FFVersion);
	toread = 12;
	read = f.Read(DataName, toread);
	if (read<toread){
		return false;
	}
	if (show)	Alert0(DataName);

	toread = 8;
	read = f.Read(ADCUnits, toread);
	if (read<toread){
		return false;
	}
	if (show)	Alert0(ADCUnits);

	toread = 128;
	read = f.Read(Comment, toread);
	if (read<toread){
		return false;
	}
	if (show)		Alert0(Comment);

	ReByNS(TimePerPointInMicroSecs);
	if (show)		ShowFloat(TimePerPointInMicroSecs, "TimePerPointInMicroSecs");
	ReByNS(InterpolationFactor);
	if (show)		ShowFloat(InterpolationFactor, "InterpolationFactor");
	ReByNS(AnalysisFilter);
	if (show)		ShowFloat(AnalysisFilter, "AnalysisFilter");
	ReByNS(EpisodeSize);
	if (show)		ShowFloat(EpisodeSize, "EpisodeSize");
	if (EpisodeSize!= 0){
		IsClampex = true;
	}
	else{
		IsClampex = false;
	}
	ReByNS(TriggerStart);
	if (show)		ShowFloat(TriggerStart, "TriggerStart");
	ReByNS(Holding);
	if (show)		ShowFloat(Holding, "Holding");
	ReByNS(AcquistionFilter);
	if (show)		ShowFloat(AcquistionFilter, "AcquistionFilter");
	ReByNS(Gaps);
	if (show)		ShowFloat(Gaps, "Gaps");

	ReByNS(TriggerEnd);
	if (show)		ShowFloat(TriggerEnd, "TriggerEnd");
	ReByNS(NumLevels);
	if (show)		ShowFloat(NumLevels, "NumLevels");

	toread = 68;
	read = f.Read(Reserved, toread);
	if (read<toread){
		return false;
	}
	return true;
}

bool PClampEvlHeader::WriteToFile(CFile & f){
	CFile * fp = &(f);
	UINT towrite = 8;
	f.Write(FileID, towrite);
	WriByNS(FFVersion);
	towrite = 12;
	f.Write(DataName, towrite);
	towrite = 8;
	f.Write(ADCUnits, towrite);

	towrite = 128;
	f.Write(Comment, towrite);

	WriByNS(TimePerPointInMicroSecs);
	WriByNS(InterpolationFactor);
	WriByNS(AnalysisFilter);
	WriByNS(EpisodeSize);
	WriByNS(TriggerStart);
	WriByNS(Holding);
	WriByNS(AcquistionFilter);
	WriByNS(Gaps);

	WriByNS(TriggerEnd);
	WriByNS(NumLevels);

	towrite = 68;
	f.Write(Reserved, towrite);
	return true;
}




PClampEvlEvent::PClampEvlEvent(){
	IsClampex = false;
}
PClampEvlEvent::~PClampEvlEvent(){
}

bool PClampEvlEvent::ReadFromFile(CFile & f, bool Clampex, int e, bool ShowValues){
	CFile * fp = &(f);

	IsClampex = Clampex;
	if (IsClampex){
		ReByNS(EventEpi);
		if (ShowValues) ShowFloat(EventEpi, "EventEpi");
	}

	ReByNS(LevelStart);
	ReByNS(Amplitude);
	ReByNS(LevelLength);
	ReByNS(EventsStandardDev);
	ReByNS(EventCurrentLevel);
	ReByNS(EventNotes);
	if (EventCurrentLevel==0){
		ReByNS(BaseLine);
	}
	if (ShowValues){ 
		ShowFloat(LevelStart, "LevelStart");
		ShowFloat(Amplitude, "Amplitude");
		ShowFloat(LevelLength, "LevelLength");
		ShowFloat(EventsStandardDev, "EventsStandardDev");
		ShowFloat(EventCurrentLevel, "EventCurrentLevel");
		ShowFloat(EventNotes, "EventNotes");
		ShowFloat(BaseLine, "BaseLine");
	}

	return true;
}
bool PClampEvlEvent::WriteToFile(CFile & f){
	CFile * fp = &(f);
	if (IsClampex){
		WriByNS(EventEpi);
	}
	WriByNS(LevelStart);
	WriByNS(Amplitude);
	WriByNS(LevelLength);
	WriByNS(EventsStandardDev);
	WriByNS(EventCurrentLevel);
	WriByNS(EventNotes);
	if (EventCurrentLevel==0){
		WriByNS(BaseLine);
	}
	return true;
}




PClampEvl::	PClampEvl(){
	NEvents = 0;
	NAllocs = 0;
	events = NULL;;
	IsClampex = false;
}
PClampEvl::~PClampEvl(){
	DeAlloc();
}

void PClampEvl::DeAlloc(){
	if (events){
		delete [] events;
	}
	events = NULL;
}
void PClampEvl::Alloc(){
	if (NEvents<NAllocs){
		return;
	}
	DeAlloc();
	NAllocs = NEvents;

	events = new PClampEvlEvent[NAllocs];
}

bool PClampEvl::ReadFromFile(CFile & f){
	UINT s = f.GetLength();
	CFile * fp = &(f);

	if (s<256){
		return false;
	}
	PClampEvlHeader TestHeader;

	if (!TestHeader.ReadFromFile(f)){
		return false;
	}

	header = TestHeader;
	IsClampex = header.IsClampex;
	if (IsClampex){
//		Alert0("CLAMPEX");
	}
	else{
//		Alert0("not CLAMPEX");
	}

	if (false)
	{
		if (false)
		{
			for (int s=16; s<100; s++){
				UINT pos = 256+26+s;
				f.Seek(pos, CFile::begin);
				__int16 ee;
				ReByNS(ee);
				ShowFloat(s, "s");
				ShowFloat(ee, "Epi 1");
				pos = 256 + 26 + 2*s;
				ReByNS(ee);
				ShowFloat(ee, "Epi 2");
			}
		}
		UINT s1 = 26; // for clampexfiles
		UINT s2 = 24; // for fetchanfiles

		UINT n1 = (s-256)/s1;
		UINT n2 = (s-256)/s2;

		UINT S1 = n1*s1+256;
		UINT S2 = n2*s2+256;

		if (IsClampex){
			NEvents = n1;
		}
		else{
			NEvents = n2;
		}
		ShowFloat(s, "s");
		ShowFloat(n1, "n1");
		ShowFloat(n2, "n2");
		ShowFloat(S1, "S1");
		ShowFloat(S2, "S2");
	}



/*
	bool TryBoth = false;
	if (S1 != s){
		Alert0("Must be not clampex");
		if (S2 != s){
			Alert0("But is also not fetchex");
			return false;
		}
		IsClampex = false;
		NEvents = n2;
	}
	else{
		if (S2 != s){
			Alert0("Must be clampex");
			IsClampex = true;
			NEvents = n1;
		}
		else{
			Alert0("CANNOT DECIDE - ASSUME NOT CLAMPEX");
			TryBoth = true;
			IsClampex = true;
			NEvents = n1;
		}
	}
*/


	int e;
	int NE=0;

	CWaitCursor dummy;
	PClampEvlEvent TestEvent;
	while (TestEvent.ReadFromFile(f, IsClampex, NE, false)){
		NE++;
	}
	NEvents = NE;
	ShowFloat(NE, " events");
	Alloc();

	f.SeekToBegin();
	if (!header.ReadFromFile(f)){
		return false;
	}

	for (e=0; e<NEvents; e++){
		if (!events[e].ReadFromFile(f, IsClampex, e, false)){
			Alert0("Error reading event");
			return false;
		}
	}

	bool ErrorDetected = false;
//	if (TryBoth){
		for (e=0; e<NEvents; e++){
			PClampEvlEvent & evt = events[e];
			if (evt.LevelStart <0){
				ErrorDetected = true;
				break;
			}
			if (evt.LevelLength<0){
				ErrorDetected = true;
				break;
			}
			if (evt.EventsStandardDev<0.0){
				ErrorDetected = true;
				break;
			}

		}

//	}
	if (ErrorDetected){
//		ShowFloat(NEvents, "NEvents");
		ShowFloat(e, "e");

		Alert0("error detect");
	}
	else{
//		Alert0("OK");
	}

	return true;
}
bool PClampEvl::WriteToFile(CFile & f){
	if (NEvents<1){
		return false;
	}
	if (!events){
		return false;
	}
	if (!header.WriteToFile(f)){
		return false;
	}
	int e;
	for (e=0; e<NEvents; e++){
		if (!events[e].WriteToFile(f)){
			return false;
		}
	}

	return true;
}

bool PClampEvl::MakeEventFile(__int16 * data, int NData, double gain, double SampleTime){
	if (!data){
		return false;
	}
	if (NData<1){
		return false;
	}
#define MAXLEVELSINMAKEEVENTFILEPCLAMPEVL 10000

	__int16 levels[MAXLEVELSINMAKEEVENTFILEPCLAMPEVL];

	int NL=1;
	levels[0]=data[0];

	int i;
	for (i=1; i<NData; i++){
		__int16 d = data[i];
		bool NewLevel = true;
		for (int j=0; j<NL; j++){
			if (levels[j] == d){
				NewLevel = false;
//				break;
			}
		}
		if (NewLevel){
			levels[NL] = d;
			NL++;
			if (NL>=MAXLEVELSINMAKEEVENTFILEPCLAMPEVL){
				Alert0("too many levels");
				return false;
			}
		}
	}
	ShowFloat(NL, "levels");
	
	Order(levels, NL, false);

	__int16 lastd = levels[0]-1;

	// Smallest level first

	double level0 = double(levels[0])*gain;
	double levelN = double(levels[NL-1])*gain;

	if (fabs(levelN)<fabs(level0)){ // The levels are negative
		Order(levels, NL, true);
	}

	// Now the baseline (defined as level with smallest absolute current level) is level0

	header.AcquistionFilter = 1.0/2.0/SampleTime;
	header.AnalysisFilter = header.AcquistionFilter;
	header.EpisodeSize = NData;
	header.IsClampex = true;
	header.NumLevels = NL;
	header.TimePerPointInMicroSecs = SampleTime*1e6;
	header.TriggerStart = 0;
	header.TriggerEnd = 0;
	header.Gaps = header.TimePerPointInMicroSecs;

	NEvents = 1;
	__int16 initlevel = data[0];
	for (i=1; i<NData; i++){
		if (data[i] != initlevel){
			initlevel = data[i]; 
			NEvents++;
		}
	}


	Alloc();
	int e=-1;

	for (i=0; i<NData; i++){
		int d = data[i];
		if (d != lastd){
			e++;
			events[e].LevelLength = 1;
			events[e].LevelStart = i;
			events[e].EventEpi = 1;
			events[e].IsClampex = header.IsClampex;
			for (int j=0; j<NL; j++){
				if (d == levels[j]){
					events[e].EventCurrentLevel = j;
				}
			}
			events[e].Amplitude = float(d)*gain*1e12;
			events[e].EventsStandardDev = 0.1;//events[e].Amplitude/10.0;
			lastd = d;
		}
		else{
			events[e].LevelLength++;
		}
	}

	return true;



}
