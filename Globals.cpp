//////////////////////////////////////////////////////////////////////
// Globals.cpp: implementation of the CGlobals class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Globals.h"
#include <mmsystem.h>
#include <afxmt.h>
#include <io.h>
//#include <afx.h>
//#include "psapi.h"
#include "FileSysInfo.h"
#include <Time.h>
#include <stdio.h>
#include "ModeSub71.h"      //son220405

#pragma comment(lib, "mpr.lib")   //sylee231010-1  //son240705
#include <winnetwk.h>   //sylee231010-1   //son240705
#include <WinSock2.h>   //sylee231010-1   //son240705

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobals::CGlobals()
{

}

CGlobals::~CGlobals()
{

}



////////////////////
// 전역 변수 설정
////////////////////

int nView1;		//son220215 moved from gobal.h


sFILE_DATA		g_sFile;
WORD			g_wPrintState = 0x0007;		// 0x01, PRT_BASIC    on 
                                            // 0x07, PRT_BASIC, PRT_LEVEL1, PRT_LEVEL2 on


//son Multi DUT 4W Net Data   (Grid Display용, R값은 별도로 d4W_Log1[][], d4W_Lo2[][]에 저장한다.)
//son190704-4WDUT16: gd4W1[][][]를 g_s4WNet 구조체로 변경 	
C4WNet	g_s4WNet[MAX_DUT];			
  
//son190801 gd4W1[][][] 삭제. 	g_s4WNet[MAX_DUT]로  대체 완료
//float	gd4W1[MAX_DUT][MAX_4W_NET +1][15];//SYLEE20120116  //4W NETLIST  //sylee170728-1
//son   1. LoadSaveSub17()에서 BDL위치\4W.ini  값을 gd4W1[MAX_DUT][MAX_4W_NET +1][15]에 write/read 한다.
//son   2. LoadSaveSub17()에서 BDL위치\4W.ini  값을 gd4W1[MAX_DUT][MAX_4W_NET +1][15]에 write/read 한다.
//son	gd4W1[dut][0][0]  :  4W Net 갯수 
//son   gd4W1[dut][1~5][0~14]:  의미없음.
//son   gd4W1[dut][6][1~12]:  4W.ini의 첫번째 라인 데이터 12 항목을  저장.
//son   gd4W1[dut][5+i][0~11]:  gd4W1[0][0] 갯수만큼 i를 1부터 증가시켜 가면서 4W.ini의 데이터를 저장한다.
//								i가 1부터 이므로   gd4W1[dut][6]부터 시작함.
//
//                 0     1      2      3      4       5        6            7       8         9           10       11          ....  14
//				   No	Pin1   Pin2   Pin3   Pin4   Set.Min   Set.Avg   Set.Max    CalMode    Test.Min   Test.Avg  Test.Max         nCurrMode
//	ex)			   1     1      2      385    386    1.060   1.120        1.150    171       1.060       1.120     1.150
//	 ==> MAP->4W의 Grid 화면과 일치한다.
 





#ifndef __OPVECTOR__

//son  nBDLData31[][][], nBDLData2Op[][][]을  ChilcView1.cpp-> Globals.cpp로 이동.  
//     extern선언과 추가된 Class의 definition은 Globals.h에 정의함.

//son200420-4WNET10000		
//short nBDLData2Op[DefDutMax][DEF_MAX_PIECE1][dSetPinTot2][3];//open data    //sylee150604-kushan   //sylee150917-2  16400  33000
short nBDLData2Op[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2][3];//open data    //son220824_2 DefDutMax(33) -> MAX_DUT1(33)  //son220823_6
		//son190705
		//piece 1, piecePinIdx 1부터 할당함
		//son	nBDLData2Op[nDut][piece][0][0]++;								: piece별 open test 대상 pin 수량
		//son	nBDLData2Op[nDut][piece][piecePinIdx][1]=nBDLData2[i][1];		: 대표핀
		//son	nBDLData2Op[nDut][piece][piecePinIdx][2]=nBDLData2[i][2];		: PinNo

//son200420-4WNET10000		
//son nBDLData3을 piece 별로 나누어 재배치한 Short data
//short nBDLData31[DefDutMax][DEF_MAX_PIECE1][dSetPinTot2];//short data    //sylee150604-kushan
short nBDLData31[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2];//short data    //son220824_2 DefDutMax(33) -> MAX_DUT1(33) //son220823_6
		//son nBDLData31[nDut][piece][0]                : piece별 Short pin 총수
		//son nBDLData31[nDut][piece][pieceShortPinIdx] : piece별 Short pinNo
#endif

#ifdef __OPVECTOR__
using namespace std;

//son200420-4WNET10000		
int     g_nOpenTestCount = 0;       //son Total OpenTest 갯수
vector <COpenTestItem>	*g_pvBDLData2Op[MAX_DUT+1][MAX_PIECE+1];  // 동적할당 vector
            //son 1. nBDLData2Op[nDut][piece][0][0]    => (g_pvBDLData2Op[nDut][nPiece]->size() -1)로 대체
            //    2. nBDLData2Op[nDut][piece][nPin][1] => (*g_pvBDLData2Op[nDut][nPiece])[nPin].wFirstPin으로 대체
            //    3. nBDLData2Op[nDut][piece][nPin][2] => (*g_pvBDLData2Op[nDut][nPiece])[nPin].wPin로 대체


//son200420-4WNET10000		
int      g_nShortTestCount = 0;     //son Total ShortTest 갯수 
vector <CShortTestItem>	*g_pvBDLData31[MAX_DUT+1][MAX_PIECE+1];        // 동적할당 vector
            //son 1. nBDLData31[nDut][piece][0]    => (g_pvBDLData31[nDut][nPiece]->size() -1) 로 대체
            //    2. nBDLData31[nDut][piece][nPin] => (*g_pvBDLData31[nDut][nPiece])[nPin].wFirstPin으로 대체

extern  CSysInfoPieceRange  SysInfoPieceRange;
extern  int g_nPieceMax1;
#endif



extern CModeSub71  Sub71;       //son220405



////////////////////
// 전역  함수 설정
////////////////////


int fileCopy(const char* src, const char* dst);
int fileCopy(const char* src, const char* dst)      //son210906 moved from ModeSer13.cpp
{
    CString str1,str2;
    str1.Format("d:\\copy %s  %s", src,dst);
    ::ShellExecute(NULL, _T("open"), "C:\\cmd.exe", str1, NULL, SW_SHOW);
    return 0;
}


//---------------------------------------------------------------------------------------------
//son200420-4WNET10000
//    nBDLData2Op[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2][3] 를 
//    vector <COpenTestItem> g_pvBDLData2Op[MAX_DUT][MAX_PIECE] 로 변경
//    [dSetPinTot2][3] array는 COpenTestItem 의 vector로 바뀐다.   (동적할당을 위해)
//    [3] array는 COpenTestItem 내부의 member 변수 wFirstPin, wPin으로 대체된다.
//    
//    사용할때에는 (*g_pvBDLData2Op[nDut][nPiece])[nPin].wFirstPin 과 같이 사용하면 된다.
//---------------------------------------------------------------------------------------------

//son OpenTest Data를 메모리에 추가 한다.   //son200420-4WNET10000
int InsertOpenTest(int nDut, int nPiece, COpenTestItem openTestItem)
{
#ifdef __OPVECTOR__
    CString strTemp;

	// g_nOpenTestCount가  MAX_OPEN_TEST 갯수를 넘어서면 OpenTest 추가를 중지
	if (g_nOpenTestCount >= (MAX_OPEN_TEST + 1))    // 1번 vector부터 사용하므로 사이즈1 추가
		return -1;

	// g_pvBDLData2Op[nDut][nPiece]가 없다면 새로 new해서 생성한다. 
	// 문제있다면 -1리턴.  이미 존재한다면 그냥 0리턴.  
	// -1로 리턴된 경우만 아니라면 g_pvBDLData2Op[nDut][nPiece] vector를 사용할 수 있다.
	int ret = InsertOpenTest_Vector(nDut, nPiece);
	if (ret < 0)
		return -1;

	// vector 1개의 size는 32768을 넘을 수 없다.
	if (g_pvBDLData2Op[nDut][nPiece]->size() > (MAX_PIN + 1))    
	{
		strTemp.Format("g_pvBDLData2Op[%d][%d]->size()=%d, capacity()=%d, g_nOpenTestCount=%d\n Can't Insert %d over OpenTestItem",
					nDut, nPiece, g_pvBDLData2Op[nDut][nPiece]->size(), 
					g_pvBDLData2Op[nDut][nPiece]->capacity(), g_nOpenTestCount, MAX_PIN);
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
		return -1;
	}

	// vector의 item 갯수가 2048개를 넘는다면 vector 공간을 더 확보한다.  
	// 2048이 되는 시점에 1회성으로 수행하면, vector resize 도중에 "메모리 부족" 메시지 나오는 현상을 막을 수 있다.
	if ( g_pvBDLData2Op[nDut][nPiece]->size() == 2048)		
	{		
        //__PrintMemSize(FUNC(InsertOpenTest), __LINE__);       //son200420-4WNET10000
		// Test Print
        //MyTrace(PRT_BASIC, "\n");    //son220524 begin
		//MyTrace(PRT_BASIC, "g_pvBDLData2Op[%d][%d]->size()=%d, capacity()=%d, max_size()=%d, g_nOpenTestCount=%d\n",
		//			nDut, nPiece, 
		//			g_pvBDLData2Op[nDut][nPiece]->size(),
		//			g_pvBDLData2Op[nDut][nPiece]->capacity(), 
		//			g_pvBDLData2Op[nDut][nPiece]->max_size(), g_nOpenTestCount);

		//MyTrace(PRT_BASIC, "g_pvBDLData2Op[%d][%d]->size() = 2048! reserve other space for %d files..  \n", 
		//		    nDut, nPiece, MAX_PIN);   //son220524 end
		
		g_pvBDLData2Op[nDut][nPiece]->reserve(MAX_PIN + 1);
        //__PrintMemSize(FUNC(InsertOpenTest), __LINE__);       //son200420-4WNET10000
	}



	// 해당 vector에 실제 openTestItem을 insert한다.
	//   :  time 갯수를 가질 vector에 이번 time의 openTestItem을를 push_back 한다. 
	//   openTestItem을 value형 인자로 넘기는 과정에서 copy해야 하므로 COpenTestItem 클래스의 
	//   copy constructor(복사생성자) 호출이 일어난다.  포인터를 넘기는 경우라면, 복사 생성자안에서
	//   포인터에 대해 새로 new를 해서 공간을 만들어 줘야 나중에 delete[]할때에 문제가 안 생김. (deep copy..)
	g_pvBDLData2Op[nDut][nPiece]->push_back(openTestItem);
	g_nOpenTestCount++;


	// g_nOpenTestCount가  MAX_OPEN_TEST 갯수를 넘어서면 OpenTest 추가를 중지
	if (g_nOpenTestCount == (MAX_OPEN_TEST +1))
	{
		strTemp.Format("g_nOpenTestCount=%d, Stop InsertOpenTest()!!\nCurrent nDut=%d, nPiece=%d\n", 
				                        g_nOpenTestCount , nDut, nPiece); 
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
	}
#endif
	return 0;
}

