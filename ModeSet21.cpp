//SYLEE121115-1 

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet21.h"
 #include "FileSysInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 extern CFileSysInfo  FileSysInfo01; 
 extern CSysSet211  SysSet211;  //Pin SETUP   
   
/////////////////////////////////////////////////////////////////////////////
// CModeSet21 dialog

CModeSet21::CModeSet21(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSet21::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSet21)
	m_bPrtBasic = FALSE;       //son230720
	m_bPrtLevel1 = FALSE;      //son230720
	m_bPrtLevel2 = FALSE;      //son230720
	m_bPrtLevel3 = FALSE;      //son230720
	//}}AFX_DATA_INIT
}
 
void CModeSet21::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSet21)
		DDX_Control(pDX, IDC_CHECK1,    m_check1);
	 	DDX_Control(pDX, IDC_CHECK2,    m_check2);
	 	DDX_Control(pDX, IDC_CHECK3,    m_check3);	 
	 	DDX_Control(pDX, IDC_CHECK4,    m_check4);	 
		DDX_Control(pDX, IDC_CHECK5,    m_check5);
		DDX_Control(pDX, IDC_CHECK6,    m_check6);
		DDX_Control(pDX, IDC_CHECK7,    m_check7);
		DDX_Control(pDX, IDC_CHECK8,    m_check8);
	 	DDX_Control(pDX, IDC_CHECK9,    m_check9);
	 	DDX_Control(pDX, IDC_CHECK10,    m_check10);	 
	 	DDX_Control(pDX, IDC_CHECK11,    m_check11);	 
		DDX_Control(pDX, IDC_CHECK12,    m_check12);
		DDX_Control(pDX, IDC_CHECK13,    m_check13);
		DDX_Control(pDX, IDC_CHECK14,    m_check14);
        DDX_Check(pDX, IDC_CHECK_PRT_BASIC, m_bPrtBasic);      //son230720
        DDX_Check(pDX, IDC_CHECK_PRT_LEVEL1, m_bPrtLevel1);    //son230720
        DDX_Check(pDX, IDC_CHECK_PRT_LEVEL2, m_bPrtLevel2);    //son230720
        DDX_Check(pDX, IDC_CHECK_PRT_LEVEL3, m_bPrtLevel3);    //son230720
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet21, CDialog)
	//{{AFX_MSG_MAP(CModeSet21)
		ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
		ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
		ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	 	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
		ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
		ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
		ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
        ON_BN_CLICKED(IDC_CHECK_PRT_BASIC, OnCheckPrtBasic)     //son230720
        ON_BN_CLICKED(IDC_CHECK_PRT_LEVEL1, OnCheckPrtLevel1)   //son230720
        ON_BN_CLICKED(IDC_CHECK_PRT_LEVEL2, OnCheckPrtLevel2)   //son230720
        ON_BN_CLICKED(IDC_CHECK_PRT_LEVEL3, OnCheckPrtLevel3)   //son230720
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet21 message handlers

