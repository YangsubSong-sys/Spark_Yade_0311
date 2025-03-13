// ModeMan17.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeMan17.h"
#include "Contec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern int PinData[DEF_MAX_PIN1][10];   //file reading
extern int PinDataP1[DEF_MAX_PIN1][10];  //piece sort
extern int PinDataP2[300][3]; // piece header
extern int nPcb1[5000][2]; //PCB NO , PIN NO
extern double nCalC[30][10][100];
extern int nMCon3,	nMCon2, nMCon3, nMCon4;

extern void DoEvents();
extern double OnRsMeas1(int a, int nMode2) ;
extern void OnRsModeRange(int a, int b);
extern void OnRsModeFreq(int a);  //MODE C  // 1= 1000 , 2= 120 ,  ?
extern void OnRsModeSpeed(int a);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?

extern int nRsModeC1;


//#include "GRs.h"

/////////////////////////////////////////////////////////////////////////////
// CModeMan17 dialog


CModeMan17::CModeMan17(CWnd* pParent /*=NULL*/)
	: CDialog(CModeMan17::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeMan17)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeMan17::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeMan17)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeMan17, CDialog)
	//{{AFX_MSG_MAP(CModeMan17)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeMan17 message handlers
BEGIN_EVENTSINK_MAP(CModeMan17, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeMan17)
	ON_EVENT(CModeMan17, IDC_BUTTON1001, -600 /* Click */, OnClickButton1001, VTS_NONE)
	ON_EVENT(CModeMan17, IDC_BUTTON103, -600 /* Click */, OnClickButton103, VTS_NONE)
	ON_EVENT(CModeMan17, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 
 BOOL CModeMan17::PreTranslateMessage(MSG* pMsg) 
{
// TODO: Add your specialized code here and/or call the base class
	
/*
	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_ESCAPE)) {
		return TRUE;
	}
*/
 
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
 //	 	OnOK();
		return TRUE;
	}
 
	return CDialog::PreTranslateMessage(pMsg);
}

 

BOOL CModeMan17::OnInitDialog() 
{
	CDialog::OnInitDialog(); 	 
  
	return TRUE;                  
}


void CModeMan17::OnClickButton1001() 
{
  	CString  str;
	int a, a1,a2,b,b1,b2, c;
	int nMode; 
	int  Ret, j;
	double v; 
	int nS1,nF1,nCo;
	int nModeC;
	int nRt1;
	int nModeCha1;
	int nModeCAuto1;
	
	nModeCAuto1=2; // auto sylee20110519  ,


	return;//DEMO


	nS1=0;
	nF1=0;
    nlist1=0;
	nCo=0;
	nRt1=0;

//	nModeCha1=0;
    nModeCha1=1;
 
	//nModeC=nRsModeR1O;
	nModeC=nRsModeC1;
/*
	 if( nModeC <9){
		OnRsModeFreq(1);  //MODE C  // 1= 1000 , 2= 120 ,  ?
		OnRsModeSpeed(1);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
	 }else{
		OnRsModeFreq(2);  //MODE C  // 1= 1000 , 2= 120 ,  ?
		OnRsModeSpeed(2);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
	 }
*/

	if( nModeC  <2 ) {
		 AfxMessageBox("    ***  Manual Start Error  (  C Range Mode < 2 )  \n  RECIPE Setting   C VALUE ? ***  ");
      return ;
	}

	if( nModeC  > 3 ) {
		 AfxMessageBox("    ***  Manual Start Error  (  C Range Mode > 3 )  \n  RECIPE Setting  C VALUE ? ***  ");
      return ;
	}

  //  OnRsModeRange(11,1);  //MODE Range(0~10)


	if( nModeCAuto1!=2 ){
		OnRsModeRange(nModeC,2);  //MODE Range(0~10)  //4,5,6//-->
	}else{
		nModeC=11;
		OnRsModeRange(nModeC,2);  //MODE Range(0~10)  //4,5,6//-->
	}

	//if( nlist1 >50){
	m_list1.ResetContent();
	//nlist1=0;

//	}

//	 e=RunInfo05.m_nCon;
//	 ec1=0; 
//   es1=SysInfo05.m_nConEr;
 
 	 c =  PinDataP1[0][2] ;  // save해야 한다 

	 str.Format("  **** C  Test  Start :  Net List Total No.= %d   ", c); 
 	 m_list1.AddString(str);
	 nlist1++;
	 str.Format("                                                                   " ); 
 	 m_list1.AddString(str);
	 nlist1++;

 	 if(c<1){
		  AfxMessageBox("    *** C Test  Net list  ?   ***  ");
 		 return ;//error
	 }
   
	nStop=0;
	nMode=1;//sylee//-->

	/*
	if( nRsMode2 < 1  ||  nRsMode2 > 20){ //sylee
        AfxMessageBox(" Errro No 1023, \n Mode Select Please !   ( 1~20 )    Testing Start Error!!!");
		return ; 
	 
	}else if( nRsMode2<11){
		nMode=1;//sylee   c		
	}else if( nRsMode2<21){  
		nMode=2;//sylee  r	 
	}
	*/
	//-----------------------------------------
   
 		 for( j=1; j<(c+1); j++){
			 //    DoEvents();
				if(nStop2==2){
					nStop2=0;
					 AfxMessageBox("  C   Test Mode   Stop !!!");
					return ;	
				}
		//i=9;
		 //-----------------------------------------
				  a=PinDataP1[j][0];
				  b=PinDataP1[j][1];
				  a1=nPcb1[a][0];//pcb no  0~
				  a2=nPcb1[a][1];//pin no  0~63  type b
				  b1=nPcb1[b][0];//pcb no
				  b2=nPcb1[b][1];//pin no
	 
				if(nMode==2){  //R MODE
					 Ret=pDIO.ResetALL1(2); 
					// Ret=pDIO.ResetS1();  //source
					 if ( Ret!=1 ){ AfxMessageBox("Outbit[  ResetALL1-2  C mode] Error!!!");	return ;	}
				}else if(nMode==1){  //C MODE  
					 Ret=pDIO.ResetALL1(1); 
					// Ret=pDIO.ResetS1();  //source
					 if ( Ret!=1 ){ AfxMessageBox("Outbit[  ResetALL1-1  C mode] Error!!!");	return ;	}
				}else{
					 AfxMessageBox(" Error No 2015 \n Outbit[  ResetALL1   C MOde Select Error] Error!!!");	
                     return ;
				}

				if(nStop==2){
					 AfxMessageBox("   C Mode Run    ->  Stop !!!");
					return ;	
				}

			//	pDIO.Delay_us(1000); //test100

				if(nMode==1){ //C MODE
					//Ret=pDIO.PinS1(j,1,0,2,2);//source pin set
					Ret=pDIO.PinS1(a2,1,0,a1,2);//source pin set
					//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
					if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 source pin set] Error!!!");	return ;	}
				}else if(nMode==2){  //R MODE   
				//	Ret=pDIO.PinS1(j,1,0,0,1);//source pin set
					Ret=pDIO.PinS1(a2,1,0,a1,1);//source pin set
					//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
					if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 source pin set] Error!!!");	return ;	}
				  //  Ret=pDIO.PinS1(j,1,1,1,1);//sink pin set
				    Ret=pDIO.PinS1(b2,1,1,b1,1);//sink pin set
					//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
				    if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 sink pin set] Error!!!");	return ;	}
				 }
  
    
