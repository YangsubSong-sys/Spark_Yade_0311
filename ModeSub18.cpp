// ModeSub18.cpp : implementation file
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "stdafx.h"
#include "BiOpTroCon.h"
#include "ModeSub18.h" 
#include "ModeSub71.h"
//#include "BiOpTroCon.h"
//#include "ModeSer13.h"
//#include "ChildView7.h"
#include "FileSysInfo.h"
#include "Contec.h" 
//#include "A_Define1.h"    //son190628

 
#define defCol1 21 


int defRow1Temp1;
int defRow1Temp2;

int g_flag_CheckLow;//SYLEE160503   //son210916 
int g_flag_CheckHigh;//SYLEE160503  //son210916
int g_flag_LimitPasswdOk;    //son220914_2 GTS �䱸����. passwd üũ ��� �߰�.

//son211012  int -> double�� ���� (Low Sigma Coef �߰�����)
//son220308  gd4W1_P1 -> gd4W1_P1Low
double gd4W1_P1Low[MAX_LIMIT4W_RNG+10][LIMIT4W_NUM];//SYLEE160503       //son "4W_Per_Low.ini" ���� 
                    //son  gd4W1_P1Low[j][1:LIMIT4W_R_START]    : mOhm R Lower Limit Range Start 
                    //son  gd4W1_P1Low[j][2:LIMIT4W_R_END]      : mOhm R Lower Limit Range End 
                    //son  gd4W1_P1Low[j][3:LIMIT4W_PERCENT]    : Low Percent 
                    //son  gd4W1_P1Low[j][4:LIMIT4W_OFFSET]     : mOhm Low Offset
                    //son  gd4W1_P1Low[j][5:LIMIT4W_SIGMA_COEF] : Low Sigma Coefficient   //son211012 �߰�
                    //
                    //----- ModeSub17�� ModeSub18�� ������ ������ (��Ģ ���)  ---
                    //son gd4W1_P1Low[0][0:SET4W_LIMIT_USE]     : ModeSub18:: -Low Use (m_check1) ����
                                        
//son 20�������� ���
//son220308  gd4W1_P2 -> gd4W1_P2High
double gd4W1_P2High[MAX_LIMIT4W_RNG+10][LIMIT4W_NUM];//SYLEE160503    //son "4W_Per_High.ini" ����  : j=1~ 20 ���
                    //son  gd4W1_P2High[j][1:LIMIT4W_R_START]    : mOhm R High(Upper Limit) Range Start
                    //son  gd4W1_P2High[j][2:LIMIT4W_R_END]      : mOhm R High(Upper Limit) Range End
                    //son  gd4W1_P2High[j][3:LIMIT4W_PERCENT]    : High Percent 
                    //son  gd4W1_P2High[j][4:LIMIT4W_OFFSET]     : mOhm High Offset
                    //son  gd4W1_P2High[j][5:LIMIT4W_SIGMA_COEF] : High Sigma Coefficient 
                    //
                    //----- ModeSub17�� ModeSub18�� ������ ������ (��Ģ ���)  ---
                    //son gd4W1_P2High[0][0:SET4W_LIMIT_USE]          : ModeSub18:: +High Use (m_check2)  ���� 
                    //son gd4W1_P2High[0][1:SET4W_SIGMA_COEF_USE]     : ModeSub17�� Sigma Coefficient Enable�̸� 1
                    //son gd4W1_P2High[0][2:SET4W_EJECT_LOW_HIGH_USE] : ModeSub17�� 'EJECT COUNT  LOW  HIGH' ���� �����Ǹ� 1  
                    //son gd4W1_P2High[0][3:SET4W_HIGH_ALL_PERCNT_USE]: ModeSub18:: All % Use   (m_check4) ����
                    //son gd4W1_P2High[0][4:SET4W_HIGH_ALL_PERCNT_VAL]: ModeSub18:: All % Value (m_edit4) ����
                    //
                    //son gd4W1_P2High[21][1:SET4W_AVG_PERCNT_VAL] : ModeSub17�� 'Average %' edit value (m_edit6) ����
                    //son gd4W1_P2High[21][2:SET4W_AVG_PERCNT_USE] : ModeSub17�� 'Average % Use' On     (m_check6) ����
 
 

extern CSysInfo05  SysInfo05; 
extern CSysInfoSer13  SysInfoSer13;

extern CSysSet13  SysSet13;  

extern double dCalA1[10][5];//sylee140106 
extern int dRec4w[6][11];

extern int dRec4w2[10][5];//sylee140714

extern  int nAutoProcess1;//sylee20120202


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern int nManualCount100;   //sylee20120131
extern int nManualCount100Flag;   //sylee20120131
//extern float nManualR1[101][dSet4WTot1];  //sylee20120131   [300]ȸ ,  [1000]=net   300->101 //sylee160223-6   double -> float
//extern int n4WFlag[10][dSet4WTot1];//  [10]  =mode total no.,  [ 3000]= 4w net listn total no.
extern float nManualR1[101][MAX_4W_NET +1];  //son231107 dSet4WTot1 ->  MAX_4W_NET +1 
extern int n4WFlag[10][MAX_4W_NET +1];       //son231107 dSet4WTot1 ->  MAX_4W_NET +1

extern int nCC,  nVRel,    nIR;
extern double nVSet;


 
//extern int  CalibrationLS1(int No,  double  dRealStart,   double  dRealEnd);//sylee140106 
//extern void  CalibrationLS1_Func(); //sylee140708   
extern void DoEvents(); 
 
extern __int64 GetMilSecond2();   
extern CSysInfoView01  SysInfoView01;
extern CFileSysInfo FileSysInfo01;  
extern CSysInfoSer13  SysInfoSer13;
extern CModeSub71  Sub71;

extern double V1;  //vb
extern double Y1;  //i
extern double X1;  //vc
//extern double R1;  //vc
extern double W1;  //4WIRE

extern int nTimeDelay1;

extern double nCalC[30][10][100];

extern  int n4WManualCount;


extern double dR14w[15][MAX_4W_NET +1];//sylee170803  110->15   //son231107 dSet4WTot1 ->  MAX_4W_NET +1
extern double dVB14w[15][MAX_4W_NET +1];//sylee170803  110->15  //son231107 dSet4WTot1 ->  MAX_4W_NET +1
extern double dVC14w[15][MAX_4W_NET +1];//sylee170803  110->15  //son231107 dSet4WTot1 ->  MAX_4W_NET +1
extern double dI14w[15][MAX_4W_NET +1];//sylee170803  110->15   //son231107 dSet4WTot1 ->  MAX_4W_NET +1
extern double dR14wT1[15][MAX_4W_NET +1];//sylee170803  110->15 //son231107 dSet4WTot1 ->  MAX_4W_NET +1
  

