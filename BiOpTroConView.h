// BiOpTroConView.h : interface of the CBiOpTroConView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BiOpTroConVIEW_H__EE125C1D_0E3D_4A86_86C4_437C301548B1__INCLUDED_)
#define AFX_BiOpTroConVIEW_H__EE125C1D_0E3D_4A86_86C4_437C301548B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BiOpTroConDoc.h"

class CBiOpTroConView : public CView
{
protected: // create from serialization only
	CBiOpTroConView();
	DECLARE_DYNCREATE(CBiOpTroConView)

// Attributes
public:
	CBiOpTroConDoc* GetDocument();

 using  CView::OnFilePrint;
 using  CView::OnFilePrintPreview ;

 void InitLoadFile();


// Operations
public:



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBiOpTroConView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBiOpTroConView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBiOpTroConView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BiOpTroConView.cpp
inline CBiOpTroConDoc* CBiOpTroConView::GetDocument()
   { return (CBiOpTroConDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BiOpTroConVIEW_H__EE125C1D_0E3D_4A86_86C4_437C301548B1__INCLUDED_)
