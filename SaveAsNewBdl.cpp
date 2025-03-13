// CSaveAsNewBdl.cpp : implementation file
//

#include "stdafx.h"
#include "bioptrocon.h"
#include "SaveAsNewBdl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveAsNewBdl dialog


CSaveAsNewBdl::CSaveAsNewBdl(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveAsNewBdl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveAsNewBdl)
	m_editNewBdlName = _T("");
	//}}AFX_DATA_INIT
}


void CSaveAsNewBdl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveAsNewBdl)
	DDX_Text(pDX, IDC_EDIT101, m_editNewBdlName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveAsNewBdl, CDialog)
	//{{AFX_MSG_MAP(CSaveAsNewBdl)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveAsNewBdl message handlers

BEGIN_EVENTSINK_MAP(CSaveAsNewBdl, CDialog)
    //{{AFX_EVENTSINK_MAP(CSaveAsNewBdl)
	ON_EVENT(CSaveAsNewBdl, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSaveAsNewBdl::OnClickOk()     //son220228 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);


	m_strNewBdlName = m_editNewBdlName; //son220228 MST  BDL SaveAs 기능 관련 추가

    CDialog::OnOK();
}
