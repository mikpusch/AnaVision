#include "stdafx.h"

#include <math.h>

#include "..\common\NewAbf.h"
#include "..\common\utils.h"
//#include "..\NewAbf\AbfFiles.h"

bool									NewAbf::Initialized					= false;
HINSTANCE								NewAbf::hDLL						= NULL;

FuncABF_Close							NewAbf::MyABF_Close					= NULL;
FuncABF_ReadOpen						NewAbf::MyABF_ReadOpen				= NULL;
FuncABF_ReadChannel						NewAbf::MyABF_ReadChannel			= NULL;
FuncABF_GetNumSamples					NewAbf::MyABF_GetNumSamples			= NULL;
FuncABF_GetEpisodeDuration				NewAbf::MyABF_GetEpisodeDuration	= NULL;


/*
void ShowFloat(double value, CString text)
{
 char s1[20];

// _gcvt( value, 15, s1);
 _gcvt( value, 6, s1);

//MessageBox(NULL, CString(s1), text, MB_OK);
MessageBox(NULL, CString(s1), text, MB_OK);
}

void Alert0(CString message){
	// returns 0 if not confirm

//	int result = MessageBox(NULL, message, "Sorry", MB_OK);
	int result = MessageBox(NULL, message, _T("Alert"), MB_OK);
}
*/
bool NewAbf::CheckProcedureAddress(DWORD adr, CString name){
//	ShowInt(adr, "adr");
	if (adr) return true;
	CString errorstring=CString("Error in getting address of ");
	errorstring += name;
	DWORD err=GetLastError();

	char s[20];
	_itoa(err, s, 10);

	errorstring += CString("; error: ")+CString(s);
	Alert0(errorstring);

//	Beep(1000,30);
	ShutDown();

	return false;

}
void NewAbf::ShutDown(){
	if (hDLL){
		FreeLibrary(hDLL);
	}
	Initialized = false;
	hDLL = NULL;
}


NewAbf::NewAbf(bool OpenForRead){   // this creates the object
	FileOpen = false;
	OPENFORREAD = OpenForRead;
	int i=99;
}

bool NewAbf::OpenFile(CString & FileName){
	if (!Initialized){
//		Alert0("Not Initialized");
		return false;
	}
	if (FileOpen) {
//		Alert0("FileOpne");
		return false;
	}
	if (OPENFORREAD){
//		Alert0("Try ReadOpen");
		if (!MyABF_ReadOpen(CStringA(FileName), &hFile, ABCF_DATAFILE, &FH, &uMaxSamples, &dwMaxEpi, &nError )){
			return false;
		}
//		ShowFloat(dwMaxEpi, "dwMaxEpi");
		FileOpen = true;

		ShowInfo();

	}
	return true;
}

bool NewAbf::IsInitialized(bool ShowWarning){
	if (!Initialized){
		return InitNewAbf(ShowWarning);
	}
	return Initialized;
}

bool NewAbf::CheckRead(){
	if (!Initialized) return false;
	if (!OPENFORREAD) return false;
	if (!FileOpen) return false;
	return true;
}
bool NewAbf::CheckWrite(){
	if (!Initialized) return false;
	if (OPENFORREAD) return false;
	if (!FileOpen) return false;
	return true;
}

