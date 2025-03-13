//SYLEE121115-1 

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet15.h"
 #include "FileSysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 extern CFileSysInfo  FileSysInfo01; 
 extern CSysSet15  SysSet15;  //Pin SETUP   
   
/////////////////////////////////////////////////////////////////////////////
// CModeSet15 dialog

CModeSet15::CModeSet15(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSet15::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSet15)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
 
void CModeSet15::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSet15)
		// NOTE: the ClassWizard will add DDX and DDV calls here

		DDX_Control(pDX, IDC_EDIT101,   m_edit101);		
		DDX_Control(pDX, IDC_EDIT102,   m_edit102); 
		DDX_Control(pDX, IDC_EDIT103,   m_edit103); 
		DDX_Control(pDX, IDC_EDIT104,   m_edit104);
		DDX_Control(pDX, IDC_EDIT105,   m_edit105);  
		DDX_Control(pDX, IDC_EDIT106,   m_edit106); 
	 	DDX_Control(pDX, IDC_EDIT107,   m_edit107); 
		DDX_Control(pDX, IDC_EDIT108,   m_edit108);		
		DDX_Control(pDX, IDC_EDIT109,   m_edit109); 
		DDX_Control(pDX, IDC_EDIT110,   m_edit110); 
		DDX_Control(pDX, IDC_EDIT111,   m_edit111);
		DDX_Control(pDX, IDC_EDIT112,   m_edit112);  
		DDX_Control(pDX, IDC_EDIT113,   m_edit113); 
	 	DDX_Control(pDX, IDC_EDIT115,   m_edit115); 

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet15, CDialog)
	//{{AFX_MSG_MAP(CModeSet15)
		// NOTE: the ClassWizard will add message map macros here



	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet15 message handlers

BEGIN_EVENTSINK_MAP(CModeSet15, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeSet15, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSet15, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
 	ON_EVENT(CModeSet15, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet15::OnClickOk() 
{
 


	CDialog::OnOK();
}

BOOL CModeSet15::OnInitDialog() 
{
	CDialog::OnInitDialog(); 
   
	Onload(); 

	return 1;

}
 
void CModeSet15::Onload() 
{
	CString str; 
	char szText[100];
	int a;
 
    a=FileSysInfo01.LoadSaveSet15(2);//load 
	if(a!=1){
		AfxMessageBox(" Error No 1212, \n\n  SysSet15.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}
	sprintf(szText, "%d",  SysSet15.m_nSet1);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet2);
    m_edit102.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet3);
    m_edit103.SetWindowText(szText);
	sprintf(szText, "%d",  SysSet15.m_nSet4);
    m_edit104.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet5);
    m_edit105.SetWindowText(szText); 
 	sprintf(szText, "%d",  SysSet15.m_nSet6);
    m_edit106.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet7);
    m_edit107.SetWindowText(szText);  //sylee230822
	sprintf(szText, "%d",  SysSet15.m_nSet8);
    m_edit108.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet9);
    m_edit109.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet10);
    m_edit110.SetWindowText(szText);
	sprintf(szText, "%d",  SysSet15.m_nSet11);
    m_edit111.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet12);
    m_edit112.SetWindowText(szText); 
 	sprintf(szText, "%d",  SysSet15.m_nSet13);
    m_edit113.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet15.m_nSet15);
    m_edit115.SetWindowText(szText);  //sylee230822
    return ; 
}

void CModeSet15::OnSave() 
{ 

	CString str; 
	char szText[100];
 
   	m_edit101.GetWindowText(szText,10);	
	SysSet15.m_nSet1 = atoi( szText );	 
   	 m_edit102.GetWindowText(szText,10);	
	SysSet15.m_nSet2 = atoi( szText );	 
   	m_edit103.GetWindowText(szText,10);	
	SysSet15.m_nSet3 = atoi( szText );	 
   	m_edit104.GetWindowText(szText,10);	
	SysSet15.m_nSet4 = atoi( szText );	 
   	m_edit105.GetWindowText(szText,10);	
	SysSet15.m_nSet5 = atoi( szText );	 
    m_edit106.GetWindowText(szText,10);	
	SysSet15.m_nSet6 = atoi( szText );	
    m_edit107.GetWindowText(szText,10);	
	SysSet15.m_nSet7 = atoi( szText );	
   	m_edit108.GetWindowText(szText,10);	
	SysSet15.m_nSet8 = atoi( szText );	 
   	 m_edit109.GetWindowText(szText,10);	
	SysSet15.m_nSet9 = atoi( szText );	 
   	m_edit110.GetWindowText(szText,10);	
	SysSet15.m_nSet10 = atoi( szText );	 
   	m_edit111.GetWindowText(szText,10);	
	SysSet15.m_nSet11 = atoi( szText );	 
   	m_edit112.GetWindowText(szText,10);	
	SysSet15.m_nSet12 = atoi( szText );	 
    m_edit113.GetWindowText(szText,10);	
	SysSet15.m_nSet13= atoi( szText );	
    m_edit115.GetWindowText(szText,10);	
	SysSet15.m_nSet15 = atoi( szText );	

	FileSysInfo01.LoadSaveSet15(1); //save 	 
	return ;
}
