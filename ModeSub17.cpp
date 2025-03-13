// ModeSub17.cpp : implementation file
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub71.h"
#include "ModeSub18.h"//sylee160503
#include "ModeSub17.h"
 
#include "FileSysInfo.h"
#include "Contec.h" 
#include "A_Define1.h"  //son190628
#include "PassWord1.h"  //son200724: 4W-SamplingCount 비번입력기능
#include "Globals.h"    //son220419_3

#define NUM_COL1            21      //son240622 NUM_COL1 -> NUM_COL1 
#define MAX_MANUAL_TEST     100     //son210928

extern int nuShortEnableRun; //sylee210317
//son231020 extern int n4WEnableRun; //sylee210317  //son Recipe step 중에 4W 가 있는지 표시
extern int nRecipe_4W_Use; //son231020 n4WEnableRun -> nRecipe_4W_Use로 통일 

//son200724:  4W SamplingCount 비번입력기능 
CPassWord1  g_PasswdDlg;            //son200724 : 신규 전역변수 추가
bool        g_bPassDlgOpen;         //son200724 : 신규 전역변수 추가


//son210623 defRow1Temp -> g_nGridNetCnt
int g_nGridNetCnt;    //son Grid에 추가되는 net 카운트. GridRowCnt -1    


extern int nFlag_4W_OUT50V;//sylee200411
extern int nRelay1OffSkip;//sylee170424-1
extern __int64 GetuSecond();  
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA


enum CALC_ITEM       
{
    CALC_MIN = 0,        //son210623
    CALC_AVG = 1,        //son210623
    CALC_MAX = 2,        //son210623
    CALC_SIGMA = 3,      //son210928
    CALC_REF_LOW = 4,    //son211015     Ref.Low  (LSL: Lower Spec Limit) 저장
    CALC_REF_HIGH = 5,   //son211015     Ref.High (USL: Upper Spec Limit) 저장
    CALC_NUM
};

//son "4W_Setup_A.txt" string data 를 read하여 float로 임시 저장
//    net=0, sample=1 이 시작 지점. data를 채운다.
float fData[MAX_4W_NET +1][MAX_4W_SAMPLE+1]; //sylee170705-3  201->1001    //sylee170729-1  double->float  //son210928
										//son231107 Def_4WMax1 -> MAX_4W_NET +1

//son "4W_Setup_A.txt" Calculate data 저장
//    net=1이 시작 지점.  fData[net-1]이 fCalc[net]에 해당한다. 
//son fData[net][]를 에서 min, max, avg를 구해 fCalc[net][] 에 저장한다. 
double fCalc[MAX_4W_NET +1][CALC_NUM];//sylee150806-2 //son231107 Def_4WMax1 -> MAX_4W_NET +1
								//son210623 fCalc[net][0:CALC_MIN]   
                                //son210623 fCalc[net][1:CALC_AVG]
                                //son210623 fCalc[net][2:CALC_MAX]
                                //son210928 fCalc[net][3:CALC_SIGMA]
                                //son211015 fCalc[net][4:CALC_REF_LOW]
                                //son211015 fCalc[net][5:CALC_REF_HIGH]

//son 원본 fCalc[net][CALC_AVG] 값을 보관
float fCalc_AvgOrg[MAX_4W_NET +1] ;//sylee160530-1  //son210623 nCal_2 -> fCalc_AvgOrg
									//son231107 Def_4WMax1 -> MAX_4W_NET +1
									
//son 원본 fCalc[net][CALC_SIGMA] 값을 보관
float fCalc_SigmaOrg[MAX_4W_NET +1]; //son211202_6  fCalc_SigmaOrg -> fCalc_SigmaOrg
									 //son231107 Def_4WMax1 -> MAX_4W_NET +1

//son Sampling Count 관련 변수
int n4w_Sample_Enable;//sylee150812-1
int n4w_Sample_SetCount;//sylee150812-1

//son Sigma 계수 관련 변수
BOOL   g_bSigmaCoef_Enable;       //son200827-4w_Sigma
double g_dSigmaCoef;             //son200827-4w_Sigma    // Sigma 상한, 하한 계산을 위한 계수

//son200320-4WDUT16:  Multi DUT 구조로 수정
int n4w_Sample_RunCount[MAX_DUT];//sylee150812-1
int n4w_Sample_SheetCount[MAX_DUT]; //son211112 n4w_Sample_RunCount[dutIdx]는 양품인 경우만 증가
                                 //          n4w_Sample_SheetCount[dutIdx]는 불량, 양품 모두 증가. 실제 Sampling sheet 개수

int n4w_Sample_Old;//sylee150812-1
int n4w_Measure1_Enable;//sylee200512 //sylee200515-patch

bool    g_bUseLowerCalJig_4W;     //son220419_7  0: 기존 Pin 사용. 1: 하부핀 R 측정 모드


extern bool g_b4wSampleEnd[MAX_DUT];  //son200320-4WDUT16:  신규 추가 //son211217_3 삭제 //son220203 원복

extern short nPinSetSkipVb;//sylee160803-6 //ACE400 	//son230127
extern short nPinSetSkipVc;//sylee160803-6//ACE400		//son230127
extern CSysInfo05  SysInfo05; 
extern CSysInfoSer13  SysInfoSer13;
extern CSysSet13  SysSet13;  
extern double dCalA1[10][5];//sylee140106 

//son 4W용 전압별 paramter
//extern int dRec4w[6][11];       // 전압,  전압별 paramter
extern int dRec4w[15][11];       // 전압,  전압별 paramter  //son220419_5 dRec4w[6][11]로 잘못된 것 수정.
//extern int dRec4w2[10][5];//sylee140714   //son220419_5 미사용으로 삭제
extern  int nAutoProcess1;//sylee20120202


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int nManualCount100;   //sylee20120131
int nManualCount100Flag;   //sylee20120131

//son 4W 수동 Test 결과 Avg data를 저장. nManualCount100를 index로 사용하고 1 부터 저장함.
//float nManualR1[101][dSet4WTot1];  //sylee20120131   [300]회 ,  [1000]=net   300->101 //sylee160223-6   double -> float
float nManualR1[101][MAX_4W_NET +1];   //son231107 dSet4WTot1 ->  MAX_4W_NET +1
                                    

extern int nCC,  nVRel,    nIR;
extern double nVSet;

extern _int64  Get_uSecond1() ;//sylee150716
extern CSysSet19  SysSet19;  //Pin SETUP    //sylee150716
extern  CChildView7  pView7; //sylee150716

//int n4WFlag[10][dSet4WTot1];//  [10]  =mode total no.,  [ 3000]= 4w net listn total no.
int n4WFlag[10][MAX_4W_NET +1];  //son231107 dSet4WTot1 ->  MAX_4W_NET +1

 
extern int  CalibrationLS1(int No,  double  dRealStart,   double  dRealEnd);//sylee140106
//extern void  CalibrationLS1_Func(); //sylee140708     //son220419_5 미사용으로 삭제

extern CSysInfoView01  SysInfoView01;//sylee150806



extern int dRec4w1[15][11];//sylee160520

extern CSysSet211  SysSet211;  //Pin SETUP   
   
extern void DoEvents(); 
extern void DoEvents();
extern __int64 GetMilSecond2();   
extern CSysInfoView01  SysInfoView01; 
extern CFileSysInfo FileSysInfo01;   
extern CSysInfoSer13  SysInfoSer13;
extern CModeSub71  Sub71;

extern double V1;  //vb
extern double Y1;  //i
extern double X1;  //vc 
extern double W1;  //4WIRE

extern int nTimeDelay1;

extern double nCalC[30][10][100];

int n4WManualCount;

//sylee160223-1 memory delete  double dR14w2[110][MAX_4W_NET +1];
// double dR14w3[110][MAX_4W_NET +1];  //4WSETUP
 

//son Auto Sampling 임시 저장소. Grid 를 이 값으로 그린다. (real ohm R)
// double dR14w[15][MAX_4W_NET +1]
//
//   d4W_Log2[][][]는 DUT별로 나누어진 실제 Auto Sampling 저장소이고 dR14w[][][]는 임시 저장소
//
//   1) Manual Test의 경우  : ModeSub17::OnButtonTest2_3()
//      이중에서 양품인 경우만 Avg를 측정해서 (float)dR14w[2][net]에 저장하고 
//        nManualR1[][]을 거쳐 d4W_Log2[][][]로  저장한다.  
//
//  CModeSub17::OnButtonTest2_3()
//  1567:   nManualR1[nManualCount100][net] =(float)dR14w[2][net]; 
//
//  CModeSub17::OnButtonTestSave()
//  2050:   for( net=1; net<=nEnd; net++){//sylee170125     //son190704-4WDUT16
//               for( j=1; j<nTotal; j++){
//                  d4W_Log2[dutIdx][j][net]= nManualR1[j][net];    
//              }        
//          }
//
//   2) Auto Test의 경우    : CChildView7::ARun201_4w_2_Sample1() 
//      Auto 화면에서 양품인 경우만 dR14w[6][net] 에 저장하고  d4W_Log2[][][]로  저장하고, "4W_Setup_A.txt"에 write하면
//      4W 화면에서  "4W_Setup_A.txt"를 'Load' 버튼으로 4W Grid에 로드할 수 있다.
//   14340:   d4W_Log2[dutIdx][n4w_Sample_RunCount[dutIdx]+1][net]=(float)dR14w[6][net];

//double dR14w[15][dSet4WTot1]; //sylee170803  110->15
double dR14w[15][MAX_4W_NET +1];  //son231107 dSet4WTot1 ->  MAX_4W_NET +1
                            //son net 1 부터 사용 
                            //son   dR14w[0][net]   : sum   for Manual 4W Test 
                            //son   dR14w[1][net]   : min   for Manual 4W Test 
                            //son   dR14w[2][net]   : avg   for Manual 4W Test
                            //son   dR14w[3][net]   : max   for Manual 4W Test 
                            //son   dR14w[4][net]   : diff  (max-min) for Manual 4W Test 
                            //son   dR14w[5][net]   ; diff2 (diff / avg * 100. ) : diff % 비율 
                            
                            //son   dR14w[6~10][net] : R  ???
                            //son   dR14w[6][net]    : real R <= 현재 사용
                            //
                            //son   dR14w[11][net]   : min   for Auto Sampling
                            //son   dR14w[12][net]   : max   for Auto Sampling
                            //
double dVB14w[15][MAX_4W_NET +1];//sylee170803  110->15  //son231107 dSet4WTot1 ->  MAX_4W_NET +1
                            //son net 1 부터 사용 
                            
double dVC14w[15][MAX_4W_NET +1];//sylee170803  110->15  //son231107 dSet4WTot1 ->  MAX_4W_NET +1
                            //son net 1 부터 사용 
                            
double dI14w[15][MAX_4W_NET +1];//sylee170803  110->15   //son231107 dSet4WTot1 ->  MAX_4W_NET +1
                            //son net 1 부터 사용 
                            
double dR14wT1[15][MAX_4W_NET +1];//sylee170803  110->15 //son231107 dSet4WTot1 ->  MAX_4W_NET +1
                            //son net 1 부터 사용 


extern  int nADCx5;//sylee180611-1

 
/////////////////////////////////////////////////////////////////////////////
// CModeSub17 dialog


CModeSub17::CModeSub17(CWnd* pParent /*=NULL*/)
    : CDialog(CModeSub17::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeSub17)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CModeSub17::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeSub17)
	DDX_Control(pDX, IDC_CHECK_LowerCalJig, m_checkLowerCalJig);
#ifdef __GTS_4W_SPCIALNET__	
	DDX_Control(pDX, IDC_CHECK_ALL_SPECIAL, m_check_AllSpecial);     //son221111
#endif
    DDX_Control(pDX, IDC_GRID_1,  m_grid1);
    DDX_Control(pDX, IDC_EDIT1,   m_edit1);
    DDX_Control(pDX, IDC_EDIT2,   m_edit2); //sylee190221-1
    DDX_Control(pDX, IDC_EDIT3,   m_edit3); //sylee181010-1
    DDX_Control(pDX, IDC_EDIT4,   m_edit4); //sylee181010-1
    DDX_Control(pDX, IDC_EDIT6,   m_edit6); //son Average value   
    DDX_Control(pDX, IDC_EDIT7,   m_edit7); 
    DDX_Control(pDX, IDC_EDIT8,   m_edit8); //son Sigma 계수 value
    DDX_Control(pDX, IDC_CHECK1,  m_check1); //son 'Sigma 계수' On
    DDX_Control(pDX, IDC_CHECK2,  m_check2); 
    DDX_Control(pDX, IDC_CHECK5,  m_check5); //son 'Auto Sampling  Enable' ON, n4W_Sample_Enable
    DDX_Control(pDX, IDC_CHECK6,  m_check6); //son 'Average Use' On
    DDX_Control(pDX, IDC_LABEL101,  m_Label101);
    DDX_Control(pDX, IDC_LABEL102,  m_Label102);
    DDX_Control(pDX, IDC_LABEL103,  m_LabelPoint);      
    DDX_Control(pDX, IDC_LABEL1001,  m_Label1001); 
    DDX_Control(pDX, IDC_LABEL1004,  m_Label1004);
    DDX_Control(pDX, IDC_LABEL1006,  m_Label1006);
    DDX_Control(pDX, IDC_LABEL1007,  m_Label1007);
    DDX_Control(pDX, IDC_COMBO_DUT, m_comboDut);    //son200708: 4W Setup 화면에서 DUT 선택기능추가
    DDX_Control(pDX, IDC_BUTTON104,  m_button1001);
    DDX_Control(pDX, IDC_BLOAD,  m_button1004);
    DDX_Control(pDX, IDC_BUTTON101,  m_button1005);
    DDX_Control(pDX, IDC_BUTTON102,  m_button1006);
    DDX_Control(pDX, IDC_BSAVE,  m_button1007);
    DDX_Control(pDX, IDC_BUTTON103,  m_button1008); 
    DDX_Control(pDX, IDC_BUTTON115,  m_button1010);     
    DDX_Control(pDX, IDC_OK,  m_button1011);
    DDX_Control(pDX, IDC_BUTTON113,  m_button1013); 
    DDX_Control(pDX, IDC_BUTTON111,  m_button1014); 
    DDX_Control(pDX, IDC_BUTTON112,  m_button1015);
	//}}AFX_DATA_MAP
}




BEGIN_MESSAGE_MAP(CModeSub17, CDialog)
    //{{AFX_MSG_MAP(CModeSub17)
        ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)
        ON_CBN_SELCHANGE(IDC_COMBO02, OnSelchangeCombo2)
        ON_CBN_SELCHANGE(IDC_COMBO03, OnSelchangeCombo3)
        ON_WM_MOUSEWHEEL()
        ON_WM_VSCROLL()
        ON_CBN_SELCHANGE(IDC_COMBO_DUT, OnSelchangeComboDut)    //son200708: 4W Setup 화면에서 DUT 선택기능추가
        ON_EN_SETFOCUS(IDC_EDIT7, OnSetfocusEdit7)      //son200724: 4W-SamplingCount 비번입력기능
        ON_BN_CLICKED(IDC_CHECK1, OnCheck_SigmaCoef)     //son211015
        ON_BN_CLICKED(IDC_CHECK2, OnCheck_EjectLowHigh)     //son211015
        ON_BN_CLICKED(IDC_CHECK6, OnCheck_AvgPercentUse)     //son211015
        ON_WM_HSCROLL()
    	ON_BN_CLICKED(IDC_CHECK_LowerCalJig, OnCHECKLowerCalJig)    //son220419_7
#ifdef __GTS_4W_SPCIALNET__
    	ON_BN_CLICKED(IDC_CHECK_ALL_SPECIAL, OnCheck_AllSpecial)    //son221111
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub17 message handlers

BEGIN_EVENTSINK_MAP(CModeSub17, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub17)

    ON_EVENT(CModeSub17, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)     
                                                                                    //son 'Exit'
 
//  ON_EVENT(CModeSub17, IDC_LABEL2,    -600 /* Click */, OnClickGrid1, VTS_NONE)
    ON_EVENT(CModeSub17, IDC_GRID_1,    -601 /* DblClick */,OnDblClickMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSub17, IDC_GRID_1,    71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSub17, IDC_GRID_1,    72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSub17, IDC_GRID_1,    -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)

    ON_EVENT(CModeSub17, IDC_BSAVE,     -600 /* Click */, OnButton_Save, VTS_NONE)  
                                                          //son 'Ref.SAVE' button   //현재 Grid를 "4W.ini" 파일에 Save
                                                          
    ON_EVENT(CModeSub17, IDC_BLOAD,     -600 /* Click */, OnButton_Load, VTS_NONE)  
                                                          //son 'Ref.ReLoad' button //"4W.ini" 파일을 Grid에 Load 
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON101, -600 /* Click */, OnButtonTestManual, VTS_NONE) 
                                                          //son 'Manual Test' button
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON102, -600 /* Click */, OnButton_Copy_TestDatato_RefAvg, VTS_NONE)    
                                                          //son 'Test->Ref.Avg' button
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON103, -600 /* Click */, OnButton_Load_Auto4WData, VTS_NONE)   
                                                          //son 'Load 4w Auto Sample' button
                                                          // "4W_Setup_A.txt" 파일 로딩. 4W Auto Test결과를 Grid에 반영한다.
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON115, -600 /* Click */, OnButton_ViewFile_Auto4wData, VTS_NONE)   
                                                          //son 'ViewFile 4W Auto Sample' button 
                                                          //"4W_Setup_A.txt" 파일 보기.   4W Auto Test결과 파일을 Open한다. 
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON104, -600 /* Click */, OnButton_ViewFile_4wRef, VTS_NONE)    
                                                          //son 'View File 4W Ref.' button //"4W.ini" 파일을 Open
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON110, -600 /* Click */, OnButton_Open4wLimitSubDlg, VTS_NONE)    
                                                          //son 'SET Ref.Limit' button      // 4W Limit Setting Sub Dialog을 띄운다.    
                                                          //son220308 OnButton110 -> OnButton_Open4wLimitSubDlg
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON117, -600 /* Click */, OnButton_SetLimitHigh, VTS_NONE)    
                                                          //son 'Limit High +' button   //son220308
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON118, -600 /* Click */, OnButton_SetLimitLow, VTS_NONE)    
                                                          //son 'Limit Low -' button    //son220308
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON111, -600 /* Click */, OnButtonTestInit, VTS_NONE)   
                                                          //son test 'Manual Init' button
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON112, -600 /* Click */, OnButtonTestCancel, VTS_NONE) 
                                                          //son 'Cancel' button
                                                          
    ON_EVENT(CModeSub17, IDC_BUTTON113, -600 /* Click */, OnButtonTestSave, VTS_NONE)   
                                                          //son test 'Save ManualTest Data' button
                                                          // 수동 테스트 결과를 "4W_Setup.txt"에 write한다. 
	ON_EVENT(CModeSub17, IDC_BUTTON120,	-600 /* Click */, OnButton120, VTS_NONE)   //sylee221017-1
    ON_EVENT(CModeSub17, IDC_BUTTON121, -600 /* Click */, OnButton121, VTS_NONE)     //son 'NG %' button

    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()




 

BOOL CModeSub17::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class
    
 
    if((pMsg->message == WM_VSCROLL) ) { 
        return TRUE;
    }
    if((pMsg->message == WM_KEYDOWN) 
        && (pMsg->wParam == VK_RETURN)) {
        return TRUE;
    }

    if(pMsg->message == VK_SCROLL){ 
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

    //int  i;


BOOL CModeSub17::OnInitDialog() 
{
    CDialog::OnInitDialog();


    CString str; 
    i=9;
    InitControl();  

    //son200724: 4W-SamplingCount 비번입력기능 
    g_bPassDlgOpen = false;
    g_passDlgLogOnUser = 0;


    ::ZeroMemory(&nData, sizeof(nData));
 
    m_nMode1=71; 
    m_nMode2=2; 
    m_nMode3=1;

    m_edit2.SetWindowText("0"); //sylee190221-1

 
    //son200827 n4w_Sample_SetCount 로딩 및 range 체크 기능은  InitControl()로 이동
 
     n4WManualCount=1;

     nManualCount100=1;   //sylee20120131
     nManualCount100Flag=1;   //sylee20120131
     FileSysInfo01.LoadSaveSet12(_LOAD);//SYLEE131112  load:2

     nAutoProcess1=0; //sylee20120202

    //son220426 UPPER_LOWER caldata를 둘다 쓰는 모드가 아니라면 체크박스를 숨긴다.
    if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)
        m_checkLowerCalJig.ShowWindow(SW_HIDE);         //son220426
    else
        m_checkLowerCalJig.ShowWindow(SW_SHOW);         //son220426

     //onload();
    
    return TRUE;   
                  
}




void CModeSub17::OnSelchangeCombo1() 
{
 
}

void CModeSub17::OnSelchangeCombo2() 
{
  //    m_nMode2= m_combo2.GetCurSel();  //m_nMode1   1   R CAL MODE   /   2= R REAL MODE
}


void CModeSub17::OnSelchangeCombo3() 
{
    //   m_nMode3= m_combo3.GetCurSel();  //m_nMode1   1   R CAL MODE   /   2= R REAL MODE
}

//son200708: 4W Setup 화면에서 DUT Id 선택기능추가
void CModeSub17::OnSelchangeComboDut() 
{
    nMulNetFlag = m_comboDut.GetCurSel() +1;

    BeginWaitCursor();          //son200708: 커서를 waiting 모드로 바꾼다.

    //son Init Grid & DUT NetCount
    //son m_grid와 Dut NetCount의 상태를 현재 dutIdx에 맞게 설정한다.
    //    FileSysInfo01.LoadSaveSub17(_LOAD)를  Init_grid(dutIdx)안에서 수행한다.   //son210623
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    Init_grid(dutIdx);    //son210524 DUT 변경시에 "subscript out of range" 발생문제 수정. GTS보고사항.

    OnButton_Load();    //son 4W.ini를 로딩한다.

    EndWaitCursor();            //son200708: 커서를 원상 복구한다.
}

