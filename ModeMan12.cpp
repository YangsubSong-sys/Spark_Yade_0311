// ModeMan12.cpp : implementation file
//
 

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeMan12.h"
#include "Contec.h"


#include "ModeSet17.h"
#include "ModeSub71.h"
#include "FileSysInfo.h"
#include "ChildView7.h"

//#include "ChildView7.h"
#include "A_Define1.h"//sylee161126
#include "Globals.h"    //son220412
 
 
extern int nJigTestOpen,nJigTestShort;//sylee181008-1
extern short nBDLData2[1][DEF_MAX_PIN1][10];//open data  

extern short nPinSetSkipVb; //sylee151005 //ACE400	//son230127
extern short nPinSetSkipVc; //sylee151005 //ACE400	//son230127

int nManuaOpenPin1;//sylee160127-3
int nManuaOpenPin2;//sylee160127-3
int nManuaOpenR1; //sylee161214

int nST1,nST2;
int nRdisplay1;//sylee161215


extern short  OnPinConvertFlag1; //sylee160223-1

extern int nMultyNetTotal;//sylee160128
extern void DoEvents();
extern __int64 Get_uSecond1();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int nFlag_Manual_Run1;//sylee161215

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA
  
extern CChildView7  dView7;  //sylee131102


extern CSysSet12  SysSet12;  //Pin SETUP     
extern CSysInfo05  SysInfo05;
extern CModeSub71  Sub71;

extern int nMCon3,	nMCon2, nMCon3, nMCon4;
extern int nAutoProcess1;
extern int g_nPieceMax1; 
extern int nFailL1Co;
extern int nStop;
extern int nRsModeR1S;

extern int nFailCount[15];

//extern short nBDLData3[DefDutMax][dSetPinTot1][10];//short data  
extern short nBDLData3[MAX_DUT1][DEF_MAX_PIN1][10];//short data  //son220824_2 DefDutMax -> MAX_DUT1(33)
 
//extern int nBDLHead1[DefDutMax][5][20];
extern int nBDLHead1[MAX_DUT1][5][20]; //son220824_2 DefDutMax -> MAX_DUT1(33)

extern short nBDLData1[MAX_DUT1][DEF_MAX_PIN1][10];//raw data //son220824_2 DefDutMax -> MAX_DUT1(33)

/*
extern int nBDLHead1[2][5][20];
extern short nBDLData1D[DEF_MAX_PIN1][10];//raw data
extern int nBDLHead1E[5][20];
extern short nBDLData1E[DEF_MAX_PIN1][10];//raw data
extern int nBDLHead1F[5][20];
extern short nBDLData1F[DEF_MAX_PIN1][10];//raw data
*/


extern int PinData[DEF_MAX_PIN1][10];   //file reading
extern int PinDataP1[DEF_MAX_PIN1][10];  //piece sort
extern int PinDataP2[300][3]; // piece header
extern int PinDS1[DEF_MAX_PIN1][5]; 
extern int nPcb1[5000][2]; //PCB NO , PIN NO
extern int PinDS1R1[DEF_MAX_PIN1];
extern double nCalC[30][10][100];
extern double nFailL1Open[DEF_MAX_PIN1][20];  //[10]=1,2= pinno, 


extern void DoEvents();
 
 
extern void OnRsModeAuto(int a);  
extern void OnRsModeRange(int a, int b);  //MODE Range(0~10)
extern void OnRsModeFreq(int a);  //MODE C  // 1= 1000 , 2= 120 ,  ?
extern void OnRsModeSpeed(int a);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
extern double OnRsMeas1(int a, int nMode2);  //measure?  //nMode2=1 R,    2= C
//int a ==> =1=100m,2=1,3=10,4=100,5=1k,6=10k,7=100k,8=1M,9=10M,10=200M

extern BOOL OpenRS( int nInt); 
extern BOOL CloseRS( int nInt); 
extern BOOL ReadRS(int nPort);
extern BOOL WriteRS(int nPort, char* cStr);



extern CSysSet13  SysSet13; 

/////////////////////////////////////////////////////////////////////////////
// CModeMan11 dialog


CModeMan12::CModeMan12(CWnd* pParent /*=NULL*/)
	: CDialog(CModeMan12::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeMan12)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeMan12::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeMan12)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LIST1, m_list1);
		DDX_Control(pDX, IDC_COMBO101, m_combo101);
		DDX_Control(pDX, IDC_LABEL1001, m_Label1001);
		DDX_Control(pDX, IDC_EDIT101, m_edit101);		
		DDX_Control(pDX, IDC_EDIT102, m_edit102); 
		DDX_Control(pDX, IDC_EDIT103, m_edit103); 

		DDX_Control(pDX, IDC_LABEL8,  m_Label8);
		DDX_Control(pDX, IDC_LABEL9,  m_Label9);


		DDX_Control(pDX, IDC_LABEL1,  m_Label1); 
		DDX_Control(pDX, IDC_LABEL2,  m_Label2); 
		DDX_Control(pDX, IDC_LABEL3,  m_Label3); 	 

//sylee161213-1	DDX_Control(pDX, IDC_BUTTON1001, m_button1001); 
		DDX_Control(pDX, IDC_BUTTON1005, m_button1005);
		DDX_Control(pDX, IDC_BUTTON103, m_button103); 
		DDX_Control(pDX, IDC_OK, m_buttonOk); 

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeMan12, CDialog)
	//{{AFX_MSG_MAP(CModeMan12)
		// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_COMBO101, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeMan12 message handlers
BEGIN_EVENTSINK_MAP(CModeMan12, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeMan12)
 //sylee161213-1  	ON_EVENT(CModeMan12, IDC_BUTTON1001, -600 /* Click */, OnClickButton1001, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_BUTTON1005, -600 /* Click */, OnClickButton1005, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_BUTTON103, -600 /* Click */, OnClickButton103, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
    ON_EVENT(CModeMan12, IDC_BUTTON101, -600 /* Click */, OnButton101, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_BUTTON102, -600 /* Click */, OnButton102, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_BUTTON104, -600 /* Click */, OnButton104, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_BUTTON105, -600 /* Click */, OnButton105, VTS_NONE)
	ON_EVENT(CModeMan12, IDC_BUTTON107, -600 /* Click */, OnButton107, VTS_NONE)

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 
 BOOL CModeMan12::PreTranslateMessage(MSG* pMsg) 
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

 
void CModeMan12::OnSelchangeCombo1() //SYLEE130605
{
     nFlag_Manual_Run1  = m_combo101.GetCurSel(); 

}



BOOL CModeMan12::OnInit_File1(short type) 
{
 	FILE *fp; 
	char  fName[200];  
	CString str; 

    ::ZeroMemory(&fName, sizeof(fName)); 
    str.Format("%s\\SETUP2\\ManOpen.ini", SysInfoView01.m_pStrFilePath1);   //son210514 "C:\\ACE500\\SETUP2\\ManOpen.ini"
    strcat(fName , str); //son201118    //son210514
 
 	switch(type)
	{
	case 1: //DATASAVE :   
 		fp = fopen(fName,"wt");
		if(fp == NULL) return 0;
		fprintf(fp, "%d\n",nManuaOpenPin1);
		fprintf(fp, "%d\n",nManuaOpenPin2);	 
		break;

	case 2://DATALOAD :
		fp=fopen(fName,"rt");
		if(fp==NULL) return 0;
		 fscanf(fp, "%d\n", &nManuaOpenPin1);		 
	     fscanf(fp, "%d\n", &nManuaOpenPin2);  
		break;

	default :
		break;
	}
	fclose(fp); 

	return 1;

 }




BOOL CModeMan12::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
	CString str1;

//	m_combo101.SetCurSel(1);//sylee130605 
	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928

	A_Language();

	OnInit_File1(2);//sylee160128-1

	nManuaOpenPin1=1;
	nManuaOpenPin2=4096*4;

	str1.Format("%d",nManuaOpenPin1); //sylee160127-3
	m_edit101.SetWindowText(str1);//sylee160127-3
 	str1.Format("%d",nManuaOpenPin2); //sylee160127-3
    m_edit102.SetWindowText(str1);//sylee160127-3


	nDut=1;  //sylee160128
	str1.Format("%7d", nDut);
	m_Label8.SetCaption(str1);
 
	if(nMultyNetTotal<=0){
		nMultyNetTotal=0;	 		
	}else if(nMultyNetTotal> MAX_DUT){//SYLEE180810-1-DUT16     //son220824_2 DefDutMax2 -> MAX_DUT
		nMultyNetTotal=1;	   		
	}

	str1.Format("%7d", nMultyNetTotal);
	m_Label9.SetCaption(str1);

	if( nMultyNetTotal<=0){
         AfxMessageBox("  File loading! ???? ,   [FILE]? ,    DUT  Total=0 ?", MB_OK);
	}

	nRdisplay1=1;

    str1.Format("100"); //sylee160127-3
    m_edit103.SetWindowText(str1);//sylee161216-1

	m_combo101.SetCurSel(1);//sylee161216-1

	if(SysSet12.m_nPinTotMax== PIN_TOT_4K){	//4K  //sylee151007 //ACE400
		a1=1;
		a4=4096;
		a2=a4/2;
		a3=a2+1;
	}
    else  if(SysSet12.m_nPinTotMax== PIN_TOT_8K){//8k		
		a1=1;
		a4=4096*2;
		a2=a4/2;
		a3=a2+1;
	}
    else  if(SysSet12.m_nPinTotMax== PIN_TOT_16K){//16k   
		a1=1;
		a4=4096*4;
		a2=a4/2;
		a3=a2+1;
	}
    else  if(SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS){//32k	//son230216_1
		a1=1;
		a4=4096*8;
		a2=a4/2;
		a3=a2+1;
	}
    else {	// 20K, 24K, Org 32K
		a1=1;
		a4=4096*8;
		a2=a4/2;
		a3=a2+1;
	}


  OnButton107(); //all start

  return TRUE;   
	               
}


//open test

void CModeMan12::OnClickButton1001() 
{
	nPinSetSkipVb=0; //sylee151007 //ACE400 
	nPinSetSkipVc=0; //sylee151007 //ACE400
	nOpenMode1=1;
    OnManualOpen();
}

void CModeMan12::OnClickButton1005() 
{
	nPinSetSkipVb=0; //sylee151007 //ACE400 
	nPinSetSkipVc=0; //sylee151007 //ACE400
    nOpenMode1=2;
    OnManualOpen();
}


  
void CModeMan12::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
	  m_Label1.SetWindowText(LoadMessage(1,"TitleSetupOpen", 101));
 	  m_Label2.SetWindowText(LoadMessage(1,"TitleSetupOpen", 102));
	  m_Label3.SetWindowText(LoadMessage(1,"TitleSetupOpen", 103));
	  m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupOpen", 104));

	//sylee161213-1  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupOpen", 105));
	  m_button1005.SetWindowText(LoadMessage(1,"TitleSetupOpen", 106));
	  m_button103.SetWindowText(LoadMessage(1,"TitleSetupOpen", 107));
 	  m_buttonOk.SetWindowText(LoadMessage(1,"TitleSetupOpen", 108));
   }
} 





int CModeMan12::OnManualOpen() 
{
	int nRet1;
 
	nRet1=Sub71.AOnPoweroff2();  //SYLEE20120702 AOnPoweroff();
 	if(nRet1!=1){
		AfxMessageBox(" Error No 11025, \n   Power all off fail !,   MANUAL SHORT    Start Error!!!"); 
		return 0 ;
	}

	nRet1=Sub71.AOnVSetMulti1(10,10,100,100) ;
 
	if( nRet1!=1){
		AfxMessageBox(" Error No 11026, \n   V SET ERROR !,    MANUAL SHORT    Start Error!!!"); 
		return 0;
	}
   

	Sleep(200);

	if(nFlag_Manual_Run1==1){
 		 OnManualOpen1_Run3_1_Double(); 	//sylee170112-1
	}else if(nFlag_Manual_Run1==2){
 		  OnManualOpen1_Run3_2();//sylee170112
	}else if(nFlag_Manual_Run1==3){
		  OnManualOpen1_Run();  //type4  original		   
	}else if(nFlag_Manual_Run1==4){
		  OnManualOpen1_Run3();	//sylee161215-1   ///type3		   
	}else{
 		 OnManualOpen1_Run3_1_Double(); 	//sylee170112-1
	}

    Sub71.AOnPoweroff2();//sylee170411
 	nJigTestOpen=2;//sylee181008-1

	return 1;
}

 
void CModeMan12::OnButton101() 
{
    char szText[50];
	int nStart,nEnd,nMaxPin1,No1,i1,i2,nCount;
	int nMax[5][2];
 
    int nLoweStart;//sylee160223-3

	int t1,t2;//sylee160223-3

 	
 	//son .CON 파일이 존재한다면
	if(OnPinConvertFlag1==1)//sylee151016-1
	{

		t1=0;t2=0;
		for( int i=0; i<33000; i++){
		   t1=PinMap[2][i];//pin block pin 
			   if( t1>=(4096*2)){
				   t2=i;//i=  user pin
				   break;
			   }
		}

		if(t2<(4096)){
			nLoweStart=2048;
		}else if(t2<(4096*2)){
			nLoweStart=4096;
		}else{
		    nLoweStart=8192;
		}


	}else{
		//nLoweStart=8192;//sylee160223-3
		nLoweStart=a2;//sylee160223-3
	}
	
	if(SysSet12.m_nLowerJigStartPin>0){
		nLoweStart=SysSet12.m_nLowerJigStartPin-1;//sylee220908  //sylee22012  -1
	}
	
	if( nDut==2){
		nCount=nBDLHead1[2][3][1];
	}else if( nDut==3){
		nCount=nBDLHead1[3][3][1];
	}else if( nDut==4){		
		nCount=nBDLHead1[4][3][1];
	}else{
	    nCount=nBDLHead1[1][3][1];
	}

    nStart=1;
	nEnd=4096*2;
	i1=0;
	i2=0;

	nMaxPin1=4096*4;
   
	::ZeroMemory(&nMax, sizeof(nMax)); 
 

   for(int i=1; i<=nCount; i++){

		if( nDut==2){
			 No1=nBDLData1[2][i][1]; 
		}else if( nDut==3){
			 No1=nBDLData1[3][i][1]; 
		}else if( nDut==4){		
			 No1=nBDLData1[4][i][1]; 
		}else{
			 No1=nBDLData1[1][i][1]; 
		}


		 //sylee160223 if(No1<8192){
		 if(No1<nLoweStart){
			  if(i1==0){	 
				  nMax[1][1]=No1; 	 
				  nMax[0][1]=No1;
				  i1=1;
			  }
			  if(nMax[1][1]<No1){//max	
				  nMax[1][1]=No1;
			  }
			  if(nMax[0][1]>No1){//min	
				  nMax[0][1]=No1;
			  }
		  }else{  //lower
			 // i2++;
		     if(i2==0){			 
				  nMax[3][1]=No1; 		 
				  nMax[2][1]=No1;
				  i2=1;
			  }

			   if(nMax[3][0]<No1){//max				 
				  nMax[3][1]=No1;
			   }
			   if(nMax[2][0]>No1){//min			 
				  nMax[2][1]=No1;
				}
		  }

   }




    nStart=nMax[0][1]+1;
    nEnd=nMax[1][1]+1;

/*

	if( nStart<a1){
		nStart=a1;
	}

    if( nStart>a2){
		nStart=a1;
	}

    if( nEnd<a1){
		nEnd=a1;
	}

    if( nEnd>a2){
		nEnd=a1;
	}
*/

 	sprintf(szText, "%d", nStart);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d", nEnd);
    m_edit102.SetWindowText(szText); 
	

}


void CModeMan12::OnButton107() //all start
{
    char szText[50];
	int nStart,nEnd,nMaxPin1,No1,i1,i2,nCount;
	int nMax[5][2];
 
    int nLoweStart;//sylee160223-3

	int t1,t2;//sylee160223-3

 	
 	//son .CON 파일이 존재한다면
	if(OnPinConvertFlag1==1)//sylee151016-1
	{	

		t1=0;t2=0;
		for( int i=0; i<33000; i++){
		   t1=PinMap[2][i];//pin block pin 
			   if( t1>=(4096*2)){
				   t2=i;//i=  user pin
				   break;
			   }
		}

		if(t2<(4096)){
			nLoweStart=2048;
		}else if(t2<(4096*2)){
			nLoweStart=4096;
		}else{
		    nLoweStart=8192;
		}


	}else{
		//nLoweStart=8192;//sylee160223-3
		nLoweStart=a2;//sylee160223-3
	}


	nLoweStart=4096*8;//sylee160223-3


  //sylee221012 	if(SysSet12.m_nLowerJigStartPin>0){
 //sylee221012 		nLoweStart=SysSet12.m_nLowerJigStartPin;//sylee220908  //sylee221012 n -1
 //sylee221012 	}

	
	if( nDut==2){
		nCount=nBDLHead1[2][3][1];
	}else if( nDut==3){
		nCount=nBDLHead1[3][3][1];
	}else if( nDut==4){		
		nCount=nBDLHead1[4][3][1];
	}else{
	    nCount=nBDLHead1[1][3][1];
	}

    nStart=1;
	nEnd=4096*2;
	i1=0;
	i2=0;

	nMaxPin1=4096*4;
   
	::ZeroMemory(&nMax, sizeof(nMax)); 
 

   for(int i=1; i<=nCount; i++){

		if( nDut==2){
			 No1=nBDLData1[2][i][1]; 
		}else if( nDut==3){
			 No1=nBDLData1[3][i][1]; 
		}else if( nDut==4){		
			 No1=nBDLData1[4][i][1]; 
		}else{
			 No1=nBDLData1[1][i][1]; 
		}


		 //sylee160223 if(No1<8192){
		 if(No1<nLoweStart){
			  if(i1==0){	 
				  nMax[1][1]=No1; 	 
				  nMax[0][1]=No1;
				  i1=1;
			  }
			  if(nMax[1][1]<No1){//max	
				  nMax[1][1]=No1;
			  }
			  if(nMax[0][1]>No1){//min	
				  nMax[0][1]=No1;
			  }
		  }else{  //lower
			 // i2++;
		     if(i2==0){			 
				  nMax[3][1]=No1; 		 
				  nMax[2][1]=No1;
				  i2=1;
			  }

			   if(nMax[3][0]<No1){//max				 
				  nMax[3][1]=No1;
			   }
			   if(nMax[2][0]>No1){//min			 
				  nMax[2][1]=No1;
				}
		  }

   }




    nStart=nMax[0][1]+1;
    nEnd=nMax[1][1]+1;

/*

	if( nStart<a1){
		nStart=a1;
	}

    if( nStart>a2){
		nStart=a1;
	}

    if( nEnd<a1){
		nEnd=a1;
	}

    if( nEnd>a2){
		nEnd=a1;
	}
*/

 	sprintf(szText, "%d", nStart);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d", nEnd);
    m_edit102.SetWindowText(szText); 
	

}




