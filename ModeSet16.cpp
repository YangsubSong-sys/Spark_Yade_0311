//SYLEE121115-1 

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet16.h"
 #include "FileSysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 extern CFileSysInfo  FileSysInfo01; 
 extern CSysSet16  SysSet16;  //Pin SETUP   
 extern double  d4WADCPer1 ,d4WADCPer2,d4WADCPer3 , d4WADCSigma1; //sylee221208-1
   
/////////////////////////////////////////////////////////////////////////////
// CModeSet16 dialog

CModeSet16::CModeSet16(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSet16::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSet16)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
 
void CModeSet16::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSet16)
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


		DDX_Control(pDX, IDC_CHECK1,    m_check1);
		DDX_Control(pDX, IDC_CHECK2,    m_check2);
		DDX_Control(pDX, IDC_CHECK3,    m_check3);
		DDX_Control(pDX, IDC_CHECK4,    m_check4);
		DDX_Control(pDX, IDC_CHECK5,    m_check5);
		DDX_Control(pDX, IDC_CHECK6,    m_check6);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet16, CDialog)
	//{{AFX_MSG_MAP(CModeSet16)
		// NOTE: the ClassWizard will add message map macros here 	
		ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
		ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
		ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
		ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
		ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
		ON_BN_CLICKED(IDC_CHECK6, OnCheck6)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet16 message handlers

BEGIN_EVENTSINK_MAP(CModeSet16, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeSet16, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSet16, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
 	ON_EVENT(CModeSet16, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet16::OnClickOk() 
{
 	CDialog::OnOK();
}

BOOL CModeSet16::OnInitDialog() 
{
	CDialog::OnInitDialog(); 
   
	Onload(); 
	return 1;
}
 



void CModeSet16::Onload() 
{
	CString str; 
	char szText[100];
	int a;
 
    a=FileSysInfo01.LoadSaveSet16(2);//load 
	if(a!=1){
		AfxMessageBox(" Error No 1212, \n\n  SysSet16.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}

	sprintf(szText, "%d",  SysSet16.m_nSet1);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet16.m_nSet2);
    m_edit102.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet16.m_nSet3);
    m_edit103.SetWindowText(szText);
	sprintf(szText, "%d",  SysSet16.m_nSet4);
    m_edit104.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet16.m_nSet5);
    m_edit105.SetWindowText(szText); 
 	sprintf(szText, "%d",  SysSet16.m_nSet6);
    m_edit106.SetWindowText(szText);            //son "4W PassCount 1 M"

	sprintf(szText, "%d",  SysSet16.m_nSet7);
    m_edit107.SetWindowText(szText);            //son "4W   ~10 mohm     mohm   x0.01"
 	sprintf(szText, "%d",  SysSet16.m_nSet8);
    m_edit108.SetWindowText(szText);            //son "   4W    ~MAX. mohm %    x 0.01"
 	sprintf(szText, "%d",  SysSet16.m_nSet9);
    m_edit109.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet16.m_nSet10);
    m_edit110.SetWindowText(szText);            //son "4W   SIGMA       x 0.01"
 	sprintf(szText, "%d",  SysSet16.m_nSet21);
    m_edit111.SetWindowText(szText);            //son "4W   ~  45. mohm    %    x0.01 "


 	sprintf(szText, "%d",  SysSet16.m_nSet22);//sylee230430
    m_edit112.SetWindowText(szText);            //son "4W PassCount 2 A "
 	sprintf(szText, "%d",  SysSet16.m_nSet23);//sylee230430
    m_edit113.SetWindowText(szText);            //son "4W PassCount 3  AS"

	
	if( SysSet16.m_nSet11==1 ){
		m_check1.SetCheck(1); 
	}

	if( SysSet16.m_nSet12==1 ){
		m_check2.SetCheck(1); 
	}

	if( SysSet16.m_nSet13==1 ){
		m_check3.SetCheck(1); 
	}

	if( SysSet16.m_nSet14==1 ){
		m_check4.SetCheck(1);           //son "4W No AC"
	}

	if( SysSet16.m_nSet15==1 ){
		m_check5.SetCheck(1);           //son "4W ADC% USE"
	}

	if( SysSet16.m_nSet16==1 ){
		m_check6.SetCheck(1); 
	}

    return ; 
}



