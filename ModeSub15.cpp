// ModeSub15.cpp : implementation file
//

#include "stdafx.h"


#include "BiOpTroCon.h"
#include "ModeSub15.h"
#include "FileSysInfo.h"

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int  g_nLogOnRecipe1Retest; //sylee170720-1

extern int nFlagEstModelMake;//sylee130325  //estimation 
extern int nFlagEstModelMakeSelect;//sylee130325    //estimation
extern int nFlagEstModelMakeCount;//sylee130325    //estimation


extern int nProOneP_Flag1;//sylee130916  1=load   2= model make   3= run  etc  disable
extern int nProOneP_ModelCount1;//sylee130916
extern int nProOneP_ModelCheck1;//sylee131018
extern int nProOneP_ModelCheck1Retry;//sylee131018
//extern  int nProOneP_ModelSet;//sylee130916

extern short nProOneP_ShortPin[301][1000]; //SYLEE161212   30->101
extern short nProOneP_Short1_Li[5001][2];
extern short nProOneP_Short1[5001];//process
extern int nProOneP_Short1_Co;
extern short nProOneP_Open1[5001][2];//process
extern int nProOneP_Open1_Co;
extern int nProOneP_RunFlag1;//sylee131011


extern CSysInfo05  SysInfo05;
extern CSysInfo05  RunInfo05;
extern CFileSysInfo FileSysInfo01;
extern int nMCon1,	nMCon2, nMCon3 , nMCon4;
 

//extern int nHVCalFlag1;//sylee20120812
//extern int nHVCalFlag1Co1;//sylee20120812
extern double IADClog1[DEF_MAX_PIECE1][100];//sylee20120812 //son220823_6


extern CSysSet13  SysSet13;//SYLEE151207
extern CSysSet16  SysSet16; //SYLEE151207
 extern CSysSet211  SysSet211;  //Pin SETUP   


/////////////////////////////////////////////////////////////////////////////
// CModeSub15 dialog


CModeSub15::CModeSub15(CWnd* pParent )
	: CDialog(CModeSub15::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub15)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub15::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub15)
 		DDX_Control(pDX, IDC_COMBO02, m_combo2);
 		DDX_Control(pDX, IDC_COMBO03, m_combo3);		
		DDX_Control(pDX, IDC_COMBO04, m_combo4);			
		DDX_Control(pDX, IDC_COMBO06, m_combo6);
		DDX_Control(pDX, IDC_COMBO08, m_combo8); 		
		DDX_Control(pDX, IDC_COMBO09, m_combo9); 
		DDX_Control(pDX, IDC_COMBO10, m_combo10);
 		DDX_Control(pDX, IDC_COMBO22, m_combo22);
		DDX_Control(pDX, IDC_COMBO23, m_combo23); 		
		DDX_Control(pDX, IDC_COMBO24, m_combo24); 
     	DDX_Control(pDX, IDC_COMBO25, m_combo25); 
		DDX_Control(pDX, IDC_COMBO26, m_combo26); 
    	DDX_Control(pDX, IDC_COMBO27, m_combo27);
 		DDX_Control(pDX, IDC_COMBO28, m_combo28); 		
 		DDX_Control(pDX, IDC_COMBO30, m_combo30); 
 		DDX_Control(pDX, IDC_COMBO32, m_combo32);
		DDX_Control(pDX, IDC_COMBO33, m_combo33); 		
		DDX_Control(pDX, IDC_COMBO34, m_combo34); 
		DDX_Control(pDX, IDC_COMBO35, m_combo35); 
		DDX_Control(pDX, IDC_COMBO36, m_combo36); 
 		DDX_Control(pDX, IDC_COMBO37, m_combo37);
 		DDX_Control(pDX, IDC_COMBO38, m_combo38);
		DDX_Control(pDX, IDC_COMBO40, m_combo40);
		DDX_Control(pDX, IDC_COMBO44, m_combo44); // hdpark230223
		DDX_Control(pDX, IDC_COMBO45, m_combo45); // hdpark230223
		DDX_Control(pDX, IDC_COMBO46, m_combo46); // hdpark230331
		DDX_Control(pDX, IDC_COMBO47, m_combo47); // hdpark230331
		DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollbar2);
		DDX_Control(pDX, IDC_SCROLLBAR3, m_scrollbar3);
		DDX_Control(pDX, IDC_SCROLLBAR5, m_scrollbar5); 
 		DDX_Control(pDX, IDC_SCROLLBAR6, m_scrollbar6); 
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);
		DDX_Control(pDX, IDC_LABEL3,  m_Label3);	
		DDX_Control(pDX, IDC_LABEL5,  m_Label5);
 		DDX_Control(pDX, IDC_LABEL6,  m_Label6);
    	DDX_Control(pDX, IDC_EDIT1,   m_edit1);
		DDX_Control(pDX, IDC_EDIT2,   m_edit2);//sylee181016-1
		DDX_Control(pDX, IDC_EDIT101,   m_edit101);
		DDX_Control(pDX, IDC_EDIT3,   m_edit3);
		DDX_Control(pDX, IDC_EDIT5,   m_edit5);
	    DDX_Control(pDX, IDC_EDIT02,   m_edit02);
		DDX_Control(pDX, IDC_EDIT57,   m_edit57);
	    DDX_Control(pDX, IDC_EDIT47,   m_edit47);   //SYLEE200709 NEW_SPARK
		DDX_Control(pDX, IDC_EDIT55,   m_edit55);
		DDX_Control(pDX, IDC_EDIT66,   m_edit66);  // hdpark230223
        DDX_Control(pDX, IDC_EDIT60,   m_edit60);//sylee170915-1
	 	DDX_Control(pDX, IDC_CHECK1, m_check1);
	 	DDX_Control(pDX, IDC_CHECK2, m_check2);
		DDX_Control(pDX, IDC_CHECK3, m_check3);     //sylee180331 
	    DDX_Control(pDX, IDC_CHECK4, m_check4);     //sylee181016-1
	    DDX_Control(pDX, IDC_CHECK11, m_check11);   //sylee200511 
	    DDX_Control(pDX, IDC_CHECK5, m_check5);     //son220613 AC ��� 
		DDX_Control(pDX, IDC_COMBO20, m_combo20); 
		DDX_Control(pDX, IDC_COMBO43, m_combo43); 
	    DDX_Control(pDX, IDC_EDIT125,   m_edit125); 
		DDX_Control(pDX, IDC_LABEL1001,  m_Label1001);
		DDX_Control(pDX, IDC_LABEL1002,  m_Label1002);
		DDX_Control(pDX, IDC_LABEL1003,  m_Label1003);
 		DDX_Control(pDX, IDC_LABEL1004,  m_Label1004);
		DDX_Control(pDX, IDC_LABEL1006,  m_Label1006);
		DDX_Control(pDX, IDC_LABEL1007,  m_Label1007);
		DDX_Control(pDX, IDC_LABEL1008,  m_Label1008);
 		DDX_Control(pDX, IDC_LABEL1009,  m_Label1009);
  		DDX_Control(pDX, IDC_LABEL1010,  m_Label1010);		
		DDX_Control(pDX, IDC_LABEL1011,  m_Label1011);
		DDX_Control(pDX, IDC_LABEL1012,  m_Label1012);
		DDX_Control(pDX, IDC_LABEL1013,  m_Label1013);
 		DDX_Control(pDX, IDC_LABEL1014,  m_Label1014);
  		DDX_Control(pDX, IDC_LABEL1015,  m_Label1015);
		DDX_Control(pDX, IDC_LABEL1016,  m_Label1016);
		DDX_Control(pDX, IDC_LABEL1017,  m_Label1017);
		DDX_Control(pDX, IDC_LABEL1018,  m_Label1018);
 		DDX_Control(pDX, IDC_LABEL1019,  m_Label1019);
  		DDX_Control(pDX, IDC_LABEL1020,  m_Label1020);
		DDX_Control(pDX, IDC_LABEL1021,  m_Label1021);
		DDX_Control(pDX, IDC_LABEL1022,  m_Label1022);
		DDX_Control(pDX, IDC_LABEL1023,  m_Label1023);
		DDX_Control(pDX, IDC_LABEL1026,  m_Label1026);
		DDX_Control(pDX, IDC_LABEL1047,  m_Label1047); //hdpark230412
		DDX_Control(pDX, IDC_LABEL1046,  m_Label1046);	//hdpark230412
		DDX_Control(pDX, IDC_LABEL1027,  m_Label1027);
		DDX_Control(pDX, IDC_LABEL1028,  m_Label1028);
 		DDX_Control(pDX, IDC_LABEL1029,  m_Label1029);
//DDX_Control(pDX, IDC_LABEL1030,  m_Label1030);  //son220613 hwnd�� 0�̾ SW_HIDE�ϴϱ� DDX_Control() �̺κп��� ����.
		DDX_Control(pDX, IDC_LABEL1032,  m_Label1032);
		DDX_Control(pDX, IDC_BLOAD,  m_button1001);
		DDX_Control(pDX, IDC_BSAVE,  m_button1002);
		DDX_Control(pDX, IDC_OK,  m_button1003);
		DDX_Control(pDX, IDC_LABEL1036,  m_LabelCurrent);   //son220630 ���� Label 

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub15, CDialog)
	//{{AFX_MSG_MAP(CModeSub15)
		ON_BN_CLICKED(IDC_CHECK1, OnCheck1) 
 		ON_BN_CLICKED(IDC_CHECK2, OnCheck2) 
		ON_BN_CLICKED(IDC_CHECK3, OnCheck3) //sylee180331
		ON_BN_CLICKED(IDC_CHECK4, OnCheck4) //sylee181016-1
	    ON_WM_HSCROLL()
    	ON_BN_CLICKED(IDC_CHECK11, OnCheck11) //sylee200222
    	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)     //son220613 AC ���
    	ON_CBN_SELCHANGE(IDC_COMBO22, OnSelchangeCombo22)       //son221227
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////
// CModeSub15 message handlers