void CModeMan12::OnButton102() 
{
    char szText[50];
	int nStart,nEnd,nMaxPin1,No1,i1,i2,nCount;
	int nMax[5][2];
    int nLoweStart;//sylee160223-3

	int t1,t2;//sylee160223-3

 	
 	//son .CON 파일이 존재한다면
	if(OnPinConvertFlag1==1)//sylee151016-1
	{

		t1=0;t2=0;
		for( int i=0; i<33000; i++){
		   t1=PinMap[2][i];//pin block pin 
		   if( t1>=(4096*2)){
			   t2=i;//i=  user pin
			   break;
		   }
		}

		if(t2<(4096)){
			nLoweStart=2048;
		}else if(t2<(4096*2)){
			nLoweStart=4096;
		}else{
		    nLoweStart=8192;
		}
	}else{
		nLoweStart=a2;//sylee160223-3
	}

	if(SysSet12.m_nLowerJigStartPin>0){
		nLoweStart=SysSet12.m_nLowerJigStartPin-1;//sylee220908    //sylee221012  -1
	}


	i1=0;
	i2=0;

   if( nDut==2){
		nCount=nBDLHead1[2][3][1];
	}else if( nDut==3){
		nCount=nBDLHead1[3][3][1];
	}else if( nDut==4){		
		nCount=nBDLHead1[4][3][1];
	}else{
	    nCount=nBDLHead1[1][3][1];
	}



    nStart=4096*2+1;
	nEnd=4096*4;

	nMaxPin1=4096*4;
   ::ZeroMemory(&nMax, sizeof(nMax)); 

   for(int i=1; i<=nCount; i++){
		  
		 if( nDut==2){
			 No1=nBDLData1[2][i][1]; 
		}else if( nDut==3){
			 No1=nBDLData1[3][i][1]; 
		}else if( nDut==4){		
			 No1=nBDLData1[4][i][1]; 
		}else{
			 No1=nBDLData1[1][i][1]; 
		}

		 //sylee160223 if(No1<8192){
		 if(No1<nLoweStart){
			  if(i1==0){	 
				  nMax[1][1]=No1; 	 
				  nMax[0][1]=No1;
				  i1=1;
			  }
			  if(nMax[1][1]<No1){//max	
				  nMax[1][1]=No1;
			  }
			  if(nMax[0][1]>No1){//min	
				  nMax[0][1]=No1;
			  }
		  }else{  //lower
			 // i2++;
		     if(i2==0){			 
				  nMax[3][1]=No1; 		 
				  nMax[2][1]=No1;
				  i2=1;
			  }

			   if(nMax[3][1]<No1){//max				 
				  nMax[3][1]=No1;
			   }
			   if(nMax[2][1]>No1){//min			 
				  nMax[2][1]=No1;
				}
		  }

   }

     nStart=nMax[2][1]+1;
    nEnd=nMax[3][1]+1;

 	sprintf(szText, "%d", nStart);
    m_edit101.SetWindowText(szText);
 	sprintf(szText, "%d", nEnd);
    m_edit102.SetWindowText(szText);  
}

 
void CModeMan12::OnButton104() 
{


 	  CString Str; 
 
  	  Str.Format("D:\\Manual_OPEN1_JigTest.TXT");// SYLEE20120418 최승환DL요청 
	 ::ShellExecute(NULL,"open","notepad.EXE",Str,"NULL",SW_SHOWNORMAL);	   
 
 
}




void CModeMan12::OnButton105() 
{
	CString str; 

	if(nMultyNetTotal<0){
		nDut=1;		
	}else if(nMultyNetTotal>4){
		nDut=1;		
	}else{
		if(nDut<0){
			nDut=1;	
		}else if(nDut>=nMultyNetTotal){
			nDut=1;	
		}else{
			nDut++;
		}
	}

	str.Format("%7d", nDut);
	m_Label8.SetCaption(str);
 

}


void CModeMan12::OnClickButton103() 
{ 
  nStop2=2;
}



void CModeMan12::OnClickOk() 
{
 
	char szText[50];
    nStop2=2;

    m_edit101.GetWindowText( szText,10); 
	nManuaOpenPin1 = atoi( szText );

	m_edit102.GetWindowText( szText,10); 
	nManuaOpenPin2 = atoi( szText );
 //   OnRsModeRange(5,1);  //MODE Range(0~10)

	OnInit_File1(1);//sylee160128

	CDialog::OnOK(); 
	
}




int CModeMan12::OnManualOpen1_Run() //sylee131023 
{

	CString   str,str1;
	CString   strfName,strfName1;

	char  fName[200], fName1[200]; 
    int i , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nCDelay1;
	int nFil,nDisChargeADC1,nHVIADCFlag, nDischargeCount ;
	int nShRes1T,logno1;
	double nISet,dVbR1,dVbR2,InterI;
	int nFailL1CoOpen,nPinA,nPinB,nReCo1,nRetrySet;

// 	short nShRes1[33000];
	int nSQ1Fail[200];//, nSQPinCheck[34000];
	int nSh1Ret1[100],dPinVC[200],dPinVB[200];// pin array//sylee1209

	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200];  
 
    int nIControl, nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR ; 
    int m_nMode1,nMod1;
	double nProRv1;

	int nBDLPin1[DEF_MAX_PIN1];
	int nBDLPin2[DEF_MAX_PIN1];
     char   szText[50]; 
 
     int nlist1;

	 nlist1=0;    

	m_list1.ResetContent();

	str.Format(" < RUN  TYPE  : 3 >  " );	 
  	m_list1.AddString(str); 
	nlist1++;

	str.Format("RUN"); 
	m_Label1001.SetCaption(str);

	/////////////////////////////////////
    //net list load
	// nBDLHead1[1][3][1]  &nBDLData1[1][i][j]);     
//	 nErrorFlag1[1]=0; //sylee160130
	 
     int k1,k2,i1,nMaxPin1;

	 k2=nBDLHead1[1][3][1];

	// ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	// ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));

	 k1=FileSysInfo01.LoadSaveSet12(2);//load 
	 if( k1!=1){
		str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 return 0;
	 }


	if(SysSet12.m_nPinTotMax== PIN_TOT_4K){//4k  //SYLEE121003
		nMaxPin1=4096;		
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_8K){//8k
		nMaxPin1=4096*2;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_16K){//16k
		nMaxPin1=4096*4;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS){//32k //son230216_1
		nMaxPin1=4096*8;
	}
	else{ // 20K, 24K, Org 32K
		nMaxPin1=4096*8;
	}
	
//	int *nBDLPin1 = new int[nMaxPin1+1];
//	int *nBDLPin2 = new int[nMaxPin1+1];


	 	str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;

		str.Format("  ***> Manual Open :  START  *******  ");			 
		m_list1.AddString(str); 
		nlist1++;
	
		DoEvents();  

     // int *nBDLPin1 = new int[nMaxPin1+1];
 //	 int *nBDLPin2 = new int[nMaxPin1+1];   
	 

	 for(int ik1=0; ik1<=nMaxPin1; ik1++){
		 nBDLPin1[ik1]=0;
		 nBDLPin2[ik1]=0;
	 }

    // ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
   //  ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));



	 					if(nOpenMode1==2){//pcb open


						}else{  //jig up
 							 for(  i1=1 ; i1<= k2 ; i1++){		 
								   k1=nBDLData1[1][i1][1];
								   nBDLPin1[k1+1]=1;	  
							 }  

							 k1=0;
							 for(  i1=1 ; i1<=nMaxPin1 ; i1++){		 
								 if( nBDLPin1[i1]==1){
									  nBDLPin1[0]=nBDLPin1[0]+1;
									  k1++;
									  nBDLPin2[k1]=i1;
								 }		    
							 }
							 nBDLPin2[0]=k1;
						}
  
	//////////////////////////////////////





     nMod1=1;
	 //m_nMode1=6;
	 m_nMode1=7;//sylee161216-1
	 nProRv1=150;//raw   100 R

	

	 //====================

	m_edit103.GetWindowText( szText,10); 
	nManuaOpenR1 = atoi( szText );
	if( nManuaOpenR1<1){
		nManuaOpenR1=1;
	}
 
