// ModeSub13.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub13.h"

#include "FileSysInfo.h"

 
extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA

 
extern CSysInfoBlock  SysInfoBlock;
extern CFileSysInfo FileSysInfo01;
extern CSysSet211  SysSet211;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	double dRec[362][16];



/////////////////////////////////////////////////////////////////////////////
// CModeSub13 dialog


CModeSub13::CModeSub13(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub13::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub13)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub13::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub13)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_LABEL5,  m_Label5);//sylee150804-2
		DDX_Control(pDX, IDC_COMBO01, m_combo1);
	    DDX_Control(pDX, IDC_COMBO02, m_combo2);
		DDX_Control(pDX, IDC_GRID_1, m_grid1); 
		DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollbar1);
		DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollbar2); 
		DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);
		DDX_Control(pDX, IDC_LABEL3,  m_Label3);
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
		DDX_Control(pDX, IDC_BUTTON101,  m_button1001);
		DDX_Control(pDX, IDC_BLOAD,  m_button1002);
		DDX_Control(pDX, IDC_BSAVE,  m_button1003);
		DDX_Control(pDX, IDC_OK,  m_button1004);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub13, CDialog)
	//{{AFX_MSG_MAP(CModeSub13)
		// NOTE: the ClassWizard will add message map macros here
 
	ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO02, OnSelchangeCombo2)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub13 message handlers



BEGIN_EVENTSINK_MAP(CModeSub13, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub13)
	ON_EVENT(CModeSub13, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_LABEL2, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSub13, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	ON_EVENT(CModeSub13, IDC_BUTTON101, -600 /* Click */, OnClickButton101, VTS_NONE)

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

 

BOOL CModeSub13::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
	CString str; 

	InitControl();

	m_combo1.SetCurSel(SysInfoBlock.m_nBlockAlign);
	m_combo2.SetCurSel(SysInfoBlock.m_nStepDirection);//sylee180417
 
	m_scrollbar1.SetScrollRange(1, 100, FALSE);
	m_scrollbar2.SetScrollRange(1, 100, FALSE); 

	m_scrollbar1.SetScrollPos(SysInfoBlock.m_nRow); 
	m_scrollbar2.SetScrollPos(SysInfoBlock.m_nCol); 
 
	str.Format("%d", SysInfoBlock.m_nRow);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoBlock.m_nCol);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoBlock.m_nBlock);
	m_Label3.SetCaption(str);


    //son Manual -> Set1 에서 '8) Block XY+- : Auto Display' Enable이면
    //    Step Direction을 'Z:STEPPATH.txt 기준으로 로딩하므로 
    //    Step Direction IDC_COMBO02 comboBox에서 Change하지 못하도록 Disable한다.
    //    IDC_COMBO02 comboBox는 edit 박스처럼 Z:STEPPATH.txt의 상황을 반영만 한다.
    if(SysSet211.m_nSet21==1)       //son200510-4WLOG
        GetDlgItem(IDC_COMBO02)->EnableWindow(FALSE);
    else
        GetDlgItem(IDC_COMBO02)->EnableWindow(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CModeSub13::OnSelchangeCombo1() 
{

	CString str;
	int i;

    SysInfoBlock.m_nBlockAlign = m_combo1.GetCurSel();
    
    i = SysInfoBlock.m_nBlockAlign;
	i=(int)dRec[i][6]*10;
}


void CModeSub13::OnSelchangeCombo2() 
{

	CString str;
    SysInfoBlock.m_nStepDirection = m_combo2.GetCurSel(); 
}


BOOL CModeSub13::PreTranslateMessage(MSG* pMsg) 
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

void CModeSub13::OnClickOk() 
{
 
	FileSysInfo01.LoadSaveSub12(2);
	CDialog::OnOK();

}

void CModeSub13::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here

	int i;
	i=3;
	
}

void CModeSub13::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}
 
