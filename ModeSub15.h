#if !defined(AFX_MODESUB15_H__861B1CC1_DAD0_4CAD_8E92_1207E1FE2028__INCLUDED_)
#define AFX_MODESUB15_H__861B1CC1_DAD0_4CAD_8E92_1207E1FE2028__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "A_Define1.h"  //son220705


//son Recepe Open R enum: IDC_COMBO02
//    SysInfo05.m_nCon 의 value로 사용.
enum eRECIPE_OPEN_R_COMBO02 {   //son220705 begin : Recipe Open R 콤보박스 중국어 ohm 표시 기능 추가.
                                 // <Open >
    RECP_OPENR_5ohm        =  1, // *
    RECP_OPENR_10ohm       =  2, // 10 ohm
    RECP_OPENR_20ohm       =  3, // 20 ohm
    RECP_OPENR_30ohm       =  4, // 30 ohm
    RECP_OPENR_40ohm       =  5, // 40 ohm
    RECP_OPENR_50ohm       =  6, // 50 ohm
    RECP_OPENR_60ohm       =  7, // 60 ohm
    RECP_OPENR_70ohm       =  8, // 70 ohm
    RECP_OPENR_80ohm       =  9, // 80 ohm
    RECP_OPENR_90ohm       = 10, // 90 ohm
    RECP_OPENR_100ohm      = 11, // 100 ohm
    RECP_OPENR_200ohm      = 12, // 200 ohm
    RECP_OPENR_300ohm      = 13, // 300 ohm
    RECP_OPENR_400ohm      = 14, // 400 ohm
    RECP_OPENR_500ohm      = 15, // 500 ohm
    RECP_OPENR_600ohm      = 16, // 600 ohm
    RECP_OPENR_700ohm      = 17, // 700 ohm
    RECP_OPENR_800ohm      = 18, // 800 ohm
    RECP_OPENR_900ohm      = 19, // 900 ohm
    RECP_OPENR_1Kohm       = 20, // 1 kohm
    RECP_OPENR_10Kohm      = 21, // 10 kohm (10V)
    RECP_OPENR_30Kohm      = 22, // 30 kohm (10V)
    RECP_OPENR_50Kohm      = 23, // 50 kohm (10V)
    RECP_OPENR_100Kohm     = 24, // 100 kohm (10V)
    RECP_OPENR_USER     = 25, // *User
    RECP_OPENR_END      = RECP_OPENR_USER,
    RECP_OPENR_NUM      = 26
};  //son220705 end




//son230904_2 begin  : moved from A_Define.h

#define MAX_OPEN_USER_R     100000      //son220705 100000 ohm (100 Kohm) SysInfo05.m_nConU 에서 사용. 
#define MAX_USHORT_USER_R   200         //son220922 200 Kohm
#define MAX_SHORT_USER_R    2000        //son220922 2 Mohm


//son220922 GTS, 베트남 영풍, 베트남 나래전자, 국내 영풍, BH, 는 기존대로 HR 20Mohm 까지 Recepe 설정 가능하고
//              다른 고객사는 HR 10Mohm 까지만 설정가능
//son230904 #if (defined(__YOUNPUNG__CUSTOMER) || defined(__YP_VIET_CUSTOMER) \
//        || defined(__BH__CUSTOMER) || defined(__GTS__CUSTOMER))
#ifdef __HR_20M_        //son230904
    #define MAX_HR_USER_R       20000       //son220922 20 Mohm 
#else
    #define MAX_HR_USER_R       10000       //son220922 10 Mohm 
#endif
 

#if (defined(__LEAK_500M_))    //sylee230823
    #define MAX_LEAK_USER_R     500000    //500M    //sylee230823

#elif (defined(__LEAK_300M_))  //sylee230823
	#define MAX_LEAK_USER_R     300000    //300M    //sylee230823

#elif (defined(__LEAK_200M_))  //sylee221005
    #define MAX_LEAK_USER_R     200000    //200M    (GTS는 CCTC만 Leak 200 Mohm  설정)

#else
    #define MAX_LEAK_USER_R     100000    //100M
#endif 

//#define     MAX_4W_R            200000   //sylee230420 moved to A_Define1.h
//son230904_2 end

//yklee240624 ACE500 BH Spark 250V [
#ifdef __BH__SPARK_DECISION_BY_230828
#define MAX_SPARK_V_SET         250         
#else
#define MAX_SPARK_V_SET         90         //son220922, //hdpark230223, 250 ->90
#endif

//yklee240624 ACE500 BH Spark 250V ]
#define MAX_SPARK_I_SET         20         //son220922 //hdpark230223, 100 -> 20 

//son221226 begin : Recipe Open V 콤보박스 

enum eRECIPE_OPEN_V_COMBO22 {   
    RECP_OPENV_0       = 0,   // <Volt.>
    RECP_OPENV_1       = 1,   // *
    RECP_OPENV_2_10V   = 2,   // 10V
    RECP_OPENV_3       = 3,   // *
    RECP_OPENV_4_30V   = 4,   // 30V
    RECP_OPENV_5_40V   = 5,   // 40V
    RECP_OPENV_6_50V   = 6,   // 50V*
    RECP_OPENV_7_60V   = 7,   // 60V
    RECP_OPENV_8_70V   = 8,   // 70V
    RECP_OPENV_9_80V   = 9,   // 80V
    RECP_OPENV_10_90V  = 10,  // 90V
    RECP_OPENV_11_100V = 11,  // 100V*
    RECP_OPENV_12_110V = 12,  // 110V
    RECP_OPENV_13_120V = 13,  // 120V
    RECP_OPENV_14_130V = 14,  // 130V
    RECP_OPENV_15_140V = 15,  // 140V
    RECP_OPENV_16_150V = 16,  // 150V*
    RECP_OPENV_17_160V = 17,  // 160V
    RECP_OPENV_18_170V = 18,  // 170V
    RECP_OPENV_19_180V = 19,  // 180V
    RECP_OPENV_20_190V = 20,  // 190V
    RECP_OPENV_21_200V = 21,  // 200V*
    RECP_OPENV_22_210V = 22,  // 210V
    RECP_OPENV_23_220V = 23,  // 220V
    RECP_OPENV_24_230V = 24,  // 230V
    RECP_OPENV_25_240V = 25,  // 240V
    RECP_OPENV_26_250V = 26,  // 250V*
    RECP_OPENV_NUM = 27

};  


