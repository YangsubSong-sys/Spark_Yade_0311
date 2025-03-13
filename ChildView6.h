#if !defined(AFX_CHILDVIEW6_H__730BA27A_D1E1_4F2C_80BB_4062DFAC45DF__INCLUDED_)
#define AFX_CHILDVIEW6_H__730BA27A_D1E1_4F2C_80BB_4062DFAC45DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView6.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CChildView6 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

extern int g_nLogOnUser1;

class CChildView6 : public CFormView
{
protected:
         // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView6)

// Form Data
public:
	CChildView6();  
	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

	//{{AFX_DATA(CChildView6)
		enum { IDD = IDD_Form_View6 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView6)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView6();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Generated message map functions
	//{{AFX_MSG(CChildView6)

	afx_msg void OnButton101();
	afx_msg void OnButton102();
	afx_msg void OnButton103();
	afx_msg void OnButton105();
	afx_msg void OnButton107();
	afx_msg void OnButton109();	  
	afx_msg void OnButton110();	 
    afx_msg void OnButton111();	  

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW6_H__730BA27A_D1E1_4F2C_80BB_4062DFAC45DF__INCLUDED_)
