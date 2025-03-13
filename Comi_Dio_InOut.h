#if !defined(__COMI_DIO_IN_OUT_H__)
#define __COMI_DIO_IN_OUT_H__


//------------------------------
// DIN  : 계측기 <= 메인장비
//------------------------------

//son Bit 위치: bit ->Byte Location       //son200827
enum DIO_IN_TYPE {
    DIN_START   = 0,
    DIN_TEST    = 1,
    DIN_ABORT   = 2,
    DIN_SKIP    = 3,
    DIN_REVERSE = 4,
    DIN_DUT1    = 5,        // DUT1 or print
    DIN_DUT2    = 6,        // DUT2 or error
    DIN_RESHEET = 7,

    DIN_4W_REF_INIT = 8,   //son231016 원격 4W Ref 초기화 
#ifdef __2BYTE_DIO__      //son231018
    DIN_NUM = 16          //son230914
#else
    DIN_NUM = 8           //son231018 __2BYTE_DIO__가 아닌 경우는 기존대로 8 bit만 Read
#endif
};

enum DIN_CMD_TYPE {         
	DIN_CMD_START	= 0x01,     // 1
	DIN_CMD_TEST	= 0x02,     // 2
	DIN_CMD_ABORT	= 0x04,     // 4
	DIN_CMD_SKIP	= 0x08,     // 8
	DIN_CMD_REVERSE	= 0x10,     // 16
	DIN_CMD_DUT1	= 0x20,     // 32       // DUT1 or print
	DIN_CMD_DUT2	= 0x40,     // 64       // DUT2 or error
	DIN_CMD_RESHEET	= 0x80,     // 128

	DIN_CMD_4W_REF_INIT = 0x100  // 256     //son231016
};

//son201028 ChildView1 BDL 자동로딩기능
#define	DIN_CMD_BDL_LOAD	    (DIN_CMD_ABORT + DIN_CMD_SKIP + DIN_CMD_DUT1 + DIN_CMD_DUT2)	//0x6C (108)

#define DIN_CMD_4W_MANUAL_ABORT (DIN_CMD_TEST + DIN_CMD_ABORT)          //sylee230426-8 //0x06





//------------------------------
// DOUT  : 계측기 => 메인장비
//------------------------------

//  int nDioS2;//sylee160520		//son 계측기 -> Handler : 출력 IO값 (DOUT_CMD_TYPE  사용)
//     0x01(1)  PASS	  : PASS 
//     0x02(2)  SHORT	  : Short 불량
//     0x04(4)  OPEN	  : Open 불량
//     0x08(8)  RETEST	  : 블록 재검사 신호 (Block을 같은 자리에서 다시 테스트)
//     0x10(16) CONFAIL	  : 연속핀 불량      (메인 Hanlder는 이 신호를 받으면 정지한다.)
//     0x20(32) END 	  : 4W Auto Sampling Mode Return 신호
//      				    일반적인 경우에는 END를 보내지 않는다.  block의 끝을 계측기는 알 수 없기 때문.
//     0x06(6)  SHORT+OPEN: Short & Open 불량
//     0x05(5)  PASS+OPEN : (ACK_SKIP_ABORT) ABORT 또는 SKIP을 장비에서 받았을 때 계측기->장비로 주는 Return 신호 
//     ?????              : (ACK_SKIP_ABORT_RESHEET) ABORT/SKIP/RESHEET 를 장비에서 받았을 때 계측기->장비로 주는 Return 신호              


//son231016 begin
// DIO Out Bit 위치 (Location)
enum DIO_OUT_TYPE {
    DOUT_LOC_PASS         = 0,
    DOUT_LOC_SHORT        = 1,
    DOUT_LOC_OPEN         = 2,
    DOUT_LOC_RETEST       = 3,
    DOUT_LOC_CONFAIL      = 4,
    DOUT_LOC_SAMPLING_END = 5,
    DOUT_LOC_RESERVED1    = 6,
    DOUT_LOC_ACK_4W_REF_INIT    = 7,

    DOUT_NUM = 8
};

