#if !defined(AFX_MODEAUOT11_H__626377A1_090C_4D8B_BF6B_C0A648D10FC0__INCLUDED_)
#define AFX_MODEAUOT11_H__626377A1_090C_4D8B_BF6B_C0A648D10FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeAuot11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeAuot11 dialog

class CModeAuto11 : public CDialog
{
// Construction
public:
	CModeAuto11(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeAuto11)
	enum { IDD = IDD_MODE_AUTO11 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeAuto11)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeAuto11)
		// NOTE: the ClassWizard will add member functions here

	
	afx_msg void OnClickButton1001();
	afx_msg void OnClickButton1002();
	afx_msg void OnClickButton1003();
	afx_msg void OnClickOk();

	//}}AFX_MSG
 	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEAUTO11_H__626377A1_090C_4D8B_BF6B_C0A648D10FC0__INCLUDED_)
