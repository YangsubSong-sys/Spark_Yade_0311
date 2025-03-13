// ModeSer13.cpp : implementation file
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stdafx.h"

#include "ModeSer13.h"
#include "ModeSUB71.h"
#include "FileSysInfo.h"
#include "Contec.h" 
#include "Globals.h"     //son220412

extern  int   n4W_Use;  //sylee200615-2

extern CSysSet19  SysSet19;  //SYLEE200217-1-4WCVMODE  //sylee200515-patch

extern int nFlag_4W_OUT50V;//SYLEE200411  //sylee200515-patch
//son231012_2 extern int nIADC1;  //sylee20120711-2
extern double dIADC1; //son231012_2 int nIADC1  -> double dIADC1으로 수정. 소수점이하 잘리는 문제보완.

extern CFileSysInfo  FileSysInfo01;//SYLEE130618 
extern CSysSet21  SysSet21; //SYLEE130618  
extern CSysSet22  SysSet22; //SYLEE130618  
extern CSysSet13  SysSet13;
extern CSysSet12  SysSet12; //son220331
extern CSysInfo05 SysInfo05;    // hdpark230105 spark cal
extern float dSparkV2[MAX_PIECE+1];   // hdpark230105 spark cal
extern float dSparkI2[MAX_PIECE+1];   // hdpark230105 spark cal
extern int nSparkRefV, nSparkHWoffSet1;	// hdpark230105 spark cal

extern int nMode4W2; //sylee140717

extern int dRecSetup[80][11]; //SYLEE130618

int nFlag1;
int nFlagQCMode1;//sylee121007

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int  nFlag_NewSparkBaord;//SYLEE201024-1  //SPARK NEW BOARD  


extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

extern  int nCC, nVRel, nIR;
extern  double  nVSet;

int i;

double  SCount, WFactor;//SYLEE20120313


extern void DoEvents();


extern __int64 GetMilSecond2();

//extern CSysInfoView01  SysInfoView01; //son moved to FileSysInfo.h

extern CModeSub71  Sub71;
extern double V1;  //vb
extern double Y1;  //i
extern double X1;  //vc
extern double W1;  //4WIRE


extern CFileSysInfo FileSysInfo01;
extern CSysInfoSer13  SysInfoSer13;

int nTimeDelay1;
double nCalC[30][10][100];
double dR1[20][150];            //son dR1[0][RList]  : Avg  Raw R
                                //son dR1[11][RList] : Max
                                //son dR1[12][RList] : Min
                                //son dR1[13][RList] : Diff      (Max - Min)
                                //son dR1[14][RList] : DiffRate  (Diff/Avg * 100)
double dVB1[20][150];
double dVC1[20][150];
double dI1[20][150];

extern  int nADCx5;//sylee180611-1


//son 3자리마다 ','를 포함해서 출력하도록 숫자 string을 가공. 
void StringComa1(CString& str1)
{
    CString strT1, strT2, str;
    int a, b, c, d, e, i3;

    str.Format("%s", str1);

    e = str.Find('.');
    if (e >= 1) {
        strT1 = str.Left(e);  //son '.'의 왼쪽 string 
        strT2 = str.Mid(e);   //son '.'을 포함해서 오른쪽 string
        a = strT1.GetLength();
        b = a % 3;
        c = a / 3;

        if (b == 0) {
            d = 0;
        }
        else if (a > 3) {
            d = -1;
        }
        else {
            d = 0;
        }
        for (i3 = c - 1; i3 > d; i3--) {
            strT1.Insert(b + 3 * i3, ',');
        }
        str = strT1 + strT2;
    }
    else {

        a = str.GetLength();
        b = a % 3;
        c = a / 3;

        if (b == 0) {
            d = 0;
        }
        else if (b == 1) {
            d = -1;
        }
        else if (b == 2) {
            d = -1;
        }
        else if (a > 3) {
            d = -1;
        }
        else {
            d = 1;
        }
        //for(  i3=c-1; i3>d; i3--){                         
        for (i3 = c - 1; i3 > d; i3--) {
            str.Insert(b + 3 * i3, ',');
        }
    }

    str1.Format("%s", str);
}


CModeSer13::CModeSer13(CWnd* pParent /*=NULL*/)
    : CDialog(CModeSer13::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeSer13)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

//son220328 IDC_CHECK_LowerPin 추가: 하부Pin Cal 측정 기능  
void CModeSer13::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeSer13)
        DDX_Control(pDX, IDC_CHECK_LowerPin, m_checkLowerPin);
        DDX_Control(pDX, IDC_COMBO01, m_combo1);
        DDX_Control(pDX, IDC_COMBO02, m_combo2);
        DDX_Control(pDX, IDC_GRID_1, m_grid1);
        DDX_Control(pDX, IDC_EDIT1, m_edit1);
        DDX_Control(pDX, IDC_EDIT2, m_edit2);
        DDX_Control(pDX, IDC_EDIT3, m_edit3);
        DDX_Control(pDX, IDC_EDIT5, m_edit5);
        DDX_Control(pDX, IDC_EDIT53, m_editUseStart);
        DDX_Control(pDX, IDC_EDIT54, m_editUseEnd);
    //}}AFX_DATA_MAP
}


//son220328 OnCHECKLowerPin 추가: 하부Pin Cal 측정 기능  
BEGIN_MESSAGE_MAP(CModeSer13, CDialog)
    //{{AFX_MSG_MAP(CModeSer13)
        ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)
        ON_CBN_SELCHANGE(IDC_COMBO02, OnSelchangeCombo2)
        ON_WM_MOUSEWHEEL()
        ON_WM_VSCROLL()
        ON_WM_HSCROLL()
        ON_BN_CLICKED(IDC_CHECK_LowerPin, OnCHECKLowerPin) 
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSer13 message handlers


