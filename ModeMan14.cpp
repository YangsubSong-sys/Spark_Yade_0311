// ModeMan14.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeMan14.h"
#include "ModeSub71.h"
#include "ModeSet12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Pin Probe
#include "comidaq.h"//SYLEE
#include "comiled.h"//SYLEE
#include "comitoggleswitch.h"//SYLEE
#include "ComiDaqComm.h" //SYLEE
#include "Dio.h" //SYLEE
#include "comiled.h"
#include "resource.h" //SYLEE

#include "ModeMan16.h"
#include "A_Define1.h"//sylee161126
#include "FileSysInfo.h"
 
extern CDio	DIO; //SYLEE

//#define COMI_SD414	0xB414
//#define COMI_SD424	0xB424
int nStartFlag1;
int nStartDI1;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

extern CSysSet13  SysSet13; //sylee161006

int  nTotalCoSet;//sylee130522

extern int nTotalPin;//sylee130522

extern void DoEvents(); 
 
 

extern short nPinSetSkipVb; //sylee151005 //ACE400	//son230127
extern short nPinSetSkipVc; //sylee151005 //ACE400	//son230127


extern CModeSub71  Sub71;
extern CModeSet12  ModeSet12; 

extern CSysSet12  SysSet12;  //Pin SETUP    


 

extern int nCC,  nVRel, nIR;

extern double nVSet,nISet;




 
extern short PinSearchPin[Def_SwitchMaxPin];    //nSwitchMaxPinsylee130518-1 pin search bdl


int nPinSearchAll;//sylee130518 //son 0: BDL check box 1 �϶�. BDL pin�� Ž�� 
                                //son 1: BDL check box 0 �϶�. ALL Pin Ž�� 

//������. 
/////////////////////////////////////////////////////////////////////////////
// CModeMan14 dialog

//MANUAL PIN SEARCH 

CModeMan14::CModeMan14(CWnd* pParent /*=NULL*/)
	: CDialog(CModeMan14::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeMan14)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeMan14::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeMan14)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LIST2, m_list1);
//		DDX_Control(pDX, IDC_LIST2, m_list2);//sylee130310

		DDX_Control(pDX, IDC_LABEL1001, m_LabelStatus);	//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus �� ����
		DDX_Control(pDX, IDC_LABEL1002, m_LabelTimeCnt);//son-32KPIN-20190902: m_Label1002 -> m_LabelTimeCnt �� ����
		DDX_Control(pDX, IDC_CHECK1,    m_checkBdl);	//son-32KPIN-20190902: m_check1 -> m_checkBdl�� ����
		DDX_Control(pDX, IDC_LABEL101,  m_LabelFault);	//son-32KPIN-20190902: m_Label101->m_LabelFault �� ����

		DDX_Control(pDX, IDC_LABEL1,  m_Label1);
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);

		DDX_Control(pDX, IDC_BUTTON101,  m_button101); 
		DDX_Control(pDX, IDC_BUTTON102,  m_button102);  

		DDX_Control(pDX, IDC_OK, m_buttonOk); 

//	    DDX_Control(pDX, IDC_COMIDAQCTRL1, m_ComiDaq3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeMan14, CDialog)
	//{{AFX_MSG_MAP(CModeMan14)
		// NOTE: the ClassWizard will add message map macros here
			ON_BN_CLICKED(IDC_CHECK1, OnCheck1)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CModeMan14, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeMan14) 
	ON_EVENT(CModeMan14, IDC_BUTTON101, -600 /* Click */, OnButtonStart, VTS_NONE)
	ON_EVENT(CModeMan14, IDC_BUTTON102, -600 /* Click */, OnButtonStop, VTS_NONE)
	ON_EVENT(CModeMan14, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 

/////////////////////////////////////////////////////////////////////////////
// CModeMan14 message handlers

//son-32KPIN-20190902: OnButton101 -> OnButtonStart �� ����
void CModeMan14::OnButtonStart() //start
{ 
	CString str;
	// int Ret;

	if(nPinSearchFlag==1){
		return;	  
	}

	str.Format("START"); 
	m_LabelStatus.SetCaption(str);		//son �����Ȳ Display
										//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus �� ����

	nPinSearchFlag=1;  
	Sub71.AOnPinSearch(1);

	OnATest101();

}


//son-32KPIN-20190902: OnButton102 -> OnButtonStop ���� ����
void CModeMan14::OnButtonStop() //stop
{
//	   int Ret;
   
	nPinSearchFlag=2;  
    Sub71.AOnPinSearch(0);

	OnEnd101();
}


void CModeMan14::A_Language()  //LANGUAGE CHANGE
{
	if( gDef_Language1==3)
	{ 

		m_Label1.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 101));
		m_LabelFault.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 102));
		//  m_LabelStatus.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 103));
		m_LabelTimeCnt.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 103));
		//IDC_CHECK1
		m_Label2.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 105));

		m_button101.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 106));
		m_button102.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 107));
		m_buttonOk.SetWindowText(LoadMessage(1,"TitleSetupPinSearch", 108));
	}

} 



void CModeMan14::OnClickOk() //exit
{   
    
	OnButtonStop(); //stop  //son-32KPIN-20190902: OnButton102 -> OnButtonStop ���� ����

	CDialog::OnOK();	
}


int CModeMan14::OnATest101() 
{  
	if( nPinSearchFlag==1){	
		OnSet();
		OnRun101();
	 	//temp OnRun101_test();
	}
    else  if( nPinSearchFlag==2){	
	    //OnRun101();
	}  

	return 1;
}

 


