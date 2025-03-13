#if !defined(AFX_ModeSet19_H__98510B36_3CD9_46B2_9E8F_1665931DFB3F__INCLUDED_)
#define AFX_ModeSet19_H__98510B36_3CD9_46B2_9E8F_1665931DFB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet19.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet19 dialog

class CModeSet19 : public CDialog
{
// Construction
public:
	CModeSet19(CWnd* pParent = NULL);   // standard constructor

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
	CEdit	m_edit113;//sylee170228
	CEdit	m_edit115;//sylee170228

    CEdit	m_edit126;//sylee180706-2
    CEdit	m_edit127;//sylee180706-2
    CEdit	m_edit128;//sylee180706-2
    CEdit	m_edit129;//sylee180706-2
    CEdit	m_edit130;//sylee180706-2
    CEdit	m_edit131;//sylee180706-2
    CEdit	m_edit132;//sylee180706-2
    CEdit	m_edit133;//sylee201027
    CEdit	m_edit134;//sylee201027
    CEdit	m_edit135;//sylee230214    //sylee230420
	CEdit	m_edit136;//sylee230214
	CEdit	m_edit137;//sylee230214
	CEdit	m_edit138;//sylee230225
	CEdit	m_edit139;//sylee230225
	CEdit	m_edit140;//sylee230817


// Dialog Data
	//{{AFX_DATA(CModeSet19)
	enum { IDD = IDD_MODE_SET19_FASTMODE2 };
		// NOTE: the ClassWizard will add data members here
	 
	   CButton	m_check1;
	   CButton	m_check2;
	   CButton	m_check3;
	   CButton	m_check4;
	   CButton	m_check5; 
	   CButton	m_check6; 
	   CButton	m_check7;    //sylee230420
	   CButton	m_check8;    //sylee230420
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet19)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet19)
		// NOTE: the ClassWizard will add member functions here
	 	 	afx_msg void OnClickOk(); 
			afx_msg void OnSave();
 			afx_msg void Onload();

            afx_msg void OnCheck1();
            afx_msg void OnCheck2();
            afx_msg void OnCheck3();
            afx_msg void OnCheck4();
            afx_msg void OnCheck5();
			afx_msg void OnCheck6();
			afx_msg void OnCheck7();   	   //sylee230420
			afx_msg void OnCheck8();   	   //sylee230420


	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSet19_H__98510B36_3CD9_46B2_9E8F_1665931DFB3F__INCLUDED_)
