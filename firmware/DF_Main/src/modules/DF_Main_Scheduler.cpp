 
#include "../DF_Main_Internal.h"
#include "DF_Main_Scheduler.h"
void imuDataOutCmdSend(int onOff)
{
	String msg = String(onOff);
	 
	 
	eNow.write(DF_Protocol_MainToRod_ImuDataControl, msg);		 
}

 
 
void usb5vChange_Check()
{
	if(TE10_USB5V)
	{
		 
		motor_AllOff();		 

		reelOut_AllOff();	 

		LogPrintln(" LG] InTST usb5v TE");

	}
}
 
 
 
#define DICSIZ	    4096
#define HASHSIZ     1001U
#define MAXLEN	    18



 
 
 
 
 
 

 


 
 
 
void Log_Control_Step()
{
	
		 
		LogPrintln(" LG] ext_led_dim_step " + String(ext_led_dim_step));
		 
		LogPrintln(" LG] bbnMotorStop_Step " + String(bbnMotorStop_Step));
	
		 
		LogPrintln(" LG] wave_tq_step " + String(wave_tq_step));
		LogPrintln(" LG] wave_bbn_tq_step " + String(wave_bbn_tq_step));

		LogPrintln(" LG] wave_torq_bbn_step " + String(wave_torq_bbn_step));
		
	
		 
		LogPrintln(" LG] bite_wf_step " + String(bite_wf_step));
	
		 
		LogPrintln(" LG] bite_justTorq_step " + String(bite_jt_step));
		LogPrintln(" LG] bite_incTorq_step " + String(bite_it_step));
		LogPrintln(" LG] bite_bldc_step " + String(bite_bldc_step));
	
			 
			LogPrintln(" LG] hitResistRdStep " + String(hitResistRdStep));
			LogPrintln(" LG] hit_resist_step " + String(hit_resist_step));
			
			 
			LogPrintln(" LG] hit_BldcStep " + String(hit_BldcStep));
			LogPrintln(" LG] hit_brk_step " + String(hit_brk_step));
	
		 
		LogPrintln(" LG] bbnFight_step " + String(bbnFight_step));
	
			 
			LogPrintln(" LG] hit_holdon_step " + String(hit_holdon_step));
			LogPrintln(" LG] holdon_bldc_step " + String(holdon_bldc_step));
			
		 
		LogPrintln(" LG] randingStep " + String(randingStep));
		 
		LogPrintln(" LG] fastHoldStep " + String(fastHoldStep));
		 
		LogPrintln(" LG] btnComboStep " + String(btnComboStep));
	
	
		 
		LogPrintln(" LG] rdy_incStep " + String(rdy_incStep)); 
		
		
		 
	 
				
}

 
 
 
void reelAllOff_SendControl()
{
	static int _to;

	switch(reelAlloffStep)
	{
		 
		case 0:
			break;

		 
		case 10:
			setTO(_to);
			reelAlloffStep = 20;	 
			break;
		
		case 20:
			if(checkTO(_to, SEND_NOW_STEP_TIME))
			{
				imuDataOutCmdSend(IMU_DATA_OFF);	 
				setTO(_to);
				reelAlloffStep = 30;
			}
			break;
		
		case 30:
			if(checkTO(_to, SEND_NOW_STEP_TIME))
			{
				sendRodVrtCmd(0,0,0,0); 	 
				setTO(_to);
				reelAlloffStep = 40;
			}
			break;
		
		case 40:
			if(checkTO(_to, SEND_NOW_STEP_TIME))
			{
				sendRodBtnLedCmd(0,0,0,0,0);	 
				setTO(_to);
				reelAlloffStep = 50;
			}
			break;

		case 50:
			if(checkTO(_to, SEND_NOW_STEP_TIME))
			{
				breakMotorOff();				 
				setTO(_to);
				reelAlloffStep = 90;	 
			}
			break;
		
		case 90:
			 
			reelAlloffStep = 0;			 
			reelOut_AllOff_Flag = 0;	 
			break;
			
		default:
			break;
	}
}


 
 
 

 
void lmLongWireCheck_Start()
{
	wireLongCheck_Flag = 1;
	currWirePulse = mainEnc.bbnCnt;	 

}

 
void lmLongWireCheck_Stop()
{
	if(wireLongCheck_Flag)
	{
		wireLongCheck_Flag = 0;
	}
}

#define LM_WIRE_ADD_TARGET	5		 
static volatile short targetAddPulse = LM_WIRE_ADD_TARGET;		 
static short lmWire_Step = 0;

 
void checkBbnCcwCheck()
{
	if(!lmWireControl_Flag)
	{
		if(BBN_MOT_CCW == mainEnc.bbnDir)	 
		{
			 
			if( (currWirePulse + targetAddPulse) < mainEnc.bbnCnt)
			{
				targetWirePulse = mainEnc.bbnCnt;	 
				lmWireControl_Start();
			}
		}
	}
}

void lmWireControl_Start()
{
	lmWireControl_Flag = 1; 	 
	lmWire_Step = 10;
}

void lmWireControl_End()
{
	lmWire_Step = 90;
}

void lmWireControl_Stop()
{

	lmWireControl_Flag = 0;
	lmWire_Step = 0;

	wireLongCheck_Flag = 0;

	
}

#define LM_WIRE_CONTROL_TO	(15*1000)
 
 
 
 
 
void lmWireControl()
{
	 
  
	 
	if(19 < lmWire_Step && 40 > lmWire_Step && BBN_MOT_CW == mainEnc.bbnDir)
	{
		 
		if((currWirePulse + targetAddPulse) > mainEnc.bbnCnt)
		{
			 
			lmWire_Step = 40;	 
		}
	}

	static unsigned long _endto=0;
	static unsigned long acCnt=0;
	static unsigned long _to;
	int tmVal = 0;
	int bbnVal = 0;
	switch(lmWire_Step)
	{
		case 0:
			break;

		case 10:
			acCnt=0;
			setTO(_endto);
			lmWire_Step = 20;
			break;

		 
		case 20:
			 
			tmVal = stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_c].tm;
			bbnVal = stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_c].bm;
			torqMotor.on(tmVal);
			bbnMotor.onBldc(BBN_MOT_CW, bbnVal);
			acCnt++;
			setTO(_to);
			lmWire_Step = 21;
			break;
			
		case 21:
			 
			if(checkTO(_to,stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_c].bmTime ))
			{
				 
				tmVal = stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_a].tm;
				bbnVal = stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_a].bm;
				torqMotor.on(tmVal);
				bbnMotor.onBldc(BBN_MOT_CW, bbnVal);
				setTO(_to);
				lmWire_Step = 22;
			}
			break;

		case 22:
			 
			if(checkTO(_to,stLmDutyTbl[FISH_LVL_1][MOT_PWR_LVL_a].bmTime ))
			{
				lmWire_Step = 20;	 
			}
			break;

		 
		case 40:
			 
			tmVal = stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_a].tm;
			bbnVal = stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_a].bm;
			torqMotor.on(tmVal);
			bbnMotor.onBldc(BBN_MOT_CW, bbnVal);
			setTO(_to);
			lmWire_Step = 41;
			break;

		case 41:
			if(checkTO(_to,stLmWireDutyTbl[FISH_LVL_1][MOT_PWR_LVL_a].tmTime ))
			{
				lmWire_Step = 90;	 
			}
			if(currWirePulse > mainEnc.bbnCnt)
			{
				lmWire_Step = 90;	 
			}
			break;

		case 90:
			if(68 > reqTorqueMotor) { tmVal = 68; }					 
			else					{ tmVal = reqTorqueMotor; }
			torqMotor.on(tmVal);
			bbnMotor.offBldc();
			 
			lmWireControl_Stop();
			break;
			
		default:
			break;
	}

	 
	if(19 <= lmWire_Step && checkTO(_endto, LM_WIRE_CONTROL_TO))	 
	{
		lmWire_Step = 90;	 
	}
	
	 
	
}

 
 
 
 
void wat_isrLogOut()
{
	 
	bool encFlagCheck;
	int encOutLog;
	unsigned int target;

	 
	portENTER_CRITICAL(&mux);   
		encFlagCheck = bIsrEncInterruptFlag;
		if(encFlagCheck) { bIsrEncInterruptFlag = 0; }
		encOutLog = pidExecOutLog;
		if(encOutLog) { pidExecOutLog = 0; target = iTargetDist; }
	portEXIT_CRITICAL(&mux);    

	if(encFlagCheck)
	{
		unsigned int isrCnt;
		unsigned int interval;
		unsigned short sEncDir;
		int intEncCnt;
		unsigned int currDist;
		double dCalVal;
		short sOutDuty;
	
		portENTER_CRITICAL(&mux);   
			isrCnt = iIsrEncInterruptCnt;
			interval = iEncIntrIntervalTime;
			sEncDir = sIsrBbnEncDir;
			intEncCnt = iIsrbbnEncCnt;
			currDist = iWireDistance;
			dCalVal = dIsrOutDuty;
			sOutDuty = sMotorSpeed;
	
		portEXIT_CRITICAL(&mux);    
	
		LogPrintln(" lg] WireC EncINT c:" + String(isrCnt)
				+ ", invt:" + String(interval)
				+ ", dir:" + String(sEncDir)
				+ ", enc:" + String(intEncCnt)
				+ ", dist:" + String(currDist)
				+ ", out:" + String(sOutDuty)
				+ ", calVal:" + String(dCalVal)
				);
	}
	
	if(encOutLog)
	{
		LogPrintln(" lg] WireC Target:" + String(target)+ ", GmSts:" + String(gmWatStatus));
	}
}

 


void t1ms_Process_Exec()
{

	 
	mainEnc.rotate();	 

	  
	 
	if(devReadychkFlag)   { devChkReady_Control(); }				 
	
	if(AP_IS_TM == apType)
	{
		if(mainMotAutoSet_Flag)
		{
			 
			 
			mainMot_MinOut_AutoSet_Control3_36PulseTime();
		}
	}

	


	 

}

 







void t10ms_Process_Exec()
{


	 

 
	 

	 
		apPatternReq_Bite_Hold();		 
	

	 
	diagInputMonitor();

	 
	
	 
	if(diagOutflag_bbnTimeOut) { diagBbnMotor_TimeoutControl(); }
	 
	if(diagBbnflag_TokTokControl) { diagBbnMotor_TokTokControl(bbnActKind); }
	 
	if(exeFlag_bbnMotorStop) { bbnMotor_OffTime_Control(); }

	if(exeFlag_torqMotorStop) { torqMotor_OffTime_Control(); }

	 
	if(diagOutflag_lmMotTimeOut) { diagLmMotor_TimeoutControl(); }


	 

	 
	{
		check_PowerSwitch_Change_10ms();
	 
	}

	 
	extLed_Control();

	 
	usb5vChange_Check();

	 
	 

	 
	if(DF_CONFIG_LMJIG == dfConfig)
	{
		if(lmHome_Flag) { lmHome_Control(); }
		else if(LM_PRIORITY_LEFT == lmPriority && lmLeft_Flag) { lmLeft_Control(); }
		else if(LM_PRIORITY_RIGHT == lmPriority && lmRight_Flag) { lmRight_Control(); }
		else if(lmLeft_Flag) { lmLeft_Control(); }
		else if(lmRight_Flag) { lmRight_Control(); }
	}

	 
	if(wireAgingTest_Flag)
	{
		if(AP_IS_TM == apType)	 
		{
			wireAging_Test_Control(); 
		}
		else
		{
			wireAging_Test_Stop("$98010");	 
		}
	}

	 
		 
		if(stbyTorqOffControl_Flag) { stbyTorqOff_Control(); }	 


}


 
 
 
void t100ms_Process_Exec()
{
	if(exeTo1secLog)	{ setAfterTo_LogOut_Control(); }
	
	if(rodRegistMode)	{ rodRegistToControl();		}	 

}

 
void t500ms_Process_Exec()
{
	 
	 
	 
	 

	 
	boardLed_Control();
}

 
 
 
void t1sec_Process_Exec()
{
	 
	sendCmdSts_MainPwrOn();
	
	 
	apAlive_Check();
	
	if(!rodRegistMode)		 
	{
		 
		send2RodAlive_CntCheck();		 
		 
		checkRodTimer();   
	}
}

static int pwrOn1stCnt = 0;
int pwrOn1stCmdSendFlag = 1;
 
 
 
