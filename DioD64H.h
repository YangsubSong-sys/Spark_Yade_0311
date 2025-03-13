#ifndef __DIO_D64H_H__      //son231205 header guard Ãß°¡
#define __DIO_D64H_H__

//################################################################

//sylee170328 
// #CARD_CHANGE_A_170328
// INPUT 32bit+ OUTPUT 32bit
 
//Project >link>   D64HDLL.lib 
//################################################################


class CDIO_D64 : public CWnd
{

public:

	CDIO_D64();
	virtual ~CDIO_D64();

	short CardH64_Open1();
	short CardH64_Close1();

public:	
	
	typedef unsigned long  U32;
	typedef unsigned short U16;
	typedef unsigned char  U8;
	
	U32 dwDOData; 
	U32 dwDIStatus;
	U16 wValue;
	U8 bCardId;
	U8 bCardId2;

	BYTE m_bCardID;
	BYTE m_bCardID2;
	
	CString strErrMsg;
	CString tmpMsg;

	short  wCardNum, nErrCode;

public:  //single command 



public:   // muti command
 

};

extern	CDIO_D64  pDIO_D64;


#endif 
