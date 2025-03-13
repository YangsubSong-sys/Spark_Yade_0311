
// 하부시작핀값이 Sw dependent해서 DD, SD로 줄인값으로 입력받은 경우라면 QD  기준으로 늘려서 
// 쓰기위해 getQdSwPin()을 사용한다. 내부적으로 getQD_UpperPartSize()를 사용함.
// 20200615: 하부시작핀을 줄이는 케이스를 상부 box 중 일부를 사용 안하는 경우로 한정해서 생각한 코드임.
//           실제로는 box 와 상관 없이 QD 로 만든 BDL을 DD에서 사용한다던지의 경우로 하부시작핀을 이용하게 되므로
//           box range와 상관없이 하부시작핀을 설정할 수 있는게 맞다.  
//           추후 getQdSwPin_Fixed()를 이용하도록 수정이 필요. getQdSwPin_Fixed는 HW 하부시작핀은 16385로 고정한다.
//           실제로 HW 하부시작핀은 16385 고정이 맞다. 4K, 8K, 16K, 32K 상관없이 모두 16384임
int CModeSet12::getQdSwPin(UINT nSwPin)
{
	int nBoardPinCnt, nBoardNo, nPinNo;
	int nQdSwPin = 0;

	// 기본 range check  
	if (nSwPin < 0 || nSwPin >= MAX_PIN) 
	    return -1;

	// 상단 Sw Pin 갯수
	UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1; 	


	//son SD, DD면 줄여서 하부시작핀을 입력받은 케이스이므로  QD 기준으로 다시 늘려줌
	UINT nQD_UpperPartSize = getQD_UpperPartSize(); 


	// Line(Card), Column(CardPin) 모두 0부터 시작.
	
	
	// 상부 
	if(nSwPin < nQD_UpperPartSize)
	{
		if (SysSet12.m_nPinQDUpper == BOX_SD)
		{
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;	// SD BoardPinCnt : 32
			nBoardNo = nSwPin / nBoardPinCnt;	// SD BoardNO
			nPinNo = nSwPin % nBoardPinCnt;		// SD PinNo

			// SD는 홀수 Line(Board or Card)는 건너 뛰고 짝수 라인만(MAX_BOX_LINE *2) 사용하고
			// 짝수 라인안에서도 짝수 칸만 사용된다. (nPinNo * 2) 
			nQdSwPin = nBoardNo * MAX_BOX_LINE * 2 + nPinNo * 2;	// MAX_BOX_LINE: 64

			return nQdSwPin;
		}

		if (SysSet12.m_nPinQDUpper == BOX_DD)	
		{                                    	
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;   // DD BoardPinCnt
			nBoardNo = nSwPin / nBoardPinCnt;    // DD BoardNO
			nPinNo = nSwPin % nBoardPinCnt;      // DD PinNo

			// DD 짝수 라인이면 홀수 칸은 건너뛰고 짝수 칸만 사용된다. (nPinNo * 2) 
			nQdSwPin = nBoardNo * MAX_BOX_LINE + nPinNo * 2; // MAX_BOX_LINE: 64

			// DD는 홀수 라인이면 짝수칸을 건너뛰고 홀수 칸만 사용된다. (nPinNo * 2 +1)
			if(nBoardNo % 2 == 1)
				nQdSwPin++;
			return nQdSwPin;
		}


	}

	// 하부 
	else
	{
		if (SysSet12.m_nPinQDLower == BOX_SD)
		{
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;							// SD BoardPinCnt : 32
			nBoardNo = (nSwPin - nOrg_UpperPartSize) / nBoardPinCnt;    	// SD BoardNO
			nPinNo = (nSwPin - nOrg_UpperPartSize) % nBoardPinCnt;      	// SD PinNo

			// SD는 홀수 Line(Board or Card)는 건너 뛰고 짝수 라인만(MAX_BOX_LINE *2) 사용하고
			// 짝수 라인안에서도 짝수 칸만 사용된다. (nPinNo * 2) 
			nQdSwPin = nQD_UpperPartSize + nBoardNo * MAX_BOX_LINE *2 + nPinNo * 2;
			return nQdSwPin;
		}

		if (SysSet12.m_nPinQDLower == BOX_DD)
		{
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;							// DD BoardPinCnt : 32
			nBoardNo = (nSwPin - nOrg_UpperPartSize) / nBoardPinCnt;		// DD BoardNO    			
			nPinNo   = (nSwPin - nOrg_UpperPartSize) % nBoardPinCnt;     // DD PinNo        			
			nQdSwPin = nQD_UpperPartSize + nBoardNo * MAX_BOX_LINE + nPinNo * 2;  	

			// DD는 홀수 라인이면 짝수칸을 건너뛰고 홀수 칸만 사용된다. (nPinNo * 2 +1)
			if(nBoardNo % 2 == 1)
				nQdSwPin++;
			return nQdSwPin;
		}

	}

	//  QD라면 계산없이 원래 nSwPin을 리턴한다 .
	return nSwPin;


}
