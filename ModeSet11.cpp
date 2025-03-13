// ModeSet11.cpp : implementation file
////sylee180921 
//
// IDD_MODE_SET11_SELF1: SelfTest Cycle TR

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet11.h"

#include "ModeSet12.h"
#include "ModeSet19.h"
#include "ModeSub71.h"
#include "FileSysInfo.h"

extern CFileSysInfo  FileSysInfo01; 
extern CModeSub71  Sub71;
extern CSysSet12  SysSet12; 
extern CSysSet16  SysSet16; 
extern CSysSet19  SysSet19;
extern double dRec[362][16];
extern __int64 GetMilSecond2(); 
extern __int64 GetuSecond();
extern void DoEvents();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeSet11 dialog

#define DEFINE_HVTIMEOUT1  15000000  

extern double nVSet, dProT1;
extern double  d4WADCPer1 ,d4WADCPer2,d4WADCPer3 , d4WADCSigma1; 




CModeSet11::CModeSet11(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSet11::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSet11)
		// NOTE: the ClassWizard will add member initialization here


	//}}AFX_DATA_INIT
}


void CModeSet11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSet11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//	DDX_Control(pDX, IDC_CHECK1, m_check1);
	   DDX_Control(pDX, IDC_EDIT101,   m_edit101);  
	   DDX_Control(pDX, IDC_EDIT102,   m_edit102);
	   DDX_Control(pDX, IDC_EDIT103,   m_edit103);
	   DDX_Control(pDX, IDC_EDIT104,   m_edit104);
	   DDX_Control(pDX, IDC_LABEL101,  m_Label101); 	 
 	   DDX_Control(pDX, IDC_LABEL102,  m_Label102); 
	   DDX_Control(pDX, IDC_BUTTON101, m_button101); 
	   DDX_Control(pDX, IDC_BUTTON102, m_button102); 
	  	DDX_Control(pDX, IDC_LIST1, m_list1);//sylee230817
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet11, CDialog)
	//{{AFX_MSG_MAP(CModeSet11)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet11 message handlers

