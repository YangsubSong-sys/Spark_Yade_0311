#if !defined(AFX_CHILDVIEW7_H__8C393B65_3BF2_46D4_A89D_E6BA97808E0B__INCLUDED_)
#define AFX_CHILDVIEW7_H__8C393B65_3BF2_46D4_A89D_E6BA97808E0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView7.h : header file
//
#include "comidaq.h"//SYLEE
#include "ComiDaqComm.h" //SYLEE
#include "commandbutton.h"
#include "A_Define1.h"
#include "Comi_Dio_InOut.h"     //son230914  Comi DIO ���� define�� ���� ����� �и��ؼ� include.

#include <math.h>

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define     MAX_LIMIT4W_RNG     20     //sylee230520
 
//-----------------------
//son �������� 
//-----------------------
extern int g_nLogOnUser1;
extern int g_nInitFlag1,g_nInitFlag2; 
extern double nCalC[30][10][100];



//son211112 Unitech �䱸����. 4W_Setup_A.txt�� ����ȸ��, BarCode ���� �߰� ��û
#define MAX_BARCODE_STR			30
#define MAX_MODELNAME_STR       30      //son231026
#define MAX_LOTNAME_STR         30      //son231026
#define MAX_SHEETNO_STR         30      //son231026

class C4wSamplingInfo       
{
public:
	short  wSheetNo;		                // �ش� d4WLog2 R�� sample Sheet��ȣ (�ҷ��� ��쵵 ����. ���� sample Run Cnt�� ��ǰ�� ī��Ʈ)
	char   strBarCode[MAX_BARCODE_STR+1];	// �ش� d4WLog2 R�� sample �������� BarCode string ����

public:
	C4wSamplingInfo()
	{
		wSheetNo = 0;
		strcpy(strBarCode, "");
	}
};

//son211112 Unitech �䱸����. 4W_Setup_A.txt�� step, BarCode ���� �߰� ��û
//extern C4wSamplingInfo	g_4wSampling[MAX_DUT][MAX_4W_SAMPLE+1]; //son211008

//son220124 ������ �䱸����.  d:\log4w\������ LotName �߰� 
extern char   g_strLotName[MAX_BARCODE_STR+1];;	//son220124 BarCode.txt �� Lot Name ����

 
//-----------------------
//son define,  enum 
//-----------------------
#define dEtnStep    3000  //estimation
#define MAX_DATA	50	
#define MAX_CH		10

#define COMI_SD414	0xB414
#define COMI_SD424	0xB424
extern CComiDaq	g_ComiDaq2; //son201028 ChildView1�� �����ϱ� ���� ��������� �۷κ������� ����

#define VIEW7_TIMER_VIEW_INIT           10
#define VIEW7_TIMER_PROCESS             20
#define VIEW7_TIMER_INIT                30   
#define VIEW7_TIMER_PRINT               40   
#define VIEW7_TIMER_SCREEN_REFRESH      50
#define VIEW7_TIMER_SELF_TR             60
#define VIEW7_TIMER_DIO                 70
#define VIEW7_TIMER_4W_LOG              80


#define		MAX_ERR_COUNT		5000            //son210712

// Output Data
//  :  Short Test���� Ȱ���ϱ� ���� Open Test ��� ErrList
enum	OPEN_TEST_OUTPUT_LOC { 	OP_TO_FIRST_PIN,	// 0		//son210712
								OP_TO_TEST_PIN,	    // 1
								OP_TO_reserved1,	// 2
								OP_TO_NUM		    // 3
								
};

//son nOpenErrList[��ǥ��][] ���� ���
enum	OPEN_ERR_LOC	{	
    OPERR_START = 0,	// 0        //son210712
	OPERR_END   = 1,	// 1
	OPERR_NUM   = 2		// 2
};


enum CSV_4W_DRIVE_TYPE {         //son200821-RESHEET��ɺ���
    CSV_4W_ALL = 0,
    CSV_4W_Z_DRV = 1,
    CSV_4W_D_DRV = 2
};

//son nRunOld_xxxxx  status ǥ�ÿ� ���.  previous status
enum DIO_ON_OFF {
    D_ON  = 1,
    D_OFF = 2,
};
    




//son for m_Label700 . CChildView7::DisplayRes1_BigLabel(a)      //son200827
enum TEST_DISPLAY_TYPE {
    TEST_DIS_IDLE  = 0,  
    TEST_DIS_PASS  = 1,
    TEST_DIS_O_S   = 2,  // Open + Short 
    TEST_DIS_OPEN  = 3,
    TEST_DIS_SHORT = 4,
    TEST_DIS_LEAK  = 5,
    TEST_DIS_4W    = 6,
    TEST_DIS_FAIL  = 7, 
    TEST_DIS_RUN   = 8,
    TEST_DIS_READY = 9,
    TEST_DIS_ERROR = 14
};


