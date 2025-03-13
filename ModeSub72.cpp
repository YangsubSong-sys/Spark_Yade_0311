// ModeSub72.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub72.h"
#include "ModeSUB71.h"
#include "Contec.h"
#include "IO.h"
#include "A_Define1.h"

 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 

//=======================================
//#define DEF_CARD_A1  //contec  //sylee170328 

#ifdef DEF_CARD_A1  //sylee170328

#else  //#ifdef DEF_CARD_A1  

	//=======================================
	#include "DioD64H.h"//sylee170229
	#include "DioD96.h"//sylee170229
	#include "UniDAQ.h"//sylee170229
	#include "PCI_D64H.h" //sylee170229 ㅣ
	extern CDIO_D64  pDIO_D64;//sylee170229
	CString str2;
	//=======================================

#endif  //#ifdef DEF_CARD_A1 



/////////////////////////////////////////////////////////////////////////////
// CModeSub72 dialog

extern CModeSub71  Sub71;
//TEMPextern short nTriAddFlag[20]; 

struct CSub72Set//Pin SETUP  
{
	int m_nGuard_Delay;// 1=QD, 2=DD, 3=SD
	int m_nRY_Delay;  //
	int m_nPinSet_Delay;
	int m_nMeasure_Delay;
	int m_nMeasure_Count;
	int m_nVB_BeginPin;
	int m_nVB_EndPin;
	int m_nVC_BeginPin;
	int m_nVC_EndPin;
};

CSub72Set sSub72Set;

//extern int nModeIR;



CModeSub72::CModeSub72(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub72::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub72)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nAutoFileDisplay=0;
}

void CModeSub72::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub72)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	
	 DDX_Control(pDX, IDC_LIST1, m_list1);

	 DDX_Control(pDX, IDC_EDIT103,   m_edit103); 
	 DDX_Control(pDX, IDC_EDIT104,   m_edit104); 

	 DDX_Control(pDX, IDC_EDIT201,   m_edit201); 
	 DDX_Control(pDX, IDC_EDIT202,   m_edit202);
	 DDX_Control(pDX, IDC_EDIT203,   m_edit203);
	 DDX_Control(pDX, IDC_EDIT204,   m_edit204);
	 DDX_Control(pDX, IDC_EDIT205,   m_edit205);

	 DDX_Control(pDX, IDC_EDIT301,   m_edit301); 
	 DDX_Control(pDX, IDC_EDIT302,   m_edit302);
	 DDX_Control(pDX, IDC_EDIT303,   m_edit303);
	 DDX_Control(pDX, IDC_EDIT304,   m_edit304);

	 DDX_Control(pDX, IDC_EDIT401,   m_edit401); 
	 DDX_Control(pDX, IDC_EDIT402,   m_edit402);
	 
	 DDX_Control(pDX, IDC_CHECK1,  m_check1);
	 DDX_Control(pDX, IDC_CHECK2,  m_check2);

	 DDX_Control(pDX, IDC_BUTTON110,  m_Button110);
	 DDX_Control(pDX, IDC_BUTTON120,  m_Button120);
	 DDX_Control(pDX, IDC_BUTTON140,  m_Button140);
	 DDX_Control(pDX, IDC_BUTTON141,  m_Button141);
	 DDX_Control(pDX, IDC_BUTTON142,  m_Button142);
	 DDX_Control(pDX, IDC_BUTTON903,  m_Button903);

	 DDX_Control(pDX, IDC_LABEL207,  m_Label207);
	 DDX_Control(pDX, IDC_LABEL234,  m_Label234);
	 DDX_Control(pDX, IDC_LABEL211,  m_Label211);
	 DDX_Control(pDX, IDC_LABEL219,  m_Label219);
	 DDX_Control(pDX, IDC_LABEL226,  m_Label226);
	 DDX_Control(pDX, IDC_LABEL227,  m_Label227);

	 DDX_Control(pDX, IDC_LABEL235,  m_Label235);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub72, CDialog)
	//{{AFX_MSG_MAP(CModeSub72)
	
	ON_BN_CLICKED(IDC_RADIO21, OnRadio21)
	ON_BN_CLICKED(IDC_RADIO22, OnRadio22)
	ON_BN_CLICKED(IDC_RADIO23, OnRadio23)
	ON_BN_CLICKED(IDC_RADIO24, OnRadio24)
	ON_BN_CLICKED(IDC_RADIO25, OnRadio25)
	ON_BN_CLICKED(IDC_RADIO26, OnRadio26)
	 ON_BN_CLICKED(IDC_RADIO01, OnRadio01) 
     ON_BN_CLICKED(IDC_RADIO02, OnRadio02) 
     ON_BN_CLICKED(IDC_RADIO03, OnRadio03) 
     ON_BN_CLICKED(IDC_RADIO04, OnRadio04) 
     ON_BN_CLICKED(IDC_RADIO05, OnRadio05) 
	 ON_BN_CLICKED(IDC_RADIO06, OnRadio06) 
     ON_BN_CLICKED(IDC_RADIO07, OnRadio07) 
     ON_BN_CLICKED(IDC_RADIO08, OnRadio08) 
     ON_BN_CLICKED(IDC_RADIO09, OnRadio09) 
     ON_BN_CLICKED(IDC_RADIO10, OnRadio10) 
     ON_BN_CLICKED(IDC_RADIO11, OnRadio11) 
	 ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	 ON_BN_CLICKED(IDC_RADIO13, OnRadio13) 
	 ON_BN_CLICKED(IDC_RADIO14, OnRadio14)
	 ON_BN_CLICKED(IDC_RADIO15, OnRadio15) 
	 ON_BN_CLICKED(IDC_RADIO16, OnRadio16)
	 ON_BN_CLICKED(IDC_RADIO18, OnRadio18)
	 ON_BN_CLICKED(IDC_RADIO19, OnRadio19)
	 ON_BN_CLICKED(IDC_RADIO20, OnRadio20)   //embedded
	 ON_BN_CLICKED(IDC_RADIO28, OnRadio28)   //embedded
	 ON_BN_CLICKED(IDC_RADIO29, OnRadio29)   //embedded
	 ON_BN_CLICKED(IDC_RADIO30, OnRadio30)   //embedded
	 ON_BN_CLICKED(IDC_RADIO31, OnRadio31)
	 ON_BN_CLICKED(IDC_RADIO32, OnRadio32)
	 ON_BN_CLICKED(IDC_RADIO33, OnRadio33)
	 ON_BN_CLICKED(IDC_RADIO34, OnRadio34)
	 ON_BN_CLICKED(IDC_RADIO35, OnRadio35)
	 ON_BN_CLICKED(IDC_RADIO36, OnRadio36)
	 ON_BN_CLICKED(IDC_RADIO37, OnRadio37)
	 ON_BN_CLICKED(IDC_RADIO38, OnRadio38)
	 ON_BN_CLICKED(IDC_RADIO39, OnRadio39)   //embedded
	 ON_BN_CLICKED(IDC_RADIO40, OnRadio40)   //embedded
	 ON_BN_CLICKED(IDC_RADIO41, OnRadio41)   //embedded
	 ON_BN_CLICKED(IDC_RADIO42, OnRadio42)   //embedded

     ON_BN_CLICKED(IDC_RADIO43, OnRadio43)   //embedded
     ON_BN_CLICKED(IDC_RADIO44, OnRadio44)   //embedded
	 ON_BN_CLICKED(IDC_RADIO45, OnRadio45)   //embedded
     ON_BN_CLICKED(IDC_RADIO46, OnRadio46)   //embedded

	 ON_BN_CLICKED(IDC_RADIO27, OnRadio27)

  
	 ON_BN_CLICKED(IDC_BUTTON306, OnButton303)
	 ON_BN_CLICKED(IDC_BUTTON307, OnButton304)	
	 ON_BN_CLICKED(IDC_BUTTON506, OnButton506)	 
	 ON_BN_CLICKED(IDC_BUTTON310, On_Test1)	 

	 ON_BN_CLICKED(IDC_BUTTON110, OnButton110)	 
	 ON_BN_CLICKED(IDC_BUTTON120, OnButton120)	 
	 ON_BN_CLICKED(IDC_BUTTON140, OnButton140)	 
	 ON_BN_CLICKED(IDC_BUTTON141, OnButton141)	 
	 ON_BN_CLICKED(IDC_BUTTON142, OnButton142)	 
	 
	 ON_BN_CLICKED(IDC_BUTTON903, OnButton903)
	 
	 ON_BN_CLICKED(IDC_BUTTON601, OnButton601)	

	 ON_BN_CLICKED(IDC_CHECK1, OnCheck1) 
		  
	 //}}AFX_MSG_MAP

