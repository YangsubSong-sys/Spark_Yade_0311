// ChildView4.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView4.h"
#include "FileSysInfo.h"
// #include "Gobal.h"

extern int nView1;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSysInfoView01  SysInfoView01;

extern nAutoProcess1;

/////////////////////////////////////////////////////////////////////////////
// CChildView4

IMPLEMENT_DYNCREATE(CChildView4, CFormView)

CChildView4::CChildView4()
	: CFormView(CChildView4::IDD)
{
	//{{AFX_DATA_INIT(CChildView4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CChildView4::~CChildView4()
{
}

void CChildView4::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView4)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LABEL1,  m_Label1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView4, CFormView)
	//{{AFX_MSG_MAP(CChildView4)
		ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CChildView4, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView4)

	ON_EVENT(CChildView4, IDC_BUTTON1001, -600 /* Click */, OnButton1001, VTS_NONE)
 	ON_EVENT(CChildView4, IDC_BUTTON1002, -600 /* Click */, OnButton1002, VTS_NONE)

 	
//    ON_EVENT(CChildView4, IDC_BUTTON1001, -600 /* Click */, OnClickButton1001, VTS_NONE)/
//	ON_EVENT(CChildView4, IDC_BUTTON1002, -600 /* Click */, OnClickButton1002, VTS_NONE)
//	ON_EVENT(CChildView4, IDC_BUTTON1003, -600 /* Click */, OnClickButton1003, VTS_NONE)
//	ON_EVENT(CChildView4, IDC_BUTTON1004, -600 /* Click */, OnClickButton1004, VTS_NONE)
// 	ON_EVENT(CChildView4, IDC_BUTTON1005, -600 /* Click */, OnClickButton1005, VTS_NONE)

 

//	ON_EVENT(CChildView4, IDC_BUTTON1007, -600 /* Click */, OnClickButton1007, VTS_NONE)
//	ON_EVENT(CChildView4, IDC_BUTTON1008, -600 /* Click */, OnClickButton1008, VTS_NONE)
 //	ON_EVENT(CChildView4, IDC_BUTTON1009, -600 /* Click */, OnClickButton1009, VTS_NONE)
//	ON_EVENT(CChildView4, IDC_BUTTON1010, -600 /* Click */, OnClickButton1010, VTS_NONE)
//	ON_EVENT(CChildView4, IDC_BUTTON1011, -600 /* Click */, OnClickButton1011, VTS_NONE)

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



/////////////////////////////////////////////////////////////////////////////
// CChildView4 diagnostics


#ifdef _DEBUG
void CChildView4::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView4::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView4 message handlers


BOOL CChildView4::OnEraseBkgnd(CDC* pDC)    
{
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));

//	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));//ÇÏ´Ã»ö 
// DrawGradation(pDC, RGB(0, 0, 0), RGB(221, 236, 255));  //°ËÀº»ö  -> °ËÈò»ö 
//	DrawGradation(pDC, RGB(255, 255, 255), RGB(221, 236, 255));  //Èò»ö   -> Èò»öÇÏ´Ã»ö  
//	DrawGradation(pDC, RGB(254, 242, 216), RGB(252, 219, 143));  //È²Åä»ö
  
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


void CChildView4::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
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

/*
void CChildView4::OnClickButton1006() 
{
	CModeView16 Dlg;
	Dlg.DoModal(); 	
}
*/



void CChildView4::OnButton1001() //open
{
 
	OnDataLoad();



}


void CChildView4::OnButton1002() //open
{
  int i;
  i=9;
 	CFileDialog dlg(TRUE,"\\LOG\\ERROR\\","*.*");

	if(dlg.DoModal()==IDOK)
	{
		CFile file;
//		FILE *fp;	 
		if ((file.Open(dlg.GetFileName(), CFile::modeRead))==NULL) {
			AfxMessageBox("  Error No 1018,    Open File Reading  !.", MB_OK);
			return;
		}


 
		char buffer[65535];
		::ZeroMemory(buffer, sizeof(buffer));  
		file.Read(buffer, 65534);	
		file.Close();
		SetDlgItemText(IDC_DATA, buffer);
 
 
	//	ShellExecute(NULL,"open","notepad.exe",dlg.GetFileName(),NULL,SW_SHOWNORMAL); 

	}	

 
	char  szTemp1[100] ;
	char  m_Folder[200];	 
 
 
	::ZeroMemory(&szTemp1, sizeof(szTemp1));
	::ZeroMemory(&m_Folder, sizeof(m_Folder));
 
 	strcat( szTemp1 , "\\" );
 
    strcat( m_Folder, dlg.GetFileName());
	strcat( szTemp1, m_Folder );

	CString str;  
 	str.Format("%s", szTemp1 );
	m_Label1.SetCaption(str);

   //Print1.OnStartPrinter();//SYLEETEST

}
 




void CChildView4::OnDataLoad()  //open button
{		
	
//	CFileDialog dlg(TRUE);

	CFileDialog dlg(TRUE,"\\RESULT","*.*");

	if(dlg.DoModal()==IDOK)
	{
		CFile file;
 	 
		if ((file.Open(dlg.GetFileName(), CFile::modeRead))==NULL) {
			AfxMessageBox("  Error No 1019, Open File Reading   !.", MB_OK);
			return;
		}


 
		char buffer[65535];
		::ZeroMemory(buffer, sizeof(buffer));  
		file.Read(buffer, 65534);	
		file.Close();
		SetDlgItemText(IDC_DATA, buffer);
 
 
	//	ShellExecute(NULL,"open","notepad.exe",dlg.GetFileName(),NULL,SW_SHOWNORMAL); 

	}	

 
	char  szTemp1[100] ;
	char  m_Folder[200];	 
 
 
	::ZeroMemory(&szTemp1, sizeof(szTemp1));
	::ZeroMemory(&m_Folder, sizeof(m_Folder));
 
 	strcat( szTemp1 , "\\" );
 
    strcat( m_Folder, dlg.GetFileName());
	strcat( szTemp1, m_Folder );

	CString str;  
 	str.Format("%s", szTemp1 );
	m_Label1.SetCaption(str);


}

 
 

void CChildView4::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	int i;
	i=9;
	
}

BOOL CChildView4::PreTranslateMessage(MSG* pMsg) 
{

//	char buffer[65535];
	// TODO: Add your specialized code here and/or call the base class
	if(nView1==4){
		nView1=0;

	//	::ZeroMemory(buffer, sizeof(buffer));  
 
    //SetDlgItemText(IDC_DATA, buffer);
	SetDlgItemText(IDC_DATA, "");

	CString str;  
 	//str.Format("%s", szTemp1 );
	str.Format(" " );
	m_Label1.SetCaption(str);



	}


	return CFormView::PreTranslateMessage(pMsg);
}
