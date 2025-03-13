#if !defined(AFX_MODESET12_H__98510B36_3CD9_46B2_9E8F_1665931DFB1F__INCLUDED_)
#define AFX_MODESET12_H__98510B36_3CD9_46B2_9E8F_1665931DFB1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSet12.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSet12 dialog


#include "FileSysInfo.h"    //son-32KPIN-20200228 : �ű� �߰�


#define     MAX_BOX_LINE            64      
#define     MAX_LINE_COL            64  // card pin�� �ش�ȴ�. (Line������ col ���� ��, card�� pin ����)
#define     MAX_SD_DD_CARD_PIN      32  // SD Card, DD Card�� Line��  col�� �ش�.
#define     MAX_QD_CARD_PIN         64  //son201218

#define     MAX_BOX_CARD64          64  // 64  pin card�� 1�� line�� 1�� card�� �ش�.   BOX_CARD�� 64 
#define     MAX_BOX_CARD128         32  // 128 pin card�� 2�� line�� 1�� card�� �ش�.   BOX_CARD�� 32 <= ���� �̰� ���
#define     MAX_BOX_CARD            MAX_BOX_CARD128

enum HWBOX_LOC  {   BOX_A,              // 0
                    BOX_B,              // 1
                    BOX_C,              // 2
                    BOX_D,              // 3
                    MAX_BOX_PER_PART    // 4
};

enum HWBOX_TYPE {   TYPE_AC,    // 0
                    TYPE_BD     // 1
};


#define     MAX_BOX_PIN     (MAX_BOX_LINE * MAX_LINE_COL)       // 4096     Box �� Max pin ����
#define     MAX_PART_PIN    (MAX_BOX_PIN * MAX_BOX_PER_PART)    // 16384    UPPER�� LOWER�� pin ����
#define     MAX_UPPER_PIN   MAX_PART_PIN                        // 16384    UPPER�� ���� Max pin ����. 
#define     MAX_LOWER_PIN   MAX_PART_PIN                        // 16384    LOWER�� ���� Max pin ����. 
//#define   MAX_PIN         (MAX_PART_PIN * MAX_PART)           // 32768    32K,  UPPER�� LOWER 2���� part�� ����
                                                                // ==> Globals.h �� �̵�.
#define     MAX_BOX         MAX_BOX_PER_PART *  MAX_PART        // 8


#define     UPPER_BOX_NUM   4 //son241007-BSW-9


//-----------------------------------------
//for IDC_EDIT_LOWER_START_PIN ComboBox
enum E_LOWER_START_PIN {     
                            E_1025,     // 0    
                            E_2049,     // 1    
                            E_4097,     // 2    
                            E_8193,     // 3    
                            E_16385,    // 4    
                            E_3073,     // 5        //son210525 SD Upper box 3�� �Ϻν�����
                            E_6145,     // 6        //son210525 DD Upper box 3�� �Ϻν�����
                            E_12289,    // 7        //son210525 QD Upper box 3�� �Ϻν�����

                            E_5121,     // 8        //son241007-BSW-9 SD 3�� DD 1�� �Ϻν�����
                            E_7169,     // 9        //son241007-BSW-9 SD 1�� DD 3�� �Ϻν�����
                            E_9217,     // 10       //son241007-BSW-9 SD 1�� DD 2�� QD 1�� �Ϻν�����
                            E_10241,    // 11       //son241007-BSW-9 SD 2��        QD 2�� �Ϻν�����
                            E_11265,    // 12       //son241007-BSW-9 SD 2�� DD 1�� QD 2�� �Ϻν�����
                            E_13313,    // 13       //son241007-BSW-9 SD 1��        QD 3�� �Ϻν�����
                            E_14337,    // 14       //son241007-BSW-9        DD 1�� QD 3�� �Ϻν�����
                            LOWER_STARTPIN_NUM
};