int CModeMan14::OnRun101() //run
{

	CString str, strLabel;
	int nTimeDelay1;
	int nPinSearchSetup[50][3];	//son 	 nPinSearchSetup[0][0] : box ����
								//son    nPinSearchSetup[box][1] : BoxStartPin, nPinSearchSetup[box][2] : BoxEndPin

	//	int a1,b1;//SYLEE130215JOYTE1CH

	nPinSetSkipVb=0; //sylee151007 //ACE400 	//son230127
	nPinSetSkipVc=0; //sylee151007 //ACE400		//son230127

	str.Format("RUN"); 
	m_LabelStatus.SetCaption(str);	//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus �� ����

	strLabel.Format("                        "); 									 
	m_LabelFault.SetCaption(strLabel);	//son-32KPIN-20190902: m_Label101->m_LabelFault �� ����

	::ZeroMemory(&nPinSearchSetup,sizeof(nPinSearchSetup));

/*
    nPinSearchSetup[0][0]=3;
    nPinSearchSetup[1][1]=0;      nPinSearchSetup[1][2]=2047;
	nPinSearchSetup[2][1]=2048;   nPinSearchSetup[2][2]=4095;
	nPinSearchSetup[3][1]=4096;   nPinSearchSetup[3][2]=6143;
*/
//nPinSearchSetup[0][0]=4;//16k


	if( SysSet12.m_nPinTotMax== PIN_TOT_4K){ //4k : 1	//son-32KPIN-20190902: enum ���� ����
	    //sylee151007    nPinSearchSetup[0][0]=1; 
	    nPinSearchSetup[0][0]=2; //sylee151007
	}
	else  if( SysSet12.m_nPinTotMax== PIN_TOT_8K){ //8k : 2
	    nPinSearchSetup[0][0]=2; 
	}
	else  if( SysSet12.m_nPinTotMax== PIN_TOT_16K){ //16k : 3
	    nPinSearchSetup[0][0]=4; 
	}
	else  if( IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax) )    //son210202
	{
#if 0
	    nPinSearchSetup[0][0]=8;//32k
#else
	    return OnRun101_32K();	//son-32KPIN-20191223
#endif
	}
	else {
	    AfxMessageBox("   Error No 7703 !.   [PM]=>[MAX PIN] SET Check!.   ", MB_OK);		     
	    OnEnd101();
	    return 1;
	}


	//son    PIN_TOT_4K	:  
	//			nPinSearchSetup[0][0] : box 2��
	//							startPin					endPin
	//			box 1		nPinSearchSetup[1][1]=0			nPinSearchSetup[1][2]=2047
	//			box 2   	nPinSearchSetup[2][1]=16384		nPinSearchSetup[2][2]=18431
	//
	//son    PIN_TOT_8K	:  
	//			nPinSearchSetup[0][0] : box 2��
	//							startPin					endPin
	//			box 1		nPinSearchSetup[1][1]=0			nPinSearchSetup[1][2]=4095
	//			box 2   	nPinSearchSetup[2][1]=16384		nPinSearchSetup[2][2]=20479
	//
	//son    PIN_TOT_12K	:  
	//			nPinSearchSetup[0][0] : box 3��
	//							startPin					endPin
	//			box 1		nPinSearchSetup[1][1]=0			nPinSearchSetup[1][2]=4095
	//			box 2   	nPinSearchSetup[2][1]=4096		nPinSearchSetup[2][2]=8191
	//			box 3   	nPinSearchSetup[3][1]=16384		nPinSearchSetup[3][2]=20479
	//
	//son    PIN_TOT_16K	:  
	//			nPinSearchSetup[0][0] : box 4��
	//							startPin					endPin
	//			box 1		nPinSearchSetup[1][1]=0			nPinSearchSetup[1][2]=4095
	//			box 2   	nPinSearchSetup[2][1]=4096		nPinSearchSetup[2][2]=8191
	//			box 3   	nPinSearchSetup[3][1]=16384		nPinSearchSetup[3][2]=20479
	//			box 4   	nPinSearchSetup[4][1]=20480		nPinSearchSetup[4][2]=24575
	//
	//son    PIN_TOT_32K_TRANS	:  
	//			nPinSearchSetup[0][0] : box 8��
	//							startPin					endPin
	//			box 1		nPinSearchSetup[1][1]=0			nPinSearchSetup[1][2]=4095
	//			box 2   	nPinSearchSetup[2][1]=4096		nPinSearchSetup[2][2]=8191
	//			box 3   	nPinSearchSetup[3][1]=8192		nPinSearchSetup[3][2]=12287
	//			box 4   	nPinSearchSetup[4][1]=12288		nPinSearchSetup[4][2]=16383
	//
	//			box 5		nPinSearchSetup[5][1]=16384		nPinSearchSetup[5][2]=20479
	//			box 6   	nPinSearchSetup[6][1]=20480		nPinSearchSetup[6][2]=24575
	//			box 7   	nPinSearchSetup[7][1]=24576		nPinSearchSetup[7][2]=28671
	//			box 8   	nPinSearchSetup[8][1]=28672		nPinSearchSetup[8][2]=32767	

	// nPinSearchSetup[0][0]=8;//32k 
	//sylee161110-2

	for( int i=1; i<=nPinSearchSetup[0][0];  i++)  //32k 
	{
		//sylee151007   nPinSearchSetup[i][1]=4096*(i-1);   
		//sylee151007 nPinSearchSetup[i][2]=4096*i-1; 

		if( SysSet12.m_nPinTotMax== PIN_TOT_4K) //4k:1   box =2		//son-32KPIN-20190902: enum ���� ����
		{
			//sylee151007    nPinSearchSetup[0][0]=1; 
			if( i==1){ 
				nPinSearchSetup[i][1]=2048*(i-1);
				nPinSearchSetup[i][2]=2048*i-1; 			 
			}
			else{
				//sylee161121-1        nPinSearchSetup[i][1]=16384+ 2048*(i-1);
				//sylee161121-1		 nPinSearchSetup[i][2]=16384+ 2048*i-1;
				nPinSearchSetup[i][1]=16384+ 2048*(i-2);//sylee161121-1        
				nPinSearchSetup[i][2]=16384+ 2048*(i-1)-1;//sylee161121-1        

			}
		}

		else if( SysSet12.m_nPinTotMax== PIN_TOT_8K) //8k:2  box =2	//son-32KPIN-20190902: enum ���� ����
		{
			// nPinSearchSetup[0][0]=2; 
			if( i==1){ 
				nPinSearchSetup[i][1]=4096*(i-1);
				nPinSearchSetup[i][2]=4096*i-1; 			 
			}
			else{
				//sylee161006        nPinSearchSetup[i][1]=16384+ 4096*(i-1); 
				//sylee161006		 nPinSearchSetup[i][2]=16384+ 4096*i-1;
				//sylee181217-3	 if(SysSet13.m_nSet5==1 ){//sylee161005-2  //DD PIN BLOCK
				//sylee181217-3          nPinSearchSetup[i][1]= 4096*(i-1);
				//sylee181217-3	      nPinSearchSetup[i][2]= 4096*i-1;
				//sylee181217-3	 }else{
				nPinSearchSetup[i][1]=16384+ 4096*(i-2);//sylee161121-1
				nPinSearchSetup[i][2]=16384+ 4096*(i-1)-1;//sylee161121-1
				//sylee181217-3	 }
			}
		}

		else if( SysSet12.m_nPinTotMax== PIN_TOT_16K) //16k:3  box =4	//son-32KPIN-20190902: enum ���� ����
		{
			// nPinSearchSetup[0][0]=4; 
			if( i<=2){ 
				nPinSearchSetup[i][1]=4096*(i-1);
				nPinSearchSetup[i][2]=4096*i-1; 			 
			}
			else{
				nPinSearchSetup[i][1]=16384+ 4096*((i-2)-1);
				nPinSearchSetup[i][2]=16384+ 4096*(i-2)-1;
			}
		}

		else if( SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS //32k:4    box =8	//son-32KPIN-20190902: enum ���� ���� //son230216_1
				 //|| SysSet12.m_nPinTotMax== PIN_TOT_24K_UP_TR //son230216_1
				 )
		{
			// nPinSearchSetup[0][0]=8;//32k
			if( i<=4){ 
				nPinSearchSetup[i][1]=4096*(i-1);
				nPinSearchSetup[i][2]=4096*i-1; 			 
			}
			else{
				nPinSearchSetup[i][1]=16384+ 4096*((i-4)-1);
				nPinSearchSetup[i][2]=16384+ 4096*(i-4)-1;
			}
		}

		else
		{

			if( i<=4){ 
				nPinSearchSetup[i][1]=4096*(i-1);
				nPinSearchSetup[i][2]=4096*i-1; 			 
			}
			else{
				nPinSearchSetup[i][1]=16384+ 4096*((i-4)-1);
				nPinSearchSetup[i][2]=16384+ 4096*(i-4)-1;
			}
		}

		/*
		   nPinSearchSetup[1][1]=0;      nPinSearchSetup[1][2]=4096-1; 
		   nPinSearchSetup[2][1]=4096;   nPinSearchSetup[2][2]=4096*2-1;
		   nPinSearchSetup[3][1]=4096*2;   nPinSearchSetup[3][2]=4096*3-1; 
		   nPinSearchSetup[4][1]=4096*3;   nPinSearchSetup[4][2]=4096*4-1;
		   */


	}


	//if(	SysSet13.m_nPinBlockType==4  ) {//sylee160928  //YAMAHA  //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA  
	if(	SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  ) //son-32KPIN-20190902  //son230216_1
	{
		nPinSearchSetup[1][1]=0;      nPinSearchSetup[1][2]=2048-1; //sylee161111
		nPinSearchSetup[2][1]=4096*4;   nPinSearchSetup[2][2]=4096*4+2048-1;  //sylee161111
	}

	//if(	SysSet13.m_nPinBlockType==6 ) {//sylee171109-1
	if(	SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421 ) //son-32KPIN-20190902 
	{
		nPinSearchSetup[1][1]=0;      nPinSearchSetup[1][2]=2048*2-1+2048; //sylee161111
		nPinSearchSetup[2][1]=4096*4;   nPinSearchSetup[2][2]=4096*4+2048*2-1+2048;  //sylee161111
	}

	//if(	SysSet13.m_nPinBlockType==2 ) {//sylee180411
	if(	SysSet13.m_nPinBlockType== PIN_BLK_SD ) //son-32KPIN-20190902 
	{
		nPinSearchSetup[1][1]=0;      nPinSearchSetup[1][2]=2048*2-1; //sylee180411
		nPinSearchSetup[2][1]=4096*4;   nPinSearchSetup[2][2]=4096*4+2048*2-1;  //sylee180411
	}


	nTimeDelay1=20;//200;

	Sub71.AOnPinSearch(1);


	//son-32KPIN-20190902: �������� ���������� ����
	int   nEventCount = 0;		//son nCo ->nEventCount�� ����. DoEvent ȣ���� ���� ī��Ʈ
	int   nlist1 = 0;			//son List�ڽ� ��� ī��Ʈ
	int   nBoxCount = 0;		//son nCount ->nBoxCount�� ����. Pin Search timeout. üũ�� box Ƚ��
	int   nTimeCount = 0;		//son nCount2 ->nTimeCount�� ����. Pin Search timeout Count

	while( nPinSearchFlag==1)
	{ 

		//testDoEvents();  
		// Sub71.Delay_100ns(50);   
		//nTotalPin=nBDLHead1[3][1]; 


		//son BDL ��� �ɸ� Search �Ѵٸ�
		//son �׽�Ʈ ��� ������ �� �ʰ� �����̹Ƿ�  Test�� �ʹ� ���� ���� �� �־ Test Count�� �÷���.
		if(nPinSearchAll!=1)//sylee130522
		{
			if(nTotalPin>3000){			//son	3000 < nTotal 
				nTotalCoSet=1000;
			}
			else  if(nTotalPin>2000){	//son	2000 < nTotalPin <= 3000
				nTotalCoSet=2000;
			}	
			else  if(nTotalPin>1000){	//son 	1000 < nTotalPin <= 2000
				nTotalCoSet=3000;
			}
			else  if(nTotalPin>500){	//son	500 < nTotalPin <= 1000
				nTotalCoSet=5000;
			}
			else  if(nTotalPin>100){	//son	100 < nTotalPin <= 500
				nTotalCoSet=10000;
			}
			else  if(nTotalPin>50){		//son 	50 < nTotalPin <= 100
				nTotalCoSet=30000;
			}
			else {						//son    nTotalPin <= 50
				nTotalCoSet=50000;
			} 
		}

		//son All Pin Search �Ѵٸ�
		else{
			nTotalCoSet=1000;
		}

		//son-32KPIN-20190902: i�� box �� ����
		for (int box=1; box<=nPinSearchSetup[0][0]; box++)	
		{   
			//son ��ü box�� 2ȸ �̻� Search �ߴٸ� Run Time out�� �غ�
			nBoxCount++;
			if (nBoxCount > (2*nPinSearchSetup[0][0]))
			{
				nBoxCount = 0;
				nTimeCount++;

				//son ���� Timeout ī��Ʈ�� ���
				str.Format("%d",nTotalCoSet - nTimeCount); 
				m_LabelTimeCnt.SetCaption(str);	//son-32KPIN-20190902: m_Label1002 -> m_LabelTimeCnt �� ����
			}

			//son Timeout ������ ���������Ƿ� 'Run Time Out'���� �������Ѵ�.
			if (nTimeCount > nTotalCoSet)
			{  
				nPinSearchFlag = 3;		//son Pin Search ����
				nTimeCount=0;
				DoEvents(); 							  

				str.Format(" RUN TIME OUT. "  );  	//son Timeout���� ������� ����Ʈ�� ǥ����
				m_list1.AddString(str); 			 		 
				m_list1.SetTopIndex(m_list1.GetCount()-1); 
				nlist1++;
			}


			//son-32KPIN-20190902: j�� pin���� ����
			for( int pin=nPinSearchSetup[box][1]; pin <= nPinSearchSetup[box][2]; pin++)
			{  

				nEventCount++;

				//	  Sub71.Delay_100ns(10);  //TEST

				//son 500 ī��Ʈ���� 1ȸ DoEvnet() ȣ��
				if( nEventCount>500){
					DoEvents(); 
					nEventCount=0;
				}



				if(nPinSearchAll!=1)//sylee130518
				{
					//son hwPin�� bdl ���Ͽ� �������� ������ pass �Ѵ�.
					if(PinSearchPin[pin]!=1){    ////sylee130522  Tm2->j    //son-32KPIN-20190902 j->pin
						continue;
					}
				}


				//if(SysSet13.m_nPinBlockType==6){//sylee171109-1
				if(SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son-32KPIN-20190902 
				{
					if((pin>=2048)&&(pin<4096)){//sylee171109-1
						continue;
					}
					else if(pin>=(4096*4+2048)&&(pin<(4096*4+4096))){//sylee171109-1
						continue;
					}
				}

				//Sub71.AOnPinSet1(1 , pin , 2);  //   nMod  =2:VB,   pin ����   //sylee200709-1-JTECH  (VB�� 2�� �ٲ�) 
				Sub71.AOnPinSet1(1 , pin , MOD_TR_SET_VB);  //   nMod  =2:VB,   pin ����   //son210220: MOD_TR_SET_VB�� ����

				Sub71.Delay_100ns(500);//sylee150318-test   

				//=================================================
				//I,V  READING 

				Sub71.AOnReadV1I1(1);			 


				//-------------------------------------------
				//son ������ 7mA �ʰ���� : Pin Fault �߰�! 
				//-------------------------------------------
				if( Sub71.Y1 > 7.0) //sylee150318-test 										
				{
					Beep(923, 20);

					CString boxStr;
					if (SysSet12.m_nPinTotMax == PIN_TOT_4K || SysSet12.m_nPinTotMax == PIN_TOT_8K) //son210603
                        boxStr.Format("%s", (box == (BOX_A+1)) ? "UPPER_A":
                                            (box == (BOX_A+2)) ? "LOWER_A": "UNDEF_BOX");

                    else if (SysSet12.m_nPinTotMax == PIN_TOT_16K)  //son210603
                        boxStr.Format("%s", (box == (BOX_A+1)) ? "UPPER_A":
                                            (box == (BOX_A+2)) ? "UPPER_B":
                                            (box == (BOX_A+3)) ? "LOWER_A":
                                            (box == (BOX_A+4)) ? "LOWER_B": "UNDEF_BOX");
                    else    //son210603
                        boxStr.Format("%s", (box == (BOX_A+1)) ? "UPPER_A":
                                            (box == (BOX_A+2)) ? "UPPER_B":
                                            (box == (BOX_A+3)) ? "UPPER_C":
                                            (box == (BOX_A+4)) ? "UPPER_D": 
                                            (box == (BOX_A+5)) ? "LOWER_A": 
                                            (box == (BOX_A+6)) ? "LOWER_B": 
                                            (box == (BOX_A+7)) ? "LOWER_C": 
                                            (box == (BOX_A+8)) ? "LOWER_D": "UNDEF_BOX");

					str.Format (" Unused, box=%s, hwPin=%5d", boxStr, pin+1); 
					strLabel.Format("  Pin= ? "  ); 									 

					//son ���� SwitchCard Pin�� �ش��ϴ� jigPin ���� ã�Ƽ� "str"���� �����. 
					//son-32KPIN-20190902 nNetPin, ik-> jigPin 
					for (int jigPin=0; jigPin<Def_SwitchMaxPin; jigPin++)  ///nSwitchMaxPin
					{
						if (PinMap[2][jigPin] == pin)
						{
						    //son list box�� ��¹�
							str.Format(" %9d, box=%s, hwPin=%5d" ,PinMap[0][jigPin]+1, boxStr, pin+1 );

							//son 'PinSearch' Fault Label ��¹�
							strLabel.Format(" %d " ,PinMap[0][jigPin]+1 ); 
							break;
						} 
					}


					//son Fault Pin�� ���� 'PinSearch' Fault �󺧿� ���
					m_LabelFault.SetCaption(strLabel);		//son-32KPIN-20190902: m_Label101->m_LabelFault �� ����

					//son list box���� �ش� pin �߰� 
					m_list1.AddString(str); 			 		 
					m_list1.SetTopIndex(m_list1.GetCount()-1); 
					nlist1++;

					if( nlist1>30){
						nlist1=0;
						m_list1.ResetContent();
						m_list1.AddString(str); 			 		 
						m_list1.SetTopIndex(m_list1.GetCount()-1); 
						nlist1++;
					}							 

					Sub71.OnPinAllReset1(); ///PIN ALL RESET  
					Sleep(100);//sylee150318-test
				}

				Sub71.OnPinAllReset1(); ///PIN ALL RESET 


			}//end of pin

		}//end of box
	}//while


	// �Ǵ� 
	if( nPinSearchFlag==3){  
		OnEnd101();
	}


	return 1;
}