void CModeSub13::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nScrollPos;
	CString strValue;
	
	int ka,i,j;
	CString str;

	
	if(pScrollBar->GetSafeHwnd() == m_scrollbar1.GetSafeHwnd())
	{
		switch(nSBCode)
        {
            case SB_LINELEFT:		case SB_PAGELEFT:
                nScrollPos = pScrollBar->GetScrollPos();
                if(nScrollPos > 1){
                    pScrollBar->SetScrollPos(--nScrollPos);	
                    strValue.Format("%d", nScrollPos);
                    m_Label1.SetCaption(strValue);
                    SysInfoBlock.m_nRow=nScrollPos;

                }
                break;

            case SB_LINERIGHT:		 case SB_PAGERIGHT:
                nScrollPos = pScrollBar->GetScrollPos();
                if(nScrollPos <100){
                    pScrollBar->SetScrollPos(++nScrollPos);	
                    strValue.Format("%d", nScrollPos);
                    m_Label1.SetCaption(strValue);
                    SysInfoBlock.m_nRow=nScrollPos;				 
                }
                break;

            case SB_THUMBTRACK:
                pScrollBar->SetScrollPos(nPos);
                strValue.Format("%d", nPos);
                m_Label1.SetCaption(strValue);
                SysInfoBlock.m_nRow=nPos;	 		
                break;
        }

        ka=m_scrollbar2.GetScrollPos();
        SysInfoBlock.m_nBlock=ka*SysInfoBlock.m_nRow;
        //		m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
        strValue.Format("%d", SysInfoBlock.m_nBlock);
        m_Label3.SetCaption(strValue);	

        i=SysInfoBlock.m_nRow ;
        j= SysInfoBlock.m_nCol ;
        i=i+1;	
        m_grid1.SetRows(i); 
        for ( j = 0 ; j < i-1; j++) 
        { 
            m_grid1.SetCol(0); 
            m_grid1.SetRow(j+1); 
            str.Format("No. %d", j+1); 
            m_grid1.SetText(str); 
        } 



    }
    else if(pScrollBar->GetSafeHwnd() == m_scrollbar2.GetSafeHwnd())
    {
		switch(nSBCode)
        {
            case SB_LINELEFT:		case SB_PAGELEFT:
                nScrollPos = pScrollBar->GetScrollPos();
                if(nScrollPos > 1){
                    pScrollBar->SetScrollPos(--nScrollPos);	
                    strValue.Format("%d", nScrollPos);
                    m_Label2.SetCaption(strValue);
                    SysInfoBlock.m_nCol=nScrollPos;

                }
                break;

            case SB_LINERIGHT:		 case SB_PAGERIGHT:
                nScrollPos = pScrollBar->GetScrollPos();
                if(nScrollPos <100){
                    pScrollBar->SetScrollPos(++nScrollPos);	
                    strValue.Format("%d", nScrollPos);
                    m_Label2.SetCaption(strValue);
                    SysInfoBlock.m_nCol=nScrollPos;

                }
                break;

            case SB_THUMBTRACK:
                pScrollBar->SetScrollPos(nPos);
                strValue.Format("%d", nPos);
                m_Label2.SetCaption(strValue);
                SysInfoBlock.m_nCol=nPos;

                break;
        }

		ka=m_scrollbar1.GetScrollPos();
		SysInfoBlock.m_nBlock=ka*SysInfoBlock.m_nCol;
//		m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
		strValue.Format("%d", SysInfoBlock.m_nBlock);
	    m_Label3.SetCaption(strValue);	
		
       // i= SysInfoBlock.m_nRow ;
		j= SysInfoBlock.m_nCol ;
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



void CModeSub13::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here
 
	
}

extern CSysInfoView01  SysInfoView01;//sylee150804-2

void CModeSub13::InitControl()
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

/*void CModeSub13::OnLabel2() 
{ 
	int i;
	i=2; 
}
*/


void CModeSub13::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	BeginEditing();		
	
}

void CModeSub13::OnEnterCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSub13::OnLeaveCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	if (OnEditing) {
		EndEditing();
	}	
}

void CModeSub13::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	m_grid1.SetRowSel(m_nCurRow);
	m_grid1.SetColSel(m_nCurCol);	
}


void CModeSub13::A_Language()  //LANGUAGE CHANGE
{

  if( gDef_Language1==3){ 
	  m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupBlock", 101));
 	  m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupBlock", 102));
	  m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupBlock", 103));
	  m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupBlock", 104));
      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupBlock", 105));
 	  m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupBlock", 119));
	  m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupBlock", 120)); 
 	  
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupBlock", 115));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupBlock", 121));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupBlock", 122));
	  m_button1004.SetWindowText(LoadMessage(1,"TitleSetupBlock", 123));
   }

}

void CModeSub13::EndEditing()
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

void CModeSub13::BeginEditing()
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

void CModeSub13::OnSave() 
{
	int i,k; 	
    CString str;

   ::ZeroMemory(&SysInfoBlock.m_nData,sizeof(SysInfoBlock.m_nData));

   for( i = 0 ; i < (SysInfoBlock.m_nRow); i++) 
   { 
		for( k = 0 ; k < (SysInfoBlock.m_nCol);  k++) 
		{ 
		     str= m_grid1.GetTextMatrix(i+1, k+1);  
			 sscanf(LPCTSTR(str), "%d", &SysInfoBlock.m_nData[i][k]);
		}
   }

	FileSysInfo01.LoadSaveBlockInfo(_SAVE); // 1: save  //son220824_5

	Onload(); //load
//	i=3;

}


