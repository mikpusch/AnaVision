// PolluxWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PolluxWnd.h"
#include <stdio.h>

#ifdef GENERALPURPOSEFLAGDEFINED
#include "../GeneralPurpose/GeneralPurpose/GeneralPurpose.h"
#endif

#if defined GEPULSESTIM
	#include "../GePulse/GePulse.h"
#endif


IMPLEMENT_DYNCREATE(PolluxWnd, CFrameWnd)

PolluxWnd::PolluxWnd()
{
	Initialized = false;
	ActualPosition = 0.0;
	amount = 100.0;
	WindowCreated = false;
	shown = false;
	StepSize = 50;
}

PolluxWnd::~PolluxWnd()
{
}


BEGIN_MESSAGE_MAP(PolluxWnd, CFrameWnd)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS( 12345, OnReadAmount)
	ON_BN_CLICKED( 12346, OnClickLeft)
	ON_BN_CLICKED( 12347, OnClickRight)
	ON_BN_CLICKED( 12348, OnClickGoHome)
	ON_BN_CLICKED( 12349, OnClickSetHome)
	ON_BN_CLICKED(12350, OnClickStepUp)
	ON_BN_CLICKED(12351, OnClickStepDown)
	ON_EN_KILLFOCUS(12352, OnReadStepSize)

END_MESSAGE_MAP()


// PolluxWnd message handlers
void PolluxWnd::Shutdown(){
	if (!Initialized){
		return;
	}
	CloseHandle(hSerial);
	hSerial = 0;
	Initialized = false;
}
void PolluxWnd::GetLastPolluxError(CString & c){
	c = LastError;
}

double PolluxWnd::GetPosition(){
	return ActualPosition;
}
bool PolluxWnd::IsInitialized(){
	return Initialized;
}
bool PolluxWnd::ReInitialize(CWnd * parent){
	if (WindowCreated){
		parent = NULL;
	}
	if (Initialized){
		Shutdown();
	}
	return Initialize(parent);
}

bool PolluxWnd::Initialize(CWnd * parent){
	if (Initialized){
		return true;
	}

	if (parent){
		WindowCreated = true;
//		CtrlWnd = new CWnd();
		CRect rect;
		rect.left=50;
		rect.right=rect.left+145; //135;
		rect.top=50;
		rect.bottom=rect.top+165;
		CMainFrame* frame = (CMainFrame *)(AfxGetMainWnd( ));

//		DWORD dwStyle= WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_CHILD |WS_DLGFRAME; // | WS_OVERLAPPEDWINDOW |WS_SYSMENU;
		DWORD dwStyle= WS_OVERLAPPEDWINDOW ; //|WS_SYSMENU;
		this->Create(NULL, "Pollux", dwStyle, rect, parent);
//		this->Create(NULL, "Pollux", dwStyle, rect, frame);

//		DWORD style = ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE | WS_CHILD | WS_BORDER;

		rect.left=3;
		rect.right=rect.left+70;
		rect.top=2;
		rect.bottom=rect.top+30;

		#ifdef GENERALPURPOSEFLAGDEFINED
			PositionWindow = new Text(12344, 0.0, -10000.0, 10000.0, false, rect, this);
		#endif
		#ifdef GEPULSEMODELFRAME
			PositionWindow = new GePulseText(12344, 0.0, -10000.0, 10000.0, false, rect, this);
		#endif

		rect.top=rect.bottom+2;
		rect.bottom=rect.top+30;

		#ifdef GENERALPURPOSEFLAGDEFINED
			amountWindow = new Text(12345, amount, -1000.0, 1000.0, true, rect, this);
		#endif
		#ifdef GEPULSEMODELFRAME
			amountWindow = new GePulseText(12345, amount, -1000.0, 1000.0, true, rect, this);
		#endif
//		amountWindow->Create(style, CRect(0,0,100,50), this, 12345);
		amountWindow->SetFloat(amount);
		SetAmount();
		UpdatePosition();
		CRect rect2=rect;
		rect2.left=rect.right+1;
		rect2.right=rect2.left+20;
		rect2.bottom = rect2.top+(rect.bottom-rect.top)/2;
		StepUp = new Button(12350, false, "^", rect2, this);

		rect2.top = rect2.bottom;
		rect2.bottom = rect2.top+(rect.bottom-rect.top)/2;
		StepUp = new Button(12351, false, "v", rect2, this);

		rect2.top = rect.top;
		rect2.bottom = rect.bottom;
		rect2.left=rect2.right;
		rect2.right=rect2.left+50;

		#ifdef GENERALPURPOSEFLAGDEFINED
			StepSizeWindow = new Text(12352, StepSize, -1000.0, 1000.0, true, rect2, this);
		#endif
		#ifdef GEPULSEMODELFRAME
			StepSizeWindow = new GePulseText(12352, StepSize, -1000.0, 1000.0, true, rect2, this);
		#endif


		rect.right=rect.left+53;
		rect.top=rect.bottom+2;
		rect.bottom=rect.top+20;

		Left = new Button(12346, false, "<-", rect, this);

		rect.left = rect.right+2;
		rect.right=rect.left+53;
		Right = new Button(12347, false, "->", rect, this);

		rect.left = 3;
		rect.right=rect.left+120;
		rect.top=rect.bottom+2;
		rect.bottom=rect.top+20;
		GoHome = new Button(12348, false, "Go Home", rect, this);

		rect.top=rect.bottom+2;
		rect.bottom=rect.top+20;
		SetThisHome = new Button(12349, false, "Set pos. as home", rect, this);

		this->ShowWindow(SW_SHOW);
		//SetAmount();
		shown = true;
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
				LastError = "serial port does not exist";
//				Alert0(" serial port does not exist \n");
		}
		else{
			LastError = " some other error occured. Inform user.";
//			Alert0(" some other error occured. Inform user.\n");
		}
		return false;
	}

	COMMCONFIG dcbSerialParams;

	if (!GetCommState(hSerial, &dcbSerialParams.dcb)) {
		LastError = "error getting state ";
//		Alert0("error getting state \n");
		return false;
	}
	dcbSerialParams.dcb.DCBlength = sizeof(dcbSerialParams.dcb);


	dcbSerialParams.dcb.BaudRate = CBR_19200;
	dcbSerialParams.dcb.ByteSize = 8;
	dcbSerialParams.dcb.StopBits = TWOSTOPBITS;
	dcbSerialParams.dcb.Parity = NOPARITY;

	dcbSerialParams.dcb.fBinary = true;
	dcbSerialParams.dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcbSerialParams.dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcbSerialParams.dcb.fOutxCtsFlow = false;
	dcbSerialParams.dcb.fOutxDsrFlow = false;
	dcbSerialParams.dcb.fDsrSensitivity= false;
	dcbSerialParams.dcb.fAbortOnError = true;

	if (!SetCommState(hSerial, &dcbSerialParams.dcb)) {
		LastError = " error setting serial port state ";
//		Alert0(" error setting serial port state \n");
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
		LastError = "error setting port state ";
//		Alert0("error setting port state \n");
		return false;
	}
	Initialized = true;


	if (!ExecuteCommand("0.4 1 snv ")){
	//	LastError = "Could not set velocity";
//		Alert0("Could not set velocity");
		Initialized = false;
		return false;
	}

	if (!ExecuteCommand("10.0 1 sna ")){
	//	LastError = "Could not set accell.";
//		Alert0("Could not set accell.");
		Initialized = false;
		return false;
	}

	Initialized = true;
	return true;
}



