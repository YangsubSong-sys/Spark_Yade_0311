// ModeMan11.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeMan11.h"
#include "Contec.h"
#include "ModeSet17.h"
#include "ModeSub71.h"
#include "FileSysInfo.h"
#include "ChildView7.h"
//#include "ChildView7.h"
#include "FileSysInfo.h"
#include "A_Define1.h"//sylee161126
#include "Globals.h"  //son220412
 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int nJigTestOpen,nJigTestShort;//sylee181008-1
 
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

extern short nPinSetSkipVb; //sylee151005 //ACE400	//son230127
extern short nPinSetSkipVc; //sylee151005 //ACE400	//son230127

int nShortCount;//sylee150811
extern int nlist1;//sylee150811
CChildView7  dView7;  //sylee131102

  

extern CFileSysInfo  FileSysInfo01;//SYLEE130618 
extern CSysSet12  SysSet12;  //Pin SETUP     
//extern CChildView7  View7;//sylee131102
extern CSysInfo05  SysInfo05;
extern CModeSub71  Sub71;

extern int nMCon3,	nMCon2, nMCon3, nMCon4;
extern int nAutoProcess1;
extern int g_nPieceMax1; 
extern int nFailL1Co;
extern int nStop;
extern int nRsModeR1S;

//extern short nBDLData3[DefDutMax][DEF_MAX_PIN1][10];//short data   //sylee180906  1-> DefDutMax
extern short nBDLData3[MAX_DUT1][DEF_MAX_PIN1][10];//short data   //son220824_2 DefDutMax -> MAX_DUT1(33)

//extern short nBDLData31[DefDutMax][DEF_MAX_PIECE1][DEF_MAX_PIN2];//short data   //sylee180906  1-> DefDutMax
extern short nBDLData31[MAX_DUT1][DEF_MAX_PIECE1][DEF_MAX_PIN2];//short data   //son220824_2 DefDutMax -> MAX_DUT1(33)  //son220823_6

extern int nFailCount[TEST_MAX];        //son231109 15 -> TEST_MAX
//extern int nBDLHead1[DefDutMax][5][20];
extern int nBDLHead1[MAX_DUT1][5][20]; //son220824_2 DefDutMax -> MAX_DUT1(33)

//extern short nBDLData1[DefDutMax][dSetPinTot1][10];//raw data 
extern short nBDLData1[MAX_DUT1][DEF_MAX_PIN1][10];//raw data //son220824_2 DefDutMax -> MAX_DUT1(33)

extern int PinData[DEF_MAX_PIN1][10];   //file reading
extern int PinDataP1[DEF_MAX_PIN1][10];  //piece sort
extern int PinDataP2[300][3]; // piece header
extern int PinDS1[DEF_MAX_PIN1][5]; 
extern int nPcb1[5000][2]; //PCB NO , PIN NO
extern int PinDS1R1[DEF_MAX_PIN1];
extern double nCalC[30][10][100];
extern double nFailL1[DEF_MAX_PIN1][20];  //[10]=1,2= pinno, 
extern double dRec[362][16];

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

/////////////////////////////////////////////////////////////////////////////
// CModeMan11 dialog

CModeMan11::CModeMan11(CWnd* pParent /*=NULL*/)
	: CDialog(CModeMan11::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeMan11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeMan11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeMan11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LIST1, m_list1);
		DDX_Control(pDX, IDC_COMBO101, m_combo101);
		DDX_Control(pDX, IDC_LABEL1001, m_Label1001);

	 	DDX_Control(pDX, IDC_LABEL1,  m_Label1); 
		DDX_Control(pDX, IDC_LABEL2,  m_Label2); 
		DDX_Control(pDX, IDC_LABEL3,  m_Label3); 
 
		DDX_Control(pDX, IDC_BUTTON1001, m_button1001); 
		DDX_Control(pDX, IDC_BUTTON103, m_button103); 
		DDX_Control(pDX, IDC_OK, m_buttonOk); 


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeMan11, CDialog)
	//{{AFX_MSG_MAP(CModeMan11)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

 
BEGIN_EVENTSINK_MAP(CModeMan11, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeMan11)
 	ON_EVENT(CModeMan11, IDC_BUTTON1001, -600 /* Click */, OnClickButton101, VTS_NONE)
 	ON_EVENT(CModeMan11, IDC_BUTTON103, -600 /* Click */, OnClickButton103, VTS_NONE)
	ON_EVENT(CModeMan11, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void CModeMan11::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
	  m_Label1.SetWindowText(LoadMessage(1,"TitleSetupShort", 101));
 	  m_Label2.SetWindowText(LoadMessage(1,"TitleSetupShort", 102));
	  m_Label3.SetWindowText(LoadMessage(1,"TitleSetupShort", 103));
	  m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupShort", 104));

	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupShort", 105));
	  m_button103.SetWindowText(LoadMessage(1,"TitleSetupShort", 106));
 	  m_buttonOk.SetWindowText(LoadMessage(1,"TitleSetupShort", 107));
 
  }

}


/////////////////////////////////////////////////////////////////////////////
// CModeMan11 message handlers

BOOL CModeMan11::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_combo101.SetCurSel(1);//sylee130605
	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928
	A_Language();   
	return TRUE;       
}

 BOOL CModeMan11::PreTranslateMessage(MSG* pMsg) 
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
 
		nStop2=2;
		return TRUE;
	}
 
	return CDialog::PreTranslateMessage(pMsg);
}


void CModeMan11::OnClickButton101() 
{
	nPinSetSkipVb=0; //sylee151007 //ACE400 
	nPinSetSkipVc=0; //sylee151007 //ACE400

     OnManualShort1();	
}



void CModeMan11::OnClickButton103() 
{ 
      nStop2=2;
}


void CModeMan11::OnClickOk() 
{ 
     nStop2=2;
	CDialog::OnOK(); 
	
}



int CModeMan11::OnManualShort1() //sylee131023
{
 
	int nRet1;
 
	nRet1=Sub71.AOnPoweroff2();  //SYLEE20120702 AOnPoweroff();
 	if(nRet1!=1){
		AfxMessageBox(" Error No 11025, \n   Power all off fail !,   MANUAL SHORT    Start Error!!!"); 
		return 0 ;
	}

	 nRet1=FileSysInfo01.LoadSaveSer13(_LOAD, 207, PART_MIXED); //son220412
	 nRet1= (int) dRec[207][3];//SYLEE180307-2
	 if((nRet1>20)||(nRet1<=10)){//SYLEE180307-2
		 nRet1=10;//SYLEE180307-2
	 }
	//SYLEE180307-2 nRet1=Sub71.AOnVSetMulti1(10,10,200,0) ;
	 nRet1=Sub71.AOnVSetMulti1(nRet1,10,200,0) ;//SYLEE180307-2  
	if( nRet1!=1){
		AfxMessageBox(" Error No 11026, \n   V SET ERROR !,    MANUAL SHORT    Start Error!!!"); 
		return 0;
	}

	nShortCount=0;//sylee150811 
    OnManualShort1_LV();


	if(nShortCount<1){//sylee150811
        OnManualShort1_HV(); //sylee150811
	}

	nJigTestShort=2;//sylee181008-1
	return 1;
}//end of CChildView3::OnManualShort1(){//sylee131023



