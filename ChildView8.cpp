// ChildView8.cpp : implementation file
//

#include "stdafx.h"
#include "stdlib.h"

#include "BiOpTroCon.h"
#include "ChildView8.h"


#include <winuser.h>

#include "ModeSub71.h"
#include "FileSysInfo.h"

#include "ModeSet11.h"
#include "ModeSet12.h"  //son210831
#include "ModeSub31.h"
#include "ModeSet15.h"
#include "A_Define1.h"//sylee161126

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_FAIL_COUNT		4096 // hdpark221005 switch box 32EA 기준으로 수정 

extern int  gDef_Language1; //SYLEE151028   //1=KOREA ,2=ENGLISH, 3=CHINA

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1

extern short nPinSetSkipVb; //sylee151005 //ACE400	//son230127
extern short nPinSetSkipVc; //sylee151005 //ACE400	//son230127

extern int nCC,  nVRel, nIR;
extern double   nVSet;
extern CSysSet12  SysSet12;  //Pin SETUP 
extern CSysSet13  SysSet13;
extern CSysSet15  SysSet15;   

extern int g_nSelfAutoFlag1;//sylee120903       //son defined in ChildView1.cpp

extern __int64 GetMilSecond2(); 
 
extern void DoEvents();
 

extern CModeSub71  Sub71;
extern CModeSet12  ModeSet12;           //son210831
extern CSysInfoView01  SysInfoView01;
extern nAutoProcess1;
extern int nStop;
extern int nTimeDelay1;
//son231012_2 extern int nIADC1; // hdpark221005
extern double dIADC1; //son231012_2 int nIADC1  -> double dIADC1으로 수정. 소수점이하 잘리는 문제보완.

int nHW_CardPin,nCardPinMax;//sylee150903-1  //ACE400 //ACE380

int g_nSelfTestInterlock;//sylee131121		//son Auto 화면에서 SelfTest를 미리 진행을 했는지 확인하기 위한 용도로 사용한다.	
											//son 이 값이 0이면 SelfTest에 문제가 있음. 1이면 SelfTest가 완료됨
											//son ChildView7-> Timer 20(엔터키 입력) 시에 이 값이 1이어야 AProcess100()으로 진입한다.
int g_nMode1, g_nErrCount1, g_nErrCountTot, g_nSelfRunFlag;

int g_nListCount;		// OnList1Mes1()로 write한 로그 갯수
int g_nStopFlag, g_nAutoFlag, nAllPinSearchFlag;//sylee120927

double nVSet1,nVSet2,nISet;
 
 
float dSelfErr1[MAX_FAIL_COUNT][10];//sylee151210-memorydel //hdpark221005 3001 -> MAX_FAIL_COUNT(4096)
        //son                                   ChildView5(2W)                  4W:OnATestRun4W()                  ChardView8(ServiceSelf)
		//son 	dSelfErr1[g_nErrCount1][0]	:	cardNo    (1~ )                 cardNo(1~)                         cardNo(1~)
		//son 	dSelfErr1[g_nErrCount1][1]	:	cardPinNo (1~ )	                hw pinNo(1~)                       cardPinNo VB (1~ )
		//son 	dSelfErr1[g_nErrCount1][2]	:	hw pinNo  (1~ )	                testCase (1: 4W S1 Self Short,     cardPinNo VC (1~ ) 
		//                                                                                2: 4W S2 Self Short      	
		//                                                                                3: 4W S1 Self Open,   
		//                                                                                4: 4W S2 Self Open)
		//son 	dSelfErr1[g_nErrCount1][3]	:	VB Set    (Sub71.V1)            VB Set    (Sub71.V1)	           VB Set    (Sub71.V1)
		//son 	dSelfErr1[g_nErrCount1][4]	:	VC 측정값 (Sub71.X1)		    VC 측정값 (Sub71.X1)               VC 측정값 (Sub71.X1)
		//son 	dSelfErr1[g_nErrCount1][5]	:   I  측정값 (Sub71.Y1)            I  측정값 (Sub71.Y1)               I  측정값 (Sub71.Y1)
		//son 	dSelfErr1[g_nErrCount1][6]	:   R  계산값 (Sub71.R1)	        R  계산값 (Sub71.R1)               R  계산값 (Sub71.R1)
		//son 	dSelfErr1[g_nErrCount1][7]	:   nProStep1 (1:Open, 2:LV Short1, 3:LV Short2, 
		//                                     4:LV Leak1,  5:LV Leak2,  6:LV LeakA,  7:LV LeakB,  
		//                                    14:HV Leak1, 15:HV Leak2, 16:HV LeakA, 17:HV LeakB) 	

short nCardS1[300][150];//==0  ,   2= fail 

extern int nG_Language;  //SYLEE150707



__int64 GetMilSecond2()  //sylee
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();

    //SYLEE100115 return((((__int64)now.QuadPart) * ((__int64)10000000)) / ((__int64)frequency.QuadPart));
	return((((__int64)now.QuadPart) * ((__int64)1000000)) / ((__int64)frequency.QuadPart));
}

/////////////////////////////////////////////////////////////////////////////
// CChildView8

IMPLEMENT_DYNCREATE(CChildView8, CFormView)

CChildView8::CChildView8()
	: CFormView(CChildView8::IDD)
{
	//{{AFX_DATA_INIT(CChildView8)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	 
}

CChildView8::~CChildView8()
{
   g_nStopFlag=2;
}

void CChildView8::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView8)
	DDX_Control(pDX, IDC_PROGRESS4, m_Progress4);
	DDX_Control(pDX, IDC_PROGRESS3, m_Progress3);
	DDX_Control(pDX, IDC_PROGRESS2, m_Progress2);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
	DDX_Control(pDX, IDC_PROGRESS5, m_Progress5);
	DDX_Control(pDX, IDC_PROGRESS6, m_Progress6);
	DDX_Control(pDX, IDC_CHECK1, m_check1);     // check System Power 
	DDX_Control(pDX, IDC_CHECK2, m_check2);     // check System VB
	DDX_Control(pDX, IDC_CHECK3, m_check3);     // check System AD
	DDX_Control(pDX, IDC_CHECK4, m_check4);     // Check Lv Open
	DDX_Control(pDX, IDC_CHECK5, m_check5);     // Check Lv Short
	DDX_Control(pDX, IDC_CHECK6, m_check6);     // Check HV Open
	DDX_Control(pDX, IDC_CHECK7, m_check7);     // Check HV Short

	DDX_Control(pDX, IDC_EDIT1,   m_edit1);     // Begin Card No
	DDX_Control(pDX, IDC_EDIT2,   m_edit2);     // End Card No 
	DDX_Control(pDX, IDC_EDIT8,   m_edit8);     // Refer. 


	DDX_Control(pDX, IDC_EDIT11,   m_edit11);   // Net Pin Info	
	DDX_Control(pDX, IDC_EDIT12,   m_edit12);   // DD Pin Info 
	DDX_Control(pDX, IDC_EDIT13,   m_edit13);   // VB Pin	
	DDX_Control(pDX, IDC_EDIT15,   m_edit15);   // VC Pin
	
    DDX_Control(pDX, IDC_EDIT59,   m_edit59);   // SW Pin Info

	DDX_Control(pDX, IDC_COMBO1, m_combo1);     // Test Point
	DDX_Control(pDX, IDC_COMBO2, m_combo2);     // Test Case
 	 
	DDX_Control(pDX, IDC_LIST1, m_list1);

	DDX_Control(pDX, IDC_LABEL21,  m_Label21);  // VB Set Status
	DDX_Control(pDX, IDC_LABEL22,  m_Label22);  // I
	DDX_Control(pDX, IDC_LABEL24,  m_Label24);  // V 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView8, CFormView)
	//{{AFX_MSG_MAP(CChildView8)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_CHECK1, OnCheck1)   
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)  
    ON_BN_CLICKED(IDC_CHECK3, OnCheck3)   
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)  
    ON_BN_CLICKED(IDC_CHECK5, OnCheck5)   
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)  
    ON_BN_CLICKED(IDC_CHECK7, OnCheck7)   
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2_Print_SrvSelfPinMap)   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BEGIN_EVENTSINK_MAP(CChildView8, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView8) 	

    ON_EVENT(CChildView8, IDC_BUTTON101, -600 /* Click */, OnButton101_SystemPower, VTS_NONE)
    ON_EVENT(CChildView8, IDC_BUTTON102, -600 /* Click */, OnButton102_SystemVB, VTS_NONE)
    ON_EVENT(CChildView8, IDC_BUTTON103, -600 /* Click */, OnButton103_SystemAD, VTS_NONE)
    ON_EVENT(CChildView8, IDC_BUTTON104, -600 /* Click */, OnButton104_SwCardLvOpen, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON105, -600 /* Click */, OnButton105_SwCardLvShort, VTS_NONE)
    ON_EVENT(CChildView8, IDC_BUTTON106, -600 /* Click */, OnButton106_SwCardHvOpen, VTS_NONE)
    ON_EVENT(CChildView8, IDC_BUTTON107, -600 /* Click */, OnButton107_SwCardHvShort, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON108, -600 /* Click */, OnButton108_SwCardRelay, VTS_NONE)
 	ON_EVENT(CChildView8, IDC_BUTTON109, -600 /* Click */, OnButton109_SwCardRsCheck, VTS_NONE)
	
	ON_EVENT(CChildView8, IDC_BUTTON111, -600 /* Click */, OnButton111_VBSet, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON112, -600 /* Click */, OnButton112_Read, VTS_NONE)

	ON_EVENT(CChildView8, IDC_BUTTON121, -600 /* Click */, OnButton121_SetVbPin, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON122, -600 /* Click */, OnButton122_SetVcPin, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON123, -600 /* Click */, OnButton123_ResetPin, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON124, -600 /* Click */, OnButton124_PinVBOff, VTS_NONE)
 	ON_EVENT(CChildView8, IDC_BUTTON125, -600 /* Click */, OnButton125_VBPinInfo, VTS_NONE)
 	ON_EVENT(CChildView8, IDC_BUTTON128, -600 /* Click */, OnButton128_VCPinInfo, VTS_NONE)


    ON_EVENT(CChildView8, IDC_BUTTON201, -600 /* Click */, OnButton201_SystemAuto, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON207, -600 /* Click */, OnAuto_Self1, VTS_NONE) 

	ON_EVENT(CChildView8, IDC_BUTTON202, -600 /* Click */, OnButton202_StopAutoSelf, VTS_NONE)
 	ON_EVENT(CChildView8, IDC_BUTTON302, -600 /* Click */, OnButton302_ClearMessage, VTS_NONE)

	ON_EVENT(CChildView8, IDC_BUTTON701, -600 /* Click */, OnButton701_NetPinInfo, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON702, -600 /* Click */, OnButton702_DDPinInfo, VTS_NONE)
	ON_EVENT(CChildView8, IDC_BUTTON703, -600 /* Click */, OnButton703_SWPinInfo, VTS_NONE)


	//}}AFX_EVENTSINK_MAP

END_EVENTSINK_MAP()



/////////////////////////////////////////////////////////////////////////////
// CChildView8 diagnostics

#ifdef _DEBUG
void CChildView8::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView8::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView8 message handlers

BOOL CChildView8::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
 
 	 nProgress1Rate=0;
	 nProgress2Rate=0;
	 nProgress3Rate=0;
     nProgress4Rate=0;
	 nProgress5Rate=0;
     nProgress6Rate=0;


	 g_nAutoFlag=0;

 
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}







void CChildView8::A_Language() //sylee151028 //LANGUAGE CHANGE
{
  if( gDef_Language1==3){//sylee151028-1 	  

		m_Label1001.SetWindowText(LoadMessage(1,"TitleSelfTest2", 101));
		m_Label1002.SetWindowText(LoadMessage(1,"TitleSelfTest2", 102));
		m_button101.SetWindowText(LoadMessage(1,"TitleSelfTest2", 103));
		m_button102.SetWindowText(LoadMessage(1,"TitleSelfTest2", 104));
		m_button103.SetWindowText(LoadMessage(1,"TitleSelfTest2", 105));
		m_button201.SetWindowText(LoadMessage(1,"TitleSelfTest2", 106));
		m_Label28.SetWindowText(LoadMessage(1,"TitleSelfTest2", 107));
		m_Label31.SetWindowText(LoadMessage(1,"TitleSelfTest2", 108));
		m_Label35.SetWindowText(LoadMessage(1,"TitleSelfTest2", 109));
		m_Label36.SetWindowText(LoadMessage(1,"TitleSelfTest2", 110));
		m_Label44.SetWindowText(LoadMessage(1,"TitleSelfTest2", 111));
		m_button302.SetWindowText(LoadMessage(1,"TitleSelfTest2", 112));
		m_Label64.SetWindowText(LoadMessage(1,"TitleSelfTest2", 113));
		m_button104.SetWindowText(LoadMessage(1,"TitleSelfTest2", 114));
		m_button105.SetWindowText(LoadMessage(1,"TitleSelfTest2", 115));
		m_button106.SetWindowText(LoadMessage(1,"TitleSelfTest2", 116));
		m_button107.SetWindowText(LoadMessage(1,"TitleSelfTest2", 117));
		m_button108.SetWindowText(LoadMessage(1,"TitleSelfTest2", 118));
		m_button207.SetWindowText(LoadMessage(1,"TitleSelfTest2", 119));
		m_button202.SetWindowText(LoadMessage(1,"TitleSelfTest2", 120));

		m_Label39.SetWindowText(LoadMessage(1,"TitleSelfTest2", 121));
		m_button125.SetWindowText(LoadMessage(1,"TitleSelfTest2", 122));
		m_button128.SetWindowText(LoadMessage(1,"TitleSelfTest2", 123));
		m_button111.SetWindowText(LoadMessage(1,"TitleSelfTest2", 124));
		m_button121.SetWindowText(LoadMessage(1,"TitleSelfTest2", 125));
		m_button122.SetWindowText(LoadMessage(1,"TitleSelfTest2", 126));
		m_button112.SetWindowText(LoadMessage(1,"TitleSelfTest2", 127));
		m_Label40.SetWindowText(LoadMessage(1,"TitleSelfTest2", 128));

		m_button124.SetWindowText(LoadMessage(1,"TitleSelfTest2", 129));
		m_button123.SetWindowText(LoadMessage(1,"TitleSelfTest2", 130));
		m_button701.SetWindowText(LoadMessage(1,"TitleSelfTest2", 131));
		m_button702.SetWindowText(LoadMessage(1,"TitleSelfTest2", 132));
		m_button703.SetWindowText(LoadMessage(1,"TitleSelfTest2", 133));

 
  }

}




void CChildView8::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default



    switch (nIDEvent)
	{
		// The clock timer?
	case 10:
		/*if(nProgress1Rate>100){
			 nProgress1Rate=0;
		}else{
			nProgress1Rate++;
		}
		*/
    	m_Progress1.SetPos(nProgress1Rate);
		break; 

	case 20:
	/*	if(nProgress2Rate>100){
			 nProgress2Rate=0;
		}else{
			nProgress2Rate++;
		}
		*/
    	m_Progress2.SetPos(nProgress2Rate); 
		break; 
	case 30:
		/*
		if(nProgress3Rate>100){
			 nProgress3Rate=0;
		}else{
			nProgress3Rate++;
		}
		*/
    	m_Progress3.SetPos(nProgress3Rate);
		break; 

	case 40:
		/*
		if(nProgress4Rate>100){
			 nProgress4Rate=0;
		}else{
			nProgress4Rate++;
		}
		*/
    	m_Progress4.SetPos(nProgress4Rate); 
		break; 

	case 50:
		/*
		if(nProgress5Rate>100){
			 nProgress5Rate=0;
		}else{
			nProgress5Rate++;
		}
		*/
    	m_Progress5.SetPos(nProgress5Rate); 
		break; 

	case 60:
		
		/*if(nProgress6Rate>100){
			 nProgress6Rate=0;
		}else{
			nProgress6Rate++;
		}
		*/
    	m_Progress6.SetPos(nProgress6Rate); 
		break; 

	}

	// Update the dialog
	//
	UpdateData(FALSE);
 
	
	CFormView::OnTimer(nIDEvent);
}

void CChildView8::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();	

	CFont font2;

	LOGFONT lf;

	int nCardX;//sylee150210 //ACE380
	CString str;//sylee150210 //ACE380

	//SYLEE151007 	nCardX=2;//sylee150210 //ACE380

	nCardX=1;//SYLEE151007 //ACE400



	lf.lfHeight			= MulDiv(9, 500, 300);
	lf.lfWidth			= 0;
	lf.lfEscapement		= 0;
	lf.lfOrientation	= 0; 
	lf.lfWeight			= FW_NORMAL;
	lf.lfItalic			= FALSE;
	lf.lfUnderline		= FALSE;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= HANGEUL_CHARSET;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfQuality		= DEFAULT_QUALITY;
	lf.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;

	font2.CreateFontIndirect(&lf);


	m_list1.SetFont(&font2); 


	FileSysInfo01.LoadSaveSet13(2);//load //sylee170607
	FileSysInfo01.LoadSaveSet12(_LOAD); //load: 2

	//son m_combo1 data 초기화        : son-32KPIN-20200227 
	m_combo1.ResetContent();    //son230328 
    m_combo1.InsertString(-1, "None< Pin Set>");        //son None< Pin Set> 
    m_combo1.InsertString(-1, "A Box");                 //son A Box
    m_combo1.InsertString(-1, "B Box");                 //son B Box
    m_combo1.InsertString(-1, "C Box");                 //son C Box
    m_combo1.InsertString(-1, "D Box");                 //son D Box
    m_combo1.InsertString(-1, "E Box");                 //son E Box
    m_combo1.InsertString(-1, "F Box");                 //son F Box
    m_combo1.InsertString(-1, "G Box");                 //son G Box
    m_combo1.InsertString(-1, "H Box");                 //son H Box
                                                        
	for (int i=1; i < PIN_TOT_END; i++)                 //son 4K
	{                                                   //son 8K
		m_combo1.InsertString(-1, STR_MAX_PIN(i) );     //son 16K
	}                                                   //son 32K
                                                        //son 20K
                                                        //son 24K
                                                        //son 24K_FULL_TRANS
                                                        //son 16K_FULL_TRANS

    //son 현재 SysSet12.m_nPinTotMax에 맞추어서 m_combo1을 설정함.
	if (SysSet12.m_nPinTotMax >= PIN_TOT_4K && SysSet12.m_nPinTotMax < PIN_TOT_END)
        m_combo1.SetCurSel(8 + SysSet12.m_nPinTotMax);
    else
		m_combo1.SetCurSel(1);

	nTestCase1=1;
	m_combo2.SetCurSel(nTestCase1);


	m_edit1.SetWindowText("1");
	m_nCardNoStart = 1;


    //son240125 ChildView8 코드는 card skip하지 않고 user card번호로 사용하므로 getCardNoEnd() 함수 사용 불가.
    //          FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax)로 교체함
    //m_nCardNoEnd = FileSysInfo01.getCardNoEnd(1, SysSet12.m_nPinTotMax); //son211020 Skip card 함수
    m_nCardNoEnd = FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax, SysSet13.m_nPinBlockType); //son240125 Skip no card 함수


	str.Format("%d",m_nCardNoEnd);//sylee150210
	m_edit2.SetWindowText(str);//sylee150210 //ACE380

	m_edit8.SetWindowText("8");
	nISOSet1 = 8;
	// TODO: Add your specialized code here and/or call the base class




}







void CChildView8::OnSelchangeCombo1() 
{

	int Tmp,a,b;
	CString str1,str2;
      
	Tmp= m_combo1.GetCurSel(); 


	if(SysSet12.m_nPinBoxMax==1)  //2k  //sylee121005
    {
        if( Tmp>=1 && Tmp<=8  )
        {
            a=(Tmp-1)*16+1;
            b=Tmp*16;
            str1.Format("%d",a); 
            str2.Format("%d",b);
            m_edit1.SetWindowText(str1);
            m_edit2.SetWindowText(str2);
        }else if( Tmp==9 ){//SYLEE140617
            b=(int)(32);//SYLEE140617
        }else if( Tmp==10 ){//SYLEE140617
            b=64*1;//SYLEE140617
        }else if( Tmp==11 ){//SYLEE140617
            b=64*2;//SYLEE140617
        }else if( Tmp==12 ){  //SYLEE140617
            b=64*4;//SYLEE140617
        }



    }
    else
    {
        if( Tmp>=1 && Tmp<=8  )
        {
            a=(Tmp-1)*32+1;
            b=Tmp*32;
            str1.Format("%d",a); 
            str2.Format("%d",b);
            m_edit1.SetWindowText(str1);
            m_edit2.SetWindowText(str2);
        }else if( Tmp==9 ){//SYLEE140617
            b=(int)(32);//SYLEE140617
        }else if( Tmp==10 ){//SYLEE140617
            b=64*1;//SYLEE140617
        }else if( Tmp==11 ){//SYLEE140617
            b=64*2;//SYLEE140617
        }else if( Tmp==12 ){  //SYLEE140617
            b=64*4;//SYLEE140617
        }

    }

    Get_nCardPinMax(); //sylee150903 //ACE400        //son231227

    //son 2K box용 64pin card 인 경우
    if(nCardPinMax==64){ //sylee150903-1  //ACE400
        b=b*2; //sylee150903-1 //ACE400
    }



    if( Tmp==9 || Tmp==10 || Tmp==11 || Tmp==12 ){//SYLEE140617
        str2.Format("%d",b);
        m_edit1.SetWindowText("1");
        m_edit2.SetWindowText(str2);
    }
 

}



void CChildView8::OnDestroy() 
{
    CFormView::OnDestroy();

    g_nInitFlag1 = 0; 
    KillTimer(10);
    KillTimer(20);
    KillTimer(30);
    KillTimer(40);	 

    KillTimer(50);
    KillTimer(60);	


    // TODO: Add your message handler code here	
}



BOOL CChildView8::OnEraseBkgnd(CDC* pDC)    
{
//NO1
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));

 
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

void CChildView8::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
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

HBRUSH CChildView8::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here


	int nID = pWnd->GetDlgCtrlID();
	if(g_hbr != NULL)	::DeleteObject(g_hbr);
 
	else
	{
		//흰배경에 검은색 텍스트로 설정. 폼뷰 배경색이 이것으로 결정됨.
		g_hbr = CreateSolidBrush(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(0, 0, 0));
	}

 	pDC->SetBkMode(TRANSPARENT);


	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CChildView8::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
}

BOOL CChildView8::PreTranslateMessage(MSG* pMsg) 
{

	CString str;


	// TODO: Add your specialized code here and/or call the base class
	if( g_nInitFlag1==8  &&  g_nInitFlag2==1 ) // view8 init   
    {	 	

         g_nSelfRunFlag=0;
		 g_nInitFlag2=2;
		
         //g_nInitFlag2 = 0; 
	 	 SetTimer(10, 300, NULL);   //son240520 30 -> 300 ms로 변경
		 SetTimer(20, 300, NULL);   //son240520 30 -> 300 ms로 변경
		 SetTimer(30, 300, NULL);   //son240520 30 -> 300 ms로 변경
		 SetTimer(40, 300, NULL);   //son240520 30 -> 300 ms로 변경
		 SetTimer(50, 300, NULL);   //son240520 30 -> 300 ms로 변경
		 SetTimer(60, 300, NULL);   //son240520 30 -> 300 ms로 변경
/*		 
		m_edit1.SetWindowText("1");
		m_nCardNoStart = 0;

		m_edit2.SetWindowText("16");
		m_nCardNoEnd = 15;

 */

		FileSysInfo01.LoadSaveSet12(_LOAD);	//load: 2


        str.Format("OFF");
	    m_Label21.SetCaption(str);

		m_nCardNoStart = 1;
		m_edit1.SetWindowText("1");


        //son240125 ChildView8 코드는 card skip하지 않고 user card번호로 사용하므로 getCardNoEnd() 함수 사용 불가.
        //          FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax)로 교체함
        //m_nCardNoEnd = FileSysInfo01.getCardNoEnd(1, SysSet12.m_nPinTotMax); //son211020 Skip card 함수
        m_nCardNoEnd = FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax, SysSet13.m_nPinBlockType); //son240125 Skip no card 함수

        str.Format("%d",m_nCardNoEnd);  //son211020
        m_edit2.SetWindowText(str);     //son211020 


	    m_edit8.SetWindowText("8");
		nISOSet1 = 8;

        //son 현재 SysSet12.m_nPinTotMax에 맞추어서 m_combo1을 설정함.
        if (SysSet12.m_nPinTotMax >= PIN_TOT_4K && SysSet12.m_nPinTotMax <  PIN_TOT_END)
            m_combo1.SetCurSel(8 + SysSet12.m_nPinTotMax);
        else
            m_combo1.SetCurSel(1);

		nTestCase1=1;
		m_combo2.SetCurSel(nTestCase1);


/*	m_edit31.SetWindowText("0");
		nVBStartPin = 0;
		m_edit32.SetWindowText("127");
		nVBEndPin = 127;
		m_edit33.SetWindowText("128");
		nVCStartPin = 128;
		m_edit34.SetWindowText("255");
		nVCEndPin = 255;
*/
		 m_check1.SetCheck(1);
 		 m_check2.SetCheck(1);
		 m_check3.SetCheck(1);
 		 m_check4.SetCheck(1);
		 m_check5.SetCheck(1);
 		 m_check6.SetCheck(1);
		 m_check7.SetCheck(1);

		 nDischarge1=0;
//		 m_check8.SetCheck(nDischarge1);


 		 nAllPinSearchFlag=0;
//		 m_check8.SetCheck(nAllPinSearchFlag);





	}




    if( g_nSelfAutoFlag1==10){//SYLEE130916
        g_nSelfAutoFlag1=1;
#ifndef _PC_TEST_   //son240608
        OnAuto_Self1();
#endif

    }

    if( g_nSelfAutoFlag1==2)
    {
        g_nSelfAutoFlag1=3;

        // OnList1Mes1("*** Auto Self Test    Start***");
        
        //son211020 CFileSysInfo::getEndBoxNo(int nPinTotMax) 함수호출로 변경
        //son240520 if( g_nSelfAutoFlag1==2) 내부로 이동
        int EndSelfLoop;
        EndSelfLoop = FileSysInfo01.getEndBoxNo(SysSet12.m_nPinTotMax);  //son211020  //son240520


        for(int box1=1; box1<=EndSelfLoop; box1++)   //son-32KPIN-20200228: jk를 box1로 수정.
        {
            if(g_nSelfAutoFlag1==0){					 
                break;
            }		

            //son-32KPIN-20200228 : 16K_FT 상부 뒤쪽 두개를 skip
            //son 16K_FT의 경우는 상부 2box1(box1, box2)만, 하부 2box1(box5, box6) 사용.
            //    box3과 box4는 건너 뛰어야 한다.  box3: card64 ~ card95 
            //                                     box4: card96 ~ card127
            //if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS
			//	|| SysSet12.m_nPinTotMax == PIN_TOT_12K	//son210202  box1부터 시작, 3번째, 4번째 box1 skip
			//	|| SysSet12.m_nPinTotMax == PIN_TOT_12K_FULL_TRANS) //son210319
			if (ModeSet12.IsUpper8k(SysSet12.m_nPinTotMax))     //son210831
            {
                if (box1 >= 3  && box1 < 5) 
                    continue;
            }

			//son-8K4097-20200605 : 8K_구형4097  상부 뒤쪽  1 box1를 skip
			//son 8K_구형4097의 경우는 상부 1box1(box0)만, 하부 1box1(box2) 사용.
			//    box1은 건너 뛰어야 한다.  box1: card32 ~ card63 
			//if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
			//        || SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)  //son210319 //son230216_1
			if (ModeSet12.IsUpper4k(SysSet12.m_nPinTotMax))     //son210831
			{
                if (box1 >= 2  && box1 < 5)   //box1부터 시작, 2번째, 3번째, 4번째 box1 skip
					continue;
			}

            //son210531 상부 3 box1, 하부 1 box1, 하부 4번째 box1는 skip 
            //if (IS_UPPER_12K(SysSet12.m_nPinTotMax))    //son210531
            if (ModeSet12.IsUpper12k(SysSet12.m_nPinTotMax))    //son240522
			{
				if (box1 == 4) //box0부터 시작, 4번째 box1 skip
					continue;
			}

            if (SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)      //son211020
			{
				if (box1 >= 3) //son211020 상부 1, 2 box만 존재. box5부터 하부 통째로 skip      
					continue;
			}
    


            if(SysSet12.m_nPinBoxMax==1)  //2k  //sylee121005
            {
                if(box1==1){//4k								 
                    OnList1Mes1("      ");
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX A : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 1 ~ ");
                    m_edit1.SetWindowText("1");
                    m_nCardNoStart = 1;
                    m_edit2.SetWindowText("16");
                    m_nCardNoEnd = 16;
                }
                else if(box1==2){//8k								
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX B : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("               * Start Card 17 ~ ");
                    m_edit1.SetWindowText("17");
                    m_nCardNoStart = 17;
                    m_edit2.SetWindowText("32");
                    m_nCardNoEnd = 32;
                    Sleep(500);
                }
                else if(box1==3){//16k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX C : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 33 ~ ");
                    m_edit1.SetWindowText("33");
                    m_nCardNoStart = 33;
                    m_edit2.SetWindowText("48");
                    m_nCardNoEnd = 48;
                    Sleep(500);
                }
                else if(box1==4){//16k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX D : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 49 ~ ");
                    m_edit1.SetWindowText("49");
                    m_nCardNoStart = 49;					
                    m_edit2.SetWindowText("64");
                    m_nCardNoEnd = 64;	
                    Sleep(500);
                }
                else if(box1==5){//32k								
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX E : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("               * Start Card 65 ~ ");
                    m_edit1.SetWindowText("65");
                    m_nCardNoStart = 65;
                    m_edit2.SetWindowText("80");
                    m_nCardNoEnd = 80;
                    Sleep(500);
                }
                else if(box1==6){//32k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX F : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 81 ~ ");
                    m_edit1.SetWindowText("81");
                    m_nCardNoStart = 81;
                    m_edit2.SetWindowText("96");
                    m_nCardNoEnd = 96;
                    Sleep(500);
                }
                else if(box1==7){//32k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX G : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 97 ~ ");
                    m_edit1.SetWindowText("97");
                    m_nCardNoStart = 97;					
                    m_edit2.SetWindowText("112");
                    m_nCardNoEnd = 112;	
                    Sleep(500);								 
                }
                else if(box1==8){//32k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX H : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 113 ~ ");
                    m_edit1.SetWindowText("113");
                    m_nCardNoStart = 113;					
                    m_edit2.SetWindowText("128");
                    m_nCardNoEnd = 128;	
                    Sleep(500);								 
                }


            }
            else
            {

                if(box1==1){//4k


                    OnList1Mes1("      ");
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX A : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 1 ~ ");
                    m_edit1.SetWindowText("1");
                    m_nCardNoStart = 1;
                    m_edit2.SetWindowText("32");
                    m_nCardNoEnd = 32;
                }else if(box1==2){//8k

                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX B : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("               * Start Card 33 ~ ");
                    m_edit1.SetWindowText("33");
                    m_nCardNoStart = 33;
                    m_edit2.SetWindowText("64");
                    m_nCardNoEnd = 64;
                    Sleep(500);
                }else if(box1==3){//16k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX C : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 65 ~ ");
                    m_edit1.SetWindowText("65");
                    m_nCardNoStart = 65;
                    m_edit2.SetWindowText("96");
                    m_nCardNoEnd = 96;
                    Sleep(500);
                }else if(box1==4){//16k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX D : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 97 ~ ");
                    m_edit1.SetWindowText("97");
                    m_nCardNoStart = 97;					
                    m_edit2.SetWindowText("128");
                    m_nCardNoEnd = 128;	
                    Sleep(500);
                }else if(box1==5){//32k

                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX E : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("               * Start Card 129 ~ ");
                    m_edit1.SetWindowText("129");
                    m_nCardNoStart = 129;
                    m_edit2.SetWindowText("160");
                    m_nCardNoEnd = 160;
                    Sleep(500);
                }else if(box1==6){//32k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX F : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 161 ~ ");
                    m_edit1.SetWindowText("161");
                    m_nCardNoStart = 161;
                    m_edit2.SetWindowText("192");
                    m_nCardNoEnd = 192;
                    Sleep(500);
                }else if(box1==7){//32k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX G : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 193 ~ ");
                    m_edit1.SetWindowText("193");
                    m_nCardNoStart = 193;					
                    m_edit2.SetWindowText("224");
                    m_nCardNoEnd = 224;	
                    Sleep(500);

                }else if(box1==8){//32k
                    OnList1Mes1("      "); 
                    OnList1Mes1(" =============================================");
                    OnList1Mes1(" < BOX H : Start >  ");
                    OnList1Mes1("      ");
                    OnList1Mes1("              * Start Card 225 ~ ");
                    m_edit1.SetWindowText("225");
                    m_nCardNoStart = 225;					
                    m_edit2.SetWindowText("256");
                    m_nCardNoEnd = 256;	
                    Sleep(500);

                }

            }//2k


            OnAuto_Self1();//sylee120903

            if(box1==1){//4k
                OnList1Mes1("              * End Card ~32  ");
            }else if(box1==2){//8k
                OnList1Mes1("               * End Card ~64  ");
            }else if(box1==3){//16k
                OnList1Mes1("               * End Card ~96  ");					
            }else if(box1==4){//16k
                OnList1Mes1("              * End Card ~128  ");				 	 

            }else if(box1==5){//32k
                OnList1Mes1("               * End Card ~160  ");
            }else if(box1==6){//32k
                OnList1Mes1("               * End Card ~192 ");					
            }else if(box1==7){//32k
                OnList1Mes1("              * End Card ~224  ");				 	 
            }else if(box1==8){//32k
                OnList1Mes1("               * End Card ~256  ");				 

            }
            //	*/




        }

        // OnList1Mes1("*** Auto Self Test     End***");


        OnList1Mes1(" ==============================================");
        g_nSelfAutoFlag1=1;


    }


	return CFormView::PreTranslateMessage(pMsg);
}




 


 

