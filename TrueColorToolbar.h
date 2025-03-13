#if !defined(AFX_TRUECOLORTOOLBAR_H__A3A9BEA9_1EFB_4DCC_B98F_59D43DDE98D5__INCLUDED_)
#define AFX_TRUECOLORTOOLBAR_H__A3A9BEA9_1EFB_4DCC_B98F_59D43DDE98D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrueColorToolbar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolbar window

class CTrueColorToolbar : public CToolBar
{
// Construction
public:
	CTrueColorToolbar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrueColorToolbar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrueColorToolbar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTrueColorToolbar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRUECOLORTOOLBAR_H__A3A9BEA9_1EFB_4DCC_B98F_59D43DDE98D5__INCLUDED_)