void CModeSub17::OnCheck_SigmaCoef()    //son211015 
{

    if(m_check1.GetCheck()) 
    {
        g_bSigmaCoef_Enable= 1;    //son220308

#ifndef __MST__CUSTOMER //son220308 MST는 Sigma 계수필터와 Average % 필터를 동시 적용 요청

        //son 'Sigma Coef' Enable ON 됐는데,  'Average % use' 도 On이면 Off한다.
        //     상호 배타적이어야 한다.
        if(m_check6.GetCheck()) 
        {
            m_check6.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
#endif
    }
    else
        g_bSigmaCoef_Enable= 0;    //son220308
}

void CModeSub17::OnCheck_EjectLowHigh()     //son211015
{

    //son 'Eject Low/High' Enable ON 됐는데,  'Average % use' 도 On이면 Off한다.
    //     상호 배타적이어야 한다.
    if(m_check2.GetCheck()) 
    {
#ifndef __MST__CUSTOMER //son220308 MST는 Sigma 계수필터와 Average % 필터를 동시 적용 요청
        if(m_check6.GetCheck()) 
        {
            m_check6.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
#endif
    }

}

void CModeSub17::OnCheck_AvgPercentUse()  //son211015
{

    //son 'Average % use' Enable ON 됐는데,  'Sigma Coef' 도 On이면 Off한다.
    //     상호 배타적이어야 한다.
    if(m_check6.GetCheck()) 
    {
        gd4W1_P2High[21][SET4W_AVG_PERCNT_USE] = 1;     //son220308

#ifndef __MST__CUSTOMER //son220308 MST는 Sigma 계수필터와 Average % 필터를 동시 적용 요청

        if(m_check1.GetCheck()) 
        {
            m_check1.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        } 

        // Eject Low/High 도 On 이라면 Off 한다.
        if(m_check2.GetCheck()) 
        {
            m_check2.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
#endif

    }
    else
        gd4W1_P2High[21][SET4W_AVG_PERCNT_USE] = 0;     //son220308

}

//son 'Test' button : 4W 측정
// 이 버튼을 여러번 눌러서 측정을 반복해야 한다.
void CModeSub17::OnButtonTestManual() //test
{  
      OnButtonTest2_3(); //muti range  //ACE380 //SYLEE150127
}
 
extern CSysSet12  SysSet12;  //Pin SETUP  //SYLEE180724-1

extern int n4W_IDropCheck1(int nNet);//sylee180823-1 //sylee180912-1
extern int nFlag_ADICheck1,nFlag_ADIRet1;//sylee180823-1
extern int dAdcISet,dAdcISet2;//sylee200515

//son CChildView7::ARun201_4w_2_Sample1()와 구조가 매우 비슷하다.
void CModeSub17::OnButtonTest2_3() //muti range  //ACE380 //SYLEE150127  I RANGE FIX
{
    CString   str,str1, strfName,strfName1;
    FILE *fp;

    int n4w100, Kcount, j, i, m,  nCount, n4Wire,nRet1, k1, nPassCount1;//sylee141211
    int Lo1,nLoop,NUM_COL1Temp,ka,  nRet,nIFilter,nHVOffSet,nHVGain,nFil,nEnd;//sylee130704  
    double nISet,dVbR1,dVbR2,aa, ab;
    __int64   tStart1, tStart2 ; 
    short dPinVC[200], dPinVB[200]; 
    //son230328 short n4WNet[MAX_4W_NET +1][5]; //sylee160719  2000->5001      //son pinNo 저장
    int n4WNet[MAX_4W_NET +1][5]; //son230328 short -> int //son231107 Def_4WMax1 -> MAX_4W_NET +1

    short nPos1[MAX_4W_NET +1];//sylee140617 //son231107 Def_4WMax1 -> MAX_4W_NET +1
    char fName[FILE_PATH_SIZE], fName1[FILE_PATH_SIZE];        //son210625 200-> FILE_PATH_SIZE
    double nT[10],nData[130][15]; 
    int nRangeStep,nIGain,n4wGain;//SYLEE150123 
    __int64 tStart11,  tStart22 ;//TESTDEBUG
    double bbb[10];//TESTDEBUG
    int nPassCo1, nRunMode1;//sylee170313-1
    int nZeroCo1,nZeroCo2;//sylee180520-1

//  int nHalfPin1;//SYLEE180724-1       //son210215 미사용이므로 코멘트로 막음

    int nPinNo[5]; //sylee200515-patch
    int n4W_Retry1,n4W_RetrySet;
    double d4W_R1, dmin1,dmax1;

    n4W_RetrySet=1;

    if(SysSet19.m_n4WHVon==1){ //sylee200515-patch
        nFlag_4W_OUT50V=1;//SYLEE200411
        SysSet13.m_n4WCVMode=1;//SYLEE201030
    }else{
        nFlag_4W_OUT50V=0;//SYLEE200411
    }

    FileSysInfo01.LoadSaveSet12(_LOAD);//load:2  //SYLEE180724-1

/*
 *   //son210215  nHalfPin1 미사용 관련 코드 삭제
 
    if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //SYLEE180724-1
        nHalfPin1=2048;     
    }
    else if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
        nHalfPin1=4096;
    }
    else if(SysSet12.m_nPinTotMax == PIN_TOT_16K) //16k
    {
        nHalfPin1=4096*2;
    }
    else if(SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS//32k
                || SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR       //son-32KPIN-20200120 //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR       //son-32KPIn-20200120 //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS       //son-32KPIN-20200120 //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS       //son-32KPIN-20200228 //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN   //12k    //son-8K4097-20200605: 8K 구형 하부시작핀 사용시
                || SysSet12.m_nPinTotMax == PIN_TOT_12K)//son210202
    {
        nHalfPin1=4096*4;
    }
    else{//32k, 20K, 24K
        nHalfPin1=4096*4;
    }
*/


    nZeroCo2=0;//sylee180520-1

    //son 4W용 dRec4w[15][11]을 read한다.
    FileSysInfo01.LoadSaveSet21_4W_Range(2);//SYLEE180203

//sylee201028   if(SysSet19.m_nSet12==1){//sylee170313-1 
//sylee201028       nRunMode1=0;//sylee170313-1   /wait=0   //1  fix delay
//sylee201028   }
//sylee201028   else{
    nRunMode1=1;//sylee170313-1   
//sylee201028   }

    nPinSetSkipVb=0;//sylee160803-6	//son230127
    nPinSetSkipVc=0;//sylee160803-6	//son230127


    nRangeStep=4;//sylee200909 5->4
    nPassCount1=0;//sylee141211

    str.Format(" Start ");//sylee141211
    m_Label102.SetCaption(str); 

    m_nMode3=2;//sylee150709


    //------------------------
    //son  VB Check Out 
    
    //son 고압-> 저압 전환시 고압상태를 해소하기 위해  VB Out Check 수행.
    nVRel=(int)dRec[171][2];//SYLEE150210 //ACE380    FAIL =0  , PASS=1
    Sub71.AVB_CheckOut1(nVRel,7);//SYLEE150210 //ACE380    FAIL =0  , PASS=1


    DoEvents();
    DoEvents();
    ///////////////////////////////////////////////////////////////////////////
    //변수 초기화 
    ::ZeroMemory(&nPos1, sizeof(nPos1));//sylee140617 
    ::ZeroMemory (&nData, sizeof(nData));
    ::ZeroMemory (&dR14w, sizeof(dR14w));
    ::ZeroMemory (&dVB14w, sizeof(dVB14w));
    ::ZeroMemory (&dVC14w, sizeof(dVC14w));
    ::ZeroMemory (&dI14w, sizeof(dI14w));
    ::ZeroMemory (&nT, sizeof(nT));
    ::ZeroMemory (&nData, sizeof(nData));
    ::ZeroMemory (&dPinVC, sizeof(dPinVC));
    ::ZeroMemory (&dPinVB, sizeof(dPinVB)); 
    ::ZeroMemory (&fName, sizeof(fName));
    ::ZeroMemory (&fName1, sizeof(fName1));
    ::ZeroMemory (&dR14wT1, sizeof(dR14wT1));
    ::ZeroMemory (&n4WFlag, sizeof(n4WFlag));
    ::ZeroMemory (&n4WNet, sizeof(n4WNet));     //son pinNo  저장

    nStop=0;
    n4Wire=0;


    NUM_COL1Temp=22;

    //son "\SETUP2\Recipe.ini"를 struct CSysInfo05에  Load(type=2)
    FileSysInfo01.LoadSaveSub15(2);//sylee140210

    //son Calibration Reference Data Load
    //  ex) "C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI" 에서 i mode의 이전 4W Calibration Ref 값 로드
    for(i=171;i<=(170+nRangeStep);i++){ //sylee20120202 
        FileSysInfo01.LoadSaveSer13(_LOAD, i, PART_MIXED) ; //4WSETUP   //son220412
    }

    //son????? 4W 정도개선코드에는 없는 부분. 일단 현상 유지함.
    if(SysSet13.m_n4WCVMode==1){     //sylee200217-4WCVMODE  
        for(i=176; i<180; i++){  
            FileSysInfo01.LoadSaveSer13(_LOAD, i, PART_MIXED) ; //4WSETUP //son220412

        }
    }


    //------------------------------------------------
    //son  n4WNet[net][]에 PinNo 할당  & Range 체크 

    int net;        //son190704-4WDUT16
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16

    nEnd=(int)g_s4WNet[dutIdx].wCount;          //son190704-4WDUT16
    for(net=1; net<=nEnd; net++) //sylee20120202    //son190704-4WDUT16
    { 
        for(int pin = 0; pin < _4W_PIN_NUM; pin++)  //son180704-DUT16
        {
            if (g_bUseLowerCalJig_4W)  //son220419_7
                n4WNet[net][_4W_PIN1 + pin]=(int)g_s4WNet[dutIdx].saData[net-1].waPin[pin] +(SysSet12.m_nLowerJigStartPin -1); //son190704-4WDUT16
            else
                n4WNet[net][_4W_PIN1 + pin]=(int)g_s4WNet[dutIdx].saData[net-1].waPin[pin]; //son190704-4WDUT16
        }


        if( n4WNet[net][_4W_PIN1]<0  ||  n4WNet[net][_4W_PIN2]<0  || n4WNet[net][_4W_PIN3]<0  || n4WNet[net][_4W_PIN4]<0 )
        {
            str.Format("Error No 1023,\n   4W READ Error!!! \n  PIN NO < 0 . 4W FILE CHECK.,  file Row  =%d    ",(int)net+5);        
            AfxMessageBox(str); 
            return;//error
        }    
    }


    //------------------------------------
    //son  Clear 4W measured Data
    int row, col;
    for( row=1; row<=g_nGridNetCnt; row++)      //son190704-4WDUT16     //son210623 
    {
        for(col= 1+_4W_TEST_MIN; col<=NUM_COL1Temp ; col++){//sylee20120310      //son190704-4WDUT16
            str.Format("          ");
            m_grid1.SetTextMatrix(row, col, str);
        }  
    }

    DoEvents(); 

    nCount=1; 
    Kcount=1; //sylee150812-1 

    if( Kcount<1){
        Kcount=0;
    }
    else if( nCount>100){
        Kcount=100;
    }    

//son220426_2 begin  fp 두번 open 하는 문제 관련 삭제 
//    strfName.Format("%s\\SETUP\\LOGCAL\\001LOGRCal%d.csv", SysInfoView01.m_pStrFilePath1, m_nMode1);//son201118 //son210514
//    ::ZeroMemory(&fName,sizeof(fName));
//
//    strcat( fName , strfName );  
//    fp = fopen(fName,"wt"); 
//    if(fp == NULL){  
//        str.Format(" Error No   001LOGRCal%d.csv file open   ?  Error, Saving Error !     file close! =>   *.csv file  Close!   ",m_nMode1);
//        AfxMessageBox( str, MB_OK); //error messege
//        return  ;
//    }
//son220426_2 end

    if( nManualCount100==1){//sylee131112
        ::ZeroMemory(&nManualR1, sizeof(nManualR1));
    }

    ::ZeroMemory(&bbb, sizeof(bbb));//TESTDEBUG
    //nRangeStep=4;
    //------------------------------------------------------
    //son  전압 mode(1:171~ 5:175)별로 검사를 진행한다.

    int calPart = PART_UPPER;   //son220419_2
    int mode;
    for( mode=1; mode<=nRangeStep; mode++) //  range auto change
    {   
        if(mode==1){
            nFlag_ADICheck1=1;  //sylee180823-1
            dAdcISet=(int)(dRec[171][5]*0.9);  //sylee181119 0.8->0.9
            dAdcISet2=(int)(dRec[170+mode][5]*1.1); //sylee200215//sylee200515-patch
        }
        else{ 
            nFlag_ADICheck1=0; //sylee180823-1
        }

        if(SysSet13.m_n4WCVMode==1){//sylee200515-patch-4WCVMODE         
            nFlag_ADICheck1=0;
        }

        nFlag_ADIRet1=0; //sylee180823-1

        tStart11=GetuSecond();//TESTDEBUG
        if(mode==1)     //son 171 m_nMode
            {tStart22=tStart11; }//TESTDEBUG
        bbb[mode]=(tStart11-tStart22)/1000000.;//TESTDEBUG
        tStart22=tStart11;//TESTDEBUG

        ::ZeroMemory (&dR14w, sizeof(dR14w));
        ::ZeroMemory (&dVB14w, sizeof(dVB14w));
        ::ZeroMemory (&dVC14w, sizeof(dVC14w));
        ::ZeroMemory (&dI14w, sizeof(dI14w));

        if(mode==1) 
            nRelay1OffSkip=1;//sylee170424-1
        if(nRelay1OffSkip==2) nRelay1OffSkip=3;//sylee170424-1

        m_nMode1=dRec4w[mode][1];

        if(SysSet13.m_n4WCVMode==1){    //sylee191015-4WCVMODE: ray
            if(m_nMode1==171)  m_nMode1=176; 
            if(m_nMode1==172)  m_nMode1=177; 
    //sylee201207 if(m_nMode1==173)  m_nMode1=178; //SYLEE201030
    //sylee201207 if(m_nMode1==174)  m_nMode1=179; //SYLEE201030
    //sylee201207 if(m_nMode1==175)  m_nMode1=179; //SYLEE201030
        }

        if(m_nMode1<170){ 
            m_nMode1=171;
        }
        n4Wire=4;
        Kcount=1;//sylee160529-1

        if(mode==1) //SYLEE180706-1
        {
            if( SysSet19.m_nSet8<1){//SYLEE180706-1  
                Kcount=1; 
            }
            else if( SysSet19.m_nSet8<=20){//SYLEE180706-1
                Kcount=SysSet19.m_nSet8;       
            }
            else{//SYLEE180706-1
                Kcount=1; 
            }
        }
        else{//SYLEE180706-1
            Kcount=1; 
        }

        if(SysSet13.m_n4WCVMode!=1) //sylee200217-4WCVMODE  //sylee200515-patch
        {
            if( m_nMode1<171  ||  m_nMode1>(170+nRangeStep)  ){  
                AfxMessageBox(" Error No 1023, \n    Mode Select Please !,    ( 171~175 ) ,  Testing Start Error!!!");    
                return ;
            }  
        }

 
        //son Calibration 결과 저장 buffer Load
        //     "C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI" 에서 SysInfoSer13.m_nData[part][m_nMode1][][RList]를 Load
        //     'Use=1'인 경우만 SysInfoSer13.m_nData[part][m_nMode1][1~3][RList]를  gdCal1[part][m_nMode1][1~3][RList]에 저장
        nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412
        if( nRet!=1){
            str.Format("ERRor NO 7002,   \n  4w  Calibration file reading Error , \n   Mode no= %d  ", m_nMode1 );
            AfxMessageBox( str);
            return ; 
        }

        nCC= (int)dRec[m_nMode1][1];
        nVRel= (int)dRec[m_nMode1][2];   
        nVSet= (int)dRec[m_nMode1][3];
        nIR= (int)dRec[m_nMode1][4];
        nISet= (double)dRec[m_nMode1][5];
        nTimeDelay1 = (int)dRec[m_nMode1][6]*10;
        Sub71.nADCount1 =(int)dRec[m_nMode1][7];    


        if(Sub71.nADCount1<1 ){
            Sub71.nADCount1=1;
        }
        if( Sub71.nADCount1>300){
            Sub71.nADCount1=300;
        }

        nFil=(int)dRec[m_nMode1][8];//SYLEE130706
        nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130704        
        nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130704
        nHVGain = (int)dRec[m_nMode1][11];  //SYLEE130704

        nIGain = (int)dRec[m_nMode1][12];       //ACE380 //SYLEE150123    
        n4wGain = (int)dRec[m_nMode1][13];     //ACE380//SYLEE150123  


        SysInfoSer13.m_nData[PART_UPPER][m_nMode1][0][1]=(double)nTimeDelay1;//delay time   //son220411
        SysInfoSer13.m_nData[PART_LOWER][m_nMode1][0][1]=(double)nTimeDelay1;               //son220411

        //=========================================================================================
        //interlock

        if(nCC!=1 && nCC!=2){ 
            //fclose(fp);   //son220426_2

            AfxMessageBox(" Error No 17024, \n    CC CV    ( NO 1,2 ? ) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
            return ;
        }

        //son?????  CV모드기능 머지할 때에는 SysSet13.m_n4WCVMode 체크가 있는데 
        //          4W 정도개선 기능 코드에는  SysSet13.m_n4WCVMode 체크 빠짐. 일단 4W 정도개선 코드와 동일하게 
        //          SysSet19.m_n4WHVon만 체크하도록 수정. 
        //if(SysSet13.m_n4WCVMode!=1){   //sylee200321-4WCVMODE
        if(SysSet19.m_n4WHVon!=1){  //sylee200217-4WCVMODE   //sylee200515-patch
            if( nVSet<1 || nVSet>20 ){      
                //fclose(fp); //son220426_2
                AfxMessageBox(" Error No 7714, \n    Votage Set Range Error ( 4W  1~20  V) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
                return ;
            }
        }
        else{ //sylee191023-1
            if( nVSet<1 || nVSet>100 ){ //sylee190225-4w power up   
                //fclose(fp); //son220426_2
                AfxMessageBox(" Error No 7714, \n    Votage Set Range Error ( 4W  1~100  V) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
                return ;
            }
        } //sylee191023-1

        if( nVRel==1 || nVRel==2  || nVRel==3 || nVRel==4 ){


        }
        else
        {       
            //fclose(fp); //son220426_2
            AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
            return ;
        }

        //****************************************************************8
        
        //son m_nMode 171인 경우
        if(mode==1) //sylee150128 ace380
        {

            //son Power 관련 Output 비트를 모두 초기화 (D96 All Init.  HV, LV  VB All Relay off)
            nRet1=Sub71.AOnPoweroff2();//sylee20120702  
            if(nRet1!=1){
                //fclose(fp); //son220426_2
                AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
                return  ;
            }

            //--------------------------
            //son VB Rel(source) 설정
            if(SysSet19.m_n4WHVon!=1)  //sylee200217-4WCVMODE //sylee200515-patch
            {
                if( nVRel==1){
                    nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
                }
                else if( nVRel==2){
                    nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
                } 
                else  if( nVRel==3){//sylee200224-1-4WCVMODE
                    nRet1=Sub71.AOnVSetMulti1(0,0,nVSet,0) ;//sylee200224-1
                }
                else  if( nVRel==4){//sylee200224-1-4WCVMODE
                    nRet1=Sub71.AOnVSetMulti1(0,0,0,nVSet) ;//sylee200224-1
                }  
            }
            else{   //sylee191023-1 //sylee200515-patch
                if( nVRel==1){
                    nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
                }
                else if( nVRel==2){
                    nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
                } 
            }   //sylee191023-1 //sylee200515-patch

            if( nRet1!=1){
                //fclose(fp);  //son220426_2
                AfxMessageBox(" Error No 1023, \n   V SET ERROR !,   SW TEST Run Start Error!!!"); 
                return ;
            }

            Sub71.Delay_100ns(100000);  //500->10ms     //sylee150202


            tStart1=GetMilSecond2();

            Lo1=30; //sylee150202  3->30  


Task101:  

            //------------------------
            //son Read VB (HW Check)
            if(SysSet19.m_n4WHVon!=1)   //sylee200217-4WCVMODE
            {
#ifdef _PC_TEST_
                Sub71.V1 =15;
#else
                Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
#endif

                if(nVSet<5){
                    dVbR2=nVSet*(0.35);//10%
                }
                else {
                    dVbR2=nVSet*(0.1);//10%
                }
                dVbR1=nVSet-dVbR2;
                dVbR2=nVSet+dVbR2;

                if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){
                    if(Lo1-->0){
                        Sub71.Delay_100ns(300000);  //30ms //sylee150202  300->30  
                        goto Task101;
                    }

                    str.Format("Error No 17023,\n  \n\n  Power Check? \n\n\n   Calibration NO=%d VB READ Error!!! \n VB Set=%.1f, VB Read =%.1f ,\n Range(VB low=%.1f, VB high =%.1f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
                    AfxMessageBox(str); 
                    //  return ;
                }
                else {
                    Sub71.V1=nVSet;
                } 
            }//if(nFlag4WRelayOff!=1){  //sylee190705-1

            //--------------------------
            //son VC READ  INIT SET
            Sub71.AOnReadV1Enable1(5);//sylee130704   all off  

            if(nIFilter!=1  &&  nIFilter!=2 ){
                AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
                nIFilter=1;
            }

            //----------------------------
            //son I Filter 설정  (ADC_A)
            Sub71.I_Filter(nIFilter);//sylee130704

        }// if(mode==1){//sylee150128 ace380


        //---------------------
        //son I Gain 설정 
        if( nIGain==2){//sylee150123 //ACE380
            Sub71.I_Gain(2);  //10x  =io on     
        }
        else{
            Sub71.I_Gain(1);  //1x  =io off
        }

        //---------------------
        //son 4W Gain 설정 
        if( n4Wire==4 ) //ACE380
        {   
            if(n4wGain==2){
                Sub71.On4W_GainSet(2); 
            }
            else  if(n4wGain==3){
                Sub71.On4W_GainSet(3); 
            }
            else  if(n4wGain==4){
                Sub71.On4W_GainSet(4);
            }
            else{
                Sub71.On4W_GainSet(1);
            }
        }
        else{
            Sub71.On4W_GainSet(1);
        }


        //------------------
        //son I Value Set
        
        if(mode==1){//sylee150128 //ACE380
            Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);
        }
        else // if(mode==1){//sylee150128 //ACE380
        {

            Sub71.nModeIR=1;
            if(nISet==1){  //sylee150715
                Sub71.AOnISet1Run1(nISet );     
            }
            else{
                Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE150701
            }
        } //    if(mode==1){//sylee150128 //ACE380


        //------------------------------------
        //son ADC B Filter를 VC Fast로 설정
        if(SysSet19.m_n4WHVon!=1){//sylee200515-patch
            Sub71.FilterSet(1);  //sylee140617
        }else{//sylee200515-patch
            Sub71.FilterSet(4); //sylee200515-patch
        }

        tStart2=GetMilSecond2(); 
        nT[0]=(double)(tStart2-tStart1)/1000.;  //test    챨 
        ::ZeroMemory(&dCalA1, sizeof(dCalA1));//temp//gobal   //4WSETUP   
        //CalibrationLS1_Func(); //sylee140708         //son220419_5 미사용으로 삭제 

        nRet1=Sub71.OnPinAllReset1(); ///PIN ALL RESET 
        Sub71.Delay_100ns(50000);  //5ms //sylee150709  ray 
        Sub71.nADCount1 = 1;    //sylee150713



        //-----------------------
        //son  예비 4W 검사: 
        //     4W mode 1 (CalMode 171) 에 대해 4개의 4W pin을 설정하고 V값과 I 값을 측정을 Net갯수 만큼 반복
        int ret;
        if(mode==1)
        {
            for( net=1; net<=nEnd; net++ )  //count //son-4WDUT16-190704
            {
                ret = Sub71.AOnPinSet1(2, n4WNet[net][_4W_PIN4]-1, 1);   //VB  TR       //son-4WDUT16-190704
#if 1   
                if (ret < 0)        //son200103-32K:    nPin=PinMap[2][No] 가 -1이면 없는 카드.
                {       
                    str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d (VB_TR)  ",  
                            n4WNet[net][_4W_PIN4]-1);
                    AfxMessageBox(str);
                    //fclose(fp); //son220426_2
                    return ;
                }
#endif


                ret = Sub71.AOnPinSet1(2, n4WNet[net][_4W_PIN3]-1, 2);   //VC  TR PinNo(0~8k) //SYLEE20111022   nMod =1,VB,    =2VC,
#if 1   
                if (ret < 0)        //son200103-32K:    nPin=PinMap[2][No] 가 -1이면 없는 카드.
                {       
                    str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d  (VC_TR) ",  
                            n4WNet[net][_4W_PIN3]-1);
                    AfxMessageBox(str);
                    //fclose(fp); //son220426_2
                    return ;
                }
#endif

                for(k1=1; k1<=300; k1++){ //300uS
                    if(net==1 && k1<=10){
                        Sub71.Delay_100ns(200);  //20uS
                    }
                    Sub71.Delay_100ns(100);  //10uS
                    Sub71.AOnReadV1I1(5); // READ  4W V  , I 
                    if( Sub71.Y1> (nISet*0.6)){//ok
                        break;
                    }
                    if(k1==300){//fail
                        nPos1[net]=2;
                    }
                }

                Sub71.OnPinAllReset1(); ///PIN ALL RESET 

                //son-4WDUT16-190704
                ret = Sub71.AOnPinSet1(2, n4WNet[net][_4W_PIN1]-1, 1);    //VB  TR PinNo(0~8k) //SYLEE20111022   nMod =1,VB,    =2VC,  //SYLEE20111118
#if 1   
                if (ret < 0)        //son200103-32K:    nPin=PinMap[2][No] 가 -1이면 없는 카드.
                {       
                    str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d (VB_TR) ",  
                            n4WNet[net][_4W_PIN1]-1);
                    AfxMessageBox(str);
                    //fclose(fp); //son220426_2
                    return ;
                }
#endif
                ret = Sub71.AOnPinSet1(2, n4WNet[net][_4W_PIN2]-1, 2);    //VB  RELAY PinNo(0~8k) //SYLEE20111022   nMod =1,VB,    =2VC,  //SYLEE20111118
#if 1   
                if (ret < 0)        //son200103-32K:    nPin=PinMap[2][No] 가 -1이면 없는 카드.
                {       
                    str.Format("ERROR NO 7002-9, \nSub71.AOnPinSet1(2, ...) Error! \n\n\nCan't find hwPin for jigPin_%d (VC_TR) ",  
                            n4WNet[net][_4W_PIN2]-1);
                    AfxMessageBox(str);
                    //fclose(fp); //son220426_2
                    return ;
                }
#endif

                for(k1=1; k1<=300; k1++) //300uS
                { 
                    if(net==1 && k1<=10){//200us
                        Sub71.Delay_100ns(200);  //20uS
                    }
                    Sub71.Delay_100ns(100);  //10uS
                    Sub71.AOnReadV1I1(5); // READ  4W V  , I 
                    if( Sub71.Y1> (nISet*0.6)){//ok
                        break;
                    }
                    if(k1==300){//fail
                        nPos1[net]=2;
                    }
                }
                Sub71.OnPinAllReset1(); ///PIN ALL RESET 

            }//end of  for( j=1; j<=nCount; j++ )  //count

        }// if(mode==1)//sylee140617


        //------------------------------
        //son ADC B Filter를 4W로 설정
        Sub71.FilterSet(4);//sylee140617     

        //------------------------------
        //son CC/CV Filter 설정
        if( m_nMode1>=176 &&  m_nMode1<=180  ){ //sylee200217-4WCVMODE  //sylee200515-patch 
            Sub71.OnCCCV_Set(2);  //cv  
        }
        else{
            Sub71.OnCCCV_Set(1);  //cc   
        } 
        Sub71.Delay_100ns(50000); //5ms //sylee150202

        //////////////////////////////////////////////////////////////////
        n4w100=1;
        nLoop=1;  //sylee150203test

        int KcountTmp;//4WSETUP
        Sub71.nADCount1 =(int)dRec[m_nMode1][7];

        if(nADCx5==2){//sylee180611-1    
            Sub71.nADCount1 =(int)dRec[m_nMode1][7]*10; //sylee181120 //RAY  
        }
        if(Sub71.nADCount1<1 ){
            Sub71.nADCount1=1;
        }
        if(Sub71.nADCount1>300 ){
            Sub71.nADCount1=300;
        } 
        //////////////////////////////////////////////////////////////////

        if(mode==1 || mode==2 || mode==3 || mode==4  || mode==5 ) //sylee200319-4WCVMODE    //sylee200515-patch
        {      

            Sub71.Delay_100ns(10000); // 1mS

            if(nEnd<10)
            {
                for(int k12=1; k12<=1 ; k12++) // net
                {   
                    for( int k11=1; k11<=10; k11++){  //sylee180307  10->3
                        Sub71.AOnPinSet1(2 ,n4WNet[1][_4W_PIN4]-1, 4);      //son190704-4WDUT16
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY1);   
                        Sub71.AOnPinSet1(2 ,n4WNet[1][_4W_PIN2]-1, 3);      
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY2);   
                        Sub71.AOnPinSet1(2 ,n4WNet[1][_4W_PIN3]-1, 2); 
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY3);   
                        Sub71.AOnPinSet1(2 ,n4WNet[1][_4W_PIN1]-1, 1);                 
                        Sub71.Delay_100ns(nTimeDelay1); 
                        Sub71.AOnReadV1I1(5); // READ  4W V  , I                         
                        Sub71.OnPinAllReset1(); 
                    }
                }
            }
            else
            {        

                for(int k12=1; k12<=1 ; k12++){   // net
                    for( int k11=1; k11<=10; k11++){
                        Sub71.AOnPinSet1(2 ,n4WNet[k11][_4W_PIN4]-1, 4);    //son190704-4WDUT16
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY1);   
                        Sub71.AOnPinSet1(2 ,n4WNet[k11][_4W_PIN2]-1, 3);  
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY2);   
                        Sub71.AOnPinSet1(2 ,n4WNet[k11][_4W_PIN3]-1, 2); 
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY3);   
                        Sub71.AOnPinSet1(2 ,n4WNet[k11][_4W_PIN1]-1, 1);                       
                        Sub71.Delay_100ns(nTimeDelay1); 
                        Sub71.AOnReadV1I1(5); // READ  4W V  , I                         
                        Sub71.OnPinAllReset1(); 
                    }
                }
            }    
        }
        //sylee190430-1 Sub71.Delay_100ns(10000); // 1mS
        ///////////////////////////////////////////////////////

        //-------------------------------------------------------------------------------------------
        //son 4개의 4W pin을 설정하고 V값과 I 값을 측정, R값 계산을  Net갯수 만큼 반복 -> 이걸 다시 nCount 만큼 반복 측정
        //    계산 결과는 dR14w[][], dVB14w[][], dVC14w[][], dI14w[][]에 임시 저장했다가 Grid Matrix에 출력한다. 
        //    Matrix의 값이 계산결과이며 다른 곳에서도 값을 참고하려면 Matrix에서 read하여 사용한다.
        //     (문제 있음. 소수점 2자리까지만 Grid로 출력이 되므로 다른곳에서 값을 Grid에서 참고하면 
        //        data 왜곡이 발생해서 avg, sigma등을 재계산 했을때 오차가 있다.)
        
        for( j=1; j<=nCount; j++ ) //count 
        {  
            for( net=0; net<=nEnd; net++){          //son190704-4WDUT16
                dR14w[j+5][net]=0.0;   //R1;
                dVB14w[j+5][net]=0.0;   //vb
                dVC14w[j+5][net]=0.0;  //vc
                dI14w[j+5][net]=0.0;    //net
            }
            nPassCo1=0;//sylee150710    



            for( net=1; net<=nEnd; net++) // net    //son190704-4WDUT16
            {   

                KcountTmp=Kcount; //4WSETUP

                for( k=0; k<KcountTmp; k++) //sylee131230-ray-test  /4WSETUP  Tmp
                { 
                    nZeroCo1=0;//sylee180520-1

Task1: //sylee131227
                    if( nStop==2){
                        AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
                        goto task2;
                    }

 

                    if( nPos1[net]==2){
                        Sub71.R1=8888888; //SYLEE200318-1-1-4WCVMODE
                    }
                    else
                    {

                        n4W_Retry1=0;   //son220714 CC 모드일때 무한루프문제 수정.
                                        //    CC인데 if( n4W_IDropCheck1(net)==-1) 이면 n4W_Retry1++하고 Task5701:로 점프하는데
                                        //    점프하자마자 n4W_Retry1=0이 되어서 무한루프에 빠짐.
Task5701://sylee180823-1
                        //son220714 n4W_Retry1=0;   
                        d4W_R1=0.0; //sylee200515-patch
                        dmin1=0.0;  dmax1=0.0; //sylee200515-patch

                        //-----------------
                        //son 4W pin 설정

                        //son 먼지제거 기능:
                        //    핀설정 방향을 바꿔가면서 테스트해서 전기극성때문에 붙었던 먼지를 제거
                        if(n4W_Retry1==1){
                            nPinNo[1]= n4WNet[net][_4W_PIN4]-1 ;  //vb tr   //SYLEE220125
                            nPinNo[2]= n4WNet[net][_4W_PIN1]-1 ;  //vc tr   
                            nPinNo[3]= n4WNet[net][_4W_PIN3]-1 ;  //vb relay
                            nPinNo[4]= n4WNet[net][_4W_PIN2]-1 ;  //vc relay                            
                        }else if(n4W_Retry1==2){
                            nPinNo[1]= n4WNet[net][_4W_PIN1]-1 ;  //vb tr   //SYLEE220125
                            nPinNo[2]= n4WNet[net][_4W_PIN4]-1 ;  //vc tr
                            nPinNo[3]= n4WNet[net][_4W_PIN2]-1 ;  //vb relay
                            nPinNo[4]= n4WNet[net][_4W_PIN3]-1 ;  //vc relay                                    
                        }else if(n4W_Retry1==3){
                            nPinNo[1]= n4WNet[net][_4W_PIN4]-1 ;  //vb tr   //SYLEE220125
                            nPinNo[2]= n4WNet[net][_4W_PIN1]-1 ;  //vc tr
                            nPinNo[3]= n4WNet[net][_4W_PIN3]-1 ;  //vb relay
                            nPinNo[4]= n4WNet[net][_4W_PIN2]-1 ;  //vc relay                            
                        }else{
                            nPinNo[1]= n4WNet[net][_4W_PIN1]-1 ;  //vb tr   //SYLEE220125
                            nPinNo[2]= n4WNet[net][_4W_PIN4]-1 ;  //vc tr
                            nPinNo[3]= n4WNet[net][_4W_PIN2]-1 ;  //vb relay
                            nPinNo[4]= n4WNet[net][_4W_PIN3]-1 ;  //vc relay
                        }                


                        Sub71.AOnPinSet1(2 ,nPinNo[1], 2);    //Vc  TR PinNo(0~8k)   //sylee200220-1 ray
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY3);   
                        Sub71.AOnPinSet1(2 ,nPinNo[2], 1);    //Vb  TR PinNo(0~8k)   //sylee200220-1 ray                                
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY3); 
                        Sub71.AOnPinSet1(2 ,nPinNo[3], 4);    //Vc  RELAY PinNo(0~8k)  //sylee200220-1 ray                              
                        Sub71.Delay_100ns(DEF_4W_PIN_DELAY2);  
                        Sub71.AOnPinSet1(2 ,nPinNo[4], 3);    //Vb  RELAY PinNo(0~8k)  //sylee200220-1 ray 
                        Sub71.Delay_100ns(nTimeDelay1); //sylee170313-1       ray fix delay 

                        calPart =FileSysInfo01.checkUseCalDataPart(nPinNo[1],  nPinNo[3]);    //son220419_2

                        //----------------------------
                        //son VC값, I 값 측정

                        Sub71.AOnReadV1I1(5); //sylee180221-1  

                        if(SysSet13.m_n4WCVMode!=1){ //sylee200217-4WCVMODE
                            if( n4W_IDropCheck1(net)==-1){ //sylee180823-1 //sylee180912-1
                                n4W_Retry1++;
                                if(n4W_Retry1<5){  
                                    goto Task5701;//sylee180823-1
                                }
                            }
                        }

                        //son raw R 값 계산 (I range 반영)
                        ARun201_4w_R1(nISet);  //sylee150710            

                        //son real R 값 추정
                        ARun201_4w_R_Cal1(m_nMode1, calPart);//sylee150710        //son220412   

                        nPassCo1=0;//sylee150710 
                        Sub71.OnPinAllReset1(); ///PIN ALL RESET     

                        d4W_R1+=Sub71.R1; //sylee200515-patch

                        if(n4W_RetrySet>=4){ //sylee200515-patch
                            if(n4W_Retry1==0){
                                dmin1=Sub71.R1;
                                dmax1=Sub71.R1;
                            }else{
                                if(dmin1>Sub71.R1)
                                    dmin1=Sub71.R1;
                                if(dmax1<Sub71.R1)
                                    dmax1=Sub71.R1;
                            }   
                        }

                        n4W_Retry1++;
                        if(n4W_Retry1<n4W_RetrySet){//sylee200512  //sylee200515-patch
                            goto Task5701;
                        }

                        if(n4W_RetrySet<4){ //sylee200515-patch
                            Sub71.R1=d4W_R1/n4W_RetrySet;
                        }else{
                            Sub71.R1=(d4W_R1-dmax1-dmin1)/(n4W_RetrySet-2);
                        }

                        if(nRunMode1!=1){//sylee170313-1      ray fix delay 
                            if( nLoop >0){                  
                                if(nLoop==1){
                                    net=1;          
                                }
                                else {                   
                                    net=1;               
                                    if(net>=nEnd){//sylee140121
                                        net=1;//SYLEE150129
                                    }
                                }
                                nLoop--;
                                goto Task1;     
                            }
                        }//end of  if(nRunMode1!=1){//sylee170313-1       ray fix delay 



                        if(Sub71.Y1<0.0000001){
                            Sub71.Y1=0.0000001;
                        }

                        if(Sub71.Y1>0)
                        {
                            double nT1,nT2;//4WSETUP

                            nT1=nISet-nISet*0.1;
                            nT2=nISet+nISet*0.1;

                            if(nT1<Sub71.Y1){
                                if(Sub71.Y1<nT2){
                                    if(SysSet13.m_n4WCVMode!=1){   //sylee200217-4WCVMODE //sylee200515-patch                        
                                        Sub71.Y1=nISet;//SYLEE140107                                    
                                    }                               }
                            }                        

                            //  Sub71.Y1=nISet; //SYLEE171024-1 I FIX RAY
                            if( nIR==1 || nIR==2){//I mA                 
                                Sub71.R1=((Sub71.W1)/Sub71.Y1)*10.;//sylee161207-0 x10    ray ok                 
                            }
                            else if( nIR==3 || nIR==4  || nIR==5 ){  //I uA          
                                Sub71.R1=((Sub71.W1)/Sub71.Y1)*1000.;// /1000                    
                            }
                            else {
                                Sub71.R1=9999999999; 
                            }       
                        }
                        else {
                            Sub71.R1=9999999999; 
                        }

                        if(Sub71.W1<=0.0){
                            Sub71.R1=99999999; //sylee150713  import.
                        }

                    }//sylee140617   //end of if( nPos1[k]==2){      }else{


                    Sub71.R1=Sub71.R1*1000.;   //R1;

                    if(Sub71.W1>10.5){//sylee150812 //SYLEE161202-1   
                        Sub71.R1=9999999.;   //R1;//sylee150812
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                    if( k==1){                                    
                        dR14w[1][net] = Sub71.R1;           //son190704-4WDUT16
                        dR14w[3][net] = Sub71.R1;           //son190704-4WDUT16
                    }
                    else {          
                        if( Sub71.R1 < dR14w[1][net]){      //son Min
                            dR14w[1][net] =Sub71.R1;
                        }   
                        if( Sub71.R1 > dR14w[3][net]){      //son Max
                            dR14w[3][net] =Sub71.R1;
                        }
                    }

                    ///////////////////////////////////////////////////////////////////////////////////////////////////

                    dR14w[j+5][net]  = dR14w[j+5][net] + Sub71.R1;   //R1;          //son190704-4WDUT16
                    dVB14w[j+5][net] = (float)(dVB14w[j+5][net]+Sub71.V1);  //vb
                    dVC14w[j+5][net] = (float)(dVC14w[j+5][net]+Sub71.X1);  //vc
                    dI14w[j+5][net]  = (float)(dI14w[j+5][net]+Sub71.Y1);   //net 

                }//        for( k=0; k<10; k++){  //4WSETUP
                //=================================================================================================

                if(Sub71.R1<= MAX_4W_R){//sylee141211   //sylee171026  100000->200 000
                    nPassCount1++;//sylee141211
                }

                //son dR14W[j+5][net]을 KCount로 평균낸다.
                if(KcountTmp < 3)//sylee20120304   //4WSETUP
                {
                    dR14w[j+5][net]  = dR14w[j+5][net]  / KcountTmp;   //R1;            //son190704-4WDUT16
                    dVB14w[j+5][net] = dVB14w[j+5][net] / KcountTmp;   //vb
                    dVC14w[j+5][net] = dVC14w[j+5][net] / KcountTmp;  //vc
                    dI14w[j+5][net]  = dI14w[j+5][net]  / KcountTmp;    //net
                }

                //son 3이상이면, Min, Max는 빼고 dR14W[j+5][net]을 KCount로 평균낸다.
                else //sylee20120304  //Kcount  Min Max eject
                {
                    dR14w[j+5][net]  = (dR14w[j+5][net] - dR14w[1][net] - dR14w[3][net]) / (KcountTmp-2);   //R1;
                    dVB14w[j+5][net] = dVB14w[j+5][net] / KcountTmp;   //vb
                    //  dVC14w[j+5][net]=dVC14w[j+5][net]/Kcount;  //vc
                    dVC14w[j+5][net] = (dVC14w[j+5][net] - dVC14w[1][net] -dVC14w[3][net]) / (KcountTmp-2);   //dVC14w[j+5][net]/Kcount;  //vc
                    dI14w[j+5][net]  = dI14w[j+5][net] / KcountTmp;    //net
                }


                //==============================================================================================================
                //int part = PART_UPPER;  //son220412
                int part = calPart;  //son220419_7
                if(  (m_nMode3!=1)&&(m_nMode2==2)  ) //SYLEE20120304
                {   

                    int nPo;
                    nPo=0;

                    ka=(int)gdCal1[part][m_nMode1][0][0];// Cal  list 개수             
                    Sub71.R1=dR14w[j+5][net];       //son190704-4WDUT16


                    if(  (Sub71.R1>0)   &&   (ka>0) &&   (ka<=128) ){                                    
                        //sylee171026-2      for( int k2=0;  k2<ka;  k2++){  // 4WSETUP
                        for( int k2=1;  k2<=ka;  k2++){  // 4WSETUP  //sylee171026-2
                            if(gdCal1[part][m_nMode1][CAL_USE][k2]==1){  //gdCal1[part][m_nMode1][CAL_USE][k]  cal enable 1,0 
                                if( Sub71.R1< gdCal1[part][m_nMode1][CAL_REF_RAWR][k2]){   //gdCal1[part][m_nMode1][CAL_REF_RAWR][k]  cal R raw                                                            
                                    nPo=k2;
                                    k2=ka+1;  
                                } 
                            }
                        }                       

                        if(nPo==0){       //gdCal1[part][m_nMode1][CAL_REF_R][k]  cal ref  R 
                            Sub71.R1= 999999.9 ; //sylee161208-1  import
                        }
                        else {
                            if( (gdCal1[part][m_nMode1][CAL_REF_RAWR][nPo]-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPo-1])!=0.0){
                                aa=(Sub71.R1-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPo-1])
                                       / (gdCal1[part][m_nMode1][CAL_REF_RAWR][nPo]-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPo-1]);  // [3]= lookup table Calibration Raw R
                                ab=gdCal1[part][m_nMode1][CAL_REF_R][nPo]-gdCal1[part][m_nMode1][CAL_REF_R][nPo-1];// [2]= lookup table Calibration real R
                                Sub71.R1=ab*aa+gdCal1[part][m_nMode1][CAL_REF_R][nPo-1];                                           
                            }
                            else {
                                //son  raw R -> real R
                                Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][CAL_REF_RAWR][nPo-1])*gdCal1[part][m_nMode1][CAL_REF_R][nPo-1];  
                            }
                        }


                    }  

                    //son dR14w[6][net]  에 real R 값 저장
                    dR14w[j+5][net]=Sub71.R1;       //son190704-4WDUT16
                } 

            } //    for( net=1; net<=nEnd; net++){ 

        }  // for( j=1; j<=nCount; j++ ) //count 