void CChildView8::OnButton101_SystemPower() //power //son230302
{


	return ;


	g_nStopFlag=0;

	if(g_nSelfRunFlag>0){
	  //  AfxMessageBox("   Running.  \n \n       Selftest   System Check \n   Power Check    Start Error  !!!"); 
        return;
	}
    	g_nSelfRunFlag=1;


	  OnList1Mes1("*** HW Power Check Start***");

      OnCheckPowerHW1(1);  
	  OnList1Mes1("*** HW Power Check Finish***");
	  OnList1Mes1("                         ");



	 g_nSelfRunFlag=0;

	  return ;
}

void CChildView8::OnButton102_SystemVB() //vb check //son230302
{

		return ;

	g_nStopFlag=0;

	if(g_nSelfRunFlag>0){
	  //  AfxMessageBox("   Running.   \n \n      Selftest   System Check    \n   VB Check    Start Error  !!!"); 
        return;
	}
	  g_nSelfRunFlag=1;


	  OnList1Mes1("*** VB   Check Start***");
	  OnCheckVB1();  
	  OnList1Mes1("*** VB  Check Finish***");
	  OnList1Mes1("                       ");	 

	 g_nSelfRunFlag=0;


	return ;
}


void CChildView8::OnButton103_SystemAD()  //son230302
{



		return ;

 


	return ;
}
	 //SYLEE20120506   VER3.1   
int CChildView8::OnCheckPowerHW1(int MesType) //power
{


	return 1;
/*


	  int Ret; 
	  int OkDisplay;
	  int nBoxUse[10]={0,
		               1,1,0,0,  //A~D
					   0,0,0,0,  //E~H
					   0};   //A ~ H       8EA          UINT  2K , 4K           MAX TOTAL  32K
	  int nFail;

	  CString str;


	  nFail=0;
	  OkDisplay=0;
 
	  Ret=Sub71.OnPowerCheck1(2);
	   if(Ret==1){
		   str.Format("Error No 8001  : MAIN 5V  off. Check ? ");
		   if(MesType ==1){
			     OnList1Mes1(str);
			//	 OnList1Mes1("Error No 8001  : MAIN 5V  off. Check ? ");
		   }else{			   
			   AfxMessageBox(str, MB_OK);
		   }
	   }else{
		   if(OkDisplay==1){			    
			   if(MesType ==1){					 
			         OnList1Mes1("  OK,  MAIN 5V ON. ");			 
			   }				
		   }
	   }


	 if( g_nStopFlag==2){

		g_nSelfRunFlag=0;
		return 0;
	 }
  	 
	 DoEvents();


	   Ret=Sub71.OnPowerCheck1(1);
	   if(Ret==1){
		        str.Format("Error No 8001  : MAIN 12V  off. Check ? ");
		   if(MesType ==1){
			    OnList1Mes1(str);
			//	 OnList1Mes1("Error No 8001  : MAIN 12V  off. Check ? ");
		   }else{			   
			   AfxMessageBox(str, MB_OK);
		   }

	   }else{
		   if(OkDisplay==1){
				OnList1Mes1("  OK,  MAIN 12V ON. ");
		   }
	   }

	  if( g_nStopFlag==2){
		  g_nSelfRunFlag=0;
		  return 0;
	  }
	   DoEvents();

 	   Ret=Sub71.OnPowerCheck1(3);
	   if(Ret==1){
		   str.Format("Error No 8001  : MAIN 24V  off. Check ? ");			
		   if(MesType ==1){
			     OnList1Mes1(str);
			//	 OnList1Mes1("Error No 8001  : MAIN 24V  off. Check ? ");
		   }else{			   
			   AfxMessageBox(str, MB_OK);
		   }

	   }else{
		   if(OkDisplay==1){
				OnList1Mes1("  OK,  MAIN 24V ON. ");
		   }
	   }


	  if( g_nStopFlag==2){
		  g_nSelfRunFlag=0;
		 return 0;
	  }
	   DoEvents();


	   Ret=Sub71.OnPowerCheck1(4);
	   if(Ret==1){
		   str.Format("Error No 8001  : MAIN HV  off. Check ? ");			
		   if(MesType ==1){
			     OnList1Mes1(str);
			//	 OnList1Mes1("Error No 8001  : MAIN HV  off. Check ? ");
		   }else{			   
			   AfxMessageBox(str, MB_OK);
		   }			
		}else{
		   if(OkDisplay==1){
				OnList1Mes1("  OK,  MAIN HV  ON. ");
		   }
		}

	   if( g_nStopFlag==2){
		   g_nSelfRunFlag=0;
	       return 0;
	   }
	   	   DoEvents();









//=================================================
 	   if( nBoxUse[4]==1){
		   Ret=Sub71.OnPowerCheck1(5);
		   if(Ret==1){
			    str.Format("Error No 8001 :SWITCH CARD  BOX D - VC 5V  off. Check ?" );		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX D - VC 5V  off. Check ?" );
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
 		   Ret=Sub71.OnPowerCheck1(6);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD BOX D - VB 5V  off. Check ?" );		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX D - VB 5V  off. Check ?" );
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
	   }

	   if( g_nStopFlag==2){
		   g_nSelfRunFlag=0;
	       return 0;
	   }
	   	   DoEvents();


	   if( nBoxUse[3]==1){
 		   Ret=Sub71.OnPowerCheck1(7);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD  BOX C - VC 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX C - VC 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
		   Ret=Sub71.OnPowerCheck1(8);
		   if(Ret==1){
			    str.Format("Error No 8001: SWITCH CARD  BOX C - VB 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX C - VB 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
	   }

	   if( g_nStopFlag==2){
		   g_nSelfRunFlag=0;
	       return 0;
	   }

	   	   DoEvents();
   
 	   if( nBoxUse[2]==1){
 		   Ret=Sub71.OnPowerCheck1(9);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD  BOX B - VC 5V  off. Check ?" );		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX B - VC 5V  off. Check ?" );
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
		   Ret=Sub71.OnPowerCheck1(10);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD  BOX B - VB 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX B - VB 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
	   }


	   if( g_nStopFlag==2){
	       return 0;
	   }

	   	   DoEvents();

	   if( nBoxUse[1]==1){
 		   Ret=Sub71.OnPowerCheck1(11);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD  BOX A - VC 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX A - VC 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
		   Ret=Sub71.OnPowerCheck1(12);
		   if(Ret==1){
			    str.Format("Error No 8001: SWITCH CARD  BOX A - VB 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//	OnList1Mes1("Error No 8001  : BOX A - VB 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }			
		   }
	   }
  
	   if( g_nStopFlag==2){
		   g_nSelfRunFlag=0;
	       return 0;
	   }

	   	   DoEvents();
//=================================================

	   if( nBoxUse[8]==1){
		   Ret=Sub71.OnPowerCheck1(13);
		   if(Ret==1){
			    str.Format("Error No 8001 :SWITCH CARD  BOX H - VC 5V  off. Check ?" );		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX H - VC 5V  off. Check ?" );
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
 		   Ret=Sub71.OnPowerCheck1(14);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD  BOX H - VB 5V  off. Check ?" );		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//	OnList1Mes1("Error No 8001  : BOX H - VB 5V  off. Check ?" );
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }			
		   }
	   }

	   if( g_nStopFlag==2){
	       return 0;
	   }

	   	   DoEvents();


	   if( nBoxUse[7]==1){
 		   Ret=Sub71.OnPowerCheck1(15);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD  BOX G - VC 5V  off. Check ? ");		
			   if(MesType ==1){
				   OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX G - VC 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }
				
		   }
		   Ret=Sub71.OnPowerCheck1(16);
		   if(Ret==1){
			    str.Format("Error No 8001 :SWITCH CARD BOX G - VB 5V  off. Check ? ");		
			   if(MesType ==1){
				   OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX G - VB 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
	   }


	   if( g_nStopFlag==2){
		   g_nSelfRunFlag=0;
	       return 0;
	   }

	   	   DoEvents();
   
 	   if( nBoxUse[6]==1){
 		   Ret=Sub71.OnPowerCheck1(17);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD BOX F - VC 5V  off. Check ?" );		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX F - VC 5V  off. Check ?" );
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
		   Ret=Sub71.OnPowerCheck1(18);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD BOX F - VB 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX F - VB 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
	   }

	   if( g_nStopFlag==2){
		   g_nSelfRunFlag=0;
	       return 0;
	   }
	   	   DoEvents();

	   if( nBoxUse[5]==1){
 		   Ret=Sub71.OnPowerCheck1(19);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD BOX E - VC 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//	OnList1Mes1("Error No 8001  : BOX E - VC 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }
			
		   }
		   Ret=Sub71.OnPowerCheck1(20);
		   if(Ret==1){
			    str.Format("Error No 8001 : SWITCH CARD BOX E - VB 5V  off. Check ? ");		
			   if(MesType ==1){
					 OnList1Mes1(str);
				//OnList1Mes1("Error No 8001  : BOX E - VB 5V  off. Check ? ");
			   }else{			   
				   AfxMessageBox(str, MB_OK);
			   }				
		   }
	   }
	   //=================================================
	   DoEvents();

*/

	   return 1;
}

 



	 /*//SYLEE20120506   VER3  조이텍용  
int CChildView8::OnCheckPowerHW1() //power
{
	  int Ret; 

  

	  
	  Ret=Sub71.OnPowerCheck1(2);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : MAIN 5V  off. Check ? ");
	   }else{
		   OnList1Mes1("  OK,  MAIN 5V ON. ");
	   }


	   Ret=Sub71.OnPowerCheck1(1);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : MAIN 12V  off. Check ? ");
	   }else{
		   OnList1Mes1("  OK,  MAIN 12V ON. ");
	   }

 	   Ret=Sub71.OnPowerCheck1(3);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : MAIN 24V  off. Check ? ");
	   }else{
		   OnList1Mes1("  OK,  MAIN 24V ON. ");
	   }
	   Ret=Sub71.OnPowerCheck1(4);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : MAIN HV  off. Check ? ");
		}else{
		   OnList1Mes1("  OK,  MAIN HV  ON. ");
		}



/*
	   Ret=Sub71.OnPowerCheck1(5);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX D - VC 5V  off. Check ?" );
	   }
 	   Ret=Sub71.OnPowerCheck1(6);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX D - VB 5V  off. Check ?" );
	   }
 	   Ret=Sub71.OnPowerCheck1(7);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX C - VC 5V  off. Check ? ");
	   }
	   Ret=Sub71.OnPowerCheck1(8);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX C - VB 5V  off. Check ? ");
	   }

  */


/*
 	   Ret=Sub71.OnPowerCheck1(9);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX B - VC 5V  off. Check ?" );
	   }
	   Ret=Sub71.OnPowerCheck1(10);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX B - VB 5V  off. Check ? ");
	   }
 	   Ret=Sub71.OnPowerCheck1(11);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX A - VC 5V  off. Check ? ");
	   }
	   Ret=Sub71.OnPowerCheck1(12);
	   if(Ret==1){
			OnList1Mes1("Error No 8001  : BOX A - VB 5V  off. Check ? ");
	   }
  */

/*
	   return 1;
}

*/



/*
O - 64	52	DT12V	DETECTION MAIN 12V	0 : 정상, 1: ERROR
O - 65	53	DT5V	DETECTION MAIN 5V	0 : 정상, 1: ERROR
O - 66	54	DT24V	DETECTION MAIN 24V	0 : 정상, 1: ERROR
O - 67	55	DTHV	DETECTION MAIN HV	0 : 정상, 1: ERROR
O - 70	56	DT5VC_D	DETECTION BOX D - VC 5V	0 : 정상, 1: ERROR
O - 71	57	DT5VB_D	DETECTION BOX D - VB 5V	0 : 정상, 1: ERROR
O - 72	58	DT5VC_C	DETECTION BOX C - VC 5V	0 : 정상, 1: ERROR
O - 73	59	DT5VB_C	DETECTION BOX C - VB 5V	0 : 정상, 1: ERROR
O - 74	60	DT5VC_B	DETECTION BOX B - VC 5V	0 : 정상, 1: ERROR
O - 75	61	DT5VB_B	DETECTION BOX B - VB 5V	0 : 정상, 1: ERROR
O - 76	62	DT5VC_A	DETECTION BOX A - VC 5V	0 : 정상, 1: ERROR
O - 77	63	DT5VB_A	DETECTION BOX A - VB 5V	0 : 정상, 1: ERROR
*/

 
 

void CChildView8::OnList1Mes1(CString str) //ad
{        
		m_list1.AddString(str); 			 		 
		m_list1.SetTopIndex(m_list1.GetCount()-1); 
		g_nListCount++;
		if( g_nListCount>1000){
			g_nListCount=0;
			m_list1.ResetContent();
		} 

}


 


// int nMod1 = open, ushort, short, hr , hv , 4w 
// m_nMode1  1~65    
//  nProMod1= R VALE
//int nPStep1  1~ 7 step
 


int CChildView8::OnCheckVB1()   //son230302
{

    CString   str,str1; 

    int nRet1;
    double dVbR2,dVbR1;

    int VLSet[4]={1,5,10,20};
    int VHSet[4]={100,150,200,250}; 

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //mode ( 1~65)
    //step ( open, ㅏushort, short, hr, hv  4w)

    Sub71.OnPinAllReset1();


    for( int k=0; k<4; k++)
    {
        DoEvents();

        nVSet1= VLSet[k];  
        nVSet2= VHSet[k];			   


        nRet1=Sub71.AOnPoweroff2();//sylee20120702
        if(nRet1!=1){ 
            AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
            return 0;
        }

        for( int j=0; j<200; j++){
            DoEvents();

            if( g_nStopFlag==2){
                g_nSelfRunFlag=0;
                goto Task1;
            }

        }

        //Sub71.Delay_100ns(1000000);  //1000ms 

        nRet1=Sub71.AOnVSetMulti1(nVSet1,nVSet1,nVSet2,nVSet2) ;

        if( nRet1!=1){ 
            AfxMessageBox(" Error No 8023, \n   V SET ERROR !,   Self VB CHECK SET error!!!"); 
            return 0;
        } 

        for(  j=0; j<200; j++){
            DoEvents();
            if( g_nStopFlag==2){
                g_nSelfRunFlag=0;
                goto Task1;
            }
        }



        for( int i=1; i<=4; i++) 
        {
            DoEvents();

            int Lo1=1000;   //vb  retry
            int Lo2=10;

            if( i==1 || i==2 ){
                nVSet=nVSet1;
            }else{
                nVSet=nVSet2;
            }
            nVRel=i;


Task101:    


            Sub71.nMode1=0;

            Sub71.AOnReadVB1Run1(nVRel);    // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;

            Sub71.Delay_100ns(30000);  //3ms 

            if(nVSet<=5){
                dVbR2=nVSet*(0.35);//35%
            }else{
                dVbR2=nVSet*(0.1);//10%
            }
            dVbR1=nVSet-dVbR2;
            dVbR2=nVSet+dVbR2;

            if( Sub71.V1>300){
                if(Lo1-->0){
                    Sub71.Delay_100ns(30000);  //3ms   
                    goto Task101;
                }
                str.Format("  * VB%d,  Set=%d  ERROR : =>VB Read=%.2f, ",nVRel, nVSet , Sub71.V1);
                OnList1Mes1(str); 
            }

            if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
                if(Lo1-->0){
                    Sub71.Delay_100ns(100000);  //10ms   
                    goto Task101;
                } 
                str.Format("  * VB %d,   Set=%.2f  ERROR : =>VB Read=%.2f, ",nVRel,nVSet , Sub71.V1);    
            }else{
                if(Lo2-->0){
                    Sub71.Delay_100ns(100000);  //10ms   
                    goto Task101;
                } 

                str.Format("  * VB %d,   Set=%.2f  OK : =>VB Read=%.2f, V",nVRel,nVSet , Sub71.V1);
            }
            OnList1Mes1(str);  

        }//for( i=1; i<=4; i++)


    }//for( k=0; k<5; k++)



Task1:


    Sub71.ARunoff1();//sylee20111020//vc sense off       /VC-SS  CMD off

    DoEvents();

    //  nPinSetVSet=0;  //2=hv;

    return 1;

}





