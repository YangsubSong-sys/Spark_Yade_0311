#if !defined(AFX_MODESUB72_H__585FE8AD_1DB1_4F2A_A034_54E39D33EDD3__INCLUDED_)
#define AFX_MODESUB72_H__585FE8AD_1DB1_4F2A_A034_54E39D33EDD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub72.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSub72 dialog

class CModeSub72 : public CDialog
{
// Construction  
public:

	CModeSub72(CWnd* pParent = NULL);   // standard constructor

	CListBox m_list1;
	CButton	 m_check1;
	CButton	 m_check2;

 	CEdit	m_edit103;
	CEdit	m_edit104;

	CEdit	m_edit201;
	CEdit	m_edit202;
	CEdit	m_edit203;
	CEdit	m_edit204;
	CEdit	m_edit205;

	CEdit	m_edit301;
	CEdit	m_edit302;

	CEdit	m_edit303;
	CEdit	m_edit304;

	CEdit	m_edit401;
	CEdit	m_edit402;

	CButton m_Button110;
	CButton m_Button120;
	CButton m_Button140;
	CButton m_Button141;
	CButton m_Button142;
	CButton m_Button903;

	CLabelControl	m_Label207;
	CLabelControl	m_Label234;
	CLabelControl	m_Label211;
	CLabelControl	m_Label219;
	CLabelControl	m_Label226;
	CLabelControl	m_Label227;

	CLabelControl	m_Label235;


	int nIf;
	int nICo;
	int m_nAutoFileDisplay;

	double V1;  //vb
	double Y1;  //i
	double X1;  //vc
	double R1;  //vc
	double W1;  //4WIRE
	double P1;  //PULSE

	double lIset;

	int nMode1,nModeIR, nModeCC, nModeVB_AC;
 
 
	int nModeVB_OUT2,nModeVB_OUT3, nModeVB_OUT4, nModeVB_OUT5;


 
	int OnEmbed_Freq_Display(int nCh);   
	void On_Test1_Run1();
	void On_Test1_log1(); 
	void On_Initial_Display1(); 
	void Oninit1();
	void On_Get_SetValue(); 
	void OnCheck1();
	void OnCheck2();
 

//  unsigned long  AVB_PULSE_ADC();/// I READING    manual  

// Dialog Data
	//{{AFX_DATA(CModeSub72)
	enum { IDD = IDD_MODE_SUB72_MANUAL_Embeded };


	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub72)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub72)

	afx_msg void OnRadio01();
 	afx_msg void OnRadio02();
	afx_msg void OnRadio03();
	afx_msg void OnRadio04();
 	afx_msg void OnRadio05();
	afx_msg void OnRadio06();
	afx_msg void OnRadio07();
 	afx_msg void OnRadio08();
	afx_msg void OnRadio09();
	afx_msg void OnRadio10();
 	afx_msg void OnRadio11();
	afx_msg void OnRadio12();
	afx_msg void OnRadio13();
	afx_msg void OnRadio14();
    afx_msg void OnRadio15();
	afx_msg void OnRadio16();
	afx_msg void OnRadio18();
 	afx_msg void OnRadio19();
	afx_msg void OnRadio20();
	afx_msg void OnRadio21();
 	afx_msg void OnRadio22();
	afx_msg void OnRadio23();
	afx_msg void OnRadio24();
 	afx_msg void OnRadio25();
 	afx_msg void OnRadio26();
 	afx_msg void OnRadio27(); 
    afx_msg void OnRadio28();
 	afx_msg void OnRadio29();
 	afx_msg void OnRadio30();
	afx_msg void OnRadio31();
	afx_msg void OnRadio32();
 	afx_msg void OnRadio33();
	afx_msg void OnRadio34();
	afx_msg void OnRadio35();
 	afx_msg void OnRadio36();
 	afx_msg void OnRadio37();
 	afx_msg void OnRadio38();
	afx_msg void OnRadio39();
 	afx_msg void OnRadio40();
 	afx_msg void OnRadio41();
 	afx_msg void OnRadio42();

 	afx_msg void OnRadio43();
 	afx_msg void OnRadio44();
 	afx_msg void OnRadio45();
 	afx_msg void OnRadio46();

	afx_msg void OnButton127();

	afx_msg void OnButton110();
	afx_msg void OnButton120();
    afx_msg void OnButton140();
	afx_msg void OnButton141();
    afx_msg void OnButton142();
	afx_msg void OnButton903();	
	
	afx_msg void OnButton206();
	afx_msg void OnButton303();
	afx_msg void OnButton304();
	afx_msg void OnButton506();
	afx_msg void OnButton601();

	afx_msg void On_Test1();
		
	afx_msg void OnClickOk();
	

	//}}AFX_MSG
 
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB72_H__585FE8AD_1DB1_4F2A_A034_54E39D33EDD3__INCLUDED_)
