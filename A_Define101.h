#ifndef __A_DEFINE101_H__
#define __A_DEFINE101_H__

//A_Define1.h   //sylee161126

#undef __DUT16__        
#undef __DUT32__

//============================================
//#define Def_SwitchMaxPin 	4096*2+1   	// 8K
#define Def_SwitchMaxPin  	16384+1  	// 16K
//============================================

//#define Def_4WMax1  5020  //only china  170803
//son231107 #define Def_4WMax1  3010   // BH   YONGPUNG
//son231107 #define dSetPieceTot1   Def_4WMax1  //son231107 dSetPieceTot1 ->  MAX_4W_NET +1
//son231107 #define dSet4WTot1      Def_4WMax1  //son231107 dSet4WTot1 ->  MAX_4W_NET +1
#define MAX_4W_NET  3000    //son231107 Def_4WMax1 -> MAX_4W_NET +1

//======================================================
//#define Def_4WSampleCount1 310   
//#define Def_4WSampleCount1 1010  //only young pung 
//#define Def_4WSampleCount1   1010  
#define MAX_4W_SAMPLE       1000  //son210928 Def_4WSampleCount1 =>  MAX_4W_SAMPLE 

//======================================================
//#define DefDutMax2 4  //sylee180810-1
#define MAX_DUT   4  //sylee180810-1 //son220824_2 DefDutMax2 -> MAX_DUT

//#define DefDutMax  DefDutMax2+1 //sylee180810-1
#define MAX_DUT1  (MAX_DUT+1) //sylee180810-1 //son220824_2 DefDutMax -> MAX_DUT1

//#define Def_MaxPiece1 211  //BH YOUNGPUNG
#define DEF_MAX_PIECE1 301 //only china  170803  NANOSYS    //son220823_6 
#define DEF_MAX_BLOCK1 501//sylee170327  //151->201         //son220823_6

//===================================================
#define DEF_MAX_PIN1  Def_SwitchMaxPin  //8200  //son220126 dSetPinTot1  -> DEF_MAX_PIN1
#define DEF_MAX_PIN2  Def_SwitchMaxPin  //8200  //son220126 dSetPinTot2  -> DEF_MAX_PIN2
    
#define Def_MaxPiece3 51  

 
//======================================================


#define dSheetRetestUse  1    //sylee171020   1=use  0 =disable

 
 
#endif 