Task101:
				 DoEvents();
				 UpdateData(FALSE);
				 if( nModeCha1==0){
			   		 if(j==1){
			 			  pDIO.Delay_us(1000); //test100//simulator    1000   16회 1.6초      100  0.1초 
					 }
				 }
				 if( nModeCha1==1){
				     //pDIO.Delay_us(80); 
				      pDIO.Delay_us(2000); 
				 }

				if(nMode==1){  //C MODE
			 	     pDIO.Delay_us(2000); //MECATEST

				}

				if( nModeCAuto1==2 ){
					pDIO.Delay_us(10000); //MECATEST
				}
				
				
				if(nMode==1){ //C MODE
	                //v=OnRsMeas1(2,nMode); //MES?
					v=OnRsMeas1(2,2); //MES?
				}else if(nMode==2){  //R MODE 
	                //v=OnRsMeas1(2,nMode); //MES?
					v=OnRsMeas1(2,1); //MES?
				}
				//v=999999999999999.0; //MES?simulator
				//v=999.0; //MES?  simulator
                
				if(v<0){								
				 	v=-1;
					AfxMessageBox("   RS232 Communication Error?   \n    C OR R MODE ?  Error!!!");
					return ; 
				}  

		    //	v=9999999999;//simulator

				if( nModeCha1==0){
					nRt1=100;
				}

				if(v<100000000){ 
				   v=v*1000000000000;
				////////// /////////////////////////
				 if( nModeCAuto1!=2 ){
					 int ka, kc,k;
					 double  aa,ab,ac;
					 int m;

					 aa=0.0;   ab=0.0;  ac=0.0;
					 kc=0;

					 m=nModeC; //-->
					 ka=(int)nCalC[m][1][0];
	

					 if(ka>100){
						 ka=0;
					 }
					 // v=v*1000000000000;

					 if(v>0){
						 if( v<nCalC[m][2][ka]){
							 if(ka>0){
								 for( k=1; k<ka+1;  k++){
									 if( v< nCalC[m][2][k]){									 
										 if(k==1){
											 //v=nCalC[m][1][k];
											 v=-0.0;
										 }else{
											 aa=(v-nCalC[m][2][k-1])/(nCalC[m][2][k]-nCalC[m][2][k-1]);  
											 ab=nCalC[m][1][k]-nCalC[m][1][k-1];
											 ac=ab*aa+nCalC[m][1][k-1];
										 }
										 kc=1;
										 k=ka;					 
									 }
								 }
							 }
							 v=ac;
						 }else{

						 }
					 }else{
                        v=88888888888; 
					 }


				 }//if( nModeCAuto1!=2 ){


				}//end if(v<100000000){ 



 
				if(v>100000000 || v<0.1){
				 	//v=0;
					//v=v;
					/*
					if(nRt1<3){
						nRt1++;
						if(v==-1.0){
							str.Format("        NO %d,PIN=%d,%d,Value=Communication Error. or  Out of  Range?  Ohmn<%d ?: mode%d=>change ",j,a, b, nMCon3,nModeC);
						}else if(v>9999999999){
					        str.Format("        NO %d,PIN=%d, %d,  Value=Out of Range.  Ohmn  < %d  FAIL : R mode=%d=>change  ",j,a, b, nMCon3,nModeC);						
						}else{
							str.Format("        NO %d,PIN=%d,%d,Value=%.2fOhmn<%d ?: mode%d=>change ",j,a, b,v,nMCon3,nModeC);
						}
					    m_list1.AddString(str);
			            nlist1++;

						if(nRt1==1){
							nModeC=nRsModeC1-1;
						}else if(nRt1==2){
							nModeC=nRsModeC1+1;
						}
						OnRsModeRange(nModeC,1);
					 //	pDIO.Delay_us(100);

	
						goto Task101;
					*/
					if(nRt1<2){

						if( nModeCAuto1!=2 ){
							nRt1++;
							if(v==-1.0){
								str.Format("        NO %d,PIN=%d, Value=Communication Error. or  Out of  Range?  pF <%d ?: C mode%d=>change ",j,a, nMCon3,nModeC);
							}else if(v>9999999999){
								str.Format("        NO %d,PIN=%d, Value=Out of Range.  pF  < %d  FAIL : C mode=%d=>change  ",j,a, nMCon3,nModeC);						
							}else{
								str.Format("        NO %d,PIN=%d, Value=%.2f  pF< %d ?: C mode%d=>change ",j,a,v,nMCon3,nModeC);
							}

							m_list1.AddString(str);
							nlist1++;

							if(nRt1==1){
								if(nRsModeC1==2){
									nModeC=3;
								}else{
									nModeC=2;
								}
							}else{
								nModeC=2;
							}
						
							OnRsModeRange(nModeC,2);
							//	pDIO.Delay_us(100);
							goto Task101;
						}//if( nModeCAuto1!=2 ){

					}else{
					///	nRt1=0;
					/*	nModeC=nRsModeR1O;
						if(nModeCha1==1){
							OnRsModeRange(nModeC,1);
						//	pDIO.Delay_us(100);
						}
						*/
					}

				}else{

                    /*
					if( nModeCha1==1 ){
						if( nRt1>0 ){
							
							 if( v<nMCon3 ){
								str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d OK : mode=%d",j,a, b,v,nMCon3,nModeC);						 
							}else{
								str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d FAIL: mode=%d",j,a, b,v,nMCon3,nModeC);
							}
					        m_list1.AddString(str);
			                nlist1++;


							nRt1=0;
							 
						 	nModeC=nRsModeR1O;
						 	OnRsModeRange(nModeC,1);
						//	pDIO.Delay_us(10);
						}
					}
					*/

					//if( nRsMode2 ==2  || nRsMode2 ==3 ){
					//	 v=v*1000000000000;
				    //}

				}//end  


 		
          //  v=100; //simulator

			//int nMCon3,	nMCon2;

			if( (v>0.1) && (v<nMCon3) ) {//ok 				
			//	v=100;	
				nF1=0;
			}else{ //fail
 				nF1=1;
				nCo=nCo++;;
			}

 	         //if( nModeCha1==0 ){


			if( nRt1>0 ){
				if( nF1==0 ){
					str.Format(" NO %d,    PIN=%d,  Value=%.2f pF  < %d  OK  : C mode=%d  ",j,a,v,nMCon3 ,nModeC); 
				}else{
					if(v==-1.0){
						str.Format(" NO %d,    PIN=%d,  Value=Communicaton Error.  or   Out of Range ?   pF  < %d  FAIL : C mode=%d  ",j,a, nMCon3,nModeC);
					}else if(v>9999999999){
						str.Format(" NO %d,    PIN=%d,  Value=Out of Range.  pF  < %d  FAIL : C mode=%d   ",j,a,  nMCon3,nModeC);
					}else{
						str.Format(" NO %d,    PIN=%d, Value=%.2f pF  < %d  FAIL : C mode=%d  ",j,a, v,nMCon3,nModeC);
					}
				}

			}else{
        		if( nF1==0 ){
					str.Format(" NO %d,    PIN=%d,    Value=%.2f pF  < %d  OK  : C mode=%d  ",j,a, v,nMCon3 ,nModeC); 
				}else{
					if(v==-1.0){
						str.Format(" NO %d,    PIN=%d, Value=Communicaton Error.  or   Out of Range ?   pF  < %d  FAIL : C  mode=%d  ",j,a,  nMCon3,nModeC);
					}else if(v>9999999999){
						str.Format(" NO %d,    PIN=%d, Value=Out of Range.  pF  < %d  FAIL :  C mode=%d  ",j,a,  nMCon3,nModeC);
					}else{
						str.Format(" NO %d,    PIN=%d,  Value=%.2f pF  < %d  FAIL : C mode=%d  ",j,a, v,nMCon3,nModeC);
					}
				}
			}

				  
				if( nModeCha1==1 ){
					if( nRt1>0 ){
						
						/*if( v<nMCon3 ){
							str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d OK : mode=%d",j,a, b,v,nMCon3,nModeC);						 
						}else{
							str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d FAIL: mode=%d",j,a, b,v,nMCon3,nModeC);
						}
					    m_list1.AddString(str);
			            nlist1++;
                        */ 

						nRt1=0;
						 
						nModeC=nRsModeC1;
						if( nModeCAuto1!=2 ){
							OnRsModeRange(nModeC,2);
						}
					//	pDIO.Delay_us(10);
					}
				}
					 

		
 		 		m_list1.AddString(str);
				nlist1++;
			 //}
    }   

	 str.Format("                                     " ); 
	 m_list1.AddString(str);
	 nlist1++;
 
	 nF1=c-nCo;

	 //str.Format("      : OPen Error = %d EA ,       OK = %d EA  " ,nCo,nF1); 
	 //m_list1.AddString(str);
	 //nlist1++;

	 str.Format("  **** C END ****   " ); 
	 m_list1.AddString(str);
	 nlist1++;

	 if( nCo ==0){
		str.Format("            ==> *SUCCESS  : Error = %d EA ,   OK = %d EA      " ,nCo,nF1); 
	 }else{
		str.Format("            ==> *FAIL : Error = %d EA ,    OK = %d EA     " ,nCo,nF1); 
	 }

 	m_list1.InsertString(1, str);
	nlist1++;
 
    Ret=pDIO.ResetInit(); 
    if ( Ret!=1 ){ AfxMessageBox("Outbit[ResetInit] Error!!!");	return ;	}

    AfxMessageBox("    *** C Test  End***  ");
	
}



