#if !defined(AFX_MODEVIEW16_H__0851F873_E5E3_4245_A38A_470D90D7B677__INCLUDED_)
#define AFX_MODEVIEW16_H__0851F873_E5E3_4245_A38A_470D90D7B677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeView16.h : header file
//





#include "MyRichEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CModeView16 dialog


class CModeView16 : public CDialog
{
// Construction
public:
	CModeView16(CWnd* pParent = NULL);   // standard constructor

	CMyRichEditCtrl m_RichEditCtrl;

	CBiOpTroConDoc* GetDocument();

//	 using   CWinApp::OnFilePrintSetup;
 //	using   CWinApp::OnFilePrintPreview;


// Dialog Data
	//{{AFX_DATA(CModeView16)
	enum { IDD = IDD_MODE_VIEW16 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeView16)
	public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CModeView16)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickOk();
	afx_msg void OnClickPrint();

	//}}AFX_MSG
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEVIEW16_H__0851F873_E5E3_4245_A38A_470D90D7B677__INCLUDED_)
