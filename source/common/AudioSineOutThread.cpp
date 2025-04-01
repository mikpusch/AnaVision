// AudioSineOutThread.cpp : implementation file
//

#include "stdafx.h"
//#include "TestAudio.h"
#include "../common/AudioSineOutThread.h"
#include "../common/utils.h"
#include <math.h>
//#include "../GePulse/Gepulse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CRITICAL_SECTION AudioSineOutThread::waveCriticalSection	= 0;
WAVEHDR* AudioSineOutThread::waveBlocks						= NULL;
volatile int AudioSineOutThread::waveFreeBlockCount			= 0;
volatile bool AudioSineOutThread::Break						= false;
int AudioSineOutThread::waveCurrentBlock					= 0;
bool AudioSineOutThread::Initialized						= false;
double AudioSineOutThread::frequency						= 1000;
bool AudioSineOutThread::Error								= false;
double AudioSineOutThread::x								= 0; 
double AudioSineOutThread::deltax							= 0; 

/////////////////////////////////////////////////////////////////////////////
// AudioSineOutThread

//Global
CRITICAL_SECTION waveCriticalSection;

IMPLEMENT_DYNCREATE(AudioSineOutThread, CWinThread)

AudioSineOutThread::AudioSineOutThread()
{
//	frequency = 1000;
	Break = false;
}

AudioSineOutThread::~AudioSineOutThread()
{
}


void AudioSineOutThread::Initialize(){
	if (Initialized){
		return;
	}

	waveBlocks = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
    waveFreeBlockCount = BLOCK_COUNT;
    waveCurrentBlock= 0;
    InitializeCriticalSection(&waveCriticalSection);

	HWAVEOUT hWaveOut; /* device handle */
    WAVEFORMATEX wfx; /* look this up in your documentation */

	wfx.nSamplesPerSec = 44100; /* sample rate */
    wfx.wBitsPerSample = 16; /* sample size */
    wfx.nChannels= 1; /* channels*/
    wfx.cbSize = 0; /* size of _extra_ info */
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

    /*
    * try to open the default wave device. WAVE_MAPPER is
    * a constant defined in mmsystem.h, it always points to the
    * default wave device on the system (some people have 2 or
    * more sound cards).
    */

   Initialized = true;
   if(waveOutOpen(
    &hWaveOut, 
    WAVE_MAPPER, 
    &wfx, 
    (DWORD) waveOutProc, 
    (DWORD) &waveFreeBlockCount, 
    CALLBACK_FUNCTION


        ) != MMSYSERR_NOERROR) {
//		Alert0("unable to open wave mapper device"); 
//        AfxEndThread(0);
//		delete this;
//		return TRUE;
	   Error = true;
    }
   else{
	   waveOutClose(hWaveOut);
   }
}

void AudioSineOutThread::CheckFrequency(){

	if (frequency<10.0) frequency = 10.0;
	if (frequency>44100.0) frequency = 44100.0;
}

#define TWO_PI_AudioSineOutThread 6.283185307179586
void AudioSineOutThread::FillBuffer(){
	deltax=TWO_PI_AudioSineOutThread*frequency/44100.0;
	return;
	bufferLength = (int)(44100.0/frequency);
	if (bufferLength>44100) bufferLength=44100;
	double r=2.0*3.14159/double(bufferLength);
	for(int i=0; i<=bufferLength; i++){
	    buffer[i] = __int16(16000.0*sin(r*double(i)));
	}
}

void AudioSineOutThread::ShutDown(){
//	while(waveFreeBlockCount < BLOCK_COUNT)
//		Sleep(10);
    /*
    * unprepare any blocks that are still prepared
    */
/*
    for(int i = 0; i < waveFreeBlockCount; i++) 
	    if(waveBlocks[i].dwFlags & WHDR_PREPARED)
			waveOutUnprepareHeader(hWaveOut, &waveBlocks[i], sizeof(WAVEHDR));
*/	
	DeleteCriticalSection(&waveCriticalSection);
	
	freeBlocks(waveBlocks);

//    waveOutClose(hWaveOut);
}

WAVEHDR* AudioSineOutThread::allocateBlocks(int size, int count)
    {
    unsigned char* buffer;
    int i;
    WAVEHDR* blocks;
    DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
    /*
    * allocate memory for the entire set in one go
    */
	buffer = (unsigned char* )HeapAlloc(   GetProcessHeap(), HEAP_ZERO_MEMORY, totalBufferSize);
	if (!buffer)  {
		return 0;
/*		ShutDown();
        fprintf(stderr, "Memory allocation error\n");
        ExitProcess(1);
*/
    }
    /*
    * and set up the pointers to each bit
    */
    blocks = (WAVEHDR*)buffer;
    buffer += sizeof(WAVEHDR) * count;


    for(i = 0; i < count; i++) {
        blocks[i].dwBufferLength = size;
        blocks[i].lpData = (char *)buffer;
        buffer += size;
    }
    return blocks;
}

