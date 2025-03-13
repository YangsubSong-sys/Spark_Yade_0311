#if !defined(AFX_MODESUB13_H__8F2588CB_83DF_4410_A89A_E333F3E3EBFD__INCLUDED_)
#define AFX_MODESUB13_H__8F2588CB_83DF_4410_A89A_E333F3E3EBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub13.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSub13 dialog

class CModeSub13 : public CDialog
{
// Construction
public:

	CModeSub13(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CModeSub13)
	enum { IDD = IDD_MODE_SUB13_BLOCK };

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub13)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub13)
		// NOTE: the ClassWizard will add member functions here
	
	afx_msg void OnClickOk();
	afx_msg void OnClickGrid1();
	afx_msg void OnDblClickGrid1();
	afx_msg	void OnChangeSlider1();
	afx_msg	void OnChangeSlider2();
	afx_msg	void OnChangeSlider3();
 
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnEnterCellMsflexgrid1();
	afx_msg void OnLeaveCellMsflexgrid1();
	afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
	afx_msg void OnSave();
	afx_msg void Onload();
    afx_msg void OnClickButton101();
 
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	
public:
	void A_Language();//sylee151028

	CLabelControl	m_Label5;//sylee150804-2


    CMSFlexGrid	 m_grid1;
	CSliderCtrl	 m_slider1;
	CSliderCtrl	 m_slider2;
	CSliderCtrl	 m_slider3;
	CLabelControl	m_Label1;
    CLabelControl	m_Label2;
	CLabelControl	m_Label3;
	CComboBox	m_combo1;
	CComboBox	m_combo2;
	CScrollBar	m_scrollbar1;
	CScrollBar	m_scrollbar2;
 

	CLabelControl	m_Label1001;
    CLabelControl	m_Label1002;
	CLabelControl	m_Label1003;
    CLabelControl	m_Label1004;
	CLabelControl	m_Label1005;
    CLabelControl	m_Label1006;
	CLabelControl	m_Label1007;
  
 	      
	CCommandButton m_button1001;
	CCommandButton m_button1002;
	CCommandButton m_button1003;
	CCommandButton m_button1004;

	int m_nCurCol;
	int m_nCurRow;
	double m_dRatio;
	CRect m_rectGrid;
	CEdit	m_edit1;
	int	m_edit2;
	int	m_edit3;	
	BOOL OnEditing;


	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void InitControl();
	void SetAttr(int iOpa); 
	void BeginEditing();
	void EndEditing();	

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB13_H__8F2588CB_83DF_4410_A89A_E333F3E3EBFD__INCLUDED_)
