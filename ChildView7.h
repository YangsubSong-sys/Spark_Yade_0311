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
#include "Comi_Dio_InOut.h"     //son230914  Comi DIO 관련 define을 전용 헤더로 분리해서 include.

#include <math.h>

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define     MAX_LIMIT4W_RNG     20     //sylee230520
 
//-----------------------
//son 전역변수 
//-----------------------
extern int g_nLogOnUser1;
extern int g_nInitFlag1,g_nInitFlag2; 
extern double nCalC[30][10][100];



//son211112 Unitech 요구사항. 4W_Setup_A.txt에 수집회차, BarCode 정보 추가 요청
#define MAX_BARCODE_STR			30
#define MAX_MODELNAME_STR       30      //son231026
#define MAX_LOTNAME_STR         30      //son231026
#define MAX_SHEETNO_STR         30      //son231026

class C4wSamplingInfo       
{
public:
	short  wSheetNo;		                // 해당 d4WLog2 R값 sample Sheet번호 (불량인 경우도 포함. 기존 sample Run Cnt는 양품만 카운트)
	char   strBarCode[MAX_BARCODE_STR+1];	// 해당 d4WLog2 R값 sample 수집시의 BarCode string 정보

public:
	C4wSamplingInfo()
	{
		wSheetNo = 0;
		strcpy(strBarCode, "");
	}
};

//son211112 Unitech 요구사항. 4W_Setup_A.txt에 step, BarCode 정보 추가 요청
//extern C4wSamplingInfo	g_4wSampling[MAX_DUT][MAX_4W_SAMPLE+1]; //son211008

//son220124 제이텍 요구사항.  d:\log4w\폴더명에 LotName 추가 
extern char   g_strLotName[MAX_BARCODE_STR+1];;	//son220124 BarCode.txt 내 Lot Name 정보

 
//-----------------------
//son define,  enum 
//-----------------------
#define dEtnStep    3000  //estimation
#define MAX_DATA	50	
#define MAX_CH		10

#define COMI_SD414	0xB414
#define COMI_SD424	0xB424
extern CComiDaq	g_ComiDaq2; //son201028 ChildView1과 공유하기 위해 멤버변수를 글로별변수로 변경

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
//  :  Short Test에서 활용하기 위한 Open Test 결과 ErrList
enum	OPEN_TEST_OUTPUT_LOC { 	OP_TO_FIRST_PIN,	// 0		//son210712
								OP_TO_TEST_PIN,	    // 1
								OP_TO_reserved1,	// 2
								OP_TO_NUM		    // 3
								
};

//son nOpenErrList[대표핀][] 에서 사용
enum	OPEN_ERR_LOC	{	
    OPERR_START = 0,	// 0        //son210712
	OPERR_END   = 1,	// 1
	OPERR_NUM   = 2		// 2
};


enum CSV_4W_DRIVE_TYPE {         //son200821-RESHEET기능보완
    CSV_4W_ALL = 0,
    CSV_4W_Z_DRV = 1,
    CSV_4W_D_DRV = 2
};

//son nRunOld_xxxxx  status 표시에 사용.  previous status
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


