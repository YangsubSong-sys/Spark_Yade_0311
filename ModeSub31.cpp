// ModeSub31.cpp : implementation file
//

#include "stdafx.h" 
#include "BiOpTroCon.h"
#include "ModeSub31.h"
 
 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

  


/////////////////////////////////////////////////////////////////////////////
// CModeSub31 dialog


CModeSub31::CModeSub31(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub31::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub31)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub31::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub31)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub31, CDialog)
	//{{AFX_MSG_MAP(CModeSub31)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

 
BEGIN_EVENTSINK_MAP(CModeSub31, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub31)
 //	ON_EVENT(CModeSub31, IDC_BUTTON1001, -600 /* Click */, OnClickButton101, VTS_NONE)
 //	ON_EVENT(CModeSub31, IDC_BUTTON103, -600 /* Click */, OnClickButton103, VTS_NONE)
 	ON_EVENT(CModeSub31, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModeSub31 message handlers

BOOL CModeSub31::OnInitDialog() 
{
	CDialog::OnInitDialog(); 

	OnListDisplay1();

	return TRUE;       
}

 BOOL CModeSub31::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

 	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_RETURN)) {  
		return TRUE;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}



 void  CModeSub31::OnClickOk() 
{
	 CDialog::OnOK();
	  
}

void  CModeSub31::OnListDisplay1()
{	
 
 
		CFile file;
 	 
		if ((file.Open("d:\\log\\SelfAuto_2WTr.CSV", CFile::modeRead))==NULL) {
			AfxMessageBox("  Error No 1019, Open File Reading   !.", MB_OK);
			return;
		}
 
 
		char buffer[65535];
		::ZeroMemory(buffer, sizeof(buffer));  
		file.Read(buffer, 65534);	
		file.Close();
		SetDlgItemText(IDC_DATA, buffer);
 
 
}
