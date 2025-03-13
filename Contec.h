#ifndef __CONTEC_H__        //son231205 header guard Ãß°¡
#define __CONTEC_H__

//contec DIO 

//sylee140218 #include "apidio.h"											// Header file for API-DI

#include "CDio.h" //sylee140218

#define RST					16
#define ENRC				24
#define ENWC				25
#define ENWCC				26

//#define SYSTEM_SLEEP		300


class DIO_Info : public CWnd
{

public:
//sylee140218	HANDLE	hDrv;

	short   hDrv;  //sylee140218
	short   Id;  //sylee140218
	short   Id2;  //sylee140218

	int 	Ret;
	WORD	i;
	char	szText[100];
	WORD	DrvNo, GrpNo;
	WORD	PortNo, BitNo;
	BYTE	Buf;
	DWORD	Data;
	DWORD	DataCMD;	

//sylee140218	HANDLE	hDrv2;

	short	hDrv2; //sylee140218
	int 	Ret2;
	WORD	i2;
	char	szText2[100];
	WORD	DrvNo2, GrpNo2;
	WORD	PortNo2, BitNo2;
	BYTE	Buf2;
	DWORD	Data2;
	DWORD	DataCMD2; 

public:

	DIO_Info();
	virtual ~DIO_Info();
	void	DIO_Initial();
	void	DIO_Destroy();

public:  // muti command

    int PinS1(int a, int b, int c, int i, int j);//a=pin no(0~63) :   b= pin sel=0,disel=1 :    c= sou=1/sink=0    : i=pcbno  ; j=1 R, 2= C

public:  //single commane

	int Pin(int a, int b, int c);//a=pin no(0~63) : b= pin sel=0,disel=1 : c= sou=1/sink=0 
	int PinEna(); 
	int ResetS1();  //source
	int ResetS2(); //sink
	int ResetALL();  
	int ResetALL1(int i);//i=1 r, 2=c
	int ModeR();
 	int ResetInit();

public:   //time delay command

	void	Delay_ms(int dt);
	void	Delay_10us(int dt);
	void	Delay_us(int dt);
	void    Delay_100ns(int dt);
	double	SetDelay_1ms(int t);
	int		CheckDelay_1ms(double time1);
	double	SetDelay_10us(int t);
	int		CheckDelay_10us(double time1);public:

	clock_t	 mi_clock_t_start, mi_clock_t_finish[10];
	double   md_clock_t_Time;
 
	void	RESET(); 

};


extern	DIO_Info  pDIO; 

#endif