//son220823_4 for nBlockRes[nCellNo], nReSheetRes[nCellNo], GraphDisplayBlock(x, y)�� y
enum BLOCK_RES_TYPE {
    BLOCK_RES_INIT0         = 0,    //son RGB(255, 255, 255) White
    BLOCK_RES_PASS          = 1,    //son RGB(0, 255, 0)     Green
    BLOCK_RES_OPEN          = 2,    //son RGB(255, 255, 0)   Yellow
    BLOCK_RES_SHORT         = 3,    //son RGB(255, 0, 0)     Red
    BLOCK_RES_O_S           = 4,    //son RGB(0, 0, 255)     Blue
    BLOCK_RES_NO_TEST       = 5,    //son RGB(0, 110, 110)   Dark Green (Piece Disable)
    BLOCK_RES_UNDEF         = 6,    //son RGB(20, 240, 96)   ���λ� 
    BLOCK_RES_RUN_FAIL      = 7,    //son RGB(255, 255, 255) White
    BLOCK_RES_4W            = 8,    //son RGB(255, 0, 255)   Magenta
    BLOCK_RES_PIECE_DISABLE = 9,    //son RGB(128, 128, 0)   ����, ������ �̻��. BLOCK_RES_NO_TEST ���.
    BLOCK_RES_SPARK         = 10
                                    //son RGB(255, 176, 255) ������. 4W Sampling�� ��.

};


//int nAutoClear1;


/////////////////////////////////////////////////////////////////////////////
// CChildView7 form view

class CChildView7 : public CFormView
{

//protected:

public:

    // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView7)

//#ifdef __NANOSYS__CUSTOMER   //SYLEE201203
//son240905 #if (defined(__NANOSYS__CUSTOMER) || defined(__GTS__CUSTOMER))
#if (defined(__MULTI_CURRENT__) || defined(__GTS__CUSTOMER))   //son240905 __MULTI_CURRENT__ �̸� ������ ���� ����ϵ��� ĭ�� ���� UI�� ����
    enum { IDD = IDD_Form_View71 };
#else
    #ifdef	__AUTO_COUNT_DISPLAY_DISABLE__    //sylee230125
    //	#if(defined(__SST__CUSTOMER) || defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) )  //sylee230125
		enum { IDD = IDD_Form_View72 }; //son FailCount ���� UI
	#else
		enum { IDD = IDD_Form_View7 };	
	#endif
#endif	
    CChildView7(); 

public:

    //CComiDaq	m_ComiDaq2;     //son201028 ChildView1�� �����ϱ� ���� ��������� �۷κ������� ����
    int nLvModelRealR;//sylee180922-1
	short nCurrenMode1,nCurrenMode2;//sylee181005-3


public: 
    
	int nHalfPin1;//SYLEE180724-1 
	short n4WlogFlagOn1;//sylee170808-2
 	short n4WlogFlagCo1;//sylee170808-2
	CString str71;//sylee170808-2 4w 
	char fName71[200];//sylee170808-2 4w 
    void  n4W_TotalFile1(); //  //sylee170808-2

	int nErrPin1O[Def_SwitchMaxPin],   nErrPin1S[Def_SwitchMaxPin], nErrPin1S0[Def_SwitchMaxPin];//SYLEE170810-1
	bool nErrPin1O2[Def_SwitchMaxPin],   nErrPin1S2[Def_SwitchMaxPin],nErrPinClear;//SYLEE170810-1
    int nErrPinDisO[51][3],nErrPinDisS[51][3];
    short t2;//sylee170810-1
    int  OnErrPin_Rank1(); //sylee170810-1

	short n4wFail1Old[DEF_MAX_PIECE1];//sylee160602-5       //son220823_6
	int  n4wFail1OldCo1;//sylee160602-5

////////////////////////////////////////////////////////
	int nFlag_4W_Use1;  //sylee160528-1
 	int	nFastHv1RunStep;//sylee150415 fasthv
	int	nFastHv1AdRun;//sylee150415 fasthv
	int	nFastHv1AdReturn;//sylee150415 fasthv
	int nFastHv1PieceReturn;// 
	int nFastHv1Piece[2001];//piecemax=2000
	int nFastHv1_LVAd1;//sylee150417-5 
	int nFastHv1_LVAd2;//sylee150605
	int nFastHv1_LV_2;//sylee170112
//sylee200622 	int nFastHv1_HVAd1;//sylee150417-5  
//sylee200622   LEAK �˻��   LV SHORT ��ȯ ������ Mohm���� 100Kohm���� ����.  
	int nFastHv1_HV_VSet;//sylee150420-2
/////////////////////////////////////////////////////////
    int nEstEnableHv;//estimation //sylee150407
    int nEstEnableHr;//estimation //sylee150407

