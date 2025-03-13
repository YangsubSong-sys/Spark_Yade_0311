// ModeSet12.cpp : implementation file
/**
 * @brief Pin Connection 조건 관리
 *
 * @details Jig Pin 과 Hw Pin 간의 Mapping 방법에 대한 조건을 관리
 *
 * @author 강홍구 (모듈 헤더 등록)
 * @date 2024-11-26
 *
 * 수정 이력:
 * - 2024-11-26 son241124-BSW-9 ACE500 C&B 핀블록별로 QD/DD 설정 기능 관련 기능 오류 hotfix
 */

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet12.h"
#include "FileSysInfo.h"
#include "A_Define1.h"//sylee161126


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//int nPinBlockRange[10][5];//sylee210402     
                    
//son210514  int nPinBlockRange[10][5]를 CPinBlockRange sPinBlockRange[10]로 변경
CPinBlockRange sPinBlockRange[10];    //son MakePinBlockRangeUserNo()에서 생성  //son210514
        //son nPinBlockRange[nStep][0]  => sPinBlockRange[nStep].nStartPin
        //son nPinBlockRange[nStep][1]  => sPinBlockRange[nStep].nEndPin
        //son nPinBlockRange[nStep][2]  => sPinBlockRange[nStep].nBlockType  1:QD, 2:DD, 3:SD
        //son nPinBlockRange[nStep][3]  => sPinBlockRange[nStep].nHwBoxType  0:TYPE_AC, 1:TYPE_BD


extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

extern CFileSysInfo  FileSysInfo01; 
extern CSysSet12  SysSet12;  //Pin SETUP 
extern short  OnPinConvertFlag1; //sylee151016-1  //sylee151111add
extern CSysSet13  SysSet13;  //SYLEE161108-1


  
 
extern short nPinC1[17001][2];  //sylee150922-1 


 

/////////////////////////////////////////////////////////////////////////////
// CModeSet12 dialog


CModeSet12::CModeSet12(CWnd* pParent /*=NULL*/)
    : CDialog(CModeSet12::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeSet12)
    //}}AFX_DATA_INIT
}




void CModeSet12::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeSet12)
        DDX_Control(pDX, IDC_COMBO101, m_combo1);
        DDX_Control(pDX, IDC_COMBO102, m_combo2);
        DDX_Control(pDX, IDC_COMBO103, m_combo3);
        DDX_Control(pDX, IDC_COMBO104, m_combo4);
        DDX_Control(pDX, IDC_COMBO105, m_combo5);
        DDX_Control(pDX, IDC_COMBO108, m_combo8);
        DDX_Control(pDX, IDC_CHECK1, m_check1);     //son Upper1 QD
        DDX_Control(pDX, IDC_CHECK2, m_check2);     //son Upper1 DD
        DDX_Control(pDX, IDC_CHECK3, m_check3);     //son Upper1 SD
        DDX_Control(pDX, IDC_CHECK4, m_check4);     //son Manual QD/DD/SD 체크박스
        DDX_Control(pDX, IDC_CHECK5, m_check5);     //son Lower1 QD
        DDX_Control(pDX, IDC_CHECK6, m_check6);     //son Lower1 DD
        DDX_Control(pDX, IDC_CHECK7, m_check7);     //son Lower1 SD

        DDX_Control(pDX, IDC_CHECK8, m_check8);     //son Upper2 QD
        DDX_Control(pDX, IDC_CHECK9, m_check9);     //son Upper2 DD
        DDX_Control(pDX, IDC_CHECK10, m_check10);   //son Upper2 SD

        DDX_Control(pDX, IDC_CHECK11, m_check11);   //son Lower2 QD
        DDX_Control(pDX, IDC_CHECK12, m_check12);   //son Lower2 DD
        DDX_Control(pDX, IDC_CHECK13, m_check13);   //son Lower2 SD 

        DDX_Control(pDX, IDC_LABEL1001,  m_Label1001);
        DDX_Control(pDX, IDC_LABEL1002,  m_Label1002);
        DDX_Control(pDX, IDC_LABEL1003,  m_Label1003);
        DDX_Control(pDX, IDC_LABEL1004,  m_Label1004);
        DDX_Control(pDX, IDC_LABEL1005,  m_Label1005);
        DDX_Control(pDX, IDC_LABEL1006,  m_Label1006);
        DDX_Control(pDX, IDC_LABEL1007,  m_Label1007);
        DDX_Control(pDX, IDC_LABEL1008,  m_Label1008);
        DDX_Control(pDX, IDC_LABEL1009,  m_Label1009);
        DDX_Control(pDX, IDC_LABEL1010,  m_Label1010);  
        DDX_Control(pDX, IDC_LABEL1011,  m_Label1011);   
        DDX_Control(pDX, IDC_BLOAD,  m_button1001);
        DDX_Control(pDX, IDC_BSAVE,  m_button1002);
        DDX_Control(pDX, IDC_OK,  m_button1003);
        DDX_Control(pDX, IDC_COMBO_LOWER_START_PIN, m_combo_LowerJigStartPin);
        DDX_Control(pDX, IDC_COMBO_END_PIN, m_combo_EndPin);
        DDX_Control(pDX, IDC_COMBO21, m_combo21);
        DDX_Control(pDX, IDC_COMBO22, m_combo22);
        DDX_Control(pDX, IDC_COMBO23, m_combo23);
        DDX_Control(pDX, IDC_COMBO24, m_combo24);

    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet12, CDialog)
    //{{AFX_MSG_MAP(CModeSet12)
        ON_CBN_SELCHANGE(IDC_COMBO101, OnSelchangeCombo1)
        ON_CBN_SELCHANGE(IDC_COMBO102, OnSelchangeCombo2)
        ON_CBN_SELCHANGE(IDC_COMBO103, OnSelchangeCombo3)
        ON_CBN_SELCHANGE(IDC_COMBO104, OnSelchangeCombo4)
        ON_CBN_SELCHANGE(IDC_COMBO105, OnSelchangeCombo5)
        ON_CBN_SELCHANGE(IDC_COMBO108, OnSelchangeCombo8)       
        ON_BN_CLICKED(IDC_CHECK1, OnCheck1)   //son Upper1 QD
        ON_BN_CLICKED(IDC_CHECK2, OnCheck2)   //son Upper1 DD
        ON_BN_CLICKED(IDC_CHECK3, OnCheck3)   //son Upper1 SD
        ON_BN_CLICKED(IDC_CHECK4, OnCheck4) 
        ON_BN_CLICKED(IDC_CHECK5, OnCheck5)   //son Lower1 QD
        ON_BN_CLICKED(IDC_CHECK6, OnCheck6)   //son Lower1 DD
        ON_BN_CLICKED(IDC_CHECK7, OnCheck7)   //son Lower1 SD
        ON_BN_CLICKED(IDC_CHECK8, OnCheck8)   //son Upper2 QD
        ON_BN_CLICKED(IDC_CHECK9, OnCheck9)   //son Upper2 DD
        ON_BN_CLICKED(IDC_CHECK10, OnCheck10) //son Upper2 SD
        ON_BN_CLICKED(IDC_CHECK11, OnCheck11) //son Lower2 QD
        ON_BN_CLICKED(IDC_CHECK12, OnCheck12) //son Lower2 DD  
        ON_BN_CLICKED(IDC_CHECK13, OnCheck13) //son Lower2 SD 
        ON_CBN_SELCHANGE(IDC_COMBO_LOWER_START_PIN, OnSelchangeComboLowerStartPin)
        ON_CBN_SELCHANGE(IDC_COMBO_END_PIN, OnSelchangeComboEndPin)
        ON_CBN_SELCHANGE(IDC_COMBO21, OnCombo21)
        ON_CBN_SELCHANGE(IDC_COMBO22, OnCombo22)
        ON_CBN_SELCHANGE(IDC_COMBO23, OnCombo23)
        ON_CBN_SELCHANGE(IDC_COMBO24, OnCombo24)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet12 message handlers

BEGIN_EVENTSINK_MAP(CModeSet12, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
    ON_EVENT(CModeSet12, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
    ON_EVENT(CModeSet12, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
//  ON_EVENT(CModeSet12, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)

    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet12::OnClickOk() 
{
 
    //son OK로 exit할 때 하부 Jig 번호값을 SysSet12.m_nLowerJigStartPin 에 일단 반영한다. 
    //    file에 save하는 것은 'save' 에 맡긴다.
    if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
    {

        //son201222 OK 클릭했을 때 OnPinConvert1_32K()가 수행되도록 자동 save 되게  수정.
        //son LoadSaveSet12(_SAVE)-> ModeSet12.OnPinConvert1_32K(OnPinConvertType1)
        int ret=FileSysInfo01.LoadSaveSet12(_SAVE); //son190902 32KPIN: !!LoadSaveSet12(_SAVE)신규 추가
        if(ret!=1){
            errMessageBox(1212, "SysSet12.ini  Save ERROR !  \n    file Read Error !!!");    //son230217_2
        }
    }


//  FileSysInfo01.LoadSaveSub71(2);

    CDialog::OnOK();

}

extern short  OnPinConvertType1; //son241007-BSW-9    //son .CON 파일이 의미있으면 1 (total이 1이상, checksum 정상)
BOOL CModeSet12::OnInitDialog() 
{

    CDialog::OnInitDialog();

    CString str;
    int ret;

    //son LoadSaveSet12(_LOAD)-> ModeSet12.OnPinConvert1_32K(OnPinConvertType1)
    ret=FileSysInfo01.LoadSaveSet12(_LOAD); //son190902 32KPIN: enum 표시
    if(ret!=1)
        errMessageBox(1212, "CModeSet12::OnInitDialog()  SysSet12.ini  Load ERROR !  \n    file Read Error !!!");    //son230217_2
    
    //sylee180413-1 if( SysSet13.m_nPinBlockType==PIN_BLK_DD)    
    if (SysSet13.m_nPinBlockType== PIN_BLK_DD 
            || SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421)//sylee180413-1 //son190902 32KPIN 
    {

        GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);     
        GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);

        m_check1.SetWindowText(_T("  "));
        m_check8.SetWindowText(_T("  "));
        m_check5.SetWindowText(_T("  "));
        m_check11.SetWindowText(_T("  "));   

  
        if( SysSet12.m_nPinQDUpper == BOX_QD){ 
            SysSet12.m_nPinQDUpper = BOX_DD;     
        }               
        if( SysSet12.m_nPinQDLower == BOX_QD){ 
            SysSet12.m_nPinQDLower = BOX_DD; 
        }               
        if( SysSet12.m_nPinQDUpper2 == BOX_QD){  
            SysSet12.m_nPinQDUpper2 = BOX_DD;    
        }               
        if( SysSet12.m_nPinQDLower2 == BOX_QD){  
            SysSet12.m_nPinQDLower2 = BOX_DD;    
        }
    }


    //son PInBlock이 12K_QD_DD (상부 QD 2 box, 하부 DD 2box로  12K) 특이타입일 때 
    //if( SysSet13.m_nPinBlockType==7) {   //sylee121803      
    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //son190902 32KPIN  7:PIN_BLK_12K_QD_DD 
    {
        //son  하부 check box와 QD BlockType을  DD로 고정하여 설정한다.
        GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);    //son Lower1 QD Disable 
        GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);   //son Lower2 QD Disable
        m_check5.SetWindowText(_T("  "));
        m_check11.SetWindowText(_T("  "));              
        if( SysSet12.m_nPinQDLower == BOX_QD){       
            SysSet12.m_nPinQDLower = BOX_DD;         
        }               
        if( SysSet12.m_nPinQDLower2 == BOX_QD){      
            SysSet12.m_nPinQDLower2 = BOX_DD;        
        }
    }



    //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA )      
    {
        GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);      //son Upper1 DD 
        GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);      //son Upper1 SD
        GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);      //son Upper2 DD
        GetDlgItem(IDC_CHECK10)->EnableWindow(FALSE);     //son Upper2 SD
        GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);      //son Lower1 DD
        GetDlgItem(IDC_CHECK7)->EnableWindow(FALSE);      //son Lower1 SD
        GetDlgItem(IDC_CHECK12)->EnableWindow(FALSE);     //son Lower2 DD
        GetDlgItem(IDC_CHECK13)->EnableWindow(FALSE);     //son Lower2 SD

        //son QD 위치의 이름을 SD로 바꾸고, DD, SD 는 공백처리
        m_check1.SetWindowText(_T("SD"));
        m_check8.SetWindowText(_T("SD"));
        m_check5.SetWindowText(_T("SD"));
        m_check11.SetWindowText(_T("SD"));

        m_check2.SetWindowText(_T(" "));
        m_check3.SetWindowText(_T(" "));
        m_check9.SetWindowText(_T(" "));
        m_check10.SetWindowText(_T(" "));
        m_check6.SetWindowText(_T(" "));
        m_check7.SetWindowText(_T(" "));
        m_check12.SetWindowText(_T(" "));
        m_check13.SetWindowText(_T(" "));           

        SysSet12.m_nPinQDUpper = BOX_QD;        //son 1: QD
        SysSet12.m_nPinQDLower = BOX_QD;        //son 1: QD
        SysSet12.m_nPinQDUpper2 = BOX_QD;       //son 1: QD
        SysSet12.m_nPinQDLower2 = BOX_QD;       //son 1: QD
    }



    //son QD,DD,SD 자유선택모드가 아니면 
    if( SysSet12.m_nBlockSet1  != 1)    //son210514
    {
        //son 신규PinMode는  Upper2는 Upper1과 동일하게, Lower2는 Lower1과 동일하게 강제설정한다.
        if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
        {
            SysSet12.m_nPinQDUpper2 = SysSet12.m_nPinQDUpper;       //son201218
            SysSet12.m_nPinQDUpper3 = SysSet12.m_nPinQDUpper;       //son230217
            SysSet12.m_nPinQDUpper4 = SysSet12.m_nPinQDUpper;       //son230217

            SysSet12.m_nPinQDLower2 = SysSet12.m_nPinQDLower;       //son230217
            SysSet12.m_nPinQDLower3 = SysSet12.m_nPinQDLower;       //son230217
            SysSet12.m_nPinQDLower4 = SysSet12.m_nPinQDLower;       //son230217
            SysSet12.m_nPinQDLower5 = SysSet12.m_nPinQDLower;       //son230217
            SysSet12.m_nPinQDLower6 = SysSet12.m_nPinQDLower;       //son230217
        }
    }

    if( SysSet12.m_nPinQDUpper  == BOX_QD){         //son 1: QD
        m_check1.SetCheck(1); 
        m_check2.SetCheck(0); 
        m_check3.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDUpper  == BOX_DD){    //son 2: DD
        m_check1.SetCheck(0); 
        m_check2.SetCheck(1); 
        m_check3.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDUpper  == BOX_SD){    //son 3: SD
        m_check1.SetCheck(0); 
        m_check2.SetCheck(0); 
        m_check3.SetCheck(1);
    }
    if( SysSet12.m_nPinQDLower  == BOX_QD){         //son 1: QD
        m_check5.SetCheck(1); 
        m_check6.SetCheck(0); 
        m_check7.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDLower  == BOX_DD){    //son 2: DD
        m_check5.SetCheck(0); 
        m_check6.SetCheck(1); 
        m_check7.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDLower  == BOX_SD){    //son 3: SD
        m_check5.SetCheck(0); 
        m_check6.SetCheck(0); 
        m_check7.SetCheck(1);
    }

    if( SysSet12.m_nPinQDUpper2  == BOX_QD){        //son 1: QD
        m_check8.SetCheck(1); 
        m_check9.SetCheck(0); 
        m_check10.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDUpper2  == BOX_DD){   //son 2: QD
        m_check8.SetCheck(0); 
        m_check9.SetCheck(1); 
        m_check10.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDUpper2  == BOX_SD){   //son 3: QD
        m_check8.SetCheck(0); 
        m_check9.SetCheck(0); 
        m_check10.SetCheck(1);
    }

    if( SysSet12.m_nPinQDLower2  == BOX_QD){        //son 1: QD
        m_check11.SetCheck(1); 
        m_check12.SetCheck(0); 
        m_check13.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDLower2  == BOX_DD){   //son 2: DD
        m_check11.SetCheck(0); 
        m_check12.SetCheck(1); 
        m_check13.SetCheck(0); 
    }
    else if( SysSet12.m_nPinQDLower2  == BOX_SD){   //son 3: SD
        m_check11.SetCheck(0); 
        m_check12.SetCheck(0); 
        m_check13.SetCheck(1);
    }
    

    if( SysSet12.m_nBlockSet1  == 1){   //son241007-BSW-9
        m_check4.SetCheck(1);
    }
    else{
        m_check4.SetCheck(0);
    }

    //---------------------------------------------------------
    //son-8K4097-20200605  : MaxPin 콤보박스(m_combo1) data 초기화
    int i;
    m_combo1.ResetContent();    //son230328 
    for (i=0; i < PIN_TOT_END; i++) 
    {
//son230216_1 begin:
        if (i== 0)
            str.Format("%s", STR_MAX_PIN(i));
        else
            str.Format("%d. %s", i, STR_MAX_PIN(i)); 
        m_combo1.InsertString(-1, str ); 
//son230216_1 end
    }
    m_combo1.SetCurSel(SysSet12.m_nPinTotMax);


    //son 32K Trans Pin Block Type일 때에는 PinBoxMax를 4K로 고정 한다.
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS)    //son210525 //son230216_1
    {
        SysSet12.m_nPinBoxMax = 2;  //1:2K, 2:4K
        GetDlgItem(IDC_COMBO102)->EnableWindow(FALSE);       
    } //son210525

    //son 32K 외의 기존 Pin Block Type일 때에는 PinBoxMax를 입력받을 수 있다.
    else //son210525
    {
        GetDlgItem(IDC_COMBO102)->EnableWindow(TRUE);           
    }   //son210525

    m_combo2.SetCurSel(SysSet12.m_nPinBoxMax);

    m_combo3.SetCurSel(SysSet12.m_nPinSwitchMode);
    m_combo4.SetCurSel(SysSet12.m_nPinType);
    m_combo5.SetCurSel(SysSet12.m_nCardReverse);
    m_combo8.SetCurSel(SysSet12.m_nCardReverseB);

    EnableDisable_QDDDSDControl();  //son210514



    //son 적당한 User로 로그인이 되지 않았다면 사용 제한을 한다.
    if(  g_nLogOnUser1!=5 &&  g_nLogOnUser1!=7)//sylee151111-3
    {
//SYLEE210421   GetDlgItem(IDC_COMBO101)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO101)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO102)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO103)->EnableWindow(FALSE);   
        GetDlgItem(IDC_COMBO104)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO105)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO108)->EnableWindow(FALSE);       
    }
    else//sylee151016-1
    {
        GetDlgItem(IDC_COMBO101)->EnableWindow(TRUE);
        //son210525 32K Trans 인 경우는 PinBoxMax를 ReadOnly로 사용.
        if (SysSet13.m_nPinBlockType != PIN_BLK_32K_TRANS)    //son230216_1
            GetDlgItem(IDC_COMBO102)->EnableWindow(TRUE);

        GetDlgItem(IDC_COMBO103)->EnableWindow(TRUE);    
        GetDlgItem(IDC_COMBO104)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO105)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO108)->EnableWindow(TRUE); 

    }


    //------------------------------------------------------------------
    //son-32KPIN-20200227  : 하부StartPin 콤보박스(m_combo_LowerJigStartPin) data 초기화
 
    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    //    반드시 QD와 MaxPin 초기화(FileSysInfo01.LoadSaveSet12(_LOAD))가 완료된 다음에 호출해야 한다.
    if( SysSet12.m_nBlockSet1  == 1)    //son210514
        //son bForce가 TRUE이면 default 하부시작핀이 무조건 설정된다.
        initCombo_LowerStartPin(BOX_QD, SysSet12.m_nPinTotMax, TRUE); //son210514 QD 기준으로 default 하부시작핀을 콤보박스에 설정
    else 
        initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
    

    // 변경된 하부시작핀에 맞는 EndPin combo 설정.
    initCombo_EndPin(SysSet12.m_nPinTotMax); //son241007-BSW-9

 
    UpdateData(FALSE);      // UI에 값을 반영한다.

    FileSysInfo01.LoadSaveSet211(2);   //load //sylee160928

    A_Language() ;

    return 1;
}

//son210514 QD/DD/SD 컨트롤을 상황에 맞게 Enable Disable한다.
void CModeSet12::EnableDisable_QDDDSDControl()      //son210514
{

    //--------------------------------
    //son All Check, combo Enable
    GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);      
    GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);     
    GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);     //son Lower1 QD  
    GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);     //son Lower1 DD  
    GetDlgItem(IDC_CHECK7)->EnableWindow(TRUE);     //son Lower1 SD

    GetDlgItem(IDC_CHECK8)->EnableWindow(TRUE);     //son Upper2 QD 
    GetDlgItem(IDC_CHECK9)->EnableWindow(TRUE);     //son Upper2 DD 
    GetDlgItem(IDC_CHECK10)->EnableWindow(TRUE);    //son Upper2 SD
    GetDlgItem(IDC_CHECK11)->EnableWindow(TRUE);    //son Lower2 QD 
    GetDlgItem(IDC_CHECK12)->EnableWindow(TRUE);    //son Lower2 DD 
    GetDlgItem(IDC_CHECK13)->EnableWindow(TRUE);    //son Lower2 SD

    m_combo21.SetCurSel(SysSet12.m_nPinQDUpper3-1);//sylee210330-1 
    m_combo22.SetCurSel(SysSet12.m_nPinQDUpper4-1);//sylee210330-1 
    m_combo23.SetCurSel(SysSet12.m_nPinQDLower3-1);//sylee210330-1 
    m_combo24.SetCurSel(SysSet12.m_nPinQDLower4-1);//sylee210330-1 


    //-----------------------------------------------------------
    //son MaxPin 값 변경시에 하부시작핀 박스 enable/disable하기   
    //-----------------------------------------------------------
    
    //son190902 32KPIN: 32K PinBlock타입일 때에만 LowerJigStartPin 콤보 박스를 enable한다.    
    //son241007-BSW-9 EnableDisable_QDDDSDControl() 함수 내부로 이동 
    if ( SysSet12.m_nBlockSet1  != 1 //son241007-BSW-9
            && IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
    {
        //son QD/DD/SD Block Set이 OFF이면 하부시작핀 Combo는 Enable한다.
        GetDlgItem(IDC_COMBO_LOWER_START_PIN)->EnableWindow(TRUE);
        
    }
    else
    {
        //son QD/DD/SD Block Set이 ON이면 하부시작핀 Combo는 Disable한다.
        //    현재 QD/DD/SD Block Set 상태에서는 하부시작핀 기능이 제대로 동작하지 않는다.
		//son241007-BSW-9 32K Org 정방향 핀블록 QD/DD/SD Block Set이 ON이어도 하부시작핀 설정하는 기능 추가
		if (SysSet12.m_nPinTotMax != PIN_TOT_32K_ORG)
            GetDlgItem(IDC_COMBO_LOWER_START_PIN)->EnableWindow(FALSE);
        

    }


    //-------------------------------------------------------------------
    //son QD/DD/SD BlockSet 상태일때 특정케이스 check & Combo Disable

    if( SysSet12.m_nBlockSet1  == 1)    //son210514
    {    
        ////////////////////////////////////////////////////////////////////
        //SYLEE210421   

        GetDlgItem(IDC_LABEL21)->EnableWindow(TRUE);     //son Upper 3 Enable
        GetDlgItem(IDC_COMBO21)->EnableWindow(TRUE);    

        GetDlgItem(IDC_LABEL22)->EnableWindow(TRUE);     //son Upper 4 Enable
        GetDlgItem(IDC_COMBO22)->EnableWindow(TRUE);                      

        GetDlgItem(IDC_LABEL23)->EnableWindow(TRUE);     //son Lower 3 Enable
        GetDlgItem(IDC_COMBO23)->EnableWindow(TRUE);    

        GetDlgItem(IDC_LABEL24)->EnableWindow(TRUE);     //son Lower 4 Enable
        GetDlgItem(IDC_COMBO24)->EnableWindow(TRUE);


        if(SysSet12.m_nPinTotMax == PIN_TOT_4K   
                || SysSet12.m_nPinTotMax == PIN_TOT_8K      || SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN)
                //|| SysSet12.m_nPinTotMax == PIN_TOT_12K    
                //|| SysSet12.m_nPinTotMax == PIN_TOT_16K      )
        {
            //son Upper2 Disable
            GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);         
            GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);         
            GetDlgItem(IDC_CHECK10)->EnableWindow(FALSE); 

            //son Lower2 Disable
            GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);    
            GetDlgItem(IDC_CHECK12)->EnableWindow(FALSE);        
            GetDlgItem(IDC_CHECK13)->EnableWindow(FALSE); 
        }

        if (IS_UPPER_12K_2(SysSet12.m_nPinTotMax))    //son210525   //son230718
        {
            //son Upper4 Disable
            GetDlgItem(IDC_LABEL22)->EnableWindow(FALSE);
            GetDlgItem(IDC_COMBO22)->EnableWindow(FALSE);
            m_combo22.SetCurSel(0);
        }


        if (IS_UPPER_8K_2(SysSet12.m_nPinTotMax) || IS_UPPER_4K(SysSet12.m_nPinTotMax)) //son230718
        {
            if (IS_UPPER_4K(SysSet12.m_nPinTotMax))  //son210525
            {
                //son Upper2 Disable
                GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);         
                GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);         
                GetDlgItem(IDC_CHECK10)->EnableWindow(FALSE); 
            }

            //son Upper3 Disable
            GetDlgItem(IDC_LABEL21)->EnableWindow(FALSE);        
            GetDlgItem(IDC_COMBO21)->EnableWindow(FALSE);   
            m_combo21.SetCurSel(0);      

            //son Upper4 Disable
            GetDlgItem(IDC_LABEL22)->EnableWindow(FALSE);
            GetDlgItem(IDC_COMBO22)->EnableWindow(FALSE);
            m_combo22.SetCurSel(0);

        }

        if (IS_LOWER_8K(SysSet12.m_nPinTotMax) || IS_LOWER_4K(SysSet12.m_nPinTotMax))
        {
            if (IS_LOWER_4K(SysSet12.m_nPinTotMax))  //son210525
            {
                //son Lower2 Disable
                GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);    
                GetDlgItem(IDC_CHECK12)->EnableWindow(FALSE);        
                GetDlgItem(IDC_CHECK13)->EnableWindow(FALSE); 
            }

            //son Lower3 Disable
            GetDlgItem(IDC_LABEL23)->EnableWindow(FALSE);
            GetDlgItem(IDC_LABEL24)->EnableWindow(FALSE);
            m_combo23.SetCurSel(0);      

            //son Lower4 Disable
            GetDlgItem(IDC_COMBO23)->EnableWindow(FALSE);
            GetDlgItem(IDC_COMBO24)->EnableWindow(FALSE);
            m_combo24.SetCurSel(0);
        }
        ////////////////////////////////////////////////////////////////////
    }


    //-------------------------------------------------------------------
    //son QD/DD/SD BlockSet 아닐 때  특정케이스 check & Combo Disable
    else
    {

        //son-32KPIN-20200117: 32K에서도 QD, DD, SD 체크기능 추가 관련
        //------------------------------------------------------------------------
        // 32K는 Upper1, Lower1 체크박스와 LowerJigStartPin을 가지고 동작하므로
        // Upper2, Lower2 관련 체크박스들은 모두 Disable 한다.
        // 32K가 아닌 경우라면 Upper2, Lower 체크박스를 다시 enable해야 한다.
        //------------------------------------------------------------------------


        GetDlgItem(IDC_LABEL21)->EnableWindow(FALSE);   //son210514 Upper3 Disable       
        GetDlgItem(IDC_LABEL22)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO21)->EnableWindow(FALSE);   //son210514 Upper4 Disable  
        GetDlgItem(IDC_COMBO22)->EnableWindow(FALSE);

        GetDlgItem(IDC_LABEL23)->EnableWindow(FALSE);   //son210514 Lower3 Disable
        GetDlgItem(IDC_LABEL24)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO23)->EnableWindow(FALSE);   //son210514 Lower4 Disable
        GetDlgItem(IDC_COMBO24)->EnableWindow(FALSE);

        //son  No 32K -> 32K
        if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
        {
            //son-32KPIN-20200117: 32K BlockType에서 QD, DD, SD 체크기능 추가
            //   32K Upper1, Lower1 체크박스와 LowerJigStartPin을 가지고 동작하므로
            //       Upper2, Lower2 관련 체크박스들은 모두 Disable 한다.
            GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);    //son Upper2 QD
            GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);    //son Upper2 DD
            GetDlgItem(IDC_CHECK10)->EnableWindow(FALSE);   //son Upper2 SD

            GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);   //son Lower2 QD
            GetDlgItem(IDC_CHECK12)->EnableWindow(FALSE);   //son Lower2 DD
            GetDlgItem(IDC_CHECK13)->EnableWindow(FALSE);   //son Lower2 SD

        }

    }
    UpdateData(FALSE);       // 화면에 값을 반영 
}
 
//son210115  QDType, MaxPin과 현재  LowerStartPin이 맞지 않는 경우에 보정한다. 
//           반드시 QD와 MaxPin 초기화가 완료된 다음에 호출해야 한다.
//
//  File에서 로딩한 하부시작핀 값이 변경됐을 때의 이후 조치를 취한다. 
//  repair_LowerStartPin()을 사용하는 OnPinConvert1_32K()가 전역함수로 
//  FileSysInfo에서 ModeSet12(Pin설정 UI)와 관계 없이  호출되므로 
//  하부시작핀 콤보박스는 여기서 변경할 수 없고 initCombo_LowerStartPin()로 
//  나중에 Pin 설정 UI 구동시에 변경한다.
void CModeSet12::repair_LowerStartPin(int nQdType, int nMaxPin) 
{

    //==========================================================================
    //son BlockSet1=1: BlockRange로 하부시작핀이 자동결정되는 케이스
    //==========================================================================
    int nModeLowerStart;

    //son 32K Org이고 QD/DD/SD 혼용일 때에는 SysSet12.m_nLowerJigStartPin_Offset 때문에 
    //     코드가 꼬일 수 있어서 하부시작핀 보정하지 않고 m_nUpperBox 만 설정한다. 
    if (SysSet12.m_nBlockSet1 == 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG) //son241007-BSW-9 begin
    {
        nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin); 
        if (nModeLowerStart >= 0)       
            SysSet12.m_nUpperBox = findUpperBoxNum(nModeLowerStart);    
        return;
    } //son241007-BSW-9 end

    if (SysSet12.m_nBlockSet1 == 1      //son210525
            //son230718 || IS_UPPER_12K(nMaxPin)  
            || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS     //son210826 현재 nEnd에 맞는 고정하부시작핀 필요
            || SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS    //son230718 고정하부핀 사용 특수케이스
            || SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)     //son230718 고정하부핀 사용 특수케이스
    {                 
        //return ; //sylee210407  
        
        //son MakePinBlockRangeUserNo()에서 생성한 sPinBlockRange[5].nStartPin으로 nUpperEnd 계산
        int nUpperEnd = sPinBlockRange[5].nStartPin -1;

        //son 하부시작핀 보정.  QD 기준으로
        SysSet12.m_nLowerJigStartPin = FindBlockSet_LowerStartPin(nUpperEnd);  //son210514
        return;
    }


    //================================================
    // 사용자가 설정한  하부시작핀을 사용하는 케이스 
    //================================================

    //son210202: 구형 타입인 경우는 HW QDType에  하부시작핀이 변동되지 않으므로 return한다.
    if (nMaxPin == PIN_TOT_8K_LOW_STPIN //son 구형 16KHW에 8K(4K+4K) 실장시
            || nMaxPin == PIN_TOT_12K
            || nMaxPin == PIN_TOT_U8K_UPPER_ONLY)   //son211020
        return;
 

    //----------------------------------------------------------
    //son 하부시작핀 정보에 따라 UpperBox 개수 정보를 찾는다.
    nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin); //son240516
    if (nModeLowerStart >= 0)       
        SysSet12.m_nUpperBox = findUpperBoxNum(nModeLowerStart);    //son240516

    
    //----------------------------------------------------------
    //son QDType와 MaxPin 값을 잘 보고 하부시작Pin 오류를 보정
    
    //son 현재 하부시작핀Pin 값이 range 이내라면 유지 시켜 주고  range를 벗어난다면 default값으로 설정해 준다.
    //son son 입력 순서는 E_LOWER_STARTPIN enum 순서를 지켜야 한다. 
    //son 기본적으로 하부시작핀은 HalfPin+1인데,(32K라면 16385) 
    //son Upper Box의 뒷부분을 dummy로 설정하기 위해  boxPin 갯수 단위(QD라면 4097, 8193 )로 선택할 수 있다. 
    switch (nQdType)
    {
        case BOX_SD:
            //son BoxSize 1024, 1 box  // 8K SD HalfPin이 1024이므로 1025 가능,  
            if (nMaxPin == PIN_TOT_8K_TRANS)       //son210319 //son230216_1
            {
                if (SysSet12.m_nLowerJigStartPin != 1025)
                    SysSet12.m_nLowerJigStartPin = 1025;          
            }

            //son BoxSize 1024, 2 box  // 16K SD HalfPin이 2048이므로 2049, 1025 가능,  
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS    //son210319 //son230216_1
                    || nMaxPin == PIN_TOT_32K_U8K_L24K)     //son230221_2
            {
                if (SysSet12.m_nLowerJigStartPin != 1025 && 
                    SysSet12.m_nLowerJigStartPin != 2049 )
                    SysSet12.m_nLowerJigStartPin = 2049;          //son210308 || 조건으로 잘못코딩. &&로 수정완료
            } 

            //son BoxSize 1024, 4 box // 32K SD HalfPin이 4096이므로 4097, 2049, 1025 가능,  
            else                                                                                    
            {
                if (SysSet12.m_nLowerJigStartPin != 1025 && 
                    SysSet12.m_nLowerJigStartPin != 2049 && 
                    SysSet12.m_nLowerJigStartPin != 3073 &&     //son240522 상부 block 3개 기능 추가
                    SysSet12.m_nLowerJigStartPin != 4097 ) 
                {
                    SysSet12.m_nLowerJigStartPin = 4097;          //son210308 || 조건으로 잘못코딩. &&로 수정완료
                    SysSet12.m_nUpperBox = 4;    //son240516 초기화
                }
            }
            break;

        case BOX_DD:
            //son BoxSize 2048, 1 box  // 8K DD HalfPin이 2048이므로 2049만 가능,  
            if (nMaxPin == PIN_TOT_8K_TRANS)       //son210319 //son230216_1
            {
                if ( SysSet12.m_nLowerJigStartPin != 2049 )
                    SysSet12.m_nLowerJigStartPin = 2049;          
            }
            //son BoxSize 2048, 2 box  // 16K DD HalfPin이 4096이므로 4097, 2049 가능,  
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS //son210319 //son230216_1
                    || nMaxPin == PIN_TOT_32K_U8K_L24K)     //son230221_2
            {
                if ( SysSet12.m_nLowerJigStartPin != 2049 && 
                    SysSet12.m_nLowerJigStartPin != 4097 )
                    SysSet12.m_nLowerJigStartPin = 4097;          //son210308 || 조건으로 잘못코딩. &&로 수정완료
            }

            //son BoxSize 2048, 4 box  //32K DD HalfPin이 8192이므로 8193, 4097, 2049 가능,  
            else                                                                                    
            {
                if (SysSet12.m_nLowerJigStartPin != 2049 && 
                    SysSet12.m_nLowerJigStartPin != 4097 && 
                    SysSet12.m_nLowerJigStartPin != 6145 &&     //son240522 상부 block 3개 기능 추가
                    SysSet12.m_nLowerJigStartPin != 8193 ) 
                {
                    SysSet12.m_nLowerJigStartPin = 8193;          //son210308 || 조건으로 잘못코딩. &&로 수정완료
                    SysSet12.m_nUpperBox = 4;    //son240516 초기화
                }

            }
            break;

        case BOX_QD:
        default:
            //son Boxsize 4096, 1 box           // 8K QD HalfPin이 4096이므로 4097만 가능   
            if (nMaxPin == PIN_TOT_8K_TRANS)         //son210319  //son230216_1
            {
                if ( SysSet12.m_nLowerJigStartPin != 4097) 
                    SysSet12.m_nLowerJigStartPin = 4097;          
            }

            //son Boxsize 4096, 2 box          // 16K QD HalfPin이 8192이므로 8193, 4097 가능   
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS  //son210319 //son230216_1
                    || nMaxPin == PIN_TOT_32K_U8K_L24K)     //son230221_2
            {
                if ( SysSet12.m_nLowerJigStartPin != 4097 && 
                    SysSet12.m_nLowerJigStartPin != 8193 )
                    SysSet12.m_nLowerJigStartPin = 8193;          //son210308 || 조건으로 잘못코딩. &&로 수정완료
            }

            //son Boxsize 4096, 4 box           //32K QD HalfPin이 16384이므로 16385, 8193, 4097 가능 
            else
            {
                if (SysSet12.m_nLowerJigStartPin != 4097 && 
                    SysSet12.m_nLowerJigStartPin != 8193 && 
                    SysSet12.m_nLowerJigStartPin != 12289 &&   //son240522 상부 block 3개 기능 추가
                    SysSet12.m_nLowerJigStartPin != 16385 )
                {
                    SysSet12.m_nLowerJigStartPin = 16385;          //son210308 || 조건으로 잘못코딩. &&로 수정완료
                    SysSet12.m_nUpperBox = 4;    //son240516 초기화
                }

            }
            break;
    }

}


