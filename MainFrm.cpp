// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView7.h"
#include "labelcontrol.h"
//#include "Gobal.h"
#include "FileSysInfo.h"
#include "ModeSet13.h"
#include "MainFrm.h"
#include "FileSysInfo.h"
#include "Globals.h"
#include "PassWord1.h"  //son230829 중국 유니텍 Setup 클릭시 비번입력 기능

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern 	int OnSelfTr1_Stop1;//sylee190110-1
extern 	int OnSelfTr1_Flag_Run1;
extern void DoEvents();

extern int nFlagFileLoad2;//sylee180406-1
extern int  A_Interlock1();//sylee171025-1 
extern CSysInfo05  SysInfo05;//sylee170824
extern int gFlag_Start_Int_bdlError;//sylee170717		//son 값이 1이면 BDL 파일 에러.
           

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1

extern int  gDef_Language1; //SYLEE151028   //1=KOREA ,2=ENGLISH, 3=CHINA
extern CFileSysInfo  FileSysInfo01; 
extern CSysSet211  SysSet211;  //Pin SETUP 
extern int Check_4WErrorNet();  //sylee171122   //son231019_2 OnErrorNet1() -> Check_4WErrorNet()

extern CSysInfoPiece  SysInfoPiece; //sylee180905


int nCurrentScreen;
CChildView7 pChildView7;
int g_nInitFlag1;   //son 현재 view ID: SwitchView(VIEW_ID7) 일때  7, SwitchView(VIEW_ID5)일때 8
                    
int g_nInitFlag2;   //son MainFrm으로 부터 들어왔다는 표시로 1회성 설정 
                    //son VIEW_ID5(SelfTest), VIEW_ID7(AutoTest)로 SwitchView할때  1로 표시
                    //son CChildView7::OnPaint()에서  2로 변경했다가,  m_ComiDaq2 장비 Init 후에 0으로 변경 
                    //son ChildView5내에서 2로 변경

 
extern CSysSet13  SysSet13;  
extern int g_nSelfRunFlag;		//son defined in ChildView8.cpp
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_Menu_Test1, OnMenuTest1)
	ON_COMMAND(ID_Menu_Test2, OnMenuTest2)
	ON_COMMAND(ID_Menu_Test3, OnMenuTest3)
	ON_COMMAND(ID_Menu_Test4, OnMenuTest4)
 	ON_COMMAND(ID_Menu_Test5, OnMenuTest5)
	ON_COMMAND(ID_Menu_Test6, OnMenuTest6)
	ON_COMMAND(ID_Menu_Test7, OnMenuTest7)
	ON_COMMAND(ID_Menu_Test8, OnMenuTest8)
	ON_COMMAND(ID_Menu_Test9, OnMenuTest9)

	ON_COMMAND(ID_Menu_P1, OnMenuP1)
	ON_COMMAND(ID_Menu_P2, OnMenuP2)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()
 
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pToolBarBmp = new CBitmap;
	m_strTime = _T("");
}

CMainFrame::~CMainFrame()
{
	delete m_pToolBarBmp;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndReBar.Create(this))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST
		| TBSTYLE_TRANSPARENT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

   // m_wndStatusBar.SetPaneInfo(1,ID_INDECATOR_CAPS,SBPS_POPOUT,100);
	 m_wndStatusBar.SetPaneInfo(1,2,SBPS_POPOUT,300);
	 m_wndStatusBar.SetPaneInfo(2,3,SBPS_NORMAL,200);
	 m_wndStatusBar.SetPaneInfo(3,4,SBPS_STRETCH,200); 