task2:  //STOP JUMP POSTION


        Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018

        tStart1=GetMilSecond2();     
        nT[1]=(double)(tStart1-tStart2)/1000.;  //test   챨 



        int nCo1;

        if(nCount>10){
            nCo1=10;
        }
        else{
            nCo1=nCount;
        }

        //==========================================================================
        for( j=1; j<=5; j++) //INITIAL
        {  
            for( net=1; net<=nEnd ; net++){         //son190704-4WDUT16
                dR14w[j][net]=0; 
                dVC14w[j][net]=0; 
                dI14w[j][net]=0; 
            }
        }
        //==========================================================================
        for( j=1; j<=nCount; j++) //count 
        {  
            for( net=1; net<=nEnd ; net++){     //son190704-4WDUT16
                if( dR14w[j+5][net]<=0.0){       // 6~  END+6)
                    dR14w[j+5][net]=0.0;  
                } 
                if( dVC14w[j+5][net]<=0.0){
                    dVC14w[j+5][net]=0.0;  
                } 
                if( dI14w[j+5][net]<=0.0){
                    dI14w[j+5][net]=0.0;  
                } 

                dR14w[0][net]  = dR14w[0][net]  + dR14w[j+5][net];//sum     //son190704-4WDUT16
                dVC14w[0][net] = dVC14w[0][net] + dVC14w[j+5][net];//VC sum
                dI14w[0][net]  = dI14w[0][net]  + dI14w[j+5][net];//IC sum

            }//for( net=1; net<= nEnd ; net++){ 

        }//for( j=1; j<=nCount; j++){  //count 

        //===============================================================================
        for( net=1; net<=nEnd; net++){  //          //son190704-4WDUT16
            dR14w[2][net]=dR14w[0][net] / nCount;     //AVE  
            dVC14w[2][net]=dVC14w[0][net] / nCount;    
            dI14w[2][net]=dI14w[0][net] / nCount; 
        }
        //===============================================================================
        for( net=1; net<=nEnd; net++) //    auto mode   //son190704-4WDUT16
        {  
            if( mode==1){
                g_s4WNet[dutIdx].saData[net-1].n4wMode=0;   //son190704-4WDUT16
                str.Format("0");             
                m_grid1.SetTextMatrix(net, 1+_4W_MODE, str);
            }

            if(g_s4WNet[dutIdx].saData[net-1].n4wMode==0)   //son190704-4WDUT16
            {
                if(dR14w[2][net]>=999999){//sylee140708
                    g_s4WNet[dutIdx].saData[net-1].n4wMode = 0;//sylee140708    //son190704-4WDUT16

                }
                else
                {//sylee140708              

                    Sub71.R1=dR14w[2][net];  //4WSETUP     <===================              

                    if(Sub71.R1<0.1){
                        if(m_nMode1==171){
                            //sylee200515-patch Sub71.R1=0.1;
                            Sub71.R1=999999.0; //sylee200515-patch
                        }
                        else {
                            Sub71.R1=99999999.0;
                        }
                    }                           

                    if((dRec4w[mode][2]< Sub71.R1) &&  (Sub71.R1< dRec4w[mode][3]))     //sylee140714   
                    {
                        str.Format("%d", m_nMode1);              
                        if(m_nMode1>175){ //sylee200217-4WCVMODE //sylee200515-patch
                            g_s4WNet[dutIdx].saData[net-1].n4wMode = m_nMode1-5;        //son-4WDUT16-190704
                            str.Format("%d", m_nMode1-5);   
                        }else{
                            g_s4WNet[dutIdx].saData[net-1].n4wMode = m_nMode1;      //son-4WDUT16-190704
                            str.Format("%d", m_nMode1);     
                        }
                        m_grid1.SetTextMatrix(net, 1+_4W_MODE, str); 
                        n4WFlag[mode][net]=1;
                    }  

                }//sylee140708
            }
        }
        //==========================================================================  

        for( net=1; net<=nEnd ; net++)          //son190704-4WDUT16
        {  
            for( j=1; j<=nCount; j++)  //count 
            {
                if( ((n4WFlag[mode][net]==1) && (g_s4WNet[dutIdx].saData[net-1].n4wMode>0))   //son190704-4WDUT16
                        || ((g_s4WNet[dutIdx].saData[net-1].n4wMode==0)&&(mode==4)) )
                {
                    //MAX 
                    if( j==1)
                    {
                        //min                         
                        dR14w[1][net]= dR14w[j+5][net]; 
                        dVC14w[1][net]= dVC14w[j+5][net]; 
                        dI14w[1][net]= dI14w[j+5][net]; 

                        //avg
                        dR14w[2][net]= dR14w[j+5][net]; 
                        dVC14w[2][net]= dVC14w[j+5][net]; 
                        dI14w[2][net]= dI14w[j+5][net]; 

                        //max
                        dR14w[3][net]= dR14w[j+5][net]; 
                        dVC14w[3][net]= dVC14w[j+5][net]; 
                        dI14w[3][net]= dI14w[j+5][net];
                    }
                    else
                    {
                        //min
                        if( dR14w[j+5][net]<dR14w[1][net]){
                            dR14w[1][net]=dR14w[j+5][net];
                        }
                        if( dVC14w[j+5][net]<dVC14w[1][net]){
                            dVC14w[1][net]=dVC14w[j+5][net];
                        }
                        if( dI14w[j+5][net]<dI14w[1][net]){
                            dI14w[1][net]=dI14w[j+5][net];
                        }
                        //max
                        if( dR14w[j+5][net]>dR14w[3][net]){
                            dR14w[3][net]=dR14w[j+5][net];
                        }
                        if( dVC14w[j+5][net]>dVC14w[3][net]){
                            dVC14w[3][net]=dVC14w[j+5][net];
                        }
                        if( dI14w[j+5][net]>dI14w[3][net]){
                            dI14w[3][net]=dI14w[j+5][net];
                        }
                    }//if( j==1)  else

                }// if(   ((n4WFlag[mode][net]==1)&&(g_s4WNet[dutIdx].saData[net-1].n4wMode>0))   
                // ||    ((g_s4WNet[dutIdx].saData[net-1].n4wMode==0)&&(mode==4))   )


            }//for( j=1; j<=nCount; j++)  //count 

            if(  ((n4WFlag[mode][net]==1)&&(g_s4WNet[dutIdx].saData[net-1].n4wMode>0))   ||    //son190704-4WDUT16
                    ((g_s4WNet[dutIdx].saData[net-1].n4wMode==0)&&(mode==4))   ){
                dI14w[4][net]=dI14w[3][net]-dI14w[1][net];// 
            }

            //***************************************************************

            //son   dR14w[6~10][net]  R 값을 0.1 이하이면 모두 9999.0으로 설정. (171 mode는 0.1로)
            if( (m_nMode3==1)&&(m_nMode2==2)  ) //SYLEE20120304 
            {   
                for( int km=1; km<=(nCount+5) ; km++) //sylee140204 
                {  
                    Sub71.R1=dR14w[km][net];  //4WSETUP                      

                    if(Sub71.R1<0.1){
                        if(m_nMode1==171){
                            Sub71.R1=0.1;
                        }
                        else {
                            Sub71.R1=9999.0;
                        }
                    }                    
                    dR14w[km][net]=Sub71.R1;
                }

            }//END    if(  (m_nMode3==1)&&(m_nMode2==2)  ) //SYLEE20120304    
        }//for( net=1; net<= nEnd ; net++) 


        //==================================================
        //son 측정한 R 값을 Grid ' j+_4W_DATA1' 위치에 출력 
        //==================================================


        for( j=1; j<=nCo1; j++) // count   10회  //son j= 측정 횟수
        {  
            for( net=1; net<=nEnd; net++)       //son190704-4WDUT16 
            {  // 
                if( ((n4WFlag[mode][net]==1)&&(g_s4WNet[dutIdx].saData[net-1].n4wMode>0))       //son190704-4WDUT16
                        ||    ((g_s4WNet[dutIdx].saData[net-1].n4wMode==0)&&(mode==4))   )
                {

                    if( dR14w[j+5][net]<=0.0){
                        str.Format("N/A");  
                    }
                    else {
                        str.Format("%.1f", dR14w[j+5][net]);  
                    }   

                    if( g_s4WNet[dutIdx].saData[net-1].n4wMode==0){//sylee160520-5      //son190704-4WDUT16
                        str.Format("999999");
                        dR14w[j+5][net]=999999;

                        if(nPos1[net]==2){//SYLEE200318-1-4WCVMODE
                            str.Format("-888888");
                            dR14w[j+5][net]=888888;                  
                        }
                    }

                    m_grid1.SetTextMatrix(net, j+_4W_DATA1, str);                       
                }
            }
        }

        //=================================
        //son AVG, Min, Max  display
        //=================================
        
        for( net=1; net<=nEnd; net++)       //son190704-4WDUT16
        {  

            if(  ((n4WFlag[mode][net]==1)&&(g_s4WNet[dutIdx].saData[net-1].n4wMode>0))      //son190704-4WDUT16
                    ||    ((g_s4WNet[dutIdx].saData[net-1].n4wMode==0)&&(mode==4))   )
            {


                //-----------------------------------------------------
                //son Average 값을 Grid '1+ _4W_TEST_AVG' 위치에 출력
                if( dR14w[2][net]<=0.0){  //ave
                    str.Format("N/A");  
                }
                else {
                    str.Format("%.1f", dR14w[2][net]);  
                }

                if( g_s4WNet[dutIdx].saData[net-1].n4wMode==0){//sylee160520-5      //son190704-4WDUT16
                    str.Format("999999");
                    dR14w[j+5][net]=999999;
                    if(nPos1[net]==2){//SYLEE200318-1-4WCVMODE
                        str.Format("-888888");
                        dR14w[j+5][net]=888888;              
                    }
                }
                m_grid1.SetTextMatrix(net, 1+_4W_TEST_AVG, str);    //son190704-4WDUT16


                //-----------------------------------------------------
                //son Min 값을 Grid '1+ _4W_TEST_MIN' 위치에 출력
                if( dR14w[1][net]<=0.0){  //min
                    str.Format("N/A");  
                }
                else {
                    str.Format("%.1f", dR14w[1][net]);  
                }   

                if( g_s4WNet[dutIdx].saData[net-1].n4wMode==0){//sylee160520-5      //son190704-4WDUT16
                    str.Format("999999");
                    dR14w[j+5][net]=999999;
                    if(nPos1[net]==2){//SYLEE200318-1-4WCVMODE
                        str.Format("-888888");
                        dR14w[j+5][net]=888888;                  
                    }
                }
                m_grid1.SetTextMatrix(net, 1+_4W_TEST_MIN, str);    //son190704-4WDUT16



                //-----------------------------------------------------
                //son Max 값을 Grid '1+ _4W_TEST_MAX' 위치에 출력
                if( dR14w[3][net]<=0.0){  //max
                    str.Format("N/A");  
                }
                else {
                    str.Format("%.1f", dR14w[3][net]);  
                }

                if( g_s4WNet[dutIdx].saData[net-1].n4wMode==0){//sylee160520-5      //son190704-4WDUT16
                    str.Format("999999");
                    dR14w[j+5][net]=999999;
                    dR14w[2][net]=999999;

                    if(nPos1[net]==2){//SYLEE200318-1-4WCVMODE
                        str.Format("-888888");
                        dR14w[j+5][net]=888888;
                        dR14w[2][net]=888888;
                    }
                }
                m_grid1.SetTextMatrix(net, 1+_4W_TEST_MAX, str);    //son190704-4WDUT16



                //son Diff 값 계산
                dR14w[4][net]=dR14w[3][net] - dR14w[1][net]; 

                //son Diff %  값 계산
                dR14w[5][net]=dR14w[4][net] / dR14w[2][net]  *100. ;//  


                for( m=0; m<=(nCount+5); m++){
                    dR14wT1[m][net]=dR14w[m][net];
                }

                //son Avg 값을 계산해서 nManualR1[nManualCount100][net] 에 저장.
                nManualR1[nManualCount100][net] =(float)dR14w[2][net]; 

            }//if( -  ((n4WFlag[mode][net]==1)&&(g_s4WNet[dutIdx].saData[net-1].n4wMode>0))   
            //||    ((g_s4WNet[dutIdx].saData[net-1].n4wMode==0)&&(mode==4))   )

        }// end of         for( net=1; net<=nEnd; net++)  // 


    }  //for( mode=1; mode<=5; mode++)   //  range auto change



    nRelay1OffSkip=0;//sylee170424-1



    if( nManualCount100Flag==1){//sylee20120305
        str.Format("%d",nManualCount100);
        m_Label101.SetCaption(str); 
    }


    if( nManualCount100<  MAX_MANUAL_TEST){//sylee160223-6      //son210928 MAX_MANUAL_TEST 100
        nManualCount100++;
    }
    else {
        AfxMessageBox("   Error No 1701   4w  setup  count 100 over  !.", MB_OK);//sylee160223-6
    }

    for( net=1; net<=nEnd; net++){          //son190704-4WDUT16
        for( m=0; m<=(nCount+5); m++){
            dR14w[m][net]=dR14wT1[m][net];
        }
    }

    //son Power 관련 Output 비트를 모두 초기화 (D96 All Init.  HV, LV  VB All Relay off)
    Sub71.AOnPoweroff2();  //sylee150128 ace380  


    CTime   curTime = CTime::GetCurrentTime();


    int nLogMode;

    nLogMode=2;

    strfName.Format("d:\\LOG\\001_RESULT_4W_%2dH%2dM%2dS.csv",curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond() );
    ::ZeroMemory(&fName,sizeof(fName)); 
    strcat( fName , strfName );


    fp = fopen(fName,"wt"); 
    if(fp == NULL){ 
        str.Format("Error    file open Error  ?  ,   \\RESULT4W\\001_RESULT_4W_%2dH%2dM%2dS.csv     ",m_nMode1);
        AfxMessageBox( str, MB_OK);  
        return  ;
    }


    fprintf(fp, "  4W Make Date,  %04d-%02d-%02d,  %02d:%02d:%02d,  \n\n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

    fprintf(fp, " NET NO.,  "); 

    for( j=1; j<=nCount; j++){
        fprintf(fp, "Test %d  , ",j );
    }
    if( nLogMode==2){//MIN  
        fprintf(fp, "*MIN., *AVE., *MAX.,* Dif.Value ,* Dif.Rate ");
    }

    fprintf(fp, "   \n ");

    for( net=1; net<=nEnd; net++){          //son190704-4WDUT16
        fprintf(fp, "  %d, ", net); 

        for( j=1; j<=nCount; j++){
            fprintf(fp, "  %.3f, ", dR14w[j+5][net]);     
        }

        if( nLogMode==2){ //min  avg  max   diff diffRate
            fprintf(fp, "  %.1f, %.1f, %.1f, %.1f, %.1f, ", dR14w[1][net],dR14w[2][net],dR14w[3][net],dR14w[4][net],dR14w[5][net]);
        }

        fprintf(fp, " \n");
    }


    fclose(fp); 

    if( nStop==2){
        AfxMessageBox("   4W  SETUP   Stop!!!");     
    }

    double NetTotle1;

    if(nT[1]>0){
        NetTotle1 = ((nEnd * nCount) / nT[1]) *1000.0;
    }
    else {
        NetTotle1=0;
    }
    str.Format(" 4W  SETUP   END!!! \n\n\n   => Set Time   %.3f mS    => Process Time    %.3f mS   \n\n\n\n  => Net/Second= %.3f       \n\n ", 
            nT[0],nT[1],NetTotle1  );


    //son Manual Test FailNet Label 출력-----
    nPassCount1=0;//sylee150708
    for( net=1; net<=nEnd; net++){//sylee150708     //son190704-4WDUT16
        if(g_s4WNet[dutIdx].saData[net-1].n4wMode>=171){//sylee150708       //son190704-4WDUT16
            nPassCount1++;//sylee150708
        }
    }
    str.Format("%d",nPassCount1-nEnd);//sylee150708 
    m_Label102.SetCaption(str); 
    Sub71.AOnReadV1Enable1(5);//sylee170417   all off

    Sub71.OnCCCV_Set(1);  //sylee200217-4WCVMODE
    nFlag_4W_OUT50V=0;//SYLEE200411 //sylee200515-patch

    return;

}


//sylee20120131
void CModeSub17::OnButtonTestInit() //test  manual  inial
{
    //son Manual Test 카운트를 1로 초기화한다.
     nManualCount100=1;   //sylee20120131
     nManualCount100Flag=1;   //sylee20120131

    //son 4W Test 결과 data를 초기화한다.
     ::ZeroMemory(&nManualR1, sizeof(nManualR1)); 
     m_Label101.SetCaption("ini.");
}

//=============================================================================================================
 



//=========================================================================================================================
//son 'Cancel'버튼 : 바로 직전 1회 측정된 data 취합을 취소한다. 
void CModeSub17::OnButtonTestCancel() //test  auto
{
    //sylee131112 OnButtonTestInit();

    CString str;//sylee141212

    if( nManualCount100>0){
        nManualCount100=nManualCount100-1;
    }

    str.Format("%d",nManualCount100-1);//sylee141212
    m_Label101.SetCaption(str);//sylee141212
    DoEvents(); DoEvents();//sylee141212

    //sylee141212
}

 

//son  Manual 테스트결과(nManualR1[j][net]) 를 Manual 수동 테스트 결과 파일에 보존 ("PathBDL"\4W_Setup.txt)
//son  Auto 테스트 결과 파일에도 보존 ("PathBDL"\4w_Setup_A.txt)하고 d4W_Log2[dutIdx][]영역에도 업데이트한다.
extern float d4W_Log2[MAX_DUT][MAX_4W_SAMPLE+10][MAX_4W_NET +1];//sylee170125   //memory //sylee180906-1
												//son231107 Def_4WMax1 -> MAX_4W_NET +1
void CModeSub17::OnButtonTestSave() //test  save
{
    CString   str,str1;
    CString   strfName,strfName1;
 
    int  j , nTotal;  
    FILE *fp ;
    //FILE *fp2 ;

    char fName1[200]/*, fName2[200]*/; 
    int nEnd;
    double dMin[3000], dMax[3000],dAvg[3000],dDiff[3000],dDiffRate[3000];       //son210916

    ///////////////////////////////////////////////////////////////////////////

    if( nManualCount100Flag<1  ||  nManualCount100<1) {
        str.Format("Error    manual test data count  <1  .  " );
        AfxMessageBox( str, MB_OK);  
        return;
    }


    nTotal=nManualCount100;

    if(nTotal<=1) //sylee140102
    {
        if( SysSet13.m_nSet7==1 ){//sylee130724
              ::ShellExecute(NULL,"open","EXCEl.EXE","D:\\4W.CSV","NULL",SW_SHOWNORMAL);     
        }
        else{                       
             ::ShellExecute(NULL,"open","notepad.EXE","D:\\4W.CSV","NULL",SW_SHOWNORMAL);                  
        }
        return;
    }

    //4W LOOP LOGGING
 
    CTime  curTime = CTime::GetCurrentTime();   
 
  
    ::ZeroMemory(&fName1, sizeof(fName1));//SYLEE150806
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE150806
    strcat( fName1 ,str);//SYLEE150806

    //son200320-4WDUT16: Multi DUT 구조로 수정.
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  
    if (dutIdx == 0)
        str.Format("\\4W_Setup.txt" ); //SYLEE150806
    else    
        str.Format("\\4W_Setup.txt%d", dutIdx+1 ); 
    strcat( fName1 , str ); 

//      ::ZeroMemory(&fName2,sizeof(fName2));
//    strcat( fName2 , fName1 );///sylee150806 
 

    fp = fopen(fName1,"wt"); 
    if(fp == NULL){ 
        str.Format("Error    %s file open   ?  Error, Saving Error !     file close!   ", fName1);
        AfxMessageBox( str, MB_OK);  
        return  ;
    }
 
    nEnd=(int)g_s4WNet[dutIdx].wCount;

    //----------------------------------------------
    //son 4W data 헤더를 "4W_Setup.txt"에  출력
    //----------------------------------------------
    
    fprintf(fp, "  4W Make Date ,Year:%04d,Month:%02d,Day:%02d,    ,  %02dH , %02dM , %02dS,  \n ", 
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
    fprintf(fp, " * Test  total count =, %d, * Net  total count =, %d,    \n ",nManualCount100-1,nEnd );//sylee1507=806
    fprintf(fp, " NET NO.,     ,Pin1 ,Pin2 ,Pin3 ,Pin4 ,     ");   


    for( i=1; i< nManualCount100; i++)
    {
        if (i==1){
            fprintf(fp,  "        #%d     ,   ", i);           
        }
        else{
            fprintf(fp,  "        #%d     ,   ", i);           
        } 
    }

    fprintf(fp, "                 MIN.,       AVE. ,       MAX. ,           Dif.Value ,   Dif.Rate, ");
    fprintf(fp, "  \n ");

    nEnd=(int)g_s4WNet[dutIdx].wCount;      //son190704-4WDUT16

    ::ZeroMemory (&dMin, sizeof(dMin));
    ::ZeroMemory (&dMax, sizeof(dMax));
    ::ZeroMemory (&dAvg, sizeof(dAvg));
    ::ZeroMemory (&dDiff, sizeof(dDiff));
    ::ZeroMemory (&dDiffRate, sizeof(dDiffRate));


    //----------------------------------------------
    //son 4W Data를 "4W_Setup.txt" 파일에 출력
    //----------------------------------------------

    int net;
    for( net=1; net<=nEnd; net++)       //son190704-4WDUT16
    {

        //------------------------
        //son  Net, Pin, R 출력
        for( j=1; j<nTotal; j++)
        {
            //son 먼저 Net, Pin4개, 첫번째 R 값을 찍고
            if(j==1){ 
                //son         NetNo         Pin1  Pin2  Pin3  Pin4        R
                fprintf(fp, " Net%d,  PIN,   %d,   %d,   %d,   %d,        %10.2f,   ", 
                        net,
                        g_s4WNet[dutIdx].saData[net-1].waPin[0], g_s4WNet[dutIdx].saData[net-1].waPin[1],   //son190704-4WDUT16
                        g_s4WNet[dutIdx].saData[net-1].waPin[2], g_s4WNet[dutIdx].saData[net-1].waPin[3], 
                        nManualR1[j][net]  );///sylee150722-2

                dMin[net]=nManualR1[j][net];
                dMax[net]=nManualR1[j][net]; 

            }

            //son 나머지  R 값을 줄을 바꾸지 않고 쭉 출력한다. 
            else 
            {
                fprintf(fp, "     %10.2f,   ",   nManualR1[j][net]);  

                if( nManualR1[j][net]  < dMin[net]){
                    dMin[net]=nManualR1[j][net];
                }
                if( nManualR1[j][net]  > dMax[net]){
                    dMax[net]=nManualR1[j][net];
                }

            }
            dAvg[net]=dAvg[net]+nManualR1[j][net];

        }
        //son 출력단계에서 R 값 avg, min, max를 구한다. 
        if ((nManualCount100-1) > 0 )   //son210928
            dAvg[net]=dAvg[net]/(double)(nManualCount100-1);  //sylee20120202   


        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        // 치  ,count

        double dSqureSum1,dSigma1,dOldAvg1;
 
        dSqureSum1=0.0; dSigma1=0.0;  dOldAvg1=0.0;

        nManualR1[0][net]=0;
        dOldAvg1=dAvg[net];
        dAvg[net]=0;

        //----------------------------------------------------------------------
        //son R 값 중에서 튀는 값을 제외하고 avg, min, max를 다시 계산한다. 
        for( j=1; j<nTotal; j++)
        {
            //son R 값이 200000 mohm 초과면 제외
            if(nManualR1[j][net]> MAX_4W_R){  //sylee160601  //sylee180222-1 
                continue;
            }

            //son R 값이 50 mohm 미만이고. ?   500 mohm 초과면 제외???  (이건 Or 연산해야 하지 않을까?????)
            //    이 상태 그대로이면 continue 로 걸리는 코드가 전혀 없는 의미없는 코드가 됨.
            if(nManualR1[j][net]<50){
                if(nManualR1[j][net]>(500)){
                    continue;
                }
            }
            //son R 값이 50 mohm 이상이고 Avg의 5배를 넘는다면 제외
            else {
                if(nManualR1[j][net]>(dOldAvg1*5)){
                    continue;
                }
            }

            nManualR1[0][net]=nManualR1[0][net]+1;

            if(nManualR1[0][net]==1){                       
                dMin[net]=nManualR1[j][net];
                dMax[net]=nManualR1[j][net]; 
            }
            else {
                if( nManualR1[j][net]  < dMin[net]){
                    dMin[net]=nManualR1[j][net];
                }
                if( nManualR1[j][net]  > dMax[net]){
                    dMax[net]=nManualR1[j][net];
                }                           
            }                       

            dAvg[net]=dAvg[net]+nManualR1[j][net];

        }//end of for( j=1; j<nTotal; j++){


        //son 측정값 개수가 3개 이상이면 최대와 최소를 제외하고 nAvg[net]을 다시 계산한다. 
        if(nManualR1[0][net]>=3){//sylee150721
            if(nManualR1[0][net]>0){//sylee150721
                dAvg[net]=(dAvg[net]-dMax[net]-dMin[net])/(double)(nManualR1[0][net]-2);//sylee150721
            }
            else {
                dAvg[net]=0.0;//sylee150721
            }
        }
        //son 측정값 개수가 3개 미만이면 nAvg[net]를 측정값 개수로 그냥 나누어서 계산한다.
        else
        {
            if(nManualR1[0][net]>0){
                dAvg[net]=dAvg[net]/(double)(nManualR1[0][net]);                                 
            }
            else {
                dAvg[net]=0.0;
            }
        }

        //--------------------------------
        //son  'Sigma 계수' Enable이면

        for( j=1; j<nTotal; j++)
        {
            if(nManualR1[j][net]> MAX_4W_R){  //sylee180222
                continue;
            }
            //son R 값이 50 mohm 미만이고. ?   500 mohm 초과면 제외???  (이건 Or 연산해야 하지 않을까?????)
            //    이 상태 그대로이면 continue 로 걸리는 코드가 전혀 없는 의미없는 코드가 됨.
            if(nManualR1[j][net]<50){
                if(nManualR1[j][net]>(500)){
                    continue;
                }
            }
            //son R 값이 50 mohm 이상이고 Avg의 5배를 넘는다면 제외
            else {
                if(nManualR1[j][net]>(dOldAvg1*5)){
                    continue;
                }
            }                       
            //son 'Sigma 계수' Enable이면  Avg와의 편차제곱의 합 를 구한다.
            if(m_check1.GetCheck()){//3챰琉 
                dSqureSum1=dSqureSum1+((dAvg[net]-nManualR1[j][net])*(dAvg[net]-nManualR1[j][net]));  //剋                       
            }

        }//end of for( j=1; j<nTotal; j++){

        //son 'Sigma 계수' Enable이면
        if(m_check1.GetCheck()) //3챰琉  
        {
            if(nManualR1[0][net]>0)
            {
                //son 편차 제곱의 합을 이용해서 표준편차(dSigma1)를 구한다.
                //    ??? 순서 이상하다.  보통은  Avg로 먼저 나누고 그 다음에 루트를 씌우는데 반대로 하고 있음. 
                //dSqureSum1=dSqureSum1/nManualR1[0][net];
                dSigma1= pow(dSqureSum1, 1.0/2.0 ); 
                dSigma1= dSigma1/nManualR1[0][net]; 
            }
            else{
                dSqureSum1=0.0;//error
                dSigma1=0.0;
            }

            //son 평균값을  3*sigma를 더해서 보정한다.
            dAvg[net]=dAvg[net]+3*dSigma1; //+ 3챰琉        //son ??? 3 시그마만 하는 이유??? 버그??? 미수정???

        }
        else{//3챰琉 틈璣 

        }//endof //3챰琉



        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
        //------------------------------------------
        //son Min, Avg, Max, Diff, DiffRate 출력
        dDiff[net]=dMax[net]-dMin[net];
        dDiffRate[net]=dDiff[net]/dAvg[net]*100.0;

        fprintf(fp, "             %10.2f,  %10.2f,  %10.2f,          %10.2f,  %10.2f,   ", 
                                dMin[net] , dAvg[net],dMax[net],   dDiff[net],   dDiffRate[net] );  
        fprintf(fp, "  \n ");
 
        //===================================================================================
        //화면 DISPLAY
        //===================================================================================
 
        //son Min, Avg, Max  Grid 화면출력
        str.Format(" %10.2f", dMin[net]);  
        m_grid1.SetTextMatrix(net, 1+_4W_TEST_MIN, str);    //son190704-4WDUT16
        str.Format(" %10.2f", dAvg[net]);  
        m_grid1.SetTextMatrix(net, 1+_4W_TEST_AVG, str);
        str.Format(" %10.2f", dMax[net]);  
        m_grid1.SetTextMatrix(net, 1+_4W_TEST_MAX, str); 

        //son 현재 avg에 맞는 Mode값 Grid 화면 출력
        for( int k1=1 ; k1<=5; k1++){//sylee170314-2
            if(dAvg[net]<dRec4w[k1][3]){
                str.Format(" %d", k1+170);  
                m_grid1.SetTextMatrix(  net, 1+_4W_MODE, str);
                break; 
            } 
        }                    

        //son '_4W_DATA1 ~ _4W_DATA10' 까지 grid에 화면 출력 
        if(nTotal<=10)//sylee141210
        {
    #if 1
            for(int k=13; k<=11+nTotal ; k++){      //son 버그??? k<=12+nTotal 아닌지?  nTotal 10개면 1~9까지만 찍음. 10은 안찍음
                //str.Format("          ");
                str.Format(" %10.2f", nManualR1[k-12][net]); 
                m_grid1.SetTextMatrix( net, k, str);
            }   

    #else
            for(int data=1; data <= nTotal ; data++){       
                //str.Format("          ");
                str.Format(" %10.2f", nManualR1[data][net]); 
                m_grid1.SetTextMatrix( net, data +_4W_DATA1, str);      // _4W_DATA1 =12
            }   
    #endif
        }

    }

    //===================================================================================

    fprintf(fp, "  \n ");
    fclose(fp); 


    //son CsvFileUse 라면
    if( SysSet13.m_nSet7==1 ){//sylee130724
        ::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);     
    }
    else{                       
        ::ShellExecute(NULL,"open","notepad.EXE",fName1,"NULL",SW_SHOWNORMAL);                 
    }



 
    //################################################################################
  
    //son  BdlPath\4W_Setup.txt  에 출력한 R 값을 d4W_Log2[dutIdx]에도 역반영한다. 
    //     Auto Test 결과에도 반영을 해서 Data Sync를 맞추기 위한 것으로 보인다.
    nEnd=(int)g_s4WNet[dutIdx].wCount;  //son190704-4WDUT16

    for( net=1; net<=nEnd; net++){//sylee170125     //son190704-4WDUT16
        for( j=1; j<nTotal; j++){
            d4W_Log2[dutIdx][j][net]= nManualR1[j][net];    
        }        
    }
    n4w_Sample_SetCount=nTotal-1;
    str.Format("%d", n4w_Sample_SetCount);//son200826 : 변경된 n4w_Sample_SetCount 값을 UI에도 반영.
    m_edit7.SetWindowText(str);           //son200826
    UpdateData(FALSE);                    //son200826


    //==================================================================
    //son  4W_Setup.txt와 동일한 내용으로  4W_Setup_A.txt을 다시 출력

    double dDiffRate2[MAX_4W_NET +1];				//son231107 Def_4WMax1 -> MAX_4W_NET +1
    float  dSigmaVar1[MAX_4W_NET +1];//sylee170125	//son231107 Def_4WMax1 -> MAX_4W_NET +1
    float  dSum1; //sylee170125
    int nCount1;//sylee170125



    ::ZeroMemory(&dSigmaVar1, sizeof(dSigmaVar1));//sylee170125 
    ::ZeroMemory(&dMin, sizeof(dMin)); 
    ::ZeroMemory(&dMax, sizeof(dMax));
    ::ZeroMemory(&dAvg, sizeof(dAvg));
    ::ZeroMemory(&dDiff, sizeof(dDiff));
    ::ZeroMemory(&dDiffRate, sizeof(dDiffRate));
    ::ZeroMemory(&dDiffRate2, sizeof(dDiffRate2));
    ::ZeroMemory(&fName1, sizeof(fName1));//SYLEE150806
   
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE150806
    strcat( fName1 ,str);//SYLEE150806

    //son200320-4WDUT16: Multi DUT 구조로 수정.
    if (dutIdx == 0)
        str.Format("\\4W_Setup_A.txt" ); //SYLEE150806
    else    
        str.Format("\\4W_Setup_A.txt%d", dutIdx+1 ); 
    strcat( fName1 , str ); 

 
    fp = fopen(fName1,"wt"); 
    if(fp == NULL){ 
        str.Format("Error    %s file open   ?  Error, Saving Error !     file close!   ", fName1);
        AfxMessageBox( str, MB_OK);  
        return  ;
    }

    nEnd=(int)g_s4WNet[dutIdx].wCount;  //son190704-4WDUT16
    fprintf(fp, "  %d,    %d, \n",n4w_Sample_SetCount,nEnd );//sylee1507=806     



    for( int i=1; i<=n4w_Sample_SetCount; i++)
    {
        if(i==1){
            fprintf(fp,  " NetNo.,  *  ,Pin1 , Pin2 , Pin3 , Pin4 ,   " );           
        }

        fprintf(fp,  "         #%d    ,   ", i);           

    }

    fprintf(fp, "             MIN.    ,       AVE. ,       MAX. ,           Dif.Value    ,   Dif.Rate%, ((Max.-MIn.)/2)/Avg. ,Sigma.1");



    fprintf(fp, "  \n ");
    nEnd=(int)g_s4WNet[dutIdx].wCount;      //son190704-4WDUT16

    for( net=1; net<=nEnd; net++)           //son190704-4WDUT16
    {

        for( int j=1; j<=n4w_Sample_SetCount; j++)
        {
            if(j==1)
            { 
                //fprintf(fp, "%d, ", net  );///sylee150722-2

                fprintf(fp, " Net%d,  PIN,   %d,   %d,   %d,   %d,      ", 
                        net, g_s4WNet[dutIdx].saData[net-1].waPin[0],       //son190704-4WDUT16
                             g_s4WNet[dutIdx].saData[net-1].waPin[1],
                             g_s4WNet[dutIdx].saData[net-1].waPin[2],
                             g_s4WNet[dutIdx].saData[net-1].waPin[3]  );///sylee150722-2
                dMin[net]= d4W_Log2[dutIdx][j][net];
                dMax[net]= d4W_Log2[dutIdx][j][net]; 
            } 

            if(d4W_Log2[dutIdx][j][net]>0){
                fprintf(fp, "     %10.2f,   ",   d4W_Log2[dutIdx][j][net]); 
            }
            else{
                fprintf(fp, "     999999,  " );
            }


            if(j!=1){ 
                if( d4W_Log2[dutIdx][j][net] < dMin[net]){
                    dMin[net]=d4W_Log2[dutIdx][j][net];
                }
                if( d4W_Log2[dutIdx][j][net] > dMax[net]){
                    dMax[net]=d4W_Log2[dutIdx][j][net];
                }                           
            }
            dAvg[net]=dAvg[net]+d4W_Log2[dutIdx][j][net];


        }
        dAvg[net]=dAvg[net]/n4w_Sample_SetCount;
        dDiff[net]=dMax[net]-dMin[net];
        dDiffRate[net]=dDiff[net]/dAvg[net]*100.0;
        dDiffRate2[net]=(dDiff[net]/2.)/dAvg[net];
        fprintf(fp, "             %10.2f,  %10.2f,  %10.2f,          %10.2f,  %10.2f,%10.2f,    ", 
                    dMin[net] , dAvg[net],dMax[net],  dDiff[net], dDiffRate[net], dDiffRate2[net] );  


        //===========================


        //SIGMA
        //   float  dSigmaVar1[5001];//sylee170125
        //   float  dSum1; //sylee170125
        //   ::ZeroMemory(&dSigmaVar1, sizeof(dSigmaVar1));//sylee170125

        //      for( int i3=1; i3<=nEnd ;i3++){
        // dSum1=0;//SIGMA
        // nCount1=0; //SIGMA
        for(int  k=1; k<=n4w_Sample_SetCount; k++)  //sylee170125
        {
            dSum1=0;//SIGMA //sylee170125
            nCount1=0; //SIGMA //sylee170125

            if( d4W_Log2[dutIdx][k][net]< dMin[net]){ 
                continue;
            }
            if( d4W_Log2[dutIdx][k][net]> dMax[net]){ 
                continue;
            }
            nCount1++; 
            dSum1=dSum1+(float)(d4W_Log2[dutIdx][k][net]-dAvg[net])*(float)(d4W_Log2[dutIdx][k][net]-dAvg[net]); 
            //}

            if (nCount1 > 0)  {  //son210928
                dSum1=dSum1/nCount1;  
                dSigmaVar1[net]=(float)sqrt(dSum1);          
            }
        } //sylee170125

        fprintf(fp, "   %10.2f,    ", dSigmaVar1[net]  );  //sylee170125
    
        fprintf(fp, "  \n ");       

    }
    fprintf(fp, "  \n ");
    fclose(fp); 



    return ;

}





void CModeSub17::A_Language()  //LANGUAGE CHANGE
{
    if( gDef_Language1==3)
    { 
        m_Label1001.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 101)); 
        m_button1001.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 105));
        m_button1004.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 113));
        m_button1005.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 114));
        m_button1006.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 115));
        m_button1007.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 116)); 
        m_button1008.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 117));
        m_button1010.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 119)); 
        m_button1011.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 120)); 
        m_button1013.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 122));
        m_button1014.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 124)); 
        m_button1015.SetWindowText(LoadMessage(1,"TitleSetup4WTest", 126));

    }

}


//son 'Test->Ref.Avg' button
//son _4W_TEST 값을 _4W_SET 값으로 반영
void CModeSub17::OnButton_Copy_TestDatato_RefAvg()  //  copy
{ 
    CString   str;
    int net;
    for( net=1; net<=g_nGridNetCnt; net++)          //son211015
    { 
        //son211015 이 부분 이상함.  MIN값을 왜 Ref.Low로 사용해야 하는지 
        str= m_grid1.GetTextMatrix(net, 1+_4W_TEST_MIN);    //son190704-4WDUT16
        m_grid1.SetTextMatrix(net, 1+_4W_REF_LOW, str);     //son211005
        fCalc[net][CALC_REF_LOW] = atof(str);                 //son211015 Ref.Low Raw Data 저장기능 추가

        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str= m_grid1.GetTextMatrix(net, 1+_4W_TEST_AVG); 
        m_grid1.SetTextMatrix(net, 1+_4W_REF_AVG, str);     //son211005
        fCalc[net][CALC_AVG] = atof(str);                     //son211015 Ref.Avg Raw Data 저장기능 추가

        //son211015 이 부분 이상함.  MAX값을 왜 Ref.High로 사용해야 하는지 
        str= m_grid1.GetTextMatrix(net, 1+_4W_TEST_MAX); 
        m_grid1.SetTextMatrix(net, 1+_4W_REF_HIGH, str);     //son211005
        fCalc[net][CALC_REF_HIGH] = atof(str);                 //son211015 Ref.High Raw Data 저장기능 추가
    }
}




//son 'Ref.ReLoad' button //"4W.ini" 파일을 Grid에 Load 

