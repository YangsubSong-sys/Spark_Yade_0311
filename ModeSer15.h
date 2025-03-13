 #if !defined(AFX_MODESER15_H__F0EF8800_0DD0_4611_90CD_AC40C93ADD82__INCLUDED_)
 #define AFX_MODESER15_H__F0EF8800_0DD0_4611_90CD_AC40C93ADD82__INCLUDED_
 
#include "resource.h"
#include "msflexgrid.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum R_TestType {           //son211230
    R_TEST_SKIP     = 0,
    R_TEST_OPEN     = 1,
    R_TEST_uSHORT   = 2,
    R_TEST_HR_SHORT = 3,
    R_TEST_LEAK     = 4,
    R_TEST_4W       = 5,
    R_TEST_NUM  = 6
};

enum R_RunMode {    //son211230 used for m_nRunMode1
    R_AUTO = 1,
    R_MANUAL = 2
};


//son211230
#define VMODE_LIST_NUM      52
enum E_VMODE {

#if (defined(__NANOSYS__CUSTOMER) || defined(__LM__CUSTOMER))	//son220915_2
//son211230 나노시스 Customer인 경우는 10V 단위의 Voltage List로 사용한다

    E_OPEN_10V  = 0,   
    E_OPEN_20V,  
//    E_OPEN_30V  = 2,      //son220107 삭제요청 by RAY
//    E_OPEN_40V  = 3,      //son220107 삭제요청 by RAY
    E_OPEN_50V,
    E_OPEN_60V,
    E_OPEN_70V,

    E_OPEN_80V  = 5,
    E_OPEN_90V,
    E_OPEN_100V,
    E_OPEN_110V,
    E_OPEN_120V,

    E_OPEN_130V = 10,
    E_OPEN_140V,
    E_OPEN_150V,
    E_OPEN_160V,
    E_OPEN_170V,

    E_OPEN_180V = 15,
    E_OPEN_190V,
    E_OPEN_200V,
    E_OPEN_210V,
    E_OPEN_220V,

    E_OPEN_230V = 20,
    E_OPEN_240V,
    E_OPEN_250V,
    E_USHORT_1V,
    E_HR_10V,

    E_LEAK_20V  = 25,
//    E_LEAK_30V  = 28,     //son220107 삭제요청 by RAY
//    E_LEAK_40V  = 29,     //son220107 삭제요청 by RAY
    E_LEAK_50V,
    E_LEAK_60V,
    E_LEAK_70V,
    E_LEAK_80V,

    E_LEAK_90V  = 30,
    E_LEAK_100V,
    E_LEAK_110V,
    E_LEAK_120V,
    E_LEAK_130V,

    E_LEAK_140V = 35,
    E_LEAK_150V,
    E_LEAK_160V,
    E_LEAK_170V,
    E_LEAK_180V,

    E_LEAK_190V = 40,
    E_LEAK_200V,
    E_LEAK_210V,
    E_LEAK_220V,
    E_LEAK_230V,

    E_LEAK_240V = 45,
    E_LEAK_250V,

    E_MODE_NUM = 47

#else
//son220110 나노시스 Customer가 아닌 경우는 원래의 50V 단위의 Voltage List로 사용한다.  by 사장님.
    E_OPEN_10V  = 0,   
    E_OPEN_50V,
    E_OPEN_100V,
    E_OPEN_150V,
    E_OPEN_200V,

    E_OPEN_250V = 5,
    E_USHORT_1V,
    E_HR_10V,
    E_LEAK_50V,
    E_LEAK_100V,

    E_LEAK_150V = 10,
    E_LEAK_200V,
    E_LEAK_250V,
    E_MODE_NUM = 13


#endif

};

typedef struct sV_MODE //son211230
{
    CString str;
    int     nMode;
} sV_MODE; 


