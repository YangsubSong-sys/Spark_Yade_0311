#if !defined(AFX_MODERUN11_H__A913AB35_35A4_4EA4_91CA_9A53B006FA4E__INCLUDED_)
#define AFX_MODERUN11_H__A913AB35_35A4_4EA4_91CA_9A53B006FA4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeRun11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeRun11 dialog

class CModeRun11 : public CDialog
{
// Construction
public:
	CModeRun11(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeRun11)
	enum { IDD = IDD_MODE_RUN11 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeRun11)
	public:


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeRun11)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODERUN11_H__A913AB35_35A4_4EA4_91CA_9A53B006FA4E__INCLUDED_)
