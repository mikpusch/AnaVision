; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=ParamsDiag
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "BufferedDiffusion.h"
LastPage=0

ClassCount=14
Class1=CBufferedDiffusionApp
Class2=CBufferedDiffusionDoc
Class3=CBufferedDiffusionView
Class4=CMainFrame

ResourceCount=16
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_BUFFERTYPE
Class5=CChildFrame
Class6=CAboutDlg
Resource4=IDR_MENU_EDITCTRL
Resource5=IDD_REDRAWDIAG
Resource6=IDR_MENU_POP2 (Italian (Italy))
Class7=DiagIO
Resource7=IDR_MAINFRAME (English (U.S.))
Class8=ParamsDiag
Resource8=IDD_DIAGIO (English (U.S.))
Class9=RedrawDiag
Resource9=IDR_MENU_EDITCTRL (Italian (Italy))
Class10=DiagSeriesSimul
Class11=SimulSeriesDiag
Resource10=IDD_DIAG_SIMUL_SERIES
Class12=MyEditCtrl
Resource11=IDD_ABOUTBOX (English (U.S.))
Resource12=IDD_DIAG_SIMUL_SERIES (Italian (Italy))
Resource13=IDD_REDRAWDIAG (Italian (Italy))
Resource14=IDR_MENU_POP2
Resource15=IDR_BUFFERTYPE (English (U.S.))
Class13=SimThread
Class14=GoThread
Resource16=IDD_DIAGPARAMS (English (U.S.))

[CLS:CBufferedDiffusionApp]
Type=0
HeaderFile=BufferedDiffusion.h
ImplementationFile=BufferedDiffusion.cpp
Filter=N

[CLS:CBufferedDiffusionDoc]
Type=0
HeaderFile=BufferedDiffusionDoc.h
ImplementationFile=BufferedDiffusionDoc.cpp
Filter=N

[CLS:CBufferedDiffusionView]
Type=0
HeaderFile=BufferedDiffusionView.h
ImplementationFile=BufferedDiffusionView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_FILE_LOADSIMULATIONSERIESFROMFILE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=BufferedDiffusion.cpp
ImplementationFile=BufferedDiffusion.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
CommandCount=8
Command8=ID_APP_ABOUT

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
CommandCount=8
Command8=ID_APP_ABOUT

[MNU:IDR_BUFFERTYPE]
Type=1
Class=CBufferedDiffusionView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
CommandCount=21
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_BUFFERTYPE (English (U.S.))]
Type=1
Class=CBufferedDiffusionView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_FILE_SAVESIMULATIONSERIESONFILE
Command11=ID_FILE_LOADSIMULATIONSERIESFROMFILE
Command12=ID_APP_EXIT
Command13=ID_FILE_READDRDTTABLE
Command14=ID_FILE_WRITEDRDTTABLE
Command15=ID_EDIT_UNDO
Command16=ID_EDIT_CUT
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_EDIT_PARAMETERS
Command20=ID_VIEW_TOOLBAR
Command21=ID_VIEW_STATUS_BAR
Command22=ID_WINDOW_NEW
Command23=ID_WINDOW_CASCADE
Command24=ID_WINDOW_TILE_HORZ
Command25=ID_WINDOW_ARRANGE
Command26=ID_APP_ABOUT
Command27=ID_TEST_TEST
Command28=ID_GO_GO
Command29=ID_GO_LINEARIZED
Command30=ID_GO_SECONDLINEARIZED
Command31=ID_GO_CLEARWINDIOW
Command32=ID_GO_REDRAWOLDSIMULATION
Command33=ID_GO_SERIESOFSIMULATIONS
Command34=ID_GO_CONTINUESERIESSIMULATION
Command35=ID_STOP_STOPSERIESSIMULATION
Command36=ID_STOP_SLOWALMOSTPAUSESIMULATION
Command37=ID_STOP_RESTART
Command38=ID_STOP_SAVEENERGY
Command39=ID_STOP_CHANGENOOFSECONDSOUT
CommandCount=39

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_GO_CLEARWINDIOW
Command2=ID_GO_GO
Command3=ID_GO_LINEARIZED
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_EDIT_PARAMETERS
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIAGIO (English (U.S.))]
Type=1
Class=DiagIO
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_NUMBER,edit,1350762624
Control4=IDC_STRING,edit,1484849280

[CLS:DiagIO]
Type=0
HeaderFile=DiagIO.h
ImplementationFile=DiagIO.cpp
BaseClass=CDialog
Filter=D
LastObject=DiagIO
VirtualFilter=dWC

