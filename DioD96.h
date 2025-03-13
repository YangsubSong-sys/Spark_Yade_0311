#ifndef __DIO_D96_H__  //son231205 header guard Ãß°¡ 
#define __DIO_D96_H__

//################################################################
//sylee170328 
// #CARD_CHANGE_A_170328
// OUTPUT 96bit
 
//Project >link>   D64HDLL.lib 
//################################################################

#define INVALID_CARD_ID  0xFF
/*  //sample

 	Ret = Ixud_WriteDOBit(0,3,4,0); //( Board , Port , Bit , Val);
 	if( Ret!=0 ) { AfxMessageBox("  Card96_DOBit(0,3,4,0) Error!!!");	return  ;	}
*/
class CDIO_D96 : public CWnd  
{


public:		 
	CString strErrMsg;
	CString str1;
	short  wCardNum, nErrCode; 
	WORD wTotalBoards;
    WORD wRtn;
//	WORD wBoardNo;
//	WORD wOutPortNo;


public:

	CDIO_D96();
	virtual ~CDIO_D96();

	short CardD96_Open1();
	short CardD96_Close1();


};


extern CDIO_D96  pDIO_D96;


#endif  
