#if !defined(__COMI_DIO_IN_OUT_H__)
#define __COMI_DIO_IN_OUT_H__


//------------------------------
// DIN  : ������ <= �������
//------------------------------

//son Bit ��ġ: bit ->Byte Location       //son200827
enum DIO_IN_TYPE {
    DIN_START   = 0,
    DIN_TEST    = 1,
    DIN_ABORT   = 2,
    DIN_SKIP    = 3,
    DIN_REVERSE = 4,
    DIN_DUT1    = 5,        // DUT1 or print
    DIN_DUT2    = 6,        // DUT2 or error
    DIN_RESHEET = 7,

    DIN_4W_REF_INIT = 8,   //son231016 ���� 4W Ref �ʱ�ȭ 
#ifdef __2BYTE_DIO__      //son231018
    DIN_NUM = 16          //son230914
#else
    DIN_NUM = 8           //son231018 __2BYTE_DIO__�� �ƴ� ���� ������� 8 bit�� Read
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

//son201028 ChildView1 BDL �ڵ��ε����
#define	DIN_CMD_BDL_LOAD	    (DIN_CMD_ABORT + DIN_CMD_SKIP + DIN_CMD_DUT1 + DIN_CMD_DUT2)	//0x6C (108)

#define DIN_CMD_4W_MANUAL_ABORT (DIN_CMD_TEST + DIN_CMD_ABORT)          //sylee230426-8 //0x06





//------------------------------
// DOUT  : ������ => �������
//------------------------------

//  int nDioS2;//sylee160520		//son ������ -> Handler : ��� IO�� (DOUT_CMD_TYPE  ���)
//     0x01(1)  PASS	  : PASS 
//     0x02(2)  SHORT	  : Short �ҷ�
//     0x04(4)  OPEN	  : Open �ҷ�
//     0x08(8)  RETEST	  : ��� ��˻� ��ȣ (Block�� ���� �ڸ����� �ٽ� �׽�Ʈ)
//     0x10(16) CONFAIL	  : ������ �ҷ�      (���� Hanlder�� �� ��ȣ�� ������ �����Ѵ�.)
//     0x20(32) END 	  : 4W Auto Sampling Mode Return ��ȣ
//      				    �Ϲ����� ��쿡�� END�� ������ �ʴ´�.  block�� ���� ������� �� �� ���� ����.
//     0x06(6)  SHORT+OPEN: Short & Open �ҷ�
//     0x05(5)  PASS+OPEN : (ACK_SKIP_ABORT) ABORT �Ǵ� SKIP�� ��񿡼� �޾��� �� ������->���� �ִ� Return ��ȣ 
//     ?????              : (ACK_SKIP_ABORT_RESHEET) ABORT/SKIP/RESHEET �� ��񿡼� �޾��� �� ������->���� �ִ� Return ��ȣ              


//son231016 begin
// DIO Out Bit ��ġ (Location)
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
//son Oring �ؼ� ��� ����. value ��        //son200827
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

#define DOUT_CMD_ACK_SKIP_ABORT    (DOUT_CMD_PASS + DOUT_CMD_OPEN )         //son 0x05 : SKIP(or ABORT) �ߴٰ� ��� ����

//TBD CCTC, E&E ������� Manual ȭ�鿡�� ����.  (������������� 4W ���� ��ȣ�� ���? )
//#define DOUT_CMD_ACK_SKIP_ABORT_RESHEET 


//------------------------------
// Command ���� �ó�����
//------------------------------

    //son230914 int nDio[8];    
	//int nDio[DIN_NUM]; 	    //son230914 16: DIN_NUM