[DLG:IDD_DIAGPARAMS (English (U.S.))]
Type=1
Class=ParamsDiag
ControlCount=89
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_DT,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_R0,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_RMAX,edit,1350631552
Control10=IDC_STATIC,button,1342177287
Control11=IDC_FIXDR,button,1342177289
Control12=IDC_FIXM,button,1476395017
Control13=IDC_STATIC,static,1342308352
Control14=IDC_DR,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_NSLICES,edit,1484849280
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_ALPHA,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_BETA,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_KD,edit,1350631552
Control24=IDC_CALCALPHA,button,1342177289
Control25=IDC_CALCBETA,button,1342177289
Control26=IDC_CALCKD,button,1342177289
Control27=IDC_STATIC,static,1342308352
Control28=IDC_DH,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_DB,edit,1350631552
Control31=IDC_STATIC,static,1342308352
Control32=IDC_NITER,edit,1350631552
Control33=IDC_STATIC,static,1342308352
Control34=IDC_NUPDATE,edit,1350631552
Control35=IDC_STATIC,static,1342308352
Control36=IDC_PHBULK,edit,1350631552
Control37=IDC_STATIC,static,1342308352
Control38=IDC_TOTALBUFFER,edit,1350631552
Control39=IDC_STATIC,static,1342308352
Control40=IDC_IH,edit,1350631552
Control41=IDC_AUTOCALCBUFFERCAPAC,button,1342242819
Control42=IDC_SIMPLEWAGNERKEIZER,button,1476460547
Control43=IDC_WRITEFILES,button,1342242819
Control44=IDC_OUTFILENAMES,edit,1350631552
Control45=IDC_STATIC,static,1342308352
Control46=IDC_CHANGEFILENAME,button,1342242816
Control47=IDC_STATIC,button,1342177287
Control48=IDC_CONSTANTCURRENT,button,1342177289
Control49=IDC_TRAIN,button,1342177289
Control50=IDC_STATIC,static,1342308352
Control51=IDC_TIMECURRENTON,edit,1350631552
Control52=IDC_STATIC,static,1342308352
Control53=IDC_TIMECURRENTOFF,edit,1350631552
Control54=IDC_STATIC,static,1342308352
Control55=IDC_NPULSESTRAIN,edit,1350631552
Control56=IDC_RENORMALIZE,button,1342242819
Control57=IDC_STATIC,static,1342308352
Control58=IDC_DT2,edit,1350631552
Control59=IDC_STATIC,static,1342308352
Control60=IDC_FACTDR,edit,1350631552
Control61=IDC_STATIC,static,1342308352
Control62=IDC_DT3,edit,1350631552
Control63=IDC_STATIC,static,1342308352
Control64=IDC_MAXDR,edit,1350631552
Control65=IDC_STATIC,static,1342308352
Control66=IDC_NROOTS,edit,1484849280
Control67=IDC_USEWAGNERKEIZER,button,1342242819
Control68=IDC_STATIC,button,1342177287
Control69=IDC_STATIC,static,1342308352
Control70=IDC_EXTERNALLYSUPPLEIDBUFFERCAPAC,edit,1350631552
Control71=IDC_USE_FLUX_CORRECTION,button,1342242819
Control72=IDC_STATIC,static,1342308352
Control73=IDC_RANGEPH,edit,1350631552
Control74=IDC_STATIC,static,1342308352
Control75=IDC_FACTORCORRECTION,edit,1350631552
Control76=IDC_STATIC,static,1342308352
Control77=IDC_INTERNALBUFFERCAPAC,edit,1484849280
Control78=IDC_STATIC,button,1342177287
Control79=IDC_CHECK_INCLUDE_FIXED_SURFACE_BUFFER,button,1342242819
Control80=IDC_STATIC,static,1342308352
Control81=IDC_KD_FIXED_SURFACEBUFFER,edit,1350631552
Control82=IDC_STATIC,static,1342308352
Control83=IDC_AMOUNT_FIXED_SURFACEBUFFER,edit,1350631552
Control84=IDC_STATIC,static,1342308352
Control85=IDC_AMOUNT_FIXED_THICKNESSFIXEDSURFACE,edit,1350631552
Control86=IDC_READFILE,button,1342242819
Control87=IDC_STATIC,static,1342308352
Control88=IDC_NSECONDSOUT,edit,1350631552
Control89=IDC_USE_PREDICTORIN_WAGNERKEIZER,button,1476460547

[CLS:ParamsDiag]
Type=0
HeaderFile=ParamsDiag.h
ImplementationFile=ParamsDiag.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CHANGEFILENAME
VirtualFilter=dWC

