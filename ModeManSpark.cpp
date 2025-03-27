// ModeManSpark.cpp : implementation file
//

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeManSpark.h"
#include "FileSysInfo.h"
#include "ModeSUB71.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeManSpark dialog

extern CSysSet13  SysSet13;
extern CFileSysInfo  FileSysInfo01;//SYLEE130618 
extern CModeSub71  Sub71;
extern CSysInfoSer13  SysInfoSer13;
extern CSysSet19  SysSet19;

extern float dSparkV2[10], dSparkI2[10];
extern int  nFlag_NewSparkBaord;


CModeManSpark::CModeManSpark(CWnd* pParent /*=NULL*/)
    : CDialog(CModeManSpark::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeManSpark)
    m_nSelectSparkVolt = -1;
    memset(m_dSparkRef, 0, sizeof(m_dSparkRef));
    memset(m_dSparkRead, 0, sizeof(m_dSparkRead));
    //}}AFX_DATA_INIT
}


void CModeManSpark::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeManSpark)
    DDX_Radio(pDX, IDC_RADIO_SPARK_V10, m_nSelectSparkVolt);
	DDX_Control(pDX, IDC_COMBO_SPARK_VOLT_SET, m_ctrlSparkVoltSet);
	DDX_Control(pDX, IDC_COMBO_SPARK_PIN_SEL, m_ctrlSparkPinSel);

    for (int i = 0; i < MAX_SPARK_CAL_NET-0; i++)
    {
//		DDX_Control(pDX, IDC_RADIO_SPARK_V10 - i, m_ctrlSparkRadioBtn[i]);

        DDX_Text(pDX, IDC_EDIT_SPARK_REF10 + i, m_dSparkRef[i]);
        DDV_MinMaxDouble(pDX, m_dSparkRef[i], 0., 250.);
        DDX_Text(pDX, IDC_EDIT_SPARK_READ_V10 + i, m_dSparkRead[i]);
        DDV_MinMaxDouble(pDX, m_dSparkRead[i], 0., 250.);
    }

    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeManSpark, CDialog)
    //{{AFX_MSG_MAP(CModeManSpark)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeManSpark message handlers

BEGIN_EVENTSINK_MAP(CModeManSpark, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeManSpark)
    ON_EVENT(CModeManSpark, IDC_BUTTON_SPARK_CAL, -600 /* Click */, OnButtonSparkRead, VTS_NONE)
    ON_EVENT(CModeManSpark, IDC_BUTTON_SPARK_READ_APPLY, -600 /* Click */, OnButtonSparkReadApply, VTS_NONE)
    ON_EVENT(CModeManSpark, IDC_OK, -600 /* Click */, OnOk, VTS_NONE)
    ON_EVENT(CModeManSpark, IDC_BUTTON_SPARK_REF_LOAD, -600 /* Click */, OnButtonSparkRefLoad, VTS_NONE)
    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CModeManSpark::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
    {
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CModeManSpark::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_nSelectSparkPin = -1;
	m_ctrlSparkVoltSet.SetCurSel(0);
	m_ctrlSparkPinSel.SetCurSel(0);
    ::ZeroMemory(m_dR1, sizeof(m_dR1));

	//hdpark230403 begin
	CString strSparkVoltLabel;
	int nSparkLabel1[] = { 90, 50, 40, 30, 20, 10,  2, 1, 0, 0};
	int nSparkLabel2[] = { 90, 60, 50, 40, 30, 20, 10, 8, 5, 2};

	for (int i = 0; i < MAX_SPARK_CAL_NET; i++)
	{
//		if (SysSet13.m_nPinBlockType >= PIN_BLK_32K_TRANS)
//#ifdef __GTS__CUSTOMER	// E&E 32K New board는 cal 값이 작아서 별도로 관리함. //hdpark231121, 모든 고객사 다 적용
#if 1
		{
			if (nSparkLabel1[i] != 0)
			{
				strSparkVoltLabel.Format("%d.   %d V", MAX_SPARK_CAL_NET - (i + 1), nSparkLabel1[i]);
				GetDlgItem(IDC_RADIO_SPARK_V10 - i)->SetWindowText(strSparkVoltLabel);
			}
			else
			{
				GetDlgItem(IDC_RADIO_SPARK_V10 - i)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SPARK_REF10 + i)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SPARK_READ_V10 + i)->ShowWindow(SW_HIDE);
			}
		}
