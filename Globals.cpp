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
// ���� ���� ����
////////////////////

int nView1;		//son220215 moved from gobal.h


sFILE_DATA		g_sFile;
WORD			g_wPrintState = 0x0007;		// 0x01, PRT_BASIC    on 
                                            // 0x07, PRT_BASIC, PRT_LEVEL1, PRT_LEVEL2 on


//son Multi DUT 4W Net Data   (Grid Display��, R���� ������ d4W_Log1[][], d4W_Lo2[][]�� �����Ѵ�.)
//son190704-4WDUT16: gd4W1[][][]�� g_s4WNet ����ü�� ���� 	
C4WNet	g_s4WNet[MAX_DUT];			
  
//son190801 gd4W1[][][] ����. 	g_s4WNet[MAX_DUT]��  ��ü �Ϸ�
//float	gd4W1[MAX_DUT][MAX_4W_NET +1][15];//SYLEE20120116  //4W NETLIST  //sylee170728-1
//son   1. LoadSaveSub17()���� BDL��ġ\4W.ini  ���� gd4W1[MAX_DUT][MAX_4W_NET +1][15]�� write/read �Ѵ�.
//son   2. LoadSaveSub17()���� BDL��ġ\4W.ini  ���� gd4W1[MAX_DUT][MAX_4W_NET +1][15]�� write/read �Ѵ�.
//son	gd4W1[dut][0][0]  :  4W Net ���� 
//son   gd4W1[dut][1~5][0~14]:  �ǹ̾���.
//son   gd4W1[dut][6][1~12]:  4W.ini�� ù��° ���� ������ 12 �׸���  ����.
//son   gd4W1[dut][5+i][0~11]:  gd4W1[0][0] ������ŭ i�� 1���� �������� ���鼭 4W.ini�� �����͸� �����Ѵ�.
//								i�� 1���� �̹Ƿ�   gd4W1[dut][6]���� ������.
//
//                 0     1      2      3      4       5        6            7       8         9           10       11          ....  14
//				   No	Pin1   Pin2   Pin3   Pin4   Set.Min   Set.Avg   Set.Max    CalMode    Test.Min   Test.Avg  Test.Max         nCurrMode
//	ex)			   1     1      2      385    386    1.060   1.120        1.150    171       1.060       1.120     1.150
//	 ==> MAP->4W�� Grid ȭ��� ��ġ�Ѵ�.
 





#ifndef __OPVECTOR__

//son  nBDLData31[][][], nBDLData2Op[][][]��  ChilcView1.cpp-> Globals.cpp�� �̵�.  
//     extern����� �߰��� Class�� definition�� Globals.h�� ������.

//son200420-4WNET10000		
//short nBDLData2Op[DefDutMax][DEF_MAX_PIECE1][dSetPinTot2][3];//open data    //sylee150604-kushan   //sylee150917-2  16400  33000
short nBDLData2Op[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2][3];//open data    //son220824_2 DefDutMax(33) -> MAX_DUT1(33)  //son220823_6
		//son190705
		//piece 1, piecePinIdx 1���� �Ҵ���
		//son	nBDLData2Op[nDut][piece][0][0]++;								: piece�� open test ��� pin ����
		//son	nBDLData2Op[nDut][piece][piecePinIdx][1]=nBDLData2[i][1];		: ��ǥ��
		//son	nBDLData2Op[nDut][piece][piecePinIdx][2]=nBDLData2[i][2];		: PinNo

//son200420-4WNET10000		
//son nBDLData3�� piece ���� ������ ���ġ�� Short data
//short nBDLData31[DefDutMax][DEF_MAX_PIECE1][dSetPinTot2];//short data    //sylee150604-kushan
short nBDLData31[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2];//short data    //son220824_2 DefDutMax(33) -> MAX_DUT1(33) //son220823_6
		//son nBDLData31[nDut][piece][0]                : piece�� Short pin �Ѽ�
		//son nBDLData31[nDut][piece][pieceShortPinIdx] : piece�� Short pinNo
#endif

#ifdef __OPVECTOR__
using namespace std;

//son200420-4WNET10000		
int     g_nOpenTestCount = 0;       //son Total OpenTest ����
vector <COpenTestItem>	*g_pvBDLData2Op[MAX_DUT+1][MAX_PIECE+1];  // �����Ҵ� vector
            //son 1. nBDLData2Op[nDut][piece][0][0]    => (g_pvBDLData2Op[nDut][nPiece]->size() -1)�� ��ü
            //    2. nBDLData2Op[nDut][piece][nPin][1] => (*g_pvBDLData2Op[nDut][nPiece])[nPin].wFirstPin���� ��ü
            //    3. nBDLData2Op[nDut][piece][nPin][2] => (*g_pvBDLData2Op[nDut][nPiece])[nPin].wPin�� ��ü


//son200420-4WNET10000		
int      g_nShortTestCount = 0;     //son Total ShortTest ���� 
vector <CShortTestItem>	*g_pvBDLData31[MAX_DUT+1][MAX_PIECE+1];        // �����Ҵ� vector
            //son 1. nBDLData31[nDut][piece][0]    => (g_pvBDLData31[nDut][nPiece]->size() -1) �� ��ü
            //    2. nBDLData31[nDut][piece][nPin] => (*g_pvBDLData31[nDut][nPiece])[nPin].wFirstPin���� ��ü

extern  CSysInfoPieceRange  SysInfoPieceRange;
extern  int g_nPieceMax1;
#endif



extern CModeSub71  Sub71;       //son220405



////////////////////
// ����  �Լ� ����
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
//    nBDLData2Op[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2][3] �� 
//    vector <COpenTestItem> g_pvBDLData2Op[MAX_DUT][MAX_PIECE] �� ����
//    [dSetPinTot2][3] array�� COpenTestItem �� vector�� �ٲ��.   (�����Ҵ��� ����)
//    [3] array�� COpenTestItem ������ member ���� wFirstPin, wPin���� ��ü�ȴ�.
//    
//    ����Ҷ����� (*g_pvBDLData2Op[nDut][nPiece])[nPin].wFirstPin �� ���� ����ϸ� �ȴ�.
//---------------------------------------------------------------------------------------------

