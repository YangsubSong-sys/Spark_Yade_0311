#if !defined(AFX_MODESUB11_H__D0316DF3_BB6D_42D2_AA2C_D280B4F8F7B5__INCLUDED_)
#define AFX_MODESUB11_H__D0316DF3_BB6D_42D2_AA2C_D280B4F8F7B5__INCLUDED_


#include "msflexgrid.h"
#include "labelcontrol.h"
#include "file.h"
//#include "fileSysInfo.h"

//extern	CSysInfo SysInfo;
//CSysInfo SysInfo;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub11.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSub11 dialog

class CModeSub11 : public CDialog
{
// Construction
public:

	
	void A_Language();//sylee151028

	
    CLabelControl  m_Label1001;
    CLabelControl  m_Label1002;
	CLabelControl  m_Label1003;
    CLabelControl  m_Label1004;
	CLabelControl  m_Label1005;
	CLabelControl  m_Label1006; 
    CLabelControl  m_Label1007;
	CLabelControl  m_Label1008;
    CLabelControl  m_Label1009;
	CLabelControl  m_Label1010;

	CCommandButton m_button1001;
	CCommandButton m_button1002;
	CCommandButton m_button1003;
	CCommandButton m_button1004;
	CCommandButton m_button1005;
 

	CLabelControl	m_Label5;//sylee150804-2

	CScrollBar	m_scrollbar1;
	CScrollBar	m_scrollbar2;
	CScrollBar	m_scrollbar3;


	int m_nCurCol;
	int m_nCurRow;
	double m_dRatio;
	CRect m_rectGrid;
	CEdit	m_edit1;
    int	m_edit2;
  	int	m_edit3;
	int	m_edit4;
	CEdit	m_edit21;
	CEdit	m_edit22;
	CEdit	m_edit23;
	CEdit	m_edit24;


	BOOL OnEditing;

	CModeSub11(CWnd* pParent = NULL);   // standard constructor

	void InitControl();
    void SetAttr(int iOpa); 
    void BeginEditing();
	void EndEditing();
 
// Dialog Data
	//{{AFX_DATA(CModeSub11)

	enum { IDD = IDD_MODE_SUB11_PIECERANGE };
	
	CMSFlexGrid	 m_grid1;
//	CSliderCtrl	 m_slider1;
//	CSliderCtrl	 m_slider2;
	CLabelControl	m_Label1;
    CLabelControl	m_Label2;
	CLabelControl	m_Label3;

    CLabelControl	m_Label8;//SYLEE150917-5
    CLabelControl	m_Label21;//SYLEE150917-5
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub11)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub11)
	afx_msg void OnClickOk();
	afx_msg void OnClickGrid1();
	afx_msg void OnDblClickGrid1();
	afx_msg	void OnChangeSlider1();
	afx_msg	void OnChangeSlider2();
	afx_msg void OnLabel2();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnEnterCellMsflexgrid1();
	afx_msg void OnLeaveCellMsflexgrid1();
	afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
	afx_msg void OnSave();
	afx_msg void Onload();
	afx_msg	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSet1();
	afx_msg void OnButton103();//sylee150917-6

//	afx_msg void OnOk(LPDISPATCH Cancel, LPDISPATCH Data, float X, float Y, long DragState, LPDISPATCH Effect, short Shift);

	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB11_H__D0316DF3_BB6D_42D2_AA2C_D280B4F8F7B5__INCLUDED_)
