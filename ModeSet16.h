#if !defined(AFX_ModeSet16_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_)
#define AFX_ModeSet16_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet16.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet16 dialog

class CModeSet16 : public CDialog
{
// Construction
public:
	CModeSet16(CWnd* pParent = NULL);   // standard constructor

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

// Dialog Data
	//{{AFX_DATA(CModeSet16)
	enum { IDD = IDD_MODE_SET16_FASTMODE1 };
		// NOTE: the ClassWizard will add data members here
	 
	   CButton	m_check1;
	   CButton	m_check2;
	   CButton	m_check3;
	   CButton	m_check4;
	   CButton	m_check5; 
	   CButton	m_check6;
		



 

	//}}AFX_DATA


 			//afx_msg 
				void Onload();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet16)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet16)
		// NOTE: the ClassWizard will add member functions here
	 	 	afx_msg void OnClickOk(); 
			afx_msg void OnSave();


            afx_msg void OnCheck1();
            afx_msg void OnCheck2();
            afx_msg void OnCheck3();
            afx_msg void OnCheck4();
            afx_msg void OnCheck5();
			afx_msg void OnCheck6();




	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSet16_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_)