END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CModeSub72, CDialog)

    //{{AFX_EVENTSINK_MAP(CModeSub712)
 
	ON_EVENT(CModeSub72, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	
	ON_EVENT(CModeSub72, IDC_BUTTON206,-600 /* Click */, OnButton206, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON306,-600 /* Click */, OnButton303, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON307,-600 /* Click */, OnButton304, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON310,-600 /* Click */, On_Test1, VTS_NONE)
    ON_EVENT(CModeSub72, IDC_BUTTON506,-600 /* Click */, OnButton506, VTS_NONE)

	ON_EVENT(CModeSub72, IDC_BUTTON110,-600 /* Click */, OnButton110, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON120,-600 /* Click */, OnButton120, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON140,-600 /* Click */, OnButton140, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON141,-600 /* Click */, OnButton141, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON142,-600 /* Click */, OnButton142, VTS_NONE)
	ON_EVENT(CModeSub72, IDC_BUTTON903,-600 /* Click */, OnButton903, VTS_NONE)

	ON_EVENT(CModeSub72, IDC_BUTTON127,-600 /* Click */, OnButton127, VTS_NONE) 

    ON_EVENT(CModeSub72, IDC_BUTTON601,-600 /* Click */, OnButton601, VTS_NONE)

  //ON_EVENT(CModeSub72, IDC_BUTTON207,-600 /* Click */, OnButton207, VTS_NONE)
	//ON_EVENT(CModeSub72, IDC_BUTTON302,-600 /* Click */, OnButton302, VTS_NONE)
 
	//}}AFX_EVENTSINK_MAP

END_EVENTSINK_MAP()





/////////////////////////////////////////////////////////////////////////////
// CModeSub72 message handlers
void CModeSub72::OnRadio01() 
{
 
}
 void CModeSub72::OnRadio02() 
{
 
}
 void CModeSub72::OnRadio03() 
{
 
}
 void CModeSub72::OnRadio04() 
{
 
}
 void CModeSub72::OnRadio05() 
{
 
}
 void CModeSub72::OnRadio06() 
{
 
}
void CModeSub72::OnRadio07() 
{
 
}
void CModeSub72::OnRadio08() 
{
 
}
void CModeSub72::OnRadio09() 
{
 
}
void CModeSub72::OnRadio10() 
{
 
}
void CModeSub72::OnRadio11() 
{
 
}
void CModeSub72::OnRadio12() 
{
 
}

void CModeSub72::OnClickOk() 
{
 
	CDialog::OnOK();

//	FileSysInfo01.LoadSaveSub71(2);
    OnButton506();


}

 void CModeSub72::OnRadio13() //I GAIN
{
	((CButton*)GetDlgItem( IDC_RADIO13))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO14))->SetCheck(FALSE); 
	 Sub71.I_Gain(1); // 
}


 
void CModeSub72::OnRadio14() 
{
 
	((CButton*)GetDlgItem( IDC_RADIO14))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO13))->SetCheck(FALSE);
	Sub71.I_Gain(2); 
 
}



