#if !defined(AFX_CHILDVIEW5_H__DBB7654E_4A4F_4290_8D30_B3B47DC212A2__INCLUDED_)
#define AFX_CHILDVIEW5_H__DBB7654E_4A4F_4290_8D30_B3B47DC212A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView5.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView5 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

extern int g_nLogOnUser1;
extern int Get_nCardPinMax();      //son231227 nHW_CardPin_Check() -> Get_nCardPinMax() in ChildView8
extern int GetHwPinNo_Old(int card, int cardPin );  //son231222_2   //son240125
extern char* getStrSelfTestCase(int nCase);         //son240201
extern char* getStrSelfTestCase2(int nProStep1);    //son240205
 


class CChildView5 : public CFormView
{
//protected:


public:
           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView5)



public:

	CChildView5();

	
// Form Data
	//{{AFX_DATA(CChildView5)
	enum { IDD = IDD_Form_View5_SELF };  

	CButton	m_check_System;		//son-32KPIN-20190902: m_check1 -> m_check_System 으로 변경
	CButton	m_check_VB;			//son-32KPIN-20190902: m_check2 -> m_check_VB 로 변경
	CButton	m_check_AD;			//son-32KPIN-20190902: m_check3 -> m_check_AD 로 변경
	CButton	m_check_2wIO;		//son-32KPIN-20190902: m_check4 -> m_check_2wIO 로 변경
	CButton	m_check_4wIO; 		//son-32KPIN-20190902: m_check5 -> m_check_4wIO 로 변경
	CButton	m_check_2wLeak;     //sylee240509

	CProgressCtrl	m_progress_System;	//son-32KPIN-20190902: m_Progress1 -> m_progress_System
	CProgressCtrl	m_progress_VB;		//son-32KPIN-20190902: m_Progress2 -> m_progress_VB
	CProgressCtrl	m_progress_AD;		//son-32KPIN-20190902:	m_Progress3 -> m_progress_AD
	CProgressCtrl	m_progress_2wIO;	//son-32KPIN-20190902: m_Progress4 -> m_progress_2wIO
	CProgressCtrl	m_progress_4wIO;	//son-32KPIN-20190902: m_Progress5 -> m_progress_4wIO
    CProgressCtrl   m_progress_Leak;    //sylee240509
 
	CListBox		m_list1;

	CLabelControl	m_Label_System;	//son-32KPIN-20190902: m_Label1001 -> m_Label_System
	CLabelControl	m_Label_VB;		//son-32KPIN-20190902: m_Label1002 -> m_Label_VB
	CLabelControl	m_Label_AD;		//son-32KPIN-20190902: m_Label1003 -> m_Label_AD
	CLabelControl	m_Label_2wIO;	//son-32KPIN-20190902: m_Label1004 -> m_Label_2wIO
    CLabelControl	m_Label_4wIO;	//son-32KPIN-20190902: m_Label1005 -> m_Label_4wIO
	CLabelControl   m_Label_2wLeak; //sylee040509

	CEdit	m_edit101;

	CCommandButton  m_button305;
	CCommandButton  m_button302;
	CCommandButton  m_button101;
	CCommandButton  m_button102;
	CCommandButton  m_button103;
	CCommandButton  m_button104;
	CCommandButton  m_button105;
	CCommandButton  m_button201;
	CCommandButton  m_button202;
	CCommandButton  m_button106;
	CCommandButton  m_button107;

	CLabelControl	m_Label1;
	CLabelControl	m_Label101;
 	CLabelControl	m_Label2;
 	CCommandButton  m_button1001;
  	CCommandButton  m_button1005;
 	CCommandButton  m_buttonOk; 

	//}}AFX_DATA




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView5)
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView5();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Generated message map functions
	//{{AFX_MSG(CChildView5)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheck_System();	//son-32KPIN-20190902: OnCheck1 -> OnCheck_System 으로 
	afx_msg void OnCheck_VB();      //son-32KPIN-20190902: OnCheck2 -> OnCheck_VB 로 변경
	afx_msg void OnCheck_AD();      //son-32KPIN-20190902: OnCheck3 -> OnCheck_AD 로 변경
	afx_msg void OnCheck_2wIO();    //son-32KPIN-20190902: OnCheck4 -> OnCheck_2wIO 로 변경
	afx_msg void OnCheck_4wIO();    //son-32KPIN-20190902: OnCheck5 -> OnCheck_4wIO 로 변경

    afx_msg void OnButtonSystemCheck();	//son-32KPIN-20190902: OnButton101 ->OnButtonSystemCheck 로 변경
	afx_msg void OnButtonVbCheck();		//son-32KPIN-20190902: OnButton102 ->OnButtonVbCheck 로 변경 
	afx_msg void OnButtonAdCheck();		//son-32KPIN-20190902: OnButton103 ->OnButtonAdCheck 로 변경 
	afx_msg void OnButton_2wIoCheck();	//son-32KPIN-20190902: OnButton104 ->OnButton_2wIoCheck 로 변경
	afx_msg void OnButton_4wIoCheck(); 	//son-32KPIN-20190902: OnButton105 ->OnButton_4wIoCheck 로 변경
	afx_msg void OnButtonOpen();		//son-32KPIN-20190902: OnButton106 ->OnButtonOpen 으로 변경
	afx_msg void OnButtonShort(); 		//son-32KPIN-20190902: OnButton107 ->OnButtonShort 로 변경
	afx_msg void OnButton_2wLeak();     //sylee240509	 
	afx_msg void OnButtonStart();		//son-32KPIN-20190902: OnButton201 ->OnButtonStart 로 변경
 	afx_msg void OnButtonStop();		//son-32KPIN-20190902: OnButton202 ->OnButtonStop 으로 변경
 	afx_msg void OnButtonMessageClear();	//son-32KPIN-20190902: OnButton302 ->OnButtonMessageClear 로 변경

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

