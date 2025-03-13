#if !defined(AFX_MODESub71_H__12BD64BC_351C_4DE0_812F_BC3C7D5C5C3D__INCLUDED_)
#define AFX_MODESub71_H__12BD64BC_351C_4DE0_812F_BC3C7D5C5C3D__INCLUDED_

#include "msflexgrid.h"
#include "labelcontrol.h"
#include "file.h"
#include "resource.h"
#include "A_Define1.h"
#include "MainFrm.h"	// hdpark220511 spark

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub71.h : header file
//

// hdpark220511 spark begin
#define	SPARK_BEGIN		0
#define SPARK_END		1
#define SPARK_TRG_ON	2
#define SPARK_TRG_OFF	3
// spark end

// hdpark221115 spark begin
#define MIN_SPARK_DROP_VOLT		 2	//  2 V
#define MAX_SPARK_DROP_VOLT		250	// 90 V
#define MAX_SPARK_DROP_CURR		20	// 20 mA
#define MIN_SPARK_DROP_CURR		0.5	// 0.5 mA
// hdpark221115 spark end

//son200220 VB는 2,4 이고  VC는 1,3 인 경우   VB_2_MOD를 컴파일 옵션으로 추가해서 사용한다.
//son Sub71.AOnPinSet1(1 , pin , 2); 와 같이 nMod=2가 VB인 경우 'VB_2_MOD'를 define해서 사용한다.
#define VB_2_MOD   

typedef enum  {        //son210220
     _MOD_TR_SET_VB =  1,
     _MOD_TR_SET_VC =  2,
     _MOD_RY_SET_VB =  3,
     _MOD_RY_SET_VC =  4
} ORGIN_PIN_SET_MOD;

#ifdef VB_2_MOD
    //son200723 김영필 부장님 요구사항으로 VC부터 핀을 설정하기 위해 
    //    AOnPinSet1()에서 nMod의 HW적인 설정을 1<->2,  3<->4 를 바꾸도록 수정됨.
    //    이전에 nMod는 1: TR SET VB,  2: TR SET VC, 3: RY SET VB, 4: RY SET VC 였으나
    //    SW적으로      1: TR SET VC,  2: TR SET VB, 3: RY SET VC, 4: RY SET VB 로 VB와 VC의 의미가 바뀌었다.
        
    //son210220 PIN_SET_MOD enum 값 추가
    //    Sub71.AOnPinSet1(1 , pin , MOD_TR_SET_VB); 처럼  Sub71.AOnPinSet1()를 사용하는 코드에 추가함.
    //    Sub71.AOnPinSet1(1 , pin , MOD_TR_SET_VB) 내부에서는 1이 들어오면 강제로 2로 바꾸는 코드가 있으므로  
    //    Sub71.AOnPinSet1(1 , pin , MOD_TR_SET_VB) 내부에서는 ORGIN_PIN_SET_MOD enum을 사용한다. 
    
    //son200220 VB는 2,4 이고  VC는 1,3 인 경우   VB_2_MOD를 컴파일 옵션으로 추가해서 사용한다.
    typedef enum  {
         MOD_TR_SET_VC =  1,
         MOD_TR_SET_VB =  2,
         MOD_RY_SET_VC =  3,
         MOD_RY_SET_VB =  4
    } PIN_SET_MOD;
#else
    //son200220 원래대로 VB는 1,3 이고  VC는 2,4 인 경우 
    typedef enum {
         MOD_TR_SET_VB = _MOD_TR_SET_VB, // 1
         MOD_TR_SET_VC = _MOD_TR_SET_VC, // 2
         MOD_RY_SET_VB = _MOD_RY_SET_VB, // 3
         MOD_RY_SET_VC = _MOD_RY_SET_VC  // 4
    } PIN_SET_MOD;
#endif
 

/////////////////////////////////////////////////////////////////////////////
// CModeSub71 dialog



class CModeSub71 : public CDialog
{
// Construction

public:

			FILE* fpLeak;


	CModeSub71(CWnd* pParent = NULL);   // standard constructor 


  
    CCommandButton  m_button101; 
    CCommandButton  m_button102; 
    CCommandButton  m_button103; 
    CCommandButton  m_button104; 
 