void CModeSub72::OnRadio15() 
{
 
}
void CModeSub72::OnRadio16() 
{
 
}
 
void CModeSub72::OnRadio18() 
{
	((CButton*)GetDlgItem(IDC_RADIO18))->SetCheck(TRUE);
    ((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(FALSE); 
//TEMP	 Sub71.OnEmbed_Speed(1);//fast  //1=fast   2=Slow
}

void CModeSub72::OnRadio19() 
{
	((CButton*)GetDlgItem(IDC_RADIO18))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(TRUE); 
//TEMP	 Sub71.OnEmbed_Speed(2);//Slow
}


int CModeSub72::OnEmbed_Freq_Display(int nCh){

	((CButton*)GetDlgItem(IDC_RADIO20))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO28))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO29))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO30))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO39))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO40))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO41))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO42))->SetCheck(FALSE); 

	if( nCh==1) ((CButton*)GetDlgItem(IDC_RADIO20))->SetCheck(TRUE);
	if( nCh==2) ((CButton*)GetDlgItem(IDC_RADIO28))->SetCheck(TRUE); 
    if( nCh==3) ((CButton*)GetDlgItem(IDC_RADIO29))->SetCheck(TRUE); 
	if( nCh==4) ((CButton*)GetDlgItem(IDC_RADIO30))->SetCheck(TRUE); 
	if( nCh==5) ((CButton*)GetDlgItem(IDC_RADIO39))->SetCheck(TRUE); 
    if( nCh==6) ((CButton*)GetDlgItem(IDC_RADIO40))->SetCheck(TRUE); 
	if( nCh==7) ((CButton*)GetDlgItem(IDC_RADIO41))->SetCheck(TRUE); 
	if( nCh==8) ((CButton*)GetDlgItem(IDC_RADIO42))->SetCheck(TRUE); 

	return  1;
}



void CModeSub72::OnRadio20() 
{
	 Sub71.OnEmbed_Freq(1);//200hz
	 OnEmbed_Freq_Display(1);

//	 Oninit1();//test

}

void CModeSub72::OnRadio28() 
{
	 Sub71.OnEmbed_Freq(2);//500hz
	 OnEmbed_Freq_Display(2);
}

void CModeSub72::OnRadio29() 
{ 
	 Sub71.OnEmbed_Freq(3);//1khz
	 OnEmbed_Freq_Display(3);
}

void CModeSub72::OnRadio30() 
{ 
	 Sub71.OnEmbed_Freq(4);//5khz 
	 OnEmbed_Freq_Display(4);
}

void CModeSub72::OnRadio39() 
{
	 Sub71.OnEmbed_Freq(5); //10khz
	 OnEmbed_Freq_Display(5);
}

void CModeSub72::OnRadio40() 
{
	 Sub71.OnEmbed_Freq(6);//20khz  
	 OnEmbed_Freq_Display(6);
}

void CModeSub72::OnRadio41() 
{ 
	 Sub71.OnEmbed_Freq(7);//50khz 
	 OnEmbed_Freq_Display(7);
}

void CModeSub72::OnRadio42() 
{ 
	 Sub71.OnEmbed_Freq(8);//100khz 
	 OnEmbed_Freq_Display(8);
}

void CModeSub72::OnRadio43() 
{ 
	 
}

void CModeSub72::OnRadio44() 
{ 
	 
}
void CModeSub72::OnRadio45() 
{ 
	 
}
void CModeSub72::OnRadio46() 
{ 
	 
}

void CModeSub72::OnRadio31() 
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(TRUE);
    ((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO34))->SetCheck(FALSE); 

	 Sub71.OnEmbed_VSet(1);// 1v 
}

void CModeSub72::OnRadio32() 
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(TRUE); 
    ((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO34))->SetCheck(FALSE); 
	 Sub71.OnEmbed_VSet(2);// 2V
}

void CModeSub72::OnRadio33() 
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(TRUE); 
	((CButton*)GetDlgItem(IDC_RADIO34))->SetCheck(FALSE); 
	 Sub71.OnEmbed_VSet(3);// 5V 
}
void CModeSub72::OnRadio34() 
{
	((CButton*)GetDlgItem(IDC_RADIO31))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO32))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO33))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO34))->SetCheck(TRUE); 
	 Sub71.OnEmbed_VSet(4);// 10V
}


void CModeSub72::OnRadio35() 
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(TRUE);
    ((CButton*)GetDlgItem(IDC_RADIO36))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO38))->SetCheck(FALSE); 
	 Sub71.OnEmbed_AV_Set(1);// 1v 
}
void CModeSub72::OnRadio36() 
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO36))->SetCheck(TRUE); 
    ((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO38))->SetCheck(FALSE); 
	 Sub71.OnEmbed_AV_Set(2);// 1v 
}
void CModeSub72::OnRadio37() 
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO36))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(TRUE); 
	((CButton*)GetDlgItem(IDC_RADIO38))->SetCheck(FALSE); 
	 Sub71.OnEmbed_AV_Set(3);// 1v 
}
void CModeSub72::OnRadio38() 
{
	((CButton*)GetDlgItem(IDC_RADIO35))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO36))->SetCheck(FALSE); 
    ((CButton*)GetDlgItem(IDC_RADIO37))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_RADIO38))->SetCheck(TRUE); 
	 Sub71.OnEmbed_AV_Set(4);// 1v 
}