//son201222 nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
//    반드시 QD와 MaxPin 초기화(FileSysInfo01.LoadSaveSet12(_LOAD))가 완료된 다음에 호출해야 한다.
// 
void CModeSet12::initCombo_LowerStartPin(int nQdType, int nMaxPin, bool bForce)  //son210514 bForce추가
{

    m_combo_LowerJigStartPin.ResetContent();

//son230221_2 begin
    //son K600 특이 케이스  
    //    BDL은 상부 8K,  하부 24K.  HW적으로는 하부 5, 6 box 상부 HW  3,4 박스로 연결된다. 
    if (nMaxPin == PIN_TOT_32K_U8K_L24K)
    {
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 0: E_1025

        SysSet12.m_nUpperBox = 2;    //son240516 초기화
        switch (SysSet12.m_nLowerJigStartPin)   //son210514
        {
            case 2049:  //SD
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1: E_2049
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 2: E_4097
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 3: E_8193
                setCombo_DefaultLowerStartPin(E_2049, TRUE);
                break;

            case 4097:  //DD
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 1: E_2049
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2: E_4097
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 3: E_8193
                setCombo_DefaultLowerStartPin(E_4097, TRUE);
                break;

            case 8193: //QD
            default:
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 1: E_2049
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 2: E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3: E_8193
                setCombo_DefaultLowerStartPin(E_8193, TRUE);
                break;
        }
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 4: E_16385

        return;
    }
//son230221_2 end

    //son210525 특이 케이스 별도 처리 
    //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS는 여기에 해당이 안 된다. 5번째 box가 16385로 시작해야 함.
    if (IS_UPPER_12K_2(nMaxPin))  //son210603    //son210826    //son230718
    {
        SysSet12.m_nUpperBox = 3;    //son240516 초기화    

        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 0: E_1025
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 1: E_2049
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 2: E_4097
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 3: E_8193
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 4: E_16385

        switch (SysSet12.m_nLowerJigStartPin)   //son210514
        {
            case 3073:  //SD
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_3073].str);  // 5: E_3073
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 6: E_6145
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 7: E_12289
                setCombo_DefaultLowerStartPin(E_3073, TRUE);
                break;

            case 6145:  //DD
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 5: E_3073
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_6145].str);  // 6: E_6145
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 7: E_12289
                setCombo_DefaultLowerStartPin(E_6145, TRUE);
                break;

            case 12289: //QD
            default:
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 5: E_3073
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 6: E_6145
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_12289].str);  // 7: E_12289
                setCombo_DefaultLowerStartPin(E_12289, TRUE);
                break;
        }
        return;
    }

    //son210603 특이케이스 2
    //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS 특이케이스. 5번째 box가 16385로 시작해야 함.
    if ( nMaxPin == PIN_TOT_20K_U12K_L4K_UP_TRANS        //son210603 하부시작핀 QD 16385, DD 8193, SD 4097 고정으로 추가함. 
            || nMaxPin == PIN_TOT_20K_U8K_L4K_UP_TRANS  //son210826 하부시작핀 QD 16385, DD 8193, SD 4097 고정케이스 2
            || nMaxPin == PIN_TOT_24K_U12K_L8K_TRANS)   //son230718 하부시작핀 QD 16385, DD 8193, SD 4097 고정케이스 3
    {

        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 0: E_1025
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 1: E_2049
        if (nMaxPin == PIN_TOT_20K_U12K_L4K_UP_TRANS)
            SysSet12.m_nUpperBox = 3;    //son240516 초기화
        if (nMaxPin == PIN_TOT_20K_U8K_L4K_UP_TRANS)
            SysSet12.m_nUpperBox = 2;    //son240516 초기화

        switch (SysSet12.m_nLowerJigStartPin)   //son210514
        {
            case 4097:  //SD
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2: E_4097
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3: E_8193
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4: E_16385
                setCombo_DefaultLowerStartPin(E_4097, TRUE);
                break;

            case 8193:  //DD
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 2: E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3: E_8193
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4: E_16385
                setCombo_DefaultLowerStartPin(E_8193, TRUE);
                break;

            case 16385: //QD
            default:
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 2: E_4097
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3: E_8193
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_16385].str); // 4: E_16385
                setCombo_DefaultLowerStartPin(E_16385, TRUE);
                break;
        }

        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 5: E_3073
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 6: E_6145
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");   // 7: E_12289
        return;
    }

    //son QD/DD/SD BlockSet을 사용하는 경우
    if (SysSet12.m_nBlockSet1 == 1)     //son210514
    {
        if (nMaxPin == PIN_TOT_32K_ORG) //son241007-BSW-9 begin
        {
            for (int i=0; i < LOWER_STARTPIN_NUM; i++)   
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[i].str);  

            int nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin); 
            if (nModeLowerStart >= 0) 
                setCombo_DefaultLowerStartPin(nModeLowerStart, TRUE);     
            return;
        } //son241007-BSW-9 end

        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3:E_8193
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_16385].str); // 4:E_16385

        switch (SysSet12.m_nLowerJigStartPin)   //son210514
        {
            case 1025: setCombo_DefaultLowerStartPin(E_1025, TRUE); break;
            case 2049: setCombo_DefaultLowerStartPin(E_2049, TRUE); break;
            case 4097: setCombo_DefaultLowerStartPin(E_4097, TRUE); break;
            case 8193: setCombo_DefaultLowerStartPin(E_8193, TRUE); break;

            case 16385: 
            default:   setCombo_DefaultLowerStartPin(E_16385, TRUE); break;
        }
        return;
    }


    //son-8K4097-20200605: 상부 4K, 하부 4K 구형으로 
    //               hwPin<->swPin 변환이 없고 하부시작핀 Offest을  QD, DD, SD  상관없이 사용하는 경우. 
    //son 구형 16KHW에 8K 실장시
    //son 16K구형 HW에 1box만 실장해서 8K BoxQd=DD 로 쓰는 특수 케이스는 하부시작핀은 16K처럼 선택하게 해준다.
    if(nMaxPin == PIN_TOT_8K_LOW_STPIN)   
    {
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

        //son 8K with 하부시작핀 모드 :   
        //   기본 HW는 QD: 16K BlockType. 상부 1block 1block(skip), 하부 1block 1block(skip) 총 2 block 사용하므로 
        //   default 하부시작핀은 4097으로 설정한다.  QD, DD, SD 상관없이 4097 하부시작핀 설정이 가능해야 한다.
        setCombo_DefaultLowerStartPin(E_4097, bForce);  //son201222 //son210514
        //SysSet12.m_nUpperBox = 1;    //son240516 초기화
        return;
    }

    //son210202: 상부 8K, 하부 4K 구형 BlockType.
    //           hwPin<->swPin변환이 없고, 하부시작핀 Offest을  QD, DD, SD  상관없이 사용하는 경우. 
    if(nMaxPin == PIN_TOT_12K
            || nMaxPin == PIN_TOT_U8K_UPPER_ONLY)   //son211020 8K구형 상부만 존재하는 케이스 (f550j 나노시스) 
    {
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3:E_8193
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

        //son 12K with 하부시작핀 모드 :   
        //   기본 HW는 QD: 16K BlockType. 상부 2block, 하부 1block  총 3 block 사용하므로 
        //   default 하부시작핀은 8193으로 설정한다.  
        setCombo_DefaultLowerStartPin(E_8193, bForce);  //son201222 //son210514
        //SysSet12.m_nUpperBox = 2;    //son240516 초기화
        return;
    }

 
    //----------------------------------------------------------
    //son QDType와 MaxPin 값을 잘 보고 하부시작Pin을 설정한다.
    //----------------------------------------------------------
 
    //son 현재 하부시작핀Pin 값이 range 이내라면 유지 시켜 주고  range를 벗어난다면 default값으로 설정해 준다.
    //son son 입력 순서는 E_LOWER_STARTPIN enum 순서를 지켜야 한다. 
    
    //son 기본적으로 하부시작핀은 HalfPin+1인데,(32K라면 16385) 
    //son Upper Box의 뒷부분을 dummy로 설정하기 위해  boxPin 갯수 단위(QD라면 4097, 8193 )로 선택할 수 있다. 
    
    switch (nQdType)
    {
        case BOX_SD:
            //son 8K SD HalfPin이 1024이므로 1025만  가능,  (BoxSize 1024, 1box) 
            if (nMaxPin == PIN_TOT_8K_TRANS)      //son210319   //son230216_1
            {
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 2
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 2
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

                setCombo_DefaultLowerStartPin(E_1025, bForce);  //son201222 //son210514
                //SysSet12.m_nUpperBox = 1;    //son240516 초기화
            }
            //son 16K SD HalfPin이 2048이므로 2049, 1025 가능,  (BoxSize 1024, 2box) 
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS)  //son210319  //son230216_1
            {
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 2
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

                setCombo_DefaultLowerStartPin(E_2049, bForce);  //son201222 //son210514
                //SysSet12.m_nUpperBox = 2;    //son240516 초기화
            }

            //son 32K SD HalfPin이 4096이므로 4097, 2049, 1025 가능,  (BoxSize 1024, 4box) 
            // 주의: PIN_TOT_16K도 여기로 들어오게 됨. PIN_TOT_16K, SD 하부시작핀 2049도 이 안에 있으므로 OK.
            else                                                                                    
            {
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_3073].str);  // 5:E_3073  //son240522 상부 3block 기능추가 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

                setCombo_DefaultLowerStartPin(E_4097, bForce);  //son201222  
                //SysSet12.m_nUpperBox = 4;    //son240516 초기화
            }
            break;

        case BOX_DD:
            //son MaxPin은 8K. DD HalfPin이 2049이므로  2049만  가능  (Boxsize 2048, 1box)
            if (nMaxPin == PIN_TOT_8K_TRANS)      //son210319   //son230216_1
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 0
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 2  
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3  
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

                setCombo_DefaultLowerStartPin(E_2049, bForce);  //son201222 //son210514
                //SysSet12.m_nUpperBox = 1;    //son240516 초기화

            }
            //son 16K DD HalfPin이 4096이므로 4097, 2049 가능,  (BoxSize 2048, 2box) 
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS) //son210319 //son230216_1
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 0
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049 
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3  
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

                setCombo_DefaultLowerStartPin(E_4097, bForce);  //son201222 //son210514
                //SysSet12.m_nUpperBox = 2;    //son240516 초기화
            }

            //son 32K DD HalfPin이 8192이므로 8193, 4097, 2049 가능,  (BoxSize 2048, 4box) 
            // 주의: PIN_TOT_16K도 여기로 들어오게 됨. PIN_TOT_16K, DD 하부시작핀 4097도 이 안에 있으므로 OK.
            else                                                                                    
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 0
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049 
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3:E_8193 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_6145].str);  // 6:E_6145    //son240522 상부 3 block 기능추가
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525
                setCombo_DefaultLowerStartPin(E_8193, bForce); //son201222 
                //SysSet12.m_nUpperBox = 4;    //son240516 초기화
            }
            break;

        case BOX_QD:
        default:
            //son MaxPin은 8K. QD HalfPin이 4096이므로  4097만  가능   (Boxsize 4096, 1box)
            if (nMaxPin == PIN_TOT_8K_TRANS)       //son210319 //son230216_1
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 0
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 1
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);      // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, "<Not_Set>");                             // 3
                m_combo_LowerJigStartPin.InsertString(-1, "<Not_Set>");                             // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 7    //son210525

                setCombo_DefaultLowerStartPin(E_4097, bForce);  //son201222  //son210514
                //SysSet12.m_nUpperBox = 1;    //son240516 초기화

            }

            //son 16K QD HalfPin이 8192이므로 8193, 4097 가능   (Boxsize 4096, 2box)
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS) //son210319 //son230216_1
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 0
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 1
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);      // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);      // 3:E_8193
                m_combo_LowerJigStartPin.InsertString(-1, "<Not_Set>");                             // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 7    //son210525
                setCombo_DefaultLowerStartPin(E_8193, bForce);  //son201222 //son210514
                //SysSet12.m_nUpperBox = 2;    //son240516 초기화
            }

            //son 32K QD HalfPin이 16384이므로 16385, 8193, 4097 가능 (Boxsize 4096, 4box)
            // 주의: PIN_TOT_16K도 여기로 들어오게 됨. PIN_TOT_16K, QD 하부시작핀 8193도 이 안에 있으므로 OK.
            else
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 0
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 1
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);      // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);      // 3:E_8193
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_16385].str);     // 4:E_16385
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_12289].str);     // 7:E_12289    //son240522 상부 3 block 기능추가
                setCombo_DefaultLowerStartPin(E_16385, bForce); //son201222  //son210514
                //SysSet12.m_nUpperBox = 4;    //son240516 초기화
            }
            break;
    }


}

//son201222 현재 System의 하부시작핀 값이 ComboBox 안에 있다면 유지 시켜 주고 없다면 default값으로 설정해 준다.
// return false: 현재 SysSet12.m_nLowerJigStartPin 을 유지      //son240516 return 값 추가
// return true:  요청한 default 값으로 변경 
bool CModeSet12::setCombo_DefaultLowerStartPin(int nDefaultMode_LowerStart, bool bForce)  //son210514 bForce 추가
{
    bool bRet = false;  //son240516
    int nModeLowerStart;
    CString strJigStart;        
    //nModeLowerStart =  m_LowerStartPin.findItemNo(LOWER_STARTPIN_NUM, SysSet12.m_nLowerJigStartPin);
    //if (nModeLowerStart < 0)       
    //son 현재 System의 하부시작핀 값이 ComboBox 안에 없다면 default값으로 설정한다.
    strJigStart.Format("%d", SysSet12.m_nLowerJigStartPin);        
    nModeLowerStart =  m_combo_LowerJigStartPin.FindString(-1, strJigStart);
    if (nModeLowerStart == CB_ERR   //son Combo안에 현재 하부시작핀 값이 없거나
            || bForce == TRUE)      //son210514 무조건 default 값으로 설정하는 옵션이 On이라면 
    {
#if 0
        //sylee230224      m_combo_LowerJigStartPin.SetCurSel(nDefaultMode_LowerStart); 
	    m_combo_LowerJigStartPin.SetCurSel(nModeLowerStart);	  //sylee230224  
#else
        m_combo_LowerJigStartPin.SetCurSel(nDefaultMode_LowerStart);    
#endif
        OnSelchangeComboLowerStartPin(); //son 변경한 LowerJigStartPin combobox 설정을 내부 변수에 반영
        bRet = true;  //son240516
    }   
    else 
        //son 현재 SysSet12.m_nLowerJigStartPin 이 콤보박스에 보이게 설정
        m_combo_LowerJigStartPin.SetCurSel(nModeLowerStart);

    //son combo박스에 값을 반영한다.
    UpdateData(FALSE);      

    return bRet;   //son240516
}

//son241007-BSW-9  m_nLowerEndBox_EndPin[i]를 생성하고 EndPin Combo를 채우고 SysSet12.m_nEndPin도 재설정한다.
int CModeSet12::initCombo_EndPin_BlockSet()  
{
    int i;

    ASSERT(SysSet12.m_nBlockSet1  == 1); 

    // Determine the pin count for the upper box type
    int nUpperPin = 0;
    int upper_pins[MAX_BOX_PER_PART]= {0, 0, 0, 0};
    for (i= 0; i < MAX_BOX_PER_PART; i++)
    {
        switch(i)
        {
            case 0: upper_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDUpper); break;
            case 1: upper_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDUpper2); break;
            case 2: upper_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDUpper3); break;
            case 3: upper_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDUpper4); break;
            default: break;
        }
        nUpperPin += upper_pins[i];
    }

    //son m_nLowerJigStartPin_Offset이 추가 설정된  경우에는 EndPin도 같이 밀려야 한다. 
    nUpperPin += SysSet12.m_nLowerJigStartPin_Offset; //son241007-BSW-9  

    //---------------------------------------------------------------------------------
    //son UpperBox 개수로  EndPin 케이스 4가지를 만들어서 EndPin combo 박스에 넣는다.
    //    이 단계에서는 아직 m_nLowerBox를 고정하지 않고 box 4가지 case를 combo에 넣어야 한다.  
    //    m_combo_EndPin 콤보박스는 4개의 항목을 가지며 각 항목의 combo 위치는 박스번호-1 (0~3)이다.

    int nEndPin = nUpperPin;
    int lower_pins[MAX_BOX_PER_PART]= {0, 0, 0, 0};
    CString strLowerPin[MAX_BOX_PER_PART]; 
    for (i= 0; i < MAX_BOX_PER_PART; i++)
    {
        switch(i)
        {
            case 0: lower_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDLower); break;
            case 1: lower_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDLower2); break;
            case 2: lower_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDLower3); break;
            case 3: lower_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDLower4); break;
            default: break;
        }

        nEndPin += lower_pins[i];
        m_nLowerEndBox_EndPin[i] = nEndPin;
        strLowerPin[i].Format("%dK", (m_nLowerEndBox_EndPin[i] /1024));

        m_combo_EndPin.InsertString(-1, strLowerPin[i].GetBuffer(10));           
    }

    //son 현재 FileSysInfo의 SysSet12.m_nLowerBox 값으로  m_combo_EndPin, SysSet12.m_nEndPin을 설정.
    SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[SysSet12.m_nLowerBox -1];   //son241126-BSW-222


    //son FileSysInfo의 SysSet12.m_nLowerBox값이  없다면 4번째 box 최대값으로 default 지정한다. 
    if (SysSet12.m_nLowerBox <= 0 || SysSet12.m_nLowerBox > MAX_BOX_PER_PART) 
    {
        SysSet12.m_nLowerBox = 4;  //son 4번째 box
        SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[BOX_D];
    }


    //son 현재 FileSysInfo의 SysSet12.m_nLowerBox 값으로  m_combo_EndPin을 설정.
    m_combo_EndPin.SetCurSel((SysSet12.m_nLowerBox -1));
    return 0;


}

//son240516 추가 
//son m_nLowerEndBox_EndPin[i]를 생성하고 EndPin Combo를 채우고 SysSet12.m_nEndPin도 재설정한다.
int CModeSet12::initCombo_EndPin(int nMaxPin)  
{

    m_combo_EndPin.ResetContent();

    //son nMaxPin이  PIN_TOT_32K_ORG 일 때에만 initCombo_EndPin() 정상동작한다.
    if (nMaxPin != PIN_TOT_32K_ORG)
    {
        UpdateData(TRUE);       // 화면의 값을 가져온다.
        //son PIN_TOT_32K_ORG가 아니면 EndPin 콤보박스는 Disable 
        GetDlgItem(IDC_COMBO_END_PIN)->EnableWindow(FALSE);
        return -1;
    }

    ASSERT(SysSet12.m_nUpperBox == 4 || SysSet12.m_nUpperBox == 3 || SysSet12.m_nUpperBox == 2 || SysSet12.m_nUpperBox == 1);


    GetDlgItem(IDC_COMBO_END_PIN)->EnableWindow(TRUE);   //son241126-BSW-222

    if( SysSet12.m_nBlockSet1  == 1) { //son241007-BSW-9
        //son SysSet12.m_nPinQDUpper~SysSet12.m_nPinQDUpper4와 SysSet12.m_nPinQDLower~m_nPinQDLower4로 혼용하는 경우
        return initCombo_EndPin_BlockSet(); 
    }


    //-------------------------------------------------------------------------------------------
    //son Upper는 SysSet12.m_nPinQDLower  고정이고, Lower는 SysSet12.m_nPinQDLower  고정인 경우
    //-------------------------------------------------------------------------------------------


    // Determine the pin count for the upper box type
    int nUpperPin = 0; //son241007-BSW-9
    nUpperPin = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDUpper) * SysSet12.m_nUpperBox; 
    if (nUpperPin == 0) {
        MyTrace(PRT_BASIC, "initCombo_EndPin(): Invalid upper box type. nUpperDensity=%d, SysSet12.m_nUpperBox=%d \n",
                                SysSet12.m_nPinQDUpper, SysSet12.m_nUpperBox);
        return -3;
    }

    //---------------------------------------------------------------------------------
    //son UpperBox 개수로  EndPin 케이스 4가지를 만들어서 EndPin combo 박스에 넣는다.
    //    이 단계에서는 아직 m_nLowerBox를 고정하지 않고 box 4가지 case를 combo에 넣어야 한다.  
    //    m_combo_EndPin 콤보박스는 4개의 항목을 가지며 각 항목의 combo 위치는 박스번호-1 (0~3)이다.

    int lower_pins[MAX_BOX_PER_PART]= {0, 0, 0, 0};
    CString strLowerPin[MAX_BOX_PER_PART]; 
    for (int i= 0; i < MAX_BOX_PER_PART; i++)
    {
        lower_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDLower) * (i + 1);
        m_nLowerEndBox_EndPin[i] = nUpperPin + lower_pins[i]; //son241007-BSW-9
        strLowerPin[i].Format("%dK", (m_nLowerEndBox_EndPin[i] /1024));

        m_combo_EndPin.InsertString(-1, strLowerPin[i].GetBuffer(10));           
    }

    //son 현재 FileSysInfo의 SysSet12.m_nLowerBox 값으로  m_combo_EndPin, SysSet12.m_nEndPin을 설정.
    SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[SysSet12.m_nLowerBox -1];      //son241126-BSW-222

    //son FileSysInfo의 SysSet12.m_nLowerBox값이  없다면 4번째 box 최대값으로 default 지정한다. 
    if (SysSet12.m_nLowerBox <= 0 || SysSet12.m_nLowerBox > MAX_BOX_PER_PART) 
    {
        SysSet12.m_nLowerBox = 4;  //son 4번째 box
        SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[BOX_D];
    }

    m_combo_EndPin.SetCurSel(SysSet12.m_nLowerBox -1);   
    return 0;

}



int CModeSet12::getBoxPinCount(PIN_BOXTYPE boxDensity)     //son240516
{
    switch (boxDensity) {
        case BOX_QD: return (int)QD_PINS;
        case BOX_DD: return (int)DD_PINS;
        case BOX_SD: return (int)SD_PINS;
        default: return 0;
    }
}


//son240516 Function to calculate total pins
int CModeSet12::calculate_TotalPins(PIN_BOXTYPE nUppderDensity, int nUpperBox, PIN_BOXTYPE nLowerDensity, int nLowerBox) 
{
    int upper_pins = 0;
    int lower_pins = 0;

    // Determine the pin count for the upper box type
    upper_pins = getBoxPinCount(nUppderDensity) * nUpperBox; 
    if (upper_pins == 0) {
        MyTrace(PRT_BASIC, "Invalid upper box type\n");
        return -1;
    }
    
    // Determine the pin count for the lower box type
    lower_pins = getBoxPinCount(nLowerDensity) * nLowerBox; 
    if (lower_pins == 0) {
        MyTrace(PRT_BASIC, "Invalid lower box type\n");
        return -1;
    }

    // Calculate the total pins
    return upper_pins + lower_pins;
}



//son MaxPin 값 변경시
void CModeSet12::OnSelchangeCombo1() 
{
    //------------------------------
    //son MaxPin 값 UI에서 가져오기 

    CString str, out;
    int nNewPinTotMax = m_combo1.GetCurSel();


    //son-32KPIN-20200103
    //------------------------------------------------------------------------------
    //son PinBlockType이 32K인데 PinTotMax 값을 32K 아닌값을 설정했다면     
    //    거부하고  원래의 값으로 되돌린다. PinBlockType을 먼저 설정해야 함.
    //------------------------------------------------------------------------------
 
    // 새로 입력된 값이 기존값과 다르다면 메시지출력
    if (SysSet12.m_nPinTotMax != nNewPinTotMax) 
    {   
        //------------------------------------------
        //  PinBlockType과 PinTotMax 불일치 확인 
        //------------------------------------------
        
        //son  No 32K PinBlock타입인데  32K Max Pin 체크
        if (SysSet13.m_nPinBlockType != PIN_BLK_32K_TRANS)    // prev value //son230216_1
        {
            //son PinBlockType을 32K로 입력하지 않고 PinTotMax 값을 32K로 변경하는 것은 거부한다.
            if(IS_TRANS_TOTMAX(nNewPinTotMax))  //son210319
            {
//son230217_2 begin:
                str.Format("PinBlockType= %s \n\n  MaxPin=%s is not matched. This won't change.\n",
                        STR_PIN_BLOCK(SysSet13.m_nPinBlockType), STR_MAX_PIN(nNewPinTotMax));
                out = str;
                str.Format("If you want to change this, \n Go to Menu-> MAP -> \"Set1\" first,\n");   
                out += str;
                str.Format("then change the PinBlockType to 32K type and change MaxPin again.\n");
                out += str;
                errMessageBox(7701, out);
//son230217_2 end   

                // 원래 SysSet12.m_nPinTotMax의 값으로 복구한다.
                m_combo1.SetCurSel(SysSet12.m_nPinTotMax);     

                //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
                initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

                // 변경된 하부시작핀에 맞는 EndPin combo 설정.
                initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

                UpdateData(FALSE);      // 화면에 값을 반영.
                return;
            }   

        }

        //son  32K -> No 32K
        if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) // prev Value //son230216_1
        {
            //son PinBlockType을 no 32K로 입력하지 않고 PinTotMax 값을 no 32K로 변경하는 것은 거부한다.
            if(!IS_TRANS_TOTMAX(nNewPinTotMax))  //son210319
            {
//son230217_2 begin:
                str.Format("PinBlockType= 32K \n\n  MaxPin=%s is not matched. This won't change.\n",
                        STR_MAX_PIN(nNewPinTotMax));
                out = str;
                str.Format("If you just want to change this, \n Go to Menu-> MAP -> \"Set1\" first,\n");   
                out += str;
                str.Format("then change the PinBlockType to QD or DD type and change MaxPin again.");
                out += str;
                errMessageBox(7701, out);   
//son230217_2 end
                // 원래 SysSet12.m_nPinTotMax의 값으로 복구한다.
                m_combo1.SetCurSel(SysSet12.m_nPinTotMax);  

                //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
                initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

                // 변경된 하부시작핀에 맞는 EndPin combo 설정.
                initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

                UpdateData(FALSE);      // 화면에 값을 반영.
                return;
            }   
        }

//son230216_2 begin:
        //--------------------
        // 32K Original K600
        if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG)    
        {
            //son 23.02.17일 기준으로 32K Original 타입은 U8K_L24K(K600), PIN_TOT_32K_ORG임. 이외의 타입으로 변경하는 걸 거부한다.
            if (nNewPinTotMax != PIN_TOT_32K_U8K_L24K && nNewPinTotMax != PIN_TOT_32K_ORG)
            {
                str.Format("If the PinBlockType is \'32K (Original)\', the PinMode must be  \'32K U 8K, L 24K\'\n");
                out = str;
                str.Format("PinMode(%s) is not applicable.\n", STR_MAX_PIN(nNewPinTotMax));
                out += str;
                errMessageBox(7701, out); //son230217_2

                // 원래 SysSet12.m_nPinTotMax의 값으로 복구한다.
                m_combo1.SetCurSel(SysSet12.m_nPinTotMax);  

                //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
                initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

                // 변경된 하부시작핀에 맞는 EndPin combo 설정.
                initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

                UpdateData(FALSE);      // 화면에 값을 반영.
                return;
            }
        }   

        //son Org 32K 가 아닌데  U8K_L24K(K600)으로 설정하는 것은 거부한다.
        if (SysSet13.m_nPinBlockType != PIN_BLK_32K_ORG 
                && (nNewPinTotMax == PIN_TOT_32K_U8K_L24K || nNewPinTotMax == PIN_TOT_32K_ORG))
        {
            str.Format("PinMode(%s) cannot be applied because the pin block is not \'32K (Original)\'.\n", STR_MAX_PIN(nNewPinTotMax));
            out = str;
            str.Format("First, please go to Menu > Map > Set1, and change the Pin Block to \'32K (Original)\'.\n");
            out += str;
            errMessageBox(7701, out); //son230217_2

            // 원래 SysSet12.m_nPinTotMax의 값으로 복구한다.
            m_combo1.SetCurSel(SysSet12.m_nPinTotMax);  

            //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
            initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

            // 변경된 하부시작핀에 맞는 EndPin combo 설정.
            initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

            UpdateData(FALSE);      // 화면에 값을 반영.
            return;

        }
//son230216_2 end

        SysSet12.m_nPinTotMax = nNewPinTotMax;    

//son230221 begin:
        switch(SysSet12.m_nPinTotMax)
        {
            //son230216_2 begin
            case PIN_TOT_32K_U8K_L24K:
                //son K600은 BlockSet1 사용 안한다. Off여야 함.  
                //    Lower가 6 box 여야 하는데 허용하면 UI 바꿔야 한다. (Pin 설정 UI 230216 현재  Lower 4개까지만 설정 가능)
                SysSet12.m_nBlockSet1  = 0; 
                m_check4.SetCheck(0);
                break;
            //son230216_2 end

            case PIN_TOT_16K_U12K_L4K_TRANS:    //son210525
                //son210525 U12K_L4K인 경우는 BlockSet1 ON 이어야만 한다.  
                SysSet12.m_nBlockSet1  = 1; 
                m_check4.SetCheck(1);
                break;

            default:
                //son210531 Upper Trans는 BlockSet1  사용 안한다. Off 여야 함.
                if(IS_UPPER_TRANS(SysSet12.m_nPinTotMax))    //son210531
                {
                    SysSet12.m_nBlockSet1  = 0; 
                    m_check4.SetCheck(0);
                }
                break;
        }
//son230221 end

        EnableDisable_QDDDSDControl();  //son210514


        //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
        MakePinBlockRangeUserNo();//son210514
        initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
        initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9


    }


    UpdateData(FALSE);      // 화면에 값을 반영.    //son210215
}



void CModeSet12::OnSelchangeCombo2() 
{
    SysSet12.m_nPinBoxMax = m_combo2.GetCurSel();
}

void CModeSet12::OnSelchangeCombo3() 
{
    SysSet12.m_nPinSwitchMode = m_combo3.GetCurSel();
}

void CModeSet12::OnSelchangeCombo4() 
{
    SysSet12.m_nPinType = m_combo4.GetCurSel();
}


void CModeSet12::OnSelchangeCombo5() 
{
    SysSet12.m_nCardReverse = m_combo5.GetCurSel();
}

void CModeSet12::OnSelchangeCombo8() 
{
    SysSet12.m_nCardReverseB = m_combo8.GetCurSel();
}



void CModeSet12::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
 
      m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupPin", 101));
      m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupPin", 102));
      m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupPin", 103));
      m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupPin", 104));
      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupPin", 105));
      m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupPin", 106));
      m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupPin", 107));
      m_Label1008.SetWindowText(LoadMessage(1,"TitleSetupPin", 108));
      m_Label1009.SetWindowText(LoadMessage(1,"TitleSetupPin", 109)); 
      m_Label1010.SetWindowText(LoadMessage(1,"TitleSetupPin", 110));
      m_Label1011.SetWindowText(LoadMessage(1,"TitleSetupPin", 111)); 

      m_button1001.SetWindowText(LoadMessage(1,"TitleSetupPin", 112));
      m_button1002.SetWindowText(LoadMessage(1,"TitleSetupPin", 113));
      m_button1003.SetWindowText(LoadMessage(1,"TitleSetupPin", 114));   
  } 

}


//son Upper1 QD
void CModeSet12::OnCheck1()  
{

    //if( SysSet13.m_nPinBlockType==1) { //sylee161108-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_DD) //son190902 32KPIN 
    {
        m_check1.SetCheck(0); 
        return;
    }
    if(m_check1.GetCheck())     //son 0:QD, 2: SD, 3: S100-1, 4: S100-2, 5: YAMAHA
    {
        SysSet12.m_nPinQDUpper  = BOX_QD;       //son 1:QD
        m_check1.SetCheck(1); 
        m_check2.SetCheck(0); 
        m_check3.SetCheck(0);       

        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K 관련타입에서 Upper2,Upper3, Upper4는 Upper1을 따른다
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDUpper2 = BOX_QD;
                m_check8.SetCheck(1); 
                m_check9.SetCheck(0); 
                m_check10.SetCheck(0);      
//son230217 begin:
                //son U8K, 24K인 경우는 QD Upper2까지만 같이 QD로 설정 
                if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K)  
                {
                    //son QD Upper3, 4도 QD로 설정
                    SysSet12.m_nPinQDUpper3 = BOX_QD;   //son210531 콤보
                    SysSet12.m_nPinQDUpper4 = BOX_QD;   //son210531 콤보
                    EnableDisable_QDDDSDControl();      //son210531
                }
//son230217 end
            }
        }
    }
  
    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514   여기서 하부시작핀도 결정된다.
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9
}



//son Upper1 DD
void CModeSet12::OnCheck2()  
{
    //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //|| SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {  
    //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5) 
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {    
        m_check2.SetCheck(0); 
        return ;    
    }
    if(m_check2.GetCheck())
    {
        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper로 변경
        SysSet12.m_nPinQDUpper  = BOX_DD;   //son 2:DD
        m_check1.SetCheck(0); 
        m_check2.SetCheck(1);   //son Upper1 DD On 
        m_check3.SetCheck(0); 

        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K 관련타입에서 Upper 2, 3, 4는 Upper1을 따르도록 수정
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDUpper2 = BOX_DD;
                m_check8.SetCheck(0); 
                m_check9.SetCheck(1);   //son Upper2 DD On 
                m_check10.SetCheck(0);      

                //son U8K, 24K인 경우는  Upper2까지만 같이 DD로 설정 
                if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    //son 그외는 DD Upper3, 4도 DD로 설정
                    SysSet12.m_nPinQDUpper3 = BOX_DD;   //son210531
                    SysSet12.m_nPinQDUpper4 = BOX_DD;   //son210531
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }
  
    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9
}

//son Upper1 SD
void CModeSet12::OnCheck3()  
{
    // 5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //  || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 ) {     
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {
        m_check3.SetCheck(0); 
        return ;    
    }

    if(m_check3.GetCheck())
    {
        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper로 변경
        SysSet12.m_nPinQDUpper  = BOX_SD;       //son 3: SD
        m_check1.SetCheck(0); 
        m_check2.SetCheck(0); 
        m_check3.SetCheck(1);     
    
        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K 관련타입에서 Upper 2, 3, 4는 Upper1을 따르도록 수정
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDUpper2 = BOX_SD;
                m_check8.SetCheck(0); 
                m_check9.SetCheck(0); 
                m_check10.SetCheck(1);      

                //son U8K, 24K인 경우는 Upper2까지만 같이 SD로 설정 
                if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    //son  Upper3, 4도 SD로 설정
                    SysSet12.m_nPinQDUpper3 = BOX_SD;   //son210531
                    SysSet12.m_nPinQDUpper4 = BOX_SD;   //son210531
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }
  
    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9
}


//son Manual QD/DD/SD Block Set 
void CModeSet12::OnCheck4()  
{
    //son BlockSet On
    if(m_check4.GetCheck())
    { 
        //son210531 U12K_L4K Upper Trans 인 경우는 BlockSet1  Off 이어야만 한다. 그래서 On 할 수 없음.
        //          복잡도를 덜기 위함임.
        if (IS_UPPER_TRANS(SysSet12.m_nPinTotMax)
                ||  OnPinConvertType1 == 1)  //son241007-BSW-9  
        {
            CString str;
            //str.Format("Can't On. BlockSet checkBox in %s mode.\n", STR_MAX_PIN(SysSet12.m_nPinTotMax));
            //errMessageBox(11203, str); //son230217_2

            //son .CON 파일이 존재하는 경우는 QD/DD/SD 혼용까지 하면 이중으로 변형하게 되므로 BlockSet1 설정을 거부한다. 
            if (OnPinConvertType1 == 1) //son241007-BSW-9 begin
            {
                str.Format("BDL(%s) has .CON file, so you can't turn on this checkBox.\n", SysInfoView01.m_pStrFileBDLName);
                errMessageBox(11203, str); 
            } //son241007-BSW-9 end

            SysSet12.m_nBlockSet1 = 0;  //son 이전 상태로 돌림. 현상유지
            m_check4.SetCheck(0);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }

        //son BlockSet ON
        else
        {
            SysSet12.m_nBlockSet1  = 1; 
            m_check4.SetCheck(1);

            MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
            initCombo_LowerStartPin(BOX_QD, SysSet12.m_nPinTotMax, TRUE); //son210514 QD 기준으로 default 하부시작핀을 콤보박스에 설정
            initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }
    }

    //son BlockSet Off
    else
    {
        //son210525 16K_U12K_L4K인 경우는 BlockSet1 이어야만 한다. 그래서 Off 할 수 없음.
        if (SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS)    //son210525
        {
            //CString str;
            //str.Format("Can't Off BlockSet checkBox in %s mode.\n", STR_MAX_PIN(SysSet12.m_nPinTotMax));
            //errMessageBox(str); //son230217_2

            SysSet12.m_nBlockSet1  = 1;     //son 이전 상태로 돌림. 현상유지
            m_check4.SetCheck(1);
            UpdateData(FALSE);      // UI에 값을 반영한다.
        }

        //son BlockSet OFF
        else
        {
            SysSet12.m_nBlockSet1  = 0; 
            m_check4.SetCheck(0);

            MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
            initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
            initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
        }
    }

    //son QD/DD/SD 컨트롤을 상황에 맞게 각 컨트롤들을 Enable Disable한다.
    EnableDisable_QDDDSDControl();  //son210514
  
}

//son Lower1 QD
void CModeSet12::OnCheck5()  
{

    //if( SysSet13.m_nPinBlockType==1) { //sylee161108-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_DD) //son190902 32KPIN 
    { 
        //son  하부 check box와 QD BlockType을  DD로 고정해야 하므로 QD 설정을 거부한다
        m_check5.SetCheck(0); 
        return;
    }

    //if( SysSet13.m_nPinBlockType==7) { //sylee180221-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //son190902 32KPIN 
    {
        //son  하부 check box와 QD BlockType을  DD로 고정해야 하므로 QD 설정을 거부한다
        m_check5.SetCheck(0); 
        return;
    }

    if(m_check5.GetCheck())
    {
        SysSet12.m_nPinQDLower  = BOX_QD;   //son201218
        m_check5.SetCheck(1); 
        m_check6.SetCheck(0); 
        m_check7.SetCheck(0);   

        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K 관련타입에서 Lower2는 Lower1을 따르도록 수정
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDLower2 = BOX_QD;
                m_check11.SetCheck(1); 
                m_check12.SetCheck(0); 
                m_check13.SetCheck(0);      

                SysSet12.m_nPinQDLower3 = BOX_QD;   //son210531
                SysSet12.m_nPinQDLower4 = BOX_QD;   //son210531

                //son U8K, 24K인 경우는  Lower 5, 6도 QD로 설정
                if (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    SysSet12.m_nPinQDLower5 = BOX_QD;   //son230217
                    SysSet12.m_nPinQDLower6 = BOX_QD;   //son230217
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }

    }  
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9

}

