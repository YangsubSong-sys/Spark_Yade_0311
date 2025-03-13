// MyRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "SimplePad1.h"
#include "MyRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRichEditCtrl

CMyRichEditCtrl::CMyRichEditCtrl()
{
}

CMyRichEditCtrl::~CMyRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CMyRichEditCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRichEditCtrl message handlers

static DWORD CALLBACK ReadFunction(
	DWORD dwCookie, 
    LPBYTE buf,
	LONG lBytesToRead,
	LONG *lpBytesRead)
{
	CFile *fp = (CFile *)dwCookie;
	LONG lBytes = fp->Read(buf,lBytesToRead);
	*lpBytesRead = lBytes;
	return 0;
}

void CMyRichEditCtrl::ReadFile(CString fileName, CString fileExt)
{
	EDITSTREAM EditStream;

	CWaitCursor waitCursor;
	CFile file(fileName, CFile::modeRead);
	EditStream.dwCookie = (DWORD)&file;
	EditStream.pfnCallback = ReadFunction;

	int iFormat = 
	fileExt.CompareNoCase(_T("rtf"))==0	? SF_RTF : SF_TEXT;

	StreamIn(iFormat,EditStream);
}

static DWORD CALLBACK WriteFunction(
	DWORD dwCookie, 
    LPBYTE buf,
	LONG lBytesToWrite,
	LONG *lpBytesWritten)
{
	CFile *fp = (CFile *)dwCookie;
	fp->Write(buf,lBytesToWrite);
	*lpBytesWritten = lBytesToWrite;
	return 0;
}

void CMyRichEditCtrl::WriteFile(CString fileName, CString fileExt)
{
	EDITSTREAM EditStream;

	CWaitCursor waitCursor;
	CFile file(fileName,CFile::modeWrite|CFile::modeCreate);
	EditStream.dwCookie = (DWORD)&file;
	EditStream.pfnCallback = WriteFunction;

	int iFormat = 
	fileExt.CompareNoCase(_T("rtf"))==0	? SF_RTF : SF_TEXT;

	StreamOut(iFormat,EditStream);
}