/////////////////////////////////////////////////////////////////////////////
// CModeSub18 dialog


CModeSub18::CModeSub18(CWnd* pParent /*=NULL*/)
    : CDialog(CModeSub18::IDD, pParent)
{
    //{{AFX_DATA_INIT(CModeSub18)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CModeSub18::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CModeSub18)
        // NOTE: the ClassWizard will add DDX and DDV calls here
 
      DDX_Control(pDX, IDC_GRID_1, m_grid1);
      DDX_Control(pDX, IDC_GRID_2, m_grid2);
      DDX_Control(pDX, IDC_EDIT1,   m_edit1);
      DDX_Control(pDX, IDC_EDIT2,   m_edit2);  
      DDX_Control(pDX, IDC_EDIT4,   m_edit4);
       DDX_Control(pDX, IDC_CHECK1, m_check1);   
       DDX_Control(pDX, IDC_CHECK2, m_check2);    
       DDX_Control(pDX, IDC_CHECK4, m_check4);  

       DDX_Control(pDX, IDC_BUTTON101, m_button1);
       DDX_Control(pDX, IDC_BUTTON102, m_button2);
 
       //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeSub18, CDialog)
    //{{AFX_MSG_MAP(CModeSub18)
        // NOTE: the ClassWizard will add message map macros here
        ON_WM_MOUSEWHEEL()
        ON_WM_HSCROLL()
        ON_WM_VSCROLL()
     //   ON_BN_CLICKED(IDC_GRID_1, OnDblClickMsflexgrid1)
     //   ON_BN_CLICKED(IDC_GRID_2, OnDblClickMsflexgrid2)
 
      //  ON_BN_CLICKED(IDC_BUTTON101, OnButton101)
      //  ON_BN_CLICKED(IDC_BUTTON102, OnButton102) 

        ON_BN_CLICKED(IDC_CHECK1, OnCheck01) 
        ON_BN_CLICKED(IDC_CHECK2, OnCheck02)        
        ON_BN_CLICKED(IDC_CHECK4, OnCheck04) 


    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSub18 message handlers

BEGIN_EVENTSINK_MAP(CModeSub18, CDialog)
    //{{AFX_EVENTSINK_MAP(CModeSub18)

    ON_EVENT(CModeSub18, IDC_OK, -600 /* Click */, OnClickOk, VTS_NONE) 
 
    ON_EVENT(CModeSub18, IDC_GRID_1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSub18, IDC_GRID_1, 71 /* EnterCell */, OnEnterCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSub18, IDC_GRID_1, 72 /* LeaveCell */, OnLeaveCellMsflexgrid1, VTS_NONE)
    ON_EVENT(CModeSub18, IDC_GRID_1, -607 /* MouseUp */, OnMouseUpMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)

    ON_EVENT(CModeSub18, IDC_GRID_2, -601 /* DblClick */, OnDblClickMsflexgrid2, VTS_NONE)
    ON_EVENT(CModeSub18, IDC_GRID_2, 71 /* EnterCell */, OnEnterCellMsflexgrid2, VTS_NONE)
    ON_EVENT(CModeSub18, IDC_GRID_2, 72 /* LeaveCell */, OnLeaveCellMsflexgrid2, VTS_NONE)
    ON_EVENT(CModeSub18, IDC_GRID_2, -607 /* MouseUp */, OnMouseUpMsflexgrid2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
 
    ON_EVENT(CModeSub18, IDC_BUTTON101,-600 /* Click */, OnButton101, VTS_NONE)
                                                            //son Low Set
                                                            
    ON_EVENT(CModeSub18, IDC_BUTTON102,-600 /* Click */, OnButton102, VTS_NONE)
                                                            //son High Set
 
    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


//son  m_check1.GetCheck()�� ���°� Ư¡�� �Լ�
//     �ܼ��� ������ ������  g_flag_CheckLow���� �ݴ�� ����ϱ⸸ �Ѵ�.
void CModeSub18::OnCheck01() 
{
 
     if(g_flag_CheckLow!=1){    //son210916
    //  m_grid1.SetEnabled(1);
        m_grid1.EnableWindow(1);
        m_check1.SetCheck(1); 
        m_button1.EnableWindow(1); 
        g_flag_CheckLow=1;  //son210916
     }
     else{ 
        m_grid1.EnableWindow(0); 
        m_check1.SetCheck(0);  
        m_button1.EnableWindow(0); 
        g_flag_CheckLow=0;  //son210916
        EndEditing1();
     }
    // sprintf(cBuf, "1141");//testing
     m_grid1.SetTextMatrix(1, 1, "0"); 
}

//son  m_check2.GetCheck()�� ���°� Ư¡�� �Լ�
//     �ܼ��� ������ ������  g_flag_CheckHigh���� �ݴ�� ����ϱ⸸ �Ѵ�.
void CModeSub18::OnCheck02() 
{
    
     if(g_flag_CheckHigh!=1){//son210916
    //  m_grid2.SetEnabled(1);
        m_grid2.EnableWindow(1);
        m_check2.SetCheck(1); 
        m_button2.EnableWindow(1); 
        g_flag_CheckHigh=1;//son210916
     }
     else{
    //  m_grid2.SetEnabled(0); 
        m_grid2.EnableWindow(0); 
        m_check2.SetCheck(0);  
        m_button2.EnableWindow(0); 
        g_flag_CheckHigh=0;//son210916
        EndEditing2();
     }
     //sprintf(cBuf, "0");
    m_grid2.SetTextMatrix(1, 1, "0"); 
}

 

void CModeSub18::OnCheck03() 
{
     
}


void CModeSub18::OnCheck04() 
{
     
}


BOOL CModeSub18::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class
    
/*
    if ((pMsg->message == WM_KEYDOWN) 
        && (pMsg->wParam == VK_ESCAPE)) {
        return TRUE;
    }
*/
    if ((pMsg->message == WM_VSCROLL) ) {
 //     OnOK();
        return TRUE;
    }
    if ((pMsg->message == WM_KEYDOWN) 
        && (pMsg->wParam == VK_RETURN)) {
 //     OnOK();
        return TRUE;
    }

    if (  pMsg->message == VK_SCROLL){ 
     
 //     OnOK();
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

    //int  i;


BOOL CModeSub18::OnInitDialog() 
{
    CDialog::OnInitDialog();

    InitControl();  
     //onload();
    OnLoadDisplay1();
    OnLoadDisplay2();

    //son220914_2 bebin : GTS�䱸����. 4W LimitSet ȭ�� ������ ��� �߰� 
    //son passwd ��� �������� ��� check �ڽ��� disable �Ѵ�.
    //    Grid�� ��ư�� m_check1�� m_check2�� ���¿� ���ӵǹǷ� 
    //    check �ڽ��鸸 disable�ϸ� �ȴ�.
    if (g_flag_LimitPasswdOk == 0)  
    {
        m_check1.EnableWindow(0);   
        m_check2.EnableWindow(0);   
        m_check4.EnableWindow(0);   
    } //son220914_2 end

    
    return TRUE;   
                  
}

 
//son "4W Low Set" 
//  1) ���� ����ġ Grid�� ������ ����  gd4W1_P1Low[][]�� write �Ѵ�.
//  2) ����� gd4W1_P1Low[][] ���� "4W_Per_Low.ini" �� write�ϰ� 
//     �ٽ� Load�ؼ� �߸� write �Ȱ� ���� Ȯ���Ѵ�.
void CModeSub18::OnButton101()   
{ 
    CString str;

//  m_nCurRow=0;
    m_nCurCol=0;

    EndEditing1();

    m_grid1.SetTextMatrix(1, 1, "0");//testing
 
    ::ZeroMemory(&gd4W1_P1Low, sizeof(gd4W1_P1Low));
            
    if(g_flag_CheckLow<=0){ //son210916
        gd4W1_P1Low[0][SET4W_LIMIT_USE]=0;       //son ����ġ check box off  //son220914
    }
    if(g_flag_CheckLow>=1){ //son210916
        gd4W1_P1Low[0][SET4W_LIMIT_USE]=1;       //son ����ġ check box on   //son220914
    }
   
    for( int j=1; j<=MAX_LIMIT4W_RNG; j++)      //son211012 20 -> MAX_LIMIT4W_RNG
    { 
        for( int k=1; k<=LIMIT4W_SIGMA_COEF; k++)   //son211012  LIMIT4W_OFFSET -> LIMIT4W_SIGMA_COEF . Low SigmaCoef �߰�
        {
            str= m_grid1.GetTextMatrix(j, k); 

            if(k< LIMIT4W_SIGMA_COEF){      //son211012
                gd4W1_P1Low[j][k]=atoi(str);
            }
            //son �ñ׸� ����� ��츸 float������ ó�� 
            else{
                gd4W1_P1Low[j][k]=atof(str);   //son211012
            }

            if(gd4W1_P1Low[j][k]<0){
                gd4W1_P1Low[j][k]=0;
            }
            if(gd4W1_P1Low[j][k] > MAX_4W_R){
                gd4W1_P1Low[j][k]=0;
            }   

            //son �ñ׸� ������ 6���� Ŭ �� ����.
            if(k== LIMIT4W_SIGMA_COEF){//son211012
                if(gd4W1_P1Low[j][k]>6){
                    gd4W1_P1Low[j][k]=6;
                }                 
            }   

            // if(j==1&&k==1){
            //sylee180222-2  gd4W1_P1Low[j][k]=0;
            // }
            if(k== LIMIT4W_R_END)
            {
                if(gd4W1_P1Low[j][LIMIT4W_R_END] <= MAX_4W_R)
                { 
                    //son 20: ������ Range�� ��� gd4W1_P1Low[j][LIMIT4W_R_END]�� 200ohm �̾�� �Ѵ�.
                    if(j== MAX_LIMIT4W_RNG){  
                        gd4W1_P1Low[j][LIMIT4W_R_END]= MAX_4W_R; 
                    }
                    //son gd4W1_P1Low[j][LIMIT4W_R_END]�� ���� Range�� Start Ohm���� �����Ѵ�.
                    else{
                        gd4W1_P1Low[j+1][LIMIT4W_R_START]=gd4W1_P1Low[j][LIMIT4W_R_END];
                    }
                }
            }            
        }
    }

    //---------------------------------------------
    //son ����� gd4W1_P1Low[][] ���� "4W_Per_Low.ini" �� write�Ѵ�.
    FileSysInfo01.LoadSaveSub18(_SAVELOW);//save    //son211012 11 -> _SAVELOW

    //son �ٽ� Load�ؼ� �߸� write �Ȱ� ���� Ȯ���Ѵ�.
    OnLoadDisplay1() ;
    return; 
}


//son "4W High Set"
//  1) ���� ����ġ Grid�� ������ ����  gd4W1_P2High[][]�� write �Ѵ�.
//  2) ����� gd4W1_P2High[][] ���� "4W_Per_High.ini" �� write�ϰ� 
//     �ٽ� Load�ؼ� �߸� write �Ȱ� ���� Ȯ���Ѵ�.
void CModeSub18::OnButton102()  //save high
{ 
     
    CString str;

    m_nCurCol2=0;   

    EndEditing2();

    m_grid2.SetTextMatrix(1, 1, "0");
 
    ::ZeroMemory(&gd4W1_P2High, sizeof(gd4W1_P2High));
            
    if(g_flag_CheckHigh<=0){//son210916
        gd4W1_P2High[0][SET4W_LIMIT_USE]=0;  //son + High Use m_check2  //son220914
    }
    if(g_flag_CheckHigh>=1){//son210916
        gd4W1_P2High[0][SET4W_LIMIT_USE]=1;  //son220914
    }

//----------------------------------------------
/*
    if(m_check3.GetCheck()){
        gd4W1_P2High[0][SET4W_SIGMA_EVG_USE]=1;         //son210916
    }else{
        gd4W1_P2High[0][SET4W_SIGMA_EVG_USE]=0;         //son210916
    }

    m_edit3.GetWindowText( szText,10);  
    k  = atoi( szText );
    if(k<0){
       k=0;
    }
    if(k>6){
       k=6;
    }
    gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=k;    //son210916
*/
    
    if(m_check4.GetCheck()){
        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]=1;       //son210916 
    }else{
        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]=0;         //son210916  
    }


    //son + High: 0 ~ 500% 
    m_edit4.GetWindowText( szText,10);  
    k  = atoi( szText );
    if(k<0){
       k=0;
    }
    if(k>500){
       k=500;
    }
    gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=k;   //son210916


    /*

    if(m_check5.GetCheck()){
        gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=1;        //son 'Average % Use' On   //son210916
    }else{
        gd4W1_P2High[21][SET4W_AVG_PERCNT_USE]=0;           //son210916
    }


    m_edit5.GetWindowText( szText,10);  
    k  = atoi( szText );
    if(k<0){
       k=0;
    }
    if(k>500){
       k=500;
    }
    //son 'Average %' edit value
    gd4W1_P2High[21][SET4W_AVG_PERCNT_VAL]=k;       //son210916
*/



//----------------------------------------------
   
    for( int j=1; j<= MAX_LIMIT4W_RNG; j++)     //son211012 20 -> MAX_LIMIT4W_RNG
    { 
        for( int k=1; k<= LIMIT4W_SIGMA_COEF; k++)
        {   
            str= m_grid2.GetTextMatrix(j, k); 

            if(k< LIMIT4W_SIGMA_COEF){
                gd4W1_P2High[j][k]=atoi(str);//sylee170819-1 
            }

            //son �ñ׸� ����� ��츸 float������ ó�� 
            else{
                gd4W1_P2High[j][k]=atof(str);//sylee170819-1 
            }


            if(gd4W1_P2High[j][k]<0){
                gd4W1_P2High[j][k]=0;
            }

            //son �ñ׸� ������ 6���� Ŭ �� ����.
            if(k== LIMIT4W_SIGMA_COEF){//sylee160604-1
                if(gd4W1_P2High[j][k]>6){
                    gd4W1_P2High[j][k]=6;
                }                 
            }   

            if(gd4W1_P2High[j][k]> MAX_4W_R){  
                gd4W1_P2High[j][k]=0;
            }   
            // if(j==1&&k==1){
            // sylee180222-2     gd4W1_P2High[j][k]=0;
            // }
            if(k== LIMIT4W_R_END)
            {
                if(gd4W1_P2High[j][LIMIT4W_R_END]<= MAX_4W_R)  //sylee180222
                {
                    //son 20: ������ Range�� ��� gd4W1_P2High[j][LIMIT4W_R_END]�� 200ohm �̾�� �Ѵ�.
                    if(j==MAX_LIMIT4W_RNG){ 
                        gd4W1_P2High[j][LIMIT4W_R_END]= MAX_4W_R;  //sylee180222
                    }
                    //son gd4W1_P2High[j][LIMIT4W_R_END]�� ���� Range�� Start Ohm���� �����Ѵ�.
                    else{
                        gd4W1_P2High[j+1][LIMIT4W_R_START]=gd4W1_P2High[j][LIMIT4W_R_END];
                    }
                }
            }                    


        }
    }

//---------------------------------------------
    FileSysInfo01.LoadSaveSub18(_SAVEHIGH);//save   //son211012 21 -> _SAVEHIGH

    OnLoadDisplay2();

    return; 
}   
 




//son ����ġ Low Grid�� Display
void CModeSub18::OnLoadDisplay1()   
{ 


    FileSysInfo01.LoadSaveSub18(_LOADLOW);    //son211012 12 -> _LOADLOW

    if(gd4W1_P1Low[0][SET4W_LIMIT_USE]==1){  //son220914
        g_flag_CheckLow=0;  //son210916 �� 1�� �����Ȱ� 0���� �ٲٴ���??? OnCheck01()�� ����.  
    }else{ 
        g_flag_CheckLow=1;  
    }
    //son211012 g_flag_CheckLow�� ������ ������ �ݴ밪���� �ٲ��. UI�� ���� ������. 
    //   gd4W1_P1Low[0][SET4W_LIMIT_USE]�� 1�̶�� �Ͻ������� g_flag_CheckLow�� 0���� �����ߴٰ�
    //   OnCheck01()�� ��ģ �Ŀ� 1�� �ٲ�.  �����ϱ� ��ư� ���������ϱ� �������� SW ����. 
    //   OnCheck01()�� g_flag_CheckLow�� toggle ��Ű�� �����̹Ƿ� 1�� ����� ���� 
    //   g_flag_CheckLow�� ���� 0���� �����  OnCheck01()�� ȣ���ؼ� Check �ڽ� UI��� �Բ� g_flag_CheckLow�� 1�� ������ ��.
    OnCheck01();  

    for( j=1; j<= MAX_LIMIT4W_RNG; j++)  //son211012  20 -> MAX_LIMIT4W_RNG
    { 
        for( k=1; k<= LIMIT4W_SIGMA_COEF; k++)  //son211012  LIMIT4W_OFFSET -> LIMIT4W_SIGMA_COEF . Low SigmaCoef �߰�
        {  
            //son ù��° �׸�
            if((j==1)&&(gd4W1_P1Low[j][LIMIT4W_R_START]>=0)
                     &&(gd4W1_P1Low[j][LIMIT4W_R_START]<= MAX_4W_R))//(j>1)&&  //sylee180222   //sylee200828  100->200 
            {
                //son 0���� �۰ų� 200000 mohm (200 ohm) ���� ũ�� 0���� ��ȯ
                if((gd4W1_P1Low[j][k]< 0)||(gd4W1_P1Low[j][k]> MAX_4W_R)){ //sylee180222
                    gd4W1_P1Low[j][k]=0; 
                }
                if(k < LIMIT4W_SIGMA_COEF){     //son211012
                    sprintf(cBuf, "%d", (int) gd4W1_P1Low[j][k]);  //son211012
                }
                //son Sigma ����� �Ҽ��� float�� ���
                else{
                    sprintf(cBuf, "%2.1f", gd4W1_P1Low[j][k]);//son211012
                }
            }

            //son ù��° ���� �׸� Start ���� 0 �̻����� ������ �� �ִٸ�
            else if((gd4W1_P1Low[j][LIMIT4W_R_START]>0)&&(gd4W1_P1Low[j][LIMIT4W_R_START]<= MAX_4W_R))// //sylee180222
            {
                //son 0���� �۰ų� 200000 mohm (200 ohm) ���� ũ�� 0���� ��ȯ
                if((gd4W1_P1Low[j][k]< 0)||(gd4W1_P1Low[j][k]> MAX_4W_R)){ //sylee180222
                    gd4W1_P1Low[j][k]=0; 
                }
                if(k < LIMIT4W_SIGMA_COEF){ //son211012
                    sprintf(cBuf, "%d", (int)gd4W1_P1Low[j][k]);   //son211012
                }
                //son Sigma ����� �Ҽ��� float�� ���
                else
                {
                    if (k== LIMIT4W_SIGMA_COEF){//sylee160504-1
                        if( gd4W1_P1Low[j][k]< 0 ){             
                            gd4W1_P1Low[j][k]=0; 
                        }
                        //son �ñ׸� ����� 6���� Ŭ �� ����.
                        else if( gd4W1_P1Low[j][k]>6 ){             
                            gd4W1_P1Low[j][k]=6;
                        }
                    }

                    sprintf(cBuf, "%2.1f", gd4W1_P1Low[j][k]);//son211012
                }

            }
            else
            {
                gd4W1_P1Low[j][k]=0;
                sprintf(cBuf, "     ");
            }




            //son201109 ���� ???  �� gd4W1_P1Low[j][1]�� �ƴ϶�  gd4W1_P2High[j][LIMIT4W_R_START]�� üũ???????????
            //if((j!=1)&&(gd4W1_P2High[j][LIMIT4W_R_START]==0))//sylee160604-1         
            if((j!=1)&&(gd4W1_P1Low[j][LIMIT4W_R_START]==0))//son201109
            {
                //son ù��° �׸��� �ƴѵ� start�� 0�̸� �ƹ��͵� ������� �ʴ´�.
                m_grid1.SetTextMatrix(j, k, "   ");//sylee160604-1
            }
            else{//sylee160604-1
                m_grid1.SetTextMatrix(j, k, cBuf);
            }//sylee160604-1


        }
    }


    DoEvents();

    return; 
}



//son ����ġ High Grid�� Display
void CModeSub18::OnLoadDisplay2()   
{ 
    
    //----------------------------------------------
 /*   if(gd4W1_P2High[0][SET4W_SIGMA_EVG_USE]==1){          //son210916
        m_check3.SetCheck(1); 
    }else{
        m_check3.SetCheck(0);
        gd4W1_P2High[0][SET4W_SIGMA_EVG_USE]=0;         //son210916
    }
 


    k=gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE];    //son210916
    if(k<0){
       k=0;
    }
    if(k>6){
       k=6;
    }
     gd4W1_P2High[0][SET4W_EJECT_LOW_HIGH_USE]=k;   //son210916
    sprintf(szText, "%d",  k);
    m_edit3.SetWindowText(szText); 
    */
    
 
    
    if(gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]==1){      //son 'All % Use'       //son210916
        m_check4.SetCheck(1); 
    }else{
        m_check4.SetCheck(0);
        gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_USE]=0;           //son210916
    }


    k=(int)gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL];  //son210916
    if(k<0){
       k=0;
    }
    if(k>500){
       k=500;
    }
    //son "4W_Per_High.ini"���� �о�� 'All % value'�� edit4�� ���� ( 0 ~  500 %)
    gd4W1_P2High[0][SET4W_HIGH_ALL_PERCNT_VAL]=k;       //son210916
    sprintf(szText, "%d",  k);
    m_edit4.SetWindowText(szText); 