//son IDC_COMBO22 (Recipe Open V), SysInfo05.m_nOpenV 관련해서 사용.
class CList_Recipe_Open_V   
{
public:
    CString     strList[RECP_OPENV_NUM]; 
    int         nOpenR[RECP_OPENV_NUM];

public:
    CList_Recipe_Open_V()      // Constructor: 생성자
    {
        strList[RECP_OPENV_0]          = "<Volt.>";    
        strList[RECP_OPENV_1]          = "*";          
        strList[RECP_OPENV_2_10V]      = "10V";        
        strList[RECP_OPENV_3]          = "*";          

//son230711 영풍 SST에 30V, 40V, 50V 풀어주라는 사장님 지시사항. 
//          son221226 기능 코멘트처리하고 30V,40V, 50V 무조건 보여주게 수정함.
// #ifdef __YP_VIET_CUSTOMER
//        strList[RECP_OPENV_4_30V]      = "30V";   //son221226 영풍베트남일 때에만 OPEN 30V, 40V, 50V를 눈에 보이게 함. 
//        strList[RECP_OPENV_5_40V]      = "40V";   //      30V, 40V, 50V는 현재 dRec(SysSet21_ACE400.ini)에서 default 10mA가 아니라
//        strList[RECP_OPENV_6_50V]      = "50V*";  //      3mA여야 하는 HW 상황을 최대한 고객눈에 안보이게 가리기 위함. 
//#else                                             // 전류세분화 기능이 제공되지 않고 OPEN 50V를 꼭 써야만 하는 영풍베트남만 30V, 40V, 50V 가 
//        strList[RECP_OPENV_4_30V]      = "*";     // OPEN Voltage 리스트에 보이고 다른 고객에게는 '*'로 보인다.  
//        strList[RECP_OPENV_5_40V]      = "*";      
//        strList[RECP_OPENV_6_50V]      = "*";          
//#endif
        strList[RECP_OPENV_4_30V]      = "30V";     //son230711 영풍 SST, 대만 MST, 제이텍등에서 50V 사용요청함.
        strList[RECP_OPENV_5_40V]      = "40V";     //   30V, 40V, 50V를 보여 주되 10mA를 사용하면 Open Fail 나올 수 있으므로 (사장님 지시사항)
        strList[RECP_OPENV_6_50V]      = "50V*";    //   30V, 40V, 50V는  dRec 상관없이 강제로 3mA로 설정하는 기능으로 대체하기로 함. 
        strList[RECP_OPENV_7_60V]      = "60V";        
        strList[RECP_OPENV_8_70V]      = "70V";        
        strList[RECP_OPENV_9_80V]      = "80V";        

        strList[RECP_OPENV_10_90V]     = "90V";         
        strList[RECP_OPENV_11_100V]    = "100V*";       
        strList[RECP_OPENV_12_110V]    = "110V";        
        strList[RECP_OPENV_13_120V]    = "120V";        
        strList[RECP_OPENV_14_130V]    = "130V";        

        strList[RECP_OPENV_15_140V]    = "140V";        
        strList[RECP_OPENV_16_150V]    = "150V*";        
        strList[RECP_OPENV_17_160V]    = "160V";        
        strList[RECP_OPENV_18_170V]    = "170V";        
        strList[RECP_OPENV_19_180V]    = "180V";        

        strList[RECP_OPENV_20_190V]    = "190V";        
        strList[RECP_OPENV_21_200V]    = "200V*";        
        strList[RECP_OPENV_22_210V]    = "210V";        
        strList[RECP_OPENV_23_220V]    = "220V";        
        strList[RECP_OPENV_24_230V]    = "230V";        

        strList[RECP_OPENV_25_240V]    = "240V";        
        strList[RECP_OPENV_26_250V]    = "250V*";        
    }
};
//son221226 end


