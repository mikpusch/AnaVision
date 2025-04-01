#pragma once


// PolluxWnd frame

class GePulseText;
class Text;
class Button;

class PolluxWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(PolluxWnd)
protected:
public:
	PolluxWnd();           // protected constructor used by dynamic creation
	virtual ~PolluxWnd();

	void Shutdown();
	bool Initialize(CWnd * parent = NULL); 
	bool ReInitialize(CWnd * parent = NULL); 

	bool MoveHome();
	bool MoveRelative(double xinmicrom);
	bool MoveAbsolute(double xinmicrom);
	bool SetHome();// sets the home position at the actual position
	void Test();
	void ShowControl();
	void HideControl();
	void ShowHide();
	double GetPosition();
	bool IsInitialized();
	void GetLastPolluxError(CString & c);

private:
	CString LastError;

#ifdef GENERALPURPOSEFLAGDEFINED
	Text * amountWindow;
	Text * PositionWindow;
	Text * StepSizeWindow;
#endif
#ifdef GEPULSEMODELFRAME
	GePulseText * amountWindow;
	GePulseText * PositionWindow;
	GePulseText * StepSizeWindow;
#endif
	Button * Left;
	Button * Right;
	Button * GoHome;
	Button * SetThisHome;

	Button * StepUp;
	Button * StepDown;

	double StepSize;

	CButton * ForwardButton;
	bool Initialized;
	bool	WindowCreated;
	double ActualPosition;
	bool ExecuteCommand(CString command);
	HANDLE hSerial;
	double amount;
	void SetAmount();
	void GetAmount();
	void UpdatePosition();
	bool shown;

protected:
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnReadAmount();
	afx_msg void OnClickLeft();
	afx_msg void OnClickRight();
	afx_msg void OnClickGoHome();
	afx_msg void OnClickSetHome();
	afx_msg void OnClickStepUp();
	afx_msg void OnClickStepDown();
	afx_msg void OnReadStepSize();


};