//son-32KPIN-20191223
int CModeMan14::OnRun101_32K() //run for 32K
{

	CString str,strLabel;

	//int nPinSearchSetup[50][3];	-> nBoxNum

	//son 	 nPinSearchSetup[0][0]: Box ����
	//son    nPinSearchSetup[i][1]: BoxStartPin,  nPinSearchSetup[i][2]: BoxEndPin
	//
	//nPinSearchSetup[0][0]=8;//32k
	//
	

	m_list1.ResetContent();	// ������ Fault ����Ʈ �ʱ�ȭ

	int nBoxNum;		// 8: �ڽ�  ����
    nBoxNum = FileSysInfo01.getEndBoxNo(SysSet12.m_nPinTotMax);        //son211020



	Sub71.AOnPinSearch(1);

	int nEventCount = 0;	//son DoEvent ȣ���� ���� ī��Ʈ
	int nlist1 = 0;			//son List�ڽ� ��� ī��Ʈ

	long nBoxCount = 0;		//son Pin Serch timeout. üũ�� box Ƚ��
	long nTimeCount= 0;		//son Pin Search timeout Count

	while (nPinSearchFlag == 1)
	{ 

		//--------------------
		//son All Pin Search
		nTotalCoSet=1000;		
		

		//----------------------------------
		//son BDL ��� �ɸ� Search �Ѵٸ�
		
		//son �׽�Ʈ ��� ������ �� �ʰ� �����̹Ƿ�  Test�� �ʹ� ���� ���� �� �־ Test Count�� �÷���.
		//son nTotalPin==nBDLHead1[3][1]. CChildView1::OnDataLoad_Type_A()���� ������.
		if(nPinSearchAll!=1) 	
		{
			nTotalCoSet=50000; 			//son    nTotalPin <= 50
			
			if(nTotalPin>3000){			//son	3000 < nTotal 
				nTotalCoSet=1000;
			}
			else  if(nTotalPin>2000){	//son	2000 < nTotalPin <= 3000
				nTotalCoSet=2000;
			}	
			else  if(nTotalPin>1000){	//son 	1000 < nTotalPin <= 2000
				nTotalCoSet=3000;
			}
			else  if(nTotalPin>500){	//son	500 < nTotalPin <= 1000
				nTotalCoSet=5000;
			}
			else  if(nTotalPin>100){	//son	100 < nTotalPin <= 500
				nTotalCoSet=10000;
			}
			else  if(nTotalPin>50){		//son 	50 < nTotalPin <= 100
				nTotalCoSet=30000;
			}
		}



		for (int box=0; box < nBoxNum; box++)		
		{
			
			//son ��ü box�� 2ȸ �̻� Search �ߴٸ� Run Time out�� �غ�
			nBoxCount++;
			if (nBoxCount > (2*nBoxNum))
			{
				nBoxCount=0;
				nTimeCount++;

				//son ���� Timeout ī��Ʈ�� ���
				str.Format("%d",nTotalCoSet-nTimeCount); 
				m_LabelTimeCnt.SetCaption(str);	//son-32KPIN-20190902: m_Label1002 -> m_LabelTimeCnt �� ����
			}

			//son Timeout ������ ���������Ƿ� �������Ѵ�.
			if (nTimeCount > nTotalCoSet)
			{  
				nPinSearchFlag=3;	//son Pin Search ����
				nTimeCount=0;
				DoEvents(); 							  

				str.Format(" RUN TIME OUT. "  );  	//son Timeout���� ������� ����Ʈ�� ǥ����
				m_list1.AddString(str); 			 		 
				m_list1.SetTopIndex(m_list1.GetCount()-1); 
				nlist1++;
			}

			//son-32KPIN-20200228 : 16K_FT ��� ���� �ΰ��� skip
			//son 16K_FT�� ���� ��� 2box(box0, box1)��, �Ϻ� 2box(box4, box5) ���.
			//    box2�� box3�� �ǳ� �پ�� �Ѵ�.  box2: card64 ~ card95 
			//                                     box3: card96 ~ card127
			//if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS 	//son230216_1
			//        || SysSet12.m_nPinTotMax == PIN_TOT_12K             //son210319
			//        || SysSet12.m_nPinTotMax == PIN_TOT_12K_FULL_TRANS) //son210319 ���� 12K ��� box2,box3 2�� skip
			//if (IS_UPPER_8K(SysSet12.m_nPinTotMax))     //son210531
			if (ModeSet12.IsUpper8k(SysSet12.m_nPinTotMax))     //son210831
			{
				if (box >= 2  && box < 4)
					continue;
			}

			//son-8K4097-20200605 : 8K_����4097  ��� ����  3 box�� skip
			//son 8K_����4097�� ���� ��� 1box(box0)��, �Ϻ� 1box(box4) ���.
			//    box1,2,3�� �ǳ� �پ�� �Ѵ�.  box1: card32 ~ card63   128pin card
			//if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
			//        || SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)  //son210319 ���� 8K ��� box1, box2, box3 3�� skip //son230216_1
			//if (IS_UPPER_4K(SysSet12.m_nPinTotMax))     //son210531
			if (ModeSet12.IsUpper4k(SysSet12.m_nPinTotMax))     //son210831
			{
				if (box >= 1  && box < 4) //box0���� ����, 2,3,4��° box skip
					continue;
			}

            //son210531 ��� 3 box, �Ϻ� 1 box, ��� 4��° box�� skip 
		    //son240522 if (IS_UPPER_12K(SysSet12.m_nPinTotMax))     //son210531	
		    if (ModeSet12.IsUpper12k(SysSet12.m_nPinTotMax))     //son240522
			{
				if (box == 3) //box0���� ����, 4��° box skip
					continue;
			}

			int nBoxStartPin = box * MAX_BOX_PIN;		// box * 4096
			int nBoxEndPin   = (box+1) * MAX_BOX_PIN -1;

			for (int hwPin=nBoxStartPin; hwPin <= nBoxEndPin; hwPin++)
			{  
				//son 500 ī��Ʈ���� 1ȸ DoEvent() ȣ��
				nEventCount++;
				if( nEventCount>500){
					DoEvents(); 
					nEventCount=0;
				}

				//son BDL pin�� Search �Ѵٸ�
				if(nPinSearchAll!=1) //sylee130518
				{
					
					//son hwPin�� bdl ���Ͽ� �������� �����Ƿ� pass �Ѵ�.
					//    PinSearchPin[hwPin] : �ش� hwPin�� bdl ���Ͽ� �����Ѵٸ� 1�� ���õ�
					if(PinSearchPin[hwPin]!=1)    
						continue;
					
				}

                //son if(jigPin < 0) üũ�� 210831 �������� ����, Unused ���̽��� pinSearch ����ϴ°� �´� �� ����.
                
                //son230329 //son �ش� hwPin�� �´� jigPin�� ���ٸ� PinMap�� ���� pin�̹Ƿ� pass�Ѵ�. 
                //son230329 //int jigPin = ModeSet12.make32kSwPin(hwPin);     //son210831
                //son230329 int jigPin = nJigPinMap[hwPin]; //son230303
                //son230329 if (jigPin < 0) 
                //son230329     continue;

				//Sub71.AOnPinSet1(1 , hwPin , 2);  //   nMod  =2:VB,   pin ����   //sylee200709-1-JTECH  (VB�� 2�� �ٲ�) 
				Sub71.AOnPinSet1(1 , hwPin , MOD_TR_SET_VB);  //   nMod  =2:VB,   pin ����   //son210220: MOD_TR_SET_VB�� ����

				Sub71.Delay_100ns(500);//sylee150318-test   

				Sub71.AOnReadV1I1(1);		 //I,V  READING 


				//------------------------------------------
				//son ������ 7mA �ʰ����  : Pin Fault �߰�
				if( Sub71.Y1 > 7.0) //sylee150318-test 										
				{
					Beep(923, 20);

					CString boxStr; 
					boxStr.Format("%s", ((box+1) == (BOX_A+1)) ? "UPPER_A":
										((box+1) == (BOX_A+2)) ? "UPPER_B":
										((box+1) == (BOX_A+3)) ? "UPPER_C":
										((box+1) == (BOX_A+4)) ? "UPPER_D": 
										((box+1) == (BOX_A+5)) ? "LOWER_A": 
										((box+1) == (BOX_A+6)) ? "LOWER_B": 
										((box+1) == (BOX_A+7)) ? "LOWER_C": 
										((box+1) == (BOX_A+8)) ? "LOWER_D": "UNDEF_BOX");

					// 8K�����̸� ��� box0 ��� (box1,2,3 skip), �Ϻ� box4 ��� 
			        if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN)
                    {
                        boxStr.Format("%s", ((box+1) == (BOX_A+1)) ? "UPPER_A":
                                            ((box+1) == (BOX_A+5)) ? "LOWER_A": "UNDEF_BOX"); 
                    }

					str.Format (" Unused, box=%s, hwPin=%5d", boxStr, hwPin+1); //son �ʱ�ȭ
					strLabel.Format("  Pin= ?(hwPin=%d) ", hwPin+1  ); 		    //son �ʱ�ȭ

					//------------------------------------------------------------------------
					//son ���� SwitchCard Pin�� �ش��ϴ� jigPin ���� ã�Ƽ� "str"���� �����. 
					//int jigPin = ModeSet12.make32kSwPin(hwPin);       //son210831 �ڸ�Ʈ�� �����ϰ� �������� �̵�.
					int jigPin = nJigPinMap[hwPin]; //son230303 //son230329  Unused ���̽��� pinSearch ����� �ǵ���
					if (jigPin != -1)                           //son230329  son210831 ������ �� ���󺹱� 
					{
						//str.Format (" %d " ,jigPin+1); 		//son listBox ��¿�									 
						str.Format(" %9d, box=%s, hwPin=%5d", jigPin+1, boxStr, hwPin+1); 							 
						strLabel.Format(" %d " ,jigPin+1); 		//son PIN Search Label  ��¿�
					}
					//------------------------------------------------------------------------

					//son Fault Pin�� ���� 'PinSearch' �󺧿� ���
					m_LabelFault.SetCaption(strLabel);		//son-32KPIN-20190902: m_Label101->m_LabelFault �� ����

#if  0
					int index = m_list1.FindString(-1, str);	// ������ ���� pin �̶��

					//son list box����  �ش� pin �߰� 
					if(index == LB_ERR)
						m_list1.AddString(str); 			 		 
#else
					//son �ϴ� fault detect �� �Ŵ� ����Ʈ�� �ִ��� ����ϴ� ���� �´� ������ �ǴܵǾ� 
					//    �Ʒ��� ���� ������ ����ϵ��� ������ : 2019.10.04
					//son list box���� �ش� pin �߰� 
					m_list1.AddString(str); 			 		 
#endif
					m_list1.SetTopIndex(m_list1.GetCount()-1); 
					nlist1++;

					// �ʹ� ���� ���� ��µǸ� ȭ���� �ٿ�Ǵ� ���� �����ؼ� 30�� �̻� ��µ��� �ʵ��� ��.
					if( nlist1>30){
						nlist1=0;
						m_list1.ResetContent();
						m_list1.AddString(str); 			 		 
						m_list1.SetTopIndex(m_list1.GetCount()-1); 
						nlist1++;
					}							 

					Sub71.OnPinAllReset1(); ///PIN ALL RESET  
					Sleep(100);//sylee150318-test
				}

				Sub71.OnPinAllReset1(); ///PIN ALL RESET 


			}	// end of hwPin

		}	// end of box

	} // while


	// �Ǵ� 
	if( nPinSearchFlag==3){  
		OnEnd101();
	}


	return 1;
}