BEGIN_EVENTSINK_MAP(CModeSub15, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub15) 	
    ON_EVENT(CModeSub15, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE) 
	ON_EVENT(CModeSub15, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub15, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE) 
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CModeSub15::OnInitDialog() 
{
	CDialog::OnInitDialog(); 

	m_scrollbar2.SetScrollRange(1, 2000, FALSE);
	m_scrollbar3.SetScrollRange(1, 100, FALSE);
 
	m_scrollbar5.SetScrollRange(0, 10, FALSE); 
	m_scrollbar6.SetScrollRange(0, 30, FALSE);


//son221226 begin:
    //---------------------------------------
    // Recipe Open V �޺� �ʱ�ȭ                    
    CList_Recipe_Open_V  recipe_OpenV;
    m_combo22.ResetContent();    //son230328 
    for (int openV=0; openV < RECP_OPENV_NUM; openV++)
    {
        m_combo22.InsertString(-1, recipe_OpenV.strList[openV]); 
    }
	m_combo22.SetCurSel(SysInfo05.m_nOpenV); 

//son221226 end

    //-------------------------------------------------------
    //son220705 begin: Recipe  R �޺��ڽ� �߱��� ohm ǥ�� ��� 

    //son Recipe Open R �޺��ڽ� 
    CList_Recipe_Open_R  recipe_OpenR;
    m_combo2.ResetContent();    //son230328
    for (int openR=0; openR < RECP_OPENR_NUM; openR++)
    {
        m_combo2.InsertString(-1, recipe_OpenR.strList[openR]); 
    }
	m_combo2.SetCurSel(SysInfo05.m_nCon); 

    //son Recipe uShort R �޺��ڽ� 
    CList_Recipe_uShortR  recipe_uShortR;
    m_combo9.ResetContent();    //son230328
    for (int uShortR=0; uShortR < RECP_uShortR_NUM; uShortR++)
    {
        m_combo9.InsertString(-1, recipe_uShortR.strList[uShortR]); 
    }
	m_combo9.SetCurSel(SysInfo05.m_nShortR);    

    //son Recipe LShort R �޺��ڽ� 
    CList_Recipe_LShortR  recipe_LShortR;
    m_combo10.ResetContent();    //son230328
    for (int LShortR=0; LShortR < RECP_LShortR_NUM; LShortR++)
    {
        m_combo10.InsertString(-1, recipe_LShortR.strList[LShortR]); 
    }
	m_combo10.SetCurSel(SysInfo05.m_nShortR2);    

    //son Recipe HR Short R �޺��ڽ� 
    CList_Recipe_HRShortR  recipe_HRShortR;
    m_combo8.ResetContent();    //son230328
    for (int HRShortR=0; HRShortR < RECP_HRShortR_NUM; HRShortR++)
    {
        m_combo8.InsertString(-1, recipe_HRShortR.strList[HRShortR]); 
    }
	m_combo8.SetCurSel(SysInfo05.m_nHR);    

    //son Recipe Leak1, Leak2 R �޺��ڽ� 
    CList_Recipe_LEAK_R  recipe_LeakR;
    m_combo3.ResetContent();    //son230328
    m_combo43.ResetContent();    //son230328
    for (int leak=0; leak < RECP_LEAK_NUM; leak++)
    {
        m_combo3.InsertString(-1,  recipe_LeakR.strList[leak]); 
        m_combo43.InsertString(-1, recipe_LeakR.strList[leak]); 
    }
	m_combo3.SetCurSel(SysInfo05.m_nHv1R);    
	m_combo43.SetCurSel(SysInfo05.m_nHv2R);    
    //son220705 end


    Onload();   

	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928
	A_Language() ;

	GetDlgItem(IDC_SCROLLBAR5)->EnableWindow(TRUE);//sylee170720-1
	GetDlgItem(IDC_LABEL5)->EnableWindow(TRUE);//sylee170720-1
	if(  g_nLogOnRecipe1Retest==2  && SysSet13.m_nSet17==1 && SysSet13.m_nSet19==1){  //sylee170720-1
		GetDlgItem(IDC_SCROLLBAR5)->EnableWindow(FALSE);//sylee170720-1
		GetDlgItem(IDC_LABEL5)->EnableWindow(FALSE);//sylee170720-1		
	}
  
//son220609 GTS�� 22�� 6�� 9�� �Ͻ������� ������ ���� ����ȭ �޺��ڽ��� ���̵��� SW_HIDE ����� ����.
//          ��������ȭ ���� ����� ���������� �ʴ´�. __MULTI_CURRENT__ define�� GTS���� ����.
//son220902 MST�� ���������� ��������ȭ �޺��ڽ� Hide����� �־�� ��.
//#if (defined(__GTS__CUSTOMER) && !defined(__MST__CUSTOMER))
#ifdef __GTS__CUSTOMER //hdpark230412
		m_combo44.ResetContent();
		m_combo44.AddString("Voltage");
		m_combo44.SetCurSel(0);

		GetDlgItem(IDC_COMBO45)->ShowWindow(SW_HIDE);

	//	GetDlgItem(IDC_LABEL1046)->ShowWindow(SW_HIDE);  //sylee230426-2
	//sylee230426-2
		GetDlgItem(IDC_LABEL1046)->SetWindowText("");

		GetDlgItem(IDC_COMBO46)->ShowWindow(SW_HIDE);	// spark volt, current select
		GetDlgItem(IDC_COMBO47)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LABEL1048)->SetWindowText("");

#else
    //son ��������ȭ ����� ON�� �ƴϸ� Recipe�� ��������ȭ �޺��ڽ��� �����.
	if( SysSet13.m_nSet1 !=1 )
	{
		GetDlgItem(IDC_COMBO25)->ShowWindow(SW_HIDE);//sylee181008-5
		GetDlgItem(IDC_COMBO27)->ShowWindow(SW_HIDE);//sylee181008-5 
		GetDlgItem(IDC_COMBO28)->ShowWindow(SW_HIDE);//sylee181008-5 
		GetDlgItem(IDC_COMBO30)->ShowWindow(SW_HIDE);//sylee181008-5 
		//m_LabelCurrent.ShowWindow(SW_HIDE); //son220630 "Current" Lable Disable -> SW_HIDE�ϴٰ� �״� �����־ ������.
		m_LabelCurrent.SetWindowText("");     //son220630 SW_HIDE�ϴٰ� �״� ���� ������ ��ĭ���� �����ϴ� ������ �ٲ�. 
	}
#endif

//yklee240426 [ACE-500 Spark Current�� �׻� ����� ����-[ACE-400 SST�� ���ܻ���]]
// hdpark230331 begin 
#ifndef SPARK_CURRENT_DETECTION
	m_combo44.ResetContent();
	m_combo44.AddString("Voltage");
	m_combo44.SetCurSel(0); //yklee comment: Error Log������ Voltage Detect���� �˾ƾ� ���� ǥ�� �ϹǷ� 0[����] ���� �ʿ�
	
	GetDlgItem(IDC_COMBO44)->ShowWindow(SW_HIDE);		//yklee240426  SPARK Combo Box ����-ACE500
	GetDlgItem(IDC_LABEL1047)->SetWindowText("TBD");		//yklee240426  SPARK Detection Label->TBD ����-ACE500
	GetDlgItem(IDC_COMBO45)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO46)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO47)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL1046)->SetWindowText("TBD");
	GetDlgItem(IDC_LABEL1048)->SetWindowText("");
#else
	m_combo44.ResetContent();
	m_combo44.AddString("Volt & Current");
	m_combo44.SetCurSel(0);
#endif
// hdpark230331 end 
//yklee240426 [ACE-500 Spark Current�� �׻� ����� ����-[ACE-400 SST�� ���ܻ���]]

	//���� ����ȭ ��� On �� �� action.
	if( SysSet13.m_nSet1 ==1 )//son220902 else -> 	if( SysSet13.m_nSet1 ==1 )�� ����
    {
        if( gDef_Language1==3)
	        m_LabelCurrent.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 148));     //son220630 ���� Label
        else 
	        m_LabelCurrent.SetWindowText("Current");     //son220630 ���� Label
    }



//son220705 begin
//son GTS Recipeȭ���� ohm ǥ�ø� �߱��� ������  ohm ��ȣ�� ǥ��
//#ifdef __GTS__CUSTOMER
#if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
    GetDlgItem(IDC_LABEL1040)->SetWindowText("��");    //son220704
    GetDlgItem(IDC_LABEL1041)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1042)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1043)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1044)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1045)->SetWindowText("K��");   
#else
    GetDlgItem(IDC_LABEL1040)->SetWindowText("��");    //son220704 
    GetDlgItem(IDC_LABEL1041)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1042)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1043)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1044)->SetWindowText("K��");   
    GetDlgItem(IDC_LABEL1045)->SetWindowText("K��");   