//----------------------------------------------


    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load   //son211012 22 -> _LOADHIGH
 
    

    if(gd4W1_P2High[0][SET4W_LIMIT_USE]==1){   //son '+High Use' //son220914
        g_flag_CheckHigh=0;//son210916  ����???? �� 1�� �����Ȱ� 0���� �ٲٴ���??? 
    }else{ 
        g_flag_CheckHigh=1;//son210916  ����???
    }
    OnCheck02();  //son211012 g_flag_CheckHigh�� ������ ������ �ݴ밪���� �ٲ��. UI�� ���� ������.

    for( j=1; j<= MAX_LIMIT4W_RNG; j++)   //son211012  20 -> MAX_LIMIT4W_RNG
    { 
        for( k=1; k<= LIMIT4W_SIGMA_COEF; k++)   //sylee160604-1  4_.5
        {
            //son ù��° �׸� 
            if ((j==1) && (gd4W1_P2High[j][LIMIT4W_R_START] >= 0)
                       && (gd4W1_P2High[j][LIMIT4W_R_START] <= MAX_4W_R))//(j>1)&&  //sylee180222  //sylee200828  100->200  
            {
                //son 0���� �۰ų� 200000���� ũ�� 0���� ��ȯ 
                if((gd4W1_P2High[j][k]< 0)||(gd4W1_P2High[j][k]> MAX_4W_R)){ //sylee180222
                    gd4W1_P2High[j][k]=0; 
                }

                if(k < LIMIT4W_SIGMA_COEF){
                    sprintf(cBuf, "%d", (int)gd4W1_P2High[j][k]);//sylee170819-1
                }
                //son Sigma ����� �Ҽ��� float�� ���
                else{
                    sprintf(cBuf, "%2.1f", gd4W1_P2High[j][k]);//sylee170819-1
                }

            }

            //son ù��° ���� �׸� Start ���� 0 �̻����� ������ �� �ִٸ�
            else if((gd4W1_P2High[j][LIMIT4W_R_START]>0) && (gd4W1_P2High[j][LIMIT4W_R_START]<= MAX_4W_R))//  //sylee180222
            {
                //son 0���� �۰ų� 200000���� ũ�� 0���� ��ȯ 
                if((gd4W1_P2High[j][k]< 0)||(gd4W1_P2High[j][k]> MAX_4W_R)){  //sylee180222
                    gd4W1_P2High[j][k]=0; 
                }

                if(k < LIMIT4W_SIGMA_COEF){
                    sprintf(cBuf, "%d", (int)gd4W1_P2High[j][k]);//sylee170819-1
                }
                //son Sigma ����� �Ҽ��� float�� ���
                else{
                    sprintf(cBuf, "%2.1f", gd4W1_P2High[j][k]);//sylee170819-1
                }

            }
            else{
                gd4W1_P2High[j][k]=0;
                sprintf(cBuf, "     ");
            }

            if (k== LIMIT4W_SIGMA_COEF){//sylee160504-1
                if( gd4W1_P2High[j][k]< 0 ){             
                    gd4W1_P2High[j][k]=0; 
                }
                //son �ñ׸� ����� 6���� Ŭ �� ����.
                else if( gd4W1_P2High[j][k]>6 ){             
                    gd4W1_P2High[j][k]=6;
                }

                sprintf(cBuf, "%2.1f", gd4W1_P2High[j][k]);//sylee170819-1
            }

            //son ù��° �׸��� �ƴѵ� start�� 0�̸� �ƹ��͵� ������� �ʴ´�.
            if((j!=1)&&(gd4W1_P2High[j][LIMIT4W_R_START]==0)){//sylee160604-1
                m_grid2.SetTextMatrix(j, k, "   ");//sylee160604-1
            }
            else{//sylee160604-1
                m_grid2.SetTextMatrix(j, k, cBuf);
            }//sylee160604-1
        }
    }


    
    DoEvents();
 
    return; 

}
 