/*
 	if(nManuaOpenR1>200 ){
		nManuaOpenR1=200;
	}
*/

 	if(nManuaOpenR1>10000 ){//SYLEE170110
		nManuaOpenR1=10000;
	}

	str.Format("%d",nManuaOpenR1);
	m_edit103.SetWindowText(str); 


 
      m_nMode1=51;//100V
 

	if( nManuaOpenR1>1000){
        m_nMode1=m_nMode1+2;
	}else if( nManuaOpenR1>200){
		m_nMode1=m_nMode1+1;
	}




	m_edit101.GetWindowText( szText,10); 
	nST1 = atoi( szText );
	if( nST1<1){
		nST1=1;
	}
	if(nST1>Def_SwitchMaxPin ){
		nST1=Def_SwitchMaxPin;
	}
	str.Format("%d",nST1);
	m_edit101.SetWindowText(str); 
 

	m_edit102.GetWindowText( szText,10); 
	nST2 = atoi( szText );
	if( nST2<1){
		nST2=1;
	}
	if(nST2>Def_SwitchMaxPin ){
		nST2=Def_SwitchMaxPin;
	}
	str.Format("%d",nST2);
	m_edit102.SetWindowText(str); 
 /////////////////////////////////////////////////





	Sub71.R1=nManuaOpenR1;  

	 int nRet;
	 nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412
	 if( nRet!=1){
		str.Format("ERRor NO 3005,   \n JIg open test:  Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
		AfxMessageBox( str);
		return 0; 
	 }

	  RCalCovert(m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER);   // m_nMode1 1~76   , //son220412_3 
	  nProRv1=Sub71.R1;
	  //===============================================================================================


 
    logno1=0;    nShRes1T=0;	nStop=0;
	ipFlag=0;	n4Wire=0;	ipEnd=1; 
	nCDelay1=100;
  	nDisChargeADC1=10;//testmode
 	nDischargeCount=20;//sylee20120830
   
	nHVIADCFlag=1;//SYLEE121018   //waiting 
	nIControl=0;
 

//	::ZeroMemory (&nShRes1, sizeof(nShRes1));
	::ZeroMemory (&nData, sizeof(nData));
   	::ZeroMemory (&dR1, sizeof(dR1));
   	::ZeroMemory (&dVB1, sizeof(dVB1));
	::ZeroMemory (&dVC1, sizeof(dVC1));
	::ZeroMemory (&dI1, sizeof(dI1));
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory (&fName, sizeof(fName));
	::ZeroMemory (&fName1, sizeof(fName1));
	::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
	::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907
 
 
 
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//mode ( 1~65)
//step ( open, ㅏushort, short, hr, hv  4w) 

	

   nCC= (int) dRec[m_nMode1][1];
   nVRel= (int)dRec[m_nMode1][2];
   nVSet= (int) dRec[m_nMode1][3];
   nIR= (int)dRec[m_nMode1][4];
   nISet= dRec[m_nMode1][5];
   nDelay10= (int)dRec[m_nMode1][6]*10;
   nFil=(int)dRec[m_nMode1][8];

   nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
   nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
   nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629

   if( nISet<0.1){
	   AfxMessageBox(" Error No 12025, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
   }

    
  
   //ADC COUNT
    Sub71.nADCount1 = (int)dRec[m_nMode1][7];
	
  			 
		 if(nIR==5){//uA
			InterI=	10.0;
		 }else{
			InterI=nISet*0.9;
		 }

//=========================================================================================
//interlock    

   		if( nCC!=1 && nCC!=2 ){
			AfxMessageBox(" Error No 12024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
 			str.Format("Error"); 
     		m_Label1001.SetCaption(str);
 			return 0;
		}

		if( nVSet<1 || nVSet>300 ){
			AfxMessageBox(" Error No 12025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
			str.Format("Error"); 
     		m_Label1001.SetCaption(str);
 			return 0;
		}

		if( nVRel<1  || nVRel>4 ){ 
			AfxMessageBox(" Error No 12025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
			str.Format("Error"); 
     		m_Label1001.SetCaption(str); 
			return 0; 
		}

  
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//설정부 
	
		
 //sylee130903	Lo1=200;

	   Lo1=1000;//sylee130903  200nS-> 1sec  //sylee150708  200->1000
       Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

 Task101:    

        Sub71.AOnReadVB1Run1(nVRel); 

		if(nVSet<=5){
			dVbR2=nVSet*(0.35);//10%
		}else{
			dVbR2=nVSet*(0.1);//10%
		}
		dVbR1=nVSet-dVbR2;
		dVbR2=nVSet+dVbR2;
 
		if( Sub71.V1>300){
			 if(Lo1-->0){					 					
		//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
				 Sub71.Delay_100ns(30000);  //1ms     //sylee150708  1->3
				 goto Task101;
			 }
			 str.Format("Error No 12001,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
			 AfxMessageBox(str);	
   			 str.Format("Error"); 
     		 m_Label1001.SetCaption(str);
       		 return 0;
		}

		if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
			 if(Lo1-->0){
		//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
				 Sub71.Delay_100ns(10000);  //1ms   
				 goto Task101;
			 }					   
			 str.Format("Error No 12013,\n \n\n  Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
			 AfxMessageBox(str);						 
			 str.Format("Error"); 
     		 m_Label1001.SetCaption(str);
   			return 0;
		}
 
 
 
 	 	Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 
 
        Sub71.FilterSet(nFil);//sylee130702 
		if(nIFilter==3){
			AfxMessageBox(" Error No 12023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
			nIFilter=1;
		}
		Sub71.I_Filter(nIFilter);//sylee130629

	 	Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE20111213     //vb RELAY ON
  	
		Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118
 
 
//--------------------------------------------------------------------------------     
	    ipEnd=1;
		nEnd=0;

	 
		 nRCount=nBDLData3[1][0][1];
		 if( g_nPieceMax1>1 ){//PIECE
		   ipEnd=g_nPieceMax1;
		   ipFlag=1;
		 } 	 
	 	
 /*
 		if( nVSet>20){
	    	nLoop=1;
		    str.Format("Error No 12313,\n \n\n  MANUAL OPEM  START ERROR    VSET >20V !!    "); 
		    AfxMessageBox(str);
            str.Format("Error"); 
     		m_Label1001.SetCaption(str);
            return 0;
		}else{
			nLoop=0;
		}

*/
	
nLoop=0;
		


ipFlag=2;
ipEnd=1;

nRCount=k1;
nRetrySet=3;


nFailL1CoOpen=0;
nReCo1=0;
nStop2=0;

	if(nOpenMode1==2){//pcb open
		nRCount=nBDLData2[1][0][1]+1 ;
	}


	

short nPin1[Def_SwitchMaxPin];

::ZeroMemory(&nPin1, sizeof(nPin1));


	for( int ip3=1; ip3<=ipEnd; ip3++ ){  //SYLEE161213-2
		if(nRCount<=0){		 
			continue;//sylee20120402
		}
		 for(int  i3=1; i3< nRCount; i3++){ 
			nPinA=nBDLData2[1][i3][1];		 
			nPinB=nBDLData2[1][i3][2];		 

			if((nST1<=(nPinA+1)) &&((nPinA+1)<=nST2)){//SYLEE161215
				nPin1[nPinA]=1;
			}

			if((nST1<=(nPinB+1)) &&((nPinB+1)<=nST2)){//SYLEE161215
			    nPin1[nPinB]=1;	 		 
			}
		 }
	}




//====================================================================================================================================
for( int ip=1; ip<=ipEnd; ip++ ){ 
 
	if(nRCount<=0){
		//ERROR DEBUG   PIECE=ip NO LIST.
		continue;//sylee20120402
	}

	 for( i=1; i< nRCount; i++){ //SYLEE20111212

         		 	DoEvents(); 


					if( nStop2==2){
						str.Format(" *****  STOP  *******");			 		
   						m_list1.AddString(str); 
						nlist1++;
						goto Task211;					

					}
 
 
 Task1:

			
						if(nOpenMode1==2){//pcb open
							nPinA=nBDLData2[1][i][1];		 
							nPinB=nBDLData2[1][i][2];
						}else{
							nPinA=nBDLPin2[i]-1;		 
							nPinB=nBDLPin2[i+1]-1;
						}



						
					//	nPinA=i;//sylee161214


						if( nPin1[nPinA]!=1){//sylee161214
							continue;
						}

                     	if( nPin1[nPinB]!=1){//sylee161214
							continue;
						}

				 

 			 		    Sub71.AOnPinSet1(2, nPinB, 1); //vc  
     				    Sub71.AOnPinSet1(2, nPinA, 2);   


  					Sub71.Delay_100ns(nDelay10); 
				
					if( nCC==2){ 
						  Sub71.AOnReadV1I1(7);     	 		
					}else{ 
						  Sub71.AOnReadV1I1(nVRel);  
					}  
 					
					Sub71.OnPinAllReset1(); ///PIN ALL RESET 
  					
					if(Sub71.Y1<0.0000001){
						Sub71.Y1=0.0000001;
					} 

 					if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
						 if( nIR==1 || nIR==2 ){
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
						}else if( nIR==3 ||  nIR==4 ){
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
						}else if( nIR==5 ){	//sylee121115
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
						} 
					}else{
						if( nIR==1 ||  nIR==2 ){
							Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
						}else if( nIR==3 ||  nIR==4 ||  nIR==5){
						 	Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
						}      
					}


					if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
				      	Sub71.R1=9999999;
					}

				    if( Sub71.Y1<nISet*0.7 ){//sylee211009
						Sub71.R1=9999999;
					}


					int Error;

					if(nOpenMode1==2){
					    if( Sub71.R1<=nProRv1){//pass 
							Error=0;
						}else{
							Error=1;
						}
					}else{
					    if( Sub71.R1>=nProRv1){//pass 
							Error=0;
						}else{
							Error=1;
						}
					} 
  


					if(Error==1){
  							 nReCo1++;
							 if( nReCo1<nRetrySet){											 
								 Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
 							     goto  Task1;
							 }
							 nReCo1=0;
							  
							if(nFailL1CoOpen<3000){//array size nFailL1[
		 
 								nFailL1CoOpen++;
								
                     			nFailL1Open[nFailL1CoOpen][F_VB_PIN]=nPinA+1;//pin 1~                     //son  4:F_VB_PIN
								nFailL1Open[nFailL1CoOpen][F_VC_PIN]=nPinB+1;//pin 1~                     //son  5:F_VC_PIN
                                                                                                                                 
								nFailL1Open[nFailL1CoOpen][F_RAW_R]=Sub71.R1 ;//Sub71.R1                  //son  6:F_RAW_R
								nFailL1Open[nFailL1CoOpen][F_PRORV_RAW_R]=nProRv1;//nProRv1 판정 rawR     //son 17: F_PRORV_RAW_R

								//son220419_6 OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2  //son220412
								//son220419_6 nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2

                                int calPart =FileSysInfo01.checkUseCalDataPart(nFailL1Open[i][F_VB_PIN]-1,  nFailL1Open[i][F_VC_PIN]-1);    //son220419_2
                                RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart); //son220419_6
                                nFailL1Open[nFailL1CoOpen][F_REAL_R]=Sub71.R1 ;          //son220419_6 16으로 잘못된 것을 15:F_REAL_R로 수정

								
  								if(nFailL1CoOpen >=SysInfo05.m_nConEr){
 									goto Task211;
								}
 						 
							}


					}//}else{//fail  //SYLEE20120117


				//	}


	   }////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


}//ip
 


Task211:

//==============================================================================================================================
//################################################################################################################################

       Sub71.OnPinAllReset1(); ///PIN ALL RESET 
       Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
  	   Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
 	   Sub71.FilterSet(0);//sylee130704 
	   Sub71.I_Filter(0);//sylee130704
 	   Sub71.AOnReadV1Enable1(5); //sylee130711

	   ////////////////////////
	   //[PRINTING 
    

	   
	   
  	FILE *fp;  
	int nFile1;

	nFile1=0;

 
	::ZeroMemory(&fName , sizeof(fName ));
	 strcat( fName, "D:\\Manual_OPEN1_JigTest.TXT" ); 

	fp = fopen(fName,"wt"); 
	if(fp == NULL){
		nFile1=2;
	//	return 0;
	}



  
	 	str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 if(nFile1==0) fprintf(fp, "%s  \n",str );
	 
   	if( nFailL1CoOpen <=0){
		str.Format("  ***>    Open  NO Error *******  ");			 
	}else{
		str.Format("  ===>  Result : [Open  Error] => %d  ", nFailL1CoOpen );			 		
	}
  	m_list1.AddString(str); 
	nlist1++;
	 if(nFile1==0) fprintf(fp, "%s  \n",str );


	str.Format(" ");			 		
   	m_list1.AddString(str); 
	nlist1++;
	 if(nFile1==0) fprintf(fp, "%s  \n",str );

	if( nFailL1CoOpen==SysInfo05.m_nConEr){
 		str.Format("  ***>    Open Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nConEr );			 		
   		m_list1.AddString(str); 
		nlist1++;
		 if(nFile1==0) fprintf(fp, "%s  \n",str );
	}

		str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 if(nFile1==0) fprintf(fp, "%s  \n",str );
 
	for( i=1 ; i<=nFailL1CoOpen ; i++){
     

		
	   if( nRdisplay1!=1){
		      str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f  ",i, nFailL1Open[i][4],nFailL1Open[i][5] );			 
		}else{

		   if(m_nMode1==51){//100V

			//sylee221014	 if(nFailL1Open[i][16]>=200){
			   	 if(nFailL1Open[i][F_REAL_R]>=200){//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f,  R> 200 ohm ",i, nFailL1Open[i][4] ,nFailL1Open[i][5] );//sylee161214
				}else{

		//sylee181217-3	 if( SysSet13.m_nSet5==1 ){//sylee161221
		//sylee181217-3			            str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f,   R= %7.0f ohm  ",i, nFailL1Open[i][4],nFailL1Open[i][5],nFailL1Open[i][16]  );//sylee161214
		//sylee181217-3				  }else{
						  	 str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f  ",i, nFailL1Open[i][4],nFailL1Open[i][5]  );//sylee161214
		//sylee181217-3				  }
				}


		   }else{


			//sylee221014	if(nFailL1Open[i][16]>1000){
			   	if(nFailL1Open[i][F_REAL_R]>1000){//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f,  R> 1k ohm ",i, nFailL1Open[i][4] ,nFailL1Open[i][5] );//sylee161214
				}else{
				//sylee181217-3		 if( SysSet13.m_nSet5==1 ){//sylee161221
				 //sylee181217-3	        str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f,   R= %7.0f ohm  ",i, nFailL1Open[i][4],nFailL1Open[i][5],nFailL1Open[i][16]  );//sylee161214
				//sylee181217-3		 }else{
						 str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f  ",i, nFailL1Open[i][4],nFailL1Open[i][5]  );//sylee161214
				//sylee181217-3		 }
				}
		   }

		}


		m_list1.AddString(str); 
		nlist1++;
		 if(nFile1==0) fprintf(fp, "%s  \n",str );
	}
 		str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 if(nFile1==0) fprintf(fp, "%s  \n",str );



		if( nStop2==2){
			str.Format("  ***> Manual Open Check    STOP.  *******  ");	 
		}else{
			str.Format("  ***> Manual Open Check    FINISH.  *******  ");			 
		}
		m_list1.AddString(str); 
		nlist1++;
		 if(nFile1==0) fprintf(fp, "%s  \n",str );


	   if( nStop2==2){
		   	str.Format("STOP"); 
	   }else{
		   	str.Format("FINISH"); 	 
	   }
		
	
     	m_Label1001.SetCaption(str);
//		 if(nFile1==0) fprintf(fp, "%s  \n",str );


        if(nFile1==0)  fclose(fp); 

		
	 //	delete [] nBDLPin1; 
     //   delete [] nBDLPin2;  

//::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);
    
	
	return 1;

}//end of CChildView3::OnManualShort1(){//sylee131023







int CModeMan12::OnManualOpen1_Run3() //sylee131023 //kunshan ss  //sylee161214-1
{
 
 	CString   str,str1,strfName,strfName1;

	char fName[200], fName1[200]; 
    int  i , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nCDelay1;
	int nFil,nDisChargeADC1,nHVIADCFlag, nDischargeCount ;
	int nShRes1T,logno1; 
	int nIControl, nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR, m_nMode1,nMod1;
	int nFailL1CoOpen,nPinA,nPinB,nReCo1,nRetrySet;
	int nlist1, SysInfo05m_nConEr1;//sylee151218
	int k1,k2 ,nMaxPin1;

	double nISet,dVbR1,dVbR2,InterI,nProRv1; 
	int nStarPin1;//sylee160115

  
	int nSQ1Fail[200], nSh1Ret1[100],dPinVC[200],dPinVB[200];// pin array//sylee1209

	double nT[10],nData[130][15], dR1[11][200], dVB1[11][200],dVC1[11][200],dI1[11][200]; 
	char szText[100];
	_int64   tStart11, tStart12 , tStart13; //sylee160127-5

//  char   szText[50];
	int  nOpen1;

//int nBDLPin1[DEF_MAX_PIN1];
//int nBDLPin2[DEF_MAX_PIN1];
     short nPinE1[DEF_MAX_PIN1]; 
 

	if(SysInfo05.m_nConEr>1000){//sylee151218
	    SysInfo05m_nConEr1=10000;//sylee151218
	}else{
		SysInfo05m_nConEr1=SysInfo05.m_nConEr;//sylee151218
	}

	 nlist1=0;    

	m_list1.ResetContent();


    str.Format(" < RUN  TYPE  : 4 >  " );	 
  	m_list1.AddString(str); 
	nlist1++;

	DoEvents();
	DoEvents();
	str.Format("RUN"); 
	m_Label1001.SetCaption(str);
 
	 k2=nBDLHead1[1][3][1];
 
	 k1=FileSysInfo01.LoadSaveSet12(2);//load 

	 if( k1!=1){
		str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 return 0;
	 }


	if(SysSet12.m_nPinTotMax== PIN_TOT_4K){//4k  //SYLEE121003
		nMaxPin1=4096;		
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_8K){//8k
		nMaxPin1=4096*2;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_16K){//16k
		nMaxPin1=4096*4;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS){//32k //son230216_1
		nMaxPin1=4096*8;
	}
	else{ // 20K, 24K, Org 32K
		nMaxPin1=4096*8;
	}



	int nMaxPin1Tmp1;

	// short *nPinE1 = new short[nMaxPin1+1];

	m_edit102.GetWindowText( szText,10); 
	nMaxPin1Tmp1 = atoi( szText );

	if(nMaxPin1Tmp1<2){	 
		str1.Format("%d",nMaxPin1); 
        m_edit102.SetWindowText(str1);
 
	}
    else  if(nMaxPin1Tmp1<nMaxPin1){
		nMaxPin1=nMaxPin1Tmp1;
	 
	}
	else{
	    str1.Format("%d",nMaxPin1); 
        m_edit102.SetWindowText(str1);
		 
	}
//===========================================================
    m_edit101.GetWindowText( szText,10); 
	nStarPin1 = atoi( szText );

	if( nStarPin1<1){
		nStarPin1=1;
	}
	if(nStarPin1>=nMaxPin1){
		nStarPin1=1;
	} 

//	nStarPin1;//sylee160115
    str.Format("%d",nStarPin1);
	m_edit101.SetWindowText(str); 
  //===========================================================

	 	str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;

	    str.Format(" * [ MANUAL : OPEN TEST ] *    ");	
		
		m_list1.AddString(str); 
		nlist1++;

		DoEvents();   

  
	 for(int ik1=0; ik1<=nMaxPin1; ik1++){
 		 nPinE1[ik1]=0;//sylee160114
	 }

	//////////////////////////////////////

     nMod1=1;
	 m_nMode1=6;
	 nProRv1=150;//raw   100 R
	 //====================

	m_edit103.GetWindowText( szText,10); 
	nManuaOpenR1 = atoi( szText );
	if( nManuaOpenR1<1){
		nManuaOpenR1=1;
	}
 	if(nManuaOpenR1>10000 ){
		nManuaOpenR1=10000;
	}
   if(nManuaOpenR1<200 ){
      m_nMode1=51;//100V
   }else  if(nManuaOpenR1<1000 ){  
	   m_nMode1=52;//100V
   }else{
	   m_nMode1=53;//100V
   }

	str.Format("%d",nManuaOpenR1);
	m_edit103.SetWindowText(str); 

	Sub71.R1=nManuaOpenR1;  

	 int nRet;
	 nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412
	 if( nRet!=1){
		str.Format("ERRor NO 3005,   \n JIg open test:  Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
		AfxMessageBox( str);
		return 0; 
	 }

	  RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER );   // m_nMode1 1~76   ,    //son220412_3 
	  nProRv1=Sub71.R1;
	  //===============================================================================================
 
    logno1=0;    nShRes1T=0;	nStop=0;
	ipFlag=0;	n4Wire=0;	ipEnd=1; 
	nCDelay1=100;
  	nDisChargeADC1=10;//testmode
 	nDischargeCount=20;//sylee20120830
   
	nHVIADCFlag=1;//SYLEE121018   //waiting 
	nIControl=0;
 

//	::ZeroMemory (&nShRes1, sizeof(nShRes1));
	::ZeroMemory (&nData, sizeof(nData));
   	::ZeroMemory (&dR1, sizeof(dR1));
   	::ZeroMemory (&dVB1, sizeof(dVB1));
	::ZeroMemory (&dVC1, sizeof(dVC1));
	::ZeroMemory (&dI1, sizeof(dI1));
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory (&fName, sizeof(fName));
	::ZeroMemory (&fName1, sizeof(fName1));
	::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
	::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907
 
 
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//mode ( 1~65)
//step ( open, ㅏushort, short, hr, hv  4w) 

   nCC= (int) dRec[m_nMode1][1];
   nVRel= (int)dRec[m_nMode1][2];
   nVSet= (int) dRec[m_nMode1][3];
   nIR= (int)dRec[m_nMode1][4];
   nISet= dRec[m_nMode1][5];
   nDelay10= (int)dRec[m_nMode1][6]*10;
   nFil=(int)dRec[m_nMode1][8];

   nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
   nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
   nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629

   if( nISet<0.1){
	   AfxMessageBox(" Error No 12025, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
   }
  
  
   //ADC COUNT
    Sub71.nADCount1 = (int)dRec[m_nMode1][7];
	
  			 
		 if(nIR==5){//uA
			InterI=	10.0;
		 }else{
			InterI=nISet*0.9;
		 }

//=========================================================================================
//interlock    

   		if( nCC!=1 && nCC!=2 ){
			AfxMessageBox(" Error No 12024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
 			str.Format("Error"); 
     		m_Label1001.SetCaption(str);
 			return 0;
		}

		if( nVSet<1 || nVSet>300 ){
			AfxMessageBox(" Error No 12025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
			str.Format("Error"); 
     		m_Label1001.SetCaption(str);
 			return 0;
		}

		if( nVRel<1  || nVRel>4 ){ 
			AfxMessageBox(" Error No 12025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
			str.Format("Error"); 
     		m_Label1001.SetCaption(str); 
			return 0; 
		}

  
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//설정부 
	
		
 //sylee130903	Lo1=200;

	   Lo1=1000;//sylee130903  200nS-> 1sec  //sylee150708  200->1000
       Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

 Task101:    

        Sub71.AOnReadVB1Run1(nVRel); 

		if(nVSet<=5){
			dVbR2=nVSet*(0.35);//10%
		}else{
			dVbR2=nVSet*(0.1);//10%
		}
		dVbR1=nVSet-dVbR2;
		dVbR2=nVSet+dVbR2;
 
		if( Sub71.V1>300){
			 if(Lo1-->0){					 					
		//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
				 Sub71.Delay_100ns(30000);  //1ms     //sylee150708  1->3
				 goto Task101;
			 }
			 str.Format("Error No 12001,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
			 AfxMessageBox(str);	
   			 str.Format("Error"); 
     		 m_Label1001.SetCaption(str);
       		 return 0;
		}

		if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
			 if(Lo1-->0){
		//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
				 Sub71.Delay_100ns(10000);  //1ms   
				 goto Task101;
			 }					   
			 str.Format("Error No 12013,\n \n\n  Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
			 AfxMessageBox(str);						 
			 str.Format("Error"); 
     		 m_Label1001.SetCaption(str);
   			return 0;
		}
 
 
 
 	 	Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 
 
        Sub71.FilterSet(nFil);//sylee130702 
		if(nIFilter==3){
			AfxMessageBox(" Error No 12023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
			nIFilter=1;
		}
		Sub71.I_Filter(nIFilter);//sylee130629

	 	Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE20111213     //vb RELAY ON
  	
		Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118
 
 
//--------------------------------------------------------------------------------     
	    ipEnd=1;
		nEnd=0;

	 
		 nRCount=nBDLData3[1][0][1];
		 if( g_nPieceMax1>1 ){//PIECE
		   ipEnd=g_nPieceMax1;
		   ipFlag=1;
		 } 	 
	 	
  

	nLoop=0;
		


ipFlag=2;
ipEnd=1;

nRCount=k1;
nRetrySet=3;


nFailL1CoOpen=0;
nReCo1=0;
nStop2=0;

	nRCount=nMaxPin1;//SYLEE160114

	ipEnd=1;//SYLEE160114

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//nPinA search

	int nPinA_Enable;//sylee160114
	nPinA_Enable=0;//sylee160114
//====================================================================================================================================

//	if(nRCount<=0){
		//ERROR DEBUG   PIECE=ip NO LIST.
//		continue;//sylee20120402
//	} 

	    str.Format("             ( Start=%d  ~  End=%d ) ", nStarPin1 , nMaxPin1   ); 
  		m_list1.AddString(str); 
		nlist1++;

		str.Format("     "); 
		m_list1.AddString(str); 
		nlist1++;

	 	str.Format("  ########  START  ######## "); 
		m_list1.AddString(str); 
		nlist1++;



    for( i=nStarPin1; i<nRCount; i++){ //SYLEE20111212
 					if(nPinA_Enable==1){//sylee160114
 						break;
					}
 		            nPinA=i;		
					nPinB=i+1;

					if(nPinA==nPinB){
						continue;
					}    
//===================================================
			
		      int nCount,No1,nEnable1;

				No1=-1;
				nEnable1=0;
					
				if( nDut==2){
					nCount=nBDLHead1[2][3][1];
				}else if( nDut==3){
					nCount=nBDLHead1[3][3][1];
				}else if( nDut==4){		
					nCount=nBDLHead1[4][3][1];
				}else{
					nCount=nBDLHead1[1][3][1];
				}


				for(int k1=1; k1<=nCount; k1++){
				    if( nDut==2){
						 No1=nBDLData1[2][k1][1]; 
					}else if( nDut==3){
						 No1=nBDLData1[3][k1][1]; 
					}else if( nDut==4){		
						 No1=nBDLData1[4][k1][1]; 
					}else{
						 No1=nBDLData1[1][k1][1]; 
					}
					if( i==No1){
						nEnable1=1;
						break;
					}
				}

				if(nEnable1!=1){
				    // str.Format("Error No 12801,\n \n\n Manual Open pin error (pin no.=%d), i    "); 
		           // AfxMessageBox(str);
					nEnable1=nEnable1;
					continue;
				}
//===================================================

				if( nStop2==2){
					str.Format(" *****  STOP  *******");			 		
   					m_list1.AddString(str); 
					nlist1++;
					goto Task211;					

				}


// Task12:

     				Sub71.AOnPinSet1(2, nPinA-1, 1); 
 			 		Sub71.AOnPinSet1(2, nPinB-1, 2); //vc  
 
					Sub71.Delay_100ns(nDelay10); //sylee160127
				
					if( nCC==2){ 
						  Sub71.AOnReadV1I1(7);     	 		
					}else{ 
						  Sub71.AOnReadV1I1(nVRel);  
					}  
 					
					Sub71.OnPinAllReset1(); ///PIN ALL RESET 
  					
					if(Sub71.Y1<0.0000001){
						Sub71.Y1=0.0000001;
					} 

 					if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
						 if( nIR==1 || nIR==2 ){
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
						}else if( nIR==3 ||  nIR==4 ){
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
						}else if( nIR==5 ){	//sylee121115
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
						} 
					}else{
						if( nIR==1 ||  nIR==2 ){
							Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
						}else if( nIR==3 ||  nIR==4 ||  nIR==5){
						 	Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
						}      
					}


					if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
				      	Sub71.R1=9999999;
					}
					if( Sub71.Y1<nISet*0.7 ){//sylee211009
						Sub71.R1=9999999;
					}

					if( Sub71.R1<=3000){//pass //sylee160114
 
							Sub71.OnPinAllReset1(); ///PIN ALL RESET 
							Sub71.Delay_100ns(30000); //2mS
							Sub71.AOnPinSet1(2, nPinA-1, 1); 
 			 				Sub71.AOnPinSet1(2, nPinB-1, 2); //vc  
  							Sub71.Delay_100ns(nDelay10); 
						
							if( nCC==2){ 
								  Sub71.AOnReadV1I1(7);     	 		
							}else{ 
								  Sub71.AOnReadV1I1(nVRel);  
							}  		 
							Sub71.OnPinAllReset1(); ///PIN ALL RESET 
  							
							if(Sub71.Y1<0.0000001){
								Sub71.Y1=0.0000001;
							} 

 							if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
								 if( nIR==1 || nIR==2 ){
									Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
								}else if( nIR==3 ||  nIR==4 ){
									Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
								}else if( nIR==5 ){	//sylee121115
									Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
								} 
							}else{
								if( nIR==1 ||  nIR==2 ){
									Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
								}else if( nIR==3 ||  nIR==4 ||  nIR==5){
						 			Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
								}      
							}

							if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
				      			Sub71.R1=9999999;
							}

			               if( Sub71.Y1<nISet*0.7 ){//sylee211009
								Sub71.R1=9999999;
							}

					 		if( Sub71.R1<=nProRv1){//pass //sylee160114
								nPinE1[i]=1;//sylee160114
								nPinA_Enable=1;//sylee160114
							}
				 
					}

 
	 }////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 
 

	 if(nPinA_Enable!=1){
		 //all open
		 	str.Format("           ");			 		
   			m_list1.AddString(str); 
			nlist1++;

		 	str.Format(" --> ERROR: ALL POINT is OPEN.   (Test point=%d)" , (nMaxPin1-nStarPin1+1) );			 		
   			m_list1.AddString(str); 
			nlist1++;

		 nRCount=0;
	 }
 
//==========================================================================================
//====================================================================================================================================
 
 Sub71.OnPinAllReset1(); ///PIN ALL RESET 

//	if(nRCount<=0){
		//ERROR DEBUG   PIECE=ip NO LIST.
//		continue;//sylee20120402
//	}

	 for( i=nStarPin1; i<=nRCount; i++){ //SYLEE20111212

					nPinB=i;

					if(nPinA==nPinB){
						continue;
					}

         		 	DoEvents(); 

					if( nStop2==2){
						str.Format(" *****  STOP  *******");			 		
   						m_list1.AddString(str); 
						nlist1++;
						goto Task211;
					} 


					 int nLoop2; //sylee160127-5
					 nLoop2=0; //sylee160127-5

 
 
 			 		Sub71.AOnPinSet1(2, nPinB-1, 1); //vc  
     				Sub71.AOnPinSet1(2, nPinA-1, 2); 

					tStart11=Get_uSecond1();//uS 


Task500://sylee160127-5

				    nLoop2++;//sylee160127-5 

					if(nLoop2==1){//sylee160127-5
					   Sub71.Delay_100ns(300);//100uS//sylee160127-5
					}else{
						Sub71.Delay_100ns(100);//3uS//sylee160127-5
					}//sylee160127-5

				
					if( nCC==2){ 
						  Sub71.AOnReadV1I1(7);     	 		
					}else{ 
						  Sub71.AOnReadV1I1(nVRel);  
					}  					
				

  					
					if(Sub71.Y1<0.0000001){
						Sub71.Y1=0.0000001;
					} 

 					if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
						 if( nIR==1 || nIR==2 ){
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
						}else if( nIR==3 ||  nIR==4 ){
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
						}else if( nIR==5 ){	//sylee121115
							Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
						} 
					}else{
						if( nIR==1 ||  nIR==2 ){
							Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
						}else if( nIR==3 ||  nIR==4 ||  nIR==5){
						 	Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
						}      
					}

					if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
				      	Sub71.R1=9999999;
					}

				    if( Sub71.Y1<nISet*0.7 ){//sylee211009
						Sub71.R1=9999999;
					}


                      if( Sub71.R1>nProRv1){	//sylee160127-5  //fail				 
							if( nLoop2<50){	//sylee160127-5			 
                                tStart12=Get_uSecond1();//uS 
	                            tStart13=(tStart12-tStart11);	
								if( tStart13< (nDelay10/10)){  	//sylee160127-5
                                   goto Task500;	//sylee160127-5
								}else{
									tStart13=tStart13;//test
								}
							}
 
					   }

					  	Sub71.OnPinAllReset1(); ///PIN ALL RESET 



					int Error;

	 
					if( Sub71.R1<=nProRv1){//pass //sylee160114
						Error=0;
						nPinE1[i]=1;//sylee160114   //pass
					}else{
						Error=1;
						nPinE1[i]=2;//sylee160114
					}

					if(Error==1){
  						 nReCo1++;
						//f( nReCo1<nRetrySet){											 
						//Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
 						//goto  Task1;
						//
						 nReCo1=0;
						  
						if(nFailL1CoOpen<10001){//array size nFailL1[  //sylee160114
	 
 							nFailL1CoOpen++;
							
                     		nFailL1Open[nFailL1CoOpen][4]=nPinA;//pin 1~
							nFailL1Open[nFailL1CoOpen][5]=nPinB;//pin 1~

							nFailL1Open[nFailL1CoOpen][6]=Sub71.R1 ;//Sub71.R1 
							nFailL1Open[nFailL1CoOpen][17]=nProRv1;//nProRv1 판정 							
  				 
						//	if(nFailL1CoOpen >=SysInfo05m_nConEr1){//sylee151218
							if(nFailL1CoOpen >=10000){//sylee151218
 								goto Task211;
							}
 					 
						}


					}//}else{//fail  //SYLEE20120117
 

	   }////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212



Task211:

//==============================================================================================================================
//################################################################################################################################

       Sub71.OnPinAllReset1(); ///PIN ALL RESET 
       Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
  	   Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
 	   Sub71.FilterSet(0);//sylee130704 
	   Sub71.I_Filter(0);//sylee130704
 	   Sub71.AOnReadV1Enable1(5); //sylee130711

	   ////////////////////////
	   //[PRINTING 

	   
	   FILE *fp;  
	int nFile1;

	nFile1=0;

 
	::ZeroMemory(&fName , sizeof(fName ));
	 strcat( fName, "D:\\Manual_OPEN1_JigTest.TXT" ); 

	fp = fopen(fName,"wt"); 
	if(fp == NULL){
		nFile1=2;
	//	return 0;
	}


	  
		 if(nPinA_Enable==1){
			str.Format(" ");			 		
   			m_list1.AddString(str); 
			nlist1++; 
			
            if(nFile1==0) fprintf(fp, "%s  \n",str );
		 
   			if( nFailL1CoOpen <=0){			 
				str.Format("  ***>    Open  NO Error *******  ");			 
			}else{
				str.Format("=> Result:[Test Point]=%d,  [Pass]=%d,[Open]=%d",  (nMaxPin1-nStarPin1+1) ,(nMaxPin1-nStarPin1+1)-nFailL1CoOpen,   nFailL1CoOpen );			 		
			}
  			m_list1.AddString(str); 
			nlist1++;
			if(nFile1==0){fprintf(fp," %s \n",str);}

			str.Format(" ");			 		
   			m_list1.AddString(str); 
			nlist1++;
			if(nFile1==0){fprintf(fp," %s \n",str);}
 
		//	if( nFailL1CoOpen==SysInfo05.m_nConEr){//sylee151218 
	 		if( nFailL1CoOpen==10000){//sylee151218 
 			//	 str.Format("  ***>    Open Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05m_nConEr1 );//sylee151218
				str.Format("  ***>    Open Error Limited Set Count=10000,  Error Over !.   Stopped.  " );//sylee151218
   				m_list1.AddString(str); 
				nlist1++;
				if(nFile1==0){fprintf(fp," %s \n",str);}
			}

			str.Format(" ");			 		
   			m_list1.AddString(str); 
			nlist1++;
			if(nFile1==0){fprintf(fp," %s \n",str);}
 
 
	       int   nPass1,nFail1;
			  nPass1=0; nFail1=0;

			for( i=nStarPin1 ; i<=nMaxPin1 ; i++){
     			
	 
					  if(nPinE1[i]==1){//sylee160114   //pass
							nPass1++;
							nOpen1=-2;
					        nFail1=0; 
							continue;
						}

						nFail1++;
						if(nFail1==1){	//fail
							nOpen1=i;
							if(nPinE1[i+1]==1){//pass
						       str.Format("O: %d   ",   i  ); 
						        m_list1.AddString(str); nlist1++; 
								if(nFile1==0){fprintf(fp," %s \n",str);}
								nOpen1=-1;

							}else{							 
							//	nOpen1=i;
							}
													
						}else{ // fail count  
							 if(i==nMaxPin1){
								if(nFail1>1){
									str.Format("O: %d ~ %d   ",  nOpen1, i   ); 
									  m_list1.AddString(str); nlist1++;
									  if(nFile1==0) fprintf(fp, "%s  \n",str );
									  nFail1=0;
								}
							 }else{
								 if(nPinE1[i+1]==1){//pass
						            str.Format("O: %d ~ %d  ", nOpen1,  i   ); 
									m_list1.AddString(str); nlist1++;
									if(nFile1==0){fprintf(fp," %s \n",str);}
								 }
							 }

						}	
			}

		}//end of 	 if(nPinA_Enable==1){


 		str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		if(nFile1==0){fprintf(fp," %s \n",str);}

		if( nStop2==2){
			str.Format("  ########   STOP.  ########  ");	 
		}else{
			str.Format("  ########   END.   ########   ");			 
		}
		m_list1.AddString(str); 
		nlist1++;
		if(nFile1==0){fprintf(fp," %s \n",str);}


	   if( nStop2==2){
		   	str.Format("STOP"); 
	   }else{
		   	str.Format("FINISH"); 	 
	   }
		
	
     	m_Label1001.SetCaption(str);
		if(nFile1==0){fprintf(fp," %s \n",str);}




		if(nFile1==0)   fclose(fp); 

 //######################################################################################################################333

//	FILE *fp; //SYLEE160114
 
	::ZeroMemory(&fName , sizeof(fName ));
	 strcat( fName, "D:\\ERROR_OPEN1.TXT" );//SYLEE160114

	fp = fopen(fName,"wt");//SYLEE160114
	if(fp == NULL) return 0; //SYLEE160114



		    if(nPinA_Enable==1){ 
	           str.Format("=>[Test Point]=%d\n  [Pass]=%d, [Open]=%d\n\n",  (nMaxPin1-nStarPin1+1) , (nMaxPin1-nStarPin1+1-nFailL1CoOpen), nFailL1CoOpen );			 		
			}else{
				str.Format("=>[Test Point]=%d\n  [Pass]=0, [Open]=%d\n\n",  (nMaxPin1-nStarPin1+1) ,   (nMaxPin1-nStarPin1+1) );			 		
			}
            fprintf(fp, str );
		 //   str.Format("  ( Start=%d ~ End=%d ) \n\n", nStarPin1 , nMaxPin1   ); 
		 //   fprintf(fp, str ); 

			CTime 	curTime = CTime::GetCurrentTime();				 
			fprintf(fp, "  %04d-%02d-%02d  %02d:%02d:%02d \n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());		 
		 //	fprintf(fp, " Model Name : ERROR_OPEN1.TXT \n" );
		//	fprintf(fp, " Model Name: 00LP.TXT \n" );

			str.Format(" \n ####### START ######\n "   ); 
			fprintf(fp, str );

			str.Format("   \n ");
			fprintf(fp, str );

   			if( nFailL1CoOpen >0){
				str.Format(" \n [OPEN]\n"   ); 
				fprintf(fp, str );
			}
			if(nPinA_Enable!=1){ 
				str.Format("  ***>   ALL POINT  is OPEN   \n ");
			    fprintf(fp, str );
			}




   		 if(nPinA_Enable==1){ 

			if( nFailL1CoOpen <=0){			 
				str.Format("  ***>  Open  NO Error ******* \n ");
			    fprintf(fp, str );
			}

	 		if( nFailL1CoOpen==10000){//sylee151218 
 		 		str.Format(" **>Open Error 10000ea Over !.Stopped. \n " );//sylee151218
   				fprintf(fp, str );
			}  

	       int   nPass1,nFail1;
			  nPass1=0; nFail1=0;


			if( nFailL1CoOpen>0){

					for( i=nStarPin1 ; i<=nMaxPin1 ; i++){ 

                        if(nPinE1[i]==1){//sylee160114   //pass
							nPass1++;
							nOpen1=-2;
					        nFail1=0; 
							continue;
						}
					    

						nFail1++;
						if(nFail1==1){	//fail
							nOpen1=i;
							if(nPinE1[i+1]==1){//pass
						       str.Format("O: %d\n",   i  ); 
						        fprintf(fp, str );
								nOpen1=-1;
							}
													
						}else{ // fail count  
							 if(i==nMaxPin1){
								if(nFail1>1){
									str.Format("O: %d ~ %d\n",  nOpen1, i   ); 
									  fprintf(fp, str );
									  nFail1=0;
								}
							 }else{
								 if(nPinE1[i+1]==1){//pass
						            str.Format("O: %d ~ %d\n", nOpen1,  i   ); 
									fprintf(fp, str );
								 }
							 }

						}	
 
 			
					 
//========================================================================
					}
			}else{
				fprintf(fp, " No  Error! " );
			}

		}//end of 	 if(nPinA_Enable==1){

		str.Format(" \n ######## END ########\n "   ); 
		fprintf(fp, str );
		// fprintf(fp, str );		  
     //	m_Label1001.SetCaption(str);
        fclose(fp);//SYLEE160114

		///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		

	::ZeroMemory(&fName, sizeof(fName));
	 strcat( fName, "Z:\\ERROR_OPEN1.TXT" );//SYLEE160114
	fp = fopen(fName,"wt");//SYLEE160114
	if(fp == NULL) return 0; //SYLEE160114



		    if(nPinA_Enable==1){ 
	           str.Format("=>[Test Point]=%d\n  [Pass]=%d, [Open]=%d\n\n",  (nMaxPin1-nStarPin1+1) , (nMaxPin1-nStarPin1+1-nFailL1CoOpen), nFailL1CoOpen );			 		
			}else{
				str.Format("=>[Test Point]=%d\n  [Pass]=0, [Open]=%d\n\n",  (nMaxPin1-nStarPin1+1) ,   (nMaxPin1-nStarPin1+1) );			 		
			}
            fprintf(fp, str );
		 //   str.Format("  ( Start=%d ~ End=%d ) \n\n", nStarPin1 , nMaxPin1   ); 
		 //   fprintf(fp, str ); 

		//	CTime 	curTime = CTime::GetCurrentTime();				 
			fprintf(fp, "  %04d-%02d-%02d  %02d:%02d:%02d \n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());		 
		 //	fprintf(fp, " Model Name : ERROR_OPEN1.TXT \n" );
			fprintf(fp, " Model Name: 00LP.TXT \n" );

			str.Format(" \n ####### START ######\n "   ); 
			fprintf(fp, str );

			str.Format("   \n ");
			fprintf(fp, str );

   			if( nFailL1CoOpen >0){
				str.Format(" \n [OPEN]\n"   ); 
				fprintf(fp, str );
			}
			if(nPinA_Enable!=1){ 
				str.Format("  ***>   ALL POINT  is OPEN   \n ");
			    fprintf(fp, str );
			}




   		 if(nPinA_Enable==1){ 

			if( nFailL1CoOpen <=0){			 
				str.Format("  ***>  Open  NO Error ******* \n ");
			    fprintf(fp, str );
			}

	 		if( nFailL1CoOpen==10000){//sylee151218 
 		 		str.Format(" **>Open Error 10000ea Over !.Stopped. \n " );//sylee151218
   				fprintf(fp, str );
			}  

	       int   nPass1,nFail1;
			  nPass1=0; nFail1=0;


			if( nFailL1CoOpen>0){

					for( i=nStarPin1 ; i<=nMaxPin1 ; i++){ 

                        if(nPinE1[i]==1){//sylee160114   //pass
							nPass1++;
							nOpen1=-2;
					        nFail1=0; 
							continue;
						}
					    

						nFail1++;
						if(nFail1==1){	//fail
							nOpen1=i;
							if(nPinE1[i+1]==1){//pass
						       str.Format("O: %d\n",   i  ); 
						        fprintf(fp, str );
								nOpen1=-1;
							}
													
						}else{ // fail count  
							 if(i==nMaxPin1){
								if(nFail1>1){
									str.Format("O: %d ~ %d\n",  nOpen1, i   ); 
									  fprintf(fp, str );
									  nFail1=0;
								}
							 }else{
								 if(nPinE1[i+1]==1){//pass
						            str.Format("O: %d ~ %d\n", nOpen1,  i   ); 
									fprintf(fp, str );
								 }
							 }

						}	
 
 			
					 
//========================================================================
					}
			}else{
				fprintf(fp, " No  Error! " );
			}

		}//end of 	 if(nPinA_Enable==1){

		str.Format(" \n ######## END ########\n "   ); 
		fprintf(fp, str );
		// fprintf(fp, str );		  
     //	m_Label1001.SetCaption(str);
        fclose(fp);//SYLEE160114

	
	return 1;

}//end of CChildView3::OnManualShort1(){//sylee131023

 





int CModeMan12::OnManualOpen1_Run3_1_Double() //sylee17012  상부 => 하부 
{

	CString   str,str1;
	CString   strfName,strfName1;

	char  fName[200], fName1[200]; 
    int i , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nCDelay1;
	int nFil,nDisChargeADC1,nHVIADCFlag, nDischargeCount ;
	int nShRes1T,logno1; 

	double nISet,dVbR1,dVbR2,InterI;
	int nFailL1CoOpen,nPinA,nPinB,nReCo1,nRetrySet;

// 	short nShRes1[33000];
	int nSQ1Fail[200];//, nSQPinCheck[34000];
	int nSh1Ret1[100],dPinVC[200],dPinVB[200];// pin array//sylee1209

	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200];  
 
    int nIControl, nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR ; 
    int m_nMode1,nMod1;
	double nProRv1;

	int nBDLPin1[DEF_MAX_PIN1];
	int nBDLPin2[DEF_MAX_PIN1];

	char  szText[200]; 
    int nlist1;
	short nPass1[Def_SwitchMaxPin];//sylee161216-1

	int nHalfPin1;//sylee171112-1

 

	 ::ZeroMemory(&nPass1, sizeof(nPass1));//sylee161216-1

    nlist1=0;    

	m_list1.ResetContent();

    str.Format(" < RUN  TYPE  : 1 >  " );	 
  	m_list1.AddString(str); 
	nlist1++;

	str.Format("RUN"); 
	m_Label1001.SetCaption(str);
	/////////////////////////////////////
    //net list load
	// nBDLHead1[1][3][1]  &nBDLData1[1][i][j]);     
	 
     int k1,k2,i1,nMaxPin1;

	 k2=nBDLHead1[1][3][1];
	// ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	// ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));

	 k1=FileSysInfo01.LoadSaveSet12(2);//load 
	 if( k1!=1){
		str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 return 0;
	 }


	if(SysSet12.m_nPinTotMax== PIN_TOT_4K){//4k  //SYLEE121003
		nMaxPin1=4096;		
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_8K){//8k
		nMaxPin1=4096*2;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_16K){//16k
		nMaxPin1=4096*4;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS){//32k //son230216_1
		nMaxPin1=4096*8;
	}
	else{ // 20K, 24K, Org 32K
		nMaxPin1=4096*8;
	}



	nHalfPin1=nMaxPin1/2;//sylee171112-1


	//	int *nBDLPin1 = new int[nMaxPin1+1];
	//	int *nBDLPin2 = new int[nMaxPin1+1];

	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;

	str.Format("  ***> Manual Open :  START  *******  ");			 
	m_list1.AddString(str); 
	nlist1++;


	DoEvents();  

	// int *nBDLPin1 = new int[nMaxPin1+1];
	//	 int *nBDLPin2 = new int[nMaxPin1+1];   
	

	for(int ik1=0; ik1<=nMaxPin1; ik1++){
	    nBDLPin1[ik1]=0;
	    nBDLPin2[ik1]=0;
	}

	// ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	//  ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));



	if(nOpenMode1==2){//pcb open


	}
	else //jig up
	{  
	    for(  i1=1 ; i1<= k2 ; i1++){		 
	   	 k1=nBDLData1[1][i1][1];
	   	 nBDLPin1[k1+1]=1;	  
	    }  

	    k1=0;
	    for(  i1=1 ; i1<=nMaxPin1 ; i1++){		 
	   	 if( nBDLPin1[i1]==1){
	   		 nBDLPin1[0]=nBDLPin1[0]+1;
	   		 k1++;
	   		 nBDLPin2[k1]=i1;
	   	 }		    
	    }
	    nBDLPin2[0]=k1;
	}
  
	/////////////////////////////////////


    nMod1=1;
	m_nMode1=7;//sylee161216-1
	nProRv1=150;//raw   100 R

	//====================

	m_edit103.GetWindowText( szText,10); 
	nManuaOpenR1 = atoi( szText );
	if( nManuaOpenR1<1){
		nManuaOpenR1=1;
	}

/*
if(nManuaOpenR1>1000 ){
	nManuaOpenR1=1000;
}
*/

 
 	if(nManuaOpenR1>200 ){
		nManuaOpenR1=200;
	}
 
  /*
    if(nManuaOpenR1>10000 ){//SYLEE170110
		nManuaOpenR1=10000;
	}
	*/
      m_nMode1=51;//100V 

	if( nManuaOpenR1>1000){
        m_nMode1=m_nMode1+2;
	}
	else if( nManuaOpenR1>200){
		m_nMode1=m_nMode1+1;
	}


	str.Format("%d",nManuaOpenR1);
	m_edit103.SetWindowText(str); 

	Sub71.R1=nManuaOpenR1;  

	/////////////////////////////////////////////////
	m_edit101.GetWindowText( szText,10); 
	nST1 = atoi( szText );
	if( nST1<1){
		nST1=1;
	}
	if(nST1>Def_SwitchMaxPin ){
		nST1=Def_SwitchMaxPin;
	}
	str.Format("%d",nST1);
	m_edit101.SetWindowText(str); 
 

	m_edit102.GetWindowText( szText,10); 
	nST2 = atoi( szText );
	if( nST2<1){
		nST2=1;
	}
	if(nST2>Def_SwitchMaxPin ){
		nST2=Def_SwitchMaxPin;
	}
	str.Format("%d",nST2);
	m_edit102.SetWindowText(str); 
 /////////////////////////////////////////////////
 



	int nRet;
	nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412
	if( nRet!=1){
	    str.Format("ERRor NO 3005,   \n JIg open test:  Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
	    AfxMessageBox( str);
	    return 0; 
	}

	RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER );   // m_nMode1 1~76  //son220412_3 
	nProRv1=Sub71.R1;
	//===============================================================================================


 
	logno1=0;    nShRes1T=0;	nStop=0;
	ipFlag=0;	n4Wire=0;	ipEnd=1; 
	nCDelay1=100;
	nDisChargeADC1=10;//testmode
	nDischargeCount=20;//sylee20120830

	nHVIADCFlag=1;//SYLEE121018   //waiting 
	nIControl=0;


	//	::ZeroMemory (&nShRes1, sizeof(nShRes1));
	::ZeroMemory (&nData, sizeof(nData));
   	::ZeroMemory (&dR1, sizeof(dR1));
   	::ZeroMemory (&dVB1, sizeof(dVB1));
	::ZeroMemory (&dVC1, sizeof(dVC1));
	::ZeroMemory (&dI1, sizeof(dI1));
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory (&fName, sizeof(fName));
	::ZeroMemory (&fName1, sizeof(fName1));
	::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
	::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907
 
 
 
	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//mode ( 1~65)
	//step ( open, ㅏushort, short, hr, hv  4w) 



	nCC= (int) dRec[m_nMode1][1];
	nVRel= (int)dRec[m_nMode1][2];
	nVSet= (int) dRec[m_nMode1][3];
	nIR= (int)dRec[m_nMode1][4];
	nISet= dRec[m_nMode1][5];
	nDelay10= (int)dRec[m_nMode1][6]*10;
	nFil=(int)dRec[m_nMode1][8];

	nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
	nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
	nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629

	if( nISet<0.1){
		AfxMessageBox(" Error No 12025, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
	}



	//ADC COUNT
	Sub71.nADCount1 = (int)dRec[m_nMode1][7];
	
	
	if(nIR==5){//uA
		InterI=	10.0;
	}
	else{
		InterI=nISet*0.9;
	}
	
	//=========================================================================================
	//interlock    

	if( nCC!=1 && nCC!=2 ){
		AfxMessageBox(" Error No 12024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}
	
	if( nVSet<1 || nVSet>300 ){
		AfxMessageBox(" Error No 12025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}
	
	if( nVRel<1  || nVRel>4 ){ 
		AfxMessageBox(" Error No 12025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
		str.Format("Error"); 
		m_Label1001.SetCaption(str); 
		return 0; 
	}
	
	
	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//설정부 

		
 //sylee130903	Lo1=200;

	Lo1=1000;//sylee130903  200nS-> 1sec  //sylee150708  200->1000
	Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

 Task101:    

        Sub71.AOnReadVB1Run1(nVRel); 

		if(nVSet<=5){
			dVbR2=nVSet*(0.35);//35%
		}else{
			dVbR2=nVSet*(0.1);//10%
		}
		dVbR1=nVSet-dVbR2;
		dVbR2=nVSet+dVbR2;
 
		if( Sub71.V1>300){
			 if(Lo1-->0){					 					
		//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
				 Sub71.Delay_100ns(30000);  //1ms     //sylee150708  1->3
				 goto Task101;
			 }
			 str.Format("Error No 12001,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
			 AfxMessageBox(str);	
   			 str.Format("Error"); 
     		 m_Label1001.SetCaption(str);
       		 return 0;
		}

		if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
			 if(Lo1-->0){
		//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
				 Sub71.Delay_100ns(10000);  //1ms   
				 goto Task101;
			 }					   
			 str.Format("Error No 12013,\n \n\n  Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
			 AfxMessageBox(str);						 
			 str.Format("Error"); 
     		 m_Label1001.SetCaption(str);
   			return 0;
		}
 
 
 
 	 	Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 
 
        Sub71.FilterSet(nFil);//sylee130702 
		if(nIFilter==3){
			AfxMessageBox(" Errro No 12023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
			nIFilter=1;
		}
		Sub71.I_Filter(nIFilter);//sylee130629

	 	Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE20111213     //vb RELAY ON
  	
		Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118
 
 
//--------------------------------------------------------------------------------     
	    ipEnd=1;
		nEnd=0;
	 
		 nRCount=nBDLData3[1][0][1];
		 if( g_nPieceMax1>1 ){//PIECE
		   ipEnd=g_nPieceMax1;
		   ipFlag=1;
		 } 		 	
 
/*
 		if( nVSet>20){
	    	nLoop=1;
		    str.Format("Error No 12313,\n \n\n  MANUAL OPEM  START ERROR    VSET >20V !!    "); 
		    AfxMessageBox(str);
            str.Format("Error"); 
     		m_Label1001.SetCaption(str);
            return 0;
		}else{
			nLoop=0;
		}
*/
 
    nLoop=0;

		
	ipFlag=2;
	ipEnd=1;
	nRCount=k1;
	nRetrySet=3;
	nFailL1CoOpen=0;
	nReCo1=0;
	nStop2=0;

	if(nOpenMode1==2){//pcb open
		nRCount=nBDLData2[1][0][1]+1 ;
	}



    short nPin1[Def_SwitchMaxPin];

    ::ZeroMemory(&nPin1, sizeof(nPin1));


	for( int ip3=1; ip3<=ipEnd; ip3++ ){  //SYLEE161213-2
		if(nRCount<=0){		 
			continue;//sylee20120402
		}
		 for(int  i3=1; i3< nRCount; i3++){ 
			nPinA=nBDLData2[1][i3][1];		 
			nPinB=nBDLData2[1][i3][2];		 

			if((nST1<=(nPinA+1)) &&((nPinA+1)<=nST2)){//SYLEE161215
				nPin1[nPinA]=1;
			}

			if((nST1<=(nPinB+1)) &&((nPinB+1)<=nST2)){//SYLEE161215
			    nPin1[nPinB]=1;	 		 
			}
		 }
	}
//###################################################################################################################################
//###################################################################################################################################
 //first-1

	 for(int  i5=0; i5<= nHalfPin1; i5++){ 
		 if( nPin1[i5]==1){
			 nPinA=i5;
 			 Sub71.AOnPinSet1(2, nPinA, 1); //vb  		 
		 }
	 }

//====================================================================================================================================
 
	 nRCount=nHalfPin1;//sylee161214

	for( i=0; i< nRCount; i++) //SYLEE20111212
	{
	
		DoEvents(); 
	
		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}
	
	
Task1:

 	
		nPinA=i;//sylee161214
	
	
		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}
	
 	
		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 2);   
	
		Sub71.Delay_100ns(nDelay10); 
	
		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}
		else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  					
	
		//sylee161214	Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	
		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 
	
		if( nFil==3 )//HV OPEN ENABLE //SYLEE20120411
		{
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}
			else  if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}
			else  if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}
		else
		{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}
			else  if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}
	
	
		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}
	
	
		int Error;
	
		if(nOpenMode1==2)
		{
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}
			else {
				Error=1;
			}
		}
		else 
		{
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}
			else {
				Error=1;
			}
		}   
	
	
	
		if(Error==1)
		{	
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task1;
			}
	
			nReCo1=0;	
			nPass1[i]=2;//sylee161216-1  fail
		}//}else{//fail  //SYLEE20120117


	
		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us 
		Sub71.AOnPinSet1(2, nPinA, 1); 
	
	
	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


//sylee161214  }//ip
 
//#######################################################################################################
//#######################################################################################################
//#######################################################################################################

  //Task212:
	   

	Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118


//###################################################################################################################################
//###################################################################################################################################
 //second



	 for(   i5=0; i5<= nHalfPin1; i5++){ 
		 if( nPin1[i5]==1){
			 nPinA=i5;
 			 Sub71.AOnPinSet1(2, nPinA, 1); //vb  		 
		 }
	 }

//====================================================================================================================================
//sylee161214 for( int ip=1; ip<=ipEnd; ip++ ){ 
 
//sylee161214 	if(nRCount<=0){
		//ERROR DEBUG   PIECE=ip NO LIST.
//sylee161214 		continue;//sylee20120402
//sylee161214 	}

	nRCount=nHalfPin1;//sylee161214

	for( i=0; i< nRCount; i++) //SYLEE20111212
	{
	
		DoEvents(); 
	
	
		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}


Task102:

		/*		
				if(nOpenMode1==2){//pcb open  //sylee161214 move
				nPinA=nBDLData2[1][i][1];		 
				nPinB=nBDLData2[1][i][2];
				}else{
				nPinA=nBDLPin2[i]-1;		 
				nPinB=nBDLPin2[i+1]-1;
				}
				*/
	
		nPinA=i;//sylee161214
	
	
		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}
	
		if( nPass1[i]!=2){//sylee161216-1  fail
			continue;
		}
	
		//SYLEE161214  Sub71.AOnPinSet1(2, nPinB, 1); //vc ========>
	
		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 2);   
	
	
		Sub71.Delay_100ns(nDelay10); 
	
		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  
	
	
	
		//sylee161214		Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	
		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 
	
		if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}else if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}else if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}else{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}else if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}
	
	
		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}
	
	
		int Error;
	
		if(nOpenMode1==2){
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		}else{
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		} 
	
	
	
		if(Error==1){
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task102;
			}
			nReCo1=0;
	
			if(nFailL1CoOpen<3000){//array size nFailL1[
	
				nFailL1CoOpen++;
	
				nFailL1Open[nFailL1CoOpen][F_VB_PIN]=nPinA+1;//pin 1~                   //son  4:F_VB_PIN
				nFailL1Open[nFailL1CoOpen][F_VC_PIN]=nPinB+1;//pin 1~                   //son  5:F_VC_PIN
				nFailL1Open[nFailL1CoOpen][F_RAW_R]=Sub71.R1 ;//Sub71.R1                //son  6:F_RAW_R
				nFailL1Open[nFailL1CoOpen][F_PRORV_RAW_R]=nProRv1;//nProRv1 판정 rawR   //son 17:F_PRORV_RAW_R
	
				//son220419_6 OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2      //son220412
				//son220419_6 nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2
                int calPart =FileSysInfo01.checkUseCalDataPart(nFailL1Open[i][F_VB_PIN]-1,  nFailL1Open[i][F_VC_PIN]-1);    //son220419_2
				RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart); //son220419_6
				nFailL1Open[nFailL1CoOpen][F_REAL_R]=Sub71.R1 ;                //son220419_6 16으로 잘못된 것을 15:F_REAL_R로 수정
	
	
	
				if(nFailL1CoOpen >=SysInfo05.m_nConEr){
					goto Task211;
				}
	
			}
	
	
		}//}else{//fail  //SYLEE20120117
	
	
		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us 
		Sub71.AOnPinSet1(2, nPinA, 1); 
	
	
	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


	//sylee161214  }//ip
	
	//#######################################################################################################
	//#######################################################################################################
	//#######################################################################################################


	Sub71.OnPinAllReset1(); ///PIN ALL RESET   //sylee170112








	//###################################################################################################################################
	//###################################################################################################################################
	//first-2

	for( i5=nHalfPin1; i5<= Def_SwitchMaxPin; i5++){ 
		if( nPin1[i5]==1){
			nPinA=i5;
			Sub71.AOnPinSet1(2, nPinA, 1); //vb  		 
		}
	}

