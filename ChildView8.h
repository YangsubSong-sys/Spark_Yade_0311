#if !defined(AFX_CHILDVIEW8_H__967DB85E_D544_45F4_9F16_B41B2D962CBA__INCLUDED_)
#define AFX_CHILDVIEW8_H__967DB85E_D544_45F4_9F16_B41B2D962CBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView8.h : header file
//


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "FileSysInfo.h"        //son-32KPIN-20200228 : 신규 추가        
#include "ModeSub71.h"          //son240608


/////////////////////////////////////////////////////////////////////////////
// CChildView8 form view
extern int g_nLogOnUser1;

extern int Get_nCardPinMax();    //son231227 nHW_CardPin_Check() -> Get_nCardPinMax() in ChildView8
extern int GetHwPinNo_Old(int card, int cardPin );   //son240125
extern char* getStrSelfTestCase(int nCase);         //son240201
extern char* getStrSelfTestCase2(int nProStep1);    //son240205

class CChildView8 : public CFormView
{
protected:
            // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView8)

// Form Data
public:
	CChildView8();


	CProgressCtrl	m_Progress4;
	CProgressCtrl	m_Progress3;
	CProgressCtrl	m_Progress2;
	CProgressCtrl	m_Progress1;
	CProgressCtrl	m_Progress5;
	CProgressCtrl	m_Progress6;

	CButton	m_check1;
	CButton	m_check2;
	CButton	m_check3;
	CButton	m_check4;
	CButton	m_check5;
	CButton	m_check6;
	CButton	m_check7;
	CButton	m_check8;
 
	CEdit	m_edit1;
	CEdit	m_edit2;
	CEdit	m_edit8;

	CEdit	m_edit11;
	CEdit	m_edit12;
	CEdit	m_edit13;
	CEdit	m_edit15;
 
    CEdit	m_edit59;

	CComboBox	m_combo1;
	CComboBox	m_combo2;

	CListBox	m_list1;

	
	CLabelControl	m_Label21;
	CLabelControl	m_Label22;
	CLabelControl	m_Label24;
 

	CLabelControl  m_Label1001;
	CLabelControl  m_Label1002;
	CLabelControl  m_button101;
	CLabelControl  m_button102;
	CCommandButton  m_button103;
	CCommandButton  m_button201;
	CLabelControl  m_Label28;
	CLabelControl  m_Label31;
	CLabelControl  m_Label35;
	CLabelControl  m_Label36;
	CLabelControl  m_Label44;
	CCommandButton  m_button302;
	CLabelControl   m_Label64;
	CCommandButton   m_button104;
	CCommandButton  m_button105;
	CCommandButton  m_button106;
	CCommandButton   m_button107;
	CCommandButton   m_button108;	

	CCommandButton   m_button207;
	CCommandButton   m_button202;
	CLabelControl    m_Label39;
	CCommandButton   m_button125;
	CCommandButton   m_button128;
 
	CCommandButton  m_button111;
	CCommandButton   m_button121;
	CCommandButton   m_button122;
	CCommandButton   m_button112;
	CLabelControl   m_Label40;
	CCommandButton  m_button124;
	CCommandButton   m_button123;
	CCommandButton  m_button701;
	CCommandButton  m_button702;
	CCommandButton  m_button703;

	void A_Language();//sylee151028

	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

	//{{AFX_DATA(CChildView8)
	enum { IDD = IDD_Form_View8_SELF };
        // NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView8)
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
	virtual ~CChildView8();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Generated message map functions
	//{{AFX_MSG(CChildView8)
	afx_msg void OnButton101_SystemPower();     //son230302
	afx_msg void OnButton102_SystemVB();        //son230302
	afx_msg void OnButton103_SystemAD();        //son230302
	afx_msg void OnButton104_SwCardLvOpen();    //son230302
	afx_msg void OnButton105_SwCardLvShort();   //son230302
	afx_msg void OnButton106_SwCardHvOpen();    //son230302
	afx_msg void OnButton107_SwCardHvShort();   //son230302
	afx_msg void OnButton108_SwCardRelay();     //son230302
 	afx_msg void OnButton109_SwCardRsCheck();   //son230302
	afx_msg void OnButton111_VBSet();           //son230302
 	afx_msg void OnButton112_Read();            //son230302
	afx_msg void OnButton121_SetVbPin();        //son230302
	afx_msg void OnButton122_SetVcPin();        //son230302
	afx_msg void OnButton123_ResetPin();        //son230302
	afx_msg void OnButton124_PinVBOff();        //son230302
 	afx_msg void OnButton125_VBPinInfo();       //son230302
 	afx_msg void OnButton128_VCPinInfo();       //son230302
	afx_msg void OnButton201_SystemAuto();      //son230302
	afx_msg void OnButton202_StopAutoSelf();    //son230302
 	afx_msg void OnButton302_ClearMessage();    //son230302
 	afx_msg void OnButton701_NetPinInfo();      //son230302
 	afx_msg void OnButton702_DDPinInfo();       //son230302
 	afx_msg void OnButton703_SWPinInfo();       //son230302
    afx_msg void OnAuto_Self1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
 	afx_msg void OnCheck9();
	afx_msg void OnCheck10();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButton2_Print_SrvSelfPinMap();    //son240123
	//}}AFX_MSG
	

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()


// Operations
public:


	int nProPer1;
	int nProPer2;
	int nProPer3;
	int nProPer4;


		 
    int nDischarge1 ;
 

	int	nVBStartPin  ;	 
	int	nVBEndPin  ;	 
	int	nVCStartPin  ; 
	int	nVCEndPin  ;


 	int nProgress1Rate;
 	int nProgress2Rate;
 	int nProgress3Rate;
 	int nProgress4Rate;
 	int nProgress5Rate;
 	int nProgress6Rate;

	int nTestCase1;


	

	int  OnCheckPowerHW1(int MesType); //power
//	int  OnCheckAD_RRead1(); 
	int  OnCheckVB1(); 


	int  m_nCardNoStart; 
	int  m_nCardNoEnd;
	
	double nISOSet1;        //son  Reference 

	void  OnATestRun(int  nVMode,  int nIso1,  int nAuto1); //hv iso      //son231122 m_nMode1 -> mVMode
 
	
//  void  OnATestRun2W(); //hv iso	//son240131 안쓰는 코드 코멘트처리
    void  OnATestRun4W(int  m_nMode1  ); //hv iso  // int Case=1 short, 2=OPEN	

    void  OnList1Mes1(CString str); //ad

    int  AutoSelf_PowerModeSet(int m_nMode1);//  m_nMode1=1  LV SET  , 2= HV SET

    //int  nPinSW_Net(int SW);//sylee131117   sw->net //son231222_4 nPinSW_Net() 함수 삭제하고 nJigPinMap[hwPin0] + 1로  대체. 속도개선
    //int  nHW_CardPin_Check();//sylee150903 //ACE400 //son231227 nHW_CardPin_Check() -> Get_nCardPinMax()

    void Self_AOnPinSet1(int nPin, PIN_SET_MOD nMode, int nCardPin); //son240608
    int  Self_OnPinAllReset1();     //son240608
    int  m_nCardPinVB;   //son240608
    int  m_nCardPinVC;   //son240608
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW8_H__967DB85E_D544_45F4_9F16_B41B2D962CBA__INCLUDED_)
