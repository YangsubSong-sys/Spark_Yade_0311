// FileSysInfo.cpp: implementation of the CFileSysInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "BiOpTroCon.h"
#include "FileSysInfo.h"
#include "ModeSet12.h"
#include "ModeSet13.h"
#include "ModeSet15.h"  //sylee121115-1
#include "ModeSet17.h"  //sylee121115-1
#include "A_Define1.h"//sylee161126
#include "ModeSub18.h"
#include "ModeSub15.h"  //son220705
#include <Time.h>
 
#define Def_MaxPiece2 151

extern BOOL FileExists(CString strFilePath);//sylee160801-6

CSysInfoPath01  SysInfoPath01;
CSysInfoView01  SysInfoView01;
//CSysInfoInOutPath_Hanlder SysInfoInOutPath_Hanlder;     //son201201 장비&계측기 공유폴더 In/Out 폴더 분리 

CSysInfoPieceRange  SysInfoPieceRange;
CSysInfo02  SysInfo02;
CSysInfoBlock  SysInfoBlock;
CSysInfoPiece  SysInfoPiece;
CSysInfo05  SysInfo05;
CSysInfo05  RunInfo05;

CSysInfo19  SysInfo19;
CSysInfo20  SysInfo20;

CModeSet12  ModeSet12;
CModeSet13  ModeSet13;
CModeSet15  ModeSet15;//sylee121115-1
CModeSet17  ModeSet17;

CSysInfoSer11  SysInfoSer11; 
CSysInfoSer12  SysInfoSer12; 
CSysInfoSer13  SysInfoSer13; 

CSysSet12  SysSet12;  //Pin SETUP  
CSysSet13  SysSet13;  //SETUP  
CSysSet15  SysSet15;  //SETUP //SYLEE121115-1 
CSysSet16  SysSet16;  //SETUP //SYLEE121115-1    //estimation
CSysSet17  SysSet17;  //SETUP //SYLEE121115-1    //estimation
CSysSet19  SysSet19;  //SETUP //SYLEE150407    //estimation
CSysSet21  SysSet21;  //SETUP //SYLEE130618
CSysSet22  SysSet22;  //SETUP //SYLEE130618
CSysSet211  SysSet211;  //SETUP //SYLEE160524-1
CFileSysInfo FileSysInfo01;

int nPinAutoCal1[5];//sylee171020

//son piece별 4W Net Data  :  Piece갯수 * 4WNet 갯수 * 4WData 12개+alpha
float ng4WSet[DEF_MAX_PIECE1][MAX_4W_NET +1][NUM_4W_GRID_COL];// piece, pin list  
                        //SYLEE131022//4W  //son220823_6    //son231107 10001 -> MAX_4W_NET +1
                        //son211022 15 -> NUM_4W_GRID_COL로 수정  4W gitTest 잘 안되는 문제관련

extern  int g_nPieceMax1;
int g_CheckFlag1; //sylee121115 
short  OnPinConvertFlag1; //sylee150922-1  //sylee151111 add    //son .CON 파일이 존재하면 1
short  OnPinConvertType1; //sylee150922-1   //son .CON 파일이 의미있으면 1 (total이 1이상, checksum 정상)
extern TCHAR szTSetup2Name[200];  //sylee121022

//son  Calibration data 저장 buffer
//son  'Use=1'인 경우만 SysInfoSer13.m_nData[part][mode][1~3][RList]를  gdCal1[m_nMode1][1~3][RList]에 저장
//      FileSysInfo01.LoadSaveSer13(2, m_nMode1, part) 관련.
double gdCal1[MAX_PART3][MAX_TEST_MODE+2][5][150];  //son220411 cal data 상부, 하부,혼합 3set data를 위해 [MAX_PART3] 추가
                            //son gdCal1[part][m_nMode1][0][0]   : Use=1인 Cal list 개수
                            //son gdCal1[part][m_nMode1][1:CAL_USE    ][RList]  cal enable 1,0 
                            //son gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList]  cal ref realR (ohm)
                            //son gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList] cal Avg rawR (ADC)
                            
//son Rlist의 'Use=1' 상관없이 MODE6_OPEN_10V, MODE6_OPEN_10V, MODE327_LEAK_250V의 Rlist를 온전히 가져온다.
double gdCal2[MAX_PART3][3][5][150];  //hdpark221005 self test 판별 Cal 값(mode 6, 206, 327)
                                    //son gdCal1[part][0] : for mode 6 MODE6_OPEN_10V
                                    //son gdCal1[part][1] : for mode 206 MODE6_OPEN_10V
                                    //son gdCal1[part][2] : for mode 327 MODE327_LEAK_250V
                                    // 나머지 구조는 gcCal1과 같음. 

extern double dRec[MAX_TEST_MODE+2][16];//sylee130630       //son mode, 전압mode별 파라미터
double dEsModel[500][10];//  a,b,c,   //estimation 
//son230222 //short PinMap[7][Def_SwitchMaxPin];//sylee20120617  nSwitchMaxPin 
int PinMap[7][Def_SwitchMaxPin];//son230222 short -> int
            //son PinMap[0][] : jig pin no. 0부터 시작 (사용자 pin=jigPin 번호=swPin, index와 동일)
            //son PinMap[1][] : QD pin no (중간 단계)
            //son PinMap[2][] : SWITCH CARD pin no(hwPin). 0부터 시작  (jigPin(swPin)을 index로 넣어서 hwPin값을 찾을 수 있다.)
            //son PinMap[3][] : PinBlock No(1~8)                      //son210514
            //son PinMap[4][] : PinBlockType(BOX_QD/BOX_DD/BOX_SD)    //son210514
            //son PinMap[5][] : PinBlock HwBoxType(TYPE_AC/TYPE_BD)   //son210514
            //son PinMap[6][] : GetBlockNo_PinNo()로 받아온 nPin1(QdJigPin) //son210514
            
//son hwPin을 index로 jigPin0을 찾기 위한 array
int nJigPinMap[MAX_PIN+1];     //son index:hwPin, val:jigPin0. 0 base    //son230303       
           

//extern int nHVCalFlag1;//sylee20120812
//extern int nHVCalFlag1Co1;//sylee20120812
extern double IADClog1[DEF_MAX_PIECE1][100];//sylee20120812 //son220823_6

extern int g_nPinPieceNo[MAX_DUT1][DEF_MAX_PIN1];//SYLEE180810-1-DUT1 //son220824_2 DefDutMax -> MAX_DUT1(33)
//extern int gd4W1_P1Low[30][5];//SYLEE160503      //son211012 moved to ModeSub18.h
//extern double gd4W1_P2High[30][6];//SYLEE160503  //sylee170819-1  int->double     //son211012 moved to ModeSub18.h
extern int nBlockMaxLast1;//sylee170417-1


//son210324 for TestCondigionLog(삼성전기 포맷 recpie 출력)
sTestConditionData    g_testConditionLogData[MAX_TEST_CONDITION_ITEM];       

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileSysInfo::CFileSysInfo()
{

}

CFileSysInfo::~CFileSysInfo()
{

}

//son      SD: 1card 64pin,   DD: 1card 64pin,   QD: 1card 128pin,   S100-1 : 1card에 64pin
//        
//son      SD 1box 1024pin (16card * 64pin)
//son      DD 1box 2048pin (32card * 64pin)
//son      QD 1box 4096pin (32card * 128pin)
//        
//                    상부pin                       하부pin               Box  EndBox  상부card        하부card   EndCard
//   4K S100-1 SD :  2K(2box *16card *64pin)    2K(2box *16card *64pin)    4      4    2*16=32            2*16=32    64
//         8K  DD :  4K(2box *32card *64pin)    4K(2box *32card *64pin)    4      4    2*32=64            2*32=64    128    
//(8KDD변형)4K DD :  2K(1box *32card *64pin)    2K(1box *32card *64pin)    2      2    32                 32         64
//         16K QD :  8K(2box *32card *128pin)   8K(2box *32card *128pin)   4      4    2*32=64            2*32=64    128  <== 여기까지는 endBox 규칙 잘 안맞음
//                                                                                     32K아닌 기존 코드에서    CardPin 마지막 계산때 +16384 offset으로 계산함.
//      12K QD_DD :  8K(2box *32card *128pin)   4K(2box *32card *64pin)    4      4    2*32=64            2*32=64    128  
//
//(16K변형)8KNewOrg: 4K(1box *32card *128pin)   4K(1box *32card *128pin)   2      5    1*32=32  32+64(skip) 1*32     160 (32+32+64+32)
//                    상부 3box skip             하부 1box 없음 
//(16K변형) 12K QD:  8K(2box *32card *128pin)   4K(1box *32card *128pin)   3      5    2*32=64   64(skip)   1*32     160 (64+64+32)  //son210202
//                    상부 2box skip             하부 1box  없음                                 
//(16K변형) U_8K QD: 8K(2box *32card *128pin)    하부 2box  없음           2      2    2*32=64                       64
                                                                                    
//         32K TR : 16K(4box *32card *128pin)  16K(4box *32card *128pin)   8      8    4*32=128           4*32=128   256
//
//         24K TR : 16K(4box *32card *128pin)   8K(2box *32card *128pin)   6      6    4*32=128           2*32=64    192
//                                               하부 2box  없음
//         20K TR : 16K(4box *32card *128pin)   4K(1box *32card *128pin)   5      5    4*32=128           1*32=32    160
//                                               하부 3box  없음
//(32K 변형)16K_TR:  8K(2box *32card *128pin)   8K(2box *32card *128pin)   4      6    2*32=64  64(skip)  2*32=64    192 (64+64+64) 
//                     상부 2box skip            하부 2box 없음                                   
//(32K 변형)12K TR:  8K(2box *32card *128pin)   4K(1box *32card *128pin)   3      5    2*32=64  64(skip)  1*32=32    160 (64+64+32) 
//                     상부 2box skip            하부 3box 없음 

//son211020
//son240522 bool bIsHwPin 추가. if (nPinTotMax== PIN_TOT_32K_ORG) 일때만 유효함. 
//           false: skip 블록을 뺀 endBox 개수리턴, 
//           true : skip 블록 포함 endBox 개수 리턴 
int CFileSysInfo::getEndBoxNo(int nPinTotMax, bool bIsHwPin)     //son211020
{ 
    int nEndBox;

    //sylee130223 if(nPinTotMax>0&& nPinTotMax<5){

    nEndBox=4;
    if(nPinTotMax > 0 && nPinTotMax < PIN_TOT_END)
            //&& SysSet12.m_nPinSwitchMode==3)  //son211020  m_nPinSwitchMode 3은 아무 의미 없는 걸로 보임. 삭제.
    {         

        if (nPinTotMax == PIN_TOT_16K)             //son190902 32KPIN: enum 변경
            nEndBox=4;      //son box 4개: 4K *4 = 16K
        
        else if (nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)      //son211020
            nEndBox=2;      //son box 2개: 4K *2 = 8K   //son211020

        else if (nPinTotMax == PIN_TOT_32K_TRANS //son230216_1
            	|| nPinTotMax== PIN_TOT_32K_U8K_L24K)//son230224_2
	            //son240516|| nPinTotMax== PIN_TOT_32K_ORG)     //son230224_2
            nEndBox= MAX_BOX;       //son-32KPIN-20191223  box 8개: 4K *8 = 32K

        else if (nPinTotMax== PIN_TOT_32K_ORG)     //son240516
        {
            if (bIsHwPin)
                nEndBox= MAX_BOX_PER_PART + SysSet12.m_nLowerBox;  //son240522
            else
                nEndBox= SysSet12.m_nUpperBox + SysSet12.m_nLowerBox;  //son240516
             return nEndBox;
        }


        else if (IS_LOWER_8K(nPinTotMax))   //son210531
            nEndBox=6;      //son box 6개: 4K *6 = 24K


        //else if (nPinTotMax == PIN_TOT_8K_LOW_STPIN       //son-8K4097-20200615: 8K 구형 하부시작핀 사용시
        //       || nPinTotMax == PIN_TOT_12K               //son210202
        //       || nPinTotMax == PIN_TOT_12K_TRANS         //son210319 //son230216_1
        //       || nPinTotMax == PIN_TOT_8K_TRANS)         //son210319 //son230216_1
        //{
        //  nEndBox=5;      //son 8K_NEW: box 5개: 4K *3 = 12K (상부 뒤쪽 3개 box는 skip 필요, 상부1개, 하부1개만 사용)
        //                  //    12K   : Use UpperBox 1, 2 LowerBox 5,   Skip UpperBox 3, 4
        //}
        else if (IS_LOWER_4K(nPinTotMax))   //son210531
            nEndBox=5;      //son box 5개: 4K *5 = 20K

        else 
        {
            //son 32K base new mode라면 box 8개가 기본
            if (IS_NEW_PIN_TOTMAX(nPinTotMax))  //son211020
                nEndBox= MAX_BOX;  //son 8: MAX_BOX //son210531  default 로 추가

            //son 32K 이전 코드는 box 4개 (QD 기준)
            else 
                nEndBox= 4;       //son211020
        }
        
    }

    return nEndBox;
}

//son ChildView5, ChildView8 기존 (QD base Old. SYLEE ) SelfTest 코드에서 nCardNoEnd 계산함수
//    중간에 card skip 고려 안하는 경우
int CFileSysInfo::getCardNoEnd_NoSkip(int nCardX, int nPinTotMax, int nPinBlockType) //son240125 
{
    int nCardNoEnd = 128;

    //son230224_2
	//	nCardX=2;//sylee150210 //ACE380
	if (nPinTotMax == PIN_TOT_4K) {//4k 		//son-32KPIN-20190902 : enum 으로 수정
		nCardNoEnd = 64 * nCardX;//SYLEE170327  64->64     
	}
	else if (nPinTotMax == PIN_TOT_8K //8k		 
	          || nPinTotMax == PIN_TOT_8K_TRANS)    //son240125
    {
		nCardNoEnd = 64 * nCardX;//SYLEE170327  128->64
		if (nPinBlockType == PIN_BLK_DD) { 	//son-32KPIN-20190902 : enum 으로 수정
			//nCardNoEnd = 64 * 2;//64pin x 128ea = 8K //sylee180830-1-DDTYPE-JTECH
			nCardNoEnd = 128 * nCardX;  //son230224_2
		}
	}
    else  if (nPinTotMax == PIN_TOT_8K_LOW_STPIN        //son240125
            || nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)    //son240125
    {
        nCardNoEnd = 64*nCardX;//nCardX;//son2101020
    }
	else if (nPinTotMax == PIN_TOT_16K //16k		//son-32KPIN-20190902 : enum 으로 수정
	         || nPinTotMax == PIN_TOT_16K_TRANS    //son240125
	         || nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS)  //son240125
    {
		nCardNoEnd = 128 * nCardX;
	}
	else if (nPinTotMax == PIN_TOT_12K //12k		//son240125
	        || nPinTotMax == PIN_TOT_12K_TRANS     //son240125
	        || nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS)     //son240125
    {
		nCardNoEnd = 96 * nCardX;
	}
	else if (nPinTotMax == PIN_TOT_20K_UP_TR //20k		//son240125
	       || nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS   //son240125
	       || nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)   //son240125
    {
		nCardNoEnd = 160 * nCardX;
	}
	else if (nPinTotMax == PIN_TOT_24K_UP_TR //24k		//son240125
	       || nPinTotMax == PIN_TOT_24K_TRANS) //son240125
    {
		nCardNoEnd = 192 * nCardX;
	}
	else if (nPinTotMax == PIN_TOT_32K_TRANS //32k		//son-32KPIN-20190902 : enum 으로 수정 //son230216_1
	          || nPinTotMax== PIN_TOT_32K_U8K_L24K //son230224_2
	          || nPinTotMax== PIN_TOT_32K_ORG)      //son230224_2
    {
		nCardNoEnd = 256 * nCardX;
	}
	else {
		nCardNoEnd = 128 * nCardX;
	}

    if (nPinBlockType == PIN_BLK_YAMAHA) { //son-32KPIN-20190902: 
        nCardNoEnd = 24 *nCardX;	//SYLEE170922	   
    }


    //sylee161115    0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    if (nPinBlockType == PIN_BLK_S100_2_WIRE) { //son-32KPIN-20190902: //son230216_1
        nCardNoEnd = 32 *nCardX;
    }


    //sylee171109-1    0=QD,1=DD,2=SD,3=S100-1,4=S100-2  SDBLOCK ,5=YAMAHA 
    if (nPinBlockType == PIN_BLK_4K_DD_421) { //son-32KPIN-20190902: 
        nCardNoEnd = 64  * nCardX;
    }

    if (nPinBlockType == PIN_BLK_12K_QD_DD) { //sylee180316-1 
        nCardNoEnd = 128 * nCardX;
    }


    return nCardNoEnd;
}

//son211020  New SelfTest  용.  중간에 card skip 하는 경우
// return : nCardNoEnd  (1 base )
int CFileSysInfo::getCardNoEnd(int nCardX, int nPinTotMax)
{
    int nCardNoEnd;

//son240516 begin: 
    if (nPinTotMax == PIN_TOT_32K_ORG)  
    {
        //son211020 EndBox 만 제대로 찾아서 그냥  32(MAX_BOX_CARD)만 곱해도 비슷한 결과를 낼 수 있다.
        //          side effect 가 있을지 모르므로 현 상태 유지한다.  
        //son240516 기존 제품과 호환 문제가 적은 PIN_TOT_32K_ORG만 이 코드 사용하기로 함.
        int box_num = getEndBoxNo(nPinTotMax);
        nCardNoEnd = (MAX_BOX_CARD * box_num) *nCardX;

        CString str;
        if (nCardNoEnd > (MAX_BOX_CARD * MAX_BOX * nCardX)) {   //son 256 : MAX_BOX_CARD(32) * MAX_BOX(8)
            str.Format("getCardNoEnd() Error.  EndCard(=%d) > (%d*%d) (32k).  \n \nSet Error !", 
                            nCardNoEnd, (MAX_BOX_CARD * MAX_BOX), nCardX);//ACE380
            errMessageBox(7502, str); 
            return -1;	
        }
        return nCardNoEnd;
    }
//son240516 end.
    
    nCardNoEnd = 128;       //son211020
    if(nPinTotMax== PIN_TOT_4K)         //4k
    {
        //son End box 2  * 32 card (DD)  or
        //    End box 4  * 16 card (S100-1) 
        nCardNoEnd = 64*nCardX; //sylee151007  32->64  //ACE400   DD
    }
    else  if(nPinTotMax == PIN_TOT_8K)   // 8k DD
    {
        //son End box 4  * 32 card (DD)
        //son230224_2 //nCardNoEnd = 128*nCardX;//nCardX;//sylee151007  64->128  //ACE400  DD
        nCardNoEnd = 64 * nCardX;                     //son230224_2
        if (SysSet13.m_nPinBlockType == PIN_BLK_DD)  	//son230224_2
            nCardNoEnd = 128 * nCardX;                  //son230224_2
    }
    else  if (nPinTotMax == PIN_TOT_16K)   //16k QD
    {
        //son End box 4  * 32 card (QD)
        nCardNoEnd = 128*nCardX;//nCardX;//sylee150210 //ACE380
    }
    else  if (nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)   //8k QD Upper Only     //son211020
    {
        //son End box 2  * 32 card (QD)
        nCardNoEnd = 64*nCardX;//nCardX;//son2101020
    }

    else  if (nPinTotMax == PIN_TOT_32K_TRANS//32k //son230216_1
               || nPinTotMax == PIN_TOT_32K_U8K_L24K    //son230224_2
               || nPinTotMax == PIN_TOT_32K_ORG)        //son230224_2
    {
        //son End box 8  * 32 card   ( *128 pin 하면 32768)
        nCardNoEnd = 256*nCardX; //sylee150210 //ACE380   
    }
    else if (IS_LOWER_4K(nPinTotMax)) //son210531
    {
        //son End box 5  * 32 card
        nCardNoEnd = (128+32) * nCardX;       
    }
    else if (IS_LOWER_8K(SysSet12.m_nPinTotMax)) //son210531
    {
        //son End box 6 * 32 card
        nCardNoEnd = (128+64) * nCardX;        
    }
    else {
        //son End Box 
        //nCardNoEnd = 32*nCardX;//nCardX;//sylee150210 //ACE380
        nCardNoEnd = 128;       //son211020
    }


    return nCardNoEnd;
}

//son231213 BH 12K Point PinMode SelfTest 문제점 보완
//    : 16K Orignal (QD block) 인데  OnAuto_Self1_32K() 사용하는 경우 
//      65번 card error가 129번 card error로 출력되는 문제점 보완
//return: nPrintCard
int CFileSysInfo::getPrintCardNo(int nPinTotMax, int nHwCard)     //son211020
{
    int nPrintCard = nHwCard;


    switch (nPinTotMax)
    {
        //------------------------------------------------------------
        //son QD PinBlock: 실제로 상부에 2box(64 card)밖에 없는 경우.  
        //------------------------------------------------------------
        case PIN_TOT_16K:
        case PIN_TOT_8K_LOW_STPIN:
        case PIN_TOT_12K:   //실제로는 상부에 2box(64 card)밖에 없지만 hwCard는 상부 4개(128 card)인 걸로 코딩돼 있음.
                            //하부시작 hwCard번호는 128 이므로 -64해서 64로 만들어 준다.
            if (nHwCard >= 128) //son240522 if (nHwCard >= 64)
                nPrintCard = nHwCard - 64; 
            break;

//son240522 begin:
        case PIN_TOT_32K_ORG:  
            switch(SysSet12.m_nUpperBox)
            {
                case 1: //son 상부 1 block만 있음.   skip된 1개 block 만큼 96개 card 번호를 당긴다.
                    if (nHwCard >= 128)
                        nPrintCard = nHwCard - 96; 
                    break;

                case 2: //son 상부 2 block만 있음.   skip된 2개 block 만큼 64개 card 번호를 당긴다.
                    if (nHwCard >= 128)
                        nPrintCard = nHwCard - 64; 
                    break;

                case 3:     //son 상부 3 block만 있음.  skip된 1개 block 만큼 32개 card 번호를 당긴다.
                    if (nHwCard >= 128)
                        nPrintCard = nHwCard - 32; 
                    break;

                default:
                    break;
            }
            break;
//son240522 end.

        default:
            break;

    }
    

    return nPrintCard;
}



//son "C:\\ACE400\\SETUP2\\SysSetPin.ini"를 struct CSysSet12에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadSaveSet12(int type)//sylee140110  
{   
    FILE *fp,*fp2; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    char  fName2[FILE_PATH_SIZE];  //sylee140110  //son210524 200-> FILE_PATH_SIZE
    CString str; 

    //son BDL별로 별도 생성되는 QD 정보를 로딩하는 파일 !!!
    ::ZeroMemory(&fName, sizeof(fName));    
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE121202
    strcat( fName ,str);
    strcat( fName , "\\SETUP2\\SysSetPin.ini" );
    

    //son QD 이외의 공통 정보를 로딩하는 파일 
    ::ZeroMemory(&fName2, sizeof(fName2));//sylee140110 
    str.Format("%s\\SETUP2\\SysSetPin.ini", SysInfoView01.m_pStrFilePath1); //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat( fName2 , str ); //son201118 //son210514
                    
                    
    int ret;        //son211214
    CString rStr;   //son211214
    switch(type)                
    {
        
    case _SAVE: //1: SAVE   //son211012           
                    
        fp2 = fopen(fName2,"wt");           
        if(fp2 == NULL){            
            str.Format("Error No 2001 !\n\n  %s \n\n file Data Saving Error !     file close!   ", fName2);//SYLEE121202        
            AfxMessageBox( str , MB_OK); //error messege        
            return 0;       
        }       
            
        fprintf(fp2, "%10d\n",SysSet12.m_nPinTotMax);                       
        fprintf(fp2, "%10d\n",SysSet12.m_nPinBoxMax);           
        fprintf(fp2, "%10d\n",SysSet12.m_nPinSwitchMode);           
        fprintf(fp2, "%10d\n",SysSet12.m_nPinType);             
        fprintf(fp2, "%10d\n",SysSet12.m_nCardReverse);             
        fprintf(fp2, "%10d\n",SysSet12.m_nBlockSet1);           
        fprintf(fp2, "%10d\n",SysSet12.m_nCardReverseB);            
        //fprintf(fp2, "%10d\n",SysSet12.m_nLowerJigStartPin); //sylee150917    
        fclose(fp2); 
        
        //son BDL별 하부시작핀 정보파일에 SysSet12.m_nLowerJigStartPin를 write한다.
        ret = LoadSaveLowerStartPin(_SAVE, rStr);//sylee210406   //son211214                   
        if (ret <= 0) {
            //son 에러 출력은 LoadSaveLowerStartPin() 함수 호출 지점에서 출력하는 것으로 수정.  
            AfxMessageBox(rStr, MB_OK); //son211214
        }

        //son BDL 별 QD 정보를 로딩하는 파일 Open  : 
        //    !!! BDL 마다 별도로 있는 Setup2\SysSetPin.ini write. !!!!
        // ex) "C:\ACE400\QC-JIG-S100-BHFLEX\4W_YP_S100\SETUP2\SysSetPin.ini"  
        fp = fopen(fName,"wt");         
        if(fp == NULL){             
            str.Format("Error No 2001 !   \n\n  %s  \n\n file Data Saving Error !   file close!   ", fName);//SYLEE121202       
            AfxMessageBox( str , MB_OK); //error messege        
            return 0;       
        }       
 
        //son DD
        if( SysSet13.m_nPinBlockType==PIN_BLK_DD        //sylee180413-1
                || SysSet13.m_nPinBlockType==PIN_BLK_4K_DD_421 ) //son190902 32KPIN 
        { 

            if( SysSet12.m_nPinQDUpper  == BOX_QD){ //son 1
                SysSet12.m_nPinQDUpper = BOX_DD;    //son 2
            }               
            if( SysSet12.m_nPinQDLower == BOX_QD){  //son 1
                SysSet12.m_nPinQDLower = BOX_DD;    //son 2
            }               
            if( SysSet12.m_nPinQDUpper2 == BOX_QD){ //son 1
                SysSet12.m_nPinQDUpper2 = BOX_DD;   //son 2
            }               
            if( SysSet12.m_nPinQDLower2 == BOX_QD){ //son 1
                SysSet12.m_nPinQDLower2 = BOX_DD;   //son 2
            }
        }
      
        //son 12K 변형 PinBlock (상부 QD 4K *2, 하부 DD 2K *2 -> 12K) 인 경우
        if( SysSet13.m_nPinBlockType==PIN_BLK_12K_QD_DD)//sylee180323-1     //son190902 32KPIN 
        {   
 
            if( SysSet12.m_nPinQDLower == BOX_QD){  //son 1
                SysSet12.m_nPinQDLower = BOX_DD;
            }   
            if( SysSet12.m_nPinQDLower2 == BOX_QD){ //son 1
                SysSet12.m_nPinQDLower2 = BOX_DD;
            }
        }
 
        
        if( SysSet13.m_nPinBlockType==PIN_BLK_SD  || SysSet13.m_nPinBlockType==PIN_BLK_S100_1   //son190902 32KPIN 
                || SysSet13.m_nPinBlockType==PIN_BLK_S100_2_WIRE  || SysSet13.m_nPinBlockType==PIN_BLK_YAMAHA  ) 
        {    
 
            SysSet12.m_nPinQDUpper = BOX_QD;            //son 1
            SysSet12.m_nPinQDLower = BOX_QD;            //son 1
            SysSet12.m_nPinQDUpper2 = BOX_QD;           //son 1
            SysSet12.m_nPinQDLower2 = BOX_QD;           //son 1
        }

        //if( SysSet12.m_nPinTotMax == PIN_TOT_32K_TRANS || SysSet12.m_nPinTotMax == PIN_TOT_20K_UP_TR 
        //        || SysSet12.m_nPinTotMax == PIN_TOT_24K_UP_TR || SysSet12.m_nPinTotMax == PIN_TOT_24K_TRANS ){//sylee210330-1 //son230216_1

        //}else{
        //    SysSet12.m_nPinQDUpper3=0; SysSet12.m_nPinQDLower3=0; 
        //    SysSet12.m_nPinQDUpper4=0; SysSet12.m_nPinQDLower4=0;
        //}
        //son Upper 3,4를  안 쓰는 경우에는 값을 0으로 강제 설정한다.
        if (IS_UPPER_8K_2(SysSet12.m_nPinTotMax)         //son210826 //son230718
                || IS_UPPER_4K(SysSet12.m_nPinTotMax))  //son210525
        {
            SysSet12.m_nPinQDUpper3=0; 
            SysSet12.m_nPinQDUpper4=0; 
        }

        //son PIN_TOT_16K_U12K_L4K_TRANS, PIN_TOT_16K_U12K_L4K_UP_TRANS Upper box 4 skip
        else if (IS_UPPER_12K_2(SysSet12.m_nPinTotMax))    //son210525   //son230718
        {
            SysSet12.m_nPinQDUpper4=0; 
        }
        
        //son Lower 3,4를 안 쓰는 경우에는 값을 0으로 강제 설정한다.
        if (IS_LOWER_8K(SysSet12.m_nPinTotMax) || IS_LOWER_4K(SysSet12.m_nPinTotMax))      //son210525
        {
            SysSet12.m_nPinQDLower3=0; 
            SysSet12.m_nPinQDLower4=0;
            SysSet12.m_nPinQDLower5=0;  //son230217
            SysSet12.m_nPinQDLower6=0;  //son230217
        }


        fprintf(fp, "%10d\n",SysSet12.m_nPinQDUpper);                       
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDLower);           
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDUpper2);                      
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDLower2);  
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDUpper3); //sylee210330-1                      
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDLower3); //sylee210330-1              
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDUpper4);  //sylee210330-1                         
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDLower4);  //sylee210330-1         
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDLower5);  //son230217         
        fprintf(fp, "%10d\n",SysSet12.m_nPinQDLower6);  //son230217
        fclose(fp);     

        break;          

                    
    case _LOAD://2: LOAD  //son211012 
                    
        //son "C:\\ACE400\\SETUP2\\SysSetPin.ini"  QD 이외의 정보를 로딩하는 파일 Open 
        fp2 = fopen(fName2,"rt");           
        if(fp2 == NULL){            
            str.Format("Error No 2001 !  \n\n  %s file Loading Error !   \n\n  file close!  file check!  ", fName2);//SYLEE121202       
            AfxMessageBox( str , MB_OK); //error messege        
            return 0;       
        }           
                    
        fscanf(fp2, "%10d\n", &SysSet12.m_nPinTotMax);                      //son ex)  1
        fscanf(fp2, "%10d\n", &SysSet12.m_nPinBoxMax);                      //son ex)  1
        fscanf(fp2, "%10d\n", &SysSet12.m_nPinSwitchMode);                  //son ex)  2
        fscanf(fp2, "%10d\n", &SysSet12.m_nPinType);                        //son ex)  2
        fscanf(fp2, "%10d\n", &SysSet12.m_nCardReverse);                    //son ex)  1        
        fscanf(fp2, "%10d\n", &SysSet12.m_nBlockSet1);                      //son ex)  0
        fscanf(fp2, "%10d\n", &SysSet12.m_nCardReverseB);                   //son ex)  1


        //son BDL별 폴더에서 SysSet12.m_nLowerJigStartPin를  읽어 온다.
        ret = LoadSaveLowerStartPin(_LOAD, rStr);//sylee210406  //son211214
        if (ret <= 0) {
            //son211214
            //son BDL 폴더에 "\\SETUP2\\LowerStartPin.ini" 파일이 없다면 
            //    기존 방식으로 하부시작핀 정보를 읽어온다. 에러 출력은 하지 않는다.
            ret = fscanf(fp2, "%10d\n", &SysSet12.m_nLowerJigStartPin);//son211214

            //son fscanf()까지 실패했다면 에러 출력을 한다.
            if (ret <= 0) 
                AfxMessageBox(rStr, MB_OK); //son211214
        }
                    
        fclose(fp2);                
                    
        //son QD 정보를 로딩하는 파일 Open  : 
        //    !!! BDL 마다 별도로 있는 Setup2\SysSetPin.ini 에서 가져온다!!!!
        // ex) "C:\ACE400\QC-JIG-S100-BHFLEX\4W_YP_S100\SETUP2\SysSetPin.ini"  
        fp=fopen(fName,"rt");           
        if(fp == NULL){             
            str.Format("Error No 2001 !\n  %s file Loading Error !     file close!  file check!  ", fName);//SYLEE121202            
            AfxMessageBox( str , MB_OK); //error messege        
            return 0;           
        }                   
                    
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDUpper);      //son ex) 1     
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDLower);      //son ex) 1     
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDUpper2);     //son ex) 1     
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDLower2);     //son ex) 1
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDUpper3);     //sylee210330-1     
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDLower3);     //sylee210330-1     
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDUpper4);     //sylee210330-1 
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDLower4);     //sylee210330-1 
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDLower5);     //son230217
        fscanf(fp, "%10d\n", &SysSet12.m_nPinQDLower6);     //son230217

        //son Upper 3,4를  안 쓰는 경우에는 값을 BOX_SKIP으로 강제 설정한다.
        if(IS_UPPER_8K_2(SysSet12.m_nPinTotMax)             //son210826  //son230718
                || IS_UPPER_4K(SysSet12.m_nPinTotMax))      //son210525
        {
            SysSet12.m_nPinQDUpper3= BOX_SKIP; //son241007-BSW-9  0 -> BOX_SKIP
            SysSet12.m_nPinQDUpper4= BOX_SKIP; //son241007-BSW-9  0 -> BOX_SKIP
        }

        //son PIN_TOT_16K_U12K_L4K_TRANS, PIN_TOT_16K_U12K_L4K_UP_TRANS skip Upper 4'th box 
        else if(IS_UPPER_12K_2(SysSet12.m_nPinTotMax))    //son210525  //son210826 //son230718
        {
            SysSet12.m_nPinQDUpper4= BOX_SKIP; //son241007-BSW-9  0 -> BOX_SKIP
        }
        
        //son Lower 3,4를 안 쓰는 경우에는 값을 BOX_SKIP으로 강제 설정한다.
        if(IS_LOWER_8K(SysSet12.m_nPinTotMax) || IS_LOWER_4K(SysSet12.m_nPinTotMax))      //son210525
        {
            SysSet12.m_nPinQDLower3= BOX_SKIP; //son241007-BSW-9  0 -> BOX_SKIP
            SysSet12.m_nPinQDLower4= BOX_SKIP; //son241007-BSW-9  0 -> BOX_SKIP
            SysSet12.m_nPinQDLower5= BOX_SKIP;  //son230217 //son241007-BSW-9  0 -> BOX_SKIP
            SysSet12.m_nPinQDLower6= BOX_SKIP;  //son230217 //son241007-BSW-9  0 -> BOX_SKIP
        }

        fclose(fp); 

     
        //son BlockType이 DD 인데 boxType이 QD라면 DD로 보정
        if(  SysSet13.m_nPinBlockType==PIN_BLK_DD                //sylee180413-1    
                || SysSet13.m_nPinBlockType==PIN_BLK_4K_DD_421 ) //son190902 32KPIN 
        { 
            if( SysSet12.m_nPinQDUpper  == BOX_QD){ //son 1
                SysSet12.m_nPinQDUpper = BOX_DD;    //son 2
            }               
            if( SysSet12.m_nPinQDLower == BOX_QD){  //son 1
                SysSet12.m_nPinQDLower = BOX_DD;    //son 2
            }               
            if( SysSet12.m_nPinQDUpper2 == BOX_QD){ //son 1
                SysSet12.m_nPinQDUpper2 = BOX_DD;   //son 2
            }               
            if( SysSet12.m_nPinQDLower2 == BOX_QD){ //son 1
                SysSet12.m_nPinQDLower2 = BOX_DD;   //son 2
            }
        } 

        //son BlockType이 SD 관련이면 boxType을 QD로 강제 수정. 
        //    (SD, S100은 CModeSet12::OnInitDialog()에서 QD를 SD로 바꿔서 보여줌)
        if( SysSet13.m_nPinBlockType==PIN_BLK_SD  || SysSet13.m_nPinBlockType==PIN_BLK_S100_1 //son190902 32KPIN 
                || SysSet13.m_nPinBlockType==PIN_BLK_S100_2_WIRE   //son230216_1
				|| SysSet13.m_nPinBlockType==PIN_BLK_YAMAHA ) 
        {
            SysSet12.m_nPinQDUpper = BOX_QD;        //son 1
            SysSet12.m_nPinQDLower = BOX_QD;        //son 1
            SysSet12.m_nPinQDUpper2 = BOX_QD;       //son 1
            SysSet12.m_nPinQDLower2 = BOX_QD;       //son 1
        }


        if(OnPinConvertType1==1) //son241007-BSW-9 begin:
        {
            //son .CON이 있는데  BlockSet(QD/DD/SD 혼용)은 사용하면 이중으로 변형하게 되므로 강제로 Off한다 (.CON이 우선된다.)
            SysSet12.m_nBlockSet1 = 0;   

            //son .CON이 있다면 모두 QD 타입으로 고정한다.
            SysSet12.m_nPinQDUpper  = BOX_QD;   
            SysSet12.m_nPinQDUpper2 = BOX_QD;   
            SysSet12.m_nPinQDUpper3 = BOX_QD;   
            SysSet12.m_nPinQDUpper4 = BOX_QD;   

            SysSet12.m_nPinQDLower  = BOX_QD;   
            SysSet12.m_nPinQDLower2 = BOX_QD;   
            SysSet12.m_nPinQDLower3 = BOX_QD;   
            SysSet12.m_nPinQDLower4 = BOX_QD;   

            //son .CON이 제대로 해석되기 위해 hw구조대로 하부시작핀은 16385, endPin은 32768로 한다.
            SysSet12.m_nLowerJigStartPin = 16385; 
            SysSet12.m_nEndPin = 32768; 

        } //son241007-BSW-9 end.
 
        if(OnPinConvertFlag1!=1){//sylee150922-1   //sylee151111add
             OnPinConvertFlag1 =0;//sylee151111 
        }//sylee151111 
            
                    
        break;              
        

    default :               
        break;          
    }               

    //--------------------------
    //son nPinMap[] 생성    
    
    //son nPinMap[] 생성하고 D:\\PinMap.txt 파일에 write한다.
    if( SysSet13.m_nPinBlockType==PIN_BLK_YAMAHA) {  //YAMAHA  //sylee161108-1 //son190902 32KPIN
        ModeSet12.OnPinConvert1_YAMAHA();        
    }

    else if (SysSet13.m_nPinBlockType == PIN_BLK_S100_2_WIRE         //sylee180413-1 //son230216_1
            ||(SysSet13.m_nPinBlockType == PIN_BLK_DD && OnPinConvertType1 == 0)) { //son190902 32KPIN 
        ModeSet12.OnPinConvert1() ;
    }

    else if(IS_NEW_PIN_TOTMAX2(SysSet13.m_nPinBlockType, SysSet12.m_nPinTotMax))   //son210319
    {
#if 0
// TestCode:  Make all 32K, 24K,20K  PinMap Test Case.
        if (type == _SAVE)
        {
            UINT prevQDUpper = SysSet12.m_nPinQDUpper;
            UINT prevQDLower = SysSet12.m_nPinQDLower;
            UINT prevLowerJigStartPin = SysSet12.m_nLowerJigStartPin;
            for (int i = 0; i < 3; i++) 
            {
                // BOX_SD, BOX_DD, BOX_QD
                SysSet12.m_nPinQDUpper = BOX_QD+i;
                SysSet12.m_nPinQDLower = BOX_QD+i;
                for (int j = 2-i; j < 5-i ; j++)
                {
                    //  i=0, QDUpper=BOX_QD, j=2, JigStartPin=4097
                    //  i=0, QDUpper=BOX_QD, j=3, JigStartPin=8193
                    //  i=0, QDUpper=BOX_QD, j=4, JigStartPin=16385
                    //  i=1, QDUpper=BOX_DD, j=1, JigStartPin=2049
                    //  i=1, QDUpper=BOX_DD, j=2, JigStartPin=4097
                    //  i=1, QDUpper=BOX_DD, j=3, JigStartPin=8193
                    //  i=2, QDUpper=BOX_SD, j=0, JigStartPin=1025
                    //  i=2, QDUpper=BOX_SD, j=1, JigStartPin=2049
                    //  i=2, QDUpper=BOX_SD, j=2, JigStartPin=4097
                    SysSet12.m_nLowerJigStartPin = ModeSet12.m_LowerStartPin.saList[j].nPin;

                    TRACE("i=%d, QDUpper=%s, j=%d, JigStartPin=%d\n", 
                            i, STR_BOXQD_TYPE(SysSet12.m_nPinQDUpper),
                            j, SysSet12.m_nLowerJigStartPin);
                    ModeSet12.OnPinConvert1_32K(OnPinConvertType1);  
                }
            }

            SysSet12.m_nPinQDUpper = prevQDUpper; 
            SysSet12.m_nPinQDLower = prevQDLower; 
            SysSet12.m_nLowerJigStartPin = prevLowerJigStartPin;
        }
#endif

        int ret = ModeSet12.OnPinConvert1_32K(OnPinConvertType1);  

        //son-32KPIN-20200113
        // 20K인데 32K 영역의 BDL을 로딩한다면 없는 B,C, D 박스의 PinMap[1][pin]는 -1로 리턴됨. 
        // 이 경우는 계측기 SW 오동작할 수 있으므로 미리 경고해 주기 위해 nErrCount 추가함.
        // nErrCount가 1이상이면 -1을 리턴해서 오류가 있음을 표시해 준다.
        return ret;

    }

    else  // QD BLOCK
    {
        ModeSet12.OnPinConvert1_READ(OnPinConvertType1);  //sylee160520   //qd block
    }           

                    
                    
    return 1;               
}                   
                    