	int AOn_Spark_ReadV1_I1();//SYLEE200713   V AND I   NEW_SPARK 
	int AOn_Spark_V_I_Check1(int nType, int nPin=0); //SYLEE200711  NEW_SPARK, hdpark220511 nPin추가
#ifdef __BH__SPARK_DECISION_BY_230828	//yklee240621 BH 230828 Spark SW 판단로직 적용
	int AOn_Spark_V_I_Check1_BH_230828(int nType, int nPin=0);
#endif
    
#ifdef __JTECH__SPARK_DECISION_BY_230828  //yklee240802 JTech Spark 판단 내부 로직 적용(함수명 같지만, 내부 다름) - JTech 양산성 적용 위함
    int AOn_Spark_V_I_Check1_for_JTech(int nType, int nPin=0);
#endif

	int AOn_Spark_Wait_ReadV1_I1_init();//SYLEE200721  NEW_SPARK
	int AOn_Spark_Wait_ReadV1_Ibuff(int AD_I);//SYLEE200721   V AND I buffer  NEW_SPARK
    int OnSparkADC(int nOn);//SYLEE200714  NEW_SPARK
	int AOn_Spark_WaitCheck1();//sylee210430

    //son231012_3 DWORD AOnRead_4W_VC();             //son210611 4W VC 측정 전용
    double AOnRead_4W_VC();             //son210611 4W VC 측정 전용 //son231012_3 DWORD -> double. 리턴타입수정
    void InitAll_Output();             //son210611 모든 Output 비트 초기화 
    void Set_LV1_ON();                 //son210611 LV1 ON 전용 함수


///////////////////////////////////////////////////////////
//embeded

	int AVB_CheckOut1(int Mod1, int nType);//SYLEE150210 //ACE380
	int AVB_CheckOut1(int Mod1, int nType, int Vset1);  //SYLEE150420-2 

	int OnInit_Byte12();//sylee150122  FAIL =0  , PASS=1
	int nFlag71_VB_G_ON;
	int nFlag71_VB_G_SEL1;
	int nFlag71_VC_G_ON;
	int nFlag71_VC_G_SEL1;

	int OnEmbed_Freq_Display(int nCh); 	

    int	nVBOutCkFlag;
	int nFlag71_I_FilterMode;//sylee150118
	int nFlag71_HV_AC_Mode1;//sylee150118
	int nFlag71_HV_AC_Mode1_Relay;
    int nFlag71_VB_OUT_CHECK;//sylee150118
	int nFlag71_VB_AC_Mode1;//sylee150118
	int nFlag71_VB_AC_Mode_Pulse1;//sylee150118
	int nFlag71_VC_Filter_Mode_LV;//sylee150118
	int nFlag71_VC_Filter_Mode_HV;//sylee150118

	void OnHV_AC_Mode_Set(int set); //ON=1, Off=0 SET  //sylee150118
	void OnHV_AC_Mode_Set_Relay(int set); //ON=1, Off=0 SET  //sylee150118
 

    void OnVB_OUT_CHECK_Mode_Set(int set); //ON=1, Off=0 SET  //sylee150118

	void On4W_GainSet(int set); //4W GAIN  //sylee150118   SLOW GAIN=1, FAST GAIN X10 =2 , FAST GAIN X100 =3 , FAST GAIN X1000 =4 ,

	
    void OnCCCV_Set(int set); //CC=1, CV=2 SET  //sylee13060550118
	void OnVB_AC_Mode_Set(int set); //ON=1, Off=0 SET  //sylee150118
	void OnVB_AC_Mode_Set_And_PulseSet(int set); //ON=1, Off=0 SET  //sylee150118
    void OnVB_AC_Mode_PulseRelaySet1(int set); //ON=1, Off=0 SET  //sylee150118

    int  I_Gain(int a);   //  1=I GAIn X 1  ,   2=I GAIN X 10  //sylee150118
    int OnEmbed_Freq(int a);  // req  1~8 //sylee150118
 
    int OnEmbed_VSet(int a);  // V SET  1 2 5 10V //sylee150118
	int OnEmbed_AV_Set(int a);  // AV MODE 1 ~4 //sylee150118


//////////////////////////////////////////////////////////////



