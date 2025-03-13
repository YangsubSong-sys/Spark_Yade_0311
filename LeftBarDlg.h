#if !defined(AFX_LEFTBARDLG_H__6CE46B00_526D_4E9A_B928_40EED743087A__INCLUDED_)
#define AFX_LEFTBARDLG_H__6CE46B00_526D_4E9A_B928_40EED743087A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftBarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog

class CLeftBarDlg : public CDialog
{
// Construction
public:
	CLeftBarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLeftBarDlg)
	enum { IDD = IDD_Dialog_LeftBar };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeftBarDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTBARDLG_H__6CE46B00_526D_4E9A_B928_40EED743087A__INCLUDED_)