//son  g_pvBDLData2Op[nDut][nPiece]가 없다면 새로 new해서 생성한다.     //son200420-4WNET10000
//     이미 존재한다면 그냥 0리턴, 문제있다면 -1리턴
int InsertOpenTest_Vector(int nDut, int nPiece)
{
#ifdef __OPVECTOR__
	CString strTemp;
	if ((nDut <= 0 || nDut > MAX_DUT)                   //son nDUB, nPiece 모두 1부터 시작
			|| (nPiece <= 0 || nPiece > MAX_PIECE))
	{ 	
		strTemp.Format("InsertOpenTest_Vector(): nDut=%d, nPiece=%d  Range(0<=Dut<%d, 0<=Piece<%d) Over Error!\n",
				                                nDut, nPiece, MAX_DUT, MAX_PIECE);
		//ERR.Set(RANGE_OVER, strTemp); 
		//ErrMsg();  ERR.Reset(); 
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
		return -1; 
	}
	
	// 해당 Dut, Piece 위치에 data가 insert된 적이 없다면  vector를 새로 생성해서 assign한다.
	//  : Dut * Piece 갯수의 vector 포인터에 실제 vector를 assign
	if (g_pvBDLData2Op[nDut][nPiece] == NULL)
	{
		vector <COpenTestItem>*  pvOpenTest = new vector <COpenTestItem>;
		if (pvOpenTest == NULL)		// 메모리할당 실패!
		{
			strTemp.Format("InsertOpenTest_Vector() Fail!: nDut=%d, nPiece=%d\n", nDut, nPiece);
			//ERR.Set(MEM_ALLOC_FAIL, strTemp); 
			//ErrMsg(-1, TRUE);  ERR.Reset(); 
			//ErrMsg();  ERR.Reset(); 
            AfxMessageBox(strTemp, MB_ICONINFORMATION);
            MyTrace(PRT_BASIC, strTemp);
			return -1;
		}
#if 0
		// test print
		else
        {
            MyTrace(PRT_BASIC, "\n");
			MyTrace(PRT_BASIC, "InsertOpenTest_Vector(): nDut=%d, nPiece=%d, g_nOpenTestCount=%d\n", 
			                        nDut, nPiece, g_nOpenTestCount);
        }
#endif

		g_pvBDLData2Op[nDut][nPiece] = pvOpenTest;
	}
#endif

	return 0;
}

// 모든 OpenTest data를 삭제하고 할당된 memory를 delete한다.    //son200420-4WNET10000
void DeleteAllOpenTestItem()
{
#ifdef __OPVECTOR__
	int dut, piece;

    //MyTrace(PRT_BASIC, "\n");
    //__PrintMemSize(FUNC(DeleteAllOpenTestItem), __LINE__);       //son200420-4WNET10000
	
	for (dut =1; dut <= MAX_DUT; dut++)
    {
        for (piece =1; piece <= MAX_PIECE; piece++)
        {
            if(g_pvBDLData2Op[dut][piece] != NULL)
            {
                //int prevTupleSize = g_pvBDLData2Op[dut][piece]->size();	// piece마다 tupleSize가 다름!

                // vector의 모든 data를비우고 (모든 tuple을 삭제하고)
                if (g_pvBDLData2Op[dut][piece]->empty() == false)
                    g_pvBDLData2Op[dut][piece]->clear();		


                delete g_pvBDLData2Op[dut][piece];		// 동적할당된 vcetor를 반환한다.
                g_pvBDLData2Op[dut][piece] = NULL;		// 반환한 vector 포인터도 NULL 처리

                //MyTrace(PRT_BASIC, "DeleteAllOpenTestItem(): dut=%d, piece=%d\n\n\n", dut, piece);
            }
        }
    }

    g_nOpenTestCount = 0;

    //__PrintMemSize(FUNC(DeleteAllOpenTestItem), __LINE__);       //son200420-4WNET10000
#endif
}


//son ShortTest Data를 *g_pvBDLData31[MAX_DUT+1][MAX_PIECE+1] vector 메모리에 추가 한다.   //son200420-4WNET10000
int InsertShortTest(int nDut, int nPiece, CShortTestItem shortTestItem)
{
#ifdef __OPVECTOR__
    CString strTemp;

	// g_nShortTestCount가  MAX_SHORT_TEST 갯수를 넘어서면 ShortTest 추가를 중지
	if (g_nShortTestCount >= (MAX_SHORT_TEST + 1))    // 1번 vector부터 사용하므로 사이즈1 추가
		return -1;

	// g_pvBDLData31[nDut][nPiece]가 없다면 새로 new해서 생성한다. 
	// 문제있다면 -1리턴.  이미 존재한다면 그냥 0리턴.  
	// -1로 리턴된 경우만 아니라면 g_pvBDLData31[nDut][nPiece] vector를 사용할 수 있다.
	int ret = InsertShortTest_Vector(nDut, nPiece);
	if (ret < 0)
		return -1;

	// vector 1개의 size는 32768을 넘을 수 없다.
	if (g_pvBDLData31[nDut][nPiece]->size() > (MAX_PIN + 1))    
	{
		strTemp.Format("g_pvBDLData31[%d][%d]->size()=%d, capacity()=%d, g_nShortTestCount=%d\n Can't Insert %d over ShortTestItem",
					nDut, nPiece, g_pvBDLData31[nDut][nPiece]->size(), 
					g_pvBDLData31[nDut][nPiece]->capacity(), g_nShortTestCount, MAX_PIN);
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
		return -1;
	}

	// vector의 item 갯수가 2048개를 넘는다면 vector 공간을 더 확보한다.  
	// 2048이 되는 시점에 1회성으로 수행하면, vector resize 도중에 "메모리 부족" 메시지 나오는 현상을 막을 수 있다.
	if ( g_pvBDLData31[nDut][nPiece]->size() == 2048)		
	{		
        MyTrace(PRT_BASIC, "\n");
        //__PrintMemSize(FUNC(InsertShortTest), __LINE__);       //son200420-4WNET10000
		// Test Print
		MyTrace(PRT_BASIC, "g_pvBDLData31[%d][%d]->size()=%d, capacity()=%d, max_size()=%d, g_nShortTestCount=%d\n",
					nDut, nPiece, 
					g_pvBDLData31[nDut][nPiece]->size(),
					g_pvBDLData31[nDut][nPiece]->capacity(), 
					g_pvBDLData31[nDut][nPiece]->max_size(), g_nShortTestCount);

		MyTrace(PRT_BASIC, "g_pvBDLData31[%d][%d]->size() = 2048! reserve other space for %d Pins..  \n", 
				    nDut, nPiece, MAX_PIN);
		
		g_pvBDLData31[nDut][nPiece]->reserve(MAX_PIN + 1);

		MyTrace(PRT_BASIC, "g_pvBDLData31[%d][%d]->size()=%d, capacity()=%d, max_size()=%d, g_nShortTestCount=%d\n",
					nDut, nPiece, 
					g_pvBDLData31[nDut][nPiece]->size(),
					g_pvBDLData31[nDut][nPiece]->capacity(), 
					g_pvBDLData31[nDut][nPiece]->max_size(), g_nShortTestCount);

        //__PrintMemSize(FUNC(InsertShortTest), __LINE__);       //son200420-4WNET10000
	}


	// 해당 vector에 실제 shortTestItem을 insert한다.
	//   :  time 갯수를 가질 vector에 이번 time의 shortTestItem을를 push_back 한다. 
	g_pvBDLData31[nDut][nPiece]->push_back(shortTestItem);
	g_nShortTestCount++;


	// g_nShortTestCount가  MAX_SHORT_TEST 갯수를 넘어서면 ShortTest 추가를 중지
	if (g_nShortTestCount == (MAX_SHORT_TEST +1))
	{
		strTemp.Format("g_nShortTestCount=%d, Stop InsertShortTest()!!\nCurrent nDut=%d, nPiece=%d\n", 
				                        g_nShortTestCount , nDut, nPiece); 
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
	}
#endif
	return 0;
}