//	 m_wndStatusBar.SetPaneText(1," KURITA Corporation.");
	 m_wndStatusBar.SetPaneText(1," BiOptro  Co. Ltd.");
	 m_wndStatusBar.SetPaneText(2,"DATE");
	 m_wndStatusBar.SetPaneText(3,"TIME  :  : ");

	 CTime curTime = CTime::GetCurrentTime();
	 m_strTime.Format("SW START %dM/%dD-%dH:%dM:%dS", curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
     m_wndStatusBar.SetPaneText(3,m_strTime);  //sw start time

	CRect FrameRect;
	GetClientRect(FrameRect);
	m_wndToolBar.SetButtons(NULL, 9);

	UINT uButtonStyle	= 0;
	UINT uButtonStyleEx	= 0;
	m_wndToolBar.SetButtonText(0, "FILE");
	m_wndToolBar.SetButtonInfo(0, ID_Menu_Test1, uButtonStyle |	0, 3);
//	m_wndToolBar.SetButtonTextStyle(0,1); 
	m_wndToolBar.SetButtonText(1, "SETUP");
	m_wndToolBar.SetButtonInfo(1, ID_Menu_Test2, uButtonStyle, 1);
	m_wndToolBar.SetButtonText(2, "MANUAL");
	m_wndToolBar.SetButtonInfo(2, ID_Menu_Test3, uButtonStyle, 2);
//sylee160626-1	m_wndToolBar.SetButtonText(3, "HISTORY");
//sylee160626-1	m_wndToolBar.SetButtonInfo(3, ID_Menu_Test4, uButtonStyle, 3);
	m_wndToolBar.SetButtonText(3, "SERVICE");
	m_wndToolBar.SetButtonInfo(3, ID_Menu_Test5, uButtonStyle, 4);
	m_wndToolBar.SetButtonText(4, "MAP"); 
	m_wndToolBar.SetButtonInfo(4, ID_Menu_Test6, uButtonStyle, 5);
	m_wndToolBar.SetButtonText(5, "*AUTO*");
	m_wndToolBar.SetButtonInfo(5, ID_Menu_Test7, uButtonStyle, 6);
	m_wndToolBar.SetButtonText(6, "Self TEST");
	m_wndToolBar.SetButtonInfo(6, ID_Menu_Test8, uButtonStyle, 7);
//	m_wndToolBar.SetButtonText(8, "BiOptro");
	m_wndToolBar.SetButtonText(7, "NPX");
	m_wndToolBar.SetButtonInfo(7, ID_Menu_Test9, uButtonStyle, 8);
	
	FileSysInfo01.LoadSaveView01(_LOAD);	//son210514
	FileSysInfo01.LoadSaveSet211(_LOAD);//load //sylee160928

   if(SysSet211.m_nSet13==1){//sylee160907-1   //chinese
     	gDef_Language1=3;//redboard
	}else{//sylee160907-1
        gDef_Language1=2; //englishos
	}//sylee160907-1

	if( gDef_Language1==3){//sylee151028-1  //LANGUAGE CHANGE
		m_wndToolBar.SetButtonText(0, LoadMessage(1,"Title", 101)); 
        m_wndToolBar.SetButtonText(1, LoadMessage(1,"Title", 102)); 
		m_wndToolBar.SetButtonText(2, LoadMessage(1,"Title", 103));	 
   //sylee160626-1	     m_wndToolBar.SetButtonText(3, LoadMessage(1,"Title", 104)); 
		m_wndToolBar.SetButtonText(3, LoadMessage(1,"Title", 105));	 
        m_wndToolBar.SetButtonText(4, LoadMessage(1,"Title", 106)); 
		m_wndToolBar.SetButtonText(5, LoadMessage(1,"Title", 107));	 
        m_wndToolBar.SetButtonText(6, LoadMessage(1,"Title", 108));	
		m_wndToolBar.SetButtonText(7, LoadMessage(1,"Title", 109));	       
	}else{
		m_wndToolBar.SetButtonText(0, "FILE");
		m_wndToolBar.SetButtonText(1, "SETUP");
		m_wndToolBar.SetButtonText(2, "MANUAL");
//sylee160626-1			m_wndToolBar.SetButtonText(3, "HISTORY"); 
		m_wndToolBar.SetButtonText(3, "SERVICE");	
		m_wndToolBar.SetButtonText(4, "MAP"); 
		m_wndToolBar.SetButtonText(5, "*AUTO*");
		m_wndToolBar.SetButtonText(6, "Self TEST");
		m_wndToolBar.SetButtonText(7, "NPX");
	}

	CToolBarCtrl& ToolBarCtrl = m_wndToolBar.GetToolBarCtrl();
	ToolBarCtrl.SetExtendedStyle(uButtonStyleEx);
	m_wndToolBar.SetSizes(CSize(100, 48), CSize(32, 32));//SYLEE 38->48
	m_wndToolBar.SetHeight(48);//SYLEE 38->48

	CImageList	ImageListHot, ImageListNormal;
	CBitmap		Bitmap;

	Bitmap.LoadBitmap(IDB_Bitmap_Toolbar_Normal);
	ImageListNormal.Create(32, 32, ILC_COLORDDB | ILC_MASK, 10, 0);
	ImageListNormal.Add(&Bitmap, RGB(255, 0, 255));
//	ToolBarCtrl.SetImageList(&ImageListNormal);
	ToolBarCtrl.SetHotImageList(&ImageListHot);
	ImageListNormal.Detach();
	Bitmap.Detach();

	Bitmap.LoadBitmap(IDB_Bitmap_Toolbar_Hot);
	ImageListHot.Create(32, 32, ILC_COLORDDB | ILC_MASK, 10, 0);
	ImageListHot.Add(&Bitmap, RGB(255, 0, 255));
	ToolBarCtrl.SetHotImageList(&ImageListHot);
	ImageListHot.Detach();
	Bitmap.Detach();

	m_wndAnimate.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 10, 10),
										this, AFX_IDW_TOOLBAR + 1);
    //DEMO	m_wndAnimate.Open(IDR_AVI);
	m_pToolBarBmp->LoadBitmap(IDB_Bitmap_Toolbar_Back);
    // 	m_wndReBar.AddBar(&m_wndToolBar, "TOOLBAR : ", m_pToolBarBmp,
    //									RBBS_FIXEDBMP | RBBS_BREAK);
	m_wndReBar.AddBar(&m_wndToolBar, "  ", m_pToolBarBmp,
 								RBBS_FIXEDBMP | RBBS_BREAK);
	m_wndReBar.AddBar(&m_wndAnimate, NULL, NULL,
								RBBS_FIXEDBMP | RBBS_FIXEDSIZE);
	m_wndAnimate.Play(0, -1, -1);
	GetClientRect(FrameRect);
	//컨트롤 바를 생성한다.
	if(!m_CtrlBarLeft.Create(NULL, NULL, WS_VISIBLE | WS_CHILD | CBRS_LEFT,
						CRect(0, 0, 200, FrameRect.bottom), this, AFX_IDW_TOOLBAR + 2))
	{
		TRACE0("Failed to create rebar\n");
		return -1;
	}
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	g_sFile.Init();


	CString str, str2, str3, strTemp; 
	str2.Format("");


