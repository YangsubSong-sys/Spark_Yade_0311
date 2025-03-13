#if !defined(AFX_DIO_H__DCD75600_87ED_45B7_9818_3EB63F03D6F2__INCLUDED_)
#define AFX_DIO_H__DCD75600_87ED_45B7_9818_3EB63F03D6F2__INCLUDED_

// Dio.h: interface for the CDio class.
//
//////////////////////////////////////////////////////////////////////
#include "comidaq.h"
//#include "comiled.h"
//#include "comitoggleswitch.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comidaqApi.h"

class CDio  : public CWnd
{
public:
	CComiDaq*	m_pBoard[5];
	BOOL SetToggle(long nCh);
	BOOL SetOff(long nCh);
	BOOL SetOn(long nCh);
	BOOL IsOff(long nCh);
	BOOL IsOn(long nCh);
	CDio();
	virtual ~CDio();

};

#endif // !defined(AFX_DIO_H__DCD75600_87ED_45B7_9818_3EB63F03D6F2__INCLUDED_)