//son220823_4 for nBlockRes[nCellNo], nReSheetRes[nCellNo], GraphDisplayBlock(x, y)의 y
enum BLOCK_RES_TYPE {
    BLOCK_RES_INIT0         = 0,    //son RGB(255, 255, 255) White
    BLOCK_RES_PASS          = 1,    //son RGB(0, 255, 0)     Green
    BLOCK_RES_OPEN          = 2,    //son RGB(255, 255, 0)   Yellow
    BLOCK_RES_SHORT         = 3,    //son RGB(255, 0, 0)     Red
    BLOCK_RES_O_S           = 4,    //son RGB(0, 0, 255)     Blue
    BLOCK_RES_NO_TEST       = 5,    //son RGB(0, 110, 110)   Dark Green (Piece Disable)
    BLOCK_RES_UNDEF         = 6,    //son RGB(20, 240, 96)   연두색 
    BLOCK_RES_RUN_FAIL      = 7,    //son RGB(255, 255, 255) White
    BLOCK_RES_4W            = 8,    //son RGB(255, 0, 255)   Magenta
    BLOCK_RES_PIECE_DISABLE = 9,    //son RGB(128, 128, 0)   쑥색, 실질적 미사용. BLOCK_RES_NO_TEST 사용.
    BLOCK_RES_SPARK         = 10
                                    //son RGB(255, 176, 255) 연보라. 4W Sampling일 때.

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
#if (defined(__MULTI_CURRENT__) || defined(__GTS__CUSTOMER))   //son240905 __MULTI_CURRENT__ 이면 전류를 같이 출력하도록 칸을 넓힌 UI로 설정
    enum { IDD = IDD_Form_View71 };
#else
    #ifdef	__AUTO_COUNT_DISPLAY_DISABLE__    //sylee230125
    //	#if(defined(__SST__CUSTOMER) || defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) )  //sylee230125
		enum { IDD = IDD_Form_View72 }; //son FailCount 삭제 UI
	#else
		enum { IDD = IDD_Form_View7 };	
	#endif
#endif	
    CChildView7(); 

public:

    //CComiDaq	m_ComiDaq2;     //son201028 ChildView1과 공유하기 위해 멤버변수를 글로별변수로 변경
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
//sylee200622   LEAK 검사시   LV SHORT 전환 조건을 Mohm에서 100Kohm으로 변경.  
	int nFastHv1_HV_VSet;//sylee150420-2
/////////////////////////////////////////////////////////
    int nEstEnableHv;//estimation //sylee150407
    int nEstEnableHr;//estimation //sylee150407

    //son230914 int nDio[8];    
	int nDio[DIN_NUM]; 	    //son230914 16: DIN_NUM
	                        //  nDio[]에 대한 보다 자세한 설명은 Comi_Dio_InOut.h를 참고한다.
    int nDioS2;//sylee160520  //son 계측기 -> Handler : 출력 IO값 (DOUT_BIT_TYPE  사용)
                              // enum DOUT_CMD_TYPE						
                              // 0x01(1)  PASS	  : PASS 
                              // 0x02(2)  SHORT	  : Short 불량
                              // 0x04(4)  OPEN	  : Open 불량
                              // 0x08(8)  RETEST	  : 블록 재검사 신호 (Block을 같은 자리에서 다시 테스트)
                              // 0x10(16) CONFAIL	  : 연속핀 불량      (메인 Hanlder는 이 신호를 받으면 정지한다.)
                              // 0x20(32) END 	  : 4W Auto Sampling Mode Return 신호
                              //					    일반적인 경우에는 END를 보내지 않는다.  block의 끝을 계측기는 알 수 없기 때문.
                              // 0x06(6)  SHORT+OPEN: Short & Open 불량
                              // 0x05(5)  PASS+OPEN : (ACK_SKIP) ABORT/SKIP을 장비에서 받았을 때 계측기->장비로 주는 Return 신호 
                                                
		
    CString strLog1;							
	int nFlagAutoPress1;//sylee150413  autopress  //son 0:실제 test, 
	short nAutoReTest[MAX_CELL +1];//max piece*block 10001 	//sylee140825 //son240510 Block마다 Retest한 횟수를 기록해 둔다.
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
                                    //son m_nSparkDetection[0] : Common Spark 감지 방식 (Recipe > Spark Detection combo 박스)
                                    //son m_nSparkDetection[1] : leak 1 Spark 감지 방식 
                                    //son m_nSparkDetection[2] : leak 2 Spark 감지 방식

public:
    void Init_ComiDaqDevice();          //son201216 ComiDaq 디바이스를 ChildView1과 공유하기 위한 ComiDaq 초기화 함수추가
    bool Check_DIN_StartResheet();      //son200821-RESHEET기능보완 신규추가
    void Process_ViewTimerDio();        //son240502 case VIEW7_TIMER_DIO(70): 처리 기능을 함수로 분리.
    void Process_StartTest(int nCurStates);                 //son240502
    void Process_HandlerManualTest(int nCurStates);         //son240502
    void Process_HandlerTargetTest(int nCurStates);         //son240502
    void Process_Test(int nCurStates);                      //son240502
    void Process_StartSkip_Or_StartAbort(int nCurStates);   //son240502
    void Process_Skip_Or_Abort(int nCurStates);             //son240502
    void Comi_DoPutByte(byte bDOUT);                        //son240430
    bool Check_DIN_Val(unsigned short wVal);   
                                        //son240426 START+ABORT+TEST가 들어오기 직전에  ABORT 신호만 일시적으로 발생하는 문제관련 추가                                         
    int  Read_DUTNo(int nDio_DUT1, int nDio_DUT2, int nLine); 
                                       //son240502 Get_DIN_DUTNo(), ComDut1(__LINE__) 이중호출 코드를 하나의 함수로 간소화. 
    void Get_DIN_DUTNo(int nDio_DUT1, int nDio_DUT2);  //son200092 신규추가 : 코드 간소화    
    int On_SparkFileLog1_V(int nMaxX, int nMaxY); //sylee200710 NEW_SPARK
	int On_SparkFileLog1_I(int nMaxX, int nMaxY); //sylee200710 NEW_SPARK   
 
