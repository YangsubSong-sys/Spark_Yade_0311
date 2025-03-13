// ModeSet12.cpp : implementation file
/**
 * @brief Pin Connection ���� ����
 *
 * @details Jig Pin �� Hw Pin ���� Mapping ����� ���� ������ ����
 *
 * @author ��ȫ�� (��� ��� ���)
 * @date 2024-11-26
 *
 * ���� �̷�:
 * - 2024-11-26 son241124-BSW-9 ACE500 C&B �ɺ�Ϻ��� QD/DD ���� ��� ���� ��� ���� hotfix
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
                    
//son210514  int nPinBlockRange[10][5]�� CPinBlockRange sPinBlockRange[10]�� ����
CPinBlockRange sPinBlockRange[10];    //son MakePinBlockRangeUserNo()���� ����  //son210514
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
        DDX_Control(pDX, IDC_CHECK4, m_check4);     //son Manual QD/DD/SD üũ�ڽ�
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
 
    //son OK�� exit�� �� �Ϻ� Jig ��ȣ���� SysSet12.m_nLowerJigStartPin �� �ϴ� �ݿ��Ѵ�. 
    //    file�� save�ϴ� ���� 'save' �� �ñ��.
    if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
    {

        //son201222 OK Ŭ������ �� OnPinConvert1_32K()�� ����ǵ��� �ڵ� save �ǰ�  ����.
        //son LoadSaveSet12(_SAVE)-> ModeSet12.OnPinConvert1_32K(OnPinConvertType1)
        int ret=FileSysInfo01.LoadSaveSet12(_SAVE); //son190902 32KPIN: !!LoadSaveSet12(_SAVE)�ű� �߰�
        if(ret!=1){
            errMessageBox(1212, "SysSet12.ini  Save ERROR !  \n    file Read Error !!!");    //son230217_2
        }
    }


//  FileSysInfo01.LoadSaveSub71(2);

    CDialog::OnOK();

}

extern short  OnPinConvertType1; //son241007-BSW-9    //son .CON ������ �ǹ������� 1 (total�� 1�̻�, checksum ����)
BOOL CModeSet12::OnInitDialog() 
{

    CDialog::OnInitDialog();

    CString str;
    int ret;

    //son LoadSaveSet12(_LOAD)-> ModeSet12.OnPinConvert1_32K(OnPinConvertType1)
    ret=FileSysInfo01.LoadSaveSet12(_LOAD); //son190902 32KPIN: enum ǥ��
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


    //son PInBlock�� 12K_QD_DD (��� QD 2 box, �Ϻ� DD 2box��  12K) Ư��Ÿ���� �� 
    //if( SysSet13.m_nPinBlockType==7) {   //sylee121803      
    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //son190902 32KPIN  7:PIN_BLK_12K_QD_DD 
    {
        //son  �Ϻ� check box�� QD BlockType��  DD�� �����Ͽ� �����Ѵ�.
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

        //son QD ��ġ�� �̸��� SD�� �ٲٰ�, DD, SD �� ����ó��
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



    //son QD,DD,SD �������ø�尡 �ƴϸ� 
    if( SysSet12.m_nBlockSet1  != 1)    //son210514
    {
        //son �ű�PinMode��  Upper2�� Upper1�� �����ϰ�, Lower2�� Lower1�� �����ϰ� ���������Ѵ�.
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
    //son-8K4097-20200605  : MaxPin �޺��ڽ�(m_combo1) data �ʱ�ȭ
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


    //son 32K Trans Pin Block Type�� ������ PinBoxMax�� 4K�� ���� �Ѵ�.
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS)    //son210525 //son230216_1
    {
        SysSet12.m_nPinBoxMax = 2;  //1:2K, 2:4K
        GetDlgItem(IDC_COMBO102)->EnableWindow(FALSE);       
    } //son210525

    //son 32K ���� ���� Pin Block Type�� ������ PinBoxMax�� �Է¹��� �� �ִ�.
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



    //son ������ User�� �α����� ���� �ʾҴٸ� ��� ������ �Ѵ�.
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
        //son210525 32K Trans �� ���� PinBoxMax�� ReadOnly�� ���.
        if (SysSet13.m_nPinBlockType != PIN_BLK_32K_TRANS)    //son230216_1
            GetDlgItem(IDC_COMBO102)->EnableWindow(TRUE);

        GetDlgItem(IDC_COMBO103)->EnableWindow(TRUE);    
        GetDlgItem(IDC_COMBO104)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO105)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO108)->EnableWindow(TRUE); 

    }


    //------------------------------------------------------------------
    //son-32KPIN-20200227  : �Ϻ�StartPin �޺��ڽ�(m_combo_LowerJigStartPin) data �ʱ�ȭ
 
    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    //    �ݵ�� QD�� MaxPin �ʱ�ȭ(FileSysInfo01.LoadSaveSet12(_LOAD))�� �Ϸ�� ������ ȣ���ؾ� �Ѵ�.
    if( SysSet12.m_nBlockSet1  == 1)    //son210514
        //son bForce�� TRUE�̸� default �Ϻν������� ������ �����ȴ�.
        initCombo_LowerStartPin(BOX_QD, SysSet12.m_nPinTotMax, TRUE); //son210514 QD �������� default �Ϻν������� �޺��ڽ��� ����
    else 
        initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
    

    // ����� �Ϻν����ɿ� �´� EndPin combo ����.
    initCombo_EndPin(SysSet12.m_nPinTotMax); //son241007-BSW-9

 
    UpdateData(FALSE);      // UI�� ���� �ݿ��Ѵ�.

    FileSysInfo01.LoadSaveSet211(2);   //load //sylee160928

    A_Language() ;

    return 1;
}

//son210514 QD/DD/SD ��Ʈ���� ��Ȳ�� �°� Enable Disable�Ѵ�.
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
    //son MaxPin �� ����ÿ� �Ϻν����� �ڽ� enable/disable�ϱ�   
    //-----------------------------------------------------------
    
    //son190902 32KPIN: 32K PinBlockŸ���� ������ LowerJigStartPin �޺� �ڽ��� enable�Ѵ�.    
    //son241007-BSW-9 EnableDisable_QDDDSDControl() �Լ� ���η� �̵� 
    if ( SysSet12.m_nBlockSet1  != 1 //son241007-BSW-9
            && IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
    {
        //son QD/DD/SD Block Set�� OFF�̸� �Ϻν����� Combo�� Enable�Ѵ�.
        GetDlgItem(IDC_COMBO_LOWER_START_PIN)->EnableWindow(TRUE);
        
    }
    else
    {
        //son QD/DD/SD Block Set�� ON�̸� �Ϻν����� Combo�� Disable�Ѵ�.
        //    ���� QD/DD/SD Block Set ���¿����� �Ϻν����� ����� ����� �������� �ʴ´�.
		//son241007-BSW-9 32K Org ������ �ɺ�� QD/DD/SD Block Set�� ON�̾ �Ϻν����� �����ϴ� ��� �߰�
		if (SysSet12.m_nPinTotMax != PIN_TOT_32K_ORG)
            GetDlgItem(IDC_COMBO_LOWER_START_PIN)->EnableWindow(FALSE);
        

    }


    //-------------------------------------------------------------------
    //son QD/DD/SD BlockSet �����϶� Ư�����̽� check & Combo Disable

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
    //son QD/DD/SD BlockSet �ƴ� ��  Ư�����̽� check & Combo Disable
    else
    {

        //son-32KPIN-20200117: 32K������ QD, DD, SD üũ��� �߰� ����
        //------------------------------------------------------------------------
        // 32K�� Upper1, Lower1 üũ�ڽ��� LowerJigStartPin�� ������ �����ϹǷ�
        // Upper2, Lower2 ���� üũ�ڽ����� ��� Disable �Ѵ�.
        // 32K�� �ƴ� ����� Upper2, Lower üũ�ڽ��� �ٽ� enable�ؾ� �Ѵ�.
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
            //son-32KPIN-20200117: 32K BlockType���� QD, DD, SD üũ��� �߰�
            //   32K Upper1, Lower1 üũ�ڽ��� LowerJigStartPin�� ������ �����ϹǷ�
            //       Upper2, Lower2 ���� üũ�ڽ����� ��� Disable �Ѵ�.
            GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);    //son Upper2 QD
            GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);    //son Upper2 DD
            GetDlgItem(IDC_CHECK10)->EnableWindow(FALSE);   //son Upper2 SD

            GetDlgItem(IDC_CHECK11)->EnableWindow(FALSE);   //son Lower2 QD
            GetDlgItem(IDC_CHECK12)->EnableWindow(FALSE);   //son Lower2 DD
            GetDlgItem(IDC_CHECK13)->EnableWindow(FALSE);   //son Lower2 SD

        }

    }
    UpdateData(FALSE);       // ȭ�鿡 ���� �ݿ� 
}
 
//son210115  QDType, MaxPin�� ����  LowerStartPin�� ���� �ʴ� ��쿡 �����Ѵ�. 
//           �ݵ�� QD�� MaxPin �ʱ�ȭ�� �Ϸ�� ������ ȣ���ؾ� �Ѵ�.
//
//  File���� �ε��� �Ϻν����� ���� ������� ���� ���� ��ġ�� ���Ѵ�. 
//  repair_LowerStartPin()�� ����ϴ� OnPinConvert1_32K()�� �����Լ��� 
//  FileSysInfo���� ModeSet12(Pin���� UI)�� ���� ����  ȣ��ǹǷ� 
//  �Ϻν����� �޺��ڽ��� ���⼭ ������ �� ���� initCombo_LowerStartPin()�� 
//  ���߿� Pin ���� UI �����ÿ� �����Ѵ�.
void CModeSet12::repair_LowerStartPin(int nQdType, int nMaxPin) 
{

    //==========================================================================
    //son BlockSet1=1: BlockRange�� �Ϻν������� �ڵ������Ǵ� ���̽�
    //==========================================================================
    int nModeLowerStart;

    //son 32K Org�̰� QD/DD/SD ȥ���� ������ SysSet12.m_nLowerJigStartPin_Offset ������ 
    //     �ڵ尡 ���� �� �־ �Ϻν����� �������� �ʰ� m_nUpperBox �� �����Ѵ�. 
    if (SysSet12.m_nBlockSet1 == 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG) //son241007-BSW-9 begin
    {
        nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin); 
        if (nModeLowerStart >= 0)       
            SysSet12.m_nUpperBox = findUpperBoxNum(nModeLowerStart);    
        return;
    } //son241007-BSW-9 end

    if (SysSet12.m_nBlockSet1 == 1      //son210525
            //son230718 || IS_UPPER_12K(nMaxPin)  
            || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS     //son210826 ���� nEnd�� �´� �����Ϻν����� �ʿ�
            || SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS    //son230718 �����Ϻ��� ��� Ư�����̽�
            || SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)     //son230718 �����Ϻ��� ��� Ư�����̽�
    {                 
        //return ; //sylee210407  
        
        //son MakePinBlockRangeUserNo()���� ������ sPinBlockRange[5].nStartPin���� nUpperEnd ���
        int nUpperEnd = sPinBlockRange[5].nStartPin -1;

        //son �Ϻν����� ����.  QD ��������
        SysSet12.m_nLowerJigStartPin = FindBlockSet_LowerStartPin(nUpperEnd);  //son210514
        return;
    }


    //================================================
    // ����ڰ� ������  �Ϻν������� ����ϴ� ���̽� 
    //================================================

    //son210202: ���� Ÿ���� ���� HW QDType��  �Ϻν������� �������� �����Ƿ� return�Ѵ�.
    if (nMaxPin == PIN_TOT_8K_LOW_STPIN //son ���� 16KHW�� 8K(4K+4K) �����
            || nMaxPin == PIN_TOT_12K
            || nMaxPin == PIN_TOT_U8K_UPPER_ONLY)   //son211020
        return;
 

    //----------------------------------------------------------
    //son �Ϻν����� ������ ���� UpperBox ���� ������ ã�´�.
    nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin); //son240516
    if (nModeLowerStart >= 0)       
        SysSet12.m_nUpperBox = findUpperBoxNum(nModeLowerStart);    //son240516

    
    //----------------------------------------------------------
    //son QDType�� MaxPin ���� �� ���� �Ϻν���Pin ������ ����
    
    //son ���� �Ϻν�����Pin ���� range �̳���� ���� ���� �ְ�  range�� ����ٸ� default������ ������ �ش�.
    //son son �Է� ������ E_LOWER_STARTPIN enum ������ ���Ѿ� �Ѵ�. 
    //son �⺻������ �Ϻν������� HalfPin+1�ε�,(32K��� 16385) 
    //son Upper Box�� �޺κ��� dummy�� �����ϱ� ����  boxPin ���� ����(QD��� 4097, 8193 )�� ������ �� �ִ�. 
    switch (nQdType)
    {
        case BOX_SD:
            //son BoxSize 1024, 1 box  // 8K SD HalfPin�� 1024�̹Ƿ� 1025 ����,  
            if (nMaxPin == PIN_TOT_8K_TRANS)       //son210319 //son230216_1
            {
                if (SysSet12.m_nLowerJigStartPin != 1025)
                    SysSet12.m_nLowerJigStartPin = 1025;          
            }

            //son BoxSize 1024, 2 box  // 16K SD HalfPin�� 2048�̹Ƿ� 2049, 1025 ����,  
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS    //son210319 //son230216_1
                    || nMaxPin == PIN_TOT_32K_U8K_L24K)     //son230221_2
            {
                if (SysSet12.m_nLowerJigStartPin != 1025 && 
                    SysSet12.m_nLowerJigStartPin != 2049 )
                    SysSet12.m_nLowerJigStartPin = 2049;          //son210308 || �������� �߸��ڵ�. &&�� �����Ϸ�
            } 

            //son BoxSize 1024, 4 box // 32K SD HalfPin�� 4096�̹Ƿ� 4097, 2049, 1025 ����,  
            else                                                                                    
            {
                if (SysSet12.m_nLowerJigStartPin != 1025 && 
                    SysSet12.m_nLowerJigStartPin != 2049 && 
                    SysSet12.m_nLowerJigStartPin != 3073 &&     //son240522 ��� block 3�� ��� �߰�
                    SysSet12.m_nLowerJigStartPin != 4097 ) 
                {
                    SysSet12.m_nLowerJigStartPin = 4097;          //son210308 || �������� �߸��ڵ�. &&�� �����Ϸ�
                    SysSet12.m_nUpperBox = 4;    //son240516 �ʱ�ȭ
                }
            }
            break;

        case BOX_DD:
            //son BoxSize 2048, 1 box  // 8K DD HalfPin�� 2048�̹Ƿ� 2049�� ����,  
            if (nMaxPin == PIN_TOT_8K_TRANS)       //son210319 //son230216_1
            {
                if ( SysSet12.m_nLowerJigStartPin != 2049 )
                    SysSet12.m_nLowerJigStartPin = 2049;          
            }
            //son BoxSize 2048, 2 box  // 16K DD HalfPin�� 4096�̹Ƿ� 4097, 2049 ����,  
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS //son210319 //son230216_1
                    || nMaxPin == PIN_TOT_32K_U8K_L24K)     //son230221_2
            {
                if ( SysSet12.m_nLowerJigStartPin != 2049 && 
                    SysSet12.m_nLowerJigStartPin != 4097 )
                    SysSet12.m_nLowerJigStartPin = 4097;          //son210308 || �������� �߸��ڵ�. &&�� �����Ϸ�
            }

            //son BoxSize 2048, 4 box  //32K DD HalfPin�� 8192�̹Ƿ� 8193, 4097, 2049 ����,  
            else                                                                                    
            {
                if (SysSet12.m_nLowerJigStartPin != 2049 && 
                    SysSet12.m_nLowerJigStartPin != 4097 && 
                    SysSet12.m_nLowerJigStartPin != 6145 &&     //son240522 ��� block 3�� ��� �߰�
                    SysSet12.m_nLowerJigStartPin != 8193 ) 
                {
                    SysSet12.m_nLowerJigStartPin = 8193;          //son210308 || �������� �߸��ڵ�. &&�� �����Ϸ�
                    SysSet12.m_nUpperBox = 4;    //son240516 �ʱ�ȭ
                }

            }
            break;

        case BOX_QD:
        default:
            //son Boxsize 4096, 1 box           // 8K QD HalfPin�� 4096�̹Ƿ� 4097�� ����   
            if (nMaxPin == PIN_TOT_8K_TRANS)         //son210319  //son230216_1
            {
                if ( SysSet12.m_nLowerJigStartPin != 4097) 
                    SysSet12.m_nLowerJigStartPin = 4097;          
            }

            //son Boxsize 4096, 2 box          // 16K QD HalfPin�� 8192�̹Ƿ� 8193, 4097 ����   
            else if (nMaxPin == PIN_TOT_16K_TRANS || nMaxPin == PIN_TOT_12K_TRANS  //son210319 //son230216_1
                    || nMaxPin == PIN_TOT_32K_U8K_L24K)     //son230221_2
            {
                if ( SysSet12.m_nLowerJigStartPin != 4097 && 
                    SysSet12.m_nLowerJigStartPin != 8193 )
                    SysSet12.m_nLowerJigStartPin = 8193;          //son210308 || �������� �߸��ڵ�. &&�� �����Ϸ�
            }

            //son Boxsize 4096, 4 box           //32K QD HalfPin�� 16384�̹Ƿ� 16385, 8193, 4097 ���� 
            else
            {
                if (SysSet12.m_nLowerJigStartPin != 4097 && 
                    SysSet12.m_nLowerJigStartPin != 8193 && 
                    SysSet12.m_nLowerJigStartPin != 12289 &&   //son240522 ��� block 3�� ��� �߰�
                    SysSet12.m_nLowerJigStartPin != 16385 )
                {
                    SysSet12.m_nLowerJigStartPin = 16385;          //son210308 || �������� �߸��ڵ�. &&�� �����Ϸ�
                    SysSet12.m_nUpperBox = 4;    //son240516 �ʱ�ȭ
                }

            }
            break;
    }

}


//son201222 nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
//    �ݵ�� QD�� MaxPin �ʱ�ȭ(FileSysInfo01.LoadSaveSet12(_LOAD))�� �Ϸ�� ������ ȣ���ؾ� �Ѵ�.
// 
void CModeSet12::initCombo_LowerStartPin(int nQdType, int nMaxPin, bool bForce)  //son210514 bForce�߰�
{

    m_combo_LowerJigStartPin.ResetContent();

//son230221_2 begin
    //son K600 Ư�� ���̽�  
    //    BDL�� ��� 8K,  �Ϻ� 24K.  HW�����δ� �Ϻ� 5, 6 box ��� HW  3,4 �ڽ��� ����ȴ�. 
    if (nMaxPin == PIN_TOT_32K_U8K_L24K)
    {
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 0: E_1025

        SysSet12.m_nUpperBox = 2;    //son240516 �ʱ�ȭ
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

    //son210525 Ư�� ���̽� ���� ó�� 
    //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS�� ���⿡ �ش��� �� �ȴ�. 5��° box�� 16385�� �����ؾ� ��.
    if (IS_UPPER_12K_2(nMaxPin))  //son210603    //son210826    //son230718
    {
        SysSet12.m_nUpperBox = 3;    //son240516 �ʱ�ȭ    

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

    //son210603 Ư�����̽� 2
    //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS Ư�����̽�. 5��° box�� 16385�� �����ؾ� ��.
    if ( nMaxPin == PIN_TOT_20K_U12K_L4K_UP_TRANS        //son210603 �Ϻν����� QD 16385, DD 8193, SD 4097 �������� �߰���. 
            || nMaxPin == PIN_TOT_20K_U8K_L4K_UP_TRANS  //son210826 �Ϻν����� QD 16385, DD 8193, SD 4097 �������̽� 2
            || nMaxPin == PIN_TOT_24K_U12K_L8K_TRANS)   //son230718 �Ϻν����� QD 16385, DD 8193, SD 4097 �������̽� 3
    {

        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 0: E_1025
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");           // 1: E_2049
        if (nMaxPin == PIN_TOT_20K_U12K_L4K_UP_TRANS)
            SysSet12.m_nUpperBox = 3;    //son240516 �ʱ�ȭ
        if (nMaxPin == PIN_TOT_20K_U8K_L4K_UP_TRANS)
            SysSet12.m_nUpperBox = 2;    //son240516 �ʱ�ȭ

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

    //son QD/DD/SD BlockSet�� ����ϴ� ���
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


    //son-8K4097-20200605: ��� 4K, �Ϻ� 4K �������� 
    //               hwPin<->swPin ��ȯ�� ���� �Ϻν����� Offest��  QD, DD, SD  ������� ����ϴ� ���. 
    //son ���� 16KHW�� 8K �����
    //son 16K���� HW�� 1box�� �����ؼ� 8K BoxQd=DD �� ���� Ư�� ���̽��� �Ϻν������� 16Kó�� �����ϰ� ���ش�.
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

        //son 8K with �Ϻν����� ��� :   
        //   �⺻ HW�� QD: 16K BlockType. ��� 1block 1block(skip), �Ϻ� 1block 1block(skip) �� 2 block ����ϹǷ� 
        //   default �Ϻν������� 4097���� �����Ѵ�.  QD, DD, SD ������� 4097 �Ϻν����� ������ �����ؾ� �Ѵ�.
        setCombo_DefaultLowerStartPin(E_4097, bForce);  //son201222 //son210514
        //SysSet12.m_nUpperBox = 1;    //son240516 �ʱ�ȭ
        return;
    }

    //son210202: ��� 8K, �Ϻ� 4K ���� BlockType.
    //           hwPin<->swPin��ȯ�� ����, �Ϻν����� Offest��  QD, DD, SD  ������� ����ϴ� ���. 
    if(nMaxPin == PIN_TOT_12K
            || nMaxPin == PIN_TOT_U8K_UPPER_ONLY)   //son211020 8K���� ��θ� �����ϴ� ���̽� (f550j ����ý�) 
    {
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
        m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3:E_8193
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
        m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

        //son 12K with �Ϻν����� ��� :   
        //   �⺻ HW�� QD: 16K BlockType. ��� 2block, �Ϻ� 1block  �� 3 block ����ϹǷ� 
        //   default �Ϻν������� 8193���� �����Ѵ�.  
        setCombo_DefaultLowerStartPin(E_8193, bForce);  //son201222 //son210514
        //SysSet12.m_nUpperBox = 2;    //son240516 �ʱ�ȭ
        return;
    }

 
    //----------------------------------------------------------
    //son QDType�� MaxPin ���� �� ���� �Ϻν���Pin�� �����Ѵ�.
    //----------------------------------------------------------
 
    //son ���� �Ϻν�����Pin ���� range �̳���� ���� ���� �ְ�  range�� ����ٸ� default������ ������ �ش�.
    //son son �Է� ������ E_LOWER_STARTPIN enum ������ ���Ѿ� �Ѵ�. 
    
    //son �⺻������ �Ϻν������� HalfPin+1�ε�,(32K��� 16385) 
    //son Upper Box�� �޺κ��� dummy�� �����ϱ� ����  boxPin ���� ����(QD��� 4097, 8193 )�� ������ �� �ִ�. 
    
    switch (nQdType)
    {
        case BOX_SD:
            //son 8K SD HalfPin�� 1024�̹Ƿ� 1025��  ����,  (BoxSize 1024, 1box) 
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
                //SysSet12.m_nUpperBox = 1;    //son240516 �ʱ�ȭ
            }
            //son 16K SD HalfPin�� 2048�̹Ƿ� 2049, 1025 ����,  (BoxSize 1024, 2box) 
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
                //SysSet12.m_nUpperBox = 2;    //son240516 �ʱ�ȭ
            }

            //son 32K SD HalfPin�� 4096�̹Ƿ� 4097, 2049, 1025 ����,  (BoxSize 1024, 4box) 
            // ����: PIN_TOT_16K�� ����� ������ ��. PIN_TOT_16K, SD �Ϻν����� 2049�� �� �ȿ� �����Ƿ� OK.
            else                                                                                    
            {
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_1025].str);  // 0:E_1025
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 3
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_3073].str);  // 5:E_3073  //son240522 ��� 3block ����߰� 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525

                setCombo_DefaultLowerStartPin(E_4097, bForce);  //son201222  
                //SysSet12.m_nUpperBox = 4;    //son240516 �ʱ�ȭ
            }
            break;

        case BOX_DD:
            //son MaxPin�� 8K. DD HalfPin�� 2049�̹Ƿ�  2049��  ����  (Boxsize 2048, 1box)
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
                //SysSet12.m_nUpperBox = 1;    //son240516 �ʱ�ȭ

            }
            //son 16K DD HalfPin�� 4096�̹Ƿ� 4097, 2049 ����,  (BoxSize 2048, 2box) 
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
                //SysSet12.m_nUpperBox = 2;    //son240516 �ʱ�ȭ
            }

            //son 32K DD HalfPin�� 8192�̹Ƿ� 8193, 4097, 2049 ����,  (BoxSize 2048, 4box) 
            // ����: PIN_TOT_16K�� ����� ������ ��. PIN_TOT_16K, DD �Ϻν����� 4097�� �� �ȿ� �����Ƿ� OK.
            else                                                                                    
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 0
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_2049].str);  // 1:E_2049 
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);  // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);  // 3:E_8193 
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 4
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_6145].str);  // 6:E_6145    //son240522 ��� 3 block ����߰�
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                         // 7    //son210525
                setCombo_DefaultLowerStartPin(E_8193, bForce); //son201222 
                //SysSet12.m_nUpperBox = 4;    //son240516 �ʱ�ȭ
            }
            break;

        case BOX_QD:
        default:
            //son MaxPin�� 8K. QD HalfPin�� 4096�̹Ƿ�  4097��  ����   (Boxsize 4096, 1box)
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
                //SysSet12.m_nUpperBox = 1;    //son240516 �ʱ�ȭ

            }

            //son 16K QD HalfPin�� 8192�̹Ƿ� 8193, 4097 ����   (Boxsize 4096, 2box)
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
                //SysSet12.m_nUpperBox = 2;    //son240516 �ʱ�ȭ
            }

            //son 32K QD HalfPin�� 16384�̹Ƿ� 16385, 8193, 4097 ���� (Boxsize 4096, 4box)
            // ����: PIN_TOT_16K�� ����� ������ ��. PIN_TOT_16K, QD �Ϻν����� 8193�� �� �ȿ� �����Ƿ� OK.
            else
            {
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 0
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 1
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_4097].str);      // 2:E_4097
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_8193].str);      // 3:E_8193
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_16385].str);     // 4:E_16385
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 5    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, "<Not Set>");                             // 6    //son210525
                m_combo_LowerJigStartPin.InsertString(-1, m_LowerStartPin.saList[E_12289].str);     // 7:E_12289    //son240522 ��� 3 block ����߰�
                setCombo_DefaultLowerStartPin(E_16385, bForce); //son201222  //son210514
                //SysSet12.m_nUpperBox = 4;    //son240516 �ʱ�ȭ
            }
            break;
    }


}

//son201222 ���� System�� �Ϻν����� ���� ComboBox �ȿ� �ִٸ� ���� ���� �ְ� ���ٸ� default������ ������ �ش�.
// return false: ���� SysSet12.m_nLowerJigStartPin �� ����      //son240516 return �� �߰�
// return true:  ��û�� default ������ ���� 
bool CModeSet12::setCombo_DefaultLowerStartPin(int nDefaultMode_LowerStart, bool bForce)  //son210514 bForce �߰�
{
    bool bRet = false;  //son240516
    int nModeLowerStart;
    CString strJigStart;        
    //nModeLowerStart =  m_LowerStartPin.findItemNo(LOWER_STARTPIN_NUM, SysSet12.m_nLowerJigStartPin);
    //if (nModeLowerStart < 0)       
    //son ���� System�� �Ϻν����� ���� ComboBox �ȿ� ���ٸ� default������ �����Ѵ�.
    strJigStart.Format("%d", SysSet12.m_nLowerJigStartPin);        
    nModeLowerStart =  m_combo_LowerJigStartPin.FindString(-1, strJigStart);
    if (nModeLowerStart == CB_ERR   //son Combo�ȿ� ���� �Ϻν����� ���� ���ų�
            || bForce == TRUE)      //son210514 ������ default ������ �����ϴ� �ɼ��� On�̶�� 
    {
#if 0
        //sylee230224      m_combo_LowerJigStartPin.SetCurSel(nDefaultMode_LowerStart); 
	    m_combo_LowerJigStartPin.SetCurSel(nModeLowerStart);	  //sylee230224  
#else
        m_combo_LowerJigStartPin.SetCurSel(nDefaultMode_LowerStart);    
#endif
        OnSelchangeComboLowerStartPin(); //son ������ LowerJigStartPin combobox ������ ���� ������ �ݿ�
        bRet = true;  //son240516
    }   
    else 
        //son ���� SysSet12.m_nLowerJigStartPin �� �޺��ڽ��� ���̰� ����
        m_combo_LowerJigStartPin.SetCurSel(nModeLowerStart);

    //son combo�ڽ��� ���� �ݿ��Ѵ�.
    UpdateData(FALSE);      

    return bRet;   //son240516
}

//son241007-BSW-9  m_nLowerEndBox_EndPin[i]�� �����ϰ� EndPin Combo�� ä��� SysSet12.m_nEndPin�� �缳���Ѵ�.
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

    //son m_nLowerJigStartPin_Offset�� �߰� ������  ��쿡�� EndPin�� ���� �з��� �Ѵ�. 
    nUpperPin += SysSet12.m_nLowerJigStartPin_Offset; //son241007-BSW-9  

    //---------------------------------------------------------------------------------
    //son UpperBox ������  EndPin ���̽� 4������ ���� EndPin combo �ڽ��� �ִ´�.
    //    �� �ܰ迡���� ���� m_nLowerBox�� �������� �ʰ� box 4���� case�� combo�� �־�� �Ѵ�.  
    //    m_combo_EndPin �޺��ڽ��� 4���� �׸��� ������ �� �׸��� combo ��ġ�� �ڽ���ȣ-1 (0~3)�̴�.

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

    //son ���� FileSysInfo�� SysSet12.m_nLowerBox ������  m_combo_EndPin, SysSet12.m_nEndPin�� ����.
    SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[SysSet12.m_nLowerBox -1];   //son241126-BSW-222


    //son FileSysInfo�� SysSet12.m_nLowerBox����  ���ٸ� 4��° box �ִ밪���� default �����Ѵ�. 
    if (SysSet12.m_nLowerBox <= 0 || SysSet12.m_nLowerBox > MAX_BOX_PER_PART) 
    {
        SysSet12.m_nLowerBox = 4;  //son 4��° box
        SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[BOX_D];
    }


    //son ���� FileSysInfo�� SysSet12.m_nLowerBox ������  m_combo_EndPin�� ����.
    m_combo_EndPin.SetCurSel((SysSet12.m_nLowerBox -1));
    return 0;


}

//son240516 �߰� 
//son m_nLowerEndBox_EndPin[i]�� �����ϰ� EndPin Combo�� ä��� SysSet12.m_nEndPin�� �缳���Ѵ�.
int CModeSet12::initCombo_EndPin(int nMaxPin)  
{

    m_combo_EndPin.ResetContent();

    //son nMaxPin��  PIN_TOT_32K_ORG �� ������ initCombo_EndPin() �������Ѵ�.
    if (nMaxPin != PIN_TOT_32K_ORG)
    {
        UpdateData(TRUE);       // ȭ���� ���� �����´�.
        //son PIN_TOT_32K_ORG�� �ƴϸ� EndPin �޺��ڽ��� Disable 
        GetDlgItem(IDC_COMBO_END_PIN)->EnableWindow(FALSE);
        return -1;
    }

    ASSERT(SysSet12.m_nUpperBox == 4 || SysSet12.m_nUpperBox == 3 || SysSet12.m_nUpperBox == 2 || SysSet12.m_nUpperBox == 1);


    GetDlgItem(IDC_COMBO_END_PIN)->EnableWindow(TRUE);   //son241126-BSW-222

    if( SysSet12.m_nBlockSet1  == 1) { //son241007-BSW-9
        //son SysSet12.m_nPinQDUpper~SysSet12.m_nPinQDUpper4�� SysSet12.m_nPinQDLower~m_nPinQDLower4�� ȥ���ϴ� ���
        return initCombo_EndPin_BlockSet(); 
    }


    //-------------------------------------------------------------------------------------------
    //son Upper�� SysSet12.m_nPinQDLower  �����̰�, Lower�� SysSet12.m_nPinQDLower  ������ ���
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
    //son UpperBox ������  EndPin ���̽� 4������ ���� EndPin combo �ڽ��� �ִ´�.
    //    �� �ܰ迡���� ���� m_nLowerBox�� �������� �ʰ� box 4���� case�� combo�� �־�� �Ѵ�.  
    //    m_combo_EndPin �޺��ڽ��� 4���� �׸��� ������ �� �׸��� combo ��ġ�� �ڽ���ȣ-1 (0~3)�̴�.

    int lower_pins[MAX_BOX_PER_PART]= {0, 0, 0, 0};
    CString strLowerPin[MAX_BOX_PER_PART]; 
    for (int i= 0; i < MAX_BOX_PER_PART; i++)
    {
        lower_pins[i] = getBoxPinCount((PIN_BOXTYPE)SysSet12.m_nPinQDLower) * (i + 1);
        m_nLowerEndBox_EndPin[i] = nUpperPin + lower_pins[i]; //son241007-BSW-9
        strLowerPin[i].Format("%dK", (m_nLowerEndBox_EndPin[i] /1024));

        m_combo_EndPin.InsertString(-1, strLowerPin[i].GetBuffer(10));           
    }

    //son ���� FileSysInfo�� SysSet12.m_nLowerBox ������  m_combo_EndPin, SysSet12.m_nEndPin�� ����.
    SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[SysSet12.m_nLowerBox -1];      //son241126-BSW-222

    //son FileSysInfo�� SysSet12.m_nLowerBox����  ���ٸ� 4��° box �ִ밪���� default �����Ѵ�. 
    if (SysSet12.m_nLowerBox <= 0 || SysSet12.m_nLowerBox > MAX_BOX_PER_PART) 
    {
        SysSet12.m_nLowerBox = 4;  //son 4��° box
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



//son MaxPin �� �����
void CModeSet12::OnSelchangeCombo1() 
{
    //------------------------------
    //son MaxPin �� UI���� �������� 

    CString str, out;
    int nNewPinTotMax = m_combo1.GetCurSel();


    //son-32KPIN-20200103
    //------------------------------------------------------------------------------
    //son PinBlockType�� 32K�ε� PinTotMax ���� 32K �ƴѰ��� �����ߴٸ�     
    //    �ź��ϰ�  ������ ������ �ǵ�����. PinBlockType�� ���� �����ؾ� ��.
    //------------------------------------------------------------------------------
 
    // ���� �Էµ� ���� �������� �ٸ��ٸ� �޽������
    if (SysSet12.m_nPinTotMax != nNewPinTotMax) 
    {   
        //------------------------------------------
        //  PinBlockType�� PinTotMax ����ġ Ȯ�� 
        //------------------------------------------
        
        //son  No 32K PinBlockŸ���ε�  32K Max Pin üũ
        if (SysSet13.m_nPinBlockType != PIN_BLK_32K_TRANS)    // prev value //son230216_1
        {
            //son PinBlockType�� 32K�� �Է����� �ʰ� PinTotMax ���� 32K�� �����ϴ� ���� �ź��Ѵ�.
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

                // ���� SysSet12.m_nPinTotMax�� ������ �����Ѵ�.
                m_combo1.SetCurSel(SysSet12.m_nPinTotMax);     

                //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
                initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

                // ����� �Ϻν����ɿ� �´� EndPin combo ����.
                initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

                UpdateData(FALSE);      // ȭ�鿡 ���� �ݿ�.
                return;
            }   

        }

        //son  32K -> No 32K
        if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) // prev Value //son230216_1
        {
            //son PinBlockType�� no 32K�� �Է����� �ʰ� PinTotMax ���� no 32K�� �����ϴ� ���� �ź��Ѵ�.
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
                // ���� SysSet12.m_nPinTotMax�� ������ �����Ѵ�.
                m_combo1.SetCurSel(SysSet12.m_nPinTotMax);  

                //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
                initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

                // ����� �Ϻν����ɿ� �´� EndPin combo ����.
                initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

                UpdateData(FALSE);      // ȭ�鿡 ���� �ݿ�.
                return;
            }   
        }

//son230216_2 begin:
        //--------------------
        // 32K Original K600
        if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG)    
        {
            //son 23.02.17�� �������� 32K Original Ÿ���� U8K_L24K(K600), PIN_TOT_32K_ORG��. �̿��� Ÿ������ �����ϴ� �� �ź��Ѵ�.
            if (nNewPinTotMax != PIN_TOT_32K_U8K_L24K && nNewPinTotMax != PIN_TOT_32K_ORG)
            {
                str.Format("If the PinBlockType is \'32K (Original)\', the PinMode must be  \'32K U 8K, L 24K\'\n");
                out = str;
                str.Format("PinMode(%s) is not applicable.\n", STR_MAX_PIN(nNewPinTotMax));
                out += str;
                errMessageBox(7701, out); //son230217_2

                // ���� SysSet12.m_nPinTotMax�� ������ �����Ѵ�.
                m_combo1.SetCurSel(SysSet12.m_nPinTotMax);  

                //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
                initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

                // ����� �Ϻν����ɿ� �´� EndPin combo ����.
                initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

                UpdateData(FALSE);      // ȭ�鿡 ���� �ݿ�.
                return;
            }
        }   

        //son Org 32K �� �ƴѵ�  U8K_L24K(K600)���� �����ϴ� ���� �ź��Ѵ�.
        if (SysSet13.m_nPinBlockType != PIN_BLK_32K_ORG 
                && (nNewPinTotMax == PIN_TOT_32K_U8K_L24K || nNewPinTotMax == PIN_TOT_32K_ORG))
        {
            str.Format("PinMode(%s) cannot be applied because the pin block is not \'32K (Original)\'.\n", STR_MAX_PIN(nNewPinTotMax));
            out = str;
            str.Format("First, please go to Menu > Map > Set1, and change the Pin Block to \'32K (Original)\'.\n");
            out += str;
            errMessageBox(7701, out); //son230217_2

            // ���� SysSet12.m_nPinTotMax�� ������ �����Ѵ�.
            m_combo1.SetCurSel(SysSet12.m_nPinTotMax);  

            //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
            initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son210215

            // ����� �Ϻν����ɿ� �´� EndPin combo ����.
            initCombo_EndPin(SysSet12.m_nPinTotMax);   //son241007-BSW-9

            UpdateData(FALSE);      // ȭ�鿡 ���� �ݿ�.
            return;

        }
//son230216_2 end

        SysSet12.m_nPinTotMax = nNewPinTotMax;    

//son230221 begin:
        switch(SysSet12.m_nPinTotMax)
        {
            //son230216_2 begin
            case PIN_TOT_32K_U8K_L24K:
                //son K600�� BlockSet1 ��� ���Ѵ�. Off���� ��.  
                //    Lower�� 6 box ���� �ϴµ� ����ϸ� UI �ٲ�� �Ѵ�. (Pin ���� UI 230216 ����  Lower 4�������� ���� ����)
                SysSet12.m_nBlockSet1  = 0; 
                m_check4.SetCheck(0);
                break;
            //son230216_2 end

            case PIN_TOT_16K_U12K_L4K_TRANS:    //son210525
                //son210525 U12K_L4K�� ���� BlockSet1 ON �̾�߸� �Ѵ�.  
                SysSet12.m_nBlockSet1  = 1; 
                m_check4.SetCheck(1);
                break;

            default:
                //son210531 Upper Trans�� BlockSet1  ��� ���Ѵ�. Off ���� ��.
                if(IS_UPPER_TRANS(SysSet12.m_nPinTotMax))    //son210531
                {
                    SysSet12.m_nBlockSet1  = 0; 
                    m_check4.SetCheck(0);
                }
                break;
        }
//son230221 end

        EnableDisable_QDDDSDControl();  //son210514


        //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
        MakePinBlockRangeUserNo();//son210514
        initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
        initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9


    }


    UpdateData(FALSE);      // ȭ�鿡 ���� �ݿ�.    //son210215
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
            //son201218: 32K ����Ÿ�Կ��� Upper2,Upper3, Upper4�� Upper1�� ������
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDUpper2 = BOX_QD;
                m_check8.SetCheck(1); 
                m_check9.SetCheck(0); 
                m_check10.SetCheck(0);      
//son230217 begin:
                //son U8K, 24K�� ���� QD Upper2������ ���� QD�� ���� 
                if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K)  
                {
                    //son QD Upper3, 4�� QD�� ����
                    SysSet12.m_nPinQDUpper3 = BOX_QD;   //son210531 �޺�
                    SysSet12.m_nPinQDUpper4 = BOX_QD;   //son210531 �޺�
                    EnableDisable_QDDDSDControl();      //son210531
                }
//son230217 end
            }
        }
    }
  
    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514   ���⼭ �Ϻν����ɵ� �����ȴ�.
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
        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper�� ����
        SysSet12.m_nPinQDUpper  = BOX_DD;   //son 2:DD
        m_check1.SetCheck(0); 
        m_check2.SetCheck(1);   //son Upper1 DD On 
        m_check3.SetCheck(0); 

        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K ����Ÿ�Կ��� Upper 2, 3, 4�� Upper1�� �������� ����
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDUpper2 = BOX_DD;
                m_check8.SetCheck(0); 
                m_check9.SetCheck(1);   //son Upper2 DD On 
                m_check10.SetCheck(0);      

                //son U8K, 24K�� ����  Upper2������ ���� DD�� ���� 
                if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    //son �׿ܴ� DD Upper3, 4�� DD�� ����
                    SysSet12.m_nPinQDUpper3 = BOX_DD;   //son210531
                    SysSet12.m_nPinQDUpper4 = BOX_DD;   //son210531
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }
  
    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
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
        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper�� ����
        SysSet12.m_nPinQDUpper  = BOX_SD;       //son 3: SD
        m_check1.SetCheck(0); 
        m_check2.SetCheck(0); 
        m_check3.SetCheck(1);     
    
        if (SysSet12.m_nBlockSet1 != 1) //son210531
        {
            //son201218: 32K ����Ÿ�Կ��� Upper 2, 3, 4�� Upper1�� �������� ����
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDUpper2 = BOX_SD;
                m_check8.SetCheck(0); 
                m_check9.SetCheck(0); 
                m_check10.SetCheck(1);      

                //son U8K, 24K�� ���� Upper2������ ���� SD�� ���� 
                if (SysSet12.m_nPinTotMax != PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    //son  Upper3, 4�� SD�� ����
                    SysSet12.m_nPinQDUpper3 = BOX_SD;   //son210531
                    SysSet12.m_nPinQDUpper4 = BOX_SD;   //son210531
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }
  
    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son240516 //son241007-BSW-9
}


//son Manual QD/DD/SD Block Set 
void CModeSet12::OnCheck4()  
{
    //son BlockSet On
    if(m_check4.GetCheck())
    { 
        //son210531 U12K_L4K Upper Trans �� ���� BlockSet1  Off �̾�߸� �Ѵ�. �׷��� On �� �� ����.
        //          ���⵵�� ���� ������.
        if (IS_UPPER_TRANS(SysSet12.m_nPinTotMax)
                ||  OnPinConvertType1 == 1)  //son241007-BSW-9  
        {
            CString str;
            //str.Format("Can't On. BlockSet checkBox in %s mode.\n", STR_MAX_PIN(SysSet12.m_nPinTotMax));
            //errMessageBox(11203, str); //son230217_2

            //son .CON ������ �����ϴ� ���� QD/DD/SD ȥ����� �ϸ� �������� �����ϰ� �ǹǷ� BlockSet1 ������ �ź��Ѵ�. 
            if (OnPinConvertType1 == 1) //son241007-BSW-9 begin
            {
                str.Format("BDL(%s) has .CON file, so you can't turn on this checkBox.\n", SysInfoView01.m_pStrFileBDLName);
                errMessageBox(11203, str); 
            } //son241007-BSW-9 end

            SysSet12.m_nBlockSet1 = 0;  //son ���� ���·� ����. ��������
            m_check4.SetCheck(0);
            UpdateData(FALSE);      // UI�� ���� �ݿ��Ѵ�.
        }

        //son BlockSet ON
        else
        {
            SysSet12.m_nBlockSet1  = 1; 
            m_check4.SetCheck(1);

            MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
            initCombo_LowerStartPin(BOX_QD, SysSet12.m_nPinTotMax, TRUE); //son210514 QD �������� default �Ϻν������� �޺��ڽ��� ����
            initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
            UpdateData(FALSE);      // UI�� ���� �ݿ��Ѵ�.
        }
    }

    //son BlockSet Off
    else
    {
        //son210525 16K_U12K_L4K�� ���� BlockSet1 �̾�߸� �Ѵ�. �׷��� Off �� �� ����.
        if (SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS)    //son210525
        {
            //CString str;
            //str.Format("Can't Off BlockSet checkBox in %s mode.\n", STR_MAX_PIN(SysSet12.m_nPinTotMax));
            //errMessageBox(str); //son230217_2

            SysSet12.m_nBlockSet1  = 1;     //son ���� ���·� ����. ��������
            m_check4.SetCheck(1);
            UpdateData(FALSE);      // UI�� ���� �ݿ��Ѵ�.
        }

        //son BlockSet OFF
        else
        {
            SysSet12.m_nBlockSet1  = 0; 
            m_check4.SetCheck(0);

            MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
            initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);
            initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
        }
    }

    //son QD/DD/SD ��Ʈ���� ��Ȳ�� �°� �� ��Ʈ�ѵ��� Enable Disable�Ѵ�.
    EnableDisable_QDDDSDControl();  //son210514
  
}

//son Lower1 QD
void CModeSet12::OnCheck5()  
{

    //if( SysSet13.m_nPinBlockType==1) { //sylee161108-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_DD) //son190902 32KPIN 
    { 
        //son  �Ϻ� check box�� QD BlockType��  DD�� �����ؾ� �ϹǷ� QD ������ �ź��Ѵ�
        m_check5.SetCheck(0); 
        return;
    }

    //if( SysSet13.m_nPinBlockType==7) { //sylee180221-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //son190902 32KPIN 
    {
        //son  �Ϻ� check box�� QD BlockType��  DD�� �����ؾ� �ϹǷ� QD ������ �ź��Ѵ�
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
            //son201218: 32K ����Ÿ�Կ��� Lower2�� Lower1�� �������� ����
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDLower2 = BOX_QD;
                m_check11.SetCheck(1); 
                m_check12.SetCheck(0); 
                m_check13.SetCheck(0);      

                SysSet12.m_nPinQDLower3 = BOX_QD;   //son210531
                SysSet12.m_nPinQDLower4 = BOX_QD;   //son210531

                //son U8K, 24K�� ����  Lower 5, 6�� QD�� ����
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
            //son201218: 32K ����Ÿ�Կ��� Lower2�� Lower1�� �������� ����
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDLower2 = BOX_DD;
                m_check11.SetCheck(0); 
                m_check12.SetCheck(1); 
                m_check13.SetCheck(0);      

                SysSet12.m_nPinQDLower3 = BOX_DD;   //son210531
                SysSet12.m_nPinQDLower4 = BOX_DD;   //son210531

                //son U8K, 24K�� ���� Lower 5, 6�� DD�� ����
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
            //son201218: 32K ����Ÿ�Կ��� Lower2�� Lower1�� �������� ����
            if (IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))    //son210202
            {
                SysSet12.m_nPinQDLower2 = BOX_SD;
                m_check11.SetCheck(0); 
                m_check12.SetCheck(0); 
                m_check13.SetCheck(1);      

                SysSet12.m_nPinQDLower3 = BOX_SD;   //son210531
                SysSet12.m_nPinQDLower4 = BOX_SD;   //son210531

                //son U8K, 24K�� ���� Lower 5, 6�� SD�� ����
                if (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230217
                {
                    SysSet12.m_nPinQDLower5 = BOX_SD;   //son230217
                    SysSet12.m_nPinQDLower6 = BOX_SD;   //son230217
                }
                EnableDisable_QDDDSDControl();      //son210531
            }
        }
    }
  
    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
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
        //son190902 32KPIN : m_nPinQD3->m_nPinQDUpper2�� ����
        SysSet12.m_nPinQDUpper2  = BOX_QD;  //son Upper2 BoxType : QD
        m_check8.SetCheck(1); 
        m_check9.SetCheck(0); 
        m_check10.SetCheck(0);      
    }  

    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
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
        //son190902 32KPIN : m_nPinQD3->m_nPinQDUpper2�� ����
        SysSet12.m_nPinQDUpper2  = BOX_DD;  //son Upper2 BoxType: DD
        m_check8.SetCheck(0); 
        m_check9.SetCheck(1); 
        m_check10.SetCheck(0);      
    }  

    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
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
        //son190902 32KPIN : m_nPinQD3->m_nPinQDUpper2�� ����
        SysSet12.m_nPinQDUpper2  = BOX_SD;  //son Upper2 BoxType:  SD(3)
        m_check8.SetCheck(0); 
        m_check9.SetCheck(0); 
        m_check10.SetCheck(1);      
    }  

    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son210514 ���⼭ �Ϻν����ɵ� ����
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);     //son210514
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}


//son Lower2 QD
void CModeSet12::OnCheck11()  
{
    //if( SysSet13.m_nPinBlockType==1) { //sylee161108-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_DD) //son190902 32KPIN 
    {
        //son  �Ϻ� check box�� QD BlockType��  DD�� �����ؾ� �ϹǷ� QD ������ �ź��Ѵ�
        m_check11.SetCheck(0); 
        return;
    }
    //if( SysSet13.m_nPinBlockType==7) { //sylee180221-1
    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //son190902 32KPIN 
    {
        //son  �Ϻ� check box�� QD BlockType��  DD�� �����ؾ� �ϹǷ� QD ������ �ź��Ѵ�
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
 


//son 32K_TRANS, 20K_TRANS, 24K_TRANS������ �Ʒ� �ڵ� �̻��.
// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
int CModeSet12::OnPinConvert1() 
{
    CString str; 
    int No,Mok1,Namo,BoxMok2,i,nMaxPin1,nMaxPinHalf1; 
    
    //son sPinBlockRange[10]��  �����ϰ� "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
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
    else {//32k�� ����.  20K, 24K
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

 
    if(SysSet12.m_nPinQDUpper== BOX_SD) {//3: sd    son190902 32KPIN: enum ����
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
        if(nBoxType[i2]== BOX_SD){ //3:sd   son190902 32KPIN: enum ����
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
        if(nBoxType[nStep+1]== BOX_SD){ //3: SD son190902 32KPIN: enum ����
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
        //������ ���� 

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
            if( SysSet13.m_nPinBlockType== PIN_BLK_S100_2_WIRE) { //son190902 32KPIN : enum ���� //son230216_1
                PinMap[2][i]=No+( 2048 + 4096 + 8192 );
            }

            //if( SysSet13.m_nPinBlockType==6) { //sylee171109-1 
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN : enum ����
            {
                if((i>=4096)&&nBoxType[1]== BOX_SD && nBoxType[2]== BOX_SD){//sylee180412-3 // son190902 32KPIN: enum ����   3->BOX_SD
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

        //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
        int hwPin = PinMap[2][i];					//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)			//son230303
            nJigPinMap[hwPin] = i;    //son ���� �˻���. //son230303

        /////////////////////////////////////////////////////////////////////////////
    }//for( i=0; i< 33000; i++)



    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   //son210514 200 -> FILE_PATH_SIZE

    ::ZeroMemory(&fName, sizeof(fName));
    //strcat( fName , "d:\\PinMap.txt" ); 
    strcat( fName , "d:\\log\\PinMap.txt" ); //son220715 d:\\log ������ �̵�

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

    //son190902 32KPIN : m_PinQD-> m_nPinQDUpper�� ����
    //                   m_nPinQD2->m_nPinQDLower�� ����
    //                   m_nPinQD3->m_nPinQDUpper2�� ����
    //                   m_nPinQD4->m_nPinQDLower2�� ����
    nBoxType[1]=SysSet12.m_nPinQDUpper;      
    nBoxType[2]=SysSet12.m_nPinQDUpper2;         
    nBoxType[3]=SysSet12.m_nPinQDLower;      
    nBoxType[4]=SysSet12.m_nPinQDLower2; 

    int nBoxPinSumOld;
    nBoxPinSum=0;
    nBoxPinSumOld=0;

    for(int i2=1; i2<=4; i2++)
    {
        if(nBoxType[i2]== BOX_SD) {//3: sd      son190902 32KPIN: enum ����
            nBoxPinSum=nBoxPinSum+nBoxPinHalf_SS;//SYLEE150912-1 
        }
        else  if(nBoxType[i2]== BOX_DD){ // 2: dd//SYLEE150912-1    son190902 32KPIN: enum ����
            nBoxPinSum=nBoxPinSum+nBoxPinHalf;  //SYLEE150912-1                      
        }
        else {//dd, //qd
            nBoxPinSum=nBoxPinSum+nBoxPinFull;
        }   

        //////////////////////////////////////////////////
        if(SysSet12.m_nPinTotMax == PIN_TOT_16K)//16k
        {
            //sylee151102   ���  sd sd�ΰ��  �Ϻν��۹�ȣ  2k+1�� �ƴ϶� 4k+1  
            if(nBoxType[1]== BOX_SD && nBoxType[2]== BOX_SD)  //son190902 32KPIN: enum ����      3-> BOX_SD
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

        if(nBoxType[nStep+1]== BOX_DD)      //sylee150912-1 //son190902 32KPIN: enum ����      2-> BOX_DD
        {
            Namo=No%64;
            Mok1=Namo/32;
            No=No*2;
            if(Mok1==1){
                No++;
            }         
        }
        else  if(nBoxType[nStep+1]== BOX_SD)//sylee150912-1  SD //son190902 32KPIN: enum ����      3-> BOX_SD
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
        //������ ���� 
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

        //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
        int hwPin = PinMap[2][i];				//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
            nJigPinMap[hwPin] = i;    //son ���� �˻���. //son230303

    }//for( i=0; i< 33000; i++){


    return 1;

}




//sylee150530======================================

//son nPinMap[] �����ϰ� D:\\PinMap.txt ���Ͽ� write
//
//son nMode:  OnPinConvertType1�� .CON ������ �ǹ������� 1 (total�� 1�̻�, checksum ����)
int CModeSet12::OnPinConvert1_READ(int nMode)    //ACE400  //sylee151111add
{
    CString str; 
    int No,Mok1,Namo,BoxMok2,i;
    int nMaxPin1;                   //son Total Pin Num
    int nMaxPinHalf1;               //son nMaxPin1�� 1/2 
    int nTotal1; 
    int nLoweStartPin1;
    int nBoxPinFull;                //son QD�� BoxPin�� (SysSet12.m_nPinBoxMax 4K�� 4K, 2K�� 2K)
    int nBoxPinHalf;                //son DD�� BoxPin�� (QD�� 1/2) 
    int nBoxPinHalf_SS;             //son SD�� BoxPin�� (QD�� 1/4) 
    int nBoxPinSum;             
    int nStep;
    int nBoxPin[10];                //son Box�� ������ PinNo

    int nBoxType[10];               //son SysSet12.m_nPinQDUpper ~ m_nPinQDLower2 (Upper1,Upper2,Lower1,Lower2)
                                    //son 1=QD, 2=DD, 3=SD
                                    //son nBoxType[1] : Upper           
                                    //son nBoxType[2] : Upper2
                                    //son nBoxType[3] : Lower           
                                    //son nBoxType[4] : Lower2          

    //son sPinBlockRange[10]��  �����ϰ� "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
    //MakePinBlockRangeUserNo();//sylee210402       //son 220422 ���� PinBlockRange�� ���� �ڵ忡�� ���� ����ϰ� ���� �����Ƿ� �ڸ�Ʈó��
                                                    //    PinMap�� pinBlockRange�� �����ϰ� ����� �ִ�. 

    nLoweStartPin1=0;//sylee151112

    //======================================
    //son .CON ������ �ǹ� ���ٸ�
    if(nMode==0)//sylee151111add
    {

        //----------------------
        // nMaxPin1 ����
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
        // nBoxPinFull, nBoxType[] ����  
        
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

        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper�� ����
        //                   m_nPinQD2->m_nPinQDLower�� ����
        //                   m_nPinQD3->m_nPinQDUpper2�� ����
        //                   m_nPinQD4->m_nPinQDLower2�� ����
        nBoxType[1]=SysSet12.m_nPinQDUpper;         //son Upper1 ex) 1  (QD)     
        nBoxType[2]=SysSet12.m_nPinQDUpper2;        //son Upper2 ex) 1  (QD) 
        nBoxType[3]=SysSet12.m_nPinQDLower;         //son Lower1 ex) 1  (QD) 
        nBoxType[4]=SysSet12.m_nPinQDLower2;        //son Lower2 ex) 1  (QD) 

        //if(SysSet13.m_nPinBlockType==6){//sylee180413-1                 
        int box; //son190902 32KPIN:  k2-> box�� ����    
        if(SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) //son190902 32KPIN 
        {
            for(box=1; box <= 4; box++){            //son 4: box ����
                if(nBoxType[box] == BOX_DD) {       //son 2: DD
                    nBoxType[box] = BOX_QD;         //son 1: QD
                }
            }
        }
        int nBoxPinSumOld;
        nBoxPinSum=0;
        nBoxPinSumOld=0;

        //-------------------------------------------------------------
        //son nBoxPin[box] ���� : box�� �� �� pinNo ��ġ�� �˾Ƴ���.
        for(box=1; box <= 4; box++) //son190902 32KPIN:  i2-> box�� ����
        {
            //son ���� ���̽� ------
            //if( SysSet13.m_nPinBlockType!=6)  //SYLEE180413-5 
            if( SysSet13.m_nPinBlockType!= PIN_BLK_4K_DD_421)  //son190902 32KPIN 
            {
                if(nBoxType[box] == BOX_SD){//sd (3)    //son190902 32KPIN: enum ����
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf_SS;//SYLEE150912-1 
                }
                else  if(nBoxType[box] == BOX_DD) {//dd(2) //SYLEE150912-1 //son190902 32KPIN: enum ����
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf;  //SYLEE150912-1                      
                }
                else { //qd
                    nBoxPinSum=nBoxPinSum+nBoxPinFull;
                }
            }

            //son PIN_BLK_4K_DD_421 ���̽� --------
            else{//SYLEE180413-5

                if(nBoxType[box] == BOX_SD) {//Sd(3) //SYLEE180413-5 //son190902 32KPIN: enum ����
                    nBoxPinSum=nBoxPinSum+nBoxPinHalf;   //SYLEE180413-5 
                }
                else {//DD  //qd
                    nBoxPinSum=nBoxPinSum+nBoxPinFull;//SYLEE180413-5 
                }
            }


            if(SysSet12.m_nPinTotMax == PIN_TOT_16K)//16k               
            {
                //son ��� ������ box �� ��, 
                if(box==2)
                {   
                    //sylee151102   ���  sd sd�ΰ��  �Ϻν��۹�ȣ  2k+1�� �ƴ϶� 4k+1    
                    
                    if(nBoxType[1]== BOX_QD || nBoxType[2]== BOX_QD){   //son190902 32KPIN: enum ����    1-> BOX_QD
                        nBoxPinSum=4096*2;
                    }
                    else  if(nBoxType[1] != BOX_SD && nBoxType[2] != BOX_SD){   //son190902 32KPIN : enum ���� 3-> BOX_SD
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
        //son PinMap[0], PinMap[1], PinMap[2]�� ����� ����.

        int pin;
        for (pin=0; pin<nMaxPin1; pin++)//SYLEE121003
        {

            //-------------------------
            //son PinMap[0][pin] ����
            No=pin; 
            PinMap[0][pin]=pin;//****>net list pin no   


            //-------------------------
            //son PinMap[1][pin] ����
            
            nStep=0;
            //if( (SysSet13.m_nPinBlockType!=7) ||  ((SysSet13.m_nPinBlockType==7)&&(No<nBoxPin[2]))  ){//sylee180322-1  
            if( (SysSet13.m_nPinBlockType!= PIN_BLK_12K_QD_DD) 
                    ||  ((SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) && (No<nBoxPin[2]))  ) //son190902 32KPIN 
            {
                //son ���� pin�� �ش��ϴ� box ��ġ(nStep)�� ã�´�.
                for(int i3=1; i3<=4; i3++){         //son ??? 
                    if((No+1)>nBoxPin[i3]){//sylee150916    
                        nStep=i3;                            
                    }
                    else{//sylee150916  
                        break; 
                    } //sylee150916 
                }           

                No=No-nBoxPin[nStep];   //son �ڽ� ���� pinNo�� ���� (Offset�� ����)

                if(nBoxType[nStep+1]== BOX_DD) //sylee150912-1    DD(2) //son190902 32KPIN: enum ����
                {
                    Namo=No%64;         //son col ��ġ (0~63) 
                    Mok1=Namo/32;
                    No=No*2;
                    if(Mok1==1){        //son col��ġ�� 0~31�̸� 
                        No++;
                    }         
                }
                else if(nBoxType[nStep+1] == BOX_SD)    //sylee150912-1  SD(3) //son190902 32KPIN: enum ����
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

                //son ������ boxPinNo��  QD pin ���� pinNo�� ��ȯ
                No=(nBoxPinFull*nStep)+No;  

            }

            //son PinBlockType�� PIN_BLK_12K_QD_DD �̰� �Ϻ��� �� ��� DD�� �����ؾ� ��.
            else //sylee180322-1 //((SysSet13.m_nPinBlockType==7)&&(No>=nBoxPin[2])) 
            {


                for(int i3=1; i3<=4; i3++){     //son ??? 
                    if(No>nBoxPin[i3]){
                        nStep=i3;
                        break;
                    }
                }

                No=No-nBoxPin[nStep];
                if(nBoxType[nStep+1] == BOX_SD){    //SD(3) //son190902 32KPIN: enum ����
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
            //son PinMap[2][pin] ����

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
            //son Reaverse A ó��
            
            //son Card ������ reverse�� ������ pin ��ȣ ���ġ

            int BoxN;
            if(SysSet12.m_nPinBoxMax==1){  //2k
                //sylee120925
                if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                    BoxN=No/2048;           //son boxNo
                    No=No%2048;             //son boxPinNo
                    Mok1=No/128;            //son cardNo (0~15 card), 1�� card�� 128pin
                    Namo=No%128;            //son cardPinNo (0~127)
                    No=Namo+((15-Mok1)*128);//son newBoxPinNo = cardPinNo + ((15-cardNo)*128)
                    No=No+BoxN*2048;        //son newPinNo = newBoxPinNo + boxNo*2048
                }
            }
            else{
                if(SysSet12.m_nCardReverse ==2){  //2=Card Reverse
                    BoxN=No/4096;           //son boxNo
                    No=No%4096;             //son boxPinNo
                    Mok1=No/128;            //son cardNo (0~31 card), 1�� card�� 128pin 
                    Namo=No%128;            //son cardPinNo (0~127)
                    No=Namo+((31-Mok1)*128);//son newBoxPinNo = cardPinNo + ((31-cardNo)*128)
                    No=No+BoxN*4096;        //son newPinNo = newBoxPinNo + boxNo*4096
                }
            }

            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

            //********************************************************************************************* 
            //������ ���� 
            
            //----------------------
            //son Reaverse B ó��

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
            //son Lower StartPin(ntmp1) ��� 

            int ntmp1;//sylee151006 //ACE400        
            ntmp1=nBoxPin[2];//sylee151006 //ACE400     //son LowerStartPin ex) 4096

            if(SysSet12.m_nPinTotMax == PIN_TOT_4K){//4k //sylee161108-1
                if(ntmp1>2048){
                    ntmp1=2048;         //son ex) ntmp1 <= 2048�� ����
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
                    ntmp1=(8192*2); //son 16K�� ����
                }
            }

            //if( SysSet13.m_nPinBlockType==6) { //sylee161110-1  //sylee180413-1
            if( SysSet13.m_nPinBlockType== PIN_BLK_4K_DD_421) { //son190902 32KPIN 
                ntmp1=4096;     //son 8K�� ����
            }

            SysSet12.m_nLowerJigStartPin = ntmp1 +1;   //son220402 �Ϻν������� ����

            
            //-------------------------------------------------------------------------
            //son ���� pin��ȣ��  Lower Pin �̶�� PinNo�� �Ϻ��ɱ������� �ٽ� ���
            if(pin>=ntmp1)//sylee151006 //ACE400
            {
                //son Jig�� PinTot�� 4K�̸� ��ܿ� 2K, �ϴܿ� 2K-------------------------
                if(SysSet12.m_nPinTotMax == PIN_TOT_4K)//4k  ///sylee151006 //ACE400
                {
                    //son SwitchCard(HW) pin�� ��� 16K, �ϴ� 16K �غ�Ǿ����� 14K�� �̻���̴�. 
                    //son SwtichCard(HW) ������ LowerStartPin�� ���� 14K�� �ǳʶٰ� ��ȣ�� �Ҵ��ؾ� �Ѵ�.
                    //son Offset 14K�� ���Ѵ�.
                    PinMap[2][pin]=No+( 2048 + 4096 + 8192 );//****>SWITCH CARD pin no   //sylee151006 //ACE400 
                }


                //son PinTot�� 8K�̸� ��ܿ� 4K, �ϴܿ� 4K.  (12K�� ���ؼ� 16K�� ���� )----------------
                else  if(SysSet12.m_nPinTotMax == PIN_TOT_8K){//8k
                    //son LowerStartPin Offset 12K�� ���Ѵ�.
                    PinMap[2][pin]=No+( 4096 + 8192   );//****>SWITCH CARD pin no  //sylee151006 //ACE400
                }


                //son PinTot�� 16K�̸� ��ܿ� 8K, �ϴܿ� 8K.  (8K ���ؼ� 16K�� ����) ----------------
                else if(SysSet12.m_nPinTotMax == PIN_TOT_16K)//16k
                {
                    //son LowerStartPin Offset 8K�� ���Ѵ�.
                    PinMap[2][pin]=No+( 8192 );//****>SWITCH CARD pin no  //sylee151006 //ACE400

                    //son PinTot�� 16K������ ������ 12K ���� (��� QD 4K *2, �Ϻ� DD 2K *2 -> 12K) �� ���
                    if( SysSet13.m_nPinBlockType== PIN_BLK_12K_QD_DD) //SYLE190403 //son190902 32KPIN 
                    {
                        //son Upper1, Upper2�� ��� DD. ��� 8K�߿� ���� 4K �߰��� �̻��??
                        if((nBoxType[1]==BOX_DD)&&(nBoxType[2]==BOX_DD)){       //son190902 32KPIN: enum ����
                            PinMap[2][pin]=PinMap[2][pin]+2048;     //son !!! �߰� 4K ���ؾ� ���� �ʳ�?
                        }

                        //son Upper1, Upper2�� ��� SD 
                        else if((nBoxType[1]==BOX_SD)&&(nBoxType[2]==BOX_SD)){  //son190902 32KPIN: enum ����
                            PinMap[2][pin]=PinMap[2][pin]+2048;     //son !!! �߰� 6K ���ؾ� ���� �ʳ�?
                        }
                    }
                }

                //son 32K�� �̻����� �����Ƿ� No�� �״�� ���.----------------
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

            //son  ���..
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
                //son  64pin Card ������ pinNo�� 128pin Card ������ pinNo�� ����
                No=PinMap[2][pin];
                if(No<16384){           
                    Namo=No%64;         //son 64pin Card������  cardPinNo
                    Mok1=No/64;         //son cardNo
                    No=Mok1*128+Namo;   //son 128pin Card ������ pinNo
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

            //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
            int hwPin = PinMap[2][pin];					//son230303
            if (hwPin >= 0 && hwPin < MAX_PIN)			//son230303
                nJigPinMap[hwPin] = pin;    //son ���� �˻���. //son230303


        }//for( pin=0; pin< 33000; pin++)


    }//     if(nMode==0){//sylee151111add

 
//===========================================================================



 
    //SYLEE180413-1   if( SysSet13.m_nPinBlockType==6){ //sylee171109-1 
    //SYLEE180413-1     nMode=1;
    //SYLEE180413-1  }   

    //===========================================================================
    //son nMode== OnPinConvertType1 ==1 �� ���  (.Con ������ ����ؾ� �Ѵٸ�)
    //son CChildView1::OnDataLoad_READ_ConRead() ���� .CON ������ �о 
    //son ������  nPinC1[DEF_MAX_PIN2][2] �� �����Ѵ�. �� ������ �������� PinMap�� �����Ѵ�.

    if(nMode==1)//sylee151111add
    {

        //son nPinC1[0][0]: .CON ������ �� �ɼ� �� range�� üũ�Ѵ�.
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

        //son190902 32KPIN : m_PinQD-> m_nPinQDUpper�� ����
        //                   m_nPinQD2->m_nPinQDLower�� ����
        //                   m_nPinQD3->m_nPinQDUpper2�� ����
        //                   m_nPinQD4->m_nPinQDLower2�� ����
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
        //son nBoxPin[box] ���� : box�� �� �� pinNo ��ġ�� �˾Ƴ���.
        
        for(box=1; box<= 4; box++)
        {
            //son ���� ���̽� ------
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
            //son PIN_BLK_4K_DD_421 ���̽� --------
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
        //son nPinC1[0][0]: .CON ������ �� �ɼ� �� range�� üũ�Ѵ�.
        if( (nPinC1[0][0]>0) && (nPinC1[0][0]<= MAX_PIN)){  //sylee151111
            nTotal1=nPinC1[0][0];   
        }else{   
            nTotal1=0;   
        }

        //-------------------------------------------------
        //son PinMap[0], PinMap[1], PinMap[2]�� ����� ����.
        int swPin, conJigPin;  //son220402
        for(i=1;i<=nTotal1;i++) //sylee151111add
        {
            //son nPinC1[0] : .CON ���� ���
            //son           nPinC1[0][0]: .CON ������ �� pin ��,  nPinC1[0][1]: .CON ������ checkSum 
            //
            //son nPinC1[1]~nPinC1[DEF_MAX_PIN2-1]������ data
            //son           nPinC1[i][0]: pinBlockPinNo     nPinC1[i][1]: ����� pinNo       
            //son .CON ������ ���ٸ� jigPin�� pinBlockPin�� �����ϰ�  BDL ������ pinNo�� READ file�� pinNo�� ����. 

            //son .CON ������ jigpin�� (�ι�°�׸�) �׿� �´� PinBlockPin(ù��° �׸�)�� ���� �´�.
            swPin    = nPinC1[i][0]-1;  // pinBlock no  (1~  //sylee151111add
            conJigPin= nPinC1[i][1]-1;  // jig no  (1~  //sylee151111add    ''  //son �����(Jig) pin no 


            //----------------------------------
            //son PinMap[0][pin] ����  (conJigPin)

            PinMap[0][conJigPin]= conJigPin;   // //sylee151111addi    net list pin no


            //------------------------------------
            //son PinMap[1][pin] ���� (Qd pin No)
            
            if(conJigPin<16384){//16k    i=0~16k  //sylee151111add
                No=swPin;   //nSwitchMaxPin  33000    ,  pin no   0~
            }else{  //sylee151111
                No=swPin-8192;  //32k->16k  //nSwitchMaxPin  33000    ,  pin no   0~
            }

            //son220402 PinMap[1][i]=No;//****>QD pin no //pin block  no    //son!!!!! pinMap[1][conJigPin] = No ���� ���� �ʴ���???
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
            //������ ���� 

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
            //son Lower StartPin(ntmp1) ��� 

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
            SysSet12.m_nLowerJigStartPin = ntmp1 +1;   //son220405 �Ϻν������� ����

            //-------------------------------------------
            //  �Ϻν����� offset�� ���� hwPin�� ����
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
                if(No<16384){    //son ���
                    Namo=No%64;
                    Mok1=No/64;
                    No=Mok1*128+Namo;                    
                }
                else{    //son �Ϻ�
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

            //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
            int hwPin = PinMap[2][conJigPin];		//son230303
            if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
                nJigPinMap[hwPin] = conJigPin;    //son ���� �˻���. //son230303

        }//for( i=0; i< 33000; i++){


    }//     if(nMode==1){//sylee151111add


//===========================================================================
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  //son210514   200 -> FILE_PATH_SIZE
    CString strTemp;

    ::ZeroMemory(&fName, sizeof(fName));

    //son230825 BlockType�� SD ����(s100)�̸� boxType�� QD�� ���� ������� �����ؼ� QD ����ǥ�ø� ����.
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
       strTemp.Format("d:\\log\\PinMap_%s_%s_%d_%s_%s.csv", //son220715 d:\\log ������ �̵�
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



//son jigPin0���� Block 1 ������ nPin1������ �ٲٰ�  m_nBlockPo1(���� block��ġ)���� ȹ���Ѵ�. //son210514
int CModeSet12::GetBlockNo_PinNo_32K( int &nPin1,      int &nBlock1,    //sylee210402  input&output, output2
                                  int &nBlockType, int &nHwBoxType) //son210514    output3, output3
{
    //nPin1  0~ 
    int nEnd;

    nBlockType = -1;    //son210514
    nHwBoxType = -1;    //son210514

    //son Upper 4 box�� ���� ��� range check
    if(IS_UPPER_12K( SysSet12.m_nPinTotMax)) //son210514
    {
        //son Upper 12K�ε� ���  4 box �϶��� -1�� ó��
        if( (nPin1<(sPinBlockRange[5].nStartPin-1)) &&  (nPin1>=sPinBlockRange[3].nEndPin)){ 
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }

    }

    //son Upper 3, 4 box�� ���� ���
    if(IS_UPPER_8K( SysSet12.m_nPinTotMax)) //son210514
    {
        //son Upper 8K�ε� ��� 3, 4 box �϶��� -1�� ó��
        if( (nPin1<(sPinBlockRange[5].nStartPin-1)) &&  (nPin1>=sPinBlockRange[2].nEndPin)){ 
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }

    }
    //son Upper 2, 3, 4 box�� ���� ���
    if(IS_UPPER_4K(SysSet12.m_nPinTotMax))  //son210514
    {
        //son Upper 4K�ε� ��� 2, 3, 4 box �϶��� -1�� ó��
        if( (nPin1<(sPinBlockRange[5].nStartPin-1)) &&  (nPin1>=sPinBlockRange[1].nEndPin)){ 
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }
    }

    //son Lower 7, 8 box�� ���� ���
    if(IS_LOWER_8K(SysSet12.m_nPinTotMax)) //son210514
    {
        //son Lower 8K�ε� �Ϻ� 7, 8 box �϶� -1�� ó��
        if( nPin1>(sPinBlockRange[6].nEndPin-1)){//sylee210423   
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }           
    }


    //son Lower 6, 7, 8 box�� ���� ���
    if(IS_LOWER_4K(SysSet12.m_nPinTotMax))    //son210514
    {
        //son Lower 4K�ε� �Ϻ� 6, 7, 8 box �϶� -1�� ó��
        if( nPin1>(sPinBlockRange[5].nEndPin-1)){//sylee210423
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }           
    }


    //son ��� box4�� endPin���� ũ�ų�  �Ϻ� box5�� startPin���� ���� pin�� -1 ó��
    if ( !IS_UPPER_4K(SysSet12.m_nPinTotMax) && !IS_UPPER_8K(SysSet12.m_nPinTotMax)  //son210514 12K�̰ų� 8K�� ���� 4��° endPinüũ �Ұ�. 
                                            && !IS_UPPER_12K(SysSet12.m_nPinTotMax) ) //son210525
    {
        if (sPinBlockRange[4].nEndPin != 0) //son241007-BSW-9 Box 4 skip�̸� Box 4 Endpin�� 0�� �� �����Ƿ� �� range üũ�� �ǹ� ����
        {
            if (nPin1<(sPinBlockRange[5].nStartPin-1) && (nPin1>=sPinBlockRange[4].nEndPin)) //sylee210423
            {
                nPin1=-1;   nBlock1=-1;         
                return 1;
            }
        }    
    }

    //son �Ϻ� box8�� endPin���� ū pin�� -1 ó��
    if ( !IS_LOWER_4K(SysSet12.m_nPinTotMax) && !IS_LOWER_8K(SysSet12.m_nPinTotMax) ) //son210514 12K�̰ų� 8K�� ���� 8��° endPinüũ �Ұ�. 
    {
        if( nPin1>(sPinBlockRange[8].nEndPin-1)){//sylee210423
            nPin1=-1;   nBlock1=-1;         
            return 1;
        }
    }


    //son �Էµ� nPin���� �´� hwPin(nPin)���� �ش� Block�� ã�Ƽ� �����Ѵ�.
    for(int nStep=1; nStep<=8; nStep++)
    {
        //son Upper 4 box�� ���� ��� search�� ������ �ϱ� ���� �ٷ� continue
        if (IS_UPPER_12K(SysSet12.m_nPinTotMax))       //son210525 
        {
            if (nStep==4)
                continue; 
        }

        //son Upper 3, 4 box�� ���� ��� 
        //if( SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS ) //son230216_1
        if (IS_UPPER_8K(SysSet12.m_nPinTotMax))       //son210514 
        {
            if (nStep==3 || nStep==4)
                continue; 
        }

        //son Upper 2, 3, 4 box�� ���� ��� 
        if (IS_UPPER_4K(SysSet12.m_nPinTotMax))       //son210514 
        {
            if (nStep==2 || nStep==3 || nStep==4)
                continue; 
        }

        //son Lower 7, 8 box�� ���� ���
        if (IS_LOWER_8K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==7 || nStep==8 )  
                continue; 
        }


        //son Lower 6, 7, 8 box�� ���� ���
        if (IS_LOWER_4K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==6 || nStep==7 || nStep==8 )  
                continue; 
        }

        if (sPinBlockRange[nStep].nBlockType == BOX_SKIP) //son241007-BSW-9
            continue;

        nEnd=sPinBlockRange[nStep].nEndPin;
        if(nPin1<nEnd)  //son �ش� block�� ã����
        {
            nBlock1    = nStep;
            nBlockType = sPinBlockRange[nBlock1].nBlockType;    //son210514
            nHwBoxType = sPinBlockRange[nBlock1].nHwBoxType;    //son210514

            if(nStep!=1)
            {
                if(nStep!=5){
                    //son �ٷ� ���� block�� EndPin ���� ���� ���� startPin�� 1�� �����.
                    nPin1=nPin1-(sPinBlockRange[nStep-1].nEndPin);  //nPin1  1~
                }
                else{
                    //son step�� 5�̸� �տ� skip pinBlock�� ���� ���� �����Ƿ� 
                    //    Start��ȣ�� ���� ù��° block�� pin������ �ٲ۴�.
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

//son PinMap2[][0], PinMap2[][1], PinMap2[][2]���� ����Ѵ�.
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




//son190902 32KPIN: �űԻ��� �Լ� OnPinConvert1_32K()
//
//son nPinConvertType:  .CON ������ nPinC1[][]�� �ε�Ǿ�����  1, �ƴϸ� 0 
int CModeSet12::OnPinConvert1_32K(int nPinConvertType)   //son190902 32KPIN 
{

    int nMaxPin1;   //son PinConvert() for ���� ���� max��
 
    memset(PinMap, -1, sizeof(PinMap)); //son  �ʱ�ȭ
    memset(nJigPinMap, -1, sizeof(nJigPinMap)); //son230303

    //son sPinBlockRange[10]��  �����ϰ� "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
    MakePinBlockRangeUserNo();//sylee210402   ���⼭ �Ϻν����ɵ� ����
    

    //--------------------------------------------------------------------------
    //son �Ϻν����� ���� :  �Ϻν����� ���� ������� ���� ���� ��ġ�� ���Ѵ�. 
    //                       QDType, MaxPin�� ����  LowerStartPin�� ���� �ʴ� ��쿡 �����Ѵ�. 
    //                       �ݵ�� QD�� MaxPin �ʱ�ȭ�� �Ϸ�� ������ ȣ���ؾ� �Ѵ�.
    //
    //    OnPinConvert1_32K()�� �����Լ�ó�� ȣ��ǹǷ� �Ϻν����� �޺��ڽ��� ���⼭ ������ �� ����
    //    initCombo_LowerStartPin()�� ���߿� Pin ���� UI �����ÿ� �����Ѵ�.
                    //son210525 LoadSaveSet12(_LOAD)���� OnPinConvert_32K()�� �̵�.
                    //          MakePinBlockRangeUserNo()���� PinBlockRange������ ���Ŀ� ȣ��.
    repair_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax);  //son210115 �߰� //son241007-BSW-9 


    if (!IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax))  //son210826
        return -1;

    //son Upper 12K Trans, Lower 4K Trans
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS)   //son210826
    {
        //son210525 U12K_L4K Ÿ���� �Ϻν������� ������� �ʰ� QD/DD/SD Block Set�� default �����Ѵ�.
        SysSet12.m_nBlockSet1 = 1;      //son210525 ��������

        //son Pin����ȭ�� Init �ϱ� ���� OnPinConvert1_32K() ȣ�� �Ǹ� �Ʒ��ڵ忡�� ����. ����. //son210525
        //m_check4.SetCheck(1);           //son210525
        //GetDlgItem(IDC_COMBO_LOWER_START_PIN)->EnableWindow(FALSE);             //son210525
        //UpdateData(FALSE);        // UI�� ���� �ݿ��Ѵ�.  //son210525
    }

    //son nMaxPin1 ����
    // 20K��, 24K �� ���� 1~ 32768���� ��� swPIn�� ���� hwPin ���� ������ Ȯ���ؾ� �Ѵ�. 
    // �ǹ� ���� �����̶� -1�� ä����� �ϹǷ�  nMaxPin1 ���� 32768�� �����Ѵ�. 
    nMaxPin1 = MAX_BOX_PIN * MAX_BOX;   // 4096 *8 : 32K //son-32KPIN-20200114 : 
    

    //======================================
    //son .CON ������ �ǹ� ���ٸ�
    int jigPin0, nBlockType, nHwBoxType;
    int nPin1;//sylee210402
    int hwPin;  //son230303
    if(nPinConvertType==0)  //son .CON ���ٸ�
    {
        for (jigPin0 = 0; jigPin0 < nMaxPin1; jigPin0++)//SYLEE121003
        {
            //--------------------- 
            //son PinMap[0][jigPin0] ����
            PinMap[0][jigPin0]=jigPin0; // ****> net list pin no 


            //---------------------
            //son PinMap[1][jigPin0] ����
    
            if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
            {
                if (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG && (jigPin0 +1) > SysSet12.m_nEndPin) //son241007-BSW-9
                    break;

                nPin1=jigPin0;  //sylee210402
                m_nBlockPo1=0;  //sylee210402 
                nBlockType = BOX_QD;        //son210514 �ʱ�ȭ
                nHwBoxType = TYPE_AC;       //son210514 �ʱ�ȭ
                //if(nPin1==16385){
                //  nPin1=16385;
                //}

                //son jigPin0���� Block 1 ������ nPin1������ �ٲٰ�  m_nBlockPo1(���� block��ġ)���� ȹ���Ѵ�.
                GetBlockNo_PinNo_32K(nPin1, m_nBlockPo1, nBlockType, nHwBoxType);  //sylee210402 //son210514

                if(nPin1>=0){
                    //son Block1������ nPin������ hwPin�� ���� ���� 
                    //    MAX_BOX_PIN * (m_nBlockPo1-1) �� ���ؼ� ���� hwPin���� ���Ѵ�.
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
            //son PinMap[2][jigPin0] ����
            
            // Todo: �߰� ���� �� ���缭 pin ���� �ٽ� ���ġ�ؾ� ��.
            //      PinSwitchMode(Wire type), ReverseA, ReverseB, 
            //      PIN_BLK_SD�̸� 128 ī��� ���� �ٲٱ� ���� �߰� ���.
            
            hwPin = PinMap[2][jigPin0] = PinMap[1][jigPin0];	//son230303

            //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
            if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
				//son ���� �˻���. make32kSwPin(hwPin)�� pinMap ������ ������
                nJigPinMap[hwPin] = jigPin0;    	//son230303
        }
    }

//  if( SysSet12.m_nBlockSet1 == 1 ){ //sylee210402 
//      GetBlockNo_PinNo_fileLog();   //sylee210402 
//  }

    //====================================
    //son Con ������ ����ؾ� �Ѵٸ�
 
    //son CChildView1::OnDataLoad_READ_ConRead() ���� .CON ������ �о 
    //son ������  nPinC1[DEF_MAX_PIN2][2] �� �����Ѵ�. �� ������ �������� PinMap�� �����Ѵ�.
    int nConTotal, jigPin, swPin, conJigPin, conIdx; 
    if(nPinConvertType==1)
    {
        //son nPinC1[0][0]: .CON ������ �� �ɼ� �� range�� üũ�Ѵ�.
        if (nPinC1[0][0] > 0 && nPinC1[0][0] <= MAX_PIN)  //sylee151111
            nConTotal = nPinC1[0][0];   
        else   
            nConTotal = 0;      

        for (conIdx=1; conIdx <= nConTotal; conIdx++)
        {
 
            //son nPinC1[0] : .CON ���� ���
            //son           nPinC1[0][0]: .CON ������ �� pin ��,  nPinC1[0][1]: .CON ������ checkSum 
            //
            //son nPinC1[1]~nPinC1[DEF_MAX_PIN2-1]������ data
            //son           nPinC1[i][0]: pinBlockPinNo     nPinC1[i][1]: ����� pinNo       
            //  
            //son .CON ������ ���ٸ� jigPin�� pinBlockPin�� �����ϰ�  BDL ������ pinNo�� READ file�� pinNo�� ����. 
            
            
            //son .CON ������ jigpin�� (�ι�°�׸�) �׿� �´� PinBlockPin(ù��° �׸�)�� ���� �´�.
            swPin     = nPinC1[conIdx][0]-1;  // PinBlock PinNo  (1~ )                 //son make32kHwPin()���� ����� swPin
            conJigPin = nPinC1[conIdx][1]-1;  // con Jig PinNo  (1~ ) : �����(Jig) pin no //son BDL���� ���Ǵ� con JigPin   

            //----------------------------
            //son PinMap[0][conJigPin] ���� : jigPin 
            PinMap[0][conJigPin] = conJigPin;   // ****> net list pin no    //son210416 .CON �� ������ PinMap ��� ��� ����

            //----------------------------
            //son PinMap[1][conJigPin] ���� 
            
            //son pin���� �´� HwPin(SwitchCard pin) ���� ����.
            //son conJigPin-> pinBlockPin1_�ӽ�(swPin)-> HwPin(pinBlockPin2:����) ���� ����.
            PinMap[1][conJigPin] = make32kHwPin(swPin); //****> QD pin no (QD, DD, SD ������ �ݿ��� pinNo)


            //----------------------------
            //son PinMap[2][jigPin] ����
            
            // Todo: �߰� ���� �� ���缭 pin ���� �ٽ� ���ġ�ؾ� ��.
            //      PinSwitchMode(Wire type), ReverseA, ReverseB, 
            //      PIN_BLK_SD�̸� 128 ī��� ���� �ٲٱ� ���� �߰� ���.
            hwPin = PinMap[2][conJigPin] = PinMap[1][conJigPin];    //son210416 .CON �� ������ PinMap ��� ��� ���� //son230303

            //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
            if (hwPin >= 0 && hwPin < MAX_PIN)  //son230303
                nJigPinMap[hwPin] = conJigPin;	//son230303
        }

    }

    //============================================
    //son PinMap File ����   (jigPin -> hwPin)
    //============================================
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  //son210514   200 -> FILE_PATH_SIZE
    ::ZeroMemory(&fName, sizeof(fName));

    CString strTemp;

    if( SysSet12.m_nBlockSet1 == 1 ){ //sylee210402 
           strTemp.Format("d:\\log\\PinMap_%s_%s_%d_%d__%s_%s_%s_%s__%s_%s_%s_%s.csv",     //son220715 d:\\log ������ �̵�
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
           strTemp.Format("d:\\log\\PinMap_%s_%s_%d_%d_%s_%s.csv",     //son220715 d:\\log ������ �̵�  //son240516
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
    {   //son210416 .CON �� ������ PinMap ��� ��� ����
        strTemp.Format("Can't Write %s File.\n", fName );    
        errMessageBox(1113, strTemp); //son230217_2
        return 0;
    }
 
    //------------------
    //son ������
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS //son190902 32KPIN //son230216_1
         || SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN    //son-8K4097-20200605: 8K ���� �Ϻν����� ���ÿ��� ����.  
         || SysSet12.m_nPinTotMax == PIN_TOT_12K             //son210202: 12K ���� �߰�
         || SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY //son211020 8K���� ��θ� �����ϴ� ���̽� (f550j ����ý�) 
         || SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K //son230221
         || SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG)     //son230221
    {
        //son210416 .CON �� ������ PinMap ��� ��� ����
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
    //son Data ���
    //son 32K�϶����� HwPin <=> SwPin ��ȯ�� �Ͼ�Ƿ� ��ȯ ����� ���������� Ȯ���Ѵ�.
    //son 32K Trans�϶����� HwPin <=> SwPin ��ȯ�� �Ͼ�Ƿ� ��ȯ ����� ���������� Ȯ���Ѵ�.
    //    jigPin0 �� jigPin0->hwPin0->jigPIn0 �� ����� ������ OK
    int pin, nQdSwPin, nPinBlock, nDdSdSwPin, nDdSdSwPin2;  
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son190902 32KPIN //son230216_1
    {
        //son .CON�� ���ٸ�
        if(nPinConvertType==0)  //son210416 
        {
            if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
            {
                int nQdGap, nQdJigPin0_1, nQdJigPin0_2 ;
                //fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigpin0, QdJigPin0, QdJigPin0_1,  PinBlock, QDType \n");	//son230221
                fprintf(fp, "   UserPin, SwitchCardPin, , jigPin0, Pin0->hwPin0, hwPin0->jigpin0,  Check1, nJigPinMap, Check2 ,\n");	//son230221
                for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
                {
                    //if ( (pin % 512 != 0) && ((pin+1) % 1024 != 0)) //son ��� ������ ���̱�
                    //    continue;

                    nPinBlock    = PinMap[3][pin];    //son210514
                    nQdGap       = (nPinBlock-1)*4096 - (sPinBlockRange[nPinBlock].nStartPin-1);     //son210514
                    nQdJigPin0_1 = pin + nQdGap;      //son210514

                    nQdJigPin0_2 = make32kSwPin(PinMap[2][pin]);  //son hwPin->jigPin ���
                    //fprintf(fp, "%10d, %10d, , %d, %d, %d, , %d, %s, %d, %d\n",  
                    //      //userPin  switchPin     jigPin0   Pin0->hwPin0      hwPin0->jigPIn0  PinBlock        QDType,                         QdJigPin0       QdJigPin0_1  
                    //      pin+1, PinMap[2][pin]+1,  pin,     PinMap[2][pin],   nQdJigPin0_2,    PinMap[3][pin], STR_BOXQD_TYPE(PinMap[4][pin]), PinMap[6][pin], nQdJigPin0_1 ); 
                    
                    //son jigPin0�� QdJigPin0_2(hwPin0->jigPIn0 �� ���)�� ���ƾ� �Ѵ�.
                    //    nJigPinMap[hwPin]�� jigPin0�� ���ƾ� �Ѵ�. 
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
                    jigPin = make32kSwPin(PinMap[2][pin]);  //son hwPin->jigPin ���

                    nQdSwPin    = getQdSwPin(jigPin);       //son jigPin->hwPin�� ���� (range üũ����)
                    nDdSdSwPin  = getDdSdSwPin(nQdSwPin);   //son hwPin->jigPin�� ���� (range üũ����)

                    nDdSdSwPin2 = getDdSdSwPin(pin);     //son �̰� �׳� 0~32767������� hwPin->jigPin�� ������� ���� ����� ���� ����.

                    //fprintf(fp, "%10d, %10d, , %d, %d, %d, , %d, %d,  ,%d\n",   
                          //userPin  switchPin       jigPin0    Pin0->hwPin0   hwPin0->jigPIn0 jigPin0->hwPin0 hwPin0->jigPin0  getDdSd(hwPin0~end) 
                    //      pin+1, PinMap[2][pin]+1, pin,    PinMap[2][pin],   jigPin,         nQdSwPin,       nDdSdSwPin,      nDdSdSwPin2); 
                    
                    //son jigPin0��  nDdSdSwPin(hwPin0->jigPin0 �� ���)�� ���ƾ� �Ѵ�. (������)
                    //    nJigPinMap[hwPin]�� jigPin0�� ���ƾ� �Ѵ�. 
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
    //son-8K4097-20200605: 8K ���� �Ϻν����� ���ÿ��� ����.  
    else if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
             || SysSet12.m_nPinTotMax == PIN_TOT_12K               //son210202: 12K ���� �߰�
             || SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY   //son211020 8K���� ��θ� �����ϴ� ���̽� (f550j ����ý�) 
             || SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K //son230221
             || SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG)     //son230221
    {
        //son .CON�� ���ٸ�
        if(nPinConvertType==0)  //son210416 
        {
            //fprintf(fp, "   UserPin, SwitchCardPin, , Pin0, Pin0->hwPin0, hwPin0->jigPin0, getQdSwPin(jigPin),  getDdSd(nQdSwPin),  ,getDdSd(hw:0~end) \n");
            fprintf(fp, "   UserPin, SwitchCardPin, , Pin0, Pin0->hwPin0, hwPin0->jigPin0, Check1, nJigPinMap, Check2, getQdSwPin(jigPin),  getDdSd(nQdSwPin),  ,getDdSd(hw:0~end) \n");
            for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
            {
                if (SysSet12.m_nBlockSet1 == 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG && (pin +1) > SysSet12.m_nEndPin) //son241007-BSW-9
                   break;

                hwPin = PinMap[2][pin];   //son230303
                jigPin = make32kSwPin(PinMap[2][pin]);          //son hwPin->jigPin ���

                if (SysSet12.m_nBlockSet1 == 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG) //son241007-BSW-9
                    nQdSwPin = getQdSwPin_BlockSet1(jigPin);     //son241007-BSW-9
                else
                    nQdSwPin = getQdSwPin_Fixed(jigPin);         //son jigPin->hwPin�� ���� (range üũ����)

                nDdSdSwPin  = getDdSdSwPin_Fixed(nQdSwPin);     //son hwPin->jigPin�� ���� (range üũ����)
                nDdSdSwPin2 = getDdSdSwPin_Fixed(pin);       //son �̰� �׳� 0~32767������� hwPin->jigPin�� ������� ���� ����� ���� ����.
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
    //son .CON�� �ִٸ�
    if(nPinConvertType==1)  //son210416 .CON �� ������ PinMap ��� ��� ����    //son230314
    {
        //fprintf(fp, "   UserPin, SwitchPin, , conJigPin0, swPin0, swPin0->hwPin0, hwPin0->swPin0 \n");
        fprintf(fp, "   UserPin, SwitchPin, , conJigPin0, swPin0, swPin0->hwPin0, hwPin0->swPin0, Check1, nJigPinMap, Check2 \n"); //son230303
        for (conJigPin=0; conJigPin < nMaxPin1; conJigPin++)//SYLEE121003
        {
            
            //son conJigPin�� �´� swPin�� ã�´�. 
            swPin = -1; 
            for (conIdx=1; conIdx <= nConTotal; conIdx++) {
                if ((nPinC1[conIdx][1]-1) == conJigPin) {
                    swPin = nPinC1[conIdx][0]-1;
                    break;
                }
            }
            if (swPin < 0)  
                continue;

            //son Check1: swPin��  orgJigPin(hwPin0->swPIn0)�� ���ƾ� �Ѵ�.
            //son Check2: conJigPin��  nJigPinMap[hwPin]�� ���ƾ� �Ѵ�.
            int orgJigPin = make32kSwPin(PinMap[2][conJigPin]);  //son hwPin->orgJigPin ���


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
    //son PinMap File ����   (jigPin -> hwPin)
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
         || (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN)    //son-8K4097-20200605: 8K ���� �Ϻν����� ���ÿ��� ����.  
         || (SysSet12.m_nPinTotMax == PIN_TOT_12K))             //son210202: 12K ���� �߰�
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
            jigPin = make32kSwPin(PinMap[2][pin]);  //son hwPin->jigPin ���
            nQdSwPin    = getQdSwPin(jigPin);       //son jigPin->hwPin�� ���� (range üũ����)
            nDdSdSwPin  = getDdSdSwPin(nQdSwPin);   //son hwPin->jigPin�� ���� (range üũ����)
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

    //son-8K4097-20200605: 8K ���� �Ϻν����� ���ÿ��� ����.
    else if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
             || SysSet12.m_nPinTotMax == PIN_TOT_12K)               //son210202: 12K ���� �߰�
    {
        fprintf(fp, "   UserPin, SwitchCardPin, , Pin0, Pin0->hwPin0, hwPin0->jigPin0, getQdSwPin(jigPin),  getDdSd(nQdSwPin), getDdSd(hw:0~end) \n");
        for (pin=0; pin < nMaxPin1; pin++)//SYLEE121003
        {
            jigPin = make32kSwPin(PinMap[2][pin]);          //son hwPin->jigPin ���
            nQdSwPin    = getQdSwPin_Fixed(jigPin);         //son jigPin->hwPin�� ���� (range üũ����)
            nDdSdSwPin  = getDdSdSwPin_Fixed(nQdSwPin);     //son hwPin->jigPin�� ���� (range üũ����)
            nDdSdSwPin2 = getDdSdSwPin_Fixed(pin);       //son �̰� �׳� 0~32767������� hwPin->jigPin�� ������� ���� ����� ���� ����.
            fprintf(fp, "%10d, %10d, , %d, %d, %d, %d, %d, %d\n",  pin+1, PinMap[2][pin]+1, pin,  PinMap[2][pin], jigPin,  nQdSwPin,  nDdSdSwPin,  nDdSdSwPin2); 
        }
    }

    fclose(fp);
*/


    return 1;

 
}