    //son230914 int nDio[8];    
	int nDio[DIN_NUM]; 	    //son230914 16: DIN_NUM
	                        //  nDio[]�� ���� ���� �ڼ��� ������ Comi_Dio_InOut.h�� �����Ѵ�.
    int nDioS2;//sylee160520  //son ������ -> Handler : ��� IO�� (DOUT_BIT_TYPE  ���)
                              // enum DOUT_CMD_TYPE						
                              // 0x01(1)  PASS	  : PASS 
                              // 0x02(2)  SHORT	  : Short �ҷ�
                              // 0x04(4)  OPEN	  : Open �ҷ�
                              // 0x08(8)  RETEST	  : ��� ��˻� ��ȣ (Block�� ���� �ڸ����� �ٽ� �׽�Ʈ)
                              // 0x10(16) CONFAIL	  : ������ �ҷ�      (���� Hanlder�� �� ��ȣ�� ������ �����Ѵ�.)
                              // 0x20(32) END 	  : 4W Auto Sampling Mode Return ��ȣ
                              //					    �Ϲ����� ��쿡�� END�� ������ �ʴ´�.  block�� ���� ������� �� �� ���� ����.
                              // 0x06(6)  SHORT+OPEN: Short & Open �ҷ�
                              // 0x05(5)  PASS+OPEN : (ACK_SKIP) ABORT/SKIP�� ��񿡼� �޾��� �� ������->���� �ִ� Return ��ȣ 
                                                
		
    CString strLog1;							
	int nFlagAutoPress1;//sylee150413  autopress  //son 0:���� test, 
	short nAutoReTest[MAX_CELL +1];//max piece*block 10001 	//sylee140825 //son240510 Block���� Retest�� Ƚ���� ����� �д�.
	char cDataRS[MAX_CH][MAX_DATA];	
 
	UINT   BytesRead[MAX_CH];
	UINT   BytesWritten[MAX_CH];
	HANDLE  hCom [10];
//SYLEE130222	CRect m_Block[200];

	CRect m_RectBlock[MAX_CELL +1];//sylee170327-1  10001  //son240510 10001 -> MAX_CELL +1
 
	int  nTimeOutSet1,nEtTotalCo1; //estimation
	int  nEtSamplingTime,nEtFileLog,dEtADCSTart,nEtCount1,nInter1 ,delay_time_HV,dEt_EndTime1 ; //estimation

	double  dEtEndOffSet1,dEt_LastPer1,dEtPer1, Weight_B,Weight_C; //estimation
     
 	int  nTimeOutSet1_HR,nEtTotalCo1_HR; //estimation  //SYLEE150407
	int  nEtSamplingTime_HR,nEtFileLog_HR,dEtADCSTart_HR,nEtCount1_HR,nInter1_HR ,delay_time_HV_HR,dEt_EndTime1_HR ; //estimation  //SYLEE150407
	double  dEtEndOffSet1_HR,dEt_LastPer1_HR,dEtPer1_HR, Weight_B_HR,Weight_C_HR; //estimation  //SYLEE150407
//sylee241204      int m_nUseACLeak;	           //hdpark230323
	int m_nSparkDetection[3];		// hdpark230331 0: vlotage, 1: current
                                    //son    val   0: Voltage, 1:Current
                                    //son m_nSparkDetection[0] : Common Spark ���� ��� (Recipe > Spark Detection combo �ڽ�)
                                    //son m_nSparkDetection[1] : leak 1 Spark ���� ��� 
                                    //son m_nSparkDetection[2] : leak 2 Spark ���� ���

public:
    void Init_ComiDaqDevice();          //son201216 ComiDaq ����̽��� ChildView1�� �����ϱ� ���� ComiDaq �ʱ�ȭ �Լ��߰�
    bool Check_DIN_StartResheet();      //son200821-RESHEET��ɺ��� �ű��߰�
    void Process_ViewTimerDio();        //son240502 case VIEW7_TIMER_DIO(70): ó�� ����� �Լ��� �и�.
    void Process_StartTest(int nCurStates);                 //son240502
    void Process_HandlerManualTest(int nCurStates);         //son240502
    void Process_HandlerTargetTest(int nCurStates);         //son240502
    void Process_Test(int nCurStates);                      //son240502
    void Process_StartSkip_Or_StartAbort(int nCurStates);   //son240502
    void Process_Skip_Or_Abort(int nCurStates);             //son240502
    void Comi_DoPutByte(byte bDOUT);                        //son240430
    bool Check_DIN_Val(unsigned short wVal);   
                                        //son240426 START+ABORT+TEST�� ������ ������  ABORT ��ȣ�� �Ͻ������� �߻��ϴ� �������� �߰�                                         
    int  Read_DUTNo(int nDio_DUT1, int nDio_DUT2, int nLine); 
                                       //son240502 Get_DIN_DUTNo(), ComDut1(__LINE__) ����ȣ�� �ڵ带 �ϳ��� �Լ��� ����ȭ. 
    void Get_DIN_DUTNo(int nDio_DUT1, int nDio_DUT2);  //son200092 �ű��߰� : �ڵ� ����ȭ    
    int On_SparkFileLog1_V(int nMaxX, int nMaxY); //sylee200710 NEW_SPARK
	int On_SparkFileLog1_I(int nMaxX, int nMaxY); //sylee200710 NEW_SPARK   
 
