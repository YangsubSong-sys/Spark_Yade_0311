#if !defined(AFX_CHILDVIEW4_H__568D0771_74EA_4FEB_89DB_C05D5DE0CE9F__INCLUDED_)
#define AFX_CHILDVIEW4_H__568D0771_74EA_4FEB_89DB_C05D5DE0CE9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView4 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "ModeView16.h"

extern int g_nLogOnUser1;


class CChildView4 : public CFormView
{
protected:
	          // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView4)

// Form Data
public:

	CChildView4(); 

	CLabelControl	m_Label1;

	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);
	void  OnButton1001(); //open
 	void  OnButton1002(); //open
	void  OnDataLoad(); 

	//{{AFX_DATA(CChildView4)
	enum { IDD = IDD_Form_View4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView4)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView4();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//afx_msg void OnButton1001();

/*	
    afx_msg void OnClickButton1001();
	afx_msg void OnClickButton1002();
	afx_msg void OnClickButton1003();
	afx_msg void OnClickButton1004();
 	afx_msg void OnClickButton1005();
 */
//	afx_msg void OnClickButton1006();


	// Generated message map functions
	//{{AFX_MSG(CChildView4)

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW4_H__568D0771_74EA_4FEB_89DB_C05D5DE0CE9F__INCLUDED_)