//son241007-BSW-9
// 32K Org  BlockSet1(QD/DD/SD ȥ����) ������ HwPin ��� �Լ�
//
// nSwPin    : pinBlock 1�� ������  swPin
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

    //son block 1 ������ pin�̹Ƿ� 4096�� ���� �� ����.
    if (nSwPin < 0 || nSwPin >= MAX_BOX_PIN)    
        return -1;
    
    
    // BlockSet1 ����ϴ� ����̹Ƿ�  getQdSwPin(nSwPin) ȣ��. 
    UINT nQdSwPin = getQdSwPin_BlockSet1(nSwPin);   


    //son 32K Org�� ������ �ɺ���̹Ƿ� 32K Convert ���� return�Ѵ�.
    return nQdSwPin; 

}

//---------------------------------------------------------------------------
//son  nSwPin ���� �´� HwPin(SwitchCard pin)�� �����Ѵ�. 
//    �⺻�� 16K QD �ε� ���������� ������ �� ���̽����� ���. 
//    ��� QD 2box, �Ϻ� QD 2box,   ������ Original�� Trans �ƴ�.
//--------------------------------------------------------------------------
// parameter  : nSwPin      Input ����, 0���� �����ϴ� SwPin(JigPin)
// return val : nHwPin      nSwPin�� �´� Hw SwitchCard PinBlock Pin