BEGIN_EVENTSINK_MAP(CModeSet11, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeSet11, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSet11, IDC_BUTTON101, -600 /* Click */, OnButton101_Test, VTS_NONE)
	ON_EVENT(CModeSet11, IDC_BUTTON102, -600 /* Click */, OnButton102_Stop, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CModeSet11::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
	m_edit101.SetWindowText("1");//start pin  
	m_edit102.SetWindowText("4096");//end pin
	m_edit103.SetWindowText("1");// block
	m_edit104.SetWindowText("100");// Mohm

	Sub71.AOnVSetMulti1(15,1,250,50) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CModeSet11::OnClickOk() 
{
 
	CDialog::OnOK();
}

void CModeSet11::OnButton101_Test() //test
{
   
	OnTest1_Set1();
    OnTest1_Run1_LV();
   if( nLVNG1==1) return;//sylee230815
	OnTest1_Run1_HV();
 
	return ;
}



void CModeSet11::OnTest1_Set1()
{
 
	nFlag_Stop=0;
    m_edit103.GetWindowText( szText,10);
    nBlockNo = atoi( szText ); 
	if(nBlockNo<1){ nBlockNo=1;}
	if(nBlockNo>16){ nBlockNo=16;}
    sprintf(szText, "%d",  nBlockNo );    
    m_edit103.SetWindowText(szText);
    DoEvents();DoEvents(); 
   
   sprintf(szText, "%d",  1+(nBlockNo-1)*4096);    
   m_edit101.SetWindowText(szText);
   sprintf(szText, "%d",  (nBlockNo)*4096);    
   m_edit102.SetWindowText(szText);
   DoEvents();DoEvents(); 

    m_edit101.GetWindowText( szText,10);
    nStartPin = atoi( szText ); 
    m_edit102.GetWindowText( szText,10);
    nEndPin = atoi( szText );  
 

    if(nStartPin<1)    nStartPin=1; 
    if(nEndPin<1)      nEndPin=1; 
	if(nStartPin>(4096*16))    nStartPin=4096*6;
    if(nEndPin>(4096*16))   nEndPin=1;	
    if(nStartPin>nEndPin)    nEndPin=nStartPin;

    m_edit104.GetWindowText( szText,10);
    nMohm = atoi( szText ); 
    if(nMohm<1)       nMohm=10; 
    if(nMohm>1000)    nMohm=1000;

    sprintf(szText, "%d",  nMohm );    
    m_edit104.SetWindowText(szText);

    m_list1.ResetContent();   
	  str.Format("    <<   Selt Test  TR.   Start!   >>  "   );    
      m_list1.AddString(str);
    DoEvents();DoEvents(); 
     
	return ;
}

 


int CModeSet11::OnTest1_Run1_LV()//sylee230815
{ 	 
     
	int m_nMode1,nCC,nVRel,nIR,nISet,nDelay10,nFil,nIFilter,nHVOffSet,nHVGain,nIGain,InterI,Lo1,nProRv1; 
	double dVbR1,dVbR2;
 
    nLVNG1=0;//sylee230815
    m_nMode1=207;  
	nProRv1=500;    

    str.Format("**>  LV SHORT:  STart   "  );     m_list1.AddString(str);
  
    Sub71.OnPinAllReset1(); 
    Sub71.OnDisCharge1();  
    Sub71.OnPinAllReset1(); 
 //==============================================================

    nCC = (int)dRec[m_nMode1][1];
    nVRel = (int)dRec[m_nMode1][2];
    nVSet = dRec[m_nMode1][3];
    nIR = (int)dRec[m_nMode1][4];
    nISet = dRec[m_nMode1][5];
    nDelay10 = (int)dRec[m_nMode1][6] * 10; 
    if (m_nMode1 > 205) {
        nDelay10 = 50000; 
    }

    nFil = (int)dRec[m_nMode1][8];
    nIFilter = (int)dRec[m_nMode1][9];      
    nHVOffSet = (int)dRec[m_nMode1][10];  
    nHVGain = (int)dRec[m_nMode1][11];   
    nIGain = (int)dRec[m_nMode1][12];  

    if (nISet < 0.1) {
        AfxMessageBox(" Error No 7711, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!");
    }

    Sub71.nADCount1 = (int)dRec[m_nMode1][7];

    if (nIR == 5) {     InterI = 10.0;
    } else {      InterI = nISet * 0.9;    }

    //=========================================================================================
    if (nCC != 1 && nCC != 2) {
        AfxMessageBox(" Error No 1024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!");
          return 0;
    }

    if (nVSet < 1 || nVSet>300) {
        AfxMessageBox(" Error No 1025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!");
         return 0;
    }

    if (nVRel < 1 || nVRel>4) {
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!");
          return 0;
    }

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 

    Lo1 = 3000; //sylee141114   //sylee170525   500->3000 
    Sub71.AOnReadVB1Run1(nVRel);//sylee130802


Task101:


    Sub71.AOnReadVB1Run1(nVRel);
    if (nVSet <= 5) {
        dVbR2 = nVSet * (0.35);//35%
    }
    else {
        dVbR2 = nVSet * (0.1);//10%  //SYLEE211118    5->10%
    }
    dVbR1 = nVSet - dVbR2;
    dVbR2 = nVSet + dVbR2;
 
    if (Sub71.V1 > 300) {
        if (Lo1-- > 0) {
            //sylee121019  Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(10000);  //1ms  //sylee141114 
            goto Task101;
        } 
     
        str.Format("Error No 1301,\n\n  short  voltage set error. Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n  ,", (int)m_nMode1, nVSet, Sub71.V1);
        AfxMessageBox(str);    
        return 0;
    }  

    if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2) {
        if (Lo1-- > 0) { 
            Sub71.Delay_100ns(1000);  //0.1ms                           
            goto Task101;
        }      
        str.Format("Error No 1314,\n \n\n   LV  Short   voltage set error.   bbt controller  power check! \n\n\n VB Set=%.2f, VB Read =%.2f ,\n  ,", nVSet, Sub71.V1);
        AfxMessageBox(str);           
     
        Sub71.OnPinAllReset1(__LINE__);
        Sub71.ARunoff1();
        Sub71.AOnRelayOffSel(nVRel);
        Sub71.FilterSet(0);
        Sub71.I_Filter(0);
        Sub71.AOnReadV1Enable1(5);
        Sub71.OnPinResetInit();
        return 0;
    }

    Sub71.AOnReadV1Enable1(nVRel); 
    Sub71.FilterSet(nFil);
    if (nIFilter == 3) {
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!");
        nIFilter = 1;
    }
    Sub71.I_Filter(nIFilter);

    if (nIGain == 2) {      Sub71.I_Gain(2);      }
    else {       Sub71.I_Gain(1);      }

    Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet); 
    Sub71.OnPinAllReset1(); ///PIN ALL RESET  