BEGIN_EVENTSINK_MAP(CModeSet21, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeSet21, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSet21, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
 	ON_EVENT(CModeSet21, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet21::OnClickOk() 
{
 
	CDialog::OnOK();
}

BOOL CModeSet21::OnInitDialog() 
{
	CDialog::OnInitDialog();    
	Onload(); 
	return 1;
}
 
void CModeSet21::Onload() 
{
	CString str; 
//	char szText[100];
	int a;
 
    a=FileSysInfo01.LoadSaveSet211(_LOAD);//load    //son230720
	if(a!=1){
		AfxMessageBox(" Error No 1212, \n\n  SysSet17.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}

//son230720 begin:
    //g_wPrintState = SysSet211.m_wPrintState;

    if (g_wPrintState & (0x01 << PRT_BASIC))
        //m_bPrtBasic.SetCheck(1);
        m_bPrtBasic = true;
    else
        //m_bPrtBasic.SetCheck(0);
        m_bPrtBasic = false;
    
    if (g_wPrintState & (0x01 << PRT_LEVEL1))
        m_bPrtLevel1 = true;
    else
        m_bPrtLevel1 = false;

    if (g_wPrintState & (0x01 << PRT_LEVEL2))  
        m_bPrtLevel2 = true;
    else
        m_bPrtLevel2 = false;

    if (g_wPrintState & (0x01 << PRT_LEVEL3))  
        m_bPrtLevel3 = true;
    else
        m_bPrtLevel3 = false;

	CString strTemp;
	strTemp.Format("0x%04x", g_wPrintState);
	GetDlgItem(IDC_LABEL_PRT_STATE)->SetWindowText(strTemp);	

    UpdateData(FALSE);  //son 변경된 checkBox 변수를 UI에 반영.
//son230720 end
 

	if(SysSet211.m_nDisable4wRetest==1){// 4w recheck disable
		m_check1.SetCheck(1);
	}else{
		m_check1.SetCheck(0);
	}

	if(SysSet211.m_nSet12==1){
		m_check2.SetCheck(1);
	}else{
		m_check2.SetCheck(0);
	}
	if(SysSet211.m_nSet13==1){
		m_check3.SetCheck(1);
	}else{
		m_check3.SetCheck(0);
	}
	if(SysSet211.m_nSet14==1){
		m_check4.SetCheck(1);
	}else{
		m_check4.SetCheck(0);
	}

	if(SysSet211.m_nSet15==1){
		m_check5.SetCheck(1);
	}else{
		m_check5.SetCheck(0);
	}

	if(SysSet211.m_nSet16==1){//sylee171011-1
		m_check6.SetCheck(1);
	}else{
		m_check6.SetCheck(0);
	}

	if(SysSet211.m_nSet17==1){//sylee171011-1
		m_check7.SetCheck(1);
	}else{
		m_check7.SetCheck(0);
	}


    if(SysSet211.m_nSet21==1){// 4w recheck disable
		m_check8.SetCheck(1);
	}else{
		m_check8.SetCheck(0);
	}

	if(SysSet211.m_nSet22==1){
		m_check9.SetCheck(1);
	}else{
		m_check9.SetCheck(0);
	}
	if(SysSet211.m_nSet23==1){
		m_check10.SetCheck(1);
	}else{
		m_check10.SetCheck(0);
	}

	//son SysSet211.m_nSet24:  Manual-> Set1-> "11) Recipe: OPEN R.Offset" 
	//    이 값이 On 이면 관련 체크박스 m_check11)을 Enable, 아니면 Disable한다.
	if(SysSet211.m_nSet24==1){
		m_check11.SetCheck(1);
	}else{
		m_check11.SetCheck(0);
	}

	if(SysSet211.m_nSet25==1){
		m_check12.SetCheck(1);
	}else{
		m_check12.SetCheck(0);
	}

	if(SysSet211.m_nSet26==1){ 
		m_check13.SetCheck(1);
	}else{
		m_check13.SetCheck(0);
	}

	if(SysSet211.m_nSet27==1){ 
		m_check14.SetCheck(1);
	}else{
		m_check14.SetCheck(0);
	}




    return ; 
}




void CModeSet21::OnSave() 
{ 

	CString str; 

    if(m_check1.GetCheck())
	{
		SysSet211.m_nDisable4wRetest=1;	    //son220503 m_nSet11 -> m_nDisable4wRetest 
	}else{
		SysSet211.m_nDisable4wRetest=0;     //son220503 
	}

    if(m_check2.GetCheck())
	{
		SysSet211.m_nSet12=1;	
	}else{
		SysSet211.m_nSet12=0;
	}

	if(m_check3.GetCheck())
	{
		SysSet211.m_nSet13=1;	
	}else{
		SysSet211.m_nSet13=0;
	}

	if(m_check4.GetCheck())
	{
		SysSet211.m_nSet14=1;	
	}else{
		SysSet211.m_nSet14=0;
	}

	if(m_check5.GetCheck())
	{
		SysSet211.m_nSet15=1;	
	}else{
		SysSet211.m_nSet15=0;
	}

	if(m_check6.GetCheck())  //sylee171011-1
	{
		SysSet211.m_nSet16=1;	
	}else{
		SysSet211.m_nSet16=0;
	}

	if(m_check7.GetCheck())//sylee171011-1
	{
		SysSet211.m_nSet17=1;	
	}else{
		SysSet211.m_nSet17=0;
	}



    if(m_check8.GetCheck())//sylee180517
	{
		SysSet211.m_nSet21=1;	
	}else{
		SysSet211.m_nSet21=0;
	}

    if(m_check9.GetCheck())
	{
		SysSet211.m_nSet22=1;	
	}else{
		SysSet211.m_nSet22=0;
	}

	if(m_check10.GetCheck())
	{
		SysSet211.m_nSet23=1;	
	}else{
		SysSet211.m_nSet23=0;
	}

	//son  체크박스 m_check11)을 Enable이면 
	//     SysSet211.m_nSet24:  Manual-> Set1-> "11) Recipe: OPEN R.Offset" 값을 1로 셋팅
	if(m_check11.GetCheck())
	{
		SysSet211.m_nSet24=1;	
	}else{
		SysSet211.m_nSet24=0;
	}

	if(m_check12.GetCheck())
	{
		SysSet211.m_nSet25=1;	
	}else{
		SysSet211.m_nSet25=0;
	}

	if(m_check13.GetCheck())   
	{
		SysSet211.m_nSet26=1;	
	}else{
		SysSet211.m_nSet26=0;
	}

	if(m_check14.GetCheck()) 
	{
		SysSet211.m_nSet27=1;	
	}else{
		SysSet211.m_nSet27=0;
	}

    SysSet211.m_wPrintState = g_wPrintState;    //son230720

	FileSysInfo01.LoadSaveSet211(_SAVE); //save 	  //son230720

	return ;
}



void CModeSet21::OnCheck1()  //  4W RECHECK DISABLE
{
 

}


void CModeSet21::OnCheck2()  
{
 
}

void CModeSet21::OnCheck3()  
{
 
}

void CModeSet21::OnCheck4()  
{
 
}


void CModeSet21::OnCheck5()  
{
 
}
void CModeSet21::OnCheck6()  //sylee171011-1
{
 
}
void CModeSet21::OnCheck7()  //sylee171011-1
{
 
}

void CModeSet21::OnCheckPrtBasic()  //son230720
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// bit data가 1인 경우에만 bit 위치를 On으로 설정
	if(m_bPrtBasic) 
		g_wPrintState |= (0x01 << PRT_BASIC);

	// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
	else
		g_wPrintState &= ~(0x01 << PRT_BASIC);	

	CString strTemp;
	strTemp.Format("0x%04x", g_wPrintState);
	GetDlgItem(IDC_LABEL_PRT_STATE)->SetWindowText(strTemp);	
	
}

void CModeSet21::OnCheckPrtLevel1()     //son230720
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// bit data가 1인 경우에만 bit 위치를 On으로 설정
	if(m_bPrtLevel1) 
		g_wPrintState |= (0x01 << PRT_LEVEL1);

	// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
	else
		g_wPrintState &= ~(0x01 << PRT_LEVEL1);	
	
	CString strTemp;
	strTemp.Format("0x%04x", g_wPrintState);
	GetDlgItem(IDC_LABEL_PRT_STATE)->SetWindowText(strTemp);	
	
}

void CModeSet21::OnCheckPrtLevel2()     //son230720
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// bit data가 1인 경우에만 bit 위치를 On으로 설정
	if(m_bPrtLevel2) 
		g_wPrintState |= (0x01 << PRT_LEVEL2);

	// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
	else
		g_wPrintState &= ~(0x01 << PRT_LEVEL2);	
	
	CString strTemp;
	strTemp.Format("0x%04x", g_wPrintState);
	GetDlgItem(IDC_LABEL_PRT_STATE)->SetWindowText(strTemp);	
}

void CModeSet21::OnCheckPrtLevel3()     //son230720
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// bit data가 1인 경우에만 bit 위치를 On으로 설정
	if(m_bPrtLevel3) 
		g_wPrintState |= (0x01 << PRT_LEVEL3);

	// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
	else
		g_wPrintState &= ~(0x01 << PRT_LEVEL3);	
	
	
	CString strTemp;
	strTemp.Format("0x%04x", g_wPrintState);
	GetDlgItem(IDC_LABEL_PRT_STATE)->SetWindowText(strTemp);	
	
}