[DLG:IDD_REDRAWDIAG]
Type=1
Class=RedrawDiag
ControlCount=20
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_REDRAW_INDEX,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_RADIO_GRAY,button,1342177289
Control7=IDC_RADIO_BLACK,button,1342177289
Control8=IDC_RADIO_RED,button,1342177289
Control9=IDC_RADIO_GREEN,button,1342177289
Control10=IDC_RADIO_YELLOW,button,1342177289
Control11=IDC_RADIO_BLUE,button,1342177289
Control12=IDC_RADIO_ORANGE,button,1342177289
Control13=IDC_RADIO_PINK,button,1342177289
Control14=IDC_RADIO_BROWN,button,1342177289
Control15=IDC_RADIO_LIGHTBLUE,button,1342177289
Control16=IDC_NSIMUL,edit,1484849280
Control17=IDC_STATIC,static,1342308352
Control18=IDC_SHOWREF,button,1342242819
Control19=IDC_REFINDEX,edit,1350631552
Control20=IDC_STATIC,static,1342308352

[CLS:RedrawDiag]
Type=0
HeaderFile=RedrawDiag.h
ImplementationFile=RedrawDiag.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_REFINDEX
VirtualFilter=dWC

[CLS:DiagSeriesSimul]
Type=0
HeaderFile=DiagSeriesSimul.h
ImplementationFile=DiagSeriesSimul.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_VALUES_DRDT
VirtualFilter=dWC

[DLG:IDD_DIAG_SIMUL_SERIES]
Type=1
Class=SimulSeriesDiag
ControlCount=10
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_TOTALTIME,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_NUMBERCURVES,edit,1350631552
Control7=IDMYOK,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_INTERMEDIATETIME,edit,1350631552
Control10=IDC_USE_TWO_TIMES_POINTS,button,1342242819

[CLS:SimulSeriesDiag]
Type=0
HeaderFile=SimulSeriesDiag.h
ImplementationFile=SimulSeriesDiag.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_USE_TWO_TIMES_POINTS

[MNU:IDR_MENU_EDITCTRL]
Type=1
Class=?
Command1=ID_MENU_COPY
Command2=ID_MENU_PASTE
Command3=ID_MENU_CUT
CommandCount=3

[CLS:MyEditCtrl]
Type=0
HeaderFile=MyEditCtrl.h
ImplementationFile=MyEditCtrl.cpp
BaseClass=CRichEditCtrl
Filter=W
LastObject=MyEditCtrl
VirtualFilter=WC

[MNU:IDR_MENU_POP2]
Type=1
Class=?
Command1=ID_EDITCTRLL_SELECTALL
Command2=ID_EDITCTRLL_COPY
Command3=ID_EDITCTRLL_CUT
Command4=ID_EDITCTRLL_PASTE
CommandCount=4

[DLG:IDD_DIAG_SIMUL_SERIES (Italian (Italy))]
Type=1
Class=SimulSeriesDiag
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_TOTALTIME,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_NUMBERCURVES,edit,1350631552
Control7=IDMYOK,button,1342242816

[DLG:IDD_REDRAWDIAG (Italian (Italy))]
Type=1
Class=RedrawDiag
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_REDRAW_INDEX,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_RADIO_GRAY,button,1342177289
Control7=IDC_RADIO_BLACK,button,1342177289
Control8=IDC_RADIO_RED,button,1342177289
Control9=IDC_RADIO_GREEN,button,1342177289
Control10=IDC_RADIO_YELLOW,button,1342177289
Control11=IDC_RADIO_BLUE,button,1342177289
Control12=IDC_RADIO_ORANGE,button,1342177289
Control13=IDC_RADIO_PINK,button,1342177289
Control14=IDC_RADIO_BROWN,button,1342177289
Control15=IDC_RADIO_LIGHTBLUE,button,1342177289
Control16=IDC_NSIMUL,edit,1484849280
Control17=IDC_STATIC,static,1342308352

[MNU:IDR_MENU_EDITCTRL (Italian (Italy))]
Type=1
Class=?
Command1=ID_MENU_COPY
Command2=ID_MENU_PASTE
Command3=ID_MENU_CUT
CommandCount=3

[MNU:IDR_MENU_POP2 (Italian (Italy))]
Type=1
Class=?
Command1=ID_EDITCTRLL_SELECTALL
Command2=ID_EDITCTRLL_COPY
Command3=ID_EDITCTRLL_CUT
Command4=ID_EDITCTRLL_PASTE
CommandCount=4

[CLS:SimThread]
Type=0
HeaderFile=SimThread.h
ImplementationFile=SimThread.cpp
BaseClass=CWinThread
Filter=N
LastObject=ID_APP_ABOUT

[CLS:GoThread]
Type=0
HeaderFile=GoThread.h
ImplementationFile=GoThread.cpp
BaseClass=CWinThread
Filter=N
LastObject=GoThread

