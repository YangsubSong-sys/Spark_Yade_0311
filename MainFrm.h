// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D5946414_62E1_4A02_A440_99F69F57E4EA__INCLUDED_)
#define AFX_MAINFRM_H__D5946414_62E1_4A02_A440_99F69F57E4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrueColorToolbar.h"
#include "MyCtrlBar.h"

// hdpark220511 spark begin
const static int	MAX_SPARK_COUNT = 32;
const static int	MAX_TRIG_COUNT = 1024;

struct PIN_INFO
{
	int nSequence;			// begin, end
	int nSrcNet[MAX_SPARK_COUNT];
	int nDestNet[MAX_SPARK_COUNT];
	int nIndex;
	int nNetCount;			// measure net count
	int nTotalNet;			// total net count per a trigger
	int nTriggerCount;
	int nReturnedCount;
	int nFailCol;
	int	nHWSparkOn;
	int nADSparkOn;
	int nLine;
	double dVoltage[MAX_SPARK_COUNT];
	double dCurrent[MAX_SPARK_COUNT];
	double nFailL1[20];

};
// spark end

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	    CString	m_strTime;



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
    void getStr_BBTController_Type(CString &str);       //son220211
	void SwitchView(UINT ViewID);
	virtual ~CMainFrame();
	//afx_msg 
        void OnMenuTest6();
		void OnMenuTest7();
	//afx_msg 
		void OnMenuTest8();
		void OnMenuTest9();
		int OnAutoScreenInterlock();//sylee170824-1 

		// hdpark220511 spark begin
		PIN_INFO m_tInfo[MAX_TRIG_COUNT];
		PIN_INFO m_tInfoRcv[MAX_TRIG_COUNT];
		//hdpark220511 spark end

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CTrueColorToolbar   m_wndToolBar;
	CReBar				m_wndReBar;
	CAnimateCtrl		m_wndAnimate;
	CBitmap*			m_pToolBarBmp;
	CMyCtrlBar			m_CtrlBarLeft;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuTest1();
	afx_msg void OnMenuTest2();
	afx_msg void OnMenuTest3();
	afx_msg void OnMenuTest4();
 	afx_msg void OnMenuTest5();
//	afx_msg void OnMenuTest6();
//	afx_msg void OnMenuTest8();

	afx_msg void OnMenuP1();
	afx_msg void OnMenuP2();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);		
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D5946414_62E1_4A02_A440_99F69F57E4EA__INCLUDED_)