	CString str;//sylee150118

	short OutBitNo[20],InpBitNo[20];
	BYTE OutBitData[20],InpBitData[20];
    short OutNo[20],InpNo[20];
	short OutNum,InpNum;
    BYTE OutData[20],InpData[20]; 

	short nManualFlag_HVOpen_Offset;//SYLEE130605
    short nManualFlag_HVOpen_Gain;//SYLEE130605
	short nManualFlag_I_ADC;//SYLEE130605
	short nManualFlag_VC_ADC;//SYLEE130605

    double Caln1[2][20][50][5];//v,i,row.col//sylee20110715
	double Caln2[10][20][50][5];//v,i,row.col//sylee20110715

	int nMode,nModeV1,nModeV2, nModeV3,nModeV4;

	int nMode1,nModeIR, nModeCC,nMode4W,nModeIG;

	int nMode4WG,nModeOpenHV, nModeFilter1;


	int nModeSP;

	double nModeIS1, nModeVS1;
	int nModeRCal;

     int nTimeDelay1, nTestCount1;
	 int  nADCount1 ;

	int nCPLD1,nCPLD2, nCPLDR1, nCPLDR2;


	int ModeV,ModeI;

	double nVb,lIset;

	int nVbf, nVbCo ,nIf, nICo;

	double V1;  //vb
	double Y1;  //i
	double X1;  //vc
	double R1;  //vc
	double W1;  //4WIRE

	int ADCI1; //ADC  I  //SYLEE20120711-2

	int nflgA, nflgB;

	int nPiS1[50], nPiS2[50];

	int m_nCurCol, m_nCurRow;
 	int m_nCurColold1,m_nCurColold2;
	int m_nCurRowold1,m_nCurRowold2;

	int nfAuRead1;
	double m_dRatio;

	CComboBox	m_combo101;  //SYLEE130605
	CComboBox	m_combo102;  //SYLEE130605
	CComboBox	m_combo106;  //SYLEE130605
	CComboBox	m_combo107;  //SYLEE130605	
	
	CRect m_rectGrid;
	CEdit	m_edit1;
 //	CEdit	m_edit2;
 //	CEdit	m_edit3;	
	CEdit	m_edit101;
	CEdit	m_edit102;
	CEdit	m_edit103;
	CEdit	m_edit104;
	CEdit	m_edit105;
	CEdit	m_edit106;
	CEdit	m_edit107;
	CEdit	m_edit108;
	CEdit	m_edit109;
	CEdit	m_edit110;
	CEdit	m_edit111;
	CEdit	m_edit112; 

	CEdit	m_edit301;
 	CEdit	m_edit302;
    CEdit	m_edit3022;
	CEdit	m_edit303;
	CEdit	m_edit304;
	CEdit	m_edit305;
	CEdit	m_edit306;
 	CEdit	m_edit307;
	CEdit	m_edit308;
 	CEdit	m_edit309;
	CEdit	m_edit311;
	CEdit	m_edit314;
	CEdit	m_edit315;

	CMSFlexGrid	 m_grid1;
	CMSFlexGrid	 m_grid2;

	CListBox	m_list1;

	CLabelControl	m_Label101;
	CLabelControl	m_Label102;
	CLabelControl	m_Label103;
 
	CLabelControl	m_Label105;
	CLabelControl	m_Label106;
	CLabelControl	m_Label107;
	CLabelControl	m_Label108;
	CLabelControl	m_Label109;
	CLabelControl	m_Label110;
	CLabelControl	m_Label111;
	CLabelControl	m_Label112;
 	CLabelControl	m_Label113;//SYLEE150118

	CLabelControl	m_Label121;
	CLabelControl	m_Label122;
	CLabelControl	m_Label123;
	CLabelControl	m_Label124;
	CLabelControl	m_Label125;
	CLabelControl	m_Label126;
	CLabelControl	m_Label127;
	CLabelControl	m_Label128;
	CLabelControl	m_Label129;
	CLabelControl	m_Label130;
	CLabelControl	m_Label131;
	CLabelControl	m_Label132;
	CLabelControl	m_Label133;
	CLabelControl	m_Label134;
	CLabelControl	m_Label135;
	CLabelControl	m_Label136;
	CLabelControl	m_Label137;