//=============================================================================================
/*   //sylee20110519  auto mode insert
void CModeMan17::OnClickButton1001() 
{
  	CString   str;
	int a, a1,a2,b,b1,b2, c;
//	double dVal[5];
	int nMode; 
	int  Ret, j;
	double v; 
	int nS1,nF1,nCo;
	int nModeC;

	int nRt1;
	int nModeCha1;

	int nModeCAuto1;
	
	nModeCAuto1=2; // auto sylee20110519


	nS1=0;
	nF1=0;
    nlist1=0;
	nCo=0;
	nRt1=0;


//	nModeCha1=0;
    nModeCha1=1;
 
	//nModeC=nRsModeR1O;
	nModeC=nRsModeC1;
/*
	 if( nModeC <9){
		OnRsModeFreq(1);  //MODE C  // 1= 1000 , 2= 120 ,  ?
		OnRsModeSpeed(1);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
	 }else{
		OnRsModeFreq(2);  //MODE C  // 1= 1000 , 2= 120 ,  ?
		OnRsModeSpeed(2);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
	 }
*/
/*   //sylee20110519  auto mode insert
	if( nModeC  <2 ) {
		 AfxMessageBox("    ***  Manual Start Error  (  C Range Mode < 2 )  \n  RECIPE Setting   C VALUE ? ***  ");
      return ;
	}

	if( nModeC  > 3 ) {
		 AfxMessageBox("    ***  Manual Start Error  (  C Range Mode > 3 )  \n  RECIPE Setting  C VALUE ? ***  ");
      return ;
	}

  //  OnRsModeRange(11,1);  //MODE Range(0~10)

	OnRsModeRange(nModeC,2);  //MODE Range(0~10)  //4,5,6//-->

	//if( nlist1 >50){
	m_list1.ResetContent();
	//	nlist1=0;

//	}

//	 e=RunInfo05.m_nCon;
//	 ec1=0; 
//   es1=SysInfo05.m_nConEr;
 
 	 c =  PinDataP1[0][2] ;  // save해야 한다 

	 str.Format("  **** C  Test  Start :  Net List Total No.= %d   ", c); 
 	 m_list1.AddString(str);
	 nlist1++;
	 str.Format("                                                                   " ); 
 	 m_list1.AddString(str);
	 nlist1++;

 	 if(c<1){
		  AfxMessageBox("    *** C Test  Net list  ?   ***  ");
 		 return ;//error
	 }
   
	nStop=0;
	nMode=1;//sylee//-->

	/*
	if( nRsMode2 < 1  ||  nRsMode2 > 20){ //sylee
        AfxMessageBox(" Errro No 1023, \n Mode Select Please !   ( 1~20 )    Testing Start Error!!!");
		return ; 
	 
	}else if( nRsMode2<11){
		nMode=1;//sylee   c		
	}else if( nRsMode2<21){  
		nMode=2;//sylee  r	 
	}
	*/
	//-----------------------------------------
  
 /*   //sylee20110519  auto mode insert
 		 for( j=1; j<(c+1); j++){

			 //    DoEvents();

				if(nStop2==2){
					nStop2=0;
					 AfxMessageBox("  C   Test Mode   Stop !!!");
					return ;	
				}

		//i=9;
		 //-----------------------------------------
		          a=PinDataP1[j][0];
				  b=PinDataP1[j][1];
				      a1=nPcb1[a][0];//pcb no  0~
					  a2=nPcb1[a][1];//pin no  0~63  type b
			          b1=nPcb1[b][0];//pcb no
					  b2=nPcb1[b][1];//pin no
		 
					if(nMode==2){  //R MODE
						 Ret=pDIO.ResetALL1(2); 
						// Ret=pDIO.ResetS1();  //source
						 if ( Ret!=1 ){ AfxMessageBox("Outbit[  ResetALL1-2  C mode] Error!!!");	return ;	}
					}else if(nMode==1){  //C MODE  
						 Ret=pDIO.ResetALL1(1); 
						// Ret=pDIO.ResetS1();  //source
						 if ( Ret!=1 ){ AfxMessageBox("Outbit[  ResetALL1-1  C mode] Error!!!");	return ;	}
					}else{
						 AfxMessageBox(" Error No 2015 \n Outbit[  ResetALL1   C MOde Select Error] Error!!!");	
                         return ;
					}

					if(nStop==2){
						 AfxMessageBox("   C Mode Run    ->  Stop !!!");
						return ;	
					}

			  	//	pDIO.Delay_us(1000); //test100

					if(nMode==1){ //C MODE
						//Ret=pDIO.PinS1(j,1,0,2,2);//source pin set
						Ret=pDIO.PinS1(a2,1,0,a1,2);//source pin set
						//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
						if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 source pin set] Error!!!");	return ;	}
					}else if(nMode==2){  //R MODE   
					//	Ret=pDIO.PinS1(j,1,0,0,1);//source pin set
						Ret=pDIO.PinS1(a2,1,0,a1,1);//source pin set
						//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
						if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 source pin set] Error!!!");	return ;	}
					  //  Ret=pDIO.PinS1(j,1,1,1,1);//sink pin set
						  Ret=pDIO.PinS1(b2,1,1,b1,1);//sink pin set
						//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
				        if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 sink pin set] Error!!!");	return ;	}
					 }
  
    
Task101:
				 DoEvents();
				 UpdateData(FALSE);
				 if( nModeCha1==0){
			   		 if(j==1){
			 			  pDIO.Delay_us(1000); //test100//simulator    1000   16회 1.6초      100  0.1초 
					 }
				 }
				 if( nModeCha1==1){
				     //pDIO.Delay_us(80); 
				      pDIO.Delay_us(2000); 
				 }

				if(nMode==1){  //C MODE
			 	     pDIO.Delay_us(2000); //MECATEST

				}
				
				
				if(nMode==1){ //C MODE
	                //v=OnRsMeas1(2,nMode); //MES?
					v=OnRsMeas1(2,2); //MES?
				}else if(nMode==2){  //R MODE 
	                //v=OnRsMeas1(2,nMode); //MES?
					v=OnRsMeas1(2,1); //MES?
				}
				//v=999999999999999.0; //MES?simulator
				//v=999.0; //MES?  simulator
                
				if(v<0){								
				 	v=-1;
					AfxMessageBox("  C OR R MODE ?  Error!!!");
					return ; 
				}

		    //	v=9999999999;//simulator

				if( nModeCha1==0){
					nRt1=100;
				}

				if(v<100000000){ 
				   v=v*1000000000000;
				////////// /////////////////////////

					 int ka, kc,k;
					 double  aa,ab,ac;
					 int m;

					 aa=0.0;   ab=0.0;  ac=0.0;
					 kc=0;

					 m=nModeC; //-->
					 ka=(int)nCalC[m][1][0];
	

					 if(ka>100){
						 ka=0;
					 }
					 // v=v*1000000000000;

					 if(v>0){
						 if( v<nCalC[m][2][ka]){
							 if(ka>0){
								 for( k=1; k<ka+1;  k++){
									 if( v< nCalC[m][2][k]){									 
										 if(k==1){
											 //v=nCalC[m][1][k];
											 v=-0.0;
										 }else{
											 aa=(v-nCalC[m][2][k-1])/(nCalC[m][2][k]-nCalC[m][2][k-1]);  
											 ab=nCalC[m][1][k]-nCalC[m][1][k-1];
											 ac=ab*aa+nCalC[m][1][k-1];
										 }
										 kc=1;
										 k=ka;					 
									 }
								 }
							 }
							 v=ac;
						 }else{

						 }
					 }else{
                        v=88888888888; 
					 }



				}//end if(v<100000000){ 



 
				if(v>100000000 || v<0.1){
				 	//v=0;
					//v=v;
					/*
					if(nRt1<3){
						nRt1++;
						if(v==-1.0){
							str.Format("        NO %d,PIN=%d,%d,Value=Communication Error. or  Out of  Range?  Ohmn<%d ?: mode%d=>change ",j,a, b, nMCon3,nModeC);
						}else if(v>9999999999){
					        str.Format("        NO %d,PIN=%d, %d,  Value=Out of Range.  Ohmn  < %d  FAIL : R mode=%d=>change  ",j,a, b, nMCon3,nModeC);						
						}else{
							str.Format("        NO %d,PIN=%d,%d,Value=%.2fOhmn<%d ?: mode%d=>change ",j,a, b,v,nMCon3,nModeC);
						}
					    m_list1.AddString(str);
			            nlist1++;

						if(nRt1==1){
							nModeC=nRsModeC1-1;
						}else if(nRt1==2){
							nModeC=nRsModeC1+1;
						}
						OnRsModeRange(nModeC,1);
					 //	pDIO.Delay_us(100);

	
						goto Task101;
					*/