bool NewAbf::ReadChannel(int sweep, // 0-based
						int channel, // logical; from 0 to NChannelsvalable-1
						float * data){


	if (!CheckRead()) return false;

	if (channel<0) return false;
	
//	if (channel>=FH.nADCNumChannels) return false;
	if (channel>=ABCF_ADCCOUNT) return false;

	int chan = FH.nADCPtoLChannelMap[channel];
//	if (sweep==0) ShowFloat(chan, "chan");
	chan = channel;
//	chan = 0;

	UINT uNumSamples;

//	ShowFloat(chan, _T("chan"));
	BOOL result = MyABF_ReadChannel(hFile, &FH, chan, sweep+1, data, &uNumSamples, &nError);
//	Alert0("Hi1");

//	if (sweep==0) 	ShowFloat(uNumSamples, "uNumSamples");
	if (!result){
//		Alert0(_T("error reading channel"));
		if (nError==1012){
			if (channel<16){
				return ReadChannel(sweep, channel+1, data);
			}
		}
//		if (true) ShowFloat(nError, "error reading a sweep in NewAbf");
		if (sweep==0) ShowFloat(nError, _T("error reading a sweep in NewAbf"));
	}
	if (result)
		return true;
	else
		return false;
}
int NewAbf::GetActualAcqLength(){
	return FH.lActualAcqLength;
}
bool NewAbf::GetComment(char * s, int MaxChar){
	if (!CheckRead()) return false;

	int NComment = ABCF_FILECOMMENTLEN; // ABF CONST
	if (NComment>MaxChar) NComment = MaxChar;
	for (int ci=0; ci<NComment; ci++){
		s[ci] = FH.sFileComment[ci]; 
	}
	return true;
}

int NewAbf::GetNumberOfChannels(){
	if (!CheckRead()) return 0;
	return FH.nADCNumChannels;
}
int NewAbf::GetMaxSamples(){
	if (!CheckRead()) return 0;
	return this->uMaxSamples;
}

int NewAbf::GetNumberOfSweeps(){
	if (!CheckRead()) return 0;
	int result = FH.lActualEpisodes;
	int NS = result;

//	ShowFloat(NS, "NS");
/*
	return NS;
	if (result>0){
		int NP = GetNumberOfPointsOfSweep(0);
		int i;
		for (i=1; i<result; i++){
			if (GetNumberOfPointsOfSweep(i)<NP){
				NS--;
			}
		}
	}
*/
	return NS;
}

UINT NewAbf::GetNumberOfPointsOfSweep(DWORD sweep){
	if (!CheckRead()) return 0;
	UINT result = 0;
	MyABF_GetNumSamples(hFile, &FH, sweep+1, &result, &nError);
	return result;
//	return uMaxSamples;
}

double NewAbf::GetSampleTime(){
	if (!CheckRead()) return 0.0;
	int n0=GetNumberOfPointsOfSweep(0);
	if (n0<=0) return 1.0;

	double duration_in_ms;
	MyABF_GetEpisodeDuration(hFile, &FH, 1, &duration_in_ms, &nError);

	return duration_in_ms/double(n0)/1000.0;

}

double NewAbf::GetVHold(){
	if (!CheckRead()) return 0.0;
	return FH.fDACHoldingLevel[0]/1000.0;
}
int NewAbf::GetNoOfStimSegs(){
	int channel=-1;
	return GetNoOfStimSegs(channel);
}

int NewAbf::GetNoOfStimSegs(int & channel){
	if (!CheckRead()) return 0;
	int i, j;
	channel=-1;
//		ShowFloat(FH.fEpochInitLevel[0][0], "initòlevel");
//		ShowFloat(FH.fEpochLevelInc[0][0], "fEpochLevelInc");
//		ShowFloat(FH.lEpochInitDuration[0][0], "lEpochInitDuration");
	for (j=0; j<ABCF_DACCOUNT; j++){
		if (FH.nEpochType[j][0]!=0){
			channel = j;
			break;
		}
	}
//	ShowFloat(channel, _T("channel"));
	if (channel<0){
		return 0;
	}
	int NSegs = 0;
	for (i=0; i<ABCF_EPOCHCOUNT; i++){
		if (FH.nEpochType[channel][i]!=0){
			NSegs++;
		}
	}
	return NSegs;

}

void NewAbf::GetSegment(int i, double & A, double & T, double & SV, double & ST){
	if (!CheckRead()) return;
	int channel;
	int NSegs = GetNoOfStimSegs(channel);
	if (i>=NSegs){
		return;
	}
	A=FH.fEpochInitLevel[channel][i] ;
	T=double(FH.lEpochInitDuration[channel][i]);
	SV = FH.fEpochLevelInc[channel][i];
	ST=double(FH.lEpochDurationInc[channel][i]);

}


