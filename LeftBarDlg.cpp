// LeftBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "LeftBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog


CLeftBarDlg::CLeftBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLeftBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeftBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLeftBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftBarDlg, CDialog)
	//{{AFX_MSG_MAP(CLeftBarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg message handlers

