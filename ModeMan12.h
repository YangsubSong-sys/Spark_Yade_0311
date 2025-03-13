#if !defined(AFX_MODEMAN12_H__9B70DB49_36FD_4094_9012_74425D9B572B__INCLUDED_)
#define AFX_MODEMAN12_H__9B70DB49_36FD_4094_9012_74425D9B572B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeMan12.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeMan12 dialog

class CModeMan12 : public CDialog
{
// Construction
public:
	CModeMan12(CWnd* pParent = NULL);   // standard constructor


	void A_Language();//sylee151028

	int nStop;
	int nRsMode2;
	int  nStop2;
	int nlist1;  

	int nDut;

	 CLabelControl	m_Label8;
	 CLabelControl	m_Label9;

	CListBox	m_list1;
    CComboBox	m_combo101;  //SYLEE130605
	CLabelControl	m_Label1001;

	CLabelControl  m_Label1;	 
 	CLabelControl  m_Label2;
 	CLabelControl  m_Label3;
    CCommandButton  m_button1001; 
    CCommandButton  m_button1005; 
    CCommandButton  m_button103; 
    CCommandButton  m_buttonOk; 

	CEdit	m_edit101;
	CEdit	m_edit102;
 	CEdit	m_edit103;


	CString str2;

    int OnManualOpen();//sylee131023
    int OnManualOpen1_Run();//sylee131023	  // AUTO OPEN
    int OnManualOpen1_Run3();//sylee160126    // ONE PIN SEARCH 
    //	int OnManualOpen1_Run2_1(); //sylee161215-2   VB ALL   &  VC ONE PIN
    int OnManualOpen1_Run2_2(); //sylee161215-2   VC ALL   &  VB ONE PIN
    //	int OnManualOpen1_Run2_1_Double(); //sylee161216-1    //type1

    int OnManualOpen1_Run3_1_Double(); //sylee170112     //type1-1
    int OnManualOpen1_Run3_2();//sylee170112



    BOOL OnInit_File1(short type);

    //son220419_6 OnConvert_Real_R1()를 공통함수인 RCalCovert() 로 대체하고 삭제함.
    //double  OnConvert_Real_R1(int nMode, double R1_Raw, int part); //sylee161215-2     //son220412

    int nOpenMode1;
    int a1,a2,a3,a4;


    // Dialog Data
    //{{AFX_DATA(CModeMan12)
    enum { IDD = IDD_MODE_MAN12_OPEN };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeMan12)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeMan12)
	afx_msg void OnClickButton1001();
	afx_msg void OnClickButton1005();
	afx_msg void OnClickOk();
	afx_msg void OnClickButton103();

	afx_msg void OnButton101();
	afx_msg void OnButton102();
	afx_msg void OnButton104();
	afx_msg void OnButton105();
	afx_msg void OnButton107();	

	afx_msg void OnSelchangeCombo1();

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMAN12_H__9B70DB49_36FD_4094_9012_74425D9B572B__INCLUDED_)