	int n4wAutoCal1();//sylee171020-1
	int n4wAutoCal2();//sylee171020-1
	int n4wAutoCal1_Save();//sylee171020-1
	int LogDebug3(CString str1);//sylee1810-1-dut16
	int ComDut1(int nLine);//sylee1810-1-dut16

	void ifLastBlock_Log4WCsv(bool bUseTimer=FALSE);    //son-RESHEET-20200225 �ű� �߰�
	void Log4WCsv(bool bUseTimer, int  nDriveType= CSV_4W_ALL); //son-RESHEET-20200225 �ű� �߰�    //son200821-RESHEET��ɺ���
    //bool check_4WAllBlockPass();                      //son-RESHEET-20200225 �ű� �߰�  //son210420 ����
    
    bool checkAllDUT_4wSampleEnd();         //son200320-4WDUT16 : �ű� �߰�    
                                            //son211217_2 ����
                                            //son220203 4W Retest �ȵȴٴ� GTS ������ ����Ǿ�  20�� 3�� ��� �����ߴ� �� ����


    int ARun201_4w_Logging2_Sheet(int nDriveType= CSV_4W_ALL); //SYLEE150722      //son200821-RESHEET��ɺ���
    int ARun201_4w_Logging3_Sample2();  //SYLEE170114
    int ARun201_4w_Logging3_Sample3();  //SYLEE190220-1
    int ARun201_4w_Logging3_Sample5();  //SYLEE190220-1

    int Log4WCsv_DDrive_ColumnWrite(int nEnd_nBlock); //son201209: 4W Log CSV ���� �Ź� ���� ����. �������̿�Ʈ�� ������� Column ����.
    int Log4WCsv_DDrive_LineWrite(int nEnd_nBlock);   //son201209: 4W Log CSV ���� ����. ���� �ϳ��� READ��� ����ϰ�  Line���� ����. 
    int Log4WCsv_DDrive_ColumnWrite_MST(int nEnd_nBlock); //son231026 �븸 MST �����ؿ� 4W Log. Detailed Column write
    int Log4WCsv_DDrive_LineWrite_NANO(int nEnd_nBlock);  //son231026 NANOSYS�� 4W Log. Detailed Line write

    int Log4WCSV_ZDrive(int nEnd_nBlock, int nType);           //son210215: ������� ���޿� 4W CSV �α� (����. default)
    int Log4WCSV_ZDrive_UltLog(int nEnd_nBlock);     //son210215: ������� ���޿� 4W CSV ULTLog (Samsung Electro-Mecha �䱸����)


	///// WILL TECHNOLOG ///////////////////////
	int nFlagOpenMes1;//SYLEE210309-1  1=USE
	int OpenMes1Step,OpenMes1StepT; //SYLEE210309-1  1~200
	int OpenMes1Mode; //SYLEE210309-1   
	float OpenMes1[201][MAX_PIN];//SYLEE210309-1  // step, nBDLData2Op[][] ����
	                            //son221031 17000 ->  MAX_PIN���� ����
	                            //   1�� piece �� open ������ 57���� �Ѱ� piece�� 300���� �Ѵ� ���� ���̽��� ���´ٸ� 17000�� over �� �� ����
	                            //   32768�� 109�������� ����. open���� *  300�� 32768 ������ error ����ϵ��� ��� ���ϵ�. 
	////////////////////////////////////////////


public:


    int nRunMode_HV_LV;//SYLEE150417-5  1= HV, 2=LV
    int nManualStartFlag; //SYLEE130221JOYTECH
    int nSW_StartFlag; //SYLEE130312JOYTECH
    int nKeyStartFlag; //SYLEE130306JOYTECH
    int m_nFileLog_Error;//sylee130220 error file    //son220504_2 nFileLog1 -> m_nFileLog_Error  �� ����


    char  fName[200],fName2[200],fName3[200],fName4[200],fName5[200],fName6[200],fName7[200];   //estimation
     

