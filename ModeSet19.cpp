//SYLEE121115-1 

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet19.h"
#include "FileSysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 extern CFileSysInfo  FileSysInfo01; 
 extern CSysSet19  SysSet19;  //Pin SETUP   
   
/////////////////////////////////////////////////////////////////////////////
// CModeSet19 dialog

CModeSet19::CModeSet19(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSet19::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSet19)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
 
void CModeSet19::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSet19)
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
		DDX_Control(pDX, IDC_EDIT113,   m_edit113); //sylee170128-1
		DDX_Control(pDX, IDC_EDIT115,   m_edit115); //sylee170128-1

		DDX_Control(pDX, IDC_EDIT126,   m_edit126); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT127,   m_edit127); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT128,   m_edit128); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT129,   m_edit129); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT130,   m_edit130); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT131,   m_edit131); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT132,   m_edit132); //sylee180706-2
		DDX_Control(pDX, IDC_EDIT133,   m_edit133);  //sylee201027
		DDX_Control(pDX, IDC_EDIT134,   m_edit134);  //sylee201027
		DDX_Control(pDX, IDC_EDIT135,   m_edit135);  //sylee230214
		DDX_Control(pDX, IDC_EDIT136,   m_edit136);  //sylee230214
		DDX_Control(pDX, IDC_EDIT137,   m_edit137);  //sylee230214
		DDX_Control(pDX, IDC_EDIT138,   m_edit138);  //sylee230214
		DDX_Control(pDX, IDC_EDIT139,   m_edit139);  //sylee230214
		DDX_Control(pDX, IDC_EDIT140,   m_edit140);  //sylee230817

		DDX_Control(pDX, IDC_CHECK1,    m_check1);
		DDX_Control(pDX, IDC_CHECK2,    m_check2);
		DDX_Control(pDX, IDC_CHECK3,    m_check3);
		DDX_Control(pDX, IDC_CHECK4,    m_check4);
		DDX_Control(pDX, IDC_CHECK5,    m_check5);
		DDX_Control(pDX, IDC_CHECK6,    m_check6);
		DDX_Control(pDX, IDC_CHECK7,    m_check7);
		DDX_Control(pDX, IDC_CHECK8,    m_check8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet19, CDialog)
	//{{AFX_MSG_MAP(CModeSet19)
		// NOTE: the ClassWizard will add message map macros here 	
		ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
		ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
		ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
		ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
		ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
		ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	    ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	    ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet19 message handlers

