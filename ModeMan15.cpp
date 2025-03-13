// ModeMan15.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeMan15.h"
#include "Contec.h"
#include "ChildView1.h"
#include "FileSysInfo.h"

extern CSysInfoView01  SysInfoView01;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 





extern int PinData[DEF_MAX_PIN1][10];   //file reading
extern int PinDataP1[DEF_MAX_PIN1][10];  //piece sort
extern int PinDataP2[300][3]; // piece header
extern int PinDS1[DEF_MAX_PIN1][5]; 

extern int nPcb1[5000][2]; //PCB NO , PIN NO
extern int PinDS1R1[DEF_MAX_PIN1];

extern double nCalC[30][10][100];
extern int nMCon3,	nMCon2, nMCon3, nMCon4;


extern void DoEvents();

 
extern void OnRsModeAuto(int a);  
extern void OnRsModeRange(int a, int b);  //MODE Range(0~10)
extern double OnRsMeas1(int a, int nMode2);  //measure?  //nMode2=1 R,    2= C
extern void OnRsModeFreq(int a);  //MODE C  // 1= 1000 , 2= 120 ,  ?
extern void OnRsModeSpeed(int a);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
//int a ==> =1=100m,2=1,3=10,4=100,5=1k,6=10k,7=100k,8=1M,9=10M,10=200M

extern BOOL OpenRS( int nInt); 
extern BOOL CloseRS( int nInt); 
extern BOOL ReadRS(int nPort);
extern BOOL WriteRS(int nPort, char* cStr);


extern int nRsModeR1S;

extern long lm_nCon;;


/////////////////////////////////////////////////////////////////////////////
// CModeMan15 dialog


CModeMan15::CModeMan15(CWnd* pParent /*=NULL*/)
	: CDialog(CModeMan15::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeMan15)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeMan15::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeMan15)
		// NOTE: the ClassWizard will add DDX and DDV calls here
			DDX_Control(pDX, IDC_LIST1, m_list1);
		
			DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
			DDX_Control(pDX, IDC_LABEL1,  m_Label1);
			DDX_Control(pDX, IDC_LABEL2,  m_Label2);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeMan15, CDialog)
	//{{AFX_MSG_MAP(CModeMan15)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CModeMan15, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeMan15)
 	ON_EVENT(CModeMan15, IDC_BUTTON101, -600 /* Click */, OnClickButton101, VTS_NONE)
	ON_EVENT(CModeMan15, IDC_BUTTON102, -600 /* Click */, OnClickButton102, VTS_NONE)
	ON_EVENT(CModeMan15, IDC_BUTTON103, -600 /* Click */, OnClickButton103, VTS_NONE)
	ON_EVENT(CModeMan15, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeMan15 message handlers




BOOL CModeMan15::OnInitDialog() 
{
	CString str;

	CDialog::OnInitDialog();

	m_Progress1.SetPos(100);
	str.Format("     ");
	m_Label1.SetCaption(str);
	m_Label2.SetCaption(str);

	nStop2=0;
    
	return 1;
}


void CModeMan15::OnClickButton101() 
{
 
 

	return;//DEMO 
 	
}



void CModeMan15::OnClickButton102() 
{ 


	return;//DEMO
 
}


void CModeMan15::OnClickButton103() 
{ 
  nStop2=2;
}


void CModeMan15::OnClickOk() 
{
 
	int i;
	i=9;
	nStop2=2;
 
	CDialog::OnOK();	
}

