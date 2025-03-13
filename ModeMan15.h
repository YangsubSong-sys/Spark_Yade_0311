#if !defined(AFX_MODEMAN15_H__2FDC6F61_1A4C_4E2A_8682_5646F459798C__INCLUDED_)
#define AFX_MODEMAN15_H__2FDC6F61_1A4C_4E2A_8682_5646F459798C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeMan15.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeMan15 dialog

class CModeMan15 : public CDialog
{
// Construction
public:
	CModeMan15(CWnd* pParent = NULL);   // standard constructor


    CListBox	m_list1;
	CProgressCtrl	m_Progress1;
	CLabelControl	m_Label1;
	CLabelControl	m_Label2;

	int  nStop2;
	int nlist1;  
	
 	CButton	m_check1;


// Dialog Data
	//{{AFX_DATA(CModeMan15)
	enum { IDD = IDD_MODE_MAN15 };
		// NOTE: the ClassWizard will add data members here


	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeMan15)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeMan15)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnClickButton101();
	afx_msg void OnClickButton102();
	afx_msg void OnClickButton103();
	afx_msg void OnClickOk();
	virtual BOOL OnInitDialog();


	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMAN15_H__2FDC6F61_1A4C_4E2A_8682_5646F459798C__INCLUDED_)