bool PolluxWnd::ExecuteCommand(CString command){
	if (!Initialized){
		LastError="Not initialized: could not execute "+command;
//		Alert0("Not init");
		return false;
	}
	char cmd[256];
	int len = command.GetLength();
	if (len>=255){
		LastError = "Command too long:" + command;
//		Alert0("Command too long");
		return false;
	}
	for (int i=0; i<len; i++){
		cmd[i]=command.GetAt(i);
	}
	cmd[len]=0;
	DWORD nwrite = len;
//	Alert0(CString(cmd));
	DWORD dwBytesWritten;
	if (!WriteFile(hSerial, &cmd[0], nwrite, &dwBytesWritten, NULL)) { 
		LastError = "error writing to output buffer ";
//		Alert0("error writing to output buffer \n");
		return false;
	}

	return true;
}
bool PolluxWnd::MoveRelative(double xinmicron){
	if (!Initialized){
		LastError = "Not init in Move Rel";
//		Alert0("Not init");
		return false;
	}
//	char s[20];
	double xm= xinmicron/1000.0;
//	_gcvt(xm, 7, s);
///	sprintf(s, "%f", xm);
	CString cc;
	ConvertDoubleToFixPoint(xm , cc);

	CString c=cc+CString(" 1 nr ");
	bool result = ExecuteCommand(c);
	if (result){
		ActualPosition += xinmicron;
	}
	UpdatePosition();
	return result;

}
bool PolluxWnd::MoveAbsolute(double xinmicrom){
	if (MoveRelative(xinmicrom-ActualPosition)){
		ActualPosition = xinmicrom;
		return true;
	}
	else{
		return false;
	}
}
bool PolluxWnd::SetHome(){
	if (!Initialized){
		return false;
	}
	ActualPosition = 0.0;
	UpdatePosition();
	return true;
}

bool PolluxWnd::MoveHome(){
	return MoveAbsolute(0.0);
}

void PolluxWnd::ShowHide(){
	if (shown){
		HideControl();
	}
	else{
		ShowControl();
	}
}

void PolluxWnd::ShowControl(){
	if (WindowCreated){
		this->ShowWindow(SW_SHOW);
		shown = true;
	}
}
void PolluxWnd::HideControl(){
	if (WindowCreated){
		this->ShowWindow(SW_HIDE);
		shown = false;
	}
}

void PolluxWnd::SetAmount(){
	if (!WindowCreated) return;
	char s[20];
	_gcvt(amount, 5, s);
	amountWindow->SetWindowText(CString(s));
}
void PolluxWnd::GetAmount(){
	if (!WindowCreated) return;
	CString c;
	amountWindow->GetWindowText(c);
	amount = atof(c);

}


void PolluxWnd::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	HideControl();
//	CFrameWnd::OnClose();
}

void PolluxWnd::OnReadAmount(){
	GetAmount();
	SetAmount();
}
void PolluxWnd::OnClickLeft(){
	MoveRelative(-amount);
//	ShowFloat(amount, "Left");
}
void PolluxWnd::OnClickRight(){
	MoveRelative(amount);
//	ShowFloat(amount, "Right");
}
void PolluxWnd::OnClickGoHome(){
	MoveHome();
//	Alert0("GoHome");
}
void PolluxWnd::OnClickSetHome(){
	SetHome();
//	Alert0("SetHome");
}
void PolluxWnd::UpdatePosition(){
	if (WindowCreated){
		PositionWindow->SetFloat(ActualPosition);
	}

}
void PolluxWnd::OnClickStepUp(){
	amount += StepSize;
	SetAmount();
}
void PolluxWnd::OnClickStepDown(){
	amount -= StepSize;
	SetAmount();
}
void PolluxWnd::OnReadStepSize(){
	if (!WindowCreated) return;
	CString c;
	StepSizeWindow->GetWindowText(c);
	StepSize = atof(c);
}