void sendCmdSts_MainPwrOn()
{
	 
	if(0 == pwrOn1stCmdSendFlag)			 
	{
		return;
	}
	 
	pwrOn1stCmdSendFlag = 0;

	String respMsg = "";

	 
	eNow.write(DF_Protocol_MainToRod_PowerOn, respMsg);

	 
	respMsg = STX_INFO_REQ + ROD_REGIST_TO_SEC;
	respMsg += String(ROD_REGI_CANCEL_TO/1000);
	respMsg += "%";
	Resp2ApPrintln(respMsg);
	LogPrintln(" FW] INFOR wto:" + respMsg);

	 
	eNow.write(DF_Protocol_MainToRod_ApInfo, "0100");	 

	 

		
}


 
 
 
static volatile int logRcved_flag = 0;
static volatile int logRcved_Seq = 0;
static String logRcvApMsg = "";
static String logRcvApMsg_Save = "";

 
void logUartRecvParsing()
{

	 
	int msglength = LogSerial.available();
	if (msglength)						 
	{
		 
		for(int i=0; i<msglength; i++)
		{
			char recvCh = LogSerial.read();
			if(0) { }
			else if( 0x24 == recvCh)		 
			{
				logRcved_Seq = 50;	 
				logRcvApMsg = "$";
			}
			else if( 0x0A == recvCh)		 
			{
				logRcved_Seq = 53;  
				 
			}
			else if( 0x0D == recvCh)		 
			{
				logRcved_Seq = 52;  
				 
			}
			else if( 0x25 == recvCh)		 
			{
				if(50 == logRcved_Seq)
				{
					logRcved_Seq = 51;	 
					 
					logRcvApMsg_Save = logRcvApMsg;
					logRcved_flag = 1;				 
					break;
				}
			}
			else			 
			{
				if(50 == logRcved_Seq)
				{
					logRcvApMsg += recvCh;
				}
			}
		}

	}

}

 





void logRecvHandler()
{


	 
	if (0 == logRcved_flag)					 
	{
		return;
	}

	String msg = logRcvApMsg_Save;
	logRcvApMsg_Save = "";
	logRcved_flag = 0;  

	msg.trim(); 								 

	 
	if (msg == "")			 
	{
		return;
	}

	 
		if (0) { }	 

		else if (msg.startsWith(STX_ROD_REGIST))	 
		{
			anaRodRegist(msg);
		}
		
		 
		else if (msg.startsWith(STX_GET_ADDR))
		{
		}
		 
		else if (msg.startsWith(STX_SET_ADDR))
		{
		}
		 
		else if (msg.startsWith(STX_INFO_REQ))
		{
			LogPrintln( "LG] InfRq IF_ver: " + IfVer);
		}
		 
		else if (msg.startsWith(STX_SERVO_MOTOR))
		{
		}
		 
		else if (msg.startsWith(STX_TORQUE_MOTOR))
		{
		}

		 
		else if (msg.startsWith(STX_BBN_MOTOR))
		{
		}

		 
		else if (msg.startsWith(STX_LED_CONT))
		{
			analedContCmd(msg);
		}

		 
		else if (msg.startsWith(STX_VER_READ))
		{
			if ((ROD_CONN == rod_conn_status) && (gRodSleepStat == 0))
			{
				eNow.write(DF_Protocol_MainToRod_VersionRead, ""); 	 
				 
			}
			else
			{
				String str;
				if (g_RespVersion[0] == '$')
					str = g_RespVersion;
				else
					str = STX_VER_READ + mainVer + (g_AutoReboot ? "-R" : "") + "," + "Vs99.99.99" + "," + "Vi99.99.99" + "%";
			
				LogPrintln(" FR] Ver: " + str);		 
			}
		}

		 
		else if (msg.startsWith(STX_WAVE_SET))		 
		{
			Set_WaveData(msg);
		}

		 
		else if (msg.startsWith(STX_BITE_SET))
		{
			Set_BiteData(msg);
		}

		 
		else if (msg.startsWith(STX_HIT_SET))
		{
			Set_HitData(msg);
		}

		 
		else if (msg.startsWith(STX_BREAK_SET))
		{
			Set_BreakData(msg);
		}

		 
		else if (msg.startsWith(STX_TOKTOK_SET))
		{
			Set_BbnTokTokData(msg);
		}
		 
		else if (msg.startsWith(STX_EXTLED_SET))
		{
			Set_ExtledData(msg);
		}
		

		 
		else if (msg.startsWith(STX_ENB_SET))
		{
			Set_EnbData(msg);
		}
		else if (msg.startsWith(STX_TORQ_ADJ_SET))
		{
			Set_TorqAdjValData(msg);
		}

		 
		else if (msg.startsWith(STX_RANDING_START))
		{
			anaRanding(msg);
		}


		else if (msg.startsWith(STX_LED_SET))
		{
			Set_LedValue(msg);
		}

		 
		else if (msg.startsWith(STX_DIAG_INPUT))
		{
			mDiag_entry();
			diagInputTest(msg);
		}
		 
		else if (msg.startsWith(STX_DIAG_OUTPUT))
		{
			mDiag_entry();
		 	diagOutputTest(msg);
		}
		else if (msg.startsWith(STX_DIAG_EXIT))
		{
			mDiag_exit();
		}

		 
		else if (msg.startsWith(STX_DATA_PARA_SET))
		{
			ana_SetDataSave(msg);
		}

		 
		else if (msg.startsWith(STX_TEST_EXEC))			 
		{
			ana_TestExecution(msg);
		}
		 
		 
		else if (ROD_CONN == rod_conn_status)
		{
			 
		}
	  
}


 
#define	EX_STX_LED_OFF	"$10"
#define	EX_STX_LED_ON	"$11"
#define	EX_STX_OTHER	"$12"

 
 
 
 
void extIOuartRecvHandler()
{
	
}


static short imuIntervalCheckCnt =0;
static unsigned long measureCnt=0;
static unsigned long imuIntervalTime=0;

#define IMU_MEASURE_CNT	5
 
 
 
 
void imuInterval_Measure()
{
  	if(imuIntervalCheckCnt)
  	{
  		imuIntervalCheckCnt++;
		if(0) {}
  		else if(2 == imuIntervalCheckCnt) { measureCnt = curr_ms_tick;	} 
  		else if(3 == imuIntervalCheckCnt) { imuIntervalTime += (int)(curr_ms_tick - measureCnt); measureCnt = curr_ms_tick;	} 
  		else if(4 == imuIntervalCheckCnt) { imuIntervalTime += (int)(curr_ms_tick - measureCnt); measureCnt = curr_ms_tick;	} 
  		else if(5 == imuIntervalCheckCnt) { imuIntervalTime += (int)(curr_ms_tick - measureCnt); measureCnt = curr_ms_tick;	} 
  		else if(6 == imuIntervalCheckCnt) { imuIntervalTime += (int)(curr_ms_tick - measureCnt); measureCnt = curr_ms_tick;	} 
		else
		{
			imuIntervalTime += (int)(curr_ms_tick - measureCnt);		 
			imuIntervalTime = imuIntervalTime/IMU_MEASURE_CNT;
			 
			strImuInterval = fillZero4Char((unsigned int)imuIntervalTime);
			Resp2ApPrintln(STX_INFO_REQ+ IMU_INTVAL_TIME + strImuInterval +"%");	 
			
			imuIntervalCheckCnt = 0;	 
			imuIntervalTime = 0;
			measureCnt = 0;
		}
  	}

}

 
static unsigned short imuRecv1stTime = 0;

static String rodVer = "Vr99.99.99";


 
 
int handEncCnt = 0;
static int oldhandEncCnt = -99;

 


void nowRecvHandler()
{
	enum { DF_Main_Scheduler_ReceiveBufferSize = 128 };
	char receiveBuffer[DF_Main_Scheduler_ReceiveBufferSize];
	uint8_t sourceAddress[6];
	unsigned int receiveLength = 0;
	unsigned long overwrittenCount = 0;
	unsigned long invalidCount = 0;
	int now_rcv_id;

	DF_Main_Communication_ProcessSendResult();
	if (!DF_Main_Communication_TakeReceived(receiveBuffer, sizeof(receiveBuffer), &receiveLength, sourceAddress, &overwrittenCount, &invalidCount))
	{
		return;
	}

	String now_msg_str(receiveBuffer);
	now_rcv_id = now_msg_str.substring(0, 2).toInt();
	if (0 == now_rcv_id)
	{
		return;
	}
	if (overwrittenCount || invalidCount)
	{
		LogPrintln(" LG] nowRcv drop:" + String(overwrittenCount) + ",invalid:" + String(invalidCount));
	}
	(void)receiveLength;
	String respMsg;
	 
	 

 
	int notDefineCmdFlag = 0;
	String now_cmd_data = now_msg_str.substring(2);  

	if (now_rcv_id != DF_Protocol_Pid_RodToMain_Alive) {
		if (now_msg_str.length() > 18) {
			LogPrintln(" RD] MSG:  " + now_msg_str.substring(0, 18) + "#");	   
		}
		else {
			LogPrintln(" RD] MSG:  " + now_msg_str + "%");	   
		}
	}
	
 
	 
	if(0) { }	 

	 
	else if (DF_Protocol_Pid_RodToMain_RodAddress == now_rcv_id)	 
	{
		memcpy(rcv_src_addr_back, sourceAddress, 6);
		 
		 
		LogPrintln(" LG] RodRg BC RcvRodaddr:" + now_cmd_data);
		if(1 == rodRegistMode)		 
		{
			rodRegistExec(now_cmd_data);
		}
  	}


	 
  else if (DF_Protocol_Pid_RodToMain_Button == now_rcv_id && (!rodRegistMode))	 
  {
  		respMsg = RESP_STX_BUTTON + now_cmd_data + "%";		 
		StsSendPrintln(respMsg);	   
		
  }

	 
  else if (DF_Protocol_Pid_RodToMain_Encoder == now_rcv_id && (!rodRegistMode))	 
  {
  		respMsg = RESP_STX_ENCODER + now_cmd_data + "%";
		StsSendPrintln(respMsg);	   

  }
	else if (DF_Protocol_Pid_RodToMain_EncoderCalibration == now_rcv_id && (!rodRegistMode))
	{
		if (AP_IS_TM == apType)
		{
			respMsg = STX_ENCODER_CALIBRATION + now_cmd_data + "%";
			StsSendPrintln(respMsg);
		}
	}


   

 	 
  else if (now_rcv_id == DF_Protocol_Pid_RodToMain_ImuData && (!rodRegistMode))
  {
     
    {
    	 
       
      
      String imuStr = STX_IMU_DATA_STS + now_cmd_data + "%";
       
      ApPrintln(imuStr);		 


	   
	  if(AP_IS_TM == apType && imuIntervalCheckCnt)
	  {
		imuInterval_Measure();
	  }
	   
    }
  }
   
  else if (now_rcv_id == DF_Protocol_Pid_RodToMain_Alive && (!rodRegistMode))
  {
		 
		 
		setRod_Connected();
  }


	 
	else if (DF_Protocol_Pid_RodToMain_Version == now_rcv_id)	 
	{
	 
		g_RespVersion = STX_VER_READ + mainVer + (g_AutoReboot ? "-R" : "") + "," + now_cmd_data + "%";	 
		Resp2ApPrintln(g_RespVersion);
		
		LogPrintln(" FM] Ver__ " + g_RespVersion);	   
	}


	 
  else if (DF_Protocol_Pid_RodToMain_Battery == now_rcv_id && (!rodRegistMode))	 
  {
		 
		battLevelSet(now_cmd_data);
		
		 
		respMsg = STX_BAT_LVL + battLvlStr + "%";	 
		StsSendPrintln(respMsg);	 

		 
		if(BAT_LVL_LOW > ibatteryLvl)
		{
			respMsg = STX_DEV_CONT_REQ + ROD_BAT_LOW_WARN;
			respMsg += "%";
			StsSendPrintln(respMsg);	 
		}

  }

	 
	else if (DF_Protocol_Pid_RodToMain_BatteryDiagnostic == now_rcv_id && (!rodRegistMode))
	{
		if (AP_IS_TM == apType)
		{
			respMsg = RESP_STX_BATTERY_DIAG + now_cmd_data + "%";
			StsSendPrintln(respMsg);
		}
	}

   
  else if (DF_Protocol_Pid_RodToMain_ImuConnection == now_rcv_id)	 
  {
	 
	int recvIMUconn = now_cmd_data.substring(0).toInt();
	if(IMU_CONN_2CHAR_INT == recvIMUconn)	 
	{
		imu_conn_status = CONNECT;		 
	}
	else
	{
		imu_conn_status = DISCONNECT;		 
	}

		respMsg = STX_IMU_CONN;
		if(CONNECT == imu_conn_status)	{ respMsg += STATE_CONNECTED; }
		else							{ respMsg += STATE_DISCONN; }
		respMsg += "%";
		StsSendPrintln(respMsg);	 

	LogPrintln(" FM] IMUCN " + respMsg + ", " + String(imu_conn_status) );	 
  }

  else if (DF_Protocol_Pid_RodToMain_BoardType == now_rcv_id)  
  {
  	 
  	strRodBoardType = now_cmd_data;	 

	rodBoardType = strRodBoardType.substring(0).toInt();

	 
	respMsg = STX_INFO_REQ + ROD_BOARD_TYPE;		 
  	respMsg += strRodBoardType;		 
	respMsg += "%";
	StsSendPrintln(respMsg);	   
	
  	LogPrintln(" FS] INFOR " + respMsg + ",type:" + String(rodBoardType) );   
  }
  else if (DF_Protocol_Pid_RodToMain_Sleep == now_rcv_id)  
  {
	int stm;
	 
	battLevelSet(now_cmd_data);
	
	 
	respMsg = STX_BAT_LVL + battLvlStr + "%";	 
	StsSendPrintln(respMsg);	 

	if (now_cmd_data[3] == 'S') {
		stm = now_cmd_data.substring(4,4+4).toInt();
 	 	LogPrintf(" LG] SLEEP[r]: gRodSleepMode(%d) gRodSleepTime(%d->%d) gRodSleepStat(%d->1)\r\n", gRodSleepMode, gRodSleepTime, stm, gRodSleepStat);   
		gRodSleepTime = stm;
		gRodSleepStat = 1;
	}
	else {
		if (gRodSleepMode) {
			if (gRodSleepMode == SS_EXIT)
				stm = TIME_SLEEP_DEEP;
			else
				stm = TIME_SLEEP_LIGHT;
		}
		else {
			stm = 0;
		}
 	 	LogPrintf(" LG] SLEEP[R]: gRodSleepMode(%d) gRodSleepTime(%d->%d) gRodSleepStat(%d->0)\r\n", gRodSleepMode, gRodSleepTime, stm, gRodSleepStat);   
		gRodSleepStat = 0;
		sendSleepEnable(gRodSleepMode, stm, 3);
	}
  }


	 
  else
  {
  	notDefineCmdFlag = 1;
  }

	 
  if(notDefineCmdFlag)
  {
  		notDefineCmdFlag = 0;
		 
  }
  else				 
  {
	   
	   
	  rodAlive_SendCnt = 0;
  }

  
}

