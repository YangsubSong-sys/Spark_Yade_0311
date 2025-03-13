// Globals.h: interface for the CGlobals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_)
#define AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <io.h>
#include "stdafx.h"
#include <stdarg.h>
#include "A_Define1.h"
#include "ModeSub17.h"		// for C4WNet Structure
#include <time.h>
#include <iostream>
#include <vector>


#ifdef _DEBUG
    #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
    // allocations to be of _CLIENT_BLOCK type
#else
    #define DBG_NEW new
#endif




#ifdef _PC_TEST_
//son CONTEC Board Define
#define DEF_CARD_A1
#endif



extern int nView1;		//son220215 moved from gobal.h


#define FILE_PATH_SIZE      500         //son210420
#define GET_BUF_SIZE        4096        //son210524 
#define GET_BUF_BIG_SIZE    (2048*20)   //son210624 4W_Setup_A.txt read ���� ���� ����. 
#define GET_BUF_SMALL_SIZE  255         //son230707


#define	_4W_PIN_NUM		4
#define	MAX_PIN		    (Def_SwitchMaxPin -1)	// 32768	32K,
//#define	MAX_4W_NET	10000			        //son231107 moved to A_Define103.h 
#define MAX_BLOCK       (DEF_MAX_BLOCK1 -1)      // 500     //son220823_6
#define	MAX_PIECE		(DEF_MAX_PIECE1 -1)	    // 300      //son220823_6   //son GTS�� 500

#define MAX_BLOCK_PIECE1 10001                  // Block * Piece�� �ִ밪 +1 //sylee200919
#define MAX_CELL        (MAX_BLOCK_PIECE1 -1)   //son240510 10000   Block * Pice �ִ밪


//#define	MAX_DUT			DefDutMax2			// 16  //son220824_2 DefDutMax2 -> MAX_DUT�� �����ϸ鼭 ����



//son  __func__�̳�, __FUNCTION__�� VC60���� ���Ұ��ϹǷ� �Ʒ� define�� �����.
// ��뿹��: 
//  printf("Func:%s,   Line:%d\n", FUNC(main), __LINE__);
//   => �ܼ�â�� "Func:main,  Line:22" �̷��� ��µ�.  
//   ȣ���� ������ function name�� �������� �����ִ� idea.
#define FUNC(fn)	#fn

#define	__LINE_PRT__	(__PrintLine(__FILE__, __LINE__));



//son20200623 �����ϳ�¥ __DATE__�� ���ڷ� ��ȯ�ϴ�  define 
#define YEAR ((((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0')) * 10 \
              + (__DATE__[9] - '0')) * 10 + (__DATE__[10] - '0'))
 

#define MONTH (__DATE__[2] == 'n' ? (__DATE__[1] == 'a' ? 1 : 6) \
               : __DATE__[2] == 'b' ? 2 \
               : __DATE__[2] == 'r' ? (__DATE__[0] == 'M' ? 3 : 4) \
               : __DATE__[2] == 'y' ? 5 \
               : __DATE__[2] == 'l' ? 7 \
               : __DATE__[2] == 'g' ? 8 \
               : __DATE__[2] == 'p' ? 9 \
               : __DATE__[2] == 't' ? 10 \
               : __DATE__[2] == 'v' ? 11 : 12)
 
#define DAY ((__DATE__[4] == ' ' ? 0 : __DATE__[4] - '0') * 10 + (__DATE__[5] - '0'))
 
#define DATE_AS_INT (((YEAR - 2000) * 12 + MONTH) * 31 + DAY)



class CGlobals  
{
public:
	CGlobals();
	virtual ~CGlobals();

};




///////////////////
// ���� ��Ʈ����
///////////////////