typedef struct sPIN_LIST
{
	CString	str;
	int		nPin;
} sPIN_LIST;   //son240516

//#define PINLIST_NUM  ((LOWER_STARTPIN_NUM >= ENDPIN_NUM) ? LOWER_STARTPIN_NUM: ENDPIN_NUM)
#define PINLIST_NUM  LOWER_STARTPIN_NUM 

class CPinList   //son240516
{
public:
	sPIN_LIST   saList[PINLIST_NUM];
	int         m_nMaxPin; 

public:
    CPinList()
    {
        m_nMaxPin = PINLIST_NUM;
        for (int i = 0; i < m_nMaxPin; i++)
        {
            saList[i].str ="";
            saList[i].nPin =-1;

        }
    }


	// "C:\\ACE400\\SETUP2\\SysSetPin.ini" file(LoadSaveSet12()) write�ÿ�  
	// LowerStartPin �� �״�� file�� ���� ���� �Ʒ� �Լ� �ۼ�.
	// combo�� index�� E_LOWER_STARTPIN enum ���� file�� ���ٸ� �Ʒ� �Լ��� �ʿ� ����.
	int		findItemNo(int nPin)
	{
		for (int i = 0; i< m_nMaxPin; i++)
		{
			if (saList[i].nPin == nPin)
				return i;
		}

		// saList�� ���� LowerStartPin ���� ���Դٸ� default�� 16385�� index�� ���� �ش�.
		//return E_16385;		// defalut

		// saList�� ���� lowerStartPin �� ������ 16385 default�� �Ҵ��ϴ� �ΰ��۾��� ���� -1�� ����
		return -1;		// defalut
	}

};


class CLowerStartPinList: public CPinList   //son240516 CPinList ��ӱ����� ����.
{
//son240516	sPIN_LIST		saList[LOWER_STARTPIN_NUM];

public:
	CLowerStartPinList()		// Constructor: ������
	{
	    m_nMaxPin = LOWER_STARTPIN_NUM;    //son240516
		saList[E_1025].str  = "1025";	saList[E_1025].nPin = 1025;
		saList[E_2049].str  = "2049";	saList[E_2049].nPin = 2049;
		saList[E_4097].str  = "4097";	saList[E_4097].nPin = 4097;
		saList[E_8193].str  = "8193";	saList[E_8193].nPin = 8193;
		saList[E_16385].str = "16385";	saList[E_16385].nPin = 16385;
		saList[E_3073].str  = "3073";	saList[E_3073].nPin = 3073;         //son210525
		saList[E_6145].str  = "6145";	saList[E_6145].nPin = 6145;         //son210525
		saList[E_12289].str = "12289";	saList[E_12289].nPin = 12289;       //son210525

		saList[E_5121].str = "5121";	saList[E_5121].nPin = 5121;         //son241007-BSW-9
		saList[E_7169].str = "7169";	saList[E_7169].nPin = 7169;         //son241007-BSW-9
		saList[E_9217].str = "9217";	saList[E_9217].nPin = 9217;         //son241007-BSW-9
		saList[E_10241].str = "10241";	saList[E_10241].nPin = 10241;       //son241007-BSW-9
		saList[E_11265].str = "11265";	saList[E_11265].nPin = 11265;       //son241007-BSW-9
		saList[E_13313].str = "13313";	saList[E_13313].nPin = 13313;       //son241007-BSW-9
		saList[E_14337].str = "14337";	saList[E_14337].nPin = 14337;       //son241007-BSW-9
	}

}; 

//son210514  int nPinBlockRange[10][5]�� CPinBlockRange sPinBlockRange[10]�� ����
typedef struct CPinBlockRange
{
    int nStartPin;
    int nEndPin;
    int nBlockType;     // 1: BOX_QD,  2:BOX_DD, 3:BOX_SD, 4:BOX_SKIP(for upper3, upper4)
    int nHwBoxType;     // 0: TYPE_AC, 1:TYPE_BD
} CPinBlockRange;