/*   //sylee20110519  auto mode insert
					if(nRt1<2){
						nRt1++;
						if(v==-1.0){
							str.Format("        NO %d,PIN=%d, Value=Communication Error. or  Out of  Range?  pF <%d ?: C mode%d=>change ",j,a, nMCon3,nModeC);
						}else if(v>9999999999){
					        str.Format("        NO %d,PIN=%d, Value=Out of Range.  pF  < %d  FAIL : C mode=%d=>change  ",j,a, nMCon3,nModeC);						
						}else{
							str.Format("        NO %d,PIN=%d, Value=%.2f  pF< %d ?: C mode%d=>change ",j,a,v,nMCon3,nModeC);
						}

					    m_list1.AddString(str);
			            nlist1++;

						if(nRt1==1){
							if(nRsModeC1==2){
								nModeC=3;
							}else{
								nModeC=2;
							}
						}else{
							nModeC=2;
						}
						
						OnRsModeRange(nModeC,2);
					 //	pDIO.Delay_us(100);

						goto Task101;

					}else{
					///	nRt1=0;
					/*	nModeC=nRsModeR1O;
						if(nModeCha1==1){
							OnRsModeRange(nModeC,1);
						//	pDIO.Delay_us(100);
						}
						*/
/*   //sylee20110519  auto mode insert
					}

				}else{

                    /*
					if( nModeCha1==1 ){
						if( nRt1>0 ){
							
							 if( v<nMCon3 ){
								str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d OK : mode=%d",j,a, b,v,nMCon3,nModeC);						 
							}else{
								str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d FAIL: mode=%d",j,a, b,v,nMCon3,nModeC);
							}
					        m_list1.AddString(str);
			                nlist1++;


							nRt1=0;
							 
						 	nModeC=nRsModeR1O;
						 	OnRsModeRange(nModeC,1);
						//	pDIO.Delay_us(10);
						}
					}
					*/

					//if( nRsMode2 ==2  || nRsMode2 ==3 ){
					//	 v=v*1000000000000;
				    //}


