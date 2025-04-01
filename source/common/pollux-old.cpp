
#include "stdafx.h"
//#include "pollux.h"

#ifdef GENERALPURPOSEFLAGDEFINED
#include "../GeneralPurpose/GeneralPurpose/GeneralPurpose.h"
#endif

HINSTANCE				Pollux::hWp2CommDll				= 0;
bool					Pollux::Initialized				= 0;
double					Pollux::ActualPosition			= 0;
CWnd *					Pollux::CtrlWnd					= 0;

pInitController			Pollux::Wp2CommInitController	= 0;
pOpenController			Pollux::Wp2CommOpenController	= 0;
pCloseController		Pollux::Wp2CommCloseController	= 0;
pIdentify				Pollux::Wp2CommIdentify			= 0;
pGetStatus				Pollux::Wp2CommGetStatus		= 0;
pGetError				Pollux::Wp2CommGetError			= 0;
pSetVelocity			Pollux::Wp2CommSetVelocity		= 0;
pGetVelocity			Pollux::Wp2CommGetVelocity		= 0;
pJoystickEnable			Pollux::Wp2CommJoystickEnable	= 0;
pSetOrigin				Pollux::Wp2CommSetOrigin		= 0;
pMoveAbsolute			Pollux::Wp2CommMoveAbsolute		= 0;
pMoveRelativeAutoReply	Pollux::Wp2CommMoveRelativeAutoReply = 0;
pGetPos					Pollux::Wp2CommGetPos			= 0;
pMoveRelativeA			Pollux::Wp2MoveRelativeA		= 0;
pExecuteCommand			Pollux::Wp2CommExecuteCommand	= 0;

HANDLE					Pollux::hSerial					= 0;

Pollux::Pollux(){
	Initialized = false;
	ActualPosition = 0.0;
}
Pollux::~Pollux(){
}
void Pollux::Shutdown(){
	if (!Initialized){
		return;
	}
	CloseHandle(hSerial);
	Initialized = false;
/*
DWORD dwSuccess=(*Wp2CommCloseController)();
	if (dwSuccess)
	{
		AfxMessageBox("Unable to close controller!");
		return;
	}
*/
}
bool Pollux::Initialize(CWnd * parent){
	if (Initialized){
		return true;
	}

	CtrlWnd = new CWnd();
	CRect rect;
	rect.left=0;
	rect.right=rect.left+100;
	rect.top=0;
	rect.bottom=rect.top+200;
	DWORD dwStyle= WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_CHILD |WS_DLGFRAME |WS_OVERLAPPEDWINDOW |WS_SYSMENU;

	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd());

//	CtrlWnd->Create(NULL, "PolluxCtrl", dwStyle, rect, NULL, 0, NULL);
//	CtrlWnd->Create(NULL, "Hi", WS_VISIBLE, CRect(0, 0, 20, 20), frame, 1234);
	
	CtrlWnd->Create(NULL, "Hi", dwStyle, CRect(100, 100, 200, 200), parent, 1234);
//	CtrlWnd->Create(NULL, "Hi", WS_VISIBLE, CRect(0, 0, 200, 200), frame, 1234);
//	CtrlWnd->ShowWindow(SW_SHOW);

	hSerial = CreateFile("COM1",
	GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,//FILE_FLAG_OVERLAPPED, // FILE_ATTRIBUTE_NORMAL
			NULL);

	if ( hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)	{
				Alert0(" serial port does not exist \n");
		}
		else{
			Alert0(" some other error occured. Inform user.\n");
		}
		return false;
	}

	COMMCONFIG dcbSerialParams;

	if (!GetCommState(hSerial, &dcbSerialParams.dcb)) {
		Alert0("error getting state \n");
		return false;
	}
	dcbSerialParams.dcb.DCBlength = sizeof(dcbSerialParams.dcb);


	dcbSerialParams.dcb.BaudRate = CBR_19200;
	dcbSerialParams.dcb.ByteSize = 8;
	dcbSerialParams.dcb.StopBits = TWOSTOPBITS;
	dcbSerialParams.dcb.Parity = NOPARITY;

	dcbSerialParams.dcb.fBinary = TRUE;
	dcbSerialParams.dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcbSerialParams.dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcbSerialParams.dcb.fOutxCtsFlow = FALSE;
	dcbSerialParams.dcb.fOutxDsrFlow = FALSE;
	dcbSerialParams.dcb.fDsrSensitivity= FALSE;
	dcbSerialParams.dcb.fAbortOnError = TRUE;

	if (!SetCommState(hSerial, &dcbSerialParams.dcb)) {
		Alert0(" error setting serial port state \n");
		return false;
	}

	COMMTIMEOUTS timeouts;
	GetCommTimeouts(hSerial,&timeouts);