volatile int rcved_flag = 0;
int rcved_Seq = 0;
String rcvApMsg = "";
String rcvApMsg_Save = "";

 
void uartRecvParsing()
{


	 
	int msglength = Serial.available();
	if (msglength > 0)						 
	{
	 
		 
		for(int i=0; i<msglength; i++)
		{

				char recvCh = Serial.read();
				if( 0x24 == recvCh)		 
				{
					rcved_Seq = 50;	 
					rcvApMsg = "$";
				}
				else if( 0x0A == recvCh)		 
				{
					rcved_Seq = 53;  
					 
				}
				else if( 0x0D == recvCh)		 
				{
					rcved_Seq = 52;  
					 
				}
				else if( 0x25 == recvCh)		 
				{
					if(50 == rcved_Seq)
					{
						rcved_Seq = 51;	 
						 
						if (rcved_flag == 0) {
							rcvApMsg_Save = rcvApMsg;
							rcved_flag = 1;				 
						}
						break;
					}
				}
				else			 
				{
					if(50 == rcved_Seq)
					{
						rcvApMsg += recvCh;

					}
				}
			
		}

	}
}

	
 


void uartRecvHandler()
{
 






	if (0 == rcved_flag) 	 
	{
		return;
  	}	 

	 
	String msg = rcvApMsg_Save;
	rcvApMsg_Save = "";
	rcved_flag = 0;  

	msg.trim();				 
	 
    if (msg == "")			 
    {
    	return;
    }
 
 
 
 
	 
	if(!(msg.startsWith(STX_SERVO_MOTOR)) && !(msg.startsWith(STX_MAINALIVE)))
	{
		if (msg.length() > 10) {
			LogPrintln(" AP] ApCmd " + msg.substring(0, 10) + "%");	   
		}
		else {
			LogPrintln(" AP] ApCmd " + msg + "%");	   
		}
	}
 
	setTO(g_LastCmdTick);						 
	g_RecvCmdCount++;							 

	 
	 
	if(0) {   }

	else if (msg.startsWith(STX_ENCODER_CALIBRATION))
	{
		String payload = msg.substring(3);
		char action = payload.length() ? payload.charAt(0) : '\0';
		if (AP_IS_TM != apType) return;
		if ((DF_Protocol_EncoderCalibrationAction_Start != action) &&
			(DF_Protocol_EncoderCalibrationAction_Complete != action) &&
			(DF_Protocol_EncoderCalibrationAction_Apply != action) &&
			(DF_Protocol_EncoderCalibrationAction_Cancel != action) &&
			(DF_Protocol_EncoderCalibrationAction_Query != action))
		{
			Resp2ApPrintln(STX_ENCODER_CALIBRATION + "6,0,0,0%");
			return;
		}
		if ((ROD_CONN != rod_conn_status) || rodRegistMode || (0 != gRodSleepStat))
		{
			Resp2ApPrintln(STX_ENCODER_CALIBRATION + "5,0,0,0%");
			return;
		}
		eNow.write(DF_Protocol_MainToRod_EncoderCalibration, String(action));
		return;
	}

	 
    else if (msg.startsWith(STX_ROD_REGIST))	 
	{
		anaRodRegist(msg);
    }

	 
    else if (msg.startsWith(STX_MAINALIVE))	 
	{
		anaMainAlive(msg);
    }

	 
    else if (msg.startsWith(STX_AP_INFO_SEND))	 
	{
		anaApInfo(msg);
    }
	
	 
    else if (msg.startsWith(STX_TM_INPUT_REQ))	 
	{
		if(AP_IS_TM == apType)
		{
			anaTmInputTestReq(msg);
		}
    }

	 
    else if (msg.startsWith(STX_TM_OUTPUT_REQ))	 
	{
		if(AP_IS_TM == apType)
		{
			anaTmOutputTestReq(msg);
		}
    }
	

	 
	else if (msg.startsWith(STX_INFO_REQ))
	{
		anaDeviceInfoReq(msg);
	}
	
	
	 
    else if (msg.startsWith(STX_GET_ADDR))
	{
      String str = STX_GET_ADDR + "M" + eNow.getMyAddress() + "," + eNow.getTargetAddress() + "%";
      Resp2ApPrintln(str);
    }

	 
    else if (msg.startsWith(STX_SET_ADDR))
	{
      msg = msg.substring(3, msg.length());		 
	  int len = msg.length();
      if((DF_Protocol_MacAddressTextLength - 1) < len)
      {
      	 
      	eNow.setTargetAddress(msg);
	  		LogPrintln(" LG] T_ADDR Len:" + String(len) + ",msg:" + msg);
      }
	  else
	  {
	  	LogPrintln(" LG] ERROR T_ADR Len:" + String(len) + ",msg:" + msg);
	  }
    }

	 
    else if (msg.startsWith(STX_TORQUE_MOTOR))
    {
 		ana_MainMotControl(msg);
	}

	 
    else if (msg.startsWith(STX_SERVO_MOTOR))
    {
		ana_AngleControl(msg);
    }

	 
    else if (msg.startsWith(STX_BBN_MOTOR))
    {
		 
		ana_BobbinControl_Check(msg);
    }

	 
	else if((DF_CONFIG_LMJIG == dfConfig) && msg.startsWith(STX_RESERVE07))
	{
		ana_LineMotControl_Check(msg);
	}

	 
	else if (msg.startsWith(STX_LED_CONT))
	{
		analedContCmd(msg);
	}

	 
    else if (msg.startsWith(STX_VER_READ))
    {
    
      ApFgVer = msg.substring(3);

		if ((ROD_CONN == rod_conn_status) && (gRodSleepStat == 0))
		{
			eNow.write(DF_Protocol_MainToRod_VersionRead, "");		 
			 
		}
		else
		{
			String str;
			if (g_RespVersion[0] == '$')
				str = g_RespVersion;
			else
				str = STX_VER_READ + mainVer + (g_AutoReboot ? "-R" : "") + "," + "Vs99.99.99" + "," + "Vi99.99.99" + "%";
			Resp2ApPrintln(str);
			LogPrintln(" FM] Ver__ " + str);		 
		}
	}

	 
	else if (msg.startsWith(STX_GAME_STS))
    {
		anaGameStatus(msg);
	}

	 
	else if (msg.startsWith(STX_GAME_STS_WAT))
    {
		anaGameStatusWAT(msg);
	}


	 
	else if (msg.startsWith(STX_DEV_CONT_REQ))
    {
		anaDeviceContReq(msg);
		
	}
	
	 
	
	else if (msg.startsWith(STX_APSTS_RESP))
	 
	{
		int i = -1; 
		 
		i = msg.substring(3).toInt();
		if(0 == i)		 
		{ 
			recvPwrOffExec_Resp = 1;
			 
	  		LogPrintln(" LG] PWR_C RecvPowerOffExec from AP opt= " + String(i));
		}
	}

	 
	else if (msg.startsWith(STX_MOT_OUT_RATE_SET))
	{
		anaMotOutRateSet(msg);
	}

 
	 
	else if (msg.startsWith(STX_DIAG_INPUT))
	{
		Resp2ApPrintln(msg);
		mDiag_entry();
		diagInputTest(msg);
	}
	 
	else if (msg.startsWith(STX_DIAG_OUTPUT))
	{
		Resp2ApPrintln(msg);
		mDiag_entry();
		diagOutputTest(msg);
	}
	else if (msg.startsWith(STX_DIAG_EXIT))
	{
		mDiag_exit();
	}


 
     
     
    
     
     

	 
	else if (msg.startsWith(STX_BREAK_MOTOR))
	{
		 
		if (ROD_CONN == rod_conn_status  && (!rodRegistMode))
		{
			 
	  		if(ROD_BOARD_V1 == rodBoardType)	 
	  		{
		 		reqBreakMotor = msg.substring(3).toInt();	 
				String sendmsg = String(reqBreakMotor);		 
				 
				eNow.write(DF_Protocol_MainToRod_Break, sendmsg);
	  		}
		}
	}

	 
	else if (msg.startsWith(STX_IMU_SET))
	{
	 
		 
		if (ROD_CONN == rod_conn_status  && (!rodRegistMode))
		{	
			 
			int act = msg.substring(3,(3+1)).toInt();		 
			if( (IMU_DATA_OFF == act) || (IMU_DATA_ON  == act) )
			{
				String sendmsg = msg.substring(3,(3+1));
				eNow.write(DF_Protocol_MainToRod_ImuDataControl, sendmsg);		 
				reqImuMeasFlag = act;

				 
					String respStr = msg + "%";
					Resp2ApPrintln(respStr);	 

				 
				if(AP_IS_TM == apType)	 
				{
					if(IMU_DATA_ON  == act) { imuIntervalCheckCnt = 1; }		 
					else					{ imuIntervalCheckCnt = 0; }		 
				}
			}
			 
			 
			 
			else	 
			{
					String respStr = msg + "_ER%";
					Resp2ApPrintln(respStr);
			}
		
		}
	}


	 
	else if (msg.startsWith(STX_VRT_MOT_CONT))
	{
		String logMsg = "";
		if(ROD_BOARD_V2 == rodBoardType)
		{
			logMsg = anaVrtMotCmd(msg);
		}
		Resp2ApPrintln(msg + "%");
		
	}

	 
	else if (msg.startsWith(STX_BTN_LED_CONT))
	{
		String logMsg = "";
		if(ROD_BOARD_V2 == rodBoardType)
		{
			logMsg = anaBtnLedCmd(msg);
		}
		Resp2ApPrintln(msg + "%");
	  
	}

	 
	else if(msg.startsWith(STX_DEV_SET_REQ))	 
	{
		String	rtsMsg = "";
		short errK = ERR_OK;
		
		rtsMsg = anaDevSetReq(msg);
		if(0 > rtsMsg.toInt())	 
		{
			 
			errK = ERR_NG;
			Resp2ApPrintln(msg + rtsMsg + "%");
		}
		else
		{
			 
			 
		}
	
	}

	 
	else if (msg.startsWith(STX_DEV_FUNCTION_REQ))
	{
		String	rtsMsg = STR_ERR_OK;
		short errK = ERR_OK;
		
		rtsMsg = anaDevFunctionReq(msg);
		 
		Resp2ApPrintln(msg.substring(0,(0+6)) + rtsMsg + "%");	 

	}

	
	 
	else if (msg.startsWith(STX_DATA_PARA_SET))
	{
		ana_SetDataSave(msg);
	}

	 


	else if (msg.startsWith(STX_TEST_EXEC))			 
	{
		ana_TestExecution(msg);
	}
	
 
	else
	{
  		 
	}

}

 
 
 
String anaVrtMotCmd(String msg)
{
	String logMsg = " ok";

	int act = msg.substring(3,(3+1)).toInt();
	if(5 < act)
	{
		return " Ng act";;
	}

	switch(act)
	{
			case 0:  
			case 1:  
			case 2:  
			case 3:  
			case 4:  
			case 5:  
				 
				eNow.write(DF_Protocol_MainToRod_VerticalMotor, msg.substring(3));		 
				break;

			default:
				break;
				
	}
	
	return logMsg;
}

 
 
 
 
 
 
 
void ana_SetDataSave(String msg)
{
	int act;
	int kind;
	int iPara;
	double dPara;
	int paraNG = 0;		 
	String respMsg;
	String logMsg;

	int fish,pwr,d1,t1,d2,t2,d3,t3;
	
	act = msg.substring(3,(3+2)).toInt();	 
	switch(act)
	{
		case 0:		 
			paraNG=1;
			break;
			

		case 40:		 
			 
			for(int i=0; i < ENC_INTV_BUFF_SIZE; i++)
			{
				LogPrintln(" lg] EnInt Interval:" +String(i)
					+ ","+ String(stEncIntvBuff[i].tick)
					+ ","+ String(stEncIntvBuff[i].width)
					+ ","+ String(stEncIntvBuff[i].dir)
					+ ","+ String(stEncIntvBuff[i].enc)
					+ ","+ String(stEncIntvBuff[i].dist)
					);
			}

			break;

		case 41:		 
			wat_PidParaSet(msg);
			break;
			
		default:
			paraNG = 1;
			break;
	}
	
	 
	respMsg = msg.substring(0,(0+5)) + String(paraNG) + "%";
	Resp2ApPrintln(respMsg);

}

 
 
 
String anaBtnLedCmd(String msg)
{
		String logMsg = " ok";
		 
		{
			int act = msg.substring(4,(4+1)).toInt();		 
			if(5 < act)
			{
				return " Ng act";;
			}
			
			int posi = msg.substring(3,(3+1)).toInt();		 
			if(BTN_LED_ALL_POSI == posi && (0==act||1==act))	 
			{
				 
			}
			else if(BTN_LED_LAST_POSI < posi)		 
			{
				return " Ng posi";
			}
			
			switch(act)
			{
				case 0:	 
				case 1:	 
				case 2:  
				case 3:  
				case 4:  
				case 5:	 
					eNow.write(DF_Protocol_MainToRod_ButtonLed, msg.substring(3));		 
					break;
		  
				default:
					break;
			}
		}
	return logMsg;	
}