int CModeSet12::make16kQdBase_HwPin(UINT nSwPin) //son211020 make8kNew_Hwpin() -> make16kQdBase_HwPin()
{
    
    //---------------------
    // nSwPin Range Check
    
    switch (SysSet12.m_nPinTotMax) 
    {
        case PIN_TOT_8K_LOW_STPIN:      //son ��� QD 1box, �Ϻ� QD 1box
        case PIN_TOT_U8K_UPPER_ONLY:    //son211020  ��� QD 2box only 
            // 8K range check  (jigPin ���� 8K �̻��� ���� �� ����.)
            if (nSwPin < 0 || nSwPin >= MAX_BOX_PIN * 2)    // 8K
                return -1;
            break;

        case PIN_TOT_12K:               //son ��� QD 2box,  �Ϻ� QD 1box 
            // 12K range check  (jigPin ���� 12K �̻��� ���� �� ����.)
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
    
    // �Ϻν������� QD ��������  DD,SD �ÿ� ������ �ʴ�  ���  getQdSwPin_Fixed()�� ����Ѵ�. 
    UINT nQdSwPin = getQdSwPin_Fixed(nSwPin);       // JYK 20200605

    // 16K��, 32K��  QD ���� �Ϻ� �������̸� Hw �Ϻ� Offset�� 16384 ����.  (QD, 32K ��� ���� HW ������)
    UINT nHW_UpperPartSize = (MAX_BOX_PIN *4);  // 16384



    // jigPin0�� ���� ��ΰ� �´µ�,  QD �������� ��ȯ�� SWPin�� UpperPartSize�� �Ѿ�ٸ�
    // ex) "8K����, �Ϻ��� 4097, SD Ÿ���̸�" SD�̹Ƿ� jigPin0 2048�� hwPin 16384�� �ȴ�. 
    //     �̶� �Ϻ����� 4097�� ���������Ƿ�, jigPin 4096�� �ݵ�� hwPin�� 16384�� �ž��ϴ� hwPin �浹�� ��. 
    //     �Ϻ��� 4096, SD �� ���� jigPin 2048~ 4095������ -1�� �Ⱦ��� ���� ó���� �ؾ� �Ѵ�.
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;   // logical ��� Sw Pin ����
    if ( nSwPin < nOrg_UpperPartSize            // jigPin0�� ���
         &&   nQdSwPin >= nHW_UpperPartSize  )  // QDSwPin�� 16384�� �Ѿ
        return -1;
    
    //---------------------------
    //HW Low Part Range üũ
    
    // �Ϻ� HW ��Ʈ ������ ���� range üũ�� �Ѵ�.
    UINT nHW_LowerPartSize;
    switch (SysSet12.m_nPinTotMax)  
    {
        // 8K LOW STPIN�� �����δ� ��� 8K, �Ϻ� 8K HW ���.  jigPin�� max 8K ����̴�.
        case PIN_TOT_8K_LOW_STPIN:
            nHW_LowerPartSize = MAX_BOX_PIN * 2;       //son QD BlockType�̸�,  8K�� LowerPartSize. 
            if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son230216_1
                nHW_LowerPartSize = MAX_BOX_PIN * 4;
            break;

        // 12K ���� Ÿ���� ��� 8K, �Ϻ� 4K HW���� ����Ѵ�.
        case PIN_TOT_12K:
            nHW_LowerPartSize = MAX_BOX_PIN;    //son210202: �Ϻδ� box 1��
            break;

        case PIN_TOT_U8K_UPPER_ONLY:    //son211020 
            nHW_LowerPartSize = 0;      //son211020 �Ϻ� ����
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

    // HW �Ϻν�����(8192 �Ǵ� 16384) + LowerPartSize(QD ���� 1box)  �� �������� �Ϻ� MaxPin��
    UINT nSwLowMaxPin = nHW_UpperPartSize + nHW_LowerPartSize;
    if (nQdSwPin >= nSwLowMaxPin) // �Ϻ� Max-Pin���� 20200205 JYK
        return -1;

//son230221 begin:
    // HwPin ��ȯ ����� 32768�� �Ѵ� ���,  U8K_L24K�� 
    // bdl ���� �Ϻ� 5, 6 �ڽ� pin��  HW ��ġ�� ���  3, 4�� �ٲ� �ش�.
    // box��ȣ�δ�  box 9 -> hw box 3,  box 10 -> hw box 4 �� ��.
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG 
            && SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K) { 
        if (nQdSwPin >= MAX_PIN)                                
            nQdSwPin = nQdSwPin - (MAX_BOX_PIN * 6);            
    }
//son230221 end


    //son201218 ������ �ɺ���̹Ƿ� 32K Trans Convert ���� return�Ѵ�.
    return nQdSwPin; 

}

//son210514 �־��� nUpperEnd���� �´� �Ϻν�������ġ�� ã�´�.
//    MakePinBlockRangeUserNo() ���ο����� �����.
int CModeSet12::FindBlockSet_LowerStartPin(int nUppderEnd)  //son210514
{
    int nLowerJigStartPin;


    //----------------------------------------
    //son ��� 3box  16K_U12K_L4K �������̽�
    
    //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS�� ���⿡ �ش��� �� �ȴ�. box 5�� 16385�� �����ؾ� ��.
    if (IS_UPPER_12K_2(SysSet12.m_nPinTotMax))  //son230718
    {
        //son 12K���� QD/DD/SD �� ������ �Ҵ�ÿ� ���� �� �ִ� �Ϻ� ������ (27���� 3*3)
        //    �̰� �� �����ϰ� �� �ִ� ���� ������尡 ũ�Ƿ� 
        //    SD/SD/SD=3072, DD/DD/DD=6144, QD/QD/QD=12288  �̷��� 3���� ��츸 �ϴ� �����ϰ�
        //    ���� �䱸���� �����鼭 �߰��Ѵ�.
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
        else if (nUppderEnd <= (2048 * 3))                          //son230714 ���� ����
            nLowerJigStartPin = (2048 * 3) +1;    //son DD 6145     //son230714

        else if (nUppderEnd <= (4096 * 3))
            nLowerJigStartPin = (4096 * 3) +1;	  //son QD 12289

        return nLowerJigStartPin;
    }       //son210525

    //-----------------------
    //son �Ϲ� ���̽� 

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
   
 
 
//son sPinBlockRange[10]��  �����ϰ� "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
int CModeSet12::MakePinBlockRangeUserNo_32K()//son210525
{
    int nEnd, nLowBlockStartNo, nBlockType,nBlocksize;

    nEnd=0; nBlockType=0; nLowBlockStartNo=0; nBlocksize=0;


    ::ZeroMemory(&sPinBlockRange, sizeof(sPinBlockRange));
    //---------------------------------------------------------
    for (int nStep=1;  nStep<=8;  nStep++)
    {
        //son ��� 4th box�� ���� ��� 
        //son210603  PIN_TOT_20K_U12K_L4K_UP_TRANS�� ���⿡ �ش��� �� �ȴ�. 5th box �� 16385�� �����ؾ� ��.
        if (IS_UPPER_12K_2(SysSet12.m_nPinTotMax)) //son210603    //son210826   //son230718
        {
            if (nStep==4)
                continue; 
        }

        //son ��� 3rd, 4th box�� ���� ��� 
        if (IS_UPPER_8K_2(SysSet12.m_nPinTotMax))      //son210514 //son230718
        {
            if (nStep==3 || nStep==4)
                continue; 
        }

        //son ��� 2, 3, 4 box�� ���� ��� 
        if (IS_UPPER_4K(SysSet12.m_nPinTotMax))       //son210514 
        {
            if (nStep==2 || nStep==3 || nStep==4)
                continue; 
        }

        //son �Ϻ� 7, 8 box�� ���� ���
        if (IS_LOWER_8K(SysSet12.m_nPinTotMax))       //son210514
        {
            if (nStep==7 || nStep==8 )  
                continue; 
        }


        //son �Ϻ� 6, 7, 8 box�� ���� ���
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

        //son �ش� �ڽ� skip�̸� (upper3, upper4, lower3, lower4�� SKIP ������ �� �ִ�.)
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


        //son HW�� �Ϻν��� box (box1 5)
        if(nStep==5)
        {

            //son Step=5�϶�, QD/DD/SD ��ϼ������̽����� �Ϻν������� ����ؼ� ����Ѵ�. 
            //    QD/DD/SD ���¿� �ɸ��� �Ϻν������� ����ؼ� ����Ѵ�. (������ �� ������)
            if (SysSet12.m_nBlockSet1 == 1                      //son210525
                    //son230718|| IS_UPPER_12K(SysSet12.m_nPinTotMax)  
                    || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS     //son210826 ���� nEnd�� �´� �����Ϻν����� �ʿ�
                    || SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS    //son230718 �����Ϻ��� ��� Ư�����̽�
                    || SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)     //son230718 �����Ϻ��� ��� Ư�����̽�
            {                 
                //son �Ϻν������� �����Ͽ� ���� nEnd�� �°� �缳���Ѵ�.
                SysSet12.m_nLowerJigStartPin = FindBlockSet_LowerStartPin(nEnd);  //son210514
                nEnd = SysSet12.m_nLowerJigStartPin-1;                      //son210514
            } //son210525

            //son Step=5�϶� �Ϻν����� ���ÿ��� nEnd�� �Էµ� �Ϻν��������� �����Ѵ�.
            //    ����ڰ� ������ �Ϻν������� �������� ����ϹǷ�  ���⿡�� �Ϻν������� �����ϸ� �� �ȴ�.
            else    
            {
                nLowBlockStartNo=SysSet12.m_nLowerJigStartPin;       
                if( (nEnd < nLowBlockStartNo) && ( (nLowBlockStartNo==(1024+1)) //son 1025, 2049, 4097, 8193, 16385 �� �ϳ��� ���� �Ϻν�����.. 
                                                    || (nLowBlockStartNo==(2048+1)) 
                                                    || (nLowBlockStartNo==(2048*2+1))     
                                                    || (nLowBlockStartNo==(2048*4+1))  
                                                    || (nLowBlockStartNo==(2048*8+1))) )
                    nEnd=nLowBlockStartNo-1;        //son ��� size
            } 

        }    
        sPinBlockRange[nStep].nStartPin = nEnd+1;                                        //son hwBox StartPin 
        sPinBlockRange[nStep].nEndPin   = sPinBlockRange[nStep].nStartPin+nBlocksize -1; //son hwBox EndPin 
        sPinBlockRange[nStep].nBlockType= nBlockType;                                    //son hwBox BlockType

        nEnd=sPinBlockRange[nStep].nEndPin;  //son ������ nStep�� StartPin ����� ���� nEnd�� �Ҵ�.


        //son TRANS type�̸� box ��ȣ�� ���� trans ������ ������ �ش�.
        sPinBlockRange[nStep].nHwBoxType = TYPE_AC; //son230316        
        if (IS_TRANS_TOTMAX(SysSet12.m_nPinTotMax)) //son210514
        {
            //son ��θ� Trans�� ���
            if (IS_UPPER_TRANS(SysSet12.m_nPinTotMax))  //son210531
            {
                if(nStep==2 || nStep==4 )
                    sPinBlockRange[nStep].nHwBoxType = TYPE_BD;
                else 
                    sPinBlockRange[nStep].nHwBoxType = TYPE_AC;

            }

            //son ���, �Ϻ� ��� Full Trans �� ���
            else 
                //son nStep 1,3,5,7�� TYPE_AC(0),  nStep 2,4,6,8�� TYPE_BD(1)   
                sPinBlockRange[nStep].nHwBoxType = (nStep -1) % 2;          //son210514 
        }

    }

    ///////////////////////////////////////////////
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   //son210514  200 -> FIlE_PATH_SIZE
    ::ZeroMemory(&fName, sizeof(fName)); 
    CString strTemp;
    //strTemp.Format("d:\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),      //son210514
    strTemp.Format("d:\\log\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),   //son220715 d:\\log ������ �̵�
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
 
//son K600(�� 8K, �� 24K)����.  
//    sPinBlockRange[10]��  �����ϰ� �Ϻν������� �����ϰ�, "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
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


        //son HW�� �Ϻν��� box (box1 3). K600�� 3��° block���� �Ϻ�
        if(nStep==3)
        {
            //son PIN_TOT_32K_U8K_L24K Ÿ���̹Ƿ� �Ϻν������� QD�� �� 8193
            SysSet12.m_nLowerJigStartPin = FindBlockSet_LowerStartPin(nEnd);    //son ���⼭ nEnd�� UpperEnd�� ��.

        }    
        sPinBlockRange[nStep].nStartPin = nEnd+1;                                        //son hwBox StartPin 
        sPinBlockRange[nStep].nEndPin   = sPinBlockRange[nStep].nStartPin+nBlocksize -1; //son hwBox EndPin 
        sPinBlockRange[nStep].nBlockType= nBlockType;                                    //son hwBox BlockType

        nEnd=sPinBlockRange[nStep].nEndPin;  //son ������ nStep�� StartPin ����� ���� nEnd�� �Ҵ�.


        //son TRANS type�� �ƴϹǷ�  box ��ȣ�� ���� trans ������ AC �� ����.
        sPinBlockRange[nStep].nHwBoxType = TYPE_AC;

    }

    ///////////////////////////////////////////////
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];   
    ::ZeroMemory(&fName, sizeof(fName)); 
    CString strTemp;
    strTemp.Format("d:\\log\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),   //son d:\\log ������ �̵�
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

//son ��� QD/DD/SD ���� ���¿� ���� UpperEndJigPin ��ȣ�� ����Ѵ�.
//    QD/DD/SD ȥ��ÿ��� ��밡�� 
int CModeSet12::CalcUpperEndJigPin_BlockSet() //son241007-BSW-9  
{
    int nBlockType, nBlocksize;
    int nUpperEndJigPin = 0;

    if (SysSet12.m_nBlockSet1 != 1)
        return -1;

    //son ��� box ���� ��ŭ�� ī��Ʈ�Ѵ�.
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

        //son �ش� �ڽ� skip�̸� (upper3, upper4 �� SKIP ������ �� �ִ�.)
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

//son �Ϻν������� ���⼭ ����
//son sPinBlockRange[10]��  �����ϰ� "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
int CModeSet12::MakePinBlockRangeUserNo()//sylee210402 
{
    CString str;
    int nEnd, nLowBlockStartNo, nBlockType,nBlocksize;

    nEnd=0; nBlockType=0; nLowBlockStartNo=0; nBlocksize=0;

//son230217 begin:
    //son K600(�� 8K, �� 24K)��  sPinBlockRange[10]��  ����
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

        //son 32K ORG pinBlock, K600�� Upper 8K, Lower 24K PinMode�� ������ ȣ���Ѵ�.
        if (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)   //son230316
            return MakePinBlockRangeUserNo_K600();  
    }
//son230217 end

    if (SysSet12.m_nPinTotMax != PIN_TOT_32K_ORG) //son241007-BSW-9
    {
        //son 32K Trans PinBlock�� ���� PinRange�� �Ϻν����ɵ��� ������� �ٸ��Ƿ� 32K�� ������ �Լ��� ����.   //son210525
        if (IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax))   //son210525
        {
            return MakePinBlockRangeUserNo_32K();   //son210525
        }
    }

    //------------------------------------------------
    //son ������ ���÷��� ��� (32K ORG �Ǵ� OLD QD ���)


    ::ZeroMemory(&sPinBlockRange, sizeof(sPinBlockRange));
    for (int nStep=1;  nStep<=8;  nStep++)
    {
        //son 32K base Ÿ���� ��� 
        if( SysSet12.m_nPinTotMax==PIN_TOT_20K_UP_TR        //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_24K_UP_TR //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_24K_TRANS //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_32K_TRANS //son230216_1
                || SysSet12.m_nPinTotMax==PIN_TOT_32K_ORG)  //son230316
        {
        }

        //son 32K base Ÿ���� �ƴ�  ��� 16K�̹Ƿ� box 3, 4, 7, 8 �� �������� �ʴ´�.
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


        //son �ش� �ڽ� skip�̸� (upper3, upper4 �� SKIP ������ �� �ִ�.)
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


        //son HW �Ϻν��� box (box 5)
        if(nStep==5)
        {

            //son QD/DD/SD ȥ������ ���� �Ϻν����ɿ� offset�� �� ������ ��� ������� �Ѵ�.
            if (SysSet12.m_nBlockSet1 == 1) 
                nEnd += SysSet12.m_nLowerJigStartPin_Offset; //son241007-BSW-9

            //son �Ϻν������� �����Ͽ� �缳���Ѵ�.
            SysSet12.m_nLowerJigStartPin = nEnd +1; //son210525

        }    
        sPinBlockRange[nStep].nStartPin = nEnd+1;                                        //son hwBox StartPin 
        sPinBlockRange[nStep].nEndPin   = sPinBlockRange[nStep].nStartPin+nBlocksize -1; //son hwBox EndPin 
        sPinBlockRange[nStep].nBlockType= nBlockType;                                    //son hwBox BlockType

        nEnd=sPinBlockRange[nStep].nEndPin;  //son ������ nStep�� StartPin ����� ���� nEnd�� �Ҵ�.

        //son TRANS type�̸� box ��ȣ�� ���� trans ������ ������ �ش�.
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
    strTemp.Format("d:\\log\\PinBlockRange1_%s_%d__%s_%s_%s_%s__%s_%s_%s_%s.ini",STR_MAX_PIN(SysSet12.m_nPinTotMax),   //son220715 d:\\log ������ �̵�
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
//son nSwPin ���� �´� HwPin(SwitchCard pin)�� �����Ѵ�. 
//--------------------------------------------------------
// parameter  : nSwPin      Input ����, 0���� �����ϴ� SwPin(JigPin)
// return val : nHwPin      nSwPin�� �´� Hw SwitchCard PinBlock Pin
int CModeSet12::make32kHwPin(UINT nSwPin)
{
    UINT nPart, nHwPartBox, nHwBoxType;
    UINT nSwBoxPin, nSwLine, nSwCol;
    UINT nHwBoxPin, nHwLine, nHwCol;
    UINT nHwPin;    // return val
    
    //son-8K4097-20200605: SWMaxPin�� 8K����, HW�� 16K, QDHW ���� �Ϻν����� ���ÿ� ����.
    if(SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN   //son SW MaxPin�� 8K, HW�� 16K ���̽�
         || (SysSet12.m_nPinTotMax == PIN_TOT_12K)     //son210202: 12K ���� �߰�
         || (SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY) //son211020 8K���� ��θ� �����ϴ� ���̽� (f550j ����ý�) 
         || (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230221
         || (SysSet12.m_nBlockSet1 != 1 && SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG ))   //son230316  //son241007-BSW-9  
    {
        return make16kQdBase_HwPin(nSwPin);     //son211020 make8kNew_Hwpin() -> make16kQdBase_HwPin()
    }

    if (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG ) //son241007-BSW-9
        return make32kOrgBlockSet_HwPin(nSwPin);  //son block 1�� ������ hwPin�� return.


    //------------------------------------------
    // QD, SD, DD-> QD �������� swPin�� ��ȯ.  
    
    // �Ϻν����ɰ��� Sw dependent�ؼ� DD, SD�� ����� QD  �������� �ٲپ� ��� �ϴ� ���
    // getQdSwPin()�� ����Ѵ�. ���������� getQD_UpperPartSize()�� �����.
    UINT nQdSwPin = getQdSwPin(nSwPin);         // JYK 20200117
    

    
    // �Ϻν����������� QD ������ SW UpperPartSize ����.
    UINT nSw_UpperPartSize = getQD_UpperPartSize();
     

    //--------------------
    // nSwPin Range Check
    
    // �Ϻΰ� ��κ��� ū ��쵵 �����. (�Ϻν����� 4096�̸�..) //son-32KPIN-20200103 
    UINT nSwMaxPin; 
    UINT nSwLowMaxPin;  // �Ϻ� Max-Pin
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
        case PIN_TOT_20K_U12K_L4K_UP_TRANS:  //son210603 Ư�����̽�  skip 4th box.  QD�Ϻν����� 16385 ����
        case PIN_TOT_20K_U8K_L4K_UP_TRANS:   //son210826 Ư�����̽�  skip 3rd, 4th box. QD �Ϻν����� 16385 ����
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

    // �Ϻν������� 16384���� ���� ���̸� nSwMaxPin�� �ƴ϶� nSwLowMaxPin�� �������� swPin�� Max ���̴�.
    // nSwLowMaxPin�� �Ϻν�����+ lowerpart�� �������̹Ƿ� �������� maxPin�� ��.
    nSwLowMaxPin = nSw_UpperPartSize + nLowerPartSize;
    if (nQdSwPin >= nSwLowMaxPin) // �Ϻ� Max-Pin���� 20200205 JYK
        return -1;


    //-------------------
    // SwPin ���� �м� 
    //-------------------
    
    // Part(Upper or Lower) > Box (A~D) > Line (0~63) > Column (0~63)
    
    // ������� �ϴ����� Part�� �˾� ����.
    // UpperPartSize�� 4096, 8193�� �� �����Ƿ� swPin / PartSize�δ� ��Ʈ��ġ�� �� �� �����Ƿ�
    // UpperPartSize���� swPin�� ũ�� �Ϻη� �ν��Ѵ�.
    nPart  = (nQdSwPin >= nSw_UpperPartSize) ? PART_LOWER: PART_UPPER;      
    UINT nSwBox = nQdSwPin / MAX_BOX_PIN;           // SW Part�� box ��ȣ (0~7)

    

    //----------------------------------------------------------------------------
    // �Ϻ����̸� �Ϻ� StartPin�� ����ؼ� �Ϻ� Box ��ȣ�� Hw �������� �ٽ� �Ҵ�.  
    UINT nHwBox = nSwBox;
    if (nPart == PART_LOWER)    
    {

        // ��� box ����: nSw_UpperPartSize�� MAX_BOX_PIN:4096�� ����� 
        int nSwUpperBoxNum = nSw_UpperPartSize / MAX_BOX_PIN;   // nSw_UpperPartSize    nSwPartBoxNum
                                                                //  16384(16K)          4
                                                                //  12288(12K)          3
                                                                //  8192 (8K)           2
                                                                //  4096 (4K)           1
    
        // err check
        ASSERT(nSwUpperBoxNum == 4 || nSwUpperBoxNum == 3 || nSwUpperBoxNum == 2 || nSwUpperBoxNum == 1);

        // �Ϻ����ε� �Ϻν����� 16385�̸�(��� box 4�� ���) SwBox�� +0�ϰ�,  
        // �Ϻ����ε� �Ϻν����� 12288�̸�(��� box 3���� ���) SwBox�� +1�ϰ�,  
        // �Ϻ����ε� �Ϻν����� 8193 �̸�(��� box 2���� ���) SwBox�� +2�ϰ�,  
        //            �Ϻν����� 4097 �̸�(��� box 1���� ���) SwBox�� +3�Ѵ�.
        nHwBox = nSwBox + (MAX_BOX_PER_PART - nSwUpperBoxNum);
        if (nHwBox >= MAX_BOX)
            return -1;
        
        //----------------------------------------------------------
        //for 32K (Upper 16K Transpose  + Lower 16K Transpose) base             

        //ex) HW Upper 16K, �Ϻν����� QD 16385(DD 8193, SD 4097), ��� 4box QD, �Ϻ� 4box QD
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
        //      16384       8192    4096    1:L     4       0:A     0           4032        20416 <==�Ϻν����� 
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
        
        //ex) HW Upper�� 16K, �Ϻν����� QD 8193(DD 4097, SD 2049), ��� 2box QD, �Ϻ� 4box QD
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
        //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==�Ϻν�����
        //      12287       6143    -       1:L     2       0:A     4095        63          16447
        //      12288       6144    3072    1:L     3       1:B     0           63          20543
        //      16383       8191    -       1:L     3       1:B     4095        4032        24512
        //      16384       8192    4096    1:L     4       2:C     0           4032        28608
        //      20479       10239   -       1:L     4       2:C     4095        63          24639
        //      20480       10240   5120    1:L     5       3:D     0           63          28735
        //      24575       12288   -       1:L     5       3:D     4095        4032        32704
        //      24576 ����      ����
        
        //ex) HW Upper�� 16K, �Ϻν����� QD 4097(DD 2049, SD 1025), ��� 1box, �Ϻ� 4box
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
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===�Ϻν�����
        //      8191        4095    -       1:L     1       0:A     4095        63          16447
        //      8192        4096    2048    1:L     2       1:B     0           63          20543
        //      12287       6143    -       1:L     2       1:B     4095        4032        24512
        //      12288       6144    3072    1:L     3       2:C     0           4032        28608
        //      16383       8191    -       1:L     3       2:C     4095        63          24639
        //      16384       8192    4096    1:L     4       3:D     0           63          28735
        //      20479       10240   -       1:L     4       3:D     4095        4032        32704
        //      20480 ����       ����
        
        
    }                                

    nHwPartBox = nHwBox % MAX_BOX_PER_PART; // % 4   Hw Part�� box��ȣ�� ��ȯ.

    // ¦���� TYPE_AC, Ȧ���� TYPE_BD
    nHwBoxType = nHwPartBox % 2;    
/*
    if (nPart == PART_LOWER)    // �Ϻ� Box-Type���� 20200205 JYK
        nHwBoxType = ((nSwPin - nSw_UpperPartSize) / MAX_BOX_PIN) % 2;
    else
        nHwBoxType = (nSwPin / MAX_BOX_PIN) % 2;
*/

 
    nSwBoxPin  = nQdSwPin % MAX_BOX_PIN;        // Box�� SwPin ��ȣ (4096����� offset�� ������ swPin No)
    nSwLine    = nSwBoxPin / MAX_BOX_LINE;
    nSwCol     = nSwBoxPin % MAX_BOX_LINE;

    // HW Pin�� ���� ������� ������Ű�� 4096 ����� offset�� �����ϸ�
    // Upper_A, Upper_C, Lower_A, Lower_C�� �ɹ�ġ�� �����ϹǷ� TYPE_AC�� ����.
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

        //son20514  QD/DD/SD BlockSet  ���¶�� �Ϻιڽ����� ���θ�  m_nBlockPo1�� �Ǵ��Ѵ�.
        nHwPartBox_forCheck = m_nBlockPo1 -5; //son210514
    }


    //-----------------------------------------------
    //for 24K (12K Upper Trans + 8K Lower Trans)
    //-----------------------------------------------
    //son Upper 4��° Box(nHwParBox 3)�� ����.
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
    // ����ɸ�  Transpose ���� Ÿ���̴�. �Ϻ����� ���� 16K�� ���� Original �⺻ Ÿ��.
    // �Ϻ����ε� 20K, 24K�̸� swPin ��ġ������ hwPin���� �״�� ����ؼ� nHwPin�� ����� �����Ѵ�.
    // �Ϻ����ε� 32K or 24K_FULL�̰ų�,  �������  ���� �� �ڵ� ��������  ����.
    //if(SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR ) //son230216_1
    if(IS_UPPER_TRANS(SysSet12.m_nPinTotMax))  //son210531
    {
        //son ��� 12K Trans , �Ϻ� 4K Origin�� ���� Upper 4��° Box(nHwParBox 3)�� ����.
        if ((SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS  
                    || SysSet12.m_nPinTotMax ==  PIN_TOT_20K_U12K_L4K_UP_TRANS)    //son210603
                && nPart == PART_UPPER )    //son210531  
        {
            if (nHwPartBox == 3)    //son210531
                return -1;
        }

        //son ��� 8K Trans , �Ϻ� 4K Origin�� ���� Upper 3, 4��° Box(nHwParBox 2,3)�� ����.
        if (SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS  && nPart == PART_UPPER)    //son210826  
        {
            if (nHwPartBox == 2 || nHwPartBox == 3)    //son210826
                return -1;
        }


        //son �Ϻ� �ڽ����
        //son20514  QD/DD/SD BlockSet  ���¶�� �Ϻιڽ����� ���θ�  m_nBlockPo1�� �Ǵ��Ѵ�.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                //son210514
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )    //son210514 
        {

            //son UpperTrans�̰� �Ϻο� box�� 1���ۿ� ���� ���  BoxNum�� 1���� ���� �� ����.
            // IS_UPPER_TRANS(SysSet12.m_nPinTotMax)������ ������ �̹� üũ�� 
            if (IS_LOWER_4K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 1)  //son210826
                return -1;

            //son UpperTrans�̰� �Ϻο� box�� 2���ۿ� ���� ���  Hw BoxNum�� 2���� ���� �� ����.
            if (IS_LOWER_8K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 2)  //son210826
                return -1;

            // 20K -------------------------------
            //
            //ex) PinTotMax�� 20K, �Ϻν����� QD 16385(DD 8193, SD 4097), ��� 4box, �Ϻ� 1box
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
            //      16384       8192    4096    1:L     4       0:A     0           0           16384 <==�Ϻν����� 
            //      20414       10207   5119    1:L     4       0:A     4030        4030        20414
            //      20479       10239   -       1:L     4       0:A     4095        4095        20479
            //      20480 ���� ����.            

            //ex) PinTotMax�� 20K, �Ϻν����� QD 8193(DD 4097, SD 2049), ��� 2box,  �Ϻ� 1box
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
            //      8192        4096    2048    1:L     2       0:A         0           0           16384 <==�Ϻν�����
            //      12222       6111    3071    1:L     2       0:A         4030        4030        20414
            //      12287       6143     -      1:L     2       0:A         4095        4095        20479
            //      12288����      ����

            //ex) PinTotMax�� 20K, �Ϻν�����   QD 4097(DD 2049, SD 1025) ��� 1box, �Ϻ� 1box
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
            //      4096        2048    1024    1:L     1       0:A         0           0           16384 <===�Ϻν�����
            //      8126        4063    2047    1:L     1       0:A         4030        4030        20414
            //      8191        4095     -      1:L     1       0:A         4095        4095        20479
            //      8192 ����       ����



            // 24K -------------------------------
            //  
            //ex) PinTotMax�� 24K, �Ϻν����� QD 16385(DD 8193, SD 4097), ��� 4box, �Ϻ� 2box
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
            //      16384       8192    4096    1:L     4       0:A     0           0           16384 <==�Ϻν����� 
            //      20414       10207   5119    1:L     4       0:A     4030        3969        20414
            //      20479       10239   -       1:L     4       0:A     4095        4095        20479
            //
            //      20480       10240   5120    1:L     5       1:B     0           0           20480
            //      24510       12255   6143    1:L     5       1:B     4030        3969        24510
            //      24575       12287   -       1:L     5       1:B     4095        4095        24575
            //      24576 ���� ����.            

            //ex) PinTotMax�� 24K, �Ϻν����� QD 8193(DD 4097, SD 2049), ��� 2box,  �Ϻ� 2box
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
            //      8192        4096    2048    1:L     2       0:A     0           0           16384 <==�Ϻν�����
            //      12222       6111    3071    1:L     2       0:A     4030        3969        20414
            //      12287       6143     -      1:L     2       0:A     4095        4095        20479
            //
            //      12288       6144    3072    1:L     3       1:B     0           0           20480
            //      16318       8159    4095    1:L     3       1:B     4030                    24510
            //      16383       8191     -      1:L     3       1:B     4095        4095        24575
            //      16384����      ����

            //ex) PinTotMax�� 24K, �Ϻν�����   QD 4097(DD 2049, SD 1025), ��� 1box, �Ϻ� 2box
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
            //      4096        2048    1024    1:L     1       0:A     0           0           16384 <===�Ϻν�����
            //      8126        4063    2047    1:L     1       0:A     4030                    20414
            //      8191        4095     -      1:L     1       0:A     4095        4095        20479
            //
            //      8192        4096    2048    1:L     2       1:B     0           0           20480
            //      12222       6111    3071    1:L     2       1:B     4030        4030        24510
            //      12287       6143     -      1:L     2       1:B     4095        4095        24575
            //      12288 ����       ����
            
            
            // Upper 12K Trans + Lower 4K Org -------------------------------
            //ex) PinTotMax�� 16K, �Ϻν����� QD 12289(DD 6145, SD 3073), ��� 3box, �Ϻ� 1box
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
            //      12288       6144    3072    1:L     4       0:A     0           0           16384 <==�Ϻν����� 
            //      16318       8159    4095    1:L     4       0:A     4030        4030        20414
            //      16383       8191    -       1:L     4       0:A     4095        4095        20479
            //      16384 ���� ����.            


            //---------------------------------
            // 20K, 24K Lower HwPin ���� ��� 
            //---------------------------------

            // 20K, LOWER Part �� ���� nHwBoxPin ��� nSwBoxPin���� nHwPin�� ����ؼ� ���⼭ �����Ѵ�.
            // Original ������ �״�� �����ϹǷ�, �ϱ� �����ܰ���� �� �ʿ䰡 ����.
            nHwPin = nSwBoxPin + (nHwPartBox* MAX_BOX_PIN)      // BoxPin + Box Offset 
                + (nPart * MAX_PART_PIN);       // + HW PartOffset (upper size)

            return nHwPin;

        }
    }


    //---------------------------------------------------
    //for 24K FULL Transpose (Upper 16K + Lower 8K) base                //son-32KPIN-20200103 
    //---------------------------------------------------
    // �Ϻ����ε� 24K �� ���� �Ϻο� box�� 2���ۿ� �����Ƿ� �Ϻ� Hw BoxNum�� 2���� ���� �� ����.
    // ���������� pin ������ 32K ���̽��� ��ġ�� ������,  �Ϻ� C, D box�� ���� ���� Ư¡.
    // Pin ��ġ�� 32K Full�� ������ �����ϴ�. 

    if (SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son �Ϻιڽ����
        //son20514  QD/DD/SD BlockSet  ���¶�� �Ϻιڽ����� ���θ�  m_nBlockPo1�� �Ǵ��Ѵ�.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                    //son210514 
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )         
        {
            if (nHwPartBox_forCheck >= 2) //son210514
                return -1;

            //ex) PinTotMax�� 24K, �Ϻν����� QD 16385(DD 8193, SD 4097), ��� 4box, �Ϻ� 2box
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
            //      16384       8192    4096    1:L     4       0:A     0           4032        20416 <==�Ϻν����� 
            //      20414       10207   5119    1:L     4       0:A                             16510
            //      20479       10239   -       1:L     4       0:A     4095        63          16447
            //      
            //      20480       10240   5120    1:L     5       1:B     0           63          20543
            //      24510       12255   6143    1:L     5       1:B                             24449
            //      24575       12287   -       1:L     5       1:B     4095        4032        24512
            //      24576 ���� ����.            

            //ex) PinTotMax�� 24K, �Ϻν����� QD 8193(DD 4097, SD 2049), ��� 2box, �Ϻ� 2box
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
            //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==�Ϻν�����
            //      12287       6143    -       1:L     2       0:A     4095        63          16447
            //      12288       6144    3072    1:L     3       1:B     0           63          20543
            //      16383       8191    -       1:L     3       1:B     4095        4032        24512
            //      16384����      ����

            //ex) PinTotMax�� 24K, �Ϻν����� QD 4097(DD 2049, SD 1025), ��� 1box, �Ϻ� 2box
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
            //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===�Ϻν�����
            //      8191        4095    -       1:L     1       0:A     4095        63          16447
            //      8192        4096    2048    1:L     2       1:B     0           63          20543
            //      12287       6143    -       1:L     2       1:B     4095        4032        24512
            //      12288 ����      ����
        }   

    }

    //---------------------------------------------------
    //for 16K FULL Transpose (Upper 8K + Lower 8K) base                 //son-32KPIN-20200228 
    //for 12K FULL Transpose (Upper 8K + Lower 4K) base                 //son210319
    //---------------------------------------------------
    // �Ϻ����ε� 16K �� ���� �Ϻο� box�� 2���ۿ� �����Ƿ� �Ϻ� Hw BoxNum�� 2���� ���� �� ����.
    // ���������� pin ������ 32K ���̽��� ��ġ�� ������, ��� C, D �Ϻ� C, D box�� ���� ���� Ư¡.
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS //son230216_1
            || SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS)     //son210319 //son230216_1
    {

        //son ���, �Ϻ� ������� PartBox C, D�� ����. 
        if (nHwPartBox_forCheck >= 2) //son210514
            return -1;      

       // PIN_TOT_16K_TRANS �� �Ϻν����� QD 16385�� ����.
        // �Ϻν������� QD 8193, QD 4097 �� ���̽��� �����ϴ�.
        // Pin ��ġ�� 32K Full�� ������ �����ϴ�. 

        // 16K FullTrans -------------------------------
        //
        //ex) PinTotMax�� 16K, �Ϻν����� QD 8193(DD 4097, SD 2049), ��� 2box, �Ϻ� 2box
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
        //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==�Ϻν�����
        //      12222       6111    3071    1:L     2       0:A                             16510
        //      12287       6143     -      1:L     2       0:A     4095        63          16447
        //      
        //      12288       6144    3072    1:L     3       1:B     0           63          20543
        //      16318       8159    4095    1:L     3       1:B                             24449
        //      16383       8191     -      1:L     3       1:B     4095        4032        24512
        //      16384����      ����
        
        //ex) PinTotMax�� 16K, �Ϻν����� QD 4097(DD 2049, SD 1025), ��� 1box, �Ϻ� 2box
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
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===�Ϻν�����
        //      8126        4063    2047    1:L     1       0:A                             16510
        //      8191        4095     -      1:L     1       0:A     4095        63          16447
        //      
        //      8192        4096    2048    1:L     2       1:B     0           63          20543
        //      12222       6111    3071    1:L     2       1:B                             24449
        //      12287       6143     -      1:L     2       1:B     4095        4032        24512
        //      12288 ����      ����
        

        // 12K FullTrans -------------------------------  //son210319
        //  
        //ex) PinTotMax�� 16K, �Ϻν����� QD 8193(DD 4097, SD 2049), ��� 2box, �Ϻ� 1box
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
        //      8192        4096    2048    1:L     2       0:A     0           4032        20416 <==�Ϻν�����
        //      12222       6111    3071    1:L     2       0:A                             16510
        //      12287       6143     -      1:L     2       0:A     4095        63          16447
        //      12288����      ����
        //      
        
        //ex) PinTotMax�� 16K, �Ϻν����� QD 4097(DD 2049, SD 1025), ��� 1box, �Ϻ� 1box
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
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===�Ϻν�����
        //      8126        4063    2047    1:L     1       0:A                             16510
        //      8191        4095     -      1:L     1       0:A     4095        63          16447
        //      8192 ����      ����

    }

    //---------------------------------------------------
    //for 8K FULL Transpose (Upper 4K + Lower 4K) base              //son210319
    //---------------------------------------------------
    // �Ϻ����ε� 8K �� ���� �Ϻο� box�� 1���ۿ� �����Ƿ� �Ϻ� Hw BoxNum�� 1���� ���� �� ����.
    // ���������� pin ������ 32K ���̽��� ��ġ�� ������, ��� B, C, D �Ϻ� B, C, D box�� ���� ���� Ư¡.
    if (SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)     //son210319 //son230216_1
    {

        //son ���, �Ϻ� ������� PartBox B, C, D�� ����. 
        if (nHwPartBox_forCheck >= 1) //son210514
            return -1;

        // PIN_TOT_8K_TRANS  �Ϻν������� QD 4097 �� �����ϴ�.
        // Pin ��ġ�� 32K Full�� ������ �����ϴ�. 
        
        //ex) PinTotMax�� 8K, �Ϻν����� QD 4097(DD 2049, SD 1025), ��� 1box, �Ϻ� 1box
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
        //      4096        2048    1024    1:L     1       0:A     0           4032        20416 <===�Ϻν�����
        //      8126        4063    2047    1:L     1       0:A                             16510
        //      8191        4095     -      1:L     1       0:A     4095        63          16447
        //      8191����      ����
        

    }

    //--------------------------
    // ���� HwPin ���� ��� 
    //--------------------------
    // ���: 32K Upper & Lower, 
    //       24K_FULL Upper & Lower, 
    //       20K Upper,
    //       24K Upper  



    if (nHwBoxType == TYPE_AC)  // 0
    {
        // TYPE_AC �� �� SwPin-> HwPin
        nHwLine = (MAX_LINE_COL -1) - nSwCol;
        nHwCol  = nSwLine;
    }

    // Upper_B, Upper_D, Lower_B, Lower_D�� �ɹ�ġ�� �����ϹǷ� TYPE_BD�� ����
    else        // TYPE_BD : 1
    {
        // TYPE_BD �� �� SwPin-> HwPin
        nHwLine = nSwCol;
        nHwCol  = (MAX_LINE_COL -1) - nSwLine;
    }

    nHwBoxPin = (nHwLine * MAX_LINE_COL + nHwCol);      // Box�� HwPin ��ȣ
    nHwPin = nHwBoxPin + (nHwPartBox * MAX_BOX_PIN)     // BoxPin + Box Offset 
                        + (nPart * MAX_PART_PIN);       // + HW PartOffset (upper size)

    return nHwPin; 

}


//son201218 QDType�� �Ϻν����������� QD ������ UpperPartSize ����.
int CModeSet12::getQD_UpperPartSize()
{
    UINT nQD_UpperPartSize;

    //son ��Ϻ� QD/DD/SD ��� ���ÿ���   QD �Ϻν����� 16385�� ����
    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {
        nQD_UpperPartSize = (4*MAX_BOX_PIN);        
        return nQD_UpperPartSize;
    } //son210514

    //son ��ù��° boxType�� SKIP �� �� ����.
    ASSERT(SysSet12.m_nPinQDUpper!= BOX_SKIP); //son241007-BSW-9

    switch (SysSet12.m_nPinQDUpper)
    {
        case BOX_SD:
            // SD�ε� UpperPartSize 2048�̸� ��� 1024�� 2box�̹Ƿ� 
            // �Ϻν������� QD �������δ� 8193�� �Ǿ�� ��. �� UpperPartSize�� *4�ؼ� 8192�� �����Ѵ�.
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
            // ��� Sw Pin ���� == ��� SW ��Ʈ�� ������
            // �Ϲ������δ� MAX_PART_PIN�� nQD_UpperPartSize ���� 16K�� ������
            // ���� nQD_UpperPartSize�� 8K��  �޶����ٸ�, ��� HW�� 16K�ε� ��� SW pin�� 8K ������ ����Ѵٴ� ����.
            nQD_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;     
            break;

    }

    if (nQD_UpperPartSize > MAX_PART_PIN)   //son201218 16384 �̻��̸� 16384�� defaultó�� 
        nQD_UpperPartSize = (4*MAX_BOX_PIN);        
        //return -1;

    //-------------------------------------------------------------------
    //son ��� 3�ڽ�, �Ϻν������� 12289 base�� ��� (��� 4th skip �ƴ�) 
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS      //son210531 U 12 + L 4 = 16, skip ����
            || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS) //son230714 U 12 + L 8 = 20, skip ����.
    {
        //son230719 PIN_TOT_16K_U12K_L4K_UP_TRANS �����ϰ� save�ÿ� ����׸�忡�� ���� ���� ������ ���� ���� ����.
        ////son201218 Debug ���� err check                                                          
        //ASSERT(nQD_UpperPartSize == (3 * MAX_BOX_PIN)           // 12288
        //    || nQD_UpperPartSize == (3 * MAX_BOX_PIN/2)         // 6144
        //    || nQD_UpperPartSize == (3 * MAX_BOX_PIN/4));       // 3072

        //son201218 Release ���� err ��å: �Ϻ� �������� ������ ������ ���� ���ٸ� 12288�� default�� �����Ѵ�.
        if (nQD_UpperPartSize != (3 * MAX_BOX_PIN)                  // 12288�� �ƴϰ�
                    && nQD_UpperPartSize != (3 * MAX_BOX_PIN/2)     // 6144�� �ƴϰ�
                    && nQD_UpperPartSize != (3 * MAX_BOX_PIN/4))    // 3072�� �ƴϸ�
            nQD_UpperPartSize = (3 * MAX_BOX_PIN);      
    }

    //---------------------
    //son ���� ���̽�
    else
    {
        ASSERT(nQD_UpperPartSize == (4*MAX_BOX_PIN)         // 16384
            || nQD_UpperPartSize == (2*MAX_BOX_PIN)         // 8192
            || nQD_UpperPartSize == MAX_BOX_PIN);           // 4096
        //  || nQD_UpperPartSize == (MAX_BOX_PIN/2)         // 2048
        //  || nQD_UpperPartSize == (MAX_BOX_PIN/4)         // 1024

        //son201218 Release ���� err ��å: �Ϻ� �������� ������ ������ ���� ���ٸ� 16384�� default�� �����Ѵ�.
        if (nQD_UpperPartSize != (4*MAX_BOX_PIN)            // 16384�� �ƴϰ�
                    && nQD_UpperPartSize != (2*MAX_BOX_PIN)     // 8192�� �ƴϰ�
                    && nQD_UpperPartSize != MAX_BOX_PIN)        // 4096�� �ƴϸ�
                //  && nQD_UpperPartSize != (MAX_BOX_PIN/2)     // 2048�� �ƴϸ�
                //  && nQD_UpperPartSize != (MAX_BOX_PIN/4) // 1024�� �ƴϸ�
            nQD_UpperPartSize = (4*MAX_BOX_PIN);        
    }

     
    return nQD_UpperPartSize;
}

// JYK 20200117
// QD, SD, DD swPin��  QD ��������  ��ȯ�ؼ� �����Ѵ�.  
// parameter  : nSwPin      0���� �����ϴ� SwPin(JigPin)
// return val : nQdSwPin    QD SwPin (QD, DD, SD -> QD) ���� ��ȯ�ؼ� ����  
//                         ���� QDType�� QD�̸� ���� swPin ��  �״��  ���ϵȴ�.
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

// �Ϻν����ɰ��� Sw dependent�ؼ� DD, SD�� ���ΰ����� �Է¹��� ����� QD  �������� �÷��� 
// �������� getQdSwPin()�� ����Ѵ�. ���������� getQD_UpperPartSize()�� �����.
// 20200615: �Ϻν������� ���̴� ���̽��� ��� box �� �Ϻθ� ��� ���ϴ� ���� �����ؼ� ������ �ڵ���.
//           �����δ� box �� ��� ���� QD �� ���� BDL�� DD���� ����Ѵٴ����� ���� �Ϻν������� �̿��ϰ� �ǹǷ�
//           box range�� ������� �Ϻν������� ������ �� �ִ°� �´�.  
//           ���� getQdSwPin_Fixed()�� �̿��ϵ��� ������ �ʿ�. getQdSwPin_Fixed�� HW �Ϻν������� 16385�� �����Ѵ�.
//           ������ HW �Ϻν������� 16385 ������ �´�. 4K, 8K, 16K, 32K ������� ��� 16384 +1��
int CModeSet12::getQdSwPin(UINT nSwPin)
{
    int nQdSwPin = 0;

    // �⺻ range check  
    if (nSwPin < 0 || nSwPin >= MAX_PIN) 
        return -1;


    //son QD/DD/SD Block Set�� ���� ��� Box 0�� ������ ����. ���⼭ ����ó���ϰ� ����
    if( SysSet12.m_nBlockSet1 == 1 ) //sylee210402 
    {
        return getQdSwPin_BlockSet1(nSwPin);
    }


    // ��� Sw Pin ����
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1; 


    //son SD, DD��  jigPin �������� �ٿ��� �Ϻν������� �Է¹��� ���̽��̹Ƿ�  QD �������� �ٽ� �÷���
    UINT nQD_UpperPartSize = getQD_UpperPartSize(); 


    // Line(Card), Column(CardPin) ��� 0���� ����. 
    
    
    // ��� 
    //if(nSwPin < nQD_UpperPartSize)
    if(nSwPin < nOrg_UpperPartSize)     //son201218  ���DD, �Ϻ� QD �� hwPin ���������� ����.
    {
        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, 0, 0);        //son201218

        if (SysSet12.m_nPinQDUpper == BOX_DD)   
            return getQdPin_fromDdPin(nSwPin, 0, 0);        //son201218

        //son201218  QD��� ������ ������ nSwPin�� �����Ѵ� .

    }

    // �Ϻ� 
    else
    {
        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);       //son201218

        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getQdPin_fromDdPin(nSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);       //son201218

        // QD�� ��� 
        nSwPin = (nSwPin - nOrg_UpperPartSize) + nQD_UpperPartSize;         //son201218

    }

    return nSwPin;
}


