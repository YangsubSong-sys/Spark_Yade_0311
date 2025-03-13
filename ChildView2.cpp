// ChildView2.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ChildView2.h"
#include "ModeSet12.h"
#include "ModeSet13.h"
#include "FileSysInfo.h"
#include "PassWord1.h"//sylee180731-1
#include "A_Define1.h"	//son190628


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
extern int nFlag_PassOnNo, nFlag_PassOn;//sylee240416

extern CSysInfo05  SysInfo05;  //sylee160616

extern CSysSet13  SysSet13; 
extern int nAutoProcess1;

extern int g_nLogOnRecipe1;//slee130913

extern int gDef_Language1; //SYLEE151028   //1=KOREA ,2=ENGLISH, 3=CHINA
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum);//sylee151028-1



extern CFileSysInfo  FileSysInfo01; 
extern int  nFlagFileLoadok ;//sylee160519

 extern CSysSet211  SysSet211;  //Pin SETUP   

/////////////////////////////////////////////////////////////////////////////
// CChildView2

IMPLEMENT_DYNCREATE(CChildView2, CFormView)

CChildView2::CChildView2()
	: CFormView(CChildView2::IDD)
{
	//{{AFX_DATA_INIT(CChildView2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CChildView2::~CChildView2()
{
}

void CChildView2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
			DDX_Control(pDX, IDC_BUTTON1003, m_button1003); 
		DDX_Control(pDX, IDC_BUTTON1004, m_button1004); 
		DDX_Control(pDX, IDC_BUTTON1010, m_button1010); 
		DDX_Control(pDX, IDC_BUTTON1001, m_button1001); 
		DDX_Control(pDX, IDC_BUTTON1007, m_button1007);
		DDX_Control(pDX, IDC_BUTTON1002, m_button1002); 
		DDX_Control(pDX, IDC_BUTTON1011, m_button1011);
		DDX_Control(pDX, IDC_BUTTON1008, m_button1008); 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildView2, CFormView)
	//{{AFX_MSG_MAP(CChildView2)
	  // NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_ERASEBKGND()



	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView2 diagnostics

#ifdef _DEBUG
void CChildView2::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildView2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildView2 message handlers

void CChildView2::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
//	CFormView::PostNcDestroy();
}


BOOL CChildView2::OnEraseBkgnd(CDC* pDC)    
{
	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));

//	DrawGradation(pDC, RGB(221, 236, 255), RGB(86, 125, 204));//하늘색 
//  DrawGradation(pDC, RGB(0, 0, 0), RGB(221, 236, 255));  //검은색  -> 검흰색 
//	DrawGradation(pDC, RGB(255, 255, 255), RGB(221, 236, 255));  //흰색   -> 흰색하늘색  
//	DrawGradation(pDC, RGB(254, 242, 216), RGB(252, 219, 143));  //황토색
  
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

void CChildView2::DrawGradation(CDC *pDC, COLORREF rgbBegin, COLORREF rgbEnd)  
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

//CBiOpTroConApp *pApp=(CBiOpTroConApp *)AfxGetApp();
//CWinApp *pApp=(CWinApp *)AfxGetApp();
 

BEGIN_EVENTSINK_MAP(CChildView2, CFormView)
    //{{AFX_EVENTSINK_MAP(CChildView2)
	ON_EVENT(CChildView2, IDC_BUTTON1001, -600 /* Click */, OnClickButton1001, VTS_NONE)
	ON_EVENT(CChildView2, IDC_BUTTON1002, -600 /* Click */, OnClickButton1002, VTS_NONE)
	ON_EVENT(CChildView2, IDC_BUTTON1003, -600 /* Click */, OnClickButton1003, VTS_NONE)
	ON_EVENT(CChildView2, IDC_BUTTON1004, -600 /* Click */, OnClickButton1004, VTS_NONE)
 
	ON_EVENT(CChildView2, IDC_BUTTON1007, -600 /* Click */, OnClickButton1007, VTS_NONE)
	ON_EVENT(CChildView2, IDC_BUTTON1008, -600 /* Click */, OnClickButton1008, VTS_NONE)    //son 4W Manual
 
	ON_EVENT(CChildView2, IDC_BUTTON1010, -600 /* Click */, OnClickButton1010, VTS_NONE)
	ON_EVENT(CChildView2, IDC_BUTTON1011, -600 /* Click */, OnClickButton1011, VTS_NONE)
 
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CChildView2::OnInitialUpdate() 
{
      CFormView::OnInitialUpdate();

	  FileSysInfo01.LoadSaveSet211(2);//load //sylee160928

      A_Language();

}