// Grid 화면에 4W.ini 파일의 내용을 display한다.
//extern int g_nPinPieceNo[DefDutMax][DEF_MAX_PIN1];  //son200804: 4W Piece 정보 추가
extern int g_nPinPieceNo[MAX_DUT1][DEF_MAX_PIN1];     //son220824_2 DefDutMax(33) -> MAX_DUT1(33)
void CModeSub17::OnButton_Load()  //reset
{
    CString str;

    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16

//son220419_7   OnButton_Load() 호출마다 Init_grid(dutIdx)하면 시간 오래걸리는 문제 관련해서 삭제.
//              OnButton_Load() 직전에 필요시에만  Init_grid(dutIdx) 하도록 수정함.
//    //--------------------------------
//    //son Init Grid & DUT NetCount
//    
//
//    //son m_grid와 Dut NetCount의 상태를 현재 dutIdx에 맞게 설정한다.
//    //    FileSysInfo01.LoadSaveSub17(_LOAD)를  Init_grid(dutIdx)안에서 수행한다.   //son210623
//    int ret = Init_grid(dutIdx);    //son210524 DUT 변경시에 "subscript out of range" 발생문제 수정. GTS보고사항.
//    if( ret <= 0 )
//        return;




    //-------------------------------------------------------------
    //son 새로운 dutIdx에 맞는 4W.ini 내용을  Grid에 출력한다.
    
    //nEnd=(int)SysInfoSer13.m_nData[mode][0][0];
    int nEnd=(int)g_s4WNet[dutIdx].wCount;      //son Init_grid()->FileSysInfo01.LoadSaveSub17(_LOAD)에서 획득
    if(nEnd < 0)  //son241009 0도 처리되게 수정
    {             
        str.Format("Error No 1723,\n  sub17  4W SETUP      Netlist Num(=%d) < 0.    Min. under.   check !  BDL\\4W.INI FILE HEADER \n ", nEnd); 
        AfxMessageBox(str);           
        return;
    }
    //sylee170728-1             if(nEnd>4096){
    if(nEnd > MAX_4W_NET)      //son-4WDUT-20200403: 5000-> MAX_4W_NET
    {   //sylee170728-1  
        str.Format("Error No 1723,\n  sub17  4W SETUP      Netlist Num(=%d)> %d.  Max over. check !  BDL\\4W.INI FILE HEADER \n ", 
                                nEnd, MAX_4W_NET); 
        AfxMessageBox(str);
        return;  ///max
    }

    //son 여기서 4W_Setup_A.txt를 Grid 로딩하면 안 된다.  4W.ini 내용으로만  먼저 Grid에 로딩한다. 
    //  OnButton_Load_Auto4WData();

#ifdef __GTS_4W_SPCIALNET__
    int netSpecialCnt = 0;      //son221111
#endif

    //son matrix row 0, col 0은 헤더이므로 제외하고 row는 1부터 , col은 +1하여 출력한다.
    //son190704-4WDUT16: _4W_PIN_NUM, _4W_REF_AVG 등 enum 사용하도록  변경
    for (int net=1; net <= nEnd; net++) //son190704-4WDUT16
    {        
//son221111 begin:
#ifdef __GTS_4W_SPCIALNET__     
        //son Load from 4w_v2.ini
        str.Format("%d" , g_s4WNet[dutIdx].saData[net-1].bNetSpecial);  //son221107 
        m_grid1.SetTextMatrix(net, 1+_4W_SPECIAL, str);                 

        if (g_s4WNet[dutIdx].saData[net-1].bNetSpecial == 1)    
        {
            netSpecialCnt++;
            SetOn_NetSpecial(net);
        }

        if (g_s4WNet[dutIdx].saData[net-1].bNetSpecial == 0)
            SetOff_NetSpecial(net);
#endif
//son221111 end

        //son200804: 4W Piece 정보 추가  
        //str.Format(" %d", g_s4WNet[dutIdx].saData[net-1].wPiece); //piece     
        int firstPin1  = g_s4WNet[dutIdx].saData[net-1].waPin[0];   //son (1~)
        int piece1     = g_nPinPieceNo[dutIdx+1][firstPin1];        //son 현재 4W Net의 첫번째 pin의 piece정보 (1~)
        str.Format(" %d", piece1);
        m_grid1.SetTextMatrix(net, 1+_4W_PIECE, str);       

        for(int pin = 0; pin < _4W_PIN_NUM; pin++)
        {
            if (g_bUseLowerCalJig_4W) 
                str.Format(" %d", g_s4WNet[dutIdx].saData[net-1].waPin[pin] + (SysSet12.m_nLowerJigStartPin -1));   //son220419_7
            else
                str.Format(" %d", g_s4WNet[dutIdx].saData[net-1].waPin[pin]);   //pin   
            m_grid1.SetTextMatrix(net, 1+_4W_PIN1 +pin, str);
        }

        str.Format(" %3.3f", (float)g_s4WNet[dutIdx].saData[net-1].dRefLow);     //Ref Low //son211005 //son211121 %.2f->%3.3f
        m_grid1.SetTextMatrix(net, 1+_4W_REF_LOW, str);     //son211005

        str.Format(" %.2f", (float)g_s4WNet[dutIdx].saData[net-1].dRefAvg);     //Ref Avg //son211005
        m_grid1.SetTextMatrix(net, 1+_4W_REF_AVG, str);     //son211005

        str.Format(" %3.3f", (float)g_s4WNet[dutIdx].saData[net-1].dRefHigh);    //Ref High  //son211005 //son211121 %.2f->%3.3f
        m_grid1.SetTextMatrix(net, 1+_4W_REF_HIGH, str);     //son211005

        str.Format(" %d", g_s4WNet[dutIdx].saData[net-1].n4wMode);
        m_grid1.SetTextMatrix(net, 1+_4W_MODE, str);
    }

//son221111 begin:
#ifdef __GTS_4W_SPCIALNET__
    if (netSpecialCnt ==  g_s4WNet[dutIdx].wCount)     
        m_check_AllSpecial.SetCheck(1);
    else
        m_check_AllSpecial.SetCheck(0);
#endif
//son221111 end
   

    UpdateData(FALSE);

}




//son 'View File: 4W Ref.' button //"4W.ini" 파일을 Open
void CModeSub17::OnButton_ViewFile_4wRef() //4w setup view list
{ 

    CString Str;
    ///SYLEE130428 
    Str.Format("%s\\4W.INI",(SysInfoView01).m_pStrFileDri1);//sylee141212
    ////sylee141212    Str.Format("C:\\ACE300\\BDL\\4W.INI"); 

    if( SysSet13.m_nSet7==1 ){//sylee130724
        ::ShellExecute(NULL,"open","EXCEL.EXE",Str,"NULL",SW_SHOWNORMAL);    
    }
    else {                      
        ::ShellExecute(NULL,"open","notepad.EXE",Str,"NULL",SW_SHOWNORMAL);                    
    }

    //sylee ReadFile1(3);//TESTING               
}

 
//--------------------------------------------------------------------
//son "4W_Setup_A.txt" 파일 보기.   4W Auto Test결과 파일을 Open한다. 

//son 'View File :  Auto 4w Data' button
void CModeSub17::OnButton_ViewFile_Auto4wData() 
{ 

    CString Str;

    //son200320-4WDUT16: Multi DUT 구조로 수정.
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  
    if (dutIdx == 0)
        Str.Format("%s\\4W_Setup_A.txt",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    else    
        Str.Format("%s\\4W_Setup_A.txt%d",(SysInfoView01).m_pStrFilePathBDL, dutIdx+1 );//sylee141212


    if( SysSet13.m_nSet7==1 ){//sylee130724
        ::ShellExecute(NULL,"open","EXCEl.EXE",Str,"NULL",SW_SHOWNORMAL);    
    }
    else {                      
        ::ShellExecute(NULL,"open","notepad.EXE",Str,"NULL",SW_SHOWNORMAL);                    
    }

    //     ::ShellExecute(NULL,"open","EXCEl.EXE",Str,"NULL",SW_SHOWNORMAL);

}


//son ModeSub18 SubDialog High Range(상한치 Range)를 ModeSub17의 Grid에 적용한다.
//   SubDialog의 High Range 설정상황은  "4W_Per_High.ini"에서 로딩한다.
void CModeSub17::OnButton_SetLimitHigh()   // Limit High + set    
{                                           //son220308 OnButton117 -> OnButton_SetLimitHigh

    double  dt2,dHighPer1, dOffset ; 
    CString str;
    int k,oldVal,newVal, valChanged;

    //son "4W_Per_High.ini" 파일을 gd4W1_P2High[][][]에 load한다.
    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load   22   //son211015 22 -> _LOADHIGH

    //------------------------------------------------
    //son 'Average % Use' 가 새로 체크되었는지 확인
    valChanged=0;
    oldVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_USE];     //son210702 //son210916
    if(m_check6.GetCheck()){    //son 'Average % Use' On  
        newVal=1;        
    }
    else {
        newVal=0;        
    }

    if(oldVal!=newVal)
    {
        if(m_check6.GetCheck()){    //son 'Average % Use' On
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;               //son210702 //son210916
        }
        else {
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
        }
        valChanged=1;
    }


    //------------------------------------------------
    //son 'Average % Value' 가 변경되었는지 확인 
    oldVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];   //son210702   //son210916

    m_edit6.GetWindowText( szText,10);  //son Average % value   
    k  = atoi( szText );
    if(k<0)     k=0;
    if(k>500)   k=500;
    newVal=k;

    //son 'Average % Use'의 화면상태와 파일설정값이 다르면 파일값을 화면상태에 맞춘다.
    if(oldVal!=newVal)
    {
        m_edit6.GetWindowText( szText,10);//son Average % value 
        k  = atoi( szText );
        if(k<0)     k=0;
        if(k>500)   k=500;
        
        gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=k;   //son210702 //son210916
        valChanged=1;
    }

    //----------------------------------------------------------------------------------------------
    //son 'Average % Use' 또는 'Average % Value'가 바뀐경우 "4W_Per_High.ini" 파일에 업데이트한다. 
    //     gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL] 변경사항을 "4W_Per_High.ini" 파일에 반영.
    if(valChanged==1)
        FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21    //son211015  21-> _SAVEHIGH
    
    int net;

#if 0
    //son211005 Sigma는 AutoSample Data로딩할 때  Sigma Sampling이나 AvgPercent Sampling 후에 결정된
    //   fCalc[net][CALC_SIGMA]를 사용하기로 한다.
    //
    //   여기서 이렇게 Sigma를 또 계산하는 것은 부정확하므로 삭제함.  Min보다 작거나 Max보다 크다고 sample제외하면 안됨 
    //   AvgPercent Sampling에서는  Average%로 계산한 dAvgLsl, dAvgUsl로 sample 선택하고 있다. (Min, Max 아님)
    //   SigmaSampling에서도 Sigma계수와 Eject LowCount, HighCount로 결정된 dLsl, dUsl로 sample 선택하고 있음. 
    
    //######################################################################
    //SIGMA
    
    int   nCount1 ;     
    float  dSigmaVar1[MAX_4W_NET +1];	//son231107 Def_4WMax1 -> MAX_4W_NET +1
    float  dSum1; 
    ::ZeroMemory(&dSigmaVar1, sizeof(dSigmaVar1));

    for (net=1; net<=nTotalNet1 ;net++)     //son190704-4WDUT16
    {
        dSum1=0;
        nCount1=0;//sylee160604-1
        for(int  k=1; k<=nTotalSampCount1; k++){ 
            if( fData[net-1][k]< fCalc[net][CALC_MIN]){//sylee160604-1
                continue;
            }
            if( fData[net-1][k]> fCalc[net][CALC_MAX]){//sylee160604-1?????
                continue;
            }
            nCount1++;//sylee160604-1
            //son 편차의 제곱을 더해 간다.
            dSum1=dSum1+(float)(fData[net-1][k]-fCalc[net][CALC_AVG])*(float)(fData[net-1][k]-fCalc[net][CALC_AVG]); 
        }

        if (nCount1 > 0)  {  //son210928
            dSum1=dSum1/nCount1; //sylee160604-1  nTotalSampCount1-> nCount1 //son 편차의 평균으로 분산을 구한다.
            dSigmaVar1[net]=(float)sqrt(dSum1);     //son 분산을 제곱근해서 표준편차(Sigma)를 구한다. 

            if(dSigmaVar1[net]<0.0)     
                dSigmaVar1[net]=0.0;    //sylee160609   //son190704-4WDUT16
        }
    }

#endif

    //######################################################################

    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    for (net=1; net<=g_s4WNet[dutIdx].wCount; net++)                //son190704-4WDUT16
    {
        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str=m_grid1.GetTextMatrix(net, 1+_4W_REF_AVG);              //son211005
        dt2=atof(str);  //son avg
        //dt2 = fCalc[net][CALC_AVG];       //son211015 Grid 출력값은 %.2f로 잘린 값이므로 왜곡이 발생하여 원본인 fCalc[net][CALC_AVG]로 교체 
        //                                //son211121 원상복구. edit 한 이후에 변경된 값이 save시  반영이 안되는 문제 발생때문에 원복. 대신 %.3f로 수정.

        ///============================================================
        for (int j=1; j<= MAX_LIMIT4W_RNG; j++)   //son ModeSub18 High Range 개수 20개 //son211015 20 -> MAX_LIMIT4W_RNG
        {        
            if(gd4W1_P2High[j][LIMIT4W_R_END]<0)
                gd4W1_P2High[j][LIMIT4W_R_END]=0; 

            if(gd4W1_P2High[j][LIMIT4W_R_END]>= MAX_4W_R) 
                gd4W1_P2High[j][LIMIT4W_R_END]= MAX_4W_R;  //sylee180222

            //son 현재 net avg가  j의 Range에 포함이 된다면
            if(dt2<= gd4W1_P2High[j][LIMIT4W_R_END])
            {
                if(gd4W1_P2High[j][LIMIT4W_PERCENT]<0)         gd4W1_P2High[j][LIMIT4W_PERCENT]=0; 
                if(gd4W1_P2High[j][LIMIT4W_PERCENT]>500)       gd4W1_P2High[j][LIMIT4W_PERCENT]=500; 
                if(gd4W1_P2High[j][LIMIT4W_OFFSET]<0)          gd4W1_P2High[j][LIMIT4W_OFFSET]=0;
                if(gd4W1_P2High[j][LIMIT4W_OFFSET]> MAX_4W_R)  gd4W1_P2High[j][LIMIT4W_OFFSET]=MAX_4W_R;    //sylee180222

                dHighPer1 = gd4W1_P2High[j][LIMIT4W_PERCENT];
                dOffset   = gd4W1_P2High[j][LIMIT4W_OFFSET];

                //son High data Sigma RangeCheck
                if(gd4W1_P2High[j][LIMIT4W_SIGMA_COEF]<0)       gd4W1_P2High[j][LIMIT4W_SIGMA_COEF]=0; 
                if(gd4W1_P2High[j][LIMIT4W_SIGMA_COEF]>6)       gd4W1_P2High[j][LIMIT4W_SIGMA_COEF]=6; //son Sigma계수 Max는 6

                //---------------------------------------------------------------------------------------------------------------------
                //son LimitHigh = (avg[net] + (avg[net]/100.0 * dHighPer1)+ sigma[net]*SigmaCoef + HighOffset) * ModeSub18::HighAll%/100.0
                //---------------------------------------------------------------------------------------------------------------------
                
                dt2 = dt2 + ((dt2/100.0)*dHighPer1);                //sylee171101-2   //STEP#1  +% 

                //dt2 = dt2 + (dSigmaVar1[net]* gd4W1_P2High[j][LIMIT4W_SIGMA_COEF]);   ///STEP#2 +SIGMA    
                dt2 = dt2 + (fCalc[net][CALC_SIGMA]* gd4W1_P2High[j][LIMIT4W_SIGMA_COEF]); //son211005 dSigmaVar1[net] -> fCalc[net][CALC_SIGMA]    
                dt2 = dt2 + dOffset;                                    //sylee171101-2 //STEP#3  +R  

                //son  ModeSub18 SubDigalog에서 'High All% Use'  checkBox On이라면 
                if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]==1)   //son210916
                {
                    if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]<0)    
                        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=0;       //son210916

                    if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]>500)  
                        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=500;     //son210916

                    //son 'All % Value'  %값을 반영
                    dt2= dt2 * (gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]/100.);      //son210916
                }
                //----------------------------------------------------------------------
                //########################################################################

                if(dt2<0.0)         dt2=0.0;
                if(dt2> MAX_4W_R)   dt2= MAX_4W_R;  //sylee180222

                str.Format("%3.3f",dt2); //son211015  %10.2f -> %.2f 공간확보를 위해 //son211121 %.2f->%3.3f
                m_grid1.SetTextMatrix( net, 1+_4W_REF_HIGH, str);           //son211005
                fCalc[net][CALC_REF_HIGH] = dt2;       //son211015 Ref.High Raw Data 저장기능 추가
                break;

            }

            else 
            {
                if(j==5){
                    str.Format("%3.3f",dt2); //son211015  %10.2f -> %.2f 공간확보를 위해 //son211121 %.2f->%3.3f
                    m_grid1.SetTextMatrix( net, 1+_4W_REF_HIGH, str);       //son211005
                    fCalc[net][CALC_REF_HIGH] = dt2;       //son211015 Ref.High Raw Data 저장기능 추가
                }
            }
        }                           
        //================================================================
    }

    DoEvents(); 

}

void CModeSub17::OnButton_SetLimitLow()   //Limit low  set  ///SYLEE160406-1
{                                //son220308 OnButton118 -> OnButton_SetLimitLow 
 
    double  dt2,dLowPer1,dOffset ; 
    CString str;
 
    //son "4W_Per_Low.ini" 파일을 gd4W1_P1Low[][][]에 load한다.
    FileSysInfo01.LoadSaveSub18(_LOADLOW);//load  12      //son211015 12 -> _LOADLOW

    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    for (int net=1; net<=g_s4WNet[dutIdx].wCount; net++)                //son190704-4WDUT16
    {
        
        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str=m_grid1.GetTextMatrix(net, 1+_4W_REF_AVG);      //son211005
        dt2=atof(str);  //son avg
        //dt2 = fCalc[net][CALC_AVG];       //son211015 Grid 출력값은 %.2f로 잘린 값이므로 왜곡이 발생하여 
        //                                //          원본인 fCalc[net][CALC_AVG]로 교체 //son211121 원상복구

        ///============================================================

        for( int j=1; j<= MAX_LIMIT4W_RNG; j++)   //son ModeSub18 LowRange 개수 20개    //son211012 20 -> MAX_LIMIT4W_RNG
        {        
            if (gd4W1_P1Low[j][LIMIT4W_R_END]<0)           gd4W1_P1Low[j][LIMIT4W_R_END]=0; 
            if (gd4W1_P1Low[j][LIMIT4W_R_END]>= MAX_4W_R)  gd4W1_P1Low[j][LIMIT4W_R_END]= MAX_4W_R;  //sylee180222

            //son 현재 net avg가  j의 Range에 포함이 된다면
            if(dt2<=gd4W1_P1Low[j][LIMIT4W_R_END])
            {
                if (gd4W1_P1Low[j][LIMIT4W_PERCENT]<0)         gd4W1_P1Low[j][LIMIT4W_PERCENT]=0; 
                if (gd4W1_P1Low[j][LIMIT4W_PERCENT]>500)       gd4W1_P1Low[j][LIMIT4W_PERCENT]=500;
                if (gd4W1_P1Low[j][LIMIT4W_OFFSET]<0)          gd4W1_P1Low[j][LIMIT4W_OFFSET]=0;
                if (gd4W1_P1Low[j][LIMIT4W_OFFSET]> MAX_4W_R)  gd4W1_P1Low[j][LIMIT4W_OFFSET]= MAX_4W_R;  //sylee180222

                dLowPer1=gd4W1_P1Low[j][LIMIT4W_PERCENT];
                dOffset=gd4W1_P1Low[j][LIMIT4W_OFFSET];

                //son211012 Low data Sigma RangeCheck
                if(gd4W1_P1Low[j][LIMIT4W_SIGMA_COEF]<0)       gd4W1_P1Low[j][LIMIT4W_SIGMA_COEF]=0; //son211012  Low Grid 에도 SigmaCoef 추가
                if(gd4W1_P1Low[j][LIMIT4W_SIGMA_COEF]>6)       gd4W1_P1Low[j][LIMIT4W_SIGMA_COEF]=6; //son Sigma계수 Max는 6

                //---------------------------------------------------------------------------------------------
                //son LimitLow = avg[net] - (avg[net]/100.0 * dLowPer1) - sigma[net]*SigmaCoef - LowOffset 
                //---------------------------------------------------------------------------------------------
                dt2 = dt2 - ((dt2/100.0)*dLowPer1);                                     //son211012 Step1: -%
                dt2 = dt2 - (fCalc[net][CALC_SIGMA]* gd4W1_P1Low[j][LIMIT4W_SIGMA_COEF]);    //son211012 Step2: -(Sigma * SigmaCoef)
                dt2 = dt2 - dOffset;                                                    //son211012 Step3: -R

#if 0           //son211015 

                //son ?????n 'All % Value'  %값을  Lef.Low 에 반영하기 왜 Low Limit에서는 안하나?  (High Limit만 하고 있음)
                //son211015 Low에도 다음과 같이 추가했다가 삭제함.  특수 기능이므로 가급적 user가 사용하지 않게 하는게 나음. 
                //          대신에 High 전용이므로 UI를 "High All % Use"로 이름을 변경하기로 한다. 
                   
                //son  ModeSub18 SubDigalog에서 'All% Use '  checkBox On이라면 
                if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]==1)   //son211012
                {
                    if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]<0)    
                        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=0;       //son211012

                    if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]>500)  
                        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=500;     //son211012

                    //son 'All % Value'  %값을 반영
                    dt2= dt2 * (gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]/100.);      //son211012
                }
#endif          //son211015


                if(dt2<0.0)         dt2=0.0;
                if(dt2> MAX_4W_R)   dt2= MAX_4W_R;//sylee180222


                str.Format("%3.3f",dt2); //son211012  %10.2f -> %.2f 공간확보를 위해        //son211121 %.2f->%3.3f
                m_grid1.SetTextMatrix(net, 1+_4W_REF_LOW, str);         //son211005
                fCalc[net][CALC_REF_LOW] = dt2;       //son211015 Ref.Low Raw Data 저장기능 추가
                break;
            }
            else{
                if(j==5){
                    str.Format("%3.3f",dt2); //son211012  %10.2f -> %.2f 공간확보를 위해    //son211121 %.2f->%3.3f
                    m_grid1.SetTextMatrix(net, 1+_4W_REF_LOW, str);     //son211005
                    fCalc[net][CALC_REF_LOW] = dt2;       //son211015 Ref.Low Raw Data 저장기능 추가
                }
            }
        }                           
        //================================================================
    }


    DoEvents(); 

}



//son 'Exit' 클릭시
void CModeSub17::OnClickOk() 
{ 
    //-------------------------------------------------------
    //sylee150812-1
    CString str; 
    if(m_check5.GetCheck()){ 
        n4w_Sample_Enable=1; 
    }
    else {
        n4w_Sample_Enable=0; 
    }


    m_edit7.GetWindowText(str); 
    n4w_Sample_SetCount=atoi(str); 
    //sylee170705    if( (n4w_Sample_SetCount<=0) || (n4w_Sample_SetCount>100)){ 
    if( (n4w_Sample_SetCount<=0) || (n4w_Sample_SetCount> MAX_4W_SAMPLE)){ //sylee170705    //son210928 MAX_4W_SAMPLE 310
        n4w_Sample_SetCount=0; 
    } 

    //Sigma 계수     
    if(m_check1.GetCheck())     //son200827-4w_Sigma
        g_bSigmaCoef_Enable=1;   //son200827-4w_Sigma
    else 
        g_bSigmaCoef_Enable=0;   //son200827-4w_Sigma
    
    //son Sigma 계수 value
    m_edit8.GetWindowText(str);     //son200827-4w_Sigma 
    g_dSigmaCoef =atof(str);         //son200827-4w_Sigma
    if( (g_dSigmaCoef<=0) || (g_dSigmaCoef>10)){ //son200827-4w_Sigma
        g_dSigmaCoef=0;              //son200827-4w_Sigma 
    } 

    //son n4w_Sample_SetCount, g_dSigmaCoef 값, Enable 여부를 file에 저장한다.
    LoadSave_SamplingSigmaInput(_SAVE);      //son200724: 4W-SamplingCount 비번입력기능 //son200827-4w_Sigma



    //son200320-4WDUT16:  Multi DUT 구조로 수정
    ::ZeroMemory(&n4w_Sample_RunCount, sizeof(n4w_Sample_RunCount));
    ::ZeroMemory(&n4w_Sample_SheetCount, sizeof(n4w_Sample_SheetCount));  //son211112
    ::ZeroMemory(&g_b4wSampleEnd, sizeof(g_b4wSampleEnd));  //son200320-4WDUT16 신규추가 //son211217_3  삭제 //son220203 원복


    //=====================================================================


#if 0   //son220308
    int temp, oldVal, newVal, valChanged;


    //son "4W_Per_High.ini" 파일을 gd4W1_P2High[][][]에 load한다.
    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load  22 //son211012 22 -> _LOADHIGH

    valChanged=0;
    oldVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_USE];     //son210702   //son210916
    if(m_check6.GetCheck()){        //son 'Average % Use' On
        newVal=1;      
    }
    else {
        newVal=0;      
    }
    if(oldVal!=newVal){
        if(m_check6.GetCheck()){    //son 'Average % Use' On
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;               //son210702 //son210916
        }
        else {
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
        }
        valChanged=1;
    }



    oldVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];   //son210702 //son210916
    m_edit6.GetWindowText( szText,10);      //son Average % value   
    temp  = atoi( szText );
    if(temp<0){
        temp=0;
    }
    if(temp>500){
        temp=500;
    }
    newVal=temp;


    if(oldVal!=newVal){
        m_edit6.GetWindowText( szText,10);  //son Average % value   
        temp  = atoi( szText );
        if(temp<0){
            temp=0;
        }
        if(temp>500){
            temp=500;
        }
        gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=temp;   //son210702 //son210916
        valChanged=1;
    }

    //son "4W_Per_High.ini" 파일에 gd4W1_P2High[][][] 내용을 write
    if(valChanged==1){
        FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21   //son211012  21-> _SAVEHIGH
    }
#else
    //son220308 OK로 나갈때에는 단순히 Save하도록 수정.
    int temp;
    
    //son 'Average % Use' On
    if(m_check6.GetCheck()){                
        gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;               //son210702 //son210916
    }
    else {
        gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
    }


    //son Average % value   
    m_edit6.GetWindowText( szText,10);      
    temp  = atoi( szText );
    if(temp<0){
        temp=0;
    }
    if(temp>500){
        temp=500;
    }
    gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=temp;   //son210702 //son210916
    
    FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21   //son220308

#endif  //son220308

    //===============================================
    Sub71.AOnReadV1Enable1(5);//sylee170417   all off

    //son220419_7 하부핀 R 측정상태 save       
    FileSysInfo01.LoadSave_UseLowerPin(_SAVE, g_bUseLowerCalJig_4W);    //son220331 Cal기능과 같이 쓰기 위해 FileSysInfo의 전역함수로 변경

    CDialog::OnOK();
}

void CModeSub17::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}
 
  
void CModeSub17::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
 // TODO: Add your message handler code here and/or call default

 CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

 
 
 SetAttr(pSlider->GetPos());

 CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


 

void CModeSub17::InitControl()
{

    CString str, str2;

    BeginWaitCursor();          //son200708: 커서를 waiting 모드로 바꾼다.

    if(MAX_4W_SAMPLE< 1000){//sylee170802-1       //son210928 MAX_4W_SAMPLE 1000
        SetDlgItemText(IDC_STATIC1,"Sampling Count(<=300)");
    }
    else {
        SetDlgItemText(IDC_STATIC1,"Sampling Count(<=1000)");
    } 

    nTotalSampCount1=0;
    nTotalNet1=0;//sylee160521  



    k=0;


    //son g_bUseLowerCalJig_4W, m_checkLowerCalJig 초기화
    FileSysInfo01.LoadSave_UseLowerPin(_LOAD, g_bUseLowerCalJig_4W);   //son220419_7 begin 
    if (g_bUseLowerCalJig_4W== 1)         
    {
        if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)
        {
            g_bUseLowerCalJig_4W=0;   // 하부핀 R 모드 off
            m_checkLowerCalJig.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
        else
        {
            //son 4W Setup 창에서는 OPEN-READ.BDL 의 4W.ini가 아니면 하부 CalJig 모드를 on 할 수 없다.
            if (strcmp(SysInfoView01.m_pStrFileBDLName, "OPEN-READ.BDL") != 0)
            {
                g_bUseLowerCalJig_4W=0;   // 하부핀 R 모드 off
                m_checkLowerCalJig.SetCheck(0);
                UpdateData(FALSE);      // UI에 값을 반영한다.

            }
            else
            {
                //son 하부가 없는 PinMode일때에는 하부핀 R 측정모드를 Off해야 한다.
                if (IS_NO_LOWER(SysSet12.m_nPinTotMax)) 
                {   
                    g_bUseLowerCalJig_4W=0;   // 하부핀 R 모드 off
                    FileSysInfo01.LoadSave_UseLowerPin(_SAVE, g_bUseLowerCalJig_4W);    
                    m_checkLowerCalJig.SetCheck(0);
                    UpdateData(FALSE);      // UI에 값을 반영한다.
                }   
                else
                    m_checkLowerCalJig.SetCheck(1);
            }
        }
    }
    else
        m_checkLowerCalJig.SetCheck(0);
    //son220419_7 end 


    //--------------------------------
    //son Init DUT NetCount
    
    int dutId  = getDutId();        // Dut Id, 1부터 시작함.    //son-4WDUT16-20200318
    int dutIdx = dutId -1; 
    

    //son g_s4WNet[dutIdx]에  BDLPath\4W.ini 파일(Pin, Ref.High, Ref, Ref.Low)정보를  로딩
	                //son220209_2 중복된 Init_grid(dutIdx)는 시간을 줄이기 위해 삭제하고 대신에 LoadSaveSub17(_LOAD)를 추가.
    g_nGridNetCnt=100;  //son210623
    int ret = FileSysInfo01.LoadSaveSub17(_LOAD);// type 1,2 ,  mode 1~20  //file loading
    if (ret > 0)
    {
        int net=(int)g_s4WNet[dutIdx].wCount;
        if(net<=MAX_4W_NET){
            g_nGridNetCnt=net;//sylee200604 //son210623
            if(net<20)
                g_nGridNetCnt=20;   //son210623
        }
        else{
            g_nGridNetCnt=MAX_4W_NET;   //son210623
        }
    }
    

    //-----------------------------
    //son Init DUT ComboBox
    m_comboDut.ResetContent();
    for (int dut=0; dut < nMultyNetTotal; dut++)    //son200708: combo 박스에 dutId list 추가 
    {
        str.Format("%d", dut+1);
        m_comboDut.InsertString(-1, str);
    }
    m_comboDut.SetCurSel(dutIdx);   //son 현재 dutId로 comboBox를 초기화한다.
                                    //    dutId는 1부터, duxIdx는 0부터 시작 


    //-----------------------------------------------------------------------------------
    //son 현재 dutIdx에 맞는 기존 4W AutoSampling Test 결과(4W_Setup_A.txt)를 메모리에 로딩하고 
    //    sigma 도 다시 계산한다.  Grid 출력까지 하지는 않는다.
    //son211202_3 Load_Auto4WData(dutIdx);  //son210524   //son210623
    _Load_Auto4WData(dutIdx);  //son211202_3 Load_Auto4WData() 함수 삭제하고 _Load_Auto4WData()로 기능 통합. Sigma 기능 추가




    //-------------------------------------------------
    //son 4W.ini Reference Load  => 여기에서 Grid 출력
    
    //son Init Grid & DUT NetCount
    //son m_grid와 Dut NetCount의 상태를 현재 dutIdx에 맞게 설정한다.
    //    FileSysInfo01.LoadSaveSub17(_LOAD)를  Init_grid(dutIdx)안에서 수행한다.   //son210623
    Init_grid(dutIdx);    //son210524 DUT 변경시에 "subscript out of range" 발생문제 수정. GTS보고사항.

    OnButton_Load();    //son 4W.ini를 Grid에 로딩한다.
    nAutoProcess1=0;

    str.Format("0");
    m_Label101.SetCaption(str); 


    //---------------------------------
    //son 기타 4W 관련 File data 로딩
    
    //son "C:\\ACE400\\SETUP\\SysSet15.ini"에서 SelfTest Setting 관련 값을 SysSet15에 로딩
    FileSysInfo01.LoadSaveSet15(_LOAD);//sylee121115-1  //son211015 2 -> _LOAD

 
    //son n4w_Sample_SetCount 값을 file에서 가져온다.
                //    son200827 n4w_Sample_SetCount 로딩 및 체크기능을 OnInitDialog()에서 이동해 옴. 
                //    InitControl()내의 중복된 부분은 삭제함.
    LoadSave_SamplingSigmaInput(_LOAD);              //son200724-4W-SamplingCount: 신규추가  //son200827-4w_Sigma

    // n4w_Sample_SetCount 값을 컨트롤에 반영
    if( (n4w_Sample_SetCount<=0) || (n4w_Sample_SetCount > MAX_4W_SAMPLE)){ //son210928
       n4w_Sample_SetCount=0; 
    } 


    //---------------------------------
    //son 각종 Control UI 설정 
        
    str.Format("%d", n4w_Sample_SetCount);//sylee150812-1 
    m_edit7.SetWindowText(str);//sylee150812-1 


    // n4w_Sample_Enable값을 컨트롤에 반영
    if(n4w_Sample_Enable==1){   //sylee150812-1      
        m_check5.SetCheck(1);//sylee150812-1     
    }
    else {//sylee150812-1     
        n4w_Sample_Enable=0; //sylee150812-1     
        m_check5.SetCheck(0);//sylee150812-1     
    }//sylee150812-1   


    //son200827: 4W Sigma 설정유지
    //son  g_dSigmaCoef 값을 컨트롤에 반영
    if( (g_dSigmaCoef<=0) || (g_dSigmaCoef>10)){  //son200827-4w_Sigma
        g_dSigmaCoef=0;                          //son200827-4w_Sigma 
    } 
    str.Format("%.1f", g_dSigmaCoef);            //son200827-4w_Sigma 
    m_edit8.SetWindowText(str);     //son Sigma 계수 value       //son200827

    //son g_bSigmaCoef_Enable값을 컨트롤에 반영
    if(g_bSigmaCoef_Enable==1){  //son200827-4w_Sigma
        m_check1.SetCheck(1);   //son200827-4w_Sigma

#ifndef __MST__CUSTOMER //son220308 MST는 Sigma 계수필터와 Average % 필터를 동시 적용 요청
        //son Sigma Coef' Enable ON 이면   'Average % use' 는 OFF. 상호배타적이다.
        m_check6.SetCheck(0);   //son211015 
#endif
    }
    else {//sylee150812-1     
        g_bSigmaCoef_Enable=0;    //son200827-4w_Sigma 
        m_check1.SetCheck(0);   //son200827-4w_Sigma
    }//sylee150812-1   
    //---------------------------------------------------------


    //#################################################################
    //very import //sylee160520-3
    int  nIGain;//SYLEE160520-2 

    //son SelfTest Setting 관련 값을 SysSet15에서 로딩
    FileSysInfo01.LoadSaveSet15(_LOAD);//load   //sylee140911   //son211015 2 -> _LOAD

    m_nMode1=181;  //lv 10v   //sylee130701


    int nFil,nRet1,Lo1;
    __int64   tStart1, tStart2 ; 
    double dVbR1,dVbR2 ;
    double nT[10];
    int nIFilter,nHVOffSet,nHVGain;//sylee130629
    int nISet;


    memset(nT, 0x00, sizeof(nT));

    if(m_nMode1!=dRec[m_nMode1][0]){
        AfxMessageBox(" Error No 8516, \n   Self Test Error    Parameter loading Error  V I Delay");
        //  return 0;
    }


    nCC= (int)dRec[m_nMode1][1];
    nVRel= (int)dRec[m_nMode1][2];   
    nVSet=  dRec[m_nMode1][3];
    // nVSet= (int)40;
    nIR=  (int)dRec[m_nMode1][4];
    nISet= (int) dRec[m_nMode1][5];
    nFil=  (int)dRec[m_nMode1][8];  //    1=VC FAST, 2= VC SLOW ,  3=HV OPEN, ( 4=4W )
    nTimeDelay1=(int)(dRec[m_nMode1][6]*10);//syle20120104     SETUPCALIBRATION

    nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629        
    nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
    nHVGain = (int)dRec[m_nMode1][11];  //SYLEE130629    
    nTimeDelay1=(int) (dRec[m_nMode1][6]*10);//syle20120104     SETUPCALIBRATION

    //ADC COUNT
    Sub71.nADCount1 =(int)dRec[m_nMode1][7];

    if(Sub71.nADCount1<1 ){
        Sub71.nADCount1=1;
    }

    if( Sub71.nADCount1>100){
        Sub71.nADCount1=100;
    }

    //son Power 관련 Output 비트를 모두 초기화 (D96 All Init.  HV, LV  VB All Relay off)
    nRet1=Sub71.AOnPoweroff2();//sylee20120702 Sub71.AOnPoweroff();

/*
 *  //son200820
    // son?????  4W 정도개선 코드에는 빠져 있는 부분.  4W 정도개선 코드와 동일하게 일단 삭제. 
    if( SysSet13.m_n4WCVMode ==1 ) {     //sylee200321-4WCVMODE
        if(dRec[176][3]<=20){
            nVSet=dRec[176][3];
        }
    }else{
        if(dRec[171][3]<=20){
            nVSet=dRec[171][3];
        }
    }   //sylee200321-4WCVMODE
*/


    if( nVRel==1){
        nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
    }
    else if( nVRel==2){
        nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
    }
    else if( nVRel==3){
        nRet1=Sub71.AOnVSetMulti1(0,0,nVSet,0) ;
    }
    else if( nVRel==4){
        nRet1=Sub71.AOnVSetMulti1(0,0,0,nVSet) ;
    } 



    Sub71.Delay_100ns(10000000);  //1 sec  //SYLEE20111214 


    //son?????   CVMode 머지할 때는 2000이었는데 4W 정도개선 코드에는 20임. 4W 정도개선코드 기준으로 수정함.
    //Lo1=2000;//sylee180129  3->2000    //sylee200217-4WCVMODE 
    Lo1=200;//sylee180129  3->20   //sylee200828  20-> 200


Task101:  

#ifdef _PC_TEST_
    Sub71.V1 =15;
#else
    Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
#endif
    if(nVSet<5){
        dVbR2=nVSet*(0.35);//10%
    }
    else {
        dVbR2=nVSet*(0.1);//10%
    }
    dVbR1=nVSet-dVbR2;
    dVbR2=nVSet+dVbR2;

    if( Sub71.V1>300){
        if(Lo1-->0){
            Sub71.Delay_100ns(3000000);  //300ms   
            goto Task101;
        }

        str.Format("Error No 1312,\n \n\n  Power Check? \n\n\n    Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str); 
        //              nSelfRunFlag=0;
        //  return ;
    }

    if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){

        if(Lo1-->0){
            Sub71.Delay_100ns(3000000);  //300ms   
            goto Task101;
        }

        str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str); 
        //  return ;
    }



    if( nFil==1 ||  nFil==2){//SYLEE130628
        Sub71.AOnReadV1Enable1(nVRel); 
    }
    else {
        Sub71.AOnReadV1Enable1(5);
    }




    if(nFil==3){

        Sub71.HV_OffSet_Gain(nHVOffSet, nHVGain);    //sylee140226
    }


    Sub71.FilterSet(nFil);//sylee130628 
    if(nIFilter==3){
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
        nIFilter=1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629


    tStart1=GetMilSecond2();
    tStart2=GetMilSecond2();     
    nT[0]=(double)(tStart2-tStart1)/1000.;  //test


    Sub71.OnPinAllReset1(); ///PIN ALL RESET 

    Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);  //     //SYLEE150210  MOVE 
    Sub71.Delay_100ns(3000000);  //300ms      //SYLEE150210  MOVE


    Sub71.OnPinResetInit();//sylee151221   //ACE400


    nIGain = (int)dRec[m_nMode1][12];       //sylee150904 //ACE400

    if( nIGain==2){//sylee150904 //ACE400
        Sub71.I_Gain(2);  //10x  =io on     
    }
    else {
        Sub71.I_Gain(1);  //1x  =io off
    }


    //###################################################################

    //task2:

    //  DoEvents();


    Sub71.OnPinAllReset1(); ///PIN ALL RESET

    Sub71.AOnPinSet1(1, 1, 2);   
    Sub71.AOnPinSet1(1, 1, 1);   

    Sub71.Delay_100ns(1000);

    Sub71.AOnReadV1I1(nVRel);
    Sub71.OnPinAllReset1(); ///PIN ALL RESET     

    Sub71.AOnPoweroff2();//sylee20120702  Sub71.AOnPoweroff();

    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704


    Sub71.OnPinResetInit();//sylee151221   //ACE400

    //son "4W_Per_High.ini" 파일을 gd4W1_P2High[][][]에 load한다.
    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load  22 //sylee0606-1    //son211012 22 -> _LOADHIGH

