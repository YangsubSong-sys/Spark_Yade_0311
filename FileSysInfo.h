// FileSysInfo.h: interface for the CFileSysInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESYSINFO_H__B85E18EA_5A5B_4129_A54C_D02D75AE6A24__INCLUDED_)
#define AFX_FILESYSINFO_H__B85E18EA_5A5B_4129_A54C_D02D75AE6A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum __CC_CV {  //son231005_3 __CC(1), __CV(2) enum �߰�
    __CC = 1,
    __CV = 2
};

enum __LV_HV {  //son231222 __LV(1), __HV(2) enum �߰�
    __LV = 1,
    __HV = 2
};


//son for nVRel, nVBSrc,  dRec[m_nMode1][2]
enum VR_LV_HV {  //son240130
   VR_LV1 = 1,  
   VR_LV2 = 2,  
   VR_HV1 = 3,  
   VR_HV2 = 4 
};


enum LOAD_SAVE {        //son-32KPIN-20190902:  enum �ű� �߰�
    _SAVE = 1,
    _LOAD = 2,

    _SAVELOW = 11,        //son211012
    _LOADLOW = 12,        //son211012

    _SAVEHIGH = 21,        //son211012
    _LOADHIGH = 22,        //son211012
};

class CFileSysInfo  
{

public:

    CFileSysInfo();

    int LoadSaveView01(int type);
    int LoadFileDrive1();//sylee201201-1

//SYLEE180807-1 DUT16 DEL.  int LoadSaveView01D(int type);
//SYLEE180807-1 DUT16 DEL.  int LoadSaveView01E(int type);
//SYLEE180807-1 DUT16 DEL.  int LoadSaveView01F(int type);

    int getEndBoxNo(int nPinTotMax, bool bIsHwPin=true);  //son211020  //son240522
    int getCardNoEnd(int nCardX, int nPinTotMax);   //son211020
    int getCardNoEnd_NoSkip(int nCardX, int nPinTotMax, int nPinBlockType);   //son240125
    int getPrintCardNo(int nPinTotMax, int nCard);  //son231213
    int LoadSaveSet12(int type);
    int LoadSaveSet13(int type);//sylee121006
    int LoadSaveSet15(int type);//sylee121015-1
    int LoadSaveSet16(int type); 
    int LoadSaveSet17(int type);
    int LoadSaveSet19(int type); //SYLEE150407


    int LoadSaveSet21(int type);//sylee130618 
    int LoadSaveSet22(int type);//sylee130618 
    int LoadSaveSet21_4W_Range(int type);//SYLEE171012-1
    int LoadSaveSet21_4W_Cal1Pin(int type);//sylee171020-1

    int LoadSaveSet211(int type);//sylee160524-1

    int LoadSavePieceRangeInfo(int DutNo, int type);  //sylee180810-1-DUT16 //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
    int LoadSaveSub12(int type);
    int DisplayBlock_ReadMode1();  //sylee180502-1  //son200510-4WLOG : �߰�
    int LoadSaveBlockInfo(int type);    //son220824_5 LoadSaveSub13() -> LoadSaveBlockInfo()
    int LoadSaveLowerStartPin(int type, CString& rStr);//SYLEE210406    //son211214 rStr �߰�
    int LoadSaveSub15(int type);
    void printLog_Recipe();         //son220901 LoadSaveSub15()���� �ε��� recipe������ D:\log������ ���
    bool checkRecipeStep_ShortExist(); //son221104 Recipe�� step�� short, leak�� �����ϴ��� üũ�Ѵ�.

    int LoadSaveSub17(int type);
    void LoadSaveSub17_test(int dutIdx);
    int LoadSaveSub17_4w_ini(int type);     //son221107 GTS Ư���� ��� ���� �߰� 
#ifdef __GTS_4W_SPCIALNET__
    int LoadSaveSub17_4w_v2_ini(int type);  //son221107 GTS Ư���� ��� ���� �߰� 
#endif
#ifdef __NANOSYS__CUSTOMER
    void Init_4wFile_Ref_AllDut();   //son231018 ����ý� �䱸����. ���� 4W Ref �ʱ�ȭ ��� 
#endif

    int LoadSaveSub18(int type);//sylee160503
 
    int LoadSaveSub19(int type, int nFileType);
    int LoadSaveSub20(int type);
    int LoadSavePieceInfo(int type); //son210430 LoadSaveSub25 ->LoadSavePieceInfo

    int LoadSaveSer11(int type);
    int LoadSaveSer12(int type);
    int LoadSaveSer13(int type, int mode, int part);    //son220412

//    int  LoadSaveFullZeroCal1(int Mod1,int RecipeNo, int type);  //FULL ZERO CAL SYLE20120110
    int  LoadSaveHVCal1(int type);
    int  LoadSave_Model_Load1(int type);
//  int  LoadSave_Model_Load1_HR(int type);//sylee150407

    int SaveInfo_Piece1(int type);//sylee130911
    int SaveInfo_Block1();//sylee180209-1

    int LoadTestConditionData();  //son210324 1. C:\ACE500\Data\TestConditionSample.csv���� nItemType, szName, szFixedVal�� read  
                                  //          2.  (*fp)() �Լ� �����Ϳ� ������ �Լ��� ��ġ
    int Write_TestCondionLog();   //son210324  ���� Recipe ��Ȳ�� Z:\\TestConditionLog.csv�� ����Ѵ�.

    bool LoadSave_UseLowerPin(int type, bool &r_bUseLowerPin);  //son220331 CModeSer15���� CFileSysInfo�� LoadSave_UseLowerPin()�Լ� �̵�.
    int checkUseCalDataPart(int pin1,  int pin2);    //son220419 cal�� upper, lower ���θ� pin�� ���� �Ǵ�
    

    virtual ~CFileSysInfo();

};

extern CFileSysInfo  FileSysInfo01; //sylee150319-1


struct CSysInfoPath01  //Path   FilePathJig
{
    CString m_pStrFilePath1; //set
};



//FileSysInfo.h
//
//son "C:\\ACE400\\SETUP\\FilePathJig.ini"  ����
//
//son C:\ACE500\BDL\036901A-2                       // SysInfoView01.m_pStrFileDri1
//son C:\ACE500                                     // SysInfoView01.m_pStrFilePath1
//son C:\ACE500\BDL\036901A-2\036901A-2.BDL         // SysInfoView01.m_pStrFilePathJig1 
//son C:\ACE500\BDL\036901A-2\036901A-2.BDL         // SysInfoView01.m_pStrFilePathJig2
//son C:\ACE500\BDL\036901A-2                       // SysInfoView01.m_pStrFilePathBDL
//son 036901A-2.BDL                                 // SysInfoView01.m_pStrFileBDLName

struct CSysInfoView01  //Path   FilePathJig
{ 
    char  m_pStrFileDri1[200];                      //son ������ ���� ��ġ   ex) "C:\ACE500" �Ǵ� ������ �ٸ� ����
    char  m_pStrFilePath1[200];                     //son ������ �⺻ Folder ex) "C:\ACE500"  ����

    //son CChildView1::OnInitialUpdate()-> FileSysInfo01.LoadSaveView01(2) ȣ���ؼ� ���� �ʱ�ȭ�Ѵ�.    
    char  m_pStrFilePathJig1[200]; //jig load       //son full path  ex) C:\ACE500\QC-JIG-S100-BHFLEX\4W_YP_S100\OPEN-READ.BDL
    char  m_pStrFilePathJig2[200]; // jig set       //son full path2 ex) C:\ACE500\QC-JIG-S100-BHFLEX\4W_YP_S100\OPEN-READ.BDL
    char  m_pStrFilePath[200]; //set
    char  m_pStrFilePathBDL[200]; //SYLEE121202     //son dir Name  ex) C:\BDL\10VGAG011J
    char  m_pStrFileBDLName[200]; //SYLEE121202     //son file Name ex) 10VGAG011J.BDL
    char  m_pStrFileDrive1[200]; //sylee201201-1    //son201201 ��� -> ������ ��������  : "D:\\FromHandler" ��� �� path ���.
};
extern CSysInfoView01  SysInfoView01;   



//son201201 ���&������ �������� In/Out ���� �и�
//          ���������� ���� Z����̺�� ������-> ��� �������� ��ȯ�ϰ�
//          "D:\\FromHandler" ������ �ű� �����Ͽ� ���->������ �������� ����ϱ�� �Ѵ�.
//struct CSysInfoInOutPath_Hanlder  //son201201   "C:\\ACE500\\SETUP\\ZDrivePath.ini"  
//{ 
//    char  m_pStrPath_Input[200];   //son201201 ��� -> ������ ��������  : "D:\\FromHandler"       //son==> ����. ��� SysInfoView01.m_pStrFileDrive1 ���
//    char  m_pStrPath_Output[200];    //son201201 ������ -> ��� ��������  : "Z:\\" 
//
//};


//son200525-4WLOG  CSysInfo01 -> CSysInfoPieceRange 
//  IDD_MODE_SUB11_PIECERANGE  ����
//son C:\ACE400\SETUP2\PieceRange.ini
//
//son ex1) 
//son  1�� piece�� �������� piece�� �ɰ� ������ �� �� ���
//         1                            // SysInfoPiece.m_nSetMode[DutNo]    : ���õ�.   (1����)
//         2                            // SysInfoPiece.m_nPieceCount[DutNo] : piece count�� ���.  2��
//     30000                            // SysInfoPiece.m_nPinTot[DutNo]     : 30000�����̰ų� �ִ� pin no���� ū ��
//         8                            // SysInfoPiece.m_nRow[DutNo]        : Piece range ���� 8��
//         1         1      1509        // SysInfoPiece.m_nData[DutNo][i][0~2] : Piece range data 
//         1      3027      3036        //                  PieceNo     StartPin    EndPin
//         1      3044      3053
//         1      4097      5551
//         2      1510      3024
//         2      3037      3043
//         2      3054      3060
//         2      5552      7012
//
//son ex2) ���� ���̽�, piece�� 2�������� pieceRange�� 5���̴�.  14137~ 14164 ������ �������� piece1��. 
//1
//         2
//32768
//         5
//         1           1         3625   
//         2        3627         7252   
//         1        8193        11143   
//         2       11145        14135   
//         1       14137        14164 
//son220824_2 begin: DefDutMax(33) -> MAX_DUT1(33)
struct CSysInfoPieceRange  //SETUP MUTI  //sylee180810-1-DUT16 
{
    //int m_nMutiNo[DefDutMax];        //son200525 m_nMutiNo -> m_nPieceCount
    int m_nPieceCount[MAX_DUT1];       //son m_nPieceCount[Dut] => Label1 Piece Count

    int m_nPinTot[MAX_DUT1];           //son m_nPinTot[Dut]     => Label2 Total Point 
    int m_nRow[MAX_DUT1];              //son m_nRow[Dut]        => Label3 Row Count. piece ���� ����,  
                                       //son                   ���, �Ϻ� �����̹Ƿ� piece ������ 2��
                                       //                      ����ó�� PieceRange�� �߰��� �� �ִ� ���̽��� �ִ�.                                     
                                        
    int m_nPin[MAX_DUT1][3001][5]; //sylee170111  200-> 3001 
                                        //son m_nPin[DutNo][piece][0] : PieceNo  (1~)
                                        //son m_nPin[DutNo][piece][1] : PieceStartPin
                                        //son m_nPin[DutNo][piece][2] : PieceEndPin
                                         
};
//son220824_2 end



struct CSysInfo02  //SETUP Piece
{
    int m_nRow;
    int m_nCol;
    int m_nData[DEF_MAX_PIECE1][DEF_MAX_PIECE1];        //son220823_6
};





//son200525-4WLOG SysInfo03 -> SysInfoBlock ���� ������
//son  ex)  
//        20                                    // m_nBlock
//         5                                    // m_nRow
//         4                                    // m_nCol
//         1                                    // m_nType  (Block Align Display)
//         1         2         3         4      // m_nData[5][4]
//         5         6         7         8
//         9        10        11        12
//        13        14        15        16
//        17        18        19        20
//         1                                    // m_nType2 (Block Step Direction)
struct CSysInfoBlock  //SETUP Block from  ( BDLPath\\\SETUP2\\Block.ini)
{
    int m_nBlock;               //son   // Block ���� : m_nRow * m_nCol             // BlockCount
    int m_nRow;                 //son   // Block Row ����                           // 
    int m_nCol;                 //son   // Block Col ���� 
    int m_nData[DEF_MAX_BLOCK1][DEF_MAX_BLOCK1];        //son220823_6
                                //son   // Block No ��ġ data  [BlockRow][BlockCol] // listBlockNo

