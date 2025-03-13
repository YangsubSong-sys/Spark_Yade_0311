// ModeSub25.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub25.h"

#include "FileSysInfo.h"
 
extern CSysInfoPiece  SysInfoPiece;
extern CFileSysInfo FileSysInfo01;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

/////////////////////////////////////////////////////////////////////////////
// CModeSub25 dialog


CModeSub25::CModeSub25(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub25::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub25)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub25::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub25)
		// NOTE: the ClassWizard will add DDX and DDV calls here

	    DDX_Control(pDX, IDC_LABEL6,  m_Label6);//sylee150804-2
		DDX_Control(pDX, IDC_COMBO01, m_combo1);

		DDX_Control(pDX, IDC_GRID_1, m_grid1); 

		DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollbar1);
		DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollbar2);
		DDX_Control(pDX, IDC_SCROLLBAR3, m_scrollbar3); 
		DDX_Control(pDX, IDC_SCROLLBAR4, m_scrollbar4); 
		DDX_Control(pDX, IDC_SCROLLBAR5, m_scrollbar5); 

		DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);
		DDX_Control(pDX, IDC_LABEL3,  m_Label3);
		DDX_Control(pDX, IDC_LABEL4,  m_Label4);
		DDX_Control(pDX, IDC_LABEL5,  m_Label5);


		DDX_Control(pDX, IDC_EDIT1,   m_edit1);

		DDX_Text(pDX, IDC_EDIT2,   m_edit2);	
		DDX_Text(pDX, IDC_EDIT3,   m_edit3);

	    DDX_Control(pDX, IDC_LABEL1001,  m_Label1001);
		DDX_Control(pDX, IDC_LABEL1002,  m_Label1002);
		DDX_Control(pDX, IDC_LABEL1003,  m_Label1003);
 		DDX_Control(pDX, IDC_LABEL1004,  m_Label1004);
  		DDX_Control(pDX, IDC_LABEL1005,  m_Label1005);
		DDX_Control(pDX, IDC_LABEL1006,  m_Label1006);
		DDX_Control(pDX, IDC_LABEL1007,  m_Label1007); 
		DDX_Control(pDX, IDC_LABEL1008,  m_Label1008);
		DDX_Control(pDX, IDC_LABEL1009,  m_Label1009); 
  
		DDX_Control(pDX, IDC_BUTTON101,  m_button1001);
		DDX_Control(pDX, IDC_BLOAD,  m_button1002);
		DDX_Control(pDX, IDC_BSAVE,  m_button1003);
		DDX_Control(pDX, IDC_OK,  m_button1004);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub25, CDialog)
	//{{AFX_MSG_MAP(CModeSub25)
		// NOTE: the ClassWizard will add message map macros here
 	ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub25 message handlers



BEGIN_EVENTSINK_MAP(CModeSub25, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub25)
	ON_EVENT(CModeSub25, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_LABEL2, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSub25, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	ON_EVENT(CModeSub25, IDC_BUTTON101, -600 /* Click */, OnClickButton101, VTS_NONE)

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 

BOOL CModeSub25::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
	CString str; 


	InitControl();

	m_combo1.SetCurSel(SysInfoPiece.m_nType);
 
	m_scrollbar1.SetScrollRange(1, 100, FALSE);//sylee170327   20->100
	m_scrollbar2.SetScrollRange(1, 100, FALSE);//sylee170327   20->100
	m_scrollbar3.SetScrollRange(1, 400, FALSE);
	m_scrollbar4.SetScrollRange(0, 20, FALSE);
	m_scrollbar5.SetScrollRange(0, 20, FALSE);


	m_scrollbar1.SetScrollPos(SysInfoPiece.m_nRow); 
	m_scrollbar2.SetScrollPos(SysInfoPiece.m_nCol); 
	m_scrollbar3.SetScrollPos(SysInfoPiece.m_nPiece);
 	m_scrollbar4.SetScrollPos(SysInfoPiece.m_nColDel); 
	m_scrollbar5.SetScrollPos(SysInfoPiece.m_nRowDel); 

	str.Format("%d", SysInfoPiece.m_nRow);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPiece.m_nCol);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPiece.m_nPiece);
	m_Label3.SetCaption(str);
	str.Format("%d", SysInfoPiece.m_nColDel);
	m_Label4.SetCaption(str);
	str.Format("%d", SysInfoPiece.m_nRowDel);
	m_Label5.SetCaption(str);
 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CModeSub25::OnSelchangeCombo1() 
{
    SysInfoPiece.m_nType = m_combo1.GetCurSel();
 
}


BOOL CModeSub25::PreTranslateMessage(MSG* pMsg) 
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

void CModeSub25::OnClickOk() 
{
 
	FileSysInfo01.LoadSaveSub12(2);
	CDialog::OnOK();

}

void CModeSub25::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here

 
	
}

void CModeSub25::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}
 

