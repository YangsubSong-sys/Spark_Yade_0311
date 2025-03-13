// BiOpTroCon.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BiOpTroCon.h"

#include "MainFrm.h"
#include "BiOpTroConDoc.h"
#include "BiOpTroConView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HWND g_hwndInstance = NULL;

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConApp

BEGIN_MESSAGE_MAP(CBiOpTroConApp, CWinApp)
	//{{AFX_MSG_MAP(CBiOpTroConApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
 //	OnFilePrintSetup1()
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConApp construction

CBiOpTroConApp::CBiOpTroConApp()
{

	 m_hMutex = NULL;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
 
/////////////////////////////////////////////////////////////////////////////
// The one and only CBiOpTroConApp object

CBiOpTroConApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConApp initialization

BOOL CBiOpTroConApp::InitInstance()
{


		// 중복 실행 방지
	m_hMutex = ::CreateMutex(NULL, FALSE, _T("BiOpTro_BBT_Controller"));
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(g_hwndInstance != NULL)
		{
			::SetForegroundWindow(g_hwndInstance);
			if(::IsIconic(g_hwndInstance))
				::ShowWindow(g_hwndInstance, SW_SHOWDEFAULT);
		}

//		HWND hPrev = NULL;
//		hPrev = ::FindWindow( NULL, "BBT" );
//
//		// 실행 중인 윈도우 핸들을 찾아서 복원
//		if( hPrev )
//		{
//			 ::SendMessage( hPrev, WM_SYSCOMMAND, SC_RESTORE, 0 );
//			 ::SetForegroundWindow( hPrev );
//			 ::SetFocus( hPrev );
//		}

 
		AfxMessageBox("Machine program is already runung.\n\nFind the image name and end the process in Task Manager and then restart.", MB_OK | MB_ICONSTOP);
 
	
		return FALSE;
	}



	AfxEnableControlContainer();	

//	SetDialogBkColor(RGB(255, 225, 255), RGB(0, 0,0));   // 분홍 배경에  검은 색 글짜 
		SetDialogBkColor(RGB(255, 247, 255), RGB(0, 0,0));   // 분홍 배경에  검은 색 글짜 

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBiOpTroConDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBiOpTroConView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
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
void CBiOpTroConApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConApp message handlers


int CBiOpTroConApp::ExitInstance() 
{


 /*    CString str;
     int nRet1;
	 str.Format(" Program Exit ?   "  ); //sylee1210010									 
	 nRet1=AfxMessageBox(str,MB_YESNO | MB_ICONEXCLAMATION);
	 if(nRet1!=6){// !yes		 
         return 0;
	 } 

 */


	// 중복 실행 방지
	if(m_hMutex != NULL) ::CloseHandle(m_hMutex);

	// TODO: Add your specialized code here and/or call the base class
	if(g_hbr != NULL)	::DeleteObject(g_hbr);
	return CWinApp::ExitInstance();
}



 void CBiOpTroConApp::OnFilePrintSetup1() //sylee
{
 int i;

 OnFilePrintSetup();
 i=3;
 
 } 


 