//COMMTIMEOUTS timeouts = {0};

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier= 10;

	if(!SetCommTimeouts(hSerial, &timeouts)) {
		Alert0("error setting port state \n");
		return false;
	}
	Initialized = true;


	if (!ExecuteCommand("0.4 1 snv ")){
		Alert0("Could not set velocity");
		Initialized = false;
		return false;
	}

	if (!ExecuteCommand("10.0 1 sna ")){
		Alert0("Could not set accell.");
		Initialized = false;
		return false;
	}

	Initialized = true;
	return true;
}



bool Pollux::ExecuteCommand(CString command){
	if (!Initialized){
		Alert0("Not init");
		return false;
	}
	char cmd[256];
	int len = command.GetLength();
	if (len>=255){
		Alert0("Command too long");
		return false;
	}
	for (int i=0; i<len; i++){
		cmd[i]=command.GetAt(i);
	}
	cmd[len]=0;
	DWORD nwrite = len;
	Alert0(CString(cmd));
	DWORD dwBytesWritten;
	if (!WriteFile(hSerial, &cmd[0], nwrite, &dwBytesWritten, NULL)) { 
		Alert0("error writing to output buffer \n");
		return false;
	}

	return true;
}
bool Pollux::MoveRelative(double xinmicron){
	if (!Initialized){
		Alert0("Not init");
		return false;
	}
//	char words[256];
	char s[20];
	double xm= xinmicron/1000.0;
	_gcvt(xm, 3, s);
	CString c=CString(s)+CString(" 1 nr ");
	Alert0(c);
//	return true;
//	return ExecuteCommand("-0.1 1 nr ");
	return ExecuteCommand(c);

	char * words = "-0.1 1 nr ";
	DWORD nwrite = strlen(words);

	char * buffWrite = words;
	DWORD dwBytesWritten = 0;

	if (!WriteFile(hSerial, buffWrite, nwrite, &dwBytesWritten, NULL)) { 
		Alert0("error writing to output buffer \n");
		return false;
	}
	return true;
//printf("Data written to write buffer is \n %s \n",buffWrite);
	char buffRead[256];
	buffRead[0] = 0;
	DWORD dwBytesRead = 0;
	DWORD nread = 256;

	if (!ReadFile(hSerial, buffRead, nread, &dwBytesRead, NULL)) {
		printf("error reading from input buffer \n");
		return false;
	}
	Alert0(buffRead);




	return true;
	double xinmm=xinmicron/1000.0;
	char Data0[256];
//	sprintf(Data0,"%f",xinmm);
//	Alert0(Data0);

//	DWORD dwSuccess=(*Wp2MoveRelativeA)(Data0, 0);
//	DWORD dwSuccess=(*Wp2CommExecuteCommand)("1.0 1 nr ",1,Data0);
	DWORD dwSuccess=(*Wp2CommExecuteCommand)("1 nversion ",1,Data0);
	if (dwSuccess)
	{
		AfxMessageBox("Unable to process Wp2MoveRelativeA!");
		return false;
	}
	//Alert0(CString(Data0));
	ActualPosition += xinmicron;
	return true;
}
bool Pollux::MoveAbsolute(double xinmicrom){
	if (MoveRelative(xinmicrom-ActualPosition)){
		ActualPosition = xinmicrom;
		return true;
	}
	else{
		return false;
	}
}
void Pollux::SetHome(){
	ActualPosition = 0.0;
}


