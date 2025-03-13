#if !defined(AFX_CHILDVIEW2_H__FF6C556D_A8D3_4717_A2C0_EEA3B1ABD432__INCLUDED_)
#define AFX_CHILDVIEW2_H__FF6C556D_A8D3_4717_A2C0_EEA3B1ABD432__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView2 form view
extern int g_nLogOnUser1;

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#include "ModeSub11.h"
//#include "ModeSub12.h"
#include "ModeSub13.h"
#include "ModeSub15.h"
#include "ModeSub17.h"
#include "ModeSub19.h"
#include "ModeSub20.h"
#include "ModeSub25.h"


#include "resource.h"
#include "commandbutton.h"

class CChildView2 : public CFormView
{
protected:
	DECLARE_DYNCREATE(CChildView2)

// Form Data
public:
	CChildView2();



	CModeSub11 m_CModeSub11;
//	CModeSub12 m_CModeSub12;
	CModeSub13 m_CModeSub13;
	CModeSub15 m_CModeSub15;
	CModeSub17 m_CModeSub17;
	CModeSub19 m_CModeSub19;
	CModeSub20 m_CModeSub20;
	CModeSub25 m_CModeSub25;

	CCommandButton  m_button1003;//sylee160104
    CCommandButton  m_button1004;//sylee160104
    CCommandButton  m_button1010;//sylee160104
    CCommandButton  m_button1001;//sylee160104
    CCommandButton  m_button1007;//sylee160104
    CCommandButton  m_button1002;//sylee160104
    CCommandButton  m_button1011;//sylee160104
    CCommandButton  m_button1008;//sylee160104

 	void A_Language();//sylee151028

	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

	//{{AFX_DATA(CChildView2)
	enum { IDD = IDD_Form_View2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


	// Generated message map functions
	//{{AFX_MSG(CChildView2)

//	 afx_msg void  OnFilePrintSetup1(); //sylee
	afx_msg void OnClickButton1001();
	afx_msg void OnClickButton1002();
	afx_msg void OnClickButton1003();
	afx_msg void OnClickButton1004();
//	afx_msg void OnClickButton1005();
//	afx_msg void OnClickButton1006();
	afx_msg void OnClickButton1007();
	afx_msg void OnClickButton1008();
//	afx_msg void OnClickButton1009();
	afx_msg void OnClickButton1010();
	afx_msg void OnClickButton1011();	
	//}}AFX_MSG
	
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW2_H__FF6C556D_A8D3_4717_A2C0_EEA3B1ABD432__INCLUDED_)