    //int m_nType;      //son200525 m_nType -> m_nBlockAlign
    int m_nBlockAlign;          //son   // Block No ��� ����  (Block Align Display ����) 
                                        // 1: A (X, Top Left)                       // DirectionType
                                        // 2: B (X, Top Right)
                                        // 3: C (X, Bottom Left)
                                        // 4: D (X, Bottom Right)
                                        // 5: E (Y, Top Left)
                                        // 6: F (Y, Top Right)
                                        // 7: G (Y, Bottom Left)
                                        // 8: H (Y, Bottom Right)
                                        // ex) Block 6���϶� �� m_nType�� Block Display 
                                        //      A       B       C       D       E       F       G       H
                                        //      1 2     2 1     5 6     6 5     1 4     4 1     3 6     6 3
                                        //      3 4     4 3     3 4     4 3     2 5     5 2     2 5     5 2
                                        //      5 6     6 5     1 2     2 1     3 6     6 3     1 4     4 1
                                        
    //int m_nType2;     //son200525 m_nType2 -> m_nStepDirection
    int m_nStepDirection;//sylee180417-1 //son  // Block Step Direction (�˻� ���� ����)    
                                        // 1,  A    (X, Left  -> Right  )
                                        // 2,  B    (X, Left->Right , Right <- Left )
                                        // 3,  C    (Y, Top -> Bottom  )
                                        // 4,  D    (Y, Top->Bottom, Bottom->Top )
                                        //
                                        // ex) Block 6��  m_nType2 ��  step ���� (�˻� ����)
                                        //     1(A) :  1, 2, 3, 4, 5, 6
                                        //     2(B) :  1, 2, 4, 3, 5, 6
                                        //     3(C) :  1, 3, 5, 2, 4, 6
                                        //     4(D) :  1, 3, 5, 6, 4, 2
};

//son200525-4WLOG SysInfo25 -> SysInfoPiece ���� ������
//son IDD_MODE_SUB25_PIECE  �߿�!!
struct CSysInfoPiece  //SETUP piece
{
    //int m_nBlock;          //son200525 m_nBlock-> m_nPiece
    int m_nPiece;               //son   // Piece ���� : m_nRow * m_nCol         // TotalPiece

    int m_nRow;                 //son   // Piece Row ����                       // Row
    int m_nCol;                 //son   // Piece Col ����                       // Col
    int m_nData[DEF_MAX_PIECE1][DEF_MAX_PIECE1];        //son220823_6
                                //son   // Piece No ��ġ data  [PieceRow][PieceCol]
    int m_nType;                //son   // 1: A (X, Top Left)
                                        // 2: B (X, Top Right)
                                        // 3: C (X, Bottom Left)
                                        // 4: D (X, Bottom Right)
                                        // 5: E (Y, Top Left)
                                        // 6: F (Y, Top Right)
                                        // 7: G (Y, Bottom Left)
                                        // 8: H (Y, Bottom Right)
    int m_nRowDel;              // �̻��
    int m_nColDel;              // �̻��
};

enum SELF_TEST_TYPE {                //son240130 ChildView5, ChildView8���� ���.
    SELF_TEST_OPEN           = 1,   //son for nCase
    SELF_TEST_SHORT1         = 2,   //son for nCase
    SELF_TEST_SHORT2         = 3,   //son for nCase
    SELF_TEST_LEAK1          = 4,   //son for nCase
    SELF_TEST_LEAK2          = 5,   //son for nCase
    SELF_TEST_LEAK_A         = 6,   //son for nCase
    SELF_TEST_LEAK_B         = 7,   //son for nCase
    SELF_TEST_VC_1PIN_RST    = 8,   //son for nCase
    SELF_TEST_VB_1PIN_RST    = 9,   //son for nCase

    SELF_TEST_LV_OPEN        = 1,   //son for nProStep
    SELF_TEST_LV_SHORT1      = 2,   //son for nProStep
    SELF_TEST_LV_SHORT2      = 3,   //son for nProStep
    SELF_TEST_LV_LEAK1       = 4,   //son for nProStep
    SELF_TEST_LV_LEAK2       = 5,   //son for nProStep
    SELF_TEST_LV_LEAK_A      = 6,   //son for nProStep
    SELF_TEST_LV_LEAK_B      = 7,   //son for nProStep

    SELF_TEST_HV_LEAK1       = 14,  //son for nProStep
    SELF_TEST_HV_LEAK2       = 15,  //son for nProStep
    SELF_TEST_HV_LEAK_A      = 16,  //son for nProStep
    SELF_TEST_HV_LEAK_B      = 17,  //son for nProStep

    SELF_TEST_MAX = 18 
};

enum TEST_TYPE {                //son191212:  enum �ű� �߰�, SysInfo05.m_nStep[i]���� ���
    TEST_SKIP           = 0,
    TEST_OPEN           = 1,
    TEST_U_SHORT        = 2,    // LV
    TEST_SHORT          = 3,    // LV
    TEST_HR_SHORT       = 4,    // LV
    TEST_LEAK1          = 5,    // HV
    TEST_LEAK2          = 6,    // HV
    TEST_4W             = 7,
    TEST_RESERVED       = 8,

    TEST_ALL_OPEN       = 11,   //son open+4w count. nFailCountP2[nCellNo][TEST_ALL_OPEN]�� ���  //son220823 
    TEST_ALL_SHORT      = 12,   //son uShort+Short+Hr+Leak1+Leak2 count   nFailCount[nCellNo][TEST_ALL_SHORT]�� ���.  //son210331 
    TEST_MAX = 13 
};

//son220903 TestMode String define
#define STR_TEST_TYPE(testType)  ((testType == TEST_SKIP) ? "SKIP" : \
                                  (testType == TEST_OPEN) ? "OPEN" : \
                                  (testType == TEST_U_SHORT) ? "SHORT" : \
                                  (testType == TEST_HR_SHORT) ? "HR" : \
                                  (testType == TEST_LEAK1) ? "LEAK1" : \
                                  (testType == TEST_LEAK2) ? "LEAK2" : \
                                  (testType == TEST_4W) ? "4W" : "UNDEF_STEP")


enum eCURR_TYPE {       //son230711 enum �߰�: Recipe ȭ�� ���� �޺��ڽ��� ���.
    CURR_20mA    = 1,
    CURR_10mA    = 2,
    CURR_5mA     = 3,
    CURR_3mA     = 4,
    CURR_1mA     = 5,
    CURR_TYPE_NUM = 5
};


//son220902 Open ��������ȭ string define   
//son230711 enum �߰� 
#define STR_OPEN_CURR(current)   ((current == CURR_20mA) ? "20mA" : \
                                  (current == CURR_10mA) ? "10mA" : \
                                  (current == CURR_5mA) ? "5mA" : "")


//son220902 Short ��������ȭ string defin   
//son230711 enum �߰� 
#define STR_SHORT_CURR(current) ((current == CURR_20mA) ? "20mA" : \
                                 (current == CURR_10mA) ? "10mA" : \
                                 (current == CURR_5mA) ? "5mA" : \
                                 (current == CURR_3mA) ? "3mA" : \
                                 (current == CURR_1mA) ? "1mA" : "")

//son220902 Leak ��������ȭ string define
//son220907 �̻�������� ��û���� Leak ��������ȭ string��  20 10 7 5 3 mA -> 20 10 5 3 1 mA�� ���� 
//son230711 enum �߰� 
#define STR_LEAK_CURR(current)  ((current == CURR_20mA) ? "20mA" : \
                                 (current == CURR_10mA) ? "10mA" : \
                                 (current == CURR_5mA) ? "5mA" : \
                                 (current == CURR_3mA) ? "3mA" : \
                                 (current == CURR_1mA) ? "1mA" : "")

enum _4W_SET_TYPE {
    _4W_SET_FAST   = 0,
    _4W_SET_NORMAL = 1,
    _4W_SET_SLOW   = 2,
    _4W_SET_FINE   = 3
};

//son220903 4W Mode String define
#define STR_4W_SET(n4wSet)       ((n4wSet == 0) ? "Fast" : \
                                  (n4wSet == 1) ? "Normal" : \
                                  (n4wSet == 2) ? "Slow" : \
                                  (n4wSet == 3) ? "Fine" : "Undef")


//son IDD_MODE_SUB15_RECIPE ����
struct CSysInfo05  //SETUP Recipe
{
    int m_nShortDisable1; //sylee180331-1 
    int m_nCon;                     //son m_combo2  : (OPEN ��, Continuity R ������)
            //son 0:<Open >, 1: 5 ohm, 2: 10ohm, 3: 20ohm, 4: 30ohm, 5: 40ohm, 6: 50ohm, 7: 60ohm, 8: 70ohm, 9: 80ohm, 10: 90ohm,
            //   11: 100ohm, 12: 200ohm, 13: 300ohm, 14: 400ohm, 15: 500ohm, 16: 600ohm, 17: 700ohm, 18: 800ohm, 19: 900ohm,
            //   20: 1kohm, 21: 10kohm (10V), 22: 30kohm (10V), 23: 50kohm (10V), 24: 100kohm (10V), 25: *User
    int m_nConU;            //son m_edit1  (OPEN R User ������)

    int m_nMs;
    int m_nSp;              //son m_combo6      // Spark Mode   //0: OFF, 1: ON
    
    int m_nPr;              //son 
//  int m_nDeT;
    int m_nConEr;           //   Open Error Max Count No     
                            //son Open Fail ������ �̰��� �Ѿ�� OpenTest�� �ߴ��Ѵ�.
                            //    üũ��� OpenFail 1) nFailPiece[ip][nPstep] �� m_nConEr �Ѿ�� �ش� piece, step�� OpenTest �ߴ�
                            //    üũ��� OpenFail 2) nOpenFail_Co1[ip] �� m_nConEr �Ѿ�� �ش� piece�� OpenTest �ߴ�
                                    
    int m_nIsEr;            //   Isolation Error Max Count No: 
    int m_nConErSt;
    int m_nReCh;            // Label5  : ReCheck No.  �ҷ� �߻��� Total RETEST Count  
                            //son �����Ⱑ  RETEST�ϴ� Ƚ��. 
                            //    �� ���� �����Ǹ� Open Fail ���� ������ �� Ƚ����ŭ RETEST�� �����Ѵ�. 
                            //     (RETEST�� �����Ⱑ �����ϰ� ��� �뺸�ؼ� ������ �ٽ� �ϰ� TEST�� ������� ������ �Ѵ�.)
                            //    RETEST�� ���� block�� OpenTest, 4W �׽�Ʈ�� ������ ���� �ϳ��� Fail�� ������  
                            //    blockCountP�� ������Ű�� �ʰ� ��񿡼� TEST �޾��� �� �ٽ� ���� Block�� �׽�Ʈ�ϴ� ���̴�.
                            // nOpenErrList[][]�� nOpenErrData[][][]�� �����ϱ� ���� �̰� ��� �Ϸ��ߴ��� Ȯ����.

//  int m_nCorR;// R OR C MODE
    int m_nConFail; // 
    int m_nHR; // HR  C USER SET    //son m_combo8 (HR  R ������)
                                    //son 0: <HR>, 1: 100k, 2: 500k, 3: 1M, 4: 5M, 5: 10M, 6: *User,
    int m_nHRL; // C USER SET       //son m_edit5 (HR R User ������)

    int m_nDelay1; // C USER SET

    int m_nShortR;                  //son m_combo9 (u Short R ������)
                                    //son 0: <Short>, 1: 10 K, 2: 50 K, 3: 100 K, 4: * User
    int m_nShortRU;                 //son m_edit02 (u Short R User ������)


    int m_nShortR2;                 //son m_combo10 (L Short R ������)
                //son 0: <Short>, 1: *, 2: 1k, 3: 5k, 4: 10k, 5: 30k, 6: 50k, 7: 100k, 8: 200k, 9: 300k, 
                //   10: 400k, 11: 500k, 12: 600k, 13: 700k, 14: 800k, 15: 900k, 16: 1M, 17: *, 18: *, 19: *,
                //   20: *, 21: *, 22: *, 23: *, 24: *, 25: *, 26: *User
    int m_nShortR2U;                //son m_edit57 (L Short R User ������)

