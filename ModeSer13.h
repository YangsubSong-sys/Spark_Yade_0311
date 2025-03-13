#if !defined(AFX_MODESER13_H__F0EF8800_0DD0_4611_90CD_AC40C93ADD82__INCLUDED_)
#define AFX_MODESER13_H__F0EF8800_0DD0_4611_90CD_AC40C93ADD82__INCLUDED_
 
#include "resource.h"
#include "msflexgrid.h"
#include "A_Define1.h"//sylee180720



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSer13.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSer13 dialog


#define	MAX_RJIG_ROW	32          //son220103
#define	MAX_RJIG_COL	18          //son220103


//son230904_2 begin  : moved from A_Define.h
//sylee221005 begin:   moved from ModeSer13.cpp to A_Define.h
#define  MAX_OPEN_R_COUNT       22  //son220415 200kohm까지 측정되도록 개수를 22로 설정. requested by RAY

#if (defined(__LEAK_300M_)) //sylee230823
   #define  MAX_SHORT_R_COUNT   45 //sylee230811  38->45     300M-> 5G R CALIBRATION   
#else //sylee230823
   #define  MAX_SHORT_R_COUNT   38 //SYLEE221005 37->38 Leak 200Mohm 정상측정을 위해 Short 300Mohm 항목추가
#endif

//#ifdef __MST__CUSTOMER
#ifdef __4W_200ohm__  //son230904_2 A_Define1.h의 복잡도를 덜기 위해  __4W_200ohm__ define을 추가하고   
                    //          MAX_4W_R_COUNT 는 ModeSer13.h로 이동.
    #define  MAX_4W_R_COUNT     27  //son220503_2 4W 200 ohm 정상측정을 위해 300 ohm 항목 추가
                                    //       4W cal Jig는 mohm 단위,   OPEN, SHORT cal Jig는 ohm 단위. 
#else
    #define  MAX_4W_R_COUNT     26  //son220607 신규 RJigReadType.txt 미반영시 에러 있을 수 있어서 MST에만 적용.
#endif

//sylee221005 end
//son230904_2 end

class CModeSer13 : public CDialog
{
// Construction
public:
	
	CModeSer13(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CModeSer13)
	enum { IDD = IDD_MODE_SER13_CAL }; 
	CButton	m_checkLowerPin;    //son220328 하부Pin Cal 측정기능
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSer13)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSer13)
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnClickOk(); 
	afx_msg void OnSave();
	afx_msg void Onload();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnEnterCellMsflexgrid1();
	afx_msg void OnLeaveCellMsflexgrid1();
	afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCHECKLowerPin();
	afx_msg void OnClickBUTTONCalConvertTest();
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()


public:
	
	//double RJigData1[35][20];//sylee150316
	double RJigData1[MAX_RJIG_ROW +3][MAX_RJIG_COL +2]; //son220103
                            //  // 4W R No 1~ No 32
                            //  RJigData1[RList1][1]; 	// 4W Pin No
                            //  RJigData1[RList1][2]; 	// 4W VB TR Pin
                            //  RJigData1[RList1][3]; 	// 4W VB RY Pin
                            //  RJigData1[RList1][4]; 	// 4W VC TR Pin
                            //  RJigData1[RList1][5]; 	// 4W VC RY Pin
                            //  RJigData1[RList1][6]; 	// 4W Orginal R
                            //  
                            //  // 2W R No 1~ No 32 : 
                            //  RJigData1[RList1][7]; 	// 2W Pin No  
                            //  RJigData1[RList1][8]; 	// 2W VB TR Pin
                            //  RJigData1[RList1][9]; 	// 2W VC TR Pin    
                            //  RJigData1[RList1][10]; 	// 4W Orginal R	
                            //  
                            //  // 2W R No 33~ No 64  
                            //  RJigData1[RList1][11];  // 2W Pin No
                            //  RJigData1[RList1][12];  // 2W VB TR Pin
                            //  RJigData1[RList1][13];  // 2W VC TR Pin
                            //  RJigData1[RList1][14];  // 4W Orginal R	
                            //  
                            //  // 2W R No 65~ No 96  
                            //  RJigData1[RList1][15];  // 2W Pin No
                            //  RJigData1[RList1][16];  // 2W VB TR Pin
                            //  RJigData1[RList1][17];  // 2W VC TR Pin
                            //  RJigData1[RList1][18];  // 4W Orginal R

    double nData[20][100];
	
	int nStop, nCount,k,j,i;
	int m_nCurCol,m_nCurRow ,m_nMode1;
	int m_nMode2;   //son 1: Cal Test mode(ADC rawR)  2: Real Run mode (ohm realR)

	bool m_bUseLowerCalJig;  //son220331 0: 기존 Pin 사용. 1: 하부핀 R 측정 모드 
 

	double m_dRatio;
	BOOL OnEditing;
	CRect m_rectGrid;

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void  On_RJigFileRead_ReadType(); //sylee150316  //ACE380 개발 

	void SetAttr(int iOpa); 
    void InitControl();
	void BeginEditing();
	void EndEditing();

	void  OnA_Cal_1();//SYLEE20120609
 
	void  OnA_Cal_JIG_Read_151211(); //ACE400  기존 것 R 일부 뺀 것 151211받음 
	
    void  CalConvertTest(int m_nMode1, int nConMode, double dRefReal, double dShouldBeVal); //son230919
 
 
	void  OnButton102();
	void  OnButton103();
	void  OnButton104();
	void  OnButton105();
	void  OnButton107();
	void  OnButton108();

	void  OnButton109();

	void  OnButton129();
	void  OnButton130();
	void  OnButton135();
	void  OnButton136();

	void  OnButton125();//sylee130618
    void  OnButton126();//sylee130618
    void  OnButton127();//sylee130618

 
	void  OnInit();   //Initclear


	CMSFlexGrid	 m_grid1;
	CEdit	m_edit1;
	CEdit	m_edit2;
 	CEdit	m_edit3;
	CEdit	m_edit5;

 	CEdit	m_editUseStart;
 	CEdit	m_editUseEnd;
 
	CComboBox	m_combo1;
	CComboBox	m_combo2;
 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESER13_H__D7053240-CE69-11CD-A777-00DD01143C57__INCLUDED_)
