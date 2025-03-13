#if !defined(AFX_CHILDVIEW3_H__1E5E585E_A7E7_4735_B626_439A0BA04F0E__INCLUDED_)
#define AFX_CHILDVIEW3_H__1E5E585E_A7E7_4735_B626_439A0BA04F0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView3 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "resource.h"

#include "ModeMan11.h"
#include "ModeMan12.h"
 
#include "ModeMan14.h"
#include "ModeMan15.h"
#include "ModeMan16.h"
#include "ModeMan17.h"

#include "commandbutton.h"

extern int g_nLogOnUser1;



class CChildView3 : public CFormView
{
protected:
          // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView3)

// Form Data
public:

		CChildView3(); 

		void A_Language();//sylee151028
         
		 CCommandButton    m_button1002;  
		 CCommandButton    m_button1001;  
		 CCommandButton    m_button1004;   
		 CCommandButton    m_button1006;  
		 CCommandButton    m_button1003;  
		 CCommandButton    m_button1005;  
		 CCommandButton    m_button1007;

		void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

	//{{AFX_DATA(CChildView3)
	enum { IDD = IDD_Form_View3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
	 
public:
	void DrawBk(CDC* pDC);

protected:
	virtual ~CChildView3();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CChildView3)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	afx_msg void OnClickButton1001();
	afx_msg void OnClickButton1002();
	afx_msg void OnClickButton1003();
	afx_msg void OnClickButton1004();
 	afx_msg void OnClickButton1005();
 	afx_msg void OnClickButton1006();
 	afx_msg void OnClickButton1007();
	afx_msg void OnButtonSpark();       //hdpark230223

	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW3_H__1E5E585E_A7E7_4735_B626_439A0BA04F0E__INCLUDED_)