/*   //sylee20110519  auto mode insert

				}//end  


 		
          //  v=100; //simulator

			//int nMCon3,	nMCon2;
/*   //sylee20110519  auto mode insert

			if( (v>0.1) && (v<nMCon3) ) {//ok 				
			//	v=100;	
				nF1=0;
			}else{ //fail
 				nF1=1;
				nCo=nCo++;;
			}

 	         //if( nModeCha1==0 ){


			if( nRt1>0 ){
				if( nF1==0 ){
					str.Format(" NO %d,    PIN=%d,  Value=%.2f pF  < %d  OK  : C mode=%d  ",j,a,v,nMCon3 ,nModeC); 
				}else{
					if(v==-1.0){
						str.Format(" NO %d,    PIN=%d,  Value=Communicaton Error.  or   Out of Range ?   pF  < %d  FAIL : C mode=%d  ",j,a, nMCon3,nModeC);
					}else if(v>9999999999){
						str.Format(" NO %d,    PIN=%d,  Value=Out of Range.  pF  < %d  FAIL : C mode=%d   ",j,a,  nMCon3,nModeC);
					}else{
						str.Format(" NO %d,    PIN=%d, Value=%.2f pF  < %d  FAIL : C mode=%d  ",j,a, v,nMCon3,nModeC);
					}
				}

			}else{
        		if( nF1==0 ){
					str.Format(" NO %d,    PIN=%d,    Value=%.2f pF  < %d  OK  : C mode=%d  ",j,a, v,nMCon3 ,nModeC); 
				}else{
					if(v==-1.0){
						str.Format(" NO %d,    PIN=%d, Value=Communicaton Error.  or   Out of Range ?   pF  < %d  FAIL : C  mode=%d  ",j,a,  nMCon3,nModeC);
					}else if(v>9999999999){
						str.Format(" NO %d,    PIN=%d, Value=Out of Range.  pF  < %d  FAIL :  C mode=%d  ",j,a,  nMCon3,nModeC);
					}else{
						str.Format(" NO %d,    PIN=%d,  Value=%.2f pF  < %d  FAIL : C mode=%d  ",j,a, v,nMCon3,nModeC);
					}
				}
			}

				  
				if( nModeCha1==1 ){
					if( nRt1>0 ){
						
						/*if( v<nMCon3 ){
							str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d OK : mode=%d",j,a, b,v,nMCon3,nModeC);						 
						}else{
							str.Format("* NO %d,PIN=%d,%d, Value=%.2fOhmn < %d FAIL: mode=%d",j,a, b,v,nMCon3,nModeC);
						}
					    m_list1.AddString(str);
			            nlist1++;
                        */ 
