#if !defined(AFX_MODEMAN17_H__129A8D6E_40A5_4889_83DD_C4E9D9AD5068__INCLUDED_)
#define AFX_MODEMAN17_H__129A8D6E_40A5_4889_83DD_C4E9D9AD5068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeMan17.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeMan17 dialog

class CModeMan17 : public CDialog
{
// Construction
public:
	CModeMan17(CWnd* pParent = NULL);   // standard constructor


	int nStop;
	int nRsMode2;
	int nStop2;
	int nlist1;  

	CListBox	m_list1;

// Dialog Data
	//{{AFX_DATA(CModeMan17)
	enum { IDD = IDD_MODE_MAN17 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeMan17)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeMan17)
	afx_msg void OnClickButton1001();
	afx_msg void OnClickOk();
	afx_msg void OnClickButton103();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_MODEMAN17_H__129A8D6E_40A5_4889_83DD_C4E9D9AD5068__INCLUDED_)
