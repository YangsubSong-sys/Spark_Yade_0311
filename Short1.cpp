
                        if (nCycleRunFlag != 2 && nProOneP_Flag1 == 2
                            && ((nFailCount[TEST_OPEN] == 0 && nFailCount[TEST_SHORT] == 0 && nFailCount[TEST_HR_SHORT] == 0)//son210331 enum
                                || (nAutoReTest[nBlockCountP] == 0 && gRun05.m_nReCh == 0))
                            && nProOneP_RunFlag1 == 1
                            && (nReSheetFlag1 != 1))  //sylee140429-1
                        {
                            if ((nFlagEstModelMake < 1) && (nProStep1[TEST_LEAK1][1] < 50000000)) {//sylee151109-2 //sylee151202                                                  

                                //if(nShortFlag1==0){//sylee170614-1
                                if ((SysInfo05.m_nShortDisable1 != 7) && (nShortFlag1 == 0)) {//sylee180331-1 
                                    nRet = ARun201_S2_SQ2_HV_Re_VB(nb1, nb2, nb3, step, 2);     //sylee160803-1 
#ifndef _PC_TEST_
                                    if (nRet == 0) { nProcessFail1 = 1; }
#endif
                                }
                                else {//sylee170614-1
                                    nRet = 0;//sylee170614-1
                                }//sylee170614-1
                                if (nRet != 1) {//sylee150423-3          //sylee150611   
                                    nRet = ARun201_S2_SQ2_HV_Re_VB(nb1, nb2, nb3, step, 1);   //multi piece
#ifndef _PC_TEST_
                                    if (nRet == 0) { nProcessFail1 = 1; }
#endif
                                }

                            }
                            else {//sylee151109-2
                                nRet = ARun201_S2_SQ2_HV_Re_VB(nb1, nb2, nb3, step, 1);  //multi piece
#ifndef _PC_TEST_
                                if (nRet == 0) { nProcessFail1 = 1; }
#endif

                            }//sylee151109-2

                        }