//====================================================================================================================================
//sylee161214 for( int ip=1; ip<=ipEnd; ip++ ){ 

//sylee161214 	if(nRCount<=0){
//ERROR DEBUG   PIECE=ip NO LIST.
//sylee161214 		continue;//sylee20120402
//sylee161214 	}

	nRCount=Def_SwitchMaxPin;//sylee161214
	
	for( i=nHalfPin1; i< nRCount; i++) //SYLEE20111212
	{
	
		DoEvents(); 
	
		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}


Task201:

		/*		
				if(nOpenMode1==2){//pcb open  //sylee161214 move
				nPinA=nBDLData2[1][i][1];		 
				nPinB=nBDLData2[1][i][2];
				}else{
				nPinA=nBDLPin2[i]-1;		 
				nPinB=nBDLPin2[i+1]-1;
				}
				*/
	
		nPinA=i;//sylee161214
	
	
		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}
	
		//SYLEE161214  Sub71.AOnPinSet1(2, nPinB, 1); //vc ========>
	
		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 2);   
	
	
		Sub71.Delay_100ns(nDelay10); 
	
		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}
		else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  					
	
		//sylee161214	Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	
		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 
	
		if( nFil==3 )//HV OPEN ENABLE //SYLEE20120411
		{
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}
			else  if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}
			else  if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}

		else 
		{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}
			else  if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}


		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}


		int Error;

		if(nOpenMode1==2){
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}
			else {
				Error=1;
			}
		}
		else {
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}
			else {
				Error=1;
			}
		}   
	
	
	
		if(Error==1)
		{
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task201;
			}
	
			nReCo1=0;
	
			nPass1[i]=2;//sylee161216-1  fail
	
			/*							  
										  if(nFailL1CoOpen<3000){//array size nFailL1[
	
										  nFailL1CoOpen++;
	
										  nFailL1Open[nFailL1CoOpen][4]=nPinA+1;//pin 1~
										  nFailL1Open[nFailL1CoOpen][5]=nPinB+1;//pin 1~
	
										  nFailL1Open[nFailL1CoOpen][6]=Sub71.R1 ;//Sub71.R1 
										  nFailL1Open[nFailL1CoOpen][17]=nProRv1;//nProRv1 판정 R   
	
			//	OnConvert_Real_R1(int nMode, double R1_Raw); //sylee161215-2
	
			OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2  //son220412
			nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2 
	
			if(nFailL1CoOpen >=SysInfo05.m_nConEr){
			goto Task212;
			} 						 
			}
			*/
	
	
		}//}else{//fail  //SYLEE20120117
	
	
	
		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us 
		Sub71.AOnPinSet1(2, nPinA, 1); 
	
	
	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


