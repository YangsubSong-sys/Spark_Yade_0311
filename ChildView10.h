#if !defined(AFX_CHILDVIEW10_H__E7E089F2_FE03_4B6C_B32C_D77B78FFB575__INCLUDED_)
#define AFX_CHILDVIEW10_H__E7E089F2_FE03_4B6C_B32C_D77B78FFB575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildView10.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildView10 form view
extern int g_nLogOnUser1;

extern nAutoProcess1;



#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CChildView10 : public CFormView
{
protected:
	CChildView10();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView10)

// Form Data
public:
	//{{AFX_DATA(CChildView10)
	enum { IDD = IDD_Form_View10 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView10)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildView10();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CChildView10)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW10_H__E7E089F2_FE03_4B6C_B32C_D77B78FFB575__INCLUDED_)