void AudioSineOutThread::freeBlocks(WAVEHDR* blockArray) {
    /* 
    * and this is why allocateBlocks works the way it does
    */ 
    HeapFree(GetProcessHeap(), 0, blockArray);
}

void AudioSineOutThread::writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size) {
	if (!Initialized){
		Sleep(20);
		return;
	}

    while(!waveFreeBlockCount){
        Sleep(1);
	}

    WAVEHDR* current;
    int remain;
    current = &waveBlocks[waveCurrentBlock];

	while(size > 0) {
        /* 
        * first make sure the header we're going to use is unprepared
        */
        if(current->dwFlags & WHDR_PREPARED){ 
			 waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));
		}
		int dwuser=current->dwUser;
        remain = BLOCK_SIZE - dwuser;
//        if(size < (int)(BLOCK_SIZE - current->dwUser)) {
        if(size < (int)(remain)) {
            memcpy(current->lpData + dwuser, data, size);
            current->dwUser += size;
            break;
        }
//        remain = BLOCK_SIZE - current->dwUser;
        //     ;
        memcpy(current->lpData + current->dwUser, data, remain);
        size -= remain;
        data += remain;
        current->dwBufferLength = BLOCK_SIZE;
        waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
        EnterCriticalSection(&waveCriticalSection);
        waveFreeBlockCount--;
        LeaveCriticalSection(&waveCriticalSection);
        /*
        * wait for a block to become free
        */
        while(!waveFreeBlockCount){
	        Sleep(1);
		}
        //Sleep(1);
        /*
        * point to the next block
        */
        waveCurrentBlock++;
        waveCurrentBlock %= BLOCK_COUNT;
        current = &waveBlocks[waveCurrentBlock];
        current->dwUser = 0;

/*
		CMainFrame * f = (CMainFrame *) AfxGetMainWnd();
		CDC * pDC=f->GetDC();
		char s[20];
		int ii=waveCurrentBlock;
		_itoa(ii, s, 10);

		pDC->TextOut(10,120, CString("    ")+CString(s)+CString("  waveCurrentBlock            "));
		f->ReleaseDC(pDC);
*/	
    }
}

//int count;

void CALLBACK AudioSineOutThread::waveOutProc(
			HWAVEOUT hWaveOut, 
			UINT uMsg, 
			DWORD dwInstance, 
			DWORD dwParam1,
			DWORD dwParam2 )  {
    /*
    * pointer to free block counter
    */
    int* freeBlockCounter = (int*)dwInstance;
    /*
    * ignore calls that occur due to openining and closing the
    * device.
    */
//	count ++;
/*
	CMainFrame * f = (CMainFrame *) AfxGetMainWnd();
	CDC * pDC=f->GetDC();
	char s[20];
	int i=count;
	_itoa(i, s, 10);
*/
    if(uMsg != WOM_DONE){
//		pDC->TextOut(10,10, CString(s)+CString("  NOT wom_done            "));
		return;
	}
//	pDC->TextOut(10,10, CString(s)+CString("       dome      "));
	

    EnterCriticalSection(&waveCriticalSection);
//	waveFreeBlockCount++;
    (*freeBlockCounter)++;
//	i=(*freeBlockCounter);
    LeaveCriticalSection(&waveCriticalSection);

/*
	_itoa(i, s, 10);
	pDC->TextOut(10,40, CString(s)+CString("              "));
	f->ReleaseDC(pDC);
*/
}