//=============================================================================

	int nX1,nADC1NgLV,mStart,nEnd; 

	nX1=1;  nADC1NgLV=0;
	Sub71.OnPinAllReset1();  
	mStart=(1+(4096*(nBlockNo-1)));
	nEnd=mStart+4095;

	for( int x1=mStart;  x1<nEnd;  x1++){
		DoEvents();
		if(	nFlag_Stop==2){	break;	} 

    	if(x1==mStart){
		   for( int y1=mStart+1;  y1<=nEnd;  y1++){			
                Sub71.AOnPinSet1(1, y1, 2); 
		   } 
		}else{
			Sub71.AOnPinReSet1(1, x1, 2);  
		} 
		if(x1>mStart){
			Sub71.AOnPinReSet1(1, x1-1, 1);
		}
        Sub71.AOnPinSet1(1, x1, 1);

		AHV_ADCI_Read1_Avg3() ; //sylee230815
	    //	dADC1[x1]=Sub71.R1;
		if(Sub71.R1>nProRv1){
            Sub71.OnPinAllReset1(); 
		    nADC1NgLV=x1;
			nLVNG1=1;//sylee230815
			break;
		}
	}
 

	if(nADC1NgLV>0){ 
        str.Format("**>STOP: LV SHORT:  NG PIN => %d    ", (nADC1NgLV+(4096*(nX1-1)))   );    
	}else{
        str.Format("**>  LV SHORT: NO ERROR   "   );    
	}
	  m_list1.AddString(str);    
      str.Format("**>  LV SHORT:  FINISH   "  );     m_list1.AddString(str);

	  	if(	nFlag_Stop==2){	str.Format("**>  STOP!  "  );     m_list1.AddString(str);	} 

	   
     
	  
//=============================================================================
    Sub71.OnPinAllReset1(); 
    Sub71.ARunoff1();    
    Sub71.AOnRelayOffSel(nVRel); 
    Sub71.FilterSet(0); 
    Sub71.I_Filter(0); 
    Sub71.AOnReadV1Enable1(5);  
    Sub71.OnPinResetInit(); 
    Sub71.AIRangeOff(); 
//=============================================================================

    return 1;
}



 
int CModeSet11::AHV_ADCI_Read1_Avg3()  //sylee230815
{

	int nNo1, nADSum1, nMax1,nMax,nMin,ik1;
//	FILE *fp;
	DoEvents();

	for ( ik1 = 1; ik1 <= 10; ik1++) {
	 	if(	nFlag_Stop==2){	break;	} 
		Sub71.Delay_100ns(10000); //1ms
		Sub71.AOnReadV1I1_ADC_I();
 		if( Sub71.R1>5000){
			   Sub71.AOnReadV1I1_ADC_I();
			  if( Sub71.R1>5000){
					return 1;
			  }
		}
	}
	if(	nFlag_Stop==2){	 return 1;	} 

   // Sub71.Delay_100ns(100000); //10ms 
	nNo1 = 5;//sylee200917
    nADSum1 = 0.0; 
	nMax1=0;

	for ( ik1 = 1; ik1 <= nNo1; ik1++) {
			if(	nFlag_Stop==2){	break;	} 
            Sub71.AOnReadV1I1_ADC_I();
			if( ik1 == 1 ) {  nMin=Sub71.R1;  nMax=Sub71.R1;  }//sylee230814
			else{
				if(nMin>Sub71.R1)  nMin=Sub71.R1;//sylee230814
				if(nMax<Sub71.R1)  nMax=Sub71.R1;//sylee230814
			}            
 
			Sub71.Delay_100ns(500); //50uS
            nADSum1 = nADSum1 + Sub71.R1;
			if( Sub71.R1>nMax1){
				nMax1=Sub71.R1;
			}
		 
	}
   //sylee230814  Sub71.R1 = nADSum1 / nNo1;
	 Sub71.R1 = (nADSum1-nMin-nMax) / (nNo1-2);//sylee230814 
 
    return 1;
}

 