class C_VModeList   //son211230
{
public:
    sV_MODE     saList[E_MODE_NUM];

public:
    C_VModeList()        // Constructor: 생성자
    {
//#ifdef __NANOSYS__CUSTOMER
#if (defined(__NANOSYS__CUSTOMER) || defined(__LM__CUSTOMER))	//son220915_2
//son211230 나노시스 Customer인 경우는 10V 단위의 Voltage List로 사용한다
        saList[E_OPEN_10V ].str  = " 1.Open 10 V" ;   saList[E_OPEN_10V ].nMode = MODE6_OPEN_10V;
        saList[E_OPEN_20V ].str  = " 2.Open 20 V" ;   saList[E_OPEN_20V ].nMode = MODE11_OPEN_20V;
//      saList[E_OPEN_30V ].str  = " 3.Open 30 V" ;   saList[E_OPEN_30V ].nMode = MODE16_OPEN_30V;  //son220107 삭제요청 by RAY
//      saList[E_OPEN_40V ].str  = " 4.Open 40 V" ;   saList[E_OPEN_40V ].nMode = MODE21_OPEN_40V;  //son220107 삭제요청 by RAY
        saList[E_OPEN_50V ].str  = " 3.Open 50 V" ;   saList[E_OPEN_50V ].nMode = MODE26_OPEN_50V;
        saList[E_OPEN_60V ].str  = " 4.Open 60 V" ;   saList[E_OPEN_60V ].nMode = MODE31_OPEN_60V;
        saList[E_OPEN_70V ].str  = " 5.Open 70 V" ;   saList[E_OPEN_70V ].nMode = MODE36_OPEN_70V;

        saList[E_OPEN_80V ].str  = " 6.Open 80 V" ;   saList[E_OPEN_80V ].nMode = MODE41_OPEN_80V;
        saList[E_OPEN_90V ].str  = " 7.Open 90 V" ;   saList[E_OPEN_90V ].nMode = MODE46_OPEN_90V;
        saList[E_OPEN_100V].str  = " 8.Open 100V" ;   saList[E_OPEN_100V].nMode = MODE51_OPEN_100V;
        saList[E_OPEN_110V].str  = " 9.Open 110V" ;   saList[E_OPEN_110V].nMode = MODE56_OPEN_110V;
        saList[E_OPEN_120V].str  = "10.Open 120V" ;   saList[E_OPEN_120V].nMode = MODE61_OPEN_120V;

        saList[E_OPEN_130V].str  = "11.Open 130V" ;   saList[E_OPEN_130V].nMode = MODE66_OPEN_130V;
        saList[E_OPEN_140V].str  = "12.Open 140V" ;   saList[E_OPEN_140V].nMode = MODE71_OPEN_140V;
        saList[E_OPEN_150V].str  = "13.Open 150V" ;   saList[E_OPEN_150V].nMode = MODE76_OPEN_150V;
        saList[E_OPEN_160V].str  = "14.Open 160V" ;   saList[E_OPEN_160V].nMode = MODE81_OPEN_160V;
        saList[E_OPEN_170V].str  = "15.Open 170V" ;   saList[E_OPEN_170V].nMode = MODE86_OPEN_170V;

        saList[E_OPEN_180V].str  = "16.Open 180V" ;   saList[E_OPEN_180V].nMode = MODE91_OPEN_180V;
        saList[E_OPEN_190V].str  = "17.Open 190V" ;   saList[E_OPEN_190V].nMode = MODE96_OPEN_190V;
        saList[E_OPEN_200V].str  = "18.Open 200V" ;   saList[E_OPEN_200V].nMode = MODE101_OPEN_200V;
        saList[E_OPEN_210V].str  = "19.Open 210V" ;   saList[E_OPEN_210V].nMode = MODE106_OPEN_210V;
        saList[E_OPEN_220V].str  = "20.Open 220V" ;   saList[E_OPEN_220V].nMode = MODE111_OPEN_220V;

        saList[E_OPEN_230V].str  = "21.Open 230V" ;   saList[E_OPEN_230V].nMode = MODE116_OPEN_230V;
        saList[E_OPEN_240V].str  = "22.Open 240V" ;   saList[E_OPEN_240V].nMode = MODE121_OPEN_240V;
        saList[E_OPEN_250V].str  = "23.Open 250 V";   saList[E_OPEN_250V].nMode = MODE126_OPEN_250V;
        saList[E_USHORT_1V].str  = "24.uShort 1V" ;   saList[E_USHORT_1V].nMode = MODE205_uSHORT_1V;
        saList[E_HR_10V   ].str  = "25.HR 10 V"   ;   saList[E_HR_10V   ].nMode = MODE207_HR_SHORT_10V;

        saList[E_LEAK_20V ].str  = "26.Leak 20V"  ;   saList[E_LEAK_20V ].nMode = MODE212_LEAK_20V; 
//      saList[E_LEAK_30V ].str  = "29.Leak 30V"  ;   saList[E_LEAK_30V ].nMode = MODE217_LEAK_30V; //son220107 삭제요청 by RAY
//      saList[E_LEAK_40V ].str  = "30.Leak 40V"  ;   saList[E_LEAK_40V ].nMode = MODE222_LEAK_40V; //son220107 삭제요청 by RAY
        saList[E_LEAK_50V ].str  = "27.Leak 50 V" ;   saList[E_LEAK_50V ].nMode = MODE227_LEAK_50V;
        saList[E_LEAK_60V ].str  = "28.Leak 60 V" ;   saList[E_LEAK_60V ].nMode = MODE232_LEAK_60V;
        saList[E_LEAK_70V ].str  = "29.Leak 70 V" ;   saList[E_LEAK_70V ].nMode = MODE237_LEAK_70V;
        saList[E_LEAK_80V ].str  = "30.Leak 80 V" ;   saList[E_LEAK_80V ].nMode = MODE242_LEAK_80V;

        saList[E_LEAK_90V ].str  = "31.Leak 90 V" ;   saList[E_LEAK_90V ].nMode = MODE247_LEAK_90V;
        saList[E_LEAK_100V].str  = "32.Leak 100 V";   saList[E_LEAK_100V].nMode = MODE252_LEAK_100V;
        saList[E_LEAK_110V].str  = "33.Leak 110 V";   saList[E_LEAK_110V].nMode = MODE257_LEAK_110V;
        saList[E_LEAK_120V].str  = "34.Leak 120 V";   saList[E_LEAK_120V].nMode = MODE262_LEAK_120V;
        saList[E_LEAK_130V].str  = "35.Leak 130 V";   saList[E_LEAK_130V].nMode = MODE267_LEAK_130V;

        saList[E_LEAK_140V].str  = "36.Leak 140 V";   saList[E_LEAK_140V].nMode = MODE272_LEAK_140V;
        saList[E_LEAK_150V].str  = "37.Leak 150 V";   saList[E_LEAK_150V].nMode = MODE277_LEAK_150V;
        saList[E_LEAK_160V].str  = "38.Leak 160 V";   saList[E_LEAK_160V].nMode = MODE282_LEAK_160V;
        saList[E_LEAK_170V].str  = "39.Leak 170 V";   saList[E_LEAK_170V].nMode = MODE287_LEAK_170V;
        saList[E_LEAK_180V].str  = "40.Leak 180 V";   saList[E_LEAK_180V].nMode = MODE292_LEAK_180V;

        saList[E_LEAK_190V].str  = "41.Leak 190 V";   saList[E_LEAK_190V].nMode = MODE297_LEAK_190V;
        saList[E_LEAK_200V].str  = "42.Leak 200 V";   saList[E_LEAK_200V].nMode = MODE302_LEAK_200V;
        saList[E_LEAK_210V].str  = "43.Leak 210 V";   saList[E_LEAK_210V].nMode = MODE307_LEAK_210V;
        saList[E_LEAK_220V].str  = "44.Leak 220 V";   saList[E_LEAK_220V].nMode = MODE312_LEAK_220V;
        saList[E_LEAK_230V].str  = "45.Leak 230 V";   saList[E_LEAK_230V].nMode = MODE317_LEAK_230V;

        saList[E_LEAK_240V].str  = "46.Leak 240 V";   saList[E_LEAK_240V].nMode = MODE322_LEAK_240V;
        saList[E_LEAK_250V].str  = "47.Leak 250 V";   saList[E_LEAK_250V].nMode = MODE327_LEAK_250V;

#else
//son220110 나노시스 Customer가 아닌 경우는 원래의 50V 단위의 Voltage List로 사용한다.  by 사장님.
        saList[E_OPEN_10V ].str  = " 1.Open 10 V" ;   saList[E_OPEN_10V ].nMode = MODE6_OPEN_10V;
        saList[E_OPEN_50V ].str  = " 2.Open 50 V" ;   saList[E_OPEN_50V ].nMode = MODE26_OPEN_50V;
        saList[E_OPEN_100V].str  = " 3.Open 100V" ;   saList[E_OPEN_100V].nMode = MODE51_OPEN_100V;
        saList[E_OPEN_150V].str  = " 4.Open 150V" ;   saList[E_OPEN_150V].nMode = MODE76_OPEN_150V;
        saList[E_OPEN_200V].str  = " 5.Open 200V" ;   saList[E_OPEN_200V].nMode = MODE101_OPEN_200V;

        saList[E_OPEN_250V].str  = " 6.Open 250 V";   saList[E_OPEN_250V].nMode = MODE126_OPEN_250V;
        saList[E_USHORT_1V].str  = " 7.uShort 1V" ;   saList[E_USHORT_1V].nMode = MODE205_uSHORT_1V;
        saList[E_HR_10V   ].str  = " 8.HR 10 V"   ;   saList[E_HR_10V   ].nMode = MODE207_HR_SHORT_10V;
        saList[E_LEAK_50V ].str  = " 9.Leak 50 V" ;   saList[E_LEAK_50V ].nMode = MODE227_LEAK_50V;
        saList[E_LEAK_100V].str  = "10.Leak 100 V";   saList[E_LEAK_100V].nMode = MODE252_LEAK_100V;

        saList[E_LEAK_150V].str  = "11.Leak 150 V";   saList[E_LEAK_150V].nMode = MODE277_LEAK_150V;
        saList[E_LEAK_200V].str  = "12.Leak 200 V";   saList[E_LEAK_200V].nMode = MODE302_LEAK_200V;
        saList[E_LEAK_250V].str  = "13.Leak 250 V";   saList[E_LEAK_250V].nMode = MODE327_LEAK_250V;

#endif
    }
    
    
    // "C:\\ACE400\\SETUP2\\SysSetPin.ini" file(LoadSaveSet12()) write시에  
    // LowerStartPin 값 그대로 file에 쓰기 위해 아래 함수 작성.
    // combo의 index인 E_LOWER_STARTPIN enum 값을 file에 쓴다면 아래 함수는 필요 없음.
    int     findItemNo(int n_VMode)
    {
        for (int i = 0; i< E_MODE_NUM; i++)
        {
            if (saList[i].nMode == n_VMode)
                return i;
        }

        // saList에 없는 n_VMode 값이 들어오면 에러처리를 위해 -1로 리턴
        return -1;      // defalut
    }

};      //son211230