//son191212:  LV Short Use Option을 일시적으로 끄기 위해 사용    
//            LEAK 테스트용이므로 양산버전은 이 define 문을 주석처리해야 한다.
//#define LAB_LEAK_TEST     


//son "C:\\ACE400\\SETUP\\SysSet01.ini"를 struct CSysSet13에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadSaveSet13(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName));

#ifdef DEF_CARD_A1   
    str.Format("%s\\SETUP\\SysSet02.ini", SysInfoView01.m_pStrFilePath1);   //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName, str); //sylee181023   //son210514
#else   
    str.Format("%s\\SETUP\\SysSet01.ini", SysInfoView01.m_pStrFilePath1);   //son210514
    strcat(fName, str); //sylee121202  //SYLEE150904 //son210514
#endif   


 
    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
 
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n%s  file Data Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK); //error messege
            return 0;
        }            

        //son210215 UltLog for Samsung Electro-Mecha
        //son Z:\\LogSheet4W.csv 옵션과 Z:\\Ultlog.csv 옵션은 배타적이어야 한다. 
        //    default가  Z:\\LogSheet4W.csv이므로  Z:\\LogSheet4W.csv가 1이면 SysSet13.m_nSetUltLog는 강제로 0으로 설정한다.
        if (SysSet13.m_nSet2 == 1)
            SysSet13.m_nSetUltLog = 0;
                                                         // Line
        fprintf(fp, "%10d\n",SysSet13.m_nSet1);            // 1 //son CheckBox: Leak Current    20mA 10ma Enable
        fprintf(fp, "%10d\n",SysSet13.m_nSet2);            // 2 //son CheckBox: Z:\ LogSheet4W.CSV
        fprintf(fp, "%10d\n",SysSet13.m_nSet3);            // 3 //son CheckBox: DUT16 Use Z:\ Dut.ini
        fprintf(fp, "%10d\n",SysSet13.m_nSet4);            // 4 //son CheckBox: Selftest System Off
        fprintf(fp, "%10d\n",SysSet13.m_nSet5);            // 5 //son CheckBox: ""
        fprintf(fp, "%10d\n",SysSet13.m_nSet6);            // 6 //son CheckBox: No Password
        fprintf(fp, "%10d\n",SysSet13.m_nSet7);            // 7 //son CheckBox: Err file XT->EXCEL
        fprintf(fp, "%10d\n",SysSet13.m_nSet8);            // 8 //son CheckBox: 4W Total Logfile
        fprintf(fp, "%10d\n",SysSet13.m_nSet9);            // 9 //son CheckBox: Self Test Auto Tr1 check Use
        fprintf(fp, "%10d\n",SysSet13.m_nShortDetailOff);  // 10//son CheckBox: S K U  H L Z=> S K U  (  Print Display)    //son220318_2
        
        fprintf(fp, "%10d\n",SysSet13.m_nSet11);           // 11//son CheckBox: OPEN 200V
        fprintf(fp, "%10d\n",SysSet13.m_nSet12);           // 12//son CheckBox: Sheet Retest -> Disable
        fprintf(fp, "%10d\n",SysSet13.m_nSet13);           // 13//son CheckBox: Open R > 1K Read Enable
        fprintf(fp, "%10d\n",SysSet13.m_nSet14);           // 14//son CheckBox: Error Pin Real R. Display
        fprintf(fp, "%10d\n",SysSet13.m_nSet15);           // 15//son CheckBox: Error file -> Refer.value Disable
        fprintf(fp, "%10d\n",SysSet13.m_nSet16);           // 16//son CheckBox: LV Short Use
        fprintf(fp, "%10d\n",SysSet13.m_nSet17);           // 17//son CheckBox: Recipe Password Use
        fprintf(fp, "%10d\n",SysSet13.m_nSet18);           // 18//son CheckBox: Open + Short
        fprintf(fp, "%10d\n",SysSet13.m_nSet19);           // 19//son CheckBox: Recipe Retest Password use
        fprintf(fp, "%10d\n",SysSet13.m_nSet20);           // 20//son CheckBox: Circuit Engineer -> Interlock.Disable


        fprintf(fp, "%10d\n",SysSet13.m_nPinBlockType); //sylee161108-1   //son COMBO101:  ePIN_BLOCK_TYPE 
                                                           // 21       0:QD, 1:DD,  2:SD, 3:S100-1, 4:S100-2WIRE, 5:YAMAHA 
                                                           //          6:4k_DD_421, 7:12k QD_DD, 8:32K
        SysSet13.m_nBdlSortMode= SORT_MODE_DESCEND;//sylee220901 사장님 지시사항으로 내림차순으로 고정
        fprintf(fp, "%10d\n",SysSet13.m_nBdlSortMode);     // 22//son200722: COMBO102 0:NO_SORT, 1:SORT_MODE_DESCEND, 2:SORT_MODE_ASCEND
        fprintf(fp, "%10d\n",SysSet13.m_nUseCalData);      // 23//son220419: COMBO103 0:USECAL_UPPER_ONLY,  1:USECAL_UPPER_LOWER
        fprintf(fp, "%10d\n",SysSet13.m_nSet104);          // 24//m_combo4
        fprintf(fp, "%10d\n",SysSet13.m_nSet105);          // 25//m_combo5 

        fprintf(fp, "%10d\n",SysSet13.m_n4WCVMode);        // 26//sylee200217-4WCVMODE  0:CC  1:CV
        fprintf(fp, "%10d\n",SysSet13.m_nSparkMode);       // 27//son200908 COMBO112 0: SPARK_NEW, 1:SPARK_OLD1,  2:SPARK_OLD2
        fprintf(fp, "%10d\n",SysSet13.m_n4wLogMode);       // 28//son201209 COMBO113 0:_4W_LOG_COL, 1:_4W_LOG_LINE_DUT_PIECE, 2:_4W_LOG_LINE_DUT 
        fprintf(fp, "%10d\n",SysSet13.m_nSet114);          // 29
        fprintf(fp, "%10d\n",SysSet13.m_n4wFailSortMode);  // 30//son230106_2 COMBO115  4W Fail AutoTest 출력 Sort 모드  0:RefHigh OverRate  1: 4w NetNo
        fprintf(fp, "%10d\n",SysSet13.m_nSetUltLog);       // 31//son210215 UltLog for Samsung 0:Off,  1:On
        fprintf(fp, "%10d\n",SysSet13.m_nSet_UseLotName_4wLog); //32   //son220124 for 제이텍. LotName을 4wLog 폴더에 사용. 
        fprintf(fp, "%10d\n",SysSet13.m_nSet_NearbyOpenMeas); //33   //son230712 for 영풍SST. 근접Open 재정밀 측정기능을 On 0:Off  1: On 
        fprintf(fp, "%10d\n",SysSet13.m_nNearByOpenRate);  //34   //son230728 for 영풍SST. 근접Open 재정밀 측정기능 기준 rate (3~30 %)

        //son220426 m_nUseCalData가 UPPER_ONLY로 바뀐다면 UseLowerCalJig도 미사용으로 바뀌어야 한다. 
        if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)    //son220426 begin
        {
            bool bUseLowerCalJig = false;                
            FileSysInfo01.LoadSave_UseLowerPin(_SAVE, bUseLowerCalJig);    
        } //son220426 end
        break;


    case _LOAD://2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2002 !\n%s  file Data Loading Error", fName); //error messege
            AfxMessageBox(str, MB_OK); //error messege
            return 0;
        }

        fscanf(fp, "%10d\n", &SysSet13.m_nSet1);        
        fscanf(fp, "%10d\n", &SysSet13.m_nSet2); 
        fscanf(fp, "%10d\n", &SysSet13.m_nSet3);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet4);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet5);   
        fscanf(fp, "%10d\n", &SysSet13.m_nSet6);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet7);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet8);        
        fscanf(fp, "%10d\n", &SysSet13.m_nSet9);
        fscanf(fp, "%10d\n", &SysSet13.m_nShortDetailOff);  //son220318_2
        fscanf(fp, "%10d\n", &SysSet13.m_nSet11);       
        fscanf(fp, "%10d\n", &SysSet13.m_nSet12); 
        fscanf(fp, "%10d\n", &SysSet13.m_nSet13);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet14);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet15);   
        fscanf(fp, "%10d\n", &SysSet13.m_nSet16);
#ifndef LAB_LEAK_TEST
        SysSet13.m_nSet16=1;        //son191212: Leak1(HV) 테스트 시에는 이걸 write 할 수 있도록 수정.
                                    // 양산버전에는 이 값은 반드시 1이어야 문제가 안 생길 수 있다.
#endif      
        fscanf(fp, "%10d\n", &SysSet13.m_nSet17);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet18);
       //sylee230420  SysSet13.m_nSet18=1;
        fscanf(fp, "%10d\n", &SysSet13.m_nSet19);
        fscanf(fp, "%10d\n", &SysSet13.m_nSet20);
        fscanf(fp, "%10d\n", &SysSet13.m_nPinBlockType);//son COMBO101 ePIN_BLOCK_TYPE 
                                                        //        0:QD, 1:DD,  2:SD, 3:S100-1, 4:S100-2WIRE, 5:YAMAHA 
                                                        //        6:4k_DD_421, 7:12k QD_DD, 8:32K                                                        
        fscanf(fp, "%10d\n", &SysSet13.m_nBdlSortMode); //son200722: COMBO102 0:NO_SORT, 1:SORT_MODE_DESCEND, 2:SORT_MODE_ASCEND
		SysSet13.m_nBdlSortMode= SORT_MODE_DESCEND;//sylee220901 사장님 지시사항으로 내림차순으로 고정
        fscanf(fp, "%10d\n", &SysSet13.m_nUseCalData);  //son220419: COMBO103 0:USECAL_UPPER_ONLY,  1:USECAL_UPPER_LOWER
        fscanf(fp, "%10d\n", &SysSet13.m_nSet104);      //son COMBO104 m_combo4
        fscanf(fp, "%10d\n", &SysSet13.m_nSet105);

        fscanf(fp, "%10d\n", &SysSet13.m_n4WCVMode);    //sylee200217-4WCVMODE 0:CC  1:CV
        fscanf(fp, "%10d\n", &SysSet13.m_nSparkMode);   //son200908 COMBO112 0: SPARK_NEW, 1:SPARK_OLD1,  2:SPARK_OLD2
        fscanf(fp, "%10d\n", &SysSet13.m_n4wLogMode);   //son201209 COMBO113 0:_4W_LOG_COL, 1:_4W_LOG_LINE_DUT_PIECE, 2:_4W_LOG_LINE_DUT
        fscanf(fp, "%10d\n", &SysSet13.m_nSet114);
        fscanf(fp, "%10d\n", &SysSet13.m_n4wFailSortMode); //son230106_2 COMBO115  4W Fail AutoTest 출력 Sort 모드  0:RefHigh OverRate  1: 4w NetNo
        fscanf(fp, "%10d\n", &SysSet13.m_nSetUltLog);    //son210215 UltLog for Samsung E-Mecha 0:Off(Default),  1:On
        fscanf(fp, "%10d\n", &SysSet13.m_nSet_UseLotName_4wLog);    //son220124 for 제이텍. LotName을 4wLog 폴더에 사용.

        SysSet13.m_nSet_NearbyOpenMeas = 1; //son210712 default는 ON 
        fscanf(fp, "%10d\n", &SysSet13.m_nSet_NearbyOpenMeas);    //son230712 for 영풍SST. 근접 Open 재정밀 측정기능 On/Off 
        SysSet13.m_nNearByOpenRate = 3;     //son210728 default는 3 
        fscanf(fp, "%10d\n", &SysSet13.m_nNearByOpenRate);    //son230728 for 영풍SST. 근접Open 재정밀 측정기능 기준 rate (3~30 %) 

        //son210215 UltLog for Samsung Electro-Mecha
        //son Z:\\LogSheet4W.csv 옵션과 Z:\\Ultlog.csv 옵션은 배타적이어야 한다. 
        //    default가  Z:\\LogSheet4W.csv이므로  Z:\\LogSheet4W.csv가 1이면 SysSet13.m_nSetUltLog는 강제로 0으로 설정한다.
        if (SysSet13.m_nSet2 == 1)
            SysSet13.m_nSetUltLog = 0;
        //son220426 m_nUseCalData가 UPPER_ONLY라면 UseLowerCalJig도 미사용으로 바뀌어야 한다. 
        if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)    //son220426 begin
        {
            bool bUseLowerCalJig = false;                
            FileSysInfo01.LoadSave_UseLowerPin(_SAVE, bUseLowerCalJig);    
        } //son220426 end
 
        break;

    default :
        break;
    }

    fclose(fp); 

    return 1;
}


//son220419_2 calData를  upper, lower 중에 어느것을 사용할 지 여부를 2개의 pin으로  판단
//pin1, pin2 : 0 부터 시작하는 pin 정보 
//return value : part 정보
int CFileSysInfo::checkUseCalDataPart(int pin1,  int pin2)
{
    if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY)
        return PART_UPPER;

    int pin1Part = PART_UPPER;
    if (pin1 >= (SysSet12.m_nLowerJigStartPin -1))
        pin1Part = PART_LOWER;

    int pin2Part = PART_UPPER;
    if (pin2 >= (SysSet12.m_nLowerJigStartPin -1))
        pin2Part = PART_LOWER;

    if (pin1Part == PART_UPPER &&  pin2Part == PART_UPPER)
        return PART_UPPER;

    if (pin1Part == PART_LOWER &&  pin2Part == PART_LOWER)
        return PART_LOWER;

    return PART_MIXED;
}



//SYLEE121115-1
//son SelfTest Delay Setting 값을 가져 온다.
//son IDD_MODE_SET15_PRO1 Dialog (CModeSet15) 참조
//
//son "C:\\ACE400\\SETUP\\SysSet15.ini"에서 SelfTest Setting 관련 값을 SysSet15에 로딩하거나 Save
int CFileSysInfo::LoadSaveSet15(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName)); 
    str.Format("%s\\SETUP\\SysSet15.ini", SysInfoView01.m_pStrFilePath1);  //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName , str);//son201118     //son210514

    g_CheckFlag1=0;

    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
  
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n %s file Saving Error !   fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }
    
        fprintf(fp, "%10d\n",SysSet15.m_nSet1);  //son m_edit101:  Idle Time Jig up Test Delay  (msec)          미사용             
        fprintf(fp, "%10d\n",SysSet15.m_nSet2);  //son m_edit102:  SelfTest-> Open Delay     (usec)             SelfTestOpen_Timeout  
        fprintf(fp, "%10d\n",SysSet15.m_nSet3);  //son m_edit103:  SelfTest-> Short1,2 Delay (usec)             SelfTestShort1_2_Timeout
        fprintf(fp, "%10d\n",SysSet15.m_nSet4);  //son m_edit104:  SelfTest-> Leak1,2 Delay  (usec)             SelfTestLeak1_2_Timeout
        fprintf(fp, "%10d\n",SysSet15.m_nSet5);  //son m_edit105:  SelfTest-> LeakA, B Delay (usec)             SelfTestLeakA_B_Timeout
        fprintf(fp, "%10d\n",SysSet15.m_nSet6);  //son m_edit106:  SelfTest Delay Use Set    (7=USE)            EnableSelfTestTimeout
        fprintf(fp, "%10d\n",SysSet15.m_nSet7);

		if(SysSet15.m_nSet8>30)   SysSet15.m_nSet8=30; //sylee230822
		if(SysSet15.m_nSet8<0)   SysSet15.m_nSet8=0; //sylee230822

        fprintf(fp, "%10d\n",SysSet15.m_nSet8);   
        fprintf(fp, "%10d\n",SysSet15.m_nSet9);
        fprintf(fp, "%10d\n",SysSet15.m_nSet10); 
        fprintf(fp, "%10d\n",SysSet15.m_nSet11);   
        fprintf(fp, "%10d\n",SysSet15.m_nSet12);
        fprintf(fp, "%10d\n",SysSet15.m_nSet13); 
		fprintf(fp, "%10d\n",SysSet15.m_nSet15);

        break;

    case _LOAD: //2: LOAD  //son211012 

        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
            g_CheckFlag1=2;
            return 0;
        }

        fscanf(fp, "%10d\n", &SysSet15.m_nSet1);        
        fscanf(fp, "%10d\n", &SysSet15.m_nSet2); 
        fscanf(fp, "%10d\n", &SysSet15.m_nSet3);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet4);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet5);   
        fscanf(fp, "%10d\n", &SysSet15.m_nSet6);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet7);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet8); 
		if(SysSet15.m_nSet8>30)   SysSet15.m_nSet8=30; //sylee230822
		if(SysSet15.m_nSet8<0)    SysSet15.m_nSet8=0; //sylee230822
        fscanf(fp, "%10d\n", &SysSet15.m_nSet9);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet10);    
        fscanf(fp, "%10d\n", &SysSet15.m_nSet11);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet12); 
        fscanf(fp, "%10d\n", &SysSet15.m_nSet13);
        fscanf(fp, "%10d\n", &SysSet15.m_nSet15);
		
		break;

    default :
        break;
    }

    fclose(fp); 

    return 1;
}

 
//son "C:\\ACE400\\SETUP\\SysSet16.ini"에서 SysSet16 관련 값을 로딩하거나 Save
//SYLEE130105
int CFileSysInfo::LoadSaveSet16(int type)
{
    
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\SETUP\\SysSet16.ini",  SysInfoView01.m_pStrFilePath1);  //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat( fName , str );//son201118   //son210514

    g_CheckFlag1=0;

    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
 
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n %s file Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }
        
        fprintf(fp, "%10d\n",SysSet16.m_nSet1);   
        fprintf(fp, "%10d\n",SysSet16.m_nSet2);   
        fprintf(fp, "%10d\n",SysSet16.m_nSet3);  
        fprintf(fp, "%10d\n",SysSet16.m_nSet4);  
        fprintf(fp, "%10d\n",SysSet16.m_nSet5); 
        fprintf(fp, "%10d\n",SysSet16.m_nSet6); 
        fprintf(fp, "%10d\n",SysSet16.m_nSet7);
        fprintf(fp, "%10d\n",SysSet16.m_nSet8);   
        fprintf(fp, "%10d\n",SysSet16.m_nSet9);
        fprintf(fp, "%10d\n",SysSet16.m_nSet10);        
        fprintf(fp, "%10d\n",SysSet16.m_nSet11);   
        fprintf(fp, "%10d\n",SysSet16.m_nSet12);   
        fprintf(fp, "%10d\n",SysSet16.m_nSet13);  
        fprintf(fp, "%10d\n",SysSet16.m_nSet14);  
        fprintf(fp, "%10d\n",SysSet16.m_nSet15); 
        fprintf(fp, "%10d\n",SysSet16.m_nSet16); 
        fprintf(fp, "%10d\n",SysSet16.m_nSet17);
        fprintf(fp, "%10d\n",SysSet16.m_nSet18);   
        fprintf(fp, "%10d\n",SysSet16.m_nSet19);
        fprintf(fp, "%10d\n",SysSet16.m_nSet20);        
        fprintf(fp, "%10d\n",SysSet16.m_nSet21); //sylee230426-2
	    fprintf(fp, "%10d\n",SysSet16.m_nSet22); //sylee230430
	    fprintf(fp, "%10d\n",SysSet16.m_nSet23); //sylee230430

        break;

    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
            g_CheckFlag1=2;
            return 0;
        }

        fscanf(fp, "%10d\n", &SysSet16.m_nSet1);        
        fscanf(fp, "%10d\n", &SysSet16.m_nSet2); 
        fscanf(fp, "%10d\n", &SysSet16.m_nSet3);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet4);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet5);   
        fscanf(fp, "%10d\n", &SysSet16.m_nSet6);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet7);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet8);        
        fscanf(fp, "%10d\n", &SysSet16.m_nSet9);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet10);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet11);       
        fscanf(fp, "%10d\n", &SysSet16.m_nSet12); 
        fscanf(fp, "%10d\n", &SysSet16.m_nSet13);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet14);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet15);   
        fscanf(fp, "%10d\n", &SysSet16.m_nSet16);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet17);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet18);       
        fscanf(fp, "%10d\n", &SysSet16.m_nSet19);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet20);
        fscanf(fp, "%10d\n", &SysSet16.m_nSet21);//sylee230426-2
        fscanf(fp, "%10d\n", &SysSet16.m_nSet22);//sylee230430
        fscanf(fp, "%10d\n", &SysSet16.m_nSet23);//sylee230430

 


        /////////////////////////////////////////////////////////////////////
        break;


    default :
        break;
    }

    fclose(fp); 


    return 1;
}