#ifdef _DEBUG
    //son Debug 모드 표시
    strTemp.Format("DEBUG");
	str2 = str2+strTemp;
#endif

#ifdef NDEBUG
    //son Release 모드 표시
    strTemp.Format("RELEASE");
	str2 = str2+strTemp;
#endif

#ifdef _PC_TEST_
    //son 디버그용 PC 테스트모드.  IO Card를 무시하거나 Simulation으로 대신한다.
    strTemp.Format("/ PC_TEST");
	str2 = str2+strTemp;
#endif

#ifdef DEF_CARD_A1
    //son CONTEC IO Card 모드 표시
    strTemp.Format("/ CONTEC");
	str2 = str2+strTemp;
#endif


#ifdef __DUT16__
    //son DUT16 모드 표시
    strTemp.Format("/ DUT16");   //son210520 
	str2 = str2+strTemp;
#endif

#ifdef __DUT32__
    //son DUT32 모드 표시
    strTemp.Format("/ DUT32");
	str2 = str2+strTemp;
#endif


#ifdef __NEW_SPARK__
    //strTemp.Format("/ SPARK");   //son210318: SPARK가 이제 default이므로  타이틀바에서 표시 삭제.  
	//str2 = str2+strTemp;         //           _N 문구를 SPARK+4W10000 통합버전의미로 사용.
#endif

#ifdef __MULTI_CURRENT__
    strTemp.Format("/ MULTI_CURR");   //son220609
	str2 = str2+strTemp;         