#ifndef __MST__CUSTOMER //son220308 MST는 Sigmar계수필터와 Average % 필터를 동시 적용 요청
    if(!m_check1.GetCheck())       //son 'Average % use' ON 하려면 먼저 'Sigma Coef'가 OFF 상태여야 한다. 
#endif
    {
        //son 40%를 설정했다면,  40%이상의 데이터는 폐기하고 40% 이내의 데이터만으로 평균값을 산출한다.
        if(gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]==1){              //son210702 //son210916
            m_check6.SetCheck(1);   //son 'Average % Use' 를 On 설정  
        }
        else {
            m_check6.SetCheck(0);   //son 'Average % Use' 를 Off 설정
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
        }
    }


    k=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];  //son210702 //son210916
    if(k<0){
        k=0;
    }
    if(k>500){
        k=500;
    }
    gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=k;   //son210702 //son210916
    sprintf(szText, "%d",  k);
    m_edit6.SetWindowText(szText);      //son Average % value 


    FileSysInfo01.LoadSaveSet211(2);//load //sylee160928

    A_Language() ;

    m_edit3.SetWindowText("0");//SYLEE181010-1
    m_edit4.SetWindowText("0");//SYLEE181010-1
    m_check2.SetCheck(0);//SYLEE181010-1        //son 'Eject Low, high'  Off



#ifdef __JTECH__CUSTOMER  //sylee230525
   m_check1.SetCheck(1);
   m_edit8.SetWindowText("3");//sylee230525
#endif


    EndWaitCursor();            //son2020708: 커서를 원상 복구한다.

    UpdateData(FALSE);      // UI에 값을 반영한다.      //son220308
} 

 

void CModeSub17::OnDblClickMsflexgrid1() 
{
    // TODO: Add your control notification handler code here
    BeginEditing();     
    
}

void CModeSub17::OnEnterCellMsflexgrid1() 
{
    // TODO: Add your control notification handler code here
    m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSub17::OnLeaveCellMsflexgrid1() 
{
    // TODO: Add your control notification handler code here
    if (OnEditing) {
        EndEditing();
    }   
}

void CModeSub17::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
    // TODO: Add your control notification handler code here
    m_grid1.SetRowSel(m_nCurRow);
    m_grid1.SetColSel(m_nCurCol);   
}


void CModeSub17::BeginEditing()
{
 
    OnEditing = TRUE;
    CRect rectCell;
    rectCell.left = m_rectGrid.left + long(m_dRatio * double(m_grid1.GetCellLeft()));
    rectCell.top = m_rectGrid.top + long(m_dRatio * double(m_grid1.GetCellTop()));
    rectCell.right = rectCell.left + long(m_dRatio * double(m_grid1.GetCellWidth()));
    rectCell.bottom = rectCell.top + long(m_dRatio * double(m_grid1.GetCellHeight()));
 
    m_edit1.SetWindowText(m_grid1.GetTextMatrix(m_nCurRow, m_nCurCol));
    m_edit1.MoveWindow(rectCell);
    m_edit1.EnableWindow(TRUE);
    m_edit1.ShowWindow(SW_SHOWNORMAL);
    m_edit1.SetFocus();     //son m_edit1으로 focus를 가져 온다.
    m_edit1.SetSel(0, -1);
 
}

void CModeSub17::EndEditing()
{
 
    CString strText;

//son221111 begin:
#ifdef __GTS_4W_SPCIALNET__   
    //--------------------------------------------------------------------
    //son grid 현재위치의 text를 읽어서 변환하여 bPrevNetSpecial 를 획득.
    byte bPrevNetSpecial = 0;
    if ((m_nCurCol-1) == _4W_SPECIAL)
    {
        strText = m_grid1.GetText();            
        bPrevNetSpecial = (byte)atoi(strText);
    }
#endif
//son221111 end
 
    //----------------------------------------------------
    //son edit한 결과 str을 Grid 현재 위치에 write한다.
    m_edit1.EnableWindow(FALSE);
    m_edit1.ShowWindow(SW_HIDE);
    m_edit1.GetWindowText(strText);

    m_grid1.SetText(strText);
    OnEditing = FALSE;
    m_grid1.SetFocus(); //son grid1으로 focus를 돌려 준다.
//  SetModified(TRUE);  




//son221111 begin:
#ifdef __GTS_4W_SPCIALNET__   
    //--------------------------------------------------
    //son 특수넷으로 바뀌었다면 No 컬럼 색을 변환한다. 
    
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작
    if ((m_nCurCol-1) == _4W_SPECIAL)
    { 
        int net = m_nCurRow;
        byte bNetSpecial= 0;
        bNetSpecial     = (byte)atoi(strText);
        
        //son edit 결과가 이전과 다르다면 No 컬럼의 색을 특수넷 여부에 맞추어 바꾼다.
        if (bNetSpecial != bPrevNetSpecial)
        {
            if (bNetSpecial == 1)
                SetOn_NetSpecial(net);
            else
            {
                if (bNetSpecial == 0)
                    SetOff_NetSpecial(net);

                //son 1도 아니고 0도 아닌 기타의 값은 이전 상태를 그냥 유지한다. 
                else
                {
                    strText.Format("%d", bPrevNetSpecial);
                    m_grid1.SetText(strText);
                }
            }

            //son All Check 확인을 위해 netSpecialCnt를 센다.
            int netSpecialCnt = 0;      
            byte bNetSpecial2= 0;
            for (net=1; net <= g_s4WNet[dutIdx].wCount; net++) 
            {
                strText=m_grid1.GetTextMatrix(net, 1+_4W_SPECIAL);  // 특수넷 여부  
                bNetSpecial2=(byte)atoi(strText);     
                //bNetSpecial2 = g_s4WNet[dutIdx].saData[net-1].bNetSpecial;
                if (bNetSpecial2 == 1)
                    netSpecialCnt++;
            }

            //son 이번 체크 On으로 All Check가 되었다면 
            if (netSpecialCnt ==  g_s4WNet[dutIdx].wCount)     
                m_check_AllSpecial.SetCheck(1);

            //son 이번 체크 Off로 All Check가 아니게 되었다면 
            else
                m_check_AllSpecial.SetCheck(0);

            UpdateData(FALSE);      // UI에 값을 반영한다.
        }

    }
#endif
//son221111 end

}

//son 'Ref.SAVE' button   //현재 Grid를 "4W.ini" 파일에 Save
//
// 현재 grid의 판정값 (RefMin ~ RefMax)이 4W.ini 에 저장된다.
// 현재 grid의 판정값 (RefMin ~ RefMax)이 g_s4WNet[dutIdx]에도 저장된다.
void CModeSub17::OnButton_Save() 
{
    //int  j ;  
    CString str; 
    str=m_grid1.GetTextMatrix(1, 1);

    //son Grid에 출력된 값들을  g_s4WNet[dutIdx].saData에 다시 write하고 
    //    '4W.ini' 파일에도 저장한다.  (FileSysInfo01.LoadSaveSub17(_SAVE) 사용)
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    if( g_s4WNet[dutIdx].wCount>=0)      //son241009 0도 처리하게 
    { 
        //son matrix row 0, col 0은 헤더이므로 +1한 위치에서 값을 가져온다.
        for( int net=1 ; net <= g_s4WNet[dutIdx].wCount; net++)     //son190704-4WDUT16
        {  

//          for( j=1 ; j<13 ;  j++) //sylee20120310
//          {            
//              str=m_grid1.GetTextMatrix(net, j); 
//              gd4W1[0][net+4][j]=(float)atof(str);                            
//          } 
//
#ifdef __GTS_4W_SPCIALNET__
            str=m_grid1.GetTextMatrix(net, 1+_4W_SPECIAL);  // 특수넷 여부  //son221107
            g_s4WNet[dutIdx].saData[net-1].bNetSpecial=(byte)atoi(str);     //son221107 
#endif

            //son200803: Piece 값과, Pin 값은 Grid 로딩 후에 변경되는 값이 아니므로 g_s4WNet에는 다시 업데이트할 필요 없다.
            //str=m_grid1.GetTextMatrix(net, 1+_4W_PIECE);  // piece        //son190704-4WDUT16
            //g_s4WNet[dutIdx].saData[net-1].wPiece=(short)atoi(str);                           

            //for(int pin = 0; pin < _4W_PIN_NUM; pin++)
            //{
            //  str=m_grid1.GetTextMatrix(net, 1+_4W_PIN1+pin);     // pin
            //  g_s4WNet[dutIdx].saData[net-1].waPin[pin]=(short)atoi(str);                         
            //}


            str=m_grid1.GetTextMatrix(net, 1+_4W_REF_LOW);  // Set Low      //son211005
            g_s4WNet[dutIdx].saData[net-1].dRefLow=(float)atof(str);        //son211005
            //g_s4WNet[dutIdx].saData[net-1].dRefLow=fCalc[net][CALC_REF_LOW];    //son211015 Grid 출력값은 %.2f로 잘린 값이므로 왜곡이 발생하여 
            //                                                                  //          원본인 fCalc[net][CALC_REF_LOW]로 교체
            //     --> //son211121 edit 한 이후에 변경된 값이 save시  반영이 안되는문제 발생.  son211121 원복하고 Grid출력을 .%.3로 수정


            str=m_grid1.GetTextMatrix(net, 1+_4W_REF_AVG);  // Set Avg      //son211005
            g_s4WNet[dutIdx].saData[net-1].dRefAvg=(float)atof(str);        //son211005
            //g_s4WNet[dutIdx].saData[net-1].dRefAvg=fCalc[net][CALC_AVG];;   //son211015 Grid 출력값은 %.2f로 잘린 값이므로 왜곡이 발생하여 
            //                                                              //          원본인 fCalc[net][CALC_AVG]로 교체 
            //     --> //son211121 edit 한 이후에 변경된 값이 save시  반영이 안되는문제 발생.  son211121 원복.  Grid출력을 .%.3로 수정



            str=m_grid1.GetTextMatrix(net, 1+_4W_REF_HIGH);     // Set High //son211005
            g_s4WNet[dutIdx].saData[net-1].dRefHigh=(float)atof(str);       //son211005
            //g_s4WNet[dutIdx].saData[net-1].dRefHigh=fCalc[net][CALC_REF_HIGH];  //son211015 Grid 출력값은 %.2f로 잘린 값이므로 왜곡이 발생하여 
            //                                                                  //          원본인 fCalc[net][CALC_AVG]로 교체 
            //     --> //son211121 edit 한 이후에 변경된 값이 save시  반영이 안되는문제 발생.  son211121 원복.  Grid출력을 .%.3로 수정

            str=m_grid1.GetTextMatrix(net, 1+_4W_MODE);     // n4wMode
            g_s4WNet[dutIdx].saData[net-1].n4wMode=(int)atoi(str);                          

            str=m_grid1.GetTextMatrix(net, 1+_4W_TEST_MIN);     // Test Min
            g_s4WNet[dutIdx].saData[net-1].dTestMin=(float)atof(str);                           

            str=m_grid1.GetTextMatrix(net, 1+_4W_TEST_AVG);     // Test Avg
            g_s4WNet[dutIdx].saData[net-1].dTestAvg=(float)atof(str);                           

            str=m_grid1.GetTextMatrix(net, 1+_4W_TEST_MAX);     // Test Max
            g_s4WNet[dutIdx].saData[net-1].dTestMax=(float)atof(str);                           

            
        }       

        //son 4W.ini 에 g_s4WNet의 값을 write한다.
        FileSysInfo01.LoadSaveSub17(_SAVE);// SAVE
    }
    else{
        AfxMessageBox("ERROR No 2013  : \n \n W4.INI FILE Error !     file close! !  .\n \n    TOTAL COUNT IS < 0 ?  ", MB_OK | MB_ICONSTOP);

    }

#ifdef __JTECH__CUSTOMER
    //son 제이텍 요구사항: 4W_Setup_A_SigmaLoad.csv 에 Ref.Low, Ref.High를 더해서 4W_Setup_A_SigmaLoad_Ref.csv출력.
    AddRefLowHigh_SigmaLoadCsv();   //son230724
#endif

    OnButton_Load();  //reset

    //son 4W.ini를 로드하고  Error 여부를 체크
    Check_4WErrorNet();   //sylee171122 //son231019_2 OnErrorNet1() -> Check_4WErrorNet()

}


//------------------------------------------------------------------
//son "4W_Setup_A.txt" 파일 로딩. 4W Auto Test결과를 Grid에 반영한다.

//son "4W_Setup_A.txt" 의 string data 를 read하여 float 변수 fCalc[net][] 로 임시 저장하고 다시 Grid에 로딩한다. 
//son "4W_Setup_A.txt"는 auto sampling으로 4W auto test 한 결과일 수도 있고, 
//son 'Test' 버튼을 눌러서 수동측정한 결과를 'Man. File Save'버튼을 눌러서 저장한 결과일 수도 있다.
//son fData[net][]를 fCalc[net][] 구조에 맞추어 저장하고 Grid 의 해당 위치에 Setting한다.
void CModeSub17::OnButton_Load_Auto4WData()   //edit file  load//sylee150806
{

    int net;
    CString str1,str;
    //double dA1,dB1, dC1;
    
    int nLimitType;//sylee171103-9
    nLimitType=0;//sylee171103-9
                //son nLimitType=0: 'EJECT COUNT  LOW  HIGH' 값이나 g_dSigmaCoef 값이 둘다 Off
                //    nLimitType=1: 'EJECT COUNT  LOW  HIGH' 값이나 g_dSigmaCoef 값이 설정
                //    nLimitType=2: 'EJECT COUNT  LOW  HIGH' 값이나 g_dSigmaCoef 값이 둘다 Off이고 'Average % Use' On 


    //---------------------------------------------------------------------
    //son dutIdx에 맞는  기존 4W Manual Test 결과(4W_Setup_A.txt)를 로딩.  
    //    Sigma계산, Grid 출력은 하지 않는다.
    
    //son200320-4WDUT16: Multi DUT 구조로 수정.
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.   

    //son auto sampling 결과인  "4W_Setup_A.txt" 을  read하여 float fData[]로 임시 저장한다.
    //     fData[net][] 에서 min, max, avg, sigma를 구해 fCalc[net][] 에 저장하고 
    //     fCalc[net][CALC_AVG] 값을 따로 fCalc_AvgOrg[net]에 보관
    int ret = _Load_Auto4WData(dutIdx);   //son210623
    if (ret <= 0)
        return;



    /////SYLEE181010-1/#########################################################
    
    //=====================================================================
    //son 'Average % Use' 설정  체크

    int k,oldVal,newVal,valChanged;

    //son "4W_Per_High.ini" 파일을 gd4W1_P2High[][][]에 load한다.


#ifdef __JTECH__CUSTOMER   //sylee230525
       m_check1.SetCheck(1);  
       m_edit8.SetWindowText("3");//sylee230525
#endif

    FileSysInfo01.LoadSaveSub18(_LOADHIGH);     //load  22   //son211012 22 -> _LOADHIGH

    //son 'Average % Use'를 40%를 설정했다면,  40%이상의 데이터는 폐기하고 40% 이내의 데이터만으로 평균값을 산출한다.
    
    //son 'Average % Use' Check상태가 변경되었는지 확인------
    valChanged=0;
    //son210702 Previous 'Average % Use' 상태를 oldVal에 할당.
    oldVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_USE];     //son210916
    if(m_check6.GetCheck()){       //son 'Average % Use' On 현재 상태를 newVal에 할당.
        newVal=1;        
    }
    else{
        newVal=0;        
    }
    //son 변경이 되었다면 현재 화면상태(m_check6) 기준으로 맞춘다.
    if(oldVal!=newVal){
        if(m_check6.GetCheck()){    //son 'Average % Use' On
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;               //son210702 //son210916
        }
        else {
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
        }
        valChanged=1;
    }

    //son 'Average % value' 값이 변경되었는지 확인------
    char   szText[100];
    oldVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];   //son210702   //son210916
    m_edit6.GetWindowText( szText,10);  //son Average % value   
    k  = atoi( szText );
    if(k<0)
        k=0;
    if(k>500)
        k=500;
    newVal=k;

    if(oldVal!=newVal){
        m_edit6.GetWindowText( szText,10);      //son Average % value
        k  = atoi( szText );
        if(k<0){
            k=0;
        }
        if(k>500){
            k=500;
        }
        gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=k;   //son210702 //son210916
        valChanged=1;
    }

    //son 'Average % Use' 또는 'Average % Value'가 바뀐경우 "4W_Per_High.ini" 파일에 업데이트한다. 
    if(valChanged==1){
        FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21   //son211012  21-> _SAVEHIGH
    }


    //===============================================
    //son Eject Low, High 설정  체크 
    //
    //son  'Eject Low High' 기능 *****  //son210916
    //     가장 작은 sampling 값과,  가장 큰 sampling 값들을 제외시키기 위한 기능. 
    //     Edit3에는 nEjectLowCnt(작은 sampling 값을 제외시킬 개수),  
    //     Edit4에는 nEjectHighCnt(큰 sampling 값을 제외시킬 개수)를 설정한다. 
    //     sample이 5개 이상이어야 설정이 동작한다. 
    
    int nEjectMax = 0, nEjectLowCnt= 0, nEjectHighCnt = 0;//SYLEE181010-1   //son210916

    if(nTotalSampCount1>=5)
    {
        //son 제외개수의 상한선  : 20개 sample이면 6개보다는 크게 설정할 수 없다.
        //sylee200820-2 nEjectMax=nTotalSampCount1/2-1;
        nEjectMax=(int)((nTotalSampCount1/2-1)*0.7);//sylee200820-2  //son210916

        if(m_check2.GetCheck()) //EJECT COUNT  LOW  HIGH. //SYLEE181010-1/       
        {
            //son Eject Low 값 Range 체크
            m_edit3.GetWindowText(str);
            nEjectLowCnt =atoi(str);    //son210916
            if(nEjectLowCnt<0.0){   //son210916
                nEjectLowCnt=0;          
            }
            if(nEjectLowCnt>nEjectMax){  //son210916
                str.Format(" ERROR NO 1701-1,\n\n  [ Eject  Low, high] ( Low=%d? > %d ) \n\n ", nEjectLowCnt,nEjectMax);//sylee200820-2  //son210916
                AfxMessageBox(str, MB_OK);//sylee200820-2
                nEjectLowCnt=nEjectMax;             //son210916
            }
            sprintf(szText, "%d",  nEjectLowCnt);//sylee160803-5    //son210916
            m_edit3.SetWindowText(szText);


            //son Eject High 값 Range 체크
            m_edit4.GetWindowText(str);
            nEjectHighCnt =atoi(str);   //son210916
            if(nEjectHighCnt<0.0){  //son210916
                nEjectHighCnt=0;                //son210916
            }
            if(nEjectHighCnt>nEjectMax){ //son210916
                str.Format(" ERROR NO 1701-1,\n\n  [ Eject  Low, high] ( high=%d? > %d ) \n\n ", nEjectHighCnt,nEjectMax);//sylee200820-2    //son210916
                AfxMessageBox(str, MB_OK);//sylee200820-2
                nEjectHighCnt=nEjectMax;                //son210916
            }
            sprintf(szText, "%d",  nEjectHighCnt);//sylee160803-5   //son210916
            m_edit4.SetWindowText(szText);

            //son ModeSub17의 'EJECT COUNT  LOW  HIGH' 값이 설정되면 1 
            if(nEjectLowCnt>0 || nEjectHighCnt>0 ){//sylee171103-9  //son210916
                nLimitType=1;//sylee171103-9
                gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=1;        //son210916
            }
            else {
                gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=0;        //son210916
            }
        } 
    }
    else
    {
        gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=0; 
        nEjectLowCnt=0; nEjectHighCnt=0;       //son210916
        m_edit3.SetWindowText("0"); //son Eject Low  val
        m_edit4.SetWindowText("0"); //son Eject High val
        m_check2.SetCheck(0);
        // AfxMessageBox("    ( 4W Samping Total Count < 5) !    ->    Eject  Low, high =>  uncheck.");    
    }

    FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21   //son220308  Eject Low, High 변경도 파일에 저장하기 위해 추가.

    //########################################################################
    //sylee160722
    // sigma Coefficient
    double  dTmp1;

    //son 'Sigma Coefficient' Enable이면
    if(m_check1.GetCheck()) //sigma 계수     
    {
        //son220308 gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]=1;     //son210916
        g_bSigmaCoef_Enable = 1;        //son220308

        m_edit8.GetWindowText(str); //son Sigma 계수 value
        dTmp1 =atof(str);
        if(dTmp1<0.0){
            dTmp1=0.0;           
        }
        if(dTmp1>9.9){
            dTmp1=9.9;     //son 시그마 계수는 2.5sigma 등과 같이 튜닝을 위해 소수점으로 설정할 수 있어야 한다.
        }
        sprintf(szText, "%.1f",  dTmp1);//sylee160803-5
        m_edit8.SetWindowText(szText);  //son Sigma 계수 value
        g_dSigmaCoef = dTmp1;    //son200827-4w_Sigma

        if(g_dSigmaCoef>0){//sylee171103-9       //son211005 Average % 기능 오동작 문제로 이 위치로 이동함.
            nLimitType=1;//sylee171103-9
        }
    }
    else{
        //son220308 gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]=0;     //son210916
        g_bSigmaCoef_Enable = 0;        //son220308
    }
    LoadSave_SamplingSigmaInput(_SAVE);      //son20308: 4W-SamplingCount 비번입력기능 //son200827-4w_Sigma



    //======================================
    // Sigma Coefficient 수행  : Filtering
    //======================================
    
    //son211015 Sigma Coefficient(계수)도  Average % Use로 사용하지 않은 경우를 위해  
    //    1단계 sigma로 fCalc[net][CALC_SIGMA]를 초기화한다.  fCalc[net][CALC_SIGMA] 값은 Limit 설정시에 꼭 필요하다.
    //    즉 Sample Filtering을 하지 않은 경우는 dSigma[net]를 fCalc[net][CALC_SIGMA]로 사용한다.
    //    fCalc[net][CALC_SIGMA]는 "Set Ref Limit Low/High" 단계에서 사용해야 한다.
    for (net=1; net<=nTotalNet1 ;net++)     //son211015
         fCalc[net][CALC_SIGMA] = fCalc_SigmaOrg[net];        
    
    /////SYLEE181010-1/#########################################################
    //SYLEE181010-1  if(gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]==1){       //son210916

    //son 'Sigma 계수 Enable'이거나 'EJECT COUNT  LOW  HIGH' 값이 1이면 "4W_Setup_A_SigmaLoad.csv"를 출력 
    //    Sigma는 Avg값에 대해  Sigma 기준으로  상한, 하한 값을 정하는 기능
    if( g_bSigmaCoef_Enable==1 ||  gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]==1 ) //SYLEE181010-1     //son210916
    {

        //son  Avg를 중심으로  Coeff * Sigma 를 넘는 data들은 제외하고 Average와 sigma를 다시 계산한다.
        //     Eject Count Low/High 도 설정되었다면 해당 카운트만큼 제외한다.
        Process_SigmaCoefSampling(nLimitType, nEjectLowCnt, nEjectHighCnt);  //son211005
    }


    //=================================
    // % average Use 수행 : Filtering
    //=================================
    
    //son 'Average % Use'를 5%를 설정했다면,  +/- Avg*5%를 벗어나는 데이터는 폐기하고 범위 내의 데이터만으로 평균값을 산출한다.
    //          dAvgLsl= dAvg-(dAvg*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    
    //          dAvgUsl= dAvg+(dAvg*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    
    //          => Avg-5% ~  Avg +5% 사이의 값들만 인정하고 새로 평균을 내서 Grid에 출력한다.

    //sylee160722 if(   (gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]==1)&&(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0)){//sylee160530
    if ( ((gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]==1)        //son 'Average % Use' check 는  1 이고   //son210916
            && (gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0))   //son 'Average %' edit value는 1 이상이면 //sylee160722   //son210702    //son210916
#ifndef __MST__CUSTOMER //son220308 MST는 Sigmar계수필터와 Average % 필터를 동시 적용 요청
            && (g_bSigmaCoef_Enable==0)                      //son 'Sigma 계수 Enable'도 Off 이면       //son210916     //son220308
#endif
       )
    {
        
#ifndef __MST__CUSTOMER //son220308 MST는 Sigma계수필터와 Average % 필터를 동시 적용 요청
        //son 'EJECT COUNT  LOW  HIGH' 값이나 g_dSigmaCoef 값이 둘다 Off이고 
        if( nLimitType!=1)//sylee171103-9
#endif
        {
            //son 'Average % Use' 설정이라면  nLimitType = 2
            if(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0.1){//sylee171103-9  //son210702 //son210916
                nLimitType=2;//sylee171103-9
            }
        }

        //son  Avg를 중심으로  지정된 Percent를 넘는 data들은 제외하고 Average와 Sigma를 다시 계산한다.
        Process_AvgPercentSampling(nLimitType);      //son211005
    }


    //============================
    //son Grid Display 출력
    //============================

    int nPassCount1;
    nPassCount1=0;

    //son211005 GRID의 Ref.Min Ref.Max를  "3. Load 4W AutoData" 단계에서는 Min, Max로 업데이트하고 
    //      "5.  상/하한치 설정 및적용" 을 하고 나면 LowLimit, HighLimit로 바꾸어서 사용함. (같은 필드를 2가지 용도로 사용하는 문제.)  
    //      현장에서 불량판정기준으로 LowLimit, HighLimit를 사용해야 하는데 "5. 상/하한치 설정 및적용" 안하고 쓰면 
    //      단순히 Min, Max를 기준값으로 사용해서 불량이 실제보다 많이 감지될 수 있다. 
    //
    //  < 위준명 과장이 확인해준 현장 사용상황 >    //son211005
    // 구버전: 
    // 1. 2단자 Pass 확인후 타켓창 수동으로 지그 프레스
    // 2. 수동 쌤플링 1회 성공 및 Ref Save
    // 3. 상/하한치 조건 설정 및 적용 
    // 4. Auto Sampling
    // 5. Sigma ave/Eject Low，Hign/Average% 설정
    // 6. Load Auto 4W Data （Min = Ave = Max）
    // 7. 상/하한치 조건 설정 및 적용
    // 8. Save
    // 9. Auto Sampling off/
    //
    // 현재버전:  수동 쌤플링 필요 없는 버전 
    // 1. Auto Sampling 선택 및 테스트 진행
    // 2. 쌤플링 완료후 4W창 Open 및 Auto Sampling Off
    // 2. Sigma avg /Eject Low,High / Average% 설정
    // 3. Load Auto 4W Data （Min = Avg = Max）
    // 4. Save
    // 5. 상/하한치 설정 및 적용 
    // 6. Save
    //
    // 현장에서는 Min, Max 사용하지 않음.  Min, Max는  4W 정도 확인시에 이상윤부장님이 사용하므로 
    // 기존 항목은Ref.Low와 Ref.High로 변경하고,   이상윤부장님을 위한 SmpMin, SmpMax를 새로 추가해 주기로 함.   
    // Ref.Low와 Ref.High는  "3. Load 4W AutoData" 단계에서는  write하지 않고,  SmpMin, SmpMax로 본래 의미대로 출력한다.  
    // Ref.Low와 Ref.High는  "5. 상/하한치 설정 및적용" 을 해야 수정이 되도록 업데이트한다.   //son211005

    //son 새로 계산된 fCalc[net] avg, min, max을  Grid Ref.Min, Ref.Avg, Ref.Max에 반영한다.
    //    주의: fData[][]는 출력하지 않는다.  AutoSampling은 Grid의 데이터 출력란을 넘어서기 때문임. 
    //          Grid 대신에  "4W_Setup_A_SigmaLoad.csv", "4W_Setup_A_%AvgLoad.csv"에 fData를 출력한다.
    for( net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
    {      
        str.Format("%.2f", fCalc[net][CALC_MIN]); //min
        m_grid1.SetTextMatrix(net, 1+_4W_SMP_MIN, str);     //son211005 _4W_REF_LOW -> _4W_SMP_MIN으로 수정

        fCalc[net][CALC_REF_LOW] = 0;                         //son211015 Ref.Low Raw Data 저장기능 추가
        m_grid1.SetTextMatrix(net, 1+_4W_REF_LOW, "0");     //son211015 Ref.Low 영역은 "SET Ref.Limit" 기능으로 설정해야 한다. 이 단계에서는 0

        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str.Format("%.2f", fCalc[net][CALC_AVG]); //avg
        m_grid1.SetTextMatrix(net, 1+_4W_REF_AVG, str);     //son211005 Aut Sampling Data 적용단계에서는 Ref.Avg까지만 업데이트한다.
                                                            //     Ref.Low, Ref.High는 "SET High/Low"후에 "Limit Low-", "Limit High+" 버튼 수행해야 설정된다.
        str.Format("%.2f", fCalc[net][CALC_MAX]); //max
        m_grid1.SetTextMatrix(net, 1+_4W_SMP_MAX, str);     //son211005 _4W_REF_HIGH -> _4W_SMP_MAX로 수정

        fCalc[net][CALC_REF_HIGH] = 0;                        //son211015 Ref.High Raw Data 저장기능 추가
        m_grid1.SetTextMatrix(net, 1+_4W_REF_HIGH, "0");    //son211015 Ref.High 영역은 "SET Ref.Limit" 기능으로 설정해야 한다. 이 단계에서는 0

        //son Mode 정보를 계산해서 추가 출력한다.
        for( int k1=1 ; k1<=5; k1++)    //sylee170314-2
        {
            if(fCalc[net][CALC_AVG]<dRec4w[k1][3])
            {
                str.Format(" %d", k1+170);  
                if(fCalc[net][CALC_AVG]> MAX_4W_R){//sylee171122-1
                    str.Format("0");  //sylee171122-1
                }//sylee171122-1

                m_grid1.SetTextMatrix(net, 1+_4W_MODE, str);
                nPassCount1++;
                break; 
            } 
        }   
    }


    if( nPassCount1<nTotalNet1){
        str.Format("\n  Mode 171~175  ?   SET Please!     \n\n     Net Mode?=%d ea , Total Net=%d",  (nTotalNet1-nPassCount1) , nTotalNet1);
        AfxMessageBox(str, MB_OK);          
    }


    return ;


}

