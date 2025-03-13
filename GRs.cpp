 
#include "stdafx.h" 

#include "FileSysInfo.h"
#include "Contec.h"
#include "GRs.h"

extern CSysInfoView01  SysInfoView01;
extern void DoEvents();
 
 

__int64 GetMicroSecond2()   
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if ( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if ( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();

    //SYLEE100115 return((((__int64)now.QuadPart) * ((__int64)10000000)) / ((__int64)frequency.QuadPart));
	return((((__int64)now.QuadPart) * ((__int64)1000000)) / ((__int64)frequency.QuadPart));
}


 
 
 
BOOL OpenRS( int nInt)
{

 
	return TRUE;
}

BOOL CloseRS( int nInt ) 
{
 
	return TRUE;
}


BOOL ReadRS(int nPort)
{

 
	return 1;
}

BOOL WriteRS(int nPort, char* cStr) 
{

 
	return TRUE;
}


 
 
__int64 GetuSecond()  //sylee
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if ( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if ( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();
    //SYLEE100115 return((((__int64)now.QuadPart) * ((__int64)10000000)) / ((__int64)frequency.QuadPart));

	return((((__int64)now.QuadPart) * ((__int64)1000000)) / ((__int64)frequency.QuadPart));
}

 

 
 

void OnRsModeAuto(int a)  //MODE C  // 1= AUTO ON , 2= AUTO OFF , 3 =AUTO?
{
 
}

void OnRsModeSpeed(int a)  //MODE C  // 1= FAST , 2= NORMAL , 3 =SLOW,    ?
{
 

} 

void OnRsModeFreq(int a)  //MODE C  // 1= 1000 , 2= 120 ,  ?
{
 
} 


//int a ==> =1=100m,2=1,3=10,4=100,5=1k,6=10k,7=100k,8=1M,9=10M,10=200M
void OnRsModeRange(int a, int b)  //MODE Range(0~10)
{
 
}




double OnRsMeas1(int a, int nMode2)  //measure?  //nMode2=1 R,    2= C
{ 
 
return 1;
}




 

int ReadFile1() //ok
 {

 
	
  return 1;
 }


 


 
 