void CModeSub13::Onload() 
{

	int i, nRowCt,nColCt;
	int k; 
	CString str; 
	char cpBuffer[20];


	m_grid1.Clear();
	m_grid1.Refresh();
	m_grid1.SetRedraw(0);

	FileSysInfo01.LoadSaveBlockInfo(_LOAD); // 2:load //son220824_5
/*
	m_slider1.SetPos(SysInfoBlock.m_nRow);
 	m_slider2.SetPos(SysInfoBlock.m_nCol);
    m_slider3.SetPos(SysInfoBlock.m_nBlock);
*/
	m_scrollbar1.SetScrollPos(SysInfoBlock.m_nRow); 
	m_scrollbar2.SetScrollPos(SysInfoBlock.m_nCol); 
//	m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
 
	str.Format("%d", SysInfoBlock.m_nRow);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoBlock.m_nCol);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoBlock.m_nBlock);  //Max block
	m_Label3.SetCaption(str);


	m_edit2=SysInfoBlock.m_nCol;
	m_edit3=SysInfoBlock.m_nRow;
	UpdateData(FALSE);

//	nColCt=5;  
	nColCt=SysInfoBlock.m_nCol+1; 
	m_grid1.SetCols(nColCt);
	
	nRowCt=SysInfoBlock.m_nRow+1; 
	m_grid1.SetRows(nRowCt);

//	m_grid1.SetCol(0); 
//	m_grid1.SetRow(0); 
//	str.Format("Block"); 
//	m_grid1.SetText(str); 

    
	for ( i = 1 ; i < nRowCt; i++) 
	{ 
	//	m_grid1.SetCol(0); 
	//	m_grid1.SetRow(i); 
		str.Format("Row %d", i); 
	//	m_grid1.SetText(str); 
		m_grid1.SetTextMatrix(i, 0, str);
	} 
 
    for (  i = 1 ; i < nColCt ; i++) 
    { 
       // m_grid1.SetRow(0); 
       // m_grid1.SetCol(i); 
		str.Format("Col %d", i); 
	//	m_grid1.SetText(str); 
		m_grid1.SetTextMatrix(0, i, str);

    } 

  
   for( i = 0 ; i <  SysInfoBlock.m_nRow ; i++) 
   { 
		for( k = 0 ; k <  SysInfoBlock.m_nCol ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoBlock.m_nData[i][k]);
		     m_grid1.SetTextMatrix(i+1, k+1, cpBuffer);
              /*
			 		m_grid1.SetCol(k+1); 
					m_grid1.SetRow(i+1); 
					m_grid1.SetText(cpBuffer); 
              */
		}
   }
 
  
  

	m_grid1.Refresh();
 	m_grid1.SetRedraw(TRUE);

  	m_nCurRow = 1;
	m_nCurCol = 1;

	m_grid1.GetWindowRect(m_rectGrid);
	ScreenToClient(m_rectGrid);

	m_dRatio = double(m_rectGrid.Width()) /10500.0;//8880.0;
	OnEditing = FALSE;

	
}


void CModeSub13::OnClickButton101() 
{ 
    CString str;
    int i,j,k;

    k=0;

    for( i=1; i<SysInfoBlock.m_nRow+1 ; i++)
    {
        for( j=1; j<SysInfoBlock.m_nCol+1 ; j++)
        {
            if( SysInfoBlock.m_nBlockAlign==1){
                k++;
            }
            else if( SysInfoBlock.m_nBlockAlign==2){  //ok
                k=SysInfoBlock.m_nCol*(i)-(j-1);
            }
            else if( SysInfoBlock.m_nBlockAlign==3){  //ok
                k=SysInfoBlock.m_nCol*(SysInfoBlock.m_nRow-i)+j ;
            }
            else if( SysInfoBlock.m_nBlockAlign==4){  //ok
                k=SysInfoBlock.m_nCol*(SysInfoBlock.m_nRow-i)+(SysInfoBlock.m_nCol-j+1);
            }
            else if( SysInfoBlock.m_nBlockAlign==5){  //ok
                k=SysInfoBlock.m_nRow*(j-1)+i;	
            }
            else if( SysInfoBlock.m_nBlockAlign==6){  //ok 
                k=SysInfoBlock.m_nRow*(SysInfoBlock.m_nCol-j)+i ; 
            }
            else if( SysInfoBlock.m_nBlockAlign==7){  //ok
                k=SysInfoBlock.m_nRow*(j)+(1-i);	
            }
            else if( SysInfoBlock.m_nBlockAlign==8){  //ok
                k=SysInfoBlock.m_nRow*(SysInfoBlock.m_nCol-j+1)+(1-i);
            } 


            m_grid1.SetRowSel(i);
            m_grid1.SetColSel(j);
            str.Format("%d", k);  
            m_grid1.SetTextMatrix(i, j, str);
        }
    }



    return ;
}