int CModeMan11::OnManualShort1_LV() //sylee131023 
{

    CString   str,str1,strfName,strfName1;
    char  fName[200], fName1[200]; 
    int i, ik,j , n4Wire, nRCount,nDelay10,nEnd;	
    int nLoop, ipEnd, ipFlag,Lo1,nECount,nEtemp1,nShort2FailFlag1;
    int nEt1,nFailL1CoPiece,nCDelay1,nFil,nDisChargeADC1;
    int nHVIADCFlag, nDischargeCount ;
    int nShRes1T, ih,logno1,nSQCount1, nSQLastY1, nSQLastPin, tp1 ;
    double nISet,dVbR1,dVbR2,InterI;	
    int nFailFlag1,nFailFlag2,nFailFlag3,nFailFlag4; 
    // 	short nShRes1[33000];
    int nSQ1Fail[200];//, nSQPinCheck[34000];
    int nSh1Ret1[100],dPinVC[200],dPinVB[200],nSQNet1[200][200];// pin array//sylee1209
    double nT[10],nData[130][15], dR1[11][200];
    double dVB1[11][200],dVC1[11][200],dI1[11][200]; 
    //	double nCardOffSetHV1;//sylee120915  		
    int nMSet1Flag,nMSet1Co, nMSet1CoT2;
    int nSQStep1flag,nIControl,nRetry1,nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
    int nCC,  nVRel, nVSet,nIR , m_nMode1,nMod1;
    double nProRv1; 
    int nBDLPin1[DEF_MAX_PIN1],nBDLPin2[DEF_MAX_PIN1];
    int nIGain;//sylee240517

    //sylee150811  int nlist1;

    nlist1=0;
    nFailL1Co=0;//SYLEE140110    

    m_list1.ResetContent();

    str.Format("RUN"); 
    m_Label1001.SetCaption(str);

    /////////////////////////////////////
    //net list load
    // nBDLHead1[1][3][1]  &nBDLData1[1][i][j]);

    int k1,k2,i1,nMaxPin1;

    k2=nBDLHead1[1][3][1];
    // ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
    // ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));

    k1=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
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
    else{ // 20K, 24K
        nMaxPin1=4096*8;
    }

    //int *nBDLPin1 = new int[33000];
    //sylee161109-1	int *nBDLPin1 = new int[nMaxPin1+100];

    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;

    str.Format("  ***> Manual LV Short :  START  *******  ");			 
    m_list1.AddString(str); 
    nlist1++;

    DoEvents();  

    //sylee161109-1	 int *nBDLPin2 = new int[nMaxPin1+1];
    // for(  i1=0 ; i1< (nMaxPin1+100) ; i1++){		 //sylee150811   1->0
    //	   nBDLPin1[i1]=0;
    //}	 

    //for(  i1=0 ; i1<= nMaxPin1 ; i1++){  //sylee150811   1->0
    //	   nBDLPin2[i1]=0;		   
    //}
    ::ZeroMemory(&nBDLPin1,sizeof(nBDLPin1));//sylee190104
    ::ZeroMemory(&nBDLPin2,sizeof(nBDLPin2));//sylee190104

    for(  i1=1 ; i1<= k2 ; i1++){		 
        k1=nBDLData1[1][i1][1];
        nBDLPin1[k1]=1;	  
    }

    k1=0;
    for(  i1=0 ; i1<=nMaxPin1 ; i1++){		 //sylee150811   1->0
        if( nBDLPin1[i1]==1){
            nBDLPin1[0]=nBDLPin1[0]+1;
            k1++;
            nBDLPin2[k1]=i1;
        }		    
    }
    nBDLPin2[0]=k1;

    //////////////////////////////////////

    int nRet;

    nMod1=1;
    m_nMode1=207;
    Sub71.R1=3000000; 

    nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412

    if( nRet!=1){
        str.Format("ERRor NO 3002,   \n  HR Short Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
        AfxMessageBox( str);
        return 0; 
    }

    RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER );   // m_nMode1 1~76  //son220412_3 
    nProRv1=Sub71.R1;

    if(nProRv1<350){
        nProRv1=350;
    }

    if(nProRv1>1000){
        nProRv1=1000;
    }

    //////////////////////////////////////
    //nProRv1=400;//raw

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
    nIGain = (int)dRec[m_nMode1][12];   //SYLEE230517     

    if( nISet<0.1){
        AfxMessageBox(" Error No 7713, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
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
        AfxMessageBox(" Error No 1024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }

    if( nVSet<1 || nVSet>300 ){
        AfxMessageBox(" Error No 1025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }

    if( nVRel<1  || nVRel>4 ){ 
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        str.Format("Error"); 
        m_Label1001.SetCaption(str); 
        return 0; 
    }

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //설정부 


    //sylee130903	Lo1=200;

    //sylee150708 Lo1=100;//sylee130903  200nS-> 1sec
    Lo1=1000;//sylee150708
    Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

Task101:    

    Sub71.AOnReadVB1Run1(nVRel); 

    if(nVSet<=5){
        dVbR2=nVSet*(0.35);//10%
    }else{
        dVbR2=nVSet*(0.1);//5%
    }
    dVbR1=nVSet-dVbR2;
    dVbR2=nVSet+dVbR2;

    if( Sub71.V1>300){
        if(Lo1-->0){					 					
            //sylee121019	Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(30000);  //1ms   //sylee150708  1->3ms 
            goto Task101;
        }
        str.Format("Error No 1301,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
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


        str.Format("Error No 1317,\n \n\n Voltage set error.    Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str);						  
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }

 
 
    Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 

    Sub71.FilterSet(nFil);//sylee130702 
    if(nIFilter==3){
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
        nIFilter=1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629
    if (nIGain == 2){ //SYLEE240517    
       Sub71.I_Gain(2);  //10x  =io on        
    } else {
        Sub71.I_Gain(1);  //1x  =io off
    }

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


    if( nVSet>20){

        nLoop=1;
        str.Format("Error No 1318,\n \n\n  MANUAL SHORT START ERROR    VSET >20V !!    "); 
        AfxMessageBox(str);
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }else{
        nLoop=0;
    }



    ipFlag=2;
    ipEnd=1;
    nStop2=0;


    FileSysInfo01.LoadSaveSub15(2);//SYLEE140110

    /////////////////////////////////////////////////////////////////////
		
  
 
    //====================================================================================================================================
    for( int ip=1; ip<=ipEnd; ip++ )
    { 
     
        if(nRCount<=0){
            //ERROR DEBUG   PIECE=ip NO LIST.
            continue;//sylee20120402
        }

        nEtemp1=0;	nECount=0; 	nFailL1CoPiece=0;
        nEt1=0;	nShort2FailFlag1=0;

        //*******************************************************************************************************************************//
     //Task1:
    /*
        if(ipFlag==1){
             nRCount=nBDLData31[1][ip][0]; 
        }else{
             nRCount=nBDLData3[1][0][1];
        } 
      */

        nRCount=nBDLPin2[0];

        nSQCount1=0;//sylee120908   //SQRT( SQUARE ROOT)
        for( i=1; i<200;i++){
            tp1=i*i;
            if(tp1>=nRCount){
                nSQCount1=i;//sylee120908
                break;
            }
        }

        if( (nSQCount1==0)  || (nRCount<1)){//sylee121018
                str.Format("Error No 7513-6 ,   SET ERORR   Net List <1,  ,    piece pin no set check?   HV SQUARE  COUNT = ZERO   n"   ); 
                AfxMessageBox(str);	
                str.Format("Error"); 
                m_Label1001.SetCaption(str);
                return 0;
        }

         ::ZeroMemory(&nSQNet1, sizeof(nSQNet1));

     //################################################################################################################################
    //SET 

     
         for( ik=1; ik<=nRCount; ik++){
             i=(ik-1)/nSQCount1  + 1;//mok
             j=(ik-1)-nSQCount1*(i-1);//na
            // if(i>1){
             j=j+1;
            // }					 
     /*
             if( ipFlag==1 ){//short   PIECE MULT.
                   nSQNet1[i][j]=nBDLData31[1][ip][ik] ; // 1 VB,    =2VC, 
             }else{						 
                   nSQNet1[i][j]=nBDLData3[1][ik][1] ;   // 1  VB,    =2VC, 	 
             } 
    */

            //sylee150811	 nSQNet1[i][j]=nBDLPin2[ik]-1;//SYLEE140110  nBDLPin2[ik]-1		 
             nSQNet1[i][j]=nBDLPin2[ik];//SYLEE150811

             if( ik==nRCount){
                  nSQLastPin=j;//sylee120908   ///last row , last col.
                  nSQLastY1=i;
             }
         }

     
    //################################################################################################################################
    //#1 FIRST-1  Line&Block
    //row

        ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));	
        nShort2FailFlag1=0;
        nSQStep1flag=0;

        Sub71.OnPinAllReset1(); ///PIN ALL RESET 
         
        nRetry1=0;


    //Task1Retry:

     
        for( ik=1; ik<nSQLastY1; ik++)     // FIRST LOOP //Y  VB 
        {

            DoEvents(); 

            if( nStop2==2){
                str.Format(" *****  STOP  *******");			 		
                m_list1.AddString(str); 
                nlist1++;
                goto Task211;
            }

            if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                break;// complete  error 												 
            }

            if(ik==1  || nSQStep1flag==2){

                Sub71.OnPinAllReset1(); ///PIN ALL RESET 

                for( int ik2=1; ik2<=nSQLastY1; ik2++){
                    if(ik2>ik){
                        continue;
                    }
                    for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                        if( ik2==nSQLastY1){  //LAST Y STEP
                            if(ih>nSQLastPin){
                                break;//SKIP   
                            }
                        }
                        //Sub71.AOnPinSet1(2, nSQNet1[ik2][ih], 1); //=1Vb,    
                        Sub71.AOnPinSet1(2, nSQNet1[ik2][ih], MOD_TR_SET_VB); //=1Vb,    //son210220 enum으로 변경
                    } 
                }

                nSQStep1flag=0;


            }else{//end of if(ik==1  || nSQStep1flag==2){			   
                Sub71.OnPinResetVC();  
                for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                    if( ik==nSQLastY1){  //LAST Y STEP
                        if(ih>nSQLastPin){
                            break;//SKIP   
                        }
                    }				   
                    //Sub71.AOnPinSet1(2, nSQNet1[ik][ih], 1); //=1  vb 
                    Sub71.AOnPinSet1(2, nSQNet1[ik][ih], MOD_TR_SET_VB); //=1  vb //son210220 enum으로 변경
                }
            }

            for( ih=1; ih<=nSQCount1; ih++){  //X   // Second LOOP   vb pin  
                if( (ik+1)==nSQLastY1){
                    if(ih>nSQLastPin){
                        break;//SKIP
                    }
                } 			   
                //Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih], 2);   // =2,Vc,   all	
                Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih], MOD_TR_SET_VC);   // =2,Vc,   all	//son210220 enum으로 변경
            }
            //////////////////////////////////////////////////////////////////////////////////


            if( nHVIADCFlag==1 ){
                // dView7.AHV_ADCI_Read1(nProRv1); 
                dView7.ALV_ADCI_Read1_OneToM(nProRv1, nDelay10); 
                //  Sub71.R1= dIADC1  ;//ADC
            }else{		
                Sub71.Delay_100ns(nDelay10);
                dView7.AHV_ADCI_Read1(nProRv1); 
            } 

            if( nHVIADCFlag==1){   									    
                if( Sub71.R1<=nProRv1){// ok//HV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1; 	   		 
                    nSQStep1flag=2;
                }
            }
            else{ 
                if( Sub71.R1>=nProRv1){//ok//LV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail  
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1;
                    //	 nFailFlag1=0;//test
                    nSQStep1flag=2;
                }

            } 

            //==============================================================================================================================
            //################################################################################################################################
            //#2,  second search    hv  adc  
            // vc  m block 

            DoEvents(); 

            if(nFailFlag1==1)  //if error	 
            {
                DoEvents(); 
                if( nStop2==2){
                    str.Format(" *****  STOP  *******");			 		
                    m_list1.AddString(str); 
                    nlist1++;
                    goto Task211;					

                }



                Sub71.OnPinResetVB();   //sylee121029  

                for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc				
                {
                    nMSet1Co=0;
                    nMSet1Flag=0;
                    nMSet1CoT2=0;

                    if(ik2>ik){ //doublecheck#2 
                        continue;
                    }

                    for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                        if(ik2==nSQLastY1){  //LAST Y STEP
                            if(ih>nSQLastPin){
                                break;//SKIP   
                            }
                        }					   
                        //Sub71.AOnPinSet1(2, nSQNet1[ik2][ih], 1); //=2VC,	 
                        Sub71.AOnPinSet1(2, nSQNet1[ik2][ih], MOD_TR_SET_VB); //=2VC,	 //son210220 enum으로 변경
                    } 
                    //=====================================================================================================================
                    if( nHVIADCFlag==1 ){
                        // dView7.AHV_ADCI_Read1(nProRv1); 
                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                        //  Sub71.R1= dIADC1  ;//ADC
                    }else{							
                        Sub71.Delay_100ns(nDelay10); 

                        if( nCC==2 ){//cv
                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                        }else{// cc										 
                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                        }

                        if(Sub71.Y1<0.0000001){
                            Sub71.Y1=0.0000001;
                        }

                        if(nCC==1){ //CC    r= (vb-vc)/i
                            if( nIR==1 ||  nIR==2 ){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                            }          

                        }else if(nCC==2){//CV   r=(vb/i)
                            if( nIR==1 ||  nIR==2){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                            } 
                        }

                    }


                    //Sub71.Delay_100ns(10000);  
                    //=====================================================================================================================
                    nFailFlag2=0;
                    if( nHVIADCFlag==1){   									    
                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }else{ 
                        if( Sub71.R1>=nProRv1){//ok//LV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }//line & line 

                    //=====================================================================================================================
                    //##################################################################################################################################

                    //one line & one line  search


                    if(nFailFlag2==1)
                    {  

                        DoEvents(); 
                        if( nStop2==2){
                            str.Format(" *****  STOP  *******");			 		
                            m_list1.AddString(str); 
                            nlist1++;
                            goto Task211;					

                        }


                        for( int ih6=1; ih6<=nSQCount1; ih6++){    //X // Second LOOP   vc pin 

                            if( ik2==nSQLastY1){  //LAST Y STEP
                                if(ih6>nSQLastPin){
                                    break;//SKIP   
                                }
                            }

                            Sub71.OnPinResetVB();//SYLEE121016***** all reset								    
                            // Sub71.Delay_100ns(100);//10uSEC

                            //Sub71.AOnPinSet1(2, nSQNet1[ik2][ih6], 1); //=2VC,	 
                            Sub71.AOnPinSet1(2, nSQNet1[ik2][ih6], MOD_TR_SET_VB); //=2VC,	 //son210220 enum으로 변경

                            //===================================================================================================================== 
                            if( nHVIADCFlag==1 ){
                                dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                //  Sub71.R1= dIADC1  ;//ADC
                            }else{										

                                Sub71.Delay_100ns(nDelay10);

                                if( nCC==2 ){//cv
                                    Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                }else{// cc										 
                                    Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                }


                                if(Sub71.Y1<0.0000001){
                                    Sub71.Y1=0.0000001;
                                }


                                if(nCC==1){ //CC    r= (vb-vc)/i
                                    if( nIR==1 ||  nIR==2 ){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                    }          

                                }else if(nCC==2){//CV   r=(vb/i)
                                    if( nIR==1 ||  nIR==2){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                    } 
                                }

                            }



                            nFailFlag3=0;
                            if( nHVIADCFlag==1){   									    
                                if( Sub71.R1<=nProRv1){// ok//HV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }else{ 
                                if( Sub71.R1>=nProRv1){//ok//LV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }//line & line 


                            //=====================================================================================================================
                            //################################################################################################################################
                            ///one point  & one point search

                            if(nFailFlag3==1)
                            { 
                                DoEvents(); 
                                if( nStop2==2){
                                    str.Format(" *****  STOP  *******");			 		
                                    m_list1.AddString(str); 
                                    nlist1++;
                                    goto Task211;					

                                } 
                                for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  
                                {

                                    if( (ik+1)==nSQLastY1){
                                        if(ih2>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }

                                    Sub71.OnPinResetVC(); //Vb all reset
                                    // Sub71.Delay_100ns(100);//10uSEC   //SYLEE121017

                                    //Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all
                                    Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih2], MOD_TR_SET_VC);   // =1,VB,   all //son210220 enum으로 변경



                                    if( nHVIADCFlag==1 ){
                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                        //  Sub71.R1= dIADC1  ;//ADC
                                    }else{															
                                        Sub71.Delay_100ns(nDelay10); 
                                        if( nCC==2 ){//cv
                                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                        }else{// cc										 
                                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                        }


                                        if(Sub71.Y1<0.0000001){
                                            Sub71.Y1=0.0000001;
                                        }


                                        if(nCC==1){ //CC    r= (vb-vc)/i
                                            if( nIR==1 ||  nIR==2 ){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                            }          

                                        }else if(nCC==2){//CV   r=(vb/i)
                                            if( nIR==1 ||  nIR==2){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                            } 
                                        }

                                    }	

                                    nFailFlag4=0;
                                    if( nHVIADCFlag==1){   									    
                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        }else{//fail
                                            // 	if(ih2==ih6)
                                            nFailFlag4=1;														 

                                            for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                                if( (nFailL1[iq1][4]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][5]==(nSQNet1[ik2][ih6]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                                if( (nFailL1[iq1][5]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][4]==(nSQNet1[ik2][ih6]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                            }
                                        }
                                    }else{ 
                                        //if( (Sub71.R1>=nProRv1)&&( Sub71.Y1>5)){//ok//LV
                                        if( Sub71.Y1<5){//ok//LV
                                        }else{//fail
                                            //   if(ih2==ih6)
                                            nFailFlag4=1;
                                        }
                                    }  

                                    //##########################################################################################################################################3


                                    if(nFailFlag4==1){//sylee130111 


                                        if(nFailL1Co<3000){//array size nFailL1[

                                            nFailCount[nMod1]=nFailCount[nMod1]++; 
                                            nFailL1Co++;

                                            nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV
                                            if( nSQNet1[ik+1][ih2]<nSQNet1[ik2][ih6]){
                                                nFailL1[nFailL1Co][5]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][4]=nSQNet1[ik2][ih6]+1;//pin  vc
                                            }else{
                                                nFailL1[nFailL1Co][4]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][5]=nSQNet1[ik2][ih6]+1;//pin  vc
                                            }  																		 

                                        }   //end of if(nFailL1Co<3000)//array size nFailL1[


                                        Sub71.Delay_100ns(3000);//1mSEC   

                                        if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                                            break;// complete  error 
                                        }	 


                                    }   //if(nFailFlag4==1) 

                                } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  


                                //----------------------------------------------------------------------------------------------------------------------

                                Sub71.OnPinResetVC(); 

                                for(  int ih3=1; ih3<=nSQCount1; ih3++){  //X   // Second LOOP   vb pin  
                                    if( (ik+1)==nSQLastY1){
                                        if(ih3>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }
                                    //Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih3], 2);   // =1,VB,   all
                                    Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih3], MOD_TR_SET_VC);   // =1,VB,   all
                                } 

                            }// if(nFailFlag3==1) 

                            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                                break;// complete  error 
                            }

                            //################################################################################################################################


                        }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                        //=====================================================================================================================
                    }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                    if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                        break;// complete  error 
                    } 




                    //#2
                }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



            }//end of  if(nFailFlag1==1)

            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                break;// complete  error 
            } 

        }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB


        //==============================================================================================================================
        //################################################################################################################################

        Sub71.OnPinAllReset1(); ///PIN ALL RESET 

     
     
        //================================================================================================================
        //################################################################################################################################
        //#1 FIRST-1  Line&Block
        // Y 

        ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));
        nShort2FailFlag1=0;
        nSQStep1flag=0;
        nRetry1=0;	

    //Task2Retry:

     
        for( ik=1; ik<nSQCount1; ik++)     // FIRST LOOP //Y  VB 
        {
            DoEvents(); 

            if( nStop2==2){
                str.Format(" *****  STOP  *******");			 		
                m_list1.AddString(str); 
                nlist1++;
                goto Task211;	
            }

            if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                break;// complete  error 												 
            }

            if(ik==1  || nSQStep1flag==2){

                Sub71.OnPinAllReset1(); ///PIN ALL RESET 			   

                for( int ik2=1; ik2<=nSQCount1; ik2++){   

                    if(ik2>ik){
                        continue;
                    }
                    for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                        if( ih==nSQLastY1){  //LAST Y STEP
                            if(ik2>nSQLastPin){
                                break;//SKIP   
                            }
                        }
                        //Sub71.AOnPinSet1(2, nSQNet1[ih][ik2], 1); //=2VC,   
                        Sub71.AOnPinSet1(2, nSQNet1[ih][ik2], 1); //=2VC,   
                    } 
                }

                nSQStep1flag=0;

            }else{//end of if(ik==1  || nSQStep1flag==2){


                Sub71.OnPinResetVC(); 
                //   Sub71.Delay_100ns(100);//10uSEC  

                for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                    if( ih==nSQLastY1){  //LAST Y STEP
                        if(ik>nSQLastPin){
                            break;//SKIP   
                        }
                    }

                    Sub71.AOnPinSet1(2, nSQNet1[ih][ik], 1); //=2VC, 

                } 

            }



            for( ih=1; ih<=nSQLastY1; ih++){  //X   // Second LOOP   vb pin  
                if( ih==nSQLastY1){
                    if((ik+1)>nSQLastPin){
                        break;//SKIP
                    }
                }

                Sub71.AOnPinSet1(2, nSQNet1[ih][ik+1], 2);   // =1,VB,   all		   

            }
            //////////////////////////////////////////////////////////////////////////////////

            if( nHVIADCFlag==1 ){
                // dView7.AHV_ADCI_Read1(nProRv1); 
                dView7.ALV_ADCI_Read1_OneToM(nProRv1, nDelay10); 
                //  Sub71.R1= dIADC1  ;//ADC
            }else{		
                Sub71.Delay_100ns(nDelay10);
                dView7.AHV_ADCI_Read1(nProRv1); 
            } 



            if( nHVIADCFlag==1){   									    
                if( Sub71.R1<=nProRv1){// ok//HV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail

                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1; 
                    //nFailFlag1=0;//test
                    nSQStep1flag=2; 

                }
            }else{ 
                if( Sub71.R1>=nProRv1){//ok//LV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1;
                    //	 nFailFlag1=0;//test
                    nSQStep1flag=2;
                }

            }

            // nFailFlag1=0;  // ALLPASS
            //==============================================================================================================================
            //################################################################################################################################
            //#2,  second search    hv  adc  
            // vc  m block 

            if(nFailFlag1==1)  //if error     
            {
                DoEvents(); 
                if( nStop2==2){
                    str.Format(" *****  STOP  *******");			 		
                    m_list1.AddString(str); 
                    nlist1++;
                    goto Task211;					

                }

                Sub71.OnPinResetVB();//sylee121029          


                for( int ik2=1; ik2<=nSQCount1; ik2++){     // FIRST LOOP //Y   //Vc

                    nMSet1Co=0;
                    nMSet1Flag=0;
                    nMSet1CoT2=0;				 

                    //sylee121029  Sub71.OnPinResetVB();
                    //Sub71.Delay_100ns(100);//10uSEC   


                    if(ik2>ik){ //doublecheck#2 
                        continue;
                    }

                    for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                        if(ih==nSQLastY1){  //LAST Y STEP
                            if(ik2>nSQLastPin){
                                break;//SKIP   
                            }
                        }

                        Sub71.AOnPinSet1(2, nSQNet1[ih][ik2], 1); //=2VC, 			 
                    } 


                    //=====================================================================================================================


                    if( nHVIADCFlag==1 ){
                        // dView7.AHV_ADCI_Read1(nProRv1); 
                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                        //  Sub71.R1= dIADC1  ;//ADC
                    }else{	

                        Sub71.Delay_100ns(nDelay10); 

                        if( nCC==2 ){//cv
                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                        }else{// cc										 
                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                        }

                        if(Sub71.Y1<0.0000001){
                            Sub71.Y1=0.0000001;
                        }

                        if(nCC==1){ //CC    r= (vb-vc)/i
                            if( nIR==1 ||  nIR==2 ){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                            }          

                        }else if(nCC==2){//CV   r=(vb/i)
                            if( nIR==1 ||  nIR==2){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                            } 
                        }

                    }

                    //Sub71.Delay_100ns(10000);  
                    //=====================================================================================================================
                    nFailFlag2=0;
                    if( nHVIADCFlag==1){   									    
                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }else{ 
                        if( Sub71.R1>=nProRv1){//ok//LV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }//line & line 



                    //=====================================================================================================================
                    //##################################################################################################################################

                    //one line & one line  search


                    if(nFailFlag2==1){  


                        DoEvents(); 
                        if( nStop2==2){
                            str.Format(" *****  STOP  *******");			 		
                            m_list1.AddString(str); 
                            nlist1++;
                            goto Task211;					

                        }

                        for( int ih6=1; ih6<=nSQCount1; ih6++){    //X // Second LOOP   vc pin 


                            if( ik2==nSQLastY1){  //LAST Y STEP
                                if(ih6>nSQLastPin){
                                    break;//SKIP   
                                }
                            }

                            Sub71.OnPinResetVB();//SYLEE121016***** all reset								    
                            //  Sub71.Delay_100ns(100);//10uSEC   

                            Sub71.AOnPinSet1(2, nSQNet1[ih6][ik2], 1); //=2VC,




                            //=====================================================================================================================

                            if( nHVIADCFlag==1 ){
                                dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                //  Sub71.R1= dIADC1  ;//ADC
                            }else{	


                                Sub71.Delay_100ns(nDelay10);

                                if( nCC==2 ){//cv
                                    Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                }else{// cc										 
                                    Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                }


                                if(Sub71.Y1<0.0000001){
                                    Sub71.Y1=0.0000001;
                                }


                                if(nCC==1){ //CC    r= (vb-vc)/i
                                    if( nIR==1 ||  nIR==2 ){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                    }          

                                }else if(nCC==2){//CV   r=(vb/i)
                                    if( nIR==1 ||  nIR==2){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                    } 
                                }

                            }




                            nFailFlag3=0;
                            if( nHVIADCFlag==1){   									    
                                if( Sub71.R1<=nProRv1){// ok//HV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }else{ 
                                if( Sub71.R1>=nProRv1){//ok//LV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }//line & line 

                            //=====================================================================================================================
                            //################################################################################################################################
                            ///one point  & one point search

                            if(nFailFlag3==1){ 	



                                DoEvents(); 
                                if( nStop2==2){
                                    str.Format(" *****  STOP  *******");			 		
                                    m_list1.AddString(str); 
                                    nlist1++;
                                    goto Task211;					

                                }



                                for( int ih2=1; ih2<=nSQLastY1; ih2++){  //X   // Second LOOP   vb pin  


                                    if( ih2==nSQLastY1){
                                        if((ik+1)>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }

                                    if( ih6 != ih2){//sylee121025
                                        continue;
                                    }

                                    Sub71.OnPinResetVC(); //Vb all reset
                                    // Sub71.Delay_100ns(100);//10uSEC   //SYLEE121017

                                    Sub71.AOnPinSet1(2, nSQNet1[ih2][ik+1], 2);   // =1,VB,   all




                                    if( nHVIADCFlag==1 ){
                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                        //  Sub71.R1= dIADC1  ;//ADC
                                    }else{															
                                        Sub71.Delay_100ns(nDelay10); 
                                        if( nCC==2 ){//cv
                                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                        }else{// cc										 
                                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                        }



                                        if(Sub71.Y1<0.0000001){
                                            Sub71.Y1=0.0000001;
                                        }


                                        if(nCC==1){ //CC    r= (vb-vc)/i
                                            if( nIR==1 ||  nIR==2 ){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                            }          

                                        }else if(nCC==2){//CV   r=(vb/i)
                                            if( nIR==1 ||  nIR==2){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                            } 
                                        }

                                    }	


                                    nFailFlag4=0;
                                    if( nHVIADCFlag==1){   									    
                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        }else{ 



                                            nFailFlag4=1;


                                            for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                                if( (nFailL1[iq1][4]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][5]==(nSQNet1[ih6][ik2]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                                if( (nFailL1[iq1][5]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][4]==(nSQNet1[ih6][ik2]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                            }

                                        }
                                    }else{ 
                                        //if( (Sub71.R1>=nProRv1)&&( Sub71.Y1>5)){//ok//LV
                                        if( Sub71.Y1<5){//ok//LV

                                        }else{//fail
                                            //if(ih2==ih6)
                                            nFailFlag4=1;
                                        }
                                    }
                                    //=====================================================================================================================


                                    //*****************************************************************************//
                                    //##########################################################################################################################################3

                                    //##########################################################################################################################################3

                                    if(nFailFlag4==1){//sylee130111 



                                        if(nFailL1Co<3000){//array size nFailL1[

                                            nFailCount[nMod1]=nFailCount[nMod1]++;


                                            //log buffer
                                            nFailL1Co++;

                                            nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV

                                            if( nSQNet1[ih2][ik+1]<nSQNet1[ih6][ik2]){
                                                nFailL1[nFailL1Co][5]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][4]=nSQNet1[ih6][ik2]+1;//pin  vc
                                            }else{
                                                nFailL1[nFailL1Co][4]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][5]=nSQNet1[ih6][ik2]+1;//pin  vc
                                            }



                                        }   //end of if(nFailL1Co<3000)  //array size nFailL1[


                                        Sub71.Delay_100ns(3000);//1mSEC   


                                        if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                                            break;// complete  error 
                                        }	




                                    }   //if(nFailFlag4==1) 

                                } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  

                                //----------------------------------------------------------------------------------------------------------------------


                                Sub71.OnPinResetVC(); 



                                for(  int ih3=1; ih3<=nSQLastY1; ih3++){  //X   // Second LOOP   vb pin  


                                    if( ih3==nSQLastY1){
                                        if((ik+1)>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }
                                    Sub71.AOnPinSet1(2, nSQNet1[ih3][ik+1], 2);   // =1,VB,   all										      


                                } 


                            }// if(nFailFlag3==1) 



                            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                                break;// complete  error 
                            }


                            //################################################################################################################################


                        }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                        //=====================================================================================================================
                    }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                    if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                        break;// complete  error 
                    } 


                    //#2
                }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



            }//end of  if(nFailFlag1==1)


            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                break;// complete  error 
            } 



        }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB







    }//end of  for ( ip


    //==============================================================================================================================
    //################################################################################################################################


