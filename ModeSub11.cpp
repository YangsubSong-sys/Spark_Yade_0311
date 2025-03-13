// ModeSub11.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub11.h"
#include "ModeSet12.h"
#include "FileSysInfo.h"
 
extern CSysInfoPieceRange  SysInfoPieceRange;
extern CFileSysInfo FileSysInfo01;

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern  int nPieceMax; 
////////////////////////////////////// 
extern int 	nFlagFileLoad2;//sylee180406-1
extern CSysSet12  SysSet12;  


/////////////////////////////////////////////////
extern  int g_nPieceMax1;
extern  int nBlockMax1;
extern int nMultyNetTotal;//sylee150917-5
extern int nMulNetFlag;//sylee150917-5

//son 해당 block, piece의 cell 번호 (cell Max는 blockMax * pieceMax)
//    ChildView7에서 GraphDisplay1[]의 index로 사용.
//int nBlockPieceNo1[301][301];// BLOCK, PIECE    
int nBlockPieceNo1[MAX_BLOCK+1][MAX_PIECE+1]; //son220823_3 301->MAX_BLOCK, MAX_PIECE
                                             
//int nBlockPieceYX1[301][301];// BLOCK, PIECE    //son220823_3 301->MAX_BLOCK, MAX_PIECE
int nBlockPieceYX1[MAX_BLOCK+1][MAX_PIECE+1]; //son nBlockPieceYX1[row][col] = cell;
                                              //son nBlockPieceYX1[0][1] : Block-Piece Cell Row size 
                                              //son nBlockPieceYX1[0][2] : Block-Piece Cell Col size



//son g_nPinPieceNo[nDut][pin] : 각 핀의 piece 번호 정보(1~) 를 저장.  
//    index는 nDut(1~), pinNo(1~),    value는 pieceNo(1~)
//sylee150920  int nPiece1[DEF_MAX_PIN1];
int g_nPinPieceNo[MAX_DUT1][DEF_MAX_PIN1];//SYLEE180810-1-DUT1  //son220824_2 DefDutMax(33) -> MAX_DUT1(33)

extern short nBDLData3[MAX_DUT1][DEF_MAX_PIN1][10];//short data //son220824_2 DefDutMax(33) -> MAX_DUT1(33)


