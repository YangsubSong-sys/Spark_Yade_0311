#if !defined(AFX_ModeSet21_H__98510B36_3CD9_46B2_9E8F_1665931DFB3F__INCLUDED_)
#define AFX_ModeSet21_H__98510B36_3CD9_46B2_9E8F_1665931DFB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet21.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet21 dialog

class CModeSet21 : public CDialog
{
// Construction
public:
	CModeSet21(CWnd* pParent = NULL);   // standard constructor

 /* 
	CEdit	m_edit101;
	CEdit	m_edit102;
	CEdit	m_edit103;
 */ 

// Dialog Data
	//{{AFX_DATA(CModeSet21)
	enum { IDD = IDD_MODE_SET21_SWOP1 };
	   CButton	m_check1;
	   CButton	m_check2;
	   CButton	m_check3;
       CButton	m_check4;
       CButton	m_check5;
       CButton	m_check6;
	   CButton	m_check7;
	   CButton	m_check8;
	   CButton	m_check9;
	   CButton	m_check10;
       CButton	m_check11;
       CButton	m_check12;
       CButton	m_check13;
	   CButton	m_check14;
        BOOL	m_bPrtBasic;       //son230720
        BOOL	m_bPrtLevel1;      //son230720
        BOOL	m_bPrtLevel2;      //son230720
        BOOL	m_bPrtLevel3;      //son230720
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet21)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet21)
	 	 	afx_msg void OnClickOk(); 
			afx_msg void OnSave();
 			afx_msg void Onload();
            afx_msg void OnCheck1();
		    afx_msg void OnCheck2();
		    afx_msg void OnCheck3();
		    afx_msg void OnCheck4();
		    afx_msg void OnCheck5();
			afx_msg void OnCheck6();
		    afx_msg void OnCheck7();
            afx_msg void OnCheckPrtBasic();         //son230720
            afx_msg void OnCheckPrtLevel1();        //son230720
            afx_msg void OnCheckPrtLevel2();        //son230720
            afx_msg void OnCheckPrtLevel3();        //son230720
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSet21_H__98510B36_3CD9_46B2_9E8F_1665931DFB3F__INCLUDED_)
