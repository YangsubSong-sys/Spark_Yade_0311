#if !defined(AFX_ModeSet13_H__98510B36_3CD9_46B2_9E8F_1665931DFB1F__INCLUDED_)
#define AFX_ModeSet13_H__98510B36_3CD9_46B2_9E8F_1665931DFB1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet13.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet13 dialog

class CModeSet13 : public CDialog
{
// Construction
public:
	CModeSet13(CWnd* pParent = NULL);   // standard constructor

 


// Dialog Data
	//{{AFX_DATA(CModeSet13)
	enum { IDD = IDD_MODE_SET13_SET1 };
	CEdit	m_edit_NearByOpenRate;	//son230728
	CButton	m_check_NearbyOpenMeas;
		CComboBox	m_combo1;
		CComboBox	m_combo2;
		CComboBox	m_combo3;
		CComboBox	m_combo4;
		CComboBox	m_combo5;
		CComboBox	m_combo111;
	    CComboBox	m_combo_SparkMode;  //son200908 NEW_SPARK
		CComboBox	m_combo113;
		CComboBox	m_combo114;
		CComboBox	m_combo115;
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
	  	CButton	m_check15;
	  	CButton	m_check16;
	  	CButton	m_check17;
	  	CButton	m_check18;
	  	CButton	m_check19;
	  	CButton	m_check20;
    	CButton	m_checkUltLog;       //son210215 ULTLog for Samsung Electro-Mecha
    	CButton	m_check_UseLotName_4wLog;   //son220124 for 제이텍. LotName을 4wLog폴더 Name에 추가.
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSet13)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSet13)
	 	 	afx_msg void OnClickOk();
			afx_msg void OnSelchangeCombo1();
			afx_msg void OnSelchangeCombo2();
			afx_msg void OnSelchangeCombo3();
			afx_msg void OnSelchangeCombo4();
			afx_msg void OnSelchangeCombo5();	
			afx_msg void OnSelchangeCombo111();
			afx_msg void OnSelchangeComboSparkMode();       //son200908 NEW_SPARK
			afx_msg void OnSelchangeCombo113();
			afx_msg void OnSelchangeCombo114();
			afx_msg void OnSelchangeCombo115();	
			afx_msg void OnCheck1();
	        afx_msg void OnCheck2();
			afx_msg void OnCheck3();
		    afx_msg void OnCheck4();
			afx_msg void OnCheck5();
	        afx_msg void OnCheck6();
			afx_msg void OnCheck7();
			afx_msg void OnCheck8();
			afx_msg void OnCheck9();  //sylee131212
			afx_msg void OnCheck10();
			afx_msg void OnCheck11();
	        afx_msg void OnCheck12();
			afx_msg void OnCheck13();
		    afx_msg void OnCheck14();
			afx_msg void OnCheck15();
	        afx_msg void OnCheck16();
			afx_msg void OnCheck17();
			afx_msg void OnCheck18();
			afx_msg void OnCheck19();
			afx_msg void OnCheck20();
			afx_msg void OnSave();
        	afx_msg void OnCheckUltLog();
        	afx_msg void OnCheck_UseLotName_4wLog();    //son220124
            afx_msg void OnCheckNearbyOpenMeas();       //son230712
            afx_msg void OnChangeEditNearbyOpenrate();  //son230728
	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()


public:
    void Onload();  //son230728
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ModeSet13_H__98510B36_3CD9_46B2_9E8F_1665931DFB1F__INCLUDED_)