void CChildView8::OnButton104_SwCardLvOpen() //lv con   //son230302
{
	//10-> 46  //self 	//sylee20120509   46, 47


	if(g_nSelfRunFlag>0){
	//    AfxMessageBox("   Running.        Selftest   Switch Card  LV Open Chck    Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;  

	OnATestRun( __LV,   1,   1);// 46   LV OPEN     //son231222
 
    g_nSelfRunFlag=0;

	return ;
}


void CChildView8::OnButton105_SwCardLvShort() //lv iso  //son230302
{	 

	//10-> 46  //self 	//sylee20120509   46, 47


	if(g_nSelfRunFlag>0){
	//    AfxMessageBox("   Running.        Selftest   Switch Card    LV Short Check      Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;


	OnATestRun( __LV,   2,  1);// 46 LV SHORT	//son231222

	 g_nSelfRunFlag=0;

	return ;
}

void CChildView8::OnButton106_SwCardHvOpen() //hv con   //son230302
{

	int  i;
	CString str; 


	if(g_nSelfRunFlag>0){
	  //  AfxMessageBox("   Running.        Selftest   Switch Card  HV Open Check     Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;


	i=m_combo2.GetCurSel();

	if(i<2){
			str.Format("Error Self,  Case 1   \n   " );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
		return;
	}
 
//30-> 47  //self 	//sylee20120509   46, 47
       OnATestRun( __HV,  1,  1);// 47  =200v   HV OPEN     //son231222


	   g_nSelfRunFlag=0;


	return ;
}




void CChildView8::OnButton107_SwCardHvShort() //hv iso  //son230302
{
	int  i;
	CString str; 


	if(g_nSelfRunFlag>0){
	 //  AfxMessageBox("   Running.        Selftest   Switch Card     HV Short  Check    Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;


	i=m_combo2.GetCurSel();

	if(i<2){
			str.Format("Error Self,  Case 1   \n   " );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
		return;
	}
 

//30-> 47  //self 	//sylee20120509   46, 47
	OnATestRun( __HV,  2,  1 );// 47  =200v   HV SHORT      //son231222


	g_nSelfRunFlag=0;


	return ;
}


/*
5 Case 1 CPLD 분할
6 Case 2 VC TR 검사 (VBPIN만등록)
7 Case 3 VB VC TR  동작 검사  CROSS
8 Case 4 (VC PIN만등록)



5 CPLD 분할
6 VC TR 검사VBPIN만등록
7 VB VC TR  동작 검사 CROSS
8 VC PIN만등록

*/


/*


None<Isolation Test Case>


      **>  1 VB VC TR  Test  ->1        ( VB, VC PIN 자신PIN  연결  LV 전용  측정)
      **>  2 VB만 등록  Short Test->3   ( VB PIN 만 등록 측정) 
      **>  3 VC만 등록  Short Test->2   ( VC PIN 만 등록 측정) 




4 Connector Leak Test
5 CPLD 분할
6 VC TR 검사VBPIN만등록
7 VB VC TR  동작 검사 CROSS
8 VC PIN 등록시 영향
9 VC PIN만등록
10   
11   
        **> 12  VB PIN만  Mult   등록  ->4      ( VB PIN 연속 등록 측정 )
        **> 13  VC PIN만  Mult   등록  ->5      ( VC PIN 연속 등록 측정 )
14    
15  VC  PIN 만  Mult등록 2                  
16  VB  VC Pin  Mult등록 
*/





/*
None  < Test Case >
1  VB VC TR  Open Test
2  VB TR Short  Test
3  VC TR Short Test
4  VB All PIN  TR Short  Test
5  VC All PIN  TR Short  Test

*/


void CChildView8::OnButton108_SwCardRelay()  //relay //son230302
{
 
	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        Selftest  4W  Relay Check  Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;
	
	OnATestRun4W( 181 ); //   Case=1 s1,    2=  s2  //sylee130701  46->181
	
	g_nSelfRunFlag=0;

}


 

void CChildView8::OnButton109_SwCardRsCheck()    //vc reset check //son230302
{
 
 	g_nSelfRunFlag=1;  

 	m_combo2.SetCurSel(8);  
 	OnATestRun( __LV,   0,   1);// 46   LV  ,   0= reset check //son231222


 //	m_combo2.SetCurSel(9); //SYLEE130704  VB ONE PIN RESET
 //	OnATestRun( __LV,   0,   1);//SYLEE130704  VB ONE PIN RESET //son231222
 
 
	g_nSelfRunFlag=0;

}

 






//son m_nMode1    1: __LV,    2: __HV
//    nIso1       0: VC Reset Check 1: openTest   2: ShortTest
//    nAuto1      미사용
//son231222 void CChildView8::OnATestRun(int  m_nMode1,  int nIso1, int nAuto1) //hv iso
void CChildView8::OnATestRun(int  nVMode,  int nIso1, int nAuto1) //hv iso //son231222 m_nMode1 -> nVMode 
{		
 
	// hdpark220819 begin
	 SetTimer(10, 300, NULL);
	 SetTimer(20, 300, NULL);
	 SetTimer(30, 300, NULL);
	 SetTimer(40, 300, NULL);
	 SetTimer(50, 300, NULL);
	 SetTimer(60, 300, NULL);
	// hdpark220819 end

	CString   str,str1, strfName,strfName1, str2;
 
    int nFailflag; 
    int tmp, nFlagMode1,nFail1,n4Wire, nRet1;//,m;	 
	int dPinVC[200],dPinVB[200];
	 __int64   tStart1, tStart2 ; 

	FILE  *fp1;
	char  fName[200], fName1[200];
	char szText[50];
	
//	double nT[10];

	double nISet;	
	double nT[10];
	double nData[130][15];
	double dVbR1,dVbR2;

	int Lo1, nLoop,ki,nCase, nFil;

//	int nPin[5];
	int nPinSt[32000];

	int nProCase1, nStop,StepErrCount, PinC,PinB;
	int nIFilter,nHVOffSet,nHVGain,a, nIGain ;	    //sylee150904 //ACE400
	int nLeakRef1; //sylee230822 //son Leak Mohm의 Raw ADC 값. 
	int nLeakRef2; //sylee230822 //son Leak Mohm 
	int nLeakCheckFlag1;//sylee230822  //son 0:Leak 아님 1: Leak test  
	double dR100;

    FileSysInfo01.LoadSaveSet15(_LOAD);//sylee230822
    nLeakRef2=SysSet15.m_nSet7*1000000;//sylee230822 //son Map > Set4:SelfTest Condition > Leak Mohm
	if(nLeakRef2<0){	
	    nLeakRef2=100000000; //sylee230822  //son 100 Mohm
    }
	if(nLeakRef2>1000000000){	
	    nLeakRef2=1000000000; //sylee230822  //son 1000 Mohm
    }


	FileSysInfo01.LoadSaveSer13(_LOAD, 207, PART_MIXED);

    FileSysInfo01.LoadSaveSer13(_LOAD, 327, PART_UPPER);//sylee230822/
    FileSysInfo01.LoadSaveSer13(_LOAD, 327, MAX_PART);//sylee230822

    RCalCovert(327, nLeakRef2, REAL_TO_RAW, 0);//sylee230822
//str.Format("CChildView8::OnATestRun(%d, %d, %d): nLeakRef2=%d, Sub71.R1=%.2f, \n", nVMode, nIso1, nAuto1, nLeakRef2, Sub71.R1);  //son230915 //son231222
//MyTrace(PRT_BASIC, str);
	nLeakRef1=Sub71.R1;//sylee230822  //son Leak Mohm의 Raw ADC 값.
 
 
    Sub71.OnDisCharge1(); //sylee170725-3 ray


	nPinSetSkipVb=0; //sylee151007 //ACE400 	//son230127
	nPinSetSkipVc=0; //sylee151007 //ACE400		//son230127

    //son231227 Get_nCardPinMax();//sylee150903 //ACE400     //son231227 중복호출 삭제


	a=FileSysInfo01.LoadSaveSet15(2);//load   //sylee140911
	if(a!=1){
		AfxMessageBox(" Error No 8212, \n\n  SysSet15.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}

 ///////////////////////////////////////////////////////////////////////////
//변수 초기화 
	::ZeroMemory (&nPinSt, sizeof(nPinSt));
	::ZeroMemory (&nData, sizeof(nData)); 
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));
	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));
	::ZeroMemory (&dSelfErr1, sizeof(dSelfErr1));


    g_nStopFlag=0;
    g_nErrCount1=0;
	g_nErrCountTot=0;

	nStop=0;
	n4Wire=0;
	nFlagMode1=0;
	nFail1=0;	
 

	Sub71.nADCount1=1;//test


    //son nHW_CardPin, nCardPinMax을  SysSet12.m_nPinBoxMax와 SysSet13.m_nPinBlockType 에 맞게 설정
	Get_nCardPinMax(); //sylee150903 //ACE400        //son231227

	//----------------------------
	//son Read m_nCardNoStart 
	m_edit1.GetWindowText( szText,10); 
	m_nCardNoStart = atoi( szText );	


	if( m_nCardNoStart<1){	 
		str.Format("Error Self,  Begin Card < 1 . \n \n  Set Error !     Begin Card=%d,    ",m_nCardNoStart );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;	
	}


    //son 2K box용 64pin card 인 경우
	if(nCardPinMax==64){ //sylee150903-1 //ACE400
		if( m_nCardNoStart>512){	 
			str.Format("Error Self,  Begin Card > 512 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}
	}
	else{
		if( m_nCardNoStart>256){	 
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}
	}

 
	//----------------------------
	//son Read m_nCardNoEnd 

	m_edit2.GetWindowText( szText,10); 
	m_nCardNoEnd = atoi( szText );

	if( m_nCardNoEnd<1){	 
		str.Format("Error Self,  End Card < 1 . \n \n  Set Error !    End Card=%d,    ",m_nCardNoEnd );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;	
	}


	if(nCardPinMax==64){ //sylee150903-1  //ACE400
		if( m_nCardNoEnd>512){	 
			str.Format("Error Self,  End Card > 512 ( 32k).  \n \n Set Error !    End Card=%d,    ",m_nCardNoEnd );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}

	}
	else{

		if( m_nCardNoEnd>256){	 
			str.Format("Error Self,  End Card > 256 ( 32k).  \n \n Set Error !    End Card=%d,    ",m_nCardNoEnd );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}
	}


	if( m_nCardNoEnd<m_nCardNoStart){	 
		str.Format("Error Self,  End Card < Begin Card  . \n \n  Set Error !     Begin Card=%d,  End Card=%d,   ",m_nCardNoStart,m_nCardNoEnd );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;	
	}


	m_nCardNoStart =m_nCardNoStart-1;   //son 0 base로 수정
	m_nCardNoEnd =m_nCardNoEnd-1;
 
	m_edit8.GetWindowText( szText,10); 
	nISOSet1 = atof( szText );  

	nTestCase1=m_combo2.GetCurSel();
    nCase=nTestCase1;

    //son TestMode Range Check  1:  Open TR   
    //                          2:  Short 1 TR
    //                          3:  Short 2 TR 
    //                          4:  Leak 1 TR 
    //                          5:  Leak 2 TR 
    //                          6:  Leak A
    //                          7:  Leak B 
    //                          8:  VC One Pin Reset 
    //                          9:  VB One Pin Reset 
    
	if( nCase==SELF_TEST_VC_1PIN_RST){ //son240130 8:SELF_TEST_VC_1PIN_RST
		if( nIso1!=0){  //son Open이나 Short라면
		        str.Format("Error Self,  VC One Pin Reset Mode button click?  . \n \n  Start Error !     " );
				AfxMessageBox( str, MB_OK); //error messege
				g_nSelfRunFlag=0;
				return ;	 
		}
	}
 
 	if( nCase==SELF_TEST_VB_1PIN_RST){//SYLEE130704 //son240130 9:SELF_TEST_VB_1PIN_RST
		if( nIso1!=0){  //son Open이나 Short라면
		        str.Format("Error Self,  VB One Pin Reset Mode button click?  . \n \n  Start Error !     " );
				AfxMessageBox( str, MB_OK); //error messege
				g_nSelfRunFlag=0;
				return ;	 
		}
	}

    int m_nMode1;       //son231222
    if( nVMode == __LV ){//LV SET   //son231222 m_nMode1 -> nVMode
        //son231222  1: __LV
		m_nMode1= MODE181_SELF_10V;//46;                //son 181: MODE181_SELF_10V
	}


	if( nVMode == __HV ){// HV SET //son231222 2: __HV  //son231222 m_nMode1 -> nVMode
		 //SYLEE230821 m_nMode1=182;  // 250V SELECT 
		  m_nMode1= MODE327_LEAK_250V;  //SYLEE230821  //son 327: MODE327_LEAK_250V
	}


 

	if(nCase>11){
		str.Format("Error Self,  Test Case Selection Error  >11  \n   " );
	    AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
	    return ;	
	}


	if(nCase<0){
	    str.Format("Error Self,  Test Case Selection Error   <0  \n   " );
	    AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;
	}
 
    
	strfName1.Format("\\SETUP\\001SelfTr.CSV" );         //<=======================================
	DeleteFile(strfName1);
	::ZeroMemory(&fName1,sizeof(fName1));
    strcat( fName1 , SysInfoView01.m_pStrFilePath1 );//SYLEE20111212
	strcat( fName1 , strfName1 );
 
	fp1 = fopen(fName1,"wt"); 

	if(fp1 == NULL){ 
		str.Format("Error    001SELFHVISO file open   ?  Error, Saving Error !     file close!   " );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;

		return  ;
	}
 
	nCC= (int)dRec[m_nMode1][1];
	nVRel= (int)dRec[m_nMode1][2];   
	nVSet=  dRec[m_nMode1][3];

	nIR=  (int)dRec[m_nMode1][4];
	nISet= dRec[m_nMode1][5];

	nFil=  (int)dRec[m_nMode1][8];  //    1=VC FAST, 2= VC SLOW ,  3=HV OPEN, ( 4=4W )
	nTimeDelay1=(int)(dRec[m_nMode1][6]*10);//syle20120104     SETUPCALIBRATION

	nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
	nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
	nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629	 

	nIGain = (int)dRec[m_nMode1][12];	    //sylee150904 //ACE400 


	if( m_nMode1==MODE181_SELF_10V){     //son LV case  //son240130 181:MODE181_SELF_10V
		if( nVSet>20){
			AfxMessageBox(" Error No 8101, SelfTest, LV MODE     Vset Error> 20V \n\n     check LV !,   \n \n  Voltage SET=%d ? ",(int)nVSet); 
			g_nSelfRunFlag=0;
			fclose(fp1);
			return;
		}
	}
 
 
    nTimeDelay1=(int) (dRec[m_nMode1][6]*10);//syle20120104     SETUPCALIBRATION
 
     //ADC COUNT
    Sub71.nADCount1 =(int)dRec[m_nMode1][7];
	
	if(Sub71.nADCount1<1 ){
		Sub71.nADCount1=1;
	}

	if( Sub71.nADCount1>100){
		Sub71.nADCount1=100;
	}

	//=========================================================================================
	//interlock


	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//INITAL 부 

  
	nRet1=Sub71.AOnPoweroff2();//sylee20120702 
	if(nRet1!=1){ 
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 8310, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
		g_nSelfRunFlag=0;
		return  ;
	}


	if( nVRel==VR_LV1){      //son240130 1: VR_LV1
		nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
	}
	else  if( nVRel==VR_LV2){ //son240130 2: VR_LV2
		nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
	}
	else  if( nVRel==VR_HV1){ //son240130 3: VR_HV1
		nRet1=Sub71.AOnVSetMulti1(0,0,nVSet,0) ;
	}
	else  if( nVRel==VR_HV2){ //son240130 4: VR_HV2
		nRet1=Sub71.AOnVSetMulti1(0,0,0,nVSet) ;
	} 


	if( nRet1!=1){
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 1316, \n  Voltage   SET ERROR !,   SW TEST Run Start Error!!!"); 
		g_nSelfRunFlag=0;
		return ;
	}

	/* //SYLEE150211 //ACE380   RAY 
	   if(nVRel==3 || nVRel==4){
	   Sub71.Delay_100ns(10000000);  //1 sec  //SYLEE20111214 
	   }
	   else{
	//sylee140722Sub71.Delay_100ns(1000000);  //0.1 sec  //SYLEE20121022
	Sub71.Delay_100ns(1000000);  //0.1 sec  //SYLEE20121022
	}
	*/

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	if( nIGain==2){//sylee150904 //ACE400
		Sub71.I_Gain(2);  //10x  =io on		
	}
	else{
		Sub71.I_Gain(1);  //1x  =io off
	}


	Sub71.Delay_100ns(3000000);  //300ms //sylee150903 //ACE400


	tStart1=GetMilSecond2();

	Lo1=3;




Task101: 
		


	
	Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

	if(nVSet<5){
		dVbR2=nVSet*(0.35);//35%
	}
	else{
		dVbR2=nVSet*(0.1);//10%
	}
	dVbR1=nVSet-dVbR2;
	dVbR2=nVSet+dVbR2;

	if( Sub71.V1>300){
		if(Lo1-->0){
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}
		str.Format("Error No 1312,\n \n\n  Power Check? \n\n\n    Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%f.2, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		g_nSelfRunFlag=0;							  
		//	return ;
	}

	if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){
		if(Lo1-->0){
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}
		str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		//	return ;
	}

	//sylee150210 move	Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);  
	//sylee150210 move   Sub71.Delay_100ns(3000000);  //300ms 


	//		V1old=Sub71.V1;//sylee150210 //ACE380

	if( nFil==1 ||  nFil==2){//SYLEE130628
		Sub71.AOnReadV1Enable1(nVRel); 
	}
	else{
		Sub71.AOnReadV1Enable1(5);
	}
	if(nFil==3){ 
		Sub71.HV_OffSet_Gain(nHVOffSet, nHVGain);    //sylee140226
	}


	//	    Sub71.V1=V1old;//sylee150210 //ACE380


	if( n4Wire==4 ){ //interlock //sylee130701
		nFil=4; 
	} 

	//son240130 if( m_nMode1>=16 && m_nMode1<=155) //HVOPENSET  //SYLEE130618
	if( m_nMode1>=MODE16_OPEN_30V && m_nMode1<=MODE155_OPEN_300V) //HVOPENSET  //SYLEE130618   //son240130
    {
		nFil=3;//sylee130628
	}

	Sub71.FilterSet(nFil);//sylee130628 
	if(nIFilter==3){
		AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
		nIFilter=1;
	}
	//sylee140722   Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);
	//sylee140722	Sub71.Delay_100ns(500000);  
	Sub71.I_Filter(nIFilter);//sylee130629 

	Sub71.OnPinAllReset1();//sylee150210 

	Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);  	//sylee150210 move	

	Sub71.Delay_100ns(3000000);  //300ms  //sylee150210 move  

	//SYLEE150211 //ACE380  RAY		if( nCase==SELF_TEST_OPEN  ){ 	    //son240130 1:SELF_TEST_OPEN
	//SYLEE150211 //ACE380  RAY		    Sub71.Delay_100ns(10000000);  //1s //sylee140722
	//SYLEE150211 //ACE380  RAY		}



Task100://VB ONE PIN RESET



	Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118	     

	tStart2=GetMilSecond2();	 
	nT[0]=(double)(tStart2-tStart1)/1000.;  //test

#if 1   //son240205 begin:
	if(	nCase>=SELF_TEST_OPEN && nCase <= SELF_TEST_VB_1PIN_RST){ //son 1:SELF_TEST_OPEN,  9:SELF_TEST_VB_1PIN_RST 
		str.Format("  * [Self Test START :: Case %d] %s %s  Check.", 
		                        nCase, getStrSelfTestCase(nCase), 
		                        (nCase >=SELF_TEST_OPEN && nCase <= SELF_TEST_LEAK2)? "TR": "" );
    }
#endif  //son240205 end.


    else {
		str.Format("  *  ??  else  Check");	
		// return ;
	}


	OnList1Mes1(str);

	nLeakCheckFlag1=0;//sylee230822
	if( nCase==SELF_TEST_LEAK1 || nCase==SELF_TEST_LEAK2            //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
	        || nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B ){//sylee230822 //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
		nLeakCheckFlag1=1;//sylee230822
	}


	int nCardTo1;

	nCardTo1=m_nCardNoEnd-m_nCardNoStart+1;
	if( m_nMode1==MODE181_SELF_10V && nIso1==1){ //son SelfTest Open이면    //son240130
		nProCase1=1;
	}
    else  if( m_nMode1==MODE181_SELF_10V && nIso1==2){   //son SelfTest Short이면   //son240130
		nProCase1=2;
	}
    //SYLEE230821   else  if( m_nMode1==182 && nIso1==1){
    else  if( m_nMode1==MODE327_LEAK_250V && nIso1==1){//SYLEE230821  //son240130 
		nProCase1=3;		
	}
    //SYLEE230821  else  if( m_nMode1==182 && nIso1==2){
	else  if( m_nMode1==MODE327_LEAK_250V && nIso1==2){ //SYLEE230821 //son240130 
		nProCase1=4;
	}
    else  if( m_nMode1==MODE181_SELF_10V && nIso1==0){//sylee121022 //son240130 
		if( nCase==SELF_TEST_VC_1PIN_RST || nCase==SELF_TEST_VB_1PIN_RST ){ //SYLEE130704   //son240130
			nProCase1=6;
		}
	} 
 
	int  nCo1;

	nCo1=0;


	StepErrCount=0;


    int card;

	for( card = m_nCardNoStart; card <= m_nCardNoEnd; card++ )  //card no   //son 0부터 시작
	{


		nCo1++;
		//son240130 if( nCase>3  &&   nCase<8  )// HW INTERLOCK  
		if( nCase>=SELF_TEST_LEAK1  &&   nCase <= SELF_TEST_LEAK_B)//son240130 4:SELF_TEST_LEAK1, 7:SELF_TEST_LEAK_B
        {
			//sylee120917 if(nCo1>32){//32ea switch card  4k  //sylee120613
			if(nCo1>8){//32ea switch card  4k  //sylee120613//sylee120917
				nCo1=0;
				Self_OnPinAllReset1(); ///PIN ALL RESET    //son240608
				//sylee120903 Sub71.Delay_100ns(10000);  //1000us 
				Sub71.Delay_100ns(10000000);  //1s //sylee120903 
			}
		}

#ifdef _PC_TEST_    //son240608
		nLoop=1; 
#else
		nLoop=5; 
#endif
		nFail1=0;	  


		//==================

		if( nProCase1==1){
			nProgress1Rate= (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
			nProgress2Rate=0;
			nProgress3Rate=0;
			nProgress4Rate=0;
			nProgress5Rate=0;
			nProgress6Rate=0;

		}
		else if( nProCase1==2){
			nProgress2Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
			nProgress1Rate=0; 
			nProgress3Rate=0;
			nProgress4Rate=0;
			nProgress5Rate=0;
			nProgress6Rate=0;

		}
		else if( nProCase1==3){
			nProgress3Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
			nProgress1Rate=0;
			nProgress2Rate=0; 
			nProgress4Rate=0;
			nProgress5Rate=0;
			nProgress6Rate=0;

		}
		else if( nProCase1==4){
			nProgress4Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
			nProgress1Rate=0;
			nProgress2Rate=0;
			nProgress3Rate=0;
			nProgress5Rate=0;
			nProgress6Rate=0;

		}
		else if( nProCase1==5){
			nProgress5Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
			nProgress1Rate=0;
			nProgress2Rate=0;
			nProgress3Rate=0;
			nProgress4Rate=0;
			nProgress6Rate=0;

		}
		else if( nProCase1==6){
			nProgress6Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
			nProgress1Rate=0;
			nProgress2Rate=0;
			nProgress3Rate=0;
			nProgress4Rate=0;
			nProgress5Rate=0;
		}

		//================

		int TaskRetest1;//sylee140619
		TaskRetest1=0;//sylee140619

		BOOL bVBIdentity = FALSE;		// hdpark220819
		BOOL bVCIdentity = FALSE;		// hdpark220819

    	int cardPin;
		for( cardPin=0; cardPin < nCardPinMax; cardPin++)  //	nCardPinMax=64;//sylee150210 //ACE380
		{

Task1://sylee20120308


			DoEvents(); 
			if( g_nStopFlag==2){
				g_nSelfRunFlag=0;
				goto task2;
			} 

			if( nStop==2){
				AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
				goto task2; 		 
			}

            //son Leak1, Leak2, LeakA, LeakB는 PinAllReset을 하지 않고 이전 VB, VC를 계속 쌓아가면서 테스트한다.
            //    Open, Short1, Short2는  다음번 핀 진행 시에 앞의 핀연결은 끈다. 
			if(  nCase !=SELF_TEST_LEAK1  &&  nCase !=SELF_TEST_LEAK2               //son240130 4:SELF_TEST_LEAK1,  5:SELF_TEST_LEAK2
			        &&  nCase !=SELF_TEST_LEAK_A   &&  nCase !=SELF_TEST_LEAK_B  )  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
			{  
				Self_OnPinAllReset1(); ///PIN ALL RESET //son240608
			}


			ki=0;
			//Task5:
			////////////////////////////////////////////////////////////////

            int nPin = GetHwPinNo_Old(card, cardPin);     //son231222_2 GetHwPinNo_Old() 함수호출로 코드 간소화  //son240125_2
            PinB = nPin;
            PinC = nPin;

            //---------------
			// Open SelfTest
			//   : VB  VC  1대1 동작 검사 //  LV 전용 
			
			//son VB, VC 같은 번호의 pin을 연결해서 Open Self Test.  
			//    연결해서 전류가 잘 통하면 저항이 기준이하이므로 정상, 저항기준 이상이면 Open
            //         testPin   1   2   3   4   5   6 
            //          VB1      o   x   x   x   x   x      (연결 o, 미연결 x)
            //          VC1      o   x   x   x   x   x
			if( nCase==SELF_TEST_OPEN ){     //son240130 1: SELF_TEST_OPEN
				Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  //  2 VC,  //son210220 enum으로 변경  //son240608
				Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  // 1,VB, 	//son210220 enum으로 변경 //son240608

			}

            //------------------------------
			// short1,    pin one
		    //	
            //son VB, VC 같은 번호의 pin을 하나만 연결해서 Short1 Self Test  
            //    하나만 연결했는데 전류가 통한다면 short 인 것임. 기준 이하 저항이면 short. 
            //    Short1 :  VB TR 연결,     VC TR 미연결 상태에서 전류를 흘려 VC test
            //    Short2 :  VB TR 미연결,   VC TR 연결 상태에서 전류를 흘려 VB test
            //    Short1, Short2는  다음번 핀 진행 시에 앞의 핀연결은 끈다. 
            //    ex) Short1 테스트 진행
            //    VBpin  ＼test     1   2   3   4   5   6 
            //       1              o   x   x   x   x   x   (연결 o, 미연결 x)
            //       2              x   o   x   x   x   x
            //       3              x   x   o   x   x   x  			
			else  if( nCase==SELF_TEST_SHORT1 ){   //son240130 2: SELF_TEST_SHORT1
				Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  // =1 VB,            //son210220 enum으로 변경  //son240608
			}

            //-------------------------
			// short2, pin one
			//
            //son VB, VC 같은 번호의 pin을 하나만 연결해서 Short2 Self Test  
            //    하나만 연결했는데 전류가 통한다면 short 인 것임. 기준 이하 저항이면 short. 
            //    Short2 :  VB TR 미연결,   VC TR 연결 상태에서 전류를 흘려 VB test
			else  if( nCase==SELF_TEST_SHORT2 ){   //son240130 3: SELF_TEST_SHORT2
				Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  // =2 VC,  pin one   //son210220 enum으로 변경  //son240608
			}


            //------------------------------
			// leak1,    pin multi 
		    //	
            //son VB, VC 같은 번호의 pin을 하나만 연결해서 Leak1 Self Test 
            //    하나만 연결했는데 전류가 통한다면 short 인 것임. 기준 이하 저항이면 short.
            //    
            //    Leak1 :  VB TR 연결,     VC TR 미연결 상태에서 전류를 흘려 VC test
            //    Leak2 :  VB TR 미연결,   VC TR 연결 상태에서 전류를 흘려 VB test
            //    Leak1, Leak2는  다음번 핀 진행 시에 앞의 핀연결을 유지하고 쌓아가면서 테스트한다.  
            //    적은 전류가 점점 쌓여서 leak가 되는 상황을 만들기 위함임.
            //    ex) Leak1 테스트 진행
            //    VBpin  ＼test     1   2   3   4   5   6 
            //       1              o   x   x   x   x   x  (연결 o, 미연결 x)
            //       2              o   o   x   x   x   x
            //       3              o   o   o   x   x   x  
			else  if( nCase==SELF_TEST_LEAK1 ){   //son240130 4: SELF_TEST_LEAK1 
				Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  // =1 VB,		    //son210220 enum으로 변경  //son240608
			}

            //------------------------------
			// leak2   pin multi
            //    Leak2 :  VB TR 미연결,   VC TR 연결 상태에서 전류를 흘려 VB test
			else  if( nCase==SELF_TEST_LEAK2 ){   //son240130 5: SELF_TEST_LEAK2
				Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  // =2 VC,            //son210220 enum으로 변경  //son240608
			}


            //------------
			//leak A 
            //son  VB 1, VC 2번 이런식으로 근접한  pin을 대상으로 LeakA Self Test 
            //      기준 이하 저항이면 short.
            //     ex) LeakA 테스트 진행
            //       testPin   1   2   3   4   5   6  
            //        VB       1   2   3   4   5   6
            //        VC       2   3   4   5   6   7
			else  if( nCase==SELF_TEST_LEAK_A )    //son240130 6: SELF_TEST_LEAK_A
			{  
// hdpark220819 begin
//				nStep1=cardPin%2;
//				if (nStep1 == 0) {
//					Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  //    =2Vb,      //son210220 enum으로 변경  //son240608
//				}
//				else {
//					Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  //    =2Vc,      //son210220 enum으로 변경  //son240608
//				}

				if(!bVBIdentity){
					Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  //    =2Vb,      //son210220 enum으로 변경  //son240608
					bVBIdentity = TRUE;
					continue;   //son 이 지점에서 continue해서 바로 옆 cardPin을 VC로 만들 수 있다. 
				}
				else {
					Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  //    =2Vc,      //son210220 enum으로 변경  //son240608
					bVBIdentity = FALSE;
					if(cardPin < nCardPinMax - 1)	
					    cardPin--;					// VB, VC 연속 Pin으로 등록하기 위해.. 기존은 1,3,5,.. -> 변경 1,2,3,4,5,...
				}
// hdpark220819 end
			}

            //------------
			//LEAK B			
            //son  VB 1, VC 2번 이런식으로 근접한  pin을 대상으로 LeakB Self Test. 기준 이하 저항이면 short.
            //     ex) LeakB 테스트 진행
            //       testPin   1   2   3   4   5   6  
            //        VB       2   3   4   5   6   7
            //        VC       1   2   3   4   5   6		
			else  if( nCase==SELF_TEST_LEAK_B  )     //son240130 7: SELF_TEST_LEAK_B
            {
// hdpark220819 begin
//				nStep1 = cardPin % 2;
//				if (nStep1 == 0) {
//					Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  //    =2Vc,      //son210220 enum으로 변경  //son240608
//				}
//				else {
//					Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  //    =2Vb,		//son210220 enum으로 변경  //son240608
//				}

				if(!bVCIdentity){
					Self_AOnPinSet1( PinC, MOD_TR_SET_VC, cardPin);  //    =2Vc,      //son210220 enum으로 변경  //son240608
					bVCIdentity = TRUE;
					continue;   //son 이 지점에서 continue해서 바로 옆 cardPin을 VB로 만들 수 있다.
				}
				else {
					Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);  //    =2Vb,		//son210220 enum으로 변경  //son240608
					bVCIdentity = FALSE;
					if(cardPin < nCardPinMax - 1)	
					    cardPin--;					// VB, VC 연속 Pin으로 등록하기 위해.. 기존은 1,3,5,.. -> 변경 1,2,3,4,5,...
				}				


			}
			//---------------------------------
			// CASE8   ONE pin  VC  RESET CHECK	 				  
			else  if( nCase==SELF_TEST_VC_1PIN_RST )  //son240130 8: SELF_TEST_VC_1PIN_RST  
            {
				Sub71.Delay_100ns(100);  //10us 
				Self_AOnPinSet1(PinB, MOD_TR_SET_VC, cardPin); //son210220 enum으로 변경  //son240608

				//Sub71.AOnPinReSet1(1,PinB, 2); //   vc reset     nMod =1,VB,    =2VC,
				Sub71.AOnPinReSet1(1,PinB, MOD_TR_SET_VC); //   vc reset     nMod =1,VB,    =2VC, //son210220 enum으로 변경
				Sub71.Delay_100ns(100);  //10us  

				Self_AOnPinSet1( PinB, MOD_TR_SET_VB, cardPin);   //son210220 enum으로 변경  //son240608

			}
			
			//---------------------------------
			// CASE9   ONE pin  VB  RESET CHECK	  
			else  if( nCase==SELF_TEST_VB_1PIN_RST  )  //SYLEE130704//son240130 9: 	SELF_TEST_VB_1PIN_RST
            {
				Sub71.Delay_100ns(100);  //10us 
				Self_AOnPinSet1(PinB, MOD_TR_SET_VB, cardPin); //son210220 enum으로 변경  //son240608

				//Sub71.AOnPinReSet1(1,PinB, 1); //   vc reset     nMod =1,VB,    =2VC,
				Sub71.AOnPinReSet1(1,PinB, MOD_TR_SET_VB); //   vc reset     nMod =1,VB,    =2VC, //son210220 enum으로 변경

				Sub71.Delay_100ns(100);  //10us  
				Self_AOnPinSet1( PinB, MOD_TR_SET_VC, cardPin);   //son210220 enum으로 변경  //son240608

 
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////


			double InterI; //sylee20120612



            if( nIR==1 || nIR==2 ){  //mA			 
                InterI=nISet*0.9;				 
            }
            else  if( nIR==3 || nIR==4 ){   // uA				 
                InterI=nISet*0.9;				 
            }
            else  if(nIR==5){//uA
                InterI=	10.0;
            }

            int nRet1;

#if 0	//son240202 LeakA 사전측정시에는  Sub71.AOnReadV1I1(nVRel)호출하고 Sub71.Y1으로 판정후 jump했는데 
		//          실제 에러는 Sub71.AOnReadV1I1_ADC_I() 기준 코드(Sub71.R1)로 판정되어 leak가 검출 안되는 
		//          문제발생.  김영필이사님 의견으로 사전체크기능은 필요없는 것으로 판단하고 삭제함. 

            //son short나 Leak인 경우, I 값만으로 error 사전측정 하고  메시지박스 출력.
			//if( nCase!=1 && nCase!=8 && nCase!=9 && nCase!=10 && nCase!=11 )
			if( nCase!=SELF_TEST_OPEN && nCase!=SELF_TEST_VC_1PIN_RST && nCase!=SELF_TEST_VB_1PIN_RST 
			                    && nCase!=10 && nCase!=11 )
			{ 
				Sub71.AOnReadV1I1(nVRel); 
				if( Sub71.Y1> InterI) //sylee121101
                {
					Sub71.Delay_100ns(2000);  //200uS	//sylee121101			   
					Sub71.AOnReadV1I1(nVRel); //sylee121101
					if( Sub71.Y1> InterI){ 
						Self_OnPinAllReset1(); ///PIN ALL RESET  //son240608
						//  Sub71.Delay_100ns(10000);  //1ms  relay
						Sub71.Delay_100ns(100);  //10us 
						str.Format("Error No 8201,\n\n     short or leak    \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
						        card+1,cardPin+1,Sub71.Y1,InterI); 
						nRet1=AfxMessageBox(str,MB_YESNO | MB_ICONEXCLAMATION);
                        
                        //son Yes 버튼이 아니라면 다음번 pin에서 루프를 벗어난다. 
						if(nRet1!=6){
							g_nStopFlag=2;								  							    
						}
						
						//son240202 goto TaskInterR101;	//son 측정 직후로 jump해서 실제 측정은  skip.<= 문제 있는 코드

					    //son240202 TaskInterR101_prev하면 LeakA 측정시에 Sub71.AOnReadV1I1(nVRel) 읽었을 때는 
                        //          Sub71.Y1값이 읽히는데 Sub71.AOnReadV1I1_ADC_I()로 읽으면 Sub71.R1은 안 읽히는 문제가 있음.
						//          에러가 있는 핀을 연속으로 LeakA 두번 측정하는 데에 무리가 있는 것인지? 
						//          이 기능이 잘 되지 않아서 사전체크기능을 아예 빼는 것으로 결정함. 
						//goto TaskInterR101_prev;	//son240202 측정 직전으로  jump해서 delay만 skip. 
						                            // Leak 일때 사전측정과 실제측정의 판정기준과 측정함수가 달라서 err 판단오류가 생기는문제 보완.
					}
				}
			}
#endif //son240202


			///////////////////////////////////////////////////////////////////////////////////////////////////////// 					


//sylee230912 if(SysSet15.m_nSet6==7)//sylee140911  //test ray
//sylee230912 {
                int nTime1;

                if( nCase==SELF_TEST_OPEN){     //son240130 1: SELF_TEST_OPEN               
                    nTime1=SysSet15.m_nSet2*10;
                    if(nTime1<0){
                        nTime1=0;
                    }
                    Sub71.Delay_100ns(nTime1);  
                }
                else if( nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_SHORT2 ){  //son240130 2: SELF_TEST_SHORT1, 
                    nTime1=SysSet15.m_nSet3*10;                                 //          3: SELF_TEST_SHORT2 
                    if(nTime1<0){
                        nTime1=0;
                    }
                    Sub71.Delay_100ns(nTime1);  
                }
                else if( nCase==SELF_TEST_LEAK1 || nCase==SELF_TEST_LEAK2 ){    //son240130 4: SELF_TEST_LEAK1
                    nTime1=SysSet15.m_nSet4*10;                                 //          5: SELF_TEST_LEAK2
                    if(nTime1<0){
                        nTime1=0;
                    }
                    Sub71.Delay_100ns(nTime1);  
                }
                else if( nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B ){  //son240130 6: SELF_TEST_LEAK_A
                    nTime1=SysSet15.m_nSet5*10;                                 //          7: SELF_TEST_LEAK_B
                    if(nTime1<0){
                        nTime1=0;
                    }
                    Sub71.Delay_100ns(nTime1);  
                }
                else if( nCase==SELF_TEST_VC_1PIN_RST  ){   //son240130 8:  SELF_TEST_VC_1PIN_RST
                    Sub71.Delay_100ns(2500);   
                }
                else if( nCase==SELF_TEST_VB_1PIN_RST  ){  
                    Sub71.Delay_100ns(2500);    //son240130 9: SELF_TEST_VB_1PIN_RST 
                }
                else{
                    Sub71.Delay_100ns(5000);   
                }

 	/*//sylee230912
	        }
	        else
	        {
				if( nCase==1){
					Sub71.Delay_100ns(3500);  //350uS  sylee120917  100->200us 	//32k fail  VC 2.8V   12/8/17
				}else if( nCase==8  ){  //sylee121022
					Sub71.Delay_100ns(2500);  //250uS
				}else if( nCase==9  ){ //SYLEE130704
					Sub71.Delay_100ns(2500);  //250uS//SYLEE130704
				}else{
					Sub71.Delay_100ns(5000);  //150uS	//SYLEE121204
				}
			}
	*/ //sylee230912


//TaskInterR101_prev:  //son240202 측정 직전 위치 

            //-------------------
            //R  READING : 측정

			// CASE8   ONE pin  VC  RESET CHECK	 //SYLEE130704
			if( nCase==SELF_TEST_VC_1PIN_RST ||  nCase==SELF_TEST_VB_1PIN_RST){ //son240130 8: SELF_TEST_VC_1PIN_RST,  
				Sub71.AOnReadV1I1_ADC_I();                                      //          9: SELF_TEST_VB_1PIN_RST
			}
			else
			{
				if( n4Wire==4 ){ //SYLEE20111118
					Sub71.AOnReadV1I1(5); // READ  4W V  , I 
				}
				else
				{
					if(	nLeakCheckFlag1==1){//sylee230822
					    Sub71.AOnReadV1I1_ADC_I(); //sylee230822
					}
					else{
						Sub71.AOnReadV1I1(nVRel);//SYLEE20111115 
					}
				}

				//=============================================


//son240202 TaskInterR101:  //son  측정 직후 위치

				if(Sub71.Y1<0.0000001){
					Sub71.Y1=0.0000001;
				}
			 

			    if(	nLeakCheckFlag1==0 ) {//sylee230822
					if( nIR==1 ||  nIR==2 ||	nIR==3 ||  nIR==4 ){ //sylee150909 	 //ace400			 
						Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
						//sylee150909 //ace400	else if( nIR==3 ||  nIR==4  ||  nIR==5 )
					}
					else if( nIR==4 ){ //sylee150909 	 //ace400	
						Sub71.Y1=Sub71.Y1*1000.; //sylee150909 	 //ace400
					}
					else if(   nIR==5 ){//sylee150909 //ace400   
						Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
					} 					
				}
			 

			}


			////////////////////////////////////////////////////////////////////////////////////////
			if( n4Wire==4 ){
				Sub71.R1=Sub71.R1*1000;
			}
#if 0   //son240202 Sub71.Y1 = dIADC1 / 100.0; 하고 나서  LV Open 일때에는 복원하지 않는 문제가 있음. 
        //   사용효과에 비해 코드 복잡도가 높아셔저 버그를 유발하므로  short, leak, open 모두 /100 하지 않기로 한다.
			// hdpark221005	begin
			Sub71.Y1 = dIADC1 / 100.0;	// 데이터 분별력을 위해 100 으로 나눔, 판별 할때 다시 복원시켜야.   
			                            //son231012 int nIADC1  -> double dIADC1
#endif			                            
			                            
			//if (nCase != 1)
			//{
			//	//son221014 Sub71.R1 = RCalConvert_spark(nJudgeMode, Sub71.Y1, RAW_TO_REAL, PART_UPPER); // 사용 안함
			//	//Sub71.R1 = RCalCovert(nJudgeMode, Sub71.Y1, RAW_TO_REAL, PART_UPPER); //사용 안함 //son221014
			//}
			// hdpark221005 end

            //son Leak가 아니면
			if(  nCase!=SELF_TEST_LEAK1   &&  nCase!=SELF_TEST_LEAK2            //son240130 4:SELF_TEST_LEAK1,  5:SELF_TEST_LEAK2 
			        &&  nCase!=SELF_TEST_LEAK_A  &&  nCase!=SELF_TEST_LEAK_B )  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B 
			{ 
				nRet1=Self_OnPinAllReset1(); ///PIN ALL RESET //son240608
				//sylee130906        Sub71.OnDisChargeuS(100);// sylee20120621  1->10	

				if( nRet1!=1){
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
					g_nSelfRunFlag=0;
					return  ;
				}
			}


			if( g_nStopFlag!=2){
				if( card==m_nCardNoStart){
					if( nLoop >0){  //sylee20120308
						if(nLoop==1){ 
							cardPin=0;						 
						}
						nLoop--;
						goto Task1; 	
					}
				}
			}


			if(cardPin==0 && TaskRetest1==0){//sylee140619
                //son Leak이면 ----------
				//son240130 if( nCase>3 &&  nCase<8 )//sylee140624
				if( nCase>=SELF_TEST_LEAK1 &&  nCase<=SELF_TEST_LEAK_B ) //son240130 4: SELF_TEST_LEAK1, 7:SELF_TEST_LEAK_B
                {
					if((card==m_nCardNoStart) || (nCo1==0)){ 
						TaskRetest1=1;
						goto Task1;
					}
				}
			}


			if(nIso1==2)  //short
            {
				if(nLeakCheckFlag1==0)
                {

#if 0   //son240202 /100했다가 *100 으로 복원하는 기능 사용하지 않기로 함.
				   dR100=(Sub71.Y1 * 100);//sylee230822  // hdpark221005 /100 복원위해 *100 해줌
#else 
                   dR100= Sub71.Y1; //son240202
                   //son 이 위치에서의 nISOSet1은 'Refer' m_edit8 에 설정된 값.
#endif
				}
				else{
					//son240202 dR100=Sub71.R1;
					dR100= dIADC1; //son240202 Leak에서 사용하는 Sub71.AOnReadV1I1_ADC_I() 에서 
					                // Sub71.R1 값을 dIADC1으로 리턴하므로 명시적으로 dIADC1으로 변경한다. 

					//son Map > Set4:SelfTest Condition > 'Leak Mohm' 값으로 판정기준값을 변경.
				    nISOSet1=nLeakRef1;//sylee230822

				}

			   //sylee230822 	if( (Sub71.Y1 * 100) > nISOSet1 )//fail log  0-> nISOSet1 // hdpark221005 /100 복원위해 *100 해줌
   
			    if( (dR100 ) > nISOSet1 )//sylee230822
                {
					if(g_nErrCount1<MAX_FAIL_COUNT){//sylee160514-2	//hdpark221005 2999->MAX_FAIL_COUNT
						g_nErrCount1++;
						g_nErrCountTot++;
						StepErrCount++;
					}
					else{
						break;
					}

					dSelfErr1[g_nErrCount1][0]=(float)card +1;  //  CARD no.     //son240608

#if  0 //son240608
                    //son VB TR 미연결, VC TR 연결 상태라면
					if( nCase==SELF_TEST_SHORT2 || nCase==SELF_TEST_LEAK2 ){ //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2	
					}
					else{
					    //son VB 핀 기록
						dSelfErr1[g_nErrCount1][1]=(float)(cardPin +1);  //vb  pin  //sylee151007 //ACE400 // hdpark220819
					}	

					//vc  pin
					if(nCase==SELF_TEST_OPEN){   //son240130  1: SELF_TEST_OPEN
						dSelfErr1[g_nErrCount1][2]=(float)( cardPin+1 ) ;
					}
					else if(nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_LEAK1  ){    //son240130 2: SELF_TEST_SHORT1, 4:SELF_TEST_LEAK1
						dSelfErr1[g_nErrCount1][2]=-1; //son VB만 연결이므로 VC pin을 -1로
					}
					else{
					    //son Short, Leak VC 핀 기록
						dSelfErr1[g_nErrCount1][2]= (float)(cardPin + 1);  //	//sylee151007 //ACE400 // hdpark220819
					}
#else //son240608
                    //son Self_AOnPinSet1(nPin, nMode, cardPin) 설정할 때 write한 VB cardPin, VC cardPin 값으로 error 로그한다. 
                    dSelfErr1[g_nErrCount1][1] = m_nCardPinVB +1;    //son240608 
                    dSelfErr1[g_nErrCount1][2] = m_nCardPinVC +1;    //son240608 

#endif //son240608

					dSelfErr1[g_nErrCount1][3]=(float)Sub71.V1;  //vb
					dSelfErr1[g_nErrCount1][4]=(float)Sub71.X1;  //vc
					dSelfErr1[g_nErrCount1][5]=(float)Sub71.Y1;   //cardPin
					dSelfErr1[g_nErrCount1][6]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3];

					nFail1++;		
				}


			}
			else if( nIso1==1) //(nIso1==1  //open   //sylee121022
			{
				nFailflag=0;

				if(nCase==SELF_TEST_OPEN){       //son240130  1: SELF_TEST_OPEN
				    //son Sub71.AOnReadV1I1(nVRel)로 읽은 Sub71.Y1 전류값이 'Refer' editbox에 설정된값보다 작으면
					if( (Sub71.Y1< nISOSet1) || (Sub71.X1<8)  ){ 
						nFailflag=1;
					}

				}
				else{
					if( Sub71.Y1< nISOSet1){ 
						nFailflag=1;
					}
				}

				if( nFailflag==1) //sylee20120512
                {
					if (g_nErrCount1 < MAX_FAIL_COUNT)//sylee160514-2. //hdpark221005 2999->MAX_FAIL_COUNT
                    {
						g_nErrCount1++;
						StepErrCount++;
					}
					else{
						break;
					}

					dSelfErr1[g_nErrCount1][0]=(float)card +1;  //  CARD no.    //son240608

#if 0   //son240608 안 쓰는 케이스로 판단되어 삭제
					if( nCase==9 || nCase==10 || nCase==11 ){
						dSelfErr1[g_nErrCount1][1]=(float)PinB +1;  //vb  pin   //son240608
						dSelfErr1[g_nErrCount1][2]=(float)PinC +1;  //vb  pin   //son240608
					}
					else
#endif
					{

						dSelfErr1[g_nErrCount1][1]=(float)(cardPin +1); // hdpark220819    //son240608
						dSelfErr1[g_nErrCount1][2]=(float)(cardPin +1); // hdpark220819    //son240608
					}


					dSelfErr1[g_nErrCount1][3]=(float)Sub71.V1;  //vb
					dSelfErr1[g_nErrCount1][4]=(float)Sub71.X1;  //vc
					dSelfErr1[g_nErrCount1][5]=(float)Sub71.Y1;   //cardPin
					dSelfErr1[g_nErrCount1][6]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3];

					nFail1++;


				}//sylee20120512


			}
			else{//sylee121022   if( nIso1==0) 

				//son240130 8:SELF_TEST_VC_1PIN_RST 9:SELF_TEST_VB_1PIN_RST
				if(nCase==SELF_TEST_VC_1PIN_RST || nCase==SELF_TEST_VB_1PIN_RST ){//SYLEE130704 
					if( Sub71.R1 > 500 ){
						g_nErrCount1++;
						dSelfErr1[g_nErrCount1][0]=(float)PinB;  //vb  pin	  
						dSelfErr1[g_nErrCount1][1]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3];
						nFail1++;
					}
				}
			}


			if(cardPin==(nCardPinMax-1)){ 	//nCardPinMax=64;//sylee150210 //ACE380
				if(nFail1>0){
					str.Format("  ,*  Error =>   ,Switch Card=%d   , Fail=%d    ",  card+1 , nFail1);	
					//SYLEE130704}
				OnList1Mes1(str);  

				}
			}     

			nRet1=0;

			// hdpark220819 begin
			Self_OnPinAllReset1(); //son240608
			Sub71.Delay_100ns(1000);
			// hdpark220819 end


			if (nCase==SELF_TEST_LEAK1 || nCase==SELF_TEST_LEAK2           //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
			        || nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B) //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
			{ 
				if (Sub71.Y1 > 500)
                {
					Self_OnPinAllReset1(); ///PIN ALL RESET //son240608
					str.Format("Error No 1029,\n \n\n   *Leak Check  Error*\n\n  I=%.2f >500uA,\n\n  Card=%d,Pin no=%d,Pin totle no=%d,  \n\n\n\n  *YES=RUN,  *NO=STOP",  Sub71.Y1, card+1,cardPin+1, cardPin+(card)*nCardPinMax    );  //	nCardPinMax=64;//sylee150210 //ACE380
					//AfxMessageBox(str);
					nRet1=AfxMessageBox(str,MB_YESNO | MB_ICONEXCLAMATION);
					if(nRet1==6){

					}
					else{
						cardPin=129;
						card=300;
					}
				}
			}


			if( g_nStopFlag==2){
				cardPin=1000;
				card=1000;
			}


		}////for END   for( cardPin=0; cardPin<64; cardPin++)  //pin  vb pin



	} //for END  for( card=0; card< nCount; card++)  //count

 
#if 1   //son240205 begin:
	if(	nCase>=SELF_TEST_OPEN && nCase <= SELF_TEST_VB_1PIN_RST){   //son 1:SELF_TEST_OPEN,  9:SELF_TEST_VB_1PIN_RST

		str.Format("  * [END :: Case %d] %s", nCase, getStrSelfTestCase(nCase)); 
    }
#endif //son240205 end.
    else {  
		str.Format("  * [ END :: else Case?     ");	

	}

	if( StepErrCount==0 ){
		str2.Format("  ***> PASS *");
	}
    else {
		str2.Format("  ***>  FAIL *");
	}
	str=str+str2;


	OnList1Mes1(str);
 
task2:  //STOP JUMP POSTION

					 
	Sub71.OnPinAllReset1(); ///PIN ALL RESET 

	if( g_nStopFlag==2){
		g_nSelfRunFlag=0;
		g_nStopFlag=0;
		str.Format("  * Stop   [  ::  Switch Card  TR ]  ");	
		OnList1Mes1(str);
		AfxMessageBox(str); 		 	 
	} 

	///////////////////////////////////////////////////////////////////////
	// FILE LOGGGING

	fprintf(fp1, "* SelfTest  ::  Switch Card 2W Pin Check     \n  "  );  //<=======================================

	if( nCase==SELF_TEST_VC_1PIN_RST ){     //son240130 8: SELF_TEST_VC_1PIN_RST
		fprintf(fp1, " Case8,  No  ,Switch Card       ,SW Pin      ,SW VC Pin Total     ,    I,   \n ");	
	}
	else  if( nCase==SELF_TEST_VB_1PIN_RST ){//SYLEE130704      //son240130 9: SELF_TEST_VB_1PIN_RST
		fprintf(fp1, " Case9,  No  ,Switch Card       ,SW Pin      ,SW VB Pin Total     ,    I,   \n ");	
	}
	else {   
			if(nLeakCheckFlag1==1){	//sylee230822
		       fprintf(fp1, " No, *CardNo,SW VbPin, SW VcPin,SW TotalVbPin,SW TotalVcPin,    VB,       VC,        I,            ,  ADC, *, ohm     ,  \n ");	
			}else{
			   fprintf(fp1, " No, *CardNo,SW VbPin, SW VcPin,SW TotalVbPin,SW TotalVcPin,    VB,       VC,        I,            ,   ,   \n "); 
			}
	}  

	int dd;

	int err;
	for( err=1; err<=g_nErrCount1; err++)//sylee20111215
	{

		dd=err%2;

		if (nCase==SELF_TEST_OPEN)	//son240130 1: SELF_TEST_OPEN		 
        {
			fprintf(fp1, "  %d,     *%5.0f,      %5.0f,     %5.0f,      %5.0f,     %5.0f,", 
				err, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2],     //son err, card, VB cardPin , VC cardPin
				dSelfErr1[err][1] +(int)(dSelfErr1[err][0] -1) * nCardPinMax,   //son240608
				dSelfErr1[err][2] +(int)(dSelfErr1[err][0] -1) * nCardPinMax);  //son240608

		}
		else  if( nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_LEAK1 ) //son240130 2:SELF_TEST_SHORT1, 4:SELF_TEST_LEAK1
		{ 
			fprintf(fp1, "  %d,   *%5.0f,    %5.0f,      *,  %5.0f,     *,",
				err, dSelfErr1[err][0], dSelfErr1[err][1],            //son err, card,  VB cardPin
				dSelfErr1[err][1] +(int)(dSelfErr1[err][0] -1) * nCardPinMax );  //son240608	

		}
		else  if(  nCase==SELF_TEST_SHORT2 || nCase==SELF_TEST_LEAK2    ){ //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2
			fprintf(fp1, "  %d,   *%5.0f,   *,  %5.0f,      *,  %5.0f,    ",
				err, dSelfErr1[err][0],   dSelfErr1[err][2],      //son err, card,  VC cardPin
				dSelfErr1[err][2] +(int)(dSelfErr1[err][0] -1) * nCardPinMax );  //son240608

		}
		else if( nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B ) //son240130 6:SELF_TEST_LEAK_A  7:SELF_TEST_LEAK_B
		{
            fprintf(fp1, "  %d,   *%5.0f,   %5.0f, %5.0f,    %5.0f, %5.0f, ",
                    err, dSelfErr1[err][0],  dSelfErr1[err][1], dSelfErr1[err][2],  //son err, card, VB cardPin, VC cardPin
                    dSelfErr1[err][1] + (int)(dSelfErr1[err][0] -1) * nCardPinMax,   //son240608
                    dSelfErr1[err][2] + (int)(dSelfErr1[err][0] -1) * nCardPinMax);	//son240608
 
		}

		else if(  nCase==SELF_TEST_VC_1PIN_RST ) //son240130 8:SELF_TEST_VC_1PIN_RST
		{

			int   bb1, bb2, bb3  ;	 
			bb1=  (int)dSelfErr1[err][0];  //QD PIN NO
			//bb2=  PinMap[2][bb1]; //switch card PIN NO
			bb2=  bb1/nCardPinMax+1;//card no  //	nCardPinMax=64;//sylee150210 //ACE380
			bb3=  bb1%nCardPinMax+1; //PIN NO  //	nCardPinMax=64;//sylee150210 //ACE380
			tmp=(bb1+1)%4096;

			//sylee150908-1     if( tmp !=0 ){
			fprintf(fp1, "  , %d,    %d   ,    %d   ,    %d    ,    %.1f ",err,   bb2, bb3, bb1, dSelfErr1[err][1]     );				    
			fprintf(fp1, "  \n "     );				    
			//sylee150908-1     }

		}
		else if(  nCase==SELF_TEST_VB_1PIN_RST )//SYLEE130704 //son240130 9:SELF_TEST_VB_1PIN_RST
		{

			int   bb1, bb2, bb3  ;	 
			bb1=  (int)dSelfErr1[err][0];  //QD PIN NO
		 
			bb2=  bb1/nCardPinMax+1;//card no  //	nCardPinMax=64;//sylee150210 //ACE380
			bb3=  bb1%nCardPinMax+1; //PIN NO  //	nCardPinMax=64;//sylee150210 //ACE380
			tmp=(bb1+1)%4096;		 
			fprintf(fp1, "  , %d,    %d  ,   %d   ,   %d  ,  %.1f ",err,   bb2, bb3, bb1, dSelfErr1[err][1]   );				    
			fprintf(fp1, "  \n "     );				    
		 
		}

		if( nCase!=SELF_TEST_VC_1PIN_RST  && nCase!=SELF_TEST_VB_1PIN_RST )//SYLEE130704 
		{               //son240130 8:SELF_TEST_VC_1PIN_RST, 9:SELF_TEST_VB_1PIN_RST
		 
			fprintf(fp1, "  %10.2f,  %10.2f,  %10.2f,   , %12.2f, ",dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5],dSelfErr1[err][6]);

			if( nLeakCheckFlag1==1) {	//sylee230822
				RCalCovert(327, dSelfErr1[err][6], RAW_TO_REAL, 0);//sylee230822
				if(Sub71.R1>1000000){	    fprintf(fp1, "*,%.f,Mohm, ",(Sub71.R1/1000000) ); //sylee230822
				}else if(Sub71.R1>1000){	fprintf(fp1, "*,%.f,Kohm, ",(Sub71.R1/1000) );
				}else{    				    fprintf(fp1, "*,%.f,ohm, ",Sub71.R1  );
				}
			}

			fprintf(fp1, " \n");
		 
		}

	}		 
		 
 

	if( nCase==SELF_TEST_VC_1PIN_RST ){//sylee130704   //son240130 8:SELF_TEST_VC_1PIN_RST
		m_combo2.SetCurSel(SELF_TEST_VB_1PIN_RST);     //son240130 9:SELF_TEST_VB_1PIN_RST
		nCase=SELF_TEST_VB_1PIN_RST;                   //son240130 9:SELF_TEST_VB_1PIN_RST
		goto Task100;
	}

	if( g_nErrCount1<1){
		fprintf(fp1, "     *** NO ERROR *** " );
	}
	else{
		fprintf(fp1, "     ***  TOTAL ERROR  = %d ea *** ",g_nErrCount1 );
	}


	fclose(fp1);  
 
	Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018 
	Sub71.AOnPoweroff2(); //sylee20120702 
	Sub71.AOnReadV1Enable1(5); //sylee130711

	if( nStop==2){
		AfxMessageBox(" SelfTest :  STop.    "); 	 
	}
	// ::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);

	if( SysSet13.m_nSet7==1 ){//sylee130724
		::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);		 
	}
	else{						
		::ShellExecute(NULL,"open","notepad.EXE",fName1,"NULL",SW_SHOWNORMAL);	   
	}

	//		AfxMessageBox(" SelfTest  END!!!"); 

	Sub71.OnPinResetInit();//sylee151221   //ACE400

	// hdpark220819 begin
	KillTimer(10);
	KillTimer(20);	
	KillTimer(30);
	KillTimer(40);
	KillTimer(50);
	KillTimer(60);
	// hdpark220819 end

	return;

}