    int	fOpened[10];
    int nPortDef1;
    int nRunOld_Start;  //START							//son nRunold1 -> nRunOld_Start
                                                        //    START+TEST+ABORT+SKIP  OnStart1() ���� �� 1�� ����
                                                        //    START+ABORT ������ 1�� ����

    int nRunOld_Test;  	//TEST							//son nRunold2 -> nRunOld_Test
                                                        //son  only TEST 0 �� �� ���� 2(D_OFF)�� ����.
                                                        //son START+TEST+ABORT+SKIP  OnStart1() ���� ��  1�� ����
                                                        
    int nRunOld_Abort;  //ABORT							//son nRunold3 -> nRunOld_Abort
                                                        //son  START+ABORT �϶� �� ���� 1�� ��. 
                                                        //son  ABORT 0 �� �� ���� 2(D_OFF)�� ����.
                                                        
    int nRunOld_Skip;   //SKIP							//son nRunold4 -> nRunOld_Skip
                                                        //son  SKIP 0 �� �� ���� 2(D_OFF)�� ����.

    int nRunOld_StartTest;  //START+TEST				//son nRunold11 -> nRunOld_StartTest 
                                                        //son  START 0 & TEST 0 �� �� ���� 2(D_OFF)�� ����.
                                                        //son START+TEST+ABORT+SKIP OnStart1() ������   1�� ����
                                                            
    int nRunOld_StartAbort;  //START on +  abort on		//son nRunold12 -> nRunOld_StartAbort 
                                                        //son  START 0 & ABORT 0 �� �� ���� 2(D_OFF)�� ����.
                                                        //son START+TEST+ABORT+SKIP OnStart1()������  1�� ����
                                            //***		//son  START+ABORT �� Block skip �ϸ鼭 1�� ���� ***
                                                                    
    int nRunOld_AbortSkip;  //SYLEE130221JOYTECH        //son nRunold13 -> nRunOld_StartAbort_JoyTech 
                                                        //son240503 nRunOld_StartAbort_JoyTech -> nRunOld_AbortSkip
                                                        //son ABORT+SKIP ���Žÿ� 1�� ����


    int nRunOld_StartSkip;  //SYLEE130304 //START on +  skip on		
                                                        //son nRunold14 -> nRunOld_StartSkip
                                                        //son  START 0 & SKIP 0 �� �� ���� 2(D_OFF)�� ����.
                                                        //son START+TEST+ABORT+SKIP OnStart1 ������ 1�� ����
                                            //***		//son  START+SKIP ��  Block skip �ϸ鼭 1�� ���� ***

    int nRunOld_AutoPress;  //SYLEE150604  //autopress	//son nRunold15 -> nRunOld_AutoPress

    int nMode;

    int nDisplayRef1;
    int nRsMode2;
    int nTest1;//test

    int nBlockCountP;//block ó�� ������    
    int nBlockCountS;//block �ִ� ������  

    int nComSTart; //0=idle, 1= remode
    int nComDioCheck;
    //int nCom

    int nDisPMaxX, nDisPMaxY;
    int nBlockMaxX,nBlockMaxY, nBlockXGap, nBlockYGap, nBlockMax ;
    int nPieceMaxX,nPieceMaxY, nPieceXGap, nPieceYGap, nPieceMax ;
    int nCellMaxX,nCellMaxY, nCellXGap, nCellYGap;
    int nCellMax;	//son Block * Piece ����

    int nLine1, nLine2;
    int nlist1;  

    int	nCycleFlag ;//sylee20120818
    int nFileLoop;
    int OnRecipeErrorCheckFlag;
     
    int n4WRecheckDisable; //sylee190528-1

	// *************************************************
	// son210712
	// *************************************************
//	int m_nshHVIADCFlag;
//	int m_nshDelay;
//	int m_nshCC;
//	int m_nshIR;
	int m_nCurMod1;
	int m_nCurMode1;
	double m_dCurProRv1;
	int m_nCurPStep1;
	int m_nCurVSet;
	double m_dCurISet;
//	int m_nFailL1CoPiece;
//	int m_nSparkUse;
//	int m_nShortTimeoutType;
//	int m_nShortErrorDelay;
	int m_nopCC;
	int m_nopIR;
	int m_nopFil;
	double m_dISetPercentHigh;
	double m_dISetPercentLow;
	double m_dVBSetPercentHigh;
	double m_dVBSetPercentLow;
	int m_nOpenTimeDelay;

