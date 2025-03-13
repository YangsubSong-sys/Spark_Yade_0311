// ModeRun11.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeRun11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeRun11 dialog


CModeRun11::CModeRun11(CWnd* pParent /*=NULL*/)
	: CDialog(CModeRun11::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeRun11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeRun11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeRun11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeRun11, CDialog)
	//{{AFX_MSG_MAP(CModeRun11)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeRun11 message handlers

 

void CModeRun11::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CModeRun11::OnOK() 
{
	// TODO: Add extra validation here

	int i;
	i=1;
	
	CDialog::OnOK();
}