//son IDC_COMBO02 (Recipe Open R), SysInfo05.m_nCon 관련해서 사용.
class CList_Recipe_Open_R   //son220705 begin : Recipe Open R 콤보박스 중국어 ohm 표시 기능 추가.
{
public:
    CString     strList[RECP_OPENR_NUM]; 
    int         nOpenR[RECP_OPENR_NUM];

public:
    CList_Recipe_Open_R()      // Constructor: 생성자
    {
#ifdef __GTS__CUSTOMER  //son230905 GTS만 중국OS Ω로 표시하고 MST는 ohm 으로 표시. sylee230822과 맞추어서 수정함.
//son230905 #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        strList[0]                     = "<Open>";         // 0           
        strList[RECP_OPENR_5ohm ]      = "*";              // 1 
        strList[RECP_OPENR_10ohm]      = "10 ┯";          // 2   
        strList[RECP_OPENR_20ohm]      = "20 ┯";          // 3   
        strList[RECP_OPENR_30ohm]      = "30 ┯";          // 4   

        strList[RECP_OPENR_40ohm]      = "40 ┯";          // 5   
        strList[RECP_OPENR_50ohm]      = "50 ┯";          // 6   
        strList[RECP_OPENR_60ohm]      = "60 ┯";          // 7   
        strList[RECP_OPENR_70ohm]      = "70 ┯";          // 8   
        strList[RECP_OPENR_80ohm]      = "80 ┯";          // 9   
\
        strList[RECP_OPENR_90ohm ]     = "90 ┯";          // 10   
        strList[RECP_OPENR_100ohm]     = "100 ┯";         // 11   
        strList[RECP_OPENR_200ohm]     = "200 ┯";         // 12   
        strList[RECP_OPENR_300ohm]     = "300 ┯";         // 13   
        strList[RECP_OPENR_400ohm]     = "400 ┯";         // 14   

        strList[RECP_OPENR_500ohm]     = "500 ┯";         // 15   
        strList[RECP_OPENR_600ohm]     = "600 ┯";         // 16   
        strList[RECP_OPENR_700ohm]     = "700 ┯";         // 17   
        strList[RECP_OPENR_800ohm]     = "800 ┯";         // 18   
        strList[RECP_OPENR_900ohm]     = "900 ┯";         // 19   

        strList[RECP_OPENR_1Kohm  ]    = "1 K┯";          // 20    
        strList[RECP_OPENR_10Kohm ]    = "10 K┯ (10V)";   // 21    
        strList[RECP_OPENR_30Kohm ]    = "30 K┯ (10V)";   // 22    
        strList[RECP_OPENR_50Kohm ]    = "50 K┯ (10V)";   // 23    
        strList[RECP_OPENR_100Kohm]    = "100 K┯ (10V)";  // 24    

        strList[RECP_OPENR_USER]       = "*User";          // 25    

#else
        strList[0]                     = "<Open>";         // 0           
        strList[RECP_OPENR_5ohm ]      = "*";              // 1 
        strList[RECP_OPENR_10ohm]      = "10 Ω";          // 2   
        strList[RECP_OPENR_20ohm]      = "20 Ω";          // 3   
        strList[RECP_OPENR_30ohm]      = "30 Ω";          // 4   

        strList[RECP_OPENR_40ohm]      = "40 Ω";          // 5   
        strList[RECP_OPENR_50ohm]      = "50 Ω";          // 6   
        strList[RECP_OPENR_60ohm]      = "60 Ω";          // 7   
        strList[RECP_OPENR_70ohm]      = "70 Ω";          // 8   
        strList[RECP_OPENR_80ohm]      = "80 Ω";          // 9   

        strList[RECP_OPENR_90ohm ]     = "90 Ω";          // 10   
        strList[RECP_OPENR_100ohm]     = "100 Ω";         // 11   
        strList[RECP_OPENR_200ohm]     = "200 Ω";         // 12   
        strList[RECP_OPENR_300ohm]     = "300 Ω";         // 13   
        strList[RECP_OPENR_400ohm]     = "400 Ω";         // 14   

        strList[RECP_OPENR_500ohm]     = "500 Ω";         // 15   
        strList[RECP_OPENR_600ohm]     = "600 Ω";         // 16   
        strList[RECP_OPENR_700ohm]     = "700 Ω";         // 17   
        strList[RECP_OPENR_800ohm]     = "800 Ω";         // 18   
        strList[RECP_OPENR_900ohm]     = "900 Ω";         // 19   

        strList[RECP_OPENR_1Kohm  ]    = "1 KΩ";          // 20    
        strList[RECP_OPENR_10Kohm ]    = "10 KΩ (10V)";   // 21    
        strList[RECP_OPENR_30Kohm ]    = "30 KΩ (10V)";   // 22    
        strList[RECP_OPENR_50Kohm ]    = "50 KΩ (10V)";   // 23    
        strList[RECP_OPENR_100Kohm]    = "100 KΩ (10V)";  // 24    

        strList[RECP_OPENR_USER]       = "*User";          // 25    
#endif

#ifdef OHM
        strList[0]                     = "<Open>";         // 0           
        strList[RECP_OPENR_5ohm ]      = "*";              // 1 
        strList[RECP_OPENR_10ohm]      = "10 ohm";         // 2   
        strList[RECP_OPENR_20ohm]      = "20 ohm";         // 3   
        strList[RECP_OPENR_30ohm]      = "30 ohm";         // 4   

        strList[RECP_OPENR_40ohm]      = "40 ohm";         // 5   
        strList[RECP_OPENR_50ohm]      = "50 ohm";         // 6   
        strList[RECP_OPENR_60ohm]      = "60 ohm";         // 7   
        strList[RECP_OPENR_70ohm]      = "70 ohm";         // 8   
        strList[RECP_OPENR_80ohm]      = "80 ohm";         // 9   

        strList[RECP_OPENR_90ohm ]     = "90 ohm";         // 10   
        strList[RECP_OPENR_100ohm]     = "100 ohm";        // 11   
        strList[RECP_OPENR_200ohm]     = "200 ohm";        // 12   
        strList[RECP_OPENR_300ohm]     = "300 ohm";        // 13   
        strList[RECP_OPENR_400ohm]     = "400 ohm";        // 14   

        strList[RECP_OPENR_500ohm]     = "500 ohm";        // 15   
        strList[RECP_OPENR_600ohm]     = "600 ohm";        // 16   
        strList[RECP_OPENR_700ohm]     = "700 ohm";        // 17   
        strList[RECP_OPENR_800ohm]     = "800 ohm";        // 18   
        strList[RECP_OPENR_900ohm]     = "900 ohm";        // 19   

        strList[RECP_OPENR_1Kohm  ]    = "1 Kohm";          // 20    
        strList[RECP_OPENR_10Kohm ]    = "10 Kohm (10V)";   // 21    
        strList[RECP_OPENR_30Kohm ]    = "30 Kohm (10V)";   // 22    
        strList[RECP_OPENR_50Kohm ]    = "50 Kohm (10V)";   // 23    
        strList[RECP_OPENR_100Kohm]    = "100 Kohm (10V)";  // 24    

        strList[RECP_OPENR_USER]       = "*User";          // 25    
#endif

        nOpenR[RECP_OPENR_5ohm ]       = 5;        // 1
        nOpenR[RECP_OPENR_10ohm]       = 10;       // 2
        nOpenR[RECP_OPENR_20ohm]       = 20;       // 3
        nOpenR[RECP_OPENR_30ohm]       = 30;       // 4

        nOpenR[RECP_OPENR_40ohm]       = 40;       // 5
        nOpenR[RECP_OPENR_50ohm]       = 50;       // 6
        nOpenR[RECP_OPENR_60ohm]       = 60;       // 7
        nOpenR[RECP_OPENR_70ohm]       = 70;       // 8
        nOpenR[RECP_OPENR_80ohm]       = 80;       // 9

        nOpenR[RECP_OPENR_90ohm ]      = 90;       // 10
        nOpenR[RECP_OPENR_100ohm]      = 100;      // 11
        nOpenR[RECP_OPENR_200ohm]      = 200;      // 12
        nOpenR[RECP_OPENR_300ohm]      = 300;      // 13
        nOpenR[RECP_OPENR_400ohm]      = 400;      // 14

        nOpenR[RECP_OPENR_500ohm]      = 500;      // 15
        nOpenR[RECP_OPENR_600ohm]      = 600;      // 16
        nOpenR[RECP_OPENR_700ohm]      = 700;      // 17
        nOpenR[RECP_OPENR_800ohm]      = 800;      // 18
        nOpenR[RECP_OPENR_900ohm]      = 900;      // 19

        nOpenR[RECP_OPENR_1Kohm  ]     = 1000;     // 20
        nOpenR[RECP_OPENR_10Kohm ]     = 10000;    // 21
        nOpenR[RECP_OPENR_30Kohm ]     = 30000;    // 22
        nOpenR[RECP_OPENR_50Kohm ]     = 50000;    // 23
        nOpenR[RECP_OPENR_100Kohm]     = 100000;   // 24

        nOpenR[RECP_OPENR_USER]        = 0;        // 25
    }
    
}; //son220705 end

