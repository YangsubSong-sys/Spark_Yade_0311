// ModeSub22.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub22.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeSub22 dialog


CModeSub22::CModeSub22(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub22::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub22)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub22::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub22)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub22, CDialog)
	//{{AFX_MSG_MAP(CModeSub22)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub22 message handlers
