
#include <mmsystem.h>
#include "stdafx.h"
#include "Contec.h"
#include "A_Define1.h"
 

DIO_Info  pDIO;

// extern __int64 Get_uSecond1();




#ifdef DEF_CARD_A1  //sylee170328

#else  //#ifdef DEF_CARD_A1  
    #include "DioD96.h" 
// 	extern	CDIO_D96  pDIO_D96;
#endif  //#ifdef DEF_CARD_A1 


 

__int64  Get_uSecond1()  //sylee130107  //estimation
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;
	double temp1;


    if ( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if ( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();

    //SYLEE100115 return((((__int64)now.QuadPart) * ((__int64)10000000)) / ((__int64)frequency.QuadPart));
	temp1=(((double)now.QuadPart) * (1000000.)) / ((double)frequency.QuadPart);

	if(temp1<0){
			AfxMessageBox(" Error No 1004. Get_uSecond1 <0 ?.", MB_OK);
	}

	return((__int64)temp1);
}


void DIO_Info::Delay_ms(int dt)
{
  static LARGE_INTEGER Start, L;
  double Freq;

  if(!QueryPerformanceFrequency(&L)) 
	  return;// H/W 지원하지 않음
  Freq = (double)(L.QuadPart / 1000.0);

  QueryPerformanceCounter(&Start);
  do
  {
    QueryPerformanceCounter(&L);
  }while((L.QuadPart - Start.QuadPart) / Freq < dt);
}
//---------------------------------------------------------------------------
void DIO_Info::Delay_10us(int dt)
{
  static LARGE_INTEGER Start, L;
  double Freq;

  if(!QueryPerformanceFrequency(&L)) 
	  return;// H/W 지원하지 않음
  Freq = (double)(L.QuadPart / 100000.0);

  QueryPerformanceCounter(&Start);
  do
  {
    QueryPerformanceCounter(&L);
  }while((L.QuadPart - Start.QuadPart) / Freq < dt);
}

//---------------------------------------------------------------------------
void DIO_Info::Delay_us(int dt)
{
  static LARGE_INTEGER Start, L;
  double Freq;

  if(!QueryPerformanceFrequency(&L)) 
	  return;// H/W 지원하지 않음
  Freq = (double)(L.QuadPart / 1000000.0);

  QueryPerformanceCounter(&Start);
  do
  {
    QueryPerformanceCounter(&L);
  }while((L.QuadPart - Start.QuadPart) / Freq < dt);
}


void DIO_Info::Delay_100ns(int dt)
{
  static LARGE_INTEGER Start, L;
  double Freq;

  if(!QueryPerformanceFrequency(&L)) 
	  return;// H/W 지원하지 않음
  Freq = (double)(L.QuadPart / 10000000.0);

  QueryPerformanceCounter(&Start);
  do
  {
    QueryPerformanceCounter(&L);
  }while((L.QuadPart - Start.QuadPart) / Freq < dt);
}

//---------------------------------------------------------------------------
double DIO_Info::SetDelay_1ms(int t)
{
  static LARGE_INTEGER Start, L;
  double t1;

  if(!QueryPerformanceFrequency(&L)) 
	  return -1;// H/W 지원하지 않음
  QueryPerformanceCounter(&Start);
  t1 = (double)(Start.QuadPart + L.QuadPart/1000 * t);
  return t1;
}
//---------------------------------------------------------------------------
int DIO_Info::CheckDelay_1ms(double time1)
{
  static LARGE_INTEGER L;
  if(!QueryPerformanceFrequency(&L)) 
	  return -1;// H/W 지원하지 않음
  if ( L.QuadPart > time1)
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------
double DIO_Info::SetDelay_10us(int t)
{
  static LARGE_INTEGER Start, L;
  double t1;

  if(!QueryPerformanceFrequency(&L)) 
	  return -1.0;// H/W 지원하지 않음
  QueryPerformanceCounter(&Start);
  t1 = (double)(Start.QuadPart + L.QuadPart/100000 * t);
  return t1;
}
//---------------------------------------------------------------------------
int DIO_Info::CheckDelay_10us(double time1)
{
  static LARGE_INTEGER L;
  if(!QueryPerformanceFrequency(&L)) 
	  return -1;// H/W 지원하지 않음
  if ( L.QuadPart > time1)
    return 1;
  else
    return 0;
}
//---------------------------------------------------------------------------


DIO_Info::DIO_Info()
{

	
#ifdef DEF_CARD_A1  //sylee170328
	DIO_Initial();
#else  //#ifdef DEF_CARD_A1   

 
#endif  //#ifdef DEF_CARD_A1 



}

DIO_Info::~DIO_Info()
{


#ifdef DEF_CARD_A1  //sylee170328
	DIO_Destroy();
#else  //#ifdef DEF_CARD_A1   

#endif  //#ifdef DEF_CARD_A1 


}


void DIO_Info::DIO_Initial()
{





#ifdef DEF_CARD_A1  //sylee170328
	char szDeviceName[100]="DIO000";
	char szDeviceName2[100]="DIO001";
 
	Ret = DioInit(szDeviceName, &Id);
	hDrv= Id;

  	if( Ret!=0 )
	{
	 	AfxMessageBox("DioOpen Error!!!");
	}
	else{}	

 

	Ret2 = DioInit(szDeviceName2, &Id2);
	hDrv2= Id2;

	if ( Ret2!=0 )
	{
		AfxMessageBox("DioOpen2 Error!!!");
		return;
	}
	else{}	

#else  //#ifdef DEF_CARD_A1  

#endif  //#ifdef DEF_CARD_A1 

}


void DIO_Info::DIO_Destroy(){

#ifdef DEF_CARD_A1  //sylee170328
	Ret = DioExit(hDrv);
	Ret = DioExit(hDrv2);
#else  //#ifdef DEF_CARD_A1  

#endif  //#ifdef DEF_CARD_A1 



}

 

//===============================================================================
 

int DIO_Info::Pin(int a, int b, int c)//a=pin no(0~63) : b= pin sel=0,disel=1 : c= sou=1/sink=0
{
 
	return 1;
}




//===============================================================================
int DIO_Info::PinEna()
{
 	
	return 1;
}

int DIO_Info::ResetS1()  //source
{ 
	return 1;
}

int DIO_Info::ResetS2() //sink
{
 	return 1;
}


int DIO_Info::ResetALL()
{
	return 1;
}
   
int DIO_Info::PinS1(int a, int b, int c, int i, int j) 
{
	return 1;
}

int DIO_Info::ResetALL1(int i)
{
	return 1;
}

int DIO_Info::ResetInit()
{
 
 	return 1;
}


int DIO_Info::ModeR()
{
 

	return 1;
}
 
//===============================================================================

 
 

//===============================================================================
