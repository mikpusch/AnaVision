#ifndef AUDIOLIB_H
#define AUDIOLIB_H

#define MMNODRV        // Installable driver support
#define MMNOSOUND      // Sound support
#define MMNOMIDI       // MIDI support
#define MMNOAUX        // Auxiliary audio support
#define MMNOMIXER      // Mixer support
#define MMNOTIMER      // Timer support
#define MMNOJOY        // Joystick support
#define MMNOMCI        // MCI support
#define MMNOMMIO       // Multimedia file I/O support
#define MMNOMMSYSTEM   // General MMSYSTEM functions

#include <wtypes.h>
#include <mmsystem.h>

#define ALSYSERR_BASE (WAVERR_LASTERROR + 1)

typedef unsigned int ALRESULT;

#define ALSYSERR_NOERROR 0
#define ALSYSERR_ERROR (ALSYSERR_BASE + 1)
#define ALSYSERR_BAD_SAMPLE_WIDTH (ALSYSERR_BASE + 2)
#define ALSYSERR_BAD_SAMPLE_FORMAT (ALSYSERR_BASE + 3)
#define ALSYSERR_BAD_NUM_CHANNELS (ALSYSERR_BASE + 4)
#define ALSYSERR_BAD_PORT_TYPE (ALSYSERR_BASE + 5)
#define ALSYSERR_BAD_PORT_FORMAT (ALSYSERR_BASE + 6)
#define ALSYSERR_MUST_BE_OUTPUT_PORT (ALSYSERR_BASE + 7)
#define ALSYSERR_MUST_BE_INPUT_PORT (ALSYSERR_BASE + 8)
#define ALSYSERR_BAD_SAMPLING_RATE (ALSYSERR_BASE + 9)

typedef struct _ALPort *ALPort;
typedef struct _ALConfig * ALConfig;



#define AL_PORT_INPUT 1
#define AL_PORT_OUTPUT 2

#define AL_MONO 1
#define AL_STEREO 2

#define AL_SAMPLE_FORMAT_FLOAT 1
#define AL_SAMPLE_FORMAT_SHORT 2

#define AL_SAMPLING_RATE_11025 11025
#define AL_SAMPLING_RATE_22050 22050
#define AL_SAMPLING_RATE_44100 44100

#define AL_SAMPLE_WIDTH_8 8
#define AL_SAMPLE_WIDTH_16 16



ALConfig ALCreateConfig(void);
ALRESULT ALSetSampleWidth(ALConfig config, int width);
ALRESULT ALSetNumQueueFrames(ALConfig config, int numFrames);
ALRESULT ALSetNumChannels(ALConfig config, int numChannels);
ALRESULT ALSetSamplingRate(ALConfig config, int rate);
ALRESULT ALSetFrameLength(ALConfig config, int length);
ALRESULT ALSetSampleFormat(ALConfig config, int type);
ALRESULT ALOpenPort(ALPort * portPtr, ALConfig config, int type);
ALRESULT ALClosePort(ALPort);
ALRESULT ALWriteSamples(ALPort config, void *, int length);
ALRESULT ALReadSamples(ALPort config, void *, int length);
void ALFreeConfig(ALConfig config);
ALRESULT ALGetErrorText(ALRESULT result, char * field, int fieldLength);
int ALIsQueueEmpty(ALPort);
int ALIsFrameFree(ALPort);
int ALIsFrameReady(ALPort);


#endif