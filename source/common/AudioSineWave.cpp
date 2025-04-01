#include "stdafx.h"
#include "../common/audiolib.h"
#include "../common/AudioSineWave.h"
#include "../common/utils.h"

BOOL					AudioSineWave::Initialized	= false;
BOOL					AudioSineWave::IsPlaying	= false;
double					AudioSineWave::frequency	= 1000; 
AudioSineOutThread *	AudioSineWave::pT			= NULL;
BOOL					AudioSineWave::StartStopActive = false;

CRITICAL_SECTION startstopCriticalSection;

void AudioSineWave::Initialze(){// this has to be called once
	AudioSineOutThread::Initialize();
	Initialized = true;
	InitializeCriticalSection(&startstopCriticalSection);

}

void AudioSineWave::SetFrequency(double f){
	if (AudioSineOutThread::Error) return;
	if (frequency == f) return;
	frequency = f;
	if (!Initialized) return;
	if (!pT) return;
	if (!IsPlaying) return;

	while (StartStopActive){
		Sleep(1);
	}
	EnterCriticalSection(&startstopCriticalSection);
    StartStopActive = true;
    LeaveCriticalSection(&startstopCriticalSection);

	pT->frequency = frequency;
//	ShowFloat(frequency, "f");

	EnterCriticalSection(&startstopCriticalSection);
    StartStopActive = false;
    LeaveCriticalSection(&startstopCriticalSection);


}; 

void AudioSineWave::Start(){		// starts to play a sine wave
	if (!Initialized) return;
	if (IsPlaying) return;
	
//	if (pT) delete pT;
	if (AudioSineOutThread::Error) return;
	
	while (StartStopActive){
		Sleep(1);
	}
	EnterCriticalSection(&startstopCriticalSection);
    StartStopActive = true;
    LeaveCriticalSection(&startstopCriticalSection);

	pT = new AudioSineOutThread();
	pT->m_pThreadParams = NULL;

	AudioSineOutThread::frequency = frequency;
	
		// Create Thread in a suspended state so we can set the Priority 
		// before it starts getting away from us
	if (!pT->CreateThread(CREATE_SUSPENDED)){
		EnterCriticalSection(&startstopCriticalSection);
	    StartStopActive = false;
		LeaveCriticalSection(&startstopCriticalSection);
		return;
	}


		// since everything is successful, add the thread to our list
	VERIFY(pT->SetThreadPriority(THREAD_PRIORITY_NORMAL));
	pT->ResumeThread();
	IsPlaying = true;
	EnterCriticalSection(&startstopCriticalSection);
    StartStopActive = false;
	LeaveCriticalSection(&startstopCriticalSection);
//	Alert0("is playing");
	
}

void AudioSineWave::Stop(){
	if (!Initialized){
		//Alert0("not initialized");
		return;
	}
	if (AudioSineOutThread::Error) return;
	if (!IsPlaying){
//		Alert0("not playing");
		return;
	}
	if (!pT){
//		Alert0("not pT");
		IsPlaying = false;
		return;
	}
//	Alert0("Set break");
	while (StartStopActive){
		Sleep(1);
	}
	EnterCriticalSection(&startstopCriticalSection);
    StartStopActive = true;
    LeaveCriticalSection(&startstopCriticalSection);

	AudioSineOutThread::Break = true;
	pT=NULL;
	IsPlaying = false;
	EnterCriticalSection(&startstopCriticalSection);
    StartStopActive = false;
    LeaveCriticalSection(&startstopCriticalSection);

}

