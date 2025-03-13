 
#include "stdafx.h"  
#include "UniDAQ.h"
#include "DioD96.h"




#include "A_Define2.h"//sylee181015


 	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1  
       #pragma comment(lib,"UniDAQ.lib")

     #endif  //#ifdef DEF_CARD_A1 


 CDIO_D96::CDIO_D96()
 { 
 	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
		  CardD96_Open1();
	#endif  //#ifdef DEF_CARD_A1 

 }

 CDIO_D96::~CDIO_D96()
 { 
	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
		  CardD96_Close1();
	#endif  //#ifdef DEF_CARD_A1     
 } 
 
short CDIO_D96::CardD96_Open1()
{     


#ifndef DEF_CARD_A1  //sylee170328

	//------------------------------------
	// Init the resource for  Driver
	
	//son Ixud_DriverInit()�� UniDAQ ���� ��� �ڵ��߿� ���� ���� ȣ��Ǿ�� �Ѵ�. 
	//son system�� ���ҽ��� üũ�ؼ� ��ġ�� DAQ board ������ ȹ��. 
	wRtn=Ixud_DriverInit(&wTotalBoards);
	if(wRtn!=Ixud_NoErr)
	{
		printf("\n nDriver Init Error PIO 96 (%d)", wRtn);
		return wRtn;
	} 


	//-------------------
	// Config the Port


//	wBoardNo=0;
//	wOutPortNo=0; 

	//son ������Ʈ�� I/O ��带 �����Ѵ�. ����� I/O ��Ʈ�� ������. 
	wRtn=Ixud_SetDIOModes32(0,          // BoardNo   		0: First board
	                        0x0FFF );   // DioModeMask		CN1 Port 0,1,2 	CN2 Port 3,4,5
											// 					CN3 Port 6,7,8  CN4 Port 9,10,11
											// 					BitVal=0: Input,  BitVal=1: Output
											//      => Port0~ Port11���� 1(0x0fff)�̹Ƿ� (12 port * 8 = 96 bit) 
											//         PIO_D96�� DioModeMask�� 96 bit ��� Output�������� ������ ����. 
											//         (refer from 'ICP DAS UNIDAQ' Driver DLL User Manual')
	for( int i=0; i<=11; i++){
	   wRtn=Ixud_WriteDO(0,i,0x00);
	}
#endif  //#ifdef DEF_CARD_A1     

	return 1;
}



short CDIO_D96::CardD96_Close1()
{ 
	#ifdef DEF_CARD_A1  //sylee170328

	#else  //#ifdef DEF_CARD_A1   
	//if( m_bCardID3 != INVALID_CARD_ID )

	   wRtn=Ixud_WriteDO(0,0,0x00);
	   if(wRtn==0){
 	      wRtn=Ixud_DriverClose();
		  if(wRtn==0){
	//sylee180607-1		AfxMessageBox("   D96 Card close OK !.", MB_OK);
		  }
	   }else{
		   	AfxMessageBox("   D96 Card close fail .  open ???? .", MB_OK);
	   }

	#endif  //#ifdef DEF_CARD_A1     
 	 return 1;
}


 