	// Temporary OpenErrData. 
	//  : 1�ܰ�, 2�ܰ� OpenCheck �Ϸ�ñ��� �̰� ����ϰ� Ȯ���� �Ǹ�
	//     �ش� piece�� nOpenErrData[piece]��  �������� �����Ѵ�.
	int		m_waTempOpenErrData[MAX_ERR_COUNT][OP_TO_NUM];	                            //son210712
	int		m_wTempOpenErrCnt;			// ���� Net�� Temporary OpenErr count           //son210712
	BOOL	m_check_bChainTest;                                                         //son210712


public:


    int nOpenMaxCo;//sylee211122

    int nFlagInsul1;//sylee170816-1

    void A_Language();//sylee151028
    int  ARun201_O11_Finish(  int m_nMode1 );//sylee150427
    int  ARun201_O11_SetStart(  int m_nMode1 );//sylee150427
    int  ARun201_S2_SQ2_HV_Re_VB_Finish();//sylee150427 

    __int64 GetMicroSecond(); 
    __int64 GetuSecond();  
    __int64 Get_uSecond1(); 
    __int64 GetSecond1();  


    void OnRecipeErrorCheck();//sylee140127
     
    int  DisplayStatus_TestMode(int nMode); //SYLEE120925   //son220824
    int AHV_ADCI_Read1(double nProRv1 ); //sylee120919  
    //SYLEE180416-2 int DisplayNo_Map1(); //sylee130821
    int  DisplayBlock_Map1();  //sylee180416-2    
    //int  DisplayBlock_ReadMode1();//sylee180502-1  //son200510-4WLOG: ����. FileSysInfo�� �̵�

    int AutoSelf_Fun1_OpenLvShort1();//sylee140824
    int AutoSelf_OPen1_Log1(int nMode);
    int AutoSelf_OPen1_Run1();//sylee140824
    int AutoSelf_LvShort1_Log1(int nMode);
    int AutoSelf_LvShort1_Run1();//sylee140824
    int Log_Event1(int nType);//sylee150228

    int AHV_ADCI_Read1_OneToM(double nProRv1 , double nDelay10, int nntype ); //sylee121016
    int AHV_ADCI_Read1_OneToM(double nProRv1 , double nDelay10 ); //sylee121016
    int AHV_ADCI_Read1_Wait1(double nProRv1 , double nDelay10 ); //sylee121016
    int ALV_ADCI_Read1_OneToM(double nProRv1 , double nDelay10 ); //sylee121016
    int AHV_ADCI_Read1_Avg1(double nProRv1 , double nDelay10 , int nntype ); //sylee121116
    int AHV_ADCI_Read1_Avg2(); //sylee230811
    int ALV_ADCI_Read1_Avg1(double nProRv1 , double nDelay10 , int nntype ); //sylee201205
    int AHV_ADCI_Read1_EST1(double nProRv1 , double nDelay10 ); //sylee121228   //estimation
     

    int AHV_EST1(double nProRv1  ,int nType1  ); //sylee130131 //estimation
    int AHV_EST1_RUN_LOG1(double nProRv1  ,int nType1  ); //sylee130131
    int AHV_EST1_RUN_1(double nProRv1  ,int nType1  ); //sylee130131 
    int AHV_EST1_MODEL(double nProRv1  ,int nType1  ); //sylee130131
     
    int AHV_EST1_SET1(); //sylee130107  //estimation
    int AHV_ADCI_Read1_LV1(double nProRv1 ,int nCase); //sylee120921 
    int ARun201_O11(int nMod1, int m_nMode1, double nProRv1, int nPStep1); //SYLEE20120326  //SETUPCALIBRATION 
    int ARun201_O11_Log1(int nMode);  //sylee210309

    int ARun201_O31_F(int nMod1, int m_nMode1, double nProRv1, int nPStep1); //SYLEE180412 FAKEOPEN ADD
    int ARun201_O11_SelfCheck1( int nVRel, double nVSet, double nISet, int nFil );

 
    //========================================================================================================  
    int ARun201_S2_SQ2_HV_Re_VB(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType); //sylee121221  //sylee121025 
    int ARun201_S2_SQ2_HV_Re_VB1(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType); //sylee200813 
    int ARun201_S2_SQ2_HV_Re_VB1_AC(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType); // hdpark230228 ac leak
    int ARun201_S2_SQ2_HV_N1(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType);  //sylee200813 
    int ARun201_S2_HV_2N1(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType); //sylee150423-3
    int ARun201_Short_Model1();  //sylee150622
    int ARun201_S2_SQ2_LV_Re_VB(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType, int nLeakMode=0); //sylee121221  //sylee121025 //son230202 
    int ARun201_LV_OneModel(int nMod1, int m_nMode1, double nProRv1, int nPStep1, short nProType);   //sylee121221 //sylee121025 
     
    int ARun201_4w_Net_Sort1(); //sylee191122-1 

     
    int ARun201_4w_3(int nPStep1); //SYLEE200515-PATCH
	int ARun201_4W_Old_230420(int nPStep1); //SYLEE230420
    int ARun201_4W_New_230420(int nPStep1); //SYLEE230420


