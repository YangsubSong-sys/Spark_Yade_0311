// ModeSub19.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub19.h"//sylee131121

#include "FileSysInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


 
extern CSysInfoBlock  SysInfoBlock;
extern CSysInfo19  SysInfo19;
extern CSysInfoPiece  SysInfoPiece;
extern CFileSysInfo FileSysInfo01;

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

extern double dRec[362][16];
extern int nBlockMaxLast1;//sylee170417-1
 

/////////////////////////////////////////////////////////////////////////////
// CModeSub19 dialog


CModeSub19::CModeSub19(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub19::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub19)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub19::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub19)
		// NOTE: the ClassWizard will add DDX and DDV calls here
 
	    DDX_Control(pDX, IDC_LABEL5,  m_Label5); 

		DDX_Control(pDX, IDC_GRID_1, m_grid1);
 
		DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollbar1);
		DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollbar2);

		DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);
		DDX_Control(pDX, IDC_LABEL3,  m_Label3);
		
		DDX_Control(pDX, IDC_EDIT1,   m_edit1);

		DDX_Text(pDX, IDC_EDIT2,   m_edit2);	
		DDX_Text(pDX, IDC_EDIT3,   m_edit3);

		DDX_Control(pDX, IDC_CHECK1, m_check1);
		DDX_Control(pDX, IDC_EDIT101,   m_edit101);		
		DDX_Control(pDX, IDC_EDIT102,   m_edit102); 
		DDX_Control(pDX, IDC_EDIT103,   m_edit103); 
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
		DDX_Control(pDX, IDC_SET,  m_button1002);

		DDX_Control(pDX, IDC_BLOAD,  m_button1003);
		DDX_Control(pDX, IDC_OK,     m_button1004);
		DDX_Control(pDX, IDC_BSAVE,  m_button1005);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub19, CDialog)
	//{{AFX_MSG_MAP(CModeSub19)
		// NOTE: the ClassWizard will add message map macros here 
	    ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub19 message handlers



BEGIN_EVENTSINK_MAP(CModeSub19, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub19)
	ON_EVENT(CModeSub19, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_LABEL2, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSub19, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
    ON_EVENT(CModeSub19, IDC_SET2,-600 /* Click */, OnClickSet2, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_BUTTON101, -600 /* Click */, OnClickButton101, VTS_NONE)
	ON_EVENT(CModeSub19, IDC_SET, -600 /* Click */, OnClickSet, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 


void CModeSub19::OnCheck1()  
{
	if(m_check1.GetCheck())
	{
		SysInfo19.m_nUse=1;
	}
	else
	{
		SysInfo19.m_nUse=0;
	}
	m_check1.SetCheck(SysInfo19.m_nUse); 

}



BOOL CModeSub19::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	int  i;
	CString str; 
//	int n1,n2;  
	//	OnPieceBlockSet1(); //SYLEE131122
    Onload();
	InitControl();	//sylee150804-2
  	//OnSave();
	m_edit101.SetWindowText("1");
	m_edit102.SetWindowText("1");
	m_edit103.SetWindowText("1");
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
void CModeSub19::OnSelchangeCombo1() 
{

	CString str;
	int i;

    SysInfo19.m_nType = m_combo1.GetCurSel();
    
    i = SysInfo19.m_nType;
	i=(int)dRec[i][6]*10;
// str.Format("%d", dRec[m_nMode1][6]*10
//	str.Format("%d", i);
//	m_edit3.SetWindowText(str);
 
}

*/
 


BOOL CModeSub19::PreTranslateMessage(MSG* pMsg) 
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

void CModeSub19::OnClickOk() 
{
 
	FileSysInfo01.LoadSaveSub19(2,1);
	CDialog::OnOK();

}

void CModeSub19::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here

	int i;
	i=3;
	
}

void CModeSub19::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}

 /*
void CModeSub19::OnChangeSlider1()	
{

	int i,j,k;
	CString strValue;
	CString str;
 
	i=m_slider1.GetPos(); 
	m_slider1.SetPos(i);

	j=m_slider2.GetPos();
	k=i*j;
	m_slider3.SetPos(k);

	strValue.Format("%d", k);
	m_Label3.SetCaption(strValue);
	SysInfoBlock.m_nBlock=k;


	
	SysInfoBlock.m_nRow=i;	
    strValue.Format("%d", i);
	m_Label1.SetCaption(strValue);
	i=i+1;	
	m_grid1.SetRows(i);  

    for ( j = 0 ; j < i-1; j++) 
    { 
		    m_grid1.SetCol(0); 
            m_grid1.SetRow(j+1); 
            str.Format("No. %d", j+1); 
            m_grid1.SetText(str); 
    } 

	UpdateData(FALSE);
}

void CModeSub19::OnChangeSlider2()	
{

	int i,j,k;
	CString strValue;
	CString str;
 
	i=m_slider2.GetPos();
	m_slider2.SetPos(i);

	j=m_slider1.GetPos();
	k=i*j;
	m_slider3.SetPos(k);


	strValue.Format("%d", k);
	m_Label3.SetCaption(strValue);
	SysInfoBlock.m_nBlock=k;


 	SysInfoBlock.m_nCol=i;
	strValue.Format("%d", i);
	m_Label2.SetCaption(strValue);

    i=i+1;	
	m_grid1.SetCols(i);  

    for ( j = 0 ; j < i-1; j++) 
    { 
		    m_grid1.SetRow(0); 
            m_grid1.SetCol(j+1); 
            str.Format("No. %d", j+1); 
            m_grid1.SetText(str); 
    } 

	UpdateData(FALSE);
}

void CModeSub19::OnChangeSlider3()	
{

	int i,j ;
	CString strValue;
	CString str;
 
	j=m_slider3.GetPos();
	m_slider3.SetPos(j);



 	SysInfoBlock.m_nBlock=j;
	strValue.Format("%d", j);
	m_Label3.SetCaption(strValue);

 	UpdateData(FALSE);
}

*/