//son OpenTest Data�� �޸𸮿� �߰� �Ѵ�.   //son200420-4WNET10000
int InsertOpenTest(int nDut, int nPiece, COpenTestItem openTestItem)
{
#ifdef __OPVECTOR__
    CString strTemp;

	// g_nOpenTestCount��  MAX_OPEN_TEST ������ �Ѿ�� OpenTest �߰��� ����
	if (g_nOpenTestCount >= (MAX_OPEN_TEST + 1))    // 1�� vector���� ����ϹǷ� ������1 �߰�
		return -1;

	// g_pvBDLData2Op[nDut][nPiece]�� ���ٸ� ���� new�ؼ� �����Ѵ�. 
	// �����ִٸ� -1����.  �̹� �����Ѵٸ� �׳� 0����.  
	// -1�� ���ϵ� ��츸 �ƴ϶�� g_pvBDLData2Op[nDut][nPiece] vector�� ����� �� �ִ�.
	int ret = InsertOpenTest_Vector(nDut, nPiece);
	if (ret < 0)
		return -1;

	// vector 1���� size�� 32768�� ���� �� ����.
	if (g_pvBDLData2Op[nDut][nPiece]->size() > (MAX_PIN + 1))    
	{
		strTemp.Format("g_pvBDLData2Op[%d][%d]->size()=%d, capacity()=%d, g_nOpenTestCount=%d\n Can't Insert %d over OpenTestItem",
					nDut, nPiece, g_pvBDLData2Op[nDut][nPiece]->size(), 
					g_pvBDLData2Op[nDut][nPiece]->capacity(), g_nOpenTestCount, MAX_PIN);
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
		return -1;
	}

	// vector�� item ������ 2048���� �Ѵ´ٸ� vector ������ �� Ȯ���Ѵ�.  
	// 2048�� �Ǵ� ������ 1ȸ������ �����ϸ�, vector resize ���߿� "�޸� ����" �޽��� ������ ������ ���� �� �ִ�.
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



	// �ش� vector�� ���� openTestItem�� insert�Ѵ�.
	//   :  time ������ ���� vector�� �̹� time�� openTestItem���� push_back �Ѵ�. 
	//   openTestItem�� value�� ���ڷ� �ѱ�� �������� copy�ؾ� �ϹǷ� COpenTestItem Ŭ������ 
	//   copy constructor(���������) ȣ���� �Ͼ��.  �����͸� �ѱ�� �����, ���� �����ھȿ���
	//   �����Ϳ� ���� ���� new�� �ؼ� ������ ����� ��� ���߿� delete[]�Ҷ��� ������ �� ����. (deep copy..)
	g_pvBDLData2Op[nDut][nPiece]->push_back(openTestItem);
	g_nOpenTestCount++;


	// g_nOpenTestCount��  MAX_OPEN_TEST ������ �Ѿ�� OpenTest �߰��� ����
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

//son  g_pvBDLData2Op[nDut][nPiece]�� ���ٸ� ���� new�ؼ� �����Ѵ�.     //son200420-4WNET10000
//     �̹� �����Ѵٸ� �׳� 0����, �����ִٸ� -1����
int InsertOpenTest_Vector(int nDut, int nPiece)
{
#ifdef __OPVECTOR__
	CString strTemp;
	if ((nDut <= 0 || nDut > MAX_DUT)                   //son nDUB, nPiece ��� 1���� ����
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
	
	// �ش� Dut, Piece ��ġ�� data�� insert�� ���� ���ٸ�  vector�� ���� �����ؼ� assign�Ѵ�.
	//  : Dut * Piece ������ vector �����Ϳ� ���� vector�� assign
	if (g_pvBDLData2Op[nDut][nPiece] == NULL)
	{
		vector <COpenTestItem>*  pvOpenTest = new vector <COpenTestItem>;
		if (pvOpenTest == NULL)		// �޸��Ҵ� ����!
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

// ��� OpenTest data�� �����ϰ� �Ҵ�� memory�� delete�Ѵ�.    //son200420-4WNET10000
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
                //int prevTupleSize = g_pvBDLData2Op[dut][piece]->size();	// piece���� tupleSize�� �ٸ�!

                // vector�� ��� data������ (��� tuple�� �����ϰ�)
                if (g_pvBDLData2Op[dut][piece]->empty() == false)
                    g_pvBDLData2Op[dut][piece]->clear();		


                delete g_pvBDLData2Op[dut][piece];		// �����Ҵ�� vcetor�� ��ȯ�Ѵ�.
                g_pvBDLData2Op[dut][piece] = NULL;		// ��ȯ�� vector �����͵� NULL ó��

                //MyTrace(PRT_BASIC, "DeleteAllOpenTestItem(): dut=%d, piece=%d\n\n\n", dut, piece);
            }
        }
    }

    g_nOpenTestCount = 0;

    //__PrintMemSize(FUNC(DeleteAllOpenTestItem), __LINE__);       //son200420-4WNET10000
#endif
}


