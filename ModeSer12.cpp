// ModeSer12.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSer12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FileSysInfo.h"
 
extern CSysInfoSer12  SysInfoSer12;
extern CFileSysInfo FileSysInfo01;
/////////////////////////////////////////////////////////////////////////////
// CModeSer12 dialog


CModeSer12::CModeSer12(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSer12::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSer12)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSer12::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSer12)
		DDX_Control(pDX, IDC_GRID_1, m_grid1);
		DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	//	DDX_Control(pDX, IDC_SLIDER2, m_slider2);		
		DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
	//	DDX_Control(pDX, IDC_LABEL2,  m_Label2);		
		DDX_Control(pDX, IDC_EDIT1,   m_edit1);
 		DDX_Text(pDX, IDC_EDIT2,   m_edit2);	
//		DDX_Text(pDX, IDC_EDIT3,   m_edit3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSer12, CDialog)
	//{{AFX_MSG_MAP(CModeSer12)
	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_SLIDER1, OnChangeSlider1)	
//	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_SLIDER2, OnChangeSlider2)	
//	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_LABEL2, OnLabel2) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSer12 message handlers
BEGIN_EVENTSINK_MAP(CModeSer12, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSer12)
	ON_EVENT(CModeSer12, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
//	ON_EVENT(CModeSer12, IDC_LABEL2, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CModeSer12, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSer12, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSer12, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSer12, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSer12, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSer12, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



BOOL CModeSer12::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	int  i;
 	CString str; 

	m_slider1.SetRange(1,100);  
 //   m_slider2.SetRange(1,51200); 
  
 	InitControl();

 	m_slider1.SetPos(SysInfoSer12.m_nMutiNo);
 //	m_slider2.SetPos(SysInfoSer12.m_nPinTot/512);
//	m_slider2.SetPos(SysInfoSer12.m_nPinTot);

	str.Format("%d", SysInfoSer12.m_nMutiNo);
	m_Label1.SetCaption(str);
//	str.Format("%d", SysInfoSer12.m_nPinTot);
//	m_Label2.SetCaption(str);
 
//m_slider1.SetPos(255);          
//m_nOpa=m_slider1.GetPos();  
// CFileS  mFileS;
// mFileS.SaveMulti1();
// SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)| WS_EX_LAYERED);
 
 // SetAttr(m_nOpa);
 //SetLayeredWindowAttributes(RGB(0,0,255),m_nOpa,LWA_ALPHA | LWA_COLORKEY);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CModeSer12::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
/*	if ((pMsg->message == WM_KEYDOWN) 
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

void CModeSer12::OnClickOk() 
{
 
    FileSysInfo01.LoadSaveSer12(2);
 
	CDialog::OnOK();

}

void CModeSer12::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here

	int i;
	i=3;
	
}

void CModeSer12::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}

 
void CModeSer12::OnChangeSlider1()	
{

	int i,j;
	CString strValue;
	CString str;
 
	i=m_slider1.GetPos();
 
	m_slider1.SetPos(i);	
	SysInfoSer12.m_nMutiNo=i;	
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

/*void CModeSer12::OnChangeSlider2()	
{

	int i;
	CString strValue;
 
//	i=m_slider2.GetPos();
//	m_slider2.SetPos(i);
 //	i=i*512;

 //	SysInfoSer12.m_nPinTot=i;

 //   strValue.Format("%d", i);
//	m_Label2.SetCaption(strValue);

	UpdateData(FALSE);
}
*/
void CModeSer12::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


void CModeSer12::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here

int i;
i=2;
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

void CModeSer12::InitControl()
{

  Onload(); 

} 

void CModeSer12::OnLabel2() 
{ 
	int i;
	i=2; 
}

void CModeSer12::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	BeginEditing();		
	
}

void CModeSer12::OnEnterCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSer12::OnLeaveCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	if (OnEditing) {
		EndEditing();
	}	
}

void CModeSer12::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	m_grid1.SetRowSel(m_nCurRow);
	m_grid1.SetColSel(m_nCurCol);	
}

void CModeSer12::EndEditing()
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

void CModeSer12::BeginEditing()
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