	CLabelControl	m_Label202; 
	CLabelControl	m_Label205; 	
	CLabelControl	m_Label207; 
	CLabelControl	m_Label209; 
	CLabelControl	m_Label210; 

	CLabelControl	m_Label214; 
    CLabelControl	m_Label2141; 
	CLabelControl	m_Label215; 
	CLabelControl	m_Label218; 
	CLabelControl	m_Label217; 
	
	CLabelControl	m_Label221; 
	CLabelControl	m_Label222; 
	CLabelControl	m_Label223; 
	CLabelControl	m_Label224; 
	CLabelControl	m_Label225;

	CLabelControl	m_Label231;
	CLabelControl	m_Label232;
	CLabelControl	m_Label234;
	CLabelControl	m_Label235;
	CLabelControl	m_Label236; 

	CLabelControl	m_Label252;
	CLabelControl	m_Label253;
	CLabelControl	m_Label254;

    CLabelControl	m_Label301;

	CLabelControl	m_Label751;	
	CLabelControl	m_Label752;	
	CLabelControl	m_Label753;	
	CLabelControl	m_Label754;	
	CLabelControl	m_Label755;
	CLabelControl	m_Label756;	
	CLabelControl	m_Label757;	
	CLabelControl	m_Label758;	
	CLabelControl	m_Label759;	
	CLabelControl	m_Label760;

	CButton m_Button110;
//	CButton m_Button111;
	CButton m_Button112;
	CButton m_Button701;
	CButton m_Button121;
	CButton m_Button127;
	CButton	m_check11;//sylee150115

	BOOL OnEditing;

    int  OnEmbeded_Guard(int a, int b);
	void OnFiler_VC(int a); //SYLEE130605    VC ADC   FAST  SLOW MODE
    void OnFiler_I(int a);  //SYLEE130605    I ADC   FAST  SLOW MODE
    void OnFiler_I_M(int a);  //SYLEE130605    I ADC   FAST  SLOW MODE
 
    int  AOnISetEnable1();    //SYLEE20111125 TEST
  
	void Delay_100ns(int dt);
 
    int Load1(); //sylee20110714
	void InitControl();
	void SetAttr(int iOpa); 
	void BeginEditing();
	void EndEditing(); 
	void On4WireMode(int a); //4WIRE  a=1 ON, 0 OFF
//============================================================================================
 	int AOnPoweroff();
	int AOnPoweroff2();

 	int AOnISet1(double dIv);    //void CModeSub71::OnButton304()  decimal  + OnButton303() 16bit
	int AOnISet1Run1(double dIv);   //SYLEE20111213 
	int AOnISetIncre1(double nISet,  int nICoMax, int nuSec ) ;  //SYLEE121008 // ISET, LOOP, uSec

    int  ADCI_Read_Wait1(int  nModeRef,  int nDiffRef, int nPassCountRef ); //sylee121007 
 
	void  OnSparkOn(int nOn); ////SPARK ON   //SYLEE130830    1=on   ext ==off  //sylee140430
	int OnSparkStatus(); ////SPARK  STATUS  //sylee130830   1=on  0 off  //sylee140430

	int AOnVBSet1(int nMod, double nV);//   VB SET, 16bit  nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK   
    int AOnIRanSet1(int Mod, int nOff); //  I range  ( 1~5,   off=1  on  o off
	int AOnIRanSet1Run1(int Mod, int nOff); //  I range  ( 1~5,   off=1  on  o off  //SYLEE20111213
    int AOnRelayOn1(int nCC, int nVSw, double nVSet, int nIR, double nI); //1=lv1,2=lv2, 3=hv1,4=hv2    // nCC=1  CC, 2=CV
 

	double AOnReadVI1();  // V I  READ
	int AOnReadI1(); /// I READING 
    int AOnReadV1(int nMod); // VC READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;   

	int AOnReadV1I1(int nMod);//SYLEE20111115 
	int AOnReadV1I1_New230220(int nMod);//SYLEE230420
	int AOnReadV1I1_Old230220(int nMod);//SYLEE230420
	int AOnReadV1I1_ADC_I();//SYLEE20120711-2  