void CModeSub72::OnRadio21() 
{
	int Ret;
	CString str;

	((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);
	
 	Ret=Sub71.AOnIRanSet1Run1( 1 , 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
 	str.Format("Range 1");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=1;
 

}
 
void CModeSub72::OnRadio22() 
{
	int Ret;
	CString str;
	((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);
		Ret=Sub71.AOnIRanSet1Run1( 2 , 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
	str.Format("Range 2");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=2;
}
void CModeSub72::OnRadio23() 
{
	int Ret;
	CString str;
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);
	Ret=Sub71.AOnIRanSet1Run1( 3, 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
	str.Format("Range 3");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=3;
}
void CModeSub72::OnRadio24() 
{
	int Ret;
	CString str;
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);
	Ret=Sub71.AOnIRanSet1Run1( 4, 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
	str.Format("Range 4");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=4;
}
 
 void CModeSub72::OnRadio25() 
{
	 int Ret;
	 CString str;
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);
	Ret=Sub71.AOnIRanSet1Run1( 5, 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
	str.Format("Range 5");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=5;
}



 void CModeSub72::OnRadio26() 
{
	 int Ret;
	 CString str;
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);
   	Ret=Sub71.AOnIRanSet1Run1( 6, 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
	str.Format("Range 6");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=6;
}

 void CModeSub72::OnRadio27() 
{
	 int Ret;
	 CString str;
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(TRUE);
    ((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	Ret=Sub71.AOnIRanSet1Run1( 74, 1); //  I range  ( 1~5,   off=1  on  o off   
	if( Ret!=1 ){   
          AfxMessageBox("   I Range1  Mode Set Error ", MB_OK);   return; 	   
    }
	str.Format("Range 7");
 //	m_Label202.SetCaption(str); 
	m_list1.InsertString(0,str);
 	nModeIR=7;
}

 

 
void CModeSub72::OnButton206() 
{
	CString str; 

 
    Sub71.AOnIRanSet1Run1(0,0);

	str.Format("I Rang.Reset");
 //	m_Label202.SetCaption(str);
	m_list1.InsertString(0,str);

 	nModeIR=0;

	((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
    ((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO26))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO27))->SetCheck(FALSE);

}



 
 
void CModeSub72::OnButton303() //SYLEE20120215
{


	CString str;	
	char szText[20];
	int a, Ret;   
	BYTE La,Ha;

 
	nICo=0;

	//if( nMode1==1 ||  nMode1==2 ||  nMode1==3 ||  nMode1==4 ){  //SYLEE20111005
    //       AfxMessageBox(" SET Error 717.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
    //}

	 if( nModeIR<1  ||  nModeIR>7 ){
		str.Format(" * ERROR : I SET  (1~7)  Range Mode? " ); 
		m_list1.InsertString(0,str);
		m_edit103.SetWindowText("Range?");
		return;
	 }
 
   	m_edit103.GetWindowText( szText,10);	
	a = atoi( szText );
	if( a > 65535){ 
		str.Format(" * ERROR : I SET 16bit=%d  > 65535 ",a); 
	    m_list1.InsertString(0,str);
		m_edit103.SetWindowText("");
		return;
	}
 
    La=(BYTE)(a&0xff);
	Ha=(BYTE)((a>>8)&0xff);




#ifdef DEF_CARD_A1  //sylee170328

	Ret = DioOutBit(pDIO.hDrv, DO17, 1);   // /I SET CLEAR
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 17, 1] Error!!!");	return  ;	}
    Ret = DioOutByte(pDIO.hDrv, 0, La); 
	if( Ret!=0 ){ AfxMessageBox("DioOutByte[  ] Error!!!");	return  ;	}
    Ret = DioOutByte(pDIO.hDrv, 1, Ha); 
	if( Ret!=0 ){ AfxMessageBox("DioOutByte[  ] Error!!!");	return  ;	} 
	Ret = DioOutBit(pDIO.hDrv, DO16, 1);    //I SET CLOCK
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 61, 1] Error!!!");	return  ;	}  
 	Sub71.Delay_100ns(500);	//SYLEE20120223     OK   //(200); ERROR 
	Ret = DioOutBit(pDIO.hDrv, DO16, 0);    //I SET CLOCK
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 61, 0] Error!!!");	return  ;	} 

#else  //#ifdef DEF_CARD_A1  
	
	Ret = Ixud_WriteDOBit(0,2,1,1);  //I SET CLEAR  //sylee160427
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,2,1,1) Error!!!");	return    ;}

	Ret = Ixud_WriteDO(0,0,La);	 
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOByte(0,0,La) Error!!!");	return   ;	}
	Ret = Ixud_WriteDO(0,1,Ha);	 
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOByte(0,1,Ha) Error!!!");	return   ;	}

	
	Ret = Ixud_WriteDOBit(0,2,0,1);  //I SET CLOCK
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,2,0,1) Error!!!");	return    ;}

 	Sub71.Delay_100ns(500);	//SYLEE20120223     OK   //(200); ERROR  
	
	Ret = Ixud_WriteDOBit(0,2,0,0);  //I SET CLOCK
	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,2,0,0) Error!!!");	return    ;}



#endif  //#ifdef DEF_CARD_A1 
  
	str.Format(" curren sensing 16bit=%d  ",a); 
	m_list1.InsertString(0,str);
	//nflgA=1; 
    Sub71.Delay_100ns(50000);  //5msec OK 

} 
 