#define STR_RTS_OK			"00"
#define STR_RTS_NG  		"-1"
#define STR_RTS_RANGE_OVER	"-2"
#define STR_RTS_PARANG  	"-3"

 
 
 
String anaDevSetReq(String msg)
{
	String rts = STR_RTS_OK;	 
	
	int iKind = msg.substring(3,(3+2)).toInt();		 
	switch(iKind)
	{
		case 0:
			rts = STR_RTS_PARANG;
			break;

		case 1:
			 
			if(AP_IS_TM == apType)
			{
				rts = anaDevSet_MainMotor_AutoSet(msg);
			}
			else
			{
				rts = STR_ERR_FAIL;
			}			
			break;

		default:
			rts = STR_RTS_PARANG;
			break;
	}

	return rts;
}

 
String anaDevSet_MainMotor_AutoSet(String msg)
{
	String rts = STR_RTS_OK;	 
	
	int iAct = msg.substring(5,(5+1)).toInt();		 
	switch(iAct)
	{
		case 0:
			mainMot_MinOut_AutoSet_Stop(msg);	 
			break;

		case 1:
			 
			mainMot_MinOut_AutoSet_Start(msg);
			break;

		default:
			rts = STR_RTS_PARANG;
			break;
	}

	return rts;
}


 
 
 
String anaDevFunctionReq(String msg)
{
	String rts = STR_RTS_OK;	 
	
	int iKind = msg.substring(3,(3+2)).toInt();		 
	switch(iKind)
	{
		case 0:
			rts = STR_RTS_PARANG;
			break;

		case 1:
			 
			if(AP_IS_TM == apType)
			{
				rts = anaDevFunc_wireAging(msg);
			}
			else
			{
				rts = STR_ERR_FAIL;
			}
			break;

		default:
			rts = STR_RTS_PARANG;
			break;
	}

	return rts;
}


 
String anaDevFunc_wireAging(String msg)
{
	String rts = STR_RTS_OK;	 
	
	int iAct = msg.substring(5,(5+1)).toInt();		 
	switch(iAct)
	{
		case 0:
			wireAging_Test_Stop(msg);
			break;

		case 1:
			 
			wireAging_Test_Start(msg);
			break;

		default:
			rts = STR_RTS_PARANG;
			break;
	}

	return rts;
}


 
 
void mainMot_MinOut_AutoSet_Start(String msg)
{
	mainMotAutoSet_Flag = 1;
	mainMotAutoSet_Step = 10;
}

void mainMot_MinOut_AutoSet_Stop(String msg)
{
	mainMotAutoSet_Flag = 0;
	mainMotAutoSet_Step = 0;
	torqMotor_DefaultOut();		 
	bbnMotor.offBldc();			 
}

 
void torqMotor_DefaultOut()
{
	if(recv_TorqMotor_Flag)
	{
		torqMotor.setValue(reqTorqueMotor);
	}
	else
	{
		torqMotor.setValue(defaultTorqueMotor);
	}
}

 
#define MAINMOT_MIN_RETRY_CNT	(1)	 


 


#define MAINENCA_SPEC_PERIOD_SP_HIGH	102  
#define MAINENCA_SPEC_PERIOD_SP_MID		112  
#define MAINENCA_SPEC_PERIOD_SP_LOW		122  


 
#define BBN_REV_1TIME_SIGCHG_CNT	(18*2*1)
#define BBN_REV_2TIME_SIGCHG_CNT	(18*2*2)
#define BBN_REV_3TIME_SIGCHG_CNT	(18*2*3)
#define BBN_REV_4TIME_SIGCHG_CNT	(18*2*4)

#define BBN_REV_READY_SIGCHG_CNT	(70*2*1)
#define BBN_REV_TIMEOUT_SIGCHG_CNT	(80*2*1)

#define MAINMOT_MIN_AUTOSET_TO_1S		1000  
#define MAINMOT_MIN_AUTOSET_TO_2S		2000  
#define MAINMOT_MIN_AUTOSET_TO_2p5S		2500  
#define MAINMOT_MIN_AUTOSET_TO_3S		3000  
#define MAINMOT_MIN_AUTOSET_TO_3p2S		3200  


#define MAINMOT_MIN_AUTOSET_REV_1ST_TIME	500  
#define MAINMOT_MIN_AUTOSET_REV_TIME	1200  

#define MAINMOT_MIN_AUTOSET_BREAK_TO		50

static unsigned long lastPulseWidth = 9999;
unsigned long pulseWidthMs = 9999;

#define PULSE_RANGE_OK (MAINENCA_SPEC_PERIOD_SP_HIGH < pulseWidthMs) && (MAINENCA_SPEC_PERIOD_SP_LOW > pulseWidthMs)

#define PULSE_MEASURE_CNT	4	 
#define PULSE_OK_MIN_CNT	6
#define PULSE_OK_SPEC_CNT	36	 

 
 
 
 
 
 
 
 

 
void mainMot_MinOut_AutoSet_Control1_Period()
{
		static unsigned long _to;
	
		static unsigned long _toSenChg;

		static unsigned long _toReverseTime;
	
		static unsigned short oldmainMotAutoSet_Step;
		static short errK;
	
		static int _out;
		static unsigned short _oldSen;
	
		static unsigned short _retryCnt;
		static unsigned short _chgCnt;

		static unsigned short speed_high;	 

		static int _deltaOut;
		
		short result;
		
		String msg;
	
	
		
		switch(mainMotAutoSet_Step)
		{
	
			 
			case 0:
				break;
			
			 
			case 10:
				torqOff();					 
				bbnMotor.offBldc();

				_retryCnt = 0;		 
				_toReverseTime = 0;
				
				_chgCnt = 0;
				_toSenChg = 0;
				speed_high = 0;
				errK = ERR_OK;
				_out = (70+1);		 
				_deltaOut = 0;

				setTO(_to);
				mainMotAutoSet_Step = 11;
				break;
			
			 
			case 11:
				if(checkTO(_to, 50))	 
				{
					setTO(_to);
					mainMotAutoSet_Step = 20;
				}
				break;


			 
			case 20:
				 
				if(0 == _retryCnt)	{ _toReverseTime = MAINMOT_MIN_AUTOSET_REV_1ST_TIME; }
				else				{ _toReverseTime = MAINMOT_MIN_AUTOSET_REV_TIME; }
				 
				if(MAINMOT_MIN_RETRY_CNT < _retryCnt)	 
				{
					errK = ERR_RETRY_OVER;		 
			
					setTO(_to);
					mainMotAutoSet_Step = 75;  
				}
				else	 
				{
					 
					bbnMotor.onBldc(BBN_MOT_CCW, (DEVCHK_BBN_WIRE_REL_DUTY+0) );	 

					setTO(_to);
					mainMotAutoSet_Step = 21;		
				}
				break;
				
			case 21:
				if(checkTO(_to, _toReverseTime) )	 
				{
					bbnMotor.offBldc();
					torqMotor.setValue(70);	 
					setTO(_to);
					mainMotAutoSet_Step = 22;	 
				}
				break;
				
			case 22:
				 
				if(checkTO(_to, 80) )	 
				{
					 
					torqOff();
					setTO(_to);
					mainMotAutoSet_Step = 23;
				}
				break;

			case 23:
				 
				if(checkTO(_to, 70) )	 
				{
					setTO(_to);
					mainMotAutoSet_Step = 30;
				}
				break;

			 
			case 30:
				 
				_chgCnt = 0;
				_oldSen = LVL01_BBNENCA;
				pulseWidthMs = 9999;				
			 
				 
			 
				torqMotor.setValue(_out);	 
				setTO(_to);
				mainMotAutoSet_Step = 31;
				break;

			case 31:
				 
				if(LVL01_BBNENCA != _oldSen)	 
				{
					_oldSen = LVL01_BBNENCA;	 
					_chgCnt++;
				}
				 
				if(PULSE_MEASURE_CNT <= _chgCnt)	 
				{
					 
					portENTER_CRITICAL(&mux);   
						pulseWidthMs = iEncIntrIntervalTime;	 
					portEXIT_CRITICAL(&mux);    
				}

				 
				if(0) {}	 
				 
				else if( checkTO(_to, MAINMOT_MIN_AUTOSET_TO_3S) )  
				{
					 
					if(PULSE_MEASURE_CNT > _chgCnt)	 
					{
						errK= ERR_FAIL;
						setTO(_to);
						mainMotAutoSet_Step = 75;
					}

					 
					else
					{
						 
						_deltaOut = ( (pulseWidthMs - MAINENCA_SPEC_PERIOD_SP_MID)*10/15 );
						if(0 == _deltaOut)
						{
							if(MAINENCA_SPEC_PERIOD_SP_MID < pulseWidthMs)	 
								_deltaOut++;
							else
								_deltaOut--;
						}
						_out += _deltaOut;
						if(60 > _out) {_out = 60;}
						else if(80 < _out) {_out = 80;}
						 









						setTO(_to);
						mainMotAutoSet_Step = 32;		 
					}
				}

			
				 
				 
				else if( checkTO(_to, MAINMOT_MIN_AUTOSET_TO_2S) )
				{
					 
					if(speed_high && PULSE_RANGE_OK )
					{
						speed_high = 0;
						 
					 	if( 59 < _out && 81 > _out)  
						{
							errK = ERR_OK;		 
							 
						}
						else
						{
							errK = ERR_RANGE_OVER;  
						}
						setTO(_to);
						mainMotAutoSet_Step = 75;					
					}
					 
					else if(!speed_high && (PULSE_OK_MIN_CNT < _chgCnt) && (MAINENCA_SPEC_PERIOD_SP_LOW > pulseWidthMs))
					{
						speed_high = 1;
						 
						_out--; 		 
						setTO(_to);
						mainMotAutoSet_Step = 32;		
					}
				}

				 
				else if( !speed_high && (PULSE_OK_MIN_CNT < _chgCnt) && (MAINENCA_SPEC_PERIOD_SP_LOW > pulseWidthMs))	 
				{
					speed_high = 1;
					 
					_out--; 		 
					setTO(_to);
					mainMotAutoSet_Step = 32;
				}
				break;

			 
			case 32:
				 
				lastPulseWidth = pulseWidthMs;
				torqOff();
				bbnMotor.onBldc(BBN_MOT_CCW, (DEVCHK_BBN_WIRE_REL_DUTY+0) );
				setTO(_to);
				mainMotAutoSet_Step = 33;					


				break;

			case 33:
				if(checkTO(_to, MAINMOT_MIN_AUTOSET_BREAK_TO) )	 
				{
					bbnMotor.offBldc();
					setTO(_to);
					mainMotAutoSet_Step = 34;	 
				}
				break;

			 
			case 34:
				if(checkTO(_to, 200) )	 
				{
					_retryCnt++;		 
					setTO(_to);
					mainMotAutoSet_Step = 20;	 
				}
				break;


			 
			case 50:
				break;


			 
			case 75:
				 
				lastPulseWidth = pulseWidthMs;
				torqOff();
				bbnMotor.offBldc();
			 
				 
			 
			
				if(ERR_OK == errK)
				{
					 
					result = fsInfo.saveInfo(MAINMOT_INFO, String(_out));
					if(0 < result)	 
					{
						defaultTorqueMotor = (int)_out;
						
						 
						msg = STX_INFO_REQ + MAINMOT_INFO_READ;
						msg += String(defaultTorqueMotor);
						msg += "%";
						Resp2ApPrintln(msg);
						
						mainMotAutoSet_Step = 90;  
					}
					else	 
					{
						errK = ERR_FAIL;
						mainMotAutoSet_Step = 80;  
					}
				}
				else
				{
					 
					mainMotAutoSet_Step = 80;  
				}
				break;
				
			 
			case 80:
				mainMotAutoSet_Flag = 0;	 
				mainMotAutoSet_Step = 0;
				 

				 
				msg = STX_DEV_SET_REQ + STR_MAINMOT_MINSET_REQ + "1" + String(errK);	 
				msg += "%";
				Resp2ApPrintln(msg);
				
				 
				msg = STX_DEV_CONT_REQ + MAIN_MMOT_AUTOSET_ERR;
				msg += "%";
				Resp2ApPrintln(msg);
			
				
				break;
			
			case 90:
				bbnMotor.onBldc(BBN_MOT_CCW, (DEVCHK_BBN_WIRE_REL_DUTY+0) );
				setTO(_to);
				mainMotAutoSet_Step = 91;					
				break;

			case 91:
				if(checkTO(_to, 50) )	 
				{
					bbnMotor.offBldc();
					setTO(_to);
					mainMotAutoSet_Step = 93;	 
				}
				break;
			 
			case 93:

				mainMotAutoSet_Flag = 0;	 
				mainMotAutoSet_Step = 0;	 

				 
				msg = STX_DEV_SET_REQ + STR_MAINMOT_MINSET_REQ;
				msg += "1";		 
				msg += String(defaultTorqueMotor);
				msg += "%";
				Resp2ApPrintln(msg);

				 
				msg = STX_DEV_CONT_REQ + MAIN_DEV_CHK_OK;
				msg += "%";
				Resp2ApPrintln(msg);
				
		
				break;
			
			default:
				break;
				

		}

}