BEGIN_EVENTSINK_MAP(CModeSet19, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeSet19, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSet19, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
 	ON_EVENT(CModeSet19, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet19::OnClickOk() 
{
 
	CDialog::OnOK();
}

BOOL CModeSet19::OnInitDialog() 
{
	CDialog::OnInitDialog(); 
   
	Onload(); 

	return 1;

}
 
void CModeSet19::Onload() 
{
	CString str; 
	char szText[100];
	int a;
 
    a=FileSysInfo01.LoadSaveSet19(2);//load 
	if(a!=1){
		AfxMessageBox(" Error No 1212, \n\n  SysSet19.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}
	sprintf(szText, "%d",  SysSet19.m_nSet1);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet2);
    m_edit102.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet3);
    m_edit103.SetWindowText(szText);
	sprintf(szText, "%d",  SysSet19.m_nSet4);
    m_edit104.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet5);
    m_edit105.SetWindowText(szText); 
 //sylee170313	sprintf(szText, "%d",  SysSet19.m_nSet6);
	sprintf(szText, "%.1f",  SysSet19.m_nSet6); //sylee170313
    m_edit106.SetWindowText(szText);

	sprintf(szText, "%d",  SysSet19.m_nSet7);
    m_edit107.SetWindowText(szText); 
 	sprintf(szText, "%d",  SysSet19.m_nSet8);
    m_edit108.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet9);
    m_edit109.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet10);
    m_edit110.SetWindowText(szText);

	sprintf(szText, "%d",  SysSet19.m_nSet21);
    m_edit111.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet22);
    m_edit112.SetWindowText(szText);

 	sprintf(szText, "%d",  SysSet19.m_nSet23); //sylee170228
    m_edit113.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet25); //sylee170228
    m_edit115.SetWindowText(szText);

 	sprintf(szText, "%d",  SysSet19.m_nSet31); //sylee180706-3
    m_edit126.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet32); //sylee180706-3
    m_edit127.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet33); //sylee180706-3
    m_edit128.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet34); //sylee180706-3
    m_edit129.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet35); //sylee180706-3
    m_edit130.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet36); //sylee180706-3
    m_edit131.SetWindowText(szText);
 	sprintf(szText, "%d",  SysSet19.m_nSet37); //sylee180706-3
    m_edit132.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet19.m_nSet38); //sylee201027
    m_edit133.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet19.m_nSet39); //sylee201027
    m_edit134.SetWindowText(szText);

 
	if( SysSet19.m_nSet11==1 ){     //son MAP > Set3 > Spark VB Mode
		m_check1.SetCheck(1); 
	}else{		                            m_check1.SetCheck(0); 	}//sylee230301
	if( SysSet19.m_nSet12==1 ){     //son MAP > Set3 > Old SparkBoard
		m_check2.SetCheck(1); 
	}else{		                            m_check2.SetCheck(0); 	}//sylee230301
	if( SysSet19.m_nSet13==1 ){     //son MAP > Set3 > Spark Calibration 
		m_check3.SetCheck(1); 
	}else{		                            m_check3.SetCheck(0); 	}//sylee230301
	if( SysSet19.m_nSet14==1 ){
		m_check4.SetCheck(1); 
	}else{		                            m_check4.SetCheck(0); 	}//sylee230301
	if( SysSet19.m_n4WHVon==1 ){    //sylee200520-4W정도개선
		m_check5.SetCheck(1);       //son MAP > Set3 > 4W 50V Board Use*
	}

 
	if(SysSet19.m_nSet41>100) SysSet19.m_nSet41=100;
	if(SysSet19.m_nSet42>100) SysSet19.m_nSet42=100;
	if(SysSet19.m_nSet43>100) SysSet19.m_nSet43=100;

  	sprintf(szText, "%d",  SysSet19.m_nSet41); //sylee230114
    m_edit135.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet19.m_nSet42); //sylee230114
    m_edit136.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet19.m_nSet43); //sylee230114
    m_edit137.SetWindowText(szText);
    sprintf(szText, "%d",  SysSet19.m_nSet44); //sylee230225
    m_edit138.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet19.m_nSet45); //sylee230225
    m_edit139.SetWindowText(szText);
  	sprintf(szText, "%d",  SysSet19.m_nSet46); //sylee230225
    m_edit140.SetWindowText(szText);
	 
	
	if( SysSet19.m_n4W_Data_Flag1==1 ){		m_check6.SetCheck(1); 
	}else{	                             	m_check6.SetCheck(0); 	}//sylee230301

    if( SysSet19.m_n4W_Data_Flag3==1 ){		m_check7.SetCheck(1); 
	}else{	                              	m_check7.SetCheck(0); 	}//sylee230301
    if( SysSet19.m_n4W_ContactCheck1==1 ){	m_check8.SetCheck(1); 
	}else{	                              	m_check8.SetCheck(0); 	}//sylee230420


    return ; 
}

