#if !defined(AFX_MODEMAN11_H__965694F5_6228_492F_976F_71DE9818A083__INCLUDED_)
#define AFX_MODEMAN11_H__965694F5_6228_492F_976F_71DE9818A083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeMan11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeMan11 dialog

class CModeMan11 : public CDialog
{
// Construction
public:
	CModeMan11(CWnd* pParent = NULL);   // standard constructor

	void A_Language();//sylee151028

	int  nStop;
	int  nRsMode2;
	int  nlist1;  
	int  nStop2;

	CListBox	m_list1;
	CComboBox	m_combo101;  //SYLEE130605
	CLabelControl	m_Label1001;

    CLabelControl  m_Label1;	 
 	CLabelControl  m_Label2;
 	CLabelControl  m_Label3;
    CCommandButton  m_button1001; 
    CCommandButton  m_button103; 
    CCommandButton  m_buttonOk; 

	int OnManualShort1();//sylee131023
	int OnManualShort1_LV();//sylee131023	
	int OnManualShort1_HV();//sylee131023	



// Dialog Data
	//{{AFX_DATA(CModeMan11)
	enum { IDD = IDD_MODE_MAN11_SHORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeMan11)

public:
	 virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeMan11)

		afx_msg void OnClickButton101();
		afx_msg void OnClickOk();
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnClickButton103();

	//}}AFX_MSG

 	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMAN11_H__965694F5_6228_492F_976F_71DE9818A083__INCLUDED_)
