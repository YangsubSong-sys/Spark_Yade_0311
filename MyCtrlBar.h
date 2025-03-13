#if !defined(AFX_MYCTRLBAR_H__0F52A581_7DBA_4F87_AA24_5A7671F64222__INCLUDED_)
#define AFX_MYCTRLBAR_H__0F52A581_7DBA_4F87_AA24_5A7671F64222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCtrlBar.h : header file
//

#include "LeftBarDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyCtrlBar window

class CMyCtrlBar : public CControlBar
{
// Construction
public:
	CLeftBarDlg		m_wndChildLeft;

	CMyCtrlBar();
	~CMyCtrlBar();

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCtrlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
//	virtual ~CMyCtrlBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCTRLBAR_H__0F52A581_7DBA_4F87_AA24_5A7671F64222__INCLUDED_)