//son240608 Pin 설정할 때 VB CardPin, VC CardPin 정보를 저장하기 위한 wrapper 함수.
void CChildView8::Self_AOnPinSet1(int nPin, PIN_SET_MOD nMode, int nCardPin)
{
    if (nMode == MOD_TR_SET_VC)
        m_nCardPinVC = nCardPin;
    else
        m_nCardPinVB = nCardPin;

    Sub71.AOnPinSet1(1, nPin, nMode);  

}

//son240608 Pin All Reset할 때 VB cardPin, VC CardPin 정보를 clear하기 위한 wrrapper 함수.
int CChildView8::Self_OnPinAllReset1()
{
    m_nCardPinVB = -1;
    m_nCardPinVC = -1;
    return Sub71.OnPinAllReset1();
}

//=======================================================================================================



void CChildView8::OnATestRun4W(int  m_nMode1  )    //   Case=1 s1,    2=  s2
{		
 
	CString   str,str1,  strfName,strfName1;
 
	int nTestMode;
	int	nFlagMode1,nFail1,n4Wire,nRet1;//,m;	 
	int dPinVC[200], dPinVB[200];
	 __int64   tStart1, tStart2 ; 
	FILE  *fp1;
	char  fName[200], fName1[200],szText[50];
 
	double nISet,dVbR1,dVbR2;	
	double nT[10];
	double nData[130][15];
	int Lo1, nLoop,  ki, nFil,nIFilter,nHVOffSet,nHVGain;//sylee130629 
	int nPinSt[32000],nSta[350][128][5],nFailSta[500];//card max no

    
    Sub71.OnDisCharge1(); //sylee170725-3 ray

    //son231227 Get_nCardPinMax();//sylee150903 //ACE400     //son231227 중복호출 삭제


 ///////////////////////////////////////////////////////////////////////////
//변수 초기화 

	::ZeroMemory (&nPinSt, sizeof(nPinSt));
	::ZeroMemory (&nData, sizeof(nData));
 	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory (&fName, sizeof(fName));
	::ZeroMemory (&fName1, sizeof(fName1));
 
	::ZeroMemory (&dSelfErr1, sizeof(dSelfErr1));
	::ZeroMemory (&nFailSta, sizeof(nFailSta));


    g_nErrCount1=0;
	g_nErrCountTot=0;
    Sub71.nADCount1=1;//test

	nStop=0;
	n4Wire=0;
	nFlagMode1=0;
	nFail1=0;
	g_nStopFlag=0;
	//  nTestCase1=m_combo2.GetCurSel();
	//  nCase=nTestCase1;


	//sylee150903  //ACE400 shin 	str.Format("9.0"); 
	//sylee150903  //ACE400 shin	m_edit8.SetWindowText(str);
	m_edit8.GetWindowText( szText,10); 
	nISOSet1 = atof( szText ); //sylee20120509  

    //son nHW_CardPin, nCardPinMax을  SysSet12.m_nPinBoxMax와 SysSet13.m_nPinBlockType 에 맞게 설정
	Get_nCardPinMax(); //sylee150903 //ACE400        //son231227

	//----------------------------
	//son Read m_nCardNoStart 
	m_edit1.GetWindowText( szText,10); 
	m_nCardNoStart = atoi( szText );	

	if( m_nCardNoStart<1){	 
		str.Format("Error Self,  Begin Card < 1 . \n \n  Set Error !     Begin Card=%d,    ",m_nCardNoStart );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;	
	}


	if(nCardPinMax==64){ //sylee150903-1	//ACE400	  
		if( m_nCardNoStart>512){	 
			str.Format("Error Self,  Begin Card > 512( 32k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}

	}
	else{
		if( m_nCardNoStart>256){	 
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}
	}


	//----------------------------
	//son Read m_nCardNoEnd 

	m_edit2.GetWindowText( szText,10); 
	m_nCardNoEnd = atoi( szText );

	if( m_nCardNoEnd<1){	 
		str.Format("Error Self,  End Card < 1 . \n \n  Set Error !    End Card=%d,    ",m_nCardNoEnd );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;	
	}



	//son231227 Get_nCardPinMax(); //sylee150903 //ACE400    //son231227 중복호출 삭제

	if(nCardPinMax==64){ //sylee150903-1 //ACE400
		if( m_nCardNoEnd>512){	 
			str.Format("Error Self,  End Card > 512 ( 32k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}

	}
	else{

		if( m_nCardNoEnd>256){	 
			str.Format("Error Self,  End Card > 256 ( 32k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return ;	
		}

	}

	if( m_nCardNoEnd<m_nCardNoStart){	 
		str.Format("Error Self,  End Card < Begin Card  . \n \n  Set Error !     Begin Card=%d,  End Card=%d,   ",m_nCardNoStart,m_nCardNoEnd );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return ;	
	}


	m_nCardNoStart =m_nCardNoStart-1;
	m_nCardNoEnd =m_nCardNoEnd-1;

 
	///////////////////////////////////////////////////////////////////////////
	// 변수 설정 
 
	strfName1.Format("\\SETUP\\001SelfRelay.csv");         //<=======================================
	DeleteFile(strfName1);
	::ZeroMemory(&fName1,sizeof(fName1));
    strcat( fName1 , SysInfoView01.m_pStrFilePath1 );//SYLEE20111212
	strcat( fName1 , strfName1 );
 
	fp1 = fopen(fName1,"wt"); 
	if(fp1 == NULL){ 
		str.Format("Error    001SELFHVISO file open   ?  Error, Saving Error !     file close!   " );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return  ;
	}
 
	nCC= (int)dRec[m_nMode1][1];
	nVRel= (int)dRec[m_nMode1][2];   
	nVSet=  dRec[m_nMode1][3];
	nIR=  (int)dRec[m_nMode1][4];
	nISet= dRec[m_nMode1][5];
	nFil=  (int)dRec[m_nMode1][8];

	nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
	nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
	nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629


	if( nVSet>20 ){
		AfxMessageBox(" Error No 8312, SelfTest,Relay \n\n     Set check LV !,  4W is LV.  \n \n  Voltage SET=%d ? ",(int)nVSet); 
		g_nSelfRunFlag=0;
		return; 
		//sylee130701   nFil= 0;//Fast speed
	}


	if( (nIR==1)  &&  (nISet>10)  ){
		AfxMessageBox(" Error No 8314, SelfTest,Relay \n\n    Set check !,  4W is I=1,  (nISet>10 ?  error ) . \n \n    I Range SET=%d? ",nIR); 
		g_nSelfRunFlag=0;
		return; 
	}


	nTimeDelay1=(int) (dRec[m_nMode1][6]*10);   
	Sub71.nADCount1 =(int)dRec[m_nMode1][7];

	if(Sub71.nADCount1<1 ){
		Sub71.nADCount1=1;
	}

	if( Sub71.nADCount1>100){
		Sub71.nADCount1=100;
	}


	nRet1=Sub71.AOnPoweroff2();//sylee20120702
	if(nRet1!=1){
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 8315, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
		g_nSelfRunFlag=0;
		return  ;
	}

	if( nVRel==1){
		nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
	}
	else  if( nVRel==2){
		nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;

	} 

	if( nRet1!=1){
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 1315, \n   V SET ERROR !,   SW TEST Run Start Error!!!"); 
		g_nSelfRunFlag=0;
		return ;
	}
	Sub71.Delay_100ns(3000000);  //300ms   

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//설정부 	 

	tStart1=GetMilSecond2(); 


	Lo1=3;
	n4Wire=4;//sylee130702

 Task101:  
	
	Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

	if(nVSet<5){
		dVbR2=nVSet*(0.35);//10%
	}
	else{
		dVbR2=nVSet*(0.1);//5%
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
		//	return ;
	}

	if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){

		if(Lo1-->0){
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}

		str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		g_nSelfRunFlag=0;
		//	return ;
	}

	//SYLEE150210 MOVE		Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);  // vb relay on 기능   leakage발생 부분   *************************

	//SYLEE150210 MOVE		Sub71.Delay_100ns(3000000);  //300ms  

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//		V1old=Sub71.V1;//sylee150210 //ACE380


	Sub71.AOnReadV1Enable1(nVRel);//SYLEE130704

	//		Sub71.V1=V1old;//sylee150210 //ACE380
	nFil=1;//all off//SYLEE130704
	Sub71.FilterSet(nFil);//sylee130628 

	nIFilter=1;
	Sub71.I_Filter(nIFilter);//sylee130629 

	Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);   //SYLEE150210 MOVE 
	Sub71.Delay_100ns(3000000);  //300ms    //SYLEE150210 MOVE  


	//sylee130906		Sub71.OnDisChargeuS(100000);  // -> 100ms 
	tStart2=GetMilSecond2();


	nT[0]=(double)(tStart2-tStart1)/1000.;  //test 
	CTime curTime = CTime::GetCurrentTime();

	//       fprintf(fp1, "#MakeDate:   Year%04d:   Month%02d:   Day%02d    %02dH:%02dM:%02dS, \n\n\n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
	fprintf(fp1, "#   %04d /  %02d /  %02d   -   %02d: %02d: %02d  , \n\n\n", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());


	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//측정부

	OnList1Mes1("                                 ");
	str.Format("  ***  4W Relay Check  Start  ***   ");
	OnList1Mes1(str);

	fprintf(fp1, str );
	fprintf(fp1, "\n\n" ); 
	::ZeroMemory (&nSta, sizeof(nSta));  

	//	testCase       int nSta[128][10];  0= all , 
	//            1 =VB VC OFF S1,   2 =VBON S1,         
	//            3 =VB VC OFF S2,   4 =VCON S2, 
	// ERROR =1, PASS=0

	int card, cardPin; 

	for( int testCase=1; testCase<=4; testCase++)//TEST CASE //sylee20120516
	{


		if( testCase==1 || testCase==3 ){//  s1
			if(testCase==3 ){
				//sylee130802
				//sylee131102	Sub71.Delay_100ns(6000); //sylee130702     10mSec->1mS
				Sub71.Delay_100ns(10000); //sylee131102
				//Sub71.Delay_100ns(5000);//sylee130802
			} 
			Sub71.On4WS12Set( 1, 1);	    //s1 self on //38  i46	
		}
		else if( testCase==2 ||  testCase==4 ){//s2
			if(testCase==4 ){
				//sylee130802
				Sub71.Delay_100ns(10000); 
				//sylee131102Sub71.Delay_100ns(6000); //sylee130702     10mSec->1mS
				//Sub71.Delay_100ns(5000);//sylee130802	
			} 
			Sub71.On4WS12Set( 2, 1);		//s2  self on //39	i47 
		}


		int nCardTo1;

		for( card=m_nCardNoStart; card<=m_nCardNoEnd; card++ ) //card no
		{ 

#if 0       //son210701
            //son-32KPIN-20200228 : 16K 상부 뒤쪽 두개를 skip
            //son 16K의 경우는 상부 2box(box 1, box 2)만, 하부 2box(box 5, box 6) 사용.
            //    box 3과 box 4는 건너 뛰어야 한다.  box 3: card64 ~ card95 
            //                                       box 4: card96 ~ card127
            //if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS   //son230216_1
			//	|| SysSet12.m_nPinTotMax == PIN_TOT_12K	//son210202  box1부터 시작, 3번째, 4번째 box skip
			//	|| SysSet12.m_nPinTotMax == PIN_TOT_12K_FULL_TRANS) //son210319
			if (ModeSet12.IsUpper8k(SysSet12.m_nPinTotMax))     //son210831
            {
                if (card >= 64  && card < 128)
                    continue;
            }
            //son-8K4097-20200605 : 8K_구형4097  상부 뒤쪽  2,3,4 box를 skip
            //son 8K_구형4097의 경우는 상부 1box(box 1)만, 하부 1box(box 5) 사용.
            //    box 2,3,4 는 건너 뛰어야 한다.  box 2: card32 ~ card63   128pin card
            //                                    box 3: card64 ~ card95 
            //                                    box 4: card96 ~ card127
            //if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN || SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)  //son210319 //son230216_1
            //if (IS_UPPER_4K(SysSet12.m_nPinTotMax))     //son210531
			if (ModeSet12.IsUpper4k(SysSet12.m_nPinTotMax))     //son210831
            {
                if (card >= 32  && card < 128)
                    continue;
            }

            //son Upper box 4 skip
            //    PIN_TOT_20K_U12K_L4K_UP_TRANS, PIN_TOT_16K_U12K_L4K_TRANS, PIN_TOT_16K_U12K_L4K_UP_TRANS 
            if (IS_UPPER_12K(SysSet12.m_nPinTotMax))     //son210531
            {
                if (card >= 96  && card < 128)
                    continue;
            }
#endif

			nLoop=5; 
			//sylee130906	   Sub71.OnDisChargeuS(1000);//1mS//SYLEE20120517

			nCardTo1=m_nCardNoEnd-m_nCardNoStart+1;//SYLEE130704
			// if( nProCase1==6){
			nProgress5Rate=  (int)((double)((((testCase-1)*nCardTo1)+(card-m_nCardNoStart+1))/((double)(4.*nCardTo1))*100.));
			nProgress1Rate=0;
			nProgress2Rate=0;
			nProgress3Rate=0;
			nProgress4Rate=0;
			nProgress6Rate=0;

			// }

			int nPin;//sylee151007 //ACE400 //son231222_3 nTemp1 => nPin


			for(cardPin=0; cardPin<nCardPinMax; cardPin++)   //nCardPinMax=64;//sylee150210 //ACE380 
			{


Task1://sylee20120308

				//DoEvents();
				DoEvents();

				if( g_nStopFlag==2){
					g_nSelfRunFlag=0;
					goto task2;
				} 

				Sub71.OnPinAllReset1(); ///PIN ALL RESET
				//test	 Sub71.AOnReadV1I1(nVRel);//SYLEE20111115 
				ki=0;
				//Task5:
				////////////////////////////////////////////////////////////////
				
                nPin = GetHwPinNo_Old(card, cardPin);     //son240125 GetHwPinNo_Old() 함수호출로 코드 간소화 
				

				if(  testCase==3 ){
					Sub71.AOnPinSet1(1, nPin , MOD_RY_SET_VB);  //  relay VB 3,   ///sylee151007 //ACE400 //son210220 enum으로 변경
				}
				else if( testCase==4 ){
					Sub71.AOnPinSet1(1, nPin , MOD_RY_SET_VC);  //  relay vc 4,   //nCardPinMax=64;//sylee150210 //son210220 enum으로 변경
				}	

				//sylee130802if( testCase==3 || testCase==4){
				//sylee130802 Sub71.Delay_100ns(10000); //     2mSec  //SYLEE130704   2mS->1mS
				//sylee130802} 

				//Sub71.AOnPinSet1(1, nPin, 1);  //    //nCardPinMax=64;//sylee150210 //ACE380 
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  //    //nCardPinMax=64;//sylee150210 //ACE380 //son210220 enum으로 변경

				Sub71.Delay_100ns(10000); //sylee131102 
				//sylee131102	  Sub71.Delay_100ns(6000); //sylee130702     10mSec->1mS
				//Sub71.Delay_100ns(5000);//sylee130802

				//sylee130702
				Sub71.AOnReadV1I1(nVRel);//SYLEE20111115 
				// Sub71.AOnReadV1I1(5);//SYLEE130702

				//=============================================
				if(Sub71.Y1<0.0000001){
					Sub71.Y1=0.0000001;
				}

				if( nIR==1 ||  nIR==2 ){
					Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
				}
				else if( nIR==3 ||  nIR==4  ||  nIR==5 ){
					Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
				}          

				////////////////////////////////////////////////////////////////////////////////////////
				//if( n4Wire==4 ){
				//	Sub71.R1=Sub71.R1*1000;
				//}

				nRet1=Sub71.OnPinAllReset1(); ///PIN ALL RESET  
				if( nRet1!=1){
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
					g_nSelfRunFlag=0;
					return  ;
				}


				Sub71.Delay_100ns(10000); //     1mSec  

				//sylee130802 Sub71.OnDisChargeuS(5000);//5mS 
				//sylee130906	Sub71.OnDisChargeuS(1000);//1mS  //sylee130802 



				if( card==m_nCardNoStart){
					if( nLoop >0){  //sylee20120308
						if(nLoop==1){ 
							cardPin=0;
						}
						nLoop--;
						goto Task1; 	
					}
				}

				if(	testCase==3 || testCase==4 ){ //open test
					nTestMode=0;
				}
				else if( testCase==1 || testCase==2 ){ //short test
					nTestMode=2;   
				}

				if(nTestMode==2){  //short	 
					//	if( Sub71.X1> nISOSet1 ){//fail log  0-> nISOSet1  0.61
					if( Sub71.X1> 1 ){//fail log  0-> nISOSet1  0.61
						if(g_nErrCount1<MAX_FAIL_COUNT){		//hdpark221005 2999->MAX_FAIL_COUNT
							g_nErrCount1++;
							g_nErrCountTot++;
							nSta[card][cardPin][testCase]=1;//cardPin(0~127)/ testCase  test case 1~6)
						}
						else{
							break;
						}

						dSelfErr1[g_nErrCount1][0]=(float)(card+1);  //  CARD no.			    
						dSelfErr1[g_nErrCount1][1]=(float)(nPin+1);  ////sylee151007 //ACE400				  
						dSelfErr1[g_nErrCount1][2]=(float) testCase ; // 1=  VB RELAY,   2= VC RELAY					  
						dSelfErr1[g_nErrCount1][3]=(float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4]=(float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5]=(float)Sub71.Y1;   //cardPin
						dSelfErr1[g_nErrCount1][6]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3]; 	 		
					}
				}
				else//(nIso1==1  //open			 
				{

					if( Sub71.X1< nISOSet1){ //sylee20120512    8v
						if(g_nErrCount1<MAX_FAIL_COUNT){		// hdpark221005 2999->MAX_FAIL_COUNT
							g_nErrCount1++;
							g_nErrCountTot++;
							nSta[card][cardPin][testCase]=1;//cardPin(0~127)/ testCase  test case 1~6)
						}
						else{
							break;
						}

						dSelfErr1[g_nErrCount1][0]=(float)(card+1);  //  CARD no.			    
						dSelfErr1[g_nErrCount1][1]=(float)(nPin+1); ///sylee151007 //ACE400					  
						dSelfErr1[g_nErrCount1][2]=(float) testCase ; // 1=  VB RELAY,   2= VC RELAY					  
						dSelfErr1[g_nErrCount1][3]=(float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4]=(float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5]=(float)Sub71.Y1;   //cardPin
						dSelfErr1[g_nErrCount1][6]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3];

					} 

				}


				if( nSta[card][cardPin][testCase]>0){
					nSta[card][cardPin][0]+=nSta[card][cardPin][testCase];
				}

			}////for END   for( cardPin=0; cardPin<128; cardPin++)   //pin 

		}   //for END  for( card=0; card< nCount; card++)  //CARD NO 

	}   // for( int testCase=1; testCase<=4; testCase++)

 

	for( card=m_nCardNoStart; card<=m_nCardNoEnd; card++ )  //card no
	{
		for( cardPin=0; cardPin<nCardPinMax; cardPin++){   //pin   //nCardPinMax=64;//sylee150210 //ACE380 
			if(nSta[card][cardPin][0]>0){
				nFailSta[card]++;
			}
		}

		if(nFailSta[card]>0){
			str.Format("  *  Error :4W Relay=>   Switch Card=%d   Fail=%d     ",   card+1 , nFailSta[card]);	
			OnList1Mes1(str);  
			fprintf(fp1,   str );  //<=======================================
			fprintf(fp1, "\n" );
		} 
	}

	str.Format("  ***  4W Relay Check  End  ***   ");	
	OnList1Mes1(str);	


task2:  //STOP JUMP POSTION


	if( g_nStopFlag==2)
	{
		g_nSelfRunFlag=0;
		g_nStopFlag=0;
		str.Format("  * Stop   [Self Test  ::  RELAY ]  ");	
		OnList1Mes1(str);	 
		AfxMessageBox(str); 		 	 
	}  

	fprintf(fp1, " \n  "  );
	fprintf(fp1, "* SelfTest   RELAY SELF TEST    \n  "  );  //<=======================================
	fprintf(fp1, " , No,  *CardNo         ,   Pin,  Total Pin ,      *Result* ,   VB,        VC,       I,         R,   \n ");	
 
	// for( m=0; m<1; m++){
	int err;
	for( err=1; err<=g_nErrCount1; err++)//sylee20111215
	{
		if(dSelfErr1[err][2]==1){
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S1 Short  ,",
			        err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*nCardPinMax),dSelfErr1[err][1]);//nCardPinMax=64;//sylee150210 //ACE380 
		}
		else if(dSelfErr1[err][2]==2){
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S2 Short  ,",
			        err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*nCardPinMax),dSelfErr1[err][1]);//nCardPinMax=64;//sylee150210 //ACE380 
		}
		else if(dSelfErr1[err][2]==3){
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S1 Open  ,",
			        err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*nCardPinMax),dSelfErr1[err][1]);//nCardPinMax=64;//sylee150210 //ACE380 
		}
		else if(dSelfErr1[err][2]==4){
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S2 Open  ,",
			        err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*nCardPinMax),dSelfErr1[err][1]);//nCardPinMax=64;//sylee150210 //ACE380 
		}
		fprintf(fp1, "   %10.2f, %10.2f, %10.2f,  %12.2f, ",dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5],dSelfErr1[err][6]);
		fprintf(fp1, " \n");
	}

	if( g_nErrCount1<1){
		fprintf(fp1, "      ***NO ERROR *** " );
	}
	else{
		fprintf(fp1, "      ***  TOTAL ERROR  = %d ea *** ",g_nErrCount1 );
	}

	fclose(fp1); 

	Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018 
	Sub71.AOnPoweroff2();//sylee20120702

	Sub71.AOnRelayOff1();	//sylee20120616 


	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704
	Sub71.AOnReadV1Enable1(5); //sylee130711

	Sub71.Delay_100ns(20000);  //2ms 

	if( nStop==2){
		AfxMessageBox("   SelfTest::  4W Relay Check STop!!!"); 	 
	}



	if( SysSet13.m_nSet7==1 ){//sylee130724
		::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);		 
	}
	else{						
		::ShellExecute(NULL,"open","notepad.EXE",fName1,"NULL",SW_SHOWNORMAL);	   
	}


	if(g_nErrCount1>=MAX_FAIL_COUNT){//sylee151007 //hdpark221005 2999->MAX_FAIL_COUNT
		str.Format("  * =>  4W ERROR : 3000 ea over. more error   ");	
		OnList1Mes1(str);

	}


	return ;

}