//son 1) PieceRange.ini의 정보로 g_nPinPieceNo1[] 정보 생성
//       g_nPinPieceNo1[] : 각 핀의 piece 번호 정보를 저장.  index는 pinNo, value는 pieceNo
//son200420 nPieceNoRead1(int nDUT) -> ReadPinPieceNo(int nDUT)로 변경 
void ReadPinPieceNo(int nDUT) //sylee180810-1-DUT16
{
	CString str;
    int nRet1;

	//son RangeCheck nDut: 1~ 32 여야 한다.
	if (nDUT < 0){      
		nDUT=1;
	}
	if (nDUT > MAX_DUT){ //son220824_2 DefDutMax2 -> MAX_DUT(32)
		nDUT=1;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//BDL  NET LIST PIECE SET
	//PIN  PIECE SET NO1
	::ZeroMemory(&g_nPinPieceNo[nDUT], sizeof(g_nPinPieceNo[nDUT]));

	//son piece 정보 갯수 (piece갯수 *2. for 상부, 하부)가 0 이상이면
	if (SysInfoPieceRange.m_nRow[nDUT] > 0)
	{
		for (int row=0; row < SysInfoPieceRange.m_nRow[nDUT]; row++) 
		{
		    int startPin = SysInfoPieceRange.m_nPin[nDUT][row][1]; 	
		    int endPin   = SysInfoPieceRange.m_nPin[nDUT][row][2];

			if (startPin > endPin)
			{
				str.Format("Error No 1318,\n \n\n  PIECE SET ERROR .  \n\n SET CHECK ? \n\n  if( StartPinno=%d > End Pinno=%d ) Error. ", 
				        startPin, endPin);
				AfxMessageBox(str);
				break;//sylee131128
			}

			//-------------------------------------------------
			//son g_nPinPieceNo[nDUT][pin] 정보 획득 (k=pin)
			for (int pin = startPin; pin <= endPin; pin++)      //son pin(1~)
			{ 
				if(g_nPinPieceNo[nDUT][pin]!=0)//SYLEE180810-1 DOWN
				{
					str.Format("Error No 1319,\n \n\n  PIECE Double Set  ERROR .  \n\n ***PIN=%d***   \n\n Set Check?  \n\n  [SETUP]=> [PIECE POINT]?? \n\n    Error.\n\n\n\n *YES=RUN,  *NO=STOP \n\n ", pin); 
					nRet1=AfxMessageBox(str,MB_YESNO | MB_ICONEXCLAMATION);			 	 
					if(nRet1!=6){						 
						return;
					}
					break;//sylee131128
				}

				//son g_nPinPieceNo[nDUT][pin] 위치에 해당하는 piece 정보를 write한다.
				g_nPinPieceNo[nDUT][pin] = SysInfoPieceRange.m_nPin[nDUT][row][0];	//son m_nPieceInfo[DutNo][piece][0] : PieceNo	
			}
		}	    
	}

	// g_nPieceMax1=nPieceMax;//test
	g_nPieceMax1 = SysInfoPieceRange.m_nPieceCount[nDUT];

#if 0 //son221103_2 ReadPinPieceNo()에서 makePieceShortTestData()로  하단 코드  분리 
      //   ReadPinPieceNo(nDUT) 가 본연의 기능에 충실하도록 하고 
      //   Short List 생성은 명시적으로 makePieceShortTestData()를  호출하도록 변경.

	//------------------------------
	// SHORT LIST를 PIECE별로 분리 
	//------------------------------
    int firstPin1 = 0;
    int piece = 0;
    int t4 = 0;

	//son200513: Piece 200인 경우에 Auto화면에서 bdl 파일에러, 및 Block-Piece 맵이 안 그려지는 문제, 죽는 문제 발생
	//             아래 array size가 200이면 arrPiecePinCnt[199]까지만 access가능하므로 죽을 수 있어서 +1하여 size를 키워줌.
	int arrPiecePinCnt[MAX_PIECE +1];//PIECE 증가 번호 //son 해당 piece의 pinCount, 
	::ZeroMemory(&arrPiecePinCnt, sizeof(arrPiecePinCnt));
	                                                //    index는 piece, value는 pin
	DeleteDutShortTestItem(nDUT);

    // 한번 정의해서 계속 재활용한다.(InsertShortTest()에서 실제 data 영역에 copy, Call by value)
    CShortTestItem shortTestItem; 


#ifdef __OPVECTOR__
    //son-20200420 :  (*g_pvBDLData31[nDut][piece])[piecePin] 의 원본인 
    //     nBDLData31[nDut][piece][piecePinIdx]를 ChildView7에서 사용할 때 
    //     모두 1부터 piecePin을 access하고 있음. ChildView7 영역을 모두 0으로 시작하게 바꾸는 것이
    //     리스크가 커서 메모리가 아깝지만,  vector도 1부터 시작하도록 수정하기로 함.
    //     초기화할때 첫번째 item이면 무조건 push_back을 먼저 1회 하고 시작하도록 수정한다.
    //     InsertShortTest() 함수 내부에서 index 0이면 두번 넣게 수정 -> 20200702 삭제 
    //
    //son-20200702 : 대표핀만 있는 BDL인 경우, OpenTest 없이 ShortTest만 하게 됨.  
    //     이경우에는  g_pvBDLData2Op[dutId][ip]->size() 가 0이어서 메모리 access하다 죽는 현상 발생함.
    //     ex) nTOpenSum1=nTOpenSum1 + (g_pvBDLData2Op[nMulNetFlag][ip]->size() -1);
    //         nBDLData2Op[nDut][piece][0][0] 를 (g_pvBDLData2Op[nDut][nPiece]->size() -1)로 대체한 것이 위험해짐.
    //    
    //     이 문제 관련해서 ShortTest도 비슷한 위험성이 있어서 ReadPinPieceNo()에서 모든 piece에 대해 
    //     무조건 InsertShortTest를 1회 하고 시작하도록 수정한다. (기본 size()가 1이 되게 ) 
    //     InsertShortTest() 함수 내부에서 index 0이면 push_back() 두번 넣게 했던 코드는 삭제함.
    
    //son 모든 piece에 대해 무조건 InsertShortTest()을 먼저 1회 넣는다. 
    for(int pieceIdx=1; pieceIdx <= SysInfoPieceRange.m_nPieceCount[nDUT]; pieceIdx++)
        InsertShortTest(nDUT, pieceIdx, shortTestItem);     //son 공백인  shortTestItem을 insert
#endif

	//son nBDLData3[dut][ShortPin][]을 piece별로 나누어서 nBDLData31[dut][piece][ShortPin]으로 재배치한다.
	int nShortTestCnt = nBDLData3[nDUT][0][1];
	if (nShortTestCnt > 0)//son 현재 DUT의 short 총수(=대표핀수).  max  32k
    {
		for (int shortIdx = 1; shortIdx <= nShortTestCnt; shortIdx++)
		{
		    //son 해당 대표핀의 piece번호를 알아낸다.
			firstPin1 = nBDLData3[nDUT][shortIdx][BDL_PIN0] +1;//SYLEE120910 //son 대표핀 (1~)
			piece     = g_nPinPieceNo[nDUT][firstPin1];				        //son 대표핀의 piece정보  (1~)
			arrPiecePinCnt[piece]++;                                        //son 해당 piece의 pinCount
			int piecePinIdx = arrPiecePinCnt[piece];

			//son pinNo를 nBDLData31[dut][piece][piecPinIdx] 에 write한다.
			if(piece <= g_nPieceMax1)
			{  
#ifndef __OPVECTOR__
				nBDLData31[nDUT][piece][piecePinIdx] = firstPin1 -1;//SYLEE120910
	            nBDLData31[nDUT][piece][0] = arrPiecePinCnt[piece];// PIECE별 Short 총 갯수
#else
                shortTestItem.wFirstPin  = firstPin1 -1;
                shortTestItem.wNetPinCnt = nBDLData3[nDUT][shortIdx][BDL_NET_PINCNT]; //son200722-nBDLData3-Sort: 신규추가


                // shortTestItem을  nDut, piece에 해당하는 'piece별 ShortTest vector' 메모리에 추가한다.
                InsertShortTest(nDUT, piece, shortTestItem);
#endif
                //son???? t4는 어디에 사용??
				if(t4 < (firstPin1-1)) {//SYLEE120910
					t4 = (firstPin1-1);//SYLEE120910
				}
			}
			else
			{
				//error message 
				str.Format("Error No 7304. \n\n SHORT PIECE SORTING ERROR. \n \n\n SHORT PIN PIECE NO.? \n\n   PIECE MAX. OVER.  \n\n  [SETUP]=>[PIECE POINT] Check"); 
				AfxMessageBox(str);
				return ;//sylee20120503
			}
		}

	}

#endif

	return;

}



/////////////////////////////////////////////////////////////////////////////
// CModeSub11 dialog


CModeSub11::CModeSub11(CWnd* pParent /*=NULL*/)
	: CDialog(CModeSub11::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeSub11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeSub11::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeSub11)
     	DDX_Control(pDX, IDC_LABEL5,  m_Label5);//sylee150804-2
		DDX_Control(pDX, IDC_GRID_1, m_grid1);
//		DDX_Control(pDX, IDC_SLIDER1, m_slider1);
//		DDX_Control(pDX, IDC_SLIDER2, m_slider2);
		DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollbar1);
		DDX_Control(pDX, IDC_SCROLLBAR2, m_scrollbar2);
		DDX_Control(pDX, IDC_SCROLLBAR3, m_scrollbar3); 
		
		DDX_Control(pDX, IDC_LABEL1,  m_Label1);	
		DDX_Control(pDX, IDC_LABEL2,  m_Label2);
		DDX_Control(pDX, IDC_LABEL3,  m_Label3);

		DDX_Control(pDX, IDC_EDIT1,   m_edit1);
		DDX_Text(pDX, IDC_EDIT2,   m_edit2);	
		DDX_Text(pDX, IDC_EDIT3,   m_edit3);
		DDX_Text(pDX, IDC_EDIT5,   m_edit4);

	    DDX_Control(pDX, IDC_EDIT21,   m_edit21);
	    DDX_Control(pDX, IDC_EDIT22,   m_edit22);
	    DDX_Control(pDX, IDC_EDIT23,   m_edit23);
	    DDX_Control(pDX, IDC_EDIT24,   m_edit24);

		DDX_Control(pDX, IDC_LABEL8,  m_Label8);//sylee150917-5
		DDX_Control(pDX, IDC_LABEL21,  m_Label21);//sylee150917-5		

	    DDX_Control(pDX, IDC_LABEL1001,  m_Label1001);
		DDX_Control(pDX, IDC_LABEL1002,  m_Label1002);
		DDX_Control(pDX, IDC_LABEL1003,  m_Label1003);
 		DDX_Control(pDX, IDC_LABEL1004,  m_Label1004);
  		DDX_Control(pDX, IDC_LABEL1005,  m_Label1005);

		DDX_Control(pDX, IDC_LABEL1006,  m_Label1006);
		DDX_Control(pDX, IDC_LABEL1007,  m_Label1007);
		DDX_Control(pDX, IDC_LABEL1008,  m_Label1008);
 		DDX_Control(pDX, IDC_LABEL1009,  m_Label1009);
  		DDX_Control(pDX, IDC_LABEL1010,  m_Label1010);	 
 
		DDX_Control(pDX, IDC_SET1,  m_button1001);
		DDX_Control(pDX, IDC_BUTTON103,  m_button1002);

		DDX_Control(pDX, IDC_BLOAD,  m_button1003);
		DDX_Control(pDX, IDC_BSAVE,  m_button1005);
		DDX_Control(pDX, IDC_OK,  m_button1004);

		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub11, CDialog)
	//{{AFX_MSG_MAP(CModeSub11)

