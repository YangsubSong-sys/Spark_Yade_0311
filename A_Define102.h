#ifndef __A_DEFINE102_H__
#define __A_DEFINE102_H__


//*******************************************
//  DUT4 용 define 
//*******************************************


 
//============================================
//#define Def_SwitchMaxPin    	4096*2+1    //8k
#define Def_SwitchMaxPin   		32768+1   // 32K 

#define DEF_MAX_PIN1  Def_SwitchMaxPin  //son220126 dSetPinTot1  -> DEF_MAX_PIN1
#define DEF_MAX_PIN2  Def_SwitchMaxPin  //son220126 dSetPinTot2  -> DEF_MAX_PIN2


//============================================
//son 4W Net 갯수
//#define Def_4WMax1  3010      // BH   YONGPUNG
//#define Def_4WMax1  5020      // only china  170803
//#define Def_4WMax1  6000      //son200403-4WDUT
//son231107 #define Def_4WMax1  10000       //son200420-4WNET10000
//son231107 #define dSetPieceTot1   Def_4WMax1  //son231107 dSetPieceTot1 ->  DEF_MAX_PIN1
//son231107 #define dSet4WTot1      Def_4WMax1  //son231107 dSet4WTot1 ->  MAX_4W_NET +1 
#define MAX_4W_NET  10000    //son231107 Def_4WMax1 -> MAX_4W_NET +1


//============================================
#define		MAX_SQUARE_N		200     //son210401


//======================================================
//son 4W Auto Sampling 갯수
//#define Def_4WSampleCount1 1010   //only young pung 
//#define Def_4WSampleCount1 110      //son200420-4WNET10000 
//#define Def_4WSampleCount1   310  
#define MAX_4W_SAMPLE   300  //son210928 Def_4WSampleCount1 =>  MAX_4W_SAMPLE 

//======================================================
//origin sw  160M

// dut1=1  piece 300  =160M <=====
// dut1=1  piece 100  =99M
// dut1=4  piece 100  =135M  <=====
// dut1=16  piece 100  =275M

//#define DefDutMax2 4  //sylee180810-1
//sylee221005#define MAX_DUT 4  //sylee180810-1 //son220824_2 DefDutMax2 -> MAX_DUT
#define MAX_DUT 4  //sylee180810-1 //son220824_2 DefDutMax2 -> MAX_DUT

//#define DefDutMax  DefDutMax2+1 //sylee180810-1
#define MAX_DUT1  (MAX_DUT+1) //sylee180810-1 //son220824_2 DefDutMax -> MAX_DUT1

//#define DEF_MAX_PIECE1 201  //BH YOUNGPUNG
#define DEF_MAX_PIECE1 301 //only china  170803	//son200806 CCTC 위준명 과장 요청 201->301  //son220823_6
#define DEF_MAX_BLOCK1 501//sylee170327  //151->201 //son220823_6
    
#define Def_MaxPiece3 51   

//======================================================
 
#define dSheetRetestUse 1      //sylee171020   1=use  0 =disable  import.

 
 
#endif
