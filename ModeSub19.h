#if !defined(AFX_MOdeSub19_H__8F2588CB_83DF_4410_A89A_E333F3E3EBFD__INCLUDED_)
#define AFX_MOdeSub19_H__8F2588CB_83DF_4410_A89A_E333F3E3EBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSub19.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSub19 dialog

class CModeSub19 : public CDialog
{
// Construction
public:

	// Dialog Data
	//{{AFX_DATA(CModeSub19)
	enum { IDD = IDD_MODE_SUB19_PIECE_DIS };

	   CButton	m_check1;


		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	 CLabelControl	m_Label5;//sylee150804-2

	 
	void A_Language();//sylee151028

	CEdit	m_edit101;
	CEdit	m_edit102;
	CEdit	m_edit103;

    CMSFlexGrid	 m_grid1;
	CSliderCtrl	 m_slider1;
	CSliderCtrl	 m_slider2;
	CSliderCtrl	 m_slider3;
	CLabelControl	m_Label1;
    CLabelControl	m_Label2;
	CLabelControl	m_Label3;
//	CComboBox	m_combo1;

	CScrollBar	m_scrollbar1;
	CScrollBar	m_scrollbar2;
//	CScrollBar	m_scrollbar3;


	

    CLabelControl  m_Label1001;
    CLabelControl  m_Label1002;
	CLabelControl  m_Label1003;
    CLabelControl  m_Label1004;
	CLabelControl  m_Label1005;
	CLabelControl  m_Label1006; 
    CLabelControl  m_Label1007;
	CLabelControl  m_Label1008;
    CLabelControl  m_Label1009;
	

	CCommandButton m_button1001;
	CCommandButton m_button1002;
	CCommandButton m_button1003;
	CCommandButton m_button1004;
	CCommandButton m_button1005;


	int m_nCurCol;
	int m_nCurRow;
	double m_dRatio;
	CRect m_rectGrid;
	CEdit	m_edit1;
	int	m_edit2;
	int	m_edit3;	
	BOOL OnEditing;

	CModeSub19(CWnd* pParent = NULL);   // standard constructor

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	void OnPieceBlockSet1(); //SYLEE131122

	void InitControl();
	void SetAttr(int iOpa); 
	void BeginEditing();
	void EndEditing();	




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSub19)

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeSub19)
		// NOTE: the ClassWizard will add member functions here
	
	afx_msg void OnClickOk();
	afx_msg void OnClickGrid1();
	afx_msg void OnDblClickGrid1();
	afx_msg	void OnChangeSlider1();
	afx_msg	void OnChangeSlider2();
	afx_msg	void OnChangeSlider3();
//	afx_msg void OnLabel2();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnEnterCellMsflexgrid1();
	afx_msg void OnLeaveCellMsflexgrid1();
	afx_msg void OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y);
	afx_msg void OnSave();
	afx_msg void Onload();
	afx_msg void OnClickSet();
	afx_msg void OnClickSet2();
    afx_msg void OnClickButton101();


//	afx_msg void OnOk(LPDISPATCH Cancel, LPDISPATCH Data, float X, float Y, long DragState, LPDISPATCH Effect, short Shift);

//	afx_msg void OnSelchangeCombo1();
	afx_msg void OnCheck1();

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOdeSub19_H__8F2588CB_83DF_4410_A89A_E333F3E3EBFD__INCLUDED_)
