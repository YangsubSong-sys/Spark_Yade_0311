#if !defined(AFX_COMILED_H__02AB01F5_6D66_477B_8BFC_5E5D79F3C365__INCLUDED_)
#define AFX_COMILED_H__02AB01F5_6D66_477B_8BFC_5E5D79F3C365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CCOMILed wrapper class

class CCOMILed : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCOMILed)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x209656a8, 0x857b, 0x4e7b, { 0x8e, 0xb5, 0xc5, 0x4a, 0xf2, 0xb5, 0x56, 0x8 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:
	short GetLedStyle();
	void SetLedStyle(short);
	unsigned long GetBkGndColor();
	void SetBkGndColor(unsigned long);
	COleFont GetTextFont();
	void SetTextFont(LPDISPATCH);
	unsigned long GetStateTextColor();
	void SetStateTextColor(unsigned long);
	CString GetStateText();
	void SetStateText(LPCTSTR);
	short GetStateSoundStyle();
	void SetStateSoundStyle(short);
	unsigned long GetStateColor();
	void SetStateColor(unsigned long);
	short GetState();
	void SetState(short);
	short GetStateSuppliedSoundID();
	void SetStateSuppliedSoundID(short);
	CString GetStateUserSoundID();
	void SetStateUserSoundID(LPCTSTR);
	unsigned long GetFrameColor();
	void SetFrameColor(unsigned long);
	CString GetUserBkGndImageID();
	void SetUserBkGndImageID(LPCTSTR);
	short GetEnableUserBkGnd();
	void SetEnableUserBkGnd(short);
	short GetTransparent();
	void SetTransparent(short);

// Operations
public:
	short GetNumOfState();
	void DeleteState(long nStateIndex);
	void AddState();
	BOOL EnableLed(BOOL bEnable);
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMILED_H__02AB01F5_6D66_477B_8BFC_5E5D79F3C365__INCLUDED_)