//son QD/DD/SD Block Set�� ���� ��� Box 0�� ������ ����.
int CModeSet12::getQdSwPin_BlockSet1(UINT nSwPin) //son241007-BSW-9
{
    //son QD/DD/SD Block Set�� ���� ��� Box 0�� ������ ����. ���⼭ ����ó���ϰ� ����
    if( SysSet12.m_nBlockSet1 != 1 ) //sylee210402 
        return -1;

    if(sPinBlockRange[m_nBlockPo1].nBlockType== BOX_SD) //tran  //sylee210402
        return getQdPin_fromSdPin(nSwPin, 0, 0); 

    if(sPinBlockRange[m_nBlockPo1].nBlockType== BOX_DD) 
        return getQdPin_fromDdPin(nSwPin, 0, 0);   
    
    return nSwPin;  // BOX_QD
    
}

// �Ϻν����� Offest��  QD, DD, SD  ������� ���.  
// ��, �Ϻν������� DD, SD ������ �پ���� �ʴ� ���
// getQdSwPin_Fixed()�� ����Ѵ�. (�Ϻν����� -1) ���� �׳� UpperPartSize�� �����. 
int CModeSet12::getQdSwPin_Fixed(UINT nSwPin)
{
    int nQdSwPin = 0;


    // �⺻ range check  
    if (nSwPin < 0 || nSwPin >= MAX_PIN) 
       return -1;

    // ��� Sw Pin ����
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;   


    // 16K��, 32K��  Hw �Ϻ� Offset�� MAX_PART_PIN(16384) ����
    UINT nHW_UpperPartSize = MAX_PART_PIN;  //son Fixed!!!


    // Line(Card), Column(CardPin) ��� 0���� ����.
    
    // ��� 
    if(nSwPin < nOrg_UpperPartSize)
    {
        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, 0, 0);        //son201218

        if (SysSet12.m_nPinQDUpper == BOX_DD)   
            return getQdPin_fromDdPin(nSwPin, 0, 0);        //son201218

        //  QD��� ������ ���� nSwPin�� �����Ѵ� .
        nQdSwPin = nSwPin;
    }

    // �Ϻ� 
    else
    {
        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getQdPin_fromSdPin(nSwPin, nOrg_UpperPartSize, nHW_UpperPartSize);        //son201218

        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getQdPin_fromDdPin(nSwPin, nOrg_UpperPartSize, nHW_UpperPartSize);        //son201218

        // QD�� ��� 
        nQdSwPin = (nSwPin - nOrg_UpperPartSize) + nHW_UpperPartSize;  //HW �Ϻ� Offset�� 16384 �Ǵ� 8192

    }

    return nQdSwPin;

}



