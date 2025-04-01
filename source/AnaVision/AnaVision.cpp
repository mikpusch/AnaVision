// AnaVision.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AnaVision.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionApp

BEGIN_MESSAGE_MAP(CAnaVisionApp, CWinApp)
	//{{AFX_MSG_MAP(CAnaVisionApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_NEW, MyOnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, OnMyFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionApp construction

CAnaVisionApp::CAnaVisionApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

#if defined SIMPLE_IMAGE_CORE
	Core = arivis::simpleimagecore::CreateImageCore("PMKR2EW4ZSD9UERMG5QG6YHUF");

	if (!Core){
		Alert0("could not create core");
	}
#endif

}
void CAnaVisionApp::MySaveString(int & byteswritten, CFile * fp, CString & value){
	int l=value.GetLength();
//	int byteswritten = 0;
	MyWriteVariable(l);
	if (l<1){
//		return byteswritten;
		return ;
	}
	TCHAR t;
	for (int i=0; i<l; i++){
		t=value.GetAt( i ) ;
		MyWriteVariable(t);
	}
//	return byteswritten;
}
bool CAnaVisionApp::MyLoadString(int & bytesavailable, CFile * fp, CString & value){
	int l;
	MyReadVariable(l);
	if (l>0){
	    value=CString(" ", l);
	}
	else{
		value = CString("");
		l=0;
		return true;
	}
	TCHAR t;
	for (int i=0; i<l; i++){
		if (fp->Read(&t, sizeof(TCHAR))!=sizeof(TCHAR)){
			return false;
		}
		bytesavailable -= sizeof(TCHAR);
		value.SetAt(i, t);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAnaVisionApp object

CAnaVisionApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAnaVisionApp initialization

BOOL CAnaVisionApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(9);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_ANAVISTYPE,
		RUNTIME_CLASS(CAnaVisionDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAnaVisionView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return false;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return false;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAnaVisionApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CAnaVisionApp::OnMyFileOpen(){
	CWinApp::OnFileOpen();
}

CDocument* CAnaVisionApp::OpenDocumentFile( LPCTSTR lpszFileName ){
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		bool DeleteThis = false;

		// Get the active MDI child window.
		CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
		if (pChild){
//			Beep(1000,20);
			CView* pV = pChild->GetActiveView( );
			if (pV){
//				Beep(2000,20);
				CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
				if (pDoc){
//					Beep(500,20);
					p = pDoc->dat.p;
				}
			}
			else {
//				Alert0("no view");
			}
		}
	
		{ // Test Tif
			bool IsTiffExtension = false;
			CString Name = lpszFileName;
			int l=Name.GetLength();
			if (l>4){
				CString Ext = Name.Right(3);
				CString EXT = Ext;
				EXT.MakeUpper();
				if (EXT.Compare("TIF") == 0){
					IsTiffExtension = true;
				}
				else{
					CString Ext = Name.Right(4);
					CString EXT = Ext;
					EXT.MakeUpper();
					if (EXT.Compare("TIFF") == 0){
						IsTiffExtension = true;
					}
				}
			}
			if (IsTiffExtension){
				OnFileNew();
				//CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
				CChildFrame  *pChild = (CChildFrame *) pFrame->GetActiveFrame();

				if (pChild){
		//			Beep(1000,20);
					pChild->DoOpenTif(Name);
					DeleteThis = true;
					
//					return NULL;
				}

			}
	}

//	Alert0(c);

	if (DeleteThis){
		return NULL;
//		return CWinApp::OpenDocumentFile( NULL );
	}
	return CWinApp::OpenDocumentFile( lpszFileName );
}

ImageDataType ** ImageData::AllocOneImage(int xsize, int ysize){
	ImageDataType ** id;
	#ifdef ImageDataTypeDouble
		id = dmatrix(0, xsize-1, 0, ysize-1);
	#endif
	#ifdef ImageDataTypeFloat
		id = matrix(0, xsize-1, 0, ysize-1);
	#endif
	#ifdef ImageDataTypeInt
		id = imatrix(0, xsize-1, 0, ysize-1);
	#endif
	return id;
}
void ImageData::DeallocOneImage (ImageDataType ** image, int xsize, int ysize){
	#ifdef ImageDataTypeDouble
			free_dmatrix(image, 0, xsize-1, 0, ysize-1);
	#endif
	#ifdef ImageDataTypeFloat
			free_matrix(image, 0, xsize-1, 0, ysize-1);
	#endif
	#ifdef ImageDataTypeInt
			free_imatrix(image, 0, xsize-1, 0, ysize-1);
	#endif
}

void ImageData::Alloc(int Xsize, int Ysize){
	xsize = Xsize;
	ysize = Ysize;
	
	data = AllocOneImage(xsize, ysize);

	if (!data){
		Alert0("Error in allocating memory for image");
	}
	refdata = AllocOneImage(xsize, ysize);

	if (!refdata){
		Alert0("Error in allocating memory for ref image");
	}
	meanrefdata =  AllocOneImage(xsize, ysize);
//	if (!currentimage){
	if (!meanrefdata){
		Alert0("Error in allocating memory for mean ref image");
	}

}
void ImageData::DeAlloc(){
	if (meanrefdata){
//		DeallocOneImage(meanrefdata, MAX_X_SIZEIMAGE, MAX_Y_SIZEIMAGE);
		DeallocOneImage(meanrefdata, xsize, ysize);
	}
	if (refdata){
		DeallocOneImage(refdata, xsize, ysize);
	}
	if (data){
		DeallocOneImage(data, xsize, ysize);
	}
	data = refdata = meanrefdata = NULL;

}
ImageData::~ImageData(){
	DeAlloc();
}

void CAnaVisionApp::MyOnFileNew(){
	CWinApp::OnFileNew();

		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

		// Get the active MDI child window.
		CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
		if (pChild){
//			Beep(1000,20);
			CView* pV = pChild->GetActiveView( );
			if (pV){
//				Beep(2000,20);
				CAnaVisionDoc* pDoc = (CAnaVisionDoc *)(pV->GetDocument());
				if (pDoc){
//					Beep(500,20);
					pDoc->dat.p = p;
				}
			}
			else {
//				Alert0("no view");
			}
		}

}
/////////////////////////////////////////////////////////////////////////////
// CAnaVisionApp message handlers