/*
void CModeSub25::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
 // TODO: Add your message handler code here and/or call default

 CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

 switch(pSlider->GetDlgCtrlID())
 {
 case IDC_SLIDER1:     
  m_slider1.SetPos(pSlider->GetPos());
  break;
 default:
  break;
 }
 SetAttr(pSlider->GetPos());

 CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
*/


void CModeSub25::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nScrollPos;
	CString strValue;
	
	int ka,i,j;
	CString str;

	
	if(pScrollBar->GetSafeHwnd() == m_scrollbar1.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfoPiece.m_nRow=nScrollPos;
				 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <101){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfoPiece.m_nRow=nScrollPos;				 
			}
			break;

	     case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				strValue.Format("%d", nPos);
				m_Label1.SetCaption(strValue);
				SysInfoPiece.m_nRow=nPos;	 		
			break;
			}

			ka=m_scrollbar2.GetScrollPos();
			SysInfoPiece.m_nPiece=ka*SysInfoPiece.m_nRow;
			m_scrollbar3.SetScrollPos(SysInfoPiece.m_nPiece);
			strValue.Format("%d", SysInfoPiece.m_nPiece);
	        m_Label3.SetCaption(strValue);	
			
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

 

	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar2.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfoPiece.m_nCol=nScrollPos;
 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <101){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfoPiece.m_nCol=nScrollPos;
 
			}
			break;

	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label2.SetCaption(strValue);
			SysInfoPiece.m_nCol=nPos;
 
			break;
		}

		ka=m_scrollbar1.GetScrollPos();
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
 

	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar3.GetSafeHwnd()){
	
	
	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar4.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label4.SetCaption(strValue);
				SysInfoPiece.m_nColDel=nScrollPos;
				 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <101){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label4.SetCaption(strValue);
				SysInfoPiece.m_nColDel=nScrollPos;				 
			}
			break;

	     case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				strValue.Format("%d", nPos);
				m_Label4.SetCaption(strValue);
				SysInfoPiece.m_nColDel=nPos;	 		
			break;
			}

 
	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar5.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label5.SetCaption(strValue);
				SysInfoPiece.m_nRowDel=nScrollPos;
 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <101){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label5.SetCaption(strValue);
				SysInfoPiece.m_nRowDel=nScrollPos;
 
			}
			break;

	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label5.SetCaption(strValue);
			SysInfoPiece.m_nRowDel=nPos;
 
			break;
		}
 
	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CModeSub25::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here

 	
}


extern CSysInfoView01  SysInfoView01;//sylee150804-2

void CModeSub25::InitControl()
{

	CString str;
	int no;
    

    Onload(); 

  
	str=SysInfoView01.m_pStrFileBDLName;
  	no = str.Find('.');
	str= str.Left(no);
	m_Label6.SetCaption(str);//sylee150804-2

	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928
	A_Language();

} 



void CModeSub25::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
 	//  m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupBlock", 101));
 	  m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupPiece", 102));
	  m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupPiece", 103));
	  m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupPiece", 104));
      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupPiece", 124));
  	  m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupPiece", 125));
	  m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupPiece", 105)); 
  	  m_Label1008.SetWindowText(LoadMessage(1,"TitleSetupPiece", 119));
	  m_Label1009.SetWindowText(LoadMessage(1,"TitleSetupPiece", 120)); 
	  
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupPiece", 115));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupPiece", 121));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupPiece", 122));
	  m_button1004.SetWindowText(LoadMessage(1,"TitleSetupPiece", 123)); 
 	  
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupPiece", 115));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupPiece", 121));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupPiece", 122));
	  m_button1004.SetWindowText(LoadMessage(1,"TitleSetupPiece", 123));

  }

}

 

void CModeSub25::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	BeginEditing();		
	
}

void CModeSub25::OnEnterCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSub25::OnLeaveCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	if (OnEditing) {
		EndEditing();
	}	
}

void CModeSub25::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	m_grid1.SetRowSel(m_nCurRow);
	m_grid1.SetColSel(m_nCurCol);	
}

void CModeSub25::EndEditing()
{
 
	CString strText;
 
	m_edit1.EnableWindow(FALSE);
	m_edit1.ShowWindow(SW_HIDE);
	m_edit1.GetWindowText(strText);
 
	m_grid1.SetText(strText);
	OnEditing = FALSE;
	m_grid1.SetFocus();
//	SetModified(TRUE);	
}

void CModeSub25::BeginEditing()
{
 
	OnEditing = TRUE;
	CRect rectCell;
	rectCell.left = m_rectGrid.left + long(m_dRatio * double(m_grid1.GetCellLeft()));
	rectCell.top = m_rectGrid.top + long(m_dRatio * double(m_grid1.GetCellTop()));
	rectCell.right = rectCell.left + long(m_dRatio * double(m_grid1.GetCellWidth()));
	rectCell.bottom = rectCell.top + long(m_dRatio * double(m_grid1.GetCellHeight()));
 
	m_edit1.SetWindowText(m_grid1.GetTextMatrix(m_nCurRow, m_nCurCol));
	m_edit1.MoveWindow(rectCell);
	m_edit1.EnableWindow(TRUE);
	m_edit1.ShowWindow(SW_SHOWNORMAL);
	m_edit1.SetFocus();
	m_edit1.SetSel(0, -1);
 
}


