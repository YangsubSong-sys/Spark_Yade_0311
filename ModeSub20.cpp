// ModeSub20.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub20.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "FileSysInfo.h"
 
extern CSysInfo20  SysInfo20;
extern CFileSysInfo FileSysInfo01;

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

/////////////////////////////////////////////////////////////////////////////
// CModeSub20 dialog


CModeSub20::CModeSub20(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub20::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub20)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub20::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub20)
		// NOTE: the ClassWizard will add DDX and DDV calls here

		DDX_Control(pDX, IDC_COMBO01, m_combo1);

		DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollbar1);
		DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollbar2);
		DDX_Control(pDX, IDC_SCROLLBAR3, m_scrollbar3); 
		DDX_Control(pDX, IDC_SCROLLBAR4, m_scrollbar4); 

		DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);
		DDX_Control(pDX, IDC_LABEL3,  m_Label3);	
		DDX_Control(pDX, IDC_LABEL4,  m_Label4);
		DDX_Control(pDX, IDC_LABEL5,  m_Label5);	
		DDX_Control(pDX, IDC_LABEL6,  m_Label6);

		DDX_Control(pDX, IDC_LABEL1001,  m_Label1001);
		DDX_Control(pDX, IDC_LABEL1002,  m_Label1002);
		DDX_Control(pDX, IDC_LABEL1003,  m_Label1003);
		DDX_Control(pDX, IDC_LABEL1004,  m_Label1004);
		DDX_Control(pDX, IDC_LABEL1005,  m_Label1005);

		DDX_Control(pDX, IDC_LABEL1006,  m_Label1006);
		DDX_Control(pDX, IDC_LABEL1007,  m_Label1007);
		DDX_Control(pDX, IDC_LABEL1008,  m_Label1008);


		DDX_Control(pDX, IDC_BLOAD,  m_button1001);
		DDX_Control(pDX, IDC_BSAVE,  m_button1002);
		DDX_Control(pDX, IDC_OK,  m_button1003);
		DDX_Control(pDX, IDC_BUTTON1,  m_button1004);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub20, CDialog)
	//{{AFX_MSG_MAP(CModeSub20)
		// NOTE: the ClassWizard will add message map macros here

 
		ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub20 message handlers



BEGIN_EVENTSINK_MAP(CModeSub20, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub20) 	

    ON_EVENT(CModeSub20, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE) 
	ON_EVENT(CModeSub20, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub20, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE) 
	ON_EVENT(CModeSub20, IDC_BUTTON1,-600 /* Click */, OnClear1, VTS_NONE) 
 

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CModeSub20::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
 

    m_scrollbar1.SetScrollRange(1, 50000, FALSE);
	m_scrollbar2.SetScrollRange(1, 50000, FALSE);
	m_scrollbar3.SetScrollRange(1, 50000, FALSE);
	m_scrollbar4.SetScrollRange(1, 50000, FALSE);
 
    Onload();   

	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928

	A_Language() ;
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CModeSub20::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
/*
	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_ESCAPE)) {
		return TRUE;
	}
*/

 	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_RETURN)) {
 //	 	OnOK();
		return TRUE;
	}
 
	return CDialog::PreTranslateMessage(pMsg);
}

void CModeSub20::OnClickOk() 
{ 
 	FileSysInfo01.LoadSaveSub20(2);
 	CDialog::OnOK();
}
 

void CModeSub20::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
      m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 101));
 	  m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 102));
	  m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 106));
	  m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 107));
      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 108));
 	  m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 109));
	  m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 110));
	  m_Label1008.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 111));
  
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 112));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 113));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 114));
	  m_button1004.SetWindowText(LoadMessage(1,"TitleSetupAutoCount", 115));

  }

 

}


void CModeSub20::OnSave() 
{ 
 	FileSysInfo01.LoadSaveSub20(1);
}
 