//	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_SLIDER1, OnChangeSlider1)	
//	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_SLIDER2, OnChangeSlider2)	
	ON_NOTIFY(NM_RELEASEDCAPTURE,IDC_LABEL2, OnLabel2) 
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub11 message handlers

BEGIN_EVENTSINK_MAP(CModeSub11, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub11)
	ON_EVENT(CModeSub11, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_LABEL2, -600 /* Click */, OnClickGrid1, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CModeSub11, IDC_BSAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_BLOAD,-600 /* Click */, Onload, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_SET1,-600 /* Click */, OnSet1, VTS_NONE)
	ON_EVENT(CModeSub11, IDC_BUTTON103, -600 /* Click */, OnButton103, VTS_NONE) 
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


 
void CModeSub11::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
 
      m_Label1001.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 101));
 	  m_Label1002.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 102));
	  m_Label1003.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 103));
	  m_Label1004.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 104));
      m_Label1005.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 110));
 	  m_Label1006.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 105));
	  m_Label1007.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 106));
	  m_Label1008.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 107));
      m_Label1009.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 108));
 	  m_Label1010.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 111));
 
	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 109));
	  m_button1002.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 112));
	  m_button1003.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 113));
	  m_button1004.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 114));
	  m_button1005.SetWindowText(LoadMessage(1,"TitleSetupPieceRange", 115));

  } 

}

BOOL CModeSub11::OnInitDialog() 
{
	CDialog::OnInitDialog();
 
 	CString str; 

	InitControl();
//	m_slider1.SetRange(1,100);  
//  m_slider2.SetRange(1,51200); 
//	m_slider1.SetPos(SysInfoPieceRange.m_nPieceCount); 
//	m_slider2.SetPos(SysInfoPieceRange.m_nPinTot);
	m_scrollbar1.SetScrollRange(1, 200, FALSE);
	m_scrollbar2.SetScrollRange(1, 32766, FALSE);//32768
	m_scrollbar3.SetScrollRange(1, 200, FALSE);

	m_scrollbar1.SetScrollPos(SysInfoPieceRange.m_nPieceCount[1]); 
	m_scrollbar2.SetScrollPos(SysInfoPieceRange.m_nPinTot[1]); 
	m_scrollbar3.SetScrollPos(SysInfoPieceRange.m_nRow[1]);

	str.Format("%d", SysInfoPieceRange.m_nPieceCount[1]);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nPinTot[1]);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nRow[1]);
	m_Label3.SetCaption(str);
 
// m_slider1.SetPos(255);          
// m_nOpa=m_slider1.GetPos();  
// CFileS  mFileS;
// mFileS.SaveMulti1();
// SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)| WS_EX_LAYERED);
// SetAttr(m_nOpa);
// SetLayeredWindowAttributes(RGB(0,0,255),m_nOpa,LWA_ALPHA | LWA_COLORKEY);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CModeSub11::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
/*	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_ESCAPE)) {
		return TRUE;
	}
*/
 	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_RETURN)) {
 //	 	OnOK();
		return TRUE;
	}
 
	return CDialog::PreTranslateMessage(pMsg);
}