/*
void CModeSub19::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


void CModeSub19::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nScrollPos;
	CString strValue;
	
	int ka,i,j;
	CString str;


	return ;//sylee131122

 
	if(pScrollBar->GetSafeHwnd() == m_scrollbar1.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfo19.m_nCellX=nScrollPos;				 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfo19.m_nCellX=nScrollPos;				 
			}
			break;

	     case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				strValue.Format("%d", nPos);
				m_Label1.SetCaption(strValue);
				SysInfo19.m_nCellX=nPos;	 		
			break;
			}

			ka=m_scrollbar2.GetScrollPos();
	//		SysInfoBlock.m_nBlock=ka*SysInfoBlock.m_nRow;
	//		m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
			strValue.Format("%d", SysInfo19.m_nCellX);
	        m_Label3.SetCaption(strValue);	
			
            i=SysInfo19.m_nCellX ;
			j= SysInfo19.m_nCellY ;
			i=i+1;	
			m_grid1.SetRows(i); 
			for ( j = 0 ; j < i-1; j++) 
			{ 
				m_grid1.SetCol(0); 
				m_grid1.SetRow(j+1); 
				str.Format("No. %d", j+1); 
				m_grid1.SetText(str); 
			} 

//			Invalidate();

	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar2.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 1){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfo19.m_nCellY=nScrollPos;
//				Invalidate();
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfo19.m_nCellY=nScrollPos;
//				Invalidate();
			}
			break;

	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label2.SetCaption(strValue);
			SysInfo19.m_nCellY=nPos;
//	 		Invalidate();
			break;
		}

		ka=m_scrollbar1.GetScrollPos();
	//	SysInfoBlock.m_nBlock=ka*SysInfoBlock.m_nCol;
//		m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
		strValue.Format("%d", SysInfo19.m_nCellY);
	    m_Label3.SetCaption(strValue);	
		
       // i= SysInfoBlock.m_nRow ;
		j= SysInfo19.m_nCellY ;
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

//	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar3.GetSafeHwnd()){
	/*	switch(nSBCode){
		case SB_LINELEFT: 		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfoBlock.m_nBlock=nScrollPos;
				Invalidate();
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfoBlock.m_nBlock=nScrollPos;
				Invalidate();
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label3.SetCaption(strValue);
			SysInfoBlock.m_nBlock=nPos;
	 		Invalidate();
			break;
		}
     

  */
	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CModeSub19::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here
 
/*	 // 그리드 컨트롤을 더블클릭 했을 때 처리하는 루틴
    CEditGridDlg dlg;    

    // 현재의 x, y 좌표를 얻는다. 
    int x, y; 

    x = m_grid1.GetColSel(); 
    y = m_grid1.GetRowSel(); 

    dlg.m_editStr = m_grid1.GetText();      

    if ( dlg.DoModal() == IDOK)
    { 
            m_grid1.SetText(dlg.m_editStr); 
    } 
*/
	
}


extern CSysInfoView01  SysInfoView01;//sylee150804-2

void CModeSub19::InitControl()
{

     Onload(); 

  	 //extern CSysInfoView01  SysInfoView01;//sylee150804-2

	CString str;
	int no;
    str=SysInfoView01.m_pStrFileBDLName;
  	no = str.Find('.');
	str= str.Left(no);
	m_Label5.SetCaption(str);//sylee150804-2

	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928
	
	A_Language();


} 

