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
//son ���� ���� define �ɼ��� �ϳ��� ON �ؾ� �Ѵ�. 
//    ���� ���� define �ɼ��� ���� ������ Common(����) ������ �ȴ�.
//
//#define VERSION "v1.6.0"    //son241113  //son241211  Project Settings > C/C++ Preprocessor definitions: ���� �̵��� 

// ACE500:
//#define __COMMON__CUSTOMER        // Common Customer, WillTech
//#define __GTS__CUSTOMER           //(GS) GTS
//#define __CCTC__CUSTOMER          //(CCTC) CCTC               //sylee220923
//#define __CHINA_UNITECH__CUSTOMER //(CHUNI)China Unitiec      //sylee230430 GTS�� China Unitech �и�	
//#define __MST__CUSTOMER           //(MT) �븸 TTF(MST)              //son220228
//#define __JTECH__CUSTOMER         //(JT) ������,������        //SYLEE201012   ONLY
//#define __LM__CUSTOMER            //(LM) LM������             //son220314 ���������� ����ý�
//#define __SIMTECH__CUSTOMER       //(SIM)�����̽þ� Simmtech  //son220425  
//#define __SST__CUSTOMER           //(ST) ���� SST             //SYLEE201012  ONLY     //son220401 ACE500���� ���󺹱�
//#define __NANOSYS__CUSTOMER       //(NS) ����ý�, ��������, �Ｚ���� //SYLEE201203-1   ONLYfmf 
#define __BH__CUSTOMER            //(BH) BH, ���� ������      //son220922 //yskim 241218 BHVT define ����(����ǥ�� ohm���� �����ϱ� ����)
//#define __KCC__CUSTOMER           //(KCC) KCC in CnB
//#define __KCC_FCBGA__CUSTOMER     //(KCC) KCC QUAD AT-7Q in CnB ���(for FCBGA) ��  
//#define __BNTECH__CUSTOMER        //BNTECH   //sylee241129 
     
//ACE400:
//#define __YOUNPUNG__CUSTOMER      //(YP)   ������ǳ               //SYLEE201207   ONLY
//#define __YP_VIET_CUSTOMER        //(YPVT) ��Ʈ����ǳ, ��Ʈ����������         //son210824
//#define __SST__CUSTOMER__ACE400	//(ST400) ��� SST, ACE400 Spark Current Detection	//hdpark230331
//#define __JTECH__CUSTOMER__ACE400	//(JT)   �����ؿ� ACE400 Contec ���� (�����ְ����û)  //son240118 

//##############################################################################################################################

//----------------------------
// Main ��� ����
//----------------------------
// FPC ���ڵ����     HDI(Index)���       Package ���        FCBGA (Flip Chip Ball Grid Array) ���
// F-550                H-30                 C-30                AT-7Q (Quad FCBGA)
//                      H-30W                C-50                AT-7U (Unit FCBGA)
//                                                               HP-10Q Hybrid (FCBGA + Package) 

//son241011 KCC�� Multi Current ��ɵ� C&B�� ������ ����� ��� �����ϰ� �����ϱ� ����
//son241114 KCC�ܿ� FCBGA QUAD(AT-7Q ���) �������� KCC_P3 Customer�� �߰���. 
//          CnB�� �������� �ƴϹǷ� CnB ���� ����� CNB_FEATURE  define���� �����ϰ� 
//          KCC�� KCC_P3�� ��� CNB_FEATURE�� �ο���.


#if defined(__KCC__CUSTOMER) || defined(__KCC_FCBGA__CUSTOMER)
    #define CNB_FEATURE
#endif


#ifdef __KCC_FCBGA__CUSTOMER  //son241114
    #define __NANOSYS__CUSTOMER_4WAVG  //sylee241108   C&B   KCC    AT-7Q ���

    #define __QUAD_LEAK1_LEAK2__    //son231122  sylee23080* ���� QUAD ���� ����� __QUAD_LEAK1_LEAK2__ define �ɼ�ó��
                                    //          Leak1, Leak2 �� ū ������ �����ϰ� ����
#endif
 