void CModeMan14::OnTimer(UINT nIDEvent) //SYLEE20120706
{
	// TODO: Add your message handler code here and/or call default
 
//	int nCurStates;
//	int nDio[8]; 
	


	switch(nIDEvent) 
	{


		case 70:
					

#if 0
			if( nStartFlag1==0){

				nCurStates = m_ComiDaq3.DiGetAll(); 
				for( int i=0; i<8; i++) {

					nDio[i] = (nCurStates >> i) & 0x1;


					if( nDio[0] == 0 ){     // start off�� 
						nStartDI1=0; 
					}
					if( nDio[1] == 0 ){     // test off��    

					}
					if( nDio[2] == 0 ){     // abort off��  

					}

					if( nDio[3] == 0 ){     // skip off��    

					}


					if(nStartDI1==0){ 
						if( nDio[1] == 1 ){ //    /remote start  on
							nStartFlag1=1;
							nStartDI1=1;
						}
					}
				}
			}

#endif

			break;


//		default:
//			break;
	}	
 
	
	CDialog::OnTimer(nIDEvent);


}




//son-32KPIN-20191226 :  ���� �̻��.
int CModeMan14::OnRun101_test() //run//sylee20120705
{

	FILE *fp;//sylee20120706
	CString str,str1, strLabel;

	int nTimeDelay1,nCo1, nlist1, nNetPin ;
	int i, oldPin,StartPin,EndPin,fileCo,nMok,nFlag,j2,StartPinDef;

	int nPinSearchSetup[50][3];

	long  nCount;  

	char  fName[200];//sylee20120706


	fileCo=0; 
	nStartFlag1=0;

	str.Format("RUN"); 
	m_LabelStatus.SetCaption(str);		//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus �� ����

	strLabel.Format("                        "); 									 
	m_LabelFault.SetCaption(strLabel);		//son-32KPIN-20190902: m_Label101->m_LabelFault �� ����


	::ZeroMemory(&nPinSearchSetup,sizeof(nPinSearchSetup));
	nCo1=0;
	nlist1=0;
	nCount=0;


	::ZeroMemory(&fName, sizeof(fName));   //sylee20120706                
	CTime 	curTime = CTime::GetCurrentTime();				 
	//	str.Format("d:\\PinSearch_%2dH%2dM.txt",curTime.GetHour(), curTime.GetMinute() );	


	str.Format("d:\\PinSearch_%2dD%2dH.txt", curTime.GetDay(),curTime.GetHour()  );
	strcat( fName , str ); 		  
	fp = fopen(fName,"wt"); 
	if(fp == NULL){ 
		AfxMessageBox(" Error 7506, \n\n    hvleak file close.\n\n     hvleak file open   ?  Error, Saving Error !     file close!", MB_OK); //error messege
		return 0 ;
	} 	 


	if( SysSet12.m_nPinTotMax== PIN_TOT_4K){ //4k:1	//son-32KPIN-20190902: enum ���� ����
		nPinSearchSetup[0][0]=1; 
	}
	else   if( SysSet12.m_nPinTotMax== PIN_TOT_8K){ //8k:2
		nPinSearchSetup[0][0]=2; 
	}
	else  if( SysSet12.m_nPinTotMax== PIN_TOT_16K){ //16k:3
		nPinSearchSetup[0][0]=4; 
	}
	else  if( SysSet12.m_nPinTotMax== PIN_TOT_32K_TRANS //32k:4 //son230216_1
	          || SysSet12.m_nPinTotMax== PIN_TOT_32K_U8K_L24K //son230224_2
	          || SysSet12.m_nPinTotMax== PIN_TOT_32K_ORG)     //son230224_2
	{
		nPinSearchSetup[0][0]=8;//32k
	}

	for(   i=1; i<=8;  i++){  //32k
		nPinSearchSetup[i][1]=4096*(i-1);  // start no 		   
		nPinSearchSetup[i][2]=4096*i-1;   //end no 
	}

	i=nPinSearchSetup[0][0];

	StartPinDef=(4096*4-1); 
	StartPin=StartPinDef; 
	EndPin=4096*8;
	nFlag=0;
	oldPin=0;

	nTimeDelay1=20;//200;
	Sub71.AOnPinSearch(1);



	while( nPinSearchFlag==1)
	{  

		DoEvents(); 
		Sub71.Delay_100ns(30);  

		if(nStartFlag1==1)
		{	  

			for( int j=StartPin; j<=EndPin; j++)
			{    //real	

				if( j <StartPinDef){// if( StartPin <StartPinDef){
					j2=(5-(StartPinDef-j))+oldPin;
				}
				else{//  if( StartPin==StartPinDef){//StartPinDef=(4096*4-1)-210; 							  
					j2=j;
				}

				j2=j;

				//Sub71.AOnPinSet1(1 , j2 , 2);  //   nMod  =2:VB,   pin ����   //sylee200709-1-JTECH  (VB�� 2�� �ٲ�)
				Sub71.AOnPinSet1(1 , j2 , MOD_TR_SET_VB);  //   nMod  =2:VB,   pin ����  //son210220: MOD_TR_SET_VB�� ���� 

				//test	 Sub71.Delay_100ns(50);//100ns(30) 16K   5V 500uA  20120703					 
				Sub71.Delay_100ns(20);//100ns(30) 16K   5V 500uA  20120703					 
				Sub71.AOnReadV1I1(1);

				if( Sub71.Y1> 5){ //sylee20120703

					if(Sub71.Y1<= 7){//32k //sylee20120703
						Sub71.Delay_100ns(20);
						Sub71.AOnReadV1I1(1);							 
					}


					if( Sub71.Y1> 7){


						StartPin=StartPinDef-10;							

						Beep(923, 100);

						nCo1=0;
						nNetPin=0;
						nMok=j/4096+1; 



						if(j>200){
							StartPin=j-200;
						}
						else{
							StartPin=0;
						}


						for(int ik=nPinSearchSetup[nMok][1]; ik<=nPinSearchSetup[nMok][2]; ik++)
						{
							if(PinMap[2][ik]==j)
							{
								nNetPin=ik;
								// str.Format(" Net=%d, QD=%d, Switch=%d, i=,%.2f\n",PinMap[0][nNetPin]+1,PinMap[1][nNetPin]+1,j+1,Sub71.Y1 ); '

								fprintf(fp," %dH,%dM,   *Net=%d, QD=%d, Switch=%d, i=,%.2f  \n", curTime.GetHour(), curTime.GetMinute(),PinMap[0][nNetPin]+1,PinMap[1][nNetPin]+1,j+1,Sub71.Y1  );

								oldPin=j; 


								fileCo++;
								if( fileCo>100){
									fileCo=0;  	  											   											   
									fclose(fp);

									::ZeroMemory(&fName, sizeof(fName));   //sylee20120706                
									CTime 	curTime = CTime::GetCurrentTime();				 
									str.Format("d:\\PinSearch_%2dD%2dH.txt", curTime.GetDay(),curTime.GetHour()  );			 
									strcat( fName , str ); 		  

									fp = fopen(fName,"at+"); 
									if(fp == NULL){ 
										AfxMessageBox(" Error7507   \n\n    hvleak file close.\n\n hvleak file open   ?  Error, Saving Error !     file close!", MB_OK); //error messege
										return 0 ;
									} 
								}



								str.Format(" %d\n",PinMap[0][nNetPin]+1 );    //sylee131205 

								strLabel.Format(" %d " ,PinMap[0][nNetPin]+1 );     //sylee131205 
								break;
							} 
						}


						m_LabelFault.SetCaption(strLabel);		//son-32KPIN-20190902: m_Label101->m_LabelFault �� ����
						m_list1.AddString(str); 			 		 
						m_list1.SetTopIndex(m_list1.GetCount()-1); 
						nlist1++;

						if( nlist1>30){
							nlist1=0;
							m_list1.ResetContent();
							m_list1.AddString(str); 			 		 
							m_list1.SetTopIndex(m_list1.GetCount()-1); 
							nlist1++;
						}


					}//if( Sub71.Y1> 300){

					Sub71.OnPinAllReset1(); ///PIN ALL RESET 
					Sub71.AOnPinSearch(0);		  
					Sub71.AOnPinSearch(1);

					nStartFlag1=0; 
					nCo1=0;

					goto Task11;

				}
				else{  // if( Sub71.Y1> 100){ //sylee20120703
					Sub71.OnPinAllReset1();  
				}


			}//end of for( int j=StartPin; j<=(StartPin+400); j++){  




Task11:			   										 



			if( j> EndPin )
			{

				StartPin=StartPinDef-10; 

				Beep(623,300);
				DoEvents();

				fprintf(fp," %dH,%dM,   ****> OPEN TIME OUT   \n", curTime.GetHour(), curTime.GetMinute()  );

				fclose(fp);

				::ZeroMemory(&fName, sizeof(fName));   //sylee20120706                
				CTime 	curTime = CTime::GetCurrentTime();				 
				str.Format("d:\\PinSearch_%2dH.txt",curTime.GetHour() );			 
				strcat( fName , str ); 		  
				//fp = fopen(fName,"wt"); 
				fp = fopen(fName,"at+"); 
				if(fp == NULL){ 
					AfxMessageBox("  Error 7509.  \n\n    hvleak file close.\n\n  hvleak file open   ?  Error, Saving Error !     file close!", MB_OK); //error messege
					return 0 ;
				}

				str.Format("OPEN  TIME OUT"); 

				m_list1.AddString(str); 			 		 
				m_list1.SetTopIndex(m_list1.GetCount()-1); 
				nlist1++;


				nStartFlag1=0; 
			}




		}//if(nStartFlag1==1){


	}//while



	if( nPinSearchFlag==3){  
		OnEnd101();
	}

	fclose(fp);//sylee20120706	
	KillTimer(70);//sylee20120706	

	nStartFlag1=0;


	return 1;
}




