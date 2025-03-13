#if !defined(AFX_CSaveAsNewBdl_H__8D636FC4_3DD7_4CDF_8BAB_4BF95222BF23__INCLUDED_)
#define AFX_CSaveAsNewBdl_H__8D636FC4_3DD7_4CDF_8BAB_4BF95222BF23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSaveAsNewBdl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveAsNewBdl dialog

class CSaveAsNewBdl : public CDialog
{
// Construction
public:
	CSaveAsNewBdl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveAsNewBdl)
	enum { IDD = IDD_DIALOG_NEW_BDL };
	CString	m_editNewBdlName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveAsNewBdl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveAsNewBdl)
	afx_msg void OnClickOk();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



public:
    CString m_strNewBdlName;        //son220228 MST  BDL SaveAs 기능 관련 추가
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSaveAsNewBdl_H__8D636FC4_3DD7_4CDF_8BAB_4BF95222BF23__INCLUDED_)