    int ARun201_4w_R1(double nISet);//sylee150710
    int ARun201_4w_R2(double nISet , int nStep );//sylee200909
    //son220419_3 int ARun201_4w_R_Cal1(int m_nMode1, int part);  //sylee150710   //son220412 part �߰� //son220429_3 moved to Globals.cpp
	
	int ARun201_OPEN_Mea1(int nStep, double nProRv1, int ip, int nCC, int nFil, int nIR, int nISet, int nRCount, int nDelay1);//sylee211122

    //son211112 GTS Unitech. 4W_Setup_A.txt�� step, BarCode ���� �߰� ��û
    //son240622 int ReadBarCode_fromHandlerDrive(char* strBarCode, char* strLotName);   //son220124 strLtName �߰�. for ������
    int GetBarCode(char* strBarCode); //son240622 ReadBarCode_fromHandlerDrive() -> GetdBarCode()�� ����.
                                      // 4W Csv �����̳� 4W_Setup_A.txt �����ÿ��� GetdBarCode() �� ������ Ȱ�븸 �Ѵ�.
                                      // ���� BarCode.txt�� read���� ����
                            
    int ReadBarCode_TXT();  //son240622 BarCode.txt�� START+TEST �۽� ������ Main PC���� ����. sheetNo���� ���� ����ġ�� �ִ��� ��������
                            //   ������� START+TEST �������ڸ��� ReadBarCode_TXT() ȣ���ؼ� BarCode.txt�� read�� ������ ������ �д�.
            
    int _ReadBarCode_fromHandlerDrive(char* pStrBarCode, char* pStrLotName,   //son ���� BarCode.txt read �Լ�
                                char* pStrModelName, char* pStrSheetNo, //son230707 for MST ������
                                char* pStrTotalNo);                     //son240109 for MST ������



	// *************************************************
	// son210712
	// *************************************************
    int ARun201_O11_New(int nMod1, int m_nMode1, double nProRv1, int nPStep1);          //son210712
    int OpenTest_forSameNet(int piece, int startPinIdx, int endPinIdx);                 //son210712
    int OpenTest_forSameNet_Phase1Phase2(int piece, int startPinIdx, int endPinIdx);    //son210712
    int OpenTest_forSameNet_Phase1Chain(int piece, int startPinIdx, int endPinIdx);     //son210712
    int CheckOpen();                                                                    //son210712
    int MeasureOpen();                                                                  //son210712 

    void SaveOpenErrorData(int nPieceNo, int nPinA, int nPinB);     //son210712
	void VB_SwOn(int nPieceNo, short wVbPin);       //son210712
	void VC_SwOn(int nPieceNo, short wVbPin);       //son210712
	void VB_SwOn(int wVbPin);       //son210712
	void VC_SwOn(int wVbPin);       //son210712

	int nFileSaveLowStartPin(int nLowStartPinNo );  //sylee241108     
    int LogDebug1(int nblock, int nNet);     //SYLEE150812
    int LogDebug2(CString str1) ;    //SYLEE170618

    void OnStart1(); 
	int OnPinMeasure1();//sylee241107

    void A_Run101Init1( int m_nMode1 ); 
    void A_Run101Init3(); 
    void A_Run101Init2( double nVSet1, double nVSet2, double nVSet3, double nVSet4 );
     
    int AInterStart1();
    int AProcess100(); 
    int AProcess101();
    int makeProcess101_ErrorFile_forMainHandler();         //son211217_1 �ű�

    int ContinueFailCheck();  
    int DisplayNo();//sylee120901 
    int Increase_nBlockCountP();        //son220624 OnInitRunP1() -> Increase_nBlockCountP()
    int Display_AutoCount();            //son220624 RunEnd1P1()  -> Display_AutoCount()
	int Sub_Display_AutoCount(); //sylee220331  //son220624 RunEnd1P1_Display_AutoCount1() ->Sub_Display_AutoCount()
    void Count();
    void A_OnInit();  
    void GraphDisplayBlock(int nCell, int nResult);
     
    int  DisplayRes1_BigLabel(int a);   //son220824 DisplayRes1() -> DisplayRes1_BigLabel() 
     
    int PowerCheckError1();//sylee140212
	int	FindLeakFailPin(short wVcPin, short wVbPin);
    void    ResultSpark(int nPin, int nPieceNo, int nMod1, int nPStep1, double nProRv1, double nISet, int m_nMode1);
    int     SetSparkDetectionVolt(double dSetVolt = 15.0, BOOL bUse = TRUE);    // hdpark20240116

    //======================================================
     