    int m_nHv1R;                    //son m_combo3  HV(Leak1) R ������.  //son210324 m_nConIs-> m_nHv1R
                                    //son 0: <Leak>, 1: 1M, 2: 10M, 3: 20M, 4: 50M   (>=100V), 5: 100M (>=150V), 6: *User
    int m_nHv1RU;                   //son m_edit101  (Leak1 R User ������)  //son210324 m_nConIsU-> m_nHv1RU
    
    int m_nHv2R; //sylee140923      //son m_combo43 (HV Leak2 R ������)
                 //son 0: <Leak>, 1: 1M, 2: 10M, 3: 20M, 4: 50M   (>=100V), 5: 100M (>=150V), 6: *User, 
    int m_nHv2RU; //sylee140923     //son m_edit25 (HV Leak2 R User ������)

    int m_nOpenV;                   //son m_combo22
                //son 0: <Volt.>, 1: *, 2: 10V, 3: *, 4: 30V,  5: 40V, 6: 50V*, 7: 60V, 8: 70V, 9: 80V, 
                //   10: 90V,  11: 100V*, 12: 110V, 13: 120V, 14: 130V, 15: 140V, 16: 150V*, 17: 160V, 18: 170V, 19: 180V
                //   20: 190V, 21: 200V*, 22: 210V, 23: 220V, 24: 230V, 25: 240V, 26: 250V*  
    
    int m_nShort1V;                 //son m_combo23     //0: <Volt.>, 1: 1V
    int m_nShort2V;                 //son m_combo24     //0: <Volt.>, 1: 10V
    int m_nHRV;                     //son m_combo26     // HR �˻� �ΰ� V ��,   
    int m_nHv;                      //son m_combo4
    int m_nHv2; //sylee140923       //son m_combo20

    int m_nCC[7];                   //son m_nCC[3]: m_check4    Open Offset Enable. üũ�� ������ m_edit2 ��� 
                                    //son m_nCC[4]: m_edit2     Open offset Value �뵵�� ���� ���� ����    -> ini ���Ϸ� ����
                                    
    int m_nLeak1_Current;           //son220706 m_nCC[1] -> m_nLeak1_Current      
                                    //son m_combo25   Leak1 ��������.   // 1:20mA  2:10mA  3:7mA  4:5mA, 5:3mA 
                                    //   CModeSub15 m_combo25.GetCurSel()+1; 1�� ���ؼ� ����Ѵ�.

    int m_nLeak2_Current;           //son220706 m_nCC[2] -> m_nLeak2_Current      
                                    //son m_combo27   Leak2 ��������.   // 1:20mA  2:10mA  3:7mA  4:5mA, 5:3mA
                                    //    CModeSub15 m_combo27.GetCurSel()+1; 1�� ���ؼ� ����Ѵ�.
                                    

    int m_nShort_Current;           //son220706 m_nCC[5] -> m_nShort_Current 
                                    //son m_combo30   LShort, HR Short ���� ��������.  // 1:20mA  2:10mA  3:5mA  4:3mA, 5:1mA
                                    //    CModeSub15 m_combo30.GetCurSel()+1; 1�� ���ؼ� ����Ѵ�.

    int m_nOpen_Current;            //son220706 m_nCC[6] -> m_nOpen_Current       
                                    //son m_combo28   Open  ��������.  // 1:20mA  2:10mA  3:5mA
                                    //    CModeSub15 m_combo28.GetCurSel()+1; 1�� ���ؼ� ����Ѵ�.
                                                            
                                    
    int m_nStep[10];    //son ������ �׽�Ʈ ��������Ʈ.  enum TEST_TYPE  ���
                        //son 0:skip, 1:open, 2:uShort LV, 3:Short LV, 4:HR Short(LV), 5:Leak1(HV), 6:Leak2(HV), 7:4W)
                        //
                        //son m_nStep[1]: m_combo32     // 0: Skip, 1: Open
                        //son m_nStep[2]: m_combo33     // 0: Skip, 1: uShort (LV), 2: Short (LV), 3: HR Short (LV), 
                        //       m_nStep[2]�� +1�ؼ� ���. 2: uShort (LV), 3: Short (LV), 4: HR Short (LV)
                        //
                        //son m_nStep[3]: m_combo34     // 0: Skip, 1: Leak1  (HV), 2: *Leak2  (HV)                  
                        //       m_nStep[3]�� +4�ؼ� ���: 5: Leak1  (HV), 6: *Leak2  (HV)
                        //
                        //son m_nStep[4]: m_combo35     // 0: Skip 1: uShort (LV), 2: Short (LV), 3: HR Short (LV), 4: Leak1 (HV), 5: Leak2 (HV), 6: *4W
                        //       m_nStep[4]�� +1�ؼ� ���: 2: uShort (LV), 3: Short (LV), 4: HR Short (LV), 5: Leak1 (HV), 6: Leak2 (HV), 7: *4W
                        //
                        //son m_nStep[5]: m_combo36     // 0: Skip, 1: Open, 2: uShort (LV), 3: Short (LV), 4: HR Short (LV),  5: Leak1 (HV), 6: Leak2 (HV), 7: *4W
                        //son m_nStep[6]: m_combo37     // 0: Skip, 1: Open, 2: uShort (LV), 3: Short (LV), 4: HR Short (LV),  5: Leak1 (HV), 6: Leak2 (HV), 7: *4W
                        //son m_nStep[7]: m_combo38     // 0: Skip, 1: Open, 2: uShort (LV), 3: Short (LV), 4: HR Short (LV),  5: Leak1 (HV), 6: Leak2 (HV), 7: *4W
                        //son m_nStep[8]: m_combo39  :  ����� m_combo39 ������� 4�� ����
    int m_nMode1;  //CC  CV  
    int m_nOpenErrorLimit;
    int m_nOpenShortLimit;
    int m_nPer3;
    int m_nPer4;
    int m_nPer5;
    //sylee170915-1     int m_nDual;//sylee20120617
    int m_n4WErrPerLimit;   //sylee170915-1 

    int m_nProSpMode;//sylee131221      // Spark ����.  ������ 4 �� ���.   -> ����.
//sylee131221   int m_nOpRetry;

    int m_n4WSet1;      //son ModeSub15 m_combo40  // 0:Fast, 1:Normal, 2:Slow, 3:Fine   (enum _4W_SET_TYPE ���)
    int m_nHVCal1;
    int m_nCycle;
    int m_nOpenMea1;    //sylee211122 m_nUniversal->m_nOpenMea1
    int m_4W_Measure;   //sylee200520-4W��������
    int m_nSparkSet_B;//sylee200709-1-JTECH NEW_SPARK
    int m_nS6;
    int m_nS7;
    int m_nS8;
    int m_nS9;
    int m_nSparkSet;//sylee140430
    int m_nAC;          //son220613 
	int m_nSparkDetection[3]; //hdpark230331, 0:common, 1: leak1, 2: leak2
	                            //son    val   0: Voltage, 1:Current
	                            //son m_nSparkDetection[0] : Common Spark ���� ��� (Recipe > Spark Detection combo �ڽ�)
	                            //son m_nSparkDetection[1] : leak 1 Spark ���� ���
	                            //son m_nSparkDetection[2] : leak 2 Spark ���� ���
	                            
	int m_nSparkCurrentSet; //hdpark230223 //son Recipe > Spark I combo �ڽ�
	                                       //son  0: 1mA,  1: 2mA, 2: 4mA, 3: 6mA, 4: 10mA, 5:15mA, 6:20mA 
	double m_dSparkCurrentSet; //hdpark230223

};


//son210324: for TestConditionLog  (Samsung Electro-Mecha)
typedef struct _sTestConditionData 
{
    int  nType;                 // TEST_CONDI_TYPE  0:����, 1:����, 2:Recipe �Է°�
    char szName[40];            // TestCondition Name, C:\ACE500\Data\TestConditionSample.csv���� Read�ؼ� ���
    char szFixedVal[30];        // nType�� 1(����)�� ��� �� ���� TestConditionLog�� ��� 
                                // ������ value�� C:\ACE500\Data\TestConditionSample.csv���� Read�Ѵ�.
                                
    char* (*funcp)();           // char*�� ��ȯ����  �ִ� �Լ� ������  //son230707_2 .fp()-> .funpc()
                                //   nItemType�� 2(Recipe �Է°�)�� ��쿡 �ش� �迭�� �� �Լ��� ȣ���Ѵ�.
} sTestConditionData;

#define     MAX_TEST_CONDITION_ITEM     238 
extern sTestConditionData    g_testConditionLogData[MAX_TEST_CONDITION_ITEM];

//int  LoadTestConditionData();    FileSysInfo01.LoadTestConditionData()�� ȣ���Ѵ�.
//                                  //son 1. C:\ACE500\Data\TestConditionSample.csv���� nItemType, szName, szFixedVal�� read  
//                                  //    2.  (*fp)() �Լ� �����Ϳ� ������ �Լ��� ��ġ
//int   Write_TestCondionLog();      FileSysInfo01.Write_TestCondionLog()�� ȣ���Ѵ�.                                   
//                                  //son 1. ���� Recipe ��Ȳ�� Z:\\TestConditionLog.csv�� ����Ѵ�.
enum TEST_CONDI_TYPE
{
    TCONDI_TYPE_BLANK  = 0,
    TCONDI_TYPE_FIXED  = 1,
    TCONDI_TYPE_RECIPE = 2
};

//son type�� 2(Recipe�Է�)�� ��츦 ǥ���ϱ� ���� enum
enum TEST_RECIPE_CONDI
{
    TREC_CONDI_DATE_TIME     = 0,
    TREC_CONDI_DELAY         = 1,
    TREC_CONDI_CON_FAIL      = 2,
    TREC_CONDI_RECHECK       = 3,
    TREC_CONDI_CON_ERR       = 6,
    TREC_CONDI_CON_FAIL_CNT  = 8,

    TREC_CONDI_BLOCK_X       = 11,
    TREC_CONDI_BLOCK_Y       = 12,
    TREC_CONDI_DUT_CNT       = 13,

    TREC_CONDI_OPEN_R        = 15,
    TREC_CONDI_OPEN_V        = 17,
    TREC_CONDI_4W_TEST       = 79,
    TREC_CONDI_4W_MODE       = 81,

    TREC_CONDI_USHORT_TEST   = 118,
    TREC_CONDI_USHORT_R      = 119,

    TREC_CONDI_LSHORT_TEST   = 121,
    TREC_CONDI_LSHORT_R      = 122,

    TREC_CONDI_HRSHORT_TEST  = 127,
    TREC_CONDI_HRSHORT_R     = 128,

    TREC_CONDI_LEAK1_TEST          = 139,
    TREC_CONDI_LEAK1_SPARK_CHECK   = 140,
    TREC_CONDI_LEAK1_V_SPARK_CHECK = 142,
    TREC_CONDI_LEAK1_SPARK_V       = 143,
    TREC_CONDI_LEAK1_V             = 148,
    TREC_CONDI_LEAK1_R             = 149,

    TREC_CONDI_LEAK2_TEST          = 157,
    TREC_CONDI_LEAK2_V_SPARK_CHECK = 158,
    TREC_CONDI_LEAK2_SPARK_V       = 159,
    TREC_CONDI_LEAK2_V             = 164,
    TREC_CONDI_LEAK2_R             = 165
};

extern char* getStr_Blank();
extern char* getStr_DateTime();
extern char* getStr_SysRecipe_m_nDelay1();
extern char* getStr_SysRecipe_m_nConFail_ON();
extern char* getStr_SysRecipe_m_nReCh();
extern char* getStr_SysRecipe_m_nConEr();
extern char* getStr_SysRecipe_m_nConFail();
extern char* getStr_SysInfoBlock_m_nCol();
extern char* getStr_SysInfoBlock_m_nRow();
extern char* getStr_nDUTCnt();

extern int   getValOhm_SysRecipe_m_nCon();          //son for Open Test
extern char* getStrOhm_R(int nR_Ohm);
extern char* getStrOhm_R_2(int nR_Ohm);
extern char* getStr_SysRecipe_m_nCon();
extern char* getStr_SysRecipe_m_nOpenV();

extern int   getState_SysRecipe_TestModeEnable(int TestMode); //son for 4W Test
extern char* getStr_SysRecipe_4WTest_Enable();
extern char* getStr_SysRecipe_m_n4WSet1();