//################################################################################################################


int CModeMan14::OnEnd101() //End
{
	 
	CString str;
 
    Sub71.AOnPoweroff2();//sylee20120702   Sub71.AOnPoweroff();

	ErrorPinDis();//sylee130310
	   
	str.Format("END"); 
	m_LabelStatus.SetCaption(str);	//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus �� ����
 
  
  	return 1;
}



int CModeMan14::OnStop101() //stop
{
 

 	//	 if( nStop==2){
	//			AfxMessageBox("   R Calibraion   STop!!!"); 	 
	//	 }
 
  

  	return 1;
}

int CModeMan14::OnSet() //stop
{

	CString   str,str1;
	CString   strfName,strfName1;
 
// 	int  nTimeDelay1;
//	int nISet,nTimeDelay1;
	int nRet1;
	int	Lo1;
	double dVbR1,dVbR2;
 
 
 

	nCC=   1;
	nVRel= 2;  //sylee20120617 
	nVSet= 10 ;
	nIR=   2; //SYLEE130308
	nISet= 10 ;//10mA  //SYLEE130308
	Sub71.nADCount1 =1;
 

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
	//INITAL �� 


	str.Format("POWER SET"); 
	m_LabelStatus.SetCaption(str);	//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus �� ����

	nRet1=Sub71.AOnPoweroff2();//sylee20120702 Sub71.AOnPoweroff();
	if(nRet1!=1){ 
		AfxMessageBox(" Error No 1023, \n   Power all off fail !, \n  PIN SEARCH :Run Start Error!!!"); 
		return 0;
	}

	nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;//sylee130518

	if( nRet1!=1){ 
		AfxMessageBox(" Error No 1023, \n   V SET ERROR !,  \n  PIN SEARCH :  Run Start Error!!!"); 
		return 0;
	} 
	Sub71.Delay_100ns(3000000);  //300mSECOND

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
	//������ 	 


	Lo1=50;

Task101:  
	
	Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

	if(nVSet<5){
	    dVbR2=nVSet*(0.35);//35%
	}
	else{
	    dVbR2=nVSet*(0.1);//10%
	}
	dVbR1=nVSet-dVbR2;
	dVbR2=nVSet+dVbR2;
	
	
	if( Sub71.V1 < dVbR1 || Sub71.V1 > dVbR2)
	{
	    if(Lo1-- > 0){
	 	   Sub71.Delay_100ns(1000000);  //300ms   
	 	   goto Task101;
	    }
	
	    str.Format("Error No 14023,\n \n\n  Power Check? \n\n\n    Pinsearch  \n\n VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),", nVSet,Sub71.V1,dVbR1,dVbR2); 
	    AfxMessageBox(str);	
	    //	return ;
	}
	
	
	
	Sub71.FilterSet(1);//sylee130803
	Sub71.I_Filter(1);//sylee130803		 
	
	Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet); 	   
	
	Sub71.AOnReadV1Enable1(nVRel,1);  //VC SENSE INIT SET//sylee150318  ,1
	
	Sub71.OnPinAllReset1(); ///PIN ALL RESET  		   
	
	return 1;
}




 