typedef struct sFILE_DATA
{
	CString	SystemDir;					// ACE400Analysis ���α׷��� ������ ���丮
	//CString ParameterDir;
	CString DataDir;					
	CString LogDir;					


	CString ACE400_4WDataDir;			// ACE400�� 4W data Dir ("D:\\log4w");

	CString SystemFileName; 
	//CString SystemFile; 

	sFILE_DATA()
	{
		SystemFileName		= "System.";

		ACE400_4WDataDir    = "D:\\log4w";
		LogDir              = "d:\\log";    //son220419_8
	}

	void Init()
	{
		// ���� ������SW�� ������ ���丮 ��ġ�� �����´�.
		char buffer[_MAX_PATH];
		if(_getcwd( buffer, _MAX_PATH ) == NULL)
		{
			AfxMessageBox("Root directory not found.");
			return;
		}

		SystemDir.Format("%s", buffer);
		//ParameterDir	= SystemDir + "\\" + "Parameter";
		DataDir			= SystemDir + "\\" + "Data";
		//son220510 LogDir			= SystemDir + "\\" + "LOG";

		//if( (_access(ParameterDir, 0 )) == -1 )	_mkdir(ParameterDir);
		if( (_access(DataDir, 0 )) == -1 )		
			_mkdir(DataDir);

		if( (_access(LogDir, 0 )) == -1 )		
			_mkdir(LogDir);

		//SystemFile		= ParameterDir	+ "\\" + SystemFileName;
	}

} sFILE_DATA;






//--------------------------
// Common Structure 
//--------------------------

// m_grid1 ��� enum

//	  MAP->4W�� Grid ȭ��
//                 0     1      2      3      4       5        6            7       8         9           10       11
//				  Piece Pin1   Pin2   Pin3   Pin4   Set.Min   Set.Avg   Set.Max    CalMode    Test.Min   Test.Avg  Test.Max 
//	ex)			   1     1      2      385    386    1.060   1.120        1.150    171       1.060       1.120     1.150
enum DATA4W_COL_TYPE {
#ifdef __GTS_4W_SPCIALNET__ 
   _4W_SPECIAL,     // 0  //son221107 GTS Ư���� ǥ��. Ư���� �߰��ÿ��� ���� ������ ��� 1�� �и���.
                    //    NUM_4W_GRID_COL �� 25�� ��.
#endif

   _4W_PIECE, 		// 0       //son200804: 4W Piece ���� ����.     //son211005 _4W_NET -> _4W_PIECE
   _4W_PIN1,		// 1
   _4W_PIN2,		// 2
   _4W_PIN3,		// 3
   _4W_PIN4,		// 4


   _4W_REF_LOW,	    // 5 //son211005 _4W_SET_MIN -> _4W_REF_LOW
   _4W_REF_AVG,	    // 6 //son211005 _4W_SET_AVG -> _4W_REF_AVG
   _4W_REF_HIGH,	// 7 //son211005 _4W_SET_MAX -> _4W_REF_HIGH
   _4W_SMP_MIN,	    // 8 //son211005 �ű�
   _4W_SMP_MAX,	    // 9 //son211005 �ű�

   _4W_MODE,		// 10 

   _4W_TEST_MIN,	// 11 
   _4W_TEST_AVG,	// 12 
   _4W_TEST_MAX,	// 13 

   _4W_DATA1,		// 14 
   _4W_DATA2,		// 15 
   _4W_DATA3,		// 16 
   _4W_DATA4,		// 17 
   _4W_DATA5,		// 18 
   _4W_DATA6,		// 19 
   _4W_DATA7,		// 20 
   _4W_DATA8,		// 21 
   _4W_DATA9,		// 22 
   _4W_DATA10,		// 23 

   NUM_4W_GRID_COL	// 24

};



//---------------------------------------
//son 4W net ���� define, enum, structure