extern char* getStr_SysRecipe_uShortTest_Enable();            //son for uShort Test
extern int   getValOhm_SysRecipe_m_nShortR();
extern char* getStr_SysRecipe_m_nShortR();      

extern char* getStr_SysRecipe_LShortTest_Enable();            //son for L Short Test
extern int   getValOhm_SysRecipe_m_nShortR2();
extern char* getStr_SysRecipe_m_nShortR2();     

extern char* getStr_SysRecipe_HRShortTest_Enable();           //son for HR Short Test
extern int   getValOhm_SysRecipe_m_nHR();
extern char* getStr_SysRecipe_m_nHR();          

extern char* getStr_Leak1Test_Enable();             //son for Leak1 Test
extern int   getValOhm_SysRecipe_Leak1R();
extern char* getStr_SysRecipe_Leak1R();          
extern char* getStr_SysRecipe_SparkCheck();          
extern char* getStr_SysRecipe_SparkVoltage();
extern char* getStr_SysRecipe_Leak1Voltage();

extern char* getStr_Leak2Test_Enable();             //son for Leak2 Test
extern int   getValOhm_SysRecipe_Leak2R();
extern char* getStr_SysRecipe_Leak2R();          
extern char* getStr_SysRecipe_Leak2Voltage();

extern char* getStr_OpenCurrent(int nOpenVolt, int nOpenCurrent);   //son221227

/*
struct CRunInfo05  //SETUP Recipe
{
    int m_nPinM;
    int m_nCon;
    int m_nConU;
    int m_nConIs;
    int m_nMs;
    int m_nHv;  
    int m_nSp;
    int m_nPr;
    int m_nDeT;
    int m_nConEr;
    int m_nIsEr;
    int m_nConErSt;
    int m_nReCh;
};
 */
 

//son  "\\SETUP2\\BlockDis.ini" ����
//son  IDD_MODE_SUB19_PIECE_DIS  ���� 
//son  SysInfo19: Block�� row, Piece�� col�� �ϴ� ������...

struct CSysInfo19  //SETUP Cell(Block-Piece)
{
    int m_nCellX;           //son Block (PieceDisable Grid������ row=block, name �����ʿ�, m_nTotBlock)
    int m_nCellY;           //son Piece (PieceDisable Grid������ col=piece, name �����ʿ�, m_nTotPiece)
    int m_nCellTotal;       //son Block * Piece ���� 
    int m_nType;            //son ������ 1
    int m_nUse;             //son PieceDisable Use (Use�̸� 1) 
    //short m_nData[301][301];//sylee131227  block, piece//sylee150618 [blockcount][piececount]
    short m_nData[MAX_BLOCK+1][MAX_PIECE+1];//son220823  Block * Piece ������ data��.  

    int m_nCheck;//sylee131117  //son Disable�� block-piece cell�� �ִٰ� ����ϱ� ���� �뵵
//  short m_nData1[200][200];
};




struct CSysInfo20  //SETUP ErrCount
{
    int m_nUse;
    int m_nPass;
    int m_nOpen;
    int m_nShort;
    int m_nOpenShort;
    int m_nFail;
    int m_nTotle;
};



struct CSysInfoSer11  //SETUP Service  
{
    int m_nPriMode;
    int m_nBox;
    int m_nPinTot;
};


struct CSysInfoSer12  //SETUP Service 
{
//sylee180810-1-DUT16   int m_nSetMode;
    int m_nMutiNo;
//  int m_nPinTot;
    int m_nPin[200][5];
};


// m_nMode Type     //son211222
enum TESTMODE {
    MODE1_OPEN_1V           = 1,
    MODE_OPEN_START        = MODE1_OPEN_1V,
    MODE2_OPEN_1V           = 2,
    MODE6_OPEN_10V          = 6,
    MODE7_OPEN_10V          = 7,
    MODE8_OPEN_10V          = 8,
    MODE9_OPEN_10V          = 9,
    MODE10_OPEN_10V         = 10,

    MODE11_OPEN_20V         = 11,
    MODE12_OPEN_20V         = 12,
    MODE13_OPEN_20V         = 13,
    MODE14_OPEN_20V         = 14,
    MODE15_OPEN_20V         = 15,
    MODE16_OPEN_30V         = 16,
    MODE17_OPEN_30V         = 17,
    MODE18_OPEN_30V         = 18,
    MODE19_OPEN_30V         = 19,
    MODE20_OPEN_30V         = 20,

    MODE21_OPEN_40V         = 21,
    MODE22_OPEN_40V         = 22,
    MODE23_OPEN_40V         = 23,
    MODE24_OPEN_40V         = 24,
    MODE25_OPEN_40V         = 25,
    MODE26_OPEN_50V         = 26,
    MODE27_OPEN_50V         = 27,
    MODE28_OPEN_50V         = 28,
    MODE29_OPEN_50V         = 29,
    MODE30_OPEN_50V         = 30,

    MODE31_OPEN_60V         = 31,
    MODE32_OPEN_60V         = 32,
    MODE33_OPEN_60V         = 33,
    MODE34_OPEN_60V         = 34,
    MODE35_OPEN_60V         = 35,
    MODE36_OPEN_70V         = 36,
    MODE37_OPEN_70V         = 37,
    MODE38_OPEN_70V         = 38,
    MODE39_OPEN_70V         = 39,
    MODE40_OPEN_70V         = 40,

    MODE41_OPEN_80V         = 41,
    MODE42_OPEN_80V         = 42,
    MODE43_OPEN_80V         = 43,
    MODE44_OPEN_80V         = 44,
    MODE45_OPEN_80V         = 45,
    MODE46_OPEN_90V         = 46,
    MODE47_OPEN_90V         = 47,
    MODE48_OPEN_90V         = 48,
    MODE49_OPEN_90V         = 49,
    MODE50_OPEN_90V         = 50,

    MODE51_OPEN_100V        = 51,
    MODE52_OPEN_100V        = 52,
    MODE53_OPEN_100V        = 53,
    MODE54_OPEN_100V        = 54,
    MODE55_OPEN_100V        = 55,
    MODE56_OPEN_110V        = 56,
    MODE57_OPEN_110V        = 57,
    MODE58_OPEN_110V        = 58,
    MODE59_OPEN_110V        = 59,
    MODE60_OPEN_110V        = 60,

    MODE61_OPEN_120V        = 61,
    MODE62_OPEN_120V        = 62,
    MODE63_OPEN_120V        = 63,
    MODE64_OPEN_120V        = 64,
    MODE65_OPEN_120V        = 65,
    MODE66_OPEN_130V        = 66,
    MODE67_OPEN_130V        = 67,
    MODE68_OPEN_130V        = 68,
    MODE69_OPEN_130V        = 69,
    MODE70_OPEN_130V        = 70,

    MODE71_OPEN_140V        = 71,
    MODE72_OPEN_140V        = 72,
    MODE73_OPEN_140V        = 73,
    MODE74_OPEN_140V        = 74,   //son220429
    MODE75_OPEN_140V        = 75,   //son220429
    MODE76_OPEN_150V        = 76,
    MODE77_OPEN_150V        = 77,
    MODE78_OPEN_150V        = 78,
    MODE79_OPEN_150V        = 79,   //son220429
    MODE80_OPEN_150V        = 80,   //son220429

    MODE81_OPEN_160V        = 81,
    MODE82_OPEN_160V        = 82,
    MODE83_OPEN_160V        = 83,
    MODE84_OPEN_160V        = 84,   //son220429
    MODE85_OPEN_160V        = 85,   //son220429
    MODE86_OPEN_170V        = 86,
    MODE87_OPEN_170V        = 87,
    MODE88_OPEN_170V        = 88,
    MODE89_OPEN_170V        = 89,   //son220429
    MODE90_OPEN_170V        = 90,   

    MODE91_OPEN_180V        = 91,
    MODE92_OPEN_180V        = 92,
    MODE93_OPEN_180V        = 93,
    MODE94_OPEN_180V        = 94,   //son220429
    MODE95_OPEN_180V        = 95,   //son220429
    MODE96_OPEN_190V        = 96,
    MODE97_OPEN_190V        = 97,
    MODE98_OPEN_190V        = 98,
    MODE99_OPEN_190V        = 99,   //son220429
    MODE100_OPEN_190V       = 100,  //son220429

    MODE101_OPEN_200V       = 101,
    MODE102_OPEN_200V       = 102,
    MODE103_OPEN_200V       = 103,
    MODE104_OPEN_200V       = 104,  //son220429
    MODE105_OPEN_200V       = 105,  //son220429
    MODE106_OPEN_210V       = 106,
    MODE107_OPEN_210V       = 107,
    MODE108_OPEN_210V       = 108,
    MODE109_OPEN_210V       = 109,  //son220429
    MODE110_OPEN_210V       = 110,  //son220429

    MODE111_OPEN_220V       = 111,
    MODE112_OPEN_220V       = 112,
    MODE113_OPEN_220V       = 113,
    MODE114_OPEN_220V       = 114,  //son220429
    MODE115_OPEN_220V       = 115,  //son220429
    MODE116_OPEN_230V       = 116,
    MODE117_OPEN_230V       = 117,
    MODE118_OPEN_230V       = 118,
    MODE119_OPEN_230V       = 119,  //son220429
    MODE120_OPEN_230V       = 120,  //son220429

    MODE121_OPEN_240V       = 121,
    MODE122_OPEN_240V       = 122,
    MODE123_OPEN_240V       = 123,
    MODE124_OPEN_240V       = 124,  //son220429
    MODE125_OPEN_240V       = 125,  //son220429
    MODE126_OPEN_250V       = 126,
    MODE127_OPEN_250V       = 127,
    MODE128_OPEN_250V       = 128,
    MODE129_OPEN_250V       = 129,  //son220429
    MODE130_OPEN_250V       = 130,  //son220429

    MODE131_OPEN_260V       = 131,
    MODE132_OPEN_260V       = 132,
    MODE133_OPEN_260V       = 133,
    MODE134_OPEN_260V       = 134,  //son220429
    MODE135_OPEN_260V       = 135,  //son220429
    MODE136_OPEN_270V       = 136,
    MODE137_OPEN_270V       = 137,
    MODE138_OPEN_270V       = 138,
    MODE139_OPEN_270V       = 139,  //son220429
    MODE140_OPEN_270V       = 140,  //son220429

    MODE141_OPEN_280V       = 141,
    MODE142_OPEN_280V       = 142,
    MODE143_OPEN_280V       = 143,
    MODE144_OPEN_280V       = 144,  //son220429
    MODE145_OPEN_280V       = 145,  //son220429
    MODE146_OPEN_290V       = 146,
    MODE147_OPEN_290V       = 147,
    MODE148_OPEN_290V       = 148,
    MODE149_OPEN_290V       = 149,  //son220429
    MODE150_OPEN_290V       = 150,  //son220429

    MODE151_OPEN_300V       = 151,
    MODE152_OPEN_300V       = 152,
    MODE153_OPEN_300V       = 153,      
    MODE154_OPEN_300V       = 154,      
    MODE155_OPEN_300V       = 155,      //son OPEN   : 1 ~ 170

    MODE_OPEN_END           = 170,

    MODE_4W_START           = 171,
    MODE171_4W              = 171,      //son 4W     : 171 ~ 180 
    MODE172_4W              = 172,
    MODE173_4W              = 173,
    MODE174_4W              = 174,
    MODE175_4W              = 175,
    MODE176_4W_CV           = 176,
    MODE177_4W_CV           = 177,
    MODE178_4W_CV           = 178,
    MODE179_4W_CV           = 179,
    MODE_4W_END             = 180,


    MODE181_SELF_10V        = 181,
    MODE182_SELF_200V       = 182,

    MODE_SHORT_START        = 201,

    MODE205_uSHORT_1V       = 205,      //son  uShort   : 201 ~ 205
    MODE_uSHORT_END         = MODE205_uSHORT_1V,

    MODE206_HR_SHORT_10V    = 206,      //son  HR Short : 206 ~ 209 
    MODE207_HR_SHORT_10V    = 207,      //son  HR Short
    MODE_HR_SHORT_END       = 210,

    MODE211_LEAK_20V       = 211,       //son  LEAK     : 210 ~ 350  
    MODE212_LEAK_20V       = 212,
    MODE216_LEAK_30V       = 216,
    MODE217_LEAK_30V       = 217,