#ifdef __CHINA_UNITECH__CUSTOMER //sylee230430    
    #define __GTS__CUSTOMER     //son230512
    #define __GTS_4W_SPCIALNET__    //son2211107 4W Special Net ǥ�� ��� define.			
    #define __NEW_4W_230420     //son230721 ���ظ�����û���� �����ؿ��� 4W������������  ����    // yskim241205 CHUNI 4wire Sampling������ ���� �ּ�ó�� ���� 
    #define __4WCSV_SHEETNO_BARCODE  //son231124 son211112 Unitech �䱸����. 4W_Setup_A.txt�� step, BarCode ���� �߰� ��û
                                     //    ���� ����� __4WCSV_SHEETNO_BARCODE define �ɼ����� �����ϰ� 
                                     //    __CHINA_UNITECH__CUSTOMER���� ����ϵ��� ��.
#endif


#ifdef __CCTC__CUSTOMER
    //son220923 CCTC Customer�� ���������� GTS�� ����� �����ϰ� ���� CCTC���� �߰� ����� ���´�.
    //          CCTC���� Leak 200 Mohm  ��� UI�θ� ����(sylee220923) �����Ͽ� �߰� ��. 
    #define __GTS__CUSTOMER 
    #define __RECIPE_LEAK_MORE1   //sylee231013  CCTC ������
#endif

#ifdef __MST__CUSTOMER
    //son220228 MST ����� GTS ����� �����ϰ� ���� MST ���� �߰������ ���� �� �ִ�.
    //          m_button_NewBdlSet �ű� ��� ������ ������.
    //son220913 #define __GTS__CUSTOMER //son220913 GTS ����� MST ���� �и� 
#endif

	

// hdpark230331 begin, sst only
#if (defined(__SST__CUSTOMER__ACE400))
	#define __YOUNPUNG__CUSTOMER
	#define SPARK_CURRENT_DETECTION				
#endif
//	hdpark230331 end

#if (defined(__JTECH__CUSTOMER__ACE400))        //son240118 �����ؿ� ACE400 Contec ���� ��û by �����ְ���
    #define __JTECH__CUSTOMER
#endif
 

#if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
    //son220629 #define OHM     //sylee210324   //son220629 GTS �߱��� ������ �� ǥ�� �߰� �����Ͽ� ������
    #define __GTS_4WLOG_PIECE__ //son200804 GTS ������ �䱸����. 4WLog�� Piece ���� �߰�
    #define __DUT32__           //son210514 GTS �䱸����. 32 DUT
#endif

 
//#if (defined(__SIMTECH__CUSTOMER) || defined(__MST__CUSTOMER))  //sylee230822 MST�� OHM���� �ٽ� ����.
#if (defined(__BH__CUSTOMER) || defined(__SIMTECH__CUSTOMER) || defined(__MST__CUSTOMER))  //son240308 ��Ʈ�� BH�� OHM���� ����
    #define OHM     //son220425 �����̽þ� simmtech �� ǥ�ð� ������ ������� ����
#endif

  
#if (defined(__YOUNPUNG__CUSTOMER)  ||  defined(__SST__CUSTOMER) )   //sylee230125    //son230201
//#if (defined(__YOUNPUNG__CUSTOMER) || defined (__YP_VIET_CUSTOMER) ||  defined(__SST__CUSTOMER) )   //sylee230125
     #define __AUTO_COUNT_DISPLAY_DISABLE__   //sylee230125
#endif 


#if ( defined(__CCTC__CUSTOMER) )    //sylee231201
   #define __DISPLAY_ERRORFILE_STEP  //sylee231201
#endif 



//sylee221005 
//son230418 __GTS__CUSTOMER �� __LEAK_200M ����߰� 
//son230828 __BH__CUSTOMER  �� __LEAK_200M ����߰� (������û)
//son230904 __MST__CUSTOMER �� __LEAK_200M �߰�. ����� ���û���.
//son240703 JTECH�� __LEAK_200M �߰�. �迵���̻�� ��û 
//son240905 C&B������ CNB_FEATURE �� __LEAK_200M �߰� (�����ְ��� ��û) 
#if (defined(__YOUNPUNG__CUSTOMER) || defined (__YP_VIET_CUSTOMER) \
        || defined(__CCTC__CUSTOMER) || defined(__GTS__CUSTOMER) \
        || defined(__BH__CUSTOMER) || defined(__MST__CUSTOMER) || defined(__JTECH__CUSTOMER) \
        || defined(CNB_FEATURE))
   #define __LEAK_200M_