void CModeSet16::OnSave() 
{ 

	CString str; 
	char szText[100];
//	int a; 
   	m_edit101.GetWindowText(szText,10);	
	SysSet16.m_nSet1 = atoi( szText );	 
   	 m_edit102.GetWindowText(szText,10);	
	SysSet16.m_nSet2 = atoi( szText );	 
   	m_edit103.GetWindowText(szText,10);	
	SysSet16.m_nSet3 = atoi( szText );	 
   	m_edit104.GetWindowText(szText,10);	
	SysSet16.m_nSet4 = atoi( szText );	 
   	m_edit105.GetWindowText(szText,10);	
	SysSet16.m_nSet5 = atoi( szText );	 
    m_edit106.GetWindowText(szText,10);	
	SysSet16.m_nSet6 = atoi( szText );	

  	m_edit107.GetWindowText(szText,10);	
	SysSet16.m_nSet7 = atoi( szText );	 
    m_edit108.GetWindowText(szText,10);	
	SysSet16.m_nSet8 = atoi( szText );	

  	m_edit109.GetWindowText(szText,10);	
	SysSet16.m_nSet9 = atoi( szText );	
	
   	m_edit112.GetWindowText(szText,10);	//sylee230430
	SysSet16.m_nSet22 = atoi( szText );	//sylee230430
   	m_edit113.GetWindowText(szText,10);	//sylee230430
	SysSet16.m_nSet23 = atoi( szText );	//sylee230430


	if(SysSet16.m_nSet5>300){
		AfxMessageBox("  (>) 300 over  error.      (10<=  <=300) set please!  .  ", MB_OK); //sylee130408
		SysSet16.m_nSet5=0;
	}

	if(SysSet16.m_nSet5<10){  //sylee170614   50->10  rayok
		AfxMessageBox("  (<) 10 under error.        (10<= <=300) set please!  .  ", MB_OK);//sylee130408
		SysSet16.m_nSet5=0;
	}

  	m_edit110.GetWindowText(szText,10);	
	SysSet16.m_nSet10 = atoi( szText );	 
  	m_edit111.GetWindowText(szText,10);	
	SysSet16.m_nSet21 = atoi( szText );

	FileSysInfo01.LoadSaveSet16(1); //save 	 

	return ;
}



void CModeSet16::OnCheck1()  
{
 	if(m_check1.GetCheck())
	{
		SysSet16.m_nSet11=1;	
	}else{
		SysSet16.m_nSet11=0;
	}
}



void CModeSet16::OnCheck2()  
{
 	if(m_check2.GetCheck())
	{
		SysSet16.m_nSet12=1;	
	}else{
		SysSet16.m_nSet12=0;
	}
}


void CModeSet16::OnCheck3()  
{
 	if(m_check3.GetCheck())
	{
		SysSet16.m_nSet13=1;	
	}else{
		SysSet16.m_nSet13=0;
	}
}


void CModeSet16::OnCheck4()  
{
 	if(m_check4.GetCheck())
	{
		SysSet16.m_nSet14=1;	 
	}else{
		SysSet16.m_nSet14=0;
	}
}


void CModeSet16::OnCheck5()  
{
 	if(m_check5.GetCheck())
	{
		SysSet16.m_nSet15=1;	
	}else{
		SysSet16.m_nSet15=0;
	}
}


void CModeSet16::OnCheck6()  
{
 	if(m_check6.GetCheck())
	{
		SysSet16.m_nSet16=1;	
	}else{
		SysSet16.m_nSet16=0;
	}
}