void CModeSet19::OnSave() 
{ 

	CString str; 
	char szText[100];
//	int a; 
   	m_edit101.GetWindowText(szText,10);	
	SysSet19.m_nSet1 = atoi( szText );	 

   	 m_edit102.GetWindowText(szText,10);	
	SysSet19.m_nSet2 = atoi( szText );	 

   	m_edit103.GetWindowText(szText,10);	
	SysSet19.m_nSet3 = atoi( szText );	 
	
   	m_edit104.GetWindowText(szText,10);	
	SysSet19.m_nSet4 = atoi( szText );	 

   	m_edit105.GetWindowText(szText,10);	
	SysSet19.m_nSet5 = atoi( szText );	 

    m_edit106.GetWindowText(szText,10);	
	SysSet19.m_nSet6 = atof( szText );	//sylee170313

    if(	SysSet19.m_nSet6<0){//sylee170313
		SysSet19.m_nSet6=1;
	} 

  	m_edit107.GetWindowText(szText,10);	
	SysSet19.m_nSet7 = atoi( szText );	 

    m_edit108.GetWindowText(szText,10);	
	SysSet19.m_nSet8 = atoi( szText );	

  	m_edit109.GetWindowText(szText,10);	
	SysSet19.m_nSet9 = atoi( szText );	 

//sylee230811	if(SysSet19.m_nSet5>300){
    if(SysSet19.m_nSet5>1000){
		AfxMessageBox("  (>) 1000 over  error.      (50<=  <=1000) set please!  .  ", MB_OK); //sylee130408
		SysSet19.m_nSet5=0;
	}

	if(SysSet19.m_nSet5<10){   //sylee170614   50->10  rayok
		AfxMessageBox("  (< ) 10 under error.        (10<= <=1000) set please!  .  ", MB_OK);//sylee130408
		SysSet19.m_nSet5=0;
	}

  	m_edit110.GetWindowText(szText,10);	
	SysSet19.m_nSet10 = atoi( szText );	

	m_edit111.GetWindowText(szText,10);	
	SysSet19.m_nSet21 = atoi( szText );	

    m_edit112.GetWindowText(szText,10);	
	SysSet19.m_nSet22 = atoi( szText );	

    m_edit113.GetWindowText(szText,10);	
	SysSet19.m_nSet23 = atoi( szText );	//sylee170228

    m_edit115.GetWindowText(szText,10);	
	SysSet19.m_nSet25 = atoi( szText );	//sylee170228

    m_edit126.GetWindowText(szText,10);	
	SysSet19.m_nSet31 = atoi( szText );//sylee180706-3

    m_edit127.GetWindowText(szText,10);	
	SysSet19.m_nSet32 = atoi( szText );//sylee180706-3

    m_edit128.GetWindowText(szText,10);	
	SysSet19.m_nSet33 = atoi( szText );//sylee180706-3

    m_edit129.GetWindowText(szText,10);	
	SysSet19.m_nSet34 = atoi( szText );//sylee180706-3

    m_edit130.GetWindowText(szText,10);	
	SysSet19.m_nSet35 = atoi( szText );//sylee180706-3

    m_edit131.GetWindowText(szText,10);	
	SysSet19.m_nSet36 = atoi( szText );//sylee180706-3

    m_edit132.GetWindowText(szText,10);	
	SysSet19.m_nSet37 = atoi( szText );//sylee180706-3

    m_edit133.GetWindowText(szText,10);	
	SysSet19.m_nSet38 = atoi( szText );//sylee201027

    m_edit134.GetWindowText(szText,10);	
	SysSet19.m_nSet39 = atoi( szText );//sylee201027

    m_edit135.GetWindowText(szText,10);	 //sylee230420
	SysSet19.m_nSet41 = atoi( szText );//sylee230114
    m_edit136.GetWindowText(szText,10);	
	SysSet19.m_nSet42 = atoi( szText );//sylee230114
    m_edit137.GetWindowText(szText,10);	
	SysSet19.m_nSet43 = atoi( szText );//sylee230114
    m_edit138.GetWindowText(szText,10);	 //sylee230420
	SysSet19.m_nSet44 = atoi( szText );//sylee230225
    m_edit139.GetWindowText(szText,10);	
	SysSet19.m_nSet45 = atoi( szText );//sylee230225

	if(SysSet19.m_nSet41>1000) SysSet19.m_nSet41=1000;
	if(SysSet19.m_nSet42>1000) SysSet19.m_nSet42=1000;
	if(SysSet19.m_nSet43>1000) SysSet19.m_nSet43=1000;

    m_edit140.GetWindowText(szText,10);	
	SysSet19.m_nSet46 = atoi( szText );//sylee230817



	if(	SysSet19.m_nSet1<1){//sylee161026
		SysSet19.m_nSet1=1;
	}
	if(	SysSet19.m_nSet2<1){//sylee161026
		SysSet19.m_nSet2=1;
	}
	if(	SysSet19.m_nSet3<1){//sylee161026
		SysSet19.m_nSet3=1;
	}
	if(	SysSet19.m_nSet4<1){//sylee161026
		SysSet19.m_nSet4=1;
	}
	if(	SysSet19.m_nSet5<1){//sylee161026
		SysSet19.m_nSet5=1;
	}
	if(	SysSet19.m_nSet9<1){//sylee161026
		SysSet19.m_nSet9=1;
	}
	if(	SysSet19.m_nSet21<1){//sylee161026
		SysSet19.m_nSet21=1;
	}
	if(	SysSet19.m_nSet22<1){//sylee161026
		SysSet19.m_nSet22=1;
	}	
	if(	SysSet19.m_nSet1>300){//sylee161026
		SysSet19.m_nSet1=300;
	}
	if(	SysSet19.m_nSet2>300){//sylee161026
		SysSet19.m_nSet2=300;
	}
	if(	SysSet19.m_nSet3>300){//sylee161026
		SysSet19.m_nSet3=300;
	}
	if(	SysSet19.m_nSet4>300){//sylee161026
		SysSet19.m_nSet4=300;
	}

	if(	SysSet19.m_nSet5>1000){//sylee161026  //sylee230811  300->10000
		SysSet19.m_nSet5=1000;
	}
	if(	SysSet19.m_nSet9>1000){//sylee161026  //sylee230811  300->10000
		SysSet19.m_nSet9=1000;
	}
	if(	SysSet19.m_nSet21>300){//sylee161026
		SysSet19.m_nSet21=300;
	}
	if(	SysSet19.m_nSet22>300){//sylee161026
		SysSet19.m_nSet22=300;
	}
	if(	SysSet19.m_nSet23>10000){//sylee171017-1
		SysSet19.m_nSet23=10000;
	}
	if(	SysSet19.m_nSet25>1000){//sylee161026
		SysSet19.m_nSet25=1000;
	}
	if(	SysSet19.m_nSet23<0){//sylee161026
		SysSet19.m_nSet23=0;
	}
	if(	SysSet19.m_nSet25<0){//sylee161026
		SysSet19.m_nSet25=0;
	}

	FileSysInfo01.LoadSaveSet19(1); //save 	 

	return ;
}