class C4WNetData			//son190704-4WDUT16  Multi DUT ��� �߰�
{
public:                         // ��������
 	short	wPiece;				// gd4W1[0][net+5][0]   //son211005  wNet -> wPiece
	int	    waPin[_4W_PIN_NUM];	// gd4W1[0][net+5][1~4] //son  value�� pin ��(1~)   //son230407 short -> int
	double  dRefLow;			// gd4W1[0][net+5][5]   //son211005 dSetMin -> dRefLow (����)
	double  dRefAvg;			// gd4W1[0][net+5][6]   //son211005 dSetAvg -> dRefAvg
	double  dRefHigh;			// gd4W1[0][net+5][7]   //son211005 dSetMax -> dRefHigh (����)
	int  	n4wMode;			// gd4W1[0][net+5][8]	//son 4W TESTMODE ex) 171: MODE171_4W			
	double	dTestMin;			// gd4W1[0][net+5][9]				
	double	dTestAvg;			// gd4W1[0][net+5][10]								
	double  dTestMax;			// gd4W1[0][net+5][11]					
#ifdef __GTS_4W_SPCIALNET__
	byte 	bNetSpecial;		//son221107 edit�� dRefLow, dRefHigh�� ������ Ư������ ���  (0:�Ϲ�Net,  1:Ư��Net)
#else
	double	dReservd1;			// 
#endif
	double	dReservd2;			// 
	int  	nCurrMode;			// gd4W1[0][net+5][14] 	//son BDL data�� �ƴ�. (debug��)
													 	//son n4wMode�� �ӽ÷� �����ϴ� ��쿡 ����. 
								

public: 
   C4WNetData() 		//������
   {
		wPiece = 0;       //son211005  wNet -> wPiece
		::ZeroMemory(waPin, sizeof(waPin));

		dRefLow = 0;            //son 4W ��ǰ ���� ���� : Ref.Low    //son211005 dSetMin -> dRefLow
		dRefAvg = 0;            //son                   : Ref.Avg    //son211005 dSetAvg -> dRefAvg
		dRefHigh = 0;           //son 4W ��ǰ ���� ���� : Ref.High   //son211005 dSetMax -> dRefHigh
 		n4wMode = 0;

		dTestMin = 0;
		dTestAvg = 0;
 		dTestMax = 0;
#ifdef __GTS_4W_SPCIALNET__
 		bNetSpecial = 0;        //son221107
#else
 		dReservd1 = 0;
#endif
 		dReservd2 = 0;
 		nCurrMode = 0;

	}
};


//----------------------
//son 4W Net Data

//son 4W Net�� BDL ������ �����Ѵ�.

class C4WNet
{
public: 
	short			wCount;
	C4WNetData		saData[MAX_4W_NET];     //son index 0���� ���.

public:
	C4WNet() 		// ������
	{
		wCount = 0;
		::ZeroMemory(saData, sizeof(saData));
	}
 	void InitMember()
	{
	  	wCount = 0;
		::ZeroMemory(saData, sizeof(saData));

	}
};



//----------------------------------
//son g_pvBDLData2Op ���� item Data

//son Piece�� Open �˻� ���� Data ������ �����Ѵ�.
class COpenTestItem
{
public:
    short   wFirstPin;      //son 0 base
    short   wPin;           //son 0 base

public:
    COpenTestItem()     // ������
    {
        wFirstPin = 0;           
        wPin = 0;
    }

    void initMember()
    {
        wFirstPin = 0;           
        wPin = 0;
    }
}; 


//----------------------------------
//son g_pvBDLData31 ���� item Data

//son Piece�� Short �˻� ���� Data ������ �����Ѵ�.
class CShortTestItem
{
public:
    short   wFirstPin;      //son 0 base
    short   wNetPinCnt;     //son 0 base

public:
    CShortTestItem()     // ������
    {
        wFirstPin = 0;           
        wNetPinCnt = 0;
    }

    void initMember()
    {
        wFirstPin = 0;           
        wNetPinCnt = 0;
    }
}; 

///////////////
// ���� ���� 
///////////////


extern int 		nMulNetFlag;
extern int 		nMultyNetTotal;
extern int		getDutId();
extern int		get4wDutIdx();

//son gd4W1[][][]�� g_s4WNet ����ü�� ���� 	
extern C4WNet	g_s4WNet[MAX_DUT];			//son Multi DUT 4W Net Data 	//son190704-4WDUT16

//son190801 gd4W1[][][] ����. 	g_s4WNet[MAX_DUT]��  ��ü �Ϸ�
//extern float 	gd4W1[MAX_DUT][MAX_4W_NET][15]; 	

extern WORD		g_wPrintState;
extern sFILE_DATA 	g_sFile;

enum UNIT			{ sec=0, msec=1, usec=2, mA, uA, Mohm, Kohm, ohm, mohm, uohm };// sec=0, msec=1, usec=2 ����
enum UNIT_CONVERT_TYPE	{ 
	MM_TO_UM, UM_TO_MM, SEC_TO_MSEC, MSEC_TO_SEC
};
                                            //    mask
enum ePRT_TYPE { PRT_BASIC, 				// 0: 0x0001
				PRT_LEVEL1, 				// 1: 0x0002
				PRT_LEVEL2, 				// 2: 0x0003
				PRT_LEVEL3, 				// 3: 0x0004
				PRT_DEVICE, 				// 4: 0x0005
				PRT_DEV_D64,				// 5: 0x0006

//				PRT_TIMEOFF = 0x0100		// 0x0100
};