//son Lower1 DD
void CModeSet12::OnCheck6()  
{
    // 5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5) {    
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  	//son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {    
        m_check6.SetCheck(0); 
        return ;    
    }
    if(m_check6.GetCheck())
    {
        SysSet12.m_nPinQDLower  = BOX_DD;       //son201218 
        m_check5.SetCheck(0); 
        m_check6.SetCheck(1); 
        m_check7.SetCheck(0);   

        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K 관련타입에서 Lower2는 Lower1을 따르도록 수정
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDLower2 = BOX_DD;
                m_check11.SetCheck(0); 
                m_check12.SetCheck(1); 
                m_check13.SetCheck(0);      

                SysSet12.m_nPinQDLower3 = BOX_DD;   //son210531
                SysSet12.m_nPinQDLower4 = BOX_DD;   //son210531

                //son U8K, 24K인 경우는 Lower 5, 6도 DD로 설정
                if (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    SysSet12.m_nPinQDLower5 = BOX_DD;   //son230217
                    SysSet12.m_nPinQDLower6 = BOX_DD;   //son230217
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }  
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9
}

//son Lower1 SD
void CModeSet12::OnCheck7()  
{
    //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //                  || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5) {    
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {    
        m_check7.SetCheck(0); 
        return ;    
    }

    if(m_check7.GetCheck())
    {
        SysSet12.m_nPinQDLower  = BOX_SD;   //son201218 Lower1 BoxType : SD
        m_check5.SetCheck(0); 
        m_check6.SetCheck(0); 
        m_check7.SetCheck(1);       

        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K 관련타입에서 Lower2는 Lower1을 따르도록 수정
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDLower2 = BOX_SD;
                m_check11.SetCheck(0); 
                m_check12.SetCheck(0); 
                m_check13.SetCheck(1);      

                SysSet12.m_nPinQDLower3 = BOX_SD;   //son210531
                SysSet12.m_nPinQDLower4 = BOX_SD;   //son210531

                //son U8K, 24K인 경우는 Lower 5, 6도 SD로 설정
                if (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    SysSet12.m_nPinQDLower5 = BOX_SD;   //son230217
                    SysSet12.m_nPinQDLower6 = BOX_SD;   //son230217
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }
  
    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
    //initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210514
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9
}


//son Upper2 QD
void CModeSet12::OnCheck8()  
{
    //if( SysSet13.m_nPinBlockType==1) { //sylee161108-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_DD) //son190902 32KPIN 
    { 
        m_check8.SetCheck(0); 
        return;
    }

    if(m_check8.GetCheck())
    {
        //son190902 32KPIN : m_nPinQD3->m_nPinQDUpper2로 변경
        SysSet12.m_nPinQDUpper2  = BOX_QD;  //son Upper2 BoxType : QD
        m_check8.SetCheck(1); 
        m_check9.SetCheck(0); 
        m_check10.SetCheck(0);      
    }  

    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210514
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9 
}

//son Upper2 DD
void CModeSet12::OnCheck9()  
{
    // 5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    // sylee180412-2    if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //                      || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //              || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 ) { 
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    { 
        m_check9.SetCheck(0); 
        return ;    
    }

    if(m_check9.GetCheck())
    {
        //son190902 32KPIN : m_nPinQD3->m_nPinQDUpper2로 변경
        SysSet12.m_nPinQDUpper2  = BOX_DD;  //son Upper2 BoxType: DD
        m_check8.SetCheck(0); 
        m_check9.SetCheck(1); 
        m_check10.SetCheck(0);      
    }  

    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210514
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}
 
//son Upper2 SD
void CModeSet12::OnCheck10()  
{
    
    ////5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //                  || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //              || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5) {   
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {   
        m_check10.SetCheck(0); 
        return ;    
    }

    if(m_check10.GetCheck())
    {
        //son190902 32KPIN : m_nPinQD3->m_nPinQDUpper2로 변경
        SysSet12.m_nPinQDUpper2  = BOX_SD;  //son Upper2 BoxType:  SD(3)
        m_check8.SetCheck(0); 
        m_check9.SetCheck(0); 
        m_check10.SetCheck(1);      
    }  

    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son210514 여기서 하부시작핀도 결정
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);     //son210514
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}


//son Lower2 QD
void CModeSet12::OnCheck11()  
{
    //if( SysSet13.m_nPinBlockType==1) { //sylee161108-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_DD) //son190902 32KPIN 
    {
        //son  하부 check box와 QD BlockType을  DD로 고정해야 하므로 QD 설정을 거부한다
        m_check11.SetCheck(0); 
        return;
    }
    //if( SysSet13.m_nPinBlockType==7) { //sylee180221-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //son190902 32KPIN 
    {
        //son  하부 check box와 QD BlockType을  DD로 고정해야 하므로 QD 설정을 거부한다
        m_check11.SetCheck(0); 
        return;
    }

    if(m_check11.GetCheck())
    {
        SysSet12.m_nPinQDLower2  = 1;       //son Lower2 BoxType:  QD(1)
        m_check11.SetCheck(1); 
        m_check12.SetCheck(0); 
        m_check13.SetCheck(0);      
    }  
    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}

//son Lower2 DD
void CModeSet12::OnCheck12()  
{

    //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5) {    
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {    
        m_check12.SetCheck(0); 
        return ;    
    }
    if(m_check12.GetCheck())
    {
        SysSet12.m_nPinQDLower2  = 2;   //son Lower2 BoxType:  DD(2)
        m_check11.SetCheck(0); 
        m_check12.SetCheck(1); 
        m_check13.SetCheck(0);      
    }  
    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}
 
//son Lower2 SD
void CModeSet12::OnCheck13()  
{

    ////5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    //sylee180412-2 if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //                  || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 || SysSet13.m_nPinBlockType==6) {    
    //if( SysSet13.m_nPinBlockType==2  || SysSet13.m_nPinBlockType==3 
    //          || SysSet13.m_nPinBlockType==4  || SysSet13.m_nPinBlockType==5 ) {   
    
    if( SysSet13.m_nPinBlockType== PIN_BLK_SD  
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE  //son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA) //son190902 32KPIN 
    {   
        m_check13.SetCheck(0); 
        return ;    
    }

    if(m_check13.GetCheck())
    {
        SysSet12.m_nPinQDLower2  = 3;       //son Lower2 SD
        m_check11.SetCheck(0); 
        m_check12.SetCheck(0); 
        m_check13.SetCheck(1);      
    }  
    MakePinBlockRangeUserNo();//son241007-BSW-9/
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}

//nt m_nPinQDUpper;// QD, DD, SD
 


void CModeSet12::OnSave() 
{

    UpdateData(TRUE);   
    FileSysInfo01.LoadSaveSet12(_SAVE);  

}
 


//son 32K_TRANS, 20K_TRANS, 24K_TRANS에서는 아래 코드 미사용.
// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
int CModeSet12::OnPinConvert1() 
{
    CString str; 
    int No,Mok1,Namo,BoxMok2,i,nMaxPin1,nMaxPinHalf1; 
    
    //son sPinBlockRange[10]를  생성하고 "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
    MakePinBlockRangeUserNo();//sylee210402 

    if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //SYLEE121003
        nMaxPin1=4096;      
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
        nMaxPin1=4096*2;
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
        nMaxPin1=4096*4;
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS){//32k //son230216_1
        nMaxPin1=4096*8;
    }
    else {//32k와 동일.  20K, 24K
        nMaxPin1=4096*8;
    }
    nMaxPinHalf1=nMaxPin1/2;
 
    memset(PinMap, -1, sizeof(PinMap));//sylee180323
    memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303

    //sylee140918
    int nBoxPinFull,nBoxPinHalf,nBoxPinSum,nStep;
    int nBoxPin[10],nBoxType[10];   //son nBoxType[1] : Upper           
                                    //son nBoxType[2] : Upper2
                                    //son nBoxType[3] : Lower           
                                    //son nBoxType[4] : Lower2          

    ::ZeroMemory(&nBoxPin,sizeof(nBoxPin));
    ::ZeroMemory(&nBoxType,sizeof(nBoxType));

    if(SysSet12.m_nPinBoxMax==1){  //2k
        nBoxPinFull=2048;
        nBoxPinHalf=1024;
    }
    else{ //4k
        nBoxPinFull=2048*2;
        nBoxPinHalf=1024*2;
    }

 
    if(SysSet12.m_nPinQDUpper== BOX_SD) {//3: sd    son190902 32KPIN: enum 변경
        nBoxType[1]= BOX_SD;
    }
    if(SysSet12.m_nPinQDUpper2== BOX_SD) {//3: sd
        nBoxType[2]= BOX_SD;
    }
    if(SysSet12.m_nPinQDLower== BOX_SD) {//3: sd
        nBoxType[3]= BOX_SD;
    }
    if(SysSet12.m_nPinQDLower2== BOX_SD) {//3: sd
        nBoxType[4]= BOX_SD;
    }


    if(nBoxType[1]== BOX_SD && nBoxType[2]== BOX_SD)//sylee161107-1
    {
        if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  
            nMaxPinHalf1=2048;
        }
        else if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
            nMaxPinHalf1=2048;
        }
    }


    nBoxPinSum=0;
    for(int i2=1; i2<=4; i2++)
    {
        if(nBoxType[i2]== BOX_SD){ //3:sd   son190902 32KPIN: enum 변경
            nBoxPinSum=nBoxPinSum+nBoxPinHalf;           
        }
        else{//dd, //qd
            nBoxPinSum=nBoxPinSum+nBoxPinFull;
        }
        nBoxPin[i2]=nBoxPinSum;
    }




    for (i=0; i< nMaxPin1; i++)//SYLEE121003
    { 

        No=i; 
        PinMap[0][i]=i;//****>net list pin no
        nStep=0;

        for(int i3=1; i3<=4; i3++){
            if(No>nBoxPin[i3]){
                nStep=i3;
                break;
            }
        }

        No=No-nBoxPin[nStep];
        if(nBoxType[nStep+1]== BOX_SD){ //3: SD son190902 32KPIN: enum 변경
            Namo=No%32; 
            Mok1=No/32;  
            No=Mok1*64+Namo;          
        }

        No=(nBoxPinFull*nStep)+No;

        /////////////////////////////////////////////////////////////////////////////////////////
/*//sylee210403
        if(SysSet12.m_n32k_16k==1){//3=SD->QD
            if(No>=(4096*2)){                    
                No=No+(4096*2);
            }
        } 
*/

        PinMap[1][i]=No;//****>QD pin no




        if(  SysSet12.m_nPinSwitchMode ==2)   //2=H, 1=Z  
        {
            Mok1=No/64;
            Namo=No%64;//same 

            if(SysSet12.m_nPinBoxMax==1){  //2k
                //sylee120925

                BoxMok2=Mok1/32;//same
                Mok1=Mok1%32;

                if(Mok1<16){
                    Mok1=Mok1*2;
                }
                else  if( Mok1<32){
                    Mok1=(Mok1-15)*2-1;
                }
                else {
                    str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                    errMessageBox(7701, str);   //son230217_2
                }
                No= (Mok1+(BoxMok2*32))*64+Namo;


            }
            else //SysSet12.m_nPinBoxMax==2)  //4k
            {

                BoxMok2=Mok1/64;//same
                Mok1=Mok1%64;
                if(Mok1<32){
                    Mok1=Mok1*2;
                }
                else  if( Mok1<64){
                    Mok1=(Mok1-31)*2-1;
                }
                else {
                    str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                    errMessageBox(7701, str); //son230217_2
                }
                No= (Mok1+(BoxMok2*64))*64+Namo;

            }


        }



        int BoxN;


        if(SysSet12.m_nPinBoxMax==1)
        {  //2k
            //sylee120925

            if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                BoxN=No/2048;
                No=No%2048;
                Mok1=No/128;//0`31 card
                Namo=No%128;//0`31 card
                No=Namo+((15-Mok1)*128);
                No=No+BoxN*2048;
            }

        }
        else
        {
            if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                BoxN=No/4096;
                No=No%4096;
                Mok1=No/128;//0`31 card
                Namo=No%128;//0`31 card
                No=Namo+((31-Mok1)*128);
                No=No+BoxN*4096;
            }
        }

        //********************************************************************************************* 
        //조이텍 전용 

        //2048,4096,6144,8192,10240,12288,14336
        //CONNECTOR  REVERSE = 64 PIN REVERSE
        if( SysSet12.m_nCardReverseB==2 ){  //SYLEE130809               
            Namo=No%128;
            Mok1=No/128;
            if(Namo<64){
                Namo=Namo+64;
            }
            else {
                Namo=Namo-64;//191=127+64
            }
            No=Mok1*128+Namo;
        }


        //if( SysSet13.m_nPinBlockType==6) { //sylee171109-1
        if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
        {
            if( (No>=2048) && (No<(4096))){//sylee171109-1
                No=No+2048;
            }
        }

        PinMap[2][i]=No;//****>SWITCH CARD pin no

        //////////////////////////////////////////////////////////////////////////////////////////sylee161107-1
        int ntmp1;//sylee161107-1//ACE400
        ntmp1=nBoxPin[2];//sylee161107-1 //ACE400

        if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k //sylee161108-1
            if(ntmp1>2048){
                ntmp1=2048;
            }                        
        }
        else if(SysSet12.m_nPinTotMax == PIN_TOT_8K){ //8k//sylee161108-1
            if(ntmp1>4096){
                ntmp1=4096;
            }                        
        }
        else if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k//sylee161108-1
            if(ntmp1>8192){
                ntmp1=8192;
            }                     
        }
        else if(SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS){//32k //sylee161108-1 //son230216_1
            if(ntmp1>(8192*2)){
                ntmp1=(8192*2);
            }
        }
        //if( SysSet13.m_nPinBlockType==4 ) { //sylee161110-1
        if( SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE ) { //son190902 32KPIN //son230216_1
            ntmp1=2048;
        }
        //if( SysSet13.m_nPinBlockType==6) { //sylee161110-1
        if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) { //son190902 32KPIN 
            ntmp1=2048*2;
        }


        if(i>=ntmp1)    //sylee161107-1 //ACE400
        {
            if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //sylee161107-1 //ACE400
                PinMap[2][i]=No+( 2048 + 4096 + 8192 ); 
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//sylee161107-1//8k
                PinMap[2][i]=No+( 4096 + 8192   ); 
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
                PinMap[2][i]=No+( 8192 ); 
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS){//32k //son230216_1
                PinMap[2][i]=No; 
            }
            else {//32k 
                PinMap[2][i]=No;//****>SWITCH CARD pin no
            }

            //if( SysSet13.m_nPinBlockType==4) { //sylee161110-1
            if( SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE) { //son190902 32KPIN : enum 변경 //son230216_1
                PinMap[2][i]=No+( 2048 + 4096 + 8192 );
            }

            //if( SysSet13.m_nPinBlockType==6) { //sylee171109-1 
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN : enum 변경
            {
                if((i>=4096)&&nBoxType[1]== BOX_SD && nBoxType[2]== BOX_SD){//sylee180412-3 // son190902 32KPIN: enum 변경   3->BOX_SD
                    No=No+( 8192 );//sylee180412-3
                }
                else{//sylee180412-3
                    No=No+( 4096 + 8192 );//sylee171109-1
                }//sylee180412-3

                if( No>=(4096*4+2048)){//sylee171109-1
                    No=No+2048;
                }
                PinMap[2][i]=No;//sylee171109-1

            }

        }
        else{//sylee161107-1 //ACE400
            PinMap[2][i]=No;//****>SWITCH CARD pin no
        }//sylee151006 //ACE400

        //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
        int hwPin = PinMap[2][i];					//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)			//son230303
            nJigPinMap[hwPin] = i;    //son 빠른 검색용. //son230303

        /////////////////////////////////////////////////////////////////////////////
    }//for( i=0; i< 33000; i++)



    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   //son210514 200 -> FILE_PATH_SIZE

    ::ZeroMemory(&fName, sizeof(fName));
    //strcat( fName , "d:\\PinMap.txt" ); 
    strcat( fName , "d:\\log\\PinMap.txt" ); //son220715 d:\\log 폴더로 이동

    fp = fopen(fName,"wt");
    if(fp == NULL) return 0;

    fprintf(fp, " \n     ==>   DD BLOCK  \n ");
    fprintf(fp, "   *User Pin*, / *Switch card Pin*, QdPin \n");
    for( i=0; i<nMaxPin1; i++){//SYLEE121003
        //fprintf(fp, "%10d, %10d, %10d\n",i+1,PinMap[2][i]+1, ,PinMap[1][i]+1);
        fprintf(fp, "%10d, %10d, %10d\n",i,PinMap[2][i], PinMap[1][i]);
    }
    fclose(fp);  

    MyTrace(PRT_BASIC, "OnPinConvert1(): %s PinMap Created.\n", fName);        //son220715

    return 1;

}




//sylee150530======================================
int CModeSet12::OnPinConvert1_QD()  //SYLEE150912-1  //sylee151006 kunshan  //ACE400
{
    CString str; 
    int No,Mok1,Namo,BoxMok2,i,nMaxPin1,nMaxPinHalf1;         
    int nBoxPinHalf_SS;//SYLEE150912-1
 

    if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //SYLEE121003
        nMaxPin1=4096;      
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
        nMaxPin1=4096*2;
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
        nMaxPin1=4096*4;
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //son230216_1
        nMaxPin1=4096*8;
    }
    else {//32k
        nMaxPin1=4096*8;
    }

    nMaxPinHalf1=nMaxPin1/2;  
    memset(PinMap, -1, sizeof(PinMap));//sylee151006
    memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303

    int nBoxPinFull,nBoxPinHalf,nBoxPinSum,nStep;
    int nBoxPin[10],nBoxType[10];   //son nBoxType[1] : Upper           
                                    //son nBoxType[2] : Upper2
                                    //son nBoxType[3] : Lower           
                                    //son nBoxType[4] : Lower2          

    ::ZeroMemory(&nBoxPin,sizeof(nBoxPin));
    ::ZeroMemory(&nBoxType,sizeof(nBoxType));

    if(SysSet12.m_nPinBoxMax==1){  //2k
        nBoxPinFull=2048;
        nBoxPinHalf=1024;
        nBoxPinHalf_SS=1024/2;//SYLEE150912-1
    }
    else { //4k
        nBoxPinFull=2048*2;
        nBoxPinHalf=1024*2;
        nBoxPinHalf_SS=1024;//SYLEE150912-1
    }

    //son190902 32KPIN : m_PinQD-> m_nPinQDUpper로 변경
    //                   m_nPinQD2->m_nPinQDLower로 변경
    //                   m_nPinQD3->m_nPinQDUpper2로 변경
    //                   m_nPinQD4->m_nPinQDLower2로 변경
    nBoxType[1]=SysSet12.m_nPinQDUpper;      
    nBoxType[2]=SysSet12.m_nPinQDUpper2;         
    nBoxType[3]=SysSet12.m_nPinQDLower;      
    nBoxType[4]=SysSet12.m_nPinQDLower2; 

    int nBoxPinSumOld;
    nBoxPinSum=0;
    nBoxPinSumOld=0;

    for(int i2=1; i2<=4; i2++)
    {
        if(nBoxType[i2]== BOX_SD) {//3: sd      son190902 32KPIN: enum 변경
            nBoxPinSum=nBoxPinSum+nBoxPinHalf_SS;//SYLEE150912-1 
        }
        else  if(nBoxType[i2]== BOX_DD){ // 2: dd//SYLEE150912-1    son190902 32KPIN: enum 변경
            nBoxPinSum=nBoxPinSum+nBoxPinHalf;  //SYLEE150912-1                      
        }
        else {//dd, //qd
            nBoxPinSum=nBoxPinSum+nBoxPinFull;
        }   

        //////////////////////////////////////////////////
        if(SysSet12.m_nPinTotMax == PIN_TOT_16K)//16k
        {
            //sylee151102   상부  sd sd인경우  하부시작번호  2k+1이 아니라 4k+1  
            if(nBoxType[1]== BOX_SD && nBoxType[2]== BOX_SD)  //son190902 32KPIN: enum 변경      3-> BOX_SD
            {
                if(i2==2){
                    nBoxPinSum=nBoxPinSum+4096;
                }
            }
        }
        /////////////////////////////////////////////////
        nBoxPin[i2]=nBoxPinSum;          
    }




    for(   i=0; i<nMaxPin1; i++)    //SYLEE121003
    {

        No=i; 
        PinMap[0][i]=i;//****>net list pin no   
        nStep=0;

        for(int i3=1; i3<=4; i3++){
            if((No+1)>nBoxPin[i3]){//sylee150916    
                nStep=i3;
                //sylee150916   break;
            }
            else {//sylee150916 
                break;//sylee150916 
            }//sylee150916  
        }           

        No=No-nBoxPin[nStep];

        if(nBoxType[nStep+1]== BOX_DD)      //sylee150912-1 //son190902 32KPIN: enum 변경      2-> BOX_DD
        {
            Namo=No%64;
            Mok1=Namo/32;
            No=No*2;
            if(Mok1==1){
                No++;
            }         
        }
        else  if(nBoxType[nStep+1]== BOX_SD)//sylee150912-1  SD //son190902 32KPIN: enum 변경      3-> BOX_SD
        {
            Namo=No%32; 
            Mok1=No/32;  
            No=Mok1*128+Namo*2;  
        } 
        No=(nBoxPinFull*nStep)+No;              
        ////////////////////////////////////////////////////////////////////////////////////////
/*//sylee210403
        if(SysSet12.m_n32k_16k==1){//3=SD->QD
            if(No>=(4096*2)){                    
                No=No+(4096*2);
            }
        } 
*/

        PinMap[1][i]=No;//****>QD pin no 

        if(  SysSet12.m_nPinSwitchMode ==2)  //2=H, 1=Z  
        {

            Mok1=No/64;
            Namo=No%64;//same 

            if(SysSet12.m_nPinBoxMax==1){  //2k
                //sylee120925                   
                BoxMok2=Mok1/32;//same
                Mok1=Mok1%32;

                if(Mok1<16){
                    Mok1=Mok1*2;
                }
                else  if( Mok1<32){
                    Mok1=(Mok1-15)*2-1;
                }
                else {
                    str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );  //son230217_2      
                    errMessageBox(7701, str); //son230217_2
                }
                No= (Mok1+(BoxMok2*32))*64+Namo;                      

            }
            else {//SysSet12.m_nPinBoxMax==2){  //4k

                BoxMok2=Mok1/64;//same
                Mok1=Mok1%64;
                if(Mok1<32){
                    Mok1=Mok1*2;
                }
                else  if( Mok1<64){
                    Mok1=(Mok1-31)*2-1;
                }
                else {
                    str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );  //son230217_2      
                    errMessageBox(7701, str);	//son230217_2
                }
                No= (Mok1+(BoxMok2*64))*64+Namo;
            }          

        }



        int BoxN;


        if(SysSet12.m_nPinBoxMax==1){  //2k
            //sylee120925
            if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                BoxN=No/2048;
                No=No%2048;
                Mok1=No/128;//0`31 card
                Namo=No%128;//0`31 card
                No=Namo+((15-Mok1)*128);
                No=No+BoxN*2048;
            }
        }
        else {
            if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                BoxN=No/4096;
                No=No%4096;
                Mok1=No/128;//0`31 card
                Namo=No%128;//0`31 card
                No=Namo+((31-Mok1)*128);
                No=No+BoxN*4096;
            }
        }

        //********************************************************************************************* 
        //조이텍 전용 
        if( SysSet12.m_nCardReverseB==2 ){  //SYLEE130809               
            Namo=No%128;
            Mok1=No/128;
            if(Namo<64){
                Namo=Namo+64;
            }
            else {
                Namo=Namo-64;//191=127+64
            }
            No=Mok1*128+Namo;
        }

        int ntmp1;//sylee151006 //ACE400
        ntmp1=nBoxPin[2];//sylee151006 //ACE400

        if(i>=ntmp1){//sylee151006 //ACE400
            if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  ///sylee151006 //ACE400
                PinMap[2][i]=No+( 2048 + 4096 + 8192 );//****>SWITCH CARD pin no     //sylee151006 //ACE400 
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
                PinMap[2][i]=No+( 4096 + 8192   );//****>SWITCH CARD pin no  //sylee151006 //ACE400
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
                PinMap[2][i]=No+( 8192 );//****>SWITCH CARD pin no  //sylee151006 //ACE400
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //son230216_1
                PinMap[2][i]=No;//****>SWITCH CARD pin no   //sylee151006 //ACE400
            }
            else {//32k 
                PinMap[2][i]=No;//****>SWITCH CARD pin no
            }               
        }
        else {//sylee151006 //ACE400
            PinMap[2][i]=No;//****>SWITCH CARD pin no
        }//sylee151006 //ACE400

        //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
        int hwPin = PinMap[2][i];				//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
            nJigPinMap[hwPin] = i;    //son 빠른 검색용. //son230303

    }//for( i=0; i< 33000; i++){


    return 1;

}




//sylee150530======================================

//son nPinMap[] 생성하고 D:\\PinMap.txt 파일에 write
//
//son nMode:  OnPinConvertType1값 .CON 파일이 의미있으면 1 (total이 1이상, checksum 정상)
int CModeSet12::OnPinConvert1_READ(int nMode)    //ACE400  //sylee151111add
{
    CString str; 
    int No,Mok1,Namo,BoxMok2,i;
    int nMaxPin1;                   //son Total Pin Num
    int nMaxPinHalf1;               //son nMaxPin1의 1/2 
    int nTotal1; 
    int nLoweStartPin1;
    int nBoxPinFull;                //son QD용 BoxPin수 (SysSet12.m_nPinBoxMax 4K면 4K, 2K면 2K)
    int nBoxPinHalf;                //son DD용 BoxPin수 (QD의 1/2) 
    int nBoxPinHalf_SS;             //son SD용 BoxPin수 (QD의 1/4) 
    int nBoxPinSum;             
    int nStep;
    int nBoxPin[10];                //son Box의 끝지점 PinNo

    int nBoxType[10];               //son SysSet12.m_nPinQDUpper ~ m_nPinQDLower2 (Upper1,Upper2,Lower1,Lower2)
                                    //son 1=QD, 2=DD, 3=SD
                                    //son nBoxType[1] : Upper           
                                    //son nBoxType[2] : Upper2
                                    //son nBoxType[3] : Lower           
                                    //son nBoxType[4] : Lower2          

    //son sPinBlockRange[10]를  생성하고 "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
    //MakePinBlockRangeUserNo();//sylee210402       //son 220422 현재 PinBlockRange를 이후 코드에서 현재 사용하고 있지 않으므로 코멘트처리
                                                    //    PinMap도 pinBlockRange와 무관하게 만들고 있다. 

    nLoweStartPin1=0;//sylee151112

    //======================================
    //son .CON 파일이 의미 없다면
    if(nMode==0)//sylee151111add
    {

        //----------------------
        // nMaxPin1 설정
        if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //SYLEE121003
            nMaxPin1=4096;      
        }
        else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
            nMaxPin1=4096*2;
        }
        else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
            nMaxPin1=4096*4;
        }
        else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //son230216_1
            nMaxPin1=4096*8;
        }
        else {//32k
            nMaxPin1=4096*8;
        }

        nMaxPinHalf1=nMaxPin1/2;  

        //if( SysSet13.m_nPinBlockType==7 ) {//sylee180323-1
        if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD ) { //son190902 32KPIN 
            nMaxPin1=4096*3;    //son MaxPin 12K
        }


        //--------------------------------
        // nBoxPinFull, nBoxType[] 설정  
        
        memset(PinMap, -1, sizeof(PinMap));//sylee151006
        memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303

        ::ZeroMemory(&nBoxPin,sizeof(nBoxPin));
        ::ZeroMemory(&nBoxType,sizeof(nBoxType));

        if(SysSet12.m_nPinBoxMax==1){  //2k
            nBoxPinFull=2048;
            nBoxPinHalf=1024;
            nBoxPinHalf_SS=1024/2;//SYLEE150912-1
        }
        else { //4k
            nBoxPinFull=2048*2;
            nBoxPinHalf=1024*2;
            nBoxPinHalf_SS=1024;//SYLEE150912-1
        }

        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper로 변경
        //                   m_nPinQD2->m_nPinQDLower로 변경
        //                   m_nPinQD3->m_nPinQDUpper2로 변경
        //                   m_nPinQD4->m_nPinQDLower2로 변경
        nBoxType[1]=SysSet12.m_nPinQDUpper;         //son Upper1 ex) 1  (QD)     
        nBoxType[2]=SysSet12.m_nPinQDUpper2;        //son Upper2 ex) 1  (QD) 
        nBoxType[3]=SysSet12.m_nPinQDLower;         //son Lower1 ex) 1  (QD) 
        nBoxType[4]=SysSet12.m_nPinQDLower2;        //son Lower2 ex) 1  (QD) 

        //if(SysSet13.m_nPinBlockType==6){//sylee180413-1                 
        int box; //son190902 32KPIN:  k2-> box로 변경    
        if(SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
        {
            for(box=1; box <= 4; box++){            //son 4: box 갯수
                if(nBoxType[box] == BOX_DD) {       //son 2: DD
                    nBoxType[box] = BOX_QD;         //son 1: QD
                }
            }
        }
        int nBoxPinSumOld;
        nBoxPinSum=0;
        nBoxPinSumOld=0;

        //-------------------------------------------------------------
        //son nBoxPin[box] 설정 : box의 맨 끝 pinNo 위치를 알아낸다.
        for(box=1; box <= 4; box++) //son190902 32KPIN:  i2-> box로 변경
        {
            //son 정상 케이스 ------
            //if( SysSet13.m_nPinBlockType!=6)  //SYLEE180413-5 
            if( SysSet13.m_nPinBlockType!= PIN_BLK_4K_DD_421)  //son190902 32KPIN 
            {
                if(nBoxType[box] == BOX_SD){//sd (3)    //son190902 32KPIN: enum 변경
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf_SS;//SYLEE150912-1 
                }
                else  if(nBoxType[box] == BOX_DD) {//dd(2) //SYLEE150912-1 //son190902 32KPIN: enum 변경
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf;  //SYLEE150912-1                      
                }
                else { //qd
                    nBoxPinSum=nBoxPinSum+nBoxPinFull;
                }
            }

            //son PIN_BLK_4K_DD_421 케이스 --------
            else{//SYLEE180413-5

                if(nBoxType[box] == BOX_SD) {//Sd(3) //SYLEE180413-5 //son190902 32KPIN: enum 변경
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf;   //SYLEE180413-5 
                }
                else {//DD  //qd
                    nBoxPinSum=nBoxPinSum+nBoxPinFull;//SYLEE180413-5 
                }
            }


            if(SysSet12.m_nPinTotMax == PIN_TOT_16K)//16k               
            {
                //son 상부 마지막 box 일 때, 
                if(box==2)
                {   
                    //sylee151102   상부  sd sd인경우  하부시작번호  2k+1이 아니라 4k+1    
                    
                    if(nBoxType[1]== BOX_QD || nBoxType[2]== BOX_QD){   //son190902 32KPIN: enum 변경    1-> BOX_QD
                        nBoxPinSum=4096*2;
                    }
                    else  if(nBoxType[1] != BOX_SD && nBoxType[2] != BOX_SD){   //son190902 32KPIN : enum 변경 3-> BOX_SD
                        nBoxPinSum=4096;
                    }
                }                   
            }
            /////////////////////////////////////////////////
            nBoxPin[box]=nBoxPinSum;    //son  ex) nBoxPin[0]= 0,   nBoxPin[1]=2048     Upper1
                                        //son                       nBoxPin[2]=4096     Lower1   
                                        //son                       nBoxPin[3]=6144     Upper2
                                        //son                       nBonPin[4]=8192     Lower2
        }

        //-------------------------------------------------
        //son PinMap[0], PinMap[1], PinMap[2]를 만들어 낸다.

        int pin;
        for (pin=0; pin<nMaxPin1; pin++)//SYLEE121003
        {

            //-------------------------
            //son PinMap[0][pin] 설정
            No=pin; 
            PinMap[0][pin]=pin;//****>net list pin no   


            //-------------------------
            //son PinMap[1][pin] 설정
            
            nStep=0;
            //if( (SysSet13.m_nPinBlockType!=7) ||  ((SysSet13.m_nPinBlockType==7)&&(No<nBoxPin[2]))  ){//sylee180322-1  
            if( (SysSet13.m_nPinBlockType!= PIN_BLK_12K_QD_DD) 
                    ||  ((SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) && (No<nBoxPin[2]))  ) //son190902 32KPIN 
            {
                //son 현재 pin에 해당하는 box 위치(nStep)을 찾는다.
                for(int i3=1; i3<=4; i3++){         //son ??? 
                    if((No+1)>nBoxPin[i3]){//sylee150916    
                        nStep=i3;                            
                    }
                    else{//sylee150916  
                        break; 
                    } //sylee150916 
                }           

                No=No-nBoxPin[nStep];   //son 박스 내부 pinNo로 변경 (Offset을 뺀다)

                if(nBoxType[nStep+1]== BOX_DD) //sylee150912-1    DD(2) //son190902 32KPIN: enum 변경
                {
                    Namo=No%64;         //son col 위치 (0~63) 
                    Mok1=Namo/32;
                    No=No*2;
                    if(Mok1==1){        //son col위치가 0~31이면 
                        No++;
                    }         
                }
                else if(nBoxType[nStep+1] == BOX_SD)    //sylee150912-1  SD(3) //son190902 32KPIN: enum 변경
                {
                    Namo=No%32; 
                    Mok1=No/32;
                    //if( SysSet13.m_nPinBlockType==6) {//SYLEE180413-1 
                    if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) { //son190902 32KPIN 
                        No=Mok1*64+Namo; 
                    }
                    else{
                        No=Mok1*128+Namo*2;  
                    }
                } 

                //son 재계산한 boxPinNo을  QD pin 기준 pinNo로 변환
                No=(nBoxPinFull*nStep)+No;  

            }

            //son PinBlockType은 PIN_BLK_12K_QD_DD 이고 하부핀 인 경우 DD로 동작해야 함.
            else //sylee180322-1 //((SysSet13.m_nPinBlockType==7)&&(No>=nBoxPin[2])) 
            {


                for(int i3=1; i3<=4; i3++){     //son ??? 
                    if(No>nBoxPin[i3]){
                        nStep=i3;
                        break;
                    }
                }

                No=No-nBoxPin[nStep];
                if(nBoxType[nStep+1] == BOX_SD){    //SD(3) //son190902 32KPIN: enum 변경
                    Namo=No%32; 
                    Mok1=No/32;  
                    No=Mok1*64+Namo;          
                }

                No=(4096*2+   2048*(nStep-3))+No;  //sylee180322-1  

            }
            ////////////////////////////////////////////////////////////////////////////////////////
/*//sylee210403
            if(SysSet12.m_n32k_16k==1){//3=SD->QD
                if(No>=(4096*2)){                    
                    No=No+(4096*2);
                }
            } 
*/

            PinMap[1][pin]=No;//****>QD pin no 


            //-------------------------
            //son PinMap[2][pin] 설정

            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            //if( SysSet13.m_nPinBlockType==6)  //sylee171109-1 //SYLEE180413-1 MOVE
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421)  //son190902 32KPIN 
            {

                //H TYPE//SYLEE161024         //SYLEE161024  //ACE350  WIRE TYPE  
                if(  SysSet12.m_nPinSwitchMode ==2){  //2=H, 1=Z  

                    Mok1=No/64;
                    Namo=No%64;//same 

                    if(SysSet12.m_nPinBoxMax==1){  //2k
                        //sylee120925                   
                        BoxMok2=Mok1/32;//same
                        Mok1=Mok1%32;

                        if(Mok1<16){
                            Mok1=Mok1*2;
                        }else if( Mok1<32){
                            Mok1=(Mok1-15)*2-1;
                        }else{
                            str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                            errMessageBox(7701, str); //son230217_2
                        }
                        No= (Mok1+(BoxMok2*32))*64+Namo;                      

                    }
                    else{//SysSet12.m_nPinBoxMax==2){  //4k

                        BoxMok2=Mok1/64;//same
                        Mok1=Mok1%64;
                        if(Mok1<32){
                            Mok1=Mok1*2;
                        }else if( Mok1<64){
                            Mok1=(Mok1-31)*2-1;
                        }else{
                            str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                            errMessageBox(7701, str); //son230217_2
                        }
                        No= (Mok1+(BoxMok2*64))*64+Namo;
                    }          

                }

            }//END OF  if( SysSet13.m_nPinBlockType==6) { //sylee171109-1 //SYLEE180413-1 MOVE
            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


            //----------------------
            //son Reaverse A 처리
            
            //son Card 순서를 reverse로 했을때 pin 번호 재배치

            int BoxN;
            if(SysSet12.m_nPinBoxMax==1){  //2k
                //sylee120925
                if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                    BoxN=No/2048;           //son boxNo
                    No=No%2048;             //son boxPinNo
                    Mok1=No/128;            //son cardNo (0~15 card), 1개 card에 128pin
                    Namo=No%128;            //son cardPinNo (0~127)
                    No=Namo+((15-Mok1)*128);//son newBoxPinNo = cardPinNo + ((15-cardNo)*128)
                    No=No+BoxN*2048;        //son newPinNo = newBoxPinNo + boxNo*2048
                }
            }
            else{
                if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                    BoxN=No/4096;           //son boxNo
                    No=No%4096;             //son boxPinNo
                    Mok1=No/128;            //son cardNo (0~31 card), 1개 card에 128pin 
                    Namo=No%128;            //son cardPinNo (0~127)
                    No=Namo+((31-Mok1)*128);//son newBoxPinNo = cardPinNo + ((31-cardNo)*128)
                    No=No+BoxN*4096;        //son newPinNo = newBoxPinNo + boxNo*4096
                }
            }

            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

            //********************************************************************************************* 
            //조이텍 전용 
            
            //----------------------
            //son Reaverse B 처리

            //2048,4096,6144,8192,10240,12288,14336
            //CONNECTOR  REVERSE = 64 PIN REVERSE

            if( SysSet12.m_nCardReverseB==2 ){  //SYLEE130809           //SYLEE170212   
                Namo=No%128;
                Mok1=No/128;
                if(Namo<64){
                    Namo=Namo+64;
                }
                else{
                    Namo=Namo-64;//191=127+64
                }
                No=Mok1*128+Namo;
            }

            //if( SysSet13.m_nPinBlockType==6) { //sylee171109-1
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
            {
                if( (No>=2048) && (No<(4096))){//sylee171109-1
                    No=No+2048;
                }
                if( No>=(2048+4096)){//sylee180413-1
                    No=No+2048;
                }
            }

            //---------------------------------
            //son Lower StartPin(ntmp1) 계산 

            int ntmp1;//sylee151006 //ACE400        
            ntmp1=nBoxPin[2];//sylee151006 //ACE400     //son LowerStartPin ex) 4096

            if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k //sylee161108-1
                if(ntmp1>2048){
                    ntmp1=2048;         //son ex) ntmp1 <= 2048로 변신
                }                        
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){ //8k//sylee161108-1
                if(ntmp1>4096){
                    ntmp1=4096;
                }                        
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k//sylee161108-1
                if(ntmp1>8192){
                    ntmp1=8192;
                }                 
            }
            else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //sylee161108-1 //son230216_1
                if(ntmp1>(8192*2)){
                    ntmp1=(8192*2); //son 16K의 절반
                }
            }

            //if( SysSet13.m_nPinBlockType==6) { //sylee161110-1  //sylee180413-1
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) { //son190902 32KPIN 
                ntmp1=4096;     //son 8K의 절반
            }

            SysSet12.m_nLowerJigStartPin = ntmp1 +1;   //son220402 하부시작핀을 결정

            
            //-------------------------------------------------------------------------
            //son 현재 pin번호가  Lower Pin 이라면 PinNo를 하부핀기준으로 다시 계산
            if(pin>=ntmp1)//sylee151006 //ACE400
            {
                //son Jig의 PinTot가 4K이면 상단에 2K, 하단에 2K-------------------------
                if(SysSet12.m_nPinTotMax == PIN_TOT_4K)//4k  ///sylee151006 //ACE400
                {
                    //son SwitchCard(HW) pin은 상단 16K, 하단 16K 준비되었지만 14K는 미사용이다. 
                    //son SwtichCard(HW) 기준의 LowerStartPin은 뒤의 14K를 건너뛰고 번호를 할당해야 한다.
                    //son Offset 14K를 더한다.
                    PinMap[2][pin]=No+( 2048 + 4096 + 8192 );//****>SWITCH CARD pin no   //sylee151006 //ACE400 
                }


                //son PinTot가 8K이면 상단에 4K, 하단에 4K.  (12K를 더해서 16K로 점프 )----------------
                else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
                    //son LowerStartPin Offset 12K를 더한다.
                    PinMap[2][pin]=No+( 4096 + 8192   );//****>SWITCH CARD pin no  //sylee151006 //ACE400
                }


                //son PinTot가 16K이면 상단에 8K, 하단에 8K.  (8K 더해서 16K로 점프) ----------------
                else if(SysSet12.m_nPinTotMax == PIN_TOT_16K)//16k
                {
                    //son LowerStartPin Offset 8K를 더한다.
                    PinMap[2][pin]=No+( 8192 );//****>SWITCH CARD pin no  //sylee151006 //ACE400

                    //son PinTot는 16K이지만 실제는 12K 변형 (상부 QD 4K *2, 하부 DD 2K *2 -> 12K) 인 경우
                    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //SYLE190403 //son190902 32KPIN 
                    {
                        //son Upper1, Upper2가 모두 DD. 상단 8K중에 뒤의 4K 추가로 미사용??
                        if((nBoxType[1]==BOX_DD)&&(nBoxType[2]==BOX_DD)){       //son190902 32KPIN: enum 변경
                            PinMap[2][pin]=PinMap[2][pin]+2048;     //son !!! 추가 4K 더해야 하지 않나?
                        }

                        //son Upper1, Upper2가 모두 SD 
                        else if((nBoxType[1]==BOX_SD)&&(nBoxType[2]==BOX_SD)){  //son190902 32KPIN: enum 변경
                            PinMap[2][pin]=PinMap[2][pin]+2048;     //son !!! 추가 6K 더해야 하지 않나?
                        }
                    }
                }

                //son 32K는 미사용공간 없으므로 No를 그대로 사용.----------------
                else if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //son230216_1
                    PinMap[2][pin]=No;//****>SWITCH CARD pin no   //sylee151006 //ACE400
                }
                else{//32k 
                    PinMap[2][pin]=No;//****>SWITCH CARD pin no
                }               
            }
            else{//sylee151006 //ACE400
                PinMap[2][pin]=No;//****>SWITCH CARD pin no
            }//sylee151006 //ACE400 

            //son  결과..
            //son PinMap[2][0] = 0
            //son PinMap[2][1] = 1
            //son PinMap[2][20] = 20
            //son PinMap[2][2047] = 2047
            //son PinMap[2][2048] = 16384       
            //son PinMap[2][2049] = 16385 
            //son PinMap[2][4095] = 18431 



            //if( SysSet13.m_nPinBlockType==2){ //sylee180409-1  SD
            if( SysSet13.m_nPinBlockType== PIN_BLK_SD) //son190902 32KPIN 
            {
                //son  64pin Card 기준의 pinNo를 128pin Card 기준의 pinNo로 변경
                No=PinMap[2][pin];
                if(No<16384){           
                    Namo=No%64;         //son 64pin Card기준의  cardPinNo
                    Mok1=No/64;         //son cardNo
                    No=Mok1*128+Namo;   //son 128pin Card 기준의 pinNo
                }
                else{
                    No=No-16384;
                    Namo=No%64;
                    Mok1=No/64; 
                    No=Mok1*128+Namo+16384;
                }
                PinMap[2][pin]=No;                  
            }


            //if( SysSet13.m_nPinBlockType==6) { //sylee171109-1 //SYLEE180413-1 MOVE
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
            {
                //son SD(3)
                if(nBoxType[1]==BOX_SD && nBoxType[2]==BOX_SD)//sylee180412-3
                {
                    /*  //SYLEE180413-5 
                        if(pin<1024){

                        }else if(pin<(1024*2)){
                        PinMap[2][pin]=PinMap[2][pin]-4160;
                        }else if(pin<(1024*3)){
                        PinMap[2][pin]=PinMap[2][pin]-6144;
                        }else if(pin<(1024*4)){
                        PinMap[2][pin]=PinMap[2][pin]-6144-4160;
                        }else if(pin<(1024*5)){//4096_
                        PinMap[2][pin]=PinMap[2][pin]-4160;
                        }else if(pin<(1024*6)){
                        PinMap[2][pin]=PinMap[2][pin]-6144;
                        }else if(pin<(1024*7)){
                        PinMap[2][pin]=PinMap[2][pin]-6144-4160;
                        }else{
                        PinMap[2][pin]=PinMap[2][pin]-6144-8192;
                        }
                        */

                    if(pin>=(1024*4)){
                        PinMap[2][pin]=PinMap[2][pin]-6144;
                    }

                }
                else //sylee180412-3
                {

                    if(pin<4096){

                    }else if(pin<(4096*2)){
                        //  PinMap[2][pin]=PinMap[2][pin];
                    }else if(pin<(4096*3)){
                        //  PinMap[2][pin]=PinMap[2][pin];
                    }else{
                        PinMap[2][pin]=PinMap[2][pin]+2048;
                    }
                }//sylee180412-3 
            }

            //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
            int hwPin = PinMap[2][pin];					//son230303
            if (hwPin >= 0 && hwPin < MAX_PIN)			//son230303
                nJigPinMap[hwPin] = pin;    //son 빠른 검색용. //son230303


        }//for( pin=0; pin< 33000; pin++)


    }//     if(nMode==0){//sylee151111add

 