//SYLEE150407
int CFileSysInfo::LoadSaveSet19(int type)
{
    
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\SETUP\\SysSet19.ini", SysInfoView01.m_pStrFilePath1); //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat( fName , str ); //son201118  //son210514

    g_CheckFlag1=0;

    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
 
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n %s file Data Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        } 

        if(SysSet19.m_nSet10<0){//sylee190116-1
            SysSet19.m_nSet10=0;
        }
        if(SysSet19.m_nSet10>100){//sylee190116-1
            SysSet19.m_nSet10=0;
        }
        
        if(SysSet19.m_nSet31>50){
            SysSet19.m_nSet31=0;
        }
        if(SysSet19.m_nSet31>100){
            SysSet19.m_nSet31=0;
        }
        if(SysSet19.m_nSet34<=1){//sylee190116
            SysSet19.m_nSet34=1;            
        }//( mS)  1-10
        if(SysSet19.m_nSet34>1000){//sylee190116
            SysSet19.m_nSet34=1000;         
        }
        if(SysSet19.m_nSet35<=1){//sylee190116
            SysSet19.m_nSet35=1;            
        }// (count)  1-5000
        if(SysSet19.m_nSet35>10000){//sylee190116
            SysSet19.m_nSet35=10000;            
        }
        if(SysSet19.m_nSet36<=1){//sylee190116
            SysSet19.m_nSet36=0;    //sylee200520-4W정도개선: 4W Level1으로 용도변경
        }//(max pin) 1-   16384
        if(SysSet19.m_nSet36>16384){//sylee190116
            SysSet19.m_nSet36=16384;            
        }

        if(SysSet19.m_nSet38<=1){//sylee201027
            SysSet19.m_nSet38=0;            
        }
        if(SysSet19.m_nSet36>10){//sylee201027
            SysSet19.m_nSet36=10;           
        }       
        
        fprintf(fp, "%10d\n",SysSet19.m_nSet1);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet2);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet3);  
        fprintf(fp, "%10d\n",SysSet19.m_nSet4);  
        fprintf(fp, "%10d\n",SysSet19.m_nSet5);   
        fprintf(fp, "%10.1f\n",SysSet19.m_nSet6); //sylee170313
        fprintf(fp, "%10d\n",SysSet19.m_nSet7);
        fprintf(fp, "%10d\n",SysSet19.m_nSet8);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet9);
        fprintf(fp, "%10d\n",SysSet19.m_nSet10);        
        fprintf(fp, "%10d\n",SysSet19.m_nSet11);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet12);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet13);  
        fprintf(fp, "%10d\n",SysSet19.m_nSet14);  
        fprintf(fp, "%10d\n",SysSet19.m_n4WHVon); //sylee200520-4W정도개선
        fprintf(fp, "%10d\n",SysSet19.m_nSet16); 
        fprintf(fp, "%10d\n",SysSet19.m_nSet17);
        fprintf(fp, "%10d\n",SysSet19.m_nSet18);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet19);
        fprintf(fp, "%10d\n",SysSet19.m_nSet20);        
        fprintf(fp, "%10d\n",SysSet19.m_nSet21);
        fprintf(fp, "%10d\n",SysSet19.m_nSet22); 
        fprintf(fp, "%10d\n",SysSet19.m_nSet23); //sylee171027

		if( (SysSet19.m_nSet25<0) || (SysSet19.m_nSet25>100) ){//sylee220705
			SysSet19.m_nSet25=0;
		}
        fprintf(fp, "%10d\n",SysSet19.m_nSet25); //sylee171027
        fprintf(fp, "%10d\n",SysSet19.m_nSet31);//sylee180706-2
        fprintf(fp, "%10d\n",SysSet19.m_nSet32);   
        fprintf(fp, "%10d\n",SysSet19.m_nSet33);
        fprintf(fp, "%10d\n",SysSet19.m_nSet34);        
        fprintf(fp, "%10d\n",SysSet19.m_nSet35);
        fprintf(fp, "%10d\n",SysSet19.m_nSet36); 
        fprintf(fp, "%10d\n",SysSet19.m_nSet37);  

		if( (SysSet19.m_nSet38<0) || (SysSet19.m_nSet38>100) ){//sylee220705
			SysSet19.m_nSet38=0;
		}
        fprintf(fp, "%10d\n",SysSet19.m_nSet38); //sylee201027
        fprintf(fp, "%10d\n",SysSet19.m_nSet39); //sylee201027
	    fprintf(fp, "%10d\n",SysSet19.m_nSet46);//sylee230818		

        break;



    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
            g_CheckFlag1=2;
            return 0;
        }

        fscanf(fp, "%10d\n", &SysSet19.m_nSet1);        
        fscanf(fp, "%10d\n", &SysSet19.m_nSet2); 
        fscanf(fp, "%10d\n", &SysSet19.m_nSet3);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet4);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet5);  

        SysSet19.m_nSet6=0.0;        
        fscanf(fp, "%s ", str);     
        SysSet19.m_nSet6=atof(str); 

        fscanf(fp, "%10d\n", &SysSet19.m_nSet7);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet8);        
        fscanf(fp, "%10d\n", &SysSet19.m_nSet9);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet10);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet11);       
        fscanf(fp, "%10d\n", &SysSet19.m_nSet12); 
        fscanf(fp, "%10d\n", &SysSet19.m_nSet13);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet14);
        fscanf(fp, "%10d\n", &SysSet19.m_n4WHVon); //sylee200520-4W정도개선   
        fscanf(fp, "%10d\n", &SysSet19.m_nSet16);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet17);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet18);       
        fscanf(fp, "%10d\n", &SysSet19.m_nSet19);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet20);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet21);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet22);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet23);//sylee171027
        fscanf(fp, "%10d\n", &SysSet19.m_nSet25);//sylee171027
        fscanf(fp, "%10d\n", &SysSet19.m_nSet31);  //sylee180706-2
        fscanf(fp, "%10d\n", &SysSet19.m_nSet32);   
        fscanf(fp, "%10d\n", &SysSet19.m_nSet33);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet34);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet35);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet36);
        fscanf(fp, "%10d\n", &SysSet19.m_nSet37); 
        fscanf(fp, "%10d\n", &SysSet19.m_nSet38); //sylee201027
        fscanf(fp, "%10d\n", &SysSet19.m_nSet39);//sylee201027
        fscanf(fp, "%10d\n", &SysSet19.m_nSet46);  //sylee230818
 
        
        if(SysSet19.m_nSet10<0){//sylee190116-1
            SysSet19.m_nSet10=0;
        }
        if(SysSet19.m_nSet10>100){//sylee190116-1
            SysSet19.m_nSet10=0;
        }

        if(SysSet19.m_nSet31>50){
            SysSet19.m_nSet31=0;
        }
        if(SysSet19.m_nSet32>100){
            SysSet19.m_nSet32=0;
        }
        if(SysSet19.m_nSet34<=1){//sylee190116
            SysSet19.m_nSet34=1;            
        }//( mS)  1-10
        if(SysSet19.m_nSet34>1000){//sylee190116
            SysSet19.m_nSet34=1000;         
        }
        if(SysSet19.m_nSet35<=1){//sylee190116
            SysSet19.m_nSet35=1;            
        }// (count)  1-5000
        if(SysSet19.m_nSet35>10000){//sylee190116
            SysSet19.m_nSet35=10000;            
        }
        if(SysSet19.m_nSet36<=1){//sylee190116
            SysSet19.m_nSet36=0;    //sylee200520-4W정도개선: 4W Level1으로 용도변경
        }//(max pin) 1-   16384
        if(SysSet19.m_nSet36>16384){//sylee190116
            SysSet19.m_nSet36=16384;            
        }
	 
        /////////////////////////////////////////////////////////////////////
        break;

    default :
        break;
    }

    fclose(fp); 
/////////////////////////////////////////////////////////////////////////////////////////////////
//SYLEE230418
#ifdef __NEW_4W_230420      //son230519

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\SETUP\\SysSet19-2.ini", SysInfoView01.m_pStrFilePath1); //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat( fName , str ); //son201118  //son210514


    switch(type)
    {
        case _SAVE: //1: SAVE       

            fp = fopen(fName,"wt");
            if(fp == NULL){ 
                str.Format(" Error No 2001 !\n %s file Data Saving Error !     file close!", fName); //error messege
                AfxMessageBox(str, MB_OK);
                // return 0;
            }else{ 

                fprintf(fp, "%10d\n",SysSet19.m_n4W_Data_Flag1); //sylee230418
                fprintf(fp, "%10d\n",SysSet19.m_n4W_Data_Flag2);
                fprintf(fp, "%10d\n",SysSet19.m_n4W_Data_Flag3); 
                fprintf(fp, "%10d\n",SysSet19.m_nSet41); 
                fprintf(fp, "%10d\n",SysSet19.m_nSet42); 
                fprintf(fp, "%10d\n",SysSet19.m_nSet43); 
                fprintf(fp, "%10d\n",SysSet19.m_nSet44); 
                fprintf(fp, "%10d\n",SysSet19.m_nSet45);  
                fprintf(fp, "%10d\n",SysSet19.m_n4W_ContactCheck1);  
                fprintf(fp, "%10d\n",SysSet19.m_nSet46);//sylee230818
                fclose(fp); //sylee230418
            }


            break;

        case _LOAD: //2: LOAD   


            fp=fopen(fName,"rt");
            if(fp == NULL){ 
                str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
                AfxMessageBox(str, MB_OK);
                g_CheckFlag1=2;
                //return 0;
            }else{

                fscanf(fp, "%10d\n", &SysSet19.m_n4W_Data_Flag1);   //sylee230418
                fscanf(fp, "%10d\n", &SysSet19.m_n4W_Data_Flag2); 
                fscanf(fp, "%10d\n", &SysSet19.m_n4W_Data_Flag3);
                fscanf(fp, "%10d\n", &SysSet19.m_nSet41); 
                fscanf(fp, "%10d\n", &SysSet19.m_nSet42); 
                fscanf(fp, "%10d\n", &SysSet19.m_nSet43); 
                fscanf(fp, "%10d\n", &SysSet19.m_nSet44); 
                fscanf(fp, "%10d\n", &SysSet19.m_nSet45); 
                fscanf(fp, "%10d\n", &SysSet19.m_n4W_ContactCheck1);		
                fscanf(fp, "%10d\n", &SysSet19.m_nSet46);  //sylee230818
                fclose(fp); //sylee230418

            }

            /////////////////////////////////////////////////////////////////////
            break;

        default :
            break;
    }
#endif

    return 1;
}





int CFileSysInfo::LoadSaveSet17(int type)//sylee130805
{
    
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName)); 
    str.Format("%s\\SETUP\\SysSet17.ini", SysInfoView01.m_pStrFilePath1);   //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName , str); //son201118     //son210514
 
    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
 
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n %s file Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }   
        fprintf(fp, "%10d\n",SysSet17.m_nSet1);   
        fprintf(fp, "%10d\n",SysSet17.m_nSet2);   
        fprintf(fp, "%10d\n",SysSet17.m_nSet3);         
        fprintf(fp, "%10d\n",SysSet17.m_nSet11);        
        break;

    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }
        fscanf(fp, "%10d\n", &SysSet17.m_nSet1);  //loop count      
        fscanf(fp, "%10d\n", &SysSet17.m_nSet2); //delay
        fscanf(fp, "%10d\n", &SysSet17.m_nSet3); //short error set
        fscanf(fp, "%10d\n", &SysSet17.m_nSet11); 
        break;

    default :
        break;
    }

    fclose(fp); 

    return 1;
}




int CFileSysInfo::LoadSaveSet211(int type)//sylee160524-1
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;
 
    ::ZeroMemory(&fName, sizeof(fName)); 
    str.Format("%s\\SETUP\\SysSet211.ini", SysInfoView01.m_pStrFilePath1);  //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName, str); //son20111)    //son2105148

    if(dSheetRetestUse==0){    //sylee171020 
       SysSet211.m_nSet16=0;
    }
    if(SysSet13.m_nSet12==1){//sylee180206
        SysSet211.m_nSet16=0;
    }
 
    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
 
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n  %s file Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }           
        fprintf(fp, "%10d\n",SysSet211.m_nDisable4wRetest);    //son220503 m_nSet11 -> m_nDisable4wRetest 
        fprintf(fp, "%10d\n",SysSet211.m_nSet12);  
        fprintf(fp, "%10d\n",SysSet211.m_nSet13);  //syee160907
        fprintf(fp, "%10d\n",SysSet211.m_nSet14);  //syee160907 
        fprintf(fp, "%10d\n",SysSet211.m_nSet15);  //syee161018
        fprintf(fp, "%10d\n",SysSet211.m_nSet16);  //syee171011-1
        fprintf(fp, "%10d\n",SysSet211.m_nSet17);  //syee171011-1

        if(SysSet211.m_nSet21!=1){
            SysSet211.m_nSet21=0;
        }
        if(SysSet211.m_nSet22!=1){
            SysSet211.m_nSet22=0;
        } 
//      SysSet211.m_nSet23=0;
//sylee181016-1 SysSet211.m_nSet24=0;
//SYLEE181025-1 SysSet211.m_nSet25=0;
//sylee200919   SysSet211.m_nSet26=0;
        SysSet211.m_nSet27=0;
        fprintf(fp, "%10d\n",SysSet211.m_nSet21); 
        fprintf(fp, "%10d\n",SysSet211.m_nSet22);  
        fprintf(fp, "%10d\n",SysSet211.m_nSet23);  //syee160907
        fprintf(fp, "%10d\n",SysSet211.m_nSet24);  //syee160907 
        fprintf(fp, "%10d\n",SysSet211.m_nSet25);  //syee161018
        fprintf(fp, "%10d\n",SysSet211.m_nSet26);  //syee171011-1
        fprintf(fp, "%10d\n",SysSet211.m_nSet27);  //syee171011-1
        fprintf(fp, "%10d\n",SysSet211.m_wPrintState);  //son230720

        break;

    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }
 
        fscanf(fp, "%10d\n", &SysSet211.m_nDisable4wRetest);    //son220503 m_nSet11 -> m_nDisable4wRetest
        fscanf(fp, "%10d\n", &SysSet211.m_nSet12); 
        fscanf(fp, "%10d\n", &SysSet211.m_nSet13); 
        fscanf(fp, "%10d\n", &SysSet211.m_nSet14); 
        fscanf(fp, "%10d\n", &SysSet211.m_nSet15); //sylee161018
        fscanf(fp, "%10d\n", &SysSet211.m_nSet16);  //syee171011-1
        fscanf(fp, "%10d\n", &SysSet211.m_nSet17);  //syee171011-1

        fscanf(fp, "%10d\n", &SysSet211.m_nSet21);
        fscanf(fp, "%10d\n", &SysSet211.m_nSet22); 
        fscanf(fp, "%10d\n", &SysSet211.m_nSet23); 
        fscanf(fp, "%10d\n", &SysSet211.m_nSet24); 
        fscanf(fp, "%10d\n", &SysSet211.m_nSet25);  
        fscanf(fp, "%10d\n", &SysSet211.m_nSet26);  
        fscanf(fp, "%10d\n", &SysSet211.m_nSet27); 
        SysSet211.m_wPrintState = g_wPrintState;        //son230720
        fscanf(fp, "%10d\n", &SysSet211.m_wPrintState); //son230720 
        g_wPrintState = SysSet211.m_wPrintState;        //son230728

        if(SysSet211.m_nSet21!=1){
            SysSet211.m_nSet21=0;
        }
        if(SysSet211.m_nSet22!=1){
            SysSet211.m_nSet22=0;
        } 
//      SysSet211.m_nSet23=0;
//sylee181016-1 SysSet211.m_nSet24=0;
//SYLEE181025-1     SysSet211.m_nSet25=0;
//sylee200919       SysSet211.m_nSet26=0;
        SysSet211.m_nSet27=0;

        break;


    default :
        break;
    }

    fclose(fp); 


   if(dSheetRetestUse==0){    //sylee171020 
       SysSet211.m_nSet16=0;
   }
    if(SysSet13.m_nSet12==1){//sylee180206
        SysSet211.m_nSet16=0;
    }

    return 1;
}


int CFileSysInfo::SaveInfo_Block1()//sylee180209-1
{
    
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;
 
    ::ZeroMemory(&fName, sizeof(fName)); 
    strcat( fName , "Z:\\BLOCK.TXT" );    
 
 
    fp = fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format(" Error No 2001 !\n   %s file Saving Error ! \n\n\n\n    Z: Network driver connection  Check!,  \n\n\n\n      file close!", fName); //error messege
        AfxMessageBox(str, MB_OK);
        return 0;
    }     

    fprintf(fp, "%d\n",SysInfoBlock.m_nCol);
    fprintf(fp, "%d\n",SysInfoBlock.m_nRow);

    fclose(fp); 

    return 1;
}


int CFileSysInfo::SaveInfo_Piece1(int type)//sylee130911
{
    
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;
 
   ::ZeroMemory(&fName, sizeof(fName)); 
    strcat( fName , "Z:\\PIECE.TXT" );
 
    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           

            fp = fopen(fName,"wt");
            if(fp == NULL){ 
                str.Format(" Error No 2001 !\n   %s file Data Saving Error ! \n\n\n\n    Z: Network driver connection  Check!,  \n\n\n\n      file close!", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            if(SysInfoPiece.m_nType==5){
                fprintf(fp, "2\n");
            }
            else{
                fprintf(fp, "1\n");
            }

            fprintf(fp, "%d\n",SysInfoPiece.m_nCol);
            fprintf(fp, "%d\n",SysInfoPiece.m_nRow);                
            break;

        default :
            break;
    }

    fclose(fp); 
    return 1;
}


//son "C:\\ACE400\\SETUP\\FilePathJig.ini"  샘플
//
//son C:\ACE400\BDL\036901A-2                       // SysInfoView01.m_pStrFileDri1      : ACE400 수행한 폴더
//son C:\ACE400                                     // SysInfoView01.m_pStrFilePath1     : C:ACE400 위치
//son C:\ACE400\BDL\036901A-2\036901A-2.BDL         // SysInfoView01.m_pStrFilePathJig1  : 현재 BDL full path
//son C:\ACE400\BDL\036901A-2\036901A-2.BDL         // SysInfoView01.m_pStrFilePathJig2  : 현재 BDL full path(.BDL까지 포함)
//son C:\ACE400\BDL\036901A-2                       // SysInfoView01.m_pStrFilePathBDL   : 현재 BDL 폴더 path
//son 036901A-2.BDL                                 // SysInfoView01.m_pStrFileBDLName   : 현재 BDL file name(.BDL only)

//son "C:\ACE400\SETUP\FilePathJig.ini"를 struct CSysInfoView01에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadSaveView01(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str, strbuf;

    ::ZeroMemory(&fName, sizeof(fName));
#if defined(__ACE500__) || defined(__ACE550__) //son231205 | => || 로 오류 수정
    strcat( fName , "C:\\ACE500\\SETUP\\FilePathJig.ini" ); //son201118 
#elif defined(__ACE1000__)
    strcat( fName , "C:\\ACE1000\\SETUP\\FilePathJig.ini" ); //son210514    ACE1000 추가
#else
    strcat( fName , "C:\\ACE400\\SETUP\\FilePathJig.ini" ); //sylee121202 
#endif

    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
            fp = fopen(fName,"wt");
            if(fp == NULL){ 
                str.Format(" Error No 2002 !\n %s file Data Saving Error !     file close!", fName); //error messege            
                AfxMessageBox(str, MB_OK);
                return 0;
            }   
            fprintf(fp, "%s\n",SysInfoView01.m_pStrFileDri1);  //set path file
            ::ZeroMemory(&SysInfoView01.m_pStrFilePath1, sizeof(SysInfoView01.m_pStrFilePath1)); //sylee180906 
#if defined(__ACE500__) || defined(__ACE550__) //son231205 | => || 로 오류 수정
            strcat( SysInfoView01.m_pStrFilePath1 , "C:\\ACE500" ); //son201118
#elif defined(__ACE1000__)
            strcat( SysInfoView01.m_pStrFilePath1 , "C:\\ACE1000" ); //son210514    ACE10000 추가
#else
            strcat( SysInfoView01.m_pStrFilePath1 , "C:\\ACE400" ); //sylee180906 
#endif

            fprintf(fp, "%s\n",SysInfoView01.m_pStrFilePath1);  //set path file
            fprintf(fp, "%s\n",SysInfoView01.m_pStrFilePathJig1);  //set path file
            fprintf(fp, "%s\n",SysInfoView01.m_pStrFilePathJig2);  //set path file
            fprintf(fp, "%s\n",SysInfoView01.m_pStrFilePathBDL);  //SYLEE121202     //son C:BDL\xxxx  선택한 BDL 폴더
            fprintf(fp, "%s\n",SysInfoView01.m_pStrFileBDLName);  //SYLEE121202     //son C:BDL\xxxx  선택한 BDL file name 

            break;

        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL){ 
                str.Format(" Error No 2002 !\n %s file Data Loading Error", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            //son210330 공백이 있는 BDL Path Read 실패 문제 보완  
            //          fscanf() -> fgest() 으로 바꾸고 str 끝의 LineFeed 문자를 제외하고 저장한다. 
            char buf[GET_BUF_SIZE];     //son210524 200 -> GET_BUF_SIZE
            int no;

            ::ZeroMemory(&buf, sizeof(buf));    //son211112
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   //son 읽어온 한 줄을 CString 타입으로 변환
            no     = strbuf.Find('\n'); //son 마지막항목. str끝의 LineFeed 문자를 제외한다.
            strbuf = strbuf.Left(no);
            strcpy(SysInfoView01.m_pStrFileDri1, strbuf);

            ::ZeroMemory(&buf, sizeof(buf));    //son211112
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   //son 읽어온 한 줄을 CString 타입으로 변환
            no     = strbuf.Find('\n'); //son 마지막항목. str끝의 LineFeed 문자를 제외한다.
            strbuf = strbuf.Left(no);
            strcpy(SysInfoView01.m_pStrFilePath1, strbuf);

            ::ZeroMemory(&SysInfoView01.m_pStrFilePath1, sizeof(SysInfoView01.m_pStrFilePath1)); //sylee180906 
#if defined(__ACE500__) || defined(__ACE550__)    //son231205 | => || 로 오류 수정
            strcat( SysInfoView01.m_pStrFilePath1 , "C:\\ACE500" ); //son201118 
#elif defined(__ACE1000__)
            strcat( SysInfoView01.m_pStrFilePath1 , "C:\\ACE1000" ); //son210514 ACE10000 추가
#else
            strcat( SysInfoView01.m_pStrFilePath1 , "C:\\ACE400" ); //sylee180906 
#endif

            ::ZeroMemory(&buf, sizeof(buf));    //son211112
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   
            no     = strbuf.Find('\n'); 
            strbuf = strbuf.Left(no);
            strcpy(SysInfoView01.m_pStrFilePathJig1, strbuf);
            
            ::ZeroMemory(&buf, sizeof(buf));    //son211112
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   
            no     = strbuf.Find('\n'); //son 마지막항목. str끝의 LineFeed 문자를 제외한다.
            strbuf = strbuf.Left(no);
            strcpy(SysInfoView01.m_pStrFilePathJig2, strbuf);       //sw loading init folder

            ::ZeroMemory(&buf, sizeof(buf));    //son211112
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   
            no     = strbuf.Find('\n'); //son 마지막항목. str끝의 LineFeed 문자를 제외한다.
            strbuf = strbuf.Left(no);
            strcpy(SysInfoView01.m_pStrFilePathBDL, strbuf);        //son C:BDL\xxxx  선택한 BDL 폴더

            ::ZeroMemory(&buf, sizeof(buf));    //son211112
            fgets(buf, sizeof(buf),fp);
            strbuf.Format("%s", buf);   
            no     = strbuf.Find('\n'); //son 마지막항목. str끝의 LineFeed 문자를 제외한다.
            strbuf = strbuf.Left(no);
            strcpy(SysInfoView01.m_pStrFileBDLName, strbuf);        //son C:BDL\xxxx  선택한 BDL file name

            break;

        default :
            break;
    }
    fclose(fp); 
    return 1;
}

#if 0
//son "C:\ACE400\SETUP\ZDrivePath.ini"를 struct CSysInfoInOutPath_Hanlder에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadSaveInOutPath_Handler(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName));
#if defined(__ACE500__) || defined(__ACE550__)
    strcat( fName , "C:\\ACE500\\SETUP\\ZDrivePath.ini" ); //son201118 
#else
    strcat( fName , "C:\\ACE400\\SETUP\\ZDrivePath.ini" ); //sylee121202 
#endif

    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
            fp = fopen(fName,"wt");
            if(fp == NULL){ 
                str.Format(" Error No 2002 !\n %s file Data Saving Error !     file close!", fName); //error messege            
                AfxMessageBox(str, MB_OK);
                return 0;
            }   
            fprintf(fp, "%s\n",SysInfoInOutPath_Hanlder.m_pStrPath_Input);   
            fprintf(fp, "%s\n",SysInfoInOutPath_Hanlder.m_pStrPath_Output);  
            break;


        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL){ 
                str.Format(" Error No 2002 !\n %s file Data Loading Error", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }
            fscanf(fp, "%s\n", SysInfoInOutPath_Hanlder.m_pStrPath_Input);      
            fscanf(fp, "%s\n", SysInfoInOutPath_Hanlder.m_pStrPath_Output); 
            break;

        default :
            break;
    }
    fclose(fp); 
    return 1;
}
#endif

//son "C:\ACE500\path\DrivePath.ini"를 struct CSysInfoInOutPath_Hanlder에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadFileDrive1()//sylee201201-1
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\path\\DrivePath.ini", SysInfoView01.m_pStrFilePath1);   //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName, str); //son201118     //son210514
 
    fp=fopen(fName,"rt");
    if(fp == NULL)
    { 
        //son210115 LoadFileDrive1() 2회 호출시에 SysInfoView01.m_pStrFileDrive1 가 왜곡되는 현상 관련 초기화 코드 추가.
        ::ZeroMemory(&SysInfoView01.m_pStrFileDrive1, sizeof(SysInfoView01.m_pStrFileDrive1));

        //son "C:\ACE500\path\DrivePath.ini" 파일이 없다면 예전 기준으로 "Z:" 드라이브로 설정한다.
        strcat( SysInfoView01.m_pStrFileDrive1 ,"Z:"  );    //son210115: Z -> Z: 로 수정
        /*
        str.Format(" Error No 2002 !\n %s file Data Loading Error", fName);  
        AfxMessageBox(str, MB_OK);
        */
        return 0;
    }

    //son210115 SysInfoView01.m_pStrFileDrive1 
    //   InFolder:  DUT.ini, SkipPiece 용 (장비->계측기)  
    //             계측기에서 사용할 폴더이므로 빠르게 인식하기 위해 계측기에서 생성하고, 
    //             장비에서 네트워크 공유폴더로 연결해서 사용한다.
    //             "D:\\FromHandler" 로 "C:\ACE500\path\DrivePath.ini" 에 지정돼 있다.
    fscanf(fp, "%s\n", SysInfoView01.m_pStrFileDrive1); //son "Z:" 또는 "D:\\FromHandler"
    fclose(fp); 



    ::ZeroMemory(&fName, sizeof(fName));
    strcat( fName , "D:\\DrivePath2.ini" ); 
    fp=fopen(fName,"wt");
    if(fp == NULL){ 
        str.Format(" Error No 2002 !\n %s file Data Loading Error", fName);  
        AfxMessageBox(str, MB_OK);
        return 0;
    } 
    fprintf(fp, "%s",SysInfoView01.m_pStrFileDrive1); 
    fclose(fp); 



    return 1;
}

 
//son "\SETUP2\PieceRange.ini"를 struct CSysInfoPieceRange에 Save(type=1) 또는 Load(type=2)
int CFileSysInfo::LoadSavePieceRangeInfo(int DutNo , int type)  //sylee180810-1-DUT16
{                                                               //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 
    int  i;
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;  
    
    ::ZeroMemory(&fName, sizeof(fName));

    if(DutNo<1){
        DutNo=1;
    } 
    if(DutNo==1){
        str.Format("%s\\SETUP2\\PieceRange.ini", SysInfoView01.m_pStrFilePathBDL); 
    }
    else{
        str.Format("%s\\SETUP2\\PieceRange%d.ini", SysInfoView01.m_pStrFilePathBDL,DutNo); 
    } 
    strcat( fName ,str);


    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
        // file 삭제 
 
        fp = fopen(fName,"wt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s.  file Saving Error !  fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }

//sylee180810-1-DUT16       SysInfoPieceRange.m_nSetMode[DutNo]=1;
//sylee180810-1-DUT16        fprintf(fp, "%10d\n",SysInfoPieceRange.m_nSetMode[DutNo]);
        fprintf(fp, "0\n");//sylee180810-1-DUT16
        fprintf(fp, "%10d\n",SysInfoPieceRange.m_nPieceCount[DutNo]);
        fprintf(fp, "%10d\n",SysInfoPieceRange.m_nPinTot[DutNo]);   
        fprintf(fp, "%10d\n",SysInfoPieceRange.m_nRow[DutNo]);  
        
        for(i=0;i<SysInfoPieceRange.m_nRow[DutNo];i++){
            if(SysInfoPieceRange.m_nPin[DutNo][i][0]>SysInfoPieceRange.m_nPieceCount[DutNo]){//sylee151016
               AfxMessageBox(" Error No 2005  Piece No. Set1 Error!", MB_OK); //error messege           
            //   break;
            }
            if(SysInfoPieceRange.m_nPin[DutNo][i][0]<=0){//sylee151016
               AfxMessageBox(" Error No 2005  Piece No. Set1 Error!", MB_OK); //error messege           
            //  break;
            }
            fprintf(fp, "%10d%10d%10d\n",SysInfoPieceRange.m_nPin[DutNo][i][0],SysInfoPieceRange.m_nPin[DutNo][i][1],
                                         SysInfoPieceRange.m_nPin[DutNo][i][2]);
        }

        break;


    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s. file Data Loading Error ! ", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }
        
        //sylee180810-1-DUT16   fscanf(fp, "%10d\n", &SysInfoPieceRange.m_nSetMode[DutNo]);
        fscanf(fp, "%10d\n", &SysInfoPieceRange.m_nPieceCount[DutNo]);  //sylee180810-1-DUT16
        fscanf(fp, "%10d\n", &SysInfoPieceRange.m_nPieceCount[DutNo]); 
        fscanf(fp, "%10d\n", &SysInfoPieceRange.m_nPinTot[DutNo]);              
        fscanf(fp, "%10d\n", &SysInfoPieceRange.m_nRow[DutNo]);

        for(i=0;i<SysInfoPieceRange.m_nRow[DutNo];i++)  //fix 20? -> SysInfoPieceRange.m_nPieceCount
        {
            fscanf(fp, "%10d", &SysInfoPieceRange.m_nPin[DutNo][i][0]);         //son PieceNo   

            if(SysInfoPieceRange.m_nPin[DutNo][i][0]>SysInfoPieceRange.m_nPieceCount[DutNo]){//sylee151016
                AfxMessageBox(" Error No 2005  Piece No. Load1 Error!", MB_OK); //error messege         
                //   break;
            }
            if(SysInfoPieceRange.m_nPin[DutNo][i][0]<=0){//sylee151016
                AfxMessageBox(" Error No 2005  Piece No. Load1 Error!", MB_OK); //error messege         
                //  break;
            }

            fscanf(fp, "%10d", &SysInfoPieceRange.m_nPin[DutNo][i][1]);         //son Piece StartPIn         
            fscanf(fp, "%10d", &SysInfoPieceRange.m_nPin[DutNo][i][2]);         //son Piece EndPIn
        } 
        break;

    default :
        break;
    }
 
    fclose(fp); 

    return 1;
}



//son "C:\\ACE400\\Setup\\PieceDisplay.ini"를 SysInfo02에 Save/로딩
int CFileSysInfo::LoadSaveSub12(int type)
{
    FILE *fp;
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    int  i,k; 
    CString str;


    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\Setup\\PieceDisplay.ini", SysInfoView01.m_pStrFilePath1);   //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat( fName , str ); //son20111) //son2105148


    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
        // file 삭제 
        DeleteFile(fName);
        fp = fopen(fName,"wt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }

        fprintf(fp, "%10d\n",SysInfo02.m_nRow);  
        fprintf(fp, "%10d\n",SysInfo02.m_nCol); 
       
        for(i=0;i<SysInfo02.m_nRow;i++){
            for(k=0;k<SysInfo02.m_nCol;k++){
            fprintf(fp, "%10d",SysInfo02.m_nData[i][k]);
            }
            fprintf(fp,"\n");
        }

        break;

    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp==NULL) 
        { 
            str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }

        fscanf(fp, "%10d\n", &SysInfo02.m_nRow);
        fscanf(fp, "%10d\n", &SysInfo02.m_nCol);     

        for(i=0;i<SysInfo02.m_nRow;i++){
            for(k=0;k<SysInfo02.m_nCol;k++){
                fscanf(fp, "%10d", &SysInfo02.m_nData[i][k]);
            }
        } 
        break;

    default :
        break;
    }
    fclose(fp); 

    return 1;
}