static int minOut = 0;
 
 
 
void mainMot_MinOut_AutoSet_Control2_Time()
{

		static unsigned long _to;
		static unsigned long _toErr;
		static long okTime;
		static long logTime;
	
		static unsigned long _toSenChg;

		static unsigned long _toReverseTime;
	
		static unsigned short oldmainMotAutoSet_Step;
		static short errK;

		static int _out;
		
		static unsigned short _oldSen;
	
		static unsigned short _retryCnt;
		static unsigned short _chgCnt;

		static unsigned short speed_high;	 

		static int _deltaOut;
		
		short result;
		
		String msg;
		String logMsg;
	
	
		
		switch(mainMotAutoSet_Step)
		{
			 
			case 0:
				break;
			
			 
			case 10:
				torqOff();					 
				bbnMotor.offBldc();

				_retryCnt = 0;		 
				_toReverseTime = 0;
				
				_chgCnt = 0;
				_toSenChg = 0;
				speed_high = 0;
				errK = ERR_OK;
				
				_out = (0);		 
				minOut = 0;
				_deltaOut = 0;

				okTime = 1060;		 
				logTime = 5555;		 

				setTO(_to);
				setTO(_toErr);
				mainMotAutoSet_Step = 11;
				break;
			
			 
			case 11:
				if(checkTO(_to, 50))	 
				{
					setTO(_to);
					mainMotAutoSet_Step = 20;
				}
				break;


			 
			case 20:
				 
				_retryCnt++;
				if(MAINMOT_MIN_RETRY_CNT < _retryCnt)	 
				{
					if(ERR_OK == errK) { errK = ERR_RETRY_OVER; }		 
					setTO(_to);
					mainMotAutoSet_Step = 75;  
				}
				else	 
				{
					torqMotor.setValue(70);	 
					setTO(_to);
					mainMotAutoSet_Step = 30;
				}
				break;

			 
			case 30:
				 
				_chgCnt = 0;
				_oldSen = LVL01_BBNENCA;
				pulseWidthMs = 9999;

				setTO(_to);
				setTO(_toErr);
				mainMotAutoSet_Step = 31;
				break;

			case 31:
				 
				if(LVL01_BBNENCA != _oldSen)	 
				{
					_oldSen = LVL01_BBNENCA;	 
					_chgCnt++;
				}
				
				 
				if(PULSE_MEASURE_CNT <= _chgCnt)	 
				{
					 
					portENTER_CRITICAL(&mux);   
						pulseWidthMs = iEncIntrIntervalTime;	 
					portEXIT_CRITICAL(&mux);    
				}

				 
				if(0) {}	 

				 
				else if( checkTO(_to, MAINMOT_MIN_AUTOSET_TO_3S) )  
				{
					logTime = getTO(_to);
					 
					setTO(_to);
					if(PULSE_OK_MIN_CNT > _chgCnt)
					{
						errK= ERR_NOT_ROTATE;	 
						mainMotAutoSet_Step = 75;		 
					}
					else
					{
						errK= ERR_TO_OVER;	 
						mainMotAutoSet_Step = 32;		 
					}
				}
				
				 
				else if( (PULSE_OK_MIN_CNT < _chgCnt) && (MAINENCA_SPEC_PERIOD_SP_MID > pulseWidthMs) )
				{
					lastPulseWidth = pulseWidthMs;
					okTime = getTO(_to);
					logTime = okTime;
					if(0 > okTime)
					{
						errK = ERR_FW_CAL_NG;  
						setTO(_to);
						mainMotAutoSet_Step = 75;		 
					}
					else
					{
						_out = MAIN_MOT_DEFAULT_MIN_DUTY + ((okTime - 1060)/88);
						 
						 
						 
						 
						 
						if(MAIN_MOT_DEFAULT_MIN_DUTY > _out)
						{
							 
							minOut = MAIN_MOT_DEFAULT_MIN_DUTY;
						}
						else if (MAIN_MOT_DEFAULT_MAX_DUTY < _out)
						{
							minOut = MAIN_MOT_DEFAULT_MAX_DUTY;
						}
						else
						{
							minOut = _out;
						}				
						errK= ERR_OK;	 
						setTO(_to);
						mainMotAutoSet_Step = 32;		 
					}
				}
				break;

			
			 
			case 32:
				torqOff();
				bbnMotor.onBldc(BBN_MOT_CCW, (DEVCHK_BBN_WIRE_REL_DUTY+0) );
				setTO(_to);
				mainMotAutoSet_Step = 33;
				break;

			 
			case 33:
				if(checkTO(_to, MAINMOT_MIN_AUTOSET_BREAK_TO) )	 
				{
					bbnMotor.offBldc();
					setTO(_to);
					mainMotAutoSet_Step = 34;	 
				}
				break;

			 
			case 34:
				if(checkTO(_to, 200) )	 
				{

					if(ERR_OK == errK)
					{
						setTO(_to);
						mainMotAutoSet_Step = 75; 	 
					}
					else
					{
						setTO(_to);
						mainMotAutoSet_Step = 20;	 
					}
				}
				break;

			 
			case 50:
				break;


			 
			case 75:
				 
				torqOff();
				bbnMotor.offBldc();
			 
				 
			 
			
				if(ERR_OK == errK)
				{
					 
					result = fsInfo.saveInfo(MAINMOT_INFO, String(minOut));
					if(0 < result)	 
					{
						defaultTorqueMotor = minOut;
						
						 
						msg = STX_INFO_REQ + MAINMOT_INFO_READ;
						msg += String(defaultTorqueMotor);
						msg += "%";
						Resp2ApPrintln(msg);
						
						mainMotAutoSet_Step = 90;  
					}
					else	 
					{
						errK = ERR_FS_WRITE;		 
						mainMotAutoSet_Step = 80;	 
					}
				}
				else
				{
					 
					mainMotAutoSet_Step = 80;  
				}

				 
				Resp2ApPrintln( STX_DEV_CHK_ERR_LOG + MAIN_MMOT_AUTOSET_ERR + (ERR_OK==errK? "OK":"NG") + ",try:" + String(_retryCnt) + ",Out:" + String(_out)  + ",Save:" + String(minOut) +",ms:" + String(logTime) );
				
				
				break;
				
			 
			case 80:
				mainMotAutoSet_Flag = 0;	 
				mainMotAutoSet_Step = 0;
				 

				 
				msg = STX_DEV_SET_REQ + STR_MAINMOT_MINSET_REQ + "1" + String(errK);	 
				msg += "%";
				Resp2ApPrintln(msg);
				
				 
				msg = STX_DEV_CONT_REQ + MAIN_MMOT_AUTOSET_ERR;
				msg += "%";
				Resp2ApPrintln(msg);
			
				
				break;

				 
			case 90:
				mainMotAutoSet_Flag = 0;	 
				mainMotAutoSet_Step = 0;	 

				 
				msg = STX_DEV_SET_REQ + STR_MAINMOT_MINSET_REQ;
				msg += "1";		 
				msg += String(defaultTorqueMotor);
				msg += "%";
				Resp2ApPrintln(msg);

				 
				msg = STX_DEV_CONT_REQ + MAIN_DEV_CHK_OK;
				msg += "%";
				Resp2ApPrintln(msg);
				
		
				break;

			default:
				break;
	}
}

#define MMOT_AUTOSET_OUT_DUTY	80	 
 
 
 
 
 