//son  g_pvBDLData31[nDut][nPiece]가 없다면 새로 new해서 생성한다.     //son200420-4WNET10000
//     이미 존재한다면 그냥 0리턴, 문제있다면 -1리턴
int InsertShortTest_Vector(int nDut, int nPiece)
{
#ifdef __OPVECTOR__
	CString strTemp;

	//son nDut, nPiece 모두 1부터 시작
	if ((nDut <= 0 || nDut > MAX_DUT) || (nPiece <= 0 || nPiece > MAX_PIECE))
	{ 	
		strTemp.Format("InsertShortTest_Vector(): nDut=%d, nPiece=%d  Range(0<Dut<=%d, 0<Piece<=%d) Over Error!\n",
				                                nDut, nPiece, MAX_DUT, MAX_PIECE);
		//ERR.Set(RANGE_OVER, strTemp); 
		//ErrMsg();  ERR.Reset(); 
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
		return -1; 
	}
	
	// 해당 Dut, Piece 위치에 data가 insert된 적이 없다면  vector를 새로 생성해서 assign한다.
	//  : Dut * Piece 갯수의 vector 포인터에 실제 vector를 assign
	if (g_pvBDLData31[nDut][nPiece] == NULL)
	{
		vector <CShortTestItem>*  pvShortTest = new vector <CShortTestItem>;
		if (pvShortTest == NULL)		// 메모리할당 실패!
		{
			strTemp.Format("InsertShortTest_Vector() Fail!: nDut=%d, nPiece=%d\n", nDut, nPiece);
			//ERR.Set(MEM_ALLOC_FAIL, strTemp); 
			//ErrMsg(-1, TRUE);  ERR.Reset(); 
			//ErrMsg();  ERR.Reset(); 
            AfxMessageBox(strTemp, MB_ICONINFORMATION);
            MyTrace(PRT_BASIC, strTemp);
			return -1;
		}
#if 0   //son211020
		// test print
		else
        {
            MyTrace(PRT_BASIC, "\n");
			MyTrace(PRT_BASIC, "InsertShortTest_Vector(): nDut=%d, nPiece=%d, g_nShortTestCount=%d\n", 
			                    nDut, nPiece, g_nShortTestCount);
        }
#endif

		g_pvBDLData31[nDut][nPiece] = pvShortTest;
	}
#endif

	return 0;
}

//son 해당 DUT, Piece의 ShortTest data를 삭제하고 할당된 memory를 delete한다.    //son200722-BDL-Sort
void ZeroPieceShortTestItem(int nDut, int nPiece)
{
#ifdef __OPVECTOR__
	int prevTupleSize = 0;

    //__PrintMemSize(FUNC(DeletePieceShortTestItem), __LINE__);       //son200420-4WNET10000
	
    if(g_pvBDLData31[nDut][nPiece] != NULL)
    {
        prevTupleSize = g_pvBDLData31[nDut][nPiece]->size() ;	// nPiece마다 tupleSize가 다름!

        // vector의 모든 data를비우고 (모든 tuple을 삭제하고)
        if (g_pvBDLData31[nDut][nPiece]->empty() == false)
            g_pvBDLData31[nDut][nPiece]->clear();		

        //son  Zero화 시키는 것이므로 Vector delete를 하지는 않는다. (코멘트처리)
        //delete g_pvBDLData31[nDut][nPiece];		// 동적할당된 vcetor를 반환한다.
        //g_pvBDLData31[nDut][nPiece] = NULL;		// 반환한 vector 포인터도 NULL 처리

        // 삭제한 현재 dut의 ShortTest 카운트만큼 전체 ShortTest 카운트를 줄여 준다.
        g_nShortTestCount -= prevTupleSize;

        MyTrace(PRT_BASIC, "ZeroPieceShortTestItem(): nDut=%d, nPiece=%d, g_nShortTestCount=%d \n\n", 
                                                    nDut, nPiece, g_nShortTestCount);
    }


    //__PrintMemSize(FUNC(DeletePieceShortTestItem), __LINE__);       //son200420-4WNET10000
#endif
}

//son 해당 DUT의 ShortTest data를 삭제하고 할당된 memory를 delete한다.    //son200420-4WNET10000
void DeleteDutShortTestItem(int nDut)
{
#ifdef __OPVECTOR__
	int prevTupleSize = 0;

    //__PrintMemSize(FUNC(DeleteDutShortTestItem), __LINE__);       //son200420-4WNET10000
	
    for (int piece =1; piece <= MAX_PIECE; piece++)
    {
        if(g_pvBDLData31[nDut][piece] != NULL)
        {
            prevTupleSize = g_pvBDLData31[nDut][piece]->size() ;	// piece마다 tupleSize가 다름!

            // vector의 모든 data를비우고 (모든 tuple을 삭제하고)
            if (g_pvBDLData31[nDut][piece]->empty() == false)
                g_pvBDLData31[nDut][piece]->clear();		


            delete g_pvBDLData31[nDut][piece];		// 동적할당된 vcetor를 반환한다.
            g_pvBDLData31[nDut][piece] = NULL;		// 반환한 vector 포인터도 NULL 처리

            // 삭제한 현재 dut의 ShortTest 카운트만큼 전체 ShortTest 카운트를 줄여 준다.
            g_nShortTestCount -= prevTupleSize;

            //MyTrace(PRT_BASIC, "DeleteDutShortTestItem(): nDut=%d, piece=%d, g_nShortTestCount=%d \n\n", 
            //                                            nDut, piece, g_nShortTestCount);
        }
    }


    //__PrintMemSize(FUNC(DeleteDutShortTestItem), __LINE__);       //son200420-4WNET10000
#endif
}


// 모든 ShortTest data를 삭제하고 할당된 memory를 delete한다.    //son200420-4WNET10000
void DeleteAllShortTestItem()
{
#ifdef __OPVECTOR__
	int dut;

    MyTrace(PRT_BASIC, "\n");
	
	for (dut =1; dut <= MAX_DUT; dut++)
        DeleteDutShortTestItem(dut);

    g_nShortTestCount = 0;

#endif
}



//son 1부터 시작하는 dutId를 리턴한다.
int	getDutId()
{

	if ((nMulNetFlag > 1) && (nMulNetFlag <= nMultyNetTotal))
		return nMulNetFlag;

	return 1;

}


//son-4WDUT-20200312:
//son 0부터 시작하는 g_s4WNet용 dutIdx를 리턴한다.  dutIdx = dutId -1
int	get4wDutIdx()
{

#ifdef __4W_DUT1__
    //son-4WDUT-20200312 : 4W Multi DUT 문제점 때문에  무조건 0으로 리턴하는 코드 추가함.
    //          DUT 하나만이라도 되도록.  임시 코드 . multi DUT 해결이 되면 원상복구해야 한다. 
    //          문제점 해결 되면 원상복구 해야함!!!   __4W_DUT1__은 컴파일 옵션에서 빼고 컴파일하면 된다.
    
	return 0;
#else

    return (getDutId() -1) ;
#endif
}


/*
void DoEvents(DWORD dwMilliseconds)
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	Sleep(dwMilliseconds);
}

void Delay(UINT delaytime, UINT unit)
{
	// 입력값 측정값 검증
	// 1->7.5~8.5u  2->9u 4->11u 5->12 7->13u 10->17u
	// 100->106u 200->206u 500->506u

	//static LONGLONG FreqQuadPart=0;
	static double FreqQuadPart=0;
	static LARGE_INTEGER Freq, ZeroCnt, CurCnt;

	// 고해상도 타이머의 1초당 진동수 획득
	if(!QueryPerformanceFrequency(&Freq)) return;

	switch(unit)
	{
	case sec:
		FreqQuadPart = (double)Freq.QuadPart;
		break;

	case msec:
		FreqQuadPart = (double)Freq.QuadPart/1000.0;
		break;

	default:
	case usec:		// 1us(micro sec) 당 진동수로 변환
		FreqQuadPart = (double)Freq.QuadPart/1000000.0;
		break;
	}
	
	//---------------------------------------------------
	// ex) delaytime=200, unit=usec(2) 라면
	// 	    200 micro sec 동안 DoEvents()를 반복하여 수행한다.
	

#if 0		
	// Elapsed time도 표시하기
	__int64	startTime, endTime;
	startTime = GetMicroSecond();

	QueryPerformanceCounter(&ZeroCnt);	// 시작시점의 counter 값 획득
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()를 수행한 이후의 counter 값 획득
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	endTime = GetMicroSecond();

	MyTrace(PRT_LEVEL1, "			Delay(%d %s), Elapsed time=%d(usec)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?",  (endTime - startTime));

#else
	QueryPerformanceCounter(&ZeroCnt);	// 시작시점의 counter 값 획득
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()를 수행한 이후의 counter 값 획득
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	MyTrace(PRT_LEVEL1, "			Delay(%d %s)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?" ) ;

#endif


}

__int64 GetMicroSecond()  //sylee
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();

	return((((__int64)now.QuadPart) * ((__int64)1000000)) / ((__int64)frequency.QuadPart));
}
*/

