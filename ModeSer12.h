#if !defined(AFX_MODESER12_H__089C4E43_DF45_48F8_A0BA_20F07C433A1D__INCLUDED_)
#define AFX_MODESER12_H__089C4E43_DF45_48F8_A0BA_20F07C433A1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSer12.h : header file
//

#include "msflexgrid.h"
#include "labelcontrol.h"
#include "file.h"


/////////////////////////////////////////////////////////////////////////////
// CModeSer12 dialog

class CModeSer12 : public CDialog
{
// Construction
public:

	CModeSer12(CWnd* pParent = NULL);   // standard constructor

	int m_nCurCol;
	int m_nCurRow;
	double m_dRatio;
	CRect m_rectGrid;
	CEdit	m_edit1;
	int	m_edit2;
	int	m_edit3;	
	BOOL OnEditing;

	CModeSub11(CWnd* pParent = NULL);   // standard constructor
	
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void InitControl();
    void SetAttr(int iOpa); 
    void BeginEditing();
	void EndEditing();

// Dialog Data
	//{{AFX_DATA(CModeSer12)
	enum { IDD = IDD_MODE_SER12 };
	
    CMSFlexGrid	 m_grid1;
	CSliderCtrl	 m_slider1;
	CSliderCtrl	 m_slider2;
	CLabelControl	m_Label1;
    CLabelControl	m_Label2;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSer12)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSer12)
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
//	afx_msg void OnOk(LPDISPATCH Cancel, LPDISPATCH Data, float X, float Y, long DragState, LPDISPATCH Effect, short Shift);

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESER12_H__089C4E43_DF45_48F8_A0BA_20F07C433A1D__INCLUDED_)