BOOL AudioSineOutThread::InitInstance(){
//	count = 0;

	wfx.nSamplesPerSec = 44100; /* sample rate */
    wfx.wBitsPerSample = 16; /* sample size */
    wfx.nChannels= 1; /* channels*/
    wfx.cbSize = 0; /* size of _extra_ info */
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	waveFreeBlockCount = BLOCK_COUNT;
    /*
    * try to open the default wave device. WAVE_MAPPER is
    * a constant defined in mmsystem.h, it always points to the
    * default wave device on the system (some people have 2 or
    * more sound cards).
    */


   if(waveOutOpen(
    &hWaveOut, 
    WAVE_MAPPER, 
    &wfx, 
    (DWORD) waveOutProc, 
    (DWORD) &waveFreeBlockCount, 
    CALLBACK_FUNCTION


        ) != MMSYSERR_NOERROR) {
//		Alert0("unable to open wave mapper device"); 
//        AfxEndThread(0);
//		delete this;
//		return TRUE;
		return true;
    }
  
	CheckFrequency();

	double OldFrequency = frequency;

	FillBuffer();
	x=0;

//	int waveindex = 0;
#define intbuffersize_AUDIO_SINE_THREAD 1024
	__int16 intbuffer[intbuffersize_AUDIO_SINE_THREAD]; /* intermediate buffer for reading */

//	int count=0;
	while (true){
/*
		count ++;
		CMainFrame * f = (CMainFrame *) AfxGetMainWnd();
		CDC * pDC=f->GetDC();
		char s[20];
		int ii=count;
		_itoa(ii, s, 10);

		pDC->TextOut(10,80, CString("    ")+CString(s)+CString("  count            "));
*/	
		if (Break){
//			Alert0("hi");
			waveOutReset(hWaveOut);
			waveOutClose(hWaveOut);

			AfxEndThread(0);

			delete this;
			return true;
		}

		if (frequency != OldFrequency){
			CheckFrequency();
			FillBuffer();
			OldFrequency = frequency;
//			waveindex = 0;
		}
		for(int i=0; i<intbuffersize_AUDIO_SINE_THREAD; i++) {   //fill the output frame
//			intbuffer[2*i] = intbuffer[2*i+1] = buffer[waveindex];
			intbuffer[i] = __int16(16000.0*sin(x));
			x += deltax;
			if (x>TWO_PI_AudioSineOutThread) x -= TWO_PI_AudioSineOutThread;
			
//			buffer[waveindex];
//			ShowFloat(intbuffer[i], "v");
//			waveindex++;
//			if (waveindex>=bufferLength){
//				waveindex = 0;
//			}
		}
        writeAudio(hWaveOut, (char*)(&(intbuffer[0])), sizeof(intbuffer));
//		Alert0("written");
	}
	return true;
}

/*
BOOL AudioSineOutThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here



	ALConfig AL;

	ALRESULT result;
	ALPort outputPort;



	AL = ALCreateConfig();
//	Alert0("Hi0");
//	return FALSE;
	ALSetSampleWidth(AL, AL_SAMPLE_WIDTH_16);
//	Alert0("Hi1");
	ALSetNumQueueFrames(AL, 3);
//	Alert0("Hi2");
	ALSetFrameLength(AL, FRAME_LENGTH);
//	Alert0("Hi3");
	ALSetNumChannels(AL, AL_MONO);
//	Alert0("Hi4");
	ALSetSamplingRate(AL, AL_SAMPLING_RATE_44100);
//	Alert0("Hi5");
	ALSetSampleFormat(AL, AL_SAMPLE_FORMAT_FLOAT);
//	Alert0("Hi6");
//	result = ALOpenPort(&outputPort, outputConfig, AL_PORT_OUTPUT);
	result = ALOpenPort(&outputPort, AL, AL_PORT_OUTPUT);
//	Alert0("Hi7");

  //make sure we successfully opened the port
	if (result != ALSYSERR_NOERROR) {
		Alert0("Couldn't open output port" );
		char message[256];
		ALGetErrorText(result, message,256);
		Alert0(message );
		return FALSE;
	}

	CheckFrequency();

	double OldFrequency = frequency;

	FillBuffer();

	int waveindex = 0;
	while (TRUE){

		if (Break){
			AfxEndThread(0);
			delete this;
			return TRUE;
		}

		if (frequency != OldFrequency){
			FillBuffer();
			double OldFrequency = frequency;
			void FillBuffer();
			waveindex = 0;
		}
		for(int i=0; i<FRAME_LENGTH; i++) {   //fill the output frame
			outputFrame[i] = buffer[waveindex];
			if (waveindex>=bufferLength){
				waveindex = 0;
			}
		}
		while(!ALIsFrameFree(outputPort)){  //wait till room to write data
	  		Sleep(1);

		}
//		Alert0("hi");
		ALWriteSamples(outputPort, &outputFrame[0], FRAME_LENGTH);
		Sleep(1);
		Alert0("hi");
	}

	return TRUE;
}
*/

int AudioSineOutThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(AudioSineOutThread, CWinThread)
	//{{AFX_MSG_MAP(AudioSineOutThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AudioSineOutThread message handlers