/*   //sylee20110519  auto mode insert
						nRt1=0;
						 
						nModeC=nRsModeC1;
						OnRsModeRange(nModeC,2);
					//	pDIO.Delay_us(10);
					}
				}
					 

		
 		 		m_list1.AddString(str);
				nlist1++;
			 //}
    }   

	 str.Format("                                     " ); 
	 m_list1.AddString(str);
	 nlist1++;
 
	 nF1=c-nCo;

	 //str.Format("      : OPen Error = %d EA ,       OK = %d EA  " ,nCo,nF1); 
	 //m_list1.AddString(str);
	 //nlist1++;

	 str.Format("  **** C END ****   " ); 
	 m_list1.AddString(str);
	 nlist1++;

	 if( nCo ==0){
		str.Format("            ==> *SUCCESS  : Error = %d EA ,   OK = %d EA      " ,nCo,nF1); 
	 }else{
		str.Format("            ==> *FAIL : Error = %d EA ,    OK = %d EA     " ,nCo,nF1); 
	 }

 	m_list1.InsertString(1, str);
	nlist1++;
 
    Ret=pDIO.ResetInit(); 
    if ( Ret!=1 ){ AfxMessageBox("Outbit[ResetInit] Error!!!");	return ;	}

    AfxMessageBox("    *** C Test  End***  ");
	
}

*/  //sylee20110519  auto mode insert