#endif

//son ohm���� ǥ�ñ�� On �϶� (ex: �����̽þ� ����)
#ifdef OHM
    GetDlgItem(IDC_LABEL1040)->SetWindowText("ohm");    //son220704
    GetDlgItem(IDC_LABEL1041)->SetWindowText("Kohm");   
    GetDlgItem(IDC_LABEL1042)->SetWindowText("Kohm");   
    GetDlgItem(IDC_LABEL1043)->SetWindowText("Kohm");   
    GetDlgItem(IDC_LABEL1044)->SetWindowText("Kohm");   
    GetDlgItem(IDC_LABEL1045)->SetWindowText("Kohm");   
#endif
//son220705 end


	//son OpenOffset ��� SysInfo ������ Off��� ���� edit box,  checkBox,  Lable�� �Ⱥ��̰� ó���Ѵ�
	if( SysSet211.m_nSet24 !=1 ){//sylee181016-1
		GetDlgItem(IDC_CHECK4)->ShowWindow(SW_HIDE);    //son "Open Offest" check box Disable 
 		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);     //son "Open Offset" ��  editbox Disable 
		GetDlgItem(IDC_LABEL1100)->ShowWindow(SW_HIDE); //son "Open Offet" Lable Disable 
	}

    //son220620 ����� ���û������� ��� ���翡 ���ؼ� OPEN Near üũ�ڽ� ���� 
	GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);    //son220613 "Open Near" check box Disable 
	GetDlgItem(IDC_LABEL1030)->ShowWindow(SW_HIDE); //son220613 "Open Near" Lable Disable 

#ifndef __CCTC__CUSTOMER //sylee230426-2
//    GetDlgItem(IDC_LABEL1038)->ShowWindow(SW_HIDE); //sylee230426-2
#endif

#if defined(__GTS__CUSTOMER) || defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) || defined(__BH__CUSTOMER) 	// hdpark231205  //sylee241204-2
//#ifdef __GTS__CUSTOMER	
    //son220613 GTS�϶����� OPEN Near üũ�ڽ�  ����
	//GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);    //son220613 "Open Near" check box Disable 
	//GetDlgItem(IDC_LABEL1030)->ShowWindow(SW_HIDE); //son220613 "Open Near" Lable Disable 
#else
    //son FM20 ��Ʈ���� ó���� ������ ���� ���̵� �״�� ��� â ���� �� �״� ������ ����.
    //    FM20 ��Ʈ���� DDX ��ϵ� �������� ����� �Ѵ�.
    //    Ư�� FM20 ��Ʈ�� ���� SW_HIDE �Ϸ��� DDX_Control ������� ���� �Ѵ�. 
	GetDlgItem(IDC_CHECK5)->ShowWindow(SW_HIDE);    //son220610 "AC" check box Disable 
//sylee230426-2	
	GetDlgItem(IDC_LABEL1038)->ShowWindow(SW_HIDE); //son220610 "AC" Lable Disable 
#endif


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



 


BOOL CModeSub15::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
 /*
	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_ESCAPE)) {
		return TRUE;
	}
*/
 	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_RETURN)) {
 //	 	OnOK();
		return TRUE;
	}
 
	return CDialog::PreTranslateMessage(pMsg);
}


void CModeSub15::OnCheck1()  
{
	if(m_check1.GetCheck())
	{
//		nHVCalFlag1  = 1;
		SysInfo05.m_nHVCal1=1;
	}
	else
	{
//		nHVCalFlag1  = 0; 
		SysInfo05.m_nHVCal1=0;
	}
	m_check1.SetCheck(SysInfo05.m_nHVCal1); 
//	nHVCalFlag1Co1=0;//sylee20120812
	
	::ZeroMemory (&IADClog1, sizeof(IADClog1)); 
}

void CModeSub15::OnCheck2()  
{
	if(m_check2.GetCheck())
	{	 
		SysInfo05.m_nOpenMea1=1;	//sylee211122 m_nUniversal->m_nOpenMea1
	}
	else
	{	 
		SysInfo05.m_nOpenMea1=0;	//sylee211122 m_nUniversal->m_nOpenMea1
	}
	m_check2.SetCheck(SysInfo05.m_nOpenMea1);  	//sylee211122 m_nUniversal->m_nOpenMea1
}


void CModeSub15::OnCheck3()  //sylee180331-1
{
	if(m_check3.GetCheck())
	{	 
		SysInfo05.m_nShortDisable1=7;  //sylee180331-1 
	}else{	 
		SysInfo05.m_nShortDisable1=0;
	}
	m_check3.SetCheck(SysInfo05.m_nShortDisable1);  
}

//son 'Open Offset' CheckBox
void CModeSub15::OnCheck4()  //sylee181016-1
{
	if(m_check4.GetCheck())//sylee181016-1
	{	 
		SysInfo05.m_nCC[3]=1;  
	}else{	 
		SysInfo05.m_nCC[3]=0;
	}
	m_check4.SetCheck(SysInfo05.m_nCC[3]);  //sylee181016-1
}

void CModeSub15::OnCheck11()    //sylee200222
{
	if(m_check11.GetCheck()) 
	{	 
		SysInfo05.m_4W_Measure=1;  
	}else{	 
		SysInfo05.m_4W_Measure=0;
	}
	m_check11.SetCheck(SysInfo05.m_4W_Measure);  //sylee200222 
	
}

int CModeSub15::OnSaveInterlock() //sylee140929
{ 


   return 1;

	int nTmp1,nTmp2;
	nTmp1= m_combo4.GetCurSel();
	nTmp2= m_combo20.GetCurSel();

	if( nTmp2 <=1){
	    AfxMessageBox("   Error 1501   \n\n  leak2 voltage set please. \n\n ", MB_OK );
		return 0;
	} 

	if( nTmp1 >nTmp2){
	    AfxMessageBox("   Error 1502   \n\n   leak1  voltage   <  leak2 voltage   set please. \n\n ", MB_OK );
		return 0;
	}
	return 1;
}


void CModeSub15::OnClickOk() 
{ 
	 CString str;
	 int nRet1;

	 str.Format("  \n Save ?   \n " ); //sylee130215joytechrequest
	 nRet1=AfxMessageBox(str,MB_YESNO | MB_ICONEXCLAMATION);  //- IDOK : 1- IDCANCEL : 2/- IDABORT : 3- IDRETRY : 2- IDIGNORE : 5- IDYES : 6- IDNO : 7
  
     if(nRet1==IDYES){
		 nRet1=OnSaveInterlock();//sylee140929
		 if(nRet1==0){//sylee140929
			 AfxMessageBox("   Save Error ! ", MB_OK );
			 return ;//sylee140929
		 }//sylee140929

         //son "\SETUP2\Recipe.ini"�� Save(type=1)
		 FileSysInfo01.LoadSaveSub15(_SAVE);	 //son220705
	 }

    //son "\SETUP2\Recipe.ini"�� struct CSysInfo05�� Load(type=2)
	FileSysInfo01.LoadSaveSub15(_LOAD);			//son220705

#if 0  //son220705 begin: Recipe Open R �޺��ڽ� �߱��� ohm ǥ�� ��� �߰�. 
//son220705_2 �̻�� �ڵ��̹Ƿ� ����
	//if( SysInfo05.m_nCon ==1) {
	//	nMCon1=5;
	//}else 	if( SysInfo05.m_nCon ==2) {
	//	nMCon1=10;
	//}else 	if( SysInfo05.m_nCon ==3) {
	//	nMCon1=20;
	//}else 	if( SysInfo05.m_nCon ==4) {
	//	nMCon1=30;

	//}else 	if( SysInfo05.m_nCon ==5) {
	//	nMCon1=40;
	//}else 	if( SysInfo05.m_nCon ==6) {
	//	nMCon1=50;
	//}else 	if( SysInfo05.m_nCon ==7) {
	//	nMCon1=60;
	//}else 	if( SysInfo05.m_nCon ==8) {
	//	nMCon1=70;
	//}else 	if( SysInfo05.m_nCon ==9) {
	//	nMCon1=80;

	//}else 	if( SysInfo05.m_nCon ==10) {
	//	nMCon1=90;
	//}else 	if( SysInfo05.m_nCon ==11) {
	//	nMCon1=100;
	//}else 	if( SysInfo05.m_nCon ==12) {
	//	nMCon1=250;
	//}else 	if( SysInfo05.m_nCon ==13) {
	//	nMCon1=500;
	//}else 	if( SysInfo05.m_nCon ==14) {
	//	nMCon1=1000;

	//}else 	if( SysInfo05.m_nCon ==15) {
	//	nMCon1=2000;
	//}else 	if( SysInfo05.m_nCon ==16) {
	//	nMCon1=5000;
	//}else 	if( SysInfo05.m_nCon ==17) {
	//	nMCon1=10000;
	//}else 	if( SysInfo05.m_nCon ==18) {
	//	nMCon1=20000;
	//}else if( SysInfo05.m_nCon ==19) {//user
	//    nMCon1=SysInfo05.m_nConU;

 	//}else{
	//	nMCon1=0;
	//}	 
#else
//son220705_2 �̻�� �ڵ��̹Ƿ� ����
    //CList_Recipe_Open_R  recipe_OpenR;
    //nMCon1 = recipe_OpenR.nOpenR[SysInfo05.m_nCon];     
    //if( SysInfo05.m_nCon == RECP_OPENR_USER) //user     
    //{
    //    if( SysInfo05.m_nConU > MAX_OPEN_USER_R)    //son MAX_OPEN_USER_R:100000
    //        nMCon1=0;
    //    else
    //	    nMCon1=SysInfo05.m_nConU; 
    //}

	//if (SysInfo05.m_nCon < RECP_OPENR_5ohm || SysInfo05.m_nCon > RECP_OPENR_USER)
	//	nMCon1=0;

#endif  //son220705 end
 
#if 0

    //son210324 SysInfo05.m_nConIs-> SysInfo05.m_nHv1R �� �ٲٸ鼭 �Ʒ� �ڵ�� �ڸ�Ʈ ó����.
	if( SysInfo05.m_nConIs ==1) {
		nMCon2=1000;
	}else 	if( SysInfo05.m_nConIs ==2) {
		nMCon2=2000;
	}else 	if( SysInfo05.m_nConIs ==3) {
		nMCon2=3000;
	}else 	if( SysInfo05.m_nConIs ==4) {
		nMCon2=4000;
	}else 	if( SysInfo05.m_nConIs ==5) {
		nMCon2=5000;
	}else 	if( SysInfo05.m_nConIs ==6) {
		nMCon2=6000 ;
	}else 	if( SysInfo05.m_nConIs ==7) {
		nMCon2=7000;
	}else 	if( SysInfo05.m_nConIs ==8) {
		nMCon2=8000;
	}else 	if( SysInfo05.m_nConIs ==9) {
		nMCon2=9000;
	}else 	if( SysInfo05.m_nConIs ==10) {
		nMCon2=10000;
	}else 	if( SysInfo05.m_nConIs ==11) {
		nMCon2=20000;
	}else 	if( SysInfo05.m_nConIs ==12) {
		nMCon2=SysInfo05.m_nConIs;
	}else{
    	nMCon2=0;
	}
#endif

	CDialog::OnOK();
}
 

