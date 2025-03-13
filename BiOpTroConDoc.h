// BiOpTroConDoc.h : interface of the CBiOpTroConDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BiOpTroConDOC_H__D7BD387A_E41C_4CE1_A66B_38DF92E8C99F__INCLUDED_)
#define AFX_BiOpTroConDOC_H__D7BD387A_E41C_4CE1_A66B_38DF92E8C99F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 


class CBiOpTroConDoc : public CDocument
{
protected: // create from serialization only
	CBiOpTroConDoc();
	DECLARE_DYNCREATE(CBiOpTroConDoc)

// Attributes
public:

  
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBiOpTroConDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBiOpTroConDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBiOpTroConDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BiOpTroConDOC_H__D7BD387A_E41C_4CE1_A66B_38DF92E8C99F__INCLUDED_)