void CModeSub72::OnButton304() 
{
	CString str; 
	char	szText[20];
 	int a; 
    double c; 
 

	//if( nMode1==1 ||  nMode1==2 ||  nMode1==3 ||  nMode1==4 ){   
    //      AfxMessageBox(" SET Error 718.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
    //}

	//if( nModeIR<1  ||  nModeIR>6 ){
	//	str.Format(" * ERROR : 전류 세팅    Range Mode? " ); 
	//    m_list1.InsertString(0,str);
	//	m_edit103.SetWindowText("Range?");
	//	return;
	//}

   	m_edit104.GetWindowText( szText,10);	
 
	c = atof( szText );
 	lIset=(double)c;

	if(c==0){
		a=0;
	}else{
		if(nModeIR==1){	
			if(c>0 && c<=100.0){
				a=(int)(c*40.0);   
			}else{
				a=0;
			}
		}else if(nModeIR==2  || nModeIR==6 ){  
			if(c>0 && c<= 20.0 ){
			//sylee130627	a=(int)(c*5000.0);  
				a=(int)(c*200.0); 
			}else{
				a=0;
			}
		}else if(nModeIR==3){
			if(c>0 && c<=10.0){
				a=(int)(c*400.0);   
			}else{
				a=0;
			}
		}else if(nModeIR==4){
			if(c>0 && c<=2.0){
				a=(int)(c*2000.0);   
			}else{
				a=0;
			} 
		}else if(nModeIR==5){
			if(c>0 && c<=200.0){
				a=(int)(c*20.0);   
			}else{
				a=0;
			} 
		}
	}//if(c==0){ }else{
 
	str.Format("%d", a); 
	m_edit103.SetWindowText(str); 

  // V1=0;  //vb
	 Y1=0;  //i
	 X1=0;  //vc
	 R1=0;  //R

    OnButton303();

	str.Format(" current  setting value =%d  ",a); 
	m_list1.InsertString(0,str);
 
}


 
void CModeSub72::Oninit1() 
{
	
	m_edit201.SetWindowText("3000"); //guard delay
	m_edit202.SetWindowText("3000"); //ry apply deley
	m_edit203.SetWindowText("0"); //pinset delay
	m_edit204.SetWindowText("3000"); //measure  delay
	m_edit205.SetWindowText("1"); //measure count

	m_edit301.SetWindowText("1"); //Start Pin  
	m_edit302.SetWindowText("64"); //End Pin  
	m_edit303.SetWindowText("65"); //Start Pin  
	m_edit304.SetWindowText("128"); //End Pin  

	m_edit401.SetWindowText("10"); //VB Guard V Set  
	m_edit402.SetWindowText("10"); //VC Guard V Set  
}

void CModeSub72::On_Test1() 
{	
	On_Test1_Run1();

}


void CModeSub72::On_Initial_Display1() 
{	
    m_edit201.SetWindowText("1000"); //guard delay
	m_edit202.SetWindowText("1000"); //ry apply deley
	m_edit203.SetWindowText("0"); //pinset delay
	m_edit204.SetWindowText("3000"); //measure  delay
	m_edit205.SetWindowText("1"); //measure count

	m_edit301.SetWindowText("1"); //Start Pin  
	m_edit302.SetWindowText("64"); //End Pin  
	m_edit303.SetWindowText("65"); //Start Pin  
	m_edit304.SetWindowText("128"); //End Pin  

	m_edit401.SetWindowText("10"); //VB Guard V Set  
	m_edit402.SetWindowText("10"); //VC Guard V Set 
	
    m_edit104.SetWindowText("10"); //i set

}





void CModeSub72::On_Get_SetValue() 
{
  	char szText[100];

   	m_edit201.GetWindowText( szText,10);	
	sSub72Set.m_nGuard_Delay = atoi( szText );
   	m_edit202.GetWindowText( szText,10);	
	sSub72Set.m_nRY_Delay = atoi( szText );
   	m_edit203.GetWindowText( szText,10);	
	sSub72Set.m_nPinSet_Delay = atoi( szText );
	m_edit204.GetWindowText( szText,10);
   	sSub72Set.m_nMeasure_Delay = atoi( szText );
   	m_edit205.GetWindowText( szText,10);	
	sSub72Set.m_nMeasure_Count = atoi( szText );
	
   	m_edit301.GetWindowText( szText,10);	
	sSub72Set.m_nVB_BeginPin = atoi( szText );
   	m_edit302.GetWindowText( szText,10);	
	sSub72Set.m_nVB_EndPin = atoi( szText );
   	m_edit303.GetWindowText( szText,10);	
	sSub72Set.m_nVC_BeginPin = atoi( szText );
	m_edit304.GetWindowText( szText,10);
   	sSub72Set.m_nVC_EndPin = atoi( szText );

}



