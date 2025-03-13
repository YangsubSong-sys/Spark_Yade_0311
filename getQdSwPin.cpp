
// �Ϻν����ɰ��� Sw dependent�ؼ� DD, SD�� ���ΰ����� �Է¹��� ����� QD  �������� �÷��� 
// �������� getQdSwPin()�� ����Ѵ�. ���������� getQD_UpperPartSize()�� �����.
// 20200615: �Ϻν������� ���̴� ���̽��� ��� box �� �Ϻθ� ��� ���ϴ� ���� �����ؼ� ������ �ڵ���.
//           �����δ� box �� ��� ���� QD �� ���� BDL�� DD���� ����Ѵٴ����� ���� �Ϻν������� �̿��ϰ� �ǹǷ�
//           box range�� ������� �Ϻν������� ������ �� �ִ°� �´�.  
//           ���� getQdSwPin_Fixed()�� �̿��ϵ��� ������ �ʿ�. getQdSwPin_Fixed�� HW �Ϻν������� 16385�� �����Ѵ�.
//           ������ HW �Ϻν������� 16385 ������ �´�. 4K, 8K, 16K, 32K ������� ��� 16384��
int CModeSet12::getQdSwPin(UINT nSwPin)
{
	int nBoardPinCnt, nBoardNo, nPinNo;
	int nQdSwPin = 0;

	// �⺻ range check  
	if (nSwPin < 0 || nSwPin >= MAX_PIN) 
	    return -1;

	// ��� Sw Pin ����
	UINT nOrg_UpperPartSize = SysSet12.m_nLowerJigStartPin-1; 	


	//son SD, DD�� �ٿ��� �Ϻν������� �Է¹��� ���̽��̹Ƿ�  QD �������� �ٽ� �÷���
	UINT nQD_UpperPartSize = getQD_UpperPartSize(); 


	// Line(Card), Column(CardPin) ��� 0���� ����.
	
	
	// ��� 
	if(nSwPin < nQD_UpperPartSize)
	{
		if (SysSet12.m_nPinQDUpper == BOX_SD)
		{
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;	// SD BoardPinCnt : 32
			nBoardNo = nSwPin / nBoardPinCnt;	// SD BoardNO
			nPinNo = nSwPin % nBoardPinCnt;		// SD PinNo

			// SD�� Ȧ�� Line(Board or Card)�� �ǳ� �ٰ� ¦�� ���θ�(MAX_BOX_LINE *2) ����ϰ�
			// ¦�� ���ξȿ����� ¦�� ĭ�� ���ȴ�. (nPinNo * 2) 
			nQdSwPin = nBoardNo * MAX_BOX_LINE * 2 + nPinNo * 2;	// MAX_BOX_LINE: 64

			return nQdSwPin;
		}

		if (SysSet12.m_nPinQDUpper == BOX_DD)	
		{                                    	
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;   // DD BoardPinCnt
			nBoardNo = nSwPin / nBoardPinCnt;    // DD BoardNO
			nPinNo = nSwPin % nBoardPinCnt;      // DD PinNo

			// DD ¦�� �����̸� Ȧ�� ĭ�� �ǳʶٰ� ¦�� ĭ�� ���ȴ�. (nPinNo * 2) 
			nQdSwPin = nBoardNo * MAX_BOX_LINE + nPinNo * 2; // MAX_BOX_LINE: 64

			// DD�� Ȧ�� �����̸� ¦��ĭ�� �ǳʶٰ� Ȧ�� ĭ�� ���ȴ�. (nPinNo * 2 +1)
			if(nBoardNo % 2 == 1)
				nQdSwPin++;
			return nQdSwPin;
		}


	}

	// �Ϻ� 
	else
	{
		if (SysSet12.m_nPinQDLower == BOX_SD)
		{
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;							// SD BoardPinCnt : 32
			nBoardNo = (nSwPin - nOrg_UpperPartSize) / nBoardPinCnt;    	// SD BoardNO
			nPinNo = (nSwPin - nOrg_UpperPartSize) % nBoardPinCnt;      	// SD PinNo

			// SD�� Ȧ�� Line(Board or Card)�� �ǳ� �ٰ� ¦�� ���θ�(MAX_BOX_LINE *2) ����ϰ�
			// ¦�� ���ξȿ����� ¦�� ĭ�� ���ȴ�. (nPinNo * 2) 
			nQdSwPin = nQD_UpperPartSize + nBoardNo * MAX_BOX_LINE *2 + nPinNo * 2;
			return nQdSwPin;
		}

		if (SysSet12.m_nPinQDLower == BOX_DD)
		{
			nBoardPinCnt = MAX_SD_DD_CARD_PIN;							// DD BoardPinCnt : 32
			nBoardNo = (nSwPin - nOrg_UpperPartSize) / nBoardPinCnt;		// DD BoardNO    			
			nPinNo   = (nSwPin - nOrg_UpperPartSize) % nBoardPinCnt;     // DD PinNo        			
			nQdSwPin = nQD_UpperPartSize + nBoardNo * MAX_BOX_LINE + nPinNo * 2;  	

			// DD�� Ȧ�� �����̸� ¦��ĭ�� �ǳʶٰ� Ȧ�� ĭ�� ���ȴ�. (nPinNo * 2 +1)
			if(nBoardNo % 2 == 1)
				nQdSwPin++;
			return nQdSwPin;
		}

	}

	//  QD��� ������ ���� nSwPin�� �����Ѵ� .
	return nSwPin;


}
