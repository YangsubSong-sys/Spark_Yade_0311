// ChildView10.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView10.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView10

IMPLEMENT_DYNCREATE(CChildView10, CFormView)

CChildView10::CChildView10()
	: CFormView(CChildView10::IDD)
{
	//{{AFX_DATA_INIT(CChildView10)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CChildView10::~CChildView10()
{
}

void CChildView10::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView10)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView10, CFormView)
	//{{AFX_MSG_MAP(CChildView10)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView10 diagnostics

#ifdef _DEBUG
void CChildView10::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView10::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView10 message handlers
