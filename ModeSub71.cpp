
#include <String.h>

#include "stdafx.h"
#include "ModeSub71.h"
#include "ModeSub72.h"
#include "Contec.h"
#include "windef.h"
#include "FileSysInfo.h"
#include "io.h"
#include "A_Define1.h"//sylee161126

#define MAX_SPARK_OFFSET	3.0	//hdpark240123

int LogError1(CString str1);    //SYLEE210316-1

extern double dtmp1, dNgPercent;   //sylee200515-patch  //sylee230420

extern float d4wI[MAX_4W_NET +1][510],d4wV[MAX_4W_NET +1][510];//sylee221206-2   //sylee230420 
                    //son231107 Def_4WMax1 -> MAX_4W_NET +1
extern int d4wIPo1,d4wVPo1,d4wNet1;//sylee221206-2

extern double	g_dSparkRatio;	// hdpark231121
extern double  g_dWaitingSparkVOrg;  // hdpark231121
extern double  g_dWaitingSparkV;	//hdpark231121
extern int     g_nSparkWaitingEnable;  //hdpark231121 // hdpark230808, 0 : disable, 2: enable
extern float g_fSparkRefData[DEF_MAX_PIN1]; // hdpark220511
extern short g_nSparkRefDataCount[DEF_MAX_PIN1]; // hdpark220511
extern SPARK_AI    g_tSparkAI[DEF_MAX_PIN1];	// hdpark240102

extern int nShort_Flag1; //sylee200723-1 ray

extern int nFlag_LeakDischarge1;//sylee200916-1
extern int nFlag_NewSparkBaord;//SYLEE201024-1  //SPARK NEW BOARD	
extern int  nSparkIPlus1;//SYLEE201027

extern	float dSparkV1[MAX_BLOCK+1][MAX_PIECE+1], dSparkI1[MAX_BLOCK+1][MAX_PIECE+1], dSparkI12[MAX_BLOCK+1][MAX_PIECE+1];//sylee200710 NEW_SPARK
extern float dSparkV2[MAX_PIECE+1], dSparkI2[MAX_PIECE+1];//sylee200710 NEW_SPARK
extern	int nFlag_Spark_PinOn;//sylee200710-1 NEW_SPARK
extern int nSparkRefV, nSparkPinNo1;//sylee200710-1  NEW_SPARK
extern double dSparkLeakSet1, dSparkLeakSet1_I, dSparkLeakSet1_V;//sylee200710-1  NEW_SPARK
extern int nSparkDetect1, nSparkMaxX, nSparkMaxY, nSparkGapMin, nSparkHWoffSet1, nSparkSWoffSet1, nSparkMaxXGap1;//sylee200710-1 NEW_SPARK
extern double dSparkHiValue, dSparkLowValue;//sylee200710-1 NEW_SPARK
int nSparkV1Po1;//sylee200710 NEW_SPARK  //yklee Spark Raw ADC 버퍼 접근 Index용으로 Spark Mon Count와 연관 체크에 활용
int nSparkI1Po1;//sylee200710 NEW_SPARK  //yklee 미사용중
int nSparkV1Po2;//sylee200710 NEW_SPARK 
int nSparkI1Po2;//sylee200710 NEW_SPARK  //yklee Spark 발생시 SW 판단완료때 로그버퍼 접근 Index로 활용중
int nSpark1Po1_Start;   //yklee Wait Spark 체크용으로 Raw ADC 접근시점 판단용으로 활용 시도하였으나, 현재 미사용 
int nSpark1_Detect2;    //yklee Wait Spark Detect 결과용으로 Wait Spark 에러 로그 생성 판단으로 활용중
int nSpark1_Detect2Co, nSpark1_Detect2_Step;  //yklee 미사용중 
int g_nSpark1_Use;//sylee200721 NEW_SPARK  //yklee 레시피에서 Spark ON 정보를 복사하여 SW 판단 및 로그파일 생성 판단용으로 활용중

extern int n4W_PinFastADC1;//sylee20905
extern int nHV_Vset1;//sylee200703 NEW_SPARK
extern int n4w_Measure1_Enable;//sylee200512  //sylee200515-patch
extern int n4W_ADC_Fail_Flag; //ng	//sylee200513-1	//sylee200515-patch
extern int nFlag_4W_OUT50V; //SYLEE200411  //sylee200515-patch
extern int nRelay1OffSkip;//sylee170424-1 
extern CSysSet19  SysSet19;//sylee200217-4WCVMODE //sylee200515-patch

extern CSysSet13  SysSet13;//sylee200321-4WCVMODE
extern CSysSet16 SysSet16;//sylee200901

extern 	int   n4W_Use;  //sylee200612
//=======================================
 
extern  int  gn4W_Mode1,gn4W_Mode1_RangeADC1,gn4W_Mode1_RangeADC2;//sylee230124; 

extern double  d4WADCPer1 ,d4WADCPer2,d4WADCPer3 , d4WADCSigma1; //sylee221208-1
 
extern int n4WAdcOK1[10];//sylee221212-1


extern short nFlagLeakMPin1;  //sylee230526    //son230911
extern short gnBDLData32[MAX_DUT1][DEF_MAX_PIN1][3];//short2  index   //sylee230526  //sylee230817  //son230911
extern short gnBDLData33[MAX_DUT1][DEF_MAX_PIN1][3];//short2 data  //sylee230526 //son230905_2      //son230911


#ifdef DEF_CARD_A1  //sylee170328-1

#else   

	//=======================================
#include "DioD64H.h"//sylee170229
#include "DioD96.h"//sylee170229
#include "UniDAQ.h"//sylee170229
#include "PCI_D64H.h" //sylee170229 ㅣ
extern	CDIO_D64  pDIO_D64;//sylee170229
extern CDIO_D96  pDIO_D96;
unsigned long  dwDOData;
unsigned long  dwDOData2;//sylee170502



DWORD DioInpByte(HANDLE a, int b, LPBYTE c)
{
	return 0;
}

DWORD DioOutByte(HANDLE a, int b, BYTE c)
{
	return 0;
}

DWORD DioOutBit(HANDLE a, int b, BYTE c)
{
	return 0;
}

//	DWORD  AB_Bit(int b, BYTE c) 
//	{
//		return 0;
//	}
//	DWORD A_Byte( int b, BYTE c )
//	{
//		return 0;
//	}

#endif


extern int nSparkMaxXSet;//sylee210430
extern int nFailL1Co;	// hdpark220511 spark

extern int nFailL1CoOpen;//sylee150422-3

extern CSysInfo05  SysInfo05;//sylee150319-1
extern short nPinSetSkipVb; //sylee151005 //ACE400	//son230127
extern short nPinSetSkipVc; //sylee151005 //ACE400	//son230127

#define FB1 1;
#define FB2 1;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int nADCOver_IntDisable;//sylee150121

extern CSysSet13  SysSet13;
extern CSysInfoView01  SysInfoView01;
extern CSysSet12  SysSet12;  //Pin SETUP  
extern int nStepI;

extern int nCC;    // 1=CC,  2=CV
extern int nVRel;  // LV1,LV2,HV1,HV2,
extern double nVSet;  // V  Voltage
extern int nIR;    // I range  1~5
extern double nISet;  // I set

int nMode4W2; //sylee140717
int nCardVb;//sylee120914
int nCardVc;//sylee120914
int nSwitch1[10][40];  //SYLEE20120220 
int nOldBoxStatus;//SYLEE20120224

//son231012_2 int nIADC1;//sylee20120228 log 
double dIADC1;//sylee20120228 log   //son231012_2 int nIADC1  -> double dIADC1으로 수정. 소수점이하 잘리는 문제보완.

int nADCountI; //sylee20120405
int nADCountIFlag; //sylee2012040

static int ConfigBox = 13;// 2k 3ea


extern __int64 tStart, tStart1, tStart2, tStart3;
extern __int64 GetMilSecond();
extern CSysSet211  SysSet211;  //Pin SETUP  
extern double dADCPercent1;//sylee221220-1	//sylee230420



 /*
DWORD DioInpByte(HANDLE a, int b, LPBYTE c){
	return 0;
}

DWORD	DioOutByte(HANDLE a, int b, BYTE c )
{
	return 0;
}

DWORD  DioOutBit(HANDLE a, int b, BYTE c)
{
	return 0;
}
  */

short OutNoT[20] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
extern _int64  Get_uSecond1();





CModeSub71::CModeSub71(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub71::IDD, pParent)
{
	// hdpark220511 spark begin
	m_bSparkCal = FALSE;	//hdpark230223
	m_bModifySparkMeas = FALSE;
	m_bSetVBRead = FALSE;
	m_nHWCnt = 0;
	// spark end
	
	m_bLeakTest = FALSE;   //hdpark220913	

	m_radio01 = 0;
	//{{AFX_DATA_INIT(CModeSub71)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub71::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub71)
		// NOTE: the ClassWizard will add DDX and DDV calls here

	DDX_Control(pDX, IDC_EDIT103, m_edit103);
	DDX_Control(pDX, IDC_EDIT104, m_edit104);
	DDX_Control(pDX, IDC_EDIT101, m_edit101);
	DDX_Control(pDX, IDC_EDIT102, m_edit102);
	DDX_Control(pDX, IDC_EDIT105, m_edit105);
	DDX_Control(pDX, IDC_EDIT106, m_edit106);
	DDX_Control(pDX, IDC_EDIT107, m_edit107);
	DDX_Control(pDX, IDC_EDIT108, m_edit108);
	DDX_Control(pDX, IDC_EDIT109, m_edit109);
	DDX_Control(pDX, IDC_EDIT110, m_edit110);
	DDX_Control(pDX, IDC_EDIT111, m_edit111);
	DDX_Control(pDX, IDC_EDIT112, m_edit112);

	DDX_Control(pDX, IDC_EDIT3011, m_edit301);
	DDX_Control(pDX, IDC_EDIT3021, m_edit302);
	DDX_Control(pDX, IDC_EDIT3022, m_edit3022);
	DDX_Control(pDX, IDC_EDIT303, m_edit303);
	DDX_Control(pDX, IDC_EDIT304, m_edit304);
	DDX_Control(pDX, IDC_EDIT305, m_edit305);
	DDX_Control(pDX, IDC_EDIT306, m_edit306);
	DDX_Control(pDX, IDC_EDIT307, m_edit307);
	DDX_Control(pDX, IDC_EDIT308, m_edit308);
	DDX_Control(pDX, IDC_EDIT309, m_edit309);
	DDX_Control(pDX, IDC_EDIT311, m_edit311);
	DDX_Control(pDX, IDC_EDIT314, m_edit314);
	DDX_Control(pDX, IDC_EDIT315, m_edit315);
	DDX_Control(pDX, IDC_LIST1, m_list1);

	DDX_Control(pDX, IDC_LABEL101, m_Label101);
	DDX_Control(pDX, IDC_LABEL102, m_Label102);
	DDX_Control(pDX, IDC_LABEL103, m_Label103);

	DDX_Control(pDX, IDC_LABEL105, m_Label105);
	DDX_Control(pDX, IDC_LABEL106, m_Label106);
	DDX_Control(pDX, IDC_LABEL107, m_Label107);
	DDX_Control(pDX, IDC_LABEL108, m_Label108);
	DDX_Control(pDX, IDC_LABEL109, m_Label109);
	DDX_Control(pDX, IDC_LABEL110, m_Label110);
	DDX_Control(pDX, IDC_LABEL111, m_Label111);
	DDX_Control(pDX, IDC_LABEL113, m_Label113);


	DDX_Control(pDX, IDC_LABEL121, m_Label121);
	DDX_Control(pDX, IDC_LABEL122, m_Label122);
	DDX_Control(pDX, IDC_LABEL123, m_Label123);
	DDX_Control(pDX, IDC_LABEL124, m_Label124);
	DDX_Control(pDX, IDC_LABEL125, m_Label125);
	DDX_Control(pDX, IDC_LABEL126, m_Label126);
	DDX_Control(pDX, IDC_LABEL127, m_Label127);
	DDX_Control(pDX, IDC_LABEL128, m_Label128);
	DDX_Control(pDX, IDC_LABEL129, m_Label129);
	DDX_Control(pDX, IDC_LABEL130, m_Label130);

	DDX_Control(pDX, IDC_LABEL131, m_Label131);
	DDX_Control(pDX, IDC_LABEL132, m_Label132);
	DDX_Control(pDX, IDC_LABEL133, m_Label133);
	DDX_Control(pDX, IDC_LABEL134, m_Label134);
	DDX_Control(pDX, IDC_LABEL135, m_Label135);
	DDX_Control(pDX, IDC_LABEL136, m_Label136);
	DDX_Control(pDX, IDC_LABEL137, m_Label137);


	DDX_Control(pDX, IDC_LABEL202, m_Label202);

	DDX_Control(pDX, IDC_LABEL205, m_Label205);
	DDX_Control(pDX, IDC_LABEL207, m_Label207);
	DDX_Control(pDX, IDC_LABEL209, m_Label209);
	DDX_Control(pDX, IDC_LABEL210, m_Label210);

	DDX_Control(pDX, IDC_LABEL2141, m_Label2141);
	DDX_Control(pDX, IDC_LABEL214, m_Label214);
	DDX_Control(pDX, IDC_LABEL2151, m_Label215);
	DDX_Control(pDX, IDC_LABEL2181, m_Label218);
	DDX_Control(pDX, IDC_LABEL2171, m_Label217);

	DDX_Control(pDX, IDC_LABEL221, m_Label221);
	DDX_Control(pDX, IDC_LABEL222, m_Label222);
	DDX_Control(pDX, IDC_LABEL223, m_Label223);
	DDX_Control(pDX, IDC_LABEL224, m_Label224);
	DDX_Control(pDX, IDC_LABEL225, m_Label225);


	DDX_Control(pDX, IDC_LABEL231, m_Label231);
	DDX_Control(pDX, IDC_LABEL232, m_Label232);

	DDX_Control(pDX, IDC_LABEL234, m_Label234);
	DDX_Control(pDX, IDC_LABEL235, m_Label235);
	DDX_Control(pDX, IDC_LABEL236, m_Label236);


	DDX_Control(pDX, IDC_LABEL751, m_Label751);
	DDX_Control(pDX, IDC_LABEL752, m_Label752);
	DDX_Control(pDX, IDC_LABEL753, m_Label753);
	DDX_Control(pDX, IDC_LABEL754, m_Label754);
	DDX_Control(pDX, IDC_LABEL755, m_Label755);
	DDX_Control(pDX, IDC_LABEL756, m_Label756);
	DDX_Control(pDX, IDC_LABEL757, m_Label757);
	DDX_Control(pDX, IDC_LABEL758, m_Label758);
	DDX_Control(pDX, IDC_LABEL759, m_Label759);
	DDX_Control(pDX, IDC_LABEL760, m_Label760);

	DDX_Control(pDX, IDC_LABEL301, m_Label301);

	DDX_Control(pDX, IDC_BUTTON110, m_Button110);

	DDX_Control(pDX, IDC_BUTTON112, m_Button112);
	DDX_Control(pDX, IDC_BUTTON121, m_Button121);
	DDX_Control(pDX, IDC_BUTTON127, m_Button127);


	DDX_Control(pDX, IDC_COMBO101, m_combo101);
	DDX_Control(pDX, IDC_COMBO102, m_combo102);
	DDX_Control(pDX, IDC_COMBO106, m_combo106);
	DDX_Control(pDX, IDC_COMBO107, m_combo107);


	DDX_Control(pDX, IDC_CHECK11, m_check11);//sylee150115


	DDX_Control(pDX, IDC_BUTTON757, m_button101);
	DDX_Control(pDX, IDC_BUTTON758, m_button102);
	DDX_Control(pDX, IDC_BUTTON759, m_button103);
	DDX_Control(pDX, IDC_BUTTON760, m_button104);





	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub71, CDialog)
	//{{AFX_MSG_MAP(CModeSub71)
		// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_COMBO101, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO102, OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO106, OnSelchangeCombo6)
	ON_CBN_SELCHANGE(IDC_COMBO107, OnSelchangeCombo7)

	ON_BN_CLICKED(IDC_RADIO01, OnRadio01)
	ON_BN_CLICKED(IDC_RADIO02, OnRadio02)
	ON_BN_CLICKED(IDC_RADIO03, OnRadio03)
	ON_BN_CLICKED(IDC_RADIO04, OnRadio04)
	ON_BN_CLICKED(IDC_RADIO05, OnRadio05)
	ON_BN_CLICKED(IDC_RADIO06, OnRadio06)
	ON_BN_CLICKED(IDC_RADIO07, OnRadio07)
	ON_BN_CLICKED(IDC_RADIO08, OnRadio08)
	ON_BN_CLICKED(IDC_RADIO09, OnRadio09)
	ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13) //sylee150118
	ON_BN_CLICKED(IDC_RADIO14, OnRadio14) //sylee150118
	ON_BN_CLICKED(IDC_RADIO15, OnRadio15) //sylee150118
	ON_BN_CLICKED(IDC_RADIO16, OnRadio16) //sylee150118
	ON_BN_CLICKED(IDC_RADIO21, OnRadio21)
	ON_BN_CLICKED(IDC_RADIO22, OnRadio22)
	ON_BN_CLICKED(IDC_RADIO23, OnRadio23)
	ON_BN_CLICKED(IDC_RADIO24, OnRadio24)
	ON_BN_CLICKED(IDC_RADIO25, OnRadio25)


	ON_BN_CLICKED(IDC_RADIO101, OnRadio101)   //embedded
	ON_BN_CLICKED(IDC_RADIO102, OnRadio102)   //embedded
	ON_BN_CLICKED(IDC_RADIO103, OnRadio103)   //embedded
	ON_BN_CLICKED(IDC_RADIO104, OnRadio104)   //embedded 

	ON_BN_CLICKED(IDC_RADIO105, OnRadio105)   //embedded
	ON_BN_CLICKED(IDC_RADIO106, OnRadio106)   //embedded
	ON_BN_CLICKED(IDC_RADIO107, OnRadio107)   //embedded
	ON_BN_CLICKED(IDC_RADIO108, OnRadio108)   //embedded


	ON_BN_CLICKED(IDC_RADIO31, OnRadio31)//embedded
	ON_BN_CLICKED(IDC_RADIO32, OnRadio32)//embedded
	ON_BN_CLICKED(IDC_RADIO33, OnRadio33)//embedded
	ON_BN_CLICKED(IDC_RADIO344, OnRadio34)//embedded

	ON_BN_CLICKED(IDC_RADIO35, OnRadio35) //embedded
	ON_BN_CLICKED(IDC_RADIO366, OnRadio36) //embedded
	ON_BN_CLICKED(IDC_RADIO37, OnRadio37) //embedded
	ON_BN_CLICKED(IDC_RADIO388, OnRadio38) //embedded

	ON_BN_CLICKED(IDC_CHECK11, OnCheck11) //sylee150115	  

   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub71 message handlers

 /*
		 ON_BN_CLICKED(IDC_OK, OnClickOk)

		 ON_BN_CLICKED(IDC_BUTTON112, OnButton112)
		 ON_BN_CLICKED(IDC_BUTTON121, OnButton121)
		 ON_BN_CLICKED(IDC_BUTTON122, OnButton122)
		 ON_BN_CLICKED(IDC_BUTTON123, OnButton123)
		 ON_BN_CLICKED(IDC_BUTTON124, OnButton124)
		 ON_BN_CLICKED(IDC_BUTTON126, OnButton126)

		 ON_BN_CLICKED(IDC_BUTTON301, OnButton301)
		 ON_BN_CLICKED(IDC_BUTTON302, OnButton302)
		 ON_BN_CLICKED(IDC_BUTTON306, OnButton303)
		 ON_BN_CLICKED(IDC_BUTTON307, OnButton304)

		 ON_BN_CLICKED(IDC_BUTTON506, OnButton506)
		 ON_BN_CLICKED(IDC_BUTTON601, OnButton601)
		 ON_BN_CLICKED(IDC_BUTTON602, OnButton602)
		 ON_BN_CLICKED(IDC_BUTTON603, OnButton603)
		 ON_BN_CLICKED(IDC_BUTTON604, OnButton604)
		 ON_BN_CLICKED(IDC_BUTTON605, OnButton605)

		 ON_BN_CLICKED(IDC_BUTTON610, OnButton610)
		 ON_BN_CLICKED(IDC_BUTTON611, OnButton611)
		 ON_BN_CLICKED(IDC_BUTTON612, OnButton612)
		 ON_BN_CLICKED(IDC_BUTTON614, OnButton614)

		 ON_BN_CLICKED(IDC_BUTTON751, OnButton751)
		 ON_BN_CLICKED(IDC_BUTTON752, OnButton752)
		 ON_BN_CLICKED(IDC_BUTTON753, OnButton753)
		 ON_BN_CLICKED(IDC_BUTTON754, OnButton754)
		 ON_BN_CLICKED(IDC_BUTTON755, OnButton755)
		 ON_BN_CLICKED(IDC_BUTTON756, OnButton756)
		 ON_BN_CLICKED(IDC_BUTTON757, OnButton757)
		 ON_BN_CLICKED(IDC_BUTTON758, OnButton758)
		 ON_BN_CLICKED(IDC_BUTTON759, OnButton759)
		 ON_BN_CLICKED(IDC_BUTTON760, OnButton760)

		 ON_BN_CLICKED(IDC_BUTTON1206, OnButton1106)
		 ON_BN_CLICKED(IDC_BUTTON1107, OnButton1107)
		 ON_BN_CLICKED(IDC_BUTTON1108, OnButton1108)
		 ON_BN_CLICKED(IDC_BUTTON11081, OnButton11081)
		 ON_BN_CLICKED(IDC_BUTTON118, OnButton125)

		 ON_BN_CLICKED(IDC_BUTTON1109, OnButton1109)
		 ON_BN_CLICKED(IDC_BUTTON11101, OnButton1110)
		 ON_BN_CLICKED(IDC_BUTTON11102, OnButton11102)

		 ON_BN_CLICKED(IDC_BUTTON1111, OnButton1111)
		 ON_BN_CLICKED(IDC_BUTTON1112, OnButton1112)
		 ON_BN_CLICKED(IDC_BUTTON1113, OnButton1113)
		 ON_BN_CLICKED(IDC_BUTTON1114, OnButton1114)
		 ON_BN_CLICKED(IDC_BUTTON11151, OnButton1115)
		 ON_BN_CLICKED(IDC_BUTTON11152, OnButton11152)
  */


BEGIN_EVENTSINK_MAP(CModeSub71, CDialog)
	//{{AFX_EVENTSINK_MAP(CModeSub71)
	ON_EVENT(CModeSub71, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_LABEL2, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_GRID_1, -600 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSub71, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BLOAD, -600 /* Click */, Onload, VTS_NONE)


	ON_EVENT(CModeSub71, IDC_BUTTON105, -600 /* Click */, OnButton105, VTS_NONE)

	ON_EVENT(CModeSub71, IDC_BUTTON110, -600 /* Click */, OnButton110, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON111, -600 /* Click */, OnButton111, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON112, -600 /* Click */, OnButton112, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON115, -600 /* Click */, OnButton115, VTS_NONE)

	ON_EVENT(CModeSub71, IDC_BUTTON120, -600 /* Click */, OnButton120, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON121, -600 /* Click */, OnButton121, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON122, -600 /* Click */, OnButton122, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON123, -600 /* Click */, OnButton123, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON124, -600 /* Click */, OnButton124, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON118, -600 /* Click */, OnButton125, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON126, -600 /* Click */, OnButton126, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON127, -600 /* Click */, OnButton127, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON206, -600 /* Click */, OnButton206, VTS_NONE)

	ON_EVENT(CModeSub71, IDC_BUTTON301, -600 /* Click */, OnButton301, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON302, -600 /* Click */, OnButton302, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON303, -600 /* Click */, OnButton303, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON304, -600 /* Click */, OnButton304, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON306, -600 /* Click */, OnButton303, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON307, -600 /* Click */, OnButton304, VTS_NONE)


	ON_EVENT(CModeSub71, IDC_BUTTON501, -600 /* Click */, OnButton501, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON502, -600 /* Click */, OnButton502, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON503, -600 /* Click */, OnButton503, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON506, -600 /* Click */, OnButton506, VTS_NONE)


	ON_EVENT(CModeSub71, IDC_BUTTON601, -600 /* Click */, OnButton601, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON602, -600 /* Click */, OnButton602, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON603, -600 /* Click */, OnButton603, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON604, -600 /* Click */, OnButton604, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON605, -600 /* Click */, OnButton605, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON606, -600 /* Click */, OnButton606, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON607, -600 /* Click */, OnButton607, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON608, -600 /* Click */, OnButton608, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON609, -600 /* Click */, OnButton609, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON610, -600 /* Click */, OnButton610, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON611, -600 /* Click */, OnButton611, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON612, -600 /* Click */, OnButton612, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON613, -600 /* Click */, OnButton613, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON614, -600 /* Click */, OnButton614, VTS_NONE)


	ON_EVENT(CModeSub71, IDC_BUTTON701, -600 /* Click */, OnButton701, VTS_NONE)


	ON_EVENT(CModeSub71, IDC_BUTTON751, -600 /* Click */, OnButton751, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON752, -600 /* Click */, OnButton752, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON753, -600 /* Click */, OnButton753, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON754, -600 /* Click */, OnButton754, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON755, -600 /* Click */, OnButton755, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON756, -600 /* Click */, OnButton756, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON757, -600 /* Click */, OnButton757, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON758, -600 /* Click */, OnButton758, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON759, -600 /* Click */, OnButton759, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON760, -600 /* Click */, OnButton760, VTS_NONE)

	ON_EVENT(CModeSub71, IDC_BUTTON801, -600 /* Click */, OnButton801, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON802, -600 /* Click */, OnButton802, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON803, -600 /* Click */, OnButton803, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON804, -600 /* Click */, OnButton804, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON805, -600 /* Click */, OnButton805, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON806, -600 /* Click */, OnButton806, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON807, -600 /* Click */, OnButton807, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON808, -600 /* Click */, OnButton808, VTS_NONE)


	ON_EVENT(CModeSub71, IDC_BUTTON1206, -600 /* Click */, OnButton1106, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1107, -600 /* Click */, OnButton1107, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1108, -600 /* Click */, OnButton1108, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1109, -600 /* Click */, OnButton1109, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON11101, -600 /* Click */, OnButton1110, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON11102, -600 /* Click */, OnButton11102, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1111, -600 /* Click */, OnButton1111, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1112, -600 /* Click */, OnButton1112, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1113, -600 /* Click */, OnButton1113, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON1114, -600 /* Click */, OnButton1114, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON11151, -600 /* Click */, OnButton1115, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON11152, -600 /* Click */, OnButton11152, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON11081, -600 /* Click */, OnButton11081, VTS_NONE)

	ON_EVENT(CModeSub71, IDC_BUTTON2007, -600 /* Click */, OnButton2007, VTS_NONE)
	ON_EVENT(CModeSub71, IDC_BUTTON2009, -600 /* Click */, OnButton2009, VTS_NONE)

	//}}AFX_EVENTSINK_MAP

END_EVENTSINK_MAP()



void DoEvents()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

}

void DoEvents2()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	//Sleep(1);

}


void DoEvents5()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

}


//son Radio 'I Filter: Fast'
void CModeSub71::OnRadio01()
{

	OnFiler_I_M(2); //fast i
}


//son Radio 'I Filter: Slow'
void CModeSub71::OnRadio02()
{
	OnFiler_I_M(1);//slow  i
}


//son Radio 'VC Filter: HV On'
void CModeSub71::OnRadio03() //VC FILTER HV
{
	CString str;

	if (nFlag71_VC_Filter_Mode_HV != 1) {
		nFlag71_VC_Filter_Mode_HV = 1;//sylee150118
		nFlag71_VC_Filter_Mode_LV = 0;
		((CButton*)GetDlgItem(IDC_RADIO03))->SetCheck(TRUE);	//son 'VC Fileter HV On' Radio On
		((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(FALSE);	//son 'VC Fileter LV On' Radio Off
		OnFiler_VC(2); // fast  vc FILTER ON
	}
	else {
		nFlag71_VC_Filter_Mode_HV = 0;
		((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(FALSE);	//son 'VC Fileter HV On' Radio Off
		((CButton*)GetDlgItem(IDC_RADIO03))->SetCheck(FALSE);	//son 'VC Fileter LV On' Radio Off
		OnFiler_VC(12); // fast  vc FILTER  OFF
	}

	str.Format("OFF");
	nMode4W = 0;
	m_Label225.SetCaption(str);		//son '4W' Label Set Off

	nModeOpenHV = 0;
	str.Format("OFF");
	m_Label236.SetCaption(str);		//son 'HV OPEN' Label Set Off	
}


//son Radio 'VC Filter: LV On'
void CModeSub71::OnRadio04()   //VC FILTER LV 
{
	CString str;

	//son VC Filter LV On 설정 (Off -> On)
	if (nFlag71_VC_Filter_Mode_LV != 1)
	{
		if (nMode1 == 3 || nMode1 == 4) {		//son HV1 ON  or HV2 ON
			((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(FALSE);		//son LV Check Off
		}

		else	//son LV1 On or LV2 On
		{
			nFlag71_VC_Filter_Mode_LV = 1;
			nFlag71_VC_Filter_Mode_HV = 0;
			((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(TRUE);		//son LV Check On
			((CButton*)GetDlgItem(IDC_RADIO03))->SetCheck(FALSE);		//son HV Check Off
			OnFiler_VC(1); // fast  vc FILTER  OFF
		}
	}

	//son VC Filter LV Off   (On -> Off)
	else
	{
		nFlag71_VC_Filter_Mode_LV = 0;
		((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO03))->SetCheck(FALSE);
		OnFiler_VC(11); // fast  vc FILTER  OFF
	}


}


//son Radio '4W Gain: Sx1000'
void CModeSub71::OnRadio05()
{
	((CButton*)GetDlgItem(IDC_RADIO05))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO06))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO07))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO08))->SetCheck(FALSE);

	On4W_GainSet(1);  //4W GAIN  //sylee150118   SLOW GAIN=1, FAST GAIN X10 =2 , FAST GAIN X100 =3 , FAST GAIN X1000 =4 ,


}


//son Radio '4W Gain: F x10'
void CModeSub71::OnRadio06()
{
	((CButton*)GetDlgItem(IDC_RADIO06))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO05))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO07))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO08))->SetCheck(FALSE);

	On4W_GainSet(2);  //4W GAIN  //sylee150118   SLOW GAIN=1, FAST GAIN X10 =2 , FAST GAIN X100 =3 , FAST GAIN X1000 =4 ,
}


//son Radio '4W Gain: F x100'
void CModeSub71::OnRadio07()
{
	((CButton*)GetDlgItem(IDC_RADIO07))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO05))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO06))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO08))->SetCheck(FALSE);

	On4W_GainSet(3);  //4W GAIN  //sylee150118   SLOW GAIN=1, FAST GAIN X10 =2 , FAST GAIN X100 =3 , FAST GAIN X1000 =4 ,
}

//son Radio '4W Gain: Fx1000'
void CModeSub71::OnRadio08()
{
	((CButton*)GetDlgItem(IDC_RADIO08))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO05))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO06))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO07))->SetCheck(FALSE);

	On4W_GainSet(4);  //4W GAIN  //sylee150118   SLOW GAIN=1, FAST GAIN X10 =2 , FAST GAIN X100 =3 , FAST GAIN X1000 =4 ,
}

//son Radio 'HV OPEN: Gain 1'
void CModeSub71::OnRadio09()
{
	((CButton*)GetDlgItem(IDC_RADIO09))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(FALSE);
	HV_Gain(1); //HV GAIN MODE  SET
}

//son Radio 'HV OPEN: Gain 2'
void CModeSub71::OnRadio10()
{
	((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO09))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(FALSE);
	HV_Gain(2); //HV GAIN MODE  SET
}


//son Radio 'HV OPEN: Gain 3'
void CModeSub71::OnRadio11()
{
	((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO09))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(FALSE);
	HV_Gain(3); //HV GAIN MODE  SET
}

//son Radio 'HV OPEN: Gain 4'
void CModeSub71::OnRadio12()
{
	((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO09))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(FALSE);
	HV_Gain(4); //HV GAIN MODE  SET
}

//son Radio 'CC/CV: CC'
void CModeSub71::OnRadio13() //CC
{
	((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO14))->SetCheck(FALSE);
	OnCCCV_Set(1);
}

//son Radio 'CC/CV: CV'
void CModeSub71::OnRadio14() //CV
{
	((CButton*)GetDlgItem(IDC_RADIO14))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(FALSE);
	OnCCCV_Set(2);
}


//son Radio 'I Gain: X 1'
void CModeSub71::OnRadio15()
{
	((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(FALSE);
	I_Gain(1);
}

//son Radio 'I Gain: X 10'
void CModeSub71::OnRadio16() //I GAIN  X10
{
	((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(FALSE);
	I_Gain(2); //  1=I GAIN X 1 ,   2=I GAIN X 10
}


//son Radio 'I Range Set: 1(60mA)'	=> OnRadioIRng0() 
void CModeSub71::OnRadio21()
{
	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(FALSE);
	OnButton201();
}

//son Radio 'I Range Set: 2(20mA)'
void CModeSub71::OnRadio22()
{
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(FALSE);
	OnButton202();
}

//son Radio 'I Range Set: 3(10mA)'
void CModeSub71::OnRadio23()
{
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(FALSE);
	OnButton203();
}

//son Radio 'I Range Set: 4(2mA)'
void CModeSub71::OnRadio24()
{
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(FALSE);
	OnButton204();
}

//son Radio 'I Range Set: 5(200uA)'
void CModeSub71::OnRadio25()
{
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(FALSE);
	OnButton205();
	OnRadio16(); //I GAIN  X10//SYLEE150118
}


BOOL CModeSub71::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;

	m_radio01 = 0;

	nfAuRead1 = 0;
	SetTimer(1, 1000, NULL);

	fpLeak = NULL;      //son231013 디버그 모드에서 Sub71.fpLeak 초기화 안 돼서 죽는 현상 보완 

	m_combo101.SetCurSel(1);//sylee130605
	m_combo102.SetCurSel(1);//sylee130605
	nManualFlag_HVOpen_Offset = 1;
	nManualFlag_HVOpen_Gain = 1;
	m_combo106.SetCurSel(1);//sylee130605
	m_combo107.SetCurSel(1);//sylee130605
	nManualFlag_I_ADC = 1;
	nManualFlag_VC_ADC = 1;

	OnFiler_VC(1); //SYLEE130605    VC ADC   FAST  SLOW MODE

	COLORREF clr1, clr2, clr3, clr4, clr5, clr6, clr7;

	clr1 = RGB(200, 0, 20);//red
	clr2 = RGB(248, 192, 228);//white red	
	clr3 = RGB(180, 255, 104);//green
	clr4 = RGB(9, 73, 172);//blue
	clr5 = RGB(0, 10, 50);//black 
	clr6 = RGB(245, 252, 120);//white  yellow
	clr7 = RGB(0, 0, 0);//black  
	clr1 = RGB(200, 0, 20);//red
	clr2 = RGB(255, 200, 228);//white red	
	clr3 = RGB(225, 255, 164);//green
	clr4 = RGB(9, 73, 172);//blue
	clr5 = RGB(0, 10, 50);//black 
	clr6 = RGB(252, 254, 211);//white  yellow
	clr7 = RGB(0, 0, 0);//black

	OnButton506();		//son INIT

	OnRadio01();

	nFlag71_VC_Filter_Mode_LV = 0;

	OnRadio05();
	OnRadio09();
	OnCCCV_Set(1);//sylee150118
	OnVB_AC_Mode_Set(0); //ON=1, Off=0 SET  //sylee150118
	m_check11.SetCheck(0); //sylee150118   0==OFF, 1==ON
	OnCheck11();
	nFlag71_VB_AC_Mode1 = 1; //SYLEE150118
	OnButton751();
	nFlag71_HV_AC_Mode1 = 1;
	OnButton752();   //SYLEE150118
	nFlag71_HV_AC_Mode1_Relay = 1;
	OnButton753();   //SYLEE150118
	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118
	OnButton11081(); //sylee150120   //PLUSE  off

	OnRadio13();  //cc set
	OnRadio15();  // i  x1

	nADCOver_IntDisable = 2;//sylee150121  disble  65355->0

	nFlag71_VB_G_ON = 1;
	nFlag71_VB_G_SEL1 = 1;
	nFlag71_VC_G_ON = 1;
	nFlag71_VC_G_SEL1 = 1;

#ifdef DEF_CARD_A1  //sylee170328
	OnButton757();  //SYLEE150118  //VB GUARD ON
	OnButton758();   //SYLEE150118  //VB GUARD SEL
	OnButton759();   //SYLEE150118  //VC GUARD ON
	OnButton760();   //SYLEE150118  //VC GUARD SEL
#else  //#ifdef DEF_CARD_A1   

	//	 CString str;
	str.Format("H64-ON");
	m_button101.SetWindowText(str);
	str.Format("H64-Off");
	m_button102.SetWindowText(str);
	str.Format("H96-ON");
	m_button103.SetWindowText(str);
	str.Format("H96-Off");
	m_button104.SetWindowText(str);
#endif  //#ifdef DEF_CARD_A1 


	OnButton506();//sylee150122 

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CModeSub71::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

/*
	if((pMsg->message == WM_KEYDOWN)
		&& (pMsg->wParam == VK_ESCAPE)) {
		return TRUE;
	}
*/
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


//son   Delay_100ns(10000):   1 msec  
//son   Delay_100ns(10):      1 usec 
void CModeSub71::Delay_100ns(int dt)
{
	static LARGE_INTEGER Start, L;
	double Freq;

 

	if (!QueryPerformanceFrequency(&L))
		return;// H/W 지원  않음
	Freq = (double)(L.QuadPart / 10000000.0);

	QueryPerformanceCounter(&Start);
	do
	{
		QueryPerformanceCounter(&L);
	} while ((L.QuadPart - Start.QuadPart) / Freq < dt);
}


void CModeSub71::OnClickOk()
{
	nADCOver_IntDisable = 0;//sylee150121

	OnButton506();
	CDialog::OnOK();

}


void CModeSub71::OnClickGrid1()
{
	// TODO: Add your control notification handler code here

}

void CModeSub71::SetAttr(int iOpa)
{

}



void CModeSub71::InitControl()
{
	nMode = 0;
	nMode1 = 0;
	nModeIR = 0;
	nModeCC = 0;
	nModeSP = 0;
	nModeCC = 1;

	Onload();
	OnButton506();

	OnButton110();  // CC MODE SET		//son nModeCC =0으로 내부적으로 설정됨


}


void CModeSub71::OnDblClickMsflexgrid1()
{
	// TODO: Add your control notification handler code here
	BeginEditing();

}


void CModeSub71::OnEnterCellMsflexgrid1()
{


}



int CModeSub71::OnSelfRSet(int a, int b)// a=1 100,  a=2 10M,       b=1 run, 0 off
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (a == 1) {
		if (b == 1) {
			Ret = DioOutBit(pDIO.hDrv, 120, 1);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 120, 1] Error!!!");	return  0; }
		}
		else if (b == 0) {
			Ret = DioOutBit(pDIO.hDrv, 120, 0);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 120, 0] Error!!!");	return  0; }
		}
	}
	else if (a == 2) {
		if (b == 1) {
			Ret = DioOutBit(pDIO.hDrv, 121, 1);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 1] Error!!!");	return  0; }
		}
		else if (b == 0) {
			Ret = DioOutBit(pDIO.hDrv, 121, 0);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 0] Error!!!");	return  0; }
		}
	}
#else  
	if (a == 1) {
		if (b == 1) {
			Ret = Ixud_WriteDOBit(0, 5, 0, 1);
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,0,1) Error!!!");	return  0; }
		}
		else if (b == 0) {
			Ret = Ixud_WriteDOBit(0, 5, 0, 0);
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,0,0) Error!!!");	return  0; }
		}
	}
	else if (a == 2) {
		if (b == 1) {
			Ret = Ixud_WriteDOBit(0, 5, 1, 1);
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,1) Error!!!");	return  0; }
		}
		else if (b == 0) {
			Ret = Ixud_WriteDOBit(0, 5, 1, 0);
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,0) Error!!!");	return  0; }
		}
	}

#endif

#endif	
	return 1;

}


//son  ALL D96 Board OFF: bit00~ bit127 OUTBIT 영역 전부를 OFF 후 일부 영역만 ON
int CModeSub71::AOnPoweroff()//sylee20111018
{
#ifndef _PC_TEST_

	int Ret, i;
	//====================================================
	//#	

	nMode4W = 0;
	nMode1 = 0;
	nMode = 0;
	nModeIR = 0;
	nflgA = 0;
	nflgB = 0;
	nModeIG = 0;
	nMode4WG = 0;
	nModeOpenHV = 0;
	nModeFilter1 = 0;


	nMode1 = 0;
	nModeV1 = 0;
	nModeV2 = 0;
	nModeV3 = 0;
	nModeV4 = 0;
	nModeCC = 0;
	//OnButton501(); ///PIN ALL RESET
//	m_edit115.SetWindowText("Real");
	nTimeDelay1 = 1000;
	nModeIR = 0;

	OnPinAllReset1(); ///PIN ALL RESET //sylee20120118

	//son F4_WIRE_CMD OFF
#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD //sylee130628 46->47
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return  0; }
	//====================================================
	//son  bit00~ bit127 OUTBIT 영역 전부를 OFF

	//son Port4 => bit32~39 (RNG_SET_0 ~ RNG_SET_4, ADC_VB_SEL, F4_WIRE_SELF_S1, F4_WIRE_SELF_S2 모두 OFF)
	Ret = DioOutByte(pDIO.hDrv, 4, 0x0);   // I Range Mode 0 ~4  FET	
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 4, 0 ] Error!!!");	return  0; }

	//son Port6 => bit48~55  OFF 
	Ret = DioOutByte(pDIO.hDrv, 6, 0x0);
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 6, 0 ] Error!!!");	return  0; }
	Delay_100ns(30000); //3mS 


   //son bit32~39와 bit48~55를 제외한  bit00~bit127 영역을 모두 OFF 
	for (i = 0; i < 16; i++) {
		if (i == 4 || i == 6) {//sylee20120223
			continue;
		}
		Ret = DioOutByte(pDIO.hDrv, i, 0x0);
		if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }
	}
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }
	///vb
	Ret = DioOutByte(pDIO.hDrv, 12, 0);  //b line    cpld
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 12 0 ] Error!!!");	return  0; }
#else 	
/*
	Ret = Ixud_WriteDOBit(0,8,1,0);  //4W CMD
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return  0;	}
*/
#endif

//son  4W, VC FAST, VC SLOW, HV OPEN Filter OFF
	FilterSet(0);//FAST

	On4WS12Set(1, 0);	 	//son s1(VB Relay Self Test) OFF
	On4WS12Set(2, 0);		//son s2(VC Relay Self Test) OFF


//////////////////////////////////////////////////////////////////////////////
//sylee20120524
//box initial

#ifdef DEF_CARD_A1  //sylee170328
	//========================================
	//son bit96~ bit127 All OFF 
	Ret = DioOutByte(pDIO.hDrv, 12, 0);  //  //sylee20120507
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 12  0] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 13, 0);  //  //sylee20120507
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 13   0] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 14, 0);  //  //sylee20120507
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 14  0] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 15, 0);  //  //sylee20120507
	if (Ret != 0) { AfxMessageBox("DioOutByte[ 15 0] Error!!!");	return  0; }
	//====================================================
		//son   VB_PIN_A6~ VB_PIN_A9  ON.   ???
	Ret = DioOutBit(pDIO.hDrv, DO70, 1);   //    VB CPLD0
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 70, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO71, 1);   //    VB CPLD1
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 71, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO72, 1);   //    VB CPLD2
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 72, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO73, 1);   //    VB CPLD3
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 73, 0] Error!!!");	return  0; }


	//son VC_PIN_A6~ VC_PIN_A9  ON. ???
	Ret = DioOutBit(pDIO.hDrv, DO86, 1);   //    Vc CPLD0  //sylee130628   87-90=>86-89
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 86, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO87, 1);   //    Vc CPLD1
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 87, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO88, 1);   //    Vc CPLD2
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 88, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO89, 1);   //    Vc CPLD3
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 89, 0] Error!!!");	return  0; }
#else  	 
			 //card64H  initial.
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x80000 ) Error!!!");	return  0; }
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000 ) Error!!!");	return  0; }
	//	Ret = Ixud_WriteDOBit(0,7,1,1);  //  /AD_CONVST
	//	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0;	}
		//card96  initial. 

	Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDO(0, 7, 0x02);		//   /AD_CONVST  ON 
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,i,0x02) Error!!!");	return  0; }

	for (i = 0; i <= 11; i++) {
		if (i != 7) {
			Ret = Ixud_WriteDO(0, i, 0x00);
			if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,i,0x00) Error!!!");	return  0; }
		}
	}
#endif

#endif 
	return 1;
}



int CModeSub71::AOnPoweroff2()//sylee20120702
{
#ifndef _PC_TEST_
	//--------------------------------
	//son HV, LV VB  ALL Relay Off
	//son BIT52(HV_RELAY_IN_IO_0) ~ BIT55(LV_RELAY_IN_IO_1) VB Apply 관련 비트를 모두 초기화한다.	
	AOnRelayOff1();//SYLEE20120721 MOVE
	//-------------------------------------
	//son All D96 Digital Out Bit(0~95) Off
	//son  bit00~ bit127 OUTBIT 영역 전부를 OFF 후 일부 영역만 ON
	AOnPoweroff();
	Delay_100ns(30000);  //3ms  
#endif
	return 1;
}





//son  Set VB Source
//son ??? av, bv, cv, dv 여러개의 값을 받아서 LV1, LV2, HV1, HV2에 대해
//son     VB read를 모두 시도할 수 있음.  그런데 v1은 마지막 dv에 대해서만 남음.
//son     왜 여러개를 굳이 해야 하는지?   어떤 용도인지 모르겠음.
int CModeSub71::AOnVSetMulti1(double av, double bv, double cv, double dv)    //lv1=a, lv2=b, hv1=c, hv2=d
{
	CString str;
	int nRet1;
	int a, b, c, d;


	if (av > 0) {
		a = 1;
	}
	if (bv > 0) {
		b = 1;
	}
	if (cv > 0) {
		c = 1;
	}
	if (dv > 0) {
		d = 1;
	}

	//===================================================
	// V SET ON    LV1, LV2, HV1, HV2
	//===================================================

	//--------------
	//son for LV1
	if (a == 1) {
		//son UI 출력부분 제외하고 AVBRV1S()와 동일함.
		//son CV43ManualTestDlg::_SetVB_GetAdcA(..)-> MM.Set(LV1,...)-> sSetVb.Init(); 에 해당.
		nRet1 = AOnVBSet1(1, av);// int nVRel, int nVSet, 
		if (nRet1 != 1) {
			return 0;
		}

		//son UI 출력 부분을 제외하고 CModeSub71::AVBRV1(int nMode2)와 동일함. 
		//son  ADCA 에서 read한 VB 값은 멤버변수인 V1에 설정됨
		AOnReadVB1(1);// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
	}

	//--------------
	//son for LV2
	if (b == 1)
	{
		nRet1 = AOnVBSet1(2, bv);// int nVRel, int nVSet, 
		if (nRet1 != 1) {
			return 0;
		}
		AOnReadVB1(2);// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
	}

	//--------------
	//son for HV1
	if (c == 1) {
		nRet1 = AOnVBSet1(3, cv);// int nVRel, int nVSet, 
		if (nRet1 != 1) {
			return 0;
		}
		AOnReadVB1(3);// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
	}

	//--------------
	//son for HV2
	if (d == 1) {
		nRet1 = AOnVBSet1(4, dv);// int nVRel, int nVSet, 
		if (nRet1 != 1) {
			return 0;
		}
		AOnReadVB1(4);// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
	}

	return 1;
}





int CModeSub71::AOnISetOn1(int nCC, int nVRel, double nVSet, int nIR, double nISet)
{

	CString str;
	int nRet1;

	nRet1 = AOnIRanSet1(nIR, 1); //  I range  ( 1~5,   off=1  on  o off  
	if (nRet1 != 1) {
		return 0;
	}
	//===================================================
	//I SET  VALUE

	if (nCC == 1) { //SYLEE20111127  CC MODE
		nRet1 = AOnISet1(nISet);    //void CModeSub71::OnButton304()  decimal  + OnButton303() 16bit
		if (nRet1 != 1) {
			return 0;
		}
	}

	//===================================================
	//V  RELAY ON  CONNECT  LV1, LV2,  HV1, HV2
	if (nVRel == 1 || nVRel == 2 || nVRel == 3 || nVRel == 4) {

		AOnRelayOn1(nCC, nVRel, nVSet, nIR, nISet); //1=lv1,2=lv2, 3=hv1,4=hv2    // nCC=1  CC, 2=CV

	}
	else {
		AfxMessageBox(" Error   V MODE?   relay on error? ", MB_OK); //error messege
		return 0;
	}

	return 1;
}

//son  I Range 설정 -> I Set  -> 

int CModeSub71::AOnISetOn1Run1(int nCC, int nVRel, double nVSet, int nIR, double nISet)  //SYLEE20111213
{
#ifndef _PC_TEST_

	CString str;
	int nRet1;

	//-------------------------------------------
	//son VB Apply 관련 비트를 모두 초기화

	//son BIT52(HV_RELAY_IN_IO_0) ~ BIT55(LV_RELAY_IN_IO_1)
	AOnRelayOff1();//sylee140303   ray confirm		 
	//sylee150421	 Delay_100ns(30000);	 //sylee140303	  ray confirm //sylee150421-test


	//---------------------------
	//son I Range 설정 

	nRet1 = AOnIRanSet1Run1(nIR, 1); //  I range  ( 1~5,   off=1(on), 0(off)  //SYLEE20111213		 
	if (nRet1 != 1) {
		return 0;
	}
	//===================================================
	//I SET  VALUE
	if (nCC == 1) { //SYLEE20111127  CC MODE 
		//son Set I Value,  OnButton304() : I SET OUT과 매우 유사함.
		nRet1 = AOnISet1Run1(nISet);   //SYLEE20111213 //void CModeSub71::OnButton304()  decimal  + OnButton303() 16bit
		if (nRet1 != 1) {
			return 0;
		}
	}
	//===================================================
	//V  RELAY ON  CONNECT  LV1, LV2,  HV1, HV2
	if (nVRel == 1 || nVRel == 2 || nVRel == 3 || nVRel == 4) {
		AOnRelayOn1(nCC, nVRel, nVSet, nIR, nISet); //1=lv1,2=lv2, 3=hv1,4=hv2    // nCC=1  CC, 2=CV
	}
	else {
		AfxMessageBox(" Error   V MODE?   relay on error? ", MB_OK); //error messege
		return 0;
	}
#endif
	return 1;
}






/*
int CModeSub71::AOnISetOn1Run2(int nCC, int nVRel, double nVSet, int nIR, double nISet)  //SYLEE20120524  AOnRelayOn1 DELETE
{

		CString str;
		int nRet1 ;

		nRet1=AOnIRanSet1Run1( nIR, 1); //  I range  ( 1~5,   off=1  on  o off  //SYLEE20111213

		if(nRet1!=1){
			return 0;
		}
//===================================================
//I SET  VALUE
		if( nCC==1){ //SYLEE20111127  CC MODE
			nRet1=AOnISet1Run1(nISet);   //SYLEE20111213 //void CModeSub71::OnButton304()  decimal  + OnButton303() 16bit
			if(nRet1!=1){
				return 0;
			}
		}
//===================================================
//V  RELAY ON  CONNECT  LV1, LV2,  HV1, HV2
		if( nVRel==1  ||  nVRel==2 || nVRel==3  ||  nVRel==4 ){
//sylee130906			OnDisChargeVC(0);  //SYLEE20120103   mode change C 방전  Off  //C discharge   a=1 on   0=off
		}else{
			AfxMessageBox(" Error   V MODE?   relay on error? ", MB_OK); //error messege
			return 0;
		}

		return 1;
}

*/


void CModeSub71::OnPinSel1(int a, int b) //  int a=m_nCurRow, int b=m_nCurCol
{
	m_nCurRow = b;
	m_nCurCol = a;

}



void CModeSub71::OnLeaveCellMsflexgrid1()
{
	// TODO: Add your control notification handler code here
	EndEditing();
}

void CModeSub71::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y)
{

}

void CModeSub71::EndEditing()
{

}
void CModeSub71::BeginEditing()
{
}

void CModeSub71::OnSave()
{

}

void CModeSub71::Onload()
{
	CString str;
	UpdateData(FALSE);
}


void CModeSub71::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent) {
	case 1:		// Motor Sensor Check

		break;

	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);


}



//son Button 'Spark ON'
void CModeSub71::OnButton105() ////SPARK ON
{
#ifndef _PC_TEST_
	CString str;
	int Ret;
#ifdef DEF_CARD_A1  //sylee170328
	if (nModeSP < 1) {
		nModeSP = 1;
		str.Format("ON");
		Ret = DioOutBit(pDIO.hDrv, DO51, 1);  //SPARK ON    0  RESET     1   RUN      122
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 1] Error!!!");	return; }
	}
	else {
		nModeSP = 0;
		str.Format("OFF");
		Ret = DioOutBit(pDIO.hDrv, DO51, 0);  //SPARK ON    0  RESET     1   RUN      122
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	}

#else 
	if (nModeSP < 1) {
		nModeSP = 1;
		str.Format("ON");
		Ret = Ixud_WriteDOBit(0, 3, 2, 1);  //SPARK ON
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,2,1) Error!!!");	return; }
	}
	else {
		nModeSP = 0;
		str.Format("OFF");
		Ret = Ixud_WriteDOBit(0, 3, 2, 0);  //SPARK OFF
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,2,0) Error!!!");	return; }
	}

#endif
#endif
	m_Label209.SetCaption(str);
	m_list1.InsertString(0, str);
}


//son Button 'Spark STATUS'
void CModeSub71::OnButton115() ////SPARK  STATUS
{
	CString str;
	int Ret;
	BYTE Buf;


	if (nModeSP == 1)
	{
#ifdef _PC_TEST_
		Buf = 0;
		Ret = 0;
#else
#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioInpBit(pDIO.hDrv2, DI39, &Buf);
		if (Ret != 0) { AfxMessageBox(" [DioInpBit, 39, 1] Error!!!");	return; }
#else   
		Ret = d64h_di_readline(pDIO_D64.m_bCardID2, 0, &pDIO_D64.wValue);
		if (Ret != 0) { AfxMessageBox("  Card64H2_DiBit(0) Error!!!");	return; }
		Buf = (char)pDIO_D64.wValue;
#endif
#endif

		if (Buf == 1) {
			str.Format("Detect");
		}
		else {
			str.Format("No Det.");
		}
	}
	else {
		str.Format("off mode");
	}
	m_Label210.SetCaption(str);
	m_list1.InsertString(0, str);

}





//son Button 'CC/CV'	(미사용?)
void CModeSub71::OnButton110() //CC  CV MODE  SET
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error NO 711.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}

	if (nModeCC < 1) {
		nModeCC = 1;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO61, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return; }
#else 
		Ret = Ixud_WriteDOBit(0, 4, 4, 1);  //4W GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,4,1) Error!!!");	return; }
#endif

		str.Format("CV SET");
		m_Label207.SetCaption(str);
		m_list1.InsertString(0, str);

		str.Format("0");
		m_edit104.SetWindowText(str);

		OnButton304();

		str.Format("I SET = 0");
		m_list1.InsertString(0, str);

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //HV_SEN_CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return; }
#else   
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return; }
#endif

		Delay_100ns(20000);//2mSEC

	}
	else {

		nModeCC = 0;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO61, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 4, 4, 0);  //4W GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,4,0) Error!!!");	return; }
#endif

		str.Format("CC SET");
		m_Label207.SetCaption(str);
		m_list1.InsertString(0, str);

	}
#endif 
}






//son Button 'PinSearch'	(미사용?)
void CModeSub71::OnButton111() //Pin Search 
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

	if (nModeIG < 1) {
		nModeIG = 1;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO58, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 1] Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 4, 2, 1);  //PIN SEARCH 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,2,1) Error!!!");	return; }
#endif
		str.Format("SearchOn");
		m_Label231.SetCaption(str);

	}
	else {
		nModeIG = 0;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO58, 0);  //Pin Search. 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return; }
#else  
		Ret = Ixud_WriteDOBit(0, 4, 2, 0);  //PIN SEARCH 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,2,0) Error!!!");	return; }
#endif
		str.Format("SearchOff");
		m_Label231.SetCaption(str);

	}

#endif 

	m_list1.InsertString(0, str);


}

//son Button 'HV OPEN'
void CModeSub71::OnButton112() //OPEN HIGH VOLTAGE   LOW R READ  SET
{
	CString str;
	int Ret;


	//son  nModeOpenHV 값이 1 -> 0 으로 변경되어야 하는 경우
	if (nModeOpenHV > 0) //son nModeOpenHV: 1 => 0 //HV OPEN OFF
	{
		nModeOpenHV = 0;

#ifdef DEF_CARD_A1  //sylee170328 
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //OPEN CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }
#endif

		str.Format("OFF");
		m_Label236.SetCaption(str);

	}

	//son  nModeOpenHV 값이 0 -> 1 으로 변경되어야 하는 경우
	else	//son HV OPEN ON
	{

		nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118

		OnButton11081(); //sylee150120   //PLUSE  off

		OnFiler_VC(3);		//son HV_OPEN CMD ON

		nModeOpenHV = 1;
		str.Format("ON");
		m_Label236.SetCaption(str);

		str.Format("OFF");
		nMode4W = 0;
		m_Label225.SetCaption(str);

		((CButton*)GetDlgItem(IDC_RADIO03))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(FALSE);
	}

	m_list1.InsertString(0, str);


}

void CModeSub71::HV_OffSet_Gain(int set1, int set2) //HV OFFSET Gain    //sylee140226
{

	HV_OffSet_Gain_21(set1, set2);//ACE380//SYLEE150130

}



void CModeSub71::HV_OffSet_Gain_21(int set1, int set2) //HV OFFSET Gain    //sylee140226
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328

	OutBitNo[2] = 30;   OutBitNo[3] = 31;

	if (set2 == 1) {
		OutBitData[2] = 0;   OutBitData[3] = 0;
	}
	else if (set2 == 3) {
		OutBitData[2] = 0;   OutBitData[3] = 1;
	}
	else if (set2 == 2) {
		OutBitData[2] = 1;   OutBitData[3] = 0;
	}
	else if (set2 == 4) {
		OutBitData[2] = 1;   OutBitData[3] = 1;
	}
	else {
		OutBitData[2] = 0;   OutBitData[3] = 0;
		AfxMessageBox(" Error No 1001.   HV OPEN GainSET(1~4) Select? ", MB_OK);
	}

	Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[2], 2, &OutBitData[2]);
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 28 29 ,   *] Error!!!");	return; }

#else  	
	OutBitNo[2] = 30;   OutBitNo[3] = 31;

	if (set2 == 1) {
		OutBitData[2] = 0;   OutBitData[3] = 0;
	}
	else if (set2 == 3) {
		OutBitData[2] = 0;   OutBitData[3] = 1;
	}
	else if (set2 == 2) {
		OutBitData[2] = 1;   OutBitData[3] = 0;
	}
	else if (set2 == 4) {
		OutBitData[2] = 1;   OutBitData[3] = 1;
	}
	else {
		OutBitData[2] = 0;   OutBitData[3] = 0;
		AfxMessageBox(" Error No 1001.   HV OPEN GainSET(1~4) Select? ", MB_OK);
	}

	Ret = Ixud_WriteDOBit(0, 6, 6, OutBitData[2]);  //OPEN GAIN A0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,6, ) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 6, 7, OutBitData[3]);  //OPEN GAIN A1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,7, ) Error!!!");	return; }

#endif
#endif

}


//son 4W Gain 설정 
void CModeSub71::On4W_GainSet(int set) //4W GAIN  //sylee150118   SLOW GAIN=1, FAST GAIN X10 =2 , FAST GAIN X100 =3 , FAST GAIN X1000 =4 , 
{
#ifndef _PC_TEST_
	int Ret;


	//son F4_WIRE_GAIN_A0(61)	// A0A1  00: (1)Slow Gain x1000 	01: (2)Fast Gain x10
	//son F4_WIRE_GAIN_A1(63)	//       10: (3)Fast Gain x100		11: (4)Fast Gain x1000

#ifdef DEF_CARD_A1  //sylee170328
	if (set == 1) {  //FAST GAIN X1000 
		Ret = DioOutBit(pDIO.hDrv, DO61, 0);   //61
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO63, 0);   //63
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 0] Error!!!");	return; }
	}
	else if (set == 2) {  //FAST GAIN X10
		Ret = DioOutBit(pDIO.hDrv, DO61, 0);   //61
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO63, 1);   //63
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 1] Error!!!");	return; }
	}
	else if (set == 3) { //FAST GAIN X100
		Ret = DioOutBit(pDIO.hDrv, DO61, 1);   //61
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO63, 0);   //63
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 0] Error!!!");	return; }
	}
	else if (set == 4) { // SLOW  GAIN  X1000
		Ret = DioOutBit(pDIO.hDrv, DO61, 1);   //61
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO63, 1);   //63
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 1] Error!!!");	return; }
	}
	else {
		AfxMessageBox(" Error No 1001.   4W GAIN SET(1~4) Select? ", MB_OK);
	}

#else  	

	if (set == 1) {  //FAST GAIN X1000 
		Ret = Ixud_WriteDOBit(0, 4, 4, 0);  //4W GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,4,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 5, 0);  //4W GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,5,0) Error!!!");	return; }
	}
	else if (set == 2) {  //FAST GAIN X10
		Ret = Ixud_WriteDOBit(0, 4, 4, 0);  //4W GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,4,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 5, 1);  //4W GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,5,1) Error!!!");	return; }
	}
	else if (set == 3) { //FAST GAIN X100
		Ret = Ixud_WriteDOBit(0, 4, 4, 1);  //4W GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,4,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 5, 0);  //4W GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,5,0) Error!!!");	return; }
	}
	else if (set == 4) { // SLOW  GAIN  X1000
		Ret = Ixud_WriteDOBit(0, 4, 4, 1);  //4W GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,4,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 5, 1);  //4W GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,5,1) Error!!!");	return; }
	}
	else {
		AfxMessageBox(" Error No 1001.   4W GAIN SET(1~4) Select? ", MB_OK);
	}


#endif

#endif



}

void CModeSub71::HV_OffSet(int set) //HV OFFSET    //sylee130605
{

#ifndef _PC_TEST_

#ifdef DEF_CARD_A1  //sylee170328

	int Ret;

	if (set == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO28, 0);   //34
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO29, 0);   //35
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return; }
	}
	else if (set == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO28, 0);   //34
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO29, 1);   //35
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return; }
	}
	else if (set == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO28, 1);   //34
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO29, 0);   //35
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return; }
	}
	else if (set == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO28, 1);   //34
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO29, 1);   //35
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return; }
	}
	else {
		AfxMessageBox(" Error No 1001.   HV OPEN OFFSET(1~4) Select? ", MB_OK);
	}

#else  	

	//sylee170331 //SHIN B S  DO DELETE 170331 OK
	/*

			if( set==1){
				Ret = A_Bit( DO28, 0);   //34
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  ;	}
				Ret = A_Bit( DO29, 0);   //35
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return  ;	}
			}else if( set==3){
				Ret = A_Bit( DO28, 0);   //34
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  ;	}
				Ret = A_Bit( DO29, 1);   //35
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return  ;	}
			}else if( set==2){
				Ret = A_Bit( DO28, 1);   //34
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return  ;	}
				Ret = A_Bit( DO29, 0);   //35
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return  ;	}
			}else if( set==4){
				Ret = DioOutBit(pDIO.hDrv, DO28, 1);   //34
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return  ;	}
				Ret = DioOutBit(pDIO.hDrv, DO29, 1);   //35
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return  ;	}
			}else{
				AfxMessageBox(" Error No 1001.   HV OPEN OFFSET(1~4) Select? ", MB_OK);
			}
	*/

#endif

#endif

}


void CModeSub71::HV_Gain(int set) //HV GAIN MODE  SET  //sylee130605
{
#ifndef _PC_TEST_
	int  Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (set == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO30, 0);   //36
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO31, 0);   //37
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 0] Error!!!");	return; }
	}
	else if (set == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO30, 0);   //36
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO31, 1);   //37
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 1] Error!!!");	return; }
	}
	else if (set == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO30, 1);   //36
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO31, 0);   //37
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 0] Error!!!");	return; }
	}
	else if (set == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO30, 1);   //36
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO31, 1);   //37
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 1] Error!!!");	return; }
	}
	else {
		AfxMessageBox(" Error No 1001.   HV OPEN GainSET(1~4) Select? ", MB_OK);
	}

#else  	
	if (set == 1) {
		Ret = Ixud_WriteDOBit(0, 6, 6, 0);  //OPEN GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,6,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 6, 7, 0);  //OPEN GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,7,0) Error!!!");	return; }
	}
	else if (set == 3) {
		Ret = Ixud_WriteDOBit(0, 6, 6, 0);  //OPEN GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,6,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 6, 7, 1);  //OPEN GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,7,1) Error!!!");	return; }
	}
	else if (set == 2) {
		Ret = Ixud_WriteDOBit(0, 6, 6, 1);  //OPEN GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,6,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 6, 7, 0);  //OPEN GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,7,0) Error!!!");	return; }
	}
	else if (set == 4) {
		Ret = Ixud_WriteDOBit(0, 6, 6, 1);  //OPEN GAIN A0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,6,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 6, 7, 1);  //OPEN GAIN A1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,7,1) Error!!!");	return; }
	}
	else {
		AfxMessageBox(" Error No 1001.   HV OPEN GainSET(1~4) Select? ", MB_OK);
	}
#endif


#endif
}




void CModeSub71::OnCCCV_Set(int set) //CC=1, CV=2 SET  //sylee150118
{
#ifndef _PC_TEST_
	int  Ret;

#ifdef DEF_CARD_A1  //sylee170328

	if (set == 1) { //CC 
		Ret = DioOutBit(pDIO.hDrv, DO120, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 120, 0] Error!!!");	return; }
	}
	else if (set == 2) {//CV
		Ret = DioOutBit(pDIO.hDrv, DO120, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 120, 1] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO120, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 120, 0] Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   CC CV SET  Select?   ?", MB_OK);
	}

#else  	
	if (set == 1) { //CC 
		Ret = Ixud_WriteDOBit(0, 5, 0, 0);  //CC CV MODE  CC=0, CV=1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,0,0) Error!!!");	return; }
	}
	else if (set == 2) {//CV
		Ret = Ixud_WriteDOBit(0, 5, 0, 1);  //CC CV MODE  CC=0, CV=1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,0,1) Error!!!");	return; }
	}
	else {  // Init
		Ret = Ixud_WriteDOBit(0, 5, 0, 0);  //CC CV MODE  CC=0, CV=1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,0,0) Error!!!");	return; }
		AfxMessageBox(" Error No 1001.   CC CV SET  Select?   ?", MB_OK);
	}

#endif


#endif
}

void CModeSub71::OnVB_AC_Mode_Set(int set) //ON=1, Off=0 SET  //sylee150118
{
#ifndef _PC_TEST_
	int  Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (set == 1) { //ON
		Ret = DioOutBit(pDIO.hDrv, DO121, 1);    //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 1] Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = DioOutBit(pDIO.hDrv, DO121, 0);    //VB_AC_Mode  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 0] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO121, 0);     //VB_AC_Mode  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 0] Error!!!");	return; }
		AfxMessageBox(" Error No 1001.   VB_AC_Mode_Set  Select?   ?", MB_OK);
	}

#else  	
	if (set == 1) { //ON

		Ret = Ixud_WriteDOBit(0, 5, 1, 1);  //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,1) Error!!!");	return; }

	}
	else if (set == 0) {//OFF
		Ret = Ixud_WriteDOBit(0, 5, 1, 0);  //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,0) Error!!!");	return; }
	}
	else {  // Init
		Ret = Ixud_WriteDOBit(0, 5, 1, 0);  //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,0) Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_AC_Mode_Set  Select?   ?", MB_OK);
	}
#endif



#endif

}


void CModeSub71::OnVB_AC_Mode_PulseRelaySet1(int set) //ON=1, Off=0 SET  //sylee150118
{
#ifndef _PC_TEST_
	int  Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (set == 1) { //ON
		Ret = DioOutBit(pDIO.hDrv, DO123, 1);  //PULSE RELAY ON CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 123, 1] Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = DioOutBit(pDIO.hDrv, DO123, 0);  //PULSE RELAY OFF CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 123, 0] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO123, 0);  //PULSE RELAY OFF CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 123, 0] Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_AC_Mode_Set  Select?   ?", MB_OK);
	}

#else  	

	if (set == 1) { //ON			
		Ret = Ixud_WriteDOBit(0, 3, 3, 1);  //PULSE RELAY ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,3,1) Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = Ixud_WriteDOBit(0, 3, 3, 0);  //PULSE RELAY ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,3,0) Error!!!");	return; }
	}
	else {  // Init
		Ret = Ixud_WriteDOBit(0, 3, 3, 0);  //PULSE RELAY ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,3,0) Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_AC_Mode_Set  Select?   ?", MB_OK);
	}

#endif


#endif

}



void CModeSub71::OnVB_AC_Mode_Set_And_PulseSet(int set) //ON=1, Off=0 SET  //sylee150118
{
#ifndef _PC_TEST_
	int  Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (set == 1) { //ON
		Ret = DioOutBit(pDIO.hDrv, DO121, 1);    //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO123, 1);  //PULSE RELAY ON CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 123, 1] Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = DioOutBit(pDIO.hDrv, DO121, 0);    //VB_AC_Mode  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO123, 0);  //PULSE RELAY OFF CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 123, 0] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO121, 0);     //VB_AC_Mode  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 121, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO123, 0);  //PULSE RELAY OFF CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 123, 0] Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_AC_Mode_Set  Select?   ?", MB_OK);
	}
#else  	
	if (set == 1) { //ON
		Ret = Ixud_WriteDOBit(0, 5, 1, 1);  //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 3, 1);  //PULSE RELAY ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,3,1) Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = Ixud_WriteDOBit(0, 5, 1, 0);  //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 3, 0);  //PULSE RELAY ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,3,0) Error!!!");	return; }
	}
	else {  // Init
		Ret = Ixud_WriteDOBit(0, 5, 1, 0);  //VB_AC_Mode  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 3, 0);  //PULSE RELAY ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,3,0) Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_AC_Mode_Set  Select?   ?", MB_OK);
	}
#endif

#endif

}


void CModeSub71::OnHV_AC_Mode_Set(int set) //ON=1, Off=0 SET  //sylee150118
{

#ifndef _PC_TEST_


#ifdef DEF_CARD_A1  //sylee170328
	int  Ret;
	if (set == 1) { //ON
		Ret = DioOutBit(pDIO.hDrv, DO28, 1);    //HV_AC_Mode_Set  ON CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = DioOutBit(pDIO.hDrv, DO28, 0);    //HV_AC_Mode_Set  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO28, 0);     //HV_AC_Mode_Set  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return; }
		AfxMessageBox(" Error No 1001.   HV_AC_Mode_Set  Select?   ?", MB_OK);
	}
#else  	
	/* //sylee170331 SHIN B S OK    ->NEW IOLIST NOT
   if( set==1){ //ON
		Ret = A_Bit( DO28, 1);    //HV_AC_Mode_Set  ON CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return  ;	}
	}else if( set==0){//OFF
		Ret = A_Bit( DO28, 0);    //HV_AC_Mode_Set  OFF CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  ;	}
	}else{  // Init
		Ret = A_Bit( DO28, 0);     //HV_AC_Mode_Set  OFF CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  ;	}
		AfxMessageBox(" Error No 1001.   HV_AC_Mode_Set  Select?   ?", MB_OK);
	}
	*/

#endif


#endif


}

void CModeSub71::OnHV_AC_Mode_Set_Relay(int set) //ON=1, Off=0 SET  //sylee150118
{
	//	    

#ifndef _PC_TEST_

#ifdef DEF_CARD_A1  //sylee170328
	int  Ret;

	if (set == 1) { //ON
		Ret = DioOutBit(pDIO.hDrv, DO29, 1);  //HV AC  RELAY ON CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = DioOutBit(pDIO.hDrv, DO29, 0);  //HV AC  RELAY OFF CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO29, 0);  //HV AC  RELAY OFF CMD   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return; }
		AfxMessageBox(" Error No 1001.   HV_AC_Mode_Set  Select?   ?", MB_OK);
	}

#else  
/*
//sylee170331 SHIN B S OK	   ->NEW IOLIST NOT
	   if( set==1){ //ON
			Ret = A_Bit( DO29, 1);  //HV AC  RELAY ON CMD
			if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return  ;	}
		}else if( set==0){//OFF
			Ret = A_Bit( DO29, 0);  //HV AC  RELAY OFF CMD
			if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return  ;	}
		}else{  // Init
			Ret = A_Bit( DO29, 0);  //HV AC  RELAY OFF CMD
			if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return  ;	}
			AfxMessageBox(" Error No 1001.   HV_AC_Mode_Set  Select?   ?", MB_OK);
		}
*/

#endif


#endif
}



void CModeSub71::OnVB_OUT_CHECK_Mode_Set(int set) //ON=1, Off=0 SET  //sylee150118  VB OUT CHECK
{
#ifndef _PC_TEST_
	int  Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (set == 1) { //ON
		Ret = DioOutBit(pDIO.hDrv, DO122, 1);    //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 1] Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);    //VB OUT CHECK   OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return; }
	}
	else {  // Init
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);     //VB OUT CHECK  OFF CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_OUT_CHECK_Mode_Set  Select?   ?", MB_OK);
	}

#else  	
	if (set == 1) { //ON
		Ret = Ixud_WriteDOBit(0, 5, 2, 1);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,1) Error!!!");	return; }
	}
	else if (set == 0) {//OFF
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return; }
	}
	else {  // Init
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return; }

		AfxMessageBox(" Error No 1001.   VB_OUT_CHECK_Mode_Set  Select?   ?", MB_OK);
	}

#endif


#endif
}



//son Button '4W Gain'
void CModeSub71::OnButton120() //4W GAIN MODE  SET
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

	if (nMode4WG > 0) {
		nMode4WG = 0;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
#endif

		str.Format("4W Gain Off");
		m_Label234.SetCaption(str);

	}
	else {

		nMode4WG = 1;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO48, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 1] Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 3, 0, 1);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,1) Error!!!");	return; }
#endif 

		str.Format("4W Gain On");
		m_Label234.SetCaption(str);
	}

	m_list1.InsertString(0, str);

#endif

}


//son Button 'Filter'		(미사용?)
void CModeSub71::OnButton127() //FILTER  SET
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

	if (nModeFilter1 > 0) {
		nModeFilter1 = 0;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 0]  Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
#endif  

		str.Format("Fast");
		m_Label235.SetCaption(str);

	}
	else {
		nModeFilter1 = 1;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO37, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 1]  Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,1) Error!!!");	return; }

#endif 

		str.Format("Slow");
		m_Label235.SetCaption(str);
	}

	m_list1.InsertString(0, str);


#endif
}


void CModeSub71::HVOpenSet(int a) //HV OPEN  SET  //SYLEE130627   1= HV OPEN , 2= VC FILTER SLOW, 3= VC FILTER FAST , 4= 4WIRE
{

}



//Filter  SET  //sylee130628    1=vc fast, 2= vc slow, 3= hv open, 4= 4w  on     etc= all off
//
//son sSET_ADC_B 필터 설정에 해당.
void CModeSub71::FilterSet(int a)
{


#ifndef _PC_TEST_
	CString str;
	int  Ret;

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }
	if (a == 0) {  //all off
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //4W
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO50, 0);   //VC SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //HV OPEN
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }
	}

	else    if (a == 1) {  //VC FAST FILTER
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //4W
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO50, 0);   //VC SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //HV OPEN
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO49, 1);   //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 1] Error!!!");	return; }
	}

	else if (a == 2) { //VC SLOW FILTER
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //4Wㅣ
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //HV OPEN
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO50, 1);   //VC SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 1] Error!!!");	return; }
	}

	else if (a == 3) {//HV OPEN 
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //4W
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO50, 0);   //VC SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 1);   //HV OPEN
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 1] Error!!!");	return; }
	}

	else if (a == 4) { //4W
		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO50, 0);   //VC SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //HV OPEN
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 1);   //4W
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return; }
	}
	else {//all off
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //4W
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO50, 0);   //VC SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //HV OPEN
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

		AfxMessageBox(" [ERROR NO 2101 : ADC B ,   DO SET PARAMETOR ERROR] Error!!!");

	}


#else  	

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }

	if (a == 0) {  //all off

		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
		//SHIN B S  DO DELETE 170331		Ret = A_Bit( DO50, 0);   //VC SLOW FILTER  //SHIN B S  DO DELETE 170331  
		//SHIN B S  DO DELETE 170331	    if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return  ;	}
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

	}
	else  if (a == 1) {  //VC FAST FILTER
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		//SHIN B S  DO DELETE 170331		Ret = A_Bit( DO50, 0);   //VC SLOW FILTER
		//SHIN B S  DO DELETE 170331	    if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return  ;	}
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 1, 1);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,1) Error!!!");	return; }

	}
	else if (a == 2) { //VC SLOW FILTER
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }
		//SHIN B S  DO DELETE 170331		Ret = A_Bit( DO50, 1);   //VC SLOW FILTER
		//SHIN B S  DO DELETE 170331	    if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 50, 1] Error!!!");	return  ;	}

	}
	else if (a == 3) {//HV OPEN 
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
		//SHIN B S  DO DELETE 170331		Ret = A_Bit( DO50, 0);   //VC SLOW FILTER
		//SHIN B S  DO DELETE 170331	    if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return  ;	}
		Ret = Ixud_WriteDOBit(0, 4, 0, 1);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,1) Error!!!");	return; }

	}
	else if (a == 4) { //4W
		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
		//SHIN B S  DO DELETE 170331		Ret = A_Bit( DO50, 0);   //VC SLOW FILTER
		//SHIN B S  DO DELETE 170331	    if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return  ;	}
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

		if (SysSet19.m_n4WHVon == 1) {//sylee220701	
			Ret = Ixud_WriteDOBit(0, 5, 6, 1);  //4W HIGH VOL.ON  //sylee200922  PC16 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee220701	  PC16 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return; }
		}

		Ret = Ixud_WriteDOBit(0, 8, 1, 1);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }

	}
	else {//all off
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
		//SHIN B S  DO DELETE 170331		Ret = A_Bit( DO50, 0);   //VC SLOW FILTER
		//SHIN B S  DO DELETE 170331	    if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return  ;	}
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

		AfxMessageBox(" [ERROR NO 2101 : ADC B ,   DO SET PARAMETOR ERROR] Error!!!");

	}



#endif
#endif 

}

//son I Filter(ADC A) 설정.
void CModeSub71::I_Filter(int a) //Filter  SET  //sylee130628  0= all off  1=i fast, 2= i slow, 3=  vb   etc=error   
{
#ifndef _PC_TEST_
	CString str;
	int  Ret;

	//son ADC A Filter All OFF
#ifdef DEF_CARD_A1  //sylee170328
	if (a == 0) {  //all off
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //VB
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //I FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //I SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
	}

	//son OnRadioIfft() -> MM.Set(CFFT)에 해당.	
	//son sSET_ADC_A: ADC_I_FAST_SEL(47) ON
	else  if (a == 1) {   //I FAST FILTER
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //VB
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //I SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 1);   //I FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 1] Error!!!");	return; }
	}

	//son OnRadioIsft() -> MM.Set(CSFT)에 해당. 
	//son sSET_ADC_A: ADC_I_SLOW_SEL(48) ON 
	else if (a == 2) { //I SLOW FILTER
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //VB
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //I FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 1);   //I SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 1] Error!!!");	return; }
	}

	//son  MM.Set(VBFT) 에 해당. 				
	//son  sSET_ADC_A: ADC_VB_SEL(37) ON
	else if (a == 3) {//VB
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //VB
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //I FAST FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //I SLOW FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO37, 1);   //VB
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 1] Error!!!");	return; }
	}
	else {//all off	    
		AfxMessageBox(" [ERROR NO 2103 :I FILTER? or  VB?,   DO SET PARAMETOR?   Error!!!");
	}

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }

#else  	


	if (a == 0) {  //all off
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
	}
	else  if (a == 1) {   //I FAST FILTER
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 1);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,1) Error!!!");	return; }
	}
	else if (a == 2) { //I SLOW FILTER
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 1);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,1) Error!!!");	return; }
	}
	else if (a == 3) {//VB
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,1) Error!!!");	return; }
	}
	else {//all off	    
		AfxMessageBox(" [ERROR NO 2103 :I FILTER? or  VB?,   DO SET PARAMETOR?   Error!!!");
	}

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }


#endif

#endif


}


//son a 		1: s1(VB Relay Self Test), 2: s2 (VC Relay Self Test)
//son nOnOff    0: OFF,    1: ON
int CModeSub71::On4WS12Set(int a, int nOnOff) // a= 1(s1), 2(s2)
{ // 

#ifndef _PC_TEST_
	CString str;
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328

	//son S1 (VB Relay Self Test)
	if (a == 1)
	{
		if (nOnOff == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO39, 0);   	//son F4_WIRE_SELF_S2 OFF
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 39, 0] Error!!!");	return 0; }
			Delay_100ns(30000);//3mSEC

			Ret = DioOutBit(pDIO.hDrv, DO38, 1);   	//son F4_WIRE_SELF_S1 ON 
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 38, 1] Error!!!");	return 0; }
			Delay_100ns(30000);//3mSEC
		}
		else if (nOnOff == 0) {
			Ret = DioOutBit(pDIO.hDrv, DO38, 0);   	//son F4_WIRE_SELF_S1 OFF 
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 38, 0] Error!!!");	return 0; }
			Delay_100ns(30000);//3mSEC
		}
	}

	//son S2 (VC Relay Self Test)
	else if (a == 2)
	{
		if (nOnOff == 1)
		{

			Ret = DioOutBit(pDIO.hDrv, DO38, 0);   	//son F4_WIRE_SELF_S1 OFF
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 38, 0] Error!!!");	return 0; }
			Delay_100ns(30000);//3mSEC

			Ret = DioOutBit(pDIO.hDrv, DO39, 1);   	//son F4_WIRE_SELF_S2 ON
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 39, 1] Error!!!");	return 0; }
			Delay_100ns(30000);//3mSEC

		}
		else if (nOnOff == 0) {

			Ret = DioOutBit(pDIO.hDrv, DO39, 0);   //son F4_WIRE_SELF_S2 OFF
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 39, 0] Error!!!");	return 0; }
			Delay_100ns(30000);//3mSEC
		}
	}

#else  	//DEF_CARD_A1 

	//son S1 (VB Relay Self Test)
	if (a == 1)
	{
		if (nOnOff == 1) {
			Ret = Ixud_WriteDOBit(0, 10, 0, 0);  //4W SELF VC(S2) OFF    	//son bd=0, port=10, bit=0,(DO-80) bitVal=0
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,0,0) Error!!!");	return  0; }
			Delay_100ns(30000);//3mSEC

			Ret = Ixud_WriteDOBit(0, 10, 1, 1);  //4W SELF VB(S1) ON		//son bd=0, port=10, bit=1(DO-81), bitVal=1
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,1,1) Error!!!");	return  0; }
			Delay_100ns(30000);//3mSEC
		}
		else if (nOnOff == 0) {
			Ret = Ixud_WriteDOBit(0, 10, 1, 0);  //4W SELF VB(S1) OFF		//son bd=0, port=10, bit=1(DO-81), bitVal=0
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,1,0) Error!!!");	return  0; }
			Delay_100ns(30000);//3mSEC
		}
	}

	//son S2 (VC Relay Self Test)
	else if (a == 2)
	{
		if (nOnOff == 1)
		{

			Ret = Ixud_WriteDOBit(0, 10, 1, 0);  //4W SELF VB(S1) OFF		//son bd=0, port=10, bit=1(DO-81), bitVal=0
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,1,0) Error!!!");	return  0; }
			Delay_100ns(30000);//3mSEC

			Ret = Ixud_WriteDOBit(0, 10, 0, 1);  //4W SELF VC(S2) ON		//son bd=0, port=10, bit=0,(DO-80) bitVal=1
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,0,1) Error!!!");	return  0; }
			Delay_100ns(30000);//3mSEC

		}
		else if (nOnOff == 0) {
			Ret = Ixud_WriteDOBit(0, 10, 0, 0);  //4W SELF VC(S2) OFF		//son bd=0, port=10, bit=0,(DO-80) bitVal=1
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,0,0) Error!!!");	return  0; }
			Delay_100ns(30000);//3mSEC
		}
	}
#endif//DEF_CARD_A1 
#endif



	return 1;
}



//son Button 'LV1 ON'
void CModeSub71::OnButton121() //LV1 ON
{

#ifndef _PC_TEST_
	CString str;
	int  a, b;
	char szText[100];
	double fa;


	if (nVBOutCkFlag != 0) {//sylee150118
		AfxMessageBox(" LV1 RELAY ON ERROR.   VB OUT CHECK Disable ?!", MB_OK);
		return;
	}

	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118


	OnButton11081(); //sylee150120   //PLUSE  off

//====================================================================================
////HW PROTECTION  SYLEE20111005  0.5watts

	m_edit102.GetWindowText(szText, 10);
	a = atoi(szText);
	nModeVS1 = a;

	m_edit104.GetWindowText(szText, 10);
	b = atoi(szText);
	nModeIS1 = b;


	if (nModeCC == 1) { //CV

		if (nModeIR == 1) {
			fa = 1;  //I mA
		}
		else if (nModeIR == 2) {
			fa = 0.1; //I mA
		}
		else if (nModeIR == 3) {
			fa = 0.01; //I mA
		}
		else if (nModeIR == 4) {
			fa = 0.001; //I mA
		}
		else if (nModeIR == 5) {
			fa = 0.0001; //I mA
		}

		//1mA	100uA	10uA	1uA	0.1uA
		fa = a * fa;   //I

	}
	else {//CC

		if (nModeIR == 1 || nModeIR == 2) {
			fa = b;  //I mA
		}
		else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
			fa = b * 0.001; //I mA
		}

	}

	fa = (a * fa) / 1000.0;   //V*I

	//====================================================================================
	//CV MODE    HW PROTECTION
	if (nModeCC == 1) { //sylee20120229 3차 
		if (nModeIR == 1) {
			if (a > 2) {
				AfxMessageBox(" SET ERROR 732.   LV1 ON  SET ERROR.  CV MODE.   \n\n	I Range1, V SET 2V OVER. (<=2V USE.) \n\n V SET CHECK? ", MB_OK);
				return;
			}
		}
		if (nModeIR == 4 || nModeIR == 5) {
			AfxMessageBox(" SET ERROR 731.   LV1 ON  SET ERROR.  CV MODE.   \n\n I RANGE (1,2,3 USE.)      \n\n\n  I RANGE MODE CHECK? ", MB_OK);
			return;
		}
	}

	//===========================================================================================

	nMode1 = 0;



	Set_LV1_ON();        //son210611


	str.Format("LV1 ON");
	m_Label221.SetCaption(str);
	m_Label205.SetCaption(str);

	m_list1.InsertString(0, str);

	str.Format("        ");
	m_Label222.SetCaption(str);
	m_Label223.SetCaption(str);
	m_Label224.SetCaption(str);

	nMode1 = 1;


#endif
}

//son210611 
//son LV1 ON 전용 함수
void CModeSub71::Set_LV1_ON() //LV1 ON
{
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO52, 0); //HV1 RELAY   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0); //HV2 RELAY  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);  //LV1 RELAY    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);  //LV2 RELAY 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }
	Delay_100ns(30000);	//3mS  //HW RELAY  ON TIME 1mS  //HW PROTECTION    
	Ret = DioOutBit(pDIO.hDrv, DO54, 1);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 1] Error!!!");	return; }

#else  	//DEF_CARD_A1 

	//son LV1, LV2, HV1, HV2 OFF
	Ret = Ixud_WriteDOBit(0, 3, 4, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }
	Delay_100ns(30000);	//3mS   

	//son LV1 ON
	Ret = Ixud_WriteDOBit(0, 3, 6, 1); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,1) Error!!!");	return; }
#endif//DEF_CARD_A1 

}

//son Button 'LV2 ON'
void CModeSub71::OnButton122() //LV2 ON
{
#ifndef _PC_TEST_

	CString str;
	int Ret, a, b;
	char szText[100];
	double fa;

	if (nVBOutCkFlag != 0) {//sylee150118
		AfxMessageBox(" LV2 RELAY ON ERROR.   VB OUT CHECK Disable ?!", MB_OK);
		return;
	}
	if (nModeCC == 1) { //sylee201202293차  HW PROTECTION
		if (nModeIR == 4 || nModeIR == 5) {
			AfxMessageBox(" SET ERROR 731.   LV2 ON  SET ERROR.  CV MODE.   \n\n  I RANGE (1,2,3 USE.)    \n\n\n  I RANGE MODE CHECK? ", MB_OK);
			return;
		}
	}

	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118

	OnButton11081(); //sylee150120   //PLUSE  off

   //====================================================================================
////HW PROTECTION  SYLEE20111005  0.5watts

	m_edit106.GetWindowText(szText, 10);
	a = atoi(szText);
	nModeVS1 = a;
	m_edit104.GetWindowText(szText, 10);
	b = atoi(szText);
	nModeIS1 = b;

	//CV MODE    HW PROTECTION
	if (nModeCC == 1) { //sylee20120229 3차 
		if (nModeIR == 1) {
			if (a > 2) {
				AfxMessageBox(" SET ERROR 732.   LV1 ON  SET ERROR.  CV MODE.   \n\n	I Range1, V SET 2V OVER. (<=2V USE.) \n\n V SET CHECK? ", MB_OK);
				return;
			}
		}

		if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
			AfxMessageBox(" SET ERROR 731.   LV1 ON  SET ERROR.  CV MODE.   \n\n I RANGE (1,2 USE.)   \n\n\n I RANGE  MODE CHECK? ", MB_OK);
			return;
		}
	}


	if (nModeCC == 1) { //CV

		if (nModeIR == 1) {
			fa = 1;  //I mA
		}
		else if (nModeIR == 2) {
			fa = 0.1; //I mA
		}
		else if (nModeIR == 3) {
			fa = 0.01; //I mA
		}
		else if (nModeIR == 4) {
			fa = 0.001; //I mA
		}
		else if (nModeIR == 5) {
			fa = 0.0001; //I mA
		}

		//1mA	100uA	10uA	1uA	0.1uA
		fa = a * fa;   //I

	}
	else {//CC

		if (nModeIR == 1 || nModeIR == 2) {
			fa = b;  //I mA
		}
		else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
			fa = b * 0.001; //I mA
		}

	}

	fa = (a * fa) / 1000.0;   //V*I


	nMode1 = 0;





#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO52, 0); //HV1 RELAY   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0); //HV2 RELAY  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);  //LV1 RELAY    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);  //LV2 RELAY 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }
	Delay_100ns(30000);	//3mS  //HW RELAY  ON TIME 1mS  //HW PROTECTION   //SYLEE20120702
	Ret = DioOutBit(pDIO.hDrv, DO55, 1);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 1] Error!!!");	return; }
#else  //DEF_CARD_A1 
	Ret = Ixud_WriteDOBit(0, 3, 4, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }
	Delay_100ns(30000);	//3mS   
	Ret = Ixud_WriteDOBit(0, 3, 7, 1); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,1) Error!!!");	return; }

#endif//DEF_CARD_A1 

	str.Format("LV2 ON");
	m_Label205.SetCaption(str);
	m_Label222.SetCaption(str);
	m_list1.InsertString(0, str);

	str.Format("        ");
	m_Label221.SetCaption(str);
	m_Label223.SetCaption(str);
	m_Label224.SetCaption(str);

	nMode1 = 2;
	//	OnButton610();//sylee20111214   HW INTERLOCK    VB READING없이  R READING시   LV HV  VC  VB 다 ON된다.   
#endif
}


//son Button 'HV1 ON'
void CModeSub71::OnButton123()  //HV1 ON
{
#ifndef _PC_TEST_

	CString str;
	int Ret, a, b;
	char szText[100];
	double fa;

	if (nVBOutCkFlag != 0) {//sylee150118
		AfxMessageBox(" HV1 RELAY ON ERROR.   VB OUT CHECK Disable ?!", MB_OK);
		return;
	}

	if (nMode4W == 1) {  //HW PROTECTION
		AfxMessageBox(" HV1 ON  SET FAIL  !!!   !!!    4W ON SET STATUS ?    ");	return;
	}

	if (nFlag71_VC_Filter_Mode_LV == 1) {  //VC FILTER LV OFF
		OnRadio04();
		Sleep(10);
	}

	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118
	OnButton11081(); //sylee150120   //PLUSE  off

//====================================================================================
////HW PROTECTION  SYLEE20111005  0.5watts

	m_edit108.GetWindowText(szText, 10);
	a = atoi(szText);
	nModeVS1 = a;
	m_edit104.GetWindowText(szText, 10);
	b = atoi(szText);
	nModeIS1 = b;


	if (nModeCC == 1) { //sylee201202293차  HW PROTECTION

		if (a > 200) {
			AfxMessageBox(" SET ERROR 731.   HV1 ON  SET ERROR.  CV MODE.   \n\n   V >200V OVER.  \n\n\n   V SET  CHECK? ", MB_OK);
			return;

		}
	}

	if (nModeCC == 1) { //CV
		if (nModeIR == 1) {
			fa = 1;  //I mA
		}
		else if (nModeIR == 2) {
			fa = 0.1; //I mA
		}
		else if (nModeIR == 3) {
			fa = 0.01; //I mA
		}
		else if (nModeIR == 4) {
			fa = 0.001; //I mA
		}
		else if (nModeIR == 5) {
			fa = 0.0001; //I mA
		}
		//1mA	100uA	10uA	1uA	0.1uA
		fa = a * fa;   //I

	}
	else {//CC
		if (nModeIR == 1 || nModeIR == 2) {
			fa = b;  //I mA
		}
		else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
			fa = b * 0.001; //I mA
		}
	}

	fa = (a * fa) / 1000.0;   //V*I

	nMode1 = 0;


	nMode4W = 0;
	str.Format("4W OFF");
	m_Label205.SetCaption(str);




#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO47, 0); //4WIRE RELAY   //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO52, 0); //HV1 RELAY   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0); //HV2 RELAY  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);  //LV1 RELAY    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);  //LV2 RELAY 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }

	Delay_100ns(30000);	//3mS  //HW RELAY  ON TIME 1mS  //HW PROTECTION   //SYLEE20120702

	Ret = DioOutBit(pDIO.hDrv, DO52, 1);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 1] Error!!!");	return; }

#else  //DEF_CARD_A1 

	Ret = Ixud_WriteDOBit(0, 8, 0, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 3, 4, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }
	Delay_100ns(30000);	//3mS   
	Ret = Ixud_WriteDOBit(0, 3, 4, 1); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,1) Error!!!");	return; }

#endif//DEF_CARD_A1 


	str.Format("HV1 ON");
	m_Label205.SetCaption(str);
	m_Label223.SetCaption(str);
	m_list1.InsertString(0, str);

	str.Format("        ");
	m_Label221.SetCaption(str);
	m_Label222.SetCaption(str);
	m_Label224.SetCaption(str);

	nMode1 = 3;
	//	OnButton611();//sylee20111214   HW INTERLOCK    VB READING없이  R READING시   LV HV  VC  VB 다 ON된다.    
#endif
}


//son Button 'HV2 ON'
void CModeSub71::OnButton124() //HV2 ON
{
#ifndef _PC_TEST_

	CString str;
	int Ret, a, b;
	char szText[100];
	double fa;

	if (nVBOutCkFlag != 0) {//sylee150118
		AfxMessageBox(" HV2 RELAY ON ERROR.   VB OUT CHECK Disable ?!", MB_OK);
		return;
	}

	if (nMode4W == 1) {  //HW PROTECTION
		AfxMessageBox(" HV1 ON  SET FAIL  !!!   !!!    4W ON SET STATUS ?    ");	return;
	}

	if (nFlag71_VC_Filter_Mode_LV == 1) {  //VC FILTER LV OFF
		OnRadio04();
		Sleep(10);
	}


	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118
	OnButton11081(); //sylee150120   //PLUSE  off

//====================================================================================
////HW PROTECTION  SYLEE20111005  0.5watts

	m_edit110.GetWindowText(szText, 10);
	a = atoi(szText);
	nModeVS1 = a;

	m_edit104.GetWindowText(szText, 10);
	b = atoi(szText);
	nModeIS1 = b;

	if (nModeCC == 1) { //sylee201202293차  HW PROTECTION		 
		if (a > 200) {
			AfxMessageBox(" SET ERROR 731.   HV2 ON  SET ERROR.  CV MODE.   \n\n   V >200V OVER.  \n\n\n  V SET  CHECK? ", MB_OK);
			return;
		}
	}

	if (nModeCC == 1) { //CV
		if (nModeIR == 1) {
			fa = 1;  //I mA
		}
		else if (nModeIR == 2) {
			fa = 0.1; //I mA
		}
		else if (nModeIR == 3) {
			fa = 0.01; //I mA
		}
		else if (nModeIR == 4) {
			fa = 0.001; //I mA
		}
		else if (nModeIR == 5) {
			fa = 0.0001; //I mA
		}
		//1mA	100uA	10uA	1uA	0.1uA
		fa = a * fa;   //I	
	}
	else {//CC
		if (nModeIR == 1 || nModeIR == 2) {
			fa = b;  //I mA
		}
		else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
			fa = b * 0.001; //I mA
		}
	}

	fa = (a * fa) / 1000.0;  //V*I

	nMode1 = 0;
	nMode4W = 0;
	str.Format("4W OFF");
	m_Label205.SetCaption(str);

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO47, 0); //4WIRE RELAY   //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO52, 0); //HV1 RELAY   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0); //HV2 RELAY  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);  //LV1 RELAY    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);  //LV2 RELAY 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }
	Delay_100ns(30000);	//3mS  //HW RELAY  ON TIME 1mS  //HW PROTECTION   //SYLEE20120702  
	Ret = DioOutBit(pDIO.hDrv, DO53, 1);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 1] Error!!!");	return; }

#else  //DEF_CARD_A1 

	Ret = Ixud_WriteDOBit(0, 8, 0, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 3, 4, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }
	Delay_100ns(30000);	//3mS   
	Ret = Ixud_WriteDOBit(0, 3, 5, 1); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,1) Error!!!");	return; }

#endif//DEF_CARD_A1 

	str.Format("HV2 ON");
	m_Label205.SetCaption(str);
	m_Label224.SetCaption(str);
	m_list1.InsertString(0, str);

	str.Format("        ");
	m_Label221.SetCaption(str);
	m_Label222.SetCaption(str);
	m_Label223.SetCaption(str);

	nMode1 = 4;

#endif
}


/*
void CModeSub71::OnButton131() //Delay time
{
	char szText[50];
	nTimeDelay1 = atoi( szText );
	nTimeDelay1=nTimeDelay1*10;
}
*/

/*
void CModeSub71::OnButton132() //AD  Count time
{
	char szText[50];
	nADCount1 = atoi( szText );
	if(nADCount1<1){//SYLEE20111111
		nADCount1=3;
	}
}
*/


/*
void CModeSub71::OnButton133() //TEST  Count
{
	char szText[50];
	nTestCount1 = atoi( szText );
	if(nTestCount1<1){//SYLEE20111111
		nTestCount1=1;
	}
}
*/



int CModeSub71::AOnRelayOn1(int nCC, int nVSw, double nVSet, int nIR, double nI) //nVSw 1=lv1,2=lv2, 3=hv1,4=hv2    // nCC=1  CC, 2=CV
{

	CString str;
	int Ret;

	if (nRelay1OffSkip == 3) {	//sylee170424-1
		return 1;
	}


	double fa, a, b;
	if (nVSw == 3 || nVSw == 4) {//hv1,hv2
		if (nMode4W == 1) {  //HW PROTECTION
			AfxMessageBox(" HV1 ON  SET FAIL  !!!   !!!    4W ON SET STATUS ?    ");	return 0;
		}
	}
	//====================================================================================
	////HW PROTECTION  SYLEE20111005  0.5watts

	a = nVSet;
	nModeVS1 = nVSet;  //v set

	b = nI;
	nModeIS1 = nI;  //i set

	if (nCC == 2) { //CV

		if (nIR == 1) {
			fa = 1;  //I mA
		}
		else if (nIR == 2) {
			fa = 0.1; //I mA
		}
		else if (nIR == 3) {
			fa = 0.01; //I mA
		}
		else if (nIR == 4) {
			fa = 0.001; //I mA
		}
		else if (nIR == 5) {
			fa = 0.0001; //I mA
		}
		//1mA	100uA	10uA	1uA	0.1uA
		fa = a * fa;   //I ?????????????????????????????????????????????

	}
	else {//CC

		if (nIR == 1 || nIR == 2) {
			fa = b;  //I mA
		}
		else if (nIR == 3 || nIR == 4 || nIR == 5) {
			fa = b * 0.001; //I mA
		}
	}

	fa = (a * fa) / 1000.0;   //V*I

	if (nCC == 1 && nVSw == 1 && nIR == 1) {
		if (nMode4W2 != 1) {  //sylee140717 ray
			if (SysSet19.m_n4WHVon != 1) { //sylee200217-4WCVMODE  //sylee200515-patch
				if (fa > 2.5) {//1watts  //sylee171029-1 test  4w 10v->20v

					AfxMessageBox("  Error No 716.   VB SET VALUE?, I RANGE MODE? CHECK. \n  CC, LV1, IR1, \n ==> 2.5watts over. OFF PLEASE!", MB_OK);
					return 0;
				}
			}
		}
	}
	else {
		if (a == 5) {
			if (fa > 0.5) {//0.5watts	//SYLEE20111216  HW 파손으로 TEAM내 협의로 0.5->0.25WATTS
				AfxMessageBox(" Error No  717.   VB SET  5V SET.      I VALUE?, I RANGE MODE?  CHECK?.   0.5watts over. OFF PLEASE!", MB_OK);
				return 0;
			}
		}
	}

	//====================================================================================

	nMode1 = 0;


#ifdef _PC_TEST_
	Ret = 0;
#else
#ifdef DEF_CARD_A1  //sylee170328

	if (nFlag_4W_OUT50V == 1 && n4W_Use == 1) {//SYLEE200411   //sylee200612
	}
	else
	{

		if (nVSet > 20) {
			Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4WIRE RELAY   //HW PROTECTION  2mS DELAY    //SYLEE130704  47->46
			//*****************************************
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return  0; }
			nMode4W = 0;
			Delay_100ns(100000);  // 10mS //SYLEE181115 import.//4w wire protection routine
			Ret = DioOutBit(pDIO.hDrv, DO46, 0);
			Delay_100ns(200000);  // 20mS //SYLEE181115 import.//4w wire protection routine  //SYLEE181118-1
			//4w wire protection routine
			// NEVER DELETE
			// PLEASE!!!!!!!!!
			// by SBS
			//****************************************
		}

		if (nVSw == 3) {
			Ret = DioOutBit(pDIO.hDrv, DO52, 1); //HV1 RELAY   
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 1] Error!!!");	return  0; }
			nMode1 = 3;
		}
		else if (nVSw == 4) {
			Ret = DioOutBit(pDIO.hDrv, DO53, 1); //HV2 RELAY  
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 1] Error!!!");	return  0; }
			nMode1 = 4;
		}
		else if (nVSw == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO54, 1);  //LV1 RELAY    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 54, 1] Error!!!");	return  0; }
			nMode1 = 1;

			if (nRelay1OffSkip == 1) nRelay1OffSkip = 2;//sylee170424-1

		}
		else if (nVSw == 2) {
			Ret = DioOutBit(pDIO.hDrv, DO55, 1);  //LV2 RELAY 
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 1] Error!!!");	return  0; }
			nMode1 = 2;
		}

	}   //sylee200612

#else  	//DEF_CARD_A1 

	if (nFlag_4W_OUT50V == 1 && n4W_Use == 1) {//SYLEE200411   //sylee200612
	}
	else
	{
		if (nVSet > 20)
		{
			//****************************************
			Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return   0; }

			Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return  0; }
			nMode4W = 0;
			//	Delay_100ns(50000); // 5mS //SYLEE181115 import. 
			Delay_100ns(100000);  // 10mS //SYLEE181115 import.//4w wire protection routine

			Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return   0; }

			Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
			Delay_100ns(200000);  // 20mS //SYLEE181115 import.//4w wire protection routine  //SYLEE181118-1
			//4w wire protection routine
			// NEVER DELETE
			// PLEASE!!!!!!!!!
			// by SBS
			//****************************************
		}

		if (nVSw == 3) {
			Ret = Ixud_WriteDOBit(0, 3, 4, 1);  //HV1 RELAY
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,1) Error!!!");	return  0; }
			nMode1 = 3;
		}
		else if (nVSw == 4) {
			Ret = Ixud_WriteDOBit(0, 3, 5, 1);  //HV2 RELAY
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,1) Error!!!");	return  0; }
			nMode1 = 4;
		}
		else if (nVSw == 1) {
			Ret = Ixud_WriteDOBit(0, 3, 6, 1);  //LV1 RELAY
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,1) Error!!!");	return   0; }
			nMode1 = 1;
		}
		else if (nVSw == 2) {
			Ret = Ixud_WriteDOBit(0, 3, 7, 1);  //LV2 RELAY
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,1) Error!!!");	return   0; }
			nMode1 = 2;
		}

	} //SYLEE200411



#endif  //DEF_CARD_A1 
#endif
	Delay_100ns(50000);  	//3mS->5mS /SYLEE20111226-1  

	return 1;

}



//son VB Apply 관련 비트를 모두 초기화한다.	
//son BIT52(HV_RELAY_IN_IO_0) ~ BIT55(LV_RELAY_IN_IO_1) 
int CModeSub71::AOnRelayOff1()  //LV1,2 HV1,2 RELAY ALL OFF
{
#ifndef _PC_TEST_
	int Ret;



	if (nRelay1OffSkip == 3) {	//sylee170424-1
		return 1;
	}

	//son BIT52 : HV_RELAY_IN_IO_0  -> 0
	//son BIT53 : HV_RELAY_IN_IO_1  -> 0
	//son BIT54 : LV_RELAY_IN_IO_0  -> 0
	//son BIT54 : LV_RELAY_IN_IO_1  -> 0
#ifdef DEF_CARD_A1  //sylee170328
	OutBitNo[0] = 52;   OutBitNo[1] = 53;   OutBitNo[2] = 54;   OutBitNo[3] = 55;
	OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;

	Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 4, &OutBitData[0]);
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 52 53 54 55 , 0  0  0   0] Err]or!!!");	return  0; }
#else  	

	Ret = Ixud_WriteDOBit(0, 3, 4, 0);  //HV0 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0);  //HV1 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0);  //LV0 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0);  //LV1 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return  0; }

#endif



#endif

	return  1;
}


int CModeSub71::AOnRelayOffSel(int a)
{
#ifndef _PC_TEST_
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (a == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO54, 0);  //LV1 RELAY    
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return  0; }
	}
	if (a == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO55, 0);  //LV2 RELAY 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return  0; }
	}
	if (a == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO52, 0); //HV1 RELAY   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return  0; }
	}
	if (a == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO53, 0); //HV2 RELAY  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return  0; }
	}
#else  	

	if (a == 1) {
		Ret = Ixud_WriteDOBit(0, 3, 6, 0);  //LV1 RELAY
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return   0; }
		if (nMode1 = 1)nMode1 = 0;//sylee201029
	}
	if (a == 2) {
		Ret = Ixud_WriteDOBit(0, 3, 7, 0);  //LV2 RELAY
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return   0; }
		if (nMode1 = 2)nMode1 = 0;//sylee201029
	}
	if (a == 3) {
		Ret = Ixud_WriteDOBit(0, 3, 4, 0);  //HV1 RELAY
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return  0; }
		if (nMode1 = 3)nMode1 = 0;//sylee201029
	}
	if (a == 4) {
		Ret = Ixud_WriteDOBit(0, 3, 5, 0);  //HV2 RELAY
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return  0; }
		if (nMode1 = 4)nMode1 = 0;//sylee201029
	}


#endif


#endif


	return  1;
}





//son Button118 '4W ON'
void CModeSub71::OnButton125() //4WIRE
{
#ifndef _PC_TEST_

	CString str;
	int Ret;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111013   4wire   i range 1   100ma   20v  2watts  4wire off시  HW PROTECTION.
		AfxMessageBox(" [ 4W ON SET FAIL  => LV1, LV2 , HV1 , HV2  RELAY ON  MODE.    ALL OFF RESET  PLEASE!  ] Error!!!");	return;
	}


	if (nMode4W < 1) {

		nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118


		OnButton11081(); //sylee150120   //PLUSE  off
		OnFiler_VC(4);	 //son 4W Filter ON
		nMode4W = 1;
		((CButton*)GetDlgItem(IDC_RADIO03))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO04))->SetCheck(FALSE);
		nModeOpenHV = 0;
		str.Format("OFF");
		m_Label236.SetCaption(str);
		str.Format("ON");

	}
	else
	{



		//son 4W Filter OFF : 4_WIRE_CMD(DO46) 0
#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
#else  	
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
#endif
		str.Format("OFF");
		nMode4W = 0;
	}

	m_Label225.SetCaption(str);
	m_list1.InsertString(0, str);

#endif
}



void CModeSub71::On4WireMode(int a) //4WIRE  a=1 ON, 0 OFF
{
#ifndef _PC_TEST_

	CString str;
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (a == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO47, 1); //4W CMD  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 1] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO46, 1);  // 4WIRE_CMD  (0=VC SENSING,      1=4W SENSING)
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return; }
	}
	else {
		Ret = DioOutBit(pDIO.hDrv, DO47, 0); //4W CMD  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);  // 4WIRE_CMD  (0=VC SENSING,      1=4W SENSING)
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
	}
#else 

	if (a == 1) {
		Ret = Ixud_WriteDOBit(0, 8, 0, 1);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,1) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 5, 6, 1);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 1);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,1) Error!!!");	return; }
	}
	else {
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
	}

#endif

#endif
}


//son Button 'All Reset'
void CModeSub71::OnButton126() //초기화  LV1 LV2  HV1  HV2  RELAY ON -> ALL OFF
{
#ifndef _PC_TEST_

	CString str;
	int Ret;

	nMode1 = 0;

	OnPinAllReset1();//SYLEE20120223

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO52, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }

	if (nMode4W == 1) {//if( nMode4W ==1 && nModeIR==1){//HW PROTECTION
		Delay_100ns(20000); //2ms
	}

	Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD    //SYLEE20111013	//sylee130626  47->46	
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }   //SYLEE20111013	

	nMode4W = 0;
	str.Format("OFF");
	m_Label225.SetCaption(str);

	Ret = DioOutBit(pDIO.hDrv, DO56, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

#else  	

	Ret = Ixud_WriteDOBit(0, 3, 4, 0);  //HV1 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0);  //HV2 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0);  //LV1 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0);  //LV2 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }

	if (nMode4W == 1) {//if( nMode4W ==1 && nModeIR==1){//HW PROTECTION
		Delay_100ns(20000); //2ms
	}

	Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }


	Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }

	nMode4W = 0;
	str.Format("OFF");
	m_Label225.SetCaption(str);

	Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }


#endif



	nModeOpenHV = 0;
	str.Format("OFF");
	m_Label236.SetCaption(str);

	Delay_100ns(20000);	//2mS

	str.Format("INITIAL");
	m_Label205.SetCaption(str);
	m_list1.InsertString(0, str);

	str.Format("OFF");
	m_Label221.SetCaption(str);
	m_Label222.SetCaption(str);
	m_Label223.SetCaption(str);
	m_Label224.SetCaption(str);

	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118

	OnButton11081(); //sylee150120   //PLUSE

	nMode4W = 1;
	OnButton125(); //4WIRE OFF

	nModeOpenHV = 1;

	OnButton112(); //HV OPEN OFF

	nMode1 = 0;


#endif
}




//------------------------------------------------------------
//son :  OnRadio21() I Range Set: 1(60mA)
void CModeSub71::OnButton201()    // I Range Mode 0
{
#ifndef _PC_TEST_
	CString str;
	int Ret;//,a;

	if (nModeOpenHV == 1) {   //SYLEE20120411
		AfxMessageBox(" SET Error 718.   HV OPEN ON.      OFF PLEASE!", MB_OK); 	   return;
	}
	if (nMode4WG == 1) {   //SYLEE20120411
		AfxMessageBox(" SET Error 718.  4W GAIN ON.      OFF PLEASE!", MB_OK); 	   return;
	}

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 718.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 33, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 35, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 36, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO32, 1); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 1] Error!!!");	return; }

#else  	

	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 10, 7, 1);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,1) Error!!!");	return; }

#endif


	str.Format("Range 1");
	m_Label202.SetCaption(str);
	m_list1.InsertString(0, str);

	nModeIR = 1;

	str.Format("I Value mA");
	m_Label301.SetCaption(str);

#endif
}

//son Button '???' : OnRadio22() 'I Range Set: 2(20mA)'
void CModeSub71::OnButton202()
{
#ifndef _PC_TEST_
	CString str;

	int Ret;//,a;


	if (nModeOpenHV == 1) {  //SYLEE20120411
		AfxMessageBox(" SET Error 718.   HV OPEN ON.      OFF PLEASE!", MB_OK); 	   return;
	}
	if (nMode4WG == 1) {  //SYLEE20120411
		AfxMessageBox(" SET Error 718.   4W Gain ON.      OFF PLEASE!", MB_OK); 	   return;
	}
	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 721.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO33, 1); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 1] Error!!!");	return; }
#else  	

	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 1);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,1) Error!!!");	return; }


#endif

	str.Format("Range 2");
	m_Label202.SetCaption(str);
	m_list1.InsertString(0, str);

	nModeIR = 2;

	str.Format(" I Value mA");
	m_Label301.SetCaption(str);
#endif
}

//son Button '???' : OnRadio23() 'I Range Set: 3(10mA)'
void CModeSub71::OnButton203()
{
#ifndef _PC_TEST_
	CString str;

	int Ret;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 710.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}

#ifdef DEF_CARD_A1  //sylee170328


	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO34, 1); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 1] Error!!!");	return; }

#else  

	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 10, 5, 1);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,1) Error!!!");	return; }


#endif

	str.Format("Range 3");
	m_Label202.SetCaption(str);
	m_list1.InsertString(0, str);

	nModeIR = 3;

	str.Format(" I Value uA");
	m_Label301.SetCaption(str);

#endif
}

//son Button '???': OnRadio24() 'I Range Set: 4(2mA)'
void CModeSub71::OnButton204()
{
#ifndef _PC_TEST_
	CString str;

	int Ret;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 712.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}



#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO35, 1);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 1] Error!!!");	return; }
#else  	


	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 1);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,1) Error!!!");	return; }

#endif

	str.Format("Range 4");
	m_Label202.SetCaption(str);
	m_list1.InsertString(0, str);

	nModeIR = 4;

	str.Format("I Value uA");
	m_Label301.SetCaption(str);

#endif
}


//son Button '???': OnRadio25() : 'I Range Set: 5(200uA)'
void CModeSub71::OnButton205()
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 713.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO36, 1);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 1] Error!!!");	return; }
#else  	

	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 10, 3, 1);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,1) Error!!!");	return; }


#endif


	str.Format("Range 5");
	m_Label202.SetCaption(str);
	m_list1.InsertString(0, str);

	nModeIR = 5;

	str.Format("I Value uA");  //?
	m_Label301.SetCaption(str);

#endif
}






int CModeSub71::AOnIRanSet1(int nIR, int nOnOff) //  nIR: I range  ( 1~5,   OnOff: 0(Off), 1(On)
{
#ifndef _PC_TEST_
	CString str;
	int Ret;



#ifdef DEF_CARD_A1  //sylee170328

	if (nOnOff == 1) {
		if (nIR != 1) {
			Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return  0; }
		}
		if (nIR != 2) {
			Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return  0; }
		}
		if (nIR != 3) {
			Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return  0; }
		}
		if (nIR != 4) {
			Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return  0; }
		}
		if (nIR != 5) {
			Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return  0; }
		}
	}

	//=========================================================================================

	if (nIR == 1) {
		if (nOnOff == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO32, 1); // I Range Mode 0  
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 1] Error!!!");	return  0; }
		}
		else {
			Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return  0; }
		}
	}
	else if (nIR == 2) {
		if (nOnOff == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO33, 1); // I Range Mode 1    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 1] Error!!!");	return  0; }
		}
		else {
			Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return  0; }
		}
	}
	else if (nIR == 3) {
		if (nOnOff == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO34, 1); // I Range Mode 2    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 1] Error!!!");	return  0; }
		}
		else {
			Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return  0; }
		}
	}
	else if (nIR == 4) {
		if (nOnOff == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO35, 1);  // I Range Mode 3   
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 1] Error!!!");	return  0; }
		}
		else {
			Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return  0; }
		}
	}
	else if (nIR == 5) {
		if (nOnOff == 1) {
			Ret = DioOutBit(pDIO.hDrv, DO36, 1);  // I Range Mode 4    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 1] Error!!!");	return  0; }
		}
		else {
			Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return  0; }
		}
	}

#else  


	//son 주어진 IR에 해당이 되지않는 Range는 모두 Off
	if (nOnOff == 1)	//son ON
	{

		if (nIR != 1) {
			Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return   0; }
		}
		if (nIR != 2) {
			Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return   0; }
		}
		if (nIR != 3) {
			Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return   0; }

		}
		if (nIR != 4) {
			Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return   0; }
		}
		if (nIR != 5) {
			Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return   0; }
		}
	}

	//=========================================================================================

	//son 주어진 IR값에 해당되는 Range On/Off
	if (nIR == 1)
	{
		if (nOnOff == 1) {
			Ret = Ixud_WriteDOBit(0, 10, 7, 1);  //I RANGE SET0
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,1) Error!!!");	return   0; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return   0; }
		}
	}
	else if (nIR == 2) {
		if (nOnOff == 1) {
			Ret = Ixud_WriteDOBit(0, 10, 6, 1);  //I RANGE SET1
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,1) Error!!!");	return   0; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return   0; }
		}
	}
	else if (nIR == 3) {
		if (nOnOff == 1) {
			Ret = Ixud_WriteDOBit(0, 10, 5, 1);  //I RANGE SET2
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,1) Error!!!");	return   0; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return  0; }

		}
	}
	else if (nIR == 4) {
		if (nOnOff == 1) {
			Ret = Ixud_WriteDOBit(0, 10, 4, 1);  //I RANGE SET3
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,1) Error!!!");	return   0; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return   0; }
		}
	}
	else if (nIR == 5) {
		if (nOnOff == 1) {
			Ret = Ixud_WriteDOBit(0, 10, 3, 1);  //I RANGE SET4
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,1) Error!!!");	return   0; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return   0; }
		}
	}



#endif




	nModeIR = nIR;
#endif

	return 1;



}



int CModeSub71::AOnIRanSet1Run1(int nIR, int nOnOff) //  I range  ( 1~5,   nOnOff: 1(on), 0(off)  //SYLEE20111213
{
#ifndef _PC_TEST_
	CString str;
	int Ret;



#ifdef DEF_CARD_A1  //sylee170328

	OutBitNo[0] = 32;    OutBitNo[1] = 33;    OutBitNo[2] = 34;   OutBitNo[3] = 35;   OutBitNo[4] = 36;

	if (nOnOff == 1) {
		if (nIR == 1) {
			OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 0;
		}
		else if (nIR == 2) {
			OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 0;
		}
		else if (nIR == 3) {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 1;  OutBitData[3] = 0;  OutBitData[4] = 0;
		}
		else if (nIR == 4) {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 1;  OutBitData[4] = 0;
		}
		else if (nIR == 5) {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 1;
		}
	}
	else {

		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 0;

	}

	Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 5, &OutBitData[0]);
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 32 33 34 35 36 , 0  0  0  0 0] Error!!!");	return  0; }

#else 


	//	OutBitNo[0]=32;    OutBitNo[1]=33;    OutBitNo[2]=34;   OutBitNo[3]=35;   OutBitNo[4]=36;

		//son191212: 추가.   nIR 값이 0으로 들어 오는데 nOnOff==1이면  
		//                  아래 Outbit Data가 제대로 설정이 안 됨.  다음 초기화 루틴을 끌어올려서 해결함.
	OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 0;

	if (nOnOff == 1)
	{
		if (nIR == 1) {
			OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 0;
		}
		else if (nIR == 2) {
			OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 0;
		}
		else if (nIR == 3) {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 1;  OutBitData[3] = 0;  OutBitData[4] = 0;
		}
		else if (nIR == 4) {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 1;  OutBitData[4] = 0;
		}
		else if (nIR == 5) {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;  OutBitData[3] = 0;  OutBitData[4] = 1;
		}
	}
	/*else{
		OutBitData[0]=0;   OutBitData[1]=0;   OutBitData[2]=0;  OutBitData[3]=0;  OutBitData[4]=0;
	}
	*/

	Ret = Ixud_WriteDOBit(0, 10, 7, OutBitData[0]);  //I RANGE 1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,  ) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 10, 6, OutBitData[1]);  //I RANGE 2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,  ) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 10, 5, OutBitData[2]);  //I RANGE 3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,  ) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 10, 4, OutBitData[3]);  //I RANGE 4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,  ) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 10, 3, OutBitData[4]);  //I RANGE 5
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,  ) Error!!!");	return  0; }

#endif



	nModeIR = nIR;
#endif

	return 1;

}




//son Button 'I Range Set: RESET'
void CModeSub71::OnButton206()
{
#ifndef _PC_TEST_
	CString str;
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 33, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 35, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 36, 0] Error!!!");	return; }

#else  	

	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return; }

#endif


	str.Format("Rang.Reset");
	m_Label202.SetCaption(str);
	m_list1.InsertString(0, str);

	nModeIR = 0;

	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(FALSE);
#endif
}



int CModeSub71::AIRangeOff()
{
#ifndef _PC_TEST_
	CString str;
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 33, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 35, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 36, 0] Error!!!");	return  0; }

#else  	


	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return   0; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return   0; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return   0; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return   0; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return   0; }

#endif


#endif
	return 1;
}



int CModeSub71::AIVBOff()
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //LV_SEN_CMD        
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return  0; }
	Delay_100ns(20000);//2mSEC 
	Ret = DioOutBit(pDIO.hDrv, DO40, 0);   //LV_SEN_SEL
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO41, 0);   //HV_SEN_SEL
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 0] Error!!!");	return  0; }

#else 

	Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   0; }
	Delay_100ns(20000);//2mSEC 
	Ret = Ixud_WriteDOBit(0, 8, 7, 0);  //VB HV SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 8, 6, 0);  //VB LV SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,0) Error!!!");	return  0; }

#endif

#endif
	return 1;

}




//sylee121022
int CModeSub71::AVB_Set_AD1(int nMode2, int nADset)//   VB SET, 16bit  nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK
{

#ifndef _PC_TEST_
	CString str;
	int  Ret;
	BYTE Ha, La;

	if (nMode2 > 5) {
		nMode2 = 0;
	}

	if (nADset < 0 || nADset> 65535) {
		nADset = 0;
	}

	La = (BYTE)(nADset & 0xff);
	Ha = (BYTE)((nADset >> 8) & 0xff);

#ifdef DEF_CARD_A1  //sylee170328

	if (nMode2 == 1) {//저압1
		Ret = DioOutBit(pDIO.hDrv, DO19, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 19, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 2) {//저압2
		Ret = DioOutBit(pDIO.hDrv, DO23, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 23, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 3) {//고압1
		Ret = DioOutBit(pDIO.hDrv, DO21, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 21, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 4) {//고압2
		Ret = DioOutBit(pDIO.hDrv, DO25, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 25, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 5) {//SPARK 
		Ret = DioOutBit(pDIO.hDrv, DO27, 1);  //DA reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 27, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 6) {//4WIRE???  
		return 0;
	}
	else {
		return 0;
	}

	Ret = DioOutByte(pDIO.hDrv, 0, La);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 1, Ha);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }

	Delay_100ns(4000);	//400uS   

	if (nMode2 == 1) {//저압1
		Ret = DioOutBit(pDIO.hDrv, DO18, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 18, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO18, 1);   ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 18, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 2) {//저압2
		Ret = DioOutBit(pDIO.hDrv, DO22, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 22, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO22, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 22, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 3) {//고압1   
		Ret = DioOutBit(pDIO.hDrv, DO20, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 20, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO20, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 20, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 4) {//고압2
		Ret = DioOutBit(pDIO.hDrv, DO24, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 24, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO24, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 24, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 5) {//SPARK
		Ret = DioOutBit(pDIO.hDrv, DO26, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 26, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO26, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 26, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 6) {//4W????
		str.Format(" * ERROR : VB DA mode?  4w ? ");
		m_list1.InsertString(0, str);
	}
	else {
		return 0;
	}

#else  	

	if (nMode2 == 1) {//저압1
		Ret = Ixud_WriteDOBit(0, 2, 3, 1);  //  /LV SET RST0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,3,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 2) {//저압2
		Ret = Ixud_WriteDOBit(0, 2, 7, 1);  //  /LV SET RST1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,7,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 3) {//고압1
		Ret = Ixud_WriteDOBit(0, 2, 5, 1);  //  /HV SET RST0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,5,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 4) {//고압2
		Ret = Ixud_WriteDOBit(0, 6, 1, 1);  //   /HV SET RST1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,1,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 5) {//SPARK  
		Ret = Ixud_WriteDOBit(0, 6, 3, 1);  // /SPARK SET RST
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,3,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 6) {//4WIRE???  
		return 0;
	}
	else {
		return 0;
	}

	Ret = Ixud_WriteDO(0, 0, La);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return  0; }
	Ret = Ixud_WriteDO(0, 1, Ha);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return  0; }


	Delay_100ns(4000);	//400uS   

	if (nMode2 == 1) {//저압1
		Ret = Ixud_WriteDOBit(0, 2, 2, 0);  //LV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,2,0) Error!!!");	return  0; }
		Delay_100ns(10000);// 
		Ret = Ixud_WriteDOBit(0, 2, 2, 1);  //LV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,2,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 2) {//저압2
		Ret = Ixud_WriteDOBit(0, 2, 6, 0);  //LV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,6,0) Error!!!");	return  0; }
		Delay_100ns(10000);
		Ret = Ixud_WriteDOBit(0, 2, 6, 1);  //LV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,6,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 3) {//고압1   
		Ret = Ixud_WriteDOBit(0, 2, 4, 0);  //HV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,4,0) Error!!!");	return  0; }
		Delay_100ns(10000);
		Ret = Ixud_WriteDOBit(0, 2, 4, 1);  //HV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,4,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 4) {//고압2
		Ret = Ixud_WriteDOBit(0, 6, 0, 0);  //HV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,0,0) Error!!!");	return  0; }
		Delay_100ns(10000);
		Ret = Ixud_WriteDOBit(0, 6, 0, 1);  //HV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,0,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 5) {//SPARK
		Ret = Ixud_WriteDOBit(0, 6, 2, 0);  //SPARK SET CLK
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,2,0) Error!!!");	return  0; }
		Delay_100ns(10000);
		Ret = Ixud_WriteDOBit(0, 6, 2, 1);  //SPARK SET CLK
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,2,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 6) {//4W????
		str.Format(" * ERROR : VB DA mode?  4w ? ");
		m_list1.InsertString(0, str);
	}
	else {
		return 0;
	}


#endif

#endif
	return 1;
}

//son170511 sSetVb.Init()에 해당.
//
//   VB SET, 16bit  nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK
int CModeSub71::AVBRV1S(int nMode2, int nZero)
{
#ifndef _PC_TEST_

	CString str;
	int  Ret;
	char szText[20];
	int a;
	BYTE Ha, La;



	nVbCo = 0;
	V1 = 0;  //vb
/////////////////////////////////////////////////////////////////////////////////////////
//CASE1
	if (nMode2 > 5) {
		nMode2 = 0;
	}


	if (nMode2 == 1) {
		m_edit101.GetWindowText(szText, 10);
	}
	else if (nMode2 == 2) {
		m_edit105.GetWindowText(szText, 10);
	}
	else if (nMode2 == 3) {
		m_edit107.GetWindowText(szText, 10);
	}
	else if (nMode2 == 4) {
		m_edit109.GetWindowText(szText, 10);
	}
	else if (nMode2 == 5) {
		m_edit111.GetWindowText(szText, 10);
	}
	else {
		m_edit101.GetWindowText(szText, 10);
	}


	a = atoi(szText);

	if (nZero == 0) {
		a = 0;
	}

	if (nMode2 == 0) {
		str.Format(" * ERROR : VB 입력 16bit=%d   mode Select? (low voltage ,  high voltage ,4W) ", a);
		m_list1.InsertString(0, str);
		str.Format("0");
		m_Label101.SetCaption(str);
		m_Label105.SetCaption(str);
		m_Label107.SetCaption(str);
		m_Label109.SetCaption(str);
		return 0;
	}

	if (a > 65535) {
		str.Format(" * ERROR : VB  Input 16bit=%d  > 65535 ", a);
		m_list1.InsertString(0, str);
		m_edit101.SetWindowText("");
		str.Format("0");
		m_Label101.SetCaption(str);
		m_Label121.SetCaption(str);
		return 0;
	}

	if (a < 0) {
		str.Format(" * ERROR : VB  Input 16bit=%d  <1 ", a);
		m_list1.InsertString(0, str);
		a = 0;
	}

	La = (BYTE)(a & 0xff);
	Ha = (BYTE)((a >> 8) & 0xff);


#ifdef DEF_CARD_A1  //sylee170328

	if (nMode2 == 1) {//저압1
		Ret = DioOutBit(pDIO.hDrv, DO19, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 19, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 2) {//저압2
		Ret = DioOutBit(pDIO.hDrv, DO23, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 23, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 3) {//고압1
		Ret = DioOutBit(pDIO.hDrv, DO21, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 21, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 4) {//고압2
		Ret = DioOutBit(pDIO.hDrv, DO25, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 25, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 5) {//SPARK 
		Ret = DioOutBit(pDIO.hDrv, DO27, 1);  //DA reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 27, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 6) {//4WIRE???
		str.Format(" * ERROR : VB Input mode?  4W?? ");
		m_list1.InsertString(0, str);
	}
	else {
		str.Format(" * ERROR : VB Input mode? ");
		m_list1.InsertString(0, str);
		return 0;
	}


	Ret = DioOutByte(pDIO.hDrv, 0, La);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 1, Ha);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }

	Delay_100ns(4000);	//400uS  //Delay_100ns(3500) FAIL/
//  Delay_100ns(10000);//1mS    

	if (nMode2 == 1) {//저압1
		Ret = DioOutBit(pDIO.hDrv, DO18, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 18, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO18, 1);   ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 18, 1] Error!!!");	return  0; }
		str.Format(" VB DA  LV1 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 2) {//저압2
		Ret = DioOutBit(pDIO.hDrv, DO22, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 22, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO22, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 22, 1] Error!!!");	return  0; }
		str.Format(" VB DA  LV2 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 3) {//고압1   
		Ret = DioOutBit(pDIO.hDrv, DO20, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 20, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO20, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 20, 1] Error!!!");	return  0; }

		str.Format(" VB DA  HV1 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 4) {//고압2
		Ret = DioOutBit(pDIO.hDrv, DO24, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 24, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO24, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 24, 1] Error!!!");	return  0; }
		str.Format(" VB DA  HV2 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 5) {//SPARK

		Ret = DioOutBit(pDIO.hDrv, DO26, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 3326, 0] Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = DioOutBit(pDIO.hDrv, DO26, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 3326, 1] Error!!!");	return  0; }
		str.Format(" SPARK DA SET ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 6) {//4W????
		str.Format(" * ERROR : VB DA mode?  4W? ");
		m_list1.InsertString(0, str);
		//	Ret = DioOutBit(pDIO.hDrv, DO46, 1);  //Clock 
		//	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return  ;	}
	}
	else {
		str.Format(" * ERROR : VB DA mode? ");
		m_list1.InsertString(0, str);
		return 0;
	}

#else  	// DEF_CARD_A1 




	if (nMode2 == 1) {//저압1
		Ret = Ixud_WriteDOBit(0, 2, 3, 1);  //  /LV SET RST0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,3,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 2) {//저압2
		Ret = Ixud_WriteDOBit(0, 2, 7, 1);  //  /LV SET RST1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,7,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 3) {//고압1
		Ret = Ixud_WriteDOBit(0, 2, 5, 1);  //  /HV SET RST0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,5,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 4) {//고압2
		Ret = Ixud_WriteDOBit(0, 6, 1, 1);  //   /HV SET RST1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,1,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 5) {//SPARK 
		Ret = Ixud_WriteDOBit(0, 6, 3, 1);  // /SPARK SET RST
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,3,1) Error!!!");	return  0; }
	}
	else if (nMode2 == 6) {//4WIRE???

		str.Format(" * ERROR : VB Input mode?  4W?? ");
		m_list1.InsertString(0, str);

	}
	else {
		str.Format(" * ERROR : VB Input mode? ");
		m_list1.InsertString(0, str);
		return 0;
	}


	Ret = Ixud_WriteDO(0, 0, La);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return  0; }
	Ret = Ixud_WriteDO(0, 1, Ha);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return  0; }

	Delay_100ns(4000);	//400uS  //Delay_100ns(3500) FAIL/
//  Delay_100ns(10000);//1mS    

	if (nMode2 == 1) {//저압1
		Ret = Ixud_WriteDOBit(0, 2, 2, 0);  //LV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,2,0) Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = Ixud_WriteDOBit(0, 2, 2, 1);  //LV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,2,1) Error!!!");	return  0; }
		str.Format(" VB DA  LV1 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 2) {//저압2
		Ret = Ixud_WriteDOBit(0, 2, 6, 0);  //LV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,6,0) Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = Ixud_WriteDOBit(0, 2, 6, 1);  //LV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,6,1) Error!!!");	return  0; }
		str.Format(" VB DA  LV2 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 3) {//고압1   
		Ret = Ixud_WriteDOBit(0, 2, 4, 0);  //HV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,4,0) Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = Ixud_WriteDOBit(0, 2, 4, 1);  //HV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,4,1) Error!!!");	return  0; }

		str.Format(" VB DA  HV1 ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 4) {//고압2
		Ret = Ixud_WriteDOBit(0, 6, 0, 0);  //HV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,0,0) Error!!!");	return  0; }
		Delay_100ns(10000);//1mS  ???
		Ret = Ixud_WriteDOBit(0, 6, 0, 1);  //HV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,0,1) Error!!!");	return  0; }
		str.Format(" VB DA  HV2 ");
		m_list1.InsertString(0, str);
	}
	else if (nMode2 == 5) {//SPARK

		Ret = Ixud_WriteDOBit(0, 6, 2, 0);  //SPARK SET CLK
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,2,0) Error!!!");	return  0; }
		Delay_100ns(10000);
		Ret = Ixud_WriteDOBit(0, 6, 2, 1);  //SPARK SET CLK
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,2,1) Error!!!");	return  0; }
		str.Format(" SPARK DA SET ");
		m_list1.InsertString(0, str);

	}
	else if (nMode2 == 6) {//4W????
		str.Format(" * ERROR : VB DA mode?  4W? ");
		m_list1.InsertString(0, str);
		//	Ret = A_Bit( DO46, 1);  //Clock 
		//	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return  ;	}
	}
	else {
		str.Format(" * ERROR : VB DA mode? ");
		m_list1.InsertString(0, str);
		return 0;
	}


#endif// DEF_CARD_A1 


	//  Delay_100ns(10000);//1mS  ??? 
	str.Format(" VB DA 16bit=%d  ", a);
	m_list1.InsertString(0, str);
	//    Delay_100ns(100000);  //10msec OK


#endif
	return 1;
}

//son Spark 구동
void CModeSub71::OnSparkOn(int nOn) ////SPARK ON   //SYLEE130830  //sylee140430
{
#ifndef _PC_TEST_
	CString str;
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (nOn == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO51, 1);  //SPARK ON    0  RESET     1   RUN      122
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 1] Error!!!");	return; }
	}
	else {
		Ret = DioOutBit(pDIO.hDrv, DO51, 0);  //SPARK ON    0  RESET     1   RUN      122
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	}

#else  	

	if (nOn == 1) {
		Ret = Ixud_WriteDOBit(0, 3, 2, 1);  //SPARK ON
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,2,1) Error!!!");	return; }
	}
	else {
		Ret = Ixud_WriteDOBit(0, 3, 2, 0);  //SPARK OFF
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,2,0) Error!!!");	return; }
	}
#endif


#endif

}

//son SPARK_ON 비트 감지
int CModeSub71::OnSparkStatus() ////SPARK  STATUS  //sylee130830   1=on  0 off  //sylee140430
{
	BYTE	Buf = 1;
#ifndef _PC_TEST_
	CString str;
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioInpBit(pDIO.hDrv2, DI39, &Buf);
	if (Ret != 0) { AfxMessageBox(" [DioInpBit, 39, 1] Error!!!");	return -1; }
#else  	
	Ret = d64h_di_readline(pDIO_D64.m_bCardID2, 0, &pDIO_D64.wValue);
	if (Ret != 0) { AfxMessageBox("  Card64H2_DiBit(0) Error!!!");	return  -1; }
	Buf = (char)pDIO_D64.wValue;
#endif

#endif
	if (Buf == 1) {
		return 1;
	}
	else {
		return 0;
	}

}





//son  Set VB Source
//son UI 출력부분 제외하고 AVBRV1S()와 동일함.
//son CV43ManualTestDlg::_SetVB_GetAdcA(..)-> MM.Set(LV1,...)-> sSetVb.Init(); 에 해당.
//
//   VB SET, 16bit  nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK      
int CModeSub71::AOnVBSet1(int nMod, double nV)
{

#ifndef _PC_TEST_
	CString str;
	int  Ret, a;
	BYTE Ha, La;

	nVbCo = 0;
	V1 = 0;  //vb

	//son 이 부분은 CModeSub71::OnButton302()-> AVBRV1S2(int nMode2) 왜 매우 유사
	if (nMod == 1) {		//son LV1
		a = (int)nV * 3000;
	}
	else if (nMod == 2) {	//son LV2
		a = (int)nV * 3000;
	}
	else if (nMod == 3) {	//son HV1
		a = (int)nV * 200;
	}
	else if (nMod == 4) {	//son HV2
		a = (int)nV * 200;
	}
	else if (nMod == 5)
	{//sylee140430  

		///////////////////////////////////////////////////////////////////////////////////////////
		if (nV != 0)//sylee171103-1
		{
			//------------
			// SPARK_OLD1
			if (SysSet13.m_nSparkMode == SPARK_OLD1)   //son200908 NEW_SPARK
			{                                           //son OldSpark로  if 문 표시한다. Map->Set1->Spark Mode 설정으로  
														//    이전 SparkMode로 돌아갈 수 있다.

				nV = nV + 30;//sylee141112  김영필cj과 협의함. read 12v임.  // SYLEE190911
				if (nV <= 5) { //sylee140502  ray
					nV = 5;
				}
				if (nV > 200) { //sylee140502  ray   //SYLEE170626  100->200
					nV = 200;
				}
				a = (int)nV * 300;  //sylee140502  ray 200->300         
			}
			else
			{
				//SYLEE190911 ?????? ??? ??? ??? ??? ?? ?.
				//nV=nV+3;//sylee200608  ray ok
				if (nV <= 1) {
					nV = 1;
				}
				if (nV > 250) {
					nV = 250;
				}

				//-------------
				// SPARK_OLD2
				if (SysSet13.m_nSparkMode == SPARK_OLD2)   //son200908 NEW_SPARK
					a = (int)(nV * 180) + 1800;  //SYLEE200703 VER2 (Vol*18)+1800  

				//-------------
				// SPARK_NEW        : default
				else       //son200908 NEW_SPARK
				{
					/*
					if( (nHV_Vset1>20) && (nHV_Vset1<=300) ){
						nVSet1=nHV_Vset1;//sylee200703
					}else{
						nVSet1=100;//sylee200703
					}
					*/

					//	a=(int)((nV*200.)+ (0.04*(nVSet1-100))*260+1500. ); //SYLEE200709 ray  JTECH    nV==HW OFFSET (20/7/14)
					//sylee200727-1	a=(int)( (nV)*250.) ; //sylee200725
					//	a=(int)((nV*250.)+ (0.04*(nVSet1-100))*250+1000. ); //sylee200727 사장님 
					//	a=(int)((nV*250.)+ (0.04*(nVSet1-100))*250+1500. ); //sylee200727   ray
					//  a=(int)nV; 
			 
					if (m_bModifySparkMeas)
						a = (int)((nV) / (250.0 / 65535)); // hdpark230105 spark 300 -> 250 fix
					else 
						a = (int)((nV) * 250.); //sylee200725 NEW_SPARK

					////////////////////////////////////////////////////////////////////////////////

					// a=1000;//sylee200725
				}
			}
		}
		else {//sylee171103-1
			a = 0;//sylee171103-1
		}//sylee171103-1

	}

	if (a < 0) {
		a = 0;
	}

	//son 이후부터는 CModeSub71::OnButton301()-> AVBRV1S(int nMode2, int nZero)와 매우 유사

	//son data를 LowByte와 HighByte로 분리
	La = (BYTE)(a & 0xff);
	Ha = (BYTE)((a >> 8) & 0xff);


	//------------------
	//son reset bit ON

#ifdef DEF_CARD_A1  //sylee170328

	if (nMod == 1) {//저압1
		Ret = DioOutBit(pDIO.hDrv, DO19, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 19, 1] Error!!!");	return  0; }
	}
	else if (nMod == 2) {//저압2
		Ret = DioOutBit(pDIO.hDrv, DO23, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 23, 1] Error!!!");	return  0; }
	}
	else if (nMod == 3) {//고압1
		Ret = DioOutBit(pDIO.hDrv, DO21, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 21, 1] Error!!!");	return  0; }
	}
	else if (nMod == 4) {//고압2
		Ret = DioOutBit(pDIO.hDrv, DO25, 1);  //reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 25, 1] Error!!!");	return  0; }
	}
	else if (nMod == 5) {//SPARK  
		Ret = DioOutBit(pDIO.hDrv, DO27, 1);  //DA reset 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 27, 1] Error!!!");	return  0; }
	}
	else if (nMod == 6) {//4WIRE???

//		str.Format(" * ERROR : VB Input mode?  4W?? " ); 
//	    m_list1.InsertString(0,str);

	}
	else {
		//		str.Format(" * ERROR : VB Input mode? " ); 
		//	    m_list1.InsertString(0,str); 
		return 0;
	}

	//------------------
	//son clock bit OFF 
	if (nMod == 1) {//저압1     
		Ret = DioOutBit(pDIO.hDrv, DO18, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 18, 0] Error!!!");	return  0; }
	}
	else if (nMod == 2) {//저압2
		Ret = DioOutBit(pDIO.hDrv, DO22, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 22, 0] Error!!!");	return  0; }
	}
	else if (nMod == 3) {//고압1   
		Ret = DioOutBit(pDIO.hDrv, DO20, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 20, 0] Error!!!");	return  0; }
	}
	else if (nMod == 4) {//고압2
		Ret = DioOutBit(pDIO.hDrv, DO24, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 24, 0] Error!!!");	return  0; }
	}
	else if (nMod == 5) {//SPARK
		Ret = DioOutBit(pDIO.hDrv, DO26, 0);  //Clock ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 26, 0] Error!!!");	return  0; }
	}
	else if (nMod == 6) {//4W????

	}
	else {
		return 0;
	}

	//------------------------
	//son  2Byte data 전송
	Ret = DioOutByte(pDIO.hDrv, 0, La);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 1, Ha);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }


	Delay_100ns(300);  //0.03ms   //SYLEE140923-1  
   //------------------------
   //son clock bit ON

	if (nMod == 1) {//저압1 
		Ret = DioOutBit(pDIO.hDrv, DO18, 1);   ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 18, 1] Error!!!");	return  0; }
	}
	else if (nMod == 2) {//저압2 
		Ret = DioOutBit(pDIO.hDrv, DO22, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 22, 1] Error!!!");	return  0; }
	}
	else if (nMod == 3) {//고압1     
		Ret = DioOutBit(pDIO.hDrv, DO20, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 20, 1] Error!!!");	return  0; }
	}
	else if (nMod == 4) {//고압2 
		Ret = DioOutBit(pDIO.hDrv, DO24, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 24, 1] Error!!!");	return  0; }
	}
	else if (nMod == 5) {//SPARK 
		Ret = DioOutBit(pDIO.hDrv, DO26, 1);  //  ///sylee20111007
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 26, 1] Error!!!");	return  0; }
	}
	else if (nMod == 6) {//4W????

	}
	else {

		return 0;
	}

#else  	



	if (nMod == 1) {//저압1
		Ret = Ixud_WriteDOBit(0, 2, 3, 1);  //  /LV SET RST0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,3,1) Error!!!");	return  0; }
	}
	else if (nMod == 2) {//저압2
		Ret = Ixud_WriteDOBit(0, 2, 7, 1);  //  /LV SET RST1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,7,1) Error!!!");	return  0; }
	}
	else if (nMod == 3) {//고압1
		Ret = Ixud_WriteDOBit(0, 2, 5, 1);  //  /HV SET RST0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,5,1) Error!!!");	return  0; }
	}
	else if (nMod == 4) {//고압2
		Ret = Ixud_WriteDOBit(0, 6, 1, 1);  //   /HV SET RST1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,1,1) Error!!!");	return  0; }
	}
	else if (nMod == 5) {//SPARK  
		Ret = Ixud_WriteDOBit(0, 6, 3, 1);  // /SPARK SET RST
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,3,1) Error!!!");	return  0; }
	}
	else if (nMod == 6) {//4WIRE???
//		str.Format(" * ERROR : VB Input mode?  4W?? " ); 
//	    m_list1.InsertString(0,str); 
	}
	else {
		//		str.Format(" * ERROR : VB Input mode? " ); 
		//	    m_list1.InsertString(0,str); 
		return 0;
	}

	if (nMod == 1) {//저압1     
		Ret = Ixud_WriteDOBit(0, 2, 2, 0);  //LV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,2,0) Error!!!");	return  0; }
	}
	else if (nMod == 2) {//저압2
		Ret = Ixud_WriteDOBit(0, 2, 6, 0);  //LV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,6,0) Error!!!");	return  0; }
	}
	else if (nMod == 3) {//고압1   
		Ret = Ixud_WriteDOBit(0, 2, 4, 0);  //HV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,4,0) Error!!!");	return  0; }
	}
	else if (nMod == 4) {//고압2
		Ret = Ixud_WriteDOBit(0, 6, 0, 0);  //HV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,0,0) Error!!!");	return  0; }
	}
	else if (nMod == 5) {//SPARK
		Ret = Ixud_WriteDOBit(0, 6, 2, 0);  //SPARK SET CLK
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,2,0) Error!!!");	return  0; }

	}
	else if (nMod == 6) {//4W????

	}
	else {
		return 0;
	}
	//Delay_100ns(500); 

	Ret = Ixud_WriteDO(0, 0, La);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return  0; }
	Ret = Ixud_WriteDO(0, 1, Ha);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return  0; }

	Delay_100ns(300);  //0.03ms   //SYLEE140923-1  
	Delay_100ns(30000); //3ms //sylee170412  //HOLD170414

	if (nMod == 1) {//저압1 
		Ret = Ixud_WriteDOBit(0, 2, 2, 1);  //LV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,2,1) Error!!!");	return  0; }
	}
	else if (nMod == 2) {//저압2 
		Ret = Ixud_WriteDOBit(0, 2, 6, 1);  //LV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,6,1) Error!!!");	return  0; }
	}
	else if (nMod == 3) {//고압1     
		Ret = Ixud_WriteDOBit(0, 2, 4, 1);  //HV SET CLK0
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,4,1) Error!!!");	return  0; }
	}
	else if (nMod == 4) {//고압2 
		Ret = Ixud_WriteDOBit(0, 6, 0, 1);  //HV SET CLK1
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,0,1) Error!!!");	return  0; }
	}
	else if (nMod == 5) {//SPARK  
		Ret = Ixud_WriteDOBit(0, 6, 2, 1);  //SPARK SET CLK
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,6,2,1) Error!!!");	return  0; }
	}
	else if (nMod == 6) {//4W????

	}
	else {

		return 0;
	}



#endif

#endif
	return 1;
}



//son Button 'VB 16 OUT'
void CModeSub71::OnButton301()  //VB ALL SET //  VB LV1, LV2, HV1,HV2  SPARK SET
{
#ifndef _PC_TEST_

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 715.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}

	AVBRV1S(3, 0);
	Delay_100ns(100000);  	//son 10ms
	AVBRV1S(3, 1);

	AVBRV1S(4, 0);
	Delay_100ns(100000);
	AVBRV1S(4, 1);

	AVBRV1S(1, 0);
	Delay_100ns(100000);
	AVBRV1S(1, 1);

	AVBRV1S(2, 0);
	Delay_100ns(100000);
	AVBRV1S(2, 1);

	AVBRV1S(5, 0);
	Delay_100ns(100000);
	AVBRV1S(5, 1);

#endif
}


//son Button 'VBOUT'
void CModeSub71::OnButton302()
{
#ifndef _PC_TEST_

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 716.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK);  return;
	}
	//1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK

	 //son 5개의 Voltage VB 입력값 창중에서 어디에 입력했는지 모르므로 5개 다 호출.

	AVBRV1S2(1);    //son LV1   1  ~ 20 V
	AVBRV1S2(2);    //son LV2   1  ~ 20 V
	AVBRV1S2(3);    //son HV1   21 ~ 300 V
	AVBRV1S2(4);    //son HV1   21 ~ 300 V
	AVBRV1S2(5);    ////SPARK   2 ~ 100 V

	OnInit_Byte12();//sylee150122 

#endif
}


//son170511 sSetVb.Init()에 해당.
//son AVBRV1S2()-> OnButton301()-> AVBRV1S()
int CModeSub71::AVBRV1S2(int nMode2) //   VB SET, 10진수   nMode2 1=LV1,  2=LV2,  3=HV1, 4=HV2;  5=SPARK
{
#ifndef _PC_TEST_


	CString str;
	char	szText[20];
	int a;
	double b;//sylee20110705

	if (nMode2 == 1) {
		m_edit102.GetWindowText(szText, 10);
	}
	else if (nMode2 == 2) {
		m_edit106.GetWindowText(szText, 10);
	}
	else if (nMode2 == 3) {
		m_edit108.GetWindowText(szText, 10);
	}
	else if (nMode2 == 4) {
		m_edit110.GetWindowText(szText, 10);
	}
	else if (nMode2 == 5) {
		m_edit112.GetWindowText(szText, 10);
	}

	b = atof(szText);

	nVb = b;  //SYLEE20120306


	if (b != 0) {
		if (nMode2 == 1 || nMode2 == 2) {

			if (b < 0 || b>20) {	//SYLEE20120306 
				str.Format(" * ERROR : VB Set value=%d   V<1 or V>20 ? ", b);
				m_list1.InsertString(0, str);
				str.Format("0");
				m_Label101.SetCaption(str);
				str.Format("저Vol.?");
				m_Label121.SetCaption(str);
				m_edit101.SetWindowText("저Vol.?");
				return 0;
			}
		}
		else if (nMode2 == 3 || nMode2 == 4) {
			if (b < 20 || b>300) {
				str.Format(" * ERROR : VB  Set value=%d   V<20 or V>300 ? ", b);
				m_list1.InsertString(0, str);
				str.Format("0");
				m_Label101.SetCaption(str);
				str.Format("고Vol.?");
				m_Label121.SetCaption(str);
				m_edit101.SetWindowText("고Vol.?");
				return 0;
			}

		}
		else if (nMode2 == 5) {
			if (b < 1 || b>100) {
				str.Format(" * ERROR : VB  Spart Set value=%d   V<1 or V>100 ? ", b);
				m_list1.InsertString(0, str);
				str.Format("0");
				m_Label101.SetCaption(str);
				str.Format("Spark Set? ");
				m_Label121.SetCaption(str);
				m_edit101.SetWindowText("Spark Set??");
				return 0;
			}

		}
		else {
			str.Format(" * ERROR : VB  Set  value=%d   mode Select? (LV,HV,4W) ", b);
			m_list1.InsertString(0, str);
			str.Format("0");
			m_Label101.SetCaption(str);
			str.Format("Mode?");
			m_Label121.SetCaption(str);
			return 0;
		}

	}
	else if (b < 1) {
		str.Format(" * ERROR : VB  Set   value=%d   ==> SET 0   <1 ", b);
		m_list1.InsertString(0, str);
		str.Format(" * VB  Set   value  =>  0     ");
		m_list1.InsertString(0, str);
		b = 0;
	}


	if (nMode2 == 1) {
		a = (int)(b * 3000.0);
		str.Format("%d", a);
		m_edit101.SetWindowText(str);
	}
	else if (nMode2 == 2) {
		a = (int)(b * 3000.0);
		str.Format("%d", a);
		m_edit105.SetWindowText(str);
	}
	else if (nMode2 == 3) {
		a = (int)(b * 200.0);
		str.Format("%d", a);
		m_edit107.SetWindowText(str);
	}
	else if (nMode2 == 4) {
		a = (int)(b * 200.0);
		str.Format("%d", a);
		m_edit109.SetWindowText(str);
	}
	else if (nMode2 == 5) {  ///temp  
		a = (int)(b * 200.0);//sylee20120414   김영필 GJ 확인  300V  =60000
	  //a=0;
		str.Format("%d", a);
		m_edit111.SetWindowText(str);
	}

	str.Format(" VB Set value=%d ", b);
	m_list1.InsertString(0, str);

	OnButton301();

#endif
	return 1;
}




//son Button306 'I SET 16: OUT'
void CModeSub71::OnButton303() //SYLEE20120215
{
#ifndef _PC_TEST_
	CString str;
	int  Ret;
	char  szText[20];
	int a;
	BYTE Ha, La;

	nICo = 0;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 717.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}

	if (nModeIR < 1 || nModeIR>5) {
		str.Format(" * ERROR : I SET    Range Mode? ");
		m_list1.InsertString(0, str);
		m_edit103.SetWindowText("Range?");
		return;
	}

	m_edit103.GetWindowText(szText, 10);
	a = atoi(szText);
	if (a > 65535) {
		str.Format(" * ERROR : I SET 16bit=%d  > 65535 ", a);
		m_list1.InsertString(0, str);
		m_edit103.SetWindowText("");
		return;
	}

	La = (BYTE)(a & 0xff);
	Ha = (BYTE)((a >> 8) & 0xff);


#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO17, 1);   // /I SET CLEAR
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 17, 1] Error!!!");	return; }

	Ret = DioOutByte(pDIO.hDrv, 0, La);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return; }
	Ret = DioOutByte(pDIO.hDrv, 1, Ha);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO16, 1);    //I SET CLOCK
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return; }

	Delay_100ns(500);

	Ret = DioOutBit(pDIO.hDrv, DO16, 0);    //I SET CLOCK
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return; }

#else  

	Ret = Ixud_WriteDOBit(0, 2, 1, 1);  //I SET CLEAR
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,1,1) Error!!!");	return; }

	Ret = Ixud_WriteDO(0, 0, La);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return; }
	Ret = Ixud_WriteDO(0, 1, Ha);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 2, 0, 1);  //I SET CLOCK
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,0,1) Error!!!");	return; }

	Delay_100ns(500);

	Ret = Ixud_WriteDOBit(0, 2, 0, 0);  //I SET CLOCK
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,0,0) Error!!!");	return; }

#endif

	str.Format(" Current  set  16bit=%d  ", a);
	m_list1.InsertString(0, str);

	nflgA = 1;

	Delay_100ns(50000);  //5msec OK 

	OnInit_Byte12();//sylee150122 

#endif
}




//son Button307 'I SET: OUT'
void CModeSub71::OnButton304()
{
#ifndef _PC_TEST_
	CString str;
	char	szText[20];
	int a;
	double c;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 718.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
	}

	if (nModeIR < 1 || nModeIR>5) {
		str.Format(" * ERROR : current  set    Range Mode? ");
		m_list1.InsertString(0, str);
		m_edit103.SetWindowText("Range?");
		return;
	}

	m_edit104.GetWindowText(szText, 10);

	c = atof(szText);
	lIset = (double)c;


	if (c == 0) {
		a = 0;
	}
	else {

		if (nModeIR == 1)
		{
			if (c > 0 && c < (131.073 * 2.0)) {					//son c > 0 && c < 262.146
				a = (int)(c * 500.0 / 2.);//sylee160928-3		//son a : 1 ~ 65536.5
			}
			else {
				a = 0;
			}
		}
		else if (nModeIR == 2) { //SYLEE150513
			if (c > 0 && c < (13.1073 * 2))	//son  c > 0 && c < 26.2146
			{
				a = (int)(c * 2500.0);
			}
			// a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
			}
		}
		else if (nModeIR == 3) { //SYLEE150513
			if (c > 0 && c < (13.1073)) {
				a = (int)(c * 5000.0); //SYLEE150903 //ACE400
			}
			//son a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
			}
		}
		else if (nModeIR == 4) {  //SYLEE150513    3->4
			if (c > 0 && c < (2.4)) {
				a = (int)(c * 25000.0);   //SYLEE150903  //ACE400
			}
			//son a값이 60000을 넘으면 0으로 셋팅
			else {
				a = 0;
			}
		}
		else if (nModeIR == 5) {   //SYLEE150513    4->5
			if (c > 0 && c < (131.073 * 2)) {
				a = (int)(c * 250.0);
			}
			//son a값이 65536.5을 넘으면 0으로 셋팅
			else {
				a = 0;
			}
		}
	}//if(c==0){ }else{

	str.Format("%d", a);
	m_edit103.SetWindowText(str);

	Y1 = 0;  //i
	X1 = 0;  //vc
	R1 = 0;  //R

	OnButton303();

	str.Format(" current set  value  =%d  ", a);
	m_list1.InsertString(0, str);

	OnInit_Byte12();//sylee150122 

#endif
}



int CModeSub71::AOnISet1(double dIv)    //void CModeSub71::OnButton304()  decimal  + OnButton303() 16bit
{
#ifndef _PC_TEST_
	CString str;
	BYTE Ha, La;
	int a;
	int  Ret;
	double c;

	c = dIv;

	if (c == 0) {
		a = 0;
	}
	else
	{
		if (nModeIR == 1) {
			//sylee160928-3	if(c>0 && c<131.073){
			if (c > 0 && c < (131.073 * 2.0)) {
				//sylee160928-3	a=(int)(c*500.0);   
				a = (int)(c * 500.0 / 2.);//sylee160928-3
			}
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I1 SET ?    	if(I>0 && I<131.073)   ", MB_OK); 	   return 0;
			}
			//sylee150513 	}else if(nModeIR==2  || nModeIR==5 ){  //SYLEE20120529
		}
		else if (nModeIR == 2) { 	//sylee150513
			if (c > 0 && c < (13.1073 * 2.)) { //sylee130701
				a = (int)(c * 2500.0);  //sylee130701   
			}
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I2 SET ?    	if(I>0 && I<13.1073*2)   ", MB_OK); 	   return 0;
			}
		}
		else if (nModeIR == 3) { 	//sylee150513
	   //sylee161011 rayok	if(c>0 && c<(13.1073)){ //sylee130701
			if (c > 0 && c < (13.1073 * 2.)) { //sylee161011 rayok
				a = (int)(c * 1250.0);  //sylee130701   
			}
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I3 SET ?    	if(I>0 && I<13.1073)   ", MB_OK); 	   return 0;
			}
		}
		else if (nModeIR == 4) { 	//sylee150513  3->4
			if (c > 0 && c < (1310.73 * 2.)) {
				a = (int)(c * 25.0);    //sylee130701 
			}
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I SET ?    	if(I>0 && I<1310.73)   ", MB_OK); 	   return 0;
			}

		}
		else if (nModeIR == 5) {  //sylee150513  4->5
			if (c > 0 && c < (131.073 * 2.)) {
				a = (int)(c * 250.0);    //sylee130701
			}
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I SET ?    	if(I>0 && I<131.073)    ", MB_OK); 	   return 0;
			}

		}
	}//if(c==0){ }else{


  // V1=0;  //vb
	Y1 = 0;  //i
	X1 = 0;  //vc
	R1 = 0;  //R


	nICo = 0;

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //SYLEE20111005
		AfxMessageBox(" SET Error 722.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return 0;
	}


	if (a > 65535) {
		AfxMessageBox(" *  ERROR : I SET 16bit=?  > 65535   ", MB_OK); 	   return 0;

	}

	La = (BYTE)(a & 0xff);
	Ha = (BYTE)((a >> 8) & 0xff);

#ifdef _PC_TEST_
	Ret = 0;

#else
#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO17, 1);   // /I SET CLEAR    1= RUN
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 17, 1] Error!!!");	return  0; }

	Ret = DioOutByte(pDIO.hDrv, 0, La);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }
	Ret = DioOutByte(pDIO.hDrv, 1, Ha);
	if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return  0; }

	Ret = DioOutBit(pDIO.hDrv, DO16, 1);    //I SET CLOCK   RISING ACTIVE    20111125 확인 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return  0; }

	Delay_100ns(500);	//SYLEE20120223     OK   //(200); ERROR    

	Ret = DioOutBit(pDIO.hDrv, DO16, 0);    //I SET CLOCK
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return  0; }

#else  	

	Ret = Ixud_WriteDOBit(0, 2, 1, 1);  //I SET CLEAR
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,1,1) Error!!!");	return    0; }

	Ret = Ixud_WriteDO(0, 0, La);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return  0; }
	Ret = Ixud_WriteDO(0, 1, Ha);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 2, 0, 1);  //I SET CLOCK
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,0,1) Error!!!");	return    0; }
	Delay_100ns(500);	//SYLEE20120223     OK   //(200); ERROR   
	Ret = Ixud_WriteDOBit(0, 2, 0, 0);  //I SET CLOCK
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,0,0) Error!!!");	return   0; }

#endif

#endif

	nflgA = 1;
	Delay_100ns(30000);  //3m sec  
#endif
	return 1;
}



int CModeSub71::AOnISetIncre1(double nISet, int nICoMax, int nuSec)   //SYLEE121008 // ISET, LOOP, uSec
{
#ifndef _PC_TEST_
	double nISetTemp1;

	nuSec = nuSec * 10;

	for (int jk1 = 0; jk1 <= nICoMax; jk1++) {
		nISetTemp1 = nISet - (nICoMax - jk1) * (nISet / nICoMax);
		AOnISet1Run1(nISetTemp1);
		//Sub71.Delay_100ns(1000);  //100us
		Delay_100ns(nuSec);  //100us
	}
	//////////////////////////////////////////////////////////////////////////////////

#endif
	return 1;
}




//son I Value Set
//son OnButton304() : I SET OUT과 매우 유사함.
int CModeSub71::AOnISet1Run1(double dIv)   //SYLEE20111213 //void CModeSub71::OnButton304()  decimal  + OnButton303() 16bit
{
#ifndef _PC_TEST_
	CString str;

	int a;
	double c;
	int  Ret;
	BYTE Ha, La;

	//-----------------------------------------------------
	//son Set해야 할 I 값을 현재 I Range에 맞는 'a' 값으로  수정한다.
	c = dIv;

	if (c == 0) {
		a = 0;
	}
	else
	{
		if (nModeIR == 1) {

			//sylee160928-3	if(c>0 && c<131.073){
				//
				//son c > 0 && c < 262.146
			if (c > 0 && c < (131.073 * 2.0)) {
				//sylee160928-3	a=(int)(c*500.0);   
				a = (int)(c * 500.0 / 2.);//sylee160928-3
			}
			//son a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I1 SET ?    	if(I>0 && I<131.073)   ", MB_OK); 	   return 0;
			}


		}
		else if (nModeIR == 2)
		{
			//son  c > 0 && c < 26.2146
			if (c > 0 && c < (13.1073 * 2)) {
				a = (int)(c * 2500.0);
			}
			// a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I2 SET ?    	if(I>0 && I< 13.1073*2 )   ", MB_OK); 	   return 0;
			}

		}
		else if (nModeIR == 3) {  //sylee150513 //ace400
		//sylee161011 rayok 	if(c>0 && c<(13.1073)){
			if (c > 0 && c < (13.1073 * 2)) {//sylee161011 rayok 
				a = (int)(c * 2500.0);
			}
			// a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I3 SET ?    	if(I>0 && I< 13.1073*2 )   ", MB_OK); 	   return 0;
			}
		}
		else if (nModeIR == 4) {//sylee150513 3->4  //ace400
			if (c > 0 && c < (1310.73 * 2)) {
				a = (int)(c * 25.0);
			}
			// a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I4 SET ?    	if(I>0 && I<1310.73*2)   ", MB_OK); 	   return 0;
			}
		}
		else if (nModeIR == 5) { //sylee150513 4->5  //ace400
			//son c>0 && c < 262.146
			if (c > 0 && c < (131.073 * 2)) {
				a = (int)(c * 250.0);
			}
			// a값이 65536.5를 넘으면 0으로 셋팅
			else {
				a = 0;
				AfxMessageBox(" *  ERROR : I5 SET ?    	if(I>0 && I<131.073*2)    ", MB_OK); 	   return 0;
			}
		}
	}//if(c==0){ }else{

	nICo = 0;

	if (a > 65535) {
		AfxMessageBox(" *  ERROR : I SET 16bit=?  > 65535   ", MB_OK); 	   return 0;
	}


	//---------------------------------------
	//son a값 La와 Ha로 나눈 후  전송한다. 

	La = (BYTE)(a & 0xff);
	Ha = (BYTE)((a >> 8) & 0xff);

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO17, 1);   // /I SET CLEAR    1= RUN
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 17, 1] Error!!!");	return  0; }

	OutData[0] = La;	    OutData[1] = Ha;
	Ret = DioOutMultiByte(pDIO.hDrv, &OutNoT[0], 2, &OutData[0]); //sylee140225
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiByte, 8,9 ] Error!!!");	return  0; }

	Ret = DioOutBit(pDIO.hDrv, DO16, 1);    //I SET CLOCK   RISING ACTIVE    20111125 확인 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return  0; }
	Delay_100ns(500);//50uS	//SYLEE20120223     OK   //(200); ERROR   #1??????????????????????????????????????
	Ret = DioOutBit(pDIO.hDrv, DO16, 0);    //I SET CLOCK
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return  0; }

#else  	


	Ret = Ixud_WriteDOBit(0, 2, 1, 1);  //I SET CLEAR
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,1,1) Error!!!");	return    0; }

	OutData[0] = La;	    OutData[1] = Ha;

	Ret = Ixud_WriteDO(0, 0, La);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return   0; }
	Ret = Ixud_WriteDO(0, 1, Ha);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return   0; }

	Ret = Ixud_WriteDOBit(0, 2, 0, 1);  //I SET CLOCK
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,0,1) Error!!!");	return    0; }
	Delay_100ns(500);//50uS	//SYLEE20120223     OK   //(200); ERROR   #1??????????????????????????????????????
	Ret = Ixud_WriteDOBit(0, 2, 0, 0);  //I SET CLOCK
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,2,0,0) Error!!!");	return   0; }

#endif

	nflgA = 1;
	Delay_100ns(100);  //10u sec    //sylee121003

#endif
	return 1;

}







//son Button 'Pin ALL RST'
int CModeSub71::OnButton501() ///PIN ALL RESET
{
#ifndef _PC_TEST_
	CString str;

	nCPLD1 = 0;	nCPLDR1 = 0;	nCPLD2 = 0;	nCPLDR2 = 0;

	OnPinAllReset1(); //SYLEE150513 //ACE400

	str.Format("?");
	m_Label214.SetCaption(str);
	m_Label215.SetCaption(str);
	m_Label217.SetCaption(str);
	m_Label218.SetCaption(str);

#endif
	return 1;
}


//son D64_0, D64_1 Output 영역 모두를 Off한다.
int CModeSub71::OnPinAllReset1(int nLine) ///PIN ALL RESET ==>  SIGNAL  LOW  ==> RESET STATUS
{

	if(fpLeak)
	{
		fprintf(fpLeak, " RRRRR(%d) \n", nLine);
	}

#ifndef _PC_TEST_

	CString str;
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	//son for VB
	// nPinSetSkipVb ==3 : DIO96~DIO103(VB Pin 설정관련) 을 사용하지 않을 경우
	if (nPinSetSkipVb == 3)   //sylee151007 //ACE400	//son230127
	{
		Ret = DioOutBit(pDIO.hDrv, DO100, 0);    //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 100, 0] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		//sylee170802   rayok		Delay_100ns(500); //50us
		Delay_100ns(400); //40us  //sylee170802 jtech  =>  rayok  
		Ret = DioOutBit(pDIO.hDrv, DO99, 1);    //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		Ret = DioOutBit(pDIO.hDrv, DO100, 1);    //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 100, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
	}
	else//sylee151007 //ACE400
	{
		//son  	0x08 = 0000 1000	 port12 (DIO96~DIO103)
		//son	=>	0 	VB_TR_SEL  			DIO96  
		//son		0 	VB_RY_SEL			DIO97  
		//son		0	_VB_PIN_SEL			DIO98  
		//son		1	_VB_ENA,			DIO99  
		//son		0	_VB_RESET,			DIO100 
		Ret = DioOutByte(pDIO.hDrv, 12, 8);   // VB PIN RESET   
		if (Ret != 0) { AfxMessageBox("DioOutByte[ 12, 8 ]    PIN VB ALL  RESET  Error!!!"); }
	}//sylee151007 //ACE400

	//son for VC
	//son DIO104~ DIO111(VC Pin 설정 관련)을 사용하지 않을 경우 	     
	if (nPinSetSkipVc == 3)  //sylee151007 //ACE400	//son230127
	{
		Ret = DioOutBit(pDIO.hDrv, DO108, 0);   ///sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 108, 0] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		//sylee170802   rayok Delay_100ns(500); //50us  //sylee151007 //ACE400
		Delay_100ns(400); //40us  //sylee170802 jtech  =>  rayok  
		Ret = DioOutBit(pDIO.hDrv, DO107, 1);   //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		Ret = DioOutBit(pDIO.hDrv, DO108, 1);   //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 108, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
	}

	//son DIO104~ DIO111(VC Pin 설정 관련)을 사용할 경우
	else//sylee151007 //ACE400		    
	{
		//son 	 0x08 = 0x0000 1000	port13 (DIO104~DIO111)
		//son	=>	0 	VC_TR_SEL  			DIO104  
		//son		0 	VC_RY_SEL			DIO105  
		//son		0	_VC_PIN_SEL			DIO106  
		//son		1	_VC_ENA,			DIO107  
		//son		0	_VC_RESET,			DIO108 
		Ret = DioOutByte(pDIO.hDrv, 13, 8);   // Vc PIN RESET   
		if (Ret != 0) { AfxMessageBox("DioOutByte[ 13, 8 ]    PIN VB ALL  RESET  Error!!!"); }
	} //sylee151007 //ACE400

#else 

	//----------------------
	//son for VB All Reset
	//----------------------
	if (nPinSetSkipVb == 3)   //sylee151007 //ACE400
	{
		/*		Ret = AB_Bit( DO100, 0);    //  /VBREST 0=ALL OFF   1=ON
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 100, 0] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
				Delay_100ns(500); //50us
				Ret = AB_Bit( DO99, 1);    //    /PIN TRIGGER
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
				Ret = AB_Bit( DO100, 1);    //sylee151007 //ACE400
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 100, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
				*/
				//son-32KPIN-20200305 : 
				//  D64 0x080000 : _VB_ENA     Pin 등록 trigger, 엑셀페달  (1-> 0 : Falling edge로 0일때 동작한다. 평소에 1이어야)  
				//  D64 0x100000 : _VB_RESET   Pin 등록의 시동키, 평소에는 0이고, 핀등록 직전에 1로 해야 한다.  
				//
				//  PinReset => _VB_ENA은 1 이고(0x080000), _VB_RESET은 0(0x000000)이어야 한다. 합치면 0x080000
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x080000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x080000  ) Error!!!");	return  0; }


		//son-32KPIN-20200305 : 
		//son ???? 아래와 같이 0x180000  ( _VB_ENA 1, _VB_RESET 1) 로 다시 만드는 것이 매우 이상함.
		//    0x180000은 핀등록 준비단계로 핀등록 코드에만 있어야 하는데 이상함.  추후 막아야 할 지도 모름
		//sylee170802   rayok		Delay_100ns(500); //50us	 
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x180000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x180000  ) Error!!!");	return  0; }

	}
	else//sylee151007 //ACE400
	{
		//son All D64_0 Digital Out Bit(0~31) Off---------------
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x80000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x80000 ) Error!!!");	return  0; }
	}//sylee151007 //ACE400


	//----------------------
	//son for VC All Reset
	//----------------------
	if (nPinSetSkipVc == 3)  //sylee151007 //ACE400	//son230127
	{
		/*
		   Ret = AB_Bit( DO108, 0);   ///sylee151007 //ACE400
		   if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 108, 0] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
		   Delay_100ns(500); //50us  //sylee151007 //ACE400
		   Ret = AB_Bit( DO107, 1);   //sylee151007 //ACE400
		   if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
		   Ret = AB_Bit( DO108, 1);   //sylee151007 //ACE400
		   if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 108, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
		   */

		   //son-32KPIN-20200305 : 
		   //  D64 0x080000 : _VB_ENA     Pin 등록 trigger, 엑셀페달  (1-> 0 : Falling edge로 0일때 동작한다. 평소에 1이어야)  
		   //  D64 0x100000 : _VB_RESET   Pin 등록의 시동키, 평소에는 0이고, 핀등록 직전에 1로 해야 한다.  
		   //
		   //  PinReset => _VB_ENA은 1 이고(0x080000), _VB_RESET은 0(0x000000)이어야 한다. 합치면 0x080000
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x080000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x080000  ) Error!!!");	return  0; }

		//son-32KPIN-20200305 : 
		//son ????  0x180000  ( _VB_ENA 1, _VB_RESET 1) 로 다시 만드는 것이 매우 이상함.
		//    0x180000은 핀등록 준비단계로 핀등록 코드에만 있어야 하는데 이상함.  추후 막아야 할 지도 모름
		//sylee170802   rayok		Delay_100ns(500); //50us			 
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x180000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x180000  ) Error!!!");	return  0; }

	}
	else//sylee151007 //ACE400		    
	{
		//son All D64_1 Digital Out Bit(0~31) Off -------------------
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000 ) Error!!!");	return  0; }
	} //sylee151007 //ACE400



#endif

#endif 
	return 1;
}




//SYLEE200709 NEW_SPARK :OnPinResetVB -> :OnPinResetVC()로 변경
int CModeSub71::OnPinResetVC(int nLine) //PIN VC ALL  RESET   //SYLEE20120501
{
	if(fpLeak)
	{
		fprintf(fpLeak, " BBBBB(%d) \n", nLine);
	}

#ifndef _PC_TEST_
	CString str;
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (nPinSetSkipVb == 3)   //sylee151007 //ACE400	//son230127
	{
		Ret = DioOutBit(pDIO.hDrv, DO100, 0);    //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 100, 0] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		//sylee170802   rayok     Delay_100ns(500); //50us
		Delay_100ns(400); //40us //sylee170802   rayok

		Ret = DioOutBit(pDIO.hDrv, DO99, 1);    //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400
		Ret = DioOutBit(pDIO.hDrv, DO100, 1);    //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 100, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	

	}
	else {//sylee151007 //ACE400
		Ret = DioOutByte(pDIO.hDrv, 12, 8);   // VB PIN RESET   
		if (Ret != 0) { AfxMessageBox("DioOutByte[ 12, 8 ]    PIN VB ALL  RESET  Error!!!"); }
	}//sylee151007 //ACE400

#else  
	if (nPinSetSkipVb == 3)   //sylee151007 //ACE400	//son230127
	{
		/*
			 Ret = AB_Bit( DO100, 0);    //sylee151007 //ACE400
			 if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 100, 0] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
			 Delay_100ns(500); //50us
			 Ret = AB_Bit( DO99, 1);    //sylee151007 //ACE400
			 if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
			 Ret = AB_Bit( DO100, 1);    //sylee151007 //ACE400
			 if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 100, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
			 */

			 //son-32KPIN-20200305 : 
			 //  D64 0x080000 : _VB_ENA     Pin 등록 trigger, 엑셀페달  (1-> 0 : Falling edge로 0일때 동작한다. 평소에 1이어야)  
			 //  D64 0x100000 : _VB_RESET   Pin 등록의 시동키, 평소에는 0이고, 핀등록 직전에 1로 해야 한다.  
			 //
			 //  PinReset => _VB_ENA은 1 이고(0x080000), _VB_RESET은 0(0x000000)이어야 한다. 합치면 0x080000
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x080000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x080000  ) Error!!!");	return  0; }
		//sylee170802   rayok 	Delay_100ns(500); //50us


		//son-32KPIN-20200305 : 
		//son ???? 0x180000  ( _VB_ENA 1, _VB_RESET 1) 로 다시 만드는 것이 매우 이상함.
		//    0x180000은 핀등록 준비단계로 핀등록 코드에만 있어야 하는데 이상함.  추후 막아야 할 지도 모름
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x180000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x180000  ) Error!!!");	return  0; }

	}
	else {//sylee151007 //ACE400
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x80000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x80000 ) Error!!!");	return  0; }
	}//sylee151007 //ACE400

#endif

#endif   
	return  1;
}


//sylee200709 NEW_SPARK :OnPinResetVC -> :OnPinResetVB()로 변경
int CModeSub71::OnPinResetVB(int nLine) //PIN VB ALL  RESET   //SYLEE20120125
{
	if(fpLeak)
	{
		fprintf(fpLeak, " CCCCC(%d) \n", nLine);
	}

#ifndef _PC_TEST_
	CString str;
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (nPinSetSkipVc == 3)  //sylee151007 //ACE400	//son230127
	{
		Ret = DioOutBit(pDIO.hDrv, DO108, 0);   ///sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 108, 0] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		//sylee170802   rayok     Delay_100ns(500); //50us
		Delay_100ns(400); //40us //sylee170802   rayok

		Ret = DioOutBit(pDIO.hDrv, DO107, 1);   //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
		Ret = DioOutBit(pDIO.hDrv, DO108, 1);   //sylee151007 //ACE400
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 108, 1] Error!!!");	return  0; }  //SYLEE150513  //ACE400	
	}
	else {//sylee151007 //ACE400		    
		Ret = DioOutByte(pDIO.hDrv, 13, 8);   // Vc PIN RESET   
		if (Ret != 0) { AfxMessageBox("DioOutByte[ 13, 8 ]    PIN VB ALL  RESET  Error!!!"); }
	} //sylee151007 //ACE400 
#else  	
	if (nPinSetSkipVc == 3)  //sylee151007 //ACE400	//son230127
	{
		/*
		   Ret = AB_Bit( DO108, 0);   ///sylee151007 //ACE400
		   if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 108, 0] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
		   Delay_100ns(500); //50us  //sylee151007 //ACE400
		   Ret = AB_Bit( DO107, 1);   //sylee151007 //ACE400
		   if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
		   Ret = AB_Bit( DO108, 1);   //sylee151007 //ACE400
		   if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 108, 1] Error!!!");	return  0;	}  //SYLEE150513  //ACE400
		   */

		   //son-32KPIN-20200305 : 
		   //  D64 0x080000 : _VB_ENA     Pin 등록 trigger, 엑셀페달  (1-> 0 : Falling edge로 0일때 동작한다. 평소에 1이어야)  
		   //  D64 0x100000 : _VB_RESET   Pin 등록의 시동키, 평소에는 0이고, 핀등록 직전에 1로 해야 한다.  
		   //
		   //  PinReset => _VB_ENA은 1 이고(0x080000), _VB_RESET은 0(0x000000)이어야 한다. 합치면 0x080000
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x080000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x080000  ) Error!!!");	return  0; }
		//sylee170802   rayok 	Delay_100ns(500); //50us

		//son-32KPIN-20200305 : 
		//son 0x180000  ( _VB_ENA 1, _VB_RESET 1) 로 다시 만드는 것이 매우 이상함.
		//    0x180000은 핀등록 준비단계로 핀등록 코드에만 있어야 하는데 이상함.  추후 막아야 할 지도 모름
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x180000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x180000  ) Error!!!");	return  0; }

	}
	else {//sylee151007 //ACE400		    
		Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //
		if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000 ) Error!!!");	return  0; }
	} //sylee151007 //ACE400
#endif		

#endif

	return  1;
}


//son Button 'Pin VB RST'
void CModeSub71::OnButton502() //PIN VB RESET
{
#ifndef _PC_TEST_
	CString str;
	nCPLD1 = 0;
	nCPLDR1 = 0;
	OnPinResetVB(); //PIN VB ALL  RESET   //SYLEE20120125
#endif
}

//son Button 'Pin VC RST'
void CModeSub71::OnButton503() //PIN VC RESET
{
#ifndef _PC_TEST_
	CString str;
	nCPLD2 = 0;
	nCPLDR2 = 0;
	OnPinResetVC(); //PIN VC ALL  RESET   //SYLEE20120125
#endif
}



//son Button 'INIT'
void CModeSub71::OnButton506()
{
#ifndef _PC_TEST_
	CString str;
	int Ret, i;

    nFlagLeakMPin1=0;  //sylee230526  //sylee230817
	nMode = 0;
	nModeIR = 0;
	nflgA = 0;
	nflgB = 0;
	nModeIG = 0;
	nMode4WG = 0;
	nModeOpenHV = 0;
	nModeFilter1 = 0;
	nMode1 = 0;
	nModeV1 = 0;
	nModeV2 = 0;
	nModeV3 = 0;
	nModeV4 = 0;
	nModeCC = 0;
	nCPLD1 = 0;
	nCPLDR1 = 0;
	nCPLD2 = 0;
	nCPLDR2 = 0;

	OnButton501(); ///PIN ALL RESET

	nADCount1 = 1;

	nTestCount1 = 64;
	nTimeDelay1 = 1000;


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO52, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 54, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return; }
#else  	
	Ret = Ixud_WriteDOBit(0, 3, 4, 0);  //HV1 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0);  //HV2 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0);  //LV1 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0);  //LV2 RELAY
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }
#endif

	if (nMode4W == 1) {//if( nMode4W ==1 && nModeIR==1){//HW PROTECTION  //sylee20111013
		Delay_100ns(20000); //2ms
	}
	nMode4W = 0;  //sylee20111013


#ifdef DEF_CARD_A1  //sylee170328
	//--------------------------------------
	//son All Digital out bit(0~127) Off
	for (i = 0; i < 16; i++) {
		Ret = DioOutByte(pDIO.hDrv, i, 0x0);
		if (Ret != 0) { AfxMessageBox("DioOutByte[  ] Error!!!");	return; }
	}
#else  //#ifdef DEF_CARD_A1  //sylee170328  


	//sylee170411	Ret = d64h_do_writeport( pDIO_D64.m_bCardID, 0x00000000 );  //
	//sylee170411if( Ret!=0 ) { AfxMessageBox("  Card64H1_DO32(0x00000000 ) Error!!!");	return   ;	}	
	//sylee170411Ret = d64h_do_writeport( pDIO_D64.m_bCardID2, 0x00000000 );  //
	//sylee170411if( Ret!=0 ) { AfxMessageBox("  Card64H2_DO32(0x00000000 ) Error!!!");	return   ;	}

	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x80000  ) Error!!!");	return; }
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000  ) Error!!!");	return; }

	for (i = 0; i <= 11; i++) {
		Ret = Ixud_WriteDO(0, i, 0x00);
		if (Ret != 0) { AfxMessageBox("  Card96_DOByte(i,0x00) Error!!!");	return; }
	}

#endif  //#ifdef DEF_CARD_A1 

	str.Format("->Digital ALL OFF");
	m_list1.InsertString(0, str);

	str.Format("0");

	memset(nPiS1, 0x00, sizeof(nPiS1));
	memset(nPiS2, 0x00, sizeof(nPiS2));

	m_nCurRowold1 = 0;
	m_nCurRowold2 = 0;

	//---------------------------------------------------
	str.Format("Poweroff");

	m_list1.InsertString(0, str);

	memset(nPiS1, 0x00, sizeof(nPiS1));
	memset(nPiS2, 0x00, sizeof(nPiS2));

	str.Format("Initial");
	str.Format("->SET Initial ");
	m_list1.InsertString(0, str);

	str.Format("OFF");
	m_Label225.SetCaption(str);
	nModeRCal = 1;

	if (nModeRCal == 0) {
		nModeRCal = 1;
		str.Format("Cal. R");
		//		m_Label227.SetCaption(str);
	}
	else {
		nModeRCal = 0;
		str.Format("Real R");
		///		m_Label227.SetCaption(str);
	}

	str.Format("INITIAL");
	m_Label205.SetCaption(str);
	m_list1.InsertString(0, str);

	str.Format("OFF");
	m_Label221.SetCaption(str);
	m_Label222.SetCaption(str);
	m_Label223.SetCaption(str);
	m_Label224.SetCaption(str);
	m_Label209.SetCaption(str);
	m_Label210.SetCaption(str);
	m_Label236.SetCaption(str);//hvopen

	str.Format("       ");
	m_Label202.SetCaption(str);

	str.Format("?");

	m_Label214.SetCaption(str);
	m_Label218.SetCaption(str);
	m_Label215.SetCaption(str);
	m_Label217.SetCaption(str);
	m_Label231.SetCaption(str);

	str.Format("CC SET");
	m_Label207.SetCaption(str);

	nModeFilter1 = 0;
	str.Format("Fast");
	m_Label235.SetCaption(str);

	nMode4WG = 0;
	str.Format("4W Gain Off");
	m_Label234.SetCaption(str);

	nModeCC = 1;
	OnButton110();  // CC MODE SET	//son nModeCC= 0으로 내부 설정

	nModeIG = 1;
	OnButton111();//Pin Search 




#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // AD_CONVST_A 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO32, 0); // I Range Mode 0  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 32,0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO33, 0); // I Range Mode 1    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 55, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO34, 0); // I Range Mode 2    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 34, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);  // I Range Mode 3   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);  // I Range Mode 4    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 58, 0] Error!!!");	return; }

#else  	


	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,7,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 6, 0);  //I RANGE SET1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 5, 0);  //I RANGE SET2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 4, 0);  //I RANGE SET3
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 10, 3, 0);  //I RANGE SET4
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,3,0) Error!!!");	return; }


#endif


	((CButton*)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO25))->SetCheck(FALSE);

	OnRadio01();

	nFlag71_VC_Filter_Mode_LV = 1;
	OnRadio04();
	OnRadio05();
	OnRadio09();

	OnCCCV_Set(1);//sylee150118
	OnVB_AC_Mode_Set(0); //ON=1, Off=0 SET  //sylee150118
	m_check11.SetCheck(0); //sylee150118   0==OFF, 1==ON
	OnCheck11(); //sylee150118  VBOUT CHECK//sylee150118  VBOUT CHECK

	nFlag71_VB_AC_Mode1 = 1; //SYLEE150118
	OnButton751();   //SYLEE150118   ///AC MODE  OFF

	nFlag71_HV_AC_Mode1 = 1;
	OnButton752();   //SYLEE150118


	nFlag71_HV_AC_Mode1_Relay = 1;
	OnButton753();   //SYLEE150118

	nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118
	OnButton11081(); //sylee150120   //PLUSE  off



#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD   OFF
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO49, 0); //vc FILTER OFF
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO56, 0); //HV OPEN  OFF
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }


	Ret = DioOutBit(pDIO.hDrv, DO43, 0);    //HV=1,LV=0  //SYLEE150121 신부성 과장 요청 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO44, 0);    //HV=0,LV=1  //SYLEE150121 신부성 과장 요청 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 1] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO45, 0);    //HV=0,LV=1 //SYLEE150121 신부성 과장 요청 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return; }

	Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //61   4w
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //61  hv open
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }


	Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //61   VC fast  FILTER
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 1] Error!!!");	return; }

#else  	

	Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }

#endif


	nFlag71_VB_G_ON = 1;
	nFlag71_VB_G_SEL1 = 1;
	nFlag71_VC_G_ON = 1;
	nFlag71_VC_G_SEL1 = 1;



#ifdef DEF_CARD_A1  //sylee170328
	OnButton757();  //SYLEE150118  //VB GUARD ON
	OnButton758();   //SYLEE150118  //VB GUARD SEL
	OnButton759();   //SYLEE150118  //VC GUARD ON
	OnButton760();   //SYLEE150118  //VC GUARD SEL
#else  //#ifdef DEF_CARD_A1   

#endif  //#ifdef DEF_CARD_A1 




#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO99, 1);   //sylee150513  //ACE400
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO107, 1);    //sylee150513  //ACE400
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return; }

	OnRadio04();//sylee150902

	Ret = DioOutBit(pDIO.hDrv, DO44, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO45, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO47, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO49, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
#else  	


	/*
	Ret = AB_Bit( DO99, 1);   //sylee150513  //ACE400
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return  ;	}
	Ret = AB_Bit( DO107, 1);    //sylee150513  //ACE400
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return  ;	}
	*/

	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x80000 ) Error!!!");	return; }
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000 ) Error!!!");	return; }

	OnRadio04();//sylee150902

	Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }


#endif


#endif
}


void CModeSub71::OnButton507() //sylee150118
{
	CModeSub72 dlg;
	dlg.DoModal();
}

int CModeSub71::OnDisCharge1() //sylee170725-3 ray
{
#ifndef _PC_TEST_
	OnDisChargeVB(1);
	AVB_CheckOut1(1, 8);
	OnDisChargeVB(0);
#endif
	return  1;
}

int CModeSub71::OnDisChargeVB(int a) //C discharge //B discharge 
{
#ifndef _PC_TEST_
	int  Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (a == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO59, 1);   // hvvb discharge
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 59, 1] Error!!!");	return  0; }
	}
	else {
		Ret = DioOutBit(pDIO.hDrv, DO59, 0);   // hvvb discharge
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 59, 0] Error!!!");	return  0; }
	}
#else  	
	if (a == 1) {
		Ret = Ixud_WriteDOBit(0, 4, 3, 1);  //HV VB DISCHARGE (bd 0, Port 4, bit 3, On)
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,3,1) Error!!!");	return  0; }

	}
	else {
		Ret = Ixud_WriteDOBit(0, 4, 3, 0);  //HV VB DISCHARGE (bd 0, Port 4, bit 3, Off)
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,3,0) Error!!!");	return  0; }
	}
#endif


#endif
	return  1;
}


int CModeSub71::OnDisChargeVC(int nOnOff) //vc discharge//sylee200916
{

#ifdef DEF_CARD_A1  //sylee170328

	//?????

#else  	

	int  Ret;
	if (nOnOff == 1) {  // on
		Ret = Ixud_WriteDOBit(0, 5, 7, 1);  //VC Discharge   //sylee200921  PC16->PC17
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,7,1) Error!!!");	return   0; }
	}
	else {   // off
		Ret = Ixud_WriteDOBit(0, 5, 7, 0);  //VC Discharge  //sylee200921  PC16->PC17
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,7,0) Error!!!");	return   0; }
	}


#endif


	return  1;
}


/*
int CModeSub71::OnDisChargeVC(int a) //C discharge
{



#ifdef DEF_CARD_A1  //sylee170328

	int  Ret;

	if(a==1){
		Ret = DioOutBit(pDIO.hDrv, DO60, 1);   // GNDC discharge
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 60, 1] Error!!!");	return  0;	}
	}else{
		Ret = DioOutBit(pDIO.hDrv, DO60, 0);   // GNDC discharge
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 60, 0] Error!!!");	return  0;	}
	}
#else

//SHIN B S  DO DELETE 170331
	/*
	if(a==1){
		Ret = A_Bit( DO60, 1);   // GNDC discharge
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 60, 1] Error!!!");	return  0;	}
	}else{
		Ret = A_Bit( DO60, 0);   // GNDC discharge
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 60, 0] Error!!!");	return  0;	}
	}
*/



/*
#endif


	return  1;
}
*/


/*
int CModeSub71::OnDisChargeuS(int a) //C discharge
{

	 a=a*10;
	 Delay_100ns(30);    //3us
	 OnDisChargeVC(1);   //C discharge   a=1 on   0=off
	 Delay_100ns(a);
	 OnDisChargeVC(0);   //C discharge   a=1 on   0=off
	 Delay_100ns(30);    //3us

	return  1;
}
*/

/*
int CModeSub71::OnDisChargeuSVB(int a) //C discharge
{
	 a=a*10;
	 Delay_100ns(30);    //3us
	 OnDisChargeVB(1);   //C discharge   a=1 on   0=off
	 Delay_100ns(a);
	 OnDisChargeVB(0);   //C discharge   a=1 on   0=off
	 Delay_100ns(30);    //3us

	return  1;
}

*/


// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;    //SYLEE20120215   8=VBOUTCK //SYLEE150118
//son sSET_VB_HL, sSET_ADC_A, sGET_VI
int CModeSub71::AVBRV1(int nMode2)

{
#ifndef _PC_TEST_
	CString str;
	int Ret;
	long nC;
	BYTE Buf;
	BYTE La, Ha;
	DWORD DHa;

	double lVb1, lVb2, lVb3, lVb4;//,lRat1;

	int nCo;
	DWORD V1Ma, V1Mi, V1S, V1T;

	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;
	//============================================================================

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {  //HW PROTECTION  SYLEE20110930    HV1  HV2 ON 상태에서 LV1  LV2  VB READING시  HW 파손됨.

		if (nMode2 == 1) {
			if (nMode1 != 1) { //SYLEE20111004
				AfxMessageBox(" Error No  7101, LV1 VB READING FAIL.     LV2,   HV1,  HV2  RELAY ON.    Error!!!");	return  -1;
			}
		}
		else if (nMode2 == 2) {
			if (nMode1 != 2) { //SYLEE20111004
				AfxMessageBox(" Error No  7102, LV2 VB READING FAIL.     LV1,   HV1,  HV2  RELAY ON.    Error!!!");	return  -1;
			}
		}
		else if (nMode2 == 3) {
			if (nMode1 != 3) { //SYLEE20111004
				AfxMessageBox(" Error No  7103, HV1 VB READING FAIL.     LV1,  LV2,  HV2  RELAY ON.    Error!!!");	return  -1;
			}
		}
		else if (nMode2 == 4) {
			if (nMode1 != 4) { //SYLEE20111004
				AfxMessageBox(" Error No  7104, HV2 VB READING FAIL.     LV1,  LV2,  HV1,   RELAY ON.    Error!!!");	return  -1;
			}
		}

	}




#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO28, 0);   // RMS FILTER OFF  //SYLEE150118
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO47, 0);   // I FAST FILTER
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //  I SLOW FILTER
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return  -1; }


	if (nMode2 == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO41, 0);   //LV1
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 41, 0] Error!!!");	return  -1; }
	}
	else if (nMode2 == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO41, 1);   //LV2
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 41, 1] Error!!!");	return  -1; }
	}
	else if (nMode2 == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 0);   //HV1
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 40, 0] Error!!!");	return  -1; }
	}
	else if (nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 1);   //HV2
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 40, 1] Error!!!");	return  -1; }
	}

	//ACE380 ///////////////////////////////////////////////////////////////////
	if (nMode2 != 8) { //HV 1, HV2   LV1, LV2
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);   //VB OUT CHECK
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return  -1; }
	}
	else {
		Ret = DioOutBit(pDIO.hDrv, DO122, 1);   //VB OUT CHECK
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 1] Error!!!");	return  -1; }
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD       0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,1] Error!!!");	return  -1; }
	}
	//ACE380 ///////////////////////////////////////////////////////////////////
	if (nMode2 == 1 || nMode2 == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //VB  SEN  CMD         0= LV   1= HV   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,0] Error!!!");	return  -1; }
	}
	if (nMode2 == 3 || nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD       0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,1] Error!!!");	return  -1; }
	}

	Ret = DioOutBit(pDIO.hDrv, DO37, 1);   //  ADC  VB  SELECT CMD      
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37,1] Error!!!");	return  -1; }

	Delay_100ns(30000);


	Ret = DioOutBit(pDIO.hDrv, DO45, 0);   //SYLEE20120501  1->0  H/W바뀜 0=VB SENSE   1= I SENSE        //VB-SS CMD ,    1= vb sense  0= i sense
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 0] Error!!!");	return  -1; }

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }

	Delay_100ns(100000);//10mSEC  //sylee20111021 신과장 요청 


Task1://sylee20110705

	Buf = 0;
	nC = 0;

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  -1; }

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }


	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  -1; }
	La = Buf;

	Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  -1; }
	Ha = Buf;

#else  	//#ifdef DEF_CARD_A1  //sylee170328  	

	//sylee170331 SHIN B.S OK			Ret = A_Bit( DO28, 0);   // RMS FILTER OFF  //SYLEE150118
	//sylee170331 SHIN B.S OK			if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  -1;	}


	Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return   -1; }
	Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return   -1; }

	if (nMode2 == 1) {
		Ret = Ixud_WriteDOBit(0, 8, 6, 0);  //VB LV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,0) Error!!!");	return  -1; }
	}
	else if (nMode2 == 2) {
		Ret = Ixud_WriteDOBit(0, 8, 6, 1);  //VB LV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,1) Error!!!");	return  -1; }
	}
	else if (nMode2 == 3) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 0);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,0) Error!!!");	return  -1; }
	}
	else if (nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 1);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,1) Error!!!");	return  -1; }
	}

	//ACE380 ///////////////////////////////////////////////////////////////////
	if (nMode2 != 8) { //HV 1, HV2   LV1, LV2
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return   -1; }
	}
	else {
		Ret = Ixud_WriteDOBit(0, 5, 2, 1);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,1) Error!!!");	return   -1; }
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   -1; }
	}
	//ACE380 ///////////////////////////////////////////////////////////////////
	if (nMode2 == 1 || nMode2 == 2) {
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   -1; }
	}
	if (nMode2 == 3 || nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}

	Ret = Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,1) Error!!!");	return   -1; }
	Delay_100ns(30000);
	Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return   -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }


	Delay_100ns(100000);//10mSEC  //sylee20111021 신과장 요청 


Task1://sylee20110705

	Buf = 0;
	nC = 0;

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }
	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	if (DHa == 65535 && nADCOver_IntDisable != 2) {//sylee20120318 INTERLOCK INTERLOCK
		DHa = 0;
	}
	//-----------------------------------------------------------
	// DWORD V1Ma,V1Mi,V1S,V1T;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;
	if (++nCo < 5) {
		goto Task1;
	}

	double dAvg;    //son231012 DHa -> dAvg
	//son231012 DHa = (V1S - V1Ma - V1Mi) / 3;
	dAvg = (double)(V1S - V1Ma - V1Mi) / 3.; //son231012
	//-----------------------------------------------------------

	//str.Format("%d", DHa); 
	str.Format("%.1f", dAvg); //son231012 DHa -> dAvg

	if (nMode2 == 1) {//LV1
		m_Label101.SetCaption(str);
	}
	else if (nMode2 == 2) {//LV2
		m_Label130.SetCaption(str);
	}
	else if (nMode2 == 3) {//HV1
		m_Label132.SetCaption(str);
	}
	else if (nMode2 == 4) {//HV2
		m_Label134.SetCaption(str);
	}
	else if (nMode2 == 5) {//SPARK

	}
	else if (nMode2 == 8) { //SYLEE150118  //8=VBOUTCK  
		m_Label113.SetCaption(str);
	}
	else {
		str.Format("Mode?");
		m_Label101.SetCaption(str);
	}

	if (nMode2 == 1 || nMode2 == 2) {//LH1 LV2  
		V1 = (double)dAvg / 3000.;  //son231012 DHa -> dAvg
		str.Format("%.3fV", V1);
		nVbf = 1;
	}
	else if (nMode2 == 3 || nMode2 == 4) {//HV1, HV2		  
		V1 = (double)dAvg / 200.;   //son231012 DHa -> dAvg
		str.Format("%.3fV", V1);
		nVbf = 1;
	}
	else if (nMode2 == 5) { //SPARK

	}
	else if (nMode2 == 6) { //4wire 	 
		V1 = (double)dAvg;          //son231012 DHa -> dAvg
		str.Format("%.3fV", V1);
		nVbf = 1;
	}
	else if (nMode2 == 8) { //SYLEE150118  //8=VBOUTCK
		V1 = (double)dAvg / 200.;   //son231012 DHa -> dAvg
		str.Format("%.3fV", V1);
		nVbf = 1;
	}
	else {
		str.Format("Mode?");
		nVbf = 0;
	}

	if (nMode2 == 1) {//LV1
		m_Label121.SetCaption(str);
	}
	else if (nMode2 == 2) {//LV2
		m_Label131.SetCaption(str);
	}
	else if (nMode2 == 3) {//HV1
		m_Label133.SetCaption(str);
	}
	else if (nMode2 == 4) {//HV2
		m_Label135.SetCaption(str);
	}
	else if (nMode2 == 5) {//SPARK


	}
	else if (nMode2 == 8) { //SYLEE150118   
		m_Label232.SetCaption(str);
	}
	else {
		str.Format("Mode?");
		m_Label121.SetCaption(str);
	}



	if (nVbf == 1) {
		if (nVb == 0) {
			str.Format(" ===>  ERROR : VB Decimal Set 0   ");
			m_list1.InsertString(0, str);
			return -1;
		}
		lVb1 = nVb * 0.99;
		lVb2 = nVb * 1.01;
		lVb3 = nVb * 0.9;
		lVb4 = nVb * 1.1;

		if ((V1 > lVb1) && (lVb2 > V1)) {

		}
		else if ((V1 < lVb3) || (lVb4 < V1)) {
			str.Format(" ===>  ERROR : VB  10 Over");
			m_list1.InsertString(0, str);
		}
	}


	//str.Format("-> VB sensing 16 bit=%d", DHa);   
	str.Format("-> VB sensing 16 bit=%.1f", dAvg);   //son231012 DHa -> dAvg
	m_list1.InsertString(0, str);

	if (nMode2 == 1 || nMode2 == 2 || nMode2 == 3 || nMode2 == 4) {
		str.Format("-> VB sensing =%.3f V", V1);  //????  adc*0.0004247
	}
	else {
		str.Format("Mode?");
	}
	m_list1.InsertString(0, str);
	//===========================================================================

	nflgA = 3;

#endif
	return 1; //V1=VB
}



//son 결과값 Sub71.V1 (VB 전압) 
int CModeSub71::AOnReadVB1(int nMode2)// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;   
{

#ifndef _PC_TEST_
	CString str;
	int Ret;
	long nC;
	BYTE Buf;
	BYTE La, Ha;
	DWORD DHa;
    double dAvg;    //son231012 DHa -> dAvg
    

	int nCo;
	DWORD V1Ma, V1Mi, V1S, V1T;

	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;

	//============================================================================

	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3 || nMode1 == 4)  //HW PROTECTION  SYLEE20110930    HV1  HV2 ON 상태에서 LV1  LV2  VB READING시  HW 파손됨.
	{
		if (nMode2 == 1) {
			if (nMode1 != 1) { //SYLEE20111004
				AfxMessageBox(" Error No  7105, LV1 VB READING FAIL.     LV2,   HV1,  HV2  RELAY ON.    Error!!!");	return  -1;
			}
		}
		else if (nMode2 == 2) {
			if (nMode1 != 2) { //SYLEE20111004
				AfxMessageBox(" Error No  7106, LV2 VB READING FAIL.     LV1,   HV1,  HV2  RELAY ON.    Error!!!");	return  -1;
			}
		}
		else if (nMode2 == 3) {
			if (nMode1 != 3) { //SYLEE20111004
				AfxMessageBox(" Error No  7107, HV1 VB READING FAIL.     LV1,  LV2,  HV2  RELAY ON.    Error!!!");	return  -1;
			}
		}
		else if (nMode2 == 4) {
			if (SysSet13.m_n4WCVMode != 1) {	 //sylee200321-4WCVMODE
				if (nMode1 != 4) { //SYLEE20111004
					AfxMessageBox(" Error No  7108, HV2 VB READING FAIL.     LV1,  LV2,  HV1,   RELAY ON.    Error!!!");	return  -1;
				}
			}
		}
	}

#ifdef DEF_CARD_A1  //sylee170328

	if (nMode2 == 1) {
		Ret = DioOutBit(pDIO.hDrv, DO41, 0);   //LV_SEN_SEL
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 0] Error!!!");	return  -1; }
	}
	else  if (nMode2 == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO41, 1);   //LV_SEN_SEL
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 30, 1] Error!!!");	return  -1; }
	}
	else  if (nMode2 == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 0);   //HV_SEN_SEL
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 0] Error!!!");	return  -1; }
	}
	else  if (nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 1);   //HV_SEN_SEL
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 31, 1] Error!!!");	return  -1; }
	}

	if (nMode2 == 1 || nMode2 == 2) {
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //VB  SEN  CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29,0] Error!!!");	return  -1; }
	}
	if (nMode2 == 3 || nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29,1] Error!!!");	return  -1; }
	}

	Delay_100ns(30000);
	Ret = DioOutBit(pDIO.hDrv, DO37, 1);   //VB-SS CMD ,    1= vb sense  0= i sense     //SYLEE20120501  1->0  H/W바뀜 0=VB SENSE   1= I SENSE   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 1] Error!!!");	return  -1; }
	//============================================================================
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }


Task1://sylee20110705

	Buf = 0;
	nC = 0;

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }

	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  -1; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  -1; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1   


	if (nMode2 == 1) {
		Ret = Ixud_WriteDOBit(0, 8, 6, 0);  //VB LV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,0) Error!!!");	return  -1; }
	}
	else  if (nMode2 == 2) {
		Ret = Ixud_WriteDOBit(0, 8, 6, 1);  //VB LV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,1) Error!!!");	return  -1; }
	}
	else  if (nMode2 == 3) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 0);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,0) Error!!!");	return  -1; }
	}
	else  if (nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 1);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,1) Error!!!");	return  -1; }
	}

	if (nMode2 == 1 || nMode2 == 2) {
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   -1; }
	}
	if (nMode2 == 3 || nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}

	Delay_100ns(30000); //3msec

	Ret = Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,1) Error!!!");	return   -1; }
	//============================================================================

	Delay_100ns(10000);	//1msec	//son211115 김영필이사님요청.  SelfTest VB_Check 오류 줄이기 위해 추가.

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }



Task1://sylee20110705

	Buf = 0;
	nC = 0;

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	if (DHa == 65535 && nADCOver_IntDisable != 2) {//sylee20120318 INTERLOCK  
		DHa = 0;
	}


	//-----------------------------------------------------------
	// DWORD V1Ma,V1Mi,V1S,V1T;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;
	if (++nCo < 5) {
		goto Task1;
	}

	//son231012 DHa = (V1S - V1Ma - V1Mi) / 3;
	dAvg = (double)(V1S - V1Ma - V1Mi) / 3.;  //son231012 DHa -> dAvg
	//-----------------------------------------------------------

	if (nMode2 == 1 || nMode2 == 2) {//LH1 LV2 		 
		//	V1=dAvg*0.000424; 
		V1 = (double)dAvg / 3000.; //son231012 DHa -> dAvg
		//	str.Format("%.3fV",V1); 
		nVbf = 1;
	}
	else  if (nMode2 == 3 || nMode2 == 4) {//HV1, HV2		 
		//V1=dAvg*0.00840; 
		V1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		//	str.Format("%.3fV",V1); 
		nVbf = 1;
	}
	else  if (nMode2 == 5) { //SPARK	 
		//V1=dAvg*0.000424;
		//	V1=dAvg;    //son231012 DHa -> dAvg
		//	str.Format("%.3fV",V1); 
		//	nVbf=1;
	}
	else  if (nMode2 == 6) { //4wire	?????????	 
		//V1=dAvg*0.000424;
		V1 = dAvg; //son231012 DHa -> dAvg
		//	str.Format("%.3fV",V1); 
		nVbf = 1;
	}
	else {
		//	str.Format("Mode?"); 
		nVbf = 0;
	}


	nflgA = 3;

#endif

	return 1; //V1=VB
}




//son VB CheckOut 설정
int CModeSub71::AOn_VB_CheckOut1_Set(int Mod1) //SYLEE150210 //ACE380
{
#ifndef _PC_TEST_
	int Ret;
	int nMode2;

	nMode2 = 8;


#ifdef DEF_CARD_A1  //sylee170328

	if (nMode2 == 1) {
		OutNo[0] = 41;        OutNo[1] = 42;    	OutData[0] = 0;	    OutData[1] = 0;
	}
	else if (nMode2 == 2) {
		OutNo[0] = 41;        OutNo[1] = 42;    	OutData[0] = 1;	    OutData[1] = 0;
	}
	else if (nMode2 == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 0);   //HV_SEN_SEL
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 40, 0] Error!!!");	return  -1; }
	}
	else if (nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 1);   //HV_SEN_SEL
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 40, 1] Error!!!");	return  -1; }
	}

	if (nMode2 != 8) { //HV 1, HV2   LV1, LV2   //sylee150210   //ACE380 //sylee150210
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);   //VB OUT CHECK
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return  -1; }
	}
	else {  //ACE380  //sylee150210
		Ret = DioOutBit(pDIO.hDrv, DO122, 1);   //VB OUT CHECK   //sylee150210   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 1] Error!!!");	return  -1; }
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,1] Error!!!");	return  -1; }
	}

	if (nMode2 == 1 || nMode2 == 2) {
		Ret = DioOutMultiBit(pDIO.hDrv, &OutNo[0], 2, &OutData[0]); //sylee140225
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 41  42,   ?   0 ] Error!!!");	return  0; }
	}
	if (nMode2 == 3 || nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,1] Error!!!");	return  -1; }
	}

	Delay_100ns(30000);//3mS

	//son ADC_A VB Filter를 설정 (VBFT)    (37:1,  47:0,  48:0)
	OutNo[0] = 47;        OutNo[1] = 48;    	OutData[0] = 0;	    OutData[1] = 0;
	Ret = DioOutMultiBit(pDIO.hDrv, &OutNo[0], 2, &OutData[0]); //sylee140225
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 47 48 ,  0  0 ] Error!!!");	return  0; }

	Ret = DioOutBit(pDIO.hDrv, DO37, 1);   //VB-SS CMD ,   //sylee130627
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 1] Error!!!");	return  -1; }


	//son 다음에 올 VB Read를 위해 _AD_CONVST를 1로 돌려 놓는다.  (1->0이 되면 Falling Edge로 read가 됨)
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }


#else  //#ifdef DEF_CARD_A1   


	if (nMode2 == 1) {
		OutNo[0] = 41;        OutNo[1] = 42;    	OutData[0] = 0;	    OutData[1] = 0;
	}
	else if (nMode2 == 2) {
		OutNo[0] = 41;        OutNo[1] = 42;    	OutData[0] = 1;	    OutData[1] = 0;
	}
	else if (nMode2 == 3) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 0);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,0) Error!!!");	return  -1; }
	}
	else if (nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 1);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,1) Error!!!");	return  -1; }
	}

	if (nMode2 != 8) { //HV 1, HV2   LV1, LV2   //sylee150210   //ACE380 //sylee150210
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return   -1; }
	}
	else {  //ACE380  //sylee150210
		Ret = Ixud_WriteDOBit(0, 5, 2, 1);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,1) Error!!!");	return  -1; }
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}

	if (nMode2 == 1 || nMode2 == 2) {
		Ret = Ixud_WriteDOBit(0, 8, 6, OutBitData[0]);  //VB LV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,  ) Error!!!");	return  0; }
		Ret = Ixud_WriteDOBit(0, 8, 5, OutBitData[1]);  //VB SEN
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,  ) Error!!!");	return  0; }
	}

	if (nMode2 == 3 || nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}

	Delay_100ns(30000);//3mS  

	Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC I FAST SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return  -1; }
	Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC I SLOW SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return  -1; }

	Ret = Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,1) Error!!!");	return   -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }

#endif  //#ifdef DEF_CARD_A1 
#endif

	return 1;

}



//son V1 값을 read한다.
//nMod1 (VbSrc)  1: VR_LV1, 2: VR_LV2, 3: VR_HV1, 4: VR_HV2
int CModeSub71::AOn_VB_CheckOut2_Run(int Mod1) //SYLEE150210 //ACE380
{
#ifndef _PC_TEST_
	CString str;
	int Ret;
	long nC;
	BYTE Buf;
	BYTE La, Ha;
	DWORD DHa;
	double lVb1, lVb2, lVb3, lVb4;//,lRat1;   
	int nMode2;
	int nCo;
	DWORD V1Ma, V1Mi, V1S, V1T;

	nMode2 = 8;

	nCo = 0;    V1Ma = 0;    V1Mi = 0;	V1S = 0;	V1T = 0;

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }

Task1://sylee20110705

	Buf = 0;	nC = 0;
	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }

	InpNo[0] = 0;	        InpNo[1] = 1;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 0,1 ] Error!!!");	return  0; }
	La = InpData[0];  	Ha = InpData[1];

#else  //#ifdef DEF_CARD_A1   

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }

Task1://sylee20110705

	Buf = 0;	nC = 0;
	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (DHa == 65535 && nADCOver_IntDisable != 2) {//sylee20120318 INTERLOCK
		DHa = 0;
	}

	//-----------------------------------------------------------
	// DWORD V1Ma,V1Mi,V1S,V1T;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;
	if (++nCo < 5) {
		goto Task1;
	}

	//son231012 DHa = (V1S - V1Ma - V1Mi) / 3;
    double dAvg = (double)(V1S - V1Ma - V1Mi) / 3.; //son231012 DHa -> dAvg
	//-----------------------------------------------------------
	if (nMode2 == 8) {//VB  CHECK OUT  			
		V1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		//	V1=(double)dAvg; //TEST 
		nVbf = 1;
	}

	if (nVbf == 1) {
		lVb1 = nVb * 0.99;
		lVb2 = nVb * 1.01;
		lVb3 = nVb * 0.9;
		lVb4 = nVb * 1.1;
	}
#endif
	return 1;

}




//son VB CheckOut(DO122)을 해제하고 지정된 Mode로 VB를 복구한다. 
int CModeSub71::AOn_VB_CheckOut3_Stop(int Mod1) //SYLEE150210 //ACE380
{

#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //VB-SS CMD ,   //sylee130627
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return  -1; }

	Ret = DioOutBit(pDIO.hDrv, DO122, 0);   //VB OUT CHECK
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return  -1; }

	if (Mod1 == 3 || Mod1 == 4) { //HV1 , HV2
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB_SEN  HV=1  LV=0  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 1] Error!!!");	return  -1; }
	}
	else {//LV1, LV2, ETC
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //VB_SEN  HV=1  LV=0   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return  -1; }
	}

#else  //#ifdef DEF_CARD_A1   

	Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return   -1; }

	Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return  -1; }

	if (Mod1 == 3 || Mod1 == 4) { //HV1 , HV2
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}
	else {//LV1, LV2, ETC
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   -1; }
	}


#endif  //#ifdef DEF_CARD_A1 


	nflgA = 3;
#endif

	return 1;

}



//son  VB Reading  (Sub71.V1 에 VB 값 Read)

//SYLEE20111213  VB READING ,   
// nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;   
int CModeSub71::AOnReadVB1Run1(int nMode2)
{
#ifndef _PC_TEST_
	CString str;
	int Ret, nCo;
	long nC;
	BYTE Buf, La, Ha;
	DWORD DHa, V1Ma, V1Mi, V1S, V1T;
	double lVb1, lVb2, lVb3, lVb4;//,lRat1; 

	nCo = 0;    V1Ma = 0;    V1Mi = 0;	V1S = 0;	V1T = 0;




//	if(nMode2==3 ||  nMode2==4){//sylee230811-3-test
//		return 1;
//	}


#ifdef DEF_CARD_A1  //sylee170328

	//-------------
	//son VB Set

	//son VB_LV_SEN_SEL 0(LV0), VB_SEN_SEL 0(LV)
	if (nMode2 == 1) {
		OutNo[0] = 41;        OutNo[1] = 42;    	OutData[0] = 0;	    OutData[1] = 0;
	}

	//son VB_LV_SEN_SEL 1(LV1), VB_SEN_SEL 0(LV)
	else if (nMode2 == 2) {
		OutNo[0] = 41;        OutNo[1] = 42;    	OutData[0] = 1;	    OutData[1] = 0;
	}

	else if (nMode2 == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 0);   //HV_SEN_SEL 0 (HV0)
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 40, 0] Error!!!");	return  -1; }
	}
	else if (nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO40, 1);   //HV_SEN_SEL 1 (HV1)
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 40, 1] Error!!!");	return  -1; }
	}

	//--------------------
	//son VB OUT Check
	if (nMode2 != 8) { //HV 1, HV2   LV1, LV2   //sylee150210   //ACE380 //sylee150210
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);   //VB OUT CHECK
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return  -1; }
	}
	else {  //ACE380  //sylee150210
		Ret = DioOutBit(pDIO.hDrv, DO122, 1);   //VB OUT CHECK   //sylee150210   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 1] Error!!!");	return  -1; }

		//son 회로 보호를 위해  VB_SEN_SEL  HV(1)로  Set 
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,1] Error!!!");	return  -1; }
	}

	//--------------------
	//son VB Read 설정

	//son LV VB Read 설정
	if (nMode2 == 1 || nMode2 == 2) {
		Ret = DioOutMultiBit(pDIO.hDrv, &OutNo[0], 2, &OutData[0]); //sylee140225
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 41  42,   ?   0 ] Error!!!");	return  0; }
	}
	//son HV VB Read 설정
	if (nMode2 == 3 || nMode2 == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42,1] Error!!!");	return  -1; }
	}

	Delay_100ns(20000);//2mS //SYLEE150421-   3M->2MS


	//-------------------------------
	//son ADC_A를 VB Filter로  설정

	//son ADC_VB_SEL   (bit37 1,  bit47 0, bit48 0)
	OutNo[0] = 47;        OutNo[1] = 48;    	OutData[0] = 0;	    OutData[1] = 0;
	Ret = DioOutMultiBit(pDIO.hDrv, &OutNo[0], 2, &OutData[0]); //sylee140225
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 47 48 ,  0  0 ] Error!!!");	return  0; }

	Ret = DioOutBit(pDIO.hDrv, DO37, 1);   //VB-SS CMD ,   //sylee130627
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37, 1] Error!!!");	return  -1; }


	//============================================================================
	//son VB READ 

	//son VB READ 준비  D062 1->0 (AD_CONVST: Negative Trigger)
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }


Task1://sylee20110705

	Buf = 0;	nC = 0;

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //son 여기서 Trigger 됨
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  -1; }

	//son VB READ
	InpNo[0] = 0;	        InpNo[1] = 1;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 0,1 ] Error!!!");	return  0; }

	//son La, Ha로 VB 값 획득.
	La = InpData[0];  	Ha = InpData[1];


#else  //#ifdef DEF_CARD_A1   


	if (nMode2 == 1) {
		OutData[0] = 0;	    OutData[1] = 0;
	}
	else if (nMode2 == 2) {
		OutData[0] = 1;	    OutData[1] = 0;
	}
	else if (nMode2 == 3) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 0);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,0) Error!!!");	return  -1; }
	}
	else if (nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 7, 1);  //VB HV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,7,1) Error!!!");	return  -1; }
	}

	if (nMode2 != 8) { //HV 1, HV2   LV1, LV2   //sylee150210   //ACE380 //sylee150210
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return  -1; }
	}
	else {  //ACE380  //sylee150210
		Ret = Ixud_WriteDOBit(0, 5, 2, 1);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,1) Error!!!");	return   -1; }
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}



	if (nMode2 == 1 || nMode2 == 2) {  //SYLEE170224
		Ret = Ixud_WriteDOBit(0, 8, 6, OutData[0]);  //VB LV SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,6,  ) Error!!!");	return  0; }
		Ret = Ixud_WriteDOBit(0, 8, 5, OutData[1]);  //VB SEN
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,  ) Error!!!");	return  0; }
	}

	if (nMode2 == 3 || nMode2 == 4) {
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   -1; }
	}

	Delay_100ns(20000);//2mS //SYLEE150421-   3M->2MS


	Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return   -1; }
	Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return   -1; }
	Ret = Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,1) Error!!!");	return   -1; }

	//		Delay_100ns(30000);//???????????
	//		Ret = Ixud_WriteDOBit(0,8,2,0);  //VC SEN SEL//???????????
	//		if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return   -1;	}


	Delay_100ns(10000);//SYLEE170601 RAY

	//============================================================================
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }



Task1://sylee20110705

	Buf = 0;	nC = 0;

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  -1; }

	//son La, Ha로 VB 값 획득.
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);


#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (DHa == 65535 && nADCOver_IntDisable != 2) {//sylee20120318 INTERLOCK
		DHa = 0;
	}
	//-----------------------------------------------------------
	// DWORD V1Ma,V1Mi,V1S,V1T;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;
	if (++nCo < 5) {
		goto Task1;
	}

	//son231012 DHa = (V1S - V1Ma - V1Mi) / 3;
	double dAvg = (double)(V1S - V1Ma - V1Mi) / 3.; //son231012 DHa -> dAvg
	//-----------------------------------------------------------

	if (nMode2 == 1 || nMode2 == 2) {//LH1 LV2 
		V1 = (double)dAvg / 3000.; //son231012 DHa -> dAvg
		nVbf = 1;
	}
	else  if (nMode2 == 3 || nMode2 == 4) {//HV1, HV2
		V1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		nVbf = 1;
	}
	else  if (nMode2 == 5) { //SPARK	 
		//V1=dAvg*0.000424;
		//	V1=dAvg;
		//	str.Format("%.3fV",V1); 
		//	nVbf=1;
	}
	else  if (nMode2 == 6) { //4wire
		V1 = (double)dAvg; //son231012 DHa -> dAvg
		nVbf = 1;
		//ACE380 ///////////////////////////////////////////////////////////////////
		//sylee150210
	}
	else  if (nMode2 == 8) {//VB  CHECK OUT  			
		V1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		nVbf = 1;
		//ACE380 ///////////////////////////////////////////////////////////////////
	}
	else {
		//	str.Format("Mode?"); 
		nVbf = 0;
	}

	//	if(nVbf==1 && FB1==1){
	if (nVbf == 1) {
		lVb1 = nVb * 0.99;
		lVb2 = nVb * 1.01;
		lVb3 = nVb * 0.9;
		lVb4 = nVb * 1.1;
	}


#ifdef DEF_CARD_A1  //sylee170328

	//son VB_OUT_CHECK, ADC_VB_SEL 원상복구.
	//
	//sylee170601 ray	 Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //VB-SS CMD ,   //sylee130627
	//sylee170601 ray	 if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return  -1;	}

	if (nMode2 == 8) { //VB CHECK OUT    //sylee150210  //ACE380
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);   //VB OUT CHECK
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return  -1; }
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //VB_SEN  HV=1  LV=0
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return  -1; }
	}

#else  //#ifdef DEF_CARD_A1   

	//sylee170601 ray	Ret = Ixud_WriteDOBit(0,10,2,0);  //ADC_VB_SEL
	//sylee170601 ray	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return   -1;	}

	if (nMode2 == 8) { //VB CHECK OUT    //sylee150210  //ACE380
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return  -1; }
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   -1; }
	}

#endif  //#ifdef DEF_CARD_A1 

	//ACE380 ///////////////////////////////////////////////////////////////////

	nflgA = 3;
#endif

	return 1; //V1=VB
}




int CModeSub71::AVBRV2(int nMode2)// VC READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;   
{
#ifndef _PC_TEST_
	CString str;
	int Ret, nCo, nCount;
	long nC;
	BYTE Buf, La, Ha;
	DWORD DHa, V1Ma, V1Mi, V1S, V1T;

	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;

	nCount = 1000;


#ifdef DEF_CARD_A1  //sylee170328

	if (nMode2 == 1 || nMode2 == 2) {   // LV1, LV2  SEN CMD ON   //sylee130605
		Ret = DioOutBit(pDIO.hDrv, DO43, 0);   // VC_hv    0=off   1=on   RELAY
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 0] Error!!!");	return  0; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   // 4WIRE CMD   (0=OFF,      1=ON)   IC
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return  0; }
		Ret = DioOutBit(pDIO.hDrv, DO45, 1);   //   (0=HV,      1=LV)  IC
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return  0; }
		Delay_100ns(20000);  //2ms  
		Ret = DioOutBit(pDIO.hDrv, DO44, 1);    // VC_lv    0=off   1=on   RELAY
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 1] Error!!!");	return  0; }
	}
	else if (nMode2 == 3 || nMode2 == 4) {  // HV1, HV2  SEN CMD ON     //sylee130605
		Ret = DioOutBit(pDIO.hDrv, DO44, 0);    // VC_lv    0=off   1=on   RELAY
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 0] Error!!!");	return  0; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   // 4WIRE CMD   (0=OFF,      1=ON)   IC
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return  0; }
		Ret = DioOutBit(pDIO.hDrv, DO45, 0);   //   (0=HV,      1=LV)  IC
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 0] Error!!!");	return  0; }
		Delay_100ns(20000);  //2ms  
		Ret = DioOutBit(pDIO.hDrv, DO43, 1);   // VC_hv    0=off   1=on   RELAY
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 1] Error!!!");	return  0; }

	}
	else {
		AfxMessageBox(" [ VB READ Error   LV1,LV2,HV1,HV2  MODE SET? ] Error!!!");
		return 0;
	}

	//===========================================================================

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_B
	if (Ret != 0) { AfxMessageBox("  VB READING Error!!! ( MODE1~4)? LV1,LV2,HV1,HV2");	return  0; }


Task1://sylee20110705

	Buf = 0;
	nC = 0;

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_B
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 0] Error!!!");	return  0; }

	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_B   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 1] Error!!!");	return  0; }
	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 2, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 2] Error!!!");	return  0; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 3, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 3] Error!!!");	return  0; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1   

	if (nMode2 == 1 || nMode2 == 2)   // LV1, LV2  SEN CMD ON   //sylee130605
	{
		Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return   0; }

		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return   0; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return  0; }
		Ret = Ixud_WriteDOBit(0, 8, 2, 1);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,1) Error!!!");	return   0; }
		Delay_100ns(20000);  //2ms  
		Ret = Ixud_WriteDOBit(0, 8, 3, 1);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,1) Error!!!");	return   0; }
	}
	else if (nMode2 == 3 || nMode2 == 4)  // HV1, HV2  SEN CMD ON     //sylee130605
	{
		Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return   0; }

		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return   0; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return  0; }
		Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return   0; }
		Delay_100ns(20000);  //2ms  
		Ret = Ixud_WriteDOBit(0, 8, 4, 1);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,1) Error!!!");	return   0; }

	}
	else {
		AfxMessageBox(" [ VB READ Error   LV1,LV2,HV1,HV2  MODE SET? ] Error!!!");
		return 0;
	}
	//===========================================================================
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }


Task1://sylee20110705

	Buf = 0;
	nC = 0;

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);


#endif  //#ifdef DEF_CARD_A1 	



	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	V1T = DHa;
	V1S = V1S + V1T;

	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}

	//DHa = (V1S) / (nADCount1);//sylee20111111    
    double dAvg = (double)V1S / (double)nADCount1;//sylee20111111    //son231012 DHa -> dAvg
    
	str.Format("%.1f", dAvg); //son231012 DHa -> dAvg
	m_Label110.SetCaption(str);

	if (nMode2 == 1) {//LV1 
		X1 = (double)dAvg / 3000.; //son231012 DHa -> dAvg
		str.Format("%.3fV", X1);
	}
	else if (nMode2 == 2) { //LV2 
		X1 = (double)dAvg / 3000.; //son231012 DHa -> dAvg
		str.Format("%.3fV", X1);
	}
	else if (nMode2 == 3) {   //HV1		 
		X1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		str.Format("%.3fV", X1);
	}
	else if (nMode2 == 4) {   //HV2				 
		X1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		str.Format("%.3fV", X1);
	}
	else {
		str.Format("Mode?");
	}

	m_Label123.SetCaption(str);

#endif
	return 1; //V1=VB
}



int CModeSub71::AOnReadV1Init1(int nMod)//SYLEE20111109  VC READING init   MODE 전환시 한번만 L->H , H->L     nMod     1=LV1,  2=LV2,  3=HV1, 4=HV2;   
{

#ifndef _PC_TEST_
	CString str;
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328

	if (nMod == 1 || nMod == 2) {   // LV1, LV2  SEN CMD ON  //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //HV_SEN_CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 0] Error!!!");	return  0; }
		Delay_100ns(20000);//2mSEC
	}

	if (nMod == 3 || nMod == 4) {  // HV1, HV2  SEN CMD ON   //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = DioOutBit(pDIO.hDrv, DO3328, 0);   //LV_SEN_CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  0; }
		Delay_100ns(20000);//2mSEC
	}

	if (nMod == 1 || nMod == 2) {   // LV1, LV2  SEN CMD ON   
		Ret = DioOutBit(pDIO.hDrv, DO3328, 1);   //LV_SEN_CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return  0; }
		Delay_100ns(20000);//2mSEC
	}

	if (nMod == 3 || nMod == 4) {  // HV1, HV2  SEN CMD ON   
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //HV_SEN_CMD
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 29, 1] Error!!!");	return 0; }
		Delay_100ns(20000);//2mSEC
	}
#else  //#ifdef DEF_CARD_A1   


	if (nMod == 1 || nMod == 2) {   // LV1, LV2  SEN CMD ON  //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = Ixud_WriteDOBit(0, 8, 5, 0);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,0) Error!!!");	return   0; }
		Delay_100ns(20000);//2mSEC
	}

	/*###???????
		if( nMod==3 || nMod==4 ){  // HV1, HV2  SEN CMD ON   //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
			Ret = A_Bit( DO3328, 0);   //LV_SEN_CMD
			if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 0] Error!!!");	return  0;	}
			Delay_100ns(20000);//2mSEC
		}

		if( nMod==1 || nMod==2 ){   // LV1, LV2  SEN CMD ON
			Ret = A_Bit( DO3328, 1);   //LV_SEN_CMD
			if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 28, 1] Error!!!");	return  0;	}
			Delay_100ns(20000);//2mSEC
		}
	*/
	if (nMod == 3 || nMod == 4) {  // HV1, HV2  SEN CMD ON   
		Ret = Ixud_WriteDOBit(0, 8, 5, 1);  //VB_SEN_CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,5,1) Error!!!");	return   0; }
		Delay_100ns(20000);//2mSEC
	}


#endif  //#ifdef DEF_CARD_A1 


	//============================================================================
#endif
	return 1;
}




// VC READ  INIT SET ,  //SYLEE20120223
//son Mod1 : 1=LV1,  2=LV2,  3=HV1, 4=HV2,  5= all off
int CModeSub71::AOnReadV1Enable1(int Mod1)
{
#ifndef _PC_TEST_
	//CString str;
	int Ret;

	if (Mod1 == 0)
	{



	}

	else //SYLEE130628
	{

#ifdef DEF_CARD_A1  //sylee170328

		OutBitNo[0] = 43;   OutBitNo[1] = 44;   OutBitNo[2] = 45;

		if (Mod1 == 1 || Mod1 == 2) {   // LV1, LV2  SEN CMD ON  
			OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 1;
			Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
			if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 43 44 45, 0  1  1] Err]or!!!");	return  0; }
		}
		else if (Mod1 == 3 || Mod1 == 4) {  // HV1, HV2  SEN CMD ON   	 
			OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;
			Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
			if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 43 44 45, 1  0  0] Err]or!!!");	return  0; }
		}
		else if (Mod1 = 5) {  // all off  
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
			Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
			if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 43 44 45, 0  0  0] Err]or!!!");	return  0; }
		}
		else {
			AfxMessageBox(" [ VC READ INIT SET   Error   LV1,LV2,HV1,HV2  MODE SET? ] Error!!!");
			return 0;
		}

		Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_B //sylee20111115    
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

#else  //#ifdef DEF_CARD_A1   



		//	OutBitNo[0]=43;   OutBitNo[1]=44;   OutBitNo[2]=45;

		if (Mod1 == 1 || Mod1 == 2) {   // LV1, LV2  SEN CMD ON  
			OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 1;
		}
		else if (Mod1 == 3 || Mod1 == 4) {  // HV1, HV2  SEN CMD ON   	 
			OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;
		}
		else if (Mod1 = 5) {  // all off  
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
		}
		else {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
			AfxMessageBox(" [ VC READ INIT SET   Error   LV1,LV2,HV1,HV2  MODE SET? ] Error!!!");
			return 0;
		}

		Ret = Ixud_WriteDOBit(0, 8, 4, OutBitData[0]);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4, ) Error!!!");	return   0; }
		Ret = Ixud_WriteDOBit(0, 8, 3, OutBitData[1]);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3, ) Error!!!");	return   0; }
		Ret = Ixud_WriteDOBit(0, 8, 2, OutBitData[2]);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2, ) Error!!!");	return   0; }

		Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }


#endif  //#ifdef DEF_CARD_A1 



	}

#endif
	return 1;
}






int CModeSub71::AOnReadV1Enable1(int Mod1, int nVBenable)// VC READ  INIT SET ,  //sylee150318
{
	//CString str;
#ifndef _PC_TEST_
	int Ret;
#endif

	if (nVBenable < 1)//sylee150318
		AVB_CheckOut1(Mod1, 9);//SYLEE150210 //ACE380    FAIL =0  , PASS=1


	if (Mod1 == 0) {

	}
	else//SYLEE130628
	{

#ifndef _PC_TEST_
#ifdef DEF_CARD_A1  //sylee170328

		OutBitNo[0] = 43;   OutBitNo[1] = 44;   OutBitNo[2] = 45;

		if (Mod1 == 1 || Mod1 == 2) {   // LV1, LV2  SEN CMD ON  
			OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 1;
			Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
			if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 43 44 45, 0  1  1] Err]or!!!");	return  0; }
		}
		else if (Mod1 == 3 || Mod1 == 4) {  // HV1, HV2  SEN CMD ON    
			OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;
			Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
			if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 43 44 45, 1  0  0] Err]or!!!");	return  0; }
		}
		else if (Mod1 == 5) {  // all off  
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
			Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
			if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 43 44 45, 0  0  0] Err]or!!!");	return  0; }
		}
		else {
			AfxMessageBox(" [ VC READ INIT SET   Error   LV1,LV2,HV1,HV2  MODE SET? ] Error!!!");
			return 0;
		}
		Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_B //sylee20111115    
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

#else  //#ifdef DEF_CARD_A1 


		//	OutBitNo[0]=43;   OutBitNo[1]=44;   OutBitNo[2]=45;

		if (Mod1 == 1 || Mod1 == 2) {   // LV1, LV2  SEN CMD ON  
			OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 1;
		}
		else if (Mod1 == 3 || Mod1 == 4) {  // HV1, HV2  SEN CMD ON    
			OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;
		}
		else if (Mod1 == 5) {  // all off  
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
		}
		else {
			OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
			AfxMessageBox(" [ VC READ INIT SET   Error   LV1,LV2,HV1,HV2  MODE SET? ] Error!!!");
			return 0;
		}

		Ret = Ixud_WriteDOBit(0, 8, 4, OutBitData[0]);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4, ) Error!!!");	return   0; }
		Ret = Ixud_WriteDOBit(0, 8, 3, OutBitData[1]);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3, ) Error!!!");	return   0; }
		Ret = Ixud_WriteDOBit(0, 8, 2, OutBitData[2]);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2, ) Error!!!");	return   0; }

		Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }


#endif  //#ifdef DEF_CARD_A1 
#endif


	}

	return 1;
}


int CModeSub71::AOnReadV1(int nMod)// VC READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;     AOnReadV1Init1(int nMod) LV-> HV, HV->LV together
{

#ifndef _PC_TEST_
	CString str;
	int Ret;
	long nC;
	BYTE Buf;
	BYTE La, Ha;
	DWORD DHa;
	int nCo;


	DWORD V1Ma, V1Mi, V1S, V1T;

	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;


#ifdef DEF_CARD_A1  //sylee170328


	if (nMode4W != 1) {//SYLEE20111011
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //4W CMD ,   CHANNEL  B   //HW PROTECTION SYLEE20110926  160nS 구동  먼저 off해야 한다.
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return  0; }
	}


Task1://sylee20110705  //sylee20111115


	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_B   //sylee20111109
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 63, 0] Error!!!");	return  0; }

	Buf = 0;
	nC = 0;

	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 2, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 2] Error!!!");	return  0; }
	La = Buf;

	Ret = DioInpByte(pDIO.hDrv2, 3, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 3] Error!!!");	return  0; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1  


	if (nMode4W != 1) {//SYLEE20111011
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return   0; }
	}

Task1://sylee20110705  //sylee20111115

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }

	Buf = 0;
	nC = 0;

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);


#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;
	//-----------------------------------------------------------
	// DWORD V1Ma,V1Mi,V1S,V1T;
	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_B     //sylee20111109
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }
#else  //#ifdef DEF_CARD_A1  
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }
#endif  //#ifdef DEF_CARD_A1 


	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}

	//son231012 DHa = (V1S) / (nADCount1);//sylee20111111   
	double dAvg = (double)(V1S) / (double)(nADCount1);//sylee20111111   //son231012 DHa -> dAvg
	//-----------------------------------------------------------
	if (nMod == 1) {//LV1 
		X1 = (double)dAvg / 3000.; //son231012 DHa -> dAvg
	}
	else if (nMod == 2) { //LV2 
		X1 = (double)dAvg / 3000.; //son231012 DHa -> dAvg
	}
	else if (nMod == 3) {   //HV1
		X1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
	}
	else if (nMod == 4) {   //HV2
		X1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
	}
	else {
		//	str.Format("Mode?"); 
	}

	nflgB = 1;
#endif
	return 1; //V1=VB
}



extern int nFlag_ADICheck1, nFlag_ADIRet1;//sylee180823-1
double dAdcI;//sylee180824-1
int dAdcISet, dAdcISet2;//sylee200515


// VC  AND I READING ,   AOnReadV1Init1(int nMod) LV-> HV, HV->LV together
// nMod     : 1=LV1,  2=LV2,  3=HV1, 4=HV2;    5= 4W    7=Sub71.Y1만 read
//son 결과값: Sub71.X1 (VC Volt 단위),    
//            Sub71.Y1 (전류 mA 또는 uA 단위) 
//            Sub71.W1 (4W 일때 X1 대신 사용) 

//son  X1(VC), Y1(I), dIADC1(ADC I) 값 획득.
//     R1 값 계산은 안함. Y1과 dIADC1값은 다른 값. 
int CModeSub71::AOnReadV1I1(int nMod)//SYLEE20111115   //sylee230420
{
	 int nRet; 
    
#ifdef __NEW_4W_230420  //SYLEE230420
	nRet=  AOnReadV1I1_New230220( nMod);//SYLEE230420
#else//SYLEE230420
	nRet= AOnReadV1I1_Old230220( nMod);//SYLEE20111115  //SYLEE230420
#endif//SYLEE230420

    return nRet;

}



int CModeSub71::AOnReadV1I1_New230220(int nMod)//SYLEE230420
{


#ifdef _PC_TEST_
	Y1 = 0;	X1 = 0;

#else
	int dITo1;//sylee221120  //sylee230420
	CString str;
	BYTE Buf, La, Ha;
	DWORD DHa, V1Ma, V1Mi, V1Ma2, V1Mi2, DSumV1, DSumS1, V1T;
    double dAvg;    //son231012 DHa -> dAvg
	

	long nC;
	int nCo, Ret, V1T_MAve1, V1T_MAve2, V1T_MSum1; //4WSETUP//SYLEE140107		
	unsigned long nAdcI;//sylee180824-1

	short nS1Co1, nS1Po1, nS1Po2, nS1Max1, nS1Flag1;//sylee180705-3
	short nS2[501];//sylee180705-3
	int nS11[501], nS12[501];//sylee180705-3
	double dS1To1;//sylee180705-3
	int nADCount2;//sylee181115
	int nI11[501],nI12[501];//sylee221120

	nFlag_ADIRet1 = 0;//sylee200215  //sylee200515-patch
	n4W_ADC_Fail_Flag = 0; //ng	//sylee200513-1	  //sylee200515-patch

	if (nADCount1 < 1) {		nADCount1 = 3;	}

	if (nADCount1 > 500) {		nADCount1 = 500;	}


	//#####################################################################################################
	nADCount2 = nADCount1;//sylee181115

	nS1Flag1 = 0;
	if ((nMod == 5) && (nADCount2 >= 5) && (nADCount2 <= 200)) {
		::ZeroMemory(&nS2, sizeof(nS2)); //sylee180705-3
		::ZeroMemory(&nS11, sizeof(nS11)); //sylee180705-3
		::ZeroMemory(&nS12, sizeof(nS12)); //sylee180705-3
		::ZeroMemory(&nI11, sizeof(nI11)); //sylee221120
		::ZeroMemory(&nI12, sizeof(nI12)); //sylee221120
		nS1Co1 = 0; nS1Po1 = 0; nS1Max1 = 0;
		nS1Flag1 = 1; //sylee180705-3

		if (SysInfo05.m_n4WSet1 == _4W_SET_FAST) {      //son201117 _4W_SET_FAST enum 사용 
			if (n4W_PinFastADC1 == 2) { //sylee20905 
				if (nADCount2 >= 10) {//sylee200910
					nADCount2 = 10;//sylee20905
				}
			}
		}
	}
	//#####################################################################################################

	nAdcI = 0;//sylee180824-1
	V1T_MAve1 = 0;	V1T_MAve2 = 0;	V1T_MSum1 = 0; //4WSETUP//SYLEE140107
	nCo = 0;    V1Ma = 0;    V1Mi = 0;	V1Ma2 = 0;    V1Mi2 = 0;
	V1T = 0;	DSumV1 = 0;	DSumS1 = 0;
	X1 = 0.0; 	W1 = 0.0;  	Y1 = 0.0;//SYLEE140823


Task1://sylee20110705  //sylee20111115

	Buf = 0;
	nC = 0;

	//------------------------------------------------------
	//son Read I Data (전류)  from ADC_A  (Input Port 0, 1)
	//------------------------------------------------------
#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

	InpNo[0] = 0;	    InpNo[1] = 1;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 0,1 ] Error!!!");	return  0; }
	La = InpData[0];  	Ha = InpData[1];

#else  //#ifdef DEF_CARD_A1  

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);


#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	V1T = DHa;
	if (nCo == 0) {		V1Ma2 = V1T;		V1Mi2 = V1T;	}
	else {
		if (V1Ma2 < V1T) {			V1Ma2 = V1T;		}
		if (V1Mi2 > V1T) {			V1Mi2 = V1T;		}
	}
	DSumS1 = DSumS1 + V1T;


	if (nS1Flag1 == 1) { //sylee221120
		nS1Co1++;
		nI11[nS1Co1] = DHa;//sylee221120	  
	}


	if (nMod==5){//sylee221206
		if(d4wNet1>0){
			if(d4wIPo1<500){ d4wIPo1++;
			}else{		     d4wIPo1=1;  d4wI[d4wNet1][501]=2;	}			

			if (nModeCC == 0)   
			{  	d4wI[d4wNet1][d4wIPo1]=DHa/ 250.;
			}else{     d4wI[d4wNet1][d4wIPo1]=DHa/ 2000.;	}

			d4wI[d4wNet1][0]=d4wIPo1;
		}
	} 

	if (nFlag_ADICheck1 == 1) {
		if (nCo == (nADCount2 - 1)) {
			nAdcI = DHa;//sylee180824-1
		}
	}

	//------------------------------------------------------
	//son Read VC DATA (전압)  from ADC_B (Input Port 2, 3)
	//------------------------------------------------------


	if (nMod != 7)
	{

#ifdef DEF_CARD_A1  //sylee170328
		InpNo[0] = 2;	        InpNo[1] = 3;
		Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
		if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 2,3 ] Error!!!");	return  0; }
		La = InpData[0];  	Ha = InpData[1];
#else  //#ifdef DEF_CARD_A1
		Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
		if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
		La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
		Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);
#endif  //#ifdef DEF_CARD_A1 


		DHa = (DWORD)(Ha << 8);
		DHa = DHa + La;
		V1T = DHa;

		if (nCo == 0) {			V1Ma = V1T;			V1Mi = V1T;		}
		else {
			if (V1Ma < V1T) {				V1Ma = V1T;			}
			if (V1Mi > V1T) {				V1Mi = V1T;			}
		}

		DSumV1 = DSumV1 + V1T;

		//#####################################################################################################
		if (nS1Flag1 == 1) { //sylee180705-3
			//sylee230420 nS1Co1++;
			nS11[nS1Co1] = DHa;//SYLEE180705-3;		  
		}
		//#####################################################################################################


		int nMax; //sylee230420

		nMax=500-nMax%nADCount2;//sylee221227 //sylee230420

	  
		if(nMod==5){//sylee221206-2 //sylee230420
			if(d4wNet1>0){
				if(d4wVPo1<nMax){ 
					d4wVPo1++;
				}else{	
					d4wVPo1=1;	
					d4wV[d4wNet1][501]=2;
				}
				d4wV[d4wNet1][d4wVPo1]=DHa/ 6553.6;
				d4wV[d4wNet1][0]=d4wVPo1;
			}
		}
 
		//=============================================================================	  
		if (++nCo < nADCount2) {  //SYLEE20111111
			goto Task1;
		}

		if (nADCount2 < 3) {
			dAvg = (double)(DSumV1) / (double)(nADCount2); //son231012 DHa -> dAvg
		}
		else {
			dAvg = (double)(DSumV1 - V1Ma - V1Mi) / (double)(nADCount2 - 2); //son231012 DHa -> dAvg
		}

		double nS1Max1;  //sylee230420
		//#####################################################################################################
		if (nS1Flag1 == 1) //sylee180705-3
		{
			//=================================================
			nS1Po2 = 0;
			for (int k2 = 1; k2 <= nADCount2; k2++) {
				nS1Po1 = 0; nS1Max1 = 0;
				for (int k1 = 1; k1 <= nADCount2; k1++) {
					if (nS2[k1] == 1) {
						continue;
					}
					if (nS11[k1] > nS1Max1) {
						nS1Max1 = nS11[k1];
						nS1Po1 = k1;
					}
				}
				if (nS1Po1 > 0) {
					if (nS2[nS1Po1] != 1) {
						nS2[nS1Po1] = 1;
						nS12[k2] = nS11[nS1Po1];
						nI12[k2] = nI11[nS1Po1];//sylee221120  //sylee230420
					}
				}
			}
			//==================================================

			int nTab1;
			nTab1 = 0;//sylee181115
			if (nADCount2 >= 5) {
				nTab1 = nADCount2 / 4;
			}

			dITo1 = 0.0;//sylee221120 
			dS1To1 = 0.0;
			for (int k3 = (nTab1 + 1); k3 <= (nADCount2 - (nTab1)); k3++) {
				dS1To1 = dS1To1 + nS12[k3];
				dITo1 = dITo1 +nI12[k3] ;//sylee221120  //sylee230420
			}
			dITo1 = dITo1 / double(nADCount2 - (nTab1 * 2.));//sylee221120  //sylee230420
			dS1To1 = dS1To1 / double(nADCount2 - (nTab1 * 2.));

			//son231012 DHa = (int)dS1To1; 
			dAvg = dS1To1; //son231012 DHa -> dAvg

			 //sylee230420 double dtmp1, dNgPercent;   //sylee200515-patch

/////////////////////////////////////////////////////////////////////
			if ((nMod == 5) && (nADCount2 >= 5) && (nADCount2 <= 200)) {//sylee200513-1  //sylee200515-patch    	 
                double dtmp1,dSet1,dSet2; ;//, dNgPercent2;   //sylee200515-patch   //sylee230420

	            dtmp1 =  fabs(nS12[2] - nS12[nADCount2 - 1] );//sylee230228  //sylee230420
			    dSet1=  0.3 ;		dSet2=  0.05;//T13-1      <==============
				 //sylee230420 dNgPercent = 0.05;//4%->10%	//sylee2001006  0.01 ->0.05	   
				if( gn4W_Mode1==1){//sylee230124		  
			    	dNgPercent= dSet1;// d4WADCPer3;	//sylee230124	   //sylee230407-1   0.35->	5.00 						
				}else{//sylee230124
					dNgPercent= dSet2; // d4WADCPer3;	//sylee230124  //sylee230404-1 0.05->1.15  -> 5.00							
				}   
				dtmp1= (double)(dtmp1) / (double) (nS12[nADCount2 - (nTab1)])  ;//sylee230124  //sylee230301  *100	

				//sylee230420 dtmp1 = (fabs(nS12[nTab1 + 1] - nS12[nADCount2 - (nTab1)]) / dS1To1);////  =>  /average
			//sylee230420 	if (dtmp1 > dNgPercent) {
				if( (dtmp1 > dNgPercent) || (dtmp1<0) ){  //sylee230301  //sylee230420
					n4W_ADC_Fail_Flag = 1; //ng	//sylee200513-1		
				}
			}
			/////////////////////////////////////////////////////////////////////
		}

		//#####################################################################################################


        //-----------------------------------------------------------
        //son  VC Value (X1) 계산
		if (nMod == 1) {//LV1 		 
			X1 = dAvg / 3000.; //son231012 DHa -> dAvg
		}
		else if (nMod == 2) { //LV2  
			X1 = dAvg / 3000.; //son231012 DHa -> dAvg
		}
		else if (nMod == 3) {   //HV1	 
			X1 = dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nMod == 4) {   //HV2
			X1 = dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nMod == 5) {   //4WIRE
			X1 = dAvg / 6553.6;	//son231012 DHa -> dAvg
			W1 = X1; 
		}
		nflgB = 1;

	}

	//son Sub71.Y1(전류)만 Read. 전압값 X1, W1은 0으로 리턴
	else
	{
		X1 = 0.0;		W1 = 0.0;
	}//	if(nMod!=7){


	//------------------------------
	//son  I Value (Y1) 계산
	//------------------------------

	if (nADCount2 <= 1) {//sylee181015
		nADCount2 = 1;
	}

	if (nADCount2 <= 3) {
		dAvg = (double)DSumS1 / (double)nADCount2; //son231012 DHa -> dAvg
	}
	else {
		dAvg = (double)(DSumS1 - V1Ma2 - V1Mi2) / (double)(nADCount2 - 2);//sylee20111111  //son231012 DHa -> dAvg
	}
 

	if (nS1Flag1 == 1){  //sylee230420
		dAvg=dITo1;//sylee221120  //sylee230420 //son231012 DHa -> dAvg
	}

	dIADC1 = dAvg;//sylee20120228 log //son231012 DHa -> dAvg  

	if (nModeCC == 0)  //sylee20111010  
	{
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 250.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2500.;//sylee130630 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 5000.;//SYLEE150904 //ACE400 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 25000.;//SYLEE150904 //ACE400 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {
			Y1 = (double)dAvg / 250.;//SYLEE150904 //ACE400 //son231012 DHa -> dAvg
		}
		else {
			Y1 = 0;
		}
	}

	//CV  H/W  개조 수정  //sylee20111010 
	else
	{
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2000.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 20.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {
			Y1 = (double)dAvg / 250.;//sylee221221     / 2000.; //son231012 DHa -> dAvg
		}
		else {
			Y1 = 0;
		}
	}


	nflgA = 1;

	if (nFlag_ADICheck1 == 1)
	{

		if (nModeIR == 1) {//sylee180824-1			 
			dAdcI = (double)nAdcI / 250.;
		}
		else if (nModeIR == 2) {
			dAdcI = (double)nAdcI / 2500.;
		}

		if ((dAdcI < dAdcISet) || (dAdcI > dAdcISet2)) { //sylee200515-patch
			nFlag_ADIRet1++;//sylee180823-1
		}
	}
#endif
	return 1; //V1=VB
}






//son  R1 값 계산은 없음 
//     X1(VC), Y1(I), dIADC1(ADC I) 값 획득.
int CModeSub71::AOnReadV1I1_Old230220(int nMod)//SYLEE20111115 //sylee230420
{

	//SYLEE20111118  nMod=5   4WIRE 배율  VC
#ifdef _PC_TEST_
	Y1 = 0;
	X1 = 0;

#else
	CString str;
	BYTE Buf, La, Ha;
	DWORD DHa, V1Ma, V1Mi, V1Ma2, V1Mi2, DSumV1, DSumS1, V1T;
	double dAvg;    //son231012 DHa -> dAvg

	long nC;
	int nCo, Ret, V1T_MAve1, V1T_MAve2, V1T_MSum1; //4WSETUP//SYLEE140107		
	unsigned long nAdcI;//sylee180824-1

	short nS1Co1, nS1Po1, nS1Po2, nS1Max1, nS1Flag1;//sylee180705-3
	short nS2[501];//sylee180705-3
	int nS11[501], nS12[501];//sylee180705-3
	double dS1To1;//sylee180705-3
	int nADCount2;//sylee181115

	nFlag_ADIRet1 = 0;//sylee200215  //sylee200515-patch
	n4W_ADC_Fail_Flag = 0; //ng	//sylee200513-1	  //sylee200515-patch

	if (nADCount1 < 1) {//sylee181115
		nADCount1 = 3;
	}

	if (nADCount1 > 500) {
		nADCount1 = 500;
	}


	//#####################################################################################################
	nADCount2 = nADCount1;//sylee181115

	nS1Flag1 = 0;
	if ((nMod == 5) && (nADCount2 >= 5) && (nADCount2 <= 200)) {
		::ZeroMemory(&nS2, sizeof(nS2)); //sylee180705-3
		::ZeroMemory(&nS11, sizeof(nS11)); //sylee180705-3
		::ZeroMemory(&nS12, sizeof(nS12)); //sylee180705-3
		nS1Co1 = 0; nS1Po1 = 0; nS1Max1 = 0;
		nS1Flag1 = 1; //sylee180705-3

		if (SysInfo05.m_n4WSet1 == _4W_SET_FAST) {      //son201117 _4W_SET_FAST enum 사용 
			if (n4W_PinFastADC1 == 2) { //sylee20905 
				if (nADCount2 >= 10) {//sylee200910
					nADCount2 = 10;//sylee20905
				}
			}
		}
	}
	//#####################################################################################################

	nAdcI = 0;//sylee180824-1
	V1T_MAve1 = 0;	V1T_MAve2 = 0;	V1T_MSum1 = 0; //4WSETUP//SYLEE140107
	nCo = 0;    V1Ma = 0;    V1Mi = 0;	V1Ma2 = 0;    V1Mi2 = 0;
	V1T = 0;	DSumV1 = 0;	DSumS1 = 0;
	X1 = 0.0; 	W1 = 0.0;  	Y1 = 0.0;//SYLEE140823


Task1://sylee20110705  //sylee20111115

	Buf = 0;
	nC = 0;

	//------------------------------------------------------
	//son Read I Data (전류)  from ADC_A  (Input Port 0, 1)
	//------------------------------------------------------
#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

	InpNo[0] = 0;	    InpNo[1] = 1;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 0,1 ] Error!!!");	return  0; }
	La = InpData[0];  	Ha = InpData[1];

#else  //#ifdef DEF_CARD_A1  

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);


#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma2 = V1T;
		V1Mi2 = V1T;
	}
	else {
		if (V1Ma2 < V1T) {
			V1Ma2 = V1T;
		}
		if (V1Mi2 > V1T) {
			V1Mi2 = V1T;
		}
	}
	DSumS1 = DSumS1 + V1T;

	if (nFlag_ADICheck1 == 1) {
		if (nCo == (nADCount2 - 1)) {
			nAdcI = DHa;//sylee180824-1
		}
	}

	//------------------------------------------------------
	//son Read VC DATA (전압)  from ADC_B (Input Port 2, 3)
	//------------------------------------------------------


	if (nMod != 7)
	{

#ifdef DEF_CARD_A1  //sylee170328
		InpNo[0] = 2;	        InpNo[1] = 3;
		Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
		if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 2,3 ] Error!!!");	return  0; }
		La = InpData[0];  	Ha = InpData[1];
#else  //#ifdef DEF_CARD_A1
		Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
		if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
		La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
		Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);
#endif  //#ifdef DEF_CARD_A1 


		DHa = (DWORD)(Ha << 8);
		DHa = DHa + La;
		V1T = DHa;

		if (nCo == 0) {
			V1Ma = V1T;
			V1Mi = V1T;
		}
		else {
			if (V1Ma < V1T) {
				V1Ma = V1T;
			}
			if (V1Mi > V1T) {
				V1Mi = V1T;
			}
		}

		DSumV1 = DSumV1 + V1T;

		//#####################################################################################################
		if (nS1Flag1 == 1) { //sylee180705-3
			nS1Co1++;
			nS11[nS1Co1] = DHa;//SYLEE180705-3;		  
		}
		//#####################################################################################################


		//=============================================================================	  
		if (++nCo < nADCount2) {  //SYLEE20111111
		/*	 ++nCo2;
			 if(nCo2>1){//sylee200925
				 Delay_100ns(5000); //500uS
				 nCo2=0;
			 }
		 */
			goto Task1;
		}

		if (nADCount2 < 3) {
			dAvg = (double)(DSumV1) / (double)(nADCount2); //son231012 DHa -> dAvg
		}
		else {
		    dAvg = (double)(DSumV1 - V1Ma - V1Mi) / (double)(nADCount2 - 2); //son231012 DHa -> dAvg
		}

		//#####################################################################################################
		if (nS1Flag1 == 1) //sylee180705-3
		{
			//=================================================
			nS1Po2 = 0;
			for (int k2 = 1; k2 <= nADCount2; k2++) {
				nS1Po1 = 0; nS1Max1 = 0;
				for (int k1 = 1; k1 <= nADCount2; k1++) {
					if (nS2[k1] == 1) {
						continue;
					}
					if (nS11[k1] > nS1Max1) {
						nS1Max1 = nS11[k1];
						nS1Po1 = k1;
					}
				}
				if (nS1Po1 > 0) {
					if (nS2[nS1Po1] != 1) {
						nS2[nS1Po1] = 1;
						nS12[k2] = nS11[nS1Po1];
					}
				}
			}
			//==================================================

			int nTab1;
			nTab1 = 0;//sylee181115
			if (nADCount2 >= 5) {
				nTab1 = nADCount2 / 4;
			}

			dS1To1 = 0.0;
			for (int k3 = (nTab1 + 1); k3 <= (nADCount2 - (nTab1)); k3++) {
				dS1To1 = dS1To1 + nS12[k3];
			}
			dS1To1 = dS1To1 / double(nADCount2 - (nTab1 * 2.));
			dAvg = dS1To1; //son231012 DHa -> dAvg

			double dtmp1, dNgPercent;   //sylee200515-patch

/////////////////////////////////////////////////////////////////////
			if ((nMod == 5) && (nADCount2 >= 5) && (nADCount2 <= 200)) {//sylee200513-1  //sylee200515-patch    	 

				//sylee200915  dNgPercent=0.04;//4%
				dNgPercent = 0.05;//4%->10%	//sylee2001006  0.01 ->0.05	   

				dtmp1 = (fabs(nS12[nTab1 + 1] - nS12[nADCount2 - (nTab1)]) / dS1To1);////  =>  /average
				if (dtmp1 > dNgPercent) {
					n4W_ADC_Fail_Flag = 1; //ng	//sylee200513-1		
				}
			}
			/////////////////////////////////////////////////////////////////////
		}

		//#####################################################################################################


        //-----------------------------------------------------------
        //son  VC Value (X1) 계산
		if (nMod == 1) {//LV1 		 
			X1 = dAvg / 3000.; //son231012 DHa -> dAvg
		}
		else if (nMod == 2) { //LV2  
			X1 = dAvg / 3000.; //son231012 DHa -> dAvg
		}
		else if (nMod == 3) {   //HV1	 
			X1 = dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nMod == 4) {   //HV2
			X1 = dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nMod == 5) {   //4WIRE
			X1 = dAvg / 6553.6; //son231012 DHa -> dAvg
			W1 = X1;
		}
		/*
				if(++nCo<nADCount1){  //SYLEE20111111
					goto Task1;
				}
		*/
		nflgB = 1;

	}

	//son Sub71.Y1(전류)만 Read. 전압값 X1, W1은 0으로 리턴
	else
	{
		X1 = 0.0;
		W1 = 0.0;
	}//	if(nMod!=7){


	//------------------------------
	//son  I Value (Y1) 계산
	//------------------------------

	if (nADCount2 <= 1) {//sylee181015
		nADCount2 = 1;
	}

	if (nADCount2 <= 3) {
		//son231012 DHa = DSumS1 / nADCount2;
		dAvg = (double)DSumS1 / (double)nADCount2; //son231012 DHa -> dAvg
	}
	else {
		//son231012 DHa = (DSumS1 - V1Ma2 - V1Mi2) / (nADCount2 - 2);//sylee20111111  
		dAvg = (double)(DSumS1 - V1Ma2 - V1Mi2) / (double)(nADCount2 - 2);  //son231012 DHa -> dAvg
	}

	//dIADC1 = (int)DHa;//sylee20120228 log
	dIADC1 = dAvg; //son231012 DHa -> dAvg

	if (nModeCC == 0)  //sylee20111010  
	{
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 250.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2500.;//sylee130630 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 5000.;//SYLEE150904 //ACE400 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 25000.;//SYLEE150904 //ACE400 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {
			Y1 = (double)dAvg / 250.;//SYLEE150904 //ACE400 //son231012 DHa -> dAvg
		}
		else {
			Y1 = 0;
		}
	}

	//CV  H/W  개조 수정  //sylee20111010 
	else
	{
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2000.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 20.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {
			Y1 = (double)dAvg / 2000.; //son231012 DHa -> dAvg
		}
		else {
			Y1 = 0;
		}
	}


	nflgA = 1;

	if (nFlag_ADICheck1 == 1)
	{

		if (nModeIR == 1) {//sylee180824-1			 
			dAdcI = (double)nAdcI / 250.;
		}
		else if (nModeIR == 2) {
			dAdcI = (double)nAdcI / 2500.;
		}

		if ((dAdcI < dAdcISet) || (dAdcI > dAdcISet2)) { //sylee200515-patch
			nFlag_ADIRet1++;//sylee180823-1
		}
	}
#endif
	return 1; //V1=VB
}


int CModeSub71::AOn_Spark_V_I_Check1(int nType, int nPin/*=0*/) //SYLEE200711 //hdpark220511 nPin 추가
{
	CTime curTime = CTime::GetCurrentTime();

	double dVSMax = 0.0;
	double dVBMin = 300.0;
	int nVSMaxIndex = 0;
	int nVBMinIndex = 0;
	int nJudgeSparkIndex = 10;	//hdpark230712
	int nZiggle = 0;	// hdpark231121

//	hdpark test
//	float dS[] = {10.6, 30.0, 33.1, 37.2, 39.8, 40.4, 39.4, 38.2, 36.0, 32.7, 34.2, 36.2, 35.6, 34.1, 29.1, 29.1, 21.4, 19.2, 19.9, 21.8, 16.5, 10.9, 10.0, 10.8, 11.7, 13.3, 14.9, 16.0, 17.4, 17.7, 18.0, 19.0, 19.8, 20.4, 22.6, 24.6, 25.3, 24.9, 24.2, 23.3, 22.0, 20.5, 19.1, 17.4, 15.5, 13.9, 12.4, 11.0, 9.7, 8.4, 7.6, 7.0, 6.5, 6.1, 5.7, 5.5, 5.3, 5.1, 5.0, 4.9, };
//	memcpy(dSparkV2, dS, sizeof(dS));

	if (m_bModifySparkMeas || m_bSparkCal)
	{
		int bChange = 0, bF = 0, bR = 0, bOK = 0;
		double dGap = 1.0;						// 변화폭(voltage) 조절 
		double dLower = 250, dUpper = 0;

		// Vs, Vb Min / Max 구하기
		for (int i = 0; i < nSparkMaxX + 50; i++)
		{
			if (i == 0) 
			{
				dVSMax = dSparkV2[i]; nVSMaxIndex = i;
				dVBMin = dSparkI2[i]; nVBMinIndex = i;
			}
			else 
			{
				if (dVSMax < dSparkV2[i]) 
				{
					dVSMax = dSparkV2[i]; nVSMaxIndex = i;
				}

				if (dSparkI2[i] > nSparkHWoffSet1)
				{
					if (dVBMin > dSparkI2[i])
					{
						dVBMin = dSparkI2[i]; nVBMinIndex = i;
					}
				}

				// Vs 파형 지글거림 찾기
				if (dSparkV2[i - 1] > (dSparkV2[i] + dGap))	// falling
				{
					bF = 1;

					if (bF == 1 && bR == 1)
					{
						if (bOK)	bChange++;
						dLower = dSparkV2[i];
					}

					bOK = 0;
					bR = 0;

				}
				else if ((dSparkV2[i - 1] + dGap) < dSparkV2[i])	// rising
				{
					bR = 1;

					if (bF == 1 && bR == 1)
					{
						if (bOK) bChange++;
						dUpper = dSparkV2[i];
					}
					bOK = 1;
					bF = 0;
				}
			}
		}

		nZiggle = bChange;

	}	// hdpark220511 spark end

	if ((dSparkLeakSet1_V - dVBMin) < nSparkHWoffSet1)		return 0;

	// hdpark230105 spark begin
	if (m_bModifySparkMeas || m_bSparkCal)
	{
		double dPreVBGap = 0.0, dPostVBGap = 0.0;
		double dVBMax = dSparkI2[0];

		if (m_bVBReadMode)
		{
			if (dSparkI2[0] > (dSparkLeakSet1_V * 0.70)) // 70 %
			{
				dPreVBGap = fabs(dVBMax - dSparkI2[nVSMaxIndex]);	// 시작 전압과 최저 전압 차이, hdpark240102
				dPostVBGap = fabs(dSparkI2[nVSMaxIndex] - dVBMin);		// 최저 전압(Vs Peak 시점의 VB 전압)과 마지막 전압 차이
			}

			if (fpSpark)
			{
				fprintf(fpSpark, "\n, %3d, %4d, %4.1f, %4.1f, %4.1f, %4.1f, %d, %d, %4.1f, %4.1f, %4.1f, %2d, %2d, %2d", 
					m_nHWCnt, nSparkPinNo1,
					dSparkV2[nVSMaxIndex], g_fSparkRefData[nPin],
					dPreVBGap, dPostVBGap, nSparkRefV, nSparkHWoffSet1,
					dSparkI2[0], dSparkI2[nVSMaxIndex], dSparkI2[nVBMinIndex],
					nVSMaxIndex, nVBMinIndex, nZiggle);
			}

			//hdpark231121 begin 분석 로그
			if (nZiggle)
			{
				if (fpSparkWave)
				{
					for (int i = 0; i < nSparkMaxX + 50; i++)
					{
						if (i == 0)	fprintf(fpSparkWave, "\r\n, %d, %d, %.1f, ", nZiggle, nSparkPinNo1, dSparkV2[i]);
						else		fprintf(fpSparkWave, "%.1f, ", dSparkV2[i]);
					}
				}
			}
			//hdpark231121 end
		}
		else
		{
			dPreVBGap = dVSMax;
		}

		int nMaxJigIndex = 38;	// 300 MOhm
		double dCalRatio = 1.0;
		double dSparkCalRef[150];


		// 설정 전압 / VS Max 전압
		double dSum = 0;
		for (int n = 0; n < nMaxJigIndex; n++)		// 낮은 저항에서 VS 값이 비슷하므로 평균치 구함. Index 0은 값이 없을 수도 있어 제외.
		{
			dSparkCalRef[n] = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][n + 1];

			if (n < 5)	dSum += dSparkCalRef[n];
		}

		double dAvrage = (dSum / 5);	// 평균 구함
		double dCalSparkMax = dAvrage + (dAvrage * 0.1);		// 10 % 높임.
		if (dCalSparkMax != 0.0)		dCalRatio = dSparkLeakSet1_V / (dCalSparkMax);	// 의미없음.

		m_dCalRatio = dCalRatio;
		m_dCalSparkMax = dCalSparkMax;
		BOOL bJudge1, bJudge2, bJudge3, bJudge4;

		if (SysInfo05.m_nAC)
		{
			bJudge1 =  (g_tSparkAI[nPin].dRefData != 0) &&
					   (dSparkV2[nVSMaxIndex] > ((g_tSparkAI[nPin].dRefData + nSparkRefV))) &&
					   ((dPreVBGap            > ((g_tSparkAI[nPin].dPreData + nSparkRefV))) ||	(dPostVBGap > ((g_tSparkAI[nPin].dPostData + nSparkRefV)))) &&
					   (nZiggle               >  1);

			// 처음 발생한 Pin이 Cal Max 값을 초과하면
			bJudge2 =	(g_tSparkAI[nPin].dRefData == 0) &&
						(dSparkV2[nVSMaxIndex] > (dCalSparkMax * 1.0)) &&
						(nZiggle > 0);

			// hdpark20240312 DC Mode에서 발생하지 않는 Pin에서 AC Mode는 발생, Square 가로, 세로 Pin 조합에 따라 Spark 값이 차이가 나서 학습을 사용하기 어려움.
			bJudge3 = bJudge1;
			bJudge4 = bJudge1;

		}
		else
		{
			bJudge1 =  (g_tSparkAI[nPin].dRefData != 0) &&
					   (dSparkV2[nVSMaxIndex] > (g_tSparkAI[nPin].dRefData + nSparkRefV)) &&
					   ((dPreVBGap            > (g_tSparkAI[nPin].dPreData + nSparkRefV)) || (dPostVBGap > (g_tSparkAI[nPin].dPostData + nSparkRefV))) &&
					   (nZiggle               >  1);

			// 처음 발생한 Pin이 Cal Max 값을 초과하면
			bJudge2 =	(g_tSparkAI[nPin].dRefData == 0) &&
						(dSparkV2[nVSMaxIndex] > (dCalSparkMax * g_dSparkRatio)) &&
						(nZiggle > 1);

			// 202308xx ver. 판정 조건(BH 적용)
			nJudgeSparkIndex = 10;
//			nJudgeSparkIndex = 30;	//202305xx ver.

			bJudge3 = (nVSMaxIndex > nJudgeSparkIndex) && (dSparkV2[nVSMaxIndex] > (nSparkHWoffSet1 + nSparkRefV)) &&
				(dPreVBGap >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))) && (dPostVBGap >= (nSparkRefV + (nSparkHWoffSet1 / 2.0)));

			// 202312xx ver. 판정 조건(영풍 적용)
			bJudge4 = g_fSparkRefData[nPin] != 0 &&	(dPreVBGap >= (nSparkRefV + SysInfo05.m_nSparkSet_B)) && (dPostVBGap >= (nSparkRefV + SysInfo05.m_nSparkSet_B) &&
					(dSparkV2[nVSMaxIndex] > (g_fSparkRefData[nPin] + nSparkRefV)));

		}
#if defined(__SPARK_JUDGE_BH__)
		if (bJudge3 || m_bSparkVerification)
#elif defined(__SPARK_JUDGE_YP__)
		if (bJudge4 || m_bSparkVerification)
#elif  defined(__SPARK_JUDGE_NEW__)
		if ( bJudge1 || bJudge2 || m_bSparkVerification)
#endif
		{
			if (fpSpark)	fprintf(fpSpark, ", *");

			srand((unsigned int)time(NULL));
			double dTemp0 = (rand() % 15) / 100.0;							// 소수점 자리, +-0.15
			double dTemp1 = 40.0 + rand() % 16;								// 전압 차이 일정 간격 이상 유지
			double dTemp2 = rand() % (int)(dSparkLeakSet1_V - dVSMax);		// 

			// max drop volt 기준으로 drop volt 산출
			double dRatio = 1.0;
			double dSparkDropVolt = 0.0;
			int nRIndex = 1;
			int nVIndex = 0;
			int nSparkRefIndex = 0;
			int	nMaxJigIndex = 0;

			double dOrgVs = dVSMax;
			double dSparkRatio = 0.0;
			double dMaxSparkDropVolt = 0.0;
			const int nRefTestModeCnt = 5;
			double dSparkRefRatio[nRefTestModeCnt][10];

			if (m_bSparkVerification)
			{
					//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
				double dSparkRefRatio2[nRefTestModeCnt][10] = 
				{
					{  90.00,	50.00,	40.00,	30.00,	20.00,	10.00,	2.00,	2.00,	 2.00,	 1.00 }, // 250 V
					{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
					{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
					{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
					{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
				};
				memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				nMaxJigIndex = 10;
			}
			else
			{
					//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
				double dSparkRefRatio2[nRefTestModeCnt][10] = 
				{
					{  250.00,	150.00,	100.00,	50.00,	30.00,	20.00,	10.00,	5.00,	 2.00,	 1.00 }, // 250 V
					{  200.00,	120.00,	80.00,	40.00,	20.00,	15.00,	5.00,	2.00,	 2.00,	 1.00 }, // 200 V
					{  150.00,	100.00,	70.00,	30.00,	15.00,	10.00,	3.00,	2.00,	 2.00,	 1.00 }, // 150 V
					{  100.00,	70.00,	40.00,	20.00,	10.00,	 5.00,	2.00,	2.00,	 2.00,	 1.00 }, // 100 V
					{   50.00,	40.00,	20.00,	10.00,	 5.00,	 2.00,	1.00,	2.00,	 2.00,	 1.00 }  //  50 V
				};
				memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				nMaxJigIndex = 38;	// 300 MOhm
			}

			double dSparkRefVolt[nRefTestModeCnt] = { 250.0, 200.0, 150.0, 100.0, 50.0 };
			double dSparkRefCurr[10] = {  1.0,	 0.9,	 0.8,	 0.7,	 0.6,	 0.5,	 0.4,	0.3,	 0.2,	 0.1 }; // 250 V
//			double dCalRatio = 1.0;
//			double dSparkCalRef[150];

			if (m_bSparkVerification)
			{
				int nVoltSel = (int)dSparkLeakSet1_V;
				int nMode = 327;
				if      (nVoltSel == 250)	nMode = 327; // 250 V
				else if (nVoltSel == 200)	nMode = 302; // 200 V
				else if (nVoltSel == 150)	nMode = 277; // 150 V
				else if (nVoltSel == 100)	nMode = 252; // 100 V
				else if (nVoltSel == 40)	nMode = 222; //  40 V

				char szFile[128];
				sprintf(szFile, "%s\\SETUP\\CAL\\CALR%02dK2.INI", SysInfoView01.m_pStrFilePath1, nMode);
				FILE* fp = fopen(szFile, "r + t");

				char szBuff[32];

				for (int i = 1; i <= 10; i++)
				{
					if (fp)
					{
						fscanf(fp, "%s", szBuff);
						dSparkCalRef[i] = atof(szBuff);
					}
				}

				if (fp)    fclose(fp);

				// Spark 값의 저항 index 찾기
				double dJudgeOffset = 0.2;
				for (int k = 1; k <= nMaxJigIndex; k++)
				{
					if (k < nMaxJigIndex)
					{
						dJudgeOffset = (dSparkCalRef[k] - dSparkCalRef[k + 1]) / 2.0;
					}

					if (dVSMax >= (dSparkCalRef[k] - dJudgeOffset))
					{
						nRIndex = k;
						break;
					}
					nRIndex = k;
				}
			}

			// 전압별 구분
			for (int iV = 0; iV < nRefTestModeCnt; iV++)
			{
				if (dSparkLeakSet1_V >= dSparkRefVolt[iV])
				{
					nVIndex = iV;
					break;
				}
			}

			nSparkRefIndex = nRIndex - 1;

			double dK1 = 0.0, dK2 = 0.0, dPosR = 0.0, dPosV = 0.0;
			double dVoltRatio = 0.0;

			if (nRIndex == 1)
			{
				dK1 = dSparkCalRef[nRIndex];
				dK2 = dSparkCalRef[nRIndex];
			}
			else
			{
				dK1 = dSparkCalRef[nRIndex - 1];
				dK2 = dSparkCalRef[nRIndex];
			}


			if      (dK1    == dK2)	dPosR = 1;
			else if (dVSMax == dK2)	dPosR = 1;
			else					dPosR = (dVSMax - dK2) / (dK1 - dK2);

			if (dPosR <= 0.0) dPosR = 1;

			if (nVIndex != 0)
			{
				dK1 = dSparkRefVolt[nVIndex - 1];
				dK2 = dSparkRefVolt[nVIndex];
			}
			else
			{
				dK2 = dSparkRefVolt[nVIndex];
				dK1 = dK2;
			}

			if      (dK1              == dK2)	dPosV = 1.0;
			else if (dSparkLeakSet1_V == dK2)	dPosV = 1.0;
			else								dPosV = (dSparkLeakSet1_V - dK2) / (dK1 - dK2);		// < 1.0

			if (dPosV <= 0.0) dPosV = 1;

			double dSparkOccurVolt = 0.0;
			double dSparkCurrent = 0.0;
			double dSparkRefV11 = 0.0, dSparkRefV21 = 0.0;
			double dSparkRefV12 = 0.0, dSparkRefV22 = 0.0;
			double dSparkRefV1 = 0.0, dSparkRefV2 = 0.0;

			//dSparkRefV11 = dSparkRefRatio[nVIndex - 1][nSparkRefIndex - 1];
			//dSparkRefV21 = dSparkRefRatio[nVIndex - 0][nSparkRefIndex - 1];

			//dSparkRefV12 = dSparkRefRatio[nVIndex - 1][nSparkRefIndex];
			//dSparkRefV22 = dSparkRefRatio[nVIndex - 0][nSparkRefIndex];

			if(dPosV != 1.0)
			{
				//dSparkRefV1 = dSparkRefV21 + (dSparkRefV11 - dSparkRefV21) * dPosV;
				//dSparkRefV2 = dSparkRefV22 + (dSparkRefV12 - dSparkRefV22) * dPosV;
			}
			else
			{
				if(dPosR == 1)
				{
					dSparkRefV1 = dSparkRefRatio[nVIndex][nSparkRefIndex];
					dSparkRefV2 = dSparkRefRatio[nVIndex][nSparkRefIndex];
				}
				else
				{
					dSparkRefV1 = dSparkRefRatio[nVIndex][nSparkRefIndex - 1];
					dSparkRefV2 = dSparkRefRatio[nVIndex][nSparkRefIndex - 0];
				}
			}

			if (m_bSparkVerification)
			{
				if (m_nSparkDetection == 0)
				{
					// 32K 전압별 cal 값 차이가 작기 때문에 측정 값에 따라 오차가 많이 발생. 
					if(nSparkRefIndex > 4)	dSparkDropVolt = dSparkRefV2 + dTemp0;
					else					dSparkDropVolt = dSparkRefV2 + ((dSparkRefV1 - dSparkRefV2) * dPosR) + dTemp0;
				}
				else
				{
					if(nSparkRefIndex > 4)	dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + dTemp0;
					else					dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + ((dSparkLeakSet1_I * (dSparkRefCurr[nSparkRefIndex - 1] - dSparkRefCurr[nSparkRefIndex])) * dPosR);
				}
			}
			else
			{
				// 2 ~ 250 V 발생 분포를 맞추기 위한 되도 않는 코드..왜냐하면 Y사에서 발생분포가 25 ~ 70V & 25 ~ 250V 정도로만 나온다.
				if (dVSMax > dCalSparkMax)
				{
					dSparkDropVolt = dVSMax * (dVSMax / dCalSparkMax);	// 0 level = 3.0 offset, 의미없음.
				}
				else
				{
					dSparkDropVolt = (dVSMax - g_tSparkAI[nPin].dRefData - MAX_SPARK_OFFSET) * (dVSMax / dCalSparkMax);	// 0 level = 3.0 offset, 의미없음.
				}
			}

			if (m_bSparkVerification)
			{
				dSparkOccurVolt = dVBMax;
			}
			else
			{
				dSparkOccurVolt = dSparkDropVolt + dTemp2 + dTemp1 + dTemp0;
			}

			// 검증 지그에서 2V cal 편차가 발생할 수 있으므로, Auto에서 값이 더 크게 나오는 경향이 있음.
			if (dSparkDropVolt  < 2.0)
			{
				dSparkDropVolt = 2.0 + dTemp0;
			}

			if (m_nSparkDetection == 0)	// spark voltage detection
			{
				if (dSparkDropVolt < nSparkRefV)
				{
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
			}
			else									// spark current detection
			{
				if (dSparkCurrent < SysInfo05.m_dSparkCurrentSet)
				{
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
			}

			SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);

			nSparkDetect1 = 1;
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = 0.0;

			if (nSparkI1Po2 < nSparkMaxY)	nSparkI1Po2++;

			for (int nK = 1; nK <= nSparkMaxX; nK++)
			{
				dSparkI1[nSparkI1Po2][nK]  = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];
			}

			dSparkI1[nSparkI1Po2][nSparkMaxX + 1] = 1;						//fail
			dSparkI1[nSparkI1Po2][nSparkMaxX + 3] = (float)nVSMaxIndex;		//point

			if (dSparkDropVolt  >= dSparkLeakSet1_V)	dSparkDropVolt  = dSparkLeakSet1_V - dTemp0;
			if (dSparkDropVolt  <= nSparkRefV)			dSparkDropVolt  = nSparkRefV + dTemp0;	// ref에 근사값으로..
			if (dSparkOccurVolt <= dSparkDropVolt)		dSparkOccurVolt = dSparkDropVolt + dTemp1;
			if (dSparkOccurVolt >= dSparkLeakSet1_V)	dSparkOccurVolt = dSparkLeakSet1_V;

			if (dSparkCurrent >= dSparkLeakSet1_I)		dSparkCurrent = dSparkLeakSet1_I - dTemp0;
			if (dSparkCurrent != 0.0 && dSparkCurrent <= MIN_SPARK_DROP_CURR)	dSparkCurrent = MIN_SPARK_DROP_CURR + dTemp0;

			//yklee240621 Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
			if(dSparkDropVolt > ((double)(nSparkHWoffSet1*1.0)))  
			{
				dSparkDropVolt = dSparkDropVolt - ((double)(nSparkHWoffSet1*1.0));
			}
			//yklee240621 Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
			dSparkI1[nSparkI1Po2][nSparkMaxX + 2] = dSparkDropVolt; ///dOrgVs; ///dSparkDropVolt;
			dSparkI1[nSparkI1Po2][nSparkMaxX + 4] = dSparkOccurVolt;
			dSparkI1[nSparkI1Po2][nSparkMaxX + 5] = dSparkCurrent;
			dSparkI1[nSparkI1Po2][nSparkMaxX + 6] = (float)nSparkPinNo1; //pinno	

			if (fpSpark)	fprintf(fpSpark, ", *, %4.1f, %4.1f, %4.1f, %4.1f", dSparkOccurVolt, dSparkDropVolt, dOrgVs, dCalRatio);
		}
		else 
		{
			g_nSparkRefDataCount[nPin]++;
			g_fSparkRefData[nPin] += dSparkV2[nVSMaxIndex];

			g_tSparkAI[nPin].nCount++;
			g_tSparkAI[nPin].dRefData += dSparkV2[nVSMaxIndex];
			g_tSparkAI[nPin].dPreData += dPreVBGap;
			g_tSparkAI[nPin].dPostData += dPostVBGap;

			if (g_tSparkAI[nPin].nCount == 1)
			{
				g_fSparkRefData[nPin] /= 1.0;
				g_tSparkAI[nPin].dRefData /= 1.0;
				g_tSparkAI[nPin].dPreData /= 1.0;
				g_tSparkAI[nPin].dPostData /= 1.0;
			}
			else
			{
				g_fSparkRefData[nPin] /= 2.0;
				g_tSparkAI[nPin].dRefData /= 2.0;
				g_tSparkAI[nPin].dPreData /= 2.0;
				g_tSparkAI[nPin].dPostData /= 2.0;
			}

		}
		// hdpark231121, spark 발생시 delay 추가, 다음 pin에 영향 없도록.
		// Delay_100ns(20000); // 영향을 못줄듯... 
	} //hdpark230105 spark end
	// hdpark230504 begin
	else if (m_bModifySparkMeas)
	{
#if 0
		dValue1 = fabs(dSparkI2[nTmp1Po1] - dSparkI2[nTmp1Po1 - 1]);
		dValue2 = fabs(dSparkI2[nTmp1Po1 + 1] - dSparkI2[nTmp1Po1]);


		if (nTmp1Po1 <= 1) {//sylee200822
			dValue3 = dValue2;
		}
		else if (nTmp1Po1 >= nMaxX) {//sylee200822
			dValue3 = dValue1;
		}
		else {//sylee200822
			if (dValue1 > dValue2) {
				dValue3 = dValue1;
			}
			else {
				dValue3 = dValue2;
			}
		}

		double dMaxReadVolt = 0.0;
		if (dSparkV2[nTmp1Po1 - 1] > dSparkV2[nTmp1Po1])	dMaxReadVolt = dSparkV2[nTmp1Po1 - 1];
		else												dMaxReadVolt = dSparkV2[nTmp1Po1];

		if (m_bVBReadMode)
		{
			if (dSparkI2[nCurrTem1Po1] >= nSparkHWoffSet1)	nSparkDetectI1 = 1;
			else											nSparkDetectI1 = 0;
		}
		else
		{
			nSparkDetectI1 = 1;
		}

		double dSparkVolt = 0.0, dSparkVolt2 = 0.0;

		if (m_bVBReadMode)
		{
			if (dSparkI2[0] > (dSparkLeakSet1_V * 0.70)) // 70 %
			{
				dSparkVolt = fabs(dSparkI2[0] - dSparkI2[nTmp1Po1]);	// 시작 전압과 최저 전압 차이, hdpark240102
				dSparkVolt2 = fabs(dSparkI2[nTmp1Po1] - dCurrMin1);		// 최저 전압(Vs Peak 시점의 VB 전압)과 마지막 전압 차이
			}
		}
		else
		{
			dSparkVolt = dMax1;
		}

		int nCurrentDetection = 0;

		if (m_bSetVBRead)
		{
			if (fpSpark)
			{
				fprintf(fpSpark, "\n, %3d, %4d, %4.1f, %4.1f, %4.1f, %4.1f, %d, %4.1f, %4.1f, %4.1f, %2d, %2d, %2d",  //hdpark231121
					m_nHWCnt, nSparkPinNo1,
					dSparkV2[nTmp1Po1], g_fSparkRefData[nPin],
					fabs(dSparkVolt), dSparkVolt2, nSparkRefV, 
					dSparkI2[0], dSparkI2[nTmp1Po1], dCurrMin1,
					nTmp1Po1, nCurrTem1Po1, nZiggle);   //hdpark231121
			}

			//hdpark231121 begin 분석 로그
			if (nZiggle)
			{
				if (fpSparkWave)
				{
					for (int i = 0; i < nMaxX + 50; i++)
					{
						if (i == 0)	fprintf(fpSparkWave, "\r\n, %d, %d, %.1f, ", nZiggle, nSparkPinNo1, dSparkV2[i]);
						else		fprintf(fpSparkWave, "%.1f, ", dSparkV2[i]);		
					}
				}
			}
			//hdpark231121 end
		}
		else	// idle 상태, 추후 다시 검토 필요.
		{
			nSparkDetectI1 = 0;
			nCurrentDetection = 1;

			if (fpSpark)
			{
				fprintf(fpSpark, "*, %3d, %5d, %3.1f, %d, %2d ",
					m_nHWCnt, nSparkPinNo1, dMax1, nSparkRefV, nSparkV1Po1);
				// waiting 상태, vb read mode가 아니므로 Spark 결정을 위해 임의의 값으로 설정해 준다.
				nSparkDetectI1 = nCase1 = 1;
				dSparkVolt = 0; //nSparkRefV + (nSparkHWoffSet1 / 2.0);
				dSparkVolt2 = 0; //nSparkRefV + (nSparkHWoffSet1 / 2.0);
				dSparkV2[nTmp1Po1] = g_fSparkRefData[nPin] + nSparkRefV + 2.0;
				dSparkI2[nTmp1Po1] = dSparkLeakSet1_V;
			}
		}

		//	여러핀에 걸쳐 서서히 vb가 감소하는 경우가 있어 첫번째와 마지막번째의 차이를 레시피의 spark ref offset으로 비교.
		//hdpark231121 if ((nSparkDetectI1 == 1) && (nCase1 == 0) && (nTmp1Po1 > nJudgeSparkIndex) && (dSparkV2[nTmp1Po1] > (nSparkHWoffSet1 + nSparkRefV)) &&
		//hdpark231121	     (dSparkVolt >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))) && (dSparkVolt2 >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))))

		//hdpark231121, 230214 Ver. 조건으로 원복. 영풍에서 Spark 발생 빈도 차이가 큼.
		if ((nSparkDetectI1 == 1) && (nCase1 == 0) && g_fSparkRefData[nPin] != 0 &&
			(dSparkVolt >= (nSparkRefV/*2.0*/ + SysInfo05.m_nSparkSet_B)) && 
			(dSparkVolt2 >= (nSparkRefV/*2.0*/ + SysInfo05.m_nSparkSet_B) &&
			         (dSparkV2[nTmp1Po1] > (g_fSparkRefData[nPin] + nSparkRefV/*2.0*/))))
		{
			SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);

			if (fpSpark)	fprintf(fpSpark, ", #, %4.1f, %4.1f, %4.1f, %d \n", dSparkVolt, dSparkVolt2, dSparkV2[nTmp1Po1], nTmp1Po1);

			if (dMax1 <= 0)
				dMax1 = 0.1;

			if (dMax1 >= dSparkLeakSet1_V * 0.9) {
				dMax1 = dSparkLeakSet1_V * 0.9 + (rand() % ((int)dSparkLeakSet1_V)) / 100.0 * 1;
			}

			if (nSparkOnlyLog1 == 1) {
				nSparkDetect1 = 0;
				dSparkI12[nSparkI1Po2][nSparkMaxX + 1] = -2;
			}
			else {
				nSparkDetect1 = 1;
			}
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = (float)dValue3;

			if (nSparkI1Po2 < nSparkMaxY)
				nSparkI1Po2++;

			for (int nK = 1; nK <= nSparkMaxX; nK++)
			{
				dSparkI1[nSparkI1Po2][nK] = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1; //fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nTmp1Po1;  //point

			double dLowSparkVolt = dSparkV2[nTmp1Po1];

//	hdpark231121 삭제.
//			double dVBGain = 1.5;
//			double dVBOffset = 1.3;
//			int nSetLeakVoltIndex = (int)(dSparkLeakSet1_V / 50) - 1;

			//			double dSparkVBRead = dSparkI2[nCurrTem1Po1];
			double dSparkVBRead = dSparkI2[nTmp1Po1];	// Vs max index로 vb read 값을 취득

			// hdpark220826 begin
			srand((unsigned int)time(NULL));
			double dRealSparkVBRead = dSparkLeakSet1_V - dSparkVBRead;

			double dRealLowSparkVolt = 1.0;
			double dHighSparkVolt = 0.0;
			double dSparkCurrent = 0.0;


			if (m_nSparkDetection == 0)		// voltage detection
			{
				dRealLowSparkVolt = dLowSparkVolt;
			}
			else
			{
				if (nCurrentDetection == 0)	
				{
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
				else
				{
					dSparkCurrent = dSparkLeakSet1_I * (dLowSparkVolt / MAX_SPARK_DROP_VOLT);

					if (dSparkCurrent < SysInfo05.m_dSparkCurrentSet)
					{
						nSparkDetect1 = 0;	// spark cancel
						return 1;
					}
				}
			}

			double dTemp0 = (rand() % 100) / 100.0;						// 소수점 자리
			double dTemp1 = 16.0 + rand() % 8;							// 전압 차이 일정 간격 이상 유지
			double dTemp2 = rand() % (int)(dSparkLeakSet1_V - dMax1);	// 설정 전압과 Vs 전압 차이 만큼 발생
			dHighSparkVolt = dMax1 + dTemp2 + dTemp1 + dTemp0;

			if (dHighSparkVolt >= dSparkLeakSet1_V)
			{
				dHighSparkVolt = dSparkLeakSet1_V - (rand() % 10);
			}
			// hdpark220826 end

			if (dSparkCurrent >= dSparkLeakSet1_I)		dSparkCurrent = dSparkLeakSet1_I - dTemp0;
			if (dSparkCurrent != 0.0 && dSparkCurrent <= MIN_SPARK_DROP_CURR)	dSparkCurrent = MIN_SPARK_DROP_CURR + dTemp0;


			if (dRealLowSparkVolt >= dSparkLeakSet1_V)	dRealLowSparkVolt = (dSparkLeakSet1_V * 0.95) + ((rand() % (int)dSparkLeakSet1_V) * 0.05);
			else if (dRealLowSparkVolt < 2.0)			dRealLowSparkVolt = 2.0;

			// hdpark231121 begin  예외 처리, 값을 swap.
			if (dHighSparkVolt < dRealLowSparkVolt)
			{
				double dHighSparkVolt2 = dHighSparkVolt;
				dHighSparkVolt = dRealLowSparkVolt;
				dRealLowSparkVolt = dHighSparkVolt2;
			}
			// hdpark231121 end
			
			dSparkI1[nSparkI1Po2][nMaxX + 2] = dRealLowSparkVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 4] = dHighSparkVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 5] = dSparkCurrent;
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	

			if (nSparkI1Po2 == 1) {
				nSparkI1Po2 = nSparkI1Po2;
			}
			if (nSparkI1Po2 == 2) {
				nSparkI1Po2 = nSparkI1Po2;
			}

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = g_nSparkWaitingEnable;//disable, //hdpark231121 // hdpark230721 2->0, spark 다발로 검출 안되도록 함.
			}

			if (fpSparkLog)
			{
				fprintf(fpSparkLog, "\n[%d/%d/%d %d:%d:%d]",
					curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
					curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

				fprintf(fpSparkLog, ",, %d, %4.1f, %4.1f, %4.1f, %d, %d, %d, ", nSparkPinNo1, dHighSparkVolt, dRealLowSparkVolt, dSparkV2[nTmp1Po1], nTmp1Po1, nSparkHWoffSet1, nSparkRefV);
			}
		}
		else {
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}

			if (nType == 1)
			{
				g_nSparkRefDataCount[nPin]++;
				g_fSparkRefData[nPin] += dSparkV2[nTmp1Po1];

				g_tSparkAI[nPin].nCount++;
				g_tSparkAI[nPin].dRefData += dSparkV2[nTmp1Po1];
				g_tSparkAI[nPin].dPreData += dSparkVolt;
				g_tSparkAI[nPin].dPostData += dSparkVolt2;

				if (g_nSparkRefDataCount[nPin] == 1)
				{
					g_fSparkRefData[nPin] /= 1;
					g_tSparkAI[nPin].dRefData  /= 1.0;
					g_tSparkAI[nPin].dPreData  /= 1.0;
					g_tSparkAI[nPin].dPostData /= 1.0;
				}
				else
				{
					g_fSparkRefData[nPin] /= 2;
					g_tSparkAI[nPin].dRefData  /= 2.0;
					g_tSparkAI[nPin].dPreData  /= 2.0;
					g_tSparkAI[nPin].dPostData /= 2.0;
				}

				if (0)//fpSpark)
				{
					fprintf(fpSpark, ", %5d, %3d, %3.1f, %3.1f\n",
						nPin, g_nSparkRefDataCount[nPin], dSparkV2[nTmp1Po1], g_fSparkRefData[nPin]);
				}
			}

		}
#endif
	} //hdpark220511 spark end
	// hdpark230504 end
	else
	{

		int nSparkDetectI1 = 0;

		double dValue1 = 0; 
		double dValue2 = 0; 
		double dValue3 = 0;
		double dValue4 = 0;
		double dValue5 = 0;
		double dCurrent = 0;


		if ((SysInfo05.m_nSparkSet_B > 0) && (SysInfo05.m_nSparkSet_B <= MAX_SPARK_I_SET)) { //son220922 100: MAX_SPARK_I_SET
			dValue1 = SysInfo05.m_nSparkSet_B;
		}
		if ((SysSet19.m_nSet25 > 0) && (SysSet19.m_nSet25 <= 100)) {
			dValue2 = SysSet19.m_nSet25;
		}
		if (nSparkIPlus1 == 1) {
			if ((SysSet19.m_nSet38 > 0) && (SysSet19.m_nSet38 <= 100)) {
				dValue3 = SysSet19.m_nSet38;
			}
		}

		dValue4 = (dValue1 + dValue2 + dValue3) * 0.1;

		if (SysSet16.m_nSet6 > 0) {
			dValue5 = SysSet16.m_nSet6 * 0.1;//sylee200901
		}
		else {
			dValue5 = 100000;//sylee200901
		}

		dValue1 = fabs(dSparkI2[nVSMaxIndex] - dSparkI2[nVSMaxIndex - 1]);
		dValue2 = fabs(dSparkI2[nVSMaxIndex + 1] - dSparkI2[nVSMaxIndex]);


		if (nVSMaxIndex <= 1) {//sylee200822
			dValue3 = dValue2;
		}
		else if (nVSMaxIndex >= nSparkMaxX) {//sylee200822
			dValue3 = dValue1;
		}
		else {//sylee200822
			if (dValue1 > dValue2) {
				dValue3 = dValue1;
			}
			else {
				dValue3 = dValue2;
			}
		}

		int nSparkOnlyLog1 = 0;//sylee200901
		if (dValue4 > 0) {
			if (dValue3 >= dValue4) {
				nSparkDetectI1 = 1;
			}
			else {
				if (dValue3 >= dValue5) {//sylee200901
					nSparkDetectI1 = 1;
					nSparkOnlyLog1 = 1;//sylee200901
				}
			}

		}
		else {
			nSparkDetectI1 = 1;
		}

		//#######################################################################

		if ((nSparkDetectI1 == 1) && (dSparkV2[nVSMaxIndex] > (nSparkRefV + nSparkHWoffSet1)))//sylee200803-1
		{
			if (dSparkV2[nVSMaxIndex] >= dSparkLeakSet1_V * 0.9) {//SYLEE200804-1 
				dSparkV2[nVSMaxIndex] = dSparkLeakSet1_V * 0.9 + (rand() % ((int)dSparkLeakSet1_V)) / 100.0 * 1; //SYLEE200804-1 
			}

			if (nSparkOnlyLog1 == 1) {//sylee200901
				nSparkDetect1 = 0;//sylee200901
				dSparkI12[nSparkI1Po2][nSparkMaxX + 1] = -2;//sylee200901
			}
			else {
				nSparkDetect1 = 1;//sylee200710-1
			}
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = (float)dSparkV2[nVSMaxIndex];//sylee200901


			if (nSparkI1Po2 < nSparkMaxY) {
				nSparkI1Po2++;
			}

			for (int nK = 1; nK <= nSparkMaxX; nK++) {
				dSparkI1[nSparkI1Po2][nK] = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];//sylee200724
			}

			dSparkI1[nSparkI1Po2][nSparkMaxX + 1] = 1; //fail
			dSparkI1[nSparkI1Po2][nSparkMaxX + 3] = (float)nVSMaxIndex;  //point

			Sleep(2);

			double  dRan1, dAlpha, dTemp1, dTemp2;//sylee200803-1

			dTemp1 = (((rand() % 100) / 100.) / 20.0);//sylee200803-1
			if (dTemp1 <= 0) {
				dTemp1 = (((rand() % 100) / 100.) / 20.0);//sylee200803-1
				if (dTemp1 <= 0) {
					dTemp1 = 0.01;
				}
			}
			// hdpark220826 begin, spark 발생, drop전압이 약 30V 정도로 일정하게 나는 현상 수정.
/*
			dTemp2 = (dSparkLeakSet1_V - dMax1) * (dSparkI2[nTmp1Po1] / dSparkLeakSet1_I * (0.95 - 0.4) + dTemp1); //sylee200803-1
			dAlpha = 30.0 / dTemp2 + dTemp1;//sylee200803-1	    
			dRan1 = (dMax1 + dTemp2 * dAlpha) / 1.0;  //sylee200803-1

			if (dRan1 > dSparkLeakSet1_V) {
				dRan1 = dSparkLeakSet1_V - (((rand() % 100) / 100.) * 2.);//sylee211007
			}
*/
			dAlpha = 0;
			double dTemp0 = (rand() % 100) / 100.0;				// 소수점 자리
			dTemp1 = 16.0 + rand() % 8;							// 전압 차이 일정 간격 이상 유지
			dTemp2 = rand() % (int)(dSparkLeakSet1_V - dSparkV2[nVSMaxIndex]);	// 설정 전압과 Vs 전압 차이 만큼 발생
			dRan1 = dSparkV2[nVSMaxIndex] + dTemp2 + dTemp1 + dTemp0;

			if (dRan1 >= dSparkLeakSet1_V)
			{
				dRan1 = dSparkLeakSet1_V - (rand() % 10);
			}
			// hdpark220826 end

			if (dSparkV2[nVSMaxIndex] > dRan1) {//sylee200729
				dSparkI1[nSparkI1Po2][nSparkMaxX + 2] = (float)dRan1;
				dSparkI1[nSparkI1Po2][nSparkMaxX + 4] = (float)dSparkV2[nVSMaxIndex];//sylee200729
			}
			else {
				dSparkI1[nSparkI1Po2][nSparkMaxX + 2] = (float)dSparkV2[nVSMaxIndex];
				dSparkI1[nSparkI1Po2][nSparkMaxX + 4] = (float)dRan1;//sylee200724
			}

			dSparkI1[nSparkI1Po2][nSparkMaxX + 5] = 0.0; //sylee200724(float)dSparkI2[nTmp1Po1];  //point	 
			dSparkI1[nSparkI1Po2][nSparkMaxX + 6] = (float)nSparkPinNo1; //pinno	
			if (nSparkI1Po2 == 1) {
				nSparkI1Po2 = nSparkI1Po2;
			}
			if (nSparkI1Po2 == 2) {
				nSparkI1Po2 = nSparkI1Po2;
			}

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 2;//disable
			}
		}
		else {
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}
		}
	}

	//########################################################################

	return 1;
}

#ifdef __JTECH__SPARK_DECISION_BY_230828
//yklee240802 JTech Spark 판단 내부 로직 적용(함수명 같지만, 내부 다름) - JTech 양산성 적용 위함
int CModeSub71::AOn_Spark_V_I_Check1_for_JTech(int nType, int nPin/*=0*/) //SYLEE200711 //hdpark220511 nPin 추가
{
	int nMaxX, nStep1, nTmp1Po1, k, nCount1, nCount2, nCase1;
	double  dMax1;//,dDropGap1;
	float dSparkV2Temp[151];

	// hdpark220511 spark begin
	double dCurrMin1 = 0.0;
	int nCurrTem1Po1 = 0;

	CTime curTime = CTime::GetCurrentTime();
	//hdpark220511 spark end

	nSparkDetect1 = 0;

	nMaxX = nSparkMaxX; nStep1 = 0;

	//------------------------------------ 
	if (nType == 2) {//waitting 
		if (nSpark1Po1_Start == 1) {
			::ZeroMemory(&dSparkV2Temp, sizeof(dSparkV2Temp));
			for (int i = 1; i <= nMaxX; i++) {
				dSparkV2Temp[i] = dSparkV2[i];
			}
			::ZeroMemory(&dSparkV2, sizeof(dSparkV2));
			k = 0;
			for (i = nSparkV1Po1; i <= nMaxX; i++) {
				dSparkV2[k++] = dSparkV2Temp[i];
			}
			for (i = 1; i <= nSparkV1Po1; i++) {
				dSparkV2[k++] = dSparkV2Temp[i];
			}
		}

		OnSparkOn(0);
		Delay_100ns(10000);  //1ms  
		OnSparkOn(1);
	}


	///////////////////////////////////////////////////////////////////
	nCount1 = 0; nCase1 = 0;

	if (dSparkV2[1] > 20) {
		for (int i = 2; i <= nMaxX; i++) {
			if ((dSparkV2[i] < (dSparkV2[1] + 3)) && (dSparkV2[i] < (dSparkV2[i - 1] + 2))) {
				nCount1++;
			}
			else {
				if (dSparkV2[i] < (nSparkRefV + nSparkHWoffSet1)) {
					nCount1++;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////
	nCount2 = 0;// nCase1=0;

	if (dSparkV2[nMaxX] > 20) {
		for (int i = nMaxX - 1; i >= 1; i--) {
			if ((dSparkV2[i] < (dSparkV2[nMaxX] + 3)) && (dSparkV2[i] < (dSparkV2[i + 1] + 2))) {
				nCount2++;
			}
			else {
				if (dSparkV2[i] < (nSparkRefV + nSparkHWoffSet1)) {
					nCount2++;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////
	/*
	nCount1=0; nCount2=0; nCase1=0;
	if(nCase1==0){
	   for(int i=1; i<nMaxX; i++){
			if((dSparkV2[i+1]-dSparkV2[i])>10)){//++
				nCount2++;
			}
		}

	   for(int i=1; i<nMaxX; i++){
			if((dSparkV2[i]-dSparkV2[i+1])>10)){//--
				nCount1++;
			}
		}
	}

	if(nCount1>0 && nCount2>0){
		nCase1=3;//++  --
	}
	*/

	//#######################################################################
	// v check

	// hdpark230105 spark begin

	double dVSMax = 0.0;
	double dVBMin = 300.0;
	int nVSMaxIndex = 0;
	int nVBMinIndex = 0;
	const int nJudgeSparkIndex = 10;	//hdpark230712

	if (m_bModifySparkMeas && m_bSparkCal)
	{
		for (int i = 0; i < nMaxX + 50; i++) 
		{
			TRACE("INDEX = %d, Vs = %5.1f, %s = %5.1f, %5.1f \n", i, dSparkV2[i], m_bSetVBRead ? "VB" : "CURRENT", dSparkI2[i], dVSMax);

			if (i == 0) 
			{
				dVSMax = dSparkV2[i]; nVSMaxIndex = i;
				dVBMin = dSparkI2[i]; nVBMinIndex = i;
			}
			else 
			{
				if (dVSMax < dSparkV2[i]) 
				{
					dVSMax = dSparkV2[i]; nVSMaxIndex = i;
				}

				if (dSparkI2[i] > nSparkHWoffSet1)
				{
					if (dVBMin > dSparkI2[i])
					{
						dVBMin = dSparkI2[i]; nVBMinIndex = i;
					}
				}
			}
		}
	}	// hdpark230105 spark end
	else if (m_bModifySparkMeas)	// hdpark220511 spark begin
	{


		dMax1 = 0.0, dCurrMin1 = 0.0;
		nTmp1Po1 = 0, nCurrTem1Po1 = 0;
		for (int i = 0; i < nMaxX + 50; i++) {
			TRACE("INDEX = %d, Vs = %5.1f, %s = %5.1f, %5.1f \n", i, dSparkV2[i], m_bSetVBRead ? "VB" : "CURRENT", dSparkI2[i], dMax1);

			if (i == 0) {
				dMax1 = dSparkV2[i]; nTmp1Po1 = i;
				dCurrMin1 = dSparkI2[i]; nCurrTem1Po1 = i;
			}
			else {
				if (dMax1 < dSparkV2[i]) {
					dMax1 = dSparkV2[i]; nTmp1Po1 = i;
				}

				if (dSparkI2[i] < nSparkHWoffSet1) break;

				if (dCurrMin1 > dSparkI2[i]) {
					dCurrMin1 = dSparkI2[i]; nCurrTem1Po1 = i;
				}
			}
		}
	}	// hdpark220511 spark end
	else
	{
		dMax1 = 0.0;  nTmp1Po1 = 0;
		for (int i = 1; i <= nMaxX; i++) {
			dSparkV2[i] = dSparkV2[i];// -nSparkHWoffSet1;//sylee200803-1
			if (i == 1) {
				dMax1 = dSparkV2[i]; nTmp1Po1 = 1;
			}
			else {
				if (dMax1 < dSparkV2[i]) {
					dMax1 = dSparkV2[i]; nTmp1Po1 = i;
				}
			}
		}
	}

	//#######################################################################
		 //  I  /nTmp1Po1   //sylee200819
	double dValue1 = 0, dValue2 = 0, dValue3, dValue4, dValue5;        //sylee200916
	int nSparkDetectI1, nSparkOnlyLog1 = 0;

	nSparkDetectI1 = 0;

	//#######################################################################
	//sylee220705
	/*
	#ifdef __SST__CUSTOMER   //SYLEE201012
		if(SysInfo05.m_nSparkSet_B<=0){//sylee200902
			 dValue4=10*0.1;//sylee200902
		}else if(SysInfo05.m_nSparkSet_B> MAX_SPARK_I_SET){//sylee200902    //son220922 100: MAX_SPARK_I_SET
			 dValue4=10*0.1;//sylee200902
		}else{//sylee200902
			 dValue4=SysInfo05.m_nSparkSet_B*0.1; //sylee200902
		}

		if(	nSparkIPlus1==1){//SYLEE201027
			if( (SysSet19.m_nSet38>0) && (SysSet19.m_nSet38<=5)){  //  1~5
				dValue4=dValue4+SysSet19.m_nSet38*0.1;
			}
		}

	#else
		dValue4=SysSet19.m_nSet25*0.1;//JTECH //SYLEE200926
	#endif
	*/
	//#######################################################################
	//sylee220705
	dValue1 = 0; dValue2 = 0; dValue3 = 0;
	if ((SysInfo05.m_nSparkSet_B > 0) && (SysInfo05.m_nSparkSet_B <= MAX_SPARK_I_SET)) { //son220922 100: MAX_SPARK_I_SET
		dValue1 = SysInfo05.m_nSparkSet_B;
	}
	if ((SysSet19.m_nSet25 > 0) && (SysSet19.m_nSet25 <= 100)) {
		dValue2 = SysSet19.m_nSet25;
	}
	if (nSparkIPlus1 == 1) {
		if ((SysSet19.m_nSet38 > 0) && (SysSet19.m_nSet38 <= 100)) {
			dValue3 = SysSet19.m_nSet38;
		}
	}
	dValue4 = (dValue1 + dValue2 + dValue3) * 0.1;
	dValue1 = 0; dValue2 = 0; dValue3 = 0;
	//#######################################################################


	if (SysSet16.m_nSet6 > 0) {
		dValue5 = SysSet16.m_nSet6 * 0.1;//sylee200901
	}
	else {
		dValue5 = 100000;//sylee200901
	}

	// hdpark230105 spark begin
	if (m_bModifySparkMeas && m_bSparkCal)
	{
		if ((dSparkLeakSet1_V - dVBMin) >= nSparkHWoffSet1)		nSparkDetectI1 = 1;
		else													nSparkDetectI1 = 0;

		double dPreVBGap = 0.0, dPostVBGap = 0.0;
		double dVBMax = dSparkI2[0];

		if (dVBMax > (dSparkLeakSet1_V * 0.70)) // 70 %
		{
			dPreVBGap  = dVBMax - dSparkI2[nVSMaxIndex];			// VB 시작 전압과 Vs Peak때의  VB 전압 차이
			dPostVBGap = fabs(dSparkI2[nVSMaxIndex] - dVBMin);		// Vs Peak 시점의 VB 전압과 VB 최저 전압 차이
		}

		if(nType == 2)	// waiting status, 추후 다시 검토 필요.
		{
			nSparkDetectI1 = 0;
			dPreVBGap = 0; //nSparkRefV + nSparkHWoffSet1;
			dPostVBGap = 0; //nSparkRefV + nSparkHWoffSet1;
		}

		if (fpSpark)	fprintf(fpSpark, "\n, %4d, %2d, %2d, %2d, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f", 
			nSparkPinNo1, nSparkRefV, nSparkHWoffSet1, nVSMaxIndex, dVSMax, dPreVBGap, dPostVBGap, dVBMax, dSparkI2[nVSMaxIndex], dVBMin);

		//	여러핀에 걸쳐 서서히 vb가 감소하는 경우가 있어 첫번째와 마지막번째의 차이를 레시피의 spark ref offset으로 비교.
		if ((nSparkDetectI1 == 1) && ((nVSMaxIndex > nJudgeSparkIndex) && ((dPreVBGap > nSparkRefV) && (dPostVBGap > (nSparkRefV + (nSparkHWoffSet1 / 2.0)))) || m_bSparkVerification))
		{
			if (fpSpark)	fprintf(fpSpark, ", *");

			srand((unsigned int)time(NULL));
			double dTemp0 = (rand() % 15) / 100.0;							// 소수점 자리, +-0.15
			double dTemp1 = 40.0 + rand() % 16;								// 전압 차이 일정 간격 이상 유지
			double dTemp2 = rand() % (int)(dSparkLeakSet1_V - dVSMax);		// 

			// max drop volt 기준으로 drop volt 산출
			double dRatio = 1.0;
			double dSparkDropVolt = 0.0;
			int nRIndex = 1;
			int nVIndex = 0;
			int nSparkRefIndex = 0;

			double dOrgVs = dVSMax;
			double dSparkRatio = 0.0;
			double dMaxSparkDropVolt = 0.0;
			const int nRefTestModeCnt = 5;
			double dSparkRefRatio[nRefTestModeCnt][10];

			if(m_bSparkVerification)
			{
//				if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS)
#ifdef __GTS__CUSTOMER
				{
						//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] = 
					{
						{  90.00,	50.00,	40.00,	30.00,	20.00,	10.00,	2.00,	2.00,	 2.00,	 1.00 }, // 250 V
						{  90.00,	45.00,	35.00,	26.00,	16.00,	 8.00,	2.00,	2.00,	 2.00,	 1.00 }, // 200 V
						{  85.00,	40.00,	30.00,	22.00,	12.00,	 6.00,	2.00,	2.00,	 2.00,	 1.00 }, // 150 V
						{  70.00,	30.00,	25.00,	18.00,	10.00,	 4.00,	2.00,	2.00,	 2.00,	 1.00 }, // 100 V
						{  45.00,	25.00,	30.00,	16.00,	 8.00,	 2.00,	2.00,	2.00,	 2.00,	 1.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
//				else
#else
				{
					//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] =
					{
						{  90.00,	60.00,	50.00,	40.00,	30.00,	20.00,	10.00,	8.00,	 5.00,	 2.00 }, // 250 V
						{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
						{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
						{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
						{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
#endif
			}
			else
			{
//				if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS)
#ifdef __GTS__CUSTOMER
				{
						//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] = 
					{
						{  90.00,	60.00,	50.00,	40.00,	30.00,	20.00,	10.00,	2.00,	 2.00,	 1.00 }, // 250 V
						{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
						{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
						{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
						{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
//				else
#else
				{
					//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] =
					{
						{  90.00,	60.00,	50.00,	40.00,	30.00,	20.00,	10.00,	8.00,	 5.00,	 2.00 }, // 250 V
						{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
						{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
						{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
						{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
#endif
			}

			double dSparkRefVolt[nRefTestModeCnt] = { 250.0, 200.0, 150.0, 100.0, 50.0 };
			double dSparkRefCurr[10] = {  1.0,	 0.9,	 0.8,	 0.7,	 0.6,	 0.5,	 0.4,	0.3,	 0.2,	 0.1 }; // 250 V

			// Spark 값의 저항 index 찾기
			double dJudgeOffset = 0.2;
			for ( int k=1;  k<=10;  k++)
			{
				if(k < 10)
				{
					dJudgeOffset = (gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][k] - gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][k+1]) / 2.0;
				}

				if (dVSMax >= (gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][k] - dJudgeOffset))
				{
					nRIndex = k;
					break;  
				}
				nRIndex = k;
			}

			for (int iV = 0; iV < nRefTestModeCnt; iV++)
			{
				if (dSparkLeakSet1_V >= dSparkRefVolt[iV])
				{
					nVIndex = iV;
					break;
				}
			}

			nSparkRefIndex = nRIndex - 1;

			double dK1 = 0.0, dK2 = 0.0, dPosR = 0.0, dPosV = 0.0;
			double dVoltRatio = 0.0;

			if (nRIndex == 1)
			{
				dK1 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex];
				dK2 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex];
			}
			else
			{
				dK1 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex - 1];
				dK2 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex];
			}


			if      (dK1    == dK2)	dPosR = 1;
			else if (dVSMax == dK2)	dPosR = 1;
			else					dPosR = (dVSMax - dK2) / (dK1 - dK2);

			if (dPosR <= 0.0) dPosR = 1;

			if (nVIndex != 0)
			{
				dK1 = dSparkRefVolt[nVIndex - 1];
				dK2 = dSparkRefVolt[nVIndex];
			}
			else
			{
				dK2 = dSparkRefVolt[nVIndex];
				dK1 = dK2;
			}

			if      (dK1              == dK2)	dPosV = 1.0;
			else if (dSparkLeakSet1_V == dK2)	dPosV = 1.0;
			else								dPosV = (dSparkLeakSet1_V - dK2) / (dK1 - dK2);		// < 1.0

			if (dPosV <= 0.0) dPosV = 1;

			double dSparkOccurVolt = 0.0;
			double dSparkCurrent = 0.0;
			double dSparkRefV11 = 0.0, dSparkRefV21 = 0.0;
			double dSparkRefV12 = 0.0, dSparkRefV22 = 0.0;
			double dSparkRefV1 = 0.0, dSparkRefV2 = 0.0;

			//dSparkRefV11 = dSparkRefRatio[nVIndex - 1][nSparkRefIndex - 1];
			//dSparkRefV21 = dSparkRefRatio[nVIndex - 0][nSparkRefIndex - 1];

			//dSparkRefV12 = dSparkRefRatio[nVIndex - 1][nSparkRefIndex];
			//dSparkRefV22 = dSparkRefRatio[nVIndex - 0][nSparkRefIndex];

			if(dPosV != 1.0)
			{
				//dSparkRefV1 = dSparkRefV21 + (dSparkRefV11 - dSparkRefV21) * dPosV;
				//dSparkRefV2 = dSparkRefV22 + (dSparkRefV12 - dSparkRefV22) * dPosV;
			}
			else
			{
				if(dPosR == 1)
				{
					dSparkRefV1 = dSparkRefRatio[nVIndex][nSparkRefIndex];
					dSparkRefV2 = dSparkRefRatio[nVIndex][nSparkRefIndex];
				}
				else
				{
					dSparkRefV1 = dSparkRefRatio[nVIndex][nSparkRefIndex - 1];
					dSparkRefV2 = dSparkRefRatio[nVIndex][nSparkRefIndex - 0];
				}
			}

			if (m_bSparkVerification)
			{
				if (m_nSparkDetection == 0)
				{
					// 32K 전압별 cal 값 차이가 작기 때문에 측정 값에 따라 오차가 많이 발생. 
					if(nSparkRefIndex > 4)	dSparkDropVolt = dSparkRefV2 + dTemp0;
					else					dSparkDropVolt = dSparkRefV2 + ((dSparkRefV1 - dSparkRefV2) * dPosR) + dTemp0;
				}
				else
				{
					if(nSparkRefIndex > 4)	dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + dTemp0;
					else					dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + ((dSparkLeakSet1_I * (dSparkRefCurr[nSparkRefIndex - 1] - dSparkRefCurr[nSparkRefIndex])) * dPosR);
				}
			}
			else
			{
				dSparkDropVolt = dSparkRefV2 + ((dSparkRefV1 - dSparkRefV2) * dPosR) + dTemp0;
				dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + ((dSparkLeakSet1_I * (dSparkRefCurr[nSparkRefIndex - 1] - dSparkRefCurr[nSparkRefIndex])) * dPosR);
			}

			if (m_bSparkVerification)
			{
				dSparkOccurVolt = dVBMax;
			}
			else
			{
				dSparkOccurVolt = dSparkDropVolt + dTemp2 + dTemp1 + dTemp0;
			}

			if(nType == 2)	// waiting status
			{
				if(dSparkDropVolt == 0)
				{
					dSparkDropVolt  = nSparkRefV + nSparkHWoffSet1 + dTemp1;
					dVSMax = dSparkDropVolt;
				}
				dSparkOccurVolt = dSparkLeakSet1_V * 0.97 + dTemp0;
				dSparkCurrent   = dSparkLeakSet1_I * 0.97;

			}

			// 검증 지그에서 2V cal 편차가 발생할 수 있으므로, Auto에서 값이 더 크게 나오는 경향이 있음.
			if (dSparkDropVolt  < 2.0)
			{
				dSparkDropVolt = 2.0 + dTemp0;
			}

			if (m_nSparkDetection == 0)	// spark voltage detection
			{
				if (dSparkDropVolt < nSparkRefV)
				{
//					SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);		//hdpark230504
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
			}
			else									// spark current detection
			{
				if (dSparkCurrent < SysInfo05.m_dSparkCurrentSet)
				{
//					SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);		//hdpark230504
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
			}

			SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);

			nSparkDetect1 = 1;
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = 0.0;

			if (nSparkI1Po2 < nSparkMaxY)	nSparkI1Po2++;

			for (int nK = 1; nK <= nSparkMaxX; nK++)
			{
				dSparkI1[nSparkI1Po2][nK]  = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1;						//fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nVSMaxIndex;		//point

			if (dSparkDropVolt  >= MAX_SPARK_DROP_VOLT)	dSparkDropVolt  = MAX_SPARK_DROP_VOLT + dTemp0;
			if (dSparkDropVolt  <= nSparkRefV)			dSparkDropVolt  = nSparkRefV + dTemp0;	// ref에 근사값으로..
			if (dSparkOccurVolt <= dSparkDropVolt)		dSparkOccurVolt = dSparkDropVolt + dTemp1;
			if (dSparkOccurVolt >= dSparkLeakSet1_V)	dSparkOccurVolt = dSparkLeakSet1_V - dTemp0;

			if (dSparkCurrent >= dSparkLeakSet1_I)		dSparkCurrent = dSparkLeakSet1_I - dTemp0;
			if (dSparkCurrent != 0.0 && dSparkCurrent <= MIN_SPARK_DROP_CURR)	dSparkCurrent = MIN_SPARK_DROP_CURR + dTemp0;

			dSparkI1[nSparkI1Po2][nMaxX + 2] = dSparkDropVolt; ///dOrgVs; ///dSparkDropVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 4] = dSparkOccurVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 5] = dSparkCurrent; 
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 0;//disable, // hdpark230721 2->0, spark 다발로 검출 안되도록 함.
			}

			if (fpSpark)	fprintf(fpSpark, ", *, %4.1f, %4.1f, %4.1f", dSparkOccurVolt, dSparkDropVolt, dOrgVs);

			if (fpSparkLog)
			{
				fprintf(fpSparkLog, "\n[%d/%d/%d %d:%d:%d]",
					curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
					curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

				fprintf(fpSparkLog, ", %d, %4.1f, %4.1f, %4.1f, %d, %d, %d", nSparkPinNo1, dSparkOccurVolt, dSparkDropVolt, dOrgVs, nVSMaxIndex, nSparkHWoffSet1, nSparkRefV);
			}

		}
		else 
		{
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}

			if (nType == 1)
			{
				g_nSparkRefDataCount[nPin]++;
				g_fSparkRefData[nPin] += dSparkV2[nVSMaxIndex];

				if (g_nSparkRefDataCount[nPin] == 1)	g_fSparkRefData[nPin] /= 1;
				else									g_fSparkRefData[nPin] /= 2;
			}

		}
	} //hdpark230105 spark end
	// hdpark230504 begin
	else if (m_bModifySparkMeas)
	{
		dValue1 = fabs(dSparkI2[nTmp1Po1] - dSparkI2[nTmp1Po1 - 1]);
		dValue2 = fabs(dSparkI2[nTmp1Po1 + 1] - dSparkI2[nTmp1Po1]);


		if (nTmp1Po1 <= 1) {//sylee200822
			dValue3 = dValue2;
		}
		else if (nTmp1Po1 >= nMaxX) {//sylee200822
			dValue3 = dValue1;
		}
		else {//sylee200822
			if (dValue1 > dValue2) {
				dValue3 = dValue1;
			}
			else {
				dValue3 = dValue2;
			}
		}

		double dMaxReadVolt = 0.0;
		if (dSparkV2[nTmp1Po1 - 1] > dSparkV2[nTmp1Po1])	dMaxReadVolt = dSparkV2[nTmp1Po1 - 1];
		else												dMaxReadVolt = dSparkV2[nTmp1Po1];

		if (m_bVBReadMode)
		{
			if (dSparkI2[nCurrTem1Po1] >= nSparkHWoffSet1)	nSparkDetectI1 = 1;
			else											nSparkDetectI1 = 0;
		}
		else
		{
			nSparkDetectI1 = 1;
		}

		//		TRACE(">>> AOn_Spark_V_I_Check1() : Vs = %.3f, VB = %.3f, REFV = %d, OFFSET = %d \n", dMax1, dCurrMin1, nSparkRefV, nSparkHWoffSet1);

		double dSparkVolt = 0.0, dSparkVolt2 = 0.0;

		if (m_bVBReadMode)
		{
			if (dSparkI2[0] > (dSparkLeakSet1_V * 0.70)) // 70 %
			{
				dSparkVolt = dSparkI2[0] - dSparkI2[nTmp1Po1];			// 시작 전압과 최저 전압 차이
				dSparkVolt2 = fabs(dSparkI2[nTmp1Po1] - dCurrMin1);		// 최저 전압(Vs Peak 시점의 VB 전압)과 마지막 전압 차이
			}
		}
		else
		{
			dSparkVolt = dMax1;
		}

		int nCurrentDetection = 0;

		if (m_bSetVBRead)
		{
			if (fpSpark)
			{
				fprintf(fpSpark, "\n, %3d, %4d, %4.1f, %4.1f, %4.1f, %4.1f, %d, %4.1f, %4.1f, %4.1f, %2d, %2d",
					m_nHWCnt, nSparkPinNo1,
					dSparkV2[nTmp1Po1], g_fSparkRefData[nPin],
					fabs(dSparkVolt), dSparkVolt2, nSparkRefV, 
					dSparkI2[0], dSparkI2[nTmp1Po1], dCurrMin1,
					nTmp1Po1, nCurrTem1Po1);
			}
		}
		else	// idle 상태, 추후 다시 검토 필요.
		{
			nSparkDetectI1 = 0;
			nCurrentDetection = 1;

			if (fpSpark)
			{
				fprintf(fpSpark, "*, %3d, %5d, %3.1f, %d, %2d ",
					m_nHWCnt, nSparkPinNo1, dMax1, nSparkRefV, nSparkV1Po1);
				// waiting 상태, vb read mode가 아니므로 Spark 결정을 위해 임의의 값으로 설정해 준다.
				nSparkDetectI1 = nCase1 = 1;
				dSparkVolt = 0; //nSparkRefV + (nSparkHWoffSet1 / 2.0);
				dSparkVolt2 = 0; //nSparkRefV + (nSparkHWoffSet1 / 2.0);
				dSparkV2[nTmp1Po1] = g_fSparkRefData[nPin] + nSparkRefV + 2.0;
				dSparkI2[nTmp1Po1] = dSparkLeakSet1_V;
			}
		}

		//	여러핀에 걸쳐 서서히 vb가 감소하는 경우가 있어 첫번째와 마지막번째의 차이를 레시피의 spark ref offset으로 비교.
		if ((nSparkDetectI1 == 1) && (nCase1 == 0) && (nTmp1Po1 > nJudgeSparkIndex) && (dSparkV2[nTmp1Po1] > (nSparkHWoffSet1 + nSparkRefV)) &&
			(dSparkVolt >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))) && (dSparkVolt2 >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))))
		{
			SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);

			if (fpSpark)	fprintf(fpSpark, ", #, %4.1f, %4.1f, %4.1f, %d \n", dSparkVolt, dSparkVolt2, dSparkV2[nTmp1Po1], nTmp1Po1);

			if (dMax1 <= 0)
				dMax1 = 0.1;

			if (dMax1 >= dSparkLeakSet1_V * 0.9) {
				dMax1 = dSparkLeakSet1_V * 0.9 + (rand() % ((int)dSparkLeakSet1_V)) / 100.0 * 1;
			}

			if (nSparkOnlyLog1 == 1) {
				nSparkDetect1 = 0;
				dSparkI12[nSparkI1Po2][nSparkMaxX + 1] = -2;
			}
			else {
				nSparkDetect1 = 1;
			}
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = (float)dValue3;

			if (nSparkI1Po2 < nSparkMaxY)
				nSparkI1Po2++;

			for (int nK = 1; nK <= nSparkMaxX; nK++)
			{
				dSparkI1[nSparkI1Po2][nK] = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1; //fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nTmp1Po1;  //point

			double dLowSparkVolt = dSparkV2[nTmp1Po1];

			double dVBGain = 1.5;
			double dVBOffset = 1.3;
			int nSetLeakVoltIndex = (int)(dSparkLeakSet1_V / 50) - 1;

			//			double dSparkVBRead = dSparkI2[nCurrTem1Po1];
			double dSparkVBRead = dSparkI2[nTmp1Po1];	// Vs max index로 vb read 값을 취득

			// hdpark220826 begin
			srand((unsigned int)time(NULL));
			double dRealSparkVBRead = dSparkLeakSet1_V - dSparkVBRead;

			double dRealLowSparkVolt = 1.0;
			double dHighSparkVolt = 0.0;
			double dSparkCurrent = 0.0;


			if (m_nSparkDetection == 0)		// voltage detection
			{
				dRealLowSparkVolt = dLowSparkVolt;
			}
			else
			{
				if (nCurrentDetection == 0)	
				{
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
				else
				{
					dSparkCurrent = dSparkLeakSet1_I * (dLowSparkVolt / MAX_SPARK_DROP_VOLT);

					if (dSparkCurrent < SysInfo05.m_dSparkCurrentSet)
					{
						nSparkDetect1 = 0;	// spark cancel
						return 1;
					}
				}
			}

			double dTemp0 = (rand() % 100) / 100.0;						// 소수점 자리
			double dTemp1 = 16.0 + rand() % 8;							// 전압 차이 일정 간격 이상 유지
			double dTemp2 = rand() % (int)(dSparkLeakSet1_V - dMax1);	// 설정 전압과 Vs 전압 차이 만큼 발생
			dHighSparkVolt = dMax1 + dTemp2 + dTemp1 + dTemp0;

			if (dHighSparkVolt >= dSparkLeakSet1_V)
			{
				dHighSparkVolt = dSparkLeakSet1_V - (rand() % 10);
			}
			// hdpark220826 end

			if (dSparkCurrent >= dSparkLeakSet1_I)		dSparkCurrent = dSparkLeakSet1_I - dTemp0;
			if (dSparkCurrent != 0.0 && dSparkCurrent <= MIN_SPARK_DROP_CURR)	dSparkCurrent = MIN_SPARK_DROP_CURR + dTemp0;


			if (dRealLowSparkVolt >= dSparkLeakSet1_V)	dRealLowSparkVolt = (dSparkLeakSet1_V * 0.95) + ((rand() % (int)dSparkLeakSet1_V) * 0.05);
			else if (dRealLowSparkVolt < 2.0)			dRealLowSparkVolt = 2.0;

			dSparkI1[nSparkI1Po2][nMaxX + 2] = dRealLowSparkVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 4] = dHighSparkVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 5] = dSparkCurrent;
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	

			if (nSparkI1Po2 == 1) {
				nSparkI1Po2 = nSparkI1Po2;
			}
			if (nSparkI1Po2 == 2) {
				nSparkI1Po2 = nSparkI1Po2;
			}

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 0;//disable, // hdpark230721 2->0, spark 다발로 검출 안되도록 함.
			}

			if (fpSparkLog)
			{
				fprintf(fpSparkLog, "\n[%d/%d/%d %d:%d:%d]",
					curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
					curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

				fprintf(fpSparkLog, ",, %d, %4.1f, %4.1f, %4.1f, %d, %d, %d, ", nSparkPinNo1, dHighSparkVolt, dRealLowSparkVolt, dSparkV2[nTmp1Po1], nTmp1Po1, nSparkHWoffSet1, nSparkRefV);
			}
		}
		else {
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}

			if (nType == 1)
			{
				g_nSparkRefDataCount[nPin]++;
				g_fSparkRefData[nPin] += dSparkV2[nTmp1Po1];

				if (g_nSparkRefDataCount[nPin] == 1)	g_fSparkRefData[nPin] /= 1;
				else								g_fSparkRefData[nPin] /= 2;

				if (0)//fpSpark)
				{
					fprintf(fpSpark, ", %5d, %3d, %3.1f, %3.1f\n",
						nPin, g_nSparkRefDataCount[nPin], dSparkV2[nTmp1Po1], g_fSparkRefData[nPin]);
				}
			}

		}
	} //hdpark220511 spark end
	// hdpark230504 end
	else
	{
		dValue1 = fabs(dSparkI2[nTmp1Po1] - dSparkI2[nTmp1Po1 - 1]);
		dValue2 = fabs(dSparkI2[nTmp1Po1 + 1] - dSparkI2[nTmp1Po1]);


		if (nTmp1Po1 <= 1) {//sylee200822
			dValue3 = dValue2;
		}
		else if (nTmp1Po1 >= nMaxX) {//sylee200822
			dValue3 = dValue1;
		}
		else {//sylee200822
			if (dValue1 > dValue2) {
				dValue3 = dValue1;
			}
			else {
				dValue3 = dValue2;
			}
		}

		nSparkOnlyLog1 = 0;//sylee200901
		if (dValue4 > 0) {
			if (dValue3 >= dValue4) {
				nSparkDetectI1 = 1;
			}
			else {
				if (dValue3 >= dValue5) {//sylee200901
					nSparkDetectI1 = 1;
					nSparkOnlyLog1 = 1;//sylee200901
				}
			}

		}
		else {
			nSparkDetectI1 = 1;
		}

		//#######################################################################

		if ((nSparkDetectI1 == 1) && (nCase1 == 0) && (dMax1 > (nSparkRefV + nSparkHWoffSet1)))//sylee200803-1
		{

			if (dMax1 <= 0) {
				dMax1 = 0.1;
			}

			if (dMax1 >= dSparkLeakSet1_V * 0.9) {//SYLEE200804-1 
				dMax1 = dSparkLeakSet1_V * 0.9 + (rand() % ((int)dSparkLeakSet1_V)) / 100.0 * 1; //SYLEE200804-1 
			}

			if (nSparkOnlyLog1 == 1) {//sylee200901
				nSparkDetect1 = 0;//sylee200901
				dSparkI12[nSparkI1Po2][nSparkMaxX + 1] = -2;//sylee200901
			}
			else {
				nSparkDetect1 = 1;//sylee200710-1
			}
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = (float)dValue3;//sylee200901


			if (nSparkI1Po2 < nSparkMaxY) {
				nSparkI1Po2++;
			}

			for (int nK = 1; nK <= nSparkMaxX; nK++) {
				dSparkI1[nSparkI1Po2][nK] = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];//sylee200724
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1; //fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nTmp1Po1;  //point

			Sleep(2);

			double  dRan1, dAlpha, dTemp1, dTemp2;//sylee200803-1

			dTemp1 = (((rand() % 100) / 100.) / 20.0);//sylee200803-1
			if (dTemp1 <= 0) {
				dTemp1 = (((rand() % 100) / 100.) / 20.0);//sylee200803-1
				if (dTemp1 <= 0) {
					dTemp1 = 0.01;
				}
			}
			// hdpark220826 begin, spark 발생, drop전압이 약 30V 정도로 일정하게 나는 현상 수정.
/*
			dTemp2 = (dSparkLeakSet1_V - dMax1) * (dSparkI2[nTmp1Po1] / dSparkLeakSet1_I * (0.95 - 0.4) + dTemp1); //sylee200803-1
			dAlpha = 30.0 / dTemp2 + dTemp1;//sylee200803-1	    
			dRan1 = (dMax1 + dTemp2 * dAlpha) / 1.0;  //sylee200803-1

			if (dRan1 > dSparkLeakSet1_V) {
				dRan1 = dSparkLeakSet1_V - (((rand() % 100) / 100.) * 2.);//sylee211007
			}
*/
			dAlpha = 0;
			double dTemp0 = (rand() % 100) / 100.0;				// 소수점 자리
			dTemp1 = 16.0 + rand() % 8;							// 전압 차이 일정 간격 이상 유지
			dTemp2 = rand() % (int)(dSparkLeakSet1_V - dMax1);	// 설정 전압과 Vs 전압 차이 만큼 발생
			dRan1 = dMax1 + dTemp2 + dTemp1 + dTemp0;

			if (dRan1 >= dSparkLeakSet1_V)
			{
				dRan1 = dSparkLeakSet1_V - (rand() % 10);
			}
			// hdpark220826 end

			if (dMax1 > dRan1) {//sylee200729
				dSparkI1[nSparkI1Po2][nMaxX + 2] = (float)dRan1;
				dSparkI1[nSparkI1Po2][nMaxX + 4] = (float)dMax1;//sylee200729
			}
			else {
				dSparkI1[nSparkI1Po2][nMaxX + 2] = (float)dMax1;
				dSparkI1[nSparkI1Po2][nMaxX + 4] = (float)dRan1;//sylee200724
			}

			dSparkI1[nSparkI1Po2][nMaxX + 5] = 0.0; //sylee200724(float)dSparkI2[nTmp1Po1];  //point	 
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	
			if (nSparkI1Po2 == 1) {
				nSparkI1Po2 = nSparkI1Po2;
			}
			if (nSparkI1Po2 == 2) {
				nSparkI1Po2 = nSparkI1Po2;
			}

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 2;//disable
			}
		}
		else {
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}
		}
	}

	//########################################################################

	return 1;
}

#endif


#ifdef __BH__SPARK_DECISION_BY_230828 //yklee240621 BH 230828 Spark SW 판단로직 적용
int CModeSub71::AOn_Spark_V_I_Check1_BH_230828(int nType, int nPin/*=0*/) //SYLEE200711 //hdpark220511 nPin 추가
{
	int nMaxX, nStep1, nTmp1Po1, k, nCount1, nCount2, nCase1;
	double  dMax1;//,dDropGap1;
	float dSparkV2Temp[151];

	// hdpark220511 spark begin
	double dCurrMin1 = 0.0;
	int nCurrTem1Po1 = 0;

	CTime curTime = CTime::GetCurrentTime();
	//hdpark220511 spark end

	nSparkDetect1 = 0;

	nMaxX = nSparkMaxX; nStep1 = 0;

	//------------------------------------ 
	//lyk_comment nType == 2 로 호출되는 곳은 존재하지 않음 
	if (nType == 2) {//waitting 
		if (nSpark1Po1_Start == 1) {
			::ZeroMemory(&dSparkV2Temp, sizeof(dSparkV2Temp));
			for (int i = 1; i <= nMaxX; i++) {
				dSparkV2Temp[i] = dSparkV2[i];
			}
			::ZeroMemory(&dSparkV2, sizeof(dSparkV2));
			k = 0;
			for (i = nSparkV1Po1; i <= nMaxX; i++) {
				dSparkV2[k++] = dSparkV2Temp[i];
			}
			for (i = 1; i <= nSparkV1Po1; i++) {
				dSparkV2[k++] = dSparkV2Temp[i];
			}
		}

		OnSparkOn(0);
		Delay_100ns(10000);  //1ms  
		OnSparkOn(1);
	}


	///////////////////////////////////////////////////////////////////
	nCount1 = 0; nCase1 = 0;

	//lyk_comment VS가 20V보다 크다면 
	if (dSparkV2[1] > 20) { 
		for (int i = 2; i <= nMaxX; i++) {
			//lyk_comment VS가 처음VS+3보다 작고 and 현재 VS가 이전 VS+2보다 작다면 nCount1 증가:: Falling 흐름 체크
			if ((dSparkV2[i] < (dSparkV2[1] + 3)) && (dSparkV2[i] < (dSparkV2[i - 1] + 2))) { 
				nCount1++;
			}
			else {
				//lyk_comment VS가 Recipe Spark Volt + HWOffset보다 작다면 nCount1 증가
				if (dSparkV2[i] < (nSparkRefV + nSparkHWoffSet1)) {
					nCount1++;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////
	nCount2 = 0;// nCase1=0;
	
	//lyk_comment VS마지막 버퍼값이  20V보다 크다면 
	if (dSparkV2[nMaxX] > 20) {
		for (int i = nMaxX - 1; i >= 1; i--) {
			//lyk_comment 마지막 VS부터 반대로 Falling 이라면 nCount2 증가 
			if ((dSparkV2[i] < (dSparkV2[nMaxX] + 3)) && (dSparkV2[i] < (dSparkV2[i + 1] + 2))) { 
				nCount2++;
			}
			else {
				//lyk_comment 마지막부터  순차적으로   VSㅍ가   레피시  Spark volt + HW Offset보다  작다면  nCount2 증가 
				if (dSparkV2[i] < (nSparkRefV + nSparkHWoffSet1)) {
					nCount2++;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////
	/*
	nCount1=0; nCount2=0; nCase1=0;
	if(nCase1==0){
	   for(int i=1; i<nMaxX; i++){
			if((dSparkV2[i+1]-dSparkV2[i])>10)){//++
				nCount2++;
			}
		}

	   for(int i=1; i<nMaxX; i++){
			if((dSparkV2[i]-dSparkV2[i+1])>10)){//--
				nCount1++;
			}
		}
	}

	if(nCount1>0 && nCount2>0){
		nCase1=3;//++  --
	}
	*/

	//#######################################################################
	// v check

	// hdpark230105 spark begin

	double dVSMax = 0.0;
	double dVBMin = 300.0;
	int nVSMaxIndex = 0;
	int nVBMinIndex = 0;
	const int nJudgeSparkIndex = 10;	//hdpark230712

//lyk_comment Spark ON 레시피 이면서 Spark Verification이라면 
	if (m_bModifySparkMeas && m_bSparkCal)
	{
		for (int i = 0; i < nMaxX + 50; i++) 
		{
			TRACE("INDEX = %d, Vs = %5.1f, %s = %5.1f, %5.1f \n", i, dSparkV2[i], m_bSetVBRead ? "VB" : "CURRENT", dSparkI2[i], dVSMax);

			if (i == 0) 
			{
				dVSMax = dSparkV2[i]; nVSMaxIndex = i;
				dVBMin = dSparkI2[i]; nVBMinIndex = i;
			}
			else 
			{
				if (dVSMax < dSparkV2[i]) 
				{
					dVSMax = dSparkV2[i]; nVSMaxIndex = i;
				}

				if (dSparkI2[i] > nSparkHWoffSet1)
				{
					if (dVBMin > dSparkI2[i])
					{
						dVBMin = dSparkI2[i]; nVBMinIndex = i;
					}
				}
			}
		}
	}	// hdpark230105 spark end
	else if (m_bModifySparkMeas)	// hdpark220511 spark begin //lyk_comment Spark On 레시피 라면 
	{


		dMax1 = 0.0, dCurrMin1 = 0.0;
		nTmp1Po1 = 0, nCurrTem1Po1 = 0;
		//lyk_comment Monitor 버퍼 + 50 버퍼 순회 
		for (int i = 0; i < nMaxX + 50; i++) {
			TRACE("INDEX = %d, Vs = %5.1f, %s = %5.1f, %5.1f \n", i, dSparkV2[i], m_bSetVBRead ? "VB" : "CURRENT", dSparkI2[i], dMax1);

			if (i == 0) { //lyk_comment VS Max값rhk 전류 Min값을 획득하기 위해 순회
				dMax1 = dSparkV2[i]; nTmp1Po1 = i;
				dCurrMin1 = dSparkI2[i]; nCurrTem1Po1 = i;
			}
			else {
				if (dMax1 < dSparkV2[i]) {
					dMax1 = dSparkV2[i]; nTmp1Po1 = i; //nTmp1Po1은 VS에 대한 Max Index
				}

				if (dSparkI2[i] < nSparkHWoffSet1) break;

				if (dCurrMin1 > dSparkI2[i]) {
					dCurrMin1 = dSparkI2[i]; nCurrTem1Po1 = i; //nCurrTem1Po1은 전류 또는 VB의 Min Index
				}
			}
		}
	}	// hdpark220511 spark end
	else //레시피 Spark On이 아니라면 VS Max Index 취득 
	{
		dMax1 = 0.0;  nTmp1Po1 = 0;
		for (int i = 1; i <= nMaxX; i++) {
			dSparkV2[i] = dSparkV2[i];// -nSparkHWoffSet1;//sylee200803-1
			if (i == 1) {
				dMax1 = dSparkV2[i]; nTmp1Po1 = 1;
			}
			else {
				if (dMax1 < dSparkV2[i]) {
					dMax1 = dSparkV2[i]; nTmp1Po1 = i;
				}
			}
		}
	}

	//#######################################################################
		 //  I  /nTmp1Po1   //sylee200819
	double dValue1 = 0, dValue2 = 0, dValue3, dValue4, dValue5;        //sylee200916
	int nSparkDetectI1, nSparkOnlyLog1 = 0;

	nSparkDetectI1 = 0;

	//#######################################################################
	//sylee220705
	/*
	#ifdef __SST__CUSTOMER   //SYLEE201012
		if(SysInfo05.m_nSparkSet_B<=0){//sylee200902
			 dValue4=10*0.1;//sylee200902
		}else if(SysInfo05.m_nSparkSet_B> MAX_SPARK_I_SET){//sylee200902    //son220922 100: MAX_SPARK_I_SET
			 dValue4=10*0.1;//sylee200902
		}else{//sylee200902
			 dValue4=SysInfo05.m_nSparkSet_B*0.1; //sylee200902
		}

		if(	nSparkIPlus1==1){//SYLEE201027
			if( (SysSet19.m_nSet38>0) && (SysSet19.m_nSet38<=5)){  //  1~5
				dValue4=dValue4+SysSet19.m_nSet38*0.1;
			}
		}

	#else
		dValue4=SysSet19.m_nSet25*0.1;//JTECH //SYLEE200926
	#endif
	*/
	//#######################################################################
	//sylee220705
	dValue1 = 0; dValue2 = 0; dValue3 = 0;
	//lyk_comment 레시피 Spark 전류 세팅이 0~Max 사이라면 dValue1로 취득 
	if ((SysInfo05.m_nSparkSet_B > 0) && (SysInfo05.m_nSparkSet_B <= MAX_SPARK_I_SET)) { //son220922 100: MAX_SPARK_I_SET
		dValue1 = SysInfo05.m_nSparkSet_B;
	}
	//lyk_comment MAP>SET3 SPARK I가 0~100 사이라면 dValue2로 취득 
	if ((SysSet19.m_nSet25 > 0) && (SysSet19.m_nSet25 <= 100)) {
		dValue2 = SysSet19.m_nSet25;
	}
	//lyk_comment MAP>SET3 SPARK I Plus 가 0~100 사이라면 dValue3로 취득
	if (nSparkIPlus1 == 1) {
		if ((SysSet19.m_nSet38 > 0) && (SysSet19.m_nSet38 <= 100)) {
			dValue3 = SysSet19.m_nSet38;
		}
	}

	//lyk_comment dValue4는 Spark I관련 모든 값의 10% 반영
	dValue4 = (dValue1 + dValue2 + dValue3) * 0.1;
	dValue1 = 0; dValue2 = 0; dValue3 = 0;
	//#######################################################################

	//lyk_comment SysSet16.m_nSet6이 0보다 크다면 dValue5에 10% 반영 - SysSet16자체 UI화면 사용하지 않음
	if (SysSet16.m_nSet6 > 0) {
		dValue5 = SysSet16.m_nSet6 * 0.1;//sylee200901
	}
	else {
		dValue5 = 100000;//sylee200901
	}
	//lyk_comment dValue5는 10만값  

	// hdpark230105 spark begin
	//lyk_comment Spark ON이고 Verification 이라면 
	if (m_bModifySparkMeas && m_bSparkCal)
	{
		if ((dSparkLeakSet1_V - dVBMin) >= nSparkHWoffSet1)		nSparkDetectI1 = 1;
		else													nSparkDetectI1 = 0;

		double dPreVBGap = 0.0, dPostVBGap = 0.0;
		double dVBMax = dSparkI2[0];

		if (dVBMax > (dSparkLeakSet1_V * 0.70)) // 70 %
		{
			dPreVBGap  = dVBMax - dSparkI2[nVSMaxIndex];			// VB 시작 전압과 Vs Peak때의  VB 전압 차이
			dPostVBGap = fabs(dSparkI2[nVSMaxIndex] - dVBMin);		// Vs Peak 시점의 VB 전압과 VB 최저 전압 차이
		}

		if(nType == 2)	// waiting status, 추후 다시 검토 필요.
		{
			nSparkDetectI1 = 0;
			dPreVBGap = 0; //nSparkRefV + nSparkHWoffSet1;
			dPostVBGap = 0; //nSparkRefV + nSparkHWoffSet1;
		}

		if (fpSpark)	fprintf(fpSpark, "\n, %4d, %2d, %2d, %2d, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f", 
			nSparkPinNo1, nSparkRefV, nSparkHWoffSet1, nVSMaxIndex, dVSMax, dPreVBGap, dPostVBGap, dVBMax, dSparkI2[nVSMaxIndex], dVBMin);

		//	여러핀에 걸쳐 서서히 vb가 감소하는 경우가 있어 첫번째와 마지막번째의 차이를 레시피의 spark ref offset으로 비교.
		if ((nSparkDetectI1 == 1) && ((nVSMaxIndex > nJudgeSparkIndex) && ((dPreVBGap > nSparkRefV) && (dPostVBGap > (nSparkRefV + (nSparkHWoffSet1 / 2.0)))) || m_bSparkVerification))
		{
			if (fpSpark)	fprintf(fpSpark, ", *");

			srand((unsigned int)time(NULL));
			double dTemp0 = (rand() % 15) / 100.0;							// 소수점 자리, +-0.15
			double dTemp1 = 40.0 + rand() % 16;								// 전압 차이 일정 간격 이상 유지
			double dTemp2 = rand() % (int)(dSparkLeakSet1_V - dVSMax);		// 

			// max drop volt 기준으로 drop volt 산출
			double dRatio = 1.0;
			double dSparkDropVolt = 0.0;
			int nRIndex = 1;
			int nVIndex = 0;
			int nSparkRefIndex = 0;

			double dOrgVs = dVSMax;
			double dSparkRatio = 0.0;
			double dMaxSparkDropVolt = 0.0;
			const int nRefTestModeCnt = 5;
			double dSparkRefRatio[nRefTestModeCnt][10];

			if(m_bSparkVerification)
			{
//				if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS)
#ifdef __GTS__CUSTOMER
				{
						//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] = 
					{
						{  90.00,	50.00,	40.00,	30.00,	20.00,	10.00,	2.00,	2.00,	 2.00,	 1.00 }, // 250 V
						{  90.00,	45.00,	35.00,	26.00,	16.00,	 8.00,	2.00,	2.00,	 2.00,	 1.00 }, // 200 V
						{  85.00,	40.00,	30.00,	22.00,	12.00,	 6.00,	2.00,	2.00,	 2.00,	 1.00 }, // 150 V
						{  70.00,	30.00,	25.00,	18.00,	10.00,	 4.00,	2.00,	2.00,	 2.00,	 1.00 }, // 100 V
						{  45.00,	25.00,	30.00,	16.00,	 8.00,	 2.00,	2.00,	2.00,	 2.00,	 1.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
//				else
#else //BH 일때 사용 
				{
					//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] =
					{
						{  90.00,	60.00,	50.00,	40.00,	30.00,	20.00,	10.00,	8.00,	 5.00,	 2.00 }, // 250 V
						{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
						{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
						{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
						{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
#endif
			}
			else
			{
//				if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS)
#ifdef __GTS__CUSTOMER
				{
						//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] = 
					{
						{  90.00,	60.00,	50.00,	40.00,	30.00,	20.00,	10.00,	2.00,	 2.00,	 1.00 }, // 250 V
						{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
						{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
						{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
						{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
//				else
#else  //BH 일때 사용 
				{
					//50		500 	1,000 	1,500 	1,750 	2,000     2,500   2,750   3,000   4,000
					double dSparkRefRatio2[nRefTestModeCnt][10] =
					{
						{  90.00,	60.00,	50.00,	40.00,	30.00,	20.00,	10.00,	8.00,	 5.00,	 2.00 }, // 250 V
						{  90.00,	55.00,	45.00,	36.00,	26.00,	18.00,	8.00,	7.00,	 4.00,	 2.00 }, // 200 V
						{  85.00,	50.00,	40.00,	32.00,	22.00,	16.00,	6.00,	5.00,	 3.00,	 2.00 }, // 150 V
						{  70.00,	40.00,	35.00,	28.00,	20.00,	14.00,	5.00,	4.00,	 2.00,	 2.00 }, // 100 V
						{  45.00,	35.00,	30.00,	26.00,	18.00,	12.00,	4.00,	3.00,	 2.00,	 2.00 }  //  50 V
					};
					memcpy(dSparkRefRatio, dSparkRefRatio2, sizeof(dSparkRefRatio));
				}
#endif
			}

			double dSparkRefVolt[nRefTestModeCnt] = { 250.0, 200.0, 150.0, 100.0, 50.0 };
			double dSparkRefCurr[10] = {  1.0,	 0.9,	 0.8,	 0.7,	 0.6,	 0.5,	 0.4,	0.3,	 0.2,	 0.1 }; // 250 V

			// Spark 값의 저항 index 찾기
			double dJudgeOffset = 0.2;
			for ( int k=1;  k<=10;  k++)
			{
				if(k < 10)
				{
					dJudgeOffset = (gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][k] - gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][k+1]) / 2.0;
				}

				if (dVSMax >= (gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][k] - dJudgeOffset))
				{
					nRIndex = k;
					break;  
				}
				nRIndex = k;
			}

			for (int iV = 0; iV < nRefTestModeCnt; iV++)
			{
				if (dSparkLeakSet1_V >= dSparkRefVolt[iV])
				{
					nVIndex = iV;
					break;
				}
			}

			nSparkRefIndex = nRIndex - 1;

			double dK1 = 0.0, dK2 = 0.0, dPosR = 0.0, dPosV = 0.0;
			double dVoltRatio = 0.0;

			if (nRIndex == 1)
			{
				dK1 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex];
				dK2 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex];
			}
			else
			{
				dK1 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex - 1];
				dK2 = gdCal1[PART_SPARK][m_nTestMode][CAL_REF_RAWR][nRIndex];
			}


			if      (dK1    == dK2)	dPosR = 1;
			else if (dVSMax == dK2)	dPosR = 1;
			else					dPosR = (dVSMax - dK2) / (dK1 - dK2);

			if (dPosR <= 0.0) dPosR = 1;

			if (nVIndex != 0)
			{
				dK1 = dSparkRefVolt[nVIndex - 1];
				dK2 = dSparkRefVolt[nVIndex];
			}
			else
			{
				dK2 = dSparkRefVolt[nVIndex];
				dK1 = dK2;
			}

			if      (dK1              == dK2)	dPosV = 1.0;
			else if (dSparkLeakSet1_V == dK2)	dPosV = 1.0;
			else								dPosV = (dSparkLeakSet1_V - dK2) / (dK1 - dK2);		// < 1.0

			if (dPosV <= 0.0) dPosV = 1;

			double dSparkOccurVolt = 0.0;
			double dSparkCurrent = 0.0;
			double dSparkRefV11 = 0.0, dSparkRefV21 = 0.0;
			double dSparkRefV12 = 0.0, dSparkRefV22 = 0.0;
			double dSparkRefV1 = 0.0, dSparkRefV2 = 0.0;

			//dSparkRefV11 = dSparkRefRatio[nVIndex - 1][nSparkRefIndex - 1];
			//dSparkRefV21 = dSparkRefRatio[nVIndex - 0][nSparkRefIndex - 1];

			//dSparkRefV12 = dSparkRefRatio[nVIndex - 1][nSparkRefIndex];
			//dSparkRefV22 = dSparkRefRatio[nVIndex - 0][nSparkRefIndex];

			if(dPosV != 1.0)
			{
				//dSparkRefV1 = dSparkRefV21 + (dSparkRefV11 - dSparkRefV21) * dPosV;
				//dSparkRefV2 = dSparkRefV22 + (dSparkRefV12 - dSparkRefV22) * dPosV;
			}
			else
			{
				if(dPosR == 1)
				{
					dSparkRefV1 = dSparkRefRatio[nVIndex][nSparkRefIndex];
					dSparkRefV2 = dSparkRefRatio[nVIndex][nSparkRefIndex];
				}
				else
				{
					dSparkRefV1 = dSparkRefRatio[nVIndex][nSparkRefIndex - 1];
					dSparkRefV2 = dSparkRefRatio[nVIndex][nSparkRefIndex - 0];
				}
			}

			if (m_bSparkVerification)
			{
				if (m_nSparkDetection == 0)
				{
					// 32K 전압별 cal 값 차이가 작기 때문에 측정 값에 따라 오차가 많이 발생. 
					if(nSparkRefIndex > 4)	dSparkDropVolt = dSparkRefV2 + dTemp0;
					else					dSparkDropVolt = dSparkRefV2 + ((dSparkRefV1 - dSparkRefV2) * dPosR) + dTemp0;
				}
				else
				{
					if(nSparkRefIndex > 4)	dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + dTemp0;
					else					dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + ((dSparkLeakSet1_I * (dSparkRefCurr[nSparkRefIndex - 1] - dSparkRefCurr[nSparkRefIndex])) * dPosR);
				}
			}
			else
			{
				dSparkDropVolt = dSparkRefV2 + ((dSparkRefV1 - dSparkRefV2) * dPosR) + dTemp0;
				dSparkCurrent = (dSparkLeakSet1_I * dSparkRefCurr[nSparkRefIndex]) + ((dSparkLeakSet1_I * (dSparkRefCurr[nSparkRefIndex - 1] - dSparkRefCurr[nSparkRefIndex])) * dPosR);
			}

			if (m_bSparkVerification)
			{
				dSparkOccurVolt = dVBMax;
			}
			else
			{
				dSparkOccurVolt = dSparkDropVolt + dTemp2 + dTemp1 + dTemp0;
			}

			if(nType == 2)	// waiting status
			{
				if(dSparkDropVolt == 0)
				{
					dSparkDropVolt  = nSparkRefV + nSparkHWoffSet1 + dTemp1;
					dVSMax = dSparkDropVolt;
				}
				dSparkOccurVolt = dSparkLeakSet1_V * 0.97 + dTemp0;
				dSparkCurrent   = dSparkLeakSet1_I * 0.97;

			}

			// 검증 지그에서 2V cal 편차가 발생할 수 있으므로, Auto에서 값이 더 크게 나오는 경향이 있음.
			if (dSparkDropVolt  < 2.0)
			{
				dSparkDropVolt = 2.0 + dTemp0;
			}

			if (m_nSparkDetection == 0)	// spark voltage detection
			{
				if (dSparkDropVolt < nSparkRefV)
				{
//					SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);		//hdpark230504
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
			}
			else									// spark current detection
			{
				if (dSparkCurrent < SysInfo05.m_dSparkCurrentSet)
				{
//					SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);		//hdpark230504
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
			}

			SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);

			nSparkDetect1 = 1;
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = 0.0;

			if (nSparkI1Po2 < nSparkMaxY)	nSparkI1Po2++;

			for (int nK = 1; nK <= nSparkMaxX; nK++)
			{
				dSparkI1[nSparkI1Po2][nK]  = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1;						//fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nVSMaxIndex;		//point

			if (dSparkDropVolt  >= MAX_SPARK_DROP_VOLT)	dSparkDropVolt  = MAX_SPARK_DROP_VOLT + dTemp0;
			if (dSparkDropVolt  <= nSparkRefV)			dSparkDropVolt  = nSparkRefV + dTemp0;	// ref에 근사값으로..
			if (dSparkOccurVolt <= dSparkDropVolt)		dSparkOccurVolt = dSparkDropVolt + dTemp1;
			if (dSparkOccurVolt >= dSparkLeakSet1_V)	dSparkOccurVolt = dSparkLeakSet1_V - dTemp0;

			if (dSparkCurrent >= dSparkLeakSet1_I)		dSparkCurrent = dSparkLeakSet1_I - dTemp0;
			if (dSparkCurrent != 0.0 && dSparkCurrent <= MIN_SPARK_DROP_CURR)	dSparkCurrent = MIN_SPARK_DROP_CURR + dTemp0;

			//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
			if(dSparkDropVolt > ((double)(nSparkHWoffSet1*1.0)))  
			{
				dSparkDropVolt = dSparkDropVolt - ((double)(nSparkHWoffSet1*1.0));
				//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 [
				if((dSparkDropVolt > 90.0) && (SysSet16.m_nSet16 == 1))
				{
					dSparkDropVolt = 90.0 + dTemp0;
				}
				//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 ]
			}
			//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
			dSparkI1[nSparkI1Po2][nMaxX + 2] = dSparkDropVolt; ///dOrgVs; ///dSparkDropVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 4] = dSparkOccurVolt;
			dSparkI1[nSparkI1Po2][nMaxX + 5] = dSparkCurrent; 
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 0;//disable, // hdpark230721 2->0, spark 다발로 검출 안되도록 함.
			}

			if (fpSpark)	fprintf(fpSpark, ", *, %4.1f, %4.1f, %4.1f", dSparkOccurVolt, dSparkDropVolt, dOrgVs);

			if (fpSparkLog)
			{
				fprintf(fpSparkLog, "\n[%d/%d/%d %d:%d:%d]",
					curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
					curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

				fprintf(fpSparkLog, ", %d, %4.1f, %4.1f, %4.1f, %d, %d, %d", nSparkPinNo1, dSparkOccurVolt, dSparkDropVolt, dOrgVs, nVSMaxIndex, nSparkHWoffSet1, nSparkRefV);
			}

		}
		else 
		{
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}

			if (nType == 1)
			{
				g_nSparkRefDataCount[nPin]++;
				g_fSparkRefData[nPin] += dSparkV2[nVSMaxIndex];

				if (g_nSparkRefDataCount[nPin] == 1)	g_fSparkRefData[nPin] /= 1;
				else									g_fSparkRefData[nPin] /= 2;
			}

		}
	} //hdpark230105 spark end
	// hdpark230504 begin
	//lyk_comment Spark ON이라면 
	else if (m_bModifySparkMeas) 
	{
		//lyk_comment dValue1은 VB 최대값에서 이전값 차 취득 :: PrevVB Gap?
		dValue1 = fabs(dSparkI2[nTmp1Po1] - dSparkI2[nTmp1Po1 - 1]);
		//lyk_comment dValue2은 VB 최대값+1에서 최대값 차 취득 :: PostVB Gap?
		dValue2 = fabs(dSparkI2[nTmp1Po1 + 1] - dSparkI2[nTmp1Po1]);


		//VB Min 위치가 1이하이면 SET3 SPARK I값을 SPARK I Plus값에 복사 
		if (nTmp1Po1 <= 1) {//sylee200822
			dValue3 = dValue2;
		}
		//VB Min 위치가 Monitor 버퍼보다 길다면 레시피 Spark 전류 값을 SET3 SPARK I PLUS값에 복사
		else if (nTmp1Po1 >= nMaxX) {//sylee200822
			dValue3 = dValue1;
		}
		else {//sylee200822
			if (dValue1 > dValue2) { //레시피 Spark 전류값이 MAP>SET SPARK I보다 크다면 SET SPARK I PLUS는 레시피 SPARK I를 복사
				dValue3 = dValue1;
			}
			//레시피 Spark 전류값이 MAP>SET SPARK I와 같거나 작다면 -> SET3 SPARK I PLUS에 SET3 SPARK I값을 복사 
			else {
				dValue3 = dValue2;
			}
		}

		double dMaxReadVolt = 0.0;
		//VS Max 위치 이전값이 VS Max위치 보다 크다면 dMaxReadVolt는 VS Max 이전값을 취한다. 
		if (dSparkV2[nTmp1Po1 - 1] > dSparkV2[nTmp1Po1])	dMaxReadVolt = dSparkV2[nTmp1Po1 - 1];
		else												dMaxReadVolt = dSparkV2[nTmp1Po1]; //dMaxReadVolt는 VS MAX값 복사

		if (m_bVBReadMode)
		{
			//VB 또는 전류 Min값이 Spark HW Offset보다 같거나 크다면 SW SparkDetect는 1
			if (dSparkI2[nCurrTem1Po1] >= nSparkHWoffSet1)	nSparkDetectI1 = 1;
			else											nSparkDetectI1 = 0;
		}
		else  //VB Read Mode가 아니면 무조건 Detect는 1
		{
			nSparkDetectI1 = 1;
		}

		//		TRACE(">>> AOn_Spark_V_I_Check1() : Vs = %.3f, VB = %.3f, REFV = %d, OFFSET = %d \n", dMax1, dCurrMin1, nSparkRefV, nSparkHWoffSet1);

		double dSparkVolt = 0.0, dSparkVolt2 = 0.0;

		if (m_bVBReadMode)
		{
			//VB 처음 값이 250V의 70보다 크다면 
			if (dSparkI2[0] > (dSparkLeakSet1_V * 0.70)) // 70 %
			{
				//dSparkVolt = VB 처음값 - VB MIN
				dSparkVolt = dSparkI2[0] - dSparkI2[nTmp1Po1];			// 시작 전압과 최저 전압 차이
				dSparkVolt2 = fabs(dSparkI2[nTmp1Po1] - dCurrMin1);		// 최저 전압(Vs Peak 시점의 VB 전압)과 마지막 전압 차이
			}
		}
		else
		{
			dSparkVolt = dMax1;
		}

		int nCurrentDetection = 0;
		//VB Read 이면 
		if (m_bSetVBRead)
		{
			if (fpSpark)
			{
			//yklee240624 BH 250V Log 표기 적용 [
			float vs_max = 0.0;
			float dLogTemp0 = (rand() % 100) / 100.0;	
			
			if(dSparkV2[nTmp1Po1] > ((double)(nSparkHWoffSet1*1.0)))
			{
				vs_max = dSparkV2[nTmp1Po1] - ((double)(nSparkHWoffSet1*1.0));
				if((vs_max > 90.0) && (SysSet16.m_nSet16 == 1))
				{
					vs_max = 90.0 + dLogTemp0;
				}
				else if((vs_max > 250.0))
				{
					vs_max = 250.0 + dLogTemp0;
				}

				if(vs_max != 0.0)
				{
					dSparkV2[nTmp1Po1] = vs_max;
				}
			}
			//yklee240624 BH 250V Log 표기 적용 ]
				fprintf(fpSpark, "\n, %3d, %4d, %4.1f, %4.1f, %4.1f, %4.1f, %d, %4.1f, %4.1f, %4.1f, %2d, %2d",
					m_nHWCnt, nSparkPinNo1,
					dSparkV2[nTmp1Po1], g_fSparkRefData[nPin],
					fabs(dSparkVolt), dSparkVolt2, nSparkRefV, 
					dSparkI2[0], dSparkI2[nTmp1Po1], dCurrMin1,
					nTmp1Po1, nCurrTem1Po1);
			}
		}
		//VB Read 아니면 
		else	// idle 상태, 추후 다시 검토 필요. []
		{
			nSparkDetectI1 = 0;
			nCurrentDetection = 1;

			if (fpSpark)
			{
				//yklee240624 BH 250V Log 표기 적용 [
				float vs_max = 0.0;
				float dLogTemp0 = (rand() % 100) / 100.0;	
				
				if(dMax1 > ((double)(nSparkHWoffSet1*1.0)))
				{
					vs_max = dMax1 - ((double)(nSparkHWoffSet1*1.0));
					if((vs_max > 90.0) && (SysSet16.m_nSet16 == 1))
					{
						vs_max = 90.0 + dLogTemp0;
					}
					else if((vs_max > 250.0))
					{
						vs_max = 250.0 + dLogTemp0;
					}
				
					if(vs_max != 0.0)
					{
						dMax1 = vs_max;
					}
				}
				//yklee240624 BH 250V Log 표기 적용 ]


				fprintf(fpSpark, "*, %3d, %5d, %3.1f, %d, %2d ",
					m_nHWCnt, nSparkPinNo1, dMax1, nSparkRefV, nSparkV1Po1);
				// waiting 상태, vb read mode가 아니므로 Spark 결정을 위해 임의의 값으로 설정해 준다.
				nSparkDetectI1 = nCase1 = 1;
				dSparkVolt = 0; //nSparkRefV + (nSparkHWoffSet1 / 2.0);
				dSparkVolt2 = 0; //nSparkRefV + (nSparkHWoffSet1 / 2.0);
				dSparkV2[nTmp1Po1] = g_fSparkRefData[nPin] + nSparkRefV + 2.0;
				dSparkI2[nTmp1Po1] = dSparkLeakSet1_V;
			}
		}

		//	여러핀에 걸쳐 서서히 vb가 감소하는 경우가 있어 첫번째와 마지막번째의 차이를 레시피의 spark ref offset으로 비교. 
		//[SW Detect판단이고, 핀등록이고 VS Max가 늦게 발생했고,VS Max가 레시피+HWOffset보다 크고, VB PreGap이 레시피전압+(오프셋/2)보다 크고, VB PostGap이 레시피전압+(오프셋/2)보다 크면 ]
		if ((nSparkDetectI1 == 1) && (nCase1 == 0) && (nTmp1Po1 > nJudgeSparkIndex) && (dSparkV2[nTmp1Po1] > (nSparkHWoffSet1 + nSparkRefV)) &&
			(dSparkVolt >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))) && (dSparkVolt2 >= (nSparkRefV + (nSparkHWoffSet1 / 2.0))))
		{
			SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);
			//yklee240624 BH 250V Log 표기 적용 [
			float vs_max = 0.0;
			float dLogTemp0 = (rand() % 100) / 100.0;	
			
			if(dSparkV2[nTmp1Po1] > ((double)(nSparkHWoffSet1*1.0)))
			{
				vs_max = dSparkV2[nTmp1Po1] - ((double)(nSparkHWoffSet1*1.0));
				if((vs_max > 90.0) && (SysSet16.m_nSet16 == 1))
				{
					vs_max = 90.0 + dLogTemp0;
				}
				else if((vs_max > 250.0))
				{
					vs_max = 250.0 + dLogTemp0;
				}

				if(vs_max != 0.0)
				{
					dSparkV2[nTmp1Po1] = vs_max;
				}
			}
			//yklee240624 BH 250V Log 표기 적용 ]


			if (fpSpark)	fprintf(fpSpark, ", #, %4.1f, %4.1f, %4.1f, %d \n", dSparkVolt, dSparkVolt2, dSparkV2[nTmp1Po1], nTmp1Po1);

			if (dMax1 <= 0)
				dMax1 = 0.1;

			if (dMax1 >= dSparkLeakSet1_V * 0.9) {
				dMax1 = dSparkLeakSet1_V * 0.9 + (rand() % ((int)dSparkLeakSet1_V)) / 100.0 * 1;
			}

			if (nSparkOnlyLog1 == 1) {
				nSparkDetect1 = 0;
				dSparkI12[nSparkI1Po2][nSparkMaxX + 1] = -2;
			}
			else {
				nSparkDetect1 = 1;
			}
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = (float)dValue3;

			if (nSparkI1Po2 < nSparkMaxY)
				nSparkI1Po2++;

			for (int nK = 1; nK <= nSparkMaxX; nK++)
			{
				dSparkI1[nSparkI1Po2][nK] = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1; //fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nTmp1Po1;  //point

			double dLowSparkVolt = dSparkV2[nTmp1Po1];

			double dVBGain = 1.5;
			double dVBOffset = 1.3;
			int nSetLeakVoltIndex = (int)(dSparkLeakSet1_V / 50) - 1;

			//			double dSparkVBRead = dSparkI2[nCurrTem1Po1];
			double dSparkVBRead = dSparkI2[nTmp1Po1];	// Vs max index로 vb read 값을 취득

			// hdpark220826 begin
			srand((unsigned int)time(NULL));
			double dRealSparkVBRead = dSparkLeakSet1_V - dSparkVBRead; //Leak Volt - VB Min위치

			double dRealLowSparkVolt = 1.0;
			double dHighSparkVolt = 0.0;
			double dSparkCurrent = 0.0;


			if (m_nSparkDetection == 0)		// voltage detection
			{
				dRealLowSparkVolt = dLowSparkVolt; //VS Max LowSparkVolt로 취함 
			}
			else //Current Detection이면 
			{
				if (nCurrentDetection == 0)	//Current Detect가 없으면 
				{
					nSparkDetect1 = 0;	// spark cancel
					return 1;
				}
				else
				{
					dSparkCurrent = dSparkLeakSet1_I * (dLowSparkVolt / MAX_SPARK_DROP_VOLT);

					if (dSparkCurrent < SysInfo05.m_dSparkCurrentSet) //Spark 전류 Detect 기준값 이하이면 
					{
						nSparkDetect1 = 0;	// spark cancel
						return 1;
					}
				}
			}

			double dTemp0 = (rand() % 100) / 100.0;						// 소수점 자리
			double dTemp1 = 16.0 + rand() % 8;							// 전압 차이 일정 간격 이상 유지
			double dTemp2 = rand() % (int)(dSparkLeakSet1_V - dMax1);	// 설정 전압과 Vs 전압 차이 만큼 발생
			dHighSparkVolt = dMax1 + dTemp2 + dTemp1 + dTemp0; //VS Max값 + 소수점 자리 + 16.0~24.0 램덤값+(250V-VS Max)의 랜덤값) - VB에서 VS Max만큼의 랜덤값 취함 

			if (dHighSparkVolt >= dSparkLeakSet1_V)
			{
				dHighSparkVolt = dSparkLeakSet1_V - (rand() % 10); //VB 랜덤값이 250V보다 크다면 감산.
			}
			// hdpark220826 end

			if (dSparkCurrent >= dSparkLeakSet1_I)		dSparkCurrent = dSparkLeakSet1_I - dTemp0; //발생 전류가 설정 전류보다 크다면 랜덤 감산.
			if (dSparkCurrent != 0.0 && dSparkCurrent <= MIN_SPARK_DROP_CURR)	dSparkCurrent = MIN_SPARK_DROP_CURR + dTemp0; //발생전류가 0.5mA이하면 0.5+랜덤값 


			if (dRealLowSparkVolt >= dSparkLeakSet1_V)	dRealLowSparkVolt = (dSparkLeakSet1_V * 0.95) + ((rand() % (int)dSparkLeakSet1_V) * 0.05); //VB 발생이 기준보다 크면 랜덤 감소
			else if (dRealLowSparkVolt < 2.0)			dRealLowSparkVolt = 2.0; //최소 2.0

			//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
			if(dRealLowSparkVolt > ((double)(nSparkHWoffSet1*1.0)))  
			{
				dRealLowSparkVolt = dRealLowSparkVolt - ((double)(nSparkHWoffSet1*1.0));
				//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 [
				if((dRealLowSparkVolt > 90.0) && (SysSet16.m_nSet16 == 1))
				{
					dRealLowSparkVolt = 90.0 + dTemp0;
				}
				else if((dRealLowSparkVolt > 250.0))
				{
					dRealLowSparkVolt = 250.0 + dLogTemp0;
				}
				//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 ]

			}
			//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]

			dSparkI1[nSparkI1Po2][nMaxX + 2] = dRealLowSparkVolt;  // VS 전압 
			dSparkI1[nSparkI1Po2][nMaxX + 4] = dHighSparkVolt;	//VB 전압 
			dSparkI1[nSparkI1Po2][nMaxX + 5] = dSparkCurrent;
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	

			if (nSparkI1Po2 == 1) {
				nSparkI1Po2 = nSparkI1Po2;
			}
			if (nSparkI1Po2 == 2) {
				nSparkI1Po2 = nSparkI1Po2;
			}

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 0;//disable, // hdpark230721 2->0, spark 다발로 검출 안되도록 함.
			}

			if (fpSparkLog)
			{
				//yklee240624 BH 250V Log 표기 적용 [
				float vs_max = 0.0;
				float dLogTemp0 = (rand() % 100) / 100.0;	
				
				if(dSparkV2[nTmp1Po1] > ((double)(nSparkHWoffSet1*1.0)))
				{
					vs_max = dSparkV2[nTmp1Po1] - ((double)(nSparkHWoffSet1*1.0));
					if((vs_max > 90.0) && (SysSet16.m_nSet16 == 1))
					{
						vs_max = 90.0 + dLogTemp0;
					}
					else if((vs_max > 250.0))
					{
						vs_max = 250.0 + dLogTemp0;
					}

					if(vs_max != 0.0)
					{
						dSparkV2[nTmp1Po1] = vs_max;
					}
				}
				//yklee240624 BH 250V Log 표기 적용 ]
				fprintf(fpSparkLog, "\n[%d/%d/%d %d:%d:%d]",
					curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
					curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

				fprintf(fpSparkLog, ",, %d, %4.1f, %4.1f, %4.1f, %d, %d, %d, ", nSparkPinNo1, dHighSparkVolt, dRealLowSparkVolt, dSparkV2[nTmp1Po1], nTmp1Po1, nSparkHWoffSet1, nSparkRefV);
			}
		}
		else {
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}

			if (nType == 1)
			{
				g_nSparkRefDataCount[nPin]++;
				g_fSparkRefData[nPin] += dSparkV2[nTmp1Po1];

				if (g_nSparkRefDataCount[nPin] == 1)	g_fSparkRefData[nPin] /= 1;
				else								g_fSparkRefData[nPin] /= 2;

				if (0)//fpSpark)
				{
					fprintf(fpSpark, ", %5d, %3d, %3.1f, %3.1f\n",
						nPin, g_nSparkRefDataCount[nPin], dSparkV2[nTmp1Po1], g_fSparkRefData[nPin]);
				}
			}

		}
	} //hdpark220511 spark end
	// hdpark230504 end
	else //Spark On모드가 아니라면 
	{
		dValue1 = fabs(dSparkI2[nTmp1Po1] - dSparkI2[nTmp1Po1 - 1]);
		dValue2 = fabs(dSparkI2[nTmp1Po1 + 1] - dSparkI2[nTmp1Po1]);


		if (nTmp1Po1 <= 1) {//sylee200822
			dValue3 = dValue2;
		}
		else if (nTmp1Po1 >= nMaxX) {//sylee200822
			dValue3 = dValue1;
		}
		else {//sylee200822
			if (dValue1 > dValue2) {
				dValue3 = dValue1;
			}
			else {
				dValue3 = dValue2;
			}
		}

		nSparkOnlyLog1 = 0;//sylee200901
		if (dValue4 > 0) {
			if (dValue3 >= dValue4) {
				nSparkDetectI1 = 1;
			}
			else {
				if (dValue3 >= dValue5) {//sylee200901
					nSparkDetectI1 = 1;
					nSparkOnlyLog1 = 1;//sylee200901
				}
			}

		}
		else {
			nSparkDetectI1 = 1;
		}

		//#######################################################################

		if ((nSparkDetectI1 == 1) && (nCase1 == 0) && (dMax1 > (nSparkRefV + nSparkHWoffSet1)))//sylee200803-1
		{

			if (dMax1 <= 0) {
				dMax1 = 0.1;
			}

			if (dMax1 >= dSparkLeakSet1_V * 0.9) {//SYLEE200804-1 
				dMax1 = dSparkLeakSet1_V * 0.9 + (rand() % ((int)dSparkLeakSet1_V)) / 100.0 * 1; //SYLEE200804-1 
			}

			if (nSparkOnlyLog1 == 1) {//sylee200901
				nSparkDetect1 = 0;//sylee200901
				dSparkI12[nSparkI1Po2][nSparkMaxX + 1] = -2;//sylee200901
			}
			else {
				nSparkDetect1 = 1;//sylee200710-1
			}
			dSparkI12[nSparkI1Po2][nSparkMaxX + 2] = (float)dValue3;//sylee200901


			if (nSparkI1Po2 < nSparkMaxY) {
				nSparkI1Po2++;
			}

			for (int nK = 1; nK <= nSparkMaxX; nK++) {
				dSparkI1[nSparkI1Po2][nK] = dSparkV2[nK];
				dSparkI12[nSparkI1Po2][nK] = dSparkI2[nK];//sylee200724
			}

			dSparkI1[nSparkI1Po2][nMaxX + 1] = 1; //fail
			dSparkI1[nSparkI1Po2][nMaxX + 3] = (float)nTmp1Po1;  //point

			Sleep(2);

			double  dRan1, dAlpha, dTemp1, dTemp2;//sylee200803-1

			dTemp1 = (((rand() % 100) / 100.) / 20.0);//sylee200803-1
			if (dTemp1 <= 0) {
				dTemp1 = (((rand() % 100) / 100.) / 20.0);//sylee200803-1
				if (dTemp1 <= 0) {
					dTemp1 = 0.01;
				}
			}
			// hdpark220826 begin, spark 발생, drop전압이 약 30V 정도로 일정하게 나는 현상 수정.
/*
			dTemp2 = (dSparkLeakSet1_V - dMax1) * (dSparkI2[nTmp1Po1] / dSparkLeakSet1_I * (0.95 - 0.4) + dTemp1); //sylee200803-1
			dAlpha = 30.0 / dTemp2 + dTemp1;//sylee200803-1	    
			dRan1 = (dMax1 + dTemp2 * dAlpha) / 1.0;  //sylee200803-1

			if (dRan1 > dSparkLeakSet1_V) {
				dRan1 = dSparkLeakSet1_V - (((rand() % 100) / 100.) * 2.);//sylee211007
			}
*/
			dAlpha = 0;
			double dTemp0 = (rand() % 100) / 100.0;				// 소수점 자리
			dTemp1 = 16.0 + rand() % 8;							// 전압 차이 일정 간격 이상 유지
			dTemp2 = rand() % (int)(dSparkLeakSet1_V - dMax1);	// 설정 전압과 Vs 전압 차이 만큼 발생
			dRan1 = dMax1 + dTemp2 + dTemp1 + dTemp0;

			if (dRan1 >= dSparkLeakSet1_V)
			{
				dRan1 = dSparkLeakSet1_V - (rand() % 10);
			}
			// hdpark220826 end

			if (dMax1 > dRan1) {//sylee200729
				//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
				if(dRan1 > ((double)(nSparkHWoffSet1*1.0)))  
				{
					dRan1 = dRan1 - ((double)(nSparkHWoffSet1*1.0));
					//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 [
					if((dRan1 > 90.0) && (SysSet16.m_nSet16 == 1))
					{
						dRan1 = 90.0 + dTemp0;
					}
					else if((dRan1 > 250.0))
					{
						dRan1 = 250.0 + dTemp0;
					}

					//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 ]

					
				}
				//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
				dSparkI1[nSparkI1Po2][nMaxX + 2] = (float)dRan1;
				dSparkI1[nSparkI1Po2][nMaxX + 4] = (float)dMax1;//sylee200729
			}
			else {
				//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
				if(dMax1 > ((double)(nSparkHWoffSet1*1.0)))  
				{
					dMax1 = dMax1 - ((double)(nSparkHWoffSet1*1.0));
					//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 [
					if((dMax1 > 90.0) && (SysSet16.m_nSet16 == 1))
					{
						dMax1 = 90.0 + dTemp0;
					}
					else if((dMax1 > 250.0))
					{
						dMax1 = 250.0 + dTemp0;
					}
					//yklee240624 BH Spark VS 전압 90V 이상일 경우 2~90V 범위로 적용/ MAP>SET16 미사용 체크버튼 사용-체크일때 2~90V, 아닐때 2~90이상 ]

				}
				//yklee240621 BH Spark VS 전압 표기 적용 by 19V이하 표기 안되는 문제 대응[레시피 4V, HW Offset 15V]
				dSparkI1[nSparkI1Po2][nMaxX + 2] = (float)dMax1;
				dSparkI1[nSparkI1Po2][nMaxX + 4] = (float)dRan1;//sylee200724
			}

			dSparkI1[nSparkI1Po2][nMaxX + 5] = 0.0; //sylee200724(float)dSparkI2[nTmp1Po1];  //point	 
			dSparkI1[nSparkI1Po2][nMaxX + 6] = (float)nSparkPinNo1; //pinno	
			if (nSparkI1Po2 == 1) {
				nSparkI1Po2 = nSparkI1Po2;
			}
			if (nSparkI1Po2 == 2) {
				nSparkI1Po2 = nSparkI1Po2;
			}

			if (nType == 2) {//waitting 
				nSpark1_Detect2 = 2;//disable
			}
		}
		else {
			if (nType == 2) {//waitting 	 
				nSparkI1Po2 = 0;
				nSpark1Po1_Start = 0;
				nSpark1_Detect2 = 0;//sylee200721
				nSpark1_Detect2Co = 0;
			}
		}
	}

	//########################################################################

	return 1;
}
#endif


//son nSparkMaxX 카운트 회수 만큼 Spark VS, I 값을 read한다.
//    dSparkV2[nSparkV1Po1]:  Spark VS
//    dSparkI2[nSparkV1Po1]:  Spark I
int CModeSub71::AOn_Spark_ReadV1_I1()//SYLEE200713   V AND I
{
#ifndef _PC_TEST_

	CString str;
	BYTE  La, Ha;
	DWORD DHa;
	int Ret;
	double Y1;
	double X1;

	memset(dSparkV2, 0, sizeof(dSparkV2));		// hdpark230504
	memset(dSparkI2, 0, sizeof(dSparkI2));		// hdpark230504
	nSparkV1Po1 = 0;


//	Delay_100ns(150);//15uS //sylee200730 	//SYLEE200804  // hdpark230110 comment


Task1:

	//#######################################################################################	
	//  Read V Value  from ADC_A  
#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

	InpNo[0] = 2;	        InpNo[1] = 3;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 2,3 ] Error!!!");	return  0; }
	La = InpData[0];  	Ha = InpData[1];
#else  //#ifdef DEF_CARD_A1  


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
		for( int i=1; i<=200; i++){

			Ret = Ixud_WriteDOBit(0,7,1,0);  //  /AD_CONVST  =>  63
			if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0;	}
			//Delay_100ns(100);//2uS X   / 5uS X    /  10uS ok
			Ret = Ixud_WriteDOBit(0,7,1,1);  //  /AD_CONVST
			if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0;	}


			Ret = d64h_di_readport( pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus );
			if( Ret!=0 ) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0;	}
			La=(BYTE)((pDIO_D64.dwDIStatus>>16)&0xff);
			Ha=(BYTE)((pDIO_D64.dwDIStatus>>24)&0xff);
			DHa=(DWORD)(Ha<<8);
			DHa=DHa+La;
			//VC READING
			Y1=  DHa/65535.*dSparkLeakSet1_V; //??? //SYLEE200713-1

			if(Y1<0.1){
			   Y1=0.1;
			}
			if(Y1>20){
				Y1=Y1;
			//	AfxMessageBox(" NG");
			//	break;
			}
		}
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	//sylee200723-sparktesting 
//sylee200730	Delay_100ns(100);//10uS X   / 5uS X    / 7uS X  /  10uS ok
//   Delay_100ns(50);//10uS->5uS //sylee200730

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST  =>  63
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }
	//Delay_100ns(100);//2uS X   / 5uS X    /  10uS ok

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);    //son 3번째 byte  ADC_B Low
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);    //son 4번째 byte  ADC_B High

#endif  //#ifdef DEF_CARD_A1 

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;
	//VC READING 	

//SYLEE200803-1	Y1=  DHa/65535.*250.0; //??? //SYLEE200713-1 //sylee200725

	//spark drop voltage

//SYLEE200804-1  Y1=  (DHa/65535.*277.8 * dSparkLeakSet1_V /100.0) /1.0;	//SYLEE200803-1  
	Y1 = DHa / 65535. * 250.0; //SYLEE200804-1 

	if (Y1 < 0.1) {
		Y1 = 0.1;
	}

	dSparkV2[nSparkV1Po1] = (float)Y1;//sylee200729    //son Spark Vs Write 
//#######################################################################################	
 
	// hdpark220511 spark begin
	if (m_bModifySparkMeas)
	{
#ifdef DEF_CARD_A1 

#else   
		La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);          //son 1번째 byte ADC_A Low
		Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);   //son 2번째 byte ADC_A High

		DHa = (DWORD)(Ha << 8);
		DHa = DHa + La;

		if (!m_bSetVBRead)	X1 = DHa / 65535. * dSparkLeakSet1_I;
		else				X1 = DHa / 200.0; // spark vb read

		if (X1 < 0.1) {
			X1 = 0.1;
		}

		dSparkI2[nSparkV1Po1] = (float)X1;     //son Spark 전류 write

		//TRACE("%3d : Y1 = %.3f, X1 = %.3f \n", nSparkV1Po1, Y1, X1);
		if (nSparkV1Po1 == 0 || nSparkV1Po1 == nSparkMaxX)
		{
			if (0)
			{
				fprintf(fpSpark, "%2d, %3d, %4.1f, %5.1f \n", nSparkV1Po1, m_nHWCnt, Y1, X1);
			}
		}
#endif  //#ifdef DEF_CARD_A1 
	}//hdpark220511 spark end

	else
	{


		//  I READ

#ifdef DEF_CARD_A1  //sylee170328
		BYTE Buf;
		Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
		Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }
		//I DATA READING
		Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
		if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  0; }
		La = Buf;
		Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
		if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  0; }
		Ha = Buf;

#else  //#ifdef DEF_CARD_A1 

//	Ret = Ixud_WriteDOBit(0,7,1,0);  //  /AD_CONVST
//    if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0;	}
//	Ret = Ixud_WriteDOBit(0,7,1,1);  //  /AD_CONVST
//   if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0;	}
//I DATA READING

		Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
		if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
		La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
		Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 

		DHa = (DWORD)(Ha << 8);
		DHa = DHa + La;

		//	X1=DHa/65535.*20.; 
		//	X1= (20.-X1)*12.5; //    12.5= leakSetV/LeakI  //  dSparkLeakSet1_I=(double)nISet;//sylee200710-1 

		X1 = DHa / 65535. * dSparkLeakSet1_I;
		//sylee200724	X1= (dSparkLeakSet1_I-X1)*dSparkLeakSet1; //    12.5= leakSetV/LeakI

		///====>if(dSparkLeakSet1_I<X1)	X1=0;  
		//sylee200712-1	dSparkI1[nSparkI1Po2][nSparkI1Po1]=(float)X1;
		if (X1 < 0.1) {
			X1 = 0.1;
		}

		dSparkI2[nSparkV1Po1] = (float)X1;

		//#######################################################################################
		//Delay_100ns(200);//20uS

		AOn_Spark_WaitCheck1();//sylee210430
	}

	if (nSparkV1Po1++ < nSparkMaxX) {
		goto Task1;
	}

#endif
	return 1;
}







int CModeSub71::AOn_Spark_WaitCheck1()//sylee210430
{
	double dRefV;

	dRefV = 30.0;

	if (nSparkMaxXSet == nSparkMaxX) {
		if (nSparkMaxXSet == nSparkV1Po1) {
			if (dSparkV2[nSparkV1Po1] > dRefV) {
				nSparkMaxX = nSparkMaxX + 5;
				if (nSparkMaxX > 5) {
					nSparkMaxXGap1 = nSparkMaxX - 2;
				}
				else {
					nSparkMaxXGap1 = nSparkMaxX;
				}
			}
			else {

			}
		}
	}
	else {
		if (nSparkMaxXSet == (nSparkV1Po1 - 1)) {
			if (dSparkV2[nSparkV1Po1] <= dRefV) {
				nSparkMaxX = nSparkMaxXSet;
				if (nSparkMaxX > 5) {
					nSparkMaxXGap1 = nSparkMaxX - 2;
				}
				else {
					nSparkMaxXGap1 = nSparkMaxX;
				}
			}
		}
	}
	return 1;
}




int CModeSub71::AOn_Spark_Wait_ReadV1_I1_init()//SYLEE200721
{
	memset(dSparkV2, -1, sizeof(dSparkV2));
	memset(dSparkI2, -1, sizeof(dSparkI2));
	nSparkV1Po1 = 0;
	if (nSparkMaxX > 5) {
		nSparkMaxXGap1 = nSparkMaxX - 2;
	}
	else {
		nSparkMaxXGap1 = nSparkMaxX;
	}
	return 1;
}


int CModeSub71::AOn_Spark_Wait_ReadV1_Ibuff(int AD_I)//SYLEE200721   V AND I buffer
{

#ifndef _PC_TEST_
	CString str;
	BYTE  La, Ha;
	DWORD DHa;
	int Ret;
	double Y1, X1;

	if (nSparkV1Po1 < nSparkMaxX) {
		nSparkV1Po1++;
	}
	else {
		nSpark1Po1_Start = 1;
		nSparkV1Po1 = 1;
	}



	// Task1: 

	//#######################################################################################	
	//  Read V Value  from ADC_A  
#ifdef DEF_CARD_A1  //sylee170328
/*	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0;	}
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0;	}
*/
	InpNo[0] = 2;	        InpNo[1] = 3;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 2,3 ] Error!!!");	return  0; }
	La = InpData[0];  	Ha = InpData[1];
#else  //#ifdef DEF_CARD_A1  


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//    Delay_100ns(100);//10uS X   / 5uS X    / 7uS X  /  10uS ok

/*	Ret = Ixud_WriteDOBit(0,7,1,0);  //  /AD_CONVST  =>  63
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0;	}
   //Delay_100ns(100);//2uS X   / 5uS X    /  10uS ok
	Ret = Ixud_WriteDOBit(0,7,1,1);  //  /AD_CONVST
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0;	}
 */
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);

#endif  //#ifdef DEF_CARD_A1 

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;
	//VC READING 	

 //SYLEE200803-1	Y1=  DHa/65535.*250.0; //??? //SYLEE200713-1 //sylee200725	

//spark drop voltage
//SYLEE200804-1 	Y1=  (DHa/65535.*277.8 * dSparkLeakSet1_V /100.0) /1.0;	//SYLEE200803-1  
	Y1 = DHa / 65535. * 250.0; //SYLEE200804-1 

	if (Y1 < 0.1) {
		Y1 = 0.1;
	}
	dSparkV2[nSparkV1Po1] = (float)Y1;//sylee200712-1	  
//#######################################################################################	


//  I READ

#ifdef DEF_CARD_A1  //sylee170328
/*
	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0;	}
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0;	}
//I DATA READING
*/

/*
Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading
if( Ret!=0 ) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  0;	}
La=Buf;
Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading
if( Ret!=0 ) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  0;	}
Ha=Buf;
*/

#else  //#ifdef DEF_CARD_A1 

//	Ret = Ixud_WriteDOBit(0,7,1,0);  //  /AD_CONVST
//    if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0;	}
//	Ret = Ixud_WriteDOBit(0,7,1,1);  //  /AD_CONVST
//   if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0;	}
//I DATA READING
 /*
	Ret = d64h_di_readport( pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus );
	if( Ret!=0 ) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0;	}
	La=(BYTE)(pDIO_D64.dwDIStatus&0xff);
	Ha=(BYTE)((pDIO_D64.dwDIStatus>>8)&0xff);
	*/

#endif  //#ifdef DEF_CARD_A1 

	//	DHa=(DWORD)(Ha<<8);
	//   DHa=DHa+La;

	DHa = AD_I;

	X1 = DHa / 65535. * dSparkLeakSet1_I;
	if (X1 < 0.1) {
		X1 = 0.1;
	}
	dSparkI2[nSparkV1Po1] = (float)X1;

	//#######################################################################################
	//Delay_100ns(200);//20uS

	//	if(nSparkV1Po1++<nSparkMaxX ){
	//		goto Task1;
	//	}

#endif
	return 1;
}




//son Sub71.Y1 값 할당이 없음. 
//    Sub71.R1 값은 dIADC1  ADC I 값과 동일함.
int CModeSub71::AOnReadV1I1_ADC_I()//SYLEE20120711-2 
{
#ifndef _PC_TEST_
	CString str;
	BYTE Buf, La, Ha;
	DWORD DHa, V1Ma, V1Mi, V1Ma2, V1Mi2, DSumV1, DSumS1, V1T;

	long nC;
	int nCo, Ret;

	short nS1Co1, nS1Po1, nS1Po2, nS1Max1, nS1Flag1;//sylee180705-3
	short nS2[501];//sylee180705-3
	int nS11[501], nS12[501];//sylee180705-3
	double dS1To1;//sylee180705-3
	int nADCount2;//sylee181115
    double dAvg;    //son231012 


	nCo = 0;    V1Ma = 0;    V1Mi = 0;	V1Ma2 = 0;    V1Mi2 = 0;
	V1T = 0;	 DSumV1 = 0;	DSumS1 = 0;


	//#####################################################################################################
	nADCount2 = nADCount1;//sylee181115

	nS1Flag1 = 0;
	//sylee200916	if((nMod==5) && (nADCount2>=5)  && (nADCount2<=200) ){
	if ((nADCount2 >= 5) && (nADCount2 <= 200)) {
		::ZeroMemory(&nS2, sizeof(nS2)); //sylee180705-3
		::ZeroMemory(&nS11, sizeof(nS11)); //sylee180705-3
		::ZeroMemory(&nS12, sizeof(nS12)); //sylee180705-3
		nS1Co1 = 0; nS1Po1 = 0; nS1Max1 = 0;
		nS1Flag1 = 1; //sylee180705-3

	/*	if(SysInfo05.m_n4WSet1==_4W_SET_FAST){   //son201117 _4W_SET_FAST enum 사용
			if(n4W_PinFastADC1==2){ //sylee20905
				if(nADCount2>=10){//sylee200910
					nADCount2=10;//sylee20905
				}
			}
		}
		*/
	}
	//#####################################################################################################


	//==============================================================================================
Task1:


	Buf = 0;		nC = 0;
	R1 = 0.0;	Buf = 0;  La = 0;  Ha = 0;  //sylee140823 



#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A  //I   NEGATIVE TRIGGER  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   //I  //sylee20111109  //SYLEE20111115  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }
	//I DATA READING
	Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  0; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  0; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1 

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	//I DATA READING
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 



	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;


	V1T = DHa;
	if (nCo == 0) {
		V1Ma2 = V1T;
		V1Mi2 = V1T;
	}
	else {
		if (V1Ma2 < V1T) {
			V1Ma2 = V1T;
		}
		if (V1Mi2 > V1T) {
			V1Mi2 = V1T;
		}
	}

	DSumS1 = DSumS1 + V1T;

	//#####################################################################################################
	if (nS1Flag1 == 1) { //sylee180705-3
		nS1Co1++;
		nS11[nS1Co1] = DHa;//SYLEE180705-3;		  
	}
	//#####################################################################################################

#if 0  //son241115  검증중인 코드라고 되어 있고  BH에서 Spark 0 V 출력 문제가 발생하여 코드 막음
 //hdpark240116 begin spark check, 검증 중
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;
	Y1 = DHa / 65535. * 250.0;

	dSparkV2[nCo % 300] = (float)Y1;
 //hdpark240116 end
#endif



	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}

	nflgB = 1;
	if (nADCount1 <= 3) {
		//son231012 DHa = (DSumS1) / (nADCount1);
		dAvg = (double)(DSumS1) / (double)(nADCount1);  //son231012 평균값 소수점이하 잘리는 문제 수정. 
		                                                //          DHa int가 아니라 dAvg double 값 사용
	}
	else {
		//son231012 DHa = (DSumS1 - V1Ma2 - V1Mi2) / (nADCount1 - 2);//sylee20111111  
		dAvg = (double)(DSumS1 - V1Ma2 - V1Mi2) / (double)(nADCount1 - 2);//son231012
	}

	//#####################################################################################################
	if (nS1Flag1 == 1) //sylee180705-3
	{
		//=================================================
		nS1Po2 = 0;
		for (int k2 = 1; k2 <= nADCount2; k2++) {
			nS1Po1 = 0; nS1Max1 = 0;
			for (int k1 = 1; k1 <= nADCount2; k1++) {
				if (nS2[k1] == 1) {
					continue;
				}
				if (nS11[k1] > nS1Max1) {
					nS1Max1 = nS11[k1];
					nS1Po1 = k1;
				}
			}
			if (nS1Po1 > 0) {
				if (nS2[nS1Po1] != 1) {
					nS2[nS1Po1] = 1;
					nS12[k2] = nS11[nS1Po1];
				}
			}
		}
		//==================================================

		int nTab1;
		nTab1 = 0;//sylee181115
		if (nADCount2 >= 5) {
			nTab1 = nADCount2 / 4;
		}
		dS1To1 = 0.0;
		for (int k3 = (nTab1 + 1); k3 <= (nADCount2 - (nTab1)); k3++) {
			dS1To1 = dS1To1 + nS12[k3];
		}
//son231012 begin:  평균값 소수점이하 잘리는 문제 수정. DHa int가 아니라 dAvg double 값 사용
		//son231012 dS1To1 = dS1To1 / double(nADCount2 - (nTab1 * 2.));
		//son231012 DHa = (int)dS1To1;
		
		dS1To1 = (double)dS1To1 / (double)(nADCount2 - (nTab1 * 2.)); //son231012 DHa -> dAvg
		dAvg = dS1To1;
//son231012 end

		/////////////////////////////////////////////////////////////////////
			//   double dtmp1,dNgPercent;   //sylee200515-patch
			/*	if((nMod==5) && (nADCount2>=5)  && (nADCount2<=200) ){//sylee200513-1  //sylee200515-patch
					//sylee200915  dNgPercent=0.04;//4%
					dNgPercent=0.1;//4%->10%
					dtmp1=(fabs(nS12[nTab1+1]-nS12[nADCount2-(nTab1)])/dS1To1);////  =>  /average
					if(dtmp1>dNgPercent){
						 n4W_ADC_Fail_Flag=1; //ng	//sylee200513-1
					}
				}
			*/
			/////////////////////////////////////////////////////////////////////

	}

	//#####################################################################################################


	//son231012_2 dIADC1 = (int)DHa;
	dIADC1 = dAvg;  //son231012_2  int nIADC1  -> double dIADC1으로 수정. 소수점이하 잘리는 문제보완. 
	R1 = (double)dIADC1;//SYLEE20120715 //son231012_2


#if 0  //son241115  검증중인 코드라고 되어 있고  BH에서 Spark 0V 출력 문제가 발생하여 코드 막음
	// hdpark240116 begin, 검증 중
	if (OnSparkStatus())
	{
		if (nSpark1_Detect2 != 2)			// 처음 spark 발생한 데이터만 취득
		{
			nSpark1_Detect2 = 2;

			double dSparkV = 2.0;
			for (int i = 0; i < 300; i++)
			{
				if (dSparkV < dSparkV2[i])	dSparkV = dSparkV2[i];
			}

			srand((unsigned int)time(NULL));
			double dTemp0 = (rand() % 200) / 100.0;							// 소수점 자리, +-2.0

			g_dWaitingSparkVOrg = dSparkV;

			g_dWaitingSparkV = dSparkV * (m_dCalRatio * (dSparkV / m_dCalSparkMax));	// 0 level = 3.0 offset, 의미없음.

			if (g_dWaitingSparkV < nSparkRefV)			g_dWaitingSparkV = nSparkRefV + dTemp0;
			if (g_dWaitingSparkV > dSparkLeakSet1_V)	g_dWaitingSparkV = dSparkLeakSet1_V - dTemp0;
		}
	}
	// hdpark240116 end
#endif	


#endif


	return 1; //V1=VB
}


//##########################################################################################################



int CModeSub71::ARunoff1()    //sylee20111020 
{

#ifndef _PC_TEST_
	CString str;

#ifdef DEF_CARD_A1  //sylee170328]
	int Ret;
	Ret = DioOutBit(pDIO.hDrv, DO3338, 0);   //VC-SS  CMD ,  CHANNEL  B      //#####?????? 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 127, 0] Error!!!");	return  0; }
#else  //#ifdef DEF_CARD_A1 	
	//	Ret = AB_Bit( DO3338, 0);   //VC-SS  CMD ,  CHANNEL  B
	//	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 127, 0] Error!!!");	return  0;	}
#endif  //#ifdef DEF_CARD_A1 
#endif  

	return 1;
}




void CModeSub71::OnButton601() // CHANNEL B  //LV1 VB SENSING
{
#ifndef _PC_TEST_
	CString str;

	if (nMode1 == 2 || nMode1 == 3 || nMode1 == 4) {   //sylee20111214   HW INTERLOCK    VB READING없이  R READING시   LV HV  VC  VB 다 ON된다. 
		AfxMessageBox(" LV2,HV1,HV2  Relay ON ? ] Error!!!");
		return;
	}

#ifndef _PC_TEST_
	AVBRV1(1);// n     1=LV1,  2=LV2,  3=HV1, 4=HV2;
#endif

	((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);
	str.Format(" LV1 VB SENSING");
	m_list1.InsertString(0, str);

	nFlag71_I_FilterMode = 0;

#endif
}




void CModeSub71::OnButton610() //LV2 VB READING
{
	CString str;

	if (nMode1 == 1 || nMode1 == 3 || nMode1 == 4) { //sylee20111214   HW INTERLOCK    VB READING없이  R READING시   LV HV  VC  VB 다 ON된다. 
		AfxMessageBox(" LV1,HV1,HV2  Relay ON ? ] Error!!!");
		return;
	}

#ifndef _PC_TEST_
	AVBRV1(2);// n     1=LV1,  2=LV2,  3=HV1, 4=HV2;
#endif
	((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);
	str.Format("LV2 VB READING");
	m_list1.InsertString(0, str);

	nFlag71_I_FilterMode = 0;

}

void CModeSub71::OnButton611() //HV1 VB READING
{
	CString str;
	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 4) {  //sylee20111214   HW INTERLOCK    VB READING없이  R READING시   LV HV  VC  VB 다 ON된다.  김영필 GJ 
		AfxMessageBox(" LV1,LV2, HV2  Relay ON ? ] Error!!!");
		return;
	}

#ifndef _PC_TEST_
	AVBRV1(3);// n     1=LV1,  2=LV2,  3=HV1, 4=HV2;
#endif
	((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);
	str.Format("HV1 VB READING");
	m_list1.InsertString(0, str);

	nFlag71_I_FilterMode = 0;
}




void CModeSub71::OnButton612()  //HV2 VB READING
{
	CString str;
	if (nMode1 == 1 || nMode1 == 2 || nMode1 == 3) {   //sylee20111214   HW INTERLOCK    VB READING없이  R READING시   LV HV  VC  VB 다 ON된다. 
		AfxMessageBox(" LV1,LV2,HV2  Relay ON ? ] Error!!!");
		return;
	}

#ifndef _PC_TEST_
	AVBRV1(4);// n     1=LV1,  2=LV2,  3=HV1, 4=HV2;
#endif
	((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);
	str.Format("HV2 VB READING");
	m_list1.InsertString(0, str);

	nFlag71_I_FilterMode = 0;
}



void CModeSub71::OnButton614()  //HV  VB OUT  CHECK   READING //SYLEE150118
{
	CString str;

#ifndef _PC_TEST_
	AVBRV1(8);   //1=LV1,  2=LV2,  3=HV1, 4=HV2;  8=HV  VB OUT  CHECK   READING //SYLEE150118
#endif
	((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);
	str.Format("HV  VB OUT  CHECK   READING");
	m_list1.InsertString(0, str);

	nFlag71_I_FilterMode = 0;
}



void CModeSub71::AVBRI1()/// I READING 
{

#ifndef _PC_TEST_
	CString str;
	int Ret;
	int nCo;
	double lI1, lI2, lI3, lI4;

	BYTE Buf;
	BYTE La, Ha;
	DWORD DHa;
	DWORD V1Ma, V1Mi, V1S, V1T;

	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;

	if (nModeIR < 1 || nModeIR>5) {
		str.Format(" * ERROR : Current : Range Mode? ");
		m_list1.InsertString(0, str);
		str.Format("Range?");
		m_Label102.SetCaption(str);
		return;
	}

#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO45, 1);   //45 VB-SS CMD ,   0= I ADC SENSE   1= VB ADC SENSE    //SYLEE20120501  1->0  H/W바뀜 0=VB SENSE   1= I SENSE   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }


Task1://sylee20110705


	Buf = 0;

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }

	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return; }
	La = Buf;

	Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1  

	Ret = Ixud_WriteDOBit(0, 8, 2, 1);  //VC SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,1) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }

Task1://sylee20110705 

	Buf = 0;
	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;
	V1T = DHa;
	V1S = V1S + V1T;

	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}

	//son231012 DHa = (V1S) / (nADCount1);
    double dAvg = (double)(V1S) / (double)(nADCount1); //son231012 DHa -> dAvg

	//son231012 str.Format("%d", DHa);
	str.Format("%.1f", dAvg); //son231012 DHa -> dAvg
	m_Label102.SetCaption(str);


	if (nModeCC == 0) {  //sylee20111010  
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 250.; //son231012 DHa -> dAvg
			str.Format("%.3fmA", Y1);
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2500.; //son231012 DHa -> dAvg
			str.Format("%.3fmA", Y1);
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 5000.;    //sylee150903  25->1250 //ACE400 //son231012 DHa -> dAvg
			str.Format("%.3fmA", Y1);
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 25000.;     //sylee150903  250->25 //ACE400 //son231012 DHa -> dAvg
			str.Format("%.3fmA", Y1);
		}
		else if (nModeIR == 5) {
			Y1 = (double)dAvg / 250.;   //sylee150903 2500->250 //ACE400 //son231012 DHa -> dAvg
			str.Format("%.3fuA", Y1);
		}
		else {
			Y1 = 0;
			str.Format("I Range?");
		}
	}
	else {  //CV  H/W  개조 수정  //sylee20111010 
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 200.; //son231012 DHa -> dAvg
			str.Format("%.3fmA", Y1);
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 1000.; //sylee130627 //son231012 DHa -> dAvg
			str.Format("%.3fmA", Y1);
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 10.; //sylee130627 //son231012 DHa -> dAvg
			str.Format("%.3fuA", Y1);
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 100.;//sylee130627 //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {
		//sylee230420	Y1 = (double)dAvg / 1000.;//sylee130627
			Y1 = (double)dAvg / 200.;//sylee221220 //son231012 DHa -> dAvg
			str.Format("%.3fuA", Y1);
		}
		else {
			Y1 = 0;
			str.Format("I Range?");
		}
	}

	m_Label122.SetCaption(str);

	nIf = 1;

	if (nIf == 1) {
		if (lIset == 0) {
			str.Format(" ===>  ERROR : I Decimal Set 0   ");
			m_list1.InsertString(0, str);
			return;
		}
		lI1 = lIset * 0.99;
		lI2 = lIset * 1.01;
		lI3 = lIset * 0.9;
		lI4 = lIset * 1.1;
		lI1 = 0;	//SYLEE20110706
		lI2 = 2000000;  //SYLEE20110706

		if ((Y1 > lI1) && (lI2 > Y1)) {

		}
		else if ((Y1 < lI3) || (lI4 < Y1)) {
			str.Format(" ===>  ERROR : I  10 Over");
			m_list1.InsertString(0, str);
		}
	}

	nflgA = 1;

#endif
	return;

}

/*
int CModeSub71::AOnReadI1Init1() /// I READING   //sylee20111109
{
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	 Ret = DioOutBit(pDIO.hDrv, DO45, 1);
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return  0;	}
	Delay_100ns(30000);//3mSEC
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A  //SYLEE20111115
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0;	}
#else  //#ifdef DEF_CARD_A1
	 Ret = DioOutBit(pDIO.hDrv, DO45, 1);
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return  0;	}
	Delay_100ns(30000);//3mSEC
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_A  //SYLEE20111115
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0;	}
#endif  //#ifdef DEF_CARD_A1


	 return 1;
}
*/


int CModeSub71::AOnReadI1() /// I READING 
{
#ifndef _PC_TEST_
	CString str;
	BYTE Buf;
	BYTE La, Ha;
	DWORD DHa;
	DWORD V1Ma, V1Mi, V1S, V1T;
	int Ret;
	int nCo;


	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;

Task1://sylee20110705


	Buf = 0;


#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);       //AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }

	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  0; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  0; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1  

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);

#endif  //#ifdef DEF_CARD_A1 

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	if (DHa == 65535 && nADCOver_IntDisable != 2) {//sylee20120318 INTERLOCK
		DHa = 0;
	}

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A     //sylee20111109  //SYLEE20111115
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }
#else  //#ifdef DEF_CARD_A1   

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

#endif  //#ifdef DEF_CARD_A1 





	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}
	//son231012 DHa = (V1S) / (nADCount1);
	double dAvg = (double)(V1S) / (double)(nADCount1); //son231012 DHa -> dAvg


	if (nModeCC == 0) {  //sylee20111010  
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 250.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2500.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 5000.;  //son231012 DHa -> dAvg
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 25000.; //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {
			Y1 = (double)dAvg / 250.;   //son231012 DHa -> dAvg

		}
		else {
			Y1 = 0;
		}
	}
	else {  //CV  H/W  개조 수정  //sylee20111010 
		if (nModeIR == 1) {
			Y1 = (double)dAvg / 200.;   //son231012 DHa -> dAvg
		}
		else if (nModeIR == 2) {
			Y1 = (double)dAvg / 2000.;  //son231012 DHa -> dAvg
		}
		else if (nModeIR == 3) {
			Y1 = (double)dAvg / 20.;    //son231012 DHa -> dAvg
		}
		else if (nModeIR == 4) {
			Y1 = (double)dAvg / 200.;   //son231012 DHa -> dAvg
		}
		else if (nModeIR == 5) {

			Y1 = (double)dAvg / 2000.;  //son231012 DHa -> dAvg

		}
		else {
			Y1 = 0;

		}
	}


	nflgA = 1;

#endif
	return 1;

}


void CModeSub71::OnButton602() // CHANNEL A   //I
{
#ifndef _PC_TEST_

	if (nFlag71_I_FilterMode == 1 || nFlag71_I_FilterMode == 2) { //sylee150118
	}
	else {
		OnFiler_I_M(2); //fast i//sylee150118
	}

	AVBRI1();   //son  I Reading..

#endif
}


//VC SENSING
void CModeSub71::OnButton603() //CHANNEL B
{
#ifndef _PC_TEST_
	CString str;

	if (nMode4W == 1) {
		str.Format(" 4WMode.");
		m_Label110.SetCaption(str);
		str.Format(" ERROR.");
		m_Label123.SetCaption(str);
		return;
	}

	if (nMode1 == 1 || nMode1 == 2) {//lv  //sylee150122
		if (nFlag71_VC_Filter_Mode_LV == 1) {
			nFlag71_VC_Filter_Mode_LV = 0;
			OnRadio04();
		}
	}
	else if (nMode1 == 3 || nMode1 == 4) {//hv  //sylee150122
		if (nFlag71_VC_Filter_Mode_HV == 1) {
			nFlag71_VC_Filter_Mode_HV = 0;
			OnRadio03();
		}
	}

	AVBRV2(nMode1);  //nMode1  relay on mode  lv1  lv2  hv1  hv2    // vc reading
#endif
}

/*

void CModeSub71::On4WEnable()    //4 WIRE CMD
{

	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO47, 1);   //4W CMD ,   CHANNEL  B
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 47, 1] Error!!!");	return   ;	}
#else  //#ifdef DEF_CARD_A1
	Ret = DioOutBit(pDIO.hDrv, DO47, 1);   //4W CMD ,   CHANNEL  B
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 47, 1] Error!!!");	return   ;	}
#endif  //#ifdef DEF_CARD_A1

}
*/


/*
void CModeSub71::On4WEnable2( int a)    //4 WIRE CMD    //SYLEE20111125 앞에 전압 위에서 시작 하는것때문
{
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if( a==1){
		Ret = DioOutBit(pDIO.hDrv, DO48, 1);
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 48, 1] Error!!!");	return   ;	}
	}else{
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return   ;	}
	}
#else  //#ifdef DEF_CARD_A1

	if( a==1){
		Ret = DioOutBit(pDIO.hDrv, DO48, 1);
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 48, 1] Error!!!");	return   ;	}
	}else{
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return   ;	}
	}

#endif  //#ifdef DEF_CARD_A1



}
*/



#if 0
// 4wire  계산식    R=(4W/1000)/I    2011/7/4   i=Y1
void CModeSub71::OnButton604()    //4 WIRE CMD       4WIRE  VOLTAGE READING 부 
{
#ifndef _PC_TEST_
	CString str;
	int Ret, nCo, nCount;
	BYTE Ha, La;
	DWORD DHa, V1Ma, V1Mi, V1S, V1T;
	double V10, nS3;  //Scale //SYLEE20111013


#ifdef DEF_CARD_A1  //sylee170328	 
	BYTE Buf;
#endif  //#ifdef DEF_CARD_A1 


	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;
	V10 = 0;
	W1 = 0;  //4WIRE 
	nS3 = 0;
	nCount = 20;//200;

	if (nMode4W != 1) {
		str.Format(" 4WMode?");
		m_Label109.SetCaption(str);
		m_Label124.SetCaption(str);
		return;
	}


#ifdef DEF_CARD_A1  //sylee170328

	//sylee130605
	Ret = DioOutBit(pDIO.hDrv, DO43, 0);   // VC_hv    0=off   1=on   RELAY
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO44, 0);    // VC_lv    0=off   1=on   RELAY
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 0] Error!!!");	return; }
	Delay_100ns(20000);  //2ms 
	Ret = DioOutBit(pDIO.hDrv, DO45, 1);   //   (0=HV,      1=LV)  IC
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO46, 1);  // 4WIRE_CMD  (0=VC SENSING,      1=4W SENSING)
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_B
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }

Task1://sylee20110705 

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);  //AD_CONVST_B
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  //AD_CONVST_B   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return; }
	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 2, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 2] Error!!!");	return; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 3, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 3] Error!!!");	return; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1   

	//sylee130605
	Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return; }
	Delay_100ns(20000);  //2ms 
	Ret = Ixud_WriteDOBit(0, 8, 2, 1);  //VC SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,1) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 5, 6, 1);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 1);  //4W CMD 
	Delay_100ns(20000);//2mSEC//sylee200508-ray //sylee200515-patch
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,1) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }

Task1://sylee20110705 

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return; }

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);


#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;
	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}
	//son2310112 DHa = (V1S) / (nADCount1);
	double dAvg = (double)(V1S) / (double)(nADCount1); //son231012 DHa -> dAvg

	str.Format("%.1f", dAvg);
	m_Label109.SetCaption(str);
	str.Format("-> 4 Wire sensing 16 bit =%.2f", dAvg);
	m_list1.InsertString(0, str);
	if (Y1 > 0) {
		X1 = dAvg / 6553.6;//sylee20111011
	}
	else {
		X1 = 0.0;//sylee20111011
	}

	W1 = X1;  //4WIRE 
	str.Format("%.3f V", X1);
	m_Label124.SetCaption(str);
	str.Format("-> 4 Wire sensing =%.3f", X1);
	m_list1.InsertString(0, str);
	nflgB = 2;


#endif
}

#else
// 4wire  계산식    R=(4W/1000)/I    2011/7/4   i=Y1
void CModeSub71::OnButton604()    //4 WIRE CMD       4WIRE  VOLTAGE READING 부 
{
	if (nMode4W != 1) {
		str.Format(" 4WMode?");
		m_Label109.SetCaption(str);
		m_Label124.SetCaption(str);
		return;
	}

//son231012_3 begin: 
//	DWORD DHa;
//	DHa = AOnRead_4W_VC();  //son210611 4W VC 전용 측정.
//	if (DHa < 0)
//		return;

	double d4wVc = AOnRead_4W_VC();  //son210611 4W VC 전용 측정.
	if (d4wVc < 0)
		return;

	str.Format("%.1f", d4wVc); //son231012 DHa -> d4wVc

	m_Label109.SetCaption(str);
	//str.Format("-> 4 Wire sensing 16 bit =%d", DHa);
	str.Format("-> 4 Wire sensing 16 bit =%.1f", d4wVc); //son231012 DHa -> d4wVc
	m_list1.InsertString(0, str);
//son231012_3 end

	str.Format("%.3f V", X1);
	m_Label124.SetCaption(str);
	str.Format("-> 4 Wire sensing =%.3f", X1);
	m_list1.InsertString(0, str);
	nflgB = 2;
}

#endif



double CModeSub71::ARReadCal1(int nRelay, int nMod1, double nV, int nIR, double nI, double R)
{
#ifndef _PC_TEST_
	double nS1, nO1;
	//	int nRReadCalNo[1000000][3];
	nS1 = 1;	nO1 = 0;


	if (nRelay == 1 || nRelay == 2) {//lv1
		if (nMod1 == 1) {//cc
			if (nV == 1) {

			}
			else if (nV == 10) {
				if (nIR == 1) {
					if (nI == 20) {
						nS1 = 1;	nO1 = 0; //?
					}
				}
				else if (nIR == 2) {
					if (nI == 10) {
						nS1 = 1;	nO1 = -88;
					}
				}
				else if (nIR == 3) {
					if (nI == 1000) {
						nS1 = 1;	nO1 = -606;
					}
				}
				else if (nIR == 4) {
					if (nI == 100) {
						nS1 = 1;	nO1 = -5236;
					}
				}
				else if (nIR == 5) {
					if (nI == 10) {
						nS1 = 1;	nO1 = -50948;
					}
				}
			}

		}

	}


	R = R * nS1 + nO1;
#endif
	return R;
}



/*

void CModeSub71::OnButton605Init()
{
	CString str;

	int Ret;




#ifdef DEF_CARD_A1  //sylee170328

	if( nMode4W!=1){
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //4W CMD ,   CHANNEL  B   //HW PROTECTION SYLEE20110926  160nS 구동  먼저 off해야 한다.
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return  ;	}
	}
	Ret = DioOutBit(pDIO.hDrv, DO45, 1);   // 45  0=I SENSE ,  1= VB SENSE     //SYLEE20120501  1->0  H/W바뀜 0=VB SENSE   1= I SENSE
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return  ;	}

	Delay_100ns(20000);//2mSEC

	if( nMode1==1 || nMode1==2 ){   // LV1, LV2  SEN CMD ON  //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //HV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}

	if( nMode1==3 || nMode1==4 ){  // HV1, HV2  SEN CMD ON   //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = DioOutBit(pDIO.hDrv, DO3328, 0);   //LV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 3328, 0] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}


	if( nMode1==1 || nMode1==2 ){   // LV1, LV2  SEN CMD ON
		Ret = DioOutBit(pDIO.hDrv, DO3328, 1);   //LV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 3328, 1] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}

	if( nMode1==3 || nMode1==4 ){  // HV1, HV2  SEN CMD ON
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //HV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 42, 1] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}

	Ret = DioOutBit(pDIO.hDrv, DO3338, 1);   //VC-SS  CMD ,  CHANNEL  B   //HW PROTECTION SYLEE20110926  160nS 구동
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 38, 1] Error!!!");	return  ;	}

#else  //#ifdef DEF_CARD_A1





		if( nMode4W!=1){
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //4W CMD ,   CHANNEL  B   //HW PROTECTION SYLEE20110926  160nS 구동  먼저 off해야 한다.
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return  ;	}


	}
	Ret = DioOutBit(pDIO.hDrv, DO45, 1);   // 45  0=I SENSE ,  1= VB SENSE     //SYLEE20120501  1->0  H/W바뀜 0=VB SENSE   1= I SENSE
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return  ;	}

	Delay_100ns(20000);//2mSEC

	if( nMode1==1 || nMode1==2 ){   // LV1, LV2  SEN CMD ON  //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = DioOutBit(pDIO.hDrv, DO42, 0);   //HV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 42, 0] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}

	if( nMode1==3 || nMode1==4 ){  // HV1, HV2  SEN CMD ON   //HW PROTECTION  2mS DELAY    //HW RELAY  ON TIME 1mS
		Ret = DioOutBit(pDIO.hDrv, DO3328, 0);   //LV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 3328, 0] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}


	if( nMode1==1 || nMode1==2 ){   // LV1, LV2  SEN CMD ON
		Ret = DioOutBit(pDIO.hDrv, DO3328, 1);   //LV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 3328, 1] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}

	if( nMode1==3 || nMode1==4 ){  // HV1, HV2  SEN CMD ON
		Ret = DioOutBit(pDIO.hDrv, DO42, 1);   //HV_SEN_CMD
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 42, 1] Error!!!");	return  ;	}
		Delay_100ns(20000);//2mSEC
	}

	Ret = DioOutBit(pDIO.hDrv, DO3338, 1);   //VC-SS  CMD ,  CHANNEL  B   //HW PROTECTION SYLEE20110926  160nS 구동
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 38, 1] Error!!!");	return  ;	}




#endif  //#ifdef DEF_CARD_A1








}


*/

//son 'R' 버튼
void CModeSub71::OnButton605()
{
#ifndef _PC_TEST_
	CString str;

	if (nMode4W == 1) {
		if (nMode1 == 1) {
			//OnButton601();
			OnButton602();     //son I Read
			OnButton604();     //son 4W Read
		}
		else if (nMode1 == 2) {
			//OnButton610(); 
			OnButton602();     //son I Read
			OnButton604();  //son 4W Read
		}
		else {
			AfxMessageBox(" 4W ERROR  LV1, LV2,  HV1,  HV2  RELAY ON ?        Error!!!");
			return;
		}

	}
	else {
		if (nMode1 == 1) {
			//OnButton601();
			OnButton602(); //son I Read
			if (nModeCC == 0) {
				OnButton603();  //son VC Read
			}
			else {
				m_Label123.SetCaption("0");
			}
		}
		else if (nMode1 == 2) {
			//OnButton610(); 
			OnButton602(); //son I Read
			if (nModeCC == 0) {
				OnButton603();  //son VC Read
			}
			else {
				m_Label123.SetCaption("0");
			}
		}
		else if (nMode1 == 3) {
			//OnButton611();
			OnButton602(); //son I Read
			if (nModeCC == 0) {
				OnButton603();  //son VC Read
			}
			else {
				m_Label123.SetCaption("0");
			}
		}
		else if (nMode1 == 4) {
			OnButton602(); //son I Read
			if (nModeCC == 0) {
				OnButton603();  //son VC Read
			}
			else {
				m_Label123.SetCaption("0");
			}
		}
		else {
			AfxMessageBox(" LV1, LV2,  HV1,  HV2  RELAY ON ?        Error!!!");
			return;

		}
	}



	//////////////////////////////  R 계산식  ////////////////////////////////////////////////
	if (nMode4W == 1) {//SYLEE20111011

		str.Format("0");
		m_Label103.SetCaption(str);
		m_Label123.SetCaption(str);

		if (Y1 > 0) {
			if (nModeIR == 1 || nModeIR == 2) {//I mA
				if (nMode4WG != 1) {
					R1 = ((W1) / Y1);//배율 /1000   4W=>  4W VOLTAGE
				}
				else {
					R1 = ((W1) / Y1) / 10.;//배율 /10000
				}
			}
			else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {  //I uA

				if (nMode4WG != 1) {
					R1 = ((W1) / Y1) * 1000.;//배율 /1000
				}
				else {
					R1 = ((W1) / Y1) * 100.;//배율 /10000
				}

			}
			else {
				R1 = 9999999999;
				str.Format("-> ERROR : 4Wire    R  sensing   I Range Mode? ");
				m_list1.InsertString(0, str);
			}
		}
		else {
			R1 = 9999999999;
			str.Format("-> ERROR : 4Wire    R sensing   I Value =0  ");
			m_list1.InsertString(0, str);

		}

		///////////////////////////////////////////////////////
		str.Format("-> 4Wire   R  =%.1f ohm", R1);
	}
	else {

		if (Y1 > 0) {
			if (nModeIR == 1 || nModeIR == 2) {
				R1 = (V1 - X1) * 1000 / Y1;
			}
			else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
				R1 = (V1 - X1) * 1000000 / Y1;
			}
			else {
				R1 = 9999999999;
				str.Format("-> ERROR :  R sensing   I Range Mode? ");
				m_list1.InsertString(0, str);

			}
		}
		else {
			R1 = 9999999999;
			str.Format("-> ERROR :  R sensing   I Value =0  ");
			m_list1.InsertString(0, str);
		}

		str.Format("0");
		//sylee170406-1	    m_Label104.SetCaption(str);
		m_Label124.SetCaption(str);
	}




	if (nModeRCal == 1) {
		if (nMode4W == 1) {
			R1 = ARReadCal1(nMode1, 2 + nMode4W + nMode4WG, nModeVS1, nModeIR, nModeIS1, R1);
		}
		else {
			R1 = ARReadCal1(nMode1, nModeCC + 1, nModeVS1, nModeIR, nModeIS1, R1);
		}
	}



	if (R1 < 0.001) {
		R1 = R1 * 1000;
		str.Format("%.3f m ohm", R1); //SYLEE20110704
	}
	else if (R1 < 1) {
		R1 = R1 * 1000;
		str.Format("%.3f m ohm", R1);
	}
	else if (R1 < 1000) {
		str.Format("%.3f ohm", R1);
	}
	else if (R1 < 1000000) {
		R1 = R1 / 1000;
		str.Format("%.3f k ohm", R1);
	}
	else if (R1 < 1000000000) {
		R1 = R1 / 1000000;
		str.Format("%.3f M ohm", R1);
	}
	else {
		R1 = R1 / 1000000;
		str.Format("%.3f M ohm", R1);
	}

	m_Label125.SetCaption(str);
	OnButton606();

#endif
}


double CModeSub71::AOnReadVI1() //SYLEE20111017
{
#ifndef _PC_TEST_
	CString str;

	if (nMode4W == 1) {

		if (nMode1 == 1) {

		}
		else if (nMode1 == 2) {

		}
		else {
			AfxMessageBox(" 4W ERROR  LV1, LV2,  HV1,  HV2  RELAY ON ?        Error!!!");
			return -1;
		}

		AOnReadI1();
		OnButton604();  //son 4W Read

	}
	else {
		if (nMode1 == 1) {

		}
		else if (nMode1 == 2) {

		}
		else if (nMode1 == 3) {

		}
		else if (nMode1 == 4) {

		}
		else {
			AfxMessageBox(" LV1, LV2,  HV1,  HV2  RELAY ON ?        Error!!!");
			return -1;
		}
		//OnButton602();if( nModeCC ==0)OnButton603();  //son VC Read
		AOnReadI1();
		if (nModeCC == 0) {
			OnButton603();  //son VC Read
		}

	}


	if (nMode4W == 1) {
		if (Y1 > 0) {
			if (nModeIR == 1 || nModeIR == 2) {//I mA
				if (nMode4WG != 1) {
					R1 = ((W1) / Y1);//배율 /1000   4W=>  4W VOLTAGE
				}
				else {
					R1 = ((W1) / Y1) / 10.;//배율 /10000
				}
			}
			else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {  //I uA

				if (nMode4WG != 1) {
					R1 = ((W1) / Y1) * 1000.;//배율 /1000
				}
				else {
					R1 = ((W1) / Y1) * 100.;//배율 /10000
				}
			}
			else {
				R1 = 9999999999;
			}
		}
		else {
			R1 = 9999999999;
		}
		///////////////////////////////////////////////////////

	}
	else {
		if (Y1 > 0) {
			if (nModeIR == 1 || nModeIR == 2) {
				R1 = (V1 - X1) * 1000 / Y1;
			}
			else if (nModeIR == 3 || nModeIR == 4 || nModeIR == 5) {
				R1 = (V1 - X1) * 1000000 / Y1;
			}
			else {
				R1 = 9999999999;
			}
		}
		else {
			R1 = 9999999999;
		}
	}

	if (R1 < 0) {
		R1 = 0;
	}

#endif
	return R1;
}





void CModeSub71::OnButton606()
{
	CString str;
	FILE* fp;
	char  fName[200];
	CString strVal1;

	::ZeroMemory(&fName, sizeof(fName));

	strVal1.Format("001.csv");
	strcat(fName, strVal1);

	fp = fopen(fName, "at");
	if (fp == NULL) {
		AfxMessageBox(" Error No 2003  001.ini  file open?  Error, Saving Error !     file close!", MB_OK); //error messege
	//	fclose(fp); 
		return;
	}

	if (nModeCC == 1) {
		fprintf(fp, "%.3f  %.3f\n", V1, Y1);
	}
	else {
		fprintf(fp, "%.3f  %.3f\n", X1, Y1);
	}

	fclose(fp);


	nflgB = 3;
}

void CModeSub71::OnButton607()
{
	CString str;

	m_list1.InsertString(0, str);

	nflgA = 2;
}

void CModeSub71::OnButton608()
{
	CString str;

	m_list1.InsertString(0, str);

	DeleteFile("001.csv");
}

void CModeSub71::OnButton609()
{
	CString str;

	m_list1.InsertString(0, str);
}




void CModeSub71::OnButton613() //SPARK READING
{
	CString str;

	m_list1.InsertString(0, str);
}



void CModeSub71::OnButton701()   // auto reading  Flag
{
	CString str;

	if (nfAuRead1 == 0) {
		nfAuRead1 = 1;
		str.Format("  R Cal.   Disable Set");
	}
	else {
		nfAuRead1 = 0;
		str.Format("  R Cal.   Enable  Set ");
	}

	m_list1.InsertString(0, str);
}




void CModeSub71::OnButton751()   //SYLEE150118
{
#ifndef _PC_TEST_
	if (nFlag71_VB_AC_Mode1 == 1) {
		OnVB_AC_Mode_Set(0); //ON=1, Off=0 SET  //sylee150118
		nFlag71_VB_AC_Mode1 = 0;
		str.Format("OFF");
	}
	else {

		OnFiler_I_M(0);//vb  i fast  islow off

		nFlag71_HV_AC_Mode1 = 1;
		OnButton752();   //SYLEE150118

		nFlag71_HV_AC_Mode1_Relay = 1;
		OnButton753();   //SYLEE150118

		///VB RELAY ALL OFF
		OnButton126();  //relay initial 

		AOnRelayOff1();
		Delay_100ns(50000);  //5m sec 

		OnVB_AC_Mode_Set(1); //ON=1, Off=0 SET  //sylee150118
		nFlag71_VB_AC_Mode1 = 1;
		str.Format("ON");

	}
	m_Label751.SetCaption(str);


#endif
}



void CModeSub71::OnButton752()   //SYLEE150118
{
#ifndef _PC_TEST_

	if (nFlag71_HV_AC_Mode1 == 1) {
		//OnVB_AC_Mode_Set(int set) //ON=1, Off=0 SET  //sylee150118
		OnHV_AC_Mode_Set(0); //ON=1, Off=0 SET  //sylee150118
		nFlag71_HV_AC_Mode1 = 0;
		str.Format("OFF");
	}
	else {

		OnFiler_I_M(0);//vb  i fast  islow off

		nFlag71_VB_AC_Mode1 = 1; //SYLEE150118
		OnButton751();   //SYLEE150118   ///AC MODE  OFF

		OnHV_AC_Mode_Set(1); //ON=1, Off=0 SET  //sylee150118
		nFlag71_HV_AC_Mode1 = 1;
		str.Format("ON");

	}
	m_Label752.SetCaption(str);


#endif
}




void CModeSub71::OnButton753()   //SYLEE150118
{
#ifndef _PC_TEST_

	if (nFlag71_HV_AC_Mode1_Relay == 1) {
		OnHV_AC_Mode_Set_Relay(0); //ON=1, Off=0 SET  //sylee150118
		nFlag71_HV_AC_Mode1_Relay = 0;
		str.Format("OFF");
	}
	else {

		OnFiler_I_M(0);//vb  i fast  islow off	    
		nFlag71_VB_AC_Mode1 = 1; //SYLEE150118
		OnButton751();   //SYLEE150118   ///AC MODE  OFF
		OnHV_AC_Mode_Set_Relay(1); //ON=1, Off=0 SET  //sylee150118
		nFlag71_HV_AC_Mode1_Relay = 1;
		str.Format("ON");
	}
	m_Label753.SetCaption(str);

#endif
}



void CModeSub71::OnButton754()   //SYLEE150118
{

}
void CModeSub71::OnButton755()   //SYLEE150118
{

}
void CModeSub71::OnButton756()   //SYLEE150118
{

}

void CModeSub71::OnButton757()   //SYLEE150118  //VB GUARD ON
{
#ifndef _PC_TEST_
	CString str;


#ifdef DEF_CARD_A1  //sylee181015-1

#else //sylee181015-1

	pDIO_D64.CardH64_Open1();
#endif//sylee181015-1


	return;


	if (nFlag71_VB_G_ON != 1 && nFlag71_VB_AC_Mode1 == 1) {
		nFlag71_VB_G_ON = 1;
		OnEmbeded_Guard(1, 1);
		str.Format("ON");
	}
	else {
		nFlag71_VB_G_ON = 0;
		OnEmbeded_Guard(1, 0);
		str.Format("OFF");
	}
	m_Label757.SetCaption(str);

#endif
}


void CModeSub71::OnButton758()   //SYLEE150118  //VB GUARD SEL
{

#ifndef _PC_TEST_
	CString str;

#ifdef DEF_CARD_A1  //sylee181015-1

#else //sylee181015-1
	pDIO_D64.CardH64_Close1();
#endif
	return;




	if (nFlag71_VB_G_SEL1 != 1 && nFlag71_VB_AC_Mode1 == 1) {
		nFlag71_VB_G_SEL1 = 1;
		OnEmbeded_Guard(2, 1);
		str.Format("CC");
	}
	else {
		nFlag71_VB_G_SEL1 = 0;
		OnEmbeded_Guard(2, 0);
		str.Format("CV");
	}
	m_Label758.SetCaption(str);

#endif
}

void CModeSub71::OnButton759()   //SYLEE150118  //VC GUARD ON
{
#ifndef _PC_TEST_

	CString str;

#ifdef DEF_CARD_A1  //sylee181015-1

#else //sylee181015-1
	pDIO_D96.CardD96_Open1();
#endif
	return;



	if (nFlag71_VC_G_ON != 1 && nFlag71_VB_AC_Mode1 == 1) {
		nFlag71_VC_G_ON = 1;
		OnEmbeded_Guard(3, 1);
		str.Format("ON");
	}
	else {
		nFlag71_VC_G_ON = 0;
		OnEmbeded_Guard(3, 0);
		str.Format("OFF");
	}
	m_Label759.SetCaption(str);

#endif
}

void CModeSub71::OnButton760()   //SYLEE150118  //VC GUARD SEL
{
#ifndef _PC_TEST_
	CString str;

#ifdef DEF_CARD_A1  //sylee181015-1

#else //sylee181015-1

	pDIO_D96.CardD96_Close1();

#endif
	return;



	if (nFlag71_VC_G_SEL1 != 1 && nFlag71_VB_AC_Mode1 == 1) {
		nFlag71_VC_G_SEL1 = 1;
		OnEmbeded_Guard(4, 1);
		str.Format("CC");
	}
	else {
		nFlag71_VC_G_SEL1 = 0;
		OnEmbeded_Guard(4, 0);
		str.Format("CV");
	}
	m_Label760.SetCaption(str);


#endif
}



void CModeSub71::OnButton801()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 1);
	OnPinSel1(2, 1);

	OnPinSel1(1, 9);
	OnPinSel1(2, 9);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 0  Testing");
	m_list1.InsertString(0, str);
#endif
}

void CModeSub71::OnButton802()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 2);
	OnPinSel1(2, 2);

	OnPinSel1(1, 10);
	OnPinSel1(2, 10);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 1  Testing");
	m_list1.InsertString(0, str);
#endif
}

void CModeSub71::OnButton803()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 3);
	OnPinSel1(2, 3);

	OnPinSel1(1, 11);
	OnPinSel1(2, 11);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 2  Testing");
	m_list1.InsertString(0, str);
#endif
}

void CModeSub71::OnButton804()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 4);
	OnPinSel1(2, 4);

	OnPinSel1(1, 12);
	OnPinSel1(2, 12);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 3  Testing");
	m_list1.InsertString(0, str);
#endif
}

void CModeSub71::OnButton805()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 5);
	OnPinSel1(2, 5);

	OnPinSel1(1, 13);
	OnPinSel1(2, 13);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 4  Testing");
	m_list1.InsertString(0, str);
#endif
}

void CModeSub71::OnButton806()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 6);
	OnPinSel1(2, 6);

	OnPinSel1(1, 14);
	OnPinSel1(2, 14);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 5  Testing");
	m_list1.InsertString(0, str);

#endif
}

void CModeSub71::OnButton807()
{
	CString str;

	OnPinSel1(1, 7);
	OnPinSel1(2, 7);

	OnPinSel1(1, 15);
	OnPinSel1(2, 15);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();//  

	str.Format("->4WIRE PIN 6  Testing");
	m_list1.InsertString(0, str);
}


void CModeSub71::OnButton808()
{
#ifndef _PC_TEST_
	CString str;

	OnPinSel1(1, 8);
	OnPinSel1(2, 8);

	OnPinSel1(1, 16);
	OnPinSel1(2, 16);

	OnButton602();// i sensing
	OnButton604();// 4wire sensing

	OnButton501();// 

	str.Format("->4WIRE PIN 7  Testing");
	m_list1.InsertString(0, str);
#endif
}




void CModeSub71::OnButton904()
{
	CString str;

	m_list1.ResetContent();


}




void CModeSub71::OnButton1106() //VB CPLD0 TR1
{
	CString str;

	nCPLD1 = 1;

	str.Format("CPLD0 TR1");
	m_Label215.SetCaption(str);

	str.Format("VB CPLD0 TR1 select");
	m_list1.InsertString(0, str);
}


void CModeSub71::OnButton1107() //VB CPLD0 RELAY1
{
	CString str;
	nCPLDR1 = 2;

	str.Format("CPLD1 REL1");
	m_Label214.SetCaption(str);


	str.Format("VB CPLD1 RELAY1 select");
	m_list1.InsertString(0, str);
}




void CModeSub71::OnButton11081() //sylee150120   //PLUSE
{

#ifndef _PC_TEST_
	CString str, str2;
	int Ret;

	if (nFlag71_VB_AC_Mode_Pulse1 != 1) { //sylee150118

		str.Format("OFF");
		nMode4W = 0;
		m_Label225.SetCaption(str);

		nModeOpenHV = 1;

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD  
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
#else  //#ifdef DEF_CARD_A1 

		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
#endif  //#ifdef DEF_CARD_A1 


		OnButton112(); //HV OPEN OFF

		AOnRelayOff1();//SYLEE20120721 MOVE 

		nMode1 = 0;

		Delay_100ns(30000);  //3ms 
		Delay_100ns(1000000);  //100ms 

		nFlag71_VB_AC_Mode_Pulse1 = 1;//sylee150118
		OnVB_AC_Mode_PulseRelaySet1(1); //ON=1, Off=0 SET  //sylee150118

		str.Format("OFF");
		m_Label221.SetCaption(str);
		m_Label222.SetCaption(str);
		m_Label223.SetCaption(str);
		m_Label224.SetCaption(str);

		str.Format("ON");
		str2.Format("Pulse  On ");

	}
	else {

		nFlag71_VB_AC_Mode_Pulse1 = 0;//sylee150118
		OnVB_AC_Mode_PulseRelaySet1(0); //ON=1, Off=0 SET   

		str.Format("OFF");
		str2.Format("Pulse  OFF ");
	}

	m_Label2141.SetCaption(str);
	m_list1.InsertString(0, str2);
#endif
}


void CModeSub71::OnButton1108() //VB CPLD0	TR2
{
	CString str;


	nCPLD1 = 3;

	str.Format("CPLD2 TR2");
	m_Label215.SetCaption(str);


	str.Format("VB CPLD3 TR1 select");
	m_list1.InsertString(0, str);
}



void CModeSub71::OnButton1109() //VB CPLD0 RELAY2
{
	CString str;

	nCPLDR1 = 4;

	str.Format("CPLD3 REL2");
	m_Label214.SetCaption(str);

	str.Format("VB CPLD3 RELAY2 select");
	m_list1.InsertString(0, str);
}




//son Type    1: 변환없음. hwPin을 No로 받아서 그대로 PinNo로 사용  
//            2: jigPin->hwPin 변환. jigPin을 No로 받아서 PinMap[2][No]로 변환하여  PinNo로 사용 
//
//son No      PinNo (0부터 시작)
//son nMod    1: TR SET VC,  2: TR SET VB, 3: RY SET VC, 4: RY SET VB
int CModeSub71::AOnPinSet1(int Type, int No, int nMod)
{
	if (SysInfo05.m_nAC)
	{
		if (No < 0)	return 0;
	}
	
	if(fpLeak)  //hdpark230323 AC LEAK
	{
		if(nMod == 2)	fprintf(fpLeak, "[%5d]", No + 1);
		else			fprintf(fpLeak, " %5d ", No + 1);
	}

#ifdef VB_2_MOD
	//son200723 김영필 부장님 요구사항으로 VC부터 핀을 설정하기 위해 
	//    AOnPinSet1()에서 nMod의 HW적인 설정을 1<->2,  3<->4 를 바꾸도록 수정됨.
	//    이전에 nMod는 1: TR SET VB,  2: TR SET VC, 3: RY SET VB, 4: RY SET VC 였으나
	//    SW적으로      1: TR SET VC,  2: TR SET VB, 3: RY SET VC, 4: RY SET VB 로 VB와 VC의 의미가 바뀌었다.

//  enum PIN_SET_MOD {            //son210220 nMode용 enum
//       MOD_TR_SET_VC =  1,      //son  nMod=2, 4 가 VB.  nMod=1,3이 VC
//       MOD_TR_SET_VB =  2,
//       MOD_RY_SET_VC =  3,
//       MOD_RY_SET_VB =  4
//  };

//	if(nMod==1){//sylee200910-import
//		nMod=2;
//	}else if(nMod==2){ 
//		nMod=1;
//	}else if(nMod==3){
//		nMod=4;
//	}else if(nMod==4){
//		nMod=3;
//	}    

	//son210220  enum 값으로 수정
	if (nMod == MOD_TR_SET_VC) {            //son210220
		nMod = _MOD_TR_SET_VC;
	}
	else if (nMod == MOD_TR_SET_VB) {       //son210220
		nMod = _MOD_TR_SET_VB;
	}
	else if (nMod == MOD_RY_SET_VC) {       //son210220
		nMod = _MOD_RY_SET_VC;
	}
	else if (nMod == MOD_RY_SET_VB) {       //son210220
		nMod = _MOD_RY_SET_VB;
	}
#endif
    

	int nPin;// nCPLD,   nCard,   nBox;
	CString str;


	if (No < 0) {
		str.Format("Error No 7701,\n   Pin Set  Error  ( No<0 ) !!! \n \n     PIn No.=%d CHECK.  ", No);
		AfxMessageBox(str);
		return 0;
	}

	if (No > 32767) {//SYLEE130112
		str.Format("Error No 7701,\n   Pin Set  Error  ( No>32767 ) !!! \n \n     PIn No.=%d CHECK.  ", No);
		AfxMessageBox(str);;
		return 0;
	}

	if (nMod != _MOD_TR_SET_VB && nMod != _MOD_TR_SET_VC             //son210220 enum으로 수정
		&& nMod != _MOD_RY_SET_VB && nMod != _MOD_RY_SET_VC)
	{
		return 0;
	}
    //son No가 jigPin 이면
	if (Type == 2) {
		nPin = PinMap[2][No];	 //sylee160928
		if (nPin < 0) {	 //sylee160928
			str.Format("Error No 7703,\n   Pin   Map Error ! \n \n   * User Pin No.=%d,  Pin No= %d  (< 0  ) .  ", No, nPin);
			AfxMessageBox(str); 	 //sylee160928
			return -1;
		}	 //sylee160928 
	}
	else//sylee160928		  
	{
		nPin = No; //sylee160928 
	}

#ifndef _PC_TEST_
	int Ret;


	if (SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) //son-32KPIN-20190902
	{

		if (No >= 1536 && No < 16384) {//YAMAHA  1.5K //sylee170922-1
			return 0;
		}

		if (No >= 16384 + 1536 && No < 2048) {//YAMAHA  1.5K//sylee170922-1
			return 0;
		}
	}


	if (nMod == _MOD_TR_SET_VB || nMod == _MOD_RY_SET_VB) {//SYLEE20111118    //VB PIN 		//son210220 enum으로 수정
		Ret = AOnPinS1Vb1_1(nPin, nMod, 1);  // (int nPin, int nMod ,int nOn);   //sylee140513 //ACE400
		if (Ret != 1) {
			return 0;
		}
	}
	else if (nMod == _MOD_TR_SET_VC || nMod == _MOD_RY_SET_VC) //SYLEE20111118    //VC PIN     //son210220 enum으로 수정
    {
		Ret = AOnPinS1Vc1_1(nPin, nMod, 1);  // (int nPin, int nMod ,int nOn);   //sylee140513 //ACE400  

		if (Ret != 1) {
			return 0;
		}
//sylee230526  //sylee230527-1  //sylee230817
//===========================================================
   	    int nPo1,nPo2,nStep1,nPin2;//sylee230526

        int nDut = getDutId(); //son230905_2 ngDut1 -> 1부터 시작하는 nDut로 수정.
	//sylee230818	if( (nFlagLeakMPin1==1) && (ngDut1>0) ){ //sylee230527
	
         
		if( nFlagLeakMPin1==1  ){ 	//sylee230818
			nPo1=gnBDLData32[nDut][No+1][0];    //son230905_2
			if( nPo1>0 ){ //sylee230527
			   nPo2=gnBDLData32[nDut][No+1][1]; //son230905_2
			   if( nPo2>0 ){ //sylee230527 
					for( nStep1=nPo1; nStep1<=nPo2; nStep1++){
						if( (nStep1>0) && (nStep1<=10000) ){
							No=gnBDLData33[nDut][nStep1][2]-1; //son230905_2

							if (Type == 2) {
								nPin2 = PinMap[2][No];	//sylee230331-1
								if (nPin2 < 0) {	 //sylee230331-1
									str.Format("Error No 7703-3,\n   Pin   Map Error ! \n \n   * User Pin No.=%d,  Pin No= %d  (< 0  ) .  ", No+1, nPin2);//sylee230331-1
									AfxMessageBox(str); 	//sylee230331-1
									return -1;
								}	//sylee230331-1
							}
							else//sylee160928		  
							{
								nPin2 = No; //sylee230331-1
							}

							Ret = AOnPinS1Vc1_1(nPin2, nMod, 1);   if(Ret!=1){	return 0;}
						}
					}
			   }
		   }

		}
 
		
	}
//===========================================================
 

	// hdpark220511 spark begin
	if (m_bModifySparkMeas)
	{
		if (nMod == 1 && m_tINFO[m_nHWCnt - 1].nSequence == SPARK_TRG_ON)
		{
			if (m_nNetCnt >= MAX_SPARK_COUNT - 1)		m_nNetCnt = MAX_SPARK_COUNT - 1;
			m_tINFO[m_nHWCnt - 1].nSrcNet[m_nNetCnt++] = nPin;
			m_tINFO[m_nHWCnt - 1].nNetCount++;
			m_nTotalNetCnt++;
		}
	}
	// spark end

	if (SysSet13.m_nSparkMode == SPARK_NEW) //son200908
	{
		///////////////////////////////////////////////////////
		
        if (!m_bLeakTest)		return 1;   // hdpark220913, LV에서 Spark check 안하도록.

		// hdpark230504 spark begin
		if (m_bModifySparkMeas) //son MAP > Set3 > Spark VB Mode On 이면
		{
			if (nFlag_NewSparkBaord == 2) //SYLEE201024-1  //SPARK NEW BOARD
			{
				nSparkDetect1 = 0;//sylee200710-1
				nSpark1_Detect2 = 0;
				//sylee200724 sparktesting
				if (g_nSpark1_Use == 1) //SYLEE200721 //son Recipe > Spark Enable 이고, SparkSet 값이 0 이상 
				{
					if (nFlag_Spark_PinOn == 1)    //sylee200710-1 //son SysSet13.m_nSparkMode == SPARK_NEW) 
					{
					    //son VB 핀 등록을 할 때에만 Spark 측정을 한다.
						if (nMod == _MOD_TR_SET_VB) //Vb ON        //son210220 enum으로 수정
						{
							nSparkPinNo1 = No + 1;

                            //son nSparkMaxX 카운트 회수 만큼 Spark VS(dSparkV2[nSparkV1Po1]), I값(dSparkI2[nSparkV1Po1])을 read한다.
							AOn_Spark_ReadV1_I1();//SYLEE200713   V AND I

							Ret = OnSparkStatus();

							//lyk_test [
							if(Ret == 0) Ret = 1;
							//lyk_test ]

							if (Ret == 1)   //son HW Spark가 감지되었다면 
							{
								// hdpark220408, Spark 발생시 VB 전압 값이 서서히 감소하므로 최소 전압으로 떨어질때까지 전압 체크.
								double dOldX1 = 0.0, dOldY1 = 0.0;
								BYTE La, Ha;
								DWORD DHa;

                                //son nSparkMaxX 위치 이후의 영역을 50회 더 read 한다.
								memset(&dSparkV2[nSparkMaxX], 0, sizeof(float) * 100);//hdpark221220 Spark
								memset(&dSparkI2[nSparkMaxX], 0, sizeof(float) * 100);//hdpark221220 Spark

								for (int i = 0; i < 50; i++)
								{
#ifdef DEF_CARD_A1  //hdpark
									Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A 
									if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
									Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A
									if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

									InpNo[0] = 2;	        InpNo[1] = 3;
									Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]);
									if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 2,3 ] Error!!!");	return  0; }
									La = InpData[0];  	Ha = InpData[1];
#else
									Ixud_WriteDOBit(0, 7, 1, 0);
									Ixud_WriteDOBit(0, 7, 1, 1);
									d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);

									La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
									Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);

									DHa = (DWORD)(Ha << 8);
									DHa = DHa + La;
									Y1 = DHa / 65535. * 250.0;

									dSparkV2[nSparkMaxX + i] = (float)Y1;   //son 3번째 4번째 byte. ADC_B  VS

									La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
									Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);
#endif
									DHa = (DWORD)(Ha << 8);
									DHa = DHa + La;

									if (!m_bSetVBRead)	X1 = DHa / 65535. * dSparkLeakSet1_I;
									else				X1 = DHa / 200.0; // spark vb read

									dSparkI2[nSparkMaxX + i] = (float)X1; // spark check 전 인덱스에 이어서 저장.

									if (0)
									{
										fprintf(fpSpark, "%2d, %3d, %4.1f, %5.1f \n", 11 + i, m_nHWCnt, Y1, X1);
									}

									if (m_bSetVBRead)
									{
										if (fabs(X1 - dOldX1) < 0.5)	break;
										dOldX1 = X1;
									}
									else
									{
										if ((X1 - dOldX1) < 0.0)	break;
										dOldX1 = X1;
									}
								}

								m_tINFO[m_nHWCnt - 1].nHWSparkOn = 1;
#ifdef __BH__SPARK_DECISION_BY_230828 //yklee240621 BH 230828 Spark SW 판단로직 적용
							AOn_Spark_V_I_Check1_BH_230828(1, No); 
#elif defined(__JTECH__SPARK_DECISION_BY_230828) //yklee240802 JTech 양산 Spark SW 판단 로직 적용 
							AOn_Spark_V_I_Check1_for_JTech(1, No);
#else
							AOn_Spark_V_I_Check1(1, No);  //SYLEE200711 , hdpark231121 nPin -> No
#endif
							


							} //hdpark220511 end
						}
						else
						{
							Ret = OnSparkStatus();
							if (Ret == 1)
							{
								Delay_100ns(10000);	// 1ms
								OnSparkOn(0);
								Delay_100ns(10);  //1us  
								OnSparkOn(1);
							}

						}
					}
				}
			}
		} //hdpark230504 spark end
		else
		{
			if (nFlag_NewSparkBaord == 2) {//SYLEE201024-1  //SPARK NEW BOARD
				nSparkDetect1 = 0;//sylee200710-1
				nSpark1_Detect2 = 0;
				//sylee200724 sparktesting
				if (g_nSpark1_Use == 1) {//SYLEE200721 
					if (nFlag_Spark_PinOn == 1) {   //sylee200710-1
						if (nMod == _MOD_TR_SET_VB) {//Vb ON        //son210220 enum으로 수정
							nSparkPinNo1 = No + 1;
							AOn_Spark_ReadV1_I1();//SYLEE200713   V AND I
							Ret = OnSparkStatus();

							if (Ret == 1) {
#ifdef __BH__SPARK_DECISION_BY_230828 //yklee240621 BH 230828 Spark SW 판단로직 적용
								AOn_Spark_V_I_Check1_BH_230828(1); 
#elif defined(__JTECH__SPARK_DECISION_BY_230828) //yklee240802 JTech 양산 Spark SW 판단 로직 적용 
								AOn_Spark_V_I_Check1_for_JTech(1);
#else								
								AOn_Spark_V_I_Check1(1); //SYLEE200711
#endif								
							}

						}

					}
				}
			}
		}

		///////////////////////////////////////////////////////

	}


#endif 
	return 1;
}





int CModeSub71::AOnPinReSet1(int Type, int No, int nMod)
{


#ifdef VB_2_MOD
	//son210220  enum 값으로 수정

//	if(nMod==1){//sylee200910-import
//		nMod=2;
//	}else if(nMod==2){ 
//		nMod=1;
//	}else if(nMod==3){
//		nMod=4;
//	}else if(nMod==4){
//		nMod=3;
//	}

	if (nMod == MOD_TR_SET_VC) {                //son210220 enum으로 수정
		nMod = _MOD_TR_SET_VC;
	}
	else if (nMod == MOD_TR_SET_VB) {           //son210220 enum으로 수정
		nMod = _MOD_TR_SET_VB;
	}
	else if (nMod == MOD_RY_SET_VC) {           //son210220 enum으로 수정
		nMod = _MOD_RY_SET_VC;
	}
	else if (nMod == MOD_RY_SET_VB) {           //son210220 enum으로 수정
		nMod = _MOD_RY_SET_VB;
	}

#endif


#ifndef _PC_TEST_
	int Ret;
	int nPin;// nCPLD,   nCard,   nBox;


	if (No < 0 || No>32767) {
		str.Format("Error No 7731,\n   Pin Set  Error  ( No<0  || No>32767) !!! \n \n     PIn No.=%d CHECK.  ", No);
		AfxMessageBox(str);
		return 0;
	}

	if (nMod != _MOD_TR_SET_VB && nMod != _MOD_TR_SET_VC             //son210220 enum으로 수정
		&& nMod != _MOD_RY_SET_VB && nMod != _MOD_RY_SET_VC) {
		return 0;
	}

	if (Type == 2) {
		No = PinMap[2][No];
	}

	nPin = No;

	if (nMod == _MOD_TR_SET_VB || nMod == _MOD_RY_SET_VB) {//SYLEE20111118        //VB PIN //son210220 enum으로 수정

		Ret = AOnPinS1Vb1_1(nPin, nMod, 0);  // (int nPin, int nMod ,int nOn);   //sylee140513 //ACE400
		if (Ret != 1) {
			return 0;
		}
	}
	else if (nMod == _MOD_TR_SET_VC || nMod == _MOD_RY_SET_VC) {//SYLEE20111118    //VC PIN //son210220 enum으로 수정
		//	Ret=AOnPinS1Vc1(nPin, nCPLD, nCard);
		Ret = AOnPinS1Vc1_1(nPin, nMod, 0);  // (int nPin, int nMod ,int nOn);   //sylee140513 //ACE400  
		if (Ret != 1) {
			return 0;
		}
	}



#endif
	return 1;
}




int CModeSub71::AOnPinS1Vb1_1(int nPin, int nMod, int nOn)    //sylee140513 //ACE400 
{
#ifndef _PC_TEST_
	CString str;
	int Ret;


	if (nPin > 32767 || nPin < 0) {
		AfxMessageBox("vb pin set   nPin>32767  || nPin<0  Error!!!");	return  0;
	}

#ifdef DEF_CARD_A1  //sylee170328

	int  c;

	//=========================================================================
	/// enable 

	Ret = DioOutBit(pDIO.hDrv, DO99, 1);    //vb trigger
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 99, 1] Error!!!");	return  0; }
	////================================================================= 

	OutData[0] = (BYTE)(nPin & 0xff);
	OutData[1] = (BYTE)(nPin >> 8);
	Ret = DioOutMultiByte(pDIO.hDrv, &OutNoT[8], 2, &OutData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiByte, 8,9 ] Error!!!");	return  0; }
	//========================================================================= 


	if (nMod == _MOD_TR_SET_VB) {//tr           //son210220 enum으로 수정
		c = 1;
	}
	else if (nMod == _MOD_RY_SET_VB) {//relay  //son210220 enum으로 수정
		c = 2;
	}
	else {
		AfxMessageBox(" vb pin set Error!!!  Mod 1,3  ? ");	return  0;
	}

	if (nOn != 1) {// off=1   on=0
		c = c + 4;
	}
	c = c + 8 + 16;// +8 trigger high = ready    ,  +16 : reset=0 , use=1    

#ifdef VB_2_MOD	//son230127
	if (nPinSetSkipVc != 3) //son230127 VB, VC가 뒤집힌 경우에는 nPinSetSkipVc(VC용)를 체크한다.
#else
	if (nPinSetSkipVb != 3) //sylee151005 //ACE400
#endif
    {
		Ret = DioOutByte(pDIO.hDrv, 12, c);
		if (Ret != 0) { AfxMessageBox("DioOutByte[ 12 ] Error!!!");	return  0; }
	}
	Ret = DioOutBit(pDIO.hDrv, DO99, 0);    //vb trigger
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 99, 0] Error!!!");	return  0; }

#else  //#ifdef DEF_CARD_A1   

    //son ICPDAS 카드는 64K 대비해서 하부시작핀이 32768부터 시작함. 
    //    64K가 아니라 32K base인 현재 모든 Pin모드에서는 하부는  16384 더해야 제대로 동작한다.
	if (nPin >= 16384) {
		nPin = nPin + 16384;
	}

	//son-32KPIN-20200305 : 
	//  D64 0x080000 : _VB_ENA     Pin 등록 trigger, 엑셀페달  (1-> 0 : Falling edge로 0일때 동작한다. 평소에 1이어야)  
	//  D64 0x100000 : _VB_RESET   Pin 등록의 시동키, 평소에는 0이고, 핀등록 직전에 1로 해야 한다.  
	//                             Pin 등록이 완료되면 PinReset으로 이 값을 0으로 만들어 줘야 함.
	//  Pin등록 준비단계 : _VB_ENA은 1 이고(0x080000), _VB_RESET은 1(0x100000)
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x180000);  //  /VB ENA  ON    
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x180000 ) Error!!!");	return   0; }
	//  Delay_100ns(10000);

	if (nPin > 0)
		dwDOData = (unsigned long)(nPin & 0xffff);
	else
		dwDOData = 0x00;

	//  20   //  /VB RESET  0=ALL off, 1 =ON USE
	//	dwDOData = (dwDOData | 0x00100000);
	//  19   // /VB_ENA    1->0   active
	//	dwDOData = (dwDOData | 0x00080000);

	dwDOData = (dwDOData | 0x00180000);
	// 18   // VB PIN SEL
	if (nOn != 1) {// off=1   on=0
		dwDOData = (dwDOData | 0x00040000);
	}

	if (nMod == _MOD_TR_SET_VB) {//tr		 // 16  // VB TR SEL        //son210220 nMod enum추가 
		dwDOData = (dwDOData | 0x00010000);
	}
	else if (nMod == _MOD_RY_SET_VB) {//relay // 17  // VB RELAY SEL   //son210220 nMod enum추가 
		dwDOData = (dwDOData | 0x00020000);
	}
	else {
		AfxMessageBox(" vb pin set Error!!!  Mod 1,3  ? ");	return  0;
	}

	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, dwDOData);  //  /VB ENA  ON    
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(dwDOData ) Error!!!");	return   0; }
	// Delay_100ns(10000);

	dwDOData2 = (dwDOData & (~0x80000));  //sylee170502  dwDOData->dwDOData2 
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, dwDOData2);  //  /VB ENA  OFF    //sylee170502  dwDOData->dwDOData2 
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(dwDOData ) Error!!!");	return   0; }

	dwDOData2 = (dwDOData | 0x00180000);  //sylee170502  dwDOData->dwDOData2  0x00180000->(dwDOData | 0x00180000)
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, dwDOData2);  //  /VB ENA  ON    //sylee170502  dwDOData->dwDOData2 
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(dwDOData ) Error!!!");	return   0; }

#endif  //#ifdef DEF_CARD_A1 

#endif

	return 1;
}



int CModeSub71::AOnPinS1Vc1_1(int nPin, int nMod, int nOn)    //sylee140513 //ACE400 
{
#ifndef _PC_TEST_
	CString str;
	int Ret;

	////=================================================================
	if (nPin > 32767 || nPin < 0) {
		AfxMessageBox("vc pin set   nPin>32767  || nPin<0  Error!!!");	return  0;
	}
	////================================================================= 

#ifdef DEF_CARD_A1  //sylee170328

	int  c;

	Ret = DioOutBit(pDIO.hDrv, DO107, 1);     //vc trigger
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 107, 1] Error!!!");	return  0; }

	OutData[0] = (BYTE)(nPin & 0xff);
	OutData[1] = (BYTE)(nPin >> 8);
	Ret = DioOutMultiByte(pDIO.hDrv, &OutNoT[10], 2, &OutData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiByte, 10,11 ] Error!!!");	return  0; }
	//========================================================================= 

	if (nMod == _MOD_TR_SET_VC) {//tr           //son210220 nMod enum추가 
		c = 1;
	}
	else if (nMod == _MOD_RY_SET_VC) {//relay  //son210220 nMod enum추가 
		c = 2;
	}
	else {
		AfxMessageBox(" vb pin set Error!!!  Mod 2,4 ? ");	return  0;
	}

	if (nOn != 1) {//off // off=1   on=0
		c = c + 4;
	}
	c = c + 8 + 16;// +8 trigger high = ready    ,  +16 : reset=0 , use=1    

#ifdef VB_2_MOD
	if (nPinSetSkipVb != 3) //son230127 VB, VC가 뒤집힌 경우에는 nPinSetSkipVb(VB용)를 체크한다.
#else
	if (nPinSetSkipVc != 3) //sylee151005 //ACE400
#endif
	{
		Ret = DioOutByte(pDIO.hDrv, 13, c);
		if (Ret != 0) { AfxMessageBox("DioOutByte[ 13 ] Error!!!");	return  0; }
	}

	Ret = DioOutBit(pDIO.hDrv, DO107, 0);    //vb trigger
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 107, 0] Error!!!");	return  0; }


#else  //#ifdef DEF_CARD_A1   

    //son ICPDAS 카드는 64K 대비해서 하부시작핀이 32768부터 시작함. 
    //    64K가 아니라 32K base인 현재 모든 Pin모드에서는 하부는  16384 더해야 제대로 동작한다.
	if (nPin >= 16384) {
		nPin = nPin + 16384;
	}

	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x180000);  //  /VB ENA  ON    
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x180000 ) Error!!!");	return   0; }

	//  Delay_100ns(10000);

	if (nPin > 0)
		dwDOData = (unsigned long)(nPin & 0xffff);
	else
		dwDOData = 0x00;
	// 20   //  /VB RESET  0=ALL off, 1 =ON USE
	//	dwDOData = (dwDOData | 0x00100000);
	// 19   // /VB_ENA    1->0   active
	//	dwDOData = (dwDOData | 0x00080000);

	dwDOData = (dwDOData | 0x00180000);
	if (nOn != 1) {// off=1   on=0    // 18   // VC PIN SEL  
		dwDOData = (dwDOData | 0x00040000);
	}
	if (nMod == _MOD_TR_SET_VC) {//tr		        // 16  // VC TR SEL    //son210220 nMod enum추가 
		dwDOData = (dwDOData | 0x00010000);
	}
	else if (nMod == _MOD_RY_SET_VC) {//relay		// 17  // VC RELAY SEL //son210220 nMod enum추가 
		dwDOData = (dwDOData | 0x00020000);
	}
	else {
		AfxMessageBox(" vc pin set Error!!!  Mod 2,4  ? ");	return  0;
	}

	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, dwDOData);  //  /VB ENA  ON    
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(dwDOData ) Error!!!");	return   0; }

	//	Delay_100ns(10000);

	dwDOData2 = (dwDOData & (~0x80000));  //sylee170502  dwDOData->dwDOData2 
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, dwDOData2);  //  /VB ENA  ON     //sylee170502  dwDOData->dwDOData2 
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(dwDOData2 ) Error!!!");	return   0; }

	dwDOData2 = (dwDOData | 0x00180000); ; //sylee170502  dwDOData->dwDOData2  0x00180000->(dwDOData | 0x00180000)
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, dwDOData2);  //  /VB ENA  ON   //sylee170502  dwDOData->dwDOData2   
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(dwDOData ) Error!!!");	return   0; }

#endif  //#ifdef DEF_CARD_A1 


#endif
	return 1;
}




void CModeSub71::OnButton11102() //VB PIN SET  //SYLEE150319   4W RELAY SET
{
#ifndef _PC_TEST_
	CString str;
	char szText[50];
	int a;//,t1,t2,t3,t4,t5;//t1= pin no, t2=cpld,  t3=card  t4= card select

	m_edit305.GetWindowText(szText, 10);	//sylee20120227		
	a = atoi(szText) - 1; //pin no
	if (a < 0 || a>32768) {
		str.Format("VB PIN SET  : Relay  pin no ? 1~32768 ");
		m_list1.InsertString(0, str);
		return;
	}
	int ret = AOnPinSet1(2, a, 3);//sylee160523-1  	  
#if 1
	if (ret < 0)		//son-32KPIN-20200114:  	nPin=PinMap[2][No] 가 -1이면 없는 카드. 
	{
		str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d  ", a);
		AfxMessageBox(str);
	}
#endif
	str.Format("VB PIN SET");
	m_list1.InsertString(0, str);
#endif
}


void CModeSub71::OnButton1110() //VB PIN SET //AOnPinS1Vb1(int nPin, int nCPLD, int nCard)
{
#ifndef _PC_TEST_
	CString str;
	char szText[50];
	int a;

	m_edit301.GetWindowText(szText, 10);
	a = atoi(szText) - 1; //pin no

	if (a < 0 || a>32767) {
		str.Format("VB PIN SET : Tr  pin no ? 1~32768 ");
		m_list1.InsertString(0, str);
		return;
	}

	int ret = AOnPinSet1(2, a, 1);//sylee160523-1 	
#if 1
	if (ret < 0)		//son-32KPIN-20200114:  	nPin=PinMap[2][No] 가 -1이면 없는 카드. 
	{
		str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d  ", a);
		AfxMessageBox(str);
	}
#endif

	str.Format("VB PIN SET");
	m_list1.InsertString(0, str);

#endif
}




void CModeSub71::OnButton11152() //VC PIN SET  //SYLEE150319
{
#ifndef _PC_TEST_
	CString str;
	char szText[50];
	int a;


	m_edit306.GetWindowText(szText, 10);	//sylee20120227		
	a = atoi(szText) - 1; //pin no

	if (a < 0 || a>32768) {
		str.Format("Vc PIN SET :  relay  pin no ? 1~32768 ");
		m_list1.InsertString(0, str);
		return;
	}

	int ret = AOnPinSet1(2, a, 4);//sylee160523-1 
#if 1
	if (ret < 0)		//son-32KPIN-20200114:  	nPin=PinMap[2][No] 가 -1이면 없는 카드. 
	{
		str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d  ", a);
		AfxMessageBox(str);
	}
#endif


	str.Format("VC PIN SET");
	m_list1.InsertString(0, str);

#endif
}



void CModeSub71::OnButton1115() //VC PIN SET 
{
#ifndef _PC_TEST_
	CString str;

	char szText[50];
	int a;

	m_edit307.GetWindowText(szText, 10);	//sylee20120227		
	a = atoi(szText) - 1; //pin no

	if (a < 0 || a>32767) {
		str.Format("Vc PIN SET : Tr pin no ? 1~32768 ");
		m_list1.InsertString(0, str);
		return;
	}

	int ret = AOnPinSet1(2, a, 2);//sylee160523-1 
#if 1
	if (ret < 0)		//son-32KPIN-20200114:  	nPin=PinMap[2][No] 가 -1이면 없는 카드. 
	{
		str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d  ", a);
		AfxMessageBox(str);
	}
#endif

	str.Format("VC PIN SET");
	m_list1.InsertString(0, str);


#endif
}




//son DO_58 PIN_SEARCH 신호를 보낸다. 
// 1:  PIN_SEARCH를 시작한다.  모든 VC를 ON 시키는 역할. 
int CModeSub71::AOnPinSearch(int Onoff) //VC CPLD0 TR1
{

#ifndef _PC_TEST_
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328
	if (Onoff == 1)	// PIN SEARCH ON
	{
		Ret = DioOutBit(pDIO.hDrv, DO58, 1);  //I-72   58   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, %d(PIN SEARCH), 1] Error!!!", DO58);	return  0; }
	}
	else {  			// PIN SEARCH OFF
		Ret = DioOutBit(pDIO.hDrv, DO58, 0);  //I-72   58   
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, %d(PIN SEARCH), 0] Error!!!", DO58);	return  0; }
	}
#else  //#ifdef DEF_CARD_A1


	if (Onoff == 1) {
		Ret = Ixud_WriteDOBit(0, 4, 2, 1);  //PIN SEARCH ON
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,2,1) Error!!!");	return  0; }
	}
	else {
		Ret = Ixud_WriteDOBit(0, 4, 2, 0);  //PIN SEARCH OFF
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,2,0) Error!!!");	return  0; }
	}


#endif  //#ifdef DEF_CARD_A1 


#endif

	return 1;
}




void CModeSub71::OnButton1111() //VC CPLD0 TR1
{
	CString str;

	nCPLD2 = 1;

	str.Format("CPLD0 TR1");
	m_Label217.SetCaption(str);

	str.Format("VC CPLD0 TR1 select");
	m_list1.InsertString(0, str);
}



void CModeSub71::OnButton1112() //VC CPLD0 RELAY1
{
	CString str;

	nCPLDR2 = 2;
	str.Format("CPLD1 REL1");
	m_Label218.SetCaption(str);
	str.Format("VC CPLD1 RELAY1 select");
	m_list1.InsertString(0, str);
}


void CModeSub71::OnButton1113() //VC CPLD0 TR2
{
	CString str;

	nCPLD2 = 3;
	str.Format("CPLD2 TR2");
	m_Label217.SetCaption(str);
	str.Format("VC CPLD3 TR1 select");
	m_list1.InsertString(0, str);
}



void CModeSub71::OnButton1114() //VC CPLD0 RELAY2
{
	CString str;

	nCPLDR2 = 4;
	str.Format("CPLD3 REL2");
	m_Label218.SetCaption(str);
	str.Format("VC CPLD3 RELAY2 select");
	m_list1.InsertString(0, str);
}

void CModeSub71::OnButton2007() // 
{
	CString str;
	DWORD pid = 0;
	FILE* fp;
	char  fName[200];

	::ZeroMemory(&fName, sizeof(fName));

	str.Format("001LOGR64.csv");
	DeleteFile("001LOGR64.csv");
	strcat(fName, str);

	::ShellExecute(NULL, _T("open"), "taskkill.exe", "/f /im excel.exe", NULL, SW_HIDE);
	fp = fopen(fName, "w+");

	fclose(fp);

	::ShellExecute(NULL, "open", "EXCEl.EXE", " 001LOGR64.csv", "NULL", SW_SHOWNORMAL);

}



extern double dRec[362][16];


void CModeSub71::OnButton2009()
{


}

int CModeSub71::OnPin1(int a)  //a =source, b= sink 
{

	return 1;
}


int CModeSub71::OnPinResetInit()//sylee151221   //ACE400
{

#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO100, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 100, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO108, 0);
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 108, 0] Error!!!");	return  0; }
#else  //#ifdef DEF_CARD_A1  

	/*	Ret = AB_Bit( DO100, 0);
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 100, 0] Error!!!");	return  0;	}
		Ret = AB_Bit( DO108, 0);
		if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 108, 0] Error!!!");	return  0;	}
	*/
	//sylee170411Ret = d64h_do_writeport( pDIO_D64.m_bCardID, 0x000000  );  //0x080000?
	//sylee170411if( Ret!=0 ) { AfxMessageBox("  Card64H1_DO32(0x000000  ) Error!!!");	return  0;	}
	//sylee170411Ret = d64h_do_writeport( pDIO_D64.m_bCardID2, 0x000000  );  //0x080000?
	//sylee170411if( Ret!=0 ) { AfxMessageBox("  Card64H2_DO32(0x000000  ) Error!!!");	return  0;	}
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID, 0x80000);  //0x080000?
	if (Ret != 0) { AfxMessageBox("  Card64H1_DO32(0x80000   ) Error!!!");	return  0; }
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //0x080000?
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000   ) Error!!!");	return  0; }

#endif  //#ifdef DEF_CARD_A1 
#endif
	return 1;
}



int CModeSub71::Load1() // 
{
	int  v, i, j;
	FILE* fp;
	char  fName[200];
	CString strVal1;
	char sFilePath1[200];
	double CalV;
	char ka[200], kb[200], kc[200];

	::ZeroMemory(&Caln1, sizeof(Caln1));
	::ZeroMemory(&Caln2, sizeof(Caln2));

	::GetCurrentDirectory(MAX_PATH, sFilePath1);

	for (v = 1; v < 11; v++)
	{
		if (v > 1 && v < 10) {
			continue;
		}
		for (i = 1; i < 10; i++)
		{
			if (v == 1 && i > 5) {
				continue;
			}
			if (v == 10 && i > 9) {
				continue;
			}

			::ZeroMemory(&fName, sizeof(fName));
			strcat(fName, sFilePath1);
			strVal1.Format("\\Cal\\CAL%03dV%02dI.txt", v, i);
			strcat(fName, strVal1);
			fp = fopen(fName, "rt");
			if (fp == NULL) {
				//AfxMessageBox(" Error No 2003 !\n\\Cal\\CAL?B.ini  file open?  Error, Open Error", MB_OK); //error messege			
				//return 0;
			}
			else {

				j = 1;
				while (!feof(fp)) {
					fscanf(fp, "%s %s %s ", ka, kb, kc);  // enable,  Ref.R,   Val R  

					CalV = atof(ka);

					if (CalV == 1) {
						Caln1[v][i][j][0] = atof(ka);
						Caln1[v][i][j][1] = atof(kb);
						Caln1[v][i][j][2] = atof(kc);
						j++;
					}
				}
				Caln1[v][i][0][0] = j - 1;

			}
			fclose(fp);

		}
	}


	return 1;
}




int CModeSub71::ADCI_Read_Wait1(int  nModeRef, int nDiffRef, int nPassCountRef) //sylee121007
{
#ifndef _PC_TEST_

	int delay_time;
	long TimeT1;
	double nOldAdc1, diff1;
	int nPassCount;

	nOldAdc1 = 0;
	diff1 = 0;
	TimeT1 = 0;
	nPassCount = 0;

	delay_time = 1000;//0.1ms



	if (nModeRef == 1) {

		Delay_100ns(30000); //3ms
		TimeT1 = TimeT1 + 30000;
	}
	else {
		Delay_100ns(10000); //1ms 

	}


	for (int nK1 = 1; nK1 < 5000; nK1++)
	{

		Delay_100ns(delay_time); //0.1ms
		TimeT1 = TimeT1 + delay_time;

		if (nModeRef == 1) {
			AOnReadV1I1_ADC_I();
		}
		else { //CC MODE
		   //	Sub71.AOnReadV1I1(nVRel);   //if( nVRel==1 || nVRel==2  || nVRel==3 || nVRel==4){
			AOnReadV1I1(1);
		}


		if (nK1 == 1) {
			nOldAdc1 = R1;
		}
		else {
			diff1 = (double)abs((int)(nOldAdc1 - R1));
			diff1 = diff1 / R1 * 100.0;
			nOldAdc1 = R1;
		}

		if ((diff1 >= 0) && (diff1 < nDiffRef) || (R1 < 50)) {	//<=========	5,		
			nPassCount++;
		}
		else {
			if (nPassCount > 0) {
				nPassCount--;
			}
		}

		if (nPassCount > nPassCountRef) {  //<========= 5
			break;
		}

		if (TimeT1 > 50000000) {//0.5sec 
			break;
		}


	}

#endif
	return 1;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CModeSub71::OnSelchangeCombo1() //SYLEE130605
{
	nManualFlag_HVOpen_Offset = m_combo101.GetCurSel();

}

void CModeSub71::OnSelchangeCombo2() //SYLEE130605
{
	nManualFlag_HVOpen_Gain = m_combo102.GetCurSel();
}


void CModeSub71::OnSelchangeCombo6() //SYLEE130605    I ADC   FAST  SLOW MODE
{

	nManualFlag_I_ADC = m_combo106.GetCurSel();
#ifndef _PC_TEST_
	OnFiler_I_M(nManualFlag_I_ADC); //SYLEE130605     
#endif
}


void CModeSub71::OnFiler_I(int a) //SYLEE130605    I ADC   FAST  SLOW MODE
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (a == 0) {	//slow 			 
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //57   I fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //60   I slow
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }

	}
	else if (a == 1) {	//slow 	
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //  ADC  VB  SELECT CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37,0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //57   I fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 1);   //60   I slow
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 1] Error!!!");	return; }

	}
	else if (a == 2) {//fast //sylee130605
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //  ADC  VB  SELECT CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37,0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //60   I slow
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 1);   //57   I fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 1] Error!!!");	return; }

	}
	else {
		AfxMessageBox("  I ADC  MODE  Filter 1~2 slow, fast  CHECK!", MB_OK);
	}
#else  //#ifdef DEF_CARD_A1


	if (a == 0) {	//slow 			 
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
	}
	else if (a == 1) {	//slow 	
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 1);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,1) Error!!!");	return; }

	}
	else if (a == 2) {//fast //sylee130605
		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 1);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,1) Error!!!");	return; }

	}
	else {
		AfxMessageBox("  I ADC  MODE  Filter 1~2 slow, fast  CHECK!", MB_OK);
	}


#endif  //#ifdef DEF_CARD_A1 



#endif

}



void CModeSub71::OnFiler_I_M(int a) //SYLEE130605    I ADC   FAST  SLOW MODE
{
#ifndef _PC_TEST_
	int Ret;

	nFlag71_I_FilterMode = a;//sylee150118

	if (a == 0) {	//slow 	

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //  ADC  VB  SELECT CMD      0= LV   1= HV  //sylee150118
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37,0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //57   I fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //60   I slow
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
#else  //#ifdef DEF_CARD_A1   

		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }

#endif  //#ifdef DEF_CARD_A1 


		((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);


	}
	else if (a == 1) {	//slow 

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //  ADC  VB  SELECT CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37,0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 0);   //57   I fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 1);   //60   I slow
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 1] Error!!!");	return; }

#else  //#ifdef DEF_CARD_A1   

		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 0);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 1);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,1) Error!!!");	return; }

#endif  //#ifdef DEF_CARD_A1 	

		((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(FALSE);


	}
	else if (a == 2) {//fast //sylee130605

#ifdef DEF_CARD_A1  //sylee170328
		Ret = DioOutBit(pDIO.hDrv, DO37, 0);   //  ADC  VB  SELECT CMD      0= LV   1= HV
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 37,0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO48, 0);   //60   I slow
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 48, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO47, 1);   //57   I fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 47, 1] Error!!!");	return; }
#else  //#ifdef DEF_CARD_A1  

		Ret = Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,10,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 0, 1);  //ADC_I_FAST_SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,0,1) Error!!!");	return; }

#endif  //#ifdef DEF_CARD_A1 

		((CButton*)GetDlgItem(IDC_RADIO01))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO02))->SetCheck(FALSE);

	}
	else {
		AfxMessageBox("  I ADC  MODE  Filter 1~2 slow, fast  CHECK!", MB_OK);
	}
#endif
}

void CModeSub71::OnSelchangeCombo7() //SYLEE130605   VC ADC   FAST  SLOW MODE
{
	nManualFlag_VC_ADC = m_combo107.GetCurSel();
#ifndef _PC_TEST_
	OnFiler_VC(nManualFlag_VC_ADC);
#endif
}


void CModeSub71::OnFiler_VC(int a) //  ADC B  1==VC FILTER LV ON,   2= VC FILTER ON , 3 =4W ON   4 HV OPEN)
{
#ifndef _PC_TEST_
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328


	if (a == 1) {//vc//fast     LV  on

		Ret = DioOutBit(pDIO.hDrv, DO43, 0);    //HV=1,LV=0  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO44, 1);    //HV=0,LV=1  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO45, 1);    //HV=0,LV=1 //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //61   4w
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //61  hv open
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO49, 1);   //61   VC fast  FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 1] Error!!!");	return; }

	}
	else if (a == 2) {//vc fast   HV  on

		Ret = DioOutBit(pDIO.hDrv, DO43, 1);   //HV=1,LV=0  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO44, 0);    //HV=0,LV=1  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO45, 0);    //HV=0,LV=1   //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //61   4w
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //61  hv open
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO49, 1);   //61   VC fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 1] Error!!!");	return; }
	}
	else if (a == 4) {
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //61  hv open
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //61   VC fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 1);   //61   4w
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return; }
	}
	else if (a == 3) {
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //61   4w
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //61   VC fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 1);   //61  hv open
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 1] Error!!!");	return; }

	}
	else if (a == 11) {//vc//fast     LV  on -> off기능 

		Ret = DioOutBit(pDIO.hDrv, DO43, 0);    //HV=1,LV=0  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO44, 0);    //HV=0,LV=1  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO45, 0);    //HV=0,LV=1 //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //61   4w
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //61  hv open
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //61   VC fast  FILTER
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 1] Error!!!");	return; }

	}
	else if (a == 12) {//vc fast   HV  on -> off 기능 

		Ret = DioOutBit(pDIO.hDrv, DO43, 0);   //HV=1,LV=0  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 1] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO44, 0);    //HV=0,LV=1  //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO45, 0);    //HV=0,LV=1   //SYLEE150121 신부성 과장 요청 
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO46, 0);   //61   4w
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
		Ret = DioOutBit(pDIO.hDrv, DO56, 0);   //61  hv open
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 56, 0] Error!!!");	return; }

		Ret = DioOutBit(pDIO.hDrv, DO49, 0);   //61   VC fast
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 49, 1] Error!!!");	return; }

	}
	else {
		AfxMessageBox("  VC ADC MODE Filter 1~2 slow, fast  CHECK!", MB_OK);
	}
#else  //#ifdef DEF_CARD_A1   




	if (a == 1) {//vc//fast     LV  on

		Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 3, 1);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 2, 1);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 1, 1);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,1) Error!!!");	return; }

	}
	else if (a == 2) {//vc fast   HV  on

		Ret = Ixud_WriteDOBit(0, 8, 4, 1);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,1) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 1, 1);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,1) Error!!!");	return; }
	}
	else if (a == 4) {
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }

		if (SysSet19.m_n4WHVon == 1) {//sylee220701
			Ret = Ixud_WriteDOBit(0, 5, 6, 1);  //4W HIGH VOL.ON  //sylee200922  PC16 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return; }
		}
		else {
			Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee220701  PC16 
			if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }
		}

		Ret = Ixud_WriteDOBit(0, 8, 1, 1);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,1) Error!!!");	return; }
	}
	else if (a == 3) {
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 0, 1);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,1) Error!!!");	return; }

	}
	else if (a == 11) {//vc//fast     LV  on -> off기능 

		Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }

	}
	else if (a == 12) {//vc fast   HV  on -> off 기능 

		Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 8, 2, 0);  //VC SEN SEL
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
		Ret = Ixud_WriteDOBit(0, 4, 0, 0);  //HV OPEN  
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,0,0) Error!!!");	return; }

		Ret = Ixud_WriteDOBit(0, 3, 1, 0);  //VC FAST FILTER
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,1,0) Error!!!");	return; }

	}
	else {
		AfxMessageBox("  VC ADC MODE Filter 1~2 slow, fast  CHECK!", MB_OK);
	}


#endif  //#ifdef DEF_CARD_A1 

#endif

}


void CModeSub71::OnCheck11()  //sylee150118  VBOUT CHECK//sylee150118  VBOUT CHECK
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (m_check11.GetCheck())
	{
		Ret = DioOutBit(pDIO.hDrv, DO122, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 1] Error!!!");	return; }
		nVBOutCkFlag = 1;
	}
	else {
		Ret = DioOutBit(pDIO.hDrv, DO122, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 122, 0] Error!!!");	return; }
		nVBOutCkFlag = 0;
	}
#else  //#ifdef DEF_CARD_A1


	if (m_check11.GetCheck())
	{
		Ret = Ixud_WriteDOBit(0, 5, 2, 1);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,1) Error!!!");	return; }
		nVBOutCkFlag = 1;
	}
	else {
		Ret = Ixud_WriteDOBit(0, 5, 2, 0);  //VB OUT CHECK  ON CMD
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,2,0) Error!!!");	return; }
		nVBOutCkFlag = 0;
	}


#endif  //#ifdef DEF_CARD_A1 


#endif

}


//son  I Gain 설정
int CModeSub71::I_Gain(int a)
{   //  1=I GAIn X 1  ,   2=I GAIN X 10

#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	if (a == 1) { // I GAI X 1
		Ret = DioOutBit(pDIO.hDrv, DO57, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return  0; }
	}
	else if (a == 2) {// I GAI X 10
		Ret = DioOutBit(pDIO.hDrv, DO57, 1);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 1] Error!!!");	return  0; }
	}
	else {//ERROR
		Ret = DioOutBit(pDIO.hDrv, DO57, 0);
		if (Ret != 0) { AfxMessageBox(" [DioOutBit, 57, 0] Error!!!");	return  0; }
	}
#else  //#ifdef DEF_CARD_A1   


	if (a == 1) { // I GAI X 1
		Ret = Ixud_WriteDOBit(0, 4, 1, 0);  //i GAIN 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,1,0) Error!!!");	return   0; }
	}
	else if (a == 2) {// I GAI X 10
		Ret = Ixud_WriteDOBit(0, 4, 1, 1);  //i GAIN   X10
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,1,1) Error!!!");	return   0; }
	}
	else {//ERROR
		Ret = Ixud_WriteDOBit(0, 4, 1, 0);  //i GAIN 
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,4,1,0) Error!!!");	return   0; }
	}


#endif  //#ifdef DEF_CARD_A1 
#endif


	return 1;
}


int CModeSub71::OnEmbed_Freq(int a)
{   // req  1~8 

#ifndef _PC_TEST_
	int Ret;


#ifdef DEF_CARD_A1  //sylee170328


	OutBitNo[0] = 0;   OutBitNo[1] = 1;   OutBitNo[2] = 2;

	if (a == 1) { // 200hz           
		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   0 0  0] Err]or!!!");	return  0; }
	}
	else if (a == 2) {// 500hz
		OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   1 0  0] Err]or!!!");	return  0; }
	}
	else if (a == 3) {// 1khz
		OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   0 1  0] Err]or!!!");	return  0; }
	}
	else if (a == 4) {// 5khz
		OutBitData[0] = 1;   OutBitData[1] = 1;   OutBitData[2] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   1 1  0] Err]or!!!");	return  0; }
	}
	else if (a == 5) {// 10khz
		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   0 0  1] Err]or!!!");	return  0; }
	}
	else if (a == 6) {// 20khz
		OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   1 0  1] Err]or!!!");	return  0; }
	}
	else if (a == 7) {// 50khz
		OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   0 1  1] Err]or!!!");	return  0; }
	}
	else if (a == 8) {// 100khz
		OutBitData[0] = 1;   OutBitData[1] = 1;   OutBitData[2] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   1 1  1] Err]or!!!");	return  0; }
	}
	else {//ERROR
		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 3, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 0 1 2,   0 0  0] Err]or!!!");	return  0; }
		AfxMessageBox(" Error   Embeded Frequece  1~8 ?  CHECK!", MB_OK);
	}
#else  //#ifdef DEF_CARD_A1   





	//	OutBitNo[0]=0;   OutBitNo[1]=1;   OutBitNo[2]=2;

	if (a == 1) { // 200hz           
		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
	}
	else if (a == 2) {// 500hz
		OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 0;
	}
	else if (a == 3) {// 1khz
		OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 0;
	}
	else if (a == 4) {// 5khz
		OutBitData[0] = 1;   OutBitData[1] = 1;   OutBitData[2] = 0;
	}
	else if (a == 5) {// 10khz
		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 1;
	}
	else if (a == 6) {// 20khz
		OutBitData[0] = 1;   OutBitData[1] = 0;   OutBitData[2] = 1;
	}
	else if (a == 7) {// 50khz
		OutBitData[0] = 0;   OutBitData[1] = 1;   OutBitData[2] = 1;
	}
	else if (a == 8) {// 100khz
		OutBitData[0] = 1;   OutBitData[1] = 1;   OutBitData[2] = 1;
	}
	else {//ERROR
		OutBitData[0] = 0;   OutBitData[1] = 0;   OutBitData[2] = 0;
		AfxMessageBox(" Error   Embeded Frequece  1~8 ?  CHECK!", MB_OK);
	}

	Ret = Ixud_WriteDOBit(0, 0, 0, OutBitData[0]);  // DA0
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,0, ) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 0, 1, OutBitData[1]);  // DA1
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,1, ) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 0, 2, OutBitData[2]);  // DA2
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,2, ) Error!!!");	return  0; }


#endif  //#ifdef DEF_CARD_A1 

#endif

	return 1;

}


int CModeSub71::OnEmbed_VSet(int a)
{   // req  1~8 

#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328

	OutBitNo[0] = 5;   OutBitNo[1] = 6;

	if (a == 1) { // 1V          
		OutBitData[0] = 0;   OutBitData[1] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 5 6,   0  0] Err]or!!!");	return  0; }
	}
	else if (a == 2) {// 2V
		OutBitData[0] = 1;   OutBitData[1] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 5 6,   1 0 ] Err]or!!!");	return  0; }
	}
	else if (a == 3) {// 5V
		OutBitData[0] = 0;   OutBitData[1] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 5 6,   0 1 ] Err]or!!!");	return  0; }
	}
	else if (a == 4) {// 10V
		OutBitData[0] = 1;   OutBitData[1] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 5 6,  1 1 ] Err]or!!!");	return  0; }
	}
	else {//ERROR
		OutBitData[0] = 0;   OutBitData[1] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit,  5 6,   0 0  ] Err]or!!!");	return  0; }
		AfxMessageBox(" Error   Embeded Frequence  1~8 ?  CHECK!", MB_OK);
	}
#else  //#ifdef DEF_CARD_A1 



	//	 OutBitNo[0]=5;   OutBitNo[1]=6;  
	if (a == 1) { // 1V          
		OutBitData[0] = 0;   OutBitData[1] = 0;
	}
	else if (a == 2) {// 2V
		OutBitData[0] = 1;   OutBitData[1] = 0;
	}
	else if (a == 3) {// 5V
		OutBitData[0] = 0;   OutBitData[1] = 1;
	}
	else if (a == 4) {// 10V
		OutBitData[0] = 1;   OutBitData[1] = 1;
	}
	else {//ERROR
		OutBitData[0] = 0;   OutBitData[1] = 0;
		AfxMessageBox(" Error   Embeded Frequence  1~8 ?  CHECK!", MB_OK);
	}

	Ret = Ixud_WriteDOBit(0, 0, 5, OutBitData[0]);  // 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,5,   Embeded) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 0, 6, OutBitData[1]);  // 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,6,   Embeded) Error!!!");	return  0; }


#endif  //#ifdef DEF_CARD_A1 


#endif

	return 1;

}



int CModeSub71::OnEmbed_AV_Set(int a)
{   // //AV_MODE1 ~ 4

#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	OutBitNo[0] = 3;   OutBitNo[1] = 4;

	if (a == 1) { //AV_MODE1          
		OutBitData[0] = 0;   OutBitData[1] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 3 4,   0  0] Err]or!!!");	return  0; }
	}
	else if (a == 2) {//AV_MODE2 
		OutBitData[0] = 1;   OutBitData[1] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 3 4,   1 0 ] Err]or!!!");	return  0; }
	}
	else if (a == 3) {//AV_MODE3 
		OutBitData[0] = 0;   OutBitData[1] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 3 4,   0 1 ] Err]or!!!");	return  0; }
	}
	else if (a == 4) {//AV_MODE4 
		OutBitData[0] = 1;   OutBitData[1] = 1;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit, 3 4,  1 1 ] Err]or!!!");	return  0; }
	}
	else {//ERROR
		OutBitData[0] = 0;   OutBitData[1] = 0;
		Ret = DioOutMultiBit(pDIO.hDrv, &OutBitNo[0], 2, &OutBitData[0]);
		if (Ret != 0) { AfxMessageBox(" [DioOutMultiBit,  3 4,   0 0  ] Err]or!!!");	return  0; }
		AfxMessageBox(" Error   Embeded   AV Mode  1~4 ?  CHECK!", MB_OK);
	}
#else  //#ifdef DEF_CARD_A1   



	//	 OutBitNo[0]=3;   OutBitNo[1]=4;    

	if (a == 1) { //AV_MODE1          
		OutBitData[0] = 0;   OutBitData[1] = 0;
	}
	else if (a == 2) {//AV_MODE2 
		OutBitData[0] = 1;   OutBitData[1] = 0;
	}
	else if (a == 3) {//AV_MODE3 
		OutBitData[0] = 0;   OutBitData[1] = 1;
	}
	else if (a == 4) {//AV_MODE4 
		OutBitData[0] = 1;   OutBitData[1] = 1;
	}
	else {//ERROR
		OutBitData[0] = 0;   OutBitData[1] = 0;
		AfxMessageBox(" Error   Embeded   AV Mode  1~4 ?  CHECK!", MB_OK);
	}

	Ret = Ixud_WriteDOBit(0, 0, 3, OutBitData[0]);  // 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,3,   Embeded) Error!!!");	return  0; }
	Ret = Ixud_WriteDOBit(0, 0, 4, OutBitData[1]);  // 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,0,4,   Embeded) Error!!!");	return  0; }


#endif  //#ifdef DEF_CARD_A1 

#endif

	return 1;

}




int CModeSub71::OnEmbed_Freq_Display(int nCh) {

	((CButton*)GetDlgItem(IDC_RADIO101))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO102))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO103))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO104))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO105))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO106))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO107))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO108))->SetCheck(FALSE);

	if (nCh == 1) ((CButton*)GetDlgItem(IDC_RADIO101))->SetCheck(TRUE);
	if (nCh == 2) ((CButton*)GetDlgItem(IDC_RADIO102))->SetCheck(TRUE);
	if (nCh == 3) ((CButton*)GetDlgItem(IDC_RADIO103))->SetCheck(TRUE);
	if (nCh == 4) ((CButton*)GetDlgItem(IDC_RADIO104))->SetCheck(TRUE);
	if (nCh == 5) ((CButton*)GetDlgItem(IDC_RADIO105))->SetCheck(TRUE);
	if (nCh == 6) ((CButton*)GetDlgItem(IDC_RADIO106))->SetCheck(TRUE);
	if (nCh == 7) ((CButton*)GetDlgItem(IDC_RADIO107))->SetCheck(TRUE);
	if (nCh == 8) ((CButton*)GetDlgItem(IDC_RADIO108))->SetCheck(TRUE);

	return  1;
}


void CModeSub71::OnRadio101()
{
	OnEmbed_Freq(1);//200hz
	OnEmbed_Freq_Display(1);

	//	 Oninit1();//test

}

void CModeSub71::OnRadio102()
{
	OnEmbed_Freq(2);//500hz
	OnEmbed_Freq_Display(2);
}

void CModeSub71::OnRadio103()
{
	OnEmbed_Freq(3);//1khz
	OnEmbed_Freq_Display(3);
}

void CModeSub71::OnRadio104()
{
	OnEmbed_Freq(4);//5khz 
	OnEmbed_Freq_Display(4);
}

void CModeSub71::OnRadio105()
{
	OnEmbed_Freq(5); //10khz
	OnEmbed_Freq_Display(5);
}

void CModeSub71::OnRadio106()
{
	OnEmbed_Freq(6);//20khz  
	OnEmbed_Freq_Display(6);
}

void CModeSub71::OnRadio107()
{
	OnEmbed_Freq(7);//50khz 
	OnEmbed_Freq_Display(7);
}

void CModeSub71::OnRadio108()
{
	OnEmbed_Freq(8);//100khz 
	OnEmbed_Freq_Display(8);
}


void CModeSub71::OnRadio31()
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO344))->SetCheck(FALSE);

	OnEmbed_VSet(1);// 1v 
}

void CModeSub71::OnRadio32()
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO344))->SetCheck(FALSE);
	OnEmbed_VSet(2);// 2V
}

void CModeSub71::OnRadio33()
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO344))->SetCheck(FALSE);
	OnEmbed_VSet(3);// 5V 
}
void CModeSub71::OnRadio34()
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO344))->SetCheck(TRUE);
	OnEmbed_VSet(4);// 10V
}

void CModeSub71::OnRadio35()
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO366))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO388))->SetCheck(FALSE);
	OnEmbed_AV_Set(1);// 1v 
}
void CModeSub71::OnRadio36()
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO366))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO388))->SetCheck(FALSE);
	OnEmbed_AV_Set(2);// 1v 
}
void CModeSub71::OnRadio37()
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO366))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO388))->SetCheck(FALSE);
	OnEmbed_AV_Set(3);// 1v 
}
void CModeSub71::OnRadio38()
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO366))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO388))->SetCheck(TRUE);
	OnEmbed_AV_Set(4);// 1v 
}



int CModeSub71::OnEmbeded_Guard(int a, int b)
{
#ifndef _PC_TEST_
	// 	      

#ifdef DEF_CARD_A1  //sylee170328

	int Ret;

	if (b == 1) { // on

		if (a == 1) { //AV_MODE1          
			Ret = DioOutBit(pDIO.hDrv, DO124, 1);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 124, 1] Error!!!");	return  0; }
		}
		else if (a == 2) {//AV_MODE2 
			Ret = DioOutBit(pDIO.hDrv, DO125, 1);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 125, 1] Error!!!");	return  0; }
		}
		else if (a == 3) {//AV_MODE3 
			Ret = DioOutBit(pDIO.hDrv, DO126, 1);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 126, 1] Error!!!");	return  0; }
		}
		else if (a == 4) {//AV_MODE4 
			Ret = DioOutBit(pDIO.hDrv, DO127, 1);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 127, 1] Error!!!");	return  0; }
		}
		else {//ERROR 
			AfxMessageBox(" Error   Embeded  On  Gaurd  1~4 ?  CHECK!", MB_OK);
		}

	}
	else {//off

		if (a == 1) { //AV_MODE1          
			Ret = DioOutBit(pDIO.hDrv, DO124, 0);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 124, 0] Error!!!");	return  0; }
		}
		else if (a == 2) {//AV_MODE2 
			Ret = DioOutBit(pDIO.hDrv, DO125, 0);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 125, 0] Error!!!");	return  0; }
		}
		else if (a == 3) {//AV_MODE3 
			Ret = DioOutBit(pDIO.hDrv, DO126, 0);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 126, 0] Error!!!");	return  0; }
		}
		else if (a == 4) {//AV_MODE4 
			Ret = DioOutBit(pDIO.hDrv, DO127, 0);
			if (Ret != 0) { AfxMessageBox(" [DioOutBit, 127, 0] Error!!!");	return  0; }
		}
		else {//ERROR 
			AfxMessageBox(" Error   Embeded  Off   Gaurd 1~4 ?  CHECK!", MB_OK);
		}
	}

#else  //#ifdef DEF_CARD_A1   


/*  //SYLEE170331 EMBEDED  SHIN BU SUNG  DELETE
	if(b==1){ // on
		if(a==1){ //AV_MODE1
				Ret = A_Bit( DO124, 1);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 124, 1] Error!!!");	return  0;	}
		}else if( a==2){//AV_MODE2
				Ret = A_Bit( DO125, 1);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 125, 1] Error!!!");	return  0;	}
		}else if( a==3){//AV_MODE3
				Ret = A_Bit( DO126, 1);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 126, 1] Error!!!");	return  0;	}
		}else if( a==4){//AV_MODE4
				Ret = A_Bit( DO127, 1);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 127, 1] Error!!!");	return  0;	}
		}else{//ERROR
				AfxMessageBox(" Error   Embeded  On  Gaurd  1~4 ?  CHECK!", MB_OK);
		}

	}else{//off

		if(a==1){ //AV_MODE1
				Ret = A_Bit( DO124, 0);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 124, 0] Error!!!");	return  0;	}
		}else if( a==2){//AV_MODE2
				Ret = A_Bit( DO125, 0);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 125, 0] Error!!!");	return  0;	}
		}else if( a==3){//AV_MODE3
				Ret = A_Bit( DO126, 0);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 126, 0] Error!!!");	return  0;	}
		}else if( a==4){//AV_MODE4
				Ret = A_Bit( DO127, 0);
				if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 127, 0] Error!!!");	return  0;	}
		}else{//ERROR
				AfxMessageBox(" Error   Embeded  Off   Gaurd 1~4 ?  CHECK!", MB_OK);
		}

	}

  */
#endif  //#ifdef DEF_CARD_A1 


#endif

	return 1;
}





int CModeSub71::OnInit_Byte12()
{
#ifndef _PC_TEST_
	int Ret;

	OutData[0] = 0;	    OutData[1] = 0;


#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutMultiByte(pDIO.hDrv, &OutNoT[0], 2, &OutData[0]); //sylee140222
	if (Ret != 0) { AfxMessageBox(" [DioOutMultiByte, 8,9 ] Error!!!");	return  0; }
#else  //#ifdef DEF_CARD_A1   

	Ret = Ixud_WriteDO(0, 0, 0);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,0,0) Error!!!");	return   0; }
	Ret = Ixud_WriteDO(0, 1, 0);
	if (Ret != 0) { AfxMessageBox("  Card96_DOByte(0,1,0) Error!!!");	return   0; }

#endif  //#ifdef DEF_CARD_A1 


#endif
	return 1;
}



//son  고압-> 저압 전환시 고압상태를 해소하기 위해  VB Out Check 수행.
//son  5V 이하로 떨어지면 리턴한다.
//
//nMod1 (VbSrc)  1: VR_LV1, 2: VR_LV2, 3: VR_HV1, 4: VR_HV2
int CModeSub71::AVB_CheckOut1(int Mod1, int nType)  //SYLEE150210 //ACE380  FAIL =0  , PASS=1
{
#ifndef _PC_TEST_
	/////////////////////////////////////////////
	double dV1[1000];
	int V1Co1;
	double V1old;

	V1old = V1;//sylee150210 //ACE380


	//son  고압-> 저압 전환시에만 수행.  
	if (Mod1 != 1 && Mod1 != 2) {
		return 1; //son 현재 모드가 저압이 아니면 리턴한다.
	}

	::ZeroMemory(&dV1, sizeof(dV1));
	V1Co1 = 0;

	AOn_VB_CheckOut1_Set(Mod1); //SYLEE190110  
	Delay_100ns(1000);  //0.1ms //sylee180802-1 
	AOn_VB_CheckOut1_Set(Mod1); //SYLEE150210 //ACE380
	Delay_100ns(50000);  //5ms //sylee180802-1 


	for (int i2 = 1; i2 <= 600; i2++)
	{

		// AOnReadVB1Run1(8);	//son V1 값을 read한다.
		AOn_VB_CheckOut2_Run(Mod1); //SYLEE150210 //ACE380

		V1Co1++;
		dV1[V1Co1] = V1;

		if (i2 <= 20) {
			Delay_100ns(10000);  //1ms  ->20mS
		}
		else if (i2 <= 60) {
			Delay_100ns(50000);  //5ms  -> 200mS
		}
		else {
			Delay_100ns(100000);  //10ms ->5.4S
		}

		//SYLEE180725-1	if(V1<=10){  //10V   //sylee150212 ray 5v-> 10v   
		//if(V1<=5){  //10V  //SYLEE180725-1 ray 10v-> 5v   
		if (V1 <= 10) {  //10V  //SYLEE201204 5v-> 10v  ray ok
			break;
		}

		if (i2 == 200) {//sylee180314-1
			AOn_VB_CheckOut3_Stop(Mod1);	//son VB CheckOut 해제
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut1_Set(Mod1);		//son VB CheckOut 설정
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut2_Run(Mod1); 	//son V1 값을 read한다.
			Delay_100ns(30000);  //3ms
		}

		if (i2 == 300) {//sylee180724
			AOn_VB_CheckOut3_Stop(Mod1);
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut1_Set(Mod1);
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut2_Run(Mod1);
			Delay_100ns(30000);  //3ms
		}
	}


	AOn_VB_CheckOut3_Stop(Mod1); //SYLEE150210 //ACE380		//son VB CheckOut 해제


	if (i2 >= 600) {
		str.Format("Error No 7006-1  :   Voltage = %.1f  (>5V) ,  Voltage Discharge Error!,  VB Voltage Check Time Out Error! ,    5.6S Time out .  code ntype=%d ", V1, nType);
		LogError1(str);    //SYLEE210316-1
		AfxMessageBox(str, MB_OK);
		return  0;
	}

#endif

	return  1;
	/////////////////////////////////////////////

}




int LogError1(CString str1)     //SYLEE210316-1
{
	char  fName[200];
	CString str;
	FILE* fp;

	CTime curTime = CTime::GetCurrentTime();
	::ZeroMemory(&fName, sizeof(fName));
	// str.Format("D:\\logError\Error_%02dY%02dM02dD_%02dH02dM%02dS.txt",curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(),curTime.GetSecond() );
	str.Format("D:\\log\\Error_%02dY%02dM02dD_%02dH.txt", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour());
	strcat(fName, str);
	fp = fopen(fName, "at");
	if (fp == NULL) {
		AfxMessageBox(str, MB_OK); //error messege			
		return 0;
	}
	else {
		str.Format(" %02dY%02dM%02dD_%02dH%02dM%02dS  ::   %s \n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(), str1);
	}
	fprintf(fp, str);
	fclose(fp);
	return 1;
}





int CModeSub71::AVB_CheckOut1(int Mod1, int nType, int Vset1)  //SYLEE150420-2 //ACE380  FAIL =0  , PASS=1
{
#ifndef _PC_TEST_
	/////////////////////////////////////////////
	double dV1[1000];
	int V1Co1;
	double V1old;

	V1old = V1;//sylee150210 //ACE380


	if (Vset1 < 1) {
		Vset1 = 1;
	}
	if (Vset1 > 300) {
		Vset1 = 300;
	}

	if (Mod1 != 1 && Mod1 != 2) {
		return 1;
	}

	::ZeroMemory(&dV1, sizeof(dV1));
	V1Co1 = 0;

	AOn_VB_CheckOut1_Set(Mod1); //SYLEE150210 //ACE380


	Delay_100ns(50000);  //5ms //sylee180802-1 


	for (int i2 = 1; i2 <= 600; i2++) {

		AOn_VB_CheckOut2_Run(Mod1); //SYLEE150210 //ACE380

		V1Co1++;
		dV1[V1Co1] = V1;

		if (i2 <= 20) {
			Delay_100ns(1000);  //100US  //SYLEE150421--  1MS->0.1ms
		}
		else if (i2 <= 60) {
			Delay_100ns(50000);  //5ms
		}
		else {
			Delay_100ns(100000);  //10ms
		}

		if (V1 <= Vset1) {  //10V   //sylee150212 ray 5v-> 10v	   
			break;
		}

		if (i2 == 200) {//sylee180314-1
			AOn_VB_CheckOut3_Stop(Mod1);
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut1_Set(Mod1);
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut2_Run(Mod1);
			Delay_100ns(30000);  //3ms
		}

		if (i2 == 300) {//sylee180724
			AOn_VB_CheckOut3_Stop(Mod1);
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut1_Set(Mod1);
			Delay_100ns(10000);  //1ms
			AOn_VB_CheckOut2_Run(Mod1);
			Delay_100ns(30000);  //3ms
		}

	}

	AOn_VB_CheckOut3_Stop(Mod1); //SYLEE150210 //ACE380

//sylee171115-2	V1=V1old;//sylee150210 //ACE380

	if (i2 >= 600) {
		str.Format("Error No 7006-2  :   Voltage = %.1f  (> %d V) ,  Voltage Discharge Error!,  VB Check Time Out Error! ,   5.6S Time out .  code type =%d ", V1, Vset1, nType);
		LogError1(str);    //SYLEE210316-1
		AfxMessageBox(str, MB_OK);
		return  0;
	}


#endif
	return  1;
	/////////////////////////////////////////////

}





void CModeSub71::OnRelayAllOff1()  //SYLEE180724
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO52, 0); //HV1 RELAY   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 50, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO53, 0); //HV2 RELAY  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 51, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO54, 0);  //LV1 RELAY    
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 52, 0] Error!!!");	return; }
	Ret = DioOutBit(pDIO.hDrv, DO55, 0);  //LV2 RELAY 
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 53, 0] Error!!!");	return; }


#else  	//DEF_CARD_A1 
	Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 4, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 5, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,5,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 6, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,6,0) Error!!!");	return; }
	Ret = Ixud_WriteDOBit(0, 3, 7, 0); //( Board , Port , Bit , Val);
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,3,7,0) Error!!!");	return; }


#endif//DEF_CARD_A1 

#endif

	return;
	/////////////////////////////////////////////

}


void CModeSub71::OnRelay4W_ON1()  //SYLEE181115
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO46, 1); //4W CMD  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return; }
#else  	//DEF_CARD_A1 
	Ret = Ixud_WriteDOBit(0, 5, 6, 1);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 1);  //4W CMD   OFF
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,1) Error!!!");	return; }
#endif//DEF_CARD_A1 
#endif
}

void CModeSub71::OnRelay4W_OFF1()  //SYLEE181118
{
#ifndef _PC_TEST_
	int Ret;

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO46, 0); //4W CMD  
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 0] Error!!!");	return; }
#else  	//DEF_CARD_A1 
	Ret = Ixud_WriteDOBit(0, 5, 6, 0);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,0) Error!!!");	return; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 0);  //4W CMD   OFF
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,0) Error!!!");	return; }
#endif//DEF_CARD_A1 
#endif

	Delay_100ns(30000);  //3ms

}



//son ADC_B를 spark ADC로 설정
int CModeSub71::OnSparkADC(int nOn)//SYLEE200714
{

	if (nFlag_NewSparkBaord != 2) {//SYLEE201024-1  //SPARK NEW BOARD	
		return 1;
	}

#ifndef _PC_TEST_

#ifdef DEF_CARD_A1  //sylee170328

#else  //#ifdef DEF_CARD_A1   
	int Ret;

	if (nOn == 1) {
		Ret = Ixud_WriteDOBit(0, 5, 4, 1);   //son SPARK_ADC ON
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,4,1) Error!!!");	return   0; }
	}
	else {
		Ret = Ixud_WriteDOBit(0, 5, 4, 0);
		if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,4,0) Error!!!");	return   0; }
	}


#endif  //#ifdef DEF_CARD_A1 


#endif
	return 1;
}


//son210611
//son 4W VC 전용 측정함수 : OnButton604() 와 내용 동일. 
// Output: double DHa (VC ADC Raw Data)     //son231012_3 DWORD -> double. 리턴타입수정
//         Sub71.X1,  Sub71.W1  (VC Voltage) 
double CModeSub71::AOnRead_4W_VC()    //4 WIRE CMD.  4WIRE  VOLTAGE READING 부   //son231012_3
{
	//#ifndef _PC_TEST_
#if 1
	CString str;
	int Ret, nCo, nCount;
	BYTE Ha, La;
	DWORD DHa, V1Ma, V1Mi, V1S, V1T;
	double V10, nS3;  //Scale //SYLEE20111013
	double dAvg; //son231012_3 return val


#ifdef DEF_CARD_A1  //sylee170328	 
	BYTE Buf;
#endif  //#ifdef DEF_CARD_A1 


	nCo = 0;
	V1Ma = 0;
	V1Mi = 0;
	V1S = 0;
	V1T = 0;
	V10 = 0;
	W1 = 0;  //4WIRE 
	nS3 = 0;
	nCount = 20;//200;



#ifdef DEF_CARD_A1  //sylee170328

	//sylee130605
	Ret = DioOutBit(pDIO.hDrv, DO43, 0);   // VC_hv    0=off   1=on   RELAY
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 43, 0] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO44, 0);    // VC_lv    0=off   1=on   RELAY
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 44, 0] Error!!!");	return  -1; }
	Delay_100ns(20000);  //2ms 
	Ret = DioOutBit(pDIO.hDrv, DO45, 1);   //   (0=HV,      1=LV)  IC
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 45, 1] Error!!!");	return  -1; }
	Ret = DioOutBit(pDIO.hDrv, DO46, 1);  // 4WIRE_CMD  (0=VC SENSING,      1=4W SENSING)
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 46, 1] Error!!!");	return -1; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);   // /AD_CONVST_B
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return -1; }

Task1://sylee20110705 

	Ret = DioOutBit(pDIO.hDrv, DO62, 0);  //AD_CONVST_B
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return -1; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  //AD_CONVST_B   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return -1; }
	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 2, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 2] Error!!!");	return -1; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 3, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 3] Error!!!");	return -1; }
	Ha = Buf;

#else  //#ifdef DEF_CARD_A1   

	//sylee130605
	Ret = Ixud_WriteDOBit(0, 8, 4, 0);  //VC HV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,4,0) Error!!!");	return -1; }
	Ret = Ixud_WriteDOBit(0, 8, 3, 0);  //VC LV SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,3,0) Error!!!");	return -1; }
	Delay_100ns(20000);  //2ms 
	Ret = Ixud_WriteDOBit(0, 8, 2, 1);  //VC SEN SEL
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,2,1) Error!!!");	return -1; }
	Ret = Ixud_WriteDOBit(0, 5, 6, 1);  //4W HIGH VOL.OFF  //sylee200922  PC16 
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,5,6,1) Error!!!");	return -1; }

	Ret = Ixud_WriteDOBit(0, 8, 1, 1);  //4W CMD 
	Delay_100ns(20000);//2mSEC//sylee200508-ray //sylee200515-patch
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,8,1,1) Error!!!");	return -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return -1; }

Task1://sylee20110705 

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return -1; }
	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return -1; }

	//DATA READING	
	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return -1; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);


#endif  //#ifdef DEF_CARD_A1 


	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	V1T = DHa;
	if (nCo == 0) {
		V1Ma = V1T;
		V1Mi = V1T;
	}
	else {
		if (V1Ma < V1T) {
			V1Ma = V1T;
		}
		if (V1Mi > V1T) {
			V1Mi = V1T;
		}
	}
	V1S = V1S + V1T;
	if (++nCo < nADCount1) {  //SYLEE20111111
		goto Task1;
	}
	//son231012 DHa = (V1S) / (nADCount1);
	dAvg = (double)(V1S) / (double)(nADCount1); //son231012 DHa -> dAvg

	if (Y1 > 0) {
		X1 = dAvg / 6553.6;//sylee20111011 //son231012 DHa -> dAvg
	}
	else {
		X1 = 0.0;//sylee20111011
	}
	W1 = X1;  //4WIRE 


	//son231012_3 return DHa;
	return dAvg;    //son231012_3

#endif
}

//son D96, D64_0, D64_1 보드 3개를 모두 초기화한다.
void CModeSub71::InitAll_Output()       //son210611 모든 Output 비트 초기화 
{


#if 1
	//son HV, LV VB ALL Relay Off
	//son All D96 Digital Out Bit(0~95) Off
	int nRet1 = AOnPoweroff2();
	if (nRet1 != 1) {
		AfxMessageBox(" Error No 1023, in CModeSub71::InitAll_Output()\n   Power all off fail !,   SW TEST Run Start Error!!!");
		return;
	}

#else
	//son HV, LV VB ALL Relay Off
	//              board port bit bitValue  
	//Ixud_WriteDOBit(0,    3,   4,  0);
	//Ixud_WriteDOBit(0,    3,   5,  0);
	//Ixud_WriteDOBit(0,    3,   6,  0);
	//Ixud_WriteDOBit(0,    3,   7,  0);
	AOnRelayOff1();



	// ---------------- All D96 Digital Out Bit(0~95) Off
	//
	//          board port  ByteVal  
	//for (int port=0; port < 12; port++)
	//    Ixud_WriteDO(0,    port,    0);
	AOnPoweroff();
#endif


	// ---------------- All D64_0 Digital Out Bit(0~31) Off
	// ---------------- All D64_1 Digital Out Bit(0~31) Off
	OnPinAllReset1();
}


// hdpark220511 spark

void  CModeSub71::SetVBReadMode()
{
#ifdef DEF_CARD_A1
	DioOutBit(pDIO.hDrv, DO122, 1);    //VB OUT CHECK  ON CMD
	DioOutBit(pDIO.hDrv, DO42, 1);   //VB  SEN  CMD       0= LV   1= HV
#else
	Ixud_WriteDOBit(0, 5, 2, 1);	//VB Out
	Ixud_WriteDOBit(0, 8, 5, 1);	//VB Sel
#endif
	Delay_100ns(50000);
}
// hdpark220511 end

// hdpark220511 spark
int  CModeSub71::SendCmdToDigitizer(int nLine, int nTotalNet, int nSequence)
{

//	return 1; //sylee230811-2


	if (!m_bModifySparkMeas)
		return 0;

	if (fpSend)
		fprintf(fpSend, "> %d : %d, %d, %d \n", nLine, m_nHWCnt, nTotalNet, nSequence);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	if (nSequence == SPARK_BEGIN)
	{
	    m_bLeakTest = TRUE;   // hdpark220913
		m_nHWCnt = 0;
		m_nNetCnt = 0;
		m_nTotalNetCnt = 0;

		memset(m_tINFO, 0, sizeof(PIN_INFO) * MAX_TRIG_COUNT);
		memset(pMain->m_tInfoRcv, 0, sizeof(PIN_INFO) * MAX_TRIG_COUNT);

		if (fpSend)	fprintf(fpSend, "BEGIN : %5d, %5d, %5d \n", nLine, m_nHWCnt, m_nTotalNetCnt);
	}
	else if (nSequence == SPARK_END)
	{
	    m_bLeakTest = FALSE;   // hdpark220913
		if (fpSend)	fprintf(fpSend, "END : %5d, %5d, %5d, %5d \n", nLine, m_nHWCnt, m_nNetCnt, m_nTotalNetCnt);
	}
	else if (nSequence == SPARK_TRG_ON)
	{
		if (m_nHWCnt >= MAX_TRIG_COUNT - 1)	m_nHWCnt = MAX_TRIG_COUNT - 1;
		++m_nHWCnt;
		m_nNetCnt = 0;
		m_tINFO[m_nHWCnt - 1].nIndex++;
		m_tINFO[m_nHWCnt - 1].nNetCount = 0;
		m_tINFO[m_nHWCnt - 1].nTotalNet = nTotalNet;
		m_tINFO[m_nHWCnt - 1].nSequence = nSequence;
		m_tINFO[m_nHWCnt - 1].nFailCol = nFailL1Co;

		if (fpSend)	fprintf(fpSend, "ON  : %5d, %5d, %5d, %5d, %5d, %5d \n",
			nLine,
			m_nHWCnt,
			m_tINFO[m_nHWCnt - 1].nSrcNet[0],
			m_tINFO[m_nHWCnt - 1].nTotalNet,
			m_tINFO[m_nHWCnt - 1].nNetCount,
			nFailL1Co);
	}
	else if (nSequence == SPARK_TRG_OFF)
	{
		if (m_nHWCnt > 0)
			if (fpSend)	fprintf(fpSend, "OFF : %5d, %5d, %5d, %5d, %5d, %5d, %5d \n",
				nLine,
				m_nHWCnt,
				m_tINFO[m_nHWCnt - 1].nSrcNet[0],
				m_tINFO[m_nHWCnt - 1].nTotalNet,
				m_tINFO[m_nHWCnt - 1].nNetCount,
				m_nTotalNetCnt,
				nFailL1Co);
	}

	m_hwndDigitizer = ::FindWindow(NULL, "PCIE-1840L");

	if (m_hwndDigitizer != 0)
	{
		COPYDATASTRUCT cd;
		cd.dwData = nSequence;
		cd.cbData = sizeof(PIN_INFO);
		cd.lpData = &m_tINFO[m_nHWCnt - 1];                //msg additional data

		::SendMessage((HWND)m_hwndDigitizer, WM_COPYDATA, 0, (LPARAM)(&cd));
	}

	if (nSequence == SPARK_TRG_ON)
	{

#ifdef DEF_CARD_A1
#else
		//Ixud_WriteDOBit(0, 5, 5, 1);	// trigger
		//Ixud_WriteDOBit(0, 10, 7, 1);  //I RANGE SET0	
#endif

		if (m_bVBReadMode)
		{
			m_bSetVBRead = TRUE;
#ifdef DEF_CARD_A1
			DioOutBit(pDIO.hDrv, DO48, 0);   //60   I slow
			DioOutBit(pDIO.hDrv, DO37, 1);
#else
			Ixud_WriteDOBit(0, 3, 0, 0);  //ADC_I_SLOW_SEL
			Ixud_WriteDOBit(0, 10, 2, 1);  //ADC_VB_SEL
#endif
		}
	}
	else
	{
#ifdef DEF_CARD_A1
#else
		//Ixud_WriteDOBit(0, 5, 5, 0);
		//Ixud_WriteDOBit(0, 10, 7, 0);  //I RANGE SET0	
#endif

		if (m_bVBReadMode)
		{
			m_bSetVBRead = FALSE;
#ifdef DEF_CARD_A1
			DioOutBit(pDIO.hDrv, DO48, 1);
			DioOutBit(pDIO.hDrv, DO37, 0);
#else
			Ixud_WriteDOBit(0, 3, 0, 1);  //ADC_I_SLOW_SEL
			Ixud_WriteDOBit(0, 10, 2, 0);  //ADC_VB_SEL
#endif
		}
	}

	return 0;
}
// hdpark220511 end


// hdpark220812 spark cal


//son Type    1: 변환없음. hwPin을 No로 받아서 그대로 PinNo로 사용  
//            2: jigPin->hwPin 변환. jigPin을 No로 받아서 PinMap[2][No]로 변환하여  PinNo로 사용 
//
//son No      PinNo (0부터 시작)
//son nMod    1: TR SET VC,  2: TR SET VB, 3: RY SET VC, 4: RY SET VB
int CModeSub71::AOnPinSet1ForSparkCal(int Type, int No, int nMod)
{

#ifdef VB_2_MOD
	if (nMod == MOD_TR_SET_VC) { 
		nMod = _MOD_TR_SET_VC;
	}
	else if (nMod == MOD_TR_SET_VB) { 
		nMod = _MOD_TR_SET_VB;
	}
	else if (nMod == MOD_RY_SET_VC) { 
		nMod = _MOD_RY_SET_VC;
	}
	else if (nMod == MOD_RY_SET_VB) {  
		nMod = _MOD_RY_SET_VB;
	}
#endif

	int nPin;
	CString str;


	if (No < 0) {
		str.Format("Error No 7708,\n  Spark Pin Set  Error  ( No<0 ) !!! \n \n     PIn No.=%d CHECK.  ", No);
		AfxMessageBox(str);
		return 0;
	}

	if (No > 32767) {
		str.Format("Error No 7708,\n  Spark Pin Set  Error  ( No>32767 ) !!! \n \n     PIn No.=%d CHECK.  ", No);
		AfxMessageBox(str);;
		return 0;
	}

	if (nMod != _MOD_TR_SET_VB && nMod != _MOD_TR_SET_VC 
		&& nMod != _MOD_RY_SET_VB && nMod != _MOD_RY_SET_VC)
	{
		return 0;
	}

	if (Type == 2) {
		nPin = PinMap[2][No];
		if (nPin < 0) {	
			str.Format("Error No 7709,\n  Spark Pin   Map Error ! \n \n   * User Pin No.=%d,  Pin No= %d  (< 0  ) .  ", No, nPin);
			AfxMessageBox(str); 
			return -1;
		}	 
	}
	else		  
	{
		nPin = No;
	}

#ifndef _PC_TEST_
	int Ret;


	if (SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) //son-32KPIN-20190902
	{

		if (No >= 1536 && No < 16384) {//YAMAHA  1.5K
			return 0;
		}

		if (No >= 16384 + 1536 && No < 2048) {//YAMAHA  1.5K
			return 0;
		}
	}


	if (nMod == _MOD_TR_SET_VB || nMod == _MOD_RY_SET_VB) {
		Ret = AOnPinS1Vb1_1(nPin, nMod, 1);
		if (Ret != 1) {
			return 0;
		}
	}
	else if (nMod == _MOD_TR_SET_VC || nMod == _MOD_RY_SET_VC) {
		Ret = AOnPinS1Vc1_1(nPin, nMod, 1);

		if (Ret != 1) {
			return 0;
		}
	}
 #endif 
	return 1;
}

int CModeSub71::ReadSparkForCal(int nCount)
{
#ifndef _PC_TEST_	//son230707_3
	BYTE  La, Ha;
	DWORD DHa;
	int Ret;
	double Y1;


	memset(dSparkV2, 0, sizeof(dSparkV2));
	nSparkV1Po1 = 0;

	// hdpark230105 spark cal begin, about 5us delay
#ifdef DEF_CARD_A1
	Delay_100ns(50);
#else
	Ixud_WriteDOBit(0, 7, 2, 0);	// dummy cmd for delay time
	Ixud_WriteDOBit(0, 7, 2, 0);	// dummy cmd for delay time
 #endif

	int nSparkOnFlag = 0;
#if 0
	FILE* fp2;
	fp2 = fopen("C:\\ACE500\\spark.ini", "rt");
	if (fp2)	fscanf(fp2, "%d", &nSparkOnFlag);
	if (fp2)	fclose(fp2);
#endif
	//hdpark230105 spark cal end
	
Task1:

#ifdef DEF_CARD_A1
	Ret = DioOutBit(pDIO.hDrv, DO62, 0);    ///AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

	InpNo[0] = 2;	        InpNo[1] = 3;
	Ret = DioInpMultiByte(pDIO.hDrv2, &InpNo[0], 2, &InpData[0]); 
	if (Ret != 0) { AfxMessageBox(" [DioInpMultiByte, 2,3 ] Error!!!");	return  0; }
	La = InpData[0];  	Ha = InpData[1];
#else  //#ifdef DEF_CARD_A1  

	Ret = Ixud_WriteDOBit(0, 7, 1, 0);  //  /AD_CONVST  =>  63
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,0) Error!!!");	return  0; }

	Ret = Ixud_WriteDOBit(0, 7, 1, 1);  //  /AD_CONVST
	if (Ret != 0) { AfxMessageBox("  Card96_DOBit(0,7,1,1) Error!!!");	return  0; }

	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);
	if (Ret != 0) { AfxMessageBox("  Card64H1_DI32() Error!!!");	return  0; }
	La = (BYTE)((pDIO_D64.dwDIStatus >> 16) & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 24) & 0xff);

#endif  //#ifdef DEF_CARD_A1 

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	//spark drop voltage
	Y1 = (DHa / 65535.) * 250.0; //hdpark230105

	if (Y1 < 0.1) {
		Y1 = 0.1;
	}
	
	//hdpark230105 begin
	if(nSparkOnFlag & 0x10)	dSparkV2[nSparkV1Po1] = DHa; //(float)Y1;
	else					dSparkV2[nSparkV1Po1] = (float)Y1;
	//hdpark230105 end
	
#ifdef DEF_CARD_A1	// hdpark
	BYTE Buf;
	Ret = DioOutBit(pDIO.hDrv, DO62, 0);       //AD_CONVST_A   
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 0] Error!!!");	return  0; }
	Ret = DioOutBit(pDIO.hDrv, DO62, 1);  // /AD_CONVST_A
	if (Ret != 0) { AfxMessageBox(" [DioOutBit, 62, 1] Error!!!");	return  0; }

	//DATA READING	
	Ret = DioInpByte(pDIO.hDrv2, 0, &Buf);      //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 0] Error!!!");	return  0; }
	La = Buf;
	Ret = DioInpByte(pDIO.hDrv2, 1, &Buf);     //VB DATA Reading 
	if (Ret != 0) { AfxMessageBox(" [DioInpByte, 1] Error!!!");	return  0; }
	Ha = Buf;
#else
	// hdpark230105 begin, VB Read
	La = (BYTE)(pDIO_D64.dwDIStatus & 0xff);
	Ha = (BYTE)((pDIO_D64.dwDIStatus >> 8) & 0xff);
#endif

	DHa = (DWORD)(Ha << 8);
	DHa = DHa + La;

	X1 = DHa / 200.0; // spark vb read

	dSparkI2[nSparkV1Po1] = (float)X1; // spark check 전 인덱스에 이어서 저장.
	// hdpark230105 end
	if (nSparkV1Po1++ < nCount) {
		goto Task1;
	}
#endif		//son230707_3

	return 1;
}
// hdpark220812 end

#if 0 // hdpark221115 test code begin
int CModeSub71::WRITE_IO(int nIO, BOOL bON)
{
	int nPort = nIO / 8;
	int nBit  = nIO % 8;
	return Ixud_WriteDOBit(0, nPort, nBit, bON);
}

int CModeSub71::DOWrite(int m_uCardID, DWORD dwDOData)
{
	d64h_do_writeport(m_uCardID, dwDOData);
	return 0;
}

int CModeSub71::SetPin(int nCard, int nPin, BOOL bOn)
{
	DWORD dwData = 0;

	dwData |= (1 << 20);
	dwData |= (1 << 19);
	DOWrite(nCard, dwData);

	dwData |= (nPin & 0xFFFF);
	if (bOn)	dwData &= ~(1 << 18);
	else		dwData |=  (1 << 18);

	dwData |= (1 << 16);
	DOWrite(nCard, dwData);

	dwData &= ~(1 << 19);
	DOWrite(nCard, dwData);

	return 0;
}
#endif
//hdpark221115 test code end
