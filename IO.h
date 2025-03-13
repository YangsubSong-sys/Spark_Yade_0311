#ifndef __IO_H_SYLEE__      //son231205 header guard 추가
#define __IO_H_SYLEE__

 //SYLEE20120429  3차 이후 양산 용  VER3.1

//------------------------------------------------------------							
#define	DO0	    0	//	DA0	"VB, I , SPARK DA 0"							
#define	DO1	    1	//	DA1	"VB, I , SPARK DA 1"							
#define	DO2 	2	//	DA2	"VB, I , SPARK DA 2"							
#define	DO3	    3	//	DA3	"VB, I , SPARK DA 3"							
#define	DO4	    4	//	DA4	"VB, I , SPARK DA 4"							
#define	DO5 	5	//	DA5	"VB, I , SPARK DA 5"							
#define	DO6	    6	//	DA6	"VB, I , SPARK DA 6"							
#define	DO7 	7	//	DA7	"VB, I , SPARK DA 7"
//===========================================================						
#define	DO8 	8	//	DA8	"VB, I , SPARK DA 8"							
#define	DO9  	9	//	DA9	"VB, I , SPARK DA 9"							
#define	DO10	10	//	DA10	"VB, I , SPARK DA 10"							
#define	DO11	11	//	DA11	"VB, I , SPARK DA 11"							
#define	DO12	12	//	DA12	"VB, I , SPARK DA 12"
#define	DO13	13	//	DA13	"VB, I , SPARK DA 13"
#define	DO14	14	//	DA14	"VB, I , SPARK DA 14"
#define	DO15	15	//	DA15	"VB, I , SPARK DA 15"
//===========================================================

#define	DO16	16	//	I_SET_CLK	I Setting Positive Trigger
#define	DO17	17	//	I_SET_RST	I Setting Reset
#define	DO18	18	//	LV_SET_CLK_0	LV0 Setting Positive Trigger
#define	DO19	19	//	LV_SET_RST_0	LV0 Setting Reset
#define	DO20	20	//	HV_SET_CLK_0	HV0 Setting Positive Trigger
#define	DO21	21	//	HV_SET_RST_0	HV0 Setting Reset
#define	DO22	22	//	LV_SET_CLK_1	LV1 Setting Positive Trigger
#define	DO23	23	//	LV_SET_RST_1	LV1 Setting Reset
//===========================================================

#define	DO24	24	//	HV_SET_CLK_1	HV0 Setting Positive Trigger
#define	DO25	25	//	HV_SET_RST_1	HV0 Setting Reset
#define	DO26	26	//	SPARK_DA_CLK	SPARK DA Setting Positive Trigger
#define	DO27	27	//	SPARK_DA_RST	SPARK DA Setting Reset
#define	DO28	28  //SPARE	
#define	DO29	29  //SPARE	
#define	DO30	30  //SPARE	
#define	DO31    31  //SPARE	
//===========================================================
//28-31  SPARE

//------------------------------------------------------------
#define	DO32	32	//	RNG_SET0	RANGE SET 0
#define	DO33	33	//	RNG_SET1	RANGE SET 1
#define	DO34	34	//	RNG_SET2	RANGE SET 2
#define	DO35	35	//	RNG_SET3	RANGE SET 3
#define	DO36	36	//	RNG_SET4	RANGE SET 4
#define	DO37	37	//	FILTER SELECT  0= LOW R, FAST    1=HIGH R, SLOW   //SYLEE20120430신규추가
#define	DO38	38	// 4WIRE S1 SELF   //SYLEE20120430신규추가
#define	DO39	39	// 4WIRE S2 SELF   //SYLEE20120430신규추가  
//===========================================================
//37-39  SPARE



//------------------------------------------------------------
#define	DO40	40	//	LV_SEN_SEL	"LV VB Sensing(0 : LV0, 1 : LV1)"
#define	DO41	41	//	HV_SEN_SEL	"HV VB Sensing(0 : LV0, 1 : LV1)"
#define	DO42	42	//	HV_SEN_CMD	"HV Sensing(VB,GNDC)"
#define	DO43	43	//3차  SYLEE20120215   VC_LV_IO   ( 0=HV, 1=LV)     //2차에선 define 안씀   SYLEE20120215 	
#define	DO44	44	//3차  SYLEE20120215   VC_LV_SEL  ( 0=HV, 1=LV)     //2차에선 define 안씀   SYLEE20120215
#define	DO45	45	//3차  SYLEE20120215   VB-SS_CMD  (0=I ADC SENSING,  1=VB ADC SENSING)
#define	DO46	46	//3차  SYLEE20120215   4WIRE_CMD  (0=VC SENSING,      1=4W SENSING)      //2차에선 define 안씀   SYLEE20120215
#define	DO47	47	//4-WIRE_CMD->  3차    4WIRE IO=4W RELAY ON로  바뀜.       0=VC SENSEING   1= 4W SENSE

