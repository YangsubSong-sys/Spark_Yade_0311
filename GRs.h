#ifndef __GRS_H__ //son231205 header guard Ãß°¡
#define __GRS_H__ 

//#include "stdafx.h"

//#include "FileSysInfo.h"
//#include "Contec.h"
//#include "Gnet1.h"

//extern void DoEvents();

HANDLE  hCom [10];
int	 fOpened[10];
int  nPortDef1;

int nMode; 

extern double nCalC[30][10][100];

char cDataRS2[MAX_CH][MAX_DATA] = {{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
															
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0},
											 
															{0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}};


UINT   BytesRead[MAX_CH];
UINT   BytesWritten[MAX_CH];


 

void OnRsModeAuto(int a);  
void OnRsModeRange(int a, int b);  //MODE Range(0~10)
double OnRsMeas1(int a, int nMode2);  //measure?  //nMode2=1 R,    2= C
void OnRsModeFreq(int a);  //MODE C  // 1= 1000 , 2= 120 ,  ?
void OnRsModeSpeed(int a);  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?

int ReadFile1() ; 


__int64 GetMicroSecond2();
 __int64 GetuSecond();
 
BOOL OpenRS( int nInt); 
BOOL CloseRS( int nInt); 
BOOL ReadRS(int nPort);
BOOL WriteRS(int nPort, char* cStr);

 

#endif 