//son201218: SD -> QD Pin���� Ȯ�뺯ȯ
int CModeSet12::getQdPin_fromSdPin(int nSwPin, int nOffset, int nQdOffset)   //son190902 32KPIN 
{
    int nBoardPinCnt, nBoardNo, nPinNo;
    int nQdSwPin = 0;

    nBoardPinCnt = MAX_SD_DD_CARD_PIN;                  // SD BoardPinCnt : 32
    nBoardNo = (nSwPin - nOffset) / nBoardPinCnt;       // SD BoardNO
    nPinNo = (nSwPin - nOffset) % nBoardPinCnt;         // SD PinNo

    // SD�� Ȧ�� Line(Board or Card)�� �ǳ� �ٰ� ¦�� ���θ�(MAX_BOX_LINE *2) ����ϰ�
    // ¦�� ���ξȿ����� ¦�� ĭ�� ���ȴ�. (nPinNo * 2) 
    nQdSwPin = nQdOffset + nBoardNo * MAX_BOX_LINE *2 + nPinNo * 2; // Hw �Ϻ� Offset�� 16384(MAX_PART_PIN) ����.
    return nQdSwPin;

}

//son201218: DD -> QD Pin���� Ȯ�뺯ȯ
int CModeSet12::getQdPin_fromDdPin(int nSwPin, int nOffset, int nQdOffset)   //son190902 32KPIN 
{
    int nBoardPinCnt, nBoardNo, nPinNo;
    int nQdSwPin = 0;

    nBoardPinCnt = MAX_SD_DD_CARD_PIN;              // DD BoardPinCnt : 32
    nBoardNo = (nSwPin - nOffset) / nBoardPinCnt;   // DD BoardNO               
    nPinNo   = (nSwPin - nOffset) % nBoardPinCnt;   // DD PinNo                 

    // DD ¦�� �����̸� Ȧ�� ĭ�� �ǳʶٰ� ¦�� ĭ�� ���ȴ�. (nPinNo * 2) 
    nQdSwPin = nQdOffset + nBoardNo * MAX_BOX_LINE + nPinNo * 2;    

    // DD�� Ȧ�� �����̸� ¦��ĭ�� �ǳʶٰ� Ȧ�� ĭ�� ���ȴ�. (nPinNo * 2 +1)
    if(nBoardNo % 2 == 1)
        nQdSwPin++;
    return nQdSwPin;
}