void CChildView8::OnButton201_SystemAuto() //all self //son230302
{

	int nStartCard;
	int nEndCard;



	return ;



	if(g_nSelfRunFlag>0){
		//    AfxMessageBox("   Running.        Switch card   2W  All selftest  Check.   Start Error  !!!"); 
		return;
	}
	g_nSelfRunFlag=1;


	//===================================================================
	//HWDEFINE

	g_nAutoFlag=2;


	nStartCard=0;
	nEndCard=32;


	//================================================================	 
	//	OnButton101();  // power check

	OnList1Mes1("*** HW Power Check Start***");
	OnCheckPowerHW1(1);  
	OnList1Mes1("*** HW Power Check Finish***");
	OnList1Mes1("                         ");



	if( g_nStopFlag==2){
		g_nSelfRunFlag=0;
		g_nStopFlag=0; 
		return;
	}
	//================================================================
	//  OnButton102(); //ad check

	OnList1Mes1("*** VB   Check Start***");
	OnCheckVB1();  
	OnList1Mes1("*** VB  Check Finish***");
	OnList1Mes1("                       ");


	if( g_nStopFlag==2){
		g_nSelfRunFlag=0;
		g_nStopFlag=0; 
		return;
	}	
	//================================================================


	if( g_nStopFlag==2){
		g_nSelfRunFlag=0;
		g_nStopFlag=0; 
		return;
	}	
	//================================================================


	g_nAutoFlag=0;

	g_nSelfRunFlag=0;


	return ;
}