//son231016 end
//son Oring 해서 사용 가능. value 값        //son200827
enum DOUT_CMD_TYPE {
    DOUT_CMD_PASS            = 0x01,        // 1
    DOUT_CMD_SHORT           = 0x02,        // 2
    DOUT_CMD_OPEN            = 0x04,        // 4
    DOUT_CMD_RETEST          = 0x08,        // 8
    DOUT_CMD_CONFAIL         = 0x10,        // 16
    DOUT_CMD_SAMPLING_END    = 0x20,        // 32       //son 4W Sampling End Signal
    DOUT_CMD_RESERVED1       = 0x40,        // 64        //son231016
    DOUT_CMD_ACK_4W_REF_INIT = 0x80         // 128       //son231016
};

#define DOUT_CMD_ALL_OFF            0

#define DOUT_CMD_ACK_SKIP_ABORT    (DOUT_CMD_PASS + DOUT_CMD_OPEN )         //son 0x05 : SKIP(or ABORT) 했다고 장비에 응답

//TBD CCTC, E&E 메인장비 Manual 화면에서 동작.  (메인장비측에서 4W 측정 신호로 사용? )
//#define DOUT_CMD_ACK_SKIP_ABORT_RESHEET 


//------------------------------
// Command 진행 시나리오
//------------------------------

    //son230914 int nDio[8];    
	//int nDio[DIN_NUM]; 	    //son230914 16: DIN_NUM

	//son Handler(메인장비)  -> 계측기 : 입력 IO 값   DIO_IN_TYPE(location),  DIN_BIT_TYPE(bit)
    //
    //      . 장비 Handler가 요청하면 계측기가 수동적으로 동작하고 응답하는 구조
    //      
    //      . Block Retest는 계측기에서 Open, 또는 Open+Short일때에 자체적으로 결정해서  
    //        nDioS2(0x08: RETEST)로 장비에 응답하고 (0x04:OPEN으로 응답하지 않는다) 같은 Block을 다시 test한다. 
    //        Short 발생했을 때는 Block Retest 하지 않는다. (Open이 섞여 있어야 Retest한다.)
    //        여러번 Block Retest해도(카운트 있음) 계속 OPEN이면 그때에는 0x04:OPEN으로 Handler에 응답한다.
    //      
    //      . RETEST를 Handler에서 받았다면 몇번을 RETEST를 받더라도 Handler는 같은 Block에 머무른다.
    //        Handler는 PASS, OPEN, SHORT를 받아야만 다음 Block으로 넘어간다. 
    //      
    //      . 계측기 입장에서는 ABORT, SKIP은 동일하다. 이걸 받으면 현재 Block은 검사하지 않고
    //        PASS+OPEN(0x05: ACK_SKIP)로 응답한다.  그럼 장비 Handler가 다음 Block 테스트를 요청한다.
    //      
    //      . Resheet(PCB의 모든 블록을 다시 test) 는 Handler에서 결정한다. 
    //        장비에서 Resheet하겠다고 nDio[7]값을 1로 계측기로 보내준다.
    //
    //      . RETEST는 전체 block 테스트 진행 도중에 계측기에서 특정 block만 Retest하려고 하는 것이고
    //        RESHEET는 전체 block 테스트 완료시점에 장비측에서  Fail이 난 block만 Test를 요청한다.
    //        다른 block은 장비측에서 skip으로 요청한다. 
    //      
    //      . nDio[] 의 신호가 On이 되면 (Handler->계측기) 계속 High 상태로 머물고 있다가
    //        계측기가 검사를 완료하고 그 결과인  nDioS2값을  계측기->Handler 로 응답하면, 
    //        nDio[]의 신호가 Low로 떨어진다.
    //
    //
    //		 RESHEET  DUT2  DUT1  REVERSE  SKIP  ABORT	TEST  START		
    //	nDio   [7]     [6]  [5]	    [4]	   [3]	  [2]	[1]    [0]		
    //
    // 	nDio[]array    byte 	 I/O 신호                   //  설명 
    //   
    //   	0000 0011 (0x03): START+TEST                    // PCB 첫번째 Block부터 Test 시작 요청 
    //   	0001 0011 (0x13): START+TEST+REVERSE            // PCB 마지막 Block부터 역순 Test 시작 요청 
    //   	1000 0011 (0x83): START+TEST+RESHEET            // PCB 첫번째 Block부터 Test 시작 요청 (Resheet Mode)
    //   	1001 0011 (0x93): START+TEST+REVERSE+RESHEET    // PCB 마지막 Block부터 역순 Test 시작 요청 (Resheet Mode)
    //   
    //   	0000 0101 (0x05): START+ABORT                   // PCB 첫번째 Block ABORT 로 시작
    //   	0001 0101 (0x15): START+ABORT+REVERSE           // PCB 마지막 Block ABORT 로 시작 
    //   	1000 0101 (0x85): START+ABORT+RESHEET           // PCB 첫번째 Block ABORT 로 시작 (Resheet Mode)
    //   	1001 0101 (0x95): START+ABORT+REVERSE+RESHEET	// PCB 마지막 Block ABORT 로 시작 (Resheet Mode)
    //   
    //   	0000 1001 (0x09): START+SKIP                    // PCB 첫번째 Block SKIP으로  시작
    //   	0001 1001 (0x19): START+SKIP+REVERSE            // PCB 마지막 Block SKIP으로  시작 
    //   	1000 1001 (0x89): START+SKIP+RESHEET            // PCB 첫번째 Block SKIP으로  시작 (Resheet Mode)
    //   	1001 1001 (0x99): START+SKIP+REVERSE+RESHEET	// PCB 마지막 Block SKIP으로  시작 (Resheet Mode)
    //   
    //   	0000 0010 (0x02): TEST	                        // 2~n Block Auto Test 요청 (첫번째 제외)
    //   	0000 0100 (0x04): ABORT	                        // 2~n ABORT 시작 (AutoTest) 
    //   	0000 1000 (0x08): SKIP	                        // 2~n SKIP  시작 (AutoTest)
    //	                 
    //
    // < 비트조합값   Command >
    //	    0000 1111 (0x0F): START+TEST+ABORT+SKIP 		// Handler Manual 창에서 테스트요청  (Block Retest 필요없음, AutoPress)
    //                                                      //    Open 불량이어도 Retry를 안하고 싶은 Handler 수동 모드일때 사용한다.
    //                                                      //    C-30, 2 Shuttle, index장비에서 사용. ABORT+SKIP과 같다.
    //                                                      //    최종상황. 일반장비는 그냥 ABORT+SKIP을 쓰고 있다고 함.(박봉규부장)
    //	    0000 1100 (0x0C): ABORT+SKIP                    // Handler Target 창에서 테스트요청 (Block Retest 필요없음. 그냥Press ) 
    //	                                                    // 첫번째 Block 수행. 
    //	    0001 1100 (0x1C): ABORT+SKIP+REVERSE            // Handler Target 창에서 테스트요청 (Block Retest 필요없음) 
    //	                                                    // 마지막 Block 수행
    //
    //
    //      0110 1100 (0x6C): ABORT+SKIP+DUT1+DUT2          // BDL 자동로딩 요청 (ChildView1 전용)
    //
    //      0000 0110 (0x06): TEST+ABORT                    // Handler Manual창 4W 정밀테스트 요청 (Block Retest 필요없음)  //sylee230426-8 MANUAL START:  ABORT DIO 
    //      0000 0111 (0x07): START+TEST+ABORT              // TEST+ABORT 와 동일한 4W 정밀테스트 요청 (Hanlder Manual Start 시에 전송됨)
    //      1000 0100 (0x84): ABORT+RESHEET                 // Handler Target창 4W 정밀테스트 요청 (Block Retest 필요없음)  //sylee231019 
    //                                                      // 'ABORT+RESHHET' 와 'ABORT+SKIP'은 배타적으로 하나만 동작함. 
    //                                                      // Index장비는 'ABORT+RESHHET' 만 사용하고, F550은 'ABORT+SKIP'만 사용하므로 
    //                                                      // F550에서는 반드시 '4W Abort 관련 옵션을 끄고 사용해야 한다.'
    //
    // 0001 0000 0000 (0x100): 4W_REF_INIT                  // 원격 4W Ref 초기화  (2byte for 나노시스)
    //                                                      // ChildView1, ChildView7 둘다 처리. 2byte Command

    
    // ex1) 시나리오 1 Retest
    //
    //    계측기신호       Handler신호   Handler동작                   계측기 동작
    //----------------------------------------------------------------------------------------------------------
    //                   <- START+TEST   1번 Block 요청                PCB 첫번째 Block 검사시작
    //                                                                 PASS 감지
    //    PASS(0x01)     ->                                            PASS 응답을 전송
    //                                                                
    //                   <- TEST	     2번 Block 요청                두번째 Block 검사 시작.
    //                                                                 OPEN 불량 감지
    //    RETEST(0x08)   ->                                            계측기가 Retest를 결정하고 RETEST 응답을 전송. 
    //    ....                		    				               
    //    RETEST(0x08)   ->                                   	       두번째 Block 검사를 Block retest카운터만큼 수행
    //    PASS(0x01) 	 ->                                            Retest결과 PASS. PASS 응답을 전송
    //                                                                
    //                   <- SKIP         3번 Block Skip 결정           3번째 block은 Skip 
    //    ACK_SKIP(0x05) ->                                            Skip 응답을 Handler로 전송.
    //                                                                
    //                   <- ABORT        4번 align실패                 장비에서 align fail이 있어서 ABORT를 보냄. 
    //                                                                 계측기입장에서는 ABORT과 SKIP 모두 그냥 Skip이다. 
    //    ACK_SKIP(0x05) ->                                            Skip 응답을 Handler로 전송.
    //                                                                
    //                                4번 Block이 마지막이므로 종료    계측기는 END를 받지 않아도 마지막 블록인지를 체크해서 자체적으로 
    //                               END를 따로 주지는 않는다.         END 처리. 마지막 블록이면 4W CSV 로그를 남긴다.  
    //                               모두 Pass이므로 종료              
    
    
    
    // ex2) 시나리오 2 Resheet
    //
    //    계측기신호      Handler신호            Handler동작              계측기 동작
    //------------------------------------------------------------------------------------------------
    //    2-1 단계-------       
    //                   <- START+TEST           1번 Block 요청         PCB 첫번째 Block 검사시작
    //                                                                  PASS 감지
    //    PASS(0x01) 	 ->                                             PASS 응답을 전송
    //                                                                 
    //                   <- TEST	             2번 Block 요청         두번째 Block 검사 시작.
    //                                                                  OPEN 불량 감지
    //    RETEST(0x08)   ->                                             Retest를 결정하고 RETEST 응답을 전송. 
    //      ....              		               			       
    //    RETEST(0x08)   ->                                             두번째 Block 검사를 Block retest 카운터만큼 수행
    //                                                                  retest 결과에도 계속 OPEN 불량상태임을 확인
    //    OPEN(0x04)     ->                                             OPEN 응답을 전송.
    //                                                                 
    //                   <- TEST                 3번 Block 요청         두번째 Block 검사 시작..
    //    PASS(0x01) 	 ->                                             PASS 응답을 전송.
    //                                                                 
    //                   <- TEST                 4번 Block 요청         두번째 Block 검사 시작.
    //    PASS(0x01) 	 ->                                             PASS 응답을 전송
    //                                                                  마지막 블록이고 모든 Block pass라면 Resheet 안오므로 로그해야 한다.
    //                                                                  모든 Block pass가 아니면 Resheet가 올 것이므로  
    //                                                                  현재가 마지막 블록이어도 4W csv 로그 안한다. 
    //    2-2 단계-------
    //
    //    2번이 Open불량이므로 Resheet 결정.
    //    1번은 Pass 였으므로 Skip
    //
    //                    <- START+SKIP+RESHEET  1번 Block Resheet요청  PCB 첫번째 Block SKIp으로 Resheet검사 시작
    //    ACK_SKIP(0x05)  ->                                            넘어간다는 응답을 Handler로 전송.
    //                                                                  nReSheetFlag1 가 1로 설정된다.
    //                                                                  
    //      			  <-  TEST	             2번 Block 요청         두번째 Block 검사 시작.
    //    PASS(0x01) 	  ->                                            PASS 응답을 전송  
    //                                                                  
    //                    <-  SKIP               3번 1차에서 pass       3번째 block Skip 
    //    ACK_SKIP(0x05)  ->                     Block Skip 결정        Skip 응답을 Handler로 전송.
    //                                                                  
    //                    <-  SKIP               4번 1차에서 pass       4번째 block Skip
    //    ACK_SKIP(0x05)  ->                     Block Skip 결정        Skip 응답을 Handler로 전송.
    //                                                                  Skip이지만 마지막 블록, 모두 pass이므로 4W csv 로그한다.
				

    // ex3) 시나리오 3 Resheet (Block 5개)
    //
    //    계측기신호         Handler신호         Handler동작             계측기 동작
    //----------------------------------------------------------------------------------------------
    //    2-1 단계-------    
    //
    //                    <-  START+TEST         1번 Block 요청        PCB 첫번째 Block 검사시작. PASS 감지
    //    PASS(0x01) 	  ->                                           PASS 응답을 전송
    //                                                              
    //                    <-  TEST	             2번 Block 요청        2번째 Block 검사 시작.
    //    PASS(0x01) 	  ->                                           PASS 응답을 전송  
    //                                                              
    //                                                              
    //                    <-  SKIP               3번 Block Skip 결정   3번째 block Skip
    //    ACK_SKIP(0x05)  ->                                           Skip 응답을 Handler로 전송.
    //                                                              
    //                    <-  ABORT              4번 align실패         장비에서 align fail이 있어서 ABORT를 보냄. 
    //                                                                 계측기입장에서는 ABORT과 SKIP 모두 그냥 Skip
    //    ACK_SKIP(0x05)  ->                                           Skip 응답을 Handler로 전송.
    //                                                              
    //                    <-  TEST	             5번 Block 요청        5번째 Block 검사 시작. OPEN 불량 감지
    //    RETEST(0x08)    ->                                           Retest를 결정하고 RETEST 응답을 전송. 
    //      ....                		         				     
    //    RETEST(0x08)    ->                                           5번째 Block 검사를 Block retest 카운터만큼 수행
    //                                                                 retest 결과에도 계속 OPEN 불량상태임을 확인
    //    OPEN(0x04)      ->                                           OPEN 응답을 전송.
    //                                                                 OnStart1()케이스 마지막 블록. 이시점에서 모든 블록이 pass가 아니므로
    //                                                                 Resheet예상된다. 4W CSV 로그 하지 않는다.
    //                                                                 이 시점에서  마지막 블록이고 모두 pass면 Resheet 안오므로 로그해야 한다.
    //    2-2 단계-------
    //
    //    2번이 Open불량이므로 Resheet 결정.
    //    1번은 Pass 였으므로 Skip
    //
    //                    <- START+SKIP+RESHEET  1번 Block Resheet요청  PCB 첫번째 Block SKIp으로 Resheet검사 시작
    //    ACK_SKIP(0x05)  ->                                            넘어간다는 응답을 Handler로 전송.
    //                                                                  nReSheetFlag1 가 1로 설정된다.
    //                                                                
    //                    <- SKIP                2번 Pass였으므로 Skip  2번째 block Skip 
    //    ACK_SKIP(0x05)  ->                     Block Skip 결정        Skip 응답을 Handler로 전송.
    //                                                                
    //                    <- SKIP                3번 1차에서 Skip       3번째 block Skip 
    //    ACK_SKIP(0x05)  ->                     Block Skip 결정        Skip 응답을 Handler로 전송.
    //                                                                
    //                    <- SKIP                4번 1차에서 Abort      4번째 block Skip
    //    ACK_SKIP(0x05)  ->                     Block Skip 결정        Skip 응답을 Handler로 전송.
    //                                                                
    //                    <- TEST                5번 Block 요청         5번째 Block 검사 시작.
    //    PASS(0x01) 	  ->                                            PASS 응답을 전송  
    //                                                                  OnStart1()케이스.
    //                                                                  마지막 블록이고, 모두 Pass이므로  4W csv 로그한다.