void CModeSub72::On_Test1_Run1() 
{
	int nStartPin1, nEndPin1,nStartPin2, nEndPin2,nStartPin2Gap1;
	int nRet1,k,nList1,m,n;
//temp	DWORD DHa;  
	double **dData1;

//================================================================
    On_Get_SetValue();
 //================================================================
	nList1=(sSub72Set.m_nVB_EndPin-sSub72Set.m_nVB_BeginPin);
	m=sSub72Set.m_nMeasure_Count+2 ;
	n=nList1+2;

    dData1=new double *[m];
	for(int i=0; i<m; i++){
		dData1[i]=new double[n];  
	}
//================================================================
    //On_Get_SetValue();

	nStartPin1=sSub72Set.m_nVB_BeginPin;
	nEndPin1=sSub72Set.m_nVB_EndPin;
	nStartPin2=sSub72Set.m_nVC_BeginPin;
	nStartPin2Gap1=sSub72Set.m_nVC_BeginPin-sSub72Set.m_nVB_BeginPin;

	if( nStartPin1<1 || nEndPin1<1  || nStartPin2<1  || nStartPin2Gap1<1 ){
		AfxMessageBox(" Pin Set  Error     No<1  ?  ", MB_OK);
		return ;
	}
	
	nEndPin2=sSub72Set.m_nVC_EndPin;



	 for( int j=1;j<=sSub72Set.m_nMeasure_Count;j++){	
		for( int i=nStartPin1;i<=nEndPin1;i++){			
			 Sub71.OnPinAllReset1();
			 nRet1=Sub71.AOnPinSet1(1, i-1, 1); 
			 if(nRet1!=1){
				 AfxMessageBox(" Pin Set  Error   ", MB_OK);
				 break;
			 }
			 nRet1=Sub71.AOnPinSet1(1, i-1+nStartPin2Gap1, 2); 
			 if(nRet1!=1){
				 AfxMessageBox(" Pin Set  Error   ", MB_OK);
				 break;
			 } 		


			  Sub71.Delay_100ns(sSub72Set.m_nMeasure_Delay);

//TEMP			 DHa=Sub71.AVB_PULSE_ADC();

		//	 dData1[j][i-nStartPin1+1]=(double)(DHa/500.);
//TEMP			 dData1[j][i-nStartPin1+1]=(double)(DHa);

		}//end of for( int i=nStartPin1,i<=nEndPin1;i++){
	}
//================================================================
//file logging

	 	
	FILE *fp; 
	CFile file; 
	CString str;
	char fName[200]; 
 

	
	//str.Format("d:\\EmTestlog.txt"); 


	str.Format("d:\\EmTestlog.CSV"); 


	::ZeroMemory(&fName, sizeof(fName));
	strcat( fName , str );
	fp = fopen(fName,"wt");
	if(fp == NULL){ 
	    str.Format("Error No 7014 :   d:\\ EmTestlog.txt   file Data Saving Error !     file close!.");
		AfxMessageBox(str, MB_OK);			
    	return  ;
	} 

	fprintf(fp, "  ," );
	for(   j=1;j<=sSub72Set.m_nMeasure_Count;j++){
		fprintf(fp, "  %d,", j);
	}
	fprintf(fp,"  \n\n" );
 

	
	for( i=nStartPin1;i<=nEndPin1;i++){
		 k=i-nStartPin1+1;
		 fprintf(fp, "  %d,", k);

		 for(   j=1;j<=sSub72Set.m_nMeasure_Count;j++){
			fprintf(fp, "  %.2f,",dData1[j][k]);
		 } 
		 fprintf(fp,"  \n" );
	}

 		CTime curTime = CTime::GetCurrentTime();	 
 
		fprintf(fp, "*> Make date:  %d/%d/%d  %d:%d:%d \n",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),  curTime.GetHour(),curTime.GetMinute(), curTime.GetSecond() );
 		fprintf(fp, " \n\n");
 
		fclose(fp);  


//================================================================
	for(  i=0; i<m; i++) 
		delete[] dData1[i];	 

	delete[] dData1;
	dData1='\0';
//================================================================

	if(m_nAutoFileDisplay==1){
		::ShellExecute(NULL,"open","EXCEl.EXE","d:\\EmTestlog.CSV","NULL",SW_SHOWNORMAL);
	//	::ShellExecute(NULL,"open","NOTEPAD.EXE","d:\\EmTestlog.CSV","NULL",SW_SHOWNORMAL);
	}

}



void CModeSub72::On_Test1_log1() 
{

}
 
 

BOOL CModeSub72::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str; 
 
	On_Initial_Display1();

	Sub71.AOnRelayOff1(); 
	Sub71.Delay_100ns(50000);  //5m sec 

	OnButton506();//initial 

	OnButton120();//cc
	OnButton127();

	OnRadio14();//x16
	OnRadio19();//slow 
	OnRadio39();//10khz
	OnRadio33();//5v
	OnRadio37();//AV3
 	OnRadio43();//VB GUARD CC
  	OnRadio45();//VC GUARD CC
  	OnRadio23();//I 10mA
	OnButton140();
	OnButton141();
	OnButton142();
	OnButton903();
	OnButton141();
	OnButton903();


	Sub71.AOnRelayOff1(); 
	Sub71.Delay_100ns(50000);  //5m sec 



	m_nAutoFileDisplay=1;
    m_check1.SetCheck(m_nAutoFileDisplay); 
 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





void CModeSub72::OnButton506() 
{
	CString str;
	int Ret,i; 
 
	nModeIR=0;
 
    Sub71.OnPinAllReset1();   ///PIN ALL RESET

//TEMP	::ZeroMemory(&nTriAddFlag, sizeof(nTriAddFlag));

//	nTimeDelay1=1000;



#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);   //###?????????????
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 35, 0] Error!!!");	return  ;	}
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);    //###?????????????
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 36, 0] Error!!!");	return  ;	}
	Ret = DioOutBit(pDIO.hDrv, DO37, 0);    //###?????????????
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return  ;	}

    for( i=0; i<16; i++){
	  	Ret = DioOutByte(pDIO.hDrv, i, 0x0);  
		if( Ret!=0 ){ AfxMessageBox("DioOutByte[  ] Error!!!");	return  ;	}
	}

#else  //#ifdef DEF_CARD_A1   

/*
	Ret = DioOutBit(pDIO.hDrv, DO35, 0);   
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 35, 0] Error!!!");	return  ;	}
	Ret = DioOutBit(pDIO.hDrv, DO36, 0);   
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 36, 0] Error!!!");	return  ;	}
	Ret = DioOutBit(pDIO.hDrv, DO37, 0);   
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 37, 0] Error!!!");	return  ;	}
*/

