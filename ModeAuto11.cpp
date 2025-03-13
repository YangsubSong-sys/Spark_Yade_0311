// ModeAuot11.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeAuto11.h"
//#include "print.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CPrint  Print1;
 
/////////////////////////////////////////////////////////////////////////////
// CModeAuot11 dialog


CModeAuto11::CModeAuto11(CWnd* pParent /*=NULL*/)
	: CDialog(CModeAuto11::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeAuto11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeAuto11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeAuto11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CModeAuto11, CDialog)
	//{{AFX_MSG_MAP(CModeAuto11)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


 
BEGIN_EVENTSINK_MAP(CModeAuto11, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeAuto11)
 	ON_EVENT(CModeAuto11, IDC_BUTTON1001, -600 /* Click */, OnClickButton1001, VTS_NONE)
 	ON_EVENT(CModeAuto11, IDC_BUTTON1002, -600 /* Click */, OnClickButton1002, VTS_NONE)
	ON_EVENT(CModeAuto11, IDC_BUTTON1003, -600 /* Click */, OnClickButton1003, VTS_NONE)
	ON_EVENT(CModeAuto11, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModeAuto11 message handlers
void CModeAuto11::OnClickButton1001() 
{
//  Print1.OnStartPrinter();//SYLEETEST


}

void CModeAuto11::OnClickButton1002() 
{
 	
// Print1.OnStartPrinter2();//SYLEETEST

}


void CModeAuto11::OnClickButton1003() 
{
 	
// Print1.OnStartPrinter3();//SYLEETEST

}

void CModeAuto11::OnClickOk() 
{
 	
	CDialog::OnOK(); 

}