void CModeSub15::OnSave() 
{ 
	char szText[100];
	int nTmp1;

   nTmp1=OnSaveInterlock(); //sylee140929
   if( nTmp1==0){
	   return ;
   }

 //sylee170622-2   SysInfo05.m_nTemp1 = m_combo1.GetCurSel();  //sylee160518
//     SysInfo05.m_nShortDisable1 =0;   //sylee170622-2  m_nShortDisable1; //sylee180331-1 
    SysInfo05.m_nCon = m_combo2.GetCurSel();
	SysInfo05.m_nHv1R = m_combo3.GetCurSel();  //son m_combo3  HV(Leak1) R ������.  //son210324 m_nConIs-> m_nHv1R
    SysInfo05.m_nHv = m_combo4.GetCurSel(); 
    SysInfo05.m_nSp = m_combo6.GetCurSel(); 
    SysInfo05.m_nHR= m_combo8.GetCurSel();
    SysInfo05.m_nShortR = m_combo9.GetCurSel();
    SysInfo05.m_nShortR2 = m_combo10.GetCurSel();
    SysInfo05.m_nOpenV = m_combo22.GetCurSel();
    SysInfo05.m_nShort1V= m_combo23.GetCurSel();
    SysInfo05.m_nShort2V = m_combo24.GetCurSel();
  //  SysInfo05.m_nDual =  m_combo25.GetCurSel();
    SysInfo05.m_nHv2 = m_combo20.GetCurSel(); //sylee140923
    SysInfo05.m_nHv2R = m_combo43.GetCurSel(); //sylee140923    //son (HV Leak2 R ������
    SysInfo05.m_nSparkDetection[0] = m_combo44.GetCurSel();  // hdpark230331
#ifndef SPARK_CURRENT_DETECTION
	SysInfo05.m_nSparkDetection[1] = 0;  // hdpark230331
	SysInfo05.m_nSparkDetection[2] = 0;  // hdpark230331
	SysInfo05.m_nSparkCurrentSet = 4;  // hdpark230223
#else
    SysInfo05.m_nSparkDetection[1] = m_combo46.GetCurSel();  // hdpark230331
    SysInfo05.m_nSparkDetection[2] = m_combo47.GetCurSel();  // hdpark230331
    SysInfo05.m_nSparkCurrentSet = m_combo45.GetCurSel();  // hdpark230223
#endif


	switch(m_combo45.GetCurSel())
	{
	case 0 : SysInfo05.m_dSparkCurrentSet = 1.0;	break;
	case 1 : SysInfo05.m_dSparkCurrentSet = 2.0;	break;
	case 2 : SysInfo05.m_dSparkCurrentSet = 4.0;	break;
	case 3 : SysInfo05.m_dSparkCurrentSet = 6.0;	break;
	case 4 : SysInfo05.m_dSparkCurrentSet = 10.0;	break;
	case 5 : SysInfo05.m_dSparkCurrentSet = 15.0;	break;
	case 6 : SysInfo05.m_dSparkCurrentSet = 20.0;	break;
	}

//sylee170915-1 	SysInfo05.m_nDual =  1;//sylee161121

	SysInfo05.m_n4WErrPerLimit =  0;//sylee170915-1 

    SysInfo05.m_nHRV = m_combo26.GetCurSel();  	

	SysInfo05.m_nLeak1_Current = m_combo25.GetCurSel()+1;//sylee181005-3    //son220706
    SysInfo05.m_nLeak2_Current = m_combo27.GetCurSel()+1;//sylee181005-3    //son220706
	SysInfo05.m_nOpen_Current = m_combo28.GetCurSel()+1;//sylee180531       //son220706
    SysInfo05.m_nShort_Current = m_combo30.GetCurSel()+1;//sylee180531      //son220706

	SysInfo05.m_nCC[3] = m_check4.GetCheck();//sylee181016-1    //son Open Offset CheckBox
	m_edit2.GetWindowText( szText,10);  
	SysInfo05.m_nCC[4] = atoi( szText );            //son m_nCC[4]: m_edit2.  Open offset  Value 
	if( SysInfo05.m_nCC[4]>50){//sylee181016-1      //son Open Offset ���� (m_edit2)�� 0 �̻� 50���Ͽ��� �Ѵ�.
		SysInfo05.m_nCC[4]=50;
	}
	if( SysInfo05.m_nCC[4]<5){//sylee181016-1
		SysInfo05.m_nCC[4]=0;
	}
	sprintf(szText, "%d",  SysInfo05.m_nCC[4]);//sylee181016-1
    m_edit2.SetWindowText(szText); 
		
    //son SysInfo05.m_nCC[3]: m_check4 (Open Offset Enable ǥ��) 
    if(SysInfo05.m_nCC[3]!=1){  //SYLEE181016-1
        SysInfo05.m_nCC[3]=0;
    } 

	//son Open Offset Enable�� �ƴ϶�� Open Offset ���� 0���� Clear�Ѵ�. 
	if(SysInfo05.m_nCC[3]!=1){//SYLEE181016-1
		SysInfo05.m_nCC[4]=0;
	}
	//son Open Offeet ���� 1���� �۴ٸ� Open Offset Enable ǥ�ø�  Disable�� �ٲ۴�.
    if(SysInfo05.m_nCC[4]<1){//SYLEE181016-1
		SysInfo05.m_nCC[3]=0;
	}

	//son "Open Recipe"�� 12(200 ohm) �̻��̸� Open Offset ����� ���� �ʴ´�.
	//    ��, Open Recepe  2(10 ohm) ~ 11(100 ohm) ���̿����� ����Ѵ�.
	//if(SysInfo05.m_nCon> 11 &&  SysInfo05.m_nCon< 25){//sylee181016-1   
	if(SysInfo05.m_nCon> RECP_OPENR_100ohm &&  SysInfo05.m_nCon< RECP_OPENR_END){   //son220705
		SysInfo05.m_nCC[3]=0;
		SysInfo05.m_nCC[4]=0;
	}
	//son "Open Recipe" User ��� �϶��� m_edit1���� �����ͼ� ����ϰ� 
	//     100�̻� �����ȴٸ� Open Offset�� ������� �ʴ´�.
    if(SysInfo05.m_nCon==RECP_OPENR_USER)//sylee181016-1   //son220705 25:RECP_OPENR_USER
    {
		m_edit1.GetWindowText( szText,10); 	
		SysInfo05.m_nConU = atoi( szText ); //open

		if(SysInfo05.m_nConU>100){//sylee181016-1
			 SysInfo05.m_nCC[3]=0;
		     SysInfo05.m_nCC[4]=0;
		}
	}

    SysInfo05.m_nStep[1] = m_combo32.GetCurSel();    
	SysInfo05.m_nStep[2] = m_combo33.GetCurSel();
	if(SysInfo05.m_nStep[2]>=1){//SYLEE140207
		SysInfo05.m_nStep[2] =SysInfo05.m_nStep[2] +1;
	}
	
	SysInfo05.m_nStep[4] = m_combo35.GetCurSel();
	if(SysInfo05.m_nStep[4]>=1){//SYLEE140207
		SysInfo05.m_nStep[4] =SysInfo05.m_nStep[4] +1;
	}

	 SysInfo05.m_nStep[3] = m_combo34.GetCurSel();   //SYLEE231128
#ifdef __RECIPE_LEAK_MORE1  //sylee231016
   //SYLEE231128 SysInfo05.m_nStep[3] = m_combo34.GetCurSel();   
    if(SysInfo05.m_nStep[3]>=1){    //SYLEE140207
        SysInfo05.m_nStep[3] =SysInfo05.m_nStep[3] +1;  
    }
    SysInfo05.m_nStep[5] = m_combo36.GetCurSel();   
    if(SysInfo05.m_nStep[5]>=1){    
        SysInfo05.m_nStep[5] =SysInfo05.m_nStep[5] +1;  
    }
    SysInfo05.m_nStep[6] = m_combo37.GetCurSel();   
    if(SysInfo05.m_nStep[6]>=1){    
        SysInfo05.m_nStep[6] =SysInfo05.m_nStep[6] +1;  
    }
    SysInfo05.m_nStep[7] = m_combo38.GetCurSel();   
    if(SysInfo05.m_nStep[7]>=1){    
        SysInfo05.m_nStep[7] =SysInfo05.m_nStep[7] +1;  
    }      
#else
	
    if(SysInfo05.m_nStep[3]>=1){        //SYLEE140207 
        SysInfo05.m_nStep[3] =SysInfo05.m_nStep[3] +4;  
    }
    SysInfo05.m_nStep[5] = m_combo36.GetCurSel();
    SysInfo05.m_nStep[6] = m_combo37.GetCurSel();
    SysInfo05.m_nStep[7] = m_combo38.GetCurSel();      
#endif  


	//SysInfo05.m_nProSpMode = m_combo39.GetCurSel() + 2 ;//sylee140415
	SysInfo05.m_nProSpMode = 4 ;//sylee161121

    if( SysInfo05.m_nProSpMode==3 ){ //sylee150520
        SysInfo05.m_nProSpMode=2;
    }

    if(  SysInfo05.m_nProSpMode !=3){ //sylee150407
        nFlagEstModelMake=0;
        nFlagEstModelMakeSelect=0 ;
    } 

    SysInfo05.m_n4WSet1 = m_combo40.GetCurSel(); 
    SysInfo05.m_nHVCal1 = m_check1.GetCheck();


    SysInfo05.m_4W_Measure=m_check11.GetCheck();    //sylee150520-4W�����м�

	if(SysInfo05.m_nHv2<SysInfo05.m_nHv){//SYLEE161208
#ifndef __QUAD_LEAK1_LEAK2__  //son231122
    	SysInfo05.m_nHv2=SysInfo05.m_nHv;
#else  //son231122
	//sylee230807	SysInfo05.m_nHv2=SysInfo05.m_nHv;
#endif  //son231122

		m_combo20.SetCurSel(SysInfo05.m_nHv2);
	}


	m_edit125.GetWindowText( szText,10); //sylee140923	
	SysInfo05.m_nHv2RU = atoi( szText ); //sylee140923

 	nProOneP_ModelCount1=0;	 //sylee131017  //sylee131221
	::ZeroMemory(&nProOneP_Short1_Co, sizeof(nProOneP_Short1_Co));
	::ZeroMemory(&nProOneP_Short1_Li, sizeof(nProOneP_Short1_Li));
	::ZeroMemory(&nProOneP_Open1, sizeof(nProOneP_Open1));
	::ZeroMemory(&nProOneP_Short1, sizeof(nProOneP_Short1));//sylee131012
	
 	//sylee131021
    if(SysInfo05.m_nProSpMode==2 ||  SysInfo05.m_nProSpMode==4){//sylee130916
        nProOneP_Flag1=1;//model start
        nProOneP_RunFlag1=1;
    }else{
        nProOneP_Flag1=0;
        nProOneP_RunFlag1=0;
    } 

    //--------------------------------------
    //son Open UserR (ohm ����)
   	m_edit1.GetWindowText( szText,10); 	
	SysInfo05.m_nConU = atoi( szText ); //open
	if( SysInfo05.m_nConU> MAX_OPEN_USER_R) {//sylee161208-2  //son220705 100000: MAX_OPEN_USER_R (100 Khom). 
		SysInfo05.m_nConU= MAX_OPEN_USER_R;               //son220705
	    //son220922 sprintf(szText, "%d",  SysInfo05.m_nConU); //son OnSave() ���������� Onload()�� ȣ���ϹǷ� ���ڵ� �ʿ� ����.
        //son220922 m_edit101.SetWindowText(szText);
 	}
	
    //--------------------------------------
	//son LEAK1 UserR (Kohm ����)
   	m_edit101.GetWindowText( szText,10); 	
	SysInfo05.m_nHv1RU = atoi( szText ); //leak 1             //son210324 m_nConIsU-> m_nHv1RU for Leak1
    if( SysInfo05.m_nHv1RU> MAX_LEAK_USER_R){//sylee161208-2  //son220922 200000 -> MAX_LEAK_USER_R(100000: 100 Mohm)
        SysInfo05.m_nHv1RU= MAX_LEAK_USER_R;  //sylee180206   //son220922
        //son220922 sprintf(szText, "%d",  SysInfo05.m_nHv1RU);
        //son220922 m_edit101.SetWindowText(szText);
    }
   
    //--------------------------------------
    //son LEAK2 UserR (Kohm ����)
   	m_edit125.GetWindowText( szText,10); 	
	SysInfo05.m_nHv2RU = atoi( szText ); //leak 2
    if( SysInfo05.m_nHv2RU> MAX_LEAK_USER_R){//sylee161208-2  //SYLEE180205 //son220922 200000 -> MAX_LEAK_USER_R(100000: 100 Mohm)
		SysInfo05.m_nHv2RU= MAX_LEAK_USER_R;  //sylee180206  //son220922
	    //son220922 sprintf(szText, "%d",  SysInfo05.m_nHv2RU);
        //son220922 m_edit101.SetWindowText(szText);
 	}
 
    //--------------------------------------
    //son uShort UserR (Kohm ����)
   	m_edit02.GetWindowText( szText,10);  
	SysInfo05.m_nShortRU = atoi( szText ); //ushort
	if( SysInfo05.m_nShortRU> MAX_USHORT_USER_R){//sylee161208-2       //son220922 200: MAX_USHORT_USER_R
		SysInfo05.m_nShortRU= MAX_USHORT_USER_R;            //son220922
	    //son220922 sprintf(szText, "%d",  SysInfo05.m_nShortRU);
        //son220922 m_edit101.SetWindowText(szText);
 	}

    //--------------------------------------
    //son HR UserR (Kohm ����)
   	m_edit5.GetWindowText( szText,10); 	
	SysInfo05.m_nHRL = atoi( szText ); //hr
	if( SysInfo05.m_nHRL> MAX_HR_USER_R){//sylee161208-2     //son220922  20000 -> MAX_HR_USER_R(default 10000)
		SysInfo05.m_nHRL= MAX_HR_USER_R;                    //son220922
	    //son220922 sprintf(szText, "%d",  SysInfo05.m_nHRL);
        //son220922 m_edit101.SetWindowText(szText);
 	}
	
    //--------------------------------------
	//son Short UserR (Kohm ����)
   	m_edit57.GetWindowText( szText,10); 	
	SysInfo05.m_nShortR2U = atoi( szText ); //lv short
	if( SysInfo05.m_nShortR2U> MAX_SHORT_USER_R){//sylee161208-2    //son220922 2000: MAX_SHORT_USER_R 
		SysInfo05.m_nShortR2U= MAX_SHORT_USER_R;                    //son220922
	    //son220922 sprintf(szText, "%d",  SysInfo05.m_nShortR2U);
        //son220922 m_edit101.SetWindowText(szText);
 	}

   	m_edit3.GetWindowText( szText,10); 	
	SysInfo05.m_nDelay1 = atoi( szText );  
	SysInfo05.m_nCycle = 0; 
	
    m_edit55.GetWindowText( szText,10); 	
	SysInfo05.m_nSparkSet = atoi( szText );  //sylee140430

//    m_edit66.GetWindowText( szText,10); 	       //hdpark230223
//	SysInfo05.m_dSparkCurrentSet = atof( szText );  //hdpark230223


//sylee140923    m_edit47.GetWindowText( szText,10); 	
//sylee140923	SysInfo05.m_nCycle = atoi( szText ); 


    //--------------------------------------
    //son SparkI (1~15)
    m_edit47.GetWindowText( szText,10); 	//sylee200709  NEW_SPARK
	SysInfo05.m_nSparkSet_B = atoi( szText );  //sylee200709 NEW_SPARK

	if(SysInfo05.m_nSparkSet_B<0) { 
	    SysInfo05.m_nSparkSet_B=0;   //sylee200709 NEW_SPARK
    }
	if(SysInfo05.m_nSparkSet_B> MAX_SPARK_I_SET){   //son220922 100: MAX_SPARK_I_SET
	    SysInfo05.m_nSparkSet_B= MAX_SPARK_I_SET;   //sylee200709 NEW_SPARK
    }


    //--------------------------------------
    //son Spark Set 1~ 250 V  : m_edit55, // hdpark230223 1V -> 2V
	if(SysInfo05.m_nSparkSet<2){	
		SysInfo05.m_nSparkSet=2;
		m_edit55.SetWindowText("2");
		AfxMessageBox(" ERORR NO 1501,  Spark V Set Error ! ( < 2V )  ->2V Set ");	
	}

	/*
	if(SysInfo05.m_nSparkSet>100){
		SysInfo05.m_nSparkSet=100;
		m_edit55.SetWindowText("100");
		AfxMessageBox(" ERORR NO 1501,  Spark V Set Error ! ( > 100V )   ->100V Set");	
	}
	*/

	if(SysInfo05.m_nSparkSet> MAX_SPARK_V_SET){//SYLEE180412  RAYOK  UTRON   //son220922 MAX_SPARK_V_SET
		SysInfo05.m_nSparkSet= MAX_SPARK_V_SET;
	    sprintf(szText, "%d",  SysInfo05.m_nSparkSet);  //son220922
		m_edit55.SetWindowText(szText);                 //son220922
		AfxMessageBox(" ERORR NO 1501,  Spark V Set Error ! ( > %dV )  -> %dV Set",     //son240630
		                               MAX_SPARK_V_SET, MAX_SPARK_V_SET);	            //son240630
	}

	// hdpark230223 begin
	//if(SysInfo05.m_dSparkCurrentSet < 1){	
	//	SysInfo05.m_dSparkCurrentSet = 1;
	//	m_edit66.SetWindowText("1");
	//	AfxMessageBox(" ERORR NO 1501,  Spark I Set Error ! ( < 0.5 mA )  ->1 mA Set ");	
	//}

	//if(SysInfo05.m_dSparkCurrentSet> MAX_SPARK_I_SET){
	//	SysInfo05.m_dSparkCurrentSet= MAX_SPARK_I_SET;
	//    sprintf(szText, "%.1f",  SysInfo05.m_dSparkCurrentSet);
	//	m_edit66.SetWindowText(szText);                 
	//	AfxMessageBox(" ERORR NO 1501,  Spark I Set Error ! ( > 20mA )   ->20mA Set");	
	//}
	// hdpark230223 end
	
    //---------------------------
    //son 4W Err.% Limit
	m_edit60.GetWindowText( szText,10);  //sylee170915
	SysInfo05.m_n4WErrPerLimit = atoi( szText ); //sylee170915
	if(SysInfo05.m_n4WErrPerLimit<0){
		SysInfo05.m_n4WErrPerLimit = 0;
	}
	if(SysInfo05.m_n4WErrPerLimit>10000){
		SysInfo05.m_n4WErrPerLimit =10000;
	}

	if( SysInfo05.m_nOpenV<1){
		SysInfo05.m_nOpenV=2;
	}

	if( SysInfo05.m_nShort1V<1){
		SysInfo05.m_nShort1V=1;
	}

	if( SysInfo05.m_nShort2V<1){
		SysInfo05.m_nShort2V=1;
	}

	if( SysInfo05.m_nHRV<1){
		SysInfo05.m_nHRV=1;
	}
    if( SysInfo05.m_nHv<1){
		SysInfo05.m_nHv=10;
	}

	if( SysInfo05.m_nHv2<1){
		SysInfo05.m_nHv2=10;
	}

	if( SysInfo05.m_nCon<1){
		SysInfo05.m_nCon= RECP_OPENR_50ohm;         //son220705 6:RECP_OPENR_50ohm  
	}

	if( SysInfo05.m_nShortR< 1){  
		SysInfo05.m_nShortR= RECP_uShortR_100Kohm;  //son220705 3: RECP_uShortR_100Kohm 
	}

	if( SysInfo05.m_nShortR2<1){
		SysInfo05.m_nShortR2= RECP_LShortR_1Mohm;  //son220705 16: RECP_LShortR_1Mohm
	}
	if( SysInfo05.m_nHR<1){
		SysInfo05.m_nHR= RECP_HRShortR_1Mohm;      //son220705 3: RECP_HRShortR_1Mohm
	}
  	if( SysInfo05.m_nHv1R<1){       //son210324 m_nConIs-> m_nHv1R
		SysInfo05.m_nHv1R= RECP_LEAK_20Mohm;        //son220705 3: RECP_LEAK_20Mohm
	}
	if( SysInfo05.m_nHv2R<1){
		SysInfo05.m_nHv2R= RECP_LEAK_20Mohm;        //son220705 3: RECP_LEAK_20Mohm
	}
    if( SysInfo05.m_nShortDisable1!=7){    //sylee160518  m_nShortDisable1; //sylee180331-1 
		SysInfo05.m_nShortDisable1=0;  //sylee180331-1 
	}

 //sylee170411 if(SysSet13.m_nSet19==1){//sylee151207
	 
	if(SysInfo05.m_nProSpMode==2|| 	 SysInfo05.m_nProSpMode==3 ||  SysInfo05.m_nProSpMode==4 ){ //sylee150417-1 
    	if(SysSet16.m_nSet11==1){  //estimation
 			nFlagEstModelMake=1;//sylee130325
		}else{
			nFlagEstModelMake=0;//sylee130325
		}
	}
 //sylee170411	}


    FileSysInfo01.LoadSaveSub15(_SAVE); //son220901
    FileSysInfo01.printLog_Recipe();    //son220901 Recipe�� D:\Log ������ ���.

    Onload();

}

 
void CModeSub15::Onload() 
{
	CString str; 
	char szText[100];
	FileSysInfo01.LoadSaveSub15(2);

//son231106_2 : 230911 SysSet21_ACE400_230911.ini �������� ���� son221226, son221227, son230711 ��ɻ��� 
//   ��ǳ SST���� 50V �����ÿ� ������ �ٹ߹��� �����Ͽ� SW�� HW�� �߰� ������.  
//    22/12/26���� �ϱ� ���� ������ �ʿ� ���������Ƿ�  ���󺹱���. (23/9/11�� �迵�� �̻�� ��û)
////son221227 begin
//    //son dRec(SysSet21_ACE400.ini)�� OPEN 30V, 40V, 50V��   10mA�� �ƴ϶� 3mA or 4mA or 5mA�� �Ǿ� �����Ƿ�
//    //    OPEN 30V ���ýÿ��� ���� ���ø���Ʈ�� 3mA��, OPEN 40V ���ýÿ��� ���� ���ø���Ʈ�� 4mA��
//    //    OPEN 50V ���ýÿ��� ���� ���ø���Ʈ�� 5mA�� ���̰� �Ѵ�. 
//	 	                                                                    //son221227 OPEN 30V�� ������ 2:3mA
//	 	                                                                    //son221227 OPEN 40V�� ������ 2:4mA
//	 	                                                                    //son221227 OPEN 50V�� ������ 2:5mA
//    if (SysInfo05.m_nOpenV == RECP_OPENV_4_30V  
//            || SysInfo05.m_nOpenV == RECP_OPENV_5_40V
//            || SysInfo05.m_nOpenV == RECP_OPENV_6_50V)
//        changeContent_OpenCurrentCombo28_HighVolt();
//    else
//        changeContent_OpenCurrentCombo28_default();
////son221227 end
    changeContent_OpenCurrentCombo28_default(); //son231106_2


    //sylee170622-2	m_combo1.SetCurSel(SysInfo05.m_nTemp1);   //sylee160518
	m_combo2.SetCurSel(SysInfo05.m_nCon);
	m_combo3.SetCurSel(SysInfo05.m_nHv1R); //son m_combo3  HV(Leak1) R ������.  //son210324 m_nConIs-> m_nHv1R
	m_combo4.SetCurSel(SysInfo05.m_nHv);   //hv
	m_combo6.SetCurSel(SysInfo05.m_nSp);
	m_combo8.SetCurSel(SysInfo05.m_nHR);
 	m_combo9.SetCurSel(SysInfo05.m_nShortR);
 	m_combo10.SetCurSel(SysInfo05.m_nShortR2);
	m_combo22.SetCurSel(SysInfo05.m_nOpenV);
	m_combo23.SetCurSel(SysInfo05.m_nShort1V);   
 	m_combo24.SetCurSel(SysInfo05.m_nShort2V);//lv
 	m_combo44.SetCurSel(SysInfo05.m_nSparkDetection[0]); // hdpark230331, voltage, current
 	m_combo46.SetCurSel(SysInfo05.m_nSparkDetection[1]); // hdpark230331, leak1 voltage, current
 	m_combo47.SetCurSel(SysInfo05.m_nSparkDetection[2]); // hdpark230331, leak2 voltage, current
 	m_combo45.SetCurSel(SysInfo05.m_nSparkCurrentSet); // hdpark230223, spark current set

	switch(SysInfo05.m_nSparkCurrentSet)
	{
	case 0 : SysInfo05.m_dSparkCurrentSet = 1.0;	break;
	case 1 : SysInfo05.m_dSparkCurrentSet = 2.0;	break;
	case 2 : SysInfo05.m_dSparkCurrentSet = 4.0;	break;
	case 3 : SysInfo05.m_dSparkCurrentSet = 6.0;	break;
	case 4 : SysInfo05.m_dSparkCurrentSet = 10.0;	break;
	case 5 : SysInfo05.m_dSparkCurrentSet = 15.0;	break;
	case 6 : SysInfo05.m_dSparkCurrentSet = 20.0;	break;
	}



//#if (defined(__GTS__CUSTOMER) && !defined(__MST__CUSTOMER)) 
#ifdef __GTS__CUSTOMER //son220913
    //son220609 GTS�� ��������ȭ�������� UI�� �����ش�.   ����� ����.
#else
 	//son ��������ȭ ���� ����� ON�̸�
    if(SysSet13.m_nSet1==1)//sylee181005-3     
#endif
    {
		m_combo25.SetCurSel(SysInfo05.m_nLeak1_Current-1);//sylee181005-3   //son Leak1 ����:  1:20mA  2:10mA  3:5mA  4:3mA, 5:1mA  //son220706
		m_combo27.SetCurSel(SysInfo05.m_nLeak2_Current-1);//sylee181005-3   //son Leak2 ����:  1:20mA  2:10mA  3:5mA  4:3mA, 5:1mA  //son220706
	 	m_combo28.SetCurSel(SysInfo05.m_nOpen_Current-1);//sylee181005-3    //son Open  ����:  1:20mA  2:10mA  3:5mA                //son220706
	//	m_combo29.SetCurSel(SysInfo05.m_nCC[4]-1);//sylee181005-3   
		m_combo30.SetCurSel(SysInfo05.m_nShort_Current-1);//sylee181005-3   //son Short ����:  1:20mA  2:10mA  3:5mA  4:3mA, 5:1mA //son220706
/*//SYLEE220517	}else{
		m_combo25.SetCurSel(2);//sylee181005-3
		m_combo27.SetCurSel(2);//sylee181005-3
		m_combo28.SetCurSel(2);//sylee181005-3
//		m_combo29.SetCurSel(2);//sylee181005-3
		m_combo30.SetCurSel(2);//sylee181005-3
*/
	}

	if(SysInfo05.m_n4WErrPerLimit<1){//sylee170915-1
		SysInfo05.m_n4WErrPerLimit=0;//sylee170915-1
	}
	//sylee170622-3	m_combo25.SetCurSel(SysInfo05.m_nDual);
 	m_combo26.SetCurSel(SysInfo05.m_nHRV); 
	
    for(int i1=1;i1<=6;i1++)
    { 
        if( i1==3){//sylee181016-1
            //son 'Open Offset' Check Off �̸�
            if(SysInfo05.m_nCC[i1]!=1){ //sylee181016-1
                SysInfo05.m_nCC[i1]=0;
            } 
        }
        else if( i1==4)//sylee181016-1
        {
            //son 'Open Offset' value range üũ
            if(SysInfo05.m_nCC[i1]>50){//sylee181016-1
                SysInfo05.m_nCC[i1]=50;	//sylee181016-1				
            }
            if(SysInfo05.m_nCC[i1]<0){//sylee181016-1
                SysInfo05.m_nCC[i1]=0;	//sylee181016-1				
            }
      //SYLEE220517  }else{ 
      //SYLEE220517      if(SysInfo05.m_nCC[i1]!=2 && SysInfo05.m_nCC[i1]!=3){ //sylee190530  
      //SYLEE220517           SysInfo05.m_nCC[i1]=1;
      //SYLEE220517      }			  
        }
    }

    m_check11.SetCheck(SysInfo05.m_4W_Measure);//sylee181016-1  //sylee200520-4W�����м�

	m_check4.SetCheck(SysInfo05.m_nCC[3]);//sylee181016-1
	sprintf(szText, "%d", SysInfo05.m_nCC[4]);//sylee181016-1
    m_edit2.SetWindowText(szText);//sylee181016-1
    m_combo32.SetCurSel(SysInfo05.m_nStep[1]);
    if(SysInfo05.m_nStep[2]>1){//SYLEE140207
		m_combo33.SetCurSel(SysInfo05.m_nStep[2]-1);
	}else{
	    m_combo33.SetCurSel(SysInfo05.m_nStep[2]);
	}

	if(SysInfo05.m_nStep[4]>=1){//SYLEE140207
		m_combo35.SetCurSel(SysInfo05.m_nStep[4]-1);
	}else{
		m_combo35.SetCurSel(SysInfo05.m_nStep[4]);
	}

#ifdef __RECIPE_LEAK_MORE1  //sylee231016 
    if(SysInfo05.m_nStep[3]>0){    
         m_combo34.SetCurSel(SysInfo05.m_nStep[3]-1);
    }
    else{      
        m_combo34.SetCurSel(SysInfo05.m_nStep[3]);  
    }

    if(SysInfo05.m_nStep[5]>4){    
        m_combo36.SetCurSel(SysInfo05.m_nStep[5]-1);
    }
    else{      
        m_combo36.SetCurSel(SysInfo05.m_nStep[5]);  
    }
    if(SysInfo05.m_nStep[6]>4){    
        m_combo37.SetCurSel(SysInfo05.m_nStep[6]-1);
    }
    else{      
        m_combo37.SetCurSel(SysInfo05.m_nStep[6]);  
    }
    if(SysInfo05.m_nStep[7]>4){    
        m_combo38.SetCurSel(SysInfo05.m_nStep[7]-1);
    }
    else{      
        m_combo38.SetCurSel(SysInfo05.m_nStep[7]);  
    }   
#else
    if(SysInfo05.m_nStep[3]>4){//SYLEE140207
        m_combo34.SetCurSel(SysInfo05.m_nStep[3]-4);
    }
    else{
        m_combo34.SetCurSel(SysInfo05.m_nStep[3]);
    }    
    m_combo36.SetCurSel(SysInfo05.m_nStep[5]);
    m_combo37.SetCurSel(SysInfo05.m_nStep[6]);
    m_combo38.SetCurSel(SysInfo05.m_nStep[7]);                                                      
#endif


	if(SysInfo05.m_nProSpMode<1){ //sylee140210
		SysInfo05.m_nProSpMode=1;	 
	}
  	m_combo40.SetCurSel(SysInfo05.m_n4WSet1); 
	m_check1.SetCheck(SysInfo05.m_nHVCal1); //sylee170713-2
    m_check2.SetCheck(SysInfo05.m_nOpenMea1);	//SYLEE210309-1	   //sylee211122 m_nUniversal->m_nOpenMea1 
	if( SysInfo05.m_nShortDisable1!=7){    //sylee180331-1 
		SysInfo05.m_nShortDisable1=0;  //sylee180331-1 
	}
    m_check3.SetCheck(SysInfo05.m_nShortDisable1);	//sylee180331-1
    m_check5.SetCheck(SysInfo05.m_nAC);	//son220613
	m_scrollbar2.SetScrollPos(SysInfo05.m_nConEr); 
	m_scrollbar3.SetScrollPos(SysInfo05.m_nIsEr);	
	//sylee190528-2 m_scrollbar5.SetScrollPos(SysInfo05.m_nReCh+1);
	m_scrollbar5.SetScrollPos(SysInfo05.m_nReCh);//sylee190528-2
 	m_scrollbar6.SetScrollPos(SysInfo05.m_nConFail+1);

	str.Format("%d", SysInfo05.m_nConEr);
	m_Label2.SetCaption(str);

	str.Format("%d", SysInfo05.m_nIsEr);
	m_Label3.SetCaption(str);

	str.Format("%d", SysInfo05.m_nReCh);
	m_Label5.SetCaption(str);	

	str.Format("%d", SysInfo05.m_nConFail);
	m_Label6.SetCaption(str);

	sprintf(szText, "%d", SysInfo05.m_nConU);
    m_edit1.SetWindowText(szText);

	sprintf(szText, "%d", SysInfo05.m_nHv1RU);      //son210324 m_nConIsU-> m_nHv1RU
    m_edit101.SetWindowText(szText);

	sprintf(szText, "%d", SysInfo05.m_nShortRU);
    m_edit02.SetWindowText(szText);

	sprintf(szText, "%d", SysInfo05.m_nHRL);
    m_edit5.SetWindowText(szText); 

	sprintf(szText, "%d", SysInfo05.m_nDelay1);
    m_edit3.SetWindowText(szText); 

	sprintf(szText, "%d", SysInfo05.m_nShortR2U);
    m_edit57.SetWindowText(szText); 

	sprintf(szText, "%d", SysInfo05.m_nSparkSet);
    m_edit55.SetWindowText(szText); 
//sylee220401

	sprintf(szText, "%.1f", SysInfo05.m_dSparkCurrentSet); // hdpark230223
    m_edit66.SetWindowText(szText); // hdpark230223


	sprintf(szText, "%d", SysInfo05.m_nSparkSet_B); //sylee200709 NEW_SPARK
    m_edit47.SetWindowText(szText);                 //sylee200709 NEW_SPARK

	sprintf(szText, "%d", SysInfo05.m_n4WErrPerLimit );//sylee170915-1 //sylee220401
    m_edit60.SetWindowText(szText);
 	m_combo20.SetCurSel(SysInfo05.m_nHv2);  //sylee140923
	if( SysInfo05.m_nHv2R<1){
		SysInfo05.m_nHv2R=2;
	}
	m_combo43.SetCurSel(SysInfo05.m_nHv2R);  //sylee140923 
	sprintf(szText, "%d", SysInfo05.m_nHv2RU);   //sylee140923
    m_edit125.SetWindowText(szText); 	//sylee140923
	
	::ZeroMemory(&RunInfo05, sizeof(RunInfo05));

//son220705_2 RunInfo05.m_nCon ���� �̻���̰� 
//            19�� User�� �ڵ���.  ���Ұ��� �����ڵ��̹Ƿ�  ������. 
//                  1  2  3  4    5  6  7 8  9    10  11  12  13  14    15   16   17    18

//	int nCon[50]={0,5,10,20,30,  40,50,60,70,80,  90,100,250,500,1000,  2000,5000,10000,2000,0,0,0};
//
//	if(SysInfo05.m_nCon<50){    //son220705 50:
//		RunInfo05.m_nCon = nCon[SysInfo05.m_nCon];
//	}else{
//        RunInfo05.m_nCon = 0;
//	}
// 
//	if( SysInfo05.m_nCon ==19) {//user
//       RunInfo05.m_nCon =  SysInfo05.m_nConU ;
//	}  

    if(SysInfo05.m_nSparkSet<1){
		SysInfo05.m_nSparkSet=10;
		m_edit55.SetWindowText("10");
	//	AfxMessageBox(" ERORR NO 1501,  Spark V Set Error ! ( < 1V )  ->10V Set ");	
	}
	if( SysInfo05.m_nOpenV<1){
		SysInfo05.m_nOpenV=2;
	}
	if( SysInfo05.m_nShort1V<1){
		SysInfo05.m_nShort1V=1;
	}
	if( SysInfo05.m_nShort2V<1){
		SysInfo05.m_nShort2V=1;
	}
	if( SysInfo05.m_nHRV<1){
		SysInfo05.m_nHRV=1;
	}
    if( SysInfo05.m_nHv<1){
		SysInfo05.m_nHv=10;
	}
	if( SysInfo05.m_nHv2<1){
		SysInfo05.m_nHv2=10;
	}
	if( SysInfo05.m_nCon< 1){ 
		SysInfo05.m_nCon= RECP_OPENR_50ohm;         //son220705 6: RECP_OPENR_50ohm
	}
	if( SysInfo05.m_nShortR<1){
		SysInfo05.m_nShortR= RECP_uShortR_100Kohm;  //son220705 3: RECP_uShortR_100Kohm
	}
	if( SysInfo05.m_nShortR2<1){
		SysInfo05.m_nShortR2= RECP_LShortR_1Mohm;    //son220705 16: RECP_LShortR_1Mohm
	}
	if( SysInfo05.m_nHR<1){
		SysInfo05.m_nHR= RECP_HRShortR_1Mohm;      //son220705 3: RECP_HRShortR_1Mohm
	}
  	if( SysInfo05.m_nHv1R<1){   //son210324 m_nConIs-> m_nHv1R   for Leak1
		SysInfo05.m_nHv1R= RECP_LEAK_20Mohm;    //son220705 3: RECP_LEAK_20Mohm
	}
	if( SysInfo05.m_nHv2R<1){
		SysInfo05.m_nHv2R= RECP_LEAK_20Mohm;    //son220705 3: RECP_LEAK_20Mohm
	}
    if( SysInfo05.m_nShortDisable1!=7){    //sylee180331-1 
		SysInfo05.m_nShortDisable1=0;  //sylee180331-1 
	}

    //if(SysInfo05.m_dSparkCurrentSet<1){ //hdpark230223 begin
	//	SysInfo05.m_dSparkCurrentSet=1; 
	//	m_edit66.SetWindowText("1");    
	//} //hdpark230223 end
 
}