Task211:					

    
    if( nFailL1Co>0){//sylee150811 
        nShortCount=nFailL1Co;//sylee150811 
    }

    Sub71.OnPinAllReset1(); ///PIN ALL RESET 
    Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
    Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704

    Sub71.AOnReadV1Enable1(5); //sylee130711

    ////////////////////////
    //[PRINTING 


    // 	delete [] nBDLPin1; 
    //     delete [] nBDLPin2; 	 



    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;

    if( nFailL1Co <=0){
        str.Format("  ***>  LV  Short  NO Error *******  ");			 
    }else{
        str.Format("   ===>  Result : [LV Short  Error] => %d  ", nFailL1Co );			 		
    }
    m_list1.AddString(str); 
    nlist1++;

    if(SysInfo05.m_nIsEr>1){//SYLEE140110
        if( nFailL1Co==SysInfo05.m_nIsEr){
            str.Format("  ***>    Short Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nIsEr );			 		
            m_list1.AddString(str); 
            nlist1++;
        }
    }

    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;
 
	for( i=1 ; i<=nFailL1Co ; i++){
     	str.Format("   --> Short No=%d,   pin %5.0f - %5.0f  ",i,nFailL1[i][5],nFailL1[i][4] );			 
		m_list1.AddString(str); 
		nlist1++;
	}
 	//	str.Format(" ");			 		
   	//	m_list1.AddString(str); 
	//	nlist1++;



    if( nStop2==2){
        str.Format("  ***> Manual LV Short Check    STOP.  *******  ");			 
    }else{
        str.Format("  ***> Manual LV Short Check    FINISH.  *******  ");	 
    }
    m_list1.AddString(str); 
    nlist1++;

    if( nStop2==2){
        str.Format("STOP"); 		 
    }else{
        str.Format("FINISH"); 	 
    }

    m_Label1001.SetCaption(str);


    //::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);

    Sub71.AOnPoweroff2();//sylee170411

	
	return 1;

}//end of CChildView3::OnManualShort1()  //sylee131023


