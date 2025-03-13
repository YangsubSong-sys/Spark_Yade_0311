// ChildView9.cpp : implementation file
//
#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView9.h"
#include "MainFrm.h"
#include "ModeSub71.h"//SYLEE130405
#include "ModeSet13.h"//SYLEE130405
#include "FileSysInfo.h"


#ifdef _DEBUG
  #define new DEBUG_NEW
#undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

 
int  g_nLogOnRecipe1Retest; //sylee170720-1
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1
extern int  gDef_Language1; //SYLEE151028   //1=KOREA ,2=ENGLISH, 3=CHINA
extern CModeSub71  Sub71;//SYLEE130405
extern CSysSet13  SysSet13;  
extern nAutoProcess1;
int g_nLogOnRecipe1;//slee130913
int g_nLogOnUser1;

/////////////////////////////////////////////////////////////////////////////
// CChildView9

IMPLEMENT_DYNCREATE(CChildView9, CFormView)

CChildView9::CChildView9()
	: CFormView(CChildView9::IDD)
{
	//{{AFX_DATA_INIT(CChildView9)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CChildView9::~CChildView9()
{
}

void CChildView9::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView9)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	    	DDX_Control(pDX, IDC_EDIT1,   m_edit1);        

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView9, CFormView)
	//{{AFX_MSG_MAP(CChildView9)
		ON_WM_ERASEBKGND()

		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CChildView9, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView1)
	ON_EVENT(CChildView9, IDC_BUTTON1001, -600 /* Click */, OnButton1001, VTS_NONE)
	ON_EVENT(CChildView9, IDC_BUTTON1002, -600 /* Click */, OnButton1002, VTS_NONE)
	ON_EVENT(CChildView9, IDC_BUTTON1003, -600 /* Click */, OnButton1003, VTS_NONE)
//	ON_EVENT(CChildView9, IDC_BUTTON_1001, 3 /* BeforeDragOver */, OnBeforeDragOverButton1001, VTS_DISPATCH VTS_DISPATCH VTS_R4 VTS_R4 VTS_I4 VTS_DISPATCH VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView9 diagnostics

#ifdef _DEBUG
void CChildView9::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView9::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView9 message handlers


BOOL CChildView9::OnEraseBkgnd(CDC* pDC)    
{
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));
//	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));//하늘색 
//  DrawGradation(pDC, RGB(0, 0, 0), RGB(221, 236, 255));  //검은색  -> 검흰색 
//	DrawGradation(pDC, RGB(255, 255, 255), RGB(221, 236, 255));  //흰색   -> 흰색하늘색  
//	DrawGradation(pDC, RGB(254, 242, 216), RGB(252, 219, 143));  //황토색
  	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


void CChildView9::A_Language() //sylee151028 //LANGUAGE CHANGE
{
  if( gDef_Language1==3){//sylee151028-1
	  m_button1001.SetWindowText(LoadMessage(1,"TitleLogOn", 101));
      m_button1002.SetWindowText(LoadMessage(1,"TitleLogOn", 102));
      m_button1003.SetWindowText(LoadMessage(1,"TitleLogOn", 103));
  }

}

void CChildView9::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
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