void CModeSub18::OnClickOk() 
{ 
  
  
    CDialog::OnOK();
}

void CModeSub18::SetAttr(int iOpa)
{
 //SetLayeredWindowAttributes(RGB(0,0,255),iOpa,LWA_ALPHA | LWA_COLORKEY);
}
 
  
void CModeSub18::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

    SetAttr(pSlider->GetPos());

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


 

void CModeSub18::InitControl()
{

    FileSysInfo01.LoadSaveSub18(_LOADHIGH);//load   //son211012 22 -> _LOADHIGH

    g_flag_CheckLow=1;  //son210916
    OnCheck01();    //son g_flag_CheckLow -> 0���� �ٲ��. UI�� ���� ������. 

    g_flag_CheckHigh=1;//son210916
    OnCheck02();    //son g_flag_CheckHigh -> 0���� �ٲ��. UI�� ���� ������.  

    OnInit_grid1_Low();
    OnInit_grid2_High();


} 
 
 

void CModeSub18::OnDblClickMsflexgrid1() 
{

    BeginEditing1();        
    
}

void CModeSub18::OnDblClickMsflexgrid2() 
{

    BeginEditing2();        
    
}


void CModeSub18::OnEnterCellMsflexgrid1() 
{

    m_grid1.GetCurCell(m_nCurRow, m_nCurCol);
}