//son Recepe uShort R enum: IDC_COMBO09
//    SysInfo05.m_nShortR 의 value로 사용.
enum eRECIPE_uSHORT_R_COMBO09 {   //son220705 begin : Recipe uShort R 콤보박스 중국어 ohm 표시 기능 추가.
                               // <uShort>
    RECP_uShortR_10Kohm   = 1, // 10 K    
    RECP_uShortR_50Kohm   = 2, // 50 K    
    RECP_uShortR_100Kohm  = 3, // 100 K   
    RECP_uShortR_USER     = 4, // *User
    RECP_uShortR_NUM      = 5

};  //son220705 end


//son IDC_COMBO09 (Recipe uShort R), SysInfo05.m_nShortR 관련해서 사용.
class CList_Recipe_uShortR   //son220705 begin : Recipe Open R 콤보박스 중국어 ohm 표시 기능 추가.
{
public:
    CString     strList[RECP_uShortR_NUM];    
    int         nR[RECP_uShortR_NUM];

public:
    CList_Recipe_uShortR()      // Constructor: 생성자
    {
#ifdef __GTS__CUSTOMER //son230905 GTS만 중국OS Ω로 표시하고 MST는 ohm 으로 표시. sylee230822과 맞추어서 수정함.
//son230905 #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        strList[0]                       = "<uShort>";        // 0           
        strList[RECP_uShortR_10Kohm ]    = "10 K┯";          // 1 
        strList[RECP_uShortR_50Kohm ]    = "50 K┯";          // 2   
        strList[RECP_uShortR_100Kohm]    = "100 K┯";         // 3   
        strList[RECP_uShortR_USER   ]    = "*User";           // 4    

#else
        strList[0]                       = "<uShort>";        // 0           
        strList[RECP_uShortR_10Kohm ]    = "10 KΩ";          // 1 
        strList[RECP_uShortR_50Kohm ]    = "50 KΩ";          // 2   
        strList[RECP_uShortR_100Kohm]    = "100 KΩ";         // 3   
        strList[RECP_uShortR_USER   ]    = "*User";           // 4    
#endif

#ifdef OHM
        strList[0]                       = "<uShort>";        // 0           
        strList[RECP_uShortR_10Kohm ]    = "10 Kohm";         // 1 
        strList[RECP_uShortR_50Kohm ]    = "50 Kohm";         // 2   
        strList[RECP_uShortR_100Kohm]    = "100 Kohm";        // 3   
        strList[RECP_uShortR_USER   ]    = "*User";           // 4    
#endif

        nR[RECP_uShortR_10Kohm ]  = 10000;        // 1
        nR[RECP_uShortR_50Kohm ]  = 50000;        // 2
        nR[RECP_uShortR_100Kohm]  = 100000;       // 3
        nR[RECP_uShortR_USER   ]  = 0;            // 4
    }
    
}; //son220705 end


//son Recepe ShortR  enum: IDC_COMBO10
//    SysInfo05.m_nShortR2 의 value로 사용.
enum eRECIPE_LSHORT_R_COMBO10 {   //son220705 begin : Recipe Short R 콤보박스 중국어 ohm 표시 기능 추가.
                                // <Short>  
    RECP_LShortR_500ohm   = 1,  // *           
    RECP_LShortR_1Kohm    = 2,  // 1k          
    RECP_LShortR_5Kohm    = 3,  // 5k          
    RECP_LShortR_10Kohm   = 4,  // 10k         