//son ShortTest Data�� *g_pvBDLData31[MAX_DUT+1][MAX_PIECE+1] vector �޸𸮿� �߰� �Ѵ�.   //son200420-4WNET10000
int InsertShortTest(int nDut, int nPiece, CShortTestItem shortTestItem)
{
#ifdef __OPVECTOR__
    CString strTemp;

	// g_nShortTestCount��  MAX_SHORT_TEST ������ �Ѿ�� ShortTest �߰��� ����
	if (g_nShortTestCount >= (MAX_SHORT_TEST + 1))    // 1�� vector���� ����ϹǷ� ������1 �߰�
		return -1;

	// g_pvBDLData31[nDut][nPiece]�� ���ٸ� ���� new�ؼ� �����Ѵ�. 
	// �����ִٸ� -1����.  �̹� �����Ѵٸ� �׳� 0����.  
	// -1�� ���ϵ� ��츸 �ƴ϶�� g_pvBDLData31[nDut][nPiece] vector�� ����� �� �ִ�.
	int ret = InsertShortTest_Vector(nDut, nPiece);
	if (ret < 0)
		return -1;

	// vector 1���� size�� 32768�� ���� �� ����.
	if (g_pvBDLData31[nDut][nPiece]->size() > (MAX_PIN + 1))    
	{
		strTemp.Format("g_pvBDLData31[%d][%d]->size()=%d, capacity()=%d, g_nShortTestCount=%d\n Can't Insert %d over ShortTestItem",
					nDut, nPiece, g_pvBDLData31[nDut][nPiece]->size(), 
					g_pvBDLData31[nDut][nPiece]->capacity(), g_nShortTestCount, MAX_PIN);
		AfxMessageBox(strTemp, MB_ICONINFORMATION);
		MyTrace(PRT_BASIC, strTemp);
		return -1;
	}

	// vector�� item ������ 2048���� �Ѵ´ٸ� vector ������ �� Ȯ���Ѵ�.  
	// 2048�� �Ǵ� ������ 1ȸ������ �����ϸ�, vector resize ���߿� "�޸� ����" �޽��� ������ ������ ���� �� �ִ�.
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


	// �ش� vector�� ���� shortTestItem�� insert�Ѵ�.
	//   :  time ������ ���� vector�� �̹� time�� shortTestItem���� push_back �Ѵ�. 
	g_pvBDLData31[nDut][nPiece]->push_back(shortTestItem);
	g_nShortTestCount++;


	// g_nShortTestCount��  MAX_SHORT_TEST ������ �Ѿ�� ShortTest �߰��� ����
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

//son  g_pvBDLData31[nDut][nPiece]�� ���ٸ� ���� new�ؼ� �����Ѵ�.     //son200420-4WNET10000
//     �̹� �����Ѵٸ� �׳� 0����, �����ִٸ� -1����
int InsertShortTest_Vector(int nDut, int nPiece)
{
#ifdef __OPVECTOR__
	CString strTemp;

	//son nDut, nPiece ��� 1���� ����
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
	
	// �ش� Dut, Piece ��ġ�� data�� insert�� ���� ���ٸ�  vector�� ���� �����ؼ� assign�Ѵ�.
	//  : Dut * Piece ������ vector �����Ϳ� ���� vector�� assign
	if (g_pvBDLData31[nDut][nPiece] == NULL)
	{
		vector <CShortTestItem>*  pvShortTest = new vector <CShortTestItem>;
		if (pvShortTest == NULL)		// �޸��Ҵ� ����!
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

//son �ش� DUT, Piece�� ShortTest data�� �����ϰ� �Ҵ�� memory�� delete�Ѵ�.    //son200722-BDL-Sort
void ZeroPieceShortTestItem(int nDut, int nPiece)
{
#ifdef __OPVECTOR__
	int prevTupleSize = 0;

    //__PrintMemSize(FUNC(DeletePieceShortTestItem), __LINE__);       //son200420-4WNET10000
	
    if(g_pvBDLData31[nDut][nPiece] != NULL)
    {
        prevTupleSize = g_pvBDLData31[nDut][nPiece]->size() ;	// nPiece���� tupleSize�� �ٸ�!

        // vector�� ��� data������ (��� tuple�� �����ϰ�)
        if (g_pvBDLData31[nDut][nPiece]->empty() == false)
            g_pvBDLData31[nDut][nPiece]->clear();		

        //son  Zeroȭ ��Ű�� ���̹Ƿ� Vector delete�� ������ �ʴ´�. (�ڸ�Ʈó��)
        //delete g_pvBDLData31[nDut][nPiece];		// �����Ҵ�� vcetor�� ��ȯ�Ѵ�.
        //g_pvBDLData31[nDut][nPiece] = NULL;		// ��ȯ�� vector �����͵� NULL ó��

        // ������ ���� dut�� ShortTest ī��Ʈ��ŭ ��ü ShortTest ī��Ʈ�� �ٿ� �ش�.
        g_nShortTestCount -= prevTupleSize;

        MyTrace(PRT_BASIC, "ZeroPieceShortTestItem(): nDut=%d, nPiece=%d, g_nShortTestCount=%d \n\n", 
                                                    nDut, nPiece, g_nShortTestCount);
    }


    //__PrintMemSize(FUNC(DeletePieceShortTestItem), __LINE__);       //son200420-4WNET10000
#endif
}

//son �ش� DUT�� ShortTest data�� �����ϰ� �Ҵ�� memory�� delete�Ѵ�.    //son200420-4WNET10000
void DeleteDutShortTestItem(int nDut)
{
#ifdef __OPVECTOR__
	int prevTupleSize = 0;

    //__PrintMemSize(FUNC(DeleteDutShortTestItem), __LINE__);       //son200420-4WNET10000
	
    for (int piece =1; piece <= MAX_PIECE; piece++)
    {
        if(g_pvBDLData31[nDut][piece] != NULL)
        {
            prevTupleSize = g_pvBDLData31[nDut][piece]->size() ;	// piece���� tupleSize�� �ٸ�!

            // vector�� ��� data������ (��� tuple�� �����ϰ�)
            if (g_pvBDLData31[nDut][piece]->empty() == false)
                g_pvBDLData31[nDut][piece]->clear();		


            delete g_pvBDLData31[nDut][piece];		// �����Ҵ�� vcetor�� ��ȯ�Ѵ�.
            g_pvBDLData31[nDut][piece] = NULL;		// ��ȯ�� vector �����͵� NULL ó��

            // ������ ���� dut�� ShortTest ī��Ʈ��ŭ ��ü ShortTest ī��Ʈ�� �ٿ� �ش�.
            g_nShortTestCount -= prevTupleSize;

            //MyTrace(PRT_BASIC, "DeleteDutShortTestItem(): nDut=%d, piece=%d, g_nShortTestCount=%d \n\n", 
            //                                            nDut, piece, g_nShortTestCount);
        }
    }


    //__PrintMemSize(FUNC(DeleteDutShortTestItem), __LINE__);       //son200420-4WNET10000
#endif
}


// ��� ShortTest data�� �����ϰ� �Ҵ�� memory�� delete�Ѵ�.    //son200420-4WNET10000
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



//son 1���� �����ϴ� dutId�� �����Ѵ�.
int	getDutId()
{

	if ((nMulNetFlag > 1) && (nMulNetFlag <= nMultyNetTotal))
		return nMulNetFlag;

	return 1;

}


//son-4WDUT-20200312:
//son 0���� �����ϴ� g_s4WNet�� dutIdx�� �����Ѵ�.  dutIdx = dutId -1
int	get4wDutIdx()
{

#ifdef __4W_DUT1__
    //son-4WDUT-20200312 : 4W Multi DUT ������ ������  ������ 0���� �����ϴ� �ڵ� �߰���.
    //          DUT �ϳ����̶� �ǵ���.  �ӽ� �ڵ� . multi DUT �ذ��� �Ǹ� ���󺹱��ؾ� �Ѵ�. 
    //          ������ �ذ� �Ǹ� ���󺹱� �ؾ���!!!   __4W_DUT1__�� ������ �ɼǿ��� ���� �������ϸ� �ȴ�.
    
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
	// �Է°� ������ ����
	// 1->7.5~8.5u  2->9u 4->11u 5->12 7->13u 10->17u
	// 100->106u 200->206u 500->506u

	//static LONGLONG FreqQuadPart=0;
	static double FreqQuadPart=0;
	static LARGE_INTEGER Freq, ZeroCnt, CurCnt;

	// ���ػ� Ÿ�̸��� 1�ʴ� ������ ȹ��
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
	case usec:		// 1us(micro sec) �� �������� ��ȯ
		FreqQuadPart = (double)Freq.QuadPart/1000000.0;
		break;
	}
	
	//---------------------------------------------------
	// ex) delaytime=200, unit=usec(2) ���
	// 	    200 micro sec ���� DoEvents()�� �ݺ��Ͽ� �����Ѵ�.
	

#if 0		
	// Elapsed time�� ǥ���ϱ�
	__int64	startTime, endTime;
	startTime = GetMicroSecond();

	QueryPerformanceCounter(&ZeroCnt);	// ���۽����� counter �� ȹ��
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()�� ������ ������ counter �� ȹ��
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	endTime = GetMicroSecond();

	MyTrace(PRT_LEVEL1, "			Delay(%d %s), Elapsed time=%d(usec)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?",  (endTime - startTime));

#else
	QueryPerformanceCounter(&ZeroCnt);	// ���۽����� counter �� ȹ��
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()�� ������ ������ counter �� ȹ��
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

//son210420 ChildView7.cpp���� Globals.cpp �� �̵�
BOOL FileExists(CString strFilePath)
{
	CFileFind aFile;
	BOOL IsExist = aFile.FindFile(strFilePath);
	aFile.Close();
	return IsExist;
}


//son211222 ChildView7.cpp���� Globals.cpp �� �̵�. FileExists()�� �ߺ��Ǿ� ����
/*BOOL CheckFileExist(char * pn)      //son210906
{
    CFileFind fn;

    if( !fn.FindFile(pn,0) ) return FALSE;
    else return TRUE;
}
*/


//son211222 ChildView7.cpp���� Globals.cpp �� �̵�
BOOL MakeNewDir(CString DirPath)
{
    int ret;
    ret = _mkdir((char*)(LPCTSTR(DirPath)));

    if(ret == -1)   return FALSE;
    else            return TRUE;
}



//son211222 ChildView7.cpp���� Globals.cpp �� �̵�
//son240110 Sub ������ �ִٸ� false�� ���ϵǴ� ������ ����. 
//          sub ������ �ִ� ���� üũ�� PathIsDirectory(buf)�� �������. 
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

            if( !fn.IsDots() )  //son240110 Sub ������ �ִٸ� false�� ���ϵȴ�.
                return FALSE;
        }
        if(!nextfile) 
            break;
        nextfile = fn.FindNextFile();
    }
    return TRUE;
}


