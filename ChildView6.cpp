// ChildView6.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView6.h"




#include "ModeSer13.h" 
#include "ModeSub17.h"
#include "ModeSub71.h"
//#include "ModeSer81.h" 
 
#include "ModeSet11.h"//sylee180921-1 
#include "ModeSet12.h" 
#include "ModeSet13.h" 
#include "ModeSet15.h" //sylee121115-1
#include "ModeSet16.h" //sylee121115-1
#include "ModeSet19.h" //sylee150407

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



extern nAutoProcess1;

/////////////////////////////////////////////////////////////////////////////
// CChildView6

IMPLEMENT_DYNCREATE(CChildView6, CFormView)

CChildView6::CChildView6()
	: CFormView(CChildView6::IDD)
{
	//{{AFX_DATA_INIT(CChildView6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CChildView6::~CChildView6()
{

}

void CChildView6::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView6)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView6, CFormView)
	//{{AFX_MSG_MAP(CChildView6)
		ON_WM_ERASEBKGND()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView6 diagnostics

#ifdef _DEBUG
void CChildView6::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView6::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView6 message handlers


BOOL CChildView6::OnEraseBkgnd(CDC* pDC)    
{
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));

//	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));//ÇÏ´Ã»ö 
// DrawGradation(pDC, RGB(0, 0, 0), RGB(221, 236, 255));  //°ËÀº»ö  -> °ËÈò»ö 
//	DrawGradation(pDC, RGB(255, 255, 255), RGB(221, 236, 255));  //Èò»ö   -> Èò»öÇÏ´Ã»ö  
//	DrawGradation(pDC, RGB(254, 242, 216), RGB(252, 219, 143));  //È²Åä»ö
  
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


void CChildView6::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
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

BEGIN_EVENTSINK_MAP(CChildView6, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView6)
	ON_EVENT(CChildView6, IDC_BUTTON101, -600 /* Click */, OnButton101, VTS_NONE)
    ON_EVENT(CChildView6, IDC_BUTTON102, -600 /* Click */, OnButton102, VTS_NONE)
	ON_EVENT(CChildView6, IDC_BUTTON103, -600 /* Click */, OnButton103, VTS_NONE)
    ON_EVENT(CChildView6, IDC_BUTTON105, -600 /* Click */, OnButton105, VTS_NONE)
	ON_EVENT(CChildView6, IDC_BUTTON107, -600 /* Click */, OnButton107, VTS_NONE)
	ON_EVENT(CChildView6, IDC_BUTTON109, -600 /* Click */, OnButton109, VTS_NONE)   
	ON_EVENT(CChildView6, IDC_BUTTON110, -600 /* Click */, OnButton110, VTS_NONE)   
    ON_EVENT(CChildView6, IDC_BUTTON111, -600 /* Click */, OnButton111, VTS_NONE) 
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



//son Map > Manual
void CChildView6::OnButton101() 
{
    CModeSub71  ModeSub71;
 	ModeSub71.DoModal();
}

//son Map > SelfTest TR
void CChildView6::OnButton102() 
{
 	CModeSet11  ModeSet11;//sylee180921-1
	ModeSet11.DoModal();//sylee180921-1
	 return ;
}

//son Map > Calibration
void CChildView6::OnButton103() 
{
	CModeSer13  ModeSer13;
	ModeSer13.DoModal();

}


//son Map > 4w Setup
void CChildView6::OnButton105() 
{
	CModeSub17 ModeSub17;
	ModeSub17.DoModal();
}


//son Map > Set1
void CChildView6::OnButton107() 
{
	CModeSet13 ModeSet13;
	ModeSet13.DoModal();
}

//son Map > Set4
void CChildView6::OnButton109() //sylee121115-1
{
	CModeSet15 ModeSet15;
	ModeSet15.DoModal();
}

//son Map > Set2
void CChildView6::OnButton110() //sylee121115-1
{
	CModeSet16 ModeSet16;
	ModeSet16.DoModal();
}

//son Map > Set3
void CChildView6::OnButton111() //sylee150407
{
	CModeSet19 ModeSet19;
	ModeSet19.DoModal();
}
