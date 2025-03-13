#ifndef __A_DEFINE1_H__
#define __A_DEFINE1_H__



//#################################################################
#ifdef _DEBUG
//#define _PC_TEST_ 
//#define _WITHOUT_MAIN_   //son240901 
//#define _TEST_OPEN_ALL_PASS_
//#define _TEST_4W_ALL_PASS_
#endif


//sylee181015 #define DEF_CARD_A1  //sylee181015  CONTEC CARD


//--------------------------
// Customer Define 
//--------------------------
//#########################################################################################################################
//son 고객사 전용 define 옵션은 하나만 ON 해야 한다. 
//    고객사 전용 define 옵션을 쓰지 않으면 Common(공용) 버전이 된다.
//
//#define VERSION "v1.6.0"    //son241113  //son241211  Project Settings > C/C++ Preprocessor definitions: 으로 이동함 

// ACE500:
//#define __COMMON__CUSTOMER        // Common Customer, WillTech
//#define __GTS__CUSTOMER           //(GS) GTS
//#define __CCTC__CUSTOMER          //(CCTC) CCTC               //sylee220923
//#define __CHINA_UNITECH__CUSTOMER //(CHUNI)China Unitiec      //sylee230430 GTS와 China Unitech 분리	
//#define __MST__CUSTOMER           //(MT) 대만 TTF(MST)              //son220228
//#define __JTECH__CUSTOMER         //(JT) 제이텍,조이텍        //SYLEE201012   ONLY
//#define __LM__CUSTOMER            //(LM) LM디지털             //son220314 내부적으론 나노시스
//#define __SIMTECH__CUSTOMER       //(SIM)말레이시아 Simmtech  //son220425  
//#define __SST__CUSTOMER           //(ST) 구미 SST             //SYLEE201012  ONLY     //son220401 ACE500으로 원상복구
//#define __NANOSYS__CUSTOMER       //(NS) 나노시스, 리노정밀, 삼성전기 //SYLEE201203-1   ONLYfmf 
#define __BH__CUSTOMER            //(BH) BH, 대전 유씨텍      //son220922 //yskim 241218 BHVT define 제거(저항표시 ohm으로 통일하기 위해)
//#define __KCC__CUSTOMER           //(KCC) KCC in CnB
//#define __KCC_FCBGA__CUSTOMER     //(KCC) KCC QUAD AT-7Q in CnB 장비(for FCBGA) 용  
//#define __BNTECH__CUSTOMER        //BNTECH   //sylee241129 
     
//ACE400:
//#define __YOUNPUNG__CUSTOMER      //(YP)   국내영풍               //SYLEE201207   ONLY
//#define __YP_VIET_CUSTOMER        //(YPVT) 베트남영풍, 베트남나래전자         //son210824
//#define __SST__CUSTOMER__ACE400	//(ST400) 대우 SST, ACE400 Spark Current Detection	//hdpark230331
//#define __JTECH__CUSTOMER__ACE400	//(JT)   제이텍용 ACE400 Contec 버전 (문동주과장요청)  //son240118 

//##############################################################################################################################

//----------------------------
// Main 장비 정보
//----------------------------
// FPC 반자동장비     HDI(Index)장비       Package 장비        FCBGA (Flip Chip Ball Grid Array) 장비
// F-550                H-30                 C-30                AT-7Q (Quad FCBGA)
//                      H-30W                C-50                AT-7U (Unit FCBGA)
//                                                               HP-10Q Hybrid (FCBGA + Package) 

//son241011 KCC에 Multi Current 기능등 C&B에 제공된 기능을 모두 동일하게 제공하기 위함
//son241114 KCC외에 FCBGA QUAD(AT-7Q 장비) 전용으로 KCC_P3 Customer가 추가됨. 
//          CnB는 최종고객이 아니므로 CnB 관련 기능은 CNB_FEATURE  define으로 변경하고 
//          KCC와 KCC_P3에 모두 CNB_FEATURE를 부여함.


#if defined(__KCC__CUSTOMER) || defined(__KCC_FCBGA__CUSTOMER)
    #define CNB_FEATURE
#endif


#ifdef __KCC_FCBGA__CUSTOMER  //son241114
    #define __NANOSYS__CUSTOMER_4WAVG  //sylee241108   C&B   KCC    AT-7Q 장비

    #define __QUAD_LEAK1_LEAK2__    //son231122  sylee23080* 관련 QUAD 장비용 기능을 __QUAD_LEAK1_LEAK2__ define 옵션처리
                                    //          Leak1, Leak2 중 큰 값으로 동일하게 설정