void CModeSer12::OnSave() 
{
	int i,k; 	
   CString str;
   
   ::ZeroMemory(&SysInfoSer12.m_nPin,sizeof(SysInfoSer12.m_nPin));

   for( i = 1 ; i < (SysInfoSer12.m_nMutiNo+1); i++) 
   { 
		for( k = 1 ; k < 5;  k++) 
		{ 
		     str= m_grid1.GetTextMatrix(i, k);  
			 sscanf(LPCTSTR(str), "%d", &SysInfoSer12.m_nPin[i-1][k-1]);
		}
   }

	FileSysInfo01.LoadSaveSer12(1); //save

	Onload(); //load
//	i=3;

}


void CModeSer12::Onload() 
{

	int i, nRowCt,nColCt;
	int k; 
	CString str; 
	char cpBuffer[20];


	m_grid1.Clear();
	m_grid1.Refresh();
	m_grid1.SetRedraw(0);


	FileSysInfo01.LoadSaveSer12(2);

	m_slider1.SetPos(SysInfoSer12.m_nMutiNo);
 //	m_slider2.SetPos(SysInfoSer12.m_nPinTot/512);
//	m_slider2.SetPos(SysInfoSer12.m_nPinTot);
	str.Format("%d", SysInfoSer12.m_nMutiNo);
	m_Label1.SetCaption(str);

//	str.Format("%d", SysInfoSer12.m_nPinTot);
//	m_Label2.SetCaption(str);

	m_edit2=SysInfoSer12.m_nMutiNo;
//	m_edit3=SysInfoSer12.m_nPinTot;
	UpdateData(FALSE);

	nColCt=5;  
	m_grid1.SetCols(nColCt);
	
	nRowCt=SysInfoSer12.m_nMutiNo+1; 
	m_grid1.SetRows(nRowCt);

//	m_grid1.SetCol(0); 
//	m_grid1.SetRow(0); 
//	str.Format("Block"); 
//	m_grid1.SetText(str); 

    
	for ( i = 1 ; i < nRowCt; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format(" No.%d", i); 
		m_grid1.SetText(str); 
	} 
  
    for (  i = 1 ; i < nColCt ; i++) 
    { 
        m_grid1.SetRow(0); 
        m_grid1.SetCol(i); 
		//str.Format("Col %d", i); 
		if( i==1){
           str.Format("1st Start ", i); 
		}else if( i==2){
           str.Format("1st End", i); 
		}else if( i==3){
           str.Format("2st Start", i); 
		}else if( i==4){
           str.Format("2st End", i); 
		}  
        m_grid1.SetText(str); 
    } 

  
 //	m_grid1.SetRows(SysInfoSer12.m_nMutiNo+1); 

   for( i = 1 ; i < (SysInfoSer12.m_nMutiNo+1); i++) 
   { 
		for( k = 1 ; k < 5;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoSer12.m_nPin[i-1][k-1]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }

     m_grid1.SetRow(1); 
     m_grid1.SetCol(1);
   
  
 
//data cell color
/*

    long BackColor[3],FontColor[3];

  
 // FontColor[0]=0X00400000;
 // BackColor[0]=0x00FFFFFF;  
  FontColor[1]=0X000000FF;
  BackColor[1]=0x00FFFFB0; 

for ( k = 1 ; k < m_grid1.GetRows(); k++) 
{
 for ( i = 1 ; i < m_grid1.GetCols(); i++) 
  { 
  m_grid1.SetRow(k); 
  m_grid1.SetCol(i); 

   // FontColor[0]=RGB(221, 236, 255);//파랑색
  //BackColor[0]=RGB(86, 125, 204); //하늘색 

  m_grid1.SetCellBackColor(RGB(221, 236, 255));//RGB(221, 236, 255);//파랑색
  m_grid1.SetCellForeColor(RGB(86, 125, 204));//RGB(86, 125, 204); //하늘색
  }
}
 */

	m_grid1.Refresh();
 	m_grid1.SetRedraw(TRUE);

  	m_nCurRow = 1;
	m_nCurCol = 1;

	m_grid1.GetWindowRect(m_rectGrid);
	ScreenToClient(m_rectGrid);

	//
	m_dRatio = double(m_rectGrid.Width())/5900.0;
//	m_dRatio = 100000.0;
	OnEditing = FALSE;
	
}

 