BEGIN_EVENTSINK_MAP(CModeSer13, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSer13)
    ON_EVENT(CModeSer13, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
    ON_EVENT(CModeSer13, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
    ON_EVENT(CModeSer13, IDC_BLOAD, -600 /* Click */, Onload, VTS_NONE)
    ON_EVENT(CModeSer13, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSer13, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSer13, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSer13, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSer13, IDC_BUTTON101, -600 /* Click */, OnA_Cal_1, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON102, -600 /* Click */, OnButton102, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON103, -600 /* Click */, OnButton103, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON104, -600 /* Click */, OnButton104, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON105, -600 /* Click */, OnButton105, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON107, -600 /* Click */, OnButton107, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON108, -600 /* Click */, OnButton108, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON109, -600 /* Click */, OnButton109, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON129, -600 /* Click */, OnButton129, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON130, -600 /* Click */, OnButton130, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON135, -600 /* Click */, OnButton135, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON136, -600 /* Click */, OnButton136, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON125, -600 /* Click */, OnButton125, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON126, -600 /* Click */, OnButton126, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON127, -600 /* Click */, OnButton127, VTS_NONE)
	ON_EVENT(CModeSer13, IDC_BUTTON_CalConvertTest, -600 /* Click */, OnClickBUTTONCalConvertTest, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



BOOL CModeSer13::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

/*
    if ((pMsg->message == WM_KEYDOWN)
        && (pMsg->wParam == VK_ESCAPE)) {
        return TRUE;
    }
*/
    if ((pMsg->message == WM_VSCROLL)) {
        //     OnOK();
        return TRUE;
    }
    if ((pMsg->message == WM_KEYDOWN)
        && (pMsg->wParam == VK_RETURN)) {
        //     OnOK();
        return TRUE;
    }
    if (pMsg->message == VK_SCROLL) {

        //     OnOK();
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}


BOOL CModeSer13::OnInitDialog()
{
    CDialog::OnInitDialog();

    int  i;
    CString str;
    i = 9;
    InitControl();

    ::ZeroMemory(&nData, sizeof(nData));


    m_combo1.SetCurSel(5);
    m_nMode1 = MODE6_OPEN_10V;       //son220103 6 -> MODE6_OPEN_10V


    m_combo2.SetCurSel(1);
    m_nMode2 = R_RAW;     //son 1: ADC rawR  2: Real ohm R  //son230214

    m_edit2.SetWindowText("1");
    m_edit3.SetWindowText("1");
    m_edit5.SetWindowText("0");


    //son220426 UPPER_LOWER caldata를 둘다 쓰는 모드가 아니라면 체크박스를 숨긴다.
    m_bUseLowerCalJig = 0;   //son220429
    if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)
    {
        m_checkLowerPin.ShowWindow(SW_HIDE);         //son220426
        m_bUseLowerCalJig = 0;   //son220429
    }
    else
    {
        m_checkLowerPin.ShowWindow(SW_SHOW);         //son220426


        //son m_bUseLowerCalJig, m_checkLowerPin 초기화
        FileSysInfo01.LoadSave_UseLowerPin(_LOAD, m_bUseLowerCalJig);    //son220315     //son220331
        if (m_bUseLowerCalJig == 1)         //son220315
        {
            //son 하부가 없는 PinMode일때에는 하부핀 R 측정모드를 Off해야 한다.
            if (IS_NO_LOWER(SysSet12.m_nPinTotMax))
            {   //son220328

                m_bUseLowerCalJig = 0;   // 하부핀 R 모드 off
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

    On_RJigFileRead_ReadType(); //sylee150316  //ACE380 개?

    return TRUE;

}



void CModeSer13::OnSelchangeCombo1()
{
    m_nMode1 = m_combo1.GetCurSel() + 1;  //m_nMode1 1~63   //sylee130618   +1

}

void CModeSer13::OnSelchangeCombo2()
{
    m_nMode2 = m_combo2.GetCurSel();  //son 1: RAW R    2: REAL R 
}




//son 현재 Combo에 선택된  Mode Test
void CModeSer13::OnA_Cal_1() //SYLEE150122
{

    OnA_Cal_JIG_Read_151211(); //R-JIG 151211 R 위치 조정 R JIG 받음 .  //ACE400장비에 사용  시작 
}



// "C:\\ACE500\\BDL\\QC\\RJigReadType_S100.TXT" 또는 
// "C:\\ACE500\\BDL\\QC\\RJigReadType.TXT"에서 로드
#define	MAX_RJIG_ROW	32
#define	MAX_RJIG_COL	18
void CModeSer13::On_RJigFileRead_ReadType() //sylee150316  //ACE380 개발 용  
{

    // file name : RJigReadType
    // double RJigData1[35][20];//sylee150316

    FILE* fp;
    char  fName[200];
    CString str, str1;

    ::ZeroMemory(&fName, sizeof(fName));


    //sylee161115    0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //if(    SysSet13.m_nPinBlockType==3 || SysSet13.m_nPinBlockType==4 ) {
    if (SysSet13.m_nPinBlockType == PIN_BLK_S100_1
        || SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE) //son-32KPIN-20190902 //son230216_1
    {
        str.Format("%s\\BDL\\QC\\RJigReadType_S100.TXT", SysInfoView01.m_pStrFilePath1); //son201118  //son210514
    }
    else {
        str.Format("%s\\BDL\\QC\\RJigReadType.TXT", SysInfoView01.m_pStrFilePath1); //son201118   //son210514
    }

    strcat(fName, str);

    fp = fopen(fName, "rt");
    if (fp == NULL) {
        str1.Format("Error No 1010  :  File Reading ERROR!.  File Check! \n ->  %s", str);
        AfxMessageBox(str1, MB_OK);
        return;
    }

    char  buf[100];
    ::ZeroMemory(&RJigData1, sizeof(RJigData1));
    int row, col;
    for (row = 0; row <= MAX_RJIG_ROW; row++) {                  //son220103 32-> MAX_RJIG_ROW
        for (col = 1; col <= MAX_RJIG_COL; col++) {              //son220103 18-> MAX_RJIG_COL 
          // fscanf(fp, "%15.1f", &RJigData1[row][col]);
            fscanf(fp, "%s", buf);
            RJigData1[row][col] = (double)atof(buf);
        }
        fscanf(fp, "\n");
    }
    fclose(fp);
}



extern CSysSet12  SysSet12;  //SYLEE180724-1






void CModeSer13::OnA_Cal_JIG_Read_151211() //ACE400   
{
// hdpark230105 begin
	int nSparkOnFlag = 0;
	FILE* fp_vb;
	FILE* fp2;
	fp2 = fopen("C:\\ACE500\\spark.ini", "rt");
	if (fp2)	fscanf(fp2, "%d", &nSparkOnFlag);
	if (fp2)	fclose(fp2);

// hdpark230105 end

    CString   str, str1, strfName, strfName1;

    int i, nFil, nMeasCount, n4Wire, nRet1, nStartA, nStartB, nCo;
    int Lo1, nLoop, FastHR, nStart, nRCount, nShiftNo, cc;
    int nIFilter, nHVOffSet, nHVGain;//sylee130629
    int nIGain, n4wGain;//SYLEE150123    ///ACE380 
    int a1, a2, a3, a4;//sylee150316

    int dPinVC[200], dPinVB[200];
    __int64   tStart1, tStart2;

    FILE* fp;
    char  fName[200], fName1[200], szText[50];

    double nISet, dVbR1, dVbR2, InterI;
    double nT[10], nData[130][15];

    int Kcount, nLeakoffset1;

    double nAD[100][2];//sylee200917
    double nADSum1;//sylee200917
    int nNo1;//sylee200917

	// hdpark230105 spark cal begin
	int part = 0;
 
	 DoEvents(); DoEvents(); DoEvents();

	if(m_nMode2 == R_RAW || m_nMode2 == R_REAL) //son230214
	{
		part = (m_bUseLowerCalJig) ? PART_LOWER : PART_UPPER;    //son220411
	}
	else if(m_nMode2 == R_SPARK) //son230214
	{
		part = PART_SPARK;
	}
	// hdpark230105 spark cal end



    n4W_Use = 0;  //sylee200615-2

    FileSysInfo01.LoadSaveSet19(2);//sylee200217-4WCVMODE   //sylee200515-patch
    FileSysInfo01.LoadSaveSet12(2);//load  //SYLEE180724-1

    if (SysSet19.m_nSet12 != 1) {//SYLEE201024-1  //SPARK NEW BOARD 
        nFlag_NewSparkBaord = 2;//SYLEE201024-1  //SPARK NEW BOARD    
    }
    else {
        nFlag_NewSparkBaord = 0;  //SPARK OLD BOARD
    }

    if (SysSet13.m_nSparkMode == SPARK_NEW) //son200908
        Sub71.OnSparkADC(0);//SYLEE200714 

    ::ZeroMemory(&nData, sizeof(nData));
    ::ZeroMemory(&dR1, sizeof(dR1));
    ::ZeroMemory(&dVB1, sizeof(dVB1));
    ::ZeroMemory(&dVC1, sizeof(dVC1));
    ::ZeroMemory(&dI1, sizeof(dI1));
    ::ZeroMemory(&nT, sizeof(nT));
    ::ZeroMemory(&nData, sizeof(nData));
    ::ZeroMemory(&dPinVC, sizeof(dPinVC));
    ::ZeroMemory(&dPinVB, sizeof(dPinVB));
    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&fName1, sizeof(fName1));

    Kcount = 0;//sylee131227kcount
    nStop = 0;
    n4Wire = 0;

    //  V1old=0.0;//sylee150210 //ACE380

    Sub71.nADCount1 = 1;//test

    //son Grid 측정 영역 초기화
    int RList;
    for (RList = 1; RList <= MAX_CAL_RLIST; RList++) //son220408 MAX_CAL_RLIST:128
    {
        for (i = CAL_TEST_AVG; i <= CAL_TEST10; i++) {   //son220407 CAL_TEST_AVG:4,  CAL_TEST10:15
            str.Format("          ");
            m_grid1.SetTextMatrix(RList, i, str);
        }
    }
    DoEvents();

    m_edit2.GetWindowText(str);
    nMeasCount = atoi(str);
    if (nMeasCount < 1) {
        //son230210 nMeasCount = 0;
        nMeasCount = 1;
    }
    //else if( nMeasCount>20){
    //    nMeasCount=20;
    //}
    else if (nMeasCount > 10) {        //son220407  20 -> 10으로 수정
        nMeasCount = 10;
    }
    if (nMeasCount == 0) {
        AfxMessageBox(" Error No 1023,  Calibration  Count No ?  ( 1~10 )   Error!!!");
        return;
    }
    str1.Format("%d", nMeasCount);
    m_edit2.SetWindowText(str1);
    if (m_nMode1<1 || m_nMode1> MAX_TEST_MODE) {  //SYLEE20120102     //son220103 360->MAX_TEST_MODE
        AfxMessageBox(" Error No 1023, \n    Mode Select Please !,    ( 1~360 ) ,  Testing Start Error!!!");
        return;
    }

    if (m_nMode2 == R_RAW) // 1: Raw R //son230214
    {   
    //strfName.Format("C:\\ACE500\\SETUP\\LOGCAL\\001LOGRCal%d.csv",m_nMode1); //son201118
        strfName.Format("%s\\SETUP\\LOGCAL\\001LOGRCal%d%s.csv", SysInfoView01.m_pStrFilePath1, m_nMode1, //son201118 //son210514 
            (part == PART_LOWER) ? "_Lower" : ""); //son220411
    }
    else if (m_nMode2 == R_REAL) //son230214
    {//sylee120913     //son RMode 2: Real R
        //strfName.Format("C:\\ACE500\\SETUP\\LOGCAL\\001LOGRCal%dReal.csv",m_nMode1); //sylee170123-1
        strfName.Format("%s\\SETUP\\LOGCAL\\001LOGRCal%dReal%s.csv", SysInfoView01.m_pStrFilePath1, m_nMode1, //son201118 //son210514
            (part == PART_LOWER) ? "_Lower" : ""); //son220411
    }
	// hdpark230105 spark cal begin
    else if ((m_nMode2 == R_SPARK) && (m_nMode1 >= MODE221_LEAK_40V)) // 3: Spark Voltage //son230214
    {   
        strfName.Format("%s\\SETUP\\LOGCAL\\001LOGSparkCal%d.csv", SysInfoView01.m_pStrFilePath1, m_nMode1); 
    }
	else
	{
        str.Format("Run Mode Select[3] : Not support calibration mode %d", m_nMode1, m_nMode2);
        AfxMessageBox(str, MB_OK); //error messege
        return;
	}
	// hdpark230105 spark cal end

    ::ZeroMemory(&fName, sizeof(fName));
    //sylee170123-1   strcat( fName , SysInfoView01.m_pStrFilePath1 );//SYLEE20111212
    strcat(fName, strfName);
    fp = fopen(fName, "wt");
    if (fp == NULL) {
        str.Format(" Error No   001LOGRCal%d.csv file open   ?  Error, Saving Error !     file close! =>   *.cvs file  Close!   ", m_nMode1);
        AfxMessageBox(str, MB_OK); //error messege
        return;
    }

    if (m_nMode1 <= (MODE_SHORT_START - 1)) {  //SYLEE130618     //son220103 200 -> (MODE_SHORT_START-1)
        FastHR = 0;
    }
    else {
        FastHR = 1; //son Short Test 
    }

    if (m_nMode1 == MODE181_SELF_10V || m_nMode1 == MODE182_SELF_200V) {   //sylee130618  //son220103 181->MODE181_SELF_10V, 182->MODE182_SELF_200V 
        nFlagQCMode1 = 1;
    }
    else {
        nFlagQCMode1 = 0;
    }

    m_edit5.GetWindowText(str);
    nLeakoffset1 = atoi(str);//sylee161117-2
    if (nLeakoffset1 < 0) {//sylee161117-2
        nLeakoffset1 = 0;
    }
    if (nLeakoffset1 > 100) {//sylee161117-2
        nLeakoffset1 = 100;
    }
    if (m_nMode1 < MODE_SHORT_START) {//sylee161117-2  201-> MODE_SHORT_START
        nLeakoffset1 = 0;
    }

    nCC = (int)dRec[m_nMode1][1];   //son 1:__CC,  2:__CV
    nVRel = (int)dRec[m_nMode1][2];
    nVSet = dRec[m_nMode1][3];
    nIR = (int)dRec[m_nMode1][4];
    nISet = dRec[m_nMode1][5];
    nFil = (int)dRec[m_nMode1][8];  // ADC B   1:VC FAST, 2:VC SLOW ,  3:HV OPEN, ( 4=4W )
    nTimeDelay1 = (int)(dRec[m_nMode1][6] * 10);//syle20120104     SETUPCALIBRATION
    SysInfoSer13.m_nData[part][m_nMode1][0][1] = (double)nTimeDelay1;//delay time

    // int nIFilter,nHVOffSet,nHVGain;
    nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629        // ADC A   1:I FAST,  2:I SLOW,   3:VB 
    nHVOffSet = (int)dRec[m_nMode1][10]; //SYLEE130629
    nHVGain = (int)dRec[m_nMode1][11];  //SYLEE130629   

    ///*******************************************************                 
    nIGain = (int)dRec[m_nMode1][12];       //ACE380 //SYLEE150123    
    n4wGain = (int)dRec[m_nMode1][13];     //ACE380//SYLEE150123  
    ///****************************************************** 

    ///******************************************************************************************************************                  
    SCount = dRec[m_nMode1][14];         //SYLEE130629        
    WFactor = dRec[m_nMode1][15];     //SYLEE130629  
    ///******************************************************************************************************************

    //ADC COUNT
    Sub71.nADCount1 = (int)dRec[m_nMode1][7];

    int nEndMode;   //sylee200515-patch
    if (SysSet13.m_n4WCVMode == 1) { //sylee200217-4WCVMODE 
        nEndMode = MODE_4W_END;       //son220103 180-> MODE_4W_END
    }
    else {
        nEndMode = MODE_4W_END;  //sylee200910  177->180   //son220103 180-> MODE_4W_END
    } //sylee200515-patch

    if (m_nMode1 >= MODE_4W_START && m_nMode1 <= nEndMode) {//sylee180611-1     //son220103 171: MODE_4W_START
        if (nADCx5 == 2) {//sylee180611-1
#ifdef DEF_CARD_A1  //sylee170328
            Sub71.nADCount1 = (int)dRec[m_nMode1][7] * 10; //sylee181120 //RAY
#else  //DEF_CARD_A1  
            Sub71.nADCount1 = (int)dRec[m_nMode1][7] * 10;//sylee180611-1
#endif//DEF_CARD_A1 
        }
    }

    if (Sub71.nADCount1 < 1) {
        Sub71.nADCount1 = 1;
    }
    else if (Sub71.nADCount1 > 300) {
        Sub71.nADCount1 = 300;
    }

    if (SCount < 1) {
        SCount = 1;
    }

    //son 4W Mode 일 때.  nEndMode:MODE_4W_END
    if (m_nMode1 >= MODE_4W_START && m_nMode1 <= nEndMode) {//sylee180611-1  //son220103 171-> MODE_4W_START
        n4Wire = 4;
        nFil = 4;//sylee130628 
        nMode4W2 = 1;//sylee140717
        n4W_Use = 1;  //sylee200615-2

    }
    else {
        n4Wire = 0;
        nMode4W2 = 0;//sylee140717
    }


    //=========================================================================================
    //interlock
    if (nCC != __CC && nCC != __CV) { //son231005_3
        fclose(fp);
        AfxMessageBox(" Error No 1315, \n    I FAST,  I SLOW    ( NO 1,2 ? ) MODE SET ERROR !,     Calibration Run Start Error!!!");
        return;
    }

    if (nVSet < 1 || nVSet>300) {
        fclose(fp);
        AfxMessageBox(" Error No 1316, \n    Voltage Set Range Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!");
        return;
    }

    if (nVRel == 1 || nVRel == 2 || nVRel == 3 || nVRel == 4) {

    }
    else {
        fclose(fp);
        //      fclose(fp1);//sylee20111224 
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!");
        return;
    }

    if (m_nMode1 >= MODE16_OPEN_30V && m_nMode1 <= MODE155_OPEN_300V) {  //SYLEE130618 //son220103 16:MODE16_OPEN_30V, 155:MODE155_OPEN_300V
        if (n4Wire != 0) {
            AfxMessageBox(" Error No 1125, \n     HV OPEN START ERROR   ,  SET   4WIRE ?  \n\n  RECIPE NO 51~ 62 !,     Calibration Run Start Error!!!");
            return;
        }
    }


    // return ; //test위해 임시 막음.

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //INITAL SET

    nRet1 = Sub71.AOnPoweroff2();
    if (nRet1 != 1) {
        fclose(fp);     //fclose(fp1); 
        AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
        return;
    }

    //-----------
    //son VB 설정
    if ((SysSet19.m_n4WHVon == 1) && (m_nMode1 >= MODE171_4W && m_nMode1 <= MODE179_4W_CV)) { //sylee200410-4WCVMODE  //sylee200515-patch
    }
    else  //sylee200515-patch 
    {
        if (nVRel == 1) {
            nRet1 = Sub71.AOnVSetMulti1(nVSet, 0, 0, 0);
        }
        else  if (nVRel == 2) {
            nRet1 = Sub71.AOnVSetMulti1(0, nVSet, 0, 0);
        }
        else  if (nVRel == 3) {
            nRet1 = Sub71.AOnVSetMulti1(0, 0, nVSet, 0);
        }
        else  if (nVRel == 4) {
            nRet1 = Sub71.AOnVSetMulti1(0, 0, 0, nVSet);
        }
    }  //sylee200515-patch  

    if (nRet1 != 1) {
        fclose(fp);
        //          fclose(fp1); 
        AfxMessageBox(" Error No 1023, \n   V SET ERROR !,   SW TEST Run Start Error!!!");
        return;
    }

    if (n4Wire != 4) {//sylee130709         
        //sylee150126   Sub71.Delay_100ns(10000000); // 1SECOND  //SYLEE20111214 
        Sub71.Delay_100ns(10000000); // 1 sec    //sylee150126	//son220107 100ms -> 1000ms by RAY
    }

    if (nIR == 1 || nIR == 2) {  //mA            
        InterI = nISet * 0.9;
    }
    else if (nIR == 3 || nIR == 4) {   // uA   
        InterI = nISet * 0.9;
    }
    else if (nIR == 5) {//uA
        InterI = 10.0;
    }

    //---------------
    //son VB 체크   : VB가 range에 들어오지 않으면 최대 50회까지 read한다.
    
    tStart1 = GetMilSecond2();

    Lo1 = 50;


    if ((SysSet19.m_n4WHVon == 1) && (m_nMode1 >= MODE171_4W && m_nMode1 <= MODE179_4W_CV)) { //sylee201030 
    }
    else { //sylee201030
        Sub71.AOnReadVB1(nVRel);//sylee130802
    }


Task101:
    nFlag_4W_OUT50V = 0;//SYLEE200411 //sylee200515-patch

    if ((SysSet19.m_n4WHVon == 1) && (m_nMode1 >= MODE171_4W && m_nMode1 <= MODE179_4W_CV)) { //sylee200410-4WCVMODE
        nFlag_4W_OUT50V = 1;//SYLEE200411 //sylee200515-patch
        SysSet13.m_n4WCVMode = 1;//SYLEE201030
    }
    else //sylee200515-patch
    {

        Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

        if (nVSet < 5) {
            dVbR2 = nVSet * (0.35);//35%
        }
        else {
            dVbR2 = nVSet * (0.1);//10%    
        }
        dVbR1 = nVSet - dVbR2;
        dVbR2 = nVSet + dVbR2;

        if (Sub71.V1 > 300) {
            if (Lo1-- > 0) {
                Sub71.Delay_100ns(1000000);  //100ms   
                goto Task101;
            }
            str.Format("Error No 1317,\n  \n\n  Power Check? \n\n  Calibration NO=%d  VB READ Error!!! \n \n ***** HW CHECK Please! *****\n\n\n VB READING Error \nVB Set=%d, VB Read =%.2f,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1, nVSet, Sub71.V1, dVbR1, dVbR2);
            AfxMessageBox(str);
        }
        if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2) {
            if (Lo1-- > 0) {
                Sub71.Delay_100ns(1000000);  //100ms   
                goto Task101;
            }
            str.Format("Error No 1318,\n \n\n  Power Check? \n\n\n    Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),", (int)m_nMode1, nVSet, Sub71.V1, dVbR1, dVbR2);
            AfxMessageBox(str);
        }
        else {
            if (m_nMode1 >= MODE171_4W && m_nMode1 <= MODE179_4W_CV) {  //sylee200217-4WCVMODE
                Sub71.V1 = nVSet;
            }
        }

    }   //sylee200515-patch

    ///////////////////////////////////////////////////////////////////////////////////////////////

    // V1old=Sub71.V1;//sylee150210 //ACE380
    if (n4Wire == 4) { //interlock 
        nFil = 4;
        Sub71.AVB_CheckOut1(nVRel, 4);//SYLEE150210 //ACE380    FAIL =0  , PASS=1
    }

    if (nFil == 1 || nFil == 2) {//SYLEE130628
        Sub71.AOnReadV1Enable1(nVRel);
    }
    else {
        Sub71.AOnReadV1Enable1(5);
    }

    //  Sub71.V1=V1old;//sylee150210 //ACE380
    if (nFil == 3) {//hv open 
        Sub71.HV_OffSet_Gain(nHVOffSet, nHVGain);    //sylee140226
    }
    Sub71.FilterSet(nFil);//sylee130628 
    if (nIFilter == 3) {
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!");
        nIFilter = 1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629


    if (nIGain == 2) {//sylee150123 //ACE380
        Sub71.I_Gain(2);  //10x  =io on        
    }
    else {
        Sub71.I_Gain(1);  //1x  =io off
    }

    if (n4Wire == 4) {   //ACE380
        if (n4wGain == 2) {
            Sub71.On4W_GainSet(2);
        }
        else  if (n4wGain == 3) {
            Sub71.On4W_GainSet(3);
        }
        else  if (n4wGain == 4) {
            Sub71.On4W_GainSet(4);
        }
        else {
            Sub71.On4W_GainSet(1);
        }
    }
    else {
        Sub71.On4W_GainSet(1);
    }


    if (nFlagQCMode1 == 1) {//SYLEE121008
        Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, 0); //SYLEE121008                      
    }
    else {//sylee121007
        Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//SYLEE20111213     //vb RELAY ON
    }

    if (n4Wire != 4) {//sylee130709            
        Sub71.Delay_100ns(2000000); // 0.2SECOND //sylee150126
    }

    Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118

    if (n4Wire != 4) {//sylee130709             
       Sleep(100);//sylee150126  
    }

	DoEvents(); DoEvents(); DoEvents(); 

    tStart2 = GetMilSecond2();
    nT[0] = (double)(tStart2 - tStart1) / 1000.;  //test

    nStartA = 0;   // LOW  Resis.
    nStartB = 128; //high  Resis.  
    nCo = 128;
    nStart = 1;
    //nShiftNo=128*(62);  //  62 CARD START

    m_edit3.GetWindowText(szText, 10);
    cc = atoi(szText); //card no.  
    nShiftNo = 64 * (cc - 1);  //SYLEE150210//ACE380

    if (n4Wire == 4) {
        nStart = 1;
        //nRCount=27;  //sylee200909  26->27
        nRCount = MAX_4W_R_COUNT;  //son220503_2 27:MAX_4W_R_COUNT
    }
    else
    {
        if (m_nMode1 <= MODE_OPEN_END) { //open       //son220103 170: MODE_OPEN_END
            nStart = 1;
            //son220415 200kohm까지 측정되도록 개수를 22로 설정. requested by RAY
            nRCount = MAX_OPEN_R_COUNT; //ACE400-JIGNEW-1//SYLEE151212
        }
        //son Short
        else {
            if (nVSet > 20) {//hv short
			     nStart = 17;   //ACE400-JIGNEW-1//SYLEE151212
                 nRCount = MAX_SHORT_R_COUNT - nStart + 1;    //ACE400-JIGNEW-1//SYLEE151212     //SYLEE220503 37: MAX_SHORT_R_COUNT         
            }
            else {  //lv short
                nStart = 1;  //ACE400-JIGNEW-1//SYLEE151212       
                nRCount = MAX_SHORT_R_COUNT;//ACE400-JIGNEW-1//SYLEE151212    //SYLEE220503
            }
        }
    }

    nLoop = 5;//sylee20111226  //sylee20120302
    DoEvents();//sylee130719


    //--------------
    // 4W  SET
    //--------------
    int meas;
    if (n4Wire == 4)
    {

        if (m_nMode1 >= MODE176_4W_CV && m_nMode1 <= MODE_4W_END) {   //sylee200217-4WCVMODE     //sylee200515-patch   //son220103 180: MODE_4W_END
            Sub71.OnCCCV_Set(2);  //cv //sylee191015 ray    //sylee200215-4WCVMODE
        }
        else {
            Sub71.OnCCCV_Set(1);  //cc  //sylee191015 ray  //sylee200515-patch
        }
        Sub71.Delay_100ns(50000); // 5mS
        if (m_nMode1 == MODE171_4W) {     //son220103 171: MODE171_4W
            Kcount = 1;//sylee131227kcount  //sylee180221-2  kcount 3->1
        }
        else {
            Kcount = 1;
        }

        if (m_nMode1 == MODE171_4W || m_nMode1 == MODE172_4W
            || m_nMode1 == MODE176_4W_CV || m_nMode1 == MODE177_4W_CV
            || m_nMode1 == MODE178_4W_CV || m_nMode1 == MODE179_4W_CV)//sylee180221-1 //sylee200217-4WCVMODE
        {
            Sub71.Delay_100ns(10000); // 1mS
            for (int k12 = 1; k12 <= 1; k12++)
            {
                for (int RList1 = 1; RList1 <= 10; RList1++)
                {
                    a1 = (int)RJigData1[RList1][2] - 1;        //son 4W VB TR Pin
                    a2 = (int)RJigData1[RList1][3] - 1;        //son 4W VB RY Pin 
                    a3 = (int)RJigData1[RList1][4] - 1;        //son 4W VC TR Pin
                    a4 = (int)RJigData1[RList1][5] - 1;        //son 4W VC RY Pin

                    if (m_bUseLowerCalJig) {   //son220331
                        a1 += (SysSet12.m_nLowerJigStartPin - 1);
                        a2 += (SysSet12.m_nLowerJigStartPin - 1);
                        a3 += (SysSet12.m_nLowerJigStartPin - 1);
                        a4 += (SysSet12.m_nLowerJigStartPin - 1);
                    }   //son220331

                    Sub71.AOnPinSet1(2, a3, 4);
                    Sub71.Delay_100ns(DEF_4W_PIN_DELAY1);
                    Sub71.AOnPinSet1(2, a2, 3);
                    Sub71.Delay_100ns(DEF_4W_PIN_DELAY2);
                    Sub71.AOnPinSet1(2, a4, 2);
                    Sub71.Delay_100ns(DEF_4W_PIN_DELAY3);
                    Sub71.AOnPinSet1(2, a1, 1);
                    Sub71.Delay_100ns(nTimeDelay1);
                    Sub71.AOnReadV1I1(5);
                    Sub71.OnPinAllReset1();
                }
            }

        }
        //SYLEE180430-1 Sub71.Delay_100ns(10000); // 1mS


        int nLoop2;//sylee201030-3
        double dTW1[3], dTY1[3];//sylee201030-3
        for (meas = 1; meas <= nMeasCount; meas++)  //count
        {

            nLoop = 3;    //ACE380 //SYLEE150211      //sylee2010 1 -> 3

            for (RList = nStart; RList < (nStart + nRCount); RList++) {
                dR1[meas][RList] = 0.0;
            }

            for (RList = nStart; RList < (nStart + nRCount); RList++)
            {

                for (k = 0; k < Kcount; k++)    //sylee131230-ray-test
                {
                    ::ZeroMemory(&dTW1, sizeof(dTW1));//sylee201030-3
                    ::ZeroMemory(&dTY1, sizeof(dTY1));//sylee201030-3
                    nLoop2 = 0;//sylee201030-3

                Task1:
                    if (nStop == 2) {
                        AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
                        goto task_EXIT;	//son230210_4 task2 -> task_EXIT
                    }

                    //son 먼지제거 기능:
                    //    핀설정 방향을 바꿔가면서 테스트해서 전기극성때문에 붙었던 먼지를 제거
                    if (nLoop2 == 0) {//sylee201030-3
                        a1 = (int)RJigData1[RList][1 + 1] - 1;    //SYLEE220125  //son 4W VC RY Pin
                        a2 = (int)RJigData1[RList][4 + 1] - 1;    //SYLEE220125  //son 4W VB TR Pin
                        a3 = (int)RJigData1[RList][2 + 1] - 1;    //SYLEE220125  //son 4W VC TR Pin
                        a4 = (int)RJigData1[RList][3 + 1] - 1;    //SYLEE220125  //son 4W VB RY Pin
                    }
                    else {//sylee201030-3
                        a2 = (int)RJigData1[RList][5] - 1;
                        a1 = (int)RJigData1[RList][2] - 1;
                        a4 = (int)RJigData1[RList][4] - 1;
                        a3 = (int)RJigData1[RList][3] - 1;
                    }//sylee201030-3

                    if (m_bUseLowerCalJig) {   //son220331
                        a1 += (SysSet12.m_nLowerJigStartPin - 1);
                        a2 += (SysSet12.m_nLowerJigStartPin - 1);
                        a3 += (SysSet12.m_nLowerJigStartPin - 1);
                        a4 += (SysSet12.m_nLowerJigStartPin - 1);
                    }   //son220331

                    Sub71.AOnPinSet1(2, a1, 2);    //Vb  TR PinNo(0~8k)   //sylee200220-1 ray  //sylee201030
                    Sub71.Delay_100ns(DEF_4W_PIN_DELAY3);
                    Sub71.AOnPinSet1(2, a2, 1);    //Vc  TR PinNo(0~8k)  
                    Sub71.Delay_100ns(DEF_4W_PIN_DELAY3);
                    Sub71.AOnPinSet1(2, a3, 4);    //VC  RELAY PinNo(0~8k) 
                    Sub71.Delay_100ns(DEF_4W_PIN_DELAY2);
                    Sub71.AOnPinSet1(2, a4, 3);    //Vc  RELAY PinNo(0~8k) 

                    Sub71.Delay_100ns(nTimeDelay1);

                    //sylee190430-1 if(nLoop==1){//SYLEE150211 RAY  
                    //sylee190430-1     Sub71.Delay_100ns(nTimeDelay1); 
                    //sylee190430-1 }                    

                    Sub71.AOnReadV1I1(5); //sylee180221                     
                    double nT1, nT2;  //4WSETUP140107
                    nT1 = nISet - nISet * 0.1;
                    nT2 = nISet + nISet * 0.1;
                    if (nT1 < Sub71.Y1) {
                        if (Sub71.Y1 < nT2) {
                            if (m_nMode1<MODE176_4W_CV || m_nMode1>MODE_4W_END) {    //sylee200217-4WCVMODE    //sylee200515-patch
                                                                //son220103  176:MODE176_4W_CV,  180: MODE_4W_END
                                Sub71.Y1 = nISet; //SYLEE140107 //SYLEE171024-1 I FIX RAY
                            }
                        }
                    }
                    Sub71.OnPinAllReset1(); ///PIN ALL RESET 

                    if (Sub71.Y1 < 0.0000001) {
                        Sub71.Y1 = 0.0000001;
                    }
                    if (nLoop > 0) {  //sylee20111216  사장님
                        RList = 10;//syle201030
                        if (nLoop == 1) {
                            RList = 1;  //sylee130721                            
                            //sylee201030-3}else{//sylee20120129
                            //sylee201030-3 RList++;
                        }
                        nLoop--;
                        goto Task1;
                    }

                    /////////////////////////////////////////////////////////////////////////
                    //sylee201030-3
                    dTW1[nLoop2] = Sub71.W1;
                    dTY1[nLoop2] = Sub71.Y1;//sylee201030-3
                    nLoop2++;//sylee201030-3
                    if (nLoop2 < 2) {  //  2 =count 2  //sylee201102
                        goto Task1;
                    }
                    Sub71.W1 = (dTW1[0] + dTW1[1]) / 2.;
                    Sub71.Y1 = (dTY1[0] + dTY1[1]) / 2.;
                    /////////////////////////////////////////////////////////////////////////

                    if (Sub71.Y1 > 0) {
                        if (nIR == 1 || nIR == 2) {//I mA                         
                            Sub71.R1 = ((Sub71.W1) / Sub71.Y1) * 10.;//sylee  //sylee161207-0 x10    ray ok
                        }
                        else if (nIR == 3 || nIR == 4 || nIR == 5) {  //I uA                             
                            Sub71.R1 = ((Sub71.W1) / Sub71.Y1) * 1000;//배율 /1000                                  
                        }
                        else {
                            Sub71.R1 = 9999999999;
                        }
                    }
                    else {
                        Sub71.R1 = 9999999999;
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////

                    Sub71.R1 = Sub71.R1 * 1000;

                    if (k == 0) {//sylee130508 
                        dR1[meas][RList] = 0;
                        dVC1[meas][RList] = 0;
                        dVB1[meas][RList] = 0;
                        dI1[meas][RList] = 0;
                    }

                    dR1[meas][RList] = dR1[meas][RList] + Sub71.R1;   //R1; 
                    dVC1[meas][RList] = dVC1[meas][RList] + Sub71.W1;  //vc  //sylee130508
                    dVB1[meas][RList] = dVB1[meas][RList] + Sub71.V1;  //vb   //sylee130508
                    dI1[meas][RList] = dI1[meas][RList] + Sub71.Y1;   //RList      //sylee130508


                }////for END   for( RList=0; RList<64; RList++){


                if (n4Wire != 4) {//sylee140618
                    Sub71.Delay_100ns(100000);//sylee130709 //test 10ms
                }

            }//  for( k=0; k<10; k++){

            for (RList = nStart; RList < (nStart + nRCount); RList++) {
                dR1[meas][RList] = dR1[meas][RList] / (float)Kcount;//sylee131227kcount;   //R1;
                dVC1[meas][RList] = dVC1[meas][RList] / (float)Kcount;//sylee131227kcount;     //vc  
                dVB1[meas][RList] = dVB1[meas][RList] / (float)Kcount;//sylee131227kcount;   //vb   //sylee130508
                dI1[meas][RList] = dI1[meas][RList] / (float)Kcount;//sylee131227kcount;    //RList      //sylee130508    
            }

        } //for END  for( meas=0; meas< nMeasCount; meas++){  //count

    }

    //--------------
    // 2W  SET
    //--------------
    else
    {
        ::ZeroMemory(&dR1, sizeof(dR1));    // hdpark220812
        int nLoop2;//sylee180110-1

        nLoop2 = 0;//sylee180110-1

// hdpark230105 spark cal begin

		CString strFile;
		strFile.Format("D:\\LOG\\SPARK2\\spark_%d.txt", m_nMode1);
		fp_vb = fopen(strFile, "w+t");

        if ((m_nMode2 == R_SPARK) && (m_nMode1 >= MODE221_LEAK_40V))    //son spark mode 전용.
        {
			memset(dSparkV2, 0, sizeof(dSparkV2));
			memset(dSparkI2, 0, sizeof(dSparkI2));

            //int nSparkset = SysSet19.m_nSet35 + SysInfo05.m_nSparkSet; // spark offset + ref
            int nSparkset = SysSet19.m_nSet35; // spark offset
            Sub71.OnSparkADC(1);
            Sub71.AOnVBSet1(5, nSparkset);
            Sub71.OnSparkOn(0);
            Sub71.Delay_100ns(10000);
            Sub71.OnSparkOn(1);

			Sub71.m_bModifySparkMeas = TRUE;
			Sub71.m_bVBReadMode = TRUE;
            Sub71.SetVBReadMode();
            Sub71.Delay_100ns(50000);
            Sub71.SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_ON);    // VB Mode Set
            nStart = 1;
			nRCount = 32;
        }
        double dSparkMaxValue = 0.0;

// hdpark230105 cal end

        for (meas = 1; meas <= nMeasCount; meas++)  //count
        {
            nLoop2++;//sylee180110-1
            nLoop = 5; //sylee20120314  //son 첫번째 5개 정도의 R을 측정한 후에는 다시 처음부터 측정(Task7) count

            Sleep(1);//sylee161212-1

 

            for (RList = nStart; RList < (nStart + nRCount); RList++)
            {
Task7:
                if (nStop == 2) {
                    AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
                    goto task_EXIT;	//son230210_4 task2 -> task_EXIT로 명시적으로 수정
                }


                /*//SYLEE200908

                  if( nVSet>=100 ){//sylee150630
                  Sub71.Delay_100ns(3000);//sylee150630  300uS
                  }

                  if(RList<=32){
                  a1=(int)RJigData1[RList][8]-1;   //8,9 (1~1k)   12,13,(1k~1M)   16,17(1M~200M)
                  a2=(int)RJigData1[RList][9]-1;
                  }else if( RList<=64){
                  a1=(int)RJigData1[RList-32][12]-1;   //8,9 (1~1k)   12,13,(1k~1M)   16,17(1M~200M)
                  a2=(int)RJigData1[RList-32][13]-1;
                  }else{
                  a1=(int)RJigData1[RList-64][16]-1;   //8,9 (1~1k)   12,13,(1k~1M)   16,17(1M~200M)
                  a2=(int)RJigData1[RList-64][17]-1;
                  }
                  if (m_bUseLowerCalJig) {   //son220331
                      a1 += (SysSet12.m_nLowerJigStartPin -1);
                      a2 += (SysSet12.m_nLowerJigStartPin -1);
                  }   //son220331

                  Sub71.AOnPinSet1(2 ,a1, 2);    //ACE380-JIGNEW-1//  PinNo(0~8k) nMod =1,VB,    =2VC,
                  Sub71.AOnPinSet1(2 ,a2, 1);    //ACE380-JIGNEW-1//  PinNo(0~8k)    nMod =1,VB,    =2VC,

                  dPinVB[RList]= a1 ;//ACE380-JIGNEW-1
                  dPinVC[RList]= a2 ;  //ACE380-JIGNEW-1

                //task1:
                if( nVSet>20 ){
                if( FastHR==1 ){ //SYLEE20120711-2
                Sub71.AOnReadV1I1_ADC_I();
                }else{
                Sub71.AOnReadV1I1(nVRel);
                }

                if( m_nMode1>=16 && m_nMode1<=155){ //HVOPENSET //sylee130618


                }else{

                if( Sub71.Y1> InterI){
                Sub71.OnPinAllReset1(); ///PIN ALL RESET

                if(nVRel<3){
                str.Format("Error No 13301,\n\n   lv short    \n\n   I=%.2f   \n  ", Sub71.Y1);
                }else{
                str.Format("Error No 13301,\n\n   hv short    \n\n   I=%.2f   \n  ", Sub71.Y1);
                }
                AfxMessageBox(str);

                goto TaskInterR101;

                }

                }

                //SYLEE20120623 Sub71.OnDisChargeuS(50);
                }

                //SYLEE200908  */

                //############################################################################################################33



                int nLoop1;		//son 첫번째 R은 2회 측정하기 위한 count

                nLoop1 = 0;


Task11:
                //================
                // 핀 설정 
                //================


                Sub71.OnPinAllReset1(); ///PIN ALL RESET  


                if (RList <= 32)
                {
                    a1 = (int)RJigData1[RList][8] - 1;
                    a2 = (int)RJigData1[RList][9] - 1;
                }
                else if (RList <= 64) {
                    a1 = (int)RJigData1[RList - 32][12] - 1;
                    a2 = (int)RJigData1[RList - 32][13] - 1;
                }
                else {
                    a1 = (int)RJigData1[RList - 64][16] - 1;
                    a2 = (int)RJigData1[RList - 64][17] - 1;
                }

                if (m_bUseLowerCalJig) {   //son220331
                    a1 += (SysSet12.m_nLowerJigStartPin - 1);
                    a2 += (SysSet12.m_nLowerJigStartPin - 1);
                }   //son220331

 
 

				if ((m_nMode2 == R_RAW || m_nMode2 == R_REAL) //son230214
						&& m_nMode1 < MODE356_SPARK_50V)	// hdpark230105 spark cal
				{
				//	Sub71.AOnPinSet1(2, a1, 2);    //ACE380-JIGNEW-1//  PinNo(0~8k) nMod =1,VB,    =2VC,
				//	Sub71.AOnPinSet1(2, a2, 1);    //ACE380-JIGNEW-1//  PinNo(0~8k)    nMod =1,VB,    =2VC,  
	               
					
					
					Sub71.AOnPinSet1(2, a2, 1);  
					Sub71.AOnPinSet1(2, a1, 2);    
			 	
					dPinVB[RList] = a1;
					dPinVC[RList] = a2;
				}// hdpark230105 spark cal begin
                else if ((m_nMode2 == R_SPARK) && (m_nMode1 >= MODE221_LEAK_40V)) //son230214
                {
                    Sub71.Delay_100ns(nTimeDelay1);

					// jig
					//a1 = 17 - 1;
					//a2 = 25 - 1;

					Sub71.AOnPinSet1ForSparkCal(2, a1, 1);
					Sub71.AOnPinSet1ForSparkCal(2, a2, 2); 

					dPinVB[RList] = a1;
					dPinVC[RList] = a2;

					int nAdcReadCount = Sub71.nADCount1 + 40;
                    Sub71.ReadSparkForCal(nAdcReadCount);
					if (Sub71.OnSparkStatus() != 1) 
						continue;

                    if (nSparkOnFlag)
                    {
                        Sub71.OnSparkOn(0);
                        Sub71.Delay_100ns(20000);
                        Sub71.OnSparkOn(1);
                    }

                    int nMaxVsIndex = 0;
                    for (int i = 0; i < nAdcReadCount; i++)
                    {
						if(fp_vb)	fprintf(fp_vb, "%.3f, %.3f, \r\n", dSparkV2[i], dSparkI2[i]);

                        if (i == 0)
                        {
                            dSparkMaxValue = dSparkV2[i];
                        }
                        else
                        {
                            if (dSparkMaxValue < dSparkV2[i])
                            {
                                dSparkMaxValue = dSparkV2[i];
                                nMaxVsIndex = i;
                            }
                        }
                    }
					if(fp_vb)	fprintf(fp_vb, "\r\n");
					//nSparkHWoffSet1 = SysSet19.m_nSet35;
					//nSparkRefV = SysInfo05.m_nSparkSet;
					//double dRealSparkValue = dSparkMaxValue - nSparkHWoffSet1;
					//if(dRealSparkValue < nSparkRefV)	dRealSparkValue = 0;	
					dR1[meas][RList] = dSparkMaxValue;	// dSparkI2[nMaxVsIndex]; VB Read

                    ///Sub71.SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);    // VB Mode reset
					continue;
                }
				else
				{
					continue;
				}
                // hdpark230105 spark cal end

                //############################################################################################################33

                if (nFlagQCMode1 == 1) {//sylee121007                    
                    Sub71.AOnISetIncre1(nISet, 100, 100);   //SYLEE121008   // I set, count, uSec delay   
                    if (m_nMode1 == MODE181_SELF_10V) {//10v //sylee130618       //son220103 181: MODE181_SELF_10V
                        Sub71.ADCI_Read_Wait1(2, 1, 5);//sylee121007
                    }
                    else if (m_nMode1 == MODE182_SELF_200V) { //200v  //sylee130618 //son220103 182: MODE182_SELF_200V
                        Sub71.ADCI_Read_Wait1(1, 1, 5);//sylee121007
                    }
                }
                else {
                    Sub71.Delay_100ns(nTimeDelay1);
                }

                //================
                // R  READING
                //================
                if (n4Wire == 4) { //SYLEE20111118
                    Sub71.AOnReadV1I1(5); // READ  4W V  , I 

                    double nT1, nT2;  //4WSETUP140107

                    nT1 = nISet - nISet * 0.1;
                    nT2 = nISet + nISet * 0.1;

                    if (nT1 < Sub71.Y1) {
                        if (Sub71.Y1 < nT2) {
                            Sub71.Y1 = nISet; //SYLEE140107 //SYLEE171024-1 I FIX RAY
                        }
                    }
                }
                else
                {
                    if (nFlagQCMode1 == 1) {//sylee121007 
                    }
                    else
                    {
                        int ik1;  //son231005_2
                        //------------------
                        //son Short 일때
                        if (FastHR == 1) //SYLEE20120711-2
                        {
                            //sylee200917-1 Sub71.AOnReadV1I1_ADC_I();//SYLEE20120711-2

                            if (m_nMode1 < MODE356_SPARK_50V) // hdpark230105 spark cal
                            {
                                //////////////////////////////////////////////////////////////////////                                     
                                nNo1 = 20;//sylee200917	//son 20회 read해서 평균을 낸다.
                                nADSum1 = 0.0;
                                // ::ZeroMemory(&nAD, sizeof(nAD));
                                for (ik1 = 1; ik1 <= nNo1; ik1++) {
                                    Sub71.AOnReadV1I1_ADC_I();
                                    //Sub71.Delay_100ns(5000); //500 uS
                                    //son231005_2 read해서 평균내는 시간을 16.7msec 주기의 배수로 맞추어 달라는 김영필이사님 요구사항.
                                    Sub71.Delay_100ns(4860); //486 uSec	//son231005_1 5000-> 4860.   
                                                        // AOnReadV1I1_ADC_I()에서 349 uSec 소요. +486하면 835 uSec. 
                                                        // 0.835 * 20회= 16.7 msec total read 
                                                                        
                                    //son short는 dIADC1이 Sub71.R1 이어야 한다. 
                                    nADSum1 = nADSum1 + dIADC1; //son dIADC1 from Sub71.AOnReadV1I1_ADC_I()
                                                                //son231012 int nIADC1  -> double dIADC1
                                }
                                //son231006 dIADC1 = (int)(nADSum1 / nNo1);    
                                Sub71.R1 = nADSum1 / nNo1;   //son231006 나누기  20하는데 type이 int여서 소수점 이하 잘리는 버그수정

                                //son230209 short data 파일 출력에 사용하기 위해 Sub71.Y1 기록 (short의 dIADC1값 raw data)
                                Sub71.Y1 = Sub71.R1;  //son231006 //son230209
                                //////////////////////////////////////////////////////////////////////      
                            }
                        }
                        //------------------
                        //son Open 일때
                        else
                        {
                        //son231005_2 if (nCC == 2) {//cv   
                        //son231005_2     Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ               
                        //son231005_2 }
                        //son231005_2 else// cc
                        //{ //son231005_2 OPEN CV도 OPEN CC와 똑같이 20회 read해서 평균내어 달라는 김영필 이사님 요구사항.
                            //sylee200917  Sub71.AOnReadV1I1(nVRel); // CC MODE   //  V I READ

                            //////////////////////////////////////////////////////////////////////                                     
                            nNo1 = 20;//sylee200917		//son 20회 read해서 평균을 낸다.
                            nADSum1 = 0.0;
                            ::ZeroMemory(&nAD, sizeof(nAD));
                            for (ik1 = 1; ik1 <= nNo1; ik1++) 
                            {
                                //son231005_2 OPEN CV도 OPEN CC와 똑같이 20회 read해서 평균내어 달라는 김영필 이사님 요구사항. 
                                if (nCC == __CV)//son231005_3 2:cv      //son231005_2
                                    Sub71.AOnReadV1I1(7);  //son 7: Sub71.Y1(전류)만 Read. 전압값 X1, W1은 0으로 리턴  //son231005_2

                                else //cc       //son231005_2
                                    Sub71.AOnReadV1I1(nVRel);

                                //Sub71.Delay_100ns(5000 * 4); // 2 msec
                                //son231005_1 read해서 평균내는 시간을 16.7msec 주기의 배수로 맞추어 달라는 김영필이사님 요구사항.
                                Sub71.Delay_100ns(16700); //1.67 msec	//son231005_1 20000-> 16700. 
                                                                        //          1.67 * 20회= 33.4msec total read time  
                                nAD[ik1][0] = Sub71.X1;
                                nAD[ik1][1] = Sub71.Y1;
                            }
                            //////////////////////////////////////////////////////////////////////
                        //} //son231005_2

                            //son231005_2 begin:
                            //--------------------------------
                            //son Sub71.X1, Sub71Y1 평균내기 
                            for (ik1 = 1; ik1 <= nNo1; ik1++) {//sylee200917                                  
                                nAD[0][0] = nAD[0][0] + nAD[ik1][0];
                                nAD[0][1] = nAD[0][1] + nAD[ik1][1];
                            }
                            Sub71.X1 = nAD[0][0] / nNo1;
                            Sub71.Y1 = nAD[0][1] / nNo1;
                            //son231005_2 end

                        }

                    }

                }// if( n4Wire==4 ){    }else{  //SYLEE20111118 

				//son 첫번째 R 은 2회 측정한다.
                if (RList == nStart && nLoop1 == 0) {//sylee140728
                    //  Sleep(500);
                    nLoop1++;
                    goto Task11;
                }

                //sylee200908 TaskInterR101://sylee20120602

 
				nRet1 = Sub71.OnPinAllReset1(); ///PIN ALL RESET  
		     

                if (nRet1 != 1) {
                    AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
                    return;
                }

                if (Sub71.Y1 < 0.0000001) {
                    Sub71.Y1 = 0.0000001;
                }
                if (nFlagQCMode1 == 1 && m_nMode1 == MODE182_SELF_200V) { //200v){//sylee130618   //son220103 182:MODE182_SELF_200V

                }
                else //sylee121007
                {
                    //-------------------
                    //son short 이면
                    if (FastHR == 1) { //SYLEE20120711-2
                        //son231006 나누기  20하는데 type이 int여서 소수점 이하 잘리는 버그수정
						//son2301006 Sub71.R1 = dIADC1;//ADC 
						
						Sub71.X1 = 0;
						Sub71.V1 = 0; //vb
						Sleep(10);
                    }
                    else
                    {
                        //---------------
                        //son 4W 이면
                        if (n4Wire == 4)   //SYLEE20120102 
                        {
                            if (Sub71.Y1 > 0) {
                                if (nIR == 1 || nIR == 2) {//I mA                                         
                                    Sub71.R1 = ((Sub71.W1) / Sub71.Y1);//배율 /1000   4W=>  4W VOLTAGE                                   
                                }
                                else if (nIR == 3 || nIR == 4 || nIR == 5) {  //I uA    
                                    Sub71.R1 = ((Sub71.W1) / Sub71.Y1) * 1000.;//배율 /1000
                                }
                                else {
                                    Sub71.R1 = 9999999999;
                                }
                            }
                            else {
                                Sub71.R1 = 9999999999;
                            }
                        }

                        //---------------
                        //son OPEN 이면
                        else
                        {
                            if (nCC == __CV)//son231005_3 2:CV   r=(vb/RList) 
                            {
                                if (nIR == 1 || nIR == 2) {
                                    Sub71.R1 = (Sub71.V1) / Sub71.Y1 * 1000.0;
                                }
                                else if (nIR == 3 || nIR == 4 || nIR == 5) {
                                    Sub71.R1 = (Sub71.V1) / Sub71.Y1 * 1000000.0;
                                }
                            }
                            else //son231005_3 if (nCC == __CC) // 1:CC    r= (vb-vc)/RList
                            {
                                if ((nFil == 3) && (m_nMode1 >= MODE16_OPEN_30V && m_nMode1 <= MODE155_OPEN_300V)) //HVOPENSET  //SYLEE130724 
                                {                         //son220103 16: MODE16_OPEN_30V, 155: MODE155_OPEN_300V
                                    if (nIR == 1 || nIR == 2) {
                                        Sub71.R1 = (Sub71.X1) / Sub71.Y1 * 1000.0;
                                    }
                                    else if (nIR == 3 || nIR == 4) {
                                        //SYLEE200908 RAY  Sub71.R1=(Sub71.X1)/Sub71.Y1*1000000.0;  // china kushan samsung hogi 1,2   //sylee121029
                                        Sub71.R1 = (Sub71.X1) / Sub71.Y1 * 1000.0;  // //SYLEE200908 RAY 
                                    }
                                    else if (nIR == 5) { //SYLEE121124                                                  
                                        Sub71.R1 = (Sub71.X1) / Sub71.Y1 * 1000.0;//sylee121027  #define#3hogifinal#   china kushan samsung hogi 3                         
                                    }

                                    if (Sub71.X1 > 325) {//sylee150126
                                        Sub71.R1 = 999999999.0;
                                    }
                                }
                                else
                                {
                                    if (nIR == 1 || nIR == 2) {
                                        Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000.0;
                                    }
                                    else if (nIR == 3 || nIR == 4 || nIR == 5) {
                                        Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000000.0;
                                    }
                                }
                            }
                        }
                    }
                }////sylee121007if(nFlagQCMode1==1){//sylee121007


                if (Sub71.R1 > nLeakoffset1) {
                    Sub71.R1 = Sub71.R1 - nLeakoffset1;
                }

                ////////////////////////////////////////////////////////////////////////////////////////
                if (n4Wire == 4) {
                    Sub71.R1 = Sub71.R1 * 1000;
                }

                //SYLEE120913
                //son Cal은 Raw (ADC)로 측정함
                if (m_nMode2 == R_REAL) {   //son 2: Real R  <== 여기는 보통 수행이 안 된다.   //son230214
                    //RCalCovert( m_nMode1, Sub71.R1  , 1 );   // m_nMode1 1~76   ,  ConMode=1  (raw ->real),      2 (real->raw)
                    RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL, part);   // m_nMode1 1~76    //son220412_3 
                }

                dR1[meas][RList] = Sub71.R1;   //R1;    //son raw ADC R이다. real R 아님. 

                if (n4Wire == 4) {
                    dVC1[meas][RList] = Sub71.W1;  //vc
                }
                else {
                    dVC1[meas][RList] = Sub71.X1;  //vc
                }

                dVB1[meas][RList] = Sub71.V1;  //vb
                dI1[meas][RList] = Sub71.Y1;   //RList

                //TEMP  Sub71.Delay_100ns(10000);  //1ms   //SYLEE20120111

                //  if( (meas==1)  && (nLoop >0)){   
                //son 첫번째 5개 정도의 R을 측정한 후에는 다시 처음부터 측정하는 기능 (AMP를 가열하기 위해?)
 

                if (nLoop > 0)
                {
                    if (nLoop == 1)
                    {
                        if (nVSet > 20) {
                            //sylee130618 if( m_nMode1>=51 && m_nMode1<=59){ //HVOPENSET  //SYLEE20120421   
                            if (m_nMode1 >= MODE16_OPEN_30V && m_nMode1 <= MODE155_OPEN_300V) { //HVOPENSET  //SYLEE130618        
                                                                //son220103 16: MODE16_OPEN_30V, 155: MODE155_OPEN_300V
                                RList = 1;
                            }
                            else {
                                //sylee130721   RList=65;                               
                                if (m_nMode1 < MODE216_LEAK_30V) {//sylee140930-1  //son220103 216:MODE216_LEAK_30V
                                    RList = 1;
                                }
                                else {//sylee140930-1
                                    //ACE380-JIGNEW-1//SYLEE150123   RList=65;//sylee140930-1                                    
                                    RList = nStart;//ACE380-JIGNEW-1//SYLEE150123
                                }//sylee140930-1                             
                            }
                        }
                        else {
                            RList = 1;
                        }
                    }
                    else {//sylee20120129
                        RList++;    //son goto Task7하면 RList가 증가하지 않을 수 있어서 여기서 꼭 증가시켜야 함.
                    }

                    nLoop--;
                    goto Task7;
                }

                if ((m_nMode2 == R_RAW)  //son230209_2 //son Sub71.R1이 Real이 아닐 때 (raw ADC일 때) //son230214
                    && (m_nMode1 > MODE216_LEAK_30V)) //sylee140925   //son220103 216:MODE216_LEAK_30V
                {
					if (m_nMode1 < MODE356_SPARK_50V)	// hdpark230105
					{
						//son ADC raw R 이 300보다 작으면 (short이면 I값이므로 작을 수록 고저항 )
						//son230209_2 // if (Sub71.R1 < 300) {   
						if (Sub71.R1 < 400) {   //son230209_2 김영필이사님요청. short 50 Mohm면 ADC 400 정도 측정됨.
							Sleep(200);         //         short 50 Mohm부터는 천천히 측정하도록
						}
					}	// hdpark230105 begin
					else
					{
						Sleep(1);
					}
					// hdpark230105 end
                }

            }////for END   for( RList=0; RList<64; RList++)

            //Sleep(500);    //sylee180130 
            Sleep(1000);    //1000 msec //son230210_1 500 -> 1000  requested by RAY

            //SYLEE200908if(nLoop2==1){//sylee180110-1
            //SYLEE200908   meas=meas-1;//sylee180110-1
            //SYLEE200908}

        } //for END  for( meas=0; meas< nMeasCount; meas++)  //count

    }  //else{  //if( n4Wire==4 )


task_EXIT:  //STOP JUMP POSTION	 //son230210_4 task2 -> task_EXIT로 명시적으로 수정


    tStart1 = GetMilSecond2();
    nT[1] = (double)(tStart1 - tStart2) / 1000.;  //test


    ///////////////////////////////////////////////////////////////////////
    //화면 DISPLAY


    //sum
    for (meas = 1; meas <= nMeasCount; meas++)  //count 
    {
        for (RList = 1; RList <= MAX_CAL_RLIST; RList++) {  //son220408 MAX_CAL_RLIST:128

            if (dR1[meas][RList] < 0.0) {
                str.Format("N/A");
            }
            else {
                str.Format("%.2f", dR1[meas][RList]);
            }

            // Grid CAL_TEST1 ~ CAL_TEST10 영역에 dR1[meas][RList] 출력
            m_grid1.SetTextMatrix(RList, meas + (CAL_TEST1 - 1), str);//sylee20120114     //son220407 (CAL_TEST1 -1):5 

            dR1[0][RList] = dR1[0][RList] + dR1[meas][RList];//sum
            dVC1[0][RList] = dVC1[0][RList] + dVC1[meas][RList];//VC sum
            dI1[0][RList] = dI1[0][RList] + dI1[meas][RList];//IC sum
        }

    }



    //==========================================================
    //ave

    for (RList = 1; RList <= MAX_CAL_RLIST; RList++)  //son220408 MAX_CAL_RLIST:128
    {
        dR1[0][RList] = dR1[0][RList] / nMeasCount;
        dVC1[0][RList] = dVC1[0][RList] / nMeasCount;  ////R sum AVE
        dI1[0][RList] = dI1[0][RList] / nMeasCount;  ////R sum AVE

        if (dR1[0][RList] < 0.0) {
            str.Format("N/A");
        }
        else {

#if 0   //son231006_2 Short Test 결과를 cal 화면 출력할 때 소수점이하 잘리는 문제 수정.
            if (m_nMode1 > MODE_4W_END) {//sylee150126     //son220103 180: MODE_4W_END
                //str.Format("%.0f", dR1[0][RList]);  //sylee150126
				if(m_nMode2 == R_SPARK)	
				    str.Format("%.1f", dR1[0][RList]); // hdpark230105 spark cal
				else				
				    str.Format("%.0f", dR1[0][RList]);
            }
            else 
#endif
            {
                str.Format("%.2f", dR1[0][RList]);
            }
        }


        //////////////////////////////////////////////////////////////////

        StringComa1(str);

        m_grid1.SetTextMatrix(RList, CAL_TEST_AVG, str);     //son CAL_TEST_AVG: 4 
    }


    //==========================================================================
    // meas=     R1[0][RList] ave    dR1[11][RList] Max,   dR1[12][RList] Min     dR1[13][RList] dif  value    ,dR1[14][RList] dif rate

    double temp1;

    for (RList = 1; RList <= MAX_CAL_RLIST; RList++)  //son220408 MAX_CAL_RLIST:128
    {
        for (meas = 1; meas <= nMeasCount; meas++)  //count 
        {
            if (meas == 1) {
                temp1 = dR1[meas][RList];
                dR1[11][RList] = dR1[meas][RList];//max
                dR1[12][RList] = dR1[meas][RList];//min

            }
            else {
                if (dR1[meas][RList] < dR1[12][RList]) {
                    dR1[12][RList] = dR1[meas][RList];//min
                }

                if (dR1[meas][RList] > dR1[11][RList]) {
                    dR1[11][RList] = dR1[meas][RList];//max
                }
            }
        }

        dR1[13][RList] = dR1[11][RList] - dR1[12][RList];//difference value

        if (dR1[0][RList] == 0) {//if ave =0
            dR1[14][RList] = 0.0;
        }
        else {
            dR1[14][RList] = (dR1[13][RList] / dR1[0][RList]) * 100;  //difference  rate
        }

        if (dR1[14][RList] < 0.0) {
            str.Format("N/A");
        }
        else {
            str.Format("%.2f Rate", dR1[14][RList]);//sylee150126                    
        }

        m_grid1.SetTextMatrix(RList, CAL_DIFF_RATE, str);//sylee20120114    //son CAL_DIFF_RATE: 5 

    }

    ///////////////////////////////////////////////////////////////////////
    // FILE LOGGGING

    CTime curTime = CTime::GetCurrentTime();
    fprintf(fp, "<CALIBRATION>\n  ");
    //      fprintf(fp, "#MakeDate:, Year%04d:,Month%02d:,Day%02d,%02dH:%02dM:%02dS, \n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
    fprintf(fp, "#   %04d /  %02d /  %02d  -   %02d :   %02d : %02d  , \n\n\n", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

    fprintf(fp, ">SetTime:, %.3f , mS   \n  ", nT[0]);
    fprintf(fp, ">RunTime:, %.3f , mS   \n ", nT[1]);
    fprintf(fp, "   ,   ,NO,*VbPin , *VcPin  , ");
    fprintf(fp, " *Max R, *Min R,*Avg.,*Dif., Dif.Rate ,  , Ref R,  ,");//sylee20120114

    for (meas = 1; meas <= nMeasCount; meas++)       //son220106               
        fprintf(fp, "R%d, ", meas);

    for (meas = 0; meas < nMeasCount; meas++) {
        fprintf(fp, "  , Count, VB, VC, I, R, ");
    }

    fprintf(fp, "   \n ");

    int kk;//sylee20111215

    if (nVSet <= 20) {//LV
        kk = 1;
    }
    else {
        if (m_nMode1 >= MODE16_OPEN_30V && m_nMode1 <= MODE155_OPEN_300V) { //HVOPENSET  //SYLEE130618  //son220103 155:MODE155_OPEN_300V 
            kk = 1;
        }
        else {
            kk = 1;
        }
    }

    for (RList = kk; RList <= MAX_CAL_RLIST; RList++)//sylee20111215 //son220408 MAX_CAL_RLIST:128
    {
        //son  No,   *VbPin,  *VcPin
        if (RList < 65) {
            fprintf(fp, "             ,        ,%d,%d,%d,", RList, dPinVB[RList], dPinVC[RList]);
        }
        else {
            fprintf(fp, "              ,        ,%d,%d,%d,", RList - 64, dPinVB[RList], dPinVC[RList]);
        }

        //son        Max,  Min,  Avg,  Diff,  DiffRate,   Ref.R
        fprintf(fp, " %.2f,%.2f, %.2f, %.2f,  %.2f ,  ,", dR1[11][RList], dR1[12][RList], dR1[0][RList], dR1[13][RList], dR1[14][RList]);
        fprintf(fp, " %.1f, ,", SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][RList]);//ref r        //son220407

        for (meas = 1; meas <= nMeasCount; meas++) {
            //son No1 ...  No~10
            fprintf(fp, " %.3f, ", dR1[meas][RList]);
        }
        for (meas = 1; meas <= nMeasCount; meas++) {
            //son         No , VB ,  VC ,  I  ,   R
            fprintf(fp, " ,%d,%.3f, %.3f, %.3f,  %.3f , ", meas, dVB1[meas][RList], dVC1[meas][RList], dI1[meas][RList], dR1[meas][RList]);
        }
        fprintf(fp, " \n");
    }

    fclose(fp);

    Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018

    if (nVRel == 3 || nVRel == 4) {  //  HV //sylee20120817

        Sub71.OnPinAllReset1(); ///PIN ALL NO RESET   
        Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
        Sub71.AOnRelayOffSel(nVRel);//SYLEE20120720  
        Sub71.Delay_100ns(100000); //10mS//sylee20120817 
        Sub71.AOnReadV1I1(nVRel);//SYLEE20111115 

        if (Sub71.X1 > 20) {
            Sub71.Delay_100ns(100000); //10mS//sylee20120817
            Sub71.AOnReadV1I1(nVRel);//SYLEE20111115 
            if (Sub71.X1 > 20) {
                Sub71.Delay_100ns(100000); //10mS//sylee20120817
            }
        }
    }

    Sub71.AOnPoweroff2();//sylee20120702         Sub71.AOnPoweroff();

    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704
    Sub71.AOnReadV1Enable1(5); //sylee130711  
    Sub71.OnCCCV_Set(1);  //sylee200217-4WCVMODE


    // hdpark230105 spark cal begin
    //if (m_nMode1 >= MODE356_SPARK_50V)
    if ((m_nMode2 == R_SPARK) && (m_nMode1 >= MODE221_LEAK_40V))
    {
		if(fp_vb)	fclose(fp_vb);
	    Sub71.SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);    // VB Mode Set

        Sub71.OnSparkADC(0);
        Sub71.OnSparkOn(0);
    }
    // hdpark230105 end

    if (nStop == 2) {
        AfxMessageBox("   R Calibraion   STop!!!");
    }

    return;
}


//son Test -> Ref.AVG
void CModeSer13::OnButton102()  //  copy
{
    int RList;
    CString   str;
    for (RList = 1; RList <= MAX_CAL_RLIST; RList++) { //son220408 MAX_CAL_RLIST:128
      // str.Format(" %.2f", nData[4][RList]);  
        str = m_grid1.GetTextMatrix(RList, CAL_TEST_AVG);      //son220407 4: CAL_TEST_AVG
        m_grid1.SetTextMatrix(RList, CAL_REF_RAWR, str);      //son220407 3: CAL_REF_RAWR (Ref.Avg)
    }
}

void CModeSer13::OnButton108()  //  zero cal copy
{

}

//son m_grid1에 SysInfoSer13.m_nData[][][] 를 출력한다.
void CModeSer13::Onload()  //reset
{
    int col, RList, k;
    CString   str;


    col = 0;  //temp
    k = MAX_CAL_RLIST; //temp     //son220408 MAX_CAL_RLIST:128

    nFlag1 = 1;

	// hdpark230105 spark begin
	int part = 0;

	if(m_nMode2 == R_RAW || m_nMode2 == R_REAL) //son230214
	{
		part = (m_bUseLowerCalJig) ? PART_LOWER : PART_UPPER;    //son220412
	}
	else if(m_nMode2 == R_SPARK) //son230214
	{
		part = PART_SPARK;
	}
	// hdpark230105 spark end


    //son220411 해당 m_nMode1의 데이터만 지워야 하는데 전체를 지우는 문제가 있어서 수정함
    //son220411 ::ZeroMemory(&SysInfoSer13.m_nData, sizeof(SysInfoSer13.m_nData));  
    //son220411 ::ZeroMemory(&gdCal1, sizeof(gdCal1));//sylee20111224   
    ::ZeroMemory(&SysInfoSer13.m_nData[part][m_nMode1], sizeof(SysInfoSer13.m_nData[part][m_nMode1]));  //son220411
    ::ZeroMemory(&gdCal1[part][m_nMode1], sizeof(gdCal1[part][m_nMode1]));                              //son220411

    //son  "C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI" 에서 이전의 Calibration Ref 값을  Load 
    int ret = FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, part);   //son220412
    if (ret > 0)
    {
        //        k=(int)SysInfoSer13.m_nData[part][mode][0][0];

        if (k < 1)
            return;
        if (k > MAX_CAL_RLIST) //son220408 MAX_CAL_RLIST:128
            return;

        //------------------------
        //son Grid에  출력

        //  OnButton103();
        for (RList = 1; RList <= k; RList++)
        {
            for (col = CAL_USE; col <= CAL_REF_RAWR; col++)       //son220407 1:CAL_USE,  3:CAL_REF_RAWR
            {
                if (col == 1) {
                    str.Format("%d", (int)SysInfoSer13.m_nData[part][m_nMode1][col][RList]);
                }
                else {
#if 0     //son231006_3 cal Ref값을 파일에서 읽어서 화면에 로딩할 때, 소수점이하 잘리는 문제점 수정
                    if(m_nMode2 == R_SPARK)	str.Format("%.1f", SysInfoSer13.m_nData[part][m_nMode1][col][RList]); // hdpark230105 spark cal
                    else				str.Format("%.0f", SysInfoSer13.m_nData[part][m_nMode1][col][RList]);
#else
                    str.Format("%.2f", SysInfoSer13.m_nData[part][m_nMode1][col][RList]); //son231006_3
#endif
                }

                StringComa1(str);
                m_grid1.SetTextMatrix(RList, col, str);
            }
        }
    }
}

void CModeSer13::OnButton104() //view list
{
    CString Str;
    Str.Format("%s\\SETUP\\LOGCAL\\001LOGRCal%d.csv", SysInfoView01.m_pStrFilePath1, m_nMode1);  //son210514

    if (SysSet13.m_nSet7 == 1) {//sylee130724
        ::ShellExecute(NULL, "open", "EXCEl.EXE", Str, "NULL", SW_SHOWNORMAL);
    }
    else {
        ::ShellExecute(NULL, "open", "notepad.EXE", Str, "NULL", SW_SHOWNORMAL);
    }
}



void CModeSer13::OnClickOk()
{
    //son220331 하부핀 R 측정상태 save
    FileSysInfo01.LoadSave_UseLowerPin(_SAVE, m_bUseLowerCalJig);

    k = 0;
    CDialog::OnOK();
}

void CModeSer13::SetAttr(int iOpa)
{
    //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}


void CModeSer13::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

    /* switch(pSlider->GetDlgCtrlID())
       {
       case IDC_SLIDER1:
       m_slider1.SetPos(pSlider->GetPos());
       break;
       default:
       break;
       }
       */

    SetAttr(pSlider->GetPos());

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CModeSer13::InitControl()
{
    //  Onload(); 
    //   OnButton103() ;
    OnInit();   //Initclear
    nFlag1 = 0;
}


void CModeSer13::OnDblClickMsflexgrid1()
{
    // TODO: Add your control notification handler code here
    BeginEditing();

}

void CModeSer13::OnEnterCellMsflexgrid1()
{
    // TODO: Add your control notification handler code here
    m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSer13::OnLeaveCellMsflexgrid1()
{
    // TODO: Add your control notification handler code here
    if (OnEditing) {
        EndEditing();
    }
}

void CModeSer13::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y)
{
    // TODO: Add your control notification handler code here
    m_grid1.SetRowSel(m_nCurRow);
    m_grid1.SetColSel(m_nCurCol);
}

void CModeSer13::EndEditing()
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

void CModeSer13::BeginEditing()
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

void CModeSer13::OnSave()
{
    int RList, j;

    CString str;

	// hdpark230105 spark begin
	int part = 0;

	if(m_nMode2 == R_RAW || m_nMode2 == R_REAL) //son230214
	{
		//son Calibration 화면은 하부 사용 모드이면 Cal data를 하부용으로 수집한다.
		part = (m_bUseLowerCalJig) ? PART_LOWER : PART_UPPER;    //son220411
	}
	else if(m_nMode2 == R_SPARK) //son230214
	{
		part = PART_SPARK;
	}
	// hdpark230105 spark end


    if (nFlag1 != 1) {
        AfxMessageBox("  Reload!!!  Save Error");
        return;
    }

    //sylee130618 if( m_nMode1>0 && m_nMode1<=76){
    if (m_nMode1 > 0 && m_nMode1 <= MAX_TEST_MODE)//sylee130618 autocal     //son220103 360: MAX_TEST_MODE
    {
        //son220411 해당 m_nMode1의 데이터만 지워야 하는데 전체를 지우는 문제가 있어서 수정함
        //son220411 ::ZeroMemory(&SysInfoSer13.m_nData, sizeof(SysInfoSer13.m_nData));  
        ::ZeroMemory(&SysInfoSer13.m_nData[part][m_nMode1], sizeof(SysInfoSer13.m_nData[part][m_nMode1]));  //son220411


        //son 현재 Cal 화면의 데이터를 가져 온다.
        for (RList = 1; RList <= MAX_CAL_RLIST; RList++)       //son220408 MAX_CAL_RLIST:128
        {
            //son220407 Commented
            //for( j=1 ; j<=14 ;  j++) {
            //    str=m_grid1.GetTextMatrix(RList, j);  
            //    str.Remove(',');//SYLEE150227
            //    if(j<=3){
            //        SysInfoSer13.m_nData[part][m_nMode1][j][RList]=atof(str);
            //    }
            //    else{
            //        SysInfoSer13.m_nData[part][m_nMode1][j+2][RList]=atof(str);   
            //    }
            //}
            for (j = CAL_USE; j <= CAL_REF_RAWR; j++)    //son220407 1:CAL_USE ~ 3:CAL_REF_RAWR
            {
                str = m_grid1.GetTextMatrix(RList, j);
                str.Remove(',');//SYLEE150227
                SysInfoSer13.m_nData[part][m_nMode1][j][RList] = atof(str);
            }

            SysInfoSer13.m_nData[part][m_nMode1][CAL_VC][RList] = dVC1[0][RList];   //son220407 4:CAL_VC
            SysInfoSer13.m_nData[part][m_nMode1][CAL_I][RList] = dI1[0][RList];    //son220407 5:CAL_I

            for (j = CAL_TEST1; j <= CAL_TEST10; j++)    //son220407 6:CAL_TEST1 ~  15: CAL_TEST10
            {
                str = m_grid1.GetTextMatrix(RList, j);
                str.Remove(',');//SYLEE150227
                SysInfoSer13.m_nData[part][m_nMode1][j][RList] = atof(str);
            }

            if (m_nMode1 == MODE174_4W || m_nMode1 == MODE175_4W)//sylee1710126-3      //son220103 175: MODE175_4W    
            {
#if 0  //son240604  4W cal에서 200000 mohm 이상 write 안 되는 문제  수정
       //           4W는  200000 mohm  까지 측정되는게 맞지만 
       //           4W cal은 200000 mohm 측정 보장을 위해 25번째 500000 mohm 까지만 측정 가능해야 한다.
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][26] = 200000;//sylee1710126-3       //son220407
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][27] = 300000;//sylee1710126-3       //son220407
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_RAWR][27] = 30000;//sylee1710126-3     //son220407
#else
                SysInfoSer13.m_nData[part][m_nMode1][CAL_USE][26] = 0;  //son Not Use로 설정   //son240604
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][26] = 0;//sylee1710126-3       //son240604
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][26] = 0;//sylee1710126-3       //son240604

                SysInfoSer13.m_nData[part][m_nMode1][CAL_USE][27] = 0;  //son Not Use로 설정   //son240604
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][27] = 0;//sylee1710126-3       //son240604
                SysInfoSer13.m_nData[part][m_nMode1][CAL_REF_R][27] = 0;//sylee1710126-3       //son240604