    void OnSelfTr1_Result_File(int nDe1Co1);//sylee181117
    void OnSelfTr1_Stop();//stop//sylee181117
    int  OnSelfTr1_ADCI_Read1(double nProRv1,  double nTimeOutSet1 , int nntype );  
   
    void OnSelfTr1_UnSet11();//sylee181117
    int nTr1Count2,nTr1ProRv1,nTr1Delay10;

    int nStartPin,nEndPin,nCount,nDelay,nProPer1,nFlag_Stop,nMaxpin_Half;
    char szText[100];
    CString str;
    int k9Err,nDe1Co1;
    double Rval1;
    short nDe1[5001][10];

    //=======================================================
    int  OnSelfTr1_Set_File1(int type);
    int  OnSelfTr1_Run_File2(int type);

    void Label101_Status_SetCaption(CString str);    //son240712 m_Label101 ��½� ����� ����Ʈ ��� ��� �߰�
    void Label102_Cycle_SetCaption(CString str, bool prtOn=true);     //son240712 m_Label102 ��½� ����� ����Ʈ ��� ��� �߰�


public:

	CLabelControl	m_Label1;
	CLabelControl	m_Label2;
	CLabelControl	m_Label3;
	CLabelControl	m_Label4;
	CLabelControl	m_Label5;
	CLabelControl	m_Label6;
	CLabelControl	m_Label7;
 	CLabelControl	m_Label8;//SYLEE150817
	CLabelControl	m_Label21;//SYLEE150817
	CLabelControl	m_Label9;
    CLabelControl	m_Label10;	

	CLabelControl	m_Label12;
	CLabelControl	m_Label17;	//son-32KPIN-20200212: nReSheetFlag1 �� ������ ���� �߰� 
	CLabelControl	m_Label18;
	CLabelControl	m_Label20;

	CLabelControl	m_Label23;
	CLabelControl	m_Label27;
	CLabelControl	m_Label29;//
 	CLabelControl	m_Label32;//

	CLabelControl	m_Label101;
    CLabelControl	m_Label102;     //son Block test ����
	CLabelControl	m_Label112;     //son Recipe ���� 
	CLabelControl	m_Label115;     //son ������ �߰���ɰ���. ���� �̻��.

	CLabelControl	m_Label131;
    CLabelControl	m_Label132;
    CLabelControl	m_Label133;
	CLabelControl	m_Label134;
    CLabelControl	m_Label135;
    CLabelControl	m_Label136;

	CLabelControl	m_Label201;     //son Total ����
    CLabelControl	m_Label202;     //son Pass ����
	CLabelControl	m_Label203;     //son Fail ����
    CLabelControl	m_Label204;     //son Open ����
	CLabelControl	m_Label205;     //son Short ����
    CLabelControl	m_Label206;     //son O+S ����

	CLabelControl	m_Label301;     
    CLabelControl	m_Label302;     //son Pass %
	CLabelControl	m_Label303;     //son Fail %
    CLabelControl	m_Label304;     //son Open %
	CLabelControl	m_Label305;     //son Short %
    CLabelControl	m_Label306;	    //son O+S %

	CLabelControl	m_Label501;
	
	CLabelControl	m_Label601;
	CLabelControl	m_Label602;
	CLabelControl	m_Label603;
	CLabelControl	m_Label604;

	CLabelControl	m_Label700;     //son DisplayRes1_BigLabel(TEST_DIS_4W)  ����
	CLabelControl	m_Label701; 
    CLabelControl	m_Label702; 
    CLabelControl	m_Label703; 
    CLabelControl	m_Label704; 

	CLabelControl	m_Label1003;
	CLabelControl	m_Label1004;
	CLabelControl	m_Label1015;
	CLabelControl	m_Label1031;
	CLabelControl	m_Label1032;
		
	CLabelControl	m_Label2001;
	CLabelControl	m_Label2002;
	CLabelControl	m_Label2003;
	CLabelControl	m_Label2004;
	CLabelControl	m_Label2005;
 
	CListBox	m_list1;        //son Error, Finish�� �ֿ� ���� ���.

	CCommandButton	m_button1; 
	CCommandButton  m_button2001 ;
    CCommandButton  m_button2003 ;//sylee151028


public:

	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

  
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView7)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
//protected:



public:

	virtual ~CChildView7();
	virtual BOOL OnInitDialog();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



	// Generated message map functions
	//{{AFX_MSG(CChildView7)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton2001();
	afx_msg void OnButton2002();
	afx_msg void OnButton2003();
	afx_msg void OnButton103();
    afx_msg void OnButton105();
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};


extern	CChildView7  pChildView7; 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW7_H__8C393B65_3BF2_46D4_A89D_E6BA97808E0B__INCLUDED_)