void CModeSub25::OnSave() 
{
	int i,k; 
    CString str;
   
   ::ZeroMemory(&SysInfoPiece.m_nData,sizeof(SysInfoPiece.m_nData));

   for( i = 0 ; i < (SysInfoPiece.m_nRow); i++) 
   { 
		for( k = 0 ; k <(SysInfoPiece.m_nCol);  k++) 
		{ 
		     str= m_grid1.GetTextMatrix(i+1, k+1);  
			 sscanf(LPCTSTR(str), "%d", &SysInfoPiece.m_nData[i][k]);
		}
   }

	FileSysInfo01.LoadSavePieceInfo(_SAVE); //save  //son210430 LoadSaveSub25 ->LoadSavePieceInfo

	Onload(); //load
//	i=3;

}


void CModeSub25::Onload() 
{

	int i, nRowCt,nColCt;
	int k; 
	CString str; 
	char cpBuffer[20];


	m_grid1.Clear();
	m_grid1.Refresh();
	m_grid1.SetRedraw(0);

	FileSysInfo01.LoadSavePieceInfo(_LOAD); //son210430 LoadSaveSub25 ->LoadSavePieceInfo
 	m_scrollbar1.SetScrollPos(SysInfoPiece.m_nRow); 
	m_scrollbar2.SetScrollPos(SysInfoPiece.m_nCol); 
	m_scrollbar3.SetScrollPos(SysInfoPiece.m_nPiece);  //piece total
 
	str.Format("%d", SysInfoPiece.m_nRow);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPiece.m_nCol);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPiece.m_nPiece);
	m_Label3.SetCaption(str);

	m_edit2=SysInfoPiece.m_nCol;
	m_edit3=SysInfoPiece.m_nRow;
	UpdateData(FALSE);
 
	nColCt=SysInfoPiece.m_nCol+1; 
	m_grid1.SetCols(nColCt);
	
	nRowCt=SysInfoPiece.m_nRow+1; 
	m_grid1.SetRows(nRowCt);

     
	for ( i = 1 ; i < nRowCt; i++) 
	{ 
 		str.Format("Row %d", i); 
 		m_grid1.SetTextMatrix(i, 0, str);
	} 
 
    for (  i = 1 ; i < nColCt ; i++) 
    { 
  		str.Format("Col %d", i); 
 		m_grid1.SetTextMatrix(0, i, str);

    } 

  
   for( i = 0 ; i <  SysInfoPiece.m_nRow ; i++) 
   { 
		for( k = 0 ; k < SysInfoPiece.m_nCol ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoPiece.m_nData[i][k]);
		     m_grid1.SetTextMatrix(i+1, k+1, cpBuffer);
		}
   }
 
  
  

	m_grid1.Refresh();
 	m_grid1.SetRedraw(TRUE);

  	m_nCurRow = 1;
	m_nCurCol = 1;

	m_grid1.GetWindowRect(m_rectGrid);
	ScreenToClient(m_rectGrid);

	m_dRatio = double(m_rectGrid.Width())  /10500.0;//8880.0;/8880.0;
	OnEditing = FALSE;
	
}


void CModeSub25::OnClickButton101() 
{ 
    CString str;
    int i,j,k;

    k=0;

    for( i=1; i<SysInfoPiece.m_nRow+1 ; i++)
    {
        for( j=1; j<SysInfoPiece.m_nCol+1 ; j++)
        {
            if( SysInfoPiece.m_nType==1){
                k++;
            }
            else if( SysInfoPiece.m_nType==2){  //ok
                k=SysInfoPiece.m_nCol*(i)-(j-1);
            }
            else if( SysInfoPiece.m_nType==3){  //ok
                k=SysInfoPiece.m_nCol*(SysInfoPiece.m_nRow-i)+j ;
            }
            else if( SysInfoPiece.m_nType==4){  //ok
                k=SysInfoPiece.m_nCol*(SysInfoPiece.m_nRow-i)+(SysInfoPiece.m_nCol-j+1);
            }
            else if( SysInfoPiece.m_nType==5){  //ok
                k=SysInfoPiece.m_nRow*(j-1)+i;	
            }
            else if( SysInfoPiece.m_nType==6){  //ok 
                k=SysInfoPiece.m_nRow*(SysInfoPiece.m_nCol-j)+i ; 
            }
            else if( SysInfoPiece.m_nType==7){  //ok
                k=SysInfoPiece.m_nRow*(j)+(1-i);	
            }
            else if( SysInfoPiece.m_nType==8){  //ok
                k=SysInfoPiece.m_nRow*(SysInfoPiece.m_nCol-j+1)+(1-i);
            } 
            m_grid1.SetRowSel(i);
            m_grid1.SetColSel(j);
            str.Format("%d", k);  
            m_grid1.SetTextMatrix(i, j, str);
        }
    }

    return ;
}

