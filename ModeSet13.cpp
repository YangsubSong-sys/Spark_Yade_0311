// ModeSet13.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSet13.h"
#include "FileSysInfo.h"
#include "ModeSet12.h"
#include "Globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



extern CFileSysInfo  FileSysInfo01; 

extern CSysSet12  SysSet12;  //Pin SETUP 
extern CSysSet13  SysSet13;  
 
 extern int nFlagEstModelMake; //sylee150407 .//estimation
// extern int nFlagEstModelMake_HR; //sylee150407 .//estimation
 


/////////////////////////////////////////////////////////////////////////////
// CModeSet13 dialog


CModeSet13::CModeSet13(CWnd* pParent /*=NULL*/)
    : CDialog(CModeSet13::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeSet13)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}




void CModeSet13::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeSet13)
    	DDX_Control(pDX, IDC_EDIT_NEARBY_OPENRATE, m_edit_NearByOpenRate);     
    	                            //son230728 for 영풍SST
	    DDX_Control(pDX, IDC_CHECK_NEARBY_OPEN_MEAS, m_check_NearbyOpenMeas);   
	                                //son230712 for 영풍SST
        DDX_Control(pDX, IDC_COMBO101, m_combo1);
        DDX_Control(pDX, IDC_COMBO102, m_combo2);
        DDX_Control(pDX, IDC_COMBO103, m_combo3);
        DDX_Control(pDX, IDC_COMBO104, m_combo4);
        DDX_Control(pDX, IDC_COMBO105, m_combo5);
        DDX_Control(pDX, IDC_COMBO111, m_combo111);
        DDX_Control(pDX, IDC_COMBO112, m_combo_SparkMode);  
                                    //son200908 NEW_SPARK
        DDX_Control(pDX, IDC_COMBO113, m_combo113);
        DDX_Control(pDX, IDC_COMBO114, m_combo114);         
        DDX_Control(pDX, IDC_COMBO115, m_combo115);         
                                    //son230106_2 4W Fail Sort Option (for GTS)
        DDX_Control(pDX, IDC_CHECK1, m_check1);
        DDX_Control(pDX, IDC_CHECK2, m_check2);
        DDX_Control(pDX, IDC_CHECK3, m_check3);
        DDX_Control(pDX, IDC_CHECK4, m_check4);
        DDX_Control(pDX, IDC_CHECK5, m_check5);
        DDX_Control(pDX, IDC_CHECK6, m_check6);
        DDX_Control(pDX, IDC_CHECK7, m_check7);
        DDX_Control(pDX, IDC_CHECK8, m_check8);
        DDX_Control(pDX, IDC_CHECK9, m_check9);
        DDX_Control(pDX, IDC_CHECK10, m_check10);
        DDX_Control(pDX, IDC_CHECK11, m_check11);
        DDX_Control(pDX, IDC_CHECK12, m_check12);
        DDX_Control(pDX, IDC_CHECK13, m_check13);
        DDX_Control(pDX, IDC_CHECK14, m_check14);
        DDX_Control(pDX, IDC_CHECK15, m_check15);
        DDX_Control(pDX, IDC_CHECK16, m_check16);
        DDX_Control(pDX, IDC_CHECK17, m_check17);
        DDX_Control(pDX, IDC_CHECK18, m_check18);
        DDX_Control(pDX, IDC_CHECK19, m_check19);
        DDX_Control(pDX, IDC_CHECK20, m_check20);
        DDX_Control(pDX, IDC_CHECK_ULTLOG, m_checkUltLog);    
                                    //son210215 ULTLog for Samsung Electro-Mecha 
        DDX_Control(pDX, IDC_CHECK21, m_check_UseLotName_4wLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSet13, CDialog)
    //{{AFX_MSG_MAP(CModeSet13)
        ON_CBN_SELCHANGE(IDC_COMBO101, OnSelchangeCombo1)
        ON_CBN_SELCHANGE(IDC_COMBO102, OnSelchangeCombo2)
        ON_CBN_SELCHANGE(IDC_COMBO103, OnSelchangeCombo3)
        ON_CBN_SELCHANGE(IDC_COMBO104, OnSelchangeCombo4)
        ON_CBN_SELCHANGE(IDC_COMBO105, OnSelchangeCombo5)
        ON_CBN_SELCHANGE(IDC_COMBO111, OnSelchangeCombo111)
        ON_CBN_SELCHANGE(IDC_COMBO112, OnSelchangeComboSparkMode)
        ON_CBN_SELCHANGE(IDC_COMBO113, OnSelchangeCombo113)
        ON_CBN_SELCHANGE(IDC_COMBO114, OnSelchangeCombo114)
        ON_CBN_SELCHANGE(IDC_COMBO115, OnSelchangeCombo115)
        ON_BN_CLICKED(IDC_CHECK1, OnCheck1)   
        ON_BN_CLICKED(IDC_CHECK2, OnCheck2)  
        ON_BN_CLICKED(IDC_CHECK3, OnCheck3) 
        ON_BN_CLICKED(IDC_CHECK4, OnCheck4) 
        ON_BN_CLICKED(IDC_CHECK5, OnCheck5)   
        ON_BN_CLICKED(IDC_CHECK6, OnCheck6)  
        ON_BN_CLICKED(IDC_CHECK7, OnCheck7) 
        ON_BN_CLICKED(IDC_CHECK8, OnCheck8) 
        ON_BN_CLICKED(IDC_CHECK9, OnCheck9)     
        ON_BN_CLICKED(IDC_CHECK10, OnCheck10) 
        ON_BN_CLICKED(IDC_CHECK11, OnCheck11)   
        ON_BN_CLICKED(IDC_CHECK12, OnCheck12)  
        ON_BN_CLICKED(IDC_CHECK13, OnCheck13) 
        ON_BN_CLICKED(IDC_CHECK14, OnCheck14) 
        ON_BN_CLICKED(IDC_CHECK15, OnCheck15)   
        ON_BN_CLICKED(IDC_CHECK16, OnCheck16)  
        ON_BN_CLICKED(IDC_CHECK17, OnCheck17) 
        ON_BN_CLICKED(IDC_CHECK18, OnCheck18) 
        ON_BN_CLICKED(IDC_CHECK19, OnCheck19)   
        ON_BN_CLICKED(IDC_CHECK20, OnCheck20) 
        ON_BN_CLICKED(IDC_CHECK_ULTLOG, OnCheckUltLog)
        ON_BN_CLICKED(IDC_CHECK21, OnCheck_UseLotName_4wLog)
    	ON_BN_CLICKED(IDC_CHECK_NEARBY_OPEN_MEAS, OnCheckNearbyOpenMeas)
    	ON_EN_CHANGE(IDC_EDIT_NEARBY_OPENRATE, OnChangeEditNearbyOpenrate)
                                    //son230728
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSet13 message handlers