#endif           
            }


        }

        //son File에 save
        FileSysInfo01.LoadSaveSer13(_SAVE, m_nMode1, part); //save       //son220412

    }
    else {
        AfxMessageBox("ERROR No 2013  : \n \n Calibration data Saving Error !     file close! !  .\n \n   Mode Select  Please! (1 ~76)  ",
            MB_OK | MB_ICONSTOP);

    }


}



void CModeSer13::OnButton103()   //clear
{

    int i, RList;//, nRowCt,nColCt;
    CString str;

    nFlag1 = 0;

    for (RList = 1; RList <= MAX_CAL_RLIST; RList++) { //son220408 MAX_CAL_RLIST:128
        for (i = CAL_USE; i <= CAL_TEST10; i++) {       //son220407 1:CAL_USE,  15:CAL_TEST10
            str.Format("        ");
            m_grid1.SetTextMatrix(RList, i, str);
        }
    }


}

void CModeSer13::OnInit()   //Initclear
{

    int i;
    CString str;

    k = 0;

    m_grid1.Clear();

    m_grid1.SetRows(MAX_CAL_RLIST + 1);                  //son220408   129: MAX_CAL_RLIST+1
    m_grid1.SetCols(CAL_GRID_COL_NUM);//sylee20120114   //son220407    16: CAL_GRID_COL_NUM 
    m_grid1.SetCol(0);
    m_grid1.SetRow(0);

    for (i = 1; i <= MAX_CAL_RLIST; i++)       //son220408 128:  MAX_CAL_RLIST
    {
        m_grid1.SetCol(0);
        m_grid1.SetRow(i);
        str.Format("PIN%d", i);
        m_grid1.SetText(str);

    }


    COLORREF clr1, clr2, clr3, clr4, clr5, clr6, clr7;

    clr1 = RGB(200, 0, 20);//red
    clr2 = RGB(255, 200, 228);//white red 
    clr3 = RGB(225, 255, 164);//green
    clr4 = RGB(9, 73, 172);//blue
    clr5 = RGB(0, 10, 50);//black
    //clr6 = RGB(253,255,255);//white
    clr6 = RGB(252, 254, 211);//white  yellow
    clr7 = RGB(0, 0, 0);//black

    m_grid1.SetForeColor(clr7);
    m_grid1.SetBackColor(clr6);


    m_grid1.SetColColor(1, clr1, clr3);//sylee
    m_grid1.SetColColor(2, clr4, clr2);//sylee
    m_grid1.SetColColor(3, clr5, clr2);//sylee   
    m_grid1.SetColColor(4, clr1, clr3);//sylee

    str.Format("Use=1");
    m_grid1.SetTextMatrix(0, CAL_USE, str);         //son220407 1: CAL_USE

    str.Format("Ref. R");
    m_grid1.SetTextMatrix(0, CAL_REF_R, str);       //son220407 2: CAL_REF_R

    str.Format("Ref AVG.");
    m_grid1.SetTextMatrix(0, CAL_REF_RAWR, str);    //son220407 3: CAL_REF_RAWR

    str.Format("Test AVG.");
    m_grid1.SetTextMatrix(0, CAL_TEST_AVG, str);    //son220407 4: CAL_TEST_AVG

    str.Format("Dif. Rate.");
    m_grid1.SetTextMatrix(0, CAL_DIFF_RATE, str);   //son220407 5: CAL_DIFF_RATE


    //son 6:CAL_TEST1,  15:CAL_TEST10
    for (i = CAL_TEST1; i <= CAL_TEST10; i++)    //sylee20120114 
    {
        str.Format("NO. %d", i - (CAL_TEST1 - 1));     //son220407 5: (CAL_TEST1-1)
        m_grid1.SetTextMatrix(0, i, str);
    }

    m_grid1.Refresh();
    m_grid1.SetRedraw(TRUE);

    m_nCurRow = 1;
    m_nCurCol = 1;

    m_grid1.GetWindowRect(m_rectGrid);  //son m_grid1의 스크린좌표를 가져온다. 
    ScreenToClient(m_rectGrid);

    //m_dRatio = double(m_rectGrid.Width())/ 10600;//10320;  //13550;//15150;///12100.0; //14380.0;
    m_dRatio = double(m_rectGrid.Width()) / 12120;  //son220407 화면크기 변경 관련 수정.
    OnEditing = FALSE;

}