////=========================================================================================================
 

int CModeMan11::OnManualShort1_HV() //sylee150811
{
 
	CString   str,str1;
	CString   strfName,strfName1;

	char  fName[200], fName1[200]; 
    int i, ik,j , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nECount,nEtemp1,nShort2FailFlag1;
 	int nEt1,nFailL1CoPiece,nCDelay1;
	int nFil,nDisChargeADC1;
	int nHVIADCFlag, nDischargeCount ;
	int nShRes1T, ih,logno1,nSQCount1, nSQLastY1;
	int nSQLastPin, tp1 ;
	double nISet,dVbR1,dVbR2,InterI;
	int nFailFlag1,nFailFlag2,nFailFlag3,nFailFlag4;
	int nSQ1Fail[200];//, nSQPinCheck[34000];
	int nSh1Ret1[100],dPinVC[200],dPinVB[200],nSQNet1[200][200];// pin array//sylee1209
	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200]; 
//	double nCardOffSetHV1;//sylee120915  
	int nMSet1Flag,nMSet1Co, nMSet1CoT2;
	int nSQStep1flag,nIControl,nRetry1;
    int nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR ;
    int m_nMode1,nMod1;
	double nProRv1;
	int nBDLPin1[DEF_MAX_PIN1];
	int nBDLPin2[DEF_MAX_PIN1];
    int nIGain;//sylee240517

    // int nlist1;
    //sylee150811	 nlist1=0;
    nFailL1Co=0;//SYLEE140110
    //sylee150811	m_list1.ResetContent();

    str.Format("RUN"); 
    m_Label1001.SetCaption(str);
    /////////////////////////////////////
    //net list load
    // nBDLHead1[1][3][1]  &nBDLData1[1][i][j]); 

    int k1,k2,i1,nMaxPin1;

    k2=nBDLHead1[1][3][1];

    // ::ZeroMemory(&nBDLPin1, sizeof(nBDLPin1));
    // ::ZeroMemory(&nBDLPin2, sizeof(nBDLPin2));

    k1=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
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
    else{ // 20K, 24K
        nMaxPin1=4096*8;
    }

    //int *nBDLPin1 = new int[33000];
    //	int *nBDLPin1 = new int[nMaxPin1+100];

    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;
    m_list1.AddString(str); 
    nlist1++;
    m_list1.AddString(str); 
    nlist1++;

    str.Format("  ***> Manual HV Short (Leak) :  START  *******  ");			 
    m_list1.AddString(str); 
    nlist1++;


    DoEvents();  

    //	 int *nBDLPin2 = new int[nMaxPin1+1];
    //sylee190104	 for(  i1=0 ; i1< (nMaxPin1+100) ; i1++){	 //SYLEE150811   1->0	 
    //sylee190104 		   nBDLPin1[i1]=0;
    //sylee190104	 }

    //sylee190104	 for(  i1=0 ; i1<= nMaxPin1 ; i1++){  //SYLEE150811   1->0
    //sylee190104		   nBDLPin2[i1]=0;		   
    //sylee190104	 }
    ::ZeroMemory(&nBDLPin1,sizeof(nBDLPin1));//sylee190104
    ::ZeroMemory(&nBDLPin2,sizeof(nBDLPin2));//sylee190104

    for(  i1=1 ; i1<= k2 ; i1++){		 
        k1=nBDLData1[1][i1][1];
        nBDLPin1[k1]=1;	  
    }	

    k1=0;
    for(  i1=0 ; i1<=nMaxPin1 ; i1++){	 //SYLEE150811   1->0	 
        if( nBDLPin1[i1]==1){
            nBDLPin1[0]=nBDLPin1[0]+1;
            k1++;
            nBDLPin2[k1]=i1;
        }		    
    }
    nBDLPin2[0]=k1;

    //////////////////////////////////////
    
    int nRet;

    nMod1=1;
    m_nMode1=302;//sylee150811 
    Sub71.R1=100000000.; //100M  

    nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412

    if( nRet!=1){
        str.Format("ERRor NO 3002,   \n  HR Short Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
        AfxMessageBox( str);
        return 0; 
    }

    RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW , PART_UPPER);   // m_nMode1 1~76   , //son220412_3 
    nProRv1=Sub71.R1;

    if(nProRv1<100){
        nProRv1=100;
    }

    if(nProRv1>1000){
        nProRv1=1000;
    }

    //////////////////////////////////////
    //nProRv1=400;//raw
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
    nIGain = (int)dRec[m_nMode1][12];   //SYLEE230517     

    if( nISet<0.1){
        AfxMessageBox(" Error No 7713, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
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
        AfxMessageBox(" Error No 1024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }

    if( nVSet<1 || nVSet>300 ){
        AfxMessageBox(" Error No 1025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }

    if( nVRel<1  || nVRel>4 ){ 
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        str.Format("Error"); 
        m_Label1001.SetCaption(str); 
        return 0; 
    }

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //설정부 


    //sylee130903	Lo1=200;
    //sylee150708 Lo1=100;//sylee130903  200nS-> 1sec
    Lo1=1000;//sylee150708
    Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

Task101:    

    Sub71.AOnReadVB1Run1(nVRel); 

    if(nVSet<=5){
        dVbR2=nVSet*(0.35);//10%
    }else{
        dVbR2=nVSet*(0.1);//5%
    }
    dVbR1=nVSet-dVbR2;
    dVbR2=nVSet+dVbR2;

    if( Sub71.V1>300){
        if(Lo1-->0){					 					
            //sylee121019	Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(30000);  //1ms   //sylee150708  1->3ms 
            goto Task101;
        }
        str.Format("Error No 1301,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
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
        str.Format("Error No 1317,\n \n\n Voltage set error.    Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str);						  
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }
  
    Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET 
    Sub71.FilterSet(nFil);//sylee130702 
    if(nIFilter==3){
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
        nIFilter=1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629
    if (nIGain == 2){ //SYLEE240517    
       Sub71.I_Gain(2);  //10x  =io on        
    } else {
        Sub71.I_Gain(1);  //1x  =io off
    }

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

    if( nVSet>200){
        nLoop=1;
        str.Format("Error No 1318,\n \n\n  MANUAL SHORT START ERROR    VSET >200V !!    "); 
        AfxMessageBox(str);
        str.Format("Error"); 
        m_Label1001.SetCaption(str);
        return 0;
    }else{
        nLoop=0;
    }

    ipFlag=2;
    ipEnd=1;
    nStop2=0;

    FileSysInfo01.LoadSaveSub15(2);//SYLEE140110
    /////////////////////////////////////////////////////////////////////

 
//====================================================================================================================================
    for( int ip=1; ip<=ipEnd; ip++ )
    { 

        if(nRCount<=0){
            //ERROR DEBUG   PIECE=ip NO LIST.
            continue;//sylee20120402
        }
        nEtemp1=0;	nECount=0; 	nFailL1CoPiece=0;
        nEt1=0;	nShort2FailFlag1=0;
        //*******************************************************************************************************************************//
        //Task1:
        /*
           if(ipFlag==1){
           nRCount=nBDLData31[1][ip][0]; 
           }else{
           nRCount=nBDLData3[1][0][1];
           } 
         */

        nRCount=nBDLPin2[0];

        nSQCount1=0;//sylee120908   //SQRT( SQUARE ROOT)
        for( i=1; i<200;i++){
            tp1=i*i;
            if(tp1>=nRCount){
                nSQCount1=i;//sylee120908
                break;
            }
        }

        if( (nSQCount1==0)  || (nRCount<1)){//sylee121018
            str.Format("Error No 7513-6 ,   SET ERORR   Net List <1,  ,    piece pin no set check?   HV SQUARE  COUNT = ZERO   n"   ); 
            AfxMessageBox(str);	
            str.Format("Error"); 
            m_Label1001.SetCaption(str);
            return 0;
        }

        ::ZeroMemory(&nSQNet1, sizeof(nSQNet1));

        //################################################################################################################################
        //SET 


        for( ik=1; ik<=nRCount; ik++)
        {
            i=(ik-1)/nSQCount1  + 1;//mok
            j=(ik-1)-nSQCount1*(i-1);//na
            // if(i>1){
            j=j+1;
            // }					 
            /*
               if( ipFlag==1 ){//short   PIECE MULT.
               nSQNet1[i][j]=nBDLData31[1][ip][ik] ; // 1 VB,    =2VC, 
               }else{						 
               nSQNet1[i][j]=nBDLData3[1][ik][1] ;   // 1  VB,    =2VC, 	 
               } 
               */

            //SYLEE150811		 nSQNet1[i][j]=nBDLPin2[ik]-1;//SYLEE140110  nBDLPin2[ik]-1
            nSQNet1[i][j]=nBDLPin2[ik];//SYLEE150811

            if( ik==nRCount){
                nSQLastPin=j;//sylee120908   ///last row , last col.
                nSQLastY1=i;
            }
        }


        //################################################################################################################################
        //#1 FIRST-1  Line&Block
        //row

        ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));	
        nShort2FailFlag1=0;
        nSQStep1flag=0;

        Sub71.OnPinAllReset1(); ///PIN ALL RESET 

        nRetry1=0;


//Task1Retry:

 
        for( ik=1; ik<nSQLastY1; ik++)     // FIRST LOOP //Y  VB 
        {


            DoEvents(); 

            if( nStop2==2){
                str.Format(" *****  STOP  *******");			 		
                m_list1.AddString(str); 
                nlist1++;
                goto Task211;					

            }

            if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                break;// complete  error 												 
            }



            if(ik==1  || nSQStep1flag==2){

                Sub71.OnPinAllReset1(); ///PIN ALL RESET 

                for( int ik2=1; ik2<=nSQLastY1; ik2++){   

                    if(ik2>ik){
                        continue;
                    }
                    for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                        if( ik2==nSQLastY1){  //LAST Y STEP
                            if(ih>nSQLastPin){
                                break;//SKIP   
                            }
                        }
                        Sub71.AOnPinSet1(2, nSQNet1[ik2][ih], 1); //=1Vb,    
                    } 
                }

                nSQStep1flag=0;


            }else{//end of if(ik==1  || nSQStep1flag==2){			   
                Sub71.OnPinResetVC();  
                for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                    if( ik==nSQLastY1){  //LAST Y STEP
                        if(ih>nSQLastPin){
                            break;//SKIP   
                        }
                    }				   
                    Sub71.AOnPinSet1(2, nSQNet1[ik][ih], 1); //=1  vb 
                }
            }




            for( ih=1; ih<=nSQCount1; ih++){  //X   // Second LOOP   vb pin  
                if( (ik+1)==nSQLastY1){
                    if(ih>nSQLastPin){
                        break;//SKIP
                    }
                }	   

                Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih], 2);   // =2,Vc,   all	

            }
            //////////////////////////////////////////////////////////////////////////////////


            if( nHVIADCFlag==1 ){
                // dView7.AHV_ADCI_Read1(nProRv1); 
                //    dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10,1);//sylee150811
                dView7.ALV_ADCI_Read1_OneToM( nProRv1, nDelay10);//sylee150811

                //  Sub71.R1= dIADC1  ;//ADC
            }else{		
                Sub71.Delay_100ns(nDelay10);
                dView7.AHV_ADCI_Read1(nProRv1); 
            } 

            if( nHVIADCFlag==1){   									    
                if( Sub71.R1<=nProRv1){// ok//HV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1; 

                    nSQStep1flag=2;
                }
            }else{ 
                if( Sub71.R1>=nProRv1){//ok//LV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail  
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1;
                    //	 nFailFlag1=0;//test
                    nSQStep1flag=2;
                }

            }



            //==============================================================================================================================
            //################################################################################################################################
            //#2,  second search    hv  adc  
            // vc  m block 

            DoEvents(); 

            if(nFailFlag1==1)  //if error	 
            {
                DoEvents(); 
                if( nStop2==2){
                    str.Format(" *****  STOP  *******");			 		
                    m_list1.AddString(str); 
                    nlist1++;
                    goto Task211;					

                }



                Sub71.OnPinResetVB();   //sylee121029  

                for( int ik2=1; ik2<=nSQLastY1; ik2++){     // FIRST LOOP //Y   //Vc				

                    nMSet1Co=0;
                    nMSet1Flag=0;
                    nMSet1CoT2=0;

                    if(ik2>ik){ //doublecheck#2 
                        continue;
                    }

                    for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                        if(ik2==nSQLastY1){  //LAST Y STEP
                            if(ih>nSQLastPin){
                                break;//SKIP   
                            }
                        }					   
                        Sub71.AOnPinSet1(2, nSQNet1[ik2][ih], 1); //=2VC,	 
                    } 
                    //=====================================================================================================================
                    if( nHVIADCFlag==1 ){
                        // dView7.AHV_ADCI_Read1(nProRv1); 
                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                        //  Sub71.R1= dIADC1  ;//ADC
                    }else{							
                        Sub71.Delay_100ns(nDelay10); 

                        if( nCC==2 ){//cv
                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                        }else{// cc										 
                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                        }

                        if(Sub71.Y1<0.0000001){
                            Sub71.Y1=0.0000001;
                        }

                        if(nCC==1){ //CC    r= (vb-vc)/i
                            if( nIR==1 ||  nIR==2 ){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                            }          

                        }else if(nCC==2){//CV   r=(vb/i)
                            if( nIR==1 ||  nIR==2){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                            } 
                        }

                    }


                    //Sub71.Delay_100ns(10000);  
                    //=====================================================================================================================
                    nFailFlag2=0;
                    if( nHVIADCFlag==1){   									    
                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }else{ 
                        if( Sub71.R1>=nProRv1){//ok//LV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }//line & line 

                    //=====================================================================================================================
                    //##################################################################################################################################

                    //one line & one line  search


                    if(nFailFlag2==1){  

                        DoEvents(); 
                        if( nStop2==2){
                            str.Format(" *****  STOP  *******");			 		
                            m_list1.AddString(str); 
                            nlist1++;
                            goto Task211;					

                        }


                        for( int ih6=1; ih6<=nSQCount1; ih6++){    //X // Second LOOP   vc pin 

                            if( ik2==nSQLastY1){  //LAST Y STEP
                                if(ih6>nSQLastPin){
                                    break;//SKIP   
                                }
                            }

                            Sub71.OnPinResetVB();//SYLEE121016***** all reset								    
                            // Sub71.Delay_100ns(100);//10uSEC   

                            Sub71.AOnPinSet1(2, nSQNet1[ik2][ih6], 1); //=2VC,	 

                            //===================================================================================================================== 
                            if( nHVIADCFlag==1 ){
                                dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                //  Sub71.R1= dIADC1  ;//ADC
                            }else{										

                                Sub71.Delay_100ns(nDelay10);

                                if( nCC==2 ){//cv
                                    Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                }else{// cc										 
                                    Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                }


                                if(Sub71.Y1<0.0000001){
                                    Sub71.Y1=0.0000001;
                                }


                                if(nCC==1){ //CC    r= (vb-vc)/i
                                    if( nIR==1 ||  nIR==2 ){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                    }          

                                }else if(nCC==2){//CV   r=(vb/i)
                                    if( nIR==1 ||  nIR==2){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                    } 
                                }

                            }



                            nFailFlag3=0;
                            if( nHVIADCFlag==1){   									    
                                if( Sub71.R1<=nProRv1){// ok//HV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }else{ 
                                if( Sub71.R1>=nProRv1){//ok//LV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }//line & line 


                            //=====================================================================================================================
                            //################################################################################################################################
                            ///one point  & one point search

                            if(nFailFlag3==1){ 


                                DoEvents(); 
                                if( nStop2==2){
                                    str.Format(" *****  STOP  *******");			 		
                                    m_list1.AddString(str); 
                                    nlist1++;
                                    goto Task211;					

                                }


                                for( int ih2=1; ih2<=nSQCount1; ih2++){  //X   // Second LOOP   vb pin  


                                    if( (ik+1)==nSQLastY1){
                                        if(ih2>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }

                                    Sub71.OnPinResetVC(); //Vb all reset
                                    // Sub71.Delay_100ns(100);//10uSEC   //SYLEE121017

                                    Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all



                                    if( nHVIADCFlag==1 ){
                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                        //  Sub71.R1= dIADC1  ;//ADC
                                    }else{															
                                        Sub71.Delay_100ns(nDelay10); 
                                        if( nCC==2 ){//cv
                                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                        }else{// cc										 
                                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                        }


                                        if(Sub71.Y1<0.0000001){
                                            Sub71.Y1=0.0000001;
                                        }


                                        if(nCC==1){ //CC    r= (vb-vc)/i
                                            if( nIR==1 ||  nIR==2 ){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                            }          

                                        }else if(nCC==2){//CV   r=(vb/i)
                                            if( nIR==1 ||  nIR==2){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                            } 
                                        }

                                    }			





                                    nFailFlag4=0;
                                    if( nHVIADCFlag==1){   									    
                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        }else{//fail
                                            // 	if(ih2==ih6)
                                            nFailFlag4=1;


                                            for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                                if( (nFailL1[iq1][4]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][5]==(nSQNet1[ik2][ih6]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                                if( (nFailL1[iq1][5]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][4]==(nSQNet1[ik2][ih6]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                            }

                                        }
                                    }else{ 
                                        //if( (Sub71.R1>=nProRv1)&&( Sub71.Y1>5)){//ok//LV
                                        if( Sub71.Y1<5){//ok//LV
                                        }else{//fail
                                            //   if(ih2==ih6)
                                            nFailFlag4=1;
                                        }
                                    }  

                                    //##########################################################################################################################################3


                                    if(nFailFlag4==1){//sylee130111 




                                        /////////////////////////TEST////////////////////
                                        /*	
                                            Sub71.OnPinAllReset1();//SYLEE121016***** all reset								    
                                            Sub71.OnPinAllReset1();//SYLEE121016***** all reset								    
                                            Sub71.OnPinAllReset1();//SYLEE121016***** all reset								    



                                        //	Sub71.AOnPinSet1(2, nSQNet1[ik2][ih6], 1); //=2VC,
                                        // Sub71.OnPinResetVC(); //Vb all reset
                                        //    Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all

                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 

                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        Sub71.R1=Sub71.R1;
                                        }else{
                                        Sub71.R1=Sub71.R1;
                                        }

*/
                                        /*

                                           Sub71.OnPinResetVB();//SYLEE121016***** all reset								    
                                           Sub71.AOnPinSet1(2, nSQNet1[ik2][ih6], 1); //=2VC,
                                           Sub71.OnPinResetVC(); //Vb all reset
                                           Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all

                                           dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 

                                           if( Sub71.R1<=nProRv1){// ok//HV
                                           Sub71.R1=Sub71.R1;
                                           }else{
                                           Sub71.R1=Sub71.R1;
                                           }


                                           Sub71.OnPinResetVB();//SYLEE121016***** all reset								    
                                           Sub71.AOnPinSet1(2, nSQNet1[ik2][ih6], 1); //=2VC,
                                           Sub71.OnPinResetVC(); //Vb all reset
                                           Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all

                                           dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 

                                           if( Sub71.R1<=nProRv1){// ok//HV
                                           Sub71.R1=Sub71.R1;
                                           }else{
                                           Sub71.R1=Sub71.R1;
                                           }


*/  

                                        /////////////////////////TEST////////////////////



                                        //if((nOpenErrList[tmp1][0]<=0)&&(nOpenErrList[tmp2][0]<=0)) 												 

                                        if(nFailL1Co<3000){//array size nFailL1[

                                            nFailCount[nMod1]=nFailCount[nMod1]++; 
                                            nFailL1Co++;

                                            nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV
                                            if( nSQNet1[ik+1][ih2]<nSQNet1[ik2][ih6]){
                                                nFailL1[nFailL1Co][5]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][4]=nSQNet1[ik2][ih6]+1;//pin  vc
                                            }else{
                                                nFailL1[nFailL1Co][4]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][5]=nSQNet1[ik2][ih6]+1;//pin  vc
                                            }  																		 

                                        }   //end of if(nFailL1Co<3000) //array size nFailL1[


                                        Sub71.Delay_100ns(3000);//1mSEC   

                                        if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                                            break;// complete  error 
                                        }	 


                                    }   //if(nFailFlag4==1) 

                                } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  


                                //----------------------------------------------------------------------------------------------------------------------

                                Sub71.OnPinResetVC(); 
                                //   Sub71.Delay_100ns(100);//10uSEC   

                                for(  int ih3=1; ih3<=nSQCount1; ih3++){  //X   // Second LOOP   vb pin  

                                    if( (ik+1)==nSQLastY1){
                                        if(ih3>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }
                                    Sub71.AOnPinSet1(2, nSQNet1[ik+1][ih3], 2);   // =1,VB,   all										      

                                } 


                            }// if(nFailFlag3==1) 



                            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                                break;// complete  error 
                            }



                            //################################################################################################################################




                        }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                        //=====================================================================================================================
                    }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                    if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                        break;// complete  error 
                    } 




                    //#2
                }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



            }//end of  if(nFailFlag1==1)





            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                break;// complete  error 
            } 



        }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB


        //==============================================================================================================================
        //################################################################################################################################

        Sub71.OnPinAllReset1(); ///PIN ALL RESET 



        //================================================================================================================
        //################################################################################################################################
        //#1 FIRST-1  Line&Block
        // Y 

        ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));
        nShort2FailFlag1=0;
        nSQStep1flag=0;

        nRetry1=0;			  


    //Task2Retry:




        for( ik=1; ik<nSQCount1; ik++)     // FIRST LOOP //Y  VB 
        {

            DoEvents(); 

            if( nStop2==2){
                str.Format(" *****  STOP  *******");			 		
                m_list1.AddString(str); 
                nlist1++;
                goto Task211;					

            }

            if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                break;// complete  error 												 
            }



            if(ik==1  || nSQStep1flag==2)
            {

                Sub71.OnPinAllReset1(); ///PIN ALL RESET 


                for( int ik2=1; ik2<=nSQCount1; ik2++){   

                    if(ik2>ik){
                        continue;
                    }
                    for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                        if( ih==nSQLastY1){  //LAST Y STEP
                            if(ik2>nSQLastPin){
                                break;//SKIP   
                            }
                        }
                        Sub71.AOnPinSet1(2, nSQNet1[ih][ik2], 1); //=2VC,  

                    } 
                }

                nSQStep1flag=0;

            }
            else//end of if(ik==1  || nSQStep1flag==2){
            {


                Sub71.OnPinResetVC(); 
                //   Sub71.Delay_100ns(100);//10uSEC  

                for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                    if( ih==nSQLastY1){  //LAST Y STEP
                        if(ik>nSQLastPin){
                            break;//SKIP   
                        }
                    }

                    Sub71.AOnPinSet1(2, nSQNet1[ih][ik], 1); //=2VC, 

                } 

            }


	   
            for( ih=1; ih<=nSQLastY1; ih++)  //X   // Second LOOP   vb pin  
            {
                if( ih==nSQLastY1){
                    if((ik+1)>nSQLastPin){
                        break;//SKIP
                    }
                }

                Sub71.AOnPinSet1(2, nSQNet1[ih][ik+1], 2);   // =1,VB,   all		   

            }
            //////////////////////////////////////////////////////////////////////////////////

            if( nHVIADCFlag==1 ){
                // dView7.AHV_ADCI_Read1(nProRv1); 
                //  dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10,1);//sylee150811
                dView7.ALV_ADCI_Read1_OneToM( nProRv1, nDelay10);//sylee150811
                //  Sub71.R1= dIADC1  ;//ADC
            }else{		
                Sub71.Delay_100ns(nDelay10);
                dView7.AHV_ADCI_Read1(nProRv1); 
            } 



            if( nHVIADCFlag==1){   									    
                if( Sub71.R1<=nProRv1){// ok//HV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail

                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1; 
                    //nFailFlag1=0;//test
                    nSQStep1flag=2; 

                }
            }else{ 
                if( Sub71.R1>=nProRv1){//ok//LV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1;
                    //	 nFailFlag1=0;//test
                    nSQStep1flag=2;
                }

            }

            // nFailFlag1=0;  // ALLPASS
            //==============================================================================================================================
            //################################################################################################################################
            //#2,  second search    hv  adc  
            // vc  m block 

            if(nFailFlag1==1)  //if error     
            {		    	
                DoEvents(); 
                if( nStop2==2){
                    str.Format(" *****  STOP  *******");			 		
                    m_list1.AddString(str); 
                    nlist1++;
                    goto Task211;					

                }

                Sub71.OnPinResetVB();//sylee121029          


                for( int ik2=1; ik2<=nSQCount1; ik2++)     // FIRST LOOP //Y   //Vc
                {
                    nMSet1Co=0;
                    nMSet1Flag=0;
                    nMSet1CoT2=0;				 

                    //sylee121029                 Sub71.OnPinResetVB();
                    //Sub71.Delay_100ns(100);//10uSEC   


                    if(ik2>ik){ //doublecheck#2 
                        continue;
                    }

                    for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                        if(ih==nSQLastY1){  //LAST Y STEP
                            if(ik2>nSQLastPin){
                                break;//SKIP   
                            }
                        }

                        Sub71.AOnPinSet1(2, nSQNet1[ih][ik2], 1); //=2VC, 			 
                    } 


                    //=====================================================================================================================


                    if( nHVIADCFlag==1 ){
                        // dView7.AHV_ADCI_Read1(nProRv1); 
                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                        //  Sub71.R1= dIADC1  ;//ADC
                    }else{	

                        Sub71.Delay_100ns(nDelay10); 

                        if( nCC==2 ){//cv
                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                        }else{// cc										 
                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                        }

                        if(Sub71.Y1<0.0000001){
                            Sub71.Y1=0.0000001;
                        }

                        if(nCC==1){ //CC    r= (vb-vc)/i
                            if( nIR==1 ||  nIR==2 ){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                            }          

                        }else if(nCC==2){//CV   r=(vb/i)
                            if( nIR==1 ||  nIR==2){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                            } 
                        }

                    }

                    //Sub71.Delay_100ns(10000);  
                    //=====================================================================================================================
                    nFailFlag2=0;
                    if( nHVIADCFlag==1){   									    
                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }else{ 
                        if( Sub71.R1>=nProRv1){//ok//LV
                        }else{//fail
                            nFailFlag2=1;
                        }
                    }//line & line 



                    //=====================================================================================================================
                    //##################################################################################################################################

                    //one line & one line  search


                    if(nFailFlag2==1)  
                    {

                        DoEvents(); 
                        if( nStop2==2){
                            str.Format(" *****  STOP  *******");			 		
                            m_list1.AddString(str); 
                            nlist1++;
                            goto Task211;					

                        }

                        for( int ih6=1; ih6<=nSQCount1; ih6++)    //X // Second LOOP   vc pin 
                        {

                            if( ik2==nSQLastY1){  //LAST Y STEP
                                if(ih6>nSQLastPin){
                                    break;//SKIP   
                                }
                            }

                            Sub71.OnPinResetVB();//SYLEE121016***** all reset								    
                            //  Sub71.Delay_100ns(100);//10uSEC   

                            Sub71.AOnPinSet1(2, nSQNet1[ih6][ik2], 1); //=2VC,




                            //=====================================================================================================================

                            if( nHVIADCFlag==1 ){
                                dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                //  Sub71.R1= dIADC1  ;//ADC
                            }else{	


                                Sub71.Delay_100ns(nDelay10);

                                if( nCC==2 ){//cv
                                    Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                }else{// cc										 
                                    Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                }


                                if(Sub71.Y1<0.0000001){
                                    Sub71.Y1=0.0000001;
                                }


                                if(nCC==1){ //CC    r= (vb-vc)/i
                                    if( nIR==1 ||  nIR==2 ){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                    }          

                                }else if(nCC==2){//CV   r=(vb/i)
                                    if( nIR==1 ||  nIR==2){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                    }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                        Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                    } 
                                }

                            }




                            nFailFlag3=0;
                            if( nHVIADCFlag==1){   									    
                                if( Sub71.R1<=nProRv1){// ok//HV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }else{ 
                                if( Sub71.R1>=nProRv1){//ok//LV
                                }else{//fail
                                    nFailFlag3=1;
                                }
                            }//line & line 

                            //=====================================================================================================================
                            //################################################################################################################################
                            ///one point  & one point search

                            if(nFailFlag3==1) 	
                            {


                                DoEvents(); 
                                if( nStop2==2){
                                    str.Format(" *****  STOP  *******");			 		
                                    m_list1.AddString(str); 
                                    nlist1++;
                                    goto Task211;					

                                }



                                for( int ih2=1; ih2<=nSQLastY1; ih2++)  //X   // Second LOOP   vb pin  
                                {

                                    if( ih2==nSQLastY1){
                                        if((ik+1)>nSQLastPin){
                                            break;//SKIP
                                        }
                                    }

                                    if( ih6 != ih2){//sylee121025
                                        continue;
                                    }

                                    Sub71.OnPinResetVC(); //Vb all reset
                                    // Sub71.Delay_100ns(100);//10uSEC   //SYLEE121017

                                    Sub71.AOnPinSet1(2, nSQNet1[ih2][ik+1], 2);   // =1,VB,   all




                                    if( nHVIADCFlag==1 ){
                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                        //  Sub71.R1= dIADC1  ;//ADC
                                    }else{															
                                        Sub71.Delay_100ns(nDelay10); 
                                        if( nCC==2 ){//cv
                                            Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
                                        }else{// cc										 
                                            Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ									 
                                        }



                                        if(Sub71.Y1<0.0000001){
                                            Sub71.Y1=0.0000001;
                                        }


                                        if(nCC==1){ //CC    r= (vb-vc)/i
                                            if( nIR==1 ||  nIR==2 ){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                            }          

                                        }else if(nCC==2){//CV   r=(vb/i)
                                            if( nIR==1 ||  nIR==2){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                            }else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                                Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                            } 
                                        }

                                    }	


                                    nFailFlag4=0;
                                    if( nHVIADCFlag==1){   									    
                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        }else{ 


                                            //if(ih2==ih6)
                                            nFailFlag4=1;


                                            for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                                if( (nFailL1[iq1][4]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][5]==(nSQNet1[ih6][ik2]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                                if( (nFailL1[iq1][5]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][4]==(nSQNet1[ih6][ik2]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                            }

                                        }
                                    }else{ 
                                        //if( (Sub71.R1>=nProRv1)&&( Sub71.Y1>5)){//ok//LV
                                        if( Sub71.Y1<5){//ok//LV

                                        }else{//fail
                                            //if(ih2==ih6)
                                            nFailFlag4=1;
                                        }
                                    }
                                    //=====================================================================================================================


                                    //*****************************************************************************//
                                    //##########################################################################################################################################3

                                    //##########################################################################################################################################3

                                    if(nFailFlag4==1){//sylee130111 



                                        if(nFailL1Co<3000){//array size nFailL1[

                                            nFailCount[nMod1]=nFailCount[nMod1]++;


                                            //log buffer
                                            nFailL1Co++;

                                            nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV

                                            if( nSQNet1[ih2][ik+1]<nSQNet1[ih6][ik2]){
                                                nFailL1[nFailL1Co][5]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][4]=nSQNet1[ih6][ik2]+1;//pin  vc
                                            }else{
                                                nFailL1[nFailL1Co][4]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
                                                nFailL1[nFailL1Co][5]=nSQNet1[ih6][ik2]+1;//pin  vc
                                            }



                                        }   //end of if(nFailL1Co<3000)//array size nFailL1[


                                        Sub71.Delay_100ns(3000);//1mSEC   


                                        if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block 단위로 잘못 되어  piece 단위 하기 위해 막음 .
                                            break;// complete  error 
                                        }	




                                    }   //if(nFailFlag4==1) 

                                } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  

                                //----------------------------------------------------------------------------------------------------------------------


                                Sub71.OnPinResetVC(); 



                                for(  int ih3=1; ih3<=nSQLastY1; ih3++)  //X   // Second LOOP   vb pin  
                                {

                                     if( ih3==nSQLastY1){
                                         if((ik+1)>nSQLastPin){
                                             break;//SKIP
                                         }
                                     }
                                     Sub71.AOnPinSet1(2, nSQNet1[ih3][ik+1], 2);   // =1,VB,   all										      


                                 } 


                             }// if(nFailFlag3==1) 



                             if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                                 break;// complete  error 
                             }


                             //################################################################################################################################


                         }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                         //=====================================================================================================================
                     }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                     if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                         break;// complete  error 
                     } 


                     //#2
                 }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



             }//end of  if(nFailFlag1==1)


            if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                break;// complete  error 
            } 

            
        }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB


     }//end of  for  ip