int CModeSet11::OnTest1_Run1_HV()//sylee230815
{ 	 

	int m_nMode1,nCC,nVRel,nIR,nISet,nDelay10,nFil,nIFilter,nHVOffSet,nHVGain,nIGain,InterI,Lo1; 
	double dVbR1,dVbR2,nProRv1;
	long nHVTimeOutSet;
	int nErrList[5001][5];
	int nErrListCo,nErrLimit,nErrListCo2;

	m_nMode1=327; 
	if(nMohm<0)nMohm=1;
	if(nMohm>1000)nMohm=1000;
	Sub71.R1=nMohm*1000000;

	FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_UPPER);
	RCalCovert(m_nMode1, Sub71.R1, REAL_TO_RAW, PART_UPPER);  

	nProRv1=200;
	nProRv1=Sub71.R1;
	nErrLimit=100;
	nErrListCo=0;
	nErrListCo2=0;
	::ZeroMemory(&nErrList, sizeof(nErrList));

    Sub71.OnRelay4W_OFF1();  //SYLEE181118
    nHVTimeOutSet = DEFINE_HVTIMEOUT1 * 3;

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
     
    nCC = (int)dRec[m_nMode1][1];
    nVRel = (int)dRec[m_nMode1][2];
    nVSet = dRec[m_nMode1][3];
    nIR = (int)dRec[m_nMode1][4];
    nISet = dRec[m_nMode1][5];
    nDelay10 = (int)dRec[m_nMode1][6] * 10;
    nFil = (int)dRec[m_nMode1][8];
    nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629        
    nHVOffSet = (int)dRec[m_nMode1][10]; //SYLEE130629
    nHVGain = (int)dRec[m_nMode1][11];  //SYLEE130629
    nIGain = (int)dRec[m_nMode1][12];    //SYLEE150123 
   

    if (nVSet > 20) {//SYLEE141211
        if (nIR == 1) {
            AfxMessageBox("  Error  No 7101.  HV leak Mode,    I Range Mode 1 Use error.   Call bioptro! ", MB_OK);
            return 0;
        }
    }
    if (nProRv1 < 10) {       nProRv1 = 10;    } 
    if (nISet < 0.1) {
        AfxMessageBox(" Error No 7710, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!");
    }
    Sub71.nADCount1 = (int)dRec[m_nMode1][7];
    if (nIR == 5) {//uA
        InterI = 10.0;
    } else {
        InterI = nISet * 0.9;
    }

    //=========================================================================================
    //interlock  
    if (nCC != 1 && nCC != 2) {
        AfxMessageBox(" Error No 1024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!");
        return 0;
    }

    if (nVSet < 1 || nVSet>300) {
        AfxMessageBox(" Error No 1025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!");
        return 0;
    }

    if (nVRel < 1 || nVRel>4) {
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!");
        return 0;
    }

 //=========================================


    Lo1 = 3000;  