double NewAbf::GetGain(int nChannel, double & factorgain){
	factorgain = 1.0;
	if (!CheckRead()) return 0.0;

	float fact, offset;
   GetADCtoUUFactors(nChannel, &fact, &offset);

  char s[ABCF_ADCUNITLEN+1];
   //, s1[ADC_UNITLEN+1], s2[ADC_UNITLEN+1], 
	//   s3[ADC_UNITLEN+1], s4[ADC_UNITLEN+1];
  int i;
   for (i=0; i<ABCF_ADCUNITLEN; i++){
	   s[i]=FH.sADCUnits[nChannel][i];
//	   s1[i]=s2[i]=s3[i]=s4[i]=' ';
   }
//   s1[0]='p'; s1[1]='A';
//   s2[0]='µ'; s2[1]='A';
//   s3[0]='m'; s3[1]='A';
//   s4[0]='n'; s4[1]='A';
//   s[ADC_UNITLEN]=0;
//   s1[ADC_UNITLEN]=0;
//   s2[ADC_UNITLEN]=0;
//   s3[ADC_UNITLEN]=0;
//   s4[ADC_UNITLEN]=0;
//	CString c = CString(s);
//	CString c1 = CString(s1);
//	CString c2 = CString(s2);
//	CString c3 = CString(s3);
//	CString c4 = CString(s4);
//	Alert0(c);
//	Alert0(c1);

//	double factorgain=1;
//   Alert0(CString(s[0]));
//   Alert0(CString(s[1]));
	if (s[0]=='k'){
		factorgain=1e3;
//		Alert0("kilo");
	}
	if (s[0]=='M'){
		factorgain=1e6;
//		Alert0("Mega");
	}
	if (s[0]=='G'){
		factorgain=1e6;
//		Alert0("giga");
	}

//	if (c==c1){
	if (s[0]=='a'){
		factorgain=1e-18;
//		Alert0("atto");
	}
	if (s[0]=='f'){
		factorgain=1e-15;
//		Alert0("femto");
	}
	if (s[0]=='p'){
		factorgain=1e-12;
//		Alert0("pico");
	}
//	if (c==c2){
	if ((s[0]=='µ') | (s[0]=='u')){
		factorgain=1e-6;
//		Alert0("micro");
	}
	if (s[0]=='m'){
//	if (c==c3){
		factorgain=1e-3;
//		Alert0("rmilli");
	}
	if (s[0]=='n'){
//	if (c==c4){
		factorgain=1e-9;
//		Alert0("nano");
	}
//   WORD pwNumSamples;
// __int16 piBuffer[10000];
  //nErrorNum = ABF_ChannelRead( hABFFile, 1, nChannel, piBuffer, &(pwNumSamples) );
	// if (nErrorNum != ABF_SUCCESS) {
	//  Alert0("error reading episode");
	  //

//	ShowFloat(factorgain, "fg");
	double gain=factorgain;
 // ShowFloat(FH.lADCResolution, "lADCResolution");
	gain /= double(FH.lADCResolution);

//	ShowFloat(FH.fADCProgrammableGain[nChannel], "FH.fADCProgrammableGain");
//	ShowFloat(FH.fInstrumentScaleFactor[nChannel], "fInstrumentScaleFactor");
//	ShowFloat(FH.fADCRange, "fADCRange");
//	ShowFloat(FH.fSignalGain[nChannel], "fSignalGain");
//ShowFloat(FH.fADCDisplayAmplification[nChannel], "fADCDisplayAmplification");	
	gain *= double(FH.fADCRange);

//	gain *= 204.8;

	// big change:
	gain = fact * factorgain;

	BOOL GainNegative = false;
	if (gain<0.0){
		GainNegative = true;
		gain *= -1.0;
	}
	bool ABFIsFloatData = false;
	if (FH.nDataFormat>0){
//		ABF_IsFloatData(hABFFile);
		ABFIsFloatData = true;
	}


	if (ABFIsFloatData){
//	if (true){
		double gainb = gain;
		gain /= double(FH.fADCRange)*1000.0;
		gain *= double(FH.lADCResolution);
//		ShowFloat(gain/gainb, "f");
	}
	else{
	}

	return gain;

}