//-----------------------------------------------
//son nHwPin ���� �´� SwPin(JigPin)�� �����Ѵ�. 
//-----------------------------------------------
// parameter  : nHwPin  Input ����, 0���� �����ϴ� HwPin(SwitchCard pin)
// return val : nSwPin  nHwpin�� �´� SW Jig Pin
int CModeSet12::make32kSwPin(UINT nHwPin)
{
    UINT nPart, nHwBox, nHwPartBox, nHwBoxType;
    UINT nHwBoxPin, nHwLine, nHwCol;
    UINT nSwBoxPin, nSwLine, nSwCol;
    UINT nQdSwPin;      


    //son-8K4097-20200605: SWMaxPin�� 8K����, HW�� 16K, QDHW ���� �Ϻν����� ���ÿ� ����.
    if(SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN   //son SW MaxPin�� 8K, HW�� 16K ���̽�
         || (SysSet12.m_nPinTotMax == PIN_TOT_12K)            //son210202: 12K ���� �߰�
         || (SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY) //son211020 8K���� ��θ� �����ϴ� ���̽� (f550j ����ý�) 
         || (SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)  //son230221
         || (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG ))   //son230316 
    {

        //son 8K�� �����̹Ƿ� HwPin-> SwPin��ȯ�� ����.
        //son �Ϻν������� �̹� QD �������� ���� ����� QdSwPin ���� ����
        //son UpperPartSize�� QD, DD, SD�� ���� �������� �ƴ϶� HW �������� (16384:32K �Ǵ� 8192:QD)����  ó���Ѵ�.
        return getDdSdSwPin_Fixed(nHwPin);  
    }

    //---------------------------
    // HwPin ���� �м� 
    
    // 32K HW�ε� 32K�̻��̰ų�, 0���� ���� hwPin�Է��� -1�� ó���Ѵ�.
    if (nHwPin < 0 || nHwPin > MAX_PIN)  
        return -1;


    // Part(Upper or Lower) > Box (A~D) > Line (0~63) > Column (0~63)
    nPart  = nHwPin / MAX_PART_PIN;
    nHwBox = nHwPin / MAX_BOX_PIN;       // ������ 4096
    if (nHwBox < 0 || nHwBox >= MAX_BOX) // �ش� HW �ڽ���  �̻�� �����̸� swPin����  -1�� ����
        return -1;
 

    nHwPartBox = nHwBox % MAX_BOX_PER_PART; // Hw Part�� Box ��ȣ�� ��ȯ.
    nHwBoxType = nHwPartBox % 2;            // ¦���� TYPE_AC, Ȧ���� TYPE_BD

    nHwBoxPin  = nHwPin % MAX_BOX_PIN;      // Box�� HwPin ��ȣ
    nHwLine    = nHwBoxPin / MAX_BOX_LINE;
    nHwCol     = nHwBoxPin % MAX_BOX_LINE;

    // QDType�� �Ϻ�Jig StartPin ������ Jig(SW) Part ������ Ȯ���Ѵ�.
    UINT nSw_UpperPartSize = getQD_UpperPartSize();



    // nSw_UpperPartSize�� MAX_BOX_PIN:4096�� ����� 
    UINT nSwUpperBoxNum = nSw_UpperPartSize / MAX_BOX_PIN;  // nSw_UpperPartSize    nSwUpperBoxNum
                                                            //  16384(16K)          4
                                                            //  12288(12K)          3
                                                            //  8192 (8K)           2
                                                            //  4096 (4K)           1

    // �Ϻν����� �����Ͽ� ��� box�� �޺κ� �̻�뿵���� �ش��ϴ� pin�� swPin���� -1�� �����Ѵ�.
    if (SysSet12.m_nBlockSet1 != 1)
        if (nPart == PART_UPPER && nHwPartBox >= nSwUpperBoxNum)
            return -1;

    // ����� ���� PinBlock��ȣ ����, nHwPin���� ����� �� �ִ�.
    m_nBlockPo1 = nHwPin / MAX_BOX_PIN +1;   //son m_nBlockPo1�� 1���� �����ϹǷ� +1    //son210514

    //son �Ϻν������� �� ���� ��Ϻ� QD/DD/SD�� ���� ��� Block1 ���̽��� �ٲ㼭 ����ϰ� 
    //    ���߿� ������� StartPin�� ���ؼ� ����ؾ� �Ѵ�.
    //son20514  QD/DD/SD BlockSet  ���¶�� �Ϻιڽ����� ���θ�  m_nBlockPo1�� �Ǵ��Ѵ�.
    int nHwPartBox_forCheck = nHwPartBox;       //son210514
    if (SysSet12.m_nBlockSet1 == 1)             //son210514
        nHwPartBox_forCheck = m_nBlockPo1 -5;


    //-----------------------------------------
    //for 24K (12 Upper Trans + 8K Lower Trans)  
    //-----------------------------------------
    //son Upper 4��° Box(nHwParBox 3)�� ����.
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
    // �Ϻ����ε� 20K, 24K �� ���� swPin ��ġ������  hwPin��ġ ������ �����ϰ� �����ؼ� �����Ѵ�.
    // �Ϻ����ε� 32K or 24K_FULL_TRANS�̰ų�,  �������  ���� �� �ڵ� ��������  ����.
    //if    (SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR) //son230216_1
    if(IS_UPPER_TRANS(SysSet12.m_nPinTotMax))  //son210531
    {
        //son ��� 12K Trans , �Ϻ� 4K Origin�� ���� Upper 4��° Box(nHwParBox 3)�� ����.
        if ((SysSet12.m_nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS 
                        || SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS)    //son210603
                && nPart == PART_UPPER )    //son210531  
        {
            if (nHwPartBox == 3)    //son210531
                return -1;
        }

        //son ��� 8K Trans , �Ϻ� 4K Origin�� ���� Upper 3,4��° Box(nHwParBox 2,3)�� ����.
        if (SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS  && nPart == PART_UPPER )    //son210826  
        {
            if (nHwPartBox == 2 || nHwPartBox == 3)    //son210826
                return -1;
        }

        //son �Ϻ� �ڽ����
        //son20514  QD/DD/SD BlockSet  ���¶�� �Ϻιڽ����� ���θ�  m_nBlockPo1�� �Ǵ��Ѵ�.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                //son210514
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )    //son210514 
        {
            //son UpperTrans�̰� �Ϻο� box�� 1���ۿ� ���� ���  BoxNum�� 1���� ���� �� ����.
            //    IS_UPPER_TRANS(SysSet12.m_nPinTotMax)���� �̹� ������ üũ��.
            if (IS_LOWER_4K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 1) //son210826 
                return -1; // swPin���� -1�� ����

            //son UpperTrans�̰� �Ϻο� box�� 2���ۿ� ���� ���  Hw BoxNum�� 2���� ���� �� ����.
            if (IS_LOWER_8K(SysSet12.m_nPinTotMax) && nHwPartBox_forCheck >= 2) //son210826 
                return -1; // swPin���� -1�� ����

            //--------------------------------
            // 20K, 24K Lower SwPin ���� ��� 
            //--------------------------------
            // 20K, Lower Part�� rnSwBoxPin ��� rnHwBox���� rnSwPin�� ����ؼ� ���⼭ �����Ѵ�.
            nQdSwPin  = nHwBoxPin + (nHwPartBox_forCheck * MAX_BOX_PIN) // BoxPin + Box Offset  //son210514 
                + (nPart * nSw_UpperPartSize);  // + Part Offset

            // QD ���� swPin�� QdType(QD, DD, SD)�� �°� ��ȯ�ؼ� �����Ѵ�.  JYK 20200117
            return getDdSdSwPin(nQdSwPin);  // QD -> QD, DD, SD
        }
    }

    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {

        //son nHwPin�� Block 1�������� �����Ѵ�.
        nHwPin = nHwPin - (MAX_BOX_PIN * (m_nBlockPo1 -1));

        // Part(Upper or Lower) > Box (A~D) > Line (0~63) > Column (0~63)
        nPart  = nHwPin / MAX_PART_PIN;
        nHwBox = nHwPin / MAX_BOX_PIN;       // ������ 4096

        nHwPartBox = nHwBox % MAX_BOX_PER_PART; // Hw Part�� Box ��ȣ�� ��ȯ.

        nHwBoxType = sPinBlockRange[m_nBlockPo1].nHwBoxType;    //son210514 ****


        nHwBoxPin  = nHwPin % MAX_BOX_PIN;      // Box�� HwPin ��ȣ
        nHwLine    = nHwBoxPin / MAX_BOX_LINE;
        nHwCol     = nHwBoxPin % MAX_BOX_LINE;
    } //son210514


    //----------------------------------------------
    //for 24K_FULL_TRANS (Upper 16K + Lower 8K) base                
    //----------------------------------------------
    //son-32KPIN-20200103 
    // �Ϻ����ε� 24K_FULL_TRANS �� ���� �Ϻο� box�� 2���ۿ� �����Ƿ� �Ϻ� Hw BoxNum�� 2���� ���� �� ����.
    if( SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son �Ϻ� �ڽ����
        //son20514  QD/DD/SD BlockSet  ���¶�� �Ϻιڽ����� ���θ�  m_nBlockPo1�� �Ǵ��Ѵ�.
        if( (SysSet12.m_nBlockSet1 == 1 && m_nBlockPo1 >= 5)                //son210514
                || (SysSet12.m_nBlockSet1 != 1 && nPart == PART_LOWER) )    //son210514 
        {
            if (nHwPartBox_forCheck >= 2) //son210514
                return -1;

            //24K_FULL_TRANS�� Pin ��ġ�� 32K�� �����ϹǷ� box range�� OK��� 32K�� �����ϰ� pin ���� �����Ѵ�.
        }
    }

    //----------------------------------------------
    //for 16K_FULL_TRANS (Upper 8K + Lower 8K) base                 
    //for 12K_FULL_TRANS (Upper 8K + Lower 4K) base                 
    //----------------------------------------------
    //son-32KPIN-20200228 
    // 16K_FULL_TRANS �� ���� ���, �Ϻ� ���  box�� 2��. �Ϻ� Hw BoxNum�� 2���� ���� �� ����.
    if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS //son230216_1
            || SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS) //son230216_1
    {
        if (nHwPartBox_forCheck >= 2)
            return -1;

        //24K_FULL_TRANS�� Pin ��ġ�� 32K�� �����ϹǷ� box range�� OK��� 32K�� �����ϰ� pin ���� �����Ѵ�.
    }