//son  Avg를 중심으로  Coeff * Sigma 를 넘는 data들은 제외하고 Average와 sigma를 다시 계산한다.
//     Eject Count Low/High 도 설정되었다면 해당 카운트만큼 제외한다.
void CModeSub17::Process_SigmaCoefSampling(int nLimitType, int nEjectLowCnt, int nEjectHighCnt)
{
    int nFirst;

    //##########################################################
    //sylee171103-9

    //son sampling data는 No1~ No10 영역에 출력되지 않고 따로 "4W_Setup_A_SigmaLoad.csv" 파일에 출력하고 
    //    Min, Max, Avg만 Grid 화면에 출력함.  즉,  Grid의 No1 ~No10 영역은 'Load Auto 4W Data' 버튼을 눌러도 바뀌지 않는다.   
    //    sampling 카운트가 10개를 넘어 1000(MAX_4W_SAMPLE)개까지도 가능하기 때문임

    FILE *fp2;
    CString str, str2;
    char  fName2[200] ;
    ::ZeroMemory(&fName2, sizeof(fName2)); 

    str2.Format("%s\\4W_Setup_A_SigmaLoad.csv",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    strcat( fName2 , str2 );

    fp2=fopen(fName2,"wt");
    if(fp2==NULL){
        str.Format("Error No 1017.   %s   ?      ->  File Reading Error ", str2);
        AfxMessageBox(str, MB_OK);          
        return ;
    } 
    //#####################################################

    //--------------------
    //son 4W 헤더 출력
    if(nLimitType==1){//sylee171103-9
        fprintf(fp2, "  No.,  Pin1,  Pin2,   Pin3,   Pin4,   , " );  //sylee171103-9
        for(   int k2=1; k2<=nTotalSampCount1 ;k2++){
            fprintf(fp2, "    #%d       , ",k2 );  //sylee171103-9
        }
        //son210928 fprintf(fp2, " \n " );  //sylee171103-9
        fprintf(fp2, " newMin,  newAvg,  newMax,  newSigma,  ,orgAvg, orgSigma, %.1fSigmaLow, %.1fSigmaHigh  \n ", 
                g_dSigmaCoef, g_dSigmaCoef);  //sylee171103-9  //son210928 MST 요청으로 new Sigma 추가
    }

    //--------------------
    //son 4W Data 출력
    double dLsl, dUsl, dAvg1;       //son210916
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    for (int net=1; net <= nTotalNet1 ; net++)
    {

        if(nLimitType==1){//sylee171103-9
            fprintf(fp2, " %d,   %d,   %d,   %d,   %d,    ,",
                    net,(int)g_s4WNet[dutIdx].saData[net-1].waPin[0],(int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                    (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  //sylee171103-9
        }

        fCalc[net][CALC_MIN]=fCalc[net][CALC_AVG];//min
        fCalc[net][CALC_AVG]=0.0;//average
        fCalc[net][CALC_MAX]=fCalc[net][CALC_AVG];//max


        nCount=0;nFirst=0;//sylee150812
        //son Sigma 계수 Enable 이면 
        //if(gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]==1){//SYLEE181010-1     //son210916
        if(g_bSigmaCoef_Enable==1){//SYLEE181010-1     //son220308
            dAvg1= fCalc_AvgOrg[net];
            dLsl= dAvg1 - (g_dSigmaCoef * (fCalc_SigmaOrg[net]));    //son 시그마 하한 (Lower Spec Limit: LSL)  //son211202_6
            dUsl= dAvg1 + (g_dSigmaCoef * (fCalc_SigmaOrg[net]));    //son 시그마 상한 (Upper Spec Limit: USL)  //son211202_6
        }
        else {//SYLEE181010-1                        

            dLsl=0.0;//SYLEE181010-1   //son210916
            dUsl=0.0;//SYLEE181010-1   //son210916
        }

        /////SYLEE181010-1/#########################################################
        double  fDa2[MAX_4W_SAMPLE +1];    //son 오름차순 Sorting 결과  //son231106
        short   nPo1[MAX_4W_SAMPLE +1];    //son Select 여부 표시       //son231106
        int     nCo1,nCo2;

        //son  'Eject Low High' 기능 *****
        //     가장 작은 sampling 값과,  가장 큰 sampling 값들을 제외시키기 위한 기능 
        //     Edit3에는  작은 sampling 값을 제외시킬 개수,  Edit4는  큰 sampling 값을 제외시킬 개수를 write한다. 


        //son 'EJECT COUNT  LOW  HIGH' 값이 설정되었다면
        double dEjLsl, dEjUsl;//SYLEE181010-1
        dEjLsl=0.0,dEjUsl=0.0;//SYLEE181010-1
        if( gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]==1 )//SYLEE181010-1       //son210916
        {
            if(nEjectLowCnt>0 || nEjectHighCnt>0)   //son210916
            {
                //-------------------------------
                //son 일종의 Selection Sort. 
                //    fData[net-1][]를 조사해서 가장 작은 값을 fDa2[1]에 놓고, 해당 nPo1[nCo1]은 뽑혔다고 표시한다.
                ::ZeroMemory(&fDa2, sizeof(fDa2));
                ::ZeroMemory(&nPo1, sizeof(nPo1));
                for( int  sampl1=1; sampl1<=nTotalSampCount1; sampl1++)//sorting
                {
                    nCo1=0; nCo2=0;                          
                    for( int sampl2=1; sampl2<=nTotalSampCount1; sampl2++)
                    { 
                        //son 이미 뽑힌 작은 값은 제외하고 찾는다. 
                        if(nPo1[sampl2]==1){
                            continue;
                        }
                        if( nCo2==0){
                            fDa2[sampl1]=fData[net-1][sampl2];
                            nCo1=sampl2;
                            nCo2=1;
                        }
                        else {  
                            //son 현재 list에서 가장 작은 값을 찾아서 fDa2[sampl1]에 저장.
                            if(fDa2[sampl1] > fData[net-1][sampl2]){
                                fDa2[sampl1]=fData[net-1][sampl2];
                                nCo1=sampl2;
                            }
                        }
                    }
                    if(nCo1>0){
                        nPo1[nCo1]=1;   //son 작은 값으로 뽑혔음.
                    }
                }

                //son 설정된 nEjectLowCnt에 맞는 dEjLsl값을 찾는다.
                //    fDa2[]는 오름차순으로 sorting돼 있으므로 nEjectLowCnt=2라면 fDa2[nEjectLowCnt]가 두번째로 작은값.
                if(nEjectLowCnt>0){ //son210916
                    dEjLsl=fDa2[nEjectLowCnt]; //son210916
                }
                else {
                    dEjLsl=0.0;
                }

                //son 설정된 nEjectHighCnt에 맞는 dEjUsl값을 찾는다.
                //    nTotalSampCount1=20, nEjectHighCnt=3라면 fDa2[18]가 세번째로 큰값.
                if(nEjectHighCnt>0){    //son210916
                    dEjUsl=fDa2[nTotalSampCount1-nEjectHighCnt+1]; //son210916
                }
                else {
                    dEjUsl=0.0;
                }

                //son 'Eject Low High'가 설정돼 있는 동시에 'Sigma 계수' 값이 설정돼 있다면
                //     둘 중에 범위가 더 좁은 쪽으로 dUsl, dLsl을 선택한다. (LSL이면 큰쪽, USL이면 작은 쪽) 
                //if(gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]==1)        //son210916
                if(g_bSigmaCoef_Enable==1)        //son220308
                {
                    //son  dEjLsl이 dLsl보다 크다면 dLsl값을 dEjLsl로 바꾼다. 
                    if(dEjLsl>0){
                        if(dLsl<dEjLsl){    //son210916
                            dLsl=dEjLsl;    //son210916
                        }
                    }
                    //son  dEjUsl이 dUsl보다 작다면 dUsl을 dEjUsl로 바꾼다.
                    if(dEjUsl>0){
                        if(dUsl>dEjUsl){    //son210916
                            dUsl=dEjUsl;    //son210916
                        }
                    }                                
                }

                //son 'Eject Low High'만 설정되고 'Sigma Averaga' 없다면  dUsl, dLsl을 EjectCount기준으로 설정.
                else {
                    dLsl=dEjLsl;    //son210916
                    if(dEjUsl==0){
                        dEjUsl=2000000;
                    }
                    dUsl=dEjUsl;    //son210916
                }
            }
        }
        /////SYLEE181010-1/#########################################################

        int nLowOverCnt = 0; //sylee181208  //son dLsl보다 작은 sample의 개수   //son211005
        int nHighOverCnt = 0; //sylee181208 //son dUsl보다   큰 sample의 개수   //son211005

        //son 현재 sample에서 dLsl보다 작은 sample의 개수와 dUsl보다 큰 sample의 개수를 파악
        int k;
        for(k=1; k<=nTotalSampCount1; k++) //son  k: sample
        {
            if(fData[net-1][k] < dLsl){  //son210916
                nLowOverCnt++;//sylee181208
                //fprintf(fp2, "  ," );  //sylee171106-2

                //son 버그1???? continue가 아무 의미없다.  
                //    버그2???? 여기서 continue하면 아래 nHighOverCnt는 카운트가 안된다.
                continue;
            }
            if(fData[net-1][k]>dUsl){  //son210916
                nHighOverCnt++;//sylee181208
                // fprintf(fp2, "  ," );  //sylee171106-2
                continue;
            }
        }

        //son ??? nLowOverCnt>nEjectLowCnt 이면 nLowOverCnt이 0으로 만드는 건   왜 하는 건지?
        //       nEjectLowCnt 3인데  dLsl보다 작은 게 6개. 이런 케이스가 존재하는가? 
        //       ex) 1.4 1.5  1.5  1.5  1.5  1.5   1.5  1.7  1.8, 1.8  1.9  
        //           10개 데이터가 이렇다면 dEjLsl=fDa2[3] 은 1.5 이므로 dLsl은 1.5이다. 
        //  Sigma 계수만 체크돼 있고, Eject Low,High 체크 안 돼 있으면 nLowOverCnt, nHighOverCnt는 이 코드에 의해
        //  무조건  0으로 설정이 된다.  문제??
        //           
        nLowOverCnt=nEjectLowCnt-nLowOverCnt;//sylee181208  //sylee181217-2   //son210916
        if(nLowOverCnt<0 || nLowOverCnt>nEjectLowCnt){//sylee181208   //son210916
            nLowOverCnt=0;
        }

        nHighOverCnt=nEjectHighCnt-nHighOverCnt;//sylee181208 //sylee181217-2   //son210916
        if(nHighOverCnt<0 || nHighOverCnt>nEjectHighCnt){//sylee181208  //son210916
            nHighOverCnt=0;
        }


        //-------------------------------------------------------------------------------
        //son dLsl, dUsl을 벗어나는 값은 제외하고 다시 fCalc[net] avg, min, max를 구한다.
        

#ifdef __MST__CUSTOMER      //son220308
        //son220308 MST Sigma 계수필터와 Average % 필터를 동시 적용 요청
        //          fData2를 fData를 가리키게 해서 Process_SigmaCoefSampling()의 결과가 
        //          Process_AvgPercentSampling()함수에 영향이 있도록 만든다.
        float*  fData2= fData[net-1];
#else
        //son210928 new sigma 계산을 위해 사용. sampling제외된 경우는 -1로 표기한다. 
        float  fData2[MAX_4W_SAMPLE +1];    //son 제외한 데이터를 -1 표시한 fData   //son231106 300개 4W sample 로딩시 죽는문제 수정  SAMPLE -> SAMPLE+1
        ::ZeroMemory(&fData2, sizeof(fData2));   //son220308 

#endif

        for( k=1; k<=nTotalSampCount1; k++)
        {

            if(fData[net-1][k]<dLsl){  //son210916
                fprintf(fp2, "  ," );  //sylee171106-2
                fData2[k] = -1;  //son210928
                continue;
            }                            
            if(fData[net-1][k]==dLsl){ //son210916
                if(nLowOverCnt>0){
                    nLowOverCnt--;
                    fprintf(fp2, "  ," );  //sylee171106-2
                    fData2[k] = -1;  //son210928
                    continue;
                }
            } 
            if(fData[net-1][k]>dUsl){  //son210916
                fprintf(fp2, "  ," );  //sylee171106-2
                fData2[k] = -1;  //son210928
                continue;
            }
            else if(fData[net-1][k]==dUsl){    //son210916
                if(nHighOverCnt>0){
                    nHighOverCnt--;
                    fprintf(fp2, "  ," );  //sylee171106-2
                    fData2[k] = -1;  //son210928
                    continue;
                }
            }

            if(nLimitType==1){//sylee171103-9
                fprintf(fp2, " %10.2f  ,",fData[net-1][k]);  //sylee171103-9
            } 

            if( fData[net-1][k]<= MAX_4W_R)    //sylee200828  100->200 ohm (200000)
            {
                nCount++;                                               
                fData2[k] = fData[net-1][k];  //son210928

                //SYLEE181224-1 if(k==1)                       
                if(nCount==1){ //SYLEE181224-1                      
                    fCalc[net][CALC_MIN]=(float)fData[net-1][k];//min
                    fCalc[net][CALC_MAX]=(float)fData[net-1][k]; 
                    nFirst=1;
                }
                else{
                    //son new Min 획득
                    if( fData[net-1][k]  < fCalc[net][CALC_MIN]){
                        fCalc[net][CALC_MIN]=(float)fData[net-1][k];
                    }
                    //son new Max 획득
                    if( fData[net-1][k]  > fCalc[net][CALC_MAX]){
                        fCalc[net][CALC_MAX]=(float)fData[net-1][k];
                    }                           
                } 
                fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+(float)fData[net-1][k];
            }

        } 

        //son new Avg 계산--------------
        if(nCount>0){
            fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);                               
        }
        else{
            fCalc[net][CALC_MIN]=999999;//sylee150812
            fCalc[net][CALC_AVG]=999999;//sylee150812
            fCalc[net][CALC_MAX]=999999;//sylee150812
        }

        //son210928 new Sigma 계산--------------
        
        //son new sigma를  4W_Setup_A_Load.csv에 추가하기 위한 변수. 분산합. 
        nCount=0;
        double dVarSum1 = 0.0;  //son211202_2 dVarSum1 초기화 추가. 
                                //            초기화 안해서 이전 net의 dVarSum에 계속 누적되어 dVarSum 값이 왜곡되는 문제 수정함.
        for( k=1; k<=nTotalSampCount1; k++)
        {
            if (fData2[k] > MAX_4W_R)    
                continue;

            if (fData2[k] == -1) //son210928
                continue;

            nCount++;//sylee160604-1

            //son 편차의 제곱을 더해 간다.
            double diffPwr = (double)(fData2[k]-fCalc[net][CALC_AVG])*(double)(fData2[k]-fCalc[net][CALC_AVG]);
            dVarSum1=dVarSum1+ diffPwr; 
        }

        fCalc[net][CALC_SIGMA] = 999999;  //son211005
        if(nCount>0)    //son211005 devide by zero 방지
        {
            //son 편차의 평균으로 분산을 구한다.
            double dVar=dVarSum1/(double)(nCount); 

            //son 분산을 제곱근해서 표준편차(Sigma)를 구한다.   
            fCalc[net][CALC_SIGMA]=(double)sqrt(dVar);     
        }
        //son210928


        if(nLimitType==1){//sylee171103-9

            fprintf(fp2, " %10.2f  ,", fCalc[net][CALC_MIN]);  //son210928 MST 요청으로 추가
            fprintf(fp2, " %10.2f  ,", fCalc[net][CALC_AVG]);  //son210928
            fprintf(fp2, " %10.2f  ,", fCalc[net][CALC_MAX]);  //son210928
            fprintf(fp2, " %10.2f  ,  ,", fCalc[net][CALC_SIGMA]);  //son210928 new sigma (튀는 값 제거 후)

            fprintf(fp2, " %10.2f  ,", fCalc_AvgOrg[net]);   //son211005
            fprintf(fp2, " %10.2f  ,", fCalc_SigmaOrg[net]); //son211005 이전 Sigma (튀는 값 제거 이전)  //son211202_6 g_dSigma1->fCalc_SigmaOrg
            fprintf(fp2, " %10.2f  ,", dLsl);  //son211005 이전 Sigma 기준 Lsl
            fprintf(fp2, " %10.2f  ,", dUsl);  //son211005 이전 Sigma 기준 Usl
            fprintf(fp2, " \n" );//sylee171103-9  
        }                       

    }

    fclose(fp2);//sylee171103-9
}

//son230724 제이텍 요구사항: 4W_Setup_A_SigmaLoad.csv 에 Ref.Low, Ref.High를 더해서 
//    4W_Setup_A_SigmaLoad_Ref.csv출력.  
//    OnButton_Save() 내부에서 g_s4WNet[dutIdx] 데이터가 채워진 다음에 사용해야 한다.
void CModeSub17::AddRefLowHigh_SigmaLoadCsv()
{


    FILE *fp, *fp2;
    CString str, str2;
    char  fName[FILE_PATH_SIZE], fName2[FILE_PATH_SIZE] ;

    ::ZeroMemory(&fName, sizeof(fName)); 
    str2.Format("%s\\4W_Setup_A_SigmaLoad.csv",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    strcat( fName , str2 );
    fp=fopen(fName,"rt");
    if(fp==NULL){
        //str.Format("Error No 1017.   %s   ?      ->  File Open Error ", fName);
        //AfxMessageBox(str, MB_OK);          
        return ;
    } 

    int dutIdx = get4wDutIdx();

    ::ZeroMemory(&fName2, sizeof(fName2)); 
    str2.Format("%s\\4W_Setup_A_SigmaLoad_Ref_DUT%d.csv",(SysInfoView01).m_pStrFilePathBDL, dutIdx+1 );//sylee141212
    strcat(fName2 , str2 );
    fp2=fopen(fName2,"wt");
    if(fp2==NULL){
        str.Format("Error No 1017.   %s   ?      ->  File Open Error ", fName2);
        AfxMessageBox(str, MB_OK);          
        fclose(fp);
        return ;
    } 

    char buf[GET_BUF_SIZE]; 
    CString strbuf;
    int no;

    if (g_s4WNet[dutIdx].wCount >= 0)      //son241009 0도 처리하게 
    {
        for (int net=0; net <= g_s4WNet[dutIdx].wCount ; net++)
        {
            ::ZeroMemory(&buf, sizeof(buf));    
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   //son 읽어온 한 줄을 CString 타입으로 변환
            no     = strbuf.Find('\n'); //son 마지막항목. str끝의 LineFeed 문자를 제외한다.
            strbuf = strbuf.Left(no);

            if (net == 0)   //son 파일 헤더
                fprintf(fp2, "%s,  ,Ref.Low   ,Ref.High\n", strbuf);
            else
                fprintf(fp2, "%s, %10.3f  , %10.3f  \n", 
                        strbuf, g_s4WNet[dutIdx].saData[net-1].dRefLow, g_s4WNet[dutIdx].saData[net-1].dRefHigh);


        }
    }

    fclose(fp);
    fclose(fp2);
}

//son 'Average % Use'를 5%를 설정했다면,  +/- Avg*5%를 벗어나는 데이터는 폐기하고 범위 내의 데이터만으로 평균값을 산출한다.
//          dAvgLsl= dAvg-(dAvg*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    
//          dAvgUsl= dAvg+(dAvg*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    
//          => Avg-5% ~  Avg +5% 사이의 값들만 인정하고 새로 평균을 내서 Grid에 출력한다.
void CModeSub17::Process_AvgPercentSampling(int nLimitType)
{
    int nFirst;
    //##########################################################
    //sylee171103-9

    //son sampling data는 No1~ No10 영역에 출력되지 않고 따로 "4W_Setup_A_%AvgLoad.csv" 파일에 출력하고 
    //    Min, Max, Avg만 Grid 화면에 출력함.  즉,  Grid의 No1 ~No10 영역은 'Load Auto 4W Data' 버튼을 눌러도 바뀌지 않는다.   
    //    sampling 카운트가 10개를 넘어 1000(MAX_4W_SAMPLE)개까지도 가능하기 때문임
    
    if(nLimitType!=2)   //son211202
        return;

    FILE *fp2;
    CString str, str2;
    char  fName2[200] ;
    ::ZeroMemory(&fName2, sizeof(fName2)); 

    str2.Format("%s\\4W_Setup_A_%%AvgLoad.csv",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    strcat( fName2 , str2 );

    fp2=fopen(fName2,"wt");
    if(fp2==NULL){
        str.Format("Error No 1017.   %s   ?      ->  File Reading Error ", str2);
        AfxMessageBox(str, MB_OK);          
        return ;
    } 
    //#####################################################


    if(nLimitType==2){//sylee171103-9
        fprintf(fp2, "  No.,  Pin1,  Pin2,   Pin3,   Pin4,   ," );  //sylee171103-9
        for(   int k2=1; k2<=nTotalSampCount1 ;k2++){
            fprintf(fp2, "    #%d       , ",k2 );  //sylee171103-9
        }
        //son210928 fprintf(fp2, " \n " );  //sylee171103-9
        int nAvgPer = gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];
        fprintf(fp2, " newMin,  newAvg,  newMax,  newSigma ,   , orgAvg, %d%% AvgLow,  %d%% AvgHigh\n ", 
                nAvgPer, nAvgPer  );  //sylee171103-9  //son210928 MST 요청으로 new Sigma 등등 추가
    }


    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    double dAvgLsl, dAvgUsl, dAvg;
    for (int net=1; net <= nTotalNet1; net++)
    {

        fCalc[net][CALC_MIN]= fCalc[net][CALC_AVG];//min
        fCalc[net][CALC_AVG]= 0.0;//aver
        fCalc[net][CALC_MAX]= fCalc[net][CALC_AVG];//max

        nCount=0;nFirst=0;//sylee150812
        dAvg= fCalc_AvgOrg[net];
        dAvgLsl= dAvg-(dAvg*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    //son Avg 하한 //son210702   //son210916
        dAvgUsl= dAvg+(dAvg*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    //son Avg 상한 //son210702   //son210916

        if(nLimitType==2){//sylee171103-9
            fprintf(fp2, " %d,   %d,   %d,   %d,   %d,    , ",
                    net,(int)g_s4WNet[dutIdx].saData[net-1].waPin[0],(int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                    (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  //sylee171103-9
        }

        //-------------------------------------------------------------------------------
        //son dAvgLsl, dAvgUsl을 벗어나는 값은 제외하고 다시 fCalc[net] avg, min, max를 구한다.
        double  fDa2[MAX_4W_SAMPLE +1];      //son210928   //son231106
        ::ZeroMemory(&fDa2, sizeof(fDa2));   //son210928 new sigma 계산을 위해 사용. sampling제외된 경우는 -1로 표기한다. 

        for(int  k=1; k<=nTotalSampCount1; k++)
        {
#ifdef __MST__CUSTOMER      
            if(fData[net-1][k] == -1) { //son220308
                fprintf(fp2, "  ," );  
                continue;
            }   //son220308
#endif
            //son Avg 하한보다 작으면 빈칸으로 출력하고 continue로 skip.
            if(fData[net-1][k] < dAvgLsl){
                fprintf(fp2, "  ," );  //sylee171106-2
                fDa2[k] = -1;  //son210928
                continue;
            }
            //son Avg 상한보다 크면  빈칸으로 출력 continue로 skip.
            if(fData[net-1][k] > dAvgUsl){
                fprintf(fp2, "  ," );  //sylee171106-2
                fDa2[k] = -1;  //son210928
                continue;
            }                   
            if(nLimitType==2){//sylee171103-9
                fprintf(fp2, " %10.3f  ,",fData[net-1][k]);  //sylee171103-9
            }

            if( fData[net-1][k]<= MAX_4W_R)
            {
                nCount++;                               
                fDa2[k] = fData[net-1][k];  //son210928

                if(k==1){                       
                    fCalc[net][CALC_MIN]=(float)fData[net-1][k];//min
                    fCalc[net][CALC_MAX]=(float)fData[net-1][k]; 
                    nFirst=1;
                }
                else{
                    if( fData[net-1][k]  < fCalc[net][CALC_MIN]){
                        fCalc[net][CALC_MIN]=(float)fData[net-1][k];
                    }
                    if( fData[net-1][k]  > fCalc[net][CALC_MAX]){
                        fCalc[net][CALC_MAX]=(float)fData[net-1][k];
                    }                           
                } 
                fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+(float)fData[net-1][k];
            }
        }


        //son new Avg 계산 ----------------------
        if(nCount>0){
            fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);                               
        }
        else{
            fCalc[net][CALC_MIN]=999999;//sylee150812
            fCalc[net][CALC_AVG]=999999;//sylee150812
            fCalc[net][CALC_MAX]=999999;//sylee150812
        }

        //son210928 new Sigma 계산  ----------------- 
        nCount=0;
        double dVarSum1 = 0.0;  //son210928  new sigma를  4W_Setup_A_Load.csv에 추가하기 위한 변수. 분산합. 
        for( k=1; k<=nTotalSampCount1; k++)
        {
            if (fDa2[k] > MAX_4W_R)    
                continue;

            if (fDa2[k] == -1) //son210928
                continue;

            nCount++;//sylee160604-1

            //son 편차의 제곱을 더해 간다.
            dVarSum1=dVarSum1+(double)(fDa2[k]-fCalc[net][CALC_AVG])*(double)(fDa2[k]-fCalc[net][CALC_AVG]); 
        }

        fCalc[net][CALC_SIGMA] = 999999;  //son211005
        if(nCount>0)    //son211005 devide by zero 방지
        {
            //son 편차의 평균으로 분산을 구한다.
            double dVar=dVarSum1/(double)nCount;   

            //son 분산을 제곱근해서 표준편차(Sigma)를 구한다.   
            fCalc[net][CALC_SIGMA]=(double)sqrt(dVar);     
        }
        //son210928



        if(nLimitType==2){//sylee171103-9
            fprintf(fp2, " %10.2f  ,", fCalc[net][CALC_MIN]);  //son210928 MST 요청으로 new Sigma 등등 추가
            fprintf(fp2, " %10.2f  ,", fCalc[net][CALC_AVG]);  //son210928 튀는 값 제거 이후 Average
            fprintf(fp2, " %10.2f  ,", fCalc[net][CALC_MAX]);  //son210928
            fprintf(fp2, " %10.2f  ,  ,", fCalc[net][CALC_SIGMA]);  //son210928 튀는 값 제거 이후 Sigma

            fprintf(fp2, " %10.2f  ,", fCalc_AvgOrg[net]);  //son211005 튀는 값 제거 이전 Average
            fprintf(fp2, " %10.2f  ,", dAvgLsl);  //son211005
            fprintf(fp2, " %10.2f  ,", dAvgUsl);  //son211005

            fprintf(fp2, " \n" );//sylee171103-9
        } 
    }

    fclose(fp2);//sylee171103-9
}


//son 'Mode' 버튼
void CModeSub17::OnButton120()   //sylee221017-1
{  

	double dTemp1;
	int nMode1;
	CString str;

	FileSysInfo01.LoadSaveSet21_4W_Range(2);  

 	int	dutIdx = get4wDutIdx(); 	 
	if (g_s4WNet[dutIdx].wCount >= 0)		//son241009 0도 처리하게 
	{ 
	 	for( int net=1 ; net <= g_s4WNet[dutIdx].wCount; net++) 	 
		{  
			str=m_grid1.GetTextMatrix(net, 1+_4W_REF_AVG); 	 
			dTemp1=(float)atof(str);
			
			for( int i=1; i<=5; i++){
				if(dTemp1<dRec4w1[i][3]){	nMode1=170+i;	break;		}
				if(i==5){		nMode1=170+i;	break;		}
			}		 
		
			str.Format("%d", nMode1); 
		    m_grid1.SetTextMatrix(net, 1+_4W_MODE, str); 			
		}		

	}

}



//son NG % (맨 오른쪽 하단)
void CModeSub17::OnButton121()   //sylee190221-1  SPEC OUT NG FILE MAKE
{  
    FILE *fp;
    CString str1,str;
    char  fName[FILE_PATH_SIZE] ; //son210625 200-> FILE_PATH_SIZE
    double dA1,dB1, dC1;
    int nType1;//sylee171103-9
    int nSpecPer;//sylee190221-1
    char szText2[100];

    m_edit2.GetWindowText( szText2,10);     //sylee190221-1
    nSpecPer= atoi( szText2 ); //sylee190221-1
    if(nSpecPer<0){
        nSpecPer=0;
    }
    if(nSpecPer>100){
        nSpecPer=100;
    }
    str.Format("%d",nSpecPer); 
    m_edit2.SetWindowText(str);


    nType1=0;//sylee171103-9

    ::ZeroMemory(&fData, sizeof(fData));
    ::ZeroMemory(&fCalc, sizeof(fCalc)); 
    ::ZeroMemory(&fName, sizeof(fName)); 

    nTotalSampCount1=0;
    nTotalNet1=0;//sylee160521  

    //son200320-4WDUT16: Multi DUT 구조로 수정.
    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.   
    if (dutIdx == 0)
        str1.Format("%s\\4W_Setup_A.txt",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    else    
        str1.Format("%s\\4W_Setup_A.txt%d",(SysInfoView01).m_pStrFilePathBDL, dutIdx+1 );
    //str1.Format("%s\\4W_Setup_A.txt",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    
    strcat( fName , str1 );
    fp=fopen(fName,"rt");
    if(fp==NULL){
        str.Format("Error No 1017.   %s   ?      ->  File Reading Error ", str1);
        AfxMessageBox(str, MB_OK);          
        return ;
    } 

    //##########################################################
    //sylee171103-9

    FILE *fp2;
    CString str2;
    char  fName2[200] ;
    //double dA1,dB1, dC1;   
    ::ZeroMemory(&fName2, sizeof(fName2)); 

    str2.Format("%s\\4W_Setup_A_Load.csv",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    strcat( fName2 , str2 );

    fp2=fopen(fName2,"wt");
    if(fp2==NULL){
        str.Format("Error No 1017.   %s   ?      ->  File Reading Error ", str2);
        AfxMessageBox(str, MB_OK);          
        return ;
    } 
    //#####################################################

    int  no; 
    char buf[GET_BUF_BIG_SIZE];//sylee170705-3 *10      //son210624 2040*20(GET_BUF_BIG_SIZE)으로 원상복구

    fscanf(fp, " %10d,%10d,\n",  &nTotalSampCount1, &nTotalNet1); 
    fscanf(fp, "\n");

    if(nTotalSampCount1> MAX_4W_SAMPLE){//sylee170705
        str1.Format("Error No 1017.   %s  4W SET  file    test total count(=%d)  > 1000 over error    ", 
                                    str2, nTotalSampCount1);
        AfxMessageBox(str1, MB_OK); 
        fclose(fp);      
        return ;
    }

    if(nTotalSampCount1<0){//sylee160521 
        nTotalSampCount1=0;    
    }

    if(nTotalNet1 > MAX_4W_NET){//sylee160812-1   2000->5000   //son-4WDUT-20200403: 5000-> MAX_4W_NET
        str1.Format("Error No 1017.   %s  4W SET  file    Net total count(=%d)  > %d over error    ", 
                                    str2, nTotalSampCount1, MAX_4W_NET);
        AfxMessageBox(str1, MB_OK); 
        fclose(fp);      
        return ;
    }

    if(nTotalNet1<0){ //sylee160521 
        nTotalNet1=0;    
    } 

    int net;
    for( net=-1; net<=nTotalNet1; net++)        //son190704-4WDUT16
    {
        ::ZeroMemory(&buf, sizeof(buf));    //son211112
        fgets(buf, sizeof(buf),fp); //sylee170705  2048*20  //son210524 sizeof(buf)

        if(feof(fp))break;
        str.Format("%s",buf);

        for( int k=-5;  k<=nTotalSampCount1;k++)    //sylee170114
        {
            no=str.Find(',');           
            str.Left(no);
            if(k<0){//sylee170114
                str=str.Mid(no+1);
                continue;//sylee170114
            }//sylee170114

            if(net<0){
                continue;
            }
            fData[net][k]=(float)atof(str);
            str=str.Mid(no+1);
        }
    }

    fclose(fp);
    //======================================================


    int nCount,nFirst;
    ::ZeroMemory(&fCalc_AvgOrg, sizeof(fCalc_AvgOrg));   //sylee160530-1


    for( net=1; net<=nTotalNet1 ; net++)    //son190704-4WDUT16
    {
        fCalc[net][CALC_MIN]=0.0;//min
        fCalc[net][CALC_AVG]=0.0;//aver
        fCalc[net][CALC_MAX]=0.0;//max

        nCount=0;nFirst=0;//sylee150812

        //son fData[net-1]의 sampling data의 Min,Max, Avg를 구해서 
        //    fCalc[net][CALC_MAX], fCalc[net][CALC_MIN], fCalc[net][CALC_AVG]에 저장한다. 
        for(int  k=1; k<=nTotalSampCount1; k++)
        {
            if( fData[net-1][k]<= MAX_4W_R){//sylee171026-3
                nCount++;
                //  if(nFirst==0){                      
                if(k==1){                       
                    fCalc[net][CALC_MIN]=(float)fData[net-1][k];//min
                    fCalc[net][CALC_MAX]=(float)fData[net-1][k]; 
                    nFirst=1;
                }
                else {
                    if( fData[net-1][k]  < fCalc[net][CALC_MIN]){
                        fCalc[net][CALC_MIN]=(float)fData[net-1][k];
                    }
                    if( fData[net-1][k]  > fCalc[net][CALC_MAX]){
                        fCalc[net][CALC_MAX]=(float)fData[net-1][k];
                    }                           
                } 
                fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+(float)fData[net-1][k];
            }
        }

        if(nCount>=3){//sylee150721
            fCalc[net][CALC_AVG]=(fCalc[net][CALC_AVG]-fCalc[net][CALC_MAX]-fCalc[net][CALC_MIN])/(float)(nCount-2);//sylee150721
        }
        else if(nCount>0){
            fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);                               
        }
        else {
            fCalc[net][CALC_MIN]=999999;//sylee150812
            fCalc[net][CALC_AVG]=999999;//sylee150812
            fCalc[net][CALC_MAX]=999999;//sylee150812
        }

        //son 현재 fCalc[net][CALC_AVG] 값을 보관
        fCalc_AvgOrg[net]= (float)fCalc[net][CALC_AVG];//sylee160530-1

    }
    //=====================================================================


    int k,tmp1,tmp2,tmp3;

    //son "4W_Per_High.ini" 파일을 gd4W1_P2High[][][]에 load한다.
    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load  22  //son211015 22 -> _LOADHIGH

    tmp3=0;
    tmp1=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_USE];     //son210702   //son210916
    if(m_check6.GetCheck()){        //son 'Average % Use' On
        tmp2=1;      
    }
    else {
        tmp2=0;      
    }
    if(tmp1!=tmp2){
        if(m_check6.GetCheck()){    //son 'Average % Use' On
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;               //son210702 //son210916
        }
        else {
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
        }
        tmp3=1;
    }

    tmp1=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];   //son210702 //son210916
    m_edit6.GetWindowText( szText,10);      //son Average % value   
    k  = atoi( szText );
    if(k<0){
        k=0;
    }
    if(k>500){
        k=500;
    }
    tmp2=k;


    if(tmp1!=tmp2){
        m_edit6.GetWindowText( szText,10);  //son Average % value   
        k  = atoi( szText );
        if(k<0){
            k=0;
        }
        if(k>500){
            k=500;
        }
        gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=k;   //son210702 //son210916
        tmp3=1;
    }

    //son "4W_Per_High.ini" 파일에 gd4W1_P2High[][][] 내용을 write
    if(tmp3==1){
        FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21    //son211015 21 -> _SAVEHIGH
    }

    //===============================================
    //########################################################################
    // sigma  

    float  dSigma1[MAX_4W_NET +1];	//son231107 Def_4WMax1 -> MAX_4W_NET +1
    float  dSum1; 
    char   szText[100];

    ::ZeroMemory(&dSigma1, sizeof(dSigma1));

    for (net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
    {
        dSum1=0;
        for(int  k=1; k<=nTotalSampCount1; k++){ 
            // dSum1+=(fData[net-1][k]-fCalc[net][CALC_AVG])*(fData[net-1][k]-fCalc[net][CALC_AVG]);
            dSum1=dSum1+(float)(fData[net-1][k]-fCalc[net][CALC_AVG])*(float)(fData[net-1][k]-fCalc[net][CALC_AVG]); 
        }
        dSum1=dSum1/nTotalSampCount1; 
        dSigma1[net]=(float)sqrt(dSum1);    
    }  


    /////SYLEE181010-1/#########################################################

    int nTmp2,nTmp3,nTmp4;//SYLEE181010-1
    int nTmp32,nTmp42;//sylee181208
    double dA2,dB2;//SYLEE181010-1
    dA2=0.0,dB2=0.0;//SYLEE181010-1

    if(nTotalSampCount1>=5)
    {
        nTmp2=nTotalSampCount1/2-1;
        if(m_check2.GetCheck()) //EJECT COUNT  LOW  HIGH. //SYLEE181010-1/       
        {
            m_edit3.GetWindowText(str);
            nTmp3 =atoi(str);
            if(nTmp3<0.0){
                nTmp3=0;             
            }
            if(nTmp3>nTmp2){
                nTmp3=nTmp2;             
            }
            sprintf(szText, "%d",  nTmp3);//sylee160803-5
            m_edit3.SetWindowText(szText);

            m_edit4.GetWindowText(str);
            nTmp4 =atoi(str);
            if(nTmp4<0.0){
                nTmp4=0;             
            }
            if(nTmp4>nTmp2){
                nTmp4=nTmp2;            
            }
            sprintf(szText, "%d",  nTmp4);//sylee160803-5
            m_edit4.SetWindowText(szText);

            if(nTmp3>0 || nTmp4>0 ){//sylee171103-9
                nType1=1;//sylee171103-9
                gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=1;        //son210916
            }
            else {
                gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=0;        //son210916
            }
        } 
    }

    else 
    {
        gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=0; nTmp3=0; nTmp4=0;      //son210916
        m_edit3.SetWindowText("0");
        m_edit4.SetWindowText("0");
        m_check2.SetCheck(0);
        // AfxMessageBox("    ( 4W Samping Total Count < 5) !    ->    Eject  Low, high =>  uncheck.");    
    }

    //########################################################################
    //sylee160722
    // Sigma 계수

    double  dTmp1;

    if(m_check1.GetCheck()){//Sigma 계수     
        //gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]=1;     //son210916
        g_bSigmaCoef_Enable=1;                          //son220308
        m_edit8.GetWindowText(str); //son Sigma 계수 value
        dTmp1 =atof(str);
        if(dTmp1<0.0){
            dTmp1=0.0;           
        }
        if(dTmp1>9.9){  //son 시그마 계수는 2.5sigma 등과 같이 튜닝을 위해 소수점으로 설정할 수 있어야 한다. 
            dTmp1=9.9;           
        }
        sprintf(szText, "%.1f",  dTmp1);//sylee160803-5
        m_edit8.SetWindowText(szText);  //son Sigma 계수 value
        g_dSigmaCoef = dTmp1;    //son200827-4w_Sigma
    }
    else {
        //gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]=0;     //son210916
        g_bSigmaCoef_Enable=0;                          //son220308
    }

    if(g_dSigmaCoef>0){//sylee171103-9
        nType1=1;//sylee171103-9
    }

    double fDa2[MAX_4W_NET +1];	//son231107 Def_4WMax1 -> MAX_4W_NET +1
    short  nPo1[MAX_4W_NET +1];	//son231107 Def_4WMax1 -> MAX_4W_NET +1
    int nCo1,nCo2;

    /////SYLEE181010-1/#########################################################
    //SYLEE181010-1  if(gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]==1){       //son210916
    if( g_bSigmaCoef_Enable==1 ||  gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]==1 )  //SYLEE181010-1     //son220308
    {
        if(nType1==1){//sylee171103-9
            fprintf(fp2, "  No.,  Pin1,  Pin2,   Pin3,   Pin4,   , " );  //sylee171103-9
            for(   int k2=1; k2<=nTotalSampCount1 ;k2++){
                fprintf(fp2, "    #%d       , ",k2 );  //sylee171103-9
            }
            fprintf(fp2, " \n " );  //sylee171103-9
        }

        for (net=1; net<=nTotalNet1; net++) //son190704-4WDUT16
        {

            if(nType1==1){//sylee171103-9
                fprintf(fp2, " %d,   %d,   %d,   %d,   %d,    , ",
                        net,(int)g_s4WNet[dutIdx].saData[net-1].waPin[0], (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2], (int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  //sylee171103-9
            }

            fCalc[net][CALC_MIN]=fCalc[net][CALC_AVG];//min
            fCalc[net][CALC_AVG]=0.0;//aver
            fCalc[net][CALC_MAX]=fCalc[net][CALC_AVG];//max

            nCount=0;nFirst=0;//sylee150812
            if(g_bSigmaCoef_Enable==1){//SYLEE181010-1     //son210916
                dC1=fCalc_AvgOrg[net];
                dA1=dC1 - (g_dSigmaCoef * (dSigma1[net]));  //son 시그마 하한
                dB1=dC1 + (g_dSigmaCoef * (dSigma1[net]));  //son 시그마 상한
            }
            else {//SYLEE181010-1                        
                dA1=0.0;//SYLEE181010-1
                dB1=0.0;//SYLEE181010-1
            }

            /////SYLEE181010-1/#########################################################
            if( gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]==1 ){//SYLEE181010-1      //son210916
                if(nTmp3>0 || nTmp4>0){
                    ::ZeroMemory(&fDa2, sizeof(fDa2));
                    ::ZeroMemory(&nPo1, sizeof(nPo1));
                    for( int  k21=1; k21<=nTotalSampCount1; k21++){//sorting
                        nCo1=0; nCo2=0;                          
                        for( int k22=1; k22<=nTotalSampCount1; k22++){ 
                            if(nPo1[k22]==1){
                                continue;
                            }
                            if( nCo2==0){
                                fDa2[k21]=fData[net-1][k22];
                                nCo1=k22;
                                nCo2=1;
                            }
                            else {  
                                if(fDa2[k21]>fData[net-1][k22]){
                                    fDa2[k21]=fData[net-1][k22];
                                    nCo1=k22;
                                }
                            }
                        }
                        if(nCo1>0){
                            nPo1[nCo1]=1;
                        }
                    }

                    if(nTmp3>0){
                        dA2=fDa2[nTmp3];
                    }
                    else {
                        dA2=0.0;
                    }
                    if(nTmp4>0){
                        dB2=fDa2[nTotalSampCount1-nTmp4+1];
                    }
                    else {
                        dB2=0.0;
                    }

                    //if(gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]==1)        //son210916
                    if(g_bSigmaCoef_Enable==1)        //son220308
                    {
                        if(dA2>0){
                            if(dA1<dA2){
                                dA1=dA2;
                            }
                        }
                        if(dB2>0){
                            if(dB1>dB2){
                                dB1=dB2;
                            }
                        }                                
                    }
                    else {
                        dA1=dA2;
                        if(dB2==0){
                            dB2=2000000;
                        }
                        dB1=dB2;
                    }
                }
            }
            /////SYLEE181010-1/#########################################################

            nTmp32=0;//sylee181208
            nTmp42=0;//sylee181208

            for(int k=1; k<=nTotalSampCount1; k++){
                if(fData[net-1][k]<dA1){
                    nTmp32++;//sylee181208
                    //fprintf(fp2, "  ," );  //sylee171106-2
                    continue;
                }
                if(fData[net-1][k]>dB1){
                    nTmp42++;//sylee181208
                    // fprintf(fp2, "  ," );  //sylee171106-2
                    continue;
                }
            }

            nTmp32=nTmp3-nTmp32;//sylee181208  //sylee181217-2
            if(nTmp32<0 || nTmp32>nTmp3){//sylee181208
                nTmp32=0;
            }

            nTmp42=nTmp4-nTmp42;//sylee181208 //sylee181217-2
            if(nTmp42<0 || nTmp42>nTmp4){//sylee181208
                nTmp42=0;
            }

            for( k=1; k<=nTotalSampCount1; k++){

                if(fData[net-1][k]<dA1){
                    fprintf(fp2, "  ," );  //sylee171106-2
                    continue;
                }                            
                if(fData[net-1][k]==dA1){
                    if(nTmp32>0){
                        nTmp32--;
                        fprintf(fp2, "  ," );  //sylee171106-2
                        continue;
                    }
                } 
                if(fData[net-1][k]>dB1){
                    fprintf(fp2, "  ," );  //sylee171106-2
                    continue;
                }
                else if(fData[net-1][k]==dB1){
                    if(nTmp42>0){
                        nTmp42--;
                        fprintf(fp2, "  ," );  //sylee171106-2
                        continue;
                    }
                }

                if(nType1==1){//sylee171103-9
                    fprintf(fp2, " %10.3f  ,",fData[net-1][k]);  //sylee171103-9
                } 
                if( fData[net-1][k]<= MAX_4W_R){//sylee200828  100->200 
                    nCount++;                                               
                    //SYLEE181224-1 if(k==1){                       
                    if(nCount==1){ //SYLEE181224-1                      
                        fCalc[net][CALC_MIN]=(float)fData[net-1][k];//min
                        fCalc[net][CALC_MAX]=(float)fData[net-1][k]; 
                        nFirst=1;
                    }
                    else {
                        if( fData[net-1][k]  < fCalc[net][CALC_MIN]){
                            fCalc[net][CALC_MIN]=(float)fData[net-1][k];
                        }
                        if( fData[net-1][k]  > fCalc[net][CALC_MAX]){
                            fCalc[net][CALC_MAX]=(float)fData[net-1][k];
                        }                           
                    } 
                    fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+(float)fData[net-1][k];
                }

            } 

            if(nType1==1){//sylee171103-9
                fprintf(fp2, " \n" );//sylee171103-9
            }                       

            if(nCount>0){
                fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);                               
            }
            else {
                fCalc[net][CALC_MIN]=999999;//sylee150812
                fCalc[net][CALC_AVG]=999999;//sylee150812
                fCalc[net][CALC_MAX]=999999;//sylee150812
            }
        }
    }

    //======================================================================
    // % average

    //sylee160722 if(   (gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]==1)&&(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0)){//sylee160530
    if((g_bSigmaCoef_Enable==0 )        //son220308
            &&(gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]==1)   //son210916
            &&(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0))   //sylee160722   //son210702     //son210916
    {
        if( nType1!=1){//sylee171103-9
            if(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0.1){//sylee171103-9  //son210702 //son210916
                nType1=2;//sylee171103-9
            }
        }

        if(nType1==2){//sylee171103-9
            fprintf(fp2, "  No.,  Pin1,  Pin2,   Pin3,   Pin4,   , " );  //sylee171103-9
            for(   int k2=1; k2<=nTotalSampCount1 ;k2++){
                fprintf(fp2, "    #%d       , ",k2 );  //sylee171103-9
            }
            fprintf(fp2, " \n " );  //sylee171103-9
        }

        for( net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
        {

            fCalc[net][CALC_MIN]=fCalc[net][CALC_AVG];//min
            fCalc[net][CALC_AVG]=0.0;//aver
            fCalc[net][CALC_MAX]=fCalc[net][CALC_AVG];//max

            nCount=0;nFirst=0;//sylee150812
            dC1=fCalc_AvgOrg[net];
            dA1=dC1-(dC1*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    //son210702 //son210916
            dB1=dC1+(dC1*(gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]/100.));    //son210702 //son210916

            if(nType1==2){//sylee171103-9
                fprintf(fp2, " %d,   %d,   %d,   %d,   %d,    , ",
                        net,(int)g_s4WNet[dutIdx].saData[net-1].waPin[0],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  //sylee171103-9
            }


            for(int  k=1; k<=nTotalSampCount1; k++){

                if(fData[net-1][k]<dA1){
                    fprintf(fp2, "  ," );  //sylee171106-2
                    continue;
                }
                if(fData[net-1][k]>dB1){
                    fprintf(fp2, "  ," );  //sylee171106-2
                    continue;
                }                   
                if(nType1==2){//sylee171103-9
                    fprintf(fp2, " %10.3f  ,",fData[net-1][k]);  //sylee171103-9
                }
                if( fData[net-1][k]<= MAX_4W_R){//sylee200828  100->200 
                    nCount++;                               
                    if(k==1){                       
                        fCalc[net][CALC_MIN]=(float)fData[net-1][k];//min
                        fCalc[net][CALC_MAX]=(float)fData[net-1][k]; 
                        nFirst=1;
                    }
                    else {
                        if( fData[net-1][k]  < fCalc[net][CALC_MIN]){
                            fCalc[net][CALC_MIN]=(float)fData[net-1][k];
                        }
                        if( fData[net-1][k]  > fCalc[net][CALC_MAX]){
                            fCalc[net][CALC_MAX]=(float)fData[net-1][k];
                        }                           
                    } 
                    fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+(float)fData[net-1][k];
                }
            }

            if(nType1==2){//sylee171103-9
                fprintf(fp2, " \n" );//sylee171103-9
            } 

            if(nCount>0){
                fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);                               
            }
            else {
                fCalc[net][CALC_MIN]=999999;//sylee150812
                fCalc[net][CALC_AVG]=999999;//sylee150812
                fCalc[net][CALC_MAX]=999999;//sylee150812
            }
        }
    }//end of if(   (gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]==1)&&gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]>0)) //sylee160530 //son210702



    //========================================================================

    int nPassCount1;
    nPassCount1=0;

    for (net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
    {      
        str.Format("%.2f", fCalc[net][CALC_MIN]); //min
        m_grid1.SetTextMatrix(net, 1+_4W_SMP_MIN, str);     //son211005  _4W_REF_LOW -> _4W_SMP_MIN

        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str.Format("%.2f", fCalc[net][CALC_AVG]);//avg
        m_grid1.SetTextMatrix(net, 1+_4W_REF_AVG, str);     //son211005

        str.Format("%.2f", fCalc[net][CALC_MAX]); //max
        m_grid1.SetTextMatrix(net, 1+_4W_SMP_MAX, str);     //son211005 _4W_REF_HIGH -> _4W_SMP_MAX

        for( int k1=1 ; k1<=5; k1++){//sylee170314-2
            if(fCalc[net][CALC_AVG]<dRec4w[k1][3]){
                str.Format(" %d", k1+170);  
                if(fCalc[net][CALC_AVG]> MAX_4W_R){//sylee171122-1
                    str.Format("0");  //sylee171122-1
                }//sylee171122-1
                m_grid1.SetTextMatrix(net, 1+_4W_MODE, str);
                nPassCount1++;
                break; 
            } 
        }   
    }


    if( nPassCount1<nTotalNet1){
        str.Format("\n  Mode 171~175  ?   SET Please!     \n\n     Net Mode?=%d ea , Total Net=%d",  (nTotalNet1-nPassCount1) , nTotalNet1);
        AfxMessageBox(str, MB_OK);          
    }

    fclose(fp2);//sylee171103-9

    //=======================================================================
    //sylee19022-1
    int nCo;

    m_edit2.GetWindowText( szText,10); 
    nSpecPer = atoi( szText );
    if(nSpecPer<0){     nSpecPer=0;  }
    if(nSpecPer>100){   nSpecPer=100;   }
    str.Format("%d" ,  nSpecPer );
    m_edit2.SetWindowText(str);
    if(nSpecPer<=0){ return ; }  


    nCo=0;

    for( net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
    {       
        dA1=(fCalc[net][CALC_MAX]-fCalc[net][CALC_MIN])/fCalc[net][CALC_AVG]*100.0;
        if(nSpecPer>=dA1){
            continue;
        }  
        nCo++;
        if(nCo==1){
            ::ZeroMemory(&fName2, sizeof(fName2));  
            str2.Format("%s\\4W_Setup_A_Spec_NG.csv",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
            strcat( fName2 , str2 );
            fp2=fopen(fName2,"wt");
            if(fp2==NULL){
                str.Format("Error No 1017-3.   %s   ?      ->  File Reading Error ", str2);
                AfxMessageBox(str, MB_OK);          
                return ;
            } 
            fprintf(fp2, " *Net No.,  Pin1 , Pin2, Pin3, Pin4,  ,Min , Aver.,Max., Err.Value, ,Err.Rate \n " );   

        }
        fprintf(fp2, " *%d,   %d,   %d,   %d,   %d,  , ",
                net,(int)g_s4WNet[dutIdx].saData[net-1].waPin[0], (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                (int)g_s4WNet[dutIdx].saData[net-1].waPin[2], (int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);   
        fprintf(fp2, "%.2f,%.2f, %.2f, %.2f,  , %.2f, \n    ",
                fCalc[net][CALC_MIN],fCalc[net][CALC_AVG],fCalc[net][CALC_MAX],fCalc[net][CALC_MAX]-fCalc[net][CALC_MIN],(fCalc[net][CALC_MAX]-fCalc[net][CALC_MIN])/fCalc[net][CALC_AVG]*100.);

    }

    fclose(fp2);

    ::ShellExecute(NULL,"open","EXCEl.EXE",str2,"NULL",SW_SHOWNORMAL);
    //  ::ShellExecute(NULL,"open","notepad.EXE",str2,"NULL",SW_SHOWNORMAL);


    //=======================================================================

    return ;


}




//son dutIdx에 맞는 4W.ini(PinList, Min,Max)를 로딩하고 
//    m_grid와 g_nGridNetCnt, Dut NetCount의 상태를 현재 dutIdx에 맞게 설정한다.
int CModeSub17::Init_grid(int dutIdx)    //son210524
{
    int i;
    CString str; 

    //son210623 FileSysInfo01.LoadSaveSub17(_LOAD): "4W.ini" 로딩을  Init_grid(dutIdx)안에서 수행한다.   
    //son g_s4WNet[dutIdx]에  BDLPath\4W.ini 파일(Pin, Ref.High, Ref, Ref.Low)정보를  로딩
    int ret = FileSysInfo01.LoadSaveSub17(_LOAD);// type 1,2 ,  mode 1~20  //file loading
    if( ret <= 0 )
        return ret;
 

    //son210623
    //son DUT 변경시에 dutIdx에 맞게 4W.ini를 로딩하고 m_grid와 DUT NetCount의 상태를 설정한다.
    //    FileSysInfo01.LoadSaveSub17(_LOAD)를 수행하고  Init_grid(dutIdx)를 수행해야 함.
    g_nGridNetCnt=100;  //son210623
    int net=(int)g_s4WNet[dutIdx].wCount;
    if(net<=MAX_4W_NET){
        g_nGridNetCnt=net;//sylee200604 //son210623
        if(net<20)
            g_nGridNetCnt=20;   //son210623
    }
    else{
        g_nGridNetCnt=MAX_4W_NET;   //son210623
    }


    //-----------------------
    //son Set DUT Net Point
    
    //son 해당 DUT의 NetCount 갯수 출력
    str.Format("%d",(int)g_s4WNet[dutIdx].wCount);  //son190704-4WDUT16
    m_LabelPoint.SetCaption(str);

    //str.Format("%d", dutId);      //son-4WDUT16-20200318
    //m_LabelDut.SetCaption(str);   //son200708: Label 삭제. Label대신 ComboBox 추가



    //-----------------------
    //son Init Grid
    m_grid1.Clear();    
    m_grid1.SetRows(g_nGridNetCnt+1);   //son 전체 행수 지정. //son210623  
    m_grid1.SetCol(0);  //son 0번 열로   이동
    m_grid1.SetRow(0);  //son 0번 행으로 이동 


    //son 맨 왼쪽 열에 No를 출력------
    for ( i=1 ; i<=g_nGridNetCnt; i++)  //son210623
    { 
        m_grid1.SetCol(0); //son 0번 열 선택
        m_grid1.SetRow(i); //son i변 행 선택
        //str.Format("NO.%d", i);   //son211005
        str.Format("%d", i);     //son211005  Low, High, Min, Max로 항목을 늘리기 위해  grid cell 폭을 줄이기 위해 NO. string은 삭제
        m_grid1.SetText(str);  
    }  

    COLORREF colorRed, colorPink, colorRightGreen, colorBlue, colorBlack, colorRightYellow, colorGreen;

    colorRed         = RGB(200,0,20);       //son 빨강       
    colorPink        = RGB(255,200,228);    //son 분홍 
    colorRightGreen  = RGB(225,255,164);    //son 더연한 연두색 
    colorBlue        = RGB(9,73,172);       //son 남색
    colorBlack       = RGB(0,10,50);        //son 검정
    colorRightYellow = RGB(252,254,211);    //son 연노랑
    colorGreen       = RGB(175,252,150);    //son 초록

    //son ActiveX 컨트롤문제보완.  NetCount가 1000개를 넘으면 색상변경에 시간이 너무 
    //    오래 걸리므로  1000개 이하일 때에만 색상을 다르게 설정한다.
    if(g_nGridNetCnt<=1000){    //son210623
        m_grid1.SetForeColor(colorBlack);           //son 전체 Cell의 글자를 블랙으로
        m_grid1.SetBackColor(colorRightYellow);     //son 전체 Cell의 배경은 연노랑

#ifdef __GTS_4W_SPCIALNET__ 
        m_grid1.SetColColor(1+_4W_SPECIAL,   colorBlue, colorPink);            //son221111 글자:남색  배경: 분홍 
#endif
        m_grid1.SetColColor(1+_4W_PIECE,     colorBlue, colorPink);            //son210916 글자:남색  배경: 분홍 
        m_grid1.SetColColor(1+_4W_PIN1,      colorRed,  colorRightGreen);      //son210916 글자:빨강  배경: 더연한 연두색
        m_grid1.SetColColor(1+_4W_PIN2,      colorRed,  colorRightGreen);      //son210916
        m_grid1.SetColColor(1+_4W_PIN3,      colorRed,  colorRightGreen);      //son210916   
        m_grid1.SetColColor(1+_4W_PIN4,      colorRed,  colorRightGreen);      //son210916 

        m_grid1.SetColColor(1+_4W_REF_LOW,   colorBlue, colorPink);            //son211005 글자:남색  배경: 분홍   
        m_grid1.SetColColor(1+_4W_REF_AVG,   colorBlue, colorPink);            //son211005            
        m_grid1.SetColColor(1+_4W_REF_HIGH,  colorBlue, colorPink);            //son211005
        m_grid1.SetColColor(1+_4W_SMP_MIN,   colorRed,  colorRightGreen);      //son211022 글자:빨강  배경: 더연한 연두색
        m_grid1.SetColColor(1+_4W_SMP_MAX,   colorRed,  colorRightGreen);      //son211022 글자:빨강  배경: 더연한 연두색
        m_grid1.SetColColor(1+_4W_MODE,      colorRed,  colorRightGreen);//MODE//son210916 글자:빨강  배경: 더연한 연두색

        m_grid1.SetColColor(1+_4W_TEST_MIN,  colorRed,  colorGreen);           //son210916 글자:빨강  배경: 초록
        m_grid1.SetColColor(1+_4W_TEST_AVG,  colorRed,  colorGreen);           //son210916
        m_grid1.SetColColor(1+_4W_TEST_MAX,  colorRed,  colorGreen);           //son210916
    }
    else{ 
        m_grid1.SetBackColor(colorGreen);   //son 전체 Cell의 배경을 초록으로
    }


    //----------------------------------------
    //son Grid 헤더를 설정파일에 맞게  설정

    gDef_Language1=1;
 

    str.Format("Net"); 
    m_grid1.SetTextMatrix(0, 0, str);

#ifdef __GTS_4W_SPCIALNET__
    str.Format("S=1"); 
    m_grid1.SetTextMatrix(0, 1+_4W_SPECIAL,    str);    //son2221107
#endif

    str.Format("Pice"); 
    m_grid1.SetTextMatrix(0, 1+_4W_PIECE,    str);      //son211005
    str.Format("Pin1");                      
    m_grid1.SetTextMatrix(0, 1+_4W_PIN1,     str);      //son211005
    str.Format("Pin2");                      
    m_grid1.SetTextMatrix(0, 1+_4W_PIN2,     str);      //son211005
    str.Format("Pin3");                      
    m_grid1.SetTextMatrix(0, 1+_4W_PIN3,     str);      //son211005
    str.Format("Pin4");                      
    m_grid1.SetTextMatrix(0, 1+_4W_PIN4,     str);      //son211005
    str.Format("RefLow");                    
    m_grid1.SetTextMatrix(0, 1+_4W_REF_LOW,  str);      //son211005
    str.Format("RefAvg");                    
    m_grid1.SetTextMatrix(0, 1+_4W_REF_AVG,  str);      //son211005
    str.Format("RefHigh");                   
    m_grid1.SetTextMatrix(0, 1+_4W_REF_HIGH, str);      //son211005

    str.Format("SmpMin");                   
    m_grid1.SetTextMatrix(0, 1+_4W_SMP_MIN,  str);      //son211005
    str.Format("SmpMax");                   
    m_grid1.SetTextMatrix(0, 1+_4W_SMP_MAX,  str);      //son211005

    str.Format("Mod");                      
    m_grid1.SetTextMatrix(0, 1+_4W_MODE,     str);      //son211005
    str.Format("TestMin");                  
    m_grid1.SetTextMatrix(0, 1+_4W_TEST_MIN, str);      //son211005
    str.Format("TestAvg");                  
    m_grid1.SetTextMatrix(0, 1+_4W_TEST_AVG, str);      //son211005
    str.Format("TestMax");                  
    m_grid1.SetTextMatrix(0, 1+_4W_TEST_MAX, str);      //son211005
   

    if(SysSet211.m_nSet13==1){//sylee160907-1   //chinese
        gDef_Language1=3;//redboard
    }
    else {//sylee160907-1
        gDef_Language1=2; //englishos
    }//sylee160907-1

    //son 데이터 영역 No 헤더 출력 (13번째 col 부터 data 개수만큼)
    for (  i= 1+_4W_DATA1; i<=NUM_COL1+1 ; i++)    //son240622 defCol1 -> 21: NUM_COL1 
    { 
        str.Format("NO. %d", i-_4W_DATA1); 
        m_grid1.SetTextMatrix(0, i, str);
    }    
    m_grid1.Refresh();
    m_grid1.SetRedraw(TRUE);
    m_nCurRow = 1;
    m_nCurCol = 1;
    m_grid1.GetWindowRect(m_rectGrid);
    ScreenToClient(m_rectGrid);  //son  그 위치를 현재 작업 영역에 대한 좌표로 변경
    

    //son twip을 pixel로 변환하기 위한 비율.  보통 1 pixel = 15 twip이지만 해상도에 따라 달라 질 수 있음.   
    //    m_rectGrid.Width()는 pixel 단위이고 MSFlexGrid의 단위는 twip 단위이다.
    //    현재 m_rectGrid witdh가 796 pixel 이므로 796: x = 1 : 15,    x = 15*796 => 11940
    //int width = m_rectGrid.Width(); //son220928
    //son221111 m_dRatio = double(m_rectGrid.Width())/11940.;	//sylee211121   //son220928 11960 -> 11940
    m_dRatio  = double (1 / 15.);   //son221111  4W grid 크기변경관련. 변환비율 고정으로 변경함.


    OnEditing = FALSE;

    return 1;
}

#ifdef __GTS_4W_SPCIALNET__
//son221107 해당 net을 특수Net으로 화면에 표시한다.
void CModeSub17::SetOn_NetSpecial(int net)
{
    int dutIdx = get4wDutIdx();
    //g_s4WNet[dutIdx].saData[net-1].bNetSpecial = 1;

    //son 해당 row, 첫번째 No 컬럼의 배경색을 연한 노란색으로 특수넷 표시 한다.
    COLORREF colorBlack, colorRightYellow;
    int row= net;

    m_grid1.SetCurCell(row, 0);                 
    colorBlack       = RGB(0,10,50);        //son 검정    
    colorRightYellow = RGB(252,254,211);    //son 연노랑
    m_grid1.SetCellForeColor(colorBlack);       
    m_grid1.SetCellBackColor(colorRightYellow); 

    
}
#endif

#ifdef __GTS_4W_SPCIALNET__
//son221107 해당 net을 특수Net이 아닌 일반 Net으로 화면에 표시한다.
void CModeSub17::SetOff_NetSpecial(int net)
{
    int dutIdx = get4wDutIdx();
    //g_s4WNet[dutIdx].saData[net-1].bNetSpecial = 0;

    //son 해당 row, 첫번째 No 컬럼의 배경색을 특수넷이 아닌 일반 4WNet으로 되돌린다. 
    COLORREF colorDarkGray, colorWhite;
    int row = net;

    m_grid1.SetCurCell(row, 0);                 
    colorDarkGray = RGB(128,128,128);    //son 진한 회색    
    colorWhite    = RGB(255,255,255);    //son 흰색 
    m_grid1.SetCellForeColor(colorWhite);    
    m_grid1.SetCellBackColor(colorDarkGray); 
}
#endif


//son210623
//son auto sampling 결과인  "4W_Setup_A.txt" 을 dutIdx에 맞게   read하여 float fData[]로 임시 저장한다.
//    fData[net][] 에서 min, max, avg를 구해 fCalc[net][] 에 저장하고 
//    Min, Max만 제외한 fCalc[net][CALC_AVG] 값을 따로 fCalc_AvgOrg[net]에 보관
//    Sigma계산, Grid 출력은 하지 않는다.
int CModeSub17::_Load_Auto4WData(int dutIdx)      //son210524   //son210623
{

    CString str, str1;

    FILE *fp;
    char  fName[FILE_PATH_SIZE] ; //son210625 200-> FILE_PATH_SIZE
    ::ZeroMemory(&fName, sizeof(fName)); 
    if (dutIdx == 0)
        str1.Format("%s\\4W_Setup_A.txt",(SysInfoView01).m_pStrFilePathBDL );//sylee141212
    else    //son200320-4WDUT16: Multi DUT 구조로 수정.
        str1.Format("%s\\4W_Setup_A.txt%d",(SysInfoView01).m_pStrFilePathBDL, dutIdx+1 );
    //str1.Format("%s\\4W_Setup_A.txt",(SysInfoView01).m_pStrFilePathBDL );
    
    strcat( fName , str1 );
    fp=fopen(fName,"rt");
    if(fp==NULL){
        str.Format("Error No 1017-5.   File:%s.\n Does the File not Exist? Reading Error.\n", fName);  
        //AfxMessageBox(str, MB_OK);    //son220209 위준명 과장요청. flle reading error 메시지 삭제.
        //                                          이 파일이 없는게 에러 상황은 아니므로 에러 출력은 하지 말라함.
        MyTrace(PRT_BASIC, str);        //son220209 로그 출력추가.
        return 0;
    }


    //son "4W_Setup_A.txt"에서 nTotalSampCount1(측정횟수)와 nTotalNet1(Net 갯수)를 Read
    nTotalSampCount1=0;
    nTotalNet1=0;//sylee160521  
    char    buf[GET_BUF_BIG_SIZE];  //son210624 20*2048으로 원상복구 (4W_Setup_A.txt 대량 Count로딩 실패 문제)

#ifdef __GTS__CUSTOMER  
    //son211112 for Unitech. 4W_Setup_A.txt에 첫줄에  BarCode 정보 추가 
    ::ZeroMemory(&buf, sizeof(buf)); 
    fgets(buf, sizeof(buf), fp);    //son210524 512*4 -> sizeof(buf)
    sscanf(buf, " %10d,%10d,",  &nTotalSampCount1, &nTotalNet1); 
    
#else
    //son 첫줄 읽기  
    fscanf(fp, " %10d,%10d,\n",  &nTotalSampCount1, &nTotalNet1); 
    fscanf(fp, "\n");
#endif

    if(nTotalSampCount1>1000){//sylee170705
        str1.Format("Error No 1017.   %s  4W SET  file    test total count(=%d)  > 1000 over error    ", 
                                str1, nTotalSampCount1);
        AfxMessageBox(str1, MB_OK); 
        fclose(fp);      
        return 0;
    }
    if(nTotalSampCount1<0){//sylee160521 
        nTotalSampCount1=0;    
    }

    if(nTotalNet1 > MAX_4W_NET){//sylee160812-1  2000->5000   //son-4WDUT-20200403: 5000-> MAX_4W_NET
        str1.Format("Error No 1017.   %s  4W SET  file    Net total count(=%d)  > %d over error", 
                            str1, nTotalNet1, MAX_4W_NET);
        AfxMessageBox(str1, MB_OK); 
        fclose(fp);      
        return 0;
    }
    if(nTotalNet1<0){ //sylee160521 
        nTotalNet1=0;      
    }


    //=====================================================================
    //son auto sampling으로 4W auto test 한 결과인  "4W_Setup_A.txt" 의
    //     string data 를 read하여 float fData[]로 임시 저장한다.
    int     net;
    int     no; 

    ::ZeroMemory(&fData, sizeof(fData));

    //son 두번째 줄을  net== -1 로 읽고, net>=0부터 처리하므로 첫번째 항목은 n==0이 된다. 
    //    즉, fData[0] 부터 채워짐.
    for (net=-1;  net<=nTotalNet1;net++)    //SYLEE170125       //son190704-4WDUT16
    {
        ::ZeroMemory(&buf, sizeof(buf));    //son211112
        fgets(buf, sizeof(buf), fp);    //son210524 512*4 -> sizeof(buf)
        if(feof(fp))
            break;

        str.Format("%s",buf);   //son char -> Cstring으로 변환

        //son -5부터 시작해서 "Net1,  PIN,   2899,   2900,   4947,   4946, " 이런부분 건너뛰고 " 1.76," 이런 sampling 값을 read한다.
        for( int k=-5;  k<= nTotalSampCount1;k++)  //sylee170114
        {
            no=str.Find(',');           
            str.Left(no);

            if(k<0){//sylee170114
                str=str.Mid(no+1);
                continue;//sylee170114
            }//sylee170114

            if( net<0){
                continue;
            }
            fData[net][k]=(float)atof(str);
            str=str.Mid(no+1);

        }
    }


    fclose(fp);


    //=======================================================================
    //son fData[net][] 에서 min, max, avg를 구해 fCalc[net][] 에 저장한다. 

    int nCount,nFirst;
    ::ZeroMemory(&fCalc, sizeof(fCalc)); 
    ::ZeroMemory(&fCalc_AvgOrg, sizeof(fCalc_AvgOrg));   //sylee160530-1    //son210623

    for (net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
    {
        fCalc[net][CALC_MIN]=0.0;//min
        fCalc[net][CALC_AVG]=0.0;//aver
        fCalc[net][CALC_MAX]=0.0;//max

        nCount=0;nFirst=0;//sylee150812

        for(int  k=1; k<=nTotalSampCount1; k++)
        {
            if( fData[net-1][k]<= MAX_4W_R)  //sylee200828  100->200  ohm
            {
                nCount++;
                //if(nFirst==0){                        
                if(k==1){                       
                    fCalc[net][CALC_MIN]=(float)fData[net-1][k];//min
                    fCalc[net][CALC_MAX]=(float)fData[net-1][k]; 
                    nFirst=1;
                }
                else {
                    //son Min 값 구하기
                    if( fData[net-1][k]  < fCalc[net][CALC_MIN]){
                        fCalc[net][CALC_MIN]=(float)fData[net-1][k];
                    }

                    //son Max 값 구하기
                    if( fData[net-1][k]  > fCalc[net][CALC_MAX]){
                        fCalc[net][CALC_MAX]=(float)fData[net-1][k];
                    }                           
                } 

                //son fCalc[net][CALC_AVG]에는 일단 fData를 계속 sum 해 간다.
                fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+(float)fData[net-1][k];
            }
        }
#if 0   //son211202_1 주석처리 
        if(nCount>=3){//sylee150721
            //son 3개 이상의 유효값이 있다면 최대값, 최소값을 제외하고 평균을 낸다.
            fCalc[net][CALC_AVG]=(fCalc[net][CALC_AVG]-fCalc[net][CALC_MAX]-fCalc[net][CALC_MIN])/(float)(nCount-2);//sylee150721
        }
        else if(nCount>0){
            fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);                               
        }
#else
        //son211202_1 기본 Average는 최대 최소 제외 없이 구한다. 
        //    fCalc[net][CALC_AVG], fCalc[net][CALC_SIGMA] 모두 기본 계산시에는 필터링 계산없이 구하기로 결정. with 이상윤부장님.
        //    211201 이전에 Sigma는 최대, 최소 제외없이 구하고, Avg는 최대, 최소 제외하고 구하는 불일치 문제가 있었음.
        if(nCount>0){
            fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]/(float)(nCount);//son211202_1
        }

#endif
        else {
            fCalc[net][CALC_MIN]=999999;//sylee150812
            fCalc[net][CALC_AVG]=999999;//sylee150812
            fCalc[net][CALC_MAX]=999999;//sylee150812
        }

        //son 현재 fCalc[net][CALC_AVG] 값을 fCalc_AvgOrg[net]에 보관
        fCalc_AvgOrg[net]= (double)fCalc[net][CALC_AVG];//sylee160530-1    //son210623
    }


    //########################################################################
    // sigma  
    
    //son 새로 로딩한 4W fData[net-1]로 Sigma data를  계산한다.
    
    //son211202_3 OnButton_Load_Auto4WData()에서 _Load_Auto4WData(int dutIdx)로 Sigma 계산 부분 이동. 
    //          Sigma 계산 위치를 한곳으로 몰아서 코딩 실수 방지.

    double  dSum1, dVar; 
    ::ZeroMemory(&fCalc_SigmaOrg, sizeof(fCalc_SigmaOrg));    //son211202_6 g_dSigma1->fCalc_SigmaOrg
    for(   net=1; net<=nTotalNet1 ;net++)       //son190704-4WDUT16
    {
        dSum1=0;
        for(int  k=1; k<=nTotalSampCount1; k++)
        { 
            //son 편차의 제곱을 더해 간다.
            // dSum1+=(fData[net-1][k]-fCalc[net][CALC_AVG])*(fData[net-1][k]-fCalc[net][CALC_AVG]);
            dSum1 = dSum1 + (double)(fData[net-1][k]-fCalc[net][CALC_AVG]) * (double)(fData[net-1][k]-fCalc[net][CALC_AVG]); 
        }
        if ((nTotalSampCount1) > 0)   //son211005    
        {
            dVar=dSum1/(double)nTotalSampCount1;        //son 편차제곱의 평균 (분산)을 구한다.      
            fCalc[net][CALC_SIGMA]=(double)sqrt(dVar);    //son 표준편차(Sigma)를 구한다.   //son211202_3 
            fCalc_SigmaOrg[net]=fCalc[net][CALC_SIGMA];    //son 현재 fCalc[net][CALC_SIGMA] 값을 fCalc_SigmaOrg[net]에 보관  //son211202_3
        }
    }  

    return 1;
}

#if 0
//son211202_3 Sigma 계산기능을  _Load_Auto4WData() 내부로 이동하고  
//          Load_Auto4WData() 기능을  _Load_Auto4WData()로 일원화.  Load_Auto4WData()는 삭제함.

//son dutIdx에 맞는  기존 4W AutoSampling Test 결과(4W_Setup_A.txt)를 로딩하고 sigma 도 다시 계산한다.
//    Grid 출력은 하지 않는다.
void CModeSub17::Load_Auto4WData(int dutIdx)      //son210524   //son210623
{


    //---------------------------------------------------------------------
    //son dutIdx에 맞는  기존 4W AutoSampling Test 결과(4W_Setup_A.txt)를 로딩.  
    //    Sigma계산, Grid 출력은 하지 않는다.
    int ret = _Load_Auto4WData(dutIdx);   //son210623
    if (ret <= 0)
        return;



    //########################################################################
    // 3 sigma 
    
    //son 새로 로딩한 4W Data에 대해  Sigma 계수를 적용해서 Avg를 다시 계산한다.

    //low data average sigma
    float   dSum1; 
    //int     nSigmaCoef;  //son Sigma 계수.  보통 3 ? 
    //char   szText[50];
    CString str;

    ::ZeroMemory(&fCalc_SigmaOrg, sizeof(fCalc_SigmaOrg));

    for (int net=1; net<=nTotalNet1; net++)     //son190704-4WDUT16
    {
        dSum1=0;
        for(int  k=1; k<= nTotalSampCount1; k++)
        { 
            //son 편차의 제곱을 더해 간다.
            dSum1=dSum1+(float)(fData[net-1][k]-fCalc[net][CALC_AVG])*(float)(fData[net-1][k]-fCalc[net][CALC_AVG]); 
        }
        dSum1=dSum1/nTotalSampCount1;   //son 편차제곱의 평균(분산)을 구한다.
        fCalc_SigmaOrg[net]=(float)sqrt(dSum1);    //son 표준편차(Sigma)를 구한다.

        //---------------------------------------
        //son 'Sigma 계수' Enable이면
        
        if(m_check1.GetCheck())
        {
            //son 의미없는 코드.  코멘트처리로 삭제. 
            //son211202_3 if(gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]<0){  //low data  sigma  enble check        //son210916
            //    gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=0;        //son210916
            //}
            //if(gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]>6){        //son210916
            //    gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=6;        //son210916
            //}

            //nSigmaCoef =atoi(str);      //son210623
            //if(nSigmaCoef<0 ){
            //  nSigmaCoef=0;            
            //}
            //if(nSigmaCoef>6){
            //  nSigmaCoef=6;            
            //}

            //son 시그마 avg *  sigma  값만큼 이동한 avg 상한 값을 새로운 avg로 설정한다. 
            //son211202_3 fCalc[net][CALC_AVG]=fCalc[net][CALC_AVG]+ (g_dSigmaCoef  * fCalc_SigmaOrg[net] );// 3sigma
        }
        //---------------------------------------


    }
}
#endif


 

void CModeSub17::OnButton105()   //stop
{   
 
    nStop=2;
}




void CModeSub17::OnButton108()   //low % set   6->5
{   
 
    double  dt2,dLowPer1; 
    CString str; 
   
    m_edit3.GetWindowText(str);
    dLowPer1=atof(str);
    if( dLowPer1<=0){
        dLowPer1=0;
    }
    else if( dLowPer1>100){
        dLowPer1=100;
    }

    str.Format("%3.1f",dLowPer1); 
    m_edit3.SetWindowText(str);

    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16

    //for( j=1; j<=defRow1; j++){
    for (int net=1; net<=g_s4WNet[dutIdx].wCount; net++)        //son190704-4WDUT16
    {

        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str=m_grid1.GetTextMatrix(net, 1+_4W_REF_AVG);      //son211005
        dt2=atof(str);
        dt2=dt2-(dt2/100.0*dLowPer1);

        str.Format("%3.3f",dt2);         //son211015  %10.2f -> %.2f 공간확보를 위해
        m_grid1.SetTextMatrix( net, 1+_4W_REF_LOW, str);    //son211005
        fCalc[net][CALC_REF_LOW] = dt2;                       //son211015 Ref.Low Raw Data 저장기능 추가
          
    }

    DoEvents(); 
}



void CModeSub17::OnButton109()   //High % set
{   

    double  dt2,dHighPer1;
    CString str;
     
    m_edit5.GetWindowText(str);
    dHighPer1=atof(str);
    if( dHighPer1<=0){
        dHighPer1=0;
    }
    else if( dHighPer1>10000){
        dHighPer1=10000;
    }

    str.Format("%3.1f",dHighPer1); 
    m_edit5.SetWindowText(str);

    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16
    for(int net=1; net<=g_s4WNet[dutIdx].wCount; net++)                 //son190704-4WDUT16
    {

        //son g_s4WNet[dutIdx].saData[net-1].dRefAvg  값에 매칭되는 화면값
        str=m_grid1.GetTextMatrix(net, 1+_4W_REF_AVG);      //son211005
        dt2=atof(str);
        dt2=dt2+(dt2/100.0*dHighPer1);

        /////////////////////////////////////////////////////           
        /*if(m_check2.GetCheck()){//sylee150713
            if( dt2<=6){//testmode//sylee150713
                dt2=6;
            }
            else if(dt2<=10){//testmode//sylee150713
                dt2=10;
            }
        }
        */

        //sylee150805

        int nA1,nA2,nA3;
        char szText[100];

        m_edit21.GetWindowText(str);
        nA1=atoi(str);
        if( nA1<1){
            nA1=0;
            sprintf(szText, "%d",  nA1);
            m_edit21.SetWindowText(szText); 
        }
        else if( nA1>100){
            nA1=100;
            sprintf(szText, "%d",  nA1);
            m_edit21.SetWindowText(szText); 
        }

        m_edit22.GetWindowText(str);
        nA2=atoi(str);
        if( nA2<1){
            nA2=0;
            sprintf(szText, "%d",  nA2);
            m_edit22.SetWindowText(szText); 
        }
        else if( nA2>100){
            nA2=100;        
            sprintf(szText, "%d",  nA2);
            m_edit22.SetWindowText(szText); 
        }

        m_edit23.GetWindowText(str);
        nA3=atoi(str);
        if( nA3<1){
            nA3=0;
            sprintf(szText, "%d",  nA3);
            m_edit23.SetWindowText(szText); 
        }
        else if( nA3>100){
            nA3=100;                
            sprintf(szText, "%d",  nA3);
            m_edit23.SetWindowText(szText); 
        }

        if(m_check2.GetCheck() &&  m_check3.GetCheck()){//sylee150713
            if( dt2<=nA1){ 
                dt2=nA1;
            }
            else if(dt2<=nA2){ 
                dt2=nA2;
            }
        }
        else if(m_check2.GetCheck()){ 
            if( dt2<=nA1){ 
                dt2=nA1;
            } 
        }
        else if(m_check3.GetCheck()){ 
            if(dt2<=nA2){ 
                dt2=nA2;
            }
        }

        if(m_check4.GetCheck()){ 
            dt2=dt2+nA3;
        }


        /////////////////////////////////////////////////////
        str.Format("%3.3f",dt2);         //son211015  %10.2f -> %.2f  공간확보를 위해
        m_grid1.SetTextMatrix( net, 1+_4W_REF_HIGH, str);       //son211005
        fCalc[net][CALC_REF_HIGH] = dt2;                       //son211015 Ref.High Raw Data 저장기능 추가

    }

    DoEvents(); 
 
}
 



//son200724: 4W-SamplingCount 비번입력기능  관련 신규추가 함수
extern int nFlag_PassOn;
extern int nFlag_PassOnNo;
void CModeSub17::OnSetfocusEdit7() 
{
    // TODO: Add your control notification handler code here
    
#ifdef __GTS__CUSTOMER      //son211214_2 GTS만 4W-SamplingCnt 비번 기능 적용
#ifndef __MST__CUSTOMER      //son220308
    
    //son Map > Set1 > 'No Password' Off
    if (SysSet13.m_nSet6!=1)  //son230829 
    {
        UpdateData(TRUE);

        // focus가 edit박스에 있을 때 1회만 Dialog 박스를 생성하도록  Flag 처리한다.
        if(g_bPassDlgOpen == true) 
            return;

        g_bPassDlgOpen = true;      //son Dialog 생성중임을 표시 

        nFlag_PassOnNo=13;   //son C:\ACE500\Data\Message_L.ini의 103 번 항목의 passwd를 체크한다.
        g_PasswdDlg.DoModal(); // g_PasswdDlg Dialog 안에서 g_bPassDlgOpen 값이 변경된다.


        // 틀린 passwd이면 SetFocus()를 취소한다.
        //if (g_passDlgLogOnUser < 5)    
        if (nFlag_PassOn!=2) //son 103번 passwd와 사용자가 입력한 passwd가 같았다면 2가 설정됨.
        {
            AfxMessageBox("  Invalid Password! ", MB_OK ); //son Passwd error 출력

            SetFocus();      // 현재 edit box에 대한 Focus를 취소하는 효과가 된다.  
                             // ModeSub17의 SetFocus()를 호출해서 Focus를 돌려 준다.
        }      
        
        // 올바른 passwd 이면 현재 Focus가 유지되어서 edit 가 가능해 진다.

        g_bPassDlgOpen = false;     //son Dialog 종료를 표시 
        UpdateData(FALSE);
    }
#endif  //son220308
#endif
}


//son200724: 4W-SamplingCount 비번입력기능  관련 신규추가 
//son n4w_Sample_SetCount 값을 file 보존 & Read하기 위한 함수
bool CModeSub17::LoadSave_SamplingSigmaInput(int type)
{
//son230710 #ifdef __GTS__CUSTOMER      //son211214_2 GTS만 4W-SamplingCnt 비번 기능 적용
#if 1   //son230710 제이텍에서 시그마 저장계수 상황을 유지 시켜 달라는 요구사항. 
    FILE    *fp; 
    char    fName[FILE_PATH_SIZE]; //son210625 200-> FILE_PATH_SIZE
    CString str;


    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\4WSetupData.txt", g_sFile.DataDir);
    strcat( fName, str);

    switch(type)
    {
        case _SAVE:
            fp = fopen(fName,"wt");         
            if(fp == NULL){ 
                str.Format(" Error No 2001 !\n%s  file Data Saving Error !     file close!", fName); //error messege
                AfxMessageBox(str, MB_OK); //error messege
                return false;
            }   
            fprintf(fp, "%10d\n",   n4w_Sample_SetCount); 
            fprintf(fp, "%10.1f\n", g_dSigmaCoef);         //son200827-4w_Sigma
            fprintf(fp, "%10d\n",   (int)g_bSigmaCoef_Enable); //son200827-4w_Sigma
            //fprintf(fp, "%10d\n",   n4w_Sample_Enable);   //son <=이건 회수 다 채우면 풀려야 하므로 file에 저장하지 말자.  

            fclose(fp);
            break;


        case _LOAD: 
            fp=fopen(fName,"rt");
            if(fp == NULL){ 
                //son 처음 로딩할 때는 파일이 없는 것이 정상이므로 굳이 로딩시 fail은 출력하지 않고 리턴만 한다.
                //str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!", fName); //error messege
                //AfxMessageBox(str, MB_OK);
                return 0;
            }

            fscanf(fp, "%10d\n", &n4w_Sample_SetCount); 
            fscanf(fp, "%s\n", str);             //son200827-4w_Sigma
            g_dSigmaCoef = atof(str);
            //fscanf(fp, "%d\n",   &n4w_Sample_Enable); //son <=이건 회수 다 채우면 풀려야 하므로 file에 저장하지 말자.

            int     nSigmaCoef_Enable;
            fscanf(fp, "%d\n",   &nSigmaCoef_Enable);        //son200827-4w_Sigma
            g_bSigmaCoef_Enable  = (BOOL)nSigmaCoef_Enable; 

            fclose(fp);
            break;


        default:
            break;
    }

#endif

    return true;
    
}


 
//son 'Set' button      // 4W Setting Sub Dialog을 띄운다.
void CModeSub17::OnButton_Open4wLimitSubDlg()   //sylee160503
{                            //son220308 OnButton110 -> OnButton_Open4wLimitSubDlg

    g_flag_LimitPasswdOk = 1;   //son221118 초기화가 빠져서 GTS가 아닌 고객은 모두 4W LimitSet을 못하게 된 문제 수정함.
                                //          1으로 초기화해서 GTS가 아닌 경우에는 LimitSet 화면을 설정 가능하게 한다.

//son220914_2 begin: GTS요구사항. 4W LimitSet 화면 설정시 비번 추가
#ifdef __GTS__CUSTOMER      
    
    //---------------------------
    //son Passwd 다이얼로그 
    //---------------------------
    UpdateData(TRUE);

    nFlag_PassOnNo=13;   //son C:\ACE400\Data\Message_L.ini의 103 번 항목의 passwd를 체크한다.
                         //    4W Sampling Count와 동일한 비번을 설정한다.
    
    g_PasswdDlg.DoModal(); // g_PasswdDlg Dialog 안에서 g_bPassDlgOpen 값이 변경된다.


    //son 틀린 passwd이면 전역변수 g_flag_LimitPasswdOk를  0으로 ModeSub18에 전달해서
    //    LimitSet 화면의 LowCheck, HighCheck 박스를 disable하여 edit못하게 한다.
    if (nFlag_PassOn!=2) //son 103번 passwd와 사용자가 입력한 passwd가 같았다면 2가 설정됨.
    {
        AfxMessageBox("Invalid Password!\n\n Set the screen to read-only.", MB_OK ); //son Passwd error 출력

        g_flag_LimitPasswdOk = 0;  
    }      
    else
        //son 1으로 설정해서 LimitSet 화면을 설정 가능하게 한다.
        g_flag_LimitPasswdOk = 1;  
    

    UpdateData(FALSE);
#endif  
//son220914_2 end
 
    int k, prevVal, newVal, valChanged;

    //son "4W_Per_High.ini" 파일을 gd4W1_P2High[][][]에 load한다.
    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load   22    //son211015 22 -> _LOADHIGH


    //son 'Average % Use On' 저장값과 현재 상태를 비교
    valChanged=0;
    prevVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_USE];     //son210702   //son210916
    if(m_check6.GetCheck()){            //son 'Average % Use' On
        newVal=1;      
    }
    else {
        newVal=0;      
    }
    if(prevVal!=newVal){
        if(m_check6.GetCheck()){        //son 'Average % Use' On
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;               //son210702 //son210916
        }
        else {
            gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;               //son210702 //son210916
        }
        valChanged=1;
    }

    //son 'Average % edit value' 저장값과 현재 상태를 비교
    prevVal=(int)gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL];   //son210702 //son210916
    m_edit6.GetWindowText( szText,10);          //son 'Average % use'의 edit box %값을 가져 온다.
    k  = atoi( szText );
    if(k<0){
       k=0;
    }
    if(k>500){
       k=500;
    }
    newVal=k;
    
    if(prevVal!=newVal){
        m_edit6.GetWindowText( szText,10);  //son Average % value   
        k  = atoi( szText );
        if(k<0){
           k=0;
        }
        if(k>500){
           k=500;
        }
        gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=k;   //son210702 //son210916
        valChanged=1;
    }

    //son "4W_Per_High.ini" 파일에 gd4W1_P2High[][][] 내용을 write
    if(valChanged==1){
       FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save  21   //son211015  21-> _SAVEHIGH
    }

//===============================================

    //-------------------------------
    //son SET Ref Limit 다이얼로그 
    //-------------------------------
    CModeSub18 ModeSub18;
    ModeSub18.DoModal(); 
}




BOOL CModeSub17::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
 
    /*
    if( i<0)
         i=0;
    else if( i<120)
         i=i+1;
     
    else 
        i=120;
    
    if(zDelta==-120){ //down
        SendMessage(VK_NEXT,0,0);
        m_grid1.SetScrollTrack(i);  
        DoEvents();
        m_grid1.SetScrollTrack(i);
    }
    else if(zDelta==120){//up    
 
    }
*/
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}



void CModeSub17::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // TODO: Add your message handler code here and/or call default
    
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
 






//son I range에 맞게 raw R 값 계산.  Cal 반영 안 된 AD 기준
int CModeSub17::ARun201_4w_R1(double nISet)//sylee150710
{  

    if(Sub71.Y1<0.0000001){
        Sub71.Y1=0.0000001;
    }               

    double nT1,nT2;  //4WSETUP140107

    nT1=nISet-nISet*0.1;
    nT2=nISet+nISet*0.1;

    if(SysSet13.m_n4WCVMode!=1){//sylee200217-4WCVMODE
        if(nT1<Sub71.Y1){
            if(Sub71.Y1<nT2){
                Sub71.Y1=nISet; //SYLEE140107
            }
        }
    }

#if 1
    if(Sub71.Y1>0)
    {
        if( nIR==1 || nIR==2){//I mA 
            //sylee150709  Sub71.R1=((Sub71.W1)/Sub71.Y1);// /1000   4W=>  4W VOLTAGE
            Sub71.R1=Sub71.W1;  //sylee150709 
        }
        else if( nIR==3 || nIR==4  || nIR==5 ){  //I uA
            //sylee150709  Sub71.R1=((Sub71.W1)/Sub71.Y1)*1000.;// /1000    
            Sub71.R1=Sub71.W1*1000.;// /1000    //sylee150709 
        }
        else {
            Sub71.R1=9999; 
        }       
    }
    else {
        Sub71.R1=9999; 
    }

#else
//ChildView7.cpp 와 동일한 코드
    if(Sub71.Y1>0)
    { 
        if( nIR==1 ||  nIR==2 ){//sylee161207-0      
            Sub71.R1=((Sub71.W1)/Sub71.Y1)*10.;//sylee161207-0  x10  ray ok 
        }
        else if( nIR==3 ||  nIR==4 || nIR==5){  //SYLEE150904 //ACE400
            Sub71.R1=((Sub71.W1)/Sub71.Y1)*1000.;//배율 /1000   
        }
        else{
            Sub71.R1=9999999999; 
        }       
    }
    else{
        Sub71.R1=99999999999; 
    }

#endif
    Sub71.R1=Sub71.R1*1000.;   //R1;



    return 1;

}


//son220419_3 
//  Glabals.cpp의 전역함수 ARun201_4w_R_Cal1()를 ChildView7과 공통으로 사용하도록 수정.
// ModeSub17::Run201_4w_R_Cal1()는 코멘트처리함.
#if 0  

//son Cal 반영한 Real R (ohm)값 추정: Display 용        
//    Raw R -> Real R
int CModeSub17::ARun201_4w_R_Cal1(int m_nMode1, int part)  //sylee150710    //son220412 part추가
{

    int nPos, nRListCnt;
    double aa,ab;
    nPos=0;
    aa=0.0;
    ab=0.0;

    nRListCnt=(int)gdCal1[part][m_nMode1][0][0];// Cal  list su


    //      Sub71.R1=dR14w[j+5][i]; 

    if(  (Sub71.R1>0) &&  (nRListCnt>0) &&   (nRListCnt<=MAX_CAL_RLIST) )   //son220408 128: MAX_CAL_RLIST
    {
        //son220407  에러????  0부터 시작하는 것 이상하다.  gdCal1[part][m_nMode1][][RList]에서 RList는 1부터 채워지는데? 
        //           2W용 RCalCovert()나  RCalConvert_short()와 다름.
        //           실제 값은 RList 1부터 gdCal1[part][m_nMode1][][RList]가 채워져 있지만
        //           4W는 1 ohm 이하로 측정되는 경우가 있어서 1 ohm 이하의 경우는 RList=0 인 경우와 기울기로 판단한다.
        
        //son 해당 모드의 Calibration Ref 값들 중에서 현재 측정R에 맞는 값을 찾아서  nPos로 표시.
        for (int RList=0;  RList < nRListCnt;  RList++)  // 4WSETUP       
        {
            //son 'Use=1'인 경우
            if(gdCal1[part][m_nMode1][CAL_USE][RList]==1)  //gdCal1[part][m_nMode1][CAL_USE][RList]  cal enable 1,0 
            {
                // ex) 4W Open 관련된 Cal값은 오름차순 정렬이므로 측정한 Raw ADC R 값이  740이라면 
                //     RList=1 에서는  gdCal1[part][m_nMode1][CAL_REF_RAWR][1]   740 > 732.85 ?  True , nPos=1
                //     RList=2 에서는  gdCal1[part][m_nMode1][CAL_REF_RAWR][1]이 740 > 770.85 ?  False,  
                //     이후 모든 RList가 해당이 없으므로 nPos=1로  표시된다.
                //            (CALR51B.ini)   ohm Real R       ADC Raw R   
                //            RList=1          1.00             732.85              
                //            RList=2          2.00             770.64              
                //            RList=3          5.00             918.51              

                //son 측정한 R 값이 해당 모드의 모든 RList R 값중의 하나보다 작다면 nPos로 표시한다.
                if( Sub71.R1< gdCal1[part][m_nMode1][CAL_REF_RAWR][RList])     //son220407  3: CAL_REF_RAWR
                {
                    nPos = RList;            //son 해당 RList 위치를 nPos로 기록하고
                    RList = nRListCnt+1;    //son nRListCnt 만큼 바로 증가시켜서 for 문을 탈출
                } 
            }
        }                       

        //son 첫번째 R
        if(nPos==0)       //gdCal1[part][m_nMode1][CAL_REF_R][k]  cal ref  R 
        {
            //v=nCalC[m][1][k];
            if(gdCal1[part][m_nMode1][CAL_REF_RAWR][0]>0.0)
            {
                //sylee20120112 Sub71.R1=((gdCal1[part][m_nMode1][CAL_REF_RAWR][k]-Sub71.R1)/gdCal1[part][m_nMode1][CAL_REF_RAWR][k])*gdCal1[part][m_nMode1][CAL_REF_R][k];
                if(gdCal1[part][m_nMode1][CAL_REF_R][0]<=0.0){  //[2:CAL_REF_R] cal. real r
                    Sub71.R1= 0.0 ; //sylee20120112
                }
                else {
                    //-------------------------
                    //son ADC R -> real ohm R
                    //-------------------------
                    //son  Real R / Ref Real R  == 측정 Raw R / Ref Raw R  임을 이용해서  실제 R 값을 추정한다.
                    Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][CAL_REF_RAWR][0])*gdCal1[part][m_nMode1][CAL_REF_R][0]; 
                } 

            }
        }
        else 
        {
            if( (gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos]-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1])!=0.0){
                aa=(Sub71.R1-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1])/(gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos]-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1]);  // [3]= lookup table Calibration Raw R
                ab=gdCal1[part][m_nMode1][CAL_REF_R][nPos]-gdCal1[part][m_nMode1][CAL_REF_R][nPos-1];// [2]= lookup table Calibration real R
                Sub71.R1=ab*aa+gdCal1[part][m_nMode1][CAL_REF_R][nPos-1];                                          
            }
            else {
                Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1])*gdCal1[part][m_nMode1][CAL_REF_R][nPos-1];    
            }
        }

        if(nPos==nRListCnt){//sylee20120115
            //nProStep1[w][2]=(int)(((SysInfoSer13.m_nData[nMod1][2][RList]-nProStep1[w][1])/SysInfoSer13.m_nData[nMod1][2][RList])*SysInfoSer13.m_nData[nMod1][3][RList]);
            Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos])*gdCal1[part][m_nMode1][CAL_REF_R][nPos]; 
        }   
    }  


    //   dR14w[j+5][i]=Sub71.R1; 

    //############################################################################################
    //==============================================================================================================

    //son R 값이 0.1 이하이면 171은 0.1,  171이외에는 9999 로 설정.
    if(Sub71.R1<0.1){
        if(m_nMode1==171){  
            Sub71.R1=0.1;
        }
        else {
            Sub71.R1=9999;
        }
    }



    return 1;

}
#endif