    MODE221_LEAK_40V       = 221,
    MODE222_LEAK_40V       = 222,
    MODE226_LEAK_50V       = 226,
    MODE227_LEAK_50V       = 227,

    MODE231_LEAK_60V       = 231,
    MODE232_LEAK_60V       = 232,
    MODE236_LEAK_70V       = 236,
    MODE237_LEAK_70V       = 237,

    MODE241_LEAK_80V       = 241,
    MODE242_LEAK_80V       = 242,
    MODE246_LEAK_90V       = 246,
    MODE247_LEAK_90V       = 247,

    MODE251_LEAK_100V      = 251,
    MODE252_LEAK_100V      = 252,
    MODE253_LEAK_100V_10mA = 253,
    MODE254_LEAK_100V_10mA = 254,
    MODE256_LEAK_110V      = 256,
    MODE257_LEAK_110V      = 257,
    MODE258_LEAK_110V_10mA = 258,
    MODE259_LEAK_110V_10mA = 259,

    MODE261_LEAK_120V      = 261,
    MODE262_LEAK_120V      = 262,
    MODE263_LEAK_120V_10mA = 263,
    MODE264_LEAK_120V_10mA = 264,
    MODE266_LEAK_130V      = 266,
    MODE267_LEAK_130V      = 267,
    MODE268_LEAK_130V_10mA = 268,
    MODE269_LEAK_130V_10mA = 269,

    MODE271_LEAK_140V      = 271,
    MODE272_LEAK_140V      = 272,
    MODE273_LEAK_140V_10mA = 273,
    MODE274_LEAK_140V_10mA = 274,
    MODE276_LEAK_150V      = 276,
    MODE277_LEAK_150V      = 277,
    MODE278_LEAK_150V_10mA = 278,
    MODE279_LEAK_150V_10mA = 279,

    MODE281_LEAK_160V      = 281,
    MODE282_LEAK_160V      = 282,
    MODE283_LEAK_160V_10mA = 283,
    MODE284_LEAK_160V_10mA = 284,
    MODE286_LEAK_170V      = 286,
    MODE287_LEAK_170V      = 287,
    MODE288_LEAK_170V_10mA = 288,
    MODE289_LEAK_170V_10mA = 289,

    MODE291_LEAK_180V      = 291,
    MODE292_LEAK_180V      = 292,
    MODE293_LEAK_180V_10mA = 293,
    MODE294_LEAK_180V_10mA = 294,
    MODE296_LEAK_190V      = 296,
    MODE297_LEAK_190V      = 297,
    MODE298_LEAK_190V_10mA = 298,
    MODE299_LEAK_190V_10mA = 299,

    MODE301_LEAK_200V      = 301,
    MODE302_LEAK_200V      = 302,
    MODE303_LEAK_200V_10mA = 303,
    MODE304_LEAK_200V_10mA = 304,
    MODE306_LEAK_210V      = 306,
    MODE307_LEAK_210V      = 307,

    MODE311_LEAK_220V      = 311,
    MODE312_LEAK_220V      = 312,
    MODE316_LEAK_230V      = 316,
    MODE317_LEAK_230V      = 317,

    MODE321_LEAK_240V      = 321,
    MODE322_LEAK_240V      = 322,
    MODE326_LEAK_250V      = 326,
    MODE327_LEAK_250V      = 327,

    MODE331_LEAK_260V      = 331,
    MODE332_LEAK_260V      = 332,
    MODE336_LEAK_270V      = 336,
    MODE337_LEAK_270V      = 337,

    MODE341_LEAK_280V      = 341,
    MODE342_LEAK_280V      = 342,
    MODE346_LEAK_290V      = 346,
    MODE347_LEAK_290V      = 347,

    MODE351_LEAK_300V      = 351,
    MODE352_LEAK_300V      = 352,
    MODE_LEAK_END          = MODE352_LEAK_300V,

	// hdpark230105 spark cal begin
	MODE_SPARK_START       = MODE_LEAK_END,
    MODE356_SPARK_50V      = 356,
    MODE357_SPARK_100V     = 357,
    MODE358_SPARK_150V     = 358,
    MODE359_SPARK_200V     = 359,
    MODE360_SPARK_250V     = 360,
	// hdpark230105 end
    MAX_TEST_MODE = 360
};

//son220407 moved from ModeSet12.h to FileSysInfo.h 
enum    PART_LOC {  PART_UPPER =0, 
                    PART_LOWER =1, 
                    MAX_PART   =2, 

                    PART_MIXED =2,  //son gdCal1[part][][][]���� ��� 0:upper, 1:lower, 2:mixed
					PART_SPARK =3,	// hdpark230105 spark cal
                    MAX_PART3  =4
};

//C:\ACE500\BDL\QC\RJigReadType.txt �׸��
#define  MAX_CAL_RLIST       128     //son220408

//son220407 �ű� �߰�
enum CAL_GRID_COL {
    CAL_USE       = 1,
    CAL_REF_R     = 2,      //son Real ohm R
    CAL_REF_RAWR  = 3,      //son Raw  ADC R
    CAL_TEST_AVG  = 4,      
    CAL_VC = 4,             //son220407 Grid������ 4�� Test Avg,  SysInfoSer13.m_nData[mode][4][Rlist]������ VC

    CAL_DIFF_RATE = 5,      
    CAL_I  = 5,             //son220407 Grid������ 5�� Diff Rate, SysInfoSer13.m_nData[mode][5][Rlist]������ I 

    CAL_TEST1     = 6,
    CAL_TEST2     = 7,
    CAL_TEST3     = 8,
    CAL_TEST4     = 9,
    CAL_TEST5     = 10,
    CAL_TEST6     = 11,
    CAL_TEST7     = 12,
    CAL_TEST8     = 13,
    CAL_TEST9     = 14,
    CAL_TEST10    = 15,
    CAL_GRID_COL_NUM    //16 
};


//son220411 ���, �Ϻ� Cal ���� ������ �߰� [MAX_PART] �� ����ȭ.

extern double gdCal1[MAX_PART3][MAX_TEST_MODE+2][5][150];//son211222  362: MAX_TEST_MODE+2   //son220411
extern double gdCal2[MAX_PART3][3][5][150];             //hdpark221005 self test judge mode
extern double dRec[MAX_TEST_MODE+2][16];                //son211222  362: MAX_TEST_MODE+2

struct CSysInfoSer13  //Cal Service  
{

    //son Calibration ��� ���� buffer
    //son  ex) "C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI" ���� m_nMode1�� Calibration Ref �� Load
    //         SysInfoSer13.m_nData   1~128(MAX_CAL_RList1)�� 1���� ����Ѵ�. 
    double m_nData[MAX_PART3][MAX_TEST_MODE +1][20][MAX_CAL_RLIST +2];   //son220411 // hdpark230105 spark cal
                              //son m_nData[part][���и��][0][1]    : TimeDelay (dRec[m_nMode1][6]*10 �� �Է�)
                              //son m_nData[part][���и��][1:CAL_USE    ][RList1] : Grid 'Use =1' 
                              //son m_nData[part][���и��][2:CAL_REF_R  ][RList1] : Grid 'Ref.R'  : ohm realR
                              //son m_nData[part][���и��][3:CAL_REF_RAWR][RList1] : Grid 'AVG R'  : ADC rawR 
                              //son m_nData[part][���и��][4:CAL_VC     ][RList1] : dVC1[0][RList] 
                              //son m_nData[part][���и��][5:CAL_I      ][RList1] : dI1[0][RList] 
                                    
    //'Use=1'�� ��츸 ������ SysInfoSer13.m_nData[mode][1~3][RList: 1~128]
    //double gdCal1[MAX_PART][MAX_TEST_MODE+2][5][150];  //global variable  //���� mode, row(�ϱ� ����), col(net. PinSet�� ��ȣ)
    //                       //son gdCal1[part][m_nMode1][0][0]                  Use=1�� Cal list ����
    //                       //son gdCal1[part][m_nMode1][1:CAL_USE    ][RList1]  cal enable 1,0 
    //                       //son gdCal1[part][m_nMode1][2:CAL_REF_R  ][RList1]  cal ref realR (ohm)
    //                       //son gdCal1[part][m_nMode1][3:CAL_REF_RAWR][RList1]  cal Avg rawR (ADC)                                            
                                
};


//for SysSet12.m_nPinTotMax  
enum PIN_TOTMAX {       //son-32KPIN-20190902:  enum �ű� �߰�
    PIN_TOT_NOT_SET               = 0,  //son <Not Set>
    PIN_TOT_4K                    = 1,  //son U: 2K,        L: 2K    ( 4096 Point)    // Upper SD 1K 2 box Lower SD 1K 2 box (S100-1)
    PIN_TOT_8K                    = 2,  //son U: 4K,        L: 4K    ( 8192 Point)    // Upper DD 2K 2 box Lower DD 2K 2 box
    PIN_TOT_16K                   = 3,  //son U: 8K,        L: 8K    (16384 Point)    // Upper QD 4K 2 box Lower QD 4K 2 box
    PIN_TOT_32K_TRANS             = 4,  //son U: 16K_Trans, L: 16K_Transpose, Transpose�� 16K +16K�� ���� ���� Card ����
    PIN_TOT_20K_UP_TR             = 5,  //son U: 16K_Trans, L: 4K org    //son230216_1
    PIN_TOT_24K_UP_TR             = 6,  //son U: 16K_Trans, L: 8K org    //son230216_1
    PIN_TOT_24K_TRANS             = 7,  //son U: 16K_Trans, L: 8K_Trans  //son230216_1
    PIN_TOT_16K_TRANS             = 8,  //son U: 8K_Trans,  L: 8K_Trans  //son230216_1
    PIN_TOT_8K_LOW_STPIN          = 9,  //son U: 4K,        L: 4K org, Use LowerStartPin (HW�� Upper 8K, Lower 8K.  DD 4097 ���� ���)
    PIN_TOT_12K                   = 10, //son U: 8K,        L: 4K org, Use LowerStartPin  //son210202: 12K ���� �߰�
    PIN_TOT_12K_TRANS             = 11, //son U: 8K_Trans,  L: 4K_Transpose //son210319 //son230216_1
    PIN_TOT_8K_TRANS              = 12, //son U: 4K_Trans,  L: 4K_Transpose //son210319 //son230216_1

    PIN_TOT_16K_U12K_L4K_TRANS    = 13, //son U: 12K_Trans, L: 4K_Transpose //son210525 MaxPin 16K, �Ϻν����� QD 12289 ���� (�̻��)
    PIN_TOT_16K_U12K_L4K_UP_TRANS = 14, //son U: 12K_Trans, L: 4K Origin    //son210531 MaxPin 16K, �Ϻν����� QD 12289 ���� (�̻��)
    PIN_TOT_20K_U12K_L4K_UP_TRANS = 15, //son U: 12K_Trans, L: 4K Origin    //son210603 MaxPin 20K. �Ϻν����� QD 16385 ����
                                        //son U: 8K_Trans,  L: 4K Origin   
                                        //      12K Upper Trans���� ����! PIN_TOT_20K_UP_TR(UpperTras)�� QD���� �Ϻν����� 8193���� �ؼ� ����Ѵ�.
    PIN_TOT_20K_U8K_L4K_UP_TRANS  = 16,  //son U: 8K_Trans, L: 4K Origin      //son210826 MaxPin 20K. �Ϻν����� QD 16385 ����
    PIN_TOT_U8K_UPPER_ONLY        = 17,  //son U: 8K Origin L: Not provieded  //son211020 ���� 8K UpperOnly (���� 16K���� �Ϻ� ���� ���̽�) 
    PIN_TOT_32K_U8K_L24K          = 18,  //son U: 8K Org    L: 24K Org      //son230216_1 K_600 �븸 ������ 
    PIN_TOT_32K_ORG               = 19,  //son U: 16K Org   L: 16K Org      //son230216_1 32K Org �ű� Ÿ�� 
    PIN_TOT_20K_U12K_L8K_TRANS    = 20,  //son U: 12K_Trans L: 8K_Trans     //son230714  for GTS,  �Ϻν����� QD 12289 ����  (�̻��)
    PIN_TOT_24K_U12K_L8K_TRANS    = 21,  //son U: 12K_Trans L: 8K_Trans     //son230718  for GTS,  �Ϻν����� QD 16385 ���� 