//son210420 ChildView7.cpp에서 Globals.cpp 로 이동
BOOL FileExists(CString strFilePath)
{
	CFileFind aFile;
	BOOL IsExist = aFile.FindFile(strFilePath);
	aFile.Close();
	return IsExist;
}


//son211222 ChildView7.cpp에서 Globals.cpp 로 이동. FileExists()와 중복되어 삭제
/*BOOL CheckFileExist(char * pn)      //son210906
{
    CFileFind fn;

    if( !fn.FindFile(pn,0) ) return FALSE;
    else return TRUE;
}
*/


//son211222 ChildView7.cpp에서 Globals.cpp 로 이동
BOOL MakeNewDir(CString DirPath)
{
    int ret;
    ret = _mkdir((char*)(LPCTSTR(DirPath)));

    if(ret == -1)   return FALSE;
    else            return TRUE;
}



//son211222 ChildView7.cpp에서 Globals.cpp 로 이동
//son240110 Sub 폴더가 있다면 false가 리턴되는 문제가 있음. 
//          sub 폴더가 있는 폴더 체크는 PathIsDirectory(buf)를 사용하자. 
BOOL CheckDirExist(char * pn)
{
    CFileFind fn;
    char buf[FILE_PATH_SIZE];   //son210420  100 -> FIlE_PATH_SIZE
    int nextfile;

    sprintf(buf,"%s\\*.*", pn);

    if( !fn.FindFile(buf,0) )
    {
    //  AfxMessageBox("File Searching Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    nextfile = fn.FindNextFile();

//    CString strFile;

    while(1)
    {
        if( fn.IsDirectory() ) 
        {
            //strFile = fn.GetFileName();

            if( !fn.IsDots() )  //son240110 Sub 폴더가 있다면 false가 리턴된다.
                return FALSE;
        }
        if(!nextfile) 
            break;
        nextfile = fn.FindNextFile();
    }
    return TRUE;
}


//son211222 ChildView7.cpp에서 Globals.cpp 로 이동
BOOL CopyFileDir(char* dest, char* src, int deletemode/*=FALSE*/)
{
    int cnt=0, nt, i;
    char buf[FILE_PATH_SIZE];   //son210420  100 -> FIlE_PATH_SIZE

    CFileFind ff;

    sprintf(buf,"%s\\*.*",src);
//  cnt = FileCountInDir(buf);

    if( !cnt )
    {
        return FALSE;
    }

//  if( deletemode ) FileDeleteAll(dest);

    CString* filename = new CString[cnt+1];

    cnt=0;
    if( !ff.FindFile(buf,0) ) return FALSE;
    nt = ff.FindNextFile();
    while(1)
    {
        if( !ff.IsDirectory() )
        {
            filename[cnt] = ff.GetFileName();
            cnt++;
        }
        if( !nt ) break;
        nt = ff.FindNextFile();
    }

    char destfile[100];
    char srcfile[100];

    for(i=0; i<cnt; i++)
    {
        sprintf(srcfile,"%s\\%s", src, filename[i]);
        sprintf(destfile, "%s\\%s", dest, filename[i]);
        if( !CopyFile(srcfile, destfile, FALSE) )
        {
            delete[] filename;
            return FALSE;
        }
    }

    delete[] filename;
    return TRUE;
}



void GetDateAndTime(char *datetime)
{
	char buf[24];
	memset( buf, 0x00, 24 );

    time_t ltime;
    time( &ltime );

	struct tm *today;
	today = localtime( &ltime );
   
	strftime( buf, 24, "%Y%m%d", today );
 	strcpy(datetime,buf);
}


//
// 2018.06.18:  psapi.h 와 psapi.lib가 포함된 VisualC++ 6.0 SDK가 없어서 아래 코드는 사용 불가.
// VC2008에서 카피해오는 방법이 있다고는 함.
// https://stackoverflow.com/questions/4560468/compile-an-exe-in-microsoft-visual-c-6-0-using-the-psapi-h
/*
// 현재 이 process가 사용가능한 메모리의 사이즈를 가져온다.
SIZE_T GetProcessWorkingSetSize() 
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		return 0;
	}
	return pmc.WorkingSetSize;
}

//memory usage 사용법
SIZE_T usage = GetProcessWorkingSetSize();
//something
usage = GetProcessWorkingSetSize() - usage;


*/


/*
 * Simple utility to call GetProcessMemoryInfo without having 
 * to compile or link with psapi.h / psapi.lib.
 */

typedef struct {
   DWORD cb;
   DWORD PageFaultCount;
   DWORD PeakWorkingSetSize;
   DWORD WorkingSetSize;
   DWORD QuotaPeakPagedPoolUsage;
   DWORD QuotaPagedPoolUsage;
   DWORD QuotaPeakNonPagedPoolUsage;
   DWORD QuotaNonPagedPoolUsage;
   DWORD PagefileUsage;
   DWORD PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;

#define Win32MemArgs HANDLE, PROCESS_MEMORY_COUNTERS *, DWORD

extern BOOL WINAPI GetProcessMemoryInfo( Win32MemArgs );

typedef BOOL (WINAPI *GetMemInfo)( Win32MemArgs );

PROCESS_MEMORY_COUNTERS MyGetMemInfo()
{
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE                  hProc;
    char                   *pf;
    BOOL                    bOK;
    static HINSTANCE        _hLib = (HINSTANCE)0;
    static GetMemInfo       _fcn  = (GetMemInfo)0;

    /*
     * Dynamically Load library (once)
     */
    if ( !_hLib && (_hLib=::LoadLibrary( "psapi.dll" )) ) {
        pf   = "GetProcessMemoryInfo";
        _fcn = (BOOL (WINAPI *)( Win32MemArgs ))::GetProcAddress( _hLib, pf );
    }

    /*
     * Call fcn
     */
    bOK = false;
    if ( _fcn && (hProc=::GetCurrentProcess()) )
        bOK = (*_fcn)( hProc, &pmc, sizeof( pmc ) );

    if ( !bOK )
       memset( &pmc, 0, sizeof( pmc ) );
    return pmc;
}


SIZE_T GetProcessWorkingSetSize() 
{
	PROCESS_MEMORY_COUNTERS pmc;
	//if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
	//	return 0;
	//}

	pmc = MyGetMemInfo();
	
	return pmc.WorkingSetSize;
}


void __PrintMemSize(char* strFunc, int line)
{
	MyTrace(PRT_BASIC, "%s() Line_%d: memsize= %lu\n", 
			strFunc, line, GetProcessWorkingSetSize());

}

void __PrintLine(char* strFile, int line)
{
	MyTrace(PRT_BASIC, "%s Line_%d\n", strFile, line);

}

//2017.11.06  TRACE의 buffer가 512까지 밖에 허용하지 않아서 아래와 같이 신규 함수 생성
//  buffer size = 2048
//
static CCriticalSection g_cs;		

void AFX_CDECL MyTrace(ePRT_TYPE prtType, LPCTSTR lpszFormat, ...)
{

#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return;
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
		return;



	// log 파일에 printout 준비 -------
	g_cs.Lock();		// CriticalSection 설정

	char date_time[24]; 
	char path[FILE_PATH_SIZE];      //son210420 FILE_PATH_SIZE: 255
	memset(date_time,0x00,sizeof(date_time));
	memset(path,0x00,sizeof(path));
	GetDateAndTime(date_time);
	CTime t = CTime::GetCurrentTime();

	
	//son220419_8 Current ACE500\LOG\log_20111122_12 -> d:\log\log_20111122로 폴더와 name을 변경 (sylee 요청)
	//sprintf(path, "%s\\log_%s_%02d.txt", g_sFile.LogDir, date_time, t.GetHour()); // ex) .\\LOG\log_20180608_13.txt   
	sprintf(path, "d:\\log\\log_%s.txt", date_time); // ex) d:\\log\\log_20111122.txt 


	int nBuf;
	TCHAR szBuffer[2048];
	va_list args;
	va_start(args, lpszFormat);
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	va_end(args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

#ifdef _DEBUG 
	// OUTPUT 디버그 창에 print out----------------
	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << szBuffer;
#endif

#if 1
	// log 파일에 print out ----------
	CFile f;
	f.Open( path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate );
	f.SeekToEnd();	



	// time 헤더 print --------
	t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format( "[%02d:%02d:%02d] ", t.GetHour(), t.GetMinute(), t.GetSecond());
	//strTime.Format( "%02d:%02d:%02d (%12lu) ", t.GetHour(), t.GetMinute(), t.GetSecond(),  GetProcessWorkingSetSize() );

	f.Write( strTime, strTime.GetLength() );


	// data print --------
	CString strTemp = szBuffer;
	strTemp.Replace("\n", "\r\n");		// \n을 \r\n으로 변경.
	f.Write( strTemp, strlen(strTemp) );

	f.Close();
#endif

#if 0
	// log 파일에 print out ----------
	CFile f;
	CArchive ar(&f, CArchive::store);
	f.Open( path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	f.SeekToEnd(); 	// 기존 파일내용의 뒤에 붙여 쓰기 위해 이동	

	CString strTime;
	t = CTime::GetCurrentTime();
	strTime.Format( "%02d:%02d:%02d ", t.GetHour(), t.GetMinute(), t.GetSecond() );

	ar.WriteString(strTime);

	CString strTemp = szBuffer;
	strTemp.Replace("\n", "\r\n");		// \n을 \r\n으로 변경.
	f.Write( strTemp, strlen(strTemp) );

	ar.WriteString(strTemp);

	ar.Close();
	f.Close();
#endif

	g_cs.Unlock();		// CriticalSection 해제
}

//son 계측기 SW의 Event Log 중에서 2달 전의 파일은 모두 삭제한다. 
//son ex) CleanUp_MyTraceLog("d:\\log"); 
//        CleanUp_MyTraceLog("c:\\ACE500\log"); 
void CleanUp_MyTraceLog(char *pn)    //son220510 추가 
{
    CFileFind fn;
    char path[FILE_PATH_SIZE], filepath[FILE_PATH_SIZE];   

    sprintf(path,"%s\\*.*", pn);
    BOOL bWorking = fn.FindFile(path, 0);

    MyTrace(PRT_BASIC, "CleanUp_MyTraceLog(%s)\n", pn);

    while(bWorking)
    {
        bWorking = fn.FindNextFile();

        // skip . and .. files; otherwise, we'd
        // recur infinitely!
        if (fn.IsDots())
            continue;

        if (fn.IsDirectory())
            continue;

        CString dataFileName = fn.GetFileName();

        //son "log_20220315_08.txt" 이런 형식의 파일을 찾는다.
        if (dataFileName.Find("log_20") < 0)
            continue;

        if (dataFileName.Find(".txt") < 0)
            continue;

        CString yearName  = dataFileName.Mid(4, 4);
        CString monthName = dataFileName.Mid(8, 2);
        CString dayName   = dataFileName.Mid(10, 2);
        int fileYear   = atoi(yearName);
        int fileMonth  = atoi(monthName);
        int fileDay    = atoi(dayName);
        CTime  curTime = CTime::GetCurrentTime();
        sprintf(filepath, "%s\\%s", pn, dataFileName);

        //son 현재 2022년이면 2020년과 2020년 이전의 파일을 지운다. 
        if (fileYear < (curTime.GetYear() -1)) 
        {
            MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
            DeleteFile(filepath);
            continue;
        }

        //son fileYear가 직전 연도인 경우 (현재 2022년이면 2021년인 경우)
        if (fileYear < curTime.GetYear()) 
        {
            //son fileYear가 직전 연도인데 현재 1월이면,  이전년도 1월~ 11월의 파일을 지운다. (작년 12월, 현재 1월 남김)
            if( curTime.GetMonth() == 1  ) 
            {                
                if(fileMonth != 12) 
                {
                    MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
                    DeleteFile(filepath);
                    continue;
                }

            }
            //son fileYear가 직전연도인데  현재 1월이 아니라면 아니라면  직전연도의  모든 파일을 지운다.
            else
            {
                MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
                DeleteFile(filepath);
                continue;
            }
        }

        //son 현재 5월이면 3월 포함해서  3월 이전의 파일들은 모두 지운다. (1,2,3 월 지움)
        if (fileMonth <= (curTime.GetMonth() -2) ) 
        {
            MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
            DeleteFile(filepath);
            continue;
        }

    }

}

// hdpark231121 Spark log 삭제
int CleanUp_SparkLog(LPCTSTR szDir, int nRecursive, int nMonth)
{
    HANDLE handle;
    WIN32_FIND_DATA wfd;
    int bExist = 1;

    TCHAR DelPath[MAX_PATH], FullPath[MAX_PATH], TempPath[MAX_PATH];

    _tcscpy(DelPath, szDir);
    _tcscpy(TempPath, szDir);

    if (_tcscmp(DelPath + _tcslen(DelPath) - 4, _T("\\*.*")) != 0)
    {
        _tcscat(DelPath, _T("\\*.*"));
    }

    handle = FindFirstFile(DelPath, &wfd);

    if (handle == INVALID_HANDLE_VALUE)
    {
        if (nRecursive > 0) 
            RemoveDirectory(TempPath);
        return -1;
    }

    unsigned long nToday, nFileNo, nDeleteNo;
    TCHAR szDate[24];

    time_t ltime;
    time(&ltime);

    struct tm* today;
    today = localtime(&ltime);

    strftime(szDate, 24, "%Y%m%d", today);
    nToday = _ttol(szDate);
    nDeleteNo = nToday - (nMonth * 100);	// Month

    while (bExist)
    {
        wsprintf(FullPath, _T("%s\\%s"), TempPath, wfd.cFileName);

        CString str;
        str.Format("%s", wfd.cFileName);
        nFileNo = _ttol(str);           // file name : yyyymmdd

        if (nFileNo > nDeleteNo)
        {
            bExist = FindNextFile(handle, &wfd);
            continue;
        }

        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
        {
            SetFileAttributes(FullPath, FILE_ATTRIBUTE_NORMAL);
        }

        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (_tcscmp(wfd.cFileName, _T(".")) && _tcscmp(wfd.cFileName, _T("..")))
            {
                nRecursive++;
                CleanUp_SparkLog(FullPath, nRecursive, nMonth);
                nRecursive--;
            }
        }
        else
        {
            DeleteFile(FullPath);
        }

        bExist = FindNextFile(handle, &wfd);
    }

    FindClose(handle);

    if (nRecursive > 0) 
        RemoveDirectory(TempPath);

    return 0;
}


//son err 메시지박스 출력. 
//    err 코드를 포함해서 디버그용 'Output'화면 Trace와 d:\log 파일출력, 메시지박스를 함께 출력한다.
void errMessageBox(int nError, LPCTSTR errStr)  //son230217_2
{
    CString str;

    str.Format("Error No %d!\n\n%s", nError, errStr);
    AfxMessageBox(str, MB_OK);
    MyTrace(PRT_BASIC, str);
}


//son221215 begin: d 드라이브 용량 체크 기능 추가. 
void Check_DDrive_FreeSpace()    //son221215 추가 
{
    ULARGE_INTEGER  uliAvailable, uliTotal, uliFree; //son byte 
    CString strDrive, str;
    
    strDrive.Format("d:");
    GetDiskFreeSpaceEx(strDrive, &uliAvailable, &uliTotal, &uliFree);

    int iAvailable= (int)(uliAvailable.QuadPart >> 10); //son K byte
    int iTotal= (int)(uliTotal.QuadPart >> 10);
    int iFree= (int)(uliFree.QuadPart >> 10);

    MyTrace(PRT_BASIC, "Disk(%s) total(%.3fM), using(%.3fM), free(%.3fM) \n",  
                            strDrive, iTotal/(double)1000, (iTotal - iFree) /(double)1000, iFree /(double)1000);

    if (iFree < 100000) //son 100Mbyte 이하의 용량이면 경고를 출력.
    {
        str.Format("Warning No 1900. \n\n %s Drive has insufficient disk space under 100 Mbytes.  \n\nFree byte is %.3f Mbytes.\nPlease clean up the Disk.\n",
                                        strDrive, iFree /(double)1000);
        MyTrace(PRT_BASIC, str);
        AfxMessageBox(str, MB_OK);
    }

}

//son221215 end


#if 0
//2018.02.14  성능개선을 위해 print off이면 snprintf를 수행하지 않는 기능 추가.
int AFX_CDECL MySnprintf(ePRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return (-1);
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
	{
		return (-1);
	}

	va_list args;
	va_start(args, lpszFormat);

	int nBuf = _vsnprintf((char*)pStr, strSize, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	va_end(args);

	return nBuf;
}

// MyTrace와 다른 점. : pStr 포인터를 입력받아서 호출이후에 출력한 str을 재사용할 수 있다.
int AFX_CDECL MyTrace2(ePRT_TYPE prtType, char *pStr, int strSize, LPCTSTR lpszFormat, ...)
{
	int nBuf = 0;
#ifdef _DEBUG 
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return (-1);
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
	{
		return (-1);
	}

	va_list args;
	va_start(args, lpszFormat);

	//char* pStr = str.GetBuffer(2048);	// 이렇게 하니까 죽는다.. 
	//int	  strSize = str.GetLength();
	nBuf = _vsnprintf(pStr, strSize, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << pStr;

	va_end(args);

#endif
	return nBuf;
}


#endif


//sylee20120308 Calibration convertor
//son220405 moved from ChildView8.cpp to Globals.cpp 
//
// m_nMode1 1~76   
// ConMode=1: RAW_TO_REAL (raw -> real)   //son AD Raw 값을 real ohm R값으로 convert     
// ConMode=2: REAL_TO_RAW (real-> raw)    //son real ohm R 값을 AD Raw값으로 convert
// part 0:PART_UPPER, 1:PART_LOWER,  2:PART_MIXED

//son Rvalue를 Real->Raw로 바꾸거나 Raw->Real로 바꾸어서 Sub71.R1에 설정. (2W용)
double RCalCovert(int m_nMode1, double Rvalue, int ConMode, int part)      //son220412
{

    int k,   nIn, nOut;
    double  aa, ab;

    CString str, str2;

    aa=0.0;
    ab=0.0;

    if( ConMode== REAL_TO_RAW ){    //son220412_3 2:REAL_TO_RAW
        // real ohm  -> raw ADC     
        //son nIn  : gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList] : ref R(realR ohm) 
        //son nOut : gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList] : cal R(rawR ADC)
        nIn=  CAL_REF_R;      //son220407 2
        nOut= CAL_REF_RAWR;   //son220407 3
    }

    //ConMode: 1(RAW_TO_REAL)
    else{  
        // raw ADC -> real ohm 	    
        //son nIn : gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList] : cal R(rawR ADC)
        //son nOut: gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList] : ref R(realR ohm) 
        nIn=  CAL_REF_RAWR;  //son220407 3
        nOut= CAL_REF_R;     //son220407 2
    }

    int nRCount=(int)gdCal1[part][m_nMode1][0][0];// Cal  list 개수

//son230925 begin: RCalCovert() Range 체크 기능 보완
    //----------------------
    //son Range Check
    //----------------------
    if (Rvalue < 0)
    {
        Rvalue = -1;
        Sub71.R1=Rvalue; //input2
        return Sub71.R1;
    }

    //son RAW_TO_REAL SHORT : 내림차순 정렬된 ADC raw 값이 기준값 (nIn)
    if (ConMode== RAW_TO_REAL && m_nMode1 >= MODE_SHORT_START) 
    {
        if (Rvalue > 65535) //son Short의 ADC 값은 I값이어서  65535 보다 클 수 없으므로 예외 처리
        {                   //son OPEN의 ADC 값은 VC/I 값이어서 65535 보다 클 수 있다. 
            Rvalue = -1; 
            Sub71.R1=Rvalue; //input2
            return Sub71.R1;
        }
        
        //son230925 min 값 1/2 보다 더 작더라도 마지막 기울기로 처리하도록 한다.
        //son230925 double dMinLookupVal = gdCal1[part][m_nMode1][nIn][nRCount] / 2; //son Min 값의 1/2 까지는 근사값으로 처리
        //son230925 if (Rvalue < dMinLookupVal) //son Min 값의 1/2 보다 작으면 에러처리 
        //son230925 {
            //str.Format("CalConvert(Mode:%d, ConMode:RAW_TO_REAL) Rvalue(%.2f) is Lower than Limit(%.2f)\n", 
            //                       m_nMode1, Rvalue, dMinLookupVal);
            //str2 += str;
            //str.Format("Set Rvalue to 0. Check the SETUP\\CAL%dB.ini file. \n", m_nMode1);  
            //str2 += str;
            //errMessageBox(7223, str2);
            //son230925 Rvalue = 0;
        //son230925 }

    }
    //son 기준값이 오름차순 정렬된 케이스
    //    1) REAL_TO_RAW OPEN 
    //    2) REAL_TO_RAW SHORT, 
    //    3) RAW_TO_REAL OPEN,  
    else    
    {


 //sylee231130 영풍 10V (LV)로  jig away 상태에서 Open Test 시에 C-O 발생문제점 보완
 //           I 값이 미미해서  R를 999999로 처리한 경우에  여기에서 R값이 -1로 변경되고 
 //           다시 걸러져서 0.1ohm 으로 처리되었다가 최종적으로 Auto 화면에  'R < 0 ohm' 으로 잘못 출력되는 문제 발생했음. 
 /*
        double dMaxLookupVal = (gdCal1[part][m_nMode1][nIn][nRCount]) * 2;   //son Max 값의 2배까지는 근사값 처리를 한다.
        if (Rvalue > dMaxLookupVal) //son Max 값의 2배를 넘으면 에러처리.
        {
            //str.Format("CalConvert(Mode:%d, ConMode:%s) Rvalue(%.2f) is over MaxVal(%.2f)\n", 
            //        m_nMode1, (ConMode == REAL_TO_RAW) ? "REAL_TO_RAW": (ConMode == RAW_TO_REAL) ? "RAW_TO_REAL": "UNDEF",
            //        Rvalue, dMaxLookupVal);
            //str2 += str;
            //str.Format("Set Rvalue to 0. Check the SETUP\\CAL%dB.ini file. \n", m_nMode1);  
            //str2 += str;
            //errMessageBox(7223, str2);
            Rvalue = -1;
            Sub71.R1=Rvalue; //input2
            return Sub71.R1;
        }
  *///sylee231130

    }


//son230925 end

    Sub71.R1=Rvalue; //input2

    if( (Sub71.R1>0) && (nRCount>0) && (nRCount<=MAX_CAL_RLIST) )       //son220408 128: MAX_CAL_RLIST
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        //----------------------
        //son 위치 찾기 
        //----------------------
        for( k=1;  k<=nRCount;  k++){
            // ConMode RAW_TO_REAL이고  SHORT의 경우, CALR%d.ini 파일이 rawR이 내림차순으로 정렬되어 있으므로 첫번째 작은 값이 근사값이다.
            if( ( ConMode== RAW_TO_REAL)  && (m_nMode1>=MODE_SHORT_START) ){//sylee230824   //son230919 301-> 201:MODE_SHORT_START 
                if( Sub71.R1> gdCal1[part][m_nMode1][nIn][k]){//sylee230824  
                    break;  
                } 	
            }
            //son ConMode: REAL_TO_RAW는 Short, Open 오름차순 정렬된  cal table realR 중에  입력된 realR보다 첫번째 큰값이 근사값이다. 
            //    OPEN의 경우, CALR%d.ini 파일이 rawR이 오름차순으로 정렬되어 있으므로 첫번째 큰값이 근사값이다.
            else{
                if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k]){	 //son ConMode 1이면 ADC raw R
                    break;  
                } 
            }
        }

        //----------------------
        //son 기울기 계산 
        //----------------------
        //son k==1 이면 실제 찾는 위치는 k-1이다.
        if( k==1)  //son 첫번째 R보다 작은 값일 때  
        {
            //sylee230823 if(gdCal1[part][m_nMode1][nOut][0]<=0.0){ //son ConMode 1이면 ohm real R
            if(gdCal1[part][m_nMode1][nOut][0]<0.0){  //sylee230823-1
                Sub71.R1= -1.0 ;  
            }
            else
            {
                if((gdCal1[part][m_nMode1][nIn][2] - gdCal1[part][m_nMode1][nIn][1])!=0) //son devide be zero 예방
                {
                    //son230919 Sub71.R1= (Sub71.R1/gdCal1[part][m_nMode1][nIn][1])  *  gdCal1[part][m_nMode1][nOut][1]; 
                    //son230919 begin:
                    //son 첫번째 1번 항목일 때에는 1번과 2번 의 기울기를 연장해서 따르도록 한다.
                    aa= (Sub71.R1 - gdCal1[part][m_nMode1][nIn][1]) / (gdCal1[part][m_nMode1][nIn][2] - gdCal1[part][m_nMode1][nIn][1]); 
                    ab= gdCal1[part][m_nMode1][nOut][2] - gdCal1[part][m_nMode1][nOut][1]; 
                    Sub71.R1= ab * aa + gdCal1[part][m_nMode1][nOut][1];											 

                    if (Sub71.R1 < 0)   //son 2번 항목의 기울기로 1번을 계산하기 때문에 0보다 작게 나올 수도 있음. 
                        Sub71.R1 = 0;   //    0보다 작게 나오면 강제로 0으로 설정한다. 
                    //son230910 end
                }
                else{
                    //son230919 begin: 명확한 에러처리로 수정.  
                    //    if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k])로 break 했기 때문에 (<= 아님)  여기로 오는 것은 버그가 있는 경우임. 
                    str.Format("Divide by Zero. \n (gdCal1[part][%d][%d][2]:%.2f - gdCal1[part][%d][%d][1]:%.2f)  is 0! \n Mode=%d %s Converting Error !\n\n" , 
                            m_nMode1, nIn, gdCal1[part][m_nMode1][nIn][2], 
                            m_nMode1, nIn, gdCal1[part][m_nMode1][nIn][1], m_nMode1 ,  
                            (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R");   //son220412_3
                    errMessageBox(7223, str);
                    //son230919 end
                }
            } 		

        }
//son230919 대만 TTF(MST) Leak250V Mode 마지막 항목일 때 REAL_TO_RAW 계산 잘못되는 문제 관련 수정
//          마지막 항목도 기울기로 계산하도록 한 이후 문제점 해결됨.
/*
        //---------------
        //son 비율 계산
        //---------------
        else if( k==nRCount)    //son 마지막 R 
        {
            if(gdCal1[part][m_nMode1][nIn][nRCount]!=0){												     
                Sub71.R1= (Sub71.R1/gdCal1[part][m_nMode1][nIn][nRCount]) *   gdCal1[part][m_nMode1][nOut][nRCount]; 
            }
            else{
                str.Format("Error No 7202 ,   Divide by Zero. \n  gdCal1[part][m_nMode1][nIn:%d][nRCount] is 0! \n  Mode=%d  %s Converting Error !\n\n" ,
                                                nIn, m_nMode1,  (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R" );   //son220412_3
                AfxMessageBox(str);
            }
        }
*/
        //-----------------------
        //son 기울기 계산
        //-----------------------
        //son  ConMode 1일때,   rawRCal[k-1] < rawR < rawRCal[k] 인 케이스 
        else
        {
            //son230919 마지막 항목의 +근사값 처리기능.  
            if (k == (nRCount+1))
                k = k-1;     //son 마지막 항목의 근사값(nRCount+1)도 2배까지는  마지막 항목(nRCount)의 기울기로 계산하기로 한다.

            if( (gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1])!=0.0) //son devide be zero 예방
            {
                //son ConMode 1 인 경우:  ADC raw R -> ohm real R 로 동작
                //
                //ex)  a: rawRCal[k-1], b: realRCal[k-1],  c:rawRCal[k], d:realRCal[k],  x: rawR, y: realR 
                //     기울기 : (d-b)/(c-a)    y절편:  - (d-b)/(c-a)*a + b
                //     y = (d-b)/(c-a)x - (d-b)/(c-a)*a + b
                //     y = (d-b)/(c-a) * (x -a) + b     
                //     y = (x -a)/(c-a) * (d-b) + b   <==== (a,b) (c,d) 2개의 점으로 구한 1차 방정식
                //
                // realR = (rawR -a)/(c-a) * (d-b) + b
                // realR = (rawR - rawRCal[k-1])/(rawRCal[k] -rawRCal[k-1]) * (realRCal[k]-realRCal[k-1]) + realRCal[k-1]
                //
                //    aa = (rawR - rawRCal[k-1])/(rawRCal[k] -rawRCal[k-1]);
                //    ab = realRCal[k] - realRCal[k-1];
                // realR = aa * ab + realRCal[k-1];       <=== 기울기를 이용해서 x(rawR)에 대한 y(realR)을 구한다.
 
                aa=(Sub71.R1-gdCal1[part][m_nMode1][nIn][k-1])/(gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1]); 
                ab=gdCal1[part][m_nMode1][nOut][k]-gdCal1[part][m_nMode1][nOut][k-1]; 
                Sub71.R1=ab *aa +gdCal1[part][m_nMode1][nOut][k-1];											 
            }
            else{
                //son k-1과 k의 테이블 값이 동일해서 변경이 없다면 그냥 비율 계산
                //      realR/ ref realR 의 비율은 rawR/ ref rawR 비율과 같으므로
                //      realR = (rawR / RefrawR) * RefrealR
                //son230919 Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][nIn][k-1])*gdCal1[part][m_nMode1][nOut][k-1];	

                //son230919 begin: 명확한 에러처리로 수정.  
                //    if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k])로 break 했기 때문에 (<= 아님)  여기로 오는 것은 버그가 있는 경우임. 
                str.Format("Divide by Zero. \n (gdCal1[part][%d][%d][%d]:%.2f -gdCal1[part][%d][%d][%d]:%.2f) is 0! \n Mode=%d %s Converting Error !\n\n" , 
                                            m_nMode1, nIn, k, gdCal1[part][m_nMode1][nIn][k],
                                            m_nMode1, nIn, k-1, gdCal1[part][m_nMode1][nIn][k-1], m_nMode1 ,  
                                            (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R");   //son220412_3
                errMessageBox(7223, str);
                //son230919 end
            }

        }

    }

//son230925 begin:     
    //son RAW_TO_REAL(결과가 ohm인 경우) Open 최소값은 1 ohm
    if (ConMode==RAW_TO_REAL && m_nMode1 <= MODE_OPEN_END && Sub71.R1 < 1)    
    {
        Sub71.R1 = 1;   // 1보다 작게 나오면 강제로 1 ohm으로 설정.  
        return Sub71.R1;
    }
    //son RAW_TO_REAL(결과가 ohm인 경우) 4W 최소값은 0.1 mohm  (4W Cal은 단위가 mohm 임)
    if (ConMode==RAW_TO_REAL && 
            (m_nMode1 <= MODE_4W_START || m_nMode1 <= MODE_4W_END) && Sub71.R1 < 0.1)    
    {
        if(m_nMode1==MODE171_4W || m_nMode1==MODE176_4W_CV ) //sylee220204	//SYLEE220312ADD    //son220407
            Sub71.R1=0.1;
        else
            Sub71.R1=999999999;
          
        return Sub71.R1;
    }
//son230925 end

    return Sub71.R1;
									 
}

//son230925  4W RAW_TO_REAL는  0부터 기울기 체크하는 기능이 특별히 추가됨.  이상태로 현장 튜닝이 많이 돼 있으므로  
//           ARun201_4w_R_Cal1() 함수는 통합하지 않고 유지하기로 한다. 
//son220419_3 moved from ChildView7.cpp

//son Cal 반영하여 R값을 보정한다.   // Raw R -> Real R
int ARun201_4w_R_Cal1(int m_nMode1, int part)  //sylee150710   //son220412 part 추가
{
    int nPos, nRListCnt;
    double aa,ab;
    nPos=0;
    aa=0.0;
    ab=0.0;

    nRListCnt=(int)gdCal1[part][m_nMode1][0][0];// Cal  list 개수
    //      Sub71.R1=dR14w[j+5][i]; 
    if ((Sub71.R1>0) &&  (nRListCnt>0) &&   (nRListCnt<=MAX_CAL_RLIST))   //son220408 128: MAX_CAL_RLIST
    {                                    
        
        //son220407  4W는  2W용 RCalCovert()와 다르게 특별히 0부터 시작한다.    
        //           실제 값은 RList 1부터 gdCal1[part][m_nMode1][][RList]가 채워져 있지만
        //           4W는 1 ohm 이하로 측정되는 경우가 있어서 1 ohm 이하의 경우는 RList=0 인 경우와 기울기로 판단한다.
        //           2W는 반면에 20230925 현재 1 ohm 이하로 측정되는 경우는  RList=1, RList=2 사이의 기울기로 판단한다. (차이점)
        
        //son 해당 모드의 Calibration 값들 중에서 현재 측정R에 맞는 값을 찾는다.
        for( int RList1=0;  RList1<=nRListCnt;  RList1++)  // 4WSETUP   //sylee171113-2  
        {
            //son 'Use=1'인 경우   
			//son  FileSysInfo01.LoadSaveSer13(_LOAD,..) 할 때 이미 1인 경우에만 gdCal1에 write하고 nRcount가 증가한다.
            //     여기서 1을 체크해도 되고 안해도 됨. 
            if(gdCal1[part][m_nMode1][CAL_USE][RList1]==1)  //cal enable 1,0  //son220407
            {
                //son 측정한 R 값이 해당 모드의 모든 RList1 R 값중의 하나보다 작다면 stop.
                if( Sub71.R1< gdCal1[part][m_nMode1][CAL_REF_RAWR][RList1])      // cal R raw       //son220407
                {
                    nPos=RList1;            //son 해당 RList1 위치를 nPos로 기록하고
                    RList1=nRListCnt+1;    //son nRListCnt 만큼 바로 증가시켜서 for 문을 탈출 
                    break;//sylee171113-2
                } 
            }
        }                       

        if(nPos==0){       //gdCal1[part][m_nMode1][0:CAL_REF_R][k]  cal ref  R 
            Sub71.R1=999999; //sylee180607
        }
        else
        {
            //son  실제R / Real Ref R  == 측정R / Calibration Raw R  임을 이용해서  실제 R 값을 추정한다.
            if( (gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos] - gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1])!=0.0)   //son220407
            {
                aa=(Sub71.R1-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1])/(gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos]-gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1]);  // [3]= lookup table Calibration Raw R //son220407
                ab=gdCal1[part][m_nMode1][CAL_REF_R][nPos]-gdCal1[part][m_nMode1][CAL_REF_R][nPos-1];// [2:CAL_REF_R]= lookup table Calibration real R  //son220407
                Sub71.R1=ab*aa+gdCal1[part][m_nMode1][CAL_REF_R][nPos-1];                                          
            }
            else{
                //son Raw R -> Real R
                Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][CAL_REF_RAWR][nPos-1])*gdCal1[part][m_nMode1][CAL_REF_R][nPos-1];     //son220407
            }
        }
    }  
  
    //son R 값이 0.1 이하이면 171은 0.1,  171이외에는 999999999 로 설정.
    if(Sub71.R1<0.1)
    {
        if(m_nMode1==MODE171_4W || m_nMode1==MODE176_4W_CV ){ //sylee220204	//SYLEE220312ADD    //son220407
            Sub71.R1=0.1;
        }
        else{
            Sub71.R1=999999999;
        }
    }

   return 1;

}