Task101:
 

     Sub71.AOnReadVB1Run1(nVRel);

    if (nVSet <= 5) {
        dVbR2 = nVSet * (0.35);//35%
    }else {
        dVbR2 = nVSet * (0.1);//10%  //SYLEE211118    5->10%
    }
    dVbR1 = nVSet - dVbR2;
    dVbR2 = nVSet + dVbR2; 

 
        if (Sub71.V1 > 300) {
            if (Lo1-- > 0) {
                //SYLEE121019 Sub71.Delay_100ns(100000);  //10ms   
                Sub71.Delay_100ns(10000);  //1ms   //SYLEE121019
                goto Task101;
            }
            str.Format("Error No 1301,\n\n   high voltage power set error. Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1, nVSet, Sub71.V1, dVbR1, dVbR2);
            AfxMessageBox(str);
            return 0;
        }

#ifdef _PC_TEST_
        Sub71.V1 = nVSet;       //son220927 PC_TEST에서 HW Check 에러 무시하기 위함.
#endif
        if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2)
        {
            if (Lo1-- > 0) {
                //SYLEE121019 Sub71.Delay_100ns(100000);  //10ms   
                Sub71.Delay_100ns(10000);  //1ms   //SYLEE121019
                goto Task101;
            }
            str.Format("Error No 1313,\n \n\n high voltage power  set  error.  Power Check? \n\n\n   VB READ Error!!    Calibration NO=%d ! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),", (int)m_nMode1, nVSet, Sub71.V1, dVbR1, dVbR2);
            AfxMessageBox(str); 
            return 0;
        }  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Sub71.FilterSet(nFil);//sylee130628 
    if (nIFilter == 3) {
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!");
        nIFilter = 1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629

    if (nIGain == 2) {     Sub71.I_Gain(2); 
    } else {      Sub71.I_Gain(1);    }

    Sub71.On4W_GainSet(1);
    Sub71.AOnReadV1Enable1(nVRel);
    Sub71.OnPinAllReset1(); //SYLEE150120
    Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet); 
    Sub71.Delay_100ns(50000); //10->5mS  //sylee170427 
    Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118
  
//=============================================================================
 	Sleep(1000);//  1second
//	str.Format("**>  HV SHORT:  START   "  );     m_list1.AddString(str);  
//=============================================================================

  
	long  x1,y1,x2,nX1,nADC1NgHV,mStart,nEnd,mStart2; 
 
	nX1=1;  nADC1NgHV=0; 
	Sub71.OnPinAllReset1(); 
	mStart=(1+(4096*(nBlockNo-1)));
	nEnd=mStart+4095;



 /////////////////////////////////////////////////////////////////////////////////////////////


	for(   x1=mStart;  x1<mStart+50;  x1++){ // 
		DoEvents();
		if(	nFlag_Stop==2){	Sub71.OnPinAllReset1();		break;	} 

    	if(x1==mStart){
		   for(  y1=mStart+1;  y1<=nEnd;  y1++){			
                 Sub71.AOnPinSet1(1, y1, 2); 
		   } 
		}else{
	  		Sub71.AOnPinReSet1(1, x1, 2);  
		}
 
	 	Sub71.OnPinResetVB(); 
		if(x1>mStart){
     		Sub71.AOnPinReSet1(1, x1-1, 1);
		}
        Sub71.AOnPinSet1(1, x1, 1);
		AHV_ADCI_Read1_Avg3() ; //sylee230815	 
		if(Sub71.R1 > 1000){
            Sub71.OnPinAllReset1(); 
	    	break;
			/*
	        Sub71.OnPinResetVC(); 
			for(  x2=(mStart2+1) ;  x2<=nEnd; x2++){ 
	    		 Sub71.AOnPinSet1(1, x2, 2);				 
			}
			*/
		}
	}
 