	int n4wAutoCal1();//sylee171020-1
	int n4wAutoCal2();//sylee171020-1
	int n4wAutoCal1_Save();//sylee171020-1
	int LogDebug3(CString str1);//sylee1810-1-dut16
	int ComDut1(int nLine);//sylee1810-1-dut16

	void ifLastBlock_Log4WCsv(bool bUseTimer=FALSE);    //son-RESHEET-20200225 신규 추가
	void Log4WCsv(bool bUseTimer, int  nDriveType= CSV_4W_ALL); //son-RESHEET-20200225 신규 추가    //son200821-RESHEET기능보완
    //bool check_4WAllBlockPass();                      //son-RESHEET-20200225 신규 추가  //son210420 삭제
    
    bool checkAllDUT_4wSampleEnd();         //son200320-4WDUT16 : 신규 추가    
                                            //son211217_2 삭제
                                            //son220203 4W Retest 안된다는 GTS 문제점 보고되어  20년 3월 기능 삭제했던 것 복구


    int ARun201_4w_Logging2_Sheet(int nDriveType= CSV_4W_ALL); //SYLEE150722      //son200821-RESHEET기능보완
    int ARun201_4w_Logging3_Sample2();  //SYLEE170114
    int ARun201_4w_Logging3_Sample3();  //SYLEE190220-1
    int ARun201_4w_Logging3_Sample5();  //SYLEE190220-1

    int Log4WCsv_DDrive_ColumnWrite(int nEnd_nBlock); //son201209: 4W Log CSV 파일 매번 새로 생성. 기존바이옵트로 방식으로 Column 누적.
    int Log4WCsv_DDrive_LineWrite(int nEnd_nBlock);   //son201209: 4W Log CSV 파일 재사용. 파일 하나에 READ사와 비슷하게  Line으로 누적. 
    int Log4WCsv_DDrive_ColumnWrite_MST(int nEnd_nBlock); //son231026 대만 MST 유니텍용 4W Log. Detailed Column write
    int Log4WCsv_DDrive_LineWrite_NANO(int nEnd_nBlock);  //son231026 NANOSYS용 4W Log. Detailed Line write

    int Log4WCSV_ZDrive(int nEnd_nBlock, int nType);           //son210215: 메인장비 전달용 4W CSV 로그 (기존. default)
    int Log4WCSV_ZDrive_UltLog(int nEnd_nBlock);     //son210215: 메인장비 전달용 4W CSV ULTLog (Samsung Electro-Mecha 요구사항)


	///// WILL TECHNOLOG ///////////////////////
	int nFlagOpenMes1;//SYLEE210309-1  1=USE
	int OpenMes1Step,OpenMes1StepT; //SYLEE210309-1  1~200
	int OpenMes1Mode; //SYLEE210309-1   
	float OpenMes1[201][MAX_PIN];//SYLEE210309-1  // step, nBDLData2Op[][] 개수
	                            //son221031 17000 ->  MAX_PIN으로 수정
	                            //   1개 piece 당 open 개수가 57개를 넘고 piece가 300개를 넘는 동시 케이스가 나온다면 17000은 over 될 수 있음
	                            //   32768은 109개까지는 가능. open개수 *  300이 32768 넘으면 error 출력하도록 기능 보완됨. 
	////////////////////////////////////////////


public:


