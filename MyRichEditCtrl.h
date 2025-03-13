#if !defined(AFX_MYRICHEDITCTRL_H__A6B7F6A1_AFD3_11D2_B7E5_0020E00EF9DE__INCLUDED_)
#define AFX_MYRICHEDITCTRL_H__A6B7F6A1_AFD3_11D2_B7E5_0020E00EF9DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRichEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyRichEditCtrl window

class CMyRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CMyRichEditCtrl();

// Attributes
public:

// Operations
public:
	void ReadFile(CString fileName, CString fileExt);
	void WriteFile(CString fileName, CString fileExt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRichEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyRichEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyRichEditCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRICHEDITCTRL_H__A6B7F6A1_AFD3_11D2_B7E5_0020E00EF9DE__INCLUDED_)