enum R_COL_TYPE {           //son211230
    R_COL_PIN1          = 1,
    R_COL_PIN2          = 2,
    R_COL_REF_R         = 3,
    R_COL_AVG           = 4,
    R_COL_REF_AVG_RATE  = 5,
    R_COL_DIFF_RATE     = 6,
    R_COL_INFO_NUM      = R_COL_DIFF_RATE,  // 6

    R_COL_NO1           = 7,
    R_COL_NO10          = 16,
    R_COL_NUM = R_COL_NO10          // 16
};          //son211230

//son MAX_R_COL_MEAS는 Grid용. MAX_R_MEAS는 CSV 파일 출력용.
#define MAX_R_COL_MEAS   (R_COL_NO10 - R_COL_NO1 +1)        //10    //son211230

//son MAX_R_MEAS는 Auto Test로 10~300 이상 진행할때 사용.
//          edit2의 nMeasCnt가 10이상이어도 GRID는  MAX_R_COL_MEAS (10) 까지만 출력한다.
#define MAX_R_MEAS          300         //son211230
#define MAX_R_LIST          128         //son211230


/////////////////////////////////////////////////////////////////////////////
// CModeSer15 dialog

class CModeSer15 : public CDialog
{
// Construction
public:
	CModeSer15(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CModeSer15)
	enum { IDD = IDD_MODE_SER15_R };
	CButton	m_checkLowerPin;        //son220315 하부Pin R 측정 기능
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSer15)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSer15)
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2(); 
	afx_msg void OnClickOk(); 
	afx_msg void OnSave();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnEnterCellMsflexgrid1();
	afx_msg void OnLeaveCellMsflexgrid1();
	afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCHECKLowerPin();
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