void CModeSub20::Onload() 
{

//    int i;
	CString str; 
//	char szText[100];

	FileSysInfo01.LoadSaveSub20(2);

	m_combo1.SetCurSel(SysInfo20.m_nUse);
 
/*	
    m_slider1.SetPos(SysInfo20.m_nPass);
	m_slider2.SetPos(SysInfo20.m_nOpen);
	m_slider3.SetPos(SysInfo20.m_nShort);
	m_slider4.SetPos(SysInfo20.m_nOpenShort);
*/

	m_scrollbar1.SetScrollPos(SysInfo20.m_nPass);
	m_scrollbar2.SetScrollPos(SysInfo20.m_nOpen);
	m_scrollbar3.SetScrollPos(SysInfo20.m_nShort);
	m_scrollbar4.SetScrollPos(SysInfo20.m_nOpenShort);

	//	i=  SysInfo20.m_nPinTot*4096;

	str.Format("%d", SysInfo20.m_nPass);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfo20.m_nOpen);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfo20.m_nShort);
	m_Label3.SetCaption(str);	
	str.Format("%d", SysInfo20.m_nOpenShort);
	m_Label4.SetCaption(str);
  	str.Format("%d", SysInfo20.m_nFail);
	m_Label5.SetCaption(str);	
	str.Format("%d", SysInfo20.m_nTotle);
	m_Label6.SetCaption(str);




}

void CModeSub20::OnSelchangeCombo1() 
{
    SysInfo20.m_nUse = m_combo1.GetCurSel();
}

/*
void CModeSub20::OnChangeSlider1()	
{ 
 	int i;
	CString strValue; 
 
 	i=m_slider1.GetPos(); 
 	m_slider1.SetPos(i);	
	SysInfo20.m_nPass=i;	
    strValue.Format("%d ", i);
	m_Label1.SetCaption(strValue);  
}


void CModeSub20::OnChangeSlider2()	
{
    int i;
	CString strValue; 
 
	i=m_slider2.GetPos(); 
	m_slider2.SetPos(i);	
	SysInfo20.m_nOpen=i;	
    strValue.Format("%d", i);
	m_Label2.SetCaption(strValue);  
} 

void CModeSub20::OnChangeSlider3()	
{ 
 	int i;
	CString strValue; 
 
	i=m_slider3.GetPos(); 
	m_slider3.SetPos(i);	
	SysInfo20.m_nShort=i;	
    strValue.Format("%d ", i);
	m_Label3.SetCaption(strValue);  
}

void CModeSub20::OnChangeSlider4()	
{
    int i;
	CString strValue; 
 
	i=m_slider4.GetPos(); 
	m_slider4.SetPos(i);	
	SysInfo20.m_nOpenShort=i;	
    strValue.Format("%d", i);
	m_Label4.SetCaption(strValue);  
}
*/


void CModeSub20::OnClear1()	
{
    int i;
	CString strValue; 
 
	i=0; 	




	SysInfo20.m_nPass=0;	
    strValue.Format("%d", i);
	m_Label1.SetCaption(strValue);  


	SysInfo20.m_nOpen=0;	
    strValue.Format("%d", i);
	m_Label2.SetCaption(strValue);  


	SysInfo20.m_nShort=0;	
    strValue.Format("%d", i);
	m_Label3.SetCaption(strValue);  


	SysInfo20.m_nOpenShort=0;	
    strValue.Format("%d", i);
	m_Label4.SetCaption(strValue);  

  
 	SysInfo20.m_nFail=0;	
    strValue.Format("%d", i);
	m_Label5.SetCaption(strValue);  
  	
	SysInfo20.m_nTotle=0;	
    strValue.Format("%d", i);
	m_Label6.SetCaption(strValue);  
 


}



/*
void CModeSub20::OnClear2()	
{
    int i;
	CString strValue; 
 

}
*/




void CModeSub20::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nScrollPos;
	CString strValue;
	