// BASIC ������ Time header off�� ����ϰ��� �� �� ���
//#define		PRT_TMFBAS		(PRT_BASIC | PRT_TIMEOFF)


//son220419_4  nFailL1[][20],  nFailL1Open[][20] item index�� ���.
enum FAIL_ITEM {                //son220407
    F_BLOCK           = 2,
    F_PIECE           = 3,
    F_VB_PIN          = 4,      // 1 base pin
    F_VC_PIN          = 5,      // 1 base pin
    F_RAW_R           = 6,		// ������  raw R
    F_PRORV_REAL_R    = 7,		// ���� real R
    F_PRO_STEP_REAL_R = 8,
    F_VSET            = 9,
    F_ISET            = 10,
    F_MODE1           = 11,
    F_VB              = 12,		// ������  VB
    F_VC              = 13,		// ������  VC
    F_I               = 14,		// ������  I
    F_REAL_R          = 15,		// ������  real R
    F_TEST_TYPE       = 16,		// TEST_TYPE(1:open 2:ushort 3:short 4:hr 5:hv   7:4w)
    F_PRORV_RAW_R     = 17,		// ���� raw R
    F_STEP            = 18,		//sylee231013 
	F_AC              = 19,     //sylee241204
    F_FAIL_ITEM_NUM   = 20      //son220920


};


#ifndef __OPVECTOR__
//son ChilcView7.cpp���� ����� �̵�. 
//extern short nBDLData2Op[DefDutMax][DEF_MAX_PIECE1][dSetPinTot2][3];//open data  //sylee150604-kushan    //sylee150917-2  16400 ->33000
extern short nBDLData2Op[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2][3];//open data  //son220824_2 DefDutMax -> MAX_DUT1(33) //son220823_6

//extern short nBDLData31[DefDutMax][DEF_MAX_PIECE1][dSetPinTot2];//short data   //sylee150604-kushan 100->20
extern short nBDLData31[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2];//short data   //son220824_2 DefDutMax -> MAX_DUT1(33)   //son220823_6

#else
//son200420-4WNET10000 ------------------------------------------------------------------
//    nBDLData2Op[MAX_DUT1][DEF_MAX_PIECE1][dSetPinTot2][3] �� 
//    vector <COpenTestItem> g_pvBDLData2Op[MAX_DUT+1][MAX_PIECE+1] �� ����
//    [dSetPinTot2] array�� COpenTestItem �� vector�� �ٲ��.   (�����Ҵ��� ����)
//    [3] array�� COpenTestItem ������ member ���� wFirstPin, wPin���� ��ü�ȴ�.
//
//    ����Ҷ����� (*g_pvBDLData2Op[nDut][nPiece])[nPiecePin].wFirstPin �� ���� ����ϸ� �ȴ�.
//------------------------------------------------------------------------------------------

using namespace std;

//son         DUT(32) * MAX_PIN(32768)  = 1048576
//    DUT(32) * MAX_PIN(32768) * 4byte  = 4194304  
//    Piece�� Pin�� �ݺ�ʰ��� (Piece * Pin = 32768). Piece�� 1�̸� Pin�� max�� 32768���� ����.
//    ���� OpenTest ������ MaxPin* MaxDUT������ �غ��ϸ� �ȴ�.
#define MAX_OPEN_TEST           (MAX_DUT * (Def_SwitchMaxPin-1))  

extern int     g_nOpenTestCount;

extern vector <COpenTestItem>	*g_pvBDLData2Op[MAX_DUT+1][MAX_PIECE+1];        // �����Ҵ� vector
            //son 1. nBDLData2Op[nDut][piece][0][0]  => g_pvBDLData2Op[nDut][nPiece]->size() �� ��ü
            //    2. nBDLData2Op[nDut][piece][piecePinIdx][1] => (*g_pvBDLData2Op[nDut][nPiece])[nPin].wFirstPin�� ��ü
            //    3. nBDLData2Op[nDut][piece][piecePinIdx][2] => (*g_pvBDLData2Op[nDut][nPiece])[nPin].wPin�� ��ü


extern int  InsertOpenTest(int nDut, int nPiece, COpenTestItem openTestItem);    // OpenTestItem�� vector�� �߰�
extern int  InsertOpenTest_Vector(int nDut, int nPiece);
extern void DeleteAllOpenTestItem();

