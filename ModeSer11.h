#if !defined(AFX_MODESER11_H__478333AE_4DC8_4444_BF6E_6402CE586038__INCLUDED_)
#define AFX_MODESER11_H__478333AE_4DC8_4444_BF6E_6402CE586038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSer11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSer11 dialog

class CModeSer11 : public CDialog
{
// Construction
public:
	CModeSer11(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeSer11)
	enum { IDD = IDD_MODE_SER11 };
		// NOTE: the ClassWizard will add data members here

	CSliderCtrl	 m_slider1;
	CSliderCtrl	 m_slider2;
	CComboBox	m_combo1;
	CLabelControl	m_Label1;
	CLabelControl	m_Label2;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSer11)
  
protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CModeSer11)
		// NOTE: the ClassWizard will add member functions here
		afx_msg void OnSelchangeCombo1();
		afx_msg	void OnChangeSlider1();
 	    afx_msg	void OnChangeSlider2();

		afx_msg void OnClickOk(); 
		afx_msg void OnSave();
		afx_msg void Onload();


	//}}AFX_MSG
	
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESER11_H__478333AE_4DC8_4444_BF6E_6402CE586038__INCLUDED_)