//==============================================================================================================================
//################################################################################################################################



Task211:					

       
 
    Sub71.OnPinAllReset1(); ///PIN ALL RESET 
    Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
    Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704
    Sub71.AOnReadV1Enable1(5); //sylee130711
    ////////////////////////
    //[PRINTING 
    //  	delete [] nBDLPin1; 
    //      delete [] nBDLPin2; 

    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;
	 
   	if( nFailL1Co <=0){
		str.Format("  ***>   HV Short(Leak)     NO Error *******  ");			 
	}else{
		str.Format("  ====>  Result : [HV Short(Leak)  Error] => %d  ", nFailL1Co );			 		
	}
  	m_list1.AddString(str); 
	nlist1++;

    if(SysInfo05.m_nIsEr>1){//SYLEE140110
        if( nFailL1Co==SysInfo05.m_nIsEr){
            str.Format("  ***>    Short Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nIsEr );			 		
            m_list1.AddString(str); 
            nlist1++;
        }
    }

    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;

    for( i=1 ; i<=nFailL1Co ; i++){
        str.Format("   --> HV Short No=%d,   pin %5.0f - %5.0f  ",i,nFailL1[i][5],nFailL1[i][4] );			 
        m_list1.AddString(str); 
        nlist1++;
    }
    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;

    if( nStop2==2){
        str.Format("  ***> Manual HV Short(Leak) Check    STOP.  *******  ");			 
    }else{
        str.Format("  ***> Manual HV Short(Leak) Check    FINISH.  *******  ");	 
    }
    m_list1.AddString(str); 
    nlist1++;

    if( nStop2==2){
        str.Format("STOP"); 		 
    }else{
        str.Format("FINISH"); 	 
    }

    m_Label1001.SetCaption(str);
    //::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);
    Sub71.AOnPoweroff2();//sylee170411

    return 1;

}//end of CChildView3::OnManualShort1()  //sylee131023