#endif
 




#ifdef __CHINA_UNITECH__CUSTOMER //sylee230430    
    #define __GTS__CUSTOMER     //son230512
    #define __GTS_4W_SPCIALNET__    //son2211107 4W Special Net 표시 기능 define.			
    #define __NEW_4W_230420     //son230721 위준명과장요청으로 유니텍에도 4W정도개선사항  적용    // yskim241205 CHUNI 4wire Sampling문제로 인해 주석처리 해제 
    #define __4WCSV_SHEETNO_BARCODE  //son231124 son211112 Unitech 요구사항. 4W_Setup_A.txt에 step, BarCode 정보 추가 요청
                                     //    관련 기능을 __4WCSV_SHEETNO_BARCODE define 옵션으로 변경하고 
                                     //    __CHINA_UNITECH__CUSTOMER에만 사용하도록 함.
#endif


#ifdef __CCTC__CUSTOMER
    //son220923 CCTC Customer는 내부적으로 GTS와 기능을 동일하게 갖되 CCTC만의 추가 기능을 갖는다.
    //          CCTC에만 Leak 200 Mohm  기능 UI로만 제공(sylee220923) 관련하여 추가 됨. 
    #define __GTS__CUSTOMER 
    #define __RECIPE_LEAK_MORE1   //sylee231013  CCTC 전용기능
#endif

#ifdef __MST__CUSTOMER
    //son220228 MST 고객사는 GTS 기능을 동일하게 갖되 MST 만의 추가기능을 가질 수 있다.
    //          m_button_NewBdlSet 신규 기능 때문에 도입함.
    //son220913 #define __GTS__CUSTOMER //son220913 GTS 고객사와 MST 고객사 분리 
#endif

	

// hdpark230331 begin, sst only
#if (defined(__SST__CUSTOMER__ACE400))
	#define __YOUNPUNG__CUSTOMER
	#define SPARK_CURRENT_DETECTION				
#endif
//	hdpark230331 end

#if (defined(__JTECH__CUSTOMER__ACE400))        //son240118 제이텍용 ACE400 Contec 버전 요청 by 문동주과장
    #define __JTECH__CUSTOMER
#endif
 

#if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
    //son220629 #define OHM     //sylee210324   //son220629 GTS 중국어 윈도우 Ω 표시 추가 관련하여 삭제함
    #define __GTS_4WLOG_PIECE__ //son200804 GTS 유니텍 요구사항. 4WLog에 Piece 정보 추가
    #define __DUT32__           //son210514 GTS 요구사항. 32 DUT
#endif

 
//#if (defined(__SIMTECH__CUSTOMER) || defined(__MST__CUSTOMER))  //sylee230822 MST는 OHM으로 다시 수정.
#if (defined(__BH__CUSTOMER) || defined(__SIMTECH__CUSTOMER) || defined(__MST__CUSTOMER))  //son240308 베트남 BH도 OHM으로 수정
    #define OHM     //son220425 말레이시아 simmtech Ω 표시가 깨지는 현상관련 보완
#endif

  
#if (defined(__YOUNPUNG__CUSTOMER)  ||  defined(__SST__CUSTOMER) )   //sylee230125    //son230201
//#if (defined(__YOUNPUNG__CUSTOMER) || defined (__YP_VIET_CUSTOMER) ||  defined(__SST__CUSTOMER) )   //sylee230125
     #define __AUTO_COUNT_DISPLAY_DISABLE__   //sylee230125
#endif 


#if ( defined(__CCTC__CUSTOMER) )    //sylee231201
   #define __DISPLAY_ERRORFILE_STEP  //sylee231201
#endif 



//sylee221005 
//son230418 __GTS__CUSTOMER 에 __LEAK_200M 기능추가 
//son230828 __BH__CUSTOMER  에 __LEAK_200M 기능추가 (영업요청)
//son230904 __MST__CUSTOMER 에 __LEAK_200M 추가. 사장님 지시사항.
//son240703 JTECH에 __LEAK_200M 추가. 김영식이사님 요청 
//son240905 C&B용으로 CNB_FEATURE 에 __LEAK_200M 추가 (문동주과장 요청) 
#if (defined(__YOUNPUNG__CUSTOMER) || defined (__YP_VIET_CUSTOMER) \
        || defined(__CCTC__CUSTOMER) || defined(__GTS__CUSTOMER) \
        || defined(__BH__CUSTOMER) || defined(__MST__CUSTOMER) || defined(__JTECH__CUSTOMER) \
        || defined(CNB_FEATURE))
   #define __LEAK_200M_