void CModeSub15::A_Language()  //LANGUAGE CHANGE
{

   if(SysSet211.m_nSet13==1){//sylee160907-1   //chinese
       		gDef_Language1=3;//redboard
	}else{//sylee160907-1
        gDef_Language1=2; //englishos
	}//sylee160907-1

  if( gDef_Language1==3){ 
 
      m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 101));
 	  m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 102));
	  m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 103));
	  m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 104));
//      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 105));
 	  m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 106));
	  m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 107));
	  m_Label1008.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 108));
      m_Label1009.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 109));
 	  m_Label1010.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 110));

	  m_Label1011.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 111));
	  m_Label1012.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 141));
      m_Label1013.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 142));
 	  m_Label1014.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 143));
	  m_Label1015.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 144));
	  m_Label1016.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 145));
 	  m_Label1017.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 146));
	  m_Label1018.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 147));

	  m_Label1019.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 114));
      m_Label1020.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 115));
 	  m_Label1021.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 116));
	  m_Label1022.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 117));

	  m_Label1023.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 112));
 	  
//	  m_Label1024.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 118));
//	  m_Label1025.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 124));
	  m_Label1026.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 128));
//		m_Label1047.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 129));	// spark detection
//		m_Label1046.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 130));	// spark-I

 	  m_Label1027.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 131));
	  m_Label1028.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 132));
	  m_Label1029.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 135));