bool NewAbf::SetComment(char * s, int MaxChar){
	if (!CheckWrite()) return false;

	int NComment = ABCF_FILECOMMENTLEN; // ABF CONST
	if (NComment>MaxChar) NComment = MaxChar;
	for (int ci=0; ci<NComment; ci++){
		FH.sFileComment[ci] = s[ci]; 
	}
	return true;
}

bool NewAbf::SetNumberOfChannels(UINT nchan){
	if (!CheckWrite()) return false;
	FH.nADCNumChannels = nchan;
	return true;
}
bool NewAbf::SetNumberOfSweeps(UINT nsweep){
	if (!CheckWrite()) return false;
	FH.lActualEpisodes = nsweep;
	return true;
}
bool NewAbf::SetNumberOfPointsOfSweeps(UINT * puNPoints){
	if (!CheckWrite()) return false;
	return true;
}
bool NewAbf::SetSampleTime(double stInSeconds){
	if (!CheckWrite()) return false;
	return true;
}
bool NewAbf::SetVHold(double vhInVolts){
	if (!CheckWrite()) return false;
	return true;
}


NewAbf::~NewAbf(){
	if (FileOpen){
		Close();
	}
}
bool NewAbf::Close(){
	if (FileOpen){
		MyABF_Close(hFile, &nError);
		FileOpen = false;
		return true;
	}
	else{
		return false;
	}
}

bool NewAbf::InitNewAbf(bool ShowWarning){
	if (Initialized) return true;


//	hDLL=AfxLoadLibrary( "abffio.dll");
	hDLL=LoadLibrary( _T("abffio.dll"));


	if (!hDLL){
		if (ShowWarning){
			Alert0(_T("ERROR: could not find abffio.dll"));
		}
		return false;
	}

	MyABF_Close = (FuncABF_Close)GetProcAddress(hDLL, "ABF_Close");
	if  (!CheckProcedureAddress(DWORD(MyABF_Close), _T("MyABF_Close"))){
		return false;
	}

	MyABF_ReadOpen = (FuncABF_ReadOpen)GetProcAddress(hDLL, "ABF_ReadOpen");
	if  (!CheckProcedureAddress(DWORD(MyABF_ReadOpen), _T("MyABF_ReadOpen"))){
		return false;
	}

	MyABF_ReadChannel = (FuncABF_ReadChannel)GetProcAddress(hDLL, "ABF_ReadChannel");
	if  (!CheckProcedureAddress(DWORD(MyABF_ReadChannel), _T("MyABF_ReadChannel"))){
		return false;
	}
	MyABF_GetNumSamples = (FuncABF_GetNumSamples)GetProcAddress(hDLL, "ABF_GetNumSamples");
	if  (!CheckProcedureAddress(DWORD(MyABF_GetNumSamples), _T("MyABF_GetNumSamples"))){
		return false;
	}
	MyABF_GetEpisodeDuration = (FuncABF_GetEpisodeDuration)GetProcAddress(hDLL, "ABF_GetEpisodeDuration");
	if  (!CheckProcedureAddress(DWORD(MyABF_GetEpisodeDuration), _T("MyABF_GetEpisodeDuration"))){
		return false;
	}

	
	Initialized = true;
	return true;
}