// Attributes
public:

 

 	int 	m_nProgSystem_Rate;		//son-32KPIN-20190902: nProgress1Rate -> m_nProgSystem_Rate
 	int 	m_nProgVB_Rate;			//son-32KPIN-20190902: nProgress2Rate -> m_nProgVB_Rate
 	int 	m_nProgAD_Rate;			//son-32KPIN-20190902: nProgress3Rate -> m_nProgAD_Rate
 	int 	m_nProg2wIO_Rate;		//son-32KPIN-20190902: nProgress4Rate -> m_nProg2wIO_Rate
 	int 	m_nProg4wIO_Rate;		//son-32KPIN-20190902: nProgress5Rate -> m_nProg4wIO_Rate
	int 	m_nProg2wLeak_Rate;		//sylee240509
    int 	m_SelfAutoFlag1;		//son-32KPIN-20190902: SelfAutoFlag1 -> m_SelfAutoFlag1 
	int 	m_nCardNoStart; 		//son-32KPIN-20190902: nCardNoStart  -> m_nCardNoStart 
	int 	m_nCardNoEnd;			//son-32KPIN-20190902: nCardNoEnd    -> m_nCardNoEnd
	double 	m_dISOSet1;				//son-32KPIN-20190902: nISOSet1      -> m_dISOSet1
 	int 	m_nCC;					//son-32KPIN-20190902: nCC 		-> m_nCC
 	int 	m_nVRel;				//son-32KPIN-20190902: nVRel 		-> m_nVRel 
 	int 	m_nIR;					//son-32KPIN-20190902: nIR 		-> m_nIR
 	double 	m_dVSet; 				//son-32KPIN-20190902: nVSet 		-> m_dVSet
 	double 	m_dVSet1; 				//son-32KPIN-20190902: nVSet1 		-> m_dVSet1
 	double 	m_dVSet2; 				//son-32KPIN-20190902: nVSet2 		-> m_dVSet2
 	double 	m_dISet; 				//son-32KPIN-20190902:	nISet 		-> m_dISet
 	int		m_nFil;					//son-32KPIN-20190902:	nFil		-> m_nFil
    int 	m_nIFilter;				//son-32KPIN-20190902: nIFilter 	-> m_nIFilter
    int  	m_nHVOffSet;			//son-32KPIN-20190902: nHVOffSet 	-> m_nHVOffSet
    int		m_nIGain;				//son-32KPIN-20190902:	nIGain	  	-> m_nIGain
    int		m_nHVGain;				//son-32KPIN-20190902:	nHVGain	  	-> m_nHVGain
 
 

// Operations
public:
	void A_Language();//sylee151028

    //	int  OnCheckPowerHW1(int MesType); //power
	int  PowerCheckError1();//sylee141029
    //son231122	int  nHW_CardPin_Check();//sylee150903 //ACE400

	void OnAuto_Self1();
	void OnAuto_Self1_32K();							//son-32KPIN-20190902:  32K 전용
	int  Do_ProStep_2wSelfTest_32K(int nProStep1, 		// input	//son-32KPIN-20190902:  32K 전용
								int &rnStepErrCount);	// output
	//output
	int  AutoSelf_PowerModeSet(int m_nMode1);//  m_nMode1=1  LV SET  , 2= HV SET
    //	int  OnCheckAD_RRead1(); 
    int  OnCheckAD_RRead2(); //확인 필요

	int  OnCheckVB1(); 
	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);
	void OnATestRun4W(int  m_nMode1  ); //hv iso  // int Case=1 short, 2=OPEN	
	void OnATestRun4W_32K(int  m_nMode1); 			//son-32KPIN-20190902:  32K 전용
	int  Do_TestCase_4wSelfTest_32K(int testCase);	//son-32KPIN-20190902:  32K 전용

    void OnList1Mes1(CString str); //ad

    //int  nPinSW_Net(int SW);//sylee131117   sw->net //son231222_4 nPinSW_Net() 함수 삭제하고 nJigPinMap[hwPin0] + 1로  대체. 속도개선

	void OnAuto_Self1_Init();   ///sylee160520-1

     int OnManualShort1_LV();//SYLEE240509
	 int OnManualShort1_HV2();//SYLEE240509


 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW5_H__967DB85E_D544_45F4_9F16_B41B2D962CBA__INCLUDED_)