#endif

//son230808 begin:
#ifdef __NEW_4W_230420      
    strTemp.Format("/ NEW_4W"); //son230808 E&E 4W 신규 기능 포함 여부  //son231205
    str2 = str2 + strTemp;  
#endif
//son230808 end

//son231205 begin   
#ifdef __LEAK_500M_      
    strTemp.Format("/ LEAK_500 Mohm"); 
    str2 = str2 + strTemp;  
#elif defined(__LEAK_300M_)
    strTemp.Format("/ LEAK_300 Mohm"); 
    str2 = str2 + strTemp;  
#elif defined(__LEAK_200M_)
    strTemp.Format("/ LEAK_200 Mohm"); 
    str2 = str2 + strTemp;  
#endif
//son231205 end

    getStr_BBTController_Type(str3);       //son220211


    //son 컴파일 정보, 날짜, Time 출력 
    str.Format("%s: NPX BBT Controller(%s) %s %d-%02d-%02d, %s", 
	//         ACE500(CUSTOMER)  RELEASE/ DUT16   /2020-06-23, 16:12:12
               str3,             str2,        VERSION,   YEAR, MONTH, DAY, (CString)__TIME__ );//SWVERSION 


	SetWindowText(str);//SYLEE171113
	UpdateData(TRUE);



 
	return 0;
}

void CMainFrame::getStr_BBTController_Type(CString &str3)       //son220211
{
    CString strTemp;

    str3.Format("ACE400");
#ifdef __ACE550__
    str3.Format("ACE550");      //son201118
#elif defined(__ACE500__)
    str3.Format("ACE500");      //son201118
#elif defined(__ACE1000__)
    str3.Format("ACE1000");     //son210514
#endif

#ifdef __OPVECTOR__
    //son OpenTest Vector 기능 추가 모드
    strTemp.Format("_N");       //son210318: _N 문구를 SPARK+4W10000 통합버전의미로 사용.
	str3 = str3+strTemp;
#endif

    strTemp.Format("");     //son220322_3 어떤 고객사도 선택하지 않으면 Common(CM) Customer로 컴파일한다.

#ifdef __GTS__CUSTOMER
    strTemp.Format("(GS)");    //son201118  //son210302 GTS->GS
#endif

#ifdef __CCTC__CUSTOMER
    strTemp.Format("(CCTC)");    //son220923
#endif

#ifdef __CHINA_UNITECH__CUSTOMER
    strTemp.Format("(CHUNI)");    //son230512  E&E Customer 추가
#endif

#ifdef __MST__CUSTOMER
    strTemp.Format("(MT)");    //son220228 m_button_NewBdlSet 기능 요구 관련. 대만 MST 신규 추가
#endif

#ifdef __JTECH__CUSTOMER
    strTemp.Format("(JT)");  //son201118    //son210302 JTECH->JT
#endif

#ifdef __LM__CUSTOMER
    strTemp.Format("(LM)");    //son220314 LM 디지털 추가
#endif

#ifdef __SIMTECH__CUSTOMER
    strTemp.Format("(SIM)");   //son220425 말레이시아 Simmtech 추가 
#endif

#ifdef __SST__CUSTOMER
    strTemp.Format("(ST)");    //son201118  //son210302 SST->ST
#endif

#ifdef __NANOSYS__CUSTOMER
    strTemp.Format("(NS)");    //son210115  //son210302 NANOSYS->NS
#endif

#ifdef __BH__CUSTOMER //yskim 241218 BHVT define 제거(저항표시 ohm으로 통일하기 위해)
    strTemp.Format("(BH)");    //son220922  Recipe HR 20 Mohm 사용 고객 관련 추가
#endif

#ifdef __KCC__CUSTOMER
    strTemp.Format("(KCC)"); //son231205
#endif

#ifdef __KCC_FCBGA__CUSTOMER
    strTemp.Format("(KCC_FCBGA)"); //son241114
#endif

#ifdef __BNTECH__CUSTOMER
    strTemp.Format("(BNTECH)"); //son241202
#endif

#ifdef __YOUNPUNG__CUSTOMER
    strTemp.Format("(YP)");    //son210115  //son210302 YOUNGPUNG_4W_2W_CHECK->YP
#endif

#ifdef __YP_VIET_CUSTOMER  //son210824
    strTemp.Format("(YP_VT)");    
#endif

#ifdef __SST__CUSTOMER__ACE400
    strTemp.Format("(ST400)"); //son230512 ACE400 SST 
#endif

//#ifdef __LEENOJUNGMIL__CUSTOMER       //son220323 이상윤부장님 요청으로 리노정밀 define 옵션은 삭제
//    strTemp.Format("(LN)");    //sylee210413      
//#endif
 
//#ifdef __WIll_TECHNOLOGY__CUSTOMER  //SYLEE210322_3     //son220323 WillTech 고객사 사용하는 코드 없으므로 삭제
//    strTemp.Format("(WILL)");    
//#endif

    str3 = str3 + strTemp;

}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs 
	cs.style = WS_OVERLAPPEDWINDOW;//sylee130405
	cs.style &= ~WS_SYSMENU;//sylee130405
	cs.hMenu = NULL;	//sylee130405
	cs.cx=::GetSystemMetrics(SM_CXSCREEN);
	cs.cy=::GetSystemMetrics(SM_CXSCREEN);
	cs.x=1;//cs.cx/2;
	cs.y=1;//cs.cy/2;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
 