void CChildView8::OnAuto_Self1()    //Switch card   All selftest  
{
	

 	CString   str,str1,str2, strfName,strfName1; 
	FILE  *fp1;
	char  fName[200], fName1[200], szText[50];	
    int nFlagMode1, nFail1, n4Wire, nRet1, nHVGain, nFil, a;
	int dPinVC[200],dPinVB[200], nPinSt[32000];
    int CardS[260][130];     //cardno, pin no 
	double nISet;	
	double nT[10], nData[130][15]; 
	int nLoop,ki,nCase, nProCase1,nStep1,nCo1,nCardTo1,nIso1,nRet;
	int StepErrCount, nFailflag,nEarthFlag, nIFilter,nHVOffSet;
	int nOpenErrPin[201][5];//sylee141019
	int nOpenErrPinCount;//sylee141019

 
//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380
	int nCardX;//sylee150210 //ACE380
 	
	int  nIGain ;	    //sylee150904 //ACE400


//ACE380//////////////////////////////////////////////////////////////
//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380
	nCardX=2;//sylee150210 //ACE380
//ACE380//////////////////////////////////////////////////////////////
    //son231227 Get_nCardPinMax();//sylee150903 //ACE400     //son231227 중복호출 삭제

  	if(g_nSelfRunFlag>0){
	    AfxMessageBox("   Running?    IO  start error !!!"); 
        return;
	}

 	g_nSelfRunFlag=1;


	a=FileSysInfo01.LoadSaveSet15(2);//load   //sylee140911
	if(a!=1){
		AfxMessageBox(" Error No 8212, \n\n  SysSet15.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n "); 
		return ;
	}


	nOpenErrPinCount=0;//sylee141019
	::ZeroMemory(&nOpenErrPin, sizeof(nOpenErrPin));//sylee141019


	g_nListCount=0;
 
	OnButton302_ClearMessage(); //son230302

	DoEvents(); DoEvents();
	DoEvents(); DoEvents();


	str.Format("  ***  IO Check Start  ***   ");								 
	OnList1Mes1(str);

	DoEvents(); DoEvents();

	nEarthFlag=0;


	::ZeroMemory (&CardS, sizeof(CardS));
	::ZeroMemory (&nPinSt, sizeof(nPinSt));
	::ZeroMemory (&nData, sizeof(nData)); 
	::ZeroMemory (&nT, sizeof(nT));
	::ZeroMemory (&nData, sizeof(nData));
	::ZeroMemory (&dPinVC, sizeof(dPinVC));
	::ZeroMemory (&dPinVB, sizeof(dPinVB));	
	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));

	::ZeroMemory (&dSelfErr1, sizeof(dSelfErr1));


	g_nStopFlag=0;
	g_nErrCount1=0;
	g_nErrCountTot=0;
	nStop=0;
	n4Wire=0;
	nFlagMode1=0;
	nFail1=0;
	g_nSelfTestInterlock=0;//sylee131121


	
	g_nAutoFlag=3;
    Sub71.nADCount1=1;//test


	//===================================================================
	//HWDEFINE

	//sylee140130	strfName1.Format("\\SETUP\\SelfAuto_2WTr.CSV" );         //<=======================================
	strfName1.Format("d:\\log\\SelfAuto_2WTr.CSV" );         //<=======================================
	//	DeleteFile(strfName1);
	::ZeroMemory(&fName1,sizeof(fName1));

	strcat( fName1 , strfName1 ); 
	fp1 = fopen(fName1,"wt"); 

	if(fp1 == NULL){ 
		str.Format("Error    001SelfAuto_2WTr file open   ?  Error, Saving Error !     file close!   " );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		return  ;
	}

	//=======================================================================
	m_edit1.GetWindowText( szText,10); 
	m_nCardNoStart = atoi( szText );	

	if( m_nCardNoStart<1){	 
		str.Format("Error Self,  Begin Card < 1 . \n \n  Set Error !     Begin Card=%d,    ",m_nCardNoStart );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		g_nAutoFlag=0;
		return ;	
	}

    //son nHW_CardPin, nCardPinMax을  SysSet12.m_nPinBoxMax와 SysSet13.m_nPinBlockType 에 맞게 설정
	Get_nCardPinMax(); //sylee150903 //ACE400        //son231227

	if(nCardPinMax==64){ //sylee150903-1 //ACE400
		if( m_nCardNoStart>512){	 
			//SYLEE150210 //ACE380	str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );
			str.Format("Error Self,  Begin Card > 512 ( 16k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );//SYLEE150210 //ACE380
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			g_nAutoFlag=0;
			return ;	
		}

	}
	else{

		if( m_nCardNoStart>256){	 
			//SYLEE150210 //ACE380	str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );
			str.Format("Error Self,  Begin Card > 256 ( 16k)  .  \n \nSet Error !     Begin Card=%d,    ",m_nCardNoStart );//SYLEE150210 //ACE380
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			g_nAutoFlag=0;
			return ;	
		}
	}

	m_edit2.GetWindowText( szText,10); 
	m_nCardNoEnd = atoi( szText );

	if( m_nCardNoEnd<1){	 
		str.Format("Error Self,  End Card < 1 . \n \n  Set Error !    End Card=%d,    ",m_nCardNoEnd );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		g_nAutoFlag=0;
		return ;	
	}

	//son231227 Get_nCardPinMax(); //sylee150903 //ACE400  //son231227 중복호출 삭제

	if(nCardPinMax==64){ //sylee150903-1 //ACE400

		if( m_nCardNoEnd>512){	 
			//SYLEE150210 //ACE380 str.Format("Error Self,  End Card > 256 ( 32k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );
			str.Format("Error Self,  End Card > 512 ( 15k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );//SYLEE150210 //ACE380
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			g_nAutoFlag=0;
			return ;	
		}
	}
	else{

		if( m_nCardNoEnd>256){	 
			//SYLEE150210 //ACE380 str.Format("Error Self,  End Card > 256 ( 32k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );
			str.Format("Error Self,  End Card > 256 ( 15k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );//SYLEE150210 //ACE380
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			g_nAutoFlag=0;
			return ;	
		}
	}

	if( m_nCardNoEnd<m_nCardNoStart){	 
		str.Format("Error Self,  End Card < Begin Card  . \n \n  Set Error !     Begin Card=%d,  End Card=%d,   ",m_nCardNoStart,m_nCardNoEnd );
		AfxMessageBox( str, MB_OK); //error messege
		g_nSelfRunFlag=0;
		g_nAutoFlag=0;
		return ;	
	}


	nCardTo1=m_nCardNoEnd-m_nCardNoStart+1;

	m_nCardNoStart = m_nCardNoStart-1;
	m_nCardNoEnd = m_nCardNoEnd-1;


	//============================================================================================

 
	for( int nProStep1=SELF_TEST_LV_OPEN; nProStep1<=(SELF_TEST_MAX -1); nProStep1++ )  //son240130 1:SELF_TEST_LV_OPEN, 18:SELF_TEST_MAX
	{

		StepErrCount=0;

		Sub71.OnPinAllReset1(); ///PIN ALL RESET

		if( g_nStopFlag==2 ){	//SYLEE131121
			nProStep1=SELF_TEST_MAX;   //son240130 18:SELF_TEST_MAX
			continue;
		}

		if( g_nStopFlag!=2 ){	//SYLEE130916	 
			Sleep(200);	Sleep(200);	Sleep(200);	Sleep(200);	Sleep(200);
		}

		if( g_nStopFlag!=2){
			Sub71.Delay_100ns(5000000);  //400ms //sylee120906
		}
		if( nProStep1==SELF_TEST_LV_LEAK1 ||  nProStep1==SELF_TEST_LV_LEAK2 ){  //SPARE   //son240130 4:SELF_TEST_LV_LEAK1,  5:SELF_TEST_LV_LEAK2
			continue;
		}
		if( nProStep1>=SELF_TEST_VC_1PIN_RST &&  nProStep1<=13 ){  //SPARE  //son240130 8:SELF_TEST_VC_1PIN_RST
			continue;
		}
		if( nProStep1==SELF_TEST_LV_OPEN ||  nProStep1==SELF_TEST_HV_LEAK1 )		//son240130 1:SELF_TEST_LV_OPEN,  14:SELF_TEST_HV_LEAK1	
        {
			if( nProStep1==SELF_TEST_LV_OPEN ){     //son240130 1:SELF_TEST_LV_OPEN
				g_nMode1=MODE181_SELF_10V;  //lv 10v   //sylee130701    //son240130
			}
			else if( nProStep1==SELF_TEST_HV_LEAK1 ){   //son240130 14:SELF_TEST_HV_LEAK1
				g_nMode1=MODE182_SELF_200V;  //hv  250v  //sylee130701   //son240130
			}

			nRet=AutoSelf_PowerModeSet(g_nMode1);
			if( nRet==0){
				fclose(fp1); 
				AfxMessageBox(" Errro No 8023, \n    AutoSelf_PowerModeSet     SET ERROR !"); 
				//return; 
				goto task2;
			}


			nCC= (int)dRec[g_nMode1][1];
			nVRel= (int)dRec[g_nMode1][2];   
			nVSet=  dRec[g_nMode1][3];

			nIR=  (int)dRec[g_nMode1][4];
			nISet= dRec[g_nMode1][5];

			nFil=(int)dRec[g_nMode1][8];//SYLEE130706

			nIFilter = (int)dRec[g_nMode1][9];   //SYLEE130629		  
			nHVOffSet =(int) dRec[g_nMode1][10]; //SYLEE130629
			nHVGain = (int)dRec[g_nMode1][11];	//SYLEE130629

        }


		nIGain = (int)dRec[g_nMode1][12];	    //sylee150904 //ACE400

		if( nIGain==2){//sylee150904 //ACE400
			Sub71.I_Gain(2);  //10x  =io on		
		}
		else{
			Sub71.I_Gain(1);  //1x  =io off
		}

		if( nProStep1<=10 ){
			nCase=nProStep1;
		}
		else{
			nCase=nProStep1-10;
		}

		m_combo2.SetCurSel(nCase);


		if( nCase==SELF_TEST_OPEN){      //son240130 1:SELF_TEST_OPEN
			Sub71.Delay_100ns(1000000); //100mS  //sylee120921
		}
		if( nProStep1 ==SELF_TEST_LV_OPEN)  //son240130 1:SELF_TEST_LV_OPEN
        {
			nISOSet1=(nISet*0.8);
			//test	nISOSet1=10;
			nIso1=1;//open
			nProCase1=1;
		}
		else if( nProStep1 <= SELF_TEST_LV_SHORT2)        //son240130 3:SELF_TEST_LV_SHORT2
        {
			nISOSet1=2;
			//test			nISOSet1=0;
			nIso1=2;//short
			nProCase1=2;
		}
		else if(   nProStep1==SELF_TEST_HV_LEAK2         //son240130 15:SELF_TEST_HV_LEAK2
		          || nProStep1==SELF_TEST_HV_LEAK_A      //son240130 16:SELF_TEST_HV_LEAK_A
		          || nProStep1==SELF_TEST_HV_LEAK_B)     //son240130 17:SELF_TEST_HV_LEAK_B
		{
			nISOSet1=500; //500;
			nIso1=2;
			nProCase1=4;
		}
		else if( nProStep1==SELF_TEST_LV_LEAK1           //son240130 4:SELF_TEST_LV_LEAK1
		        || nProStep1==SELF_TEST_LV_LEAK2         //son240130 5:SELF_TEST_LV_LEAK2
		        || nProStep1==SELF_TEST_LV_LEAK_A        //son240130 6:SELF_TEST_LV_LEAK_A
		        || nProStep1==SELF_TEST_LV_LEAK_B)       //son240130 7:SELF_TEST_LV_LEAK_B
		{
			nISOSet1=5;
			nIso1=2;
			nProCase1=2;			
		}
		else if(  nProStep1==SELF_TEST_HV_LEAK1  ){     //son240130 14:SELF_TEST_HV_LEAK1
			nISOSet1=40;//sylee121022  30->40   KWS
			nIso1=2;			 
			nProCase1=4;			 
		}
		else{
			nISOSet1=5;
			nIso1=2;
		}

		str.Format("%.2f",nISOSet1); 
		m_edit8.SetWindowText(str);

		DoEvents(); 

		if( g_nStopFlag==2 ){	
			g_nSelfRunFlag=0;
			goto task2;
		}   


		//=====================================================================

		double InterI; //sylee20120612


		if( nIR==1 || nIR==2 ){  //mA 
			InterI=nISet*0.9; 
		}
		else if( nIR==3 || nIR==4 ){   // uA 
			InterI=nISet*0.9; 
		}
		else if( nIR==5 ){//uA
			InterI=	10.0;
		}
		//=====================================================================

		nCo1=0;

    	int cardPin, card; 
		for( card=m_nCardNoStart; card<=m_nCardNoEnd; card++ )  //card no     
		{


			nCo1++;
			//son Leak 이면
			if( nCase>=SELF_TEST_LEAK1  &&  nCase<=SELF_TEST_LEAK_B )//son240130 4:SELF_TEST_LEAK1, 7:SELF_TEST_LEAK_B	 
			//son240130 if( nCase>3  &&  nCase<8 )// HW INTERLOCK	
            {
				if(nCo1>8){ 
					nCo1=0;
					Sub71.OnPinAllReset1(); ///PIN ALL RESET
					Sub71.Delay_100ns(100);  //10us 
					//SYLEE130814 Sub71.OnDisChargeuS(100);  // -> 100uS	
				}
			}

			nLoop=5; 
			nFail1=0;	 				  

			//==================

			if( nProCase1==1 ){
				nProgress1Rate= (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
				nProgress2Rate=0;
				nProgress3Rate=0;
				nProgress4Rate=0;
				nProgress5Rate=0;
				nProgress6Rate=0;

			}
			else if( nProCase1==2 ){
				nProgress2Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
				nProgress1Rate=0; 
				nProgress3Rate=0;
				nProgress4Rate=0;
				nProgress5Rate=0;
				nProgress6Rate=0;

			}
			else if( nProCase1==3 ){
				nProgress3Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
				nProgress1Rate=0;
				nProgress2Rate=0; 
				nProgress4Rate=0;
				nProgress5Rate=0;
				nProgress6Rate=0;

			}
			else if( nProCase1==4 ){
				nProgress4Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
				nProgress1Rate=0;
				nProgress2Rate=0;
				nProgress3Rate=0;
				nProgress5Rate=0;
				nProgress6Rate=0;

			}
			else if( nProCase1==5 ){
				nProgress5Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
				nProgress1Rate=0;
				nProgress2Rate=0;
				nProgress3Rate=0;
				nProgress4Rate=0;
				nProgress6Rate=0;

			}
			else if( nProCase1==6){
				nProgress6Rate=  (int)((double)(card-m_nCardNoStart+1)/(double)nCardTo1*100);
				nProgress1Rate=0;
				nProgress2Rate=0;
				nProgress3Rate=0;
				nProgress4Rate=0;
				nProgress5Rate=0;

			}

			//================



			for( cardPin=0; cardPin<nCardPinMax; cardPin++ ) //	nCardPinMax=64;//sylee150210 //ACE380
			{


Task1://sylee20120308

				if(CardS[card+1][cardPin+1]>0){
					continue;
				}

				DoEvents();

				if( g_nStopFlag==2 ){	
					g_nSelfRunFlag=0;
					goto task2;
				} 

				if( nStop==2 ){
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
					goto task2;						 
				}

				if (nCase !=SELF_TEST_LEAK1 && nCase !=SELF_TEST_LEAK2               //son240130 4:SELF_TEST_LEAK1,  5:SELF_TEST_LEAK2
				        && nCase !=SELF_TEST_LEAK_A &&  nCase !=SELF_TEST_LEAK_B)    //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
                {
					Sub71.OnPinAllReset1(); ///PIN ALL RESET
				}

				ki=0;

				//Task5:
				////////////////////////////////////////////////////////////////



				if( nCase==SELF_TEST_SHORT1  ||  nCase==SELF_TEST_SHORT2 ){ //short1,2 //son240130 2: SELF_TEST_SHORT1, 3:SELF_TEST_SHORT2
					Sub71.Delay_100ns(10000); //1mS  //sylee120921
				}

				//VC PIN   set
				if( nCase==SELF_TEST_OPEN ){  //  VB  VC  1대1 동작 검사 //  LV 전용 //son240130 1:SELF_TEST_OPEN
					//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 2);    //	nCardPinMax=64;//sylee150210 //ACE380
					//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 1);    //	nCardPinMax=64;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VC);     //son210220 enum으로 변경
					Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VB);     //son210220 enum으로 변경
				}
				else  if( nCase==SELF_TEST_SHORT1 ){  //  case 2= short1,    pin one   //son240130 2:SELF_TEST_SHORT1
					//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 1);     //	nCardPinMax=64;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VB);     //son210220 enum으로 변경
				}
				else  if( nCase==SELF_TEST_SHORT2 ){  //  case3= short2,   //son240130 3: SELF_TEST_SHORT2
					//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 2);     //	nCardPinMax=64;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VC);     //son210220 enum으로 변경
				}
				else  if( nCase==SELF_TEST_LEAK1 ){    //  case4= leak 1,    pin multi //son240130 4: SELF_TEST_LEAK1
					//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 1);     //	nCardPinMax=64;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VB);     //son210220 enum으로 변경
				}
				else  if(  nCase==SELF_TEST_LEAK2 ){  //    case 5= leak2   pin multi //son240130 5:SELF_TEST_LEAK2
					//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 2);      //	nCardPinMax=64;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VC);      //son210220 enum으로 변경
				}
				else  if( nCase==SELF_TEST_VC_1PIN_RST  ){  // CASE6    pin   //son240130 8:SELF_TEST_VC_1PIN_RST


				}
				else  if( nCase==SELF_TEST_LEAK_A  ){  //leak A        //son240130 6:SELF_TEST_LEAK_A
					nStep1=cardPin%2;
					if(nStep1==0){
						//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 1);  //	nCardPinMax=64;//sylee150210 //ACE380
						Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VB);  //son210220 enum으로 변경
					}
					else {
						//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 2);  //	nCardPinMax=64;//sylee150210 //ACE380
						Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VC);  //son210220 enum으로 변경
					}					 

				}
				else  if( nCase==SELF_TEST_LEAK_B  ){//LEAK B	//son240130 7:SELF_TEST_LEAK_B								
					nStep1=cardPin%2;
					if(nStep1==0){
						//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 2);  //	nCardPinMax=64;//sylee150210 //ACE380
						Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VC);  //son210220 enum으로 변경
					}
					else {
						//Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, 1);  //	nCardPinMax=64;//sylee150210 //ACE380								 
						Sub71.AOnPinSet1(1, cardPin+card*nCardPinMax, MOD_TR_SET_VB);  //son210220 enum으로 변경
					} 				 

				}

//sylee230912	if(SysSet15.m_nSet6==7){//sylee140911  //test ray

					int nTime1;

					if( nCase==SELF_TEST_OPEN){  //so240130 1:SELF_TEST_OPEN
						nTime1=SysSet15.m_nSet2*10;
						if(nTime1<0){
							nTime1=0;
						}
						Sub71.Delay_100ns(nTime1);  
					}
					else  if( nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_SHORT2 ){   //son240130 2:SELF_TEST_SHORT1, 3:SELF_TEST_SHORT2
						nTime1=SysSet15.m_nSet3*10;
						if(nTime1<0){
							nTime1=0;
						}
						Sub71.Delay_100ns(nTime1);  
					}
					else  if( nCase==SELF_TEST_LEAK1 || nCase==SELF_TEST_LEAK2 ){ //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
						nTime1=SysSet15.m_nSet4*10;
						if(nTime1<0){
							nTime1=0;
						}
						Sub71.Delay_100ns(nTime1);  
					}
					else  if( nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B ){   //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
						nTime1=SysSet15.m_nSet5*10;
						if(nTime1<0){
							nTime1=0;
						}
						Sub71.Delay_100ns(nTime1);  
					}
					else  if( nCase==SELF_TEST_VC_1PIN_RST  ){      //son240130 8:SELF_TEST_VC_1PIN_RST
						Sub71.Delay_100ns(2500);   
					}
					else  if( nCase==SELF_TEST_VB_1PIN_RST  ){  //son240130 9:SELF_TEST_VB_1PIN_RST
						Sub71.Delay_100ns(2500);  
					}
					else {
						Sub71.Delay_100ns(5000);   
					} 

//sylee230912
/*
				}
				else {//if(SysSet15.m_nSet6==7){//sylee140911

					if( nCase==1 ){  //  VB  VC  1대1 동작 검사 //  LV 전용  
						Sub71.Delay_100ns(3500);  
					}
					else  if( nCase==2 ){  //  case 2= short1,    pin one
						Sub71.Delay_100ns(1000);  
					}
					else  if( nCase==3 ){  //  case3= short2,  
						Sub71.Delay_100ns(1000);  
					}
					else  if( nCase==4 ){    //  case4= leak 1,    pin multi 
						Sub71.Delay_100ns(1000); //150uS   
					}
					else  if(  nCase==5 ){  //    case 5= leak2   pin multi 
						Sub71.Delay_100ns(1000); //100uS  
					}
					else  if( nCase==6  ){  //leak A 
						//sylee150609  ray	Sub71.Delay_100ns(5000);//sylee150417  ray  1000->5000  
						Sub71.Delay_100ns(20000);//sylee150609  ray  500uS->2mS

					}
					else  if( nCase==7  ){//LEAK B	 
						//sylee150609  ray	Sub71.Delay_100ns(5000);//sylee150417  ray  1000->5000  
						Sub71.Delay_100ns(20000);//sylee150609  ray  500uS->2mS
					}



				}//if(SysSet15.m_nSet6==7){//sylee140911
*/ //sylee230912

				//////////////////////////////////////////////////////////////////////////////////////////////////////

				if( nCase!=SELF_TEST_OPEN )  //son240130 1:SELF_TEST_OPEN
                {
					Sub71.AOnReadV1I1(nVRel); 

					if( Sub71.Y1> InterI){ 
						Sub71.OnPinAllReset1(); ///PIN ALL RESET  
						//  Sub71.Delay_100ns(10000);  //1ms  relay
						Sub71.Delay_100ns(100);  //10us 
						//sylee20120707  Sub71.OnDisChargeuS(100);///100uS     //M R = 5mS
						//SYLEE130814	  Sub71.OnDisChargeuS(500);///100uS     //M R = 5mS  //sylee20120707
						if(nVRel<3){
							str.Format("Error No 8211-7,\n\n   LV Short     \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
							                    card+1,cardPin+1,Sub71.Y1,InterI); 
						}
						else {
							str.Format("Error No 8211-8,\n\n   HV Short      \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
							                    card+1,cardPin+1,Sub71.Y1,InterI); 
						}
						//AfxMessageBox(str);

						nRet1=AfxMessageBox(str,MB_YESNO | MB_ICONEXCLAMATION);

						if(nRet1!=6){	    //son Yes가 아니면
							g_nStopFlag=2;	//SYLEE140701
							goto task2;
						}
						goto TaskInterR101;					
					}
				}




				///////////////////////////////////////////////////////////////////////////////////////////////////////// 					

				Sub71.Delay_100ns(100);  //10uS 
				if( n4Wire==4 ){ //SYLEE20111118
					Sub71.AOnReadV1I1(5); // READ  4W V  , I 
				}
				else{
					Sub71.AOnReadV1I1(nVRel);//SYLEE20111115 
				}
				//=============================================




TaskInterR101:


				if(Sub71.Y1<0.0000001){
					Sub71.Y1=0.0000001;
				}


				//sylee150909 //ace400	if( nIR==1 ||  nIR==2 ){
				if( nIR==1 ||  nIR==2 || nIR==3 ||  nIR==4  ){	//sylee150909 //ace400
					Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000.0;   
					//sylee150909 //ace400	}else if( nIR==3 ||  nIR==4  ||  nIR==5 ){
				}
				else if(   nIR==5 ){	//sylee150909 //ace400	
					Sub71.R1=(Sub71.V1-Sub71.X1)/Sub71.Y1*1000000.0;
				}      

				if(  nIR==4 ){	//sylee150909 //ace400	
					Sub71.Y1=Sub71.Y1*1000.0;
				}




				if(  nCase!=SELF_TEST_LEAK1   &&  nCase!=SELF_TEST_LEAK2   //son240130 4:SELF_TEST_LEAK1,  5:SELF_TEST_LEAK2
				        &&  nCase!=SELF_TEST_LEAK_A  &&  nCase!=SELF_TEST_LEAK_B )    //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
				{ 
					nRet1=Sub71.OnPinAllReset1(); ///PIN ALL RESET  
					if( nRet1!=1){
						AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!"); 
						g_nSelfRunFlag=0;
						fclose(fp1); 
						return  ;
					}
				}


				if( card==m_nCardNoStart){
					if( nLoop >0){  //sylee20120308
						if(nLoop==1){ 
							cardPin=0;
						}
						nLoop--;
						goto Task1; 	
					}
				}


				if(nIso1==2){  //short
					//	if( Sub71.Y1>nIValue1){//fail log
					if( Sub71.Y1> nISOSet1 ){//fail log  0-> nISOSet1


						CardS[card+1][cardPin+1]=nProStep1;

						if(g_nErrCount1<MAX_FAIL_COUNT){//sylee160514-2	//hdpark221005 2999->MAX_FAIL_COUNT
							g_nErrCount1++;
							g_nErrCountTot++;
							StepErrCount++;
						}
						else{
							break;
						}

						dSelfErr1[g_nErrCount1][0]=(float)(card+1);  //  CARD no.

						if (nCase==SELF_TEST_OPEN                                       //son240130 1:SELF_TEST_OPEN, 
						        || nCase==SELF_TEST_SHORT2 || nCase==SELF_TEST_LEAK2    //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2, 
						        || nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B)  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
						{
							dSelfErr1[g_nErrCount1][1]= (float)(cardPin+1);  //vc  pin
						}
						else{
							dSelfErr1[g_nErrCount1][1]= (float)(cardPin+1);  //vc  pin  //sylee131123
						}			



						//vc  pin
						if (nCase==SELF_TEST_OPEN                                       //son240130 1:SELF_TEST_OPEN, 
						        || nCase==SELF_TEST_SHORT2 || nCase==SELF_TEST_LEAK2    //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2, 
						        || nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B)  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
						{
							dSelfErr1[g_nErrCount1][2]=(float)( cardPin+1+card*nCardPinMax) ;  //vc pin total  //	nCardPinMax=64;//sylee150210 //ACE380
						}
						// vb pin
						else if(nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_LEAK1  ) //son 2:SELF_TEST_SHORT1, 4:SELF_TEST_LEAK1
						{ 
							dSelfErr1[g_nErrCount1][2]=(float) (cardPin+1+card*nCardPinMax) ;//sylee131123  //	nCardPinMax=64;//sylee150210 //ACE380	
						}
						else{
							dSelfErr1[g_nErrCount1][2]=(float) (cardPin+1+card*nCardPinMax) ;  //	nCardPinMax=64;//sylee150210 //ACE380	
						}

						dSelfErr1[g_nErrCount1][3]=(float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4]=(float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5]=(float)Sub71.Y1;   //cardPin
						dSelfErr1[g_nErrCount1][6]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3];
						dSelfErr1[g_nErrCount1][7]=(float)nProStep1;//sylee131212

						nFail1++;		
					}

				}
				else//(nIso1==1  //open 
				{

					nFailflag=0; 

					if(nCase==SELF_TEST_OPEN)    //son240130 1:SELF_TEST_OPEN
					{
						if( (Sub71.Y1< nISOSet1) || (Sub71.X1<8)  ){ 
							nFailflag=1;
						}

						if(Sub71.X1<8){ 
							if( nEarthFlag==0){
								nEarthFlag=1;

								if( nG_Language!=2){//SYLEE150707
									//AfxMessageBox(" Error No 8027, \n\n   if all point is  (same voltage<8),   Earth!?,  All  Cable, pin block, jig,  etc. Check!?  \n\n   \n\n\n    , Self Test   Error!!!"); 
									str.Format("  , *> [문제 발생: 오픈측정 할수 없음]:   ");												 
									OnList1Mes1(str); 
									str.Format("  ,*> [조치방법]:   ");												 
									OnList1Mes1(str); 
									str.Format("  ,*>1)지그탈착하여 확인! 접지누설문제");	
									OnList1Mes1(str); 
									str.Format("  ,*>2)스위치카드를 교환하여 확인하십시요! ");
									OnList1Mes1(str); 
									str.Format("  ,*>3)스위치카드에 연결케이블을 분리확인! 접지누설문제");
									OnList1Mes1(str); 
								}
								else{
									str.Format("  , *> [ERROR : Can't open test]:   ");												 
									OnList1Mes1(str); 
									str.Format("  ,*> [check point]:   ");												 
									OnList1Mes1(str); 
									str.Format("  ,*>1) jig  unclamp check !  ground error?");	
									OnList1Mes1(str); 
									str.Format("  ,*>2) switch card change check ! ");
									OnList1Mes1(str); 
									str.Format("  ,*>3) switch card~pin block   cable  check!,  pin block particle check! ");
									OnList1Mes1(str); 


								}

							}
						}

					}
					else
					{
						if( Sub71.Y1< nISOSet1){ 
							nFailflag=1;
						}
					} 

					if( nFailflag==1){ //sylee20120512							 
						//if( Sub71.Y1< nISOSet1){ //sylee20120512 
						CardS[card+1][cardPin+1]=1;

						if(g_nErrCount1<MAX_FAIL_COUNT){//sylee160514-2	//hdpark221005 2999->MAX_FAIL_COUNT
							g_nErrCount1++;
							g_nErrCountTot++;
							StepErrCount++;
						}
						else{
							break;
						}

						dSelfErr1[g_nErrCount1][0]=(float)(card+1);  //  CARD no.

						dSelfErr1[g_nErrCount1][1]=(float)(cardPin+1) ;  //vb  pin
						dSelfErr1[g_nErrCount1][2]=(float)(cardPin+1+(card)*nCardPinMax);  //vb  pin  //	nCardPinMax=64;//sylee150210 //ACE380										 


						dSelfErr1[g_nErrCount1][3]=(float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4]=(float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5]=(float)Sub71.Y1;   //cardPin
						dSelfErr1[g_nErrCount1][6]=(float)Sub71.R1;   //R1;  //double dR1[2][5000][3];
						dSelfErr1[g_nErrCount1][7]=(float)nProStep1;//sylee131212

						nFail1++;


						if(nCase==SELF_TEST_OPEN)//sylee141019       //son240130 1:SELF_TEST_OPEN
                        {
							if(nOpenErrPinCount<200){
								if((Sub71.X1<2)&&(Sub71.Y1<2)){//sylee141019
									nOpenErrPinCount++;//sylee141019
									nOpenErrPin[nOpenErrPinCount][1]= (card+1);  //SWITCH CARD No.;//sylee141019
									nOpenErrPin[nOpenErrPinCount][2]= (cardPin+1);  //SWITCH CARD Pin No.//sylee141019
									nOpenErrPin[nOpenErrPinCount][3]= card*nCardPinMax+cardPin+1;  //	nCardPinMax=64;//sylee150210 //ACE380										 
									//son231222_4 nOpenErrPin[nOpenErrPinCount][4]= nPinSW_Net(card*nCardPinMax+cardPin+1);  //  BDL PIN 
									int hwPin0 = card*nCardPinMax+cardPin; //son231222_4
									nOpenErrPin[nOpenErrPinCount][4]= nJigPinMap[hwPin0] + 1; //son231222_4  BDL PIN 
								}
							}
						}

					}//sylee20120512


				}



				if(cardPin==127){
					if(nFail1>0){
						// fprintf(fp1, " \n  " );
						str.Format("  ,*  Error =>    ,  Switch Card=%d      ,   Fail=%d    ",  card+1 , nFail1);	
						OnList1Mes1(str);  
						//		 fprintf(fp1, str);
						//		 fprintf(fp1, " \n " );
					}
				}



				int nRet1;
				nRet1=0;

				if (nCase==SELF_TEST_LEAK1 || nCase==SELF_TEST_LEAK2          //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
				        || nCase==SELF_TEST_LEAK_A || nCase==SELF_TEST_LEAK_B //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B 
				        || nCase==SELF_TEST_VC_1PIN_RST )                     //son240130 8:SELF_TEST_VC_1PIN_RST
                {
					if( Sub71.Y1>500){
						Sub71.OnPinAllReset1(); ///PIN ALL RESET
						Sub71.Delay_100ns(1000);  //100us 
					}
				}


			}////for END   for( cardPin=0; cardPin<64; cardPin++)  //pin  vb pin



		} //for END  for( card=0; card< nCount; card++)  //count



		if(nVSet<=20)
		{
#if 1   //son240205 begin:
            if (nCase>=SELF_TEST_OPEN && nCase && nCase <=SELF_TEST_VB_1PIN_RST)
    		    str.Format("  * [  END :: Case %d ]  LV %s ", nCase, getStrSelfTestCase(nCase));
#endif  //son240205
			else {  
				str.Format("  * [  END :: else Case? LV %s ", getStrSelfTestCase(nCase));	    //son240205

			}

		}
		else 
		{
#if 1   //son240205 begin:
            if (nCase>=SELF_TEST_OPEN && nCase && nCase <=SELF_TEST_VB_1PIN_RST)
    		    str.Format("  * [  END :: Case %d ]  HV %s ", nCase, getStrSelfTestCase(nCase));
#endif  //son240205
			else {  
				str.Format("  * [  END :: else Case?   HV %s ", getStrSelfTestCase(nCase));	//son240205

			}
		}


		if( StepErrCount==0 ){
			str2.Format("  ***> PASS *");
		}
		else {
			str2.Format("  ***> FAIL *");
		}

		str=str+str2;

		OnList1Mes1(str);