    int nRunMode_HV_LV;//SYLEE150417-5  1= HV, 2=LV
    int nManualStartFlag; //SYLEE130221JOYTECH
    int nSW_StartFlag; //SYLEE130312JOYTECH
    int nKeyStartFlag; //SYLEE130306JOYTECH
    int m_nFileLog_Error;//sylee130220 error file    //son220504_2 nFileLog1 -> m_nFileLog_Error  로 변경


    char  fName[200],fName2[200],fName3[200],fName4[200],fName5[200],fName6[200],fName7[200];   //estimation
     

    int	fOpened[10];
    int nPortDef1;
    int nRunOld_Start;  //START							//son nRunold1 -> nRunOld_Start
                                                        //    START+TEST+ABORT+SKIP  OnStart1() 수행 후 1로 변경
                                                        //    START+ABORT 수행후 1로 변경

    int nRunOld_Test;  	//TEST							//son nRunold2 -> nRunOld_Test
                                                        //son  only TEST 0 일 때 값이 2(D_OFF)로 변경.
                                                        //son START+TEST+ABORT+SKIP  OnStart1() 수행 후  1로 변경
                                                        
    int nRunOld_Abort;  //ABORT							//son nRunold3 -> nRunOld_Abort
                                                        //son  START+ABORT 일때 이 값이 1이 됨. 
                                                        //son  ABORT 0 일 때 값이 2(D_OFF)로 변경.
                                                        
    int nRunOld_Skip;   //SKIP							//son nRunold4 -> nRunOld_Skip
                                                        //son  SKIP 0 일 때 값이 2(D_OFF)로 변경.

    int nRunOld_StartTest;  //START+TEST				//son nRunold11 -> nRunOld_StartTest 
                                                        //son  START 0 & TEST 0 일 때 값이 2(D_OFF)로 변경.
                                                        //son START+TEST+ABORT+SKIP OnStart1() 수행후   1로 변경
                                                            
    int nRunOld_StartAbort;  //START on +  abort on		//son nRunold12 -> nRunOld_StartAbort 
                                                        //son  START 0 & ABORT 0 일 때 값이 2(D_OFF)로 변경.
                                                        //son START+TEST+ABORT+SKIP OnStart1()수행후  1로 변경
                                            //***		//son  START+ABORT 로 Block skip 하면서 1로 변경 ***
                                                                    
    int nRunOld_AbortSkip;  //SYLEE130221JOYTECH        //son nRunold13 -> nRunOld_StartAbort_JoyTech 
                                                        //son240503 nRunOld_StartAbort_JoyTech -> nRunOld_AbortSkip
                                                        //son ABORT+SKIP 수신시에 1로 변경


    int nRunOld_StartSkip;  //SYLEE130304 //START on +  skip on		
                                                        //son nRunold14 -> nRunOld_StartSkip
                                                        //son  START 0 & SKIP 0 일 때 값이 2(D_OFF)로 변경.
                                                        //son START+TEST+ABORT+SKIP OnStart1 수행후 1로 변경
                                            //***		//son  START+SKIP 로  Block skip 하면서 1로 변경 ***

    int nRunOld_AutoPress;  //SYLEE150604  //autopress	//son nRunold15 -> nRunOld_AutoPress

    int nMode;

    int nDisplayRef1;
    int nRsMode2;
    int nTest1;//test

    int nBlockCountP;//block 처리 증가값    
    int nBlockCountS;//block 최대 설정값  

    int nComSTart; //0=idle, 1= remode
    int nComDioCheck;
    //int nCom