void mainMot_MinOut_AutoSet_Control3_36PulseTime()
{

		static unsigned long _to;
		
		static long okTime = 0;
	
		 

		 
	
		static unsigned short oldmainMotAutoSet_Step;
		static short errK;

		static int _out;
		 
		
		static unsigned short _oldSen;
	
		static unsigned short _retryCnt;
		static unsigned short _chgCnt;

		 

		static int _deltaOut;
		
		short result;
		
		String msg;
		String logMsg;
	
	
		
		switch(mainMotAutoSet_Step)
		{
			 
			case 0:
				break;
			
			 
			case 10:
				torqOff();					 
				bbnMotor.offBldc();

				_retryCnt = 0;		 
				 
				
				_chgCnt = 0;
				 
				 
				errK = ERR_OK;
				
				_out = (0);		 
				 
				minOut = 0;
				 

				okTime = 0;		 
				 

				setTO(_to);
				 
				mainMotAutoSet_Step = 11;
				break;
			
			 
			case 11:
				if(checkTO(_to, 50))	 
				{
					setTO(_to);
					mainMotAutoSet_Step = 20;
				}
				break;


			 
			case 20:
				 
				_retryCnt++;
				if(MAINMOT_MIN_RETRY_CNT < _retryCnt)	 
				{
					if(ERR_OK == errK) { errK = ERR_RETRY_OVER; }		 
					setTO(_to);
					mainMotAutoSet_Step = 75;  
				}
				else	 
				{
					_chgCnt = 0;
					_oldSen = LVL01_BBNENCA;
					pulseWidthMs = 9999;
					
					torqMotor.setValue(MMOT_AUTOSET_OUT_DUTY);	 
					setTO(_to);
					 
					mainMotAutoSet_Step = 31;
				}
				break;

			 
			case 30:
				 
				 
				 

				setTO(_to);
				 
				mainMotAutoSet_Step = 31;
				break;

			case 31:
				 
				if(LVL01_BBNENCA != _oldSen)	 
				{
					_oldSen = LVL01_BBNENCA;	 
					_chgCnt++;
				}
				
				 
				if(PULSE_MEASURE_CNT <= _chgCnt)	 
				{
					 
					portENTER_CRITICAL(&mux);   
						pulseWidthMs = iEncIntrIntervalTime;	 
					portEXIT_CRITICAL(&mux);    
				}

				 
				if(0) {}	 

				 
				else if( checkTO(_to, MAINMOT_MIN_AUTOSET_TO_3p2S) )  
				{
					okTime = getTO(_to);
					lastPulseWidth = pulseWidthMs;
					 
					setTO(_to);
					if(PULSE_OK_MIN_CNT > _chgCnt)
					{
						errK= ERR_NOT_ROTATE;	 
						mainMotAutoSet_Step = 75;		 
					}
					else
					{
						errK= ERR_TO_OVER;	 
						mainMotAutoSet_Step = 32;		 
					}
				}
				
				 
				else if( (PULSE_OK_SPEC_CNT*2) < _chgCnt )
				{
					lastPulseWidth = pulseWidthMs;
					okTime = getTO(_to);
					 
					 
					if(0 > okTime)
					{
						errK = ERR_FW_CAL_NG;  
						setTO(_to);
						mainMotAutoSet_Step = 75;		 
					}
					else
					{
						if(900 > okTime)
						{
							_out = 10 + (0.0667*okTime);
							 
						}
						else
						{
							_out = 53 + ( 0.0175*okTime);
						}

						if(MAIN_MOT_DEFAULT_MIN_DUTY > _out)
						{
							 
							minOut = MAIN_MOT_DEFAULT_MIN_DUTY;
						}
						else if (MAIN_MOT_DEFAULT_MAX_DUTY < _out)
						{
							minOut = MAIN_MOT_DEFAULT_MAX_DUTY;
						}
						else
						{
							minOut = _out;
						}				
						errK= ERR_OK;	 
						setTO(_to);
						mainMotAutoSet_Step = 32;		 
					}
				}
				break;

			
			 
			case 32:
				torqOff();
				bbnMotor.onBldc(BBN_MOT_CCW, (DEVCHK_BBN_WIRE_REL_DUTY+0) );
				setTO(_to);
				mainMotAutoSet_Step = 33;
				break;

			 
			case 33:
				if(checkTO(_to, MAINMOT_MIN_AUTOSET_BREAK_TO) )	 
				{
					bbnMotor.offBldc();
					setTO(_to);
					mainMotAutoSet_Step = 34;	 
				}
				break;

			 
			case 34:
				if(checkTO(_to, 200) )	 
				{

					if(ERR_OK == errK)
					{
						setTO(_to);
						mainMotAutoSet_Step = 75; 	 
					}
					else
					{
						setTO(_to);
						mainMotAutoSet_Step = 20;	 
					}
				}
				break;

			 
			case 50:
				break;


			 
			case 75:
				 
				torqOff();
				bbnMotor.offBldc();
			 
				 
			 
			
				if(ERR_OK == errK)
				{
					 
					result = fsInfo.saveInfo(MAINMOT_INFO, String(minOut));
					if(0 < result)	 
					{
						defaultTorqueMotor = minOut;
						
						 
						msg = STX_INFO_REQ + MAINMOT_INFO_READ;
						msg += String(defaultTorqueMotor);
						msg += "%";
						Resp2ApPrintln(msg);
						
						mainMotAutoSet_Step = 90;  
					}
					else	 
					{
						errK = ERR_FS_WRITE;		 
						mainMotAutoSet_Step = 80;	 
					}
				}
				else
				{
					 
					mainMotAutoSet_Step = 80;  
				}

				 
				Resp2ApPrintln( STX_DEV_CHK_ERR_LOG + MAIN_MMOT_AUTOSET_ERR + (ERR_OK==errK? "OK":"NG")
					+ ",try:" + String(_retryCnt) + ",Out:" + String(_out)  + ",Save:" + String(minOut) 
					+",cnt:" + String(_chgCnt) +",ms:" + String(okTime) +",plsWid:" + String(lastPulseWidth));
				
				
				break;
				
			 
			case 80:
				mainMotAutoSet_Flag = 0;	 
				mainMotAutoSet_Step = 0;
				 

				 
				msg = STX_DEV_SET_REQ + STR_MAINMOT_MINSET_REQ + "1" + String(errK);	 
				msg += "%";
				Resp2ApPrintln(msg);
				
				 
				msg = STX_DEV_CONT_REQ + MAIN_MMOT_AUTOSET_ERR;
				msg += "%";
				Resp2ApPrintln(msg);
			
				
				break;

				 
			case 90:
				mainMotAutoSet_Flag = 0;	 
				mainMotAutoSet_Step = 0;	 

				 
				msg = STX_DEV_SET_REQ + STR_MAINMOT_MINSET_REQ;
				msg += "1";		 
				msg += String(defaultTorqueMotor);
				msg += "%";
				Resp2ApPrintln(msg);

				 
				msg = STX_DEV_CONT_REQ + MAIN_DEV_CHK_OK;
				msg += "%";
				Resp2ApPrintln(msg);
				
		
				break;

			default:
				break;
	}
}


 
 

void stbyTorqOffControl_Start()
{
	 
	if(devReadychkFlag)
	{
		stbyTorqOffControl_Req_Flag = 1;	 
	}
	else
	{
		stbyTorqOffControl_Req_Flag = 0;
		stbyTorqOffControl_Flag = 1;		 
		stbyTorqOffControlStep = STEP_START;
	}
}

 
void stbyTorqOffControl_Stop(int flag)
{
	int outTorq=0;
	 
	{
		stbyTorqOffControl_Flag = 0;		 
		stbyTorqOffControlStep = STEP_IDLE;

		if (flag)
		{
			if(recv_TorqMotor_Flag)
			{
				torqMotor.setValue(reqTorqueMotor);	 
				outTorq = reqTorqueMotor;
			}
			else
			{
				torqMotor.setValue(defaultTorqueMotor);  
				outTorq = defaultTorqueMotor;
			}
		}
		bbnMotor.offBldc();						 
		 
		 
		send2resp_FwSelfControlCmd(STX_TORQUE_MOTOR, outTorq);		 
		 
		Resp2ApPrintln(STX_BBN_MOTOR + "00000000%");	 
	}
}

 
void stbyTorqOff_ReqCheck()
{
	if(stbyTorqOffControl_Req_Flag)
	{
		 

		 
		stbyTorqOffControl_Req_Flag = 0;
		stbyTorqOffControl_Start();	
	}
}

 
#define ENC_STOP_WAIT_TO		CHECK_TIME_1SEC_TO  
#define TENSION_HOLD_TIME_TO	7000  

#define STBY_OFF_CONT_SEN	LVL10_BBNENCA	 
#define STBY_OFF_CONT_ENCAB	LVL10_ENCAB		 

#define	STBY_BBN_MIN_DUTY		10
#define	BBN_MOVE_DETECT_CNT		10
 
 
 
 
 
 
 
 
 
 
 
 
void stbyTorqOff_Control()
{

	static unsigned long _to;
	static unsigned long _toErr;

	static unsigned short oldstbyTorqOffControlStep;
	static short errK = ERR_OK;

	static unsigned short _out;
	static unsigned short _oldSen;

	static unsigned short _retryCnt = 0;
	static unsigned short _chgCnt = 0;

	static unsigned short _senErrK = ENC_SEN_OK;

	int outTorq;
	
	String msg;
	String logMsg;

	switch(stbyTorqOffControlStep)
	{
		 
		case 0:
			break;
			
		 
		case 10:
			bbnMotor.offBldc();

			_retryCnt = 0;
			_chgCnt = 0;
			_oldSen = STBY_OFF_CONT_SEN;

			 
			setTO(_to);
			stbyTorqOffControlStep = 1;
			break;

			break;

		 
		case 1:
			if(checkTO(_to, 50))	 
			{
				setTO(_to);
				stbyTorqOffControlStep = 2;
			}
			break;

		 
		case 2:
			_chgCnt = 0;
			_oldSen = STBY_OFF_CONT_SEN;

			if(recv_TorqMotor_Flag)
				torqMotor.setValue(reqTorqueMotor);	 
			else
				torqMotor.setValue(defaultTorqueMotor);  
			setTO(_to);
			setTO(_toErr);
			stbyTorqOffControlStep = 3;
			break;

		case 3:
			 
			if( checkTO(_toErr, CHECK_TIME_10SEC_TO) )
			{
				 
				errK = ERR_WIRE_BROKEN;		 
				setTO(_to);
				stbyTorqOffControlStep = 75;
			}
			 
			else if( checkTO(_to, CHECK_TIME_1SEC_TO) )	 
			{
				_retryCnt = 0;
				setTO(_to);
				stbyTorqOffControlStep = 11;		 
			}
			else if(STBY_OFF_CONT_SEN != _oldSen)
			{
				_oldSen = STBY_OFF_CONT_SEN;
				 
				_chgCnt++;
				setTO(_to);
			}
			break;


			
		 
		case 11:
			_retryCnt++;
			if(12 < _retryCnt)
			{
				torqOff();
				setTO(_to);
				stbyTorqOffControlStep = 20;
			}
			else
			{
				 
				if(recv_TorqMotor_Flag)
					torqMotor.setValue( reqTorqueMotor + 0 - (_retryCnt*2) );					 
				else
					torqMotor.setValue( defaultTorqueMotor + 0 - (_retryCnt*2) );					 
				setTO(_to);
				stbyTorqOffControlStep = 12;
			}
			break;

			 
		case 12:
			if(checkTO(_to, 100))
			{
				setTO(_to);
				stbyTorqOffControlStep = 11;
			}
			break;


		 
		case 20:
			_chgCnt = 0;
			_oldSen = STBY_OFF_CONT_SEN;	 

			setTO(_to);
			setTO(_toErr);
			stbyTorqOffControlStep = 21;
			break;
			
		 
		case 21:
			 
			if(checkTO(_toErr, CHECK_TIME_20SEC_TO))
			{
				 

				errK = ERR_ENC_NOT_STABLE;
				 
				setTO(_to);
				stbyTorqOffControlStep = 75;	 
			}
			else
			{
				 
				 
				if(checkTO(_to, ENC_STOP_WAIT_TO))		 
				{
					 
					_chgCnt = 0;
					_oldSen = STBY_OFF_CONT_SEN;
					 
					setTO(_to);
					stbyTorqOffControlStep = 22;
				}
				 
				else if(STBY_OFF_CONT_SEN != _oldSen)	 
				{
					_oldSen = STBY_OFF_CONT_SEN;	 
					_chgCnt++;		 
					setTO(_to);		 
				}
			}
			break;
			
		 
		case 22:
			 
			if(STBY_OFF_CONT_SEN != _oldSen)	 
			{
				_oldSen = STBY_OFF_CONT_SEN;	 
				_chgCnt++;		 
			}

			 
			if( (4 < _chgCnt) && checkTO(_to, CHECK_TIME_1SEC_TO))
			{
				setTO(_to);
				stbyTorqOffControlStep = 30;
			}
			break;

		 
		case 30:
			_chgCnt = 0;
			if(recv_TorqMotor_Flag)
			{
				torqMotor.setValue(reqTorqueMotor);
				outTorq =reqTorqueMotor;
			}
			else
			{
				torqMotor.setValue(defaultTorqueMotor);
				outTorq = defaultTorqueMotor;
			}
			 
			send2resp_FwSelfControlCmd(STX_TORQUE_MOTOR, outTorq);
		
			setTO(_to);
			stbyTorqOffControlStep = 31;
			break;

			
		 
		case 31:
			 
			if(checkTO(_to, TENSION_HOLD_TIME_TO))
			{
				 
				_chgCnt = 0;
				_oldSen = STBY_OFF_CONT_SEN;
				 
				setTO(_to);
				stbyTorqOffControlStep = 32;
			}
			 
			else if(STBY_OFF_CONT_SEN != _oldSen)	 
			{
				_oldSen = STBY_OFF_CONT_SEN;	 
				_chgCnt++;		 
				setTO(_to);		 
			}
			break;

		case 32:
			 
			bbnMotor.offBldc();
		
			 
			_retryCnt = 0;
			stbyTorqOffControlStep = 11; 	 
			break;

		 
		case 40: 
			break;

		 
		case 60:
			break;

		 
		case 75:
			torqOff();		 
			bbnMotor.offBldc();

			 
			if(ERR_OK == errK)
			{
				stbyTorqOffControlStep = 90;	 
			}
			else
			{
				stbyTorqOffControlStep = 80;	 
			}
			break;

		 
		case 80:
			stbyTorqOffControl_Flag = 0;	 
			stbyTorqOffControlStep = 0;
	
			 
			if(ERR_WIRE_BROKEN == errK)
			{
				msg = STX_DEV_CONT_REQ + MAIN_WIRE_BROKEN_ERR;
				logMsg = MAIN_WIRE_BROKEN_ERR;
			}
			else
			{
				msg = STX_DEV_CONT_REQ + MAIN_WIRE_NOTSTABLE_ERR;
				logMsg = MAIN_WIRE_NOTSTABLE_ERR;
			}
			msg += "%";
			Resp2ApPrintln(msg);
	
				
			break;
	
		 
		case 90:
			stbyTorqOffControl_Flag = 0;	 
			stbyTorqOffControlStep = 0;		 
	
	
			break;

		default:
			break;
		
	}

}

 
typedef struct _stAgingPara
{
	unsigned short cw_duty1;
	unsigned short bldc_duty1;		 
	unsigned long cw_time1;
	unsigned short cw_duty2;
	unsigned short bldc_duty2;	
	unsigned long cw_time2;
} stAgingPara;
typedef stAgingPara* ptrstAgingPara;

stAgingPara wireAging_Mmot =
{
	100, 20, 2000,			 
	 70,  0, 1000,			 
};


 
static String strData[10] =
{
	"",
};

 
 