class CModeSet12 : public CDialog
{
// Construction
public:
    CModeSet12(CWnd* pParent = NULL);   // standard constructor


    void A_Language();//sylee151028

    
    CLabelControl  m_Label1001;
    CLabelControl  m_Label1002;
    CLabelControl  m_Label1003;
    CLabelControl  m_Label1004;
    CLabelControl  m_Label1005;
    CLabelControl  m_Label1006; 
    CLabelControl  m_Label1007;
    CLabelControl  m_Label1008;
    CLabelControl  m_Label1009;
    CLabelControl  m_Label1010;
    CLabelControl  m_Label1011;

 
    CCommandButton m_button1001;
    CCommandButton m_button1002;
    CCommandButton m_button1003;

    int  MakePinBlockRangeUserNo();//sylee210402 
    int  MakePinBlockRangeUserNo_32K();//son210525
    int  MakePinBlockRangeUserNo_K600(); //son230217
    int  CalcUpperEndJigPin_BlockSet();  //son241007-BSW-9  

    int  OnPinConvert1(); 
    int  OnPinConvert1_QD(); //sylee151006
    int  OnPinConvert1_2RD(); //test //SYLEE140430 R&D MANUAL MACHINE SWITCH CARD JIG
    int  OnPinConvert1_READ(int nMode);  //SYLEE150922-1  //sylee151111add
    int  OnPinConvert1_YAMAHA();    //ACE400  //SYLEE160907-1
    //int  OnPinConvert1_YAMAHA_T0(int nMode);    //ACE400  //SYLEE160907-1

    int  OnPinConvert1_32K(int nPinConvert);        //son-32KPIN-20190902
    int  make32kHwPin(UINT nSwPin);                 //son-32KPIN-20190902
    int  make32kSwPin(UINT nHwPin);                 //son-32KPIN-20190902
    int  getQdSwPin(UINT nSwPin);                   //son-32KPIN-20200117    DD, SD -> QD
    int  getDdSdSwPin(UINT nSwPin);                 //son-32KPIN-20200117    QD -> DD, SD
    int  getQD_UpperPartSize();                     //son-32KPIN-20200206
    void initCombo_LowerStartPin(int nQdType, int nMaxPin, bool bForce=FALSE); //son-32KPIN-20200228   //son210514 bForce�߰�
    bool setCombo_DefaultLowerStartPin(int nDefaultMode_LowerStart, bool bForce=FALSE);    //son201222 //son210514 bForce�߰� //son240516
	void repair_LowerStartPin(int nQdType, int nMaxPin);   //son210115 //son241007-BSW-9 init_=> repair_ �� �̸�����
	int  initCombo_EndPin(int nMaxPin); //son240516  //son241007-BSW-9
	int  initCombo_EndPin_BlockSet(); //son241007-BSW-9
	int  findUpperBoxNum(int nModeLowerStart);   //son240516

	int  calculate_TotalPins(PIN_BOXTYPE nUppderDensity, int nUpperBox, PIN_BOXTYPE nLowerDensity, int nLowerBox);  //son240516
	int  getBoxPinCount(PIN_BOXTYPE type);         //son240516

    int  make16kQdBase_HwPin(UINT nSwPin);    //son-8K4097-20200605     //son211020
    int  getQdSwPin_Fixed(UINT nSwPin);       //son-8K4097-20200605 �ű�: by JYK 
    int  getDdSdSwPin_Fixed(UINT nQdSwPin);   //son-8K4097-20200605 �ű�: by JYK

    int  make32kOrgBlockSet_HwPin(UINT nSwPin);  //son241007-BSW-9  
    int  getQdSwPin_BlockSet1(UINT nSwPin);      //son241007-BSW-9  