task2:  //STOP JUMP POSTION


		Sub71.OnPinAllReset1();
		//SYLEE130814    Sub71.OnDisChargeuS(200);// sylee20120621  1->10	

		if( g_nStopFlag!=2 ){	//SYLEE130916	 
			Sub71.Delay_100ns(5000000);  //1000us 
		} 
		if( g_nStopFlag==2 ){	
			//sylee131218
			g_nSelfRunFlag=0;
			//	goto task3;
			break;
		} 

	}//   for  prostep








	//SYLEE131220 task3:  //STOP JUMP POSTION



	/////////////////////////////////////////////////////////////////////////
	//log


	int nProStep2Tmp;


	int err;
	for(nProStep2Tmp=SELF_TEST_LV_OPEN; nProStep2Tmp<= (SELF_TEST_MAX -1); nProStep2Tmp++ )    //son240130 1:SELF_TEST_LV_OPEN, 18:SELF_TEST_MAX
	{

		if( nProStep2Tmp<=10 ){
			nCase=nProStep2Tmp;
		}
		else{
			nCase=nProStep2Tmp-10;
		}


		if( nProStep2Tmp==1 ){
			fprintf(fp1, "         ,    No  , Net VbPin,  Net VcPin, * CardNo, SW VbPin, SW VcPin,  SW VbPinTotal, SW VcPinTotal,     VB,        VC,       I,            R,   \n ");	
		}  

		// for( m=0; m<1; m++){

		int dd,ePin1,ePin2,ntmp1;

		for( err=1; err<=g_nErrCount1; err++)//sylee20111215
		{

			dd=err%2;
			ntmp1=(int)dSelfErr1[g_nErrCount1][7];

			if(ntmp1!=nProStep2Tmp){//sylee131212
				continue;
			}

			// err=g_nErrCount1;//sylee20120703


#if 1   //son240205
            fprintf(fp1, "%s   ,", getStrSelfTestCase2(nProStep2Tmp));  //son240205
#endif  //son240205

			ePin1 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4

			if (nCase==SELF_TEST_OPEN) //son240130 1:SELF_TEST_OPEN
            {
				//ePin1=  nPinSW_Net((int)dSelfErr1[err][2]);  //son231222_4 
				//ePin2=  nPinSW_Net((int)dSelfErr1[err][2]);  //son231222_4
			    ePin2 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4	

				fprintf(fp1, "   %d,        %d,         %d,    *%3.0f,      %5.0f,     %5.0f,         %5.0f,         %5.0f,",
				        err,ePin1,ePin2,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][1],dSelfErr1[err][2] ,dSelfErr1[err][2]);
			}
			else  if (nCase==SELF_TEST_SHORT2)   //son240130 3:SELF_TEST_SHORT2
            {
				//son231222_4 ePin1=  nPinSW_Net((int)dSelfErr1[err][2]); 
				fprintf(fp1, "    %d,   *,  %d,  *%5.0f,           *,      %5.0f,        *,     %5.0f, ",
				        err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);
			}
			else if( nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_LEAK1 ) //son240130 2:SELF_TEST_SHORT1, 4:SELF_TEST_LEAK1
            {
				//son231222_4 ePin1=  nPinSW_Net((int)dSelfErr1[err][2]); 						 
				if(nProStep2Tmp==SELF_TEST_HV_LEAK1)    //son240130 14:SELF_TEST_HV_LEAK1
				{
					fprintf(fp1, "  %d, %d,   *, *%5.0f, %5.0f,  *, %5.0f, *,",err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);
				}
				else {
					fprintf(fp1, "   %d, %d,   *,      *%5.0f,        %5.0f,       *,    %5.0f,      *,",
					            err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);
				}
			}
			else if (nCase==SELF_TEST_LEAK2){     //son240130 5:SELF_TEST_LEAK2
				//son231222_4 ePin1=  nPinSW_Net((int)dSelfErr1[err][2]); 						 

				fprintf(fp1, "    %d, *,  %d,    *%5.0f,           *,      %5.0f,        *,     %5.0f, ",
				                err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);
				// fprintf(fp1, "             %d,          *%5.0f,           *,      %5.0f,        *,     %5.0f, ",
				//              err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*128),dSelfErr1[err][1] );
			}
			else  if (nCase==SELF_TEST_LEAK_A){ //son240130 6:SELF_TEST_LEAK_A
				//son231222_4 ePin1=  nPinSW_Net((int)dSelfErr1[err][2]); 
				fprintf(fp1, "     %d, %d,   *,   *%5.0f,       %5.0f,       *,     %5.0f,      *, ",
				                err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);				    
				//str2.Format( "           %d, %d,   *,   *%5.0f,       %5.0f,       *,     %5.0f,      *, ",
				//              err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);				    

				//fprintf(fp1, "           %d,        *%5.0f,       %5.0f,       *,     %5.0f,      *, ",
				//              err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*128),dSelfErr1[err][1] );
			}
			else  if (nCase==SELF_TEST_LEAK_B){   //son240130 7:SELF_TEST_LEAK_B
				//son231222_4 ePin1=  nPinSW_Net((int)dSelfErr1[err][2]); 
				fprintf(fp1, "       %d,    *, %d,    *%5.0f,           *,      %5.0f,       *,     %5.0f, ",
				                err,ePin1,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][2]);				   		   
				// fprintf(fp1, "           %d,        *%5.0f,           *,      %5.0f,       *,     %5.0f, ",
				//              err,dSelfErr1[err][0],dSelfErr1[err][1]-((dSelfErr1[err][0]-1)*128),dSelfErr1[err][1] );
			}
			else  if (nCase==SELF_TEST_VC_1PIN_RST){ //son240130 8:SELF_TEST_VC_1PIN_RST
				//								fprintf(fp1, "  %d,     %5.0f,     %5.0f,   %5.0f,  %5.0f, ",err, nPin[1] ,nPin[2],nPin[3],nPin[4]);
			}
			fprintf(fp1, " %10.2f,%10.2f,%10.2f,      %12.2f, ",dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5],dSelfErr1[err][6]);
			fprintf(fp1, " \n");


		} //for( err=1; err<=g_nErrCount1; err++)//sylee20111215

	}//

	//sylee131122 g_nErrCount1=0;//sylee20120721
	//===================================================================================


	Sub71.OnPinAllReset1(); ///PIN ALL RESET

	//SYLEE130814	 Sub71.OnDisChargeuS(100000);  // -> 100mS	

	Sub71.AOnPoweroff2();//sylee20120702  Sub71.AOnPoweroff();

	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704


	if( g_nStopFlag==2){

		//sylee131121	 g_nStopFlag=0;
		g_nSelfRunFlag=0;
		g_nSelfAutoFlag1=0;

		str.Format("  * Stop   [Self Test  ::  Switch Card  TR ]  ");	
		OnList1Mes1(str);
		AfxMessageBox(str); 		 	 
	} 


	if( g_nErrCountTot<1){
		fprintf(fp1, "    ***NO ERROR *** " );
	}
	else{
		fprintf(fp1, "    ***  TOTAL ERROR  = %d ea *** ",g_nErrCountTot );
	}


	///==========================================================================
	//ending
	fprintf(fp1, "  \n  \n   ************************************************* " );

	fclose(fp1); 






	//sylee131123 if( g_nSelfAutoFlag1==3){
	//  g_nSelfAutoFlag1=1;

	//}else{
	if( (g_nStopFlag!=2)&& (g_nErrCount1<1)){//SYLEE131121
		g_nSelfTestInterlock=1;//sylee131121   
		AfxMessageBox("    Self Test.   OK!!  ");//sylee131121 
	}
	if(g_nErrCount1>0)//sylee131121  
	{
		//sylee140130 ::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);


		if( SysSet13.m_nSet7==1 ){//sylee130724
			::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);		 
		}
		else{						
			::ShellExecute(NULL,"open","notepad.EXE",fName1,"NULL",SW_SHOWNORMAL);	   
		}


	}

	//}


	if( g_nStopFlag==2){
		g_nSelfTestInterlock=1;//sylee131212 import
		g_nStopFlag=0;//sylee131121
	}

       

	//==========================================
	//ENDING



	g_nAutoFlag=0;
	g_nSelfRunFlag=0;

	///////////////////////////////////////////////////
	if(nOpenErrPinCount>0)
	{
		strfName1.Format("d:\\log\\SelfError_EarthPin.CSV" );         //<=======================================
		DeleteFile(strfName1);
		::ZeroMemory(&fName1,sizeof(fName1)); 
		strcat( fName1 , strfName1 ); 
		fp1 = fopen(fName1,"wt"); 
		if(fp1 == NULL){ 
			str.Format("Error    d:\\log\\SelfError_EarthPin.CSV      file open   ?  Error, Saving Error !     file close!   " );
			AfxMessageBox( str, MB_OK); //error messege
			g_nSelfRunFlag=0;
			return  ;
		}			

		CTime curTime = CTime::GetCurrentTime();
		fprintf(fp1, " \n\n");	

		if( nG_Language!=2){//SYLEE150707
			str.Format(  " *   발생 시간 : %d년 %d월 %d일  %d시 %d분 %d초  ",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
			fprintf(fp1,str);	 fprintf(fp1, "  \n\n");  		 	
			str.Format( "  **>[문제 발생]: 문제 핀에 대해   오픈측정을  할수 없습니다.  ");
			fprintf(fp1,str);
			fprintf(fp1, " \n\n");	
			str.Format( "  **>[조치 방법]:   ");
			fprintf(fp1,str);
			fprintf(fp1, " \n");

			str.Format( "    1경우) 지그를 탈착합니다. 문제가 없어진다면  지그내부에 접지불량이 있습니다." );
			fprintf(fp1,str);    fprintf(fp1, " \n");
			str.Format( "    2경우) 문제 스위치 카드 박스에 전원이 인가 되었는지 확인하십시요.  LED 램프 ON 확인.   " );
			fprintf(fp1,str);  		  fprintf(fp1, " \n");
			str.Format( "    3경우) 스위치카드를 교체하여 정상화 되는지 확인합니다.  "  );
			fprintf(fp1,str);  		  fprintf(fp1, " \n");	 
			str.Format( "    4경우) 문제 핀 스위치 카드에서 핀 블록까지 연결된 케이블 연결 잭을 분리하여 문제가 없어진다면    " );
			fprintf(fp1,str);            fprintf(fp1, " \n");
			str.Format( "                연장된 케이블에서 핀블록까지  문제가 있습니다.    " );
			fprintf(fp1,str);            fprintf(fp1, " \n");
			str.Format( "                이와같이 연장된 부분을  구간 분리하면서 위치를  찾아 해결합니다. " );
			fprintf(fp1,str);            fprintf(fp1, " \n");
		}
		else{

			str.Format(  " *   발생 시간 : %d년 %d월 %d일  %d시 %d분 %d초  ",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
			fprintf(fp1,str);	 fprintf(fp1, "  \n\n");  		 	
			str.Format( "  **>[문제 발생]: 문제 핀에 대해   오픈측정을  할수 없습니다.  ");
			fprintf(fp1,str);
			fprintf(fp1, " \n\n");	
			str.Format( "  **>[check points]:   ");
			fprintf(fp1,str);
			fprintf(fp1, " \n");

			str.Format( "   case 1)  jig  unclamp check!" );
			fprintf(fp1,str);    fprintf(fp1, " \n");
			str.Format( "  case 2)  switch card power on check!.   " );
			fprintf(fp1,str);  		  fprintf(fp1, " \n");
			str.Format( "   case 3) switch card  change check! "  );
			fprintf(fp1,str);  		  fprintf(fp1, " \n");	 
			str.Format( "   case 4)  switch card ~  pin block   cable  check!        " );
			fprintf(fp1,str);            fprintf(fp1, " \n");
			str.Format( "           cable ?         " );
			fprintf(fp1,str);            fprintf(fp1, " \n");
			str.Format( "           pin block pin short?  particle? " );
			fprintf(fp1,str);            fprintf(fp1, " \n");

		}


		fprintf(fp1, " \n\n\n");

		for( int i1=1; i1<=nOpenErrPinCount; i1++){
			str.Format( "  **>문제:   지그핀번호=%5d    (스위치카드번호 =%4d  :  스위치카드핀번호 =%5d)  ",nOpenErrPin[i1][4],nOpenErrPin[i1][1],nOpenErrPin[i1][2] );
			//	  str1.Format( " *>문제: 지그핀번호=%d (스위치카드번호=%d) ",nOpenErrPin[i1][4],nOpenErrPin[i1][1]);
			fprintf(fp1,str);
			fprintf(fp1, " \n");
			//	  OnList1Mes1(str1);  
		}

		if( nOpenErrPinCount==200){
			fprintf(fp1, " \n\n");
			if( nG_Language!=2){//SYLEE150707
				str.Format( "  **> 문제 200까지만  확인후 중단되었습니다.    더 있을수 있습니다.  " );
			}else{
				str.Format( "  **>  error count limited  200ea.   it's over " );
			}

			fprintf(fp1,str);
			fprintf(fp1, " \n");		
		}

		fclose(fp1); 
		if( SysSet13.m_nSet7==1 ){//sylee130724
			::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);		 
		}
		else{						
			::ShellExecute(NULL,"open","notepad.EXE",fName1,"NULL",SW_SHOWNORMAL);	   
		}
	}

	str.Format("  ***  IO Check  Finish  ***   ");										 
	OnList1Mes1(str);


	if( nStop==2){
		g_nSelfTestInterlock=1;//sylee131212
		AfxMessageBox("   :  Switch Card 2W Pin Check.   STop!!!"); 	 
	}


	return;
}










void CChildView8::OnButton202_StopAutoSelf() //stop //son230302
{
	 
   g_nStopFlag=2;
   Sleep(500);//sylee140702

	return ;
}


/*
void CChildView8::OnButton203() //set
{
	 CModeSet11 ModeSet11;
     ModeSet11.DoModal();

 	return ;
}

  */


/*void CChildView8::OnButton203() //set
{
 


 	return ;
}
*/






/*void CChildView8::OnButton204() //end temp
{
	 
	return ;
}
*/
 
 


void CChildView8::OnButton302_ClearMessage() //message Clear //son230302
{	 
    m_list1.ResetContent();

	return ;
}
 



void CChildView8::OnCheck1()  
{
	if(m_check1.GetCheck())
	{
	//	nCycleAuto1  = 1;

 

//		m_check2.SetCheck(nCycleAuto2);

	}
	else
	{



 
	}
//	m_check1.SetCheck(nCycleAuto1);

//	nLoopCount1 =0;


 
}


void CChildView8::OnCheck2()  
{
	if(m_check2.GetCheck())
	{
	//	nCycleAuto1  = 1;

	}
	else
	{
 
	}

}

void CChildView8::OnCheck3()  
{
	if(m_check3.GetCheck())
	{
	//	nCycleAuto1  = 1;

	}
	else
	{
 
	}

}
void CChildView8::OnCheck4()  
{
	if(m_check4.GetCheck())
	{
	//	nCycleAuto1  = 1;

	}
	else
	{
 
	}

}
void CChildView8::OnCheck5()  
{
	if(m_check5.GetCheck())
	{
	//	nCycleAuto1  = 1;

	}
	else
	{
 
	}

}
void CChildView8::OnCheck6()  
{
	if(m_check6.GetCheck())
	{
	//	nCycleAuto1  = 1;

	}
	else
	{
 
	}

}

void CChildView8::OnCheck7()  
{
	if(m_check7.GetCheck())
	{
	//	nCycleAuto1  = 1;

	}
	else
	{
 
	}

}


/* 
void CChildView8::OnCheck8()  
{

	if(m_check8.GetCheck())
	{
	  	nDischarge1  = 1;
 	}
	else
	{
	 	nDischarge1  = 0;
 	}
 	 m_check8.SetCheck(nDischarge1);
		 
}

*/
 
/*
void CChildView8::OnCheck10()  
{

	if(m_check10.GetCheck())
	{
	  	nAllPinSearchPlag  = 1;
 	}
	else
	{
	 	nAllPinSearchPlag  = 0;
 	}
 	 m_check10.SetCheck(nAllPinSearchPlag);
		 
}

*/




int CChildView8::AutoSelf_PowerModeSet(int m_nMode1) 
{


	int nFil,nRet1,Lo1;
	__int64   tStart1, tStart2 ; 
	double dVbR1,dVbR2 ;
	double nT[10];
	int nIFilter,nHVOffSet,nHVGain;//sylee130629

	CString str;


	//	double V1old;//sylee150210 //ACE380

	//    V1old=0.0;//sylee150210 //ACE380



	memset(nT, 0x00, sizeof(nT));


	if(m_nMode1!=dRec[m_nMode1][0]){
		AfxMessageBox(" Error No 8516, \n   Self Test Error    Parameter loading Error  V I Delay");
		return 0;
	}

	nCC= (int)dRec[m_nMode1][1];
	nVRel= (int)dRec[m_nMode1][2];   
	nVSet=  dRec[m_nMode1][3];
	// nVSet= (int)40;
	nIR=  (int)dRec[m_nMode1][4];
	nISet= dRec[m_nMode1][5];

	nFil=  (int)dRec[m_nMode1][8];  //    1=VC FAST, 2= VC SLOW ,  3=HV OPEN, ( 4=4W )
	nTimeDelay1=(int)(dRec[m_nMode1][6]*10);//syle20120104     SETUPCALIBRATION

	nIFilter = (int)dRec[m_nMode1][9];   //SYLEE130629		  
	nHVOffSet =(int) dRec[m_nMode1][10]; //SYLEE130629
	nHVGain = (int)dRec[m_nMode1][11];	//SYLEE130629	 
	///***************************************************
	/*
	   if( nVSet>20){
	//nFil= (int) dRec[m_nMode1][8];
	nFil= 1;//slow speed 
	}else{
	nFil= 0;//fast speed
	}
	*/
	nTimeDelay1=(int) (dRec[m_nMode1][6]*10);//syle20120104     SETUPCALIBRATION

	//ADC COUNT
	Sub71.nADCount1 =(int)dRec[m_nMode1][7];

	if(Sub71.nADCount1<1 ){
		Sub71.nADCount1=1;
	}

	if( Sub71.nADCount1>100){
		Sub71.nADCount1=100;
	}

	nRet1=Sub71.AOnPoweroff2();//sylee20120702 Sub71.AOnPoweroff();


	if(nRet1!=1){
		//	fclose(fp);
		//	fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 8316, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
		g_nSelfRunFlag=0;
		return  0;
	}


	if( nVRel==1){
		nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
	}else  if( nVRel==2){
		nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
	}else  if( nVRel==3){
		nRet1=Sub71.AOnVSetMulti1(0,0,nVSet,0) ;
	}else  if( nVRel==4){
		nRet1=Sub71.AOnVSetMulti1(0,0,0,nVSet) ;
	} 


	if( nRet1!=1){
		//	fclose(fp);
		//	fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 1316, \n   V SET ERROR !,   SW TEST Run Start Error!!!"); 
		g_nSelfRunFlag=0;
		return 0;
	}


	Sub71.Delay_100ns(10000000);  //1 sec  //SYLEE20111214 

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	tStart1=GetMilSecond2();



	Lo1=3;

Task101:  

	Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

	if(nVSet<5){
		dVbR2=nVSet*(0.35);//10%
	}else{
		dVbR2=nVSet*(0.1);//5%
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
		g_nSelfRunFlag=0;
		//	return ;
	}

	if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){

		if(Lo1-->0){
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}

		str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		//	return ;
	}




	//SYLEE151020     Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);  // vb relay on 기능   leakage발생 부분   *************************
	//SYLEE151020	   Sub71.Delay_100ns(3000000);  //300ms 


	//SYLEE130814  Sub71.OnDisChargeuS(1000);  // -> 1000us 	 	 


	//SYLEE130618   Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET



	///////////////////////////////////////////////////////////////////////////////////////////////
	// V1old=Sub71.V1;//sylee150210 //ACE380

	if( nFil==1 ||  nFil==2){//SYLEE130628
		Sub71.AOnReadV1Enable1(nVRel); 
	}else{
		Sub71.AOnReadV1Enable1(5);
	}

	//Sub71.V1=V1old;//sylee150210 //ACE380


	if(nFil==3){
		//sylee140226	Sub71.HV_OffSet(nHVOffSet); 
		//sylee140226	Sub71.HV_Gain(nHVGain);	
		Sub71.HV_OffSet_Gain(nHVOffSet, nHVGain);    //sylee140226
	}


	Sub71.FilterSet(nFil);//sylee130628 
	if(nIFilter==3){
		AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
		nIFilter=1;
	}
	Sub71.I_Filter(nIFilter);//sylee130629



	tStart2=GetMilSecond2();	 
	nT[0]=(double)(tStart2-tStart1)/1000.;  //test


	Sub71.OnPinAllReset1(); ///PIN ALL RESET

	//SYLEE130814	 Sub71.OnDisChargeuS(100000);  // -> 100mS		



	Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);    //SYLEE151020   MOVE

	Sub71.Delay_100ns(3000000);  //300ms   //SYLEE151020   MOVE

	return  1;

}





void CChildView8::OnButton111_VBSet() // VB SET    //SYLEE20120720 //son230302
{
	int nFil,nRet1,Lo1,nIFilter,nIGain;
	double dVbR1,dVbR2; 
	CString str;

	//Sub71.OnDisCharge1(); //sylee170725-3 ray

	if(g_nSelfRunFlag>0){
		// AfxMessageBox("   Running.        Selftest  4W  Relay Check  Start Error  !!!"); 
		return;
	}

	g_nSelfRunFlag=1;

	str.Format("ON");
	m_Label21.SetCaption(str);

	nCC= 1;
	nVRel= 1;   
	nVSet= 10; 
	nIR= 2 ;	 //sylee151007  //ACE400
	nISet= 1;   //sylee151007  //ACE400
	nIGain = 1;	    //sylee151207 //ACE400
	nFil= 1; //sylee151007  //ACE400 	 
	Sub71.nADCount1 = 10 ;
	nIFilter=1;	

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//INITAL 부 

	nRet1=Sub71.AOnPoweroff2();//sylee20120702 
	if(nRet1!=1){	 
		AfxMessageBox(" Error No 8310, \n   Power all off fail !,  \n\nSelf Test,  Pin Test,  "); 
		g_nSelfRunFlag=0;
		return  ;
	}

	Sub71.OnDisCharge1(); //sylee170725-3 ray

	if( nVRel==1){
		nRet1=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
	}else  if( nVRel==2){
		nRet1=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
	}else  if( nVRel==3){
		nRet1=Sub71.AOnVSetMulti1(0,0,nVSet,0) ;
	}else  if( nVRel==4){
		nRet1=Sub71.AOnVSetMulti1(0,0,0,nVSet) ;
	}  

	if( nRet1!=1){	 
		//	fclose(fp1); 
		AfxMessageBox(" Error No 1317, \n   V SET ERROR !,   \n\n  Self Test,  Pin Test,     "); 
		g_nSelfRunFlag=0;
		return ;
	}

	Sub71.Delay_100ns(10000000);  //1 sec  


	if( nIGain==2){////sylee151207 //ACE400
		Sub71.I_Gain(2);  //10x  =io on		
	}else{
		Sub71.I_Gain(1);  //1x  =io off
	}


	Sub71.Delay_100ns(3000000);  //300ms //sylee151207 //ACE400

	Lo1=300;



Task101: 


	Sub71.AOnReadVB1(nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

	if(nVSet<5){
		dVbR2=nVSet*(0.35);//10%
	}else{
		dVbR2=nVSet*(0.1);//5%
	}
	dVbR1=nVSet-dVbR2;
	dVbR2=nVSet+dVbR2;

	if( Sub71.V1>300){
		if(Lo1-->0){
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}
		str.Format("Error No 1312,\n \n\n  Power Check? \n\n\n    Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%f.2, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)g_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		g_nSelfRunFlag=0;

	}

	if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2){
		if(Lo1-->0){
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}
		str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),",(int)g_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
		AfxMessageBox(str);	
		//	return ;
	}



	Sub71.AOnReadV1Enable1(nVRel); //SYLEE20120223   //VC SENSE INIT SET

	Sub71.FilterSet(nFil);//sylee130628 
	Sub71.I_Filter(nIFilter); //sylee130704
	Sub71.OnPinAllReset1(); ///PIN ALL RESET			

	Sub71.AOnISetOn1(nCC, nVRel, nVSet, nIR, nISet);  //sylee150210 move 
	Sub71.Delay_100ns(3000000);  //300ms   //sylee150210 move	

	g_nSelfRunFlag=0;

	return ;
}




void CChildView8::OnButton112_Read() // READING //son230302
{
	

	CString str;
 
	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        Selftest  4W  Relay Check  Start Error  !!!"); 
        return;
	}

	g_nSelfRunFlag=1;



	Sub71.AOnReadV1I1(1);

//Sub71.R1  Sub71.Y1
 
//SYLEE151007 //ACE400  	str.Format("I=%.2f",Sub71.Y1);	
	str.Format("I=%.2f",Sub71.Y1*1000);	//SYLEE151007 //ACE400  
	m_Label22.SetCaption(str);

	str.Format("V=%.2f",Sub71.X1);  //SYLEE151007 //ACE400  ???????????
	m_Label24.SetCaption(str);

	g_nSelfRunFlag=0;

 	return ;
}







