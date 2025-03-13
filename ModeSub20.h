#if !defined(AFX_MODESUB20_H__665BD2C9_2467_462D_ACBB_EE08F2AE2A53__INCLUDED_)
#define AFX_MODESUB20_H__665BD2C9_2467_462D_ACBB_EE08F2AE2A53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub20.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSub20 dialog

class CModeSub20 : public CDialog
{
// Construction
public:
	CModeSub20(CWnd* pParent = NULL);   // standard constructor

	
	void A_Language();//sylee151028

	CScrollBar	m_scrollbar1;
	CScrollBar	m_scrollbar2;
	CScrollBar	m_scrollbar3;
	CScrollBar	m_scrollbar4;

	CComboBox	m_combo1;
	CLabelControl	m_Label1;
	CLabelControl	m_Label2;
	CLabelControl	m_Label3;
	CLabelControl	m_Label4;
	CLabelControl	m_Label5;
	CLabelControl	m_Label6;

	
	CLabelControl  m_Label1001;
    CLabelControl  m_Label1002;
	CLabelControl  m_Label1003;
    CLabelControl  m_Label1004;
	CLabelControl  m_Label1005;
	CLabelControl  m_Label1006; 
    CLabelControl  m_Label1007;
	CLabelControl  m_Label1008;
 

	CCommandButton m_button1001;
	CCommandButton m_button1002;
	CCommandButton m_button1003;
	CCommandButton m_button1004;


// Dialog Data
	//{{AFX_DATA(CModeSub20)
	enum { IDD = IDD_MODE_SUB20_AUTOCount };
		// NOTE: the ClassWizard will add data members here
 

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub20)

protected:

    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub20)
		// NOTE: the ClassWizard will add member functions here
		afx_msg void OnSelchangeCombo1();
		afx_msg void OnClickOk(); 
		afx_msg void OnSave();
		afx_msg void Onload();
		afx_msg void OnClear1();
 
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	//}}AFX_MSG

 
	
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB20_H__665BD2C9_2467_462D_ACBB_EE08F2AE2A53__INCLUDED_)