    RECP_LShortR_30Kohm   = 5,  // 30k         
    RECP_LShortR_50Kohm   = 6,  // 50k         
    RECP_LShortR_100Kohm  = 7,  // 100k        
    RECP_LShortR_200Kohm  = 8,  // 200k        
    RECP_LShortR_300Kohm  = 9,  // 300k        

    RECP_LShortR_400Kohm  = 10, // 400k        
    RECP_LShortR_500Kohm  = 11, // 500k        
    RECP_LShortR_600Kohm  = 12, // 600k        
    RECP_LShortR_700Kohm  = 13, // 700k        
    RECP_LShortR_800Kohm  = 14, // 800k        

    RECP_LShortR_900Kohm  = 15, // 900k        
    RECP_LShortR_1Mohm    = 16, // 1M          
    RECP_LShortR_2Mohm    = 17, // *           
    RECP_LShortR_3Mohm    = 18, // *           
    RECP_LShortR_4Mohm    = 19, // *           

    RECP_LShortR_5Mohm    = 20, // *           
    RECP_LShortR_6Mohm    = 21, // *           
    RECP_LShortR_7Mohm    = 22, // *           
    RECP_LShortR_8Mohm    = 23, // *           
    RECP_LShortR_9Mohm    = 24, // *           

    RECP_LShortR_10Mohm   = 25, // *           
    RECP_LShortR_USER     = 26, // *User    
    RECP_LShortR_NUM      = 27

};  //son220705 end


//son IDC_COMBO10 (Recipe Short R), SysInfo05.m_nShortR2 관련해서 사용.
class CList_Recipe_LShortR   //son220705 begin : Recipe Open R 콤보박스 중국어 ohm 표시 기능 추가.
{
public:
    CString     strList[RECP_LShortR_NUM];    
    int         nR[RECP_LShortR_NUM];

public:
    CList_Recipe_LShortR()      // Constructor: 생성자
    {
#ifdef __GTS__CUSTOMER //son230905 GTS만 중국OS Ω로 표시하고 MST는 ohm 으로 표시. sylee230822과 맞추어서 수정함.
//son230905 #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        strList[0]                       = "<Short>";         // 0           
        strList[RECP_LShortR_500ohm]     = "*";               // 1 
        strList[RECP_LShortR_1Kohm ]     = "1 K┯";           // 2   
        strList[RECP_LShortR_5Kohm ]     = "5 K┯";           // 3   
        strList[RECP_LShortR_10Kohm]     = "10 K┯";          // 4    

        strList[RECP_LShortR_30Kohm ]    = "30 K┯";          // 5    
        strList[RECP_LShortR_50Kohm ]    = "50 K┯";          // 6    
        strList[RECP_LShortR_100Kohm]    = "100 K┯";         // 7    
        strList[RECP_LShortR_200Kohm]    = "200 K┯";         // 8    
        strList[RECP_LShortR_300Kohm]    = "300 K┯";         // 9    

        strList[RECP_LShortR_400Kohm]    = "400 K┯";         // 10    
        strList[RECP_LShortR_500Kohm]    = "500 K┯";         // 11    
        strList[RECP_LShortR_600Kohm]    = "600 K┯";         // 12    
        strList[RECP_LShortR_700Kohm]    = "700 K┯";         // 13    
        strList[RECP_LShortR_800Kohm]    = "800 K┯";         // 14    

        strList[RECP_LShortR_900Kohm]    = "900 K┯";         // 15    
        strList[RECP_LShortR_1Mohm  ]    = "1 M┯";           // 16    
#else
        strList[0]                       = "<Short>";         // 0           
        strList[RECP_LShortR_500ohm ]     = "*";               // 1 
        strList[RECP_LShortR_1Kohm ]     = "1 KΩ";           // 2   
        strList[RECP_LShortR_5Kohm ]     = "5 KΩ";           // 3   
        strList[RECP_LShortR_10Kohm]     = "10 KΩ";          // 4    

        strList[RECP_LShortR_30Kohm ]    = "30 KΩ";          // 5    
        strList[RECP_LShortR_50Kohm ]    = "50 KΩ";          // 6    
        strList[RECP_LShortR_100Kohm]    = "100 KΩ";         // 7    
        strList[RECP_LShortR_200Kohm]    = "200 KΩ";         // 8    
        strList[RECP_LShortR_300Kohm]    = "300 KΩ";         // 9    

        strList[RECP_LShortR_400Kohm]    = "400 KΩ";         // 10    
        strList[RECP_LShortR_500Kohm]    = "500 KΩ";         // 11    
        strList[RECP_LShortR_600Kohm]    = "600 KΩ";         // 12    
        strList[RECP_LShortR_700Kohm]    = "700 KΩ";         // 13    
        strList[RECP_LShortR_800Kohm]    = "800 KΩ";         // 14    

        strList[RECP_LShortR_900Kohm]    = "900 KΩ";         // 15    
        strList[RECP_LShortR_1Mohm  ]    = "1 MΩ";           // 16    
#endif

#ifdef OHM

        strList[0]                       = "<Short>";         // 0           
        strList[RECP_LShortR_500ohm ]    = "*";               // 1 
        strList[RECP_LShortR_1Kohm ]     = "1 Kohm";          // 2   
        strList[RECP_LShortR_5Kohm ]     = "5 Kohm";          // 3   
        strList[RECP_LShortR_10Kohm]     = "10 Kohm";         // 4    

        strList[RECP_LShortR_30Kohm ]    = "30 Kohm";         // 5    
        strList[RECP_LShortR_50Kohm ]    = "50 Kohm";         // 6    
        strList[RECP_LShortR_100Kohm]    = "100 Kohm";        // 7    
        strList[RECP_LShortR_200Kohm]    = "200 Kohm";        // 8    
        strList[RECP_LShortR_300Kohm]    = "300 Kohm";        // 9    

        strList[RECP_LShortR_400Kohm]    = "400 Kohm";        // 10    
        strList[RECP_LShortR_500Kohm]    = "500 Kohm";        // 11    
        strList[RECP_LShortR_600Kohm]    = "600 Kohm";        // 12    
        strList[RECP_LShortR_700Kohm]    = "700 Kohm";        // 13    
        strList[RECP_LShortR_800Kohm]    = "800 Kohm";        // 14    

        strList[RECP_LShortR_900Kohm]    = "900 Kohm";        // 15    
        strList[RECP_LShortR_1Mohm  ]    = "1 Mohm";          // 16    
#endif
        strList[RECP_LShortR_2Mohm  ]    = "*";               // 17    
        strList[RECP_LShortR_3Mohm  ]    = "*";               // 18    
        strList[RECP_LShortR_4Mohm  ]    = "*";               // 19    

        strList[RECP_LShortR_5Mohm  ]    = "*";               // 20    
        strList[RECP_LShortR_6Mohm  ]    = "*";               // 21    
        strList[RECP_LShortR_7Mohm  ]    = "*";               // 22    
        strList[RECP_LShortR_8Mohm  ]    = "*";               // 23    
        strList[RECP_LShortR_9Mohm  ]    = "*";               // 24    

        strList[RECP_LShortR_10Mohm]     = "*";               // 25    
        strList[RECP_LShortR_USER]       = "*User";           // 26    

        //-----------------
        nR[RECP_LShortR_500ohm ]         = 500;         // 1
        nR[RECP_LShortR_1Kohm ]          = 1000;        // 2
        nR[RECP_LShortR_5Kohm ]          = 5000;        // 3
        nR[RECP_LShortR_10Kohm]          = 10000;       // 4

        nR[RECP_LShortR_30Kohm ]         = 30000;       // 5
        nR[RECP_LShortR_50Kohm ]         = 50000;       // 6
        nR[RECP_LShortR_100Kohm]         = 100000;      // 7
        nR[RECP_LShortR_200Kohm]         = 200000;      // 8
        nR[RECP_LShortR_300Kohm]         = 300000;      // 9

        nR[RECP_LShortR_400Kohm]         = 400000;      // 10
        nR[RECP_LShortR_500Kohm]         = 500000;      // 11
        nR[RECP_LShortR_600Kohm]         = 600000;      // 12
        nR[RECP_LShortR_700Kohm]         = 700000;      // 13
        nR[RECP_LShortR_800Kohm]         = 800000;      // 14

        nR[RECP_LShortR_900Kohm]         = 900000;      // 15
        nR[RECP_LShortR_1Mohm  ]         = 1000000;     // 16
        nR[RECP_LShortR_2Mohm  ]         = 2000000;     // 17
        nR[RECP_LShortR_3Mohm  ]         = 3000000;     // 18
        nR[RECP_LShortR_4Mohm  ]         = 4000000;     // 19

        nR[RECP_LShortR_5Mohm  ]         = 5000000;     // 20
        nR[RECP_LShortR_6Mohm  ]         = 6000000;     // 21
        nR[RECP_LShortR_7Mohm  ]         = 7000000;     // 22
        nR[RECP_LShortR_8Mohm  ]         = 8000000;     // 23
        nR[RECP_LShortR_9Mohm  ]         = 9000000;     // 24

        nR[RECP_LShortR_10Mohm ]         = 10000000;    // 25
        nR[RECP_LShortR_USER   ]         = 0;           // 26 
    }
    
}; //son220705 end





