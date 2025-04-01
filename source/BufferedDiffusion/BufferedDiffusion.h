// BufferedDiffusion.h : main header file for the BUFFEREDDIFFUSION application
//

#if !defined(AFX_BUFFEREDDIFFUSION_H__1FA2DC4C_6D58_406A_9570_A69183E603B1__INCLUDED_)
#define AFX_BUFFEREDDIFFUSION_H__1FA2DC4C_6D58_406A_9570_A69183E603B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "../common/utils.h"
#include "../common/nrutil.h"
#include "../common/Dcompl.h"

#include "diffusion.h"



#include "SeriesSimulation.h"

#include "resource.h"       // main symbols
#include "DiagIO.h"
#include "ParamsDiag.h"
#include "RedrawDiag.h"

#include "SimulSeriesDiag.h"

#include "SimulationMemory.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "BufferedDiffusionDoc.h"
#include "BufferedDiffusionView.h"
#include "MyEditCtrl.h"
#include "SimThread.h"
#include "GoThread.h"

#define MESSAGE_GO_GO		(WM_USER + 2)
#define MESSAGE_FINISHED	(WM_USER + 3)
#define MESSAGE_DRAWCURVES	(WM_USER + 4)
#define MESSAGE_GO_FINISHED (WM_USER + 5)
#define	SAVEENEGRYTIMERMESSAGE (WM_USER + 6)


/////////////////////////////////////////////////////////////////////////////
// CBufferedDiffusionApp:
// See BufferedDiffusion.cpp for the implementation of this class
//

class CBufferedDiffusionApp : public CWinApp
{
public:
	CBufferedDiffusionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBufferedDiffusionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBufferedDiffusionApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUFFEREDDIFFUSION_H__1FA2DC4C_6D58_406A_9570_A69183E603B1__INCLUDED_)