//son BDLPath\\\SETUP2\\Block.ini 를 SysInfoBlock에 로딩하거나 Save한다.
//int CFileSysInfo::LoadSaveSub13(int type)  //son220824_5 LoadSaveSub13()  -> LoadSaveBlockInfo() 
int CFileSysInfo::LoadSaveBlockInfo(int type)   
{
 
    int  i,k;   
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE121202
    strcat( fName ,str);
    strcat( fName , "\\SETUP2\\Block.ini" );
    
    int nTotBlock;  //son220823_2
    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
            // file 삭제 
            //      DeleteFile(fName);
            fp = fopen(fName,"wt");
            if(fp == NULL) 
            { 
                str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            //son200525-4WLOG SysInfo03 -> SysInfoBlock 으로 수정함
            fprintf(fp, "%10d\n",SysInfoBlock.m_nBlock);        //son Block 갯수
            fprintf(fp, "%10d\n",SysInfoBlock.m_nRow);          //son Block Row 갯수
            fprintf(fp, "%10d\n",SysInfoBlock.m_nCol);          //son Block Col 갯수

            //son200525-4WLOG SysInfo03.m_nType->  SysInfoBlock.m_nBlockAlign으로 수정함
            fprintf(fp, "%10d\n",SysInfoBlock.m_nBlockAlign);   //son Block Align Display 정보) 

            for(i=0;i<SysInfoBlock.m_nRow;i++)
            {
                for(k=0;k<SysInfoBlock.m_nCol;k++)
                    fprintf(fp, "%10d",SysInfoBlock.m_nData[i][k]);

                fprintf(fp,"\n");
            }

            //son200525-4WLOG SysInfo03.m_nType2->  SysInfoBlock.m_nStepDirection 로 수정함
		    if(SysInfoBlock.m_nStepDirection!=2 && SysInfoBlock.m_nStepDirection!=3 && SysInfoBlock.m_nStepDirection!=4){//sylee180417-2 //sylee220901
                SysInfoBlock.m_nStepDirection=1;  
			}
            fprintf(fp, "%10d\n",SysInfoBlock.m_nStepDirection); //sylee180417-2
		   //sylee220901 if(SysInfoBlock.m_nStepDirection!=2 && SysInfoBlock.m_nStepDirection!=3 && SysInfoBlock.m_nStepDirection!=4){//sylee180417-2
           //sylee220901     SysInfoBlock.m_nStepDirection=1;  
		   //sylee220901 }

            //son200510-4WLOG
            //son 현재 설정된 Step Direction을 'Z:STEPPATH.txt 로 save하는 것은 안 된다.
            //son Manual -> Set1 에서 '8) Block XY+- : Auto Display' Enable이면 장비에서 설정한 기준으로 움직이겠다는 의미이다.
            //son 즉, 'Z:STEPPATH.txt'는 장비에서만 write해야 함.

            break;

        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            }

            //son200525-4WLOG SysInfo03 -> SysInfoBlock 으로 수정함
            fscanf(fp, "%10d\n", &SysInfoBlock.m_nBlock);        
            fscanf(fp, "%10d\n", &SysInfoBlock.m_nRow);      
            fscanf(fp, "%10d\n", &SysInfoBlock.m_nCol);           


            //son200525-4WLOG SysInfo03.m_nType->  SysInfoBlock.m_nBlockAlign으로 수정함
            fscanf(fp, "%10d\n", &SysInfoBlock.m_nBlockAlign);

            for(i=0;i<SysInfoBlock.m_nRow;i++)
            {
                for(k=0;k<SysInfoBlock.m_nCol;k++){
                    fscanf(fp, "%10d", &SysInfoBlock.m_nData[i][k]);
                }
            } 

            //son200525-4WLOG SysInfo03.m_nType2->  SysInfoBlock.m_nStepDirection 로 수정함
            fscanf(fp, "%10d\n", &SysInfoBlock.m_nStepDirection); //sylee180417-2
		  
           if(SysInfoBlock.m_nStepDirection!=2 && SysInfoBlock.m_nStepDirection!=3 && SysInfoBlock.m_nStepDirection!=4){//sylee180417-2
                SysInfoBlock.m_nStepDirection=1;
		   }

            //son200510-4WLOG
            //son Manual -> Set1 에서 '8) Block XY+- : Auto Display' Enable이면
            //    Step Direction을 'Z:STEPPATH.txt 기준으로 로딩한다.
            if(SysSet211.m_nSet21==1)       //son200510-4WLOG
            {
                //son 장비에서 실제 Step 순서 (Z:\\STEPPATH.TXT => SysInfoBlock.m_nStepDirection)를 가져 온다.
                //    이 함수 내부에서 SysInfoBlock.m_nStepDirection가 변경된다.
                FileSysInfo01.DisplayBlock_ReadMode1();//sylee180502-1  //son200510-4WLOG
            }

            if (SysInfoBlock.m_nBlock > MAX_BLOCK) //son220823_2 begin:
            { 
                str.Format(" Error No 2005-5 !\n %s file. SysInfoBlock.m_nBlock(%d) is over %d(MAX_BLOCK)", 
                        fName, SysInfoBlock.m_nBlock, MAX_BLOCK); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            } 

            nTotBlock = SysInfoBlock.m_nRow * SysInfoBlock.m_nCol; 
            if (nTotBlock > MAX_BLOCK)  
            { 
                str.Format(" Error No 2005-6 !\n %s file. nTotBlock(%d) is over %d(MAX_BLOCK)", 
                        fName, nTotBlock, MAX_BLOCK); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            }   
            if (nTotBlock != SysInfoBlock.m_nBlock)  
            { 
                str.Format(" Error No 2005-7 !\n %s file. nTotBlock(%d) is not same with SysInfoBlock.m_nBlock(%d)", 
                        fName, nTotBlock, SysInfoBlock.m_nBlock); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            }   
            //son220823_2 end

            break;

        default :
            break;
    }
    fclose(fp); 
    return 1;
}


int CFileSysInfo::LoadSaveLowerStartPin(int type, CString& rStr)//SYLEE210406    //son211214 rStr 추가
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);
    strcat( fName ,str);
    strcat( fName , "\\SETUP2\\LowerStartPin.ini" );
    
    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
     
            fp = fopen(fName,"wt");
            if(fp == NULL) 
            { 
                rStr.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName);  
                //son 에러 출력은 LoadSaveLowerStartPin() 함수 호출 지점에서 출력하는 것으로 수정.
                //son211214 AfxMessageBox(str, MB_OK);
                return 0;
            } 
//son240516 begin:
			//son range를 벗어나는 경우의 값이 있는 경우는 default로 보정하고 write한다.
			if (SysSet12.m_nUpperBox <= 0 || SysSet12.m_nUpperBox > MAX_BOX_PER_PART)
                SysSet12.m_nUpperBox = 4;   
			if (SysSet12.m_nLowerBox <= 0 || SysSet12.m_nLowerBox > MAX_BOX_PER_PART)
                SysSet12.m_nLowerBox = 4;   
            if (SysSet12.m_nEndPin <= 0 || SysSet12.m_nEndPin > MAX_PIN)                    //son240618
                SysSet12.m_nEndPin = MAX_PIN;                                               //son240618
//son240516 end.
			fprintf(fp, "%10d\n",SysSet12.m_nLowerJigStartPin);  	 
			fprintf(fp, "%10d\n",SysSet12.m_nEndPin);  	    //son240516 
			fprintf(fp, "%10d\n",SysSet12.m_nUpperBox);  	//son240516 
			fprintf(fp, "%10d\n",SysSet12.m_nLowerBox);  	//son240516 	      
			fprintf(fp, "%10d\n",SysSet12.m_nLowerJigStartPin_Offset);  	//son241007-BSW-9
            break;

        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                rStr.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
                //son 에러 출력은 LoadSaveLowerStartPin() 함수 호출 지점에서 출력하는 것으로 수정.  
                //son211214 AfxMessageBox(rStr, MB_OK);
                return 0;
            }          
			fscanf(fp, "%10d\n", &SysSet12.m_nLowerJigStartPin);	     
			fscanf(fp, "%10d\n", &SysSet12.m_nEndPin);	    //son240516 
			fscanf(fp, "%10d\n", &SysSet12.m_nUpperBox);	//son240516 
			fscanf(fp, "%10d\n", &SysSet12.m_nLowerBox);	//son240516 
			fscanf(fp, "%10d\n", &SysSet12.m_nLowerJigStartPin_Offset);	//son240516   //son241124-BSW-9
//son240516 begin:
			//son range를 벗어나는 경우의 값이 있는 경우는 default로 보정해서 사용한다.
			if (SysSet12.m_nUpperBox <= 0 || SysSet12.m_nUpperBox > MAX_BOX_PER_PART)
                SysSet12.m_nUpperBox = 4;   
			if (SysSet12.m_nLowerBox <= 0 || SysSet12.m_nLowerBox > MAX_BOX_PER_PART)
                SysSet12.m_nLowerBox = 4;   
//son240516 end.	     
            if (SysSet12.m_nEndPin <= 0 || SysSet12.m_nEndPin > MAX_PIN)                    //son240618
                SysSet12.m_nEndPin = MAX_PIN;                                               //son240618
            if (SysSet12.m_nLowerJigStartPin <= 0 || SysSet12.m_nLowerJigStartPin > MAX_PIN)//son240618
                SysSet12.m_nLowerJigStartPin = MAX_PIN/2 + 1;    //son 16385                //son240618

            break;

        default :
            break;
    }
    fclose(fp); 
    return 1;
}


//son200510-4WLOG  
// ChildView7->FileSysInfo.cpp로 DisplayBlock_ReadMode1() 이동.  
//  FileSysInfo 내부 LoadSaveBlockInfo() 내부에서 호출할 수 있도록 전역함수로 변경.

//son 장비에서 실제 Step 순서 (Z:\\STEPPATH.TXT => SysInfoBlock.m_nStepDirection)를 가져 온다.
int CFileSysInfo::DisplayBlock_ReadMode1()  //sylee180502-1
{
    FILE *fp;  
    //char *fName;    //son210115 fName이 포인터만 잡혀 있는데 아래에서 초기화하다가 죽는 현상 수정.
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    char str[5];

    //fName="Z:\\STEPPATH.TXT";     //sylee201201-1 
    ::ZeroMemory(&fName, sizeof(fName));//sylee201201-1 
    strcat( fName , SysInfoView01.m_pStrFileDrive1 );//sylee201201-1 
    strcat( fName , "\\STEPPATH.TXT" );//sylee201201-1 

    fp = fopen(fName,"rt"); 
    if(fp == NULL){ 
        //  AfxMessageBox(" ERROR 7501  file read error . Z:\\STEPPATH.TXT   ", MB_OK | MB_ICONSTOP); 
        if( SysInfoBlock.m_nStepDirection<1 || SysInfoBlock.m_nStepDirection>4){
            SysInfoBlock.m_nStepDirection=1;
        }
        return 1;
    }
    ::ZeroMemory(&str, sizeof(str));
    fscanf(fp, "%s\n", str);    

    if(str[0]=='A'){    
        SysInfoBlock.m_nStepDirection=1;
    }
    else if(str[0]=='B'){
        SysInfoBlock.m_nStepDirection=2;
    }
    else if(str[0]=='C'){
        SysInfoBlock.m_nStepDirection=3;
    }
    else if(str[0]=='D'){
        SysInfoBlock.m_nStepDirection=4;
    }
    else{
        if( SysInfoBlock.m_nStepDirection<1 || SysInfoBlock.m_nStepDirection>4){
            SysInfoBlock.m_nStepDirection=1;
        }
    }
    fclose(fp);     

    return 1;
}





//son "\SETUP2\Recipe.ini"를 struct CSysInfo05에 Save(type=1) 또는 Load(type=2)
int CFileSysInfo::LoadSaveSub15(int type)
{


    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 
    int i; 

    ::ZeroMemory(&fName, sizeof(fName));
 
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE160720
    strcat( fName ,str);//SYLEE160720
    strcat( fName , "\\SETUP2\\Recipe.ini" );//SYLEE160720

    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           

            fp = fopen(fName,"wt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fprintf(fp, "%10d\n",SysInfo05.m_nShortDisable1); //sylee160518   m_nShortDisable1; //sylee180331-1      
            fprintf(fp, "%10d\n",SysInfo05.m_nCon);
            fprintf(fp, "%10d\n",SysInfo05.m_nConU);     
            fprintf(fp, "%10d\n",SysInfo05.m_nHv1R);        //son210324 m_nConIs-> m_nHv1R
            fprintf(fp, "%10d\n",SysInfo05.m_nHv1RU);       //son210324 m_nConIsU-> m_nHv1RU
            fprintf(fp, "%10d\n",SysInfo05.m_nHv);
            fprintf(fp, "%10d\n",SysInfo05.m_nMs);
            fprintf(fp, "%10d\n",SysInfo05.m_nSp);   
            fprintf(fp, "%10d\n",SysInfo05.m_nPr);  
            //      fprintf(fp, "%10d\n",SysInfo05.m_nDeT);  
            fprintf(fp, "%10d\n",SysInfo05.m_nConEr);   
            fprintf(fp, "%10d\n",SysInfo05.m_nIsEr);
            fprintf(fp, "%10d\n",SysInfo05.m_nConErSt);  
            fprintf(fp, "%10d\n",SysInfo05.m_nReCh);
            //  fprintf(fp, "%10d\n",SysInfo05.m_nCorR);  
            fprintf(fp, "%10d\n",SysInfo05.m_nConFail);  
            fprintf(fp, "%10d\n",SysInfo05.m_nHR);
            fprintf(fp, "%10d\n",SysInfo05.m_nHRL);
            fprintf(fp, "%10d\n",SysInfo05.m_nDelay1);
            fprintf(fp, "%10d\n",SysInfo05.m_nShortR);
            fprintf(fp, "%10d\n",SysInfo05.m_nShortRU);
            fprintf(fp, "%10d\n",SysInfo05.m_nShortR2);
            fprintf(fp, "%10d\n",SysInfo05.m_nShortR2U);

            fprintf(fp, "%10d\n",SysInfo05.m_nOpenV);
            fprintf(fp, "%10d\n",SysInfo05.m_nShort1V);
            fprintf(fp, "%10d\n",SysInfo05.m_nShort2V);
            fprintf(fp, "%10d\n",SysInfo05.m_nHRV);

            if(SysInfo05.m_n4WErrPerLimit<10){//sylee170915-1
                SysInfo05.m_n4WErrPerLimit=0;
            }
            if(SysInfo05.m_n4WErrPerLimit>10000){//sylee170915-1
                SysInfo05.m_n4WErrPerLimit=10000;
            }

            fprintf(fp, "%10d\n",SysInfo05.m_n4WErrPerLimit);//sylee170915-1


            fprintf(fp, "%10d\n",SysInfo05.m_nProSpMode);//sylee131221
            //sylee131221 fprintf(fp, "%10d\n",SysInfo05.m_nOpRetry);

#if 0
            for(i=1; i<6; i++){
                fprintf(fp, "%10d\n",SysInfo05.m_nCC[i]);
            }
#else
            fprintf(fp, "%10d\n",SysInfo05.m_nLeak1_Current);   //son220706 m_nCC[1] -> m_nLeak1_Current
            fprintf(fp, "%10d\n",SysInfo05.m_nLeak2_Current);   //son220706 m_nCC[2] -> m_nLeak2_Current
            fprintf(fp, "%10d\n",SysInfo05.m_nCC[3]);
            fprintf(fp, "%10d\n",SysInfo05.m_nCC[4]);
            fprintf(fp, "%10d\n",SysInfo05.m_nShort_Current);   //son220706 m_nCC[5] -> m_nShort_Current

#endif
            for(i=1; i<8; i++){
                fprintf(fp, "%10d\n",SysInfo05.m_nStep[i]);
            }

#ifdef __GTS__CUSTOMER 	
            SysInfo05.m_nOpenMea1=0;	//son220613 GTS는 SysInfo05.m_nOpenMea1는 무조건 0으로 설정. 기능 Off. 
#endif

//#ifndef __CCTC__CUSTOMER        //son230331 CCTC만 AC Leak 옵션이 1이 될 수 있도록 수정(사장님 지시)
//sylee241204 #if !defined(__CCTC__CUSTOMER) && !defined(__YOUNPUNG__CUSTOMER) && !defined(__YP_VIET_CUSTOMER)	// hdpark231205

#if !defined(__CCTC__CUSTOMER) && !defined(__YOUNPUNG__CUSTOMER) && !defined(__YP_VIET_CUSTOMER) && !defined(__BH__CUSTOMER) 	// hdpark231205  //sylee241204 

			SysInfo05.m_nAC=0;	//son220613 GTS가 아니면  SysInfo05.m_nAC는 무조건 0으로 설정. 기능 Off. 
#endif

            fprintf(fp, "%10d\n",SysInfo05.m_nOpenErrorLimit);
            fprintf(fp, "%10d\n",SysInfo05.m_nOpenShortLimit);
            fprintf(fp, "%10d\n",SysInfo05.m_nPer3);
            fprintf(fp, "%10d\n",SysInfo05.m_nPer4);
            fprintf(fp, "%10d\n",SysInfo05.m_nPer5);

            fprintf(fp, "%10d\n",SysInfo05.m_n4WSet1);
            fprintf(fp, "%10d\n",SysInfo05.m_nHVCal1);
            fprintf(fp, "%10d\n",SysInfo05.m_nCycle);
            fprintf(fp, "%10d\n",SysInfo05.m_nOpenMea1);	//sylee211122 m_nUniversal->m_nOpenMea1
            fprintf(fp, "%10d\n",SysInfo05.m_4W_Measure);   //sylee200520-4W정도개선

			if( (SysInfo05.m_nSparkSet_B<0) || (SysInfo05.m_nSparkSet_B>100) ){//sylee220705
				SysInfo05.m_nSparkSet_B=0;
			}
            fprintf(fp, "%10d\n",SysInfo05.m_nSparkSet_B);//sylee200709-1-JTECH NEW_SPARK
            fprintf(fp, "%10d\n",SysInfo05.m_nS6);
            fprintf(fp, "%10d\n",SysInfo05.m_nS7);
            fprintf(fp, "%10d\n",SysInfo05.m_nS8);
            fprintf(fp, "%10d\n",SysInfo05.m_nS9);
            fprintf(fp, "%10d\n",SysInfo05.m_nSparkSet);

            fprintf(fp, "%10d\n",SysInfo05.m_nHv2);  //sylee140923
            fprintf(fp, "%10d\n",SysInfo05.m_nHv2R);  //sylee140923
            fprintf(fp, "%10d\n",SysInfo05.m_nHv2RU);  //sylee140923
            //fprintf(fp, "%10d\n",SysInfo05.m_nCC[6]);   //sylee140923
            fprintf(fp, "%10d\n",SysInfo05.m_nOpen_Current);   //son220706 m_nCC[6] -> m_nOpen_Current
            fprintf(fp, "%10d\n",SysInfo05.m_nAC);   //son220613
            fprintf(fp, "%10d\n",SysInfo05.m_nSparkDetection[0]);   //hdpark230331
            fprintf(fp, "%10d\n",SysInfo05.m_nSparkDetection[1]);   //hdpark230331
            fprintf(fp, "%10d\n",SysInfo05.m_nSparkDetection[2]);   //hdpark230331
            fprintf(fp, "%10d\n",SysInfo05.m_nSparkCurrentSet);	// hdpark230223

            //son MapDisplay->Set1:  IDD_MODE_SET13_SET1 Dialog "Z:\Ultlog.csv, TestConditionLog.csv" 옵션이 1일때 
            if (SysSet13.m_nSetUltLog == 1)
                Write_TestCondionLog();     //son210324 for TestConditionLog: 삼성전기 포맷 recipe 출력


            break;


        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fscanf(fp, "%10d\n", &SysInfo05.m_nShortDisable1);//sylee160518   m_nShortDisable1; //sylee180331-1 
            fscanf(fp, "%10d\n", &SysInfo05.m_nCon);         
            fscanf(fp, "%10d\n", &SysInfo05.m_nConU);    
            fscanf(fp, "%10d\n", &SysInfo05.m_nHv1R);        //son210324 m_nConIs-> m_nHv1R
            fscanf(fp, "%10d\n", &SysInfo05.m_nHv1RU);       //son210324 m_nConIsU-> m_nHv1RU
            if( SysInfo05.m_nHv1RU> MAX_LEAK_USER_R){        SysInfo05.m_nHv1RU=MAX_LEAK_USER_R;	}   //sylee220923
            fscanf(fp, "%10d\n", &SysInfo05.m_nHv);          
            fscanf(fp, "%10d\n", &SysInfo05.m_nMs);      
            fscanf(fp, "%10d\n", &SysInfo05.m_nSp);      
            fscanf(fp, "%10d\n", &SysInfo05.m_nPr);      
            fscanf(fp, "%10d\n", &SysInfo05.m_nConEr);
            fscanf(fp, "%10d\n", &SysInfo05.m_nIsEr);        
            fscanf(fp, "%10d\n", &SysInfo05.m_nConErSt);         
            fscanf(fp, "%10d\n", &SysInfo05.m_nReCh);
            if(SysInfo05.m_nReCh>10){SysInfo05.m_nReCh=10;}//sylee190528-2
            //fscanf(fp, "%10d\n", &SysInfo05.m_nCorR);         
            fscanf(fp, "%10d\n", &SysInfo05.m_nConFail);            
            fscanf(fp, "%10d\n", &SysInfo05.m_nHR);
            fscanf(fp, "%10d\n", &SysInfo05.m_nHRL );
            fscanf(fp, "%10d\n", &SysInfo05.m_nDelay1);     
            fscanf(fp, "%10d\n", &SysInfo05.m_nShortR);
            fscanf(fp, "%10d\n", &SysInfo05.m_nShortRU);
            fscanf(fp, "%10d\n", &SysInfo05.m_nShortR2);
            fscanf(fp, "%10d\n", &SysInfo05.m_nShortR2U); 
            fscanf(fp, "%10d\n", &SysInfo05.m_nOpenV);
            fscanf(fp, "%10d\n", &SysInfo05.m_nShort1V);
            fscanf(fp, "%10d\n", &SysInfo05.m_nShort2V);
            fscanf(fp, "%10d\n", &SysInfo05.m_nHRV);

            fscanf(fp, "%10d\n", &SysInfo05.m_n4WErrPerLimit);//sylee170915-1
            if(SysInfo05.m_n4WErrPerLimit<10){//sylee170915-1
                SysInfo05.m_n4WErrPerLimit=0;
            }
            if(SysInfo05.m_n4WErrPerLimit>10000){//sylee170915-1
                SysInfo05.m_n4WErrPerLimit=10000;
            }


            fscanf(fp, "%10d\n", &SysInfo05.m_nProSpMode);//sylee131221
            //sylee131221   fscanf(fp, "%10d\n", &SysInfo05.m_nOpRetry);

#if 0
            for(i=1; i<6; i++){         
                fscanf(fp, "%10d\n", &SysInfo05.m_nCC[i]);       
            }
#else
            fscanf(fp, "%10d\n", &SysInfo05.m_nLeak1_Current);      //son220706 m_nCC[1] -> m_nLeak1_Current 
            fscanf(fp, "%10d\n", &SysInfo05.m_nLeak2_Current);      //son220706 m_nCC[2] -> m_nLeak2_Current 
            fscanf(fp, "%10d\n", &SysInfo05.m_nCC[3]);              //son Open Offset CheckBox
            fscanf(fp, "%10d\n", &SysInfo05.m_nCC[4]);              //son Open Offset Value  
            fscanf(fp, "%10d\n", &SysInfo05.m_nShort_Current);      //son220706 m_nCC[5] -> m_nShort_Current   

#endif
            for(i=1; i<8; i++){
                fscanf(fp, "%10d\n", &SysInfo05.m_nStep[i] );            
            }

            fscanf(fp, "%10d\n", &SysInfo05.m_nOpenErrorLimit); 
            fscanf(fp, "%10d\n", &SysInfo05.m_nOpenShortLimit);
            fscanf(fp, "%10d\n", &SysInfo05.m_nPer3);
            fscanf(fp, "%10d\n", &SysInfo05.m_nPer4);
            fscanf(fp, "%10d\n", &SysInfo05.m_nPer5);

            fscanf(fp, "%10d\n", &SysInfo05.m_n4WSet1); 
            fscanf(fp, "%10d\n", &SysInfo05.m_nHVCal1);
            fscanf(fp, "%10d\n", &SysInfo05.m_nCycle);
            fscanf(fp, "%10d\n", &SysInfo05.m_nOpenMea1); //sylee211122 m_nUniversal->m_nOpenMea1
            fscanf(fp, "%10d\n", &SysInfo05.m_4W_Measure);  //sylee200520-4W정도개선
            fscanf(fp, "%10d\n", &SysInfo05.m_nSparkSet_B);//sylee200709-1-JTECH  NEW_SPARK
            fscanf(fp, "%10d\n", &SysInfo05.m_nS6);
            fscanf(fp, "%10d\n", &SysInfo05.m_nS7);
            fscanf(fp, "%10d\n", &SysInfo05.m_nS8);
            fscanf(fp, "%10d\n", &SysInfo05.m_nS9);
            fscanf(fp, "%10d\n", &SysInfo05.m_nSparkSet);//sylee140430

            fscanf(fp, "%10d\n",&SysInfo05.m_nHv2);  //sylee140923
            fscanf(fp, "%10d\n",&SysInfo05.m_nHv2R);  //sylee140923
            fscanf(fp, "%10d\n",&SysInfo05.m_nHv2RU);  //sylee140923
			if( SysInfo05.m_nHv2RU> MAX_LEAK_USER_R){        SysInfo05.m_nHv2RU=MAX_LEAK_USER_R;	}//sylee220923
            //fscanf(fp, "%10d\n",&SysInfo05.m_nCC[6]);   //sylee140923
            fscanf(fp, "%10d\n",&SysInfo05.m_nOpen_Current);   //son220706 m_nCC[6] -> m_nOpen_Current
            fscanf(fp, "%10d\n",&SysInfo05.m_nAC);  //son220613
            fscanf(fp, "%10d\n",&SysInfo05.m_nSparkDetection[0]);  //hdpark230223
            fscanf(fp, "%10d\n",&SysInfo05.m_nSparkDetection[1]);  //hdpark230223
            fscanf(fp, "%10d\n",&SysInfo05.m_nSparkDetection[2]);  //hdpark230223
            fscanf(fp, "%10d\n",&SysInfo05.m_nSparkCurrentSet);  //hdpark230223

            //son MapDisplay->Set1:  IDD_MODE_SET13_SET1 Dialog "Z:\Ultlog.csv, TestConditionLog.csv" 옵션이 1일때 
            if (SysSet13.m_nSetUltLog == 1)
                LoadTestConditionData();        //son210324 for TestConditionLog

#ifdef __GTS__CUSTOMER 	
            SysInfo05.m_nOpenMea1=0;	//son220613 GTS는 SysInfo05.m_nOpenMea1는 무조건 0으로 설정. 기능 Off. 
#endif
//#ifndef __CCTC__CUSTOMER 	    //son230331 CCTC만 AC Leak 옵션이 1이 될 수 있도록 수정(사장님 지시)
#if !defined(__CCTC__CUSTOMER) && !defined(__YOUNPUNG__CUSTOMER) && !defined(__YP_VIET_CUSTOMER) && !defined(__BH__CUSTOMER)	// hdpark231205  //sylee241204-2
            SysInfo05.m_nAC=0;	//son220613 GTS가 아니면  SysInfo05.m_nAC는 무조건 0으로 설정. 기능 Off. 
#endif


            break;

        default :
            break;
    }
    fclose(fp); 
    return 1;
}



//son220901 LoadSaveSub15()에서 로드한 recipe정보를 D:\log폴더에 출력
void CFileSysInfo::printLog_Recipe()
{


    MyTrace(PRT_BASIC, "-----RECIPE-------\n");
    MyTrace(PRT_BASIC, "                       VOLTAGE     R              USER_R      Current\n");
    CList_Recipe_Open_R  recipe_OpenR;
    int nOpenVolt = (SysInfo05.m_nOpenV - 1) * 10;
    MyTrace(PRT_BASIC, "                 OPEN= %2d:%4s  %2d(%12s) %8s  %8s\n", 
                                        SysInfo05.m_nOpenV,  getStr_SysRecipe_m_nOpenV(),
                                        SysInfo05.m_nCon, recipe_OpenR.strList[SysInfo05.m_nCon], 
                                        getStr_SysRecipe_m_nCon(), 
                                        //STR_OPEN_CURR(SysInfo05.m_nOpen_Current));  //son220902
                                        getStr_OpenCurrent(SysInfo05.m_nOpenV, SysInfo05.m_nOpen_Current));  //son221227
    CList_Recipe_uShortR  recipe_uShortR;
    MyTrace(PRT_BASIC, "               uShort= %2d:%4s  %2d(%12s) %8s\n", 
                                        SysInfo05.m_nShort1V, (SysInfo05.m_nShort1V == 1) ? "1V": "Undef",
                                        SysInfo05.m_nShortR,recipe_uShortR.strList[SysInfo05.m_nShortR], 
                                        getStr_SysRecipe_m_nShortR());
    CList_Recipe_LShortR  recipe_LShortR;
    MyTrace(PRT_BASIC, "                Short= %2d:%4s  %2d(%12s) %8s  %8s\n", 
                                        SysInfo05.m_nShort2V, (SysInfo05.m_nShort2V == 1) ? "10V": "Undef", 
                                        SysInfo05.m_nShortR2, recipe_LShortR.strList[SysInfo05.m_nShortR2], 
                                        getStr_SysRecipe_m_nShortR2(), STR_SHORT_CURR(SysInfo05.m_nShort_Current));  //son220902
    CList_Recipe_HRShortR  recipe_HRShortR;
    MyTrace(PRT_BASIC, "                   HR= %2d:%4s  %2d(%12s) %8s\n", 
                                        SysInfo05.m_nHRV, (SysInfo05.m_nHRV == 1) ? "10V": "Undef",
                                        SysInfo05.m_nHR, recipe_HRShortR.strList[SysInfo05.m_nHR], 
                                        getStr_SysRecipe_m_nHR());
    CList_Recipe_LEAK_R  recipe_LeakR;
    MyTrace(PRT_BASIC, "              m_nHv1R= %2d:%4s  %2d(%12s) %8s  %8s\n", 
                                        SysInfo05.m_nHv,  getStr_SysRecipe_Leak1Voltage(),
                                        SysInfo05.m_nHv1R, recipe_LeakR.strList[SysInfo05.m_nHv1R], 
                                        getStr_SysRecipe_Leak1R(), STR_LEAK_CURR(SysInfo05.m_nLeak1_Current));  //son220902
    MyTrace(PRT_BASIC, "              m_nHv2R= %2d:%4s  %2d(%12s) %8s  %8s\n", 
                                        SysInfo05.m_nHv2, getStr_SysRecipe_Leak2Voltage(),
                                        SysInfo05.m_nHv2R, recipe_LeakR.strList[SysInfo05.m_nHv2R], 
                                        getStr_SysRecipe_Leak2R(), STR_LEAK_CURR(SysInfo05.m_nLeak2_Current));  //son220902
    MyTrace(PRT_BASIC, "\n");

    MyTrace(PRT_BASIC, "Step[]= %s %s %s %s %s %s %s \n", STR_TEST_TYPE(SysInfo05.m_nStep[1]), 
                        STR_TEST_TYPE(SysInfo05.m_nStep[2]), STR_TEST_TYPE(SysInfo05.m_nStep[3]),
                        STR_TEST_TYPE(SysInfo05.m_nStep[4]), STR_TEST_TYPE(SysInfo05.m_nStep[5]), 
                        STR_TEST_TYPE(SysInfo05.m_nStep[6]),  STR_TEST_TYPE(SysInfo05.m_nStep[7]));
    MyTrace(PRT_BASIC, "\n");

    MyTrace(PRT_BASIC, "   Open Err Max Count= %d \n", SysInfo05.m_nConEr);
    MyTrace(PRT_BASIC, "  Short Err Max Count= %d \n", SysInfo05.m_nIsEr);
    MyTrace(PRT_BASIC, "        ReCheck Count= %d \n", SysInfo05.m_nReCh);  //son240718
    MyTrace(PRT_BASIC, "     Continue Fail No= %d \n", SysInfo05.m_nConFail);
    MyTrace(PRT_BASIC, "\n");

    MyTrace(PRT_BASIC, "       Spark I (1~15)= %d \n", SysInfo05.m_nSparkSet_B);
    MyTrace(PRT_BASIC, "                Spark= %d(%s) \n", SysInfo05.m_nSp, (SysInfo05.m_nSp ? "ON":"OFF"));
    MyTrace(PRT_BASIC, "  m_nSparkSet(1~250V)= %d \n", SysInfo05.m_nSparkSet);
    MyTrace(PRT_BASIC, "     m_n4WErrPerLimit= %d \n", SysInfo05.m_n4WErrPerLimit);
    MyTrace(PRT_BASIC, "            m_n4WSet1= %d(%s) \n", SysInfo05.m_n4WSet1, STR_4W_SET(SysInfo05.m_n4WSet1));
    MyTrace(PRT_BASIC, "         m_4W_Measure= %d \n", SysInfo05.m_4W_Measure);
    MyTrace(PRT_BASIC, "            m_nDelay1= %d mSec\n", SysInfo05.m_nDelay1);
    MyTrace(PRT_BASIC, "     m_nShortDisable1= %d \n", SysInfo05.m_nShortDisable1);
    MyTrace(PRT_BASIC, " Open Offset CheckBox= %d \n", SysInfo05.m_nCC[3]);
    MyTrace(PRT_BASIC, "    Open Offset Value= %d \n", SysInfo05.m_nCC[4]);
    MyTrace(PRT_BASIC, "                m_nAC= %d \n", SysInfo05.m_nAC);
    //MyTrace(PRT_BASIC, "                m_nMs= %d \n", SysInfo05.m_nMs);
    //MyTrace(PRT_BASIC, "                m_nPr= %d \n", SysInfo05.m_nPr);
    //MyTrace(PRT_BASIC, "           m_nConErSt= %d \n", SysInfo05.m_nConErSt);
    //MyTrace(PRT_BASIC, "    m_nOpenErrorLimit= %d \n", SysInfo05.m_nOpenErrorLimit);
    //MyTrace(PRT_BASIC, "    m_nOpenShortLimit= %d \n", SysInfo05.m_nOpenShortLimit);
    //MyTrace(PRT_BASIC, "              m_nPer3= %d \n", SysInfo05.m_nPer3);
    //MyTrace(PRT_BASIC, "              m_nPer4= %d \n", SysInfo05.m_nPer4);

    //MyTrace(PRT_BASIC, "             m_nCycle= %d \n", SysInfo05.m_nCycle);
    //MyTrace(PRT_BASIC, "          m_nOpenMea1= %d \n", SysInfo05.m_nOpenMea1);
    //MyTrace(PRT_BASIC, "                m_nS6= %d \n", SysInfo05.m_nS6);
    //MyTrace(PRT_BASIC, "                m_nS7= %d \n", SysInfo05.m_nS7);
    //MyTrace(PRT_BASIC, "                m_nS8= %d \n", SysInfo05.m_nS8);
    //MyTrace(PRT_BASIC, "                m_nS9= %d \n", SysInfo05.m_nS9);

    MyTrace(PRT_BASIC, "\n");


}