void CModeSub11::OnClickOk() 
{
 
    FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD);  //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
	CDialog::OnOK();

}

void CModeSub11::OnClickGrid1() 
{
	// TODO: Add your control notification handler code here
 
}

void CModeSub11::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}

 /*
void CModeSub11::OnChangeSlider1()	
{

	int i,j;
	CString strValue;
	CString str;
 
	i=m_slider1.GetPos();
 
	m_slider1.SetPos(i);	
	SysInfoPieceRange.m_nPieceCount=i;	
    strValue.Format("%d", i);
	m_Label1.SetCaption(strValue);
	i=i+1;	
	m_grid1.SetRows(i);  

    for ( j = 0 ; j < i-1; j++) 
    { 
		    m_grid1.SetCol(0); 
            m_grid1.SetRow(j+1); 
            str.Format("No. %d", j+1); 
            m_grid1.SetText(str); 
    } 

	UpdateData(FALSE);
}

void CModeSub11::OnChangeSlider2()	
{
	int i;
	CString strValue;
	
	i=m_slider2.GetPos();
	m_slider2.SetPos(i);
 //	i=i*512;

 	SysInfoPieceRange.m_nPinTot=i;

    strValue.Format("%d", i);
	m_Label2.SetCaption(strValue);

	UpdateData(FALSE);
}
*/

 

void CModeSub11::OnDblClickGrid1() 
{
	// TODO: Add your control notification handler code here
 
/*	 // 그리드 컨트롤을 더블클릭 했을 때 처리하는 루틴
    CEditGridDlg dlg;    

    // 현재의 x, y 좌표를 얻는다. 
    int x, y; 

    x = m_grid1.GetColSel(); 
    y = m_grid1.GetRowSel(); 

    dlg.m_editStr = m_grid1.GetText();      

    if ( dlg.DoModal() == IDOK)
    { 
            m_grid1.SetText(dlg.m_editStr); 
    } 
*/
	
}


extern CSysInfoView01  SysInfoView01;//sylee150804-2


void CModeSub11::InitControl()
{
	int a;
    Onload(); 

	 //extern CSysInfoView01  SysInfoView01;//sylee150804-2
	CString str;
	int no;
    str=SysInfoView01.m_pStrFileBDLName;
  	no = str.Find('.');
	str= str.Left(no);
	m_Label5.SetCaption(str);//sylee150804

    a=FileSysInfo01.LoadSaveSet12(_LOAD);//load :2

	if(a!=1){
		AfxMessageBox(" Error No 1312, \n    SysSet12.ini  Load ERROR !  \n    file Read Error !!!");    
	}

    m_edit21.SetWindowText("1");//sylee130925

    //son 하부시작핀 표시       
    str.Format("%d", SysSet12.m_nLowerJigStartPin); //son-32KPIN-20200120
	m_edit23.SetWindowText(str);

#if 0
	//OnButton103();//sylee150917-8`q   //son220824_4 삭제. 화면초기화하는데 Dut 번호 증가시킬 이유 없음.
#else	
	//son220824_4 begin: OnButton103() 삭제하고 Label8, Label21 출력 기능을 추가 
    str.Format("%d", nMulNetFlag);
    m_Label8.SetCaption(str);//SYLEE150917
    str.Format("%d", nMultyNetTotal);
    m_Label21.SetCaption(str);//SYLEE150917
    //son220824_4 end
#endif
	
	FileSysInfo01.LoadSaveSet211(2);//load //sylee160928
	A_Language();
//	nMultyNetTotal=16;//sylee180810???????? test

} 


void CModeSub11::OnLabel2() 
{ 
 
}

void CModeSub11::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	BeginEditing();		
	
}

void CModeSub11::OnEnterCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSub11::OnLeaveCellMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	if (OnEditing) {
		EndEditing();
	}	
}

void CModeSub11::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	m_grid1.SetRowSel(m_nCurRow);
	m_grid1.SetColSel(m_nCurCol);
//	m_grid1.OnHScroll(10,20,1);
}

void CModeSub11::EndEditing()
{
 
	CString strText;
 
	m_edit1.EnableWindow(FALSE);
	m_edit1.ShowWindow(SW_HIDE);
	m_edit1.GetWindowText(strText);
 
	m_grid1.SetText(strText);
	OnEditing = FALSE;
	m_grid1.SetFocus();
//	SetModified(TRUE);	
}

void CModeSub11::BeginEditing()
{
 
	OnEditing = TRUE;
	CRect rectCell;
	rectCell.left = m_rectGrid.left + long(m_dRatio * double(m_grid1.GetCellLeft()));
	rectCell.top = m_rectGrid.top + long(m_dRatio * double(m_grid1.GetCellTop()));
	rectCell.right = rectCell.left + long(m_dRatio * double(m_grid1.GetCellWidth()));
	rectCell.bottom = rectCell.top + long(m_dRatio * double(m_grid1.GetCellHeight()));
 
	m_edit1.SetWindowText(m_grid1.GetTextMatrix(m_nCurRow, m_nCurCol));
	m_edit1.MoveWindow(rectCell);
	m_edit1.EnableWindow(TRUE);
	m_edit1.ShowWindow(SW_SHOWNORMAL);
	m_edit1.SetFocus();
	m_edit1.SetSel(0, -1);
 
}