void CModeSer13::OnButton105()   //stop
{
    nStop = 2;
}



BOOL CModeSer13::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

    /*
   if( i<0){
         i=0;
    }else   if( i<120){
         i=i+1;
    }else{
        i=120;
    }

    if(zDelta==-120){ //down
        SendMessage(VK_NEXT,0,0);
        m_grid1.SetScrollTrack(i);
        DoEvents();
        m_grid1.SetScrollTrack(i);
    }else if(zDelta==120){//up


    }
*/

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CModeSer13::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


////ZERO CAL

void CModeSer13::OnButton107() //SYLEE20120110
{
    //   OnButtonZeroCal(0, 255,1);
}


void CModeSer13::OnButton109() //SYLEE20120110
{
    //   OnButtonZeroCal(0, 5000, 2);//1 ~32768-)  //temp
}

void CModeSer13::OnButton126() //SYLEE130618 autocal
{
    //::ShellExecute(NULL,"open","notepad.EXE"," C:\\ACE300\\SETUP\\SysSet22.ini","NULL",SW_SHOWNORMAL);
    CString strPath;
    strPath.Format(" %s\\SETUP\\SysSet21_ACE400.ini", SysInfoView01.m_pStrFilePath1);   //son210514
#if defined(__ACE1000__)   //son240807
    strPath.Format(" %s\\SETUP\\SysSet21_ACE1000.ini", SysInfoView01.m_pStrFilePath1);   //son240807  for CCTC Plan B (ACE500코드로 ACE1000 계측기 지원)
#endif
    ::ShellExecute(NULL, "open", "notepad.EXE", strPath, "NULL", SW_SHOWNORMAL); //son201118 //son210514
}
void CModeSer13::OnButton127() //SYLEE130618 autocal
{
    //////////////////////////////////////////////////////////////////////
    FileSysInfo01.LoadSaveSet21(2);//sylee130618  //load  
    FileSysInfo01.LoadSaveSet22(2);//sylee130618  //load 

//////////////////////////////////////////////////////////////////////

}
void CModeSer13::OnButton125() //SYLEE130618 autocal
{

    int mode;
    int nCardStartNo, nDRecMax;
    CString str;
    char szText[50];


    m_edit3.GetWindowText(szText, 10);
    nCardStartNo = atoi(szText);
    if (nCardStartNo < 1) {
        nCardStartNo = 1;
    }

    nDRecMax = MAX_TEST_MODE;  //son MAX_TEST_MODE: 360

    if (MessageBox(" 4W Calibration?", "Confirm", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
    {
		m_nMode2 = R_RAW; // hdpark230105 spark cal //son230214
        str.Format("%d", nCardStartNo + 3);
        m_edit3.SetWindowText(str);//card start no
        //m_edit3.SetWindowText("4");//card start no
        DoEvents();

        for (mode = 1; mode <= nDRecMax; mode++)
        {
            //son CalMode 3: 4W
            if (dRec[mode][15] == 3.0)
            {
                m_combo1.SetCurSel(mode - 1);
                m_nMode1 = mode;
                DoEvents();
                Onload();
                DoEvents();
                OnA_Cal_1(); //test
                OnButton102();
                OnSave();
                DoEvents();
                if (nStop == 2) {
                    AfxMessageBox("  Auto  R  Calibration   Stopped  1 !!!");
                    return;
                }
                Sleep(2000); //son220418 수동Cal과 AutoCal 차이 있는 문제 관련 수정 500msec ->2000 msec
                             //          4W Cal 끝난 이후 한참 응답없는 문제 관련하여 
                             //          sleep을 for( mode=1; mode<=nDRecMax; mode++) 안 쪽으로 이동

            }
            //son220418 Sleep(500);//sylee170510
        }
        Sleep(20000);   //son220418 4W와 Open 사이에 20초 정도 Gap을 추가. (방전을 위해?)
    }

    ///lv//////////////////////////////////


    if (MessageBox(" \n\n  OPEN \n\n R  Calibration?", "Confirm", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
    {
		m_nMode2 = R_RAW; // hdpark230105 spark cal //son230214
        //m_edit3.SetWindowText("1");//card start no
        str.Format("%d", nCardStartNo);
        m_edit3.SetWindowText(str);//card start no
        DoEvents();

        for (mode = 1; mode <= nDRecMax; mode++)
        {
            //son CalMode 1: OPEN
            if (dRec[mode][15] == 1.0) {
                m_combo1.SetCurSel(mode - 1);
                m_nMode1 = mode;
                DoEvents();
                Onload();
                DoEvents();
                OnA_Cal_1(); //test
                OnButton102();
                OnSave();
                DoEvents();

                if (nStop == 2) {
                    AfxMessageBox("  Auto  R  Calibraion   SToped  2 !!!");
                    return;
                }

                Sleep(2000); //son220418 수동Cal과 AutoCal 차이 있는 문제 관련 수정 500msec ->2000 msec
                             //          Open Cal 끝난 이후 Short 넘어가기 직전에 2분이상 응답없는 문제 관련하여 
                             //          sleep을 for( mode=1; mode<=nDRecMax; mode++) 안 쪽으로 이동
            }
            //son220418 Sleep(500);//sylee170510
        }
        Sleep(20000);   //son220418 Open과 Short Test 사이에 20초 정도 Gap을 추가. (방전을 위해?)

    }
    //hv//////////////////////////////////


    if (MessageBox(" \n\n  Leak ,Short  \n\n  R Calibration?", "Confirm", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
    {
		m_nMode2 = R_RAW; // hdpark230105 spark cal //son230214
        //m_edit3.SetWindowText("2");//card start no
        str.Format("%d", nCardStartNo + 1);
        m_edit3.SetWindowText(str);//card start no
        DoEvents();

        for (mode = 1; mode <= nDRecMax; mode++)
        {
            //son CalMode 2: SHORT
            if (dRec[mode][15] == 2.0) {
                m_combo1.SetCurSel(mode - 1);
                m_nMode1 = mode;
                DoEvents();
                Onload();
                DoEvents();
                OnA_Cal_1(); //test
                OnButton102();
                OnSave();
                DoEvents();

                if (nStop == 2) {
                    AfxMessageBox("  Auto  R  Calibraion   SToped 3 !!!");
                    return;
                }
                Sleep(2000); //son220418 수동Cal과 AutoCal 차이 있는 문제 관련 수정 50msec ->2000 msec
                             //          Short Cal 끝난 이후 한참 응답없는 문제 관련하여 
                             //          sleep을 for( mode=1; mode<=nDRecMax; mode++) 안 쪽으로 이동
            }
            //son220418 Sleep(50);//sylee170510
        }

    }
    ///////////////////////////////////////////////////
	// hdpark230105 spark cal begin
#if 0
    if (MessageBox(" \n\n  Spark  \n\n  R Calibration?", "Confirm", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
    {
		m_nMode2 = R_SPARK;
        str.Format("%d", nCardStartNo + 1);
        m_edit3.SetWindowText(str);//card start no
        DoEvents();

        for (mode = 1; mode <= nDRecMax; mode++)
        {
            if ((dRec[mode][15] == 2.0) && (mode >= MODE221_LEAK_40V)) {
                m_combo1.SetCurSel(mode - 1);
                m_nMode1 = mode;
                DoEvents();
                Onload();
                DoEvents();
                OnA_Cal_1();
                OnButton102();
                OnSave();
                DoEvents();

                if (nStop == 2) {
                    AfxMessageBox("  Auto  R  Calibraion   SToped 4 !!!");
                    return;
                }
                Sleep(2000);
            }
        }
    }
#endif
	// hdpark230105 spark cal end
	
}



//son m_editUseStart 부터 m_editUseEnd까지 RList를 "Use Set 1" 으로 설정.
void CModeSer13::OnButton129() //SYLEE20120801  
{
    int a, b, RList;
    char szText[100];
    CString str;

    m_editUseStart.GetWindowText(szText, 10);
    a = atoi(szText);
    if (a < 1) {
        a = 1;
        m_editUseStart.SetWindowText("1");
    }

    if (a > MAX_CAL_RLIST) { //son220408 MAX_CAL_RLIST:128
        a = MAX_CAL_RLIST;
        m_editUseStart.SetWindowText("128");
    }


    m_editUseEnd.GetWindowText(szText, 10);
    b = atoi(szText);
    if (b < 1) {
        b = 1;
        m_editUseEnd.SetWindowText("1");
    }


    if (b > MAX_CAL_RLIST) { //son220408 MAX_CAL_RLIST:128
        b = MAX_CAL_RLIST;
        m_editUseEnd.SetWindowText("128");
    }

    for (RList = a; RList <= b; RList++) {
        str.Format("1");
        m_grid1.SetTextMatrix(RList, CAL_USE, str);  //son220407 1:CAL_USE
    }

    DoEvents();

}


//son m_editUseStart 부터 m_editUseEnd까지 RList를 "Use Set 0" 으로 설정.
void CModeSer13::OnButton130() //SYLEE20120801    
{
    int a, b, RList;
    char szText[100];
    CString str;

    m_editUseStart.GetWindowText(szText, 10);
    a = atoi(szText);
    if (a < 1) {
        a = 1;
        m_editUseStart.SetWindowText("1");
    }

    if (a > MAX_CAL_RLIST) { //son220408 MAX_CAL_RLIST:128
        a = MAX_CAL_RLIST;
        m_editUseStart.SetWindowText("128");
    }

    m_editUseEnd.GetWindowText(szText, 10);
    b = atoi(szText);
    if (b < 1) {
        b = 1;
        m_editUseEnd.SetWindowText("1");
    }

    if (b > MAX_CAL_RLIST) { //son220408 MAX_CAL_RLIST:128
        b = MAX_CAL_RLIST;
        m_editUseEnd.SetWindowText("128");
    }

    for (RList = a; RList <= b; RList++) {
        // for(i=4; i<=15; i++){
        str.Format("0");
        m_grid1.SetTextMatrix(RList, CAL_USE, str); //son220407 1:CAL_USE
        //  }  
    }

    DoEvents();


}


//son ALL 1 Set
void CModeSer13::OnButton135() //SYLEE20120801    
{
    int  RList;
    CString str;

    for (RList = 1; RList <= MAX_CAL_RLIST; RList++) { //son220408 MAX_CAL_RLIST:128

        str.Format("1");
        m_grid1.SetTextMatrix(RList, CAL_USE, str); //son220407 1:CAL_USE

    }

    DoEvents();


}


//son ALL 0 Set
void CModeSer13::OnButton136() //SYLEE20120801    
{
    int  RList;
    CString str;

    for (RList = 1; RList <= MAX_CAL_RLIST; RList++) {   //son220408 MAX_CAL_RLIST:128
        str.Format("0");
        m_grid1.SetTextMatrix(RList, CAL_USE, str);      //son220407 1:CAL_USE
    }

    DoEvents();


}




//son 하부핀 Cal 측정 모드를 On하고  grid를 다시 그린다.
//    여기서 체크한 하부핀 Cal 측정 모드는 R 기능에도 영향을 준다.
//    R에서 하부로 측정 on하면 Cal에도 On이 된다. 
void CModeSer13::OnCHECKLowerPin()  //son220331 하부핀 Cal 측정기능
{
    // TODO: Add your control notification handler code here
    CString str, str2;	    //son220419

    if (m_checkLowerPin.GetCheck())      //son220315
    {
        if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)    //son220419 begin
        {
            str.Format("Error No 2114 !\n\nSysSet13.m_nUseCalData=UPPER_ONLY.\nCan't turn on this \"Use LowerCalJig\" CheckBox.\n\n");
            str2 += str;
            str.Format("First of all, change the Menu->MAP->Set1->UsaCalData to \"Upper_Lower\".\n\n");
            str2 += str;
            MyTrace(PRT_LEVEL1, str2);
            AfxMessageBox(str2, MB_OK);

            m_bUseLowerCalJig = 0;   // 하부핀 R 모드 off
            m_checkLowerPin.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
        else //son220419 end
        {
            //son 하부가 없는 PinMode일때에는 하부핀 Cal 측정모드를 Off해야 한다.
            if (IS_NO_LOWER(SysSet12.m_nPinTotMax))
            {   //son220331
                CString str;
                str.Format("Error No 11203, PinMode:%s has no lower pin. Can't use LowerPin for R Measure.\n",
                    STR_MAX_PIN(SysSet12.m_nPinTotMax));
                MyTrace(PRT_BASIC, str);
                AfxMessageBox(str);

                m_bUseLowerCalJig = 0;   // 하부핀 Cal 모드 off
                m_checkLowerPin.SetCheck(0);
                UpdateData(FALSE);      // UI에 값을 반영한다.
            }   //son220331
            else
                m_bUseLowerCalJig = 1;
        }
    }
    else {
        m_bUseLowerCalJig = 0;
    } //son220315

    //son Cal은 화면에 Pin 출력을 하지 않으므로 Grid를 다시 그릴 필요는 없음.
    //Onload_Grid(m_nTestType);    
}

//son230919 CalCovert 테스트 호출 함수 추가
void CModeSer13::CalConvertTest(int m_nMode1, int nConMode, double dRvalue, double dExpectVal) 
{
    //if (m_nMode1 >= MODE_SHORT_START) 
    //    RCalConvert_short(m_nMode1, dRvalue, nConMode, PART_UPPER);  //son RCalConvert_short()함수는 삭제. 
    //else
    
    //son230925  4W RAW_TO_REAL는  0부터 기울기 체크하는 기능이 특별히 추가됨.  이상태로 현장 튜닝이 많이 돼 있으므로  
    //           ARun201_4w_R_Cal1() 함수는 통합하지 않고 유지하기로 한다.  
    if ((nConMode == RAW_TO_REAL) && (m_nMode1 >= MODE_4W_START && m_nMode1 <= MODE_4W_END)  ) 
    {
        Sub71.R1 = dRvalue;
        ARun201_4w_R_Cal1(m_nMode1, PART_UPPER);
    }
    else
        RCalCovert(m_nMode1, dRvalue, nConMode, PART_UPPER);

    MyTrace(PRT_BASIC, "RCalCovert(mode:%d): %s=%.2f => %s=%.2f : expect=%.2f \n", 
                            m_nMode1, (nConMode == REAL_TO_RAW) ? "Real_R": 
                                      (nConMode == RAW_TO_REAL) ? "Raw_R": "UNDEF", dRvalue,
                            (nConMode == REAL_TO_RAW) ? "Raw_R": 
                            (nConMode == RAW_TO_REAL) ? "Real_R": "UNDEF", Sub71.R1, dExpectVal );
}

//son230919  CalConvert 함수 테스트
void CModeSer13::OnClickBUTTONCalConvertTest() 
{
	// TODO: Add your control notification handler code here
    double dRefReal, dRefRaw, dExpectVal; 
    int nConMode, m_nMode1;
	
    FileSysInfo01.LoadSaveSer13(_LOAD, MODE6_OPEN_10V, PART_UPPER);//son230919
    FileSysInfo01.LoadSaveSer13(_LOAD, MODE327_LEAK_250V, PART_UPPER);//son230919
    FileSysInfo01.LoadSaveSer13(_LOAD, MODE171_4W, PART_UPPER);//son230927

#if 1
    //----------------------------------------------------------------
    //MODE6_OPEN_10V  Open REAL_TO_RAW    Real R 기준 오름차순  (ohm 단위)
    //----------------------------------------------------------------
    nConMode = REAL_TO_RAW;
    m_nMode1 = MODE6_OPEN_10V;
    MyTrace(PRT_BASIC, "OPEN_10V  REAL_TO_RAW ----------------------\n");

    //son 리스트 첫번째 Real값의 - 근사값입력
    dRefReal = 0.8;  dExpectVal= 21;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 첫번째의 Real값 입력
    dRefReal= 1;  dExpectVal= 22.32;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 첫번째 Real값의 + 근사값입력
    dRefReal = 1.4;  dExpectVal= 22.7;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 첫번째 +1항목 의 Real값 입력
    dRefReal= 10;  dExpectVal= 31.02;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 첫번째 +1항목 Real값의 + 근사값입력
    dRefReal = 12;  dExpectVal= 37;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 중간 1번째의  Real값 입력 
    dRefReal = 100;  dExpectVal= 113.73;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 중간1 번째  Real값의 +근사값 입력 
    dRefReal = 110;  dExpectVal= 122;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 중간 2번째의  Real값 입력 
    dRefReal = 10000;  dExpectVal= 9675.66;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    
    //son 리스트 중간 2번째의  Real값의 +근사값  입력 
    dRefReal = 15000;  dExpectVal= 14000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막 -1 항목의  Real값 입력 
    dRefReal = 100000;  dExpectVal= 101599.65;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막 -1 항목 Real 값의  + 근사값  입력 
    dRefReal = 150000;  dExpectVal= 140000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막의 Real값 입력 
    dRefReal = 200000;  dExpectVal= 189947.53;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 맨마지막 Real값의  +근사값 입력 
    dRefReal = 250000;  dExpectVal= 235000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을 0으로 입력한 경우  
    dRefReal = 0;  dExpectVal= 0;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을  0 미만으로 입력한 경우 
    dRefReal = -3;  dExpectVal= -1;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을 매우 큰 값으로 입력한 경우  
    dRefReal = 999999999999;  dExpectVal= -1;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
#endif
    
#if 1
    //----------------------------------------------------------------
    //MODE171_4W  4W REAL_TO_RAW    Real R 기준 오름차순 (mohm 단위)
    //----------------------------------------------------------------
    nConMode = REAL_TO_RAW;
    m_nMode1 = MODE171_4W;
    MyTrace(PRT_BASIC, "MODE171_4W  REAL_TO_RAW ----------------------\n");

    //son 리스트 첫번째 Real값의 - 근사값입력
    dRefReal = 0.8;  dExpectVal= 21;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 첫번째의 Real값 입력
    dRefReal= 1;  dExpectVal= 32;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 첫번째 Real값의 + 근사값입력
    dRefReal = 1.4;  dExpectVal= 34;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 첫번째 +1항목 의 Real값 입력
    dRefReal= 2;  dExpectVal= 39;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 첫번째 +1항목 Real값의 + 근사값입력
    dRefReal = 2.3;  dExpectVal= 42;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    

    //son 리스트 중간 1번째의  Real값 입력 
    dRefReal = 10;  dExpectVal= 105;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 중간1 번째  Real값의 +근사값 입력 
    dRefReal = 12;  dExpectVal= 120;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 중간 2번째의  Real값 입력 
    dRefReal = 200;  dExpectVal= 1000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    
    //son 리스트 중간 2번째의  Real값의 +근사값  입력 
    dRefReal = 300;  dExpectVal= 1000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막 -1 항목의  Real값 입력 
    dRefReal = 75000;  dExpectVal= 1000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막 -1 항목 Real 값의  + 근사값  입력 
    dRefReal = 77000;  dExpectVal= 1020;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막의 Real값 입력 
    dRefReal = 100000;  dExpectVal= 1292;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 맨마지막 Real값의  +근사값 입력 
    dRefReal = 102000;  dExpectVal= 1300;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을 0으로 입력한 경우  
    dRefReal = 0;  dExpectVal= 0;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을  0 미만으로 입력한 경우 
    dRefReal = -3;  dExpectVal= -1;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을 매우 큰 값으로 입력한 경우  
    dRefReal = 999999999999;  dExpectVal= -1;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
#endif    
	

#if 1
    //----------------------------------------------------------------
    //MODE327_LEAK_250V  Short REAL_TO_RAW  Real R 기준 오름차순 (ohm 단위)
    //----------------------------------------------------------------
    nConMode = REAL_TO_RAW;
    m_nMode1 = MODE327_LEAK_250V;


    MyTrace(PRT_BASIC, "LEAK_250V  REAL_TO_RAW --------------------- \n");

    //son 리스트 첫번째 Real값의 - 근사값입력
    dRefReal = 9999;  dExpectVal= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 첫번째의 Real값 입력
    dRefReal= 10000;  dExpectVal= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 첫번째 Real값의 + 근사값입력
    dRefReal = 10030;  dExpectVal= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 첫번째 +1항목 의 Real값 입력
    dRefReal= 20000;  dExpectVal= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 첫번째 +1항목 Real값의 + 근사값입력
    dRefReal = 20020;  dExpectVal= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 중간 1번째의  Real값 입력 
    dRefReal = 75000;  dExpectVal= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 중간1 번째  Real값의 +근사값 입력 
    dRefReal = 75100;  dExpectVal= 65000;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);

    //son 리스트 중간 2번째의  Real값 입력 
    dRefReal = 100000;  dExpectVal= 61500;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    
    //son 리스트 중간 2번째의  Real값 입력 
    dRefReal = 100300;  dExpectVal= 61420;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막 -1 항목의  Real값 입력 
    dRefReal = 200000000;  dExpectVal= 286;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막 -1 항목 Real 값의  + 근사값  입력 
    dRefReal = 203000000;  dExpectVal= 283;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son 리스트 맨마지막의 Real값 입력 
    dRefReal = 300000000;  dExpectVal= 278;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
    
    //son 리스트 맨마지막 Real값의  +근사값 입력 
    dRefReal = 305000000;  dExpectVal= 275;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을 0으로 입력한 경우  
    dRefReal = 0;  dExpectVal= 0;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을  0 미만으로 입력한 경우 
    dRefReal = -3;  dExpectVal= -1;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);


    //son Real 값을 매우 큰 값으로 입력한 경우  
    dRefReal = 999999999999;  dExpectVal= -1;
    CalConvertTest(m_nMode1, nConMode, dRefReal, dExpectVal);
#endif    

#if 1
    //----------------------------------------------------------------
    //MODE6_OPEN_10V  Open RAW_TO_REAL    Raw ADC R 기준 오름차순
    //----------------------------------------------------------------
    nConMode = RAW_TO_REAL;
    m_nMode1 = MODE6_OPEN_10V;
	
    MyTrace(PRT_BASIC, "OPEN_10V  RAW_TO_REAL ---------------------------\n");
    //son 리스트 첫번째 ADC Raw  값의 - 근사값입력
    dExpectVal = 0.8;  dRefRaw= 21;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 첫번째의  ADC Raw R값 입력
    dExpectVal= 1;  dRefRaw= 22.32;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 첫번째 ADC Raw 값의 + 근사값입력
    dExpectVal = 1.4;  dRefRaw= 22.7;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 첫번째 +1항목의  ADC Raw  값 입력
    dExpectVal= 10;  dRefRaw= 31.02;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 첫번째 +1항목의 ADC Raw 값의 + 근사값입력
    dExpectVal = 12;  dRefRaw= 37;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 중간 1 번째의  ADC Raw 값 입력
    dExpectVal = 100;  dRefRaw= 113.73;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 중간 1 번째  ADC Raw 값의 +근사값 입력
    dExpectVal = 110;  dRefRaw= 122;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 중간 2 번째의  ADC Raw 값 입력 
    dExpectVal = 10000;  dRefRaw= 9675.66;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    
    //son 리스트 중간 2 번째  ADC Raw 값의 +근사값 입력 
    dExpectVal = 15000;  dRefRaw= 14000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막 -1항목의 ADC Raw 값 입력 
    dExpectVal = 100000;  dRefRaw= 101599.65;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막 -1항목의 ADC Raw 값의  +근사값 입력
    dExpectVal = 150000;  dRefRaw= 140000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막의 ADC Raw 값 입력
    dExpectVal = 200000;  dRefRaw= 189947.53;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 맨마지막 ADC Raw 값의  +근사값 입력
    dExpectVal = 250000;  dRefRaw= 235000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son ADC Raw 값을 0으로 입력한 경우  
    dExpectVal = 1;  dRefRaw= 0;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son ADC Raw 값을  0 미만으로 입력한 경우 
    dExpectVal = -1;  dRefRaw= -3;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son ADC Raw 값을 매우 큰 값으로 입력한 경우  
    dExpectVal = -1;  dRefRaw= 999999999999;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
#endif
    
#if 1
    //----------------------------------------------------------------
    //MODE171_4W  4W RAW_TO_REAL    Real R 기준 오름차순 (mohm 단위)
    //----------------------------------------------------------------
    nConMode = RAW_TO_REAL;
    m_nMode1 = MODE171_4W;
    MyTrace(PRT_BASIC, "MODE171_4W  RAW_TO_REAL ----------------------\n");

    //son 리스트 첫번째 Real값의 - 근사값입력
    dExpectVal = 0.8;  dRefRaw= 21;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 첫번째의 Real값 입력
    dExpectVal= 1;  dRefRaw= 32;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 첫번째 Real값의 + 근사값입력
    dExpectVal = 1.4;  dRefRaw= 34;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 첫번째 +1항목 의 Real값 입력
    dExpectVal= 2;  dRefRaw= 39;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 첫번째 +1항목 Real값의 + 근사값입력
    dExpectVal = 2.3;  dRefRaw= 42;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    

    //son 리스트 중간 1번째의  Real값 입력 
    dExpectVal = 10;  dRefRaw= 105;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 중간1 번째  Real값의 +근사값 입력 
    dExpectVal = 12;  dRefRaw= 120;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 중간 2번째의  Real값 입력 
    dExpectVal = 200;  dRefRaw= 1000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    
    //son 리스트 중간 2번째의  Real값의 +근사값  입력 
    dExpectVal = 300;  dRefRaw= 1000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막 -1 항목의  Real값 입력 
    dExpectVal = 75000;  dRefRaw= 1000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막 -1 항목 Real 값의  + 근사값  입력 
    dExpectVal = 77000;  dRefRaw= 1020;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막의 Real값 입력 
    dExpectVal = 100000;  dRefRaw= 1292;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 맨마지막 Real값의  +근사값 입력 
    dExpectVal = 102000;  dRefRaw= 1300;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son Real 값을 0으로 입력한 경우  
    dExpectVal = 0.1;  dRefRaw= 0;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son Real 값을  0 미만으로 입력한 경우 
    dExpectVal = -3;  dRefRaw= -1;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son Real 값을 매우 큰 값으로 입력한 경우  
    dExpectVal = 999999999999;  dRefRaw= -1;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
#endif    

#if 1
    //----------------------------------------------------------------
    //MODE327_LEAK_250V  Short RAW_TO_REAL  Raw ADC R 기준 내림차순
    //----------------------------------------------------------------
    nConMode = RAW_TO_REAL;
    m_nMode1 = MODE327_LEAK_250V;

    MyTrace(PRT_BASIC, "LEAK_250V  RAW_TO_REAL ---------------------------\n");
	
    //son 리스트 첫번째 ADC Raw  값의 - 근사값입력
    dExpectVal = 9999;  dRefRaw= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 첫번째의  ADC Raw R값 입력
    dExpectVal= 10000;  dRefRaw= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 첫번째 ADC Raw 값의 + 근사값입력
    dExpectVal = 10030;  dRefRaw= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    

    //son 리스트 첫번째 +1항목의  ADC Raw  값 입력
    dExpectVal= 20000;  dRefRaw= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 첫번째 +1항목의 ADC Raw 값의 + 근사값입력
    dExpectVal = 20020;  dRefRaw= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 중간 1 번째의  ADC Raw 값 입력
    dExpectVal = 75000;  dRefRaw= 65535;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 중간 1 번째  ADC Raw 값의 +근사값 입력
    dExpectVal = 75100;  dRefRaw= 65000;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);

    //son 리스트 중간 2 번째의  ADC Raw 값 입력 
    dExpectVal = 100000;  dRefRaw= 61500;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    
    //son 리스트 중간 2 번째  ADC Raw 값의 +근사값 입력 
    dExpectVal = 100300;  dRefRaw= 61420;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막 -1항목의 ADC Raw 값 입력 
    dExpectVal = 200000000;  dRefRaw= 286;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막 -1항목의 ADC Raw 값의  +근사값 입력
    dExpectVal = 203000000;  dRefRaw= 283;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son 리스트 맨마지막의 ADC Raw 값 입력
    dExpectVal = 300000000;  dRefRaw= 278;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
    
    //son 리스트 맨마지막 ADC Raw 값의  +근사값 입력
    dExpectVal = 305000000;  dRefRaw= 275;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son ADC Raw 값을 0으로 입력한 경우  
    dExpectVal = 0;  dRefRaw= 0;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son Real 값을  0 미만으로 입력한 경우 
    dExpectVal = -1;  dRefRaw= -3;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);


    //son Real 값을 매우 큰 값으로 입력한 경우  
    dExpectVal = -1;  dRefRaw= 9999999999;
    CalConvertTest(m_nMode1, nConMode, dRefRaw, dExpectVal);
#endif    
}