bool Pollux::InitializeOld(){
	if (Initialized){
		return true;
	}


	CMainFrame * frame = (CMainFrame *)(AfxGetMainWnd());

	DWORD	dwSuccess;
	DWORD	dwBaudrate;
	int					nPort;
//	char			acBuf[256];


	// TODO: Add your control notification handler code here

	hWp2CommDll=LoadLibrary("WP2COMM.DLL");

	if (hWp2CommDll==NULL)
	{
		AfxMessageBox("Unable to load WP2COMM.DLL");
		return false;
	}
	Wp2CommInitController=(pInitController)GetProcAddress(hWp2CommDll,"InitController");

	if (Wp2CommInitController==0)
	{
		AfxMessageBox("GetProcAddress-Error (InitController)");
		return false;
	}
	DWORD		dwAxes;
	DWORD		dwControllerMode;

	dwControllerMode=WP2_CONTROLLER_POLLUX;
	dwAxes=1;
	dwBaudrate = 19200;
	nPort=1; //m_ComboPort.GetCurSel()+1;
//	dwSuccess=(*Wp2CommInitController)(dwControllerMode,dwAxes,nPort,dwBaudrate,frame->GetSafeHwnd(),WMU_ASYNCMSG,1050884);
//	dwSuccess=(*Wp2CommInitController)(dwControllerMode,dwAxes,nPort,dwBaudrate,NULL,WMU_ASYNCMSG,1050884);
	dwSuccess=(*Wp2CommInitController)(dwControllerMode,dwAxes,nPort,dwBaudrate,NULL,0,1050884);
	if (dwSuccess)
	{
		AfxMessageBox("Unable to initialize controller!");
		return false;
	}
	Wp2CommOpenController=(pOpenController)GetProcAddress(hWp2CommDll,"OpenController");
	if (Wp2CommOpenController==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommOpenController)");
		return false;
	}
	dwSuccess=(*Wp2CommOpenController)();

	if (dwSuccess)
	{
		AfxMessageBox("Unable to open controller!");
		return false;
	}

	Wp2CommCloseController=(pCloseController)GetProcAddress(hWp2CommDll,"CloseController");
	if (Wp2CommCloseController==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommCloseController)");
		return false;
	}
	Wp2CommIdentify=(pIdentify)GetProcAddress(hWp2CommDll,"Identify");
	if (Wp2CommIdentify==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommIdentify)");
		return false;
	}
	Wp2CommGetStatus=(pGetStatus)GetProcAddress(hWp2CommDll,"GetStatus");
	if (Wp2CommGetStatus==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommGetStatus)");
		return false;
	}
	Wp2CommGetError=(pGetError)GetProcAddress(hWp2CommDll,"GetError");
	if (Wp2CommGetError==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommGetError)");
		return false;
	}
	Wp2CommSetVelocity=(pSetVelocity)GetProcAddress(hWp2CommDll,"SetVelocity");
	if (Wp2CommSetVelocity==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommSetVelocity)");
		return false;
	}
	Wp2CommGetVelocity=(pGetVelocity)GetProcAddress(hWp2CommDll,"GetVelocity");
	if (Wp2CommGetVelocity==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommGetVelocity)");
		return false;
	}
	Wp2CommSetOrigin=(pSetOrigin)GetProcAddress(hWp2CommDll,"SetOrigin");
	if (Wp2CommSetOrigin==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommSetOrigin)");
		return false;
	}
	Wp2CommMoveAbsolute=(pMoveAbsolute)GetProcAddress(hWp2CommDll,"MoveAbsolute");
	if (Wp2CommMoveAbsolute==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommMoveAbsolute)");
		return false;
	}
	Wp2CommGetPos=(pGetPos)GetProcAddress(hWp2CommDll,"GetPos");
	if (Wp2CommGetPos==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommGetPos)");
		return false;
	}
	Wp2CommMoveRelativeAutoReply=(pMoveRelativeAutoReply)GetProcAddress(hWp2CommDll,"MoveRelativeAutoReply");
	if (Wp2CommMoveRelativeAutoReply==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommMoveRelativeAutoReply)");
		return false;
	}

	Wp2MoveRelativeA = (pMoveRelativeA)GetProcAddress(hWp2CommDll,"MoveRelativeA");
	if (Wp2MoveRelativeA==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2MoveRelativeA)");
		return false;
	}

	Wp2CommExecuteCommand=(pExecuteCommand)GetProcAddress(hWp2CommDll,"ExecuteCommand");
	if (Wp2CommExecuteCommand==0)
	{
		AfxMessageBox("GetProcAddress-Error (Wp2CommExecuteCommand)");
		return false;
	}

	char				Data0[256];
//	dwSuccess=(*Wp2CommExecuteCommand)("10 1 setnvel\r\n",1,Data0);
	dwSuccess=(*Wp2CommExecuteCommand)("1 nversion ",1,Data0);
	if (dwSuccess)
	{
		AfxMessageBox("Error executing command");
		return false;
	}
	Alert0(Data0);

/*
dwSuccess=(*Wp2CommSetVelocity)("10");
	if (dwSuccess)
	{
		AfxMessageBox("Unable to process SetVelocity!");
		return false;
	}

*/

	Initialized = true;
	if (Initialized){
		Alert0("Hi");
	}
	return true;
}


	void ShowControl();
	void HideControl();