void CModeSub18::OnEnterCellMsflexgrid2() 
{

    m_grid2.GetCurCell(m_nCurRow2, m_nCurCol2);
}

void CModeSub18::OnLeaveCellMsflexgrid1() 
{

    if (OnEditing1) {
        EndEditing1();
    }   
}

void CModeSub18::OnLeaveCellMsflexgrid2() 
{

    if (OnEditing2) {
        EndEditing2();
    }   
}

void CModeSub18::OnMouseUpMsflexgrid1(short Button, short Shift, long x, long y) 
{

    m_grid1.SetRowSel(m_nCurRow);
    m_grid1.SetColSel(m_nCurCol);   
}

void CModeSub18::OnMouseUpMsflexgrid2(short Button, short Shift, long x, long y) 
{

    m_grid2.SetRowSel(m_nCurRow2);
    m_grid2.SetColSel(m_nCurCol2);  
}


void CModeSub18::EndEditing1()
{

    CString str;
    CString strText;
    char szText[30];
    int tmp1,tmp2;
    double dtmp3;//son211012

    m_edit1.EnableWindow(FALSE);
    m_edit1.ShowWindow(SW_HIDE);
    m_edit1.GetWindowText(strText);
    //==========================================
    //  m_nCurRow, m_nCurCol
    //sylee160503

    m_edit1.GetWindowText( szText,10);
    if(m_nCurCol!= LIMIT4W_SIGMA_COEF){//son211012 5 -> LIMIT4W_SIGMA_COEF 
        tmp1 = atoi( szText ); 
    }else{
        dtmp3 = atof( szText );//son211012
    }

    if(m_nCurCol==LIMIT4W_R_END)  //~100000
    {
        if(tmp1<0 ){
            tmp1=0;
        }
        if( tmp1 > MAX_4W_R){ //sylee180222
            tmp1 = MAX_4W_R;             
        }

        str= m_grid1.GetTextMatrix(m_nCurRow, LIMIT4W_R_START);  
        sscanf(LPCTSTR(str), "%d", &tmp2);
        if(tmp1<=tmp2){
            tmp1=MAX_4W_R;
        }
        str.Format("%d",tmp1);
        m_grid1.SetText(str);

        //----------------------------------------------------------
        //son ���� Row ������Ȳ�� ���� next Row�� ������ �����Ѵ�. 
        if(m_nCurRow< MAX_LIMIT4W_RNG)   //son211012 20 -> MAX_LIMIT4W_RNG
        {
            if(tmp1<= MAX_4W_R) //sylee200828  100->200               
            {
                //son ���� end ���� ������ row�� start�� �Ǿ�� �Ѵ�.
                m_grid1.SetTextMatrix( m_nCurRow+1, LIMIT4W_R_START, str);                

                //son ������ row�� end���� �̸� �о�ͼ� ���� end������ �۴ٸ� ������ MAX_4W_R������ �����Ѵ�.
                str= m_grid1.GetTextMatrix(m_nCurRow+1, LIMIT4W_R_END);  
                sscanf(LPCTSTR(str), "%d", &tmp2);
                if(tmp2<=tmp1){
                    str.Format("%d", MAX_4W_R);
                    m_grid1.SetTextMatrix( m_nCurRow+1, LIMIT4W_R_END, str);    //son201109 200000 ���
                    m_grid1.SetTextMatrix( m_nCurRow+1, LIMIT4W_PERCENT, "0");
                    m_grid1.SetTextMatrix( m_nCurRow+1, LIMIT4W_OFFSET, "0");
                    m_grid1.SetTextMatrix( m_nCurRow+1, LIMIT4W_SIGMA_COEF, "0");  //son211012 �߰�
                }
            }
        }

    }
    else if(m_nCurCol==LIMIT4W_PERCENT)//%  0 ~ 100%
    {
        if(tmp1<0 ){
            tmp1=0;
        }
        if( tmp1>100){
            tmp1=100;            
        }
        str.Format("%d",tmp1);
        m_grid1.SetText(str);

    }
    else if(m_nCurCol==LIMIT4W_OFFSET){//-R
        if(tmp1<0 ){
            tmp1=0;
        }
        if( tmp1> MAX_4W_R){
            tmp1= MAX_4W_R;          
        }
        str.Format("%d",tmp1);
        m_grid1.SetText(str);
    }
    else if(m_nCurCol==LIMIT4W_SIGMA_COEF)// sigma  //son211012
    {
        if(dtmp3<0 ){    
            dtmp3=0;  
        }
        //son �ñ׸� ����� 6���� Ŭ �� ����.
        if( dtmp3>6){
            dtmp3=6;             
        }
        str.Format("%.1f",dtmp3);
        m_grid1.SetText(str);
    } //son211012




    //--------------------------------------------------------
    //son LIMIT4W_R_END �� 200 ohm (MAX_4W_R)�� �׸� ������ 
    //    Grid ������  ��� ����ó��
    //--------------------------------------------------------
    tmp1=0;
    for(int y1=1;y1<= MAX_LIMIT4W_RNG;y1++){    //son211012 20 =>MAX_LIMIT4W_RNG
        str= m_grid1.GetTextMatrix(y1, LIMIT4W_R_END);  
        sscanf(LPCTSTR(str), "%d", &tmp2);
        if(tmp2== MAX_4W_R){
            tmp1=y1;
            break;
        }
    }

    for(  y1=(tmp1+1); y1<= MAX_LIMIT4W_RNG;y1++){      //son211012 20 -> MAX_LIMIT4W_RNG
        for(int x1=1; x1<= LIMIT4W_SIGMA_COEF;x1++){    //son211012 LIMIT4W_OFFSET -> LIMIT4W_SIGMA_COEF  : Low SigmaCoef�߰�
            m_grid1.SetTextMatrix(y1, x1, "    ");
        }
    }


    //========================================

    //sylee160503   m_grid1.SetText(strText);
    OnEditing1 = FALSE;
    m_grid1.SetFocus();
    //  SetModified(TRUE);  
}