//son220613      m_Label1030.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 136));
// 	  m_Label1031.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 137));
	  m_Label1032.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 138));
 
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 113));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 139));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 140));
	  m_LabelCurrent.SetWindowText(LoadMessage(1,"TitleSetupRecipe", 148));     //son220630 ���� Label
	  
 
 
//105=CC/CV
 
  }

}



 


void CModeSub15::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nScrollPos;
	CString strValue;

 
	if(pScrollBar->GetSafeHwnd() == m_scrollbar2.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfo05.m_nConEr=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfo05.m_nConEr=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label2.SetCaption(strValue);
			SysInfo05.m_nConEr=nPos;
	 	//	Invalidate();
			break;
		}
	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar3.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfo05.m_nIsEr=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfo05.m_nIsEr=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label3.SetCaption(strValue);
			SysInfo05.m_nIsEr=nPos;
	 	//	Invalidate();
			break;
		}

/*	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar4.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT: 		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label4.SetCaption(strValue);
				SysInfo05.m_nConErSt=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label4.SetCaption(strValue);
				SysInfo05.m_nConErSt=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label4.SetCaption(strValue);
			SysInfo05.m_nConErSt=nPos;
	 	//	Invalidate();
			break;
		}
*/
	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar5.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label5.SetCaption(strValue);
				SysInfo05.m_nReCh=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <31){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label5.SetCaption(strValue);
				SysInfo05.m_nReCh=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label5.SetCaption(strValue);
			SysInfo05.m_nReCh=nPos;
	 	//	Invalidate();
			break;
		}

	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar6.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label6.SetCaption(strValue);
				SysInfo05.m_nConFail=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <31){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label6.SetCaption(strValue);
				SysInfo05.m_nConFail=nScrollPos;
			//	Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label6.SetCaption(strValue);
			SysInfo05.m_nConFail=nPos;
	 	//	Invalidate();
			break;
		}
	}	
 
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
 

