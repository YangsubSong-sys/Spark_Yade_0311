// ModeSer11.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSer11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



 


#include "FileSysInfo.h"
 
extern CSysInfoSer11  SysInfoSer11;
extern CFileSysInfo FileSysInfo01;


/////////////////////////////////////////////////////////////////////////////
// CModeSer11 dialog


CModeSer11::CModeSer11(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSer11::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSer11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSer11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSer11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
			DDX_Control(pDX, IDC_COMBO01, m_combo1);
			DDX_Control(pDX, IDC_SLIDER1, m_slider1);
		    DDX_Control(pDX, IDC_SLIDER2, m_slider2);
			DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
		    DDX_Control(pDX, IDC_LABEL2,  m_Label2);
	//}}AFX_DATA_MAP
}


 



BEGIN_MESSAGE_MAP(CModeSer11, CDialog)
	//{{AFX_MSG_MAP(CModeSer11)
		// NOTE: the ClassWizard will add message map macros here

	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_SLIDER1, OnChangeSlider1)	
	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_SLIDER2, OnChangeSlider2)	 
	ON_CBN_SELCHANGE(IDC_COMBO01, OnSelchangeCombo1)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModeSer11 message handlers


BEGIN_EVENTSINK_MAP(CModeSer11, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSer11) 	

    ON_EVENT(CModeSer11, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE) 
	ON_EVENT(CModeSer11, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSer11, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE) 

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CModeSer11::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
 	m_slider1.SetRange(1,20);  //box      1box=4k, 8k
    m_slider2.SetRange(1,20);  //max point no
   
    Onload();   
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CModeSer11::PreTranslateMessage(MSG* pMsg) 
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

void CModeSer11::OnClickOk() 
{ 
 	FileSysInfo01.LoadSaveSer11(2);
 	CDialog::OnOK();
}
 

void CModeSer11::OnSave() 
{ 
 	FileSysInfo01.LoadSaveSer11(1);
}
 


void CModeSer11::Onload() 
{

    int i;
	CString str; 
//	char szText[100];

	FileSysInfo01.LoadSaveSer11(2);

	m_combo1.SetCurSel(SysInfoSer11.m_nPriMode);
 
	m_slider1.SetPos(SysInfoSer11.m_nBox);
	m_slider2.SetPos(SysInfoSer11.m_nPinTot);

	str.Format("%d", SysInfoSer11.m_nBox);
	m_Label1.SetCaption(str);
	
	i=  SysInfoSer11.m_nPinTot*4096;
	str.Format("%d", i);
	m_Label2.SetCaption(str);
  
}

 

void CModeSer11::OnChangeSlider1()	
{
 
 	int i;
	CString strValue;
	CString str;
 
	i=m_slider1.GetPos(); 
	m_slider1.SetPos(i);	
	SysInfoSer11.m_nBox=i;	
    strValue.Format("%d Box", i);
	m_Label1.SetCaption(strValue);
  
}


void CModeSer11::OnChangeSlider2()	
{

  int i;
	CString strValue;
	CString str;
 
	i=m_slider2.GetPos(); 
	m_slider2.SetPos(i);	
	SysInfoSer11.m_nPinTot=i;	
    strValue.Format("%d", i*4096);
	m_Label2.SetCaption(strValue);
  
}


void CModeSer11::OnSelchangeCombo1() 
{
    SysInfoSer11.m_nPriMode = m_combo1.GetCurSel();
}

 