void CMainFrame::OnTimer(UINT nIDEvent) 
{
 
	switch (nIDEvent)
	{
	case ID_CLOCK_TIMER:
	// Display the current time
	//	CTime curTime = CTime::GetCurrentTime();
	//	m_strTime.Format("%d:%d:%d", curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
		break;

	case 1001:
	    	OnMenuTest7() ; 		
			KillTimer(1001);
		break; 
	}
 
	UpdateData(FALSE); 
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnMenuTest1() 
{
 	SwitchView(VIEW_ID1);
}

void CMainFrame::OnMenuTest2() 
{	 
	  SwitchView(VIEW_ID2);
}

void CMainFrame::OnMenuTest3() 
{
	  SwitchView(VIEW_ID3);
}
 
void CMainFrame::OnMenuTest4() 
{
 	return;
	SwitchView(VIEW_ID4);
	nView1=4;
}
 
void CMainFrame::OnMenuTest5() 
{
	SwitchView(VIEW_ID8);	
}

void CMainFrame::OnMenuTest6() 
{

	if(nCurrentScreen==VIEW_ID6){
	  return;	
	}//sylee141112
	SwitchView(VIEW_ID6);
}




void CMainFrame::OnMenuTest7() 
{

    int nRet;

    //son 'BDL File 로딩'이 안 되었다면 ChildView7 (Auto Test 화면)으로 못 넘어간다.
    if( nFlagFileLoad2!=1){//sylee180406-1
        AfxMessageBox("  Error 7001     => [FILE]   => [Open Set]   =>   bdl file open!.  Please! ", MB_OK);
        return ;
    }

    nRet=OnAutoScreenInterlock();//sylee170824-1 	
    if(gFlag_Start_Int_bdlError!=1 && nRet==1){//sylee170717 //sylee170824-1 
        SwitchView(VIEW_ID7);	
    }

    if(gFlag_Start_Int_bdlError==1){//sylee170717
        AfxMessageBox("  Error 7021 => [File]=>  BDL FILE  => Total pin ?   pin list  ?  ", MB_OK);
    }
}



extern int nRecipe_4W_Use; //sylee210317  //son231020 Recipe step 중에 4W 가 있는지 표시
int CMainFrame::OnAutoScreenInterlock()//sylee170824-1 
{
	int nSum,i, ntmp1 = 0;

	nSum=0;

	for (i=1; i<= TEST_4W; i++){  //sylee150622	    //son 7: TEST_4W
		nSum+=SysInfo05.m_nStep[i];   //open, ushort, short, hr, hv 
	}
	if (nSum<=0){
		AfxMessageBox("  Error 7031 => [Recipe]=>  Step1~7  All skip  !   ", MB_OK);
		return 0;
	}
	////===========================================
	nRecipe_4W_Use=0;    //son231020 nUse4W -> nRecipe_4W_Use
	int mode=0; //sylee171122  //son231020 ntmp1-> mode
		
    //son "\SETUP2\Recipe.ini"를 SysInfo05에 Load(type=2)
	FileSysInfo01.LoadSaveSub15(2);//sylee180713-1

	for (i=1; i<= TEST_4W; i++){   //sylee171122	    //son 7: TEST_4W 
		 mode=SysInfo05.m_nStep[i];   //open, ushort, short, hr, hv  //son231020 ntmp1-> mode
		 if(mode== TEST_4W){  //son231020 7:TEST_4W
			 nRecipe_4W_Use=1;   //son231020 nUse4W -> nRecipe_4W_Use
		 }
	} 
	if (nRecipe_4W_Use==1) //sylee171122  //son231020 nUse4W -> nRecipe_4W_Use
    {
	    //son 4W.ini를 로드하고  Error 여부를 체크
		nSum= Check_4WErrorNet();   //sylee171122 //son231019_2 OnErrorNet1() -> Check_4WErrorNet()
//#ifndef _PC_TEST_     //son231020
		if(nSum>0)
		{
			AfxMessageBox(" \n Error 7032 => [Setup]=> [4wire]=>   Check!   \n\n (4w Mode 171~175 ?)   ( RefLow?)    ( RefHigh?)  \n  ", MB_OK);
			return 0;
		}
//#endif        //son231020
	}
	////===========================================

    CString str;

    FileSysInfo01.LoadSavePieceInfo(_LOAD); //son210430 LoadSaveSub25 ->LoadSavePieceInfo

    if(DEF_MAX_PIECE1>100){                 //son220823_6
        ntmp1=int(DEF_MAX_PIECE1/100)*100;  //son220823_6
    }else if(DEF_MAX_PIECE1>10){            //son220823_6
        ntmp1=int(DEF_MAX_PIECE1/10)*10;    //son220823_6
    }else  {
        ntmp1=int(DEF_MAX_PIECE1);          //son220823_6
    }
    if(ntmp1<0){
        ntmp1=1;
    }

    if( SysInfoPiece.m_nPiece >ntmp1){  //sylee180905-1
        str.Format(" Error No 7033 :  [SETUP]=>[Piece Set]=Piece No.?=  Piece Set Over Error!    ( Piece Set=%d ?? )   >   ( %d=Piece Max.)   ", SysInfoPiece.m_nPiece  ,ntmp1 );
        AfxMessageBox(str);	//sylee050308
        return 0;
    }

    return 1;
}


void CMainFrame::OnMenuTest8() 
{
	if(nCurrentScreen==VIEW_ID5){
	  return;	
	}//sylee141112
	 SwitchView(VIEW_ID5);	
}
 
void CMainFrame::OnMenuTest9() 
{	 
	 SwitchView(VIEW_ID9);	
}
 
void CMainFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;
 
	m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
 
	if(pNMToolBar->iItem == ID_Menu_Test1)
	{
		CMenu menu;
		CMenu* pPopup;
		menu.LoadMenu(IDR_Menu_Popup);
		pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
							rect.left, rect.top + 1, AfxGetMainWnd());
	}
	*pResult = TBDDRET_DEFAULT;
}

