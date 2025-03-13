// ModeSer15.cpp : implementation file
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stdafx.h" 
#include "ModeSer15.h"
#include "ModeSUB71.h" 
#include "FileSysInfo.h"
#include "Contec.h" 
#include "Globals.h"        //son211230

extern CSysSet12  SysSet12; //son220315

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFileSysInfo FileSysInfo01;   
extern CModeSub71  Sub71; 
extern CSysSet13  SysSet13;

extern void DoEvents();  
extern __int64 GetMilSecond2();  
 
//extern CSysInfoView01  SysInfoView01;
// extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
//extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA 

//int nListMax; //son211230 전역변수 nListMax -> 멤버변수 m_nListMax 
//int nTMode1;  //son211230 전역변수 nTMode1  -> 멤버변수 m_nTestType
//int nMaxCol;  //son211230 전역변수 nMaxCol  -> 멤버변수 m_nMeasColMax

//int nPin1R[6][101][6]   //son 테스트 타입별  RList 용 pin, Ref 정보
int g_nPin1R[R_TEST_NUM][MAX_R_LIST +1][R_COL_INFO_NUM];//OPEN //uShort //HR //Leak      //son211230 6 -> R_TEST_NUM
        //son g_nPin1R[testType][0][0] : RListCount
        //son g_nPin1R[testType][RList][1:R_COL_PIN1] : Pin1
        //son g_nPin1R[testType][RList][2:R_COL_PIN2] : Pin2
        //son g_nPin1R[testType][RList][3:R_COL_REF_R] : Ref.R
        

enum R_LOC {
    R_LOC_AVG       = 0,
    R_LOC_MAX       = MAX_R_MEAS +1,      //son 301     //son220103 11 -> R_LOC_MAX,
    R_LOC_MIN       = MAX_R_MEAS +2,      //son 302     //son220103 12 -> R_LOC_MIN
    R_LOC_DIFF      = MAX_R_MEAS +3,      //son 303     //son220103 13 -> R_LOC_DIFF
    R_LOC_DIFF_RATE = MAX_R_MEAS +4       //son 304     //son220103 14 -> R_LOC_DIFF_RATE
};        
double  dR11[MAX_R_MEAS +10][MAX_R_LIST +10];        //son211230  300: MAX_R_MEAS
                //son dR11[R_LOC_AVG][RList]           : Avg
                //son dR11[1 ~300][RList]              : R Value
                //son dR11[301:R_LOC_MAX][RList]       : max
                //son dR11[302:R_LOC_MIN][RList]       : min
                //son dR11[303:R_LOC_DIFF][RList]      : diff
                //son dR11[304:R_LOC_DIFF_RATE][RList] : diffRate
                
double  dVB11[MAX_R_MEAS +1][MAX_R_LIST +1];       //son211230
double  dVC11[MAX_R_MEAS +1][MAX_R_LIST +1];       //son211230
double  dI11[MAX_R_MEAS +1][MAX_R_LIST +1];        //son211230

//son231012_2 extern int nIADC1;      //son ModeSub71.cpp external
extern double dIADC1; //son231012_2 int nIADC1  -> double dIADC1으로 수정. 소수점이하 잘리는 문제보완.

extern int nMode4W2;    //son ModeSub71.cpp external

extern double V1,Y1,X1,W1;  //4WIRE

extern void StringComa1(CString &str1);
  

CModeSer15::CModeSer15(CWnd* pParent /*=NULL*/)
    : CDialog(CModeSer15::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeSer15)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


//son220315 IDC_CHECK_LowerPin 추가: 하부Pin R 측정 기능  
void CModeSer15::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeSer15)
    	DDX_Control(pDX, IDC_CHECK_LowerPin, m_checkLowerPin);      
        DDX_Control(pDX, IDC_COMBO01, m_combo1);
        DDX_Control(pDX, IDC_COMBO02, m_combo2); 
        DDX_Control(pDX, IDC_GRID_1,  m_grid1); 
        DDX_Control(pDX, IDC_EDIT1,   m_edit1);
        DDX_Control(pDX, IDC_EDIT2,   m_edit2);     
        DDX_Control(pDX, IDC_LABEL103,  m_Label103);
	//}}AFX_DATA_MAP
}

//son220315 OnCHECKLowerPin() 추가. 하부Pin R 측정기능
BEGIN_MESSAGE_MAP(CModeSer15, CDialog)
    //{{AFX_MSG_MAP(CModeSer15)
        ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)
        ON_CBN_SELCHANGE(IDC_COMBO02, OnSelchangeCombo2) 
        ON_WM_MOUSEWHEEL()
        ON_WM_VSCROLL()
        ON_WM_HSCROLL()
    	ON_BN_CLICKED(IDC_CHECK_LowerPin, OnCHECKLowerPin) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSer15 message handlers