    int nDisPMaxX, nDisPMaxY;
    int nBlockMaxX,nBlockMaxY, nBlockXGap, nBlockYGap, nBlockMax ;
    int nPieceMaxX,nPieceMaxY, nPieceXGap, nPieceYGap, nPieceMax ;
    int nCellMaxX,nCellMaxY, nCellXGap, nCellYGap;
    int nCellMax;	//son Block * Piece 갯수

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
	//  : 1단계, 2단계 OpenCheck 완료시까지 이걸 사용하고 확정이 되면
	//     해당 piece의 nOpenErrData[piece]에  정식으로 저장한다.
	int		m_waTempOpenErrData[MAX_ERR_COUNT][OP_TO_NUM];	                            //son210712
	int		m_wTempOpenErrCnt;			// 현재 Net의 Temporary OpenErr count           //son210712
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
    //int  DisplayBlock_ReadMode1();//sylee180502-1  //son200510-4WLOG: 삭제. FileSysInfo로 이동

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
    //son220419_3 int ARun201_4w_R_Cal1(int m_nMode1, int part);  //sylee150710   //son220412 part 추가 //son220429_3 moved to Globals.cpp
	
	int ARun201_OPEN_Mea1(int nStep, double nProRv1, int ip, int nCC, int nFil, int nIR, int nISet, int nRCount, int nDelay1);//sylee211122

    //son211112 GTS Unitech. 4W_Setup_A.txt에 step, BarCode 정보 추가 요청
    //son240622 int ReadBarCode_fromHandlerDrive(char* strBarCode, char* strLotName);   //son220124 strLtName 추가. for 제이텍
    int GetBarCode(char* strBarCode); //son240622 ReadBarCode_fromHandlerDrive() -> GetdBarCode()로 수정.
                                      // 4W Csv 파일이나 4W_Setup_A.txt 생성시에는 GetdBarCode() 로 정보를 활용만 한다.
                                      // 새로 BarCode.txt를 read하지 않음
                            
    int ReadBarCode_TXT();  //son240622 BarCode.txt는 START+TEST 송신 직전에 Main PC에서 생성. sheetNo등의 정보 불일치를 최대한 막기위해
                            //   계측기는 START+TEST 수신하자마자 ReadBarCode_TXT() 호출해서 BarCode.txt를 read한 정보를 보관해 둔다.
            
    int _ReadBarCode_fromHandlerDrive(char* pStrBarCode, char* pStrLotName,   //son 실제 BarCode.txt read 함수
                                char* pStrModelName, char* pStrSheetNo, //son230707 for MST 유니텍
                                char* pStrTotalNo);                     //son240109 for MST 유니텍



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
    int makeProcess101_ErrorFile_forMainHandler();         //son211217_1 신규

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

    void Label101_Status_SetCaption(CString str);    //son240712 m_Label101 출력시 디버그 프린트 출력 기능 추가
    void Label102_Cycle_SetCaption(CString str, bool prtOn=true);     //son240712 m_Label102 출력시 디버그 프린트 출력 기능 추가


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
	CLabelControl	m_Label17;	//son-32KPIN-20200212: nReSheetFlag1 값 추적을 위해 추가 
	CLabelControl	m_Label18;
	CLabelControl	m_Label20;

	CLabelControl	m_Label23;
	CLabelControl	m_Label27;
	CLabelControl	m_Label29;//
 	CLabelControl	m_Label32;//

	CLabelControl	m_Label101;
    CLabelControl	m_Label102;     //son Block test 상태
	CLabelControl	m_Label112;     //son Recipe 조건 
	CLabelControl	m_Label115;     //son 조이텍 추가기능관련. 현재 미사용.

	CLabelControl	m_Label131;
    CLabelControl	m_Label132;
    CLabelControl	m_Label133;
	CLabelControl	m_Label134;
    CLabelControl	m_Label135;
    CLabelControl	m_Label136;

	CLabelControl	m_Label201;     //son Total 개수
    CLabelControl	m_Label202;     //son Pass 개수
	CLabelControl	m_Label203;     //son Fail 개수
    CLabelControl	m_Label204;     //son Open 개수
	CLabelControl	m_Label205;     //son Short 개수
    CLabelControl	m_Label206;     //son O+S 개수

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

	CLabelControl	m_Label700;     //son DisplayRes1_BigLabel(TEST_DIS_4W)  관련
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
 
	CListBox	m_list1;        //son Error, Finish등 주요 정보 출력.

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