//sylee170411	Ret = d64h_do_writeport( pDIO_D64.m_bCardID, 0x00000000 );  //
//sylee170411	if( Ret!=0 ) { AfxMessageBox("  Card64H1_DO32(0x00000000 ) Error!!!");	return   ;	}	
//sylee170411	Ret = d64h_do_writeport( pDIO_D64.m_bCardID2, 0x00000000 );  //
//sylee170411	if( Ret!=0 ) { AfxMessageBox("  Card64H2_DO32(0x00000000 ) Error!!!");	return   ;	}

	Ret = d64h_do_writeport( pDIO_D64.m_bCardID, 0x80000  );  //
	if( Ret!=0 ) { AfxMessageBox("  Card64H1_DO32(0x80000  ) Error!!!");	return   ;	}	
	Ret = d64h_do_writeport( pDIO_D64.m_bCardID2, 0x80000  );  //
	if( Ret!=0 ) { AfxMessageBox("  Card64H2_DO32(0x80000  ) Error!!!");	return   ;	}
	for(   i=0; i<=11; i++){
	   Ret=Ixud_WriteDO(0,i,0x00);
	   if( Ret!=0 ) { AfxMessageBox("  Card96_DOByte(i,0x00) Error!!!");	return   ;	}
	}

#endif  //#ifdef DEF_CARD_A1 


	str.Format("->Digital ALL OFF");
    m_list1.InsertString(0,str);
//	str.Format("0");
//	m_Label101.SetCaption(str); 
//---------------------------------------------------
	str.Format("Poweroff"); 
	m_list1.InsertString(0,str); 

	str.Format("-> INITIAL ");
	m_list1.InsertString(0,str);  
 
	((CButton*)GetDlgItem( IDC_RADIO21))->SetCheck(FALSE);
    ((CButton*)GetDlgItem( IDC_RADIO22))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO23))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO24))->SetCheck(FALSE);
	((CButton*)GetDlgItem( IDC_RADIO25))->SetCheck(FALSE);



#ifdef DEF_CARD_A1  //sylee170328	 
 	Ret = DioOutBit(pDIO.hDrv, DO38, 0);     //  DA_IO_SET_CE   INITIAL
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 38, 0] Error!!!");	return  ;	}
	Ret = DioOutBit(pDIO.hDrv, DO39, 0);   //   /DA_IO_SET_CLR   INITIAL
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 39, 0] Error!!!");	return  ;	}
    Ret = DioOutBit(pDIO.hDrv, DO55, 1);   // AD_CONVST_A 
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 55, 1] Error!!!");	return  ;	}
#else  //#ifdef DEF_CARD_A1  


/* //sylee170426
	Ret = DioOutBit(pDIO.hDrv, DO38, 0);     //  DA_IO_SET_CE   INITIAL
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 38, 0] Error!!!");	return  ;	}
	Ret = DioOutBit(pDIO.hDrv, DO39, 0);   //   /DA_IO_SET_CLR   INITIAL
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 39, 0] Error!!!");	return  ;	}
    Ret = DioOutBit(pDIO.hDrv, DO55, 1);   // AD_CONVST_A 
	if( Ret!=0 ) { AfxMessageBox(" [DioOutBit, 55, 1] Error!!!");	return  ;	}
	*/
#endif  //#ifdef DEF_CARD_A1 


//TEMP    Sub71.OnTrigger1(0,0);    //add=100 all off
 
    nModeCC=1;

    OnButton110();
 	OnButton141();
 	OnButton903();

}


void CModeSub72::OnCheck1()  
{
	if(m_check1.GetCheck())
	{
		m_nAutoFileDisplay=1;
	}
	else
	{
		m_nAutoFileDisplay=0;
	}
	m_check1.SetCheck( m_nAutoFileDisplay); 
}


void CModeSub72::OnCheck2()  
{
	if(m_check2.GetCheck())
	{
//		m_nAutoFileDisplay=1;
	}
	else
	{
//		m_nAutoFileDisplay=0;
	}
//	m_check2.SetCheck( m_nAutoFileDisplay); 

}
 

void CModeSub72::OnButton110() //CC  CV MODE  SET
{
	CString str; 
   

	if( nMode1==1 ||  nMode1==2 ||  nMode1==3 ||  nMode1==4 ){  //SYLEE20111005
       AfxMessageBox(" SET Error NO 711.   LV1, LV2, HV1  HV2  RELAY ON.    OFF PLEASE!", MB_OK); 	   return;
   }

	if( nModeCC<1){
    	 nModeCC=1;
//TEMP		 Sub71.OnTrigger1(0,3);  //CV  ON 

		 Sub71.OnCCCV_Set(2); //CC=1, CV=2 SET  //sylee13060550118
		 
		 str.Format("0");
	     m_edit104.SetWindowText(str); 
//TEMP	 	 OnButton304();

		 str.Format("CV"); 
 	     m_Label207.SetCaption(str);
		 str.Format("CV SET"); 
		 m_list1.InsertString(0,str); 
 	

	}else{
		nModeCC=0;
//TEMP		Sub71.OnTrigger1(0,2);  //add    /CC OFF
		Sub71.OnCCCV_Set(1); //CC=1, CV=2 SET  //sylee13060550118
 
		str.Format("CC"); 
  	    m_Label207.SetCaption(str);	
		str.Format("CC SET"); 
		m_list1.InsertString(0,str);
	} 
}


