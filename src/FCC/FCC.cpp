// FCC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FCC.h"
#include "MainFrm.h"

#include "FCCDoc.h"
#include "FCCView.h"
#include "Shlwapi.h"
#include "StartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFCCApp

BEGIN_MESSAGE_MAP(CFCCApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFCCApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CFCCApp construction

CFCCApp::CFCCApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFCCApp object

CFCCApp theApp;
 // which is used to synchronize the work thread and main thread to paint classifiers.

// CFCCApp initialization

BOOL CFCCApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Fingerprint-based Compound Classifier"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	
	char pathName[MAX_PATH];
	CString tmpstr;
	::SHGetSpecialFolderPathA(NULL,pathName,CSIDL_APPDATA ,0);
	tmpstr=pathName;
	tmpstr+="\\FCC";
	
	if(!PathFileExists(tmpstr))
	{
		TRACE("%s\nnot found, to be created\n",tmpstr);
		::CreateDirectory(tmpstr,NULL);
	}
	tmpstr+="\\fcc";
	if(!PathFileExists(tmpstr))
	{
		TRACE("%s\nnot found, to be created\n",tmpstr);
		CFile hFile;
		hFile.Open(tmpstr,CFile::modeCreate|CFile::modeWrite);
		CArchive ar(&hFile,CArchive::store);
		int linewidth=1;
		int linestyle=PS_SOLID;
		float ratio=1.0;
		int NumGridVer=10;int NumGridHor=4;
		bool showgrid=true;
		COLORREF color=RGB(0,0,160);
		ar<<linewidth;
		ar<<linestyle;
		ar<<color;//save training display option
		color=RGB(160,16,0);
		ar<<linewidth;
		ar<<linestyle;
		ar<<color;//save cv display option
		ar<<NumGridVer;
		ar<<NumGridHor;
		ar<<showgrid;//save grid display option
		ar<<ratio;
		ar<<ratio;//save zoom ratio
		ar.Close();
		hFile.Close();
	}
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFCCDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CStartView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	if(cmdInfo.m_nShellCommand==CCommandLineInfo::FileNew)
	{
		POSITION pos=pDocTemplate->GetFirstDocPosition();
		CDocument* pDoc=pDocTemplate->GetNextDoc(pos);
		if(pDoc)
			pDoc->OnNewDocument();
	}
	m_pMainWnd->UpdateWindow();
	
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CFCCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CFCCApp message handlers


CDocument* CFCCApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: Add your specialized code here and/or call the base class
	return CWinApp::OpenDocumentFile(lpszFileName);
}