void CModeSub15::OnCheck5() 
{
	// TODO: Add your control notification handler code here
	if(m_check5.GetCheck())
	{	 
		SysInfo05.m_nAC=1;	//son220613
	}
	else
	{	 
		SysInfo05.m_nAC=0;	//son220613
	}
	m_check5.SetCheck(SysInfo05.m_nAC);  	//son220613
	
}



void CModeSub15::OnSelchangeCombo22() 
{
	// TODO: Add your control notification handler code here
    SysInfo05.m_nOpenV = m_combo22.GetCurSel();

//son231106_2 begin: 230911 SysSet21_ACE400_230911.ini �������� ���� son221227 ��ɻ���. �� �Լ� ȣ��κ��� ������.
//   ��ǳ SST���� 50V �����ÿ� ������ �ٹ߹��� �����Ͽ� SW�� HW�� �߰� ������.  
//   221226, 221227 �ϱ� ���� ������ �ʿ� ���������Ƿ�  ���󺹱���. (23/9/11�� �迵�� �̻�� ��û)
//   dRec���� 30V, 40V, 50V ��� 10mA�� ���󺹱���
//
//son221227 begin
//    //son dRec(SysSet21_ACE400.ini)�� OPEN 30V, 40V, 50V��   10mA�� �ƴ϶� 3mA or 4mA or 5mA�� �Ǿ� �����Ƿ�
//    //    OPEN 30V ���ýÿ��� ���� ���ø���Ʈ�� 3mA��, OPEN 40V ���ýÿ��� ���� ���ø���Ʈ�� 4mA��
//    //    OPEN 50V ���ýÿ��� ���� ���ø���Ʈ�� 5mA�� ���̰� �Ѵ�. 
//    if (SysInfo05.m_nOpenV == RECP_OPENV_4_30V  
//            || SysInfo05.m_nOpenV == RECP_OPENV_5_40V
//            || SysInfo05.m_nOpenV == RECP_OPENV_6_50V)
//        changeContent_OpenCurrentCombo28_HighVolt();
//    else
//        changeContent_OpenCurrentCombo28_default();
//son221227 end
//son231106_2 end	
}