    PIN_TOT_END  = 22 
};

//son for m_combo1: son-8K4097-20200605 : �ű� �߰� //son230216_1 //son230714 //son230718
#define    STR_MAX_PIN(nPinTotMax)    ((nPinTotMax == PIN_TOT_NOT_SET)               ? "<Not Set>" : \
                                       (nPinTotMax == PIN_TOT_4K)                    ? "4K_Point": \
                                       (nPinTotMax == PIN_TOT_8K)                    ? "8K_Point": \
                                       (nPinTotMax == PIN_TOT_16K)                   ? "16K_Point": \
                                       (nPinTotMax == PIN_TOT_32K_TRANS)             ? "32K_Point(Trans)": \
                                       (nPinTotMax == PIN_TOT_20K_UP_TR)             ? "20K_Point(UpperTrans)": \
                                       (nPinTotMax == PIN_TOT_24K_UP_TR)             ? "24K_Point(UpperTrans)": \
                                       (nPinTotMax == PIN_TOT_24K_TRANS)             ? "24K_Point(Trans)": \
                                       (nPinTotMax == PIN_TOT_16K_TRANS)             ? "16K_Point(Trans)": \
                                       (nPinTotMax == PIN_TOT_8K_LOW_STPIN)          ? "8K_Point(LowerStartPin)": \
                                       (nPinTotMax == PIN_TOT_12K)                   ? "12K_Point(LowerStartPin)" : \
                                       (nPinTotMax == PIN_TOT_12K_TRANS)             ? "12K_Point(Trans)" : \
                                       (nPinTotMax == PIN_TOT_8K_TRANS)              ? "8K_Point(Trans)" : \
                                       (nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS)    ? "16K_U_12K,L_4K(Trans)" : \
                                       (nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS) ? "16K_U_12K,L_4K(UpperTrans)" : \
                                       (nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS) ? "20K_U_12K,L_4K(UpperTrans)" : \
                                       (nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS)  ? "20K_U_8K,L_4K(UpperTrans)" : \
                                       (nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)        ? "8K_Point(UpperOnly)" : \
                                       (nPinTotMax == PIN_TOT_32K_U8K_L24K)          ? "32K_U_8K,L_24K" : \
                                       (nPinTotMax == PIN_TOT_32K_ORG)               ? "32K_Point" : \
                                       (nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS)    ? "20K_U_12K,L_8K(Trans)" : \
                                       (nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)    ? "24K_U_12K,L_8K(Trans)" : "UNDEF_MaxPin")


//son210525  //son230216_1 
#define   IS_UPPER_16K(nPinTotMax)    (nPinTotMax == PIN_TOT_32K_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_TRANS)

//son ��� 3box ���̽�  //son210531 //son230714 //son230718
#define   IS_UPPER_12K(nPinTotMax)    (nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS \
                                       || nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)

//son230718 ��� 3box (�Ϻν������� 16385���� �ϴ� ��� ����)
//    PinBlockRange�� ���������� 4��° box���� �Ҵ��ϰ� �ϱ� ���� �� define�� ����Ѵ�.
//    PIN_TOT_20K_U12K_L4K_UP_TRANS : Upper 12K���� 20K BDL�̿� (�Ϻν����� 16385) //son210603
//    PIN_TOT_24K_U12K_L8K_TRANS    : Upper 12K���� 24K BDL�̿� (�Ϻν����� 16385) //son230718
#define   IS_UPPER_12K_2(nPinTotMax)    (IS_UPPER_12K(nPinTotMax) \
                                            && nPinTotMax != PIN_TOT_20K_U12K_L4K_UP_TRANS \
                                            && nPinTotMax != PIN_TOT_24K_U12K_L8K_TRANS)

//son ��� 2box ���̽�  //son210514 //son230216_1 
#define   IS_UPPER_8K(nPinTotMax)     ( nPinTotMax == PIN_TOT_16K_TRANS \
                                       || nPinTotMax == PIN_TOT_12K \
                                       || nPinTotMax == PIN_TOT_12K_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)
                                      // || nPinTotMax == PIN_TOT_32K_U8K_L24K )

//son230718 ��� 2box (�Ϻν������� 16385���� �ϴ� ��� ����)
//    PinBlockRange�� ���������� 3,4��° box���� �Ҵ��ϰ� �ϱ� ���� �� define�� ����Ѵ�.
//    PIN_TOT_20K_U8K_L4K_UP_TRANS : Upper 8K���� 20K Bdl �̿�(�Ϻν����� 16385)   //son210826
#define IS_UPPER_8K_2(nPinTotMax)       (IS_UPPER_8K(nPinTotMax) \
                                            && SysSet12.m_nPinTotMax != PIN_TOT_20K_U8K_L4K_UP_TRANS)

//son211020 �Ϻ� ���� pinMode 
#define   IS_NO_LOWER(nPinTotMax)     (nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)

//son230216_1
#define   IS_UPPER_4K(nPinTotMax)     ( nPinTotMax == PIN_TOT_8K_LOW_STPIN \
                                       || nPinTotMax == PIN_TOT_8K_TRANS)
//son230216_1   //son230714 //son230718
#define   IS_LOWER_8K(nPinTotMax)     (nPinTotMax == PIN_TOT_24K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS \
                                       || nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS)

//son230216_1
#define   IS_LOWER_4K(nPinTotMax)     ( nPinTotMax == PIN_TOT_20K_UP_TR \
                                       || nPinTotMax == PIN_TOT_8K_LOW_STPIN \
                                       || nPinTotMax == PIN_TOT_12K \
                                       || nPinTotMax == PIN_TOT_12K_TRANS \
                                       || nPinTotMax == PIN_TOT_8K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS )

//son210531 ��θ� Trans, �Ϻδ� ������ ���.   //son230216_1
#define   IS_UPPER_TRANS(nPinTotMax)  (nPinTotMax == PIN_TOT_20K_UP_TR \
                                            || nPinTotMax == PIN_TOT_24K_UP_TR \
                                            || nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS \
                                            || nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS \
                                            || nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS )


//son210319 : ���� 32K base TRANS Pin Mode���� Ȯ��. if ������ ��� //son230216_1   //son230714 //son230718
#define   IS_TRANS_TOTMAX(nPinTotMax)  (nPinTotMax == PIN_TOT_32K_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_TRANS \
                                       || nPinTotMax == PIN_TOT_12K_TRANS \
                                       || nPinTotMax == PIN_TOT_8K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS \
                                       || nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS )


//son210202 �ű�  nPinTotMax Ÿ������ �Ǵ��� �ش�. if �� �ȿ��� ����ؾ� ��.
//  32K ����PinBlock, 8K_LOWST, 12K �� sonjiwon�� ������ �ű� PinTotMax Ÿ���� ��� 
//  ������ �ڵ带 ����ϱ� ���� �뵵�� ���ȴ�.  �Ϻν����� �޺��ڽ��� �ʿ�ÿ��� ���.
//  PinBlockType�� PinTotMax�� ��� �ű�Mode���� Ȯ��.      //son230216_1
#define    IS_NEW_PIN_TOTMAX2(nPinBlockType, nPinTotMax) \
                             ( (nPinBlockType == PIN_BLK_32K_TRANS && IS_TRANS_TOTMAX(nPinTotMax)) \
                            || (nPinBlockType == PIN_BLK_QD  && (nPinTotMax == PIN_TOT_12K \
                                                              || nPinTotMax == PIN_TOT_8K_LOW_STPIN \
                                                              || nPinTotMax == PIN_TOT_U8K_UPPER_ONLY)) \
                            || (nPinBlockType == PIN_BLK_32K_ORG && (nPinTotMax == PIN_TOT_32K_U8K_L24K \
                                                                  || nPinTotMax == PIN_TOT_32K_ORG)) )

//son210202  2019�� 6�� ���� �ű� PinMode ���� Ȯ��. if������ ���. PinTotMax ������ Ȯ��.  //son230216_1 //son230714 //son230718
#define   IS_NEW_PIN_TOTMAX(nPinTotMax)  (nPinTotMax == PIN_TOT_32K_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_UP_TR \
                                       || nPinTotMax == PIN_TOT_24K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_TRANS \
                                       || nPinTotMax == PIN_TOT_8K_LOW_STPIN   \
                                       || nPinTotMax == PIN_TOT_12K \
                                       || nPinTotMax == PIN_TOT_12K_TRANS \
                                       || nPinTotMax == PIN_TOT_8K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_TRANS \
                                       || nPinTotMax == PIN_TOT_16K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_20K_U8K_L4K_UP_TRANS \
                                       || nPinTotMax == PIN_TOT_U8K_UPPER_ONLY \
                                       || nPinTotMax == PIN_TOT_32K_U8K_L24K \
                                       || nPinTotMax == PIN_TOT_32K_ORG \
                                       || nPinTotMax == PIN_TOT_20K_U12K_L8K_TRANS \
                                       || nPinTotMax == PIN_TOT_24K_U12K_L8K_TRANS) 


// Define pin counts for each type
#define QD_PINS 4096        //son240522
#define DD_PINS 2048        //son240522
#define SD_PINS 1024        //son240522


enum PIN_BOXTYPE {      //son190902-32KPIN:  enum �ű� �߰�
    BOX_QD      = 1,        
    BOX_DD      = 2,        
    BOX_SD      = 3,
    BOX_SKIP    = 4,   //son241007-BSW-9 : for upper3, upper4
    MAX_BOXTYPE = 4
};

//son for m_combo1: son200605-8K4097 : �ű� �߰�
#define    STR_BOXQD_TYPE(nBoxQdType)  ((nBoxQdType == BOX_QD) ? "QD": \
                                        (nBoxQdType == BOX_DD) ? "DD": \
                                        (nBoxQdType == BOX_SD) ? "SD": \
                                        (nBoxQdType == BOX_SKIP) ? "SKIP": "X")

//son230222 //extern short PinMap[7][Def_SwitchMaxPin];//nSwitchMaxPinsylee20120617    [0]= NET PIN, [1]= QD PIN, [2]= SWTICH PIN, 
extern int PinMap[7][Def_SwitchMaxPin];//son230222 short -> int :for U8K, L24K. �Ϻΰ� 24K ���� pin���� �Ͻ������� 32768�� ���� �� ����
//son PinMap[0][] : net list pin no
//son PinMap[1][] : QD pin no  -> ���� �̻��
//son PinMap[2][] : SWITCH CARD pin no
//son PinMap[3][] : PinBlock No(1~8)                      //son210514
//son PinMap[4][] : PinBlockType(BOX_QD/BOX_DD/BOX_SD)    //son210514
//son PinMap[5][] : PinBlock HwBoxType(TYPE_AC/TYPE_BD)   //son210514
//son PinMap[6][] : GetBlockNo_PinNo()�� �޾ƿ� nPin1(QdJigPin) //son210514

extern int nJigPinMap[MAX_PIN+1];     //son index:hwPin, val:jigPin.    //son230303       

//son Setup-> Pin ���� �޴� ����   ( from C:\ACE400\SETUP\2\SysSetPin.ini)
//son IDD_MODE_SET12_PIN Dialog ���� (PIN Connection)
struct CSysSet12  //Pin SETUP  
{
    //-------------------------------------------------
    //son FilePathBDL+ "\\SETUP2\\SysSetPin.ini" �� ����
	int m_nPinQDUpper;					//son Upper1 BoxType: 1=QD, 2=DD, 3=SD	 enum PIN_BOXTYPE
	int m_nPinQDLower;					//son Lower1 BoxType: 1=QD, 2=DD, 3=SD
	int m_nPinQDUpper2;					//son Upper2 BoxType: 1=QD, 2=DD, 3=SD
	int m_nPinQDLower2;					//son Lower2 BoxType: 1=QD, 2=DD, 3=SD
	int m_nPinQDUpper3;	//sylee210330-1			 
	int m_nPinQDLower3; //sylee210330-1					 
	int m_nPinQDUpper4;	//sylee210330-1				 
	int m_nPinQDLower4;	//sylee210330-1					 
    int m_nPinQDLower5; //son230217 K600 �Ϻ� 24K (PIN_TOT_32K_U8K_L24K) �� ���� �߰�
    int m_nPinQDLower6; //son230217

