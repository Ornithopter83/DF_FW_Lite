 
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



#define LM_WIRE_ADD_TARGET	5		 
static volatile short targetAddPulse = LM_WIRE_ADD_TARGET;		 
static short lmWire_Step = 0;



#define LM_WIRE_CONTROL_TO	(15*1000)



 


void t1ms_Process_Exec()
{

	 
	mainEnc.rotate();	 

	  
	 
	if(devReadychkFlag)   { devChkReady_Control(); }


	


	 

}

 







void t10ms_Process_Exec()
{


	 

 
	 

	 
				 
	

	 
	diagInputMonitor();

	 
	
	 
	if(diagOutflag_bbnTimeOut) { diagBbnMotor_TimeoutControl(); }
	 
	if(diagBbnflag_TokTokControl) { diagBbnMotor_TokTokControl(bbnActKind); }
	 
	if(exeFlag_bbnMotorStop) { bbnMotor_OffTime_Control(); }

	if(exeFlag_torqMotorStop) { torqMotor_OffTime_Control(); }

	 
	{
		check_PowerSwitch_Change_10ms();
	 
	}

	 
	extLed_Control();

	 
	usb5vChange_Check();

	 
	 

	 


	 


	 
		 
			 


}


 
 
 
void t100ms_Process_Exec()
{

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



static int minOut = 0;



#define MMOT_AUTOSET_OUT_DUTY	80	 



 
#define ENC_STOP_WAIT_TO		CHECK_TIME_1SEC_TO  
#define TENSION_HOLD_TIME_TO	7000  

#define STBY_OFF_CONT_SEN	LVL10_BBNENCA	 
#define STBY_OFF_CONT_ENCAB	LVL10_ENCAB		 

#define	STBY_BBN_MIN_DUTY		10
#define	BBN_MOVE_DETECT_CNT		10



 
void boardLed_Control()
{
    static int bdLed2_out = LOW;

    if (DIAG_MODE == mainMode)
    {
        return;
    }

    if (CONNECT == ap_conn_status)
    {
        if (ROD_CONN == rod_conn_status)
        {
            bdLed2_out = !bdLed2_out;
        }
        else if (!(count_500ms % 5))
        {
            bdLed2_out = !bdLed2_out;
        }
    }
    else
    {
        bdLed2_out = (ROD_CONN == rod_conn_status) ? HIGH : LOW;
    }

    digitalWrite(BD_LED2_PIN, bdLed2_out);
}


 
 
 
 
 
void read_input_1ms()
{
	 
	{
	 
	
	sensor1ms[SENSOR_1].curr =  (digitalRead(BBN_ENC_B_DT_PIN) << SNS_bbnEncB) |
								(digitalRead(BBN_ENC_A_CK_PIN) << SNS_bbnEncA) |
								(digitalRead(BLDC_FG_PIN) << SNS_bbnMotFg);

	 
	 
	 

	 
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

 

 
 
 
short setConfig()
{
	return DF_CONFIG_STAND_ALONE;
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

	 
	 

	  
	 
	 

	 

	 
	 

	 
		
		 


			pinMode(WDT_OUT_PIN, OUTPUT);
			digitalWrite(WDT_OUT_PIN, HIGH);	 
			digitalWrite(WDT_OUT_PIN, LOW); 	 
			digitalWrite(WDT_OUT_PIN, HIGH);	 
		

	

	 
	pinMode(BD_LED2_PIN, OUTPUT);	   
	
	pinMode(BD_LED3_PIN, OUTPUT);		 
	pinMode(BD_LED4_PIN, OUTPUT);		 



	 
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



 



 


