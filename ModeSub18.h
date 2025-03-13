#if !defined(AFX_MODESUB18_H__7D4F9785_9D78_4B6C_AD94_F8B0319D9DF0__INCLUDED_)
#define AFX_MODESUB18_H__7D4F9785_9D78_4B6C_AD94_F8B0319D9DF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub18.h : header file
//
#include "resource.h"
#include "msflexgrid.h"

/////////////////////////////////////////////////////////////////////////////
// CModeSub18 dialog

//sylee200828  100->200   //son230707 MAX_4W_R was moved to A_Define1.h
//son230707 #define     MAX_4W_R            200000        //son 200 ohm , 200000 mohm //son201109

#define     MAX_LIMIT4W_RNG     20      //son211012 Limit 4W Range 최대개수

enum LIMIT_4W_R_ITEM        //son201109
{
    LIMIT4W_R_START    = 1,   //son mOhm R Range Start : 이 한계치가 적용되는 범위 시작 
    LIMIT4W_R_END      = 2,   //son mOhm R Range End   : 이 한계치가 적용되는 범위 끝
    LIMIT4W_PERCENT    = 3,   //son Percent Offset     : 한계치를  % 값으로 지정
    LIMIT4W_OFFSET     = 4,   //son mOhm Offset        : 한계치를 Offset으로 지정
    LIMIT4W_SIGMA_COEF = 5,    //son Sigma Coefficient (시그마 계수) : 한계치를 시그마의 배수로 지정

    //son  LimitHigh = (avg[net] + (avg[net]/100.0 * dHighPer1)+ sigma[net]*SigmaCoef + HighOffset) * ModeSub18::All%/100.0
    //son  LimitLow  = avg - (avg * dLowPer1/100.0) + LowOffset 
    LIMIT4W_NUM = 6
};

enum SETUP_4W_0        //son210916
{
    SET4W_LIMIT_USE           = 0,  //son gd4W1_P2High[0][0] : ModeSub18::  -Low Use (m_check1) +High Use (m_check2)  저장 
    SET4W_SIGMA_COEF_USE      = 1,  //son gd4W1_P2High[0][1] : ModeSub17의 Sigma Coefficient Enable이면 1
    SET4W_EJECT_LOW_HIGH_USE  = 2,  //son gd4W1_P2High[0][2] : ModeSub17의 'EJECT COUNT  LOW  HIGH' val 값이 설정되면 1 
    SET4W_HIGH_ALL_PERCNT_USE = 3,  //son gd4W1_P2High[0][3] : ModeSub18:: All % Use   (m_check4) 저장
    SET4W_HIGH_ALL_PERCNT_VAL = 4,  //son gd4W1_P2High[0][4] : ModeSub18:: All % Value (m_edit4) 저장

    SET4W_NUM = 5
};

enum SETUP_4W_21    //son210916
{
    SET4W_AVG_PERCNT_VAL = 1,       //son gd4W1_P2High[21][1] : ModeSub17의 'Average %' edit value (m_edit6) 저장
    SET4W_AVG_PERCNT_USE = 2        //son gd4W1_P2High[21][2] : ModeSub17의 'Average % Use' On     (m_check6) 저장
};

//son "4W_Per_Low.ini" data from  ModeSub18.cpp
                        //son220308  gd4W1_P1 -> gd4W1_P1Low
extern double gd4W1_P1Low[MAX_LIMIT4W_RNG+10][LIMIT4W_NUM];//SYLEE160503          //son211012 


//son "4W_High_High.ini" data from  ModeSub18.cpp
                        //son220308  gd4W1_P2 -> gd4W1_P2High
extern double gd4W1_P2High[MAX_LIMIT4W_RNG+10][LIMIT4W_NUM];//SYLEE160503  //sylee170819-1  int->double     //son211012
 

extern int g_flag_LimitPasswdOk;    //son220914_2 GTS 요구사항. passwd 체크 기능 추가.

class CModeSub18 : public CDialog
{
// Construction
public:
	CModeSub18(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeSub18)
	enum { IDD = IDD_MODE_SUB18_4W_ADD1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	char cBuf[20];
	char szText[20];

	int nStop;
    int nCount;
 

	int k,j,i;
	double nData[20][100];

	int m_nCurCol;
	int m_nCurRow;
	int m_nCurCol2;
	int m_nCurRow2;

	int m_nMode1;
	int m_nMode2;
	int m_nMode3;

	double m_dRatio;	
	double m_dRatio2;

	BOOL OnEditing1,OnEditing2;
	CRect m_rectGrid1,m_rectGrid2;
	
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void SetAttr(int iOpa); 
    void InitControl();
	void BeginEditing1();
	void BeginEditing2();

	void EndEditing1(); 
	void EndEditing2();
 
  	void  OnButton101(); 
	void  OnButton102();
  
	void  OnInit_grid1_Low();   //Initclear
	void  OnInit_grid2_High();   //Initclear

	CMSFlexGrid	 m_grid1;
	CMSFlexGrid	 m_grid2;

	CEdit	m_edit1;
 	CEdit	m_edit2;
  //  CEdit	m_edit3;
    CEdit	m_edit4;
 //   CEdit	m_edit5;

	CButton	m_check1; 
	CButton	m_check2; 
//	CButton	m_check3; 
	CButton	m_check4; 
 	CButton	m_check5; 

	CButton m_button1;
	CButton m_button2;
 
	int m_nPerLow;//sylee160223-7
	int m_nPerHigh;//sylee160223-7
 
	void OnLoadDisplay1();
	void OnLoadDisplay2();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub18)
	protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

 
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub18)
		// NOTE: the ClassWizard will add member functions here
       
	    afx_msg void OnClickOk(); 
	 
//		afx_msg void OnSave();
//		afx_msg void Onload();
		afx_msg void OnDblClickMsflexgrid1();
		afx_msg void OnEnterCellMsflexgrid1();
		afx_msg void OnLeaveCellMsflexgrid1();
		afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);

		afx_msg void OnDblClickMsflexgrid2();
		afx_msg void OnEnterCellMsflexgrid2();
		afx_msg void OnLeaveCellMsflexgrid2();
		afx_msg void OnMouseUpMsflexgrid2(short Button, short Shift, long x, long y);


		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnCheck01();
		afx_msg void OnCheck02();
        afx_msg void OnCheck03();
		afx_msg void OnCheck04();


     //	afx_msg void OnClickOk();
	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB18_H__7D4F9785_9D78_4B6C_AD94_F8B0319D9DF0__INCLUDED_)