//=============================================================================================


/*   auto  //sylee20110425  
void CModeMan17::OnClickButton1001() 
{
  	CString   str;
	int a, a1,a2,b,b1,b2, c,  e, i, ec1, es1;
	double dVal[5],d;
	int nMode; 
	int  Ret,k,j;
	double v; 
	int nS1,nF1,nCo;

	nS1=0;
	nF1=0;
    nlist1=0;
	nCo=0;

    OnRsModeRange(11,1);  //MODE Range(0~10)

	//if( nlist1 >50){
		m_list1.ResetContent();
	//	nlist1=0;

//	}

			 
 
//	 e=RunInfo05.m_nCon;
//	 ec1=0; 
//   es1=SysInfo05.m_nConEr;
 
 	 c =  PinDataP1[0][2] ;  // save해야 한다 


	 str.Format("  **** OPen  Test    Start :    Net List   Total No.= %d   ", c); 
 	 m_list1.AddString(str);
	 nlist1++;
	 str.Format("                                                                   " ); 
 	 m_list1.AddString(str);
	 nlist1++;

 	 if(c<1){
		  AfxMessageBox("    *** Open Test  Net list  ?   ***  ");
 		 return ;//error
	 }
   
	nStop=0;
	nMode=2;//sylee

 
	//if( nRsMode2 < 1  ||  nRsMode2 > 20){ //sylee
    //    AfxMessageBox(" Errro No 1023, \n Mode Select Please !   ( 1~20 )    Testing Start Error!!!");
	//	return ; 
	 
	//}else if( nRsMode2<11){
	//	nMode=1;//sylee   c		
	//}else if( nRsMode2<21){  
	//	nMode=2;//sylee  r	 
//	}
 
	//-----------------------------------------
  
 
 		 for( j=1; j<(c+1); j++){
		//i=9;
		 //-----------------------------------------
		          a=PinDataP1[j][0];
				  b=PinDataP1[j][1];
				      a1=nPcb1[a][0];//pcb no  0~
					  a2=nPcb1[a][1];//pin no  0~63  type b
			          b1=nPcb1[b][0];//pcb no
					  b2=nPcb1[b][1];//pin no

				//	Sleep(0);	
			 
					if(nMode==2){  //R MODE
						 Ret=pDIO.ResetALL1(2); 
						// Ret=pDIO.ResetS1();  //source
						 if ( Ret!=1 ){ AfxMessageBox("Outbit[  ResetALL1-2  R mode] Error!!!");	return ;	}
					}else if(nMode==1){  //C MODE  
						 Ret=pDIO.ResetALL1(1); 
						// Ret=pDIO.ResetS1();  //source
						 if ( Ret!=1 ){ AfxMessageBox("Outbit[  ResetALL1-1 C mode] Error!!!");	return ;	}
					}else{
						 AfxMessageBox(" Error No 2015 \n Outbit[  ResetALL1   MOde Select Error] Error!!!");	
                         return ;
					}

					if(nStop==2){
						 AfxMessageBox("  Calibration Mode Run    ->  Stop !!!");
						return ;	
					}

				//	pDIO.Delay_us(1000); //test100

					if(nMode==1){ //C MODE
						Ret=pDIO.PinS1(j,1,0,2,2);//source pin set
						//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
						if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 source pin set] Error!!!");	return ;	}
					}else  if(nMode==2){  //R MODE   
					//	Ret=pDIO.PinS1(j,1,0,0,1);//source pin set
						Ret=pDIO.PinS1(a2,1,0,a1,1);//source pin set
						//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
						if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 source pin set] Error!!!");	return ;	}
					  //  Ret=pDIO.PinS1(j,1,1,1,1);//sink pin set
						  Ret=pDIO.PinS1(b2,1,1,b1,1);//sink pin set
						//  (int a, int b, int c, int d)//a=pin no(0~63) :   b= pin sel=1,disel=0 :    c= sou=0/sink=1    : d=pcbno
				        if ( Ret!=1 ){ AfxMessageBox("Outbit[PinS1 sink pin set] Error!!!");	return ;	}
					 }
				   //pDIO.Delay_us(1000000); //test100
			 
  
                //sylee20110419
					if(j==1){
			 			pDIO.Delay_us(20000); //test100//simulator    1000   16회 1.6초      100  0.1초 
					}

					pDIO.Delay_us(20000); //test100//simulator    1000   16회 1.6초      100  0.1초 
 

				if(nMode==1){ //C MODE
	                //v=OnRsMeas1(2,nMode); //MES?
					v=OnRsMeas1(2,2); //MES?
				}else if(nMode==2){  //R MODE 
	                //v=OnRsMeas1(2,nMode); //MES?
					v=OnRsMeas1(2,1); //MES?
				}
				//v=999999999999999.0; //MES?simulator
				//v=999.0; //MES?  simulator
                
				if(v<0){
				 	v=0;
				}
 
				if(v>100000000){
				 	//v=0;
					//v=v;
				}else{
					if( nRsMode2 ==2  || nRsMode2 ==3 ){
						 v=v*1000000000000;
					}
				}
 				///  기타   추가 해야 한다ㅏㅏ????/?/??

//////////calibartion  change/////////////////////////	
			if(nRsMode2==2){
 
 
					 int ka, kc,k;
					 double  aa,ab,ac;
					 aa=0.0;ab=0.0;ac=0.0;
					 ka=(int)nCalC[m][1][0];
					 kc=0;
					 if(ka>100){
						 ka=0;
					 }
					//	 v=v*1000000000000;
					 if( v<nCalC[m][2][ka]){
						 if(ka>0){
							 for( k=1; k<ka+1;  k++){
								 if( v< nCalC[m][2][k]){									 
									 if(k==1){
										 //v=nCalC[m][1][k];
										 v=0.0;
									 }else{
										 aa=(v-nCalC[m][2][k-1])/(nCalC[m][2][k]-nCalC[m][2][k-1]);  
										 ab=nCalC[m][1][k]-nCalC[m][1][k-1];
										 ac=ab*aa+nCalC[m][1][k-1];
									 }
									 kc=1;
									 k=ka;									 
								 }
							 }
						 }
						 v=ac;
					 }else{

					 }

			}

          //  v=100; //simulator

			//int nMCon3,	nMCon2;

			if( v < nMCon3 ) {//ok 				
			//	v=100;	
				nF1=0;
			}else{ //fail
 				nF1=1;
				nCo=nCo++;;
			}


//////////calibartion  change/////////////////////////	
        	if( nF1==0){
			   str.Format(" NO %d,    PIN=%d, %d,      Value=%.2f Ohmn  < %d  OK  ",j,a, b,v,nMCon3); 
			}else{
                str.Format(" NO %d,    PIN=%d, %d,     Value=%.2f Ohmn > %d  FAIL  ",j,a, b,v,nMCon3);

			}

 



 		 	 m_list1.AddString(str);
			 nlist1++;



		//	  k=i+4;//sylee
			  //-----------------------------------------
		//	  nData[k][j]= v   ;//test temp   측정 값으로 대체 해댜 한다..
			  //sylee  nData[k][j]=j*1.1*k;//test temp   측정 값으로 대체 해댜 한다..

    	  DoEvents();
	 

   }  //for( j=0; j<64; j++){  end

	 str.Format("                                     " ); 
	 m_list1.AddString(str);
	 nlist1++;
 
	 nF1=c-nCo;

	 //str.Format("      : OPen Error = %d EA ,       OK = %d EA  " ,nCo,nF1); 
	 //m_list1.AddString(str);
	 //nlist1++;

	 str.Format("  **** OPen END ****   " ); 
	 m_list1.AddString(str);
	 nlist1++;

	 if( nCo ==0){
		str.Format("            ==> *SUCCESS  : Error = %d EA ,   OK = %d EA      " ,nCo,nF1); 
	 }else{
		str.Format("            ==> *FAIL : Error = %d EA ,    OK = %d EA     " ,nCo,nF1); 
	 }

 	m_list1.InsertString(1, str);
	nlist1++;
 
    Ret=pDIO.ResetInit(); 
    if ( Ret!=1 ){ AfxMessageBox("Outbit[ResetInit] Error!!!");	return ;	}


    AfxMessageBox("    *** Open Test  End***  ");

	
}




 */ //sylee20110425


void CModeMan17::OnClickButton103() 
{ 
  nStop2=2;
}



void CModeMan17::OnClickOk() 
{
 
	int i;
	i=9;
    nStop2=2;

    OnRsModeRange(2,2);  //MODE Range(0~10)

	CDialog::OnOK(); 
	
}

void CModeMan17::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
   
   Sleep(0);

	UpdateData(FALSE);
	 
}
