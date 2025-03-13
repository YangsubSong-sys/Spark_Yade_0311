// ChildView1.cpp : implementation file
//
 
#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView1.h"
#include "FileSysInfo.h"
#include "MainFrm.h"
#include "Gnet1.h"
//#include "print.h"

#include "ModeSet13.h" //sylee150407
#include "A_Define1.h"//sylee161126
#include "PassWord1.h"//sylee210621
#include "SaveAsNewBdl.h"   //son220228

#include <String.h>
#include <Math.h>//sylee150309
#include <io.h>//sylee150817

//#include "ModeSet16.h"//sylee150203
 
#include "ChildView7.h"         //son201216 ChildView7과 공통사용하는 g_ComiDaq2 초기화를 위해 추가
#include "Comi_Dio_InOut.h"     //son230914 Comi DIO 관련 define을 전용 헤더로 분리해서 include.
#include "Globals.h"            //son240705

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined(__ACE500__) || defined(__ACE550__)
    #define BDL_PATH    "C:\\ACE500\\BDL\\" //son201118
#elif defined (__ACE1000__)
    #define BDL_PATH    "C:\\ACE1000\\BDL\\" //son210514
#else
    #define BDL_PATH    "C:\\ACE400\\BDL\\" //sylee150922-1
#endif


int nJigTestOpen,nJigTestShort;//sylee181008-1

//son201216  ChildView7과 공통으로 사용하는 g_ComiDaq2 사용전 초기화를 위해 추가
extern CChildView7*     g_pChildView7;       
extern int g_nCurDioCmd;	//son231018
extern int g_nPrevDioCmd;	//son231018
 
//extern int nErrPin_Fileload1;//SYLEE170810-1



int nFlagFileLoadok;//sylee160519       //son  1: BDL File Load OK.  
                                        //        OnButton1001()->OnDataLoad()->OnDataLoad_Type_A() 수행 후
                                        //        OnButton1001()->OnButton1002()->OnDataSave()에서 아무에러 없으면 1로 설정.
                                        //        OnButton1001()->OnButton1002()->ReadFileB(1)에서 BDL fopen() 성공시 1로 설정
                                        //
                                        //     0: OnDataLoad_Type_A() ->LoadBDLFile()  수행 첫부분에 0으로 초기화
                                        //son -1: OnDataLoadAuto() 시에 fopen() 실패했다는 의미로 -1 설정
                                        
int nFlagFileLoad2;//sylee180406-1      //son 1: OnDataLoad_Type_A() 수행 직전에 0으로, 수행 직후 1로 설정. 
                                        //       'MainFrm'등 외부클래스에서 'BDL File Load' 여부를 체크하는 용도로 쓰인다. 
                                                
int gFlag_Start_Int_bdlError;//sylee170717      //son 값이 1이면 BDL 파일 에러.

int nFlag_FileDialogOn;         //son201216: FileDialog가 열려 있는데 BDL 자동로딩 메시지 수신시 다운문제 관련 추가
                                //  1: BDL CFileDialog 관련 로딩 진행중

int  gDef_Language1; //SYLEE151028   //1=KOREA ,2=ENGLISH, 3=CHINA

TCHAR szTSetup2Name[200];  //sylee121022


extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1
extern CString LoadMessage2(UINT uFileType, CString strFile, UINT uNum);//sylee180731-1
 
extern CSysSet12  SysSet12;//sylee150203
extern CSysSet13  SysSet13;  //sylee150407

short nPinC1[DEF_MAX_PIN2][2];  //sylee150922-1   //sylee151111add
            //son190705
            //son nPinC1[0] : .CON 파일 헤더
            //son           nPinC1[0][0]: .CON 파일의 총 pin 수,  nPinC1[0][1]: .CON 파일의 checkSum 
            //
            //son nPinC1[1]~nPinC1[DEF_MAX_PIN2-1]까지는 data
            //son           nPinC1[i][0]: pinBlockPinNo(임시), swPin     
            //              nPinC1[i][1]: 사용자 con jigPinNo       
            //
            //son .FPN이 없다면 .CON 파일도 없다.(내용 동일, "" 만 뺌) 
            //son  이 경우는 PinBlockNo와 사용자 jigPinNo가 동일하며 BDL파일의 PinNo와 ReadFile의 PinNo가 같음
            //
            // CChildView1::OnDataLoad_READ_ConRead()에서 read함.
            //
            //    ex) fscanf(fp, "%10d  %10d   ", &nConTotal1,&nConCheckSum1);      //son 총핀수,  checkSum
            //      for(  i=1; i<=nConTotal1; i++){  //FPN EXIT
            //          fscanf(fp, "%10d %10d   ", &nPinC1[i][0],&nPinC1[i][1]);    //son pinBlockPinNo,  사용자PinNo(JigPinNo#) 
            //      }



CString m_strFileName;//sylee150922-1

int nMultyNetTotal;//sylee150817
short nData2[DEF_MAX_PIN2][6];//sylee150305//sylee150305   KUNSHAN  //sylee150917-2 16400->33000
extern CSysSet16  SysSet16;  //SETUP //sylee130325   //estimation
extern CSysSet19  SysSet19;  //SETUP //sylee150417-1  //estimation
extern int nProOneP_Flag1;//sylee130916  1=load   2= model make   3= run  etc  disable   //sylee131221
extern int nProOneP_ModelCount1;//sylee130916  
extern int nProOneP_ModelCheck1;//sylee131018
extern int nProOneP_ModelCheck1Retry;//sylee131018
//extern  int nProOneP_ModelSet;//sylee130916
extern short nProOneP_ShortPin[301][1000];//SYLEE161212   30->101
extern short nProOneP_Short1_Li[5001][2];
extern short nProOneP_Short1[5001];//process
extern int nProOneP_Short1_Co;
extern short nProOneP_Open1[5001][2];//process
extern int nProOneP_Open1_Co;
extern int nProOneP_RunFlag1;//sylee131011
extern short  OnPinConvertFlag1; //sylee150922-1    //son .CON 파일이 존재하면 1
extern short  OnPinConvertType1; //sylee150922-1    //son .CON 파일이 의미있으면 1 (total이 1이상, checksum 정상)
extern void ReadPinPieceNo(int nDUT);//sylee180810-1-DUT16
extern  int g_nPieceMax1;//sylee121202
extern int g_nPinPieceNo[MAX_DUT1][DEF_MAX_PIN1];//SYLEE180810-1-DUT1 //son220824_2 DefDutMax -> MAX_DUT1(33)
extern int PinData[DEF_MAX_PIN1][10]; 
extern int PinDataP1[DEF_MAX_PIN1][10];  //piece sort
extern int PinDS1[DEF_MAX_PIN1][5]; 
extern int nPcb1[DEF_MAX_PIN1][2];       //PCB NO , PIN NO
extern int PinDS1R1[DEF_MAX_PIN1];
extern int nRsModeC1;
extern int nRsModeR1O;
extern int nRsModeR1S;
extern int nAutoClear1; 
extern CSysInfoView01  SysInfoView01;
extern CFileSysInfo  FileSysInfo01; 
extern CSysInfoPieceRange  SysInfoPieceRange;
extern CSysInfoPiece       SysInfoPiece;        //son210430 추가. Piece.ini와 PieceRange.ini 불일치시 SW 다운 문제 보완
extern CSysInfo05  SysInfo05;

extern int nBDLk;

extern int nSelf1_RunPin1;

extern int nAutoProcess1;//sylee130325
int  nAutoStartInterlock100;//sylee050308
int  nHpin,nLpin;//sylee050309
short PinSearchPin[DEF_MAX_PIN1];    //sylee130518-1 pin search bdl
                                    //son PinSearchPin[hwPin] : 해당 hwPin이 bdl 파일에 존재한다면 1이 셋팅됨.
                                    //            ChildView1.cpp 2275라인(OnDataLoad_Type_A() 내부)에서 설정

int nTotalPin;//sylee130522
int nFlagEstModelMake;//sylee130325    //estimation
int nFlagEstModelMakeSelect;//sylee130325    //estimation
int nFlagEstModelMakeCount;//sylee130325    //estimation

int g_nSelfAutoFlag1;//sylee120903  //son-32KPIN-20190902:  nSelfAutoFlag1 -> g_nSelfAutoFlag1 로 변경
int nT1Net;//SYLEE120902
int nNetList1[Def_SwitchMaxPin];//SYLEE120902       //son Net 수량 갯수
                                                    //son nNetList1[1] : 단독핀  Net 개수
                                                    //son nNetList1[2] : 2 핀으로 구성된 net 갯수 
                                                    //son nNetList1[3] : 3 핀으로 구성된 net 갯수
                                                    // ...
                                                    //son nNetList1[32] : 32 핀으로 구성된 net 갯수
                                                    // ...                                                  
                                                    
int nWarn1;


//son190705
//son ex) BDL 파일 
//20000                     
//32    32  0               
//32
//264   1   0   1   2   1   
//  393 1   0   1   2   1 

///////////////////////////////////////
int nBDLHead1[MAX_DUT1][5][20];     //son220824_2
        //son nBDLHead1[nDut][1][1])        ex) 20000       (BDL 파일 표시) 
        //son nBDLHead1[nDut][2][1])        총핀수, 
        //son nBDLHead1[nDut][2][2])        상부핀수    
        //son nBDLHead1[nDut][2][3])        하부핀수    
        //son                           ex) 32  32  0   (총핀수, 상부핀수, 하부핀수)
        //son nBDLHead1[nDut][3][1])        ex) 32   (총핀수)

short nBDLData1[MAX_DUT1][DEF_MAX_PIN1][10];//raw data          //son220824_2
        //son190705
        //son DEF_MAX_PIN1 :switch의 max pin 수
        //son pinIdx : Pin-List 순서 (1부터 시작)
        //son nDut   : dut 번호
        //
        //son nBDLData1[nDut][pinIdx][1]    PinNo, 0부터 시작 : nBDLHead1[nDut][3][1] 은 이  pin 라인의 갯수임
        //son nBDLData1[nDut][pinIdx][2]    NetNo
        //son nBDLData1[nDut][pinIdx][3]    0 고정수 
        //son nBDLData1[nDut][pinIdx][4]    1 고정수 
        //son nBDLData1[nDut][pinIdx][5]    해당 Net의 PinCount
        //son nBDLData1[nDut][pinIdx][6]    1고정. 사용여부 (0:미사용, 1:사용, 2:SpecialNet) 
        //
        //son nBDLData1[nDut][pinIdx][7]    Pin에 해당하는 Piece Num   (g_nPinPieceNo[]를 이용해서 획득.BDL 파일엔 없는 항목)
 
//son OPEN 검사 관련 BDL 데이터
//    : 구조가 nBDLData1과 비슷하나, netPinCnt부터 순서가 다름. 
//      대표핀이 아닌 pin만 write된다.   결과적으로 openList갯수 + shortList 갯수 => BDL 핀수가 된다.
short nBDLData2[MAX_DUT1][DEF_MAX_PIN1][10];//open data         //son220824_2
        //son   nBDLData2[nDut][0][1]=j;  // OPEN BDL 총 갯수  //SYLEE20111212
        //
        //son   nBDLData2[nDut][openPin][1]=nFirstPin;          // 대표핀 (0~)
        //son   nBDLData2[nDut][openPin][2]=nBDLData1[1][i][1]; // PinNo  (0~)
        //son   nBDLData2[nDut][openPin][3]=nBDLData1[1][i][5];  // PIECE OPEN  : Net의 총핀 수 (netPinCount) 
        //son   nBDLData2[nDut][openPin][4]=nBDLData1[1][i][6];  // PIECE       : (1 고정수)


//son SHORT 검사 관련 BDL 데이터
//    : 구조가 nBDLData1과 동일함.  대표핀만 write된다.  
//      대표핀만 write되므로 shortList 갯수는 Net 갯수와 동일하다.
short nBDLData3[MAX_DUT1][DEF_MAX_PIN1][10];//short data        //son220824_2
        //son piece별로 재배치하기 전의  Short data (piece 1개 상태)
        //son nBDLData3[nDut][0][1]         : 1 piece일때 Short 총수
        //son nBDLData3[nDut][shortPin][1]  : shortPin pinNo       (from nBDLData1[1][i][1])
        //son nBDLData3[nDut][shortPin][5]  : shortPin netPinCount (from nBDLData1[1][i][5])
        //son nBDLData3[nDut][shortPin][7]  : shortPin Piece       (from nBDLData1[1][i][7])


//son  nBDLData31[][][], nBDLData2Op[][][]을  ChilcView1.cpp-> Globals.cpp로 이동.  
//     extern선언과 추가된 Class의 definition은 Globals.h에 정의함.



short nBDLPinT1[DEF_MAX_PIN1];//sylee190104
        //son  BDL 로딩된 pin+1 값을 index로 하는 BDL 테이블 
        //     BDL 로딩시에 pin을 index로 접근해서  1로 표시한다. 
        
short nBDLPinT2[DEF_MAX_PIN1];//sylee190104
        //son nBDLPinT2[nTPin1] : nBDLPinT1[nTpin1]의 역 data. 
        //    nBDLPinT2[1] ~ nBDLPinT2[BDL갯수] : 현재 로딩된 Pin+1 값의 리스트. 올림차순 

short gnBDLData32[MAX_DUT1][DEF_MAX_PIN1][3];//short2 index //sylee230526  //sylee230817

short gnBDLData33[MAX_DUT1][DEF_MAX_PIN1][3];//short2 data  //sylee230526  //son230905_2 10001 -> DEF_MAX_PIN1     
short nBDLData3P[Def_MaxPiece3][10];//short data  
short nBDLData31P[Def_MaxPiece3][DEF_MAX_PIN2];//short data  //SYLEE120911 
 extern CSysSet211  SysSet211;  //Pin SETUP   
 int nADCx5;//sylee180611-1
 extern int nSelf1_SetStartPin;//sylee190105
 extern int nSelf1_RunEnable;//sylee190105
 extern float g_fSparkRefData[DEF_MAX_PIN1];    // hdpark220511
 extern short g_nSparkRefDataCount[DEF_MAX_PIN1]; // hdpark220511

 extern SPARK_AI    g_tSparkAI[DEF_MAX_PIN1];          //hdpark240102

// CChildView1

IMPLEMENT_DYNCREATE(CChildView1, CFormView)

CChildView1::CChildView1()
    : CFormView(CChildView1::IDD)
{
    //{{AFX_DATA_INIT(CChildView1)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CChildView1::~CChildView1()
{

}

void CChildView1::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChildView1)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_LABEL1,  m_LabelPath);
    DDX_Control(pDX, IDC_LIST1, m_list1);
    DDX_Control(pDX, IDC_LIST2, m_list2);  //SYLEE150817
    DDX_Control(pDX, IDC_LIST3, m_list3);  //SYLEE150817
    DDX_Control(pDX, IDC_LIST4, m_list4);  //SYLEE150817
    DDX_Control(pDX, IDC_LABEL2,  m_Label2);
//  DDX_Control(pDX, IDC_LIST2, m_list2);
    DDX_Control(pDX, IDC_LABEL3,  m_Label3);
//  DDX_Control(pDX, IDC_LIST3, m_list3);
    DDX_Control(pDX, IDC_LABEL4,  m_Label4);
    DDX_Control(pDX, IDC_LABEL15,  m_LabelDut); //SYLEE150817
    DDX_Control(pDX, IDC_LABEL_59,  m_LabelLotNameTitle); //son220124
    DDX_Control(pDX, IDC_LABEL141,  m_LabelLotName); //son220124
//  DDX_Control(pDX, IDC_LIST4, m_list4);

//  DDX_Control(pDX, IDC_LABEL101,  m_Label101); 
    DDX_Control(pDX, IDC_BUTTON1001, m_button1001); 
    DDX_Control(pDX, IDC_BUTTON3001, m_button3001); 
    DDX_Control(pDX, IDC_BUTTON_NEWBDL, m_button_SaveAsNewBdl);    //son220228

    DDX_Control(pDX, IDC_EDIT101,   m_edit101);     //sylee210621

    //}}AFX_DATA_MAP
}

//son210906 IDC_BUTTON201 버튼 관련 문제: 
//   영풍베트남 이외의 고객사버전에서 ACE400 종료시에 SW_HIDE된 OLE 컨트롤(IDC_BUTTON201)이 종료가 잘 안되는  
//   문제 관련하여 IDC_BUTTON201 버튼을 FM20 OLE Control 버튼에서 PushButton 으로 변경하여 해결함. 
//   이를 위해 ON_EVENT()가 아니라 ON_BN_CLICKED()에서 OnButton201()을 처리하도록 수정함.
//   son220124 또 다른 해결책. DDX_Control로 등록한 컨트롤은 SW_HIDE해도 종료가 잘 된다. 
//   son220610 다른 해결책 2. FM20 컨트롤은 처음에 설정된 숫자 아이디를 그대로 쓴다면 DDX_Control 등록 안해도  창 닫을 때 죽는 문제가 없다.

BEGIN_MESSAGE_MAP(CChildView1, CFormView)
    //{{AFX_MSG_MAP(CChildView1)
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_BUTTON201, OnButton201)       //son210906 
//SYLEE180807-1 DUT16 DEL.  ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CChildView1, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView1)
    ON_EVENT(CChildView1, IDC_BUTTON1001, -600 /* Click */, OnButton1001, VTS_NONE)
//SYLEE180807-1 DUT16 DEL.  ON_EVENT(CChildView1, IDC_BUTTON1003, -600 /* Click */, OnButton1003, VTS_NONE) 
//SYLEE180807-1 DUT16 DEL.  ON_EVENT(CChildView1, IDC_BUTTON1006, -600 /* Click */, OnButton1006, VTS_NONE) 
//SYLEE180807-1 DUT16 DEL.  ON_EVENT(CChildView1, IDC_BUTTON1008, -600 /* Click */, OnButton1008, VTS_NONE) 
    ON_EVENT(CChildView1, IDC_BUTTON111, -600 /* Click */, OnButton111, VTS_NONE)
    ON_EVENT(CChildView1, IDC_BUTTON113, -600 /* Click */, OnButton113, VTS_NONE)
    ON_EVENT(CChildView1, IDC_BUTTON112, -600 /* Click */, OnButton112, VTS_NONE)
    ON_EVENT(CChildView1, IDC_BUTTON114, -600 /* Click */, OnButton114, VTS_NONE)
    ON_EVENT(CChildView1, IDC_BUTTON3001, -600 /* Click */, OnButton3001, VTS_NONE)
    ON_EVENT(CChildView1, IDC_BUTTON_NEWBDL, -600 /* Click */, OnButton_SaveAsNewBdl, VTS_NONE)   //son220228     
    //ON_EVENT(CChildView1, IDC_BUTTON201, -600 /* Click */, OnButton201, VTS_NONE)     //sylee210621   //son210906 삭제
    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView1 diagnostics

#ifdef _DEBUG
void CChildView1::AssertValid() const
{
    CFormView::AssertValid();
}

void CChildView1::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CChildView1 message handlers

void CChildView1::PostNcDestroy() 
{
    // TODO: Add your specialized code here and/or call the base class
    delete this;
//  CFormView::PostNcDestroy();
}


void CChildView1::A_Language() //sylee151028 //LANGUAGE CHANGE
{
 
    //son C:\\ACE400\\SETUP\\SysSet211.ini 3번째 항목
    if(SysSet211.m_nSet13==1){//sylee160907-1   //chinese
        gDef_Language1=3;//redboard
    }
    else{//sylee160907-1
        gDef_Language1=2; //englishos
    }//sylee160907-1

    if( gDef_Language1==3){//sylee151028-1 
        //  m_Label101.SetWindowText(LoadMessage(1,"TitleFile", 101));
        m_button3001.SetWindowText(LoadMessage(1,"TitleFile", 102));
        m_button1001.SetWindowText(LoadMessage(1,"TitleFile", 104)); 
    }

}

BOOL CChildView1::OnEraseBkgnd(CDC* pDC)    
{
//  DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204)); 권장 파랑색 
    DrawGradation(pDC, RGB(221, 236, 255), RGB(100, 125, 204)); //권장 파랑색 
 
    return TRUE;
//  return CDialog::OnEraseBkgnd(pDC);
}

/////////////////////////////////////////////////////////////////////////////
//그라데이션을 그리는 함수.
void CChildView1::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
{
    CRect Rect;
    GetClientRect(&Rect);

    COLORREF rgbReturn = 0;
    int byRBegin = GetRValue(rgbBegin);
    int byGBegin = GetGValue(rgbBegin);
    int byBBegin = GetBValue(rgbBegin);

    int byR = GetRValue(rgbBegin);
    int byG = GetGValue(rgbBegin);
    int byB = GetBValue(rgbBegin);

    int byR_diff = 0;
    int byG_diff = 0;
    int byB_diff = 0;

    byR_diff = byRBegin - GetRValue(rgbEnd);
    byG_diff = byGBegin - GetGValue(rgbEnd);
    byB_diff = byBBegin - GetBValue(rgbEnd);

    CPen* pNewPen = NULL;
    CPen* pOldPen = NULL;

    int byOffset = 0;
    int nPercent = 0;

    nAutoProcess1=0;

    for(int i = 0; i < Rect.bottom; ++i)
    {
        pNewPen = new CPen(PS_SOLID, 1, RGB(byR, byG, byB));
        rgbReturn = RGB(byR, byG, byB);
        pOldPen = pDC->SelectObject(pNewPen);

        pDC->MoveTo(0, i);
        pDC->LineTo(Rect.right, i);

        nPercent = i * 100 / Rect.bottom;
        byOffset = byR_diff * nPercent / 100;
        byR = byRBegin - byOffset;
        byOffset = byG_diff * nPercent / 100;
        byG = byGBegin - byOffset;
        byOffset = byB_diff * nPercent / 100;
        byB = byBBegin - byOffset;

        pDC->SelectObject(pOldPen);
        delete pNewPen;
    }
}


BOOL CChildView1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
    return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CChildView1::OnInitialUpdate() 
{
    CFormView::OnInitialUpdate();

    CString str;  
 
    CWnd* pWnd;     
    pWnd = GetDlgItem(IDC_STATIC_MAIN1001); 


    //son201216 ComiDaq 디바이스를 ChildView7과 공유하기 위한 ComiDaq 초기화 함수 호출
    g_pChildView7->Init_ComiDaqDevice();
    g_ComiDaq2.DoPutAll(DOUT_CMD_ALL_OFF);  //son231016 
    MyTrace(PRT_BASIC, "ChildView1-> ChildView7::Init_ComiDaqDevice() Called. \n");

    CleanUp_MyTraceLog("d:\\log");                                  //son220510 "d\\log" 폴더 2달 이상 지난 로그 삭제 
    str.Format("%s\\log", SysInfoView01.m_pStrFilePath1);           //son220510 "C:\\ACE500\\log" 폴더 2달 이상 지난 로그 삭제
    CleanUp_MyTraceLog(str.GetBuffer(FILE_PATH_SIZE));
#ifdef _PC_TEST_
    CleanUp_MyTraceLog(g_sFile.LogDir.GetBuffer(FILE_PATH_SIZE));   //son220510 현재수행폴더의 "\\log" 폴더 정리
#endif


    //son201216:  FileDialog가 열려 있는데 BDL 자동로딩 메시지 수신시 다운문제 관련 추가
    //son BDL Loading FileDialog가 열려있지 않다는 의미로 0으로 초기화 한다.
    nFlag_FileDialogOn =0;  //son201216

    //::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePath1);   //son210514
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1);

    //son "C:\ACE400\SETUP\FilePathJig.ini"를 struct CSysInfoView01에 Load(type=2) 
    FileSysInfo01.LoadSaveView01(2);//load

    //son 현재 디렉토리명을 SysInfoView01의 m_pStrFileDri1에 할당. 
    //::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePath1);   //son210514 MainFrm에서 SysInfoView01.m_pStrFilePath1를 먼저 C:\ACE500으로 설정
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1);

   //sylee140402 FileSysInfo01.LoadSaveView01(1);


    //son 현재 BDL 파일의 path&파일명을 화면의 m_LabelPath에 출력.
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);
    m_LabelPath.SetCaption(str);
 
//SYLEE20120629

//SYLEE180807-1 DUT16 DEL.
/*
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePath1D);
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1D);
    FileSysInfo01.LoadSaveView01D(2);//load     //son ??? for DUT2 ? 
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePath1D);
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1D);
    FileSysInfo01.LoadSaveView01D(1); 
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1D);
    m_Label2.SetCaption(str);                   //son 관련 m_Label2 현재 UI에 없음.

//SYLEE20120809
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePath1E);
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1E);
    FileSysInfo01.LoadSaveView01E(2);//load
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePath1E);
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1E);
    FileSysInfo01.LoadSaveView01E(1);  
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1E);
    m_Label3.SetCaption(str);

  */  //SYLEE180807-1 DUT16 DEL.

    str.Format("                                    "  );
    m_Label2.SetCaption(str);
    m_Label3.SetCaption(str);
    m_Label4.SetCaption(str);

    //son "C:\\ACE400\\SETUP\\SysSet21.ini"를 struct CSysSet21에 Load(type=2) 또는 Save(type=1) 
    FileSysInfo01.LoadSaveSet21(2);//sylee130702  //load 

    //son "C:\\ACE400\\SETUP\\SysSet22.ini"를 struct CSysSet22에 Load(type=2) 또는 Save(type=1) 
    FileSysInfo01.LoadSaveSet22(2); 
 
    // ModeSet16.Onload();//sylee150203
    //son "C:\\ACE400\\SETUP\\SysSet01.ini"를 struct CSysSet13에 Load(type=2)
    FileSysInfo01.LoadSaveSet13(2);//load 
 

    OnButton2001();//sylee130228        //son OnDataLoadAuto() 호출. 버튼 2001은 실제로 없음.

    //son OnDataSave() 호출후에 ReadFileB(1) 호출. 버튼 1002는 실제로 없음.
    OnButton1002();//sylee130228

    Check_DDrive_FreeSpace();      //son221215 d 드라이브 용량 체크 

 
    CMainFrame* pFrame;
    pFrame = (CMainFrame*)AfxGetMainWnd();
  // int nRet1;


//sylee180627     Auto Self Start Disable    if(SysSet13.m_nSet9!=1){//sylee150203
    //son 'Self Test' 화면
    pFrame->OnMenuTest8() ; //SYLEE131105 JOYTECH SAJANGNIM           
    g_nSelfAutoFlag1=10; //SYLEE131105 JOYTECH SAJANGNIM

//sylee180627  Auto Self Start Disable   }else{
//sylee180627         //son 'MAP' 화면
//sylee180627         pFrame->OnMenuTest6() ; 
//sylee180627    }//sylee150203
     
        
    CFont font2;    
    LOGFONT lf;

    lf.lfHeight         = MulDiv(9, 500, 300);
    lf.lfWidth          = 0;
    lf.lfEscapement     = 0;
    lf.lfOrientation    = 0; 
    lf.lfWeight         = FW_NORMAL;
    lf.lfItalic         = FALSE;
    lf.lfUnderline      = FALSE;
    lf.lfStrikeOut      = 0;
    lf.lfCharSet        = HANGEUL_CHARSET;
    lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    lf.lfQuality        = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
      
    font2.CreateFontIndirect(&lf);
 
    m_list1.SetFont(&font2); 
    m_list2.SetFont(&font2); //SYLEE150817
    m_list3.SetFont(&font2); //SYLEE150817
    m_list4.SetFont(&font2); //SYLEE150817

    FileSysInfo01.LoadSaveSet211(_LOAD);//load //sylee160928

    A_Language();

#ifndef __YP_VIET_CUSTOMER //son210906
    //son210906 영풍 베트남용 SW가 아니면 Multi DUT 생성을 위한 edit박스와 버튼을 숨긴다.
    
    //son210906 IDC_BUTTON201버튼 관련 문제: 
    //   영풍베트남 이외의 고객사버전에서 ACE400 종료시에 SW_HIDE된 OLE 컨트롤(IDC_BUTTON201)이 종료가 잘 안되는  
    //   문제 관련하여 IDC_BUTTON201 버튼을 FM20 OLE Control 버튼에서 PushButton 으로 변경하여 해결함. 
    //   이를 위해 ON_EVENT()가 아니라 ON_BN_CLICKED()에서 OnButton201()을 처리하도록 수정함.
    GetDlgItem(IDC_EDIT101)->ShowWindow(SW_HIDE); 
    GetDlgItem(IDC_BUTTON201)->ShowWindow(SW_HIDE); 
#endif

    //son220124 제이텍 요구사항.  d:\log4w\폴더명에 LotName 추가를 위해 LotName 확인 기능을 ChildView1에 추가함.
    //  FM20 Control도 DDX_Control()로 등록해 주면  SW_HIDE가 가능하다.
    if (SysSet13.m_nSet_UseLotName_4wLog != 1)  //son220124
    {
        //son 'Use Lot Name for 4w Log'  옵션이 꺼져 있다면  숨긴다.
        m_LabelLotNameTitle.ShowWindow(SW_HIDE);        //son220124
        m_LabelLotName.ShowWindow(SW_HIDE); 
    }
    else
    {
        m_LabelLotNameTitle.ShowWindow(SW_SHOW);        //son220124
        m_LabelLotName.ShowWindow(SW_SHOW); 
    }

#ifndef __MST__CUSTOMER
    //son220228 MST 고객사가 아니면 m_button_SaveAsNewBdl 버튼을 숨긴다.
    m_button_SaveAsNewBdl.ShowWindow(SW_HIDE);         //son220228
#endif
}
 
int PinDataLine; 
int PinDataS1[DEF_MAX_PIN1][2]; 
int PinDataS2[DEF_MAX_PIN1][5]; 
int nPinDa1H[MAX_DUT1][5][20];//SYLEE180810-1-DUT16     //son220824_2
        //son nPinDa1H[nDut][1][0])     ex) 20000       (BDL 파일 표시) 
        //
        //son nPinDa1H[nDut][2][0])     ex) 32  32  0   (총핀수, 상부핀수, 하부핀수)
        //son nPinDa1H[nDut][2][1])         
        //son nPinDa1H[nDut][2][2])         
        //son nPinDa1H[nDut][2][3])     ex) 32   (총핀수)
        //son nPinDa1H[nDut][2][4])     ex) 264  ??
        //
        //son nPinDa1H[nDut][3][0])     


int nPinDa12w[MAX_DUT1][200][5];// BDL READING RAW DATA  2w 16k //SYLEE180810-1-DUT1        //son220824_2
int nPinOp1[DEF_MAX_PIN1][10]; // BDL 2W  OPEN  CASE1  ( PIN NO, PIN NO) -> PCB, PIN, PCB, PIN   ( 0,1,2,3,4,5)
int nPinSh1[DEF_MAX_PIN1][10]; // BDL 2W  SHORT CASE1  ( PIN NO, PIN NO) -> PCB, PIN, PCB, PIN   ( 0,1,2,3,4,5)

/////////////////////////////////////////////////////////////////////////////////////////////////////




//son BDL Data Read 
void ReadFileB(int nDUT)
{
    CFile file;  
    FILE *fp; 
    CString strVal1, strfName,str;  
    char fName1[200]; 
    long   i=0,j=0,k=0,m=0,p=0,val=0;

    //son Range Check nDut : 1~ 32
    if( nDUT<1)
    {
        nDUT=1;  
        str.Format("  Error No 1012. . ReadFileB DUT=  ???\n");     //son220317
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    //if( nDUT>DefDutMax){  //son > 33 으로 체크하면 33은 정상통과하는 문제있음.
    if (nDUT > MAX_DUT)     //son220824_3 33이 통과 못하게 DefDutMax(33) -> MAX_DUT(32)로 수정. 
    {
        nDUT=1;  
        str.Format("  Error No 1013. . ReadFileB DUT=  ???\n");     //son220317
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    //==========================================================================================================
    ::ZeroMemory(&nPinDa1H[nDUT], sizeof(nPinDa1H[nDUT]));
    ::ZeroMemory(&nPinDa12w[nDUT], sizeof(nPinDa12w[nDUT]));
    ::ZeroMemory(&nPinOp1, sizeof(nPinOp1));
    ::ZeroMemory(&nPinSh1, sizeof(nPinSh1));
    //==========================================================================================================
    
    //son "\SETUP2\PieceRange.ini"를 struct CSysInfoPieceRange에 Load(type=2)
    FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD); //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
    ////////////////////////////////////////////////////
    //******************************************************************************************************************
    // bdl data  reading

    strfName.Format("\\BDLW\\%s",SysInfoView01.m_pStrFilePathJig2);
    ::ZeroMemory(&fName1,sizeof(fName1));


    strcat( fName1 , SysInfoView01.m_pStrFilePath1 );       //son  sw 수행한 디렉토리 
    strcat( fName1 , strfName ); 

    if(SysInfoView01.m_pStrFilePathJig1[0] == NULL ){ //sylee140403
        str.Format("Error No 1009  :  \n\n  NULL ==> SysInfoView01.m_pStrFilePathJig1 , \n\n Open File Reading  !.\n" );
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return ;
    }

    //sylee180810-1-dut16- import //sytlee180\901-import fp=fopen(SysInfoView01.m_pStrFilePathJig1,"rt");



    CString str2,str3;  
    char fName[200]; 
    int No;

    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , SysInfoView01.m_pStrFilePathBDL );//SYLEE121202
    str2.Format("%d",nDUT); //SYLEE180810-1-DUT16
    str3.Format("%s",SysInfoView01.m_pStrFileBDLName); //SYLEE180810-1-DUT16

    //son200630: BH 코드 머지  
    //     '.bdl'의 위치를 찾아 보고 없으면, '.BDL'의 위치를 찾는다. 
    //     'AMB611WP01-MAIN(REV1.4-M1.0)-4W.BDL' 과 같이 중간에 '.' 이 포함된 경우에는
    //     '.'만 찾아서는 확장자의 위치를 제대로 찾을 수 없으므로 ".bdl"이나 ".BDL"의 위치를 찾도록 수정. 
    //No=str3.Find('.');    //sylee190926 
    No = str3.Find(".bdl");     //son200701  1) 홑따옴표를 겹따옴표로 수정. string은 겹따옴표로 묶어야 제대로 Find가 됨.
    if (No < 0) {               //           2) if (No = -1) 로 체크하던 버그 수정
        No = str3.Find(".BDL");
    }

    str3=str3.Left(No);

    if(nDUT>1){
        str2="\\"+str3+".bdl"+str2; //SYLEE180810-1-DUT16
    }
    else{
        str2="\\"+str3+".bdl"; //SYLEE180810-1-DUT16
    }
    strcat( fName , str2 );//sylee150817

    fp=fopen(fName,"rt");   //sylee180810-1-dut16- import //sytlee180\901-import 

    //son OnDataLoadAuto()에서 fopen()이 이미 실패한게 아니라면 fp NULL인지 체크 
    if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp==NULL){  
            strfName.Format("Error No 1001.    file open error    %s ", fName); //son201216 fName 출력되도록 수정.
            MyTrace(PRT_BASIC, strfName);        //son211022
            AfxMessageBox(strfName, MB_OK);
            //return  ;
        }
    }
    else{
        if(fp!=NULL){  //sylee161116-2

            //son "BDL File Load OK"를 설정
            nFlagFileLoadok=1;//sylee161116-2
        }
    }


    //==========================================================================================================
    //BDL BIOPTRO FORMAT CHECK

    if(fp!=NULL)
    {

        fscanf(fp, "%d\n", &nPinDa1H[nDUT][1][0]);
        if( nPinDa1H[nDUT][1][0] != 20000 ){ //sylee180810-1-DUT16-//SYLEE180901-1
            str.Format("  Error No 1002.   NET LIST FILE SAVE?.    BDL File Reading Error.    BiOptro format ? !.", MB_OK);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            fclose(fp); 
            return ;    
        }
        //==========================================================================================================
        //BDL HEADER READING
        for(i=0; i<5; i++){   
            fscanf(fp, "%d", &nPinDa1H[nDUT][2][i]);
        }
        fscanf(fp, "%d\n", &nPinDa1H[nDUT][3][0]);      //son190620 int를 %s로 read하던 것을 %d로 오류수정

        //==========================================================================================================

        //Pin data  reading
        if(nPinDa1H[nDUT][2][0]<1){
            str.Format("Error No 1003.   NET LIST FILE SAVE?.     BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            fclose(fp);
            return; 
        }

        if(nPinDa1H[nDUT][2][0]>=DEF_MAX_PIN1){  //32k
            str.Format(" Error No 1004.   NET LIST FILE SAVE?.    BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            fclose(fp);
            return; 
        } 

        //son Error??? Bug???  nPinDa1H[nDUT][1][0] 이 아니라  nPinDa1H[nDUT][2][0] 이 맞지 않나? 
        for(i=1; i<(nPinDa1H[nDUT][1][0]+1); i++)
        {
            for(k=0; k<5; k++){ 
                fscanf(fp, "%d", &nPinDa12w[nDUT][i][k]); 

                if(k==4){
                    fscanf(fp, "\n" );
                }

                //son nPinDa12w[][]   nBDLData1[][][] 와 유사함...

                //son k==0 (PinNo),  k==1(NetNo)
                if(k==0 || k==1){
                    //if(nPinDa12w[1][i][k]<0  ||  nPinDa12w[1][i][k]>16383){
                    if(nPinDa12w[nDUT][i][k]<0  ||  nPinDa12w[nDUT][i][k]> MAX_PIN){
                        //fclose(fp);   //son201217
                        str.Format(" Error No 1005.   NET LIST FILE SAVE?.    BDL File Reading Error.     format  postion  0, 1 :  Pin data <0 or >= 32768 Reading Error ?.", MB_OK);
                        MyTrace(PRT_BASIC, str);        //son211022
                        AfxMessageBox(str, MB_OK);
                    }
                    fclose(fp); //son201217추가 : fclose하지 않고 리턴하는 케이스 관련 fclose 위치 수정
                    return;                         
                }

                //son k==4 PinCount이므로 1보다 작을 수 없다.
                if(k==4){   
                    //if(nPinDa12w[1][i][k]<1 ||  nPinDa12w[1][i][k]>16383){ //16K=16384
                    if(nPinDa12w[nDUT][i][k]<1 ||  nPinDa12w[nDUT][i][k]> MAX_PIN){ //16K=16384
                        //fclose(fp);   //son201217
                        str.Format(" Error No 1006.   NET LIST FILE SAVE?.    BDL File Reading Error.     format   postion  4 : Pin data <1 or >= 32768 Reading Error ?.", MB_OK);
                        MyTrace(PRT_BASIC, str);        //son211022
                        AfxMessageBox(str, MB_OK);

                    }
                    fclose(fp); //son201217추가  : fclose하지 않고 리턴하는 케이스 관련 fclose 위치 수정
                    return;                         
                }
            }
        }

        fclose(fp);
    }


    //******************************************************************************************************************
    // open, short data   분류 
    k=1;
    j=0;
    //  ::ZeroMemory(&nTm1, sizeof(nTm1));
    for(i=1; i<(nPinDa1H[nDUT][2][0]+1); i++){   // one point를 [0]에  다른 pin [1]에 
        nPinOp1[i][0]=nPinDa12w[nDUT][k][0];
        nPinOp1[i][1]=nPinDa12w[nDUT][i][0];
        nPinOp1[i][2]=nPinDa12w[nDUT][i][4];
        j++;
        if(j==nPinDa12w[nDUT][i][4]){
            j=0;
            k=i+1;
        }    
    }

    k=1;
    j=1;
    for(i=1; i<(nPinDa1H[nDUT][2][0]+1); i++){    
        if(nPinOp1[i][0]!=nPinOp1[i][1]){    // [0][1] 같으면 open에서 삭제.
            nPinOp1[k][0]=nPinOp1[i][0];
            nPinOp1[k][1]=nPinOp1[i][1];
            nPinOp1[k][2]=nPinOp1[i][2];
            k++;
        }
        else{       // [0][1] 같으면   short  data   취합     net  fisrt  point 
            nPinSh1[j][0]=nPinOp1[i][0];
            j++;
        }
    }

    nPinOp1[0][0]=k-1; //open   count
    nPinSh1[0][0]=j-1; //short  count

    if( nPinOp1[0][0]!=(nPinDa1H[nDUT][2][0]-nPinDa1H[nDUT][2][1])){
        str.Format(" Error No 1007.   NET LIST FILE SAVE?.    BDL File Reading Error.   Open Sort Error.     format  Pin data Reading Error ?.\n"); //son220317
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
        return; 
    }


    FileSysInfo01.LoadSaveSet12(_LOAD);// //sylee150304  LoadSaveSub15(2); 
    ::ZeroMemory(&nPcb1, sizeof(nPcb1));


    if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp!=NULL){
            str.Format("  Net list [BDL] File Reading   Success. \n");    //son220317
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
        }
        else{
            str.Format("  FAIL  ..   Net list [BDL] File Reading   . \n");  //son220317
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
        }
    }

}




 /* //SYLEE180810-1-DUT16

 void ReadFileB1()
 {
     CFile file;  
     FILE *fp; 
     CString strVal1, strfName,str;  
     char fName1[200]; 
     long   i=0,j=0,k=0,m=0,p=0,val=0;
//   int   z1;//PinDataSet[3][15]={0,},nTm1[DEF_MAX_PIN1][10];

//==========================================================================================================
     ::ZeroMemory(&nPinDa1H[1], sizeof(nPinDa1H[1]));
     ::ZeroMemory(&nPinDa12w[1], sizeof(nPinDa12w[1]));
     ::ZeroMemory(&nPinOp1, sizeof(nPinOp1));
     ::ZeroMemory(&nPinSh1, sizeof(nPinSh1));
//==========================================================================================================
     FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD);    //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
     ////////////////////////////////////////////////////
 //==========================================================================================================
// bdl data  reading
    // (SysInfoView01).m_pStrFilePath1
    strfName.Format("\\BDLW\\%s",SysInfoView01.m_pStrFilePathJig2);
    ::ZeroMemory(&fName1,sizeof(fName1));
        

    strcat( fName1 , SysInfoView01.m_pStrFilePath1 ); 
    strcat( fName1 , strfName ); 

     if(SysInfoView01.m_pStrFilePathJig1[0] == NULL ){ //sylee140403
        str.Format("Error No 1009  :  \n\n  NULL ==> SysInfoView01.m_pStrFilePathJig1 , \n\n Open File Reading  !.\n" );
        AfxMessageBox(str, MB_OK);          
        return ;
     }

    fp=fopen(SysInfoView01.m_pStrFilePathJig1,"rt");

    if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp==NULL){  
            strfName.Format("Error No 1001.    file open error    %s ", (SysInfoView01).m_pStrFilePathJig1);
            AfxMessageBox(strfName, MB_OK);
            //return  ;
        }
    }
    else{
        if(fp!=NULL){  //sylee161116-2
            nFlagFileLoadok=1;//sylee161116-2
        }
    }


//==========================================================================================================
    //BDL BIOPTRO FORMAT CHECK

    if(fp!=NULL){
 
            fscanf(fp, "%d\n", &nPinDa1H[1][1][0]);
            if( nPinDa1H[1][1][0] != 20000 ){ 
                fclose(fp); 
                AfxMessageBox("  Error No 1002.   NET LIST FILE SAVE?.    BDL File Reading Error.    BiOptro format ? !.", MB_OK);
                return ;    
            }
        //==========================================================================================================
            //BDL HEADER READING
            for(i=0; i<5; i++){   
                fscanf(fp, "%d", &nPinDa1H[1][2][i]);
            }
            fscanf(fp, "%d\n", &nPinDa1H[1][3][0]);     //son190620 int를 %s로 read하던 것을 %d로 오류수정
 
        //==========================================================================================================

            //Pin data  reading
            if(nPinDa1H[1][2][0]<1){
                fclose(fp);
                AfxMessageBox("Error No 1003.   NET LIST FILE SAVE?.     BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            }

            if(nPinDa1H[1][2][0]>=DEF_MAX_PIN1){  //32k
                fclose(fp);
                AfxMessageBox(" Error No 1004.   NET LIST FILE SAVE?.    BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            } 

            for(i=1; i<(nPinDa1H[1][1][0]+1); i++){
               for(k=0; k<5; k++){ 
                    fscanf(fp, "%d", &nPinDa12w[1][i][k]); 

                    if(k==4){
                        fscanf(fp, "\n" );
                    }

                    if(k==0 || k==1){
                        //if(nPinDa12w[1][i][k]<0  ||  nPinDa12w[1][i][k]>16383){
                        if(nPinDa12w[1][i][k]<0  ||  nPinDa12w[1][i][k]>32768){
                            fclose(fp);
                            //AfxMessageBox(" Error No 1005.   NET LIST FILE SAVE?.    BDL File Reading Error.     format  postion  0, 1 :  Pin data <0 or >= 16384 Reading Error ?.", MB_OK);
                            AfxMessageBox(" Error No 1005.   NET LIST FILE SAVE?.    BDL File Reading Error.     format  postion  0, 1 :  Pin data <0 or >= 32768 Reading Error ?.", MB_OK);
                            
                        }
                        return;                         
                    }

                    if(k==4){
                        //if(nPinDa12w[1][i][k]<1 ||  nPinDa12w[1][i][k]>16383){ //16K=16384
                        if(nPinDa12w[1][i][k]<1 ||  nPinDa12w[1][i][k]>32768){ //16K=16384
                            fclose(fp);
                            //AfxMessageBox(" Error No 1006.   NET LIST FILE SAVE?.    BDL File Reading Error.     format   postion  4 : Pin data <1 or >= 16384 Reading Error ?.", MB_OK);
                            AfxMessageBox(" Error No 1006.   NET LIST FILE SAVE?.    BDL File Reading Error.     format   postion  4 : Pin data <1 or >= 32768 Reading Error ?.", MB_OK);
                            
                        }
                        return;                         
                    }
               }
            }

            fclose(fp);
    }

         
 //==========================================================================================================
 // open, short data   분류 
    k=1;
    j=0;
//  ::ZeroMemory(&nTm1, sizeof(nTm1));
    for(i=1; i<(nPinDa1H[1][2][0]+1); i++){   // one point를 [0]에  다른 pin [1]에 
        nPinOp1[i][0]=nPinDa12w[1][k][0];
        nPinOp1[i][1]=nPinDa12w[1][i][0];
        nPinOp1[i][2]=nPinDa12w[1][i][4];
        j++;
        if(j==nPinDa12w[1][i][4]){
           j=0;
           k=i+1;
        }    
    }

    k=1;
    j=1;
    for(i=1; i<(nPinDa1H[1][2][0]+1); i++){    
        if(nPinOp1[i][0]!=nPinOp1[i][1]){    // [0][1] 같으면 open에서 삭제.
            nPinOp1[k][0]=nPinOp1[i][0];
            nPinOp1[k][1]=nPinOp1[i][1];
            nPinOp1[k][2]=nPinOp1[i][2];
            k++;
        }
        else{       // [0][1] 같으면   short  data   취합     net  fisrt  point 
            nPinSh1[j][0]=nPinOp1[i][0];
            j++;
        }
    }

    nPinOp1[0][0]=k-1; //open   count
    nPinSh1[0][0]=j-1; //short  count

    if( nPinOp1[0][0]!=(nPinDa1H[1][2][0]-nPinDa1H[1][2][1])){
        AfxMessageBox(" Error No 1007.   NET LIST FILE SAVE?.    BDL File Reading Error.   Open Sort Error.     format  Pin data Reading Error ?.", MB_OK);
        return; 
    }
 //==========================================================================================================
    // pin type 
    // pin shift
    // piece
    // enable  disable
    // pin no -> pcb  + pin no
 //==========================================================================================================
 
//==========================================================================================================
    i=9; 
//////////////////////////////////////////////////////////////////
// PIN TYPE CONVERTING
//PIN MODE

     FileSysInfo01.LoadSaveSet12(2);// //sylee150304  LoadSaveSub15(2); 
     ::ZeroMemory(&nPcb1, sizeof(nPcb1));


     if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp!=NULL){
             AfxMessageBox("  Net list [BDL] File Reading   Success.  ", MB_OK);
        }
        else{
            AfxMessageBox("  FAIL  ..   Net list [BDL] File Reading   .  ", MB_OK);
        }
     }

 }



 void ReadFileB2()
 {
     CFile file;  
     FILE *fp;
     CString strVal1;
     CString  strfName;  
     char fName1[200]; 
     long   i=0,j=0,k=0,m=0,p=0,val=0;
//   int  PinDataSet[3][15]={0,};  

//==========================================================================================================

     ::ZeroMemory(&nPinDa1H[2], sizeof(nPinDa1H[2]));
     ::ZeroMemory(&nPinDa12w[2], sizeof(nPinDa12w[2]));
     ::ZeroMemory(&nPinOp1, sizeof(nPinOp1));
     ::ZeroMemory(&nPinSh1, sizeof(nPinSh1));
//==========================================================================================================
     FileSysInfo01.LoadSavePieceRangeInfo(2, _LOAD);//sylee180725 2->22 
                                                    //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
     ////////////////////////////////////////////////////
 //==========================================================================================================
// bdl data  reading

    strfName.Format("\\BDLW\\%s",(SysInfoView01).m_pStrFilePathJig2);
    ::ZeroMemory(&fName1,sizeof(fName1));
    strcat( fName1 , SysInfoView01.m_pStrFilePath1 ); 
    strcat( fName1 , strfName );

    CString str;//SYLEE150821 
    
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);//SYLEE150821 
    str=str+'2';//SYLEE150821 

//SYLEE150821       fp=fopen(SysInfoView01.m_pStrFilePathJig1,"rt");
    ::ZeroMemory(&fName1,sizeof(fName1));  //SYLEE150821
    strcat( fName1 , str );//SYLEE150821
    fp=fopen(fName1,"rt");//SYLEE150821
    if(fp==NULL){ 
    //  fclose(fp); 
    //  AfxMessageBox(" Error No 1001.   NET LIST FILE SAVE?.   BDL File Reading     Please BDL FILE  SET SAVE! ", MB_OK);
        //return  ;
    }
//==========================================================================================================
    //BDL BIOPTRO FORMAT CHECK

    if(fp!=NULL){
        
            fscanf(fp, "%d\n", &nPinDa1H[2][1][0]);
            if( nPinDa1H[2][1][0] != 20000 ){ 
                fclose(fp); 
                AfxMessageBox("  Error No 1002.   NET LIST FILE SAVE?.    BDL File Reading Error.    BiOptro format ? !.", MB_OK);
                return ;    
            }
        //==========================================================================================================
            //BDL HEADER READING
            for(i=0; i<5; i++){   
                fscanf(fp, "%d", &nPinDa1H[2][2][i]);
            }
            fscanf(fp, "%d\n", &nPinDa1H[2][3][0]);     //son190620 int를 %s로 read하던 것을 %d로 오류수정
 
        //==========================================================================================================

            //Pin data  reading
            if(nPinDa1H[2][2][0]<1){
                fclose(fp);
                AfxMessageBox("Error No 1003.   NET LIST FILE SAVE?.     BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            }

            if(nPinDa1H[2][2][0]>=DEF_MAX_PIN1){  //32k
                fclose(fp);
                AfxMessageBox(" Error No 1004.   NET LIST FILE SAVE?.    BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            } 


            for(i=1; i<(nPinDa1H[2][1][0]+1); i++){
               for(k=0; k<5; k++){ 
                    fscanf(fp, "%d", &nPinDa12w[2][i][k]); 

                    if(k==4){
                        fscanf(fp, "\n" );
                    }

                    if(k==0 || k==1){
                        if(nPinDa12w[2][i][k]<0  ||  nPinDa12w[2][i][k]>16383){//16K
                            fclose(fp);
                            AfxMessageBox(" Error No 1005.   NET LIST FILE SAVE?.    BDL File Reading Error.     format  postion  0, 1 :  Pin data <0 or >= 16384 Reading Error ?.", MB_OK);
                        }
                        return; 
                    }

                    if(k==4){
                        if(nPinDa12w[2][i][k]<1 ||  nPinDa12w[2][i][k]>16383){ //16K=16384
                            fclose(fp);
                            AfxMessageBox(" Error No 1006.   NET LIST FILE SAVE?.    BDL File Reading Error.     format   postion  4 : Pin data <1 or >= 16384 Reading Error ?.", MB_OK);
                        }
                        return; 
                    }
               }
            }

            fclose(fp);
    }

         
 //==========================================================================================================
 // open, short data   분류 
    k=1;
    j=0;
//  ::ZeroMemory(&nTm1, sizeof(nTm1));
    for(i=1; i<(nPinDa1H[2][2][0]+1); i++){   // one point를 [0]에  다른 pin [1]에 
        nPinOp1[i][0]=nPinDa12w[2][k][0];
        nPinOp1[i][1]=nPinDa12w[2][i][0];
        nPinOp1[i][2]=nPinDa12w[2][i][4];
        j++;
        if(j==nPinDa12w[2][i][4]){
           j=0;
           k=i+1;
        }    
    }

    k=1;
    j=1;
    for(i=1; i<(nPinDa1H[2][2][0]+1); i++){    
        if(nPinOp1[i][0]!=nPinOp1[i][1]){    // [0][1] 같으면 open에서 삭제.
            nPinOp1[k][0]=nPinOp1[i][0];
            nPinOp1[k][1]=nPinOp1[i][1];
            nPinOp1[k][2]=nPinOp1[i][2];
            k++;
        }
        else{       // [0][1] 같으면   short  data   취합     net  fisrt  point 
            nPinSh1[j][0]=nPinOp1[i][0];
            j++;
        }
    }

    nPinOp1[0][0]=k-1; //open   count
    nPinSh1[0][0]=j-1; //short  count

    if( nPinOp1[0][0]!=(nPinDa1H[2][2][0]-nPinDa1H[2][2][1])){
        AfxMessageBox(" Error No 1007.   NET LIST FILE SAVE?.    BDL File Reading Error.   Open Sort Error.     format  Pin data Reading Error ?.", MB_OK);
        return; 
    }
 
//==========================================================================================================
    i=9;
 
//////////////////////////////////////////////////////////////////
// PIN TYPE CONVERTING
//PIN MODE

     FileSysInfo01.LoadSaveSet12(2);// //sylee150304  LoadSaveSub15(2); 

     ::ZeroMemory(&nPcb1, sizeof(nPcb1));  
 
///////////////////////////////////////////////////////////
//   fclose(fp);
 
     if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp!=NULL){
             AfxMessageBox("  Net list [BDL] File Reading   Success.  ", MB_OK);
        }
        else{
            AfxMessageBox("  FAIL  ..   Net list [BDL] File Reading   .  ", MB_OK);
        }
     }

 }




 //void CChildView1::ReadFileB1()
 void ReadFileB3()
 {
     CFile file;  
     FILE *fp;
     CString strVal1;
     CString  strfName; 
     char fName1[200]; 
     long   i=0,j=0,k=0,m=0,p=0,val=0;
//   int  PinDataSet[3][15]={0,};  

//==========================================================================================================
     ::ZeroMemory(&nPinDa1H[3], sizeof(nPinDa1H[3]));
     ::ZeroMemory(&nPinDa12w[3], sizeof(nPinDa12w[3]));
     ::ZeroMemory(&nPinOp1, sizeof(nPinOp1));
     ::ZeroMemory(&nPinSh1, sizeof(nPinSh1));
//==========================================================================================================
     FileSysInfo01.LoadSavePieceRangeInfo(3, _LOAD); //sylee180725 2->32
                                                     //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
     ////////////////////////////////////////////////////
 //==========================================================================================================
// bdl data  reading
    // (SysInfoView01).m_pStrFilePath1
    strfName.Format("\\BDLW\\%s",(SysInfoView01).m_pStrFilePathJig2); //SYLEE180807-1 DUT16 DEL. E->
    ::ZeroMemory(&fName1,sizeof(fName1));
    strcat( fName1 , SysInfoView01.m_pStrFilePath1 );   //SYLEE180807-1 DUT16 DEL.  E->
    strcat( fName1 , strfName );
    CString str;//SYLEE150821 
    
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);//SYLEE150821 
    str=str+'3';//SYLEE150821 

//SYLEE150821   fp=fopen(SysInfoView01.m_pStrFilePathJig1E,"rt");
    ::ZeroMemory(&fName1,sizeof(fName1));  //SYLEE150821
    strcat( fName1 , str );//SYLEE150821
    fp=fopen(fName1,"rt");//SYLEE150821
    if(fp==NULL){ 
    //  fclose(fp); 
    //  AfxMessageBox(" Error No 1001.   NET LIST FILE SAVE?.   BDL File Reading     Please BDL FILE  SET SAVE! ", MB_OK);
        //return  ;
    }
//==========================================================================================================
    //BDL BIOPTRO FORMAT CHECK

    if(fp!=NULL){
 
            fscanf(fp, "%d\n", &nPinDa1H[3][1][0]);
            if( nPinDa1H[1][1][0] != 20000 ){ 
                fclose(fp); 
                AfxMessageBox("  Error No 1002.   NET LIST FILE SAVE?.    BDL File Reading Error.    BiOptro format ? !.", MB_OK);
                return ;    
            }
        //==========================================================================================================
            //BDL HEADER READING
            for(i=0; i<5; i++){   
                fscanf(fp, "%d", &nPinDa1H[3][2][i]);
            }
            fscanf(fp, "%d\n", &nPinDa1H[3][3][0]);     //son190620 int를 %s로 read하던 것을 %d로 오류수정
 
        //==========================================================================================================

            //Pin data  reading
            if(nPinDa1H[3][2][0]<1){
                fclose(fp);
                AfxMessageBox("Error No 1003.   NET LIST FILE SAVE?.     BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            }

            if(nPinDa1H[3][2][0]>=DEF_MAX_PIN1){  //32k
                fclose(fp);
                AfxMessageBox(" Error No 1004.   NET LIST FILE SAVE?.    BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            } 


            for(i=1; i<(nPinDa1H[3][1][0]+1); i++){
               for(k=0; k<5; k++){ 
                    fscanf(fp, "%d", &nPinDa12w[3][i][k]);
 

                    if(k==4){
                        fscanf(fp, "\n" );
                    }

                    if(k==0 || k==1){
                        if(nPinDa12w[3][i][k]<0  ||  nPinDa12w[3][i][k]>16383){//16K
                            fclose(fp);
                            AfxMessageBox(" Error No 1005.   NET LIST FILE SAVE?.    BDL File Reading Error.     format  postion  0, 1 :  Pin data <0 or >= 16384 Reading Error ?.", MB_OK);
                        }
                        return; 
                    }

                    if(k==4){
                        if(nPinDa12w[3][i][k]<1 ||  nPinDa12w[3][i][k]>16383){ //16K=16384
                            fclose(fp);
                            AfxMessageBox(" Error No 1006.   NET LIST FILE SAVE?.    BDL File Reading Error.     format   postion  4 : Pin data <1 or >= 16384 Reading Error ?.", MB_OK);
                        }
                        return; 
                    }
               }
            }

            fclose(fp);
    }

         
 //==========================================================================================================
 // open, short data   분류 
    k=1;
    j=0;
//  ::ZeroMemory(&nTm1, sizeof(nTm1));
    for(i=1; i<(nPinDa1H[3][2][0]+1); i++){   // one point를 [0]에  다른 pin [1]에 
        nPinOp1[i][0]=nPinDa12w[3][k][0];
        nPinOp1[i][1]=nPinDa12w[3][i][0];
        nPinOp1[i][2]=nPinDa12w[3][i][4];
        j++;
        if(j==nPinDa12w[3][i][4]){
           j=0;
           k=i+1;
        }    
    }

    k=1;
    j=1;
    for(i=1; i<(nPinDa1H[3][2][0]+1); i++){    
        if(nPinOp1[i][0]!=nPinOp1[i][1]){    // [0][1] 같으면 open에서 삭제.
            nPinOp1[k][0]=nPinOp1[i][0];
            nPinOp1[k][1]=nPinOp1[i][1];
            nPinOp1[k][2]=nPinOp1[i][2];
            k++;
        }
        else{       // [0][1] 같으면   short  data   취합     net  first  point 
            nPinSh1[j][0]=nPinOp1[i][0];
            j++;
        }
    }

    nPinOp1[0][0]=k-1; //open   count
    nPinSh1[0][0]=j-1; //short  count

    if( nPinOp1[0][0]!=(nPinDa1H[3][2][0]-nPinDa1H[3][2][1])){
        AfxMessageBox(" Error No 1007.   NET LIST FILE SAVE?.    BDL File Reading Error.   Open Sort Error.     format  Pin data Reading Error ?.", MB_OK);
        return; 
    }
 
//==========================================================================================================
    i=9; 

//////////////////////////////////////////////////////////////////

// PIN TYPE CONVERTING
// PIN MODE
 

 

     FileSysInfo01.LoadSaveSet12(2);// //sylee150304  LoadSaveSub15(2); 

     ::ZeroMemory(&nPcb1, sizeof(nPcb1));  
 
///////////////////////////////////////////////////////////
//   fclose(fp);

     if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp!=NULL){
             AfxMessageBox("  Net list [BDL] File Reading   Success.  ", MB_OK);
        }
        else{
            AfxMessageBox("  FAIL  ..   Net list [BDL] File Reading   .  ", MB_OK);
        }
     }

 }

 

 //void CChildView1::ReadFileB1()
 void ReadFileB4()
 {
     CFile file;  
     FILE *fp;
     CString strVal1;
     CString  strfName;  
     char fName1[200]; 
     long   i=0,j=0,k=0,m=0,p=0,val=0;
//   int  PinDataSet[3][15]={0,};  

//==========================================================================================================

     ::ZeroMemory(&nPinDa1H[4], sizeof(nPinDa1H[4]));
     ::ZeroMemory(&nPinDa12w[4], sizeof(nPinDa12w[4]));
     ::ZeroMemory(&nPinOp1, sizeof(nPinOp1));
     ::ZeroMemory(&nPinSh1, sizeof(nPinSh1));

//==========================================================================================================
     FileSysInfo01.LoadSavePieceRangeInfo(4, _LOAD);  //sylee180725 2->42
                                                      //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
     ////////////////////////////////////////////////////
 //==========================================================================================================
// bdl data  reading


    // (SysInfoView01).m_pStrFilePath1


    strfName.Format("\\BDLW\\%s",(SysInfoView01).m_pStrFilePathJig2);   //SYLEE180807-1 DUT16 DEL. E->
    ::ZeroMemory(&fName1,sizeof(fName1));
    strcat( fName1 , SysInfoView01.m_pStrFilePath1 );   //SYLEE180807-1 DUT16 DEL. E->

    strcat( fName1 , strfName );

    CString str;//SYLEE150821 
    
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);//SYLEE150821 
    str=str+'4';//SYLEE150821 

//SYLEE150821       fp=fopen(SysInfoView01.m_pStrFilePathJig1E,"rt");
    ::ZeroMemory(&fName1,sizeof(fName1));  //SYLEE150821
    strcat( fName1 , str );//SYLEE150821
    fp=fopen(fName1,"rt");//SYLEE150821
    if(fp==NULL){ 
    //  fclose(fp); 
    //  AfxMessageBox(" Error No 1001.   NET LIST FILE SAVE?.   BDL File Reading     Please BDL FILE  SET SAVE! ", MB_OK);
        //return  ;
    }
//==========================================================================================================
    //BDL BIOPTRO FORMAT CHECK

    if(fp!=NULL){
 
            fscanf(fp, "%d\n", &nPinDa1H[4][1][0]);
            if( nPinDa1H[4][1][0] != 20000 ){ 
                fclose(fp); 
                AfxMessageBox("  Error No 1002.   NET LIST FILE SAVE?.    BDL File Reading Error.    BiOptro format ? !.", MB_OK);
                return ;    
            }
        //==========================================================================================================
            //BDL HEADER READING
            for(i=0; i<5; i++){   
                fscanf(fp, "%d", &nPinDa1H[4][2][i]);
            }
            fscanf(fp, "%d\n", &nPinDa1H[4][3][0]);         //son190620 int를 %s로 read하던 것을 %d로 오류수정
 
        //==========================================================================================================

            //Pin data  reading
            if(nPinDa1H[4][2][0]<1){
                fclose(fp);
                AfxMessageBox("Error No 1003.   NET LIST FILE SAVE?.     BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            }

            if(nPinDa1H[4][2][0]>=DEF_MAX_PIN1){  //32k
                fclose(fp);
                AfxMessageBox(" Error No 1004.   NET LIST FILE SAVE?.    BDL File Reading Error.    format  Pin data Reading Error ?.", MB_OK);
                return; 
            } 


            for(i=1; i<(nPinDa1H[4][1][0]+1); i++){
               for(k=0; k<5; k++){ 
                    fscanf(fp, "%d", &nPinDa12w[4][i][k]);
 

                    if(k==4){
                        fscanf(fp, "\n" );
                    }

                    if(k==0 || k==1){
                        if(nPinDa12w[4][i][k]<0  ||  nPinDa12w[4][i][k]>16383){//16K
                            fclose(fp);
                            AfxMessageBox(" Error No 1005.   NET LIST FILE SAVE?.    BDL File Reading Error.     format  postion  0, 1 :  Pin data <0 or >= 16384 Reading Error ?.", MB_OK);
                        }
                        return; 
                    }

                    if(k==4){
                        if(nPinDa12w[4][i][k]<1 ||  nPinDa12w[4][i][k]>16383){ //16K=16384
                            fclose(fp);
                            AfxMessageBox(" Error No 1006.   NET LIST FILE SAVE?.    BDL File Reading Error.     format   postion  4 : Pin data <1 or >= 16384 Reading Error ?.", MB_OK);
                        }
                        return; 
                    }
               }
            }

            fclose(fp);
    }

         
 //==========================================================================================================
 // open, short data   분류 
    k=1;
    j=0;
//  ::ZeroMemory(&nTm1, sizeof(nTm1));
    for(i=1; i<(nPinDa1H[4][2][0]+1); i++){   // one point를 [0]에  다른 pin [1]에 
        nPinOp1[i][0]=nPinDa12w[4][k][0];
        nPinOp1[i][1]=nPinDa12w[4][i][0];
        nPinOp1[i][2]=nPinDa12w[4][i][4];
        j++;
        if(j==nPinDa12w[4][i][4]){
           j=0;
           k=i+1;
        }    
    }

    k=1;
    j=1;
    for(i=1; i<(nPinDa1H[4][2][0]+1); i++){    
        if(nPinOp1[i][0]!=nPinOp1[i][1]){    // [0][1] 같으면 open에서 삭제.
            nPinOp1[k][0]=nPinOp1[i][0];
            nPinOp1[k][1]=nPinOp1[i][1];
            nPinOp1[k][2]=nPinOp1[i][2];
            k++;
        }
        else{       // [0][1] 같으면   short  data   취합     net  first  point 
            nPinSh1[j][0]=nPinOp1[i][0];
            j++;
        }
    }

    nPinOp1[0][0]=k-1; //open   count
    nPinSh1[0][0]=j-1; //short  count

    if( nPinOp1[0][0]!=(nPinDa1H[4][2][0]-nPinDa1H[4][2][1])){
        AfxMessageBox(" Error No 1007.   NET LIST FILE SAVE?.    BDL File Reading Error.   Open Sort Error.     format  Pin data Reading Error ?.", MB_OK);
        return; 
    }
 //==========================================================================================================
    // pin type 
    // pin shift
    // piece
    // enable  disable
    // pin no -> pcb  + pin no
 //==========================================================================================================
 
//==========================================================================================================
 
 
//////////////////////////////////////////////////////////////////
// PIN TYPE CONVERTING
//PIN MODE

     FileSysInfo01.LoadSaveSet12(2);// //sylee150304  LoadSaveSub15(2); 

     ::ZeroMemory(&nPcb1, sizeof(nPcb1)); 
//////////////////////////////////////////////////////////////////

    if(nFlagFileLoadok!=-1){//sylee161116-2
        if(fp!=NULL){
             AfxMessageBox("  Net list [BDL] File Reading   Success.  ", MB_OK);
        }
        else{
            AfxMessageBox("  FAIL  ..   Net list [BDL] File Reading   .  ", MB_OK);
        }
     }

 }

 */  //SYLEE180810-1-DUT16



 
void CChildView1::OnDataLoad()  //open button// sylee20111204
{


    //son LoadBDLFile() 수행 직전에 0으로, 수행 직후 1로 설정. 
    nFlagFileLoad2=0;//sylee180406-1 
            
    OnDataLoad_Type_A(); //sylee130907  JOYTECH_DEFINE

    //son  LoadBDLFile() 수행 직후 1로 설정. 
    nFlagFileLoad2=1;//sylee180406-1

    nJigTestOpen=0; nJigTestShort=0;//sylee181008-1

    memset(g_fSparkRefData, 0, sizeof(g_fSparkRefData)); // hdpark220511
    memset(g_nSparkRefDataCount, 0, sizeof(g_nSparkRefDataCount)); // hdpark220511
    memset(g_tSparkAI, 0, sizeof(SPARK_AI) * DEF_MAX_PIN1); // hdpark240102

#ifdef __OPVECTOR__
    MyTrace(PRT_BASIC,"CChildView1::OnDataLoad_Type_A(): g_nOpenTestCount=%d, g_nShortTestCount=%d\n",
                        g_nOpenTestCount, g_nShortTestCount);
    __PrintMemSize(FUNC(CChildView1::OnDataLoad), __LINE__);       //son200420-4WNET10000
#endif
}

 

int CChildView1::OnDataLoad_READ_ConRead() // sylee150922-1  //sylee151111add
{

    //son221019 CString fileName2 = "D:\\1.CON";  //bdl folder
    //son221019 CString fileName3 = "D:\\1.CON2";  //bdl folder         //son 테스트용 (data 정상 확인) 

    FILE *fp,*fp2; 
    char  fName[200], fName2[200]; 
    CString str,str2; 
    int nConTotal1,nConCheckSum1;
    int i,nSum1 ; 

    OnPinConvertType1=0;// sylee150922-1

    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&fName2, sizeof(fName2));
    ::ZeroMemory(&nPinC1, sizeof(nPinC1));
    nSum1=0;
    //---------------------------------------------------------------               
    str2.Format("%s",SysInfoView01.m_pStrFilePathJig2);

    //son '.BDL' 확장자를 삭제 
    str2.TrimRight('l');str2.TrimRight('L');//sylee150308
    str2.TrimRight('d');str2.TrimRight('D');//sylee150308
    str2.TrimRight('b');str2.TrimRight('B');//sylee150308
    str2.TrimRight('.'); //sylee150308

    //son '.CON' 확장자를 추가
    //  ex) C:\ACE400\QC-JIG-S100-BHFLEX\4W_YP_S100\OPEN-READ.CON
    str.Format("%s.CON",str2);//sylee150308

    strcat( fName , str ); 

    str.Format("%s.CON2",str2);//sylee150308
    strcat( fName2 , str ); 
    //------------------------------------------------ 
    //son .CON 파일 open
    fp=fopen(fName,"rt");
    if(fp == NULL){  
        OnPinConvertFlag1=0;//sylee150922-1
        OnPinConvertType1=0; 
        return 0;
    }
    OnPinConvertFlag1=1;//sylee150922-1

    fp2=fopen(fName2,"wt");
    if(fp2 == NULL){ 
        //son221019 str.Format("file open error ,   %s file check?  folder?.", fileName3);
        str.Format("file open error ,   %s file check?  folder?.", fName2); //son221019 
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str);
        return 0;
    }

    //---------------------------
    //son .CON 파일 헤더 read
    //---------------------------       
    fscanf(fp, "%10d  %10d   ", &nConTotal1,&nConCheckSum1);        //son 총핀수,  checkSum
    fprintf(fp2, "%10d  %10d \n  ", nConTotal1,nConCheckSum1);  

    if((nConTotal1>0)&&(nConTotal1<=32768)){
        nPinC1[0][0]=nConTotal1;
    }
    else{
        nPinC1[0][0]=0;
        nConTotal1=0;
        nConCheckSum1=0;
    }

    if(nConTotal1==0 && nConCheckSum1==0){//SYLEE150922-1
        OnPinConvertType1=0;
    }
    else{
        OnPinConvertType1=1;        
    }

    //---------------------------
    //son .CON 파일 data read
    //---------------------------
    //son   nPinC1[i][0]: pinBlockPinNo     nPinC1[i][1]: 사용자 pinNo       
    for(  i=1; i<=nConTotal1; i++){  //FPN EXIT
        fscanf(fp, "%10d %10d   ", &nPinC1[i][0],&nPinC1[i][1]);   //son pinBlockPinNo,  사용자PinNo(JigPinNo#) 
        fprintf(fp2, "%10d  %10d \n  ", nPinC1[i][0],nPinC1[i][1]); 
        nSum1=nSum1+nPinC1[i][0]+nPinC1[i][1];  
    }

    fclose(fp); 
    fclose(fp2); 

    //son checkSum 계산해서 맞는지 확인
    //son BdlConverter 코드에서 CBdlConverterDlg::MakeConnectionFile() 의 관련 부분 참고.
    if(nSum1>1000000){
        nSum1=nSum1/1000000;
    }
    else if(nSum1>1000){
        nSum1=nSum1/1000;
    }
    if(nSum1!=nConCheckSum1){
        OnPinConvertType1=0;//error
        str.Format(" ERROR 1201,   File load fail.    *.con  CheckSum Error!    " );
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str);
    }

    return 1;   
}


//son200420-4WNET10000
//son Piece별 OpenTest List 생성
//
//son  nDut (1~)
void CChildView1::makePieceOpenTestData(int nDut)    //son200420-4WNET10000  
{
    int openIdx;
    int firstPin1= 0;   //son (1~)
    int piece = 0;

#ifndef __OPVECTOR__ 
    int piecePinCnt = 0;
    int arrPiecePinCnt[MAX_PIECE];   

    //------------------------------------------------
    //son open test를 위한 nBDLData2Op[][][] 생성 
    ::ZeroMemory(&arrPiecePinCnt,sizeof(arrPiecePinCnt));
    ::ZeroMemory(&nBDLData2Op[nDut],sizeof(nBDLData2Op[nDut]));
    for(openIdx=1; openIdx<=nBDLData2[nDut][0][1]; openIdx++)
    {
        //son 대표핀과 TestPin이 같은 핀이면 테스트 진행 불가. 
        if(nBDLData2[nDut][openIdx][1] != nBDLData2[nDut][openIdx][2])//sylee130515
        {
            //son 해당 대표핀의 piece번호를 알아낸다.
            firstPin1 = nBDLData2[nDut][openIdx][1] + 1;//son t1 : pinNo (1~)
            piece    = g_nPinPieceNo[nDut][firstPin1];  //son t3 : pinNo에 해당하는 pieceNo  //sylee150917-7

            arrPiecePinCnt[piece]++;                    //son tmp[piece]: piece의 pinCount를 증가시킨다.
            piecePinCnt = arrPiecePinCnt[piece];//count //son t2 : pinCount의 현재 index

            //son piece별 OpenTest Array에 대표핀,TestPin 세트를 추가한다.
            nBDLData2Op[nDut][piece][piecePinCnt][1]=nBDLData2[nDut][openIdx][1];       //son 대표핀
            nBDLData2Op[nDut][piece][piecePinCnt][2]=nBDLData2[nDut][openIdx][2];       //son open 테스트해야할 pinNo
            nBDLData2Op[nDut][piece][0][0] =  arrPiecePinCnt[piece];

        } //sylee130515-test
    }
#endif

#ifdef __OPVECTOR__

    //------------------------------------------------
    //son open test를 위한 nBDLData2Op[][][] 생성 

    // 한번 정의해서 계속 재활용한다.(InsertOpenTest()에서 실제 data 영역에 copy, Call by value)
    COpenTestItem openTestItem; 

    int nOpenTestCount=  nBDLData2[nDut][0][1];

    //son-20200420 : (*g_pvBDLData2Op[nDut][piece])[piecePin] 를 ChildView7에서 사용할때 
    //     모두 1부터 piecePin을 access하고 있음. ChildView7 영역을 모두 0으로 시작하게 바꾸는 것이
    //     리스크가 커서 메모리가 아깝지만,  vector도 1부터 시작하도록 수정하기로 함.
    //     InsertOpenTest() 함수 내부에서 index 0이면 두번 넣게 코드 수정. -> 20200702 삭제 
    //
    //son-20200702 : 대표핀만 있는 BDL인 경우, OpenTest 없이 ShortTest만 하게 됨.  
    //     이경우에는  g_pvBDLData2Op[dutId][ip]->size() 가 0이어서 메모리 access하다 죽는 현상 발생함.
    //     ex) nTOpenSum1=nTOpenSum1 + (g_pvBDLData2Op[nMulNetFlag][ip]->size() -1);
    //         nBDLData2Op[nDut][piece][0][0] 를 (g_pvBDLData2Op[nDut][nPiece]->size() -1)로 대체한 것이 위험해짐.
    //    
    //     이에따라 makePieceOpenTestData() 에서 모든 piece에 대해 무조건 InsertOpenTest()을 먼저 1회 하고 
    //     시작하도록 수정한다.  InsertOpenTest() 함수 내부에서 index 0이면 push_back()두번 넣게 했던 코드는 삭제함.


    //son 모든 piece에 대해 무조건 InsertOpenTest()을 먼저 1회 넣는다. 
    int pieceIdx;
    for(pieceIdx=1; pieceIdx <= SysInfoPieceRange.m_nPieceCount[nDut]; pieceIdx++)
        InsertOpenTest(nDut, pieceIdx, openTestItem);   //son 공백인 openTestItem을 insert
    
    for (openIdx = 1; openIdx <= nOpenTestCount; openIdx++)
    {
        //son 대표핀과 TestPin이 같은 핀이면 테스트 진행 불가. 
        if(nBDLData2[nDut][openIdx][1] == nBDLData2[nDut][openIdx][2]) //sylee130515
            continue;

        //son 해당 대표핀의 piece번호를 알아낸다.
        firstPin1 = nBDLData2[nDut][openIdx][1] +1;    //son firstPin1 : pinNo (1~)
        piece    = g_nPinPieceNo[nDut][firstPin1];     //son pinNo에 해당하는 pieceNo(1~)

        openTestItem.wFirstPin = nBDLData2[nDut][openIdx][1];   //son 대표핀
        openTestItem.wPin      = nBDLData2[nDut][openIdx][2];   //son open 테스트해야할 pinNo

        // openTestItem을 nDut, piece에 해당하는 'piece별 OpenTest vector' 메모리에 추가한다.
        InsertOpenTest(nDut, piece, openTestItem);

    }
    for(pieceIdx=1; pieceIdx <= SysInfoPieceRange.m_nPieceCount[nDut]; pieceIdx++)  //son220524
    {
		MyTrace(PRT_BASIC, "g_pvBDLData2Op[%d][%d]->size()=%d\n",
					nDut, pieceIdx, g_pvBDLData2Op[nDut][pieceIdx]->size()); 
    }

#endif
}

//son Piece별 Short List 생성
//    piece정보를 이용해서 nBDLData3[nDUT]를  g_pvBDLData31[nDUT][piece] 에 InsertShortTest()해서 piece별로 재배치한다.
//son  nDut (1~)
//
//    주의! makePieceShortTestData(dut) 호출 이전에는 ReadPinPieceNo(dut)를 반드시 호출해서 
//    g_nPieceMax1 값과 g_nPinPieceNo[nDUT][firstPin1] 정보가 존재해야 한다. 
void CChildView1::makePieceShortTestData(int nDUT)    //son221103_2 ReadPinPieceNo()에서 함수내용  분리 
{
    CString str;

	//------------------------------
	// SHORT LIST를 PIECE별로 분리 
	//------------------------------

    int firstPin1 = 0;
    int piece = 0;
    int t4 = 0;

	//son200513: Piece 200인 경우에 Auto화면에서 bdl 파일에러, 및 Block-Piece 맵이 안 그려지는 문제, 죽는 문제 발생
	//             아래 array size가 200이면 arrPiecePinCnt[199]까지만 access가능하므로 죽을 수 있어서 +1하여 size를 키워줌.
	int arrPiecePinCnt[MAX_PIECE +1];//PIECE 증가 번호 //son 해당 piece의 pinCount, 
	::ZeroMemory(&arrPiecePinCnt, sizeof(arrPiecePinCnt));
	                                                //    index는 piece, value는 pin
	DeleteDutShortTestItem(nDUT);

    // 한번 정의해서 계속 재활용한다.(InsertShortTest()에서 실제 data 영역에 copy, Call by value)
    CShortTestItem shortTestItem; 


#ifdef __OPVECTOR__
    //son-20200420 :  (*g_pvBDLData31[nDut][piece])[piecePin] 의 원본인 
    //     nBDLData31[nDut][piece][piecePinIdx]를 ChildView7에서 사용할 때 
    //     모두 1부터 piecePin을 access하고 있음. ChildView7 영역을 모두 0으로 시작하게 바꾸는 것이
    //     리스크가 커서 메모리가 아깝지만,  vector도 1부터 시작하도록 수정하기로 함.
    //     초기화할때 첫번째 item이면 무조건 push_back을 먼저 1회 하고 시작하도록 수정한다.
    //     InsertShortTest() 함수 내부에서 index 0이면 두번 넣게 수정 -> 20200702 삭제 
    //
    //son-20200702 : 대표핀만 있는 BDL인 경우, OpenTest 없이 ShortTest만 하게 됨.  
    //     이경우에는  g_pvBDLData2Op[dutId][ip]->size() 가 0이어서 메모리 access하다 죽는 현상 발생함.
    //     ex) nTOpenSum1=nTOpenSum1 + (g_pvBDLData2Op[nMulNetFlag][ip]->size() -1);
    //         nBDLData2Op[nDut][piece][0][0] 를 (g_pvBDLData2Op[nDut][nPiece]->size() -1)로 대체한 것이 위험해짐.
    //    
    //     이 문제 관련해서 ShortTest도 비슷한 위험성이 있어서 ReadPinPieceNo()에서 모든 piece에 대해 
    //     무조건 InsertShortTest를 1회 하고 시작하도록 수정한다. (기본 size()가 1이 되게 ) 
    //     InsertShortTest() 함수 내부에서 index 0이면 push_back() 두번 넣게 했던 코드는 삭제함.
    
    //son 모든 piece에 대해 무조건 InsertShortTest()을 먼저 1회 넣는다. 
    for(int pieceIdx=1; pieceIdx <= SysInfoPieceRange.m_nPieceCount[nDUT]; pieceIdx++)
        InsertShortTest(nDUT, pieceIdx, shortTestItem);     //son 공백인  shortTestItem을 insert
#endif

	//son nBDLData3[dut][ShortPin][]을 piece별로 나누어서 nBDLData31[dut][piece][ShortPin]으로 재배치한다.
	int nShortTestCnt = nBDLData3[nDUT][0][1];
	if (nShortTestCnt > 0)//son 현재 DUT의 short 총수(=대표핀수).  max  32k
    {
		for (int shortIdx = 1; shortIdx <= nShortTestCnt; shortIdx++)
		{
		    //son 해당 대표핀의 piece번호를 알아낸다.
			firstPin1 = nBDLData3[nDUT][shortIdx][BDL_PIN0] +1;//SYLEE120910 //son 대표핀 (1~)
			piece     = g_nPinPieceNo[nDUT][firstPin1];				        //son 대표핀의 piece정보  (1~)
			arrPiecePinCnt[piece]++;                                        //son 해당 piece의 pinCount
			int piecePinIdx = arrPiecePinCnt[piece];

			//son pinNo를 nBDLData31[dut][piece][piecPinIdx] 에 write한다.
			if(piece <= g_nPieceMax1)
			{  
#ifndef __OPVECTOR__
				nBDLData31[nDUT][piece][piecePinIdx] = firstPin1 -1;//SYLEE120910
	            nBDLData31[nDUT][piece][0] = arrPiecePinCnt[piece];// PIECE별 Short 총 갯수
#else
                shortTestItem.wFirstPin  = firstPin1 -1;
                shortTestItem.wNetPinCnt = nBDLData3[nDUT][shortIdx][BDL_NET_PINCNT]; //son200722-nBDLData3-Sort: 신규추가


                // shortTestItem을  nDut, piece에 해당하는 'piece별 ShortTest vector' 메모리에 추가한다.
                InsertShortTest(nDUT, piece, shortTestItem);
#endif
                //son???? t4는 어디에 사용??
				if(t4 < (firstPin1-1)) {//SYLEE120910
					t4 = (firstPin1-1);//SYLEE120910
				}
			}
			else
			{
				//error message 
				str.Format("Error No 7304. \n\n SHORT PIECE SORTING ERROR. \n \n\n SHORT PIN PIECE NO.? \n\n   PIECE MAX. OVER.  \n\n  [SETUP]=>[PIECE POINT] Check"); 
				AfxMessageBox(str);
				return ;//sylee20120503
			}
		}

	}




}

//son200420-4WNET10000
//son piece별 open g_pvBDLData2Op[][][]를 "D:\\log\\0OpenPinListPiece_DUT%d" 에 출력한다. (for debug)
//    nDut는 1부터 시작.
int CChildView1::printPieceOpenTestData(int nDut)    //son200420-4WNET10000  
{

    FILE *fp; 
    CString str;
    char fName[200];
    int  nDutOpenTestCnt = 0;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //FILE OPEN FILE SAVE  DATE
 
    str.Format("D:\\log\\0OpenPinListPiece_DUT%d", nDut);   //son210622
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format("Error No 1014 :  %s  file Data Saving Error !     file close!.\n", fName);  //son210622
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return -1;
    }

    CTime curTime = CTime::GetCurrentTime();

    //fprintf(fp, "2011 \n" );
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  
                curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : DUT_%d Piece OPEN LIST  FILE \n", nDut);  

    //son pieceCount를 먼저 출력
    int pieceListCount;
    CString strTemp, strTemp2;
    for (int piece = 1; piece <= g_nPieceMax1; piece++) 
    {
        pieceListCount = 0;
#ifndef __OPVECTOR__ 
        pieceListCount = nBDLData2Op[nDut][piece][0][0];
        fprintf(fp, "* piece=%3d,  pieceListCount = %5d \n", piece, pieceListCount);

#else
        if (g_pvBDLData2Op[nDut][piece] == NULL) 
        {
            strTemp.Format(" g_pvBDLData2Op[dut_%d][piece_%d] is NULL. Error! \n\n", nDut, piece );   //son220317
            fprintf(fp, strTemp);

            strTemp2.Format(" Error No 3001: %s", strTemp);
            MyTrace(PRT_BASIC, strTemp2);
            AfxMessageBox(strTemp2, MB_OK);
            continue;
        }

        if (g_pvBDLData2Op[nDut][piece]->size() == 0)
        {
            strTemp.Format(" g_pvBDLData2Op[dut_%d][piece_%d]->size() = 0.  Error. size cannot be 0.\n\n", nDut, piece);
            fprintf(fp, strTemp);

            strTemp2.Format(" Error No 3002: %s", strTemp);
            MyTrace(PRT_BASIC, strTemp2);
            AfxMessageBox(strTemp2, MB_OK);
            continue;
        }
        
        //son-20200420 : vector를 0번째는 skip하고 1번째부터 사용하므로 size -1이 실제 pieceListCount이다.
        pieceListCount = (g_pvBDLData2Op[nDut][piece]->size() -1);
        fprintf(fp, "* piece=%3d,  pieceListCount = %5d \n", piece, pieceListCount);
        
#endif
        nDutOpenTestCnt += pieceListCount;
    }
    
    fprintf(fp, " ***>OPEN LIST Count = %d\n", nDutOpenTestCnt);

    fprintf(fp, " \n"); 
    fprintf(fp, " \n"); 
    fprintf(fp,"     Piece, wFirstPin,      wPin, \n"  );

    //son Piece, nFirstPin, nTestPin을 출력한다.
    for (piece = 1; piece <= g_nPieceMax1; piece++) 
    {
        pieceListCount = 0;
#ifndef __OPVECTOR__
        pieceListCount = nBDLData2Op[nDut][piece][0][0];
        for (int piecePin= 1; piecePin <= pieceListCount; piecePin++) 
        {
            fprintf(fp, "    %6d,    %6d,    %6d\n",
                    piece, nBDLData2Op[nDut][piece][piecePin][1]+1, nBDLData2Op[nDut][piece][piecePin][2]+1);  
        }
#else
        if (g_pvBDLData2Op[nDut][piece] == NULL) 
            continue;

        if (g_pvBDLData2Op[nDut][piece]->size() == 0)
            continue;

        //son-20200420 : vector를 0번째는 skip하고 1번째부터 사용하므로 size -1이 실제 pieceListCount이다.
        pieceListCount = (g_pvBDLData2Op[nDut][piece]->size() -1);

        //son 주의!  vector의 piecePin은 0부터 시작해야 하므로 
        //           기존 piecePin보다 1작은 값들을 사용해야 정상 동작한다.
        //for (int piecePin= 0; piecePin < pieceListCount; piecePin++) 
        
        //son-20200420 :  (*g_pvBDLData2Op[nDut][piece])[piecePin] 를 ChildView7에서 사용할때 
        //     모두 1부터 piecePin을 access하고 있음. ChildView7 영역을 모두 0으로 시작하게 바꾸는 것이
        //     리스크가 커서 메모리가 아깝지만,  vector도 1부터 시작하도록 수정하기로 함.
        //     초기화할때 1이면 무조건 push_back을 먼저 1회 하고 시작하도록 수정한다.
        for (int piecePin= 1; piecePin <= pieceListCount; piecePin++) 
        {
            fprintf(fp, "    %6d,    %6d,    %6d\n", 
                    piece, (*g_pvBDLData2Op[nDut][piece])[piecePin].wFirstPin+1, 
                           (*g_pvBDLData2Op[nDut][piece])[piecePin].wPin+1);
        }

#endif

    }

    fclose(fp);  

    return nDutOpenTestCnt;

}

//son200420-4WNET10000
//son piece별  g_pvBDLData31[][][]를 "D:\\log\\0ShortPinListPiece_DUT%d"  에 출력한다. (for debug)
//    nDut는 1부터 시작.
int CChildView1::printPieceShortTestData(int nDut)    //son200420-4WNET10000  
{
    int  nDutShortTestCnt = 0;

#ifdef __OPVECTOR__
    FILE *fp; 
    CString str;
    char fName[200];

	if( nDut==1) {//sylee230529
    	::ZeroMemory(&gnBDLData32, sizeof(gnBDLData32));	//sylee230529    
		::ZeroMemory(&gnBDLData33, sizeof(gnBDLData33));    //sylee230529
	}
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //FILE OPEN FILE SAVE  DATE
 
    str.Format("D:\\log\\0ShortPinListPiece_DUT%d", nDut);  //son210622
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format("Error No 1014 :   D:\\log\\0ShortPinListPiece_DUT%d   file Data Saving Error !     file close!.", nDut);    //son210622
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return -1;
    }

    CTime curTime = CTime::GetCurrentTime();

    //fprintf(fp, "2011 \n" );
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
            curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : DUT_%d Piece SHORT LIST  FILE \n", nDut);  

    //son pieceCount를 먼저 출력
    int pieceListCount;
    CString strTemp, strTemp2;
    for (int piece = 1; piece <= g_nPieceMax1; piece++) 
    {
        if (g_pvBDLData31[nDut][piece] == NULL) 
        {
            strTemp.Format(" g_pvBDLData31[dut_%d][piece_%d] is NULL. Error! \n\n", nDut, piece);   //son220317
            fprintf(fp, strTemp);

            strTemp2.Format(" Error No 3001: %s", strTemp);
            MyTrace(PRT_BASIC, strTemp2);
            AfxMessageBox(strTemp2, MB_OK);
            continue;
        }

        if (g_pvBDLData31[nDut][piece]->size() == 0)
        {
            strTemp.Format(" g_pvBDLData31[dut_%d][piece_%d]->size() = 0.  Error. size cannot be 0.\n\n", nDut, piece);
            fprintf(fp, strTemp);

            strTemp2.Format(" Error No 3002: %s", strTemp);
            MyTrace(PRT_BASIC, strTemp2);
            AfxMessageBox(strTemp2, MB_OK);
            continue;
        }

        //son-20200420 : vector를 0번째는 skip하고 1번째부터 사용하므로 size -1이 실제 pieceListCount이다.
        pieceListCount = (g_pvBDLData31[nDut][piece]->size() -1);
        fprintf(fp, "* piece=%3d,  pieceListCount = %5d \n", piece, pieceListCount);
        nDutShortTestCnt += pieceListCount;
        
    }
    
    fprintf(fp, " ***>SHORT LIST Count = %d\n", nDutShortTestCnt);

    fprintf(fp, " \n"); 
    fprintf(fp, " \n"); 
    fprintf(fp,"     Piece, FirstPin,      NetPinCount, \n"  );

    //son Piece, nFirstPin, nTestPin을 출력한다.
    for (piece = 1; piece <= g_nPieceMax1; piece++) 
    {
        if (g_pvBDLData31[nDut][piece] == NULL) 
            continue;

        if (g_pvBDLData31[nDut][piece]->size() == 0)
            continue;

        //son-20200420 : vector를 0번째는 skip하고 1번째부터 사용하므로 size -1이 실제 pieceListCount이다.
        pieceListCount = (g_pvBDLData31[nDut][piece]->size() -1);

        //son 주의!  vector의 piecePin은 0부터 시작해야 하므로 
        //           기존 piecePin보다 1작은 값들을 사용해야 정상 동작한다.
        //for (int piecePin= 0; piecePin < pieceListCount; piecePin++) 
        
        //son-20200420 :  (*g_pvBDLData31[nDut][piece])[piecePin] 를 ChildView7에서 사용할때 
        //     모두 1부터 piecePin을 access하고 있음. ChildView7 영역을 모두 0으로 시작하게 바꾸는 것이
        //     리스크가 커서 메모리가 아깝지만,  vector도 1부터 시작하도록 수정하기로 함.
        //     초기화할때 1이면 무조건 push_back을 먼저 1회 하고 시작하도록 수정한다.
        int nFirstPin1, nNetPinCnt;//sylee230529	//sylee230817
        for (int piecePin= 1; piecePin <= pieceListCount; piecePin++) 
        {
		    nFirstPin1=(*g_pvBDLData31[nDut][piece])[piecePin].wFirstPin+1;//sylee230529  //sylee230817
		    nNetPinCnt=(*g_pvBDLData31[nDut][piece])[piecePin].wNetPinCnt; //sylee230529
           //sylee230818-2   fprintf(fp, "    %6d,    %6d,   %6d\n", 
           //sylee230818-2          piece, (*g_pvBDLData31[nDut][piece])[piecePin].wFirstPin+1, 
           //sylee230818-2                (*g_pvBDLData31[nDut][piece])[piecePin].wNetPinCnt);
		    fprintf(fp, "    %6d,    %6d,   %6d\n", piece,  nFirstPin1,  nNetPinCnt);//sylee230529  //sylee230817
			if(nFirstPin1>0){//sylee230529
			      gnBDLData32[nDut][nFirstPin1][2]=nNetPinCnt;//sylee230529   net   pin count  //sylee230817
			}
        }


    }

    fclose(fp);  

#endif
    return nDutShortTestCnt;

}


int CChildView1::LeakMultPinList1(int nDut)    //sylee230526 //sylee230817
{

    int nNetCo1,nNetCo2,nNetPo1,nNetPo2,nNetTotal ;
    int  nMaxSetNo2;
    int nShortPin1,nOpenPin1,nOpenFirstPin1,nOpenFirstPin2;
    int nCo1, nStepPo1, nMaxPin1;


	FileSysInfo01.LoadSaveSet19(_LOAD);//sylee230818


    nMaxPin1=0;	nNetCo1=0; nNetCo2=0; nNetPo1=0; nNetPo2=0; nNetTotal=0; nStepPo1=0; 


    for (int piece = 1; piece <= g_nPieceMax1; piece++) 
    {
        if (g_pvBDLData31[nDut][piece] == NULL) 
            continue;

        if (g_pvBDLData31[nDut][piece]->size() == 0)
            continue;

		double dSetMpinPer;//sylee230602
		int nSetMpin1,nSetMpin2,nSetMpin1Co;//sylee230818

        if(SysSet19.m_nSet46>0){    
            dSetMpinPer=(double)SysSet19.m_nSet46/100.;//sylee230602
        }
        else{     
            dSetMpinPer=0.0;//sylee230602
        }

        nSetMpin1=(int)(dSetMpinPer*10);//sylee230818
        if(nSetMpin1==0){ 
            nSetMpin2=0;}
        else if(nSetMpin1==1){ 
            nSetMpin2=10;}
        else if(nSetMpin1==2){ 
            nSetMpin2=5;} //sylee230818-2
        else if(nSetMpin1==3){ 
            nSetMpin2=3;} //sylee230818-2
        else if(nSetMpin1==4){ 
            nSetMpin2=2;} //sylee230818-2
        else if(nSetMpin1==5){ 
            nSetMpin2=2;} //sylee230818-2
        else{ 
            nSetMpin2=1;} //sylee230818-2


        int pieceListCount = (g_pvBDLData31[nDut][piece]->size() -1);
        for (int piecePin=1; piecePin<=pieceListCount; piecePin++) 
        {		  

			
		  	nSetMpin1Co=0;//sylee230818-2

            // (*g_pvBDLData31[nDut][piece])[piecePin].wFirstPin+1,    // (*g_pvBDLData31[nDut][piece])[piecePin].wNetPinCnt);
            //sylee230529-2 if((*g_pvBDLData31[nDut][piece])[piecePin].wNetPinCnt>=nMaxSetNo1){
            nShortPin1=(*g_pvBDLData31[nDut][piece])[piecePin].wFirstPin+1;
            nCo1=0;
            //--------------------------------------------------------------------------------------------			  
            for (int piece2 = 1; piece2 <= g_nPieceMax1; piece2++) 
            {
                //sylee230529-1 pieceListCount = 0;
                if (g_pvBDLData2Op[nDut][piece2] == NULL) 
                    continue;
                if (g_pvBDLData2Op[nDut][piece2]->size() == 0)
                    continue;
                int pieceListCount2 = (g_pvBDLData2Op[nDut][piece2]->size() -1);




                for (int piecePin2= 1; piecePin2<=pieceListCount2;  piecePin2++) 
                { 						

                   //sylee230817   nOpenFirstPin1=(*g_pvBDLData2Op[nDut][piece2])[piecePin2].wNetFirstPin+1;
					nOpenFirstPin1=(*g_pvBDLData2Op[nDut][piece2])[piecePin2].wFirstPin+1;//sylee230817 
                    nOpenFirstPin2=(*g_pvBDLData2Op[nDut][piece2])[piecePin2].wFirstPin+1;
                    nOpenPin1=0;


                    if( (nShortPin1==nOpenFirstPin1)  &&   (nOpenFirstPin1==nOpenFirstPin2 ) ){

                        nOpenPin1=(*g_pvBDLData2Op[nDut][piece2])[piecePin2].wPin+1; 
                        nMaxSetNo2= (int) ( gnBDLData32[nDut][nOpenFirstPin1][2]*dSetMpinPer); //sylee230602

						if(nSetMpin1Co<nSetMpin2){ nSetMpin1Co++; } //sylee230818-2
						else{ nSetMpin1Co=1;   } //sylee230818-2
					//SYLEE230818-3	if(nSetMpin1Co != nSetMpin1){	continue;	} //sylee230818-2
					    nCo1++;

                        if( nCo1<= nMaxSetNo2 ){ 
                            nStepPo1++;
                            if(nStepPo1<=5000){
                                if( nMaxPin1<nOpenFirstPin2 ){ nMaxPin1=nOpenFirstPin2; }
                                gnBDLData33[nDut][nStepPo1][0]=piece2;
                                gnBDLData33[nDut][nStepPo1][1]=nOpenFirstPin2;
                                gnBDLData33[nDut][nStepPo1][2]=nOpenPin1;
                                gnBDLData33[nDut][0][0]=nStepPo1;							
                                gnBDLData33[nDut][0][1]=nMaxPin1;
                                if( nCo1==1 ){ 
                                    gnBDLData32[nDut][nOpenFirstPin2][0]=nStepPo1;
                                }else if( nCo1==nMaxSetNo2){  
                                    gnBDLData32[nDut][nOpenFirstPin2][1]=nStepPo1;
                                }
                            } 								 
                        }

                    }else{
                        nCo1=0;
                    }


                }


            }

            //---------------------------------------------------------------------------------------------
            //sylee230529-2  }

        }


    }


    LeakMultPinList1_File(nDut); //sylee230526

    return 1;
}







int CChildView1::LeakMultPinList1_File(int nDut) //sylee230526 //sylee230817
{

    FILE *fp; 
    CString str;
    char fName[200];


    str.Format("D:\\log\\0Short2_PinListPiece_DUT%d", nDut);  //son210622
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format("Error No 1014 :   D:\\log\\0ShortPinListPiece_DUT%d   file Data Saving Error !     file close!.", nDut);    //son210622
        AfxMessageBox(str, MB_OK);			
        return -1;
    }
    CTime curTime = CTime::GetCurrentTime(); 
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
            curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : DUT_%d Piece SHORT * 2 LIST  FILE \n", nDut);    
    fprintf(fp, " ***>SHORT LIST Count = %d\n", gnBDLData33[nDut][0][0]); 
    fprintf(fp,"  \n \n  piece,    FirstPin,  Pin2  , \n"  );

    for (int nStep = 1; nStep <= gnBDLData33[nDut][0][0]; nStep++) 
    {
        if( (gnBDLData33[nDut][nStep][0] <=0)  || (gnBDLData33[nDut][nStep][1] <=0) || (gnBDLData33[nDut][nStep][2]<=0) )
            continue;
        fprintf(fp,"    %6d,    %6d,   %6d\n",gnBDLData33[nDut][nStep][0],gnBDLData33[nDut][nStep][1],gnBDLData33[nDut][nStep][2]);
    }
    fclose(fp);  

    //====================


    str.Format("D:\\log\\0Short2_2_PinListPiece_DUT%d", nDut);  //son210622
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format("Error No 1014-2 :   D:\\log\\0Short2_2_PinListPiece_DUT%d   file Data Saving Error !     file close!.", nDut);    //son210622
        AfxMessageBox(str, MB_OK);			
        return -1;
    }

    curTime = CTime::GetCurrentTime(); 

    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : DUT_%d Piece SHORT * 2 LIST  FILE \n", nDut);    
    fprintf(fp, " ***>SHORT LIST Count = %d\n", gnBDLData33[nDut][0][0]); 
    fprintf(fp,"  \n \n  pin ,    po1~,  po2  , \n"  );


    for (int nStep2 = 1; nStep2 <= gnBDLData33[nDut][0][1]; nStep2++) 
    {
        if( (gnBDLData32[nDut][nStep2][0] <=0)  || (gnBDLData32[nDut][nStep2][1] <=0)   )
            continue;
        fprintf(fp,"   %6d,   %6d,    %6d \n",nStep2, gnBDLData32[nDut][nStep2][0],gnBDLData32[nDut][nStep2][1] );
    }
    fclose(fp);  

    //===========================================

    return 1;

}


//son2000722 :  nBDLData1에 대한 정렬코드. 현재 미사용. 
//son NetCount가 적은 Net부터 nBDLData1 전체를 정렬한다.
void CChildView1::Sort_nBDLData1_byNetPinCount(int nDut)
{

    if (SysSet13.m_nBdlSortMode == NO_SORT)
        return;

    //son 먼저 piece 내부를 NetCount가 
    //son 먼저 Piece 내부에서 NetCount가 적은 Net부터  nBDLData1을 다시 Sorting한다.
    //Sort_nBDLData1_byNetPinCount_Piece();


    //son NetCount가 적은 piece부터 nBDLData1을 다시 Sorting한다.
    //son piece는 진짜로 sorting하긴 어렵고, sort된 정보를 여기서 만들고,  
    //    실제 openTest, ShortTest에서 sort된 순서대로 test 하도록 해야 함.


    int pin1, pin2; 
    int netPinStart, netPinEnd;         //son 같은 net의 pin 시작위치, pin End 위치
    int netPinCnt, netPinCntMin, netPinCntMax; 
    int nPos;                   //son max 또는 min 으로 골라낸  위치

    //son 정렬결과 저장. sortedNetCount를 하나씩 증가시키면서 index로 쓴다.
    short wSortedNetData[DEF_MAX_PIN1][2];   
    int   sortedNetCount = 0;
    ::ZeroMemory(&wSortedNetData, sizeof(wSortedNetData));

    //son 이미 정렬되어 뽑혀 갔음을 표시.  
    //    nBDLData1[nDut][pin][] 리스트와 배치가 같음. index는 pin
    bool  bSorted[DEF_MAX_PIN1];     
    ::ZeroMemory(&bSorted, sizeof(bSorted));

    int bdlPinEnd =  nBDLHead1[nDut][3][1];                //son BDL의 pin 갯수.
    if (bdlPinEnd <= 0 || bdlPinEnd >= DEF_MAX_PIN1)     //son Check Range
        return;


    for (pin1=1; pin1 <= bdlPinEnd; pin1++)
    {
        netPinCntMin= 0;
        netPinCntMax= 0;
        nPos= 0;
        for (pin2=1; pin2 <= bdlPinEnd; pin2++)
        {

            //son 이미 정렬되어 뽑혀 갔으므로 넘어간다.
            if(bSorted[pin2] == true)
                continue;
            

            //son 현재 pin과 같은 net인 pin 범위를 확인하고 net 단위로 pin을 점프하면서 확인한다.
            netPinStart = pin2;
            netPinEnd   = netPinStart + nBDLData1[nDut][pin2][BDL_NET_PINCNT] -1;
            if (netPinStart > netPinEnd)//sylee131105
            {
                //str.Format(" **> Error!   BDL FILE,   TOTAL POINT CHECK !!! ");
                //m_list1.InsertString(0,str);
                //AfxMessageBox("  ERROR 1111,  BDL NET LIST LOADING ERROR.      BDL FILE, TOTAL POINT  CHECK!  ", MB_OK);          
                break;//sylee131128 continue;//errr?
            }


            //son 내림차순이면 netPinCnt가 제일 큰 net을 pick해서 해당 pin들을 sorted 표시한다.
            //if (SysSet13.m_nBdlSortMode == SORT_MODE_DESCEND)
            //{
                netPinCnt =  nBDLData1[nDut][pin2][BDL_NET_PINCNT]; 
                if (nPos == 0)
                {
                    netPinCntMax = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                    continue;
                }

                // 현재 netPinCntMax보다 큰 값을 찾았다면 netPinCntMax를 교체하고 
                // pin2를 nPos에 저장한다.  (maxSize가 같은 경우도 리스트에 저장해야 함)
                if (netPinCnt > netPinCntMax)
                {
                    netPinCntMax = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                }

            //}
            //son pin2 점프: NetEnd 위치로 pin2를 옮겨간다. 
            pin2 = netPinEnd;       

        }

        //son 최적값을 찾았으면 정렬되었다고 표시
        if(nPos > 0){ 
            wSortedNetData[sortedNetCount][0] = nPos;
            wSortedNetData[sortedNetCount][1] = netPinCntMax;
            sortedNetCount++;

            for (int pin=nPos; pin < (nPos + netPinCntMax); pin++)
                bSorted[pin] = true;      //son min 값으로 뽑혀간 위치라고 해당 pin과 같은 net인 pin들에 표시
        }

    }
#if 0
    //son wSortedNetData[]를 디버그를 위해 출력 
    FILE *fp; 
    char  fName[200]; 
    CString str;
    int pinPos;
 
    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("Z:\\Sort_NetData_DUT%d.TXT", nDut);
    strcat(fName, str); 

    fp = fopen(fName,"wt");
    if(fp == NULL){
        str.Format("Error!  Can't open %s ", fName);
        AfxMessageBox(str, MB_OK);
        return;
    }
    fprintf(fp, "<<Sort_NetData Dut_%d>>\n", nDut);
    for (int net= 0; net < sortedNetCount; net++)
    {
        pinPos = wSortedNetData[net][0];  
        netPinCnt = wSortedNetData[net][1];
        fprintf(fp, "pinPos=%d, pin[pinPos]=%d,  netPinCnt=%d, piece=%d\n", 
                pinPos, nBDLData1[nDut][pinPos][BDL_PIN0], netPinCnt, nBDLData1[nDut][pinPos][BDL_PIECE]);
    }
    fclose(fp);
#endif
    
    //son wSortedNetData[]를 확인해서 nBdlData1[nDut]를 다시 재배치한다.
}

//son200722-BDL-Sort
//son NetCount의 갯수 기준으로   nBDLData3 전체를 정렬한다.   
//    default는 내림차순이며  NetCount가 많은 순. 오름차순도 선택할 수 있다.
void CChildView1::Sort_nBDLData3_byNetPinCount(int nDut)
{

    if (SysSet13.m_nBdlSortMode != SORT_MODE_DESCEND 
            && SysSet13.m_nBdlSortMode != SORT_MODE_ASCEND)
        return;

    if (nBDLData3[nDut][0][1] <= 0)     //son ShortPin 총수
        return;



    int pin1, pin2; 
    int netPinCnt, netPinCntMin, netPinCntMax; 
    int nPos;                   //son max 또는 min 으로 골라낸  위치

    //son 정렬결과 저장. sortedNetCount를 하나씩 증가시키면서 index로 쓴다.
    short wSortedNetData[DEF_MAX_PIN1][4];       //index 0: pinPos,  1:pin,   2: netPinCnt,  3: piece
    int   sortedNetCount = 0;
    ::ZeroMemory(&wSortedNetData, sizeof(wSortedNetData));

    //son 이미 정렬되어 뽑혀 갔음을 표시.  
    //    nBDLData3[nDut][pin][] 리스트와 배치가 같음. index는 pin
    bool  bSorted[DEF_MAX_PIN1];     
    ::ZeroMemory(&bSorted, sizeof(bSorted));

    int shortPinCnt =  nBDLData3[nDut][0][1] ;              //son shortPin(대표핀)  갯수.
    if (shortPinCnt <= 0 || shortPinCnt >= DEF_MAX_PIN1)     //son Check Range
        return;


    for (pin1=1; pin1 <= shortPinCnt; pin1++)
    {
        netPinCntMin= 0;
        netPinCntMax= 0;
        nPos= 0;
        for (pin2=1; pin2 <= shortPinCnt; pin2++)
        {

            //son 이미 정렬되어 뽑혔으므로  넘어간다.
            if(bSorted[pin2] == true)
                continue;

            netPinCnt = nBDLData3[nDut][pin2][BDL_NET_PINCNT]; 

            //son 내림차순이면 netPinCnt가 제일 큰 net을 pick해서  sorted 표시한다.
            if (SysSet13.m_nBdlSortMode == SORT_MODE_DESCEND)
            {
                if (nPos == 0)
                {
                    netPinCntMax = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                    continue;
                }

                // 현재 netPinCntMax보다 큰 값을 찾았다면 netPinCntMax를 교체하고 pin2를 nPos에 저장한다.  
                if (netPinCnt > netPinCntMax)
                {
                    netPinCntMax = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                }
            }

            //son 오름차순이면 netPinCnt가 제일 작은  net을 pick해서 sorted 표시한다.
            else
            {
                //netPinCnt = nBDLData3[nDut][pin2][BDL_NET_PINCNT]; 
                if (nPos == 0)
                {
                    netPinCntMin = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                    continue;
                }

                // 현재 netPinCntMin보다 작은 값을 찾았다면 netPinCntMin를 교체하고  pin2를 nPos에 저장한다.  
                if (netPinCnt < netPinCntMin)
                {
                    netPinCntMin = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                }
            }


        }

        //son 최적값을 찾았으면 정렬되었다고 표시
        if(nPos > 0)
        { 
            wSortedNetData[sortedNetCount][0] = nPos;
            wSortedNetData[sortedNetCount][1] = nBDLData3[nDut][nPos][BDL_PIN0];

            if (SysSet13.m_nBdlSortMode == SORT_MODE_DESCEND) //son 내림차순
                wSortedNetData[sortedNetCount][2] = netPinCntMax;
            else //son 오름차순
                wSortedNetData[sortedNetCount][2] = netPinCntMin;

            wSortedNetData[sortedNetCount][3] = nBDLData3[nDut][nPos][BDL_PIECE];
            sortedNetCount++;

            //son max 값으로 뽑혀간 위치라고 표시 
            bSorted[nPos] = true; 
        }

    }

    CString str;
    int     net;    //son ShortTest 이므로 shortPin하나 하나가 net의 대표핀이다. 
#if 0
    //son wSortedNetData[]를 디버그를 위해 출력 
    FILE *fp; 
    char  fName[200]; 
 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , SysInfoView01.m_pStrFilePathBDL );//SYLEE121202

    str.Format("\\Sort_nBdlData3_DUT%d.TXT", nDut);
    strcat(fName, str); 

    fp = fopen(fName,"wt");
    if(fp == NULL){
        str.Format("Error!  Can't open %s ", fName);
        AfxMessageBox(str, MB_OK);
        return;
    }
    fprintf(fp, "<<Sort_nBdlData3 : Dut_%d>>\n", nDut);
    int     pinPos; 
    int     pin, piece;
    for (net= 0; net < sortedNetCount; net++)
    {
        pinPos    = wSortedNetData[net][0];  
        pin       = wSortedNetData[net][1];  
        netPinCnt = wSortedNetData[net][2];
        piece     = wSortedNetData[net][3];
        fprintf(fp, "pinPos=%d,  pin[pinPos]=%d,  netPinCnt=%d,  piece=%d\n", 
                    pinPos, pin, netPinCnt, piece);
    }
    fclose(fp);
#endif
    
    if (nBDLData3[nDut][0][1] != sortedNetCount)
    {
        str.Format("Error! Sort_nBDLData3_byNetPinCount(): nBDLData3[%d][0][1](%d) != sortedNetCnt(%d)\n", 
                        nDut, nBDLData3[nDut][0][1], sortedNetCount);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return;
    }

    //son wSortedNetData[]로 nBdlData3[nDut]를 다시 재배치한다.
  
    ::ZeroMemory(&nBDLData3[nDut], sizeof(nBDLData3[nDut]));
    nBDLData3[nDut][0][1] = sortedNetCount;
    for (net= 0; net < sortedNetCount; net++)
    {    
        nBDLData3[nDut][net+1][BDL_PIN0]       = wSortedNetData[net][1];      //son pin       
        nBDLData3[nDut][net+1][BDL_NET_PINCNT] = wSortedNetData[net][2];      //son netPinCnt 
        nBDLData3[nDut][net+1][BDL_PIECE]      = wSortedNetData[net][3];      //son piece     
    }

}


//son200722-BDL-Sort
//son NetCount의 갯수 기준으로 Piece별 shortTest Data인 nBDLData31[nDut]를   정렬한다.   
//    default는 내림차순이며  NetCount가 많은 순. 오름차순도 선택할 수 있다.
void CChildView1::Sort_nBDLData31_byNetPinCount(int nDut)
{

    int nPieceMax1=SysInfoPieceRange.m_nPieceCount[1];  
    if (nPieceMax1 < 1)
        return;
    
    int piece;
    for(piece=1; piece<=nPieceMax1; piece++) 
    {
        Sort_nBDLData31_byNetPinCount_piece(nDut, piece);

		MyTrace(PRT_BASIC, "g_pvBDLData31[%d][%d]->size()=%d\n",            //son220524
					nDut, piece, g_pvBDLData31[nDut][piece]->size()); 

    }

    //son printPieceShortTestData()와 동일하므로 출력 막음.
#if 0
    //son wSortedNetData[]를 디버그를 위해 출력 
    FILE *fp; 
    char  fName[200]; 
    CString str;
 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , SysInfoView01.m_pStrFilePathBDL );//SYLEE121202

    str.Format("\\Sort_nBdlData31_DUT%d.TXT", nDut);
    strcat(fName, str); 

    fp = fopen(fName,"wt");
    if(fp == NULL){
        str.Format("Error!  Can't open %s ", fName);
        AfxMessageBox(str, MB_OK);
        return;
    }
    fprintf(fp, "<<Sort_nBdlData31 : Dut_%d>>\n", nDut);
    int  pin, netPinCnt, shortPinCnt;
    int  net;    //son ShortTest 이므로 shortPin하나 하나가 net의 대표핀이다. 
    for(piece=1; piece<=nPieceMax1; piece++) 
    {
        shortPinCnt =  (g_pvBDLData31[nDut][piece]->size() -1); 

        for (net= 0; net < shortPinCnt; net++)
        {
            pin       = (*g_pvBDLData31[nDut][piece])[net+1].wFirstPin;  
            netPinCnt = (*g_pvBDLData31[nDut][piece])[net+1].wNetPinCnt;
            fprintf(fp, "net=%d,  pin=%d,  netPinCnt=%d,  piece=%d\n", 
                        net, pin, netPinCnt, piece);
        }
    }
    fclose(fp);
#endif
}


//son200722-BDL-Sort
void CChildView1::Sort_nBDLData31_byNetPinCount_piece(int nDut, int nPiece)
{

    if (SysSet13.m_nBdlSortMode != SORT_MODE_DESCEND 
            && SysSet13.m_nBdlSortMode != SORT_MODE_ASCEND)
        return;


#if 1
    //son piece별 Short pin 총수 Check
    int shortPinCnt =  (g_pvBDLData31[nDut][nPiece]->size() -1);    //son piece별 shortPin(대표핀)  갯수.
    if (shortPinCnt <= 0 || shortPinCnt >= DEF_MAX_PIN1)             //son Check Range
        return;


    int pin1, pin2; 
    int netPinCnt, netPinCntMin, netPinCntMax; 
    int nPos;                   //son max 또는 min 으로 골라낸  위치

    //son 정렬결과 저장. sortedNetCount를 하나씩 증가시키면서 index로 쓴다.
    short wSortedNetData[DEF_MAX_PIN1][4];       //index 0: pinPos,  1:pin,   2: netPinCnt,  3: piece
    int   sortedNetCount = 0;
    ::ZeroMemory(&wSortedNetData, sizeof(wSortedNetData));

    //son 이미 정렬되어 뽑혀 갔음을 표시.  
    //    nBDLData3[nDut][pin][] 리스트와 배치가 같음. index는 pin
    bool  bSorted[DEF_MAX_PIN1];     
    ::ZeroMemory(&bSorted, sizeof(bSorted));



    for (pin1=1; pin1 <= shortPinCnt; pin1++)
    {
        netPinCntMin= 0;
        netPinCntMax= 0;
        nPos= 0;
        for (pin2=1; pin2 <= shortPinCnt; pin2++)
        {
            //son 이미 정렬되어 뽑혔으므로  넘어간다.
            if(bSorted[pin2] == true)
                continue;

            netPinCnt = (*g_pvBDLData31[nDut][nPiece])[pin2].wNetPinCnt; 

            //son 내림차순이면 netPinCnt가 제일 큰 net을 pick해서  sorted 표시한다.
            if (SysSet13.m_nBdlSortMode == SORT_MODE_DESCEND)
            {
                if (nPos == 0)
                {
                    netPinCntMax = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                    continue;
                }

                // 현재 netPinCntMax보다 큰 값을 찾았다면 netPinCntMax를 교체하고 pin2를 nPos에 저장한다.  
                if (netPinCnt > netPinCntMax)
                {
                    netPinCntMax = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                }
            }

            //son 오름차순이면 netPinCnt가 제일 작은  net을 pick해서 sorted 표시한다.
            else
            {
                if (nPos == 0)
                {
                    netPinCntMin = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                    continue;
                }

                // 현재 netPinCntMin보다 작은 값을 찾았다면 netPinCntMin를 교체하고  pin2를 nPos에 저장한다.  
                if (netPinCnt < netPinCntMin)
                {
                    netPinCntMin = netPinCnt;
                    nPos = pin2;        //son pin2 위치(net Start 위치)를 저장 
                }
            }


        }

        //son 최적값을 찾았으면 정렬되었다고 표시
        if(nPos > 0)
        { 
            wSortedNetData[sortedNetCount][0] = nPos;
            wSortedNetData[sortedNetCount][1] = (*g_pvBDLData31[nDut][nPiece])[nPos].wFirstPin;

            if (SysSet13.m_nBdlSortMode == SORT_MODE_DESCEND) //son 내림차순
                wSortedNetData[sortedNetCount][2] = netPinCntMax;
            else //son 오름차순
                wSortedNetData[sortedNetCount][2] = netPinCntMin;

            sortedNetCount++;

            //son max 값으로 뽑혀간 위치라고 표시 
            bSorted[nPos] = true; 
        }

    }

    CString str;
    if (shortPinCnt != sortedNetCount)
    {
        str.Format("Error! Sort_nBDLData3_byNetPinCount_piece(): g_pvBDLData31[%d][%d] shortPinCnt(%d) != sortedNetCnt(%d)\n", 
                        nDut, nPiece, shortPinCnt, sortedNetCount);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return;
    }

    //son wSortedNetData[]로 nBdlData31[nDut][nPiece]를 다시 재배치한다.
#if 1    
    ZeroPieceShortTestItem(nDut, nPiece);
    CShortTestItem   shortTestItem;
    InsertShortTest(nDut, nPiece, shortTestItem);   //son 공백을 무조건 한번 Insert한다.

    for (int net= 0; net < sortedNetCount; net++)
    {    
        shortTestItem.wFirstPin  = wSortedNetData[net][1];      //son pin 
        shortTestItem.wNetPinCnt = wSortedNetData[net][2];      //son netPinCnt 
        InsertShortTest(nDut, nPiece, shortTestItem); 
    }
#else
    ::ZeroMemory(&nBDLData31[nDut][nPiece], sizeof(short)* DEF_MAX_PIN2);
    nBDLData31[nDut][nPiece][0] = sortedNetCount;
    for (int net= 0; net < sortedNetCount; net++)
    {    
        nBDLData31[nDut][nPiece][net+1]        = wSortedNetData[net][1];      //son pin       
        nBDLData31_netCnt[nDut][nPiece][net+1] = wSortedNetData[net][2];      //son netPinCnt 
    }
#endif

#endif
}

//#############################################################################################
 
void CChildView1::OnDataLoadAuto()  //open button// sylee20130228
{       

    FILE *fp; 
    CFile file;
    int  i,j ;
    char fName[200]; 
    char szTemp1[100];

    CString str,strVal1; 

    long Sum1,Sum2;// ##3
    int nTotal1,nTotal2,Namo1,nLibEnable;//sylee151202


    //  nErrPin_Fileload1=1;//SYLEE170810-1
    nLibEnable=1;//sylee151202

    gFlag_Start_Int_bdlError = 0; //son BDL 파일 에러 표시

    ::ZeroMemory(&nBDLHead1, sizeof(nBDLHead1));
    ::ZeroMemory(&nBDLData1, sizeof(nBDLData1));
    ::ZeroMemory(&nBDLData2, sizeof(nBDLData2));//sylee20120403
    ::ZeroMemory(&nBDLData3, sizeof(nBDLData3));//sylee20120403
    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&szTemp1, sizeof(szTemp1));

#ifdef __OPVECTOR__
    //son g_pvBDLData2Op vector 전체를 초기화한다.
    DeleteAllOpenTestItem();        //son200420-4WNET10000

    //son g_pvBDLData31 vector 전체를 초기화한다.
    DeleteAllShortTestItem();        //son200420-4WNET10000
#endif

    //-------------------------------
    //son Piece Range 정보 Loading 
    

    //son struct CSysInfoPieceRange에 "\SETUP2\PieceRange.ini"를  Load(type=2)
    FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD);//sylee130228import. //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo

    //son210430 Piece.ini와 PieceRange.ini 불일치시 SW 다운 문제 보완
    FileSysInfo01.LoadSavePieceInfo(_LOAD);//son210430 추가 
    CString str2 = "";
    if (SysInfoPieceRange.m_nPieceCount[1] != SysInfoPiece.m_nPiece)    //son210430
    {
        str.Format("Error No 1020  :  \n\n PieceCount info between PieceRange.ini(=%d) and Piece.ini(=%d) is different!\n", 
                                                            SysInfoPieceRange.m_nPieceCount[1], SysInfoPiece.m_nPiece );
        str2 += str;
        str.Format("\n%s BDL PieceCount Error!. Check .\\Setup2\\PieceRange.ini and Piece.ini.\n", SysInfoView01.m_pStrFilePathBDL);
        str2 += str;
        MyTrace(PRT_BASIC, str2);        //son211022
        AfxMessageBox(str2, MB_OK);         

        //son BDL 파일 에러 표시
        gFlag_Start_Int_bdlError = 1;
        return ;
    }

                                
    nProOneP_ModelCount1=0; 
    ::ZeroMemory(&nProOneP_Short1_Co, sizeof(nProOneP_Short1_Co));
    ::ZeroMemory(&nProOneP_Short1_Li, sizeof(nProOneP_Short1_Li));
    ::ZeroMemory(&nProOneP_Open1, sizeof(nProOneP_Open1));


    //---------------------------
    //son  SETUP Recipe Loading

    //son "\SETUP2\Recipe.ini"를 struct CSysInfo05에 Load(type=2)
    FileSysInfo01.LoadSaveSub15(_LOAD);//SYLEE140110 

    nProOneP_ModelCheck1Retry=0;//sylee131018
    nProOneP_ModelCheck1=0;//sylee131018

    if(SysInfo05.m_nProSpMode==2 || SysInfo05.m_nProSpMode==4){//sylee130916
        nProOneP_Flag1=1;//model start
        nProOneP_RunFlag1=1;
    }
    else{
        nProOneP_Flag1=0;
        nProOneP_RunFlag1=0;
    }

    if(  SysInfo05.m_nProSpMode !=3){ //sylee140210 
        nFlagEstModelMake=0;
        nFlagEstModelMakeSelect=0;
    }



    //----------------------------------------------------------------
    //son SysInfoView01.m_pStrFilePathJig2 의 BDL 파일을 file Open

    if(SysInfoView01.m_pStrFilePathJig2[0] == NULL ){ //sylee140403  
        str.Format("Error No 1009  :  \n\n  NULL ==> SysInfoView01.m_pStrFilePathJig2 , \n\n Open File Reading  !." );
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return ;
    }


    strcat( fName , SysInfoView01.m_pStrFilePathJig2 );



    fp=fopen(fName,"rt");
    if(fp == NULL){ 
        //son OnDataLoadAuto() 시에 fopen() 실패했다는 의미로 -1 설정
        nFlagFileLoadok=-1;//sylee160519
        str.Format("\n\nError 1001   :  BDL File Open Error !. \n\n  BDL File Open please!  \n\n ==> ..\\BDL\\%s \n\n ", fName);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return ;
    }
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig2, sizeof(SysInfoView01.m_pStrFilePathJig2));
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1, sizeof(SysInfoView01.m_pStrFilePathJig1));

    strcat( SysInfoView01.m_pStrFilePathJig2, fName );  //SYLEE20111212 
    strcat( SysInfoView01.m_pStrFilePathJig1, SysInfoView01.m_pStrFilePathJig2);
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);
    m_LabelPath.SetCaption(str);


    //-----------------------------------
    //son  현재 FilePathJig  Path 를 저장
    //
    //son struct CSysInfoView01를 "C:\ACE400\SETUP\FilePathJig.ini"에 Save(type=1)
    FileSysInfo01.LoadSaveView01(1);//SYLEE121202

    if( nLibEnable==2)//sylee151202
    {
        //char  fName[200]; 
        CString str1,str2;  //##2
        HFILE   hFile;
        int ret;


        str2.Format("%s",SysInfoView01.m_pStrFilePathJig2);
        str2.TrimRight('l');str2.TrimRight('L');//sylee150308
        str2.TrimRight('d');str2.TrimRight('D');//sylee150308
        str2.TrimRight('b');str2.TrimRight('B');//sylee150308
        str2.TrimRight('.'); //sylee150308
        str1.Format("%s.lib",str2);//sylee150308

        ::ZeroMemory(&nData2, sizeof(nData2)); 
        ::ZeroMemory(&fName, sizeof(fName));
        //str = BDL_PATH + m_strFileName+"\\Lock_"+m_strFileName+".Lib";
        str=str1;
        strcat( fName ,  str );  

        hFile = _lopen(str, OF_READWRITE | OF_SHARE_DENY_NONE);
        if(hFile == -1) {
            str.Format("Error No 201 :   File Read error!. -> %s  ",str);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            _lclose(hFile);
            return ; 
        }

        ret= _lread(hFile, (LPVOID)&nData2, (UINT)sizeof(nData2));
        if(nData2[0][0]<1){
            //fprintf(fp , " ---> ERROR!,  DATA EMPTY!"  );
            //_lclose(hFile); 
            str.Format("Error No 202 :   File Read error!. -> %s   data empty!",str);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            _lclose(hFile);
            return ;
        }
        _lclose(hFile);

        nBDLHead1[1][1][1]=nData2[0][1];
        nBDLHead1[1][2][1]=nData2[0][2];//pin total 
        nBDLHead1[1][2][2]=nData2[0][3];
        nBDLHead1[1][2][3]=nData2[0][4];
        nBDLHead1[1][3][1]=nData2[0][2];


        nHpin=0;
        nLpin=0;//sylee050309

        if( nData2[0][3]>0){//sylee050309
            nHpin=nData2[0][3];
        }
        if( nData2[0][4]>0){//sylee050309
            nLpin=nData2[0][4];
        }

    }
    //--------------------
    //son Read BDL Head             
    //--------------------
    else{//if( LibEnable==2){//sylee151202
        fscanf(fp, "%d\n", &nBDLHead1[1][1][1]);        //son  ex) 20000        (BDL 파일 표시) 
        fscanf(fp, "%d"  , &nBDLHead1[1][2][1]);        //son  ex) 32  32  0    (총핀수, 상부핀수, 하부핀수)
        fscanf(fp, "%d"  , &nBDLHead1[1][2][2]);
        fscanf(fp, "%d"  , &nBDLHead1[1][2][3]);            
        fscanf(fp, "%d\n", &nBDLHead1[1][3][1]);//list sum
    }//if( LibEnable==2){//sylee151202  end


    //-------------------------
    //son BDL 파일 에러 체크 

    //son BDL 파일이 아니라면
    if(nBDLHead1[1][1][1]!=20000){
        str.Format("Error No 1009  : BDL FILE ?   HEADER  DATA 20000?   Reading ERROR  !.\n  ..\\BDL\\%s", fName);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
        return  ;
    }


    //son 총핀수가 1보다 작다면
    if(nBDLHead1[1][3][1]<1){
        nBDLHead1[1][3][1]=0;
        str.Format("Error No 1113  :   BDL FILE.  HEADER Info.:  Pin Totle list No. data zero.  Reading ERROR  !. \n         ..\\BDL\\%s", fName);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
        return  ;
    }

    ////SYLEE120905//////////////////////////////////////

    char fName3[200];//, fName4[200]; 
    //  CString str1;

    ::ZeroMemory(&fName3, sizeof(fName3));

    ::ZeroMemory(nNetList1, sizeof(nNetList1)); //SYLEE120902

    ::ZeroMemory(PinSearchPin, sizeof(PinSearchPin)); //sylee130518-1 pin search bdl



    //son230224 short nTmp1;//sylee130518-1 pin search bdl

    //      long Sum1,Sum2;// ##3
    //      int nTotal1,Namo1;


    if( nLibEnable==2)//sylee151202
    {

        Sum1=0; Sum2=0; Namo1=0;

        nTotal1=nBDLHead1[1][3][1];
        for(i=1; i<=nBDLHead1[1][3][1]; i++)
        {
            if(i> MAX_PIN){//SYLEE150917-3
                str.Format(" Error No 1030:  auto load.  *.bdl   loading fail PIN Line(=%d) > %d.\n Check .BDL FILE PIN Line count!", i, MAX_PIN);      //son210831
                m_list1.AddString(str);
                MyTrace(PRT_BASIC, str);        //son211022
                AfxMessageBox( str );  
                return ;
            }           


            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                //sylee150308   fscanf(fp, "%d", &nBDLData1[1][i][j]); 
                //3,4,5 error 추가할것...?
                nBDLData1[1][i][j]= nData2[i][j-1];//sylee150308   //##4
            }


            //son230224 PinSearchPin[]의 index는 hwPin이므로 jigPin index로 1로 초기화하는 부분은 삭제해야 함.
            //son230224 //nTmp1=nBDLData1[1][i][1];//sylee130518-1 pin search bdl
            //son230224 //PinSearchPin[nTmp1]=1;//sylee130518-1 pin search bdl
            //sylee150308  fscanf(fp, "\n");                
            
            nT1Net=nBDLData1[1][i][5];//SYLEE120902
            nNetList1[nT1Net]++;//SYLEE120902
            Sum1=Sum1+nData2[i][0];//##5
            Sum2=Sum2+nData2[i][4];
        }

    }
    else
    {//if( LibEnable==2){//sylee151202

        //-------------------
        //son BDL Data Read 
        //-------------------

        for(i=1; i<=nBDLHead1[1][3][1]; i++)        //son nBDLHead1[3][1] : 총핀수
        {   

            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                fscanf(fp, "%d", &nBDLData1[1][i][j]); 
                //3,4,5 error 추가할것...?
            }


            //son230224 PinSearchPin[]의 index는 hwPin이므로 jigPin index로 1로 초기화하는 부분은 삭제해야 함.
            //son230224 //nTmp1=nBDLData1[1][i][1];//sylee130518-1 pin search bdl     //son nTmp1 <=핀번호
            //son230224 //PinSearchPin[nTmp1]=1;//sylee130518-1 pin search bdl        //son PinSearchPin[핀번호] 위치에 1을 표시
            
            fscanf(fp, "\n");
            nT1Net=nBDLData1[1][i][5];//SYLEE120902                     //son nT1Net <= Net핀수
            nNetList1[nT1Net]++;//SYLEE120902
        }
    }//if( LibEnable==2){//sylee151202  end



    //son BDL 파일 close
    fclose(fp);  

    //son BDL 파일 총 핀수 확인
    if(nBDLHead1[1][3][1]>0){
        nTotalPin=nBDLHead1[1][3][1];//sylee130522
    }
    else{
        nTotalPin=0;//sylee130522
    }

    if( nLibEnable==2){//sylee151202

        if( nTotal1>0){//sylee150309
            Namo1=(int)fmod(Sum1,10000.);

            nTotal2=nData2[nTotal1+1][0];
            if( Sum1>0){
                if( Namo1!=nTotal2){
                    str.Format("  Error No 1301,   Pin Check Sum Error!   \n\n  ??? (%d != %d) (Set Sum ,Load Sum)  \n  \n      ",  nTotal2,Namo1); 
                    MyTrace(PRT_BASIC, str);        //son211022
                    AfxMessageBox(str, MB_OK);
                    nAutoStartInterlock100=2;//sylee050308  //sylee150602
                }
            }
            Namo1=(int)fmod(Sum2,10000.);

            nTotal2=nData2[nTotal1+1][4];
            if( Sum2>0){
                if( Namo1!=nTotal2){
                    str.Format("  Error No 1301,   Pin Group Count  Sum Error!   \n\n  ??? (%d != %d) (Set Sum ,Load Sum)  \n  \n      ",  nTotal2,Namo1); 
                    MyTrace(PRT_BASIC, str);        //son211022
                    AfxMessageBox(str, MB_OK);
                    nAutoStartInterlock100=2;//sylee050308  //sylee150602
                }
            }




        }
    }//if( LibEnable==2)//sylee151202  end

    //------------------------
    //son Z:NETLIST.TXT 생성
    //------------------------
    
   	NetWorkConnection1();//sylee231010-1   //son240705 mereged from ACE1000
 
    
    //son 현재 BDL을 Net 갯수별로 그루핑한 List를 출력한다.  
    //  ex: 싱글Net 몇개, 2pin Net 몇개, ... 10pin Net 몇개 ...

    FILE *fp2; 
    char  fName2[200]; 

    ::ZeroMemory(&fName2, sizeof(fName2));

    strcat( fName2 , "Z:\\NETLIST.TXT" );


    fp2 = fopen(fName2,"wt");
    if(fp2 == NULL){
        str.Format("Error No 1915  :  Network Driver Check!. ( Z DRIVER)     Network Driver disconnection.                     NetList Make Error,               Check  Netlist file! ");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }
    else{

        fprintf(fp2, "<<NetList>>\n");
        //  str.Format("Error No 1008  : ..\\BDL\\%s  Open File Reading  !.", dlg.GetFileName());
        CTime curTime = CTime::GetCurrentTime();
        //fprintf(fp, "2011 \n" );
        fprintf(fp2, "*>Date:%d/%d/%d, %d:%d:%d\n",
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        int Temp1,Sum1,Sum2;
        Sum1=Sum2=0;
        for( i=1;i<=Def_SwitchMaxPin; i++){//SYLEE120902
            if(nNetList1[i]>0){
                Temp1=nNetList1[i]=nNetList1[i]/i;
                if(Temp1>0){//sylee171108-1
                    fprintf(fp2, "Length[%5d],Count=%5d\n",i, Temp1);
                }
            }
            Sum1=Sum1+nNetList1[i];
        }
        Sum2=nNetList1[1];

        fprintf(fp2, "----------------------\n");
        fprintf(fp2, "* Single Count,=%5d\n", Sum2);
        fprintf(fp2, "* Net    Count,=%5d\n", Sum1-Sum2);
        fprintf(fp2, "----------------------\n");
        fprintf(fp2, "* Total  Count,=%5d\n", Sum1);

        fprintf(fp2,"* %s\n", fName );
        fprintf(fp2, "----------------------\n");


        fclose(fp2); 


    }//sylee131121



    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //화면 display

    m_list1.ResetContent();

    str.Format("          *NO.,        *PIN,      *NET NO,    *NET COUNT  " );
    m_list1.InsertString(0,str);

    for(i=1; i<=nBDLHead1[1][3][1]; i++){   

        ::ZeroMemory(&szTemp1, sizeof(szTemp1));

        if( nBDLData1[1][i][1] < 0  ||   nBDLData1[1][i][1]>=DEF_MAX_PIN1 ){
            str.Format("Error No 1011  :   BDL FILE.  Error line=%d.   (PIN NO < 0  ||   PIN NO>= %d  ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i ,DEF_MAX_PIN1, fName);
            m_list1.AddString(str);
        }
        if( nBDLData1[1][i][2] < 0  ||   nBDLData1[1][i][2]>=DEF_MAX_PIN1 ){
            str.Format("Error No 1012  :   BDL FILE.  Error line=%d.   (NET LIST < 0  ||  NET LIST>= %d   ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i,DEF_MAX_PIN1 , fName);
            m_list1.AddString(str);
        }

        if( nBDLData1[1][i][5] < 1){
            str.Format("Error No 1013  :   BDL FILE.  Error line=%d.   NET LIST COUNT <1.     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i , fName);
            m_list1.AddString(str);
        }

        // str.Format("  NO=%7d,     %10d,     %10d,      %10d,  ", i,  nBDLData1[1][i][1],nBDLData1[1][i][2],nBDLData1[1][i][5] );
        str.Format("  NO=%d,     %d,     %d,      %d,  ", i,  nBDLData1[1][i][1]+1,nBDLData1[1][i][2],nBDLData1[1][i][5] );
        //      m_list1.InsertString(0,str);   /REVERSE   DISPLAY
        m_list1.AddString(str);
    }


    //son BDL Data NetCount 오류 점검       
    if (Check_BDLNetCountError(1) < 0)       //son210831
        return;


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //OPEN    DATA 


    int nFirstPin,netPinCnt, netPinIdx  ;

    int  nTmp11,nTmp12,nTmpCo1,nTmpPin1;//SYLEE130222JOYTECH
    int nDebug[500];//debug
    int nBdlStart,nBdlEnd;

    ::ZeroMemory(&nDebug, sizeof(nDebug));//debug

    nTmpCo1=0;//SYLEE130222JOYTECH

    nFirstPin=0; 
    netPinIdx=0; 

    //--------------------------------------------------------------------------
    //son "\SETUP2\PieceRange.ini"를 struct CSysInfo01에 Load(type=2)
    FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD); //SYLEE130312JOYTECH    
                                                    //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
    //son 1. PieceRange.ini의 정보로 g_nPinPieceNo[] 정보 생성
    //    g_nPinPieceNo[] : 각 핀의 piece 번호 정보를 저장.  index는 pinNo, value는 piece
    ReadPinPieceNo(1);//SYLEE130222JOYTECH

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //sort//sylee130224joytech[
    
    //---------------------------------------------
    //son piece 1부터 nBDLData1을 재배치한다.
    int nSortNo1[DEF_MAX_PIN1][8];//16k   //sylee150604   9000->33000  //sylee150917-2  16400  33000
    ::ZeroMemory(&nSortNo1, sizeof(nSortNo1));


    //son g_nPinPieceNo[]에서 BDL 핀의 piece 정보를 획득하여 nBDLData1[i][BDL_PIECE] 에 저장.
    for(i=1; i<=nBDLHead1[1][3][1]; i++){
        nTmp11=nBDLData1[1][i][1];
        nTmp11=g_nPinPieceNo[1][nTmp11+1];
        nBDLData1[1][i][BDL_PIECE]=nTmp11;//piece set   //son221102 7: BDL_PIECE
    }

    //son 보통 "BDL폴더\Setup\PieceRange.ini" 를 확인해 보면 piece 1부터 배치돼 있으므로
    //    Sort한 결과 nSortNo1[netPinIdx]가 원본 nBDLData[pinIdx]과 보통 똑같다.
    netPinIdx=0; 
    int pinIdx, pieceIdx;
    for(pinIdx=1; pinIdx<=nBDLHead1[1][3][1]; pinIdx++)     //son  총핀수 만큼 반복
    {
        nBdlStart=pinIdx;
        nBdlEnd=nBdlStart+nBDLData1[1][pinIdx][5]-1;        //son 해당 net의 총핀수 -1
        if(nBdlStart>nBdlEnd){//sylee131105
            // str.Format(" **> Error!   BDL FILE,   TOTAL POINT CHECK !!! ");
            // m_list1.InsertString(0,str);
            str.Format("  ERROR 1111,  BDL NET LIST LOADING ERROR.      BDL FILE, TOTAL POINT  CHECK! \n");      //son220317
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            break;//sylee131128 continue;//errr?
        }
        pinIdx=nBdlEnd;

        //son ReadPinPieceNo1()에서 g_nPieceMax1 <= SysInfoPieceRange.m_nPieceCount 할당.
        //son  pieceIdx(pieceNum)
        for(pieceIdx=1; pieceIdx<=g_nPieceMax1; pieceIdx++)
        {
            //son                           pin net         cnt     piece#
            //son                        0   1   2   3   4   5   6   7 
            // ex)  k2=1, nBDLData1[1] = 0, 264, 1,  0,  1,  2,  1,  1
            // ex)  k2=2, nBDLData1[2] = 0, 393, 1,  0,  1,  2,  1,  1
            // ex)  k2=3, nBDLData1[3] = 0, 272, 1,  0,  1,  2,  1,  1
            // ex)  k2=4, nBDLData1[4] = 0, 401, 1,  0,  1,  2,  1,  1
            // ex)  k2=5, nBDLData1[5] = 0, 276, 2,  0,  1,  2,  1,  1
            // ex)  k2=6, nBDLData1[6] = 0, 405, 2,  0,  1,  2,  1,  1

            for(int k2=nBdlStart; k2<=nBdlEnd; k2++)
            {
                //son piece와 현재 BDL 핀의 pieceNum이 같다면
                if(pieceIdx==nBDLData1[1][k2][7])
                {
                    netPinIdx++;
                    for(int m2=1; m2<=7; m2++)
                    {
                        nSortNo1[netPinIdx][m2]=nBDLData1[1][k2][m2];
                        //son                      pin net         cnt     piece#
                        //son                   0  1    2   3   4   5   6   7
                        //son ex) nSortNo1[1] = 0, 264, 1,  0,  1,  2,  1,  1
                        //son ex) nSortNo1[2] = 0, 393, 1,  0,  1,  2,  1,  1
                        //son ex) nSortNo1[3] = 0, 272, 1,  0,  1,  2,  1,  1
                        //son ex) nSortNo1[4] = 0, 401, 1,  0,  1,  2,  1,  1
                        //son ex) nSortNo1[5] = 0, 276, 2,  0,  1,  2,  1,  1
                        //son ex) nSortNo1[6] = 0, 405, 2,  0,  1,  2,  1,  1
                    }
                }
            }
        }
    }


    if( g_nPieceMax1 < 1){//sylee130228
        str.Format("  OnDataLoadAuto()    \n  g_nPieceMax1(=%d) <0  \n\n ?  ",g_nPieceMax1);    //son220317
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    //son  piece 1부터 순서대로 정렬된 Sort한 결과를 nSortNo1[pinIdx]를 nBDLData1[pinIdx]에 덮어쓴다.
    for( pinIdx=1; pinIdx<=nBDLHead1[1][3][1]; pinIdx++){
        for(int m2=1; m2<=7; m2++){
            nBDLData1[1][pinIdx][m2]=nSortNo1[pinIdx][m2];
        }
    }
    ////////////////////////////////////////////////////////////////
    
    


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    netPinIdx=0; 
    int openPinCnt=0;//OPEN 
    int shortPinCnt=0;//short
    for(i=1; i<=nBDLHead1[1][3][1]; i++)
    {

        //son  net pin 수가 1이면 One point net이다.
        if( nBDLData1[1][i][BDL_NET_PINCNT]==1){  //short  if one point
            shortPinCnt++;
            nBDLData3[1][shortPinCnt][BDL_PIN0]      =nBDLData1[1][i][BDL_PIN0];                            //son 대표핀 pinNo
            nBDLData3[1][shortPinCnt][BDL_NET_PINCNT]=nBDLData1[1][i][BDL_NET_PINCNT];//sylee150909-1 2N1+  //son netPinCount
            nBDLData3[1][shortPinCnt][BDL_PIECE]     =nBDLData1[1][i][BDL_PIECE];                          //son pinPiece
            netPinIdx=0;
            continue;
        }

        //son netPinCnt가 1보다도 작다면 해당 라인은 의미 없음          
        if( nBDLData1[1][i][BDL_NET_PINCNT]<1){             
            netPinIdx=0;
            continue;
        }

        netPinIdx++;

        //-------------------------
        //son ShortTest List 생성
        //-------------------------

        //son Net의 첫번째 pin이라면 short 관련 nBDLData3[1][shortPinCnt]을 채운다.
        if( netPinIdx==1)
        {       
            //  if( nBDLData1[1][i][BDL_NET_PINCNT]>1){
            nFirstPin=nBDLData1[1][i][BDL_PIN0];        //son 현재 Net의 대표핀
            netPinCnt=nBDLData1[1][i][BDL_NET_PINCNT];  //son 현재 Net의 PinCount

            if(nFirstPin==9){//debug
                nFirstPin=9;
            }

            shortPinCnt++;//short 
            nBDLData3[1][shortPinCnt][BDL_PIN0]      = nBDLData1[1][i][BDL_PIN0];//short                    //son 대표핀 pinNo 
            nBDLData3[1][shortPinCnt][BDL_NET_PINCNT]= nBDLData1[1][i][BDL_NET_PINCNT];//sylee150909-1 2N1+ //son netPinCount
            nBDLData3[1][shortPinCnt][BDL_PIECE]     = nBDLData1[1][i][BDL_PIECE];                          //son piece

            //////////////////////////////////////////////////////////////
            //                  ::ZeroMemory(&nBdlPieceSel1, sizeof(nBdlPieceSel1));//SYLEE130222JOYTECH
            if(netPinCnt>1){        
                nTmp11=g_nPinPieceNo[1][nFirstPin+1]; 
            }
            //////////////////////////////////////////////////////////////

            //  }
        }

        //son count가 Net의 PinCount 갯수 이내라면
        else if(netPinIdx<=netPinCnt)
        {               

            //////////////////////////////////////////////////////////////
            nTmpPin1=nBDLData1[1][i][BDL_PIN0];     //son pinNo
            nTmp12=g_nPinPieceNo[1][nTmpPin1+1];    //son 현재pin의 pieceNo

            //son 대표핀의 pieceNo와 현재pin의 pieceNo가 다르다면 예외케이스처리
            if(nTmp11!=nTmp12){
                netPinIdx =0;//sylee130224
                i=i-1;          //son pinIdx를 원복하고 for 초기로 돌아감
                continue;
            } 



            //-------------------------
            //son OpenTest List 생성
            //------------------------- 

            openPinCnt++;

            //son 같은 Net의 데이터 정보를 모아준다. 
            nBDLData2[1][openPinCnt][1]=nFirstPin;
            nBDLData2[1][openPinCnt][2]=nBDLData1[1][i][1];
            nBDLData2[1][openPinCnt][3]=nBDLData1[1][i][5];  //PIECE OPEN
            nBDLData2[1][openPinCnt][4]=nBDLData1[1][i][6];  //PIECE

            //son 해당 net의 마지막핀이라면
            if(netPinIdx==netPinCnt){
                netPinIdx =0;
            }

        }

    }


    if(i>0){
        if( (i-1)==nBDLHead1[1][3][1] ){
            nBDLData2[1][0][1]=openPinCnt;  // OPEN BDL 총 갯수  //SYLEE20111212
        }
    }

    //son200420-4WNET10000 shortPinCnt 설정 위치를 현재 위치로 이동 
    //son221103 if (shortPinCnt > 1) //short        
    if (shortPinCnt >= 1)  { //short        //son221103 shortPinCnt 1 일때 nBDLData3[1][0][1] 에 write안 되는 문제 수정.
        nBDLData3[1][0][1]=shortPinCnt;   // short  BDL 총 갯수  //SYLEE20111212
    }
    


    //-------------------------------------------------------------------------------------------
    //son 1. PieceRange.ini의 정보로 g_nPinPieceNo[] 정보 생성, 
    ReadPinPieceNo(1);//SYLEE121202

    //son  2. piece 정보를 이용해서 nBDLData3[nDUT]를 piece별로 g_pvBDLData31[][][]에 재배치한다. 
    //       InsertShortTest()  실제 수행
    makePieceShortTestData(1);       //son221103_2

    //son piece를 1개일 때의 통검사 ShortTest Data를 NetPinCount 기준으로 정렬한다.
    Sort_nBDLData3_byNetPinCount(1); //son200722-BDL-Sort

    //son piece별 ShortTest Data를 NetPinCount 기준으로 정렬한다.
    Sort_nBDLData31_byNetPinCount(1);  //son200722-BDL-Sort


    //--------------------------------
    //son Piece별 OpenTest List 생성
    makePieceOpenTestData(1);       //son for DUT1  //son200420-4WNET10000



    //------------------------------------------------------------------
    //son Debug Print: Piece별 OpenTest List, ShortTest List 가 제대로 생성되었는지 확인 

    //son 생성한 Dut, piece별 COpenTestItem g_pvBDLData2Op[dut][piece]를  "D:\\log\\0OpenPinListPiece_DUT%d" 에 출력한다. (확인용)
    int nIPOpenTotal = 0;
    nIPOpenTotal = printPieceOpenTestData(1);   //son200420-4WNET10000

    //son 생성한 Dut, piece별  CShortTestItem g_pvBDLData31[dut][piece]를 "D:\\log\\0ShortPinListPiece_DUT%d" 에 출력한다. (for debug)
    printPieceShortTestData(1);   //son200420-4WNET10000

	LeakMultPinList1(1);    //sylee230526
    //son 생성한 piece별 open nBDLData2Op[][][]를 원래의 nBDLData2[][][]의 카운트와 비교해서 
    //    제대로 생성이 되었는지 확인한다.
    if (nBDLData2[1][0][1] != nIPOpenTotal){
        str.Format(" ERROR NO 4001,\n BDL OPEN LIST LINK ERROR.  BDL RELOADIG!  ,\n CHECK BDL FILE or PIECE RANGERANGE SET  or FILE !   \n\n  DON'T  RUN \n\n  list1=%d,list2=%d \n\n ", nBDLData2[1][0][1],nIPOpenTotal);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    //------------------------------------------------------------------
    //son m_list에 로딩한 BDL 리스트와 Open List의 정보를 출력한다.
    str.Format("          ");
    m_list1.AddString(str);
    str.Format(" ***>BDL LIST  =%d    Reading Complete", nBDLHead1[1][3][1] );
    m_list1.AddString(str);

    nBDLData1[1][0][2]=nBDLHead1[1][3][1];

    str.Format("          ");
    m_list1.AddString(str);
    str.Format(" ***>OPEN LIST  =%d    Reading Complete", nBDLData2[1][0][1] );
    m_list1.AddString(str);
    //  m_list1.InsertString(0,str);
    str.Format("          ");
    m_list1.AddString(str);


#if 0   //son221103_3  printPieceOpenTestData() 와 내용 중복되는 파일 출력이므로 삭제.
    //--------------------------------
    // Save OpenTest Data File 
    //--------------------------------

    str.Format("OPENSETUPLIST"); 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        //  str.Format("Error No 1001  : D:\\AUTO\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
        str.Format("Error No 1014 :   .\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return  ;
    }

    CTime curTime = CTime::GetCurrentTime();

    //fprintf(fp, "2011 \n" );
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
            curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : OPEN LIST  FILE \n");  
    fprintf(fp, " \n"); 
    fprintf(fp," ***>OPEN LIST Count =%d    Reading Complete", nBDLData2[1][0][1] );
    fprintf(fp, " \n");
    fprintf(fp, " \n"); 
    fprintf(fp,"  PIN, PIN, \n"  );

    for(i=1; i<=nBDLData2[1][0][1]; i++){   
        fprintf(fp, "    %d,   %d\n",nBDLData2[1][i][1]+1, nBDLData2[1][i][2]+1);  //set path file   //sylee150527
    }

    fclose(fp);  
#endif

#if 0   //son221103_3  printPieceShortTestData() 와 내용 중복되는 파일 출력이므로 삭제.
    //--------------------------------
    // Save ShortTest Data File 
    //--------------------------------
    str.Format("SHORTSETUPLIST"); 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format("Error No 1015  : .\\BDL\\ SHORTSETUPLIST   file Data Saving Error !     file close!.");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return  ;
    }

    curTime = CTime::GetCurrentTime();

    //fprintf(fp, "2011 \n" );
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
                                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  
                                curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : SHORT LIST  FILE \n");  
    fprintf(fp, " \n"); 
    fprintf(fp," ***>SHORT LIST Count =%d    Read Complete", nBDLData3[1][0][1] );
    fprintf(fp, " \n");
    fprintf(fp, " \n"); 
    fprintf(fp,"      PIN,   NetPinCount,   Piece \n"  );

    for(i=1; i<=nBDLData3[1][0][1]; i++){   
        fprintf(fp, "    %5d,        %4d,    %2d\n", nBDLData3[1][i][BDL_PIN0]+1,  //set path file  ////sylee150527
                                                  nBDLData3[1][i][BDL_NET_PINCNT],
                                                  nBDLData3[1][i][BDL_PIECE]);
    }
    fclose(fp);  
#endif

    str.Format(" ***>SHORT LIST  =%d    Reading Complete", nBDLData3[1][0][1] );
    m_list1.AddString(str);

    str.Format("     ");
    m_list1.AddString(str);


    //--------------------
    //son PinMap 로딩
    //--------------------

    //son "C:\\ACE500\\SETUP2\\SysSetPin.ini"를 struct CSysSet12에 Load(type=2)
    int a;//sylee130518
    a=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
    if(a!=1){
        //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
        str.Format(" Error No 1112, \n    %s\\SETUP2\\SysSetPin.ini  Load ERROR !  \n    file Read Error !!!", SysInfoView01.m_pStrFilePath1);    //son210514
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    //sylee130518
    ::ZeroMemory(PinSearchPin, sizeof(PinSearchPin)); //sylee130518-1 pin search bdl
    //      short nTmp1;//sylee130518-1 pin search bdl
    short wJigPin, wHwPin;  
    for(i=1; i<=nBDLHead1[1][3][1]; i++)
    {
        wJigPin=nBDLData1[1][i][1];//sylee130518-1 pin search bdl   //son BDL pin 번호(jigPin. 0부터시작)
        wHwPin=PinMap[2][wJigPin];

        //son-32KPIN-20200113
        // 20K인데 32K 영역의 BDL을 로딩한다면 없는 B,C, D 박스의 PinMap[2][pin]는 -1로 리턴됨. 
        // 이 경우는 계측기 SW 오동작할 수 있으므로 미리 경고해 주기 위해 nErrCount 추가함.
        // nErrCount가 1이상이면 ModeSet12.make32kHwPin()에서  -1을 리턴해서 오류가 있음을 표시해 준다.
        
        // wHwPin값이 마이너스일 때, PinSearchPin[wHwPin] 접근하는 것을 방지한다.
        if (wHwPin < 0)
        {
            str.Format(" Error No 1113, \n Can't find hwPin for userPin_%d! \n\n Wrong BDL File loaded which does not match LowerStartPin(%s_%d), Hw MaxPin(%s)!\n",  
                        wJigPin+1, STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                        SysSet12.m_nLowerJigStartPin, STR_MAX_PIN(SysSet12.m_nPinTotMax) );
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);    

            //son BDL 파일 에러 표시
            gFlag_Start_Int_bdlError = 1;
            return;
        }
        PinSearchPin[wHwPin]=1;//sylee130518-1 pin search bdl
    }


    //son "\SETUP2\Recipe.ini"를 struct CSysInfo05로 Load(type=2)
    FileSysInfo01.LoadSaveSub15(_LOAD);//SYLEE140110 

    FileSysInfo01.LoadSaveSet16(_LOAD);//sylee130325-//estimation
    FileSysInfo01.LoadSaveSet19(_LOAD);//sylee150407//estimation

    nFlagEstModelMakeCount=0;//sylee130325

    //sylee170411   if(SysSet13.m_nSet19==1){//sylee150407

    if(SysInfo05.m_nProSpMode==2||   SysInfo05.m_nProSpMode==3 ||  SysInfo05.m_nProSpMode==4 ){ //sylee150417-1 
        if(SysSet16.m_nSet11==1){  //estimation
            nFlagEstModelMake=1;//sylee130325
        }
        else{
            nFlagEstModelMake=0;//sylee130325
        }
    }
    //sylee170411   }




    nAutoStartInterlock100=0;//sylee050308   //sylee150602    //##6
    if(   nBDLHead1[1][3][1] !=  (nBDLData3[1][0][1]+nBDLData2[1][0][1]) ){ //sylee050308     //sylee150602 
        nAutoStartInterlock100=2;//sylee050308

        str.Format(" Error!  BDL file,  UDL FILE CHECK!   ");//sylee050308
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str);

        str.Format(" Error!  Don't RUN!   MANAGER CALL!,  Engineer Call!    BDL LOADING ERROR .   TOTAL PIN=%d != ( open pin=%d + short pin=%d)????    ", 
                                  nBDLHead1[1][3][1], nBDLData2[1][0][1], nBDLData3[1][0][1] );
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str); //sylee050308

        //son BDL 파일 에러 표시
        gFlag_Start_Int_bdlError = 1;   //son221103_4
    }

    //son221104 begin: shortPinCount 1개 일때 Recipe에 short Test 있으면 warning 출력 
    if (nBDLData3[1][0][1] <= 1 && FileSysInfo01.checkRecipeStep_ShortExist() == true)  
    {
        str.Format("\n Warning No 1036, Short Test Pin Count is only \'1\'.\n\n ShortTest, LeakTest included in Recipe cannot be performed\n");  
        MyTrace(PRT_BASIC, str);     
        AfxMessageBox(str, MB_OK);          

    } //son221104 end

}
 

 


//===================================================================================================


//SYLEE180807-1 DUT16 DEL.
/*
void CChildView1::OnDataLoad2()  //open button// sylee20111204
{       


//  return;
    
//  CFileDialog dlg(TRUE);
//  char buffer[65535];

    FILE *fp; 
    CFile file;

    int  i,j,k ;

    char fName[200]; 
    char szTemp1[100];

    CString str;  
    CString strVal1; 
 
    
    ::ZeroMemory(&nBDLHead1D, sizeof(nBDLHead1D));
    ::ZeroMemory(&nBDLData1D, sizeof(nBDLData1D));
    ::ZeroMemory(&nBDLData2D, sizeof(nBDLData2D));//sylee20120403
    ::ZeroMemory(&nBDLData3D, sizeof(nBDLData3D));//sylee20120403
    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&szTemp1, sizeof(szTemp1));



//  strcat( fName , SysInfoView01.m_pStrFilePath1 ); 
//    CFileDialog dlg(TRUE,"D:\\AUTO\\BDL","*.BDL");
//    CFileDialog dlg(TRUE,"D:\\AUTO\\BDL","*.*");
     CFileDialog dlg(TRUE,".\\BDL","*.BDL");

    if(dlg.DoModal()==IDOK)
    {   
        //str.Format("D:\\AUTO\\BDL\\%s", dlg.GetFileName()); 
         str.Format("%s", dlg.GetFileName()); 

         //DATALOAD :
         strcat( fName , str );
         fp=fopen(fName,"rt");
         if(fp == NULL){ 
            str.Format("Error No 1008-10  : .\\BDL\\%s  Open File Reading  !.", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);          
            return ;
         }

            ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig2D, sizeof(SysInfoView01.m_pStrFilePathJig2D));
            ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1D, sizeof(SysInfoView01.m_pStrFilePathJig1D));

            strcat( SysInfoView01.m_pStrFilePathJig2D, fName );  //SYLEE20111212
            strcat( SysInfoView01.m_pStrFilePathJig1D, ".\\BDL");
            strcat( SysInfoView01.m_pStrFilePathJig1D, SysInfoView01.m_pStrFilePathJig2D);
            str.Format("%s", SysInfoView01.m_pStrFilePathJig1D);
            m_Label2.SetCaption(str);

            fscanf(fp, "%d\n", &nBDLHead1[2][1][1]);
            fscanf(fp, "%d"  , &nBDLHead1[2][2][1]);
            fscanf(fp, "%d"  , &nBDLHead1[2][2][2]);
            fscanf(fp, "%d"  , &nBDLHead1[2][2][3]);            
            fscanf(fp, "%d\n", &nBDLHead1[2][3][1]);//list sum
 

        if(nBDLHead1[2][1][1]!=20000){
            str.Format("Error No 1009  : BDL 2   FILE ?   HEADER  DATA 20000?   Reading ERROR  !.\n  ..\\BDL\\%s", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);
            return  ;
        }


        if(nBDLHead1[2][3][1]<1){
            nBDLHead1[2][3][1]=0;
            str.Format("Error No 1116  :   BDL 2  FILE.  HEADER Info.:  Pin Totle list No. data zero.  Reading ERROR  !. \n D:\\AUTO\\BDL\\%s", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);
            return  ;
        }

 
        for(i=1; i<=nBDLHead1[2][3][1]; i++){
            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                fscanf(fp, "%d", &nBDLData1[2][i][j]); 
                //3,4,5 error 추가할것...?
            }
            fscanf(fp, "\n");
        }
    }
    else{
        return;
    }
        fclose(fp);  

//////////////////////////////////////////////////////////////////////////////////////////////////////
//화면 display

//          m_list2.ResetContent();
//  str.Format(" * ERROR : VB  Input 16bit=%d  > 65535 ",a); 
//  m_list1.InsertString(0,str);
        str.Format("          *NO.,        *PIN,      *NET NO,    *NET COUNT  " );
//      m_list2.InsertString(0,str);

        for(i=1; i<=nBDLHead1[2][3][1]; i++){   
            
            ::ZeroMemory(&szTemp1, sizeof(szTemp1));
            
             if( nBDLData1[2][i][1] < 0  ||   nBDLData1[2][i][1]>=DEF_MAX_PIN1 ){
                     str.Format("Error No 1011  :   BDL FILE.  Error line=%d.   (PIN NO < 0  ||   PIN NO>= %d  ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i ,DEF_MAX_PIN1, dlg.GetFileName());
//                   m_list2.AddString(str);
             }
             if( nBDLData1[2][i][2] < 0  ||   nBDLData1[2][i][2]>=DEF_MAX_PIN1 ){
                     str.Format("Error No 1012  :   BDL FILE.  Error line=%d.   (NET LIST < 0  ||  NET LIST>= %d   ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i,DEF_MAX_PIN1 , dlg.GetFileName());
//                   m_list2.AddString(str);
             }

             if( nBDLData1[2][i][5] < 1){
                     str.Format("Error No 1013  :   BDL FILE.  Error line=%d.   NET LIST COUNT <1.     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i , dlg.GetFileName());
//                   m_list2.AddString(str);
             }

                str.Format("  NO=%7d,     %10d,     %10d,      %10d,  ", i,  nBDLData1[2][i][1],nBDLData1[2][i][2],nBDLData1[2][i][5] );
        //      m_list1.InsertString(0,str);   /REVERSE   DISPLAY
//              m_list2.AddString(str);
        }


//////////////////////////////////////////////////////////////////////////////////////////////////////
//OPEN    DATA 
 
        
        int nFirstPin,netPinCnt, netPinIdx  ;
        
        nFirstPin=0; 
        netPinIdx=0; 
        j=0;//OPEN 
        k=0;//short


        for(i=1; i<=nBDLHead1[2][3][1]; i++){

                if( nBDLData1[2][i][5]==1){  //short  if one point
                    k++;
                    nBDLData3[2][k][1]=nBDLData1[2][i][1];
                    netPinIdx=0;
                    continue;
                }

                if( nBDLData1[2][i][5]<1){              
                    netPinIdx=0;
                    continue;
                }

                netPinIdx++;

                if( netPinIdx==1){
                //  if( nBDLData1[1][i][5]>1){
                    nFirstPin=nBDLData1[2][i][1];
                    netPinCnt=nBDLData1[2][i][5];

                    k++;//short 
                    nBDLData3[2][k][1]=nBDLData1[2][i][1];//short 

                //  }
                }
                else if(netPinIdx<=netPinCnt){
                    j++;
                    nBDLData2[2][j][1]=nFirstPin;
                    nBDLData2[2][j][2]=nBDLData1[2][i][1];
                    nBDLData2[2][j][3]=nBDLData1[2][i][5];  //PIECE OPEN
                    nBDLData2[2][j][4]=nBDLData1[2][i][6];  //PIECE
                    if(netPinIdx==netPinCnt){
                        netPinIdx =0;
                    }
                }


        }


        if(i>0){
                if(  (i-1)==nBDLHead1[2][3][1] ){
                    nBDLData2[2][0][1]=j;  // OPEN BDL 총 갯수  //SYLEE20111212
                }
        }




    //  if( i!=nBDLHead1[1][3][1] ){
    //      str.Format("Error No 1101,\n\n BDL READ?, SET? ERROR \n\n\n BDL FILE Check!\n\n i=%d,   !=,  nBDLHead1[1][3][1]=%d,", i,nBDLHead1[1][3][1]); 
    //      AfxMessageBox(str);

    //  }


        if( k>1){   //short        
            nBDLData3[2][0][1]=k;   // short  BDL 총 갯수  //SYLEE20111212
        }


             str.Format("          ");
//           m_list2.AddString(str);
             str.Format(" ***>BDL LIST2  =%d    Reading Complete", nBDLHead1[2][3][1] );
//           m_list2.AddString(str);


             str.Format("          ");
//           m_list2.AddString(str);
             str.Format(" ***>OPEN LIST2  =%d    Reading Complete", nBDLData2[2][0][1] );
//           m_list2.AddString(str);
        //  m_list1.InsertString(0,str);

             str.Format("          ");
//           m_list2.AddString(str);
        //  m_list1.InsertString(0,str);
//////////////////////////////////////////////////////////////////////////////////////////////////////
//FILE OPEN FILE SAVE  DATE

//????????????????????????????????????
        str.Format("OPENSETUPLIST2"); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            //  str.Format("Error No 1001  : D:\\AUTO\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
            str.Format("Error No 1014 : .\\BDL\\ OPENSETUPLIST2   file Data Saving Error !     file close!.");
            AfxMessageBox(str, MB_OK);          
            return  ;
        }

        CTime curTime = CTime::GetCurrentTime();
     
       //fprintf(fp, "2011 \n" );
        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : OPEN LIST2  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>OPEN LIST2 Count =%d    Reading Complete", nBDLData2[2][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, PIN, \n"  );

        for(i=1; i<=nBDLData2[2][0][1]; i++){   
             fprintf(fp, "    %d,   %d\n",nBDLData2[2][i][1], nBDLData2[2][i][2]);  //set path file
        }
 
        fclose(fp);  


//////////////////////////////////////////////////////////////////////////////////////////////////////
//SHORT   DATA 


        str.Format("SHORTSETUPLIST2"); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format("Error No 1015  : .\\BDL\\ SHORTSETUPLIST2   file Data Saving Error !     file close!.");
            AfxMessageBox(str, MB_OK);          
            return  ;
        }

         curTime = CTime::GetCurrentTime();
     
       //fprintf(fp, "2011 \n" );
        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : SHORT LIST2  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>SHORT  LIST2 Count =%d    Reading Complete", nBDLData3[2][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, \n"  );

        for(i=1; i<=nBDLData3[2][0][1]; i++){   
             fprintf(fp, "    %d,   \n",nBDLData3[2][i][1]);  //set path file
        }
 
        fclose(fp);  


            str.Format(" ***>SHORT  LIST2  =%d    Reading Complete", nBDLData3[2][0][1] );
//           m_list2.AddString(str);
            //m_list1.InsertString(0,str);

            str.Format("     ");
//          m_list2.AddString(str);
        //  m_list1.InsertString(0,str);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//FILE SHORT FILE SAVE  DATE

//????????????????????????????????????

 
 //////////////////////////////////////////////////////////////////////////////////////////////////////
 
}


*/



//SYLEE180807-1 DUT16 DEL.
/*

void CChildView1::OnDataLoad3()  //open button// sylee20111204
{       


//  return;
    
//  CFileDialog dlg(TRUE);
//  char buffer[65535];

    FILE *fp; 
    CFile file;

    int  i,j,k ;

    char fName[200]; 
    char szTemp1[100];

    CString str;  
    CString strVal1; 
 
    
    ::ZeroMemory(&nBDLHead1E, sizeof(nBDLHead1E));
    ::ZeroMemory(&nBDLData1E, sizeof(nBDLData1E));
    ::ZeroMemory(&nBDLData2E, sizeof(nBDLData2E));//sylee20120403
    ::ZeroMemory(&nBDLData3E, sizeof(nBDLData3E));//sylee20120403
    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&szTemp1, sizeof(szTemp1));



//  strcat( fName , SysInfoView01.m_pStrFilePath1 ); 
//    CFileDialog dlg(TRUE,"D:\\AUTO\\BDL","*.BDL");
//    CFileDialog dlg(TRUE,"D:\\AUTO\\BDL","*.*");
     CFileDialog dlg(TRUE,".\\BDL","*.BDL");

    if(dlg.DoModal()==IDOK)
    {   
        //str.Format("D:\\AUTO\\BDL\\%s", dlg.GetFileName()); 
         str.Format("%s", dlg.GetFileName()); 

         //DATALOAD :
         strcat( fName , str );
         fp=fopen(fName,"rt");
         if(fp == NULL){ 
            str.Format("Error No 1008-11  : .\\BDL\\%s  Open File Reading  !.", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);          
            return ;
         }

            ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig2E, sizeof(SysInfoView01.m_pStrFilePathJig2E));
            ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1E, sizeof(SysInfoView01.m_pStrFilePathJig1E));

            strcat( SysInfoView01.m_pStrFilePathJig2E, fName );  //SYLEE20111212
            strcat( SysInfoView01.m_pStrFilePathJig1E, ".\\BDL");
            strcat( SysInfoView01.m_pStrFilePathJig1E, SysInfoView01.m_pStrFilePathJig2E);
            str.Format("%s", SysInfoView01.m_pStrFilePathJig1E);
            m_Label3.SetCaption(str);

            fscanf(fp, "%d\n", &nBDLHead1[3][1][1]);
            fscanf(fp, "%d"  , &nBDLHead1[3][2][1]);
            fscanf(fp, "%d"  , &nBDLHead1[3][2][2]);
            fscanf(fp, "%d"  , &nBDLHead1[3][2][3]);            
            fscanf(fp, "%d\n", &nBDLHead1[3][3][1]);//list sum
 

        if(nBDLHead1[3][1][1]!=20000){
            str.Format("Error No 1009  : BDL 3   FILE ?   HEADER  DATA 20000?   Reading ERROR  !.\n  ..\\BDL\\%s", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);
            return  ;
        }


        if(nBDLHead1[3][3][1]<1){
            nBDLHead1[3][3][1]=0;
            str.Format("Error No 1117  :   BDL 3  FILE.  HEADER Info.:  Pin Totle list No. data zero.  Reading ERROR  !. \n D:\\AUTO\\BDL\\%s", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);
            return  ;
        }

                 
    //  fscanf(fp, "%s\n", SysInfoView01.m_pStrFilePath1); 
    //  fscanf(fp, "%s\n", SysInfoView01.m_pStrFilePathJig1);
    //  fscanf(fp, "%s\n", SysInfoView01.m_pStrFilePathJig2);  //sw loading init folder

        for(i=1; i<=nBDLHead1[3][3][1]; i++){
            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                fscanf(fp, "%d", &nBDLData1[3][i][j]); 
                //3,4,5 error 추가할것...?
            }
            fscanf(fp, "\n");
        }
    }
    else{
        return;
    }
        fclose(fp);  

//////////////////////////////////////////////////////////////////////////////////////////////////////
//화면 display

//          m_list3.ResetContent();
//  str.Format(" * ERROR : VB  Input 16bit=%d  > 65535 ",a); 
//  m_list1.InsertString(0,str);
        str.Format("          *NO.,        *PIN,      *NET NO,    *NET COUNT  " );
//      m_list3.InsertString(0,str);

        for(i=1; i<=nBDLHead1[3][3][1]; i++){   
            
            ::ZeroMemory(&szTemp1, sizeof(szTemp1));
            
             if( nBDLData1[3][i][1] < 0  ||   nBDLData1[3][i][1]>=DEF_MAX_PIN1 ){
                     str.Format("Error No 1011  :   BDL FILE.  Error line=%d.   (PIN NO < 0  ||   PIN NO>= %d  ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i ,DEF_MAX_PIN1, dlg.GetFileName());
//                   m_list3.AddString(str);
             }
             if( nBDLData1[3][i][2] < 0  ||   nBDLData1[3][i][2]>=DEF_MAX_PIN1 ){
                     str.Format("Error No 1012  :   BDL FILE.  Error line=%d.   (NET LIST < 0  ||  NET LIST>= %d   ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i ,DEF_MAX_PIN1, dlg.GetFileName());
//                   m_list3.AddString(str);
             }

             if( nBDLData1[3][i][5] < 1){
                     str.Format("Error No 1013  :   BDL FILE.  Error line=%d.   NET LIST COUNT <1.     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i , dlg.GetFileName());
//                   m_list3.AddString(str);
             }

                    str.Format("  NO=%7d,     %10d,     %10d,      %10d,  ", i,  nBDLData1[3][i][1]+1,nBDLData1[3][i][2],nBDLData1[3][i][5] );
        //          m_list1.InsertString(0,str);   /REVERSE   DISPLAY
//                  m_list3.AddString(str);
        }


//////////////////////////////////////////////////////////////////////////////////////////////////////
//OPEN    DATA 
 
        
        int nFirstPin,netPinCnt, netPinIdx  ;
        
        nFirstPin=0; 
        netPinIdx=0; 
        j=0;//OPEN 
        k=0;//short


        for(i=1; i<=nBDLHead1[3][3][1]; i++){

                if( nBDLData1[3][i][5]==1){  //short  if one point
                    k++;
                    nBDLData3[3][k][1]=nBDLData1[3][i][1];
                    netPinIdx=0;
                    continue;
                }

                if( nBDLData1[3][i][5]<1){              
                    netPinIdx=0;
                    continue;
                }

                netPinIdx++;

                if( netPinIdx==1){
                //  if( nBDLData1[1][i][5]>1){
                    nFirstPin=nBDLData1[3][i][1];
                    netPinCnt=nBDLData1[3][i][5];

                    k++;//short 
                    nBDLData3[3][k][1]=nBDLData1[3][i][1];//short 

                //  }
                }else if(netPinIdx<=netPinCnt){
                    j++;
                    nBDLData2[3][j][1]=nFirstPin;
                    nBDLData2[3][j][2]=nBDLData1[3][i][1];
                    nBDLData2[3][j][3]=nBDLData1[3][i][5];  //PIECE OPEN
                    nBDLData2[3][j][4]=nBDLData1[3][i][6];  //PIECE
                    if(netPinIdx==netPinCnt){
                        netPinIdx =0;
                    }
                }


        }


        if(i>0){
                if(  (i-1)==nBDLHead1[3][3][1] ){
                    nBDLData2[3][0][1]=j;  // OPEN BDL 총 갯수  //SYLEE20111212
                }
        }




    //  if( i!=nBDLHead1[1][3][1] ){
    //      str.Format("Error No 1101,\n\n BDL READ?, SET? ERROR \n\n\n BDL FILE Check!\n\n i=%d,   !=,  nBDLHead1[1][3][1]=%d,", i,nBDLHead1[1][3][1]); 
    //      AfxMessageBox(str);

    //  }


        if( k>1){   //short        
            nBDLData3[3][0][1]=k;   // short  BDL 총 갯수  //SYLEE20111212
        }


             str.Format("          ");
//           m_list3.AddString(str);
             str.Format(" ***>BDL LIST3  =%d    Reading Complete", nBDLHead1[3][3][1] );
//           m_list3.AddString(str);


             str.Format("          ");
//           m_list3.AddString(str);
             str.Format(" ***>OPEN LIST3  =%d    Reading Complete", nBDLData2[3][0][1] );
//           m_list3.AddString(str);
        //  m_list1.InsertString(0,str);

             str.Format("          ");
//           m_list3.AddString(str);
        //  m_list1.InsertString(0,str);
//////////////////////////////////////////////////////////////////////////////////////////////////////
//FILE OPEN FILE SAVE  DATE

//????????????????????????????????????
        str.Format("OPENSETUPLIST3"); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            //  str.Format("Error No 1001  : D:\\AUTO\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
            str.Format("Error No 7014 : .\\BDL\\ OPENSETUPLIST3   file Data Saving Error !     file close!.");
            AfxMessageBox(str, MB_OK);          
            return  ;
        }

        CTime curTime = CTime::GetCurrentTime();
     
       //fprintf(fp, "2011 \n" );
        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : OPEN LIST2  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>OPEN LIST3 Count =%d    Reading Complete", nBDLData2[3][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, PIN, \n"  );

        for(i=1; i<=nBDLData2[3][0][1]; i++){   
             fprintf(fp, "    %d,   %d\n",nBDLData2[3][i][1], nBDLData2[3][i][2]);  //set path file
        }
 
        fclose(fp);  


//////////////////////////////////////////////////////////////////////////////////////////////////////
//SHORT   DATA 


        str.Format("SHORTSETUPLIST3"); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format("Error No 1015  : .\\BDL\\ SHORTSETUPLIST2   file Data Saving Error !     file close!.");
            AfxMessageBox(str, MB_OK);          
            return  ;
        }

         curTime = CTime::GetCurrentTime();
     
       //fprintf(fp, "2011 \n" );
        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : SHORT LIST2  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>SHORT  LIST3 Count =%d    Reading Complete", nBDLData3[3][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, \n"  );

        for(i=1; i<=nBDLData3[3][0][1]; i++){   
             fprintf(fp, "    %d,   \n",nBDLData3[3][i][1]);  //set path file
        }
 
        fclose(fp);  


            str.Format(" ***>SHORT  LIST3  =%d    Reading Complete", nBDLData3[3][0][1] );
//           m_list3.AddString(str);
            //m_list1.InsertString(0,str);

            str.Format("     ");
//           m_list3.AddString(str);
        //  m_list1.InsertString(0,str);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//FILE SHORT FILE SAVE  DATE

//????????????????????????????????????

 
 //////////////////////////////////////////////////////////////////////////////////////////////////////
 
}



*/  



//SYLEE180807-1 DUT16 DEL.
/*

void CChildView1::OnDataLoad4()  //open button// sylee20120813
{       


//  return;
    
//  CFileDialog dlg(TRUE);
//  char buffer[65535];

    FILE *fp; 
    CFile file;

    int  i,j,k ;

    char fName[200]; 
    char szTemp1[100];

    CString str;  
    CString strVal1; 
 
    
    ::ZeroMemory(&nBDLHead1F, sizeof(nBDLHead1F));
    ::ZeroMemory(&nBDLData1F, sizeof(nBDLData1F));
    ::ZeroMemory(&nBDLData2F, sizeof(nBDLData2F));//sylee20120403
    ::ZeroMemory(&nBDLData3F, sizeof(nBDLData3F));//sylee20120403
    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&szTemp1, sizeof(szTemp1));



//  strcat( fName , SysInfoView01.m_pStrFilePath1 ); 
//    CFileDialog dlg(TRUE,"D:\\AUTO\\BDL","*.BDL");
//    CFileDialog dlg(TRUE,"D:\\AUTO\\BDL","*.*");
     CFileDialog dlg(TRUE,".\\BDL","*.BDL");

    if(dlg.DoModal()==IDOK)
    {   
        //str.Format("D:\\AUTO\\BDL\\%s", dlg.GetFileName()); 
         str.Format("%s", dlg.GetFileName()); 

         //DATALOAD :
         strcat( fName , str );
         fp=fopen(fName,"rt");
         if(fp == NULL){ 
            str.Format("Error No 1108  : .\\BDL\\%s  Open File Reading  !.", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);          
            return ;
         }

            ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig2F, sizeof(SysInfoView01.m_pStrFilePathJig2F));
            ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1F, sizeof(SysInfoView01.m_pStrFilePathJig1F));

            strcat( SysInfoView01.m_pStrFilePathJig2F, fName );  //SYLEE20111212
            strcat( SysInfoView01.m_pStrFilePathJig1F, ".\\BDL");
            strcat( SysInfoView01.m_pStrFilePathJig1F, SysInfoView01.m_pStrFilePathJig2F);
            str.Format("%s", SysInfoView01.m_pStrFilePathJig1F);
            m_Label4.SetCaption(str);

            fscanf(fp, "%d\n", &nBDLHead1[4][1][1]);
            fscanf(fp, "%d"  , &nBDLHead1[4][2][1]);
            fscanf(fp, "%d"  , &nBDLHead1[4][2][2]);
            fscanf(fp, "%d"  , &nBDLHead1[4][2][3]);            
            fscanf(fp, "%d\n", &nBDLHead1[4][3][1]);//list sum
 

        if(nBDLHead1[4][1][1]!=20000){
            str.Format("Error No 1010  : BDL 4   FILE ?   HEADER  DATA 20000?   Reading ERROR  !.\n  ..\\BDL\\%s", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);
            return  ;
        }


        if(nBDLHead1[4][3][1]<1){
            nBDLHead1[4][3][1]=0;
            str.Format("Error No 1118  :   BDL 4  FILE.  HEADER Info.:  Pin Totle list No. data zero.  Reading ERROR  !. \n D:\\AUTO\\BDL\\%s", dlg.GetFileName());
            AfxMessageBox(str, MB_OK);
            return  ;
        }

                 
    //  fscanf(fp, "%s\n", SysInfoView01.m_pStrFilePath1); 
    //  fscanf(fp, "%s\n", SysInfoView01.m_pStrFilePathJig1);
    //  fscanf(fp, "%s\n", SysInfoView01.m_pStrFilePathJig2);  //sw loading init folder

        for(i=1; i<=nBDLHead1[4][3][1]; i++){
            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                fscanf(fp, "%d", &nBDLData1[4][i][j]); 
                //3,4,5 error 추가할것...?
            }
            fscanf(fp, "\n");
        }
    }else{
        return;
    }
        fclose(fp);  

//////////////////////////////////////////////////////////////////////////////////////////////////////
//화면 display

//          m_list4.ResetContent();
//  str.Format(" * ERROR : VB  Input 16bit=%d  > 65535 ",a); 
//  m_list1.InsertString(0,str);
        str.Format("          *NO.,        *PIN,      *NET NO,    *NET COUNT  " );
//      m_list4.InsertString(0,str);

        for(i=1; i<=nBDLHead1[4][3][1]; i++){   
            
            ::ZeroMemory(&szTemp1, sizeof(szTemp1));
            
             if( nBDLData1[4][i][1] < 0  ||   nBDLData1[4][i][1]>=DEF_MAX_PIN1 ){
                     str.Format("Error No 1011  :   BDL FILE.  Error line=%d.   (PIN NO < 0  ||   PIN NO>= %d  ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i ,DEF_MAX_PIN1, dlg.GetFileName());
//                   m_list4.AddString(str);
             }
             if( nBDLData1[4][i][2] < 0  ||   nBDLData1[4][i][2]>=DEF_MAX_PIN1 ){
                     str.Format("Error No 1012  :   BDL FILE.  Error line=%d.   (NET LIST < 0  ||  NET LIST>= %d   ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i ,DEF_MAX_PIN1, dlg.GetFileName());
//                   m_list4.AddString(str);
             }

             if( nBDLData1[4][i][5] < 1){
                     str.Format("Error No 1013  :   BDL FILE.  Error line=%d.   NET LIST COUNT <1.     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",i , dlg.GetFileName());
//                   m_list4.AddString(str);
             }

                    str.Format("  NO=%7d,     %10d,     %10d,      %10d,  ", i,  nBDLData1[4][i][1]+1,nBDLData1[4][i][2],nBDLData1[4][i][5] );
        //      m_list1.InsertString(0,str);   /REVERSE   DISPLAY
//                  m_list4.AddString(str);
        }


//////////////////////////////////////////////////////////////////////////////////////////////////////
//OPEN    DATA 
 
        
        int nFirstPin,netPinCnt, netPinIdx  ;
        
        nFirstPin=0; 
        netPinIdx=0; 
        j=0;//OPEN 
        k=0;//short


        for(i=1; i<=nBDLHead1[4][3][1]; i++){

                if( nBDLData1[4][i][5]==1){  //short  if one point
                    k++;
                    nBDLData3[4][k][1]=nBDLData1[4][i][1];
                    netPinIdx=0;
                    continue;
                }

                if( nBDLData1[4][i][5]<1){              
                    netPinIdx=0;
                    continue;
                }

                netPinIdx++;

                if( netPinIdx==1){
                //  if( nBDLData1[1][i][5]>1){
                    nFirstPin=nBDLData1[4][i][1];
                    netPinCnt=nBDLData1[4][i][5];

                    k++;//short 
                    nBDLData3[4][k][1]=nBDLData1[4][i][1];//short 

                //  }
                }else if(netPinIdx<=netPinCnt){
                    j++;
                    nBDLData2[4][j][1]=nFirstPin;
                    nBDLData2[4][j][2]=nBDLData1[4][i][1];
                    nBDLData2[4][j][3]=nBDLData1[4][i][5];  //PIECE OPEN
                    nBDLData2[4][j][4]=nBDLData1[4][i][6];  //PIECE
                    if(netPinIdx==netPinCnt){
                        netPinIdx =0;
                    }
                }


        }


        if(i>0){
                if(  (i-1)==nBDLHead1[4][3][1] ){
                    nBDLData2[4][0][1]=j;  // OPEN BDL 총 갯수  //SYLEE20111212
                }
        }




    //  if( i!=nBDLHead1[1][3][1] ){
    //      str.Format("Error No 1101,\n\n BDL READ?, SET? ERROR \n\n\n BDL FILE Check!\n\n i=%d,   !=,  nBDLHead1[1][3][1]=%d,", i,nBDLHead1[1][3][1]); 
    //      AfxMessageBox(str);

    //  }


        if( k>1){   //short        
            nBDLData3[4][0][1]=k;   // short  BDL 총 갯수  //SYLEE20111212
        }


             str.Format("          ");
//           m_list4.AddString(str);
             str.Format(" ***>BDL LIST3  =%d    Reading Complete", nBDLHead1[4][3][1] );
//           m_list4.AddString(str);


             str.Format("          ");
//           m_list4.AddString(str);
             str.Format(" ***>OPEN LIST3  =%d    Reading Complete", nBDLData2[4][0][1] );
//           m_list4.AddString(str);
        //  m_list1.InsertString(0,str);

             str.Format("          ");
//           m_list4.AddString(str);
        //  m_list1.InsertString(0,str);
//////////////////////////////////////////////////////////////////////////////////////////////////////
//FILE OPEN FILE SAVE  DATE

//????????????????????????????????????
        str.Format("OPENSETUPLIST4"); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            //  str.Format("Error No 1001  : D:\\AUTO\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
            str.Format("Error No 7014 : .\\BDL\\ OPENSETUPLIST4   file Data Saving Error !     file close!.");
            AfxMessageBox(str, MB_OK);          
            return  ;
        }

        CTime curTime = CTime::GetCurrentTime();
     
       //fprintf(fp, "2011 \n" );
        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : OPEN LIST4  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>OPEN LIST4 Count =%d    Reading Complete", nBDLData2[4][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, PIN, \n"  );

        for(i=1; i<=nBDLData2[4][0][1]; i++){   
             fprintf(fp, "    %d,   %d\n",nBDLData2[4][i][1], nBDLData2[4][i][2]);  //set path file
        }
 
        fclose(fp);  


//////////////////////////////////////////////////////////////////////////////////////////////////////
//SHORT   DATA 


        str.Format("SHORTSETUPLIST4"); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format("Error No 1015  : .\\BDL\\ SHORTSETUPLIST4   file Data Saving Error !     file close!.");
            AfxMessageBox(str, MB_OK);          
            return  ;
        }

         curTime = CTime::GetCurrentTime();
     
       //fprintf(fp, "2011 \n" );
        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : SHORT LIST2  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>SHORT  LIST4 Count =%d    Reading Complete", nBDLData3[4][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, \n"  );

        for(i=1; i<=nBDLData3[4][0][1]; i++){   
             fprintf(fp, "    %d,   \n",nBDLData3[4][i][1]);  //set path file
        }
 
        fclose(fp);  


            str.Format(" ***>SHORT  LIST3  =%d    Reading Complete", nBDLData3[4][0][1] );
//           m_list4.AddString(str);
            //m_list1.InsertString(0,str);

            str.Format("     ");
//           m_list4.AddString(str);
        //  m_list1.InsertString(0,str);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//FILE SHORT FILE SAVE  DATE

//????????????????????????????????????

 
 
}


*/


void CChildView1::OnDataSave() //save button 
{

    LPCTSTR lpszPath ;
    TCHAR szTemp[200];
    char    szTemp1[10] ;       
    CString str; 
    

    ::GetCurrentDirectory(MAX_PATH, szTemp );
    lpszPath = szTemp;

    
    ::ZeroMemory(&szTSetup2Name, sizeof(szTSetup2Name));
    ::ZeroMemory(&szTemp1, sizeof(szTemp1));
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1, sizeof(SysInfoView01.m_pStrFilePathJig1));
 
    strcat( szTemp1 , "\\" );
//SYLEE121202   strcat( SysInfoView01.m_pStrFilePathJig1, szTemp );
//SYLEE121202   strcat( SysInfoView01.m_pStrFilePathJig1, szTemp1 );


    //son ACE400을 현재 수행한 위치를 확인한다. => 이 시점에 "C:\BDL\10VGAG011J" 처럼 BDL Path로 바뀐다.
    ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFileDri1);///sylee1201022

    strcat( szTSetup2Name, SysInfoView01.m_pStrFilePathJig1 );//sylee1201022



  //  strcat( SysInfoView01.m_pStrFilePathJig1,dlg.GetFileName());
    strcat( SysInfoView01.m_pStrFilePathJig1, SysInfoView01.m_pStrFilePathJig2);
 
 
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);
    m_LabelPath.SetCaption(str);

    FileSysInfo01.LoadSaveView01(1);        //son save



    if(nFlagFileLoadok != -1)//sylee161116-2
    {
        //sylee170717  if(SysInfoView01.m_pStrFilePathJig2[0] == NULL ){ //sylee140403
        if(SysInfoView01.m_pStrFilePathJig2[0] == NULL  || gFlag_Start_Int_bdlError==1 ){ //sylee140403//sylee170717
            str.Format("  BDL SET  NG !  \n\n   %s\n", SysInfoView01.m_pStrFilePathJig1); 
            nFlagFileLoadok=0;//sylee160519
        }

        else{
            //son "BDL File Load OK"를 표시한다.
            str.Format("  BDL SET OK !  \n\n    %s\n", SysInfoView01.m_pStrFilePathJig1);
            nFlagFileLoadok=1;//sylee160519
        }
        //AfxMessageBox("  BDL File SAVE(SE) Completed!   %s \n");  //son220317
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
        
        
        memset(g_fSparkRefData, 0, sizeof(g_fSparkRefData)); // hdpark231121
        memset(g_nSparkRefDataCount, 0, sizeof(g_nSparkRefDataCount)); // hdpark231121

        // hdpark231121 begin spark log 삭제
        char szPath1[MAX_PATH], szPath2[MAX_PATH];
        sprintf(szPath1, "%s", "D:\\LogSpark2");
        sprintf(szPath2, "%s", "D:\\LogSpark3");

        CleanUp_SparkLog(szPath1, 0, 1);    // 1 Month
        CleanUp_SparkLog(szPath2, 0, 3);    // 3 Month
        // hdpark231121 end
    }


}




//SYLEE180807-1 DUT16 DEL.
/*
void CChildView1::OnDataSave2() //save button 
{



//  return;

    LPCTSTR lpszPath ;
    TCHAR szTemp[200];
    char    szTemp1[10] ;       
    CString str; 
    

    ::GetCurrentDirectory(MAX_PATH, szTemp );
    lpszPath = szTemp;

    ::ZeroMemory(&szTemp1, sizeof(szTemp1));
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1D, sizeof(SysInfoView01.m_pStrFilePathJig1D));
 
    strcat( szTemp1 , "\\" );
    strcat( SysInfoView01.m_pStrFilePathJig1D, szTemp );
    strcat( SysInfoView01.m_pStrFilePathJig1D, szTemp1 );

  //  strcat( SysInfoView01.m_pStrFilePathJig1,dlg.GetFileName());
    strcat( SysInfoView01.m_pStrFilePathJig1D, SysInfoView01.m_pStrFilePathJig2D);
 
 
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1D);
    m_Label2.SetCaption(str);

    FileSysInfo01.LoadSaveView01D(1);

     str.Format("   SET OK !  \n  \n    %s ", SysInfoView01.m_pStrFilePathJig1D);
    //AfxMessageBox("  BDL File SAVE(SE) Completed!   %s  ", MB_OK);
     AfxMessageBox(str, MB_OK);


}

*/




//SYLEE180807-1 DUT16 DEL.
/*

void CChildView1::OnDataSave3() //save button //sylee20120809
{



    LPCTSTR lpszPath ;
    TCHAR szTemp[200];
    char    szTemp1[10] ;       
    CString str; 
    

    ::GetCurrentDirectory(MAX_PATH, szTemp );
    lpszPath = szTemp;

    ::ZeroMemory(&szTemp1, sizeof(szTemp1));
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1E, sizeof(SysInfoView01.m_pStrFilePathJig1E));
 
    strcat( szTemp1 , "\\" );
    strcat( SysInfoView01.m_pStrFilePathJig1E, szTemp );
    strcat( SysInfoView01.m_pStrFilePathJig1E, szTemp1 );

  //  strcat( SysInfoView01.m_pStrFilePathJig1,dlg.GetFileName());
    strcat( SysInfoView01.m_pStrFilePathJig1E, SysInfoView01.m_pStrFilePathJig2E);
 
 
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1E);
    m_Label3.SetCaption(str);

    FileSysInfo01.LoadSaveView01E(1);

     str.Format("   SET OK !  \n  \n    %s ", SysInfoView01.m_pStrFilePathJig1E);
    //AfxMessageBox("  BDL File SAVE(SE) Completed!   %s  ", MB_OK);
     AfxMessageBox(str, MB_OK);


}


*/


/*  //SYLEE180807-1 DUT16 DEL.

void CChildView1::OnDataSave4() //save button //sylee20120809
{



    LPCTSTR lpszPath ;
    TCHAR szTemp[200];
    char    szTemp1[10] ;       
    CString str; 
    

    ::GetCurrentDirectory(MAX_PATH, szTemp );
    lpszPath = szTemp;

    ::ZeroMemory(&szTemp1, sizeof(szTemp1));
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1F, sizeof(SysInfoView01.m_pStrFilePathJig1F));
 
    strcat( szTemp1 , "\\" );
    strcat( SysInfoView01.m_pStrFilePathJig1F, szTemp );
    strcat( SysInfoView01.m_pStrFilePathJig1F, szTemp1 );

  //  strcat( SysInfoView01.m_pStrFilePathJig1,dlg.GetFileName());
    strcat( SysInfoView01.m_pStrFilePathJig1F, SysInfoView01.m_pStrFilePathJig2F);
 
 
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1F);
    m_Label4.SetCaption(str);

    FileSysInfo01.LoadSaveView01F(1);

     str.Format("   SET OK !  \n  \n    %s ", SysInfoView01.m_pStrFilePathJig1F);
    //AfxMessageBox("  BDL File SAVE(SE) Completed!   %s  ", MB_OK);
     AfxMessageBox(str, MB_OK);


}



*/



void CChildView1::OnDataSaveAs() //  save as button
{   
    CString buffer;
    CString str,str1,strVal1;
    FILE *fp;
    LPCTSTR lpszPath ;
    TCHAR szTemp[200];
    char    szTemp1[10] ;
    int nT1[5][20],i;


    ::ZeroMemory (&nT1, sizeof(nT1));

    CFileDialog dlg(FALSE);

    if(dlg.DoModal() == IDOK)
    {
        CFile file;

        str.Format("%s", dlg.GetFileName());

    //******************************************************************************************************************
    // bdl data  reading
//  fp=fopen(str,"rt");
    fp=fopen(str,"rt");
    if(fp==NULL){ 
    //  fclose(fp); 
        str1.Format("%s    BDL File Reading Error No 1016 , BDL File SaveAs ", dlg.GetFileName());
    //  AfxMessageBox("  BDL File Reading Error No 1002.    Please BDL FILE  SET SAVE! ", MB_OK);
        MyTrace(PRT_BASIC, str1);        //son211022
        AfxMessageBox(str1, MB_OK);
        return  ;
    }
   
    //BDL BIOPTRO FORMAT CHECK
    fscanf(fp, "%d\n", &nT1[0][0]);
 
    if( nT1[0][0] != 1995  ||   nT1[0][0] != 20000){ 
        fclose(fp); 
        str1.Format("  BDL File Reading Error  No 1003.    BiOptro format ? !.  BDL File SaveAs.\n");   //son220317
        MyTrace(PRT_BASIC, str1);        //son211022
        AfxMessageBox(str1, MB_OK);
        return ;    
    }

    if( nT1[0][0] == 1995 ){///UDL   //UDL Converting

        // reading 
           //BDL HEADER READING
            for(i=0; i<10; i++){   
                fscanf(fp, "%d", &nT1[1][i]);
             
            }
            //fscanf(fp, "%s\n", strVal1);
            fscanf(fp, " \n"   );
            for(i=0; i<10; i++){   
                fscanf(fp, "%d", &nT1[2][i]);
            }
            fclose(fp);
   //saveing

            fp=fopen(str,"wt");
            if(fp==NULL){ 
            //  fclose(fp); 
                str1.Format("Error No 1017.   %s    BDL File Reading  , BDL File SaveAs.\n", dlg.GetFileName());    //son220317
                MyTrace(PRT_BASIC, str1);        //son211022
                AfxMessageBox(str1, MB_OK);
                return  ;
            }
            fprintf(fp, "20000\n");    

            //*2줄:  1) 전체 Pin 총 수,   2) 마지막 Pin No,  3) Net 총수,  4) 상부 Pin 총수,  5) 하부 Pin 총    
            fprintf(fp, "%d  %d  %d 0 0\n", nT1[2][7], nT1[1][5], nT1[2][1]);    

    }

        fclose(fp);

//==========================================================================================================
   
    //  file.Open(dlg.GetFileName(), CFile::modeCreate | CFile::modeWrite);
        file.Open(str, CFile::modeCreate | CFile::modeWrite);

        GetDlgItemText(IDC_DATA, buffer);
        file.Write((LPCSTR)buffer, buffer.GetLength());
        file.Close();

    //  SetDlgItemText(IDC_DATA, "로드된 자료 없음");
    }

    
    ::GetCurrentDirectory(MAX_PATH, szTemp );
    lpszPath = szTemp;

    ::ZeroMemory(&szTemp1, sizeof(szTemp1));
    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1, sizeof(SysInfoView01.m_pStrFilePathJig1));
 
    strcat( szTemp1 , "\\" );
    strcat( SysInfoView01.m_pStrFilePathJig1, szTemp );
    strcat( SysInfoView01.m_pStrFilePathJig1, szTemp1 );
    strcat( SysInfoView01.m_pStrFilePathJig1,dlg.GetFileName());

 
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);
    m_LabelPath.SetCaption(str);

    FileSysInfo01.LoadSaveView01(1);

}



void CChildView1::ChangeView(CFormView* pDlg)
{
    if(m_pCurMainDlg != pDlg) {
        if(m_pCurMainDlg->GetSafeHwnd())
            m_pCurMainDlg->ShowWindow(SW_HIDE);
        if(pDlg->GetSafeHwnd())
            pDlg->ShowWindow(SW_SHOW);
        m_pCurMainDlg = pDlg;
    }
}

 

void CChildView1::OnButton1001() //open
{
    OnDataLoad();
    OnButton1002(); //save  //sylee130306
     
}


//extern int fileCopy(const char* src, const char* dst);   //sylee210621 
extern int nFlag_PassOnNo;
extern int nFlag_PassOn; 

//son210906 영풍 베트남 전용기능  "Make same multi DUT BDL" 버튼 
// m_edit101에 입력된 개수만큼 기존 BDL과 4W.ini 파일, PieceRange.ini을 동일 폴더내에 카피해 준다.  
// DUT1개인 BDL을 여러개 DUT BDL로 변경하기 위한 기능임.
// 이 파일을 수행하고 나면 .BDL과 4w.ini, Setup2\PieceRange.ini 가 DUT 개수만큼 늘어난다.   
void CChildView1::OnButton201()  //sylee210621
{

#ifdef __YP_VIET_CUSTOMER
    char szText[100],destfile[100], srcfile[100] ;
    int dutCount, dut;
    CString str;

    m_edit101.GetWindowText( szText,10);
    dutCount=atoi( szText );
    if(dutCount<=1)
    {
        //son 2개 이상의 DUT 개수를 입력해야 BDL을 카피해서 DUT 개수를 늘릴 수 있음.
        str.Format("First, enter the number of DUTs to make and click the button.  (dutCount=%d <=1)  \n", dutCount);    //son220317 //son230829
        errMessageBox(1001, str);   //son230829
        return ;        //son220317
    }
    if(dutCount> MAX_DUT)      //son220228 16 -> MAX_DUT
    {
        str.Format(" The number of DUTs exceeded the Max number. (dutCount=%d >16) \n", dutCount);  //son220317 //son230829
        errMessageBox(1001, str);   //son230829
        return ;        //son220317
    }
    //son230829 210906 시점에 15로 코딩이 되었으나 CPassWord1 Dlg 쪽에서 15에 대한 코딩이 안 되어서 11로 
    //          동작이 되고 있었음. 기존 사이트 호환을 위해 명시적으로 11로 바꾸고 CPassWord1 Dlg 쪽도 보완함. 
	//son230829 nFlag_PassOnNo=15;  //son C:\ACE400\Data\Message_L.ini의 103 번 항목의 passwd를 체크한다. 
    nFlag_PassOnNo=11;  //son C:\ACE400\Data\Message_L.ini의 101 번 항목의 passwd를 체크한다.  //son230829  


    CPassWord1 Dlg; 
    Dlg.DoModal();      
    if(  nFlag_PassOn!=2 ){ //son 틀린 패스워드면 리턴.
        nFlag_PassOn=0;
        AfxMessageBox("         PASSWORD  NG !!!         \n     ");
        return ;
    }   

    nFlag_PassOn=0;     
    AfxMessageBox("         PASSWORD  OK!!!   \n\n    FILE MAKING !!!   \n\n     ");

    /////////////////////////////////////////////////////////////
    //FILE SEARCH : xxx.BDL
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s\\%s", SysInfoView01.m_pStrFilePathBDL, (SysInfoView01).m_pStrFileBDLName); 
    strcat( srcfile , str); 
    for(   dut=2; dut<=dutCount; dut++)
    {
        ::ZeroMemory(&destfile, sizeof(destfile));
        str.Format("%s\\%s", SysInfoView01.m_pStrFilePathBDL, (SysInfoView01).m_pStrFileBDLName);  
        strcat(destfile,str);
        str.Format("%d",dut);
        strcat(destfile,str); 
        if ( (_access(destfile , 0 )) != -1 ) {      
            str.Format("%s file exist.   -> Delete file! and Retry! ", destfile);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str);             
            return;
        }
    }
    /////////////////////////////////////////////////////////////
    // File Search : 4W.ini
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL); 
    strcat( srcfile , str); 
    strcat( srcfile , "\\4W.INI" ); 
    for(   dut=2; dut<=dutCount; dut++){
        ::ZeroMemory(&destfile, sizeof(destfile));
        str.Format("%s", SysInfoView01.m_pStrFilePathBDL);  
        strcat(destfile,str);
        str.Format("\\4W.INI%d",dut);
        strcat(destfile,str);
        if ( (_access(destfile , 0 )) != -1 ) {      
            str.Format("%s file exist.   -> Delete file! and Retry! ", destfile);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str);             
            return;
        }
    }
    ///////////////////////////////////////////////////////////// 
    // File Search : PieceRange.ini
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s\\SETUP2\\PieceRange.ini", SysInfoView01.m_pStrFilePathBDL); 
    strcat( srcfile , str); 

    for(   dut=2; dut<=dutCount; dut++){
        ::ZeroMemory(&destfile, sizeof(destfile)); 
        str.Format("%s\\SETUP2\\PieceRange%d.ini", SysInfoView01.m_pStrFilePathBDL, dut); 
        strcat(destfile,str); 
        if ( (_access(destfile , 0 )) != -1 ) {      
            str.Format("%s file exist.   -> Delete file! and Retry! ", destfile);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str);             
            return;
        }
    }

    /////////////////////////////////////////////////////////////
    // CopyFile : XXX.BDL
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s\\%s", SysInfoView01.m_pStrFilePathBDL,(SysInfoView01).m_pStrFileBDLName); 
    strcat( srcfile , str); 
    for(   dut=2; dut<=dutCount; dut++){
        ::ZeroMemory(&destfile, sizeof(destfile));
        str.Format("%s\\%s", SysInfoView01.m_pStrFilePathBDL,(SysInfoView01).m_pStrFileBDLName);  
        strcat(destfile,str);
        str.Format("%d",dut);
        strcat(destfile,str);
        CopyFile(srcfile, destfile, FALSE);
    }

    /////////////////////////////////////////////////////////////
    // CopyFile : 4W.ini
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL); 
    strcat( srcfile , str); 
    strcat( srcfile , "\\4W.INI" ); 
    for(   dut=2; dut<=dutCount; dut++){
        ::ZeroMemory(&destfile, sizeof(destfile));
        str.Format("%s", SysInfoView01.m_pStrFilePathBDL);  
        strcat(destfile,str);
        str.Format("\\4W.INI%d",dut);
        strcat(destfile,str);
        CopyFile(srcfile, destfile, FALSE);
    }
    ///////////////////////////////////////////////////////////// 
    // CopyFile : PieceRange.ini
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s\\SETUP2\\PieceRange.ini", SysInfoView01.m_pStrFilePathBDL); 
    strcat( srcfile , str); 

    for(   dut=2; dut<=dutCount; dut++){
        ::ZeroMemory(&destfile, sizeof(destfile)); 
        str.Format("%s\\SETUP2\\PieceRange%d.ini", SysInfoView01.m_pStrFilePathBDL,dut); 
        strcat(destfile,str); 
        CopyFile(srcfile, destfile, FALSE);
    }
    /////////////////////////////////////////////////////////////

#endif  //sylee210621
}



void CChildView1::OnButton3001() //netlist
{
   ::ShellExecute(NULL,"open","NOTEPAD.EXE","Z:\\NETLIST.TXT","NULL",SW_SHOWNORMAL);
}

void CChildView1::OnButton2001() //open
{ 
    OnDataLoadAuto(); 

#ifdef __OPVECTOR__
    //son piece마다 첫항목은 공백이므로 실제 Open 갯수, Short갯수보다 piece 갯수만큼 더 많이 나온다.
    // ex) CChildView1::OnDataLoadAuto():  g_nOpenTestCount=4090, g_nShortTestCount=1356
    //     piece는 2개이고, 실제 OpenList는   4088,  ShortList는   1354 임. 
    
    MyTrace(PRT_BASIC,"CChildView1::OnDataLoadAuto():  g_nOpenTestCount=%d, g_nShortTestCount=%d\n",
                        g_nOpenTestCount, g_nShortTestCount);
    __PrintMemSize(FUNC(CChildView1::OnButton2001), __LINE__);       //son200420-4WNET10000
#endif
}


void CChildView1::OnButton1002() //save  
{
    OnDataSave(); //save button  
    ReadFileB(1);  //jig read set
}

 


void CChildView1::On_Net_Convert1()  
{

    FILE *fp; 
    CFile file;

    CString str;  
    CString strVal1; 

    int  i,j;

    char fName[200]; 
    char szTemp1[100];

    //  int Temp1[32000][10];
    int Temp1[DEF_MAX_PIN1][10];
    int Temp2[DEF_MAX_PIN1][10];


    int nStart1,nEnd1;


    nStart1=20; 
    nEnd1=5889; 


    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&szTemp1, sizeof(szTemp1));
    ::ZeroMemory(&Temp1, sizeof(Temp1));
    ::ZeroMemory(&Temp2, sizeof(Temp2));



    CFileDialog dlg(TRUE,".\\BDL","*.BDL");

    if(dlg.DoModal()==IDOK)
    {   
        //str.Format("D:\\AUTO\\BDL\\%s", dlg.GetFileName()); 
        str.Format("%s", dlg.GetFileName()); 

        //DATALOAD :
        strcat( fName , str );
        fp=fopen(fName,"rt");

        if(fp == NULL){ 
            str.Format("Error No 1008-12  : .\\BDL\\%s  Open File Reading  !.", dlg.GetFileName());
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);          
            return  ;
        }



        //      for(i=1; i<20; i++){
        //          fscanf(fp, "\n");
        //      } 






        nStart1=1;
        nEnd1=5870;

        for(i=nStart1; i<=nEnd1; i++){
            for(j=1; j<=8; j++){   
                fscanf(fp, "%d", &Temp1[i][j]); 
            }
            fscanf(fp, "\n");
        }

    }
    else{
        return;
    }
    fclose(fp);  
    ////////////////////////////////
    /*
       1)순서  2) PIN NO   3) USE  4)X축 5)Y축  6)X축 7)Y축8)NET
       */

    int nCo1, oldT1;
    nCo1=0;
    oldT1=Temp1[1][8];

    for(i=1; i<=nEnd1 ; i++){

        if( oldT1==Temp1[i][8]){
            nCo1++; 
        }
        else{
            for(j=0; j<nCo1; j++){
                Temp1[i-j-1][9]=nCo1;
            } 

            nCo1=1;
            oldT1=Temp1[i][8];
        }

        if(i==nEnd1){
            for(j=0; j<nCo1; j++){
                Temp1[i-j-1][9]=nCo1;
            } 
            if(i==nEnd1){
                Temp1[i][9]=nCo1;
            }
        }

    }


    for(i=1; i<=nEnd1; i++){    
        Temp2[i][1]=Temp1[i][2];  
        Temp2[i][2]=Temp1[i][8];  
        Temp2[i][3]=0;  
        Temp2[i][4]=1;  
        Temp2[i][5]=Temp1[i][9];  
        Temp2[i][6]=1;
    } 

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //FILE OPEN FILE SAVE  DATE 
    str.Format("0Coverted.BDL"); 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        //  str.Format("Error No 1001  : D:\\AUTO\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
        str.Format("Error No 1014 : .\\BDL\\ 0Coverted   file Data Saving Error !     file close!.");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return  ;
    } 

    fprintf(fp,"20000\n" );
    fprintf(fp," %d   0    0\n", nEnd1  );
    fprintf(fp," %d\n", nEnd1  ); 

    for(i=1; i<=nEnd1; i++){    
        for(j=1; j<=6; j++){    
            fprintf(fp, " %d",Temp2[i][j]);   
        }
        fprintf(fp," \n"  ); 
    } 
    fclose(fp);


}
 



 

void CChildView1::OnButton111() 
{

    CString str;
    m_list1.ResetContent();
    str.Format("                        " ); 
    m_LabelPath.SetCaption(str);
}

void CChildView1::OnButton112() 
{    
    CString str;
//  m_list2.ResetContent();
    str.Format("                        " ); 
    m_Label2.SetCaption(str);   
}


void CChildView1::OnButton113() 
{
        CString str;
//    m_list3.ResetContent();
      str.Format("                        " ); 
      m_Label3.SetCaption(str);
}



void CChildView1::OnButton114() 
{
    CString str;
//  m_list4.ResetContent(); 
    str.Format("                        " ); 
    m_Label4.SetCaption(str);

}

//son200420-4WNET10000
BOOL CChildView1::DestroyWindow() 
{
    // TODO: Add your specialized code here and/or call the base class
    
#ifdef __OPVECTOR__
    //son g_pvBDLData2Op vector 전체를 초기화한다.
    DeleteAllOpenTestItem();        //son200420-4WNET10000

    //son g_pvBDLData31 vector 전체를 초기화한다.
    DeleteAllShortTestItem();       //son200420-4WNET10000
#endif

    MyTrace(PRT_BASIC, "\"ChildView1\" Object destroyed...\n" );
    return CFormView::DestroyWindow();
}


//son201028 : 메인장비요청에 의한  BDL 자동 로딩 기능 관련 추가
void CChildView1::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CFormView::OnShowWindow(bShow, nStatus);
    
    // TODO: Add your message handler code here
    //
    // VIEW가 SHOW 될때 
    if (bShow)
    {
        //son LotName Read.
        //son240622 char    strBarCode[MAX_BARCODE_STR+1];
        //son240622 g_pChildView7->ReadBarCode_fromHandlerDrive(strBarCode, g_strLotName);     //son220124
        g_pChildView7->ReadBarCode_TXT();  //son240622 "BarCode.txt" 정보 획득.

        if (SysSet13.m_nSet_UseLotName_4wLog == 1) //son220124
        {
            CString str; 
            str.Format("%s", g_strLotName);     //son220124 for 제이텍. 4wLog 폴더명에 Lot Name추가 관련. LotName 확인용
            m_LabelLotName.SetCaption(str);     //son220124 for 제이텍. 4wLog 폴더명에 Lot Name추가 관련. LotName 확인용
        }


#ifdef _PC_TEST_
        //son PC test용으로 2초 타이머로 설정.
        SetTimer(VIEW1_TIMER_DIO, 2000, NULL);  
#else
        SetTimer(VIEW1_TIMER_DIO, 100, NULL);   //son201028 100 msec Timer 설정
#endif
    }

    // VIEW가 HIDE 될때 
    else
    {
        //son201028: Auto 화면에 영향을 주지 않도록 설정했던 ComiDaq DIO timer를  kill한다.
        KillTimer(VIEW1_TIMER_DIO);
    }
    
}

 
//son201028 : 메인장비요청에 의한  BDL 자동 로딩 기능 관련 추가

void CChildView1::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default

    CString strTemp, str;

    //son ChildView1이 구동중일 때에만 아래 코드가 동작해야 한다. 
    if (g_nInitFlag1 != 1)      
        return; 
    
    switch(nIDEvent) 
    {   
        case VIEW1_TIMER_DIO: //son201028 case 170    DIO  100ms
        {
#ifdef _PC_TEST_
//TRACE("ChildView1: Recv VIEW1_TIMER_DIO\n");
            //son Debug 중 Timer가 너무 많이 쌓여서 오동작하는 일을 막기 위해  timer가 
            //    _PC_TEST_ 디버그시에는 더이상 들어오지 않게 일단 중지하고 이  timer 처리 완료 시점에 다시 시작한다.
            KillTimer(VIEW1_TIMER_DIO);

#endif
            //-------------------------------------------------
            //son 장비 PC와 통신하여 필요한 값을 받아 온다.
            //-------------------------------------------------
            int tempStatus = g_ComiDaq2.DiGetAll();
#ifdef __2BYTE_DIO__
            g_nCurDioCmd = tempStatus & 0x01ff;  //son231018 mask로 2byte 중 9 bit를 처리하도록 한다.  
#else
            g_nCurDioCmd = tempStatus & 0xff;  //son mask로 1byte 중 8 bit를 처리하도록 한다.  
#endif
            if (g_nCurDioCmd != g_nPrevDioCmd)
            {
                switch (g_nCurDioCmd)
                {
                    //son Input이 0이 들어오면 Output도 0으로 보내 준다.
                    case 0:
                        g_nPrevDioCmd = g_nCurDioCmd;
                        g_ComiDaq2.DoPutAll(0); 
                        break;

//son231018 begin            
                    //son231018 나노시스 요구사항. 원격 4W Ref 초기화 기능
                    case DIN_CMD_4W_REF_INIT: //son  0x100
                    {
                        MyTrace(PRT_BASIC, "ChildView1:                                   <======== DIN_4W_REF_INIT (0x%02x)\n", g_nCurDioCmd);     
#ifdef __NANOSYS__CUSTOMER
                        FileSysInfo01.Init_4wFile_Ref_AllDut();
#endif
                        g_ComiDaq2.DoPutAll(DOUT_CMD_ACK_4W_REF_INIT);
                        MyTrace(PRT_LEVEL1, "ChildView1_LINE_%d g_ComiDaq2.DoPutAll(): 0x%02x  =========> ACK_4W_REF_INIT \n",    //son231018
                                                                __LINE__, DOUT_CMD_ACK_4W_REF_INIT); 

                        Sleep(1000);     //son 1000 msec
                        g_ComiDaq2.DoPutAll(DOUT_CMD_ALL_OFF);//SYLEE130227JOYTECH  //son231016
                        MyTrace(PRT_LEVEL1, "ChildView1_Line_%d g_ComiDaq2.DoPutAll():     0   =========> ALL_OFF\n", __LINE__);

                        //g_nPrevDioCmd = g_nCurDioCmd;  //son231019 ACK를 보내고 나서 다음번 4W_REF_INIT 수신 못하는 현상 관련해서
                        g_nPrevDioCmd = 0;               //son231019 1초 기다린 다음 DOUT_CMD_ALL_OFF 보내고 g_nPrevDioCmd는 0으로 초기화하기로함

                        AfxMessageBox("The 4W Ref Low/High value of the current BDL has been reset. \n\nPlease set 4w Ref Low/High again in the 4W Setup window.");

                    }
                        break;
//son231018 end            


                    case DIN_CMD_BDL_LOAD: //son (DIN_BIT_ABORT + DIN_BIT_SKIP + DIN_BIT_DUT1 + DIN_BIT_DUT2)  //0x6C  (108)
                    {
                        char     strBdlName[200] = "";
                        ::ZeroMemory(&strBdlName, sizeof(strBdlName)); 


                        MyTrace(PRT_BASIC, "ChildView1:                                   <======== DIN_CMD_BDL_LOAD (0x%02x)\n", g_nCurDioCmd);  //son220211

                        //son201216: FileDialog가 열려 있는데 BDL 자동로딩 메시지 수신시 다운문제 관련 추가
                        //son BDL Loading FileDialog가 열려있으므로 자동로딩을 수행할 수 없음.
                        if (nFlag_FileDialogOn == 1)    //son201216
                        {
                            // FileDialog가 열려 있으므로 로딩실패했다는 응답을 메인장비로 보낸다. 
                            g_ComiDaq2.DoPutAll(DOUT_CMD_SHORT);    //son221028 //son231016 
                            MyTrace(PRT_LEVEL1, "ChildView1_LINE_%d g_ComiDaq2.DoPutAll(): 0x%02x  =========> BDL_LOAD Fail \n",    //son231018
                                                                 __LINE__, DOUT_CMD_SHORT); 
                            strTemp.Format("First of All, Close FileDialog!!\n\n\"BDL File Loading Request\" from MainHandler was failed!"); //son201216
                            MyTrace(PRT_BASIC, strTemp);        //son211022
                            AfxMessageBox(strTemp, MB_OK);
                            break;
                        }

                        //son201028 BDL 자동로딩기능
                        //son201028 "Z:\\AutoLoad.txt" 파일을 확인하여 파일에 적혀 있는  BDL 모델명을 확인한다. 
                        CString str3; 
                        g_nPrevDioCmd = g_nCurDioCmd;
                        bool bRet = CheckBDLName_fromZDrive(strBdlName);    //son Output: strBdlName
                        if (bRet != true)   // BDL 이 존재하지 않으면
                        {
                            // 요청한 BDL 파일이 존재하지 않으므로 로딩실패했다는 응답을 메인장비로 보낸다. 
                            g_ComiDaq2.DoPutAll(DOUT_CMD_SHORT);    //son231016
                            MyTrace(PRT_LEVEL1, "ChildView1_LINE_%d g_ComiDaq2.DoPutAll(): 0x%02x  =========> BDL_LOAD Fail \n",    //son231018
                                                                __LINE__, DOUT_CMD_SHORT); 

                            //son BDL 파일을 못 찾아서 Fail 처리했다는 메시지 박스를 띄운다. 
                            //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
                            str3.Format("\"%s: BDL File Loading Request\" from MainHandler was failed!\n\n", strBdlName); //son201118 
                            strTemp += str3;
                            str3.Format("Can't find the BDL name in \"%s\\BDL\" or  \"C:\\BDL\" Folders\n", SysInfoView01.m_pStrFilePath1 ); 
                            strTemp += str3;
                            MyTrace(PRT_BASIC, strTemp);        //son211022
                            AfxMessageBox(strTemp, MB_OK);  //son210514
                            break;
                        }

                        memset(g_fSparkRefData, 0, sizeof(g_fSparkRefData)); // hdpark220511
                        memset(g_nSparkRefDataCount, 0, sizeof(g_nSparkRefDataCount)); // hdpark220511

                        //son221028 g_ComiDaq2.DoPutAll(DOUT_CMD_PASS); //son231016

                        //son LoadBDLFile() 수행 직전에 0으로, 수행 직후 1로 설정. 
                        nFlagFileLoad2=0;//son201217

                        //son201028: Main 장비가 지정한 BDL로 자동로딩한다. 
                        //           새로운 SysInfoView01.m_pStrFilePathJig1 를 이용해서 BDL을 로딩한다.
                        //           OnDataLoadAuto() 호출로는 auto test가 제대로 되지 않아서 OnDataLoad_Type_A() 관련 코드로 바꿈.
                        //           OnDataLoad_Type_A()에서 CFileDialog 은 띄우지 않고 동작하는 코드임.
                        int ret = LoadBDLFile(LOAD_BDL_CURR_PATH);  //son221028
                        if (ret == -2)  //son221028 begin:
                        {
                            // FileDialog가 열려 있으므로 로딩실패했다는 응답을 메인장비로 보낸다. 
                            g_ComiDaq2.DoPutAll(DOUT_CMD_SHORT);    //son231016

                            strTemp.Format("First of All, Close FileDialog!!\n\n\"BDL File Loading Request\" from MainHandler was failed!"); 
                            MyTrace(PRT_BASIC, strTemp);        
                            AfxMessageBox(strTemp, MB_OK);
                            break;
                        }   //son221028 end

                        // 요청한 BDL 파일이 존재하므로 로딩했다는 응답을 메인장비로 보낸다. 
                        g_ComiDaq2.DoPutAll(DOUT_CMD_PASS); //son221028 LoadBDLFile(LOAD_BDL_CURR_PATH) 호출 직후로 위치 이동.  //son231016
                        MyTrace(PRT_LEVEL1, "ChildView1_LINE_%d g_ComiDaq2.DoPutAll(): 0x%02x  =========> BDL_LOAD PASS \n",    //son231018
                                                                __LINE__, DOUT_CMD_PASS); 

                        // g_pvBDLData2Op 생성 전에 LoadBDLFile() return이 됐다면 g_pvBDLData2Op[1][1]->size() 호출하지 않는다. 
                        if (g_pvBDLData2Op[1][1] != NULL) //son221028 BDL Open 다이얼로그 취소시에 SW Down 문제 수정.
                        {
                            //son220907_2 MAX_OPEN_TEST: nBDLData2Op[][] 개수 Range Check 기능 추가
                            int nOpenCount = g_pvBDLData2Op[1][1]->size() - 1;
                            int nTotalOpenCount = nOpenCount * g_nPieceMax1 * nMultyNetTotal;
                            if (nTotalOpenCount > MAX_OPEN_TEST)
                            {
                                str.Format("Error No 1035, \n\n LoadBDLFile(): nOpenCnt(%d) * g_nPieceMax1(%d) * nMultyNetTotal(%d)= %d  >%d ? Range Over!\n", 
                                                nOpenCount, g_nPieceMax1, nMultyNetTotal, nTotalOpenCount, MAX_OPEN_TEST );
                                AfxMessageBox(str);
                                MyTrace(PRT_BASIC, str);
                            }
                        }

                        //son  LoadBDLFile() 수행 직후 1로 설정. 
                        nFlagFileLoad2=1;//son201217 

                        //son201217 OnDataLoad_Type_A() 버튼 수행시와 최대한 동일하게 Save 기능도 추가.
                        OnButton1002();  

                        //son 메인장비에서 요청한 새로운 BDL 파일을 로딩했다는 메시지를 띄운다. 
                        strTemp.Format("\"%s: BDL File Loading Request\" by MainHandler Succeeded.\n", strBdlName);
                        MyTrace(PRT_BASIC, strTemp);        //son211022
                        AfxMessageBox(strTemp, MB_OK);
                    }
                        break;

                    default:                            
                        break;

                }

                //g_nPrevDioCmd = g_nCurDioCmd;	//son231018
            }

#ifdef _PC_TEST_
            // VIEW7_TIMER_DIO 디버그중 타이머 쌓이는 현상을 막기 위해 Timer처리 완료 시점에 다시 시작
            SetTimer(VIEW1_TIMER_DIO,           2000,NULL); //son 2sec   SetTimer
#endif
        }
        break;


    }

    CFormView::OnTimer(nIDEvent);

}

//son201028 BDL 자동로딩기능
//son201028 "Z:\\AutoLoad.txt" 파일을 확인하여 파일에 적혀 있는  BDL 모델명을 확인한다. 
//son Output: strBdlName
//    return: true(BDL을 찾은 경우),  false (BDL을 못 찾은 경우)
bool CChildView1::CheckBDLName_fromZDrive(char* pStrBdlName)
{
    //---------------------------------------------------------------------------------------
    // D:\\FromHandler\\AutoLoad.txt" 파일을 read해서 자동로딩해야 할 BDL 이름을  가져온다. 
    FILE *fp;
    char fName[200], strBdlName[200];
    CString str, strTemp;

    ::ZeroMemory(&fName, sizeof(fName));
    //sylee201215    strcat(fName, "Z:\\AutoLoad.txt");     
    strcat(fName, SysInfoView01.m_pStrFileDrive1 );//sylee201215 Z드라이브 In/Out 분리기능으로 추가
    strcat(fName, "\\AutoLoad.txt");               //sylee201215 
    fp = fopen(fName,"rt");
    if(fp == NULL){ 
        strTemp.Format("Error No 1700 : Can't open \"%s\" file.\n", fName);
        MyTrace(PRT_BASIC, strTemp);        //son211022
        AfxMessageBox(strTemp, MB_OK);
        return false;
    }

    //----------------
    //son File Read
    ::ZeroMemory(&strBdlName, sizeof(strBdlName));
    fscanf(fp, "%s\n", strBdlName);
    if (pStrBdlName != NULL)
        strcpy(pStrBdlName, strBdlName);    //return value 

    fclose(fp);


    //-----------------------------------------------------------------------
    // C:\ACE500\BDL 폴더 내부를 탐색해서 원하는 BDL 폴더가 있는지 확인한다
    strTemp.Format("%s\\BDL", SysInfoView01.m_pStrFilePath1);   //son "C:\\ACE500\\BDL" //son210514
    str.Format("%s", strBdlName);
    bool bRet = Search_BDLFolder(strTemp, str);                                         //son210514 
    if (bRet == true)   // 해당하는 BDL 폴더가 있다면)
    {
#ifdef _PC_TEST_        
        //strTemp.Format("Found \"C:\\ACE500\\BDL\\%s\" BDL Folder!\n", str); //son201118
        strTemp.Format("BDL AutoLoad: Found \"%s\\BDL\\%s\" BDL Folder!\n", SysInfoView01.m_pStrFilePath1, str); //son201118 //son210514
        MyTrace(PRT_BASIC, strTemp); 
#endif

        //son true를 리턴한다.
        return true;
    }

    //-----------------------------------------------------------------------
    // C:\BDL 폴더 내부를 탐색해서 원하는 BDL 폴더가 있는지 확인한다
    bRet = Search_BDLFolder("C:\\BDL", str); 
    if (bRet == true)   // 해당하는 BDL 폴더가 있다면)
    {
#ifdef _PC_TEST_        
        strTemp.Format("BDL AutoLoad: Found \"C:\\BDL\\%s\" BDL Folder!\n", str);
        MyTrace(PRT_BASIC, strTemp); 
#endif

        //son true를 리턴한다.
        return true;
    }

    //son 원하는 BDL file을 찾지 못한 경우 false를 리턴
    return false;

}



//son201028 BDL 자동로딩기능
//  C:\ACE400\BDL 폴더 내부를 탐색해서 원하는 BDL 폴더가 있는지 확인다
bool CChildView1::Search_BDLFolder(CString ACEpath, CString strBdlName)
{
    CFileFind   finder;
    CString     strTemp;


    // build a string with wildcards
    CString     strWildcard(ACEpath);
    strWildcard += _T("\\*.*");

    // start working for files
    BOOL bWorking = finder.FindFile(strWildcard);


    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        // skip . and .. files; otherwise, we'd
        // recur infinitely!
        if (finder.IsDots())
            continue;


        if (finder.IsDirectory())
        {
            CString dirName = finder.GetFileName();
            CString dirPath = finder.GetFilePath();

            //son strBdlName에 맞는 dirName을 찾았다면
            if (strcmp(dirName, strBdlName) == 0)
            {
                //son 맞는 폴더를 찾았는데 안이 비었다면 (???.BDL 파일이 없다면)  실패를 리턴한다.
                CString r_strNewBdlName;        //son211027
                bool bRet = CheckEmptyBdlFolder(dirPath, r_strNewBdlName);  //son211027
                if(bRet == TRUE)    //son Empty Folder
                {
                    strTemp.Format("%s is Empty Folder. \n\nCan't find any .BDL File. \n", dirPath);
                    MyTrace(PRT_BASIC, strTemp);        //son211022
                    AfxMessageBox(strTemp, MB_OK);
                    return false;
                }

                //son201028 strBdlName에 맞는 폴더를 찾았다. 
                //son211027 CheckEmptyBdlFolder()가 dirPath 폴더내부의 .BDL의 name으로 
                //          r_strNewBdlName을 output변수로 돌려주고 false(empty 아님)를 리턴.
                ChangeFilePathJigName(dirPath+"\\"+r_strNewBdlName+".BDL",  //son BDL full path 설정: C:\ACE400\BDL\036901A-2\036901A-2.BDL
                                      dirPath,                              //son BDL 폴더 path 설정: C:\ACE400\BDL\036901A-2
                                      r_strNewBdlName+".BDL");              //son BDL file name 설정: 036901A-2.BDL
                return true;
            }

        }
    }


    return false;
}


//son201028 BDL 자동로딩기능
//son201028 빈 디렉토리인지 검사한다.   (TRUE: Empty)
//bool CChildView1::CheckEmptyFolder(CString dirPath, CString strBdlName)
bool CChildView1::CheckEmptyBdlFolder(CString dirPath,           //son input : dirPath 
                                      CString &r_strNewBdlName)  //son output: 찾은 BDL파일에서 .BDL 제외한 only name.  //son211027
{
    int         nFile = 0;
    bool        bBdlFound = false;
    CFileFind   finder_sub;

    CString     strWildcard(dirPath);
    strWildcard += _T("\\*.*");                     // build a string with wildcard
    BOOL bWorking = finder_sub.FindFile(strWildcard); // start working for files
    while (bWorking)
    {
        bWorking = finder_sub.FindNextFile();
        if (finder_sub.IsDots())
            continue;

        if (finder_sub.IsDirectory())
            continue;

        CString dataFileName = finder_sub.GetFileName();
#if 0
        //son AutoLoad.txt에 "abcde"라고 적혀 있다면 abcde.BDL만 맞는 BDL 파일로 인정이 되는 경우 
        //    BDL 폴더와 폴더안의 .BDL 파일이 이름이 같아야 BDL 자동로딩이 수행됨. 
        CString bdlFileName  = strBdlName + ".BDL";
        if (strcmp(dataFileName, bdlFileName) == 0)
        {
            bBdlFound = true;
            return false;   // 맞는 BDL이 있다면 Empty가 아니므로 false리턴
        }
#else
        //son211027  나노시스요청. BDL폴더안의 파일이 BDL 폴더 이름이 아니더라도 .BDL이기만 하면 
        //           자동 로딩이 되게 만들어 달라고 함. 
        
        //son 파일 name에 ".BDL"이 포함된 파일을 bdl 파일로 인정하고 name 부분만 output 으로 돌려준다.
        CString strExt;
        strExt = dataFileName.Right(4);
        if (strcmp(strExt, ".BDL") == 0)
        {
            int no = dataFileName.Find('.');
            r_strNewBdlName = dataFileName.Left(no);     //son .BDL 앞의 이름 부분만 output으로 write
        
            bBdlFound = true;
            return false;   // 맞는 BDL이 있다면 Empty가 아니므로 false리턴
        }   //son211027

#endif

        nFile++;

    }

    // 맞는 .BDL 파일이 없다면 빈 폴더
    if (bBdlFound == false)
        return true;

    // 파일이 하나도 없다면 빈 폴더
    if (nFile == 0)
        return true;

 

    return false;
}

//son201028 BDL 자동로딩기능
//son201028 strBdlName에 맞는 폴더를 찾았으므로 FilePathJigName을 변경하고 true를 리턴.
void CChildView1::ChangeFilePathJigName(CString fullPath, CString DirName, CString fileName)
//    fullPath  ex) C:\ACE400\BDL\036901A-2\036901A-2.BDL 
//    DirName   ex) C:\ACE400\BDL\036901A-2
//    fileName  ex) 036901A-2.BDL
{

//son C:\ACE400\BDL\036901A-2\036901A-2.BDL         // SysInfoView01.m_pStrFilePathJig2  : 현재 BDL full path
//son C:\ACE400\BDL\036901A-2                       // SysInfoView01.m_pStrFilePathBDL   : 현재 BDL 폴더 path
//son 036901A-2.BDL                                 // SysInfoView01.m_pStrFileBDLName   : 현재 BDL file name


    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1, sizeof(SysInfoView01.m_pStrFilePathJig1));
    strcat(SysInfoView01.m_pStrFilePathJig1, fullPath);

    ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig2, sizeof(SysInfoView01.m_pStrFilePathJig2));
    strcat(SysInfoView01.m_pStrFilePathJig2, fullPath);

    ::ZeroMemory(&SysInfoView01.m_pStrFilePathBDL, sizeof(SysInfoView01.m_pStrFilePathBDL));
    strcat(SysInfoView01.m_pStrFilePathBDL, DirName);

    ::ZeroMemory(&SysInfoView01.m_pStrFileBDLName, sizeof(SysInfoView01.m_pStrFileBDLName));
    strcat(SysInfoView01.m_pStrFileBDLName, fileName);


    //son201217 삭제.CChildView1::OnTimer()->OnButton1002()->OnDataSave()로 최종적으로 Save를 하므로
    //          불필요한 "BDL SET OK!. ....BDL" 메시지를 추가로 출력하지 않도록 여기서는 삭제함.
    //son 새로운 SysInfoView01.m_pStrFilePathJig1 name을 Setup file에 write한다.
    //OnDataSave(); //save button  

}

void CChildView1::OnDataLoad_Type_A()  //open button// sylee201120906  joytech
{       
    CString str;

    //son201109 OnDataLoad_Type_A() 의 본체는 LoadBDLFile(int nLoadBdlType)로 변경.
    //  LoadBDLFile(LOAD_BDL_FILE_DIALOG)로 호출하면 기존 OnDataLoad_Type_A() 와 동일하다.
    int ret = LoadBDLFile(LOAD_BDL_FILE_DIALOG);    //son221028

    // g_pvBDLData2Op 생성 전에 LoadBDLFile() return이 됐다면 g_pvBDLData2Op[1][1]->size() 호출하지 않는다. 
    if (g_pvBDLData2Op[1][1] != NULL) //son221028 BDL Open 다이얼로그 취소시에 SW Down 문제 수정.
    {
        //son220907_2 MAX_OPEN_TEST: nBDLData2Op[][] 개수 Range Check 기능 추가
        int nOpenCount = g_pvBDLData2Op[1][1]->size() - 1;
        int nTotalOpenCount = nOpenCount * g_nPieceMax1 * nMultyNetTotal;
        if (nTotalOpenCount > MAX_OPEN_TEST)
        {
            str.Format("Error No 1035, \n\n LoadBDLFile(): nOpenCnt(%d) * g_nPieceMax1(%d) * nMultyNetTotal(%d)= %d  >MAX_OPEN_TEST(%d) ? Range Over!\n", 
                            nOpenCount, g_nPieceMax1, nMultyNetTotal, nTotalOpenCount, MAX_OPEN_TEST );
            AfxMessageBox(str);
            MyTrace(PRT_BASIC, str);
        }
    }

}


//son201028 BDL 자동로딩기능
//son201109: OnDataLoad_Type_A() 수정본.
//  Main 장비가 지정한 새로운 SysInfoView01.m_pStrFilePathJig1 를 이용해서 BDL을 로딩하는 기능을 추가
//  LoadBDLFile(LOAD_BDL_CURR_PATH)로 호출하면 BDL 자동로딩기능으로 사용가능하고, 
//  LoadBDLFile(LOAD_BDL_FILE_DIALOG)로 호출하면 기존 OnDataLoad_Type_A() 와 동일하다.
int CChildView1::LoadBDLFile(int nLoadBdlType)  //son221028 return int 추가 
{
    FILE *fp; 
    CFile file;


    int  i, j, nMulPin1;//sylee130303
    int nLibEnable;//sylee151202
    char fName[200], szTemp1[100];
    CString str, strVal1,str2;//sylee150817

#if 1 //son221028 begin: 
// File Open 취소시에 SW down 문제 관련해서 FileOpen Dialog 코드를 BDL 데이터 초기화 코드보다 위로 이동함.
// File Open 취소를 해도 이전 BDL data를 유지하기 위함.
    ::ZeroMemory(&fName, sizeof(fName));

    //-----------------------------------------------------------
    //son201109 CFileDialog를 띄워서  BDL Filename을 받아 온다.
    CString strBdlName;
    if (nLoadBdlType == LOAD_BDL_FILE_DIALOG)
    {
        //son201216 BDL Loading FileDialog가 열려있고 로딩중이라는 의미로 1으로 초기화 한다.
        nFlag_FileDialogOn= 1;
        if (GetBDLFileName_FileDialog(strBdlName) < 0)      
        {    
            nFlag_FileDialogOn= 0;          //son201216

            //son File DialogOpen 실패했다는 의미로 -1 설정
            nFlagFileLoadok= -1;//son200115  파일 open시에 BDL 자동로딩이 들어와서 file open을 취소한 경우,
                              //          로딩 없이 이전 BDL에 대한 BDL Load OK라는 메시지가 출력되는 문제.
                              //          메시지 출력되지 않도록 flag를 -1으로 설정하는 코드 추가
                              
            //son221028 begin:
            str.Format("Error No 1008-1  :  Open BDL File Fail! \n\nPlease reload the BDL file properly for normal operation.\n");
            MyTrace(PRT_BASIC, str);        
            AfxMessageBox(str, MB_OK);          
            return -1;  
            //son221028 end
        }

        //------------------
        //son BDL file name
        ::ZeroMemory(&SysInfoView01.m_pStrFileBDLName, sizeof(SysInfoView01.m_pStrFileBDLName));
        strcat( SysInfoView01.m_pStrFileBDLName , strBdlName );//SYLEE121202 

        //------------------
        //son BDL dir name

        //son 현재 디렉토리명을 SysInfoView01.m_pStrFilePathBDL에 할당.
        ::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePathBDL);//SYLEE121202
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , SysInfoView01.m_pStrFilePathBDL );//SYLEE121202


        //-----------------------------------
        //son BDL full path  : for file open
        str2="\\"+strBdlName;//sylee150817  //SYLEE151202   //son str2 : ex) "\10VGAG011J.BDL"
        strcat( fName , str2 );

        ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig2, sizeof(SysInfoView01.m_pStrFilePathJig2));
        ::ZeroMemory(&SysInfoView01.m_pStrFilePathJig1, sizeof(SysInfoView01.m_pStrFilePathJig1));

        strcat( SysInfoView01.m_pStrFilePathJig2, fName );  //SYLEE20111212

        strcat( SysInfoView01.m_pStrFilePathJig1, SysInfoView01.m_pStrFilePathJig2);


    }
  
    //-----------------------------------------------------------
    //son201109 BDL 자동로딩: 정해진 BDL fileName대로  BDL 로딩을 진행
    else
    {
        //son201216 BDL Loading FileDialog가 열려있으므로 자동로딩을 수행할 수 없음.
        if (nFlag_FileDialogOn == 1)
            return -2;  //son221028

        strBdlName = SysInfoView01.m_pStrFileBDLName; //son201109 BDL file Name
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , SysInfoView01.m_pStrFilePathJig2 );         //son201109 BDL full path
    }
#endif //son221028 end

    nLibEnable=1;//sylee151202   /lib=2
    
    ::ZeroMemory(&nBDLHead1, sizeof(nBDLHead1));
    ::ZeroMemory(&nBDLData1, sizeof(nBDLData1));
    ::ZeroMemory(&nBDLData2, sizeof(nBDLData2));//sylee20120403
    ::ZeroMemory(&nBDLData3, sizeof(nBDLData3));//sylee20120403
    ::ZeroMemory(&szTemp1, sizeof(szTemp1)); 

    ::ZeroMemory(&nProOneP_Short1_Co, sizeof(nProOneP_Short1_Co));
    ::ZeroMemory(&nProOneP_Short1_Li, sizeof(nProOneP_Short1_Li));
    ::ZeroMemory(&nProOneP_Open1, sizeof(nProOneP_Open1));
    ::ZeroMemory(&nProOneP_Short1, sizeof(nProOneP_Short1));//sylee131012   
     
    nProOneP_ModelCount1=0; 
    nProOneP_ModelCheck1=0;//sylee131018
    nProOneP_ModelCheck1Retry=0;//sylee131018

#ifdef __OPVECTOR__
    //son g_pvBDLData2Op vector 전체를 초기화한다.
    DeleteAllOpenTestItem();        //son200420-4WNET10000

    //son g_pvBDLData31 vector 전체를 초기화한다.
    DeleteAllShortTestItem();        //son200420-4WNET10000
#endif

    //son "\SETUP2\Recipe.ini"를 struct CSysInfo05에 Load(type=2)
    FileSysInfo01.LoadSaveSub15(2);//SYLEE140110 
 
    if(SysInfo05.m_nProSpMode==2 ||  SysInfo05.m_nProSpMode==4 ){//sylee130916  
        nProOneP_Flag1=1;//model start
        nProOneP_RunFlag1=1;
    }
    else{ 
        nProOneP_Flag1=0;
        nProOneP_RunFlag1=0;
    } 

    if(  SysInfo05.m_nProSpMode !=3){ //sylee140210 
        nFlagEstModelMake=0;
        nFlagEstModelMakeSelect=0 ;
    }
    

    //son "\SETUP2\PieceRange.ini"를 struct CSysInfoPieceRange에 Load(type=2)
    FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD);//sylee130228import. 
                                                   //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo



    //son 입력받은 BDL 파일을 Open
    fp=fopen(fName,"rt");
    if(fp == NULL){ 
        str.Format("Error No 1008-1  : ..\\BDL\\%s  Open File Reading  !.", strBdlName);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        nFlag_FileDialogOn= 0;  //son201216
        return -3;  //son221028
    } 
    MyTrace(PRT_BASIC, "%s BDL file Reading....\n", fName);     //son220228

    //son BDL 파일명을 ChildView1 화면 라벨에 출력하고 파일에 저장한다.
    str.Format("%s", SysInfoView01.m_pStrFilePathJig1);
    m_LabelPath.SetCaption(str);
    FileSysInfo01.LoadSaveView01(_SAVE);//SYLEE121202


    CString stnADCx5="OPEN-READ.BDL";//sylee180611-1
    nADCx5=0;//sylee180611-1
    if( SysInfoView01.m_pStrFileBDLName==stnADCx5){//sylee180611-1
        nADCx5=2;//sylee180611-1
    } //sylee180611-1

    //son "BDL File Load OK" 표시 플래그를 초기화 한다.
    nFlagFileLoadok=0;//sylee161116-2


    if( nLibEnable==2)//sylee151202
    {

        CString str2,str1;  //##2
        HFILE   hFile;
        int ret;

        str2.Format("%s",SysInfoView01.m_pStrFilePathJig2);
        str2.TrimRight('l');str2.TrimRight('L');//sylee150308
        str2.TrimRight('d');str2.TrimRight('D');//sylee150308
        str2.TrimRight('b');str2.TrimRight('B');//sylee150308
        str2.TrimRight('.'); //sylee150308
        str1.Format("%s.lib",str2);//sylee150308

        ::ZeroMemory(&nData2, sizeof(nData2)); 
        ::ZeroMemory(&fName, sizeof(fName));

        str=str1;
        strcat( fName ,  str );  

        hFile = _lopen(str, OF_READWRITE | OF_SHARE_DENY_NONE);
        if(hFile == -1) {
            str.Format("Error No 201 :   File Read error!. -> %s  ",str);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            _lclose(hFile);
            nFlag_FileDialogOn= 0;  //son201216
            fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
            return -4;  //son221028
        }

        ret= _lread(hFile, (LPVOID)&nData2, (UINT)sizeof(nData2));
        if(nData2[0][0]<1){
            str.Format("Error No 202 :   File Read error!. -> %s   data empty!",str);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            _lclose(hFile);
            nFlag_FileDialogOn= 0;  //son201216
            fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
            return -5; //son221028
        }
        _lclose(hFile);

        nBDLHead1[1][1][1]=nData2[0][1];
        nBDLHead1[1][2][1]=nData2[0][2];//pin total 
        nBDLHead1[1][2][2]=nData2[0][3];
        nBDLHead1[1][2][3]=nData2[0][4];
        nBDLHead1[1][3][1]=nData2[0][2];

        nHpin=0;
        nLpin=0;//sylee050309

        if( nData2[0][3]>0){//sylee050309
            nHpin=nData2[0][3];
        }
        if( nData2[0][4]>0){//sylee050309
            nLpin=nData2[0][4];
        }

    }

    //----------------------
    //son Read BDL Header 
    //----------------------
    else//if( LibEnable==2){//sylee151202
    {

        fscanf(fp, "%d\n", &nBDLHead1[1][1][1]);                //son 20000 (BDL 파일표시)
        fscanf(fp, "%d"  , &nBDLHead1[1][2][1]);                //son 총핀수
        fscanf(fp, "%d"  , &nBDLHead1[1][2][2]);                //son 상부핀수
        fscanf(fp, "%d"  , &nBDLHead1[1][2][3]);                //son 하부핀수
        fscanf(fp, "%d\n", &nBDLHead1[1][3][1]);//list sum      //son 총핀수

    }//if( LibEnable==2){//sylee151202  end


    //son BDL file 여부 체크
    if(nBDLHead1[1][1][1]!=20000){
        str.Format("Error No 1009  : BDL FILE ?   HEADER  DATA 20000?   Reading ERROR  !.\n  ..\\BDL\\%s", strBdlName);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
        fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
        nFlag_FileDialogOn= 0;  //son201216
        return -6; //son221028
    }

    //son BDL file 핀 갯수 체크    : 총핀수가 1보다 작다면
    if(nBDLHead1[1][3][1]<1){
        nBDLHead1[1][3][1]=0;
        str.Format("Error No 1111  :   BDL FILE.  HEADER Info.:  Pin Totle list No. data zero.  Reading ERROR  !. \n         ..\\BDL\\%s", strBdlName);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
        fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
        nFlag_FileDialogOn= 0;  //son201216
        return -7; //son221028
    }
    ////SYLEE120905//////////////////////////////////////

    char fName3[200];//, fName4[200]; 

    ::ZeroMemory(&fName3, sizeof(fName3));
    if(szTSetup2Name[0]==0){
        strcat( fName3 , SysInfoView01.m_pStrFileDri1);//sylee121022
    }
    else{
        strcat( fName3 , szTSetup2Name);//sylee121022
    }
    ::ZeroMemory(nNetList1, sizeof(nNetList1)); //SYLEE120902

    //----------------------
    //son NetList 생성
    //----------------------
    //son 현재 BDL을 Net 갯수별로 그루핑한 Net List를 생성한다.  
    //  ex: 싱글Net 몇개, 2pin Net 몇개, ... 10pin Net 몇개 ...
    long Sum1,Sum2;// ##3
    int nTotal1,nTotal2,Namo1;
    if( nLibEnable==2)//sylee151202
    {
        Sum1=0; Sum2=0; Namo1=0;                    
        nTotal1=nBDLHead1[1][3][1];

        for(i=1; i<=nBDLHead1[1][3][1]; i++){
            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                nBDLData1[1][i][j]= nData2[i][j-1];//sylee150308   //##4
            }
            nT1Net=nBDLData1[1][i][5];//SYLEE120902
            nNetList1[nT1Net]++;//SYLEE120902
            Sum1=Sum1+nData2[i][0];//##5
            Sum2=Sum2+nData2[i][4];
        } 

    }
    else//if( LibEnable==2){//sylee151202
    {

        //----------------------
        //son Read BDL Data 
        //----------------------
        for(i=1; i<=nBDLHead1[1][3][1]; i++)            //son nBDLHead1[3][1] : 총핀수
        {
            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                fscanf(fp, "%d", &nBDLData1[1][i][j]);       
            }
            fscanf(fp, "\n");
            nT1Net=nBDLData1[1][i][5];//SYLEE120902     //son Net의 총 PinCount
            nNetList1[nT1Net]++;//SYLEE120902
        } 

        ///////////////////////////////////////////////////////////////

        gFlag_Start_Int_bdlError = 0;//sylee170717

        int nCh1 ;//sylee170717-1
        nCh1=0;//sylee170717-1       
        fscanf(fp, "%d", &nCh1); //sylee170717-1
        if(nCh1 > 0)//sylee170717-1
        {

            m_list1.ResetContent();
            m_list2.ResetContent();//sylee150819
            m_list3.ResetContent();//sylee150819
            m_list4.ResetContent();//sylee150819

            str.Format("      ( Total Pin ?,  More  pin list? )  " );
            m_list1.InsertString(0,str);
            str.Format("     " );
            m_list1.InsertString(0,str);
            str.Format(" ***> ERROR!  =>   BDL FILE CHECK!    " );
            m_list1.InsertString(0,str);

            str.Format("     " );
            m_list1.InsertString(0,str);

            nBDLHead1[1][3][1]=0;
            nBDLHead1[1][2][1]=0;

            //son BDL 파일 에러
            gFlag_Start_Int_bdlError = 1;//sylee170717

            str.Format("Error 1201 : BDL FILE  Total Pin Error !.    \n      ..\\BDL\\%s", strBdlName);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);

            fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
            nFlag_FileDialogOn= 0;  //son201216
            return -8; //son221028
        }

        ///////////////////////////////////////////////////////////////

    }//if( LibEnable==2){//sylee151202  end


    fclose(fp);  
    nFlag_FileDialogOn= 0;  //son201216
//========================================================================================

    if(nBDLHead1[1][3][1]>0){
         nTotalPin=nBDLHead1[1][3][1];//sylee130522
    }
    else{
         nTotalPin=0;//sylee130522
    }
 
    if( nLibEnable==2)//sylee151202
    {
        if( nTotal1>0){//sylee150309
            Namo1=(int)fmod(Sum1,10000.);         
            nTotal2=nData2[nTotal1+1][0];
            // nTotal2=7402;//TESTINGING
            if( Sum1>0){
                if( Namo1!=nTotal2){
                    str.Format("  Error No 1301,   Pin Check Sum Error!   \n\n  ??? (%d != %d) (Set Sum ,Load Sum)  \n  \n      ",  nTotal2,Namo1); 
                    MyTrace(PRT_BASIC, str);        //son211022
                    AfxMessageBox(str, MB_OK);
                    nAutoStartInterlock100=2;//sylee050308  //sylee150602
                }
            }
            Namo1=(int)fmod(Sum2,10000.);

            nTotal2=nData2[nTotal1+1][4];
            if( Sum2>0){
                if( Namo1!=nTotal2){
                    str.Format("  Error No 1301,   Pin Group Count  Sum Error!   \n\n  ??? (%d != %d) (Set Sum ,Load Sum)  \n  \n      ",  nTotal2,Namo1); 
                    MyTrace(PRT_BASIC, str);        //son211022
                    AfxMessageBox(str, MB_OK);
                    nAutoStartInterlock100=2;//sylee050308  //sylee150602
                }
            }
        }
    }//if( LibEnable==2){//sylee151202  end


    //-----------------------------
    //son Z:NETLIST.TXT 파일 생성
    //-----------------------------

    //son NetList Data로 고객에게 출력하여 제공할 NETLIST.TXT 파일을 출력한다.
    FILE *fp2; 
    char  fName2[200]; 
 
    ::ZeroMemory(&fName2, sizeof(fName2));
    strcat( fName2 , "Z:\\NETLIST.TXT" );
 
    fp2 = fopen(fName2,"wt");
    if(fp2 == NULL){
        str.Format("Error No 1911  :  Network Driver Check!. ( Z DRIVER)     Network Driver disconnection.      NetList Make Error, Check  Netlist file! ");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    else//sylee131121
    {

        fprintf(fp2, "<<NetList>>\n");
        //  str.Format("Error No 1008  : ..\\BDL\\%s  Open File Reading  !.", strBdlName);
        CTime curTime = CTime::GetCurrentTime();
        //fprintf(fp, "2011 \n" );
        fprintf(fp2, "*>Date:%d/%d/%d, %d:%d:%d\n",
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        int Temp1,Sum1,Sum2;
        Sum1=Sum2=0;
        for( i=1;i<=Def_SwitchMaxPin; i++){//SYLEE120902
            if(nNetList1[i]>0){
                Temp1=nNetList1[i]=nNetList1[i]/i;
                if(Temp1>0){//sylee171108-1
                    fprintf(fp2, "Length[%5d],Count=%5d\n",i, Temp1);
                }
            }
            Sum1=Sum1+nNetList1[i];
        }
        Sum2=nNetList1[1];

        fprintf(fp2, "----------------------\n");
        fprintf(fp2, "* Single Count,=%5d\n", Sum2);
        fprintf(fp2, "* Net    Count,=%5d\n", Sum1-Sum2);
        fprintf(fp2, "----------------------\n");
        fprintf(fp2, "* Total  Count,=%5d\n", Sum1);
        // curTime = CTime::GetCurrentTime();
        // fprintf(fp2," * YEAR%04d/ MONTH%02d/ DAY%02d  %02dH%02dM%02dS  \n ", 
        //                  curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
        fprintf(fp2,"* %s\n", strBdlName );
        fprintf(fp2, "----------------------\n");

        //  str.Format("Error No 1008  : ..\\BDL\\%s  Open File Reading  !.", strBdlName);
        fclose(fp2); 

    }//sylee131121



    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //화면 display

    m_list1.ResetContent();
    m_list2.ResetContent();//sylee150819
    m_list3.ResetContent();//sylee150819
    m_list4.ResetContent();//sylee150819
    str.Format("          *NO.,        *PIN,      *NET NO,    *NET COUNT  " );
    m_list1.InsertString(0,str);

    ::ZeroMemory(&szTemp1, sizeof(szTemp1));

    for(i=1; i<=nBDLHead1[1][3][1]; i++)
    {

        if(i> MAX_PIN){//SYLEE150917-3
            str.Format("Error No 1030 :  *.bdl   loading fail     PIN Line(=%d) > %d. \n Check .BDL  FILE PIN Line count!", i, MAX_PIN);    //son210831
            m_list1.AddString(str);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox( str );  
            return -9; //son221028
        }           

        if( nBDLData1[1][i][1] < 0  ||   nBDLData1[1][i][1]>=DEF_MAX_PIN1 ){
            str.Format("Error No 1011  :   BDL FILE.  Error line=%d.   (PIN NO < 0  ||   PIN NO>= %d  ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",
                                                    i, DEF_MAX_PIN1, strBdlName);
            m_list1.AddString(str);
        }
        if( nBDLData1[1][i][2] < 0  ||   nBDLData1[1][i][2]>=DEF_MAX_PIN1 ){
            str.Format("Error No 1012  :   BDL FILE.  Error line=%d.   (NET LIST < 0  ||  NET LIST>= %d   ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",
                                                    i ,DEF_MAX_PIN1, strBdlName);
            m_list1.AddString(str);
        }

        if( nBDLData1[1][i][5] < 1){
            str.Format("Error No 1013  :   BDL FILE.  Error line=%d.   NET LIST COUNT <1.     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",
                                                    i , strBdlName);
            m_list1.AddString(str);
        }

        str.Format("  NO=%7d,     %10d,     %10d,      %10d,  ", i,  nBDLData1[1][i][1]+1, nBDLData1[1][i][2], nBDLData1[1][i][5] );
        //  m_list1.InsertString(0,str);   /REVERSE   DISPLAY
        m_list1.AddString(str);
    }

    //son BDL Data NetCount 오류 점검       
    if (Check_BDLNetCountError(1) < 0)       //son210831    //son210929
        return -10; //son221028




    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //OPEN    DATA 
 
    int nFirstPin;      //son 현재 Net의 대표핀
    int netPinCnt;      //son 현재 Net의 PinCount       nNetCo1 -> netPinCnt
    int netPinIdx;      //son 현재 pin의 Net 내부 index    nCo1 -> netPinIdx
    int nTmp11,nTmp12,nTmpCo1,nTmpPin1;//SYLEE130222JOYTECH
    int nDebug[500];//debug
    int nBdlStart,nBdlEnd;

    ::ZeroMemory(&nDebug, sizeof(nDebug));//debug

    nTmpCo1=0;//SYLEE130222JOYTECH      
    nFirstPin=0; 
    netPinIdx=0; 

    //--------------------------------------------------------------------------
    //son "\SETUP2\PieceRange.ini"를 struct CSysInfoPieceRange에 Load(type=2)
    FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD); //SYLEE130312JOYTECH
                                                    //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
                                                    
    //son210430 Piece.ini와 PieceRange.ini 불일치시 SW 다운 문제 보완
    FileSysInfo01.LoadSavePieceInfo(_LOAD);//son210430 추가
    if (SysInfoPieceRange.m_nPieceCount[1] != SysInfoPiece.m_nPiece)    //son210430 
    {
        str2 = "";
        str.Format("Error No 1020  :  \n\n PieceCount info between PieceRange.ini(=%d) and Piece.ini(=%d) is different!\n", 
                                                            SysInfoPieceRange.m_nPieceCount[1], SysInfoPiece.m_nPiece );
        str2 += str;
        str.Format("\n%s BDL PieceCount Error!. Check .\\Setup2\\PieceRange.ini and Piece.ini.\n", SysInfoView01.m_pStrFilePathBDL);
        str2 += str;
        MyTrace(PRT_BASIC, str2);        //son211022
        AfxMessageBox(str2, MB_OK);         

        //son BDL 파일 에러 표시
        gFlag_Start_Int_bdlError = 1;
        return -11; //son221028
    }
    

    //son 1) PieceRange.ini의 정보로 g_nPinPieceNo[] 정보 생성
    //    g_nPinPieceNo[] : 각 핀의 piece 번호 정보를 저장.  index는 pinNo, value는 pieceNo
    ReadPinPieceNo(1);//SYLEE130222JOYTECH

    //son g_nPinPieceNo[]에서 BDL 핀의 piece 정보를 획득하여 nBDLData1[i][BDL_PIECE] 에 저장.
    int pinIdx;
    for (pinIdx=1; pinIdx <= nBDLHead1[1][3][1]; pinIdx++){
        nTmp11=nBDLData1[1][pinIdx][1];         //son pinN    (0~)
        nTmp11=g_nPinPieceNo[1][nTmp11+1];      //son pieceNo (1~)
        nBDLData1[1][pinIdx][BDL_PIECE]=nTmp11;  // set pieceNo  //son221102 7:BDL_PIECE
    }


    //---------------------------------------------
    //son piece 1부터 nBDLData1을 재배치한다.
    
    int nSortNo1[DEF_MAX_PIN1][8];//16k  //sylee150604  9000->33000  //sylee150917-2  16400  33000
    ::ZeroMemory(&nSortNo1, sizeof(nSortNo1));

    //son 보통 "BDL폴더\Setup\PieceRange.ini" 를 확인해 보면 piece 1부터 배치돼 있으므로
    //    Sort한 결과 nSortNo1[netPinIdx]가 원본 nBDLData[pinIdx]과 보통 똑같다.
    int pieceIdx;   //son221116
    netPinIdx=0; 
    for (pinIdx=1; pinIdx <= nBDLHead1[1][3][1]; pinIdx++)
    {
        nBdlStart=pinIdx;
        nBdlEnd=nBdlStart+nBDLData1[1][pinIdx][5]-1;        //son 해당 net의 총핀수 -1
        if (nBdlStart>nBdlEnd)//sylee131105
        {
            str.Format(" **> Error!   BDL FILE,   TOTAL POINT CHECK !!! ");
            m_list1.InsertString(0,str);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox("  ERROR 1111,  BDL NET LIST LOADING ERROR.      BDL FILE, TOTAL POINT  CHECK!  ", MB_OK);            
            break;//sylee131128 continue;//errr?
        }

        //son NetEnd 위치로 pinIdx를 옮겨간다. 
        pinIdx=nBdlEnd;                 

        for(pieceIdx=1; pieceIdx<=g_nPieceMax1; pieceIdx++) 
        {
            //son                           pin net         cnt     piece#
            //son                        0   1   2   3   4   5   6   7 
            // ex)  k2=1, nBDLData1[1] = 0, 264, 1,  0,  1,  4,  1,  1
            // ex)  k2=2, nBDLData1[2] = 0, 393, 1,  0,  1,  4,  1,  1
            // ex)  k2=3, nBDLData1[3] = 0, 272, 1,  0,  1,  4,  1,  1
            // ex)  k2=4, nBDLData1[4] = 0, 401, 1,  0,  1,  4,  1,  1

            for(int k2=nBdlStart; k2<=nBdlEnd; k2++) 
            {
                if(pieceIdx==nBDLData1[1][k2][7])   //son  pieceIdx가  Pin에 해당하는 Piece Num 이면
                {
                    netPinIdx++;
                    for(int m2=1; m2<=7; m2++) {
                        nSortNo1[netPinIdx][m2]=nBDLData1[1][k2][m2];
                        //son pieceIdx가 1인
                        //son                      pin net         cnt     piece#
                        //son                   0  1    2   3   4   5   6   7
                        //son ex) nSortNo1[1] = 0, 264, 1,  0,  1,  4,  1,  1
                        //son ex) nSortNo1[2] = 0, 393, 1,  0,  1,  4,  1,  1
                        //son ex) nSortNo1[3] = 0, 272, 1,  0,  1,  4,  1,  1
                        //son ex) nSortNo1[4] = 0, 401, 1,  0,  1,  4,  1,  1

                    }
                }
            }
        }
    }

 
    //son  piece 1부터 순서대로 정렬된 Sort한 결과를 nSortNo1[pinIdx]를 nBDLData1[nDut][pinIdx]에 덮어쓴다.
    for(pinIdx=1; pinIdx<=nBDLHead1[1][3][1]; pinIdx++)
    {
        for(int m2=1; m2<=7; m2++){
           nBDLData1[1][pinIdx][m2]=nSortNo1[pinIdx][m2];
        }
    }

    ////////////////////////////////////////////////////////////////



    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    int nTpin1;

    //son  nBDLPinT1[nTpin1] : BDL 로딩된 pin+1 값을 index로 하는 BDL 테이블 
    //     BDL 로딩시에 pin을 index로 접근해서  1로 표시한다. 
    ::ZeroMemory(&nBDLPinT1, sizeof(nBDLPinT1));//sylee190104
    ::ZeroMemory(&nBDLPinT2, sizeof(nBDLPinT2));//sylee190104
    for(i=1; i<=nBDLHead1[1][3][1]; i++){
        nTpin1=nBDLData1[1][i][1]+1;//sylee190104
        nBDLPinT1[nTpin1]=1;//sylee190104
    }

    //son nBDLPinT2[nTPin1] : nBDLPinT1[nTpin1]의 역 data. 
    //   nBDLPinT2[1] ~ nBDLPinT2[BDL갯수] : 현재 로딩된 Pin+1 값의 리스트. 올림차순 
    nTpin1=0;
    for(i=1; i<=DEF_MAX_PIN1; i++){
        if(nBDLPinT1[i]==1){
            nTpin1++;
            nBDLPinT2[nTpin1]=i;
        }            
    }
    nBDLPinT2[0]=nTpin1;
    nSelf1_SetStartPin=1;
    nSelf1_RunPin1=0;
    if((nTpin1>1) && (nTpin1<=DEF_MAX_PIN1)){
        nSelf1_RunEnable=1;//sylee190105
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    netPinIdx=0; 
    int shortPinCnt=0;//short
    int openPinCnt=0;//OPEN 
    for(pinIdx=1; pinIdx<=nBDLHead1[1][3][1]; pinIdx++)     //son nBDLHead1[3][1])  ex) 32   (총핀수)
    { 
        //son  net pin 수가 1이면 One point net이다.
        if( nBDLData1[1][pinIdx][BDL_NET_PINCNT]==1){  //short  if one point
            shortPinCnt++;
            nBDLData3[1][shortPinCnt][BDL_PIN0]      =nBDLData1[1][pinIdx][BDL_PIN0];                           //son 대표핀 pinNo
            nBDLData3[1][shortPinCnt][BDL_NET_PINCNT]=nBDLData1[1][pinIdx][BDL_NET_PINCNT];//sylee150909-1 2N1+ //son netPinCount
            nBDLData3[1][shortPinCnt][BDL_PIECE]     =nBDLData1[1][pinIdx][BDL_PIECE];                          //son pinPiece
            netPinIdx=0;
            continue;
        }

        //son netPinCnt가 1보다도 작다면 해당 라인은 의미 없음          
        if( nBDLData1[1][pinIdx][BDL_NET_PINCNT]<1){            // netPinCnt
            netPinIdx=0;
            continue;
        }

        netPinIdx++;                  

        
        //-------------------------
        //son ShortTest List 생성
        //-------------------------
        //son Net의 첫번째 pin이라면 short 관련 nBDLData3[1][shortPinCnt]을 채운다.
        if( netPinIdx==1)
        {

            nFirstPin=nBDLData1[1][pinIdx][BDL_PIN0];           //son 현재 Net의 대표핀
            netPinCnt=nBDLData1[1][pinIdx][BDL_NET_PINCNT];     //son 현재 Net의 PinCount

            //son short Test는 각 Net의 대표핀이 대상이므로 대표핀이면 shortPinCnt가 증가되어야 한다.
            shortPinCnt++;//short 
            nBDLData3[1][shortPinCnt][BDL_PIN0]      =nBDLData1[1][pinIdx][BDL_PIN0];//short                    //son 1: 대표핀 pinNo
            nBDLData3[1][shortPinCnt][BDL_NET_PINCNT]=nBDLData1[1][pinIdx][BDL_NET_PINCNT];//sylee150909-1 2N1+ //son 5: netPinCount
            nBDLData3[1][shortPinCnt][BDL_PIECE]     =nBDLData1[1][pinIdx][BDL_PIECE];                          //son 7: piece
            if(netPinCnt>1){        
                nTmp11=g_nPinPieceNo[1][nFirstPin+1];       //son 대표핀의 pieceNo
            }

        }

        //son count가 Net의 PinCount 갯수 이내라면
        else if(netPinIdx <= netPinCnt) 
        {

            //////////////////////////////////////////////////////////////
            nTmpPin1=nBDLData1[1][pinIdx][BDL_PIN0];    //son pinNo
            nTmp12=g_nPinPieceNo[1][nTmpPin1+1];        //son 현재pin의 pieceNo

            //son 대표핀의 pieceNo와 현재pin의 pieceNo가 다르다면 예외케이스처리
            if(nTmp11!=nTmp12){
                netPinIdx =0;//sylee131212      
                pinIdx=pinIdx-1;//sylee131212       //son pinIdx를 원복하고 for 초기로 돌아감
                continue;
            }    
            nMulPin1=pinIdx;//sylee130303



           
            //-------------------------
            //son OpenTest List 생성
            //------------------------- 
            //son PinNo가 nFirstPin이 아니면 openTest 대상핀. 
            //    openTest는 대표핀을 기준으로 net 내부의 각 핀을 테스트한다. 
            if(nFirstPin!=nBDLData1[1][pinIdx][1]) //sylee130515        //son pinNo
            {
                openPinCnt++;               

                
                //  if(SysSet13.m_nSet2==1){ //SYLEE180412 FAKEOPEN ADD
#ifdef DEFINE_FAKEOPEN //sylee170415
                if(openPinCnt==1){//sylee180127-1  //SYLEE180207-1-FakeOpen
                    nBDLData2[1][openPinCnt][1]=nFirstPin;
                }
                else{//sylee180127-1 //SYLEE180207-1-FakeOpen
                    nBDLData2[1][openPinCnt][1]=nBDLData1[1][pinIdx-1][1];//sylee180127-1  //SYLEE180207-1-FakeOpen
                }//sylee180127-1

#else  //DEFINE_FAKEOPEN //sylee170415 

                nBDLData2[1][openPinCnt][1]=nFirstPin;                  //son 대표핀
#endif//DEFINE_FAKEOPEN //sylee170415                   

                nBDLData2[1][openPinCnt][2]=nBDLData1[1][pinIdx][1];    //son openTest대상  PinNo
                nBDLData2[1][openPinCnt][3]=nBDLData1[1][pinIdx][5];    //son Net Pin Count
                nBDLData2[1][openPinCnt][4]=nBDLData1[1][pinIdx][6];    //son PieceNo ?? 1고정수??
            } //sylee130515-test


            //son 해당 net의 마지막핀이라면
            if(netPinIdx==netPinCnt){
                netPinIdx=0;
            }                   

        }

    }

    //son OpenPinCnt 설정
    if (pinIdx > 0){
        if( (pinIdx-1)==nBDLHead1[1][3][1] ){
            nBDLData2[1][0][1]=openPinCnt;  // OPEN BDL 총 갯수  //SYLEE20111212
        }
    }

    //son ShortPinCnt 설정
    //son200420-4WNET10000: shortPinCnt 설정 위치를 현재 위치로 이동 
    //son221103 if (shortPinCnt > 1)   //short        
    if (shortPinCnt >= 1) { //short        //son221103 shortPinCnt 1 일때 nBDLData3[1][0][1] 에 write안 되는 문제 수정.
        nBDLData3[1][0][1]=shortPinCnt;   // short  BDL 총 갯수  //SYLEE20111212
    }


    //-----------------------------------
    //son Piece별 ShortTest List 생성
    
    //son 1) PieceRange.ini의 정보로 g_nPinPieceNo[] 정보 생성
    //    g_nPinPieceNo[] : 각 핀의 piece 번호 정보를 저장.  index는 pinNo, value는 pieceNo
    ReadPinPieceNo(1);//SYLEE121202

    //son 2) piece정보를 이용해서 nBDLData3[nDUT]를  g_pvBDLData31[nDUT][piece] 에 piece별로 재배치한다. InsertShortTest()
    makePieceShortTestData(1);       //son221103_2

    //son piece를 1개일 때의 통검사 ShortTest Data를 NetPinCount 기준으로 정렬한다.
    Sort_nBDLData3_byNetPinCount(1);   //son200722-BDL-Sort: 신규 추가

    //son piece별 ShortTest Data를 NetPinCount 기준으로 정렬한다.
    Sort_nBDLData31_byNetPinCount(1);  //son200722-BDL-Sort: 신규 추가


    //------------------------------------
    //son Piece별 OpenTest List 생성
    
    //sylee130219joytech    FileSysInfo01.LoadSavePieceRangeInfo(_LOAD);    //SYLEE130107   
    g_nPieceMax1=SysInfoPieceRange.m_nPieceCount[1];    //SYLEE130107
    if( g_nPieceMax1<1){//sylee130228
        str.Format("Error No 1032, \n\n LoadBDLFile():  g_nPieceMax1(%d) <1 ? Range Over!\n",  g_nPieceMax1); //son220907_2
        AfxMessageBox(str);         //son220907_2
        MyTrace(PRT_BASIC, str);    //son220907_2
    }
    
    makePieceOpenTestData(1);       //son for DUT1  //son200420-4WNET10000

    //son220907_2 Range Check 추가
    //  nOpenCount(open테스트 개수) * piece개수가 32768을 넘어서면 ChildView7::OpenMes1[201][i]가 range over 될 수 있음.
    //  max piece 300인 경우 open Test 개수 109가 넘으면 range over가 발생하므로 에러메시지를 출력한다.   
    int nOpenCount = g_pvBDLData2Op[1][1]->size() - 1;
    if ((nOpenCount * g_nPieceMax1) >= MAX_PIN)
    {
        //son221116 piece 1에는 16761개,  piece 2에는 100개 이런식으로 불균형하게 pin이 할당 될 수 있으므로 
        //          단순히 곱하지 말고 더해서 체크하도록 한다. 
        int nOpenAllPieceCount = 0;
        for(pieceIdx=1; pieceIdx<=g_nPieceMax1; pieceIdx++) 
            nOpenAllPieceCount += g_pvBDLData2Op[1][pieceIdx]->size() - 1;

        if (nOpenAllPieceCount >= MAX_PIN)
        {

            str.Format("Error No 1034, \n\n LoadBDLFile(): nOpenAllPieceCount = %d  >=MAX_PIN(%d) ? Range Over!\n piece1 nOpenCount=%d, pieceCount=%d\n", 
                            nOpenAllPieceCount, MAX_PIN, nOpenCount, g_nPieceMax1 );
            AfxMessageBox(str);
            MyTrace(PRT_BASIC, str);
        }
    }



    //---------------------------------------------------------------------------------
    // Debug Print: Piece별 OpenTest List, ShortTest List 가 제대로 생성되었는지 확인 


    //son 생성한 Dut, piece별 COpenTestItem g_pvBDLData2Op[dut][piece]를  "D:\\log\\0OpenPinListPiece_DUT%d" 에 출력한다. (확인용)
    int nIPOpenTotal = 0;
    nIPOpenTotal = printPieceOpenTestData(1);   //son200420-4WNET10000

    //son 생성한 Dut, piece별  CShortTestItem g_pvBDLData31[dut][piece]를 "D:\\log\\0ShortPinListPiece_DUT%d" 에 출력한다. (for debug)
    printPieceShortTestData(1);   //son200420-4WNET10000
    LeakMultPinList1(1);    //sylee230526


    //son 생성한 piece별 open nBDLData2Op[][][]를 원래의 nBDLData2[][][]의 카운트와 비교해서 
    //    제대로 생성이 되었는지 확인한다.
    if(nBDLData2[1][0][1]!=nIPOpenTotal){
        str.Format(" ERROR NO 4001,\n BDL OPEN LIST LINK ERROR.  BDL RELOADIG!  ,\n CHECK BDL FILE or PIECE RANGERANGE SET  or FILE !   \n\n  DON'T  RUN \n\n  list1=%d,list2=%d \n\n ", nBDLData2[1][0][1], nIPOpenTotal);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }


    //------------------------------------------------------------------
    //son m_list에 로딩한 BDL 리스트와 Open List의 정보를 출력한다.
    str.Format("          "); 
    m_list1.InsertString(0,str);  //SYLEE131213
    str.Format(" ***>BDL LIST      = %d      ", nBDLHead1[1][3][1] );    
    m_list1.InsertString(0,str);  //SYLEE131213
    nBDLData1[1][0][2]=nBDLHead1[1][3][1]; 
    str.Format(" ***>OPEN LIST   = %d    ", nBDLData2[1][0][1] ); 
    m_list1.InsertString(0,str);  //SYLEE131213


#if 0   //son221103_3  printPieceOpenTestData() 와 내용 중복되는 파일 출력이므로 삭제.
    //--------------------------------
    // Save OpenTest Data File 
    //--------------------------------
 
    str.Format("OPENSETUPLIST"); 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        //  str.Format("Error No 1001  : D:\\AUTO\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
        str.Format("Error No 1014 :   .\\BDL\\ OPENSETUPLIST   file Data Saving Error !     file close!.");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return  -12; //son221028
    }

    CTime curTime = CTime::GetCurrentTime();

    //fprintf(fp, "2011 \n" );
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
            curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : OPEN LIST  FILE \n");  
    fprintf(fp, " \n"); 
    fprintf(fp," ***>OPEN LIST Count =%d    Read Complete", nBDLData2[1][0][1] );
    fprintf(fp, " \n");
    fprintf(fp, " \n"); 
    fprintf(fp,"  PIN, PIN, \n"  );

    //son nFirstPin, nTestPin을 출력한다.
    for(i=1; i<=nBDLData2[1][0][1]; i++){   
        //SYLEE150111    fprintf(fp, "    %d,   %d\n",nBDLData2[1][i][1], nBDLData2[1][i][2]);  //set path file
        fprintf(fp, "    %d,   %d\n", nBDLData2[1][i][1]+1, nBDLData2[1][i][2]+1);  //set path file//SYLEE150111
    }

    fclose(fp);  
#endif

    if(nBDLData2[1][0][1]>0){			//son221103_3 begin:
        if(nBDLData3[1][0][1]<1){
            nBDLData3[1][0][1]=1;//sylee161129
        }
    } //son221103_3 end

#if 0   //son221103_3  printPieceShortTestData() 와 내용 중복되는 파일 출력이므로 삭제.
    //--------------------------------
    // Save ShortTest Data File 
    //--------------------------------
    str.Format("SHORTSETUPLIST"); 
    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , str );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format("Error No 1015  : .\\BDL\\ SHORTSETUPLIST   file Data Saving Error !     file close!.");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          
        return  -13; //son221028
    }

    curTime = CTime::GetCurrentTime();

    //fprintf(fp, "2011 \n" );
    fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
                                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  
                                curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
    fprintf(fp, "* SETUP  : SHORT LIST  FILE \n");  
    fprintf(fp, " \n"); 
    fprintf(fp," ***>SHORT LIST Count =%d    Read Complete", nBDLData3[1][0][1] );
    fprintf(fp, " \n");
    fprintf(fp, " \n"); 
    fprintf(fp,"      PIN,   NetPinCount,   Piece \n"  );

    for(i=1; i<=nBDLData3[1][0][1]; i++){   
        fprintf(fp, "    %5d,        %4d,    %2d\n", nBDLData3[1][i][BDL_PIN0]+1,  //set path file  ////sylee150527
                                                  nBDLData3[1][i][BDL_NET_PINCNT],
                                                  nBDLData3[1][i][BDL_PIECE]);
    }
    fclose(fp);  
#endif

    int temp3;//sylee150909 2N1+

    temp3=nBDLData3[1][0][1];//sylee150909 2N1+ 
    str.Format(" ***>SHORT LIST = %d    ", nBDLData3[1][0][1] );         
    m_list1.InsertString(0,str);  //SYLEE131213     //son m_list1의 맨위에 출력한다.

    OnDataLoad_READ_ConRead(); // sylee170525-1

    FileSysInfo01.LoadSaveSet13(2);//load //sylee170607

    int a;//sylee130518

    //--------------------
    //son PinMap 로딩
    //--------------------
    //son "C:\\ACE400\\SETUP\\SysSetPin.ini"를 struct CSysSet12에 Load(type=2)
    a=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
    if(a != 1)
    {
        str.Format(" Error No 1112, \n    SysSet12.ini  Load ERROR !  \n    file Read Error !!!");
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);    
    }
    //son220901 begin : D:\Log 폴더에 Pin 설정, pinBlock 정보 출력 추가
    MyTrace(PRT_BASIC, "\n");
    MyTrace(PRT_BASIC, "    SysSet13.m_nPinBlockType= %s\n", STR_PIN_BLOCK(SysSet13.m_nPinBlockType));  
    MyTrace(PRT_BASIC, "       SysSet12.m_nPinTotMax= %s\n", STR_MAX_PIN(SysSet12.m_nPinTotMax));       
    MyTrace(PRT_BASIC, "SysSet12.m_nLowerJigStartPin= %d\n", SysSet12.m_nLowerJigStartPin);
    MyTrace(PRT_BASIC, "               Density Upper= %s %s %s %s\n", STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper), 
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper2), 
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper3), 
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper4));
    MyTrace(PRT_BASIC, "               Density Lower= %s %s %s %s %s %s\n", STR_BOXQD_TYPE(SysSet12.m_nPinQDLower), //son230217 
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDLower2), 
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDLower3), 
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDLower4),
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDLower5),    //son230217
                                                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDLower6));   //son230217
    //son220901 end


    ::ZeroMemory(PinSearchPin, sizeof(PinSearchPin)); //sylee130518-1 pin search bdl
    short wJigPin, wHwPin;//sylee130518-1 pin search bdl
    for(i=1; i<=nBDLHead1[1][3][1]; i++)    //son  총 핀수
    {
        wJigPin = nBDLData1[1][i][1];//sylee130518-1 pin search bdl //son BDL pin 번호(jigPin. 0부터시작)
        wHwPin  = PinMap[2][wJigPin];                               //son jigPin에 해당하는 SwitchCardPin(hwPin)

        //son-32KPIN-20200113
        // 20K인데 32K 영역의 BDL을 로딩한다면 없는 B,C, D 박스의 PinMap[2][pin]는 -1로 리턴됨. 
        // 이 경우는 계측기 SW 오동작할 수 있으므로 미리 경고해 주기 위해 nErrCount 추가함.
        // nErrCount가 1이상이면 ModeSet12.make32kHwPin()에서  -1을 리턴해서 오류가 있음을 표시해 준다.
        
        // wHwPin값이 마이너스일 때, PinSearchPin[wHwPin] 접근하는 것을 방지한다.
        if (wHwPin < 0)
        {
            str.Format(" Error No 1113, \n Can't find hwPin for userPin_%d! \n\n Wrong BDL File loaded which does not match LowerStartPin(%s_%d), Hw MaxPin(%s)!\n",  
                        wJigPin+1, STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                        SysSet12.m_nLowerJigStartPin, STR_MAX_PIN(SysSet12.m_nPinTotMax) );
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);    

            //son BDL 파일 에러 표시
            gFlag_Start_Int_bdlError = 1;

            //son MainFrm에서 아래변수를 체크해서 ChildView7 (Auto Test 화면)으로 못넘어가게 함. 
            //    여기서 강제로 1로 설정해서  32K 모드 변환이 자유롭도록 한다.
            nFlagFileLoad2 = 1;
            return -14; //son221028
        }
        PinSearchPin[wHwPin] = 1;//sylee130518-1 pin search bdl
    }

    //-------------------------------------------------------------------
    //son openTest핀수 + shortTest핀수 => BDL pin 갯수와 일치해야 한다.
    //-------------------------------------------------------------------
    int pinCnt, openPinCnt2, shortPinCnt2, totCnt;//sylee131213
    pinCnt      = nBDLHead1[1][3][1];   //son BDL Pin 갯수
    openPinCnt2 = nBDLData2[1][0][1];   //son openTest List 갯수 : 대표핀이 아닌 핀 갯수
    shortPinCnt2= nBDLData3[1][0][1];   //sylee131213 //son shortTest List 갯수 : 대표핀 갯수
    totCnt = openPinCnt2 + shortPinCnt2;

    str.Format("     " );
    m_list1.InsertString(0,str);  

    nAutoStartInterlock100=0;//sylee050308  //son221103 moved from under. 
    if( pinCnt!=totCnt)     // 불일치. 에러출력
    {
        nAutoStartInterlock100=2;//sylee05030 //son221103 moved from under.8
        str.Format("  \n\n Error No 1001,  BDL Loading Fail.   \n\n\n\n    DON'T  RUN! .   Manager Call!   \n\n\n\n\n   BDL TotalPin = SUM (Open Pin + Short Pin) ==>\n\n\n  ( %d  = %d(%d+%d)?  ) \n\n\n\n ", pinCnt, totCnt, openPinCnt2, shortPinCnt2);
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);          

        str.Format(" * Total Pin %d != %d+%d => ERROR !  Don't Run!  " ,  pinCnt, shortPinCnt2, openPinCnt2 );
        m_list1.InsertString(0,str);  //SYL
        str.Format("     " );
        m_list1.InsertString(0,str);   

        str.Format("  Don't Run!  ==>   FAIL!  " );
        m_list1.InsertString(0,str);  //SYLEE140324 INTERLOCK
        str.Format("     " );
        m_list1.InsertString(0,str); 

        m_list1.AddString(str);
        str.Format("    End  ERROR !.    " );
        m_list1.AddString(str); 

        //son BDL 파일 에러 표시
        gFlag_Start_Int_bdlError = 1;   //son221103_4

    }
    else 
    {
        //son221104 begin: shortPinCount 1개 일때 Recipe에 short Test 있으면 warning 출력 
        if (shortPinCnt2 <= 1 && FileSysInfo01.checkRecipeStep_ShortExist() == true)  
        {
            str.Format("\n Warning No 1036, Short Test Pin Count is only \'1\'.\n\n ShortTest, LeakTest included in Recipe cannot be performed\n");  
            MyTrace(PRT_BASIC, str);     
            AfxMessageBox(str, MB_OK);          

        } //son221104 end

        str.Format("    OK .   "   );
        m_list1.InsertString(0,str);  //SYL
        str.Format("             " );
        m_list1.InsertString(0,str);  //SYL
        m_list1.AddString(str);
        str.Format("    End OK .    "  );
        m_list1.AddString(str);
    }

    FileSysInfo01.LoadSaveSet16(2);//sylee130325-//estimation
    FileSysInfo01.LoadSaveSet19(2);//sylee150407//estimation

    nFlagEstModelMakeCount=0;//sylee130325 

    a=FileSysInfo01.LoadSaveSet13(_LOAD);//load //sylee150407//estimation

    if(a!=1){
        str.Format(" Error No 1112, \n    SysSet13.ini  Load ERROR !  \n    file Read Error !!!");    
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str, MB_OK);
    }

    if (SysInfo05.m_nProSpMode==2 || SysInfo05.m_nProSpMode==3 || SysInfo05.m_nProSpMode==4 ) //sylee150417-1 
    {
        if(SysSet16.m_nSet11==1){  //estimation
            nFlagEstModelMake=1;//sylee130325
        }
        else{
            nFlagEstModelMake=0;//sylee130325
        }
    }

#if 0   //son221103 상단에 동일 코드 있어서 삭제함.
    nAutoStartInterlock100=0;//sylee050308   //sylee150602    //##6
    if (nBDLHead1[1][3][1] != (nBDLData3[1][0][1]+nBDLData2[1][0][1]))  //sylee050308     //sylee150602 
    {
        nAutoStartInterlock100=2;//sylee050308
        str.Format(" Error!  BDL file,  UDL FILE CHECK!   ");//sylee050308
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str);

        str.Format(" Error!  Don't RUN!   ,  Engineer Call!    BDL LOADING ERROR .   TOTAL PIN=%d != ( open pin=%d + short pin=%d)????    ", 
                            nBDLHead1[1][3][1], nBDLData2[1][0][1], nBDLData3[1][0][1] );
        MyTrace(PRT_BASIC, str);        //son211022
        AfxMessageBox(str); //sylee050308
    }
#endif

    nMultyNetTotal=1;//sylee150817
    OnDataLoad_READ_ConRead(); // sylee150922-1  //sylee151111add

    str.Format("%d", nMultyNetTotal); 
    m_LabelDut.SetCaption(str);



    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //SYLEE180810-1-DUT16
    
    int i2;

    //son200720 MULTI NET2~ NET16
    //son 이 코드로 대체하고 위의 MULTI NET1, NET2, NET3, NET4 관련 코드는 삭제
    //son m_list1, m_list2, m_list3, m_list4마다 어떻게 다르게 출력할지 고려해야 함. 
    //    -> p_list[nDut]  포인터 array사용하도록 수정.   
    //son Multi net 4이상은 BDL 파일이 출력이 되지 않는 것도 고려해야 함.
    //    현재 구조상 BDL 4개 이상을 화면에 출력해 주는 것도 크게 의미는 없어 보임.
    //    -> if (nDut <= 4) 일때에만 display하도록 수정.
    CString str3;
    //for (int nDut=2; nDut <= DefDutMax; nDut++)   //son nDut <= 33 이면 2~33으로  for 문을 돌게 된다.   
    for (int nDut=2; nDut <= MAX_DUT; nDut++)      //son220824_3 DefDutMax(33) ->  MAX_DUT(32)로 수정
    { 

        //son201217 SysInfoView01.m_pStrFilePathBDL이 자동로딩기능에서 중간에 바뀌는 문제점 수정.
        //    DUT2부터는 DUT1과 path가 같으므로 에서는 굳이 이걸 수행해서 currPath를 overwrite할 필요 없다.
        //    자동로딩 기능에서는 이걸해서 오히려 이전에 FileOpen시의 path로 돌아가는 문제가 있었음. 
        //    코멘트로 기능을 막음.
        //::GetCurrentDirectory(MAX_PATH, SysInfoView01.m_pStrFilePathBDL);//SYLEE121202

        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , SysInfoView01.m_pStrFilePathBDL );//SYLEE121202 //son ex) "C:\BDL\10VGAG011J"

        str2.Format("%d",nDut); //SYLEE180810-1-DUT16
        //str2="\\"+str3+str2; //SYLEE180810-1-DUT16   
        str2="\\"+strBdlName+str2; //son ex) "\10VGAG011J.BDL2"
                                     //son201210 수정  
                                     // DUT2 이상 str2 이상 설정문제 수정.  str3가 할당 함수가 밖으로 빠져서
                                     // 이 지점에서 str3가 비어 있었음. str3대신에 strBdlName 을 사용하도록 수정함.
        
        strcat( fName , str2 );//sylee150817            //son ex) "C:\BDL\10VGAG011J\\10VGAG011J.BDL2"

        if ( (_access(fName , 0 ))== -1 ) { //sylee150817
            return 1; //son221028 .BDL2가 없는 것은 정상상황이므로 1을 return.
        }

        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format("Error No 1008-3  : ..\\BDL\\%s  Open File%d Reading  !.",  str2,nDut);//sylee150817
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);          
            return -3; //son221028
        }    

        MyTrace(PRT_BASIC, "%s BDL Reading....\n", fName);     //son220228

        //----------------------
        //son Read BDL Header  
        //----------------------

        fscanf(fp, "%d\n", &nBDLHead1[nDut][1][1]);
        fscanf(fp, "%d"  , &nBDLHead1[nDut][2][1]);
        fscanf(fp, "%d"  , &nBDLHead1[nDut][2][2]);
        fscanf(fp, "%d"  , &nBDLHead1[nDut][2][3]);         
        fscanf(fp, "%d\n", &nBDLHead1[nDut][3][1]);//list sum

        //son BDL file 여부 체크
        if(nBDLHead1[nDut][1][1]!=20000){
            str.Format("Error No 1009  : BDL FILE ?   HEADER  DATA 20000?   Reading ERROR  !.\n  ..\\BDL\\%s", str2);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
            return  -6; //son221028
        }

        //son BDL file 핀 갯수 체크    : 총핀수가 1보다 작다면
        if(nBDLHead1[nDut][3][1]<1){
            nBDLHead1[nDut][3][1]=0;
            str.Format("Error No 1111  :   BDL FILE.  HEADER Info.:  Pin Totle list No. data zero.  Reading ERROR  !. \n         ..\\BDL\\%s", str2);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
            fclose(fp);     //son201109: fp close 없이 return하는 오류 수정 
            return  -7; //son221028
        }
        ////SYLEE120905//////////////////////////////////////


        //----------------------
        //son Read BDL Data 
        //----------------------
        char fName3[200];//, fName4[200];
        ::ZeroMemory(&fName3, sizeof(fName3));
        if(szTSetup2Name[0]==0){
            strcat( fName3 , SysInfoView01.m_pStrFileDri1);//sylee121022
        }
        else{
            strcat( fName3 , szTSetup2Name);//sylee121022
        }
        ::ZeroMemory(nNetList1, sizeof(nNetList1)); //SYLEE120902

        for(i=1; i<=nBDLHead1[nDut][3][1]; i++)
        {
            //son Read BDL Data
            for(j=1; j<=6; j++){  //sylee20120218 PIECE
                fscanf(fp, "%d", &nBDLData1[nDut][i][j]);        
            }

            //son NetList Data 생성
            fscanf(fp, "\n");
            nT1Net=nBDLData1[nDut][i][5];//SYLEE120902
            nNetList1[nT1Net]++;//SYLEE120902
        } 

        fclose(fp);  


        if(nBDLHead1[nDut][3][1]>0){
            nTotalPin=nBDLHead1[nDut][3][1];//sylee130522
        }
        else{
            nTotalPin=0;//sylee130522
        }

        //-------------------------------
        //son NETLIST_DUT?.TXT 파일 출력
        //-------------------------------

        //son NetList Data로 고객에게 출력하여 제공할 NETLIST.TXT 파일을 출력한다.
        ::ZeroMemory(&fName2, sizeof(fName2));
        strcat( fName2 , "Z:\\NETLIST.TXT" );


        fp2 = fopen(fName2,"at");
        if(fp2 == NULL){
            str.Format("Error No 1911  :  Network Driver Check!. ( Z DRIVER)     Network Driver disconnection.      NetList Make Error, Check  Netlist file! ");
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
        }
        else//sylee131121
        {

            str.Format("<<NetList %d >>\n",nDut); //SYLEE180810-1-DUT16
            fprintf(fp2, str );

            int Temp1,Sum1,Sum2;
            Sum1=Sum2=0;
            for( i=1;i<=Def_SwitchMaxPin; i++){//SYLEE120902
                if(nNetList1[i]>0){
                    Temp1=nNetList1[i]=nNetList1[i]/i;
                    if(Temp1>0){//sylee171108-1
                        fprintf(fp2, "Length[%5d],Count=%5d\n",i, Temp1);
                    }
                }
                Sum1=Sum1+nNetList1[i];
            }
            Sum2=nNetList1[1];

            fprintf(fp2, "----------------------\n");
            fprintf(fp2, "* Single Count,=%5d\n", Sum2);
            fprintf(fp2, "* Net    Count,=%5d\n", Sum1-Sum2);
            fprintf(fp2, "----------------------\n");
            fprintf(fp2, "* Total  Count,=%5d\n", Sum1);

            // curTime = CTime::GetCurrentTime();
            // fprintf(fp2," * YEAR%04d/ MONTH%02d/ DAY%02d  %02dH%02dM%02dS  \n ", 
            //              curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
            fprintf(fp2,"* %s\n", str2 );
            fprintf(fp2, "----------------------\n");

            //  str.Format("Error No 1008  : ..\\BDL\\%s  Open File Reading  !.", strBdlName);
            fclose(fp2); 

        }//sylee131121




        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //화면 display
        
        CListBox*   p_list[4];
        p_list[0] = &m_list1; 
        p_list[1] = &m_list2; 
        p_list[2] = &m_list3; 
        p_list[3] = &m_list4; 
        if (nDut <= 4)  //m_list가 4개밖에 없으므로  nDut 4이하인 경우에만 Display 한다.
        {

            p_list[nDut-1]->ResetContent();

            str.Format("          *NO.,        *PIN,      *NET NO,    *NET COUNT  " );
            p_list[nDut-1]->InsertString(0,str);

            ::ZeroMemory(&szTemp1, sizeof(szTemp1));    //sylee150916   
            for(i=1; i<=nBDLHead1[nDut][3][1]; i++)
            {   

                //sylee150916   ::ZeroMemory(&szTemp1, sizeof(szTemp1));

                if(i> MAX_PIN){//SYLEE150917-3
                    str.Format(" ERROR!   *.bdl%d   loading fail     PIN NO.  >  %d     .BDL%d  FILE PIN NO CHECK!", 
                                nDut, MAX_PIN, nDut );   
                    p_list[nDut-1]->AddString(str);
                    MyTrace(PRT_BASIC, str);        //son211022
                    AfxMessageBox( str );  
                    return -9; //son221028
                }

                //son Check pinNo 
                if( nBDLData1[nDut][i][1] < 0  ||   nBDLData1[nDut][i][1]>=DEF_MAX_PIN1 ){
                    str.Format("Error No 1011  :   BDL FILE.  Error line=%d.   (PIN NO < 0  ||   PIN NO>= %d  ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",
                                                                    i, DEF_MAX_PIN1 , str2);
                    p_list[nDut-1]->AddString(str);
                }
                //son Check NetNo  
                if( nBDLData1[nDut][i][2] < 0  ||   nBDLData1[nDut][i][2]>=DEF_MAX_PIN1 ){
                    str.Format("Error No 1012  :   BDL FILE.  Error line=%d.   (NET LIST < 0  ||  NET LIST>= %d   ).     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",
                                                                    i ,DEF_MAX_PIN1, str2);
                    p_list[nDut-1]->AddString(str);
                }

                //son Check pinCount for Net
                if( nBDLData1[nDut][i][5] < 1){
                    str.Format("Error No 1013  :   BDL FILE.  Error line=%d.   PinCount for Net_%d <1.     Reading ERROR  !. \n D:\\AUTO\\BDL\\%s",
                                                                    i ,nBDLData1[nDut][i][2] ,str2);
                    p_list[nDut-1]->AddString(str);
                }

                str.Format("  NO=%7d,     %10d,     %10d,      %10d,  ", 
                            i,  nBDLData1[nDut][i][1]+1, nBDLData1[nDut][i][2], nBDLData1[nDut][i][5] );
                p_list[nDut-1]->AddString(str);
            }

        }

        //son BDL Data NetCount 오류 점검       
        if (Check_BDLNetCountError(nDut) < 0)   //son210929 Check_BDLNetCountError()를 모든 DUT가 되도록 수정.
            return -10;   //son221028


        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //OPEN    DATA 


        ::ZeroMemory(&nDebug, sizeof(nDebug));//debug

        nTmpCo1=0;//SYLEE130222JOYTECH      
        nFirstPin=0; 
        netPinIdx=0; 
        openPinCnt=0;//OPEN 
        shortPinCnt=0;//short


        //son "\SETUP2\PieceRange.ini"를 struct CSysInfoPieceRange에 Load(type=2)
        //  FileSysInfo01.LoadSavePieceRangeInfo(_LOAD);    //SYLEE130312JOYTECH  //piecerange      
        FileSysInfo01.LoadSavePieceRangeInfo(nDut, _LOAD);  //SYLEE150917-8
                                                //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
        //son210430 Piece.ini와 PieceRange.ini 불일치 현상 보완
        if (SysInfoPieceRange.m_nPieceCount[nDut] != SysInfoPiece.m_nPiece)    //son210430 
        {
            str2 = "";
            str.Format("Error No 1020  :  \n\n PieceCount info between PieceRange%d.ini(=%d) and Piece.ini(=%d) is different!\n", 
                                                                nDut, SysInfoPieceRange.m_nPieceCount[nDut], SysInfoPiece.m_nPiece );
            str2 += str;
            str.Format("\n%s BDL PieceCount Error!. Check .\\Setup2\\PieceRange.ini and Piece.ini.\n", SysInfoView01.m_pStrFilePathBDL);
            str2 += str;

            MyTrace(PRT_BASIC, str2);        //son211022
            AfxMessageBox(str2, MB_OK);         

            //son BDL 파일 에러 표시
            gFlag_Start_Int_bdlError = 1;
            return -11; //son221028
        }
    

        //son 1) PieceRange.ini의 정보로 nPieceNoPin1[] 정보 생성
        //       nPieceNoPin1[] : 각 핀의 piece 번호 정보를 저장.  index는 pinNo, value는 pieceNo
        ReadPinPieceNo(nDut);//SYLEE130222JOYTECH

        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        //sort//sylee130224joytech[
        //int nSortNo1[9000][8];//16k

        //---------------------------------------------
        //son piece 1부터 nBDLData1을 재배치한다.

        ::ZeroMemory(&nSortNo1, sizeof(nSortNo1));

        //son g_nPinPieceNo[]에서 BDL 핀의 piece 정보를 획득하여 nBDLData1[i][7] 에 저장.
        for(i=1; i<=nBDLHead1[nDut][3][1]; i++){
            nTmp11=nBDLData1[nDut][i][1];
            nTmp11=g_nPinPieceNo[nDut][nTmp11+1];//sylee150917-7        //son piece
            nBDLData1[nDut][i][7]=nTmp11;//piece set
        }

        //son 보통 "BDL폴더\Setup\PieceRange.ini" 를 확인해 보면 piece 1부터 배치돼 있으므로
        //    Sort한 결과 nSortNo1[netPinIdx]가 원본 nBDLData[pinIdx]과 보통 똑같다.
        netPinIdx=0; 
        for(  i2=1; i2<=nBDLHead1[nDut][3][1]; i2++)
        {        
            nBdlStart=i2;
            nBdlEnd=nBdlStart+nBDLData1[nDut][i2][5]-1;     //son 해당 net의 총핀수 -1
            if(nBdlStart>nBdlEnd){//sylee131105
                //display  str.Format(" **> Error!   BDL%d FILE,   TOTAL POINT CHECK !!! ",nDut);
                //display  m_list3.InsertString(0,str);
                str.Format(" ERROR 1111,  BDL%d  NET LIST LOADING ERROR.      BDL FILE, TOTAL POINT  CHECK!  ",nDut);
                MyTrace(PRT_BASIC, str);        //son211022
                AfxMessageBox(str, MB_OK);          
                break;//sylee131128 continue;//errr?
            }

            //son NetEnd 위치로 pinIdx를 옮겨간다.
            i2=nBdlEnd;
            for(int j2=1; j2<=g_nPieceMax1; j2++)
            {           
                for(int k2=nBdlStart; k2<=nBdlEnd; k2++)
                {
                    if(j2==nBDLData1[nDut][k2][7])  //son  pieceIdx가  Pin에 해당하는 Piece Num 이면
                    {
                        netPinIdx++;
                        for(int m2=1; m2<=7; m2++){
                            nSortNo1[netPinIdx][m2]=nBDLData1[nDut][k2][m2];
                        }
                    }
                }
            }
        }


        //son  piece 1부터 순서대로 정렬된 Sort한 결과를 nSortNo1[pinIdx]를 nBDLData1[nDut][pinIdx]에 덮어쓴다.
        for( i2=1; i2<=nBDLHead1[nDut][3][1]; i2++){
            for(int m2=1; m2<=7; m2++){
                nBDLData1[nDut][i2][m2]=nSortNo1[i2][m2];
            }
        }
        ////////////////////////////////////////////////////////////////
        
        
        //son 옵션이 설정돼 있다면 Net의 pin 갯수대로 정렬한다. (옵션에 따라 내림차순 또는 오름차순)
        //Sort_nBDLData1_byNetPinCount(nDut);        //son200722 추가


        netPinIdx=0; 
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        for(i=1; i<=nBDLHead1[nDut][3][1]; i++)     //son nBDLHead1[nDut][3][1])    ex) 32   (총핀수)
        {
            //son  One point net 이라면 (net pin 수가 1)
            if( nBDLData1[nDut][i][BDL_NET_PINCNT]==1){  //short  if one point
                shortPinCnt++;
                nBDLData3[nDut][shortPinCnt][BDL_PIN0]      =nBDLData1[nDut][i][BDL_PIN0];        //son 대표핀 pinNo                    
                nBDLData3[nDut][shortPinCnt][BDL_NET_PINCNT]=nBDLData1[nDut][i][BDL_NET_PINCNT];  //son netPinCount     //son200722 추가
                nBDLData3[nDut][shortPinCnt][BDL_PIECE]     =nBDLData1[nDut][i][BDL_PIECE];       //son netPinCount     //son200722 추가
                netPinIdx=0;
                continue;
            }

            // netPinCnt가 1보다도 작다면 해당 라인은 의미 없음 
            if( nBDLData1[nDut][i][BDL_NET_PINCNT]<1){              
                netPinIdx=0;
                continue;
            }

            netPinIdx++;              

            //--------------------------------
            //son ShortTest List 생성
            //--------------------------------
            //son Net의 첫번째 pin이라면 short 관련 nBDLData3[1][shortPinCnt]을 채운다.
            if( netPinIdx==1){
                nFirstPin=nBDLData1[nDut][i][BDL_PIN0]; //son 현재 Net의 대표핀
                netPinCnt=nBDLData1[nDut][i][BDL_NET_PINCNT];   //son 현재 Net의 netPinCount
                shortPinCnt++;//short 
                nBDLData3[nDut][shortPinCnt][BDL_PIN0]      =nBDLData1[nDut][i][BDL_PIN0];      //short  //son 대표핀 pinNo 
                nBDLData3[nDut][shortPinCnt][BDL_NET_PINCNT]=nBDLData1[nDut][i][BDL_NET_PINCNT];//short  //son netPinCount  //son200722 추가 
                nBDLData3[nDut][shortPinCnt][BDL_PIECE]     =nBDLData1[nDut][i][BDL_PIECE];     //short  //son pinPiece     //son200722 추가
                //////////////////////////////////////////////////////////////
                //                  ::ZeroMemory(&nBdlPieceSel1, sizeof(nBdlPieceSel1));//SYLEE130222JOYTECH
                if(netPinCnt>1){        
                    nTmp11=g_nPinPieceNo[nDut][nFirstPin+1];//sylee150917-7     //son piece #
                }
            }

            //son count가 Net의 PinCount 갯수 이내라면
            else if(netPinIdx<=netPinCnt)
            {

                //////////////////////////////////////////////////////////////
                nTmpPin1=nBDLData1[nDut][i][1];                         //son pinNo
                nTmp12=g_nPinPieceNo[nDut][nTmpPin1+1];//sylee150917-7  //son 현재pin의 pieceNo

                //son 대표핀의 pieceNo와 현재pin의 pieceNo가 다르다면 예외케이스처리
                if(nTmp11!=nTmp12){
                    netPinIdx =0;//sylee131212
                    i=i-1;//sylee131212     //son pinIdx를 원복하고 for 초기로 돌아감                
                    continue;
                }    

                //--------------------------------
                //son OpenTest List 생성
                //--------------------------------
                //son PinNo가 nFirstPin이 아니면 openTest 대상핀. 
                //    openTest는 대표핀을 기준으로 net 내부의 각 핀을 테스트한다. 
                if(nFirstPin!=nBDLData1[nDut][i][1])//sylee130515
                {
                    openPinCnt++;                       
                    nBDLData2[nDut][openPinCnt][1]=nFirstPin;               //son 대표핀
                    nBDLData2[nDut][openPinCnt][2]=nBDLData1[nDut][i][1];   //son openTest대상  PinNo
                    nBDLData2[nDut][openPinCnt][3]=nBDLData1[nDut][i][5];   //PIECE OPEN    //son Net Pin Count
                    nBDLData2[nDut][openPinCnt][4]=nBDLData1[nDut][i][6];   //PIECE         //son PieceNo?? 1고정수?? 
                } //sylee130515-test

                //son 해당 net의 마지막핀이라면
                if(netPinIdx==netPinCnt){
                    netPinIdx=0;
                }                   

            }

        }

        if(i>0){
            if( (i-1)==nBDLHead1[nDut][3][1] ){
                nBDLData2[nDut][0][1]=openPinCnt;  // OPEN BDL 총 갯수  //SYLEE20111212
            }
        }

        //son200420-4WNET10000 shortPinCnt 설정 위치를 현재 위치로 이동 
        //son221103 if (shortPinCnt > 1) //short        
        if (shortPinCnt >= 1){ //short        //son221103 shortPinCnt 1 일때 nBDLData3[1][0][1] 에 write안 되는 문제 수정.
            nBDLData3[nDut][0][1]=shortPinCnt;   // short  BDL 총 갯수  //SYLEE20111212
        } 

        //----------------------------------
        //son Piece별 ShortTest List 생성
        
        //son 1) PieceRange.ini의 정보로 g_nPinPieceNo[] 정보 생성
        //    g_nPinPieceNo[] : 각 핀의 piece 번호 정보를 저장.  index는 pinNo, value는 pieceNo
        ReadPinPieceNo(nDut);//SYLEE121202

        //son 2) piece정보를 이용해서 nBDLData3[nDUT]를  g_pvBDLData31[nDUT][piece] 에 piece별로 재배치한다. 
        //       InsertShortTest() 수행
        makePieceShortTestData(nDut);       //son221103_2

        //son piece를 1개일 때의 통검사 ShortTest Data를 NetCount 기준으로 정렬한다.
        Sort_nBDLData3_byNetPinCount(nDut); //son200722-BDL-Sort

        //son piece별 ShortTest Data를 NetCount 기준으로 정렬한다.
        Sort_nBDLData31_byNetPinCount(nDut);  //son200722-BDL-Sort


        //--------------------------------
        //son Piece별 OpenTest List 생성
        makePieceOpenTestData(nDut);       //son for nDUT   //son200420-4WNET10000



        //--------------------------------------------------------------------------------------
        //son Debug Print: Piece별 OpenTest List, ShortTest List 가 제대로 생성되었는지 확인 

        g_nPieceMax1=SysInfoPieceRange.m_nPieceCount[nDut]; //SYLEE130107 
        if( g_nPieceMax1<1){//sylee130228
            str.Format("  DUT%d  , OnDataLoad()    \n    g_nPieceMax1 <0  \n\n ? ",nDut );
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK );
            // AfxMessageBox("  OnDataLoad()    \n    g_nPieceMax1 <0  \n\n ?  ", MB_OK );
        }

        //son 생성한 Dut, piece별 COpenTestItem g_pvBDLData2Op[dut][piece]를  "D:\\log\\0OpenPinListPiece_DUT%d" 에 출력한다. (확인용)
        //      int nIPOpenTotal;
        nIPOpenTotal=0;  
        nIPOpenTotal = printPieceOpenTestData(nDut);    //son200420-4WNET10000

        //son 생성한 Dut, piece별  CShortTestItem g_pvBDLData31[dut][piece]를 "D:\\log\\0ShortPinListPiece_DUT%d" 에 출력한다. (for debug)
        printPieceShortTestData(nDut);   //son200420-4WNET10000
        LeakMultPinList1(nDut);    //sylee230526


        //son 생성한 piece별 open nBDLData2Op[][][]를 원래의 nBDLData2[][][]의 카운트와 비교
        if(nBDLData2[nDut][0][1]!=nIPOpenTotal){
            str.Format(" ERROR NO 4001,\n BDL OPEN LIST LINK ERROR.  BDL RELOADIG!  ,\n CHECK BDL FILE or PIECE RANGERANGE SET  or FILE !   \n\n  DON'T  RUN \n\n  list1=%d,list2=%d \n\n ", nBDLData2[nDut][0][1],nIPOpenTotal);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);
        }


        //------------------------------------------------------------------
        //son m_list에 로딩한 BDL 리스트와 Open List의 정보를 출력한다.
        
        //display 
        if (nDut <= 4)  //son display용 m_list는 4개뿐이므로
        {
        
           str.Format("          "); 
           p_list[nDut-1]->InsertString(0,str);  //SYLEE131213
           str.Format(" ***>BDL LIST      = %d      ", nBDLHead1[nDut][3][1] ); 
           p_list[nDut-1]->InsertString(0,str);  //SYLEE131213
           nBDLData1[nDut][0][2]=nBDLHead1[nDut][3][1]; 
           str.Format(" ***>OPEN LIST   = %d    ", nBDLData2[nDut][0][1] ); 
           p_list[nDut-1]->InsertString(0,str);  //SYLEE131213
           
        }


#if 0   //son221103_3  printPieceOpenTestData() 와 내용 중복되는 파일 출력이므로 삭제.
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //FILE OPEN FILE SAVE  DATE

        str.Format("OPENSETUPLIST_DUT%d",nDut); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format("Error No 1014 :   .\\BDL\\ OPENSETUPLIST%d   file Data Saving Error !     file close!.",nDut);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);          
            return -12; //son221028
        }

        curTime = CTime::GetCurrentTime();   

        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        //fprintf(fp, "* SETUP  : OPEN NET3 LIST  FILE \n");  
        str.Format("* SETUP  : OPEN NET%d LIST  FILE \n",nDut);
        fprintf(fp, str);

        fprintf(fp, " \n"); 
        fprintf(fp," ***>OPEN LIST Count =%d    Read Complete", nBDLData2[nDut][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"  PIN, PIN, \n"  );

        for(i=1; i<=nBDLData2[nDut][0][1]; i++){    
            fprintf(fp, "    %d,   %d\n",nBDLData2[nDut][i][1]+1, nBDLData2[nDut][i][2]+1);  //set path file  //sylee150917-5
        }

        fclose(fp);  
#endif

#if 0   //son221103_3  printPieceShortTestData() 와 내용 중복되는 파일 출력이므로 삭제.

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //SHORT   DATA 
        str.Format("SHORTSETUPLIST_DUT%d",nDut); 
        ::ZeroMemory(&fName, sizeof(fName));
        strcat( fName , str );
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format("Error No 1015  : .\\BDL\\ SHORTSETUPLIST%d   file Data Saving Error !     file close!.",nDut);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);          
            return -13; //son221028
        }

        curTime = CTime::GetCurrentTime();

        fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",
                        curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  
                        curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
        fprintf(fp, "* SETUP  : SHORT LIST  FILE \n");  
        fprintf(fp, " \n"); 
        fprintf(fp," ***>SHORT LIST Count =%d    Read Complete", nBDLData3[nDut][0][1] );
        fprintf(fp, " \n");
        fprintf(fp, " \n"); 
        fprintf(fp,"      PIN,   NetPinCount,   Piece \n"  );

        for(i=1; i<=nBDLData3[nDut][0][1]; i++){    
            fprintf(fp, "    %5d,        %4d,    %2d\n", nBDLData3[nDut][i][1]+1,   //set path file  //sylee150917-5
                                                      nBDLData3[nDut][i][BDL_NET_PINCNT],
                                                      nBDLData3[nDut][i][BDL_PIECE]);
                                                     
        }
        fclose(fp);  
#endif

        if (nDut <= 4)          //son200722-BDL-Sort
        {
            //int temp3;//sylee150909 2N1+
            //temp3=nBDLData3[nDut][0][1];//sylee150909 2N1+ 
            str.Format(" ***>SHORT LIST = %d    ", nBDLData3[nDut][0][1] );      
            p_list[nDut-1]->InsertString(0,str);  //SYLEE131213
        }

        if (nDut == 1)
        {

            OnDataLoad_READ_ConRead(); // sylee170525-1

            FileSysInfo01.LoadSaveSet13(2);//load //sylee170607

            int a;//sylee130518

            //son PinMap 로딩
            //son "C:\\ACE400\\SETUP\\SysSetPin.ini"를 struct CSysSet12에 Load(type=2)
            a=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
            if(a != 1)
            {
                str.Format(" Error No 1112, \n    SysSet12.ini  Load ERROR !  \n    file Read Error !!!");
                MyTrace(PRT_BASIC, str);        //son211022
                AfxMessageBox(str, MB_OK);    
            }

        }


        //sylee130518
        ::ZeroMemory(PinSearchPin, sizeof(PinSearchPin)); //sylee130518-1 pin search bdl

        for(i=1; i<=nBDLHead1[nDut][3][1]; i++)
        {
            wJigPin=nBDLData1[nDut][i][1];//sylee130518-1 pin search bdl    //son BDL pin 번호(jigPin. 0부터시작)
            wHwPin=PinMap[2][wJigPin];

            //son-32KPIN-20200113
            // 20K인데 32K 영역의 BDL을 로딩한다면 없는 B,C, D 박스의 PinMap[2][pin]는 -1로 리턴됨. 
            // 이 경우는 계측기 SW 오동작할 수 있으므로 미리 경고해 주기 위해 nErrCount 추가함.
            // nErrCount가 1이상이면 ModeSet12.make32kHwPin()에서  -1을 리턴해서 오류가 있음을 표시해 준다.

            // wHwPin값이 마이너스일 때, PinSearchPin[wHwPin] 접근하는 것을 방지한다.
            if (wHwPin < 0)
            {
                str.Format(" Error No 1113, \n Can't find hwPin for userPin_%d! \n\n Wrong BDL File loaded which does not match LowerStartPin(%s_%d), Hw MaxPin(%s)!\n",  
                        wJigPin+1, STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                        SysSet12.m_nLowerJigStartPin, STR_MAX_PIN(SysSet12.m_nPinTotMax) );
                MyTrace(PRT_BASIC, str);        //son211022
                AfxMessageBox(str, MB_OK);    

                //son BDL 파일 에러 표시
                gFlag_Start_Int_bdlError = 1;
                return -14; //son221028
            }
            PinSearchPin[wHwPin]=1;//sylee130518-1 pin search bdl
        }

        pinCnt       = nBDLHead1[nDut][3][1];
        openPinCnt2  = nBDLData2[nDut][0][1];
        shortPinCnt2 = nBDLData3[nDut][0][1];//sylee131213
        totCnt = openPinCnt2 + shortPinCnt2;

        //display       //son200722-BDL-Sort
        if (nDut <= 4)
        {
           str.Format("     " );
           p_list[nDut-1]->InsertString(0,str);  
        }
        if( pinCnt!=totCnt)
        {
            str.Format("  \n\n Error No 1001,  BDL Loading Fail.   \n\n\n\n    DON'T  RUN! .   Manager Call!   \n\n\n\n\n  DUT_%d BDL TotalPin = SUM (Open Pin + Short Pin) ==>\n\n\n  ( %d  = %d(%d+%d)?  ) \n\n\n\n ", nDut, pinCnt, totCnt, openPinCnt2, shortPinCnt2);
            MyTrace(PRT_BASIC, str);        //son211022
            AfxMessageBox(str, MB_OK);          

            //son BDL 파일 에러 표시
            gFlag_Start_Int_bdlError = 1;   //son221103_4

            //display
            if (nDut <= 4)
            {
                str.Format(" * Total Pin %d != %d+%d => ERROR !  Don't Run!  " ,  pinCnt, shortPinCnt2, openPinCnt2 );
                p_list[nDut-1]->InsertString(0,str);  //SYL
                str.Format("     " );
                p_list[nDut-1]->InsertString(0,str);   

                str.Format("  Don't Run!  ==>   FAIL!  " );
                p_list[nDut-1]->InsertString(0,str);  //SYLEE140324 INTERLOCK
                str.Format("     " );
                p_list[nDut-1]->InsertString(0,str);  

                p_list[nDut-1]->AddString(str);
                str.Format("    End  ERROR !.    " );
                p_list[nDut-1]->AddString(str); 
            }
        }
        else
        {
            //son221104 begin: shortPinCount 1개 일때 Recipe에 short Test 있으면 warning 출력 
            if (shortPinCnt2 <= 1 && FileSysInfo01.checkRecipeStep_ShortExist() == true)  
            {
                str.Format("\n Warning No 1036, DUT_%d Short Test Pin Count is only \'1\'.\n\n ShortTest, LeakTest included in Recipe cannot be performed\n", nDut);  
                MyTrace(PRT_BASIC, str);     
                AfxMessageBox(str, MB_OK);          

            } //son221104 end

            if (nDut <= 4)
            {
                //  str.Format("    OK !!!     Total Pin  %d=%d+%d " ,   pinCnt, shortPinCnt2, openPinCnt2 );

                str.Format("    OK .   "   );
                p_list[nDut-1]->InsertString(0,str);  //SYL
                str.Format("             " );
                p_list[nDut-1]->InsertString(0,str);  //SYL
                p_list[nDut-1]->AddString(str);
                str.Format("    End OK .    "  );
                p_list[nDut-1]->AddString(str);
            }

        }

        //son  여기서 BDL Data를 다시 nPinDa12w[][]에 read하는 이유가 무엇???
        ReadFileB(nDut);  //jig read set  //sylee150817-2

        nMultyNetTotal=nDut;//sylee150817
        str.Format("%d", nMultyNetTotal); 
        //display   
        m_LabelDut.SetCaption(str);
    }//     for(int nDut=1;  nDut<=MAX_DUT ; nDut++){      //son220824_2
 
    //############################################################################



    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    return 1;
}

int CChildView1::GetBDLFileName_FileDialog(CString &rStrFile)   
{
    //----------------------------------------------------------
    //son FileDialog 를 Open (BDL 파일 이름을 입력받아서 Open)
    //----------------------------------------------------------
    
    //son200522-4WNET10000:
    //    BDL 파일 로딩시에 BDL2를 선택할 위험때문에  파일리스트에 .BDL만 보이도록 수정.
    char szFilter[] = "Image (*.BDL)|*.BDL|All Files(*.*)|*.*||";
    CFileDialog dlg(TRUE,       // bOpenFileDialg = TRUE: 타이틀바에 '열기' 출력
                  NULL,         // lpszDefExt           : 기본 확장자
                  NULL,         // lpszFileName         : "파일 이름" Edit 컨트롤에 처음 출력될 파일명.
                  OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, 
                                // dwFlags :  속성   (HIDEREADONLY : read only 파일은 출력하지 않음.
                  szFilter);    // 대화상자에 출력될 파일을 확장자로 거르기 위한 필터. 


 
    if(dlg.DoModal()!=IDOK)
    { 
        return -1;
    }//end of  if(dlg.DoModal()==IDOK)


    rStrFile.Format("%s", dlg.GetFileName()); 

    return 0;
}


//son BDL Data NetCount 오류 점검       //son210831
//
//주의: 아래와 같이 모든 Net의 번호가 0으로 같다면  Check_BDLNetCountError()를 무사히 통과할 수 있다.
//      if (nextNet != currNet)   코드에 걸리지를 않고 그냥 pass함.  
//      Net번호를 아예 안쓰는 경우라고 생각함. 이상하긴 하지만 이런 경우는 이대로 동작하도록 둔다.  
//Pin      Net       x     piece   NetPinCnt  x
//243       0        0       1       1        1
//494       0        0       1       1        1
//495       0        0       1       1        1
//496       0        0       1       1        1
//507       0        0       1       1        1
int CChildView1::Check_BDLNetCountError(int nDut)       //son210929  nDut parameter 추가
{
    int i; 
    int netPinCnt2 = 0;
    int currNet, nextNet;
    CString str, strTemp;
    for(i=1; i<=nBDLHead1[nDut][3][1]; i++)    //son210929 nBDLHead1[1] -> nBDLHead1[nDut]
    {
        netPinCnt2++;
        
        currNet = nBDLData1[nDut][i][2];        //son210929 nBDLData1[1] -> nBDLData1[nDut]
        nextNet = nBDLData1[nDut][i+1][2];      //son210929
        if (nextNet != currNet) //son  현재 pin이 같은Net의  마지막 Pin이라면 카운트값을 비교
        {
            //son 카운트한 현재 Net의 pin 개수와 BDL에 기록된 netPinCount가 불일치한다면 에러를 출력한다.
            if ( nBDLData1[nDut][i][5] != netPinCnt2)
            {
                
                str.Format("Error No 1031 :  *.bdl (DUT_%d)  loading fail. Invalid NetCount! \n\n\n", nDut);    //son210929
                strTemp.Format("No=%d, pin=%d, net=%d, NetPinCnt=%d *****\n\n", 
                                   i, nBDLData1[nDut][i][1], nBDLData1[nDut][i][2], nBDLData1[nDut][i][5] );    //son210929 nBDLData1[1] -> nBDLData1[nDut]
                str += strTemp;

                strTemp.Format("The counted netPinCnt2(%d) does not match",  netPinCnt2); 
                m_list1.InsertString(0, strTemp);
                str += strTemp;
                str += "\n";

                strTemp.Format("BDL(DUT%d) NetPinCnt(%d)!", nDut, nBDLData1[nDut][i][5]); //son210929 nBDLData1[1] -> nBDLData1[nDut]
                m_list1.InsertString(1, strTemp);
                str += strTemp;
                str += "\n";

                strTemp.Format("Check pinLine No=%d ~ No=%d", (i-netPinCnt2+1), i );
                m_list1.InsertString(2, strTemp);
                str += strTemp;
                str += "\n";

                strTemp.Format(" ");
                m_list1.InsertString(3, strTemp);

                MyTrace(PRT_BASIC, str);
                AfxMessageBox( str );  


                //son BDL 파일 에러 표시
                gFlag_Start_Int_bdlError = 1;
                return -1;
            }
            netPinCnt2 = 0;
        }

    }   //son210831

    return 0;
}


//son 대만 MST 전용기능
//   현재 BDL과 동일한 BDL을 다른 이름으로 생성하는 기능. 
//   LOT 별로 동일한 BDL을 여러개 copy해서 운용하고 싶어하고 있으므로 SaveAs 기능을 추가해 줌.
void CChildView1::OnButton_SaveAsNewBdl()  //son220228
{
#ifdef __MST__CUSTOMER
    CString str, strbuf;

    //AfxMessageBox("Input New BDL Folder Name", MB_OK);          
    if (nMultyNetTotal == 0)
    {
        str.Format("Please load BDL file first." );  
        AfxMessageBox(str, MB_OK);
        return;
    }

    //son 다이얼로그에서 새로운 BDL Name을 입력받는다.
    CSaveAsNewBdl Dlg;  //son220228
    Dlg.DoModal();      



    //---------------------
    // Make New Folder
    //---------------------
    CString strNewBdlPath, strBdlName; 

    // SysInfoView01.m_pStrFilePathBDL 폴더와 같은 레벨에 새로운 이름의 폴더를 생성한다.  
    strBdlName.Format("%s",SysInfoView01.m_pStrFilePathBDL);    //ex) "C:\BDL\10VGAG011J"  폴더명
    int loc= strBdlName.ReverseFind('\\');   
    strNewBdlPath= strBdlName.Left(loc+1);
    strNewBdlPath= strNewBdlPath + Dlg.m_strNewBdlName;     //ex) "C:\BDL\10VGAG011J_Lot1" LotName을 포함한 새로운 폴더명 


    //son 새로운 이름의 폴더를 생성한다.
    char buf[FILE_PATH_SIZE];
    sprintf(buf,"%s", strNewBdlPath);
    if(!FileExists(buf))    
    {
        if(!MakeNewDir(strNewBdlPath)) 
        {
            strbuf.Format("%s creat Error!!", strNewBdlPath);
            AfxMessageBox(strbuf);
            return ;    
        }
    } 

    //son 새로운 폴더 밑에 Setup2 폴더를  생성한다.
    CString strNewBdlSetup2;
    strNewBdlSetup2.Format("%s\\Setup2", strNewBdlPath);
    sprintf(buf,"%s", strNewBdlSetup2);
    if(!FileExists(buf))    
    {
        if(!MakeNewDir(strNewBdlSetup2)) 
        {
            strbuf.Format("%s creat Error!!", strNewBdlSetup2);
            AfxMessageBox(strbuf);
            return ;    
        }
    } 

    //---------------------
    // SaveAs xxx.BDL 
    //---------------------
    int dut; 
    CString numStr;
    char destfile[FILE_PATH_SIZE], srcfile[FILE_PATH_SIZE];
    for (dut= 1; dut<= nMultyNetTotal; dut++) 
    {
        ::ZeroMemory(&srcfile, sizeof(srcfile));    
        str.Format("%s\\%s", SysInfoView01.m_pStrFilePathBDL, SysInfoView01.m_pStrFileBDLName); 
        strcat(srcfile , str);     // Source file Name ex) "C:\BDL\10VGAG011J\10VGAG011J.BDL" 
        if (dut>= 2) {
            numStr.Format("%d",dut);
            strcat(srcfile, numStr);
        }

        if(!FileExists(srcfile))    
            continue;

        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\%s.BDL", strNewBdlPath, Dlg.m_strNewBdlName);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\10VGAG011J_Lot1.BDL" 
        if (dut>= 2) 
            strcat(destfile, numStr);
        
        CopyFile(srcfile, destfile, FALSE);
    }


    //---------------------
    // SaveAs xxx.CON 
    //---------------------

    //son xxx.CON이름을 xxx.BDL에서 유추한다.
    CString strOrgBdlName; 
    str.Format("%s", SysInfoView01.m_pStrFileBDLName);
    loc= str.ReverseFind('.'); // yskim240812
    strOrgBdlName = str.Left(loc);

    for (dut= 1; dut<= nMultyNetTotal; dut++) 
    {
        ::ZeroMemory(&srcfile, sizeof(srcfile));    
        str.Format("%s\\%s.CON", SysInfoView01.m_pStrFilePathBDL, strOrgBdlName); 
        strcat(srcfile, str);     // Source file Name ex) "C:\BDL\10VGAG011J\10VGAG011J.CON" 
        if (dut>= 2) {
            numStr.Format("%d",dut);
            strcat(srcfile, numStr);
        }

        if(!FileExists(srcfile))    
            continue;

        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\%s.CON", strNewBdlPath, Dlg.m_strNewBdlName);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\10VGAG011J_Lot1.CON" 
        if (dut>= 2) 
            strcat(destfile, numStr);

        CopyFile(srcfile, destfile, FALSE);
    }

    //---------------------
    // Copy 4w.ini 
    //---------------------
    for (dut= 1; dut<= nMultyNetTotal; dut++) 
    {
        ::ZeroMemory(&srcfile, sizeof(srcfile));    
        str.Format("%s\\4W.ini", SysInfoView01.m_pStrFilePathBDL); 
        strcat(srcfile, str);     // Source file Name ex) "C:\BDL\10VGAG011J\4W.ini" 
        if (dut>= 2) {
            numStr.Format("%d",dut);
            strcat(srcfile, numStr);
        }

        if(!FileExists(srcfile))    
            continue;

        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\4W.ini", strNewBdlPath);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\4W.ini" 
        if (dut>= 2) 
            strcat(destfile, numStr);

        CopyFile(srcfile, destfile, FALSE);

    }

    //-------------------------------------------------------
    // Copy 4w_Setup_A.txt, 4W_Per_Low.ini, 4W_Per_High.ini
    //-------------------------------------------------------
    
    //son220310  4W_Setup_A.txt2~n 도 복사되도록 수정
    for (dut= 1; dut<= nMultyNetTotal; dut++) 
    {
        ::ZeroMemory(&srcfile, sizeof(srcfile));    
        str.Format("%s\\4w_Setup_A.txt", SysInfoView01.m_pStrFilePathBDL); 
        strcat(srcfile, str);     // Source file Name ex) "C:\BDL\10VGAG011J\4W_Setup_A.txt" 
        if (dut>= 2) {
            numStr.Format("%d",dut);
            strcat(srcfile, numStr);
        }

        if(!FileExists(srcfile))    
            continue;

        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\4w_Setup_A.txt", strNewBdlPath);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\4W_Setup_A.txt" 
        if (dut>= 2) 
            strcat(destfile, numStr);

        CopyFile(srcfile, destfile, FALSE);
    }
    
    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s\\4W_Per_Low.ini", SysInfoView01.m_pStrFilePathBDL); //son220310  추가  
    strcat(srcfile, str);     // Source file Name ex) "C:\BDL\10VGAG011J\4W_Per_Low.ini" 
    if(FileExists(srcfile))    
    {
        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\4W_Per_Low.ini", strNewBdlPath);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\4W_Per_Low.ini" 

        CopyFile(srcfile, destfile, FALSE);
    }

    ::ZeroMemory(&srcfile, sizeof(srcfile));    
    str.Format("%s\\4W_Per_High.ini", SysInfoView01.m_pStrFilePathBDL); //son220310  추가  
    strcat(srcfile, str);     // Source file Name ex) "C:\BDL\10VGAG011J\4W_Per_High.ini" 
    if(FileExists(srcfile))    
    {
        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\4W_Per_High.ini", strNewBdlPath);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\4W_Per_High.ini" 

        CopyFile(srcfile, destfile, FALSE);
    }


    //-------------------------
    // Copy SETUP Full Folder  	
    //-------------------------

    CFileFind   finder;

    // build a string with wildcards
    CString     strWildcard;
    strWildcard.Format("%s\\Setup2\\*.*", SysInfoView01.m_pStrFilePathBDL);

    // start working for files
    BOOL bWorking = finder.FindFile(strWildcard);

    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        // skip . and .. files; otherwise, we'd
        // recur infinitely!
        if (finder.IsDots())
            continue;

        if (finder.IsDirectory())
            continue;

        CString fileName = finder.GetFileName();

        ::ZeroMemory(&srcfile, sizeof(srcfile));    
        str.Format("%s\\Setup2\\%s", SysInfoView01.m_pStrFilePathBDL, fileName); 
        strcat(srcfile, str);     // Source file Name ex) "C:\BDL\10VGAG011J\Setup2\Block.ini" 등 Setup2 밑의 어떤 파일

        ::ZeroMemory(&destfile, sizeof(destfile));    
        str.Format("%s\\Setup2\\%s", strNewBdlPath, fileName);
        strcat(destfile, str);    // Dest file Name ex) "C:\BDL\10VGAG011J_Lot1\Setup2\Block.ini" 

        CopyFile(srcfile, destfile, FALSE);

    }

#endif
}


