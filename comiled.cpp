// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "comiled.h"

// Dispatch interfaces referenced by this interface
#include "font.h"

/////////////////////////////////////////////////////////////////////////////
// CCOMILed

IMPLEMENT_DYNCREATE(CCOMILed, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CCOMILed properties

short CCOMILed::GetLedStyle()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CCOMILed::SetLedStyle(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

unsigned long CCOMILed::GetBkGndColor()
{
	unsigned long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CCOMILed::SetBkGndColor(unsigned long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

COleFont CCOMILed::GetTextFont()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CCOMILed::SetTextFont(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

unsigned long CCOMILed::GetStateTextColor()
{
	unsigned long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void CCOMILed::SetStateTextColor(unsigned long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString CCOMILed::GetStateText()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void CCOMILed::SetStateText(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

short CCOMILed::GetStateSoundStyle()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CCOMILed::SetStateSoundStyle(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

unsigned long CCOMILed::GetStateColor()
{
	unsigned long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void CCOMILed::SetStateColor(unsigned long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

short CCOMILed::GetState()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void CCOMILed::SetState(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

short CCOMILed::GetStateSuppliedSoundID()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}

void CCOMILed::SetStateSuppliedSoundID(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}

CString CCOMILed::GetStateUserSoundID()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void CCOMILed::SetStateUserSoundID(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

unsigned long CCOMILed::GetFrameColor()
{
	unsigned long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void CCOMILed::SetFrameColor(unsigned long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

CString CCOMILed::GetUserBkGndImageID()
{
	CString result;
	GetProperty(0xc, VT_BSTR, (void*)&result);
	return result;
}

void CCOMILed::SetUserBkGndImageID(LPCTSTR propVal)
{
	SetProperty(0xc, VT_BSTR, propVal);
}

short CCOMILed::GetEnableUserBkGnd()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}

void CCOMILed::SetEnableUserBkGnd(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}

short CCOMILed::GetTransparent()
{
	short result;
	GetProperty(0xe, VT_I2, (void*)&result);
	return result;
}

void CCOMILed::SetTransparent(short propVal)
{
	SetProperty(0xe, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CCOMILed operations

short CCOMILed::GetNumOfState()
{
	short result;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CCOMILed::DeleteState(long nStateIndex)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nStateIndex);
}

void CCOMILed::AddState()
{
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CCOMILed::EnableLed(BOOL bEnable)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		bEnable);
	return result;
}

void CCOMILed::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