//////////////////////////////////////////////////////////////////////////////////////////////







 /////////////////////////////////////////////////////////////////////////////////////////////


	for(   x1=mStart;  x1<nEnd;  x1++){
		DoEvents();
		if(	nFlag_Stop==2){	Sub71.OnPinAllReset1();		break;	} 

    	if(x1==mStart){
		   for(  y1=mStart+1;  y1<=nEnd;  y1++){			
                 Sub71.AOnPinSet1(1, y1, 2); 
		   } 
		}else{
	  		Sub71.AOnPinReSet1(1, x1, 2);  
		}
 
	 	Sub71.OnPinResetVB(); 
		if(x1>mStart){
     		Sub71.AOnPinReSet1(1, x1-1, 1);
		}
        Sub71.AOnPinSet1(1, x1, 1);
		AHV_ADCI_Read1_Avg3() ; //sylee230815
 
	 
		if(Sub71.R1 > nProRv1){
            Sub71.OnPinAllReset1(); 
		    nADC1NgHV=x1;
 
//////////////////////////////////////////////////////////////////////////////////////////////
//2차 
			mStart2=x1;
             Sub71.AOnPinSet1(1, x1, 1);
			for(   x2=(mStart2+1) ;  x2<=nEnd; x2++){ 
				if(	nFlag_Stop==2){
					Sub71.OnPinAllReset1();		break;	
				} 

			 	 Sub71.AOnPinSet1(1, x2, 2);
				 AHV_ADCI_Read1_Avg3() ; //sylee230815
			     Sub71.AOnPinReSet1(1, x2, 2); 	

				 if(Sub71.R1 > nProRv1){
					 if(nErrListCo<nErrLimit){
						 nErrListCo++;
						 nErrList[nErrListCo][0]=x1;
						 nErrList[nErrListCo][1]=x2;
						 nErrList[nErrListCo][2]=(int)Sub71.R1;
						 nErrList[nErrListCo][3]=nProRv1;
					 }else{
						 Sub71.OnPinAllReset1();  
						 goto Task100;
					 }
				 }
				 
                if(Sub71.R1>3000){
					nErrListCo2++;
				}

				if(nErrListCo2>=3){
					 Sub71.OnPinAllReset1();  
					 goto Task100;
				}
			}
/////////////////////////////////////////////////////////////////////////////////////////////

	        Sub71.OnPinResetVC(); 
			for(  x2=(mStart2+1) ;  x2<=nEnd; x2++){ 
	    		 Sub71.AOnPinSet1(1, x2, 2);				 
			}
		}
	}
 
//////////////////////////////////////////////////////////////////////////////////////////////




Task100:

 	if(nErrListCo>0){ 
      str.Format("**>  HV SHORT  ERROR COUNT => %d    ", nErrListCo  ); 
	}else{
      str.Format("**>  HV SHORT: NO ERROR   ", (nADC1NgHV+(4096*(nX1-1)))   );    
	}


   m_list1.AddString(str); 
   str.Format("**>  HV SHORT:  FINISH   "  );     m_list1.AddString(str);

   if( nFlag_Stop==2){ str.Format("**>  STOP !   "  );     m_list1.AddString(str);}
	  
//=============================================================================



//==============================================================================

char fName[200];
FILE *fp; 

	if(nErrListCo>0){
		str.Format("d:\\0SelfTr_CheckResult.csv"); 
		::ZeroMemory(&fName, sizeof(fName));
		strcat( fName , str );
		fp = fopen(fName,"wt");
		if(fp == NULL){         
		}else{ 
			for(int i=1; i<=nErrListCo; i++){ 
				 str.Format("No=,%5d,TPin1=,%d,TPin2=,%d,Card1=,%d,Pin1=,%d,Card2=,%d,Pin2=,%d, ADC=,%d,RefADC,=%d,",  i, nErrList[i][0],nErrList[i][1], (nErrList[i][0]/128)+1,nErrList[i][0]%128, (nErrList[i][1]/128)+1, nErrList[i][1]%128, nErrList[i][2] ,nErrList[i][3] );    
				  m_list1.AddString(str);    
			      fprintf(fp, "No=,%5d,TPin1=,%7d,TPin2=,%7d,Card1=,%d,Pin1=,%d,Card2=,%d,Pin2=,%d, ADC=,%5d,RefADC,=%5d, \n ",i, nErrList[i][0],nErrList[i][1], (nErrList[i][0]/128)+1,nErrList[i][0]%128, (nErrList[i][1]/128)+1, nErrList[i][1]%128, nErrList[i][2] ,nErrList[i][3] );
			}
			fprintf(fp, "  \n");
			fclose(fp);
		}	
	}
 //	::ShellExecute(NULL,"open","EXCEl.EXE",str,"NULL",SW_SHOWNORMAL);	 
//==============================================================================
 
    Sub71.OnPinAllReset1(); ///PIN ALL NO RESET   
    Sub71.ARunoff1();   
    Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720
    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704
    Sub71.AOnReadV1Enable1(5);
    Sub71.AIRangeOff();//sylee171204 //ACE400
    Sub71.OnDisCharge1();   

    return 1;
}


 


void CModeSet11::OnButton102_Stop() //stop
{
    nFlag_Stop=2;
	DoEvents();
	return ;
}


 