void CModeSub18::EndEditing2()
{ 

    CString str;
    CString strText;
    char szText[30];
    int tmp1,tmp2;
    double dtmp3;//sylee170819

    m_edit2.EnableWindow(FALSE);
    m_edit2.ShowWindow(SW_HIDE);
    m_edit2.GetWindowText(strText);

    //==========================================
    //  m_nCurRow, m_nCurCol        
    //sylee160503

    m_edit2.GetWindowText( szText,10);
    if(m_nCurCol2!= LIMIT4W_SIGMA_COEF){//sylee170819   //son211012 5 -> LIMIT4W_SIGMA_COEF
        tmp1 = atoi( szText );
    }else{//sylee170819
        dtmp3 = atof( szText );//sylee170819
    }

    if(m_nCurCol2== LIMIT4W_R_END)  //~100000
    {
        if(tmp1<0 ){
            tmp1=0;
        }
        if( tmp1> MAX_4W_R){
            tmp1= MAX_4W_R;          
        }

        str= m_grid2.GetTextMatrix(m_nCurRow2, LIMIT4W_R_START);  
        sscanf(LPCTSTR(str), "%d", &tmp2);
        if(tmp1<=tmp2){
            tmp1= MAX_4W_R;
        }
        str.Format("%d",tmp1);
        m_grid2.SetText(str);

        //----------------------------------------------------------
        //son ���� Row ������Ȳ�� ���� next Row�� ������ �����Ѵ�. 
        if (m_nCurRow2 < MAX_LIMIT4W_RNG)   //son211012 20 -> MAX_LIMIT4W_RNG
        {
            if(tmp1<= MAX_4W_R)  //sylee200828  100->200    
            {
                //son ���� end ���� ������ row�� start�� �Ǿ�� �Ѵ�.
                m_grid2.SetTextMatrix(m_nCurRow2+1, LIMIT4W_R_START, str);

                //son ������ row�� end���� �̸� �о�ͼ� ���� end������ �۴ٸ� ������ MAX_4W_R������ �����Ѵ�.
                str= m_grid2.GetTextMatrix(m_nCurRow2+1, LIMIT4W_R_END);  
                sscanf(LPCTSTR(str), "%d", &tmp2);

                if(tmp2 <= tmp1)
                {
                    str.Format("%d", MAX_4W_R);
                    m_grid2.SetTextMatrix(m_nCurRow2+1, LIMIT4W_R_END, str);  //son201109 200000 ���
                    m_grid2.SetTextMatrix(m_nCurRow2+1, LIMIT4W_PERCENT, "0");
                    m_grid2.SetTextMatrix(m_nCurRow2+1, LIMIT4W_OFFSET, "0");
                    m_grid2.SetTextMatrix(m_nCurRow2+1, LIMIT4W_SIGMA_COEF, "0");
                }
            }
        }   

    }

    else if(m_nCurCol2==LIMIT4W_PERCENT)//%  0 ~ 500%
    {
        if(tmp1<0 ){
            tmp1=0;
        }
        if( tmp1>500){
            tmp1=500;            
        }
        str.Format("%d",tmp1);
        m_grid2.SetText(str);

    }
    else if(m_nCurCol2==LIMIT4W_OFFSET)//-R
    {
        if(tmp1<0 ){
            tmp1=0;
        }
        if( tmp1> MAX_4W_R){
            tmp1= MAX_4W_R;          
        }
        str.Format("%d",tmp1);
        m_grid2.SetText(str);
    }
    else if(m_nCurCol2==LIMIT4W_SIGMA_COEF)// sigma  //sylee160604-1
    {
        if(dtmp3<0 ){    //sylee170819  tmp1->dtmp3
            dtmp3=0;  
        }
        //son �ñ׸� ����� 6���� Ŭ �� ����.
        if( dtmp3>6){
            dtmp3=6;             
        }
        str.Format("%.1f",dtmp3);
        m_grid2.SetText(str);

    }


    //--------------------------------------------------------
    //son LIMIT4W_R_END �� 200 ohm (MAX_4W_R)�� �׸� ������ 
    //    Grid ������  ��� ����ó��
    //--------------------------------------------------------

    tmp1=0;
    for(int y1=1;y1<= MAX_LIMIT4W_RNG;y1++){        //son211012 20 -> MAX_LIMIT4W_RNG   
        str= m_grid2.GetTextMatrix(y1, LIMIT4W_R_END);  
        sscanf(LPCTSTR(str), "%d", &tmp2);
        if(tmp2== MAX_4W_R){
            tmp1=y1;
            break;
        }
    }

    for(  y1=(tmp1+1); y1<= MAX_LIMIT4W_RNG;y1++){  //son211012 20 -> MAX_LIMIT4W_RNG
        for(int x1=1; x1<=LIMIT4W_SIGMA_COEF; x1++){//sylee160605-1  4->5
            m_grid2.SetTextMatrix(y1, x1, "    ");
        }
    }

    //========================================

    //sylee160503   m_grid2.SetText(strText);
    OnEditing2 = FALSE;
    m_grid2.SetFocus();
    //  SetModified(TRUE);      
}