/*
    //----------------------------------------------
    //for 12K_FULL_TRANS (Upper 8K + Lower 4K) base                 
    //----------------------------------------------
    // 12K_FULL_TRANS �� ���� ��� box�� 2��, �Ϻ� 1��. 
    if (SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS)    //son210319 //son230216_1
    {
        if (nPart == PART_UPPER && nHwPartBox_forCheck >= 2)    //son ���box 2�� //son210514
            return -1;

        if (nPart == PART_LOWER && nHwPartBox_forCheck >= 1)    //son �Ϻ�box 1�� //son210514
            return -1;
    }
*/

    //----------------------------------------------
    //for 8K_FULL_TRANS (Upper 4K + Lower 4K) base              
    //----------------------------------------------
    if (SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS) //son210319 //son230216_1
    {
        // 8K_FULL_TRANS �� ���� ���, �Ϻ� ���  box�� 1��. Hw BoxNum�� 1���� ���� �� ����.
        if (nHwPartBox_forCheck >= 1) //son210514
            return -1;

        //8K_FULL_TRANS�� Pin ��ġ�� 32K�� �����ϹǷ� box range�� OK��� 32K�� �����ϰ� pin ���� �����Ѵ�.
    }


    //--------------------------
    // ���� SwPin ���� ��� 
    //--------------------------
    // ���: 32K Upper & Lower, 
    //       24K_FULL Upper & Lower, 
    //       20K Upper,
    //       24K Upper  
    
    // HW Pin�� ���� ������� ������Ű�� 4096 ����� offset�� �����ϸ�
    // Upper_A, Upper_C, Lower_A, Lower_C�� �ɹ�ġ�� �����ϹǷ� TYPE_AC�� ����.
    if (nHwBoxType == TYPE_AC)  // 0
    {
        nSwLine = nHwCol;
        nSwCol  = (MAX_LINE_COL -1) - nHwLine;
    }

    // Upper_B, Upper_D, Lower_B, Lower_D�� �ɹ�ġ�� �����ϹǷ� TYPE_BD�� ����
    else        // TYPE_BD  : 1
    {
        nSwLine = (MAX_LINE_COL -1) - nHwCol;
        nSwCol  = nHwLine;
    }
    nSwBoxPin = (nSwLine * MAX_LINE_COL + nSwCol);          // Box�� SwPin ��ȣ
    nQdSwPin    = nSwBoxPin + (nHwPartBox * MAX_BOX_PIN)    // BoxPin + Box Offset 
                          + (nPart * nSw_UpperPartSize);    // + Part Offset


    //son �Ϻν������� �� ���� ��Ϻ� QD/DD/SD�� ���� ��� 
    if( SysSet12.m_nBlockSet1 == 1 ) //son210514
    {
        //son ���� SwPin�� Block1�������� ����� ����� ���� ����� Start��ȣ�� ���ؼ� ��� ��ġ�� ������ �����Ѵ�.
        int DdSdSwPin = getDdSdSwPin(nQdSwPin) + (sPinBlockRange[m_nBlockPo1].nStartPin-1);
        return DdSdSwPin; 
    } //son210514

    // QD ���� swPin�� QdType(QD, DD, SD)�� �°� ��ȯ�ؼ� �����Ѵ�.  JYK 20200117
    return getDdSdSwPin(nQdSwPin);  // QD -> QD, DD, SD
}


