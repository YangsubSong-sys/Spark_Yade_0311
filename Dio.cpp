// Dio.cpp: implementation of the CDio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Loader.h"
#include "Dio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDio::CDio()
{
	for (int i=0; i<5; i++)
		m_pBoard[i] = NULL;
}

CDio::~CDio()
{
	for (int i=0; i<5; i++)
		m_pBoard[i] = NULL;
}


BOOL CDio::IsOn(long nCh)
{
	ASSERT(nCh >= 1000 && nCh < 3000);

	long r;
	BOOL ret = FALSE;

	if (nCh >= 1000 && nCh < 2000) {
		r = nCh-1000;
		if (r >= 0 && r < 64) {
			m_pBoard[2]->DiGetOne(r) == 0 ? ret = FALSE : ret = TRUE;
		}
		else if(r >= 64 && r < 128) {
			r -= 64;
			m_pBoard[3]->DiGetOne(r) == 0 ? ret = FALSE : ret = TRUE;
		}
		else if(r >= 128 && r < 192) {
			r -= 128;
			m_pBoard[4]->DiGetOne(r) == 0 ? ret = FALSE : ret = TRUE;
		}
	}
	else if (nCh >= 2000 && nCh < 3000) {
		r = nCh-2000;
		if (r >= 0 && r < 64) {
			m_pBoard[0]->DoGetOne(r) == 0 ? ret = FALSE : ret = TRUE;
		}
		else if(r >= 64 && r < 128) {
			r -= 64;
			m_pBoard[1]->DoGetOne(r) == 0 ? ret = FALSE : ret = TRUE;
		}
	}

	return ret;
}

BOOL CDio::IsOff(long nCh)
{
	ASSERT(nCh >= 1000 && nCh < 3000);
	BOOL ret = FALSE;
	IsOn(nCh) ? ret = FALSE : ret = TRUE;
	return ret;
}

BOOL CDio::SetOn(long nCh)
{
	ASSERT(nCh >= 2000 && nCh < 3000);
	int r = nCh-2000;

	if (r >= 0 && r < 64) {
		m_pBoard[0]->DoPutOne(r, 1);
	}
	else if (r >= 64 && r < 128) {
		r -= 64;
		m_pBoard[1]->DoPutOne(r, 1);
	}
	return TRUE;
}

BOOL CDio::SetOff(long nCh)
{
	ASSERT(nCh >= 2000 && nCh < 3000);
	int r = nCh-2000;

	if (r >= 0 && r < 64) {
		m_pBoard[0]->DoPutOne(r, 0);
	}
	else if (r >= 64 && r < 128) {
		r -= 64;
		m_pBoard[1]->DoPutOne(r, 0);
	}
	return TRUE;
}

BOOL CDio::SetToggle(long nCh)
{
	ASSERT(nCh >= 2000 && nCh < 3000);

	IsOn(nCh) ? SetOff(nCh) : SetOn(nCh);
	return TRUE;
}