    //-------------------------------------------------
    //son "C:\\ACE1000\\SETUP2\\SysSetPin.ini" �� ����
	int m_nPinTotMax;  		// m_combo1		//son fileLine 1   PIN_TOTMAX enum ���
	int m_nPinBoxMax;		// m_combo2		//son fileLine 2   1:2K, 2:4K BoxType�� �ǹ� ex) 1 => nBoxPinFull = 2048,  
	int m_nPinSwitchMode;  	// m_combo3		//son fileLine 3   1:Z,  2:H                ex) 2
	int m_nPinType;			// m_combo4 	//son fileLine 4   1:A, 2:B, 3:C, 4:D, 5:E  ex) 2
	int m_nCardReverse;		// m_combo5		//son fileLine 5   1:Normal, 2:ReverseA     ex) 1
	int m_nBlockSet1;	 //sylee210403 m_check4	  fileLine 6   1:Manual ��Ϻ�QD/DD/SD  ex) 0
	int m_nCardReverseB;	// m_combo8		//son fileLine 7   1:Normal, 2:ReverseB     ex) 1

    //-----------------------------------------------------
	//son FilePathBDL+ "\\SETUP2\\LowerStartPin.ini"�� ����
    int m_nLowerJigStartPin;//sylee150917 	//son fileLine 8                            ex) 0
    int m_nEndPin;          //son240516 	//son fileLine 9   32K_ORG�� ������ ����ϴ� ����. ex) 24576, 32768       
    int m_nUpperBox;        //son240516     //son fileLine 10  ��� �ڽ� ���� ex) 1~ 4 
    int m_nLowerBox;        //son240516     //son fileLine 11  �Ϻ� �ڽ� ���� ex) 1~ 4 
    int m_nLowerJigStartPin_Offset; //son241007-BSW-9  nBlockSet1(QD/DD/SD ȥ��) �� ������� ������ m_nLowerJigStartPin
                                    //         �� user�� �߰��� �� offset�� �� ��쿡 ����Ѵ�. m_nEndPin�� offset��ŭ ���� �и���.
                                    //         �� ���� 0�̸� ���� m_nLowerJigStartPin�� MakePinBlockRangeUserNo() �Լ����� ������� �����ϰ�
                                    //         �� ���� 0 �̻��̸� m_nLowerJigStartPin�� MakePinBlockRangeUserNo() ���ΰ���� + Offset ���̴�.
};


//for SysSet13.m_nPinBlockType  (���� m_nSet101 )
//son  SD     : box  1024 pin(16 card *  64 pin),   
//     DD     : box  2048 pin(32 card *  64 pin),   
//     QD     : box  4096 pin(32 card * 128 pin),
//  S100-1    : box  1024 pin(16 card *  64 pin)  : PinBlock Type���θ� ����ó��
//
//                         ���                                �Ϻ�                       ���card �Ϻ�card  ��card
//         4K SD  :    2K SD(2box * 1K(16 card * 64 pin))       2K SD                        32      32      64 
//         8K DD  :    4K DD(2box * 2K(32 card * 64 pin))       4K DD                        64      64      128 
//        16K QD  :    8K QD(2box * 4K(32 card *128 pin))       8K QD                        64      64      128
//        32K QD  :   16K QD(4box * 4K(32 card *128 pin))      16K QD                       128     128      256
//       12K QD/DD:    8K QD(2box * 4K(32 card *128 pin))       4K DD(2box *32card * 64pin)  64      64      128
//     S100-1(4K) :2K S100-1(2box * 1K(16 card * 64 pin))       2K SD                        32      32      64 
//        20K QD  :   16K QD(4box * 4K(32 card *128 pin))       4K QD(1box *32card *128pin) 128      32      160
//        24K QD  :   16K QD(4box * 4K(32 card *128 pin)        8K QD(2box *32card *128pin) 128      64      192 


enum ePIN_BLOCK_TYPE {       //son-32KPIN-20190902:  enum �ű� �߰�
    PIN_BLK_QD          = 0,        //son QD        Max 16K ����: Upper QD 2��(4K *2), Lower QD 2�� (4K *2)
    PIN_BLK_DD          = 1,        //son DD        Max 8K : Upper DD 2��(2K *2), Lower DD 2��(2K *2)
    PIN_BLK_SD          = 2,        //son SD        Max 4K : Upper SD 2��(2K *1), Lower SD 2��(2K *1)
    PIN_BLK_S100_1      = 3,        //son S100-1    Max 4K : Upper (64pin*16:1K) 2��(2K), Lower (64*16:1K) 2��(2K)
    PIN_BLK_S100_2_WIRE = 4,        //son DD        //son230216_1
    PIN_BLK_YAMAHA      = 5,        //son DD
    PIN_BLK_4K_DD_421   = 6,        //son DD        Max 8K : Upper DD 2��(2K *2), Lower DD 2��(2K *2)
    PIN_BLK_12K_QD_DD   = 7,        //son QD / DD   Max 12K: Upper QD 2��(4K *2), Lower DD 2��(2K *2)
    PIN_BLK_32K_TRANS   = 8,        //son QD        Max 32K: Upper QD 4��(4K *4), Lower QD 4��(4K *4)   
                                                    //son230216_1   PIN_BLK_32K -> PIN_BLK_32K_TRANS�� ����.  
    PIN_BLK_32K_ORG     = 9,        //son QD        Max 32K: No trans 32K Upper QD 4K *4, Lower QD 4K *4 //son230216_1
    MAX_PIN_BLK         = 10        //son230216_1
};

//son for m_combo1: son-8K4097-20200605 //son230216_1
#define STR_PIN_BLOCK(nPinBlockType)   ((nPinBlockType == PIN_BLK_QD)         ? "QD": \
                                        (nPinBlockType == PIN_BLK_DD)         ? "DD": \
                                        (nPinBlockType == PIN_BLK_SD)         ? "SD": \
                                        (nPinBlockType == PIN_BLK_S100_1)     ? "S100-1": \
                                        (nPinBlockType == PIN_BLK_S100_2_WIRE)? "S100-2 Wire": \
                                        (nPinBlockType == PIN_BLK_YAMAHA)     ? "YAMAHA": \
                                        (nPinBlockType == PIN_BLK_4K_DD_421)  ? "4K_DD_421": \
                                        (nPinBlockType == PIN_BLK_12K_QD_DD)  ? "12K (QD_DD)": \
                                        (nPinBlockType == PIN_BLK_32K_TRANS)  ? "32K (Trans)": \
                                        (nPinBlockType == PIN_BLK_32K_ORG)    ? "32K (Original)": "UNDEF_PinBlock")




//-----------------------------
// BDL Data ���� define, enum

//son200722-BDL-Sort: �ű��߰�
enum eBDL_SORT_MODE {
    NO_SORT           = 0,
    SORT_MODE_DESCEND = 1,
    SORT_MODE_ASCEND  = 2,
    SORT_MODE_END = 3
};

// ����  combo box�� ������ Ŭ������ �̿��ؼ� �ڵ����� �ʱ�ȭ�ϴ� ��� �߰���.
class CStrList_BdlSort
{
public:
    CString     strList[SORT_MODE_END+1]; //son Access Violation ������ SORT_MODE_END+1 �� �غ�

public:
    CStrList_BdlSort()      // Constructor: ������
    {
        strList[NO_SORT]                = "No Sort";                   
        strList[SORT_MODE_DESCEND]      = "Descending Sort";           
        strList[SORT_MODE_ASCEND]       = "Ascending Sort";            
    }
    
}; 


//son200722-BDL-Sort: �ű��߰�
// BDL Data ���� define, enum
enum eBDLDATA {
    BDL_PIN0       = 1,
    BDL_NET        = 2,
    BDL_RESERVED1  = 3,
    BDL_RESERVED2  = 4,
    BDL_NET_PINCNT = 5,
    BDL_RESERVED3  = 6,
    BDL_PIECE      = 7
};


enum SPARK_MODE {
    SPARK_NEW = 0,
    SPARK_OLD1 = 1,
    SPARK_OLD2 = 2
};

enum _4W_LOG_MODE {
    _4W_LOG_COL               = 0,
    _4W_LOG_LINE_DUT_PIECE    = 1, //son230323 piece �÷� �߰����� �̸� ����. _4W_LOG_LINE ->_4W_LOG_LINE_PIECE
                                   //son230704 _4W_LOG_LINE_PIECE -> _4W_LOG_LINE_DUT_PIECE.  �������̸����� ����
                                
    _4W_LOG_LINE_DUT          = 2, //son220304 1�� DUT ���Ͽ� All Piece�� ��� write�ϴ� ��� �߰�.
                                   //son230704 _4W_LOG_LINE_ALL_PIECE -> _4W_LOG_LINE_DUT.  �������̸����� ����
                                
    _4W_LOG_COL_V2_MST        = 3, //son230707 MST ������ 4W Csv. READ���� detail�� 4w csv�α׸� �� ��ϸ��� ���.                                  

    _4W_LOG_LINE_DUT_V2_NANO  = 4, //son231026 1�� DUT ���Ͽ� All Piece�� ��� write�ϴ� ����ý� ���� 4W CSv 
    _4W_LOG_MODE_NUM          = 5  //son231026
};

//son230707 begin: 
#define STR_4W_LOG_MODE(n4wLogMode) ((n4wLogMode == _4W_LOG_COL)              ? "Column(Original)": \
                                     (n4wLogMode == _4W_LOG_LINE_DUT_PIECE)   ? "Line_DUT_Piece": \
                                     (n4wLogMode == _4W_LOG_LINE_DUT)         ? "Line_DUT": \
                                     (n4wLogMode == _4W_LOG_COL_V2_MST)       ? "Column_V2": \
                                     (n4wLogMode == _4W_LOG_LINE_DUT_V2_NANO) ? "Line_DUT_V2": "Undef_4wLogMode")
//son230707 end 


enum eUseCalData {              //son220419
    USECAL_UPPER_ONLY  = 0,
    USECAL_UPPER_LOWER = 1,
};


//son230106_2 begin:   
//son Used for    SysSet13.m_n4wFailSortMode
enum e4W_FAIL_SORT_MODE {
    F_4W_SORT_HIGH_OVERRATE = 0,
    F_4W_SORT_NET_NO  = 1,
    F_4W_SORT_MODE_END = 2
};

//son IDC_COMBO115�� ������ Ŭ������ �̿��ؼ� �ڵ����� �ʱ�ȭ�ϴ� ��� �߰���.
class CStrList_4wFailSort
{
public:
    CString     strList[F_4W_SORT_MODE_END+1]; //son Access Violation ������ SORT_MODE_END+1 �� �غ�

public:
    CStrList_4wFailSort()      // Constructor: ������
    {
        strList[F_4W_SORT_HIGH_OVERRATE] = "RefHigh OverRate";                   
        strList[F_4W_SORT_NET_NO]         = "4w Net No";           
    }
    
}; 
//son230106_2 end


//son C:\\ACE400\\SETUP\\SysSet01.ini
//son Map > Set1:  IDD_MODE_SET13_SET1 Dialog ����
struct CSysSet13  // SETUP  
{
    int m_nSet1;            //son Check1 : Current 20mA 10mA 5mA Enable         Recipeȭ�� ����������� ON(for ��������,����ý�, BH)
    int m_nSet2;            //son Check2 : Z:\ LogSheet4W.CSV
    int m_nSet3;            //son Check3 : DUT16 Use Z:\ Dut.ini
    int m_nSet4;            //son Check4 : Selftest System Off                  SystemSelfCheckUse
    int m_nSet5;            //son Check5 : * Engneer => 4W V I File             4W_Setup_A_Votage.txt, 4W_Setup_A_Current.txt ���  
    int m_nSet6;            //son Check6 : No Password                          NoPassword
    int m_nSet7;            //son Check7 : Err file TXT->EXCEL                  CsvFileUse
    int m_nSet8;            //son Check8 : *  200M  L                     
    int m_nSet9;            //son Check9 : 4W Error Correction Mode1            �߾Ӱ� ����  20% ����� ���� ������ ������ ����
                            
