#if !defined(AFX_MODEMAN16_H__A89B0806_150B_4A55_BA55_4916593E41A1__INCLUDED_)
#define AFX_MODEMAN16_H__A89B0806_150B_4A55_BA55_4916593E41A1__INCLUDED_


 
#include "comidaq.h"
#include "comiled.h"
#include "comitoggleswitch.h"
 
#include "Comi_Dio_InOut.h"     //son230914 Comi DIO 관련 define을 전용 헤더로 분리해서 include.
 


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeMan16.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeMan16 dialog

class CModeMan16 : public CDialog
{

public:

 	//DECLARE_DYNCREATE(CModeMan16)//SYLEE20110310


	void A_Language();//sylee151028

	//son230914 CCOMILed *m_LedArr[8];
	//son230914 CComiToggleSwitch *m_BtnArr[8];
	CCOMILed *m_LedArr[DIN_4W_REF_INIT +1];      //son231016  9: (DIN_4W_REF_INIT +1) 
    CComiToggleSwitch *m_BtnArr[DOUT_NUM];   //son231016 8: DOUT_NUM

	void ExitInOutBoard();


// Construction
public:

	CModeMan16(CWnd* pParent = NULL);   // standard constructor

 
	CComiDaq	m_ComiDaq1;

// Dialog Data
	//{{AFX_DATA(CModeMan16)
	enum { IDD = IDD_MODE_MAN16_DIGITAL };
	CCOMILed	m_LED0;
	CCOMILed	m_LED1;
	CCOMILed	m_LED2;
	CCOMILed	m_LED3;
	CCOMILed	m_LED4;
	CCOMILed	m_LED5;
	CCOMILed	m_LED6;
	CCOMILed	m_LED7;
	CCOMILed	m_LED_4WRef_Init;		//son231017

	CComiToggleSwitch	m_Btn0;			//son231017
	CComiToggleSwitch	m_Btn1;
	CComiToggleSwitch	m_Btn2;
	CComiToggleSwitch	m_Btn3;
	CComiToggleSwitch	m_Btn4;
	CComiToggleSwitch	m_Btn5;
	CComiToggleSwitch	m_Btn6;
	CComiToggleSwitch	m_Btn7;

	CLabelControl m_Label101;
	CLabelControl m_Label102;
    CCommandButton	m_button1001;
    CCommandButton  m_buttonOk; 
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeMan16)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CModeMan16)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

 	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDoOut(); 
	afx_msg void OnClickOk(); 
	 
	//SYLEE	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMAN16_H__A89B0806_150B_4A55_BA55_4916593E41A1__INCLUDED_)