//===========================================================================



 
    //SYLEE180413-1   if( SysSet13.m_nPinBlockType==6){ //sylee171109-1 
    //SYLEE180413-1     nMode=1;
    //SYLEE180413-1  }   

    //===========================================================================
    //son nMode== OnPinConvertType1 ==1 인 경우  (.Con 파일을 사용해야 한다면)
    //son CChildView1::OnDataLoad_READ_ConRead() 에서 .CON 파일을 읽어서 
    //son 생성한  nPinC1[DEF_MAX_PIN2][2] 이 존재한다. 이 정보를 바탕으로 PinMap을 구성한다.

    if(nMode==1)//sylee151111add
    {

        //son nPinC1[0][0]: .CON 파일의 총 핀수 의 range를 체크한다.
        if( (nPinC1[0][0]>0) && (nPinC1[0][0]<=32768)){  //sylee151111
            nTotal1=nPinC1[0][0];   
        }else{   
            nTotal1=0;   
        }

        if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //SYLEE121003
            nMaxPin1=4096;      
        }else if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
            nMaxPin1=4096*2;
        }else if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
            nMaxPin1=4096*4;
        }else if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //son230216_1
            nMaxPin1=4096*8;
        }else{//32k
            nMaxPin1=4096*8;
        }

        nMaxPinHalf1=nMaxPin1/2; 


        //if( SysSet13.m_nPinBlockType==7 ) {//sylee180323-1
        if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD ) { //son190902 32KPIN 
            nMaxPin1=4096*3;
        }


        memset(PinMap, -1, sizeof(PinMap));//sylee151006
        memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303

        //          int nBoxPinFull,nBoxPinHalf,nBoxPinSum;//,nStep;
        //          int nBoxPin[10],nBoxType[10];

        ::ZeroMemory(&nBoxPin,sizeof(nBoxPin));
        ::ZeroMemory(&nBoxType,sizeof(nBoxType));

        if(SysSet12.m_nPinBoxMax==1){  //2k
            nBoxPinFull=2048;
            nBoxPinHalf=1024;
            nBoxPinHalf_SS=1024/2;//SYLEE150912-1
        }else{ //4k
            nBoxPinFull=2048*2;
            nBoxPinHalf=1024*2;
            nBoxPinHalf_SS=1024;//SYLEE150912-1
        }

        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper로 변경
        //                   m_nPinQD2->m_nPinQDLower로 변경
        //                   m_nPinQD3->m_nPinQDUpper2로 변경
        //                   m_nPinQD4->m_nPinQDLower2로 변경
        nBoxType[1]=SysSet12.m_nPinQDUpper;      //son Upper1 ex) 1  (QD)
        nBoxType[2]=SysSet12.m_nPinQDUpper2;     //son Upper2 ex) 1  (QD)
        nBoxType[3]=SysSet12.m_nPinQDLower;      //son Lower1 ex) 1  (QD)
        nBoxType[4]=SysSet12.m_nPinQDLower2;     //son Lower2 ex) 1  (QD)


        int box;
        //if(SysSet13.m_nPinBlockType==6){//sylee180413-1                 
        if(SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 6:PIN_BLK_4K_DD_421
        {
            for( box=1; box<= 4; box++){
                if(nBoxType[box]== BOX_DD){     //son190902 1:BOX_QD, 2: BOX_DD
                    nBoxType[box] = BOX_QD;
                }
            }
        }

        int nBoxPinSumOld;
        nBoxPinSum=0;
        nBoxPinSumOld=0;

        //-------------------------------------------------------------
        //son nBoxPin[box] 설정 : box의 맨 끝 pinNo 위치를 알아낸다.
        
        for(box=1; box<= 4; box++)
        {
            //son 정상 케이스 ------
            //if( SysSet13.m_nPinBlockType!=6) { //SYLEE180413-5 
            if( SysSet13.m_nPinBlockType!= PIN_BLK_4K_DD_421) //son190902 6:PIN_BLK_4K_DD_421 
            {
                if(nBoxType[box]== BOX_SD){//3: sd  //son190902 3:BOX_SD
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf_SS;//SYLEE150912-1 
                }
                else  if(nBoxType[box]== BOX_DD){//2: dd//SYLEE150912-1     //son190902 2: BOX_DD
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf;  //SYLEE150912-1                      
                }
                else {//dd, //qd
                    nBoxPinSum=nBoxPinSum+nBoxPinFull;
                }
            }
            //son PIN_BLK_4K_DD_421 케이스 --------
            else {//SYLEE180413-5

                if(nBoxType[box]== BOX_SD){//3: sd //SYLEE180413-5  //son190902 3: BOX_SD
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf;   //SYLEE180413-5 
                }
                else {//DD  //qd
                    nBoxPinSum=nBoxPinSum+nBoxPinFull;//SYLEE180413-5 
                }
            }
#if 0
            //sylee170523-1 
            if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k  
                if(box==2){//sylee151111-import
                    nBoxPinSum=4096;//sylee151112==> import
                    nLoweStartPin1=4097;//sylee151112
                    for(int i5=1;i5<=nTotal1;i5++){//sylee151112     
                        a= nPinC1[i5][0];  // block no  (1~  //sylee151111add
                        b= nPinC1[i5][1];  // jig no  (1~  //sylee151111add ''
                        if(b==8193&&a==8193){
                            nBoxPinSum=4096*2;
                            nLoweStartPin1=8193;//sylee151112
                        }
                    } 
                }
            }
#endif
            /////////////////////////////////////////////////

            nBoxPin[box]=nBoxPinSum;             
        }

        memset(PinMap, -1, sizeof(PinMap));//sylee151006 
        //son nPinC1[0][0]: .CON 파일의 총 핀수 의 range를 체크한다.
        if( (nPinC1[0][0]>0) && (nPinC1[0][0]<= MAX_PIN)){  //sylee151111
            nTotal1=nPinC1[0][0];   
        }else{   
            nTotal1=0;   
        }

        //-------------------------------------------------
        //son PinMap[0], PinMap[1], PinMap[2]를 만들어 낸다.
        int swPin, conJigPin;  //son220402
        for(i=1;i<=nTotal1;i++) //sylee151111add
        {
            //son nPinC1[0] : .CON 파일 헤더
            //son           nPinC1[0][0]: .CON 파일의 총 pin 수,  nPinC1[0][1]: .CON 파일의 checkSum 
            //
            //son nPinC1[1]~nPinC1[DEF_MAX_PIN2-1]까지는 data
            //son           nPinC1[i][0]: pinBlockPinNo     nPinC1[i][1]: 사용자 pinNo       
            //son .CON 파일이 없다면 jigPin과 pinBlockPin이 동일하고  BDL 파일의 pinNo와 READ file의 pinNo가 같음. 

            //son .CON 파일의 jigpin과 (두번째항목) 그에 맞는 PinBlockPin(첫번째 항목)을 가져 온다.
            swPin    = nPinC1[i][0]-1;  // pinBlock no  (1~  //sylee151111add
            conJigPin= nPinC1[i][1]-1;  // jig no  (1~  //sylee151111add    ''  //son 사용자(Jig) pin no 


            //----------------------------------
            //son PinMap[0][pin] 설정  (conJigPin)

            PinMap[0][conJigPin]= conJigPin;   // //sylee151111addi    net list pin no


            //------------------------------------
            //son PinMap[1][pin] 설정 (Qd pin No)
            
            if(conJigPin<16384){//16k    i=0~16k  //sylee151111add
                No=swPin;   //nSwitchMaxPin  33000    ,  pin no   0~
            }else{  //sylee151111
                No=swPin-8192;  //32k->16k  //nSwitchMaxPin  33000    ,  pin no   0~
            }

            //son220402 PinMap[1][i]=No;//****>QD pin no //pin block  no    //son!!!!! pinMap[1][conJigPin] = No 여야 하지 않는지???
            PinMap[1][conJigPin] = No;  //son220402


            if(  SysSet12.m_nPinSwitchMode ==2)  //2=H, 1=Z  
            {

                Mok1=No/64;
                Namo=No%64;//same 

                if(SysSet12.m_nPinBoxMax==1){  //2k
                    //sylee120925                   
                    BoxMok2=Mok1/32;//same
                    Mok1=Mok1%32;

                    if(Mok1<16){
                        Mok1=Mok1*2;
                    }else if( Mok1<32){
                        Mok1=(Mok1-15)*2-1;
                    }else{
                        str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                        errMessageBox(7701, str); //son230217_2
                    }
                    No= (Mok1+(BoxMok2*32))*64+Namo;                      

                }
                else{//SysSet12.m_nPinBoxMax==2){  //4k

                    BoxMok2=Mok1/64;//same
                    Mok1=Mok1%64;
                    if(Mok1<32){
                        Mok1=Mok1*2;
                    }else if( Mok1<64){
                        Mok1=(Mok1-31)*2-1;
                    }else{
                        str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                        errMessageBox(7701, str); //son230217_2
                    }
                    No= (Mok1+(BoxMok2*64))*64+Namo;
                }          

            }



            int BoxN;


            if(SysSet12.m_nPinBoxMax==1){  //2k
                //sylee120925
                if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                    BoxN=No/2048;
                    No=No%2048;
                    Mok1=No/128;//0`31 card
                    Namo=No%128;//0`31 card
                    No=Namo+((15-Mok1)*128);
                    No=No+BoxN*2048;
                }
            }
            else{
                if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                    BoxN=No/4096;
                    No=No%4096;
                    Mok1=No/128;//0`31 card
                    Namo=No%128;//0`31 card
                    No=Namo+((31-Mok1)*128);
                    No=No+BoxN*4096;
                }
            }


            //********************************************************************************************* 
            //조이텍 전용 

            //2048,4096,6144,8192,10240,12288,14336

            //CONNECTOR  REVERSE = 64 PIN REVERSE
            if( SysSet12.m_nCardReverseB==2 ){  //SYLEE130809               
                Namo=No%128;
                Mok1=No/128;
                if(Namo<64){
                    Namo=Namo+64;
                }else{
                    Namo=Namo-64;//191=127+64
                }
                No=Mok1*128+Namo;
            }

            //if( SysSet13.m_nPinBlockType==6) { //sylee171109-1  //sylee180413-7
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
            {
                if( (No>=2048) && (No<(4096))){//sylee171109-1
                    No=No+2048;
                }
                if( No>=(2048+4096)){//sylee180413-1
                    No=No+2048;
                }
            }


            if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k //sylee151112
                if(No<16384){//SYLEE150924-1
                }
                else{
                    No=No-8192; //SYLEE150924-1
                }
            }


            //---------------------------------
            //son Lower StartPin(ntmp1) 계산 

            int ntmp1;//sylee151006 //ACE400

            ntmp1=nMaxPinHalf1;//sylee170615-3 

            //===========================>  
            //if( SysSet13.m_nPinBlockType==6) { //sylee161110-1  //sylee180413-1
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
            { 
                if(conJigPin<4096){
                    ntmp1=4096+2048;
                }
                else{
                    ntmp1=4096; 
                }
            }
            SysSet12.m_nLowerJigStartPin = ntmp1 +1;   //son220405 하부시작핀을 결정

            //-------------------------------------------
            //  하부시작핀 offset을 더해 hwPin을 결정
            //if( SysSet13.m_nPinBlockType!=6) { //sylee18060416-1
            if(No>=ntmp1){//sylee151006 //ACE400
                if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  ///sylee151006 //ACE400
                    No=No+( 2048 + 4096 + 8192 );//****>SWITCH CARD pin no   //sylee151006 //ACE400 
                }
                else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
                    No=No+( 4096 + 8192   );//****>SWITCH CARD pin no  //sylee151006 //ACE400
                }
                else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
                    No=No+( 8192 );//****>SWITCH CARD pin no  //sylee151006 //ACE400
                }
                else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG){//32k //son230216_1
                    No=No;//****>SWITCH CARD pin no   //sylee151006 //ACE400
                }
                else {//32k 
                    No=No;//****>SWITCH CARD pin no
                }               
            }
            else {//sylee151006 //ACE400
                No=No;//****>SWITCH CARD pin no
            }//sylee151006 //ACE400
            //}




            //===========================>          


            //if( SysSet13.m_nPinBlockType==2){ //sylee180409-1  SD
            if( SysSet13.m_nPinBlockType== PIN_BLK_SD) //son190902 32KPIN 
            {
                if(No<16384){    //son 상부
                    Namo=No%64;
                    Mok1=No/64;
                    No=Mok1*128+Namo;                    
                }
                else{    //son 하부
                    No=No-16384;
                    Namo=No%64;
                    Mok1=No/64;          
                    No=Mok1*128+Namo+16384;
                }
            }



            //===========================>              
#if 0
                //sylee18060416-1
                if( SysSet13.m_nPinBlockType==6) { //sylee171109-1 //SYLEE180413-1 MOVE

                    if(nBoxType[1]==3 && nBoxType[2]==3){//sylee180412-3 
                        if(i>=(1024*4)){
                            PinMap[2][i]=PinMap[2][i]-6144;
                        }
                    }else{//sylee180412-3
                        if(i<4096){

                        }else if(i<(4096*2)){
                            //  PinMap[2][i]=PinMap[2][i];
                        }else if(i<(4096*3)){
                            //  PinMap[2][i]=PinMap[2][i];
                        }else{
                            PinMap[2][i]=PinMap[2][i]+2048;
                        }
                    }//sylee180412-3 
                }
#endif

            PinMap[2][conJigPin]=No;//****>SWITCH CARD pin no  //SYLEE150924-1

            //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
            int hwPin = PinMap[2][conJigPin];		//son230303
            if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
                nJigPinMap[hwPin] = conJigPin;    //son 빠른 검색용. //son230303

        }//for( i=0; i< 33000; i++){


    }//     if(nMode==1){//sylee151111add


//===========================================================================
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  //son210514   200 -> FILE_PATH_SIZE
    CString strTemp;

    ::ZeroMemory(&fName, sizeof(fName));

    //son230825 BlockType이 SD 관련(s100)이면 boxType을 QD로 강제 수정기능 관련해서 QD 여부표시를 삭제.
    if (SysSet13.m_nPinBlockType==PIN_BLK_SD  || SysSet13.m_nPinBlockType==PIN_BLK_S100_1 
                || SysSet13.m_nPinBlockType==PIN_BLK_S100_2_WIRE   
				|| SysSet13.m_nPinBlockType==PIN_BLK_YAMAHA ) 
    {
        strTemp.Format("d:\\log\\PinMap_%s_%s_%d_SD_SD.csv", 
                                    STR_PIN_BLOCK(SysSet13.m_nPinBlockType), 
                                    STR_MAX_PIN(SysSet12.m_nPinTotMax),     
                                    SysSet12.m_nLowerJigStartPin);
		strcat(fName, strTemp);
    }
    else
    {
        //strcat( fName , "d:\\PinMap.txt" );
       strTemp.Format("d:\\log\\PinMap_%s_%s_%d_%s_%s.csv", //son220715 d:\\log 폴더로 이동
                                    STR_PIN_BLOCK(SysSet13.m_nPinBlockType), //son220402_2
                                    STR_MAX_PIN(SysSet12.m_nPinTotMax),     
                                    SysSet12.m_nLowerJigStartPin, 
                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                                    STR_BOXQD_TYPE(SysSet12.m_nPinQDLower));
       strcat(fName, strTemp);
    }
    //if(SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD)            //son210603
    //    strcat( fName , "d:\\PinMap_PinBlock_12K_QD_DD.txt" );  //son210603
          
    fp = fopen(fName,"wt");
    if(fp == NULL) return 0;
 
    //if(SysSet13.m_nPinBlockType==0){//SYLEE180413-1
    if(SysSet13.m_nPinBlockType== PIN_BLK_QD){ //son190902 32KPIN 
        fprintf(fp, " \n     ==>   QD PIN BLOCK  \n ");
    }

    //else  if(SysSet13.m_nPinBlockType==1 || SysSet13.m_nPinBlockType==3 
    //      || SysSet13.m_nPinBlockType==4 || SysSet13.m_nPinBlockType==5 
    //      || SysSet13.m_nPinBlockType==6){//SYLEE180413-1
    else  if(SysSet13.m_nPinBlockType== PIN_BLK_DD 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_1 
            || SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE 	//son230216_1
            || SysSet13.m_nPinBlockType== PIN_BLK_YAMAHA 
            || SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
    {
        fprintf(fp, " \n     ==>   DD PIN BLOCK  \n ");
    }

    //else  if(SysSet13.m_nPinBlockType==2){//SYLEE180413-1
    else  if(SysSet13.m_nPinBlockType== PIN_BLK_SD){ //son190902 32KPIN 
        fprintf(fp, " \n     ==>   SD PIN BLOCK  \n ");
    }

    //else  if(SysSet13.m_nPinBlockType==7){//SYLEE180322-1
    else  if(SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD){ //son190902 32KPIN 
        fprintf(fp, " \n     ==>   QD / DD PIN BLOCK   12K  \n ");//SYLEE180322-1
    }

    else {
        fprintf(fp, " \n     ==>   QD BLOCK  \n ");
    }//SYLEE180322-1

    fprintf(fp, "   *User Pin* / *Switch card Pin* \n");
    for( i=0; i<nMaxPin1; i++){//SYLEE121003
        fprintf(fp, "%10d, %10d\n",i+1,PinMap[2][i]+1); 
    }
    fclose(fp); 

    MyTrace(PRT_BASIC, "OnPinConvert1_READ(): %s PinMap Created.\n", fName);        //son220715

    return 1;

}



//son jigPin0값을 Block 1 기준의 nPin1값으로 바꾸고  m_nBlockPo1(현재 block위치)값도 획득한다. //son210514
int CModeSet12::GetBlockNo_PinNo_32K( int &nPin1,      int &nBlock1,    //sylee210402  input&output, output2
                                  int &nBlockType, int &nHwBoxType) //son210514    output3, output3
{
    //nPin1  0~ 
    int nEnd;

    nBlockType = -1;    //son210514
    nHwBoxType = -1;    //son210514

    //son Upper 4 box만 없는 경우 range check
    if(IS_UPPER_12K( SysSet12.m_nPinTotMax)) //son210514
    {
        //son Upper 12K인데 상부  4 box 일때는 -1로 처리
        if( (nPin1<(sPinBlockRange[5].nStartPin-1)) &&  (nPin1>=sPinBlockRange[3].nEndPin)){ 
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }

    }

    //son Upper 3, 4 box가 없는 경우
    if(IS_UPPER_8K( SysSet12.m_nPinTotMax)) //son210514
    {
        //son Upper 8K인데 상부 3, 4 box 일때는 -1로 처리
        if( (nPin1<(sPinBlockRange[5].nStartPin-1)) &&  (nPin1>=sPinBlockRange[2].nEndPin)){ 
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }

    }
    //son Upper 2, 3, 4 box가 없는 경우
    if(IS_UPPER_4K(SysSet12.m_nPinTotMax))  //son210514
    {
        //son Upper 4K인데 상부 2, 3, 4 box 일때는 -1로 처리
        if( (nPin1<(sPinBlockRange[5].nStartPin-1)) &&  (nPin1>=sPinBlockRange[1].nEndPin)){ 
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }
    }

    //son Lower 7, 8 box가 없는 경우
    if(IS_LOWER_8K(SysSet12.m_nPinTotMax)) //son210514
    {
        //son Lower 8K인데 하부 7, 8 box 일때 -1로 처리
        if( nPin1>(sPinBlockRange[6].nEndPin-1)){//sylee210423   
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }           
    }


    //son Lower 6, 7, 8 box가 없는 경우
    if(IS_LOWER_4K(SysSet12.m_nPinTotMax))    //son210514
    {
        //son Lower 4K인데 하부 6, 7, 8 box 일때 -1로 처리
        if( nPin1>(sPinBlockRange[5].nEndPin-1)){//sylee210423
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }           
    }


    //son 상부 box4의 endPin보다 크거나  하부 box5의 startPin보다 작은 pin은 -1 처리
    if ( !IS_UPPER_4K(SysSet12.m_nPinTotMax) && !IS_UPPER_8K(SysSet12.m_nPinTotMax)  //son210514 12K이거나 8K인 경우는 4번째 endPin체크 불가. 
                                            && !IS_UPPER_12K(SysSet12.m_nPinTotMax) ) //son210525
    {
        if (sPinBlockRange[4].nEndPin != 0) //son241007-BSW-9 Box 4 skip이면 Box 4 Endpin이 0일 수 있으므로 이 range 체크는 의미 없음
        {
            if (nPin1<(sPinBlockRange[5].nStartPin-1) && (nPin1>=sPinBlockRange[4].nEndPin)) //sylee210423
            {
                nPin1=-1;   nBlock1=-1;         
                return 1;
            }
        }    
    }

    //son 하부 box8의 endPin보다 큰 pin은 -1 처리
    if ( !IS_LOWER_4K(SysSet12.m_nPinTotMax) && !IS_LOWER_8K(SysSet12.m_nPinTotMax) ) //son210514 12K이거나 8K인 경우는 8번째 endPin체크 불가. 
    {
        if( nPin1>(sPinBlockRange[8].nEndPin-1)){//sylee210423
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }
    }


    //son 입력된 nPin값에 맞는 hwPin(nPin)값과 해당 Block을 찾아서 리턴한다.
    for(int nStep=1; nStep<=8; nStep++)
    {
        //son Upper 4 box만 없는 경우 search를 빠르게 하기 위해 바로 continue
        if (IS_UPPER_12K(SysSet12.m_nPinTotMax))       //son210525 
        {
            if (nStep==4)
                continue; 
        }

        //son Upper 3, 4 box가 없는 경우 
        //if( SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS ) //son230216_1
        if (IS_UPPER_8K(SysSet12.m_nPinTotMax))       //son210514 
        {
            if (nStep==3 || nStep==4)
                continue; 
        }

        //son Upper 2, 3, 4 box가 없는 경우 
        if (IS_UPPER_4K(SysSet12.m_nPinTotMax))       //son210514 
        {
            if (nStep==2 || nStep==3 || nStep==4)
                continue; 
        }

        //son Lower 7, 8 box가 없는 경우
        if (IS_LOWER_8K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==7 || nStep==8 )  
                continue; 
        }


        //son Lower 6, 7, 8 box가 없는 경우
        if (IS_LOWER_4K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==6 || nStep==7 || nStep==8 )  
                continue; 
        }

        if (sPinBlockRange[nStep].nBlockType == BOX_SKIP) //son241007-BSW-9
            continue;

        nEnd=sPinBlockRange[nStep].nEndPin;
        if(nPin1<nEnd)  //son 해당 block을 찾으면
        {
            nBlock1    = nStep;
            nBlockType = sPinBlockRange[nBlock1].nBlockType;    //son210514
            nHwBoxType = sPinBlockRange[nBlock1].nHwBoxType;    //son210514

            if(nStep!=1)
            {
                if(nStep!=5){
                    //son 바로 직전 block의 EndPin 값을 빼서 현재 startPin을 1로 만든다.
                    nPin1=nPin1-(sPinBlockRange[nStep-1].nEndPin);  //nPin1  1~
                }
                else{
                    //son step이 5이면 앞에 skip pinBlock이 있을 수도 있으므로 
                    //    Start번호를 빼서 첫번째 block의 pin값으로 바꾼다.
                    nPin1=nPin1-(sPinBlockRange[nStep].nStartPin-1);  //nPin1 1~
                }
            }
            break;
        }       
    }
 
    return 1;
}


  




/*
short PinMap2[Def_SwitchMaxPin][3];//sylee210402 

//son PinMap2[][0], PinMap2[][1], PinMap2[][2]값을 출력한다.
//    PinMap2[][2] :  HW
int CModeSet12::GetBlockNo_PinNo_fileLog()   //sylee210402 
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  
    ::ZeroMemory(&fName, sizeof(fName)); 
    strcat( fName , "d:\\PinMap_PinBlockRange1_PinList1.ini" );
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        errMessageBox(2005, "d:\\PinMap_PinBlockRange1_PinList1.ini    file Data Saving Error ! ,    file close!");       //son230217_2
        return 0;
    } 
    for(int i=1; i<=Def_SwitchMaxPin; i++){ 
        //if(i==1 || i==4096 || i==4097 || i==(4096*2) || i==(4096*2+1) || i==(4096*3) || i==(4096*3+1) || i==(4096*4) || i==(4096*4+1))
        { 
            fprintf(fp, " No=%7d,        HW=%7d ,    B=%7d ,     No=%7d,\n ", i,  PinMap2[i][2], PinMap2[i][0],PinMap2[i][1]);   
        }
    }
    fclose(fp); 

    return 1;

}
*/




//son190902 32KPIN: 신규생성 함수 OnPinConvert1_32K()
//
//son nPinConvertType:  .CON 파일이 nPinC1[][]에 로드되었으면  1, 아니면 0 
int CModeSet12::OnPinConvert1_32K(int nPinConvertType)   //son190902 32KPIN 
{

    int nMaxPin1;   //son PinConvert() for 문을 위한 max값
 
    memset(PinMap, -1, sizeof(PinMap)); //son  초기화
    memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303

    //son sPinBlockRange[10]를  생성하고 "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
    MakePinBlockRangeUserNo();//sylee210402   여기서 하부시작핀도 결정
    

    //--------------------------------------------------------------------------
    //son 하부시작핀 보정 :  하부시작핀 값이 변경됐을 때의 이후 조치를 취한다. 
    //                       QDType, MaxPin과 현재  LowerStartPin이 맞지 않는 경우에 보정한다. 
    //                       반드시 QD와 MaxPin 초기화가 완료된 다음에 호출해야 한다.
    //
    //    OnPinConvert1_32K()는 전역함수처럼 호출되므로 하부시작핀 콤보박스는 여기서 변경할 수 없고
    //    initCombo_LowerStartPin()로 나중에 Pin 설정 UI 구동시에 변경한다.
                    //son210525 LoadSaveSet12(_LOAD)에서 OnPinConvert_32K()로 이동.
                    //          MakePinBlockRangeUserNo()에서 PinBlockRange설정한 이후에 호출.
    repair_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);  //son210115 추가 //son241007-BSW-9 


    if (!IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax))  //son210826
        return -1;

    //son Upper 12K Trans, Lower 4K Trans
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS)   //son210826
    {
        //son210525 U12K_L4K 타입은 하부시작핀은 사용하지 않고 QD/DD/SD Block Set을 default 설정한다.
        SysSet12.m_nBlockSet1 = 1;      //son210525 강제설정

        //son Pin설정화면 Init 하기 전에 OnPinConvert1_32K() 호출 되면 아래코드에서 죽음. 삭제. //son210525
        //m_check4.SetCheck(1);           //son210525
        //GetDlgItem(IDC_COMBO_LOWER_START_PIN)->EnableWindow(FALSE);             //son210525
        //UpdateData(FALSE);        // UI에 값을 반영한다.  //son210525
    }

    //son nMaxPin1 설정
    // 20K든, 24K 든 간에 1~ 32768까지 모든 swPIn에 대한 hwPin 값이 뭔지는 확인해야 한다. 
    // 의미 없는 영역이라도 -1을 채워줘야 하므로  nMaxPin1 값은 32768로 고정한다. 
    nMaxPin1 = MAX_BOX_PIN * MAX_BOX;   // 4096 *8 : 32K //son-32KPIN-20200114 : 
    

    //======================================
    //son .CON 파일이 의미 없다면
    int jigPin0, nBlockType, nHwBoxType;
    int nPin1;//sylee210402
    int hwPin;  //son230303
    if(nPinConvertType==0)  //son .CON 없다면
    {
        for (jigPin0 = 0; jigPin0 < nMaxPin1; jigPin0++)//SYLEE121003
        {
            //--------------------- 
            //son PinMap[0][jigPin0] 설정
            PinMap[0][jigPin0]=jigPin0; // ****> net list pin no 


            //---------------------
            //son PinMap[1][jigPin0] 설정
    
            if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
            {
                if (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG && (jigPin0 +1) > SysSet12.m_nEndPin) //son241007-BSW-9
                    break;

                nPin1=jigPin0;  //sylee210402
                m_nBlockPo1=0;  //sylee210402 
                nBlockType = BOX_QD;        //son210514 초기화
                nHwBoxType = TYPE_AC;       //son210514 초기화
                //if(nPin1==16385){
                //  nPin1=16385;
                //}

                //son jigPin0값을 Block 1 기준의 nPin1값으로 바꾸고  m_nBlockPo1(현재 block위치)값도 획득한다.
                GetBlockNo_PinNo_32K(nPin1, m_nBlockPo1, nBlockType, nHwBoxType);  //sylee210402 //son210514

                if(nPin1>=0){
                    //son Block1기준의 nPin값으로 hwPin을 구한 다음 
                    //    MAX_BOX_PIN * (m_nBlockPo1-1) 을 더해서 실제 hwPin값을 구한다.
                    PinMap[1][jigPin0] = MAX_BOX_PIN * (m_nBlockPo1-1) + make32kHwPin(nPin1);
                }
                else{
                    PinMap[1][jigPin0] = -1;
                }
                //PinMap2[jigPin0][2]=PinMap[1][jigPin0]; //sylee210402 
                PinMap[3][jigPin0] = m_nBlockPo1;       //son210514  PinBlockNo 1 ~ 8
                PinMap[4][jigPin0] = nBlockType;        //son210514  1:BOX_QD,  2:BOX_DD, 1:BOX_SD
                PinMap[5][jigPin0] = nHwBoxType;        //son210514  0:TYPE_AC, 1:TYPE_BD
                PinMap[6][jigPin0] = nPin1;             //son210514  QdJigPin 
            }
            else//sylee210402 
            { 
                 PinMap[1][jigPin0] = make32kHwPin(jigPin0);
            }//sylee210402 
 
            //---------------------
            //son PinMap[2][jigPin0] 설정
            
            // Todo: 추가 설정 에 맞춰서 pin 값을 다시 재배치해야 함.
            //      PinSwitchMode(Wire type), ReverseA, ReverseB, 
            //      PIN_BLK_SD이면 128 카드로 구조 바꾸기 등의 추가 기능.
            
            hwPin = PinMap[2][jigPin0] = PinMap[1][jigPin0];	//son230303

            //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
            if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
				//son 빠른 검색용. make32kSwPin(hwPin)은 pinMap 데이터 검증용
                nJigPinMap[hwPin] = jigPin0;    	//son230303
        }
    }