//	int ka,i,j;
	CString str;

	
	if(pScrollBar->GetSafeHwnd() == m_scrollbar1.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfo20.m_nPass=nScrollPos;
				 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <10000){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfo20.m_nPass=nScrollPos;				 
			}
			break;

	     case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				strValue.Format("%d", nPos);
				m_Label1.SetCaption(strValue);
				SysInfo20.m_nPass=nPos;	 		
			break;
			}

	/*		ka=m_scrollbar2.GetScrollPos();
			SysInfoPiece.m_nPiece=ka*SysInfoPiece.m_nRow;
			*/


			m_scrollbar1.SetScrollPos(SysInfo20.m_nPass);
			strValue.Format("%d", SysInfo20.m_nPass);
	        m_Label1.SetCaption(strValue);	
		/*	
            i=SysInfoPiece.m_nRow ;
			j= SysInfoPiece.m_nCol ;
			i=i+1;	
			m_grid1.SetRows(i); 
			for ( j = 0 ; j < i-1; j++) 
			{ 
					m_grid1.SetCol(0); 
					m_grid1.SetRow(j+1); 
					str.Format("No. %d", j+1); 
					m_grid1.SetText(str); 
			} 
			*/


 

	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar2.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfo20.m_nOpen=nScrollPos;
 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <10000){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfo20.m_nOpen=nScrollPos;
 
			}
			break;

	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label2.SetCaption(strValue);
			SysInfo20.m_nOpen=nPos;
 
			break;
		}

/*		ka=m_scrollbar1.GetScrollPos();
		SysInfoPiece.m_nPiece=ka*SysInfoPiece.m_nCol;
		m_scrollbar3.SetScrollPos(SysInfoPiece.m_nPiece);
		strValue.Format("%d", SysInfoPiece.m_nPiece);
	    m_Label3.SetCaption(strValue);	
		
       // i= SysInfoPiece.m_nRow ;
		j= SysInfoPiece.m_nCol ;
		j=j+1;	
	//	m_grid1.SetRows(i); 
	     m_grid1.SetCols(j); 	 
		for ( i = 0 ; i < j-1; i++) 
		{ 
		    m_grid1.SetRow(0); 
            m_grid1.SetCol(i+1);
			str.Format("No. %d", i+1); 
			m_grid1.SetText(str); 
		} 
//        Invalidate();
*/



	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar3.GetSafeHwnd()){
	 	switch(nSBCode){
		case SB_LINELEFT: 		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfo20.m_nShort=nScrollPos;
				Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <10000){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfo20.m_nShort=nScrollPos;
				Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label3.SetCaption(strValue);
			SysInfo20.m_nShort=nPos;
	 		Invalidate();
			break;
		}
     

   
	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar4.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label4.SetCaption(strValue);
				SysInfo20.m_nOpenShort=nScrollPos;
				 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <10000){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label4.SetCaption(strValue);
				SysInfo20.m_nOpenShort=nScrollPos;				 
			}
			break;

	     case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				strValue.Format("%d", nPos);
				m_Label4.SetCaption(strValue);
				SysInfo20.m_nOpenShort=nPos;	 		
			break;
			}


	 
		//	ka=m_scrollbar2.GetScrollPos();
		//	SysInfoPiece.m_nPiece=ka*SysInfoPiece.m_nRow;
			m_scrollbar3.SetScrollPos(SysInfo20.m_nOpenShort);
			strValue.Format("%d", SysInfo20.m_nOpenShort);
	        m_Label3.SetCaption(strValue);	
		/*	
            i=SysInfoPiece.m_nRow ;
			j= SysInfoPiece.m_nCol ;
			i=i+1;	
			m_grid1.SetRows(i); 
			for ( j = 0 ; j < i-1; j++) 
			{ 
					m_grid1.SetCol(0); 
					m_grid1.SetRow(j+1); 
					str.Format("No. %d", j+1); 
					m_grid1.SetText(str); 
			} 

			*/

 
 
	 




 





	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