void CModeSub11::OnSave() 
{
	int i,k; 	
    CString str;
   
	if(nMultyNetTotal<1){//sylee150918
		MessageBox("  Save error.  bdl file  open!  ", MB_OK);
		return;
	}
 
    //son BDL 로딩을 다시 하라고 플래그를 일부러  -1로 만든 것으로 보임.
	nFlagFileLoad2=-1;//sylee180406-1
	
	if(nMulNetFlag<=1){//SYLEE150917-5
		nMulNetFlag=1;
	}

	if(nMulNetFlag<= MAX_DUT)//SYLEE150917-5//son220824_2 DefDutMax -> MAX_DUT1(32)
    {                                      //son220824_3 nMulNetFlag는 1~32이므로  33이 통과못하게 MAX_DUT로 수정. 
		::ZeroMemory(&SysInfoPieceRange.m_nPin[nMulNetFlag],sizeof(SysInfoPieceRange.m_nPin[nMulNetFlag]));
		g_nPieceMax1 = SysInfoPieceRange.m_nPieceCount[nMulNetFlag];
	}


    for( i = 1 ; i < (SysInfoPieceRange.m_nRow[nMulNetFlag]+1); i++) 
    { 
        for( k = 1 ; k < 4;  k++) 
        { 
            str= m_grid1.GetTextMatrix(i, k);  
            //if(nMulNetFlag<=DefDutMax)//SYLEE150917-5 
            if(nMulNetFlag<= MAX_DUT)//SYLEE150917-5 //son220824_2 DefDutMax(33) -> MAX_DUT(32)
            {                                        //son220824_3 nMulNetFlag는 1~32이므로  33이 통과못하게 MAX_DUT로 수정. 
                sscanf(LPCTSTR(str), "%d", &SysInfoPieceRange.m_nPin[nMulNetFlag][i-1][k-1]);
            } 
        }
    }

    ReadPinPieceNo(1);//sylee20120312
    ReadPinPieceNo(2);//sylee20120312
    ReadPinPieceNo(3);//sylee20120827
	ReadPinPieceNo(4);//sylee150817
 
    FileSysInfo01.LoadSavePieceRangeInfo(nMulNetFlag, _SAVE); //save	    //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 
	Onload(); //load    

}



void CModeSub11::OnButton103() //SYLEE150917-5
{
    CString str;
    int nMultyNetTotalTmp;

    if(nMultyNetTotal<1){//SYLEE150817
        nMultyNetTotal=1;
    } 

    //if(nMultyNetTotal>=1  &&  nMultyNetTotal<= DefDutMax )
    if(nMultyNetTotal>=1  &&  nMultyNetTotal<= MAX_DUT )  //son220824_2 DefDutMax(33) -> MAX_DUT(32)
    {                                                     //son220824_3 nMulNetFlag는 1~32이므로  33이 통과못하게 MAX_DUT(32)로 수정. 
        nMultyNetTotalTmp= nMultyNetTotal;
    }
    else{
        nMultyNetTotalTmp= MAX_DUT; //son220824_3 DefDutMax(33) -> MAX_DUT(32)
    }

    if(nMulNetFlag<0){//SYLEE150817
        nMulNetFlag=1;
    }
    else if(nMulNetFlag<nMultyNetTotalTmp)  //son nMulNetFlag=31이면 MAX_DUT(32)보다 작으므로 1증가 가능. 
    {                                       //    nMulNetFlag=32이면 1 증가 불가.
        nMulNetFlag++;
    }
    else if(nMulNetFlag>=nMultyNetTotalTmp)
    {
        nMulNetFlag=1;
    }

    str.Format("%d", nMulNetFlag);
    m_Label8.SetCaption(str);//SYLEE150917
    str.Format("%d", nMultyNetTotal);
    m_Label21.SetCaption(str);//SYLEE150917

    //sylee151016
    Onload();


}