//  if( SysSet12.m_nBlockSet1 == 1 ){ //sylee210402 
//      GetBlockNo_PinNo_fileLog();   //sylee210402 
//  }

    //====================================
    //son Con 파일을 사용해야 한다면
 
    //son CChildView1::OnDataLoad_READ_ConRead() 에서 .CON 파일을 읽어서 
    //son 생성한  nPinC1[DEF_MAX_PIN2][2] 이 존재한다. 이 정보를 바탕으로 PinMap을 구성한다.
    int nConTotal, jigPin, swPin, conJigPin, conIdx; 
    if(nPinConvertType==1)
    {
        //son nPinC1[0][0]: .CON 파일의 총 핀수 의 range를 체크한다.
        if (nPinC1[0][0] > 0 && nPinC1[0][0] <= MAX_PIN)  //sylee151111
            nConTotal = nPinC1[0][0];   
        else   
            nConTotal = 0;      

        for (conIdx=1; conIdx <= nConTotal; conIdx++)
        {
 
            //son nPinC1[0] : .CON 파일 헤더
            //son           nPinC1[0][0]: .CON 파일의 총 pin 수,  nPinC1[0][1]: .CON 파일의 checkSum 
            //
            //son nPinC1[1]~nPinC1[DEF_MAX_PIN2-1]까지는 data
            //son           nPinC1[i][0]: pinBlockPinNo     nPinC1[i][1]: 사용자 pinNo       
            //  
            //son .CON 파일이 없다면 jigPin과 pinBlockPin이 동일하고  BDL 파일의 pinNo와 READ file의 pinNo가 같음. 
            
            
            //son .CON 파일의 jigpin과 (두번째항목) 그에 맞는 PinBlockPin(첫번째 항목)을 가져 온다.
            swPin     = nPinC1[conIdx][0]-1;  // PinBlock PinNo  (1~ )                 //son make32kHwPin()에서 사용할 swPin
            conJigPin = nPinC1[conIdx][1]-1;  // con Jig PinNo  (1~ ) : 사용자(Jig) pin no //son BDL에서 사용되는 con JigPin   

            //----------------------------
            //son PinMap[0][conJigPin] 설정 : jigPin 
            PinMap[0][conJigPin] = conJigPin;   // ****> net list pin no    //son210416 .CON 이 있을때 PinMap 출력 기능 보완

            //----------------------------
            //son PinMap[1][conJigPin] 설정 
            
            //son pin값에 맞는 HwPin(SwitchCard pin) 값을 설정.
            //son conJigPin-> pinBlockPin1_임시(swPin)-> HwPin(pinBlockPin2:변형) 값을 설정.
            PinMap[1][conJigPin] = make32kHwPin(swPin); //****> QD pin no (QD, DD, SD 까지만 반영한 pinNo)


            //----------------------------
            //son PinMap[2][jigPin] 설정
            
            // Todo: 추가 설정 에 맞춰서 pin 값을 다시 재배치해야 함.
            //      PinSwitchMode(Wire type), ReverseA, ReverseB, 
            //      PIN_BLK_SD이면 128 카드로 구조 바꾸기 등의 추가 기능.
            hwPin = PinMap[2][conJigPin] = PinMap[1][conJigPin];    //son210416 .CON 이 있을때 PinMap 출력 기능 보완 //son230303

            //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
            if (hwPin >= 0 && hwPin < MAX_PIN)  //son230303
                nJigPinMap[hwPin] = conJigPin;	//son230303
        }

    }

    //============================================
    //son PinMap File 생성   (jigPin -> hwPin)
    //============================================
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  //son210514   200 -> FILE_PATH_SIZE
    ::ZeroMemory(&fName, sizeof(fName));

    CString strTemp;

    if( SysSet12.m_nBlockSet1 == 1 ){ //sylee210402 
           strTemp.Format("d:\\log\\PinMap_%s_%s_%d_%d__%s_%s_%s_%s__%s_%s_%s_%s.csv",     //son220715 d:\\log 폴더로 이동
                                                STR_PIN_BLOCK(SysSet13.m_nPinBlockType), //son220402_2 
                                                STR_MAX_PIN(SysSet12.m_nPinTotMax),
                                                SysSet12.m_nLowerJigStartPin, 
                                                SysSet12.m_nEndPin,             //son241126-BSW-222
                                                STR_BOXQD_TYPE(sPinBlockRange[1].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[2].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[3].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[4].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[5].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[6].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[7].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[8].nBlockType)); 
    }
    else{
           strTemp.Format("d:\\log\\PinMap_%s_%s_%d_%d_%s_%s.csv",     //son220715 d:\\log 폴더로 이동  //son240516
                                                STR_PIN_BLOCK(SysSet13.m_nPinBlockType), //son220402_2 
                                                STR_MAX_PIN(SysSet12.m_nPinTotMax),
                                                SysSet12.m_nLowerJigStartPin, 
                                                SysSet12.m_nEndPin,             //son240516 
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower));
    }

    strcat(fName, strTemp);
          
    fp = fopen(fName,"wt");
    if(fp == NULL) 
    {   //son210416 .CON 이 있을때 PinMap 출력 기능 보완
        strTemp.Format("Can't Write %s File.\n", fName );    
        errMessageBox(1113, strTemp); //son230217_2
        return 0;
    }
 
    //------------------
    //son 헤더출력
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS //son190902 32KPIN //son230216_1
         || SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN    //son-8K4097-20200605: 8K 구형 하부시작핀 사용시에도 적용.  
         || SysSet12.m_nPinTotMax == PIN_TOT_12K             //son210202: 12K 구형 추가
         || SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY //son211020 8K구형 상부만 존재하는 케이스 (f550j 나노시스) 
         || SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K //son230221
         || SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG)     //son230221
    {
        //son210416 .CON 이 있을때 PinMap 출력 기능 보완
        if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
            fprintf(fp, "  PinBlock=%s, MaxPin=, %s, .CON=%s, LowerStartPin= %d, EndPin= %d, QDUpper=%s_%s_%s_%s, QDLower=%s_%s_%s_%s  \n\n",
                                                STR_PIN_BLOCK(SysSet13.m_nPinBlockType),    //son230224
                                                STR_MAX_PIN(SysSet12.m_nPinTotMax), (nPinConvertType==1)? "ON":"OFF",
                                                SysSet12.m_nLowerJigStartPin, 
                                                SysSet12.m_nEndPin,     //son241007-BSW-9  
                                                STR_BOXQD_TYPE(sPinBlockRange[1].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[2].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[3].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[4].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[5].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[6].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[7].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[8].nBlockType)); 
        else
            fprintf(fp, "  PinBlock=%s, MaxPin=, %s, .CON=%s, LowerStartPin= %d, EndPin= %d, QDUpper=%s, QDLower=%s  \n\n",  //son240516
                                                STR_PIN_BLOCK(SysSet13.m_nPinBlockType),    //son230224
                                                STR_MAX_PIN(SysSet12.m_nPinTotMax), (nPinConvertType==1)? "ON":"OFF",
                                                SysSet12.m_nLowerJigStartPin, 
                                                SysSet12.m_nEndPin,     //son240516
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower));

    }
    else
    {
        fprintf(fp, "  PinBlock=%s, (Mismatched with 32K), MaxPin=, %s \n\n", 		//son230221
                        STR_PIN_BLOCK(SysSet13.m_nPinBlockType), STR_MAX_PIN(SysSet12.m_nPinTotMax));//son230221

    }

    //------------------
    //son Data 출력
    //son 32K일때에는 HwPin <=> SwPin 변환이 일어나므로 변환 결과가 정상인지를 확인한다.
    //son 32K Trans일때에는 HwPin <=> SwPin 변환이 일어나므로 변환 결과가 정상인지를 확인한다.
    //    jigPin0 와 jigPin0->hwPin0->jigPIn0 한 결과가 같으면 OK
    int pin, nQdSwPin, nPinBlock, nDdSdSwPin, nDdSdSwPin2;  
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son190902 32KPIN //son230216_1
    {
        //son .CON이 없다면
        if(nPinConvertType==0)  //son210416 
        {
            if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
            {
                int nQdGap, nQdJigPin0_1, nQdJigPin0_2 ;
                //fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigpin0, QdJigPin0, QdJigPin0_1,  PinBlock, QDType \n");	//son230221
                fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigpin0,  Check1, nJigPinMap, Check2 ,\n");	//son230221
                for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
                {
                    //if ( (pin % 512 != 0) && ((pin+1) % 1024 != 0)) //son 출력 갯수를 줄이기
                    //    continue;

                    nPinBlock    = PinMap[3][pin];    //son210514
                    nQdGap       = (nPinBlock-1)*4096 - (sPinBlockRange[nPinBlock].nStartPin-1);     //son210514
                    nQdJigPin0_1 = pin + nQdGap;      //son210514

                    nQdJigPin0_2 = make32kSwPin(PinMap[2][pin]);  //son hwPin->jigPin 결과
                    //fprintf(fp, "%10d, %10d, , %d, %d, %d, , %d, %s, %d, %d\n",  
                    //      //userPin  switchPin     jigPin0   Pin0->hwPin0      hwPin0->jigPIn0  PinBlock        QDType,                         QdJigPin0       QdJigPin0_1  
                    //      pin+1, PinMap[2][pin]+1,  pin,     PinMap[2][pin],   nQdJigPin0_2,    PinMap[3][pin], STR_BOXQD_TYPE(PinMap[4][pin]), PinMap[6][pin], nQdJigPin0_1 ); 
                    
                    //son jigPin0와 QdJigPin0_2(hwPin0->jigPIn0 한 결과)가 같아야 한다.
                    //    nJigPinMap[hwPin]도 jigPin0와 같아야 한다. 
                    hwPin = PinMap[2][pin]; //son230303
                    fprintf(fp, "%10d, %10d, , %d, %d, %d,  %s, %d, %s \n",         //son210514
                           //userPin  switchPin     jigPin0   Pin0->hwPin0      hwPin0->jigPIn0     Check1   nJigPinMap  Check2 
                            pin+1, PinMap[2][pin]+1,  pin,     PinMap[2][pin],   nQdJigPin0_2 , 	//son230303
                            (nQdJigPin0_2 != pin) ? "Error": "  ", nJigPinMap[hwPin], (pin != nJigPinMap[hwPin]) ? "Error" : "  "    );  //son230303
                }
            }
            else 
            {
                //fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigPin0, ,getQdSwPin(jigPin),  getDdSd(nQdSwPin), , getDdSd(hw:0~end) \n");//son230303
                fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigPin0,  Check1, nJigPinMap, Check2 ,\n");//son230303
                for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
                {
                    jigPin = make32kSwPin(PinMap[2][pin]);  //son hwPin->jigPin 결과

                    nQdSwPin    = getQdSwPin(jigPin);       //son jigPin->hwPin의 과정 (range 체크없음)
                    nDdSdSwPin  = getDdSdSwPin(nQdSwPin);   //son hwPin->jigPin의 과정 (range 체크없음)

                    nDdSdSwPin2 = getDdSdSwPin(pin);     //son 이건 그냥 0~32767순서대로 hwPin->jigPin을 만들었을 때의 결과를 보기 위함.

                    //fprintf(fp, "%10d, %10d, , %d, %d, %d, , %d, %d,  ,%d\n",   
                          //userPin  switchPin       jigPin0    Pin0->hwPin0   hwPin0->jigPIn0 jigPin0->hwPin0 hwPin0->jigPin0  getDdSd(hwPin0~end) 
                    //      pin+1, PinMap[2][pin]+1, pin,    PinMap[2][pin],   jigPin,         nQdSwPin,       nDdSdSwPin,      nDdSdSwPin2); 
                    
                    //son jigPin0와  nDdSdSwPin(hwPin0->jigPin0 한 결과)가 같아야 한다. (검증용)
                    //    nJigPinMap[hwPin]도 jigPin0와 같아야 한다. 
                    hwPin = PinMap[2][pin]; //son230303
                    fprintf(fp, "%10d, %10d, , %d, %d, %d, %s, %d, %s\n", //son210514 //son230303
                          //userPin  switchPin       jigPin0    Pin0->hwPin0   hwPin0->jigPIn0    Check1   nJigPinMap  Check2
                            pin+1, hwPin+1, pin,    hwPin,    jigPin ,        
                            (jigPin != pin) ? "Error": "  ", nJigPinMap[hwPin], (jigPin != nJigPinMap[hwPin]) ? "Error" : "  "    );    //son230303
                }
            }

        }

    }
//son230316 begin:
    //son-8K4097-20200605: 8K 구형 하부시작핀 사용시에도 적용.  
    else if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
             || SysSet12.m_nPinTotMax == PIN_TOT_12K               //son210202: 12K 구형 추가
             || SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY   //son211020 8K구형 상부만 존재하는 케이스 (f550j 나노시스) 
             || SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K //son230221
             || SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG)     //son230221
    {
        //son .CON이 없다면
        if(nPinConvertType==0)  //son210416 
        {
            //fprintf(fp, "   UserPin, SwitchCardPin, , Pin0, Pin0->hwPin0, hwPin0->jigPin0, getQdSwPin(jigPin),  getDdSd(nQdSwPin),  ,getDdSd(hw:0~end) \n");
            fprintf(fp, "   UserPin, SwitchCardPin, , Pin0, Pin0->hwPin0, hwPin0->jigPin0, Check1, nJigPinMap, Check2, getQdSwPin(jigPin),  getDdSd(nQdSwPin),  ,getDdSd(hw:0~end) \n");
            for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
            {
                if (SysSet12.m_nBlockSet1 == 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG && (pin +1) > SysSet12.m_nEndPin) //son241007-BSW-9
                   break;

                hwPin = PinMap[2][pin];   //son230303
                jigPin = make32kSwPin(PinMap[2][pin]);          //son hwPin->jigPin 결과

                if (SysSet12.m_nBlockSet1 == 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG) //son241007-BSW-9
                    nQdSwPin = getQdSwPin_BlockSet1(jigPin);     //son241007-BSW-9
                else
                    nQdSwPin = getQdSwPin_Fixed(jigPin);         //son jigPin->hwPin의 과정 (range 체크없음)

                nDdSdSwPin  = getDdSdSwPin_Fixed(nQdSwPin);     //son hwPin->jigPin의 과정 (range 체크없음)
                nDdSdSwPin2 = getDdSdSwPin_Fixed(pin);       //son 이건 그냥 0~32767순서대로 hwPin->jigPin을 만들었을 때의 결과를 보기 위함.
                //fprintf(fp, "%10d, %10d, , %d, %d, %d, %d, %d,   ,%d\n",  
                //      //userPin  switchPin       Pin0  Pin0->hwPin0   hwPin0->jigPIn0   jigPin0->hwPin0 hwPin0->jigPin0  getDdSd(hwPin0~end) 
                //        pin+1, PinMap[2][pin]+1, pin,  PinMap[2][pin], jigPin,          nQdSwPin,       nDdSdSwPin,      nDdSdSwPin2); 

                fprintf(fp, "%10d, %10d, , %d, %d, %d, %s, %d, %s, %d, %d,   ,%d\n", 	//son230303 
                        //userPin  switchPin       Pin0  Pin0->hwPin0   hwPin0->jigPIn0  Check1   nJigPinMap  Check2 jigPin0->hwPin0 hwPin0->jigPin0  getDdSd(hwPin0~end) 
                        pin+1, hwPin+1, pin,  hwPin, jigPin,          
                        (jigPin != pin) ? "Error": "  ", nJigPinMap[hwPin], (jigPin != nJigPinMap[hwPin]) ? "Error" : "  ",
                        nQdSwPin,       nDdSdSwPin,      nDdSdSwPin2); 

            }
        }

    }
//son230316 end
    //son .CON이 있다면
    if(nPinConvertType==1)  //son210416 .CON 이 있을때 PinMap 출력 기능 보완    //son230314
    {
        //fprintf(fp, "   UserPin, SwitchPin, , conJigPin0, swPin0, swPin0->hwPin0, hwPin0->swPin0 \n");
        fprintf(fp, "   UserPin, SwitchPin, , conJigPin0, swPin0, swPin0->hwPin0, hwPin0->swPin0, Check1, nJigPinMap, Check2 \n"); //son230303
        for (conJigPin=0; conJigPin < nMaxPin1; conJigPin++)//SYLEE121003
        {
            
            //son conJigPin에 맞는 swPin을 찾는다. 
            swPin = -1; 
            for (conIdx=1; conIdx <= nConTotal; conIdx++) {
                if ((nPinC1[conIdx][1]-1) == conJigPin) {
                    swPin = nPinC1[conIdx][0]-1;
                    break;
                }
            }
            if (swPin < 0)  
                continue;

            //son Check1: swPin과  orgJigPin(hwPin0->swPIn0)이 같아야 한다.
            //son Check2: conJigPin과  nJigPinMap[hwPin]이 같아야 한다.
            int orgJigPin = make32kSwPin(PinMap[2][conJigPin]);  //son hwPin->orgJigPin 결과


            //fprintf(fp, "%10d, %10d, , %d, %d, %d, %d \n", 
            //        //userPin    switchPin            conJigPin0   swPin     swPin0->hwPin0         hwPin0->swPIn0 
            //        conJigPin+1, PinMap[2][conJigPin]+1, conJigPin,   swPin,    PinMap[2][conJigPin],   orgJigPin      ); 

            hwPin = PinMap[2][conJigPin];	//son230303
            fprintf(fp, "%10d, %10d, , %d, %d, %d, %d, %s, %d, %s, \n",  //son230303
                    //userPin    switchPin            conJigPin0   swPin     swPin0->hwPin0    hwPin0->swPIn0    Check1   nJigPinMap  Check2 
                    conJigPin+1, PinMap[2][conJigPin]+1, conJigPin, swPin,  hwPin,             orgJigPin,
                    (swPin != orgJigPin) ? "Error": "  ", nJigPinMap[hwPin], (conJigPin != nJigPinMap[hwPin]) ? "Error" : "  "    );
        }
    }

    fclose(fp); 


    MyTrace(PRT_BASIC, "OnPinConvert1_32K(): %s PinMap Created.\n", fName);        //son220715


    //#################################
/*

    //============================================
    //son PinMap File 생성   (jigPin -> hwPin)
    //============================================
//  FILE *fp; 
//  char  fName[FILE_PATH_SIZE];  //son210514
    ::ZeroMemory(&fName, sizeof(fName));

//    CString strTemp;

    if( SysSet12.m_nBlockSet1 == 1 ){ //sylee210402 
           strTemp.Format("d:\\PinMap2_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.csv", STR_MAX_PIN(SysSet12.m_nPinTotMax),
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper2),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper3),STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper4),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower),STR_BOXQD_TYPE(SysSet12.m_nPinQDLower2),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower3),STR_BOXQD_TYPE(SysSet12.m_nPinQDLower4));
    }else{
           strTemp.Format("d:\\PinMap2_%s_%d_%s_%s.csv", STR_MAX_PIN(SysSet12.m_nPinTotMax),
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower));
    }

    strcat(fName, strTemp);
          
    fp = fopen(fName,"wt");
    if(fp == NULL) 
        return 0;
 
    if ((SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son190902 32KPIN //son230216_1
         || (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN)    //son-8K4097-20200605: 8K 구형 하부시작핀 사용시에도 적용.  
         || (SysSet12.m_nPinTotMax == PIN_TOT_12K))             //son210202: 12K 구형 추가
    {
        fprintf(fp, " \n     ==>   32K PIN BLOCK (or 8KNew), MaxPin= %s, LowerStartPin= %d, QDUpper=%s, QDLower=%s  \n\n",
                                                STR_MAX_PIN(SysSet12.m_nPinTotMax),
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower));
    }
    else
    {
        fprintf(fp, " \n     ==>   %s PIN BLOCK (Mismatched with 32K) \n\n", STR_PIN_BLOCK(SysSet13.m_nPinBlockType));

    }

 
//    int nQdSwPin, nDdSdSwPin, nDdSdSwPin2;
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son190902 32KPIN //son230216_1
    {
        fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigPin0, getQdSwPin(jigPin),  getDdSd(nQdSwPin), getDdSd(hw:0~end) \n");
        for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
        {
            jigPin = make32kSwPin(PinMap[2][pin]);  //son hwPin->jigPin 결과
            nQdSwPin    = getQdSwPin(jigPin);       //son jigPin->hwPin의 과정 (range 체크없음)
            nDdSdSwPin  = getDdSdSwPin(nQdSwPin);   //son hwPin->jigPin의 과정 (range 체크없음)
            nDdSdSwPin2 = getDdSdSwPin(pin);     
           // fprintf(fp, "%10d, %10d, , %d, %d, %d, %d, %d, %d\n",pin+1, PinMap[2][pin]+1, pin, PinMap[2][pin],jigPin, nQdSwPin,nDdSdSwPin,   nDdSdSwPin2); 
        
            if(pin==0 || pin==4096-1 
                    || pin==4096 || pin==(4096*2-1) 
                    || pin==(4096*2) || pin==(4096*3-1) 
                    || pin==(4096*3) || pin==(4096*4-1) 
                    || pin==(4096*4)|| pin==(4096*5-1) 
                    || pin==(4096*5) || pin==(4096*6-1) 
                    || pin==(4096*6)|| pin==(4096*4)
                    || pin==(4096*7-1) || pin==(4096*7) 
                    || pin==(4096*8-1) || pin==(4096*8))
            { 
                fprintf(fp, "%d     , %d    \n",pin+1, PinMap[2][pin]+1); //sylee210407
            }
            if( pin==2048-1 || pin==2048 
                    || pin==(2048*3-1) || pin==(2048*3) 
                    || pin==(2048*5-1) || pin==(2048*5) 
                    || pin==(2048*7-1) || pin==(2048*7)
                    || pin==(2048*9-1) || pin==(2048*9) 
                    || pin==(2048*11-1) || pin==(2048*11)
                    || pin==(2048*13-1) || pin==(2048*13) 
                    || pin==(2048*15-1) || pin==(2048*15) )
            {
                fprintf(fp, "*>%d   , %d    \n",pin+1, PinMap[2][pin]+1); //sylee210407
            }

            if( pin==2048-1-1024 || pin==2048-1024 || pin==(2048*3-1-1024) || pin==(2048*3-1024) || pin==(2048*5-1-1024) || pin==(2048*5-1024) || pin==(2048*7-1-1024) || pin==(2048*7-1024)|| pin==(2048*9-1-1024) || pin==(2048*9-1024) || pin==(2048*11-1-1024) || pin==(2048*11-1024)|| pin==(2048*13-1-1024) || pin==(2048*13-1024) || pin==(2048*15-1-1024) || pin==(2048*15-1024) ){
                fprintf(fp, "**>%d   , %d   \n",pin+1, PinMap[2][pin]+1); //sylee210407
            }
            if(  pin==(2048*2-1-1024) || pin==(2048*2-1024) || pin==(2048*4-1-1024) || pin==(2048*4-1024) || pin==(2048*6-1-1024) || pin==(2048*6-1024)|| pin==(2048*8-1-1024) || pin==(2048*8-1024) || pin==(2048*10-1-1024) || pin==(2048*10-1024)|| pin==(2048*12-1-1024) || pin==(2048*12-1024) || pin==(2048*14-1-1024) || pin==(2048*14-1024)|| pin==(2048*16-1-1024) || pin==(2048*16-1024) ){
                fprintf(fp, "**>%d   , %d   \n",pin+1, PinMap[2][pin]+1); //sylee210407
            }

        }
    }

    //son-8K4097-20200605: 8K 구형 하부시작핀 사용시에도 적용.
    else if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
             || SysSet12.m_nPinTotMax == PIN_TOT_12K)               //son210202: 12K 구형 추가
    {
        fprintf(fp, "   UserPin, SwitchCardPin, , Pin0, Pin0->hwPin0, hwPin0->jigPin0, getQdSwPin(jigPin),  getDdSd(nQdSwPin), getDdSd(hw:0~end) \n");
        for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
        {
            jigPin = make32kSwPin(PinMap[2][pin]);          //son hwPin->jigPin 결과
            nQdSwPin    = getQdSwPin_Fixed(jigPin);         //son jigPin->hwPin의 과정 (range 체크없음)
            nDdSdSwPin  = getDdSdSwPin_Fixed(nQdSwPin);     //son hwPin->jigPin의 과정 (range 체크없음)
            nDdSdSwPin2 = getDdSdSwPin_Fixed(pin);       //son 이건 그냥 0~32767순서대로 hwPin->jigPin을 만들었을 때의 결과를 보기 위함.
            fprintf(fp, "%10d, %10d, , %d, %d, %d, %d, %d, %d\n",  pin+1, PinMap[2][pin]+1, pin,  PinMap[2][pin], jigPin,  nQdSwPin,  nDdSdSwPin,  nDdSdSwPin2); 
        }
    }

    fclose(fp);
*/


    return 1;

 
}

//son241007-BSW-9
// 32K Org  BlockSet1(QD/DD/SD 혼용기능) 사용시의 HwPin 계산 함수
//
// nSwPin    : pinBlock 1개 기준의  swPin
// nBlockPo1 : 1~8  pinBlockNo
// nBlockType : PIN_BOXTYPE (1:QD, 2:DD, 3:SD, 4:SKIP) 
int CModeSet12::make32kOrgBlockSet_HwPin(UINT nSwPin) 
{
    //---------------------
    // Range Check

    if (SysSet12.m_nPinTotMax != PIN_TOT_32K_ORG)
        return -1;

    if (SysSet12.m_nBlockSet1 != 1)
        return -1;

    //son block 1 기준의 pin이므로 4096을 넘을 수 없다.
    if (nSwPin < 0 || nSwPin >= MAX_BOX_PIN)    
        return -1;
    
    
    // BlockSet1 사용하는 경우이므로  getQdSwPin(nSwPin) 호출. 
    UINT nQdSwPin = getQdSwPin_BlockSet1(nSwPin);   


    //son 32K Org는 정방향 핀블록이므로 32K Convert 없이 return한다.
    return nQdSwPin; 

}

//---------------------------------------------------------------------------
//son  nSwPin 값에 맞는 HwPin(SwitchCard pin)을 리턴한다. 
//    기본은 16K QD 인데 여러가지로 변형이 된 케이스에서 사용. 
//    상부 QD 2box, 하부 QD 2box,   방향은 Original로 Trans 아님.
//--------------------------------------------------------------------------
// parameter  : nSwPin      Input 정보, 0으로 시작하는 SwPin(JigPin)
// return val : nHwPin      nSwPin에 맞는 Hw SwitchCard PinBlock Pin

int CModeSet12::make16kQdBase_HwPin(UINT nSwPin) //son211020 make8kNew_Hwpin() -> make16kQdBase_HwPin()
{
    
    //---------------------
    // nSwPin Range Check
    
    switch (SysSet12.m_nPinTotMax) 
    {
        case PIN_TOT_8K_LOW_STPIN:      //son 상부 QD 1box, 하부 QD 1box
        case PIN_TOT_U8K_UPPER_ONLY:    //son211020  상부 QD 2box only 
            // 8K range check  (jigPin 기준 8K 이상은 가질 수 없다.)
            if (nSwPin < 0 || nSwPin >= MAX_BOX_PIN * 2)    // 8K
                return -1;
            break;

        case PIN_TOT_12K:               //son 상부 QD 2box,  하부 QD 1box 
            // 12K range check  (jigPin 기준 12K 이상은 가질 수 없다.)
            if (nSwPin < 0 || nSwPin >= MAX_BOX_PIN * 3)    // 12K
                return -1;
            break;

//son230221 begin:
        case PIN_TOT_32K_U8K_L24K:                                      
            if (nSwPin < 0 || nSwPin >= MAX_BOX_PIN * 8)    // 32K      
                return -1;
            break;

        case PIN_TOT_32K_ORG:      //son230316 
            if (nSwPin < 0 || nSwPin >= SysSet12.m_nEndPin)    // 32K    //son240516
                return -1;
            break;
//son230221 end

        default:
            return -1;
            break;
    }

     

    //--------------------
    // nQdSwPin Range Check
    
    // 하부시작핀이 QD 고정으로  DD,SD 시에 변하지 않는  경우  getQdSwPin_Fixed()을 사용한다. 
    UINT nQdSwPin = getQdSwPin_Fixed(nSwPin);       // JYK 20200605

    // 16K든, 32K든  QD 고정 하부 시작핀이면 Hw 하부 Offset은 16384 고정.  (QD, 32K 모두 동일 HW 계측기)
    UINT nHW_UpperPartSize = (MAX_BOX_PIN *4);  // 16384



    // jigPin0는 아직 상부가 맞는데,  QD 기준으로 변환한 SWPin이 UpperPartSize를 넘어갔다면
    // ex) "8K구형, 하부핀 4097, SD 타입이면" SD이므로 jigPin0 2048이 hwPin 16384가 된다. 
    //     이때 하부핀을 4097로 지정했으므로, jigPin 4096은 반드시 hwPin도 16384가 돼야하는 hwPin 충돌이 남. 
    //     하부핀 4096, SD 인 경우는 jigPin 2048~ 4095까지는 -1로 안쓰는 영역 처리를 해야 한다.
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;   // logical 상단 Sw Pin 갯수
    if ( nSwPin < nOrg_UpperPartSize            // jigPin0는 상부
         &&   nQdSwPin >= nHW_UpperPartSize  )  // QDSwPin은 16384를 넘어감
        return -1;
    
    //---------------------------
    //HW Low Part Range 체크
    
    // 하부 HW 파트 사이즈 관련 range 체크를 한다.
    UINT nHW_LowerPartSize;
    switch (SysSet12.m_nPinTotMax)  
    {
        // 8K LOW STPIN은 실제로는 상부 8K, 하부 8K HW 사용.  jigPin만 max 8K 사용이다.
        case PIN_TOT_8K_LOW_STPIN:
            nHW_LowerPartSize = MAX_BOX_PIN * 2;       //son QD BlockType이면,  8K가 LowerPartSize. 
            if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son230216_1
                nHW_LowerPartSize = MAX_BOX_PIN * 4;
            break;

        // 12K 구형 타입은 상부 8K, 하부 4K HW에서 사용한다.
        case PIN_TOT_12K:
            nHW_LowerPartSize = MAX_BOX_PIN;    //son210202: 하부는 box 1개
            break;

        case PIN_TOT_U8K_UPPER_ONLY:    //son211020 
            nHW_LowerPartSize = 0;      //son211020 하부 없음
            break;

        case PIN_TOT_32K_U8K_L24K:                  //son230221
            nHW_LowerPartSize = MAX_BOX_PIN * 6;    //son230221
            break;                                  //son230221

        case PIN_TOT_32K_ORG:
            nHW_LowerPartSize = MAX_BOX_PIN * 4;    //son230316
            break;


        default:
            return -1;

    }

    // HW 하부시작핀(8192 또는 16384) + LowerPartSize(QD 기준 1box)  가 실질적인 하부 MaxPin임
    UINT nSwLowMaxPin = nHW_UpperPartSize + nHW_LowerPartSize;
    if (nQdSwPin >= nSwLowMaxPin) // 하부 Max-Pin설정 20200205 JYK
        return -1;

//son230221 begin:
    // HwPin 변환 결과가 32768을 넘는 경우,  U8K_L24K는 
    // bdl 상의 하부 5, 6 박스 pin을  HW 위치인 상부  3, 4로 바꿔 준다.
    // box번호로는  box 9 -> hw box 3,  box 10 -> hw box 4 가 됨.
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG 
            && SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K) { 
        if (nQdSwPin >= MAX_PIN)                                
            nQdSwPin = nQdSwPin - (MAX_BOX_PIN * 6);            
    }
//son230221 end


    //son201218 정방향 핀블록이므로 32K Trans Convert 없이 return한다.
    return nQdSwPin; 

}

//son210514 주어진 nUpperEnd값에 맞는 하부시작핀위치를 찾는다.
//    MakePinBlockRangeUserNo() 내부에서만 사용함.
int CModeSet12::FindBlockSet_LowerStartPin(int nUppderEnd)  //son210514
{
    int nLowerJigStartPin;


    //----------------------------------------
    //son 상부 3box  16K_U12K_L4K 예외케이스
    
    //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS는 여기에 해당이 안 된다. box 5가 16385로 시작해야 함.
    if (IS_UPPER_12K_2(SysSet12.m_nPinTotMax))  //son230718
    {
        //son 12K에서 QD/DD/SD 를 자유로 할당시에 나올 수 있는 하부 시작핀 (27가지 3*3)
        //    이걸 다 설정하게 해 주는 것은 오버헤드가 크므로 
        //    SD/SD/SD=3072, DD/DD/DD=6144, QD/QD/QD=12288  이렇게 3가지 경우만 일단 제공하고
        //    추후 요구사항 봐가면서 추가한다.
        //son 3072 : SD/SD/SD  
        //    4096 : SD/SD/DD, SD/DD/SD, DD/SD/SD 
        //    5120 : SD/DD/DD, DD/SD/DD, DD/DD/SD 
        //    6144 : SD/SD/QD, SD/QD/SD, DD/DD/DD, QD/SD/SD,  
        //    7168 : SD/DD/QD, SD/QD/DD, DD/SD/QD, DD/QD/SD, QD/SD/DD, QD/DD/SD  
        //    8192 : DD/DD/QD, QD/DD/DD 
        //    9216 : QD/SD/QD, QD/QD/SD 
        //    10240: DD/QD/QD, QD/DD/QD, QD/QD/DD 
        //    12288: QD/QD/QD
        nLowerJigStartPin = (4096 * 3) +1;   //son220328 
        if (nUppderEnd <= (1024 * 3))
            nLowerJigStartPin = (1024 * 3) +1;    //son SD 3073 

//son230714        else if (nUppderEnd <= (1024 * 3))
//son230714            nLowerJigStartPin = (1024 * 3) +1;    //son DD 6145
        else if (nUppderEnd <= (2048 * 3))                          //son230714 버그 수정
            nLowerJigStartPin = (2048 * 3) +1;    //son DD 6145     //son230714

        else if (nUppderEnd <= (4096 * 3))
            nLowerJigStartPin = (4096 * 3) +1;	  //son QD 12289

        return nLowerJigStartPin;
    }       //son210525

    //-----------------------
    //son 일반 케이스 

    nLowerJigStartPin = 16385;
    if (nUppderEnd <= 1024)
        nLowerJigStartPin = 1024 +1;

    else if (nUppderEnd <= 2048) 
        nLowerJigStartPin = 2048 +1;

    else if (nUppderEnd <= 4096)
        nLowerJigStartPin = 4096 +1;

    else if (nUppderEnd <= 8192)
        nLowerJigStartPin = 8192 +1;

    else if (nUppderEnd <= 16384)
        nLowerJigStartPin = 16384 +1;

    return nLowerJigStartPin;

}
   
 
 
//son sPinBlockRange[10]를  생성하고 "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
int CModeSet12::MakePinBlockRangeUserNo_32K()//son210525
{
    int nEnd, nLowBlockStartNo, nBlockType,nBlocksize;

    nEnd=0; nBlockType=0; nLowBlockStartNo=0; nBlocksize=0;


    ::ZeroMemory(&sPinBlockRange, sizeof(sPinBlockRange));
    //---------------------------------------------------------
    for (int nStep=1;  nStep<=8;  nStep++)
    {
        //son 상부 4th box만 없는 경우 
        //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS는 여기에 해당이 안 된다. 5th box 가 16385로 시작해야 함.
        if (IS_UPPER_12K_2(SysSet12.m_nPinTotMax)) //son210603    //son210826   //son230718
        {
            if (nStep==4)
                continue; 
        }

        //son 상부 3rd, 4th box가 없는 경우 
        if (IS_UPPER_8K_2(SysSet12.m_nPinTotMax))      //son210514 //son230718
        {
            if (nStep==3 || nStep==4)
                continue; 
        }

        //son 상부 2, 3, 4 box가 없는 경우 
        if (IS_UPPER_4K(SysSet12.m_nPinTotMax))       //son210514 
        {
            if (nStep==2 || nStep==3 || nStep==4)
                continue; 
        }

        //son 하부 7, 8 box가 없는 경우
        if (IS_LOWER_8K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==7 || nStep==8 )  
                continue; 
        }


        //son 하부 6, 7, 8 box가 없는 경우
        if (IS_LOWER_4K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==6 || nStep==7 || nStep==8 )  
                continue; 
        }



        switch (nStep)
        {
            case 1: nBlockType = SysSet12.m_nPinQDUpper;     break;
            case 2: nBlockType = SysSet12.m_nPinQDUpper2;    break;
            case 3: nBlockType = SysSet12.m_nPinQDUpper3;    break;
            case 4: nBlockType = SysSet12.m_nPinQDUpper4;    break;
            case 5: nBlockType = SysSet12.m_nPinQDLower;     break;
            case 6: nBlockType = SysSet12.m_nPinQDLower2;    break;
            case 7: nBlockType = SysSet12.m_nPinQDLower3;    break;
            case 8: nBlockType = SysSet12.m_nPinQDLower4;    break;

            default:
                nBlockType = SysSet12.m_nPinQDUpper;      //son210514    
                break;
        }

        //son 해당 박스 skip이면 (upper3, upper4, lower3, lower4는 SKIP 설정될 수 있다.)
        if (nBlockType == BOX_SKIP) //son241007-BSW-9
            continue;   

        switch (nBlockType)
        {
            case BOX_SKIP:      //son241007-BSW-9
                nBlocksize=0;  
                break;

            case BOX_SD:            // SD: 3
                nBlocksize=1024;  
                break;

            case BOX_DD:            // DD: 2
                nBlocksize=2048;   
                break;

            case BOX_QD:            // QD: 1
            default:
                nBlocksize=4096;      
                break;
        }


        //son HW적 하부시작 box (box1 5)
        if(nStep==5)
        {

            //son Step=5일때, QD/DD/SD 블록설정케이스에는 하부시작핀을 계산해서 사용한다. 
            //    QD/DD/SD 상태와 핀모드로 하부시작핀을 계산해서 사용한다. (지정된 값 사용안함)
            if (SysSet12.m_nBlockSet1 == 1                      //son210525
                    //son230718|| IS_UPPER_12K(SysSet12.m_nPinTotMax)  
                    || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS     //son210826 현재 nEnd에 맞는 고정하부시작핀 필요
                    || SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS    //son230718 고정하부핀 사용 특수케이스
                    || SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)     //son230718 고정하부핀 사용 특수케이스
            {                 
                //son 하부시작핀을 보정하여 현재 nEnd에 맞게 재설정한다.
                SysSet12.m_nLowerJigStartPin = FindBlockSet_LowerStartPin(nEnd);  //son210514
                nEnd = SysSet12.m_nLowerJigStartPin-1;                      //son210514
            } //son210525

            //son Step=5일때 하부시작핀 사용시에는 nEnd를 입력된 하부시작핀으로 지정한다.
            //    사용자가 지정한 하부시작핀을 기준으로 써야하므로  여기에서 하부시작핀을 변경하면 안 된다.
            else    
            {
                nLowBlockStartNo=SysSet12.m_nLowerJigStartPin;       
                if( (nEnd < nLowBlockStartNo) && ( (nLowBlockStartNo==(1024+1)) //son 1025, 2049, 4097, 8193, 16385 중 하나의 값인 하부시작핀.. 
                                                    || (nLowBlockStartNo==(2048+1)) 
                                                    || (nLowBlockStartNo==(2048*2+1))     
                                                    || (nLowBlockStartNo==(2048*4+1))  
                                                    || (nLowBlockStartNo==(2048*8+1))) )
                    nEnd=nLowBlockStartNo-1;        //son 상부 size
            } 

        }    
        sPinBlockRange[nStep].nStartPin = nEnd+1;                                        //son hwBox StartPin 
        sPinBlockRange[nStep].nEndPin   = sPinBlockRange[nStep].nStartPin+nBlocksize -1; //son hwBox EndPin 
        sPinBlockRange[nStep].nBlockType= nBlockType;                                    //son hwBox BlockType

        nEnd=sPinBlockRange[nStep].nEndPin;  //son 다음번 nStep의 StartPin 계산을 위해 nEnd에 할당.


        //son TRANS type이면 box 번호에 따라 trans 방향을 지정해 준다.
        sPinBlockRange[nStep].nHwBoxType = TYPE_AC; //son230316        
        if (IS_TRANS_TOTMAX(SysSet12.m_nPinTotMax)) //son210514
        {
            //son 상부만 Trans인 경우
            if (IS_UPPER_TRANS(SysSet12.m_nPinTotMax))  //son210531
            {
                if(nStep==2 || nStep==4 )
                    sPinBlockRange[nStep].nHwBoxType = TYPE_BD;
                else 
                    sPinBlockRange[nStep].nHwBoxType = TYPE_AC;

            }

            //son 상부, 하부 모두 Full Trans 인 경우
            else 
                //son nStep 1,3,5,7은 TYPE_AC(0),  nStep 2,4,6,8은 TYPE_BD(1)   
                sPinBlockRange[nStep].nHwBoxType = (nStep -1) % 2;          //son210514 
        }

    }

    ///////////////////////////////////////////////
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   //son210514  200 -> FIlE_PATH_SIZE
    ::ZeroMemory(&fName, sizeof(fName)); 
    CString strTemp;
    //strTemp.Format("d:\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),      //son210514
    strTemp.Format("d:\\log\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),   //son220715 d:\\log 폴더로 이동
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(sPinBlockRange[1].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[2].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[3].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[4].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[5].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[6].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[7].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[8].nBlockType)); 
    strcat( fName , strTemp ); //son210514

    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        errMessageBox(2005, "d:\\PinMap_PinBlockRange1.ini    file Data Saving Error ! ,    file close!");        //son230217_2
        return 0;
    } 
    for(int i=1; i<=8; i++)
    {
        fprintf(fp, "%s Block=%3d, %7d ~ %7d, %2s, %s\n", 
                            (i <= 4)? "Upper":"Lower", (i <= 4)? i: (i-4), //son210514
                            sPinBlockRange[i].nStartPin, sPinBlockRange[i].nEndPin,        
                            STR_BOXQD_TYPE(sPinBlockRange[i].nBlockType),                                
                            ((sPinBlockRange[i].nHwBoxType == TYPE_AC) ? "TYPE_AC":              
                             (sPinBlockRange[i].nHwBoxType == TYPE_BD) ? "TYPE_BD":"XX") );


    }
    fclose(fp); 
    ///////////////////////////////////////////////////
    return 1;
}
 