// QD ���� swPin�� QdType(QD, DD, SD)�� �°� ��ȯ�ؼ� �����Ѵ�.  JYK 20200117
// parameter  : nQdSwPin      0���� �����ϴ� SwPin(JigPin), QD ����
// return val : nDdSdSwPin    ���� QDType�� �´� SwPin���� (QD -> QD, DD, SD) ��ȯ�ؼ� ���� 
//                            ���� QDType�� QD�̸� ���� swPin�� �״��  ���ϵȴ�.

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

    // �⺻ range check  
    if (nQdSwPin < 0 || nQdSwPin >= MAX_PIN) 
        return -1;


    //son ��Ϻ� QD/DD/SD ���ô� �̷��� �����Ѵ�.
    //    ��Ϻ� QD/DD/SD �����ÿ��� �Ϻν������� ������� ����.
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
                //  QD��� ������ ���� nQdSwPin�� �����Ѵ� .
                return nQdSwPin;
                break;
        }
    } //son210514


    // ��� Sw Pin ����
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;
    UINT nQD_UpperPartSize  = getQD_UpperPartSize();




    // ��� 
    if(nQdSwPin < nQD_UpperPartSize)
    {
        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getSdPin_fromQdPin(nQdSwPin, 0, 0);      //son201218

        if (SysSet12.m_nPinQDUpper == BOX_DD)
            return getDdPin_fromQdPin(nQdSwPin, 0, 0);      //son201218

        //  QD��� ������ ���� nQdSwPin�� �����Ѵ� .
    }

    // �Ϻ� 
    else
    {

        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getSdPin_fromQdPin(nQdSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);     //son201218

        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getDdPin_fromQdPin(nQdSwPin, nOrg_UpperPartSize, nQD_UpperPartSize);     //son201218

        // QD�� ��� 
        nQdSwPin = (nQdSwPin - nQD_UpperPartSize) + nOrg_UpperPartSize;     //son201218

    }


     return nQdSwPin;

}


//son HW UpperPartSize�� QD, DD, SD�� ���� �������� �ƴ϶� HW �������� 16384 ��������  ó���Ѵ�.
int CModeSet12::getDdSdSwPin_Fixed(UINT nHwPin)
{


    int nDdSdSwPin = 0;

    
    // �⺻ range check  
    if (nHwPin < 0 || nHwPin >= MAX_PIN) 
        return -1;

    // ��� Sw Pin ����
    UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1;


    // 16K��, 32K��  QD ���� �Ϻ� �������̸� Hw �Ϻ� Offset�� 16384 ����.  (���� HW Switch)
    UINT nHW_UpperPartSize = (MAX_BOX_PIN *4);  // 16384

//son230221 begin:
    //son K600 ��� 3, 4 box��  ���� �Ϻο��� ���Ǵ� ��� (BDL �Ϻ�)
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_ORG 
            && SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K)
    {
        //son box 3-> sw box 9��  box 4-> sw box 10���� �ٲ��ش�.
        //    box 9�� �Ϻ� 5��° box, box 10�� �Ϻ� 6��° box��
        if (nHwPin >= 8192 && nHwPin <= 16383)
            nHwPin = nHwPin + (MAX_BOX_PIN * 6); 
    }
//son230221 end

    // ��� 
    if(nHwPin < nHW_UpperPartSize) 
    {

        if (SysSet12.m_nPinQDUpper == BOX_SD)
            return getSdPin_fromQdPin(nHwPin, 0, 0);      //son201218


        if (SysSet12.m_nPinQDUpper == BOX_DD)
            return getDdPin_fromQdPin(nHwPin, 0, 0);      //son201218

        //  QD ��ζ�� ������ ���� nHwPin�� �����Ѵ� .
        nDdSdSwPin = nHwPin;
    }

    // �Ϻ� 
    else
    {

        if (SysSet12.m_nPinQDLower == BOX_SD)
            return getSdPin_fromQdPin(nHwPin, nOrg_UpperPartSize, nHW_UpperPartSize);      //son201218


        if (SysSet12.m_nPinQDLower == BOX_DD)
            return getDdPin_fromQdPin(nHwPin, nOrg_UpperPartSize, nHW_UpperPartSize);      //son201218


        // QD�� ��� 
        nDdSdSwPin = (nHwPin - nHW_UpperPartSize) + nOrg_UpperPartSize;

    }

    return nDdSdSwPin;


}


//son201218 QD -> SD Pin���� ��Һ�ȯ
int CModeSet12::getSdPin_fromQdPin(UINT nQdSwPin, int nOffset, int nQdOffset)
{
    int nQdBoardPinCnt, nQdBoardNo, nQdPinNo;
    int nDdSdSwPin = 0;

    nQdBoardPinCnt = MAX_QD_CARD_PIN;   // 64
    nQdBoardNo = (nQdSwPin - nQdOffset) / nQdBoardPinCnt;
    nQdPinNo = (nQdSwPin - nQdOffset) % nQdBoardPinCnt;

    // SD Ȧ�� ������ ������ ��� �д�.
    if((nQdBoardNo % 2) != 0)
        return -1;

    // SD ¦�� �����̸鼭 Ȧ�� ���� ��� �д�.
    if((nQdPinNo % 2) != 0)
        return -1;


    // SD->QD �ÿ� ¦�� ���θ� (MAX_BOX_LINE *2�ؼ�), ¦��ĭ�� (nPinNo *2)  �Ҵ������Ƿ� 
    // QD->SD �ÿ��� /2 �� ���� Board ������ ����ϰ� nQdPinNo/2 ���� pinNo�� ����Ѵ�.  
    // MAX_SD_DD_CARD_PIN: 32
    nDdSdSwPin = nOffset + (nQdBoardNo /2) * MAX_SD_DD_CARD_PIN + nQdPinNo / 2;
    return nDdSdSwPin;
}

//son201218 QD -> DD Pin���� ��Һ�ȯ
int CModeSet12::getDdPin_fromQdPin(UINT nQdSwPin, int nOffset, int nQdOffset)
{
    int nQdBoardPinCnt, nQdBoardNo, nQdPinNo;
    int nDdSdSwPin = 0;

    nQdBoardPinCnt = MAX_QD_CARD_PIN;   // 64
    nQdBoardNo = (nQdSwPin - nQdOffset) / nQdBoardPinCnt;
    nQdPinNo = (nQdSwPin - nQdOffset) % nQdBoardPinCnt;

    // ¦�� ����
    if((nQdBoardNo % 2) == 0)
    {
        // DD ¦�������� Ȧ�� pin�� ��� �д�.
        if((nQdPinNo % 2) != 0)
            return -1;

        // MAX_SD_DD_CARD_PIN: 32
        nDdSdSwPin = nOffset + nQdBoardNo * MAX_SD_DD_CARD_PIN + nQdPinNo / 2;
    }

    // Ȧ�� ����
    else
    {
        // DD Ȧ�������� ¦�� pin�� ��� �д�.
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

    //son sPinBlockRange[10]��  �����ϰ� "d:\\PinMap_PinBlockRange1.ini" ���Ϸ� ����Ѵ�
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

        //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
        int hwPin = PinMap[2][i];				//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)		//son230303
            nJigPinMap[hwPin] = i;    //son ���� �˻���. //son230303
  
     }

 

    FILE *fp; 
    char  fName[FILE_PATH_SIZE];  //son210514  200 -> FIlE_PATH_SIZE
    CString str; 

    ::ZeroMemory(&fName, sizeof(fName));
    //strcat( fName , "d:\\PinMap.txt" );
    strcat( fName , "d:\\log\\PinMap.txt" ); //son220715 d:\\log ������ �̵�

          
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
            if((SysSet12.m_nPinQDUpper!=BOX_SD)&&(SysSet12.m_nPinQDLower==BOX_SD))  //son190902 32KPIN:  enum ���� 
                                                                                    //       m_nPinQD-> m_nPinQDUpper�� ����
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
            else  if((SysSet12.m_nPinQDUpper==BOX_SD)&&(SysSet12.m_nPinQDLower!=BOX_SD))    //son190902 32KPIN:  enum ����
                                                                                            //      m_nPinQD-> m_nPinQDUpper�� ����
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
            else  if((SysSet12.m_nPinQDUpper==BOX_SD)&&(SysSet12.m_nPinQDLower==BOX_SD))    //son190902 32KPIN:  enum ����
                                                                                            //      m_nPinQD-> m_nPinQDUpper�� ����
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

/*  //sylee131203  ������ �����
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
               ��� 
               1�� ��� SD (1-1024), 2�� ��� DD (1025-3072) ������ ����� �ߴµ�,
               �����δ� 1�� ��� ��ġ�� DD�� ����Ǿ��ְ� (1025-2077), 2�� ��� ��ġ�� SD (1-1024)�� ����Ǿ� �ֽ��ϴ�.
               ��, SD,DD ������ ���� test file�� ������ s/w���� �¿� �ٲ��ִ� �̴ϴ�.

               ���   sd( block 1)   + dd  ( block 2)  ��    block 1�� 2  �ٲ۴�.
               �Ϻ�  dd ( block 3)  +  sd  ( block 4) 

*/


            //sylee131209 ������ ����� 

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
            //sylee131203 ������ ����� 
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

        //son230303 hwPin -> jigPin�� ã�� ���� ���� ����
        int hwPin = PinMap[2][i];			//son230303
        if (hwPin >= 0 && hwPin < MAX_PIN)	//son230303
            nJigPinMap[hwPin] = i;    //son ���� �˻���. //son230303

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



//son190902 32KPIN:  �ű� ����
// �Ϻν����� ������ �����Ѵ�.
void CModeSet12::OnSelchangeComboLowerStartPin() 
{
    // TODO: Add your control notification handler code here    
    CString str, strTemp;   
    // ���� combo �ڽ��� ��ġ Ȯ��
    int nModeLowerStart = m_combo_LowerJigStartPin.GetCurSel(); 
    // ���� combo box�� string�� '<Net Set>'�̶�� 
    // ����� �Ϻν����� ������ �������� �ʴ´�.
    m_combo_LowerJigStartPin.GetLBText(nModeLowerStart, str);    
    if (str == "<Not Set>")
    {
        //son201222
        strTemp.Format("OnSelchangeComboLowerStartPin() CurSel=%d  is \\<Not Set\\>!\n\n Check Login Error\n", nModeLowerStart);
        errMessageBox(7701, strTemp); //son230217_2
        return;
    } 
    // ����� ������ �Ϻν����� System ȯ�溯���� �����Ѵ�.
    SysSet12.m_nLowerJigStartPin = m_LowerStartPin.saList[nModeLowerStart].nPin;


    //son ��� QD/DD/SD ���� ���¿� ���� UpperEndJigPin ��ȣ�� ����ϰ� 
    //     user�� ������ �Ϻν����ɰ� ���ؼ� Offset�� �ִٸ� ���ο� �Ϻν����ɿܿ� Offset�� �����Ѵ�.
    //    �� Offset�� �ݿ��ؼ�  MakePinBlockRangeUserNo()����  �Ϻ��� ��� �ɵ��� �ٽ� ���ȴ�.
    if (SysSet12.m_nBlockSet1 == 1) //son241007-BSW-9 begin
    {
        int nUpperEndJigPin = CalcUpperEndJigPin_BlockSet();
        if (nUpperEndJigPin >= 0)
        {
            //son UpperEndJigPin ���� ū ������ ������ �Ǿ��ٸ� user�� Offset�� ������ �� 
            if (SysSet12.m_nLowerJigStartPin > (nUpperEndJigPin +1))
                SysSet12.m_nLowerJigStartPin_Offset = SysSet12.m_nLowerJigStartPin - (nUpperEndJigPin +1);

            else if (SysSet12.m_nLowerJigStartPin == (nUpperEndJigPin +1))  //son241124-BSW-9
                SysSet12.m_nLowerJigStartPin_Offset = 0;                    //son241124-BSW-9


            //son UpperEndJigPin ���� ���� �����δ� �Ϻν����� ������ �� ���ٰ� ��� ���
            else if (SysSet12.m_nLowerJigStartPin < (nUpperEndJigPin +1))
            {
                strTemp.Format("LowerJigStartPin cannot be set to a value less than %d \n", nUpperEndJigPin +1);
                AfxMessageBox(strTemp, MB_OK); 

                //son �Ϻν������� nUpperEndJigPin +1 �� default �����Ѵ�.  
                SysSet12.m_nLowerJigStartPin = nUpperEndJigPin +1;
                SysSet12.m_nLowerJigStartPin_Offset = 0;

                //son nModeLowerStart ��ġ�� nUpperEndJigPin +1 �� �ٲ۴�.
                nModeLowerStart = m_LowerStartPin.findItemNo(SysSet12.m_nLowerJigStartPin);
                if (nModeLowerStart >= 0)
                    m_combo_LowerJigStartPin.SetCurSel(nModeLowerStart);
            }
        }
    } //son241007-BSW-9 end

    // ����� �Ϻν����ɿ� �´� Upper box ������ ����.
    SysSet12.m_nUpperBox = findUpperBoxNum(nModeLowerStart);    //son240516

    // ����� �Ϻν����ɿ� �´� EndPin combo ����.
    initCombo_EndPin(SysSet12.m_nPinTotMax);  //son240516    //son241007-BSW-9 
}

//son240516 begin:
//son PIN_TOT_32K_ORG �� ������ ���� �Ϻν����� ������ SysSet12.m_nUpperBox�� Ȯ���ϰ� �̰ɷ� EndPin�� ����ؾ� �Ѵ�.
//    PIN_TOT_32K_ORG �� ������ EndPin�� ����ϰ� �׿��� ��쿡�� ���� TotMaxPin�� ����ϱ� ������. 
//    ���� TotMaxPin�� �ƴ϶� EndPin�� �̿��ϴ� �������� ����. (PIN_TOT_32K_ORG ���� ���� ACE500������ �ٷ���)
int CModeSet12::findUpperBoxNum(int nModeLowerStart) 
{
    int nUpperBoxNum = UPPER_BOX_NUM;   //default  //son241007-BSW-9 4:UPPER_BOX_NUM

    if (SysSet12.m_nPinTotMax == PIN_TOT_32K_ORG)   //son241007-BSW-9 
    {
        if (SysSet12.m_nBlockSet1 == 1) 
        {
            //son ��� pin block �߿� skip�� �ƴ� ��츸 ������ ����. 
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


//son240516  EndPin ������ �����Ѵ�.
//            32K Original PinMode������ ���.
void CModeSet12::OnSelchangeComboEndPin() 
{
    // TODO: Add your control notification handler code here    
    CString str, strTemp;   

    // ���� combo �ڽ��� ��ġ Ȯ�� (0~ 3)
    int nModeEndPin= m_combo_EndPin.GetCurSel(); 

    // ���� combo box�� string�� '<Net Set>'�̶�� 
    // ����� �Ϻν����� ������ �������� �ʴ´�.
    m_combo_EndPin.GetLBText(nModeEndPin, str);    
    if (str == "<Not Set>")
    {
        //son201222
        strTemp.Format("OnSelchangeComboEndPin() CurSel=%d  is \\<Not Set\\>!\n\n Check Login Error\n", nModeEndPin);
        errMessageBox(7701, strTemp); //son230217_2
        return;
    } 

    // ����� m_EndPin, m_nLowerBox �� FileSysInfo ������ �����Ѵ�. 
    SysSet12.m_nLowerBox = (nModeEndPin +1);   // �ڽ� �����̹Ƿ�  +1 �Ѵ�.
    SysSet12.m_nEndPin = m_nLowerEndBox_EndPin[nModeEndPin];
}

//son Upper 3 Combo
void CModeSet12::OnCombo21() //sylee210330-1 
{
    int nMode = m_combo21.GetCurSel();

    //son Upper 3�� SKIP���� ������ �Ǿ��ٸ� Upper4�� SKIP�̾�� �ϹǷ� 
    //    ���� skip���� �����ϰ� user�� Upper 4 �޺� �ڽ��� �ǵ��� ���ϰ� �ٲ۴�. 
    //    3�� SKIP�� 4���� QD/DD/SD �����ϴ� ���� ���̵��� �ʹ� ���� ȿ�뼺�� ����.
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


    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
    MakePinBlockRangeUserNo();//son241007-BSW-9
    initCombo_LowerStartPin(SysSet12.m_nPinQDUpper, SysSet12.m_nPinTotMax); //son241007-BSW-9
    initCombo_EndPin(SysSet12.m_nPinTotMax);    //son241007-BSW-9
}

//son Upper 4 Combo
void CModeSet12::OnCombo22() //sylee210330-1 
{
    int nMode = m_combo22.GetCurSel();
    SysSet12.m_nPinQDUpper4 = nMode+1;

    //son nQdType, nMaxPin�� �°�  �Ϻ�StartPin �޺��ڽ� data�� �ʱ�ȭ�Ѵ�.
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


//son210831 son �Ϻν������� ��� 8K�� ������ ��츦 �Ϻν��������� ������ ������ 
//          ����ؼ� üũ�Ѵ�. SelfTest, PinSearch���� �ӵ������� ����  ���
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

    //son �Ϻν������� �ְ�, ��� 8K�� ����ϴ� ���̽��� �����ϴ� Type
    if ( nPinTotMax == PIN_TOT_32K_TRANS || nPinTotMax == PIN_TOT_20K_UP_TR //son230216_1
         || nPinTotMax == PIN_TOT_24K_UP_TR || nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son BlockSet�� ����ϴ� ��� SysSet12.m_nPinQDUpper�����δ� üũ�Ұ�. 
        //    �̰��� SelfTest, PinSearch���� PinMap jigPin�� -1�̸� �ɷ����� �ڵ�� ó���ϵ��� �׳� �д�.
        if (SysSet12.m_nBlockSet1 != 1)
        {
            //son �Ϻν������� ��� 8K�� ������ ���
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

//son210831 son �Ϻν������� ��� 8K�� ������ ��츦 �Ϻν��������� ������ ������ 
//          ����ؼ� üũ�Ѵ�. SelfTest, PinSearch���� �ӵ������� ����  ���
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

    //son �Ϻν��������� ��� 4K�� ����ϴ� ���̽��� �����ϴ� Type
    if ( nPinTotMax == PIN_TOT_32K_TRANS || nPinTotMax == PIN_TOT_20K_UP_TR     //son230216_1
         || nPinTotMax == PIN_TOT_24K_UP_TR || nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
    {
        //son BlockSet�� ����ϴ� ��� SysSet12.m_nPinQDUpper�����δ� üũ�Ұ�. 
        //    �̰��� SelfTest, PinSearch���� PinMap jigPin�� -1�̸� �ɷ����� �ڵ�� ó���ϵ��� �׳� �д�.
        if (SysSet12.m_nBlockSet1 != 1)
        {
            //son �Ϻν������� ��� 4K�� ������ ���
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
