// BiOpTroConDoc.cpp : implementation of the CBiOpTroConDoc class
//

#include "stdafx.h"
#include "BiOpTroCon.h"

#include "BiOpTroConDoc.h"

#include "ModeSUB71.h"

 



/*
#include "comidaq.h"//SYLEE
#include "comiled.h"//SYLEE
#include "comitoggleswitch.h"//SYLEE
#include "ComiDaqComm.h" //SYLEE
#include "Dio.h" //SYLEE

#include "comiled.h"

  */


 

 



extern CModeSub71  Sub71;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConDoc

IMPLEMENT_DYNCREATE(CBiOpTroConDoc, CDocument)

BEGIN_MESSAGE_MAP(CBiOpTroConDoc, CDocument)
	//{{AFX_MSG_MAP(CBiOpTroConDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConDoc construction/destruction

CBiOpTroConDoc::CBiOpTroConDoc()
{
	// TODO: add one-time construction code here

}

CBiOpTroConDoc::~CBiOpTroConDoc()
{

int i;

i=5;

Sub71.AOnPoweroff2();//sylee20120702 	     Sub71.AOnPoweroff();

Sleep(500);


}





BOOL CBiOpTroConDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConDoc serialization

void CBiOpTroConDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConDoc diagnostics

#ifdef _DEBUG
void CBiOpTroConDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBiOpTroConDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBiOpTroConDoc commands