void CModeSub72::OnButton120() //VB AC MODE
{
	CString str;
 
	if( nModeVB_AC>0){
//TEMP		Sub71.OnTrigger1(1,2);  //add  on 
		Sub71.OnVB_AC_Mode_Set(0); //ON=1, Off=0 SET  //sylee150118
		nModeVB_AC=0;  
		str.Format("OFF");
  	    m_Label234.SetCaption(str);	
		str.Format("=> VB AC MODE OFF"); 
	    m_list1.InsertString(0,str); 
	}else{ 
//TEMP		Sub71.OnTrigger1(1,3);  //add  on 

		 Sub71.AOnRelayOff1(); 
		 Sub71.Delay_100ns(50000);  //5m sec 
		//HV _AC_Mode_Set OFF
		Sub71.OnVB_AC_Mode_Set(1); //ON=1, Off=0 SET  //sylee150118
		nModeVB_AC=1; 	  
		str.Format("ON"); 
 	    m_Label234.SetCaption(str);
		str.Format("=> VB AC MODE ON"); 
	    m_list1.InsertString(0,str);  
	} 

}


void CModeSub72::OnButton140() //VB   guard on
{
	CString str;

	if( nMode1==3 || nMode1==4){
		AfxMessageBox(" Error No 1001.  HV1  HV2  RELAY ON ? ", MB_OK);
		return;
	}
 
 	if( nModeVB_OUT2>0){
//TEMP		Sub71.OnTrigger1(8,2);  //add  on 
		nModeVB_OUT2=0;  
		str.Format("OFF");
  	    m_Label211.SetCaption(str);	 

		str.Format("=> VB OUT CHK     OFF"); 
	    m_list1.InsertString(0,str);  
 
	}else{ 
//TEMP 		Sub71.OnTrigger1(8,3);  //add  on 
		nModeVB_OUT2=1; 	  
		str.Format("ON"); 
 	    m_Label211.SetCaption(str);
	    str.Format("=> VB OUT CHK     ON"); 
	    m_list1.InsertString(0,str); 
	}  
 
}


void CModeSub72::OnButton141() //VB   guard sel
{
	CString str;
	if( nMode1==3 || nMode1==4){
		AfxMessageBox(" Error No 1001.  HV1  HV2  RELAY ON ? ", MB_OK);
		return;
	}
	
	if(nModeVB_OUT3<0){
		nModeVB_OUT3=0;
	}
 
	if( nModeVB_OUT3>0){
//TEMP		Sub71.OnTrigger1(9,2);  //add  on 
		nModeVB_OUT3=0;  
		str.Format("CC");
  	    m_Label219.SetCaption(str);	 

		str.Format("=> VB OUT CHK     OFF"); 
	    m_list1.InsertString(0,str);  
	}else{ 
//TEMP		Sub71.OnTrigger1(9,3);  //add  on 
		nModeVB_OUT3=1; 	  
		str.Format("CV");
 	    m_Label219.SetCaption(str);
	    str.Format("=> VB OUT CHK     ON"); 
	    m_list1.InsertString(0,str); 
	}  
 
}

void CModeSub72::OnButton142() //Vc guard on
{
	CString str;
 
	if( nMode1==3 || nMode1==4){
		AfxMessageBox(" Error No 1001.  HV1  HV2  RELAY ON ? ", MB_OK);
		return;
	}

	if( nModeVB_OUT4>0){
//TEMP		Sub71.OnTrigger1(10,2);  //add  on 
		nModeVB_OUT4=0;  
		str.Format("OFF");
  	    m_Label226.SetCaption(str);	 
		str.Format("=> VB OUT CHK     OFF"); 
	    m_list1.InsertString(0,str);  
	}else{ 
//TEMP		Sub71.OnTrigger1(10,3);  //add  on 
		nModeVB_OUT4=1; 	  
		str.Format("ON"); 
 	    m_Label226.SetCaption(str);
	    str.Format("=> VB OUT CHK     ON"); 
	    m_list1.InsertString(0,str); 
	}  
 
}

void CModeSub72::OnButton903() //Vc guard sel
{
	CString str;
	if( nMode1==3 || nMode1==4){
		AfxMessageBox(" Error No 1001.  HV1  HV2  RELAY ON ? ", MB_OK);
		return;
	}

	if(nModeVB_OUT5<0){
		nModeVB_OUT5=0;  //DEFAULT  CV
	}
 
	if( nModeVB_OUT5>0){
//TEMP		Sub71.OnTrigger1(11,2);  //add  on 
		nModeVB_OUT5=0;  
		str.Format("CC");
  	    m_Label227.SetCaption(str);	 
		str.Format("=> VB OUT CHK     OFF"); 
	    m_list1.InsertString(0,str);  
	}else{ 
//TEMP		Sub71.OnTrigger1(11,3);  //add  on 
		nModeVB_OUT5=1; 	  
		str.Format("CV");
 	    m_Label227.SetCaption(str);
	    str.Format("=> VB OUT CHK     ON"); 
	    m_list1.InsertString(0,str); 
	}   
}


void CModeSub72::OnButton127() //FILTER  SET
{
	CString str; 
    
    Sub71.FilterSet(0); 

	if(nMode1!=5){ 
		str.Format("ON");
 		m_Label235.SetCaption(str);

		str.Format("=> PULSE ON");
		m_list1.InsertString(0,str);
 
//TEMP		Sub71.AOnRelayON_Mode1(5);
		nMode1=5;		
	}else{
		str.Format("OFF");
 		m_Label235.SetCaption(str);

		str.Format("=> PULSE OFF");
		m_list1.InsertString(0,str);
 
//TEMP		Sub71.AOnRelayON_Mode1(0);
		nMode1=0;  
	}

}


void CModeSub72::OnButton601() 
{
 
	//	::ShellExecute(NULL,"open","NOTEPAD.EXE","d:\\EmTestlog.CSV","NULL",SW_SHOWNORMAL);
 
	::ShellExecute(NULL,"open","EXCEl.EXE","d:\\EmTestlog.CSV","NULL",SW_SHOWNORMAL);

}