#else
//		else
		{
			strSparkVoltLabel.Format("%d.   %d V", MAX_SPARK_CAL_NET - (i + 1), nSparkLabel2[i]);
			GetDlgItem(IDC_RADIO_SPARK_V10 - i)->SetWindowText(strSparkVoltLabel);
		}
#endif
	}
	//hdpark230403 end

	m_nMode = 327;
    m_nSelectSparkVolt = 0;
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CModeManSpark::OnButtonSparkRead()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

	m_nSelectSparkPin = m_ctrlSparkPinSel.GetCurSel();

	int nVoltSel = m_ctrlSparkVoltSet.GetCurSel();
	if     (nVoltSel == 0)	m_nMode = 327; // 250 V
	else if(nVoltSel == 1)	m_nMode = 302; // 200 V
	else if(nVoltSel == 2)	m_nMode = 277; // 150 V
	else if(nVoltSel == 3)	m_nMode = 252; // 100 V
	else if(nVoltSel == 4)	m_nMode = 222; //  40 V

    OnSparkCal();
    m_nSelectSparkVolt = (m_nSelectSparkVolt + 1) % MAX_SPARK_CAL_NET;
	if(m_nSelectSparkVolt > MAX_SPARK_CAL_NET - 1)	m_nSelectSparkVolt = 0;

    UpdateData(FALSE);

}

void CModeManSpark::OnButtonSparkReadApply()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

	int nVoltSel = m_ctrlSparkVoltSet.GetCurSel();
	if     (nVoltSel == 0)	m_nMode = 327; // 250 V
	else if(nVoltSel == 1)	m_nMode = 302; // 200 V
	else if(nVoltSel == 2)	m_nMode = 277; // 150 V
	else if(nVoltSel == 3)	m_nMode = 252; // 100 V
	else if(nVoltSel == 4)	m_nMode = 222; //  40 V

    sprintf(m_szFile, "%s\\SETUP\\CAL\\CALR%02dK2.INI", SysInfoView01.m_pStrFilePath1, m_nMode);
    m_fp = fopen(m_szFile, "w + t");

    for (int i = 0; i < MAX_SPARK_CAL_NET; i++)
    {
//#ifdef __GTS__CUSTOMER	//hdpark231121, 모든 고객사 다 적용
		if(i >= MAX_SPARK_CAL_NET - 3) break;
//#endif
        if (m_dSparkRead[i] < 1.0)
        {
            AfxMessageBox("Calibration is not finished. Please Complete all the voltages.");
            return;
        }
//        SysInfoSer13.m_nData[PART_SPARK][m_nMode][CAL_REF_RAWR][i + 1] = m_dSparkRead[i];

        if (m_fp)    fprintf(m_fp, "%.2f \r\n", m_dSparkRead[i]);
    }
    if (m_fp)    fclose(m_fp);

//    FileSysInfo01.LoadSaveSer13(_SAVE, m_nMode, PART_SPARK);

    OnButtonSparkRefLoad();

    m_nSelectSparkVolt = 0;

    UpdateData(FALSE);
}

void CModeManSpark::OnButtonSparkRefLoad()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

	int nVoltSel = m_ctrlSparkVoltSet.GetCurSel();
	if     (nVoltSel == 0)	m_nMode = 327; // 250 V
	else if(nVoltSel == 1)	m_nMode = 302; // 200 V
	else if(nVoltSel == 2)	m_nMode = 277; // 150 V
	else if(nVoltSel == 3)	m_nMode = 252; // 100 V
	else if(nVoltSel == 4)	m_nMode = 222; //  40 V