    int m_nShortDetailOff;  //son Check10: S K U  H L Z=> S K U                 //son220318_2 m_nSet10 ->m_nShortDetailOff
    int m_nSet11;           //son Check11: OPEN 200V                            UseOpenTestVoltLimit
    int m_nSet12;           //son Check12: Sheet Retest -> Disable              DisableSheetRetest  
    int m_nSet13;           //son Check13: Open R > 1K Read Enable              ����
    int m_nSet14;           //son Check14: Error Pin Real R. Display            ����. EnableErrPinRVal
    int m_nSet15;           //son Check15: Error file -> Refer.value Disable    ����
    int m_nSet16;           //son Check16: LV Short Use                         ����   (HV Leak1 �׽�Ʈ�ÿ� LV -> Leak1�� �����ϴ� �ɼ�)
    int m_nSet17;           //son Check17: Recipe Password Use                  UseRecipePassword
    int m_nSet18;           //son Check18: * 4W MANUAL -ABORT DIO 
    int m_nSet19;           //son Check19: Recipe Retest Password use           UseeRecipeRetestPasswd
    int m_nSet20;           //son Check20: Circuit Engineer -> Interlock.Disable    HW��. DisableInterLock  -> ini���Ϸ� �̵�
    
    int m_nPinBlockType;    //sylee161108-1 //son-32KPIN-20190902: m_nSet101->m_nPinBlockType ���� ����. 
                            //son ComboBox: "Pin Block"    PIN_BLOCK_TYPE enum ���. 
                            // 32K,  16K, DD (���������� 8K, 4K, ...)  => ���� ��带 �̷��� �ܼ�ȭ�Ѵ�.
                            
    int m_nBdlSortMode;     //son200722-BDL-Sort 0:NoSort,  1:Descendig Sort,  2: Ascending Sort
                            //son200722-BDL-Sort m_nSet102-> m_nBdlSortMode�� ����.
    int m_nUseCalData;      //son220419  
    int m_nSet104; 
    int m_nSet105; 
    int m_n4WCVMode;        //sylee200515-4WCVMODE       //son 0:CC    1:CV
    int m_nSparkMode;       //son200908 SPARK_MODE enum   0:SPARK_NEW, 1:SPARK_OLD1,  2:SPARK_OLD2
    int m_n4wLogMode;       //son201209 _4W_LOG_MODE enum 0:Column(Vertical), 1:Line(Horizontal)
    int m_nSet114;
    int m_n4wFailSortMode;  //son230106_2 COMBO115  4W Fail AutoTest ��� Sort ���  0:RefHigh OverRate  1: 4w NetNo         
    int m_nSetUltLog;       //son210215 UltLog for Samsung E-Mecha 0:Off(Default),  1:On    //son210324
    int m_nSet_UseLotName_4wLog;  //son220124 for ������. LotName�� 4wLog ������ ���.
    int m_nSet_NearbyOpenMeas;//son230712 ���� Open ������ ������� On  0:Off,  1: On 
    int m_nNearByOpenRate;    //son230728 ���� Open ��������� ���� Rate : 3~30%
};



//son 'Map' > 'Set4'   : SelfTest Condition
//son IDD_MODE_SET15_PRO1 Dialog (CModeSet15) ����
struct CSysSet15  // SETUP  //SYLEE121115-1
{
    int m_nSet1;            //son m_edit101:  Idle Time Jig up Test Delay  (msec)           �̻��
    int m_nSet2;            //son m_edit102:  SelfTest-> Open Delay     (usec)              SelfTestOpen_Timeout
    int m_nSet3;            //son m_edit103:  SelfTest-> Short1,2 Delay (usec)              SelfTestShort1_2_Timeout
    int m_nSet4;            //son m_edit104:  SelfTest-> Leak1,2 Delay  (usec)              SelfTestLeak1_2_Timeout
    int m_nSet5;            //son m_edit105:  SelfTest-> LeakA, B Delay (usec)              SelfTestLeakA_B_Timeout
    int m_nSet6;            //son m_edit106:  SelfTest Delay Use Set    (7=USE)             EnableSelfTestTimeout
    int m_nSet7;            // Mohm
    int m_nSet8;            // X
    int m_nSet9;            // X
    int m_nSet10;           // X
    int m_nSet11;           // X
    int m_nSet12;            // X
    int m_nSet13;           // X
    int m_nSet14;           // X
    int m_nSet15;           // X

};


//son 'MAP' > 'SET2(HW Fast Mode Set)'
//son IDD_MODE_SET16_FASTMODE1  (ModeSet16)   HV FastMode,  4W Set
struct CSysSet16  // SETUP  //SYLEE121115-1             
{
    int m_nSet1; 
    int m_nSet2;   
    int m_nSet3;
    int m_nSet4;   
    int m_nSet5; 
    int m_nSet6;        //son m_edit106 "4W PassCount 1 M"
    int m_nSet7;        //son m_edit107 "4W   ~10 mohm     mohm   x0.01"
    int m_nSet8;        //son m_edit108 "   4W    ~MAX. mohm %    x 0.01"
    int m_nSet9;
    int m_nSet10;       //son m_edit110 "4W   SIGMA       x 0.01"
    int m_nSet11; 
    int m_nSet12;   
    int m_nSet13;
    int m_nSet14;       //son m_check4 "4W No AC" 
    int m_nSet15;       //son m_check5 "4W ADC% USE"
    int m_nSet16;
    int m_nSet17;
    int m_nSet18; 
    int m_nSet19;
    int m_nSet20;
	int m_nSet21;//sylee230420  //son m_edit111 "4W   ~  45. mohm    %    x0.01 "
	int m_nSet22;//sylee230430  //son m_edit112 "4W PassCount 2 A ""
	int m_nSet23;//sylee230430  //son m_edit113 "4W PassCount 3  AS"
};



//son 'MAP' > 'SET3' (Leak Set)
//son IDD_MODE_SET19_FASTMODE2
struct CSysSet19  // SETUP  //SYLEE150407 //estimation
{
    int m_nSet1;            // EDIT_101: 'LR 1'  LV Short 1�� Timeout     -> ini ���Ϸ�
    int m_nSet2;            // EDIT_102: 'LR 2'  LV Short 2�� Timeout     -> ini ���Ϸ�
    int m_nSet3;            // EDIT_103: 'LR 3'  LV Short 3�� Timeout     -> ini ���Ϸ�
    int m_nSet4;            // EDIT_104: 'LR 4'  LV Short 4�� Timeout     -> ini ���Ϸ�

    int m_nSet5;            // EDIT_105: 'HR 1'  HV Leak 1�� Timeout      -> ini ���Ϸ�
    int m_nSet9;            // EDIT_109: 'HR 2'  HV Leak 2�� Timeout      -> ini ���Ϸ�
    int m_nSet21;           // EDIT_111: 'HR 3'  HV Leak 3�� Timeout      -> ini ���Ϸ�
    int m_nSet22;           // EDIT_112: 'HR 4'  HV Leak 4�� Timeout      -> ini ���Ϸ�

    double m_nSet6;         // EDIT_106: '4W Auto Error %' :  (Auto Run ����)  4W �˻縦 �������ϱ� ����    Retry4WErrorRate
                            //   : Ref ���ذ����� �󸶳� ������� üũ
                                                        
    int m_nSet7;            // EDIT_107: '4W Auto Pass Count' Auto ȭ���.                                  4WTestPassCount
    int m_nSet8;            // EDIT_108: '4W K Count'      :  �׻� 1.  average ���.  10ȸ ���� ���        4wAverageCount
    int m_nSet10;           // '4W Error% Retry' :+, - Limit�� ���� %  �� �󸶳� ������� �ÿ� ��˿� (range üũ)

    int m_nSet11;           // CHECK_1:  MAP > Set3 > Spark VB Mode
    int m_nSet12;           // CHECK_2:  MAP > Set3 > Old SparkBoard
    int m_nSet13;           // CHECK_3:  MAP > Set3 > Spark Calibration  
    int m_nSet14;           // X
    int m_n4WHVon;          //sylee200520-4W��������
                            // CHECK_5:  MAP > Set3 > 4W 50V Board Use
    int m_nSet16;           // X
    int m_nSet17;           // X
    int m_nSet18;           // X
    int m_nSet19;           // X
    int m_nSet20;           // X

    int m_nSet31;//sylee180706-2    // EDIT_126: '4W Offset1(-)': 4W minus Offse1 (���� �����̻��̸�)       4wMinusOffset
    int m_nSet32;//sylee180706-2    // EDIT_127: '4W Offset1 Ref.(>)'                                       4wMinusOffset_UseRef
    int m_nSet36;//sylee180706-2    // EDIT_131: '4W Level 1'  sylee200520-4W��������: 4W Level1���� �뵵���� 
    int m_nSet37;//sylee180706-2    // EDIT_132: '4W Level 2' 
    int m_nSet23;//sylee170228      // EDIT_113: '4W Level 3'
    int m_nSet39; //sylee201027     // EDIT_134: '4W Level 4'   //son240712

    int m_nSet33;//sylee180706-2    // EDIT_128: 'OPEN Offset R'. (recipe�� ����. ���� ����)    JigOffset_Open    (SystemOffset_Open)
    int m_nSet34;//sylee180706-2    // EDIT_129: 'Spark Mon Count 25'
    int m_nSet35;//sylee180706-2    // EDIT_130: 'Spark HW Offset1' 
    int m_nSet25;//sylee170228      // EDIT_115: 'Spark I' 
    int m_nSet38; //sylee201027     // EDIT_133: 'Spark Plus I
    int m_nSet41; //sylee230114   //sylee230420
    int m_nSet42; //sylee230114   //sylee230420
    int m_nSet43; //sylee230114
    int m_nSet44; //sylee230225
    int m_nSet45; //sylee230225
    int m_n4W_Data_Flag1;        //sylee230420
    int m_n4W_Data_Flag2;        //sylee230420
    int m_n4W_Data_Flag3;     //sylee230301  //sylee230420
    int m_n4W_ContactCheck1;       //sylee230420
    int m_nSet46; //sylee230817



};

//son Auto ȭ�� Cycle Test ����
struct CSysSet17  // SETUP  //SYLEE130805
{
    int m_nSet1;        // Cycle Run Count                        
    int m_nSet2;        // Cycle Delay Second
    int m_nSet3;        // Short Error Set  :           ShortStopCount   (Short error�� �� �̻��̸� ����������. �ǹ̾����Ƿ�)
    int m_nSet11;       // Enable CheckBox              EnableCycle      (���� ������ Auto ȭ�� 'CycleTest'��ư)
 
};


//son C:\\ACE400\\SETUP\\SysSet211.ini
//son IDD_MODE_SET21_SWOP1 :  Manual -> Set1
struct CSysSet211 // SETUP  //SYLEE160524-1
{ 
    int m_nDisable4wRetest; // 4W retest ���� ����                 1) 4W Recheck Disable //son220503 m_nSet11 -> m_nDisable4wRetest 
    int m_nSet12;           // 4w error.txt�� ���� % ��� Disable  2) 4W Error % Print         Disable4wResultErrRate  
    int m_nSet13;           //          3) Chinese
    int m_nSet14;           // X        4) Buzzer
    int m_nSet15;           // ����     5) Open Limit +Short            EnableOpenLimit  (������ ����� �´�)
    int m_nSet16;  //sylee171011-1  //  6) Sheet Retest                 EnableSheetRetest
    int m_nSet17;   //sylee171011-1 //  7) 4W Auto Sampling Retest      Enable4wAutoSamplingRetest

    int m_nSet21;           //          8) Block XY+ AutoDisplay        EnableAutoBlockDirection
    int m_nSet22;           //          9) Error Pin RankingDisplay     EnableErrorPinRank
    int m_nSet23;           //         10) Skip Piece(Remote)           EnablePieceSkip  (for ���)
    int m_nSet24;           //         11) Recipe: OPEN R.Offset        EnableCustomerOpenR_Offset  
    int m_nSet25;           //         12) Print : 4W E1, E2, E3, E4    EnablePrint_4WErrorType
    int m_nSet26;  //sylee171011-1     13) OPEN R Meas.Logging    
                                    //son ChildView7 nFlagOpenMes1 ON => ARun201_O11_Log1() ȣ��    ModeSet21 m_check13   
    int m_nSet27;   //sylee171011-1

    WORD m_wPrintState;     //    D:\log  Print Option    //son230720 MyTrace() ����Ʈ state��  g_wPrintState ���� ����. 
};


struct CSysSet21  // SETUP  //SYLEE130618
{
    int m_nDrec[362][16];
};

struct CSysSet22  // SETUP  //SYLEE130618
{
    int m_dRecSetup[80][11];
};

extern int dRecSetup[80][11];

#endif // !defined(AFX_FILESYSINFO_H__B85E18EA_5A5B_4129_A54C_D02D75AE6A24__INCLUDED_)
