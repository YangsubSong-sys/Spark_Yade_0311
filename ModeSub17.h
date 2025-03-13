#if !defined(AFX_MODESUB17_H__7D4F9785_9D78_4B6C_AD94_F8B0319D9DF0__INCLUDED_)
#define AFX_MODESUB17_H__7D4F9785_9D78_4B6C_AD94_F8B0319D9DF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub17.h : header file
//
#include "resource.h"
#include "msflexgrid.h"
#include "A_Define1.h"//sylee180720

/////////////////////////////////////////////////////////////////////////////
// CModeSub17 dialog


int Check_4WErrorNet();  //sylee171122  //son231019_2 OnErrorNet1() -> Check_4WErrorNet()
int __Check_4WErrorNet(bool bPrintOn, FILE *fp);  //son231020

class CModeSub17 : public CDialog
{
// Construction
public:
	CModeSub17(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeSub17)
#ifdef __GTS_4W_SPCIALNET__	
	enum { IDD = IDD_MODE_SUB17_4WSET2 };   //son2211107 GTS Special Net 항목 추가 UI
	CButton	m_check_AllSpecial;             //son221111_2 모든 Net 특수넷 표시 
#else
	enum { IDD = IDD_MODE_SUB17_4WSET1 };
#endif
	CButton	m_checkLowerCalJig;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub17)
	protected:

	virtual void DoDataExchange(CDataExchange* pDX);     
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	 
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub17)
		afx_msg void OnSelchangeCombo1();
		afx_msg void OnSelchangeCombo2();
		afx_msg void OnSelchangeCombo3();
		afx_msg void OnClickOk(); 
		afx_msg void OnButton_Save();
		afx_msg void OnButton_Load();
		afx_msg void OnDblClickMsflexgrid1();
		afx_msg void OnEnterCellMsflexgrid1();
		afx_msg void OnLeaveCellMsflexgrid1();
		afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnSelchangeComboDut();     //son200708: 4W Setup 화면에서 DUT 선택기능추가
    	afx_msg void OnSetfocusEdit7();         //son200724: 4W-SamplingCount 비번입력기능
        afx_msg void OnCheck_SigmaCoef();        //son211015 
        afx_msg void OnCheck_EjectLowHigh();     //son211015
        afx_msg void OnCheck_AvgPercentUse();   //son211015
    	afx_msg void OnCHECKLowerCalJig();
#ifdef __GTS_4W_SPCIALNET__	
    	afx_msg void OnCheck_AllSpecial();      //son221111_2 모든 Net 특수넷 표시
#endif
	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

public:
    //son for Label Button----------
    void  OnButtonTestManual();
    void  OnButtonTest1_1();// one range
    void  OnButtonTest2_3();// auto range //ACE380 //SYLEE150127   I RANGE FIX

    void  OnButtonTestInit();//jig up down  manual  //son Manual Test 초기화
 	void  OnButtonTestCancel();//jig up down  auto  //son Manual Test 1회 취소
    void  OnButtonTestSave();//file save            //son Manual Test 결과 Save-> BDLPah\4W_Setup.txt

	void  OnButton_Copy_TestDatato_RefAvg();
	void  OnButton_Load_Auto4WData();
	void  OnButton_ViewFile_4wRef();
	void  OnButton105();
	void  OnButton108();
	void  OnButton109();
 	void  OnButton_Open4wLimitSubDlg();     //son220308 OnButton110 -> OnButton_Open4wLimitSubDlg
	void  OnButton_ViewFile_Auto4wData();
	void  OnButton_SetLimitHigh();          //son220308 OnButton117 -> OnButton_SetLimitHigh
	void  OnButton_SetLimitLow();           //son220308 OnButton118 -> OnButton_SetLimitLow
	void  OnButton120();   //sylee221017-1
	void  OnButton121();

