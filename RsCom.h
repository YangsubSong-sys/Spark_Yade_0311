// RsCom.h: interface for the CRsCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSCOM_H__257C5AE6_8AD4_47B5_80B5_E31598DF9791__INCLUDED_)
#define AFX_RSCOM_H__257C5AE6_8AD4_47B5_80B5_E31598DF9791__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 
int nPortDef2;


//=1;

class CRsCom  
{
public:
	CRsCom();
	virtual ~CRsCom();



BOOL OpenRS( int nInt);
BOOL CloseRS( int nInt ) ;
BOOL ReadRS(int nPort);
BOOL WriteRS(int nPort, char* cStr);
BOOL RsOpen();  //open
BOOL RsModeR(); //MODE R
BOOL RsModeC() ; //MODE C 
double RsModeMeas() ; //measure?
 



};

#endif // !defined(AFX_RSCOM_H__257C5AE6_8AD4_47B5_80B5_E31598DF9791__INCLUDED_)