#define MAX_SHORT_TEST         (MAX_DUT * (Def_SwitchMaxPin-1))
extern int      g_nShortTestCount;
extern vector <CShortTestItem>	*g_pvBDLData31[MAX_DUT+1][MAX_PIECE+1];        // �����Ҵ� vector
            //son 1. nBDLData31[nDut][piece][0]             => g_pvBDLData31[nDut][nPiece]->size() �� ��ü
            //    2. nBDLData31[nDut][piece][piecePinIdx]   => (*g_pvBDLData31[nDut][nPiece])[nPin].wFirstPin���� ��ü


extern int  InsertShortTest(int nDut, int nPiece, CShortTestItem shortTestItem);    // shortTestItem�� vector�� �߰�
extern int  InsertShortTest_Vector(int nDut, int nPiece);

extern void ZeroPieceShortTestItem(int nDut, int nPiece);
extern void DeleteDutShortTestItem(int nDut);
extern void DeleteAllShortTestItem();
extern void ReadPinPieceNo(int nDUT);  //sylee180810-1-DUT16

#endif //son200420-4WNET10000


////////////////
// ���� �Լ�
////////////////


//extern	void	DoEvents(DWORD dwMilliseconds=0);
//extern  void		Delay(UINT delaytime=0, UINT unit=usec);
extern  __int64 	GetMicroSecond();  //sylee
extern  BOOL		FileExists(CString strFilePath);
extern  BOOL        CheckDirExist(char * pn);           //son211222 moved from ChildView7.cpp
extern  BOOL        MakeNewDir(CString DirPath);        //son211222 moved from ChildView7.cpp
extern  BOOL        CopyFileDir(char* dest, char* src, int deletemode/*=FALSE*/);    //son211222 moved from ChildView7.cpp
extern  void 		GetDateAndTime(char *datetime);
extern  void        GetCompiled_DateAndTime(char *datetime);
extern  SIZE_T 		GetProcessWorkingSetSize();
extern  void 		__PrintMemSize(char* strFunc, int line);
extern  void 		__PrintLine(char* strFile, int line);

extern  void 		AFX_CDECL MyTrace(ePRT_TYPE prtType, LPCTSTR lpszFormat, ...);

extern  double 		p_tdist(double t, int df);		// TDist.cpp
extern  void        CleanUp_MyTraceLog(char *pn);   //son220510 �߰� 
extern  int         CleanUp_SparkLog(LPCTSTR szDir, int nRecursive, int nMonth);   //hdpark231121 �߰� 
extern  void        Check_DDrive_FreeSpace();       //son221215 �߰�
extern  void        errMessageBox(int nError, LPCTSTR errStr);     //son230217_2
extern  int         NetWorkConnection1(); //sylee231010-1 //son240705 mereged from ACE1000

enum  R_MODE {   
    R_RAW  = 1,    //son230214
    R_REAL = 2,    //son230214
    R_SPARK =3     //son230214
};

enum  CONV_MODE {   
    RAW_TO_REAL  = 1,   //son220412_3
    REAL_TO_RAW  = 2    //son220412_3
};

extern  double      RCalCovert(int m_nMode1,  double Rvalue, int ConMode, int part);    //son220412 moved from ChildView8
extern  int         ARun201_4w_R_Cal1(int m_nMode1, int part);      //son220419_3 moved from ChildView7
//son221014 extern  double      RCalConvert_spark(int m_nMode1, double Rvalue, int ConMode, int part);    //hdpark220819
//son230925 extern  double      RCalConvert_short(int mode,  double Rvalue, int ConMode, int part); 
	//son221014 moved from ModeSer15::RCalCovert3() -> Glabals.cpp RCalConvert_short() 
	//son230925 RCalConvert_short() �Լ� ����.  CalConvert_short()�� ����� RCalCovert()�� ������. 


//extern  int  		AFX_CDECL MyTrace2(ePRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...);
//extern  int  		AFX_CDECL MySnprintf(ePRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...);
//extern  BOOL		DeleteSubDir(const TCHAR* sPath);
//extern  void		DeleteFolder(char *strDirectory);

// hdpark240102 begin
typedef struct tagSPARK_AI
{
    float dRefData;
    float dPreData;
    float dPostData;
    int   nCount;
}SPARK_AI;

// hdpark240102 end

#endif // !defined(AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_)