	int AOnReadV1Enable1(int Mod1);// VC READING ,   //sylee20111115
	int AOnReadV1Enable1(int Mod1, int nVBenable);   //sylee150318

    int AOnReadVB1(int nMode2);// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;  
	int AOnReadVB1Run1(int nMode2);//SYLEE20111213
  
	int AOn_VB_CheckOut1_Set(int Mod1); //SYLEE150210 //ACE380
	int AOn_VB_CheckOut2_Run(int Mod1); //SYLEE150210 //ACE380
	int AOn_VB_CheckOut3_Stop(int Mod1); //SYLEE150210 //ACE380
	int ARunoff1();  // RUN후 초기화 
	int	AOnReadV1Init1(int nVRel);//SYLEE20111109
  	int AOnPinSet1(int Type, int No,int nMod);//nMod =1,VB,    =2VC,    PinNo(0~8k) //SYLEE20111022
	int AOnPinReSet1(int Type, int No, int nMod);//sylee20120718
    int AOnPinS1Vb1_1(int nPin, int nMod ,int nOn);   //sylee140513 //ACE400  
 	int AOnPinS1Vc1_1(int nPin, int nMod ,int nOn);   //sylee140513 //ACE400 
	int AOnPinSearch(int Onoff);// on   off
    void HVOpenSet(int a); ////SYLEE130627   1= HV OPEN , 2= VC FILTER SLOW, 3= VC FILTER FAST , 4= 4WIRE
  
	void FilterSet(int a); // Filter  0=all off, 1=vc fast, 2= vc slow, 3= hv open, 4= 4w  on     etc=error    //sylee130628
    void I_Filter(int a); // I Filter     0= all off  1=i fast, 2= i slow, 3=  vb   etc=error   //sylee130628
	void HV_OffSet(int set); //HV OFFSET  1-4   //sylee130628    
	void HV_Gain(int set); //HV GAIN  1-4    //sylee130628 

	void HV_OffSet_Gain(int set1,int set2 ); //HV OFFSET Gain    //sylee140226
//	void HV_OffSet_Gain_11(int set1,int set2 ); //HV OFFSET Gain    //sylee150130 //ACE300
    void HV_OffSet_Gain_21(int set1,int set2 ); //HV OFFSET    //sylee150130  //ACE380

    int AOnVSetMulti1(double av, double bv, double cv,  double dv) ;   //lv1=a, lv2=b, hv1=c, hv2=d
	int AOnISetOn1(int nCC, int nVRel, double nVSet, int nIR, double nISet);
  
	int AOnISetOn1Run1(int nCC, int nVRel, double nVSet, int nIR, double nISet);  //SYLEE20111213
//	int AOnISetOn1Run2(int nCC, int nVRel, double nVSet, int nIR, double nISet);  //SYLEE20111213

	int AOnRelayOff1();  
	int AOnRelayOffSel(int a ) ; 
	int OnDisChargeVB(int a);  //B discharge   a=1 on   0=off
	int OnDisCharge1(); //sylee170725-3 ray
	int OnDisChargeVC(int nOnOff); //vc discharge//sylee200916

//    int OnDisChargeVC(int a);  //C discharge   a=1 on   0=off
	int OnPin1(int a);   
	int OnPinResetInit();
    int AVBRV1(int nMode2);//   VB READING,   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;
	int AVBRV2(int nMode2);//   VC READING,   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;
    void AVBRI1();//   I READING,   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;

    int AVBRV1S(int nMode2, int nZero);//   16  bit VB SET,   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK
	int AVBRV1S2(int nMode2);//  10진수  VB SET,   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK
    void AVBRI1S();//  I READING,   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;
   	
	int  OnPinResetVB(int nLine = 0); //PIN VB ALL  RESET   //SYLEE20120125 
    int  OnPinResetVC(int nLine = 0); //PIN VC ALL  RESET   //SYLEE20120125

 	int AIRangeOff();
	int AIVBOff(); 
 
	int AVB_Set_AD1(int nMode2, int nADset);//   VB SET, 16bit  nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK

	int  OnSelfRSet(int a, int b);// a=1 100,  a=2 10M,       b=1 run, 0 off
    int  On4WS12Set(int a, int b);

    int  m_radio01;     
	double  ARReadCal1(int nRelay, int nMod1, double nV, int nIR, double nI, double R); 
    int  OnPinAllReset1(int nLine = 0);
    void OnRelayAllOff1(); //SYLEE180724
    void OnRelay4W_ON1();  //SYLEE181115
    void OnRelay4W_OFF1();  //SYLEE181118

	BOOL			m_bFirstStep;	//hdpark20240129, SPARK AI 판별 기준 추가
	//hdpark220511 spark begin
	void			SetVBReadMode();
	int				SendCmdToDigitizer(int nLine, int nTotalNet, int nSequence);
	BOOL			m_bModifySparkMeas;  //son 0: VB Mode Off, 1: VB Mode On    MAP > Set3 > Spark VB Mode
	HWND			m_hwndDigitizer;
	FILE*			fpSend;
	FILE*			fpSpark;
	FILE*			fpSparkLog;
	FILE*			fpSparkWave;	// hdpark231121, 분석용.

	PIN_INFO		m_tINFO[MAX_TRIG_COUNT];
	int				m_nNetCnt;
	int				m_nTotalNetCnt;
	int				m_nHWCnt;
	BOOL			m_bSetVBRead;
	BOOL			m_bVBReadMode;
	//hdpark220511  spark end
	
	// hdpark221115 spark begin
	BOOL			m_bSparkCal;
	BOOL			m_bSparkVerification; //hdpark221220 Spark
	int				m_nTestMode;
	double			m_dForceCurrent;
	double			m_dMaxSparkValue;
	double			m_dMinSparkValue; 
	double			m_dSparkCalRatio;//hdpark221220 Spark
	int				m_nSparkDetection;		// hdpark230331 0: vlotage, 1: current

	// hdpark221115 spark end

	// hdpark220812 spark cal begin
	int				AOnPinSet1ForSparkCal(int Type, int No, int nMod);
	int				ReadSparkForCal(int nCount=20); 
	// hdpark220812 end 
	
	//hdpark220511  test code begin
	int				WRITE_IO(int nIO, BOOL bON);
	int				DOWrite(int m_uCardID, DWORD dwData);
	int				SetPin(int nCard, int nPin, BOOL bOn);
	// hdpark221115 test code end

