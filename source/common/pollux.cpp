
#include "stdafx.h"
//#include "pollux.h"

#ifdef GENERALPURPOSEFLAGDEFINED
#include "../GeneralPurpose/GeneralPurpose/GeneralPurpose.h"
#endif

bool					Pollux::Initialized				= 0;
double					Pollux::ActualPosition			= 0;
CWnd *					Pollux::CtrlWnd					= 0;
HANDLE					Pollux::hSerial					= 0;
CEdit *					Pollux::amountWindow			= 0;
CButton *				Pollux::ForwardButton			= 0;
double					Pollux::amount					= 100.0; // micron




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
}

bool Pollux::Initialize(CWnd * parent){
	if (Initialized){
		return true;
	}

	if (parent){
		CtrlWnd = new CWnd();
		CRect rect;
		rect.left=50;
		rect.right=rect.left+200;
		rect.top=50;
		rect.bottom=rect.top+200;
		DWORD dwStyle= WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_CHILD |WS_DLGFRAME; // |WS_OVERLAPPEDWINDOW |WS_SYSMENU;
		CtrlWnd->Create(NULL, "Pollux", dwStyle, rect, parent, 1234);

		DWORD style = ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER;
		amountWindow = new CEdit();
		amountWindow->Create(style, CRect(0,0,100,50), CtrlWnd, 12345);
		SetAmount();
	}
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
	char s[20];
	double xm= xinmicron/1000.0;
	_gcvt(xm, 3, s);
	CString c=CString(s)+CString(" 1 nr ");
	return ExecuteCommand(c);

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


void Pollux::ShowControl(){
	if (CtrlWnd){
		CtrlWnd->ShowWindow(SW_SHOW);
	}
}
void Pollux::HideControl(){
	if (CtrlWnd){
		CtrlWnd->ShowWindow(SW_HIDE);
	}
}

void Pollux::SetAmount(){
	if (!CtrlWnd) return;
	char s[20];
	_gcvt(amount, 5, s);
	amountWindow->SetWindowText(CString(s));
}
void Pollux::GetAmount(){
	if (!CtrlWnd) return;
	CString c;
	amountWindow->GetWindowText(c);
	amount = atof(c);

}

void Pollux::OnEndFocusAmount(){
	GetAmount();
	SetAmount();
}