void CMainFrame::OnMenuP1() 
{
	m_CtrlBarLeft.ShowWindow(SW_SHOW);
	RecalcLayout();
}

void CMainFrame::OnMenuP2() 
{
	m_CtrlBarLeft.ShowWindow(SW_HIDE);
	RecalcLayout();
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
    //son201216 ChildView7 소유의 ComiDaq 디바이스를 ChildView1과 공유하기 위해 
    //          ChildView7의 생성을 ChildView1보다 먼저 수행하도록 변경함. 
	g_pChildView7 = new CChildView7;
	((CView*)g_pChildView7)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID7, pContext);
	g_pChildView7->ShowWindow(SW_HIDE);
	g_pChildView7->SetDlgCtrlID(VIEW_ID7);

	g_pChildView1 = new CChildView1;
	((CView*)g_pChildView1)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID1, pContext);
	g_pChildView1->ShowWindow(SW_HIDE);
	g_pChildView1->SetDlgCtrlID(VIEW_ID1);

	g_pChildView2 = new CChildView2;
	((CView*)g_pChildView2)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID2, pContext);
	g_pChildView2->ShowWindow(SW_HIDE);
	g_pChildView2->SetDlgCtrlID(VIEW_ID2);

	g_pChildView3 = new CChildView3;
	((CView*)g_pChildView3)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID3, pContext);
	g_pChildView3->ShowWindow(SW_HIDE);
	g_pChildView3->SetDlgCtrlID(VIEW_ID3);

 	g_pChildView4 = new CChildView4;
	((CView*)g_pChildView4)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID4, pContext);
	g_pChildView4->ShowWindow(SW_HIDE);
	g_pChildView4->SetDlgCtrlID(VIEW_ID4);

	g_pChildView5 = new CChildView5;
	((CView*)g_pChildView5)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID5, pContext);
	g_pChildView5->ShowWindow(SW_HIDE);
	g_pChildView5->SetDlgCtrlID(VIEW_ID5);

	g_pChildView6 = new CChildView6;
	((CView*)g_pChildView6)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID6, pContext);
	g_pChildView6->ShowWindow(SW_HIDE);
	g_pChildView6->SetDlgCtrlID(VIEW_ID6);

 	g_pChildView8 = new CChildView8;
	((CView*)g_pChildView8)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID8, pContext);
	g_pChildView8->ShowWindow(SW_HIDE);
	g_pChildView8->SetDlgCtrlID(VIEW_ID8);

	g_pChildView9 = new CChildView9;
	((CView*)g_pChildView9)->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,	this, VIEW_ID9, pContext);
	g_pChildView9->ShowWindow(SW_HIDE);
	g_pChildView9->SetDlgCtrlID(VIEW_ID9);
 
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