void wireAging_Test_Start(String msg)
{
	unsigned short length;

	unsigned short i = 0;

	 
	 
	length = splitTextToStrings(msg.substring(7), strData);

	for(i=0; i<length; i++)
	{
		if(0==i) wireAging_Mmot.cw_duty1 = strData[i].toInt();
		if(1==i) wireAging_Mmot.bldc_duty1 = strData[i].toInt();
		if(2==i) wireAging_Mmot.cw_time1 = strData[i].toInt();
		if(3==i) wireAging_Mmot.cw_duty2 = strData[i].toInt();
		if(4==i) wireAging_Mmot.bldc_duty2 = strData[i].toInt();
		if(5==i) wireAging_Mmot.cw_time2 = strData[i].toInt();
	}


	 
	wireAgingTest_Flag = 1;
	wireAgingTest_Step = 10;
	
}

unsigned short splitTextToStrings(String msg, String *s)
{
	unsigned short  idx = 0;
	int startIdx = 0;
	int commaIdx = msg.indexOf(',');

	while (commaIdx != -1) {
		s[idx++] = msg.substring(startIdx, commaIdx);
		startIdx = commaIdx + 1;
		commaIdx = msg.indexOf(',', startIdx);
	}

	 
	s[idx++] = msg.substring(startIdx);

	return idx;
}

 
static unsigned long agingPulseWidth = 9999;

void wireAging_Test_Stop(String msg)
{
	wireAgingTest_Flag = 0;
	wireAgingTest_Step = 0;
	torqOff();
}

 
void wireAging_Test_Control()
{
			static unsigned long _to;
		
			static unsigned long _toSenChg;
		
			static unsigned short oldwireAgingTest_Step;
			static short errK;
		
			static int _out;
			static unsigned short _oldSen;
		
			static unsigned short _retryCnt;
			static unsigned short _chgCnt;
	
			static unsigned short speed_high;	 
	
			static int _deltaOut;
			
			short result;
			
			String msg;
		
		
			
			switch(wireAgingTest_Step)
			{
		
				 
				case 0:
					break;
				
				 
				case 10:
					torqOff();					 
					bbnMotor.offBldc();
	
					_retryCnt = 0;
					_chgCnt = 0;
					 
					 
					errK = ERR_OK;
					 
					 
	
					setTO(_to);
					wireAgingTest_Step = 11;
					break;
				
				 
				case 11:
					if(checkTO(_to, 50))	 
					{
						setTO(_to);
						wireAgingTest_Step = 20;
					}
					break;
	
	
				 
				case 20:
					_retryCnt++;		 
					 
					{
						 
						_oldSen = LVL01_BBNENCA;	 
						_chgCnt = 0;
						torqMotor.setValue(wireAging_Mmot.cw_duty1);	 
						bbnMotor.onBldc(BBN_MOT_CW, wireAging_Mmot.bldc_duty1);	 
						setTO(_to);
						wireAgingTest_Step = 21;
					}
					break;
					
				case 21:
					if( checkTO(_to, wireAging_Mmot.cw_time1) )  
					{
						_oldSen = LVL01_BBNENCA;	 
						_chgCnt = 0;
						torqMotor.setValue(wireAging_Mmot.cw_duty2);	 
						bbnMotor.onBldc(BBN_MOT_CW, wireAging_Mmot.bldc_duty2);	 
						setTO(_to);
						wireAgingTest_Step = 22;	 
					}
					 
					else if( (2 > _chgCnt) && checkTO(_to,(wireAging_Mmot.cw_time1 - 50)) )	 
					{
						 
						errK = ERR_NG;
						setTO(_to);
						wireAgingTest_Step = 75;	 
					}
					 
					 
					 
					 
					else if(LVL01_BBNENCA != _oldSen)	 
					{
						_oldSen = LVL01_BBNENCA;	 
						_chgCnt++;
					}
					break;
					
				case 22:
					if(checkTO(_to, wireAging_Mmot.cw_time2) )  
					{
						setTO(_to);
						wireAgingTest_Step = 23;	 
					}
					 
					else if( (2 > _chgCnt) && checkTO(_to,(wireAging_Mmot.cw_time2-50)) )	 
					{
						 
						errK = ERR_NG;
						setTO(_to);
						wireAgingTest_Step = 75;	 
					}
					 
					 
					 
					 
					else if(LVL01_BBNENCA != _oldSen)	 
					{
						_oldSen = LVL01_BBNENCA;	 
						_chgCnt++;
					}
					break;
					
				case 23:
					 
					msg = STX_DEV_FUNCTION_REQ + STR_WIRE_AGING_TEST;
					msg += "9+1";
					msg += "%";
					Resp2ApPrintln(msg);

					setTO(_to);
					wireAgingTest_Step = 20;	 
					break;


				 
				
				 
				case 30:
					 
					_chgCnt = 0;
					_oldSen = LVL01_BBNENCA;
					pulseWidthMs = 9999;				
				 
					 
				 
					torqMotor.setValue(_out);	 
					setTO(_to);
					wireAgingTest_Step = 31;
					break;
	
				case 31:
					 
					if(6 < _chgCnt)
					{
						 
						portENTER_CRITICAL(&mux);   
							pulseWidthMs = iEncIntrIntervalTime;	 
						portEXIT_CRITICAL(&mux);    
					}
	
					 
					 
					 
					if( !speed_high && (6 < _chgCnt) && (MAINENCA_SPEC_PERIOD_SP_LOW > pulseWidthMs) )	 
					{
						speed_high = 1;
						 
						_out--; 		 
						setTO(_to);
						wireAgingTest_Step = 32;			
	
					}
	
					else if( (speed_high) && PULSE_RANGE_OK && checkTO(_to, MAINMOT_MIN_AUTOSET_TO_2S) )
					{
						 
						{
							speed_high = 0;
							 
							if( 59 < _out && 81 > _out)  
							{
								errK = ERR_OK;		 
								 
							}
							else
							{
								errK = ERR_RANGE_OVER;  
							}
							setTO(_to);
							wireAgingTest_Step = 75;					
						}

					}
	
					 
					 
					else if( !PULSE_RANGE_OK && checkTO(_to, MAINMOT_MIN_AUTOSET_TO_3S) )  
					{
						 
						if(6 > _chgCnt)  
						{
							errK= ERR_FAIL;
							setTO(_to);
							wireAgingTest_Step = 75;
						}
						else
						{
							 
							_deltaOut = ( (pulseWidthMs - MAINENCA_SPEC_PERIOD_SP_MID)*10/15 );
							if(0 == _deltaOut)
							{
								if(MAINENCA_SPEC_PERIOD_SP_MID < lastPulseWidth)	 
									_deltaOut = 1;
								else
									_deltaOut = -1;
							}
							_out += _deltaOut;

							setTO(_to);
							wireAgingTest_Step = 32;					
						}
					}
	
			
					 
					else if(LVL01_BBNENCA != _oldSen)	 
					{
						_oldSen = LVL01_BBNENCA;	 
						_chgCnt++;
					}
					break;
	
	
				 
				case 50:
					break;
	
	
				 
				case 75:
					 
					agingPulseWidth = pulseWidthMs;
					torqOff();
					bbnMotor.offBldc();
				 
					 
				 
				
					if(ERR_OK == errK)
					{
						wireAgingTest_Step = 90;  
					}
					else
					{
						 
						wireAgingTest_Step = 80;  
					}
					break;
					
				 
				case 80:
					wireAgingTest_Flag = 0;	 
					wireAgingTest_Step = 0;
					 
	
					 
					msg = STX_DEV_FUNCTION_REQ + STR_WIRE_AGING_TEST;
					msg += "1" + String(errK);		 
					msg += "%";
					Resp2ApPrintln(msg);
					
					
					break;

				 
				case 90:
	
					wireAgingTest_Flag = 0;	 
					wireAgingTest_Step = 0;	 
	
					 
					msg = STX_DEV_FUNCTION_REQ + STR_WIRE_AGING_TEST;
					msg += STR_ERR_OK;
					msg += "%";
					Resp2ApPrintln(msg);
	
			
					break;
				
				default:
					break;
					
	
			}

}
 


 
 
 
 
 
 
 
 
static volatile short bdLed2_out = 0;
 
void boardLed_Control()
{


	 
	if(DIAG_MODE != mainMode)
	{
		if(PROG_END == gameStatus)		 
		{
			bdLed2_out = LOW;
			digitalWrite(BD_LED2_PIN, bdLed2_out); 	 
		}
		else
		{

			 
			if (CONNECT == ap_conn_status)	   
			{
				if(ROD_CONN == rod_conn_status)
				{
					 
					bdLed2_out = !bdLed2_out;
					digitalWrite(BD_LED2_PIN, bdLed2_out);	   
				}
				else
				{
					 
					if(!(count_500ms % 5))	 
					{
						bdLed2_out = !bdLed2_out;
						digitalWrite(BD_LED2_PIN, bdLed2_out);	   
					}
				}
			}
			 
			else
			{
				if(ROD_CONN == rod_conn_status)
				{
					bdLed2_out = HIGH;
					digitalWrite(BD_LED2_PIN, bdLed2_out); 	 
				}
				else
				{
					bdLed2_out = LOW;
					digitalWrite(BD_LED2_PIN, bdLed2_out);		 
				}
			}
		}
	}

}


 
 
 
 
 
void read_input_1ms()
{
	 
	{
	 
	
	sensor1ms[SENSOR_1].curr =  (digitalRead(BBN_ENC_B_DT_PIN) << SNS_bbnEncB) |
								(digitalRead(BBN_ENC_A_CK_PIN) << SNS_bbnEncA) |
								(digitalRead(BLDC_FG_PIN) << SNS_bbnMotFg);
	if(DF_CONFIG_LMJIG == dfConfig)
	{
		sensor1ms[SENSOR_1].curr |= (digitalRead(LM_HOME_SEN_PIN) << SNS_LM_HOME) |
									(digitalRead(LM_LEFT_SEN_PIN) << SNS_LM_LEFT) |
									(digitalRead(LM_RIGHT_SEN_PIN) << SNS_LM_RIGHT) |
									(digitalRead(LM_ENC_SEN_PIN) << SNS_LM_ENC) |
									(digitalRead(LM_MOT_FG_PIN) << SNS_LMMOT_FG);
	}
	 
	 
	 

	 
	sensor1ms[SENSOR_1].lvl = ( (sensor1ms[SENSOR_1].lvl & sensor1ms[SENSOR_1].old) | (sensor1ms[SENSOR_1].curr & (sensor1ms[SENSOR_1].lvl ^ sensor1ms[SENSOR_1].old)));		 
	 
	sensor1ms[SENSOR_1].le = 0;
	sensor1ms[SENSOR_1].te = 0;
	
	sensor1ms[SENSOR_1].le = ((~sensor1ms[SENSOR_1].old2) & sensor1ms[SENSOR_1].old & sensor1ms[SENSOR_1].curr);
	sensor1ms[SENSOR_1].te = ( sensor1ms[SENSOR_1].old2 & (~sensor1ms[SENSOR_1].old) & (~sensor1ms[SENSOR_1].curr) );
	}

	 
	sensor1ms[SENSOR_1].old2 = sensor1ms[SENSOR_1].old;
	sensor1ms[SENSOR_1].old  = sensor1ms[SENSOR_1].curr;

}

 
 
 
 

#define FREQ_CHANGE_BIT_MASK 0x5F1F	  

static int lc10msCnt = 0;
 
 
 
void read_input_10ms()
{
	 
	{
	 
	sensor10ms[SENSOR_1].curr  = sensor1ms[SENSOR_1].lvl |
								(digitalRead(PWROFF_SWC_PIN) << SNS_PwrOffSwc) |	 
								(digitalRead(PC_USB5V_PIN) << SNS_usb5V) |
								(digitalRead(BOOT_MODE_PIN) << SNS_boot) |			 
								(digitalRead(SUB_ACOFF_PIN) << SNS_AcOff);			 
	if(DF_CONFIG_LMJIG == dfConfig)
	{
		sensor10ms[SENSOR_1].curr |= (digitalRead(LM_HOME_SEN_PIN) << SNS_LM_HOME) |
									 (digitalRead(LM_LEFT_SEN_PIN) << SNS_LM_LEFT) |
									 (digitalRead(LM_RIGHT_SEN_PIN) << SNS_LM_RIGHT) |
									 (digitalRead(LM_ENC_SEN_PIN) << SNS_LM_ENC) |
									 (digitalRead(LM_MOT_FG_PIN) << SNS_LMMOT_FG);
	}


	 
	sensor10ms[SENSOR_1].lvl = ( (sensor10ms[SENSOR_1].lvl & sensor10ms[SENSOR_1].old) | (sensor10ms[SENSOR_1].curr & (sensor10ms[SENSOR_1].lvl ^ sensor10ms[SENSOR_1].old)));		 

	 
	sensor10ms[SENSOR_1].le = 0;
	sensor10ms[SENSOR_1].te = 0;
	
	sensor10ms[SENSOR_1].le = ( (~sensor10ms[SENSOR_1].old2) & sensor10ms[SENSOR_1].old & sensor10ms[SENSOR_1].curr );
	sensor10ms[SENSOR_1].te = ( sensor10ms[SENSOR_1].old2 & (~sensor10ms[SENSOR_1].old) & (~sensor10ms[SENSOR_1].curr) );
	}


	 
	sensor10ms[SENSOR_1].old2 = sensor10ms[SENSOR_1].old;
	sensor10ms[SENSOR_1].old  = sensor10ms[SENSOR_1].curr;



}