void CChildView2::OnClickButton1001() 
{
#ifdef __GTS__CUSTOMER  //sylee240416        
        nFlag_PassOnNo=14; //sylee240416  
        CPassWord1 Dlg1;
        Dlg1.DoModal();
        if (nFlag_PassOn != 2) // 
        {
            nFlag_PassOn=0;
            AfxMessageBox("     PASSWORD  NG !!!     \n     ");
            return ;
        }         
#endif
	CModeSub11 Dlg;
	Dlg.DoModal(); 	
}
void CChildView2::OnClickButton1002() 
{

  
	//sylee161108-1 if(	SysSet13.m_nSet4==1 ){//sylee160928  //YAMAHA  //sylee161108-1
	//sylee160928  //YAMAHA  //sylee161108-1   0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA  
	//if(	SysSet13.m_nPinBlockType==5) {
	if(	SysSet13.m_nPinBlockType == PIN_BLK_YAMAHA) {	//son-32KPIN-20190902
	   	AfxMessageBox("       Not use!   ->    YAMAHA Type Pinblock  ", MB_OK);
		return ;
	}

	CModeSet12 Dlg;
	Dlg.DoModal(); 	
}




void CChildView2::A_Language() //sylee151028 //LANGUAGE CHANGE
{

   if(SysSet211.m_nSet13==1){//sylee160907-1   //chinese
       	gDef_Language1=3;//redboard
	}else{//sylee160907-1
        gDef_Language1=2; //englishos
	}//sylee160907-1
	
	
 
  if( gDef_Language1==3){//sylee151028-1    
	   m_button1003.SetWindowText(LoadMessage(1,"TitleSetup", 101));     
	   m_button1004.SetWindowText(LoadMessage(1,"TitleSetup", 102)); 
	   m_button1010.SetWindowText(LoadMessage(1,"TitleSetup", 103));     
	   m_button1001.SetWindowText(LoadMessage(1,"TitleSetup", 104)); 
	   m_button1007.SetWindowText(LoadMessage(1,"TitleSetup", 105));     
	   m_button1002.SetWindowText(LoadMessage(1,"TitleSetup", 106)); 
	   m_button1011.SetWindowText(LoadMessage(1,"TitleSetup", 107));     
	   m_button1008.SetWindowText(LoadMessage(1,"TitleSetup", 108));  
  }

}



extern int nFlag_PassOn;//sylee160219-3	
extern int nFlag_PassOnNo;//sylee160219-3	

void CChildView2::OnClickButton1003()  //son 'Recipe 버튼 클릭'
{

//JOYTECH_DEFINE	g_nLogOnRecipe1=7;//test	
	nFlag_PassOnNo=0;//sylee160219-3	
	if( (SysSet13.m_nSet17==1)          //son Map > Set1 > 'Recipe Password Use'  On 
	        && (SysSet13.m_nSet6!=1))   //son Map > Set1 > 'No Password' Off
	{
	    //son C:\ACE400\Data\Message_L.ini의 101 번 항목의 passwd
		nFlag_PassOnNo=11;//sylee160219-3
	    CPassWord1 Dlg;//sylee180731-1
		Dlg.DoModal(); 	
	}
//sylee180731-1	if( SysSet13.m_nSet17!=1 || g_nLogOnRecipe1==7){//SYLEE170605//sylee170719
	if( (SysSet13.m_nSet17==1 && nFlag_PassOn==2) || (SysSet13.m_nSet6==1) || ( SysSet13.m_nSet17!=1 )){ //sylee160219-3
		nFlag_PassOn=0;
		CModeSub15 Dlg;
		Dlg.DoModal(); 
	}else{
	 	AfxMessageBox("  Log In.  Please !  ", MB_OK );
	}
	g_nLogOnRecipe1=0;
 
}


