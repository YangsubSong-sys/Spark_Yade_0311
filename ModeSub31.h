#if !defined(AFX_ModeSub31_H__965694F5_6228_492F_976F_71DE9818A083__INCLUDED_)
#define AFX_ModeSub31_H__965694F5_6228_492F_976F_71DE9818A083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub31.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSub31 dialog

class CModeSub31 : public CDialog
{
// Construction
public:
	CModeSub31(CWnd* pParent = NULL);   // standard constructor

 

//	CListBox	m_list1;
//	CComboBox	m_combo101;  //SYLEE130605
//	CLabelControl	m_Label1001;

    void OnListDisplay1();



// Dialog Data
	//{{AFX_DATA(CModeSub31)
	enum { IDD = IDD_MODE_SUB31_LIST1};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub31)

public:
	 virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub31)

	//	afx_msg void OnClickButton101();
		afx_msg void OnClickOk();
//		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//	afx_msg void OnClickButton103();

	//}}AFX_MSG

 	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSub31_H__965694F5_6228_492F_976F_71DE9818A083__INCLUDED_)
