// ModeMan16.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "comidaq.h"//SYLEE
#include "comiled.h"//SYLEE
#include "comitoggleswitch.h"//SYLEE
#include "ComiDaqComm.h" //SYLEE
#include "Dio.h" //SYLEE
#include "comiled.h"
#include "resource.h" //SYLEE

#include "ModeMan16.h"
 
CDio	DIO; //SYLEE

#define COMI_SD414	0xB414
#define COMI_SD424	0xB424

extern CString LoadMessage(UINT uFileType, CString strFile, UINT uNum); 
extern int  gDef_Language1;   //1=KOREA ,2=ENGLISH, 3=CHINA


#define MODE_MAN16_TIMER_DIO    70      //son231017

/////////////////////////////////////////////////////////////////////////////
// CModeMan16 dialog


CModeMan16::CModeMan16(CWnd* pParent /*=NULL*/)
	: CDialog(CModeMan16::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeMan16)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModeMan16::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeMan16)
	DDX_Control(pDX, IDC_COMILEDCTRL1, m_LED0);
	DDX_Control(pDX, IDC_COMILEDCTRL2, m_LED1);
	DDX_Control(pDX, IDC_COMILEDCTRL3, m_LED2);
	DDX_Control(pDX, IDC_COMILEDCTRL4, m_LED3);
	DDX_Control(pDX, IDC_COMILEDCTRL5, m_LED4);
	DDX_Control(pDX, IDC_COMILEDCTRL6, m_LED5);
	DDX_Control(pDX, IDC_COMILEDCTRL7, m_LED6);
	DDX_Control(pDX, IDC_COMILEDCTRL8, m_LED7);
	DDX_Control(pDX, IDC_COMILEDCTRL9, m_LED_4WRef_Init);

	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL1, m_Btn0);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL2, m_Btn1);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL3, m_Btn2);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL4, m_Btn3);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL5, m_Btn4);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL6, m_Btn5);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL7, m_Btn6);
	DDX_Control(pDX, IDC_COMITOGGLESWITCHCTRL8, m_Btn7);
	DDX_Control(pDX, IDC_COMIDAQCTRL1, m_ComiDaq1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeMan16, CDialog)
	//{{AFX_MSG_MAP(CModeMan16)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
 
	ON_WM_TIMER()
	ON_WM_CLOSE()

// 	ON_BN_CLICKED(IDC_BUTTON3, OnDoOut)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

 
BEGIN_EVENTSINK_MAP(CModeMan16, CDialog)  //SYLEE20110310
    //{{AFX_EVENTSINK_MAP(CModeMan16)
	    ON_EVENT(CModeMan16, IDC_BUTTON101, -600 /* SEND Click */, OnDoOut, VTS_NONE)
		ON_EVENT(CModeMan16, IDC_OK,        -600 /* Exit Click */, OnClickOk, VTS_NONE) 
//	ON_EVENT(CChildView1, IDC_BUTTON_1001, 3, OnBeforeDragOverButton1001, VTS_DISPATCH VTS_DISPATCH VTS_R4 VTS_R4 VTS_I4 VTS_DISPATCH VTS_I2)
	//}}AFX_EVENTSINK_MAP

END_EVENTSINK_MAP()
 



/////////////////////////////////////////////////////////////////////////////
// CModeMan16 message handlers