//son Recepe HR Short R enum: IDC_COMBO08
//    SysInfo05.m_nHR 의 value로 사용.
enum eRECIPE_HRSHORT_R_COMBO10 {   //son220705 begin : Recipe HR Short R 콤보박스 중국어 ohm 표시 기능 추가.
                                 // <HR> 
    RECP_HRShortR_100Kohm   = 1, // 100k    
    RECP_HRShortR_500Kohm   = 2, // 500k    
    RECP_HRShortR_1Mohm     = 3, // 1M      
    RECP_HRShortR_5Mohm     = 4, // 5M
    RECP_HRShortR_10Mohm    = 5, // 10M
    RECP_HRShortR_USER      = 6, // *User
    RECP_HRShortR_NUM       = 7

};  //son220705 end


//son IDC_COMBO08 (Recipe HR Short R), SysInfo05.m_nHR 관련해서 사용.
class CList_Recipe_HRShortR   //son220705 begin : Recipe HR Short R 콤보박스 중국어 ohm 표시 기능 추가.
{
public:
    CString     strList[RECP_HRShortR_NUM];    
    int         nR[RECP_HRShortR_NUM];

public:
    CList_Recipe_HRShortR()      // Constructor: 생성자
    {
#ifdef __GTS__CUSTOMER //son230905 GTS만 중국OS Ω로 표시하고 MST는 ohm 으로 표시. sylee230822과 맞추어서 수정함.
//son230905 #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        strList[0]                        = "<HR>";           // 0           
        strList[RECP_HRShortR_100Kohm]    = "100 K┯";        // 1 
        strList[RECP_HRShortR_500Kohm]    = "500 K┯";        // 2   
        strList[RECP_HRShortR_1Mohm  ]    = "1 M┯";          // 3   
        strList[RECP_HRShortR_5Mohm  ]    = "5 M┯";          // 4   
        strList[RECP_HRShortR_10Mohm ]    = "10 M┯";         // 5   
        strList[RECP_HRShortR_USER   ]    = "*User";          // 6    

#else
        strList[0]                        = "<HR>";           // 0           
        strList[RECP_HRShortR_100Kohm]    = "100 KΩ";        // 1 
        strList[RECP_HRShortR_500Kohm]    = "500 KΩ";        // 2   
        strList[RECP_HRShortR_1Mohm  ]    = "1 MΩ";          // 3   
        strList[RECP_HRShortR_5Mohm  ]    = "5 MΩ";          // 4   
        strList[RECP_HRShortR_10Mohm ]    = "10 MΩ";         // 5   
        strList[RECP_HRShortR_USER   ]    = "*User";          // 6    
#endif

#ifdef OHM
        strList[0]                        = "<HR>";           // 0           
        strList[RECP_HRShortR_100Kohm]    = "100 Kohm";        // 1 
        strList[RECP_HRShortR_500Kohm]    = "500 Kohm";        // 2   
        strList[RECP_HRShortR_1Mohm  ]    = "1 Mohm";          // 3   
        strList[RECP_HRShortR_5Mohm  ]    = "5 Mohm";          // 4   
        strList[RECP_HRShortR_10Mohm ]    = "10 Mohm";         // 5   
        strList[RECP_HRShortR_USER   ]    = "*User";          // 6    
#endif

        nR[RECP_HRShortR_100Kohm]  = 100000;        // 1
        nR[RECP_HRShortR_500Kohm]  = 500000;        // 2
        nR[RECP_HRShortR_1Mohm  ]  = 1000000;       // 3
        nR[RECP_HRShortR_5Mohm  ]  = 5000000;       // 4
        nR[RECP_HRShortR_10Mohm ]  = 10000000;      // 5
        nR[RECP_HRShortR_USER   ]  = 0;             // 6
    }
    
}; //son220705 end