#endif

//son240314 China 유니텍에 Leak 300M 제공 (for 24/3/27일 출하준비)
#if (defined(__CHINA_UNITECH__CUSTOMER))    
    #undef  __LEAK_200M_    //son240314
    #define __LEAK_300M_//sylee230823    MAINBOARD 같이 교체해야 함.
#endif

//#define __LEAK_500M_//sylee230823    MAINBOARD 같이 교체해야 함.


//son220922 GTS, 베트남 영풍, 베트남 나래전자, 국내 영풍, BH, 는 기존대로 HR 20 Mohm 까지 Recipe 설정 가능하고
//              다른 고객사는 HR 10 Mohm 까지만 설정가능
//son230904 MST에 __HR_20M 추가. 사장님 지시사항.
//son240703 JTECH에 __HR_20M 추가. 김영식이사님 요청 
//son240905 C&B용으로 CNB_FEATURE 에 __HR_20M_ 추가 
#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) \
        || defined(__BH__CUSTOMER) || defined(__GTS__CUSTOMER) \
        || defined(__MST__CUSTOMER) || defined(__JTECH__CUSTOMER) \
        || defined(CNB_FEATURE))
    #define __HR_20M_ 
#endif

#if (defined(__MST__CUSTOMER))  //son220607 신규 RJigReadType.txt 미반영시 에러 있을 수 있어서 MST에만 4W 200 ohm 적용. 
    #define __4W_200ohm__       //son230904_2 __4W_200ohm__ define을 추가.  A_Define1.h를 복잡도를  덜고 
                                //          기능별로 define 분산을 위해 MAX_4W_R_COUNT는 ModeSer13.h로 이동.
#endif


#ifdef __GTS__CUSTOMER
//son240827 GTS, CCTC, 중국 Unitech 모두 ARun201_4W_Old_230420(int nPStep1) 버전으로 원상복구,  Retest 문제 관련 
    #define __NEW_4W_230420     //sylee230420 	 //NEW  4W  AUTO AUTOSAMPLING // yskim241205 CHUNI 4wire sampling문제로 인해 주석처리 해제  
                                //son230808  위준명과장 요청으로 모든 GTS 고객사에 신규 4W (E&E) 측정기능 적용.
                                
    #define __SAVE_4W_SAMPLING_TIME //son231117 4W pin이 많아서 sampling이 너무 오래걸리는 경우 sampling을 
                                    //    빨리 끝낼 수 있도록 GTS에만 적용한 기능      
                                    
#if 0 //son241114 현재 __SAVE_4W_SAMPL_TIME_V2 은 미구현 
//    #define __SAVE_4W_SAMPL_TIME_V2  
                       //son231124 4W sampling을 net별로 카운트해서 sampling 시간을 단축하는 기능
                         // 모든 net에 NG가 없는 케이스를 300회 수집하는 것보다는 각 net별로 NG없는 300회
                         // 를 수집하는 것이 시간이 더 단축될 것임.  
                         // Sampling NG는 마이너스 값으로 표시해서 4W_Setup_A.txt를 생성할때  제외해야 한다. 
                         
    #ifdef __SAVE_4W_SAMPL_TIME_V2          //son231124 
        // Sampling NG를 마이너스로 표시해야 하므로 __SAVE_4W_SAMPL_TIME_V2 ON 이면 __SAVE_4W_SAMPLING_TIME 기능을  off 한다.
        // __SAVE_4W_SAMPL_TIME_V2 이면 각 sample 별 net의 실제 sheetno가 다를 수 있으므로 __4WCSV_SHEETNO_BARCODE 기능도 Off 한다.
        #undef __SAVE_4W_SAMPLING_TIME      //son231124
        #undef __4WCSV_SHEETNO_BARCODE      //son231124
    #endif
#endif

#endif


//son220609 리노정밀(나노시스)만 전류세분화 기능이 실제 동작함. 
//son220609 GTS는 전류 세분화 기능의 UI만 있고 실제로는  사용 못함.
//son230331 #if (!defined(__JTECH__CUSTOMER) && !defined (__GTS__CUSTOMER) && !defined(__MST__CUSTOMER) && !defined(__LM__CUSTOMER) && !defined(__SST__CUSTOMER)  && !defined (__SIMTECH__CUSTOMER) && !defined(__YOUNPUNG__CUSTOMER) && !defined(__YP_VIET_CUSTOMER))
//son240905 C&B용으로 CNB_FEATURE 에 __MULTI_CURRENT__ 추가 