	//son Handler(�������)  -> ������ : �Է� IO ��   DIO_IN_TYPE(location),  DIN_BIT_TYPE(bit)
    //
    //      . ��� Handler�� ��û�ϸ� �����Ⱑ ���������� �����ϰ� �����ϴ� ����
    //      
    //      . Block Retest�� �����⿡�� Open, �Ǵ� Open+Short�϶��� ��ü������ �����ؼ�  
    //        nDioS2(0x08: RETEST)�� ��� �����ϰ� (0x04:OPEN���� �������� �ʴ´�) ���� Block�� �ٽ� test�Ѵ�. 
    //        Short �߻����� ���� Block Retest ���� �ʴ´�. (Open�� ���� �־�� Retest�Ѵ�.)
    //        ������ Block Retest�ص�(ī��Ʈ ����) ��� OPEN�̸� �׶����� 0x04:OPEN���� Handler�� �����Ѵ�.
    //      
    //      . RETEST�� Handler���� �޾Ҵٸ� ����� RETEST�� �޴��� Handler�� ���� Block�� �ӹ�����.
    //        Handler�� PASS, OPEN, SHORT�� �޾ƾ߸� ���� Block���� �Ѿ��. 
    //      
    //      . ������ ���忡���� ABORT, SKIP�� �����ϴ�. �̰� ������ ���� Block�� �˻����� �ʰ�
    //        PASS+OPEN(0x05: ACK_SKIP)�� �����Ѵ�.  �׷� ��� Handler�� ���� Block �׽�Ʈ�� ��û�Ѵ�.
    //      
    //      . Resheet(PCB�� ��� ����� �ٽ� test) �� Handler���� �����Ѵ�. 
    //        ��񿡼� Resheet�ϰڴٰ� nDio[7]���� 1�� ������� �����ش�.
    //
    //      . RETEST�� ��ü block �׽�Ʈ ���� ���߿� �����⿡�� Ư�� block�� Retest�Ϸ��� �ϴ� ���̰�
    //        RESHEET�� ��ü block �׽�Ʈ �Ϸ������ ���������  Fail�� �� block�� Test�� ��û�Ѵ�.
    //        �ٸ� block�� ��������� skip���� ��û�Ѵ�. 
    //      
    //      . nDio[] �� ��ȣ�� On�� �Ǹ� (Handler->������) ��� High ���·� �ӹ��� �ִٰ�
    //        �����Ⱑ �˻縦 �Ϸ��ϰ� �� �����  nDioS2����  ������->Handler �� �����ϸ�, 
    //        nDio[]�� ��ȣ�� Low�� ��������.
    //
    //
    //		 RESHEET  DUT2  DUT1  REVERSE  SKIP  ABORT	TEST  START		
    //	nDio   [7]     [6]  [5]	    [4]	   [3]	  [2]	[1]    [0]		
    //
    // 	nDio[]array    byte 	 I/O ��ȣ                   //  ���� 
    //   
    //   	0000 0011 (0x03): START+TEST                    // PCB ù��° Block���� Test ���� ��û 
    //   	0001 0011 (0x13): START+TEST+REVERSE            // PCB ������ Block���� ���� Test ���� ��û 
    //   	1000 0011 (0x83): START+TEST+RESHEET            // PCB ù��° Block���� Test ���� ��û (Resheet Mode)
    //   	1001 0011 (0x93): START+TEST+REVERSE+RESHEET    // PCB ������ Block���� ���� Test ���� ��û (Resheet Mode)
    //   
    //   	0000 0101 (0x05): START+ABORT                   // PCB ù��° Block ABORT �� ����
    //   	0001 0101 (0x15): START+ABORT+REVERSE           // PCB ������ Block ABORT �� ���� 
    //   	1000 0101 (0x85): START+ABORT+RESHEET           // PCB ù��° Block ABORT �� ���� (Resheet Mode)
    //   	1001 0101 (0x95): START+ABORT+REVERSE+RESHEET	// PCB ������ Block ABORT �� ���� (Resheet Mode)
    //   
    //   	0000 1001 (0x09): START+SKIP                    // PCB ù��° Block SKIP����  ����
    //   	0001 1001 (0x19): START+SKIP+REVERSE            // PCB ������ Block SKIP����  ���� 
    //   	1000 1001 (0x89): START+SKIP+RESHEET            // PCB ù��° Block SKIP����  ���� (Resheet Mode)
    //   	1001 1001 (0x99): START+SKIP+REVERSE+RESHEET	// PCB ������ Block SKIP����  ���� (Resheet Mode)
    //   
    //   	0000 0010 (0x02): TEST	                        // 2~n Block Auto Test ��û (ù��° ����)
    //   	0000 0100 (0x04): ABORT	                        // 2~n ABORT ���� (AutoTest) 
    //   	0000 1000 (0x08): SKIP	                        // 2~n SKIP  ���� (AutoTest)
    //	                 
    //
    // < ��Ʈ���հ�   Command >
    //	    0000 1111 (0x0F): START+TEST+ABORT+SKIP 		// Handler Manual â���� �׽�Ʈ��û  (Block Retest �ʿ����, AutoPress)
    //                                                      //    Open �ҷ��̾ Retry�� ���ϰ� ���� Handler ���� ����϶� ����Ѵ�.
    //                                                      //    C-30, 2 Shuttle, index��񿡼� ���. ABORT+SKIP�� ����.
    //                                                      //    ������Ȳ. �Ϲ����� �׳� ABORT+SKIP�� ���� �ִٰ� ��.(�ں��Ժ���)
    //	    0000 1100 (0x0C): ABORT+SKIP                    // Handler Target â���� �׽�Ʈ��û (Block Retest �ʿ����. �׳�Press ) 
    //	                                                    // ù��° Block ����. 
    //	    0001 1100 (0x1C): ABORT+SKIP+REVERSE            // Handler Target â���� �׽�Ʈ��û (Block Retest �ʿ����) 
    //	                                                    // ������ Block ����
    //
    //
    //      0110 1100 (0x6C): ABORT+SKIP+DUT1+DUT2          // BDL �ڵ��ε� ��û (ChildView1 ����)
    //
    //      0000 0110 (0x06): TEST+ABORT                    // Handler Manualâ 4W �����׽�Ʈ ��û (Block Retest �ʿ����)  //sylee230426-8 MANUAL START:  ABORT DIO 
    //      0000 0111 (0x07): START+TEST+ABORT              // TEST+ABORT �� ������ 4W �����׽�Ʈ ��û (Hanlder Manual Start �ÿ� ���۵�)
    //      1000 0100 (0x84): ABORT+RESHEET                 // Handler Targetâ 4W �����׽�Ʈ ��û (Block Retest �ʿ����)  //sylee231019 
    //                                                      // 'ABORT+RESHHET' �� 'ABORT+SKIP'�� ��Ÿ������ �ϳ��� ������. 
    //                                                      // Index���� 'ABORT+RESHHET' �� ����ϰ�, F550�� 'ABORT+SKIP'�� ����ϹǷ� 
    //                                                      // F550������ �ݵ�� '4W Abort ���� �ɼ��� ���� ����ؾ� �Ѵ�.'
    //
    // 0001 0000 0000 (0x100): 4W_REF_INIT                  // ���� 4W Ref �ʱ�ȭ  (2byte for ����ý�)
    //                                                      // ChildView1, ChildView7 �Ѵ� ó��. 2byte Command

    
    // ex1) �ó����� 1 Retest
    //
    //    �������ȣ       Handler��ȣ   Handler����                   ������ ����
    //----------------------------------------------------------------------------------------------------------
    //                   <- START+TEST   1�� Block ��û                PCB ù��° Block �˻����
    //                                                                 PASS ����
    //    PASS(0x01)     ->                                            PASS ������ ����
    //                                                                
    //                   <- TEST	     2�� Block ��û                �ι�° Block �˻� ����.
    //                                                                 OPEN �ҷ� ����
    //    RETEST(0x08)   ->                                            �����Ⱑ Retest�� �����ϰ� RETEST ������ ����. 
    //    ....                		    				               
    //    RETEST(0x08)   ->                                   	       �ι�° Block �˻縦 Block retestī���͸�ŭ ����
    //    PASS(0x01) 	 ->                                            Retest��� PASS. PASS ������ ����
    //                                                                
    //                   <- SKIP         3�� Block Skip ����           3��° block�� Skip 
    //    ACK_SKIP(0x05) ->                                            Skip ������ Handler�� ����.
    //                                                                
    //                   <- ABORT        4�� align����                 ��񿡼� align fail�� �־ ABORT�� ����. 
    //                                                                 ���������忡���� ABORT�� SKIP ��� �׳� Skip�̴�. 
    //    ACK_SKIP(0x05) ->                                            Skip ������ Handler�� ����.
    //                                                                
    //                                4�� Block�� �������̹Ƿ� ����    ������� END�� ���� �ʾƵ� ������ ��������� üũ�ؼ� ��ü������ 
    //                               END�� ���� ������ �ʴ´�.         END ó��. ������ ����̸� 4W CSV �α׸� �����.  
    //                               ��� Pass�̹Ƿ� ����              
    
    
    
