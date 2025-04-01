#if !defined(AFX_AUDIOSINEOUTTHREAD_H__566B807E_D0BF_4BB2_8C8F_6A9AD7D59F17__INCLUDED_)
#define AFX_AUDIOSINEOUTTHREAD_H__566B807E_D0BF_4BB2_8C8F_6A9AD7D59F17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AudioSineOutThread.h : header file
//


//#include "../common/audiolib.h"
#include <mmsystem.h>

#define BLOCK_SIZE 2048 //4096 //(4*4096) //8192
#define BLOCK_COUNT 4 //40 //5 //20


/////////////////////////////////////////////////////////////////////////////
// AudioSineOutThread thread

class AudioSineOutThread : public CWinThread
{
	DECLARE_DYNCREATE(AudioSineOutThread)
public:
	AudioSineOutThread();           // protected constructor used by dynamic creation

// Operations
public:

	static double frequency;
	static bool	Error;
	static double x;
	static double deltax;
	static volatile bool Break;

public:
	static void Initialize();
	static void ShutDown();

	static void CALLBACK waveOutProc(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
	static WAVEHDR* allocateBlocks(int size, int count);
	static void freeBlocks(WAVEHDR* blockArray);
	static void writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size);
/*
* module level variables
*/
	//static	CRITICAL_SECTION waveCriticalSection;

	static WAVEHDR* waveBlocks;
	static volatile int waveFreeBlockCount;
	static int waveCurrentBlock;

private:
	static bool Initialized;


/*
	ALConfig AL;

	ALRESULT result;
	ALPort outputPort;

*/
	HWAVEOUT hWaveOut; /* device handle */
    WAVEFORMATEX wfx; /* look this up in your documentation */
    MMRESULT result;


	void CheckFrequency();
	void FillBuffer();

	int bufferLength;

	__int16 buffer[44102];

//	#define FRAME_LENGTH 4096
	float outputFrame[BLOCK_SIZE];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AudioSineOutThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~AudioSineOutThread();

	// Generated message map functions
	//{{AFX_MSG(AudioSineOutThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOSINEOUTTHREAD_H__566B807E_D0BF_4BB2_8C8F_6A9AD7D59F17__INCLUDED_)
