#if !defined(AFX_CHILDVIEW9_H__AA91D43F_2FFE_42F3_9F12_1E970B32C89A__INCLUDED_)
#define AFX_CHILDVIEW9_H__AA91D43F_2FFE_42F3_9F12_1E970B32C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView9.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView9 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


//son200724: 4WSeupâ passwd üũ��� �߰� �����ؼ� �����Լ� �߰�
//           4WSeupâ���� �Ʒ� �Լ��� ����ϱ� ����. 
//           CChildView9::OnButton1001()���� passwd üũ����� �и���.
extern void  CheckPassWord_forLogOnUser(char* szText, int& r_nLogOnUser1, bool bMessage); 
extern int  g_nLogOnUser1;
extern int  g_nLogOnRecipe1Retest;
extern int  g_nLogOnRecipe1;

class CChildView9 : public CFormView
{
protected:
         // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView9)

// Form Data
public:
	CChildView9(); 

	CEdit  m_edit1;
	CCommandButton  m_button1001; 
	CCommandButton  m_button1002; 
	CCommandButton  m_button1003; 
	
	void A_Language();//sylee151028
	void DrawGradation(CDC* pDC, COLORREF rgbBegin, COLORREF rgbEnd);

	//{{AFX_DATA(CChildView9)
	     enum { IDD = IDD_Form_View9 };

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView9)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView9();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Generated message map functions
	//{{AFX_MSG(CChildView9)
		afx_msg void OnButton1001();
        afx_msg void OnButton1002();
	    afx_msg void OnButton1003();
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW9_H__AA91D43F_2FFE_42F3_9F12_1E970B32C89A__INCLUDED_)