    // ex2) �ó����� 2 Resheet
    //
    //    �������ȣ      Handler��ȣ            Handler����              ������ ����
    //------------------------------------------------------------------------------------------------
    //    2-1 �ܰ�-------       
    //                   <- START+TEST           1�� Block ��û         PCB ù��° Block �˻����
    //                                                                  PASS ����
    //    PASS(0x01) 	 ->                                             PASS ������ ����
    //                                                                 
    //                   <- TEST	             2�� Block ��û         �ι�° Block �˻� ����.
    //                                                                  OPEN �ҷ� ����
    //    RETEST(0x08)   ->                                             Retest�� �����ϰ� RETEST ������ ����. 
    //      ....              		               			       
    //    RETEST(0x08)   ->                                             �ι�° Block �˻縦 Block retest ī���͸�ŭ ����
    //                                                                  retest ������� ��� OPEN �ҷ��������� Ȯ��
    //    OPEN(0x04)     ->                                             OPEN ������ ����.
    //                                                                 
    //                   <- TEST                 3�� Block ��û         �ι�° Block �˻� ����..
    //    PASS(0x01) 	 ->                                             PASS ������ ����.
    //                                                                 
    //                   <- TEST                 4�� Block ��û         �ι�° Block �˻� ����.
    //    PASS(0x01) 	 ->                                             PASS ������ ����
    //                                                                  ������ ����̰� ��� Block pass��� Resheet �ȿ��Ƿ� �α��ؾ� �Ѵ�.
    //                                                                  ��� Block pass�� �ƴϸ� Resheet�� �� ���̹Ƿ�  
    //                                                                  ���簡 ������ ����̾ 4W csv �α� ���Ѵ�. 
    //    2-2 �ܰ�-------
    //
    //    2���� Open�ҷ��̹Ƿ� Resheet ����.
    //    1���� Pass �����Ƿ� Skip
    //
    //                    <- START+SKIP+RESHEET  1�� Block Resheet��û  PCB ù��° Block SKIp���� Resheet�˻� ����
    //    ACK_SKIP(0x05)  ->                                            �Ѿ�ٴ� ������ Handler�� ����.
    //                                                                  nReSheetFlag1 �� 1�� �����ȴ�.
    //                                                                  
    //      			  <-  TEST	             2�� Block ��û         �ι�° Block �˻� ����.
    //    PASS(0x01) 	  ->                                            PASS ������ ����  
    //                                                                  
    //                    <-  SKIP               3�� 1������ pass       3��° block Skip 
    //    ACK_SKIP(0x05)  ->                     Block Skip ����        Skip ������ Handler�� ����.
    //                                                                  
    //                    <-  SKIP               4�� 1������ pass       4��° block Skip
    //    ACK_SKIP(0x05)  ->                     Block Skip ����        Skip ������ Handler�� ����.
    //                                                                  Skip������ ������ ���, ��� pass�̹Ƿ� 4W csv �α��Ѵ�.
				