//===========================================================

#define	DO48	48	//4W GAIN  X10 
#define	DO49	49	//SELF  LOW R  100
#define	DO50	50	//SELF  HIGH R 10M
//------------------------------------------------------------
//48-51  SPARE

#define	DO51	51  // SPARK ON    0  RESET     1   RUN
#define	DO52	52	//	HV_RELAY_ON_0	HV0과 VB 연결
#define	DO53	53	//	HV_RELAY_ON_1	HV1과 VB 연결
#define	DO54	54	//	LV_RELAY_ON_0	LV0과 VB 연결
#define	DO55	55	//	LV_RELAY_ON_1	LV1과 VB 연결
//===========================================================
#define	DO56	56	//  HV OPEN   //SYLEE20120430신규추가 
#define	DO57	57  //SPARE	
//48-51  SPARE
#define	DO58	58	// PIN Search 
#define	DO59	59	//	HVGB_DISCHARGE	VB 방전       73
#define	DO60	60	//	GNDC_DISCHARGE	GNDC 방전   
#define	DO61	61	//	CV_ON	CV MODE  //sylee20110921
#define	DO62	62	//	/AD_CONVST_A
#define	DO63	63	 //SPARE	
//===========================================================
#define	DO64	64	//	BDD0	VB PIN SELECT A0
#define	DO65	65	//	BDD1	VB PIN SELECT A1
#define	DO66	66	//	BDD2	VB PIN SELECT A2
#define	DO67	67	//	BDD3	VB PIN SELECT A3
#define	DO68	68	//	BDD4	VB PIN SELECT A4
#define	DO69	69	//	BDD5	VB PIN SELECT A5
#define	DO70	70	//	BDD7    VB CPLD 0
#define	DO71	71	//	BDD8	VB CPLD 1
//===========================================================

#define	DO72	72	//	BDD7    VB CPLD 2  RELAY(0~63)   //SYLEE20120430신규추가
#define	DO73	73	//	BDD8	VB CPLD 3  RELAY(64~127) //SYLEE20120430신규추가
#define	DO74	74	//	BDD9	VB PIN SELECT / DESELCT    //SYLEE20120430 HW삭제됨
#define	DO75	75	//	BSEL0	VB CARD SELECT A0
#define	DO76	76	//	BSEL1	VB CARD SELECT A1
#define	DO77	77	//	BSEL2	VB CARD SELECT A2
#define	DO78	78	//	BSEL3	VB CARD SELECT A3
#define	DO79	79	//	BSENA_0	VB CARD SELECT ENABLE 0   (0~15)

//===========================================================

#define	DO80	80  //	BSENA_1	VB CARD SELECT ENABLE 1    (16~31)
#define	DO81	81  //	CDD0	VC PIN SELECT A0
#define	DO82	82  //	CDD1	VC PIN SELECT A1
#define	DO83	83  //	CDD2	VC PIN SELECT A2
#define	DO84	84  //	CDD3	VC PIN SELECT A3
#define	DO85	85  //	CDD4	VC PIN SELECT A4
#define	DO86	86  //	CDD5	VC PIN SELECT A5
#define DO87	87	//	CDD7    VC CPLD 0  TR(0~63)   
//===========================================================


#define DO88	88	//	CDD8	VC CPLD 1  TR(64~127)
#define DO89	89	//	CDD7    VC CPLD 2  RELAY(0~63)    //SYLEE20120430신규추가
#define DO90	90	//	CDD8	VC CPLD 3  RELAY(64~127)  //SYLEE20120430신규추가
#define	DO91	91	//	CDD9    VC PIN SELECT / DESELCT     //SYLEE20120430 HW삭제됨
#define	DO92	92	//	CSEL0	VC CARD SELECT A0 //sylee20111001  
#define	DO93	93	//	CSEL1	VC CARD SELECT A1  //sylee20111001
#define	DO94	94	//	CSEL2	VC CARD SELECT A2  2011.10.1 기준 알려주지 않았다. ????  93 //sylee20111001
#define	DO95	95	//	CSEL3	VC CARD SELECT A3   //sylee20111001

//===========================================================