//son Recepe Leak R enum: IDC_COMBO03, IDC_COMBO43
//    SysInfo05.m_nHv1R 의 value로 사용.
enum eRECIPE_LEAK_R_COMBO03 {   //son220705 begin : Recipe LEAK1 R 콤보박스 중국어 ohm 표시 기능 추가.
                              //<Leak>       
    RECP_LEAK_1Mohm     = 1, //1M              
    RECP_LEAK_10Mohm    = 2, //10M             
    RECP_LEAK_20Mohm    = 3, //20M             
    RECP_LEAK_50Mohm    = 4, //50M  (>=100V)
    RECP_LEAK_100Mohm   = 5, //100M (>=150V)
    RECP_LEAK_USER      = 6, //*User        
    RECP_LEAK_NUM       = 7

};  //son220705 end


//son IDC_COMBO03, IDC_COMBO43 (Recipe Leak1, Leak2 R), 
//    SysInfo05.m_nHv1R,  SysInfo05.m_nHv2R 관련해서 사용.
class CList_Recipe_LEAK_R   //son220705 begin : Recipe Leak1, Leak2 R 콤보박스 중국어 ohm 표시 기능 추가.
{
public:
    CString     strList[RECP_LEAK_NUM];    
    int         nR[RECP_LEAK_NUM];

public:
    CList_Recipe_LEAK_R()      // Constructor: 생성자
    {
#ifdef __GTS__CUSTOMER //son230905 GTS만 중국OS Ω로 표시하고 MST는 ohm 으로 표시. sylee230822과 맞추어서 수정함.
//son230905 #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        strList[0]                    = "<Leak>";          // 0           
        strList[RECP_LEAK_1Mohm  ]    = "1 M┯";           // 1 
        strList[RECP_LEAK_10Mohm ]    = "10 M┯";          // 2   
        strList[RECP_LEAK_20Mohm ]    = "20 M┯";          // 3   
        strList[RECP_LEAK_50Mohm ]    = "50 M┯ (>=100V)"; // 4   
        strList[RECP_LEAK_100Mohm]    = "100 M┯(>=150V)"; // 5   
        strList[RECP_LEAK_USER   ]    = "*User";           // 6    

#else
        strList[0]                    = "<Leak>";          // 0           
        strList[RECP_LEAK_1Mohm  ]    = "1 MΩ";           // 1 
        strList[RECP_LEAK_10Mohm ]    = "10 MΩ";          // 2   
        strList[RECP_LEAK_20Mohm ]    = "20 MΩ";          // 3   
        strList[RECP_LEAK_50Mohm ]    = "50 MΩ (>=100V)"; // 4   
        strList[RECP_LEAK_100Mohm]    = "100 MΩ(>=150V)"; // 5   
        strList[RECP_LEAK_USER   ]    = "*User";           // 6    
#endif

#ifdef OHM
        strList[0]                    = "<Leak>";           // 0           
        strList[RECP_LEAK_1Mohm  ]    = "1 Mohm";           // 1 
        strList[RECP_LEAK_10Mohm ]    = "10 Mohm";          // 2   
        strList[RECP_LEAK_20Mohm ]    = "20 Mohm";          // 3   
        strList[RECP_LEAK_50Mohm ]    = "50 Mohm (>=100V)"; // 4   
        strList[RECP_LEAK_100Mohm]    = "100 Mohm(>=150V)"; // 5   
        strList[RECP_LEAK_USER   ]    = "*User";            // 6    
#endif

        nR[RECP_LEAK_1Mohm  ]  = 1000000;        // 1
        nR[RECP_LEAK_10Mohm ]  = 10000000;       // 2
        nR[RECP_LEAK_20Mohm ]  = 20000000;       // 3
        nR[RECP_LEAK_50Mohm ]  = 50000000;       // 4
        nR[RECP_LEAK_100Mohm]  = 100000000;      // 5
        nR[RECP_LEAK_USER   ]  = 0;              // 6
    }
    
}; //son220705 end



/////////////////////////////////////////////////////////////////////////////
// CModeSub15 dialog

class CModeSub15 : public CDialog
{
// Construction
public:
    CModeSub15(CWnd* pParent = NULL);   // standard constructor
 