//son241114 #if (defined(__NANOSYS__CUSTOMER) || defined(__BH__CUSTOMER) || defined(CNB_FEATURE))
    #define __MULTI_CURRENT__       //son241114 multi current 기능 전고객 default 제공 결정. confirmed by 이상윤부장.
//son241114 #endif

//son231018 나노시스 요구사항. 원격 4W Ref 초기화 기능관련 2BYTE DIO
#if (defined(__NANOSYS__CUSTOMER))		
    #define __2BYTE_DIO__			//son231018

#endif


//son __DUT16__, __DUT32__ 모두 undefined 상태이면 default인  DUT4로 동작함.
#ifndef __DUT32__           //son210520
    //son200508-4WNET10000:  
    //  vector 기능 도입하면서 DUT16도 Piece 갯수를 200개로 통일. 300개로 변경 (CCTC 요구사항) 
    //  DUT16 이미지 하나로 통일하기 위해 __DUT16, __OPVECTOR__를 default로 코드에 추가함.
    #define __DUT16__       //SYLEE200622      //undefine=>  DUT4
#endif

#define __OPVECTOR__    //son for 4WNet10000

//#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER)|| defined(__JTECH__CUSTOMER) ) //son240717 김영필이사님요청으로 제이텍 임시추가했다 삭제 
#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) )  //son210416    //son231205 | => || 로 오류 수정
    #define __YOUNPUNG__CUSTOMER_4W_2WCHECK  //SYLEE201207   ONLY
#endif

//son210824     //son231205 | => || 로 오류 수정
#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) || defined(__JTECH__CUSTOMER__ACE400)) 
    //son ACE550도 ACE500도 ACE1000도 아니면 default인  ACE400으로 설정됨.
    //son210623 영풍 요청으로 영풍은 ACE400으로 설정     
#else
    #define __ACE500__      //SYLEE200622       //son210318: SPARK 통합버전 ACE500으로 정함(사장님 지시사항)
    //#define __ACE550__    //son201118         // ACE550은 추후 업그레이드 용도로 놓아둠. 당분간 미사용.
    //#define __ACE1000__     //son240807  for CCTC Plan B (ACE500코드로 ACE1000 계측기 지원)
#endif


#define __NEW_SPARK__ 

#ifdef	__BH__CUSTOMER //yklee240621 BH 230828 Spark SW 판단로직 적용
	#define __BH__SPARK_DECISION_BY_230828
#endif

#ifdef __JTECH__CUSTOMER //yklee240802 JTech 핀등록 Spark 판단 로직 내부 상이함 적용 
    #define __JTECH__SPARK_DECISION_BY_230828
#endif


// hdpark20240308 begin
#ifdef __BH__CUSTOMER //yskim 241218 BHVT define 제거(저항표시 ohm으로 통일하기 위해)
    #define __SPARK_JUDGE_BH__
#elif (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER))
    #define __SPARK_JUDGE_YP__
#else
    #define __SPARK_JUDGE_NEW__
#endif
// hdpark20240308 end

//##############################################################################################################################
//##############################################################################################################################

//--------------------------
// Include file 
//--------------------------
#include "A_Define103.h"    //sylee170828   //china DUT16 H-30 Index 장비. 24K, GTS //son190704-4WDUT16
#include "A_Define2.h"//sylee181015  card  
#include "Globals.h"


//--------------------------
// Common Define 
//--------------------------
//SYLEE180415  #define DEFINE_FAKEOPEN   //SYLEE180415   
//SYLEE181031 RAY #define DEF_4W_PIN_DELAY1 2000 //1000
//SYLEE181031 RAY #define DEF_4W_PIN_DELAY2 2000 //1000
//SYLEE181031 RAY #define DEF_4W_PIN_DELAY3 1000 //300
  
#define DEF_4W_PIN_DELAY1 200  //SYLEE181031 RAY    //son220126 
#define DEF_4W_PIN_DELAY2 500  //SYLEE181031 RAY    //SYLEE200323  1500->500        //son220126
 

#define DEF_4W_PIN_DELAY3 100  //SYLEE181031  RAY       //son220126
#define SPARK_VB_READ       //hdpark220511 spark  

//son 4W cal Jig는 mohm 단위,   OPEN, SHORT cal Jig는 ohm 단위. 
#define     MAX_4W_R            200000   //son 200 ohm //sylee230415  //sylee230420 moved from ModeSub18.h


#endif

 