void CModeSub18::BeginEditing1()
{
 
    if(m_nCurCol<=1){//sylee160503
        OnEditing1 = FALSE;
        return ;
    }
    
    OnEditing1 = TRUE;
    CRect rectCell;
    rectCell.left = m_rectGrid1.left + long(m_dRatio * double(m_grid1.GetCellLeft()));
    rectCell.top = m_rectGrid1.top + long(m_dRatio * double(m_grid1.GetCellTop()));
    rectCell.right = rectCell.left + long(m_dRatio * double(m_grid1.GetCellWidth()));
    rectCell.bottom = rectCell.top + long(m_dRatio * double(m_grid1.GetCellHeight()));
 
    m_edit1.SetWindowText(m_grid1.GetTextMatrix(m_nCurRow, m_nCurCol));
    m_edit1.MoveWindow(rectCell);
    m_edit1.EnableWindow(TRUE);
    m_edit1.ShowWindow(SW_SHOWNORMAL);
    m_edit1.SetFocus();
    m_edit1.SetSel(0, -1);

}


void CModeSub18::BeginEditing2()
{

    if(m_nCurCol2<=1){//sylee160503
        OnEditing2 = FALSE;
        return ;
    }  

    OnEditing2 = TRUE;
    CRect rectCell;
    rectCell.left = m_rectGrid2.left + long(m_dRatio2 * double(m_grid2.GetCellLeft()));
    rectCell.top = m_rectGrid2.top + long(m_dRatio2 * double(m_grid2.GetCellTop()));
    rectCell.right = rectCell.left + long(m_dRatio2 * double(m_grid2.GetCellWidth()));
    rectCell.bottom = rectCell.top + long(m_dRatio2 * double(m_grid2.GetCellHeight()));

    m_edit2.SetWindowText(m_grid2.GetTextMatrix(m_nCurRow2, m_nCurCol2));
    m_edit2.MoveWindow(rectCell);
    m_edit2.EnableWindow(TRUE);
    m_edit2.ShowWindow(SW_SHOWNORMAL);
    m_edit2.SetFocus();
    m_edit2.SetSel(0, -1);

}




 
 
 
void CModeSub18::OnInit_grid1_Low()   //Initclear
{

    int i;//, nRowCt,nColCt;

    CString str;   
    k=0; 

    // m_grid1.Clear();

    defRow1Temp1=20;    
    m_grid1.SetRows(defRow1Temp1+1);

    m_grid1.SetCol(0); 
    m_grid1.SetRow(0); 

    for ( i=1 ; i<=defRow1Temp1; i++)   { 
        m_grid1.SetCol(0); 
        m_grid1.SetRow(i); 
        str.Format("NO.%d", i); 
        m_grid1.SetText(str);  
    }  

    COLORREF clr1,clr2,clr3,clr4,clr5,clr6,clr7;

    clr1 = RGB(130,0,20);//red
    clr2 = RGB(255,200,228);//white red 
    clr3 = RGB(225,255,164);//green
    clr4 = RGB(255,137,81);// RED ORANGE
    clr5 = RGB(0,10,50);//black
    //clr6 = RGB(253,255,255);//white
    clr6 = RGB(252,254,211);//white  yellow
    clr7 = RGB(175,252,150);//  white green

    m_grid1.SetForeColor(clr7); 
    m_grid1.SetBackColor(clr6);

    m_grid1.SetColColor(LIMIT4W_R_START,    clr5 , clr3);   //son211012 
    m_grid1.SetColColor(LIMIT4W_R_END,      clr5 , clr3);   //son211012 
    m_grid1.SetColColor(LIMIT4W_PERCENT,    clr1 , clr4);   //son211012 
    m_grid1.SetColColor(LIMIT4W_OFFSET,     clr1 , clr2);   //son211012 
    m_grid1.SetColColor(LIMIT4W_SIGMA_COEF, clr1 , clr7);   //son211012

    m_grid1.Refresh();
    m_grid1.SetRedraw(TRUE);

    m_nCurRow = 1;
    m_nCurCol = 1;

    m_grid1.GetWindowRect(m_rectGrid1);
    ScreenToClient(m_rectGrid1);

    //son twip�� pixel�� ��ȯ�ϱ� ���� ����.  ���� 1 pixel = 15 twip������ �ػ󵵿� ���� �޶� �� �� ����.   
    //    m_rectGrid.Width()�� pixel �����̰� MSFlexGrid�� ������ twip �����̴�.
    //    ���� m_rectGrid witdh�� 432 pixel �̹Ƿ� 432: x = 1 : 15,    x = 15*432 => 11940
    //int width = m_rectGrid1.Width(); //son220928
    //m_dRatio  = double (1 / 15.);   //son220928
    m_dRatio = double(m_rectGrid1.Width())/ 6480.;  //son220928 6460 -> 6480

    OnEditing1 = FALSE;


}