void NewAbf::GetADCtoUUFactors( int nChannel, float *pfADCToUUFactor, float *pfADCToUUShift )
{
//   WPTRASSERT(pFH);
//   WPTRASSERT(pfADCToUUFactor);
//   WPTRASSERT(pfADCToUUShift);
//   ASSERT(nChannel < ABF_ADCCOUNT);

	if (!CheckRead()){
		return;
	}
	ABCFFileHeader * pFH = &FH;

   float fTotalScaleFactor = pFH->fInstrumentScaleFactor[nChannel] *
                             pFH->fADCProgrammableGain[nChannel];
//  ShowFloat(pFH->fInstrumentScaleFactor[nChannel], "pFH->fInstrumentScaleFactor[nChannel]");
//  ShowFloat(pFH->fADCProgrammableGain[nChannel], "pFH->fADCProgrammableGain[nChannel]");
   if (pFH->nSignalType != 0)
      fTotalScaleFactor *= pFH->fSignalGain[nChannel];
//  ShowFloat(pFH->fSignalGain[nChannel], "pFH->fSignalGain[nChannel]");
//   if (pFH->nAutosampleEnable && (pFH->nAutosampleADCNum == nChannel))
 //     fTotalScaleFactor *= pFH->fAutosampleAdditGain;
//  ShowFloat(pFH->fAutosampleAdditGain, "pFH->fAutosampleAdditGain");
//   ShowFloat(fTotalScaleFactor, "fTotalScaleFactor");

//   ASSERT(fTotalScaleFactor != 0.0F);
   if (fTotalScaleFactor==0.0F)
      fTotalScaleFactor = 1.0F;

   // InputRange and InputOffset is the range and offset of the signal in
   // user units when it hits the Analog-to-Digital converter

   float fInputRange = pFH->fADCRange / fTotalScaleFactor;
//   ShowFloat(pFH->fADCRange , "pFH->fADCRange ");
   float fInputOffset= -pFH->fInstrumentOffset[nChannel];
   if (pFH->nSignalType != 0)
      fInputOffset += pFH->fSignalOffset[nChannel];

   *pfADCToUUFactor = fInputRange / pFH->lADCResolution;

   *pfADCToUUShift  = -fInputOffset;
}
short NewAbf::GetOperationMode(){
	return FH.nOperationMode;
}

