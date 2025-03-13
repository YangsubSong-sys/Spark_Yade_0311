// PassWord1.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "PassWord1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassWord1 dialog

extern CString LoadMessage2(UINT uFileType, CString strFile, UINT uNum);//sylee180731-1

int nFlag_PassOn;//sylee160219-3
int nFlag_PassOnNo;//sylee160219-3	

//son200724: 4W-SamplingCount 비번입력기능
int g_passDlgLogOnUser;     //son200724: 신규추가

CPassWord1::CPassWord1(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWord1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassWord1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPassWord1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassWord1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
			DDX_Control(pDX, IDC_EDIT101,   m_edit101);
		
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassWord1, CDialog)
	//{{AFX_MSG_MAP(CPassWord1)
		// NOTE: the ClassWizard will add message map macros here
			 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassWord1 message handlers
BEGIN_EVENTSINK_MAP(CPassWord1, CDialog)
    //{{AFX_EVENTSINK_MAP(CPassWord1) 
	ON_EVENT(CPassWord1, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
 	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()





void CPassWord1::OnClickOk() //SYLEE180731-1
{
 	CString str1,str2;
	char szText[20];

	m_edit101.GetWindowText( szText,10);
	str1.Format("%s",szText);

    //son Setup->Recipe 창 비번 입력 기능  : ChildView2에서 사용
	if(nFlag_PassOnNo==11){ 
		str2.Format("%s",LoadMessage2(1,"Title", 101));
	}
	//son Setup->4W 창 비번 입력 기능  : ChildView2에서 사용
	else if(nFlag_PassOnNo==12){ 
		str2.Format("%s",LoadMessage2(1,"Title", 102));
	}

    //son200724: 4W-SamplingCount 비번입력기능
    //son Setup->4W->4W-SamplingCount edit 박스 비번입력기능  : ModeSub17에서 사용
	else if(nFlag_PassOnNo==13){ 
		str2.Format("%s",LoadMessage2(1,"Title", 103));
	}
//son230829 begin:
	else if(nFlag_PassOnNo==14){ 
		str2.Format("%s",LoadMessage2(1,"Title", 104));
	}
	else if(nFlag_PassOnNo==15){ 
		str2.Format("%s",LoadMessage2(1,"Title", 105));
	}
	else if(nFlag_PassOnNo==16){ 
		str2.Format("%s",LoadMessage2(1,"Title", 106));
	}
	else if(nFlag_PassOnNo==17){ 
		str2.Format("%s",LoadMessage2(1,"Title", 107));
	}
	else if(nFlag_PassOnNo==18){ 
		str2.Format("%s",LoadMessage2(1,"Title", 108));
	}
	else if(nFlag_PassOnNo==19){ 
		str2.Format("%s",LoadMessage2(1,"Title", 109));
	}
//son230829 end
	else{
	    str2.Format("%s",LoadMessage2(1,"Title", 101));
    }
    nFlag_PassOn=0; 
	 if(strcmp(str1,str2)==0){  
		 nFlag_PassOn=2; 	 
 	}
 
	CDialog::OnOK();	
}

 