//CSEL3	VC CARD SELECT A3  
#define	DO96	96  //  CSENA_0	VC CARD SELECT ENABLE 0  (0~15)   //sylee20111001
#define	DO97	97	//	CSENA_1	VC CARD SELECT ENABLE 1  (16~31)
#define	DO98	98	//	BOX_ENABLE_A        //SYLEE20120430 번지이동 100->98
#define	DO99	99	//	BOX_ENABLE_B        //SYLEE20120430 번지이동 101->99		
#define	DO100	100	//	BOX_ENABLE_C  	    //SYLEE20120430 번지이동 102->100
#define	DO101	101	//	BOX_ENABLE_D  	    //SYLEE20120430 번지이동 103->101
#define	DO102	102	//	BOX_ENABLE_E   	    //SYLEE20120430신규추가	
#define	DO103	103	//	BOX_ENABLE_F	    //SYLEE20120430신규추가

//===========================================================
	
#define	DO104	104	//	BOX_ENABLE_G	    //SYLEE20120430신규추가	
#define	DO105	105	//	BOX_ENABLE_H	    //SYLEE20120430신규추가	
#define	DO106	106	//SPARE		
#define	DO107	107 //SPARE			
#define	DO108	108	//SPARE	
#define	DO109	109	//SPARE			
#define	DO110	110	//SPARE	
#define	DO111	111	//SPARE	
//===========================================================
 
#define	DO112	112	// BOX A VB RESET  //SYLEE20120430 번지이동	104->112	
#define	DO113	113	// BOX A VC RESET  //SYLEE20120430 번지이동	105->113 	
#define	DO114	114	// BOX B VB RESET  //SYLEE20120430 번지이동	106->114  		
#define	DO115	115	// BOX B VC RESET  		
#define	DO116	116	// BOX C VB RESET 		
#define	DO117	117	// BOX C VC RESET 		
#define	DO118	118	// BOX D VB RESET  //SYLEE20120430신규추가			
#define	DO119	119	// BOX D VC RESET  //SYLEE20120430신규추가
//===========================================================			
#define	DO120	120	// BOX E VB RESET  //SYLEE20120430신규추가	
#define	DO121	121	// BOX E VC RESET  //SYLEE20120430신규추가			
#define	DO122	122	// BOX F VB RESET  //SYLEE20120430신규추가			
#define	DO123	123	// BOX F VC RESET  //SYLEE20120430신규추가			
#define	DO124	124	// BOX G VB RESET  //SYLEE20120430신규추가			
#define	DO125	125	// BOX G VC RESET  //SYLEE20120430신규추가			
#define	DO126	126	// BOX H VB RESET  //SYLEE20120430신규추가	
#define	DO127	127	// BOX H VC RESET  //SYLEE20120430신규추가	
//===========================================================

/*
//SYLEE20120430 HW삭제됨.
#define	DO112	112	// CARD_CHK_ENA		
#define	DO113	113	// CARD_CHK_0	
#define	DO114	114	// CARD_CHK_1 
#define	DO115	115	// CARD_CHK_MAIN_0		 
#define	DO116	116	// CARD_CHK_MAIN_1
*/

	

 