public:	
	CMSFlexGrid	 m_grid1;

	CEdit	m_edit1;
	CEdit	m_edit2;
// 	CEdit	m_edit3;
//	CEdit	m_edit5; 
 
	CComboBox	m_combo1;
	CComboBox	m_combo2; 

	CLabelControl	m_Label103;
    C_VModeList m_VModeList;        //son211230

	//double      RJigData1[35][20];//sylee150316
    //double      nData[20][100];	
	int     m_nStop, m_nCount,k,j,i;
	int     m_nRunMode1;    //son 1: Auto Test,   2: Manual Test
	int     m_nStep1;
	int     m_nCurCol,m_nCurRow ;
	int     m_nMode1;
	int     m_nRMode;    //son 1: REAL R(ohm) , 2: RAW R(ADC)
	double  m_dRatio;
	BOOL    OnEditing;
	CRect   m_rectGrid;
	double  m_dVSet; //son211230 ChildView7 extern에서 CModeSer15 멤버 변수로 변경

    int     m_nListMax;     //son211230 전역변수 nListMax -> 멤버변수 m_nListMax
    int     m_nTestType;    //son211230 전역변수 nTMode1  -> 멤버변수 m_nTestType
    int     m_nMeasColMax;  //son211230 전역변수 nMaxCol  -> 멤버변수 m_nMeasColMax

    bool    m_bUseLowerCalJig;     //son220315  0: 기존 Pin 사용. 1: 하부핀 R 측정 모드


	CString str; 


	void    OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void    BeginEditing();
	void    EndEditing();
	void    SetAttr(int iOpa); 

	void    On_R_file_load();
	void    On_R_Measure1(); 
	void    On_R_Measure2(); 
	void    On_R_Measure1_3();    //sylee200916
	int     On_R_Measure1_3_Set_dRec(int nMode1,                                //son input    //son220105
	                                int& rFastHR, double &rdISet, int& rnFil,   //son output
	                                int& rn4Wire, double& rnT_0,                //son output
	                                __int64& rtStart1, __int64& rtStart2,       //son output
	                                double& rdVSet, int& rnVRel, int& rnCC,     //son output
	                                int& rnIR, int& rnTimeDelay1);              //son output
 