//sylee161214  }//ip
 
//#######################################################################################################
//#######################################################################################################
//#######################################################################################################

  //Task212:
	   

		Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118


	//###################################################################################################################################
	//###################################################################################################################################
	//second



	 for(   i5=nHalfPin1; i5<= Def_SwitchMaxPin; i5++){ 
		 if( nPin1[i5]==1){
			 nPinA=i5;
 			 Sub71.AOnPinSet1(2, nPinA, 1); //vb  		 
		 }
	 }

//====================================================================================================================================
//sylee161214 for( int ip=1; ip<=ipEnd; ip++ ){ 
 
//sylee161214 	if(nRCount<=0){
		//ERROR DEBUG   PIECE=ip NO LIST.
//sylee161214 		continue;//sylee20120402
//sylee161214 	}

	nRCount=Def_SwitchMaxPin;//sylee161214
	
	for( i=nHalfPin1; i< nRCount; i++) //SYLEE20111212
	{	
		DoEvents(); 
	
	
		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}
	

Task202:
	
		/*		
				if(nOpenMode1==2){//pcb open  //sylee161214 move
				nPinA=nBDLData2[1][i][1];		 
				nPinB=nBDLData2[1][i][2];
				}else{
				nPinA=nBDLPin2[i]-1;		 
				nPinB=nBDLPin2[i+1]-1;
				}
				*/
	
		nPinA=i;//sylee161214
	
	
		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}
	
		if( nPass1[i]!=2){//sylee161216-1  fail
			continue;
		}
	
		//SYLEE161214  Sub71.AOnPinSet1(2, nPinB, 1); //vc ========>
	
		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 2);   
	
	
		Sub71.Delay_100ns(nDelay10); 
	
		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  
	
	
	
		//sylee161214		Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	
		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 
	
		if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}else if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}else if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}else{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}else if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}
	
	
		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}
	
	
		int Error;
	
		if(nOpenMode1==2){
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		}else{
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		} 
	
	
	
		if(Error==1){
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task202;
			}
			nReCo1=0;
	
			if(nFailL1CoOpen<3000){//array size nFailL1[
	
				nFailL1CoOpen++;
	
				nFailL1Open[nFailL1CoOpen][F_VB_PIN]=nPinA+1;//pin 1~                     //son  4:F_VB_PIN 
				nFailL1Open[nFailL1CoOpen][F_VC_PIN]=nPinB+1;//pin 1~                     //son  5:F_VC_PIN
                                                                                                                 
				nFailL1Open[nFailL1CoOpen][F_RAW_R]=Sub71.R1 ;//Sub71.R1                  //son  6:F_RAW_R
				nFailL1Open[nFailL1CoOpen][F_PRORV_RAW_R]=nProRv1;//nProRv1 판정 R        //son 17: F_PRORV_RAW_R
	
				//son220418_6 OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2  //son220412
				//son220418_6 nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2
                int calPart =FileSysInfo01.checkUseCalDataPart(nFailL1Open[i][F_VB_PIN]-1,  nFailL1Open[i][F_VC_PIN]-1);    //son220419_2
				RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart);           //son220419_6
				nFailL1Open[nFailL1CoOpen][F_REAL_R]=Sub71.R1 ;                 //son220419_6 16으로 잘못된 것을 15:F_REAL_R로 수정
	
	
	
				if(nFailL1CoOpen >=SysInfo05.m_nConEr){
					goto Task211;
				}
	
			}
	
	
		}//}else{//fail  //SYLEE20120117
	
	
		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC,
		Sub71.Delay_100ns(100);  //10us 
		Sub71.AOnPinSet1(2, nPinA, 1); 
		
	
	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


	//sylee161214  }//ip
	 
	//#######################################################################################################
	//#######################################################################################################
	//#######################################################################################################










