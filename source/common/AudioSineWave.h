
#include "../common/AudioSineOutThread.h"


class AudioSineWave{
	// Most member and functions are static because only one sound card is assumed 
public:
	static void Initialze(); // this has to be called once

	static void SetFrequency(double f); // in Hz
	static void Start();		// starts to play a sine wave
	static void Stop();


private:
	static BOOL	StartStopActive;
	static double frequency;
	static BOOL Initialized;
	static BOOL IsPlaying;
	static AudioSineOutThread * pT;
};