//----------------------------------------------------------
// < BDL 자동로딩기능 구현안 201018_청주 조이텍_이태호 >
//----------------------------------------------------------

// - 사전 설정
// 
// 1. Global.ini 파일에서 폴더 설정
// [Model]
// AutoRecipeLoadDir=F:\\Auto Recipe Load
// 
// 2. 지그바코드(ITEM NO)명 폴더 추가
// 3. 지그바코드(ITEM NO) 폴더 밑에 Info.ini 파일생성
// 4. Info.ini 파일 내용에 지그바코드(ITEM  NO) 해당되는 핸들러/검사기 모델명 기록
// [Linked Name]
// Handler Model Name=AAA
// Tester Model Name=BBB
// 
// 
// - 사용방법
// 
// 1. 설비 MODEL 선택 화면을 선택한다.
//    검사기 RECIPE 선택 화면을 선택한다.
// 
// 2. 지그바코드리더기로 바코드를 읽는다.
// 3. 설비 및 검사기 동시에 해당 바코드에 맞는 모델이 불러들여진다.
// 
// 
// - 검사기와 핸들러 간 통신
// 
// 1. [핸들러] 모델선택화면,
//    [검사기] RECIPE 선택화면에서 DIO 신호를 기다린다.
// 2. [핸들러] 지그바코드를 읽은 후 Bioptro\AutoLoad.txt 파일에 변경될 검사기 모델(RECIPE)명을 기록, 저장한다.
//    - 해당 모델이 존재하는 경우 모델을 변경한다. 존재하지않는 경우 에러메시지 처리
// 3. [핸들러] 4개 출력(ABORT, SKIP, PRINT, ERROR)신호를 ON 시킨다.
// 4. [검사기] 4개 입력신호가 들어오면, Bioptro\AutoLoad.txt 파일을 읽어 RECIPE명을 얻는다.
// 5. [검사기] 모델 폴더에서 해당되는 RECIPE 명이 존재하는지 확인한다.
//    - 해당 RECIPE이 존재하는 경우, PASS 신호를, 존재하지않으면, SHORT 신호를 보낸다.
//    - 해당 RECIPE이 존재하는 경우 미리 설정된 경로에서 RECIPE를 불러들인다.
// 6. [핸들러] PASS 또는 SHORT 신호가 입력되면, 4개 출력 신호를 OFF 시틴다.
//    - 일정시간동안 입력이 없으면, 에러메시지 처리