//son K600(상 8K, 하 24K)전용.  
//    sPinBlockRange[10]를  생성하고 하부시작핀을 결정하고, "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
int CModeSet12::MakePinBlockRangeUserNo_K600()//son230217
{
    int nEnd, nLowBlockStartNo, nBlockType,nBlocksize;
    CString str;

    nEnd=0; nBlockType=0; nLowBlockStartNo=0; nBlocksize=0;

    ASSERT(SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG); 
    ASSERT(SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K); 


    ::ZeroMemory(&sPinBlockRange, sizeof(sPinBlockRange));
    //---------------------------------------------------------
    for (int nStep=1;  nStep<=8;  nStep++)
    {

        switch (nStep)
        {
            case 1: nBlockType = SysSet12.m_nPinQDUpper;     break;
            case 2: nBlockType = SysSet12.m_nPinQDUpper2;    break;

            case 3: nBlockType =  SysSet12.m_nPinQDLower;    break;
            case 4: nBlockType =  SysSet12.m_nPinQDLower2;   break;
            case 5: nBlockType =  SysSet12.m_nPinQDLower3;   break;
            case 6: nBlockType =  SysSet12.m_nPinQDLower4;   break;
            case 7: nBlockType =  SysSet12.m_nPinQDLower5;   break;
            case 8: nBlockType =  SysSet12.m_nPinQDLower6;   break;

            default:
                nBlockType = SysSet12.m_nPinQDUpper;      //son210514    
                break;
        }

        switch (nBlockType)
        {
            case BOX_SKIP: nBlocksize=0;     break; // SKIP: 4 //son241007-BSW-9
            case BOX_SD:   nBlocksize=1024;  break; // SD: 3
            case BOX_DD:   nBlocksize=2048;  break; // DD: 2
            case BOX_QD:             
            default:       nBlocksize=4096;  break; // QD: 1
        }


        //son HW적 하부시작 box (box1 3). K600은 3번째 block부터 하부
        if(nStep==3)
        {
            //son PIN_TOT_32K_U8K_L24K 타입이므로 하부시작핀은 QD일 때 8193
            SysSet12.m_nLowerJigStartPin = FindBlockSet_LowerStartPin(nEnd);    //son 여기서 nEnd는 UpperEnd가 됨.

        }    
        sPinBlockRange[nStep].nStartPin = nEnd+1;                                        //son hwBox StartPin 
        sPinBlockRange[nStep].nEndPin   = sPinBlockRange[nStep].nStartPin+nBlocksize -1; //son hwBox EndPin 
        sPinBlockRange[nStep].nBlockType= nBlockType;                                    //son hwBox BlockType

        nEnd=sPinBlockRange[nStep].nEndPin;  //son 다음번 nStep의 StartPin 계산을 위해 nEnd에 할당.


        //son TRANS type이 아니므로  box 번호에 따라 trans 방향은 AC 로 고정.
        sPinBlockRange[nStep].nHwBoxType = TYPE_AC;

    }

    ///////////////////////////////////////////////
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   
    ::ZeroMemory(&fName, sizeof(fName)); 
    CString strTemp;
    strTemp.Format("d:\\log\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),   //son d:\\log 폴더로 이동
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(sPinBlockRange[1].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[2].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[3].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[4].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[5].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[6].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[7].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[8].nBlockType)); 
    strcat( fName , strTemp ); //son210514

    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        errMessageBox(2005, "d:\\PinMap_PinBlockRange1.ini    file Data Saving Error ! ,    file close!");        //son230217_2
        return 0;
    } 
    for(int i=1; i<=8; i++)
    {
        fprintf(fp, "%s Block=%3d, %7d ~ %7d, %2s, %s\n", 
                            (i <= 2)? "Upper":"Lower", (i <= 2)? i: (i-2), 
                            sPinBlockRange[i].nStartPin, sPinBlockRange[i].nEndPin,        
                            STR_BOXQD_TYPE(sPinBlockRange[i].nBlockType),                                
                            ((sPinBlockRange[i].nHwBoxType == TYPE_AC) ? "TYPE_AC":              
                             (sPinBlockRange[i].nHwBoxType == TYPE_BD) ? "TYPE_BD":"XX") );


    }
    fclose(fp); 
    ///////////////////////////////////////////////////

    return 1;
}

//son 상부 QD/DD/SD 설정 상태에 따른 UpperEndJigPin 번호를 계산한다.
//    QD/DD/SD 혼용시에만 사용가능 
int CModeSet12::CalcUpperEndJigPin_BlockSet() //son241007-BSW-9  
{
    int nBlockType, nBlocksize;
    int nUpperEndJigPin = 0;

    if (SysSet12.m_nBlockSet1 != 1)
        return -1;

    //son 상부 box 개수 만큼만 카운트한다.
    int nStep;
    for (nStep=1; nStep<= UPPER_BOX_NUM; nStep++)  
    {
        switch (nStep)
        {
            case 1: nBlockType = SysSet12.m_nPinQDUpper;     break;
            case 2: nBlockType = SysSet12.m_nPinQDUpper2;    break;
            case 3: nBlockType = SysSet12.m_nPinQDUpper3;    break;
            case 4: nBlockType = SysSet12.m_nPinQDUpper4;    break;
            default:
                nBlockType = SysSet12.m_nPinQDUpper;      //son210514    
                break;
        }

        //son 해당 박스 skip이면 (upper3, upper4 는 SKIP 설정될 수 있다.)
        if (nBlockType == BOX_SKIP) //son241007-BSW-9
            continue;   

        switch (nBlockType)
        {
            case BOX_SKIP:      //son241007-BSW-9
                nBlocksize=0;  
                break;

            case BOX_SD:            // SD: 3
                nBlocksize=1024;  
                break;

            case BOX_DD:            // DD: 2
                nBlocksize=2048;   
                break;

            case BOX_QD:            // QD: 1
            default:
                nBlocksize=4096;      
                break;
        }

        nUpperEndJigPin += nBlocksize;
    }

    return nUpperEndJigPin;

}

//son 하부시작핀을 여기서 결정
//son sPinBlockRange[10]를  생성하고 "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
int CModeSet12::MakePinBlockRangeUserNo()//sylee210402 
{
    CString str;
    int nEnd, nLowBlockStartNo, nBlockType,nBlocksize;

    nEnd=0; nBlockType=0; nLowBlockStartNo=0; nBlocksize=0;

//son230217 begin:
    //son K600(상 8K, 하 24K)용  sPinBlockRange[10]를  생성
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG) 
    {
        if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K 
                && SysSet12.m_nPinTotMax != PIN_TOT_32K_ORG )   //son230316 
        {
            str.Format("MakePinBlockRangeUserNo(): pinBlock= 32K_ORG, but PinTotMax(%d:%s) is not for 32K_ORG!\n", 
                            SysSet12.m_nPinTotMax, STR_MAX_PIN(SysSet12.m_nPinTotMax));
            errMessageBox(7701, str);
            return 0;
        }

        //son 32K ORG pinBlock, K600용 Upper 8K, Lower 24K PinMode일 때에만 호출한다.
        if (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)   //son230316
            return MakePinBlockRangeUserNo_K600();  
    }
//son230217 end

    if (SysSet12.m_nPinTotMax != PIN_TOT_32K_ORG) //son241007-BSW-9
    {
        //son 32K Trans PinBlock인 경우는 PinRange와 하부시작핀등의 계산방식이 다르므로 32K는 별도의 함수로 빼냄.   //son210525
        if (IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax))   //son210525
        {
            return MakePinBlockRangeUserNo_32K();   //son210525
        }
    }

    //------------------------------------------------
    //son 정방향 핀플록인 경우 (32K ORG 또는 OLD QD 방식)


    ::ZeroMemory(&sPinBlockRange, sizeof(sPinBlockRange));
    for (int nStep=1;  nStep<=8;  nStep++)
    {
        //son 32K base 타입인 경우 
        if( SysSet12.m_nPinTotMax==PIN_TOT_20K_UP_TR        //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_24K_UP_TR //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_24K_TRANS //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_32K_TRANS //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_32K_ORG)  //son230316
        {
        }

        //son 32K base 타입이 아닌  경우 16K이므로 box 3, 4, 7, 8 은 설정하지 않는다.
        else{
            if( nStep==3 || nStep==4 || nStep==7 || nStep==8 )  
                continue; 
        }



        switch (nStep)
        {
            case 1: nBlockType = SysSet12.m_nPinQDUpper;     break;
            case 2: nBlockType = SysSet12.m_nPinQDUpper2;    break;
            case 3: nBlockType = SysSet12.m_nPinQDUpper3;    break;
            case 4: nBlockType = SysSet12.m_nPinQDUpper4;    break;
            case 5: nBlockType = SysSet12.m_nPinQDLower;     break;
            case 6: nBlockType = SysSet12.m_nPinQDLower2;    break;
            case 7: nBlockType = SysSet12.m_nPinQDLower3;    break;
            case 8: nBlockType = SysSet12.m_nPinQDLower4;    break;

            default:
                nBlockType = SysSet12.m_nPinQDUpper;      //son210514    
                break;
        }


        //son 해당 박스 skip이면 (upper3, upper4 는 SKIP 설정될 수 있다.)
        if (nBlockType == BOX_SKIP) //son241007-BSW-9 begin
        {
            sPinBlockRange[nStep].nBlockType= nBlockType;     
            continue;   
        } //son241007-BSW-9 end

        switch (nBlockType)
        {
            case BOX_SKIP:      //son241007-BSW-9
                nBlocksize=0;  
                break;

            case BOX_SD:            // SD: 3
                nBlocksize=1024;  
                break;

            case BOX_DD:            // DD: 2
                nBlocksize=2048;   
                break;

            case BOX_QD:            // QD: 1
            default:
                nBlocksize=4096;      
                break;
        }


        //son HW 하부시작 box (box 5)
        if(nStep==5)
        {

            //son QD/DD/SD 혼용으로 계산된 하부시작핀에 offset이 더 설정된 경우 더해줘야 한다.
            if (SysSet12.m_nBlockSet1 == 1) 
                nEnd += SysSet12.m_nLowerJigStartPin_Offset; //son241007-BSW-9

            //son 하부시작핀을 보정하여 재설정한다.
            SysSet12.m_nLowerJigStartPin = nEnd +1; //son210525

        }    
        sPinBlockRange[nStep].nStartPin = nEnd+1;                                        //son hwBox StartPin 
        sPinBlockRange[nStep].nEndPin   = sPinBlockRange[nStep].nStartPin+nBlocksize -1; //son hwBox EndPin 
        sPinBlockRange[nStep].nBlockType= nBlockType;                                    //son hwBox BlockType

        nEnd=sPinBlockRange[nStep].nEndPin;  //son 다음번 nStep의 StartPin 계산을 위해 nEnd에 할당.

        //son TRANS type이면 box 번호에 따라 trans 방향을 지정해 준다.
        if( (SysSet12.m_nPinTotMax==PIN_TOT_16K_TRANS) || (SysSet12.m_nPinTotMax==PIN_TOT_24K_TRANS)  //son230216_1
                || (SysSet12.m_nPinTotMax==PIN_TOT_32K_TRANS) ) //son230216_1
        {
            if(nStep==2 || nStep==4 || nStep==6 || nStep==8 ){
                sPinBlockRange[nStep].nHwBoxType= TYPE_BD;  
            }
        }
        else if(  (SysSet12.m_nPinTotMax==PIN_TOT_20K_UP_TR) || (SysSet12.m_nPinTotMax==PIN_TOT_24K_UP_TR) ){ //son230216_1
            if(nStep==2 || nStep==4 ){
                sPinBlockRange[nStep].nHwBoxType= TYPE_BD;  
            }
        }


    }

    ///////////////////////////////////////////////
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   //son210514  200 -> FIlE_PATH_SIZE
    ::ZeroMemory(&fName, sizeof(fName)); 
    CString strTemp;
    //strTemp.Format("d:\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),      //son210514
    strTemp.Format("d:\\log\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),   //son220715 d:\\log 폴더로 이동
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(sPinBlockRange[1].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[2].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[3].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[4].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[5].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[6].nBlockType),
                                                STR_BOXQD_TYPE(sPinBlockRange[7].nBlockType),STR_BOXQD_TYPE(sPinBlockRange[8].nBlockType)); 
    strcat( fName , strTemp ); //son210514

    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        errMessageBox(2005, "d:\\PinMap_PinBlockRange1.ini    file Data Saving Error ! ,    file close!");        //son230217_2
        return 0;
    } 
    for(int i=1; i<=8; i++)
    {
        fprintf(fp, "%s Block=%3d, %7d ~ %7d, %2s, %s\n", 
                            (i <= 4)? "Upper":"Lower", (i <= 4)? i: (i-4), //son210514
                            sPinBlockRange[i].nStartPin, sPinBlockRange[i].nEndPin,        
                            STR_BOXQD_TYPE(sPinBlockRange[i].nBlockType),                                
                            ((sPinBlockRange[i].nHwBoxType == TYPE_AC) ? "TYPE_AC":              
                             (sPinBlockRange[i].nHwBoxType == TYPE_BD) ? "TYPE_BD":"XX") );

    }
    fclose(fp); 
    ///////////////////////////////////////////////////
    return 1;
}

//--------------------------------------------------------
//son nSwPin 값에 맞는 HwPin(SwitchCard pin)을 리턴한다. 
//--------------------------------------------------------
// parameter  : nSwPin      Input 정보, 0으로 시작하는 SwPin(JigPin)
// return val : nHwPin      nSwPin에 맞는 Hw SwitchCard PinBlock Pin
int CModeSet12::make32kHwPin(UINT nSwPin)
{
    UINT nPart, nHwPartBox, nHwBoxType;
    UINT nSwBoxPin, nSwLine, nSwCol;
    UINT nHwBoxPin, nHwLine, nHwCol;
    UINT nHwPin;    // return val
    
    //son-8K4097-20200605: SWMaxPin은 8K구형, HW는 16K, QDHW 고정 하부시작핀 사용시에 적용.
    if(SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN   //son SW MaxPin은 8K, HW는 16K 케이스
         || (SysSet12.m_nPinTotMax == PIN_TOT_12K)     //son210202: 12K 구형 추가
         || (SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY) //son211020 8K구형 상부만 존재하는 케이스 (f550j 나노시스) 
         || (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230221
         || (SysSet12.m_nBlockSet1 != 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG ))   //son230316  //son241007-BSW-9  
    {
        return make16kQdBase_HwPin(nSwPin);     //son211020 make8kNew_Hwpin() -> make16kQdBase_HwPin()
    }

    if (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG ) //son241007-BSW-9
        return make32kOrgBlockSet_HwPin(nSwPin);  //son block 1개 기준의 hwPin을 return.


    //------------------------------------------
    // QD, SD, DD-> QD 기준으로 swPin을 변환.  
    
    // 하부시작핀값이 Sw dependent해서 DD, SD인 경우라면 QD  기준으로 바꾸어 써야 하는 경우
    // getQdSwPin()을 사용한다. 내부적으로 getQD_UpperPartSize()를 사용함.
    UINT nQdSwPin = getQdSwPin(nSwPin);         // JYK 20200117
    

    
    // 하부시작핀정보로 QD 기준의 SW UpperPartSize 결정.
    UINT nSw_UpperPartSize = getQD_UpperPartSize();
     

    //--------------------
    // nSwPin Range Check
    
    // 하부가 상부보다 큰 경우도 고려함. (하부시작핀 4096이면..) //son-32KPIN-20200103 
    UINT nSwMaxPin; 
    UINT nSwLowMaxPin;  // 하부 Max-Pin
    UINT nLowerPartSize;
    switch (SysSet12.m_nPinTotMax)
    {
        case PIN_TOT_8K_TRANS:     //son210319 //son230216_1
            nSwMaxPin = (2 * MAX_BOX_PIN);      // 2 *4096  (U: 1box, L: 1box)
            nLowerPartSize = MAX_BOX_PIN;       // 4096 
            break;

        case PIN_TOT_12K_TRANS:    //son210319 //son230216_1
            nSwMaxPin = (3 * MAX_BOX_PIN);      // 3 *4096  (U: 2box, L: 1box)
            nLowerPartSize = (2 *MAX_BOX_PIN);  // 8192 
            break;

        case PIN_TOT_16K_TRANS: //son230216_1
            nSwMaxPin = (4 * MAX_BOX_PIN);      // 4 *4096  (U: 2box, L: 2box)
            nLowerPartSize = (2 *MAX_BOX_PIN);  // 8192 //20200205 JYK
            break;

        case PIN_TOT_20K_UP_TR: //son230216_1
        case PIN_TOT_20K_U12K_L4K_UP_TRANS:  //son210603 특이케이스  skip 4th box.  QD하부시작핀 16385 고정
        case PIN_TOT_20K_U8K_L4K_UP_TRANS:   //son210826 특이케이스  skip 3rd, 4th box. QD 하부시작핀 16385 고정
            nSwMaxPin = (5 * MAX_BOX_PIN);      // 5 *4096  (U: 4box, L: 1box)
            nLowerPartSize = MAX_BOX_PIN;       // 4096  //20200205 JYK
            break;

        case PIN_TOT_20K_U12K_L8K_TRANS:  //son230714
            nSwMaxPin = (5 * MAX_BOX_PIN);      // 5 *4096  (U: 3box, L: 2box)
            nLowerPartSize = (2 *MAX_BOX_PIN);  // 4096  
            break;

        case PIN_TOT_24K_UP_TR:     //son230216_1
        case PIN_TOT_24K_TRANS:     //son230216_1
        case PIN_TOT_24K_U12K_L8K_TRANS:  //son230718
            nSwMaxPin = (6 * MAX_BOX_PIN);      // 6 *4096  (U: 4box, L: 2box)
            nLowerPartSize =  (2 * MAX_BOX_PIN);//8192 //20200205 JYK
            break;

        case PIN_TOT_16K_U12K_L4K_TRANS:        //son210525
        case PIN_TOT_16K_U12K_L4K_UP_TRANS:     //son210531
            nSwMaxPin = (4 * MAX_BOX_PIN);      // 4 *4096  (U: 3box, L: 1box)
            nLowerPartSize =  (1 * MAX_BOX_PIN);//4096 
            break;

        case PIN_TOT_32K_TRANS: //son230216_1
            nSwMaxPin = MAX_PIN;                // 8 * 4096 (U: 4box, L: 4box)
            nLowerPartSize = (4 * MAX_BOX_PIN); //16384  20200205 JYK
            break;

        default:
            return -1;
    }


    if (nQdSwPin < 0 || nQdSwPin >= nSwMaxPin)
        return -1;

    // 하부시작핀이 16384보다 작은 값이면 nSwMaxPin이 아니라 nSwLowMaxPin이 실질적인 swPin의 Max 값이다.
    // nSwLowMaxPin은 하부시작핀+ lowerpart의 사이즈이므로 실질적인 maxPin이 됨.
    nSwLowMaxPin = nSw_UpperPartSize + nLowerPartSize;
    if (nQdSwPin >= nSwLowMaxPin) // 하부 Max-Pin설정 20200205 JYK
        return -1;


    //-------------------
    // SwPin 정보 분석 
    //-------------------
    
    // Part(Upper or Lower) > Box (A~D) > Line (0~63) > Column (0~63)
    
    // 상단인지 하단인지 Part를 알아 낸다.
    // UpperPartSize가 4096, 8193일 수 있으므로 swPin / PartSize로는 파트위치를 알 수 없으므로
    // UpperPartSize보다 swPin이 크면 하부로 인식한다.
    nPart  = (nQdSwPin >= nSw_UpperPartSize) ? PART_LOWER: PART_UPPER;      
    UINT nSwBox = nQdSwPin / MAX_BOX_PIN;           // SW Part별 box 번호 (0~7)

    

    //----------------------------------------------------------------------------
    // 하부핀이면 하부 StartPin을 고려해서 하부 Box 번호를 Hw 기준으로 다시 할당.  
    UINT nHwBox = nSwBox;
    if (nPart == PART_LOWER)    
    {

        // 상부 box 갯수: nSw_UpperPartSize는 MAX_BOX_PIN:4096의 배수임 
        int nSwUpperBoxNum = nSw_UpperPartSize / MAX_BOX_PIN;   // nSw_UpperPartSize    nSwPartBoxNum
                                                                //  16384(16K)          4
                                                                //  12288(12K)          3
                                                                //  8192 (8K)           2
                                                                //  4096 (4K)           1
    
        // err check
        ASSERT(nSwUpperBoxNum == 4 || nSwUpperBoxNum == 3 || nSwUpperBoxNum == 2 || nSwUpperBoxNum == 1);

        // 하부핀인데 하부시작핀 16385이면(상부 box 4개 사용) SwBox에 +0하고,  
        // 하부핀인데 하부시작핀 12288이면(상부 box 3개만 사용) SwBox에 +1하고,  
        // 하부핀인데 하부시작핀 8193 이면(상부 box 2개만 사용) SwBox에 +2하고,  
        //            하부시작핀 4097 이면(상부 box 1개만 사용) SwBox에 +3한다.
        nHwBox = nSwBox + (MAX_BOX_PER_PART - nSwUpperBoxNum);
        if (nHwBox >= MAX_BOX)
            return -1;
        
        //----------------------------------------------------------
        //for 32K (Upper 16K Transpose  + Lower 16K Transpose) base             

        //ex) HW Upper 16K, 하부시작핀 QD 16385(DD 8193, SD 4097), 상부 4box QD, 하부 4box QD
        //      QDSwPin(0~)  DDSw   SDSw    Part    SwBox HwPartBox nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4030        2015    1023    0       0       0:A     4030        126         126
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      
        //      4096        2048    1024    0:U     1       1:B     0           63          4159
        //      8126        4063    2047    0       1       1:B     4030        3969        8065
        //      8191        4095    -       0:U     1       1:B     4095        4032        8128
        //      
        //      8192        4096    2048    0:U     2       2:C     0           4032        12224
        //      12222       6111    3071    0       2       2:C     4030        126         8318
        //      12287       6143    -       0:U     2       2:C     4095        63          8255
        //      
        //      12288       6144    3072    0:U     3       3:D     0           63          12351
        //      16318       8159    4095    0       3       3:D     4030        3969        16257
        //      16383       8191    -       0:U     3       3:D     4095        4032        16320
        //      
        //      16384       8192    4096    1:L     4       0:A     0           4032        20416 <==하부시작핀 
        //      20414       10207   5119    1:L     4       0:A                             16510
        //      20479       10239   -       1:L     4       0:A     4095        63          16447
        //      
        //      20480       10240   5120    1:L     5       1:B     0           63          20543
        //      24510       12255   6143    1:L     5       1:B                             24449
        //      24575       12287   -       1:L     5       1:B     4095        4032        24512
        //      
        //      24576       12288   6144    1:L     6       2:C     0           4032        28608
        //      28606       14303   7167    1:L     6       2:C                             24702
        //      28671       14335   -       1:L     6       2:C     4095        63          24639
        //      
        //      28672       14336   7168    1:L     7       3:D     0           63          28735
        //      32702       16351   8191    1:L     7       3:D                             32641
        //      32767       16383   -       1:L     7       3:D     4095        4032        32704
        
        //ex) HW Upper는 16K, 하부시작핀 QD 8193(DD 4097, SD 2049), 상부 2box QD, 하부 4box QD
        //      QDSwPin(0~) DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      4096        2048    1024    0:U     1       1:B     0           63          4159
        //      8191        4095    -       0:U     1       1:B     4095        4032        8128
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       4:D     4095        4032        16320
        //                               
        //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==하부시작핀
        //      12287       6143    -       1:L     2       0:A     4095        63          16447
        //      12288       6144    3072    1:L     3       1:B     0           63          20543
        //      16383       8191    -       1:L     3       1:B     4095        4032        24512
        //      16384       8192    4096    1:L     4       2:C     0           4032        28608
        //      20479       10239   -       1:L     4       2:C     4095        63          24639
        //      20480       10240   5120    1:L     5       3:D     0           63          28735
        //      24575       12288   -       1:L     5       3:D     4095        4032        32704
        //      24576 이후      없음
        
        //ex) HW Upper는 16K, 하부시작핀 QD 4097(DD 2049, SD 1025), 상부 1box, 하부 4box
        //      SwPin(0~)   DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //      0           0       0       0:U     0       0:A     0           4032        4032
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      Unused      -       -       0:U     x       1:B     0           63          4159
        //      Unused      -       -       0:U     x       1:B     4095        4032        8128
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       4:D     4095        4032        16320
        //                               
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===하부시작핀
        //      8191        4095    -       1:L     1       0:A     4095        63          16447
        //      8192        4096    2048    1:L     2       1:B     0           63          20543
        //      12287       6143    -       1:L     2       1:B     4095        4032        24512
        //      12288       6144    3072    1:L     3       2:C     0           4032        28608
        //      16383       8191    -       1:L     3       2:C     4095        63          24639
        //      16384       8192    4096    1:L     4       3:D     0           63          28735
        //      20479       10240   -       1:L     4       3:D     4095        4032        32704
        //      20480 이후       없음
        
        
    }                                

    nHwPartBox = nHwBox % MAX_BOX_PER_PART; // % 4   Hw Part별 box번호로 전환.

    // 짝수는 TYPE_AC, 홀수는 TYPE_BD
    nHwBoxType = nHwPartBox % 2;    
/*
    if (nPart == PART_LOWER)    // 하부 Box-Type설정 20200205 JYK
        nHwBoxType = ((nSwPin - nSw_UpperPartSize) / MAX_BOX_PIN) % 2;
    else
        nHwBoxType = (nSwPin / MAX_BOX_PIN) % 2;
*/

 
    nSwBoxPin  = nQdSwPin % MAX_BOX_PIN;        // Box내 SwPin 번호 (4096배수의 offset을 제외한 swPin No)
    nSwLine    = nSwBoxPin / MAX_BOX_LINE;
    nSwCol     = nSwBoxPin % MAX_BOX_LINE;

    // HW Pin을 왼쪽 상단으로 고정시키고 4096 배수의 offset을 제외하면
    // Upper_A, Upper_C, Lower_A, Lower_C의 핀배치가 동일하므로 TYPE_AC로 설정.
    int nHwPartBox_forCheck = nHwPartBox;       //son210514
    if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
    {
       // if( (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS) || (SysSet12.m_nPinTotMax==PIN_TOT_24K_TRANS)  //son230216_1
       //           || (SysSet12.m_nPinTotMax==PIN_TOT_32K_TRANS) ){
//      if(nPinBlockRange[m_nBlockPo1][3]==2){ //tran  //sylee210402
//          nHwBoxType = TYPE_BD;
//      }else{
//          nHwBoxType = TYPE_AC;
//      }
        nHwBoxType = sPinBlockRange[m_nBlockPo1].nHwBoxType;    //son210514

        //son20514  QD/DD/SD BlockSet  상태라면 하부박스인지 여부를  m_nBlockPo1로 판단한다.
        nHwPartBox_forCheck = m_nBlockPo1 -5; //son210514
    }


    //-----------------------------------------------
    //for 24K (12K Upper Trans + 8K Lower Trans)
    //-----------------------------------------------
    //son Upper 4번째 Box(nHwParBox 3)가 없다.
    if ((SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)    //son230718
            && nPart == PART_UPPER )    
    {
        if (nHwPartBox == 3)    //son230718
            return -1;
    }


    //----------------------------------------------------------
    //for 20K (16K Upper Transpose  + 4K Lower Original) base           
    //for 24K (16K Upper Transpose  + 8K Lower Original) base           
    //for 12K Upper Trans + 4K Lower Original base          
    //----------------------------------------------------------
    
    
    //son-32KPIN-20200103 
    // 상부핀만  Transpose 변형 타입이다. 하부핀은 기존 16K와 같은 Original 기본 타입.
    // 하부핀인데 20K, 24K이면 swPin 배치순서를 hwPin으로 그대로 사용해서 nHwPin을 만들고 리턴한다.
    // 하부핀인데 32K or 24K_FULL이거나,  상부핀인  경우는 이 코드 다음으로  진행.
    //if(SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR ) //son230216_1
    if(IS_UPPER_TRANS(SysSet12.m_nPinTotMax))  //son210531
    {
        //son 상부 12K Trans , 하부 4K Origin인 경우는 Upper 4번째 Box(nHwParBox 3)가 없다.
        if ((SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS  
                    || SysSet12.m_nPinTotMax ==  PIN_TOT_20K_U12K_L4K_UP_TRANS)    //son210603
                && nPart == PART_UPPER )    //son210531  
        {
            if (nHwPartBox == 3)    //son210531
                return -1;
        }

        //son 상부 8K Trans , 하부 4K Origin인 경우는 Upper 3, 4번째 Box(nHwParBox 2,3)가 없다.
        if (SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS  && nPart == PART_UPPER)    //son210826  
        {
            if (nHwPartBox == 2 || nHwPartBox == 3)    //son210826
                return -1;
        }


        //son 하부 박스라면
        //son20514  QD/DD/SD BlockSet  상태라면 하부박스인지 여부를  m_nBlockPo1로 판단한다.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                //son210514
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )    //son210514 
        {

            //son UpperTrans이고 하부에 box가 1개밖에 없는 경우  BoxNum은 1개를 넘을 수 없다.
            // IS_UPPER_TRANS(SysSet12.m_nPinTotMax)인지는 위에서 이미 체크함 
            if (IS_LOWER_4K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 1)  //son210826
                return -1;

            //son UpperTrans이고 하부에 box가 2개밖에 없는 경우  Hw BoxNum은 2개를 넘을 수 없다.
            if (IS_LOWER_8K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 2)  //son210826
                return -1;

            // 20K -------------------------------
            //
            //ex) PinTotMax는 20K, 하부시작핀 QD 16385(DD 8193, SD 4097), 상부 4box, 하부 1box
            //      QdSwPin(0~) DDSw    SDSw    Part    SwBox HwPartBox nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0       0       0:A     0           4032        4032
            //      4030        2015    1023    0       0       0:A     4030        126         126
            //      4095        2047    -       0       0       0:A     4095        63          63  
            //
            //      4096        2048    1024    0       1       1:B     0           63          4159
            //      8126        4063    2047    0       1       1:B     4030        3969        8065
            //      8191        4095    -       0       1       1:B     4095        4032        8128
            //      
            //      8192        4096    2048    0       2       2:C     0           4032        12224
            //      12222       6111    3071    0       2       2:C     4030        126         8318
            //      12287       6143    -       0       2       2:C     4095        63          8255
            //
            //      12288       6144    3072    0       3       3:D     0           63          12351
            //      16318       8159    4095    0       3       3:D     4030        3969        16257
            //      16383       8191    -       0       3       3:D     4095        4032        16320
            //                               
            //      16384       8192    4096    1:L     4       0:A     0           0           16384 <==하부시작핀 
            //      20414       10207   5119    1:L     4       0:A     4030        4030        20414
            //      20479       10239   -       1:L     4       0:A     4095        4095        20479
            //      20480 이후 없음.            

            //ex) PinTotMax는 20K, 하부시작핀 QD 8193(DD 4097, SD 2049), 상부 2box,  하부 1box
            //      QdSwPin(0~) DDSw    SDSw    Part    SwBox   HwPartBox   nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0       0       0:A         0           4032        4032
            //      4030        2015    1023    0       0       0:A         4030        126         126
            //      4095        2047    -       0       0       0:A         4095        63          63  
            //
            //      4096        2048    1024    0       1       1:B         0           63          4159
            //      8126        4063    2047    0       1       1:B         4030        3962        8065
            //      8191        4095    -       0       1       1:B         4095        4032        8128
            //
            //      Unused      -       -       0       x       2:C         0           4032        12224
            //      Unused                      0       x       2:C                                 8328
            //      Unused      -       -       0       x       2:C         4095        63          8255
            //
            //      Unused      -       -       0       x       3:D         0           63          12351
            //      Unused                      0       x       3:D                                 16257
            //      Unused      -       -       0       x       3:D         4095        4032        16320
            //                              
            //      8192        4096    2048    1:L     2       0:A         0           0           16384 <==하부시작핀
            //      12222       6111    3071    1:L     2       0:A         4030        4030        20414
            //      12287       6143     -      1:L     2       0:A         4095        4095        20479
            //      12288이후      없음

            //ex) PinTotMax는 20K, 하부시작핀   QD 4097(DD 2049, SD 1025) 상부 1box, 하부 1box
            //      QdSwPin(0~) DDSw    SDSw    Part    SwBox   HwPartBox  nSwBoxPin    nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A         0           4032        4032
            //      4030        2015    1023    0:U     0       0:A         4030        126         126
            //      4095        2047    -       0:U     0       0:A         4095        63          63  
            //
            //      Unused      -       -       0       x       1:B         0           63          4159
            //      Unused                      0       x       1:B                                 8065
            //      Unused      -       -       0       x       1:B         4095        4032        8128
            //
            //      Unused      -       -       0       x       2:C         0           4032        12224
            //      Unused                      0       x       2:C                                 8328    
            //      Unused      -       -       0       x       2:C         4095        63          8255
            //
            //      Unused      -       -       0       x       3:D         0           63          12351
            //      Unused                      0       x       3:D                                 16257
            //      Unused      -       -       0       x       3:D         4095        4032        16320
            //                              
            //      4096        2048    1024    1:L     1       0:A         0           0           16384 <===하부시작핀
            //      8126        4063    2047    1:L     1       0:A         4030        4030        20414
            //      8191        4095     -      1:L     1       0:A         4095        4095        20479
            //      8192 이후       없음



            // 24K -------------------------------
            //  
            //ex) PinTotMax는 24K, 하부시작핀 QD 16385(DD 8193, SD 4097), 상부 4box, 하부 2box
            //      QdSwPin(0~) DDSw    SDSw    Part  SwBox  HwPartBox  nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A     0           4032        4032
            //      4030        2015    1023    0:U     0       0:A     4030        126         126 
            //      4095        2047    -       0:U     0       0:A     4095        63          63  
            //
            //      4096        2048    1024    0:U     1       1:B     0           63          4159
            //      8126        4063    2047    0:U     1       1:B     4030        3969        8065
            //      8191        4095    -       0:U     1       1:B     4095        4032        8128
            //
            //      8192        4096    2048    0:U     2       2:C     0           4032        12224
            //      12222       6111    3071    0:U     2       2:C     4030        126         8318
            //      12287       6143    -       0:U     2       2:C     4095        63          8255
            //
            //      12288       6144    3072    0:U     3       3:D     0           63          12351
            //      16318       8159    4095    0:U     3       3:D     4030        3969        16257
            //      16383       8191    -       0:U     3       3:D     4095        4032        16320
            //                              
            //      16384       8192    4096    1:L     4       0:A     0           0           16384 <==하부시작핀 
            //      20414       10207   5119    1:L     4       0:A     4030        3969        20414
            //      20479       10239   -       1:L     4       0:A     4095        4095        20479
            //
            //      20480       10240   5120    1:L     5       1:B     0           0           20480
            //      24510       12255   6143    1:L     5       1:B     4030        3969        24510
            //      24575       12287   -       1:L     5       1:B     4095        4095        24575
            //      24576 이후 없음.            

            //ex) PinTotMax는 24K, 하부시작핀 QD 8193(DD 4097, SD 2049), 상부 2box,  하부 2box
            //      QdSwPin(0~) DDSw    SDSw    Part    SwBox HwPartBox nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A     0           4032        4032
            //      4030        2015    1023    0:U     0       0:A     4030        126         126
            //      4095        2047    -       0:U     0       0:A     4095        63          63  
            //
            //      4096        2048    1024    0:U     1       1:B     0           63          4159
            //      8126        4063    2047    0:U     1       1:B     4030        3969        8065
            //      8191        4095    -       0:U     1       1:B     4095        4032        8128
            //
            //      Unused      -       -       0:U     x       2:C     0           4032        12224
            //      Unused      -       -       0:U     x       2:C                             8318
            //      Unused      -       -       0:U     x       2:C     4095        63          8255
            //
            //      Unused      -       -       0:U     x       3:D     0           63          12351
            //      Unused      -       -       0:U     x       3:D                             16257
            //      Unused      -       -       0:U     x       3:D     4095        4032        16320
            //                               
            //      8192        4096    2048    1:L     2       0:A     0           0           16384 <==하부시작핀
            //      12222       6111    3071    1:L     2       0:A     4030        3969        20414
            //      12287       6143     -      1:L     2       0:A     4095        4095        20479
            //
            //      12288       6144    3072    1:L     3       1:B     0           0           20480
            //      16318       8159    4095    1:L     3       1:B     4030                    24510
            //      16383       8191     -      1:L     3       1:B     4095        4095        24575
            //      16384이후      없음

            //ex) PinTotMax는 24K, 하부시작핀   QD 4097(DD 2049, SD 1025), 상부 1box, 하부 2box
            //      QdSwPin(0~) DDSw    SDSw    Par   SwBox HwPartBox   nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A     0           4032        4032
            //      4030        2015    1023    0:U     0       0:A     4030        126         126
            //      4095        2047    -       0:U     0       0:A     4095        63          63  
            //
            //      Unused      -       -       0:U     x       1:B     0           63          4159
            //      Unused      -       -       0:U     x       1:B     4030        3969        8065
            //      Unused      -       -       0:U     x       1:B     4095        4032        8128
            //
            //      Unused      -       -       0:U     x       2:C     0           4032        12224
            //      Unused      -       -       0:U     x       2:C                             8318
            //      Unused      -       -       0:U     x       2:C     4095        63          8255
            //
            //      Unused      -       -       0:U     x       3:D     0           63          12351
            //      Unused      -       -       0:U     x       3:D                             16257
            //      Unused      -       -       0:U     x       3:D     4095        4032        16320
            //                              
            //      4096        2048    1024    1:L     1       0:A     0           0           16384 <===하부시작핀
            //      8126        4063    2047    1:L     1       0:A     4030                    20414
            //      8191        4095     -      1:L     1       0:A     4095        4095        20479
            //
            //      8192        4096    2048    1:L     2       1:B     0           0           20480
            //      12222       6111    3071    1:L     2       1:B     4030        4030        24510
            //      12287       6143     -      1:L     2       1:B     4095        4095        24575
            //      12288 이후       없음
            
            
            // Upper 12K Trans + Lower 4K Org -------------------------------
            //ex) PinTotMax는 16K, 하부시작핀 QD 12289(DD 6145, SD 3073), 상부 3box, 하부 1box
            //      QdSwPin(0~) DDSw    SDSw    Part    SwBox HwPartBox nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0       0       0:A     0           4032        4032
            //      4030        2015    1023    0       0       0:A     4030        126         126
            //      4095        2047    -       0       0       0:A     4095        63          63  
            //
            //      4096        2048    1024    0       1       1:B     0           63          4159
            //      8126        4063    2047    0       1       1:B     4030        3969        8065
            //      8191        4095    -       0       1       1:B     4095        4032        8128
            //      
            //      8192        4096    2048    0       2       2:C     0           4032        12224
            //      12222       6111    3071    0       2       2:C     4030        126         8318
            //      12287       6143    -       0       2       2:C     4095        63          8255
            //
            //      Unused      -       -       0       3       3:D     0           63          12351
            //      Unused      -       -       0       3       3:D     4030        3969        16257
            //      Unused      -       -       0       3       3:D     4095        4032        16320
            //                               
            //      12288       6144    3072    1:L     4       0:A     0           0           16384 <==하부시작핀 
            //      16318       8159    4095    1:L     4       0:A     4030        4030        20414
            //      16383       8191    -       1:L     4       0:A     4095        4095        20479
            //      16384 이후 없음.            


            //---------------------------------
            // 20K, 24K Lower HwPin 정보 계산 
            //---------------------------------

            // 20K, LOWER Part 인 경우는 nHwBoxPin 대신 nSwBoxPin으로 nHwPin을 계산해서 여기서 리턴한다.
            // Original 순서를 그대로 유지하므로, 하기 변형단계까지 갈 필요가 없음.
            nHwPin = nSwBoxPin + (nHwPartBox* MAX_BOX_PIN)      // BoxPin + Box Offset 
                + (nPart * MAX_PART_PIN);       // + HW PartOffset (upper size)

            return nHwPin;

        }
    }


    //---------------------------------------------------
    //for 24K FULL Transpose (Upper 16K + Lower 8K) base                //son-32KPIN-20200103 
    //---------------------------------------------------
    // 하부핀인데 24K 인 경우는 하부에 box가 2개밖에 없으므로 하부 Hw BoxNum은 2개를 넘을 수 없다.
    // 전반적으로 pin 값들은 32K 케이스와 배치가 같은데,  하부 C, D box는 없는 것이 특징.
    // Pin 배치는 32K Full과 완전히 동일하다. 

    if (SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son 하부박스라면
        //son20514  QD/DD/SD BlockSet  상태라면 하부박스인지 여부를  m_nBlockPo1로 판단한다.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                    //son210514 
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )         
        {
            if (nHwPartBox_forCheck >= 2) //son210514
                return -1;

            //ex) PinTotMax는 24K, 하부시작핀 QD 16385(DD 8193, SD 4097), 상부 4box, 하부 2box
            //  QdSwPin(0~)     DDSw    SDSw    Part    Box     HwBox   nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A     0           4032        4032
            //      4030        2015    1023    0       0       0:A     4030        126         126
            //      4095        2047    -       0:U     0       0:A     4095        63          63  
            //      
            //      4096        2048    1024    0:U     1       1:B     0           63          4159
            //      8126        4063    2047    0       1       1:B     4030        3969        8065
            //      8191        4095    -       0:U     1       1:B     4095        4032        8128
            //      
            //      8192        4096    2048    0:U     2       2:C     0           4032        12224
            //      12222       6111    3071    0       2       2:C     4030        126         8318
            //      12287       6143    -       0:U     2       2:C     4095        63          8255
            //      
            //      12288       6144    3072    0:U     3       3:D     0           63          12351
            //      16318       8159    4095    0       3       3:D     4030        3969        16257
            //      16383       8191    -       0:U     3       3:D     4095        4032        16320
            //      
            //      16384       8192    4096    1:L     4       0:A     0           4032        20416 <==하부시작핀 
            //      20414       10207   5119    1:L     4       0:A                             16510
            //      20479       10239   -       1:L     4       0:A     4095        63          16447
            //      
            //      20480       10240   5120    1:L     5       1:B     0           63          20543
            //      24510       12255   6143    1:L     5       1:B                             24449
            //      24575       12287   -       1:L     5       1:B     4095        4032        24512
            //      24576 이후 없음.            

            //ex) PinTotMax는 24K, 하부시작핀 QD 8193(DD 4097, SD 2049), 상부 2box, 하부 2box
            //      QDSwPin(0~) DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A     0           4032        4032
            //      4030        2015    1023    0       0       0:A     4030        126         126
            //      4095        2047    -       0:U     0       0:A     4095        63          63  
            //      4096        2048    1024    0:U     1       1:B     0           63          4159
            //      8191        4095    -       0:U     1       1:B     4095        4032        8128
            //      Unused      -       -       0:U     x       2:C     0           4032        12224
            //      Unused      -       -       0:U     x       2:C     4095        63          8255
            //      Unused      -       -       0:U     x       3:D     0           63          12351
            //      Unused      -       -       0:U     x       4:D     4095        4032        16320
            //                                  
            //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==하부시작핀
            //      12287       6143    -       1:L     2       0:A     4095        63          16447
            //      12288       6144    3072    1:L     3       1:B     0           63          20543
            //      16383       8191    -       1:L     3       1:B     4095        4032        24512
            //      16384이후      없음

            //ex) PinTotMax는 24K, 하부시작핀 QD 4097(DD 2049, SD 1025), 상부 1box, 하부 2box
            //      SwPin(0~)   DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
            //         0        0       0       0:U     0       0:A     0           4032        4032
            //      4030        2015    1023    0       0       0:A     4030        126         126
            //      4095        2047    -       0:U     0       0:A     4095        63          63  
            //      Unused      -       -       0:U     x       1:B     0           63          4159
            //      Unused      -       -       0:U     x       1:B     4095        4032        8128
            //      Unused      -       -       0:U     x       2:C     0           4032        12224
            //      Unused      -       -       0:U     x       2:C     4095        63          8255
            //      Unused      -       -       0:U     x       3:D     0           63          12351
            //      Unused      -       -       0:U     x       4:D     4095        4032        16320
            //                               
            //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===하부시작핀
            //      8191        4095    -       1:L     1       0:A     4095        63          16447
            //      8192        4096    2048    1:L     2       1:B     0           63          20543
            //      12287       6143    -       1:L     2       1:B     4095        4032        24512
            //      12288 이후      없음
        }   

    }

    //---------------------------------------------------
    //for 16K FULL Transpose (Upper 8K + Lower 8K) base                 //son-32KPIN-20200228 
    //for 12K FULL Transpose (Upper 8K + Lower 4K) base                 //son210319
    //---------------------------------------------------
    // 하부핀인데 16K 인 경우는 하부에 box가 2개밖에 없으므로 하부 Hw BoxNum은 2개를 넘을 수 없다.
    // 전반적으로 pin 값들은 32K 케이스와 배치가 같은데, 상부 C, D 하부 C, D box는 없는 것이 특징.
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS //son230216_1
            || SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS)     //son210319 //son230216_1
    {

        //son 상부, 하부 관계없이 PartBox C, D는 없음. 
        if (nHwPartBox_forCheck >= 2) //son210514
            return -1;      

       // PIN_TOT_16K_TRANS 는 하부시작핀 QD 16385는 없음.
        // 하부시작핀은 QD 8193, QD 4097 두 케이스만 가능하다.
        // Pin 배치는 32K Full과 완전히 동일하다. 

        // 16K FullTrans -------------------------------
        //
        //ex) PinTotMax는 16K, 하부시작핀 QD 8193(DD 4097, SD 2049), 상부 2box, 하부 2box
        //      QDSwPin(0~) DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4030        2015    1023    1:U     0       0:A                             126
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      
        //      4096        2048    1024    0:U     1       1:B     0           63          4159
        //      8126        4063    2047    0:U     1       1:B                             8065
        //      8191        4095    -       0:U     1       1:B     4095        4032        8128
        //      
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       4:D     4095        4032        16320
        //                                  
        //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==하부시작핀
        //      12222       6111    3071    1:L     2       0:A                             16510
        //      12287       6143     -      1:L     2       0:A     4095        63          16447
        //      
        //      12288       6144    3072    1:L     3       1:B     0           63          20543
        //      16318       8159    4095    1:L     3       1:B                             24449
        //      16383       8191     -      1:L     3       1:B     4095        4032        24512
        //      16384이후      없음
        
        //ex) PinTotMax는 16K, 하부시작핀 QD 4097(DD 2049, SD 1025), 상부 1box, 하부 2box
        //      SwPin(0~)   DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4030        2015    1023    0:U     0       0:A                             126
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      
        //      Unused      -       -       0:U     x       1:B     0           63          4159
        //      Unused      -       -       0:U     x       1:B     4095        4032        8128
        //      
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       4:D     4095        4032        16320
        //                                  
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===하부시작핀
        //      8126        4063    2047    1:L     1       0:A                             16510
        //      8191        4095     -      1:L     1       0:A     4095        63          16447
        //      
        //      8192        4096    2048    1:L     2       1:B     0           63          20543
        //      12222       6111    3071    1:L     2       1:B                             24449
        //      12287       6143     -      1:L     2       1:B     4095        4032        24512
        //      12288 이후      없음
        

        // 12K FullTrans -------------------------------  //son210319
        //  
        //ex) PinTotMax는 16K, 하부시작핀 QD 8193(DD 4097, SD 2049), 상부 2box, 하부 1box
        //      QDSwPin(0~) DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4030        2015    1023    1:U     0       0:A                             126
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      
        //      4096        2048    1024    0:U     1       1:B     0           63          4159
        //      8126        4063    2047    0:U     1       1:B                             8065
        //      8191        4095    -       0:U     1       1:B     4095        4032        8128
        //      
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       4:D     4095        4032        16320
        //                                  
        //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==하부시작핀
        //      12222       6111    3071    1:L     2       0:A                             16510
        //      12287       6143     -      1:L     2       0:A     4095        63          16447
        //      12288이후      없음
        //      
        
        //ex) PinTotMax는 16K, 하부시작핀 QD 4097(DD 2049, SD 1025), 상부 1box, 하부 1box
        //      SwPin(0~)   DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4030        2015    1023    0:U     0       0:A     4030        126         126
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      
        //      Unused      -       -       0:U     x       1:B     0           63          4159
        //      Unused      -       -       0:U     x       1:B     4095        4032        8128
        //      
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       4:D     4095        4032        16320
        //                                  
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===하부시작핀
        //      8126        4063    2047    1:L     1       0:A                             16510
        //      8191        4095     -      1:L     1       0:A     4095        63          16447
        //      8192 이후      없음

    }

    //---------------------------------------------------
    //for 8K FULL Transpose (Upper 4K + Lower 4K) base              //son210319
    //---------------------------------------------------
    // 하부핀인데 8K 인 경우는 하부에 box가 1개밖에 없으므로 하부 Hw BoxNum은 1개를 넘을 수 없다.
    // 전반적으로 pin 값들은 32K 케이스와 배치가 같은데, 상부 B, C, D 하부 B, C, D box는 없는 것이 특징.
    if (SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)     //son210319 //son230216_1
    {

        //son 상부, 하부 관계없이 PartBox B, C, D는 없음. 
        if (nHwPartBox_forCheck >= 1) //son210514
            return -1;

        // PIN_TOT_8K_TRANS  하부시작핀은 QD 4097 만 가능하다.
        // Pin 배치는 32K Full과 완전히 동일하다. 
        
        //ex) PinTotMax는 8K, 하부시작핀 QD 4097(DD 2049, SD 1025), 상부 1box, 하부 1box
        //      SwPin(0~)   DDSw    SDSw    Part    SwBox   HwBox   nSwBoxPin   nHwBoxPin   nHwPin
        //         0        0       0       0:U     0       0:A     0           4032        4032
        //      4030        2015    1023    0:U     0       0:A     4030        126         126
        //      4095        2047    -       0:U     0       0:A     4095        63          63  
        //      
        //      Unused      -       -       0:U     x       1:B     0           63          4159
        //      Unused      -       -       0:U     x       1:B     4095        4032        8128
        //      
        //      Unused      -       -       0:U     x       2:C     0           4032        12224
        //      Unused      -       -       0:U     x       2:C     4095        63          8255
        //      
        //      Unused      -       -       0:U     x       3:D     0           63          12351
        //      Unused      -       -       0:U     x       3:D     4095        4032        16320
        //                                  
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===하부시작핀
        //      8126        4063    2047    1:L     1       0:A                             16510
        //      8191        4095     -      1:L     1       0:A     4095        63          16447
        //      8191이후      없음
        

    }

    //--------------------------
    // 변형 HwPin 정보 계산 
    //--------------------------
    // 대상: 32K Upper & Lower, 
    //       24K_FULL Upper & Lower, 
    //       20K Upper,
    //       24K Upper  



    if (nHwBoxType == TYPE_AC)  // 0
    {
        // TYPE_AC 일 때 SwPin-> HwPin
        nHwLine = (MAX_LINE_COL -1) - nSwCol;
        nHwCol  = nSwLine;
    }

    // Upper_B, Upper_D, Lower_B, Lower_D의 핀배치가 동일하므로 TYPE_BD로 설정
    else        // TYPE_BD : 1
    {
        // TYPE_BD 일 때 SwPin-> HwPin
        nHwLine = nSwCol;
        nHwCol  = (MAX_LINE_COL -1) - nSwLine;
    }

    nHwBoxPin = (nHwLine * MAX_LINE_COL + nHwCol);      // Box내 HwPin 번호
    nHwPin = nHwBoxPin + (nHwPartBox * MAX_BOX_PIN)     // BoxPin + Box Offset 
                        + (nPart * MAX_PART_PIN);       // + HW PartOffset (upper size)

    return nHwPin; 

}