BOOL CModeMan16::OnInitDialog()
{
 	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
/*	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
 
	// TODO: Add extra initialization here
 */

 //	m_ComiDaq1.SetDeviceId(COMI_SD414);
 	m_ComiDaq1.SetDeviceId(COMI_SD424);  
	m_ComiDaq1.SetDevInstance(0);

//	if (m_ComiDaq1.GnDeviceLoad() == cmFALSE) {
	if (m_ComiDaq1.GnDeviceLoad() == 0) {
		AfxMessageBox(" \n\n Communication  Error!, \n\n  Comizoa Digital Card 1 Check!  \n\n  ");
	}
  
	DIO.m_pBoard[0] = &m_ComiDaq1;

	m_LedArr[DIN_START]       = &m_LED0;            //son231016 0: DIN_START
	m_LedArr[DIN_TEST]        = &m_LED1;            //son231016 1: DIN_TEST
	m_LedArr[DIN_ABORT]       = &m_LED2;            //son231016 2: DIN_ABORT
	m_LedArr[DIN_SKIP]        = &m_LED3;            //son231016 3: DIN_SKIP
	m_LedArr[DIN_REVERSE]     = &m_LED4;            //son231016 4: DIN_REVERSE 
	m_LedArr[DIN_DUT1]        = &m_LED5;            //son231016 5: DIN_DUT1
	m_LedArr[DIN_DUT2]        = &m_LED6;            //son231016 6: DIN_DUT2
	m_LedArr[DIN_RESHEET]     = &m_LED7;            //son231016 7: DIN_RESHEET
	m_LedArr[DIN_4W_REF_INIT] = &m_LED_4WRef_Init;  //son231016 8: DIN_4W_REF_INIT

	m_BtnArr[DOUT_LOC_PASS]            = &m_Btn0;  //son231016 0: DOUT_LOC_PASS
	m_BtnArr[DOUT_LOC_SHORT]           = &m_Btn1;  //son231016 1: DOUT_LOC_SHORT
	m_BtnArr[DOUT_LOC_OPEN]            = &m_Btn2;  //son231016 2: DOUT_LOC_OPEN
	m_BtnArr[DOUT_LOC_RETEST]          = &m_Btn3;  //son231016 3: DOUT_LOC_RETEST
	m_BtnArr[DOUT_LOC_CONFAIL]         = &m_Btn4;  //son231016 4: DOUT_LOC_CONFAIL
	m_BtnArr[DOUT_LOC_SAMPLING_END]    = &m_Btn5;  //son231016 5: DOUT_LOC_SAMPLING_END
	m_BtnArr[DOUT_LOC_RESERVED1]       = &m_Btn6;  //son231016 6: DOUT_LOC_RESERVED1
	m_BtnArr[DOUT_LOC_ACK_4W_REF_INIT] = &m_Btn7;  //son231016 7: DOUT_LOC_ACK_4W_REF_INIT

#ifdef _PC_TEST_
    //son PC test용으로 2초 타이머로 설정.
    SetTimer(MODE_MAN16_TIMER_DIO, 2000, NULL);  
#else
	// DI Check Timer 주기 100ms
	SetTimer(MODE_MAN16_TIMER_DIO, 100, NULL);      //son231017
#endif
  
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CModeMan16::A_Language()  //LANGUAGE CHANGE
{
  if( gDef_Language1==3){ 
 
	  m_Label101.SetWindowText(LoadMessage(1,"TitleSetupDigital", 100));
	  m_Label102.SetWindowText(LoadMessage(1,"TitleSetupDigital", 200));

	  m_button1001.SetWindowText(LoadMessage(1,"TitleSetupDigital", 206));  
	  m_buttonOk.SetWindowText(LoadMessage(1,"TitleSetupDigital", 207));  
	   
  }

} 


void CModeMan16::ExitInOutBoard()
{
	for (int i=2000; i<2128; i++) {
		DIO.SetOff(i);
	}
	BOOL nRet;
//	if(m_ComiDaq1.GnDeviceIsLoaded() == cmFALSE) {
	//	if(m_ComiDaq1.GnDeviceUnload() == cmFALSE) {
	if(m_ComiDaq1.GnDeviceIsLoaded() == 0) {
		if(m_ComiDaq1.GnDeviceUnload() == 0) {
			nRet = FALSE;
		}
	} 
}


void CModeMan16::OnSysCommand(UINT nID, LPARAM lParam)
{
/*	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	*/
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CModeMan16::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CModeMan16::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



//******************************************************************************
//
//   DI 타이머 루틴
//    - 주기적으로 전 채널의 DI state 를 읽어들인 후 LED 에 표시 합니다.
//
//******************************************************************************
void CModeMan16::OnTimer(UINT nIDEvent) 
{
	int nCurStates;
	int nTempState;

//son231017 begin:
    switch (nIDEvent) 
    {   
        case MODE_MAN16_TIMER_DIO: //son201028 case 70: DIO 50ms
        {
#ifdef _PC_TEST_
TRACE("ModeMan16: Recv MODE_MAN16_TIMER_DIO\n");
            //son Debug 중 Timer가 너무 많이 쌓여서 오동작하는 일을 막기 위해  timer가 
            //    _PC_TEST_ 디버그시에는 더이상 들어오지 않게 일단 중지하고 이  timer 처리 완료 시점에 다시 시작한다.
            KillTimer(MODE_MAN16_TIMER_DIO);

#endif
	
            // Get DI Status
            nCurStates = m_ComiDaq1.DiGetAll();
#ifdef __2BYTE_DIO__    //son231018
            for (int i=0; i< (DIN_4W_REF_INIT +1); i++) //son230914 0 ~8(DIN_4W_REF_INIT) 까지는 read 하도록 수정 
#else
            for (int i=0; i< DIN_NUM; i++) //son231018 8:DIN_NUM   0 ~7까지 read
#endif
            {
                nTempState = (nCurStates >> i) & 0x1;
                m_LedArr[i]->SetState(nTempState);
            }
#ifdef _PC_TEST_
    //son 디버그중 타이머 쌓이는 현상을 막기 위해 Timer처리 완료 시점에 다시 시작
    SetTimer(MODE_MAN16_TIMER_DIO, 2000, NULL);  
#endif

        }
        break;

        default:
        break;
    }
//son231017 end

	CDialog::OnTimer(nIDEvent);
}

//******************************************************************************
//
//   Out 버튼의 콜백 함수
//    - 현재 설정되어 있는 Digital Output 버튼 설정에 맞게 전 채널의 Output State 를 변경합니다.
//
//******************************************************************************
void CModeMan16::OnDoOut() 
{
	int nOutStates = 0;
	
	for(int i=0; i< DOUT_NUM; i++){     //son231016 8-> DOUT_NUM
	    //	nOutStates=3;  stop   pickup  20110302
		//	nOutStates=1;  start  pickup  20110302
		if (m_BtnArr[i]->GetState())
			nOutStates |= (1 << i);
		else 
 		nOutStates &= ~(1 << i);
	}

	m_ComiDaq1.DoPutAll(nOutStates);
}

//******************************************************************************
//
//    OnClose 콜백함수
//     - UnLoadDevice 합니다.
//
//******************************************************************************
void CModeMan16::OnClose() 
{
	KillTimer(0);
	m_ComiDaq1.GnDeviceUnload();
	
	CDialog::OnClose();
}


void CModeMan16::OnClickOk() 
{ 
	 //OnClose();
 	// FileSysInfo01.LoadSaveSer11(2);
 	CDialog::OnOK();
}