//son 4W.ini를 로드하고  Error 여부를 체크
int Check_4WErrorNet()   //sylee171122 //son231019_2 OnErrorNet1() -> Check_4WErrorNet()
{


    
    //son Recipe step 중에 4W 가 없으면 진행할 필요 없다.
    //if(n4WEnableRun==1)//sylee210317
    if (nRecipe_4W_Use != 1)    //son231020 n4WEnableRun-> nRecipe_4W_Use로 통일 
        return -1;

    FileSysInfo01.LoadSaveSub17(_LOAD); //4w.ini load  //sylee160518

    CString str1,str;

    //son241014 4w.ini 에 0이 포함되었을 때  AutoTest화면  경고  출력 기능 추가. (김완수 대표님 지시사항)
    //          혹시 잘못된 경우인지를 체크하기 위한 경고일뿐 Net Count가 0이어도 관련 기능은 정상 동작함.
    for (int dut = 0; dut < nMultyNetTotal; dut++)
    {
        if (g_s4WNet[dut].wCount == 0)
        {
            str.Format("\nWarning! DUT_%d 4W Net List is 0! \n\nCheck your BDL if this situation is normal.\n", dut+1); 
            errMessageBox(17026, str);
        }
    }

    
    //son 프린트 출력하지 않고 nErrCo1만 획득
    int nErrCo1 = __Check_4WErrorNet(false, NULL);

    //son Error가 없으면 굳이 file 출력하지 않고 nErrCo1만 return.
    if (nErrCo1 <= 0)   //son231020
        return nErrCo1;


    //son Error가 있으므로 file 출력.
    FILE *fp = NULL;
    char  fName[FILE_PATH_SIZE] ; //son210625 200-> FILE_PATH_SIZE

    str1.Format("%s\\4W_Setup_ErrorNet1.txt",(SysInfoView01).m_pStrFilePathBDL ); 

    ::ZeroMemory(&fName, sizeof(fName)); 
    strcat( fName , str1 );
    fp=fopen(fName,"wt");
    if(fp==NULL){
        str.Format("Error No 1019.   %s   ?      ->  File Reading Error ", str1);
        AfxMessageBox(str, MB_OK);          
        return -1;
    }

    if(nErrCo1>0){
        fprintf(fp, "\n\n\n   Can't process 4w Auto test. Don't Use this 4W.ini BDL!!!!  \n\n");
        fprintf(fp, "\n\n\n   ===> 4W SETUP:  Error Mode   Net Total :  => %d        \n\n\n",nErrCo1 );          
    }


    //son file 출력 수행.
    nErrCo1 = __Check_4WErrorNet(true, fp);

    fclose(fp); 


    //son file 자동 Open
    if(nErrCo1>0){
        ::ShellExecute(NULL,"open","notepad.EXE",str1,"NULL",SW_SHOWNORMAL);
    }



    return nErrCo1;

}