//son200724: 4W-SamplingCount 비번입력기능
//son 전역함수.  외부 호출 가능
//son nLogOnUser1은 사용위치마다 다른 변수를 쓸 수 있도록 reference로 변경함.
void  CheckPassWord_forLogOnUser(char* szText, int& r_nLogOnUser1, bool bMessage)
{


    g_nLogOnRecipe1Retest=0; //sylee170720-1
    //sylee170605 if( SysSet13.m_nSet6!=1 ){//sylee131105
    //sylee161025 if( szText[0]=='b'&&szText[1]=='i'&&szText[2]=='o'){//super.  biotro engneer
    if( szText[0]=='o'&&szText[1]=='p'&&szText[2]=='e'){//operator
        r_nLogOnUser1=1;
    }
    else if( szText[0]=='m'&&szText[1]=='e'&&szText[2]=='l'){//mai. biotro engneer
        r_nLogOnUser1=3;
    }
    else if( szText[0]=='a'&&szText[1]=='c'&&szText[2]=='e'&&szText[3]=='4'&&szText[4]=='0'&&szText[5]=='0'){//super.  biotro engneer
        r_nLogOnUser1=5;
        g_nLogOnRecipe1=7; 
        if (bMessage == true)
            AfxMessageBox("      \n   Log on  OK  : User  5   \n\n   ", MB_OK );
    }
    else if( szText[0]=='1'&&szText[1]=='1'&&szText[2]=='1' &&szText[3]=='1'){//sylee130913   recipe
        g_nLogOnRecipe1=7; 
        if (bMessage == true)
            AfxMessageBox("      \n   Log on  OK  : Recipe   \n\n   ", MB_OK );
    }
    else if( szText[0]=='3'&&szText[1]=='5'&&szText[2]=='7' &&szText[3]=='9'){//sylee170720-1   recipe
        g_nLogOnRecipe1=7; 
        if (bMessage == true)
            AfxMessageBox("      \n   Log on  OK  : Recipe   \n\n   ", MB_OK );
    }
    else if( szText[0]=='1'&&szText[1]=='2'&&szText[2]=='3' &&szText[3]=='4'){//sylee170720-1
        g_nLogOnRecipe1=7; 
        g_nLogOnRecipe1Retest=2; //sylee170720-1
        if (bMessage == true)
            AfxMessageBox("      \n   Log on  OK  : Recipe   \n\n   ", MB_OK );
    }
    else{
        g_nLogOnRecipe1=0; 
        r_nLogOnUser1 = 0;        //son200704 추가: 아무 케이스도 아니면 0으로 초기화
    }

    if( szText[0]=='c'&&szText[1]=='v'&&szText[2]=='b'){//super.  biotro engneer
        r_nLogOnUser1=7;
        g_nLogOnRecipe1=7; 
        if (bMessage == true)
            AfxMessageBox("      \n   Log on  OK  : User  7   \n\n   ", MB_OK );
    }

}

void CChildView9::OnButton1001() //open
{
    char   szText[100];
    char  * szText1="1234";
    m_edit1.GetWindowText( szText,10);  

    //son200724: 4W-SamplingCount 비번입력기능 관련 수정
    //son Passwd를 체크한다.  
    CheckPassWord_forLogOnUser(szText, g_nLogOnUser1, true);


    ::ZeroMemory(&szText, sizeof(szText));
    m_edit1.SetWindowText( szText); 


    //son 'Auto' Test 화면으로 자동 전환한다.
    CMainFrame* pFrame;
    pFrame = (CMainFrame*)AfxGetMainWnd();
    pFrame->OnMenuTest7();      

    // pFrame->OnMenuTest9() ;
}


void CChildView9::OnButton1002() //save button 
{    
 
   char szText[100]; 
   g_nLogOnUser1=0;
   g_nLogOnRecipe1=0; //sylee130913
   	::ZeroMemory(&szText, sizeof(szText));
	m_edit1.SetWindowText( szText); 
 
}


void CChildView9::OnButton1003() //save as
{     
  	char szText[100];
 
    g_nLogOnUser1=0;

   	::ZeroMemory(&szText, sizeof(szText));
	m_edit1.SetWindowText( szText); 
    
	if (MessageBox("Exit Program?", "Confirm", MB_ICONEXCLAMATION | MB_YESNO) != IDYES) { //SYLEE130405
		return ;
	} 
 
	Sub71.OnPinAllReset1();   
	Sub71.Delay_100ns(10000);  //1ms
	Sub71.ARunoff1(); 
	Sub71.Delay_100ns(100000);  //10ms
	Sub71.AOnRelayOff1(); 
	Sub71.Delay_100ns(100000);  //10ms

   	AfxGetMainWnd()->PostMessage(WM_CLOSE);	//SYLEE140324
	
}