BEGIN_EVENTSINK_MAP(CModeSer15, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSer15)
    ON_EVENT(CModeSer15, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//  ON_EVENT(CModeSer15, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_BUTTON101,-600 /* Click */, On_R_Measure1, VTS_NONE)
       
    ON_EVENT(CModeSer15, IDC_BUTTON102,-600 /* Click */, On_R_Measure2, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_BUTTON103,-600 /* Click */, OnButton103, VTS_NONE)
//  ON_EVENT(CModeSer15, IDC_BUTTON104,-600 /* Click */, OnButton104, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_BUTTON105,-600 /* Click */, OnButton_ViewFile, VTS_NONE)
    ON_EVENT(CModeSer15, IDC_BUTTON106,-600 /* Click */, OnButton_Stop, VTS_NONE)


    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 
BOOL CModeSer15::PreTranslateMessage(MSG* pMsg) 
{
// TODO: Add your specialized code here and/or call the base class  
/*
    if ((pMsg->message == WM_KEYDOWN) 
        && (pMsg->wParam == VK_ESCAPE)) {
        return TRUE;
    }
*/
    if ((pMsg->message == WM_VSCROLL) ) { 
        return TRUE;
    }
    if ((pMsg->message == WM_KEYDOWN) 
        && (pMsg->wParam == VK_RETURN)) { 
        return TRUE;
    }
    if (  pMsg->message == VK_SCROLL){  
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

//son220412_4  측정 데이터를 Grid에서 초기화하는 함수 
void CModeSer15::InitGrid_MeasData()  
{ 
    int list, col;
    for( list=1; list<=m_nListMax; list++ ){
        for(col=R_COL_AVG; col<=R_COL_NUM; col++){
            str.Format("          ");
            m_grid1.SetTextMatrix( list, col, str);
        }  
    }
}

//son 'Initial' 버튼 클릭
void CModeSer15::OnButton103()  
{ 
    CString str;

    //son 측정 데이터를 Grid에서 초기화.
    InitGrid_MeasData();    //son220412_4
                    
    m_nStep1=1;   
    str.Format("%d", m_nStep1-1);        
    m_Label103.SetCaption(str);            
}

//son 'Exit' 버튼 클릭 
void CModeSer15::OnClickOk() 
{ 
    k=0;

    //son220315 하부핀 R 측정상태 save       
    FileSysInfo01.LoadSave_UseLowerPin(_SAVE, m_bUseLowerCalJig);    //son220331 Cal기능과 같이 쓰기 위해 FileSysInfo의 전역함수로 변경

    CDialog::OnOK();
}


BOOL CModeSer15::OnInitDialog() 
{
    CDialog::OnInitDialog();

    int i;  
 
    m_nMeasColMax= MAX_R_COL_MEAS; //son211230 10 -> MAX_R_COL_MEAS
    m_nListMax= MAX_R_LIST;     //son211230

    //------------------------------
    //son Grid 생성
    m_grid1.Clear();    
    m_grid1.SetRows(m_nListMax+1);
    m_grid1.SetCols(m_nMeasColMax+ R_COL_INFO_NUM +1);//sylee20120114   //son211230 7 -> R_COL_INFO_NUM +1 
    m_grid1.SetCol(0); 
    m_grid1.SetRow(0); 
 
    for ( i=1 ; i<=m_nListMax; i++) 
    { 
        m_grid1.SetCol(0); 
        m_grid1.SetRow(i); 
        str.Format("No.%d", i); 
        m_grid1.SetText(str); 
    }  
 
    COLORREF clr1,clr2,clr3,clr4,clr5,clr6,clr7,clr8;
 
    clr1 = RGB(200,0,20);//red
    clr2 = RGB(255,200,228);//white red 
    clr3 = RGB(225,255,164);//green
    clr4 = RGB(9,73,172);//blue
    clr5 = RGB(0,10,50);//black
    clr6 = RGB(252,254,211);//white  yellow
    clr7 = RGB(0,0,0);//black
    clr8 = RGB(225,200,184);//green

    m_grid1.SetForeColor(clr7); 
    m_grid1.SetBackColor(clr6); 
    m_grid1.SetColColor(1, clr1 , clr3);
    m_grid1.SetColColor(2, clr1 , clr3); 
    m_grid1.SetColColor(3, clr4 , clr2); 
    m_grid1.SetColColor(4, clr5 , clr3);//sylee 
    m_grid1.SetColColor(5, clr1 , clr2);//sylee
    m_grid1.SetColColor(6, clr1 , clr8);//sylee

    //------------------------------
    //son Grid 헤더 출력 
    str.Format("Pin1");     m_grid1.SetTextMatrix(0, R_COL_PIN1, str);              //son211230 1 -> R_COL_PIN1 
    str.Format("Pin2");     m_grid1.SetTextMatrix(0, R_COL_PIN2, str);              //son211230 2 -> R_COL_PIN2 
    str.Format("Ref. R");   m_grid1.SetTextMatrix(0, R_COL_REF_R, str);             //son211230 3 -> R_COL_REF_R
 
    str.Format("Test AVG.");    m_grid1.SetTextMatrix(0, R_COL_AVG, str);           //son211230 4 -> R_COL_AVG 
    str.Format("RefR-Avg Rate.");   m_grid1.SetTextMatrix(0, R_COL_REF_AVG_RATE, str); //son211230 5 -> R_COL_REF_AVG_RATE
    str.Format("Max-Min Rate");     m_grid1.SetTextMatrix(0, R_COL_DIFF_RATE, str); //son211230 6 -> R_COL_DIFF_RATE
    
    //for (  i = 7; i <= (m_nMeasColMax+ 6) ; i++)     
    //{  
    //    str.Format("NO. %d", i- 6); 
    //    m_grid1.SetTextMatrix(0, i, str);
    //}  
    for (i = 0; i < m_nMeasColMax; i++)     //son211230 
    {  
        str.Format("NO. %d", i +1); 
        m_grid1.SetTextMatrix(0, i+ R_COL_NO1, str);
    } //son211230 
 
    m_grid1.Refresh();
    m_grid1.SetRedraw(TRUE);


    m_nCurRow = 1;  m_nCurCol = 1;

    m_grid1.GetWindowRect(m_rectGrid);
    ScreenToClient(m_rectGrid);

    m_dRatio = double(m_rectGrid.Width())/ 16800; 
    OnEditing = FALSE;

    //son211230    ::ZeroMemory(&nData, sizeof(nData));  //son nData 미사용이므로 삭제

    //----------------------------------------------
    //son nMode 콤보박스(m_combo1) data 초기화      
    for (i=0; i < E_MODE_NUM; i++) {        
        m_combo1.InsertString(-1, m_VModeList.saList[i].str );      //son211230
    }
    m_combo1.SetCurSel(E_OPEN_10V);         //son211230
    //m_nMode1= 6;      //son211230 
    //m_nTestType= 1;   //son211230
    m_nMode1=  m_VModeList.saList[E_OPEN_10V].nMode;     //son211230
    m_nTestType= getRTestType(m_nMode1);   //son211230 mode에 맞는 적당한 TestType을 설정

    m_combo2.SetCurSel(1);
    m_nRMode=1;     //son RMode   1: REAL R 
 
    m_edit2.SetWindowText("1");
//  m_edit3.SetWindowText("1");
//  m_edit5.SetWindowText("0");

    //son220426 UPPER_LOWER caldata를 둘다 쓰는 모드가 아니라면 체크박스를 숨긴다.
    m_bUseLowerCalJig=0;   //son220429
    if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)
    {
        m_checkLowerPin.ShowWindow(SW_HIDE);         //son220426
        m_bUseLowerCalJig=0;   //son220429
    }
    else
    {
        m_checkLowerPin.ShowWindow(SW_SHOW);         //son220426

        //son m_bUseLowerCalJig, m_checkLowerPin 초기화
        FileSysInfo01.LoadSave_UseLowerPin(_LOAD, m_bUseLowerCalJig);    //son220315     //son220331
        if (m_bUseLowerCalJig== 1)         //son220315
        {
            //son 하부가 없는 PinMode일때에는 하부핀 R 측정모드를 Off해야 한다.
            if (IS_NO_LOWER(SysSet12.m_nPinTotMax)) 
            {   //son220328

                m_bUseLowerCalJig=0;   // 하부핀 R 모드 off
                FileSysInfo01.LoadSave_UseLowerPin(_SAVE, m_bUseLowerCalJig);    //son220328     //son220331
                m_checkLowerPin.SetCheck(0);
                UpdateData(FALSE);      // UI에 값을 반영한다.

            }   //son220328
            else
                m_checkLowerPin.SetCheck(1);
        }
        else
            m_checkLowerPin.SetCheck(0);
    }


    //----------------------------------
    //son Grid 초기화 및 R 데이터 입력 

    //son g_nPin1R[testType][list][] 데이터를 파일에서 로딩한다.
    On_R_file_load(); 

    //son m_grid1를 현재 m_nTestType의 g_nPin1R[testType][list][i] 로 채운다. 
    Onload_Grid(m_nTestType );   //son211230

    m_nStep1=1;
    str.Format("%d", m_nStep1-1);        
    m_Label103.SetCaption(str); 
    
    return TRUE;   
                  
}

void CModeSer15::OnSelchangeCombo1() 
{
/*
    int n;
    n= m_combo1.GetCurSel() +1;   
    if(n==1){        m_nMode1= MODE6_OPEN_10V;       //son211230   6 -> MODE6_OPEN_10V
    }else if(n==2){  m_nMode1= MODE26_OPEN_50V;      //son211230  26 -> MODE26_OPEN_50V 
    }else if(n==3){  m_nMode1= MODE51_OPEN_100V;     //son211230  51 -> MODE51_OPEN_100V 
    }else if(n==4){  m_nMode1= MODE76_OPEN_150V;     //son211230  76 -> MODE76_OPEN_150V
    }else if(n==5){  m_nMode1= MODE101_OPEN_200V;    //son211230 101 -> MODE101_OPEN_200V
    }else if(n==6){  m_nMode1= MODE126_OPEN_250V;    //son211230 126 -> MODE126_OPEN_250V
    }else if(n==7){  m_nMode1= MODE205_uSHORT_1V;    //son211230 205 -> MODE205_uSHORT_1V
    }else if(n==8){  m_nMode1= MODE207_HR_SHORT_10V; //son211230 207 -> MODE207_HR_SHORT_10V
    }else if(n==9){  m_nMode1= MODE227_LEAK_50V;     //son211230 227 -> MODE227_LEAK_50V
    }else if(n==10){ m_nMode1= MODE252_LEAK_100V;    //son211230 252 -> MODE252_LEAK_100V
    }else if(n==11){ m_nMode1= MODE277_LEAK_150V;    //son211230 277 -> MODE277_LEAK_150V
    }else if(n==12){ m_nMode1= MODE302_LEAK_200V;    //son211230 302 -> MODE302_LEAK_200V
    }else if(n==13){ m_nMode1= MODE327_LEAK_250V;    //son211230 327 -> MODE327_LEAK_250V
    }
*/

    int eMode= m_combo1.GetCurSel();                //son211230
    m_nMode1=  m_VModeList.saList[eMode].nMode;     //son211230

    //son mode에 맞는 적당한 TestType을 설정
    m_nTestType= getRTestType(m_nMode1);   //son211230

    //son m_grid1을 초기화하고 현재 m_nTestType의 g_nPin1R[testType][list][i] 로 채운다. 
    Onload_Grid(m_nTestType);  //son211230

    //son mode 변경시에는 지금까지의 Manumal Test는 모두 초기화
    m_nStep1=1;     //son211230

    str.Format("%d", m_nStep1-1);       //son211230    
    m_Label103.SetCaption(str);         //son211230 
}


//son mode에 맞는 적당한 TestType을 설정
int  CModeSer15::getRTestType(int nMode1)    //son211230
{
/*
    if( m_nMode1<=170){      m_nTestType= R_TEST_OPEN;     //son211230 1 -> R_TEST_OPEN
    }else if( m_nMode1<=180){     m_nTestType=5;         
    }else if( m_nMode1<=200){     m_nTestType=0;
    }else if( m_nMode1<=205){     m_nTestType=2;         
    }else if( m_nMode1<=210){     m_nTestType=3;         
    }else if( m_nMode1<=350){     m_nTestType=4;                 
    }else{          m_nTestType=0;          
    }
*/
    int nTestType = R_TEST_OPEN; //son211230 1 -> R_TEST_OPEN; 

    if(nMode1<= MODE_OPEN_END){                 nTestType= R_TEST_OPEN;       //open : 1  //son211230 
    }else if (nMode1<= MODE_4W_END){            nTestType= R_TEST_4W;         //  4w : 5  //son211230 
    }else if (nMode1<= (MODE_SHORT_START -1)){  nTestType= R_TEST_SKIP;         
    }else if (nMode1<= MODE_uSHORT_END){        nTestType= R_TEST_uSHORT;     //  us : 2  //son211230
    }else if (nMode1<= MODE_HR_SHORT_END){      nTestType= R_TEST_HR_SHORT;   //  hr : 3  //son211230
    }else if (nMode1<= MODE_LEAK_END){          nTestType= R_TEST_LEAK;       //leak : 4  //son211230 
    }

    return nTestType;
}

void CModeSer15::OnSelchangeCombo2() 
{
    //son R 화면은 default로 real R로 출력해야 하므로  1이  REAL R (ohm) 이다. 
    //    반면에 Cal 화면은 ADC R 로 출력해야 하므로 1이 Raw R(ADC) 이고 2가 Real R 임.
    m_nRMode= m_combo2.GetCurSel();  // RCalMode   1: REAL R ,  2: RAW(ADC) R   (Cal 화면과 반대임)
}
 
//son Auto Test
//    Auto Test는 특정 Mode의 Test를 지정된  Count만큼 수행하는 것. 
void CModeSer15::On_R_Measure1() //SYLEE150122
{
    m_nRunMode1= R_AUTO;//sylee170626-2   //son211230



    On_R_Measure1_3();//sylee200916 


    m_nStep1=1;

}
   
//son Manual Test
//    Manual Test는 특정 Mode의 Test를 1회 수행하는 것. 
void CModeSer15::On_R_Measure2()  
{  
    if(m_nStep1>10){
        AfxMessageBox("   > 10   , Max. over!  => [Initial] ", MB_OK);  
        return  ;
    }
    str.Format("%d", m_nStep1);      
    m_Label103.SetCaption(str); 
    m_edit2.SetWindowText("1");
    m_nRunMode1= R_MANUAL;        //son211230


    On_R_Measure1_3();//sylee200916


    m_nStep1++;
}
   
//g_nPin1R[testType][list][] 데이터를 파일에서 로딩한다.
void CModeSer15::On_R_file_load() //sylee150316  //ACE380 개발 용  
{

    FILE *fp;
    char  fName[200], buf[100] ;  
    CString str,str1;
    int list,j,testType,nListTl,nT1;

    ::ZeroMemory(&g_nPin1R, sizeof(g_nPin1R)); 

    for (testType= R_TEST_OPEN; testType<= R_TEST_LEAK; testType++)
    { 
        ::ZeroMemory(&fName, sizeof(fName)); 

//son211230 나노시스 Customer인 경우, OPEN은 100kohm, Leak는 CalJig에서 가능한 수준의 R 값까지 모두 RList에 추가한다.
#if (defined(__NANOSYS__CUSTOMER) || defined(__LM__CUSTOMER))	//son220915_2

        //son221006 begin: s100용 R 기능 data 파일 read기능 추가
        if (SysSet13.m_nPinBlockType == PIN_BLK_S100_1
            || SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE)   //son230216_1
        {
            if(testType== R_TEST_OPEN) { 
                str.Format( "%s\\BDL\\QC\\OPEN2_S100.TXT", SysInfoView01.m_pStrFilePath1 );  //son211230
            }else if(testType== R_TEST_uSHORT) { 
                str.Format( "%s\\BDL\\QC\\USHORT2_S100.TXT", SysInfoView01.m_pStrFilePath1 ); //son211230
            }else if(testType== R_TEST_HR_SHORT) { 
                str.Format( "%s\\BDL\\QC\\HR2_S100.TXT", SysInfoView01.m_pStrFilePath1 );     //son211230
            }else if(testType== R_TEST_LEAK) { 
                str.Format( "%s\\BDL\\QC\\LEAK2_S100.TXT", SysInfoView01.m_pStrFilePath1 );   //son211230
            }       
        }
        else    //son221006 end
        {
            if(testType== R_TEST_OPEN) { 
                str.Format( "%s\\BDL\\QC\\OPEN2.TXT", SysInfoView01.m_pStrFilePath1 );  //son211230
            }else if(testType== R_TEST_uSHORT) { 
                str.Format( "%s\\BDL\\QC\\USHORT2.TXT", SysInfoView01.m_pStrFilePath1 ); //son211230
            }else if(testType== R_TEST_HR_SHORT) { 
                str.Format( "%s\\BDL\\QC\\HR2.TXT", SysInfoView01.m_pStrFilePath1 );     //son211230
            }else if(testType== R_TEST_LEAK) { 
                str.Format( "%s\\BDL\\QC\\LEAK2.TXT", SysInfoView01.m_pStrFilePath1 );   //son211230
            }       

        }


//son220110 나노시스 Customer가 아닌 경우는 원래의 OPEN 200ohm 까지의 RList로 사용한다.  by 사장님.
#else

        //son221006 begin: s100용 R 기능 data 파일 read기능 추가
        if (SysSet13.m_nPinBlockType == PIN_BLK_S100_1
            || SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE) 	//son230216_1
        {
            if(testType== R_TEST_OPEN) { 
                str.Format( "%s\\BDL\\QC\\OPEN_S100.TXT", SysInfoView01.m_pStrFilePath1 );  //son210514
            }else if(testType== R_TEST_uSHORT) { 
                str.Format( "%s\\BDL\\QC\\USHORT_S100.TXT", SysInfoView01.m_pStrFilePath1 ); //son210514
            }else if(testType== R_TEST_HR_SHORT) { 
                str.Format( "%s\\BDL\\QC\\HR_S100.TXT", SysInfoView01.m_pStrFilePath1 );     //son210514
            }else if(testType== R_TEST_LEAK) { 
                str.Format( "%s\\BDL\\QC\\LEAK_S100.TXT", SysInfoView01.m_pStrFilePath1 );   //son210514
            }       
        }
        else
        {
            //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
            if(testType== R_TEST_OPEN) { 
                str.Format( "%s\\BDL\\QC\\OPEN.TXT", SysInfoView01.m_pStrFilePath1 );  //son210514
            }else if(testType== R_TEST_uSHORT) { 
                str.Format( "%s\\BDL\\QC\\USHORT.TXT", SysInfoView01.m_pStrFilePath1 ); //son210514
            }else if(testType== R_TEST_HR_SHORT) { 
                str.Format( "%s\\BDL\\QC\\HR.TXT", SysInfoView01.m_pStrFilePath1 );     //son210514
            }else if(testType== R_TEST_LEAK) { 
                str.Format( "%s\\BDL\\QC\\LEAK.TXT", SysInfoView01.m_pStrFilePath1 );   //son210514
            }       
        }
#endif

        strcat( fName , str ); 
        fp=fopen(fName,"rt");
        if(fp==NULL){       
            str1.Format("Error No 15 :  File Reading ERROR!.  File Check! \n ->  %s", str);
            AfxMessageBox(str1, MB_OK);
            MyTrace(PRT_BASIC, str1);   //son220831
            return ;
        }

        //-------------------------
        //son Read RListCount
        fscanf(fp, "%s", buf);
        nT1=(int)atoi(buf);
        if(nT1<0){
            nListTl=0;
        }else if(nT1>50){
            nListTl=50;
        }else{
            nListTl=nT1;
        }
        g_nPin1R[testType][0][0]=nListTl;


        //----------------------------------------
        //son Read List of "Pin1, Pin2, Ref.R"
        for (list=1; list<= nListTl; list++)
        {
            for (j=1; j<= 3; j++){      
                fscanf(fp, "%s", buf);
                g_nPin1R[testType][list][j]=(int)atoi(buf);
            }
            fscanf(fp, "\n");
        }    
        fclose(fp);

    }//end of   for( int testType=1;  testType<=4;   testType++)

}
 



//son220405 R과 Cal이 같이 쓸 수 있도록 RCalCovert(...)를  Globals.cpp로 이동
//extern  double RCalCovert( int m_nMode1,  double &Rvalue, int ConMode, int part );


void CModeSer15::On_R_Measure1_3() ////sylee170417-2
{
    CString   str,str1, strfName,strfName1; 
    int i,j ,n4Wire, nRet1;
    int nLoop, nFastHR;
    int nRCount, nMeasCnt;  //son211230 nCount -> nMeasCnt
    int a1,a2, a3,a4; 
    int Kcount, nLeakoffset1 = 0; 
    int nFil;

    int nCC, nVRel,  nIR, nTimeDelay1;   //son211230 extern에서 CModeSer15 내부 변수로 변경
    
    

    //int m_nTestType; 
    int nStart1,nEnd1;
    int dPinVC[MAX_R_LIST +1], dPinVB[MAX_R_LIST +1];   //son211230 200 -> MAX_R_LIST
    __int64   tStart1, tStart2 ; 
    double dISet; 
    double nT[10];
//son211230    double nData[130][15]; 

    double  dR31[MAX_R_MEAS +1][150];
    double  dVB31[MAX_R_MEAS +1][150];
    double  dVC31[MAX_R_MEAS +1][150];
    double  dI31[MAX_R_MEAS +1][150]; //sylee171229-1

    double dRefLow,dRefHigh;//sylee200916
    int nCo1, nADCsum;//sylee200916
    double R2;//sylee200916

    double nAD[MAX_R_MEAS][2];//sylee200917
    double nADSum1;//sylee200917
    int nNo1;//sylee200917

    BeginWaitCursor(); //son211230  

    if ((m_nRunMode1== R_AUTO) || (m_nRunMode1== R_MANUAL && (m_nStep1<2))){//auto    //son211230
        ::ZeroMemory (&dR11, sizeof(dR11));
        ::ZeroMemory (&dVB11, sizeof(dVB11));
        ::ZeroMemory (&dVC11, sizeof(dVC11));
        ::ZeroMemory (&dI11, sizeof(dI11));
//        ::ZeroMemory (&gdCal1, sizeof(gdCal1));       //son220412
//son211230    ::ZeroMemory (&nData, sizeof(nData));
        ::ZeroMemory (&nT, sizeof(nT));
//son211230    ::ZeroMemory (&nData, sizeof(nData));
        ::ZeroMemory (&dPinVC, sizeof(dPinVC));
        ::ZeroMemory (&dPinVB, sizeof(dPinVB));  
    }

    Kcount=0;      m_nStop=0;     n4Wire=0; 

    Sub71.nADCount1=1; 
/*
    int list, col;
    for( list=1; list<=m_nListMax; list++ ){
        for(col=R_COL_AVG; col<=R_COL_NUM; col++){
            str.Format("          ");
            m_grid1.SetTextMatrix( list, col, str);
        }  
    }
*/
    InitGrid_MeasData();    //son220412_4

    DoEvents();

    m_edit2.GetWindowText(str);
    nMeasCnt=atoi(str);
    if( nMeasCnt<1)
    {
        nMeasCnt=0;
        //sylee211101 }else if( nMeasCnt>m_nMeasColMax)
    }
    else if( nMeasCnt> MAX_R_MEAS){
        nMeasCnt=MAX_R_MEAS;//SYLEE211122 m_nMeasColMax; //son211230  100 -> MAX_R_MEAS(300)
    }
    if( nMeasCnt==0){
        str.Format(" Error No 1023,   R Measure    Count No ?  ( 1~%d )   Error!!!", MAX_R_MEAS);
        AfxMessageBox(str);   
        return ;
    }
    str1.Format("%d",nMeasCnt); 
    m_edit2.SetWindowText(str1); 

    //son Grid에는 10개까지만 Meas 결과를 출력한다. 10개 이상은 .csv Log 파일로 출력.
    for(  i=1; i <=m_nMeasColMax ; i++)     
    {  
        if( m_nRunMode1== R_AUTO){//auto  //son211230
            if(i<=nMeasCnt){
                str.Format("NO. %d", i);        
            }else{
                str.Format("  " );
            }
        }
        //son Manunual Mode인 경우는 현재 m_nstep1에만 측정해야 한다.
        else{
            if(i<=m_nStep1){
                str.Format("NO. %d", i);        
            }else{
                str.Format("  " );
            }
        }
        m_grid1.SetTextMatrix(0, i+ (R_COL_NO1 -1), str);   //son211230  6-> (R_COL_NO1 -1)
    }  

/*  //son211230  의미없는 코드.  위의 str이  Leak Offset 관련 값이 아님.
    nLeakoffset1=atoi(str);
    if(nLeakoffset1<0){     nLeakoffset1=0; } 
    if(nLeakoffset1>200){   nLeakoffset1=200; }

    //sylee170725 ray   if(nMode1<201){       nLeakoffset1=0; }
    //=======================================================
*/
    //son  dRec을 설정한다.
    nRet1 = On_R_Measure1_3_Set_dRec(m_nMode1,                  //son input         //son220105
                            nFastHR, dISet, nFil, n4Wire,        //son output
                            nT[0], tStart1, tStart2,            //son output 
                            m_dVSet, nVRel, nCC,                //son output
                            nIR, nTimeDelay1);                  //son output

    int eMode = m_VModeList.findItemNo(m_nMode1);                //son220105
    if( m_nRunMode1== R_AUTO)
        str.Format("AUTO MeasCnt=%d", nMeasCnt);
    else
        str.Format("MANUAL stepNo=%d", m_nStep1);
    MyTrace(PRT_BASIC, "\n");
    MyTrace(PRT_BASIC, "On_R_Measure1_3_Set_dRec() nRet=%d: nMode1=%d(%s). %s  \n", //son220105
                            nRet1, m_nMode1, m_VModeList.saList[eMode].str, str);     
    if (nRet1 <= 0)
        return;

    //son OPEN 테스트에서는 m_nMode1과 m_nMode1+1 의 설정을 왔다갔다 해야 하므로 기준 Mode를 Keep한다.
    int nMode1_org = m_nMode1;      //son220105 


    //     m_edit3.GetWindowText(szText,10);    
    //     cc = atoi( szText );  

    //son mode에 맞는 적당한 TestType을 설정
    m_nTestType= getRTestType(m_nMode1);     //son211230

    if( m_nTestType>0){
        nRCount=g_nPin1R[m_nTestType][0][0];       
        if(m_nTestType==R_TEST_4W){ //son211230
            nRCount=0;
        }
    }
    else{
        nRCount=0;   
    }
    if(nRCount<=0){     nRCount=0;  } 
    if(nRCount>100){    nRCount=100; } 

    nLoop=5;
    DoEvents();

    int calPart = (m_bUseLowerCalJig)? PART_LOWER: PART_UPPER;    //son220411

    int RList;
    //==============
    // 4W  SET
    //==============
    if( n4Wire==4 ) //son 4W R 기능이 구현이 되어 있지 않아서 이 코드는 사용 안 함. 
    {               //    4W는 R화면이 아니라 '4W Setup 화면'으로 고객이 R 측정성능 확인함.

        if( m_nMode1==MODE171_4W){ //son211230
            Kcount=3;
        }else{
            Kcount=1;
        }

        for( j=1; j<=nMeasCnt; j++ )  //count
        {
            nLoop=1;    

            for( i=1; i<=nRCount; i++){
                dR11[j][i]=0.0;
            } 

            for( i=1; i<=nRCount; i++)
            {
                for( k=0; k<Kcount; k++)
                {   

Task1:

                    if( m_nStop==2){
                        AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
                        goto task_EXIT;   //son230210_4 task2 -> task_EXIT로 명시적으로 수정.             
                    }   

                    a1=g_nPin1R[m_nTestType][i][1]-1;    
                    a2=g_nPin1R[m_nTestType][i][2]-1;
                    a3=g_nPin1R[m_nTestType][i][3]-1;    
                    a4=g_nPin1R[m_nTestType][i][4]-1;

                    if (m_bUseLowerCalJig) {   //son220315     //son220328
                        a1 += (SysSet12.m_nLowerJigStartPin -1);
                        a2 += (SysSet12.m_nLowerJigStartPin -1);
                        a3 += (SysSet12.m_nLowerJigStartPin -1);
                        a4 += (SysSet12.m_nLowerJigStartPin -1);
                    }   //son220315     //son220328

                    Sub71.AOnPinSet1(2 ,a2, 3);    //VC  TR PinNo(0~8k)    
                    Sub71.AOnPinSet1(2 ,a3, 4);    //VB  RELAY PinNo(0~8k)  
                    Sub71.AOnPinSet1(2 ,a4, 2);    //VC  RELAY PinNo(0~8k)  
                    Sub71.Delay_100ns(3000);      
                    Sub71.AOnPinSet1(2 ,a1, 1);    //VB  TR PinNo(0~8k)   

                    Sub71.Delay_100ns(nTimeDelay1); 
                    if(nLoop==1){//SYLEE150211 RAY  
                        Sub71.Delay_100ns(nTimeDelay1); 
                    }   

                    double dSum1_W1, dSum1_Y1, dMin1,dMax1,dMin2,dMax2;

                    dSum1_W1=0; dSum1_Y1=0;
                    dMin1=0;     dMax1=0;    dMin2=0;    dMax2=0; 

                    for( int k2=1; k2<=10; k2++){

                        Sub71.AOnReadV1I1(5); // READ  4W V  , I     

                        if( k2==1){
                            dMax1=Sub71.W1;
                            dMin1=Sub71.W1;
                            dMax2=Sub71.Y1;
                            dMin2=Sub71.Y1;
                        }else{
                            if(Sub71.W1<dMin1){
                                dMin1=Sub71.W1;
                            }
                            if(Sub71.W1>dMax1){
                                dMax1=Sub71.W1;
                            }
                            if(Sub71.Y1<dMin2){
                                dMin2=Sub71.Y1;
                            }
                            if(Sub71.Y1>dMax2){
                                dMax2=Sub71.Y1;
                            }
                        }
                        dSum1_W1=dSum1_W1+Sub71.W1;
                        dSum1_Y1=dSum1_Y1+Sub71.Y1;

                    }

                    Sub71.W1=(dSum1_W1-(dMax1+dMin1))/(10-2);
                    Sub71.Y1=(dSum1_Y1-(dMax2+dMin2))/(10-2);

                    Sub71.OnPinAllReset1(); ///PIN ALL RESET  

                    if( nLoop >0){  
                        if(nLoop==1){                               
                            i=1;                           
                        }else{
                            i++;
                        }
                        nLoop--;
                        goto Task1;     
                    }

                    if(Sub71.Y1<0.0000001){
                        Sub71.Y1=0.0000001;
                    }

                    double nT1,nT2;  //4WSETUP140107

                    nT1=dISet-dISet*0.1;
                    nT2=dISet+dISet*0.1;

                    if(nT1<Sub71.Y1){
                        if(Sub71.Y1<nT2){
                            Sub71.Y1=dISet; //SYLEE140107 //SYLEE171024-1 I FIX RAY
                        }
                    } 

                    if(Sub71.Y1>0){
                        if( nIR==1 || nIR==2){//I mA                         
                            Sub71.R1=((Sub71.W1)/Sub71.Y1)*10.;
                        }else if( nIR==3 || nIR==4  || nIR==5 ){  //I uA                             
                            Sub71.R1=((Sub71.W1)/Sub71.Y1)*1000;//배율 /1000                                  
                        }else{
                            Sub71.R1=9999999999;
                        }       
                    }else{
                        Sub71.R1=9999999999;
                    }            

                    Sub71.R1=Sub71.R1*1000;

                    if( k==0){
                        dR11[j][i]=0;   dVC11[j][i]=0;
                        dVB11[j][i]=0;  dI11[j][i]=0;
                    }
                    dR11[j][i]=dR11[j][i]+Sub71.R1;    //R1; 
                    dVC11[j][i]=dVC11[j][i]+Sub71.W1;  //vc  
                    dVB11[j][i]=dVB11[j][i]+Sub71.V1;  //vb  
                    dI11[j][i]=dI11[j][i]+Sub71.Y1;    //i  

                }////for END   for( i=0; i<64; i++)


                if( n4Wire!=4 ){
                    Sub71.Delay_100ns(100000);
                } 

            }//  for( k=0; k<10; k++)


            for( i=1; i<=nRCount; i++){
                dR11[j][i]=dR11[j][i]/(float)Kcount;//sylee131227kcount;   //R1;
                dVC11[j][i]=dVC11[j][i]/(float)Kcount;//sylee131227kcount;     //vc  
                dVB11[j][i]=dVB11[j][i]/(float)Kcount;//sylee131227kcount;   //vb   //sylee130508
                dI11[j][i]=dI11[j][i]/(float)Kcount;//sylee131227kcount;    //i      //sylee130508  
            }

        } //for END  for( j=0; j< nMeasCnt; j++)  //count

    }


    //==============
    // 2W  SET
    //==============
    else  
    {


        //###########################################################################################

        if( m_nRunMode1== R_AUTO){//auto  //son211230
            nStart1=1;
            nEnd1=nMeasCnt;
        }
        //son Manunual Mode인 경우는 현재 m_nstep1만 측정해야 한다.
        else{
            nStart1=nEnd1=m_nStep1;
        }
//son230210_3 begin:
#if 0
        for(int meas=nStart1; meas<=nEnd1; meas++ )  //count  //son211230 j -> meas
        {
#else
        int meas, meas2;  //son230213_3 첫번째 측정은 버리는 기능을 위해 추가. 
        int nStart2;

        //son Auto Test 일때는 첫번째 meas를 두번 측정하기 위해 meas2 는 0(nStart1 -1)부터 시작한다.
        if( m_nRunMode1== R_AUTO)
            nStart2= nStart1 -1;
        //son Manual Test는 원래대로 meas를 1부터 시작.
        else
            nStart2= nStart1;

        
        for(meas2=nStart2; meas2<=nEnd1; meas2++ )  
        {

            meas = meas2;
            if( m_nRunMode1== R_AUTO)
            {
                //son 첫번째 meas를 두번측정하기 위해 meas2=0과 meas2=1 모두 meas는 1이다.
                if (meas2 == (nStart1 -1))     
                    meas = nStart1;
            }
#endif
//son230210_3 end

            //SYLEE200908-1  Sleep(1);



            //#######################################################################################


            ::ZeroMemory (&dR31, sizeof(dR31));//sylee171229-1
            ::ZeroMemory (&dVB31, sizeof(dVB31));//sylee171229-1
            ::ZeroMemory (&dVC31, sizeof(dVC31));//sylee171229-1
            ::ZeroMemory (&dI31, sizeof(dI31));//sylee171229-1

            //sylee200908  for( int k2=1; k2<=3; k2++) //  3count//sylee171229-1
            for( int k2=1; k2<=1; k2++)//sylee200908 
            {
                if (m_nRunMode1== R_AUTO && 
                        //son 다음 OPEN meas에는 원래의 m_nMode1의 dRec으로 돌아간다.  RList for문 들어가기 전에 1회 호출.
                        ((m_nTestType == R_TEST_OPEN && meas > 1)
                         //son SHORT Auto 측정에서 측정마다 org dRec 설정을 한번씩 해준다. (Auto Leak 고저항 NG나오는 문제점 보완) 
                //sylee230814-2         || (m_nTestType != R_TEST_OPEN && meas > 1))) //son230214_3
				      || (m_nTestType != R_TEST_OPEN && meas >= 1)))  //sylee230814-2   
                {
                    m_nMode1 = nMode1_org;  //son m_nMode1 자체도 원래 값으로 돌린다.   //son220105
                    nRet1 = On_R_Measure1_3_Set_dRec(m_nMode1,                  //son input         //son220105
                                            nFastHR, dISet, nFil, n4Wire,       //son output
                                            nT[0], tStart1, tStart2,            //son output 
                                            m_dVSet, nVRel, nCC,                //son output
                                            nIR, nTimeDelay1);                  //son output

                    int eMode = m_VModeList.findItemNo(m_nMode1);                //son211230
                    MyTrace(PRT_BASIC, "On_R_Measure1_3_Set_dRec() nRet=%d: nMode1=%d(%s).   \n", //son220105
                                            nRet1, m_nMode1, m_VModeList.saList[eMode].str );     
                    if (nRet1 <= 0)
                        return;
                }

                //SYLEE200908-1  nLoop=5;
                //son230210_2 //nLoop=0; //SYLEE200908-1
                nLoop=5; //son230210_2 //son 첫번째 5개 정도의 R을 측정한 후에는 다시 처음부터 측정(Task_R5_retry) count

                for(RList=1; RList<=nRCount; RList++)   //son211230 i -> RList
                {
Task_R5_retry:  //son230210_2

                    if( m_nStop==2){
                        AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
                        goto task_EXIT;  
                    }

                    int nLoop1 = 0;  
                    nCo1=0;//sylee200916


                    //son211230 OPEN 200ohm 이상은 m_nMode1+1로 dRec 설정해야 한다.  200ohm 넘어가는 시점 (300ohm)에 1회 호출.
                    //          같은 OPEN Test 에서 dRec 설정만 바뀌므로 m_nTestType이나 nRCount는 바뀔 필요 없다.
                    if (m_nTestType == R_TEST_OPEN && g_nPin1R[m_nTestType][RList][R_COL_REF_R] == 300)  //son220105
                    {

                        //son m_nMode1이 전역변수여서 실제로 변경을 해야 R 계산등이 변경된 m_nMode1 기준으로 계산됨. 
                        m_nMode1 = m_nMode1 +1; //son220105
                        nRet1 = On_R_Measure1_3_Set_dRec(m_nMode1,          //son input //son220105
                                                nFastHR, dISet, nFil, n4Wire,   //son output   
                                                nT[0], tStart1, tStart2,       //son output 
                                                m_dVSet, nVRel, nCC,           //son output
                                                nIR, nTimeDelay1);             //son output
                        MyTrace(PRT_BASIC, "On_R_Measure1_3_Set_dRec() nRet=%d: OPEN new nMode1=%d! g_nPin1R[%d][%d][R_COL_REF_R]=%d\n", 
                                            nRet1, m_nMode1,  m_nTestType, RList, g_nPin1R[m_nTestType][RList][R_COL_REF_R]);
                        if (nRet1 <= 0)
                            return;
                       
                    }

                    //dRefLow=g_nPin1R[m_nTestType][RList][R_COL_REF_R]*(1.0-0.048);//sylee200916 //son211230 3 -> R_COL_REF_R
                    //dRefHigh=g_nPin1R[m_nTestType][RList][R_COL_REF_R]*1.048;//sylee200916

                    dRefLow=g_nPin1R[m_nTestType][RList][R_COL_REF_R]*(1.0-0.05);//son230210 0.048 -> 0.05
                    dRefHigh=g_nPin1R[m_nTestType][RList][R_COL_REF_R]*1.05;     //son230210 0.048 -> 0.05

//son230210_2 begin: 
//son AMP 공회전을 Cal과 동일하게 R 5개 읽고 다시 처음으로 되돌아가가는 기능으로 대체한다.
#if 0   //son RAY 요청으로 삭제
        
					//son Amp 가열을 위해 첫번째 R에서 공회전. 
                    if(RList==1  //SYLEE200908             
							|| (m_nTestType==R_TEST_OPEN && g_nPin1R[m_nTestType][RList][R_COL_REF_R] == 300))//son220107	
                                        //son 200ohm 초과일 때 dRec 변경후에 추가 공회전 필요.
                                        //    RList 7 이  C:\ACE500\BDL\QC\OPEN.txt에서 300 ohm이다. 
                    {
                        for(int  i3=1; i3<=10; i3++)  //SYLEE200908	//son220107 5-> 10  by RAY
                        {
                            a1=g_nPin1R[m_nTestType][RList][R_COL_PIN1]-1;    //son211230 1 -> R_COL_PIN1 
                            a2=g_nPin1R[m_nTestType][RList][R_COL_PIN2]-1;

                            if (m_bUseLowerCalJig) {   //son220315 //son220328
                                a1 += (SysSet12.m_nLowerJigStartPin -1);    
                                a2 += (SysSet12.m_nLowerJigStartPin -1);    
                            }   //son220315 //son220328

                            Sub71.AOnPinSet1(2 ,a1, 2);         //son VB
                            Sub71.AOnPinSet1(2 ,a2, 1);         //son VC
                            Sub71.Delay_100ns(nTimeDelay1);                 
                            if( m_dVSet>20 ){  
                                if( nFastHR==1 ){
                                    Sub71.AOnReadV1I1_ADC_I();     
                                }else{
                                    Sub71.AOnReadV1I1(nVRel); 
                                }                  
                            } 
                            Sub71.OnPinAllReset1(); ///PIN ALL RESET  
                        }
                    }
#endif
//son230210_2 end                   




Task100:   //sylee200916

                    //sylee200908 Task11:


                    nADCsum=0;//sylee200916



                    //===============
                    // 핀 설정 
                    //===============

                    Sub71.OnPinAllReset1(); ///PIN ALL RESET                    
                    a1=g_nPin1R[m_nTestType][RList][R_COL_PIN1]-1;    //son211230 1 -> R_COL_PIN1 
                    a2=g_nPin1R[m_nTestType][RList][R_COL_PIN2]-1; 
                    if (m_bUseLowerCalJig) {   //son220315     //son220328
                        a1 += (SysSet12.m_nLowerJigStartPin -1);
                        a2 += (SysSet12.m_nLowerJigStartPin -1);
                    }   //son220315     //son220328

                    Sub71.AOnPinSet1(2 ,a1, 2);    //  PinNo(0~8k) nMod =1,VB,    =2VC,
                    Sub71.AOnPinSet1(2 ,a2, 1);   //  PinNo(0~8k)    nMod =1,VB,    =2VC,  

                    dPinVB[RList]= a1; 
                    dPinVC[RList]= a2 ;          
                    Sub71.Delay_100ns(nTimeDelay1);


                    //===============
                    // R  READING
                    //===============

                    if( n4Wire==4 ){        //son <--  4W는 다른 루틴을 사용하므로 이 부분은 수행 안된다.
                        Sub71.AOnReadV1I1(5);   
                    }
                    else
                    {                            
                        int ik1;  //son231005_2
                        //------------------
                        //son Short 일때
                        if(nFastHR==1){     
                            //////////////////////////////////////////////////////////////////////                                     
                            nNo1=20;//sylee200917		//son 20회 read해서 평균을 낸다.
                            nADSum1=0.0;
                            //::ZeroMemory(&nAD, sizeof(nAD));
                            for (ik1=1; ik1<=nNo1; ik1++)
                            {
                                //Sub71.AOnReadV1I1(nVRel);     //son220103
                                Sub71.AOnReadV1I1_ADC_I();      //son220103  Cal 코드와 동일하게 수정
                                //Sub71.Delay_100ns(1000); //100uS	//son220107 5000 -> 1000 by RAY.
                                //Sub71.Delay_100ns(5000); //500 uS	//son220415 1000 -> 5000 by RAY.
                                
                                //son231005_1 read해서 평균내는 시간을 16.7msec 주기의 배수로 맞추어 달라는 김영필이사님 요구사항.
                                Sub71.Delay_100ns(4860); //486 uSec	//son231005_1 5000-> 4860.   
                                                        // AOnReadV1I1_ADC_I()에서 349 uSec 소요. +486하면 835 uSec. 
                                                        // 0.835 * 20회= 16.7 msec total read 
                                                                    
                                //son short는 dIADC1이 Sub71.R1 이어야 한다. 
                                nADSum1=nADSum1+dIADC1;     //son220103 dIADC1 from Sub71.AOnReadV1I1_ADC_I()
                                                            //son231012 int nIADC1  -> double dIADC1
                            }
                            //son231006 dIADC1 = (int)(nADSum1 / nNo1);    
                            Sub71.R1 = nADSum1 / nNo1;   //son231006 나누기  20하는데 type이 int여서 소수점 이하 잘리는 버그수정

                            //son230209 short data 파일 출력에 사용하기 위해 Sub71.Y1 기록 (short의 dIADC1값 raw data) 
                            Sub71.Y1 = Sub71.R1;  //son231006 //son230209 
                            //////////////////////////////////////////////////////////////////////
                        }
                        //------------------
                        //son OPEN 일때
                        else
                        {
                        //son231005_2 if( nCC== __CV)// 2:cv   
                        //son231005_2     Sub71.AOnReadV1I1(7);    //son 7: Sub71.Y1(전류)만 Read. 전압값 X1, W1은 0으로 리턴            
                        //son231005_2 else// cc
                        //{ //son231005_2 OPEN CV도 OPEN CC와 똑같이 20회 read해서 평균내어 달라는 김영필 이사님 요구사항. 
                            //////////////////////////////////////////////////////////////////////                                     
                            nNo1=20;//sylee200917		//son 20회 read해서 평균을 낸다.
                            nADSum1=0.0;
                            ::ZeroMemory(&nAD, sizeof(nAD));
                            for (ik1=1; ik1<=nNo1; ik1++) 
                            {
                                //son231005_2 OPEN CV도 OPEN CC와 똑같이 20회 read해서 평균내어 달라는 김영필 이사님 요구사항. 
                                if (nCC == __CV)//son231005_3 2:cv      //son231005_2
                                    Sub71.AOnReadV1I1(7);  //son 7: Sub71.Y1(전류)만 Read. 전압값 X1, W1은 0으로 리턴  //son231005_2

                                else //cc       //son231005_2
                                    Sub71.AOnReadV1I1(nVRel); 

                                //Sub71.Delay_100ns(1000); //100uS	//son220107 5000 -> 1000 by RAY.
                                //Sub71.Delay_100ns(5000*4); //2 msec	//son220415 1000 -> 20000 by RAY.
                                
                                //son231005_1 read해서 평균내는 시간을 16.7msec 주기의 배수로 맞추어 달라는 김영필이사님 요구사항.
                                Sub71.Delay_100ns(16700); //1.67 msec	//son231005_1 20000-> 16700. 
                                                                        //          1.67 * 20회= 33.4msec total read time  
                                nAD[ik1][0]=Sub71.X1;
                                nAD[ik1][1]=Sub71.Y1;                                         
                            }
                            //////////////////////////////////////////////////////////////////////
                        //} //son231005_2

                            //son231005_2 begin:
                            //--------------------------------
                            //son Sub71.X1, Sub71Y1 평균내기 
                            for (ik1=1; ik1<=nNo1; ik1++){//sylee200917   
                                nAD[0][0]=nAD[0][0]+nAD[ik1][0];
                                nAD[0][1]=nAD[0][1]+nAD[ik1][1];                                          
                            }
                            Sub71.X1= nAD[0][0] /nNo1;
                            Sub71.Y1= nAD[0][1] /nNo1;
                            //son231005_2 end
                        }                            
                    }

                    //son231006 nADC1=dIADC1;//sylee200916  //son231006 nADC1 미사용 로컬 변수이므로 삭제.



                    //sylee200908 TaskInterR101: 

                    nRet1=Sub71.OnPinAllReset1(); ///PIN ALL RESET  
                    if( nRet1!=1){
                        AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
                        return ;
                    }

                    if(Sub71.Y1<0.0000001){
                        Sub71.Y1=0.0000001;
                    } 

                    //---------------
                    //son short 이면
                    if (nFastHR==1){ 
                        //son231006 나누기  20하는데 type이 int여서 소수점 이하 잘리는 버그수정
                        //son231006 Sub71.R1 = dIADC1;//ADC  
                        
                        Sub71.X1=0; 
                        Sub71.V1=0; 
                        Sleep(10);
                    }
                    else
                    {                          
                        //---------------
                        //son 4W 이면     <== R 화면에서는 20231005 현재 4W 미사용.
                        if (n4Wire == 4)
                        {                               
                            if (Sub71.Y1 > 0){
                                if( nIR==1 || nIR==2){//I mA                                         
                                    Sub71.R1=((Sub71.W1)/Sub71.Y1);//배율 /1000   4W=>  4W VOLTAGE                                   
                                }else if( nIR==3 || nIR==4  || nIR==5 ){  //I uA                                     
                                    Sub71.R1=((Sub71.W1)/Sub71.Y1)*1000.;//배율 /1000
                                }else{
                                    Sub71.R1=9999999999;
                                }       
                            }else{
                                Sub71.R1=9999999999;
                            }
                        }
                        //---------------
                        //son OPEN 이면
                        else
                        {  
                            if (nCC == __CV)//son231005_3 2:CV   r=(vb/RList)
                            {
                                //son CV는 Sub71.AOnReadV1I1(7) 결과에서 Sub71.Y1 만 사용. 
                                if( nIR==1 ||  nIR==2){
                                    Sub71.R1=(Sub71.V1)/Sub71.Y1*1000.0;   
                                }
                                else if( nIR==3 ||  nIR==4 ||  nIR==5){
                                    Sub71.R1=(Sub71.V1)/Sub71.Y1*1000000.0;
                                } 
                            }
                            else //son231005_3 if(nCC==__CC) //1:CC    r= (vb-vc)/RList                                
                            {
                                //son nFil==3(HV Open) 200ohm 이하. (첫번째 open dRec)
                                if( (nFil==3)&&(m_nMode1>= MODE16_OPEN_30V && m_nMode1<= MODE155_OPEN_300V)) //HVOPENSET  //SYLEE130724 //son211230
                                {
                                    if( nIR==1 || nIR==2 ){
                                        Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;                                
                                    }
                                    else if( nIR==3 ||  nIR==4 ){                                             
                                        //Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;  // china kushan samsung hogi 1,2   //sylee121029
                                        Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;  // //SYLEE200908 RAY
                                    }
                                    else if( nIR==5 ){                                                
                                        Sub71.R1=(Sub71.X1)/Sub71.Y1*1000.0;// #define#3hogifinal#   china kushan samsung hogi 3                         
                                    }  

                                    if(Sub71.X1>325){
                                        Sub71.R1=999999999.0;
                                    }
                                }
                                //son nFil==1(VC Fast), 300ohm 이상. (두번째 open dRec)
                                else
                                {
                                    if( nIR==1 ||  nIR==2 ){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
                                    }
                                    else if( nIR==3 ||  nIR==4  ||  nIR==5){
                                        Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
                                    }         
                                }
                            }

                        }
                    }  


                    //sylee170725 ray   if(m_nMode1>=201){
                    if(nLeakoffset1>0){
                        if(Sub71.R1>nLeakoffset1){
                            Sub71.R1=Sub71.R1-nLeakoffset1;
                        }
                    }

                    if( n4Wire==4 ){
                        Sub71.R1=Sub71.R1*1000;
                    }

                    //son RMode 1: REAL R(ohm) 로 표기해야 함.   (Cal 화면의 m_nMode2는 2가 Real R)
                    if( m_nRMode!=2){
                        //son230925 begin
                        //if(m_nTestType== R_TEST_OPEN) //son211230 1 -> R_TEST_OPEN
                        //{
                        //    //son220107 Global RCalCovert()와 ModeSer15::RCalCovert2()가 완전히 동일함.
                        //    //RCalCovert2( m_nMode1, Sub71.R1  , 1 );   //  ConMode=1  (AD raw ->R real),      
                        //    RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart);    //son220412_3
                        //}
                        ////son Short Test 는 CALR%dB.ini 파일의 ADC R 값이 내림차순이므로 RCalConvert_short을 사용한다.
                        //else if(m_nTestType==R_TEST_uSHORT ||m_nTestType==R_TEST_HR_SHORT || m_nTestType==R_TEST_LEAK) {   //son211230
                        //    Sub71.R1 = RCalConvert_short(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart );  //  ConMode=1  (AD raw ->R real) //son220412_3 //son221014
                        //}
                        RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, calPart);   //son230925 RCalConvert_short-> RCalCovert 로 다시 통일
                    }


                    R2=Sub71.R1;

#if 1 
                    int maxCnt = 5;                                     //son230209
                    //if ( m_nMode1> MODE216_LEAK_30V && Sub71.Y1 < 400)  //son230209
                    //    maxCnt = 20;                                    //son230209

                    if((Sub71.R1<dRefLow)||(Sub71.R1>dRefHigh)) {
                        //son230209 if(nCo1<5){		//son220107 50 -> 5  by RAY.  측정시간 너무 오래 걸리는 문제점 수정.
                        if(nCo1< maxCnt){		    //son230209_2 short 50M ohm 이상인데 range 안에 안 들어오면
                            nCo1++;//sylee200916
                            goto Task100;
                        }
                    }
#endif


                    dR11[meas][RList]=Sub71.R1;   //R1;     //son real R

                    if( n4Wire==4 ){
                        dVC11[meas][RList]=Sub71.W1;  //vc
                    }else{
                        dVC11[meas][RList]=Sub71.X1;  //vc
                    }

                    dVB11[meas][RList]=Sub71.V1;  //vb
                    dI11[meas][RList]=Sub71.Y1;   //RList 


//son230210_2 begin:
#if 1
                    //son 첫번째 5개 정도의 R을 측정한 후에는 다시 처음부터 측정하는 기능 (AMP를 가열하기 위해?)
                    if (nLoop > 0)
                    {
                        if (nLoop == 1)
                        {
                            if (m_dVSet > 20) 
                            {
                                //son OPEN 
                                if (m_nMode1 >= MODE16_OPEN_30V && m_nMode1 <= MODE_OPEN_END) { 
                                    RList = 1;
                                }

                                //son SHORT
                                else {
                                    RList = 1;
                                }
                            }
                            else {
                                RList = 1;
                            }
                        }
                        else {
                            //son230214_4 nRCount가 5보다 작을 때 RList가 5이상 증가하는 오류를 막기 위해 체크함
                            if (RList == (nRCount-1))   //son230214_4
                                nLoop = 2;

                            if (RList < nRCount)    //son230214_4
                                RList++;  //son 'goto Task_R5_retry' 하면 RList가 증가하지 않을 수 있어서 여기서 꼭 증가시켜야 함.

                        }

                        nLoop--;
                        goto Task_R5_retry;
                    }
#endif                   
//son230210_2 end


                    if( m_nMode1> MODE216_LEAK_30V){ //son211230
                        //son ADC raw R이 300보다 작으면 short 고저항.
                        //son230209 //if(Sub71.R1<300){
                        if(Sub71.Y1 < 400){   //son230209_2 ADC R값과 비교해야 한다.
                           Sleep(200);       //  김영필이사님요청. short 50 Mohm면 ADC 400 정도 측정됨.  
                                              //  short 50 Mohm 부터 천천히 측정한다.
                        }
                    }


                    //son Auto Test 일 때 첫번째 R은 두번 측정하므로 meas2 =1일때부터 sum해야 한다.
                    if ((m_nRunMode1== R_AUTO) && (meas2 != (nStart1 -1)))     //son230210_3
                    {
                        dR31[meas][RList]=dR31[meas][RList]+dR11[meas][RList];   //sylee171229-1
                        dVC31[meas][RList]=dVC31[meas][RList]+dVC11[meas][RList];   //sylee171229-1
                        dVB31[meas][RList]=dVB31[meas][RList]+dVB11[meas][RList];   //sylee171229-1
                        dI31[meas][RList]=dI31[meas][RList]+dI11[meas][RList];   //sylee171229-1
                    }

                    if(k2==3){//sylee171229-1
                        dR31[meas][RList]=dR31[meas][RList]/3.;   //sylee171229-1
                        dVC31[meas][RList]=dVC31[meas][RList]/3.;   //sylee171229-1
                        dVB31[meas][RList]=dVB31[meas][RList]/3.;   //sylee171229-1
                        dI31[meas][RList]=dI31[meas][RList]/3.;   //sylee171229-1
                    }//sylee171229-1


                }////for END     for( RList=1; RList<=nRCount; RList++) 


                Sleep(100);//sylee171229-1


            }////for END  for( k2=1; k2<=3; k2++) //  3count//sylee171229-1


            //#######################################################################################




            if( m_nMode1> MODE216_LEAK_30V){ //son211230
            //sylee230814      Sleep(3000); // 1000 msec //son230210_1 200-> 1000 requested by RAY
            }else{
                Sleep(100); // 100 msec 
            }
            DoEvents();


        } //for END  for( meas=0; meas< nMeasCnt; meas++)  //count

        Sleep(100);//sylee170613  ray

    }  //else{  //if( n4Wire==4 )

    //son AutoTest 마지막 meas와, ManualTest를 위해  원래모드로 돌리는 코드를 여기에도 추가한다. 
    if (m_nTestType == R_TEST_OPEN )
        m_nMode1 = nMode1_org;      //son220105 

    //===================================================================================




task_EXIT:  //STOP JUMP POSTION	//son230210_4 task2 -> task_EXIT로 명시적으로 수정


    tStart1=GetMilSecond2();     
    nT[1]=(double)(tStart1-tStart2)/1000.;    


    Sub71.OnPinAllReset1();

    if( nVRel==3 || nVRel==4)
    {      
        Sub71.OnPinAllReset1();  
        Sub71.ARunoff1();    
        Sub71.AOnRelayOffSel(nVRel); 
        Sub71.Delay_100ns(100000);  
        Sub71.AOnReadV1I1(nVRel); 

        if( Sub71.X1>20){
            Sub71.Delay_100ns(100000);  
            Sub71.AOnReadV1I1(nVRel); 
            if( Sub71.X1>20){
                Sub71.Delay_100ns(100000);  
            }  
        }
    }

    Sub71.AOnPoweroff2(); 
    Sub71.FilterSet(0); 
    Sub71.I_Filter(0); 
    Sub71.AOnReadV1Enable1(5);  
    //============================================================================================

    // for( j=1; j<=nMeasCnt; j++)  //count 

    if( m_nRunMode1== R_AUTO){//auto  //son211230
        // nStart1=1;nEnd1=nMeasCnt;
    }
    else{
        nStart1=1;   nEnd1=m_nStep1;
    }

    //---------------------------
    //   화면 Display
    //---------------------------

    //---------------
    //son R 값 출력
    for(int meas=nStart1; meas<=nEnd1; meas++ )  //count             
    {
        //son 초기화
        if((m_nRunMode1== R_MANUAL)&&(meas==nStart1)){//auto     //son211230
            //  if((m_nRunMode1==2)){//auto 
            for( RList=1; RList<= nRCount; RList++){  // 
                dR11[R_LOC_AVG][RList]=0;//sum
                dVB11[R_LOC_AVG][RList]=0;//VB sum
                dVC11[R_LOC_AVG][RList]=0;//VC sum
                dI11[R_LOC_AVG][RList]=0;//IC sum
            }
        }

        for( RList=1; RList<= nRCount; RList++)  // 
        {
            if( dR11[meas][RList]<0.0){
                str.Format("N/A");  
            }
            else{
                if(dR11[meas][RList]<100){
                    str.Format("%.2f", dR11[meas][RList]); 
                }
                else if(dR11[meas][RList]<1000){
                    str.Format("%.1f", dR11[meas][RList]); 
                }
                else{
                    str.Format("%.0f", dR11[meas][RList]);  
                }                    
            }                     

            //son Grid에는 10개까지만 Meas 결과를 출력한다. 10개 이상은 .csv 파일로 출력.
            if(meas<=10){//sylee211101
                m_grid1.SetTextMatrix(RList, meas+ (R_COL_NO1 -1) , str);//sylee20120114 //son211230  6-> (R_COL_NO1 -1)
            }

            //son AVG 계산을 위해 SUM 계산
            dR11[R_LOC_AVG][RList]=dR11[R_LOC_AVG][RList]+dR11[meas][RList];//sum
            dVB11[R_LOC_AVG][RList]=dVB11[R_LOC_AVG][RList]+dVB11[meas][RList];//VB sum
            dVC11[R_LOC_AVG][RList]=dVC11[R_LOC_AVG][RList]+dVC11[meas][RList];//VC sum
            dI11[R_LOC_AVG][RList]=dI11[R_LOC_AVG][RList]+dI11[meas][RList];//IC sum
        }
    }

    //------------------------
    //son AVG 계산, 화면 출력
    for( RList=1; RList<=nRCount; RList++)  
    { 
        if( m_nRunMode1== R_AUTO){                  //son230210_4      
            dR11[R_LOC_AVG][RList]=dR11[R_LOC_AVG][RList]/nMeasCnt;     
            dVB11[R_LOC_AVG][RList]=dVB11[R_LOC_AVG][RList]/nMeasCnt;  
            dVC11[R_LOC_AVG][RList]=dVC11[R_LOC_AVG][RList]/nMeasCnt;  
            dI11[R_LOC_AVG][RList]=dI11[R_LOC_AVG][RList]/nMeasCnt;  
        }

        if( m_nRunMode1== R_MANUAL){                  //son211230      
            dR11[R_LOC_AVG][RList]=dR11[R_LOC_AVG][RList]/m_nStep1;
            dVB11[R_LOC_AVG][RList]=dVB11[R_LOC_AVG][RList]/m_nStep1;  
            dVC11[R_LOC_AVG][RList]=dVC11[R_LOC_AVG][RList]/m_nStep1;  
            dI11[R_LOC_AVG][RList]=dI11[R_LOC_AVG][RList]/m_nStep1;  
        }


        if( dR11[R_LOC_AVG][RList]<0.0){
            str.Format("N/A");  
        }
        else
        {                                              
            if(m_nMode1> MODE_4W_END){//sylee150126 //son211230
                str.Format("%.0f", dR11[R_LOC_AVG][RList]);  //sylee150126
            }
            else
            {
                if(dR11[R_LOC_AVG][RList]<100){
                    str.Format("%.2f", dR11[R_LOC_AVG][RList]); 
                }
                else if(dR11[R_LOC_AVG][RList]<1000){
                    str.Format("%.1f", dR11[R_LOC_AVG][RList]); 
                }
                else{
                    str.Format("%.0f", dR11[R_LOC_AVG][RList]);  
                }
            }
        } 

        StringComa1(str);       
        m_grid1.SetTextMatrix(RList, R_COL_AVG, str);       //son211230 4 -> R_COL_AVG

        double dTmp1,dTmp2;
        dTmp1=0.0;
        if(dR11[R_LOC_AVG][RList]<0.0){
            dTmp2=0;
        }
        else{
            dTmp2=dR11[R_LOC_AVG][RList];
        } 
 
        //son 계산   (AVG - Ref.R ) / Ref.R  * 100.0        //son211230 3 -> R_COL_REF_R
        dTmp1=((dTmp2 -g_nPin1R[m_nTestType][RList][R_COL_REF_R])/ g_nPin1R[m_nTestType][RList][R_COL_REF_R]) *100.0; 
        str.Format("%.2f",dTmp1 ); 
        m_grid1.SetTextMatrix(RList, R_COL_REF_AVG_RATE, str);      //son211230 5 -> R_COL_REF_AVG_RATE
    }



    //-----------------------------
    //son DiffRate 계산, 화면 출력
    double temp1;               
    for( RList=1; RList<= nRCount; RList++)  // 
    {
        //for( meas=1; meas<=nMeasCnt; meas++)  //count 
        for( meas=nStart1; meas<=nEnd1; meas++ )  //count
        {
            if( meas==1){
                temp1=dR11[meas][RList];
                dR11[R_LOC_MAX][RList]=dR11[meas][RList];//max      //son220103 11 -> R_LOC_MAX  
                dR11[R_LOC_MIN][RList]=dR11[meas][RList];//min      //son220103 12 -> R_LOC_MIN 
            }
            else
            {
                if(dR11[meas][RList]<dR11[R_LOC_MIN][RList]){
                    dR11[R_LOC_MIN][RList]=dR11[meas][RList];//min
                }
                if(dR11[meas][RList]>dR11[R_LOC_MAX][RList]){       //son220103 11 -> R_LOC_MAX
                    dR11[R_LOC_MAX][RList]=dR11[meas][RList];//max  //son220103 11 -> R_LOC_MAX
                }
            }
        }

        dR11[R_LOC_DIFF][RList]=dR11[R_LOC_MAX][RList]-dR11[R_LOC_MIN][RList];//difference value        //son220103 11 -> R_LOC_MAX

        if(dR11[R_LOC_AVG][RList]==0){//if ave =0
            dR11[R_LOC_DIFF_RATE][RList]=0.0;               //son220103 14 -> R_LOC_DIFF_RATE   
        }
        else{
            dR11[R_LOC_DIFF_RATE][RList]=(dR11[R_LOC_DIFF][RList]/dR11[R_LOC_AVG][RList])*100;  //difference  rate   //son220103 13 -> R_LOC_DIFF
        }

        if( dR11[R_LOC_DIFF_RATE][RList]<0.0){              //son220103 14 -> R_LOC_DIFF_RATE
            str.Format("N/A");  
        }
        else
        { 
            //son RMode 2: RAW ADC R
            if( m_nRMode==2){                                    
                str.Format("%.2f", dR11[R_LOC_DIFF_RATE][RList]);       //son220103 14 -> R_LOC_DIFF_RATE              
            }

            //son RMode 1: REAL R
            else{                                       
                str.Format("%.2f  ", dR11[R_LOC_DIFF_RATE][RList]);     //son220103 14 -> R_LOC_DIFF_RATE              
            }
        } 
        m_grid1.SetTextMatrix(RList, R_COL_DIFF_RATE, str);//sylee20120114  //son211230 6 -> R_COL_DIFF_RATE
    } 

    //====================================================================
    //     if( m_nStop==2){
    //          AfxMessageBox("   R Calibraion   Stop!!!");      
    //     }       
    //============================================================================================
    //if( m_nRunMode1== R_AUTO)//auto  //son211230
        //Log1( m_nTestType,  nRCount,   nMeasCnt  , (int)m_dVSet);  //sylee211101
        writeCsv_MeasFile( m_nTestType,  nRCount,   nMeasCnt  , (int)m_dVSet);  //son210103 Log1 -> writeCsv_MeasFile
    
    //============================================================================================
    
    UpdateData(FALSE); //son230210
    EndWaitCursor();   //son211230  

    return  ;

}

//son220105 OPEN 200ohm 이상은 m_nMode1+1로 설정하기 위해 dRec설정부분을 함수화함. 
int CModeSer15::On_R_Measure1_3_Set_dRec(int nMode1,                               // input
                                        int& rFastHR, double& rdISet,               // output
                                        int& rnFil, int& rn4Wire, double& rnT_0,    // output 
                                        __int64& rtStart1, __int64& rtStart2,       // output
                                        double& rdVSet, int& rnVRel, int& rnCC,     // output
                                        int& rnIR, int& rnTimeDelay1)               // output
{
    int nIFilter,nHVOffSet;     //local 
    int nHVGain,nIGain,n4wGain; //local 
    double dInterI;             //local 
    double dVbR1,dVbR2;         //local 
    double dSCount, dWFactor;   //local 
    int nRet1;
    CString str;

    //------------------------
    //son prev dRec Set Init
	Sub71.OnPinAllReset1();

	if( rnVRel==3 || rnVRel==4)
	{      
		Sub71.OnPinAllReset1();  
		Sub71.ARunoff1();    
		Sub71.AOnRelayOffSel(rnVRel); 
		Sub71.Delay_100ns(100000);  
		Sub71.AOnReadV1I1(rnVRel); 

		if( Sub71.X1>20){
			Sub71.Delay_100ns(100000);  
			Sub71.AOnReadV1I1(rnVRel); 
			if( Sub71.X1>20){
				Sub71.Delay_100ns(100000);  
			}  
		}
	}

	Sub71.AOnPoweroff2(); 
	Sub71.FilterSet(0); 
	Sub71.I_Filter(0); 
	Sub71.AOnReadV1Enable1(5);  



	//----------------------------
	 

    if( nMode1<1  ||  nMode1> MAX_TEST_MODE ){  //son211230
        str.Format(" Error No 1023, \n    Mode=%d! Select Please( 1~%d ), Testing Start Error!!!", nMode1, MAX_TEST_MODE);
        AfxMessageBox(str);      
        return -1;
    } 

    //son "C:\\ACE500\\SETUP\\CAL\\CALR%02dB.INI" 에서 이전의 Calibration Ref 값 Load	
    int calPart = (m_bUseLowerCalJig)? PART_LOWER: PART_UPPER;    //son220411
    FileSysInfo01.LoadSaveSer13(_LOAD, nMode1, calPart);       //son 2: _LOAD    //son220412


    if( nMode1 <= (MODE_SHORT_START -1) ){    //son211230 200 -> MODE_SHORT_START -1
        rFastHR=0;
    }else{  
        rFastHR=1;
    }  



    rnCC=   (int)dRec[nMode1][1];  
    rnVRel= (int)dRec[nMode1][2];   
    rdVSet=  dRec[nMode1][3];
    rnIR=   (int)dRec[nMode1][4];
    rdISet= dRec[nMode1][5];
    rnTimeDelay1=(int)(dRec[nMode1][6]*10);
    rnFil=  (int)dRec[nMode1][8];       // ADC B   1:VC FAST, 2:VC SLOW,  3:HV OPEN, ( 4=4W )

    nIFilter = (int)dRec[nMode1][9];    // ADC A   1:I FAST,  2:I SLOW,   3:VB
    nHVOffSet =(int) dRec[nMode1][10];  
    nHVGain = (int)dRec[nMode1][11]; 
    nIGain = (int)dRec[nMode1][12];             
    n4wGain = (int)dRec[nMode1][13];       
    dSCount = dRec[nMode1][14];          
    dWFactor = dRec[nMode1][15];     
    Sub71.nADCount1 =(int)dRec[nMode1][7];

    //Sub71.nADCount1=20;//sylee200916 //son220415 RAY요청으로 삭제. cal과 측정회수 동일하게 맞추어서 R NG 안 나도록.

    if(Sub71.nADCount1<1 ){
        Sub71.nADCount1=1;
    }else if( Sub71.nADCount1>100){
        Sub71.nADCount1=100;
    }

    if(dSCount<1){   dSCount=1;   }

    if(nMode1>=MODE171_4W  && nMode1<=MODE179_4W_CV ){  //son211230
        rn4Wire=4;  \
        rnFil=4;      
        nMode4W2=1;
    }else{
        rn4Wire=0;  
        nMode4W2=0;
    }

    if(rnCC != __CC && rnCC != __CV){   //son231005_3
        AfxMessageBox(" Error No 1315, \n    I FAST,  I SLOW    ( NO 1,2 ? ) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
        return -1;
    }

    if( rdVSet<1 || rdVSet>300){  
        AfxMessageBox(" Error No 1316, \n    Voltage Set Range Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
        return -1;
    }

    if( rnVRel==1 || rnVRel==2  || rnVRel==3 || rnVRel==4){

    }else{      
        AfxMessageBox(" Error No 1025, \n    Voltage Set Mode 1~4( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!"); 
        return -1 ;
    }

    //son HV OPEN
    if( nMode1>=MODE16_OPEN_30V && nMode1<= MODE155_OPEN_300V){     //son211230
        if(rn4Wire!=0){
            AfxMessageBox(" Error No 1125, \n     HV OPEN START ERROR   ,  SET   4WIRE ?  \n\n  RECIPE NO 51~ 62 !,     Calibration Run Start Error!!!"); 
            return -1 ;
        }               
    }

    // INITAL SET
    nRet1=Sub71.AOnPoweroff2(); 
    if(nRet1!=1){
        AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
        return -1  ;
    } 

    //--------------------
    //son VB 설정 
    if( rnVRel==1){
        nRet1=Sub71.AOnVSetMulti1(rdVSet,0,0,0) ;
    }else  if( rnVRel==2){
        nRet1=Sub71.AOnVSetMulti1(0,rdVSet,0,0) ;
    }else  if( rnVRel==3){
        nRet1=Sub71.AOnVSetMulti1(0,0,rdVSet,0) ;
    }else  if( rnVRel==4){
        nRet1=Sub71.AOnVSetMulti1(0,0,0,rdVSet) ;
    } 

    if( nRet1!=1){
        AfxMessageBox(" Error No 1023, \n   V SET ERROR !,   SW TEST Run Start Error!!!"); 
        return -1 ;
    } 

    if(rn4Wire!=4){      
        //son230214_5 //Sub71.Delay_100ns(5000000); 	//son220107 100ms -> 500ms by RAY
        
        //son Leak 고저항 NG문제 관련해서 Cal과 동일하게 다시 수정함. 
        //    이거 원상복구 후에 Leak 고저항 NG 문제 없어짐.    //son230214_5
        Sub71.Delay_100ns(10000000); 	//son230214_5 500 msec -> 1000 msec 
    }

    if( rnIR==1 || rnIR==2 ){  //mA            
        dInterI=rdISet*0.9;                    
    }else if( rnIR==3 || rnIR==4 ){   // uA   
        dInterI=rdISet*0.9;                    
    }else if(rnIR==5){//uA
        dInterI= 10.0;
    }



    //---------------
    //son VB 체크   : VB가 range에 들어오지 않으면 최대 50회까지 read한다.
    
    rtStart1=GetMilSecond2();
    int Lo1=50;         
    Sub71.AOnReadVB1(rnVRel); 


Task101:    

    Sub71.AOnReadVB1(rnVRel);    

    if(rdVSet<5){
        dVbR2=rdVSet*(0.35); 
    }else{
        dVbR2=rdVSet*(0.1);      
    }
    dVbR1=rdVSet-dVbR2;
    dVbR2=rdVSet+dVbR2;

    if( Sub71.V1>300){
        if(Lo1-->0){
            Sub71.Delay_100ns(1000000);  //100 ms   
            goto Task101;
        }
        str.Format("Error No 1317,\n  \n\n  Power Check? \n\n  Calibration Mode=%d  VB READ Error!!! \n \n ***** HW CHECK Please! *****\n\n\n VB READING Error \nVB Set=%d, VB Read =%.2f,\n VB low=%.2f, VB high =%.2f,", (int)nMode1,rdVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str); 
    }
    if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){
        if(Lo1-->0){
            Sub71.Delay_100ns(1000000);  //100 ms   
            goto Task101;
        }
        str.Format("Error No 1318,\n \n\n  Power Check? \n\n\n    Calibration Mode=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),",(int)nMode1, rdVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str); 
    }
    else{
        if( nMode1>=MODE171_4W  &&   nMode1<=MODE179_4W_CV){    //son211230
            Sub71.V1=rdVSet;
        }
    } 

    if( rn4Wire==4 ){ 
        rnFil=4; 
        Sub71.AVB_CheckOut1(rnVRel,6); 	//son 두번째 argument 6은 사용되지 않음.
    } 

    if( rnFil==1 ||  rnFil==2){ 
        Sub71.AOnReadV1Enable1(rnVRel); 
    }else{
        Sub71.AOnReadV1Enable1(5);
    }

    if(rnFil==3){//hv open 
        Sub71.HV_OffSet_Gain(nHVOffSet, nHVGain);  
    }
    Sub71.FilterSet(rnFil);
    if(nIFilter==3){
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
        nIFilter=1;
    }
    Sub71.I_Filter(nIFilter); 

    Sub71.I_Gain(nIGain);//sylee220924

    if( rn4Wire==4 ){    
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

    Sub71.AOnISetOn1Run1(rnCC, rnVRel, rdVSet, rnIR, rdISet);     

    if(rn4Wire!=4){         
        //son230210_5 //Sub71.Delay_100ns(2000000); //200ms 	//son220107  삭제요청 by RAY,  VSet직후 500ms delay만 남겨라.
        Sub71.Delay_100ns(2000000); //200ms 	//son220107  삭제요청 by RAY,  VSet직후 500ms delay만 남겨라.  //son230210_5 다시 살림
    }    
    Sub71.OnPinAllReset1();  

    if(rn4Wire!=4){          
        Sleep(100);   
    }

    rtStart2=GetMilSecond2();     
    rnT_0 =(double)(rtStart2-rtStart1)/1000.;     //son output //son220105

    return 1;
}

//son Auto Test 측정 결과를 CSV 파일에 저장.
//    Auto Test는 특정 Mode의 Test를 지정된  Count만큼 수행하는 것. 
int  CModeSer15::writeCsv_MeasFile( int nTestType, int nRCount, int nMeasCnt,  int nVSet) //son210103 Log1 -> writeCsv_MeasFile
{

    FILE *fp;
    char  fName[200]; 
    CString strfName; 

    if(nVSet==15){
        nVSet=10;
    }

    int nStart1, nEnd1;
    if( m_nRunMode1== R_AUTO){//auto  //son211230
        nStart1=1;
        nEnd1=nMeasCnt;
    }
    else{
        //son Manunual Mode인 경우는 현재 m_nstep1만 측정해야 한다.
        //son230214_2 nStart1=nEnd1=m_nStep1;
        
        //son Manunual Mode인 경우도 현재 1~ m_nstep1까지 출력한다.
        nStart1=1;
        nEnd1=m_nStep1;
    }


    //----------------------------------------------
    //son "d:\\logR"  폴더가 없다면 새로 생성한다.
    char buf[FILE_PATH_SIZE];//son211230
    sprintf(buf,"d:\\logR");
    strfName.Format("%s", buf);
    if(!CheckDirExist(buf))     //son211230
    {
        if(!MakeNewDir(strfName))   //son211230
        {
            str.Format("%s creat Error!!", buf);
            AfxMessageBox(str);
            //return 0; //son220401_2  폴더가 존재하는데 CheckDirExist와 MakeNewDir()가 실패하는 문제시 파일write는 되도록 수정.
        }
    } 

    //----------------
    //son file OPEN
    //----------------
    strfName = get_LogR_Name(m_nTestType, (int)nVSet);      //son211230
    ::ZeroMemory(&fName,sizeof(fName)); 
    strcat( fName , strfName ); 
    fp = fopen(fName,"wt"); 
    if(fp == NULL){ 
        str.Format(" Error No 1501      file open? Error, saving Error !     file close! =>   %s   ",strfName);
        AfxMessageBox( str, MB_OK);  
        return  0;
    } 

    //----------------
    //son 헤더 출력
    //----------------
    int meas;
    //fprintf(fp, "      ,   Pin1,   Pin2,   Ref.R,   Test AVG.,  RefR-Avg Rate.,   Max-Min Rate,   " );               
    //fprintf(fp, "      ,   Pin1,   Pin2,   Ref.R,   Test AVG,  Min, Max, Pass, RefR-Avg Rate.,   Max-Min Rate, " );   //son220103            
    fprintf(fp, "      ,   Pin1,   Pin2,   Ref.R,   Test AVG,  Min, Max, RefLow, RefHigh, Pass, RefR-Avg Rate.,   Max-Min Rate, " );   //son230214_2            
    for(meas=nStart1; meas<=nEnd1; meas++ )  
    {  
        str.Format("NO.%d, ", meas);   
        fprintf(fp, "%s ",str); 
    }    
    for(meas=nStart1; meas<=nEnd1; meas++ )  
    {  
       fprintf(fp, "  , Count, VB, VC, I, R, " );   
    }

    fprintf(fp, " \n");


    //---------------------
    //son file data 출력
    //---------------------
    for(int RList=1; RList<=nRCount;RList++)
    {
        //--------------------------------------------
        //son        No       Pin1   Pin2     Ref.R        
        fprintf(fp, "No%d,     %d,    %d,      %d,          ",
                RList, ((m_bUseLowerCalJig)? (g_nPin1R[nTestType][RList][R_COL_PIN1] +(SysSet12.m_nLowerJigStartPin -1)): g_nPin1R[nTestType][RList][R_COL_PIN1]), //son220315 //son220328
                       ((m_bUseLowerCalJig)? (g_nPin1R[nTestType][RList][R_COL_PIN2] +(SysSet12.m_nLowerJigStartPin -1)): g_nPin1R[nTestType][RList][R_COL_PIN2]), //son220315 //son220328
                g_nPin1R[nTestType][RList][R_COL_REF_R]);   //son211230 3 -> R_COL_REF_R


        //------------------
        //son Test AVG            
        if(dR11[R_LOC_AVG][RList]<100){
            fprintf(fp, "%.2f,",dR11[R_LOC_AVG][RList]);         
        }else if(dR11[R_LOC_AVG][RList]<1000){
            fprintf(fp, "%.1f,",dR11[R_LOC_AVG][RList]); 
        }else{
            fprintf(fp, "%.0f,",dR11[R_LOC_AVG][RList]);  
        }

#if 1
        //------------------------------------
        //son R Min, R Max 
        
        fprintf(fp, "%.2f,",dR11[R_LOC_MIN][RList]);    //son220103 12 -> R_LOC_MIN
        fprintf(fp, "%.2f,",dR11[R_LOC_MAX][RList]);    //son220103 11 -> R_LOC_MAX
        

        //------------------
        //son Pass 여부 
        
        //son220103  NG/ PASS 출력 기능 추가    
        double dRefLow, dRefHigh;

        //son Min, Max 중에 하나라도 기준값을 넘었다면 NG 표시 한다.
        //son OPEN은 +- 5%,  50V 미만 Short는 +- 5%, 50V 이상 Short는 +-10% 기준
        //dRefLow  = dR11[R_LOC_AVG][RList] * (1.0 - 0.05);   //son220103
        //dRefHigh = dR11[R_LOC_AVG][RList] * (1.0 + 0.05);   //son220103
        dRefLow  = g_nPin1R[nTestType][RList][R_COL_REF_R] * (1.0 - 0.05);   //son220316
        dRefHigh = g_nPin1R[nTestType][RList][R_COL_REF_R] * (1.0 + 0.05);   //son220316
        if (nTestType == R_TEST_LEAK && m_nMode1 >= MODE226_LEAK_50V)       
        {
            //dRefLow  = dR11[R_LOC_AVG][RList] * (1.0 - 0.1);   //son220103
            //dRefHigh = dR11[R_LOC_AVG][RList] * (1.0 + 0.1);   //son220103
            dRefLow  = g_nPin1R[nTestType][RList][R_COL_REF_R] * (1.0 - 0.1);   //son220316
            dRefHigh = g_nPin1R[nTestType][RList][R_COL_REF_R] * (1.0 + 0.1);   //son220316
        }

        //son RefLow, RefHigh
        fprintf(fp, "%.2f,", dRefLow);     //son230214_2
        fprintf(fp, "%.2f,", dRefHigh);    //son230214_2
        
        if ( dR11[R_LOC_MIN][RList] < dRefLow  ||  dR11[R_LOC_MAX][RList] > dRefHigh)   //son220103 11 -> R_LOC_MAX
            fprintf(fp, "NG,");
        else
            fprintf(fp, "Pass,");

#endif        
        
        //--------------------------------------------------------
        //son                     RefR-Avg Rate        DiffRate
        
        double dTmp1,dTmp2;
        dTmp1=0.0;
        if(dR11[R_LOC_AVG][RList]<0.0){
            dTmp2=0;
        }else{
            dTmp2=dR11[R_LOC_AVG][RList];
        } 
        
        //son  "RefR-Avg Rate"      //son211230 3 -> R_COL_REF_R
        dTmp1=((dTmp2 -g_nPin1R[nTestType][RList][R_COL_REF_R]) / g_nPin1R[nTestType][RList][R_COL_REF_R]) *100.0;
        fprintf(fp, "             %.2f ,                %.2f , ",dTmp1 , dR11[R_LOC_DIFF_RATE][RList]); //son220103 14 -> R_LOC_DIFF_RATE


        for(meas=nStart1; meas<=nEnd1; meas++ )  
        { 
            //-------------------------------------
            //son         No1   ....   No300
            fprintf(fp, "   %.2f , ",dR11[meas][RList]);
        }


        //son220103  VB, VC, I 출력 기능 추가    
        for(meas=nStart1; meas<=nEnd1; meas++ )  
        { 
            //---------------------------------------------
            //son        No, VB, VC, I , R   반복 출력
            fprintf(fp, " , %d,", meas );   
            fprintf(fp, "%.2f,", dVB11[meas][RList]);   //son220103 VB 출력 기능 추가      
            fprintf(fp, "%.2f,", dVC11[meas][RList]);   //son220103 VC 출력 기능 추가 
            fprintf(fp, "%.2f,", dI11[meas][RList]);    //son220103 I  출력 기능 추가 
            fprintf(fp, "%.2f,", dR11[meas][RList]);    //son220103 R  출력 기능 추가 
        }

        fprintf(fp, " \n");
    } 

    fclose(fp); 

    return 1;

}

//son211230
CString CModeSer15::get_LogR_Name(int nTestType, int vSet)
{    
    CString strfName, str;


    CTime curTime = CTime::GetCurrentTime();
    /*
       if(nTestType== R_TEST_OPEN){  //son211230 1 -> R_TEST_OPEN
       strfName.Format("d:\\logR\\logR_%04d%02d%02d_%02d%02d%02d_Open.csv", 
       curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
       curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),m_nMode1); //sylee170123-1
       }

       else if(nTestType==R_TEST_uSHORT){    //son211230 2 -> R_TEST_uSHORT
       strfName.Format("d:\\logR\\logR_%04d%02d%02d_%02d%02d%02d_uShort.csv", 
       curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
       curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),m_nMode1); //sylee170123-1
       }

       else if(nTestType==R_TEST_HR_SHORT){  //son211230 3 -> R_TEST_HR_SHORT
       strfName.Format("d:\\logR\\logR_%04d%02d%02d_%02d%02d%02d_HR.csv", 
       curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
       curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),m_nMode1); //sylee170123-1
       }

       else if(nTestType==R_TEST_LEAK){      //son211230 4 -> R_TEST_LEAK
       strfName.Format("d:\\logR\\logR_%04d%02d%02d_%02d%02d%02d_LEAK.csv", 
       curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
       curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),m_nMode1); //sylee170123-1
       }

       else{
       strfName.Format("d:\\logR\\logR_%04d%02d%02d_%02d%02d%02d_R.csv", 
       curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
       curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),m_nMode1); //sylee170123-1
       }
       */
    int calPart = (m_bUseLowerCalJig)? PART_LOWER: PART_UPPER;    //son220411

    if(nTestType== R_TEST_OPEN){ //son211230 1 -> R_TEST_OPEN
        strfName.Format("d:\\logR\\Open%02dV%s.csv",vSet, //sylee170123-1
                                                   (calPart == PART_LOWER)? "_Lower":""); //son220411
    }
    else if(nTestType==R_TEST_uSHORT){    //son211230 2 -> R_TEST_uSHORT
        strfName.Format("d:\\logR\\uShort%s.csv", //sylee170123-1
                                                   (calPart == PART_LOWER)? "_Lower":""); //son220411
    }
    else if(nTestType==R_TEST_HR_SHORT){  //son211230 3 -> R_TEST_HR_SHORT
        strfName.Format("d:\\logR\\HR%s.csv", //sylee170123-1
                                                   (calPart == PART_LOWER)? "_Lower":""); //son220411
    }
    else if(nTestType==R_TEST_LEAK){      //son211230 4 -> R_TEST_LEAK
        strfName.Format("d:\\logR\\LEAK%02dV%s.csv",vSet, //sylee170123-1
                                                   (calPart == PART_LOWER)? "_Lower":""); //son220411
    }
    else{
        int eMode = m_VModeList.findItemNo(m_nMode1);   //son220317
        strfName.Format("d:\\logR\\logR_%04d%02d%02d_%02d%02d%02d_R_%s%s.csv", 
                curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
                curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),        //sylee170123-1 
                m_VModeList.saList[eMode].str,         //son220317 
                (calPart == PART_LOWER)? "_Lower":"");   //son220411
    }

    return strfName;
}

//son m_grid1을 초기화하고  nTestType의 g_nPin1R[testType][list][i] 로 채운다. 
void CModeSer15::Onload_Grid(int nTestType)  //reset
{
    int i, list, nTotal1; 
    CString   str;


    if(nTestType>R_TEST_LEAK){        //son211230 4 -> R_TEST_LEAK
        nTestType= R_TEST_LEAK;     //son 4W는 테스트하지 않는다.
    }
    if(nTestType<=0){
        nTestType= R_TEST_OPEN;     //son211230 1 -> R_TEST_OPEN
    }

    nTotal1= g_nPin1R[nTestType][0][0];

    for( list=1; list<=m_nListMax; list++){
        //son211230 for(i=1; i<=3; i++){    //son mode 변경시에 pin Data 초기화
        
        for(i=R_COL_PIN1; i<=R_COL_NUM; i++){  //son211230  mode 변경시에는 Pin Data와 지금까지의 Manual Test 데이터까지 모두 초기화
            str.Format("  " );  
            m_grid1.SetTextMatrix( list,i, str);
        }
    }

    for( list=1; list<=nTotal1; list++){
        for(i=R_COL_PIN1; i<=R_COL_REF_R; i++){     //son211230
            if (i == R_COL_PIN1 || i == R_COL_PIN2)    //son Pin이면 하부시작핀을 더해서 출력
                str.Format("%d",  ((m_bUseLowerCalJig)? (g_nPin1R[nTestType][list][i] +(SysSet12.m_nLowerJigStartPin -1)) : g_nPin1R[nTestType][list][i]));   //son220315  //son220328

            else //Ref.R이면 그대로 출력 
                str.Format("%d",  g_nPin1R[nTestType][list][i]);     //son220315

            StringComa1(str);
            m_grid1.SetTextMatrix( list,i, str);
        }
    }
}


void CModeSer15::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}
 
  
void CModeSer15::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
    SetAttr(pSlider->GetPos());
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CModeSer15::OnDblClickMsflexgrid1() 
{
    // TODO: Add your control notification handler code here
    BeginEditing();     
}

void CModeSer15::OnEnterCellMsflexgrid1() 
{
    // TODO: Add your control notification handler code here
    m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSer15::OnLeaveCellMsflexgrid1() 
{
    // TODO: Add your control notification handler code here
    if (OnEditing) {
        EndEditing();
    }   
}

void CModeSer15::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
    // TODO: Add your control notification handler code here
    m_grid1.SetRowSel(m_nCurRow);
    m_grid1.SetColSel(m_nCurCol);   
}

void CModeSer15::EndEditing()
{
    CString strText; 
    m_edit1.EnableWindow(FALSE);
    m_edit1.ShowWindow(SW_HIDE);
    m_edit1.GetWindowText(strText);

    m_grid1.SetText(strText);
    OnEditing = FALSE;
    m_grid1.SetFocus();
    //  SetModified(TRUE);  
}


void CModeSer15::BeginEditing()
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
    m_edit1.SetFocus();
    m_edit1.SetSel(0, -1); 
} 


BOOL CModeSer15::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CModeSer15::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


//son220107  CModeSer15::RCalCovert2 대신에  ChildView8의 전역 RCalCovert() 함수 사용하도록 수정. 
//           Calibration과 R이 동일한 Cal 함수를 사용해야 불일치로 인한 문제 예방이 가능하다.
#if 0
// Calibration convertor 
// ConMode=1 (raw -> real)   //son AD Raw 값을 real R(ohm)값으로 convert     
// ConMode=2 (real-> raw)    //son R(ohm) 값을 AD Raw값으로 convert
//son RCalCovert2()은 오름차순으로 정렬된 Open Cal Data 에 사용한다.  
double CModeSer15::RCalCovert2( int mode,  double &Rvalue, int ConMode )   // mode 1~76   ,  ConMode=1  (raw ->real),      2 (real->raw)
{
    int k,   nIn, nOut;
    double  aa, ab;
    CString str;

    aa=0.0;  
    ab=0.0;

    if( ConMode==2 ){
        nIn=2;  nOut=3;  //  real ohm  -> raw ADC
    }
    else{  //ConMode: 1
        //son nOut: gdCal1[part][mode][2][k] : ref R(realR ohm) 
        //son nIn : gdCal1[part][mode][3][k] : cal R(rawR ADC)
        nIn=3;  nOut=2;  // raw ADC -> real ohm
    }

    int nRCount=(int)gdCal1[part][mode][0][0];// Cal  list 개수 
    Sub71.R1=Rvalue; //input2

    if( (Sub71.R1>0) && (nRCount>0) && (nRCount<=128) )
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //son ConMode=1 이라면 cal table rawR 중에 입력된 rawR보다 큰 첫번째 값을 찾는다(근사값) 
        //    OPEN의 경우, CALR%d.ini 파일이 rawR이 오름차순으로 정렬되어 있으므로 첫번째 큰값이 근사값이다.
        for( k=1;  k<=nRCount;  k++){                                             
            if( Sub71.R1< gdCal1[part][mode][nIn][k]){   //son ConMode 1이면 ADC raw R 
                break;  
            }                                               
        }

        if( k==1)  //son 첫번째 R 
        {
            if(gdCal1[part][mode][nIn][0]>=0.0){       //son ConMode 1이면 ADC raw R 
                if(gdCal1[part][mode][nOut][0]<=0.0){  //son ConMode 1이면 ohm real R 
                    Sub71.R1= -1.0 ;  
                }
                else
                {
                    if(gdCal1[part][mode][nIn][1]!=0){
                        //son  ConMode 1 인 경우:  ADC raw R -> ohm real R 로 동작한다.
                        //      Sub71.R1은 처음에는 rawR(ADC)이고 변환후에는 realR(ohm) 이다.
                        //      realR/realRCal 의 비율은 rawR/rawRCal 비율과 같으므로
                        //      realR = (rawR / rawRCal) * realRCal
                        Sub71.R1= (Sub71.R1/gdCal1[part][mode][nIn][1])  *  gdCal1[part][mode][nOut][1]; 
                    }
                    else{
                        //error
                        str.Format("Error No 7223 ,   Divide by Zero. \n /gdCal1[part][mode][nIn:%d][1] is 0! \n Mode=%d %s Converting Error !\n\n" , 
                                                                        nIn, mode ,  (ConMode == 2)? "Real R -> Raw R": "Raw R -> Real R"); 
                        AfxMessageBox(str);
                    }
                }                       
            }

        }
        else if( k==nRCount)    //son 마지막 R 
        {
            if(gdCal1[part][mode][nIn][nRCount]!=0){                                                    
                Sub71.R1= (Sub71.R1/gdCal1[part][mode][nIn][nRCount]) *   gdCal1[part][mode][nOut][nRCount]; 
            }
            else{
                str.Format("Error No 7202 ,   Divide by Zero. \n  gdCal1[part][mode][nIn][nRCount]   \n  Mode=%d  OnInit  Real R -> Raw R Converting Error   !       \n\n" ,mode  ); 
                AfxMessageBox(str);
            }
        }

        //son  ConMode 1일때,   rawRCal[k-1] < rawR < rawRCal[k] 인 케이스 
        else{
            if( (gdCal1[part][mode][nIn][k]-gdCal1[part][mode][nIn][k-1])!=0.0){
                //son ConMode 1 인 경우:  ADC raw R -> ohm real R 로 동작
                //
                //ex)  a: rawRCal[k-1], b: realRCal[k-1],  c:rawRCal[k], d:realRCal[k],  x: rawR, y: realR 
                //     기울기 : (d-b)/(c-a)    y절편:  - (d-b)/(c-a)*a + b
                //     y = (d-b)/(c-a)x - (d-b)/(c-a)*a + b
                //     y = (d-b)/(c-a) * (x -a) + b     
                //     y = (x -a)/(c-a) * (d-b) + b   <==== (a,b) (c,d) 2개의 점으로 구한 1차 방정식
                //
                // realR = (rawR -a)/(c-a) * (d-b) + b
                // realR = (rawR - rawRCal[k-1])/(rawRCal[k] -rawRCal[k-1]) * (realRCal[k]-realRCal[k-1]) + realRCal[k-1]
                //
                //    aa = (rawR - rawRCal[k-1])/(rawRCal[k] -rawRCal[k-1]);
                //    ab = realRCal[k] - realRCal[k-1];
                // realR = aa * ab + realRCal[k-1];       <=== 기울기를 이용해서 x(rawR)에 대한 y(realR)을 구한다.

                aa=(Sub71.R1-gdCal1[part][mode][nIn][k-1])/(gdCal1[part][mode][nIn][k]-gdCal1[part][mode][nIn][k-1]);  // [3]= lookup table Calibration Raw R
                ab=gdCal1[part][mode][nOut][k]-gdCal1[part][mode][nOut][k-1]; 
                Sub71.R1=ab*aa+gdCal1[part][mode][nOut][k-1];                                             
            }
            else{
                Sub71.R1=(Sub71.R1/gdCal1[part][mode][nIn][k-1])*gdCal1[part][mode][nOut][k-1];    
            }
        }
    }

    return Sub71.R1;

}
#endif