//son221227 begin 
char* getStr_OpenCurrent(int nOpenVolt, int nOpenCurrent)
{
    CString str = STR_OPEN_CURR(nOpenCurrent);

//son231106_2 : 230911 SysSet21_ACE400_230911.ini 수정사항 관련 son221226, son221227, son230711 중 다음 부분 기능삭제 
//   영풍 SST에서 50V 설정시에 가오픈 다발문제 관련하여 SW와 HW를 추가 수정함.  
//   son230711중에서  하기 수정 사항은 필요 없어졌으므로  원상복구함. (23/9/11일 김영필 이사님 요청)
//son231106_2    if ((nOpenVolt == RECP_OPENV_4_30V) && (nOpenCurrent == CURR_10mA))       //son current 10mA 위치가 OPEN 30V는 3mA  //son230711
//son231106_2        str = "3mA";
//son231106_2    else if ((nOpenVolt == RECP_OPENV_5_40V) && (nOpenCurrent == CURR_10mA))  //son current 10mA 위치가 OPEN 40V는 4mA  //son230711
//son231106_2        str = "4mA";
//son231106_2    else if ((nOpenVolt == RECP_OPENV_6_50V) && (nOpenCurrent == CURR_10mA))  //son current 10mA 위치가 OPEN 50V는 5mA  //son230711
//son231106_2        str = "5mA";

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat(szStr , str); 
    return szStr;

}
//son221227 end

//son221104 Recipe의 step에 short, leak가 존재하는지 체크한다.
bool CFileSysInfo::checkRecipeStep_ShortExist()
{
    bool bShortExist = false;
    for(int i=1; i<= 7; i++)
    {
        if (SysInfo05.m_nStep[i] == TEST_U_SHORT || SysInfo05.m_nStep[i] == TEST_HR_SHORT
            || SysInfo05.m_nStep[i] == TEST_LEAK1 || SysInfo05.m_nStep[i] == TEST_LEAK2)
        {
            bShortExist = true;
        }
    }

    return bShortExist;
}




//son210324 추가
//son 1. C:\ACE500\Data\TestConditionSample.csv에서 nItemType, szName, szFixedVal을 read  
//    2.  (*funcp)() 함수 포인터에 적절한 함수를 배치
int CFileSysInfo::LoadTestConditionData()
{
    //----------------------
    //son 함수 포인터 배치

    //son 함수 포인터를 일단 모두 기본함수인 getStr_Blank()로 초기화한다.
    int i;
    for (i = 0; i < MAX_TEST_CONDITION_ITEM; i++)
    {
        g_testConditionLogData[i].funcp = getStr_Blank;		//son230707_2 .fp() -> .funcp()
    }

//son230707_2 begin: .fp() -> .funcp()
    
    //son nType이 TCONDI_TYPE_RECIPE(2)인 경우 사용할 함수 
    //    TestCondition 항목에 맞게 Recipe 참조가 필요한 경우만 함수 포인터를 다시 할당한다. 
    //    함수 포인터 대상이 되는 함수들은 모두 g_testConditionLogData[i].funcp()와 동일한 형식이어야 한다.
    g_testConditionLogData[TREC_CONDI_DATE_TIME].funcp           = getStr_DateTime;
    g_testConditionLogData[TREC_CONDI_DELAY].funcp               = getStr_SysRecipe_m_nDelay1;
    g_testConditionLogData[TREC_CONDI_CON_FAIL].funcp            = getStr_SysRecipe_m_nConFail_ON;
    g_testConditionLogData[TREC_CONDI_RECHECK].funcp             = getStr_SysRecipe_m_nReCh;
    g_testConditionLogData[TREC_CONDI_CON_ERR].funcp             = getStr_SysRecipe_m_nConEr;
    g_testConditionLogData[TREC_CONDI_CON_FAIL_CNT].funcp        = getStr_SysRecipe_m_nConFail;

    g_testConditionLogData[TREC_CONDI_BLOCK_X].funcp             = getStr_SysInfoBlock_m_nCol;
    g_testConditionLogData[TREC_CONDI_BLOCK_Y].funcp             = getStr_SysInfoBlock_m_nRow;
    g_testConditionLogData[TREC_CONDI_DUT_CNT].funcp             = getStr_nDUTCnt;

    g_testConditionLogData[TREC_CONDI_OPEN_R].funcp              = getStr_SysRecipe_m_nCon;
    g_testConditionLogData[TREC_CONDI_OPEN_V].funcp              = getStr_SysRecipe_m_nOpenV;
    g_testConditionLogData[TREC_CONDI_4W_TEST].funcp             = getStr_SysRecipe_4WTest_Enable;
    g_testConditionLogData[TREC_CONDI_4W_MODE].funcp             = getStr_SysRecipe_m_n4WSet1;

    g_testConditionLogData[TREC_CONDI_USHORT_TEST].funcp         = getStr_SysRecipe_uShortTest_Enable;
    g_testConditionLogData[TREC_CONDI_USHORT_R].funcp            = getStr_SysRecipe_m_nShortR;

    g_testConditionLogData[TREC_CONDI_LSHORT_TEST].funcp         = getStr_SysRecipe_LShortTest_Enable;
    g_testConditionLogData[TREC_CONDI_LSHORT_R].funcp            = getStr_SysRecipe_m_nShortR2;

    g_testConditionLogData[TREC_CONDI_HRSHORT_TEST].funcp        = getStr_SysRecipe_HRShortTest_Enable;
    g_testConditionLogData[TREC_CONDI_HRSHORT_R].funcp           = getStr_SysRecipe_m_nHR;
    
    g_testConditionLogData[TREC_CONDI_LEAK1_TEST].funcp          = getStr_Leak1Test_Enable;
    g_testConditionLogData[TREC_CONDI_LEAK1_SPARK_CHECK].funcp   = getStr_SysRecipe_SparkCheck;
    g_testConditionLogData[TREC_CONDI_LEAK1_V_SPARK_CHECK].funcp = getStr_SysRecipe_SparkCheck;
    g_testConditionLogData[TREC_CONDI_LEAK1_SPARK_V].funcp       = getStr_SysRecipe_SparkVoltage;
    g_testConditionLogData[TREC_CONDI_LEAK1_V].funcp             = getStr_SysRecipe_Leak1Voltage;
    g_testConditionLogData[TREC_CONDI_LEAK1_R].funcp             = getStr_SysRecipe_Leak1R;

    g_testConditionLogData[TREC_CONDI_LEAK2_TEST].funcp          = getStr_Leak2Test_Enable;
    g_testConditionLogData[TREC_CONDI_LEAK2_V_SPARK_CHECK].funcp = getStr_SysRecipe_SparkCheck;
    g_testConditionLogData[TREC_CONDI_LEAK2_SPARK_V].funcp       = getStr_SysRecipe_SparkVoltage;
    g_testConditionLogData[TREC_CONDI_LEAK2_V].funcp             = getStr_SysRecipe_Leak2Voltage;
    g_testConditionLogData[TREC_CONDI_LEAK2_R].funcp             = getStr_SysRecipe_Leak2R;
//son230707_2 end

    //-------------------------------
    //son TestCondition Sample 로딩


    //son g_testConditionLogData[i]의 나머지 영역을 "TestConditionSample.csv"에서 Read해서 채운다. 
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE

    CString strfName1, str;
    strfName1.Format("c:\\ACE500\\Data\\TestConditionSample.csv");

    ::ZeroMemory(&fName, sizeof(fName));
    strcat(fName ,strfName1);

    fp=fopen(fName,"rt");
    if(fp == NULL)
    { 
        str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
        AfxMessageBox(str, MB_OK);
        return 0;
    }

    char buf[GET_BUF_SIZE];     //son210524 1024-> GET_BUF_SIZE
    CString strbuf, tmpstr;

    ::ZeroMemory(&buf, sizeof(buf));    //son211112
    fgets(buf, sizeof(buf),fp);   //son 첫줄 헤더는 무시한다.  "type,  name,  value "       //son210524 1024->sizeof(buf)
    int no;

    for (i = 0; i < MAX_TEST_CONDITION_ITEM; i++)
    {
        ::ZeroMemory(&buf, sizeof(buf));    //son211112
        fgets(buf, sizeof(buf),fp);        //son 한 줄을 읽어 온다.     //son210524 1024->sizeof(buf)
        if (feof(fp))   
            break;

        strbuf.Format("%s", buf);   //son 읽어온 한 줄을 CString 타입으로 변환

        //son 첫번째 ',' 앞의 str을  nType으로 read한다. 
        no     = strbuf.Find(',');
        tmpstr = strbuf.Left(no);
        g_testConditionLogData[i].nType = atoi(tmpstr);
        strbuf = strbuf.Mid(no+1);  //son ',' 뒤로 이동

        //son 두번째 ',' 앞의 str을  szName으로 copy한다. 
        no     = strbuf.Find(',');
        tmpstr = strbuf.Left(no);
        strcpy(g_testConditionLogData[i].szName, tmpstr);
        strbuf = strbuf.Mid(no+1);  //son ',' 뒤로 이동

        //son 마지막항목. str끝의 LineFeed 문자를 제외하고  szFixedVal로 copy한다.
        no     = strbuf.Find('\n');
        tmpstr = strbuf.Left(no);
        strcpy(g_testConditionLogData[i].szFixedVal, tmpstr);

        //fscanf(fp, "%d, %s, %s\n", &g_testConditionLogData[i].nType, 
        //                            g_testConditionLogData[i].szName, g_testConditionLogData[i].szFixedVal);
    }

    fclose(fp); 
    return 1;
}

//son210324  현재 Recipe 상황을 Z:\\TestConditionLog.csv로 출력한다.
int CFileSysInfo::Write_TestCondionLog()
{
    CString strfName1, str;  
    FILE *fp;
    char fName1[200];

    strfName1.Format("z:\\TestConditionLog.csv");
    ::ZeroMemory(&fName1,sizeof(fName1)); 
    strcat(fName1 , strfName1);

    fp = fopen(fName1,"wt");    
    if(fp == NULL){ 
        str.Format("Error    %s file open failed!", fName1);
        AfxMessageBox( str, MB_OK);  
        return -1;
    } 

    int i;
#if 0
    //-----------------------------------
    //son 세로로 출력하는 경우 : test용
    for (i = 0; i < MAX_TEST_CONDITION_ITEM; i++)
    {

        switch(g_testConditionLogData[i].nType)
        {
            case TCONDI_TYPE_BLANK:  //son Blank의 경우는 Blank 함수를 호출
            case TCONDI_TYPE_RECIPE: //son Recipe 값을 읽어오는 함수를 호출해서 value를 출력한다.
                fprintf(fp, "%s, %s\n", g_testConditionLogData[i].szName, g_testConditionLogData[i].funcp()); //son230707_2
                break;

            //son TestConditionSample.csv에 있는 Fixed value를 출력한다.
            case TCONDI_TYPE_FIXED:
                fprintf(fp, "%s, %s\n", g_testConditionLogData[i].szName, g_testConditionLogData[i].szFixedVal);
            default:
                break;
        }

    }
#endif

#if 1
    //-----------------------------------------------------
    //son 삼성전기 요구사항대로 가로로 ',' 구분하여 출력한다. 
    
    //son 먼저 TestCondition Name 라인을 출력
    for (i = 0; i < MAX_TEST_CONDITION_ITEM; i++)
    {
        fprintf(fp, "%s, ", g_testConditionLogData[i].szName);
    }
    fprintf(fp, "\n");

    //son Type에 맞는 value str 라인을 출력한다.
    for (i = 0; i < MAX_TEST_CONDITION_ITEM; i++)
    {
        switch(g_testConditionLogData[i].nType)
        {
            case TCONDI_TYPE_BLANK:  //son Blank의 경우는 Blank 함수를 호출
            case TCONDI_TYPE_RECIPE: //son Recipe 값을 읽어오는 함수를 호출해서 value를 출력한다.
                fprintf(fp, "%s, ", g_testConditionLogData[i].funcp());    //<=== 함수 포인터 사용 //son230707_2
                break;

            //son TestConditionSample.csv에 있는 Fixed value를 출력한다.
            case TCONDI_TYPE_FIXED:
                fprintf(fp, "%s, ", g_testConditionLogData[i].szFixedVal);
            default:
                break;
        }

    }
    fprintf(fp, "\n");

#endif

    fclose(fp);

    return 1;
}

//son 공백 출력
char* getStr_Blank()                 //son210324: for TestConditionLog
{
    CString str;
    str.Format(" ");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;

}

char* getStr_DateTime()                 //son210324: for TestConditionLog
{
    char date[24];
    memset( date, 0x00, 24 );

    time_t ltime;
    time( &ltime );

    struct tm *today;
    today = localtime( &ltime );
    CTime t = CTime::GetCurrentTime();
   
    strftime(date, 24, "%Y-%m-%d", today);
    
    CString str;
    str.Format("%s %02d:%02d:%02d", date, t.GetHour(), t.GetMinute(), t.GetSecond());

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}