    int  OnSaveInterlock(); //sylee140929

    //  CSliderCtrl  m_slider1;
    void A_Language();//sylee151028

    CScrollBar  m_scrollbar2;
    CScrollBar  m_scrollbar3;
    CScrollBar  m_scrollbar4;
    CScrollBar  m_scrollbar5;
    CScrollBar  m_scrollbar6;


//sylee170622-2 CComboBox   m_combo1;
    CComboBox   m_combo2;
    CComboBox   m_combo3;
    CComboBox   m_combo4; 
    CComboBox   m_combo6; 
    CComboBox   m_combo8;
    CComboBox   m_combo9;
    CComboBox   m_combo10;
    CComboBox   m_combo22;
    CComboBox   m_combo23;
    CComboBox   m_combo24;
    CComboBox   m_combo25;
    CComboBox   m_combo26;
    CComboBox   m_combo27;
    CComboBox   m_combo28;
    CComboBox   m_combo29;
    CComboBox   m_combo30;
    CComboBox   m_combo31;
    CComboBox   m_combo32;
    CComboBox   m_combo33;
    CComboBox   m_combo34;
    CComboBox   m_combo35;
    CComboBox   m_combo36;
    CComboBox   m_combo37;
    CComboBox   m_combo38;
//sylee170622-2     CComboBox   m_combo39;
    CComboBox   m_combo40;
    
    CComboBox   m_combo20;//sylee140923 
    CComboBox   m_combo41;//sylee140923
    CComboBox   m_combo43;//sylee140923
    CComboBox   m_combo44; // hdpark230223 
    CComboBox   m_combo45; // hdpark230223 
    CComboBox   m_combo46; // hdpark230331
    CComboBox   m_combo47; // hdpark230331

 
    CLabelControl   m_Label1;
    CLabelControl   m_Label2;
    CLabelControl   m_Label3; 
    CLabelControl   m_Label5;
    CLabelControl   m_Label6;
    CLabelControl   m_Label7;

    CEdit   m_edit1;
    CEdit   m_edit2;   //sylee181016-1
    CEdit   m_edit3;
    CEdit   m_edit5;

    CEdit   m_edit02;
 
    CEdit   m_edit47;   //SYLEE200709 NEW_SPARK
    CEdit   m_edit55;
    CEdit   m_edit60;   //sylee170915-1
    CEdit   m_edit66;	// hdpark230223

    CEdit   m_edit57;
    CEdit   m_edit101;
    CEdit   m_edit125;//sylee140923
 

    CButton m_check1;
    CButton m_check2;
    CButton m_check3;  //sylee180331
    CButton m_check4;  //sylee181016-1
    CButton m_check11; //sylee200511-4W정도개선
    CButton m_check5;  //son220613 AC 기능
  

    CLabelControl   m_Label1001;
    CLabelControl   m_Label1002;
    CLabelControl   m_Label1003;
    CLabelControl   m_Label1004;
//  CLabelControl   m_Label1005;
    CLabelControl   m_Label1006;    
    CLabelControl   m_Label1007;
    CLabelControl   m_Label1008;
    CLabelControl   m_Label1009;
    CLabelControl   m_Label1010;
    CLabelControl   m_Label1011;
    CLabelControl   m_Label1012;
    CLabelControl   m_Label1013;
    CLabelControl   m_Label1014;
    CLabelControl   m_Label1015;
    CLabelControl   m_Label1016;
    CLabelControl   m_Label1017;
    CLabelControl   m_Label1018;    
    CLabelControl   m_Label1019;
    CLabelControl   m_Label1020;
    CLabelControl   m_Label1021;
    CLabelControl   m_Label1022;
    CLabelControl   m_Label1023;
//  CLabelControl   m_Label1024;
//  CLabelControl   m_Label1025;
    CLabelControl   m_Label1026;
    CLabelControl   m_Label1047;	// hdpark230412
    CLabelControl   m_Label1046;	// hdpark230412
    CLabelControl   m_Label1027;
    CLabelControl   m_Label1028;
    CLabelControl   m_Label1029;
//son220613 CLabelControl   m_Label1030;    
//  CLabelControl   m_Label1031;
    CLabelControl   m_Label1032;
    CLabelControl   m_LabelCurrent;     //son220630 전류 Label 추가 
     
    CCommandButton m_button1001;
    CCommandButton m_button1002;
    CCommandButton m_button1003;





// Dialog Data
    //{{AFX_DATA(CModeSub15)
#if (defined(__RECIPE_LEAK_MORE1) )//sylee231016
    enum { IDD = IDD_MODE_SUB15_RECIPE2 };//sylee231013
#else
    enum { IDD = IDD_MODE_SUB15_RECIPE };
#endif	
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CModeSub15)

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    //}}AFX_VIRTUAL

// Implementation


protected:

    // Generated message map functions
    //{{AFX_MSG(CModeSub15)
        afx_msg void OnCheck1();      
        afx_msg void OnCheck2();
        afx_msg void OnCheck3();//sylee180331-1
        afx_msg void OnCheck4();//sylee181016-1
        afx_msg void OnClickOk(); 
        afx_msg void OnSave();
        afx_msg void Onload();
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnCheck11(); //sylee200511-4W정도개선
        afx_msg void OnCheck5();    //son220613
	    afx_msg void OnSelchangeCombo22();  //son221227
	//}}AFX_MSG

    DECLARE_EVENTSINK_MAP()
    DECLARE_MESSAGE_MAP()

public:
    //son231106_2 삭제. void changeContent_OpenCurrentCombo28_HighVolt();   //son221227
    void changeContent_OpenCurrentCombo28_default();   //son221227
            
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESUB15_H__861B1CC1_DAD0_4CAD_8E92_1207E1FE2028__INCLUDED_)