void NewAbf::ShowInfo(){
	return;
		ShowFloat(uMaxSamples, _T("uMaxSamples"));
//		ShowFloat(dwMaxEpi, "dwMaxEpi");
	ShowFloat(FH.lActualEpisodes, _T("ActualEpsi"));
	ShowFloat(FH.nADCNumChannels, _T("nADCNumChannels"));
		int i, j;
//		for (i=0; i<FH.nADCNumChannels; i++){
//			ShowFloat(FH.nADCPtoLChannelMap[i], "FH.nADCPtoLChannelMap[i]");
//		}

//		ShowFloat(this->GetSampleTime(), "ST");
		ShowFloat(FH.fEpochInitLevel[0][0], _T("initlevel"));
		ShowFloat(FH.fEpochLevelInc[0][0], _T("fEpochLevelInc"));
		ShowFloat(FH.lEpochInitDuration[0][0], _T("lEpochInitDuration"));
		for (j=0; j<ABCF_DACCOUNT; j++){
			CString c;
				char s[20];
			for (i=0; i<ABCF_EPOCHCOUNT; i++){

				_itoa(FH.nEpochType[j][i], s, 10);
				c += CString(s);
			}
			_itoa(j, s, 10);
			c = CString(s)+CString(": ")+ c;
			Alert0(c);
		
//				ShowFloat(FH.nEpochType[1][i], "Type");

		}

		CString sFileType;
		for (i=0; i<4; i++){
			char * s = (char *)&FH.fFileVersionNumber;
			sFileType += s[i];
		}
//		Alert0(sFileType);
//		ShowFloat(FH.fFileVersionNumber, "VN");
//   char     sFileType[FILETYPELEN];
 //   float    fFileVersionNumber;
		ShowFloat(FH.nOperationMode, _T("OperationMode"));
		ShowFloat(FH.lActualAcqLength, _T("FH.lActualAcqLength"));
//  long     lActualAcqLength;
/*
    short    nNumPointsIgnored;
    long     lActualEpisodes;
    long     lFileStartDate;
    long     lFileStartTime;
    long     lStopwatchTime;
    float    fHeaderVersionNumber;
    short    nFileType;
    short    nMSBinFormat;

    // GROUP #2 (80 bytes)
    long     lDataSectionPtr;
    long     lTagSectionPtr;
    long     lNumTagEntries;
    long     lLongDescriptionPtr;
    long     lLongDescriptionLines;
    long     lDACFilePtr;
    long     lDACFileNumEpisodes;
    char     sUnused68[4];
    long     lDeltaArrayPtr;
    long     lNumDeltas;
    long     lNotebookPtr;
    long     lNotebookManEntries;
    long     lNotebookAutoEntries;
    long     lSynchArrayPtr;
    long     lSynchArraySize;
    short    nDataFormat;
    char     sUnused102[18];

    // GROUP #3 (80 bytes)
    short    nADCNumChannels;
    float    fADCSampleInterval;
    float    fADCSecondSampleInterval;
    float    fSynchTimeUnit;
    float    fSecondsPerRun;
    long     lNumSamplesPerEpisode;
    long     lPreTriggerSamples;
    long     lEpisodesPerRun;
    long     lRunsPerTrial;
    long     lNumberOfTrials;
    short    nAveragingMode;
    short    nUndoRunCount;
    short    nFirstEpisodeInRun;
    float    fTriggerThreshold;
    short    nTriggerSource;
    short    nTriggerAction;
    short    nTriggerPolarity;
    float    fScopeOutputInterval;
    float    fEpisodeStartToStart;
    float    fRunStartToStart;
    float    fTrialStartToStart;
    long     lAverageCount;
    long     lClockChange;
    char     sUnused198[2];

    // GROUP #4 (44 bytes)
    short    nDrawingStrategy;
    short    nTiledDisplay;
    short    nEraseStrategy;
    short    nDataDisplayMode;
    long     lDisplayAverageUpdate;
    short    nChannelStatsStrategy;
    long     lCalculationPeriod;
    long     lSamplesPerTrace;
    long     lStartDisplayNum;
    long     lFinishDisplayNum;
    short    nMultiColor;
    short    nShowPNRawData;
    char     sUnused234[10];

    // GROUP #5 (16 bytes)
    float    fADCRange;
    float    fDACRange;
    long     lADCResolution;
    long     lDACResolution;

    // GROUP #6 (118 bytes)
    short    nExperimentType;
    short    nAutosampleEnable;
    short    nAutosampleADCNum;
    short    nAutosampleInstrument;
    float    fAutosampleAdditGain;
    float    fAutosampleFilter;
    float    fAutosampleMembraneCap;
    short    nManualInfoStrategy;
    float    fCellID1;
    float    fCellID2;
    float    fCellID3;
    char     sCreatorInfo[CREATORINFOLEN];
    char     sFileComment[FILECOMMENTLEN];
    char     sUnused366[12];

    // GROUP #7 (1044 (160 + 384 + 488 + 12) bytes) - Multi-channel information.
    short    nADCPtoLChannelMap[ADC_COUNT];
    short    nADCSamplingSeq[ADC_COUNT];
    char     sADCChannelName[ADC_COUNT][ADC_NAMELEN];
    char     sADCUnits[ADC_COUNT][ADC_UNITLEN];
    float    fADCProgrammableGain[ADC_COUNT];
    float    fADCDisplayAmplification[ADC_COUNT];// 129
    float    fADCDisplayOffset[ADC_COUNT];       // 145
    float    fInstrumentScaleFactor[ADC_COUNT];  // 113
    float    fInstrumentOffset[ADC_COUNT];       // 97
    float    fSignalGain[ADC_COUNT];
    float    fSignalOffset[ADC_COUNT];
    float    fSignalLowpassFilter[ADC_COUNT];
    float    fSignalHighpassFilter[ADC_COUNT];
    char     sDACChannelName[DAC_COUNT][DAC_NAMELEN];
    char     sDACChannelUnits[DAC_COUNT][DAC_UNITLEN];
    float    fDACScaleFactor[DAC_COUNT];
    float    fDACHoldingLevel[DAC_COUNT];
    short    nSignalType;
    char     sUnused1412[10];

    // GROUP #8 (14 bytes) - Synchronous timer outputs.
    short    nOUTEnable;
    short    nSampleNumberOUT1;
    short    nSampleNumberOUT2;
    short    nFirstEpisodeOUT;
    short    nLastEpisodeOUT;
    short    nPulseSamplesOUT1;
    short    nPulseSamplesOUT2;

    // GROUP #9 (184 bytes) - Epoch Output Waveform and Pulses;
    short    nDigitalEnable;
    short    nWaveformSource;
    short    nActiveDACChannel;
    short    nInterEpisodeLevel;
    short    nEpochType[EPOCH_COUNT];
    float    fEpochInitLevel[EPOCH_COUNT];
    float    fEpochLevelInc[EPOCH_COUNT];
    short    nEpochInitDuration[EPOCH_COUNT];
    short    nEpochDurationInc[EPOCH_COUNT];
    short    nDigitalHolding;
    short    nDigitalInterEpisode;
    short    nDigitalValue[EPOCH_COUNT];
    float    fWaveformOffset;
    char     sUnused1612[8];

    // GROUP #10 (98 bytes) - Analog Output File Waveform
    float    fDACFileScale;
    float    fDACFileOffset;
    char     sUnused1628[2];
    short    nDACFileEpisodeNum;
    short    nDACFileADCNum;
    char     sDACFileName[DACFILENAMELEN];
    char     sDACFilePath[DACFILEPATHLEN];
    char     sUnused1706[12];

    // GROUP #11 (44 bytes) - Conditioning pulse train.
    short    nConditEnable;
    short    nConditChannel;
    long     lConditNumPulses;
    float    fBaselineDuration;
    float    fBaselineLevel;
    float    fStepDuration;
    float    fStepLevel;
    float    fPostTrainPeriod;
    float    fPostTrainLevel;
    char     sUnused1750[12];

    // GROUP #12 ( 82 bytes) - Variable parameter list.
    short    nParamToVary;
    char     sParamValueList[VARPARAMLISTLEN];

    // GROUP #13 (36 bytes) - Autopeak measurement.
    short    nAutopeakEnable;
    short    nAutopeakPolarity;
    short    nAutopeakADCNum;
    short    nAutopeakSearchMode;
    long     lAutopeakStart;
    long     lAutopeakEnd;
    short    nAutopeakSmoothing;
    short    nAutopeakBaseline;
    short    nAutopeakAverage;
    char     sUnused1866[14];

    // GROUP #14 (52 bytes) - Channel Arithmetic
    short    nArithmeticEnable;
    float    fArithmeticUpperLimit;
    float    fArithmeticLowerLimit;
    short    nArithmeticADCNumA;
    short    nArithmeticADCNumB;
    float    fArithmeticK1;
    float    fArithmeticK2;
    float    fArithmeticK3;
    float    fArithmeticK4;
    char     sArithmeticOperator[ARITHMETICOPLEN];
    char     sArithmeticUnits[ARITHMETICUNITSLEN];
    float    fArithmeticK5;
    float    fArithmeticK6;
    short    nArithmeticExpression;
    char     sUnused1930[2];

    // GROUP #15 (34 bytes) - On-line subtraction.
    short    nPNEnable;
    short    nPNPosition;
    short    nPNPolarity;
    short    nPNNumPulses;
    short    nPNADCNum;
    float    fPNHoldingLevel;
    float    fPNSettlingTime;
    float    fPNInterpulse;
    char     sUnused1954[12];

    // GROUP #16 (54 bytes) - Unused space at end of header block.
    short    nListEnable;
    char     sUnused1968[80];
*/
}