//#######################################################################################################
//#######################################################################################################
//#######################################################################################################

Task211:

//==============================================================================================================================
//################################################################################################################################

	Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
	Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704
	Sub71.AOnReadV1Enable1(5); //sylee130711
	
	////////////////////////
	//[PRINTING 
	
	
	
	FILE *fp;  
	int nFile1;
	
	nFile1=0;
	
	
	::ZeroMemory(&fName , sizeof(fName ));
	strcat( fName, "D:\\Manual_OPEN1_JigTest.TXT" ); 
	
	fp = fopen(fName,"wt"); 
	if(fp == NULL){
		nFile1=2;
		//	return 0;
	}
	
	
	
	
	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	 
   	if( nFailL1CoOpen <=0){
		str.Format("  ***>    Open  NO Error *******  ");			 
	}else{
		str.Format("  ===>  Result : [Open  Error] => %d  ", nFailL1CoOpen );			 		
	}
  	m_list1.AddString(str); 
	nlist1++;
    if(nFile1==0) fprintf(fp, "%s  \n",str );


	str.Format(" ");			 		
   	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0){fprintf(fp," %s \n",str);}

	if( nFailL1CoOpen==SysInfo05.m_nConEr){
 		str.Format("  ***>    Open Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nConEr );			 		
   		m_list1.AddString(str); 
		nlist1++;
		if(nFile1==0){fprintf(fp," %s \n",str);}
	}

	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0){fprintf(fp," %s \n",str);}
 
	for( i=1 ; i<=nFailL1CoOpen ; i++)
	{
		//sylee161214	str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f  ",i, nFailL1Open[i][4],nFailL1Open[i][5] );
	
	
		if( nRdisplay1!=1){
			str.Format("   --> Open List No=%d,      pin  %5.0f   ",i, nFailL1Open[i][4]  );//sylee161214
		}
		else
		{



			if(m_nMode1==51){//100V

			//sylee221014	if(nFailL1Open[i][16]>=200){
				if(nFailL1Open[i][F_REAL_R]>=200){	//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f  ,  R> 200 ohm ",i, nFailL1Open[i][4]  );//sylee161214
				}
				else{
					//sylee181217-3	  if( SysSet13.m_nSet5==1 ){//sylee161221
					//sylee181217-3	       str.Format("   --> Open List No=%d,      pin  %5.0f  ,   R= %7.0f ohm  ",
					//						i, nFailL1Open[i][4],nFailL1Open[i][16]  );//sylee161214
					//sylee181217-3	  }else{
					str.Format("   --> Open List No=%d,      pin  %5.0f    ",i, nFailL1Open[i][4]   );//sylee161221
					//sylee181217-3	  }
				}

			}
			else
			{

			//sylee221014	if(nFailL1Open[i][16]>1000){
				if(nFailL1Open[i][F_REAL_R]>1000){//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f  ,  R> 1k ohm ",i, nFailL1Open[i][4]  );//sylee161214
				}
				else
				{
					//sylee181217-3	 if( SysSet13.m_nSet5==1 ){//sylee161221
					//sylee181217-3	    str.Format("   --> Open List No=%d,      pin  %5.0f  ,   R= %7.0f ohm  ",
					//						i, nFailL1Open[i][4],nFailL1Open[i][16]  );//sylee161214
					//sylee181217-3	 }else{
					str.Format("   --> Open List No=%d,      pin  %5.0f    ",i, nFailL1Open[i][4]  );//sylee161221
					//sylee181217-3	 }
				}
			}



		}
	
		m_list1.AddString(str); 
		nlist1++;
		if(nFile1==0){fprintf(fp," %s \n",str);}
	
	}

	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0){fprintf(fp," %s \n",str);}
	
	
	
	if( nStop2==2){
		str.Format("  ***> Manual Open Check    STOP.  *******  ");	 
	}
	else{
		str.Format("  ***> Manual Open Check    FINISH.  *******  ");			 
	}
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0){fprintf(fp," %s \n",str);}
	
	
	
	
	if( nStop2==2){
		str.Format("STOP"); 
	}
	else{
		str.Format("FINISH"); 	 
	}
	

    m_Label1001.SetCaption(str);
//	if(nFile1==0){fprintf(fp," %s \n",str);}



	if(nFile1==0)    
		fclose(fp);

		
	 //	delete [] nBDLPin1; 
     //   delete [] nBDLPin2; 


 

//::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);
    
	
	return 1;

}//end of CChildView3::OnManualShort1(){//sylee131023

 


int CModeMan12::OnManualOpen1_Run2_2() //sylee161215-2
{

	CString   str,str1;
	CString   strfName,strfName1;

	char  fName[200], fName1[200]; 
    int i , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nCDelay1;
	int nFil,nDisChargeADC1,nHVIADCFlag, nDischargeCount ;
	int nShRes1T,logno1;
 

	double nISet,dVbR1,dVbR2,InterI;
	

	int nFailL1CoOpen,nPinA,nPinB,nReCo1,nRetrySet;

// 	short nShRes1[33000];
	int nSQ1Fail[200];//, nSQPinCheck[34000];
	int nSh1Ret1[100],dPinVC[200],dPinVB[200];// pin array//sylee1209


	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200]; 
 
 
    int nIControl, nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR ; 
    int m_nMode1,nMod1;
	double nProRv1;

	int nBDLPin1[DEF_MAX_PIN1];
	int nBDLPin2[DEF_MAX_PIN1];
     char   szText[50];
 
    int nlist1;

	 nlist1=0;   

	m_list1.ResetContent();

	str.Format(" < RUN  TYPE  : 2 >  " );	 
  	m_list1.AddString(str); 
	nlist1++;

	str.Format("RUN"); 
	m_Label1001.SetCaption(str);



	/////////////////////////////////////
    //net list load

	// nBDLHead1[1][3][1]  &nBDLData1[1][i][j]); 

    
	 
     int k1,k2,i1,nMaxPin1;

	 k2=nBDLHead1[1][3][1];

	// ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	// ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));

	 k1=FileSysInfo01.LoadSaveSet12(2);//load 
	 if( k1!=1){
		str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		 return 0;
	 }


	if(SysSet12.m_nPinTotMax== PIN_TOT_4K){//4k  //SYLEE121003
		nMaxPin1=4096;		
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_8K){//8k
		nMaxPin1=4096*2;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_16K){//16k
		nMaxPin1=4096*4;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS){//32k //son230216_1
		nMaxPin1=4096*8;
	}
	else{ // 20K, 24K, Org 32K
		nMaxPin1=4096*8;
	}


	//	int *nBDLPin1 = new int[nMaxPin1+1];
	//	int *nBDLPin2 = new int[nMaxPin1+1];

	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;

	str.Format("  ***> Manual Open :  START  *******  ");			 
	m_list1.AddString(str); 
	nlist1++;


	DoEvents();  



	// int *nBDLPin1 = new int[nMaxPin1+1];
	//	 int *nBDLPin2 = new int[nMaxPin1+1];   


	for(int ik1=0; ik1<=nMaxPin1; ik1++){
		nBDLPin1[ik1]=0;
		nBDLPin2[ik1]=0;
	}

	// ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	//  ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));



	if(nOpenMode1==2){//pcb open


	}else{  //jig up
		for(  i1=1 ; i1<= k2 ; i1++){		 
			k1=nBDLData1[1][i1][1];
			nBDLPin1[k1+1]=1;	  
		}  

		k1=0;
		for(  i1=1 ; i1<=nMaxPin1 ; i1++){		 
			if( nBDLPin1[i1]==1){
				nBDLPin1[0]=nBDLPin1[0]+1;
				k1++;
				nBDLPin2[k1]=i1;
			}		    
		}
		nBDLPin2[0]=k1;
	}

	//////////////////////////////////////


	nMod1=1;
	//m_nMode1=6;
	m_nMode1=7;//sylee161216-1
	nProRv1=150;//raw   100 R
	//====================

	m_edit103.GetWindowText( szText,10); 
	nManuaOpenR1 = atoi( szText );
	if( nManuaOpenR1<1){
		nManuaOpenR1=1;
	}
	/*
	   if(nManuaOpenR1>1000 ){
	   nManuaOpenR1=1000;
	   }
	   */

	/*
	   if(nManuaOpenR1>200 ){
	   nManuaOpenR1=200;
	   }
	   */

 	if(nManuaOpenR1>10000 ){//SYLEE170110
		nManuaOpenR1=10000;
	}

	str.Format("%d",nManuaOpenR1);
	m_edit103.SetWindowText(str); 


 
      m_nMode1=51;//100V
 
	if( nManuaOpenR1>1000){
        m_nMode1=m_nMode1+2;
	}else if( nManuaOpenR1>200){
		m_nMode1=m_nMode1+1;
	}



	m_edit101.GetWindowText( szText,10); 
	nST1 = atoi( szText );
	if( nST1<1){
		nST1=1;
	}
	if(nST1>Def_SwitchMaxPin ){
		nST1=Def_SwitchMaxPin;
	}
	str.Format("%d",nST1);
	m_edit101.SetWindowText(str); 
 

	m_edit102.GetWindowText( szText,10); 
	nST2 = atoi( szText );
	if( nST2<1){
		nST2=1;
	}
	if(nST2>Def_SwitchMaxPin ){
		nST2=Def_SwitchMaxPin;
	}
	str.Format("%d",nST2);
	m_edit102.SetWindowText(str); 
 /////////////////////////////////////////////////



	Sub71.R1=nManuaOpenR1;  

	int nRet;
	nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412
	if( nRet!=1){
		str.Format("ERRor NO 3005,   \n JIg open test:  Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
		AfxMessageBox( str);
		return 0; 
	}

	RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER );   // m_nMode1 1~76  //son220412_3 
	nProRv1=Sub71.R1;
	//===============================================================================================


	logno1=0;    nShRes1T=0;	nStop=0;
	ipFlag=0;	n4Wire=0;	ipEnd=1; 
	nCDelay1=100;
	nDisChargeADC1=10;//testmode
	nDischargeCount=20;//sylee20120830

	nHVIADCFlag=1;//SYLEE121018   //waiting 
	nIControl=0;


	//	::ZeroMemory (&nShRes1, sizeof(nShRes1));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dR1, sizeof(dR1));
	::ZeroMemory (&dVB1, sizeof(dVB1));
	::ZeroMemory (&dVC1, sizeof(dVC1));
	::ZeroMemory (&dI1, sizeof(dI1));
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory (&fName, sizeof(fName));
	::ZeroMemory (&fName1, sizeof(fName1));
	::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
	::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907



	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//mode ( 1~65)
	//step ( open, ㅏushort, short, hr, hv  4w) 

	

	nCC= (int) dRec[m_nMode1][1];
	nVRel= (int)dRec[m_nMode1][2];
	nVSet= (int) dRec[m_nMode1][3];
	nIR= (int)dRec[m_nMode1][4];
	nISet= dRec[m_nMode1][5];
	nDelay10= (int)dRec[m_nMode1][6]*10;
	nFil=(int)dRec[m_nMode1][8];

	nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
	nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
	nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629

	if( nISet<0.1){
		AfxMessageBox(" Error No 12025, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
	}



	//ADC COUNT
	Sub71.nADCount1 = (int)dRec[m_nMode1][7];


	if(nIR==5){//uA
		InterI=	10.0;
	}else{
		InterI=nISet*0.9;
	}

	//=========================================================================================
	//interlock    

	if( nCC!=1 && nCC!=2 ){
		AfxMessageBox(" Error No 12024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}

	if( nVSet<1 || nVSet>300 ){
		AfxMessageBox(" Error No 12025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}

	if( nVRel<1  || nVRel>4 ){ 
		AfxMessageBox(" Error No 12025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
		str.Format("Error"); 
		m_Label1001.SetCaption(str); 
		return 0; 
	}

  
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//설정부 
	
		
 //sylee130903	Lo1=200;

    Lo1=1000;//sylee130903  200nS-> 1sec  //sylee150708  200->1000
	Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

Task101:    

	Sub71.AOnReadVB1Run1(nVRel); 

	if(nVSet<=5){
		dVbR2=nVSet*(35*0.01);//10%
	}else{
		dVbR2=nVSet*(0.1);//10%
	}
	dVbR1=nVSet-dVbR2;
	dVbR2=nVSet+dVbR2;

	if( Sub71.V1>300)
	{
		if(Lo1-->0){					 					
			//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
			Sub71.Delay_100ns(30000);  //1ms     //sylee150708  1->3
			goto Task101;
		}
		str.Format("Error No 12001,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}

	if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
		if(Lo1-->0){
			//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
			Sub71.Delay_100ns(10000);  //1ms   
			goto Task101;
		}					   
		str.Format("Error No 12013,\n \n\n  Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);						 
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}



	Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 

	Sub71.FilterSet(nFil);//sylee130702 
	if(nIFilter==3){
		AfxMessageBox(" Errro No 12023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
		nIFilter=1;
	}
	Sub71.I_Filter(nIFilter);//sylee130629

	Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE20111213     //vb RELAY ON

	Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118
 
 
	//--------------------------------------------------------------------------------     
	ipEnd=1;
	nEnd=0;

	 
	nRCount=nBDLData3[1][0][1];
	if( g_nPieceMax1>1 ){//PIECE
		ipEnd=g_nPieceMax1;
		ipFlag=1;
	} 	 
	 	
 /*
 		if( nVSet>20){
	    	nLoop=1;
		    str.Format("Error No 12313,\n \n\n  MANUAL OPEM  START ERROR    VSET >20V !!    "); 
		    AfxMessageBox(str);
            str.Format("Error"); 
     		m_Label1001.SetCaption(str);
            return 0;
		}else{
			nLoop=0;
		}

*/
	nLoop=0;



	ipFlag=2;
	ipEnd=1;

	nRCount=k1;
	nRetrySet=3;


	nFailL1CoOpen=0;
	nReCo1=0;
	nStop2=0;

	if(nOpenMode1==2){//pcb open
		nRCount=nBDLData2[1][0][1]+1 ;
	}



	short nPin1[Def_SwitchMaxPin];

	::ZeroMemory(&nPin1, sizeof(nPin1));


	for( int ip3=1; ip3<=ipEnd; ip3++ ){  //SYLEE161213-2
		if(nRCount<=0){		 
			continue;//sylee20120402
		}
		for(int  i3=1; i3< nRCount; i3++){ 
			nPinA=nBDLData2[1][i3][1];		 
			nPinB=nBDLData2[1][i3][2];		 
			if((nST1<=(nPinA+1)) &&((nPinA+1)<=nST2)){//SYLEE161215
				nPin1[nPinA]=1;
			}

			if((nST1<=(nPinB+1)) &&((nPinB+1)<=nST2)){//SYLEE161215
				nPin1[nPinB]=1;	 		 
			}
		}
	}



	for(int  i5=0; i5<= Def_SwitchMaxPin; i5++){ 
		if( nPin1[i5]==1){
			nPinA=i5;
			Sub71.AOnPinSet1(2, nPinA, 2); //vC  	//<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1	 
		}
	}




	//====================================================================================================================================
	//sylee161214 for( int ip=1; ip<=ipEnd; ip++ ){ 

	//sylee161214 	if(nRCount<=0){
	//ERROR DEBUG   PIECE=ip NO LIST.
	//sylee161214 		continue;//sylee20120402
	//sylee161214 	}

	nRCount=Def_SwitchMaxPin;//sylee161214

	for( i=0; i< nRCount; i++) //SYLEE20111212
	{
		DoEvents(); 


		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}


Task1:

		/*		
				if(nOpenMode1==2){//pcb open  //sylee161214 move
				nPinA=nBDLData2[1][i][1];		 
				nPinB=nBDLData2[1][i][2];
				}else{
				nPinA=nBDLPin2[i]-1;		 
				nPinB=nBDLPin2[i+1]-1;
				}
				*/

		nPinA=i;//sylee161214


		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}



		//SYLEE161214  Sub71.AOnPinSet1(2, nPinB, 1); //vc ========>




		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC, //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 1);   //VB //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


		Sub71.Delay_100ns(nDelay10); 

		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  



		//sylee161214		Sub71.OnPinAllReset1(); ///PIN ALL RESET 

		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 

		if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}else if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}else if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}else{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}else if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}


		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}


		int Error;

		if(nOpenMode1==2){
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		}else{
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		} 



		if(Error==1){
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task1;
			}
			nReCo1=0;

			if(nFailL1CoOpen<3000){//array size nFailL1[

				nFailL1CoOpen++;

				nFailL1Open[nFailL1CoOpen][F_VB_PIN]=nPinA+1;//pin 1~                   //son  4:F_VB_PIN
				nFailL1Open[nFailL1CoOpen][F_VC_PIN]=nPinB+1;//pin 1~                   //son  5:F_VC_PIN

				nFailL1Open[nFailL1CoOpen][F_RAW_R]=Sub71.R1 ;//Sub71.R1                //son  6:F_RAW_R
				nFailL1Open[nFailL1CoOpen][F_PRORV_RAW_R]=nProRv1;//nProRv1 판정 rawR   //son 17: F_PRORV_RAW_R

				//son220419_6 OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2  
				//son220519_6 nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2
				
                int calPart =FileSysInfo01.checkUseCalDataPart(nFailL1Open[i][F_VB_PIN]-1,  nFailL1Open[i][F_VC_PIN]-1);    //son220419_2
				RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart);           //son220419_6
				nFailL1Open[nFailL1CoOpen][F_REAL_R]=Sub71.R1 ;                 //son220419_6 16으로 잘못된 것을 15:F_REAL_R로 수정


				if(nFailL1CoOpen >=SysInfo05.m_nConEr){
					goto Task211;
				}

			}


		}//}else{//fail  //SYLEE20120117



		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC, //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		Sub71.Delay_100ns(100);  //10us
		Sub71.AOnPinSet1(2, nPinA, 2);  //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


//sylee161214  }//ip
 


Task211:

	//==============================================================================================================================
	//################################################################################################################################
	
	Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
	Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704
	Sub71.AOnReadV1Enable1(5); //sylee130711
	
	////////////////////////
	//[PRINTING 
    
	  
	FILE *fp;  
	int nFile1;

	nFile1=0;

 
	::ZeroMemory(&fName , sizeof(fName ));
	 strcat( fName, "D:\\Manual_OPEN1_JigTest.TXT" ); 

	fp = fopen(fName,"wt"); 
	if(fp == NULL){
		nFile1=2;
	//	return 0;
	}

  
	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	 
   	if( nFailL1CoOpen <=0){
		str.Format("  ***>    Open  NO Error *******  ");			 
	}
	else{
		str.Format("  ===>  Result : [Open  Error] => %d  ", nFailL1CoOpen );			 		
	}
  	m_list1.AddString(str); 
	nlist1++;
  
	//if(nFile1==0){fprintf(fp, str);fprintf(fp, "\n");}
	if(nFile1==0){fprintf(fp," %s \n",str);}


	str.Format(" ");			 		
   	m_list1.AddString(str); 
	nlist1++;
	 
	if(nFile1==0) fprintf(fp, "%s  \n",str );

	if( nFailL1CoOpen==SysInfo05.m_nConEr){
 		str.Format("  ***>    Open Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nConEr );			 		
   		m_list1.AddString(str); 
		nlist1++;
	}

	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0) 
		fprintf(fp, "%s  \n",str );
 
	for( i=1 ; i<=nFailL1CoOpen ; i++)
	{
		//sylee161214	str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f  ",i, nFailL1Open[i][4],nFailL1Open[i][5] );			 
		//	str.Format("   --> Open List No=%d,      pin  %5.0f   ",i, nFailL1Open[i][4]  );//sylee161214			 


		if( nRdisplay1!=1){
			str.Format("   --> Open List No=%d,      pin  %5.0f   ",i, nFailL1Open[i][4]  );//sylee161214
		}
		else
		{



			if(m_nMode1==51){//100V

			//sylee221014	if(nFailL1Open[i][16]>=200){
				if(nFailL1Open[i][F_REAL_R]>=200){//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f  ,  R> 200 ohm ",i, nFailL1Open[i][4]  );//sylee161214
				}
				else{
					//sylee181217-3	 if( SysSet13.m_nSet5==1 ){//sylee161221
					//sylee181217-3	       str.Format("   --> Open List No=%d,      pin  %5.0f  ,   R= %7.0f ohm  ",
					//							i, nFailL1Open[i][4],nFailL1Open[i][16]  );//sylee161214
					//sylee181217-3	 }else{
					str.Format("   --> Open List No=%d,      pin  %5.0f    ",i, nFailL1Open[i][4]  );//sylee161214
					//sylee181217-3	}
				}

			}
			else{

			//sylee221014	if(nFailL1Open[i][16]>1000){
				if(nFailL1Open[i][F_REAL_R]>1000){ //sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f  ,  R> 1k ohm ",i, nFailL1Open[i][4]  );//sylee161214
				}
				else{
					//sylee181217-3		if( SysSet13.m_nSet5==1 ){//sylee161221
					//sylee181217-3	       str.Format("   --> Open List No=%d,      pin  %5.0f  ,   R= %7.0f ohm  ",
					//							i, nFailL1Open[i][4],nFailL1Open[i][16]  );//sylee161214
					//sylee181217-3		}else{
					str.Format("   --> Open List No=%d,      pin  %5.0f    ",i, nFailL1Open[i][4]  );//sylee161214
					//sylee181217-3		}
				}
			}


		}

		m_list1.AddString(str); 
		nlist1++;
		if(nFile1==0) 
			fprintf(fp, "%s  \n",str );

	}


	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0) fprintf(fp, "%s  \n",str );



	if( nStop2==2){
		str.Format("  ***> Manual Open Check    STOP.  *******  ");	 
	}else{
		str.Format("  ***> Manual Open Check    FINISH.  *******  ");			 
	}
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0) fprintf(fp, "%s  \n",str );


	if( nStop2==2){
		str.Format("STOP"); 
	}else{
		str.Format("FINISH"); 	 
	}


	m_Label1001.SetCaption(str);
	//		  if(nFile1==0) fprintf(fp, "%s  \n",str );



	if(nFile1==0)    fclose(fp); 


	//	delete [] nBDLPin1; 
	//   delete [] nBDLPin2; 




	//::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);
    
	
	return 1;

}//end of CChildView3::OnManualShort1(){//sylee131023

 ///=======================================


 