extern int nFlag_PassOn;    	//son230829 중국 유니텍 Setup 클릭시 비번입력 기능    
extern int nFlag_PassOnNo;      //son230829
void CMainFrame::SwitchView(UINT ViewID)
{
	if(g_nSelfRunFlag==1){
	  //  AfxMessageBox("  Selftest running.  Stop Please!"); 
	    return;
	}

	if(OnSelfTr1_Flag_Run1==1)
	{
        OnSelfTr1_Stop1=1;//sylee190110-1
	 	while(1)
	 	{	
			DoEvents(); 
			Sleep(10);
     		if(OnSelfTr1_Flag_Run1==1){ 			
				break;
			}
		}
		 
	}

	CView* pOldView = GetActiveView();
	CView* pNewView = NULL;
 
	if( SysSet13.m_nSet6!=1 ){//sylee131105
 		if( ViewID==VIEW_ID6){
     	  	if(  g_nLogOnUser1!=5 &&  g_nLogOnUser1!=7){
				 AfxMessageBox(" \n \n\n   Log in  Please !!!  \n\n  Supervisor Mode [maker]!  \n\n    [MAP] button Click.  "); 
				return;
			}
			else{
				if(g_nLogOnUser1==5){
				    //g_nLogOnUser1=15;
				}
			} 
		}
	} 

	if( SysSet13.m_nSet6!=1 ){//sylee131105
 		if( ViewID==VIEW_ID8){			
     		if(  g_nLogOnUser1!=5 &&  g_nLogOnUser1!=7){
			//sylee150708	 AfxMessageBox(" \n \n\n   Log in  Please !!!  \n\n  Supervisor Mode [maker]!    \n\n     [SERVICE] button Click.  "); 
			//sylee150708	return;
			}
			else{
				if(g_nLogOnUser1==5){
				   g_nLogOnUser1=15;
				}
			}			 
		}
	} 
	
	switch (ViewID)
	{
//DEMO	case VIEW_DEFAULT:		//기본 뷰
//DEMO		pNewView = (CView*)g_pDefaultView;
 		break;

	case VIEW_ID1:			//ChildView1	
		pNewView = (CView*)g_pChildView1;
		g_nInitFlag1=1;       //son201028 현재 view ID를  ChildView1으로 설정
		g_nInitFlag2=1;       //son201028 MainFrm으로 부터 들어왔다는 표시로 1회성 설정 
		break;

	case VIEW_ID2:			//ChildView2
//son230829 begin: 중국 유니텍 Setup 클릭시 비번입력 기능
#ifdef __CHINA_UNITECH__CUSTOMER
        if (SysSet13.m_nSet6!=1)   //son Map > Set1 > 'No Password' Off
        {
            nFlag_PassOnNo=12;  //son C:\ACE400\Data\Message_L.ini의 102 번 항목의 passwd를 체크한다. 
            CPassWord1 Dlg1;
            Dlg1.DoModal();
            if (nFlag_PassOn != 2) //son 틀린 패스워드면  Setup 창을 띄우지 않는다.
            {
                nFlag_PassOn=0;
                AfxMessageBox("         PASSWORD  NG !!!         \n     ");
                break;
            }
        }
#endif
//son230829 end
	    pNewView = (CView*)g_pChildView2;
		break;

	case VIEW_ID3:			//ChildView3
     	pNewView = (CView*)g_pChildView3;
		break; 
	case VIEW_ID4:			//ChildView4
 		pNewView = (CView*)g_pChildView4;
 		break;

	case VIEW_ID5:			//ChildView5  Self Test
		pNewView = (CView*)g_pChildView5;
		g_nInitFlag1=8;
		g_nInitFlag2=1;
		break;
	case VIEW_ID6:			//ChildView6
		pNewView = (CView*)g_pChildView6;
		break;	
	/*	*/
	case VIEW_ID7:			//ChildView7
		pNewView = (CView*)g_pChildView7; 
		g_nInitFlag1=7;
		g_nInitFlag2=1; 
		break;
	case VIEW_ID8:			//ChildView8 Service(Self Test)
		pNewView = (CView*)g_pChildView8;
		//g_nInitFlag1=8,g_nInitFlag2=1;
		break;
	case VIEW_ID9:			//ChildView9
		pNewView = (CView*)g_pChildView9;
		break;
	}

	if(pNewView)
	{
		if (pOldView == pNewView)
		{ 
			return;
		} 
		pOldView->ShowWindow(SW_HIDE);
		pOldView->SetDlgCtrlID(ViewID); 
		SetActiveView(pNewView);
		pNewView->ShowWindow(SW_SHOW); 
		pNewView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);			
		RecalcLayout();
		nCurrentScreen=ViewID;//sylee141112
	}

	if(  g_nLogOnUser1>10 ){//sylee141112
         g_nLogOnUser1=0; 
	}	
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{ 
	nCmdShow=SW_SHOWMAXIMIZED;	
	CFrameWnd::ActivateFrame(nCmdShow);

	CString str;
    getStr_BBTController_Type(str);       //son220211
	MyTrace(PRT_BASIC, "\n\n\n\n");
	MyTrace(PRT_BASIC, "%s_%02d%02d%02d:\"Mainframe\" Object started...\n", str, YEAR, MONTH, DAY  ); //son220211
}

//son200420-4WNET10000
BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	g_pChildView1->DestroyWindow();     //for vector 반환       //son200420-4WNET10000

	CString str;
    getStr_BBTController_Type(str);       //son220211
	MyTrace(PRT_BASIC, "%s_%02d%02d%02d:\"Mainframe\" Object destroyed...\n\n\n\n\n", str, YEAR, MONTH, DAY  ); //son220211

	
	return CFrameWnd::DestroyWindow();
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	PIN_INFO *pInfo = (PIN_INFO*)pCopyDataStruct->lpData;
	
	CString strPin;
	strPin.Format("pin = %d, volt = %.3f", pInfo->nTriggerCount, pInfo->dVoltage[0]);
	//AfxMessageBox(strPin);
	memcpy(&m_tInfoRcv[pInfo->nTriggerCount-1], pInfo, sizeof(PIN_INFO));

	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}
