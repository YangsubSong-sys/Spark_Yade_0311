#if !defined(AFX_PASSWORD1_H__7C602951_190B_498D_91C9_4D4EBD6F0FDE__INCLUDED_)
#define AFX_PASSWORD1_H__7C602951_190B_498D_91C9_4D4EBD6F0FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassWord1.h : header file
//


//son200724: 4W-SamplingCount 비번입력기능
#include "ChildView9.h"        // Bioptro passwd 입력을 위한 클래스 include
extern int g_passDlgLogOnUser;


/////////////////////////////////////////////////////////////////////////////
// CPassWord1 dialog

class CPassWord1 : public CDialog
{
// Construction
public:
	CPassWord1(CWnd* pParent = NULL);   // standard constructor

	CEdit	m_edit101;



// Dialog Data
	//{{AFX_DATA(CPassWord1)
	enum { IDD = IDD_DIALOG_Password };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassWord1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassWord1)
	   afx_msg void OnClickOk();
	    afx_msg void OnOk1(LPDISPATCH Cancel, LPDISPATCH Data, float X, float Y, long DragState, LPDISPATCH Effect, short Shift);
 
	//}}AFX_MSG
    DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORD1_H__7C602951_190B_498D_91C9_4D4EBD6F0FDE__INCLUDED_)
