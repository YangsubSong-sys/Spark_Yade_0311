// ModeView16.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeView16.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDC_RICHEDIT                    101

/////////////////////////////////////////////////////////////////////////////
// CModeView16 dialog


CModeView16::CModeView16(CWnd* pParent /*=NULL*/)
	: CDialog(CModeView16::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeView16)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeView16::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeView16)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeView16, CDialog)
	//{{AFX_MSG_MAP(CModeView16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeView16 message handlers


BEGIN_EVENTSINK_MAP(CModeView16, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeView16)
	ON_EVENT(CModeView16, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeView16, IDC_Print, -600 /* Click */, OnClickPrint, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
 
 


void PrintBitmap(LPCTSTR filename) {

 CPrintDialog printDlg(FALSE);
 printDlg.GetDefaults(); 
 // Or get from user:
 // if (printDlg.DoModal() == IDCANCEL)   
 //        return; 
 CDC dc;
 if (!dc.Attach(printDlg.GetPrinterDC())) {
  AfxMessageBox(_T("No printer found!")); return;
 } 
 
 dc.m_bPrinting = TRUE; 
 DOCINFO di;    
 // Initialise print document details
 ::ZeroMemory (&di, sizeof (DOCINFO));
 di.cbSize = sizeof (DOCINFO);
 di.lpszDocName = filename; 
 BOOL bPrintingOK = dc.StartDoc(&di); // Begin a new print job 
 // Get the printing extents
 // and store in the m_rectDraw field of a 
 // CPrintInfo object
 CPrintInfo Info;
 Info.SetMaxPage(1); // just one page 
 int maxw = dc.GetDeviceCaps(HORZRES);
 int maxh = dc.GetDeviceCaps(VERTRES); 
 Info.m_rectDraw.SetRect(0, 0, maxw, maxh); 
 for (UINT page = Info.GetMinPage(); page <= 
      Info.GetMaxPage() && bPrintingOK; page++) {
  dc.StartPage();    // begin new page
  Info.m_nCurPage = page;
  CBitmap bitmap;
  // LoadImage does the trick here, it creates a DIB section
  // You can also use a resource here
  // by using MAKEINTRESOURCE() ... etc. 
  if(!bitmap.Attach(::LoadImage(
   ::GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, 
   LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE))) {
    AfxMessageBox(_T("Error loading bitmap!")); return;
   } 
   BITMAP bm;
   bitmap.GetBitmap(&bm);
   int w = bm.bmWidth; 
   int h = bm.bmHeight; 
   // create memory device context
   CDC memDC; 
   memDC.CreateCompatibleDC(&dc);
   CBitmap *pBmp = memDC.SelectObject(&bitmap);
   memDC.SetMapMode(dc.GetMapMode());
   dc.SetStretchBltMode(HALFTONE);
   // now stretchblt to maximum width on page


   //w=728*2;h=984*2;  test  ��� �ǳ�   ���� ����   black
   //w=728;h=984; old 
   dc.StretchBlt(0, 0, maxw, maxh, &memDC, 0, 0, w, h, SRCCOPY); 
   // clean up
   memDC.SelectObject(pBmp);
   bPrintingOK = (dc.EndPage() > 0);   // end page
 } 
 if (bPrintingOK)
   dc.EndDoc(); // end a print job
 else dc.AbortDoc();     // abort job. 
}

// CPrintDialog�� �̿��� �������� ����Ʈ ������ �о�´�.	CPrintDialog dlg(TRUE, PD_RETURNDEFAULT);	// DoModal�� �����ϸ� ������ ���̾�αװ� ���ߵ����� ���ڷ� PD_RETURNDEFAULT��	// �־��� ������ ������ ������ �ٷ� �����Ѵ�.	dlg.DoModal();	// ���̸� ���� �������� ��� �ʹٵ��� ���� �Ÿ� ��� �ʹٸ� �Ʒ��� �ڵ带 �����Ͽ�	// ����Ѵ�. DEVMODE ����ü�� �����غ��� �ٶ���. ���� ���� ���� �������� �����ϴ� ���̴�.	DEVMODE *pDevMode = (DEVMODE *)::GlobalLock(dlg.m_pd.hDevMode);	if (pDevMode) 	{        	// �������� �μ� ���� ������ �����Ѵ�.		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;		// 3�Ÿ� �ﵵ�� �Ѵ�.		pDevMode->dmCopies = 3;		::GlobalUnlock(dlg.m_pd.hDevMode);	}	CDC dc;	// ������ ������ dc ������ �ݿ��Ѵ�.	if (dc.Attach(dlg.CreatePrinterDC())) 	{		DOCINFO di;		// ���� �����Ǵ� ����Ʈ ���� �̸��� �ش�.		memset(&di, 0x00, sizeof(DOCINFO));		di.cbSize = sizeof(DOCINFO);		di.lpszDocName = "InstPhoto Output";		// ����Ʈ ���� �ϳ� ����.		dc.StartDoc(&di);		dc.StartPage();		// dc�� ���ϴ� ����� �����Ѵ�.		....		// ����� ����Ǿ����� ������ �۾��� �Ѵ�.		dc.EndPage();		dc.EndDoc();		dc.DeleteDC();




void CModeView16::OnClickOk() 
{
  
	CDialog::OnOK();

}

void CModeView16::OnDraw(CDC* pDC)
{

//	CFrameWnd* pFrame ;
		           //CWiew* pFrame ;	
 //	CBiOpTroConDoc* pDoc = GetDocument();
 //	ASSERT_VALID(pDoc);

// 	CBiOpTroConDoc *pDoc =(CBiOpTroConDoc*)pFrame->GetActiveDocument(); //CDocument* CFrameWnd::GetActiveDocument( ); 
//    ASSERT_VALID(pDoc);


	pDC->SetMapMode(MM_LOMETRIC);
	//����� Solid �귯�ø� �����Ѵ�.
	CBrush* pNewBrush = new CBrush(RGB(0, 255, 0));
	CBrush* pOldBrush = NULL;

	pOldBrush = pDC->SelectObject(pNewBrush);
	//������� ĥ���� �׸� �׷�����.
	pDC->Rectangle(CRect(10, -10, 1000, -1000));

	pDC->SelectObject(pOldBrush);
	delete pNewBrush;
}


void CModeView16::OnClickPrint() 
{ 
//::ShellExecute(NULL, "print" ,"1.txt",NULL,NULL,SW_SHOWNORMAL); 
//	::ShellExecute(NULL, "print" ,"1.txt",NULL,NULL,SW_HIDE); 
::ShellExecute(NULL, "print" ,"cmd","0.txt",NULL,SW_HIDE); 
/*  //ok 1
 char *a;  
 a="1.bmp";
 PrintBitmap(a);  // GRAPHIC  PRINT
*/
  
 /*//OnDraw(CDC* pDC);


  //OnFilePrintSetup();

 //	CBiOpTroConApp *pApp=(CBiOpTroConApp *)AfxGetApp();  
 //	pApp->OnFilePrintSetup1();
*/

 /* //test1
 // CPrintDialog�� �̿��� �������� ����Ʈ ������ �о�´�.
 CPrintDialog dlg(TRUE, PD_RETURNDEFAULT);	
 // DoModal�� �����ϸ� ������ ���̾�αװ� ���ߵ����� ���ڷ� PD_RETURNDEFAULT��	
 // �־��� ������ ������ ������ �ٷ� �����Ѵ�.
 dlg.DoModal();	
 // ���̸� ���� �������� ��� �ʹٵ��� ���� �Ÿ� ��� �ʹٸ� �Ʒ��� �ڵ带 �����Ͽ�
// ����Ѵ�. DEVMODE ����ü�� �����غ��� �ٶ���. ���� ���� ���� �������� �����ϴ� ���̴�.
 DEVMODE *pDevMode = (DEVMODE *)::GlobalLock(dlg.m_pd.hDevMode);
 if (pDevMode) 	
 {
	 // �������� �μ� ���� ������ �����Ѵ�.
	 pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	 // 3�Ÿ� �ﵵ�� �Ѵ�.
	 pDevMode->dmCopies = 1;  //3;
	 ::GlobalUnlock(dlg.m_pd.hDevMode);
}	

     CDC dc;
     // ������ ������ dc ������ �ݿ��Ѵ�.
	if (dc.Attach(dlg.CreatePrinterDC())) 
	{	
		DOCINFO di;

		// ���� �����Ǵ� ����Ʈ ���� �̸��� �ش�.
		memset(&di, 0x00, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = "InstPhoto Output";
		di.lpszOutput="1";
		// ����Ʈ ���� �ϳ� ����.
		dc.StartDoc(&di);
		dc.StartPage();
		// dc�� ���ϴ� ����� �����Ѵ�.
		// ����� ����Ǿ����� ������ �۾��� �Ѵ�.	
		dc.EndPage();	
		dc.EndDoc();		
		dc.DeleteDC();
	}

  */  //test1


	//test2
/*

	CString file=L"/select,";
file= file + _mFilePath
ShellExecute(NULL,L"open",L"explorer",file,NULL,SW_SHOWNORMAL);
*/







}



/*

CBiOpTroConDoc* CModeView16::GetDocument() // non-debug version is inline
{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBiOpTroConDoc)));
	return (CBiOpTroConDoc*)m_pDocument;
}
*/ 

BOOL CModeView16::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CRect rect;
	GetClientRect(&rect);

 	rect.left += 10;
 	rect.top += 80;
 	rect.right -= 150;
 	rect.bottom -= 10;

	m_RichEditCtrl.Create(
		WS_CHILD | WS_VISIBLE | WS_TABSTOP |
		WS_VSCROLL | WS_HSCROLL |
		ES_MULTILINE | ES_WANTRETURN | ES_SUNKEN |
		ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		rect,
		this,
		IDC_RICHEDIT);

 /*
	CFileDialog dlg(TRUE,"txt","*.txt");
	if(dlg.DoModal()==IDOK) {
		m_RichEditCtrl.ReadFile(dlg.GetPathName(), dlg.GetFileExt());
	}
 */
        char temp[MAX_PATH]; 
 
        GetCurrentDirectory(sizeof(temp),temp); 
	//  GetFileExt.Format("0  link0  link.txt" );
		CString  GetFileExt; 
	//	GetFileExt.Format("0  link" );
	//	strcopy(temp,GetFileExt);
	//	strncopy(temp2, "0  link", 7); 
         
	//	strcat(temp,"\\0  link.TXT"); 
        strcat(temp,"\\1.txt");
         
	    ShellExecute(NULL,"open","explorer",temp,NULL,SW_SHOWNORMAL);//temp
 
	    //ShellExecute(NULL,"open","wordpad.exe",temp,NULL,SW_SHOWNORMAL);  //ok only  folder name  no blank 
		//ShellExecute(NULL,"open","notepad.exe",temp,NULL,SW_SHOWNORMAL);  //ok only  folder name  no blank 
		//  ShellExecute(NULL,TEXT("open"),"wordpad.exe",temp,NULL,SW_SHOWNORMAL);  //ok only  folder name  no blank 
		
		//ShellExecute(NULL,"print","notepad.exe",temp,NULL,SW_HIDE);  //ok only  folder name  no blank 
		//WinExec(  Note.exe  , SW_SHOW);

 //     ::ShellExecute(NULL, "open" ,"mspaint.exe","1.bmp",NULL,SW_SHOWNORMAL); 
	//	ShellExecute(NULL, "open" ,"cmd",temp,NULL,SW_SHOWNORMAL);  //ok only  folder name  no blank 

	//	SendMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);

		GetFileExt.Format("txt" );
		m_RichEditCtrl.ReadFile(temp, GetFileExt);  

		m_RichEditCtrl.SetFocus();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
