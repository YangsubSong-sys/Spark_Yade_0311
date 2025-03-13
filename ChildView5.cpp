// ChildView8.cpp : implementation file
//

#include "stdafx.h"
#include "stdlib.h"
#include "BiOpTroCon.h"
#include "ChildView5.h"
#include "ModeSet17.h"
#include <winuser.h>
#include "ModeSub71.h"
#include "FileSysInfo.h"
#include "ModeSet11.h"
#include "ModeSub31.h"
#include "ModeSet15.h"
#include "Contec.h"
#include "A_Define1.h"//sylee161126
#include "ModeSet12.h"		//son-32KPIN-20190902:  added by sonji

extern short nFlagLeakMPin1;  //sylee240509
extern CModeSub71  Sub71;
extern CModeSet12  ModeSet12;


#ifdef _DEBUG
//#define CHILDVIEW5_ERR_NO_COUNT     //son240521
#endif

#ifdef DEF_CARD_A1  //sylee170328

#else  //#ifdef DEF_CARD_A1  

	//=======================================
	#include "DioD64H.h"//sylee170229
	#include "DioD96.h"//sylee170229
	#include "UniDAQ.h"//sylee170229
	#include "PCI_D64H.h" //sylee170229 ��
	extern CDIO_D64  pDIO_D64;//sylee170229
	extern CString str2;
	//=======================================


#endif  //#ifdef DEF_CARD_A1 

#define	MAX_SELF_ERR_PIN	3000


int nlist1;//sylee240509
int nLeakRunLV,nLeakRunHV;//sylee240530
int nHVIADCFlag;//sylee240509
int nVbCheckCount1;//sylee230131    //son NG �̸� 0�� �ƴ�.
int nDisplayDetail1;
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



extern CChildView7  dView7;  //sylee240509
extern CSysInfo05  SysInfo05;//sylee240509
extern int nFailCount[15];//sylee240509
extern double nFailL1[DEF_MAX_PIN1][F_FAIL_ITEM_NUM];  //sylee240509 
extern int   nFailL1Co,nFlag_Parall_Short,nFlagModeShort1, nShortCount;//sylee240509 
extern short nPinSetSkipVb;//sylee160803-6 //ACE400  //son230127
extern short nPinSetSkipVc;//sylee160803-6//ACE400	 //son230127
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1
extern int  gDef_Language1; //sylee151028   //1=KOREA ,2=ENGLISH, 3=CHINA 
extern CSysSet12  SysSet12;  //Pin SETUP 
extern CSysSet13  SysSet13;
extern CSysSet15  SysSet15;
extern int g_nSelfAutoFlag1;//sylee120903   //son defined in ChildView1.cpp
extern int nG_Language;  //sylee150707
extern __int64 GetMilSecond2();
extern void DoEvents();
extern CModeSub71  		Sub71;
extern CSysInfoView01  	SysInfoView01;
extern int nAutoProcess1;		//son defined in ChildView7.cpp (AutoTest)   !!!�տ� int ������ �߰���.
extern int nStop;			//son defined in  ChildView7.cpp (AutoTest)
extern int nTimeDelay1;		//son defined in  ModeSer13.cpp (Calibration)
extern int nManuaOpenPin1;//sylee160127-3	//son defined in ModeMan12.cpp (OpenTest)
extern int nManuaOpenPin2;//sylee160127-3

//-----------------------------------
//son extern defined in ChildView8.cpp
//-----------------------------------
extern int g_nSelfTestInterlock;//sylee131121
extern int g_nMode1;
extern int g_nErrCount1;
extern int g_nErrCountTot;
extern int g_nSelfRunFlag;
extern int g_nListCount;			// OnList1Mes1()�� write�� �α� ����
extern int g_nStopFlag;
extern int g_nAutoFlag;
extern int nAllPinSearchFlag;//sylee120927	 
extern int nHW_CardPin;//sylee150903-1  //ACE400  2=64pin   0,1 =128
extern int nCardPinMax;//sylee150903-1  //ACE380 //ACE400 
extern float dSelfErr1[MAX_SELF_ERR_PIN + 1][10];//sylee151210-memorydel
extern short nCardS1[300][150];//==0  ,   2= fail 
extern CSysSet211  SysSet211;  //Pin SETUP   
extern double dIADC1; //son231012_2 int nIADC1  -> double dIADC1���� ����. �Ҽ������� �߸��� ��������. 
/////////////////////////////////////////////////////////////////////////////
// CChildView5

IMPLEMENT_DYNCREATE(CChildView5, CFormView)

CChildView5::CChildView5()
	: CFormView(CChildView5::IDD)
{
	//{{AFX_DATA_INIT(CChildView5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
   
 	
}

CChildView5::~CChildView5()
{
	g_nStopFlag = 2;
}

void CChildView5::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView5)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress_System);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress_VB);
	DDX_Control(pDX, IDC_PROGRESS3, m_progress_AD);
	DDX_Control(pDX, IDC_PROGRESS4, m_progress_2wIO);
	DDX_Control(pDX, IDC_PROGRESS5, m_progress_4wIO);
	DDX_Control(pDX, IDC_PROGRESS6, m_progress_Leak);

    DDX_Control(pDX, IDC_CHECK1, m_check_System);
	DDX_Control(pDX, IDC_CHECK2, m_check_VB);
	DDX_Control(pDX, IDC_CHECK3, m_check_AD);
	DDX_Control(pDX, IDC_CHECK4, m_check_2wIO);
	DDX_Control(pDX, IDC_CHECK5, m_check_4wIO);
    DDX_Control(pDX, IDC_CHECK6, m_check_2wLeak);

	DDX_Control(pDX, IDC_LABEL1001, m_Label_System);
	DDX_Control(pDX, IDC_LABEL1002, m_Label_VB);
	DDX_Control(pDX, IDC_LABEL1003, m_Label_AD);
	DDX_Control(pDX, IDC_LABEL1004, m_Label_2wIO);
	DDX_Control(pDX, IDC_LABEL1005, m_Label_4wIO);
	DDX_Control(pDX, IDC_LABEL1006, m_Label_2wLeak);
	DDX_Control(pDX, IDC_LIST1, m_list1); 
    DDX_Control(pDX, IDC_EDIT101, m_edit101);		
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView5, CFormView)
	//{{AFX_MSG_MAP(CChildView5)

	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck_System)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck_VB)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck_AD)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck_2wIO)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck_4wIO)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BEGIN_EVENTSINK_MAP(CChildView5, CFormView)
	//{{AFX_EVENTSINK_MAP(CChildView5) 	

	ON_EVENT(CChildView5, IDC_BUTTON101, -600 /* Click */, OnButtonSystemCheck, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON102, -600 /* Click */, OnButtonVbCheck, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON103, -600 /* Click */, OnButtonAdCheck, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON104, -600 /* Click */, OnButton_2wIoCheck, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON105, -600 /* Click */, OnButton_4wIoCheck, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON106, -600 /* Click */, OnButtonOpen, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON107, -600 /* Click */, OnButtonShort, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON201, -600 /* Click */, OnButtonStart, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON202, -600 /* Click */, OnButtonStop, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON302, -600 /* Click */, OnButtonMessageClear, VTS_NONE)
	ON_EVENT(CChildView5, IDC_BUTTON108, -600 /* Click */, OnButton_2wLeak, VTS_NONE)

	//}}AFX_EVENTSINK_MAP

END_EVENTSINK_MAP()



/////////////////////////////////////////////////////////////////////////////
// CChildView5 diagnostics

#ifdef _DEBUG
void CChildView5::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView5::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView5 message handlers

BOOL CChildView5::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	m_nProgSystem_Rate = 0;
	m_nProgVB_Rate = 0;
	m_nProgAD_Rate = 0;
	m_nProg2wIO_Rate = 0;
	m_nProg4wIO_Rate = 0;
	m_nProg2wLeak_Rate = 0 ;
	g_nAutoFlag = 0;

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}




void CChildView5::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{
		// The clock timer?
	case 10:
 		m_progress_System.SetPos(m_nProgSystem_Rate);
		break;
	case 20:
 		m_progress_VB.SetPos(m_nProgVB_Rate);
		break;
	case 30:
 		m_progress_AD.SetPos(m_nProgAD_Rate);
		break;
	case 40:
 		m_progress_2wIO.SetPos(m_nProg2wIO_Rate);
		break;
	case 50:
 		m_progress_4wIO.SetPos(m_nProg4wIO_Rate);
		break;
	case 60:
 		m_progress_Leak.SetPos(m_nProg2wLeak_Rate);
		break;
	}
	// Update the dialog
 	UpdateData(FALSE);
	CFormView::OnTimer(nIDEvent);
}






void CChildView5::A_Language() //sylee151028 //LANGUAGE CHANGE
{

	if (SysSet211.m_nSet13 == 1) {//sylee160907-1   //chinese
		gDef_Language1 = 3;//redboard
	}
	else {//sylee160907-1
		gDef_Language1 = 2; //englishos
	}//sylee160907-1


	if (gDef_Language1 == 3) {//sylee151028-1    
		m_Label101.SetWindowText(LoadMessage(1, "TitleSelfTest1", 101));
		m_button305.SetWindowText(LoadMessage(1, "TitleSelfTest1", 102));
		m_button302.SetWindowText(LoadMessage(1, "TitleSelfTest1", 103));
		m_button101.SetWindowText(LoadMessage(1, "TitleSelfTest1", 104));
		m_button102.SetWindowText(LoadMessage(1, "TitleSelfTest1", 105));
		m_button103.SetWindowText(LoadMessage(1, "TitleSelfTest1", 106));
		m_button104.SetWindowText(LoadMessage(1, "TitleSelfTest1", 107));
		m_button105.SetWindowText(LoadMessage(1, "TitleSelfTest1", 108));
		m_button201.SetWindowText(LoadMessage(1, "TitleSelfTest1", 109));
		m_button202.SetWindowText(LoadMessage(1, "TitleSelfTest1", 110));
		m_button106.SetWindowText(LoadMessage(1, "TitleSelfTest1", 111));
		m_button107.SetWindowText(LoadMessage(1, "TitleSelfTest1", 112));
	}


	if (SysSet211.m_nSet13 == 1) {//sylee160907-1   //chinese
		nG_Language = 2;//chinese   //sylee160907-1
	}
	else {//sylee160907-1
		nG_Language = 2; //englishos
	}//sylee160907-1

}



void CChildView5::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CFont font2;
	LOGFONT lf;
	char szText[100]; 
	
	lf.lfHeight = MulDiv(9, 500, 300);
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = HANGEUL_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	font2.CreateFontIndirect(&lf);

	m_list1.SetFont(&font2);

    sprintf(szText, "100");
    m_edit101.SetWindowText(szText);
    sprintf(szText, "  "); 		 
 	m_Label_2wLeak.SetCaption(szText);//sylee240509
	// TODO: Add your specialized code here and/or call the base class

}



//son220405 extern double RCalCovert( int m_nMode1,  double &Rvalue, int ConMode );



void CChildView5::OnDestroy()
{
	CFormView::OnDestroy();

	g_nInitFlag1 = 0;
	KillTimer(10);
	KillTimer(20);
	KillTimer(30);
	KillTimer(40);
	KillTimer(50);
	KillTimer(60);//sylee240509
	// TODO: Add your message handler code here	
}



BOOL CChildView5::OnEraseBkgnd(CDC* pDC)
{

	//NO1
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));

	return TRUE;
	//	return CDialog::OnEraseBkgnd(pDC);
}

void CChildView5::DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd)
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

	nAutoProcess1 = 0;

	for (int i = 0; i < Rect.bottom; ++i)
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

HBRUSH CChildView5::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	int nID = pWnd->GetDlgCtrlID();

	if (g_hbr != NULL)
		::DeleteObject(g_hbr);
	else
	{
		//���濡 ������ �ؽ�Ʈ�� ����. ���� ������ �̰����� ������.
		g_hbr = CreateSolidBrush(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(0, 0, 0));
	}

	pDC->SetBkMode(TRANSPARENT);

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CChildView5::OnDraw(CDC* pDC)
{
	// TODO: Add your specialized code here and/or call the base class


}

BOOL CChildView5::PreTranslateMessage(MSG* pMsg)
{

#ifdef _PC_TEST_
	g_nSelfTestInterlock = 1;
	return 0;//tt
#endif


	CString str;
	int nRet1;
	// TODO: Add your specialized code here and/or call the base class
	//son  ChildView1::OnInitialUpdate() -> pFrame->OnMenuTest8()  
	//    -> CMainFrame::SwitchView(VIEW_ID5) 
	//         -> nInitFla1�� 8�̰� nIniFlag2�� 1�� MainFrm.cpp���� �����ϰ� ChildView5 �����츦 ����
	if (g_nInitFlag1 == 8 && g_nInitFlag2 == 1) // view8 init   
	{
		//------------------------------
		//son SelfTest ȭ�� �ʱ�ȭ
		//------------------------------

		Sub71.AOnVSetMulti1(1, 1, 1, 1);//sylee200915
		nManuaOpenPin1 = 1;//sylee160127-3
		nManuaOpenPin2 = 8192;//sylee160127-3
		m_SelfAutoFlag1 = 0;
		g_nSelfRunFlag = 0;
		g_nStopFlag = 0;
		g_nInitFlag2 = 2;

		SetTimer(10, 30, NULL);		// 30 msec
		SetTimer(20, 30, NULL);		// 30 msec
		SetTimer(30, 30, NULL);		// 30 msec
		SetTimer(40, 30, NULL);		// 30 msec
		SetTimer(50, 30, NULL);		// 30 msec
		SetTimer(60, 30, NULL);		// 30 msec  //sylee240509

		str.Format("");
		m_Label_System.SetCaption(str);
		m_Label_VB.SetCaption(str);
		m_Label_AD.SetCaption(str);
		m_Label_2wIO.SetCaption(str);
		m_Label_4wIO.SetCaption(str);

		m_check_System.SetCheck(1);
		m_check_VB.SetCheck(1);
		m_check_AD.SetCheck(1);
		m_check_2wIO.SetCheck(1);
		m_check_4wIO.SetCheck(1);
		m_check_2wLeak.SetCheck(1);   //sylee240509

		FileSysInfo01.LoadSaveSet12(_LOAD); 	// load: 2

		m_dISOSet1 = 8;
		//m_nTestCase1=1; 
		//nDischarge1=0;
		nAllPinSearchFlag = 0;

		CString str;

		m_list1.ResetContent();
		g_nErrCount1 = 0;
		str.Format(" ");
		m_Label_System.SetCaption(str);
		m_Label_VB.SetCaption(str);
		m_Label_AD.SetCaption(str);
		m_Label_2wIO.SetCaption(str);
		m_Label_4wIO.SetCaption(str);
	    m_Label_2wLeak.SetCaption(str);


		m_nProgSystem_Rate = 0;
		m_nProgVB_Rate = 0;
		m_nProgAD_Rate = 0;
		m_nProg2wIO_Rate = 0;
		m_nProg4wIO_Rate = 0;
        m_nProg2wLeak_Rate = 0;

		nRet1 = Sub71.AOnPoweroff2();//sylee20120702
		if (nRet1 != 1) {
			AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
			return 0;
		}

		nRet1 = Sub71.AOnVSetMulti1(1, 1, 30, 30);
		if (nRet1 != 1) {
			AfxMessageBox(" Error No 8027, \n   V SET ERROR !,   Self VB CHECK SET error!!!");
			return 0;
		}
		//	Sleep(500);
		// Sleep(500);

		Sub71.ARunoff1();//sylee20111020//vc sense off       /VC-SS  CMD off			 
		m_SelfAutoFlag1 = 0;

	    str.Format(" ");  		 
		m_Label_2wLeak.SetCaption(str);//sylee240509

	}


	//sylee131105 JOYTECH SAJANGNIM
	//son ���α׷��������� SelfTest �ϴ� ��� g_nSelfAutoFlag1 = 10���� ������. ChildView1.
	if (g_nSelfAutoFlag1 == 10) {//sylee130916
		g_nSelfAutoFlag1 = 1;
		//OnAuto_Self1();
		OnButtonStart();
	}

	if (g_nSelfAutoFlag1 == 2)
	{
		g_nSelfAutoFlag1 = 3;

		// OnList1Mes1("*** Auto Self Test    Start***");
		
        //son211020 CFileSysInfo::getEndBoxNo(int nPinTotMax) �Լ��� �ڵ� �̵� . �Լ�ȣ��� ����
        //son240520 	if (g_nSelfAutoFlag1 == 2) ���η� �̵��ؼ� �ʹ� ���� ȣ��Ǵ� ���� ����. 
        int nEndBox = FileSysInfo01.getEndBoxNo(SysSet12.m_nPinTotMax);        //son211020  //son240520


		for (int box1 = 1; box1 <= nEndBox; box1++)     //son-32KPIN-20200228: jk�� box1�� ����.
		{
			if (g_nSelfAutoFlag1 == 0) {
				break;
			}

			//son-32KPIN-20200228 : 16K_FT ��� ���� �ΰ��� skip
			//son 16K_FT�� ���� ��� 2box(box1, box2)��, �Ϻ� 2box(box5, box6) ���.
			//    box3�� box4�� �ǳ� �پ�� �Ѵ�.  box 3,4: card64 ~ card127 
			//if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS
			//	|| SysSet12.m_nPinTotMax == PIN_TOT_12K	//son210202  box1���� ����, 3��°, 4��° box skip
			//	|| SysSet12.m_nPinTotMax == PIN_TOT_12K_FULL_TRANS)    //son210319
			//if (IS_UPPER_8K(SysSet12.m_nPinTotMax))    //son210531
			if (ModeSet12.IsUpper8k(SysSet12.m_nPinTotMax))     //son210831
			{
				if (box1 >= 3 && box1 < 5)
					continue;
			}
			//son-8K4097-20200605 : 8K_����4097  ��� ����  3 box1�� skip
			//son 8K_����4097�� ���� ��� 1box(box1)��, �Ϻ� 1box(box4) ���.
			//    box3���� �ǳ� �پ�� �Ѵ�.  box 2,3,4: card32 ~ card127      128pin card
			//if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN
			//        || SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)  //son210319 //son230216_1
			//if (IS_UPPER_4K(SysSet12.m_nPinTotMax))    //son210531
			if (ModeSet12.IsUpper4k(SysSet12.m_nPinTotMax))     //son210831
			{
				if (box1 >= 2 && box1 < 5)
					continue;
			}

			//son210531 ��� 3 box, �Ϻ� 1 box, �Ϻ� 4��° box�� skip 
			if (IS_UPPER_12K(SysSet12.m_nPinTotMax))    //son210531
			{
				if (box1 == 4) //box 1���� ����, 4��° box skip
					continue;
			}

			if (SysSet12.m_nPinBoxMax == 1)  //2k  //sylee121005
			{
				if (box1 == 1) {//4k								 
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX A : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 1 ~ ");
					//		 m_edit1.SetWindowText("1");
					m_nCardNoStart = 1;
					//		 m_edit2.SetWindowText("16");
					m_nCardNoEnd = 16;
				}
				else  if (box1 == 2) {//8k								
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX B : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("               * Start Card 17 ~ ");
					//	 m_edit1.SetWindowText("17");
					m_nCardNoStart = 17;
					//	 m_edit2.SetWindowText("32");
					m_nCardNoEnd = 32;
					Sleep(500);
				}
				else  if (box1 == 3) {//16k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX C : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 33 ~ ");
					//	 m_edit1.SetWindowText("33");
					m_nCardNoStart = 33;
					//	 m_edit2.SetWindowText("48");
					m_nCardNoEnd = 48;
					Sleep(500);
				}
				else  if (box1 == 4) {//16k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX D : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 49 ~ ");
					//	 m_edit1.SetWindowText("49");
					m_nCardNoStart = 49;
					//	 m_edit2.SetWindowText("64");
					m_nCardNoEnd = 64;
					Sleep(500);
				}
				else  if (box1 == 5) {//32k								
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX E : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("               * Start Card 65 ~ ");
					//	 m_edit1.SetWindowText("65");
					m_nCardNoStart = 65;
					//	 m_edit2.SetWindowText("80");
					m_nCardNoEnd = 80;
					Sleep(500);
				}
				else  if (box1 == 6) {//32k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX F : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 81 ~ ");
					//	 m_edit1.SetWindowText("81");
					m_nCardNoStart = 81;
					//	 m_edit2.SetWindowText("96");
					m_nCardNoEnd = 96;
					Sleep(500);
				}
				else  if (box1 == 7) {//32k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX G : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 97 ~ ");
					//	 m_edit1.SetWindowText("97");
					m_nCardNoStart = 97;
					//	 m_edit2.SetWindowText("112");
					m_nCardNoEnd = 112;
					Sleep(500);
				}
				else  if (box1 == 8) {//32k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX H : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 113 ~ ");
					//	 m_edit1.SetWindowText("113");
					m_nCardNoStart = 113;
					//	 m_edit2.SetWindowText("128");
					m_nCardNoEnd = 128;
					Sleep(500);
				}

			}

			else //son 4K
			{

				if (box1 == 1) {//4k								 
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX A : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 1 ~ ");
					//	 m_edit1.SetWindowText("1");
					m_nCardNoStart = 1;
					//	 m_edit2.SetWindowText("32");
					m_nCardNoEnd = 32;
				}
				else  if (box1 == 2) {//8k

					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX B : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("               * Start Card 33 ~ ");
					//	 m_edit1.SetWindowText("33");
					m_nCardNoStart = 33;
					//	 m_edit2.SetWindowText("64");
					m_nCardNoEnd = 64;
					Sleep(500);
				}
				else  if (box1 == 3) {//16k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX C : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 65 ~ ");
					//	 m_edit1.SetWindowText("65");
					m_nCardNoStart = 65;
					//	 m_edit2.SetWindowText("96");
					m_nCardNoEnd = 96;
					Sleep(500);
				}
				else  if (box1 == 4) {//16k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX D : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 97 ~ ");
					//m_edit1.SetWindowText("97");
					m_nCardNoStart = 97;
					// m_edit2.SetWindowText("128");
					m_nCardNoEnd = 128;
					Sleep(500);
				}
				else if (box1 == 5) {//32k

					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX E : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("               * Start Card 129 ~ ");
					// m_edit1.SetWindowText("129");
					m_nCardNoStart = 129;
					// m_edit2.SetWindowText("160");
					m_nCardNoEnd = 160;
					Sleep(500);
				}
				else  if (box1 == 6) {//32k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX F : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 161 ~ ");
					//m_edit1.SetWindowText("161");
					m_nCardNoStart = 161;
					// m_edit2.SetWindowText("192");
					m_nCardNoEnd = 192;
					Sleep(500);
				}
				else  if (box1 == 7) {//32k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX G : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 193 ~ ");
					//m_edit1.SetWindowText("193");
					m_nCardNoStart = 193;
					// m_edit2.SetWindowText("224");
					m_nCardNoEnd = 224;
					Sleep(500);

				}
				else  if (box1 == 8) {//32k
					OnList1Mes1("      ");
					OnList1Mes1(" =============================================");
					OnList1Mes1(" < BOX H : Start >  ");
					OnList1Mes1("      ");
					OnList1Mes1("              * Start Card 225 ~ ");
					// m_edit1.SetWindowText("225");
					m_nCardNoStart = 225;
					// m_edit2.SetWindowText("256");
					m_nCardNoEnd = 256;
					Sleep(500);

				}

			}//2k


			OnAuto_Self1();//sylee120903

			if (box1 == 1) {//4k
				OnList1Mes1("              * End Card ~32  ");
			}
			else  if (box1 == 2) {//8k
				OnList1Mes1("               * End Card ~64  ");
			}
			else  if (box1 == 3) {//16k
				OnList1Mes1("               * End Card ~96  ");
			}
			else  if (box1 == 4) {//16k
				OnList1Mes1("              * End Card ~128  ");

			}
			else  if (box1 == 5) {//32k
				OnList1Mes1("               * End Card ~160  ");
			}
			else  if (box1 == 6) {//32k
				OnList1Mes1("               * End Card ~192 ");
			}
			else  if (box1 == 7) {//32k
				OnList1Mes1("              * End Card ~224  ");
			}
			else  if (box1 == 8) {//32k
				OnList1Mes1("               * End Card ~256  ");
			}


		}

		// OnList1Mes1("*** Auto Self Test     End***");


		OnList1Mes1(" ==============================================");
		g_nSelfAutoFlag1 = 1;


	}

	return CFormView::PreTranslateMessage(pMsg);
}




void CChildView5::OnButtonSystemCheck() //power
{

	CString str;
	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 

	Sub71.OnDisCharge1(); //sylee170725-3 ray
	OnAuto_Self1_Init();   ///sylee160520-1

	nDisplayDetail1 = 1;

	if (m_SelfAutoFlag1 != 2) {
		m_SelfAutoFlag1 = 0;
	}

	m_nProgSystem_Rate = 0;
	m_progress_System.SetPos(m_nProgSystem_Rate);

	//DoEvents();
	//Sleep(200);
	g_nStopFlag = 0;

	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running.  \n\n   System Check    Start Error  !!!");
		return;
	}
	g_nSelfRunFlag = 1;

	OnList1Mes1("*** System Check Start***");
	PowerCheckError1();//sylee141029

	OnList1Mes1("*** System Check Finish***");
	OnList1Mes1("                         ");

	g_nSelfRunFlag = 0;

	return;
}




void CChildView5::OnButtonVbCheck() //vb check
{
	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 


	Sub71.OnDisCharge1(); //sylee170725-3 ray
	//return ; 
	g_nStopFlag = 0;

	nDisplayDetail1 = 1;

	if (m_SelfAutoFlag1 != 2) {
		m_SelfAutoFlag1 = 0;
	}

	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running.   \n \n    VB Check    Start Error  !!!");
		return;
	}
	g_nSelfRunFlag = 1;


	OnList1Mes1("*** VB  Check Start***");

    nVbCheckCount1=0;//sylee230131
    for( int nLoop1=1;  nLoop1<=5; nLoop1++){//sylee230131
        OnCheckVB1();   //vb check 
        if(nVbCheckCount1==0){ //son NG�̸�  0�� �ƴϹǷ� �ִ� 5ȸ���� retry�ϰ� ��. 
            break;//sylee230131
        }
    }

	OnList1Mes1("*** VB  Check Finish***");
	OnList1Mes1("                       ");

	g_nSelfRunFlag = 0;

	m_progress_VB.SetPos(m_nProgVB_Rate);

	return;
}




void CChildView5::OnButtonAdCheck()
{

	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 
	Sub71.OnDisCharge1(); //sylee170725-3 ray
	nDisplayDetail1 = 1;

	nPinSetSkipVb = 0; //sylee160803 //ACE400  //son230127
	nPinSetSkipVc = 0; //sylee160803 //ACE400  //son230127

	//	return ;

	g_nStopFlag = 0;
	if (m_SelfAutoFlag1 != 2) {
		m_SelfAutoFlag1 = 0;
	}

	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running.   \n \n    AD Check    Start Error  !!!");
		return;
	}

	m_nProgAD_Rate = 0;
	m_progress_AD.SetPos(m_nProgAD_Rate);


	g_nSelfRunFlag = 1;

	OnList1Mes1("*** AD  Check Start***");

	OnCheckAD_RRead2();

	OnList1Mes1("*** AD  Check Finish***");
	OnList1Mes1("                       ");


	g_nSelfRunFlag = 0;

	Sub71.AOnPoweroff2();

	return;
}




void CChildView5::OnList1Mes1(CString str) //ad
{
	m_list1.AddString(str);
	m_list1.SetTopIndex(m_list1.GetCount() - 1);
	g_nListCount++;
	if (g_nListCount > 1000) {
		g_nListCount = 0;
		m_list1.ResetContent();
	}
	//sylee170725-5
}


// int nMod1 = open, ushort, short, hr , hv , 4w 
// m_nMode1  1~65    
//  nProMod1= R VALE
//int nPStep1  1~ 7 step
int CChildView5::OnCheckVB1()
{

	//#ifndef _PC_TEST_
	CString   str, str1;

	int nRet1;
	double dVbR2, dVbR1;
	int Lo1, Lo2;

	int VLSet[4] = { 1,5,10,20 };
	int VHSet[5] = { 100,150,200,250,1 }; //sylee200915
	int nVBCheckNG1;//sylee230131

	nVBCheckNG1=0;//sylee230131

	g_nErrCount1 = 0;
	m_nProgVB_Rate = 10;
	Sub71.OnPinAllReset1();
	str.Format("Run");
	m_Label_VB.SetCaption(str);


	for (int k = 2; k < 2; k++)//sylee201204	//son211115 <---- �� �κ� ���� �ȵȴ�.  k=2���Ͱ�  k<2 ���ǿ� �� ����
	{

		DoEvents();
		if (g_nStopFlag == 2) {
			g_nSelfRunFlag = 0;
			goto Task1;
		}

		m_dVSet1 = VLSet[k];
		m_dVSet2 = VHSet[k];

		nRet1 = Sub71.AOnPoweroff2();//sylee20120702
		if (nRet1 != 1) {
			AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
			return 0;
		}

		nRet1 = Sub71.AOnVSetMulti1(m_dVSet1, m_dVSet1, 0, 0);
		if (nRet1 != 1) {
			AfxMessageBox(" Error No 8023, \n   V SET ERROR !,   Self VB CHECK SET error!!!");
			return 0;
		}

		for (int j = 0; j < 200; j++) {
			DoEvents();
			if (g_nStopFlag == 2) {
				g_nSelfRunFlag = 0;
				goto Task1;
			}
		}



		for (int i = 1; i <= 2; i++)
		{

			if (g_nStopFlag == 2) {
				g_nSelfRunFlag = 0;
				goto Task1;
			}

			m_nProgVB_Rate = (int)((k * 30) + (i * 2.5));

			if (g_nErrCount1 != 0) {
				break;
			}

			if (k == 0 && i == 1) {
				m_nProgVB_Rate = 25;
			}

			// DoEvents();
			if (i == 1) {
				Lo1 = 5000;   //vb  retry 
				Lo2 = 5;//sylee180910-3  30->5
			}
			else {
				Lo1 = 2000;   //vb  retry 
				Lo2 = 5;//sylee180910-3  30->5
			}


			if (i == 1 || i == 2) {
				m_dVSet = m_dVSet1;
			}
			else {
				m_dVSet = m_dVSet2;
			}
			m_nVRel = i;


		Task101:


			Sub71.nMode1 = 0;

			Sub71.AOnReadVB1Run1(m_nVRel);    // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;

			Sub71.Delay_100ns(30000);  //3ms 
			if (m_dVSet <= 5) {
				dVbR2 = m_dVSet * (0.35);//20%		//sylee211216
			}
			else {
				dVbR2 = m_dVSet * (0.1);//10%  //sylee211118  RAY  5->10%
			}
			dVbR1 = m_dVSet - dVbR2;
			dVbR2 = m_dVSet + dVbR2;

			if (Sub71.V1 > 300)
			{
				if (Lo1-- > 0) {
					Sub71.Delay_100ns(30000);  //3ms   
					goto Task101;
				}
			    nVBCheckNG1=1;//sylee230131 NG ǥ��
				str.Format("  * 101 VB%d,  Set=%d  ERROR1 : =>Read=%.1f, ", m_nVRel, m_dVSet, Sub71.V1);
				OnList1Mes1(str);
			}

			if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2)
			{
				if (Lo1-- > 0) {
					Sub71.Delay_100ns(300000);  //30ms   
					goto Task101;
				}

				g_nErrCount1++;
			    nVBCheckNG1=1;//sylee230131 NG ǥ��
				str.Format("  * 102 VB %d, Set=%.0f  ERROR2   => Read=%.1f V", m_nVRel, m_dVSet, Sub71.V1);
				OnList1Mes1(str);
			}
			else {
				if (Lo2-- > 0) {
					Sub71.Delay_100ns(30000);  //30ms   
					goto Task101;
				}
				if (nDisplayDetail1 != 1) {//sylee141107
					str.Format("  * 103 VB %d, Set=%.0f    OK   => Read=%.1f V", m_nVRel, m_dVSet, Sub71.V1);
					OnList1Mes1(str);
				}
			}


		}//for( i=1; i<=4; i++)


	}//for( k=0; k<5; k++)



	//================================================



//sylee210308	for(   k=0; k<4; k++)
	for (k = 2; k < 3; k++)//sylee210308  ray   4->2
	{

		DoEvents();
		if (g_nStopFlag == 2) {
			g_nSelfRunFlag = 0;
			goto Task1;
		}

		m_dVSet1 = VLSet[k];     //son VLSet[2] : 10
		m_dVSet2 = VHSet[k];	    //son VHSet[2] : 200 

		nRet1 = Sub71.AOnPoweroff2();//sylee20120702
		if (nRet1 != 1) {
			AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
			return 0;
		}

		nRet1 = Sub71.AOnVSetMulti1(0, 0, m_dVSet2, m_dVSet2);  //son  0, 0, 200,200

		if (nRet1 != 1) {
			AfxMessageBox(" Error No 8023, \n   V SET ERROR !,   Self VB CHECK SET error!!!");
			return 0;
		}

		for (int j = 0; j < 200; j++)
		{
			DoEvents();
			if (g_nStopFlag == 2) {
				g_nSelfRunFlag = 0;
				goto Task1;
			}
		}



		for (int i = 3; i <= 4; i++)
		{

			if (g_nStopFlag == 2) {
				g_nSelfRunFlag = 0;
				goto Task1;
			}

			m_nProgVB_Rate = (int)((k * 30) + (i * 2.5));

			if (g_nErrCount1 != 0) {
				break;
			}

			if (k == 0 && i == 1) {
				m_nProgVB_Rate = 25;
			}

			// DoEvents();
			if (i == 1) {
				Lo1 = 5000;   //vb  retry 
				Lo2 = 5;//sylee180910-3  30->5
			}
			else {
				Lo1 = 2000;   //vb  retry 
				Lo2 = 5;//sylee180910-3  30->5
			}

			if (i == 1 || i == 2) {
				m_dVSet = m_dVSet1;
			}
			else {
				m_dVSet = m_dVSet2;
			}
			m_nVRel = i;
			Sub71.Delay_100ns(30000);  //3ms        //son211115


		Task201:


			Sub71.nMode1 = 0;

			Sub71.AOnReadVB1Run1(m_nVRel);    // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2;

			//Sub71.Delay_100ns(30000);  //3ms  //son211115 �ڸ�Ʈó��
			if (m_dVSet <= 5) {
				dVbR2 = m_dVSet * (0.35);//10%	//sylee211216
			}
			else {
				dVbR2 = m_dVSet * (0.1);//5%  //sylee211216
			}
			dVbR1 = m_dVSet - dVbR2;
			dVbR2 = m_dVSet + dVbR2;

			if (Sub71.V1 > 300) {
				if (Lo1-- > 0) {
					Sub71.Delay_100ns(30000);  //3ms 
					goto Task201;
				}
				nVBCheckNG1=1;//sylee230131  NG ǥ��
				str.Format("  * 201 VB%d,  Set=%d  ERROR1 : =>Read=%.1f, ", m_nVRel, m_dVSet, Sub71.V1);
				OnList1Mes1(str);
			}

			if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2) {
				if (Lo1-- > 0) {
					Sub71.Delay_100ns(300000);  //30ms 

					if (Lo1 == 1000 || Lo1 == 2000) {//sylee211216
						if (i == 1 || i == 2) {
							Sub71.AOnVSetMulti1(m_dVSet1, m_dVSet1, 0, 0);//sylee211216
						}
						else {
							Sub71.AOnVSetMulti1(0, 0, m_dVSet2, m_dVSet2);//sylee211216
						}
					}


					goto Task201;
				}
				nVBCheckNG1=1;//sylee230131  NG ǥ��
				g_nErrCount1++;
				str.Format("  * 202 VB %d, Set=%.0f  ERROR2   => Read=%.1f V", m_nVRel, m_dVSet, Sub71.V1);
				OnList1Mes1(str);
			}
			else {
				if (Lo2-- > 0) {
					Sub71.Delay_100ns(30000);  //30ms   
					goto Task201;
				}
				if (nDisplayDetail1 != 1) {//sylee141107
					str.Format("  * 203 VB %d, Set=%.0f    OK   => Read=%.1f V", m_nVRel, m_dVSet, Sub71.V1);
					OnList1Mes1(str);
				}
			}

		}//for( i=1; i<=4; i++)

	}//for( k=0; k<5; k++)

	//=====================================================================================


Task1:




	nRet1 = Sub71.AOnPoweroff2();//sylee20120702
	if (nRet1 != 1) {
		AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
		return 0;
	}

	//sylee200915  nRet1=Sub71.AOnVSetMulti1(1,1,30,30) ;
	nRet1 = Sub71.AOnVSetMulti1(1, 1, 1, 1); //sylee200915
	if (nRet1 != 1) {
		AfxMessageBox(" Error No 8027, \n   V SET ERROR !,   Self VB CHECK SET error!!!");
		return 0;
	}
	//	Sleep(500);

	Sub71.ARunoff1();//sylee20111020//vc sense off       /VC-SS  CMD off
	DoEvents();

	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else
	{
		if (g_nStopFlag == 2) {
			str.Format("Stop");
			//if(nDisplayDetail1!=1){//sylee141107
			OnList1Mes1("     ----  Stop   -----    ");
			//}
		}
		else {
			str.Format("OK");
			m_nProgVB_Rate = 100;//sylee210308
		}
	}
	m_Label_VB.SetCaption(str);
	Sub71.AOnPoweroff2();

	//  nPinSetVSet=0;  //2=hv;

//#endif
	 if(nVBCheckNG1==1){ //sylee230131
		 nVbCheckCount1++;//sylee230131
		 Sleep(3000);
	 }else{
		 nVbCheckCount1=0;//sylee230131
	 }
	return 1;

}

int CChildView5::OnCheckAD_RRead2()
{
	CString   str, str1;
	int m_nMode1, n4Wire;
	int nRet1, nLoop;
	int nDelay10, nDelay20;
	int Lo1, Rmod1;
 	int dPinVC[200], dPinVB[200];
	double dR1[11][200], dVB1[11][200], dVC1[11][200], dI1[11][200];
	double nT[10];
	double nData[130][15];
	double dVbR1, dVbR2;

	//	int Mod1[2]={1,6};  //100 OHM   1V=1,  10V=6,     100V=,    150V=?,   200V=?
	//	int Mod2[2]={10,19};//10M OHM   {10,19,24,28};   //10V=10,  100V=19,  150V=24,   200V=28
	int ModT[4] = { 1,6,10,19 };
	///////////////////////////////////////////////////////////////////////////
	//���� �ʱ�ȭ 
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&dR1, sizeof(dR1));
	::ZeroMemory(&dVB1, sizeof(dVB1));
	::ZeroMemory(&dVC1, sizeof(dVC1));
	::ZeroMemory(&dI1, sizeof(dI1));
	::ZeroMemory(&nT, sizeof(nT));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&dPinVC, sizeof(dPinVC));
	::ZeroMemory(&dPinVB, sizeof(dPinVB));
	//	::ZeroMemory(&fName, sizeof(fName));
	//	::ZeroMemory(&fName1, sizeof(fName1));

	nStop = 0;
	n4Wire = 0;
	//	ipEnd=1;//PIECE
	//	ipFlag=0;//PIECE
	m_nProgAD_Rate = 0;

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//mode ( 1~65)
	//step ( open, ��ushort, short, hr, hv  4w)

	for (int ik = 0; ik < 1; ik++)
	{	//4=ModT ���� 

		if (ik >= 0 && ik <= 1) {
			Rmod1 = 1;
		}
		else {
			Rmod1 = 2;
		}

		m_nMode1 = 6;
		m_nCC = 1;
		m_nVRel = 2;
		m_dVSet = 10;//v
		m_nIR = 2;
		m_dISet = 10;//mA
		nDelay10 = 10000;
		m_nFil = 1;
		m_nIFilter = 1;
		m_nHVOffSet = 0;
		m_nHVGain = 0;
		nDelay20 = 1000; //sylee140407
		g_nErrCount1 = 0;


 		if (m_dVSet > 20) {
			m_nVRel = 4;
		}
 
		//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//INITAL �� 

		nRet1 = Sub71.AOnPoweroff2();//sylee20120702
		if (nRet1 != 1) {
			AfxMessageBox(" Error No 8023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
			return  0;
		}

		if (m_nVRel == 1) {
			nRet1 = Sub71.AOnVSetMulti1(m_dVSet, 0, 0, 0);
		}
		else  if (m_nVRel == 2) {
			nRet1 = Sub71.AOnVSetMulti1(0, m_dVSet, 0, 0);
		}
		else  if (m_nVRel == 3) {
			nRet1 = Sub71.AOnVSetMulti1(0, 0, m_dVSet, 0);
		}
		else  if (m_nVRel == 4) {
			nRet1 = Sub71.AOnVSetMulti1(0, 0, 0, m_dVSet);
		}

		if (nRet1 != 1) {
			AfxMessageBox(" Error No 8023, \n   V SET ERROR !,   SW TEST Run Start Error!!!");
			return 0;
		}

		if (m_dVSet > 20) {
			Sub71.Delay_100ns(10000000);  //1000ms   =1SECOND
		}
		else {
			Sub71.Delay_100ns(10000000);  //1000ms   =1SECOND  
		}

		//ADC COUNT
		Sub71.nADCount1 = (int)dRec[m_nMode1][7];
		if (Sub71.nADCount1 < 1) {
			Sub71.nADCount1 = 1;
		}
		if (Sub71.nADCount1 > 100) {
			Sub71.nADCount1 = 100;
		}
		//if( nMod1==6 ){//4W
		//	n4Wire=4;
		//}else{
		n4Wire = 0;
		//}


		//=========================================================================================
		//interlock    

		if (m_nCC != 1 && m_nCC != 2) {
			AfxMessageBox(" Error No 8024, \n    CC CV MODE SET ERROR !,     Calibration Run Start Error!!!");
			return 0;
		}

		if (m_dVSet < 1 || m_dVSet>300) {
			AfxMessageBox(" Error No 8025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!");
			return 0;
		}

		if (m_nVRel == 1 || m_nVRel == 2 || m_nVRel == 3 || m_nVRel == 4) {

		}
		else {
			AfxMessageBox(" Error No 8025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,     Calibration Run Start Error!!!");
			return 0;
		}
		//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//������ 	 

		Sleep(50); //sylee170524 bio
		m_nProgAD_Rate = 10;
		Lo1 = 3000;   //vb  retry  //sylee170520 bhflex   100->300  //sylee170524 bio   300->3000



	Task101:


		Sub71.AOnReadVB1Run1(m_nVRel);    // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
		if (m_dVSet <= 5) {
			dVbR2 = m_dVSet * (35 * 0.01);//10%
		}
		else {
			dVbR2 = m_dVSet * (5 * 0.01);//5%
		}
		dVbR1 = m_dVSet - dVbR2;
		dVbR2 = m_dVSet + dVbR2;

		if (Sub71.V1 > 300) {
			if (Lo1-- > 0) {
				Sub71.Delay_100ns(100000);  //10ms   
				goto Task101;
			}

			str.Format("Error No 8301,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1, m_dVSet, Sub71.V1, dVbR1, dVbR2);
			AfxMessageBox(str);
			return 0;
		}

		if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2) {
			if (Lo1-- > 0) {
				Sub71.Delay_100ns(200000);  //20ms   
				goto Task101;
			}

			str.Format("Error No 8302,\n \n\n  Power Check? \n\n\n   VB READ Error!!    Calibration NO=%d ! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),", (int)m_nMode1, m_dVSet, Sub71.V1, dVbR1, dVbR2);
			AfxMessageBox(str);
			return 0;
		}

		//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//������				

		m_nProgAD_Rate = 20;

		if (m_nFil == 1 || m_nFil == 2) {//sylee130628
			Sub71.AOnReadV1Enable1(m_nVRel);
		}
		else {
			Sub71.AOnReadV1Enable1(5);
		}
		if (m_nFil == 3) {//hv open
			//sylee140226	Sub71.HV_OffSet(m_nHVOffSet); 
			//sylee140226	Sub71.HV_Gain(m_nHVGain);	
			Sub71.HV_OffSet_Gain(m_nHVOffSet, m_nHVGain);    //sylee140226
		}
		Sub71.FilterSet(m_nFil);//sylee130628 
		if (m_nIFilter == 3) {
			AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!");
			m_nIFilter = 1;
		}
		Sub71.I_Filter(m_nIFilter);//sylee130629
		Sub71.AOnISetOn1Run1(m_nCC, m_nVRel, m_dVSet, m_nIR, m_dISet);
		m_nProgAD_Rate = 30;
		//sylee131221 Sub71.Delay_100ns(400000); //40mS  //sylee130122 ray		
		Sub71.Delay_100ns(500000); //50mS
		Sub71.Delay_100ns(3000000); //300mS//sylee160511-1
		m_nProgAD_Rate = 40;
		nLoop = 5;//sylee20120104

		//	 Sub71.Delay_100ns(100000); //sylee131003  //sylee130122 ray
		//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//������		
     	Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118

		// Task1:								 
		Sub71.AOnPinSet1(1, 11, MOD_TR_SET_VB); //son210220  1->2, enum���� ����
		Sub71.Delay_100ns(5000);//sylee160720
		Sub71.AOnPinSet1(1, 11, MOD_TR_SET_VC); //son210220  2->1, enum���� ����
		m_nProgAD_Rate = 55;


		Sub71.Delay_100ns(1000000);  //100ms  //sylee151125  //sylee160720  250->100 

		if (m_nCC == 2) {//cv   //sylee160720
			Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
		}
		else {// cc
			Sub71.AOnReadV1I1(m_nVRel); // CC MODE   //  V I READ
		}

		Sub71.Delay_100ns(5000);//sylee160720

		if (m_nCC == 2) {//cv   //sylee160720
			Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
		}
		else {// cc
			Sub71.AOnReadV1I1(m_nVRel); // CC MODE   //  V I READ
		}
		Sub71.Delay_100ns(1000000); //100mS//sylee160511-1  //sylee160720   500->100


		m_nProgAD_Rate = 70;

		if (m_nCC == 2) {//cv
			Sub71.AOnReadV1I1(7);     // CV MODE   //  I READ		 		
		}
		else {// cc
			Sub71.AOnReadV1I1(m_nVRel); // CC MODE   //  V I READ
		}

		//*******************************************************************************************************************************//

		if (Sub71.Y1 < 0.0000001) {
			Sub71.Y1 = 0.0000001;
		}


		if (m_nCC == 1) { //CC    r= (vb-vc)/i
			if (m_nMode1 >= 66 && m_nMode1 <= 69) {//HV OPEN ENABLE  
				if (m_nIR == 1 || m_nIR == 2) {
					Sub71.R1 = (Sub71.X1) / Sub71.Y1 * 1000.0;
				}
				else if (m_nIR == 3 || m_nIR == 4 || m_nIR == 5) {
					Sub71.R1 = (Sub71.X1) / Sub71.Y1 * 1000000.0;
				}
			}
			else {
				if (m_nIR == 1 || m_nIR == 2) {
					Sub71.R1 = ((Sub71.V1 - Sub71.X1) / Sub71.Y1) * 1000.0;
				}
				else if (m_nIR == 3 || m_nIR == 4 || m_nIR == 5) {
					Sub71.R1 = ((Sub71.V1 - Sub71.X1) / Sub71.Y1) * 1000000.0;
				}
			}
		}
		else if (m_nCC == 2) {//CV   r=(vb/i)
			if (m_nIR == 1 || m_nIR == 2) {
				Sub71.R1 = (Sub71.V1) / Sub71.Y1 * 1000.0;
			}
			else if (m_nIR == 3 || m_nIR == 4 || m_nIR == 5) {
				Sub71.R1 = (Sub71.V1) / Sub71.Y1 * 1000000.0;
			}
		}


		Sub71.OnPinAllReset1(); ///SELF R 100OHM
		//		nRet1=Sub71.OnPinAllReset1(); ///PIN ALL RESET  

		m_nProgAD_Rate = 85;
		Sub71.Y1 = 6;//all pass   //sylee160722
		Sub71.X1 = 6;//all pass   //sylee160722

		if (Sub71.Y1 > 5) {
			if (nDisplayDetail1 != 1) {//sylee141107
				str.Format("      *  AD    Current  OK !");
				OnList1Mes1(str);
			}
		}
		else {
			str.Format("      *-> [Error] AD  Check Error!: I Check! =%f.2", Sub71.Y1);
			OnList1Mes1(str);
			g_nErrCount1++;
		}


		if (Sub71.X1 > 5) {
			if (nDisplayDetail1 != 1) {//sylee141107
				str.Format("      *  AD     Voltage  OK !");
				OnList1Mes1(str);
			}
		}
		else {
			str.Format("     *-> [Error] AD  Check Error !: [VC] Voltage Check! =%f.2", Sub71.X1);
			OnList1Mes1(str);
			g_nErrCount1++;
		}

		Sub71.AIRangeOff();// i range all off
		Sub71.AIVBOff();//VB OFF	
		DoEvents();

	}////for END      for( ik=1;  

	Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018
	if (m_nMode1 >= 51 && m_nMode1 <= 60) {
		Sub71.HVOpenSet(0); //HV OPEN  SET  //sylee20120411
	}
	Sub71.ARunoff1();//sylee20111020//vc sense off       /VC-SS  CMD off
	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else {
		str.Format("OK");
	}
	m_Label_AD.SetCaption(str);
	m_nProgAD_Rate = 100;

	return 1;
}


void CChildView5::OnButton_2wLeak() //iocheck//sylee240509
{
	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 
  	nShortCount=0;//sylee240509
    OnManualShort1_LV();//sylee240509
	if(nShortCount<1){//sylee240509
	 	OnManualShort1_HV2() ;//sylee240509
	} 

}


void CChildView5::OnButton_2wIoCheck() //iocheck
{

 	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 
	Sub71.OnDisCharge1(); //sylee170725-3 ray
	nDisplayDetail1 = 1;

	if (m_SelfAutoFlag1 != 2)
		m_SelfAutoFlag1 = 0;

	//son 32K ���� 2W SelfTest
	if (IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax))	//son210202
		OnAuto_Self1_32K();
	//son ���� 2W SelfTest
	else
		OnAuto_Self1();			// <=== 
	return;
}



void CChildView5::OnButton_4wIoCheck() //4w
{
	CString str;

	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 

	Sub71.OnDisCharge1(); //sylee170725-3 ray
	nDisplayDetail1 = 1;
	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running.        Selftest  4W  Relay Check  Start Error  !!!");
		return;
	}

	if (m_SelfAutoFlag1 != 2) {
		m_SelfAutoFlag1 = 0;
	}

	g_nSelfRunFlag = 1;
	if (g_nStopFlag == 2)
	{
		str.Format("Stop");
		m_Label_4wIO.SetCaption(str);
	}
	else
	{
		str.Format("Run");
		m_Label_4wIO.SetCaption(str);

		//son 32K ���� �ű� PinMap ���� 4W SelfTest
		if (IS_NEW_PIN_TOTMAX(SysSet12.m_nPinTotMax))	//son210202
			OnATestRun4W_32K(181);

		//son ���� 4W SelfTest
		else
			OnATestRun4W(181); //   Case=1 s1,    2=  s2  //sylee130701  46->181		
	}
	g_nSelfRunFlag = 0;
	return;
}



void CChildView5::OnButtonOpen() //open
{
	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 
	if (g_nSelfRunFlag > 0) {
		//   AfxMessageBox("   Running.        Selftest  4W  Relay Check  Start Error  !!!"); 
		return;
	}
	CModeMan12 Dlg;
	Dlg.DoModal();
	return;
}


void CChildView5::OnButtonShort() //short
{
	if(	nLeakRunLV==1 ){
		AfxMessageBox("   Leak LowVoltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	}
	if(	nLeakRunHV==1){
		AfxMessageBox("   Leak High Voltage  Running.   Start Error  !!!"); 		return  ;//sylee240530
	} 
	if (g_nSelfRunFlag > 0) {
		//   AfxMessageBox("   Running.        Selftest  4W  Relay Check  Start Error  !!!"); 
		return;
	}
	CModeMan11 Dlg;
	Dlg.DoModal();
	return;
}


//=======================================================================================================

//----------------------------
//son 32K ���� 4W SelfTest 
//----------------------------

int g_n4WCardFail[500];				//son nFailSta[500]; ��� ���

//son-32KPIN-20190902:  32K ����
void CChildView5::OnATestRun4W_32K(int nCalMode)
{
#if 1
	//   Case=1 s1,    2=  s2

	CString strOut = "";
	CString str, str1;
	CString strfName, strfName1;

	int 	nRet1;

	FILE* fp1;
	char  fName[200];
	char  fName1[200];
	int nCardX;	//sylee150210 //ACE380
	int nDisplayDetail2;
	//===============================================
	// ���� �ʱ�ȭ 
	//===============================================

	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));
	::ZeroMemory(&dSelfErr1, sizeof(dSelfErr1));
	::ZeroMemory(&g_n4WCardFail, sizeof(g_n4WCardFail));

	nDisplayDetail2 = 0;//detail error log display
	nCardX = 1;   //sylee151007 //ACE400
	g_nErrCount1 = 0;
	g_nErrCountTot = 0;
	Sub71.nADCount1 = 1;//test
	g_nStopFlag = 0;
	//===============================================
	//son   m_nCardNoStart,  m_nCardNoEnd  ����
	//===============================================
	m_nCardNoStart = 1;				//son 1
	//m_nCardNoEnd = 256*nCardX;	//son 256


	int box_num = FileSysInfo01.getEndBoxNo(SysSet12.m_nPinTotMax);        //son211020

	//son m_nCardNoEnd = 32 * 8 * nCardX;		
	m_nCardNoEnd = (MAX_BOX_CARD * box_num) * nCardX;	//son MAX_BOX_CARD: 32

	if (m_nCardNoStart < 1) {
		str.Format("Error Self,  StartCard(=%d) < 1 . \n \n  Set Error !", m_nCardNoStart);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}
	if (m_nCardNoEnd < 1) {
		str.Format("Error Self,  EndCard(=%d) < 1 . \n \n  Set Error !", m_nCardNoEnd);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}
	if (m_nCardNoEnd > (256 * nCardX)) {
		str.Format("Error Self,  EndCard(=%d) > (256*%d) (32k).  \n \nSet Error !", m_nCardNoEnd, nCardX);//ACE380
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	if (m_nCardNoEnd < m_nCardNoStart)
	{
		str.Format("Error Self,  Range Error! End Card(=%d) < Start Card(=%d)  . \n\n", m_nCardNoEnd, m_nCardNoStart);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	//------------------------------------------------
	//son Switch ī�尡 64pin���� 128 pin���� Ȯ��

	// nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 �Ǵ� 128) ���� �����Ѵ�. 
	Get_nCardPinMax();//sylee150903 //ACE400  //son231227

	//son 64�� pin ī���� ���
	if (nCardPinMax == 64) //sylee150903-1 //ACE400
	{
		if (m_nCardNoStart > 512) {
			str.Format("Error Self,  Begin Card >  512 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}

	}

	//son 128�� pin ī���� ���
	else
	{
		if (m_nCardNoStart > 256) {
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}
	}

	//son 0 base �� ����.
	m_nCardNoStart = m_nCardNoStart - 1;
	m_nCardNoEnd = m_nCardNoEnd - 1;
	//===========================
	// ���� ���� 
	//===========================
	m_dISOSet1 = 8.0; //sylee151007 //ACE400   SHIN BU SUNG  8V
	strfName1.Format("\\SETUP\\001SelfRelay.csv");         //<=======================================
	DeleteFile(strfName1);
	::ZeroMemory(&fName1, sizeof(fName1));
	strcat(fName1, SysInfoView01.m_pStrFilePath1);//sylee20111212
	strcat(fName1, strfName1);
	fp1 = fopen(fName1, "wt");

	if (fp1 == NULL) {
		str.Format("Error    001SELFHVISO file open   ?  Error, Saving Error !     file close!   ");
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}

	m_nCC = (int)dRec[nCalMode][1];
	m_nVRel = (int)dRec[nCalMode][2];
	m_dVSet = dRec[nCalMode][3];
	m_nIR = (int)dRec[nCalMode][4];
	m_dISet = dRec[nCalMode][5];
	m_nFil = (int)dRec[nCalMode][8];
	m_nIFilter = (int)dRec[nCalMode][9];   //sylee130629		  
	m_nHVOffSet = (int)dRec[nCalMode][10]; //sylee130629
	m_nHVGain = (int)dRec[nCalMode][11];	//sylee130629

	if (m_dVSet > 20) {
		str.Format(" Error No 8312, SelfTest,Relay \n\n     Set check LV !,  4W is LV.  \n \n  Voltage SET=%d ? It should be (< 20) ", (int)m_dVSet);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
		//sylee130701   m_nFil= 0;//Fast speed
	}


	if ((m_nIR == 1) && (m_dISet > 10)) {
		str.Format(" Error No 8314, SelfTest,Relay \n\n    Set check !,  4W is IRange=%d,  (m_dISet(=%.2f)>10 ?  error ).",
			m_nIR, m_dISet);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}



	nTimeDelay1 = (int)(dRec[nCalMode][6] * 10);
	Sub71.nADCount1 = (int)dRec[nCalMode][7];

	if (Sub71.nADCount1 < 1) {
		Sub71.nADCount1 = 1;
	}

	if (Sub71.nADCount1 > 100) {
		Sub71.nADCount1 = 100;
	}

	nRet1 = Sub71.AOnPoweroff2();//sylee20120702
	if (nRet1 != 1)
	{
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 8315, \n   Power all off fail !,   SW TEST Run Start Error!!!");
		g_nSelfRunFlag = 0;
		return;
	}

	if (m_nVRel == 1) {
		nRet1 = Sub71.AOnVSetMulti1(m_dVSet, 0, 0, 0);	//son LV1 ����
	}
	else  if (m_nVRel == 2) {
		nRet1 = Sub71.AOnVSetMulti1(0, m_dVSet, 0, 0);	//son LV2 ����
	}

	if (nRet1 != 1) {
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 1315, \n   V SET ERROR !,   SW TEST Run Start Error!!!");
		g_nSelfRunFlag = 0;
		return;
	}
	Sub71.Delay_100ns(3000000);  //300ms   

	//===========================
	//  ������ 	 
	//===========================

	int n4Wire;
	int Lo1;
	__int64   tStart1, tStart2;


	tStart1 = GetMilSecond2();
	n4Wire = 4;//sylee130702
	//-----------------------------------
	//son ����,������ Range Error Check.   

	strOut = "";
	Lo1 = 3;
	while (Lo1 > 0)
	{
		double dVbMax, dVbMin, dVbGap;

		//son VB READING (Sub71.V1 �� ȹ��)   
		Sub71.AOnReadVB1(m_nVRel);   //nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
		//son Range Check 
		if (m_dVSet < 5)
			dVbGap = m_dVSet * (0.35);  //sylee211216

		else
			dVbGap = m_dVSet * (0.1);   //sylee211216  ray 5->10%

		dVbMin = m_dVSet - dVbGap;
		dVbMax = m_dVSet + dVbGap;

		//son Range Check�� ����ߴٸ� Sub71.AOnReadVB1(m_nVRel) ��ƾ�� �����. 
		if (Sub71.V1 <= 300 && Sub71.V1 >= dVbMin && Sub71.V1 <= dVbMax)
			break;

		//-----------------------------------------------------------
		//son ����, ���� Range Error�� �߻�.  3ȸ ������ retry�Ѵ�.

		//son max 3ȸ����  Retry�� �� �ߴٸ� ���� �޽����� ����ϰ� ��ƾ�� �����.
		Lo1--;
		if (Lo1 <= 0)
		{
			if (Sub71.V1 > 300)
			{
				str.Format("Error No 1312,\n \n\n  Power Check? \n\n\n");
				strOut += str;
				str.Format(" ***** HW CHECK Please! *****\n");
				strOut += str;
			}

			// (Sub71.V1 >= dVbMin && Sub71.V1 <= dVbMax) ��� 
			else
			{
				str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n");
				strOut += str;
			}

			//son_debug_191220:  VB Read�� ������  %f.2  �� �߸��� ���� %.2f�� ������.
			str.Format(" Calibration NO=%d VB READ Error!!!\nVB Set=%.2f, VB Read =%.2f,\n VB low=%.2f, VB high =%.2f",
				(int)nCalMode, m_dVSet, Sub71.V1, dVbMin, dVbMax);
			strOut += str;
			AfxMessageBox(strOut);
			g_nSelfRunFlag = 0;
			break;
		}

		//son Sub71.AOnReadVB1(m_nVRel): ����, ������ ������  retry �Ѵ�. (retry Ƚ�� ������)
		Sub71.Delay_100ns(3000000);  //son Delay 300ms 	//son goto Task101�� ����

	}

	//son VC READ  INIT SET 
	Sub71.AOnReadV1Enable1(m_nVRel);//sylee130704	//???? Ȯ�� �ʿ�
	m_nFil = 1;//all off//sylee130704
	Sub71.FilterSet(m_nFil);//sylee130628 
	m_nIFilter = 1;
	Sub71.I_Filter(m_nIFilter);//sylee130629 
	Sub71.OnPinAllReset1();  //sylee150210
	Sub71.AOnISetOn1(m_nCC, m_nVRel, m_dVSet, m_nIR, m_dISet);    //sylee150210 move
	Sub71.Delay_100ns(3000000);  //300ms     //sylee150210 move
	tStart2 = GetMilSecond2();

	double nT[10];
	::ZeroMemory(&nT, sizeof(nT));

	nT[0] = (double)(tStart2 - tStart1) / 1000.;  //test 
	CTime curTime = CTime::GetCurrentTime();
	fprintf(fp1, "#   %04d/  %02d/  %02d  -  %02d: %02d: %02d  , \n\n\n",
		curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

	//===========================
	//  ������ 	 
	//===========================
	OnList1Mes1("                    ");
	str.Format("  ***  4W Check  Start ****   ");
	OnList1Mes1(str);

	fprintf(fp1, str);
	fprintf(fp1, "\n\n");

	//son testCase 1, 2: Short Self Test   3,4: Open Self Test
	for (int testCase = 1; testCase <= 4; testCase++)
	{
		int ret = Do_TestCase_4wSelfTest_32K(testCase);
		if (ret < 0)
		{
			//Sub71.OnPinAllReset1();

			//son -1�� �����ϸ� for ������ break;  (goto task2 ��� ���) 
			if (ret == -1)		// task2:
			{
				g_nSelfRunFlag = 0;
				break;
			}


			//son -2�� �����ϸ� OnAuto_Self1_32K() �Լ��� Ż��  (return ��� ���)
			else if (ret == -2)
			{
				fclose(fp1);//sylee20111224
				g_nSelfRunFlag = 0;
				return;
			}

		}
	}


	//----------------------
	//   Log
	//----------------------

	//son g_n4WCardFail[card] ������ �ִٸ� ���
	int card, cardPin;
	for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++)  //card no
	{

		int nPrintCard = FileSysInfo01.getPrintCardNo(SysSet12.m_nPinTotMax, card);     //son231213 ��¿� ī���ȣ

		if (g_n4WCardFail[card] > 0)
		{
			str.Format("  *  Error :4W Relay=>   Switch Card=%d   Fail=%d     ", 
			            nPrintCard + 1, g_n4WCardFail[card]); //son231213  card -> nPrintCard
			OnList1Mes1(str);
			fprintf(fp1, str);  //<=======================================
			fprintf(fp1, "\n");
		}
	}

	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else {
		if (g_nStopFlag == 2) {
			str.Format("Stop");
		}
		else {
			str.Format("OK");
		}
	}

	m_Label_4wIO.SetCaption(str);

	if (g_nErrCount1 > 0) {
		str.Format("  ---> NG");
	}
	else {
		if (g_nStopFlag == 2) {
			str.Format(" ----> Stop");
		}
		else {
			str.Format(" ----> OK");
			m_nProg4wIO_Rate = 100;//sylee170125

		}
	}
	OnList1Mes1(str);		//ȭ�� ���

	str.Format("  ****  4W Check  Finish ****  ");
	OnList1Mes1(str);


	if (g_nStopFlag == 2) {
		g_nSelfRunFlag = 0;
		g_nStopFlag = 0;
		str.Format("  * Stop   [Self Test  ::  4W RELAY ]  ");
		OnList1Mes1(str);
		AfxMessageBox(str);
	}

	fprintf(fp1, " \n  ");
	fprintf(fp1, "* SelfTest   RELAY SELF TEST    \n");
	fprintf(fp1, " , No,   NetPin,  *CardNo         ,  cardPin,  Pin ,      *Result* ,   VB,        VC,       I,         R,   \n");

	int err;
	for (err = 1; err <= g_nErrCount1; err++)//sylee20111215
	{
		// 4W�� dSelfErr1[err][1] �� Pin val.

		//jigPin=  nPinSW_Net((int)dSelfErr1[err][1]);  
		//int jigPin = ModeSet12.make32kSwPin((int)dSelfErr1[err][1] - 1) + 1;  //son210831 ePin->jigPin
		int hwPin0 = (int)dSelfErr1[err][1] - 1;    // 4W�� dSelfErr1[err][1] �� Pin val. //son230303
		int jigPin = nJigPinMap[hwPin0] + 1; //son230303
		
		cardPin = (int)(dSelfErr1[err][1] - (dSelfErr1[err][0] - 1) * nCardPinMax);

		                 // errNo  jigPin  card    cardPin    Pin    strInfo   VbVal     VcVal       I           R
		fprintf(fp1, "No=,   %d,   %d,     *%5.0f,    %3d,   %5.0f,   %s     , %10.2f  , %10.2f   , %10.2f   ,  %12.2f  , \n",
                            err, jigPin, dSelfErr1[err][0], cardPin, dSelfErr1[err][1],  	//son210831 ePin->jigPin
                            (dSelfErr1[err][2] == 1) ? "S1 Short" :
                            (dSelfErr1[err][2] == 2) ? "S2 Short" :
                            (dSelfErr1[err][2] == 3) ? "S1 Open" :
                            (dSelfErr1[err][2] == 4) ? "S2 Open" : "Undef 4W Self TestCase",
                            dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5], dSelfErr1[err][6]);


		if (nDisplayDetail2 == 1)
		{
			                    //  errNo      jigPin      card             cardPin      Pin
			str.Format("  *   4W  : No=%d,  jigPin=%d,  (*CardNo= %3.0f, CardPin=%3d, Pin=%5.0f) ",
                    				err,     jigPin, dSelfErr1[err][0], cardPin, dSelfErr1[err][1]);  //son210831 ePin->jigPin
			OnList1Mes1(str);
		}
	}
	if (g_nErrCount1 < 1) {
		fprintf(fp1, "      ***NO ERROR *** ");
	}
	else {
		fprintf(fp1, "      ***  TOTAL ERROR  = %d  *** ", g_nErrCount1);
	}


	//----------------
	// ������.
	//----------------
	fclose(fp1);

	Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018 
	Sub71.AOnPoweroff2();//sylee20120702
	Sub71.AOnRelayOff1();	//sylee20120616 
	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704
	Sub71.AOnReadV1Enable1(5); //sylee130711
	Sub71.Delay_100ns(20000);  //2ms 

	if (nStop == 2) {
		AfxMessageBox("   SelfTest::  4W Relay Check Stop!!!");
	}

	if (nDisplayDetail1 != 1)
	{
		if (g_nErrCount1 >= 1)//sylee141029
		{
			if (SysSet13.m_nSet7 == 1) {//sylee130724
				::ShellExecute(NULL, "open", "EXCEL.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
			else {
				::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
		}
	}

	if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))//sylee151007 //ACE400
	{
		str.Format("  * =>  4W ERROR : %d  over. more error   ", MAX_SELF_ERR_PIN);
		OnList1Mes1(str);
	}


	Sub71.OnPinResetInit();//sylee151221   //ACE400
	Sub71.AOnPoweroff2();


#endif
}


//#define _4W_SHORT_TEST_
//------------------------------------
//son TestCase�� 32K ���� 4W SelfTest
//------------------------------------
//
//son-32KPIN-20190902:  32K ����
//
// returnVal : 0���� �����ϸ� ����.
// 			  -1�� �����ϸ� for ������ break;  (goto task2 ��� ���) 
//            -2�� �����ϸ� OnATestRun4W_32K() �Լ��� Ż��  (return ��� ���)
int CChildView5::Do_TestCase_4wSelfTest_32K(int testCase) 			// input
{
	int nTestMode;		// 0: OpenTest,   2:ShortTest
	int nRet1/*, ki*/;


	switch (testCase)
	{
	case 1:
		Sub71.On4WS12Set(1, 1);	// VB Relay Self Test:s1(1), ON(1)
		nTestMode = 2; 				// 4W Short Test
		break;

	case 2:
		Sub71.On4WS12Set(2, 1); 	// VC Relay Self Test:s2(2),  ON(1)
		nTestMode = 2; 				// 4W Short Test
		break;

	case 3:
		Sub71.Delay_100ns(10000); //sylee131102 	//son 1ms
		Sub71.On4WS12Set(1, 1);	// VB Relay Self Test:s1(1), ON(1)
		nTestMode = 0; 				// 4w Open Test
		break;


	case 4:
		Sub71.Delay_100ns(10000);	//son 1ms
		Sub71.On4WS12Set(2, 1); 	// VC Relay Self Test:s2(2),  ON(1)
		nTestMode = 0; 				// 4w Open Test
		break;
	}


	int card, cardPin;
	int nCardNum;
	int nLoop;
	for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++)   //son m_nCardNoStart:  0 base
	{                                                           //son box  : 1 base

		//son191206�߰�:  Max ErrCount ������ �Ѿ�ٸ� ī�� ������ ������. 
		if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))//sylee160514-2 
			break;

		//son-32KPIN-20200228 : 16K ��� ���� �ΰ��� skip
		//son 16K�� ���� ��� 2box(box1, box2)��, �Ϻ� 2box(box5, box6) ���.
		//    box3�� box4�� �ǳ� �پ�� �Ѵ�.  box 3: card64 ~ card95 
		//                                     box 4: card96 ~ card127
		//if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS	//son230216_1
		//		|| SysSet12.m_nPinTotMax == PIN_TOT_12K	            //son210202  box1���� ����, 3��°, 4��° box skip
		//		|| SysSet12.m_nPinTotMax == PIN_TOT_12K_FULL_TRANS) //son210319
		//if (IS_UPPER_8K(SysSet12.m_nPinTotMax))     //son210531
		if (ModeSet12.IsUpper8k(SysSet12.m_nPinTotMax))     //son210831
		{
			if (card >= 64 && card < 128)
				continue;
		}

		//son-8K4097-20200605 : 8K_����4097  ��� ����  box 2,3,4 �� skip (box�� 1 base)
		//son ��� 1box(box 1), �Ϻ� 1box(box 5) ���.
		//    box 2,3,4�� �ǳ� �پ�� �Ѵ�.  box 2: card32 ~ car63  128pin card
		//                                   box 3: card64 ~ card95
		//                                   box 4: card96 ~ card127
		//if (IS_UPPER_4K(SysSet12.m_nPinTotMax))     //son210531 PIN_TOT_8K_LOW_STPIN, PIN_TOT_8K_TRANS
		if (ModeSet12.IsUpper4k(SysSet12.m_nPinTotMax))     //son210831
		{
			if (card >= 32 && card < 128)
				continue;
		}

        //son210531: ��� 3 box, �Ϻ� 1 box, ��� 4��° box�� skip  // box 4: card96 ~ card127
        //son240522 if (IS_UPPER_12K(SysSet12.m_nPinTotMax))     //son210531 PIN_TOT_16K_U12K_L4K_TRANS, PIN_TOT_16K_U12K_L4K_UP_TRANS
        if (ModeSet12.IsUpper12k(SysSet12.m_nPinTotMax))     //son240522
        {
            if (card >= 96  && card < 128)
                continue;
        }


		nLoop = 5;
		nCardNum = m_nCardNoEnd - m_nCardNoStart + 1;//sylee130704
		m_nProg4wIO_Rate = (int)((double)((((testCase - 1) * nCardNum) + (card - m_nCardNoStart + 1)) / ((double)(4. * nCardNum)) * 100.));

		int nPrintCard = FileSysInfo01.getPrintCardNo(SysSet12.m_nPinTotMax, card);     //son231213 ��¿� card ��ȣ

		//son nCardPinMax�� extern global ����... ���� ���� �ʿ�
		for (cardPin = 0; cardPin < nCardPinMax; cardPin++)
		{
			DoEvents();

			//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
			if (g_nStopFlag == 2) {
				g_nSelfRunFlag = 0;
				return -1;			//goto task2;
			}
			Sub71.OnPinAllReset1(); ///PIN ALL RESET
			//ki= 0;
			//--------------------------------
			//son VB, VC SelfTest PIN  ���� 				<====
			//--------------------------------

			int nPin;
			int jigPin;

			//son for SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS, 20K, 24K
			nPin = cardPin + card * nCardPinMax;
			//son200831 �̽��� box, card�� hwPin�̸� ó������ �ʴ´�.
			//jigPin = ModeSet12.make32kSwPin(nPin);
			jigPin = nJigPinMap[nPin]; //son230303
			if (jigPin == -1)	    //son210831
				continue;

			switch (testCase)
			{
				//-------------------------------------------------------------------------------------
				//son Self ShortTest :  ����Ʈ�� VB, �Ǵ� VC�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                      ������ ���� ���Ϸ� �帣�ų� �帣�� �ʴ´ٸ� ����.
				//                      VB�� �ٿ��µ�(�Ǵ� VC�� �ٿ��µ�)  ������ �帥�ٸ� short�̴�. 
				//-------------------------------------------------------------------------------------

				//son Self ShortTest  :  VB TR�� ���δ�.   
			case 1:  //son S1(VB) Self ShortTest
			case 2:  //son S2(VC) Self ShortTest
				//son  ��� Sub71.On4WS12Set()�� ������ 4W Self S1, S2�� ��� VB TR�� ���� �����Ƿ� 
				//     S1 Short, S2 Short ��� VB TR�� �Ҵ�.  
				//     VC TR �׽�Ʈ��� case 2���� VC TR�� ON �ϸ� �ȵȴ�. (191218. �źμ� ���� Ȯ��)
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  // VB TR(1) ON.   //son210220 1-> 2 enum���� ���� 
				break;


				//-------------------------------------------------------------------------------------
				//son Self OpenTest :  ���� ����Ʈ�� �� ��ȣ�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                     ������ �� �帥�ٸ� ����, ������ �������Ϸ� �帥�ٸ� open�̴�.
				//-------------------------------------------------------------------------------------

				//son S1 Self OpenTest  : ���� pin��  VB TR��  VB RY�� ���ÿ�  ���δ�.
			case 3:
				Sub71.AOnPinSet1(1, nPin, MOD_RY_SET_VB);  // VB Relay(3) ON     //son210220 3 -> 4 enum���� ����
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  // VB TR(1) ON        //son210220 1 -> 2 enum���� ����
				break;

				//son S2 Self OpenTest  : ���� pin�� VB TR��  VC RY�� ���ÿ� ���δ�. 
			case 4:
				Sub71.AOnPinSet1(1, nPin, MOD_RY_SET_VC);  // VC Relay(4) ON    //son210220 4 -> 3 enum���� ���� 
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  // VB TR(1) ON       //son210220 1 -> 2 enum���� ����
				break;

			}
			Sub71.Delay_100ns(10000); //sylee131102 	//son 1msec
			//-------------------------------------
			//son Self 4W Test ����, ���� ����
			//-------------------------------------
			Sub71.AOnReadV1I1(m_nVRel);//sylee20111115
			//------------------
			//son R�� ���
			//------------------
			if (Sub71.Y1 < 0.0000001) {
				Sub71.Y1 = 0.0000001;
			}

			if (m_nIR == 1 || m_nIR == 2) {
				Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000.0;
			}
			else if (m_nIR == 3 || m_nIR == 4 || m_nIR == 5) {
				Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000000.0;
			}

			nRet1 = Sub71.OnPinAllReset1(); ///PIN ALL RESET  
			if (nRet1 != 1) {
				AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
				g_nSelfRunFlag = 0;
				return -2;
			}

			Sub71.Delay_100ns(10000); //1mSec  

			//son ù��° card ó�� ����ÿ�  ������ 5ȸ �ݺ�.
			if (card == m_nCardNoStart)
			{
				if (nLoop > 0)  //sylee20120308
				{
					/*if(nLoop==1){
						cardPin=0;
					}
					else*/
					cardPin--;		//son191206: �����ڵ�� cardPin loop ó�� �κп� Task1���� �����ϹǷ�
									//    cardPin�� 0�� ���������� continue�� �ϸ� 1�� �þ�Ƿ� 
									//    �þ ���� �����ؼ� -1�� �ϰ� �����ϸ� nLoop 5ȸ �ݺ��ϴ� ���� cardPin�� 
									//    0�� ������ �� �ִ�. 
					nLoop--;
					continue;		// goto Task1;  ī���� ������ ó������ ���ư���.
				}
			}


			//--------------------------
			// 4W Self Short Error ����
			//--------------------------
			if (nTestMode == 2)
			{
				//	if( Sub71.X1> m_dISOSet1 )//fail log  0-> m_dISOSet1  0.61

#ifdef _4W_SHORT_TEST_
				// short �߻���
				Sub71.X1 = 2;
#endif
				//-----------------------------------------------
				//son ���ذ����� ������ ���� �帥�ٸ� Short�̴�.
				if (Sub71.X1 > 1)//fail log  0-> m_dISOSet1  0.61
				{
					if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1))
					{
						g_nErrCount1++;
						g_nErrCountTot++;
						g_n4WCardFail[card]++;
						dSelfErr1[g_nErrCount1][0] = (float)(nPrintCard + 1);  // CARD no.    //son231213  card -> nPrintCard
						dSelfErr1[g_nErrCount1][1] = (float)(nPin + 1);  // pin NO 	 //nCardPinMax=64  //sylee150210 //ACE380				  
						dSelfErr1[g_nErrCount1][2] = (float)testCase;  //son 1: VB Relay(s1) 4W Self Short,  2: VC Relay(s2) 4W Self Short
						                                               //son 3: VB Relay(s1) 4w Self Open,   4: VC Relay(s2) 4W Self Open
						dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  // vb
						dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  // vc
						dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;  
						dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;  // R1;  //double dR1[2][5000][3]; 

					}
					else
					{
						g_n4WCardFail[card]++;
						//sylee151007//ACE400	break;
					}
				}
			}

			//--------------------------
			// 4W Self Open Error ����			 
			//--------------------------
			else//(nIso1==1  
			{
				//--------------------------------------------------------------------------
				//son ���ذ����� ������ ���� �帣�ų� VC ���� ���� ������ �ȴٸ� open�̴�.
				//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۰ų� VC��(Sub71.X1)�� 8���� �۴ٸ�
				if (Sub71.X1 < m_dISOSet1) //sylee20120512    8v
				{
					if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1))
					{
						g_nErrCount1++;
						g_nErrCountTot++;
						g_n4WCardFail[card]++;
						dSelfErr1[g_nErrCount1][0] = (float)(nPrintCard + 1);  // CARD no.  //son231213  card -> nPrintCard
						dSelfErr1[g_nErrCount1][1] = (float)(nPin + 1);  //sylee151007//ACE400 //  pin NO 	//nCardPinMax=64  //sylee150210 //ACE380				  
						dSelfErr1[g_nErrCount1][2] = (float)testCase;  //son 1: VB Relay(s1) 4W Self Short,  2: VC Relay(s2) 4W Self Short
						                                               //son 3: VB Relay(s1) 4w Self Open,   4: VC Relay(s2) 4W Self Open
						dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  // vb
						dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  // vc
						dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;  // cardPin
						dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;  // R1;  //double dR1[2][5000][3];

					}
					else
					{
						g_n4WCardFail[card]++;
						//sylee151007//ACE400	break;
					}
				}

			}
		}
	}

	return 0;
}


void CChildView5::OnATestRun4W(int  nCalMode)    //   Case=1 s1,    2=  s2
{

	CString   str, str1;
	CString   strfName, strfName1;
	int nTestMode, nFlagMode1, nFail1, n4Wire, nRet1;//,m;	
	int dPinVC[200];
	int dPinVB[200];
	__int64   tStart1, tStart2;
	FILE* fp1;
	char  fName[200];
	char  fName1[200];

	double m_dISet;
	double nT[10];
	double nData[130][15];
	double dVbR1, dVbR2;

	int Lo1,nLoop,ki;
 	int nPinSt[32000];
	int nSta[350][128][5];
 	int nFailSta[500];//card max no
 	int nCardX;//sylee150210 //ACE380
	int nDisplayDetail2;


	nDisplayDetail2 = 0;//detail error log display
	//sylee150903	nCardPinMax=64;//sylee150210 //ACE380 //ACE400
	Get_nCardPinMax();//sylee150903 //ACE400    //son231227
	//sylee151007	nCardX=2;//sylee150210 //ACE380
	nCardX = 1;   //sylee151007 //ACE400
	//ACE380//////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//���� �ʱ�ȭ 

	::ZeroMemory(&nPinSt, sizeof(nPinSt));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&nT, sizeof(nT));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&dPinVC, sizeof(dPinVC));
	::ZeroMemory(&dPinVB, sizeof(dPinVB));
	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));
	::ZeroMemory(&dSelfErr1, sizeof(dSelfErr1));
	::ZeroMemory(&nFailSta, sizeof(nFailSta));


	m_nCardNoStart = 1;

#if 0  //son230224_2
	if (SysSet12.m_nPinTotMax == PIN_TOT_4K) {//4k 	//son-32KPIN-20190902 : enum ���� ����
		m_nCardNoEnd = 64 * nCardX;//sylee170327   32->64  64x64=4k
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_8K) {//8k		 
		m_nCardNoEnd = 64 * nCardX;//nCardX;//sylee150210 //ACE380

		//if( SysSet13.m_nPinBlockType==1 ) { //sylee180830-1-DDTYPE-JTECH
		if (SysSet13.m_nPinBlockType == PIN_BLK_DD) {	 //son-32KPIN-20190902: enum���� ����
			m_nCardNoEnd = 64 * 2;//64pin x 128ea = 8K //sylee180830-1-DDTYPE-JTECH
		}
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_16K) {//16k	
		m_nCardNoEnd = 128 * nCardX;//nCardX;//sylee150210 //ACE380
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_12K) {//12k		//son240125
		m_nCardNoEnd = 96 * nCardX;
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS //32k	//son230216_1
	          || SysSet12.m_nPinTotMax== PIN_TOT_32K_U8K_L24K //son230224_2
	          || SysSet12.m_nPinTotMax== PIN_TOT_32K_ORG)     //son230224_2
    {
		m_nCardNoEnd = 256 * nCardX;//nCardX;//sylee150210 //ACE380
	}
	else {
		m_nCardNoEnd = 128 * nCardX;//nCardX;//sylee150210 //ACE380
	}
#else
    //son240125 Old �ڵ�� card skip���� �ʰ� user card��ȣ�� ����ϹǷ� getCardNoEnd() �Լ� ��� �Ұ�.
    //          FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax)�� ��ü��
    //m_nCardNoEnd = FileSysInfo01.getCardNoEnd(1, SysSet12.m_nPinTotMax);    //son230224_2
    m_nCardNoEnd = FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax, SysSet13.m_nPinBlockType); //son240125
#endif



	g_nErrCount1 = 0;
	g_nErrCountTot = 0;
	Sub71.nADCount1 = 1;//test
	nStop = 0;
	n4Wire = 0;
	nFlagMode1 = 0;
	nFail1 = 0;
	g_nStopFlag = 0;

	//  m_nTestCase1=m_combo2.GetCurSel();
	//  nCase=m_nTestCase1;


/*	str.Format("9.0");
	m_edit8.SetWindowText(str);
	m_edit8.GetWindowText( szText,10);
	m_dISOSet1 = atof( szText ); //sylee20120509
*/


//sylee151007  m_dISOSet1 = 9.0; //sylee20120509 

	m_dISOSet1 = 8.0; //sylee151007 //ACE400   SHIN BU SUNG

	////////////////////////////
	//	m_edit1.GetWindowText( szText,10); 
	//	m_nCardNoStart = atoi( szText );//????????????????????????	

	if (m_nCardNoStart < 1) {
		str.Format("Error Self,  Begin Card < 1 . \n \n  Set Error !     Begin Card=%d,    ", m_nCardNoStart);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}


	//-------------------------------------------------
	//son ī�尡 64pin���� 128 pin���� Ȯ���Ѵ�. 

	Get_nCardPinMax(); //sylee150903 //ACE400  //son231227

	//son 64�� pin ī���� ���
	if (nCardPinMax == 64) //sylee150903-1 //ACE400
	{
		if (m_nCardNoStart > 512)
		{
			str.Format("Error Self,  Begin Card > 512 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}
	}

	//son 128�� pin ī���� ���
	else
	{

		if (m_nCardNoStart > 256) {
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}
	}

	//			m_edit2.GetWindowText( szText,10); 
	//			m_nCardNoEnd = atoi( szText );//?????????????????????????

	if (m_nCardNoEnd < 1)
	{
		str.Format("Error Self,  End Card < 1 . \n \n  Set Error !    End Card=%d,    ", m_nCardNoEnd);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}

	Get_nCardPinMax(); //sylee150903  //ACE400  //son231227

	if (nCardPinMax == 64) //sylee150903-1 //ACE400
	{
		if (m_nCardNoEnd > 512)
		{
			str.Format("Error Self,  End Card > 512 ( 32k).  \n \nSet Error !    End Card=%d,    ", m_nCardNoEnd);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}
	}
	else
	{
		if (m_nCardNoEnd > 256)
		{
			str.Format("Error Self,  End Card > 256 ( 32k).  \n \nSet Error !    End Card=%d,    ", m_nCardNoEnd);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}
	}

	if (m_nCardNoEnd < m_nCardNoStart)
	{
		str.Format("Error Self,  End Card < Begin Card  . \n \n  Set Error !     Begin Card=%d,  End Card=%d,   ", m_nCardNoStart, m_nCardNoEnd);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}

	m_nCardNoStart = m_nCardNoStart - 1;
	m_nCardNoEnd = m_nCardNoEnd - 1;

	///////////////////////////////////////////////////////////////////////////
	// ���� ���� 

	strfName1.Format("\\SETUP\\001SelfRelay.csv");         //<=======================================
	DeleteFile(strfName1);
	::ZeroMemory(&fName1, sizeof(fName1));
	strcat(fName1, SysInfoView01.m_pStrFilePath1);//sylee20111212
	strcat(fName1, strfName1);

	fp1 = fopen(fName1, "wt");
	if (fp1 == NULL) {
		str.Format("Error    001SELFHVISO file open   ?  Error, Saving Error !     file close!   ");
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}

	m_nCC = (int)dRec[nCalMode][1];
	m_nVRel = (int)dRec[nCalMode][2];
	m_dVSet = dRec[nCalMode][3];
	m_nIR = (int)dRec[nCalMode][4];
	m_dISet = dRec[nCalMode][5];
	m_nFil = (int)dRec[nCalMode][8];
	m_nIFilter = (int)dRec[nCalMode][9];   //sylee130629		  
	m_nHVOffSet = (int)dRec[nCalMode][10]; //sylee130629
	m_nHVGain = (int)dRec[nCalMode][11];	//sylee130629

	if (m_dVSet > 20) {
		AfxMessageBox(" Error No 8312, SelfTest,Relay \n\n     Set check LV !,  4W is LV.  \n \n  Voltage SET=%d ? ", (int)m_dVSet);
		g_nSelfRunFlag = 0;
		return;
		//sylee130701   m_nFil= 0;//Fast speed
	}

	if ((m_nIR == 1) && (m_dISet > 10)) {
		AfxMessageBox(" Error No 8314, SelfTest,Relay \n\n    Set check !,  4W is I=1,  (m_dISet>10 ?  error ) . \n \n    I Range SET=%d? ", m_nIR);
		g_nSelfRunFlag = 0;
		return;
	}

	nTimeDelay1 = (int)(dRec[nCalMode][6] * 10);
	Sub71.nADCount1 = (int)dRec[nCalMode][7];

	if (Sub71.nADCount1 < 1) {
		Sub71.nADCount1 = 1;
	}
	if (Sub71.nADCount1 > 100) {
		Sub71.nADCount1 = 100;
	}

	nRet1 = Sub71.AOnPoweroff2();//sylee20120702
	if (nRet1 != 1) {
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 8315, \n   Power all off fail !,   SW TEST Run Start Error!!!");
		g_nSelfRunFlag = 0;
		return;
	}

	if (m_nVRel == 1) {
		nRet1 = Sub71.AOnVSetMulti1(m_dVSet, 0, 0, 0);	//son LV1 ����
	}
	else  if (m_nVRel == 2) {
		nRet1 = Sub71.AOnVSetMulti1(0, m_dVSet, 0, 0);	//son LV2 ����
	}

	if (nRet1 != 1) {
		//	fclose(fp);
		fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 1315, \n   V SET ERROR !,   SW TEST Run Start Error!!!");
		g_nSelfRunFlag = 0;
		return;
	}
	Sub71.Delay_100ns(3000000);  //300ms   

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//������ 	 

	tStart1 = GetMilSecond2();
	Lo1 = 3;
	n4Wire = 4;//sylee130702

Task101:

	Sub71.AOnReadVB1(m_nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
	if (m_dVSet < 5) {
		dVbR2 = m_dVSet * (0.35); //sylee211216
	}
	else {
		dVbR2 = m_dVSet * (0.1);//5%  sylee211216  ray  5->10%
	}

	dVbR1 = m_dVSet - dVbR2;
	dVbR2 = m_dVSet + dVbR2;

	if (Sub71.V1 > 300)
	{
		if (Lo1-- > 0) {
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}
		str.Format("Error No 1312,\n \n\n  Power Check? \n\n\n    Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%.2f, VB Read =%.2f,\n VB low=%.2f, VB high =%.2f,", (int)nCalMode, m_dVSet, Sub71.V1, dVbR1, dVbR2);
		AfxMessageBox(str);
		//	return ;
	}

	if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2) {

		if (Lo1-- > 0) {
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}

		str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),", (int)nCalMode, m_dVSet, Sub71.V1, dVbR1, dVbR2);

		AfxMessageBox(str);
		g_nSelfRunFlag = 0;
		//	return ;
	}


	//sylee150210 move    Sub71.AOnISetOn1(m_nCC, m_nVRel, m_dVSet, m_nIR, m_dISet);  // vb relay on ���   leakage�߻� �κ�   *************************

	//sylee150210 move    Sub71.Delay_100ns(3000000);  //300ms  

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	Sub71.AOnReadV1Enable1(m_nVRel);//sylee130704	//???? Ȯ�� �ʿ�
	m_nFil = 1;//all off//sylee130704
	Sub71.FilterSet(m_nFil);//sylee130628 
	m_nIFilter = 1;
	Sub71.I_Filter(m_nIFilter);//sylee130629 
	Sub71.OnPinAllReset1();  //sylee150210
	Sub71.AOnISetOn1(m_nCC, m_nVRel, m_dVSet, m_nIR, m_dISet);    //sylee150210 move
	Sub71.Delay_100ns(3000000);  //300ms     //sylee150210 move
	tStart2 = GetMilSecond2();


	nT[0] = (double)(tStart2 - tStart1) / 1000.;  //test 
	CTime curTime = CTime::GetCurrentTime();

	//       fprintf(fp1, "#MakeDate:   Year%04d:   Month%02d:   Day%02d    %02dH:%02dM:%02dS, \n\n\n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
	fprintf(fp1, "#   %04d/  %02d/  %02d  -  %02d: %02d: %02d  , \n\n\n",
		curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());


	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//������
	OnList1Mes1("                    ");
	str.Format("  ***  4W Check  Start ****   ");
	OnList1Mes1(str);

	fprintf(fp1, str);
	fprintf(fp1, "\n\n");
	::ZeroMemory(&nSta, sizeof(nSta));



	int card, cardPin;
	for (int testCase = 1; testCase <= 4; testCase++) //TEST CASE //sylee20120516
	{

		if (testCase == 1 || testCase == 3) {//  s1
			if (testCase == 3) {
				Sub71.Delay_100ns(10000); //sylee131102 	//son 1msec
			}
			Sub71.On4WS12Set(1, 1);	    //s1 self on //38  i46	
		}
		else if (testCase == 2 || testCase == 4) {//s2
			if (testCase == 4) {
				Sub71.Delay_100ns(10000); 	//son 1msec
			}
			Sub71.On4WS12Set(2, 1);		//s2  self on //39	i47 
		}


		int nCardNum;


#if 0   //son240125_4  ������  getCardNoEnd_NoSkip(nCardX, nPinTotMax, nPinBlockType) ȣ�������Ƿ� 
        //              �� ��ġ������ �ߺ� �ڵ忩��  �ڸ�Ʈ�� ������.

		//sylee160928  //YAMAHA  //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
		//if( SysSet13.m_nPinBlockType==5 ) 
		if (SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) { //son-32KPIN-20190902: 
			//sylee170922	m_nCardNoEnd=32+24-1;		   
			m_nCardNoEnd = 23;	//sylee170922	   
		}


		//sylee161115    0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
		//if( SysSet13.m_nPinBlockType== 4  ) 
		if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE) { //son-32KPIN-20190902: //son230216_1
			m_nCardNoEnd = 32 - 1;
		}


		//sylee171109-1    0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
		//if( SysSet13.m_nPinBlockType == 6 ) 
		if (SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421) { //son-32KPIN-20190902: 
			m_nCardNoEnd = 64 - 1;
		}

		//if( SysSet13.m_nPinBlockType==7 ) { //sylee180316-1 
		if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) { //sylee180316-1 
			m_nCardNoStart = 0;
			m_nCardNoEnd = 128 - 1;
		}
#endif


		for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++)
		{  //card no

			//sylee161108-1 if(	SysSet13.m_nSet4==1 ){//sylee160928  //YAMAHA  //sylee161108-1
			//sylee160928  //YAMAHA  //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
			//sylee170922  if( SysSet13.m_nPinBlockType==5) {
			//sylee170922	   if((card>=24) && (card<=31)){
			//sylee170922		   continue;
			//sylee170922	   }
			//sylee170922  }

			nLoop = 5;

			nCardNum = m_nCardNoEnd - m_nCardNoStart + 1;//sylee130704

			m_nProg4wIO_Rate = (int)((double)((((testCase - 1) * nCardNum) + (card - m_nCardNoStart + 1)) / ((double)(4. * nCardNum)) * 100.));



			for (cardPin = 0; cardPin < nCardPinMax; cardPin++)   //nCardPinMax=64  //sylee150210 //ACE380
			{

			Task1://sylee20120308

							//DoEvents();
				DoEvents();

				if (g_nStopFlag == 2) {
					g_nSelfRunFlag = 0;
					goto task2;
				}

				Sub71.OnPinAllReset1(); ///PIN ALL RESET
				//test	 Sub71.AOnReadV1I1(m_nVRel);//sylee20111115 
				ki = 0;
				//Task5:
				////////////////////////////////////////////////////////////////


				int nPin;//sylee151007 //ACE400

				//--------------------------------
				//son VB, VC SelfTest PIN  ���� 				<====
				//--------------------------------

                nPin = GetHwPinNo_Old(card, cardPin);     //son240125_2 �Լ� ȣ��� �ڵ� ����ȭ  //son240125_2


				//--------------------------------
				//son VB, VC SelfTest PIN  ���� 				
				//--------------------------------


				if (testCase == 3) {     // <=======
					//sylee200917 Sub71.AOnPinSet1(1, nPin, 3);  //  relay VB 3,    //nCardPinMax=64  //sylee150210 //ACE380
					//Sub71.AOnPinSet1(1, nPin, 4);  //  relay VB  ,	//sylee200917
					Sub71.AOnPinSet1(1, nPin, MOD_RY_SET_VB);  //  relay VB  //son210220 3->4 enum���� ����
				}
				else if (testCase == 4) {
					//sylee200917 Sub71.AOnPinSet1(1, nPin, 4);  //  relay vc 4,    //nCardPinMax=64  //sylee150210 //ACE380
					//Sub71.AOnPinSet1(1, nPin, 3);  //  relay vc  , 	//sylee200917
					Sub71.AOnPinSet1(1, nPin, MOD_RY_SET_VC);  //  relay vc   	//son210220 4 -> 3 enum���� ����
				}

				//sylee130802if( testCase==3 || testCase==4){
				//sylee130802 Sub71.Delay_100ns(10000); //     2mSec  //sylee130704   2mS->1mS
				//sylee130802} 

				//sylee200917 Sub71.AOnPinSet1(1, nPin, 1);  // vb tr on.   //nCardPinMax=64  //sylee150210 //ACE380 		      
				//Sub71.AOnPinSet1(1, nPin, 2);  // vb tr on.//sylee200917		      
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  // vb tr on //son210220 1 -> 2 enum���� ����

				Sub71.Delay_100ns(10000); //sylee131102 	//son 1msec
				//sylee131102	  Sub71.Delay_100ns(6000); //sylee130702     10mSec->1mS
				//Sub71.Delay_100ns(5000);//sylee130802	 
				//sylee130702

				//-------------------------------------
				//son Self 4W Test ����, ���� ����
				//-------------------------------------
				Sub71.AOnReadV1I1(m_nVRel);//sylee20111115 
				// Sub71.AOnReadV1I1(5);//sylee130702 


				//---------------
				//son R�� ���
				//---------------
				if (Sub71.Y1 < 0.0000001) {
					Sub71.Y1 = 0.0000001;
				}

				if (m_nIR == 1 || m_nIR == 2) {
					Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000.0;
				}
				else if (m_nIR == 3 || m_nIR == 4 || m_nIR == 5) {
					Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000000.0;
				}

				///////////  /////////////////////////////////////////////////////////////////////////////
				//if( n4Wire==4 ){
				//	Sub71.R1=Sub71.R1*1000;
				//}

				nRet1 = Sub71.OnPinAllReset1(); ///PIN ALL RESET  
				if (nRet1 != 1) {
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
					g_nSelfRunFlag = 0;
					return;
				}


				Sub71.Delay_100ns(10000); //1mSec              

				//son ù��° card ó�� ����ÿ�  ������ 5ȸ �ݺ�.
				if (card == m_nCardNoStart) {
					if (nLoop > 0) {  //sylee20120308
						if (nLoop == 1) {
							cardPin = 0;
						}
						nLoop--;
						goto Task1;
					}
				}

				if (testCase == 3 || testCase == 4) { //open test
					nTestMode = 0;
				}
				else if (testCase == 1 || testCase == 2) { //short test
					nTestMode = 2;
				}

				//-------------------
				// Self ShortTest	 
				//-------------------
				if (nTestMode == 2)
				{
					//-----------------------------------------------
					//son ���ذ����� ������ ���� �帥�ٸ� Short�̴�.

					//	if( Sub71.X1> m_dISOSet1 ){//fail log  0-> m_dISOSet1  0.61
					if (Sub71.X1 > 1)//fail log  0-> m_dISOSet1  0.61
					{
						if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1))
						{
							g_nErrCount1++;
							g_nErrCountTot++;
							nSta[card][cardPin][testCase] = 1;//cardPin(0~127)/ testCase  test case 1~6)
							dSelfErr1[g_nErrCount1][0] = (float)(card + 1);  //  CARD no.			    
							dSelfErr1[g_nErrCount1][1] = (float)(nPin + 1);  //  pin NO 	 //nCardPinMax=64  //sylee150210 //ACE380				  
							dSelfErr1[g_nErrCount1][2] = (float)testCase; // 1=  VB RELAY,   2= VC RELAY					  
							dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  //vb
							dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  //vc
							dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   //cardPin
							dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   //R1;  //double dR1[2][5000][3]; 

						}
						else {
							nSta[card][cardPin][testCase] = 1;//sylee151007//ACE400
							//sylee151007//ACE400	break;
						}
					}
				}

				//------------------
				// Self OpenTest			 
				//------------------
				else//(nIso1==1  
				{
					//--------------------------------------------------------------------------
					//son ���ذ����� ������ ���� �帣�ų� VC ���� ���� ������ �ȴٸ� open�̴�.
					//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۰ų� VC��(Sub71.X1)�� 8���� �۴ٸ�

					if (Sub71.X1 < m_dISOSet1) //sylee20120512    8v
					{
						if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1))
						{
							g_nErrCount1++;
							g_nErrCountTot++;
							nSta[card][cardPin][testCase] = 1;			 // cardPin(0~127)/ testCase  1~6)						
							dSelfErr1[g_nErrCount1][0] = (float)(card + 1);  // CARD no.			    
							dSelfErr1[g_nErrCount1][1] = (float)(nPin + 1);  //sylee151007//ACE400 //  pin NO 	//nCardPinMax=64  //sylee150210 //ACE380				  
							dSelfErr1[g_nErrCount1][2] = (float)testCase;  // 1=  VB RELAY,   2= VC RELAY					  
							dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  // vb
							dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  // vc
							dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;  // cardPin
							dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;  // R1;  //double dR1[2][5000][3];

						}
						else
						{
							nSta[card][cardPin][testCase] = 1;//sylee151007//ACE400
							//sylee151007//ACE400	break;
						}
					}

				}


				if (nSta[card][cardPin][testCase] > 0) {
					nSta[card][cardPin][0] += nSta[card][cardPin][testCase];
				}

			}////for END   for( cardPin=0; cardPin<128; cardPin++)   //pin 

		}   //for END  for( card=0; card< nCount; card++)  //CARD NO 

	}   // for( int testCase=1; testCase<=4; testCase++)




	//if(SysSet13.m_nPinBlockType==7){//sylee180316-1
	if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) //son-32KPIN-20190902: 
	{
		if (card < 64) { //64=128x64=8k
			nCardPinMax = 128;
		}
		else {
			nCardPinMax = 64;
		}
	}


	for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++)  //card no
	{
		for (cardPin = 0; cardPin < nCardPinMax; cardPin++)   //pin  //nCardPinMax=64  //sylee150210 //ACE380
		{
			if (nSta[card][cardPin][0] > 0)
				nFailSta[card]++;
		}

		if (nFailSta[card] > 0)
		{
			str.Format("  *  Error :4W Relay=>   Switch Card=%d   Fail=%d     ", card + 1, nFailSta[card]);
			OnList1Mes1(str);
			fprintf(fp1, str);  //<=======================================
			fprintf(fp1, "\n");
		}
	}




task2:  //STOP JUMP POSTION



	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else {
		if (g_nStopFlag == 2) {
			str.Format("Stop");
		}
		else {
			str.Format("OK");
		}
	}

	m_Label_4wIO.SetCaption(str);

	if (g_nErrCount1 > 0) {
		str.Format("  ---> NG");
	}
	else {
		if (g_nStopFlag == 2) {
			str.Format(" ----> Stop");
		}
		else {
			str.Format(" ----> OK");
			m_nProg4wIO_Rate = 100;//sylee170125
		}
	}


	OnList1Mes1(str);

	str.Format("  ****  4W Check  Finish ****  ");
	OnList1Mes1(str);

	if (g_nStopFlag == 2) {
		g_nSelfRunFlag = 0;
		g_nStopFlag = 0;
		str.Format("  * Stop   [Self Test  ::  4W RELAY ]  ");
		OnList1Mes1(str);
		AfxMessageBox(str);
	}

	fprintf(fp1, " \n  ");
	fprintf(fp1, "* SelfTest   RELAY SELF TEST    \n  ");  //<=======================================
	fprintf(fp1, " , No,  *CardNo         ,   Pin,  Total Pin ,      *Result* ,   VB,        VC,       I,         R,   \n ");


	int err;
	for (err = 1; err <= g_nErrCount1; err++)//sylee20111215
	{
		if (dSelfErr1[err][2] == 1)
		{
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S1 Short  ,",
				err, dSelfErr1[err][0], dSelfErr1[err][1] - ((dSelfErr1[err][0] - 1) * nCardPinMax),
				dSelfErr1[err][1]);  //nCardPinMax=64  //sylee150210 //ACE380
		}
		else  if (dSelfErr1[err][2] == 2)
		{
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S2 Short  ,",
				err, dSelfErr1[err][0], dSelfErr1[err][1] - ((dSelfErr1[err][0] - 1) * nCardPinMax),
				dSelfErr1[err][1]); //nCardPinMax=64  //sylee150210 //ACE380
		}
		else  if (dSelfErr1[err][2] == 3)
		{
			fprintf(fp1, " No=,   %d,     *%5.0f,    %5.0f,    %5.0f,  S1 Open  ,",
				err, dSelfErr1[err][0], dSelfErr1[err][1] - ((dSelfErr1[err][0] - 1) * nCardPinMax),
				dSelfErr1[err][1]);  //nCardPinMax=64  //sylee150210 //ACE380
		}
		else  if (dSelfErr1[err][2] == 4)
		{
			fprintf(fp1, " No=,   %d,     *%5.0f,   %5.0f,    %5.0f,  S2 Open  ,",
				err, dSelfErr1[err][0], dSelfErr1[err][1] - ((dSelfErr1[err][0] - 1) * nCardPinMax),
				dSelfErr1[err][1]); //nCardPinMax=64  //sylee150210 //ACE380
		}

		fprintf(fp1, "   %10.2f, %10.2f, %10.2f,  %12.2f, ",
			dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5], dSelfErr1[err][6]);
		fprintf(fp1, " \n");

		if (nDisplayDetail2 == 1)
		{
			//son231222_4 int ePin1 = nPinSW_Net((int)dSelfErr1[err][1]);
		    int hwPin0 = (int)dSelfErr1[err][1] - 1;    // 4W�� dSelfErr1[err][1] �� Pin val    //son231222_4
			int ePin1 = nJigPinMap[hwPin0] + 1;     //son231222_4

			str.Format("  *   4W  : No=%d,  NetPin=%d,  ( *CardNo= %3.0f, CardPin=%3.0f) ",
				err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1] - ((dSelfErr1[err][0] - 1) * nCardPinMax));
			str.Format("  *   4W  : No=%d,  NetPin=%d,  ( *CardNo= %3.0f, CardPin=%3.0f) ",
				err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1] - ((dSelfErr1[err][0] - 1) * nCardPinMax));
			OnList1Mes1(str);
		}
	}

	if (g_nErrCount1 < 1) {
		fprintf(fp1, "      ***NO ERROR *** ");
	}
	else {
		fprintf(fp1, "      ***  TOTAL ERROR  = %d  *** ", g_nErrCount1);
	}

	fclose(fp1);

	Sub71.OnPinAllReset1(); ///PIN ALL NO RESET  //SYEE20111018 
	Sub71.AOnPoweroff2();//sylee20120702

	Sub71.AOnRelayOff1();	//sylee20120616 


	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704
	Sub71.AOnReadV1Enable1(5); //sylee130711

	Sub71.Delay_100ns(20000);  //2ms 

	if (nStop == 2) {
		AfxMessageBox("   SelfTest::  4W Relay Check STop!!!");
	}

	if (nDisplayDetail1 != 1)
	{
		if (g_nErrCount1 >= 1)//sylee141029
		{
			if (SysSet13.m_nSet7 == 1) {//sylee130724
				::ShellExecute(NULL, "open", "EXCEl.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
			else {
				::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
		}
	}

	if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))//sylee151007 //ACE400
	{
		str.Format("  * =>  4W ERROR : %d  over. more error   ", MAX_SELF_ERR_PIN);
		OnList1Mes1(str);
	}


	Sub71.OnPinResetInit();//sylee151221   //ACE400
	Sub71.AOnPoweroff2();
	return;

}



//----------------------------
//son 32K ���� 2W SelfTest 
//----------------------------

//son-32KPIN-20190902:  32K ����

int g_nCardS[260][130];     //cardno, pin no 
							//son Self test Error�� �߻��� Cards[card][cardPin] ��ġ�� 
							//son 1�Ǵ� nProStep1��  ǥ���Ѵ�.

#define MAX_SELF_OPEN_ERR_PIN	200						
int g_nOpenErrPinCount;
int g_nOpenErrPin[MAX_SELF_OPEN_ERR_PIN + 1][5];
//	g_nOpenErrPin[g_nOpenErrPinCount][1]= (card+1);  //SWITCH CARD No.;
//	g_nOpenErrPin[g_nOpenErrPinCount][2]= (cardPin+1);  //SWITCH CARD Pin No.
//	g_nOpenErrPin[g_nOpenErrPinCount][3]= nPin+1;  //  SWITCH ��  PIN ;
//	g_nOpenErrPin[g_nOpenErrPinCount][4]= ModeSet12.make32kSwPin(nPin) +1;  //  BDL PIN ;
void CChildView5::OnAuto_Self1_32K()    //Switch card   All selftest  
{
	CString   str, str1, str2, strfName1;
	FILE* fp1;
	char  	fName[200], fName1[200];
	int 	ret;


	int nStepErrCount;
	int nCardX;
	int nDisplayDetail2;
	int nFail1;

	nDisplayDetail2 = 0;
	nPinSetSkipVb = 0; //son230127
	nPinSetSkipVc = 0; //son230127
	nCardX = 1;

	//son Critical Section ����.  �� flag�� ������ �� �� ���� ������ ������ �Ǿ�� �Ѵ�.
	//son �ٸ� SelfTest �����߿��� �������� �ʵ��� �ϴ� ��ġ�� ����
	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running?     IO  start error !!!");
		return;
	}

	//son SelfTest Delay Setting ���� ���� �´�.
	ret = FileSysInfo01.LoadSaveSet15(2);//load   //sylee140911
	if (ret != 1) {
		AfxMessageBox(" Error No 8212, \n\n  SysSet15.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n ");
		return;
	}
	g_nSelfRunFlag = 1;
	// OnButtonMessageClear();


	str.Format("  ***<  IO Check Start > ***  ");
	OnList1Mes1(str);
	str.Format("  ");		//son �� �� ���� 
	OnList1Mes1(str);

	str.Format("Run");
	m_Label_2wIO.SetCaption(str);

	DoEvents();


	::ZeroMemory(&g_nOpenErrPin, sizeof(g_nOpenErrPin));//sylee141019
	::ZeroMemory(&g_nCardS, sizeof(g_nCardS));
	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));
	::ZeroMemory(&dSelfErr1, sizeof(dSelfErr1));

	g_nOpenErrPinCount = 0;//
	g_nListCount = 0;

	g_nStopFlag = 0;
	g_nErrCount1 = 0;
	g_nErrCountTot = 0;
	nStop = 0;
	nFail1 = 0;
	g_nSelfTestInterlock = 0;//sylee131121


	g_nAutoFlag = 3;
	Sub71.nADCount1 = 1;//test

	//===================================================================
	//HWDEFINE

	strfName1.Format("d:\\log\\SelfAuto_2WTr.CSV");
	::ZeroMemory(&fName1, sizeof(fName1));

	strcat(fName1, strfName1);
	fp1 = fopen(fName1, "wt");

	if (fp1 == NULL) {
		//son221020 str.Format("Error    001SelfAuto_2WTr file open   ?  Error, Saving Error !     file close!   ");
		str.Format("Error  %s file open   ?  Error, Saving Error !     file close!   ", fName1);    //son221020
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}


	//===============================================
	//son   m_nCardNoStart,  m_nCardNoEnd  ����
	//===============================================
	m_nCardNoStart = 1;				//son 1
	//m_nCardNoEnd = 256*nCardX;	//son 256
	int box_num;
	box_num = FileSysInfo01.getEndBoxNo(SysSet12.m_nPinTotMax);        //son211020


	m_nCardNoEnd = (MAX_BOX_CARD * box_num) * nCardX;

	if (m_nCardNoStart < 1) {
		str.Format("Error Self,  StartCard(=%d) < 1 . \n \n  Set Error !", m_nCardNoStart);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}

	if (m_nCardNoEnd < 1) {
		str.Format("Error Self,  EndCard(=%d) < 1 . \n \n  Set Error !", m_nCardNoEnd);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}


	if (m_nCardNoEnd > (256 * nCardX)) {
		str.Format("Error Self,  EndCard(=%d) > (256*%d) ( 32k).  \n \nSet Error !", m_nCardNoEnd, nCardX);//ACE380
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	if (m_nCardNoEnd < m_nCardNoStart) {
		str.Format("Error Self,  Range Error! EndCard(=%d) < StartCard(=%d)  . \n\n", m_nCardNoEnd, m_nCardNoStart);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}


	//son Switch ī�尡 64pin���� 128 pin������ ���� ȯ�溯���� ���� 
	//    nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 �Ǵ� 128) ���� �����Ѵ�. 
	Get_nCardPinMax(); //sylee150903 //ACE400   //son231227

	if (nCardPinMax == 64) //sylee150903-1 //ACE400
	{

		if (m_nCardNoStart > 256) {
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}

	}
	else
	{
		if (m_nCardNoStart > 512) {
			str.Format("Error Self,  Begin Card > 512 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}
	}


	m_nCardNoStart = m_nCardNoStart - 1;     //son 0���� �����ϵ��� ����.
	m_nCardNoEnd = m_nCardNoEnd - 1;


	//====================
	//son  2W SelfTest
	//====================
	for (int nProStep1 = SELF_TEST_LV_OPEN; nProStep1 <= (SELF_TEST_MAX-1); nProStep1++)   //son240130 1:SELF_TEST_LV_OPEN, 17:(SELF_TEST_MAX-1)
	{
		//              ----------------------------------------------------------	-----------------------------------
		//                       			LV										               HV
		//              ----------------------------------------------------------	-----------------------------------
		//              open	short1	short2	  leak1  leak2    leakA   leakB	        leak1   leak2   leakA  leakB
		// nProStep		1			2		3		4(X)	5(X)   6		7		...	14		15		16		17
		// g_nMode		181 		181		181		181		181	   181	    181			182 	182		182		182	
		//
		// nCase        1			2		3		4		5	   6		7		...	4		5		6		7
		// m_dISOSet1	m_dISet*0.8	2		2		5		5	   5		5		...	40		500		500		500
		// nIso1		open(1)		short(2) short	short	short  short	short	...	short	short	short	short
		// nProCase1	1			2		2		2		2	   2		2		...	4		4		4		4
		nStepErrCount = 0;

		Sub71.OnPinAllReset1(); ///PIN ALL RESET

		//son Stop ��ư�� ���� �ִٸ� �������� �ʴ´�.
		if (g_nStopFlag == 2) {
			nProStep1 = SELF_TEST_MAX; //son 18:SELF_TEST_MAX
			continue;
		}

		Sleep(100);

		//SPARE   4, 5, 8, 9, 10, 11, 12, 13 ��  �ǳ� �ڴ�. 
		if (nProStep1 == SELF_TEST_LV_LEAK1 || nProStep1 == SELF_TEST_LV_LEAK2        //son240130 4:SELF_TEST_LV_LEAK1,  5:SELF_TEST_LV_LEAK2
			|| nProStep1 >= SELF_TEST_VC_1PIN_RST && nProStep1 <= 13)   //son240130 8:SELF_TEST_VC_1PIN_RST,  13: (SELF_TEST_HV_LEAK1 -1)
			continue;

        //---------------------------------------
        //son ProStep�� 32K ���� 2W SelfTest
        //---------------------------------------
		int ret = Do_ProStep_2wSelfTest_32K(nProStep1, 	        //son Input
			                                nStepErrCount);  	//son Output
		if (ret < 0)
		{
			Sub71.OnPinAllReset1();

			//son -1�� �����ϸ� for ������ break;  (goto task2 ��� ���) 
			//    err list�� ����ϰ� ������.
			if (ret == -1)		// task2:
			{
				// Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
				if (g_nStopFlag == 2)
				{
					//sylee131218
					g_nSelfRunFlag = 0;
					//	goto task3;
					break;
				}
			}

			//son -2�� �����ϸ� OnAuto_Self1_32K() �Լ��� Ż��  (return ��� ���)
			//    err list�� ������� �ʰ� ��� ������.
			else if (ret == -2)
			{
				fclose(fp1);
				return;
			}
		}

	}


	//----------------------
	//   Log
	//----------------------
	int nProStep2Tmp;
	int nErrorType[30];
	int nCase;

	::ZeroMemory(&nErrorType, sizeof(nErrorType));

	for (nProStep2Tmp = SELF_TEST_LV_OPEN; nProStep2Tmp <= (SELF_TEST_MAX-1); nProStep2Tmp++)  //son240130 1:SELF_TEST_LV_OPEN, 17:(SELF_TEST_MAX-1)
	{

		if (nProStep2Tmp <= 10) {
			nCase = nProStep2Tmp;
		}
		else {
			nCase = nProStep2Tmp - 10;
		}

		if (nProStep2Tmp == SELF_TEST_LV_OPEN) {    //son240130 1:SELF_TEST_LV_OPEN
			str.Format("--------------------------------------- ");
			OnList1Mes1(str);
		}


		int dd, ePin1/*,ePin2*/, ntmp1;

		int err;		// 	//son-32KPIN-20190902:  int i �� int err�� ����
		for (err = 1; err <= g_nErrCount1; err++) //sylee20111215
		{

			dd = err % 2;
			ntmp1 = (int)dSelfErr1[g_nErrCount1][7];  //nProStep1 (1:Open, 2:LV Short1, 3:LV Short2, 
                            //                                     4:LV Leak1,  5:LV Leak2,  6:LV LeakA,  7:LV LeakB,  
                            //                                    14:HV Leak1, 15:HV Leak2, 16:HV LeakA, 17:HV LeakB) 

			if (ntmp1 != nProStep2Tmp) {//sylee131212
				continue;
			}

			nErrorType[nCase]++;


			//ePin1=  nPinSW_Net((int)dSelfErr1[err][2]);  
			//ePin1 = ModeSet12.make32kSwPin((int)dSelfErr1[err][2] - 1) + 1;   //son230303
            int hwPin0 = (int)dSelfErr1[err][2] - 1;  // 2W�� dSelfErr1[err][2]�� Pin val.  //son230303
            ePin1 = nJigPinMap[hwPin0] + 1; //son230303	

			nProStep2Tmp = ntmp1;//sylee141106


			//-------------------------
			//son List â�� ����ϱ� 

			if (nDisplayDetail2 == 1)		// ���û���: nDisplayDetail2�� �� �Լ� ���� �պκп��� �� ������. (=0)
			{
				str.Format("No=%d, %s, Net Pin=%d, (Pin=%3.0f switchCard=%3.0f, cardPin=%3.0f)",
					         err, getStrSelfTestCase2(nProStep2Tmp),     //son240205
					         ePin1, dSelfErr1[err][2], dSelfErr1[err][0], dSelfErr1[err][1]);
				OnList1Mes1(str);
			}


			// err=g_nErrCount1;//sylee20120703
			if (nErrorType[nCase] == 1) {   //son ù��° error�� �� �� �߰�
				fprintf(fp1, "  \n\n ");
			}


			//-----------------------------------------
			//son Short ù��° Error�� ��� ��� ���.
			if (nErrorType[nCase] == 1 && nCase != SELF_TEST_OPEN) { //son240130 1:SELF_TEST_OPEN
				fprintf(fp1, "  ,  No , NetVbPin, NetVcPin, CardNo, CardPin,  VbPin,  VcPin,   VB,      VC,     I,            R,   \n ");//son240201
			}


			//-------------------------------
			//son Self OpenTest file print

			//son Open�� ���� ��� ���� ������ ���� ��¸� �Ѵ�.
			if (nCase == SELF_TEST_OPEN) //son240130 1:SELF_TEST_OPEN
			{

				//  fprintf(fp1, "         ,    No  ,   NetPin,   * CardNo, SW Pin, SW VcPin,  SW VbPinTotal, SW VcPinTotal,     VB,        VC,       I,            R,   \n ");	
				fprintf(fp1, "No=%d,  LV OPEN,  NetPin=%d, (switchCard=%3.0f, cardPin=%3.0f, nPin=%3.0f), Voltage=%3.1f(<=8V)\n",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2], dSelfErr1[err][4]);

			}

			//-------------------------------------
			//son Self Short Test file print
			else
			{
				//son Self ShortTest�� Pin �����ÿ� ���� pin�� VB����, VC ���� ǥ��
				//    VB�� �ٿ��� ���� VC�� �ٿ��� ����� ����� �ٸ��� �� �ش�. 
				int  nShortPin_VBVC = 0;  	// 0: VB,  1: VC

				//son �⺻�����δ� VB�� ���δ�.   (Self Short1, Leak1)
				nShortPin_VBVC = 0;  	// 0: VB

				//son Self Short2, Leak2 Test,   VC �� ���δ�.
				if (nCase == SELF_TEST_SHORT2 || nCase == SELF_TEST_LEAK2)   //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2
					nShortPin_VBVC = 1; // 1:VC

				//son Self Leak A
				else  if (nCase == SELF_TEST_LEAK_A)   //son240130 6:SELF_TEST_LEAK_A
				{
					//son cardPin ¦�� : VB
					if (((int)(dSelfErr1[err][1]) % 2) == 0)
						nShortPin_VBVC = 0;

					//son cardPin Ȧ�� : VC
					else
						nShortPin_VBVC = 1;

				}

				//son Self Leak B
				else  if (nCase == 7)   //son240130 7:SELF_TEST_LEAK_B
				{
					//son cardPin ¦�� : VC
					if (((int)(dSelfErr1[err][1]) % 2) == 0)
						nShortPin_VBVC = 1;

					//son cardPin Ȧ�� : VB
					else
						nShortPin_VBVC = 0;

				}
				// VB�� ���� ��� ���
				if (nShortPin_VBVC == 0)    //son 0:VB
					//            Case   No   netVbPin    *      *Card    CardPin  hwVbPin   *      VB      VC      I        R           //son230303
					fprintf(fp1, "%s  ,  %d,   %d,        *,     *%5.0f,   %5.0f,   %5.0f,   *,    %5.1f,  %5.1f,  %5.1f,  %12.1f,\n ",
                        getStrSelfTestCase2(nProStep2Tmp),  //son240205
						err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2],
						dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5], dSelfErr1[err][6]);

				// VC�� ���� ��� ���
				else
					//             Case    No       *   netVcPin  *Card    CardPin   *     hwVcPin    VB      VC      I        R	        //son230303
					fprintf(fp1, " %s  ,  %d,     *,     %d,    %5.0f,   %5.0f,     *,    %5.0f,  %5.1f,  %5.1f,  %5.1f,  %12.1f,\n",
                        getStrSelfTestCase2(nProStep2Tmp),  //son240205
						err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2],
						dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5], dSelfErr1[err][6]);

			}



		} //for( err=1; err<=g_nErrCount1; err++)//sylee20111215

	}//

	//sylee131122 g_nErrCount1=0;//sylee20120721
	//===================================================================================


	Sub71.OnPinAllReset1(); ///PIN ALL RESET

	Sub71.AOnPoweroff2();//sylee20120702  Sub71.AOnPoweroff();

	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704

	if (g_nStopFlag == 2)
	{

		//sylee131121	 g_nStopFlag=0;
		g_nSelfRunFlag = 0;
		g_nSelfAutoFlag1 = 0;

		str.Format("  * Stop   [Self Test  ::  Switch Card  TR ]  ");
		OnList1Mes1(str);
		AfxMessageBox(str);
	}


	if (g_nErrCountTot < 1) {
		fprintf(fp1, "    ***NO ERROR *** ");
	}
	else {
		fprintf(fp1, "    ***  TOTAL ERROR  = %d  *** ", g_nErrCountTot);
	}


	///==========================================================================
	//ending
	fprintf(fp1, "  \n  \n   ************************************************* ");
	fclose(fp1);

	if ((g_nStopFlag != 2) && (g_nErrCount1 < 1)) {//sylee131121
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;//sylee131121   
		//   AfxMessageBox("    Self Test.   OK!! ", MB_OK);
	}

	if (g_nErrCount1 > 0)//sylee131121  
	{
		//sylee140130 ::ShellExecute(NULL,"open","EXCEL.EXE",fName1,"NULL",SW_SHOWNORMAL);
		if (nDisplayDetail1 != 1) {
			if (SysSet13.m_nSet7 == 1) {//sylee130724
				::ShellExecute(NULL, "open", "EXCEL.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
			else {
				::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
		}
	}

	//}


	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else
	{

		if (g_nStopFlag == 2) {
			str.Format("Stop");
		}
		else {
			str.Format("OK");
			m_nProg2wIO_Rate = 100;//sylee170125
		}
	}

	m_Label_2wIO.SetCaption(str);



	if (g_nStopFlag == 2) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;//sylee131212 import
		if (m_SelfAutoFlag1 != 2) {//sylee141029
			g_nStopFlag = 0;//sylee131121	 
		}
	}



	//==========================================
	//ENDING



	g_nAutoFlag = 0;
	g_nSelfRunFlag = 0;

	///////////////////////////////////////////////////
	if (g_nOpenErrPinCount > 0)
	{
		strfName1.Format("d:\\log\\SelfError_EarthPin.CSV");         //<=======================================
		DeleteFile(strfName1);
		::ZeroMemory(&fName1, sizeof(fName1));
		strcat(fName1, strfName1);
		fp1 = fopen(fName1, "wt");
		if (fp1 == NULL) {
			str.Format("Error    d:\\log\\SelfError_EarthPin.CSV      file open   ?  Error, Saving Error !     file close!   ");
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}

		CTime curTime = CTime::GetCurrentTime();
		fprintf(fp1, " \n\n");

		/*
		   if( nG_Language!=2){//sylee150707

		   str.Format(  " *   �߻� �ð� : _%d�� %d�� %d��  _%d�� %d�� %d��  ",
						curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
						curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
		   fprintf(fp1,str);	 fprintf(fp1, "  \n\n");
		   str.Format( "  **>[���� �߻�]: ���� �ɿ� ����   ����������  �Ҽ� �����ϴ�.  ");
		   fprintf(fp1,str);
		   fprintf(fp1, " \n\n");
		   str.Format( "  **>[��ġ ���]:   ");
		   fprintf(fp1,str);
		   fprintf(fp1, " \n");

		   str.Format( "    1���) ���׸� Ż���� ���� ��������  ���׳��ο��� ������ ������ �ֽ��ϴ�." );
		   fprintf(fp1,str);    fprintf(fp1, " \n");
		   str.Format( "    2���) ���� ����ġ ī�� �ڽ��� ������ �ΰ� �Ǿ����� Ȯ���Ͻʽÿ�.  LED ���� ON Ȯ��.   " );
		   fprintf(fp1,str);  		  fprintf(fp1, " \n");
		   str.Format( "    3���) ����ġī�� ��ü�Ͽ� ����ȭ �Ǵ��� Ȯ���մϴ�.  "  );
		   fprintf(fp1,str);  		  fprintf(fp1, " \n");
		   str.Format( "    4���)  ���� �� ����ġ ī�忡��    �� �������� ����� ���̺� ���� ���� �и��Ͽ� ���� ����    " );
		   fprintf(fp1,str);            fprintf(fp1, " \n");
		   str.Format( "                ����� ���̺��� �ɺ�ϱ���  ������ �ֽ��ϴ�.    " );
		   fprintf(fp1,str);            fprintf(fp1, " \n");
		   str.Format( "                �̿Ͱ��� ����� �κ���  ���� �и��ϸ鼭 ��ġ��  ã�� �ذ��մϴ�. " );
		   fprintf(fp1,str);            fprintf(fp1, " \n");
		   }else{
		   */

		str.Format(" *  Error time :  %d/%d/%d - %d:%d:%d   ",
			curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
			curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
		fprintf(fp1, str);	 fprintf(fp1, "  \n\n");
		str.Format("   **> [Error : Cannot process Self OpenTest]:   ");
		fprintf(fp1, str);
		fprintf(fp1, " \n\n");
		str.Format("   **> [Check Points]:   ");
		fprintf(fp1, str);
		fprintf(fp1, " \n");

		str.Format("       Case1) Check the 'Pin Setting' status to see if MaxPin is set according to the number of PinBlocks.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");

		str.Format("       Case2) Ground Error. Remount the jig and test again.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case3) Make sure BBT Controller is powered on.  Check LED lamp of board. ");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case4) Change the SwitchCard and test again.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case5) Check if the flat Cable is disconnected.\n");
		fprintf(fp1, str);
		fprintf(fp1, " \n");


		fprintf(fp1, " \n\n\n");

		for (int err = 1; err <= g_nOpenErrPinCount; err++)
		{
			str.Format("  **> Error:  jigPin=%d, (pin=%d, switch card=%d, cardPin=%d)  ",
				//jigPin+1              nPin+1                 card+1                 cardPin+1
				g_nOpenErrPin[err][4], g_nOpenErrPin[err][3], g_nOpenErrPin[err][1], g_nOpenErrPin[err][2]);
			fprintf(fp1, str);
			fprintf(fp1, " \n");
			//	  OnList1Mes1(str1);  
		}

		if (g_nOpenErrPinCount >= MAX_SELF_OPEN_ERR_PIN)
		{
			fprintf(fp1, " \n\n");
			str.Format("  **> Open error pinCount(=%d) range Over!  (< %d)", g_nOpenErrPinCount, MAX_SELF_OPEN_ERR_PIN);
			fprintf(fp1, str);
			fprintf(fp1, " \n");
		}

		fclose(fp1);
		if (SysSet13.m_nSet7 == 1)//sylee130724
			::ShellExecute(NULL, "open", "EXCEl.EXE", fName1, "NULL", SW_SHOWNORMAL);

		else
			::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);

	}


	// str.Format("  ******<  Auto Self Test End > ******   ");										 
	str.Format("  ");		// ���� ����
	OnList1Mes1(str);
	str.Format("  ***<  IO Check Finish > ***   ");
	OnList1Mes1(str);


	if (nStop == 2) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;//sylee131212
		AfxMessageBox("   :  Switch Card 2W Pin Check.   Stop!!!");
	}



	Sub71.OnPinResetInit();//sylee151221   //ACE400

	Sub71.AOnPoweroff2();


}


//------------------------------------
//son ProStep�� 32K ���� 2W SelfTest
//------------------------------------
//
//son-32KPIN-20190902:  32K ����
//
// returnVal : 0���� �����ϸ� ����.
// 			   -1�� �����ϸ� for ������ break;  (goto task2 ��� ���) 
//             -2�� �����ϸ� OnAuto_Self1_32K() �Լ��� Ż��  (return ��� ���)
int CChildView5::Do_ProStep_2wSelfTest_32K(int nProStep1, 			// input
										int &rnStepErrCount) 	// output
{
	int	nRet, nRet1;
	int nCo1, nProCase1, nIso1;
	int nCase;
	int nLoop;
	int nStep1, nFail1, nPin;
	int nFailflag;
	CString str, str2;
	//int n4Wire = 0;

	int nEarthFlag;
	nEarthFlag = 0;


	if (nProStep1 == SELF_TEST_LV_OPEN || nProStep1 == SELF_TEST_HV_LEAK1)	//son 1:SELF_TEST_LV_OPEN, 14:SELF_TEST_HV_LEAK1
	{
		if (nProStep1 == SELF_TEST_LV_OPEN) //son240130 1:SELF_TEST_LV_OPEN
			g_nMode1 = MODE181_SELF_10V;  //lv 10v SelfTest  //sylee130701  //son240130 181:MODE181_SELF_10V

		else if (nProStep1 == 14)   //son240130 14:SELF_TEST_HV_LEAK1
		//sylee230821	g_nMode1 = MODE182_SELF_200V;  //hv  200v SelfTest  //sylee130701 //son240130 182:MODE182_SELF_200V
			g_nMode1 = MODE327_LEAK_250V; //sylee230821       //son240130 327:MODE327_LEAK_250V


		nRet = AutoSelf_PowerModeSet(g_nMode1);
		if (nRet == 0) {
			//fclose(fp1); 
			AfxMessageBox(" Error No 8023, \n    AutoSelf_PowerModeSet     SET ERROR !");
			return -1; 		//goto task2
		}

		                                                        //son   g_nMode		181					182
		m_nCC		= (int)dRec[g_nMode1][1];					//son 	m_CC		1 (CC)				1 (CC)
		m_nVRel		= (int)dRec[g_nMode1][2];   				//son   m_nVRel		1 (LV1)				3 (HV1)
		m_dVSet		= dRec[g_nMode1][3];						//son   m_dVSet		15 V				200 V
		m_nIR		= (int)dRec[g_nMode1][4];					//son	m_nIR		2 (I Range2)		4 (I Range4)
		m_dISet		= dRec[g_nMode1][5];						//son   m_dISet		100 				10000
		m_nFil		= (int)dRec[g_nMode1][8]; //sylee130706		//son   m_nFilter	1 (VC Fast)			2 (VC Slow)
		m_nIFilter 	= (int)dRec[g_nMode1][9]; //sylee130629		//son	m_nIFilter	1 (I Fast)			2 (I Slow)
		m_nHVOffSet	= (int)dRec[g_nMode1][10];//sylee130629		//son	m_nHVOffSet	0					0
		m_nHVGain	= (int)dRec[g_nMode1][11];//sylee130629		//son   m_nHVGain	0					0
	}
	m_nIGain = (int)dRec[g_nMode1][12];//sylee150904 //ACE400	//son   IGain		1 (I HighGain Off)	1


	Sub71.I_Gain(m_nIGain);//sylee220924 

	// nCase�� ���⼭ �����Ѵ�.
	if (nProStep1 <= 10) {
		nCase = nProStep1;
	}
	else {
		nCase = nProStep1 - 10;
	}

#if 1	// hdpark221005 �ű� ��� �������� ����. hdpark221026 ���󺹱�, �ű� ��� �׽�Ʈ �� �ʿ�

	switch(nProStep1)
	{
        case SELF_TEST_LV_OPEN:     //son240130 1:SELF_TEST_LV_OPEN
			m_dISOSet1=(m_dISet*0.8);
			nIso1=1;		//open
			nProCase1=1;
			break;

        case SELF_TEST_LV_SHORT1:	//son240130 2:SELF_TEST_LV_SHORT1, 
        case SELF_TEST_LV_SHORT2:   //son240130 3:SELF_TEST_LV_SHORT2
			m_dISOSet1=2;
			nIso1=2;		//short
			nProCase1=2;
			break;

        case SELF_TEST_LEAK1:      //son240130 4: SELF_TEST_LEAK1
        case SELF_TEST_LEAK2:      //son240130 5: SELF_TEST_LEAK2
        case SELF_TEST_LEAK_A:     //son240130 6: SELF_TEST_LEAK_A
        case SELF_TEST_LEAK_B:     //son240130 7: SELF_TEST_LEAK_B
			m_dISOSet1=5;
			nIso1=2;
			nProCase1=2;
			break;

        case SELF_TEST_HV_LEAK1:    //son240130 14:SELF_TEST_HV_LEAK1
			m_dISOSet1=40;//sylee121022  30->40   KWS
			nIso1=2;			 
			nProCase1=4;
			break;

        case SELF_TEST_HV_LEAK2:    //son240130 15:SELF_TEST_HV_LEAK2
        case SELF_TEST_HV_LEAK_A:    //son240130 16:SELF_TEST_HV_LEAK_A
        case SELF_TEST_HV_LEAK_B:    //son240130 17:SELF_TEST_HV_LEAK_B
			m_dISOSet1=500; //500;
			nIso1=2;
			nProCase1=4;
			break;

        case SELF_TEST_VC_1PIN_RST:     //son240130 8: SELF_TEST_VC_1PIN_RST
        case SELF_TEST_VB_1PIN_RST:     //son240130 9: SELF_TEST_VB_1PIN_RST
        case 10: case 11:    
        case 12: case 13:
		default:	
			m_dISOSet1=5;
			nIso1=2;	
			break;
	}
#else	// hdpark221005 begin, Cal Data�� �Ǻ��� �� �ְ���.
	int nCalRIndex = 0;
	int nJudgeMode = 0;

    //----------------------------------------------------------------------------
	// mode 181, 182�� cal�� ���ϹǷ� cal data�� ������ ��忡�� ������. 

    // LV Open
	if (nProStep1 == SELF_TEST_LV_OPEN) {	  //son240130 1:SELF_TEST_LV_OPEN
		g_nMode1 = MODE181_SELF_10V;
		nJudgeMode = MODE6_OPEN_10V;
		nCalRIndex = dRec[g_nMode1][14];
		FileSysInfo01.LoadSaveSer13(_LOAD, nJudgeMode, PART_UPPER);
		m_dISOSet1 = gdCal2[PART_UPPER][0][CAL_REF_RAWR][nCalRIndex]; // 15 = 10K
		nIso1 = 1;
		nProCase1 = 1;
	}

	// LV Short
	else if (nProStep1 <= SELF_TEST_LV_SHORT2) {	  //son240130 3:SELF_TEST_LV_SHORT2
		g_nMode1 = MODE181_SELF_10V;
		nJudgeMode = MODE206_HR_SHORT_10V;
		nCalRIndex = dRec[g_nMode1][15];
		FileSysInfo01.LoadSaveSer13(_LOAD, nJudgeMode, PART_UPPER);
		m_dISOSet1 = gdCal2[PART_UPPER][1][CAL_REF_RAWR][nCalRIndex]; // 19 = 100K
		nIso1 = 2;
		nProCase1 = 2;

	}
	// HV Leak
	else if (nProStep1 == SELF_TEST_HV_LEAK1 || nProStep1 == SELF_TEST_HV_LEAK2      //son240130 14:SELF_TEST_HV_LEAK1, 15:SELF_TEST_HV_LEAK2
	        || nProStep1 == SELF_TEST_HV_LEAK_A || nProStep1 == SELF_TEST_HV_LEAK_B) //son240130 16:SELF_TEST_HV_LEAK_A, 17:SELF_TEST_HV_LEAK_B
    {
		g_nMode1 = MODE182_SELF_200V;
		nJudgeMode = MODE327_LEAK_250V;
		nCalRIndex = dRec[g_nMode1][15];
		FileSysInfo01.LoadSaveSer13(_LOAD, nJudgeMode, PART_UPPER);
		m_dISOSet1 = gdCal2[PART_UPPER][2][CAL_REF_RAWR][nCalRIndex]; // 32 = 50M
		nIso1 = 2;
		nProCase1 = 4;
	}

/*
	int nCalRIndex = dRec[g_nMode1][15];

	switch (nProStep1)
	{
	//son open
    case SELF_TEST_LV_OPEN:  //son240130 1:SELF_TEST_LV_OPEN   
		g_nMode1 = MODE181_SELF_10V;    //son221020 181: MODE181_SELF_10V
		FileSysInfo01.LoadSaveSer13(_LOAD, g_nMode1, PART_UPPER);
		m_dISOSet1 = gdCal1[PART_UPPER][g_nMode1][CAL_REF_RAWR][nCalRIndex]; // 19, 100K

		nIso1 = 1;		//open
		nProCase1 = 1;
		break;

    //son short1, short2    
    case SELF_TEST_LV_SHORT1:	      //son240130 2:SELF_TEST_LV_SHORT1
    case SELF_TEST_LV_SHORT2:         //son240130 3:SELF_TEST_LV_SHORT2
		g_nMode1 = MODE181_SELF_10V;     //son221020 181: MODE181_SELF_10V
		FileSysInfo01.LoadSaveSer13(_LOAD, g_nMode1, PART_UPPER);
		m_dISOSet1 = gdCal1[PART_UPPER][g_nMode1][CAL_REF_RAWR][nCalRIndex]; // 19, 100K

		nIso1 = 2;		//short
		nProCase1 = 2;
		break;

    //son LV leak1, leak2, leakA, leakB
    case SELF_TEST_LV_LEAK1:         //son240130 4:SELF_TEST_LV_LEAK1
    case SELF_TEST_LV_LEAK2:         //son240130 5:SELF_TEST_LV_LEAK2
    case SELF_TEST_LV_LEAK_A:        //son240130 6:SELF_TEST_LV_LEAK_A
    case SELF_TEST_LV_LEAK_B:        //son240130 7:SELF_TEST_LV_LEAK_B
		g_nMode1 = MODE181_SELF_10V;    //son221020 181: MODE181_SELF_10V
		FileSysInfo01.LoadSaveSer13(_LOAD, g_nMode1, PART_UPPER);
		m_dISOSet1 = gdCal1[PART_UPPER][g_nMode1][CAL_REF_RAWR][nCalRIndex]; // 19, 100K

		nIso1 = 2;
		nProCase1 = 2;
		break;

    //son HV Leak1
    case SELF_TEST_HV_LEAK1:    //son240130 14:SELF_TEST_HV_LEAK1
		g_nMode1 = MODE182_SELF_200V; //son221020 182: MODE182_SELF_200V
		FileSysInfo01.LoadSaveSer13(_LOAD, g_nMode1, PART_UPPER);
		m_dISOSet1 = gdCal1[PART_UPPER][g_nMode1][CAL_REF_RAWR][nCalRIndex]; // 18, 100M

		nIso1 = 2;
		nProCase1 = 4;
		break;

    //son HV Leak2, LeakA, LeakB
    case SELF_TEST_HV_LEAK2:     //son240130 15:SELF_TEST_HV_LEAK2
    case SELF_TEST_HV_LEAK_A:    //son240130 16:SELF_TEST_HV_LEAK_A
    case SELF_TEST_HV_LEAK_B:    //son240130 17:SELF_TEST_HV_LEAK_B
		g_nMode1 = MODE182_SELF_200V; //son221020 182: MODE182_SELF_200V
		FileSysInfo01.LoadSaveSer13(_LOAD, g_nMode1, PART_UPPER);
		m_dISOSet1 = gdCal1[PART_UPPER][g_nMode1][CAL_REF_RAWR][nCalRIndex]; // 18, 100M

		nIso1 = 2;
		nProCase1 = 4;
		break;

    case SELF_TEST_VC_1PIN_RST:     //son240130 8:SELF_TEST_VC_1PIN_RST
    case SELF_TEST_VB_1PIN_RST:     //son240130 9:SELF_TEST_VB_1PIN_RST
	case 10: case 11: case 12: case 13:
	default:
		g_nMode1 = MODE181_SELF_10V;    //son221020 181:MODE181_SELF_10V
		FileSysInfo01.LoadSaveSer13(_LOAD, g_nMode1, PART_UPPER);
		m_dISOSet1 = gdCal1[PART_UPPER][g_nMode1][CAL_REF_RAWR][nCalRIndex]; // 19, 100K
		nIso1 = 2;
		break;
	}
*/
#endif	// hdpark220819 end	

	DoEvents();

	//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
	if (g_nStopFlag == 2) {
		g_nSelfRunFlag = 0;
		return -1;	//goto task2
	}


	//--------------------
	//son InterI ����
	double InterI; //sylee20120612

	if (m_nIR == 1 || m_nIR == 2) {  //mA 
		InterI = m_dISet * 0.9;
	}
	else if (m_nIR == 3 || m_nIR == 4) {   // uA 
		InterI = m_dISet * 0.9;
	}
	else if (m_nIR == 5) {//uA
		InterI = 10.0;
	}

	nCo1 = 0;

	//------------------------------------------------------------
	//son SelfTest�� HW SwitchCard�� �����ϴ� ���̹Ƿ� 
	//    Jig�� ������� hwPin�� �������� ������ ������ �Ѵ�.  
	//------------------------------------------------------------

	int nCardNum;
	nCardNum = m_nCardNoEnd - m_nCardNoStart + 1;		// ī�� ����  : 32K���� 256

	int card;
	for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++)  //card no (0~ )
	{
		//son191206 Max ErrCount ������ �Ѿ�ٸ� ī�� ������ ������. 
		if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))//sylee160514-2 
#ifdef CHILDVIEW5_ERR_NO_COUNT  //son240521 �׽�Ʈ�� 
            g_nErrCount1 = 0;   //for debug
#else
			break;
#endif

		//son-32KPIN-20200228 : 16K ��� ���� �ΰ��� skip
		//son 16K�� ���� ��� 2box(box1, box2)��, �Ϻ� 2box(box5, box6) ���.
		//    box3�� box4�� �ǳ� �پ�� �Ѵ�.  box3: card64 ~ card95 
		//                                     box4: card96 ~ card127
		//if (SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS   ////son230216_1
		//		|| SysSet12.m_nPinTotMax == PIN_TOT_12K	            //son210202
		//        || SysSet12.m_nPinTotMax == PIN_TOT_12K_FULL_TRANS) //son210319
		//if (IS_UPPER_8K(SysSet12.m_nPinTotMax))     //son210531
		if (ModeSet12.IsUpper8k(SysSet12.m_nPinTotMax))     //son210831
		{
			if (card >= 64 && card < 128)
				continue;
		}

		//son-8K4097-20200615 : 8K_����4097  ��� ����  1 box�� skip
		//son 8K_����4097�� ���� ��� 1box(box0)��, �Ϻ� 1box(box2) ���.
		//    box1�� �ǳ� �پ�� �Ѵ�.  box1: card32 ~ card63   128pin card
		//if (SysSet12.m_nPinTotMax == PIN_TOT_8K_LOW_STPIN || SysSet12.m_nPinTotMax == PIN_TOT_8K_TRANS)  //son210319 //son230216_1
		//if (IS_UPPER_4K(SysSet12.m_nPinTotMax))     //son210531
		if (ModeSet12.IsUpper4k(SysSet12.m_nPinTotMax))     //son210831
		{
			if (card >= 32 && card < 128)
				continue;
		}

        //son210531: ��� 3 box, �Ϻ� 1 box, ��� 4��° box�� skip
        //    box 4�� �ǳ� �پ�� �Ѵ�.      box 4: card96 ~ card127
        //if (IS_UPPER_12K(SysSet12.m_nPinTotMax))     //son210531
        if (ModeSet12.IsUpper12k(SysSet12.m_nPinTotMax))     //son240522
        {
            if (card >= 96  && card < 128)
                continue;
        }


		nCo1++;
		//son240130 if (nCase > 3 && nCase < 8)// HW INTERLOCK		 
		if (nCase >=SELF_TEST_LEAK1 && nCase <= SELF_TEST_LEAK_B)//son240130  4:SELF_TEST_LEAK1, 7:SELF_TEST_LEAK_B
		{
			if (nCo1 > 8)
			{
				nCo1 = 0;
				Sub71.OnPinAllReset1(); ///PIN ALL RESET
				Sub71.Delay_100ns(100);  //10us 
			}
		}
		nLoop = 5;
		nFail1 = 0;

		double dRate1, dRate2;
		switch (nProCase1)
		{
		case 1:	dRate1 = 0.3;  dRate2 = 0;		    break;
		case 2: dRate1 = 0.5;  dRate2 = 0.3;    	break;
		case 3: dRate1 = 0.7;  dRate2 = 0.5;		break;
		case 4: dRate1 = 1;    dRate2 = 0.7;		break;
		case 5: dRate1 = 1;    dRate2 = 0.8;		break;

		}

		m_nProg2wIO_Rate = (int)((((double)(card - m_nCardNoStart + 1) / (double)nCardNum * 100)) * dRate1 + dRate2);

		//================
		int cardPin;
		for (cardPin = 0; cardPin < nCardPinMax; cardPin++)  	//son nCardPinMax: 32K�϶�  128
		{

			//son �̹� Self test Error�� �߻��� ī���� pass�Ѵ�. 
			if (g_nCardS[card + 1][cardPin + 1] > 0)
				continue;

#ifndef CHILDVIEW5_ERR_NO_COUNT  //son240521 �׽�Ʈ�� �ӽ�
			//son ErrCount ������ �Ѿ�ٸ� ī���� ������ ������ ���� ī��� �Ѿ� ����.
			if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))//sylee160514-2 
				break;
#endif

			DoEvents();

			//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
			if (g_nStopFlag == 2)
			{
				g_nSelfRunFlag = 0;
				return -1;		//goto task2	
			}

			if (nStop == 2)
			{
				AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
				return -1;		//goto task2
			}

            //son Leak�� �ƴϸ�
			if (nCase != SELF_TEST_LEAK1 && nCase != SELF_TEST_LEAK2             //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
			        && nCase != SELF_TEST_LEAK_A && nCase != SELF_TEST_LEAK_B)   //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
				Sub71.OnPinAllReset1(); ///PIN ALL RESET


			//son LV Short1, Short2 �� ��� 
			if (nCase == SELF_TEST_SHORT1 || nCase == SELF_TEST_SHORT2) //short1,2  //son240130 2:SELF_TEST_SHORT1, 3:SELF_TEST_SHORT2
				Sub71.Delay_100ns(10000); //1mS  //sylee120921



			//--------------------------------
			//son VB, VC SelfTest PIN  ���� 				<====
			//--------------------------------

			//son for SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS, 20K, 24K
			nPin = cardPin + card * nCardPinMax;     //son ī�忡 offset�� ���ؼ� �Ϻν������� ���ϴ� ���
			                                         //    user�� ī���ȣ�� �ٸ��Ƿ� ��½ÿ��� getPrintCardNo()�� ȣ���ؼ� card��ȣ�� ��ȯ�ؾ�

			//son �ش� hwPin�� �´� jigPin�� ���ٸ� PinMap�� ���� pin�̹Ƿ� pass�Ѵ�. 
			//int jigPin = ModeSet12.make32kSwPin(nPin);   //son210831
			int jigPin = nJigPinMap[nPin]; //son230303
			if (jigPin < 0)     //son210831
				continue;

			int nTime1 = 0;
			switch (nCase)
			{
				//-------------------------------------------------------------------------------------
				//son Self OpenTest :  ���� ����Ʈ�� VB, VC�� �ٿ��� (short��Ȳ�� ���� ) ������ �帣���� Ȯ���Ѵ�. 
				//                     ������ �� �帥�ٸ� ����, ������ �������Ϸ� �帥�ٸ� open�̴�.
				//-------------------------------------------------------------------------------------
            case SELF_TEST_OPEN: //  VB  VC  1��1 ���� �˻� //  LV ����  //son240130 1:SELF_TEST_OPEN
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);    //son210220 2->1 enum���� ����
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);    //son210220 1->2 enum���� ����
				nTime1 = 3500;
			//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet2 * 10 < 0) ? 0 : (SysSet15.m_nSet2 * 10);
				break;



				//-------------------------------------------------------------------------------------
				//son Self ShortTest :  ����Ʈ�� VB, �Ǵ� VC�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                      ������ ���� ���Ϸ� �帣�ų� �帣�� �ʴ´ٸ� ����.
				//                      VB�� �ٿ��µ�(�Ǵ� VC�� �ٿ��µ�)  ������ �帥�ٸ� short�̴�. 
				//-------------------------------------------------------------------------------------
            case SELF_TEST_SHORT1: //  case 2= short1,    pin one      //son240130 2:SELF_TEST_SHORT1
				//Sub71.AOnPinSet1(1, nPin, 1);    //son VB�� ���δ�.
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);    //son VB�� ���δ�.  //son210220 1->2 enum���� ����
				nTime1 = 1000;
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet3 * 10 < 0) ? 0 : (SysSet15.m_nSet3 * 10);
				break;

            case SELF_TEST_SHORT2: //  case3= short2,      //son240130 3:SELF_TEST_SHORT2
				//Sub71.AOnPinSet1(1, nPin, 2);    //son VC�� ���δ�.
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);    //son VC�� ���δ�.  //son210220 2->1 enum���� ����
				nTime1 = 1000;
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet3 * 10 < 0) ? 0 : (SysSet15.m_nSet3 * 10);
				break;


            case SELF_TEST_LEAK1: //  case4= leak 1,    pin multi  //son240130 4:SELF_TEST_LEAK1
				//Sub71.AOnPinSet1(1, nPin, 1);    //son VB�� ���δ�.  
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);    //son VB�� ���δ�.  //son210220 1->2 enum���� ����
				nTime1 = 1000;				//150uS 
//sylee230912				if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet4 * 10 < 0) ? 0 : (SysSet15.m_nSet4 * 10);
				break;


            case SELF_TEST_LEAK2: //    case 5= leak2   pin multi //son240130 5:SELF_TEST_LEAK2
				//Sub71.AOnPinSet1(1, nPin, 2);    //son VC�� ���δ�.  
				Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);    //son VC�� ���δ�.  //son210220 2->1 enum���� ����
				nTime1 = 1000;				//100uS
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet4 * 10 < 0) ? 0 : (SysSet15.m_nSet4 * 10);
				break;


            case SELF_TEST_LEAK_A: //leak A            //son240130 6:SELF_TEST_LEAK_A
				nStep1 = cardPin % 2;
				if (nStep1 == 0)
					//Sub71.AOnPinSet1(1, nPin, 1);    //son cardPin ¦�� : VB  
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);    //son cardPin ¦�� : VB  //son210220 1->2 enum���� ����
				else
					//Sub71.AOnPinSet1(1, nPin, 2);    //son cardPin Ȧ�� : VC  
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);    //son cardPin Ȧ�� : VC  //son210220 2->1 enum���� ����

				nTime1 = 20000;				//sylee150609  ray  500uS->2mS
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet5 * 10 < 0) ? 0 : (SysSet15.m_nSet5 * 10);
				break;


            case SELF_TEST_LEAK_B: //LEAK B	    //son240130 7:	SELF_TEST_LEAK_B							
				nStep1 = cardPin % 2;
				if (nStep1 == 0)
					//Sub71.AOnPinSet1(1, nPin, 2);    //son cardPin ¦�� : VC 
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);    //son cardPin ¦�� : VC //son210220 2->1 enum���� ����
				else
					//Sub71.AOnPinSet1(1, nPin, 1);    //son cardPin Ȧ�� : VB 
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);    //son cardPin Ȧ�� : VB //son210220 1->2 enum���� ����

				nTime1 = 20000;				//sylee150609  ray  500uS->2mS
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = (SysSet15.m_nSet5 * 10 < 0) ? 0 : (SysSet15.m_nSet5 * 10);
				break;


            case SELF_TEST_VC_1PIN_RST: // CASE6    pin     //son240130 8: SELF_TEST_VC_1PIN_RST
				nTime1 = 3500;
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = 2500;
				break;


            case SELF_TEST_VB_1PIN_RST:     //son240130 9:SELF_TEST_VB_1PIN_RST
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = 2500;
				break;


			default:
//sylee230912	if (SysSet15.m_nSet6 == 7)		// SelfTest Delay Use Set.  7=USE
					nTime1 = 5000;
				break;

			}
			Sub71.Delay_100ns(nTime1);
			//////////////////////////////////////////////////////////////////////////////////////////////////////

		    int nPrintCard = FileSysInfo01.getPrintCardNo(SysSet12.m_nPinTotMax, card);     //son231213 ��¿� ī�� ��ȣ

			//-------------------------------------
			//son Self ShortTest ����, ���� ����
			//-------------------------------------
			if (nCase != SELF_TEST_OPEN) //son Open�� �ƴ϶��   //son240130 1:SELF_TEST_OPEN
			{

				Sub71.AOnReadV1I1(m_nVRel); 		//son ����, ���� ����.
#ifdef _2W_SHORT_TEST_
				// short �߻���
				Sub71.Y1 = 5;
				Sub71.X1 = 10;
#endif
				//son Self ShortTest�ε� InterI���� ū ������ ������ �ȴٸ� ���� Short �׽�Ʈ�� �ߴ�. 
				if (Sub71.Y1 > InterI)
				{
					Sub71.OnPinAllReset1(); ///PIN ALL RESET  
					//  Sub71.Delay_100ns(10000);  //1ms  relay
					Sub71.Delay_100ns(100);  //10us 
					if (m_nVRel < 3)
						str.Format("Error No 8211-1,\n\n   lv short     \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f \n",
							nPrintCard + 1, cardPin + 1, Sub71.Y1, InterI);   //son231213  card -> nPrintCard

					else
						str.Format("Error No 8211-2,\n\n   hv short      \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f \n",
							nPrintCard + 1, cardPin + 1, Sub71.Y1, InterI);   //son231213  card -> nPrintCard

					//AfxMessageBox(str);

					nRet1 = AfxMessageBox(str, MB_YESNO | MB_ICONEXCLAMATION);

                    //son240125_2 �޽����ڽ� No Ŭ���ÿ� List�� ��� �ȵǴ� ���� ����. 
                    //            �Լ��� ����� List �� ����� �ȵǹǷ� �Լ� ����� �ʰ� ��� �����ϵ��� ����. 
					//son240125_2 if (nRet1 != 6)		//son Yes ��ư�� �ƴ϶�� �Լ��� �����.
					//son240125_2 {
					//son240125_2 	g_nStopFlag = 2;	//sylee140701
					//son240125_2 	return -1;		// goto task2;
					//son240125_2 }

					//son AfxMessageBox���� No�� �������� �ʾҴٸ� ��� ����
					//    Sub71.AOnReadV1I1(m_nVRel);�� �ٽ� ���������� �ʴ´�.
				}
				else
				{
					Sub71.Delay_100ns(100);  //10uS 

					/*if( n4Wire==4 ) //sylee20111118
					  Sub71.AOnReadV1I1(5); // READ  4W V  , I
					else*/
					Sub71.AOnReadV1I1(m_nVRel);//sylee2011111	
#ifdef _2W_SHORT_TEST_
				// short �߻���
					Sub71.Y1 = 5;
					Sub71.X1 = 10;
#endif
				}
			}

			//-------------------------------------
			//son  Self OpenTest ����, ���� ����
			//-------------------------------------
			else
			{
				Sub71.Delay_100ns(100);  //10uS 

				/*if (n4Wire == 4) //sylee20111118
					Sub71.AOnReadV1I1(5); // READ  4W V  , I
				else */
				Sub71.AOnReadV1I1(m_nVRel);//sylee20111115 
#ifdef _2W_SHORT_TEST_
				// open  ���� 
				Sub71.Y1 = 10;
				Sub71.X1 = 10;
#endif
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////// 					



			//--------------------
			//son  R �� ���
			//--------------------

			if (Sub71.Y1 < 0.0000001) {
				Sub71.Y1 = 0.0000001;
			}


			//sylee150909 //ace400	if( m_nIR==1 || m_nIR==2 ){
			if (m_nIR == 1 || m_nIR == 2 || m_nIR == 3 || m_nIR == 4)	//sylee150909 //ace400
			{
				Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000.0;
				//sylee150909 //ace400	}else if( m_nIR==3 ||  m_nIR==4  ||  m_nIR==5 ){
			}
			else if (m_nIR == 5)	//sylee150909 //ace400	
				Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000000.0;


			if (m_nIR == 4)	//sylee150909 //ace400	
				Sub71.Y1 = Sub71.Y1 * 1000.0;

#if 0  //hdpark221026 �缳��, �ű� ��� �׽�Ʈ �� �ʿ�
			// hdpark221005 begin, Cal Data�� �Ǻ�
			if (nProStep1 != 1)
			{
				Sub71.Y1 = dIADC1; //son231012 int nIADC1  -> double dIADC1
				//son221014 Sub71.R1 = RCalConvert_spark(g_nMode1, Sub71.Y1, RAW_TO_REAL, PART_UPPER);  //son �������� Convert
				//son230925 Sub71.R1 = RCalConvert_short(g_nMode1, Sub71.Y1, RAW_TO_REAL, PART_UPPER);  //son221014 RCalConvert_spark()��  RCalConvert_short()�� ��ü, �Լ�����
				Sub71.R1 = RCalCovert(g_nMode1, Sub71.Y1, RAW_TO_REAL, PART_UPPER);  //son230925 RCalConvert_short-> RCalCovert �� ����
			}
			// hdpark221005 end
#endif			

            //son Leak�� �ƴ϶��
			if (nCase != SELF_TEST_LEAK1 && nCase != SELF_TEST_LEAK2               //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
			        && nCase != SELF_TEST_LEAK_A && nCase != SELF_TEST_LEAK_B )    //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
			{
				nRet1 = Sub71.OnPinAllReset1(); ///PIN ALL RESET  
				if (nRet1 != 1) {
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
					g_nSelfRunFlag = 0;
					return  -2;
				}
			}


			//son ù��° card ó�� cardPin ����ÿ�  ������ 5ȸ �ݺ�.
			if (card == m_nCardNoStart)
			{
				if (nLoop > 0) {  //sylee20120308
					/*if(nLoop==1){
						cardPin=0;
					}
					else*/
					cardPin--;		//son191206: �����ڵ�� cardPin loop ó�� �κп� Task1���� �����ϹǷ�
									//    cardPin�� 0�� ���������� continue�� �ϸ� 1�� �þ�Ƿ� 
									//    �þ ���� �����ؼ� -1�� �ϰ� �����ϸ� nLoop 5ȸ �ݺ��ϴ� ���� cardPin�� 
									//    0�� ������ �� �ִ�. 
					nLoop--;
					continue;		// goto Task1; 
				}
			}


			//---------------------------------
			//son Self ShortTest Error ����
			//---------------------------------
			if (nIso1 == 2) //son Short�� Leak�̸�
			{
				//-----------------------------------------------
				//son ���ذ����� ������ ���� �帥�ٸ� Short�̴�.
				if (Sub71.Y1 > m_dISOSet1)//fail log  0-> m_dISOSet1
				{

					g_nCardS[card + 1][cardPin + 1] = nProStep1;

#ifndef CHILDVIEW5_ERR_NO_COUNT  //son240521 �׽�Ʈ�� �ӽ�
					//son ErrCount ������ �Ѿ�ٸ� ���� ī��� �Ѿ� ����.
					if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))
						break;
#endif

					g_nErrCount1++;
					g_nErrCountTot++;
					rnStepErrCount++;

					dSelfErr1[g_nErrCount1][0] = (float)(nPrintCard + 1);    // CARD no.  //son231213  card -> nPrintCard
					dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1); // cardPin
					dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);	// nPin
					dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;   // vb val
					dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;   // vc val
					dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   // i val
					dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   // R1 val  //double dR1[2][5000][3];
					dSelfErr1[g_nErrCount1][7] = (float)nProStep1;//sylee131212

					nFail1++;
				}

			}

			//-----------------------------
			//son Self OpenTest Error ����
			//-----------------------------
			else //son nIso1==1  �� ���
			{

				nFailflag = 0;

				// nIso1==1 �̹Ƿ� nCase�� 1�� �ƴ� �� ����.  ������ġ�� ��. 
				if (nCase == SELF_TEST_OPEN)	//son240130 1:SELF_TEST_OPEN	    
				{
					//--------------------------------------------------------------------------
					//son ���ذ����� ������ ���� �帣�ų� VC ���� ���� ������ �ȴٸ� open�̴�.
					//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۰ų� VC��(Sub71.X1)�� 8���� �۴ٸ�
					if ((Sub71.Y1 < m_dISOSet1) || (Sub71.X1 < 8))
						nFailflag = 1;


					//----------------------------------------
					//son  ���������� ������ ������ ����Ѵ�.

					//son 10V �����ߴµ� 8V ���Ϸ� ������ ���  (Open Self Test ���� �Ұ�)
					if (Sub71.X1 < 8)
					{
						if (nEarthFlag == 0)
						{
							nEarthFlag = 1;
							str.Format(" *> [Error : Cannot process Self OpenTest]:   ");
							OnList1Mes1(str);
							str.Format(" *> [Check point]:   ");
							OnList1Mes1(str);
							str.Format(" *> Case1) Ground Error. Remount the jig and test again.");
							OnList1Mes1(str);
							str.Format(" *> Case2) Change the Switch Card and test again.");
							OnList1Mes1(str);
							str.Format(" *> Case3) Check if the Flat Cable is disconnected.");
							OnList1Mes1(str);
							str.Format("   ");	 //son ���� �� ����ؼ� �ٰ��� ����
							OnList1Mes1(str);

						}
					}

				}



				//------------------------------
				//son Open SelfTest Error ���� 
				//------------------------------

				//son191206: ??? (nIso1==2) �� �ƴϸ� �ݵ�� 1�̹Ƿ� Open�̴�.
				//           ������ nCase �� 1�� �ƴ� ���̽� ��, short �� ���̽��� ���� �ڵ��̹Ƿ�  
				//           ���������� ���� �ڵ带 ������ ���� ���� ����..  ���ʿ���  �ڵ�?
				else
				{
					//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۴ٸ�
					if (Sub71.Y1 < m_dISOSet1)
						nFailflag = 1;

				}

				if (nFailflag == 1) //sylee20120512							 
				{
					//if( Sub71.Y1< m_dISOSet1){ //sylee20120512 
					g_nCardS[card + 1][cardPin + 1] = 1;

#ifndef CHILDVIEW5_ERR_NO_COUNT  //son240521 �׽�Ʈ�� �ӽ�
					//son ErrCount ������ �Ѿ�ٸ� ī���� ������ ������ ���� ī��� �Ѿ� ����.
					if (g_nErrCount1 >= (MAX_SELF_ERR_PIN - 1))//sylee160514-2 
						break;
#endif


					g_nErrCount1++;
					g_nErrCountTot++;
					rnStepErrCount++;

					dSelfErr1[g_nErrCount1][0] = (float)(nPrintCard + 1);  //  CARD no.  //son231213  card -> nPrintCard
					dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1);  //vb  pin
					dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);  //sylee151007 //ACE400//vb  pin  //	  nCardPinMax;//sylee150210 //ACE380
					dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  //vb
					dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  //vc
					dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   //i
					dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   //R1;  //double dR1[2][5000][3];
					dSelfErr1[g_nErrCount1][7] = (float)nProStep1;//sylee131212

					nFail1++;

					//son Open Case
					if (nCase == SELF_TEST_OPEN)//sylee141019	//son191206: �� ��ġ�� �翬�� OpenCase�̴�.	    //son240130
					{
						if (g_nOpenErrPinCount < MAX_SELF_OPEN_ERR_PIN)
						{
							if ((Sub71.X1 < 2) && (Sub71.Y1 < 2)) //sylee141019
							{
								g_nOpenErrPinCount++;//sylee141019
								//son231213 g_nOpenErrPin[g_nOpenErrPinCount][1] = (card + 1);  //SWITCH CARD No.;//sylee141019
								g_nOpenErrPin[g_nOpenErrPinCount][1] = (nPrintCard + 1);  //son231213  card -> nPrintCard
								g_nOpenErrPin[g_nOpenErrPinCount][2] = (cardPin + 1);  //SWITCH CARD Pin No.//sylee141019
								g_nOpenErrPin[g_nOpenErrPinCount][3] = nPin + 1;   //  SWITCH ��  PIN ;//sylee141019   
								//	  nCardPinMax;//sylee150210 //ACE380
								g_nOpenErrPin[g_nOpenErrPinCount][4] = jigPin + 1;  //  BDL PIN ;//sylee141019   //son210831
							}
						}
					}


				}//sylee20120512
			}

			//son �ش� ī���� ������ ���϶� Fail Ƚ�� ���
			if (cardPin == (nCardPinMax - 1))     // 	nCardPinMax=64;//sylee150210 //ACE380
			{
				if (nFail1 > 0)
				{
					//open
					if (nCase == SELF_TEST_OPEN) //so240130 1:SELF_TEST_OPEN
						str.Format(" * Error,  LV Open, Switch Card=%d,  FailPin Count=%d", nPrintCard + 1, nFail1); //son231213  card -> nPrintCard

					//short
					else
						str.Format(" * Error,  Switch Card=%d ,  FailPin Count=%d", nPrintCard + 1, nFail1); //son231213  card -> nPrintCard

					OnList1Mes1(str);
					//		 fprintf(fp1, str);
					//		 fprintf(fp1, " \n " );
				}
			}


			int nRet1;
			nRet1 = 0;

			if (nCase == SELF_TEST_LEAK1 || nCase == SELF_TEST_LEAK2            //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
			        || nCase == SELF_TEST_LEAK_A || nCase == SELF_TEST_LEAK_B   //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
			        || nCase == SELF_TEST_VC_1PIN_RST)                          //son240130 8:SELF_TEST_VC_1PIN_RST
			{
				if (Sub71.Y1 > 500)
				{
					Sub71.OnPinAllReset1(); ///PIN ALL RESET
					Sub71.Delay_100ns(1000);  //100us 
				}
			}

		}
	}


	//-----------
	//son  LV
	//-----------
	if (m_dVSet <= 20)
	{
		if (nCase >= SELF_TEST_OPEN && nCase <=SELF_TEST_VC_1PIN_RST) { //son240130 1:SELF_TEST_OPEN, 8:SELF_TEST_VC_1PIN_RST
		    str.Format(" * [  END :: Case %d ] LV %s ", nCase, getStrSelfTestCase(nCase));  //son240201
        }
		else {
			str.Format(" * [  END :: else Case? LV %s ", getStrSelfTestCase(nCase));    //son240201
		}

	}

	//-----------
	//son  HV
	//-----------
	else
	{

		if (nCase == SELF_TEST_OPEN) {  //son240130  1:SELF_TEST_OPEN
			str.Format(" * [  END :: Case 1 ] HV OPen,  RUN ERROR ");
		}
		else if (nCase >= SELF_TEST_SHORT1 && nCase <=SELF_TEST_VC_1PIN_RST) { //son240130 2:SELF_TEST_SHORT1, 8:SELF_TEST_VC_1PIN_RST
			str.Format(" * [  END :: Case %d ] HV %s", nCase, getStrSelfTestCase(nCase));   //son240205
        }
		else {
			str.Format(" * [  END :: else Case?   HV %s ", getStrSelfTestCase(nCase));  //son240205
		}
	}


	if (rnStepErrCount == 0)
	{
		if (nDisplayDetail1 != 1)
		{
			str2.Format("  ***> PASS *");
			str = str + str2;
			OnList1Mes1(str);
		}
	}
	else
	{
		str2.Format("  ***> FAIL *");
		str = str + str2;
		OnList1Mes1(str);
	}


	return 0;
}

#if 1	// hdpark221005 �ű� ��� �������� �ڸ�Ʈó��. //hdpark221026 ���󺹱�(�ű� ��� �׽�Ʈ �� �ʿ�)
void CChildView5::OnAuto_Self1()    //Switch card   All selftest  
{

	CString   str, str1, str2, strfName, strfName1;
	FILE* fp1;
	char  fName[200], fName1[200];
	int nFlagMode1, nFail1, n4Wire, nRet1, ret;
	int dPinVC[200], dPinVB[200], nPinSt[32000];

 
	double nT[10], nData[130][15];
	int nLoop, ki, nCase, nProCase1, nStep1, nCardNum, nIso1, nRet, nCo1;
	int StepErrCount, nFailflag, nEarthFlag;
	//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380
	int nCardX;//sylee150210 //ACE380
	int nDisplayDetail2;//sylee150210
	int  nPin;//sylee151007 //ACE400

	nDisplayDetail2 = 0;//sylee150210
	nPinSetSkipVb = 0;//sylee160803-6 //son230127
	nPinSetSkipVc = 0;//sylee160803-6 //son230127

	//ACE380//////////////////////////////////////////////////////////////
	//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380
	Get_nCardPinMax();//sylee150903 //ACE400    //son231227

	//sylee151017	nCardX=2;//sylee150210 //ACE380
	nCardX = 1;//sylee151007//ACE400
	//ACE380//////////////////////////////////////////////////////////////  
	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running?     IO  start error !!!");
		return;
	}

	ret = FileSysInfo01.LoadSaveSet15(2);//load   //sylee140911
	if (ret != 1) {
		AfxMessageBox(" Error No 8212, \n\n  SysSet15.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n ");
		return;
	}
	g_nSelfRunFlag = 1;
	// OnButtonMessageClear();
	str.Format("  ***<  IO Check Start > ***  ");
	OnList1Mes1(str);
	str.Format("   ");
	OnList1Mes1(str);	//son ���� ���� ���� �� �� ���

	str.Format("Run");
	m_Label_2wIO.SetCaption(str);

	DoEvents();

	m_nCardNoStart = 1;

#if 0   //son230224_2
	//	nCardX=2;//sylee150210 //ACE380
	if (SysSet12.m_nPinTotMax == PIN_TOT_4K) {//4k 		//son-32KPIN-20190902 : enum ���� ����
		m_nCardNoEnd = 64 * nCardX;//sylee170327  64->64     
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_8K) {//8k		 
		m_nCardNoEnd = 64 * nCardX;//sylee170327  128->64
		//if( SysSet13.m_nPinBlockType==1 )  //sylee180830-1-DDTYPE-JTECH
		if (SysSet13.m_nPinBlockType == PIN_BLK_DD) { 	//son-32KPIN-20190902 : enum ���� ����
			//m_nCardNoEnd = 64 * 2;//64pin x 128ea = 8K //sylee180830-1-DDTYPE-JTECH
			m_nCardNoEnd = 128 * nCardX;  //son230224_2
		}
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_16K) {//16k		//son-32KPIN-20190902 : enum ���� ����
		m_nCardNoEnd = 128 * nCardX;
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_12K) {//12k		//son240125
		m_nCardNoEnd = 96 * nCardX;
	}
	else if (SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS //32k		//son-32KPIN-20190902 : enum ���� ���� //son230216_1
	          || SysSet12.m_nPinTotMax== PIN_TOT_32K_U8K_L24K //son230224_2
	          || SysSet12.m_nPinTotMax== PIN_TOT_32K_ORG)      //son230224_2
    {
		m_nCardNoEnd = 256 * nCardX;
	}
	else {
		m_nCardNoEnd = 128 * nCardX;
	}
#else

    //son240125 Old �ڵ�� card skip���� �ʰ� user card��ȣ�� ����ϹǷ� getCardNoEnd() �Լ� ��� �Ұ�.
    //          FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax)�� ��ü��
    //m_nCardNoEnd = FileSysInfo01.getCardNoEnd(1, SysSet12.m_nPinTotMax);    //son230224_2
    m_nCardNoEnd = FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax, SysSet13.m_nPinBlockType); //son240125

#endif


	//Define ACE400  //sylee161006  SHIN BU SUNG
	// 32k , 16k   QD PIN BLOCK 4K  X  4EA,   4K  X  8EA 
	// 8K      DD PIN BLOCK      2K  X  4EA
	// 4K      SD  PIN BLOCK     1K  X  4EA	    


	//sylee161108-1  if(SysSet13.m_nSet4!=1 && SysSet13.m_nSet5==1 ){//sylee161006- //DD PIN BLOCK
	//sylee180830-1	if(	SysSet13.m_nPinBlockType==1) {  //sylee161006- //DD PIN BLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
	//sylee180830-1		 m_nCardNoEnd = 64*nCardX;
	//sylee180830-1	 }

	::ZeroMemory(&g_nOpenErrPin, sizeof(g_nOpenErrPin));//sylee141019
	::ZeroMemory(&g_nCardS, sizeof(g_nCardS));
	::ZeroMemory(&nPinSt, sizeof(nPinSt));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&nT, sizeof(nT));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&dPinVC, sizeof(dPinVC));
	::ZeroMemory(&dPinVB, sizeof(dPinVB));
	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));
	::ZeroMemory(&dSelfErr1, sizeof(dSelfErr1));

	g_nOpenErrPinCount = 0;//
	g_nListCount = 0;
	nEarthFlag = 0;

	g_nStopFlag = 0;
	g_nErrCount1 = 0;
	g_nErrCountTot = 0;
	nStop = 0;
	n4Wire = 0;
	nFlagMode1 = 0;
	nFail1 = 0;
	g_nSelfTestInterlock = 0;//sylee131121


	g_nAutoFlag = 3;
	Sub71.nADCount1 = 1;//test

	//===================================================================
	//HWDEFINE

	//sylee140130	strfName1.Format("\\SETUP\\SelfAuto_2WTr.CSV" );         //<=======================================
	strfName1.Format("d:\\log\\SelfAuto_2WTr.CSV");         //<=======================================
	//	DeleteFile(strfName1);
	::ZeroMemory(&fName1, sizeof(fName1));

	strcat(fName1, strfName1);
	fp1 = fopen(fName1, "wt");

	if (fp1 == NULL) {
		str.Format("Error    001SelfAuto_2WTr file open   ?  Error, Saving Error !     file close!   ");
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		return;
	}

	//=======================================================================
	//		m_edit1.GetWindowText( szText,10); 
	//		m_nCardNoStart = atoi( szText );	????????????????????????

	if (m_nCardNoStart < 1) {
		str.Format("Error Self,  Begin Card < 1 . \n \n  Set Error !     Begin Card=%d,    ", m_nCardNoStart);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}


	//son Switch ī�尡 64pin���� 128 pin������ ���� ȯ�溯���� ���� 
	//    nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 �Ǵ� 128) ���� �����Ѵ�. 
	Get_nCardPinMax(); //sylee150903 //ACE400   //son231227

	if (nCardPinMax == 64) //sylee150903-1 //ACE400
	{

		if (m_nCardNoStart > 256) {
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}

	}
	else
	{
		if (m_nCardNoStart > 512) {
			str.Format("Error Self,  Begin Card > 512 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}
	}
	//	m_edit2.GetWindowText( szText,10); 
	//	m_nCardNoEnd = atoi( szText );?????????????????????????????

	if (m_nCardNoEnd < 1) {
		str.Format("Error Self,  End Card < 1 . \n \n  Set Error !    End Card=%d,    ", m_nCardNoEnd);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}



	if (m_nCardNoEnd > (256 * nCardX)) {
		//ACE380 str.Format("Error Self,  End Card > (256 ) ( 32k).  \n \nSet Error !    End Card=%d,    ",m_nCardNoEnd );
		str.Format("Error Self,  End Card > (256*2) ( 32k).  \n \nSet Error !    End Card=%d,    ", m_nCardNoEnd);//ACE380
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	if (m_nCardNoEnd < m_nCardNoStart) {
		str.Format("Error Self,  End Card < Begin Card  . \n \n  Set Error !     Begin Card=%d,  End Card=%d,   ", m_nCardNoStart, m_nCardNoEnd);
		AfxMessageBox(str, MB_OK); //error messege
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}


	nCardNum = m_nCardNoEnd - m_nCardNoStart + 1;

	m_nCardNoStart = m_nCardNoStart - 1;    //son 0 base�� ����� ���� -1
	m_nCardNoEnd = m_nCardNoEnd - 1;


	//============================================================================================

    //son240130 1:SELF_TEST_LV_OPEN,  17:(SELF_TEST_MAX-1)
	for (int nProStep1 = SELF_TEST_LV_OPEN; nProStep1 <= (SELF_TEST_MAX-1); nProStep1++)    //son240130
	{

		StepErrCount = 0;

		Sub71.OnPinAllReset1(); ///PIN ALL RESET
		if (g_nStopFlag == 2) {	//sylee131121
			nProStep1 = SELF_TEST_MAX; //son240130 18:SELF_TEST_MAX
			continue;
		}

		if (g_nStopFlag != 2) {	//sylee130916	 
			//sylee150211//ACE380 RAY		Sleep(200);	Sleep(200);	Sleep(200);//Sleep(200);	Sleep(200);
			Sleep(100);//sylee150211//ACE380
		}

		//sylee150211//ACE380 RAY	if( g_nStopFlag!=2){
		//sylee150211//ACE380 RAY		Sub71.Delay_100ns(5000000);  //400ms //sylee120906
		//sylee150211//ACE380 RAY	}
		
		//SPARE   4, 5 
		if (nProStep1 == SELF_TEST_LV_LEAK1 || nProStep1 == SELF_TEST_LV_LEAK2) {  //son240130 4:SELF_TEST_LV_LEAK1, 5:SELF_TEST_LV_LEAK2
			continue;
		}

		//SPARE  8, 9, 10, 11, 12, 13
		if (nProStep1 >= SELF_TEST_VC_1PIN_RST && nProStep1 <= 13) {  //son240130 8:SELF_TEST_VC_1PIN_RST
			continue;
		}

		if (nProStep1 == SELF_TEST_LV_OPEN || nProStep1 == SELF_TEST_HV_LEAK1)	//son240130 1:SELF_TEST_LV_OPEN,  14:SELF_TEST_HV_LEAK1
		{
			if (nProStep1 == SELF_TEST_LV_OPEN) {   //son240130 1:SELF_TEST_LV_OPEN
				g_nMode1 = MODE181_SELF_10V;  //lv 10v SelfTest  //sylee130701   //son240130 181:MODE181_SELF_10V
			}
			else if (nProStep1 == SELF_TEST_HV_LEAK1) { //son240130  14:SELF_TEST_HV_LEAK1
			 //sylee230821	g_nMode1 = MODE182_SELF_200V;  //hv  200v SelfTest  //sylee130701 //son240130 182:MODE182_SELF_200V
		        g_nMode1 = MODE327_LEAK_250V; //sylee230821   //son240130 327:MODE327_LEAK_250V
			}

			nRet = AutoSelf_PowerModeSet(g_nMode1);
			if (nRet == 0) {
				fclose(fp1);
				AfxMessageBox(" Error No 8023, \n    AutoSelf_PowerModeSet     SET ERROR !");
				//return; 
				goto task2;
			}

			//son C:\ACE400\SETUP\SysSet21_ACE400.ini ���ǰ�. dRec[362][16]�� �ε���.	
			//son 181 Self 10V ���̽��� ����Ѵ�.
																	//son 			Self 10V(lv)		Self 200V (hv)
			m_nCC = (int)dRec[g_nMode1][1];						    //son 	m_nCC		1 (CC)				1 (CC)
			m_nVRel = (int)dRec[g_nMode1][2];   					//son   m_nVRel		1 (LV1)				3 (HV1)
			m_dVSet = dRec[g_nMode1][3];							//son   m_dVSet		10 V				200 V
			m_nIR = (int)dRec[g_nMode1][4];						    //son	m_nIR		2 (I Range2)		4 (I Range4)
			m_dISet = dRec[g_nMode1][5];							//son   m_dISet		100 				10000
			m_nFil = (int)dRec[g_nMode1][8];//sylee130706			//son   m_nFilter	1 (VC Fast)			2 (VC Slow)
			m_nIFilter = (int)dRec[g_nMode1][9];   //sylee13062		//son	m_nIFilter	1 (I Fast)			2 (I Slow)
			m_nHVOffSet = (int)dRec[g_nMode1][10]; //sylee130629	//son	HVOffset	0					0
			m_nHVGain = (int)dRec[g_nMode1][11];	//sylee130629	//son   HVGain		0					0


		}


		m_nIGain = (int)dRec[g_nMode1][12];//sylee150904 //ACE400		//son   IGain		1 (I HighGain Off)		1

	    Sub71.I_Gain(m_nIGain);//sylee220924 

		//son nCase�� ���⼭ �����Ѵ�.
		if (nProStep1 <= 10) {
			nCase = nProStep1;
		}
		else {
			nCase = nProStep1 - 10;
		}


		//------------------
		//son OPEN: case 1
		if (nProStep1 == SELF_TEST_LV_OPEN) {       //son240130 1:SELF_TEST_LV_OPEN
			m_dISOSet1 = (m_dISet * 0.8);
			//test	m_dISOSet1=10;
			nIso1 = 1;//open
			nProCase1 = 1;
		}

		//-----------------------
		//son LV Short: case 2, 3
		else if (nProStep1 <= SELF_TEST_LV_SHORT2) {      //son240130 3:SELF_TEST_LV_SHORT2
			m_dISOSet1 = 2;
			//test			m_dISOSet1=0;
			nIso1 = 2;//short
			nProCase1 = 2;
		}

		//--------------------------
		//son HV LEAK: case 15, 16, 17
		else if (nProStep1 == SELF_TEST_HV_LEAK2         //son240130 15:SELF_TEST_HV_LEAK2
		        || nProStep1 == SELF_TEST_HV_LEAK_A      //son240130 16:SELF_TEST_HV_LEAK_A
		        || nProStep1 == SELF_TEST_HV_LEAK_B)     //son240130 17:SELF_TEST_HV_LEAK_B
		{
			m_dISOSet1 = 500; //500;
			nIso1 = 2;
			nProCase1 = 4;
		}

		//--------------------------
		//son LV LEAK: case 4, 5, 6, 7
		else if (nProStep1 == SELF_TEST_LV_LEAK1      //son240130 4: SELF_TEST_LV_LEAK1
		        || nProStep1 == SELF_TEST_LV_LEAK2    //son240130 5: SELF_TEST_LV_LEAK2
		        || nProStep1 == SELF_TEST_LV_LEAK_A   //son240130 6: SELF_TEST_LV_LEAK_A
		        || nProStep1 == SELF_TEST_LV_LEAK_B)  //son240130 7: SELF_TEST_LV_LEAK_B
		{
			m_dISOSet1 = 5;
			nIso1 = 2;
			nProCase1 = 2;
		}


		//---------------------
		//son HV LEAK: case 14
		else if (nProStep1 == SELF_TEST_HV_LEAK1) { //son240130 14:SELF_TEST_HV_LEAK1
			m_dISOSet1 = 40;//sylee121022  30->40   KWS
			nIso1 = 2;
			nProCase1 = 4;
		}

		//----------------------------------
		//son case 8, 9, 10, 11, 12, 13 
		else {
			m_dISOSet1 = 5;
			nIso1 = 2;
		}

		//	str.Format("%.2f",m_dISOSet1); 
		//	m_edit8.SetWindowText(str);

		DoEvents();

		//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
		if (g_nStopFlag == 2) {
			g_nSelfRunFlag = 0;
			goto task2;
		}


		//son InterI ����
		//=====================================================================
		double InterI; //sylee20120612

		if (m_nIR == 1 || m_nIR == 2) {  //mA 
			InterI = m_dISet * 0.9;
		}
		else if (m_nIR == 3 || m_nIR == 4) {   // uA 
			InterI = m_dISet * 0.9;
		}
		else if (m_nIR == 5) {//uA
			InterI = 10.0;
		}
		//=====================================================================


		if (SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) { //son-32KPIN-20190902 
			m_nCardNoEnd = 23;//sylee170922
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE) {	//son-32KPIN-20190902	//son230216_1
			m_nCardNoStart = 0;
			m_nCardNoEnd = 32 - 1;
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421) { //son-32KPIN-20190902 
			m_nCardNoStart = 0;
			m_nCardNoEnd = 64 - 1;
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) { //son-32KPIN-20190902 
			m_nCardNoStart = 0;
			m_nCardNoEnd = 128 - 1;
		}


		nCo1 = 0;

		int card;		// 	//son-32KPIN-20190902:  int j �� int card�� ����
		for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++)  //card no   	<===
		{


			nCo1++;
			//son240130 if (nCase > 3 && nCase < 8)// HW INTERLOCK		 
			if (nCase >=SELF_TEST_LEAK1 && nCase <= SELF_TEST_LEAK_B)// HW INTERLOCK	 //son240130 4:SELF_TEST_LEAK1, 7:SELF_TEST_LEAK_B 
			{
				if (nCo1 > 8)
				{
					nCo1 = 0;
					Sub71.OnPinAllReset1(); ///PIN ALL RESET
					Sub71.Delay_100ns(100);  //10us 
				}
			}

			nLoop = 5;
			nFail1 = 0;


			double dRate1, dRate2;

			if (nProCase1 == 1) {
				dRate1 = 0.3;  dRate2 = 0;
			}
			else if (nProCase1 == 2) {
				dRate1 = 0.5;  dRate2 = 0.3;
			}
			else if (nProCase1 == 3) {
				dRate1 = 0.7; dRate2 = 0.5;
			}
			else if (nProCase1 == 4) {
				dRate1 = 1; dRate2 = 0.7;
			}
			else if (nProCase1 == 5) {
				dRate1 = 1; dRate2 = 0.8;
			}

			m_nProg2wIO_Rate = (int)((((double)(card - m_nCardNoStart + 1) / (double)nCardNum * 100)) * dRate1 + dRate2);

			//================

			int cardPin;		//son-32KPIN-20190902:  int i �� int cardPin����  ����
			for (cardPin = 0; cardPin < nCardPinMax; cardPin++)  //	  nCardPinMax;//sylee150210 //ACE380
			{

				// continue;


			Task1://sylee20120308

							//son �̹� Self test Error�� �߻��� ī���� pass�Ѵ�.
				if (g_nCardS[card + 1][cardPin + 1] > 0) {
					continue;
				}

				DoEvents();

				//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
				if (g_nStopFlag == 2) {
					g_nSelfRunFlag = 0;
					goto task2;
				}

				if (nStop == 2) {
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
					goto task2;
				}

				if (nCase != SELF_TEST_LEAK1 && nCase != SELF_TEST_LEAK2            //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
				        && nCase != SELF_TEST_LEAK_A && nCase != SELF_TEST_LEAK_B)  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
                {
					Sub71.OnPinAllReset1(); ///PIN ALL RESET
				}

				ki = 0;

				//Task5:
				////////////////////////////////////////////////////////////////


				//son  Short1, Short2 �� ��� 
				if (nCase==SELF_TEST_SHORT1 || nCase==SELF_TEST_SHORT2) { //short1,2  //son240130 2:SELF_TEST_SHORT1, 3:SELF_TEST_SHORT2
					Sub71.Delay_100ns(10000); //1mS  //sylee120921
				}


				//--------------------------------
				//son VB, VC SelfTest PIN  ���� 				<====
				//--------------------------------

                nPin = GetHwPinNo_Old(card, cardPin);     //son231222_2 �Լ� ȣ��� �ڵ� ����ȭ  //son240125_2


				//-------------------------------------------------------------------------------------
				//son Self OpenTest :  ���� ����Ʈ�� VB, VC�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                     ������ �� �帥�ٸ� ����, ������ �������Ϸ� �帥�ٸ� open�̴�.
				//-------------------------------------------------------------------------------------

				//VC PIN   set				<====
				if (nCase == SELF_TEST_OPEN) {  //  VB  VC  1��1 ���� �˻� //  LV ����       //son240130 1:SELF_TEST_OPEN
					//Sub71.AOnPinSet1(1, nPin, 2);    //	  nCardPinMax;//sylee150210 //ACE380
					//Sub71.AOnPinSet1(1, nPin, 1);    //	  nCardPinMax;//sylee150210 //ACE380

					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);   //son210220 2->1 enum���� ���� 
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);   //son210220 1->2 enum���� ���� 
				}

				//-------------------------------------------------------------------------------------
				//son Self ShortTest :  ����Ʈ�� VB, �Ǵ� VC�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                      ������ ���� ���Ϸ� �帣�ų� �帣�� �ʴ´ٸ� ����.
				//                      VB�� �ٿ��µ�(�Ǵ� VC�� �ٿ��µ�)  ������ �帥�ٸ� short�̴�. 
				//-------------------------------------------------------------------------------------

				else  if (nCase == SELF_TEST_SHORT1) {  //  case 2= short1,    pin one     //son240130 2:SELF_TEST_SHORT1
					//Sub71.AOnPinSet1(1, nPin, 1);     //	  nCardPinMax;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  //son210220 1->2 enum���� ���� 
				}
				else  if (nCase == SELF_TEST_SHORT2) {  //  case 3= short2,                 //son240130 3: SELF_TEST_SHORT2
					//Sub71.AOnPinSet1(1, nPin, 2);     //	  nCardPinMax;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);  //son210220 2->1 enum���� ���� 
				}
				else  if (nCase == SELF_TEST_LEAK1) {    // case 4= leak 1,    pin multi      //son240130 4:SELF_TEST_LEAK1
					//Sub71.AOnPinSet1(1, nPin, 1);      //	  nCardPinMax;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);  //son210220 1->2 enum���� ���� 
				}
				else  if (nCase == SELF_TEST_LEAK2) {  //   case 5= leak2   pin multi        //son240130 5:SELF_TEST_LEAK2
					//Sub71.AOnPinSet1(1, nPin, 2);      //	  nCardPinMax;//sylee150210 //ACE380
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);  //son210220 2->1 enum���� ���� 
				}
				else  if (nCase == SELF_TEST_VC_1PIN_RST) {  // CASE6    pin    //son240130 8: SELF_TEST_VC_1PIN_RST


				}
				else  if (nCase == SELF_TEST_LEAK_A)  //leak A           //son240130 6:SELF_TEST_LEAK_A
				{
					nStep1 = cardPin % 2;
					if (nStep1 == 0) {
						//Sub71.AOnPinSet1(1, nPin, 1);     //	  nCardPinMax;//sylee150210 //ACE380
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);     //son210220 1->2 enum���� ���� 
					}
					else {
						//Sub71.AOnPinSet1(1, nPin, 2);      //	  nCardPinMax;//sylee150210 //ACE380
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);      //son210220 2->1 enum���� ���� 
					}

				}
				else  if (nCase == SELF_TEST_LEAK_B)//LEAK B       //son240130 7:SELF_TEST_LEAK_B
				{
					nStep1 = cardPin % 2;
					if (nStep1 == 0) {
						//Sub71.AOnPinSet1(1, nPin, 2);      //	  nCardPinMax;//sylee150210 //ACE380
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);      //son210220 2->1 enum���� ���� 
					}
					else {
						//Sub71.AOnPinSet1(1, nPin, 1);      //	  nCardPinMax;//sylee150210 //ACE380								 
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);      //son210220 1->2 enum���� ���� 
					}

				}

				//------------------------------------------------------
				//son SysSet15.m_nSet6  (SelfTest Delay Use Set.  7=USE)
//sylee230912	if (SysSet15.m_nSet6 == 7)//sylee140911  //test ray		
//sylee230912				{
					int nTime1;
					if (nCase == SELF_TEST_OPEN) {   //son240130 1:SELF_TEST_OPEN
						nTime1 = SysSet15.m_nSet2 * 10;
						if (nTime1 < 0) {
							nTime1 = 0;
						}
						Sub71.Delay_100ns(nTime1);
					}
					else  if (nCase == SELF_TEST_SHORT1 || nCase == SELF_TEST_SHORT2) {   //son240130 2:SELF_TEST_SHORT1, 3:SELF_TEST_SHORT2
						nTime1 = SysSet15.m_nSet3 * 10;
						if (nTime1 < 0) {
							nTime1 = 0;
						}
						Sub71.Delay_100ns(nTime1);
					}
					else  if (nCase == SELF_TEST_LEAK1 || nCase == SELF_TEST_LEAK2) {   //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
						nTime1 = SysSet15.m_nSet4 * 10;
						if (nTime1 < 0) {
							nTime1 = 0;
						}
						Sub71.Delay_100ns(nTime1);
					}
					else  if (nCase == SELF_TEST_LEAK_A || nCase == SELF_TEST_LEAK_B) {   //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
						nTime1 = SysSet15.m_nSet5 * 10;
						if (nTime1 < 0) {
							nTime1 = 0;
						}
						Sub71.Delay_100ns(nTime1);
					}
					else  if (nCase == SELF_TEST_VC_1PIN_RST) {     //son240130 8:SELF_TEST_VC_1PIN_RST
						Sub71.Delay_100ns(2500);
					}
					else  if (nCase == SELF_TEST_VB_1PIN_RST) {     //son240130 9:SELF_TEST_VB_1PIN_RST
						Sub71.Delay_100ns(2500);
					}
					else {
						Sub71.Delay_100ns(5000);
					}
	//sylee230912
    /*          }
				else
				{//if(SysSet15.m_nSet6==7){//sylee140911
					if (nCase == 1) {  //  VB  VC  1��1 ���� �˻� //  LV ����  
						Sub71.Delay_100ns(3500);
					}
					else  if (nCase == 2) {  //  case 2= short1,    pin one
						Sub71.Delay_100ns(1000);
					}
					else  if (nCase == 3) {  //  case3= short2,  
						Sub71.Delay_100ns(1000);
					}
					else  if (nCase == 4) {    //  case4= leak 1,    pin multi 
						Sub71.Delay_100ns(1000); //150uS   
					}
					else  if (nCase == 5) {  //    case 5= leak2   pin multi 
						Sub71.Delay_100ns(1000); //100uS  
					}
					else  if (nCase == 6) {  //leak A 
						//sylee150609  ray	Sub71.Delay_100ns(5000);//sylee150417  ray  1000->5000  
						Sub71.Delay_100ns(20000);//sylee150609  ray  500uS->2mS
					}
					else if (nCase == 7) {//LEAK B	 
						//sylee150609  ray	Sub71.Delay_100ns(5000);//sylee150417  ray  1000->5000  
						Sub71.Delay_100ns(20000);//sylee150609  ray  500uS->2mS
					}
				}//if(SysSet15.m_nSet6==7)//sylee140911 
				//////////////////////////////////////////////////////////////////////////////////////////////////////
      */ //sylee230912

				//----------------------------------------
				//son Self ShortTest ���� ����, ���� ����
				//----------------------------------------

				if (nCase != SELF_TEST_OPEN) //son240130 1:SELF_TEST_OPEN
				{

					Sub71.AOnReadV1I1(m_nVRel);         //son ����, ���� ����.

					//son Self ShortTest�ε� InterI���� ū ������ ������ �ȴٸ� ���� Short �׽�Ʈ�� �ߴ�. 
					if (Sub71.Y1 > InterI)
					{
						Sub71.OnPinAllReset1(); ///PIN ALL RESET  
						//  Sub71.Delay_100ns(10000);  //1ms  relay
						Sub71.Delay_100ns(100);  //10us 
						if (m_nVRel < 3) {
							str.Format("Error No 8211-3,\n\n   LV Short     \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
							                                                 card + 1, cardPin + 1, Sub71.Y1, InterI);
						}
						else {
							str.Format("Error No 8211-4,\n\n   HV Short      \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
							                                                card + 1, cardPin + 1, Sub71.Y1, InterI);
						}
						//AfxMessageBox(str);

						nRet1 = AfxMessageBox(str, MB_YESNO | MB_ICONEXCLAMATION);

                        //son240125_2 �޽����ڽ� No Ŭ���ÿ� List�� ��� �ȵǴ� ���� ����. 
                        //            �Լ��� ����� List �� ����� �ȵǹǷ� �Լ� ����� �ʰ� ��� �����ϵ��� ����. 
						//son240125_2 if (nRet1 != 6) {       //son OK ��ư�� �ƴ϶�� �Լ��� �����.	
						//son240125_2 	g_nStopFlag = 2;	//sylee140701
						//son240125_2 	goto task2;
						//son240125_2 }
						goto TaskInterR101;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////////// 					


				//-----------------------------------------------
				//son  Self OpenTest, ShortTest ����, ���� ����
				//-----------------------------------------------

				Sub71.Delay_100ns(100);  //10uS 

				if (n4Wire == 4) { //sylee20111118
					Sub71.AOnReadV1I1(5); // READ  4W V  , I 
				}
				else {
					Sub71.AOnReadV1I1(m_nVRel);//sylee20111115 
				}
				//=============================================




			TaskInterR101:

				//--------------------
				//son  R �� ���
				//--------------------

				if (Sub71.Y1 < 0.0000001) {
					Sub71.Y1 = 0.0000001;
				}


	 
				if (m_nIR == 1 || m_nIR == 2 || m_nIR == 3  ) {	//sylee150909 //ace400
					Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000.0;
				}
				else if (m_nIR == 5) {	//sylee150909 //ace400	
					Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000000.0;
				}

				if (m_nIR == 4) {	//sylee150909 //ace400	
					Sub71.Y1 = Sub71.Y1 * 1000.0;
				}



                //son Leak�� �ƴϸ�
				if (nCase != SELF_TEST_LEAK1 && nCase != SELF_TEST_LEAK2           //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
				        && nCase != SELF_TEST_LEAK_A && nCase != SELF_TEST_LEAK_B) //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
				{
					nRet1 = Sub71.OnPinAllReset1(); ///PIN ALL RESET  
					if (nRet1 != 1) {
						AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
						g_nSelfRunFlag = 0;
						fclose(fp1);
						return;
					}
				}

				//son ù��° card ó�� ����ÿ�  ������ 5ȸ �ݺ�.
				if (card == m_nCardNoStart) {
					if (nLoop > 0) {  //sylee20120308
						if (nLoop == 1) {
							cardPin = 0;
						}
						nLoop--;
						goto Task1;
					}
				}


				//---------------------------------
				//son Self ShortTest Error ����
				//---------------------------------
				if (nIso1 == 2)
				{
					//	if( Sub71.Y1>nIValue1){//fail log

					//-----------------------------------------------
					//son ���ذ����� ������ ���� �帥�ٸ� Short�̴�.
					if (Sub71.Y1 > m_dISOSet1)//fail log  0-> m_dISOSet1
					{

						g_nCardS[card + 1][cardPin + 1] = nProStep1;

						//son ErrCount ������ �Ѿ�ٸ� ���� ī��� �Ѿ� ����.
						if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1)) { ////sylee160514-2
							g_nErrCount1++;
							g_nErrCountTot++;
							StepErrCount++;
						}
						else {
							break;	//son ���� ī��� �Ѿ��.
						}

						dSelfErr1[g_nErrCount1][0] = (float)(card + 1);  //  CARD no.

						if (nCase == SELF_TEST_OPEN                                         //son240130 1:SELF_TEST_OPEN
						        || nCase == SELF_TEST_SHORT2 || nCase == SELF_TEST_LEAK2    //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2
						        || nCase == SELF_TEST_LEAK_A || nCase == SELF_TEST_LEAK_B)  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
						{
							dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1);  //vc  cardPin
						}
						else {
							dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1);  //vc  cardPin  //sylee131123
						}

						//vc  pin
						if (nCase == SELF_TEST_OPEN                                         //son240130 1:SELF_TEST_OPEN
						        || nCase == SELF_TEST_SHORT2 || nCase == SELF_TEST_LEAK2    //son240130 3:SELF_TEST_SHORT2, 5:SELF_TEST_LEAK2
						        || nCase == SELF_TEST_LEAK_A || nCase == SELF_TEST_LEAK_B)  //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
						{
							dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);  //vc pin total	   //	  nCardPinMax;//sylee150210 //ACE380								 

						}
						else if (nCase == SELF_TEST_SHORT1 || nCase == SELF_TEST_LEAK1) {    //son240130 2:SELF_TEST_SHORT1, 4:SELF_TEST_LEAK1
							dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);//sylee131123  //	  nCardPinMax;//sylee150210 //ACE380
						}
						else {
							dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);   //	  nCardPinMax;//sylee150210 //ACE380
						}

						dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   //i
						dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   //R1;  //double dR1[2][5000][3];
						dSelfErr1[g_nErrCount1][7] = (float)nProStep1;//sylee131212

						nFail1++;
					}

				}

				//-----------------------------
				//son Self OpenTest Error ����
				//-----------------------------
				else //son nIso1==1  �� ��� 
				{

					nFailflag = 0;

					//son nIso1==1 �̹Ƿ� nCase�� 1�� �ƴ� �� ����. ������ġ �ε�? 
					if (nCase == SELF_TEST_OPEN)     //son240130 1:SELF_TEST_OPEN
					{
						//--------------------------------------------------------------------------
						//son ���ذ����� ������ ���� �帣�ų� VC ���� ���� ������ �ȴٸ� open�̴�.
						//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۰ų� VC��(Sub71.X1)�� 8���� �۴ٸ�
						if ((Sub71.Y1 < m_dISOSet1) || (Sub71.X1 < 8)) {
							nFailflag = 1;
						}

						//----------------------------------------
						//son  ���������� ������ ������ ����Ѵ�.

						//son 10V �����ߴµ� 8V ���Ϸ� ������ ���  (Open Self Test ���� �Ұ�)
						if (Sub71.X1 < 8)
						{
							if (nEarthFlag == 0)
							{
								nEarthFlag = 1;
								//AfxMessageBox(" Error No 8027, \n\n   if all point is  (same voltage<8),   Earth!?,  All  Cable, pin block, jig,  etc. Check!?  \n\n   \n\n\n    , Self Test   Error!!!"); 
								//if( nG_Language!=2){//sylee150707
								//    str.Format(" *> [���� �߻�: �������� �Ҽ� ����]:   ");												 
								//    OnList1Mes1(str); 
								//    str.Format(" *> [��ġ���]:   ");												 
								//    OnList1Mes1(str); 
								//    str.Format(" *> 1) ����Ż���Ͽ� Ȯ��! ������������");	
								//    OnList1Mes1(str); 
								//    str.Format(" *> 2) ����ġī�带 ��ȯ�Ͽ� Ȯ���Ͻʽÿ�! ");
								//    OnList1Mes1(str); 
								//    str.Format(" *> 3) ����ġī�忡 �������̺��� �и�Ȯ��! ������������");
								//    OnList1Mes1(str); 	
								//}else{

								str.Format(" *> [Error : Cannot process Self OpenTest]:   ");
								OnList1Mes1(str);
								str.Format(" *> [Check point]:   ");
								OnList1Mes1(str);
								str.Format(" *>   Case1) Ground Error. Remount the jig and test again.");
								OnList1Mes1(str);
								str.Format(" *>   Case2) Change the SwitchCard and test again.");
								OnList1Mes1(str);
								str.Format(" *>   Case3) Check if the Flat Cable is disconnected.");
								OnList1Mes1(str);

							}
						}

					}

					//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۴ٸ�
					else {
						if (Sub71.Y1 < m_dISOSet1) {
							nFailflag = 1;
						}
					}

					//------------------------------
					//son Open SelfTest Error ���� 
					//------------------------------
					if (nFailflag == 1) //sylee20120512							 
					{
						//if( Sub71.Y1< m_dISOSet1){ //sylee20120512 
						g_nCardS[card + 1][cardPin + 1] = 1;

						//sylee160514-2 	if(g_nErrCount1<17001){

						//son ErrCount ������ �Ѿ�ٸ� ī���� ������ ������ ���� ī��� �Ѿ� ����.
						if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1))//sylee160514-2 
						{
							g_nErrCount1++;
							g_nErrCountTot++;
							StepErrCount++;
						}
						else {
							break;		//son ���� ī��� �Ѿ��.
						}

						dSelfErr1[g_nErrCount1][0] = (float)(card + 1);  //  CARD no.
						dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1);  //vb  pin
						dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);  //sylee151007 //ACE400//vb  pin  //	  nCardPinMax;//sylee150210 //ACE380
						dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   //i
						dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   //R1;  //double dR1[2][5000][3];
						dSelfErr1[g_nErrCount1][7] = (float)nProStep1;//sylee131212

						nFail1++;

						if (nCase == SELF_TEST_OPEN) {//sylee141019  //son240130 1:SELF_TEST_OPEN
							if (g_nOpenErrPinCount < MAX_SELF_OPEN_ERR_PIN)
							{
								if ((Sub71.X1 < 2) && (Sub71.Y1 < 2)) {//sylee141019
									g_nOpenErrPinCount++;//sylee141019
									g_nOpenErrPin[g_nOpenErrPinCount][1] = (card + 1);  //SWITCH CARD No.
									g_nOpenErrPin[g_nOpenErrPinCount][2] = (cardPin + 1);  //SWITCH CARD Pin No.
									g_nOpenErrPin[g_nOpenErrPinCount][3] = nPin + 1;  //  SWITCH ��  PIN 

									//son231222_4 g_nOpenErrPin[g_nOpenErrPinCount][4] = nPinSW_Net(nPin + 1);  //  BDL PIN   
									g_nOpenErrPin[g_nOpenErrPinCount][4] = nJigPinMap[nPin] + 1; //son231222_4   nPin�� hwPin0
								}
							}
						}


					}//sylee20120512


				}



				//if(SysSet13.m_nPinBlockType==7){//sylee180316-1
				if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) { //son-32KPIN-20190902 
					if (card < 64) { //64=128x64=8k
						nCardPinMax = 128;
					}
					else {
						nCardPinMax = 64;
					}
				}


				//son ������ ī���϶� Fail Ƚ�� ���
				if (cardPin == (nCardPinMax - 1))     // 	nCardPinMax=64;//sylee150210 //ACE380
				{
					if (nFail1 > 0)
					{
						// fprintf(fp1, " \n  " );
						if (nCase == SELF_TEST_OPEN) {   //son240130 1:SELF_TEST_OPEN
							str.Format(" * Error,  LV Open, Switch Card=%d,  FailPin Count=%d", card + 1, nFail1);
						}
						else {
							str.Format(" * Error,  Switch Card=%d ,  FailPin Count=%d", card + 1, nFail1);
						}
						OnList1Mes1(str);
						//		 fprintf(fp1, str);
						//		 fprintf(fp1, " \n " );
					}
				}



				int nRet1;
				nRet1 = 0;

				if (nCase == SELF_TEST_LEAK1 || nCase == SELF_TEST_LEAK2            //son240130 4:SELF_TEST_LEAK1, 5:SELF_TEST_LEAK2
				        || nCase == SELF_TEST_LEAK_A || nCase == SELF_TEST_LEAK_B   //son240130 6:SELF_TEST_LEAK_A, 7:SELF_TEST_LEAK_B
				        || nCase == SELF_TEST_VC_1PIN_RST)                          //son240130 8:SELF_TEST_VC_1PIN_RST
				{
					if (Sub71.Y1 > 500) {
						Sub71.OnPinAllReset1(); ///PIN ALL RESET
						Sub71.Delay_100ns(1000);  //100us 
					}
				}
			}////for END   for( cardPin=0; cardPin<64; cardPin++)  //pin  vb pin
		} //for END  for( card=0; card< nCount; card++)  //count



		//-----------
		//son  LV
		//-----------
		if (m_dVSet <= 20) {
		    str.Format(" * [  END :: Case %d ] LV %s ", nCase, getStrSelfTestCase(nCase)); //son240201
		}

		//-----------
		//son  HV
		//-----------
		else {
		    str.Format(" * [  END :: Case %d ] HV %s ", nCase, getStrSelfTestCase(nCase)); //son240201
		}


		if (StepErrCount == 0) {
			if (nDisplayDetail1 != 1) {
				str2.Format("  ***> PASS *");
				str = str + str2;
				OnList1Mes1(str);
			}
		}
		else {
			str2.Format("  ***> FAIL *");
			str = str + str2;
			OnList1Mes1(str);
		}






	task2:  //STOP JUMP POSTION


		Sub71.OnPinAllReset1();

		//sylee130814  Sub71.OnDisChargeuS(200);// sylee20120621  1->10	
		//sylee150211//ACE380 RAY 	if( g_nStopFlag!=2 ){	//sylee130916	 
		//sylee150211//ACE380 RAY Sub71.Delay_100ns(5000000);  //500mS						 
		//sylee150211//ACE380 RAY 	} 

		if (g_nStopFlag == 2) {
			//sylee131218
			g_nSelfRunFlag = 0;
			//	goto task3;
			break;
		}

	}//   for  prostep



	//sylee131220 task3:  //STOP JUMP POSTION

	/////////////////////////////////////////////////////////////////////////
	//log


	int nProStep2Tmp;
	int nErrorType[30];

	::ZeroMemory(&nErrorType, sizeof(nErrorType));

    //son240130 1:SELF_TEST_LV_OPEN,  17:(SELF_TEST_MAX-1)
	for (nProStep2Tmp = SELF_TEST_LV_OPEN; nProStep2Tmp <= (SELF_TEST_MAX-1); nProStep2Tmp++)   //son240130
	{

		if (nProStep2Tmp <= 10) {
			nCase = nProStep2Tmp;
		}
		else {
			nCase = nProStep2Tmp - 10;
		}

		if (nProStep2Tmp == SELF_TEST_LV_OPEN) {        //son240130 1:SELF_TEST_LV_OPEN
			str.Format("--------------------------------------- ");
			OnList1Mes1(str);
		}


		int dd, ePin1, ePin2, ntmp1;

		int err;		// 	//son-32KPIN-20190902:  int i �� int err�� ����
		for (err = 1; err <= g_nErrCount1; err++) //sylee20111215
		{

			dd = err % 2;
			ntmp1 = (int)dSelfErr1[err][7];	// hdpark220819 g_nErrCount1 -> err

			if (ntmp1 != nProStep2Tmp) {//sylee131212
				continue;
			}

			nErrorType[nCase]++;

			// err=g_nErrCount1;//sylee20120703
			if (nErrorType[nCase] == 1) {   //son ù��° error�� �� �� �� ����
				fprintf(fp1, "  \n\n ");
			}

            //son ù��° Short Error�� ������  ��� ���
			if (nErrorType[nCase] == 1 && nCase != SELF_TEST_OPEN) { //son240130 1:SELF_TEST_OPEN
				fprintf(fp1, "  ,  No , NetVbPin, NetVcPin, *CardNo, SWVbPin, SWVcPin,  SW VbPinTotal, SW VcPinTotal,   VB,      VC,     I,            R,   \n "); //son240201
			}


			//ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);   //son231222_4
			ePin1 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4

			nProStep2Tmp = ntmp1;//sylee141106

			//-------------------------
			//son List â�� ����ϱ� 
			
#if 1   //son240201 begin:
            //son Short�϶����� testType�� ���� �� �� �÷��� ���.
			if (nProStep2Tmp != SELF_TEST_LV_OPEN)      //son240130 1:SELF_TEST_LV_OPEN
				fprintf(fp1, "%s ,", getStrSelfTestCase2(nProStep2Tmp));

			//son Detail Display �ɼ� 2��  �� ���� ���� ȭ�鿡 ���� ���. 
            if (nDisplayDetail2 == 1) {
                str.Format("No=%d,  %s , Net Pin=%d,    (Switch: Card=%3.0f,  Pin=%3.0f)", 
                            err, getStrSelfTestCase2(nProStep2Tmp), ePin1, dSelfErr1[err][0], dSelfErr1[err][1]);
                OnList1Mes1(str);
            }
#endif //son240201 end.

			//-----------------------
			//son Open Data ���.

            ePin1 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4

			if (nCase == SELF_TEST_OPEN) {   //son240130 1:SELF_TEST_OPEN
                //ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);   //son231222_4
				//ePin2 = nPinSW_Net((int)dSelfErr1[err][2]);   //son231222_4
                ePin2 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4

				//  fprintf(fp1, "         ,    No  , Net  Pin,   * CardNo, SW Pin, SW VcPin,  SW VbPinTotal, SW VcPinTotal,     VB,        VC,       I,            R,   \n ");	
				//fprintf(fp1, "  No=%d, Net Pin=%d, * CardNo=%d,SW Pin=*%3.0f,      %5.0f,     %5.0f,         %5.0f,         %5.0f,",
				//					err,ePin1,ePin2,dSelfErr1[err][0],dSelfErr1[err][1],dSelfErr1[err][1],dSelfErr1[err][2] ,dSelfErr1[err][2]);
				fprintf(fp1, " No=%d,  LV OPEN ,   Net Pin=%d,  (SwitchCard No=%3.0f,  SwitchCard Pin=%3.0f ), Voltage=%3.1f(<=8V)  ",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][4]);

			}

			//-----------------------
			//son Short Data ���.
			else  if (nCase == SELF_TEST_SHORT2)   //son240130 3:SELF_TEST_SHORT2
			{
                //son231222_4 ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);   

				fprintf(fp1, "    %d,        *,     %d,    %5.0f,       *,    %5.0f,         *, %5.0f, ",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);

			}
			else  if (nCase == SELF_TEST_SHORT1       //son240130 2:SELF_TEST_SHORT1
			        || nCase == SELF_TEST_LEAK1)      //son240130 4:SELF_TEST_LEAK1
			{
                //son231222_4 ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);
				if (nProStep2Tmp == SELF_TEST_HV_LEAK1) {       //son240130 14:SELF_TEST_HV_LEAK1
	     				fprintf(fp1, "  %d, %d,   *, *%5.0f, %5.0f,  *, %5.0f, *,",
						err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
				}
				else {
		 				fprintf(fp1, "   %d, %d,   *,      *%5.0f,        %5.0f,       *,    %5.0f,      *,",
						err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
				}

			}
			else  if (nCase == SELF_TEST_LEAK2) { //son240130 5:SELF_TEST_LEAK2
                //son231222_4 ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);

				fprintf(fp1, "    %d, *,  %d,    *%5.0f,           *,      %5.0f,        *,     %5.0f, ",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
			}
			else  if (nCase == SELF_TEST_LEAK_A) { //son240130 6:SELF_TEST_LEAK_A
				//son231222_4 ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);
				fprintf(fp1, "     %d,     %d,        *,     *%5.0f,   %5.0f,       *,         %5.0f,             *, ",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
			}
			else  if (nCase == SELF_TEST_LEAK_B) {     //son240130 7:SELF_TEST_LEAK_B
				//son231222_4 ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);
				fprintf(fp1, "       %d,    *, %d,    *%5.0f,           *,      %5.0f,       *,     %5.0f, ",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
			}
			else  if (nCase == SELF_TEST_VC_1PIN_RST) {     //son240130 8:SELF_TEST_VC_1PIN_RST
			}

			if (nCase != SELF_TEST_OPEN) {   //son240130 1:SELF_TEST_OPEN
				fprintf(fp1, "  %5.1f,  %5.1f,  %5.1f,  %12.1f, ", dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5], dSelfErr1[err][6]);
			}
			fprintf(fp1, " \n");


		} //for( err=1; err<=g_nErrCount1; err++)//sylee20111215

	}//

	//sylee131122 g_nErrCount1=0;//sylee20120721
	//===================================================================================


	Sub71.OnPinAllReset1(); ///PIN ALL RESET

	Sub71.AOnPoweroff2();//sylee20120702  Sub71.AOnPoweroff();

	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704

	if (g_nStopFlag == 2)
	{

		//sylee131121	 g_nStopFlag=0;
		g_nSelfRunFlag = 0;
		g_nSelfAutoFlag1 = 0;

		str.Format("  * Stop   [Self Test  ::  Switch Card  TR ]  ");
		OnList1Mes1(str);
		AfxMessageBox(str);
	}


	if (g_nErrCountTot < 1) {
		fprintf(fp1, "    ***NO ERROR *** ");
	}
	else {
		fprintf(fp1, "    ***  TOTAL ERROR  = %d  *** ", g_nErrCountTot);
	}


	///==========================================================================
	//ending
	fprintf(fp1, "  \n  \n   ************************************************* ");
	fclose(fp1);

	if ((g_nStopFlag != 2) && (g_nErrCount1 < 1)) {//sylee131121
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;//sylee131121   
		//   AfxMessageBox("    Self Test.   OK!! ", MB_OK);
	}

	if (g_nErrCount1 > 0)//sylee131121  
	{
		//sylee140130 ::ShellExecute(NULL,"open","EXCEl.EXE",fName1,"NULL",SW_SHOWNORMAL);
		if (nDisplayDetail1 != 1) {
			if (SysSet13.m_nSet7 == 1) {//sylee130724
				::ShellExecute(NULL, "open", "EXCEl.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
			else {
				::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
		}
	}

	//}


	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else
	{
		if (g_nStopFlag == 2) {
			str.Format("Stop");
		}
		else {
			str.Format("OK");
			m_nProg2wIO_Rate = 100;//sylee170125
		}
	}

	m_Label_2wIO.SetCaption(str);



	if (g_nStopFlag == 2) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;//sylee131212 import
		if (m_SelfAutoFlag1 != 2) {//sylee141029
			g_nStopFlag = 0;//sylee131121	 
		}
	}



	//==========================================
	//ENDING



	g_nAutoFlag = 0;
	g_nSelfRunFlag = 0;

	///////////////////////////////////////////////////
	if (g_nOpenErrPinCount > 0)
	{
		strfName1.Format("d:\\log\\SelfError_EarthPin.CSV");         //<=======================================
		DeleteFile(strfName1);
		::ZeroMemory(&fName1, sizeof(fName1));
		strcat(fName1, strfName1);
		fp1 = fopen(fName1, "wt");
		if (fp1 == NULL) {
			str.Format("Error    d:\\log\\SelfError_EarthPin.CSV      file open   ?  Error, Saving Error !     file close!   ");
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}

		CTime curTime = CTime::GetCurrentTime();
		fprintf(fp1, " \n\n");

 

		str.Format(" *  Error time :  %d/%d/%d - %d:%d:%d   ",
			curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
			curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
		fprintf(fp1, str);	 fprintf(fp1, "  \n\n");
		str.Format("   **> [Error : Cannot process Self OpenTest]:   ");
		fprintf(fp1, str);
		fprintf(fp1, " \n\n");
		str.Format("   **> [Check Points]:   ");
		fprintf(fp1, str);
		fprintf(fp1, " \n");

		str.Format("       Case1) Ground Error. Remount the jig and test again.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case2) Make sure BBT Controller is powered on.  Check LED lamp of board. ");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case3) Change the Switch Card and test again.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case4) Check if the flat Cable is disconnected.\n");
		fprintf(fp1, str);
		fprintf(fp1, " \n");


	 


		fprintf(fp1, " \n\n\n");

		for (int err = 1; err <= g_nOpenErrPinCount; err++)
		{
			str.Format("  **> Error:  jigPino=%d, (switch card=%d, cardPin=%d, pin=%d)  ",
				g_nOpenErrPin[err][4], g_nOpenErrPin[err][1], g_nOpenErrPin[err][2], g_nOpenErrPin[err][3]);
			fprintf(fp1, str);
			fprintf(fp1, " \n");  
		}

		if (g_nOpenErrPinCount == MAX_SELF_OPEN_ERR_PIN)
		{
			fprintf(fp1, " \n\n");
			str.Format("  **> Open error pinCount range Over!  (< 200 )");
			fprintf(fp1, str);
			fprintf(fp1, " \n");
		}

		fclose(fp1);
		if (SysSet13.m_nSet7 == 1) {//sylee130724
			::ShellExecute(NULL, "open", "EXCEl.EXE", fName1, "NULL", SW_SHOWNORMAL);
		}
		else {
			::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
		}
	}


	// str.Format("  ******<  Auto Self Test End > ******   ");										 
	str.Format("  ***<  IO Check Finish > ***   "); //son240201

	OnList1Mes1(str);


	if (nStop == 2) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;//sylee131212
		AfxMessageBox("   :  Switch Card 2W Pin Check.   Stop!!!");
	}



	Sub71.OnPinResetInit();//sylee151221   //ACE400

	Sub71.AOnPoweroff2();
	return;
}

#else		// hdpark221005, selftest �ű� ������, cal �� �����Ͽ� pass/fail �Ǻ�
void CChildView5::OnAuto_Self1()    //Switch card   All selftest  
{
	CString   str, str1, str2, strfName, strfName1;
	FILE* fp1;
	char  fName[200], fName1[200];
	int nFail1, n4Wire, nRet1, ret;

	double nT[10], nData[130][15];
	int nLoop, nCardNum, nIso1, nRet, nCo1;
	int StepErrCount, nFailflag, nEarthFlag;
	int nCardX = 1;
	int nDisplayDetail2;
	int  nPin;

	nDisplayDetail2 = 0;

	Get_nCardPinMax();  //son231227

	if (g_nSelfRunFlag > 0) {
		AfxMessageBox("   Running?     IO  start error !!!");
		return;
	}

	ret = FileSysInfo01.LoadSaveSet15(2);//load 
	if (ret != 1) {
		AfxMessageBox(" Error No 8212, \n\n  SysSet15.ini  Load ERROR !  \n    file Read Error !!!\n\n  Process Parametor set error!.  \n\n ");
		return;
	}
	g_nSelfRunFlag = 1;

	str.Format("  ***<  IO Check Start > ***  ");
	OnList1Mes1(str);
	str.Format("   ");
	OnList1Mes1(str);	//son ���� ���� ���� �� �� ���

	str.Format("Run");
	m_Label_2wIO.SetCaption(str);

	DoEvents();

	m_nCardNoStart = 1;

    //son240125 Old �ڵ�� card skip���� �ʰ� user card��ȣ�� ����ϹǷ� getCardNoEnd() �Լ� ��� �Ұ�.
    //          FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax)�� ��ü��
    //m_nCardNoEnd = FileSysInfo01.getCardNoEnd(1, SysSet12.m_nPinTotMax);    //son230224_2
    m_nCardNoEnd = FileSysInfo01.getCardNoEnd_NoSkip(1, SysSet12.m_nPinTotMax, SysSet13.m_nPinBlockType); //son240125


	//Define ACE400  //sylee161006  SHIN BU SUNG
	// 32k , 16k   QD PIN BLOCK 4K  X  4EA,   4K  X  8EA 
	// 8K      DD PIN BLOCK      2K  X  4EA
	// 4K      SD  PIN BLOCK     1K  X  4EA	    

	::ZeroMemory(&g_nOpenErrPin, sizeof(g_nOpenErrPin));
	::ZeroMemory(&g_nCardS, sizeof(g_nCardS));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&nT, sizeof(nT));
	::ZeroMemory(&nData, sizeof(nData));
	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName1, sizeof(fName1));
	::ZeroMemory(&dSelfErr1, sizeof(dSelfErr1));

	g_nOpenErrPinCount = 0;//
	g_nListCount = 0;
	nEarthFlag = 0;

	g_nStopFlag = 0;
	g_nErrCount1 = 0;
	g_nErrCountTot = 0;
	nStop = 0;
	n4Wire = 0;
	nFail1 = 0;
	g_nSelfTestInterlock = 0;

	g_nAutoFlag = 3;

	strfName1.Format("d:\\log\\SelfAuto_2WTr.CSV"); 
	::ZeroMemory(&fName1, sizeof(fName1));

	strcat(fName1, strfName1);
	fp1 = fopen(fName1, "wt");

	if (fp1 == NULL) {
		//son221020 str.Format("Error    001SelfAuto_2WTr file open   ?  Error, Saving Error !     file close!   ");
		str.Format("Error  %s file open   ?  Error, Saving Error !     file close!   ", fName1);    //son221020
		AfxMessageBox(str, MB_OK); 
		g_nSelfRunFlag = 0;
		return;
	}

	if (m_nCardNoStart < 1) {
		str.Format("Error Self,  Begin Card < 1 . \n \n  Set Error !     Begin Card=%d,    ", m_nCardNoStart);
		AfxMessageBox(str, MB_OK);
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	//son Switch ī�尡 64pin���� 128 pin������ ���� ȯ�溯���� ���� 
	//    nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 �Ǵ� 128) ���� �����Ѵ�. 
	Get_nCardPinMax();  //son231227

	if (nCardPinMax == 64) 
	{
		if (m_nCardNoStart > 256) {
			str.Format("Error Self,  Begin Card > 256 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); 
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}
	}
	else
	{
		if (m_nCardNoStart > 512) {
			str.Format("Error Self,  Begin Card > 512 ( 32k)  .  \n \nSet Error !     Begin Card=%d,    ", m_nCardNoStart);
			AfxMessageBox(str, MB_OK); 
			g_nSelfRunFlag = 0;
			g_nAutoFlag = 0;
			return;
		}
	}

	if (m_nCardNoEnd < 1) {
		str.Format("Error Self,  End Card < 1 . \n \n  Set Error !    End Card=%d,    ", m_nCardNoEnd);
		AfxMessageBox(str, MB_OK);
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	if (m_nCardNoEnd > (256 * nCardX)) {
		str.Format("Error Self,  End Card > (256*2) ( 32k).  \n \nSet Error !    End Card=%d,    ", m_nCardNoEnd);
		AfxMessageBox(str, MB_OK);
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	if (m_nCardNoEnd < m_nCardNoStart) {
		str.Format("Error Self,  End Card < Begin Card  . \n \n  Set Error !     Begin Card=%d,  End Card=%d,   ", m_nCardNoStart, m_nCardNoEnd);
		AfxMessageBox(str, MB_OK);
		g_nSelfRunFlag = 0;
		g_nAutoFlag = 0;
		return;
	}

	nCardNum = m_nCardNoEnd - m_nCardNoStart + 1;

	m_nCardNoStart = m_nCardNoStart - 1;
	m_nCardNoEnd = m_nCardNoEnd - 1;

	int	nProStepCnt = 0;
#if 0   //son240205 getStrSelfTestCase2(nProStep1)) ������� ��ü
	CString strItems[18] = { "", "LV OPEN ,", "LV SHORT1 ,", "LV SHORT2 ,", "LV LEAK1  ,", "LV LEAK2  ,", "LV LEAKA  ,", "LV LEAKB  ,",
		"", "", "", "", "", "",
		"HV LEAK1  ,", "HV LEAK2  ,", "HV LEAKA  ,", "HV LEAKB  ,"
	};
#endif

    //son240130 1:SELF_TEST_LV_OPEN,  17:(SELF_TEST_MAX-1)
	for (int nProStep1 = SELF_TEST_LV_OPEN; nProStep1 <= (SELF_TEST_MAX-1); nProStep1++)    //son240130
	{
		// 1 : Open, 2 : LV Short1, 3 : LV Short2, 14 : HV Leak1(TR), 15 : HV Leak2(TR), 16 : HV LeakA(NET), 17 : HV LeakB(NET)
		if (nProStep1 != SELF_TEST_LV_OPEN                              //son240130 1:SELF_TEST_LV_OPEN
		        && nProStep1 != SELF_TEST_LV_SHORT1 && nProStep1 != SELF_TEST_LV_SHORT23  //son240130 2:SELF_TEST_LV_SHORT1, 3:SELF_TEST_LV_SHORT2
		        && nProStep1 != SELF_TEST_HV_LEAK1 && nProStep1 != SELF_TEST_HV_LEAK2     //son240130 14:SELF_TEST_HV_LEAK1, 15:SELF_TEST_HV_LEAK2
		        && nProStep1 != SELF_TEST_HV_LEAK_A && nProStep1 != SELF_TEST_HV_LEAK_B)  //son240130 16:SELF_TEST_HV_LEAK_A 17:SELF_TEST_HV_LEAK_B
		{
			continue;
		}

		nProStepCnt++;
		StepErrCount = 0;

		Sub71.OnPinAllReset1(); 

		int nCalRIndex = 0;
		int nJudgeMode = 0;

		// mode 181, 182�� cal�� ���ϹǷ� cal data�� ������ ��忡�� ������. 
		if (nProStep1 == SELF_TEST_LV_OPEN) {	// LV Open      //son240130 1:SELF_TEST_LV_OPEN
			g_nMode1 = MODE181_SELF_10V;     //son240130  181:MODE181_SELF_10V
			nJudgeMode = MODE6_OPEN_10V;     //son240130 6:MODE6_OPEN_10V
			nCalRIndex = dRec[g_nMode1][14];
			FileSysInfo01.LoadSaveSer13(_LOAD, nJudgeMode, PART_UPPER);
			m_dISOSet1 = gdCal2[PART_UPPER][0][CAL_REF_RAWR][nCalRIndex]; // 15 = 10K
			nIso1 = 1;
		}
		else if (nProStep1 <= SELF_TEST_LV_SHORT2) {	// LV Short     //son240130 3:SELF_TEST_LV_SHORT2
			g_nMode1 = MODE181_SELF_10V;         //son240130 181:MODE181_SELF_10V
			nJudgeMode = MODE206_HR_SHORT_10V;   //son240130 206:MODE206_HR_SHORT_10V
			nCalRIndex = dRec[g_nMode1][15];
			FileSysInfo01.LoadSaveSer13(_LOAD, nJudgeMode, PART_UPPER);
			m_dISOSet1 = gdCal2[PART_UPPER][1][CAL_REF_RAWR][nCalRIndex]; // 19 = 100K
			nIso1 = 2;
		}

		//HV Leak
		else if (nProStep1==SELF_TEST_HV_LEAK1 || nProStep1==SELF_TEST_HV_LEAK2         //son240130 14:SELF_TEST_HV_LEAK1, 15:SELF_TEST_HV_LEAK2
		        || nProStep1==SELF_TEST_HV_LEAK_A || nProStep1==SELF_TEST_HV_LEAK_B) 	    //son240130 16:SELF_TEST_HV_LEAK_A, 17:SELF_TEST_HV_LEAK_B
        {
		 //sylee230821	g_nMode1 = 182;
            g_nMode1 = MODE327_LEAK_250V; //sylee230821     //son240130 327:MODE327_LEAK_250V
			nJudgeMode = MODE327_LEAK_250V;                 //son240130 327:MODE327_LEAK_250V
			nCalRIndex = dRec[g_nMode1][15];
			FileSysInfo01.LoadSaveSer13(_LOAD, nJudgeMode, PART_UPPER);
			m_dISOSet1 = gdCal2[PART_UPPER][2][CAL_REF_RAWR][nCalRIndex]; // 32 = 50M
			nIso1 = 2;
		}

		nRet = AutoSelf_PowerModeSet(g_nMode1);
		if (nRet == 0) {
			fclose(fp1);
			AfxMessageBox(" Error No 8023, \n    AutoSelf_PowerModeSet     SET ERROR !");
			goto task2;
		}

		m_nIGain = (int)dRec[g_nMode1][12];

		if (m_nIGain == 2) {
			Sub71.I_Gain(2);  //10x  =io on		
		}
		else {
			Sub71.I_Gain(1);  //1x  =io off
		}

		DoEvents();

		//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
		if (g_nStopFlag == 2) {
			g_nSelfRunFlag = 0;
			goto task2;
		}

		double InterI; 

		if (m_nIR == 1 || m_nIR == 2) {  //mA 
			InterI = m_dISet * 0.9;
		}
		else if (m_nIR == 3 || m_nIR == 4) {   // uA 
			InterI = m_dISet * 0.9;
		}
		else if (m_nIR == 5) {//uA
			InterI = 10.0;
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) { //son-32KPIN-20190902 
			m_nCardNoEnd = 23;
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE) {	//son-32KPIN-20190902 //son230216_1
			m_nCardNoStart = 0;
			m_nCardNoEnd = 32 - 1;
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421) { //son-32KPIN-20190902 
			m_nCardNoStart = 0;
			m_nCardNoEnd = 64 - 1;
		}

		if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) { //son-32KPIN-20190902 
			m_nCardNoStart = 0;
			m_nCardNoEnd = 128 - 1;
		}

		nCo1 = 0;

		int card;
		for (card = m_nCardNoStart; card <= m_nCardNoEnd; card++) 
		{
			nCo1++;
			//son240130 if (nProStep1 > 13 && nProStep1 < 18)// HW INTERLOCK		 
			if (nProStep1 >= SELF_TEST_HV_LEAK1 && nProStep1 <=SELF_TEST_HV_LEAK_B)//son240130 14:SELF_TEST_HV_LEAK1, 17:SELF_TEST_HV_LEAK_B
			{
				if (nCo1 > 8)
				{
					nCo1 = 0;
					Sub71.OnPinAllReset1();
					Sub71.Delay_100ns(100);  //10us 
				}
			}

			nLoop = 5;
			nFail1 = 0;

			m_nProg2wIO_Rate = (int)(((((nProStepCnt - 1) * (m_nCardNoEnd + 1)) + card) * 100) / ((m_nCardNoEnd+1) * 8));

			BOOL bVBIdentity = FALSE;
			BOOL bVCIdentity = FALSE;

			for (int cardPin = 0; cardPin < nCardPinMax; cardPin++) 
			{
Task1:
				//son �̹� Self test Error�� �߻��� ī���� pass�Ѵ�.
				if (g_nCardS[card + 1][cardPin + 1] > 0) {
					continue;
				}

				DoEvents();

				//son Stop ��ư�� ���ȴ��� �߰� �߰�  üũ
				if (g_nStopFlag == 2) {
					g_nSelfRunFlag = 0;
					goto task2;
				}

				if (nStop == 2) {
					AfxMessageBox(" Errro No 1023, \n    PIN ALL RESET  SET ERROR !,      ,  R Calibraion Run Start Error!!!");
					goto task2;
				}

				// Not Leak
				//if (nProStep1 != 14 && nProStep1 != 15 && nProStep1 != 16 && nProStep1 != 17) {
				//	Sub71.OnPinAllReset1(); 
				//}

				//son LV Short1, Short2 �� ��� 
				if (nProStep1 == SELF_TEST_LV_SHORT1 || nProStep1 == SELF_TEST_LV_SHORT2) { //son240130 2:SELF_TEST_LV_SHORT1, 3:SELF_TEST_LV_SHORT2
					Sub71.Delay_100ns(10000); //1mS 
				}

				//--------------------------------
				//son VB, VC SelfTest PIN  ���� 				<====
				//--------------------------------

                nPin = GetHwPinNo_Old(card, cardPin);     //son231222_2 �Լ� ȣ��� �ڵ� ����ȭ //son240125_2

				//-------------------------------------------------------------------------------------
				//son Self OpenTest :  ���� ����Ʈ�� VB, VC�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                     ������ �� �帥�ٸ� ����, ������ �������Ϸ� �帥�ٸ� open�̴�.
				//-------------------------------------------------------------------------------------

				//VC PIN   set				<====
				if (nProStep1 == SELF_TEST_LV_OPEN) {  //  VB  VC  1��1 ���� �˻� //  LV ����   //son240130 1:SELF_TEST_LV_OPEN
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC); 
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB); 
				}

				//-------------------------------------------------------------------------------------
				//son Self ShortTest :  ����Ʈ�� VB, �Ǵ� VC�� �ٿ��� ������ �帣���� Ȯ���Ѵ�. 
				//                      ������ ���� ���Ϸ� �帣�ų� �帣�� �ʴ´ٸ� ����.
				//                      VB�� �ٿ��µ�(�Ǵ� VC�� �ٿ��µ�)  ������ �帥�ٸ� short�̴�. 
				//-------------------------------------------------------------------------------------

				else  if (nProStep1 == SELF_TEST_LV_SHORT1) {  //  case 2= short1,    pin one //son240130 2:SELF_TEST_LV_SHORT1
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB); 
				}
				else  if (nProStep1 == SELF_TEST_LV_SHORT2) {  //  case3= short2,     //son240130 3: SELF_TEST_LV_SHORT2
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC); 
				}
				else  if (nProStep1 == SELF_TEST_HV_LEAK1) {    //  case4= leak 1,    pin multi     //son240130 14:SELF_TEST_HV_LEAK1
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB); 
				}
				else  if (nProStep1 == SELF_TEST_HV_LEAK2) {	//    case 5= leak2   pin multi     //son240130 15:SELF_TEST_HV_LEAK2
					Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC); 
				}
				else  if (nProStep1 == SELF_TEST_HV_LEAK_A)		//leak A        //son240130 16:SELF_TEST_HV_LEAK_A
				{
					if (!bVBIdentity) {
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB); 
						bVBIdentity = TRUE;
						continue;				// ���� Pin����..
					}
					else {
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);
						bVBIdentity = FALSE;
						if(cardPin < nCardPinMax - 1)	
							cardPin--;	// VB, VC ���� Pin���� ����ϱ� ����.. ������ 1,3,5,.. -> ���� 1,2,3,4,5,...
					}
				}
				else  if (nProStep1 == SELF_TEST_HV_LEAK_B)		//LEAK B    //son240130 17:	SELF_TEST_HV_LEAK_B
				{
					if (!bVCIdentity) {
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VC);
						bVCIdentity = TRUE;
						continue;				// ���� Pin����..
					}
					else {
						Sub71.AOnPinSet1(1, nPin, MOD_TR_SET_VB);
						bVCIdentity = FALSE;
						if(cardPin < nCardPinMax - 1)	
							cardPin--;	// VB, VC ���� Pin���� ����ϱ� ����.. ������ 1,3,5,.. -> ���� 1,2,3,4,5,...
					}
				}

				//son SysSet15.m_nSet6  (SelfTest Delay Use Set.  7=USE)
//sylee230912				if (SysSet15.m_nSet6 == 7)	
//sylee230912				{
					int nTime1;
					if (nProStep1 == SELF_TEST_LV_OPEN) {       //son240130 1:SELF_TEST_LV_OPEN
						nTime1 = SysSet15.m_nSet2 * 10;
					}
					else  if (nProStep1 == SELF_TEST_SHORT1        //son240130 2:SELF_TEST_SHORT1
					        || nProStep1 == SELF_TEST_SHORT2)      //son240130 3:SELF_TEST_SHORT2
					{
						nTime1 = SysSet15.m_nSet3 * 10;
					}
					else  if (nProStep1 == SELF_TEST_HV_LEAK1       //son240130 14:SELF_TEST_HV_LEAK1
					        || nProStep1 == SELF_TEST_HV_LEAK2)     //son240130 15:SELF_TEST_HV_LEAK2
					{
						nTime1 = SysSet15.m_nSet4 * 10;
					}
					else  if (nProStep1 == SELF_TEST_HV_LEAK_A       //son240130 16:SELF_TEST_HV_LEAK_A
					        || nProStep1 == SELF_TEST_HV_LEAK_B)     //son240130 17:SELF_TEST_HV_LEAK_B
					{
						nTime1 = SysSet15.m_nSet5 * 10;					// 10 -> 20, fail ���� 
					}

					if (nTime1 < 0) {
						nTime1 = 0;
					}
					Sub71.Delay_100ns(nTime1);
	/*//sylee230912
	            }
				else
				{
					if (nProStep1 == 1) {  //  VB  VC  1��1 ���� �˻� //  LV ����  
						Sub71.Delay_100ns(3500);
					}
					else  if (nProStep1 == 2) {  //  case 2= short1,    pin one
						Sub71.Delay_100ns(1000);
					}
					else  if (nProStep1 == 3) {  //  case3= short2,  
						Sub71.Delay_100ns(1000);
					}
					else  if (nProStep1 == 14) {    //  case4= leak 1,    pin multi 
						Sub71.Delay_100ns(1000); //150uS   
					}
					else  if (nProStep1 == 15) {  //    case 5= leak2   pin multi 
						Sub71.Delay_100ns(1000); //100uS  
					}
					else  if (nProStep1 == 16) {  //leak A 
						Sub71.Delay_100ns(20000);//sylee150609  ray  500uS->2mS
					}
					else if (nProStep1 == 17) {//LEAK B	 
						Sub71.Delay_100ns(20000);//sylee150609  ray  500uS->2mS
					}
				}
	 	*/ //sylee230912

				//-------------------------------------
				//son Self ShortTest ����, ���� ����
				//-------------------------------------

				// Not Open
				if (nProStep1 != SELF_TEST_LV_OPEN) //son240130 1:SELF_TEST_LV_OPEN
				{
					Sub71.AOnReadV1I1(m_nVRel);         //son ����, ���� ����.

					//son Self ShortTest�ε� InterI���� ū ������ ������ �ȴٸ� ���� Short �׽�Ʈ�� �ߴ�. 
					if (Sub71.Y1 > InterI)
					{
						Sub71.OnPinAllReset1();
						Sub71.Delay_100ns(100);  //10us 
						if (m_nVRel < 3) {
							str.Format("Error No 8211-5,\n\n   lv short     \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
							                    card + 1, cardPin + 1, Sub71.Y1, InterI);
						}
						else {
							str.Format("Error No 8211-6,\n\n   hv short      \n\n Card=%d, Pin=%d,   I=%.2f >  I interlock=%.2f   \n  ", 
							                    card + 1, cardPin + 1, Sub71.Y1, InterI);
						}

						nRet1 = AfxMessageBox(str, MB_YESNO | MB_ICONEXCLAMATION);

                        //son240125_2 �޽����ڽ� No Ŭ���ÿ� List�� ��� �ȵǴ� ���� ����. 
                        //            �Լ��� ����� List �� ����� �ȵǹǷ� �Լ� ����� �ʰ� ��� �����ϵ��� ����. 
						//son240125_2 if (nRet1 != 6) {       //son OK ��ư�� �ƴ϶�� �Լ��� �����.	
						//son240125_2 	g_nStopFlag = 2;
						//son240125_2 	goto task2;
						//son240125_2 }
						goto TaskInterR101;
					}
				}

				//-----------------------------------------------
				//son  Self OpenTest, ShortTest ����, ���� ����
				//-----------------------------------------------

				Sub71.Delay_100ns(100);  //10uS 

				if (n4Wire == 4) { 
					Sub71.AOnReadV1I1(5); // READ  4W V  , I 
				}
				else {
					Sub71.AOnReadV1I1(m_nVRel);
				}
				//=============================================

TaskInterR101:

				//--------------------
				//son  R �� ���
				//--------------------

				if (m_nIR == 1 || m_nIR == 2 || m_nIR == 3 || m_nIR == 4) {	
					Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000.0;
				}
				else if (m_nIR == 5) {
					Sub71.R1 = (Sub71.V1 - Sub71.X1) / Sub71.Y1 * 1000000.0;
				}

				//son ù��° card ó�� ����ÿ�  ������ 5ȸ �ݺ�.
				if (card == m_nCardNoStart) {
					if (nLoop > 0) { 
						if (nLoop == 1) {
							cardPin = 0;
						}
						nLoop--;
						goto Task1;
					}
				}

				//son Open�� �ƴϸ�
				if (nProStep1 != SELF_TEST_LV_OPEN)     //son240130 1:SELF_TEST_LV_OPEN
				{
					Sub71.Y1 = dIADC1;  //son231012 int nIADC1  -> double dIADC1
					//son Short �� ���� ���������� ������ ��ȭ�� �����ϹǷ�  ADC I ���� �׳� R ������ ����Ѵ�.
					//son221014 Sub71.R1 = RCalConvert_spark(nJudgeMode, Sub71.Y1, RAW_TO_REAL, PART_UPPER);    //son �������� Convert
					//son230925 Sub71.R1 = RCalConvert_short(nJudgeMode, Sub71.Y1, RAW_TO_REAL, PART_UPPER);  //son221014 RCalConvert_spark()��  RCalConvert_short()�� ��ü. �Լ�����
					Sub71.R1 = RCalCovert(nJudgeMode, Sub71.Y1, RAW_TO_REAL, PART_UPPER);  //son230925 RCalConvert_short()�� RCalCovert()�� ����
				}


				if (nIso1 == 2)		//son Self ShortTest Error ����
				{
					//son ���ذ����� ������ ���� �帥�ٸ� Short�̴�.
					if (Sub71.Y1 > m_dISOSet1)
					{
						g_nCardS[card + 1][cardPin + 1] = nProStep1;

						//son ErrCount ������ �Ѿ�ٸ� ���� ī��� �Ѿ� ����.
						if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1)) {
							g_nErrCount1++;
							g_nErrCountTot++;
							StepErrCount++;
						}
						else {
							break;	//son ���� ī��� �Ѿ��.
						}

						dSelfErr1[g_nErrCount1][0] = (float)(card + 1);  //  CARD no.
						dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1);  //vc  cardPin
						dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1);  //vc pin total	   								 
						dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   //i
						dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   //R1
						dSelfErr1[g_nErrCount1][7] = (float)nProStep1;

						nFail1++;
					}
				}
				else //son nIso1==1  �� ���, Self OpenTest Error ����
				{
					nFailflag = 0;

					// Open  ???  nIso1==1 �̹Ƿ� nCase�� 1�� �ƴ� �� ����. ���� ��ġ ???
					if (nProStep1 == SELF_TEST_LV_OPEN)     //son240130 1:SELF_TEST_LV_OPEN
					{
						//son ���ذ����� ������ ���� �帣�ų� VC ���� ���� ������ �ȴٸ� open�̴�.
						//son m_dISOSet1(Self Test Open  I �Ѱ谪) ����  I ���� �۰ų� VC��(Sub71.X1)�� 13���� �۴ٸ�
						if ((Sub71.R1 > m_dISOSet1) || (Sub71.X1 < (m_dVSet * 0.8))) {
							nFailflag = 1;
						}

						//----------------------------------------
						//son  ���������� ������ ������ ����Ѵ�.

						//son 15V �����ߴµ� 13V ���Ϸ� ������ ���  (Open Self Test ���� �Ұ�)
						if (Sub71.X1 < (m_dVSet * 0.8))
						{
							if (nEarthFlag == 0)
							{
								nEarthFlag = 1;
								//AfxMessageBox(" Error No 8027, \n\n   if all point is  (same voltage<8),   Earth!?,  All  Cable, pin block, jig,  etc. Check!?  \n\n   \n\n\n    , Self Test   Error!!!"); 
								//if( nG_Language!=2){//sylee150707
								//    str.Format(" *> [���� �߻�: �������� �Ҽ� ����]:   ");												 
								//    OnList1Mes1(str); 
								//    str.Format(" *> [��ġ���]:   ");												 
								//    OnList1Mes1(str); 
								//    str.Format(" *> 1) ����Ż���Ͽ� Ȯ��! ������������");	
								//    OnList1Mes1(str); 
								//    str.Format(" *> 2) ����ġī�带 ��ȯ�Ͽ� Ȯ���Ͻʽÿ�! ");
								//    OnList1Mes1(str); 
								//    str.Format(" *> 3) ����ġī�忡 �������̺��� �и�Ȯ��! ������������");
								//    OnList1Mes1(str); 	
								//}else{

								str.Format(" *> [Error : Cannot process Self OpenTest]:   ");
								OnList1Mes1(str);
								str.Format(" *> [Check point]:   ");
								OnList1Mes1(str);
								str.Format(" *>   Case1) Ground Error. Remount the jig and test again.");
								OnList1Mes1(str);
								str.Format(" *>   Case2) Change the SwitchCard and test again.");
								OnList1Mes1(str);
								str.Format(" *>   Case3) Check if the Flat Cable is disconnected.");
								OnList1Mes1(str);
							}
						}
					}

					if (nFailflag == 1)		//son Open SelfTest Error ���� 		 
					{
						g_nCardS[card + 1][cardPin + 1] = 1;

						//son ErrCount ������ �Ѿ�ٸ� ī���� ������ ������ ���� ī��� �Ѿ� ����.
						if (g_nErrCount1 < (MAX_SELF_ERR_PIN - 1))
						{
							g_nErrCount1++;
							g_nErrCountTot++;
							StepErrCount++;
						}
						else {
							break;		//son ���� ī��� �Ѿ��.
						}

						dSelfErr1[g_nErrCount1][0] = (float)(card + 1);  //  CARD no.
						dSelfErr1[g_nErrCount1][1] = (float)(cardPin + 1);  //vb  pin
						dSelfErr1[g_nErrCount1][2] = (float)(nPin + 1); 
						dSelfErr1[g_nErrCount1][3] = (float)Sub71.V1;  //vb
						dSelfErr1[g_nErrCount1][4] = (float)Sub71.X1;  //vc
						dSelfErr1[g_nErrCount1][5] = (float)Sub71.Y1;   //i
						dSelfErr1[g_nErrCount1][6] = (float)Sub71.R1;   //R1
						dSelfErr1[g_nErrCount1][7] = (float)nProStep1;

						nFail1++;

						if (nProStep1 == SELF_TEST_LV_OPEN) {   //son240130 1:SELF_TEST_LV_OPEN
							if (g_nOpenErrPinCount < MAX_SELF_OPEN_ERR_PIN)
							{
								if ((Sub71.X1 < 2) && (Sub71.Y1 < 2)) {
									g_nOpenErrPinCount++;
									g_nOpenErrPin[g_nOpenErrPinCount][1] = (card + 1);  //SWITCH CARD No
									g_nOpenErrPin[g_nOpenErrPinCount][2] = (cardPin + 1);  //SWITCH CARD Pin No
									g_nOpenErrPin[g_nOpenErrPinCount][3] = nPin + 1;  //  SWITCH ��  PIN 
									//son231222_4 g_nOpenErrPin[g_nOpenErrPinCount][4] = nPinSW_Net(nPin + 1);  //  BDL PIN
									g_nOpenErrPin[g_nOpenErrPinCount][4] =  nJigPinMap[nPin] + 1; //son231222_4   nPin�� hwPin0
								}
							}
						}
					}
				}

				if (SysSet13.m_nPinBlockType == PIN_BLK_12K_QD_DD) {
					if (card < 64) { //64=128x64=8k
						nCardPinMax = 128;
					}
					else {
						nCardPinMax = 64;
					}
				}

				//son ������ ī���϶� Fail Ƚ�� ���
				if (cardPin == (nCardPinMax - 1))     // 	nCardPinMax=64;
				{
					if (nFail1 > 0)
					{
						if (nProStep1 == SELF_TEST_LV_OPEN) {    //son240130 1:SELF_TEST_LV_OPEN
							str.Format(" * Error,  LV Open, Switch Card=%d,  FailPin Count=%d", card + 1, nFail1);
						}
						else {
							str.Format(" * Error,  Switch Card=%d,  FailPin Count=%d", card + 1, nFail1);
						}
						OnList1Mes1(str);
					}
				}

				// �� Pin���� Reset..
				Sub71.OnPinAllReset1(); 
				Sub71.Delay_100ns(1000);

//				if (nProStep1 == 14 || nProStep1 == 15 || nProStep1 == 16 || nProStep1 == 17) {
//					if (Sub71.Y1 > 500) {
//						Sub71.OnPinAllReset1(); 
//						Sub71.Delay_100ns(1000);  //100us 
//					}
//				}
			}////for END   for( cardPin=0; cardPin<64; cardPin++)  //pin  vb pin
		} //for END  for( card=0; card< nCount; card++)  //count

		//son240205 str.Format(" * [  END :: Case %d ] %s ", nProStep1 % 10, strItems[nProStep1]);
		str.Format(" * [  END :: Case %d ] %s ", nProStep1 % 10, getStrSelfTestCase2(nProStep1));  //son240205

		if (StepErrCount == 0) {
			if (nDisplayDetail1 != 1) {
				str2.Format("  ***> PASS *");
				str = str + str2;
				OnList1Mes1(str);
			}
		}
		else {
			str2.Format("  ***> FAIL *");
			str = str + str2;
			OnList1Mes1(str);
		}

	task2:  //STOP JUMP POSTION

		Sub71.OnPinAllReset1();

		if (g_nStopFlag == 2) {
			g_nSelfRunFlag = 0;
			break;
		}
	}//   for  prostep

	//log
	int nProStep2Tmp;
	int nErrorType[30];

	::ZeroMemory(&nErrorType, sizeof(nErrorType));

    //son240130 1:SELF_TEST_LV_OPEN,  17:(SELF_TEST_MAX-1)
	for (nProStep2Tmp = SELF_TEST_LV_OPEN; nProStep2Tmp <= (SELF_TEST_MAX-1); nProStep2Tmp++)   //son240130
	{
		if (nProStep2Tmp == SELF_TEST_LV_OPEN) {    //son240130 1:SELF_TEST_LV_OPEN
			str.Format("--------------------------------------- ");
			OnList1Mes1(str);
		}

		int dd, ePin1, ntmp1;

		for (int err = 1; err <= g_nErrCount1; err++)
		{
			dd = err % 2;
			ntmp1 = (int)dSelfErr1[err][7];	// hdpark220819 g_nErrCount1 -> err

			if (ntmp1 != nProStep2Tmp) {
				continue;
			}

			nErrorType[nProStep2Tmp]++;

            //son ù��° error��
			if (nErrorType[nProStep2Tmp] == 1) {
				fprintf(fp1, "  \n\n ");
			}

            //son ù��° Open Error ���
			if (nErrorType[nProStep2Tmp] == 1 && nProStep2Tmp == SELF_TEST_LV_OPEN) {   //son240130 1:SELF_TEST_LV_OPEN
				fprintf(fp1, "   *,   No,  NetPin,  CardNo,  CardPin,  Voltage,   R, \n ");
			}

            //son ù��° Short, Leak Error ���
			if (nErrorType[nProStep2Tmp] == 1 && nProStep2Tmp != SELF_TEST_LV_OPEN) {   //son240130 1:SELF_TEST_LV_OPEN
				fprintf(fp1, "  , No, NetVbPin, NetVcPin, CardNo, SWVbPin, SWVcPin,  SW VbPinTotal, SW VcPinTotal,   VB,      VC,     I,            R,   \n ");
			}

			//ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);   //son231222_4
			ePin1 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4

			nProStep2Tmp = ntmp1;

			//son List â�� ����ϱ� 
			//son240205 fprintf(fp1, "%s", strItems[nProStep2Tmp]);
			fprintf(fp1, "%s", getStrSelfTestCase2(nProStep2Tmp));  //son240205

			if (nDisplayDetail2 == 1) {
				str.Format("No=%d, Net Pin=%d, (Switch Card=%3.0f,  Pin=%3.0f)", err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1]);
				OnList1Mes1(str);
			}

			//ePin1 = nPinSW_Net((int)dSelfErr1[err][2]);   //son231222_4
			ePin1 = nJigPinMap[(int)dSelfErr1[err][2] - 1] + 1; //son231222_4

			//son Open Error ���.
			if (nProStep2Tmp == SELF_TEST_LV_OPEN) {	    //son240130 1:SELF_TEST_LV_OPEN
				fprintf(fp1, "   %d,  %d,  %3.0f,  %3.0f, %3.1f,  %12.1f, ",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][4], dSelfErr1[err][6]);
			}
			//son Short Error VC Pin ���.
			else  if (nProStep2Tmp == SELF_TEST_LV_SHORT2       //son240130 3: SELF_TEST_LV_SHORT2
			        || nProStep2Tmp == SELF_TEST_HV_LEAK2       //son240130 15:SELF_TEST_HV_LEAK2
			        || nProStep2Tmp == SELF_TEST_HV_LEAK_B)     //son240130 17:SELF_TEST_HV_LEAK_B
			{	
				fprintf(fp1, "   %d,     *,     %d,    %5.0f,       *,    %5.0f,         *, %5.0f, ",
					            err,            ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
			}
			//son Short Error VB Pin ���.
			else  if (nProStep2Tmp == SELF_TEST_LV_SHORT1       //son240130 2:  SELF_TEST_LV_SHORT1
			        || nProStep2Tmp == SELF_TEST_HV_LEAK1       //son240130 14: SELF_TEST_HV_LEAK1
			        || nProStep2Tmp == SELF_TEST_HV_LEAK_A)     //son240130 16: SELF_TEST_HV_LEAK_A
			{
				fprintf(fp1, "   %d, %d,   *,      %5.0f,        %5.0f,       *,    %5.0f,      *,",
					err, ePin1, dSelfErr1[err][0], dSelfErr1[err][1], dSelfErr1[err][2]);
			}

            //son Short Error ������ �κ� ���
			if (nProStep2Tmp != SELF_TEST_LV_OPEN) {    //son240130 1:SELF_TEST_LV_OPEN
				fprintf(fp1, "  %5.1f,  %5.1f,  %5.1f,  %12.1f, ", dSelfErr1[err][3], dSelfErr1[err][4], dSelfErr1[err][5], dSelfErr1[err][6]);
			}
			fprintf(fp1, " \n");
		} //for( err=1; err<=g_nErrCount1; err++)
	}

	Sub71.OnPinAllReset1();
	Sub71.AOnPoweroff2();
	Sub71.FilterSet(0);
	Sub71.I_Filter(0);

	if (g_nStopFlag == 2)
	{
		g_nSelfRunFlag = 0;
		g_nSelfAutoFlag1 = 0;

		str.Format("  * Stop   [Self Test  ::  Switch Card  TR ]  ");
		OnList1Mes1(str);
		AfxMessageBox(str);
	}

	if (g_nErrCountTot < 1) {
		fprintf(fp1, "    ***NO ERROR *** ");
	}
	else {
		fprintf(fp1, "    ***  TOTAL ERROR  = %d  *** ", g_nErrCountTot);
	}

	fprintf(fp1, "  \n  \n   ************************************************* ");
	fclose(fp1);

	if ((g_nStopFlag != 2) && (g_nErrCount1 < 1)) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1; 
		//   AfxMessageBox("    Self Test.   OK!! ", MB_OK);
	}

	if (g_nErrCount1 > 0)
	{
		if (nDisplayDetail1 != 1) {
			if (SysSet13.m_nSet7 == 1) {
				::ShellExecute(NULL, "open", "EXCEl.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
			else {
				::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
			}
		}
	}

	if (g_nErrCount1 > 0) {
		str.Format("NG");
	}
	else
	{
		if (g_nStopFlag == 2) {
			str.Format("Stop");
		}
		else {
			str.Format("OK");
			m_nProg2wIO_Rate = 100;
		}
	}

	m_Label_2wIO.SetCaption(str);

	if (g_nStopFlag == 2) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;
		if (m_SelfAutoFlag1 != 2) {
			g_nStopFlag = 0; 
		}
	}

	g_nAutoFlag = 0;
	g_nSelfRunFlag = 0;

	if (g_nOpenErrPinCount > 0)
	{
		strfName1.Format("d:\\log\\SelfError_EarthPin.CSV"); 
		DeleteFile(strfName1);
		::ZeroMemory(&fName1, sizeof(fName1));
		strcat(fName1, strfName1);
		fp1 = fopen(fName1, "wt");
		if (fp1 == NULL) {
			str.Format("Error    d:\\log\\SelfError_EarthPin.CSV      file open   ?  Error, Saving Error !     file close!   ");
			AfxMessageBox(str, MB_OK); //error messege
			g_nSelfRunFlag = 0;
			return;
		}

		CTime curTime = CTime::GetCurrentTime();
		fprintf(fp1, " \n\n");

		/*
		   if( nG_Language!=2){//sylee150707

		   str.Format(  " *   �߻� �ð� : _%d�� %d�� %d��  _%d�� %d�� %d��  ",
						curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
						curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
		   fprintf(fp1,str);	 fprintf(fp1, "  \n\n");
		   str.Format( "  **>[���� �߻�]: ���� �ɿ� ����   ����������  �Ҽ� �����ϴ�.  ");
		   fprintf(fp1,str);
		   fprintf(fp1, " \n\n");
		   str.Format( "  **>[��ġ ���]:   ");
		   fprintf(fp1,str);
		   fprintf(fp1, " \n");

		   str.Format( "    1���) ���׸� Ż���� ���� ��������  ���׳��ο��� ������ ������ �ֽ��ϴ�." );
		   fprintf(fp1,str);    fprintf(fp1, " \n");
		   str.Format( "    2���) ���� ����ġ ī�� �ڽ��� ������ �ΰ� �Ǿ����� Ȯ���Ͻʽÿ�.  LED ���� ON Ȯ��.   " );
		   fprintf(fp1,str);  		  fprintf(fp1, " \n");
		   str.Format( "    3���) ����ġī�� ��ü�Ͽ� ����ȭ �Ǵ��� Ȯ���մϴ�.  "  );
		   fprintf(fp1,str);  		  fprintf(fp1, " \n");
		   str.Format( "    4���)  ���� �� ����ġ ī�忡��    �� �������� ����� ���̺� ���� ���� �и��Ͽ� ���� ����    " );
		   fprintf(fp1,str);            fprintf(fp1, " \n");
		   str.Format( "                ����� ���̺��� �ɺ�ϱ���  ������ �ֽ��ϴ�.    " );
		   fprintf(fp1,str);            fprintf(fp1, " \n");
		   str.Format( "                �̿Ͱ��� ����� �κ���  ���� �и��ϸ鼭 ��ġ��  ã�� �ذ��մϴ�. " );
		   fprintf(fp1,str);            fprintf(fp1, " \n");
		   }else{
		   */

		str.Format(" *  Error time :  %d/%d/%d - %d:%d:%d   ",
			curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),
			curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
		fprintf(fp1, str);	 fprintf(fp1, "  \n\n");
		str.Format("   **> [Error : Cannot process Self OpenTest]:   ");
		fprintf(fp1, str);
		fprintf(fp1, " \n\n");
		str.Format("   **> [Check Points]:   ");
		fprintf(fp1, str);
		fprintf(fp1, " \n");

		str.Format("       Case1) Ground Error. Remount the jig and test again.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case2) Make sure BBT Controller is powered on.  Check LED lamp of board. ");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case3) Change the Switch Card and test again.");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		str.Format("       Case4) Check if the flat Cable is disconnected.\n");
		fprintf(fp1, str);
		fprintf(fp1, " \n");
		fprintf(fp1, " \n\n\n");

		for (int err = 1; err <= g_nOpenErrPinCount; err++)
		{
			str.Format("  **> Error:  jigPino=%d, (switch card=%d, cardPin=%d, pin=%d)  ",
				g_nOpenErrPin[err][4], g_nOpenErrPin[err][1], g_nOpenErrPin[err][2], g_nOpenErrPin[err][3]);
			fprintf(fp1, str);
			fprintf(fp1, " \n");
		}

		if (g_nOpenErrPinCount == MAX_SELF_OPEN_ERR_PIN)
		{
			fprintf(fp1, " \n\n");
			str.Format("  **> Open error pinCount range Over!  (< 200 )");
			fprintf(fp1, str);
			fprintf(fp1, " \n");
		}

		fclose(fp1);
		if (SysSet13.m_nSet7 == 1) {
			::ShellExecute(NULL, "open", "EXCEl.EXE", fName1, "NULL", SW_SHOWNORMAL);
		}
		else {
			::ShellExecute(NULL, "open", "notepad.EXE", fName1, "NULL", SW_SHOWNORMAL);
		}
	}

	//str.Format("\n  ***<  IO Check Finish > ***   ");
	str.Format("  ***<  IO Check Finish > ***   ");  //son240201

	OnList1Mes1(str);

	if (nStop == 2) {
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
		g_nSelfTestInterlock = 1;
		AfxMessageBox("   :  Switch Card 2W Pin Check.   Stop!!!");
	}

	Sub71.OnPinResetInit();
	Sub71.AOnPoweroff2();

	return;
}
#endif	//hdpark220819



void CChildView5::OnButtonStop() //stop
{

	g_nStopFlag = 2;
	Sleep(100);//sylee140702
		//son SelfTest�� ���� ���� ���� �����ٴ� �� ǥ���Ѵ�.
	g_nSelfTestInterlock = 1;
	nLeakRunLV=0;//sylee240530
	nLeakRunHV=0;//sylee240530

	return;
}




void CChildView5::OnButtonStart() //auto start
{
	CString str;

	if (m_SelfAutoFlag1 == 2) {
		return;
	}

	m_SelfAutoFlag1 = 2;

	m_list1.ResetContent();
	g_nErrCount1 = 0;

	m_nProgSystem_Rate = 0;
	m_nProgVB_Rate = 0;
	m_nProgAD_Rate = 0;
	m_nProg2wIO_Rate = 0;
	m_nProg4wIO_Rate = 0;
	m_nProg2wLeak_Rate =0 ;

	str.Format(" ");
	m_Label_System.SetCaption(str);
	m_Label_VB.SetCaption(str);
	m_Label_AD.SetCaption(str);
	m_Label_2wIO.SetCaption(str);
	m_Label_4wIO.SetCaption(str);
	m_Label_2wLeak.SetCaption(str);


	if (g_nErrCount1 < 1)
	{
		if (m_check_System.GetCheck())
		{
			if (g_nStopFlag != 2) {
				OnButtonSystemCheck(); //System
			}
		}
		if (m_check_VB.GetCheck())
		{
			if (g_nStopFlag != 2) {
				OnButtonVbCheck(); //vb check 
			}
		}
		if (m_check_AD.GetCheck())
		{
			if (g_nStopFlag != 2) {
				OnButtonAdCheck(); //ad check 
			}
		}
		if (m_check_2wIO.GetCheck())
		{
			if (g_nStopFlag != 2) {
		       OnButton_2wIoCheck(); //iocheck 		<====
			}
		}

		if (m_check_2wLeak.GetCheck())//sylee240509-2
		{
			if (g_nStopFlag != 2) {
				OnButton_2wLeak(); //LEAK CHECK //sylee240509-2
			}
		}

		if (m_check_4wIO.GetCheck())
		{
			if (g_nStopFlag != 2) {
		     	OnButton_4wIoCheck(); //iocheck 
			}
		}
	}



	m_SelfAutoFlag1 = 0;

	g_nStopFlag = 0;


	Sub71.AOnReadV1Enable1(5);//sylee150122
	Sub71.AOnReadV1Init1(1);//sylee150122



	return;
}




void CChildView5::OnButtonMessageClear() //message Clear
{
	m_list1.ResetContent();

	return;
}




void CChildView5::OnCheck_System()
{
	if (m_check_System.GetCheck())
	{

	}
	else
	{


	}
}


void CChildView5::OnCheck_VB()
{
	if (m_check_VB.GetCheck())
	{
		//	nCycleAuto1  = 1;

	}
	else
	{

	}

}



void CChildView5::OnCheck_AD()
{
	if (m_check_AD.GetCheck())
	{
		//	nCycleAuto1  = 1;

	}
	else
	{

	}

}


void CChildView5::OnCheck_2wIO()
{
	if (m_check_2wIO.GetCheck())
	{
		//	nCycleAuto1  = 1;

	}
	else
	{

	}

}


void CChildView5::OnCheck_4wIO()
{
	if (m_check_4wIO.GetCheck())
	{
		//	nCycleAuto1  = 1;

	}
	else
	{

	}

}






int CChildView5::AutoSelf_PowerModeSet(int m_nMode1)
{


	int nRet1, Lo1;
	__int64   tStart1, tStart2;
	double dVbR1, dVbR2;
	double nT[10];
	//int m_nFil, m_nIFilter,m_nHVOffSet,m_nHVGain;//sylee130629
	//	double V1old;//sylee150210 //ACE380
	//   V1old=0.0;//sylee150210 //ACE380

	CString str;
	memset(nT, 0x00, sizeof(nT));

	if (m_nMode1 != dRec[m_nMode1][0]) {
		AfxMessageBox(" Error No 8516, \n   Self Test Error    Parameter loading Error  V I Delay");
		return 0;
	}


	m_nCC = (int)dRec[m_nMode1][1];
	m_nVRel = (int)dRec[m_nMode1][2];
	m_dVSet = dRec[m_nMode1][3];
	// m_dVSet= (int)40;
	m_nIR = (int)dRec[m_nMode1][4];
	m_dISet = dRec[m_nMode1][5];

	m_nFil = (int)dRec[m_nMode1][8];  //    1=VC FAST, 2= VC SLOW ,  3=HV OPEN, ( 4=4W )
	nTimeDelay1 = (int)(dRec[m_nMode1][6] * 10);//syle20120104     SETUPCALIBRATION

	m_nIFilter = (int)dRec[m_nMode1][9];   //sylee130629		  
	m_nHVOffSet = (int)dRec[m_nMode1][10]; //sylee130629
	m_nHVGain = (int)dRec[m_nMode1][11];	//sylee130629	 
	///***************************************************
	/*
   if( m_dVSet>20){
	//m_nFil= (int) dRec[m_nMode1][8];
	m_nFil= 1;//slow speed
	}else{
	m_nFil= 0;//fast speed
	}
	*/
	nTimeDelay1 = (int)(dRec[m_nMode1][6] * 10);//syle20120104     SETUPCALIBRATION

	//ADC COUNT
	Sub71.nADCount1 = (int)dRec[m_nMode1][7];

	if (Sub71.nADCount1 < 1) {
		Sub71.nADCount1 = 1;
	}

	if (Sub71.nADCount1 > 100) {
		Sub71.nADCount1 = 100;
	}
	nRet1 = Sub71.AOnPoweroff2();//sylee20120702 Sub71.AOnPoweroff();

	if (nRet1 != 1) {
		//	fclose(fp);
		//	fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 8316, \n   Power all off fail !,   SW TEST Run Start Error!!!");
		g_nSelfRunFlag = 0;
		return  0;
	}


	if (m_nVRel == 1) {
		nRet1 = Sub71.AOnVSetMulti1(m_dVSet, 0, 0, 0);
	}
	else  if (m_nVRel == 2) {
		nRet1 = Sub71.AOnVSetMulti1(0, m_dVSet, 0, 0);
	}
	else  if (m_nVRel == 3) {
		nRet1 = Sub71.AOnVSetMulti1(0, 0, m_dVSet, 0);
	}
	else  if (m_nVRel == 4) {
		nRet1 = Sub71.AOnVSetMulti1(0, 0, 0, m_dVSet);
	}


	if (nRet1 != 1) {
		//	fclose(fp);
		//	fclose(fp1);//sylee20111224
		AfxMessageBox(" Error No 1316, \n   V SET ERROR !,   SW TEST Run Start Error!!!");
		g_nSelfRunFlag = 0;
		return 0;
	}


	Sub71.Delay_100ns(10000000);  //1 sec  //sylee20111214 

	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	tStart1 = GetMilSecond2();

	Lo1 = 3;

Task101:

	Sub71.AOnReadVB1(m_nVRel);   // VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 

	if (m_dVSet < 5) {
		dVbR2 = m_dVSet * (0.35);//35%
	}
	else {
		dVbR2 = m_dVSet * (0.1);//10%  //sylee211118  RAY  5->10%
	}
	dVbR1 = m_dVSet - dVbR2;
	dVbR2 = m_dVSet + dVbR2;

	if (Sub71.V1 > 300)
	{
		if (Lo1-- > 0) {
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}

		str.Format("Error No 1312,\n \n\n  Power Check? \n\n\n    Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1, m_dVSet, Sub71.V1, dVbR1, dVbR2);
		AfxMessageBox(str);
		g_nSelfRunFlag = 0;
		//	return ;
	}

	if (Sub71.V1<dVbR1 || Sub71.V1>dVbR2)
	{
		if (Lo1-- > 0) {
			Sub71.Delay_100ns(3000000);  //300ms   
			goto Task101;
		}

		str.Format("Error No 8023,\n \n\n  Power Check? \n\n\n      Calibration NO=%d VB READ Error!!! \n VB Set=%.2f, VB Read =%.2f ,\n Range(VB low=%.2f, VB high =%.2f),", (int)m_nMode1, m_dVSet, Sub71.V1, dVbR1, dVbR2);
		AfxMessageBox(str);
		//	return ;
	}


	if (m_nFil == 1 || m_nFil == 2) {//sylee130628
		Sub71.AOnReadV1Enable1(m_nVRel);
	}
	else {
		Sub71.AOnReadV1Enable1(5);
	}

	if (m_nFil == 3) {
		Sub71.HV_OffSet_Gain(m_nHVOffSet, m_nHVGain);    //sylee140226
	}

	Sub71.FilterSet(m_nFil);//sylee130628 
	if (m_nIFilter == 3) {
		AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!");
		m_nIFilter = 1;
	}
	Sub71.I_Filter(m_nIFilter);//sylee130629

	tStart2 = GetMilSecond2();
	nT[0] = (double)(tStart2 - tStart1) / 1000.;  //test

	Sub71.OnPinAllReset1(); ///PIN ALL RESET
	Sub71.AOnISetOn1(m_nCC, m_nVRel, m_dVSet, m_nIR, m_dISet);  //     //sylee150210  MOVE 
	Sub71.Delay_100ns(3000000);  //300ms      //sylee150210  MOVE
	Sub71.OnPinResetInit();//sylee151221   //ACE400
	return  1;

}








#if 0 //son231222_4 ChildView5::nPinSW_Net() �Լ� �����ϰ� nJigPinMap[hwPin0] + 1��  ��ü. �ӵ����� 

//son HW pin�� ������ SW pin�� ã�� �Լ�.
int  CChildView5::nPinSW_Net(int hwPin)//sylee131117   sw->net
{

	CString str;
	//	char szText[50];
	int nPin, jigPin;

	//sylee150903 //ACE400	int nCardPinMax;//sylee150210 //ACE380

	//ACE380//////////////////////////////////////////////////////////////
	//sylee150903 //ACE400	nCardPinMax=64;//sylee150210 //ACE380

	Get_nCardPinMax();//sylee150903 //ACE400    //son231227
//ACE380//////////////////////////////////////////////////////////////



	nPin = hwPin;			//son nPin�� hwPin

	if (nPin < 0) {
		return -1;
	}
	if (nPin > 32768) {
		return -2;
	}

	//son nPin(hwPin)�� �´� jigPin�� Search�Ѵ�.
	for (int i = 0; i <= 32768; i++) {
		if (nPin == PinMap[2][i]) {
			jigPin = i; 	//son  i, jigPin��  jigPin,  
			break;
		}
	}

	if (jigPin < 0) {//sylee131123
		//	str.Format("    *  Search Error  . Set Again!. \n\n     [Pin]  SET Check ex,QD,SD,DD,    Z, H,     Reverse, Normal  "   );
		//  AfxMessageBox(str);
		return -3;
	}

    //son netPin, QdPin, hwPin(switchPin)
	int lA, lB, lC;

	//sylee20120617// [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN,
	lA = PinMap[0][jigPin];	//son  lA: jigPin�� ������ ��
	lB = PinMap[1][jigPin];
	lC = PinMap[2][jigPin];	//son  lC: jigPin�� �´� switchPin(hwPin)


	int box, card, cardPin;

	//son boxSize�� 2048�̸�
	if (SysSet12.m_nPinBoxMax == 1) {  //2k //sylee150210
		box = lC / 2048 + 1;
		card = (lC % 2048) / nCardPinMax + 1;//nCardPinMax;//sylee150210 //ACE380
		cardPin = (lC % 2048) % nCardPinMax + 1;//nCardPinMax;//sylee150210 //ACE380
	}

	//son boxSize�� 4096�̸�
	else {
		box = lC / 4096 + 1;
		card = (lC % 4096) / nCardPinMax + 1;//nCardPinMax;//sylee150210 //ACE380
		cardPin = (lC % 4096) % nCardPinMax + 1;//nCardPinMax;//sylee150210 //ACE380 
	}


	//	str.Format("   NO 8003 * NET  =%d (1~),  \n\n * QD .=%d(1~ ) , \n\n SWITCH =%d(1~ ),box=%d,card=%d,pin=%d ,    ",lA+1,lB+1 ,lC+1 ,box,card,cardPin );
	//	AfxMessageBox(str);

	//sylee131123return lA+1;

	return lA;		// jigPin ���� ����	

}
#endif





int CChildView5::PowerCheckError1()//sylee140212				
{


	BYTE Buf;
	CString str1;
#ifdef DEF_CARD_A1  //sylee170328
	int nChk1, Ret, i, j2;
#else  //#ifdef DEF_CARD_A1 
	int nChk1, Ret;
#endif  //#ifdef DEF_CARD_A1 

    //son "Map > Set1 > Selftest System Off" ��  On �̸�
	if (SysSet13.m_nSet4 == 1) {//sylee161118-1

		str1.Format("RUN");
		m_Label_System.SetCaption(str1);
		DoEvents();	DoEvents();	Sleep(200);
		m_nProgSystem_Rate = 30;  DoEvents();	DoEvents();	Sleep(200);
		m_nProgSystem_Rate = 50;  DoEvents();	DoEvents();	Sleep(200);
		m_nProgSystem_Rate = 70;  DoEvents();	DoEvents();	Sleep(200);
		m_nProgSystem_Rate = 100; DoEvents();    DoEvents();	Sleep(200);

		str1.Format("OK");
		m_Label_System.SetCaption(str1);

		return 1;
	}

	nChk1 = 0;
	m_nProgSystem_Rate = 0;
	str1.Format("RUN");
	m_Label_System.SetCaption(str1);
	g_nErrCount1 = 0;
	//-------------------------------------------------------------------------------	

	m_nProgSystem_Rate = 5;

	//int nChk1,i,Ret ;

#ifdef DEF_CARD_A1  //sylee170328

	for (i = 0; i < 8; i++) {
		Ret = DioInpByte(pDIO.hDrv2, i, &Buf);
		if (Ret != 0) {
			str1.Format("  [DioInpByte, %d]  Digital Card  Com. Error!!! ", i);
			OnList1Mes1(str1);
			AfxMessageBox(str1);
			return  0;
		}

		if (Buf == 255) {
			nChk1++;
		}
	}

#else  //#ifdef DEF_CARD_A1   


	int nOnCo1;//sylee170328

	nOnCo1 = 0;
	//--------------------
	//son D64H Bd0  Read

	Ret = d64h_di_readport(pDIO_D64.m_bCardID, &pDIO_D64.dwDIStatus);  //

	if (Ret != 0) {
		AfxMessageBox("  Card64H1_DI32() Error!!!");
	}
	else
	{
		if ((pDIO_D64.dwDIStatus & 0x00000001) != 0)	nOnCo1++;       //son D64H BD0 DI00 ADC_A_D0 
		if ((pDIO_D64.dwDIStatus & 0x00000002) != 0)	nOnCo1++;       //son D64H BD0 DI01 ADC_A_D1       
		if ((pDIO_D64.dwDIStatus & 0x00000004) != 0)	nOnCo1++;       //son D64H BD0 DI02 ADC_A_D2
		if ((pDIO_D64.dwDIStatus & 0x00000008) != 0)	nOnCo1++;       //son D64H BD0 DI03 ADC_A_D3 
		if ((pDIO_D64.dwDIStatus & 0x00000010) != 0)	nOnCo1++;       //son D64H BD0 DI04 ADC_A_D4  
		if ((pDIO_D64.dwDIStatus & 0x00000020) != 0)	nOnCo1++;       //son D64H BD0 DI05 ADC_A_D5 
		if ((pDIO_D64.dwDIStatus & 0x00000040) != 0)	nOnCo1++;       //son D64H BD0 DI06 ADC_A_D6  
		if ((pDIO_D64.dwDIStatus & 0x00000080) != 0)	nOnCo1++;       //son D64H BD0 DI07 ADC_A_D7  

		if ((pDIO_D64.dwDIStatus & 0x00000100) != 0)	nOnCo1++;       //son D64H BD0 DI08 ADC_A_D8 
		if ((pDIO_D64.dwDIStatus & 0x00000200) != 0)	nOnCo1++;       //son D64H BD0 DI09 ADC_A_D9 
		if ((pDIO_D64.dwDIStatus & 0x00000400) != 0)	nOnCo1++;       //son D64H BD0 DI10 ADC_A_D10 
		if ((pDIO_D64.dwDIStatus & 0x00000800) != 0)	nOnCo1++;       //son D64H BD0 DI11 ADC_A_D11 
		if ((pDIO_D64.dwDIStatus & 0x00001000) != 0)	nOnCo1++;       //son D64H BD0 DI12 ADC_A_D12 
		if ((pDIO_D64.dwDIStatus & 0x00002000) != 0)	nOnCo1++;       //son D64H BD0 DI13 ADC_A_D13 
		if ((pDIO_D64.dwDIStatus & 0x00004000) != 0)	nOnCo1++;       //son D64H BD0 DI14 ADC_A_D14 
		if ((pDIO_D64.dwDIStatus & 0x00008000) != 0)	nOnCo1++;       //son D64H BD0 DI15 ADC_A_D15 

		if ((pDIO_D64.dwDIStatus & 0x00010000) != 0)	nOnCo1++;       //son D64H BD0 DI16 ADC_B_D0  
		if ((pDIO_D64.dwDIStatus & 0x00020000) != 0)	nOnCo1++;       //son D64H BD0 DI17 ADC_B_D1  
		if ((pDIO_D64.dwDIStatus & 0x00040000) != 0)	nOnCo1++;       //son D64H BD0 DI18 ADC_B_D2  
		if ((pDIO_D64.dwDIStatus & 0x00080000) != 0)	nOnCo1++;       //son D64H BD0 DI19 ADC_B_D3  
		if ((pDIO_D64.dwDIStatus & 0x00100000) != 0)	nOnCo1++;       //son D64H BD0 DI20 ADC_B_D4  
		if ((pDIO_D64.dwDIStatus & 0x00200000) != 0)	nOnCo1++;       //son D64H BD0 DI21 ADC_B_D5  
		if ((pDIO_D64.dwDIStatus & 0x00400000) != 0)	nOnCo1++;       //son D64H BD0 DI22 ADC_B_D6  
		if ((pDIO_D64.dwDIStatus & 0x00800000) != 0)	nOnCo1++;       //son D64H BD0 DI23 ADC_B_D7  

		if ((pDIO_D64.dwDIStatus & 0x01000000) != 0)	nOnCo1++;       //son D64H BD0 DI24 ADC_B_D8  
		if ((pDIO_D64.dwDIStatus & 0x02000000) != 0)	nOnCo1++;       //son D64H BD0 DI25 ADC_B_D9  
		if ((pDIO_D64.dwDIStatus & 0x04000000) != 0)	nOnCo1++;       //son D64H BD0 DI26 ADC_B_D10 
		if ((pDIO_D64.dwDIStatus & 0x08000000) != 0)	nOnCo1++;       //son D64H BD0 DI27 ADC_B_D11 
		if ((pDIO_D64.dwDIStatus & 0x10000000) != 0)	nOnCo1++;       //son D64H BD0 DI28 ADC_B_D12 
		if ((pDIO_D64.dwDIStatus & 0x20000000) != 0)	nOnCo1++;       //son D64H BD0 DI29 ADC_B_D13 
		if ((pDIO_D64.dwDIStatus & 0x40000000) != 0)	nOnCo1++;       //son D64H BD0 DI30 ADC_B_D14 
		if ((pDIO_D64.dwDIStatus & 0x80000000) != 0)	nOnCo1++;       //son D64H BD0 DI31 ADC_B_D15 
	}

	Ret = d64h_di_readport(pDIO_D64.m_bCardID2, &pDIO_D64.dwDIStatus);
	if (Ret != 0) {
		AfxMessageBox("  Card64H2_DI32() Error!!!");
	}

	else
	{
		if ((pDIO_D64.dwDIStatus & 0x00000001) != 0)	nOnCo1++;  //son D64H BD1 DI-00 SPARK_ON
		if ((pDIO_D64.dwDIStatus & 0x00000002) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00000004) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00000008) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00000010) != 0)	nOnCo1++;  //son D64H BD1 DI-04 DTHV   0: ERROR, 1: ����   
		if ((pDIO_D64.dwDIStatus & 0x00000020) != 0)	nOnCo1++;  //son D64H BD1 DI-05 DT24V  0: ERROR, 1: ���� 
		if ((pDIO_D64.dwDIStatus & 0x00000040) != 0)	nOnCo1++;  //son D64H BD1 DI-06 DT5V   0: ERROR, 1: ����
		if ((pDIO_D64.dwDIStatus & 0x00000080) != 0)	nOnCo1++;  //son D64H BD1 DI-07 DT12V  0: ERROR, 1: ���� 

		if ((pDIO_D64.dwDIStatus & 0x00000100) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00000200) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00000400) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00000800) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00001000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00002000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00004000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00008000) != 0)	nOnCo1++;

		if ((pDIO_D64.dwDIStatus & 0x00010000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00020000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00040000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00080000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00100000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00200000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x00400000) != 0)	nOnCo1++;   //son D64H BD1 DI-22 DT_BOT_BOX : �Ϻιڽ� Detection
		if ((pDIO_D64.dwDIStatus & 0x00800000) != 0)	nOnCo1++;   //son D64H BD1 DI-23 DT_TOP_BOX : ��ιڽ� Detection

		if ((pDIO_D64.dwDIStatus & 0x01000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x02000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x04000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x08000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x10000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x20000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x40000000) != 0)	nOnCo1++;
		if ((pDIO_D64.dwDIStatus & 0x80000000) != 0)	nOnCo1++;
	}


	if (nOnCo1 > 60) {//sylee170328
		nChk1 = 8;
	}

#endif  //#ifdef DEF_CARD_A1 


	if (nChk1 >= 7) {
		g_nErrCount1++;
		str1.Format(" ERROR! System Power off.    Please power on!", MB_OK);
		OnList1Mes1(str1);
		AfxMessageBox(str1);
	}

	if (g_nErrCount1 > 0) {
		goto  Task1;
	}

	//-------------------------------------------------------------------------------	

#ifdef DEF_CARD_A1  //sylee170328
	DioInpBit(pDIO.hDrv2, 32, &Buf);
#else  //#ifdef DEF_CARD_A1   

	//son D64H BD1 DI-07 DT12V  is ON?  (Detection Main Power 12V. 0 : ERROR, 1: ���� )
	if ((pDIO_D64.dwDIStatus & 0x000000080) != 0) {
		Buf = 1;  //son Main 12V ����
	}
	else {
		Buf = 0;  //son Main 12V Error
	}
#endif  //#ifdef DEF_CARD_A1 


	//son Main 12V Error ���
	if (Buf == 0) {
		g_nErrCount1++;
		if (nG_Language != 2) {//sylee150707
			str1.Format("  ERROR 7701 ::  ������ ����12V������ ���� �ֽ��ϴ�.���ּ���. MAIN POWER OFF - 12V  ", MB_OK);
		}
		else {
			str1.Format("  ERROR 7701.    BBT controller     MAIN POWER OFF - 12V    power turn on !  ", MB_OK);
		}
		//  str1.Format("  ERROR  MAIN POWER OFF - 12V  ", MB_OK ); 	
		OnList1Mes1(str1);
		AfxMessageBox(str1);
	}
	m_nProgSystem_Rate = 10;


#ifdef DEF_CARD_A1  //sylee170328
	DioInpBit(pDIO.hDrv2, 33, &Buf);
#else  //#ifdef DEF_CARD_A1   

	//son D64H BD1 DI-06 DT5V is ON?  0: ERROR, 1: ���� 
	if ((pDIO_D64.dwDIStatus & 0x00000040) != 0) {
		Buf = 1;   //son Main 5V ����
	}
	else {
		Buf = 0;   //son Main 5V Error
	}
#endif  //#ifdef DEF_CARD_A1 

	//son Main 5V Error���
	if (Buf == 0)
	{
		g_nErrCount1++;

		if (nG_Language != 2) {//sylee150707
			str1.Format(" ERROR 7701 ������ ����5V������ ���� �ֽ��ϴ�. ���ּ���. ", MB_OK);
		}
		else {
			str1.Format(" ERROR 7701 ::  MAIN POWER OFF - 5V    Please power on! ", MB_OK);
		}
		OnList1Mes1(str1);

		//str1.Format(" ERROR 7701 ::  ������ ����5V������ ���� �ֽ��ϴ�.���ּ���. MAIN POWER OFF - 5V  ", MB_OK ); 
		AfxMessageBox(str1);
	}
	m_nProgSystem_Rate = 20;


#ifdef DEF_CARD_A1  //sylee170328
	DioInpBit(pDIO.hDrv2, 34, &Buf);
#else  //#ifdef DEF_CARD_A1   

	//son D64H BD1 DI-05 DT24V is ON? 0: ERROR, 1: ����  
	if ((pDIO_D64.dwDIStatus & 0x00000020) != 0) {
		Buf = 1;  //son Main 24V ����
	}
	else {
		Buf = 0;  //son Main 24V Error
	}
#endif  //#ifdef DEF_CARD_A1 


	if (Buf == 0) {
		g_nErrCount1++;
		if (nG_Language != 2) {//sylee150707
			str1.Format(" ERROR 7701 ������ ���� 24V������ ���� �ֽ��ϴ�. ���ּ���. ", MB_OK);
		}
		else {
			str1.Format(" ERROR 7701 ::  MAIN POWER OFF - 24V.    Please power on! ", MB_OK);
		}
		OnList1Mes1(str1);
		AfxMessageBox(str1);
	}
	m_nProgSystem_Rate = 30;

	//===================================================================================			



//son CONTEC Switch ���� �϶�
#ifdef DEF_CARD_A1  //sylee170328		

//son230216_1 begin:
    //son   �Ʒ� �κ� Contec�������� box No�� 32K�� �̻��ϰ� ������ ��. 
	//	 CString str1;		
	int nBoxCount; 
	int nPinTotMax;     // 4: 4096,    8: 8193,   16:16384
	                    // 20: 20480,  24:24576,  32: 32768
	int nPinBoxMax, nStart, nEnd;

	if (SysSet12.m_nPinTotMax == PIN_TOT_4K) {
		nPinTotMax = 4;     //son 4/3 -> 1 box (4K box)
		                    //son 4/2 -> 2 box (2K box)
	}
	else  if (SysSet12.m_nPinTotMax == PIN_TOT_8K) {
		nPinTotMax = 8;     //son 8/3 -> 2 box (4K box)
		                    //son 8/2 -> 4 box (2K box)
	}
	else  if (SysSet12.m_nPinTotMax == PIN_TOT_16K)
	{
		nPinTotMax = 16;    //son 16/3 -> 5 box (4K box) ??? �̻���.
		                    //son 16/2 -> 8 box (2K box)
	}
	else  if (SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR //son-32KPIN-20191223: 20K �߰� //son230216_1
		|| PIN_TOT_20K_U12K_L4K_UP_TRANS		    //son210603
		|| PIN_TOT_20K_U8K_L4K_UP_TRANS)		    //son210826
	{
		nPinTotMax = 20;    //son 20/3 -> 6 box (4K box)
		                    //son 20/2 -> 10 box (2K box) ???
	}
	else  if (SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR	//son-32KPIN-20191223: 24K �߰�           //son230216_1
		|| SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS	    //son-32KPIN-20200120: 24K FullTrans �߰� //son230216_1
		|| SysSet12.m_nPinTotMax == PIN_TOT_16K_TRANS)      //son-32KPIN-20200228: 16K_FullTrans �߰� //son230216_1
	{
		nPinTotMax = 24;    //son 24/3 -> 8 box (4K box)	
		                    //son 24/2 -> 12 box (2K box)
	}
	else  if (SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS) { //son230216_1
		nPinTotMax = 32;    //son 32/3 -> 10 box (4K box) ??? �̻���.
	}
//son230216_1 end
	else  if (SysSet12.m_nPinTotMax < PIN_TOT_4K) {
		str1.Format("  ERROR 7702 :: [PIN]  MAX PIN <  List NO 1 (4k) ", MB_OK);
		OnList1Mes1(str1);
		AfxMessageBox(str1);
		nPinTotMax = 4;
	}
	else {
		str1.Format("  ERROR 7702 :: [PIN]  MAX PIN  >  List NO 4 (32k) ", MB_OK);
		OnList1Mes1(str1);
		AfxMessageBox(str1);
		nPinTotMax = 4;
	}

	if (SysSet12.m_nPinBoxMax == 1) {		//son BoxType: 1=2K
		nPinBoxMax = 2;
	}
	else  if (SysSet12.m_nPinBoxMax == 2) {	//son BoxType: 2=4K	
		nPinBoxMax = 3;
	}
	else  if (SysSet12.m_nPinBoxMax < 1) {
		str1.Format("  ERROR 7702 :: [PIN]  BOX PIN  <  List NO 1 (2k) ", MB_OK);
		OnList1Mes1(str1);
		AfxMessageBox(str1);
		nPinBoxMax = 2;
	}
	else {
		str1.Format("  ERROR 7702 :: [PIN]  BOX PIN  >  List NO 2 (4k) ", MB_OK);
		OnList1Mes1(str1);
		AfxMessageBox(str1);
		nPinBoxMax = 2;
	}

	nBoxCount = nPinTotMax / nPinBoxMax;

	if (nBoxCount == 1) {//1		
		nStart = 54, nEnd = 55;
	}
	else   if (nBoxCount == 2) {
		nStart = 52, nEnd = 55;
	}
	else   if (nBoxCount == 3) {
		nStart = 50, nEnd = 55;
	}
	else   if (nBoxCount == 4) {
		nStart = 48, nEnd = 55;
	}
	else { //8		
		nStart = 52, nEnd = 55;	//sylee151005  //ACE400	
	}

	for (i = nStart; i <= nEnd; i++)
	{
		if (nBoxCount == 1) {//1		
			//nStart=54,nEnd=55;
			if (i == 54) {
				m_nProgSystem_Rate = 70;
			}
			else  if (i == 55) {
				m_nProgSystem_Rate = 100;
			}
		}
		else if (nBoxCount == 2)
		{
			// nStart=52,nEnd=55;				
			if (i == 52) {
				m_nProgSystem_Rate = 45;
			}
			else  if (i == 53) {
				m_nProgSystem_Rate = 60;
			}
			else  if (i == 54) {
				m_nProgSystem_Rate = 80;
			}
			else  if (i == 55) {
				m_nProgSystem_Rate = 100;
			}
		}
		else   if (nBoxCount == 3) {
			//nStart=50,nEnd=55;
			if (i == 50) {
				m_nProgSystem_Rate = 40;
			}
			else  if (i == 51) {
				m_nProgSystem_Rate = 50;
			}
			else  if (i == 52) {
				m_nProgSystem_Rate = 60;
			}
			else  if (i == 53) {
				m_nProgSystem_Rate = 70;
			}
			else  if (i == 54) {
				m_nProgSystem_Rate = 80;
			}
			else  if (i == 55) {
				m_nProgSystem_Rate = 100;
			}
		}
		else   if (nBoxCount == 4) {
			// nStart=48,nEnd=55;
			if (i == 48) {
				m_nProgSystem_Rate = 40;
			}
			else  if (i == 49) {
				m_nProgSystem_Rate = 47;
			}
			else  if (i == 50) {
				m_nProgSystem_Rate = 55;
			}
			else  if (i == 51) {
				m_nProgSystem_Rate = 60;
			}
			else  if (i == 52) {
				m_nProgSystem_Rate = 70;
			}
			else  if (i == 53) {
				m_nProgSystem_Rate = 80;
			}
			else  if (i == 54) {
				m_nProgSystem_Rate = 90;
			}
			else  if (i == 55) {
				m_nProgSystem_Rate = 100;
			}
		}
		else { //8		
			// nStart=48,nEnd=63;	
		}

		DioOutByte(pDIO.hDrv, 13, 0);
		Sleep(50);

		//if(SysSet13.m_nPinBlockType==4 || SysSet13.m_nPinBlockType==6 ) 
		if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE 		//son230216_1
                || SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421) 	//son-32KPIN-20190902 
		{
			if (i == 52) {
				j2 = 48;//6-0
			}
			else  if (i == 53) {
				j2 = 49;//6-1
			}
			else  if (i == 54) {
				j2 = 52;//6-4
			}
			else  if (i == 55) {
				j2 = 53;//6-5
			}
			else {
				j2 = i;
			}
		}
		else {
			if (i == 55) {
				j2 = 55;
			}
			else  if (i == 54) {
				j2 = 51;
			}
			else  if (i == 53) {
				j2 = 54;
			}
			else  if (i == 52) {
				j2 = 50;
			}
			else  if (i == 51) {
				j2 = 53;
			}
			else  if (i == 50) {
				j2 = 49;
			}
			else  if (i == 49) {
				j2 = 52;
			}
			else  if (i == 48) {
				j2 = 48;
			}
		}

		DioInpBit(pDIO.hDrv2, j2, &Buf); //sylee151007 //ACE400 

		//if(SysSet13.m_nPinBlockType!=4) {//sylee161114-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA  
		if (SysSet13.m_nPinBlockType != PIN_BLK_S100_2_WIRE) //son-32KPIN-20190902 //son230216_1
		{
			if (Buf == 1) {		//sylee151005  //ACE400	
				if (i == 48) {	  //sylee151007 //ACE400			
					str1.Format("  ERROR 5001 DI=%d  ::  BOX D - PINBLOCK TOP - POWER OF, MB_OK ", j2);
				}
				else  if (i == 49) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX D - PINBLOCK BOTTON - POWER OFF, MB_OK ", j2);
				}
				else  if (i == 50) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX C - PINBLOCK TOP - POWER OF, MB_OK ", j2);
				}
				else  if (i == 51) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX C - PINBLOCK BOTTON - POWER OFF MB_OK ", j2);
				}
				else  if (i == 52) {	//sylee151007 //ACE400			
					str1.Format("  ERROR 5001 DI=%d  ::  BOX B - PINBLOCK TOP - POWER OF, MB_OK ", j2);
				}
				else  if (i == 53) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX B - PINBLOCK BOTTON - POWER OFF, MB_OK ", j2);
				}
				else  if (i == 54) {	//sylee151007 //ACE400			
					str1.Format("  ERROR 5001 DI=%d  ::  BOX A - PINBLOCK TOP - POWER OFF  , MB_OK ", j2);
				}
				else  if (i == 55) {	//sylee151007 //ACE400			
					str1.Format("  ERROR 5001 DI=%d  ::  BOX A - PINBLOCK BOTTON - POWER OFF, MB_OK ", j2);
				}
				g_nErrCount1++;
				OnList1Mes1(str1);
			}

		}
		else {
			//4=S100-2
			//skip SHIN BU SUNG 161114
			if (Buf != 1) {		//sylee151005  //ACE400							 
				if (i == 52) {	  //sylee151014-1 //ACE400			
					str1.Format("  ERROR 5001 DI=%d  ::  BOX B  VC- PINBLOCK TOP - POWER OF, MB_OK ", j2);
				}
				else  if (i == 53) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX B  VB- PINBLOCK TOP - POWER OFF, MB_OK ", j2);
				}
				else  if (i == 54) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX A  VC- PINBLOCK BOTTON - POWER OF, MB_OK ", j2);
				}
				else  if (i == 55) {//sylee151007 //ACE400				
					str1.Format("  ERROR 5001 DI=%d  ::  BOX A  VB- PINBLOCK BOTTON - POWER OFF MB_OK ", j2);
				}
				g_nErrCount1++;
				OnList1Mes1(str1);
			}
		}
		//	AfxMessageBox(str1);  							

	}

	//son ICP-DAS Switch ���� �϶�
#else  //#ifdef DEF_CARD_A1  //sylee170328 

	//sylee170411	Ret = d64h_do_writeport( pDIO_D64.m_bCardID2, 0x00000000 );  //
	//sylee170411	if( Ret!=0 ) { AfxMessageBox("  Card64H2_DO32(0x00000000 ) Error!!!");		}

	// son BD1(2��° D64H) _VC_ENA write
	Ret = d64h_do_writeport(pDIO_D64.m_bCardID2, 0x80000);  //
	if (Ret != 0) { AfxMessageBox("  Card64H2_DO32(0x80000 ) Error!!!");	return  0; }


	//son DT_BOT_BOX (D64H BD1 DI-22: �Ϻιڽ� Detection)
	if ((pDIO_D64.dwDIStatus & 0x00400000) == 0) {
		g_nErrCount1++;   //22
		str1.Format("  ERROR 5001  D64H BD1 DI-22  :: LOWER BOX DETECT ERROR! ");
		OnList1Mes1(str1);
	}

	//son DT_TOP_BOX (D64H BD1 DI-23: ��ιڽ� Detection)
	if ((pDIO_D64.dwDIStatus & 0x00800000) == 0) {
		g_nErrCount1++;   //23
		str1.Format("  ERROR 5001  D64H BD1 DI-23  :: UPPER BOX DETECT ERROR! ");
		OnList1Mes1(str1);
	}

#endif  //#ifdef DEF_CARD_A1 





Task1:

	if (g_nErrCount1 > 0) {
		str1.Format("NG");
		OnList1Mes1("      :: System Error!.  ");
	}
	else {
		str1.Format("OK");
		if (nDisplayDetail1 != 1) {//sylee141107
			OnList1Mes1("      :: System  OK.   ������ ��������.  ");
		}
	}
	m_Label_System.SetCaption(str1);
	m_nProgSystem_Rate = 100;

	return 1;


}


//son231222 changed to Get_nCardPinMax() in ChildView8  
#if 0

//--------------------------------------------------------------------------------
//son Switch ī�尡 64pin���� 128 pin������ ���� ȯ�溯���� ���� 
//    nHW_CardPin(0,1 =128   2=64pin), nCardPinMax (64 �Ǵ� 128) ���� �����Ѵ�. 
//--------------------------------------------------------------------------------
//son �ٸ� ���̾�α��� ChildView8���� ������  ������ nHW_CardPin, nCardPinMax�� 
//son ���� SysSet12.m_nPinBoxMax�� SysSet13.m_nPinBlockType �� �°� �ٽ� ����. 
int  CChildView5::nHW_CardPin_Check()//sylee150903 //ACE400
{
	if (SysSet12.m_nPinBoxMax == 1) {//sylee150903  2k 
		nHW_CardPin = 2;//sylee150903-1  //ACE400  2=64pin   0,1 =128
		nCardPinMax = 64;//sylee150903-1 //ACE400//ACE380
	}
	else {	// 4K
		nHW_CardPin = 0;//sylee150903-1  //ACE400  2=64pin   0,1 =128
		nCardPinMax = 128;//sylee150903-1 //ACE400//ACE380
	}

	//sylee161115 //sylee171109-1  //5=S100-2  SDBLOCK //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
	//if( SysSet13.m_nPinBlockType==4 || SysSet13.m_nPinBlockType==6 ) {
	if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE 			//son-32KPIN-20190902 //son230216_1
		|| SysSet13.m_nPinBlockType == PIN_BLK_4K_DD_421)
	{
		nHW_CardPin = 0;//sylee150903-1  //ACE400  2=64pin   0,1 =128
		nCardPinMax = 128;//sylee150903-1 //ACE400//ACE380
	}
	return 1;
}
#endif


///sylee160520-1



void CChildView5::OnAuto_Self1_Init()   ///sylee160520-1
{


	int nRet;
	//int m_nIGain;		//son-32KPIN-20190902: CildView5 ������� m_m_nIGain���� ����

	Get_nCardPinMax();//sylee150903 //ACE400
	FileSysInfo01.LoadSaveSet15(2);//load   //sylee140911
	Get_nCardPinMax(); //sylee150903 //ACE400

	g_nMode1 = 181;  //lv 10v   //sylee130701

	nRet = AutoSelf_PowerModeSet(g_nMode1);

	m_nIGain = (int)dRec[g_nMode1][12];	    //sylee150904 //ACE400

	Sub71.I_Gain(m_nIGain);//sylee220924 

	//task2:
	//	DoEvents();


	Sub71.OnPinAllReset1(); ///PIN ALL RESET

	//Sub71.AOnPinSet1(1, 1, 2);   
	//Sub71.AOnPinSet1(1, 1, 1);   
	Sub71.AOnPinSet1(1, 1, MOD_TR_SET_VC);  //son210220 2->1 enum���� ����   
	Sub71.AOnPinSet1(1, 1, MOD_TR_SET_VB);  //son210220 1->2 enum���� ���� 

	Sub71.Delay_100ns(1000);

	Sub71.AOnReadV1I1(m_nVRel);
	Sub71.OnPinAllReset1(); ///PIN ALL RESET



	Sub71.AOnPoweroff2();//sylee20120702  Sub71.AOnPoweroff();

	Sub71.FilterSet(0);//sylee130704 
	Sub71.I_Filter(0);//sylee130704


	Sub71.OnPinResetInit();//sylee151221   //ACE400

	return;
}








//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//sylee240509

extern int nFlagLeakMode1, nFlagLeakType1;//sylee230615
extern int nLeaKADC1Enable[20];//sylee230615-1 
extern int nLeaKADC1Flag1, nLeaKADC1Type1, nLeaKADC1Step1, nLeaKADC1Enable1,nLeaKADC1nMod1;//sylee230615-1 
//    m_edit101.GetWindowText( szText,10); 
//	nManuaOpenPin1 = atoi( szText );

int CChildView5::OnManualShort1_LV() //sylee240509
{
	CString   str,str1,strfName,strfName1;
	char  fName[200], fName1[200]; 
    int i, ik,j , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nECount,nEtemp1,nShort2FailFlag1;
 	int nEt1,nFailL1CoPiece,nCDelay1,nFil,nDisChargeADC1;
	int  nDischargeCount ;
	int nShRes1T, ih,logno1,nSQCount1, nSQLastY1, nSQLastPin, tp1 ;
	double nISet,dVbR1,dVbR2,InterI;	
	int nFailFlag1,nFailFlag2,nFailFlag3,nFailFlag4;  
	int nSQ1Fail[200]; 
	int nSh1Ret1[100],dPinVC[200],dPinVB[200],nSQNet1[200][200];// pin array//sylee1209
	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200]; 
 		
	int nMSet1Flag,nMSet1Co, nMSet1CoT2;
	int nSQStep1flag,nIControl,nRetry1,nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR , m_nMode1,nMod1;
	double nProRv1;  
	int nStop2,nPinBlockEnd;//sylee240509
 	int nIGain;//sylee240517
	if(	nLeakRunHV==1){
		return 0;//sylee240530
	}

	nLeakRunLV=1;//sylee240530

	m_nProg2wLeak_Rate=0;//sylee240509
	g_nStopFlag=0;//sylee240509 
	str.Format(" RUN  ");//sylee240509			 
    m_Label_2wLeak.SetCaption(str);//sylee240509
  	nlist1=0; 
    nFailL1Co=0;//sylee140110    
  
    int k1;
 
    k1=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
    if( k1!=1){
        str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
        m_list1.AddString(str); 
        nlist1++;
        return 0;
    }

  
    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;

    str.Format("  ***> Manual LV Short :  START  *******  ");			 
    m_list1.AddString(str); 
    nlist1++;

    DoEvents();  
  
    int nRet;

    nMod1=1;
    m_nMode1=207;
    Sub71.R1=3000000; 

    nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412

    if( nRet!=1){
        str.Format("ERRor NO 3002,   \n  HR Short Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
        AfxMessageBox( str);
        return 0; 
    }

    RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW, PART_UPPER );   // m_nMode1 1~76  //son220412_3 
    nProRv1=Sub71.R1;

    if(nProRv1<250){
        nProRv1=250;
    }

    if(nProRv1>1000){
        nProRv1=1000;
    }

    //////////////////////////////////////
    //nProRv1=400;//raw

    logno1=0;    nShRes1T=0;	nStop=0;
    ipFlag=0;	 n4Wire=0;	    ipEnd=1; 
    nCDelay1=100;
    nDisChargeADC1=10;//testmode
    nDischargeCount=20;//sylee20120830

    nHVIADCFlag=1;//sylee121018   //waiting 
    nIControl=0;

     ::ZeroMemory (&nData, sizeof(nData));
    ::ZeroMemory (&dR1, sizeof(dR1));
    ::ZeroMemory (&dVB1, sizeof(dVB1));
    ::ZeroMemory (&dVC1, sizeof(dVC1));
    ::ZeroMemory (&dI1, sizeof(dI1));
    ::ZeroMemory (&nT, sizeof(nT));
    ::ZeroMemory (&nData, sizeof(nData));
    ::ZeroMemory (&dPinVC, sizeof(dPinVC));
    ::ZeroMemory (&dPinVB, sizeof(dPinVB));	
    ::ZeroMemory (&fName, sizeof(fName));
    ::ZeroMemory (&fName1, sizeof(fName1));
    ::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
    ::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907 

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //mode ( 1~65)
    //step ( open, ��ushort, short, hr, hv  4w) 	

    nCC= (int) dRec[m_nMode1][1];
    nVRel= (int)dRec[m_nMode1][2];
    nVSet= (int) dRec[m_nMode1][3];
    nIR= (int)dRec[m_nMode1][4];
    nISet= dRec[m_nMode1][5];
    nDelay10= (int)dRec[m_nMode1][6]*30;
    nFil=(int)dRec[m_nMode1][8];
    nIFilter = (int)dRec[m_nMode1][9];   //sylee130629		  
    nHVOffSet =(int) dRec[m_nMode1][10]; //sylee130629
    nHVGain = (int)dRec[m_nMode1][11];	//sylee130629
    nIGain = (int)dRec[m_nMode1][12];   //sylee230517     


    if( nISet<0.1){
        AfxMessageBox(" Error No 7713, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
    }
    //ADC COUNT
    Sub71.nADCount1 = (int)dRec[m_nMode1][7];

    if(nIR==5){//uA
        InterI=	10.0;
    }else{
        InterI=nISet*0.9;
    }

    //=========================================================================================
    //interlock    

    if( nCC!=1 && nCC!=2 ){
        AfxMessageBox(" Error No 1024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
       return 0;
    }

    if( nVSet<1 || nVSet>300 ){
        AfxMessageBox(" Error No 1025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        return 0;
    }

    if( nVRel<1  || nVRel>4 ){ 
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        return 0; 
    }


    Sub71.AOnRelayOff1();//sylee240509	
	Sub71.OnDisCharge1(); //sylee240509	
    nRet=Sub71.AOnPoweroff2(); //sylee230615
 	if(nRet!=1){
		AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
		return  0;
	} 

	if( nVRel==1){//sylee230615
		nRet=Sub71.AOnVSetMulti1(nVSet,0,0,0) ;
 	}else  if( nVRel==2){//sylee230615
		nRet=Sub71.AOnVSetMulti1(0,nVSet,0,0) ;
	}  
 //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 //������ 

    Lo1=1000;//sylee150708
    Sub71.AOnReadVB1Run1(nVRel);//sylee130802
 

Task101:    

    Sub71.AOnReadVB1Run1(nVRel); 

    if(nVSet<=5){
        dVbR2=nVSet*(0.35);//10%
    }else{
        dVbR2=nVSet*(0.1);//5%
    }
    dVbR1=nVSet-dVbR2;
    dVbR2=nVSet+dVbR2;

    if( Sub71.V1>300){
        if(Lo1-->0){					 					
            //sylee121019	Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(30000);  //1ms   //sylee150708  1->3ms 
            goto Task101;
        }
        str.Format("Error No 1301,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str);	
 
        return 0;
    }

    if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
        if(Lo1-->0){
            //sylee121019	Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(10000);  //1ms   
            goto Task101;
        }			
        str.Format("Error No 1317,\n \n\n Voltage set error.    Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str);						  
         return 0;
    }
 
    Sub71.AOnReadV1Enable1(nVRel); //sylee20120223   //VC SENSE INIT SET 
    Sub71.FilterSet(nFil);//sylee130702 
    if(nIFilter==3){
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
        nIFilter=1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629
	
    if (nIGain == 2){ //sylee240517    
       Sub71.I_Gain(2);  //10x  =io on        
    } else {
        Sub71.I_Gain(1);  //1x  =io off
    }

    Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//sylee20111213     //vb RELAY ON
    Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118

    //--------------------------------------------------------------------------------     
    ipEnd=1;
    nEnd=0;

    if( nVSet>20){
        nLoop=1;
        str.Format("Error No 1318,\n \n\n  MANUAL SHORT START ERROR    VSET >20V !!    "); 
        AfxMessageBox(str);
         return 0;
    }else{
        nLoop=0;
    }

    FileSysInfo01.LoadSaveSub15(2);//sylee140110
    /////////////////////////////////////////////////////////////////////

    ipFlag=2;
    ipEnd=1;
    nStop2=0;
	ipEnd=1;//sylee240509 
	
	nPinBlockEnd=SysSet13.m_nSet105+1; //4=16k,8=32k//sylee240509
	if(nPinBlockEnd<0){nPinBlockEnd=1;}//sylee240509
 	if(nPinBlockEnd>16){nPinBlockEnd=16;}//sylee240509	



	
    for( int nLoop2=1; nLoop2<=nPinBlockEnd; nLoop2++ ) //sylee240509
    {
        m_nProg2wLeak_Rate=(int)(((double)nLoop2/ (double)nPinBlockEnd)*100.0);//sylee240509
        if( nFailL1Co>=SysInfo05.m_nIsEr){	
            str.Format("           : block=%d  Start Stop  : Error Limit    ", nLoop2);          									 
        }else{
            str.Format("           : block=%d  Start  ",nLoop2);		
        }
        m_list1.AddString(str); 
        nlist1++;

    //====================================================================================================================================
        for( int ip=1; ip<=1; ip++ )
        { 

            nEtemp1=0;	nECount=0; 	nFailL1CoPiece=0;
            nEt1=0;	nShort2FailFlag1=0;
            nSQCount1=0;//sylee120908   //SQRT( SQUARE ROOT)
            nRCount=4096;
            for( i=1; i<200;i++){
                tp1=i*i;
                if(tp1>=nRCount){
                    nSQCount1=i;//sylee120908
                    break;
                }
            }

            if( (nSQCount1==0)  || (nRCount<1)){//sylee121018
                str.Format("Error No 7513-6 ,   SET ERORR   Net List <1,  ,    piece pin no set check?   HV SQUARE  COUNT = ZERO   n"   ); 
                AfxMessageBox(str); 
                return 0;
            }

            ::ZeroMemory(&nSQNet1, sizeof(nSQNet1));

            //################################################################################################################################
            //SET 

            for( ik=1; ik<=4096; ik++){
                i=(ik-1)/nSQCount1  + 1;//mok
                j=(ik-1)-nSQCount1*(i-1);//na	 
                j=j+1;	 
                nSQNet1[i][j]=ik-1+(nLoop2-1)*4096;//sylee240509

                if( ik==4096){//sylee240509
                    nSQLastPin=j;//sylee120908   ///last row , last col.
                    nSQLastY1=i;
                }
            }

            //################################################################################################################################
            //#1 FIRST-1  Line&Block
            //row

            ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));	
            nShort2FailFlag1=0;
            nSQStep1flag=0;
            Sub71.OnPinAllReset1(); ///PIN ALL RESET  	 
            nRetry1=0; 
            //	nFlag_TOP_I=1;//sylee230302


            //Task1Retry:

            for( ik=1; ik<nSQLastY1; ik++)     // FIRST LOOP	 //Y  VB 
            {

                DoEvents(); 

                if(g_nStopFlag==2){nStop2=2;}//sylee240510

                if( nStop2==2){
                    str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");			 		
                    m_list1.AddString(str); 
                    nlist1++;
                    goto Task211;
                }

                if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
                    break;// complete  error 												 
                }

                if(ik==1  || nSQStep1flag==2){

                    Sub71.OnPinAllReset1(); ///PIN ALL RESET 

                    for( int ik2=1; ik2<=nSQLastY1; ik2++){
                        if(ik2>ik){
                            continue;
                        }
                        for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                            if( ik2==nSQLastY1){  //LAST Y STEP
                                if(ih>nSQLastPin){
                                    break;//SKIP   
                                }
                            }
                            //Sub71.AOnPinSet1(1, nSQNet1[ik2][ih], 1); //=1Vb,    
                            Sub71.AOnPinSet1(1, nSQNet1[ik2][ih], MOD_TR_SET_VB); //=1Vb,    //son210220 enum���� ����
                        } 
                    }

                    nSQStep1flag=0;


                }else{//end of if(ik==1  || nSQStep1flag==2){			   
                    Sub71.OnPinResetVC();  
                    for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                        if( ik==nSQLastY1){  //LAST Y STEP
                            if(ih>nSQLastPin){
                                break;//SKIP   
                            }
                        }				   
                        //Sub71.AOnPinSet1(1, nSQNet1[ik][ih], 1); //=1  vb 
                        Sub71.AOnPinSet1(1, nSQNet1[ik][ih], MOD_TR_SET_VB); //=1  vb //son210220 enum���� ����
                    }
                }

                for( ih=1; ih<=nSQCount1; ih++){  //X   // Second LOOP   vb pin  
                    if( (ik+1)==nSQLastY1){
                        if(ih>nSQLastPin){
                            break;//SKIP
                        }
                    } 			   
                    //Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih], 2);   // =2,Vc,   all	
                    Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih], MOD_TR_SET_VC);   // =2,Vc,   all	//son210220 enum���� ����
                }
                //////////////////////////////////////////////////////////////////////////////////


                dView7.ALV_ADCI_Read1_OneToM(nProRv1, nDelay10); 



                if( Sub71.R1<=nProRv1){// ok//HV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1; 	   		 
                    nSQStep1flag=2;
                }


                //==============================================================================================================================
                //################################################################################################################################
                //#2,  second search    hv  adc  
                // vc  m block 

                DoEvents(); 

                if(nFailFlag1==1)  //if error	 
                {
                    DoEvents(); 
                    if( nStop2==2){
                        str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                        m_list1.AddString(str); 
                        nlist1++;
                        goto Task211;
                    }

                    Sub71.OnPinResetVB();   //sylee121029  

                    for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc				
                    {
                        nMSet1Co=0;
                        nMSet1Flag=0;
                        nMSet1CoT2=0;

                        if(ik2>ik){ //doublecheck#2 
                            continue;
                        }

                        for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                            if(ik2==nSQLastY1){  //LAST Y STEP
                                if(ih>nSQLastPin){
                                    break;//SKIP   
                                }
                            }					   
                            //Sub71.AOnPinSet1(1, nSQNet1[ik2][ih], 1); //=2VC,	 
                            Sub71.AOnPinSet1(1, nSQNet1[ik2][ih], MOD_TR_SET_VB); //=2VC,	 //son210220 enum���� ����
                        } 
                        //=====================================================================================================================

                        // dView7.AHV_ADCI_Read1(nProRv1); 
                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                        //  Sub71.R1= nIADC1  ;//ADC
                        //Sub71.Delay_100ns(10000);  
                        //=====================================================================================================================
                        nFailFlag2=0;

                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag2=1;
                        }


                        //=====================================================================================================================
                        //##################################################################################################################################

                        //one line & one line  search


                        if(nFailFlag2==1)
                        {  
                            DoEvents(); 
                            if( nStop2==2){
                                str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                                m_list1.AddString(str); 
                                nlist1++;
                                goto Task211;					

                            }

                            for( int ih6=1; ih6<=nSQCount1; ih6++)    //X // Second LOOP   vc pin 
                            {
                                if( ik2==nSQLastY1){  //LAST Y STEP
                                    if(ih6>nSQLastPin){
                                        break;//SKIP   
                                    }
                                }

                                Sub71.OnPinResetVB();//sylee121016***** all reset								    
                                // Sub71.Delay_100ns(100);//10uSEC
                                //Sub71.AOnPinSet1(1, nSQNet1[ik2][ih6], 1); //=2VC,	 
                                Sub71.AOnPinSet1(1, nSQNet1[ik2][ih6], MOD_TR_SET_VB); //=2VC,	 //son210220 enum���� ����

                                //===================================================================================================================== 

                                dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 
                                //  Sub71.R1= nIADC1  ;//ADC                        

                                nFailFlag3=0;                          									    

                                if( Sub71.R1<=nProRv1){// ok//HV
                                }else{//fail
                                    nFailFlag3=1;
                                }

                                //=====================================================================================================================
                                //################################################################################################################################
                                ///one point  & one point search

                                if(nFailFlag3==1)
                                { 
                                    DoEvents(); 
                                    if( nStop2==2){
                                        str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                                        m_list1.AddString(str); 
                                        nlist1++;
                                        goto Task211;					

                                    } 
                                    for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  
                                    {

                                        if( (ik+1)==nSQLastY1){
                                            if(ih2>nSQLastPin){
                                                break;//SKIP
                                            }
                                        }

                                        Sub71.OnPinResetVC(); //Vb all reset
                                        // Sub71.Delay_100ns(100);//10uSEC   //sylee121017

                                        //Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all
                                        Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih2], MOD_TR_SET_VC);   // =1,VB,   all //son210220 enum���� ����

                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10);   //  Sub71.R1= nIADC1  ;//ADC


                                        nFailFlag4=0;

                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        }else{//fail
                                            // 	if(ih2==ih6)
                                            nFailFlag4=1;														 

                                            for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                                if( (nFailL1[iq1][4]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][5]==(nSQNet1[ik2][ih6]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                                if( (nFailL1[iq1][5]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][4]==(nSQNet1[ik2][ih6]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                            }
                                        }


                                        //##########################################################################################################################################3


                                        if(nFailFlag4==1){//sylee130111 


                                            if(nFailL1Co<3000){//array size nFailL1[

                                                nFailCount[nMod1]=nFailCount[nMod1]++; 
                                                nFailL1Co++;

                                                nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV
                                                if( nSQNet1[ik+1][ih2]<nSQNet1[ik2][ih6]){
                                                    nFailL1[nFailL1Co][5]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                                    nFailL1[nFailL1Co][4]=nSQNet1[ik2][ih6]+1;//pin  vc
                                                }else{
                                                    nFailL1[nFailL1Co][4]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                                    nFailL1[nFailL1Co][5]=nSQNet1[ik2][ih6]+1;//pin  vc
                                                }  																		 

                                            }   //end of if(nFailL1Co<3000){//array size nFailL1[


                                            Sub71.Delay_100ns(3000);//1mSEC   

                                            if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
                                                break;// complete  error 
                                            }	 


                                        }   //if(nFailFlag4==1) 

                                    } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  


                                    //----------------------------------------------------------------------------------------------------------------------

                                    Sub71.OnPinResetVC(); 

                                    for(  int ih3=1; ih3<=nSQCount1; ih3++){  //X   // Second LOOP   vb pin  
                                        if( (ik+1)==nSQLastY1){
                                            if(ih3>nSQLastPin){
                                                break;//SKIP
                                            }
                                        }
                                        //Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih3], 2);   // =1,VB,   all
                                        Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih3], MOD_TR_SET_VC);   // =1,VB,   all
                                    } 

                                }// if(nFailFlag3==1) 

                                if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                                    break;// complete  error 
                                }

                                //################################################################################################################################


                            }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                            //=====================================================================================================================
                        }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                        if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                            break;// complete  error 
                        } 


                        //#2
                    }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



                }//end of  if(nFailFlag1==1)

                if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                    break;// complete  error 
                } 

            }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB


            //==============================================================================================================================
            //################################################################################################################################

            Sub71.OnPinAllReset1(); ///PIN ALL RESET 



            //================================================================================================================
            //################################################################################################################################
            //#1 FIRST-1  Line&Block
            // Y 

            ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));
            nShort2FailFlag1=0;
            nSQStep1flag=0;
            nRetry1=0;	

            //Task2Retry:


            for( ik=1; ik<nSQCount1; ik++)     // FIRST LOOP //Y  VB 
            {
                DoEvents(); 

                if(g_nStopFlag==2){nStop2=2;}//sylee240510

                if( nStop2==2){
                    str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                    m_list1.AddString(str); 
                    nlist1++;
                    goto Task211;	
                }

                if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
                    break;// complete  error 												 
                }

                if(ik==1  || nSQStep1flag==2){

                    Sub71.OnPinAllReset1(); ///PIN ALL RESET 			   

                    for( int ik2=1; ik2<=nSQCount1; ik2++){   

                        if(ik2>ik){
                            continue;
                        }
                        for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                            if( ih==nSQLastY1){  //LAST Y STEP
                                if(ik2>nSQLastPin){
                                    break;//SKIP   
                                }
                            }
                            //Sub71.AOnPinSet1(1, nSQNet1[ih][ik2], 1); //=2VC,   
                            Sub71.AOnPinSet1(1, nSQNet1[ih][ik2], 1); //=2VC,   
                        } 
                    }

                    nSQStep1flag=0;

                }else{//end of if(ik==1  || nSQStep1flag==2)


                    Sub71.OnPinResetVC(); 
                    //   Sub71.Delay_100ns(100);//10uSEC  

                    for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                        if( ih==nSQLastY1){  //LAST Y STEP
                            if(ik>nSQLastPin){
                                break;//SKIP   
                            }
                        }
                        Sub71.AOnPinSet1(1, nSQNet1[ih][ik], 1); //=2VC,  
                    } 	
                }



                for( ih=1; ih<=nSQLastY1; ih++){  //X   // Second LOOP   vb pin  
                    if( ih==nSQLastY1){
                        if((ik+1)>nSQLastPin){
                            break;//SKIP
                        }
                    }

                    Sub71.AOnPinSet1(1, nSQNet1[ih][ik+1], 2);   // =1,VB,   all		   

                }
                //////////////////////////////////////////////////////////////////////////////////


                dView7.ALV_ADCI_Read1_OneToM(nProRv1, nDelay10); 

                if( Sub71.R1<=nProRv1){// ok//HV
                    nSQ1Fail[ik]=1;
                    nFailFlag1=0;
                }else{//fail
                    nSQ1Fail[ik]=2;  // LAST ik=0;
                    nShort2FailFlag1=1;
                    nFailFlag1=1; 
                    nSQStep1flag=2; 
                }	 

                // nFailFlag1=0;  // ALLPASS
                //==============================================================================================================================
                //################################################################################################################################
                //#2,  second search    hv  adc  
                // vc  m block 

                if(nFailFlag1==1)  //if error     
                {
                    DoEvents(); 
                    if( nStop2==2){
                        str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                        m_list1.AddString(str); 
                        nlist1++;
                        goto Task211;					

                    }

                    Sub71.OnPinResetVB();//sylee121029          


                    for( int ik2=1; ik2<=nSQCount1; ik2++)     // FIRST LOOP //Y   //Vc
                    {
                        nMSet1Co=0;
                        nMSet1Flag=0;
                        nMSet1CoT2=0;				 


                        if(ik2>ik){ //doublecheck#2 
                            continue;
                        }

                        for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
                            if(ih==nSQLastY1){  //LAST Y STEP
                                if(ik2>nSQLastPin){
                                    break;//SKIP   
                                }
                            }

                            Sub71.AOnPinSet1(1, nSQNet1[ih][ik2], 1); //=2VC, 			 
                        } 

                        //===================================================================================================================== 
                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10); 					 
                        //Sub71.Delay_100ns(10000);  
                        //=====================================================================================================================
                        nFailFlag2=0;

                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag2=1;
                        }

                        //=====================================================================================================================
                        //##################################################################################################################################

                        //one line & one line  search


                        if(nFailFlag2==1)
                        {  
                            DoEvents(); 
                            if( nStop2==2){
                                str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                                m_list1.AddString(str); 
                                nlist1++;
                                goto Task211;					

                            }

                            for( int ih6=1; ih6<=nSQCount1; ih6++){    //X // Second LOOP   vc pin 

                                if( ik2==nSQLastY1){  //LAST Y STEP
                                    if(ih6>nSQLastPin){
                                        break;//SKIP   
                                    }
                                }

                                Sub71.OnPinResetVB();//sylee121016***** all reset								    
                                //  Sub71.Delay_100ns(100);//10uSEC   

                                Sub71.AOnPinSet1(1, nSQNet1[ih6][ik2], 1); //=2VC,								   
                                //=====================================================================================================================								 
                                dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10);  


                                nFailFlag3=0;

                                if( Sub71.R1<=nProRv1){// ok//HV
                                }else{//fail
                                    nFailFlag3=1;
                                }

                                //=====================================================================================================================
                                //################################################################################################################################
                                ///one point  & one point search

                                if(nFailFlag3==1)
                                { 	
                                    DoEvents(); 
                                    if( nStop2==2){
                                        str.Format(" *****  STOP :  2W Tr check  LV SHORT *******");					 		
                                        m_list1.AddString(str); 
                                        nlist1++;
                                        goto Task211;					

                                    }

                                    for( int ih2=1; ih2<=nSQLastY1; ih2++)  //X   // Second LOOP   vb pin  											
                                    {
                                        if( ih2==nSQLastY1){
                                            if((ik+1)>nSQLastPin){
                                                break;//SKIP
                                            }
                                        }

                                        if( ih6 != ih2){//sylee121025
                                            continue;
                                        }

                                        Sub71.OnPinResetVC(); //Vb all reset
                                        // Sub71.Delay_100ns(100);//10uSEC   //sylee121017

                                        Sub71.AOnPinSet1(1, nSQNet1[ih2][ik+1], 2);   // =1,VB,   all

                                        dView7.ALV_ADCI_Read1_OneToM(nProRv1,nDelay10);   

                                        nFailFlag4=0;

                                        if( Sub71.R1<=nProRv1){// ok//HV
                                        }else{  

                                            nFailFlag4=1;

                                            for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                                if( (nFailL1[iq1][4]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][5]==(nSQNet1[ih6][ik2]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                                if( (nFailL1[iq1][5]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][4]==(nSQNet1[ih6][ik2]+1))){
                                                    nFailFlag4=0; break;
                                                }
                                            }

                                        }								 


                                        //##########################################################################################################################################3

                                        if(nFailFlag4==1){//sylee130111 														 								

                                            if(nFailL1Co<3000){//array size nFailL1[

                                                nFailCount[nMod1]=nFailCount[nMod1]++;
                                                //log buffer
                                                nFailL1Co++;																			 
                                                nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV

                                                if( nSQNet1[ih2][ik+1]<nSQNet1[ih6][ik2]){
                                                    nFailL1[nFailL1Co][5]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
                                                    nFailL1[nFailL1Co][4]=nSQNet1[ih6][ik2]+1;//pin  vc
                                                }else{
                                                    nFailL1[nFailL1Co][4]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
                                                    nFailL1[nFailL1Co][5]=nSQNet1[ih6][ik2]+1;//pin  vc
                                                }
                                                nFailL1[nFailL1Co][6] = Sub71.R1; //sylee230615-2  

                                            }   //end of if(nFailL1Co<3000){//array size nFailL1[


                                            Sub71.Delay_100ns(3000);//1mSEC   

                                            if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
                                                break;// complete  error 
                                            }	



                                        }   //if(nFailFlag4==1) 

                                    } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  

                                    //----------------------------------------------------------------------------------------------------------------------


                                    Sub71.OnPinResetVC(); 							 

                                    for(  int ih3=1; ih3<=nSQLastY1; ih3++){  //X   // Second LOOP   vb pin  								 

                                        if( ih3==nSQLastY1){
                                            if((ik+1)>nSQLastPin){
                                                break;//SKIP
                                            }
                                        }
                                        Sub71.AOnPinSet1(1, nSQNet1[ih3][ik+1], 2);   // =1,VB,   all

                                    } 


                                }// if(nFailFlag3==1) 



                                if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                                    break;// complete  error 
                                }


                                //################################################################################################################################


                            }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                            //=====================================================================================================================
                        }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                        if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                            break;// complete  error 
                        } 


                        //#2
                    }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



                }//end of  if(nFailFlag1==1)


                if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                    break;// complete  error 
                } 


            }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB

        }//end of  for ( ip

    //	str.Format("           : block=%d End  ",nLoop2);	
        if( nFailL1Co>=SysInfo05.m_nIsEr){	
            str.Format("           : block=%d  End Stop  :  Error Limit  ", nLoop2);          									 
        }else{
            str.Format("           : block=%d  End  ",nLoop2);		
        } 
     
        m_list1.AddString(str); 
        nlist1++;

    }  // for( int nLoop2=1; nLoop2<=ipEnd; nLoop2++ ) //sylee240509

//==============================================================================================================================
//################################################################################################################################


Task211:					

    
    if( nFailL1Co>0){//sylee150811 
        nShortCount=nFailL1Co;//sylee150811 
    }

    Sub71.OnPinAllReset1(); ///PIN ALL RESET 
    Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
    Sub71.AOnRelayOffSel(nVRel);//sylee20120720
    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704
    Sub71.AOnReadV1Enable1(5); //sylee130711

    if(SysInfo05.m_nIsEr>1){//sylee140110
        if( nFailL1Co==SysInfo05.m_nIsEr){
            str.Format("  ***>   2W LV Short Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nIsEr );			 		
            m_list1.AddString(str); 
            nlist1++;
        }
    }


    int nStep, nRMode1;			

    nStep=0;//sylee230616

    for( i=1 ; i<=nFailL1Co ; i++)//sylee230616
    {  
        nStep++;//sylee230616

        Sub71.R1=nFailL1[i][6];//sylee240517
        if(Sub71.R1>=65000){   nRMode1=1;//sylee240517
        }else{  nRMode1=0;		}

        RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL , PART_UPPER ); //sylee230616
        if (Sub71.R1 < 0) {			Sub71.R1 = 0.1;		}  

        if(nRMode1==0)
        {
            if( Sub71.R1 < 1000.0){
                str.Format("   --> Short No=%d,   pin %5.0f - %5.0f   ,   R = %10.2f ohm ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1 );	//sylee230615	
            }else if( Sub71.R1 < 1000000.0){
                str.Format("   --> Short No=%d,   pin %5.0f - %5.0f   ,   R = %10.2f K ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000. );	//sylee230615	
            }else {
                str.Format("   --> Short No=%d,   pin %5.0f - %5.0f   ,   R = %10.2f M ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000000. );	//sylee230615	
            }
        }else{
            if( Sub71.R1 < 1000.0){
                str.Format("   --> Short No=%d,   pin %5.0f - %5.0f   ,   R < %10.2f ohm ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1 );	//sylee230615	
            }else if( Sub71.R1 < 1000000.0){
                str.Format("   --> Short No=%d,   pin %5.0f - %5.0f   ,   R < %10.2f K ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000. );	//sylee230615	
            }else {
                str.Format("   --> Short No=%d,   pin %5.0f - %5.0f   ,   R < %10.2f M ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000000. );	//sylee230615	
            }
        }
        m_list1.AddString(str); 			nlist1++;
    }

    if( nFailL1Co <=0 ){ //sylee231215
        if( nStop2!=2){
            str.Format("  ***>    ALL  PASS .   ");
            m_list1.AddString(str);  nlist1++;   //sylee23061
        }
    }
    else{
        str.Format("   ===>   Result : [ Error ] => %d  ", nFailL1Co ); //sylee231215	 
        m_list1.AddString(str);  nlist1++;   //sylee23061
    }

    if( nStop2==2){
        str.Format("  ***>     STOP.  *******  ");			 
    }else{
        str.Format("  ***>    FINISH.  *******  ");	 
    }
    m_list1.AddString(str); 		nlist1++; 

    //::ShellExecute(NULL,"close","EXCEL.EXE","d:\\LOG\\PRO1\\HVLEAK1.CSV","NULL",SW_SHOWNORMAL);

    Sub71.OnPinAllReset1(); ///PIN ALL RESET 
    Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
    Sub71.AOnRelayOffSel(nVRel);//sylee20120720
    Sub71.FilterSet(0);//sylee130704 
    Sub71.I_Filter(0);//sylee130704 
    Sub71.AOnReadV1Enable1(5); //sylee130711 
    Sub71.AOnPoweroff2();//sylee170411
    Sub71.AOnRelayOff1();//sylee210830
    nLeakRunLV=0;//sylee240530 

    return 1;

}  




//////////////////////////////////////////////////////////////////
//sylee240509


 
extern double nProStep1[10][20];//sylee240110;

int CChildView5::OnManualShort1_HV2() //sylee230624-1
{
   
	CString   str,str1;
	CString   strfName,strfName1;

	char  fName[200], fName1[200]; 
    int i, ik,j , n4Wire, nRCount,nDelay10,nEnd;	
	int nLoop, ipEnd, ipFlag,Lo1,nECount,nEtemp1,nShort2FailFlag1;
 	int nEt1,nFailL1CoPiece,nCDelay1;
	int nFil,nDisChargeADC1;
	int nDischargeCount ;
	int nShRes1T, ih,logno1,nSQCount1, nSQLastY1;
	int nSQLastPin, tp1 ;
	double nISet,dVbR1,dVbR2,InterI;	
	int nFailFlag1,nFailFlag2,nFailFlag3,nFailFlag4; 
	int nSQ1Fail[200];//, nSQPinCheck[34000];
	int nSh1Ret1[100],dPinVC[200],dPinVB[200],nSQNet1[200][200];// pin array//sylee1209
	double nT[10],nData[130][15], dR1[11][200];
	double dVB1[11][200],dVC1[11][200],dI1[11][200]; 		
	int nMSet1Flag,nMSet1Co, nMSet1CoT2;
	int nSQStep1flag,nIControl,nRetry1; 
    int nIFilter,nHVOffSet,nHVGain;//sylee130629 	 
	int nCC,  nVRel, nVSet,nIR ;
    int m_nMode1,nMod1;	
	double nProRv1,nProRv2; 
    int nRet ;   
	int nStop2,nPinBlockEnd;//sylee240509
	int a, nSetLeakMaxR1;//sylee240509
    char szText[100];//sylee240509
    int nIGain;//sylee240517
	if(	nLeakRunLV==1){
		return 0;//sylee240530
	}
	str.Format(" RUN  ");//sylee240509			 

    nLeakRunHV=1;//sylee240530
    m_Label_2wLeak.SetCaption(str);//sylee240509

	g_nStopFlag=0;//sylee240509
	m_nProg2wLeak_Rate=0;//sylee240509 
    Sub71.AOnRelayOff1(); //sylee240509 
 	Sub71.OnDisCharge1(); //sylee240509 

    nFailL1Co=0;//sylee140110

    int k1;

    k1=FileSysInfo01.LoadSaveSet12(_LOAD);//load : 2
    if( k1!=1){
        str.Format("file loading Error!,  BDL File Load!, Etc. File Load! ");			 		
        m_list1.AddString(str); 
        nlist1++;
        return 0;
    }
 
    str.Format(" ");			 		
    m_list1.AddString(str); 
    nlist1++;
    str.Format("  ***> Manual HV Short (Leak) :  START  *******  ");			 
    m_list1.AddString(str); 
    nlist1++;
    DoEvents();  
    nMod1=1;
   // m_nMode1=327;//sylee150811 
 
	 m_nMode1=326;//sylee150811 //sylee220918  302 ->327  200v ->250v  //sylee230615  327->326
	 m_edit101.GetWindowText(szText, 10); //sylee230616-1	//sylee240509

	a = atoi(szText);//sylee230616-1 //sylee240509

 	nSetLeakMaxR1 = 300;//sylee240509	
	
#ifdef __LEAK_200M_ //sylee240509   
        nSetLeakMaxR1 = 200;///sylee240509
#endif 
#ifdef __LEAK_500M_ //sylee240509
	 	nSetLeakMaxR1 = 500;//sylee240509       
#endif 
#ifdef __LEAK_1000M_ //sylee240509
	 	nSetLeakMaxR1 = 1000;//sylee240509          
#endif 

	
	if( a<10){  a=10;   sprintf(szText, "%d", a);    m_edit101.SetWindowText(szText); }
    if( a>nSetLeakMaxR1){  a=nSetLeakMaxR1; sprintf(szText, "%d", a);    m_edit101.SetWindowText(szText); } 

     DoEvents();DoEvents();DoEvents();
    //sylee230616-1	 Sub71.R1=100000000.; //100M  
	 Sub71.R1=a*1000000.; //100M 

    nRet=FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode1, PART_MIXED); //son220412

    if( nRet!=1){
        str.Format("ERRor NO 3002,   \n  HR Short Calibration File reading Error , \n   Mode no= %d  ", m_nMode1 );
        AfxMessageBox( str);
        return 0; 
    }

    RCalCovert( m_nMode1, Sub71.R1 , REAL_TO_RAW , PART_UPPER);   // m_nMode1 1~76   , //son220412_3 
    nProRv2=Sub71.R1;//sylee240521
	nProRv1=nProRv2-10;//sylee240521  //sylee240614

    if(nProRv1<50){
        nProRv1=50;
    }
    if(nProRv1>5000){
        nProRv1=5000;
    }
    //////////////////////////////////////
    //nProRv1=400;//raw
    logno1=0;    nShRes1T=0;	nStop=0;
    ipFlag=0;	n4Wire=0;	ipEnd=1; 
    nCDelay1=100;
    nDisChargeADC1=10;//testmode
    nDischargeCount=20;//sylee20120830
    nHVIADCFlag=1;//sylee121018   //waiting 
    nIControl=0;

    ::ZeroMemory (&nData, sizeof(nData));
    ::ZeroMemory (&dR1, sizeof(dR1));
    ::ZeroMemory (&dVB1, sizeof(dVB1));
    ::ZeroMemory (&dVC1, sizeof(dVC1));
    ::ZeroMemory (&dI1, sizeof(dI1));
    ::ZeroMemory (&nT, sizeof(nT));
    ::ZeroMemory (&nData, sizeof(nData));
    ::ZeroMemory (&dPinVC, sizeof(dPinVC));
    ::ZeroMemory (&dPinVB, sizeof(dPinVB));	
    ::ZeroMemory (&fName, sizeof(fName));
    ::ZeroMemory (&fName1, sizeof(fName1));
    ::ZeroMemory (&nSh1Ret1, sizeof(nSh1Ret1));
    ::ZeroMemory (&nSQ1Fail, sizeof(nSQ1Fail));//sylee120907


    nCC= (int) dRec[m_nMode1][1];
    nVRel= (int)dRec[m_nMode1][2];
    nVSet= (int) dRec[m_nMode1][3];
    nIR= (int)dRec[m_nMode1][4];
    nISet= dRec[m_nMode1][5];
    nDelay10= (int)dRec[m_nMode1][6]*30;
    nFil=(int)dRec[m_nMode1][8];
    nIFilter = (int)dRec[m_nMode1][9];   //sylee130629		  
    nHVOffSet =(int) dRec[m_nMode1][10]; //sylee130629
    nHVGain = (int)dRec[m_nMode1][11];	//sylee130629
    nIGain = (int)dRec[m_nMode1][12];   //sylee230517     


	nRet=Sub71.AOnPoweroff2(); //sylee240509
 	if(nRet!=1){
		AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!"); 
		return  0;
	} 
 
	if( nVRel==3){//sylee240509
		nRet=Sub71.AOnVSetMulti1(0,0,nVSet,0) ;
	}else  if( nVRel==4){//sylee240509
		nRet=Sub71.AOnVSetMulti1(0,0,0,nVSet) ;
	} 
 	if(nRet!=1){
		AfxMessageBox(" Error No 1025, \n   VB Power SET   fail !,   SW TEST Run Start Error!!!"); 
		return  0;
	} 
    Sleep(50);//sylee240509
     if( nISet<0.1){
        AfxMessageBox(" Error No 7713, \n    I Set  ( <0.1 ) SET ERROR !,  Short Start Error!!!"); 
    }
 
    //ADC COUNT
    Sub71.nADCount1 = (int)dRec[m_nMode1][7];
    if(nIR==5){//uA
        InterI=	10.0;
    }else{
        InterI=nISet*0.9;
    }

    //=========================================================================================
    //interlock    

    if( nCC!=1 && nCC!=2 ){
        AfxMessageBox(" Error No 1024, \n    CC CV MODE SET ERROR !,   Run Start Error!!!"); 
        return 0;
    }
    if( nVSet<1 || nVSet>300 ){
        AfxMessageBox(" Error No 1025, \n    Votage Set Range Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        return 0;
    }
    if( nVRel<1  || nVRel>4 ){ 
        AfxMessageBox(" Error No 1025, \n    Votage Set Mode 1~4 ( LV1,2, HV1,2)  Error ( 1~300V) MODE SET ERROR !,    Run Start Error!!!"); 
        return 0; 
    }

    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //������ 
 
    Lo1=1000;//sylee150708
    Sub71.AOnReadVB1Run1(nVRel);//sylee130802

 

Task101:    

    Sub71.AOnReadVB1Run1(nVRel); 

    if(nVSet<=5){
        dVbR2=nVSet*(0.35);//10%
    }else{
        dVbR2=nVSet*(0.1);//5%
    }
    dVbR1=nVSet-dVbR2;
    dVbR2=nVSet+dVbR2;

    if( Sub71.V1>300){
        if(Lo1-->0){					 					
            //sylee121019	Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(30000);  //1ms   //sylee150708  1->3ms 
            goto Task101;
        }
        str.Format("Error No 1301,\n\n  Power Check? \n\n\n   Calibration NO=%d  VB READ Error!!! \n ***** HW CHECK Please! *****\n VB READING Error \nVB Set=%d, VB Read =%f.2,\n VB low=%.2f, VB high =%.2f,", (int)m_nMode1,nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str);	
        return 0;
    }

    if( Sub71.V1<dVbR1 || Sub71.V1>dVbR2 ){
        if(Lo1-->0){
            //sylee121019	Sub71.Delay_100ns(100000);  //10ms   
            Sub71.Delay_100ns(10000);  //1ms   
            goto Task101;
        }			
        str.Format("Error No 1317,\n \n\n Voltage set error.    Power Check? \n\n\n  VB READ Error!!    Calibration NO=%d ! \n\n\n   VB Set=%d,    VB Read =%.2f \n\n\n,\n Range(VB low=%.2f, VB high =%.2f),",(int)m_nMode1, nVSet,Sub71.V1,dVbR1,dVbR2); 
        AfxMessageBox(str);						  
        return 0;
    }
 
    Sub71.AOnReadV1Enable1(nVRel); //sylee20120223   //VC SENSE INIT SET 
    Sub71.FilterSet(nFil);//sylee130702 
    if(nIFilter==3){
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!"); 
        nIFilter=1;
    }
    Sub71.I_Filter(nIFilter);//sylee130629
	
    if (nIGain == 2){ //sylee240517    
       Sub71.I_Gain(2);  //10x  =io on        
    } else {
        Sub71.I_Gain(1);  //1x  =io off
    }
    
	Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);//sylee20111213     //vb RELAY ON
    Sub71.OnPinAllReset1(); ///PIN ALL RESET //sylee20120118

    //--------------------------------------------------------------------------------     
    ipEnd=1;
    nEnd=0;

    if( nVSet>250){//sylee240516  200->250
        nLoop=1;
        str.Format("Error No 1318,\n \n\n  MANUAL SHORT START ERROR    VSET >250V !!    "); 
        AfxMessageBox(str);
        return 0;
    }else{
        nLoop=0;
    }

    ipFlag=2;
     nStop2=0;

    FileSysInfo01.LoadSaveSub15(2);//sylee140110

  

//########################################################################################################################
	
	ipEnd=1;//sylee240509
 
	nPinBlockEnd=SysSet13.m_nSet105+1; //4=16k,8=32k//sylee240509
	if(nPinBlockEnd<0){nPinBlockEnd=1;}//sylee240509
 	if(nPinBlockEnd>16){nPinBlockEnd=16;}//sylee240509

    for( int nLoop2=1; nLoop2<=nPinBlockEnd; nLoop2++ ){ //sylee240509
  
	m_nProg2wLeak_Rate=(int)(((double)nLoop2/ (double)nPinBlockEnd)*100.0);//sylee240509
 
	if( nFailL1Co>=SysInfo05.m_nIsEr){	
		str.Format("           : block=%d  Start Stop  : Error Limit    ", nLoop2);          									 
	}else{
		str.Format("           : block=%d  Start  ",nLoop2);		
	} 	
	m_list1.AddString(str); 
	nlist1++;
//====================================================================================================================================
for( int ip=1; ip<=ipEnd; ip++ ){ 
	nEtemp1=0;	nECount=0; 	nFailL1CoPiece=0;
 	nEt1=0;	nShort2FailFlag1=0;
	//*******************************************************************************************************************************//
 //Task1:
 
	nRCount=4096;
 

	nSQCount1=0;//sylee120908   //SQRT( SQUARE ROOT)
	for( i=1; i<200;i++){
		tp1=i*i;
		if(tp1>=nRCount){
			nSQCount1=i;//sylee120908
			break;
		}
	}

	if( (nSQCount1==0)  || (nRCount<1)){//sylee121018
			str.Format("Error No 7513-6 ,   SET ERORR   Net List <1,  ,    piece pin no set check?   HV SQUARE  COUNT = ZERO   n"   ); 
			AfxMessageBox(str);	
   			return 0;
	}

    ::ZeroMemory(&nSQNet1, sizeof(nSQNet1));

    //################################################################################################################################
    //SET 


    for( ik=1; ik<=4096; ik++){

        i=(ik-1)/nSQCount1  + 1;//mok
        j=(ik-1)-nSQCount1*(i-1);//na	 
        j=j+1; 

        nSQNet1[i][j]=ik-1+(nLoop2-1)*4096;//sylee240509
        if( ik==4096){
            nSQLastPin=j;//sylee120908   ///last row , last col.
            nSQLastY1=i;
        }
    } 


/////////////////////////////////////////////////////////////////////////////////
	 
	      Sub71.OnPinAllReset1(); //sylee240612
 		  Sub71.AOnPinSet1(1, 0, 1); //sylee240612
  	      Sub71.AOnPinSet1(1, 63, 2);  //sylee240612	
 		  Sleep(10);  //sylee240612	
	      dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1);    //sylee240612	
 
//################################################################################################################################
//#1 FIRST-1  Line&Block
//row

	::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));	
	nShort2FailFlag1=0;
	nSQStep1flag=0;
    nRetry1=0;

	Sub71.OnPinAllReset1(); ///PIN ALL RESET 


//Task1Retry:

 
	for( ik=1; ik<nSQLastY1; ik++)     // FIRST LOOP //Y  VB 
    {	
        DoEvents(); 

        if(g_nStopFlag==2){nStop2=2;}//sylee240510

        if( nStop2==2){
            str.Format(" *****  STOP  *******");			 		
            m_list1.AddString(str); 
            nlist1++;
            goto Task211;	
        }

        if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
            break;// complete  error 												 
        }

        if(ik==1  || nSQStep1flag==2){

            Sub71.OnPinAllReset1(); ///PIN ALL RESET 

            for( int ik2=1; ik2<=nSQLastY1; ik2++){   

                if(ik2>ik){
                    continue;
                }
                for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                    if( ik2==nSQLastY1){  //LAST Y STEP
                        if(ih>nSQLastPin){
                            break;//SKIP   
                        }
                    }
                    Sub71.AOnPinSet1(1, nSQNet1[ik2][ih], 1); //=1Vb,    
                } 
            }

            nSQStep1flag=0;

        }
        else{//end of if(ik==1  || nSQStep1flag==2)			   
            Sub71.OnPinResetVC();  
            for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                if( ik==nSQLastY1){  //LAST Y STEP
                    if(ih>nSQLastPin){
                        break;//SKIP   
                    }
                }				   
                Sub71.AOnPinSet1(1, nSQNet1[ik][ih], 1); //=1  vb 
            }
        }

        for( ih=1; ih<=nSQCount1; ih++){  //X   // Second LOOP   vb pin  
            if( (ik+1)==nSQLastY1){
                if(ih>nSQLastPin){
                    break;//SKIP
                }
            }  			   
            Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih], 2);   // =2,Vc,   all	
        }
        //////////////////////////////////////////////////////////////////////////////////

        dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1);   

        if( Sub71.R1<=nProRv1){// ok//HV
            nSQ1Fail[ik]=1;
            nFailFlag1=0;
        }else{//fail
            nSQ1Fail[ik]=2;  // LAST ik=0;
            nShort2FailFlag1=1;
            nFailFlag1=1; 	   		 
            nSQStep1flag=2;
        } 

        //==============================================================================================================================
        //################################################################################################################################
        //#2,  second search    hv  adc  
        // vc  m block 

        DoEvents(); 


        //	nFailFlag1=0;//sylee240510-test


        if(nFailFlag1==1)  //if error	 
        {
            DoEvents(); 
            if( nStop2==2){
                str.Format(" *****  STOP  *******");			 		
                m_list1.AddString(str); 
                nlist1++;
                goto Task211;					

            }
            Sub71.OnPinResetVB();   //sylee121029  

            for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc
            {
                nMSet1Co=0;
                nMSet1Flag=0;
                nMSet1CoT2=0;

                if(ik2>ik){ //doublecheck#2 
                    continue;
                }

                for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
                    if(ik2==nSQLastY1){  //LAST Y STEP
                        if(ih>nSQLastPin){
                            break;//SKIP   
                        }
                    }					   
                    Sub71.AOnPinSet1(1, nSQNet1[ik2][ih], 1); //=2VC,	 
                } 
                //=====================================================================================================================

                dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1);//sylee240110-1   2->1 	

                //=====================================================================================================================
                nFailFlag2=0;

                if( Sub71.R1<=nProRv1){// ok//HV
                }else{//fail
                    nFailFlag2=1;
                }			 

                //=====================================================================================================================
                //##################################################################################################################################

                //one line & one line  search

                if(nFailFlag2==1)  
                {
                    DoEvents(); 
                    if( nStop2==2){
                        str.Format(" *****  STOP  *******");			 		
                        m_list1.AddString(str); 
                        nlist1++;
                        goto Task211;					

                    }

                    for( int ih6=1; ih6<=nSQCount1; ih6++)    //X // Second LOOP   vc pin 
                    {
                        if( ik2==nSQLastY1){  //LAST Y STEP
                            if(ih6>nSQLastPin){
                                break;//SKIP   
                            }
                        }

                        Sub71.OnPinResetVB();//sylee121016***** all reset								    
                        // Sub71.Delay_100ns(100);//10uSEC   

                        Sub71.AOnPinSet1(1, nSQNet1[ik2][ih6], 1); //=2VC,	 

                        //===================================================================================================================== 

                        dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1); 						 


                        nFailFlag3=0;

                        if( Sub71.R1<=nProRv1){// ok//HV
                        }else{//fail
                            nFailFlag3=1;
                        }							 

                        //=====================================================================================================================
                        //################################################################################################################################
                        ///one point  & one point search

                        if(nFailFlag3==1) 
                        {
                            DoEvents(); 
                            if( nStop2==2){
                                str.Format(" *****  STOP  *******");			 		
                                m_list1.AddString(str); 
                                nlist1++;
                                goto Task211;					

                            }


                            for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  
                            {

                                if( (ik+1)==nSQLastY1){
                                    if(ih2>nSQLastPin){
                                        break;//SKIP
                                    }
                                }

                                Sub71.OnPinResetVC(); //Vb all reset
                                // Sub71.Delay_100ns(100);//10uSEC   //sylee121017
                                Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih2], 2);   // =1,VB,   all		 
                                Sub71.Delay_100ns(200000);//100mSEC //sylee240111													 
                                dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1); 											  


                                nFailFlag4=0;

                                if( Sub71.R1<=nProRv2){// ok//HV  //sylee230521
                                }else{//fail
                                    // 	if(ih2==ih6)
                                    nFailFlag4=1;


                                    for(int iq1=1; iq1<=nFailL1Co; iq1++){
                                        if( (nFailL1[iq1][4]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][5]==(nSQNet1[ik2][ih6]+1))){
                                            nFailFlag4=0; break;
                                        }
                                        if( (nFailL1[iq1][5]==(nSQNet1[ik+1][ih2]+1))&&(nFailL1[iq1][4]==(nSQNet1[ik2][ih6]+1))){
                                            nFailFlag4=0; break;
                                        }
                                    }

                                }


                                //##########################################################################################################################################3


                                if(nFailFlag4==1){//sylee130111  
                                    //if((nOpenErrList[tmp1][0]<=0)&&(nOpenErrList[tmp2][0]<=0)) 												 

                                    if(nFailL1Co<3000){//array size nFailL1[

                                        nFailCount[nMod1]=nFailCount[nMod1]++; 
                                        nFailL1Co++;

                                        nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV
                                        if( nSQNet1[ik+1][ih2]<nSQNet1[ik2][ih6]){
                                            nFailL1[nFailL1Co][5]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                            nFailL1[nFailL1Co][4]=nSQNet1[ik2][ih6]+1;//pin  vc
                                        }else{
                                            nFailL1[nFailL1Co][4]=nSQNet1[ik+1][ih2]+1 ;//pin  vb
                                            nFailL1[nFailL1Co][5]=nSQNet1[ik2][ih6]+1;//pin  vc
                                        }  																		 
                                        nFailL1[nFailL1Co][6] = Sub71.R1;//sylee230615-2 

                                    }   //end of if(nFailL1Co<3000){//array size nFailL1[


                                    Sub71.Delay_100ns(3000);//0.3mSEC    

                                    if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
                                        break;// complete  error 
                                    }	 


                                }   //if(nFailFlag4==1) 

                            } //for( int ih2=1; ih2<=nSQCount1; ih2++)  //X   // Second LOOP   vb pin  


                            //----------------------------------------------------------------------------------------------------------------------

                            Sub71.OnPinResetVC(); 
                            //   Sub71.Delay_100ns(100);//10uSEC   

                            for(  int ih3=1; ih3<=nSQCount1; ih3++){  //X   // Second LOOP   vb pin  

                                if( (ik+1)==nSQLastY1){
                                    if(ih3>nSQLastPin){
                                        break;//SKIP
                                    }
                                }
                                Sub71.AOnPinSet1(1, nSQNet1[ik+1][ih3], 2);   // =1,VB,   all										      

                            } 


                        }// if(nFailFlag3==1) 



                        if( nFailL1Co>=SysInfo05.m_nIsEr){	   
                            break;// complete  error 
                        }

                        //################################################################################################################################


                    }//end of for( ih=1; ih<=nSQCount1; ih++)    //X // Second LOOP   vc pin 


                    //=====================================================================================================================
                }//	end of  if(nFailFlag2==1)   ///one line & one line  search


                if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
                    break;// complete  error 
                }  




                //#2
            }//end of for( int ik2=1; ik2<=nSQLastY1; ik2++)     // FIRST LOOP //Y   //Vc



        }//end of  if(nFailFlag1==1)

        if( nFailL1Co>=SysInfo05.m_nIsEr){	   
            break;// complete  error 
        } 

    }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++)     // FIRST LOOP //Y  VB



 
//================================================================================================================
//################################################################################################################################
//#1 FIRST-1  Line&Block
// Y 
    Sub71.OnPinAllReset1();  
   ::ZeroMemory(&nSQ1Fail, sizeof(nSQ1Fail));
	nShort2FailFlag1=0;
	nSQStep1flag=0;
	nRetry1=0;			  


//Task2Retry:
 
	for( ik=1; ik<nSQCount1; ik++){     // FIRST LOOP //Y  VB 

	    DoEvents(); 
		if(g_nStopFlag==2){nStop2=2;}//sylee240510

	   if( nStop2==2){
			str.Format(" *****  STOP  *******");			 		
   			m_list1.AddString(str); 
			nlist1++;
			goto Task211;
		}

		if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
	 		break;// complete  error 												 
		}

		 if(ik==1  || nSQStep1flag==2){

			   Sub71.OnPinAllReset1(); ///PIN ALL RESET			   
		  
			   for( int ik2=1; ik2<=nSQCount1; ik2++){  

					   if(ik2>ik){
						   continue;
					   }
 					   for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
						   if( ih==nSQLastY1){  //LAST Y STEP
							   if(ik2>nSQLastPin){
								  break;//SKIP   
							   }
						   }
						   Sub71.AOnPinSet1(1, nSQNet1[ih][ik2], 1); //=2VC,   
					   } 
			   }
			   
			   nSQStep1flag=0;

		}else{//end of if(ik==1  || nSQStep1flag==2){

			   
		     Sub71.OnPinResetVC(); 
		     //   Sub71.Delay_100ns(100);//10uSEC  

			   for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
				   if( ih==nSQLastY1){  //LAST Y STEP
					   if(ik>nSQLastPin){
						  break;//SKIP   
					   }
				   }

				   Sub71.AOnPinSet1(1, nSQNet1[ih][ik], 1); //=2VC, 
 
			   } 
	
		}

		   
		   for( ih=1; ih<=nSQLastY1; ih++){  //X   // Second LOOP   vb pin  
 			   if( ih==nSQLastY1){
				   if((ik+1)>nSQLastPin){
					  break;//SKIP
				   }
			   }
 			   
			   Sub71.AOnPinSet1(1, nSQNet1[ih][ik+1], 2);   // =1,VB,   all		   
 
		   }
//////////////////////////////////////////////////////////////////////////////////
 
		   dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1); 	 
 	  									    
			if( Sub71.R1<=nProRv1){// ok//HV
				nSQ1Fail[ik]=1;
				nFailFlag1=0;
			}else{//fail
				nSQ1Fail[ik]=2;  // LAST ik=0;
				nShort2FailFlag1=1;
				nFailFlag1=1; 
	   		//nFailFlag1=0;//test
				nSQStep1flag=2; 
			}
		 
             // nFailFlag1=0;  // ALLPASS
//==============================================================================================================================
//################################################################################################################################
//#2,  second search    hv  adc  
// vc  m block 



	//		nFailFlag1=0;//sylee240510-test


		if(nFailFlag1==1){  //if error     
			
   				DoEvents(); 
				if( nStop2==2){
					str.Format(" *****  STOP  *******");			 		
   					m_list1.AddString(str); 
					nlist1++;
					goto Task211;					

				}

 			     Sub71.OnPinResetVB();//sylee121029          


			 for( int ik2=1; ik2<=nSQCount1; ik2++){     // FIRST LOOP //Y   //Vc
						 
				    nMSet1Co=0;
					nMSet1Flag=0;
					nMSet1CoT2=0;		

   //sylee121029    Sub71.OnPinResetVB();
					//Sub71.Delay_100ns(100);//10uSEC   
				   
				    if(ik2>ik){ //doublecheck#2 
					   continue;
					}

				   for( ih=1; ih<=nSQLastY1; ih++){    //X // Second LOOP   vc pin 
					   if(ih==nSQLastY1){  //LAST Y STEP
						   if(ik2>nSQLastPin){
							  break;//SKIP   
						   }
					   }
					   
					   Sub71.AOnPinSet1(1, nSQNet1[ih][ik2], 1); //=2VC, 			 
				   } 


//=====================================================================================================================				 
				   dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1); 	
				     
//=====================================================================================================================
					nFailFlag2=0;
			   									    
					if( Sub71.R1<=nProRv1){// ok//HV
					}else{//fail
						nFailFlag2=1;
					}		  

//=====================================================================================================================
//##################################################################################################################################

//one line & one line  search


					  if(nFailFlag2==1){  

         					DoEvents(); 
							if( nStop2==2){
								str.Format(" *****  STOP  *******");			 		
   								m_list1.AddString(str); 
								nlist1++;
								goto Task211;					

							}
 						   
  						   for( int ih6=1; ih6<=nSQCount1; ih6++){    //X // Second LOOP   vc pin 

								  if( ik2==nSQLastY1){  //LAST Y STEP
									   if(ih6>nSQLastPin){
										  break;//SKIP   
									   }
								   }
								  
     							   Sub71.OnPinResetVB();//sylee121016***** all reset								    
								 //  Sub71.Delay_100ns(100);//10uSEC   

								   Sub71.AOnPinSet1(1, nSQNet1[ih6][ik2], 1); //=2VC,
                                    
//=====================================================================================================================								 
								   dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1); 							  


								   nFailFlag3=0;
								 									    
									if( Sub71.R1<=nProRv1){// ok//HV
									}else{//fail
										nFailFlag3=1;
									}
							 
   //=====================================================================================================================
 //################################################################################################################################
///one point  & one point search
 
				                            if(nFailFlag3==1){ 												
												
         										DoEvents(); 
												if( nStop2==2){
													str.Format(" *****  STOP  *******");			 		
   													m_list1.AddString(str); 
													nlist1++;
													goto Task211;					

												}  									
												
												for( int ih2=1; ih2<=nSQLastY1; ih2++){  //X   // Second LOOP   vb pin  
																								   
													   if( ih2==nSQLastY1){
														   if((ik+1)>nSQLastPin){
															  break;//SKIP
														   }
													   }

													   if( ih6 != ih2){//sylee121025
														   continue;
													   }

												       Sub71.OnPinResetVC(); //Vb all reset
													  // Sub71.Delay_100ns(100);//10uSEC   //sylee121017
													   Sub71.AOnPinSet1(1, nSQNet1[ih2][ik+1], 2);   // =1,VB,   all
													   Sub71.Delay_100ns(200000);//200mSEC //sylee240111 														 
												        dView7.AHV_ADCI_Read1_OneToM( nProRv1, nDelay10, 1); 														   

 														nFailFlag4=0;
													 								    
														if( Sub71.R1<=nProRv2){// ok//HV  //sylee230521
														}else{ 
															//if(ih2==ih6)
															nFailFlag4=1;
														   for(int iq1=1; iq1<=nFailL1Co; iq1++){
															    if( (nFailL1[iq1][4]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][5]==(nSQNet1[ih6][ik2]+1))){
                                                                    nFailFlag4=0; break;
																}
																if( (nFailL1[iq1][5]==(nSQNet1[ih2][ik+1]+1))&&(nFailL1[iq1][4]==(nSQNet1[ih6][ik2]+1))){
                                                                    nFailFlag4=0; break;
																}
															}

														}
														 
				    														 
//##########################################################################################################################################3
														
														if(nFailFlag4==1){//sylee130111 														 								
																								
																if(nFailL1Co<3000){//array size nFailL1[
																 
																		nFailCount[nMod1]=nFailCount[nMod1]++;
																		//log buffer
																		nFailL1Co++;																			 
																		nFailL1[nFailL1Co][1]=nMod1;  //1=open, 2=ushort, 3=short, 4=hr , 5=hv ,6=4W,  //USHORT  SHORT  HR HV
																	 
																	    if( nSQNet1[ih2][ik+1]<nSQNet1[ih6][ik2]){
																			nFailL1[nFailL1Co][5]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
																			nFailL1[nFailL1Co][4]=nSQNet1[ih6][ik2]+1;//pin  vc
																		}else{
																			nFailL1[nFailL1Co][4]=nSQNet1[ih2][ik+1]+1 ;//pin  vb
																			nFailL1[nFailL1Co][5]=nSQNet1[ih6][ik2]+1;//pin  vc
																		}
																		nFailL1[nFailL1Co][6] = Sub71.R1;//sylee230615-2 

																	}   //end of if(nFailL1Co<3000){//array size nFailL1[
																					
																
																	Sub71.Delay_100ns(3000);//0.3mSEC 

																	if( nFailL1Co>=SysInfo05.m_nIsEr){	  //block ������ �߸� �Ǿ�  piece ���� �ϱ� ���� ���� .
	 																	break;// complete  error 
																	}

															}   //if(nFailFlag4==1){ 

 												   } //for( int ih2=1; ih2<=nSQCount1; ih2++){  //X   // Second LOOP   vb pin  

//----------------------------------------------------------------------------------------------------------------------

 
												   Sub71.OnPinResetVC(); 							 

												   for(  int ih3=1; ih3<=nSQLastY1; ih3++){  //X   // Second LOOP   vb pin  
					 
													   if( ih3==nSQLastY1){
														   if((ik+1)>nSQLastPin){
															  break;//SKIP
														   }
													   }
													   Sub71.AOnPinSet1(1, nSQNet1[ih3][ik+1], 2);   // =1,VB,   all														    

 												   } 


											}// if(nFailFlag3==1){ 



											if( nFailL1Co>=SysInfo05.m_nIsEr){	   
	 											break;// complete  error 
											}


//################################################################################################################################
 					   

                      }//end of for( ih=1; ih<=nSQCount1; ih++){    //X // Second LOOP   vc pin 
  

//=====================================================================================================================
				  }//	end of  if(nFailFlag2==1){   ///one line & one line  search


				  if( nFailL1Co>=SysInfo05.m_nIsEr){	   //sylee121016
	 			     break;// complete  error 
				  } 
 
            //#2
			}//end of for( int ik2=1; ik2<=nSQLastY1; ik2++){     // FIRST LOOP //Y   //Vc


	   }//end of  if(nFailFlag1==1){
 

	  if( nFailL1Co>=SysInfo05.m_nIsEr){	   
		  break;// complete  error 
	  }  
 

  }  //=> line & block   first  	for( ik=1; ik<=nSQLastY1; ik++){     // FIRST LOOP //Y  VB


 }//end of  for ( ip


 	if( nFailL1Co>=SysInfo05.m_nIsEr){	
		str.Format("           : block=%d  End Stop  :  Error Limit  ", nLoop2);          									 
	}else{
		str.Format("           : block=%d  End  ",nLoop2);		
	} 			 		
	m_list1.AddString(str); 
	nlist1++;

}  //END OF for( int nLoop2=1; nLoop2<=nPinBlockEnd; nLoop2++ ){ //sylee240509



//==============================================================================================================================
//################################################################################################################################



Task211: 	 



	 //	str.Format(" ");  		m_list1.AddString(str);		nlist1++;	 

		 if(SysInfo05.m_nIsEr>1){//sylee140110
			if( nFailL1Co==SysInfo05.m_nIsEr){
 				str.Format("  ***>    Short Error Limited Set Count=%d,  Error Over !.   Stopped.  ", SysInfo05.m_nIsEr );			 		
   				m_list1.AddString(str); 
				nlist1++;
			}
		}

	//	str.Format(" ");   		m_list1.AddString(str); 		nlist1++;

		int nStep, nRawMode;//sylee240517
		nStep=0;//sylee230616
 
		for( i=1 ; i<=nFailL1Co ; i++){
 			 
	    	Sub71.R1=nFailL1[i][6];//sylee240111

			if(Sub71.R1>65000){nRawMode=1;}//sylee240517
			else {nRawMode=0;}//sylee240517
			

			RCalCovert(m_nMode1, Sub71.R1, RAW_TO_REAL ,PART_UPPER  ); //sylee240111
			if (Sub71.R1 < 0) {  	Sub71.R1 = 0.1;		}//sylee240111

			if(nRawMode==1){//sylee240517 		 
				if( Sub71.R1 < 1000.0){  //sylee230616
     				str.Format("   --> HV LEAK   No=%d,   pin %5.0f - %5.0f   ,   R < %10.2f ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1 );	//sylee230615	
				}else if( Sub71.R1 < 1000000.0){ //sylee230616
     				str.Format("   --> HV LEAK   No=%d,   pin %5.0f - %5.0f   ,   R < %10.2f K ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000. );	//sylee230615	
				}else { //sylee230616
     				str.Format("   --> HV LEAK   No=%d,   pin %5.0f - %5.0f   ,   R < %10.2f M ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000000. );	//sylee230615	
				}
			}else{//sylee240517 		 
				if( Sub71.R1 < 1000.0){  //sylee230616
     				str.Format("   --> HV LEAK   No=%d,   pin %5.0f - %5.0f   ,   R = %10.2f ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1 );	//sylee230615	
				}else if( Sub71.R1 < 1000000.0){ //sylee230616
     				str.Format("   --> HV LEAK   No=%d,   pin %5.0f - %5.0f   ,   R = %10.2f K ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000. );	//sylee230615	
				}else { //sylee230616
     				str.Format("   --> HV LEAK   No=%d,   pin %5.0f - %5.0f   ,   R = %10.2f M ohm  ",i,nFailL1[i][5],nFailL1[i][4],Sub71.R1/1000000. );	//sylee230615	
				}
			}			

			m_list1.AddString(str);		nlist1++;
		}


 		if( nFailL1Co <=0){    //sylee231215 
			str.Format("  ***>    ALL  PASS .     ");			 
		}else{
			str.Format("  ====>  Result : [ Error] => %d  ", nFailL1Co );	//sylee231215 		 		
		}
  		m_list1.AddString(str); 		nlist1++; 	 

		if( nStop2==2){
		    str.Format("  ***>     STOP.  *******  ");			 
		}else{
			str.Format("  ***>     FINISH.  *******  ");	 
		}
		m_list1.AddString(str);		nlist1++;

        if( nStop2==2){ //sylee240509
		    str.Format("STOP");  		 
		}		  
     	m_Label_2wLeak.SetCaption(str);//sylee240509

		if( nFailL1Co <=0){     
			str.Format(" OK  ");//sylee240509			 
		}else{
			str.Format(" NG "  ); 	//sylee240509 		
		}
     	m_Label_2wLeak.SetCaption(str);//sylee240509
 
	   Sub71.OnPinAllReset1(); ///PIN ALL RESET 
       Sub71.ARunoff1();   //vc sense off       /VC-SS  CMD off
  	   Sub71.AOnRelayOffSel(nVRel);//sylee20120720
       Sub71.FilterSet(0);//sylee130704 
	   Sub71.I_Filter(0);//sylee130704
       Sub71.AOnReadV1Enable1(5); //sylee130711
	   Sub71.AOnRelayOff1();//sylee210830
       Sub71.AOnPoweroff2();//sylee170411
   
       DoEvents();
	nLeakRunHV=0;//sylee240530

	  return 1;

}//end of CChildView3::OnManualShort1(){//sylee131023

 