//son231020 begin
int __Check_4WErrorNet(bool bPrintOn, FILE *fp)
{
    int nErrCo1 = 0, net; 


    int dutIdx = get4wDutIdx();         // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16

    /*
    if( g_s4WNet[dutIdx].wCount>=0) //son241009 0도 처리하게 
    { 
        for( net=1 ; net<=g_s4WNet[dutIdx].wCount; net++)
        { 
            if( g_s4WNet[dutIdx].saData[net-1].n4wMode<171 ||  g_s4WNet[dutIdx].saData[net-1].n4wMode>175)
            {
                nErrCo1++;
                if(nErrCo1==1)
                    fprintf(fp, "\n\n *> Ng Net.     Mode       (   Pin1,   Pin2,   Pin3,   Pin4  )       \n\n" );   
                                    
                fprintf(fp,         " *> Net%-4d,   %3d ,      (  %5d,  %5d,  %5d,  %5d )   \n  ",
                        net, (int)g_s4WNet[dutIdx].saData[net-1].n4wMode,
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[0],(int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  
            }
        }
    }

*/

    if (g_s4WNet[dutIdx].wCount >= 0)  //son190704 //son241009 0도 처리하게 
    { 
        for( net=1 ; net<=g_s4WNet[dutIdx].wCount; net++)       //son190704-4WDUT16
        { 
            //------------------------
            //son 4W Mode 오류 확인 
            if( g_s4WNet[dutIdx].saData[net-1].n4wMode<171 ||  g_s4WNet[dutIdx].saData[net-1].n4wMode>175)
            {
                nErrCo1++;
                if(bPrintOn == true && nErrCo1==1){
                    fprintf(fp, " \n\n *> Ng Net.        Mode        Ref.Low.      Ref.High.     (   Pin1,   Pin2,   Pin3,   Pin4  )       \n\n" );   
                }                   
                if (bPrintOn)
                    fprintf(fp, "   =>  Net%-4d,       *%d (171~175) ,    %d ,   %d ,    (  %5d,  %5d,  %5d,  %5d )   \n",
                        net, (int)g_s4WNet[dutIdx].saData[net-1].n4wMode,
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefLow, (int)g_s4WNet[dutIdx].saData[net-1].dRefHigh,  //son211005
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[0],(int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  
            }
            //-------------------------------
            //son 4W Ref Val Range Check  1
            else  if( g_s4WNet[dutIdx].saData[net-1].dRefLow> MAX_4W_R && g_s4WNet[dutIdx].saData[net-1].dRefHigh> MAX_4W_R) //son211005   //son231019 220000 -> MAX_4W_R 
            {
                nErrCo1++;
                if(bPrintOn == true && nErrCo1==1){
                    fprintf(fp, " \n\n *> Ng Net.     Mode        Ref.Low.    Ref.High.   (   Pin1,   Pin2,   Pin3,   Pin4  )       \n\n" );   
                }                   
                if (bPrintOn)
                    fprintf(fp, "   =>  Net%-4d,        %d ,    *%d(>%dohm),   *%d(>%dohm) ,    (  %5d,  %5d,  %5d,  %5d )     \n",
                        net, (int)g_s4WNet[dutIdx].saData[net-1].n4wMode,
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefLow,  (int)(MAX_4W_R)/1000, //son231019
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefHigh, (int)(MAX_4W_R)/1000,   //son211005   //son231019
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[0], (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2], (int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  

            }
            //-------------------------------
            //son 4W Ref Val Range Check  2
            else  if( g_s4WNet[dutIdx].saData[net-1].dRefLow> MAX_4W_R) //son211005 //son231019 220000 -> MAX_4W_R
            {
                nErrCo1++;
                if(bPrintOn == true && nErrCo1==1){
                     fprintf(fp, " \n\n *> Ng Net.     Mode        Ref.Low.    Ref.High.   (   Pin1,   Pin2,   Pin3,   Pin4  )      \n\n" );   
                }                   
                if (bPrintOn)
                    fprintf(fp, "   =>  Net%-4d,        %d ,    *%d(>%dohm) ,   %d ,    (  %5d,  %5d,  %5d,  %5d )   \n",
                        net, (int)g_s4WNet[dutIdx].saData[net-1].n4wMode,
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefLow,  (int)(MAX_4W_R)/1000, //son231019
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefHigh,   //son211005
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[0], (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  

            }
            //-------------------------------
            //son 4W Ref Val Range Check  3
            else  if( g_s4WNet[dutIdx].saData[net-1].dRefHigh> MAX_4W_R)  //son211005  //son231019 220000 -> MAX_4W_R
            {
                nErrCo1++;
                if(bPrintOn == true && nErrCo1==1){
                    fprintf(fp, " \n\n *> Ng Net.     Mode        Ref.Low.    Ref.High.   (   Pin1,   Pin2,   Pin3,   Pin4  )       \n\n" );   
                }                   
                if (bPrintOn)
                    fprintf(fp, "   =>  Net%-4d,        %d ,     %d ,   *%d(>%dohm) ,    (  %5d,  %5d,  %5d,  %5d )   \n",
                        net, (int)g_s4WNet[dutIdx].saData[net-1].n4wMode,
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefLow, //son211005 dSetMin->dRefLow, dSetMax -> dRefHigh
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefHigh,  (int)(MAX_4W_R)/1000, //son231019      
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[0], (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  

            }

            else  if( g_s4WNet[dutIdx].saData[net-1].dRefHigh<= 0)  //son211005  //son231019 220000 -> MAX_4W_R
            {
                nErrCo1++;
                if(bPrintOn == true && nErrCo1==1){
                    fprintf(fp, " \n\n *> Ng Net.     Mode        Ref.Low.    Ref.High.   (   Pin1,   Pin2,   Pin3,   Pin4  )       \n\n" );   
                }                   
                if (bPrintOn)
                    fprintf(fp, "   =>  Net%-4d,        %d ,     %d ,   *%d(<=0) ,    (  %5d,  %5d,  %5d,  %5d )   \n",
                        net, (int)g_s4WNet[dutIdx].saData[net-1].n4wMode,
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefLow, //son211005 dSetMin->dRefLow, dSetMax -> dRefHigh
                        (int)g_s4WNet[dutIdx].saData[net-1].dRefHigh,  (int)(MAX_4W_R)/1000, //son231019      
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[0], (int)g_s4WNet[dutIdx].saData[net-1].waPin[1],
                        (int)g_s4WNet[dutIdx].saData[net-1].waPin[2],(int)g_s4WNet[dutIdx].saData[net-1].waPin[3]);  

            }

        }
    }

    return nErrCo1;
}
//son231020 end

//son220419_7
void CModeSub17::OnCHECKLowerCalJig() 
{
	// TODO: Add your control notification handler code here
	
	CString str, str2;  
	
    if(m_checkLowerCalJig.GetCheck())      
    {
        if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)    
        {
            str.Format("Error No 2114 !\n\nSysSet13.m_nUseCalData=UPPER_ONLY.\nCan't turn on this \"Use LowerCalJig\" CheckBox.\n\n");
            str2 += str;
            str.Format("First of all, change the Menu->MAP->Set1->UsaCalData to \"Upper_Lower\".\n\n");
            str2 += str;
            MyTrace(PRT_LEVEL1, str2);
            AfxMessageBox(str2, MB_OK);

            g_bUseLowerCalJig_4W=0;   // 하부핀 R 모드 off
            m_checkLowerCalJig.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
        else 
        {
            //son 4W Setup 창에서는 OPEN-READ.BDL 의 4W.ini가 아니면 하부 CalJig 모드를 on 할 수 없다.
            if (strcmp(SysInfoView01.m_pStrFileBDLName, "OPEN-READ.BDL") != 0)
            {
                str.Format("Error No 2115 !\n\n Current BDL(%s) is not \"OPEN-READ.BDL\" \nCan't turn On \"Use LowerCalJig\" CheckBox.\n\n", 
                                        SysInfoView01.m_pStrFileBDLName);
                str2 += str;
                str.Format("First of all,  Load \"OPEN-READ.BDL for CalJig. \n");
                str2 += str;
                MyTrace(PRT_LEVEL1, str2);
                AfxMessageBox(str2, MB_OK);

                g_bUseLowerCalJig_4W=0;   // 하부핀 R 모드 off
                m_checkLowerCalJig.SetCheck(0);
                UpdateData(FALSE);      // UI에 값을 반영한다.

            }
            else
            {
                //son 하부가 없는 PinMode일때에는 하부핀 R 측정모드를 Off해야 한다.
                if (IS_NO_LOWER(SysSet12.m_nPinTotMax)) 
                {   
                    CString str;
                    str.Format("Error No 11203, PinMode:%s has no lower pin. Can't use LowerCalJig for 4W Measure.\n", 
                            STR_MAX_PIN(SysSet12.m_nPinTotMax));
                    MyTrace(PRT_LEVEL1, str);
                    AfxMessageBox(str);

                    g_bUseLowerCalJig_4W=0;   // 하부핀 R 모드 off
                    m_checkLowerCalJig.SetCheck(0);
                    UpdateData(FALSE);      // UI에 값을 반영한다.
                }   
                else
                    g_bUseLowerCalJig_4W=1; 
            }
        }
    }
    else {
        g_bUseLowerCalJig_4W=0; 
    } 
	
    OnButton_Load();    
}


//son221111_2 begin: 모든 Net 특수넷 표시  
#ifdef __GTS_4W_SPCIALNET__	
void CModeSub17::OnCheck_AllSpecial() 
{
    int net;
	int dutIdx = get4wDutIdx();

    if(m_check_AllSpecial.GetCheck())      
    {
        for (net=1; net<=g_s4WNet[dutIdx].wCount; net++)     
        {
            m_grid1.SetTextMatrix(net, 1+_4W_SPECIAL, "1");
            SetOn_NetSpecial(net);
        }

    }
    else {
        for (net=1; net<=g_s4WNet[dutIdx].wCount; net++)     
        {
            m_grid1.SetTextMatrix(net, 1+_4W_SPECIAL, "0");
            SetOff_NetSpecial(net);
        }
    } 
    UpdateData(FALSE);

}
#endif
//son221111_2 end