void read_input_100ms()
{
	 
}



 
 
 
 

#define eepromLogPrintln	Resp2ApPrintln
 
 
 
void eeprom_test()
{

	unsigned int reqlen = 6;
	 






		unsigned int len;
	
		byte readTempBuff[128];
		byte writeTempBuff[128];
	
		 
		eNvm.readSend(0, reqlen);
		delay(100);
	
		len = eNvm.read(&readTempBuff[0], 0, reqlen);
		eNvm.grazingGeddong(&readTempBuff[0], 0, reqlen);
		delay(100);
	
		 
		writeTempBuff[0]=readTempBuff[0]+1;   
		writeTempBuff[1]=readTempBuff[1]+1;   
		writeTempBuff[2]=readTempBuff[2]+1;
		writeTempBuff[3]=readTempBuff[3]+1;
		writeTempBuff[4]=0x89;
		eNvm.write(&writeTempBuff[0], 0, 5);
		delay(100);

		 
		eNvm.readSend(0, reqlen);
		delay(100);
		len = eNvm.read(&readTempBuff[0], 0, reqlen);
		eNvm.grazingGeddong(&readTempBuff[0], 0, reqlen);
		
	 


















































	

}

 
 
 
void subAc_Control_atReset()
{
	LogPrintln(" LG] PWRAC SUB_AC On");
	
	if(LVL10_PWROFF_SWC)		 
	{
		 

	 
		 
	}
	else
	{
		subAC_On();
		motor_AllOff();		 
		ledOn_MbBtm();
		reelOut_AllOff();	 
	}
}

 
#define CONFIG_AD_MAX	4096	 
#define CONFIG_AD_EA	2  
#define CONFIG_AD_DIVS  (CONFIG_AD_MAX/CONFIG_AD_EA)
#define CONFIG_AD_GAP	(CONFIG_AD_DIVS/2)

#define CONF_READ_CNT	5
 
 
 
short setConfig()
{
	short i;
	short conf = 0;
	short rts;
	short confAvg = -1;
	int dummy;
	int confAd[CONF_READ_CNT];

	 
	pinMode(DF_CONFIG_PIN, INPUT_PULLDOWN);	 

 
	 
	delay(50);
	 
	 
	 
	 
	for(i=0; i<CONF_READ_CNT; i++)
	{
		dummy= analogRead(DF_CONFIG_PIN);
		delay(10);
	}
	 
	for(i=0; i<CONF_READ_CNT; i++)
	{
		confAd[i] = analogRead(DF_CONFIG_PIN);
		conf += confAd[i];
		delay(10);
	}
	confAvg = conf/CONF_READ_CNT;

	 
	
	 
	 
	if(0) {}
	else if( (CONFIG_AD_DIVS*(CONFIG_AD_EA-1)) + CONFIG_AD_GAP < conf) { rts = DF_CONFIG_LMJIG; }
	 
	 
	 
	else			{ rts = DF_CONFIG_STAND_ALONE; }	

	return rts;
}

 
 
 
void ioLedOffSetting()
{
 
	 
	pinMode(LED_CENT_R_PIN, OUTPUT);
	pinMode(LED_CENT_G_PIN, OUTPUT);
	pinMode(LED_CENT_B_PIN, OUTPUT);

	 
	pinMode(LED_BTM_PIN, OUTPUT);

	extLed.init();
	
	 
		 
		 
  

}

void ioPinSetting()
{

	 
	 

	 
	


	pinMode(PC_USB5V_PIN, INPUT_PULLUP);
	pinMode(BLDC_FG_PIN, INPUT_PULLUP);
	 
	 
	pinMode(PWROFF_SWC_PIN, INPUT);			 

	pinMode(BBN_ENC_A_CK_PIN, INPUT_PULLUP);
	pinMode(BBN_ENC_B_DT_PIN, INPUT_PULLUP);

	 
	 

	  
	 
	 

	 

	 
	 

	 
		
		 
		if(DF_CONFIG_LMJIG != dfConfig)
		{
			pinMode(WDT_OUT_PIN, OUTPUT);
			digitalWrite(WDT_OUT_PIN, HIGH);	 
			digitalWrite(WDT_OUT_PIN, LOW); 	 
			digitalWrite(WDT_OUT_PIN, HIGH);	 
		}
	

	 
	pinMode(BD_LED2_PIN, OUTPUT);	   
	
	pinMode(BD_LED3_PIN, OUTPUT);		 
	pinMode(BD_LED4_PIN, OUTPUT);		 

	if(DF_CONFIG_LMJIG == dfConfig)
	{
		pinMode(LM_HOME_SEN_PIN, INPUT_PULLUP);
		pinMode(LM_LEFT_SEN_PIN, INPUT_PULLUP);
		pinMode(LM_RIGHT_SEN_PIN, INPUT_PULLUP);
		pinMode(LM_ENC_SEN_PIN, INPUT_PULLUP);
		pinMode(LM_MOT_FG_PIN, INPUT_PULLUP);
		pinMode(LM_24VON_PIN, OUTPUT);
		pinMode(LM_MOT_PWM_PIN, OUTPUT);
		pinMode(LM_MOT_ON_PIN, OUTPUT);
		pinMode(LM_MOT_DIR_PIN, OUTPUT);
	}

	 
	pinMode(TORQ_SS, OUTPUT);				    

	 

	 
	pinMode(BBN_MOT_ON_PIN, OUTPUT);		 
	pinMode(BBN_MOT_DIR_PIN, OUTPUT);
	pinMode(BBN_MOT_PWM_PIN, OUTPUT);
	
	pinMode(BBN_BLDC_24VON_PIN, OUTPUT);
	digitalWrite(BBN_BLDC_24VON_PIN, BBN_BLDC_24VOFF);	 

 
	 
	pinMode(LED_CENT_R_PIN, OUTPUT);
	pinMode(LED_CENT_G_PIN, OUTPUT);
	pinMode(LED_CENT_B_PIN, OUTPUT);

	 
	pinMode(LED_BTM_PIN, OUTPUT);
	
	 
		 
		 
  

}

 
 
 


 
 
 

void wat_PidParaSet(String msg)
{
	 
	int iPara = msg.substring(5,(5+1)).toInt();  
	double dPara = msg.substring(6).toDouble();
	switch(iPara)
	{
		case 0:  
			LogPrintln(" lg] PidPr:" + String(Kp)+","+String(Ki)+","+String(Kd));
			break;
		default:
			break;
	}

}

void wat_HookRandCheckExec()
{
	portENTER_CRITICAL(&mux);   
		bHookRandingCheckOnce = true;
	portEXIT_CRITICAL(&mux);    
}

void wat_PidStart_TargetSet(short _targetDist)
{
	portENTER_CRITICAL(&mux);   
		iTargetDist = (_targetDist);	 
		pidExecFlag = 1;
		pidExecOutLog = 1;
	portEXIT_CRITICAL(&mux);    
}


void wat_PidStop()
{	
	portENTER_CRITICAL(&mux);   
		pidExecFlag = 0;
		iWireDistance = 0;		 
		sMotorSpeed = 0;
	portEXIT_CRITICAL(&mux);    

	int iOut;
	 
	
}



 
 
 
void IRAM_ATTR ISR_onTimerHandler() {
   
   
   
   

  portENTER_CRITICAL_ISR(&mux);   
  
	lSys100UsTime++;		 
	
	sSys100UsCnt++;			 
	if((10-1) < sSys100UsCnt)
	{
  		sSys100UsCnt = 0;
		
		lSys1MsTime++;          
	}
  portEXIT_CRITICAL_ISR(&mux);    
}


#define INTERVAL_2MS_1MS	(2 * 1)
#define INTERVAL_2MS_100US	(2 * 1000/100)
 
 
 
 
 
void IRAM_ATTR ISR_encIntrHandle()
{
	unsigned long _lSys1MsTimer;
	 
	portENTER_CRITICAL_ISR(&mux);   
		 
    	 
    	_lSys1MsTimer = lSys1MsTime;
			lCurrentTime = lSys100UsTime;	 
	portEXIT_CRITICAL_ISR(&mux);    
	 
	 

	portENTER_CRITICAL_ISR(&mux);
	 
	if (lCurrentTime < lLastTime) {   
	  iEncIntrIntervalTime = ((ULONG_MAX - lLastTime) + lCurrentTime + 1);
	} else {
	  iEncIntrIntervalTime = (lCurrentTime - lLastTime);   
	}

 
	 
	if(INTERVAL_2MS_100US > iEncIntrIntervalTime)	 
	{
		portEXIT_CRITICAL_ISR(&mux);
		return;		 
	}
	 
	lLastTime = lCurrentTime;   

	 
	 
	sIsrEncBdata = (digitalRead(BBN_ENC_B_DT_PIN)? 1:0);
	if(!sIsrEncBdata)
	{
		iIsrbbnEncCnt--;
		if(10 > iIsrbbnEncCnt) { iIsrbbnEncCnt = 10; }	 
		sIsrBbnEncDir = BBN_ENC_CW;
	}
	 
	else
	{
		iIsrbbnEncCnt++;
		sIsrBbnEncDir = BBN_ENC_CCW;
	}
	
	 
	 
	if(0 > iIsrbbnEncCnt) { iIsrbbnEncCnt = 1; }	 
	iWireDistance = (iIsrbbnEncCnt*BBN_ENC_1PULSE_DIST)/1000;

	 
	 
	stEncIntvBuff[sEncBuffIdx].tick = _lSys1MsTimer;			 
	stEncIntvBuff[sEncBuffIdx].width = iEncIntrIntervalTime;	 
	stEncIntvBuff[sEncBuffIdx].dir = sIsrBbnEncDir;
	stEncIntvBuff[sEncBuffIdx].enc = iIsrbbnEncCnt;
	stEncIntvBuff[sEncBuffIdx].dist = iWireDistance;
	sEncBuffIdx++;
	sEncBuffIdx = (sEncBuffIdx & (ENC_INTV_BUFF_SIZE-1));

	 
		bIsrEncInterruptFlag = true;
		iIsrEncInterruptCnt++;
	portEXIT_CRITICAL_ISR(&mux);    



 

}

 
void spi_out(int _pin, int _val)
{
	digitalWrite(_pin, LOW);
	SPI.transfer(B00010001);  		 
	SPI.transfer(_val);       		 
	digitalWrite(_pin, HIGH);
}

static unsigned int system_delay_cnt = 0;
 





void system_counter()
{
	unsigned long add_tick = 0;

	 
	portENTER_CRITICAL(&mux);   
		curr_ms_tick = lSys1MsTime;	    
	portEXIT_CRITICAL(&mux);    
	
	if(0)
	{
	}
 









	
	 
	else
	{
		 
		 
		if(curr_ms_tick != old_ms_tick)
		{
			if( curr_ms_tick > old_ms_tick )
			{
				add_tick = curr_ms_tick - old_ms_tick;
			}
			else
			{
				add_tick = (0xFFFFFFFF - old_ms_tick) + curr_ms_tick + 1;
			   











			}
			sys_count_1ms += add_tick;
			count_1ms += add_tick;
			
gotoSysTimer:
			if( 9 < count_1ms )		 
			{
				count_1ms -= 10;
				
				sys_count_10ms++;
				count_10ms++;
				if( 9 < count_10ms)		 
				{
					count_10ms -= 10;
					
					sys_count_100ms++;
					count_100ms++;
 					if( 9 < count_100ms) 	 
					{
						count_100ms -= 10;
						
						sys_count_1sec++;
						count_1sec++;
						if( 9 < count_1sec) 	 
						{
							count_1sec -= 10;
						}
 					}
						
					 
					if(!(count_100ms % 5)) 	 
					{
						sys_count_500ms++;

						count_500ms++;
						if( 9 < count_500ms )	 
						{
							count_500ms -= 10;
					
							sys_count_5sec++;
							 
						}
					}
					 
				}
			}

			if(9 < count_1ms)	 
			{
				goto gotoSysTimer;
			}
			
			 
			 
			 
			if(2 < add_tick)
			{
				 
				system_delay_cnt++;
				LogPrintln(" LG] SYSTK MN_DLY:" + String(add_tick) + " ms,cnt:" + String(system_delay_cnt));
			}
		  
			old_ms_tick = curr_ms_tick;
		}
	}
}

 
 
 
 
 
int getBoardType()
{


	return MAIN_BOARD_V10;


}



 



 


