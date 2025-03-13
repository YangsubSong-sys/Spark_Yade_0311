// ChildView3.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView3.h"
#include "WinGdi.h"
#include "ModeSet17.h"
#include "ModeSet21.h"
#include "FileSysInfo.h" 
#include "ModeSer15.h"//sylee170417-2
#include "ModeManSpark.h"   //hdpark230223
#include "PassWord1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int nFlag_PassOnNo, nFlag_PassOn;
extern nAutoProcess1;

extern int  gDef_Language1; //SYLEE151028   //1=KOREA ,2=ENGLISH, 3=CHINA
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1

extern CFileSysInfo FileSysInfo01;

/////////////////////////////////////////////////////////////////////////////
// CChildView3

IMPLEMENT_DYNCREATE(CChildView3, CFormView)

CChildView3::CChildView3()
	: CFormView(CChildView3::IDD)
{
	//{{AFX_DATA_INIT(CChildView3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CChildView3::~CChildView3()
{
}

void CChildView3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	    DDX_Control(pDX, IDC_BUTTON1002, m_button1002); 
		DDX_Control(pDX, IDC_BUTTON1001, m_button1001); 
		DDX_Control(pDX, IDC_BUTTON1004, m_button1004); 
		DDX_Control(pDX, IDC_BUTTON1006, m_button1006); 
		DDX_Control(pDX, IDC_BUTTON1003, m_button1003); 
		DDX_Control(pDX, IDC_BUTTON1005, m_button1005); 
		DDX_Control(pDX, IDC_BUTTON1007, m_button1007);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView3, CFormView)
	//{{AFX_MSG_MAP(CChildView3)
		ON_WM_ERASEBKGND()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CChildView3, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView3)
	ON_EVENT(CChildView3, IDC_BUTTON1001, -600 /* Click */, OnClickButton1001, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON1002, -600 /* Click */, OnClickButton1002, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON1003, -600 /* Click */, OnClickButton1003, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON1004, -600 /* Click */, OnClickButton1004, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON1005, -600 /* Click */, OnClickButton1005, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON1006, -600 /* Click */, OnClickButton1006, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON1007, -600 /* Click */, OnClickButton1007, VTS_NONE)
	ON_EVENT(CChildView3, IDC_BUTTON_SPARK, -600 /* Click */, OnButtonSpark, VTS_NONE) //hdpark230223
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView3 diagnostics

#ifdef _DEBUG
void CChildView3::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView3 message handlers



void CChildView3::A_Language() //sylee151028 //LANGUAGE CHANGE
{


	//son 중국어 메뉴 사용시 
	if( gDef_Language1==3)//sylee151028-1    
	{
		m_button1002.SetWindowText(LoadMessage(1,"TitleManual", 101));  
		m_button1001.SetWindowText(LoadMessage(1,"TitleManual", 102));
		m_button1004.SetWindowText(LoadMessage(1,"TitleManual", 103));  
		m_button1006.SetWindowText(LoadMessage(1,"TitleManual", 104));
		m_button1003.SetWindowText(LoadMessage(1,"TitleManual", 105));  
		m_button1005.SetWindowText(LoadMessage(1,"TitleManual", 106));
		m_button1007.SetWindowText(LoadMessage(1,"TitleManual", 107));
	}

}




void CChildView3::OnClickButton1001() 
{
 
	CModeMan11 Dlg;
	Dlg.DoModal(); 	
	 
	return;
}


void CChildView3::OnInitialUpdate() 
{
      CFormView::OnInitialUpdate();

	  FileSysInfo01.LoadSaveSet211(2);//load //sylee160928

      A_Language();

}



void CChildView3::OnClickButton1002() 
{
  
	CModeMan12 Dlg;
	Dlg.DoModal(); 	

	return;
}

void CChildView3::OnClickButton1003() 
{



	CModeSet17 Dlg;
	Dlg.DoModal(); 	

 	//CModeMan13 Dlg;
	//Dlg.DoModal(); 	

	return;

 
}
void CChildView3::OnClickButton1004() 
{
	CModeMan14 Dlg;
	Dlg.DoModal(); 	
}


void CChildView3::OnClickButton1005() 
{
 	//	return;   //sylee170510
 	CModeSer15 Dlg;
	Dlg.DoModal(); 	

	return; 
}

void CChildView3::OnClickButton1006() 
{
	CModeMan16 Dlg;
	Dlg.DoModal(); 	
}

void CChildView3::OnClickButton1007() 
{

//	return;

	CModeSet21 Dlg;//sylee160524
	Dlg.DoModal(); 	
}

/*
BOOL CChildView3::OnEraseBkgnd(CDC* pDC)    
{
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));

//	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));//하늘색 
// DrawGradation(pDC, RGB(0, 0, 0), RGB(221, 236, 255));  //검은색  -> 검흰색 
//	DrawGradation(pDC, RGB(255, 255, 255), RGB(221, 236, 255));  //흰색   -> 흰색하늘색  
//	DrawGradation(pDC, RGB(254, 242, 216), RGB(252, 219, 143));  //황토색
  
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}
*/


void CChildView3::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
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