//son201218 QDType과 하부시작핀정보로 QD 기준의 UpperPartSize 결정.
int CModeSet12::getQD_UpperPartSize()
{
    UINT nQD_UpperPartSize;

    //son 블록별 QD/DD/SD 기능 사용시에는   QD 하부시작핀 16385로 고정
    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {
        nQD_UpperPartSize = (4*MAX_BOX_PIN);        
        return nQD_UpperPartSize;
    } //son210514

    //son 맨첫번째 boxType은 SKIP 일 수 없다.
    ASSERT(SysSet12.m_nPinQDUpper!= BOX_SKIP); //son241007-BSW-9

    switch (SysSet12.m_nPinQDUpper)
    {
        case BOX_SD:
            // SD인데 UpperPartSize 2048이면 상부 1024씩 2box이므로 
            // 하부시작핀이 QD 기준으로는 8193이 되어야 함. 즉 UpperPartSize를 *4해서 8192로 조정한다.
            //  SD UpperSize -> QD UpperSize
            //  1024            4096
            //  2048            8192
            //  4096            16384
            nQD_UpperPartSize = (SysSet12.m_nLowerJigStartPin -1) * 4;
            break;

        case BOX_DD:
            //  DD UpperSize -> QD UpperSize
            //  2048            4096
            //  4096            8192
            //  8193            16384
            nQD_UpperPartSize = (SysSet12.m_nLowerJigStartPin -1) * 2 ; 
            break;                      


        case BOX_QD: 
        default:   
            // 상단 Sw Pin 갯수 == 상단 SW 파트의 사이즈
            // 일반적으로는 MAX_PART_PIN과 nQD_UpperPartSize 값이 16K로 같지만
            // 만약 nQD_UpperPartSize만 8K로  달라진다면, 상부 HW는 16K인데 상부 SW pin은 8K 까지만 사용한다는 뜻임.
            nQD_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;     
            break;

    }

    if (nQD_UpperPartSize > MAX_PART_PIN)   //son201218 16384 이상이면 16384로 default처리 
        nQD_UpperPartSize = (4*MAX_BOX_PIN);        
        //return -1;

    //-------------------------------------------------------------------
    //son 상부 3박스, 하부시작핀이 12289 base인 경우 (상부 4th skip 아님) 
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS      //son210531 U 12 + L 4 = 16, skip 없음
            || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS) //son230714 U 12 + L 8 = 20, skip 없음.
    {
        //son230719 PIN_TOT_16K_U12K_L4K_UP_TRANS 설정하고 save시에 디버그모드에서 에러 많이 나오는 문제 관련 수정.
        ////son201218 Debug 모드용 err check                                                          
        //ASSERT(nQD_UpperPartSize == (3 * MAX_BOX_PIN)           // 12288
        //    || nQD_UpperPartSize == (3 * MAX_BOX_PIN/2)         // 6144
        //    || nQD_UpperPartSize == (3 * MAX_BOX_PIN/4));       // 3072

        //son201218 Release 모드용 err 대책: 하부 시작핀이 이전에 설정된 적이 없다면 12288을 default로 설정한다.
        if (nQD_UpperPartSize != (3 * MAX_BOX_PIN)                  // 12288도 아니고
                    && nQD_UpperPartSize != (3 * MAX_BOX_PIN/2)     // 6144도 아니고
                    && nQD_UpperPartSize != (3 * MAX_BOX_PIN/4))    // 3072도 아니면
            nQD_UpperPartSize = (3 * MAX_BOX_PIN);      
    }

    //---------------------
    //son 보통 케이스
    else
    {
        ASSERT(nQD_UpperPartSize == (4*MAX_BOX_PIN)         // 16384
            || nQD_UpperPartSize == (2*MAX_BOX_PIN)         // 8192
            || nQD_UpperPartSize == MAX_BOX_PIN);           // 4096
        //  || nQD_UpperPartSize == (MAX_BOX_PIN/2)         // 2048
        //  || nQD_UpperPartSize == (MAX_BOX_PIN/4)         // 1024

        //son201218 Release 모드용 err 대책: 하부 시작핀이 이전에 설정된 적이 없다면 16384를 default로 설정한다.
        if (nQD_UpperPartSize != (4*MAX_BOX_PIN)            // 16384도 아니고
                    && nQD_UpperPartSize != (2*MAX_BOX_PIN)     // 8192도 아니고
                    && nQD_UpperPartSize != MAX_BOX_PIN)        // 4096도 아니면
                //  && nQD_UpperPartSize != (MAX_BOX_PIN/2)     // 2048도 아니면
                //  && nQD_UpperPartSize != (MAX_BOX_PIN/4) // 1024도 아니면
            nQD_UpperPartSize = (4*MAX_BOX_PIN);        
    }

     
    return nQD_UpperPartSize;
}

// JYK 20200117
// QD, SD, DD swPin을  QD 기준으로  변환해서 리턴한다.  
// parameter  : nSwPin      0으로 시작하는 SwPin(JigPin)
// return val : nQdSwPin    QD SwPin (QD, DD, SD -> QD) 으로 변환해서 리턴  
//                         현재 QDType이 QD이면 원래 swPin 값  그대로  리턴된다.
//
//  QD  :   63  62  61  60  ...     13  12  11  10  9   8   7   6   5   4   3   2   1   0   
//          127 126 125 124 ...     77  76  75  74  73  72  71  70  69  68  67  66  65  64
//          191 190 189 188 ...     141 140 139 138 137 136 135 134 133 132 131 130 129 128
//
//  DD  :   -   31  -   30  ...     -   6   -   5   -   4   -   3   -   2   -   1   -   0
//          63  -   62  -   ...     38  -   37  -   36  -   35  -   34  -   33  -   32  -
//          -   95  -   94  ...     -   70  -   69  -   68  -   67  -   66  -   65  -   64
//
//  SD  :   -   31  -   30  ...     -   6   -   5   -   4   -   3   -   2   -   1   -   0
//          -   -   -   -   ...     -   -   -   -   -   -   -   -   -   -   -   -   -   -
//          -   63  -   62  ...     -   38  -   37  -   36  -   35  -   34  -   33  -   32

// 하부시작핀값이 Sw dependent해서 DD, SD로 줄인값으로 입력받은 경우라면 QD  기준으로 늘려서 
// 쓰기위해 getQdSwPin()을 사용한다. 내부적으로 getQD_UpperPartSize()를 사용함.
// 20200615: 하부시작핀을 줄이는 케이스를 상부 box 중 일부를 사용 안하는 경우로 한정해서 생각한 코드임.
//           실제로는 box 와 상관 없이 QD 로 만든 BDL을 DD에서 사용한다던지의 경우로 하부시작핀을 이용하게 되므로
//           box range와 상관없이 하부시작핀을 설정할 수 있는게 맞다.  
//           추후 getQdSwPin_Fixed()를 이용하도록 수정이 필요. getQdSwPin_Fixed는 HW 하부시작핀은 16385로 고정한다.
//           실제로 HW 하부시작핀은 16385 고정이 맞다. 4K, 8K, 16K, 32K 상관없이 모두 16384 +1임
int CModeSet12::getQdSwPin(UINT nSwPin)
{
    int nQdSwPin = 0;

    // 기본 range check  
    if (nSwPin < 0 || nSwPin >= MAX_PIN) 
        return -1;


    //son QD/DD/SD Block Set일 때는 모두 Box 0인 것으로 계산됨. 여기서 별도처리하고 리턴
    if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
    {
        return getQdSwPin_BlockSet1(nSwPin);
    }


    // 상단 Sw Pin 갯수
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1; 


    //son SD, DD면  jigPin 기준으로 줄여서 하부시작핀을 입력받은 케이스이므로  QD 기준으로 다시 늘려줌
    UINT nQD_UpperPartSize = getQD_UpperPartSize(); 


    // Line(Card), Column(CardPin) 모두 0부터 시작. 
    
    
    // 상부 
    //if(nSwPin < nQD_UpperPartSize)
    if(nSwPin < nOrg_UpperPartSize)     //son201218  상부DD, 하부 QD 시 hwPin 계산오류문제 수정.
    {
        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, 0, 0);        //son201218

        if (SysSet12.m_nPinQDUpper == BOX_DD)   
            return getQdPin_fromDdPin(nSwPin, 0, 0);        //son201218

        //son201218  QD라면 계산없이 원래의 nSwPin을 리턴한다 .

    }

    // 하부 
    else
    {
        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);       //son201218

        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getQdPin_fromDdPin(nSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);       //son201218

        // QD인 경우 
        nSwPin = (nSwPin - nOrg_UpperPartSize) + nQD_UpperPartSize;         //son201218

    }

    return nSwPin;
}


//son QD/DD/SD Block Set일 때는 모두 Box 0인 것으로 계산됨.
int CModeSet12::getQdSwPin_BlockSet1(UINT nSwPin) //son241007-BSW-9
{
    //son QD/DD/SD Block Set일 때는 모두 Box 0인 것으로 계산됨. 여기서 별도처리하고 리턴
    if( SysSet12.m_nBlockSet1 != 1 ) //sylee210402 
        return -1;

    if(sPinBlockRange[m_nBlockPo1].nBlockType== BOX_SD) //tran  //sylee210402
        return getQdPin_fromSdPin(nSwPin, 0, 0); 

    if(sPinBlockRange[m_nBlockPo1].nBlockType== BOX_DD) 
        return getQdPin_fromDdPin(nSwPin, 0, 0);   
    
    return nSwPin;  // BOX_QD
    
}

// 하부시작핀 Offest을  QD, DD, SD  상관없이 사용.  
// 즉, 하부시작핀이 DD, SD 비율로 줄어들지 않는 경우
// getQdSwPin_Fixed()을 사용한다. (하부시작핀 -1) 값을 그냥 UpperPartSize로 사용함. 
int CModeSet12::getQdSwPin_Fixed(UINT nSwPin)
{
    int nQdSwPin = 0;


    // 기본 range check  
    if (nSwPin < 0 || nSwPin >= MAX_PIN) 
       return -1;

    // 상단 Sw Pin 갯수
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;   


    // 16K든, 32K든  Hw 하부 Offset은 MAX_PART_PIN(16384) 고정
    UINT nHW_UpperPartSize = MAX_PART_PIN;  //son Fixed!!!


    // Line(Card), Column(CardPin) 모두 0부터 시작.
    
    // 상부 
    if(nSwPin < nOrg_UpperPartSize)
    {
        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, 0, 0);        //son201218

        if (SysSet12.m_nPinQDUpper == BOX_DD)   
            return getQdPin_fromDdPin(nSwPin, 0, 0);        //son201218

        //  QD라면 계산없이 원래 nSwPin을 리턴한다 .
        nQdSwPin = nSwPin;
    }

    // 하부 
    else
    {
        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, nOrg_UpperPartSize, nHW_UpperPartSize);        //son201218

        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getQdPin_fromDdPin(nSwPin, nOrg_UpperPartSize, nHW_UpperPartSize);        //son201218

        // QD인 경우 
        nQdSwPin = (nSwPin - nOrg_UpperPartSize) + nHW_UpperPartSize;  //HW 하부 Offset은 16384 또는 8192

    }

    return nQdSwPin;

}



//son201218: SD -> QD Pin으로 확대변환
int CModeSet12::getQdPin_fromSdPin(int nSwPin, int nOffset, int nQdOffset)   //son190902 32KPIN 
{
    int nBoardPinCnt, nBoardNo, nPinNo;
    int nQdSwPin = 0;

    nBoardPinCnt = MAX_SD_DD_CARD_PIN;                  // SD BoardPinCnt : 32
    nBoardNo = (nSwPin - nOffset) / nBoardPinCnt;       // SD BoardNO
    nPinNo = (nSwPin - nOffset) % nBoardPinCnt;         // SD PinNo

    // SD는 홀수 Line(Board or Card)는 건너 뛰고 짝수 라인만(MAX_BOX_LINE *2) 사용하고
    // 짝수 라인안에서도 짝수 칸만 사용된다. (nPinNo * 2) 
    nQdSwPin = nQdOffset + nBoardNo * MAX_BOX_LINE *2 + nPinNo * 2; // Hw 하부 Offset은 16384(MAX_PART_PIN) 고정.
    return nQdSwPin;

}

//son201218: DD -> QD Pin으로 확대변환
int CModeSet12::getQdPin_fromDdPin(int nSwPin, int nOffset, int nQdOffset)   //son190902 32KPIN 
{
    int nBoardPinCnt, nBoardNo, nPinNo;
    int nQdSwPin = 0;

    nBoardPinCnt = MAX_SD_DD_CARD_PIN;              // DD BoardPinCnt : 32
    nBoardNo = (nSwPin - nOffset) / nBoardPinCnt;   // DD BoardNO               
    nPinNo   = (nSwPin - nOffset) % nBoardPinCnt;   // DD PinNo                 

    // DD 짝수 라인이면 홀수 칸은 건너뛰고 짝수 칸만 사용된다. (nPinNo * 2) 
    nQdSwPin = nQdOffset + nBoardNo * MAX_BOX_LINE + nPinNo * 2;    

    // DD는 홀수 라인이면 짝수칸을 건너뛰고 홀수 칸만 사용된다. (nPinNo * 2 +1)
    if(nBoardNo % 2 == 1)
        nQdSwPin++;
    return nQdSwPin;
}



//-----------------------------------------------
//son nHwPin 값에 맞는 SwPin(JigPin)을 리턴한다. 
//-----------------------------------------------
// parameter  : nHwPin  Input 정보, 0으로 시작하는 HwPin(SwitchCard pin)
// return val : nSwPin  nHwpin에 맞는 SW Jig Pin
int CModeSet12::make32kSwPin(UINT nHwPin)
{
    UINT nPart, nHwBox, nHwPartBox, nHwBoxType;
    UINT nHwBoxPin, nHwLine, nHwCol;
    UINT nSwBoxPin, nSwLine, nSwCol;
    UINT nQdSwPin;      


    //son-8K4097-20200605: SWMaxPin은 8K구형, HW는 16K, QDHW 고정 하부시작핀 사용시에 적용.
    if(SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN   //son SW MaxPin은 8K, HW는 16K 케이스
         || (SysSet12.m_nPinTotMax == PIN_TOT_12K)            //son210202: 12K 구형 추가
         || (SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY) //son211020 8K구형 상부만 존재하는 케이스 (f550j 나노시스) 
         || (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230221
         || (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG ))   //son230316 
    {

        //son 8K는 구형이므로 HwPin-> SwPin변환이 없다.
        //son 하부시작핀이 이미 QD 기준으로 들어온 경우의 QdSwPin 값을 리턴
        //son UpperPartSize를 QD, DD, SD에 따른 변동형이 아니라 HW 기준으로 (16384:32K 또는 8192:QD)으로  처리한다.
        return getDdSdSwPin_Fixed(nHwPin);  
    }

    //---------------------------
    // HwPin 정보 분석 
    
    // 32K HW인데 32K이상이거나, 0보다 작은 hwPin입력은 -1로 처리한다.
    if (nHwPin < 0 || nHwPin > MAX_PIN)  
        return -1;


    // Part(Upper or Lower) > Box (A~D) > Line (0~63) > Column (0~63)
    nPart  = nHwPin / MAX_PART_PIN;
    nHwBox = nHwPin / MAX_BOX_PIN;       // 나누기 4096
    if (nHwBox < 0 || nHwBox >= MAX_BOX) // 해당 HW 박스가  미사용 영역이면 swPin으로  -1을 리턴
        return -1;
 

    nHwPartBox = nHwBox % MAX_BOX_PER_PART; // Hw Part별 Box 번호로 변환.
    nHwBoxType = nHwPartBox % 2;            // 짝수는 TYPE_AC, 홀수는 TYPE_BD

    nHwBoxPin  = nHwPin % MAX_BOX_PIN;      // Box내 HwPin 번호
    nHwLine    = nHwBoxPin / MAX_BOX_LINE;
    nHwCol     = nHwBoxPin % MAX_BOX_LINE;

    // QDType과 하부Jig StartPin 정보로 Jig(SW) Part 정보를 확인한다.
    UINT nSw_UpperPartSize = getQD_UpperPartSize();



    // nSw_UpperPartSize는 MAX_BOX_PIN:4096의 배수임 
    UINT nSwUpperBoxNum = nSw_UpperPartSize / MAX_BOX_PIN;  // nSw_UpperPartSize    nSwUpperBoxNum
                                                            //  16384(16K)          4
                                                            //  12288(12K)          3
                                                            //  8192 (8K)           2
                                                            //  4096 (4K)           1

    // 하부시작핀 관련하여 상부 box의 뒷부분 미사용영역에 해당하는 pin은 swPin으로 -1을 리턴한다.
    if (SysSet12.m_nBlockSet1 != 1)
        if (nPart == PART_UPPER && nHwPartBox >= nSwUpperBoxNum)
            return -1;

    // 계산을 위한 PinBlock번호 지정, nHwPin으로 계산할 수 있다.
    m_nBlockPo1 = nHwPin / MAX_BOX_PIN +1;   //son m_nBlockPo1은 1부터 시작하므로 +1    //son210514

    //son 하부시작핀을 안 쓰고 블록별 QD/DD/SD를 따를 경우 Block1 케이스로 바꿔서 계산하고 
    //    나중에 현블록의 StartPin을 더해서 계산해야 한다.
    //son20514  QD/DD/SD BlockSet  상태라면 하부박스인지 여부를  m_nBlockPo1로 판단한다.
    int nHwPartBox_forCheck = nHwPartBox;       //son210514
    if (SysSet12.m_nBlockSet1 == 1)             //son210514
        nHwPartBox_forCheck = m_nBlockPo1 -5;


    //-----------------------------------------
    //for 24K (12 Upper Trans + 8K Lower Trans)  
    //-----------------------------------------
    //son Upper 4번째 Box(nHwParBox 3)가 없다.
    if ((SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)    //son230718
            && nPart == PART_UPPER )    
    {
        if (nHwPartBox == 3)    //son230718
            return -1;
    }

    //--------------------------------------
    //for 20K (Upper 16K + Lower 4K) base               //son-32KPIN-20200103 
    //for 24K (Upper 16K + Lower 8K) base               //son-32KPIN-20200121 
    //--------------------------------------
    // 하부핀인데 20K, 24K 인 경우는 swPin 배치순서와  hwPin배치 순서를 동일하게 설정해서 리턴한다.
    // 하부핀인데 32K or 24K_FULL_TRANS이거나,  상부핀인  경우는 이 코드 다음으로  진행.
    //if    (SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR) //son230216_1
    if(IS_UPPER_TRANS(SysSet12.m_nPinTotMax))  //son210531
    {
        //son 상부 12K Trans , 하부 4K Origin인 경우는 Upper 4번째 Box(nHwParBox 3)가 없다.
        if ((SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS 
                        || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS)    //son210603
                && nPart == PART_UPPER )    //son210531  
        {
            if (nHwPartBox == 3)    //son210531
                return -1;
        }

        //son 상부 8K Trans , 하부 4K Origin인 경우는 Upper 3,4번째 Box(nHwParBox 2,3)가 없다.
        if (SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS  && nPart == PART_UPPER )    //son210826  
        {
            if (nHwPartBox == 2 || nHwPartBox == 3)    //son210826
                return -1;
        }

        //son 하부 박스라면
        //son20514  QD/DD/SD BlockSet  상태라면 하부박스인지 여부를  m_nBlockPo1로 판단한다.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                //son210514
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )    //son210514 
        {
            //son UpperTrans이고 하부에 box가 1개밖에 없는 경우  BoxNum은 1개를 넘을 수 없다.
            //    IS_UPPER_TRANS(SysSet12.m_nPinTotMax)인지 이미 위에서 체크함.
            if (IS_LOWER_4K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 1) //son210826 
                return -1; // swPin으로 -1을 리턴

            //son UpperTrans이고 하부에 box가 2개밖에 없는 경우  Hw BoxNum은 2개를 넘을 수 없다.
            if (IS_LOWER_8K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 2) //son210826 
                return -1; // swPin으로 -1을 리턴

            //--------------------------------
            // 20K, 24K Lower SwPin 정보 계산 
            //--------------------------------
            // 20K, Lower Part는 rnSwBoxPin 대신 rnHwBox으로 rnSwPin을 계산해서 여기서 리턴한다.
            nQdSwPin  = nHwBoxPin + (nHwPartBox_forCheck * MAX_BOX_PIN) // BoxPin + Box Offset  //son210514 
                + (nPart * nSw_UpperPartSize);  // + Part Offset

            // QD 기준 swPin을 QdType(QD, DD, SD)에 맞게 변환해서 리턴한다.  JYK 20200117
            return getDdSdSwPin(nQdSwPin);  // QD -> QD, DD, SD
        }
    }

    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {

        //son nHwPin을 Block 1기준으로 수정한다.
        nHwPin = nHwPin - (MAX_BOX_PIN * (m_nBlockPo1 -1));

        // Part(Upper or Lower) > Box (A~D) > Line (0~63) > Column (0~63)
        nPart  = nHwPin / MAX_PART_PIN;
        nHwBox = nHwPin / MAX_BOX_PIN;       // 나누기 4096

        nHwPartBox = nHwBox % MAX_BOX_PER_PART; // Hw Part별 Box 번호로 변환.

        nHwBoxType = sPinBlockRange[m_nBlockPo1].nHwBoxType;    //son210514 ****


        nHwBoxPin  = nHwPin % MAX_BOX_PIN;      // Box내 HwPin 번호
        nHwLine    = nHwBoxPin / MAX_BOX_LINE;
        nHwCol     = nHwBoxPin % MAX_BOX_LINE;
    } //son210514


    //----------------------------------------------
    //for 24K_FULL_TRANS (Upper 16K + Lower 8K) base                
    //----------------------------------------------
    //son-32KPIN-20200103 
    // 하부핀인데 24K_FULL_TRANS 인 경우는 하부에 box가 2개밖에 없으므로 하부 Hw BoxNum은 2개를 넘을 수 없다.
    if( SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son 하부 박스라면
        //son20514  QD/DD/SD BlockSet  상태라면 하부박스인지 여부를  m_nBlockPo1로 판단한다.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                //son210514
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )    //son210514 
        {
            if (nHwPartBox_forCheck >= 2) //son210514
                return -1;

            //24K_FULL_TRANS의 Pin 배치는 32K와 동일하므로 box range만 OK라면 32K와 동일하게 pin 값을 리턴한다.
        }
    }

    //----------------------------------------------
    //for 16K_FULL_TRANS (Upper 8K + Lower 8K) base                 
    //for 12K_FULL_TRANS (Upper 8K + Lower 4K) base                 
    //----------------------------------------------
    //son-32KPIN-20200228 
    // 16K_FULL_TRANS 인 경우는 상부, 하부 모두  box가 2개. 하부 Hw BoxNum은 2개를 넘을 수 없다.
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS //son230216_1
            || SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS) //son230216_1
    {
        if (nHwPartBox_forCheck >= 2)
            return -1;

        //24K_FULL_TRANS의 Pin 배치는 32K와 동일하므로 box range만 OK라면 32K와 동일하게 pin 값을 리턴한다.
    }