int NetWorkConnection1()  //sylee231010-1  //son240705 mereged from ACE1000
{
    NETRESOURCE NetRes ; 
    memset(&NetRes,0,sizeof(NETRESOURCE));
    NetRes.dwType = RESOURCETYPE_DISK; 
    NetRes.lpLocalName  =  "Z:";                      
    NetRes.lpRemoteName = "\\\\192.168.0.1\\Fromtester" ;    //
    NetRes.lpProvider   = NULL;    
    LPCSTR strUserId = "Adminstrator";
    LPCSTR strPw = ""; 
	WNetAddConnection2(&NetRes, strPw, strUserId, CONNECT_UPDATE_PROFILE);    
    return 1;
}


#if 0 //son221014 begin:  RCalConvert_spark()를  RCalConvert_short()로 교체하여 함수 통일

// hdpark220819 begin: for leak current
//double RCalConvert_spark(int m_nMode1, double Rvalue, int ConMode, int part) 
// hdpark220819 end

#else
//son221014 moved from ModeSer15.cpp. 
//          ModeSer15::RCalCovert3() -> Globals.cpp RCalConvert_short() 로 교체. 함수통일 


#if 0   //son230925 RCalCovert() 재정비후 RCalConvert_short()를 RCalCovert()로 통일.

//son 내림차순으로 정렬된 Short Cal Data 에 사용한다.  
double RCalConvert_short(int mode, double Rvalue, int ConMode, int part)  //son221014 
{
    int k,   nIn, nOut;
    double  aa, ab;     
    CString str, str2;

    aa=0.0;
    ab=0.0;

    if (ConMode == REAL_TO_RAW) {    //son220412_3 2:REAL_TO_RAW
        // real ohm  -> raw ADC     
        //son nIn  : gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList] : ref R(realR ohm) 
        //son nOut : gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList] : cal R(rawR ADC)
        nIn = CAL_REF_R;       //son220407 2  기준값
        nOut = CAL_REF_RAWR;   //son220407 3
    }

    //ConMode: 1(RAW_TO_REAL)
    else {
        // raw ADC -> real ohm 	    
        //son nIn : gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList] : cal R(rawR ADC)
        //son nOut: gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList] : ref R(realR ohm) 
        nIn = CAL_REF_RAWR;   //son220407 3 기준 값
        nOut = CAL_REF_R;     //son220407 2
    }

    int nRCount=(int)gdCal1[part][mode][0][0];// Cal  list sum

//son230919 begin;
    if (Rvalue < 0)
        Rvalue = 0;

    if( ConMode== RAW_TO_REAL && mode >= MODE_SHORT_START) //son 내림차순 정렬된 ADC raw 값이 기준값
    {
        if (Rvalue > 65535) //son ADC 값은 65535 보다 클 수 없으므로 예외 처리
            Rvalue = 0;
        
        double dMinLookupVal = gdCal1[part][mode][nIn][nRCount] / 2; //son Min 값의 1/2 까지는 근사값으로 처리
        if (Rvalue < dMinLookupVal) //son Min 값의 1/2 보다 작으면 에러처리 
        {
            //str.Format("CalConvert(Mode:%d, ConMode:RAW_TO_REAL) Rvalue(%.2f) is Lower than Limit(%.2f)\n", 
            //                       mode, Rvalue, dMinLookupVal);
            //str2 += str;
            //str.Format("Set Rvalue to 0. Check the SETUP\\CAL%dB.ini file. \n", mode);  
            //str2 += str;
            //errMessageBox(7221, str2);
            Rvalue = 0;
        }

    }
    else
    {
        double dMaxLookupVal = gdCal1[part][mode][nIn][nRCount] * 2 ; //son Max 값의 2배까지는 근사값 처리를 한다. 
        if (Rvalue > dMaxLookupVal) //son Max 값의 2배를 넘으면 에러처리.
        {
            //str.Format("CalConvert(Mode:%d, ConMode:%s) Rvalue(%.2f) is over the Limit MaxVal* 2 (%.2f)\n", 
            //        mode, (ConMode == REAL_TO_RAW) ? "REAL_TO_RAW": (ConMode == RAW_TO_REAL) ? "RAW_TO_REAL": "UNDEF",
            //        Rvalue, dMaxLookupVal);
            //str2 += str;
            //str.Format("Set Rvalue to 0. Check the SETUP\\CAL%dB.ini file. \n", mode);  
            //str2 += str;
            //errMessageBox(7221, str2);
            Rvalue = 0;
        }
    }
//son230919 end

    Sub71.R1=Rvalue; //input2



    if( (Sub71.R1>0) && (nRCount>0) && (nRCount<= MAX_CAL_RLIST) )   //son220407 MAX_CAL_RLIST:128
    {

        for( k=1;  k<=nRCount;  k++){
            // ConMode RAW_TO_REAL이고  SHORT의 경우, CALR%d.ini 파일이 rawR이 내림차순으로 정렬되어 있으므로 첫번째 작은 값이 근사값이다.
            if( ConMode== RAW_TO_REAL && mode >= MODE_SHORT_START) {
                if( Sub71.R1> gdCal1[part][mode][nIn][k]){    
                    break;      
                }
            }                                               
            //son ConMode: Open이라면 오름차순 정렬된 cal table rawR 중에 입력된 rawR보다  첫번째 큰 값을 찾는다(근사값) 
            //son ConMode: REAL_TO_RAW는 Short, Open 오름차순 정렬된  cal table realR 중에  입력된 realR보다 첫번째 큰값이 근사값이다. 
            else {
                if( Sub71.R1< gdCal1[part][mode][nIn][k]){    
                    break;  
                }

            }
        }

        //son k-1 이 현재 찾는 위치이다.

        if( k==1)  //son 첫번째 항목 
        {
            if((gdCal1[part][mode][nIn][2] - gdCal1[part][mode][nIn][1])!=0)    //son devide by zero 예방
            {
                //son230919 Sub71.R1= (Sub71.R1/gdCal1[part][mode][nIn][1])  *  gdCal1[part][mode][nOut][1]; 
                //son230919 begin
                //son 첫번째 1번 항목일 때에는 1번과 2번 의 기울기를 연장해서 따르도록 한다.
                aa= (Sub71.R1-gdCal1[part][mode][nIn][1]) / (gdCal1[part][mode][nIn][2] - gdCal1[part][mode][nIn][1]);  
                ab= gdCal1[part][mode][nOut][2] - gdCal1[part][mode][nOut][1]; 
                Sub71.R1= ab * aa + gdCal1[part][mode][nOut][1];                                             
                if (Sub71.R1 < 0)   //son 2번 항목의 기울기로 1번을 계산하기 때문에 0보다 작게 나올 수도 있음. 
                    Sub71.R1 = 0;   //    0보다 작게 나오면 강제로 0으로 설정한다. 
                //son230919 end
                
            }
            else{ //error
                //son230919 begin: 명확한 에러처리로 수정.  
                //    if( Sub71.R1< gdCal1[part][mode][nIn][k])로 break 했기 때문에 여기로 오는 것은 버그가 있는 경우임. 
                str.Format("Divide by Zero. \n (gdCal1[part][%d][%d][2]:%.2f -gdCal1[part][%d][%d][1]:%.2f) is 0! \n Mode=%d %s Converting Error !\n\n" , 
                                        mode, nIn, gdCal1[part][mode][nIn][2], 
                                        mode, nIn, gdCal1[part][mode][nIn][1], mode ,  
                                        (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R");   //son220412_3  //son230919
                errMessageBox(7221, str);
                //son230919 end
            }
        }
/* //son230919 마지막 항목도 똑같은 기울기 계산이므로 이 부분은 삭제함. 
        else if( k==nRCount)    //son 마지막 항목
        {
            if(gdCal1[part][mode][nIn][nRCount]!=0){                                                    
                Sub71.R1= ((Sub71.R1-gdCal1[part][mode][nIn][nRCount-1])/(gdCal1[part][mode][nIn][nRCount]-gdCal1[part][mode][nIn][nRCount-1])) 
                    *   (gdCal1[part][mode][nOut][nRCount]-gdCal1[part][mode][nOut][nRCount-1])+gdCal1[part][mode][nOut][nRCount-1]; //sylee20120627
            }
            else{
                str.Format("Error No 7202 ,   Divide by Zero. \n  gdCal1[part][mode][nIn][nRCount]   \n  Mode=%d  OnInit  Real R -> Raw R Converting Error   !       \n\n" ,mode  ); 
                AfxMessageBox(str);
            }
        }
*/
        //-----------------------
        //son 기울기 계산
        //-----------------------
        else
        { 
            //son230919 마지막 항목의 +근사값 처리기능.  
            if (k == (nRCount+1))
                k = k-1;     //son 마지막 항목의 근사값(k+1)도 2배까지는  마지막 항목(k)의 기울기로 계산하기로 한다.

            //son k-1과 k의 테이블 값이 다르다면 기울기 계산
            if( (gdCal1[part][mode][nIn][k]-gdCal1[part][mode][nIn][k-1])!=0.0) //son devide by zero 예방
            {
                aa=(Sub71.R1-gdCal1[part][mode][nIn][k-1])/(gdCal1[part][mode][nIn][k]-gdCal1[part][mode][nIn][k-1]);  // [3]= lookup table Calibration Raw R
                ab=gdCal1[part][mode][nOut][k]-gdCal1[part][mode][nOut][k-1]; 
                Sub71.R1=ab*aa+gdCal1[part][mode][nOut][k-1];                                             
            }
            else{ //error
                //son k-1과 k의 테이블 값이 동일해서 변경이 없다면 그냥 비율 계산
                //      Sub71.R1은 처음에는 rawR(ADC)이고 변환후에는 realR(ohm) 이다.
                //      realR/ ref realR 의 비율은 rawR/ ref rawR 비율과 같다고 가정하면
                //      realR = (rawR / refRawR) * refRealR
                //son230919 Sub71.R1=(Sub71.R1/gdCal1[part][mode][nIn][k-1])*gdCal1[part][mode][nOut][k-1]; 
                
                //son230919 begin: 명확한 에러처리로 수정.  
                //    if( Sub71.R1< gdCal1[part][mode][nIn][k])로 break 했기 때문에 (<= 아님)  여기로 오는 것은 버그가 있는 경우임. 
                str.Format("Divide by Zero. \n (gdCal1[part][%d][%d][%d]:%.2f -gdCal1[part][%d][%d][%d]:%.2f) is 0! \n Mode=%d %s Converting Error !\n\n" , 
                                            mode, nIn, k, gdCal1[part][mode][nIn][k], 
                                            mode, nIn, k-1, gdCal1[part][mode][nIn][k-1],  mode ,  
                                            (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R");   //son220412_3
                errMessageBox(7221, str);
                //son230919 end
            }
        }
    }

    return Sub71.R1;

} 
#endif //son230925 

#endif  //son221014 end

