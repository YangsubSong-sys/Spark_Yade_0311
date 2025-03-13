// BiOpTroConView.cpp : implementation of the CBiOpTroConView class
//

#include "stdafx.h"
#include "BiOpTroCon.h"

#include "BiOpTroConDoc.h"
#include "BiOpTroConView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConView

IMPLEMENT_DYNCREATE(CBiOpTroConView, CView)

BEGIN_MESSAGE_MAP(CBiOpTroConView, CView)
	//{{AFX_MSG_MAP(CBiOpTroConView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConView construction/destruction

CBiOpTroConView::CBiOpTroConView()
{
	// TODO: add construction code here
	g_pDefaultView = this;
}

CBiOpTroConView::~CBiOpTroConView()
{
}

BOOL CBiOpTroConView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConView drawing


void CBiOpTroConView::InitLoadFile()
{

	int i;

	i=0;


//JIG LOAD !!!!!!!
//SETUP FILE LOAD !!!!!!!

}



void CBiOpTroConView::OnDraw(CDC* pDC)
{
	CBiOpTroConDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	pDC->SetTextColor( RGB(0,0,255)); 
//	pDC->TextSize(20); 
//DEMO	pDC->TextOut(10, 10, "Welcome to BiOptro BBT TESTER");
		pDC->TextOut(10, 10, "Welcome to ACE400 BBT TESTER");

    InitLoadFile();
}








/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConView printing

BOOL CBiOpTroConView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBiOpTroConView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBiOpTroConView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConView diagnostics

#ifdef _DEBUG
void CBiOpTroConView::AssertValid() const
{
	CView::AssertValid();
}

void CBiOpTroConView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBiOpTroConDoc* CBiOpTroConView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBiOpTroConDoc)));
	return (CBiOpTroConDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConView message handlers