char* getStr_SysRecipe_m_nDelay1()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%dms", SysInfo05.m_nDelay1);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_m_nOpenV()       //son210324: for TestConditionLog
{
    CString str;
    if( SysInfo05.m_nOpenV >0  &&  SysInfo05.m_nOpenV <=26) 
    { 
         if(SysInfo05.m_nOpenV ==1){//sylee130723
             str.Format("%dV", SysInfo05.m_nOpenV);   
         }else{ 
             str.Format("%d0V", (SysInfo05.m_nOpenV-1));
         }
#ifdef __YP_VIET_CUSTOMER
        //son210824: 베트남영풍 코드. Apple 검수 대비 
        //son 영풍베트남  50V (SysInfo05.m_nOpenV == 6)이면 강제로 10V로 출력한다.
        if (SysInfo05.m_nOpenV == RECP_OPENV_6_50V)     //son231106 6: RECP_OPENV_6_50V
            str.Format("10V"); 
#endif
    }else{
        str.Format("?");
    } 

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_m_nConFail_ON()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%s", (SysInfo05.m_nConFail > 0) ? "ON": "OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_m_nReCh()      //son210324: for TestConditionLog
{
    CString str;
    //str.Format("%d", SysInfo05.m_nReCh);
    str.Format("%s", (SysInfo05.m_nReCh > 0) ? "ON": "OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}
char* getStr_SysRecipe_m_nConEr()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%d", SysInfo05.m_nConEr);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_m_nConFail()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%d", SysInfo05.m_nConFail);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysInfoBlock_m_nCol()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%d",SysInfoBlock.m_nCol);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysInfoBlock_m_nRow()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%d",SysInfoBlock.m_nRow);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_nDUTCnt()      //son210324: for TestConditionLog
{
    CString str;
    str.Format("%d",nMultyNetTotal);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}
int   getValOhm_SysRecipe_m_nCon()  //son210324
{
    int nOpenR_Ohm;

#if 0   //son220705 begin
    //son Recipe : m_combo2 data 참고.  (OPEN 즉, Continuity R 설정값)
    if( SysInfo05.m_nCon == 1) {                //son '*' -> 5 ohm
        nOpenR_Ohm=5;
    }else if( SysInfo05.m_nCon ==   2){         //son 10 ohm 
        nOpenR_Ohm=10;
    }else if( SysInfo05.m_nCon ==   3){         //son 20 ohm
        nOpenR_Ohm=20;
    }else if( SysInfo05.m_nCon ==   4){         //son 30 ohm
        nOpenR_Ohm=30;

    }else if( SysInfo05.m_nCon ==   5){         //son 40 ohm
        nOpenR_Ohm=40;
    }else if( SysInfo05.m_nCon ==   6){         //son 50 ohm
        nOpenR_Ohm=50;
    }else if( SysInfo05.m_nCon ==   7){         //son 60 ohm
        nOpenR_Ohm=60;
    }else if( SysInfo05.m_nCon ==   8){         //son 70 ohm
        nOpenR_Ohm=70;
    }else if( SysInfo05.m_nCon ==   9){         //son 80 ohm
        nOpenR_Ohm=80;

    }else if( SysInfo05.m_nCon ==   10){        //son 90 ohm
        nOpenR_Ohm=90;
    }else if( SysInfo05.m_nCon ==   11){        //son 100 ohm
        nOpenR_Ohm=100;
    }else if( SysInfo05.m_nCon ==   12){        //son 200 ohm
        nOpenR_Ohm=200;
    }else if( SysInfo05.m_nCon ==   13){        //son 300 ohm
        nOpenR_Ohm=300;
    }else if( SysInfo05.m_nCon ==   14){        //son 400 ohm
        nOpenR_Ohm=400;

    }else if( SysInfo05.m_nCon ==   15){        //son 500 ohm
        nOpenR_Ohm=500;
    }else if( SysInfo05.m_nCon ==   16){        //son 600 ohm
        nOpenR_Ohm=600;
    }else if( SysInfo05.m_nCon ==   17){        //son 700 ohm
        nOpenR_Ohm=700;
    }else if( SysInfo05.m_nCon ==   18){        //son 800 ohm
        nOpenR_Ohm=800;
    }else if( SysInfo05.m_nCon ==   19){        //son 900 ohm
        nOpenR_Ohm=900;

    }else if( SysInfo05.m_nCon ==   20){        //son 1 Kohm
        nOpenR_Ohm=1000;
    }else if( SysInfo05.m_nCon ==   21){        //son 10 Kohm (10V)
        nOpenR_Ohm=10000;
    }else if( SysInfo05.m_nCon ==   22){        //son 30 Kohm (10V)
        nOpenR_Ohm=30000;
    }else if( SysInfo05.m_nCon ==   23){        //son 50 Kohm (10V)
        nOpenR_Ohm=50000;
    }else if( SysInfo05.m_nCon ==   24){        //son 100 Kohm (10V)
        nOpenR_Ohm=100000; 
    }
    else if (SysInfo05.m_nCon == RECP_OPENR_USER) //son 25: *User   //son220705
    { 

        if( SysInfo05.m_nConU > MAX_OPEN_USER_R){      //son MAX_OPEN_USER_R: 100000
            nOpenR_Ohm=0;
            AfxMessageBox(" ERROR NO 7501,    OPEN R  100k  Over?,   Recipe Open R  Select Check! \n\n    ", MB_OK );
        }else{
            nOpenR_Ohm=SysInfo05.m_nConU; 
        }

    }
    else{
        nOpenR_Ohm=0;
    }

#else   
    //son220705 Recipe Open R 콤보박스 중국어 ohm 표시 기능 추가.
    nOpenR_Ohm=0;
    CList_Recipe_Open_R  recipe_OpenR;
    if (SysInfo05.m_nCon >= RECP_OPENR_5ohm &&  SysInfo05.m_nCon < RECP_OPENR_USER) //son220705
        nOpenR_Ohm = recipe_OpenR.nOpenR[SysInfo05.m_nCon];         //son220705

    if (SysInfo05.m_nCon == RECP_OPENR_USER) //son 25: *User   //son220705
    { 
        //son Open User R은 100Kohm 이상은 설정 불가.
        if (SysInfo05.m_nConU > MAX_OPEN_USER_R){      //son 100000: MAX_OPEN_USER_R
            nOpenR_Ohm=0;
            AfxMessageBox(" ERROR NO 7501,    OPEN R  100Kohm  Over?,   Recipe Open R  Select Check! \n\n    ", MB_OK );
        }
        else{
            nOpenR_Ohm=SysInfo05.m_nConU; 
        }
    }
#endif //son220705 end


    return nOpenR_Ohm;
}

//son210324  Auto 화면 출력용
char* getStrOhm_R(int nR_Ohm)      //son210324: for TestConditionLog
{
    CString str;
    int nT1;

    if( nR_Ohm ==0)
        str.Format("        ");

    else if( nR_Ohm >=1000000)
    {
        nT1=(int)nR_Ohm /1000000;
#ifdef OHM  //SYLEE210324
        str.Format("%d Mohm", nT1);
#else
    #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        str.Format("%d M%s", nT1, _T("┯"));    //son220629 GTS 중국어 윈도우 Ω 표시 추가
    #else
        //str.Format("%d MΩ", nT1);
        str.Format("%d M%s", nT1, _T("Ω"));    //son220425 말레이시아 simmtech Ω 표시가 깨지는 현상관련 보완
    #endif
#endif
    }
    else if( nR_Ohm >=1000)
    {
        nT1=(int)nR_Ohm /1000;
#ifdef OHM  //SYLEE210324
        str.Format("%d Kohm", nT1);
#else
    #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        str.Format("%d K%s", nT1, _T("┯"));    //son220629 GTS 중국어 윈도우 Ω 표시 추가
    #else
        //str.Format("%d KΩ", nT1);
        str.Format("%d K%s", nT1, _T("Ω"));    //son220425
    #endif
#endif
    }
    else
    {
        nT1=(int)nR_Ohm ;
#ifdef OHM  //SYLEE210324
        str.Format("%d ohm", nT1);
#else
    #if (defined(__GTS__CUSTOMER) || defined(__MST__CUSTOMER))  //son220913
        str.Format("%d %s", nT1, _T("┯"));     //son220629 GTS 중국어 윈도우 Ω 표시 추가
    #else
        //str.Format("%d Ω", nT1);
        str.Format("%d %s", nT1, _T("Ω"));     //son220425
    #endif
#endif
    }


    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;

}

//son210324 TestConditionLog 출력용 
// Auto화면용은 "Ω"을 이용해서 출력하므로 사용불가.  TestCondtion 전용 Ohm_R 출력함수를 새로 만듦.
char* getStrOhm_R_2(int nR_Ohm)      //son210324: for TestConditionLog
{
    CString str;
    int nT1;

    if( nR_Ohm ==0)
        str.Format("        ");

    else if( nR_Ohm >=1000000)
    {
        nT1=(int)nR_Ohm /1000000;
        str.Format("%dMohm", nT1);
    }
    else if( nR_Ohm >=1000)
    {
        nT1=(int)nR_Ohm /1000;
        str.Format("%dKohm", nT1);
    }
    else
    {
        nT1=(int)nR_Ohm ;
        str.Format("%dohm", nT1);
    }

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;

}

char* getStr_SysRecipe_m_nCon()      //son210324: for TestConditionLog
{
    int nOpenR_Ohm = getValOhm_SysRecipe_m_nCon();

    return getStrOhm_R_2(nOpenR_Ohm);
}

int getState_SysRecipe_TestModeEnable(int TestMode)     //son210324
{
    int mode, nTestEnable = 0;

    for(int k=1; k<=7; k++)
    { 
        mode =  SysInfo05.m_nStep[k];//open=1, ushort=2, short=3, hr=4, hv1=5,  hv2=6,  4w=7
        if (mode==TestMode)
            nTestEnable=1;
    }

    return nTestEnable;
}

char* getStr_SysRecipe_4WTest_Enable()        //son210324: for TestConditionLog
{
    CString str;
    int n4WEnable = getState_SysRecipe_TestModeEnable(TEST_4W);
    str.Format("%s", (n4WEnable == 1) ? "ON":"OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_m_n4WSet1()      //son210324
{
    CString str, str2;
    str2.Format("%d(Undef_4wSet)", SysInfo05.m_n4WSet1);
    str.Format("%s", (SysInfo05.m_n4WSet1 == _4W_SET_FAST) ?   "Fast": 
                     (SysInfo05.m_n4WSet1 == _4W_SET_NORMAL) ? "Normal":
                     (SysInfo05.m_n4WSet1 == _4W_SET_SLOW) ?   "Slow":
                     (SysInfo05.m_n4WSet1 == _4W_SET_FINE) ?   "Fine": str2);

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_uShortTest_Enable()        //son210324: for TestConditionLog
{
    CString str;
    int nEnable = getState_SysRecipe_TestModeEnable(TEST_U_SHORT);
    str.Format("%s", (nEnable == 1) ? "ON":"OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_LShortTest_Enable()        //son210324: for TestConditionLog
{
    CString str;
    int nEnable = getState_SysRecipe_TestModeEnable(TEST_SHORT);
    str.Format("%s", (nEnable == 1) ? "ON":"OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_HRShortTest_Enable()        //son210324: for TestConditionLog
{
    CString str;
    int nEnable = getState_SysRecipe_TestModeEnable(TEST_HR_SHORT);

    str.Format("%s", (nEnable == 1) ? "ON":"OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_Leak1Test_Enable()        //son210324: for TestConditionLog
{
    CString str;
    int nEnable = getState_SysRecipe_TestModeEnable(TEST_LEAK1);
    str.Format("%s", (nEnable == 1) ? "ON":"OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}
char* getStr_Leak2Test_Enable()        //son210324: for TestConditionLog
{
    CString str;
    int nEnable = getState_SysRecipe_TestModeEnable(TEST_LEAK2);
    str.Format("%s", (nEnable == 1) ? "ON":"OFF");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

//son for uShort Test
int getValOhm_SysRecipe_m_nShortR() //son210324: for TestConditionLog
{
    int n_uShort_Rohm;

#if 0   //son220705 begin
    if( SysInfo05.m_nShortR ==1){//SYLEE140618
        n_uShort_Rohm=10000;
    }else if( SysInfo05.m_nShortR ==2){
        n_uShort_Rohm=50000;
    }else if( SysInfo05.m_nShortR ==3){
        n_uShort_Rohm=100000;
    }else if( SysInfo05.m_nShortR ==4){  
        n_uShort_Rohm=SysInfo05.m_nShortRU*1000.; //sylee161012  //sylee161026
    }else{
        n_uShort_Rohm=0;
    }
#else
    //son220705 Recipe uShort R 콤보박스 중국어 ohm 표시 기능 추가.
    n_uShort_Rohm= 0;
    CList_Recipe_uShortR  recipe_uShortR;
    if (SysInfo05.m_nShortR >= RECP_uShortR_10Kohm &&  SysInfo05.m_nShortR < RECP_uShortR_USER) //son220705
        n_uShort_Rohm= recipe_uShortR.nR[SysInfo05.m_nShortR];         //son220705

    if (SysInfo05.m_nShortR == RECP_uShortR_USER) //son 4: *User   //son220705
        n_uShort_Rohm= SysInfo05.m_nShortRU*1000.; //sylee161012  //sylee161026

#endif //son220705 end


    return n_uShort_Rohm;
}
//son for uShort Test
char* getStr_SysRecipe_m_nShortR()      //son210324: for TestConditionLog
{
    int nR_Ohm = getValOhm_SysRecipe_m_nShortR();

    return getStrOhm_R_2(nR_Ohm);
}

//son for LShort Test
int getValOhm_SysRecipe_m_nShortR2() //son210324: for TestConditionLog
{
    int n_LShort_R;

#if 0   //son220705 begin
    if( SysInfo05.m_nShortR2 ==1) {
         n_LShort_R=500; //R
    }else if( SysInfo05.m_nShortR2 ==2) {
         n_LShort_R=1000;//R
    }else if( SysInfo05.m_nShortR2 ==3) {
         n_LShort_R=5000;//R
    }else if( SysInfo05.m_nShortR2 ==4) {
         n_LShort_R=10000;//R
    }else if( SysInfo05.m_nShortR2 ==5) {
         n_LShort_R=30000;//R
    }else if( SysInfo05.m_nShortR2 ==6) {
         n_LShort_R=50000; //R
    }else if( SysInfo05.m_nShortR2 >=7   &&  SysInfo05.m_nShortR2 <=15) {
           n_LShort_R=(SysInfo05.m_nShortR2-6)*100000;   //R
    }else if( SysInfo05.m_nShortR2 >=16   &&  SysInfo05.m_nShortR2 <=25) {
           n_LShort_R=(SysInfo05.m_nShortR2-15)*1000000;  //R
    }else if( SysInfo05.m_nShortR2 ==26) { 
           n_LShort_R=SysInfo05.m_nShortR2U*1000.;//R  //sylee161208-1
    }else{
        n_LShort_R = 0;    //son210324
    }
#else
    //son220705 Recipe Short R 콤보박스 중국어 ohm 표시 기능 추가.
    n_LShort_R = 0;
    CList_Recipe_LShortR  recipe_LShortR;
    if (SysInfo05.m_nShortR2 >= RECP_LShortR_500ohm &&  SysInfo05.m_nShortR2 < RECP_LShortR_USER) //son220705
        n_LShort_R= recipe_LShortR.nR[SysInfo05.m_nShortR2];         //son220705

    if( SysInfo05.m_nShortR2 ==RECP_LShortR_USER)    //son 26: RECP_LShortR_USER 
        n_LShort_R= SysInfo05.m_nShortR2U*1000.;//R  //sylee161208-1

#endif //son220705 end

    return n_LShort_R;
}

//son for LShort Test
char* getStr_SysRecipe_m_nShortR2()      //son210324: for TestConditionLog
{
    int nR_Ohm = getValOhm_SysRecipe_m_nShortR2();

    return getStrOhm_R_2(nR_Ohm);
}

//son for HR Short Test
int getValOhm_SysRecipe_m_nHR() //son210324: for TestConditionLog
{
    int n_HR_Short_R;

#if 0   //son220705 begin
    //if( SysInfo05.m_nHR >=1   &&  SysInfo05.m_nHR <=10) {
    
    if(SysInfo05.m_nHR ==1){//sylee140618
        n_HR_Short_R=100000;//100k
    }else if(SysInfo05.m_nHR ==2){//sylee140618
        n_HR_Short_R=500000;//500k
    }else if(SysInfo05.m_nHR ==3){//sylee140618
        n_HR_Short_R=1000000;//1M
    }else if(SysInfo05.m_nHR ==4){//sylee140618
        n_HR_Short_R=5000000;//5M
    }else if(SysInfo05.m_nHR ==5){//sylee140618
        n_HR_Short_R=10000000;//10M
    }else if(SysInfo05.m_nHR ==6){//sylee140618
        n_HR_Short_R=SysInfo05.m_nHRL*1000;  //user
    }else{
        n_HR_Short_R=10000000;//10M
        //n_HR_Short_R=(SysInfo05.m_nHR)*1000000;//1M
    }
#else
    //son220705 Recipe Short R 콤보박스 중국어 ohm 표시 기능 추가.
    n_HR_Short_R= 10000000;//10M
    CList_Recipe_HRShortR  recipe_HRShortR;
    if (SysInfo05.m_nHR >= RECP_HRShortR_100Kohm &&  SysInfo05.m_nHR < RECP_HRShortR_USER) //son220705
        n_HR_Short_R= recipe_HRShortR.nR[SysInfo05.m_nHR];         //son220705

    if (SysInfo05.m_nHR == RECP_HRShortR_USER)
        n_HR_Short_R=SysInfo05.m_nHRL*1000;  //sylee140618

#endif  //son220705 end
    return n_HR_Short_R;
}


//son for HR Short Test
char* getStr_SysRecipe_m_nHR()      //son210324: for TestConditionLog
{
    int nR_Ohm = getValOhm_SysRecipe_m_nHR();

    return getStrOhm_R_2(nR_Ohm);
}


//son for Leak1 Test
int getValOhm_SysRecipe_Leak1R() //son210324: for TestConditionLog
{
    int n_Leak1_R;

#if 0   //son220705 begin
    if( SysInfo05.m_nHv1R ==1   ) {             //son210324 m_nConIs-> m_nHv1R for Leak1
        n_Leak1_R=1000000; 
    }else if( SysInfo05.m_nHv1R ==2   ) {
        n_Leak1_R=10*1000000; 
    }else if( SysInfo05.m_nHv1R ==3   ) {
        n_Leak1_R=20*1000000;
    }else if( SysInfo05.m_nHv1R ==4   ) {
        n_Leak1_R=50*1000000; 
    }else if( SysInfo05.m_nHv1R ==5   ) {
        n_Leak1_R=100*1000000;
    }else if( SysInfo05.m_nHv1R ==6){
        n_Leak1_R=SysInfo05.m_nHv1RU*1000;      //son210324 m_nConIsU-> m_nHv1RU for Leak1
    }else{ 
        n_Leak1_R=0;///???????????
    }
#else
    //son220705 Recipe Leak1 R 콤보박스 중국어 ohm 표시 기능 추가.
    n_Leak1_R = 0;
    CList_Recipe_LEAK_R  recipe_LeakR;
    if (SysInfo05.m_nHv1R >= RECP_LEAK_1Mohm &&  SysInfo05.m_nHv1R < RECP_LEAK_USER) //son220705
        n_Leak1_R= recipe_LeakR.nR[SysInfo05.m_nHv1R];         //son220705

    if (SysInfo05.m_nHv1R == RECP_LEAK_USER)
        n_Leak1_R=SysInfo05.m_nHv1RU*1000;  //son Kohm 단위

#endif //son220705 end

    return n_Leak1_R;
}

//son for Leak1 Test
char* getStr_SysRecipe_Leak1R()      //son210324: for TestConditionLog
{
    int nR_Ohm = getValOhm_SysRecipe_Leak1R();

    return getStrOhm_R_2(nR_Ohm);
}

//son for Leak2 Test
int getValOhm_SysRecipe_Leak2R() //son210324: for TestConditionLog
{
    int n_Leak2_R;

#if 0   //son220705 begin
    if( SysInfo05.m_nHv2R ==1   ) {
        n_Leak2_R=1000000; 
    }else if( SysInfo05.m_nHv2R ==2   ) {
        n_Leak2_R=10*1000000; 
    }else if( SysInfo05.m_nHv2R ==3   ) {
        n_Leak2_R=20*1000000;
    }else if( SysInfo05.m_nHv2R ==4   ) {
        n_Leak2_R=50*1000000; 
    }else if( SysInfo05.m_nHv2R ==5   ) {
        n_Leak2_R=100*1000000;
    }else if( SysInfo05.m_nHv2R ==6){
        n_Leak2_R=SysInfo05.m_nHv2RU*1000;
    }else{ 
        n_Leak2_R=0;///???????????
    }
#else
    //son220705 Recipe Leak1 R 콤보박스 중국어 ohm 표시 기능 추가.
    n_Leak2_R = 0;
    CList_Recipe_LEAK_R  recipe_LeakR;
    if (SysInfo05.m_nHv2R >= RECP_LEAK_1Mohm &&  SysInfo05.m_nHv2R < RECP_LEAK_USER) //son220705
        n_Leak2_R= recipe_LeakR.nR[SysInfo05.m_nHv2R];         //son220705

    if (SysInfo05.m_nHv2R == RECP_LEAK_USER)
        n_Leak2_R=SysInfo05.m_nHv2RU*1000;  //son Kohm 단위

#endif //son220705 end

    return n_Leak2_R;
}

//son for Leak2 Test
char* getStr_SysRecipe_Leak2R()      //son210324: for TestConditionLog
{
    int nR_Ohm = getValOhm_SysRecipe_Leak2R();

    return getStrOhm_R_2(nR_Ohm);
}

char* getStr_SysRecipe_SparkCheck()     //son210324: for TestConditionLog
{
    CString str;
    str.Format("%s", (SysInfo05.m_nSp == 0) ? "OFF":"ON");

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_SparkVoltage()       //son210324: for TestConditionLog
{
    CString str;
    str.Format("%dV", SysInfo05.m_nSparkSet);     

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}

char* getStr_SysRecipe_Leak1Voltage()       //son210324: for TestConditionLog
{
    CString str;
    if( SysInfo05.m_nHv >0  &&  
        SysInfo05.m_nHv <29) {
        str.Format("%d0V", (SysInfo05.m_nHv+2));      
    }else{
        str.Format("?");
    } 

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}
char* getStr_SysRecipe_Leak2Voltage()       //son210324: for TestConditionLog
{
    CString str;
    if( SysInfo05.m_nHv2 >0  &&  
        SysInfo05.m_nHv2 <29) {
        str.Format("%d0V", (SysInfo05.m_nHv2+2));     
    }else{
        str.Format("?");
    } 

    static char szStr[30];
    ::ZeroMemory(&szStr, sizeof(szStr));
    strcat( szStr , str); 
    return szStr;
}


//son 4W Net Data:  g_s4WNet[dutIdx]을 BDLPath\4W.ini 파일을 저장하거나,
//                  BDL의 4W.ini 파일을 읽어서  g_s4WNet[dutIdx]에 로딩한다.
int CFileSysInfo::LoadSaveSub17(int type)//sylee20120216 //4WCODE_131118
{


    //-----------------------------------------------------
    //son SyInfo05.m_nStep[] 중에 4W 테스트가 있는지 확인       
    
    //son "\SETUP2\Recipe.ini"를 struct CSysInfo05에 Load(type=2)
    LoadSaveSub15(_LOAD);//sylee180713-1

    
//son221107 begin
#ifdef __GTS_4W_SPCIALNET__
    //---------------------------------
    //son 4W.ini를 read/write 한다.         
    CString str, strTemp; 

    if (type == _LOAD)
    {
        char    fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE

        //son 기존 4W.ini ~ 4w.ini#를 DUT 개수대로 read 한다.         
        LoadSaveSub17_4w_ini(_LOAD);

        ::ZeroMemory(&fName, sizeof(fName));  //sylee160731-2

        //son 4w_v2.ini가 존재하지 않는다면 4w_v2.ini를 새로 만든다. 
        //    4w_v2.ini는 기존 4w.ini에 없는 bNetSpecial 값과 reserved 영역을 추가해서 만들어 진다. 
        str.Format("%s\\4W_v2.INI",SysInfoView01.m_pStrFilePathBDL);  
        strcat( fName , str);  
        if(FileExists(fName) == FALSE)  //son 4w_v2.ini가 없다면 
        {

            //son 모든 bNetSpecial 영역을 0으로 해서 
            for (int dutIdx = 0; dutIdx < nMultyNetTotal; dutIdx++)
                for (int net = 0; net < g_s4WNet[dutIdx].wCount; net++)
                    g_s4WNet[dutIdx].saData[net].bNetSpecial = 0;

            
            //son 새로 4w_v2.ini~ 4w_v2.ini#을 만든다.
            LoadSaveSub17_4w_v2_ini(_SAVE);
            return 1;
        }

        //son 4w_v2.ini가 존재한다면 4w_v2.ini ~ 4w_v2.ini#를 DUT 개수대로 read하고 return한다.
        else
        {
            return  LoadSaveSub17_4w_v2_ini(_LOAD);
        }
    } 

    else    //type == _SAVE 
    {
        //son 기존 4W.ini ~ 4w.ini#를 DUT 개수대로 save 한다.         
        LoadSaveSub17_4w_ini(_SAVE);
        LoadSaveSub17_4w_v2_ini(_SAVE);
        return 1;
    }


#else
    //son 기존 4W.ini만  read/write 한다.         
    return  LoadSaveSub17_4w_ini(type);
#endif
//son221107 end


}

//son 기존 4w.ini read/write 함수.
int CFileSysInfo::LoadSaveSub17_4w_ini(int type)//son221107 LoadSaveSub17()에서 분리하여 신규생성
{
    int     i, ret, net, pin;
    FILE    *fp; 
    char    fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str, strTemp; 

    //---------------------------------
    //son 4W.ini를 read/write 한다.     
    
    
    //son200320-4WDUT16: LoadSaveSub17() for 문 추가.  4W.ini 로딩시에 모든 DUT 로딩한다.
    for (int dutIdx = 0; dutIdx < nMultyNetTotal; dutIdx++)
    {
        ::ZeroMemory(&fName, sizeof(fName));


        //son200320-4WDUT16: 
        //son DUT 번호로 4W.ini 파일 name을 생성
        CString fileName;               
        if (dutIdx == 0)
            fileName.Format("4W.ini");              
        else
            fileName.Format("4W.ini%d", dutIdx+1);

        str.Format("%s\\%s",SysInfoView01.m_pStrFilePathBDL, fileName);
        strcat( fName , str); //sylee131022

        switch(type)
        {
            case _SAVE: //DATASAVE :           
                DeleteFile(fName); // file 삭제 
                fp = fopen(fName,"wt");
                if(fp == NULL){
                    strTemp.Format(" Error No 2011, \n   file open Error  \n   %s file save error   !!!", fName);
                    AfxMessageBox(strTemp);
                    return 0;
                }

                //son240727 if(g_s4WNet[dutIdx].wCount<=0) ///error messege     //son190704-4WDUT16
                if(g_s4WNet[dutIdx].wCount<0) ///error messege     //son240727 Count == 0인 경우는 처리하게 
                {
                    strTemp.Format(" Error No 2018, \n   Save Error  \n     %s   header  Net Count?  (< 0)  !!!", fName);    //son221111_3
                    AfxMessageBox(strTemp);
                    fclose(fp);
                    return 0;
                }

                fprintf(fp, "%10.0f\n", (float)g_s4WNet[dutIdx].wCount);    //son190704-4WDUT16

                for (i = 1; i < 5 ; i++){   
                    fprintf(fp, "%10.0f\n", 0);
                }

                for (net = 0; net < g_s4WNet[dutIdx].wCount; net++)     //son190704-4WDUT16
                {
                    fprintf(fp, "  %7.0f", (float)g_s4WNet[dutIdx].saData[net].wPiece); //son211005 wNet -> wPiece

                    for(pin = 0; pin < _4W_PIN_NUM; pin++)
                        fprintf(fp, "  %7.0f", (float)g_s4WNet[dutIdx].saData[net].waPin[pin]);

                    fprintf(fp, "  %10.3f", (float)g_s4WNet[dutIdx].saData[net].dRefLow );  //son211005 dSetMin -> dRefLow
                    fprintf(fp, "  %10.3f", (float)g_s4WNet[dutIdx].saData[net].dRefAvg );  //son211005 dSetAvg -> dRefAvg
                    fprintf(fp, "  %10.3f", (float)g_s4WNet[dutIdx].saData[net].dRefHigh ); //son211005 dSetMax -> dRefHigh

                    fprintf(fp, "  %7.0f", (float)g_s4WNet[dutIdx].saData[net].n4wMode);

                    fprintf(fp, "  %10.3f", (float)g_s4WNet[dutIdx].saData[net].dTestMin ); 
                    fprintf(fp, "  %10.3f", (float)g_s4WNet[dutIdx].saData[net].dTestAvg ); 
                    fprintf(fp, "  %10.3f\n", (float)g_s4WNet[dutIdx].saData[net].dTestMax ); 

                    //  fprintf(fp, " \n" );         
                }


                break;


            case _LOAD://DATALOAD :

                char    str2[2048];         //son190620  str(CString) ->str2(char arr)로 변경. fscanf 버퍼문제 
                ::ZeroMemory(str2, sizeof(str2));

                fp=fopen(fName,"rt");       // "4W.ini"
                if(fp==NULL){
                    strTemp.Format(" Error No 2012, \n   file open Error  \n   %s file load error   !!!", fName);
                    AfxMessageBox(strTemp);
                    return 0;
                }


                for(i=0;i<5;i++)
                {
                    ret=fscanf(fp, "%s", str2);     //son190620  str(CString) ->str2(char arr)로 변경. fscanf 버퍼문제

                    //son190704-4WDUT16
                    //son 헤더 다섯줄 중에 첫번째 라인: Net count만 사용하고 뒤의 4개는 버린다.
                    if(ret==1 && i==0)      // read한 값의 갯수가 1개, 라인==0
                        g_s4WNet[dutIdx].wCount=(short)atof(str2);  

                    //son240727 if(g_s4WNet[dutIdx].wCount <=0)     //son190704-4WDUT16
                    if(g_s4WNet[dutIdx].wCount < 0)     //son240727 Count == 0인 경우는 처리하게 
                    {
                        ///error messege
                        str.Format(" Error No 2017, \n %s file reading fail! 4WNet Count(=%d) Range( < 0) Over.",   //son240727
                                fileName, g_s4WNet[dutIdx].wCount);  //son240727
                        AfxMessageBox(str);
                        fclose(fp);
                        return 0;
                    }
                }


                for(net=0; net < g_s4WNet[dutIdx].wCount; net++)    //son190704-4WDUT16
                {
                    ret=fscanf(fp, "%d", &g_s4WNet[dutIdx].saData[net].wPiece);         //son211005

                    for(pin = 0; pin < _4W_PIN_NUM; pin++)
                        ret=fscanf(fp, "%d", &g_s4WNet[dutIdx].saData[net].waPin[pin]);     

                    ret=fscanf(fp, "%s", str2);     //son190620  str(CString) ->str2(char arr)로 변경. fscanf 버퍼문제
                    if(ret==1)
                        g_s4WNet[dutIdx].saData[net].dRefLow=(double)atof(str2);        //son211005 dSetMin -> dRefLow

                    ret=fscanf(fp, "%s", str2);     
                    if(ret==1)
                        g_s4WNet[dutIdx].saData[net].dRefAvg=(double)atof(str2);        //son211005 dSetAvg -> dRefAvg

                    ret=fscanf(fp, "%s", str2);     
                    if(ret==1)
                        g_s4WNet[dutIdx].saData[net].dRefHigh=(double)atof(str2);   //son211005 dSetMax -> dRefAvg

                    ret=fscanf(fp, "%d", &g_s4WNet[dutIdx].saData[net].n4wMode);        

                    ret=fscanf(fp, "%s", str2);     
                    if(ret==1)
                        g_s4WNet[dutIdx].saData[net].dTestMin=(double)atof(str2);   

                    ret=fscanf(fp, "%s", str2);     
                    if(ret==1)
                        g_s4WNet[dutIdx].saData[net].dTestAvg=(double)atof(str2);   

                    // 마지막항목이라서 \n까지 read.
                    ret=fscanf(fp, "%s\n", str2);       
                    if(ret==1)
                        g_s4WNet[dutIdx].saData[net].dTestMax=(double)atof(str2);   
                }


                break;

            default :
                break;
        }
        fclose(fp);

        //D:\\Log\\4W-PinListPiece%d.CSV"를 생성한다.
        LoadSaveSub17_test(dutIdx);     //son190704-4WDUT16  중복코드를 함수호출로 변경
    }

    return 1;
}


//son221107 begin:
//son  4w_v2.ini read/write 함수.
#ifdef __GTS_4W_SPCIALNET__
int CFileSysInfo::LoadSaveSub17_4w_v2_ini(int type)
{
    int     i, ret, net;
    FILE    *fp; 
    char    fName[FILE_PATH_SIZE];    
    CString str, strTemp; 

    //---------------------------------
    //son 4W.ini를 read/write 한다.     
    
    for (int dutIdx = 0; dutIdx < nMultyNetTotal; dutIdx++)
    {
        ::ZeroMemory(&fName, sizeof(fName));

        CString dutStr;
        if (dutIdx == 0)
            dutStr.Format("");
        else
            dutStr.Format("%d", dutIdx+1);

        //son DUT 번호로 4W_v2.ini 파일 name을 생성
        CString fileName;               
        fileName.Format("4W_v2.ini%s", dutStr);
        str.Format("%s\\%s",SysInfoView01.m_pStrFilePathBDL, fileName);
        strcat( fName , str); 

        switch(type)
        {
            case _SAVE: //DATASAVE :           
                DeleteFile(fName); // file 삭제 
                fp = fopen(fName,"wt");
                if(fp == NULL){
                    strTemp.Format(" Error No 2011, \n   file open Error  \n   %s file save error   !!!", fName);
                    AfxMessageBox(strTemp);
                    return 0;
                }

                if (g_s4WNet[dutIdx].wCount < 0) ///error messege     //son241009 Count == 0인 경우는 처리하게
                {
                    strTemp.Format(" Error No 2018, \n   Save Error  \n   4w.ini%s header  Net Count? ( < 0)  !!!", dutStr); 
                    AfxMessageBox(strTemp);
                    fclose(fp);
                    return 0;
                }

                for (net = 0; net < g_s4WNet[dutIdx].wCount; net++)     
                {
                    fprintf(fp, "%5d", net+1); 
                    fprintf(fp, "%10d", g_s4WNet[dutIdx].saData[net].bNetSpecial); 

                    //son 10개 reserved 영역
                    for(i = 0; i < 10; i++)
                        fprintf(fp, " %5d", 0);

                    fprintf(fp, "\n");
                }

                break;


            case _LOAD://DATALOAD :

                char    str2[2048];         
                ::ZeroMemory(str2, sizeof(str2));

                fp=fopen(fName,"rt");       // "4W.ini"
                if(fp==NULL){
                    strTemp.Format(" Error No 2012, \n   file open Error  \n   %s file load error   !!!", fName);
                    AfxMessageBox(strTemp);
                    return 0;
                }

                for(net=0; net < g_s4WNet[dutIdx].wCount; net++)    
                {
                    int no;
                    ret=fscanf(fp, "%d", &no);         

                    ret=fscanf(fp, "%d", &g_s4WNet[dutIdx].saData[net].bNetSpecial);         

                    //son reserved 영역
                    for(i = 0; i < 10; i++)
                        ret=fscanf(fp, "%d", &no);     
                }

                break;
            default :
                break;
        }
        fclose(fp);
    }

    return 1;
}
#endif
//son221107 end

//son231018 begin:  나노시스 요구사항. 원격 4W Ref 초기화 기능 
#ifdef __NANOSYS__CUSTOMER
void CFileSysInfo::Init_4wFile_Ref_AllDut()
{
    LoadSaveSub17(_LOAD);

    //son 모든 4W Ref.High와 Ref.Low 를 0으로 만든다. 
    for (int dutIdx = 0; dutIdx < nMultyNetTotal; dutIdx++)
    {
        for (int net = 0; net < g_s4WNet[dutIdx].wCount; net++)
        {
            g_s4WNet[dutIdx].saData[net].dRefLow = 0;
            g_s4WNet[dutIdx].saData[net].dRefHigh = 0;
        }
    }

    LoadSaveSub17(_SAVE);

    MyTrace(PRT_LEVEL1, "Init_4wFile_Ref_AllDut()...\n");

}
#endif
//son231018 end

//------------------------------------------------
//D:\\Log\\4W-PinListPiece%d.CSV"를 생성한다.

//son190704-4WDUT16:  중복코드를 함수호출로 변경
//son200320-4WDUT16: LoadSaveSub17() for 문 추가 관련하여 dutIdx를 parameter로 받도록 수정
extern bool    g_bUseLowerCalJig_4W;     //son220419_7  0: 기존 Pin 사용. 1: 하부핀 R 측정 모드  defined in ModeSub17.cpp
void CFileSysInfo::LoadSaveSub17_test(int dutIdx)//sylee20120216 //4WCODE_131118
{

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //SYLEE131022//4W
    //set load ,  piece sort        //son190703-DUT16 Test Code 
    FILE *fp2 ; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 

    int net, pinNo, piece, pin;     //son190704-4WDUT16
    int pieceNet;                   //son190704-4WDUT16 기존 t3.   piece별 4W Net Num
    int pieceNetCnt[MAX_PIECE+1];   //son190704-4WDUT16 기존 tsum. piece별 4w Net의 Count.  piece는 1부터 시작.  
    ::ZeroMemory(&pieceNetCnt, sizeof(pieceNetCnt));
    ::ZeroMemory(&ng4WSet, sizeof(ng4WSet));//sylee170327

    int nInter4W=0;

    //son200320-4WDUT16: 
    // LoadSaveSub17() for 문 추가 관련하여 dutIdx를 parameter로 받도록 수정하고 아래 코드는 코멘트처리함. 
    //int dutIdx = get4wDutIdx(); // g_s4WNet의 index. 0부터 시작함.    //son190704-4WDUT16 
    
    int dutId = dutIdx +1;        // 1부터 시작하는 dutid             //son190704-4WDUT16

    for(net = 0; net < g_s4WNet[dutIdx].wCount; net++)      //son200320-4WDUT16
    {
        pinNo= g_s4WNet[dutIdx].saData[net].waPin[0]+1; //son190703 기존 t1. "4W Pin1" PinNo            
        piece= (int)g_nPinPieceNo[dutId][pinNo];        //son190703 기존 t2. 해당 "4W Pin1"의 piece no

        pieceNetCnt[piece]++;   //son190703 기존 tsum

        pieceNet=pieceNetCnt[piece];

        //son190704-4WDUT16 piece별 4W net data도 저장
        if (piece <= MAX_PIECE)
        {
            ng4WSet[piece][pieceNet][_4W_PIECE]= (float)g_s4WNet[dutIdx].saData[net].wPiece;  //son221114 0 -> _4W_PIECE

            for(pin=0; pin< _4W_PIN_NUM; pin++)
                ng4WSet[piece][pieceNet][_4W_PIN1+pin] = (float)g_s4WNet[dutIdx].saData[net].waPin[pin];

            ng4WSet[piece][pieceNet][_4W_REF_LOW]  = (float)g_s4WNet[dutIdx].saData[net].dRefLow;   //son211005
            ng4WSet[piece][pieceNet][_4W_REF_AVG]  = (float)g_s4WNet[dutIdx].saData[net].dRefAvg;   //son211005
            ng4WSet[piece][pieceNet][_4W_REF_HIGH]  = (float)g_s4WNet[dutIdx].saData[net].dRefHigh;  //son211005
            ng4WSet[piece][pieceNet][_4W_MODE]     = (float)g_s4WNet[dutIdx].saData[net].n4wMode;
            ng4WSet[piece][pieceNet][_4W_TEST_MIN] = (float)g_s4WNet[dutIdx].saData[net].dTestMin;
            ng4WSet[piece][pieceNet][_4W_TEST_AVG] = (float)g_s4WNet[dutIdx].saData[net].dTestAvg;
            ng4WSet[piece][pieceNet][_4W_TEST_MAX] = (float)g_s4WNet[dutIdx].saData[net].dTestMax;

        }
        else{
            //error message  pin piece no?

        }
    }

    if( g_s4WNet[dutIdx].wCount>0)      //son190704-4WDUT16 piece별 piece net count를 저장
    {
        for(piece=1; piece<=MAX_PIECE;piece++){
            ng4WSet[piece][0][0]=(float)pieceNetCnt[piece];
        }
    }

    ///file write

    int col;
    for(piece=1; piece<=g_nPieceMax1; piece++) //SYLEE121202
    {

        if( ng4WSet[piece][0][0] <=0)
            continue;
        
        ::ZeroMemory(&fName, sizeof(fName));
        str.Format("D:\\Log\\4W-PinListPiece%d.CSV",piece  );

        strcat( fName , str );
        fp2 = fopen(fName,"wt");
        if(fp2 == NULL){
            str.Format(" ERORR NO 2017 :D:\\Log\\  folder check!,  %s making error!", fName);               
            AfxMessageBox(str);
            return;
        }        

        fprintf(fp2, " \n *4W Piece=,%d,  Total Pin=,%.0f  \n \n",piece, ng4WSet[piece][0][0]);

        for(pieceNet = 1; pieceNet <= ng4WSet[piece][0][0]; pieceNet++ )
        {
            fprintf(fp2, "%d, %d , ", pieceNet, ng4WSet[piece][pieceNet][_4W_PIECE]);   //son221114 0 -> _4W_PIECE

            //son Pin1, Pin2, Pin3, Pin4 출력
            for(pin=0; pin<_4W_PIN_NUM; pin++ ){  
                if (g_bUseLowerCalJig_4W)   //son220426_3
                    fprintf(fp2, "%.0f, ", ng4WSet[piece][pieceNet][_4W_PIN1+pin] +(SysSet12.m_nLowerJigStartPin -1));  //son220426_3
                else    
                    fprintf(fp2, "%.0f, ", ng4WSet[piece][pieceNet][_4W_PIN1+pin]);
            }
            fprintf(fp2, "  , " );

            //son Ref.Low, Ref.Avg, Ref.High 출력
            for(  col=_4W_REF_LOW; col<=_4W_REF_HIGH; col++ ){           //son210015
                fprintf(fp2, "%.2f ,",ng4WSet[piece][pieceNet][col]);
            }
            fprintf(fp2, "  , " );

            //son 4w Mode 출력
            fprintf(fp2, "%.0f ,",ng4WSet[piece][pieceNet][_4W_MODE]);
            fprintf(fp2, "  , " );

            //son Test.Min, Test.Avg, Test.Max 출력 
            for(  col=_4W_TEST_MIN; col<=_4W_TEST_MAX; col++ ){  
                fprintf(fp2, "%.2f ,",ng4WSet[piece][pieceNet][col]);
            }
            fprintf(fp2, "  \n");
        }
        fclose(fp2);

    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
}

//son type 11, 12: "4W_Per_Low.ini" 을  gd4W1_P1Low[][][]에 load/save
//son type 21, 22: "4W_Per_High.ini"을  gd4W1_P2High[][][]에 load/save
int CFileSysInfo::LoadSaveSub18(int type)//SYLEE160503
{

    int  i,k,tmp;
    
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 

    char str2[2048]; 
    ::ZeroMemory(str2, sizeof(str2));   

    //-------------------------------------------------------
    //sylee160801-6
    ::ZeroMemory(&fName, sizeof(fName));  //sylee160731-2
    str.Format("%s\\4W.INI",SysInfoView01.m_pStrFilePathBDL);  
    strcat( fName , str);  

    //son 4W.ini가 없으면 "4W_Per_Low.ini"나 "4W_Per_High.ini"도 필요없으므로 리턴한다.
    if(FileExists(fName) == FALSE){//sylee160731-2
        return 1;
    }
    //===================================================================
 

    ::ZeroMemory(&fName, sizeof(fName));

    if(type==_SAVELOW || type==_LOADLOW ){      //son211012 11:_SAVELOW   12:_LOADLOW
       str.Format("%s\\4W_Per_Low.ini",SysInfoView01.m_pStrFilePathBDL);  
    }
    else{// 21: _SAVEHIGH  22: _LOADHIGH
       str.Format("%s\\4W_Per_High.ini",SysInfoView01.m_pStrFilePathBDL);  
    }
    strcat( fName , str);  


    switch(type)
    {
        case _SAVELOW:  //11:  4W_Per_Low.ini DATASAVE    //son211012

            DeleteFile(fName);
            fp = fopen(fName,"wt");
            if(fp == NULL)
            {
                str.Format(" Error No 2021, \n Saving Error  \n %s file open error   !!!", fName);
                AfxMessageBox(str);
                return 0;
            }

            fprintf(fp, "%d\n", (int)gd4W1_P1Low[0][SET4W_LIMIT_USE]);         //son '-Low Use '   //son220914
            for(i=1; i<= (MAX_LIMIT4W_RNG + 1); i++)        //son211012 21 => MAX_LIMIT4W_RNG +1 
            {
                for(k=1; k<= LIMIT4W_SIGMA_COEF; k++)       //son211012 LIMIT4W_OFFSET => LIMIT4W_SIGMA_COEF
                {
                    if(gd4W1_P1Low[i][LIMIT4W_R_START]<=0){
                        gd4W1_P1Low[i][LIMIT4W_R_START]=0;
                        if(i!=1){
                            gd4W1_P1Low[i][k]=0;
                        }
                    }
                    if(gd4W1_P1Low[i][LIMIT4W_R_START]> MAX_4W_R){ //sylee180222  //son201109 200000-> MAX_4W_R
                        gd4W1_P1Low[i][LIMIT4W_R_START]=0;
                        gd4W1_P1Low[i][k]=0;
                    }
                    if(k< LIMIT4W_SIGMA_COEF){//son211012 
                        fprintf(fp, "  %d", (int)gd4W1_P1Low[i][k] );   //son211012 
                    }
                    else{
                        fprintf(fp, "  %2.1f", gd4W1_P1Low[i][k] ); //son211012 
                    }
                }            
            }

            break;

        case _LOADLOW:  // 12: 4W_Per_Low.ini DATALOAD    //son211012 

            ::ZeroMemory(&gd4W1_P1Low, sizeof(gd4W1_P1Low));

            //////////////////////////////////////////////////////////
            fp=fopen(fName,"rt");

            //---------------------------------------------------------
            //son "4W_Per_Low.ini" 가 없다면 새로 생성해서 fopen한다.
            //---------------------------------------------------------
            if(fp==NULL)    
            {   
                //  DeleteFile(fName);
                
                FILE *fpw = fopen(fName,"wt");  
                if(fpw == NULL)
                {
                    str.Format(" Error No 2041, \n   New file Saving Error  \n   %s file open error   !!!", fName);
                    AfxMessageBox(str);
                    return 0;
                }
                //son 첫번째 항목의 end range R은 MAX_4W_R 값으로 최초 파일을 생성
                gd4W1_P1Low[1][LIMIT4W_R_END]= MAX_4W_R;//sylee180222  //son201109 200000-> MAX_4W_R

                fprintf(fpw, "%d\n", (int) gd4W1_P1Low[0][SET4W_LIMIT_USE]);   //son '-Low Use ' 출력    //son220914
                for(i=1; i<= MAX_LIMIT4W_RNG; i++)  //son211012         //son 5개의 pin 20세트 100개 값이  한 줄로 출력된다.
                {
                    for(k=1; k<= LIMIT4W_SIGMA_COEF; k++)   //son211012 LIMIT4W_OFFSET -> LIMIT4W_SIGMA_COEF
                    {                 
                        if(k< LIMIT4W_SIGMA_COEF)   //son211012
                            fprintf(fpw, "  %d", (int)gd4W1_P1Low[i][k] );     //son211012                     
                        else 
                            //son Sigma계수. 3.0 처럼 소수 첫째자리까지 표시
                            fprintf(fpw, "  %2.1f", gd4W1_P1Low[i][k] );       //son211012
                    }            
                }
                fclose(fpw);

                //son20200218 fopen(fName, "rt") 성공시에도 같은 file을 같은 fp로 fopen하는 문제가 있었음.
                //    아래와 같이 fp=NULL일 때에만 fopen()을 다시 시도하도록 if (fp==NULL) 영역 안으로 이동함. 
                fp=fopen(fName,"rt");
                if(fp==NULL)
                {
                    //son 두번 연속 fName에 대한 fopen()이 실패하면 에러처리한다.
                    str.Format(" Error No 2022, \n   Loading Error  \n   %s file open retry error   !!!", fName);
                    AfxMessageBox(str);
                    return 0;
                }
            }

            //===================================================================
            //son  READ gd4W1_P1Low[i][k] DATA 
            //===================================================================

            tmp=fscanf(fp, "%s\n", str);
            if(tmp==1){             //son '-Low Use' checkBox On 이면
                gd4W1_P1Low[0][SET4W_LIMIT_USE]=(int)atof(str);  //son220914
                if(gd4W1_P1Low[0][SET4W_LIMIT_USE]!=1){  //son220914
                    gd4W1_P1Low[0][SET4W_LIMIT_USE]=0;   //son220914
                }
            }
            else{
                gd4W1_P1Low[0][SET4W_LIMIT_USE]=0;   //son220914
            }

            for(i=1; i<= (MAX_LIMIT4W_RNG +1); i++)     //son211012 21 -> (MAX_LIMIT4W_RNG +1)
            {
                for(k=1; k<= LIMIT4W_SIGMA_COEF ; k++)  //son211012 LIMIT4W_OFFSET -> LIMIT4W_SIGMA_COEF (Low에도 SigmaCoef추가. 사장님지시)
                {    
                    if(k==LIMIT4W_SIGMA_COEF){          //son211012 LIMIT4W_OFFSET -> LIMIT4W_SIGMA_COEF
                        tmp=fscanf(fp, "%s\n", str2);       //son190620  str ->str2로 변경. fscanf 버퍼문제
                    }
                    else{
                        tmp=fscanf(fp, "%s", str2);         //son190620  str ->str2로 변경. fscanf 버퍼문제
                    }

                    if(tmp==1){
                        if(k< LIMIT4W_SIGMA_COEF)   //son211012
                            gd4W1_P1Low[i][k]=(int)atof(str2);     //son190620  str ->str2로 변경. fscanf 버퍼문제
                        else
                            gd4W1_P1Low[i][k]=atof(str2);      //son211012 
                    }
                    else{
                        gd4W1_P1Low[i][k]=0;
                    }    

                    if(gd4W1_P1Low[i][k]<=0){
                        gd4W1_P1Low[i][k]=0;
                    }

                    if(gd4W1_P1Low[i][k]> MAX_4W_R){//sylee180222  //son201109 200000-> MAX_4W_R
                        gd4W1_P1Low[i][k]=0;
                    }
                    if(gd4W1_P1Low[i][LIMIT4W_R_START]==0){
                        if(i!=1){
                            gd4W1_P1Low[i][k]=0;
                        }
                    }
                }            
            } 

            break;


        case _SAVEHIGH:  // 21:  4W_Per_High.ini DATASAVE :          //son211012 
            DeleteFile(fName);
            fp = fopen(fName,"wt");
            if(fp == NULL){
                str.Format(" Error No 2023, \n   Save Error  \n   %s file open error   !!!", fName);
                AfxMessageBox(str);
                return 0;
            }           

            //  fprintf(fp, "%d\n",gd4W1_P2High[0][SET4W_LIMIT_USE]);  
            //    fprintf(fp, "%d  %d %d  %d  %d\n",
            //          gd4W1_P2High[0][0],gd4W1_P2High[0][1],gd4W1_P2High[0][2],gd4W1_P2High[0][3],gd4W1_P2High[0][4]);
            fprintf(fp, "%d  %d %d  %d  %d\n",
                    (int)gd4W1_P2High[0][SET4W_LIMIT_USE],         //son ModeSub18 +High Use(m_check2) 저장)         //son220914
                    (int)gd4W1_P2High[0][SET4W_SIGMA_COEF_USE],        //son ModeSub17의 Sigma Coefficient Enable이면 1      //son210916
                    (int)gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE],   //son ModeSub17의 'EJECT COUNT  LOW  HIGH' 값이 설정되면 1   //son210916
                    (int)gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE],  //son ModeSub18:: All% Use (m_check4) 저장        //son210916
                    (int)gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]); //son ModeSub18:: All % Value  (m_edit4) 저장     //son210916

            for(i=1; i<=(MAX_LIMIT4W_RNG + 1); i++)   //son211012 21 => MAX_LIMIT4W_RNG +1
            { 
                for(k=1; k<= LIMIT4W_SIGMA_COEF; k++)
                {
                    if(gd4W1_P2High[i][LIMIT4W_R_START]<=0){
                        gd4W1_P2High[i][LIMIT4W_R_START]=0;
                        if(i!=1){
                            gd4W1_P2High[i][k]=0;
                        }
                    }
                    if(gd4W1_P2High[i][LIMIT4W_R_START]> MAX_4W_R){//sylee180222   //son201109 200000-> MAX_4W_R
                        gd4W1_P2High[i][LIMIT4W_R_START]=0;
                        gd4W1_P2High[i][k]=0;
                    }
                    if(k< LIMIT4W_SIGMA_COEF){//sylee170819-1
                        fprintf(fp, "  %d", (int)gd4W1_P2High[i][k] ); //sylee170819-1
                    }
                    else{
                        fprintf(fp, "  %2.1f", gd4W1_P2High[i][k] ); //sylee170819-1
                    }

                }            
            }

            break;



        case _LOADHIGH:  // 22: 4W_Per_High.ini DATALOAD : //son211012

            ::ZeroMemory(&gd4W1_P2High, sizeof(gd4W1_P2High));       
            //////////////////////////////////////////////////////////
            fp=fopen(fName,"rt");

            //---------------------------------------------------------
            //son "4W_Per_High.ini" 가 없다면 새로 생성해서 fopen한다.
            //---------------------------------------------------------
            if(fp==NULL)
            {
                //DeleteFile(fName);
                
                FILE *fpw  = fopen(fName,"wt");
                if(fpw == NULL)
                {
                    str.Format(" Error No 2042, \n   Saving New file Error  \n   %s  file open error   !!!", fName);
                    AfxMessageBox(str);
                    return 0;
                }

                //son 첫번째 항목의 end range R은 MAX_4W_R 값으로 최초 파일을 생성
                gd4W1_P2High[1][LIMIT4W_R_END]= MAX_4W_R; //sylee180222 //son201109 200000-> MAX_4W_R

                fprintf(fpw, "%d  %d %d  %d  %d\n",(int)gd4W1_P2High[0][SET4W_LIMIT_USE],  //son '+High Use' //son220914
                                                (int)gd4W1_P2High[0][SET4W_SIGMA_COEF_USE],     //son ModeSub17의 Sigma Coefficient Enable이면 1 //son210916
                                                (int)gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE],     //son ModeSub17의 'EJECT COUNT  LOW  HIGH' 값이 설정되면 1  //son210916
                                                (int)gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE],     //son 'All% Use'   //son210916
                                                (int)gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]);    //sylee170819-1 //son 'All %' Value    //son210916

                for(i=1; i<= (MAX_LIMIT4W_RNG +1); i++)    //son211012 21 -> MAX_LIMIT4W_RNG+1
                {
                    for(k=1; k<= LIMIT4W_SIGMA_COEF; k++)//sylee160604-1             
                    {
                        if(k< LIMIT4W_SIGMA_COEF){
                            fprintf(fpw, "  %d",(int) gd4W1_P2High[i][k] ); //sylee170819-1   
                        }
                        else{
                            //son Sigma계수. 3.0 처럼 소수 첫째자리까지 표시
                            fprintf(fpw, "  %2.1f", gd4W1_P2High[i][k] ); //sylee170819-1  int->double
                        }

                    }            
                }
                fclose(fpw);        

                //son20200218 fopen(fName, "rt") 성공시에도 같은 file을 같은 fp로 fopen하는 문제가 있었음.
                //    아래와 같이 fp=NULL일 때에만 fopen()을 다시 시도하도록 if (fp==NULL) 영역 안으로 이동함. 
                fp=fopen(fName,"rt");
                if(fp==NULL){
                    //son 두번 연속 fName에 대한 fopen()이 실패하면 에러처리한다.
                    str.Format(" Error No 2024, \n   Loading Error  \n   %s   file open retry error   !!!", fName);
                    AfxMessageBox(str);
                    return 0;
                }
            }


            //===================================================================
            //son  READ  gd4W1_P2High[i][k] DATA 
            //===================================================================


            //tmp=fscanf(fp, "%s\n", str);
            tmp=fscanf(fp, "%s ", str);
            if(tmp==1){                 //son '+High Use' checkBox On 이면
                gd4W1_P2High[0][SET4W_LIMIT_USE]=(int)atof(str);     //son220914
                if(gd4W1_P2High[0][SET4W_LIMIT_USE]!=1){ //son220914
                    gd4W1_P2High[0][SET4W_LIMIT_USE]=0;  //son220914
                }
            }
            else{
                gd4W1_P2High[0][SET4W_LIMIT_USE]=0;  //son220914
            }

            fscanf(fp, "%s ", str);     
            gd4W1_P2High[0][SET4W_SIGMA_COEF_USE]=(int)atof(str);  //son ModeSub17의 Sigma Coefficient Enable//son210916
            fscanf(fp, "%s ", str);     
            gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=(int)atof(str);  //son ModeSub17의 'EJECT COUNT  LOW  HIGH'   //son210916
            fscanf(fp, "%s ", str);     
            gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]=(int)atof(str);  //son 'All% Use' checkbox state //son210916
            fscanf(fp, "%s \n", str);       
            gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=(int)atof(str);  //son 'All %' Value //son210916



            for(i=1; i<= (MAX_LIMIT4W_RNG +1); i++)     //son211012 21 -> (MAX_LIMIT4W_RNG +1)
            {
                for(k=1; k<= LIMIT4W_SIGMA_COEF; k++)   //sylee160604-1  
                {
                    if(k== LIMIT4W_SIGMA_COEF){  //sylee160604-1     4->5
                        tmp=fscanf(fp, "%s\n", str2);       //son190620  str ->str2로 변경. fscanf 버퍼문제
                    }
                    else{
                        tmp=fscanf(fp, "%s", str2);         //son190620  str ->str2로 변경. fscanf 버퍼문제
                    }

                    if(tmp==1){
                        if(k< LIMIT4W_SIGMA_COEF){//sylee170819-1 
                            gd4W1_P2High[i][k]=(int)atof(str2);//sylee170819-1  //son190620  str ->str2로 변경. fscanf 버퍼문제
                        }
                        else{//sylee170819-1 
                            gd4W1_P2High[i][k]=atof(str2);//sylee170819-1       //son190620  str ->str2로 변경. fscanf 버퍼문제
                        }//sylee170819-1 

                    }
                    else{
                        gd4W1_P2High[i][k]=0;
                    }

                    if(gd4W1_P2High[i][k]<=0){
                        gd4W1_P2High[i][k]=0;
                    }

                    if(gd4W1_P2High[i][k]> MAX_4W_R){  //sylee180222 //son201109 200000-> MAX_4W_R
                        gd4W1_P2High[i][k]=0;
                    }
                    if(gd4W1_P2High[i][LIMIT4W_R_START]==0){
                        if(i!=1){
                            gd4W1_P2High[i][k]=0;
                        }
                    }

                }            
            }
            break;

        default :
            break;
    }
    fclose(fp);

    int dutIdx = get4wDutIdx(); // g_s4WNet의 index. 0부터 시작함.  //son190704-4WDUT16 
    LoadSaveSub17_test(dutIdx);     //son190704-4WDUT16  중복코드를 함수호출로 변경

 
    return 1;
}


  
 



//son "\\SETUP2\\BlockDis.ini"를  SysInfo19에 로딩하거나 Save
int CFileSysInfo::LoadSaveSub19(int type, int nFileType)
{

    int  i,k;   
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 

    ::ZeroMemory(&fName, sizeof(fName));
//SYLEE130601   strcat( fName , "C:\\ACE300\\Setup\\BlockDis.ini" );
    
    if(nFileType==2){
       strcat( fName , "Z:\\BLOCKDISABLE.TXT" );
    }
    else{
        str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE121202
        strcat( fName ,str);
        strcat( fName , "\\SETUP2\\BlockDis.ini" );
    }

    
    //son BDLPath\\\SETUP2\\Block.ini 를 SysInfoBlock에 로딩
    LoadSaveBlockInfo(_LOAD);//sylee160126-5   //son220824_5 LoadSaveSub13() -> LoadSaveBlockInfo() 

    //son BDLPath\\\SETUP2\\Piece.ini 를 SysInfoPiece에 로딩
    LoadSavePieceInfo(_LOAD); //sylee160126-5  //son210430   LoadSaveSub25() -> LoadSavePieceInfo()


    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
            // file 삭제 
            DeleteFile(fName);
            fp = fopen(fName,"wt");
            if(fp == NULL)
            { 
                //son SysInfo19: Block을 row, Piece는 col로 하는 구조임...
         
                SysInfo19.m_nCellX=SysInfoBlock.m_nBlock;//block
                SysInfo19.m_nCellY=SysInfoPiece.m_nPiece;//piece       
                SysInfo19.m_nCellTotal=SysInfo19.m_nCellX*SysInfo19.m_nCellY;
                SysInfo19.m_nType=1;
                SysInfo19.m_nUse=0;
                FileSysInfo01.LoadSaveSub19(1,1); //save//sylee160203

                fp = fopen(fName,"wt");//sylee160203
                if(fp == NULL)  //sylee160203           
                {
                    str.Format(" Error No 2218, \n\n\n  %s FILE,  Save Error !!! \n", fName);
                    AfxMessageBox(str);
                }
                return 0;
            }

            fprintf(fp, "%10d\n",SysInfo19.m_nCellX);
            fprintf(fp, "%10d\n",SysInfo19.m_nCellY); 
            fprintf(fp, "%10d\n",SysInfo19.m_nCellTotal);
            fprintf(fp, "%10d\n",SysInfo19.m_nType);
            fprintf(fp, "%10d\n",SysInfo19.m_nUse);
     
            if( SysInfo19.m_nUse!=1){
                   for(i=1;i<210;i++){//sylee131122
                        for(k=1;k<210;k++){
                              SysInfo19.m_nData[i][k] =0; 
                        }
                    }
            }
            for(i=1;i<=SysInfo19.m_nCellX;i++){
                for(k=1;k<=SysInfo19.m_nCellY;k++){
                    if(SysInfo19.m_nData[i][k]!=1){
                        SysInfo19.m_nData[i][k]=0;
                    }
                    fprintf(fp, "%10d",SysInfo19.m_nData[i][k]);
                }
                fprintf(fp,"\n");
            }

            break;




        case _LOAD: //2: LOAD  //son211012 

            fp=fopen(fName,"rt");
            if(fp==NULL)
            {

                SysInfo19.m_nCellX=SysInfoBlock.m_nBlock;//block
                SysInfo19.m_nCellY=SysInfoPiece.m_nPiece;   //piece       
                SysInfo19.m_nCellTotal=SysInfo19.m_nCellX*SysInfo19.m_nCellY;
                SysInfo19.m_nType=1;
                SysInfo19.m_nUse=0;

                FileSysInfo01.LoadSaveSub19(1,1); //save//sylee160203

                fp=fopen(fName,"rt");//sylee160203
                if(fp == NULL){ //sylee160203
                    str.Format( "   Error No 2218, \n\n       FILE Loading Error !!!   \n\n     %s \n ",fName ); 
                    AfxMessageBox(str);     
                }
                return 0;
            }

            fscanf(fp, "%10d\n", &SysInfo19.m_nCellX);  //block
            fscanf(fp, "%10d\n", &SysInfo19.m_nCellY);  //piece

            if((SysInfo19.m_nCellX==SysInfoBlock.m_nBlock)&& (SysInfo19.m_nCellY==SysInfoPiece.m_nPiece)) //piece  //sylee160126-5
            {

                fscanf(fp, "%10d\n", &SysInfo19.m_nCellTotal);
                fscanf(fp, "%10d\n", &SysInfo19.m_nType);
                fscanf(fp, "%10d\n", &SysInfo19.m_nUse);

                SysInfo19.m_nCheck=0;//sylee131117


                for(i=1;i<301;i++){//sylee131122
                    for(k=1;k<201;k++){
                        SysInfo19.m_nData[i][k] =0; 
                    }
                }

                if(SysInfo19.m_nUse==1){//sylee131123
                    for(i=1;i<=SysInfo19.m_nCellX;i++){
                        for(k=1;k<=SysInfo19.m_nCellY;k++){
                            fscanf(fp, "%10d", &SysInfo19.m_nData[i][k] );       

                            if(SysInfo19.m_nData[i][k]==1){//sylee131117
                                SysInfo19.m_nCheck=1;//sylee131117
                            }
                        }
                    } 
                }

            }
            else// if((SysInfo19.m_nCellX==SysInfoBlock.m_nBlock)&& (SysInfo19.m_nCellY==SysInfoPiece.m_nPiece)){ //piece  //sylee160126-5
            {

                SysInfo19.m_nCellX=SysInfoBlock.m_nBlock;  //sylee160126-5
                SysInfo19.m_nCellY=SysInfoPiece.m_nPiece;  //block //sylee160126-5

                SysInfo19.m_nCellTotal=SysInfo19.m_nCellX*SysInfo19.m_nCellY; //sylee160126-5
                SysInfo19.m_nType=1; //sylee160126-5
                SysInfo19.m_nUse=0; //sylee160126-5

                SysInfo19.m_nCheck=0;//sylee160126-5

                for(i=1;i<301;i++){//sylee160126-5
                    for(k=1;k<201;k++){//sylee160126-5
                        SysInfo19.m_nData[i][k] =0; //sylee160126-5
                    }
                }             

            }


            int nCo1;//sylee170417-1

            //son PieceDisable이 use일 때
            if(SysInfo19.m_nUse)
            {
                //son PieceDisable Grid에서 SysInfo19.m_nCellX는  block 개수
                for( i = 1 ; i <=SysInfo19.m_nCellX ; i++) 
                { 
                    //son PieceDisable Grid에서 SysInfo19.m_nCellY는  piece 개수
                    nCo1=0;//sylee170417-1
                    for( int  k = 1 ; k <=SysInfo19.m_nCellY ;  k++) 
                    { 
                        if(SysInfo19.m_nData[i][k]==1){
                            nCo1++;//sylee170417-1
                        }
                    }
                    if(nCo1==SysInfo19.m_nCellY){//sylee170417-1
                        nBlockMaxLast1=i-1;//sylee170417-1
                    }
                    else{
                        nBlockMaxLast1=SysInfo19.m_nCellX;//sylee170417-1   nBlockMax
                    } 
                }
            } 
            //son220707 nBlockMaxLast1와 nBlockMax 불일치났을때 4W log 뒷부분이 모두 NG되는 문제점 수정
            //          PieceDisable이 Not use일 때에는 nBlockMaxLast1 초기화하는 코드가 없어서 추가함.
            else 
                nBlockMaxLast1 = SysInfo19.m_nCellX; //son220707 현재 block 개수

            break;

     
        default :
            break;
    }

    fclose(fp);
    return 1;
}


//son "C:\\ACE400\\Setup\\AutoCount.ini" 를 SysInfo20에 로딩하거나 Save
int CFileSysInfo::LoadSaveSub20(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;
 
    ::ZeroMemory(&fName, sizeof(fName));
  
    str.Format("%s\\Setup\\AutoCount.ini", 
                    SysInfoView01.m_pStrFilePath1);  //son210514  "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName, str); //son20111)    //son2105148
 
    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
            fp = fopen(fName,"wt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fprintf(fp, "%10d\n",SysInfo20.m_nUse);
            fprintf(fp, "%10d\n",SysInfo20.m_nPass); 
            fprintf(fp, "%10d\n",SysInfo20.m_nOpen); 
            fprintf(fp, "%10d\n",SysInfo20.m_nShort);
            fprintf(fp, "%10d\n",SysInfo20.m_nOpenShort); 
            fprintf(fp, "%10d\n",SysInfo20.m_nFail); 
            fprintf(fp, "%10d\n",SysInfo20.m_nTotle); 

            break;

        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fscanf(fp, "%10d\n", &SysInfo20.m_nUse);
            fscanf(fp, "%10d\n", &SysInfo20.m_nPass);       
            fscanf(fp, "%10d\n", &SysInfo20.m_nOpen);
            fscanf(fp, "%10d\n", &SysInfo20.m_nShort );
            fscanf(fp, "%10d\n", &SysInfo20.m_nOpenShort );     
            fscanf(fp, "%10d\n",  &SysInfo20.m_nFail);
            fscanf(fp, "%10d\n", &SysInfo20.m_nTotle ); 

            break;

        default :
            break;
    }
    fclose(fp);
    return 1;
}


//son BDLPath\\\SETUP2\\Piece.ini 를 SysInfoPiece에 로딩하거나 Save한다.
int CFileSysInfo::LoadSavePieceInfo(int type)   //son210430 LoadSaveSub25 ->LoadSavePieceInfo
{
 
    int  i,k, nTotPiece;;
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s", SysInfoView01.m_pStrFilePathBDL);//SYLEE121202
    strcat( fName ,str);
    strcat( fName , "\\SETUP2\\Piece.ini" );     

    
    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           

            nTotPiece=SysInfoPiece.m_nRow * SysInfoPiece.m_nCol;

            int nPieceCo1;//sylee180905-1
            nPieceCo1=(int)DEF_MAX_PIECE1-1;//sylee180905-1 //son220823_6
            if( nTotPiece >nPieceCo1){
                str.Format("   ERROR 901,  piece  total max =%d  over!" , nPieceCo1); //sylee180905-1
                AfxMessageBox(str, MB_OK);//sylee180905-1
                return 0;
            }


            /*
               if( DEF_MAX_PIECE1<300){//SYLEE170803
               if(nTotPiece>200){//SYLEE17032
               AfxMessageBox("  ERROR 901,  piece  total max 200 over! ", MB_OK);           
               return 0;
               } 
               }
               else{
               if(nTotPiece>300){//SYLEE170803
               AfxMessageBox("  ERROR 901,  piece  total max 300 over! ", MB_OK);           
               return 0;
               } 
               }
               */

            fp = fopen(fName,"wt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fprintf(fp, "%10d\n",SysInfoPiece.m_nPiece);
            fprintf(fp, "%10d\n",SysInfoPiece.m_nRow);   
            fprintf(fp, "%10d\n",SysInfoPiece.m_nCol);
            fprintf(fp, "%10d\n",SysInfoPiece.m_nType);

            for(i=0;i<SysInfoPiece.m_nRow;i++){
                for(k=0;k<SysInfoPiece.m_nCol;k++){
                    fprintf(fp, "%10d",SysInfoPiece.m_nData[i][k]);
                }
                fprintf(fp,"\n");
            }

            fprintf(fp, "%10d\n",SysInfoPiece.m_nRowDel);    
            fprintf(fp, "%10d\n",SysInfoPiece.m_nColDel);

            break;

        case _LOAD: //2: LOAD  //son211012 
            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fscanf(fp, "%10d\n", &SysInfoPiece.m_nPiece);
            fscanf(fp, "%10d\n", &SysInfoPiece.m_nRow);          
            fscanf(fp, "%10d\n", &SysInfoPiece.m_nCol);      
            fscanf(fp, "%10d\n", &SysInfoPiece.m_nType);         

            for(i=0;i<SysInfoPiece.m_nRow;i++){
                for(k=0;k<SysInfoPiece.m_nCol;k++){
                    fscanf(fp, "%10d", &SysInfoPiece.m_nData[i][k]);
                }
            } 
            fscanf(fp, "%10d\n", &SysInfoPiece.m_nRowDel);           
            fscanf(fp, "%10d\n", &SysInfoPiece.m_nColDel);

            nTotPiece=SysInfoPiece.m_nRow * SysInfoPiece.m_nCol; 


            if (SysInfoPiece.m_nPiece > MAX_PIECE) //son220823_2 begin:
            { 
                str.Format(" Error No 2005-2 !\n %s file. SysInfoPiece.m_nPiece(%d) is over %d(MAX_PIECE)", 
                        fName, SysInfoPiece.m_nPiece, MAX_PIECE); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            }  //son220823_2 end 

#if 0
            if( DEF_MAX_PIECE1<300){//SYLEE170803
                if(nTotPiece>200){//SYLEE17032
                    AfxMessageBox("  ERROR 901-2,  piece  total max 200 over! ", MB_OK);            
                    SysInfoPiece.m_nRow=10;
                    SysInfoPiece.m_nCol=10;
                } 
            }
            else{
                if(nTotPiece>300){//SYLEE170803
                    AfxMessageBox("  ERROR 901-2,  piece  total max 300 over! ", MB_OK);            
                    SysInfoPiece.m_nRow=10;
                    SysInfoPiece.m_nCol=10;
                } 
            }
#else
            if (nTotPiece > MAX_PIECE)  //son220823_2 begin: 
            { 
                str.Format(" Error No 2005-3 !\n %s file. nTotPiece(%d) is over %d(MAX_PIECE)", 
                        fName, nTotPiece, MAX_PIECE); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            }   
            if (nTotPiece != SysInfoPiece.m_nPiece)  
            { 
                str.Format(" Error No 2005-4 !\n %s file. nTotPiece(%d) is not same with SysInfoPiece.m_nPiece(%d)", 
                        fName, nTotPiece, SysInfoPiece.m_nPiece); //error messege
                AfxMessageBox(str, MB_OK);
                MyTrace(PRT_BASIC, str);
                return 0;
            }   
            //son220823_2 end
#endif
            


            break;

        default :
            break;
    }

    fclose(fp); 
    return 1;
}


//son "./Setup/Service.ini" 파일을 loading 한다.
//son  type 1: Save,  2: Load
int CFileSysInfo::LoadSaveSer11(int type)
{
    FILE *fp;
    char *fName;
    CString str;


    fName="./Setup/Service.ini";
    
    
    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           

        fp = fopen(fName,"wt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }

        fprintf(fp, "%10d\n",SysInfoSer11.m_nPriMode);
        fprintf(fp, "%10d\n",SysInfoSer11.m_nBox); 
        fprintf(fp, "%10d\n",SysInfoSer11.m_nPinTot); 
       
        break;

    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }

        fscanf(fp, "%10d\n", &SysInfoSer11.m_nPriMode );
        fscanf(fp, "%10d\n", &SysInfoSer11.m_nBox );    
        fscanf(fp, "%10d\n", &SysInfoSer11.m_nPinTot );          
 
        break;

    default :
        break;


    }
    fclose(fp);
    return 1;
}


int CFileSysInfo::LoadSaveSer12(int type)
{
    int     i;
    FILE    *fp; 
    char    fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str;
 
    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\Setup\\Connect.ini", SysInfoView01.m_pStrFilePath1);    //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat( fName , str ); //son20111)     //son2105148
 

    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           

        fp = fopen(fName,"wt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s file Data Saving Error! fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }

//sylee180810-1-DUT16       SysInfoSer12.m_nSetMode=1;
//sylee180810-1-DUT16        fprintf(fp, "%10d\n",SysInfoSer12.m_nSetMode);
        fprintf(fp, "%10d\n",SysInfoSer12.m_nMutiNo);
            
        for(i=0;i<SysInfoSer12.m_nMutiNo;i++){
            fprintf(fp, "%10d%10d%10d%10d\n",SysInfoSer12.m_nPin[i][0],SysInfoSer12.m_nPin[i][1],
                                         SysInfoSer12.m_nPin[i][2],SysInfoSer12.m_nPin[i][3]);
        }
        break;

    case _LOAD: //2: LOAD  //son211012 
        fp=fopen(fName,"rt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2005 !\n %s file Data Loading Error! fopen() fail.", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }
        
//sylee180810-1-DUT16   fscanf(fp, "%10d\n", &SysInfoSer12.m_nSetMode);
        fscanf(fp, "%10d\n", &SysInfoSer12.m_nMutiNo);
             
        
        for(i=0;i<20;i++){  //fix 20? -> SysInfoPieceRange.m_nPieceCount
            fscanf(fp, "%10d", &SysInfoSer12.m_nPin[i][0] );             
            fscanf(fp, "%10d", &SysInfoSer12.m_nPin[i][1]);          
            fscanf(fp, "%10d", &SysInfoSer12.m_nPin[i][2]);          
            fscanf(fp, "%10d\n", &SysInfoSer12.m_nPin[i][3]);
                 
        } 
        break;

    default :
        break;
    }
    fclose(fp); 
    return 1;
}
 





                                 

//son Calibration Reference Data Load or Save
//  ex) "C:\\ACE500\\SETUP\\CAL\\CALR%02dB.INI" 에서 이전의 Calibration Ref 값 Load 또는 Save
//son  type         1:Save  2: Load
//son  m_nMode1     현재 테스트 전압  ex) 6= "6 Open 10 V" 
//son  part         0:PART_UPPER, 1:PART_LOWER, 2: PART_MIXED
bool g_bMsgFlag_LoadSaveSer13 = false;  // 1회성 메시지를 위해  
int CFileSysInfo::LoadSaveSer13(int type, int mode, int part) // type 1,2 ,  mode 1~360 //son220412_2
{
 
    int  RList, nRcount,  k;
    CString str, Str, str2;
    FILE *fp,*fp2,*fp3, *fp4, *fp5; // hdpark230105 spark
    char  fNameCal[FILE_PATH_SIZE];     //son Cal Data for Upper        //son210524 200: FILE_PATH_SIZE
    char  fName2[FILE_PATH_SIZE];    //son RealR, RawR for Upper     //son210524
    char  fName3[FILE_PATH_SIZE];    //son Full Call Data for Upper  //son210524
    char  fName4[FILE_PATH_SIZE];    //son Cal data  for Lower       //son220411 
    char  fName5[FILE_PATH_SIZE];    //hdpark230105 spark

    //son UseCalData가 UPPER_ONLY인데 part가 UPPER가 아니면   PART_UPPER로 강제 설정해서 동작한다.
    if (SysSet13.m_nUseCalData == USECAL_UPPER_ONLY) //son220419 begin
    {
        if (part == PART_LOWER && g_bMsgFlag_LoadSaveSer13 == false)
        {
            str.Format("Error No 2113 !\n\nLoadSaveSer13(%s, mode=%d, part=%d:%s). SysSet13.m_nUseCalData=UPPER_ONLY, can't have part=%d\n\n",
                                (type==_SAVE)? "_SAVE":"_LOAD", mode, part, ((part== PART_UPPER)? "UPPER": 
                                                                             (part== PART_LOWER)? "LOWER": "MIXED" ), part);
            str2 += str;
            str.Format("Please change the Menu->MAP->Set1->UsaCalData to \"Upper_Lower\".\n\n");
            str2 += str;
            str.Format("or Move to Menu->MANUAL->R, and turn off the \"Use LowerPin\" CheckBox.\n");
            str2 += str;
            MyTrace(PRT_LEVEL1, str2);
            AfxMessageBox(str2, MB_OK);
            g_bMsgFlag_LoadSaveSer13 = true;
        }
        if(part != PART_SPARK)	part = PART_UPPER;	// hdpark230105 spark      
    }   //son220419 end
    
    ::ZeroMemory(&fNameCal, sizeof(fNameCal));
    ::ZeroMemory(&fName2, sizeof(fName2));
    ::ZeroMemory(&fName3, sizeof(fName3));
    ::ZeroMemory(&fName4, sizeof(fName4));
    ::ZeroMemory(&fName5, sizeof(fName5));//hdpark230105 spark
      
    if(mode<1  ||  mode> MAX_TEST_MODE)  //son220103 360->MAX_TEST_MODE
    {    
        AfxMessageBox(" Error No 2003 !   mode select?", MB_OK); //error messege            
        return 0;
    }
 
    Str.Format("%s\\SETUP\\CAL\\CALR%02dB.INI", SysInfoView01.m_pStrFilePath1, mode ); //son210514
    strcat( fNameCal ,  Str);      //son ex) fNameCal  : C\ACE500\SETUP\CAL\CALR01B.INI

    Str.Format("%s\\SETUP\\CAL\\CALR%02dC.TXT", SysInfoView01.m_pStrFilePath1, mode ); //son210514
    strcat( fName2 , Str );     //son ex) fName2 : C\ACE500\SETUP\CAL\CALR01C.TXT

    Str.Format("%s\\SETUP\\CAL\\CALR%02dA.INI", SysInfoView01.m_pStrFilePath1, mode ); //son210514
    strcat( fName3 , Str );     //son ex) fName3 : C\ACE500\SETUP\CAL\CALR01A.INI
        
    Str.Format("%s\\SETUP\\CAL\\CALR%02dD.INI", SysInfoView01.m_pStrFilePath1, mode ); //son220412_2
    strcat( fName4 , Str );     //son ex) fName4 : C\ACE500\SETUP\CAL\CALR01D.INI

    Str.Format("%s\\SETUP\\CAL\\CALR%02dK.INI", SysInfoView01.m_pStrFilePath1, mode ); //hdpark230105 spark
    strcat( fName5 , Str );     //hdpark230105 spark ex) fName5 : C\ACE500\SETUP\CAL\CALR01K.INI

//=================================================================================================================
    int part2;  //son220412
    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           

            //son _SAVE일 때에는 part=PART_MIDEX 인 케이스가 없어야 한다.
            if (part == PART_UPPER)
            {
                //-----------------------------------------------------
                //son fNameCal  : C\ACE400\SETUP\CAL\CALR%02dB.INI(상부 CalData) 생성
                //-----------------------------------------------------
                //son    SysInfoSer13.m_nData[PART_UPPER][mode][1:CAL_USE    ][RList] : Use 1,0 
                //       SysInfoSer13.m_nData[PART_UPPER][mode][2:CAL_REF_R  ][RList] : Ref.R
                //       SysInfoSer13.m_nData[PART_UPPER][mode][3:CAL_REF_RAWR][RList] : Ref.Avg 
                //       SysInfoSer13.m_nData[PART_UPPER][mode][4:CAL_VC     ][RList] : dVC1[0][RList] 
                //       SysInfoSer13.m_nData[PART_UPPER][mode][5:CAL_I      ][RList] : dI1[0][RList] 
                fp = fopen(fNameCal,"wt");         // C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI
                if(fp == NULL)
                { 
                    Str.Format("Error No2003 !  \n  %s Saving Error!   fopen fail", fNameCal);
                    AfxMessageBox(Str, MB_OK); //error messege          
                    return 0;
                }
                nRcount=0;
                for(RList=1;RList<=MAX_CAL_RLIST;RList++)      //son220408 128: MAX_CAL_RLIST
                { 
                    for (k=1; k<= CAL_I; k++)     //son220407 5:CAL_I
                        fprintf(fp, "%15.2f    ",   SysInfoSer13.m_nData[PART_UPPER][mode][k][RList] );
                    
                    fprintf(fp, "\n" );

                    if( SysInfoSer13.m_nData[PART_UPPER][mode][CAL_USE][RList]==1){ //son220407 1: CAL_USE
                        nRcount++;                 
                    }
                }
                fclose(fp); 


                //-----------------------------------------------------
                //son fName2 : C\ACE400\SETUP\CAL\CALR%02dC.TXT 생성
                //-----------------------------------------------------
                //son TEST1 ~ TEST10까지의 값을 "CALR%02dC.INI"에 Write 한다.
                    
                fp2 = fopen(fName2,"wt");       // C:\\ACE400\\SETUP\\CAL\\CALR%02dC.INI
                if(fp2 == NULL)
                { 
                    Str.Format("Error No2003 !  \n  %s Saving Error!   fopen fail", fName2);
                    AfxMessageBox(Str, MB_OK); //error messege              
                    return 0;
                }
                for(RList=1;RList<=MAX_CAL_RLIST;RList++)      //son220408 128: MAX_CAL_RLIST
                { 
                    //son 헤더출력 -----
                    if( RList==1){  
                        CTime curTime = CTime::GetCurrentTime();    
                        fprintf(fp2, "#>FILE MAKE DATE =>  YEAR:%04d/  Month:%02d/  Day:%02d    : Hour %02d : Minute %02d :  Second %02d  \n\n ", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(),curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
                        fprintf(fp2, "            USE         REAL-R          RAW-R             VC              I          TEST1         TEST2          TEST3         TEST4          TEST5  :       TEST6          TEST7          TEST8          TEST9          TEST10     \n\n");

                    }

                    //son Use, Ref, RefR, VC, I, Test1~ Test10 출력 -----
                    for (k=1; k<= (CAL_TEST10 +1); k++)       //son220407 15:CAL_TEST10
                    {                                         // 기존에 "CALR%02dC.INI"에 16개를 출력했기 때문에 호환을 위해 하나 더 출력.
                        fprintf(fp2, "%15.2f",  SysInfoSer13.m_nData[PART_UPPER][mode][k][RList] );
                    } 
                    fprintf(fp2, "\n" );
                }
                fclose(fp2);

                //-----------------------------------------------------
                //son fName3  : C\ACE400\SETUP\CAL\CALR%02dA.INI 생성
                //-----------------------------------------------------
                fp3 = fopen(fName3,"wt");       // C:\\ACE400\\SETUP\\CAL\\CALR%02dA.INI
                if(fp3 == NULL)
                { 
                    Str.Format("Error No2003 !  \n  %s Saving Error!   fopen fail", fName3);
                    AfxMessageBox(Str, MB_OK); //error messege              
                    return 0;
                }

                // Use=1 인 RealR(Ref R)과  ADC R(Ref Avg)만 "CALR%02dA.INI"에 출력한다.
                fprintf(fp3, "%d %d\n", mode,nRcount);
                for(RList=1;RList<=MAX_CAL_RLIST;RList++){              //son220408 128: MAX_CAL_RLIST
                    if( SysInfoSer13.m_nData[PART_UPPER][mode][CAL_USE][RList]==1){ //son220407 1: CAL_USE
                        fprintf(fp3, "%.2f  %.2f\n",SysInfoSer13.m_nData[PART_UPPER][mode][CAL_REF_R][RList],          //son 2: CAL_REF_R
                                                    SysInfoSer13.m_nData[PART_UPPER][mode][CAL_REF_RAWR][RList]);      //son 3: CAL_REF_RAWR
                    }
                }
                fclose(fp3);
            }

            if (part == PART_LOWER)
            {
                //-----------------------------------------------------------------
                //son fNameCal  : C\ACE400\SETUP\CAL\CALR%02dD.INI(하부 CalData) 생성
                //-----------------------------------------------------------------
                //son    SysInfoSer13.m_nData[PART_LOWER][mode][1:CAL_USE    ][RList] : Use 1,0 
                //       SysInfoSer13.m_nData[PART_LOWER][mode][2:CAL_REF_R  ][RList] : Ref.R
                //       SysInfoSer13.m_nData[PART_LOWER][mode][3:CAL_REF_RAWR][RList] : Ref.Avg 
                //       SysInfoSer13.m_nData[PART_LOWER][mode][4:CAL_VC     ][RList] : dVC1[0][RList] 
                //       SysInfoSer13.m_nData[PART_LOWER][mode][5:CAL_I      ][RList] : dI1[0][RList] 
                fp4 = fopen(fName4,"wt");  // C:\\ACE400\\SETUP\\CAL\\CALR%02dD.INI    //son220412_2 begin
                if(fp4 == NULL)
                { 
                    Str.Format("Error No2003 !  \n  %s Saving Error!   fopen fail", fName4);
                    AfxMessageBox(Str, MB_OK); //error messege              
                    return 0;
                }   //son220412_2 end
                for(RList=1;RList<=MAX_CAL_RLIST;RList++)   //son220412_2 begin  "CALR%02dD.INI" 출력 신규 추가
                {                                           //                   나노시스 요청사항 
                    for (k=1; k<= CAL_I; k++)     
                        fprintf(fp4, "%15.2f    ",   SysInfoSer13.m_nData[PART_LOWER][mode][k][RList] );
                    fprintf(fp4, "\n" );
                } 
                fclose(fp4); //son220412_2 end
            }

			// hdpark230105 begin spark
            if (part == PART_SPARK)
            {
                fp5 = fopen(fName5,"wt");
                if(fp5 == NULL)
                { 
                    Str.Format("Error No2003 !  \n  %s Saving Error!   fopen fail", fName5);
                    AfxMessageBox(Str, MB_OK); //error messege              
                    return 0;
                }  
                for(RList=1;RList<=MAX_CAL_RLIST;RList++) 
                {                                    
                    for (k=1; k<= CAL_I; k++)     
                        fprintf(fp5, "%15.2f    ",   SysInfoSer13.m_nData[PART_SPARK][mode][k][RList] );
                    fprintf(fp5, "\n" );
                } 
                fclose(fp5); 
            }
            break;
			// hdpark230105 end

        case _LOAD: //2: LOAD  //son211012 

            char buf[GET_BUF_SIZE];    //son220407 4096: GET_BUF_SIZE

			if(part != PART_SPARK)	//hdpark230105 spark
			{
				//son LOAD 할 때에는 part 정보 상관없이 Upper와 Lower를 모두 Load해서 Mixed까지 채워야 한다.
				//if (part == PART_UPPER || part == PART_MIXED)
				{
					//--------------------
					// Load CALR%02dB.INI 
					//--------------------
					fp=fopen(fNameCal,"rt");
					if(fp == NULL)
					{ 
						str.Format(" Error No 2003 !\n %s file Loading Error! fopen() fail.", fNameCal); //error messege           
						AfxMessageBox(str);
						return 0;
					}     
					::ZeroMemory(buf, sizeof(buf));
					for(RList=1;RList<=MAX_CAL_RLIST;RList++) //son220408 MAX_CAL_RLIST:128
					{ 
						for (k=1; k<= CAL_I; k++)    //son220407 5:CAL_I
						{
							if (k== CAL_I){          //son220407 5:CAL_I
								fscanf(fp, "%s\n", buf);       //son190620  CString str -> char buf로 변경. fscanf 버퍼문제
							}
							else{
								fscanf(fp, "%s", buf);         //son190620  
							} 
							SysInfoSer13.m_nData[PART_UPPER][mode][k][RList]=atof(buf);    //son190620  
						}
					} 
					fclose(fp); 

					//--------------------
					// Load CALR%02dC.INI 
					//--------------------
					//son TEST1 ~ TEST10까지의 값을 "CALR%02dC.INI"에서 Read한다.
					fp2=fopen(fName2,"rt");
					if(fp2 == NULL)
					{ 
						str.Format(" Error No 2003 !\n %s  file Loading Error!  fopen() fail.", fName2); //error messege            
						AfxMessageBox(str);
						return 0;
					}

					//son 4줄 헤더 skip
					for (k=1; k <=4; k++) { //son220407 begin
						::ZeroMemory(&buf, sizeof(buf));    
						fgets(buf, sizeof(buf),fp2);
					}   //son220407 end

					for(RList=1;RList<=MAX_CAL_RLIST;RList++)        //son220408 MAX_CAL_RLIST:128
					{ 
						//son "CALR%02dB.INI"에서 이미 read한 5개 항목은  skip
						for (k=1; k<= CAL_I; k++)
							fscanf(fp2, "%s", buf);         //son190620  

						for (k= CAL_TEST1; k<= (CAL_TEST10 +1); k++)     //son220407 Read 6:CAL_TEST1 ~ 15:CAL_TEST10
						{
							if (k== CAL_TEST10 +1) //son220407 15: CAL_TEST10
								//fscanf(fp2, "%d\n", &SysInfoSer13.m_nData[PART_UPPER][mode][k][RList] );
								fscanf(fp2, "%s\n", buf);       
							else
								//fscanf(fp2, "%d", &SysInfoSer13.m_nData[PART_UPPER][mode][k][RList]);
								fscanf(fp2, "%s", buf);         //son190620  
							SysInfoSer13.m_nData[PART_UPPER][mode][k][RList]=atof(buf);    //son220407
						}
					}  
					fclose(fp2);   
				}

				//if (part == PART_LOWER || part == PART_MIXED)
				{
					//--------------------
					// Load CALR%02dD.INI  (하부 전용)
					//--------------------
					fp4=fopen(fName4,"rt"); // C:\\ACE400\\SETUP\\CAL\\CALR%02dD.INI    //son220412_2 begin
					if(fp4 == NULL)
					{ 
						//son "CALR%02dD.INI" 파일이 없다면 "CALR%02dB.INI" 파일을 카피해서 만든다.
						if(!FileExists(fName4) && FileExists(fNameCal))
						{
							MyTrace(PRT_BASIC, " %s file is not exist. Make %s with %s\n", fName4, fName4, fNameCal); //error messege           
							CopyFile(fNameCal, fName4, FALSE);     

							fp4=fopen(fName4,"rt"); // C:\\ACE400\\SETUP\\CAL\\CALR%02dD.INI    
							if(fp4 == NULL)
							{ 
								str.Format(" Error No 2003 !\n %s file Loading Error! fopen() fail.", fName4); //error messege           
								AfxMessageBox(str);
								return 0;
							}
						}
						else
						{
							str.Format(" Error No 2004 !\n %s file Making Error!\n Check if %s file exist. \n", fName4, fNameCal); //error messege           
							MyTrace(PRT_BASIC, str);
							AfxMessageBox(str);
							return 0;
						}
					}     
					::ZeroMemory(buf, sizeof(buf));
					for(RList=1;RList<=MAX_CAL_RLIST;RList++) //son220408 MAX_CAL_RLIST:128
					{ 
						for (k=1; k<= CAL_I; k++)    //son220407 5:CAL_I
						{
							if (k== CAL_I){          //son220407 5:CAL_I
								fscanf(fp4, "%s\n", buf);       //son190620  CString str -> char buf로 변경. fscanf 버퍼문제
							}
							else{
								fscanf(fp4, "%s", buf);         //son190620  
							} 
							SysInfoSer13.m_nData[PART_LOWER][mode][k][RList]=atof(buf);    //son190620  
						}
					} 
					fclose(fp4);    //son220412_2 end
				}

				////////////////////////////////////////////////////////////////////////////////////
				//calibration 

				//son220412_2 begin
				for (part2=PART_UPPER; part2 <= PART_LOWER; part2++) 
				{
					nRcount=1;    //son nRcount=1 부터 시작
					for(RList=1;RList<=MAX_CAL_RLIST;RList++)   //son220408 128:MAX_CAL_RLIST
					{
						//son Cal Use=1 일때에만 SysInfoSer13.m_nData[part2][mode][1~3][RList]를 
						//      gdCal1[part2][mode][1~3][nRCount]에 저장한다.
						if(SysInfoSer13.m_nData[part2][mode][CAL_USE][RList]==1){    
							gdCal1[part2][mode][CAL_USE][nRcount]     = SysInfoSer13.m_nData[part2][mode][CAL_USE][RList];   //son Cal Grid 'Use =1' 값  
							gdCal1[part2][mode][CAL_REF_R][nRcount]   = SysInfoSer13.m_nData[part2][mode][CAL_REF_R][RList]; //son Cal Grid 'Ref.R'  값  (ohm realR) 
							gdCal1[part2][mode][CAL_REF_RAWR][nRcount]= SysInfoSer13.m_nData[part2][mode][CAL_REF_RAWR][RList];  //son Cal Grid 'Ref AVG'값  (ADC rawR)
							nRcount++;
						}

						// hdpark221005  begin, self test 판별 값으로 인해 judge mode(6, 206, 327)의 고정 Index 사용을 위해 
						//               별도 저장(for mode MODE327_LEAK_250V, MODE182_SELF_200V)
						//               Cal Use 가 1인지 여부와 상관없이 write함.
						if (mode == MODE6_OPEN_10V || mode == MODE206_HR_SHORT_10V || mode == MODE327_LEAK_250V)
						{
							int nModeIndex = 0;
							if (mode ==   MODE6_OPEN_10V)           nModeIndex = 0;
							else if (mode == MODE206_HR_SHORT_10V)  nModeIndex = 1;
							else if (mode == MODE327_LEAK_250V)     nModeIndex = 2;
							gdCal2[part2][nModeIndex][CAL_USE][RList]      = SysInfoSer13.m_nData[part2][mode][CAL_USE][RList];   //son Cal Grid 'Use =1' 값  
							gdCal2[part2][nModeIndex][CAL_REF_R][RList]    = SysInfoSer13.m_nData[part2][mode][CAL_REF_R][RList]; //son Cal Grid 'Ref.R'  값  (ohm realR) 
							gdCal2[part2][nModeIndex][CAL_REF_RAWR][RList] = SysInfoSer13.m_nData[part2][mode][CAL_REF_RAWR][RList];  //son Cal Grid 'Ref AVG'값  (ADC rawR)
						}
						// hdpark221005 end

					}
					if( mode==MODE171_4W || mode==MODE176_4W_CV ){//sylee200515-patch-4W정도개선    //son220103
						 gdCal1[part2][mode][CAL_USE][0]=1;    
					} 
					gdCal1[part2][mode][0][0] = nRcount-1;     //son Use=1인 Cal  list 개수
				} 



				//son 상부, 하부 혼합 측정일 때를 위한 Cal 값 설정. 
				nRcount=1;    //son nRcount=1 부터 시작         
				for(RList=1;RList<=MAX_CAL_RLIST;RList++)   
				{
					if(SysInfoSer13.m_nData[PART_UPPER][mode][CAL_USE][RList]==1        
						|| SysInfoSer13.m_nData[PART_LOWER][mode][CAL_USE][RList]==1    )    
					{
						gdCal1[PART_MIXED][mode][CAL_USE][nRcount]     = 1;          
						gdCal1[PART_MIXED][mode][CAL_REF_R][nRcount]   = SysInfoSer13.m_nData[PART_UPPER][mode][CAL_REF_R][RList]; 
						gdCal1[PART_MIXED][mode][CAL_REF_RAWR][nRcount]= (SysInfoSer13.m_nData[PART_UPPER][mode][CAL_REF_RAWR][RList] 
																			+ SysInfoSer13.m_nData[PART_LOWER][mode][CAL_REF_RAWR][RList]) / 2.0;
						nRcount++;
					}
				}
				if( mode==MODE171_4W || mode==MODE176_4W_CV ){//sylee200515-patch-4W정도개선    //son220103
					 gdCal1[PART_MIXED][mode][CAL_USE][0]=1;    
				} 
				gdCal1[PART_MIXED][mode][0][0] = nRcount-1;     //son Use=1인 Cal  list 개수
				//son220412_2 end
			}
            //hdpark230105 begin
			else	// spark
            {
                fp5 = fopen(fName5, "rt"); // C:\\ACE500\\SETUP\\CAL\\CALR%02dK.INI 
                if (fp5 == NULL)
                {
					double g_dRList[39] = {	0,	
							1        ,	2        ,	5        ,	7.5      ,	10       ,	20       ,	50       ,	75       ,	100      ,	200      ,	
							300      ,	500      ,	750      ,	1000     ,	2000     ,	5000     ,	10000    ,	20000    ,	50000    ,	75000    ,
							100000   ,	200000   ,	500000   ,	750000   ,	1000000  ,	2000000  ,	5000000  ,	7500000  ,	10000000 ,	20000000 ,
							30000000 ,	50000000 ,	80000000 ,	100000000,	110000000,	150000000,	200000000,	300000000	};

					for (RList = 1; RList <= MAX_CAL_RLIST; RList++) 
					{
						for (k = 1; k <= CAL_I; k++) 
						{
							if(k == CAL_REF_R && RList < 39)	SysInfoSer13.m_nData[PART_SPARK][mode][k][RList] = g_dRList[RList];
							else								SysInfoSer13.m_nData[PART_SPARK][mode][k][RList] = 0;	
						}
					}
                }
				else
				{
					::ZeroMemory(buf, sizeof(buf));
					for (RList = 1; RList <= MAX_CAL_RLIST; RList++) 
					{
						for (k = 1; k <= CAL_I; k++) 
						{
							if (k == CAL_I) { 
								fscanf(fp5, "%s\n", buf);
							}
							else {
								fscanf(fp5, "%s", buf);
							}
							SysInfoSer13.m_nData[PART_SPARK][mode][k][RList] = atof(buf);
						}
					}
					fclose(fp5);
				}

				for(RList=1;RList<=MAX_CAL_RLIST;RList++) 
				{
					gdCal1[PART_SPARK][mode][CAL_USE][RList]     = SysInfoSer13.m_nData[PART_SPARK][mode][CAL_USE][RList];   //son Cal Grid 'Use =1' 값  
					gdCal1[PART_SPARK][mode][CAL_REF_R][RList]   = SysInfoSer13.m_nData[PART_SPARK][mode][CAL_REF_R][RList]; //son Cal Grid 'Ref.R'  값  (ohm realR) 
					gdCal1[PART_SPARK][mode][CAL_REF_RAWR][RList]= SysInfoSer13.m_nData[PART_SPARK][mode][CAL_REF_RAWR][RList];  //son Cal Grid 'Ref AVG'값  (ADC rawR)
				}

            }
            // hdpark230105 end

            break;      

        default :
            break;
    }

    return 1;
}
 
 



int CFileSysInfo::LoadSaveHVCal1(int type)
{
    FILE    *fp; 
    char    fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    int     Total;
    int     i,j; 
    CString str;

    ::ZeroMemory(&fName, sizeof(fName));
    str.Format("%s\\SETUP\\HVCal1.ini", SysInfoView01.m_pStrFilePath1);     //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName, str); //son20111);    //son2105148


    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
            fp = fopen(fName,"wt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2006 !\n %s  file Saving Error !     file close!", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            Total=(int)IADClog1[0][0];

            if( Total<=0){
                break;
            }

            fprintf(fp, "%10d\n ",  IADClog1[0][0] ); 

            for( i=1; i<=Total; i++){
                for(   j=1; j<=10; j++){    
                    fprintf(fp, "%10d", IADClog1[j][i] );  
                }
                fprintf(fp, "\n  "  ); 
            }       
            break;



        case _LOAD: //2: LOAD  //son211012 

            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2002 !\n %s file Loading Error", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }   

            fscanf(fp, "%10d\n",    IADClog1[0][0]);    

            Total=(int)IADClog1[0][0];
            for(   i=1; i<=Total; i++){
                for(   j=1; j<=10; j++){    
                    fscanf(fp, "%10d",  IADClog1[j][i] );   
                }
                fscanf(fp, "\n  "  );  
            }

            break;

        default :
            break;
    }
    fclose(fp); 
    return 1;
}


int  CFileSysInfo::LoadSave_Model_Load1(int type)
{  
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString  str; 
    int tmp;

    ::ZeroMemory(&fName, sizeof(fName));
    ::ZeroMemory(&dEsModel, sizeof(dEsModel));

    str.Format("%s\\Model\\Model.ini",(SysInfoView01).m_pStrFilePathBDL);//sylee140210    
    strcat( fName , str );  //sylee121202       

    fp = fopen(fName,"rt");
    if (fp == NULL)
    { 
        str.Format("Error No 2302.  %s file Loading Error. directory? file?", fName);
        AfxMessageBox(str, MB_OK); //error messege
        return 0;
    }

    char str2[2048]; 
    ::ZeroMemory(str2, sizeof(str2));

    for( int i=1; i<500; i++){
        for( int j=1; j<=7; j++){ 
            tmp=fscanf(fp, "%s", str2);             //son190620  str ->str2로 변경. fscanf 버퍼문제
            if(tmp==1){
                dEsModel[i][j]=(double)atof(str2);  //son190620  str ->str2로 변경. fscanf 버퍼문제
            }
        }
        fscanf(fp, "\n");
    }

    fclose(fp); 


    return 1;


}




//SYLEE130618
//son "C:\\ACE400\\SETUP\\SysSet21_ACE400.ini" (전압 설정 Recipe)를 struct CSysSet21에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadSaveSet21(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 
    int i,j; 

    ::ZeroMemory(&fName, sizeof(fName));
    //SYLEE150907  //ACE400     strcat( fName , "C:\\ACE400\\SETUP\\SysSet21.ini" ); 
    str.Format("%s\\SETUP\\SysSet21_ACE400.ini", SysInfoView01.m_pStrFilePath1);    //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
#if defined(__ACE1000__)   //son240807
    str.Format("%s\\SETUP\\SysSet21_ACE1000.ini", SysInfoView01.m_pStrFilePath1);   //son240807  for CCTC Plan B (ACE500코드로 ACE1000 계측기 지원) 
#endif
    strcat(fName, str); //son201118 //son210514
    g_CheckFlag1=0;

    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           

            fp = fopen(fName,"wt");
            if(fp == NULL){ 
                str.Format(" Error No 2001 !\n %s file Saving Error! fopen() fail !", fName); //error messege
                AfxMessageBox(str);
                return 0;
            }   

            for(  i=0; i<361; i++)
            {
                for(  j=0; j<16; j++){
                    //son 1. SysSet21.m_nDrec[][]의 값을  \\SETUP\\SysSet21_ACE400.ini 파일에 write
                    fprintf(fp, "%10d",SysSet21.m_nDrec[i][j]);   
                }
                fprintf(fp, "\n");  
            }           
            break;


        case _LOAD: //2: LOAD  //son211012 

            fp=fopen(fName,"rt");
            if(fp == NULL)
            { 
                str.Format(" Error No 2112 !\n\n  %s file Loading Error!\n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
                g_CheckFlag1=2;
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            for(  i=0; i<361; i++)
            {
                for(   j=0; j<16; j++)
                {               
                    //son 1. \\SETUP\\SysSet21_ACE400.ini 파일에서 SysSet21.m_nDrec로 값을 read
                    fscanf(fp, "%10d", &SysSet21.m_nDrec[i][j]);  


                    //son 2. SysSet21.m_nDrec[i][j]의 값을 dRec[i][j]로 카피
                    if(j==5){	//son nISet 일 때
                        //SYLEE150905//ACE400 SysSet21.m_nDrec[i][j]=(int)(SysSet21.m_nDrec[i][j]/10.);
                        dRec[i][j]=(SysSet21.m_nDrec[i][j]/10.);//SYLEE150905//ACE400 
                    }
                    else{   //SYLEE150905//ACE400          
                        dRec[i][j]=SysSet21.m_nDrec[i][j];//sylee130701
                    }//SYLEE150905//ACE400 
                }
                fscanf(fp, "\n");  
            }   

            break;

        default :
            break;
    }

    fclose(fp); 
    return 1;
}



//SYLEE130618
//son "C:\\ACE400\\SETUP\\SysSet22.ini"를 struct CSysSet22에 Load(type=2) 또는 Save(type=1) 
int CFileSysInfo::LoadSaveSet22(int type)
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 
    int i,j; 

    ::ZeroMemory(&fName, sizeof(fName));
//  strcat( fName , "C:\\ACE400\\SETUP\\SysSet22.ini" );  //sylee170412//HOLD170414   0<->1      1,2,3    6,7,8
    str.Format("%s\\SETUP\\SysSet22.ini", SysInfoView01.m_pStrFilePath1);   //son210514 "C:\\ACE500"  => SysInfoView01.m_pStrFilePath1
    strcat(fName, str);  //son20111)   //son2105148
    g_CheckFlag1=0;

    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n %s file Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }   
        for(  i=0; i<80; i++){
            for(  j=0; j<11; j++){
               fprintf(fp, "%10d",SysSet22.m_dRecSetup[i][j]);   
            }
            fprintf(fp, "\n");  
        }           
        break;

    case _LOAD: //2: LOAD  //son211012 

        fp=fopen(fName,"rt");
        if(fp == NULL)
        { 
            str.Format(" Error No 2112 !\n\n %s file Loading Error ! \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
            g_CheckFlag1=2;
            return 0;
        }
        for(  i=0; i<80; i++){
            for(   j=0; j<11; j++){
               fscanf(fp, "%10d", &SysSet22.m_dRecSetup[i][j]);  
               dRecSetup[i][j]=SysSet22.m_dRecSetup[i][j];//sylee130703
            }
            fscanf(fp, "\n");  
        }    

        break;

    default :
        break;
    }

    fclose(fp); 
    return 1;
}



extern int dRec4w[15][11];//SYLEE171012-1
extern int dRec4w1[15][11];//SYLEE171012-1
//son 4W용 dRec4w[15][11]을 read한다.
int CFileSysInfo::LoadSaveSet21_4W_Range(int type)//SYLEE171012-1
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 
    int i; 

 
    ::ZeroMemory(&fName, sizeof(fName)); 
 
    switch(type)
    {
    case _SAVE: //1: SAVE   //son211012           
        
//      strcat( fName , "C:\\ACE400\\SETUP\\SysSet21_4W_Save.ini" );  
        str.Format("%s\\SETUP\\SysSet21_4W_Save.ini", SysInfoView01.m_pStrFilePath1); //son210514
        strcat(fName , str );  //son20111) //son2105148
        fp = fopen(fName,"wt");
        if(fp == NULL){ 
            str.Format(" Error No 2001 !\n %s file Data Saving Error !     file close!", fName); //error messege
            AfxMessageBox(str, MB_OK);
            return 0;
        }   
        for(  i=1; i<=7; i++){   
               fprintf(fp, "%10d\n",dRec4w1[i][3] );                
        }       

        break;


    case _LOAD: //2: LOAD  //son211012 

//      strcat( fName , "C:\\ACE400\\SETUP\\SysSet21_4W.ini" ); 
        str.Format("%s\\SETUP\\SysSet21_4W.ini", SysInfoView01.m_pStrFilePath1);    //son210514
        strcat(fName, str); //son201118 //son210514
        fp=fopen(fName,"rt");
        if(fp == NULL){ 
            str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
            AfxMessageBox(str, MB_OK);
             
            return 0;
        }
        //son Max R 값 dRec4w1[i][3] 만 파일에서 읽어온다.
        // ex) C:\ACE400\SETUP\SysSet21_4W.ini
        //         35
        //         600
        //         6000
        //         500000
        //         500000
        //         0
        //         0
        for(  i=1; i<=7; i++)
        {                       
            //son 4W Max R 값을 read
            fscanf(fp, "%10d\n", &dRec4w1[i][3]);

            //son i=4~ 7은  다음의 값으로 강제 셋팅
            if(i==4){  dRec4w1[i][3]=500000;    }
            if(i==5){  dRec4w1[i][3]=5000000;   }
            if(i==6){  dRec4w1[i][3]=10000000;  }
            if(i==7){  dRec4w1[i][3]=20000000;  }

            //son 4W Min R 값은 MaxR 의 80%로 설정
            if(i<7){
                dRec4w1[i+1][2]=(int)(dRec4w1[i][3]*0.8) ; 
            }
            dRec4w[i][2]=dRec4w1[i][2];
            dRec4w[i][3]=dRec4w1[i][3];
        }
    //  fscanf(fp, "\n");  

        break;

    default :
        break;
    }

    fclose(fp); 
    return 1;
}



int CFileSysInfo::LoadSaveSet21_4W_Cal1Pin(int type)//sylee171020-1
{
    FILE *fp; 
    char  fName[FILE_PATH_SIZE];    //son210524 200-> FILE_PATH_SIZE
    CString str; 
  


    return 1;
 


   ::ZeroMemory(&fName, sizeof(fName)); 

    switch(type)
    {
        case _SAVE: //1: SAVE   //son211012           
//          strcat( fName , "C:\\ACE400\\SETUP\\SysSet21_4W_CalPin_Save.ini" );  
            str.Format("%s\\SETUP\\SysSet21_4W_CalPin_Save.ini", SysInfoView01.m_pStrFilePath1); //son210514
            strcat( fName , str );  //son201118    //son210514
            fp = fopen(fName,"wt");
            if(fp == NULL){ 
                str.Format(" Error No 2001 !\n %s file Saving Error !   file close!", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }   

            fprintf(fp, "%10d  %10d  %10d  %10d\n",nPinAutoCal1[1],nPinAutoCal1[2],nPinAutoCal1[3],nPinAutoCal1[4] );       
            break;


        case _LOAD: //2: LOAD  //son211012 
            ::ZeroMemory(&nPinAutoCal1, sizeof(nPinAutoCal1)); 

//          strcat( fName , "C:\\ACE400\\SETUP\\SysSet21_4W_CalPin.ini" ); 
            str.Format("%s\\SETUP\\SysSet21_4W_CalPin.ini", SysInfoView01.m_pStrFilePath1); //son210514
            strcat(fName, str); //son201118 //son210514
            fp = fopen(fName,"rt");
            if(fp == NULL){ 
                str.Format(" Error No 2112 !\n\n %s  file Loading Error !  \n\n\n  file check!   \n\n\n\n    *******<  Don't Start!!!  >*********** \n\n\n\n", fName); //error messege
                AfxMessageBox(str, MB_OK);
                return 0;
            }

            fscanf(fp, "%10d %10d %10d %10d\n", &nPinAutoCal1[1],&nPinAutoCal1[2],&nPinAutoCal1[3],&nPinAutoCal1[4]);

            //  fscanf(fp, "\n");  

            break;

        default :
            break;
    }

    fclose(fp); 
    return 1;
}


//son220315 하부핀 R 측정기능 관련 신규생성
//son220331 CModeSer15(R meas)에서 CFileSysInfo로 LoadSave_UseLowerPin()함수 이동.
//          CModeSer13(Calibration)에서도 같이 쓰기 위해 전역함수로 변경함.

//son 하부핀 R 측정 여부를 퍄일에 저장하고 read한다.
bool CFileSysInfo::LoadSave_UseLowerPin(int type, bool &r_bUseLowerPin) 
{
    FILE    *fp; 
    char    fName[FILE_PATH_SIZE]; //son210625 200-> FILE_PATH_SIZE
    CString str;


    ::ZeroMemory(&fName, sizeof(fName));
    //son220413 str.Format("%s\\Measure_R.txt", g_sFile.DataDir);
    str.Format("%s\\Data\\Measure_R.txt", SysInfoView01.m_pStrFilePath1); //son220413 C:\ACE500\Data\Measure_R.txt 
    strcat( fName, str);

    switch(type)
    {
        case _SAVE:
            fp = fopen(fName,"wt");         
            if(fp == NULL){ 
                str.Format(" Error No 2001 !\n%s  file Data Saving Error !     file close!", fName); //error messege
                AfxMessageBox(str, MB_OK); //error messege
                return false;
            }   

            fprintf(fp, "%10d\n",   (int)r_bUseLowerPin); //son220315   //son220331

            fclose(fp);
            break;


        case _LOAD: 
            fp=fopen(fName,"rt");
            if(fp == NULL){ 
                //son 처음 로딩할 때는 파일이 없는 것이 정상이므로 굳이 로딩시 fail은 출력하지 않고 리턴만 한다.
                //str.Format(" Error No 2112 !\n\n %s file Loading Error  \n\n\n  file check!", fName); //error messege
                //AfxMessageBox(str, MB_OK);
                return 0;
            }


            int     nUseLowerPin;
            fscanf(fp, "%d\n",   &nUseLowerPin);        //son200827-4w_Sigma
            r_bUseLowerPin  = (BOOL)((nUseLowerPin) ? true : false) ;   //son220331

            fclose(fp);
            break;


        default:
            break;
    }


    return true;
    
}


#if 0
//son220322 미사용코드이므로 삭제

CString  FileExtNo(UINT nMode)//sylee180810-1-DUT16
{
    CString str;

    if( nMode==1){ 
        str = "";
    }
    else if( nMode<1){
        str = "";
        AfxMessageBox("BDL_FileExtension error!   nMultyMode1<1");
    }
    else if( nMode> MAX_DUT){   //son220322 16 -> MAX_DUT(32)
        str = "";

        Cstring str2;
        str2.Format("BDL_FileExtension error!   nMultyMode1(%d) > %d", nMode, MAX_DUT); //son220322 16 -> MAX_DUT(32)
        AfxMessageBox(str2);
    }
    else{
        str.Format("%d",nMode); 
    }
    return str;  
}



CString  FileExtNoChar(UINT nMode)//sylee180810-1-DUT16
{
    CString str;

    if( nMode==1){ 
        str = "";
    }
    else if( nMode<1){
        str = "";
        AfxMessageBox("BDL_FileExtension error!   nMultyMode1<1");
    }
    else if( nMode> MAX_DUT){   //son220322 16 -> MAX_DUT(32)
        str = "";

        Cstring str2;
        str2.Format("BDL_FileExtension error!   nMultyMode1(%d) > %d", nMode, MAX_DUT); //son220322 16 -> MAX_DUT(32)
        AfxMessageBox(str2);
    }
    else
    {
        if(nMode<10){
            str.Format("%d",nMode); 
        }
        else if(nMode==10){
            str.Format("A"); 
        }
        else if(nMode==11){
            str.Format("B"); 
        }
        else if(nMode==12){
            str.Format("C"); 
        }
        else if(nMode==13){
            str.Format("D"); 
        }
        else if(nMode==14){
            str.Format("E");
        }
        else if(nMode==15){
            str.Format("F"); 
        }
        else if(nMode==16){
            str.Format("G"); 
        }
    }
    return str;  
}
#endif


