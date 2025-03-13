//SYLEE121115-1 

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet17.h"
 #include "FileSysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

 extern CFileSysInfo  FileSysInfo01; 
 extern CSysSet17  SysSet17;  //Pin SETUP   
   
/////////////////////////////////////////////////////////////////////////////
// CModeSet17 dialog

CModeSet17::CModeSet17(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSet17::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSet17)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
 
void CModeSet17::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSet17)
		// NOTE: the ClassWizard will add DDX and DDV calls here

		DDX_Control(pDX, IDC_EDIT101,   m_edit101);		
		DDX_Control(pDX, IDC_EDIT102,   m_edit102); 
	    DDX_Control(pDX, IDC_EDIT103,   m_edit103); 
		DDX_Control(pDX, IDC_CHECK1,    m_check1);
	 

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet17, CDialog)
	//{{AFX_MSG_MAP(CModeSet17)
		// NOTE: the ClassWizard will add message map macros here 	
		ON_BN_CLICKED(IDC_CHECK1, OnCheck1)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet17 message handlers

BEGIN_EVENTSINK_MAP(CModeSet17, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeSet17, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSet17, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
 	ON_EVENT(CModeSet17, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet17::OnClickOk() 
{
 
	CDialog::OnOK();
}

BOOL CModeSet17::OnInitDialog() 
{
	CDialog::OnInitDialog();    
	Onload(); 
	return 1;
}
 
void CModeSet17::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 

	  m_Label1.SetWindowText(LoadMessage(1,"TitleSetupCycle", 101));
 	  m_Label2.SetWindowText(LoadMessage(1,"TitleSetupCycle", 102));
	  m_Label3.SetWindowText(LoadMessage(1,"TitleSetupCycle", 103));
	  m_Label4.SetWindowText(LoadMessage(1,"TitleSetupCycle", 104));

	  m_button101.SetWindowText(LoadMessage(1,"TitleSetupCycle", 105));
	  m_button102.SetWindowText(LoadMessage(1,"TitleSetupCycle", 106));
 	  m_buttonOk.SetWindowText(LoadMessage(1,"TitleSetupCycle", 107)); 

  }

}


void CModeSet17::Onload() 
{
	CString str; 
	char szText[100];
	int a;
 
    a=FileSysInfo01.LoadSaveSet17(2);//load 
	if(a!=1){
		AfxMessageBox(" Error No 1212, \n\n  SysSet17.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}
	sprintf(szText, "%d",  SysSet17.m_nSet1);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet17.m_nSet2);
    m_edit102.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet17.m_nSet3);
    m_edit103.SetWindowText(szText); 

	if(SysSet17.m_nSet11==1){
		m_check1.SetCheck(1);
	}else{
		m_check1.SetCheck(0);
	}

    return ; 
}




void CModeSet17::OnSave() 
{ 

	CString str; 
	char szText[100];
 
   	m_edit101.GetWindowText(szText,10);	
	SysSet17.m_nSet1 = atoi( szText );	 
   	m_edit102.GetWindowText(szText,10);	
	SysSet17.m_nSet2 = atoi( szText );	 
    m_edit103.GetWindowText(szText,10);	
	SysSet17.m_nSet3 = atoi( szText );	   	 
 
	if(SysSet17.m_nSet1>50000){
			AfxMessageBox("  Count> 50000 over  error.    \n   (  <=50000) set please!  .  ", MB_OK);
			SysSet17.m_nSet1=0;
			m_edit101.SetWindowText("0");
	}

	if(SysSet17.m_nSet1<1){
			AfxMessageBox(" Count   <1,  under error.        \n   (  >=1) set please!  .  ", MB_OK);
			SysSet17.m_nSet1=0;
			m_edit101.SetWindowText("0");
	}

    if(SysSet17.m_nSet2>1000){
			AfxMessageBox("  Delay > 1000 over  error.    \n   (  <=1000) set please!  .  ", MB_OK);
			SysSet17.m_nSet2=0;
			m_edit102.SetWindowText("0");
	}

	if(SysSet17.m_nSet2<1){
			AfxMessageBox(" Delay   <1,  under error.        \n   (  >=1) set please!  .  ", MB_OK);
			SysSet17.m_nSet2=0;
			m_edit102.SetWindowText("0");
	}

    if(SysSet17.m_nSet3>100){
			AfxMessageBox("  Short Error Set > 100 over  error.    \n   (  <=100) set please!  .  ", MB_OK);
			SysSet17.m_nSet3=0;
			m_edit103.SetWindowText("0");
	}

	if(SysSet17.m_nSet2<1){
			AfxMessageBox(" Short Error Set  <1,  under error.        \n   (  >=1) set please!  .  ", MB_OK);
			SysSet17.m_nSet3=0;
			m_edit103.SetWindowText("0");
	}



    if(m_check1.GetCheck())
	{
		SysSet17.m_nSet11=1;	
	}else{
		SysSet17.m_nSet11=0;
	}

	FileSysInfo01.LoadSaveSet17(1); //save 	

	return ;
}



void CModeSet17::OnCheck1()  
{
 /*	if(m_check1.GetCheck())
	{
		SysSet17.m_nSet11=1;	
	}else{
		SysSet17.m_nSet11=0;
	}

	*/
}



  