public:
    CMSFlexGrid	 m_grid1;
    CRect   m_rectGrid;     //son Grid의 영역에 해당하는 CRect

	CEdit	m_edit1;
	CEdit	m_edit2;
 	CEdit	m_edit3;                    //son 'Eject Low, high' Low  edit
	CEdit	m_edit4;//SYLEE181010-1     //son 'Eject Low, high' High edit
	CEdit	m_edit5;
	CEdit	m_edit6;                    //son Average % value
	CEdit	m_edit7;
	CEdit	m_edit8;                    //son Sigma Coefficient value

	CEdit	m_edit21;//SYLEE150805
	CEdit	m_edit22;//SYLEE150805
	CEdit	m_edit23;//SYLEE150805

	CButton	m_check1;//sylee141209      //son 'Sigma Coefficient'
	CButton	m_check2;//sylee150713      //son 'Eject Low, high'
	CButton	m_check3;//sylee150805
	CButton	m_check4;//sylee150805
	CButton	m_check5;//sylee150812-1    //son 'Auto Sampling  Enable'
	CButton	m_check6;                   //son 'Average %  Use'
	CButton	m_check7;

//	CComboBox	m_combo1;
//	CComboBox	m_combo2;
//	CComboBox	m_combo3;

	CLabelControl	m_Label101;
	CLabelControl	m_Label102;
	CLabelControl	m_LabelPoint;

	
	CLabelControl	m_Label1001;
 
	CLabelControl	m_Label1004;
 
	CLabelControl	m_Label1006;
	CLabelControl	m_Label1007;
	//CLabelControl	m_LabelDut;     //son200708: LabelDut 대신에 ComboDut를 추가
	CComboBox	    m_comboDut;    //son200708: 4W Setup 화면에서 DUT 선택기능추가
 
 
	CCommandButton m_button1001;
 
	CCommandButton m_button1004;
 	CCommandButton m_button1005;
	CCommandButton m_button1006;
 
	CCommandButton m_button1007;
 
	CCommandButton m_button1008;
 
	CCommandButton m_button1010;
 	CCommandButton m_button1011;
 
	CCommandButton m_button1013;
 	CCommandButton m_button1014;
 
	CCommandButton m_button1015;
 
public:
	double  nData[20][100];
    int     nStop,nCount , k,j,i;
    int     m_nCurCol, m_nCurRow, m_nMode1, m_nMode2, m_nMode3;
    int     nTotalSampCount1,nTotalNet1;//sylee160521	    //son210623 nTotalCount1 -> nTotalSampCount1 
                                        //son "4W_Setup_A.txt"에서 읽은 Sample 개수와 Net 개수
    double  m_dRatio;
    BOOL    OnEditing;
	char    szText[100];

    //son220417_9 ChildView7과 공유하기 위해 g_buseLowerCalJig_4W로 변경
	//bool    m_bUseLowerCalJig;     //son220419_7  0: 기존 Pin 사용. 1: 하부핀 R 측정 모드

public:
//	void    OnInit();   //Initclear         //son211202_4 OnInit() 함수 삭제.
	int     Init_grid(int dutIdx);          //son210524 해당 duxIdx에 맞게 grid, netCount 설정 
//  void    Load_Auto4WData(int dutIdx);    //son210524 해당 dutIdx의 4W_Setup_A.txt 로딩, sigma 계산 //son211202_3 Load_Auto4WData() 함수삭제
	int     _Load_Auto4WData(int dutIdx);   //son210623 해당 dutIdx의 4W_Setup_A.txt 로딩 원본, //son211202_3 sigma 계산추가

	void    A_Language();//sylee151028
	void    OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void    SetAttr(int iOpa); 
    void    InitControl();
	void    BeginEditing();
	void    EndEditing();

    int     ARun201_4w_R1(double nISet);//sylee150710
    //son220419_3 int     ARun201_4w_R_Cal1(int m_nMode1, int part);  //sylee150710   //son220412 part 추가

    bool    LoadSave_SamplingSigmaInput(int type); //son200724: 4W-SamplingCount 비번입력기능
    void    Process_SigmaCoefSampling(int nLimitType, int nEjectLowCnt, int nEjectHighCnt);  //son211005 
    void    Process_AvgPercentSampling(int nLimitType);   //son211005

#ifdef __GTS_4W_SPCIALNET__
    void    SetOn_NetSpecial(int net);  //son221107
    void    SetOff_NetSpecial(int net); //son221107
#endif
    void    AddRefLowHigh_SigmaLoadCsv();   //son230724
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB17_H__7D4F9785_9D78_4B6C_AD94_F8B0319D9DF0__INCLUDED_)