    // ex3) �ó����� 3 Resheet (Block 5��)
    //
    //    �������ȣ         Handler��ȣ         Handler����             ������ ����
    //----------------------------------------------------------------------------------------------
    //    2-1 �ܰ�-------    
    //
    //                    <-  START+TEST         1�� Block ��û        PCB ù��° Block �˻����. PASS ����
    //    PASS(0x01) 	  ->                                           PASS ������ ����
    //                                                              
    //                    <-  TEST	             2�� Block ��û        2��° Block �˻� ����.
    //    PASS(0x01) 	  ->                                           PASS ������ ����  
    //                                                              
    //                                                              
    //                    <-  SKIP               3�� Block Skip ����   3��° block Skip
    //    ACK_SKIP(0x05)  ->                                           Skip ������ Handler�� ����.
    //                                                              
    //                    <-  ABORT              4�� align����         ��񿡼� align fail�� �־ ABORT�� ����. 
    //                                                                 ���������忡���� ABORT�� SKIP ��� �׳� Skip
    //    ACK_SKIP(0x05)  ->                                           Skip ������ Handler�� ����.
    //                                                              
    //                    <-  TEST	             5�� Block ��û        5��° Block �˻� ����. OPEN �ҷ� ����
    //    RETEST(0x08)    ->                                           Retest�� �����ϰ� RETEST ������ ����. 
    //      ....                		         				     
    //    RETEST(0x08)    ->                                           5��° Block �˻縦 Block retest ī���͸�ŭ ����
    //                                                                 retest ������� ��� OPEN �ҷ��������� Ȯ��
    //    OPEN(0x04)      ->                                           OPEN ������ ����.
    //                                                                 OnStart1()���̽� ������ ���. �̽������� ��� ����� pass�� �ƴϹǷ�
    //                                                                 Resheet����ȴ�. 4W CSV �α� ���� �ʴ´�.
    //                                                                 �� ��������  ������ ����̰� ��� pass�� Resheet �ȿ��Ƿ� �α��ؾ� �Ѵ�.
    //    2-2 �ܰ�-------
    //
    //    2���� Open�ҷ��̹Ƿ� Resheet ����.
    //    1���� Pass �����Ƿ� Skip
    //
    //                    <- START+SKIP+RESHEET  1�� Block Resheet��û  PCB ù��° Block SKIp���� Resheet�˻� ����
    //    ACK_SKIP(0x05)  ->                                            �Ѿ�ٴ� ������ Handler�� ����.
    //                                                                  nReSheetFlag1 �� 1�� �����ȴ�.
    //                                                                
    //                    <- SKIP                2�� Pass�����Ƿ� Skip  2��° block Skip 
    //    ACK_SKIP(0x05)  ->                     Block Skip ����        Skip ������ Handler�� ����.
    //                                                                
    //                    <- SKIP                3�� 1������ Skip       3��° block Skip 
    //    ACK_SKIP(0x05)  ->                     Block Skip ����        Skip ������ Handler�� ����.
    //                                                                
    //                    <- SKIP                4�� 1������ Abort      4��° block Skip
    //    ACK_SKIP(0x05)  ->                     Block Skip ����        Skip ������ Handler�� ����.
    //                                                                
    //                    <- TEST                5�� Block ��û         5��° Block �˻� ����.
    //    PASS(0x01) 	  ->                                            PASS ������ ����  
    //                                                                  OnStart1()���̽�.
    //                                                                  ������ ����̰�, ��� Pass�̹Ƿ�  4W csv �α��Ѵ�.



//----------------------------------------------------------
// < BDL �ڵ��ε���� ������ 201018_û�� ������_����ȣ >
//----------------------------------------------------------

// - ���� ����
// 
// 1. Global.ini ���Ͽ��� ���� ����
// [Model]
// AutoRecipeLoadDir=F:\\Auto Recipe Load
// 
// 2. ���׹��ڵ�(ITEM NO)�� ���� �߰�
// 3. ���׹��ڵ�(ITEM NO) ���� �ؿ� Info.ini ���ϻ���
// 4. Info.ini ���� ���뿡 ���׹��ڵ�(ITEM  NO) �ش�Ǵ� �ڵ鷯/�˻�� �𵨸� ���
// [Linked Name]
// Handler Model Name=AAA
// Tester Model Name=BBB
// 
// 
// - �����
// 
// 1. ���� MODEL ���� ȭ���� �����Ѵ�.
//    �˻�� RECIPE ���� ȭ���� �����Ѵ�.
// 
// 2. ���׹��ڵ帮����� ���ڵ带 �д´�.
// 3. ���� �� �˻�� ���ÿ� �ش� ���ڵ忡 �´� ���� �ҷ��鿩����.
// 
// 
// - �˻��� �ڵ鷯 �� ���
// 
// 1. [�ڵ鷯] �𵨼���ȭ��,
//    [�˻��] RECIPE ����ȭ�鿡�� DIO ��ȣ�� ��ٸ���.
// 2. [�ڵ鷯] ���׹��ڵ带 ���� �� Bioptro\AutoLoad.txt ���Ͽ� ����� �˻�� ��(RECIPE)���� ���, �����Ѵ�.
//    - �ش� ���� �����ϴ� ��� ���� �����Ѵ�. ���������ʴ� ��� �����޽��� ó��
// 3. [�ڵ鷯] 4�� ���(ABORT, SKIP, PRINT, ERROR)��ȣ�� ON ��Ų��.
// 4. [�˻��] 4�� �Է½�ȣ�� ������, Bioptro\AutoLoad.txt ������ �о� RECIPE���� ��´�.
// 5. [�˻��] �� �������� �ش�Ǵ� RECIPE ���� �����ϴ��� Ȯ���Ѵ�.
//    - �ش� RECIPE�� �����ϴ� ���, PASS ��ȣ��, ��������������, SHORT ��ȣ�� ������.
//    - �ش� RECIPE�� �����ϴ� ��� �̸� ������ ��ο��� RECIPE�� �ҷ����δ�.
// 6. [�ڵ鷯] PASS �Ǵ� SHORT ��ȣ�� �ԷµǸ�, 4�� ��� ��ȣ�� OFF ��ƾ��.
//    - �����ð����� �Է��� ������, �����޽��� ó��



//--------------------------------------------------------------------------------------------
// < 4W Ref ���� �ʱ�ȭ ���   ��� �ó�����.  ����ý� 231017  ����������, ���ؽ����� ���� >
//--------------------------------------------------------------------------------------------
// 1. [������] 'Auto Test' ȭ�� �Ǵ�  'File' �ε� ȭ�鿡�� DIN_CMD_4W_REF_INIT ��ȣ�� ��ٸ���. 
// 
// 2. [�ڵ鷯] �����ڵ鷯���� ������� ���� DIO ��ȣ DIN_CMD_4W_REF_INIT 
//     (0x100:    0~16�� ��ȣ ��  8��(9��° ��ȣ) ��  ON �Ѵ�. 
// 
// 3. [������] 'Auto Test' ȭ���̳�  'File' �ε� ȭ�鿡�� DIN_CMD_4W_REF_INIT  ��ȣ�� �����ϸ�, 
// ���� BDL ������ ��� DUT  4w.ini# ������ Ref.Low�� Ref.High ���� ��� 0���� �ٲپ  save �Ѵ�. 
// 
// 4. [������] ���� �ڵ鷯�� �ʱ�ȭ�� �Ϸ��ߴٴ� �ǹ̷� DOUT_CMD_ACK_4W_REF_INIT 
//       (0x80, 0~7 8�� �� 7��(8��° ��ȣ)  ��ȣ�� ������. 
// 
// 5. [�ڵ鷯]  ACK ��ȣ�� ������ DIN_CMD_4W_REF_INIT (0x100) ��ȣ�� OFF �Ѵ�. 
//           ���� �ð� ���� ACK�� ���� ���ϸ�  ���� �޽��� ó���Ѵ�. 



#endif  // !defined(__COMI_DIO_IN_OUT_H__)
