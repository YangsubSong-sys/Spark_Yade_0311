// MyCtrlBar.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "MyCtrlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCtrlBar

CMyCtrlBar::CMyCtrlBar()
{
}

CMyCtrlBar::~CMyCtrlBar()
{
}


BEGIN_MESSAGE_MAP(CMyCtrlBar, CControlBar)
	//{{AFX_MSG_MAP(CMyCtrlBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCtrlBar message handlers


void CMyCtrlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	pTarget->ShowWindow(SW_SHOW);
}

CSize CMyCtrlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize size;
	CFrameWnd* pMain = (CFrameWnd*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		CRect Rect;
		pMain->GetWindowRect(&Rect);
		size.cx = 200;
		size.cy = Rect.bottom;
	}

	return size;
}

int CMyCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT Rect;
	GetClientRect(&Rect);

	m_wndChildLeft.Create(IDD_Dialog_LeftBar, this);
//SYLEE 	SetBarStyle(CBRS_LEFT);


	::SetWindowPos(m_wndChildLeft.m_hWnd, HWND_TOP,
		Rect.top, Rect.left,
//		Rect.right,	Rect.bottom,
		Rect.right,	1000,
		SWP_SHOWWINDOW);	

	return 0;
}
