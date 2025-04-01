#include "stdafx.h"
//#include "ana.h"
#include "../common/writeabf.h"
#include "../common/utils.h"
#include "../abf/MyAbf.h"

BOOL		WriteAbf::Initialized			= FALSE;
HGLOBAL		WriteAbf::HeaderResourcePointer = NULL;
char *		WriteAbf::abfh					= NULL;

#define WRITE_ABF_HEADER_SIZE 8192

WriteAbf::WriteAbf(){

	FileType =  0;	// = pulsed
					// 1 = continuous

	VHold = 0;		// in V
	SampleTime = 1e-3;	//in sec

	NChannels=1;
	NSweeps=1;		// must be = 1 for continuous file type
	NPointsPerSweep = 1;

	NSegs = 1;

	ADCResolution = 1;

	int i;
	for (i=0; i<MAXSEGSINWRTEABF; i++){
		Volt[i]=0; // in V
		DeltaVolt[i]=0; // in V
		Time[i]=.1; // in secs
		DeltaTime[i]=0; // in secs
	}
	for (i=0; i<MAXCHANNELSINWRTEABF; i++){
		Gain[i]=1.0;
	}
	

}

void WriteAbf::Initialize(int ID){
	if (Initialized) return;

	if (!DoLoadResource(ID)){
		return;
	}

	abfh = new char[WRITE_ABF_HEADER_SIZE];
	char * c  = (char *) HeaderResourcePointer;

	int i;
	for (i=0; i<WRITE_ABF_HEADER_SIZE; i++){
		abfh[i] = c[i];
	}

	ABFFileHeader * pFH = (ABFFileHeader *)(abfh);

	pFH->lADCResolution = 1;
	pFH->fAutosampleAdditGain = 1.0;
	pFH->fADCRange = 1.0;

	int ch;
	for (ch=0; ch<MAXCHANNELSINWRTEABF; ch++){
		pFH->fADCProgrammableGain[ch] = 1.0;
		pFH->fSignalGain[ch]=1.0;
		pFH->fADCDisplayAmplification[ch]=1.0;
	}

	for (i=0; i<FILECOMMENTLEN; i++){
		pFH->sFileComment[i]=0;
	}

	pFH->lFileStartDate = 0;
    pFH->lFileStartTime = 0;
	pFH->lPreTriggerSamples = 0;
	pFH->lRunsPerTrial = 1;
	pFH->lNumberOfTrials = 1;
	pFH->nAveragingMode = 0;
	pFH->nUndoRunCount = 0;
    pFH->nFirstEpisodeInRun = 1;

  
	Initialized = TRUE;

}

BOOL WriteAbf::DoLoadResource(int ResourceID){

	CString ccc="#";
	char s[20];
	_itoa(ResourceID, s,10);
		
	ccc+=CString(s);

	HRSRC HeaderResourceHandle = FindResource( NULL, ccc, "ABF_HEADER");

	if (!HeaderResourceHandle) {
		Alert0("failed finding abf-header resource: FATAL ERROR");
		return FALSE;
	}

	HeaderResourcePointer = LoadResource(NULL, HeaderResourceHandle);

	if (!HeaderResourcePointer){
		Alert0(" abf-header resource: FATAL ERROR ");
		return FALSE;
	}
	return TRUE;
}

void WriteAbf::Shutdown(){
	if (abfh){
		delete [] abfh;
	}
	abfh = NULL;
	Initialized = FALSE;
}



void WriteAbf::WriteHeader(CFile & f){

	if (!Initialized){
		Alert0("FATAL ERROR IN WriteAbf::WriteHeader: NOT initialized");
		return;
	}

	if (NChannels<1){
		Alert0("nchannels < 1 in WriteHeader");
		return;
	}
	if (NChannels>MAXCHANNELSINWRTEABF){
		Alert0("nchannels > MAXCHANNELSINWRTEABF in WriteHeader");
		return;
	}

	ABFFileHeader * pFH = (ABFFileHeader *)(abfh);

	int i;

	pFH->nOperationMode = ABF_WAVEFORMFILE;

	if (FileType==1){
		pFH->nOperationMode = ABF_GAPFREEFILE;
	}


	pFH->nADCNumChannels = NChannels;
//	ShowFloat(NChannels, "NChannels");
	for (i=0; i<ADC_COUNT; i++){
		pFH->nADCSamplingSeq[i]=-1;
	}
	for (i=0; i<NChannels; i++){
		pFH->nADCSamplingSeq[i]=i;
	}
	pFH->fADCSampleInterval = SampleTime*1e6/double(NChannels);

	pFH->lNumSamplesPerEpisode = NPointsPerSweep*NChannels; // OK

	pFH->lEpisodesPerRun = NSweeps;
	pFH->lActualAcqLength = NSweeps*NChannels*NPointsPerSweep; // OK

	pFH->lActualEpisodes = NSweeps;
	
	pFH->fDACHoldingLevel[0] = VHold*1000.0;

	pFH->lADCResolution = ADCResolution;


	for (i=0; i<MAXSEGSINWRTEABF; i++){
		pFH->nEpochType[i]=0;
	}

	if (NSegs<0) NSegs=0;
	if (NSegs>MAXSEGSINWRTEABF) NSegs=MAXSEGSINWRTEABF;

//	ShowFloat(NSegs, "NSegs");

	for (i=0; i<NSegs; i++){
		pFH->nEpochType[i]=1;
		pFH->fEpochInitLevel[i] = 1000.0*Volt[i];
		pFH->nEpochInitDuration[i] = Time[i]/SampleTime;
		pFH->fEpochLevelInc[i] = DeltaVolt[i]*1000.0;
		pFH->nEpochDurationInc[i] = DeltaTime[i]/SampleTime;
	}


	for (int ch=0; ch<NChannels; ch++){
		pFH->fInstrumentScaleFactor[ch] = float(1e-12/Gain[ch]/double(ADCResolution));
//		ShowFloat(pFH->fInstrumentScaleFactor[ch], "gain");
	}

	f.SeekToBegin();
	f.Write(abfh, WRITE_ABF_HEADER_SIZE);


}



void WriteAbf::WriteSweep(CFile & f, int sweep, __int16 ** data, int NPointsInThisSweep){
	if (NChannels<1) return;
	if (NChannels>MAXCHANNELSINWRTEABF) return;
	if (NPointsPerSweep<1) return;
	if (NPointsInThisSweep<0) return;

	UINT pos = WRITE_ABF_HEADER_SIZE + sweep*NChannels*2*NPointsPerSweep;

//	ShowFloat(pos, "Pos");

	f.Seek(pos, CFile::begin);

	for (int i=0; i<NPointsPerSweep; i++){
		__int16 ivalue;
		for (int ch=0; ch<NChannels; ch++){
			if (i<NPointsInThisSweep){
				ivalue = data[ch][i];
			}
			else{
				ivalue = 0;
			}
			f.Write(&ivalue, 2);
		}
	}
}