int x1,x2,x3,y11,y2,y3;

void CModeMan14::OnCheck1()  
{

	//son BDL check box On �϶�. PinMap ������ pin�� Ž��
	if(m_checkBdl.GetCheck())//sylee130518
	{
		nPinSearchAll=0;//sylee130518  //sylee170523- ray
	}

	//son BDL check box Off �϶�. ALL Pin Ž�� 
	else{
		nPinSearchAll=1;
	}


	if(nPinSearchAll!=1) //sylee130518
	{
		if(nTotalPin>3000){
			nTotalCoSet=1000;
		}
		else  if(nTotalPin>2000){
			nTotalCoSet=2000;
		}
		else  if(nTotalPin>1000){
			nTotalCoSet=3000;
		}
		else  if(nTotalPin>500){
			nTotalCoSet=5000;
		}
		else  if(nTotalPin>100){
			nTotalCoSet=10000;
		}
		else  if(nTotalPin>50){
			nTotalCoSet=30000;
		}
		else {
			nTotalCoSet=50000;
		} 
	}
	else {
		nTotalCoSet=1000;
	}



}




BOOL CModeMan14::OnInitDialog()
{
 	CDialog::OnInitDialog();

	MoveWindow(230,190,700,500);
//	nPinSearchAll=0;//sylee130518 
    nPinSearchAll=1;   //sylee170525 

    //son 2019.11.15: ???  �� �κ� OnTimer���� ���źκ��� ���� ����. 
    //                ���ʿ��ϰ� ȣ���ϴ� ������ �Ǵ��ϰ� �Ʒ��� ���� �ڸ�Ʈ�� ����
	//SetTimer(70, 10, NULL);
	
	ErrorPinDis();

	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928

	A_Language() ;
  
	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CModeMan14::ErrorPinDis()
{

	CFile file;

	if ((file.Open("Z:\\error.txt", CFile::modeRead))==NULL) {
		//AfxMessageBox("  Error No 1401,   file Reading Error.      file Check!.   C\\bioptro\\error.txt ", MB_OK);
		// SetDlgItemText(IDC_DATA, "file check! =>C\\bioptro\\error.txt."   );
		return;
	}

	char buffer[65535];
	::ZeroMemory(buffer, sizeof(buffer));  
	file.Read(buffer, 65534);	
	file.Close();
	SetDlgItemText(IDC_DATA, buffer);

}