int CModeMan12::OnManualOpen1_Run3_2() //sylee170112-1
{

	CString   str,str1;
	CString   strfName,strfName1;

	char  fName[200], fName1[200]; 
    int i , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nCDelay1;
	int nFil,nDisChargeADC1,nHVIADCFlag, nDischargeCount ;
	int nShRes1T,logno1;
  	double nISet,dVbR1,dVbR2,InterI;
 	int nFailL1CoOpen,nPinA,nPinB,nReCo1,nRetrySet;
 
	int nSQ1Fail[200];//, nSQPinCheck[34000];
	int nSh1Ret1[100],dPinVC[200],dPinVB[200];// pin array//sylee1209
	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200];   
    int nIControl, nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR ,m_nMode1,nMod1;
	double nProRv1;
	int nBDLPin1[DEF_MAX_PIN1], nBDLPin2[DEF_MAX_PIN1];
    char  szText[50]; 
    int nlist1, nHalfPin1;//sylee171112-1


	 nlist1=0;   

	m_list1.ResetContent();

	str.Format(" < RUN  TYPE  : 2 >  " );	 
  	m_list1.AddString(str); 
	nlist1++;

	str.Format("RUN"); 
	m_Label1001.SetCaption(str);

	/////////////////////////////////////
    //net list load
	// nBDLHead1[1][3][1]  &nBDLData1[1][i][j]);  	 
     int k1,k2,i1,nMaxPin1;
	 k2=nBDLHead1[1][3][1];

	// ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	// ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));
	 k1=FileSysInfo01.LoadSaveSet12(2);//load 
	 if( k1!=1){
		str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		return 0;
	 }


	if(SysSet12.m_nPinTotMax== PIN_TOT_4K){//4k  //SYLEE121003
		nMaxPin1=4096;		
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_8K){//8k
		nMaxPin1=4096*2;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_16K){//16k
		nMaxPin1=4096*4;
	}
	else if(SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS){//32k //son230216_1
		nMaxPin1=4096*8;
	}
	else{ // 20K, 24K, Org 32K
		nMaxPin1=4096*8;
	}



	 nHalfPin1=nMaxPin1/2;//sylee171112-1
//	int *nBDLPin1 = new int[nMaxPin1+1];
//	int *nBDLPin2 = new int[nMaxPin1+1];
	 	str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		str.Format("  ***> Manual Open :  START  *******  ");			 
		m_list1.AddString(str); 
		nlist1++;
		DoEvents(); 
    // int *nBDLPin1 = new int[nMaxPin1+1];
 //	 int *nBDLPin2 = new int[nMaxPin1+1];   
	 
	 for(int ik1=0; ik1<=nMaxPin1; ik1++){
		 nBDLPin1[ik1]=0;
		 nBDLPin2[ik1]=0;
	 }

	 // ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
	 //  ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));

	 if(nOpenMode1==2){//pcb open

	 }
	 else{  //jig up
		 for(  i1=1 ; i1<= k2 ; i1++){		 
			 k1=nBDLData1[1][i1][1];
			 nBDLPin1[k1+1]=1;	  
		 }  

		 k1=0;
		 for(  i1=1 ; i1<=nMaxPin1 ; i1++){		 
			 if( nBDLPin1[i1]==1){
				 nBDLPin1[0]=nBDLPin1[0]+1;
				 k1++;
				 nBDLPin2[k1]=i1;
			 }		    
		 }
		 nBDLPin2[0]=k1;
	 }

	 //////////////////////////////////////


	 nMod1=1;
	 //m_nMode1=6;
	 m_nMode1=7;//sylee161216-1
	 nProRv1=150;//raw   100 R
	 //====================

	 m_edit103.GetWindowText( szText,10); 
	 nManuaOpenR1 = atoi( szText );
	 if( nManuaOpenR1<1){
		 nManuaOpenR1=1;
	 }
	 /*
		if(nManuaOpenR1>1000 ){
		nManuaOpenR1=1000;
		}
		*/

	 if(nManuaOpenR1>200 ){
		 nManuaOpenR1=200;
	 }

	 /*
		if(nManuaOpenR1>10000 ){//SYLEE170110
		nManuaOpenR1=10000;
		}
		*/
	 str.Format("%d",nManuaOpenR1);
	 m_edit103.SetWindowText(str); 

	 m_nMode1=51;//100V

	 if( nManuaOpenR1>1000){
		 m_nMode1=m_nMode1+2;
	 }else if( nManuaOpenR1>200){
		 m_nMode1=m_nMode1+1;
	 }



	 m_edit101.GetWindowText( szText,10); 
	 nST1 = atoi( szText );
	 if( nST1<1){
		 nST1=1;
	 }
	 if(nST1>Def_SwitchMaxPin ){
		 nST1=Def_SwitchMaxPin;
	 }
	 str.Format("%d",nST1);
	 m_edit101.SetWindowText(str); 
 

	m_edit102.GetWindowText( szText,10); 
	nST2 = atoi( szText );
	if( nST2<1){
		nST2=1;
	}
	if(nST2>Def_SwitchMaxPin ){
		nST2=Def_SwitchMaxPin;
	}
	str.Format("%d",nST2);
	m_edit102.SetWindowText(str); 
 /////////////////////////////////////////////////



	Sub71.R1=nManuaOpenR1;  

	 int nRet;
	 nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412
	 if( nRet!=1){
		str.Format("ERRor NO 3005,   \n JIg open test:  Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
		AfxMessageBox( str);
		return 0; 
	 }

	  RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER );   // m_nMode1 1~76   //son220412_3 
	  nProRv1=Sub71.R1;
	  //===============================================================================================

 
    logno1=0;    nShRes1T=0;	nStop=0;
	ipFlag=0;	n4Wire=0;	ipEnd=1; 
	nCDelay1=100;
  	nDisChargeADC1=10;//testmode
 	nDischargeCount=20;//sylee20120830
   
	nHVIADCFlag=1;//SYLEE121018   //waiting 
	nIControl=0;
 

//	::ZeroMemory (&nShRes1, sizeof(nShRes1));
	::ZeroMemory (&nData, sizeof(nData));
   	::ZeroMemory (&dR1, sizeof(dR1));
   	::ZeroMemory (&dVB1, sizeof(dVB1));
	::ZeroMemory (&dVC1, sizeof(dVC1));
	::ZeroMemory (&dI1, sizeof(dI1));
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory (&fName, sizeof(fName));
	::ZeroMemory (&fName1, sizeof(fName1));
	::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
	::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907
 
 
 
	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//mode ( 1~65)
	//step ( open, ㅏushort, short, hr, hv  4w) 

	

	nCC= (int) dRec[m_nMode1][1];
	nVRel= (int)dRec[m_nMode1][2];
	nVSet= (int) dRec[m_nMode1][3];
	nIR= (int)dRec[m_nMode1][4];
	nISet= dRec[m_nMode1][5];
	nDelay10= (int)dRec[m_nMode1][6]*10;
	nFil=(int)dRec[m_nMode1][8];
	
	nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
	nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
	nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629
	
	if( nISet<0.1){
	    AfxMessageBox(" Error No 12025, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
	}
	
	 
	
	//ADC COUNT
	Sub71.nADCount1 = (int)dRec[m_nMode1][7];
	
	
	if(nIR==5){//uA
	    InterI=	10.0;
	}else{
	    InterI=nISet*0.9;
	}
	
	//=========================================================================================
	//interlock    
	
	if( nCC!=1 && nCC!=2 ){
	    AfxMessageBox(" Error No 12024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
	    str.Format("Error"); 
	    m_Label1001.SetCaption(str);
	    return 0;
	}
	
	if( nVSet<1 || nVSet>300 ){
	    AfxMessageBox(" Error No 12025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
	    str.Format("Error"); 
	    m_Label1001.SetCaption(str);
	    return 0;
	}
	
	if( nVRel<1  || nVRel>4 ){ 
	    AfxMessageBox(" Error No 12025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
	    str.Format("Error"); 
	    m_Label1001.SetCaption(str); 
	    return 0; 
	}

  
	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//설정부 

		
	//sylee130903	Lo1=200;

	Lo1=1000;//sylee130903  200nS-> 1sec  //sylee150708  200->1000
	Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

 Task101:    

	Sub71.AOnReadVB1Run1(nVRel); 

	if(nVSet<=5){
		dVbR2=nVSet*(0.35);//10%
	}else{
		dVbR2=nVSet*(0.1);//10%
	}
	dVbR1=nVSet-dVbR2;
	dVbR2=nVSet+dVbR2;

	if( Sub71.V1>300){
		if(Lo1-->0){					 					
			//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
			Sub71.Delay_100ns(30000);  //1ms     //sylee150708  1->3
			goto Task101;
		}
		str.Format("Error No 12001,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}

	if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
		if(Lo1-->0){
			//sylee121019	Sub71.Delay_100ns(100000);  //10ms   
			Sub71.Delay_100ns(10000);  //1ms   
			goto Task101;
		}					   
		str.Format("Error No 12013,\n \n\n  Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);						 
		str.Format("Error"); 
		m_Label1001.SetCaption(str);
		return 0;
	}



	Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 

	Sub71.FilterSet(nFil);//sylee130702 
	if(nIFilter==3){
		AfxMessageBox(" Errro No 12023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
		nIFilter=1;
	}
	Sub71.I_Filter(nIFilter);//sylee130629

	Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE20111213     //vb RELAY ON

	Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118


	//--------------------------------------------------------------------------------     
	ipEnd=1;
	nEnd=0;


	nRCount=nBDLData3[1][0][1];
	if( g_nPieceMax1>1 ){//PIECE
		ipEnd=g_nPieceMax1;
		ipFlag=1;
	} 	 

	/*
	   if( nVSet>20){
	   nLoop=1;
	   str.Format("Error No 12313,\n \n\n  MANUAL OPEM  START ERROR    VSET >20V !!    "); 
	   AfxMessageBox(str);
	   str.Format("Error"); 
	   m_Label1001.SetCaption(str);
	   return 0;
	   }else{
	   nLoop=0;
	   }

*/
	nLoop=0;



	ipFlag=2;
	ipEnd=1;

	nRCount=k1;
	nRetrySet=3;


	nFailL1CoOpen=0;
	nReCo1=0;
	nStop2=0;

	if(nOpenMode1==2){//pcb open
		nRCount=nBDLData2[1][0][1]+1 ;
	}



	short nPin1[Def_SwitchMaxPin];

	::ZeroMemory(&nPin1, sizeof(nPin1));


	for( int ip3=1; ip3<=ipEnd; ip3++ ){  //SYLEE161213-2
		if(nRCount<=0){		 
			continue;//sylee20120402
		}
		for(int  i3=1; i3< nRCount; i3++){ 
			nPinA=nBDLData2[1][i3][1];		 
			nPinB=nBDLData2[1][i3][2];		 
			if((nST1<=(nPinA+1)) &&((nPinA+1)<=nST2)){//SYLEE161215
				nPin1[nPinA]=1;
			}

			if((nST1<=(nPinB+1)) &&((nPinB+1)<=nST2)){//SYLEE161215
				nPin1[nPinB]=1;	 		 
			}
		}
	}


	 for(int  i5=0; i5<= nHalfPin1; i5++){ //Def_SwitchMaxPin->nHalfPin1  //sylee170112
		 if( nPin1[i5]==1){
			 nPinA=i5;
 			 Sub71.AOnPinSet1(2, nPinA, 2); //vC  	//<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1	 
		 }
	 }

 //====================================================================================================================================
//sylee161214 for( int ip=1; ip<=ipEnd; ip++ ){ 
 
//sylee161214 	if(nRCount<=0){
		//ERROR DEBUG   PIECE=ip NO LIST.
//sylee161214 		continue;//sylee20120402
//sylee161214 	}

	nRCount=nHalfPin1;//sylee161214   //Def_SwitchMaxPin->nHalfPin1//sylee170112

	for( i=0; i< nRCount; i++) //SYLEE20111212
	{

		DoEvents(); 


		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}


Task1:

		/*		
				if(nOpenMode1==2){//pcb open  //sylee161214 move
				nPinA=nBDLData2[1][i][1];		 
				nPinB=nBDLData2[1][i][2];
				}else{
				nPinA=nBDLPin2[i]-1;		 
				nPinB=nBDLPin2[i+1]-1;
				}
				*/

		nPinA=i;//sylee161214


		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}



		//SYLEE161214  Sub71.AOnPinSet1(2, nPinB, 1); //vc ========>




		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC, //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 1);   //VB //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


		Sub71.Delay_100ns(nDelay10); 

		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  



		//sylee161214		Sub71.OnPinAllReset1(); ///PIN ALL RESET 

		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 

		if( nFil==3 ){//HV OPEN ENABLE //SYLEE20120411
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}else if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}else if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}else{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}else if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}


		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}


		int Error;

		if(nOpenMode1==2){
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		}else{
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}else{
				Error=1;
			}
		} 



		if(Error==1){
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task1;
			}
			nReCo1=0;

			if(nFailL1CoOpen<3000){//array size nFailL1[

				nFailL1CoOpen++;

				nFailL1Open[nFailL1CoOpen][F_VB_PIN]=nPinA+1;//pin 1~                       //son  4:F_VB_PIN 
				nFailL1Open[nFailL1CoOpen][F_VC_PIN]=nPinB+1;//pin 1~                       //son  5:F_VC_PIN
                                                                                                                   
				nFailL1Open[nFailL1CoOpen][F_RAW_R]=Sub71.R1 ;//Sub71.R1                    //son  6:F_RAW_R
				nFailL1Open[nFailL1CoOpen][F_PRORV_RAW_R]=nProRv1;//nProRv1 판정 R          //son 17: F_PRORV_RAW_R

				//son220419_6 OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2  //son220412
				//son220419_6 nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2
                int calPart =FileSysInfo01.checkUseCalDataPart(nFailL1Open[i][F_VB_PIN]-1,  nFailL1Open[i][F_VC_PIN]-1);    //son220419_2
				RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart);           //son220419_6
				nFailL1Open[nFailL1CoOpen][F_REAL_R]=Sub71.R1 ;                 //son220419_6 16으로 잘못된 것을 15:F_REAL_R로 수정


				if(nFailL1CoOpen >=SysInfo05.m_nConEr){
					goto Task211;
				}

			}


		}//}else{//fail  //SYLEE20120117


		//	}

		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC, //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		Sub71.Delay_100ns(100);  //10us
		Sub71.AOnPinSet1(2, nPinA, 2);  //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212
 


	//sylee161214  }//ip
	//####################################################################################################

	Sub71.OnPinAllReset1(); ///PIN ALL RESET   //sylee170112
	
	
	
	//#########################################################################
	//sylee170112
	
	for(   i5=nHalfPin1; i5<= Def_SwitchMaxPin; i5++){ 
		if( nPin1[i5]==1){
			nPinA=i5;
			Sub71.AOnPinSet1(2, nPinA, 2); //vC  	//<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1	 
		}
	}

	//====================================================================================================================================
	//sylee161214 for( int ip=1; ip<=ipEnd; ip++ ){ 

	//sylee161214 	if(nRCount<=0){
	//ERROR DEBUG   PIECE=ip NO LIST.
	//sylee161214 		continue;//sylee20120402
	//sylee161214 	}

	nRCount=Def_SwitchMaxPin;//sylee161214

	for( i=nHalfPin1; i< nRCount; i++) //SYLEE20111212   //sylee170112   0-> nHalfPin1
	{
		DoEvents(); 


		if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
			m_list1.AddString(str); 
			nlist1++;
			goto Task211;					
		}


 Task201:

/*		
		if(nOpenMode1==2){//pcb open  //sylee161214 move
			nPinA=nBDLData2[1][i][1];		 
			nPinB=nBDLData2[1][i][2];
		}else{
			nPinA=nBDLPin2[i]-1;		 
			nPinB=nBDLPin2[i+1]-1;
		}
*/

		nPinA=i;//sylee161214


		if( nPin1[nPinA]!=1){//sylee161214
			continue;
		}



		//SYLEE161214  Sub71.AOnPinSet1(2, nPinB, 1); //vc ========>




		Sub71.AOnPinReSet1(2,nPinA, 2); //   vc reset     nMod =1,VB,    =2VC, //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		Sub71.Delay_100ns(100);  //10us  
		Sub71.AOnPinSet1(2, nPinA, 1);   //VB //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


		Sub71.Delay_100ns(nDelay10); 

		if( nCC==2){ 
			Sub71.AOnReadV1I1(7);     	 		
		}else{ 
			Sub71.AOnReadV1I1(nVRel);  
		}  



		//sylee161214		Sub71.OnPinAllReset1(); ///PIN ALL RESET 

		if(Sub71.Y1<0.0000001){
			Sub71.Y1=0.0000001;
		} 

		if( nFil==3 )//HV OPEN ENABLE //SYLEE20120411
		{
			if( nIR==1 || nIR==2 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;   
			}
			else  if( nIR==3 ||  nIR==4 ){
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;
			}
			else  if( nIR==5 ){	//sylee121115
				Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0; 
			} 
		}
		else 
		{
			if( nIR==1 ||  nIR==2 ){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000.0;   
			}
			else  if( nIR==3 ||  nIR==4 ||  nIR==5){
				Sub71.R1=((Sub71.V1-Sub71.X1)/Sub71.Y1)*1000000.0;//sylee121027->1000000=>1000
			}      
		}


		if((Sub71.Y1<0.1)&&(Sub71.X1<5)){//interlock  //sylee121106
			Sub71.R1=9999999;
		}


		int Error;

		if(nOpenMode1==2){
			if( Sub71.R1<=nProRv1){//pass 
				Error=0;
			}
			else {
				Error=1;
			}
		}
		else
		{
			if( Sub71.R1>=nProRv1){//pass 
				Error=0;
			}
			else{
				Error=1;
			}
		} 



		if(Error==1)
		{
			nReCo1++;
			if( nReCo1<nRetrySet){											 
				Sub71.Delay_100ns(3000);  //0.3ms  //SYLEE130315 
				goto  Task201;    
			}
			nReCo1=0;

			if(nFailL1CoOpen<3000){//array size nFailL1[

				nFailL1CoOpen++;

				nFailL1Open[nFailL1CoOpen][F_VB_PIN]=nPinA+1;//pin 1~                   //son  4:F_VB_PIN 
				nFailL1Open[nFailL1CoOpen][F_VC_PIN]=nPinB+1;//pin 1~                   //son  5:F_VC_PIN
                                                                                                               
				nFailL1Open[nFailL1CoOpen][F_RAW_R]=Sub71.R1 ;//Sub71.R1                //son  6:F_RAW_R
				nFailL1Open[nFailL1CoOpen][F_PRORV_RAW_R]=nProRv1;//nProRv1 판정 R      //son 17: F_PRORV_RAW_R

				//son220419_6 OnConvert_Real_R1(m_nMode1, Sub71.R1, PART_UPPER); //sylee161215-2 //son220412
				//son220419_6 nFailL1Open[nFailL1CoOpen][16]=Sub71.R1 ;//Sub71.R1 //sylee161215-2
                int calPart =FileSysInfo01.checkUseCalDataPart(nFailL1Open[i][F_VB_PIN]-1,  nFailL1Open[i][F_VC_PIN]-1);    //son220419_2
				RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart);           //son220419_6
				nFailL1Open[nFailL1CoOpen][F_REAL_R]=Sub71.R1 ;                 //son220419_6 16으로 잘못된 것을 15:F_REAL_R로 수정


				if(nFailL1CoOpen >=SysInfo05.m_nConEr){
					goto Task211;
				}

			}


		}//}else{//fail  //SYLEE20120117


		//	}

		Sub71.AOnPinReSet1(2,nPinA, 1); //   vc reset     nMod =1,VB,    =2VC, //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		Sub71.Delay_100ns(100);  //10us
		Sub71.AOnPinSet1(2, nPinA, 2);  //<!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


	}////for END      for( i=1; i<=nRCount ; i++){ //SYLEE20111212



//sylee161214  }//ip



Task211:

	//==============================================================================================================================
	//################################################################################################################################
	
	Sub71.OnPinAllReset1(); ///PIN ALL RESET 
	Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
	Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704
	Sub71.AOnReadV1Enable1(5); //sylee130711
	
	////////////////////////
	//[PRINTING 
    
	  
	FILE *fp;  
	int nFile1;

	nFile1=0;

 
	::ZeroMemory(&fName , sizeof(fName ));
	 strcat( fName, "D:\\Manual_OPEN1_JigTest.TXT" ); 

	fp = fopen(fName,"wt"); 
	if(fp == NULL){
		nFile1=2;
	//	return 0;
	}

  
	 	str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
	 
   	if( nFailL1CoOpen <=0){
		str.Format("  ***>    Open  NO Error *******  ");			 
	}
	else{
		str.Format("  ===>  Result : [Open  Error] => %d  ", nFailL1CoOpen );			 		
	}
  	m_list1.AddString(str); 
	nlist1++;
  
	//if(nFile1==0){fprintf(fp, str);fprintf(fp, "\n");}
	if(nFile1==0){fprintf(fp," %s \n",str);}


	str.Format(" ");			 		
   	m_list1.AddString(str); 
	nlist1++;
	 
	if(nFile1==0) fprintf(fp, "%s  \n",str );

	if( nFailL1CoOpen==SysInfo05.m_nConEr){
 		str.Format("  ***>    Open Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nConEr );			 		
   		m_list1.AddString(str); 
		nlist1++;
	}

		str.Format(" ");			 		
   		m_list1.AddString(str); 
		nlist1++;
		  if(nFile1==0) fprintf(fp, "%s  \n",str );
 
	for( i=1 ; i<=nFailL1CoOpen ; i++)
	{
		//sylee161214	str.Format("   --> Open List No=%d,      pin  %5.0f - %5.0f  ",i, nFailL1Open[i][4],nFailL1Open[i][5] );			 
		//	str.Format("   --> Open List No=%d,      pin  %5.0f   ",i, nFailL1Open[i][4]  );//sylee161214			 


		if( nRdisplay1!=1){
			str.Format("   --> Open List No=%d,      pin  %5.0f   ",i, nFailL1Open[i][4]  );//sylee161214
		}
		else
		{



			if(m_nMode1==51){//100V

			//sylee221014	if(nFailL1Open[i][16]>=200){
				if(nFailL1Open[i][F_REAL_R]>=200){//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f  ,  R> 200 ohm ",i, nFailL1Open[i][4]  );//sylee161214
				}
				else
				{
					//sylee181217-3		 if( SysSet13.m_nSet5==1 ){//sylee161221
					//sylee181217-3	          str.Format("   --> Open List No=%d,      pin  %5.0f  ,   R= %7.0f ohm  ",i, nFailL1Open[i][4],nFailL1Open[i][16]  );//sylee161214
					//sylee181217-3		 }else{
					str.Format("   --> Open List No=%d,      pin  %5.0f    ",i, nFailL1Open[i][4]  );//sylee161214
					//sylee181217-3		 }
				}

			}
			else
			{

			//sylee221014	if(nFailL1Open[i][16]>1000){
				if(nFailL1Open[i][F_REAL_R]>1000){//sylee221014
					str.Format("   --> Open List No=%d,      pin  %5.0f  ,  R> 1k ohm ",i, nFailL1Open[i][4]  );//sylee161214
				}
				else
				{
					//sylee181217-3	if( SysSet13.m_nSet5==1 ){//sylee161221
					//sylee181217-3       str.Format("   --> Open List No=%d,      pin  %5.0f  ,   R= %7.0f ohm  ",i, nFailL1Open[i][4],nFailL1Open[i][16]  );//sylee161214
					//sylee181217-3	}else{
					str.Format("   --> Open List No=%d,      pin  %5.0f    ",i, nFailL1Open[i][4]  );//sylee161214
					//sylee181217-3	}
				}
			}


		}


		m_list1.AddString(str); 
		nlist1++;
		if(nFile1==0) fprintf(fp, "%s  \n",str );
	}

	str.Format(" ");			 		
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0) fprintf(fp, "%s  \n",str );



	if( nStop2==2){
		str.Format("  ***> Manual Open Check    STOP.  *******  ");	 
	}
	else{
		str.Format("  ***> Manual Open Check    FINISH.  *******  ");			 
	}
	m_list1.AddString(str); 
	nlist1++;
	if(nFile1==0) fprintf(fp, "%s  \n",str );


	if( nStop2==2){
		str.Format("STOP"); 
	}
	else{
		str.Format("FINISH"); 	 
	}
		
	
	m_Label1001.SetCaption(str);
	//		  if(nFile1==0) fprintf(fp, "%s  \n",str );



	if(nFile1==0)    fclose(fp); 

		
	 //	delete [] nBDLPin1; 
     //   delete [] nBDLPin2; 


 

//::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);
    
	
	return 1;

}//end of CChildView3::OnManualShort1(){//sylee131023

 



//son220419_6 OnConvert_Real_R1()를 공통함수인 RCalCovert() 로 대체하고 삭제함.
#if 0
//son raw R -> real R
double  CModeMan12::OnConvert_Real_R1(int nMode, double R1_Raw, int part) //sylee161215-2   //son220412
{


	CString str;
	//sylee20120308 Calibration convertor   open R

	int ka, k ,m_nMode1 ,nIn,nOut;
	double  aa,ab;						 


	aa=0.0;
	ab=0.0;

	nIn=CAL_REF_RAWR;	 nOut=CAL_REF_R;  // raw(3) -> real(2) 		 



	//   m_nMode1=(int)nFailL1Open[i][11];//input 1
	m_nMode1=nMode;

	ka=(int)gdCal1[part][m_nMode1][0][0];// Cal  list sum
	//  Sub71.R1=nFailL1Open[i][6]; //input2
	Sub71.R1=R1_Raw;


	if( (Sub71.R1>0) && (ka>0) && (ka<=MAX_CAL_RLIST) )   //son220408 128: MAX_CAL_RLIST
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//REAL R-> RAW

		//sylee130709  if(m_nMode1>60 &&  m_nMode1<71){   //R==>I ONLY
		if(m_nMode1> (MODE_SHORT_START -1) &&  m_nMode1< MAX_TEST_MODE){   //R==>I ONLY

			//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

			//gdCal1[part][m_nMode1][CAL_REF_R][k]  real  
			//gdCal1[part][m_nMode1][CAL_REF_RAWR][k]  cal R raw															 
			for( k=1;  k<=ka;  k++){											 
				if( Sub71.R1> gdCal1[part][m_nMode1][nIn][k]){	 
					break;  
				} 												 
			}


			if( k==1)  // min 값 보다  작은 경우 
			{
				if(gdCal1[part][m_nMode1][nIn][0]>=0.0){													
					// if(gdCal1[part][m_nMode1][nOut][0]<0.0){ ///?????????????????????????????  
					//	 Sub71.R1= -1.0 ;  
					//}else{
					if(gdCal1[part][m_nMode1][nIn][1]!=0){
						Sub71.R1= (Sub71.R1/gdCal1[part][m_nMode1][nIn][1])  *  gdCal1[part][m_nMode1][nOut][1]; 
					}
					else
					{

						//error
						str.Format("Error No 7221 ,  Calibration Raw Data zero.  Use Error!,   Divide by Zero. \n /gdCal1[part][m_nMode1][nIn][1] \n Mode=%d  OnInit  Real R -> Raw R Converting Error   !       \n\n" ,m_nMode1  ); 
						AfxMessageBox(str);	


					}
					/// } 

				}
			}
			else if( k==ka)    // max????????????
			{
				if(gdCal1[part][m_nMode1][nIn][ka]!=0){												     
					Sub71.R1= ((Sub71.R1-gdCal1[part][m_nMode1][nIn][ka-1])/(gdCal1[part][m_nMode1][nIn][ka]-gdCal1[part][m_nMode1][nIn][ka-1])) *   (gdCal1[part][m_nMode1][nOut][ka]-gdCal1[part][m_nMode1][nOut][ka-1])+gdCal1[part][m_nMode1][nOut][ka-1]; //sylee20120627
				}
				else{
					str.Format("Error No 7202 ,   Divide by Zero. \n  gdCal1[part][m_nMode1][nIn][ka]   \n  Mode=%d  OnInit  Real R -> Raw R Converting Error   !       \n\n" ,m_nMode1  ); 
					AfxMessageBox(str);
				}
			}
			else
			{
				if( (gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1])!=0.0){
					aa=(Sub71.R1-gdCal1[part][m_nMode1][nIn][k-1])/(gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1]);  // [3]= lookup table Calibration Raw R
					ab=gdCal1[part][m_nMode1][nOut][k]-gdCal1[part][m_nMode1][nOut][k-1]; 
					Sub71.R1=ab*aa+gdCal1[part][m_nMode1][nOut][k-1];											 
				}
				else{
					Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][nIn][k-1])*gdCal1[part][m_nMode1][nOut][k-1];	
				}
			}

			//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

		}
		else
		{

			//gdCal1[part][m_nMode1][2:CAL_REF_R][k]     real  
			//gdCal1[part][m_nMode1][3:CAL_REF_RAWR][k]  cal R raw															 
			for( k=1;  k<=ka;  k++){											 
				if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k]){	 
					break;  
				} 												 
			}


			if( k==1)  // min 값 보다  작은 경우 
			{
				if(gdCal1[part][m_nMode1][nIn][0]>=0.0){													
					// if(gdCal1[part][m_nMode1][nOut][0]<0.0){ ///?????????????????????????????  
					//	 Sub71.R1= -1.0 ;  
					//}else{
					if(gdCal1[part][m_nMode1][nIn][1]!=0){
						Sub71.R1= (Sub71.R1/gdCal1[part][m_nMode1][nIn][1])  *  gdCal1[part][m_nMode1][nOut][1]; 
					}
					else{

						//error
						str.Format("Error No 7221 ,  Calibration Raw Data zero.  Use Error!,    Divide by Zero. \n /gdCal1[part][m_nMode1][nIn][1] \n Mode=%d  OnInit  Real R -> Raw R Converting Error   !       \n\n" ,m_nMode1  ); 
						AfxMessageBox(str);	


					}
					/// } 

				}
			}

			else if( k==ka)    // max????????????
			{
				if(gdCal1[part][m_nMode1][nIn][ka]!=0){												     
					Sub71.R1= ((Sub71.R1-gdCal1[part][m_nMode1][nIn][ka-1])/(gdCal1[part][m_nMode1][nIn][ka]-gdCal1[part][m_nMode1][nIn][ka-1])) *   (gdCal1[part][m_nMode1][nOut][ka]-gdCal1[part][m_nMode1][nOut][ka-1])+gdCal1[part][m_nMode1][nOut][ka-1]; //sylee20120627
				}
				else{
					str.Format("Error No 7202 ,   Divide by Zero. \n  gdCal1[part][m_nMode1][nIn][ka]   \n  Mode=%d  OnInit  Real R -> Raw R Converting Error   !       \n\n" ,m_nMode1  ); 
					AfxMessageBox(str);
				}
			}
			else
			{
				if( (gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1])!=0.0){
					aa=(Sub71.R1-gdCal1[part][m_nMode1][nIn][k-1])/(gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1]);  // [3]= lookup table Calibration Raw R
					ab=gdCal1[part][m_nMode1][nOut][k]-gdCal1[part][m_nMode1][nOut][k-1]; 
					Sub71.R1=ab*aa+gdCal1[part][m_nMode1][nOut][k-1];											 
				}
				else{
					Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][nIn][k-1])*gdCal1[part][m_nMode1][nOut][k-1];	
				}
			}

			//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

		}

	}							 



	if( Sub71.R1<0){
		Sub71.R1=0.1;
	}

	//	 nFailL1Open[i][15]=Sub71.R1;


	return Sub71.R1;
}
#endif						 