//    FileSysInfo01.LoadSaveSer13(_LOAD, m_nMode, PART_SPARK);

    sprintf(m_szFile, "%s\\SETUP\\CAL\\CALR%02dK2.INI", SysInfoView01.m_pStrFilePath1, m_nMode);
    m_fp = fopen(m_szFile, "r + t");

    char szBuff[32];

    for (int i = 0; i < MAX_SPARK_CAL_NET; i++)
    {
        if (m_fp)    fscanf(m_fp, "%s", szBuff);

        m_dSparkRef[i] = m_dSparkRead[i] = atof(szBuff);


//        m_dSparkRef[i] = SysInfoSer13.m_nData[PART_SPARK][m_nMode][CAL_REF_RAWR][i + 1];
//        m_dSparkRead[i] = SysInfoSer13.m_nData[PART_SPARK][m_nMode][CAL_REF_RAWR][i + 1];
    }

    if (m_fp)    fclose(m_fp);

    UpdateData(FALSE);
}

void CModeManSpark::OnOk()
{
    // TODO: Add your control notification handler code here
    CDialog::OnOK();
}


int CModeManSpark::OnSparkCal()
{
    int nSparkOnFlag = 0;
    int nPin1, nPin2;	// hdpark231121, user pin
    FILE* fp2;
    fp2 = fopen("C:\\ACE500\\spark.ini", "rt");
    if (fp2)	fscanf(fp2, "%d %d %d", &nSparkOnFlag, &nPin1, &nPin2);
    if (fp2)	fclose(fp2);

    FileSysInfo01.LoadSaveSet19(2);
    FileSysInfo01.LoadSaveSet12(2);

    if (SysSet19.m_nSet12 != 1)     nFlag_NewSparkBaord = 2;
    else                            nFlag_NewSparkBaord = 0;

    if (SysSet13.m_nSparkMode == SPARK_NEW)     Sub71.OnSparkADC(0);

//    int m_nMode1 = 327;

    int nCC = (int)dRec[m_nMode][1];  // ADC A   1= I FAST,  2=I SLOW      ( 3 =VB )
    int nVRel = (int)dRec[m_nMode][2];
    int nVSet = dRec[m_nMode][3];
    int nIR = (int)dRec[m_nMode][4];
    int nISet = dRec[m_nMode][5];
    int nFil = (int)dRec[m_nMode][8];  // ADC B   1=VC FAST, 2= VC SLOW ,  3=HV OPEN, ( 4=4W )
    int nTimeDelay1 = (int)(dRec[m_nMode][6] * 10);
    SysInfoSer13.m_nData[PART_SPARK][m_nMode][0][1] = (double)nTimeDelay1;

    int nIFilter = (int)dRec[m_nMode][9];
    int nHVOffSet = (int)dRec[m_nMode][10];
    int nHVGain = (int)dRec[m_nMode][11];
    int nIGain = (int)dRec[m_nMode][12];
    int n4wGain = (int)dRec[m_nMode][13];

    Sub71.nADCount1 = (int)dRec[m_nMode][7];

    int nRet1 = Sub71.AOnPoweroff2();
    if (nRet1 != 1) 
    {
		Initialize();
        AfxMessageBox(" Error No 1023, \n   Power all off fail !,   SW TEST Run Start Error!!!");
        return 0;
    }

    if (nVRel == 1)             nRet1 = Sub71.AOnVSetMulti1(nVSet, 0, 0, 0);
    else  if (nVRel == 2)       nRet1 = Sub71.AOnVSetMulti1(0, nVSet, 0, 0);
    else  if (nVRel == 3)       nRet1 = Sub71.AOnVSetMulti1(0, 0, nVSet, 0);
    else  if (nVRel == 4)       nRet1 = Sub71.AOnVSetMulti1(0, 0, 0, nVSet);

    if (nRet1 != 1) 
    {
		Initialize();
        AfxMessageBox(" Error No 1023, \n   V SET ERROR !,   SW TEST Run Start Error!!!");
        return 0;
    }

    Sub71.AOnReadVB1(nVRel); 

    if (nFil == 1 || nFil == 2)     Sub71.AOnReadV1Enable1(nVRel);
    else                            Sub71.AOnReadV1Enable1(5);

    Sub71.FilterSet(nFil);
    if (nIFilter == 3) 
    {
        AfxMessageBox(" Errro No 2023, \n   FilterSet SET ERROR !  vb=3?,  select 1-2 !      ,  R Calibraion  Error!!!");
        nIFilter = 1;
    }
    Sub71.I_Filter(nIFilter);

    if (nIGain == 2)        Sub71.I_Gain(2);     
    else                    Sub71.I_Gain(1); 

    Sub71.On4W_GainSet(1);
    Sub71.AOnISetOn1Run1(nCC, nVRel, nVSet, nIR, nISet);
    Sub71.OnPinAllReset1();

    memset(dSparkV2, 0, sizeof(dSparkV2));
    memset(dSparkI2, 0, sizeof(dSparkI2));

    int nSparkset = SysSet19.m_nSet35; // spark offset
    Sub71.OnSparkADC(1);
    Sub71.AOnVBSet1(5, nSparkset);
    Sub71.OnSparkOn(0);
    Sub71.Delay_100ns(10000);
    Sub71.OnSparkOn(1);

    Sub71.m_bModifySparkMeas = TRUE;
    Sub71.m_bVBReadMode = TRUE;
    Sub71.SetVBReadMode();
    Sub71.Delay_100ns(50000);
    Sub71.SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_ON);    // VB Mode Set

    double dSparkMaxValue = 0.0;

    Sub71.OnPinAllReset1();

    Sub71.Delay_100ns(nTimeDelay1);

	int a1, a2;

	if(m_nSelectSparkPin == 0)
	{
		a1 = 17 - 1;
		a2 = 25 - 1;
	}
	else if(m_nSelectSparkPin == 1)
	{
		a1 = 9 - 1;
		a2 = 17 - 1;
	}
    else if (m_nSelectSparkPin == 2)	//hdpark231121, s100 jig
    {
        a1 = 63 - 1;
        a2 = 64 - 1;
    }
	else if (m_nSelectSparkPin == 5)	//hdpark231121, user pin
	{
		a1 = nPin1 - 1;
		a2 = nPin2 - 1;
	}

    Sub71.AOnPinSet1ForSparkCal(2, a1, 1);
    Sub71.AOnPinSet1ForSparkCal(2, a2, 2);