void CModeSet19::OnCheck1()  
{
 	if(m_check1.GetCheck())
	{
		SysSet19.m_nSet11=1;	
	}else{
		SysSet19.m_nSet11=0;
	}
}



void CModeSet19::OnCheck2()  
{
 	if(m_check2.GetCheck())
	{
		SysSet19.m_nSet12=1;	
	}else{
		SysSet19.m_nSet12=0;
	}
}


void CModeSet19::OnCheck3()  
{
 	if(m_check3.GetCheck())
	{
		SysSet19.m_nSet13=1;	
	}else{
		SysSet19.m_nSet13=0;
	}
}


void CModeSet19::OnCheck4()  
{
 	if(m_check4.GetCheck())
	{
		SysSet19.m_nSet14=1;	
	}else{
		SysSet19.m_nSet14=0;
	}
}


void CModeSet19::OnCheck5()  
{
 	if(m_check5.GetCheck())
	{
		SysSet19.m_n4WHVon=1;	//sylee200520-4W정도개선
	}else{
		SysSet19.m_n4WHVon=0;   //sylee200520-4W정도개선
	}
}


void CModeSet19::OnCheck6()  //sylee230420
{
 	if(m_check6.GetCheck())
	{
		SysSet19.m_n4W_Data_Flag1=1;	
	}else{
		SysSet19.m_n4W_Data_Flag1=0;
	}
}
void CModeSet19::OnCheck7()  //sylee230301
{
	if(m_check7.GetCheck())
	{
		SysSet19.m_n4W_Data_Flag3=1;	
	}else{
		SysSet19.m_n4W_Data_Flag3=0;
	}
}
void CModeSet19::OnCheck8()  //sylee230301
{
	if(m_check8.GetCheck())
	{
		SysSet19.m_n4W_ContactCheck1=1;	
	}else{
		SysSet19.m_n4W_ContactCheck1=0;
	}
}