void CChildView2::OnClickButton1004() 
{
	CModeSub13 Dlg;
	Dlg.DoModal(); 	
}


void CChildView2::OnClickButton1007() 
{
	CModeSub19 Dlg;
	Dlg.DoModal(); 	
}

//son '4w Setup' 설정 창
void CChildView2::OnClickButton1008() 
{   	


    if(nFlagFileLoadok !=1){//sylee160518		
        AfxMessageBox("  ERROR 2122,   .   First,    File  Open!.     ", MB_OK);	
        return;
    }


    FileSysInfo01.LoadSaveSub17(2); //4w.ini load  //sylee160518 

    //int	dutIdx = getDutId()-1; 		// g_s4WNet의 index. 0부터 시작함.	//son190704-4WDUT16
    int	dutIdx = get4wDutIdx(); 		// g_s4WNet의 index. 0부터 시작함.	//son190704-4WDUT16

    if(g_s4WNet[dutIdx].wCount>=0) //sylee160518 //son190704-4WDUT16  //son241009
    { 

        nFlag_PassOnNo=0;//sylee160219-3	
        if( (SysSet13.m_nSet17==1)          //son Map > Set1 > 'Recipe Password Use'  On
                && (SysSet13.m_nSet6!=1))   //son Map > Set1 > 'No Password' Off
        {
            //son C:\ACE400\Data\Message_L.ini의 102 번 항목의 passwd
            nFlag_PassOnNo=12;//sylee160219-3
            CPassWord1 Dlg;//sylee180731-1
            Dlg.DoModal(); 	
        }
        //sylee180731-1	  if( SysSet13.m_nSet17!=1 || g_nLogOnRecipe1==7){//sylee170728-1
        if( (SysSet13.m_nSet17==1 && nFlag_PassOn==2) || (SysSet13.m_nSet6==1) || ( SysSet13.m_nSet17!=1 )){ //sylee160219-3
            nFlag_PassOn=0;
            CModeSub17 Dlg;
            Dlg.DoModal(); 
        }
        else{//sylee170728-1
            AfxMessageBox("  Log In.  Please !  ", MB_OK );
        }
        g_nLogOnRecipe1=0;//sylee170728-1

    }
    else //sylee160518
    { 
        CString strTemp, fName4w;

        if (dutIdx == 0)
            fName4w.Format("4W.ini");
        else
            fName4w.Format("4W.ini%d", dutIdx+1);

        strTemp.Format("  ERROR 2121,   . \"%s\" file  loading fail !   (net total count >=0?) ", fName4w);
        AfxMessageBox(strTemp, MB_OK);			

    } //sylee160518

}

 
/*
void CChildView2::OnClickButton1009() 
{
	//CModeSub12 Dlg;
//	Dlg.DoModal();  

 
 	CBiOpTroConApp *pApp=(CBiOpTroConApp *)AfxGetApp();  
	pApp->OnFilePrintSetup1();


}
*/

void CChildView2::OnClickButton1010() 
{

#ifdef __GTS__CUSTOMER  //sylee240416        
        nFlag_PassOnNo=14; //sylee240416  
        CPassWord1 Dlg1;
        Dlg1.DoModal();
        if (nFlag_PassOn != 2) // 
        {
            nFlag_PassOn=0;
            AfxMessageBox("     PASSWORD  NG !!!     \n     ");
            return;
        }         
#endif

	CModeSub25 Dlg;
	Dlg.DoModal(); 	
}


void CChildView2::OnClickButton1011() 
{
	CModeSub20 Dlg;
	Dlg.DoModal(); 	
}