//    Sub71.Delay_100ns(150);

    int nAdcReadCount = 50;
    Sub71.ReadSparkForCal(nAdcReadCount);
    if (Sub71.OnSparkStatus() != 1)
    {
		Initialize();
        AfxMessageBox("Spark Fail!");
		return 0;
    }

    if (nSparkOnFlag)
    {
        Sub71.OnSparkOn(0);
        Sub71.Delay_100ns(20000);
        Sub71.OnSparkOn(1);
    }

    int nMaxVsIndex = 0;
    for (int i = 0; i < nAdcReadCount; i++)
    {
        if (i == 0)
        {
            dSparkMaxValue = dSparkV2[i];
        }
        else
        {
            if (dSparkMaxValue < dSparkV2[i])
            {
                dSparkMaxValue = dSparkV2[i];
                nMaxVsIndex = i;
            }
        }
    }

    int nSparkMaxValue = dSparkMaxValue * 100.0;
    dSparkMaxValue = nSparkMaxValue / 100.0;
    m_dR1[m_nSelectSparkVolt] = dSparkMaxValue;
    m_dSparkRead[m_nSelectSparkVolt] = dSparkMaxValue;

	if(m_nSelectSparkVolt == MAX_SPARK_CAL_NET - 1 - 0)
	{
		//m_dSparkRead[m_nSelectSparkVolt + 1] = dSparkMaxValue;
		//m_dSparkRead[m_nSelectSparkVolt + 2] = dSparkMaxValue;
	}

	Initialize();

    return 0;
}

int CModeManSpark::Initialize()
{
    Sub71.OnPinAllReset1();
    Sub71.AOnPoweroff2();
    Sub71.FilterSet(0);
    Sub71.I_Filter(0);
    Sub71.AOnReadV1Enable1(5);
    Sub71.OnCCCV_Set(1);

    Sub71.SendCmdToDigitizer(__LINE__, 0, SPARK_TRG_OFF);    // VB Mode Set

    Sub71.OnSparkADC(0);
    Sub71.OnSparkOn(0);

	return 0;
}