	BOOL			m_bLeakTest;	// hdpark220913, HR 일경우 Pin 등록에서 Spark Skip.
	double			m_dCalRatio;	// hdpark240123
	double			m_dCalSparkMax;	// hdpark240123
	BOOL			m_bACMode;
/////////////////////////////////////////////////
 

// Dialog Data
	//{{AFX_DATA(CModeSub71)
	  enum { IDD = IDD_MODE_SUB71_MANUAL };
 		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub71)

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub71)
		// NOTE: the ClassWizard will add member functions here

	afx_msg void OnClickOk();
	afx_msg void OnClickGrid1();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnEnterCellMsflexgrid1();
	afx_msg void OnPinSel1( int a, int b);   
	afx_msg void OnLeaveCellMsflexgrid1();
	afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
	afx_msg void OnSave();
	afx_msg void Onload();

	afx_msg void OnRadio31();  
	afx_msg void OnRadio32(); 
 	afx_msg void OnRadio33();  
	afx_msg void OnRadio34(); 
	afx_msg void OnRadio35(); 
 	afx_msg void OnRadio36();  
 	afx_msg void OnRadio37();  
 	afx_msg void OnRadio38(); 

	afx_msg void OnTimer(UINT nIDEvent);

 	afx_msg void OnButton105();
	afx_msg void OnButton109();
	afx_msg void OnButton110();
	afx_msg void OnButton111();
	afx_msg void OnButton112();
	afx_msg void OnButton115();

	afx_msg void OnButton120();
	afx_msg void OnButton121();
	afx_msg void OnButton122();
	afx_msg void OnButton123();
	afx_msg void OnButton124();
	afx_msg void OnButton125();
	afx_msg void OnButton126();
	afx_msg void OnButton127();
 
	afx_msg void OnButton201();
	afx_msg void OnButton202();
	afx_msg void OnButton203();
	afx_msg void OnButton204();
	afx_msg void OnButton205();
	afx_msg void OnButton206();

	afx_msg void OnButton301();
	afx_msg void OnButton302();
	afx_msg void OnButton303(); 
	afx_msg void OnButton304(); 

	afx_msg int OnButton501();
 
 
	afx_msg void OnButton502();
	afx_msg void OnButton503(); 
	afx_msg void OnButton506();
 	afx_msg void OnButton507(); 

	afx_msg void OnButton601(); 
	afx_msg void OnButton602();
	afx_msg void OnButton603();
	afx_msg void OnButton6032(); 
	afx_msg void OnButton604(); 
	afx_msg void OnButton605(); 
	afx_msg void OnButton606();
	afx_msg void OnButton607();
	afx_msg void OnButton608();
	afx_msg void OnButton609();
	afx_msg void OnButton610();
	afx_msg void OnButton611();
	afx_msg void OnButton612();
	afx_msg void OnButton613();
    afx_msg void OnButton614(); 
	afx_msg void OnButton701();
    afx_msg void OnButton751(); 
	afx_msg void OnButton752(); 
	afx_msg void OnButton753(); 
    afx_msg void OnButton754(); 
	afx_msg void OnButton755(); 
	afx_msg void OnButton756(); 
    afx_msg void OnButton757(); 
	afx_msg void OnButton758(); 
	afx_msg void OnButton759(); 
	afx_msg void OnButton760();
	afx_msg void OnButton801();
	afx_msg void OnButton802();
	afx_msg void OnButton803();
	afx_msg void OnButton804();
	afx_msg void OnButton805();
	afx_msg void OnButton806();
	afx_msg void OnButton807();
	afx_msg void OnButton808(); 
	afx_msg void OnButton904();  
 
	afx_msg void OnButton1106(); 
	afx_msg void OnButton1107(); 
	afx_msg void OnButton1108(); 
 	afx_msg void OnButton1109(); 
 	afx_msg void OnButton1110(); 
	afx_msg void OnButton11102();  
    afx_msg void OnButton1111(); 
 	afx_msg void OnButton1112(); 
 	afx_msg void OnButton1113(); 
	afx_msg void OnButton1114(); 
    afx_msg void OnButton1115(); 
    afx_msg void OnButton11152(); 

    afx_msg void OnButton11081();  
	afx_msg void OnButton2007();  
	afx_msg void OnButton2009(); 
  
	afx_msg void OnSelchangeCombo1(); 
	afx_msg void OnSelchangeCombo2(); 
	afx_msg void OnSelchangeCombo6(); 
	afx_msg void OnSelchangeCombo7();  

 	afx_msg void OnRadio01();
 	afx_msg void OnRadio02();
	afx_msg void OnRadio03();
	afx_msg void OnRadio04();
 	afx_msg void OnRadio05();
	afx_msg void OnRadio06();
	afx_msg void OnRadio07();
 	afx_msg void OnRadio08();
	afx_msg void OnRadio09();
	afx_msg void OnRadio10();
 	afx_msg void OnRadio11();
	afx_msg void OnRadio12();
	afx_msg void OnRadio13(); 
	afx_msg void OnRadio14(); 
	afx_msg void OnRadio15(); 
	afx_msg void OnRadio16(); 
	afx_msg void OnRadio21();
 	afx_msg void OnRadio22();
	afx_msg void OnRadio23();
	afx_msg void OnRadio24();
 	afx_msg void OnRadio25();
	afx_msg void OnCheck11();  
 
	afx_msg void OnRadio101();
	afx_msg void OnRadio102();
 	afx_msg void OnRadio103();
 	afx_msg void OnRadio104();
	afx_msg void OnRadio105();
 	afx_msg void OnRadio106();
 	afx_msg void OnRadio107();
 	afx_msg void OnRadio108();


	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

 //CModeSub71  Sub71;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESub71_H__12BD64BC_351C_4DE0_812F_BC3C7D5C5C3D__INCLUDED_)