void CModeSub18::OnInit_grid2_High()   //Initclear
{

    int i;//, nRowCt,nColCt;

    CString str; 

    k=0; 
    // m_grid2.Clear();
    defRow1Temp2=20;    
    m_grid2.SetRows(defRow1Temp2+1);

    m_grid2.SetCol(0); 
    m_grid2.SetRow(0); 

    for ( i=1 ; i<=defRow1Temp2; i++)   { 
        m_grid2.SetCol(0); 
        m_grid2.SetRow(i); 
        str.Format("NO.%d", i); 
        m_grid2.SetText(str);  
    }  

    COLORREF clr1,clr2,clr3,clr4,clr5,clr6,clr7;

    clr1 = RGB(130,0,20);//red
    clr2 = RGB(255,200,228);//white red 
    clr3 = RGB(225,255,164);//green
    clr4 = RGB(255,137,81);// RED ORANGE
    //  clr4 = RGB(9,73,172);//blue
    clr5 = RGB(0,10,50);//black
    //clr6 = RGB(253,255,255);//white
    clr6 = RGB(252,254,211);//white  yellow
    clr7 = RGB(175,252,150);//  white green

    m_grid2.SetForeColor(clr5); 
    m_grid2.SetBackColor(clr5);

    m_grid2.SetColColor(LIMIT4W_R_START,    clr5 , clr3);   //son211012
    m_grid2.SetColColor(LIMIT4W_R_END,      clr5 , clr3);   //son211012
    m_grid2.SetColColor(LIMIT4W_PERCENT,    clr1 , clr4);   //son211012
    m_grid2.SetColColor(LIMIT4W_OFFSET,     clr1 , clr2);   //son211012
    m_grid2.SetColColor(LIMIT4W_SIGMA_COEF, clr1 , clr7);   //son211012

    m_grid2.Refresh();
    m_grid2.SetRedraw(TRUE);

    m_nCurRow2 = 1;
    m_nCurCol2 = 1;

    m_grid2.GetWindowRect(m_rectGrid2);
    ScreenToClient(m_rectGrid2);

    //son twip�� pixel�� ��ȯ�ϱ� ���� ����.  ���� 1 pixel = 15 twip������ �ػ󵵿� ���� �޶� �� �� ����.   
    //    m_rectGrid.Width()�� pixel �����̰� MSFlexGrid�� ������ twip �����̴�.
    //    ���� m_rectGrid witdh�� 440 pixel �̹Ƿ� 440: x = 1 : 15,    x = 15*440 => 6600
    //int width = m_rectGrid2.Width(); //son220928
    //m_dRatio2  = double (1 / 15.);   //son220928
    m_dRatio2 = double(m_rectGrid2.Width())/6600.;  //son220928 6430 -> 6600

    OnEditing2 = FALSE;

}

 
 

//int i;

BOOL CModeSub18::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
 /*
    if( i<0){
         i=0;
    }else if( i<120){
         i=i+1;
    }else{
        i=120;
    }

    if(zDelta==-120){ //down

        SendMessage(VK_NEXT,0,0);
        m_grid1.SetScrollTrack(i);  
        DoEvents();
        m_grid1.SetScrollTrack(i);

    }else if(zDelta==120){//up
     
 
    }
*/

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}



void CModeSub18::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // TODO: Add your message handler code here and/or call default
    
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}




 
 

 