//son211222 ChildView7.cpp���� Globals.cpp �� �̵�
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
// 2018.06.18:  psapi.h �� psapi.lib�� ���Ե� VisualC++ 6.0 SDK�� ��� �Ʒ� �ڵ�� ��� �Ұ�.
// VC2008���� ī���ؿ��� ����� �ִٰ�� ��.
// https://stackoverflow.com/questions/4560468/compile-an-exe-in-microsoft-visual-c-6-0-using-the-psapi-h
/*
// ���� �� process�� ��밡���� �޸��� ����� �����´�.
SIZE_T GetProcessWorkingSetSize() 
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		return 0;
	}
	return pmc.WorkingSetSize;
}

//memory usage ����
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

//2017.11.06  TRACE�� buffer�� 512���� �ۿ� ������� �ʾƼ� �Ʒ��� ���� �ű� �Լ� ����
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



	// log ���Ͽ� printout �غ� -------
	g_cs.Lock();		// CriticalSection ����

	char date_time[24]; 
	char path[FILE_PATH_SIZE];      //son210420 FILE_PATH_SIZE: 255
	memset(date_time,0x00,sizeof(date_time));
	memset(path,0x00,sizeof(path));
	GetDateAndTime(date_time);
	CTime t = CTime::GetCurrentTime();

	
	//son220419_8 Current ACE500\LOG\log_20111122_12 -> d:\log\log_20111122�� ������ name�� ���� (sylee ��û)
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
	// OUTPUT ����� â�� print out----------------
	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << szBuffer;
#endif

#if 1
	// log ���Ͽ� print out ----------
	CFile f;
	f.Open( path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate );
	f.SeekToEnd();	



	// time ��� print --------
	t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format( "[%02d:%02d:%02d] ", t.GetHour(), t.GetMinute(), t.GetSecond());
	//strTime.Format( "%02d:%02d:%02d (%12lu) ", t.GetHour(), t.GetMinute(), t.GetSecond(),  GetProcessWorkingSetSize() );

	f.Write( strTime, strTime.GetLength() );


	// data print --------
	CString strTemp = szBuffer;
	strTemp.Replace("\n", "\r\n");		// \n�� \r\n���� ����.
	f.Write( strTemp, strlen(strTemp) );

	f.Close();
#endif

#if 0
	// log ���Ͽ� print out ----------
	CFile f;
	CArchive ar(&f, CArchive::store);
	f.Open( path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	f.SeekToEnd(); 	// ���� ���ϳ����� �ڿ� �ٿ� ���� ���� �̵�	

	CString strTime;
	t = CTime::GetCurrentTime();
	strTime.Format( "%02d:%02d:%02d ", t.GetHour(), t.GetMinute(), t.GetSecond() );

	ar.WriteString(strTime);

	CString strTemp = szBuffer;
	strTemp.Replace("\n", "\r\n");		// \n�� \r\n���� ����.
	f.Write( strTemp, strlen(strTemp) );

	ar.WriteString(strTemp);

	ar.Close();
	f.Close();
#endif

	g_cs.Unlock();		// CriticalSection ����
}

//son ������ SW�� Event Log �߿��� 2�� ���� ������ ��� �����Ѵ�. 
//son ex) CleanUp_MyTraceLog("d:\\log"); 
//        CleanUp_MyTraceLog("c:\\ACE500\log"); 
void CleanUp_MyTraceLog(char *pn)    //son220510 �߰� 
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

        //son "log_20220315_08.txt" �̷� ������ ������ ã�´�.
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

        //son ���� 2022���̸� 2020��� 2020�� ������ ������ �����. 
        if (fileYear < (curTime.GetYear() -1)) 
        {
            MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
            DeleteFile(filepath);
            continue;
        }

        //son fileYear�� ���� ������ ��� (���� 2022���̸� 2021���� ���)
        if (fileYear < curTime.GetYear()) 
        {
            //son fileYear�� ���� �����ε� ���� 1���̸�,  �����⵵ 1��~ 11���� ������ �����. (�۳� 12��, ���� 1�� ����)
            if( curTime.GetMonth() == 1  ) 
            {                
                if(fileMonth != 12) 
                {
                    MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
                    DeleteFile(filepath);
                    continue;
                }

            }
            //son fileYear�� ���������ε�  ���� 1���� �ƴ϶�� �ƴ϶��  ����������  ��� ������ �����.
            else
            {
                MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
                DeleteFile(filepath);
                continue;
            }
        }

        //son ���� 5���̸� 3�� �����ؼ�  3�� ������ ���ϵ��� ��� �����. (1,2,3 �� ����)
        if (fileMonth <= (curTime.GetMonth() -2) ) 
        {
            MyTrace(PRT_BASIC,"Delete %s\\%s\n", pn, dataFileName);
            DeleteFile(filepath);
            continue;
        }

    }

}

// hdpark231121 Spark log ����
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


//son err �޽����ڽ� ���. 
//    err �ڵ带 �����ؼ� ����׿� 'Output'ȭ�� Trace�� d:\log �������, �޽����ڽ��� �Բ� ����Ѵ�.
void errMessageBox(int nError, LPCTSTR errStr)  //son230217_2
{
    CString str;

    str.Format("Error No %d!\n\n%s", nError, errStr);
    AfxMessageBox(str, MB_OK);
    MyTrace(PRT_BASIC, str);
}


//son221215 begin: d ����̺� �뷮 üũ ��� �߰�. 
void Check_DDrive_FreeSpace()    //son221215 �߰� 
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

    if (iFree < 100000) //son 100Mbyte ������ �뷮�̸� ��� ���.
    {
        str.Format("Warning No 1900. \n\n %s Drive has insufficient disk space under 100 Mbytes.  \n\nFree byte is %.3f Mbytes.\nPlease clean up the Disk.\n",
                                        strDrive, iFree /(double)1000);
        MyTrace(PRT_BASIC, str);
        AfxMessageBox(str, MB_OK);
    }

}

//son221215 end


#if 0
//2018.02.14  ���ɰ����� ���� print off�̸� snprintf�� �������� �ʴ� ��� �߰�.
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

// MyTrace�� �ٸ� ��. : pStr �����͸� �Է¹޾Ƽ� ȣ�����Ŀ� ����� str�� ������ �� �ִ�.
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

	//char* pStr = str.GetBuffer(2048);	// �̷��� �ϴϱ� �״´�.. 
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
// ConMode=1: RAW_TO_REAL (raw -> real)   //son AD Raw ���� real ohm R������ convert     
// ConMode=2: REAL_TO_RAW (real-> raw)    //son real ohm R ���� AD Raw������ convert
// part 0:PART_UPPER, 1:PART_LOWER,  2:PART_MIXED

//son Rvalue�� Real->Raw�� �ٲٰų� Raw->Real�� �ٲپ Sub71.R1�� ����. (2W��)
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

    int nRCount=(int)gdCal1[part][m_nMode1][0][0];// Cal  list ����

//son230925 begin: RCalCovert() Range üũ ��� ����
    //----------------------
    //son Range Check
    //----------------------
    if (Rvalue < 0)
    {
        Rvalue = -1;
        Sub71.R1=Rvalue; //input2
        return Sub71.R1;
    }

    //son RAW_TO_REAL SHORT : �������� ���ĵ� ADC raw ���� ���ذ� (nIn)
    if (ConMode== RAW_TO_REAL && m_nMode1 >= MODE_SHORT_START) 
    {
        if (Rvalue > 65535) //son Short�� ADC ���� I���̾  65535 ���� Ŭ �� �����Ƿ� ���� ó��
        {                   //son OPEN�� ADC ���� VC/I ���̾ 65535 ���� Ŭ �� �ִ�. 
            Rvalue = -1; 
            Sub71.R1=Rvalue; //input2
            return Sub71.R1;
        }
        
        //son230925 min �� 1/2 ���� �� �۴��� ������ ����� ó���ϵ��� �Ѵ�.
        //son230925 double dMinLookupVal = gdCal1[part][m_nMode1][nIn][nRCount] / 2; //son Min ���� 1/2 ������ �ٻ簪���� ó��
        //son230925 if (Rvalue < dMinLookupVal) //son Min ���� 1/2 ���� ������ ����ó�� 
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
    //son ���ذ��� �������� ���ĵ� ���̽�
    //    1) REAL_TO_RAW OPEN 
    //    2) REAL_TO_RAW SHORT, 
    //    3) RAW_TO_REAL OPEN,  
    else    
    {


 //sylee231130 ��ǳ 10V (LV)��  jig away ���¿��� Open Test �ÿ� C-O �߻������� ����
 //           I ���� �̹��ؼ�  R�� 999999�� ó���� ��쿡  ���⿡�� R���� -1�� ����ǰ� 
 //           �ٽ� �ɷ����� 0.1ohm ���� ó���Ǿ��ٰ� ���������� Auto ȭ�鿡  'R < 0 ohm' ���� �߸� ��µǴ� ���� �߻�����. 
 /*
        double dMaxLookupVal = (gdCal1[part][m_nMode1][nIn][nRCount]) * 2;   //son Max ���� 2������� �ٻ簪 ó���� �Ѵ�.
        if (Rvalue > dMaxLookupVal) //son Max ���� 2�踦 ������ ����ó��.
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
        //son ��ġ ã�� 
        //----------------------
        for( k=1;  k<=nRCount;  k++){
            // ConMode RAW_TO_REAL�̰�  SHORT�� ���, CALR%d.ini ������ rawR�� ������������ ���ĵǾ� �����Ƿ� ù��° ���� ���� �ٻ簪�̴�.
            if( ( ConMode== RAW_TO_REAL)  && (m_nMode1>=MODE_SHORT_START) ){//sylee230824   //son230919 301-> 201:MODE_SHORT_START 
                if( Sub71.R1> gdCal1[part][m_nMode1][nIn][k]){//sylee230824  
                    break;  
                } 	
            }
            //son ConMode: REAL_TO_RAW�� Short, Open �������� ���ĵ�  cal table realR �߿�  �Էµ� realR���� ù��° ū���� �ٻ簪�̴�. 
            //    OPEN�� ���, CALR%d.ini ������ rawR�� ������������ ���ĵǾ� �����Ƿ� ù��° ū���� �ٻ簪�̴�.
            else{
                if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k]){	 //son ConMode 1�̸� ADC raw R
                    break;  
                } 
            }
        }

        //----------------------
        //son ���� ��� 
        //----------------------
        //son k==1 �̸� ���� ã�� ��ġ�� k-1�̴�.
        if( k==1)  //son ù��° R���� ���� ���� ��  
        {
            //sylee230823 if(gdCal1[part][m_nMode1][nOut][0]<=0.0){ //son ConMode 1�̸� ohm real R
            if(gdCal1[part][m_nMode1][nOut][0]<0.0){  //sylee230823-1
                Sub71.R1= -1.0 ;  
            }
            else
            {
                if((gdCal1[part][m_nMode1][nIn][2] - gdCal1[part][m_nMode1][nIn][1])!=0) //son devide be zero ����
                {
                    //son230919 Sub71.R1= (Sub71.R1/gdCal1[part][m_nMode1][nIn][1])  *  gdCal1[part][m_nMode1][nOut][1]; 
                    //son230919 begin:
                    //son ù��° 1�� �׸��� ������ 1���� 2�� �� ���⸦ �����ؼ� �������� �Ѵ�.
                    aa= (Sub71.R1 - gdCal1[part][m_nMode1][nIn][1]) / (gdCal1[part][m_nMode1][nIn][2] - gdCal1[part][m_nMode1][nIn][1]); 
                    ab= gdCal1[part][m_nMode1][nOut][2] - gdCal1[part][m_nMode1][nOut][1]; 
                    Sub71.R1= ab * aa + gdCal1[part][m_nMode1][nOut][1];											 

                    if (Sub71.R1 < 0)   //son 2�� �׸��� ����� 1���� ����ϱ� ������ 0���� �۰� ���� ���� ����. 
                        Sub71.R1 = 0;   //    0���� �۰� ������ ������ 0���� �����Ѵ�. 
                    //son230910 end
                }
                else{
                    //son230919 begin: ��Ȯ�� ����ó���� ����.  
                    //    if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k])�� break �߱� ������ (<= �ƴ�)  ����� ���� ���� ���װ� �ִ� �����. 
                    str.Format("Divide by Zero. \n (gdCal1[part][%d][%d][2]:%.2f - gdCal1[part][%d][%d][1]:%.2f)  is 0! \n Mode=%d %s Converting Error !\n\n" , 
                            m_nMode1, nIn, gdCal1[part][m_nMode1][nIn][2], 
                            m_nMode1, nIn, gdCal1[part][m_nMode1][nIn][1], m_nMode1 ,  
                            (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R");   //son220412_3
                    errMessageBox(7223, str);
                    //son230919 end
                }
            } 		

        }
//son230919 �븸 TTF(MST) Leak250V Mode ������ �׸��� �� REAL_TO_RAW ��� �߸��Ǵ� ���� ���� ����
//          ������ �׸� ����� ����ϵ��� �� ���� ������ �ذ��.
/*
        //---------------
        //son ���� ���
        //---------------
        else if( k==nRCount)    //son ������ R 
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
        //son ���� ���
        //-----------------------
        //son  ConMode 1�϶�,   rawRCal[k-1] < rawR < rawRCal[k] �� ���̽� 
        else
        {
            //son230919 ������ �׸��� +�ٻ簪 ó�����.  
            if (k == (nRCount+1))
                k = k-1;     //son ������ �׸��� �ٻ簪(nRCount+1)�� 2�������  ������ �׸�(nRCount)�� ����� ����ϱ�� �Ѵ�.

            if( (gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1])!=0.0) //son devide be zero ����
            {
                //son ConMode 1 �� ���:  ADC raw R -> ohm real R �� ����
                //
                //ex)  a: rawRCal[k-1], b: realRCal[k-1],  c:rawRCal[k], d:realRCal[k],  x: rawR, y: realR 
                //     ���� : (d-b)/(c-a)    y����:  - (d-b)/(c-a)*a + b
                //     y = (d-b)/(c-a)x - (d-b)/(c-a)*a + b
                //     y = (d-b)/(c-a) * (x -a) + b     
                //     y = (x -a)/(c-a) * (d-b) + b   <==== (a,b) (c,d) 2���� ������ ���� 1�� ������
                //
                // realR = (rawR -a)/(c-a) * (d-b) + b
                // realR = (rawR - rawRCal[k-1])/(rawRCal[k] -rawRCal[k-1]) * (realRCal[k]-realRCal[k-1]) + realRCal[k-1]
                //
                //    aa = (rawR - rawRCal[k-1])/(rawRCal[k] -rawRCal[k-1]);
                //    ab = realRCal[k] - realRCal[k-1];
                // realR = aa * ab + realRCal[k-1];       <=== ���⸦ �̿��ؼ� x(rawR)�� ���� y(realR)�� ���Ѵ�.
 
                aa=(Sub71.R1-gdCal1[part][m_nMode1][nIn][k-1])/(gdCal1[part][m_nMode1][nIn][k]-gdCal1[part][m_nMode1][nIn][k-1]); 
                ab=gdCal1[part][m_nMode1][nOut][k]-gdCal1[part][m_nMode1][nOut][k-1]; 
                Sub71.R1=ab *aa +gdCal1[part][m_nMode1][nOut][k-1];											 
            }
            else{
                //son k-1�� k�� ���̺� ���� �����ؼ� ������ ���ٸ� �׳� ���� ���
                //      realR/ ref realR �� ������ rawR/ ref rawR ������ �����Ƿ�
                //      realR = (rawR / RefrawR) * RefrealR
                //son230919 Sub71.R1=(Sub71.R1/gdCal1[part][m_nMode1][nIn][k-1])*gdCal1[part][m_nMode1][nOut][k-1];	

                //son230919 begin: ��Ȯ�� ����ó���� ����.  
                //    if( Sub71.R1< gdCal1[part][m_nMode1][nIn][k])�� break �߱� ������ (<= �ƴ�)  ����� ���� ���� ���װ� �ִ� �����. 
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
    //son RAW_TO_REAL(����� ohm�� ���) Open �ּҰ��� 1 ohm
    if (ConMode==RAW_TO_REAL && m_nMode1 <= MODE_OPEN_END && Sub71.R1 < 1)    
    {
        Sub71.R1 = 1;   // 1���� �۰� ������ ������ 1 ohm���� ����.  
        return Sub71.R1;
    }
    //son RAW_TO_REAL(����� ohm�� ���) 4W �ּҰ��� 0.1 mohm  (4W Cal�� ������ mohm ��)
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

//son230925  4W RAW_TO_REAL��  0���� ���� üũ�ϴ� ����� Ư���� �߰���.  �̻��·� ���� Ʃ���� ���� �� �����Ƿ�  
//           ARun201_4w_R_Cal1() �Լ��� �������� �ʰ� �����ϱ�� �Ѵ�. 
//son220419_3 moved from ChildView7.cpp

//son Cal �ݿ��Ͽ� R���� �����Ѵ�.   // Raw R -> Real R
int ARun201_4w_R_Cal1(int m_nMode1, int part)  //sylee150710   //son220412 part �߰�
{
    int nPos, nRListCnt;
    double aa,ab;
    nPos=0;
    aa=0.0;
    ab=0.0;

    nRListCnt=(int)gdCal1[part][m_nMode1][0][0];// Cal  list ����
    //      Sub71.R1=dR14w[j+5][i]; 
    if ((Sub71.R1>0) &&  (nRListCnt>0) &&   (nRListCnt<=MAX_CAL_RLIST))   //son220408 128: MAX_CAL_RLIST
    {                                    
        
        //son220407  4W��  2W�� RCalCovert()�� �ٸ��� Ư���� 0���� �����Ѵ�.    
        //           ���� ���� RList 1���� gdCal1[part][m_nMode1][][RList]�� ä���� ������
        //           4W�� 1 ohm ���Ϸ� �����Ǵ� ��찡 �־ 1 ohm ������ ���� RList=0 �� ���� ����� �Ǵ��Ѵ�.
        //           2W�� �ݸ鿡 20230925 ���� 1 ohm ���Ϸ� �����Ǵ� ����  RList=1, RList=2 ������ ����� �Ǵ��Ѵ�. (������)
        
        //son �ش� ����� Calibration ���� �߿��� ���� ����R�� �´� ���� ã�´�.
        for( int RList1=0;  RList1<=nRListCnt;  RList1++)  // 4WSETUP   //sylee171113-2  
        {
            //son 'Use=1'�� ���   
			//son  FileSysInfo01.LoadSaveSer13(_LOAD,..) �� �� �̹� 1�� ��쿡�� gdCal1�� write�ϰ� nRcount�� �����Ѵ�.
            //     ���⼭ 1�� üũ�ص� �ǰ� ���ص� ��. 
            if(gdCal1[part][m_nMode1][CAL_USE][RList1]==1)  //cal enable 1,0  //son220407
            {
                //son ������ R ���� �ش� ����� ��� RList1 R ������ �ϳ����� �۴ٸ� stop.
                if( Sub71.R1< gdCal1[part][m_nMode1][CAL_REF_RAWR][RList1])      // cal R raw       //son220407
                {
                    nPos=RList1;            //son �ش� RList1 ��ġ�� nPos�� ����ϰ�
                    RList1=nRListCnt+1;    //son nRListCnt ��ŭ �ٷ� �������Ѽ� for ���� Ż�� 
                    break;//sylee171113-2
                } 
            }
        }                       

        if(nPos==0){       //gdCal1[part][m_nMode1][0:CAL_REF_R][k]  cal ref  R 
            Sub71.R1=999999; //sylee180607
        }
        else
        {
            //son  ����R / Real Ref R  == ����R / Calibration Raw R  ���� �̿��ؼ�  ���� R ���� �����Ѵ�.
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
  
    //son R ���� 0.1 �����̸� 171�� 0.1,  171�̿ܿ��� 999999999 �� ����.
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


#if 0 //son221014 begin:  RCalConvert_spark()��  RCalConvert_short()�� ��ü�Ͽ� �Լ� ����

// hdpark220819 begin: for leak current
//double RCalConvert_spark(int m_nMode1, double Rvalue, int ConMode, int part) 
// hdpark220819 end

#else
//son221014 moved from ModeSer15.cpp. 
//          ModeSer15::RCalCovert3() -> Globals.cpp RCalConvert_short() �� ��ü. �Լ����� 


#if 0   //son230925 RCalCovert() �������� RCalConvert_short()�� RCalCovert()�� ����.

//son ������������ ���ĵ� Short Cal Data �� ����Ѵ�.  
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
        nIn = CAL_REF_R;       //son220407 2  ���ذ�
        nOut = CAL_REF_RAWR;   //son220407 3
    }

    //ConMode: 1(RAW_TO_REAL)
    else {
        // raw ADC -> real ohm 	    
        //son nIn : gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList] : cal R(rawR ADC)
        //son nOut: gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList] : ref R(realR ohm) 
        nIn = CAL_REF_RAWR;   //son220407 3 ���� ��
        nOut = CAL_REF_R;     //son220407 2
    }

    int nRCount=(int)gdCal1[part][mode][0][0];// Cal  list sum

//son230919 begin;
    if (Rvalue < 0)
        Rvalue = 0;

    if( ConMode== RAW_TO_REAL && mode >= MODE_SHORT_START) //son �������� ���ĵ� ADC raw ���� ���ذ�
    {
        if (Rvalue > 65535) //son ADC ���� 65535 ���� Ŭ �� �����Ƿ� ���� ó��
            Rvalue = 0;
        
        double dMinLookupVal = gdCal1[part][mode][nIn][nRCount] / 2; //son Min ���� 1/2 ������ �ٻ簪���� ó��
        if (Rvalue < dMinLookupVal) //son Min ���� 1/2 ���� ������ ����ó�� 
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
        double dMaxLookupVal = gdCal1[part][mode][nIn][nRCount] * 2 ; //son Max ���� 2������� �ٻ簪 ó���� �Ѵ�. 
        if (Rvalue > dMaxLookupVal) //son Max ���� 2�踦 ������ ����ó��.
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
            // ConMode RAW_TO_REAL�̰�  SHORT�� ���, CALR%d.ini ������ rawR�� ������������ ���ĵǾ� �����Ƿ� ù��° ���� ���� �ٻ簪�̴�.
            if( ConMode== RAW_TO_REAL && mode >= MODE_SHORT_START) {
                if( Sub71.R1> gdCal1[part][mode][nIn][k]){    
                    break;      
                }
            }                                               
            //son ConMode: Open�̶�� �������� ���ĵ� cal table rawR �߿� �Էµ� rawR����  ù��° ū ���� ã�´�(�ٻ簪) 
            //son ConMode: REAL_TO_RAW�� Short, Open �������� ���ĵ�  cal table realR �߿�  �Էµ� realR���� ù��° ū���� �ٻ簪�̴�. 
            else {
                if( Sub71.R1< gdCal1[part][mode][nIn][k]){    
                    break;  
                }

            }
        }

        //son k-1 �� ���� ã�� ��ġ�̴�.

        if( k==1)  //son ù��° �׸� 
        {
            if((gdCal1[part][mode][nIn][2] - gdCal1[part][mode][nIn][1])!=0)    //son devide by zero ����
            {
                //son230919 Sub71.R1= (Sub71.R1/gdCal1[part][mode][nIn][1])  *  gdCal1[part][mode][nOut][1]; 
                //son230919 begin
                //son ù��° 1�� �׸��� ������ 1���� 2�� �� ���⸦ �����ؼ� �������� �Ѵ�.
                aa= (Sub71.R1-gdCal1[part][mode][nIn][1]) / (gdCal1[part][mode][nIn][2] - gdCal1[part][mode][nIn][1]);  
                ab= gdCal1[part][mode][nOut][2] - gdCal1[part][mode][nOut][1]; 
                Sub71.R1= ab * aa + gdCal1[part][mode][nOut][1];                                             
                if (Sub71.R1 < 0)   //son 2�� �׸��� ����� 1���� ����ϱ� ������ 0���� �۰� ���� ���� ����. 
                    Sub71.R1 = 0;   //    0���� �۰� ������ ������ 0���� �����Ѵ�. 
                //son230919 end
                
            }
            else{ //error
                //son230919 begin: ��Ȯ�� ����ó���� ����.  
                //    if( Sub71.R1< gdCal1[part][mode][nIn][k])�� break �߱� ������ ����� ���� ���� ���װ� �ִ� �����. 
                str.Format("Divide by Zero. \n (gdCal1[part][%d][%d][2]:%.2f -gdCal1[part][%d][%d][1]:%.2f) is 0! \n Mode=%d %s Converting Error !\n\n" , 
                                        mode, nIn, gdCal1[part][mode][nIn][2], 
                                        mode, nIn, gdCal1[part][mode][nIn][1], mode ,  
                                        (ConMode == REAL_TO_RAW)? "Real R -> Raw R": "Raw R -> Real R");   //son220412_3  //son230919
                errMessageBox(7221, str);
                //son230919 end
            }
        }
/* //son230919 ������ �׸� �Ȱ��� ���� ����̹Ƿ� �� �κ��� ������. 
        else if( k==nRCount)    //son ������ �׸�
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
        //son ���� ���
        //-----------------------
        else
        { 
            //son230919 ������ �׸��� +�ٻ簪 ó�����.  
            if (k == (nRCount+1))
                k = k-1;     //son ������ �׸��� �ٻ簪(k+1)�� 2�������  ������ �׸�(k)�� ����� ����ϱ�� �Ѵ�.

            //son k-1�� k�� ���̺� ���� �ٸ��ٸ� ���� ���
            if( (gdCal1[part][mode][nIn][k]-gdCal1[part][mode][nIn][k-1])!=0.0) //son devide by zero ����
            {
                aa=(Sub71.R1-gdCal1[part][mode][nIn][k-1])/(gdCal1[part][mode][nIn][k]-gdCal1[part][mode][nIn][k-1]);  // [3]= lookup table Calibration Raw R
                ab=gdCal1[part][mode][nOut][k]-gdCal1[part][mode][nOut][k-1]; 
                Sub71.R1=ab*aa+gdCal1[part][mode][nOut][k-1];                                             
            }
            else{ //error
                //son k-1�� k�� ���̺� ���� �����ؼ� ������ ���ٸ� �׳� ���� ���
                //      Sub71.R1�� ó������ rawR(ADC)�̰� ��ȯ�Ŀ��� realR(ohm) �̴�.
                //      realR/ ref realR �� ������ rawR/ ref rawR ������ ���ٰ� �����ϸ�
                //      realR = (rawR / refRawR) * refRealR
                //son230919 Sub71.R1=(Sub71.R1/gdCal1[part][mode][nIn][k-1])*gdCal1[part][mode][nOut][k-1]; 
                
                //son230919 begin: ��Ȯ�� ����ó���� ����.  
                //    if( Sub71.R1< gdCal1[part][mode][nIn][k])�� break �߱� ������ (<= �ƴ�)  ����� ���� ���� ���װ� �ִ� �����. 
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

