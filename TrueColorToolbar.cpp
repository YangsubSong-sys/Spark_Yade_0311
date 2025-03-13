// TrueColorToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "TrueColorToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolbar

CTrueColorToolbar::CTrueColorToolbar()
{
}

CTrueColorToolbar::~CTrueColorToolbar()
{
}


BEGIN_MESSAGE_MAP(CTrueColorToolbar, CToolBar)
	//{{AFX_MSG_MAP(CTrueColorToolbar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolbar message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CTrueColorToolbar::OnEraseBkgnd(CDC* pDC) 
{
/*	CRect rect;
	GetClientRect(&rect);

	CBitmap Bitmap;

	Bitmap.LoadBitmap(IDB_Bitmap_Toolbar_Back);
	CBrush brush;
	brush.CreatePatternBrush(&Bitmap);
	pDC->FillRect(&rect, &brush);
*/
	return CToolBar::OnEraseBkgnd(pDC);
}
