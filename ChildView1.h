#if !defined(AFX_CHILDVIEW1_H__54A62506_CCFF_478F_97CE_483B7A56D83F__INCLUDED_)
#define AFX_CHILDVIEW1_H__54A62506_CCFF_478F_97CE_483B7A56D83F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView1 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"
#include "ModeRun11.h"
#include "labelcontrol.h"
#include "commandbutton.h"
#include "comidaq.h"        //son201028 BDL 자동로딩 기능 관련 추가
#include "ComiDaqComm.h"    //son201028

extern  CComiDaq	g_ComiDaq2; //son201028 ChildView7과 공유하기 위해 멤버변수를 글로별변수로 변경
#define VIEW1_TIMER_DIO    170	//son201028
#define COMI_SD414	0xB414      //son201028
#define COMI_SD424	0xB424      //son201028
 
extern int g_nLogOnUser1;


enum BDL_LOAD_TYPE {
    LOAD_BDL_FILE_DIALOG = 0,
    LOAD_BDL_CURR_PATH = 1
};

class CChildView1 : public CFormView
{

protected:

	DECLARE_DYNCREATE(CChildView1)

	CFormView* m_pCurMainDlg;

// Form Data
public:

	CChildView1();
		
	void A_Language();//sylee151028

	void ChangeView(CFormView* pDlg);
	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

    CEdit	m_edit101;//sylee210621
  
 	CLabelControl  m_Label101;

    CCommandButton  m_button3001;//sylee160104
    CCommandButton  m_button1001;//sylee160104
    CCommandButton  m_button_SaveAsNewBdl;     //son220228   IDC_BUTTON_NEWBDL

	CLabelControl	m_LabelPath;
	CListBox	m_list1;
	CListBox	m_list2;//SYLEE150817
	CListBox	m_list3;//SYLEE150817
	CListBox	m_list4;//SYLEE150817

	CLabelControl	m_Label2;
	CLabelControl	m_Label3;
	CLabelControl	m_Label4;
    CLabelControl	m_LabelDut;//SYLEE150817
    CLabelControl   m_LabelLotNameTitle;    //son220124
    CLabelControl   m_LabelLotName;         //son220124

	CModeRun11 m_CModeRun11;	

	void OnDataSaveAs(); 
	void OnDataSave(); 
	void  OnDataLoad_Type_A();   //SYLEE130907 JOYTECH    
	int  OnDataLoad_READ_ConRead(); // sylee150922-1   //sylee151111add
	void OnDataLoad(); //sylee130228joytech
	void OnDataLoadAuto(); //open button// sylee20130228
    void OnDataLoadAuto2(); //temp

    void Sort_nBDLData1_byNetPinCount(int nDut);                        //son200722-BDL-Sort
    void Sort_nBDLData3_byNetPinCount(int nDut);                        //son200722-BDL-Sort
    void Sort_nBDLData31_byNetPinCount(int nDut);                       //son200722-BDL-Sort
    void Sort_nBDLData31_byNetPinCount_piece(int nDut, int nPiece);     //son200722-BDL-Sort


//SYLEE180808 DUT16 DEL.	void OnDataSave2();  
//SYLEE180808 DUT16 DEL.    void OnDataLoad2(); 
//SYLEE180808 DUT16 DEL.	void OnDataSave3();  
//SYLEE180808 DUT16 DEL.	void OnDataLoad3(); 
//SYLEE180808 DUT16 DEL.	void OnDataSave4();  
//SYLEE180808 DUT16 DEL.	void OnDataLoad4(); 
	void On_Net_Convert1();//test
	void makePieceOpenTestData(int nDut);  //son Piece별 OpenTest List 생성
	void makePieceShortTestData(int nDut); //son Piece별 ShortTest List 생성
	                                       //son221103_2 ReadPinPieceNo()에서 함수내용  분리

	int printPieceOpenTestData(int nDut); //son piece별 g_pvBDLData2Op[][][]를 파일 출력
	int printPieceShortTestData(int nDut);//son piece별 g_pvBDLData31[][][]를 파일 출력.

    int LeakMultPinList1(int nDut);    //sylee230526 //sylee230817
	int LeakMultPinList1_File(int nDut); //sylee230526  //sylee230817
	bool CheckBDLName_fromZDrive(char* pStrBdlName);	   
	                                    //son201028 추가: "Z:\\AutoLoad.txt" 파일을 확인하여 파일에 적혀 있는  BDL 모델명을 확인한다.
	bool Search_BDLFolder(CString ACEpath, CString strBdlName);  
	                                    //son201028 추가: C:\ACE350\BDL 폴더 내부를 탐색해서 원하는 BDL 폴더가 있는지 확인한다.
	void ChangeFilePathJigName(CString fullPath, CString DirName, CString fileName);    //son201028 추가
    //bool CheckEmptyFolder(CString dirPath, CString strBdlName, );   //son201028 추가    
    bool CheckEmptyBdlFolder(CString dirPath, CString &r_strNewBdlName);   //son211027 r_strNewBdlName output 변수로 변경. for 나노시스
    int  LoadBDLFile(int nBdlType);	                            //son201109 함수추가    //son221028 int return 추가
    int  GetBDLFileName_FileDialog(CString &rStrFile);          //son201109 함수추가
    int  Check_BDLNetCountError(int nDut);          //son210831 //son210929

	//{{AFX_DATA(CChildView1)

	enum { IDD = IDD_Form_View1 };

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView1)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView1();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Generated message map functions
	//{{AFX_MSG(CChildView1)
	afx_msg void OnButton103();//sylee150827
	afx_msg void OnButton1001();
	afx_msg void OnButton2001();//sylee130228
    afx_msg void OnButton1002();
    afx_msg void OnButton_SaveAsNewBdl();  //son220228
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton1003();
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton1005();
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton1006();
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton1007();
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton1008();
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton1011();
//SYLEE180808 DUT16 DEL.	afx_msg void OnButton8();
	afx_msg void OnButton111();
	afx_msg void OnButton113();
	afx_msg void OnButton112();
	afx_msg void OnButton114();
	afx_msg void OnButton3001();
	afx_msg void OnButton201();     //sylee210621
 
    //son201028 BDL 자동로딩 기능 관련 추가
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	//}}AFX_MSG
	
	
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};


extern	CChildView1  pChildView1; 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW1_H__54A62506_CCFF_478F_97CE_483B7A56D83F__INCLUDED_)
