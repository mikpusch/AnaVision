// Implements the driver for the 

#ifndef POLLUX_MIK
#define POLLUX_MIK



class Pollux{
public:

	Pollux();
//	DECLARE_DYNCREATE(Pollux)

	~Pollux();
	static void Shutdown();
	static bool Initialize(CWnd * parent = NULL); 

	bool MoveRelative(double xinmicrom);
	bool MoveAbsolute(double xinmicrom);
	void SetHome();// sets the home position at the actual position
	void Test();
	void ShowControl();
	void HideControl();

	afx_msg	void OnEndFocusAmount();


private:

	static CWnd * CtrlWnd;
	static CEdit * amountWindow;
	static CButton * ForwardButton;
	static bool Initialized;
	static bool WindowCreated;
	static double ActualPosition;
	static bool ExecuteCommand(CString command);
	static HANDLE hSerial;
	static double amount;
	static void SetAmount();
	static void GetAmount();

};

#endif