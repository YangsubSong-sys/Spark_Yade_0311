#if !defined(AFX_MODEMAN14_H__05704BAD_86F2_468D_AAD5_CE86062891C5__INCLUDED_)
#define AFX_MODEMAN14_H__05704BAD_86F2_468D_AAD5_CE86062891C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeMan14.h : header file
//
//#include "comidaq.h"//SYLEE
//#include "comiled.h"//SYLEE
//#include "comitoggleswitch.h"//SYLEE
//#include "ComiDaqComm.h" //SYLEE

//#define COMI_SD414	0xB414
//#define COMI_SD424	0xB424



/////////////////////////////////////////////////////////////////////////////
// CModeMan14 dialog

class CModeMan14 : public CDialog
{
// Construction
public:
	CModeMan14(CWnd* pParent = NULL);   // standard constructor


	CListBox	m_list1;
//	CListBox	m_list2;//sylee130310

	
	void A_Language();//sylee151028
 

	CLabelControl	m_LabelFault;		//son-32KPIN-20190902: m_Label101->m_LabelFault 로 변경

	CLabelControl  m_Label1;	 
 	CLabelControl  m_LabelStatus;		//son-32KPIN-20190902: m_Label1001 -> m_LabelStatus 로 변경
    CLabelControl  m_LabelTimeCnt;		//son-32KPIN-20190902: m_Label1002 -> m_LabelTimeCnt 로 변경
//IDC_CHECK1
 	CLabelControl  m_Label2; 

    CCommandButton  m_button101; 
	CCommandButton  m_button102; 
 
    CCommandButton  m_buttonOk; 

	CButton			m_checkBdl;			//son-32KPIN-20190902: m_check1 -> m_checkBdl로 변경





// Dialog Data
	//{{AFX_DATA(CModeMan14)
	enum { IDD = IDD_MODE_MAN14_PIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeMan14)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

public:

    //CComiDaq	m_ComiDaq3;

public:


	  int nPinSearchFlag;
	
	  int OnATest101(); 
	  int OnRun101();
	  int OnRun101_32K();       //son-32KPIN-20190902: 신규 작성
	  int OnRun101_test();
	  int OnEnd101(); 	 
	  int OnStop101(); 	  
	  int OnSet(); //stop
	  int OnCheck(); //stop

	  //	void ExitInOutBoard();

	    void ErrorPinDis();//sylee130310



protected:

	// Generated message map functions
	//{{AFX_MSG(CModeMan14)
		// NOTE: the ClassWizard will add member functions here
	    virtual BOOL OnInitDialog();
		afx_msg void OnButtonStart();		//son-32KPIN-20190902: OnButton101 -> OnButtonStart 로 변경
		afx_msg void OnButtonStop();		//son-32KPIN-20190902: OnButton102 -> OnButtonStop 으로 변경
		afx_msg void OnClickOk();
		afx_msg void OnCheck1();

	    afx_msg void OnTimer(UINT nIDEvent);

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMAN14_H__05704BAD_86F2_468D_AAD5_CE86062891C5__INCLUDED_)
