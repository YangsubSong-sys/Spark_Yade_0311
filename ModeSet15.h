#if !defined(AFX_ModeSet15_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_)
#define AFX_ModeSet15_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet15.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet15 dialog

class CModeSet15 : public CDialog
{
// Construction
public:
	CModeSet15(CWnd* pParent = NULL);   // standard constructor

  	CEdit	m_edit101;
	CEdit	m_edit102;
	CEdit	m_edit103;
	CEdit	m_edit104;
	CEdit	m_edit105;	
	CEdit	m_edit106;
	CEdit	m_edit107;
  	CEdit	m_edit108;
	CEdit	m_edit109;
	CEdit	m_edit110;
	CEdit	m_edit111;
	CEdit	m_edit112;	
	CEdit	m_edit113;
	CEdit	m_edit115;

// Dialog Data
	//{{AFX_DATA(CModeSet15)
	enum { IDD = IDD_MODE_SET15_PRO1 };
		// NOTE: the ClassWizard will add data members here

 
	



 

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet15)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet15)
		// NOTE: the ClassWizard will add member functions here
	 	 	afx_msg void OnClickOk(); 
			afx_msg void OnSave();
 			afx_msg void Onload();





	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSet15_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_)