//son220107  CModeSer15::RCalCovert2 대신에  ChildView8의 전역 RCalCovert() 함수 사용하도록 수정. 
//           Calibration과 R이 동일한 Cal 함수를 사용해야 불일치로 인한 문제 예방이 가능하다.
//    double  RCalCovert2( int nMode1,  double &Rvalue, int ConMode );

//son221014    double  RCalCovert3( int nMode1,  double &Rvalue, int ConMode, int part);   //son220412 part 추가
 	void    OnButton103(); 

    int     getRTestType(int nMode1);                   //son211230
 	CString get_LogR_Name(int nTestType, int vSet);     //son211230
 	void    OnButton_ViewFile();                        //son211230
 	void    Onload_Grid(int nTestType);                 //son211230 Onload -> Onload_Grid
 	void    OnButton_Stop();                            //son220103
//	void    OnButton104(); 

	int     writeCsv_MeasFile( int nTMode1, int nRCount, int nCount,  int nVSet); //son220103 Log1 -> writeCsv_MeasFile
    bool    LoadSave_UseLowerPin(int type);         //son220315 하부핀 R 측정기능
    void    InitGrid_MeasData();        //son220412_4



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESER13_H__D7053240-CE69-11CD-A777-00DD01143C57__INCLUDED_)