#endif

//son240314 China �����ؿ� Leak 300M ���� (for 24/3/27�� �����غ�)
#if (defined(__CHINA_UNITECH__CUSTOMER))    
    #undef  __LEAK_200M_    //son240314
    #define __LEAK_300M_//sylee230823    MAINBOARD ���� ��ü�ؾ� ��.
#endif

//#define __LEAK_500M_//sylee230823    MAINBOARD ���� ��ü�ؾ� ��.


//son220922 GTS, ��Ʈ�� ��ǳ, ��Ʈ�� ��������, ���� ��ǳ, BH, �� ������� HR 20 Mohm ���� Recipe ���� �����ϰ�
//              �ٸ� ����� HR 10 Mohm ������ ��������
//son230904 MST�� __HR_20M �߰�. ����� ���û���.
//son240703 JTECH�� __HR_20M �߰�. �迵���̻�� ��û 
//son240905 C&B������ CNB_FEATURE �� __HR_20M_ �߰� 
#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) \
        || defined(__BH__CUSTOMER) || defined(__GTS__CUSTOMER) \
        || defined(__MST__CUSTOMER) || defined(__JTECH__CUSTOMER) \
        || defined(CNB_FEATURE))
    #define __HR_20M_ 
#endif

#if (defined(__MST__CUSTOMER))  //son220607 �ű� RJigReadType.txt �̹ݿ��� ���� ���� �� �־ MST���� 4W 200 ohm ����. 
    #define __4W_200ohm__       //son230904_2 __4W_200ohm__ define�� �߰�.  A_Define1.h�� ���⵵��  ���� 
                                //          ��ɺ��� define �л��� ���� MAX_4W_R_COUNT�� ModeSer13.h�� �̵�.
#endif


#ifdef __GTS__CUSTOMER
//son240827 GTS, CCTC, �߱� Unitech ��� ARun201_4W_Old_230420(int nPStep1) �������� ���󺹱�,  Retest ���� ���� 
    #define __NEW_4W_230420     //sylee230420 	 //NEW  4W  AUTO AUTOSAMPLING // yskim241205 CHUNI 4wire sampling������ ���� �ּ�ó�� ����  
                                //son230808  ���ظ���� ��û���� ��� GTS ���翡 �ű� 4W (E&E) ������� ����.
                                
    #define __SAVE_4W_SAMPLING_TIME //son231117 4W pin�� ���Ƽ� sampling�� �ʹ� �����ɸ��� ��� sampling�� 
                                    //    ���� ���� �� �ֵ��� GTS���� ������ ���      
                                    
#if 0 //son241114 ���� __SAVE_4W_SAMPL_TIME_V2 �� �̱��� 
//    #define __SAVE_4W_SAMPL_TIME_V2  
                       //son231124 4W sampling�� net���� ī��Ʈ�ؼ� sampling �ð��� �����ϴ� ���
                         // ��� net�� NG�� ���� ���̽��� 300ȸ �����ϴ� �ͺ��ٴ� �� net���� NG���� 300ȸ
                         // �� �����ϴ� ���� �ð��� �� ����� ����.  
                         // Sampling NG�� ���̳ʽ� ������ ǥ���ؼ� 4W_Setup_A.txt�� �����Ҷ�  �����ؾ� �Ѵ�. 
                         
    #ifdef __SAVE_4W_SAMPL_TIME_V2          //son231124 
        // Sampling NG�� ���̳ʽ��� ǥ���ؾ� �ϹǷ� __SAVE_4W_SAMPL_TIME_V2 ON �̸� __SAVE_4W_SAMPLING_TIME �����  off �Ѵ�.
        // __SAVE_4W_SAMPL_TIME_V2 �̸� �� sample �� net�� ���� sheetno�� �ٸ� �� �����Ƿ� __4WCSV_SHEETNO_BARCODE ��ɵ� Off �Ѵ�.
        #undef __SAVE_4W_SAMPLING_TIME      //son231124
        #undef __4WCSV_SHEETNO_BARCODE      //son231124
    #endif