//son221014 moved to Globals.cpp 
//          ModeSer15::RCalCovert3() -> Globals.cpp RCalConvert_short()  
//son221014 double CModeSer15::RCalCovert3( int mode,  double &Rvalue, int ConMode, int part )  //son220412 



//son211230
void CModeSer15::OnButton_ViewFile()  
{ 
	int nVSet = (int)m_dVSet;
    if(nVSet==15){
        nVSet=10;
    }

    CString strfName = get_LogR_Name(m_nTestType, nVSet);      //son211230

    ::ShellExecute(NULL,"open","EXCEL.EXE",strfName,"NULL",SW_SHOWNORMAL);
}
 
//son220103
void CModeSer15::OnButton_Stop()  
{ 
    m_nStop = 2;
}


//son 하부핀 R 측정 모드를 On하고  grid를 다시 그린다.
//    여기서 체크한 하부핀 Cal 측정 모드는 Cal 기능에도 영향을 준다. 
//    R에서 하부로 측정 on하면 Cal에도 On이 된다.
void CModeSer15::OnCHECKLowerPin() //son220315 하부핀 R 측정기능
{
	// TODO: Add your control notification handler code here
	
	CString str, str2;  //son220419
	
    if(m_checkLowerPin.GetCheck())      //son220315
    {
        if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)    //son220419 begin
        {
            str.Format("Error No 2114 !\n\nSysSet13.m_nUseCalData=UPPER_ONLY.\nCan't turn on this \"Use LowerCalJig\" CheckBox.\n\n");
            str2 += str;
            str.Format("First of all, change the Menu->MAP->Set1->UsaCalData to \"Upper_Lower\".\n\n");
            str2 += str;
            MyTrace(PRT_LEVEL1, str2);
            AfxMessageBox(str2, MB_OK);

            m_bUseLowerCalJig=0;   // 하부핀 R 모드 off
            m_checkLowerPin.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
        else //son220419 end
        {

            //son 하부가 없는 PinMode일때에는 하부핀 R 측정모드를 Off해야 한다.
            if (IS_NO_LOWER(SysSet12.m_nPinTotMax)) 
            {   //son220328
                CString str;
                str.Format("Error No 11203, PinMode:%s has no lower pin. Can't use LowerPin for R Measure.\n", 
                        STR_MAX_PIN(SysSet12.m_nPinTotMax));
                MyTrace(PRT_BASIC, str);
                AfxMessageBox(str);

                m_bUseLowerCalJig=0;   // 하부핀 R 모드 off
                m_checkLowerPin.SetCheck(0);
                UpdateData(FALSE);      // UI에 값을 반영한다.
            }   //son220328
            else
                m_bUseLowerCalJig=1; 
        }
    }
    else {
        m_bUseLowerCalJig=0; 
    } //son220315
	
    Onload_Grid(m_nTestType);   //son220315
}