//son231106_2 begin: 230911 SysSet21_ACE400_230911.ini �������� ���� son221227 ��ɻ���. 
//void CModeSub15::changeContent_OpenCurrentCombo28_HighVolt() //son221227 
//{
//    m_combo28.ResetContent();
//
//    m_combo28.InsertString(-1, "*");
//    switch (SysInfo05.m_nOpenV)
//    {
//        case RECP_OPENV_4_30V: m_combo28.InsertString(-1, "3mA");  break;
//        case RECP_OPENV_5_40V: m_combo28.InsertString(-1, "4mA");  break;
//
//        case RECP_OPENV_6_50V: 
//        default:               m_combo28.InsertString(-1, "5mA");  break;
//    }
//
//    m_combo28.InsertString(-1, "*");
//	
//} 
//son231106_2 end

void CModeSub15::changeContent_OpenCurrentCombo28_default()     //son221227
{
    m_combo28.ResetContent();

    m_combo28.InsertString(-1, "20mA");     //son 1: CURR_20mA   SysInfo05.m_nOpen_Current = m_combo28.GetCurSel()+1;
    m_combo28.InsertString(-1, "10mA");     //son 2: CURR_10mA   +1 �ؼ� ����ϹǷ� 0 ��ġ�� CURR_20mA ���� 1�̴�.
    m_combo28.InsertString(-1, "5mA");      //son 3: CURR_5mA    1��ġ�� CURR_10mA ���� 2, 2��ġ�� CURR_5mA ���� 3.
	
}