/*void CModeSub19::OnLabel2() 
{ 
	int i;
	i=2; 
}
*/




void CModeSub19::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
 
      m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 101));
 	  m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 102));
	  m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 103));
	  m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 104));
      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 107));
 	  m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 108));
	  m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 109));
	  m_Label1008.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 110));
      m_Label1009.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 111));
 
 
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 106));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 112));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 113));
	  m_button1004.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 114));
	  m_button1005.SetWindowText(LoadMessage(1,"TitleSetupPIeceDisable", 115));

  }
 
//105=Use 

}


void CModeSub19::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	BeginEditing();		
	
}

void CModeSub19::OnEnterCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSub19::OnLeaveCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	if (OnEditing) {
		EndEditing();
	}	
}

void CModeSub19::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	m_grid1.SetRowSel(m_nCurRow);
	m_grid1.SetColSel(m_nCurCol);	
}

void CModeSub19::EndEditing()
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

void CModeSub19::BeginEditing()
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

void CModeSub19::OnSave() 
{
	int i,k ; 
	CString str;

   ::ZeroMemory(&SysInfo19.m_nData,sizeof(SysInfo19.m_nData));

    str=m_Label1.GetCaption();
	SysInfo19.m_nCellX=atoi(str);	
    str=m_Label2.GetCaption();
	SysInfo19.m_nCellY=atoi(str);
    SysInfo19.m_nCellTotal=SysInfo19.m_nCellX*SysInfo19.m_nCellY;

    FileSysInfo01.LoadSaveBlockInfo(_LOAD);	    //son220824_5
    FileSysInfo01.LoadSavePieceInfo(_LOAD);   //son210430 LoadSaveSub25 ->LoadSavePieceInfo

   for( i = 1 ; i <= (SysInfo19.m_nCellX); i++) 
   { 
		for(  k = 1 ; k <= (SysInfo19.m_nCellY);  k++) 
		{ 
		     str= m_grid1.GetTextMatrix(i, k);  
			 sscanf(LPCTSTR(str), "%d", &SysInfo19.m_nData[i][k]);
		}
   }

    OnCheck1();

	FileSysInfo01.LoadSaveSub19(1,1); //save

	Onload(); //load
//	i=3;

}


void CModeSub19::Onload() 
{

	int i, nRowCt,nColCt;
//	int k; 
	CString str; 
 	char cpBuffer[20];
    int n1,n2;

	FileSysInfo01.LoadSaveSub19(2,1);
 	FileSysInfo01.LoadSaveBlockInfo(_LOAD);	    //son220824_5
 	FileSysInfo01.LoadSavePieceInfo(_LOAD);     //son210430 LoadSaveSub25 ->LoadSavePieceInfo
	
	n1=SysInfoBlock.m_nBlock;  //Max piece  
 	n2=SysInfoPiece.m_nPiece;  //Max block

	SysInfo19.m_nCellX=n1;
    SysInfo19.m_nCellY=n2;
	SysInfo19.m_nCellTotal=n1*n2;
	
	m_grid1.Clear();
	m_grid1.Refresh();
	m_grid1.SetRedraw(0);

	m_scrollbar1.SetScrollPos(SysInfo19.m_nCellX+1); 
	m_scrollbar2.SetScrollPos(SysInfo19.m_nCellY+1); 
 
	str.Format("%d", SysInfo19.m_nCellX);//sylee131121
	m_Label1.SetCaption(str);
 
	str.Format("%d", SysInfo19.m_nCellY);//sylee131121
	m_Label2.SetCaption(str);

    str.Format("%d", SysInfo19.m_nCellTotal);
	m_Label3.SetCaption(str);


	m_edit2=SysInfo19.m_nCellX+1;
	m_edit3=SysInfo19.m_nCellY+1;
	
	UpdateData(FALSE);
 
	nColCt=SysInfo19.m_nCellY+1; //sylee131121
    m_grid1.SetCols(nColCt);
 
	nRowCt=SysInfo19.m_nCellX+1; //sylee131121
    m_grid1.SetRows(nRowCt);
 

	if(SysInfo19.m_nUse!=1){
		SysInfo19.m_nUse=0;
	} 
	m_check1.SetCheck(SysInfo19.m_nUse); 
	
    
	for ( i = 1 ; i < nRowCt; i++) 
	{ 
	    str.Format("Block %d", i); 
 		m_grid1.SetTextMatrix(i, 0, str);
	} 
 
    for (  i = 1 ; i < nColCt ; i++) 
    { 
      	str.Format("P %d", i); 
	 	m_grid1.SetTextMatrix(0, i, str);
    } 

   for( i = 1 ; i <=SysInfo19.m_nCellX ; i++) 
   {  
		for( int  k = 1 ; k <=SysInfo19.m_nCellY ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfo19.m_nData[i][k]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);		 
		}
   }
 
 
	m_grid1.Refresh();
 	m_grid1.SetRedraw(TRUE);

  	m_nCurRow = 1;
	m_nCurCol = 1;

	m_grid1.GetWindowRect(m_rectGrid);
	ScreenToClient(m_rectGrid);

	m_dRatio = double(m_rectGrid.Width()) /11580.0;//8880.0;
	OnEditing = FALSE;

  
	
}