    int  getQdPin_fromSdPin(int nSwPin, int nOffset, int nQdOffset);    //son201218 
    int  getQdPin_fromDdPin(int nSwPin, int nOffset, int nQdOffset);    //son201218 
    int  getSdPin_fromQdPin(UINT nQdSwPin, int nOffset, int nQdOffset); //son201218
    int  getDdPin_fromQdPin(UINT nQdSwPin, int nOffset, int nQdOffset); //son201218

    int  m_nBlockPo1;//sylee210402  //son nBlockPo1-> m_nBlockPo1  ���� Pin�� block ����
    int  GetBlockNo_PinNo_32K(int &nPin1, int &nBlock1, int &nBlockType, int &nHwBoxType);   //son210514
    //int  GetBlockNo_PinNo_fileLog();                    //son210514
    void EnableDisable_QDDDSDControl();         //son210514 QD/DD/SD ��Ʈ���� ��Ȳ�� �°� Enable Disable�Ѵ�.
    int  FindBlockSet_LowerStartPin(int nUppderEnd);  //son210514 �־��� nUpperEnd���� �´� �Ϻν�������ġ�� ã�´�.
    bool IsUpper8k(int nPinTotMax);         //son210831
    bool IsUpper4k(int nPinTotMax);         //son210831
    bool IsUpper12k(int nPinTotMax); //son240522

// Dialog Data
    //{{AFX_DATA(CModeSet12)
    enum { IDD = IDD_MODE_SET12_PIN };
        CComboBox   m_combo1;
        CComboBox   m_combo2;
        CComboBox   m_combo3;
        CComboBox   m_combo4;
        CComboBox   m_combo5;
        CComboBox   m_combo8;
        CButton     m_check1;
        CButton     m_check2;
        CButton     m_check3;
        CButton     m_check4;
        CButton     m_check5;
        CButton     m_check6;
        CButton     m_check7;
        CButton     m_check8;
        CButton     m_check9;
        CButton     m_check10;
        CButton     m_check11;
        CButton     m_check12;
        CButton     m_check13;
        CButton     m_check14;
        CComboBox   m_combo_LowerJigStartPin;
		CComboBox	m_combo_EndPin;
        CComboBox   m_combo21;
        CComboBox   m_combo22;
        CComboBox   m_combo23;
        CComboBox   m_combo24;
    //}}AFX_DATA

public:
    CLowerStartPinList  m_LowerStartPin;    //son for m_combo_LowerJigStartPin
	int   m_nLowerEndBox_EndPin[MAX_BOX_PER_PART];   //son240516 �Ϻ� 4�� �ڽ��� �ϳ��� endBox�� �������� endPin

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CModeSet12)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CModeSet12)
            afx_msg void OnClickOk();
            afx_msg void OnSelchangeCombo1();
            afx_msg void OnSelchangeCombo2();
            afx_msg void OnSelchangeCombo3();
            afx_msg void OnSelchangeCombo4();
            afx_msg void OnSelchangeCombo5();   
            afx_msg void OnSelchangeCombo8();           
            afx_msg void OnCheck1();
            afx_msg void OnCheck2();
            afx_msg void OnCheck3();
            afx_msg void OnCheck4();
            afx_msg void OnCheck5();
            afx_msg void OnCheck6();
            afx_msg void OnCheck7();
            afx_msg void OnCheck8();
            afx_msg void OnCheck9();
            afx_msg void OnCheck10();
            afx_msg void OnCheck11();
            afx_msg void OnCheck12();
            afx_msg void OnCheck13();
            afx_msg void OnSave();
            afx_msg void OnSelchangeComboLowerStartPin();
	        afx_msg void OnSelchangeComboEndPin();
            afx_msg void OnCombo21();
            afx_msg void OnCombo22();
            afx_msg void OnCombo23();
            afx_msg void OnCombo24();
    //}}AFX_MSG
    DECLARE_EVENTSINK_MAP()
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODESET12_H__98510B36_3CD9_46B2_9E8F_1665931DFB1F__INCLUDED_)
