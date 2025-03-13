#ifndef __A_DEFINE103_H__
#define __A_DEFINE103_H__

//*******************************
//  DUT16 용 define
//*******************************
//  Index 장비
//  24K 중국 유니마이크론 
 

//============================================
//#define Def_SwitchMaxPin    	4096*2+1    // 8K
#define Def_SwitchMaxPin   		32768+1   	// 32K

//===================================================
#define DEF_MAX_PIN1  Def_SwitchMaxPin  //32768+1 //son220126 dSetPinTot1  -> DEF_MAX_PIN1
#define DEF_MAX_PIN2  Def_SwitchMaxPin  //32768+1 //son220126 dSetPinTot2  -> DEF_MAX_PIN2


#define		MAX_SQUARE_N		200     //son210401

//============================================
//son 4W Net 갯수
//#define Def_4WMax1  3010      // BH   YONGPUNG
//#define Def_4WMax1  5020      //only china  170803
//#define Def_4WMax1  6000      //son200403-4WDUT
//son231107 #define Def_4WMax1  10000       //son200420-4WNET10000
//son231107 #define dSetPieceTot1   Def_4WMax1  //son231107 dSetPieceTot1 ->  MAX_4W_NET +1
//son231107 #define dSet4WTot1      Def_4WMax1  //son231107 dSet4WTot1 ->  MAX_4W_NET +1 
#define MAX_4W_NET  13000    //son231107 Def_4WMax1 -> MAX_4W_NET +1 /yskim 10000 -> 13000

//======================================================
//son 4W Auto Sampling 갯수
//#define Def_4WSampleCount1 1010  //only young pung 
//#define Def_4WSampleCount1 110      //son200420-4WNET10000 
//#define Def_4WSampleCount1 310   
#define MAX_4W_SAMPLE   300  //son210928 Def_4WSampleCount1 =>  MAX_4W_SAMPLE +1 



//======================================================
#ifdef __DUT32__
    //#define DefDutMax2  32        //son200825 대만 MST 요구사항 16-> 32  
    #define MAX_DUT  32             //son220824_2 DefDutMax2 -> MAX_DUT
#else
    #ifdef __DUT16__
        //#define DefDutMax2  16  
        #define MAX_DUT  16         //son220824_2 DefDutMax2 -> MAX_DUT
    #else
        //#define DefDutMax2  4     //sylee180810-1
        #define MAX_DUT  4          //son220824_2 DefDutMax2 -> MAX_DUT
    #endif
#endif

//#define DefDutMax  DefDutMax2+1 //sylee180810-1   //son 33
#define MAX_DUT1  (MAX_DUT+1)       //sylee180810-1 //son220824_2 DefDutMax(33) -> MAX_DUT1(33)

//#define DEF_MAX_PIECE1 21 //only china  170803    //son220823_6 
//son200507-4WNET10000:  vector 기능 도입하면서 Piece 갯수는 DUT16도 200개로 통일.
//#define DEF_MAX_PIECE1 201  //BH YOUNGPUNG        //son220823_6
//#define DEF_MAX_PIECE1 301 //only china  170803	//son200806 CCTC 위준명 과장 요청 201->301      //son220823_6


//#ifdef __GTS__CUSTOMER
#if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913 
    #define DEF_MAX_PIECE1 501   //son210423 GTS 위준명 과장 최소 300개 요청 301->501       //son220823_6
#else
    #define DEF_MAX_PIECE1 301 //only china  170803	//son200806 CCTC 위준명 과장 요청 201->301      //son220823_6
#endif


#define DEF_MAX_BLOCK1 501//sylee170327  //151->201 //son220823_6



    


#define Def_MaxPiece3 51   
//======================================================
 
#define dSheetRetestUse 1      //sylee171020   1=use  0 =disable  import.

  
 
#endif 