//--------------------------------------------------------------------------------------------
// < 4W Ref 원격 초기화 기능   통신 시나리오.  나노시스 231017  손지원부장, 배준식차장 협의 >
//--------------------------------------------------------------------------------------------
// 1. [계측기] 'Auto Test' 화면 또는  'File' 로딩 화면에서 DIN_CMD_4W_REF_INIT 신호를 기다린다. 
// 
// 2. [핸들러] 메인핸들러에서 계측기로 가는 DIO 신호 DIN_CMD_4W_REF_INIT 
//     (0x100:    0~16개 신호 중  8번(9번째 신호) 를  ON 한다. 
// 
// 3. [계측기] 'Auto Test' 화면이나  'File' 로딩 화면에서 DIN_CMD_4W_REF_INIT  신호를 수신하면, 
// 현재 BDL 폴더의 모든 DUT  4w.ini# 파일의 Ref.Low와 Ref.High 값을 모두 0으로 바꾸어서  save 한다. 
// 
// 4. [계측기] 메인 핸들러로 초기화를 완료했다는 의미로 DOUT_CMD_ACK_4W_REF_INIT 
//       (0x80, 0~7 8개 중 7번(8번째 신호)  신호를 보낸다. 
// 
// 5. [핸들러]  ACK 신호를 받으면 DIN_CMD_4W_REF_INIT (0x100) 신호를 OFF 한다. 
//           일정 시간 동안 ACK를 받지 못하면  에러 메시지 처리한다. 



#endif  // !defined(__COMI_DIO_IN_OUT_H__)