void CChildView3::OnPaint() 
{
	CPaintDC dc(this);
	CDC MemDC;
	BITMAP bmpInfo;
	//화면 DC화 호환되는 메모리 DC를 생성한다.
	MemDC.CreateCompatibleDC(&dc);
	//비트맵 리소스를 로딩한다.
	CBitmap bmp;
	CBitmap* pOldBmp = NULL;
	bmp.LoadBitmap(IDB_Bitmap_Back);
	//로딩된 비트맵의 정보를 알아본다.
	bmp.GetBitmap(&bmpInfo);
	//메모리 DC에 선택한다.
	pOldBmp = MemDC.SelectObject(&bmp);
	//메모리 DC에 들어있는 비트맵을 화면 DC로 복사하여 출력한다.
//	dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC,
//												0, 0, SRCCOPY);
	 TransparentBlt(dc.m_hDC,
					0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
					MemDC.m_hDC,
					0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
					RGB(192, 192, 192));

	//메모리 DC에 들어있는 비트맵을 두배 확대하여 출력한다.
//	dc.StretchBlt(bmpInfo.bmWidth, 0,
//			bmpInfo.bmWidth * 2, bmpInfo.bmHeight * 2, &MemDC,
//			0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	 TransparentBlt(dc.m_hDC,
					bmpInfo.bmWidth, 0,
					bmpInfo.bmWidth * 2, bmpInfo.bmHeight * 2,
					MemDC.m_hDC,
					0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
					RGB(192, 192, 192));

	MemDC.SelectObject(pOldBmp);

	CBitmap BmpIcon;

	BmpIcon.LoadBitmap(IDB_Bitmap_Icon);
	BmpIcon.GetBitmap(&bmpInfo);
	MemDC.SelectObject(&BmpIcon);

	dc.BitBlt(105, 60, bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC,
												0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBmp);
}

BOOL CChildView3::OnEraseBkgnd(CDC* pDC) 
{
	//그라데이션 배경을 그린다.
	DrawBk(pDC);

	int nH = 0, nW = 0;
	CDC MemDC;
	BITMAP bmpInfo;

	CRect Rect;
	GetClientRect(&Rect);
	nW = Rect.Width();
	nH = Rect.Height();

	MemDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	CBitmap* pOldBmp = NULL;
	bmp.LoadBitmap(IDB_Bitmap_Alpha);
	bmp.GetBitmap(&bmpInfo);
	pOldBmp = MemDC.SelectObject(&bmp);
	//투명하지 않은 일반 이미지를 출력한다.
	pDC->BitBlt(nW - (bmpInfo.bmWidth * 2), nH - bmpInfo.bmHeight,
				bmpInfo.bmWidth, bmpInfo.bmHeight,
				&MemDC, 0, 0, SRCAND);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 50;
	bf.AlphaFormat = 0;
	//반투명 이미지를 출력한다.
	::AlphaBlend(pDC->m_hDC,
					nW - bmpInfo.bmWidth, nH - bmpInfo.bmHeight,
					bmpInfo.bmWidth, bmpInfo.bmHeight,
					MemDC.m_hDC,
					0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight,
					bf);

	MemDC.SelectObject(pOldBmp);

	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}

void CChildView3::DrawBk(CDC *pDC)
{
	CRect Rect;
	int nR, nG, nB;
	int nH = 0, nW = 0;

	GetClientRect(&Rect);
	nW = Rect.Width();
	nH = Rect.Height();
	//화면의 절반에 그라데이션 효과를 주기위한 펜 객체들을 생성한다.
	CPen* pPen[64];
	for(int i = 0; i < 64; ++i)
	{
		nB = 245 + (i);
		if(nB > 255)		nB = 255;

		nG = 210 + (i + 4);
		if(nG > 255)		nG = 255;

		nR = 200 + (i + 7);
		if(nR > 255)		nR = 255;

		pPen[i] = new CPen(PS_SOLID, 1, RGB(nR, nG, nB));
	}
	//화면의 절반에 선을 그려 넣어, 그라데이션 효과를 준다.
	for(i = 0; i < nH / 2; ++i)
	{
		pDC->SelectObject(pPen[(i * 63) / nH]);
		pDC->MoveTo(0, i);
		pDC->LineTo(nW, i);
	}
	//펜 객체들을 제거한다.
	for(i = 0; i < 64; i++)		delete pPen[i];
	//나머지 반은 단순히 흰색으로 채운다.
	pDC->SelectStockObject(WHITE_PEN);
	for(int j = nH / 2; j < nH; j++)
	{
		pDC->MoveTo(0, j);
		pDC->LineTo(nW, j);
	}

	pDC->SelectStockObject(BLACK_PEN);
}

void CChildView3::OnButtonSpark()  //hdpark230223
{
	// TODO: Add your control notification handler code here
	nFlag_PassOn = 0;   
	nFlag_PassOnNo = 0;  //son C:\ACE500\Data\Message_L.ini의 101 번 항목의 passwd를 체크한다.

	CPassWord1 Dlg1;
	Dlg1.DoModal();

    //son 101번 passwd와 사용자가 입력한 passwd가 같았다면 2가 설정됨.
	if (nFlag_PassOn != 2)	return;

	CModeManSpark Dlg2;
	Dlg2.DoModal();

}

 
