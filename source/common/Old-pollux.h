// Implements the driver for the 

#ifndef POLLUX_MIK
#define POLLUX_MIK

typedef DWORD (__stdcall *pInitController)(DWORD CMode,DWORD Axes,DWORD Port, DWORD Baudrate,HWND UserWin,UINT UserMsg,DWORD Mode);
typedef DWORD (__stdcall *pOpenController)();
typedef DWORD (__stdcall *pCloseController)();
typedef DWORD (__stdcall *pExecuteCommand)(LPSTR pCommand, DWORD Lines, LPSTR pData);
typedef DWORD (__stdcall *pGetReply)(LPSTR pData);
typedef DWORD (__stdcall *pIdentify)(LPSTR pData);
typedef DWORD (__stdcall *pGetStatus)(LPDWORD pData);
typedef DWORD (__stdcall *pGetError)(LPDWORD pData);
typedef DWORD (__stdcall *pSetVelocity)(LPSTR pData);
typedef DWORD (__stdcall *pGetVelocity)(LPSTR pData);
typedef DWORD (__stdcall *pJoystickEnable)();
typedef DWORD (__stdcall *pSetOrigin)();
typedef DWORD (__stdcall *pGetPos)(LPSTR Axis1Pos,LPSTR Axis2Pos,LPSTR Axis3Pos,LPSTR Axis4Pos);
typedef DWORD (__stdcall *pMoveAbsolute)(LPSTR Axis1Pos,LPSTR Axis2Pos,LPSTR Axis3Pos,LPSTR Axis4Pos);
typedef DWORD (__stdcall *pMoveRelativeAutoReply)(LPSTR Axis1Pos,LPSTR Axis2Pos,LPSTR Axis3Pos,LPSTR Axis4Pos);
typedef DWORD (__stdcall *pMoveRelativeA)(LPSTR Axis1Pos,DWORD Axis);


#define		WP2_CONTROLLER_DEFAULT	1
#define		WP2_CONTROLLER_DELTA	2
#define		WP2_CONTROLLER_ALPHA	4
#define		WP2_CONTROLLER_TAURUS	8
#define		WP2_CONTROLLER_PEGASUS	16
#define		WP2_CONTROLLER_ORION	32
#define		WP2_CONTROLLER_CORVUS	64
#define		WP2_CONTROLLER_POLLUX	128

#define		WMU_ASYNCMSG		WM_USER+10



class Pollux{
public:

	Pollux();
	~Pollux();
	static void Shutdown();

	static bool Initialize(CWnd * parent); 
	static bool InitializeOld(); 

	bool MoveRelative(double xinmicrom);
	bool MoveAbsolute(double xinmicrom);
	void SetHome();// sets the home position at the actual position
	void Test();
	void ShowControl();
	void HideControl();


private:

	static CWnd * CtrlWnd;

	static pInitController		Wp2CommInitController;
	static pOpenController		Wp2CommOpenController;
	static pCloseController		Wp2CommCloseController;
	static pIdentify			Wp2CommIdentify;
	static pGetStatus			Wp2CommGetStatus;
	static pGetError			Wp2CommGetError;
	static pSetVelocity			Wp2CommSetVelocity;
	static pGetVelocity			Wp2CommGetVelocity;
	static pJoystickEnable		Wp2CommJoystickEnable;
	static pSetOrigin			Wp2CommSetOrigin;
	static pMoveAbsolute		Wp2CommMoveAbsolute;
	static pMoveRelativeAutoReply		Wp2CommMoveRelativeAutoReply;
	static pGetPos				Wp2CommGetPos;
	static pMoveRelativeA		Wp2MoveRelativeA;
	static pExecuteCommand		Wp2CommExecuteCommand;


	static HINSTANCE	hWp2CommDll;
	void		SetupComboBoxBaudrate(int nMode);

	static bool Initialized;
	static double ActualPosition;
	static bool ExecuteCommand(CString command);


static HANDLE hSerial;
//COMMTIMEOUTS timeouts;
//COMMCONFIG dcbSerialParams;
char *words, *buffRead, *buffWrite;
DWORD dwBytesWritten, dwBytesRead;




};

#endif