/*
    //----------------------------------------------
    //for 12K_FULL_TRANS (Upper 8K + Lower 4K) base                 
    //----------------------------------------------
    // 12K_FULL_TRANS 인 경우는 상부 box가 2개, 하부 1개. 
    if (SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS)    //son210319 //son230216_1
    {
        if (nPart == PART_UPPER && nHwPartBox_forCheck >= 2)    //son 상부box 2개 //son210514
            return -1;

        if (nPart == PART_LOWER && nHwPartBox_forCheck >= 1)    //son 하부box 1개 //son210514
            return -1;
    }
*/

    //----------------------------------------------
    //for 8K_FULL_TRANS (Upper 4K + Lower 4K) base              
    //----------------------------------------------
    if (SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS) //son210319 //son230216_1
    {
        // 8K_FULL_TRANS 인 경우는 상부, 하부 모두  box가 1개. Hw BoxNum은 1개를 넘을 수 없다.
        if (nHwPartBox_forCheck >= 1) //son210514
            return -1;

        //8K_FULL_TRANS의 Pin 배치는 32K와 동일하므로 box range만 OK라면 32K와 동일하게 pin 값을 리턴한다.
    }


    //--------------------------
    // 변형 SwPin 정보 계산 
    //--------------------------
    // 대상: 32K Upper & Lower, 
    //       24K_FULL Upper & Lower, 
    //       20K Upper,
    //       24K Upper  
    
    // HW Pin을 왼쪽 상단으로 고정시키고 4096 배수의 offset을 제외하면
    // Upper_A, Upper_C, Lower_A, Lower_C의 핀배치가 동일하므로 TYPE_AC로 설정.
    if (nHwBoxType == TYPE_AC)  // 0
    {
        nSwLine = nHwCol;
        nSwCol  = (MAX_LINE_COL -1) - nHwLine;
    }

    // Upper_B, Upper_D, Lower_B, Lower_D의 핀배치가 동일하므로 TYPE_BD로 설정
    else        // TYPE_BD  : 1
    {
        nSwLine = (MAX_LINE_COL -1) - nHwCol;
        nSwCol  = nHwLine;
    }
    nSwBoxPin = (nSwLine * MAX_LINE_COL + nSwCol);          // Box내 SwPin 번호
    nQdSwPin    = nSwBoxPin + (nHwPartBox * MAX_BOX_PIN)    // BoxPin + Box Offset 
                          + (nPart * nSw_UpperPartSize);    // + Part Offset


    //son 하부시작핀을 안 쓰고 블록별 QD/DD/SD를 따를 경우 
    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {
        //son 변형 SwPin을 Block1기준으로 계산한 결과에 현재 블록의 Start번호를 더해서 블록 위치를 복구해 리턴한다.
        int DdSdSwPin = getDdSdSwPin(nQdSwPin) + (sPinBlockRange[m_nBlockPo1].nStartPin-1);
        return DdSdSwPin; 
    } //son210514

    // QD 기준 swPin을 QdType(QD, DD, SD)에 맞게 변환해서 리턴한다.  JYK 20200117
    return getDdSdSwPin(nQdSwPin);  // QD -> QD, DD, SD
}


// QD 기준 swPin을 QdType(QD, DD, SD)에 맞게 변환해서 리턴한다.  JYK 20200117
// parameter  : nQdSwPin      0으로 시작하는 SwPin(JigPin), QD 기준
// return val : nDdSdSwPin    현재 QDType에 맞는 SwPin으로 (QD -> QD, DD, SD) 변환해서 리턴 
//                            현재 QDType이 QD이면 원래 swPin이 그대로  리턴된다.

//  QD  :   63   62   61    60   ...    13   12   11   10   9    8    7    6    5    4    3    2    1    0  
//          127  126  125   124  ...    77   76   75   74   73   72   71   70   69   68   67   66   65   64
//          191  190  189   188  ...    141  140  139  138  137  136  135  134  133  132  131  130  129  128
//                               ...
//          4095 4094  4093 4092 ...    4045 4044 4043 4042 4041 4040 4039 4038 4037 4036 4035 4034 4033 4032
//
//  DD  :   -    31   -     30   ...    -    6    -    5    -    4    -    3    -    2    -    1    -    0
//          63   -    62    -    ...    38   -    37   -    36   -    35    -   34   -    33   -    32   -
//          -    95   -     94   ...    -    70   -    69   -    68   -    67   -    66   -    65   -    64
//                               ...
//          2047 -    2046  -    ...    2022 -    2021 -    2020 -    2019 -    2018 -    2017 -    2016 -
//
//  SD  :   -    31   -     30   ...    -    6    -    5    -    4    -    3    -    2    -    1    -    0
//          -    -    -     -    ...    -    -    -    -    -    -    -    -    -    -    -    -    -    -
//          -    63   -     62   ...    -    38   -    37   -    36   -    35   -    34   -    33   -    32
//                               ...
//          -    1023 -    1022  ...    -    998  -    997  -    996  -    995  -    994  -    993  -    992 

int CModeSet12::getDdSdSwPin(UINT nQdSwPin)
{

    int nDdSdSwPin = 0;

    // 기본 range check  
    if (nQdSwPin < 0 || nQdSwPin >= MAX_PIN) 
        return -1;


    //son 블록별 QD/DD/SD 사용시는 이렇게 리턴한다.
    //    블록별 QD/DD/SD 설정시에는 하부시작핀을 사용하지 않음.
    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {
        switch(sPinBlockRange[m_nBlockPo1].nBlockType)
        {
            case BOX_SD:
                return getSdPin_fromQdPin(nQdSwPin, 0, 0);
                break;

            case BOX_DD:
                return getDdPin_fromQdPin(nQdSwPin, 0, 0);
                break;

            case BOX_QD:
            default:
                //  QD라면 계산없이 원래 nQdSwPin을 리턴한다 .
                return nQdSwPin;
                break;
        }
    } //son210514


    // 상단 Sw Pin 갯수
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;
    UINT nQD_UpperPartSize  = getQD_UpperPartSize();




    // 상부 
    if(nQdSwPin < nQD_UpperPartSize)
    {
        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getSdPin_fromQdPin(nQdSwPin, 0, 0);      //son201218

        if (SysSet12.m_nPinQDUpper == BOX_DD)
            return getDdPin_fromQdPin(nQdSwPin, 0, 0);      //son201218

        //  QD라면 계산없이 원래 nQdSwPin을 리턴한다 .
    }

    // 하부 
    else
    {

        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getSdPin_fromQdPin(nQdSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);     //son201218

        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getDdPin_fromQdPin(nQdSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);     //son201218

        // QD인 경우 
        nQdSwPin = (nQdSwPin - nQD_UpperPartSize) + nOrg_UpperPartSize;     //son201218

    }


     return nQdSwPin;

}


//son HW UpperPartSize를 QD, DD, SD에 따른 변동형이 아니라 HW 기준으로 16384 고정으로  처리한다.
int CModeSet12::getDdSdSwPin_Fixed(UINT nHwPin)
{


    int nDdSdSwPin = 0;

    
    // 기본 range check  
    if (nHwPin < 0 || nHwPin >= MAX_PIN) 
        return -1;

    // 상단 Sw Pin 갯수
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;


    // 16K든, 32K든  QD 고정 하부 시작핀이면 Hw 하부 Offset은 16384 고정.  (동일 HW Switch)
    UINT nHW_UpperPartSize = (MAX_BOX_PIN *4);  // 16384

//son230221 begin:
    //son K600 상부 3, 4 box가  실제 하부에서 사용되는 경우 (BDL 하부)
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG 
            && SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)
    {
        //son box 3-> sw box 9로  box 4-> sw box 10으로 바꿔준다.
        //    box 9는 하부 5번째 box, box 10은 하부 6번째 box임
        if (nHwPin >= 8192 && nHwPin <= 16383)
            nHwPin = nHwPin + (MAX_BOX_PIN * 6); 
    }
//son230221 end

    // 상부 
    if(nHwPin < nHW_UpperPartSize) 
    {

        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getSdPin_fromQdPin(nHwPin, 0, 0);      //son201218


        if (SysSet12.m_nPinQDUpper == BOX_DD)
            return getDdPin_fromQdPin(nHwPin, 0, 0);      //son201218

        //  QD 상부라면 계산없이 원래 nHwPin을 리턴한다 .
        nDdSdSwPin = nHwPin;
    }

    // 하부 
    else
    {

        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getSdPin_fromQdPin(nHwPin, nOrg_UpperPartSize, nHW_UpperPartSize);      //son201218


        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getDdPin_fromQdPin(nHwPin, nOrg_UpperPartSize, nHW_UpperPartSize);      //son201218


        // QD인 경우 
        nDdSdSwPin = (nHwPin - nHW_UpperPartSize) + nOrg_UpperPartSize;

    }

    return nDdSdSwPin;


}


//son201218 QD -> SD Pin으로 축소변환
int CModeSet12::getSdPin_fromQdPin(UINT nQdSwPin, int nOffset, int nQdOffset)
{
    int nQdBoardPinCnt, nQdBoardNo, nQdPinNo;
    int nDdSdSwPin = 0;

    nQdBoardPinCnt = MAX_QD_CARD_PIN;   // 64
    nQdBoardNo = (nQdSwPin - nQdOffset) / nQdBoardPinCnt;
    nQdPinNo = (nQdSwPin - nQdOffset) % nQdBoardPinCnt;

    // SD 홀수 라인은 통으로 비워 둔다.
    if((nQdBoardNo % 2) != 0)
        return -1;

    // SD 짝수 라인이면서 홀수 핀은 비워 둔다.
    if((nQdPinNo % 2) != 0)
        return -1;


    // SD->QD 시에 짝수 라인만 (MAX_BOX_LINE *2해서), 짝수칸만 (nPinNo *2)  할당했으므로 
    // QD->SD 시에는 /2 한 값을 Board 값으로 사용하고 nQdPinNo/2 값을 pinNo로 사용한다.  
    // MAX_SD_DD_CARD_PIN: 32
    nDdSdSwPin = nOffset + (nQdBoardNo /2) * MAX_SD_DD_CARD_PIN + nQdPinNo / 2;
    return nDdSdSwPin;
}

//son201218 QD -> DD Pin으로 축소변환
int CModeSet12::getDdPin_fromQdPin(UINT nQdSwPin, int nOffset, int nQdOffset)
{
    int nQdBoardPinCnt, nQdBoardNo, nQdPinNo;
    int nDdSdSwPin = 0;

    nQdBoardPinCnt = MAX_QD_CARD_PIN;   // 64
    nQdBoardNo = (nQdSwPin - nQdOffset) / nQdBoardPinCnt;
    nQdPinNo = (nQdSwPin - nQdOffset) % nQdBoardPinCnt;

    // 짝수 라인
    if((nQdBoardNo % 2) == 0)
    {
        // DD 짝수라인은 홀수 pin을 비워 둔다.
        if((nQdPinNo % 2) != 0)
            return -1;

        // MAX_SD_DD_CARD_PIN: 32
        nDdSdSwPin = nOffset + nQdBoardNo * MAX_SD_DD_CARD_PIN + nQdPinNo / 2;
    }

    // 홀수 라인
    else
    {
        // DD 홀수라인은 짝수 pin을 비워 둔다.
        if((nQdPinNo % 2) != 1)
            return -1;

        // MAX_SD_DD_CARD_PIN: 32
        nDdSdSwPin = nOffset + nQdBoardNo * MAX_SD_DD_CARD_PIN + (nQdPinNo - 1) / 2;
    }
    return nDdSdSwPin;
}

int CModeSet12::OnPinConvert1_YAMAHA()    //ACE400  //SYLEE160907-1
{


    int i,Mok1,Mok2,Namo1,Namo2,Namo3,nMaxPin1,nTmp1;

    //son sPinBlockRange[10]를  생성하고 "d:\\PinMap_PinBlockRange1.ini" 파일로 출력한다
    MakePinBlockRangeUserNo();//sylee210402 

    nMaxPin1=4096*8;
 

///////////////////////////////////////
//test
    memset(PinMap, -1, sizeof(PinMap));//sylee151006
    memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303
 
    for( i=0; i<nMaxPin1; i++)//SYLEE121003
    {
    //  if( (i>=2048) && (i<4096)){
    //     continue;
    //  }
        if( i>=4096){//YAMAHA
          continue;
        }


        PinMap[0][i]=i;//****>net list pin no   
        PinMap[1][i]=i;//****>net list pin no   
//      PinMap[2][i]=i;//****>net list pin no   

        Mok1=i/128;
        Namo1=i%128;
        Mok2=Namo1/4;
        Namo2=Namo1%4;

        if(Namo2==0){
            Namo3=63;//64
        }else if(Namo2==1){
            Namo3=127;//128
        }else if(Namo2==2){
            Namo3=62;//63
        }else if(Namo2==3){
            Namo3=126;//127
        } 


        nTmp1= Mok1*128  -(Mok2*2) + Namo3 ;  //user pin no. list  0~

        if(nTmp1>=2048){
            PinMap[2][i]= nTmp1 +(4096*3+2048);
        }else{
            PinMap[2][i]= nTmp1;
        }

        //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
        int hwPin = PinMap[2][i];				//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
            nJigPinMap[hwPin] = i;    //son 빠른 검색용. //son230303
  
     }

 

    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  //son210514  200 -> FIlE_PATH_SIZE
    CString str; 

    ::ZeroMemory(&fName, sizeof(fName));
    //strcat( fName , "d:\\PinMap.txt" );
    strcat( fName , "d:\\log\\PinMap.txt" ); //son220715 d:\\log 폴더로 이동

          
    fp = fopen(fName,"wt");
    if(fp == NULL) return 0;

    fprintf(fp, " \n     ==> * YAMAHA Type Pin Block  \n ");
    fprintf(fp, " \n     ==> Upper Pin =  1~ 1536 \n ");
    fprintf(fp, "    ==> Lower Pin =  2049~ 3584 \n\n");
    fprintf(fp, "   *User Pin* / *Switch card Pin* \n");

    for( i=0; i<nMaxPin1; i++){//SYLEE121003 
    //SYLEE170615-3 if( i>=4096){
    //SYLEE170615-3      continue;
    //SYLEE170615-3 }
        fprintf(fp, "%10d, %10d\n",i+1,PinMap[2][i]+1); 
    }

    fclose(fp);  

    MyTrace(PRT_BASIC, "OnPinConvert1_YAMAHA(): %s PinMap Created.\n", fName);        //son220715
 
    return 1;
 
}
 


int CModeSet12::OnPinConvert1_2RD() //SYLEE140430 R&D MANUAL MACHINE SWITCH CARD JIG
{


    CString str; 
    int  nLimitNo;//sylee131211
    int No,Mok1,Namo,BoxMok2,i,nMaxPin1,nMaxPinHalf1;

    nLimitNo=1024*6;//sylee131211


    if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k  //SYLEE121003
        nMaxPin1=4096;      
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
        nMaxPin1=4096*2;
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_16K){//16k
        nMaxPin1=4096*4;
    }
    else  if(SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS){//32k //son230216_1
        nMaxPin1=4096*8;
    }
    else {//32k
        nMaxPin1=4096*8;
    }
    nMaxPinHalf1=nMaxPin1/2;


    memset(PinMap, 0x00, sizeof(PinMap));  // [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN, 
    memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303



    for (i=0; i< nMaxPin1; i++)//SYLEE121003
    {


        No=i;

        PinMap[0][i]=i;//****>net list pin no



        /////////////////////////////////////////////////////////////////////////////////////////
        if(SysSet12.m_nPinBoxMax==1)  //2k//sylee120925
        {


            //if((SysSet12.m_nPinQD!=3)&&(SysSet12.m_nPinQD2==3)){//UPPER=DD, LOWER= SD  //SYLEE121004 
            if((SysSet12.m_nPinQDUpper!=BOX_SD)&&(SysSet12.m_nPinQDLower==BOX_SD))  //son190902 32KPIN:  enum 변경 
                                                                                    //       m_nPinQD-> m_nPinQDUpper로 변경
            {
                if(i<nMaxPinHalf1){


                }
                else {//SYLEE121004 

                    Namo=(No-nMaxPinHalf1)%32;
                    Mok1=(No-nMaxPinHalf1)/32;
                    No=nMaxPinHalf1+ Mok1*32*2+Namo;
                }                       

            }
            //else  if((SysSet12.m_nPinQD==3)&&(SysSet12.m_nPinQD2!=3))//UPPER=SD, LOWER= DD
            else  if((SysSet12.m_nPinQDUpper==BOX_SD)&&(SysSet12.m_nPinQDLower!=BOX_SD))    //son190902 32KPIN:  enum 변경
                                                                                            //      m_nPinQD-> m_nPinQDUpper로 변경
            {

                if(i<nMaxPinHalf1){

                    Namo=(No-nMaxPinHalf1)%32;
                    Mok1=(No-nMaxPinHalf1)/32;
                    No=nMaxPinHalf1+ Mok1*32*2+Namo;

                }
                else {

                    //ing 
                }                       

            }
            //else  if((SysSet12.m_nPinQD==3)&&(SysSet12.m_nPinQD2==3))//UPPER=SD, LOWER= SD
            else  if((SysSet12.m_nPinQDUpper==BOX_SD)&&(SysSet12.m_nPinQDLower==BOX_SD))    //son190902 32KPIN:  enum 변경
                                                                                            //      m_nPinQD-> m_nPinQDUpper로 변경
            {

                Namo=(No-nMaxPinHalf1)%32;
                Mok1=(No-nMaxPinHalf1)/32;
                No=nMaxPinHalf1+ Mok1*32*2+Namo;
            } 

            /////////////////////////////////////////////////////////////////////////////////////////
        }

        else   ///4k //   if(SysSet12.m_nPinBoxMax==1){  //2k//sylee120925
        {

// 1=QD, 2=DD, 3=SD

/*  //sylee131203  연구소 실험용
                if(SysSet12.m_nPinQD==2){//  2=DD -> QD
                    Namo=No%64;
                    Mok1=Namo/32;
                    No=No*2;
                    if(Mok1==1){
                        No++;
                    }           
                }
                else  if(SysSet12.m_nPinQD==3){//3=SD->QD
                    Namo=No%32;
                    Mok1=No/32;
                    No=Mok1*128+Namo*2;
                }
                */

            /*
               상부 
               1번 블록 SD (1-1024), 2번 블록 DD (1025-3072) 순서로 출력을 했는데,
               실제로는 1번 블록 위치에 DD가 연결되어있고 (1025-2077), 2번 블록 위치에 SD (1-1024)가 연결되어 있습니다.
               즉, SD,DD 순서로 만든 test file을 이차장 s/w에서 좌우 바꿔주는 겁니다.

               상부   sd( block 1)   + dd  ( block 2)  후    block 1과 2  바꾼다.
               하부  dd ( block 3)  +  sd  ( block 4) 

*/


            //sylee131209 연구소 실험용 

            if(No<(1024)){//sd

                Namo=No%32;
                Mok1=No/32;
                No=Mok1*128+Namo*2; 

            }
            else  if(No<(1024*3)){//dd

                No=No-(1024);   

                Namo=No%64;
                Mok1=Namo/32;
                No=No*2;
                if(Mok1==1){
                    No++;
                }

                No=No+4096;

            }
            else  if(No<(1024*5)){//dd

                No=No-(1024*3);

                Namo=No%64;
                Mok1=Namo/32;
                No=No*2;
                if(Mok1==1){
                    No++;
                }

                No=No+(4096*2);

            }
            else  if(No<(1024*6)){//sd
                No=No-(1024*5);

                Namo=No%32;
                Mok1=No/32;
                No=Mok1*128+Namo*2;
                No=No+(4096*3);
            }


            /*    if(No<(4096)){
                  No=No+(4096);
                  }
                  else  if(No<(4096*2)){
                  No=No-(4096);
                  }
                  */
            //sylee131203 연구소 실험용 
            //////////////////////////////////////////////////////////////////

        }

        /////////////////////////////////////////////////////////////////////////////////////////
/*//sylee210403
        if(SysSet12.m_n32k_16k==1){//3=SD->QD
            if(No>=(4096*2)){                    
                No=No+(4096*2);
            }
        }   
*/

        if( i< nLimitNo){//sylee131211
            PinMap[1][i]=No;//****>QD pin no
        }
        else {//sylee131211
            PinMap[1][i]=-1;//****>QD pin no//sylee131211
        }//sylee131211  
        //********************************************************************************************* 
        //======================================================================================


        if (SysSet12.m_nPinSwitchMode == 2)  //2=H, 1=Z 
        {
            Mok1=No/64;
            Namo=No%64;//same

            if(SysSet12.m_nPinBoxMax==1){  //2k
                //sylee120925

                BoxMok2=Mok1/32;//same
                Mok1=Mok1%32;

                if(Mok1<16){
                    Mok1=Mok1*2;
                }
                else  if( Mok1<32){
                    Mok1=(Mok1-15)*2-1;
                }
                else {
                    str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                    errMessageBox(7701, str); //son230217_2
                }
                No= (Mok1+(BoxMok2*32))*64+Namo;


            }
            else {//SysSet12.m_nPinBoxMax==2){  //4k

                BoxMok2=Mok1/64;//same
                Mok1=Mok1%64;
                if(Mok1<32){
                    Mok1=Mok1*2;
                }
                else  if( Mok1<64){
                    Mok1=(Mok1-31)*2-1;
                }
                else {
                    str.Format("Pin converting error  !!! \n \n     PIn No.=%d CHECK., Mok1=%d   ", No,Mok1 );        
                    errMessageBox(7701, str); //son230217_2
                }
                No= (Mok1+(BoxMok2*64))*64+Namo;

            }                  

        }


        int BoxN;


        if(SysSet12.m_nPinBoxMax==1){  //2k
            //sylee120925
            if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                BoxN=No/2048;
                No=No%2048;
                Mok1=No/128;//0`31 card
                Namo=No%128;//0`31 card
                No=Namo+((15-Mok1)*128);
                No=No+BoxN*2048;
            }
        }
        else {
            if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                BoxN=No/4096;
                No=No%4096;
                Mok1=No/128;//0`31 card
                Namo=No%128;//0`31 card
                No=Namo+((31-Mok1)*128);
                No=No+BoxN*4096;
            }
        }

        //================================================================================



        //CONNECTOR  REVERSE = 64 PIN REVERSE
        if( SysSet12.m_nCardReverseB==2 ){  //SYLEE130809               
            Namo=No%128;
            Mok1=No/128;
            if(Namo<64){
                Namo=Namo+64;
            }
            else {
                Namo=Namo-64;//191=127+64
            }
            No=Mok1*128+Namo;
        }

        if( i< nLimitNo){//sylee131211
            PinMap[2][i]=No;//****>SWITCH CARD pin no
        }
        else {//sylee131211
            PinMap[2][i]=-1;//sylee131211
        }//sylee131211              

        //son230303 hwPin -> jigPin을 찾기 위한 정보 저장
        int hwPin = PinMap[2][i];			//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)	//son230303
            nJigPinMap[hwPin] = i;    //son 빠른 검색용. //son230303

    }//for( i=0; i< 33000; i++){

    //===========================================================================================


    ///////////////////////////////////////////////
    //log
    FILE *fp; 
    char  fName[FILE_PATH_SIZE]; //son210514  200 -> FIlE_PATH_SIZE

    ::ZeroMemory(&fName, sizeof(fName)); 
    strcat( fName , "d:\\PinMap.ini" );

    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        errMessageBox(2005, "d:\\PinMap.ini    file Data Saving Error ! ,    file close!");         //son230217_2
        return 0;
    } 

    for(i=0;i<(4096*4);i++){
        fprintf(fp, "%10d, %10d, %10d\n",PinMap[0][i],PinMap[1][i], PinMap[2][i]);
    }
    fclose(fp); 
    ///////////////////////////////////////////////////


    return 1;


}



//son190902 32KPIN:  신규 생성
// 하부시작핀 설정을 변경한다.
void CModeSet12::OnSelchangeComboLowerStartPin() 
{
    // TODO: Add your control notification handler code here    
    CString str, strTemp;   
    // 현재 combo 박스의 위치 확인
    int nModeLowerStart = m_combo_LowerJigStartPin.GetCurSel(); 
    // 현재 combo box의 string이 '<Net Set>'이라면 
    // 변경된 하부시작핀 설정을 적용하지 않는다.
    m_combo_LowerJigStartPin.GetLBText(nModeLowerStart, str);    
    if (str == "<Not Set>")
    {
        //son201222
        strTemp.Format("OnSelchangeComboLowerStartPin() CurSel=%d  is \\<Not Set\\>!\n\n Check Login Error\n", nModeLowerStart);
        errMessageBox(7701, strTemp); //son230217_2
        return;
    } 
    // 변경된 설정을 하부시작핀 System 환경변수에 적용한다.
    SysSet12.m_nLowerJigStartPin = m_LowerStartPin.saList[nModeLowerStart].nPin;


    //son 상부 QD/DD/SD 설정 상태에 따른 UpperEndJigPin 번호를 계산하고 
    //     user가 선택한 하부시작핀과 비교해서 Offset이 있다면 새로운 하부시작핀외에 Offset도 저장한다.
    //    이 Offset을 반영해서  MakePinBlockRangeUserNo()에서  하부의 모든 핀들이 다시 계산된다.
    if (SysSet12.m_nBlockSet1 == 1) //son241007-BSW-9 begin
    {
        int nUpperEndJigPin = CalcUpperEndJigPin_BlockSet();
        if (nUpperEndJigPin >= 0)
        {
            //son UpperEndJigPin 보다 큰 값으로 설정이 되었다면 user가 Offset을 설정한 것 
            if (SysSet12.m_nLowerJigStartPin > (nUpperEndJigPin +1))
                SysSet12.m_nLowerJigStartPin_Offset = SysSet12.m_nLowerJigStartPin - (nUpperEndJigPin +1);

            else if (SysSet12.m_nLowerJigStartPin == (nUpperEndJigPin +1))  //son241124-BSW-9
                SysSet12.m_nLowerJigStartPin_Offset = 0;                    //son241124-BSW-9


            //son UpperEndJigPin 보다 작은 값으로는 하부시작핀 설정할 수 없다고 경고문 출력
            else if (SysSet12.m_nLowerJigStartPin < (nUpperEndJigPin +1))
            {
                strTemp.Format("LowerJigStartPin cannot be set to a value less than %d \n", nUpperEndJigPin +1);
                AfxMessageBox(strTemp, MB_OK); 

                //son 하부시작핀은 nUpperEndJigPin +1 로 default 설정한다.  
                SysSet12.m_nLowerJigStartPin = nUpperEndJigPin +1;
                SysSet12.m_nLowerJigStartPin_Offset = 0;

                //son nModeLowerStart 위치도 nUpperEndJigPin +1 로 바꾼다.
                nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin);
                if (nModeLowerStart >= 0)
                    m_combo_LowerJigStartPin.SetCurSel(nModeLowerStart);
            }
        }
    } //son241007-BSW-9 end

    // 변경된 하부시작핀에 맞는 Upper box 개수를 설정.
    SysSet12.m_nUpperBox = findUpperBoxNum(nModeLowerStart);    //son240516

    // 변경된 하부시작핀에 맞는 EndPin combo 설정.
    initCombo_EndPin(SysSet12.m_nPinTotMax);  //son240516    //son241007-BSW-9 
}

//son240516 begin:
//son PIN_TOT_32K_ORG 일 때에는 기존 하부시작핀 정보로 SysSet12.m_nUpperBox를 확인하고 이걸로 EndPin을 계산해야 한다.
//    PIN_TOT_32K_ORG 일 때에만 EndPin을 사용하고 그외의 경우에는 기존 TotMaxPin을 사용하기 때문임. 
//    점차 TotMaxPin이 아니라 EndPin을 이용하는 방향으로 간다. (PIN_TOT_32K_ORG 이전 모드는 ACE500에서만 다루기로)
int CModeSet12::findUpperBoxNum(int nModeLowerStart) 
{
    int nUpperBoxNum = UPPER_BOX_NUM;   //default  //son241007-BSW-9 4:UPPER_BOX_NUM

    if (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG)   //son241007-BSW-9 
    {
        if (SysSet12.m_nBlockSet1 == 1) 
        {
            //son 상부 pin block 중에 skip이 아닌 경우만 개수를 센다. 
            int nUpperBoxNum = 0; 
            for (int nStep=1;  nStep<=UPPER_BOX_NUM;  nStep++)
            {
                if (sPinBlockRange[nStep].nBlockType != BOX_SKIP)
                    nUpperBoxNum++; 
            }
        }
        else 
        {
            switch (SysSet12.m_nPinQDUpper)
            {
                case BOX_SD:
                    switch(nModeLowerStart) {
                        case E_1025: nUpperBoxNum = 1;   break; 
                        case E_2049: nUpperBoxNum = 2;   break; 
                        case E_3073: nUpperBoxNum = 3;   break; 
                        default:
                        case E_4097: nUpperBoxNum = 4;   break; 
                    }
                    break; 

                case BOX_DD:
                    switch(nModeLowerStart) {
                        case E_2049: nUpperBoxNum = 1;   break; 
                        case E_4097: nUpperBoxNum = 2;   break; 
                        case E_6145: nUpperBoxNum = 3;   break; 
                        default:
                        case E_8193: nUpperBoxNum = 4;   break; 
                    }
                    break; 

                default:
                case BOX_QD:
                    switch(nModeLowerStart) {
                        case E_4097:  nUpperBoxNum = 1;   break; 
                        case E_8193:  nUpperBoxNum = 2;   break; 
                        case E_12289: nUpperBoxNum = 3;   break; 
                        default:
                        case E_16385: nUpperBoxNum = 4;   break; 
                    }
                    break; 

            }
        }
    }
    return nUpperBoxNum;
}
//son240516 end.


//son240516  EndPin 설정을 변경한다.
//            32K Original PinMode에서만 사용.
void CModeSet12::OnSelchangeComboEndPin() 
{
    // TODO: Add your control notification handler code here    
    CString str, strTemp;   

    // 현재 combo 박스의 위치 확인 (0~ 3)
    int nModeEndPin= m_combo_EndPin.GetCurSel(); 

    // 현재 combo box의 string이 '<Net Set>'이라면 
    // 변경된 하부시작핀 설정을 적용하지 않는다.
    m_combo_EndPin.GetLBText(nModeEndPin, str);    
    if (str == "<Not Set>")
    {
        //son201222
        strTemp.Format("OnSelchangeComboEndPin() CurSel=%d  is \\<Not Set\\>!\n\n Check Login Error\n", nModeEndPin);
        errMessageBox(7701, strTemp); //son230217_2
        return;
    } 

    // 변경된 m_EndPin, m_nLowerBox 를 FileSysInfo 변수에 저장한다. 
    SysSet12.m_nLowerBox = (nModeEndPin +1);   // 박스 개수이므로  +1 한다.
    SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[nModeEndPin];
}

//son Upper 3 Combo
void CModeSet12::OnCombo21() //sylee210330-1 
{
    int nMode = m_combo21.GetCurSel();

    //son Upper 3가 SKIP으로 변경이 되었다면 Upper4도 SKIP이어야 하므로 
    //    강제 skip으로 설정하고 user가 Upper 4 콤보 박스도 건들지 못하게 바꾼다. 
    //    3번 SKIP후 4번을 QD/DD/SD 설정하는 것은 난이도가 너무 높고 효용성이 적음.
    if (SysSet12.m_nPinQDUpper3 != nMode+1 && (nMode+1) == BOX_SKIP) //son241007-BSW-9 begin
    {
        GetDlgItem(IDC_LABEL22)->EnableWindow(FALSE);     //son Upper 4 Disable
        GetDlgItem(IDC_COMBO22)->EnableWindow(FALSE);                      

        SysSet12.m_nPinQDUpper4 = BOX_SKIP;
        m_combo22.SetCurSel(BOX_SKIP -1);
    }
    else
    {
        GetDlgItem(IDC_LABEL22)->EnableWindow(TRUE);     //son Upper 4 Enable
        GetDlgItem(IDC_COMBO22)->EnableWindow(TRUE);                      
    }  //son241007-BSW-9 end

    SysSet12.m_nPinQDUpper3 = nMode+1;


    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}

//son Upper 4 Combo
void CModeSet12::OnCombo22() //sylee210330-1 
{
    int nMode = m_combo22.GetCurSel();
    SysSet12.m_nPinQDUpper4 = nMode+1;

    //son nQdType, nMaxPin에 맞게  하부StartPin 콤보박스 data를 초기화한다.
    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}

//son Lower 3 Combo
void CModeSet12::OnCombo23() //sylee210330-1 
{
    int nMode = m_combo23.GetCurSel();
    SysSet12.m_nPinQDLower3 = nMode+1;

    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9

}

//son Lower 4 Combo
void CModeSet12::OnCombo24() //sylee210330-1 
{
    int nMode = m_combo24.GetCurSel();
    SysSet12.m_nPinQDLower4 = nMode+1;

    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}


//son210831 son 하부시작핀이 상부 8K로 지정된 경우를 하부시작핀으로 설정한 경우까지 
//          고려해서 체크한다. SelfTest, PinSearch에서 속도개선을 위해  사용
bool CModeSet12::IsUpper8k(int nPinTotMax)
{
//son240522 begin
    if (nPinTotMax == PIN_TOT_32K_ORG)
    {
        if (SysSet12.m_nUpperBox == 2)
            return true;
        return false;
    }
//son240522 end 

    if (IS_UPPER_8K(nPinTotMax))  
        return true;

    //son 하부시작핀이 있고, 상부 8K만 사용하는 케이스가 존재하는 Type
    if ( nPinTotMax == PIN_TOT_32K_TRANS || nPinTotMax == PIN_TOT_20K_UP_TR //son230216_1
         || nPinTotMax == PIN_TOT_24K_UP_TR || nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son BlockSet을 사용하는 경우 SysSet12.m_nPinQDUpper만으로는 체크불가. 
        //    이경우는 SelfTest, PinSearch에서 PinMap jigPin이 -1이면 걸러내는 코드로 처리하도록 그냥 둔다.
        if (SysSet12.m_nBlockSet1 != 1)
        {
            //son 하부시작핀이 상부 8K로 설정된 경우
            switch (SysSet12.m_nPinQDUpper)
            {
                case BOX_QD: 
                    if (SysSet12.m_nLowerJigStartPin == 8193)  
                        return true;
                    break;

                case BOX_DD:
                    if (SysSet12.m_nLowerJigStartPin == 4097)  
                        return true;
                    break;

                case BOX_SD:
                    if (SysSet12.m_nLowerJigStartPin == 2049)  
                        return true;
                    break;
            }
        }
    }


    return false;
}

//son210831 son 하부시작핀이 상부 8K로 지정된 경우를 하부시작핀으로 설정한 경우까지 
//          고려해서 체크한다. SelfTest, PinSearch에서 속도개선을 위해  사용
bool CModeSet12::IsUpper4k(int nPinTotMax)
{
//son240522 begin
    if (nPinTotMax == PIN_TOT_32K_ORG)
    {
        if (SysSet12.m_nUpperBox == 1)
            return true;

        return false;
    }
//son240522 end

    if (IS_UPPER_4K(nPinTotMax))  
        return true;

    //son 하부시작핀으로 상부 4K만 사용하는 케이스가 존재하는 Type
    if ( nPinTotMax == PIN_TOT_32K_TRANS || nPinTotMax == PIN_TOT_20K_UP_TR     //son230216_1
         || nPinTotMax == PIN_TOT_24K_UP_TR || nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son BlockSet을 사용하는 경우 SysSet12.m_nPinQDUpper만으로는 체크불가. 
        //    이경우는 SelfTest, PinSearch에서 PinMap jigPin이 -1이면 걸러내는 코드로 처리하도록 그냥 둔다.
        if (SysSet12.m_nBlockSet1 != 1)
        {
            //son 하부시작핀이 상부 4K로 설정된 경우
            switch (SysSet12.m_nPinQDUpper)
            {
                case BOX_QD: 
                    if (SysSet12.m_nLowerJigStartPin == 4097)  
                        return true;
                    break;

                case BOX_DD:
                    if (SysSet12.m_nLowerJigStartPin == 2049)  
                        return true;
                    break;

                case BOX_SD:
                    if (SysSet12.m_nLowerJigStartPin == 1025)  
                        return true;
                    break;
            }
        }
    }


    return false;
}


bool CModeSet12::IsUpper12k(int nPinTotMax) //son240522  
{
    if (nPinTotMax == PIN_TOT_32K_ORG)
    {
        if (SysSet12.m_nUpperBox == 3)
            return true;
        return false;
    }

    if (IS_UPPER_12K(nPinTotMax))  
        return true;

    return false;
}