#endif

#endif


//son220609 ��������(����ý�)�� ��������ȭ ����� ���� ������. 
//son220609 GTS�� ���� ����ȭ ����� UI�� �ְ� �����δ�  ��� ����.
//son230331 #if (!defined(__JTECH__CUSTOMER) && !defined (__GTS__CUSTOMER) && !defined(__MST__CUSTOMER) && !defined(__LM__CUSTOMER) && !defined(__SST__CUSTOMER)  && !defined (__SIMTECH__CUSTOMER) && !defined(__YOUNPUNG__CUSTOMER) && !defined(__YP_VIET_CUSTOMER))
//son240905 C&B������ CNB_FEATURE �� __MULTI_CURRENT__ �߰� 

//son241114 #if (defined(__NANOSYS__CUSTOMER) || defined(__BH__CUSTOMER) || defined(CNB_FEATURE))
    #define __MULTI_CURRENT__       //son241114 multi current ��� ���� default ���� ����. confirmed by �̻�������.
//son241114 #endif

//son231018 ����ý� �䱸����. ���� 4W Ref �ʱ�ȭ ��ɰ��� 2BYTE DIO
#if (defined(__NANOSYS__CUSTOMER))		
    #define __2BYTE_DIO__			//son231018

#endif


//son __DUT16__, __DUT32__ ��� undefined �����̸� default��  DUT4�� ������.
#ifndef __DUT32__           //son210520
    //son200508-4WNET10000:  
    //  vector ��� �����ϸ鼭 DUT16�� Piece ������ 200���� ����. 300���� ���� (CCTC �䱸����) 
    //  DUT16 �̹��� �ϳ��� �����ϱ� ���� __DUT16, __OPVECTOR__�� default�� �ڵ忡 �߰���.
    #define __DUT16__       //SYLEE200622      //undefine=>  DUT4
#endif

#define __OPVECTOR__    //son for 4WNet10000

//#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER)|| defined(__JTECH__CUSTOMER) ) //son240717 �迵���̻�Կ�û���� ������ �ӽ��߰��ߴ� ���� 
#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) )  //son210416    //son231205 | => || �� ���� ����
    #define __YOUNPUNG__CUSTOMER_4W_2WCHECK  //SYLEE201207   ONLY
#endif

//son210824     //son231205 | => || �� ���� ����
#if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) || defined(__JTECH__CUSTOMER__ACE400)) 
    //son ACE550�� ACE500�� ACE1000�� �ƴϸ� default��  ACE400���� ������.
    //son210623 ��ǳ ��û���� ��ǳ�� ACE400���� ����     
#else
    #define __ACE500__      //SYLEE200622       //son210318: SPARK ���չ��� ACE500���� ����(����� ���û���)
    //#define __ACE550__    //son201118         // ACE550�� ���� ���׷��̵� �뵵�� ���Ƶ�. ��а� �̻��.
    //#define __ACE1000__     //son240807  for CCTC Plan B (ACE500�ڵ�� ACE1000 ������ ����)
#endif


#define __NEW_SPARK__ 

#ifdef	__BH__CUSTOMER //yklee240621 BH 230828 Spark SW �Ǵܷ��� ����
	#define __BH__SPARK_DECISION_BY_230828
#endif

#ifdef __JTECH__CUSTOMER //yklee240802 JTech �ɵ�� Spark �Ǵ� ���� ���� ������ ���� 
    #define __JTECH__SPARK_DECISION_BY_230828
#endif


// hdpark20240308 begin
#ifdef __BH__CUSTOMER //yskim 241218 BHVT define ����(����ǥ�� ohm���� �����ϱ� ����)
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
#include "A_Define103.h"    //sylee170828   //china DUT16 H-30 Index ���. 24K, GTS //son190704-4WDUT16
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

//son 4W cal Jig�� mohm ����,   OPEN, SHORT cal Jig�� ohm ����. 
#define     MAX_4W_R            200000   //son 200 ohm //sylee230415  //sylee230420 moved from ModeSub18.h


#endif

 