void CChildView8::OnButton121_SetVbPin() //VB PIN SET//SYLEE20120721 //son230302
{

	char szText[50];
    int nVbPin ;


	if(g_nSelfRunFlag>0){
	  //  AfxMessageBox("   Running.        VB PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;



	m_edit13.GetWindowText( szText,10); 
	nVbPin = atoi( szText )-1;

    if(nVbPin<0 ){
		nVbPin=0;
		AfxMessageBox(" nVbPin  <0 .    Error !!!"); 
	}

    if(nVbPin>32768){
		nVbPin=0;  
		AfxMessageBox(" nVbPin  > 32768 .  Error !!!"); 
	}


 
    //int ret = Sub71.AOnPinSet1(2, nVbPin, 1);  // =1,VB,    =2VC, 	
    int ret = Sub71.AOnPinSet1(2, nVbPin, MOD_TR_SET_VB);  // =1,VB,    =2VC, 	//son210220 enum으로 변경
    if (ret < 0)
	{
		CString str;
		str.Format("Error. Can't find hwPin for nVbPin(%d)!!!", nVbPin); 
		AfxMessageBox(str); 
		nVbPin=0;  
		g_nSelfRunFlag=0;
		return;
	}


    g_nSelfRunFlag=0;


	return ;
}


void CChildView8::OnButton122_SetVcPin() //VC PIN 등록  //SYLEE20120721 //son230302
{

	char szText[50];
    int nVcPin ;



	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        VC PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;



	m_edit15.GetWindowText( szText,10); 
	nVcPin = atoi( szText )-1;

    if(nVcPin<0 ){
		nVcPin=0;
		AfxMessageBox(" nVcPin  <0 .    Error !!!"); 
	}

    if(nVcPin>32768){
		nVcPin=0;  
		AfxMessageBox(" nVcPin  > 32768 .  Error !!!"); 
	}



    //int ret = Sub71.AOnPinSet1(2, nVcPin, 2);   //=1,VB,    =2VC, 
    int ret = Sub71.AOnPinSet1(2, nVcPin, MOD_TR_SET_VC);   //=1,VB,    =2VC, //son210220 enum으로 변경
    if (ret < 0)
	{
		CString str;
		str.Format("Error. Can't find hwPin for nVbPin(%d)!!!", nVcPin); 
		AfxMessageBox(str); 
		nVcPin=0;  
		g_nSelfRunFlag=0;
		return;
	}

	g_nSelfRunFlag=0;

	return ;
}



void CChildView8::OnButton123_ResetPin() //ALL PIN RESET   //SYLEE20120721 //son230302
{

	if(g_nSelfRunFlag>0){
	//   AfxMessageBox("   Running.        ALL PIN RESET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;

    Sub71.OnPinAllReset1();  





	g_nSelfRunFlag=0;

	return ;
}

/*void CChildView8::OnButton124_PinVBOff() //VB PIN RESET  //SYLEE20120721 //son230302
{

	if(g_nSelfRunFlag>0){
	//    AfxMessageBox("   Running.        VB PIN RESET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;

	Sub71.OnPinResetVB();  

	g_nSelfRunFlag=0;

	return ;
}

void CChildView8::OnButton125_VBPinInfo() //VC PIN RESET    //SYLEE20120721 //son230302
{

	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        VC PIN RESET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;


	Sub71.OnPinResetVC();  

	g_nSelfRunFlag=0;

	return ;
}
 */

void CChildView8::OnButton124_PinVBOff() //VB POWER OFF //son230302
{


	CString str;
	g_nSelfRunFlag=1;
    str.Format("OFF");
    m_Label21.SetCaption(str);


    Sub71.AOnPoweroff2();

	g_nSelfRunFlag=0;
 
	return ;
}
 

void CChildView8::OnButton125_VBPinInfo() //VB pin info //son230302
{


	char szText[50];
    int nVbPin ;

	CString str;

	int lA,lB,lC,lD,lE;


	if(g_nSelfRunFlag>0){
	  //  AfxMessageBox("   Running.        VB PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;



	m_edit13.GetWindowText( szText,10); 
	nVbPin = atoi( szText )-1;

    if(nVbPin<0 ){
		nVbPin=0;
		AfxMessageBox(" nVbPin  <0 .    Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}

    if(nVbPin>32768)
    {
		nVbPin=0;  
		AfxMessageBox(" nVbPin  > 32768 .  Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}


 
    //Sub71.AOnPinSet1(2, nVbPin, MOD_TR_SET_VB);  // =1,VB,    =2VC, 	//son210220 enum으로 변경

	lA=PinMap[2][nVbPin];//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
#if 1
	if (lA < 0)			//son-32KPIN-20200113
	{
		str.Format("Error. Can't find hwPin for nVbPin(%d)!!!", nVbPin); 
		AfxMessageBox(str); 
		nVbPin=0;  
		g_nSelfRunFlag=0;
		return;
	}
#endif

    lB=lA/128;
	lC=lA-lB*128;

	if(lC>=64){
		lE=1;
		lD=lC-64;
	}else{
		lE=0;
	    lD=lC;
	}


  //  Sub71.AOnPinSet1(2, nVcPin, MOD_TR_SET_VC);   //=1,VB,    =2VC, //son210220 enum으로 변경



	str.Format(" <  Switch Box  >  \n\n * Card No.=%d (1~),  \n\n * Pin No.=%d(1~128) =%d+%d    ",lB+1,lC+1,lD+1 , lE*64 ); 
	AfxMessageBox(str);


    g_nSelfRunFlag=0;


 
	return ;
}
 
void CChildView8::OnButton128_VCPinInfo() //Vc pin info //son230302
{

	char szText[50];
    int nVcPin ;

	CString str;

	int lA,lB,lC,lD,lE;

	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        VC PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;



	m_edit15.GetWindowText( szText,10); 
	nVcPin = atoi( szText )-1;

    if(nVcPin<0 ){
		nVcPin=0;
		AfxMessageBox(" nVcPin  <0 .    Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}

    if(nVcPin>32768){
		nVcPin=0;  
		AfxMessageBox(" nVcPin  > 32768 .  Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}

 
	lA=PinMap[2][nVcPin];//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
#if 1
	if (lA < 0)			//son-32KPIN-20200113
	{
		str.Format("Error. Can't find hwPin for nVcPin(%d)!!!", nVcPin); 
		AfxMessageBox(str); 
		nVcPin=0;  
		g_nSelfRunFlag=0;
		return;
	}
#endif
    lB=lA/128;
	lC=lA-lB*128;

	if(lC>=64){
		lE=1;
		lD=lC-64;
	}else{
		lE=0;
		lD=lC;
	}

  //  Sub71.AOnPinSet1(2, nVcPin, MOD_TR_SET_VC);   //=1,VB,    =2VC, //son210220 enum으로 변경

	str.Format(" <  Switch Box  >  \n\n * Card No.=%d (1~),  \n\n * Pin No.=%d(1~128) =%d+%d    ",lB+1,lC+1,lD+1 , lE*64 );
	AfxMessageBox(str);

 	g_nSelfRunFlag=0;


	return ;
}


void CChildView8::OnButton701_NetPinInfo() //net pin info //son230302
{

	char szText[50];
    int nPin,nPin2 ;
	CString str;
	int lA,lB,lC ;
//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380

 //ACE380//////////////////////////////////////////////////////////////
//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380
//ACE380//////////////////////////////////////////////////////////////

    Get_nCardPinMax();//sylee150903 //ACE400     //son231227

    nPin2=-1;


	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        VC PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;



	m_edit11.GetWindowText( szText,10); 
	nPin = atoi( szText )-1;

    if(nPin<0 ){
		nPin=0;
		AfxMessageBox(" nPin  <0 .    Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}

    if(nPin>32768){
		nPin=0;  
		AfxMessageBox(" nPin  > 32768 .  Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}

    for( int i=0;i<=32768;i++){
		if( nPin==PinMap[0][i]){
			nPin2=i;
			break;
		}
	}


	if(	nPin2==-1){
     	str.Format("    *  Search Error  . Set Again!. \n\n     [Pin]  SET Check ex,QD,SD,DD,    Z, H,     Reverse, Normal  "   );
	    AfxMessageBox(str);
	    g_nSelfRunFlag=0;
	    return ;
	}

    
	lA=PinMap[0][nPin2];//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
    lB=PinMap[1][nPin2];
	lC=PinMap[2][nPin2];
#if 1
	if (lC < 0)			//son-32KPIN-20200113
	{
		str.Format("Error. Can't find hwPin for nPin2(%d)!!!", nPin2); 
		AfxMessageBox(str); 
		g_nSelfRunFlag=0;
		return;
	}
#endif

 
     int k1,k2,k3;


	 if(SysSet12.m_nPinBoxMax==1){  //2k  //sylee121005
		k1=lC/2048+1;
		k2=(lC%2048)/nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
		k3=(lC%2048)%nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
	 }else{
		k1=lC/4096+1;
		k2=(lC%4096)/nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
		k3=(lC%4096)%nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
	 }

 

	str.Format("   NO 8001 * NET  =%d (1~),  \n\n * DD .=%d(1~ ) , \n\n SWITCH =%d(1~ ),box=%d,card=%d,pin=%d ,    ",lA+1,lB+1 ,lC+1 ,k1,k2,k3 );
	AfxMessageBox(str);



//	str.Format("    * NET  =%d (1~),  \n\n * QD .=%d(1~ ) , \n\n SWITCH =%d(1~ ) ,    ",lA+1,lB+1 ,lC+1  );
//	AfxMessageBox(str);


 	g_nSelfRunFlag=0;



	return ;
}


void CChildView8::OnButton702_DDPinInfo() //net pin info //son230302
{


	char szText[50];
    int nPin,nPin2 ;

	CString str;

	int lA,lB,lC ;

//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380

 //ACE380//////////////////////////////////////////////////////////////
//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380
//ACE380//////////////////////////////////////////////////////////////
    Get_nCardPinMax();//sylee150903 //ACE400  //son231227

    nPin2=-1;


	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        VC PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;



	m_edit12.GetWindowText( szText,10); 
	nPin = atoi( szText )-1;

    if(nPin<0 ){
		nPin=0;
		AfxMessageBox(" nPin  <0 .    Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}

    if(nPin>32768){
		nPin=0;  
		AfxMessageBox(" nPin  > 32768 .  Error !!!"); 
		g_nSelfRunFlag=0;
		 return;
	}


    //son nPin(pinBlockPin: switchPin)에 맞는 jigPin(netPin)값을 찾아낸다.
    for( int i=0;i<=32768;i++){
		if( nPin==PinMap[1][i]){
			nPin2=i;
			break;
		}

	}
	
	if(	nPin2==-1){
     	str.Format("    *  Search Error  . Set Again!. \n\n     [Pin]  SET Check ex,QD,SD,DD,    Z, H,     Reverse, Normal  "   );
	    AfxMessageBox(str);
	    g_nSelfRunFlag=0;
	    return ;
	}

   
	lA=PinMap[0][nPin2];//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
    lB=PinMap[1][nPin2];
	lC=PinMap[2][nPin2];
#if 1
	if (lC < 0)			//son-32KPIN-20200113
	{
		str.Format("Error. Can't find hwPin for nPin2(%d)!!!", nPin2); 
		AfxMessageBox(str); 
		g_nSelfRunFlag=0;
		return;
	}
#endif


    int k1,k2,k3;



	if(SysSet12.m_nPinBoxMax==1){  //2k //sylee150210 		
			k1=lC/2048+1;
			k2=(lC%2048)/nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
			k3=(lC%2048)%nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380  
	 }else{		
			k1=lC/4096+1;
			k2=(lC%4096)/nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
			k3=(lC%4096)%nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380  
	 }


	str.Format("   NO 8002 * NET  =%d (1~),  \n\n * DD .=%d(1~ ) , \n\n SWITCH =%d(1~ ),box=%d,card=%d,pin=%d ,    ",lA+1,lB+1 ,lC+1 ,k1,k2,k3 );
	AfxMessageBox(str);


 	g_nSelfRunFlag=0;


	return ;
}

 
 
 
 
void CChildView8::OnButton703_SWPinInfo() //net pin info //son230302
{

	CString str;
	char szText[50];
    int nPin,nPin2 ;
	int lA,lB,lC ;
//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380

 //ACE380//////////////////////////////////////////////////////////////
//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380
//ACE380//////////////////////////////////////////////////////////////
    Get_nCardPinMax();//sylee150903 //ACE400     //son231227
    nPin2=-1;

	if(g_nSelfRunFlag>0){
	 //   AfxMessageBox("   Running.        VC PIN  SET   Start Error  !!!"); 
        return;
	}
	g_nSelfRunFlag=1;


	m_edit59.GetWindowText( szText,10); 
	nPin = atoi( szText )-1;

    if(nPin<0 ){
		nPin=0;
		AfxMessageBox(" nPin  <0 .    Error !!!"); 
		g_nSelfRunFlag=0;
		return;
	}

    if(nPin>32768){
		nPin=0;  
		AfxMessageBox(" nPin  > 32768 .  Error !!!"); 
		g_nSelfRunFlag=0;
		return;
	}

    //son nPin(pinBlockPin: switchPin)에 맞는 jigPin(netPin)값을 찾아낸다.
    for( int i=0;i<=32768;i++){
		if( nPin==PinMap[2][i]){
			nPin2=i;
			break;
		}
	}
	
	if(	nPin2==-1){
     	str.Format("    *  Search Error  . Set Again!. \n\n     [Pin]  SET Check ex,QD,SD,DD,    Z, H,     Reverse, Normal  "   );
	    AfxMessageBox(str);
	    g_nSelfRunFlag=0;
	    return ;
	}

 
	lA=PinMap[0][nPin2];//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
    lB=PinMap[1][nPin2];
	lC=PinMap[2][nPin2];
#if 1
	if (lC < 0)			//son-32KPIN-20200113
	{
		str.Format("Error. Can't find hwPin for nPin2(%d)!!!", nPin2); 
		AfxMessageBox(str); 
		g_nSelfRunFlag=0;
		return;
	}
#endif

 
    int k1,k2,k3;

	if(SysSet12.m_nPinBoxMax==1){  //2k //sylee150210
		k1=lC/2048+1;
		k2=(lC%2048)/nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
		k3=(lC%2048)%nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
	 }else{	
		k1=lC/4096+1;
		k2=(lC%4096)/nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
		k3=(lC%4096)%nCardPinMax+1;  //nCardPinMax;//sylee150210 //ACE380
	 }

	str.Format("   NO 8003 * NET  =%d (1~),  \n\n * DD .=%d(1~ ) , \n\n SWITCH =%d(1~ ),box=%d,card=%d,pin=%d ,    ",lA+1,lB+1 ,lC+1 ,k1,k2,k3 );
	AfxMessageBox(str);

//	str.Format("  NO 8001,   * NET  =%d (1~),  \n\n * QD .=%d(1~ ) , \n\n SWITCH =%d(1~ ) ,    ",lA+1,lB+1 ,lC+1  );
//	AfxMessageBox(str);


 	g_nSelfRunFlag=0;



	return ;
}
 



 
#if 0 //son231222_4 ChildView5::nPinSW_Net() 함수 삭제하고 nJigPinMap[hwPin0] + 1로  대체. 속도개선  

int  CChildView8::nPinSW_Net(int SW)//sylee131117   sw->net
{

	CString str;
//	char szText[50];
    int nPin,nPin2 ;
	int lA,lB,lC ;
//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380
 
//ACE380//////////////////////////////////////////////////////////////
//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380
//ACE380//////////////////////////////////////////////////////////////
    Get_nCardPinMax();//sylee150903 //ACE400 //son231227

	nPin = SW;

    if(nPin<0 ){	 
		return -1;
	}
    if(nPin>32768){		 
		return -2;
	}

    //son nPin(pinBlockPin: switchPin)에 맞는 jigPin(netPin)값을 찾아낸다.
    for( int i=0;i<=32768;i++){
		if( nPin==PinMap[2][i]){
			nPin2=i;
			break;
		}
	}
	
	if(	nPin2<0){//sylee131123
     //	str.Format("    *  Search Error  . Set Again!. \n\n     [Pin]  SET Check ex,QD,SD,DD,    Z, H,     Reverse, Normal  "   );
	  //  AfxMessageBox(str);
	    return -3;
	}

 
	lA=PinMap[0][nPin2];//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
    lB=PinMap[1][nPin2];
	lC=PinMap[2][nPin2];
#if 1
	if (lC < 0)			//son-32KPIN-20200113
	{
		str.Format("Error. Can't find hwPin for nPin2(%d)!!!", nPin2); 
		AfxMessageBox(str); 
		return -1;
	}
#endif
 
    int k1,k2,k3;

	if(SysSet12.m_nPinBoxMax==1){  //2k //sylee150210
		k1=lC/2048+1;
		k2=(lC%2048)/nCardPinMax+1;//nCardPinMax;//sylee150210 //ACE380
		k3=(lC%2048)%nCardPinMax+1;//nCardPinMax;//sylee150210 //ACE380
	 }else{	
		k1=lC/4096+1;
		k2=(lC%4096)/nCardPinMax+1;//nCardPinMax;//sylee150210 //ACE380
		k3=(lC%4096)%nCardPinMax+1;//nCardPinMax;//sylee150210 //ACE380 
	 }


//	str.Format("   NO 8003 * NET  =%d (1~),  \n\n * QD .=%d(1~ ) , \n\n SWITCH =%d(1~ ),box=%d,card=%d,pin=%d ,    ",lA+1,lB+1 ,lC+1 ,k1,k2,k3 );
//	AfxMessageBox(str);

	//sylee131123return lA+1;

	return lA;

}
#endif

//--------------------------------------------------------------------------------
//son nHW_CardPin, nCardPinMax을  SysSet12.m_nPinBoxMax와 SysSet13.m_nPinBlockType 에 맞게 설정
//--------------------------------------------------------------------------------
//son Switch 카드가 64pin인지 128 pin인지 확인
//    nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 또는 128) 값을 설정한다. 

//son231227 CChildView8::nHW_CardPin_Check() -> Get_nCardPinMax() 로 이름 변경
//          ChildView5, ChildView8 에 이중으로 선언된 nHW_CardPin_Check()를 ChildView8의 전역함수로 하나로 통일 
//int  CChildView8::nHW_CardPin_Check()//sylee150903  
int  Get_nCardPinMax()   //son231227
{
    //son 2K box 인 경우  (
	if(SysSet12.m_nPinBoxMax==1){//sylee150903  2k  //ACE400
		nHW_CardPin=2;//sylee150903-1    2=64pin   0,1 =128
		nCardPinMax=64;//sylee150903-1//ACE380
	}
	//son 4K box 인 경우
	else{
		nHW_CardPin=0;//sylee150903-1    2=64pin   0,1 =128
		nCardPinMax=128;//sylee150903-1//ACE380
	}

	//sylee161115 //sylee171109-1  //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
	//if( SysSet13.m_nPinBlockType==4 || SysSet13.m_nPinBlockType==6 ) { 
	if( SysSet13.m_nPinBlockType==PIN_BLK_S100_2_WIRE 
			|| SysSet13.m_nPinBlockType==PIN_BLK_4K_DD_421 ) //son-32KPIN-20190902 
	{ 
	    //son S100 인 경우: SD 2box
		nHW_CardPin=0;//sylee150903-1  //ACE400  2=64pin   0,1 =128
		nCardPinMax=128;//sylee150903-1 //ACE400//ACE380
	}




	return 1;
}

//son 이 Test 함수로 만든 출력 결과와 OnPinConvert()에서 나온 PinMap 과 비교해서 맞는지 확인해서 
//    Service SelfTest의 card, cardPin 구조의 pinMap과  OnPinConvert()의 PinMap이 일치하는지 확인한다. 
void CChildView8::OnButton2_Print_SrvSelfPinMap() //son240123 
{
	// TODO: Add your control notification handler code here
	

	char szText[50];
	

    //son nHW_CardPin, nCardPinMax을  SysSet12.m_nPinBoxMax와 SysSet13.m_nPinBlockType 에 맞게 설정
	Get_nCardPinMax(); //sylee150903 //ACE400        //son231227

	//----------------------------
	//son Read m_nCardNoStart 
	m_edit1.GetWindowText( szText,10); 
	m_nCardNoStart = atoi( szText );	


	//----------------------------
	//son Read m_nCardNoEnd 
	m_edit2.GetWindowText( szText,10); 
	m_nCardNoEnd = atoi( szText );


	m_nCardNoStart =m_nCardNoStart-1;   //son 0 base로 수정
	m_nCardNoEnd =m_nCardNoEnd-1;

//son240118_2 begin:
    CString strTemp;
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];
    ::ZeroMemory(&fName, sizeof(fName));
    strTemp.Format("d:\\log\\UserCardPinMap_%s_%s_%d_%s_%s.csv",           
                                                STR_PIN_BLOCK(SysSet13.m_nPinBlockType), 
                                                STR_MAX_PIN(SysSet12.m_nPinTotMax),
                                                SysSet12.m_nLowerJigStartPin, 
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                                                STR_BOXQD_TYPE(SysSet12.m_nPinQDLower));
    strcat(fName, strTemp);
          
    fp = fopen(fName,"wt");
    if(fp == NULL) 
    {   //son210416 .CON 이 있을때 PinMap 출력 기능 보완
        strTemp.Format("Can't Write %s File.\n", fName );    
        errMessageBox(1113, strTemp); //son230217_2
        return;
    }
//son240118_2 end.

	int card;
    int cardPin;
    int nPin;

    fprintf(fp, "   card, cardPin, ,hwPin\n");

	for( card = m_nCardNoStart; card <= m_nCardNoEnd; card++ )
    {
		for( cardPin=0; cardPin < nCardPinMax; cardPin++)
        {

            nPin = GetHwPinNo_Old(card, cardPin);     //son231222_2 GetHwPinNo_Old() 신규함수로 코드 간소화 
            fprintf(fp, "%10d, %10d, ,%10d\n", card, cardPin, nPin);

        }
    }
    fclose(fp);    //son240118_2 

	if( SysSet13.m_nSet7==1 ){      //son240123
		::ShellExecute(NULL,"open","EXCEl.EXE",fName,"NULL",SW_SHOWNORMAL);		    //son240123
	}
	else{						
		::ShellExecute(NULL,"open","notepad.EXE",fName,"NULL",SW_SHOWNORMAL);	    //son240123
	}
}


//son240125 ChildView5, ChildView8 기존 (Old. SYLEE ) SelfTest 코드에서 nPin 계산하는 함수.
//son  pin offset을 더해서 하부시작핀을 더하는 방식
// card:  0부터 시작
// cardPin:  0부터 시작
int GetHwPinNo_Old(int card, int cardPin )
{
    CString strTemp;
    int nPin = 0;
    int nCardMax = 256;   //son240608

    static bErrPrint = false;

	//son Switch 카드가 64pin인지 128 pin인지 확인
	// nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 또는 128) 값을 설정한다. 
	Get_nCardPinMax();//sylee150903 //ACE400    //son231227


    //son swtich 카드 pin 개수 예외 케이스 
    if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) {
        if (card < 64) { //64=128x64=8k
            nCardPinMax = 128;   //son 상부 8K만 QD 타입으로 128pin card
        }
        else {
            nCardPinMax = 64;    //son 하부 4K는 DD타입으로 64 pin card
        }
    }

    //son 상부 SD 1K 2 box,   하부 SD 1K 2 box (S100-1)
    //    SD는 64pin card 16개가 1box, 상부 2box면 card 32개까지가 상부.
    if (SysSet12.m_nPinTotMax == PIN_TOT_4K) {
        nCardMax = 4 * 16;  //son240608

        if (card < 32) { //son 상부.  
            nPin = cardPin + card * nCardPinMax;
        }
        else { //son 224를 더해야  32번 card가 256번(하부시작핀 16384(0기준)의 카드가 된다)
               //     64 pin 256개 card가 16384 pin이므로  224를 더해서 16384를 찾는 것. 
               //     SD 64 pin 기준 하부시작 card는  32이므로 256이 되려면 224를 더해야 한다.
            nPin = cardPin + (card + 128 + 64 + 32) * nCardPinMax;
        }
    }
    //son 상부 DD 2K 2 box,   하부 DD 2K 2 box  (DD가 기본)
    else  if (SysSet12.m_nPinTotMax == PIN_TOT_8K)//8k		
    {
        //son  SD는 64pin card 16개가 1box, 상부 2box면 card 32개까지가 상부.
        if (SysSet13.m_nPinBlockType != PIN_BLK_DD) { 
            nCardMax = 4 * 16;  //son240608

            if (card < 32) {
                nPin = cardPin + card * nCardPinMax;
            }
            else {
                nPin = cardPin + (card + 64 + 32) * nCardPinMax;
            }
        }
        //son 8K는 PIN_BLK_DD가 기본.  DD는 64pin card 32개가 1box, 상부 2box면 card 64개까지가 상부.
        else { //sylee180830-1-DDTYPE-JTECH
            nCardMax = 4 * 32;  //son240608

            if (card < 64) {// 64pin x 64ea sw =4k
                nPin = cardPin + card * nCardPinMax;
            }
            else {  //son 192를 더하면 64번 card가 256번 카드 (하부시작핀 16384(0기준)의 카드가 된다)
               //     64 pin 256개 card가 16384 pin이므로  192를 더해서 16384를 찾는 것. 
               //     DD 64 pin 기준 하부시작 card는  64이므로 256이 되려면 192를 더해야 한다.
                nPin = cardPin + (card + 192) * nCardPinMax;
            }
        }
    }

#if 1 //son240130 begin:
    //else if (SysSet12.m_nPinTotMax == PIN_TOT_16K )  //son231223 BH 12K PinMode Service > SelfTest 처리기능 추가
    else if (SysSet12.m_nPinTotMax == PIN_TOT_16K || SysSet12.m_nPinTotMax ==  PIN_TOT_12K //son231223
                                    || SysSet12.m_nPinTotMax == PIN_TOT_U8K_UPPER_ONLY) 
    {
        nCardMax = 4 * 32;  //son240608

        //son 상부 : 8K(2 * 32 card , 128 pin) 
        if (card < 64) {//64=128x64=8k
            nPin = cardPin + card * nCardPinMax;
        }
        //son 하부
        else {
            //son m_nPinBlockType == PIN_BLK_12K_QD_DD 는 m_nPinTotMax == PIN_TOT_16K 로 설정해서 사용한다.
            if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) {
                //son 192를 더하면 64번 card가 256번 카드 (하부시작핀 16384(0기준)의 카드가 된다)
                //     64 pin 256개 card가 16384 pin이므로  224를 더해서 16384를 찾는 것. 
                //     DD 64 pin 기준 하부시작 card는  64이므로 256이 되려면 192를 더해야 한다
                
                //son nPin = cardPin + (card + 192) * nCardPinMax 와 동일.
                nPin = cardPin + ((card - 64) * 64 + 64 * 128 * 2); 
            }
            //if (SysSet13.m_nPinBlockType != PIN_BLK_12K_QD_DD) 
            else {
                //son 64 card를 더해서 하부시작핀을 16385로 만들어 준다.
                //    128 pin 128 card가 16384 pin 이므로 64에 64를 더해서 128로 만들어 준다.
                //    QD는 상부에 64 card, 상부8K 이지만 hwPin 번호는 128 card, 상부16K 기준으로  계산해야 한다.
                nPin = cardPin + (card + 64) * nCardPinMax;
            }
        }
    }

    else if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN    //son QD PinBlock, 상부 4K
             || SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS )   //son 32K PinBlock, 상부 4K
    { 
        nCardMax = 2 * 32;  //son240608

        //son 상부: 4K(1 *32 card , 128 pin) ,  QD PinBlock 
        if (card < 32) {//32: 128x32 =4k
            nPin = cardPin + card * nCardPinMax;
        }
        //son 하부: 4K(1 *32 card , 128pin),  QD PinBlock
        else {
            //son 96 card를 더해서 하부시작핀을 16385로 만들어 준다.
            //    128 pin 128 card가 16384 pin 이므로 32에 96를 더해서 128로 만들어 준다.
            //    PIN_TOT_8K_LOW_STPIN는 상부에 32 card, 상부4K 이지만 hwPin 번호는 128 card, 상부16K 기준으로  계산해야 한다.
            nPin = cardPin + (card + 96) * nCardPinMax;
        }
    }

    //son 32K PinBlock 상부 8K, 상부  2box skip
    else  if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS || SysSet12.m_nPinTotMax == PIN_TOT_12K_TRANS 
                    || SysSet12.m_nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS
                    || SysSet12.m_nPinTotMax == PIN_TOT_32K_U8K_L24K) //son K600 
    { 
//son240608 begin:
        nCardMax = 4 * 32;  
        switch (SysSet12.m_nPinTotMax) {
            case PIN_TOT_12K_TRANS:             nCardMax = 3 * 32; break; 
            case PIN_TOT_20K_U8K_L4K_UP_TRANS:  nCardMax = 5 * 32; break; 
            case PIN_TOT_32K_U8K_L24K:          nCardMax = 8 * 32; break; 

            case PIN_TOT_16K_TRANS:             
            default:        nCardMax = 4 * 32; break;
        }
//son240608 end.

        //son 상부: 8K(2 * 32card, 128pin)
        if (card < 64) {//son 64: 128 x 64= 8192 
            nPin = cardPin + card * nCardPinMax;
        }
        else {
            //son 64 card를 더해서 하부시작핀을 16385로 만들어 준다.
            //    128 pin 128 card가 16384 pin 이므로 64에 64를 더해서 128로 만들어 준다.
            //    PIN_TOT_8K_LOW_STPIN는 상부에 32 card, 상부4K 이지만 hwPin 번호는 128 card, 상부16K 기준으로  계산해야 한다.
            nPin = cardPin + (card + 64) * nCardPinMax;
        }
    }
    //son 32K PinBlock 상부 12K, 상부  1 box skip
    else  if (SysSet12.m_nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS)
            //son240522 || SysSet12.m_nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)      
    { 
        nCardMax = 5* 32;  //son240608

        //son 상부: 12K(3 * 32card, 128pin)
        if (card < 96) {//son 96: 128 x 96= 12288 
            nPin = cardPin + card * nCardPinMax;
        }
        else {
            //son 32 card를 더해서 하부시작핀을 16385로 만들어 준다.
            //    128 pin 128 card가 16384 pin 이므로 96에 32를 더해서 128로 만들어 준다.
            nPin = cardPin + (card + 32) * nCardPinMax;
        }
    }
#endif //son240130 end.

    else  if (SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS) { //son230216_1
        nCardMax = 8 * 32;  //son240608
        nPin = cardPin + card * nCardPinMax;
    }

    //son  32K base, 상부 16K가 꽉 차 있는 경우
    //son  PIN_TOT_32K_ORG, PIN_TOT_20K_UP_TR, PIN_TOT_24K_UP_TR, PIN_TOT_24K_TRANS
    else {
        nCardMax = 8 * 32;  //son240608
        nPin = cardPin + card * nCardPinMax;
    }

    if (SysSet13.m_nPinBlockType == PIN_BLK_S100_1) {
        nCardMax = 4 * 16;  //son240608
        if (card < 32) {
            nPin = cardPin + card * nCardPinMax;
        }
        else {
            //son 16384 pin 기준으로 64 pin 상부 card가 256개이므로 32에 224를 더한다.
           //     SD 64 pin 기준 하부시작 card는  32이므로 256이 되려면 224를 더해야 한다.
            nPin = cardPin + (card + 128 * 2 - 32) * nCardPinMax;
        }
    }

    if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE) { //son230216_1
        if (card < 16) {
            nPin = cardPin + card * nCardPinMax;
        }
        else {
            nPin = cardPin + (card + 128 - 16) * nCardPinMax;
        }
        return nPin;    //son240608
    }

    if (SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421) { 
        if (card < 32) {
            nPin = cardPin + card * nCardPinMax;
        }
        else {
            nPin = cardPin + (card + 128 - 32) * nCardPinMax;
        }
        return nPin;    //son240608
    }

    if (SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) {
        if (card < 12) {
            nPin = cardPin + card * 128; //nCardPinMax; 
        }
        else {
            nPin = cardPin + (card + 128 - 12) * 128; //nCardPinMax; 
        }
        return nPin;    //son240608
    }

    if (SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421) { 
        if ((nPin >= 2048) && (nPin >= 2048)) {
            nPin = nPin + 2048;
        }
        else  if ((nPin >= (2048 + 4096 * 4))) {
            nPin = nPin + 2048;
        }
        return nPin;    //son240608
    }

    int No, Namo, Mok1;
    if (SysSet13.m_nPinBlockType == PIN_BLK_SD) { 
        No = nPin;
        if (No < 16384) {
            Namo = No % 64;
            Mok1 = No / 64;
            No = Mok1 * 128 + Namo;
        }
        else {
            No = No - 16384;
            Namo = No % 64;
            Mok1 = No / 64;
            No = Mok1 * 128 + Namo + 16384;
        }
        nPin = No;
        return nPin;    //son240608
    }

    //son240608 MaxCard Range를 넘어서는 경우에는 Offset을 더하지 않고 nPin 계산하도록 보정하고  1회 에러 출력  기능 추가 
    if (card >= nCardMax) {       
        if (bErrPrint == false) {  //son 프린트는 1회만 출력.
            strTemp.Format("GetHwPinNo_Old() range Error!, m_nPinTotMax=%s.\n\n card=%d is over nCardMax(%d)\n",  
                    STR_MAX_PIN(SysSet12.m_nPinTotMax), card, nCardMax);
            errMessageBox(8200, strTemp);
            bErrPrint = true;
        }
        nPin = cardPin + card * nCardPinMax;    //son range를 넘었으므로 offset 더하지 않도록 보정.
    }

    return nPin;
}


//son240201 ChildView5, Childview8 용 공통 전역함수
//          nCase를 parameter로 사용. LV, HV  공통 
char* getStrSelfTestCase(int nCase)
{
    CString strTest, strUndefTest;

    strUndefTest.Format("UndefCase:%d", nCase);
    strTest.Format("%s", (nCase == SELF_TEST_OPEN)? "OPEN":                       //son240130  1: SELF_TEST_OPEN
            (nCase == SELF_TEST_SHORT1)? "SHORT1":                                //son240130  2: SELF_TEST_SHORT1 
            (nCase == SELF_TEST_SHORT2)? "SHORT2":                                //son240130  3: SELF_TEST_SHORT2 
            (nCase == SELF_TEST_LEAK1)? "LEAK1":                                  //son240130  4: SELF_TEST_LEAK1
            (nCase == SELF_TEST_LEAK2)? "LEAK2":                                  //son240130  5: SELF_TEST_LEAK2
            (nCase == SELF_TEST_LEAK_A)? "LEAK_A":                                //son240130  6: SELF_TEST_LEAK_A
            (nCase == SELF_TEST_LEAK_B)? "LEAK_B":                                //son240130  7: SELF_TEST_LEAK_B
            (nCase == SELF_TEST_VC_1PIN_RST)? "VC One Pin Reset":                 //son240130  8: SELF_TEST_VC_1PIN_RST
            (nCase == SELF_TEST_VB_1PIN_RST)? "VB One Pin Reset": strUndefTest);  //son240130  9: SELF_TEST_VB_1PIN_RST

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat(szStr , strTest); 
    return szStr;
}

//son240205 ChildView5, Childview8 용 공통 전역함수
//          nProStep1을 parameter로 사용. LV, HV  구분된 케이스 
char* getStrSelfTestCase2(int nProStep1)
{
    CString strTest, strUndefTest;

    strUndefTest.Format("UndefTest:%d", nProStep1);
    strTest.Format("%s", (nProStep1 == SELF_TEST_LV_OPEN)? "LV OPEN":       //son230130  1: SELF_TEST_LV_OPEN
            (nProStep1 == SELF_TEST_LV_SHORT1)? "LV SHORT1":                //son230130  2: SELF_TEST_LV_SHORT1 
            (nProStep1 == SELF_TEST_LV_SHORT2)? "LV SHORT2":                //son230130  3: SELF_TEST_LV_SHORT2 
            (nProStep1 == SELF_TEST_LV_LEAK1)? "LV LEAK1":                  //son230130  4: SELF_TEST_LV_LEAK1
            (nProStep1 == SELF_TEST_LV_LEAK2)? "LV LEAK2":                  //son230130  5: SELF_TEST_LV_LEAK2
            (nProStep1 == SELF_TEST_LV_LEAK_A)? "LV LEAK_A":                //son230130  6: SELF_TEST_LV_LEAK_A
            (nProStep1 == SELF_TEST_LV_LEAK_B)? "LV LEAK_B":                //son230130  7: SELF_TEST_LV_LEAK_B
            (nProStep1 == SELF_TEST_VC_1PIN_RST)? "VC One Pin Reset":       //son230130  8: SELF_TEST_VC_1PIN_RST
            (nProStep1 == SELF_TEST_VB_1PIN_RST)? "VB One Pin Reset":       //son230130  9: SELF_TEST_VB_1PIN_RST
            (nProStep1 == SELF_TEST_HV_LEAK1)? "HV LEAK1":                  //son230130 14: SELF_TEST_HV_LEAK1
            (nProStep1 == SELF_TEST_HV_LEAK2)? "HV LEAK2":                  //son230130 15: SELF_TEST_HV_LEAK2
            (nProStep1 == SELF_TEST_HV_LEAK_A)? "HV LEAK_A":                //son230130 16: SELF_TEST_HV_LEAK_A
            (nProStep1 == SELF_TEST_HV_LEAK_B)? "HV LEAK_B": strUndefTest); //son230130 17: SELF_TEST_HV_LEAK_B

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat(szStr , strTest); 
    return szStr;
}
