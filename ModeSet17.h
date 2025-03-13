#if !defined(AFX_ModeSet17_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_)
#define AFX_ModeSet17_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet17.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet17 dialog

class CModeSet17 : public CDialog
{
// Construction
public:
	CModeSet17(CWnd* pParent = NULL);   // standard constructor

  	CEdit	m_edit101;
	CEdit	m_edit102;
	CEdit	m_edit103;

  	CLabelControl  m_Label1;	 
 	CLabelControl  m_Label2;
 	CLabelControl  m_Label3;
	CLabelControl  m_Label4;

    CCommandButton  m_button101; 
    CCommandButton  m_button102; 
    CCommandButton  m_buttonOk; 
	
	void A_Language();//sylee151028
 

// Dialog Data
	//{{AFX_DATA(CModeSet17)
		enum { IDD = IDD_MODE_SET17_CYCLE };
		// NOTE: the ClassWizard will add data members here
	 
	   CButton	m_check1;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet17)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet17)
		// NOTE: the ClassWizard will add member functions here
	 	 	afx_msg void OnClickOk(); 
			afx_msg void OnSave();
 			afx_msg void Onload();
            afx_msg void OnCheck1();
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSet17_H__98510B36_3CD9_46B2_9E8F_1665931DFB2F__INCLUDED_)