void CModeSub19::OnClickButton101() 
{ 
	CString str;
	int i,j;


	OnPieceBlockSet1();

 
	for( i=1; i<SysInfo19.m_nCellX+1 ; i++){
			for( j=1; j<SysInfo19.m_nCellY+1 ; j++){
				str.Format("0");  
				m_grid1.SetTextMatrix(i, j, str);
			}
	}
 
	return ;
}




void CModeSub19::OnClickSet() 
{ 
	CString str;
	int i,j,k1,k2,k3;
	char szText[200];

 

	m_edit101.GetWindowText(szText,10);	
	k1 = atoi( szText );   
	if(k1<1){
		k1=1;
		m_edit101.SetWindowText("1");
	}
	m_edit102.GetWindowText(szText,10);	
	k2 = atoi( szText );  		
	if(k2<1){
		k2=1;
		m_edit102.SetWindowText("1");
	}		 
	m_edit103.GetWindowText(szText,10);	
	k3 = atoi( szText ); 
	if(k3<1){
		k3=1;
		m_edit103.SetWindowText("1");
	}	

		for( i=k1; i<=k1 ; i++){
				for( j=k2; j<=k3 ; j++){
				    str.Format("1");  
				    m_grid1.SetTextMatrix(i, j, str);
				}
		}
	return ;
}



void CModeSub19::OnClickSet2() 
{ 
 
    CString strPath;
    strPath.Format("%s\\ACE400PieceDisable.exe", SysInfoView01.m_pStrFilePath1); //son210514 "C:\\ACE400\\ACE400PieceDisable.exe"
    ::ShellExecute(NULL, "open", strPath, "NULL", "NULL", SW_SHOWNORMAL); 	     //son210514 
	return ;
}









void CModeSub19::OnPieceBlockSet1() 
{ 
 
	int n1,n2,nColCt,nRowCt;
	CString str;

 	FileSysInfo01.LoadSaveBlockInfo(_LOAD);	    //son220824_5
 	FileSysInfo01.LoadSavePieceInfo(_LOAD);     //son210430 LoadSaveSub25 ->LoadSavePieceInfo
	FileSysInfo01.LoadSaveSub19(2,1); //sylee131121    //load

  	n1=SysInfoBlock.m_nBlock;  //Max piece  
 	n2=SysInfoPiece.m_nPiece;  //Max block

    SysInfo19.m_nCellX=n1;  //Max piece  //sylee131121 
	SysInfo19.m_nCellY=n2;  //Max block  //sylee131121 
    SysInfo19.m_nCellTotal=n1*n2;  //Max block  //sylee13121 
 
	m_scrollbar1.SetScrollRange(1, 100, FALSE);
	m_scrollbar2.SetScrollRange(1, 100, FALSE);
 

	m_scrollbar1.SetScrollPos(n1); 
	m_scrollbar2.SetScrollPos(n2); 
 
	str.Format("%d", n1);
	m_Label1.SetCaption(str);
	str.Format("%d", n2);
	m_Label2.SetCaption(str);
//sylee131121	str.Format("%d", n1*n2);
	str.Format("%d", SysInfo19.m_nCellTotal);  //Max block  //sylee13121 ); //sylee131121
 
	m_Label3.SetCaption(str);

 
	nColCt=SysInfo19.m_nCellY+1; //sylee131121
    m_grid1.SetCols(nColCt);
 
	nRowCt=SysInfo19.m_nCellX+1; //sylee131121
    m_grid1.SetRows(nRowCt);
 

    for ( int i = 1 ; i < nRowCt; i++) 
	{ 
	    str.Format("Block %d", i); 
 		m_grid1.SetTextMatrix(i, 0, str);
	} 
 
    for (  i = 1 ; i < nColCt ; i++) 
    { 
      //	str.Format("Piece %d", i); 
		str.Format("P %d", i); 
	 	m_grid1.SetTextMatrix(0, i, str);
    } 


	UpdateData(FALSE);





	}