void CModeSub11::OnSet1() 
{

    CString str; 
	int i1,i2,i3,i4,k, n1; 
    int i, nRowCt,nColCt;
	char cpBuffer[20];
 
	m_edit21.GetWindowText(str);    
    i1 =atoi(str); 
	m_edit22.GetWindowText(str);     
    i2 =atoi(str);

	i3=i2-i1;
     
	nColCt=4;
	nRowCt=SysInfoPieceRange.m_nPieceCount[nMulNetFlag]+1;

	m_grid1.SetCols(nColCt); 
	//m_grid1.SetRows(nRowCt*2);
	m_grid1.SetRows(nRowCt);

	for ( i = 1 ; i <nRowCt ; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format("Row No.%d", i); 
		m_grid1.SetText(str); 
	} 

 	m_scrollbar3.SetScrollPos(nRowCt);
 	SysInfoPieceRange.m_nRow[nMulNetFlag]=nRowCt-1;
//	m_scrollbar3.SetScrollPos(74);
//	SysInfoPieceRange.m_nRow=nRowCt*2-1;

	str.Format("%d", SysInfoPieceRange.m_nRow[nMulNetFlag]);
	m_Label3.SetCaption(str);
    i4=0;

    for( i=1 ; i<nRowCt; i++) 
   { 
		for( k=1 ; k<nColCt ;  k++) 
		{ 
			if(k==1){
			   n1=i;
			}else if(k==2){
				if(i==1){
					n1=i1;
				}else{
 				    n1=i4+1;
				}
				i4=n1;
			}else{
				 if(i==1){
					n1=i2;
				}else{
 				    n1=i4+i3;
				}
			//	n1=i2*i;
				i4=n1;
			}

			 sprintf(cpBuffer, "%d", n1  );
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }

///======================================== 
    m_edit23.GetWindowText(str);    
    i1 =atoi(str); 
	m_edit24.GetWindowText(str);     
    i2 =atoi(str);

	if( i1==0 && i2==0){

	}else{

		i3=i2-i1; 
		nColCt=4;
		nRowCt=SysInfoPieceRange.m_nPieceCount[nMulNetFlag]+1;

		m_grid1.SetCols(nColCt); 
		m_grid1.SetRows((nRowCt*2)-1);

		for ( i = nRowCt ; i < (nRowCt*2)-1 ; i++) 
		{ 
			m_grid1.SetCol(0); 
			m_grid1.SetRow(i); 
			str.Format("Row No.%d", i); 
			m_grid1.SetText(str); 
		} 

		m_scrollbar3.SetScrollPos(nRowCt*2);
		SysInfoPieceRange.m_nRow[nMulNetFlag]=(nRowCt*2)-2;

		str.Format("%d", SysInfoPieceRange.m_nRow[nMulNetFlag]);
		m_Label3.SetCaption(str);
		i4=0;

		for( i=nRowCt ; i< (nRowCt*2)-1; i++) 
	   { 
			for( k=1 ; k<nColCt ;  k++) 
			{ 

				if(k==1){
				   n1=i-nRowCt+1;
				}else if(k==2){
					if(i==nRowCt){
						n1=i1;
					}else{
 						n1=i4+1;
					}
					i4=n1;
				}else{
					 if(i==nRowCt){
						n1=i2;
					}else{
 						n1=i4+i3;
					}

				//	n1=i2*i;
					i4=n1;
				}

				 sprintf(cpBuffer, "%d", n1  );
				 m_grid1.SetTextMatrix(i, k, cpBuffer);
			}
	   } 

		 m_grid1.SetRow(1); 
		 m_grid1.SetCol(1);
	}

	 
	UpdateData(FALSE);

}



void CModeSub11::Onload() 
{
	int i, nRowCt,nColCt;
	int k; 
	CString str; 
	char cpBuffer[20];

	m_grid1.Clear();
	m_grid1.Refresh();
	m_grid1.SetRedraw(0);

	//if( nMulNetFlag>DefDutMax){//SYLEE150917-5
	if( nMulNetFlag> MAX_DUT){//SYLEE150917-5 //son220824_2 DefDutMax(33) -> MAX_DUT(32)
		nMulNetFlag=1;
	}
	if(nMulNetFlag>nMultyNetTotal){//SYLEE150917-5
		nMulNetFlag=1;
	}

    if(nMulNetFlag<1){//SYLEE180810-1-DUT16
		nMulNetFlag=1;
	}

	FileSysInfo01.LoadSavePieceRangeInfo(nMulNetFlag, _LOAD);   //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 	m_scrollbar1.SetScrollPos(SysInfoPieceRange.m_nPieceCount[nMulNetFlag]); 
	m_scrollbar2.SetScrollPos(SysInfoPieceRange.m_nPinTot[nMulNetFlag]); 
	m_scrollbar3.SetScrollPos(SysInfoPieceRange.m_nRow[nMulNetFlag]); 
	str.Format("%d", SysInfoPieceRange.m_nPieceCount[nMulNetFlag]);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nPinTot[nMulNetFlag]);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nRow[nMulNetFlag]);
	m_Label3.SetCaption(str);
	m_edit2=SysInfoPieceRange.m_nPieceCount[nMulNetFlag];
	m_edit3=SysInfoPieceRange.m_nPinTot[nMulNetFlag];
    m_edit4=SysInfoPieceRange.m_nRow[nMulNetFlag];

	nColCt=4;  
	m_grid1.SetCols(nColCt);	
	nRowCt=SysInfoPieceRange.m_nRow[nMulNetFlag]+1; 
	m_grid1.SetRows(nRowCt);
    
	for ( i=1 ; i<nRowCt ; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format("Row No.%d", i); 
		m_grid1.SetText(str); 
	} 
  
    for (  i=1 ; i < nColCt ; i++) 
    { 
        m_grid1.SetRow(0); 
        m_grid1.SetCol(i); 
		if( i==1){
           str.Format("Piece%d No(1~100) ", nMulNetFlag); 
		}else if( i==2){
           str.Format("Start Pin", i); 
		}else if( i==3){
           str.Format("Finish Pin", i); 
		}  
        m_grid1.SetText(str); 
    } 

   for( i=1 ; i<=SysInfoPieceRange.m_nRow[nMulNetFlag]; i++) 
   { 
		for( k=1 ; k < nColCt ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoPieceRange.m_nPin[nMulNetFlag][i-1][k-1]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }


/* //SYLEE180810-1
 if(nMulNetFlag<=1){//SYLEE150917-5

	FileSysInfo01.LoadSavePieceRangeInfo(1, _LOAD);     //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 	m_scrollbar1.SetScrollPos(SysInfoPieceRange.m_nPieceCount[1]); 
	m_scrollbar2.SetScrollPos(SysInfoPieceRange.m_nPinTot[1]); 
	m_scrollbar3.SetScrollPos(SysInfoPieceRange.m_nRow[1]); 
	str.Format("%d", SysInfoPieceRange.m_nPieceCount[1]);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nPinTot[1]);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nRow[1]);
	m_Label3.SetCaption(str);
	m_edit2=SysInfoPieceRange.m_nPieceCount[1];
	m_edit3=SysInfoPieceRange.m_nPinTot[1];
    m_edit4=SysInfoPieceRange.m_nRow[1];
	UpdateData(FALSE);
	nColCt=4;  
	m_grid1.SetCols(nColCt);	
	nRowCt=SysInfoPieceRange.m_nRow[1]+1; 
	m_grid1.SetRows(nRowCt);
    
	for ( i = 1 ; i <nRowCt ; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format("Row No.%d", i); 
		m_grid1.SetText(str); 
	} 
  
    for (  i = 1 ; i < nColCt ; i++) 
    { 
        m_grid1.SetRow(0); 
        m_grid1.SetCol(i); 
		if( i==1){
           str.Format("Piece No (1~100) ", i); 
		}else if( i==2){
           str.Format("Start Pin", i); 
		}else if( i==3){
           str.Format("Finish Pin", i); 
		}  
        m_grid1.SetText(str); 
    } 

   for( i = 1 ; i <=SysInfoPieceRange.m_nRow[1]; i++) 
   { 
		for( k = 1 ; k < nColCt ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoPieceRange.m_nPin[1][i-1][k-1]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }

//===============================================================================
 }else  if(nMulNetFlag==2){//SYLEE150917-5

	FileSysInfo01.LoadSavePieceRangeInfo(2, _LOAD);     //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 	m_scrollbar1.SetScrollPos(SysInfoPieceRange.m_nPieceCount[2]); 
	m_scrollbar2.SetScrollPos(SysInfoPieceRange.m_nPinTot[2]); 
	m_scrollbar3.SetScrollPos(SysInfoPieceRange.m_nRow[2]); 
	str.Format("%d", SysInfoPieceRange.m_nPieceCount[2]);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nPinTot[2]);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nRow[2]);
	m_Label3.SetCaption(str);
	m_edit2=SysInfoPieceRange.m_nPieceCount[2];
	m_edit3=SysInfoPieceRange.m_nPinTot[2];
    m_edit4=SysInfoPieceRange.m_nRow[2];
	UpdateData(FALSE);
	nColCt=4;  
	m_grid1.SetCols(nColCt);	
	nRowCt=SysInfoPieceRange.m_nRow[2]+1; 
	m_grid1.SetRows(nRowCt);
    
	for ( i = 1 ; i <nRowCt ; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format("Row No.%d", i); 
		m_grid1.SetText(str); 
	} 
  
    for (  i = 1 ; i < nColCt ; i++) 
    { 
        m_grid1.SetRow(0); 
        m_grid1.SetCol(i); 
		if( i==1){
           str.Format("Piece No (1~100) ", i); 
		}else if( i==2){
           str.Format("Start Pin", i); 
		}else if( i==3){
           str.Format("Finish Pin", i); 
		}  
        m_grid1.SetText(str); 
    } 

   for( i = 1 ; i <=SysInfoPieceRange.m_nRow[2]; i++) 
   { 
		for( k = 1 ; k < nColCt ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoPieceRange.m_nPin[2][i-1][k-1]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }
//===============================================================================
  //===============================================================================
 }else  if(nMulNetFlag==3){//SYLEE150917-5

	FileSysInfo01.LoadSavePieceRangeInfo(3, _LOAD);  //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 	m_scrollbar1.SetScrollPos(SysInfoPieceRange.m_nPieceCount[3]); 
	m_scrollbar2.SetScrollPos(SysInfoPieceRange.m_nPinTot[3]); 
	m_scrollbar3.SetScrollPos(SysInfoPieceRange.m_nRow[3]); 
	str.Format("%d", SysInfoPieceRange.m_nPieceCount[3]);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nPinTot[3]);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nRow[3]);
	m_Label3.SetCaption(str);
	m_edit2=SysInfoPieceRange.m_nPieceCount[3];
	m_edit3=SysInfoPieceRange.m_nPinTot[3];
    m_edit4=SysInfoPieceRange.m_nRow[3];
	UpdateData(FALSE);
	nColCt=4;  
	m_grid1.SetCols(nColCt);	
	nRowCt=SysInfoPieceRange.m_nRow[3]+1; 
	m_grid1.SetRows(nRowCt);
    
	for ( i = 1 ; i <nRowCt ; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format("Row No.%d", i); 
		m_grid1.SetText(str); 
	} 
  
    for (  i = 1 ; i < nColCt ; i++) 
    { 
        m_grid1.SetRow(0); 
        m_grid1.SetCol(i); 
		if( i==1){
           str.Format("Piece3 No (1~100) ", i); 
		}else if( i==2){
           str.Format("Start Pin", i); 
		}else if( i==3){
           str.Format("Finish Pin", i); 
		}  
        m_grid1.SetText(str); 
    } 

   for( i = 1 ; i <=SysInfoPieceRange.m_nRow[3]; i++) 
   { 
		for( k = 1 ; k < nColCt ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoPieceRange.m_nPin[3][i-1][k-1]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }
//===============================================================================
   //===============================================================================
 }else  if(nMulNetFlag==4){//SYLEE150917-5

	FileSysInfo01.LoadSavePieceRangeInfo(4, _LOAD);  //son210430 LoadSaveSub11 -> LoadSavePieceRangeInfo
 	m_scrollbar1.SetScrollPos(SysInfoPieceRange.m_nPieceCount[4]); 
	m_scrollbar2.SetScrollPos(SysInfoPieceRange.m_nPinTot[4]); 
	m_scrollbar3.SetScrollPos(SysInfoPieceRange.m_nRow[4]); 
	str.Format("%d", SysInfoPieceRange.m_nPieceCount[4]);
	m_Label1.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nPinTot[4]);
	m_Label2.SetCaption(str);
	str.Format("%d", SysInfoPieceRange.m_nRow[4]);
	m_Label3.SetCaption(str);
	m_edit2=SysInfoPieceRange.m_nPieceCount[4];
	m_edit3=SysInfoPieceRange.m_nPinTot[4];
    m_edit4=SysInfoPieceRange.m_nRow[4];
	UpdateData(FALSE);
	nColCt=4;  
	m_grid1.SetCols(nColCt);	
	nRowCt=SysInfoPieceRange.m_nRow[4]+1; 
	m_grid1.SetRows(nRowCt);
    
	for ( i = 1 ; i <nRowCt ; i++) 
	{ 
		m_grid1.SetCol(0); 
		m_grid1.SetRow(i); 
		str.Format("Row No.%d", i); 
		m_grid1.SetText(str); 
	} 
  
    for (  i = 1 ; i < nColCt ; i++) 
    { 
        m_grid1.SetRow(0); 
        m_grid1.SetCol(i); 
		if( i==1){
           str.Format("Piece4 No (1~100) ", i); 
		}else if( i==2){
           str.Format("Start Pin", i); 
		}else if( i==3){
           str.Format("Finish Pin", i); 
		}  
        m_grid1.SetText(str); 
    } 

   for( i = 1 ; i <=SysInfoPieceRange.m_nRow[4]; i++) 
   { 
		for( k = 1 ; k < nColCt ;  k++) 
		{ 
			 sprintf(cpBuffer, "%d", SysInfoPieceRange.m_nPin[4][i-1][k-1]);
		     m_grid1.SetTextMatrix(i, k, cpBuffer);
		}
   }
 }
  */
//===============================================================================   
//	m_combo25.SetCurSel(SysInfoPieceRange.m_nDut);   //sylee150917-9

    m_grid1.SetRow(1); 
    m_grid1.SetCol(1);
	m_grid1.Refresh();
 	m_grid1.SetRedraw(TRUE);
  	m_nCurRow = 1;	
	m_nCurCol = 1;
	m_grid1.GetWindowRect(m_rectGrid);
	ScreenToClient(m_rectGrid);
	m_dRatio = double(m_rectGrid.Width())/6860.0;
	OnEditing = FALSE;	

	UpdateData(FALSE);

}

 
void CModeSub11::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	long nScrollPos;
	CString strValue;	
	int i;
	CString str; 
	
	if(pScrollBar->GetSafeHwnd() == m_scrollbar1.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfoPieceRange.m_nPieceCount[nMulNetFlag]=nScrollPos;
				 
			}
			break;

	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <200){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label1.SetCaption(strValue);
				SysInfoPieceRange.m_nPieceCount[nMulNetFlag]=nScrollPos;				 
			}
			break;

	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label1.SetCaption(strValue);
			SysInfoPieceRange.m_nPieceCount[nMulNetFlag]=nPos;	 		
			break;
			}