#define	DO3328	127 // DO29반대 	//	LV_SEN_CMD	"LV Sensing(VB,GNDC)"
#define	DO3332	127	 
#define	DO3335	127	 
#define	DO3336	127	  
#define	DO3338	127
#define	DO3339	127	 
#define	DO3344	127	
#define	DO3345	127	 
#define	DO3346	127	 
#define	DO3347	127

	 
#define	DO3374	127  				
#define	DO3391	127	//	CDD10	VC PIN RESET  //sylee20111001
 	
		
		

 




	 								
//	NET NAME	비고				
#define	DI0 	0	//	ADC_A_D0	ADC A DATA D0				
#define	DI1	    1	//	ADC_A_D1	ADC A DATA D1				
#define	DI2 	2	//	ADC_A_D2	ADC A DATA D2				
#define	DI3	    3	//	ADC_A_D3	ADC A DATA D3				
#define	DI4	    4	//	ADC_A_D4	ADC A DATA D4				
#define	DI5	    5	//	ADC_A_D5	ADC A DATA D5				
#define	DI6	    6	//	ADC_A_D6	ADC A DATA D6				
#define	DI7   	7	//	ADC_A_D7	ADC A DATA D7				
#define	DI8  	8	//	ADC_A_D8	ADC A DATA D8				
#define	DI9	    9	//	ADC_A_D9	ADC A DATA D9				
#define	DI10	10	//	ADC_A_D10	ADC A DATA D10				
#define	DI11	11	//	ADC_A_D11	ADC A DATA D11				
#define	DI12	12	//	ADC_A_D12	ADC A DATA D12				
#define	DI13	13	//	ADC_A_D13	ADC A DATA D13				
#define	DI14	14	//	ADC_A_D14	ADC A DATA D14
#define	DI15	15	//	ADC_A_D15	ADC A DATA D15
#define	DI16	16	//	ADC_B_D0	ADC B DATA D0
#define	DI17	17	//	ADC_B_D1	ADC B DATA D1
#define	DI18	18	//	ADC_B_D2	ADC B DATA D2
#define	DI19	19	//	ADC_B_D3	ADC B DATA D3
#define	DI20	20	//	ADC_B_D4	ADC B DATA D4
#define	DI21	21	//	ADC_B_D5	ADC B DATA D5
#define	DI22	22	//	ADC_B_D6	ADC B DATA D6
#define	DI23	23	//	ADC_B_D7	ADC B DATA D7
#define	DI24	24	//	ADC_B_D8	ADC B DATA D8
#define	DI25	25	//	ADC_B_D9	ADC B DATA D9
#define	DI26	26	//	ADC_B_D10	ADC B DATA D10
#define	DI27	27	//	ADC_B_D11	ADC B DATA D11
#define	DI28	28	//	ADC_B_D12	ADC B DATA D12
#define	DI29	29	//	ADC_B_D13	ADC B DATA D13
#define	DI30	30	//	ADC_B_D14	ADC B DATA D14
#define	DI31	31	//	ADC_B_D15	ADC B DATA D15
#define	DI32	32	//	DETECT MAIN POWER 12V	0=정상  1=ERROR  //SYLEE20120430 번지이동 52->32
#define	DI33	33	// 	DETECT MAIN POWER 5V	0=정상  1=ERROR	 //SYLEE20120430 번지이동 53->33
#define	DI34	34	//	DETECT MAIN POWER 24V	0=정상  1=ERROR	 //SYLEE20120430 번지이동 54->34
#define	DI35	35	//	DETECT MAIN POWER HV 	0=정상  1=ERROR	 //SYLEE20120430 번지이동 55->35
#define	DI36	36	//		
#define	DI37	37	//	AD_BUSY_A	ADC A BUSY
#define	DI38	38	//	AD_BUSY_B	ADC B BUSY
#define	DI39	39	//	SPARK_ON	SPARK SIGNAL
#define	DI40	40	//	SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y0	BOXA  SWITCH CARD CHECK D0
#define	DI41	41	//	SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y1	BOXA  SWITCH CARD CHECK D1
#define	DI42	42	//	SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y2   BOXB
#define	DI43	43	//  SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y3   BOXB
#define	DI44	44	//  SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y4   BOXC
#define	DI45	45	//  SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y5   BOXC
#define	DI46	46	//	SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y6   BOXD
#define	DI47	47	//  SPARE //SYLEE20120430 HW삭제됨 SW_CHK_Y7   BOXD
#define	DI48	48	//  DETECT BOX D  VC 5V	//SYLEE20120430신규 	
#define	DI49	49	//	DETECT BOX D  VC 5V	//SYLEE20120430신규	
#define	DI50	50	//	DETECT BOX C  VC 5V	//SYLEE20120430신규
#define	DI51	51	//	DETECT BOX C  VC 5V	//SYLEE20120430신규
#define	DI52	52	//	DETECT BOX B  VC 5V	//SYLEE20120430신규
#define	DI53	53	//	DETECT BOX B  VC 5V	//SYLEE20120430신규
#define	DI54	54	//	DETECT BOX A  VC 5V	//SYLEE20120430신규
#define	DI55	55	//	DETECT BOX A  VC 5V	//SYLEE20120430신규
#define	DI56	56	//	DETECT BOX H  VC 5V	//SYLEE20120430신규	
#define	DI57	57	//	DETECT BOX H  VC 5V	//SYLEE20120430신규	
#define	DI58	58	//	DETECT BOX G  VC 5V	//SYLEE20120430신규	
#define	DI59	59	//	DETECT BOX G  VC 5V	//SYLEE20120430신규	
#define	DI60	60	//	DETECT BOX F  VC 5V	//SYLEE20120430신규	
#define	DI61	61	//	DETECT BOX F  VC 5V	//SYLEE20120430신규	
#define	DI62	62	//	DETECT BOX E  VC 5V	//SYLEE20120430신규	
#define	DI63	63	//	DETECT BOX E  VC 5V	//SYLEE20120430신규	
					
#endif
