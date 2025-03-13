 
#include "stdafx.h"
  
#include "DioD64H.h"
#include "PCI_D64H.h"
 
#include "A_Define2.h"//sylee181015


 CDIO_D64::CDIO_D64()
 { 

	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
		  CardH64_Open1();
	#endif  //#ifdef DEF_CARD_A1 

 }

 CDIO_D64::~CDIO_D64()
 { 
	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
		  CardH64_Close1();
	#endif  //#ifdef DEF_CARD_A1 

 } 
 
short CDIO_D64::CardH64_Open1()
{
	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
    
	//----------------------------------
	//son Check D64H System Resource

	nErrCode = d64h_scan( &wCardNum, NULL ); 
	if( nErrCode == SUCCESS_NO_ERROR )
	{

        //------------------------
	    //son Get Card 0 Info
        //------------------------
		nErrCode = d64h_cardinfo_get( 0, &bCardId );
		  //  strErrMsg.Format( TEXT(" d64h_open : wCardNum=%d bCardId=%d  "), wCardNum ,bCardId);//test
          //  AfxMessageBox( strErrMsg );
		if( nErrCode == SUCCESS_NO_ERROR ){
			nErrCode = d64h_open( bCardId );
			if( nErrCode == SUCCESS_NO_ERROR ){
				m_bCardID = bCardId;
				//	d64h_do_writeport( m_bCardID, 0x00000000 );
				//sylee170411		if( (nErrCode =d64h_do_writeport( m_bCardID, 0x00000000 )) != SUCCESS_NO_ERROR )
				//sylee170411		{
				//sylee170411			tmpMsg.Format( TEXT("d64h_do_writeport(ch1,0x00000000) falied with error code : %d"), nErrCode );
				//sylee170411			AfxMessageBox( tmpMsg );
				//sylee170411		}
		     	if( (nErrCode =d64h_do_writeport( m_bCardID, 0x80000  )) != SUCCESS_NO_ERROR )
				{
					tmpMsg.Format( TEXT("d64h_do_writeport(ch1,0x80000 ) falied with error code : %d"), nErrCode );
					AfxMessageBox( tmpMsg );
				}
				// GetDlgItem( IDC_BUTTON1 )->EnableWindow( TRUE );
				// m_nTimer = SetTimer( 1, 100, NULL );
			}else{
				tmpMsg.Format( TEXT(" d64h_open() falied ! nErrCode=%d "), nErrCode );
				AfxMessageBox( tmpMsg );
			}
		}else{
			tmpMsg.Format( TEXT(" d64h_cardinfo_get() falied ! nErrCode=%d "), nErrCode );
			AfxMessageBox( tmpMsg );
		}			
		
		//===========================================================
		nErrCode = d64h_cardinfo_get( 1, &bCardId2 );
		  //  strErrMsg.Format( TEXT(" d64h_open : wCardNum=%d bCardId=%d  "), wCardNum ,bCardId);//test
          //  AfxMessageBox( strErrMsg );
		if( nErrCode == SUCCESS_NO_ERROR ){
			nErrCode = d64h_open( bCardId2 );
			if( nErrCode == SUCCESS_NO_ERROR ){
				m_bCardID2 = bCardId2;
				//	d64h_do_writeport( m_bCardID2, 0x00000000 );
				 //sylee170411   if( (nErrCode =d64h_do_writeport( m_bCardID2, 0x00000000 )) != SUCCESS_NO_ERROR )
				//sylee170411	{
				//sylee170411		tmpMsg.Format( TEXT("d64h_do_writeport(ch2,0x00000000) falied with error code : %d"), nErrCode );
				//sylee170411		AfxMessageBox( tmpMsg );
				//sylee170411	}
				if( (nErrCode =d64h_do_writeport( m_bCardID2, 0x80000  )) != SUCCESS_NO_ERROR )
				{
					tmpMsg.Format( TEXT("d64h_do_writeport(ch2,0x80000 ) falied with error code : %d"), nErrCode );
					AfxMessageBox( tmpMsg );
				}

			//	GetDlgItem( IDC_BUTTON1 )->EnableWindow( TRUE );
			//	m_nTimer = SetTimer( 1, 100, NULL );
			}else{
				tmpMsg.Format( TEXT(" d64h_open(2) falied ! nErrCode=%d "), nErrCode );
				AfxMessageBox( tmpMsg );
			}
		}else{
			tmpMsg.Format( TEXT(" d64h_cardinfo_get(2) falied ! nErrCode=%d "), nErrCode );
			AfxMessageBox( tmpMsg );
		}
		//===========================================================
	}else{
		tmpMsg.Format( TEXT(" d64h_scan() falied ! nErrCode=%d "), nErrCode );
		AfxMessageBox( tmpMsg );
	}
	#endif  //#ifdef DEF_CARD_A1 

	return 1;
}



short CDIO_D64::CardH64_Close1()
{ 
	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
		if( m_bCardID != INVALID_CARD_ID )
			d64h_close( m_bCardID );
		if( m_bCardID2 != INVALID_CARD_ID )
			d64h_close( m_bCardID2 );
	#endif  //#ifdef DEF_CARD_A1 

	return 1;
}