/*
			ka=m_scrollbar2.GetScrollPos();
			SysInfoBlock.m_nBlock=ka*SysInfoBlock.m_nRow;
			m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
			strValue.Format("%d", SysInfoBlock.m_nBlock);
	        m_Label3.SetCaption(strValue);	
			
            i=SysInfoBlock.m_nRow ;
			j= SysInfoBlock.m_nCol ;
			i=i+1;	
			m_grid1.SetRows(i); 
			for ( j = 0 ; j < i-1; j++) 
			{ 
					m_grid1.SetCol(0); 
					m_grid1.SetRow(j+1); 
					str.Format("No. %d", j+1); 
					m_grid1.SetText(str); 
			} 
*/
 

	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar2.GetSafeHwnd()){
		switch(nSBCode){
		case SB_LINELEFT:		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfoPieceRange.m_nPinTot[nMulNetFlag]=nScrollPos;
 
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <32768){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label2.SetCaption(strValue);
				SysInfoPieceRange.m_nPinTot[nMulNetFlag]=nScrollPos;
 
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label2.SetCaption(strValue);
			SysInfoPieceRange.m_nPinTot[nMulNetFlag]=nPos;
 
			break;
		}

	/*	ka=m_scrollbar1.GetScrollPos();
		SysInfoBlock.m_nBlock=ka*SysInfoBlock.m_nCol;
		m_scrollbar3.SetScrollPos(SysInfoBlock.m_nBlock);
		strValue.Format("%d", SysInfoBlock.m_nBlock);
	    m_Label3.SetCaption(strValue);	
		
       // i= SysInfoBlock.m_nRow ;
		j= SysInfoBlock.m_nCol ;
		j=j+1;	
	//	m_grid1.SetRows(i); 
	     m_grid1.SetCols(j); 	 
		for ( i = 0 ; i < j-1; i++) 
		{ 
		    m_grid1.SetRow(0); 
            m_grid1.SetCol(i+1);
			str.Format("No. %d", i+1); 
			m_grid1.SetText(str); 
		} 
 
 */
	}else if(pScrollBar->GetSafeHwnd() == m_scrollbar3.GetSafeHwnd()){
	 	switch(nSBCode){
		case SB_LINELEFT: 		case SB_PAGELEFT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos > 0){
				pScrollBar->SetScrollPos(--nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfoPieceRange.m_nRow[nMulNetFlag]=nScrollPos;
 
			}
			break;
	     case SB_LINERIGHT:		 case SB_PAGERIGHT:
			nScrollPos = pScrollBar->GetScrollPos();
			if(nScrollPos <100){
				pScrollBar->SetScrollPos(++nScrollPos);	
                strValue.Format("%d", nScrollPos);
	            m_Label3.SetCaption(strValue);
				SysInfoPieceRange.m_nRow[nMulNetFlag]=nScrollPos;
 
			}
			break;
	     case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			strValue.Format("%d", nPos);
	        m_Label3.SetCaption(strValue);
			SysInfoPieceRange.m_nRow[nMulNetFlag]=nPos;
 
			break;
		}

		m_grid1.SetRows(SysInfoPieceRange.m_nRow[nMulNetFlag]+1); 
		for ( i = 1 ; i <=SysInfoPieceRange.m_nRow[nMulNetFlag] ; i++) 
		{ 
			m_grid1.SetCol(0); 
			m_grid1.SetRow(i); 
			str.Format("Row No.%d", i); 
			m_grid1.SetText(str); 
		}    

   
	}	
 
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