BEGIN_EVENTSINK_MAP(CModeSet13, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
    ON_EVENT(CModeSet13, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
    ON_EVENT(CModeSet13, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
//  ON_EVENT(CModeSet13, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)

    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CModeSet13::OnClickOk() 
{
 

//  FileSysInfo01.LoadSaveSub71(_LOAD);

    CDialog::OnOK();

}


BOOL CModeSet13::OnInitDialog() 
{


    CDialog::OnInitDialog();

 
    Onload();

    return 1;
}

//son230728 OnSave()에서 사용하기위해 OnInitDialog()에서 Onload() 분리. 
void CModeSet13::Onload()
{
    CString str;
    int a;

    a=FileSysInfo01.LoadSaveSet13(_LOAD);//2:load 

    if(a!=1){
        AfxMessageBox(" Error No 1212, \n    SysSet13.ini  Load ERROR !  \n    file Read Error !!!");    
    }


    if( SysSet13.m_nSet1==1 ){
        m_check1.SetCheck(1); 
    }
    if( SysSet13.m_nSet2==1 ){
        m_check2.SetCheck(1); 
    }
    if( SysSet13.m_nSetUltLog==1 ){  //son210215 ULTLog for Samsung Electro-Mecha
        m_checkUltLog.SetCheck(1); 
    }
	if( SysSet13.m_nSet_UseLotName_4wLog==1 ){  //son220124 for 제이텍
		m_check_UseLotName_4wLog.SetCheck(1); 
	}
	if( SysSet13.m_nSet_NearbyOpenMeas==1 ){  //son230712 for 영풍 SST
		m_check_NearbyOpenMeas.SetCheck(1); 
	}
	
    char szText[100];                                       //son230728
	sprintf(szText, "%d", SysSet13.m_nNearByOpenRate);  //son230728
    m_edit_NearByOpenRate.SetWindowText(szText);            //son230728

    if( SysSet13.m_nSet3==1 ){
        m_check3.SetCheck(1);
    }
    if( SysSet13.m_nSet4==1 ){
        m_check4.SetCheck(1); 
    }
    if( SysSet13.m_nSet5==1 ){
        m_check5.SetCheck(1);
    }
    if( SysSet13.m_nSet6==1 ){
        m_check6.SetCheck(1); 
    }
    if( SysSet13.m_nSet7==1 ){
        m_check7.SetCheck(1);
    }
    if( SysSet13.m_nSet8==1 ){
        m_check8.SetCheck(1);
    }
    //son 4W error condition Mode. 중앙값 기준  20% 벗어나면  내부 재측정을 여러번 수행
    if( SysSet13.m_nSet9==1 ){
        m_check9.SetCheck(1); 
    }
    if( SysSet13.m_nShortDetailOff==1 ){    //son220318_2
        m_check10.SetCheck(1);
    }
    
    if( SysSet13.m_nSet11==1 ){
        m_check11.SetCheck(1); 
    }
    if( SysSet13.m_nSet12==1 ){
        m_check12.SetCheck(1); 
    }
    if( SysSet13.m_nSet13==1 ){
        m_check13.SetCheck(1);
    }
    if( SysSet13.m_nSet14==1 ){
        m_check14.SetCheck(1); 
    }
    if( SysSet13.m_nSet15==1 ){
        m_check15.SetCheck(1);
    }
    if( SysSet13.m_nSet16==1 ){
        m_check16.SetCheck(1); 
    }
    if( SysSet13.m_nSet17==1 ){
        m_check17.SetCheck(1);
    }
    if( SysSet13.m_nSet18==1 ){
        m_check18.SetCheck(1);
    }
    if( SysSet13.m_nSet19==1 ){
        m_check19.SetCheck(1); 
    }
    if( SysSet13.m_nSet20==1 ){
        m_check20.SetCheck(1);
    }

//son230216_1 begin:
    int i;
    m_combo1.ResetContent();    //son230328 
    for (i=0; i < MAX_PIN_BLK; i++) 
    {
        str.Format("%d. %s", i+1, STR_PIN_BLOCK(i));
        m_combo1.InsertString(-1, str ); 
    }
    m_combo1.SetCurSel(SysSet13.m_nPinBlockType); //sylee161108-1
//son230216_1 end

    m_combo111.SetCurSel(SysSet13.m_n4WCVMode);   //sylee200217-4WCVMODE //sylee200515-patch
    m_combo_SparkMode.SetCurSel(SysSet13.m_nSparkMode);   //son200908 NEW_SPARK
    m_combo3.SetCurSel(SysSet13.m_nUseCalData);     //son220419 

	if(SysSet13.m_nSet105<0) SysSet13.m_nSet105=0;//sylee240509
	m_combo5.SetCurSel(SysSet13.m_nSet105);//sylee240509
//son231026 begin:
    m_combo113.ResetContent();    
    for (i=0; i < _4W_LOG_MODE_NUM; i++) 
    {
#if (!defined(__MST__CUSTOMER)) && (!defined(__NANOSYS__CUSTOMER))
        if (i == _4W_LOG_COL_V2_MST)   //son MST 유니텍일때에만 _4W_LOG_COL_MST_UNITECH 모드를 보여준다.
            continue;
#endif

#ifndef __NANOSYS__CUSTOMER
        if (i == _4W_LOG_LINE_DUT_V2_NANO)   //son 나노시스에만 _4W_LOG_COL_MST_UNITECH 모드를 보여준다.
            continue;                       // 순서 문제가 있어서 나노시스는 _4W_LOG_COL_V2_MST, _4W_LOG_LINE_DUT_V2_NANO 둘다 보여줘야 함
#endif
        str.Format("%s", STR_4W_LOG_MODE(i));
        m_combo113.InsertString(-1, str ); 
    }
#ifndef __MST__CUSTOMER
    //son MST 유니텍이 아니면  _4W_LOG_COL_V2 사용하지 않으므로 default 모드로 처리
    if (SysSet13.m_n4wLogMode == _4W_LOG_COL_V2_MST)    //son240109
        SysSet13.m_n4wLogMode = _4W_LOG_COL;        //son240109
#endif
#ifndef __NANOSYS__CUSTOMER
    //son 나노시스가 이 아니면  _4W_LOG_LINE_DUT_V2_NANO 사용하지 않으므로 default 모드로 처리
    if (SysSet13.m_n4wLogMode == _4W_LOG_LINE_DUT_V2_NANO)   //son240109
        SysSet13.m_n4wLogMode = _4W_LOG_COL;                 //son240109
#endif

    m_combo113.SetCurSel(SysSet13.m_n4wLogMode);	//son201209 4WCSV_NEW
//son231026 end


    //son200722-BDL-Sort  : 신규 추가
    CStrList_BdlSort    bdlSortModeStr;
    m_combo2.ResetContent();    //son230328 
    for (i=NO_SORT; i < SORT_MODE_END; i++)
    {
        m_combo2.InsertString(-1, bdlSortModeStr.strList[i]); 
    }
    //m_combo2.SetCurSel(NO_SORT);
    m_combo2.SetCurSel(SysSet13.m_nBdlSortMode);

//son230106_2 begin:
    CStrList_4wFailSort    _4wFailSortModeStr;
    m_combo115.ResetContent();    //son230328 
    for (i=F_4W_SORT_HIGH_OVERRATE; i < F_4W_SORT_MODE_END; i++)
    {
        m_combo115.InsertString(-1, _4wFailSortModeStr.strList[i]); 
    }
    //m_combo115.SetCurSel(NO_SORT);
    m_combo115.SetCurSel(SysSet13.m_n4wFailSortMode);
//son230106_2 end
}

void CModeSet13::OnSelchangeCombo1() 
{
    int PrevPinBlockType = SysSet13.m_nPinBlockType;
    SysSet13.m_nPinBlockType = m_combo1.GetCurSel();//sylee161108-1

    m_combo111.SetCurSel(SysSet13.m_n4WCVMode); //sylee200217-4WCVMODE

#if 0
    // 새로 입력된 값이 기존값과 다르다면 메시지출력, 같으면 pass.
    if (PrevPinBlockType == SysSet13.m_nPinBlockType)
        return;

    CString str, out;

    //---------------------
    //son  No 32K -> 32K
    //---------------------
    // PinBlockType이 32K인데 PinTotMax 값이 32K 아닌값 상태라면
    // PinTotMax도 32K 기준으로 바꾸라는 메시지를 출력해 준다. 
    if (SysSet13.m_nPinBlockType == PIN_BLK_32K_TRANS) //son230216_1
    {
        if(SysSet12.m_nPinTotMax != PIN_TOT_32K_TRANS //son230216_1
                && SysSet12.m_nPinTotMax != PIN_TOT_20K_UP_TR //son230216_1
                && SysSet12.m_nPinTotMax != PIN_TOT_24K_UP_TR //son230216_1
                && SysSet12.m_nPinTotMax != PIN_TOT_16K_TRANS //son230216_1
                && SysSet12.m_nPinTotMax != PIN_TOT_24K_TRANS //son230216_1
                )
        {
            str.Format("PinBlockType= 32K. \n\n  MaxPin=%s shoule be changed to 32K, 20K or 24K.\n\n",
                    STR_MAX_PIN(SysSet12.m_nPinTotMax) );
            out += str;
            str.Format("Go to Menu-> SETUP -> \"PIN\",\n and change the \"Max Pin\" value to 32K, 20K, or 24K");
            out += str;
            AfxMessageBox(out);
            return;
        }   
    }

    //---------------------
    //son  32K -> No 32K
    //---------------------
    //son PinBlockType이 32K가 아닌데  PinTotMax 값을 32K인 상태라면 
    //    PinTotMax도 32K가 아닌 값으로   바꾸라는 메시지를 출력해 준다. 
    if (PrevPinBlockType == PIN_BLK_32K_TRANS && SysSet13.m_nPinBlockType != PIN_BLK_32K_TRANS) //son230216_1
    {
        if(SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS //son230216_1
                || SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS) //son230216_1
        {
            str.Format("You changed PinBlockType to \"no 32K\" value \"%s\", \nso   MaxPin=%s should be changed, too.\n\n",
                    STR_PIN_BLOCK(SysSet13.m_nPinBlockType), STR_MAX_PIN(SysSet12.m_nPinTotMax) );
            out += str;
            str.Format("Go to Menu-> SETUP -> \"PIN\",\n and change the \"Max Pin\" value.\n");
            out += str;
            AfxMessageBox(out);
        }

    }
#endif

}


void CModeSet13::OnSelchangeCombo2() 
{
    //son200722-BDL-Sort 0:NO_SORT, 1:SORT_MODE_DESCEND, 2:SORT_MODE_ASCEND
    SysSet13.m_nBdlSortMode = m_combo2.GetCurSel();

}

void CModeSet13::OnSelchangeCombo3() 
{
    //son220419 0:USECAL_UPPER_ONLY,  1:USECAL_UPPER_LOWER
    SysSet13.m_nUseCalData = m_combo3.GetCurSel();  //son220419 m_nSet103 -> m_nUseCalData 로 변경

    if (SysSet13.m_nUseCalData == USECAL_UPPER_LOWER)   //son220426 begin
    {
        CString str ;
        str.Format("\"Lower Part Calibration\" is required for proper use of this \"UPPER_LOWER\" mode.\n");
        AfxMessageBox(str, MB_OK);
    }   //son220426 end

}

void CModeSet13::OnSelchangeCombo4() 
{
    SysSet13.m_nSet104 = m_combo4.GetCurSel();
}


void CModeSet13::OnSelchangeCombo5() 
{
    SysSet13.m_nSet105 = m_combo5.GetCurSel();
}

void CModeSet13::OnSelchangeCombo111() 
{
    SysSet13.m_n4WCVMode = m_combo111.GetCurSel();      //sylee200320-4WCVMODE
}

void CModeSet13::OnSelchangeComboSparkMode()        
{
    //son  0: SPARK_NEW, 1:SPARK_OLD1,  2:SPARK_OLD2
    SysSet13.m_nSparkMode = m_combo_SparkMode.GetCurSel();  //son200908 NEW_SPARK
}
void CModeSet13::OnSelchangeCombo113() 
{
    //son 0:_4W_LOG_COL, 1:_4W_LOG_LINE_DUT_PIECE, 2:_4W_LOG_LINE_DUT
    SysSet13.m_n4wLogMode = m_combo113.GetCurSel(); //son201209 4WCSV_NEW
}

void CModeSet13::OnSelchangeCombo114()  
{
    SysSet13.m_nSet114 = m_combo114.GetCurSel();   //son210215: ULTLog for Samsung 
}

void CModeSet13::OnSelchangeCombo115() 
{
    //son 4W Fail AutoTest 출력 Sort 모드  0:RefHigh OverRate  1: 4w NetNo
    SysSet13.m_n4wFailSortMode = m_combo115.GetCurSel();    //son230106_2
}
 

//  m_check1.SetCheck(1);  

void CModeSet13::OnCheck1()  
{
    if(m_check1.GetCheck())
    {
        SysSet13.m_nSet1=1; 
    }else{
        SysSet13.m_nSet1=0;
    }
}



void CModeSet13::OnCheck2()  
{
    if(m_check2.GetCheck())
    {
        SysSet13.m_nSet2=1; 

        //son Z:\\LogSheet4W.csv 옵션과 Z:\\Ultlog.csv 옵션은 배타적이어야 한다. 
        //    Z:\\LogSheet4W.csv가 On이면 SysSet13.m_nSetUltLog는 강제로 0으로 설정한다.
        SysSet13.m_nSetUltLog = 0; 
        m_checkUltLog.SetCheck(0); 
    }

    else
    {
        //son Z:\\LogSheet4W.csv 옵션과 Z:\\Ultlog.csv 옵션이 둘 다 Off 일 수 있으므로 
        //    ULTLog check Off인 경우는 추가적인 action을 취하지 않는다.
        SysSet13.m_nSet2=0;
    }
}
 
 

//son210215 ULTLog for Samsung Electro-Mecha 
void CModeSet13::OnCheckUltLog() 
{
    // TODO: Add your control notification handler code here
    if(m_checkUltLog.GetCheck())
    {
        SysSet13.m_nSetUltLog=1;    

        //son Z:\\LogSheet4W.csv 옵션과 Z:\\Ultlog.csv 옵션은 배타적이어야 한다. 
        //    Z:\\Ultlog.csv가 On이면 SysSet13.m_nSet2는 강제로 0으로 설정한다.
        SysSet13.m_nSet2 = 0; 
        m_check2.SetCheck(0); 
    }
    else
    {
        //son Z:\\LogSheet4W.csv 옵션과 Z:\\Ultlog.csv 옵션이 둘 다 Off 일 수 있으므로 
        //    ULTLog check Off인 경우는 추가적인 action을 취하지 않는다.
        SysSet13.m_nSetUltLog=0;
    }
    
}

void CModeSet13::OnCheck_UseLotName_4wLog()  	//son220124
{
 	if(m_check_UseLotName_4wLog.GetCheck())	
	{	
		SysSet13.m_nSet_UseLotName_4wLog=1;	 	//son220124
	}else{
		SysSet13.m_nSet_UseLotName_4wLog=0;		//son220124
	}
}

void CModeSet13::OnCheck3()  
{
    if(m_check3.GetCheck())
    {
        SysSet13.m_nSet3=1; 
    }else{
        SysSet13.m_nSet3=0;
    }
}
 
void CModeSet13::OnCheck4()  
{
    if(m_check4.GetCheck())
    {
        SysSet13.m_nSet4=1; 
    }else{
        SysSet13.m_nSet4=0;
    }
}


 
void CModeSet13::OnCheck5()  
{
    if(m_check5.GetCheck())
    {
        SysSet13.m_nSet5=1; 
    }else{
        SysSet13.m_nSet5=0;
    }
}



void CModeSet13::OnCheck6()  
{
    if(m_check6.GetCheck())
    {
        SysSet13.m_nSet6=1; 
    }else{
        SysSet13.m_nSet6=0;
    }
}

void CModeSet13::OnCheck7()  
{
    if(m_check7.GetCheck())
    {
        SysSet13.m_nSet7=1; 
    }else{
        SysSet13.m_nSet7=0;
    }
}

void CModeSet13::OnCheck8()  
{
    if(m_check8.GetCheck())
    {
        SysSet13.m_nSet8=1; 
    }else{
        SysSet13.m_nSet8=0;
    }
}
void CModeSet13::OnCheck9()  
{
    if(m_check9.GetCheck())
    {
        SysSet13.m_nSet9=1; 
    }else{
        SysSet13.m_nSet9=0;
    }
}

void CModeSet13::OnCheck10()  
{
    if(m_check10.GetCheck())
    {
        SysSet13.m_nShortDetailOff=1;   //son220318_2
    }else{
        SysSet13.m_nShortDetailOff=0;   //son220318_2
    }
}

void CModeSet13::OnCheck11()  
{
    if(m_check11.GetCheck())
    {
        SysSet13.m_nSet11=1;    
    }else{
        SysSet13.m_nSet11=0;
    }
}



void CModeSet13::OnCheck12()  
{
    if(m_check12.GetCheck())
    {
        SysSet13.m_nSet12=1;    
    }else{
        SysSet13.m_nSet12=0;
    }
}
 

void CModeSet13::OnCheck13()  
{
    if(m_check13.GetCheck())
    {
        SysSet13.m_nSet13=1;    
    }else{
        SysSet13.m_nSet13=0;
    }
}
 
void CModeSet13::OnCheck14()  
{
    if(m_check14.GetCheck())
    {
        SysSet13.m_nSet14=1;    
    }else{
        SysSet13.m_nSet14=0;
    }
}
 
void CModeSet13::OnCheck15()  
{
    if(m_check15.GetCheck())
    {
        SysSet13.m_nSet15=1;    
    }else{
        SysSet13.m_nSet15=0;
    }
}

void CModeSet13::OnCheck16()  
{
    if(m_check16.GetCheck())
    {
        SysSet13.m_nSet16=1;    
    }else{
        SysSet13.m_nSet16=0;
    }
}

void CModeSet13::OnCheck17()  
{
    if(m_check17.GetCheck())
    {
        SysSet13.m_nSet17=1;    
    }else{
        SysSet13.m_nSet17=0;
    }
}

void CModeSet13::OnCheck18()  
{
    if(m_check18.GetCheck())
    {
        SysSet13.m_nSet18=1;    
    }else{
        SysSet13.m_nSet18=0;
    }
}
void CModeSet13::OnCheck19()  
{
    if(m_check19.GetCheck())
    {
        SysSet13.m_nSet19=1;    
    }else{
        SysSet13.m_nSet19=0;
    }
}

void CModeSet13::OnCheck20()  
{
    if(m_check20.GetCheck())
    {
        SysSet13.m_nSet20=1;    
    }else{
        SysSet13.m_nSet20=0;
    }
}


void CModeSet13::OnSave() 
{ 

    OnCheck1();OnCheck2();OnCheck3();OnCheck4();OnCheck5();
    OnCheck6();OnCheck7();OnCheck8();OnCheck9();OnCheck10();
    
    OnCheck11();OnCheck12();OnCheck13();OnCheck14();OnCheck15();
    OnCheck16();OnCheck17();OnCheck18();OnCheck19();OnCheck20();


    FileSysInfo01.LoadSaveSet13(_SAVE); //1: save 


    Onload();   //son230728
}



void CModeSet13::OnCheckNearbyOpenMeas()    //son230712
{
	// TODO: Add your control notification handler code here
 	if(m_check_NearbyOpenMeas.GetCheck())	
	{	
		SysSet13.m_nSet_NearbyOpenMeas=1;	 	//son230712
	}else{
		SysSet13.m_nSet_NearbyOpenMeas=0;	    //son230712	
	}	
}



//son230728 MAP > Set 1 > 'Near By Open Rate '  
//    근접 Open 설정 기준 %  입력 edit 박스  추가
void CModeSet13::OnChangeEditNearbyOpenrate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	char szText[100];
	m_edit_NearByOpenRate.GetWindowText( szText,10); 	

	int rate = atoi(szText);
	if (rate < 3) 
	    rate = 3;
	if (rate > 30)
	    rate = 30;
	
	SysSet13.m_nNearByOpenRate = rate;

	//sprintf(szText, "%d", SysSet13.m_nNearByOpenRate);  <- 이렇게 하면 무한루프에 빠지는 듯. 하지 말자.
    //m_edit_NearByOpenRate.SetWindowText(szText);
}
