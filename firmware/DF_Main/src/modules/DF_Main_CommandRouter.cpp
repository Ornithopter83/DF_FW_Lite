
#include "../DF_Main_Internal.h"
#include "DF_Main_CommandRouter.h"

void analedContCmd(String msg)
{
	int lightReadFlag = 0;

	unsigned int pos;
	unsigned int act;
	unsigned int color;
	unsigned int time;

	unsigned int light;

  	pos = msg.substring(3,(3+1)).toInt();
	if(1> pos) { pos = 0; } if(4 < pos) { pos = 0; }  

  	act = msg.substring(4,(4+1)).toInt();
	if(0> act) { act = 10; } if(9 < act) { act = 10; }  

	switch(act)
	{
		case 0:		 
			ledCont[pos].cont = act;

			ledCont_OffStart(pos);
			break;

		case 1:		 
			ledCont[pos].cont = act;

			color = msg.substring(5,(5+2)).toInt();
			if(0> color) { color = 0; } if(14 < color) { color = 0; }  
			ledCont[pos].colorNo = color;

			ledCont_OnStart(pos);
			break;

		case 2:		 
			ledCont[pos].cont = act; 

			color = msg.substring(5,(5+2)).toInt();
			if(0> color) { color = 0; } if(14 < color) { color = 0; }  
			ledCont[pos].colorNo = color; 

			time = msg.substring(7,(7+4)).toInt();
			if(10> time) { time = 10; } if(9999 < time) { time = 9999; }  
			ledCont[pos].bTime = time; 

			ledCont_BlinkStart(pos);
			break;

		case 3:		 
			ledCont[pos].cont = act; 

			if(LED_IDX_BTM == pos)
			{
				ledCont[pos].colorMany = 1;		 
			}
			else
			{
				color = msg.substring(5,(5+2)).toInt();
				if(1> color) { color = 1; } if(14 < color) { color = 14; }  
				ledCont[pos].colorMany = color;	 
			}

			time = msg.substring(7,(7+4)).toInt();
			if(10> time) { time = 10; } if(9999 < time) { time = 9999; }  
			ledCont[pos].dTime = time; 
			ledCont_DimmStart(pos);
			break;

		case 7:			 
			ledColorOrder[pos][1] = msg.substring(5,(5+1)).toInt();
			ledColorOrder[pos][2] = msg.substring(6,(6+1)).toInt();
			ledColorOrder[pos][3] = msg.substring(7,(7+1)).toInt();
			ledColorOrder[pos][4] = msg.substring(8,(8+1)).toInt();
			ledColorOrder[pos][5] = msg.substring(9,(9+1)).toInt();
			ledColorOrder[pos][6] = msg.substring(10,(10+1)).toInt();
			ledColorOrder[pos][7] = msg.substring(11,(11+1)).toInt();
			ledColorOrder[pos][8] = msg.substring(12,(12+1)).toInt();
			ledColorOrder[pos][9] = msg.substring(13,(13+1)).toInt();

			break;

		case 8:			 
			lightReadFlag=1;
			break;

		case 9:			 
			light = msg.substring(5).toInt();
			if(  10 > light) { light = 10; }
			if(  100 < light) { light = 100; }
			ledLight = light;			 
			lightReadFlag=1;
			break;

		default:

			break;
	}

	if(lightReadFlag)
	{
		lightReadFlag = 0;
		light = ledLight;
		if(  10 > light) { light = 10; }
		if ( 100 < light) { light = 100; }
		Resp2ApPrintln("$060" + String(act) + String(light)+"%");
	}
	else
	{
	}

}

void sendRodVrtCmd(int act, unsigned int cnt, unsigned int ontime, unsigned int offtime)
{
	String msg = "0";
	int rts = 0;		 

	switch(act)
	{
		case 0:
		case 1:
			msg = String(act);
			break;

		case 2:
		case 3:
			rts = 1;
			break;		 

		case 4:
			msg = String(act);
			if(99 < cnt){ cnt = 1;}	 
			if(9 < cnt) { msg += String(cnt); }			 
			else		{ msg += "0" + String(cnt); }

			if(999 < ontime) { ontime = 100;}
			msg += fillZero2String3Char(ontime);
			if(999 < offtime) { offtime = 100;}
			msg += fillZero2String3Char(offtime);
			break;

		default:
			rts = 1;
			break;		 
	}

	if(rts)	 
	{

		LogPrintln(" lg] VrtMot CMD Error");
	}
	else
	{
		eNow.write(DF_Protocol_MainToRod_VerticalMotor,msg);
	}

}

void sendRodBtnLedCmd(int posi, int act, unsigned int cnt, unsigned int ontime, unsigned int offtime)
{
	String msg = "00";
	int rts = 0;		 

	if(2 < posi)	 
	{ 
		LogPrintln(" lg] BtnLED POSI NG:" + String(posi));
		return; 
	}	 

	msg = String(posi);
	msg += String(act);

	switch(act)
	{
		case 0:
		case 1:

			break;

		case 2:
		case 3:
			rts = 1;
			break;		 

		case 4:
			if(99 < cnt){ cnt = 5;}			 
			if(9 < cnt) { msg += String(cnt); }			 
			else		{ msg += "0" + String(cnt); }

			if(999 < ontime) { ontime = 100;}
			msg += fillZero2String3Char(ontime);
			if(999 < offtime) { offtime= 100;}
			msg += fillZero2String3Char(offtime);
			break;

		default:
			rts = 1;
			break;		 
	}

	if(rts)	 
	{

		LogPrintln(" lg] BtnLED CMD Error");
	}
	else
	{
		eNow.write(DF_Protocol_MainToRod_ButtonLed,msg);
	}

}

void anaDeviceInfoReq(String msg)
{

	String respMsg = "";

	String strKind = msg.substring(3,(3+2));

	int iKind = strKind.toInt();

	switch(iKind)
	{

		case NUM_MAIN_IF_VER:
			respMsg = STX_INFO_REQ + MAIN_IF_VER + IfVer;
			respMsg += "%";
			Resp2ApPrintln(respMsg);

			LogPrintln(" LG] InfRq IF_Ver: " + IfVer);
			break;

		case NUM_ROD_BOARD_TYPE:
			respMsg = STX_INFO_REQ + ROD_BOARD_TYPE;
			respMsg += strRodBoardType;		 
			respMsg += "%";
			Resp2ApPrintln(respMsg);

			LogPrintln(" LG] InfRq ROD Board Type: " + strRodBoardType);
			break;

		case NUM_ROD_REGIST_TO_SEC:
			respMsg = STX_INFO_REQ + ROD_REGIST_TO_SEC;
			respMsg += String(ROD_REGI_CANCEL_TO/1000);	
			respMsg += "%";
			Resp2ApPrintln(respMsg);

			LogPrintln(" LG] InfRq ROD Regist Wait T/O Sec: " + respMsg);
			break;

		case NUM_IMU_INTVAL_TIME:		 

			{
				respMsg = STX_INFO_REQ + IMU_INTVAL_TIME + strImuInterval + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq ImuInterval[ms]: " + respMsg);
			}
			break;

		case NUM_MAINMOT_INFO_READ:		 

			{
				respMsg = fsInfo.getInfo(MAINMOT_INFO);
				respMsg = STX_INFO_REQ + MAINMOT_INFO_READ + respMsg + "%";	 
				Resp2ApPrintln(respMsg);

			}
			break;

		case NUM_MAINMOT_INFO_SAVE:		 

			{
				short result = fsInfo.saveInfo(MAINMOT_INFO, msg.substring(5));

				if(0 < result)	 
				{
					defaultTorqueMotor = msg.substring(5).toInt();
				}

				respMsg = STX_INFO_REQ + MAINMOT_INFO_SAVE + ((0 < result)? String(defaultTorqueMotor): String(-1)) + "%";	 
				Resp2ApPrintln(respMsg);

			}
			break;

		case NUM_MAIN_BOARD_TYPE:		 

			respMsg = STX_INFO_REQ + MAIN_BOARD_TYPE + strMainBoard[mainBoardType]+ "%";
			Resp2ApPrintln(respMsg);

			break;

		case NUM_BLDC_MOT_LIMIT_READ:		 
			if(AP_IS_TM == apType)
			{
				respMsg = fsInfo.getInfo(BLDC_LIMIT_INFO);

				respMsg = STX_INFO_REQ + BLDC_MOT_LIMIT_READ + respMsg + "%";	 
				Resp2ApPrintln(respMsg);

			}
			break;

		case NUM_BLDC_MOT_LIMIT_WRITE:		 
			if(AP_IS_TM == apType)
			{
				short result;
				short val = msg.substring(5).toInt();

				if( (BLDC_LIMIT_MIN > val) || (BLDC_LIMIT_MAX < val) )

				{
					result = ERR_PARA_NG;	 
				}

				else
				{

					result= fsInfo.saveInfo(BLDC_LIMIT_INFO, String(val));

					if(0 < result)	 
					{
						bldcLimitVal = val;
					}
				}
				respMsg = STX_INFO_REQ + BLDC_MOT_LIMIT_WRITE + ((0 < result)? String(bldcLimitVal): String(result)) + "%";	 
				Resp2ApPrintln(respMsg);

			}
			break;

		default:
			Resp2ApPrintln(STX_INFO_REQ + strKind + "1%");	 
			break;
	}

}

#define AP_ALIVE_NOTRECV_TO	10		 

void apAlive_Check()
{
	if( CONNECT == ap_conn_status)			 
	{
		if( AP_ALIVE_NOTRECV_TO > apAliveConn_NotRecvCnt)	 
		{
			apAliveConn_NotRecvCnt++;
			if (gRodSleepMode == SS_EXIT) {
				if (gmWatStatus == 2) {
					sendSleepEnable(SS_WAIT, TIME_SLEEP_LIGHT, 0);
				}
				else {
					sendSleepEnable(SS_NONE, 0, 0);
				}
			}
		}
		else
		{

			ap_conn_status = DISCONNECT;	 
			sendSleepEnable(SS_EXIT, TIME_SLEEP_DEEP, 1);
			motor_AllOff();

			reelOut_AllOff();

		}
	}

	else if( UNKNOWN == ap_conn_status)
	{

	}
	else
	{

	}
}

void send2RodAlive_CntCheck()
{
	int	CheckTime;

	if (rodAlive_SendCnt == 0) {
		if (gRodSleepMode == SS_NONE) {
			gRodSleepTime = 0;
		}
	}
	if (gRodSleepStat) {
		CheckTime = gRodSleepTime + 4;
	}
	else {

		eNow.write(DF_Protocol_MainToRod_Alive, String(gRodSleepMode));
		CheckTime = ROD_RESP_OVER_CNT;
	}

	if(CheckTime > rodAlive_SendCnt)	 
	{

		rodAlive_SendCnt++;
	}
	else
	{

		rod_conn_status = (ROD_DISCONN);
	}
}

static unsigned int rodRegistToStep = 0;	 

void rodRegistToStop()
{
	rodRegistToStep = 0;	 
}

void rodRegistToStart()
{
	rodRegistToStep = 10;	 
}

void rodRegi3secToStart()
{
	rodRegistToStep = 30;	 
}

void rodRegistToControl()
{
	static unsigned int oldStep = 0;
	static unsigned long _to = 0;

	switch(rodRegistToStep)
	{
		case 0:
			break;

		case 10:	 
			setTO(_to);
			rodRegistToStep = 11;
			break;		
		case 11:
			if(checkTO(_to, ROD_REGI_CANCEL_TO))
			{

				rodRegistCancel();		 
				rodRegistToStep = 0;		 
				StsSendPrintln(STX_ROD_REGIST + ROD_REGI_FW_CANCEL);  
			}
			break;

		case 30:	 
			rodRegistToStep = 31;
			setTO(_to);
			break;		
		case 31:
			if(checkTO(_to, ROD_REGI_END_TO_1ST))	 
			{
				StsSendPrintln(STX_ROD_REGIST+ ROD_REGI_FINISH);  
				rodRegistToStep = 32;		 
				setTO(_to);
			}
			break;
		case 32:
			if(checkTO(_to, ROD_REGI_END_TO_2ND))	 
			{
				rodRegistToStep = 0;		 
				rodRegistCancel();

				esp_soft_reset();

			}
			break;

		default:
			break;
	}
}

void rodRegistCancel()
{
	rodRegistMode = 0;
}

void anaRodRegist(String msg)
{
	String respMsg;
	int Act = msg.substring(3).toInt();
	switch(Act)
	{
		case ROD_REGI_AP_ENTRY:		 
			rodRegistMode = 1;
			LogPrintln(" LG] RodRg ApMsg:" + String(Act));

			rodRegistToStart();
			break;
		case ROD_REGI_AP_CANCEL:		 
			if(1 == rodRegistMode)		 
			{
				rodRegistToStop();		 
				rodRegistCancel();		 
				LogPrintln(" LG] RodRg ApMsg:" + String(Act));

			}
			break;
		default:
			break;
	}

}

void rodRegistExec(String msg)
{
	  rodRegistMode = 2;	   

	  rodRegistToStop();

	  StsSendPrintln(STX_ROD_REGIST+ ROD_REGI_WRITE_START);    

	  rodRegistMode = 3;	   

	  int peerRet;
	  peerRet = esp_now_del_peer(slave_board_addr);
	  if (ESP_OK != peerRet)
	  {
		  LogPrintln(" LG] ERROR DEL PEER:" + String(peerRet));
	  }

	  peerRet = eNow.addPeerAddress(rcv_src_addr_back);    
	  if (ESP_OK != peerRet)
	  {
		  LogPrintln(" LG] ERROR Add PEER: newROD" + String(peerRet));
	  }

	  rodRegistMode = 4;

	  peerRet = eNow.addPeerAddress(broad_cast_addr);  
	  if (ESP_OK != peerRet)
	  {
		  LogPrintln(" LG] ERROR Add PEER: BC" + String(peerRet));
	  }

	  String myAddr = eNow.getMyAddress();
	  eNow.writeBC(DF_Protocol_MainToRod_MainAddress, myAddr);	   
		  LogPrintln(" LG] Write Send:" + myAddr);

	  peerRet = esp_now_del_peer(broad_cast_addr);
	  if (ESP_OK != peerRet)
	  {
		LogPrintln(" LG] ERROR nowPeer DEL: " + String(peerRet));
	  }
	  esp_now_peer_num_t rodPeer;
	  esp_now_get_peer_num(&rodPeer);
	  LogPrintln(" LG] nowPR PeerNum:" + String(rodPeer.total_num));

	  rodRegistMode = 5;

	  {

		  bool errRet = eNow.setTargetAddress(msg);	   
		  if(false == errRet)
		  {
			  LogPrintln(" LG] rodRg ERROR TRGT ADDR WRITE");
		  }
	  }

	  String wrtAddr = eNow.getTargetAddress();
	  LogPrintln(" LG] rodRg Addr:" + wrtAddr);

	  rodRegistMode = 6;	   
	  rodRegi3secToStart(); 	   

}

void tmInput_BoardElecMonitorExec(short _exec)
{
	if(AP_IS_TM == apType)
	{

		diagInFlag_BootSwc = _exec;		 
		diagInFlag_PonSwc = _exec;		 
		diagInFlag_PcUsb5V = _exec;		 
	}
}

void tmInput_BbnMotorMonitorExec(short _exec)
{
	if(AP_IS_TM == apType)
	{

		diagInFlag_BobbinEncA = _exec;		 
		diagInFlag_BobbinEncB = _exec;		 
		diagInFlag_BbnBldcFG = _exec;			 
	}
}

void anaTmInputTestReq(String msg)
{

	diagInputTest(msg);
}

void anaTmOutputTestReq(String msg)
{

	diagOutputTest(msg);
}

void anaApInfo(String msg)
{

	short kind = msg.substring(3,(3+2)).toInt();
	short act = msg.substring(5,(5+2)).toInt();

	String send2rodMsg = msg.substring(3,(3+4));

	switch(kind)
	{

		case DEF_AP_TYPE:		 
			apType = act;	 
			dfMainAdcDiagnosticEnabled = 0;
			eNow.write(DF_Protocol_MainToRod_ApInfo, send2rodMsg);

			if(AP_IS_TM == apType)
			{
				String statusMsg = RESP_STX_ROD_CONN;
				statusMsg += ((ROD_CONN == rod_conn_status) ? "01" : "00");
				StsSendPrintln(statusMsg + "%");

				statusMsg = RESP_STX_IMU_CONNECT;
				statusMsg += ((CONNECT == imu_conn_status) ? "01" : "00");
				StsSendPrintln(statusMsg + "%");

				statusMsg = STX_BAT_LVL;
				statusMsg += (ROD_CONN == rod_conn_status) ? battLvlStr : String("-2");
				StsSendPrintln(statusMsg + "%");
			}

			break;

		default:
			break;
	}

}

void anaMainAlive(String msg)
{

	String respMsg;
	respMsg = STX_MAINALIVE;  
	respMsg += "%";
	Resp2ApPrintln(respMsg);		   

  	if( (2 > recvMainConn_Pwr1stCnt))	 
  	{
		recvMainConn_Pwr1stCnt++;
 		sendCurrentStatus();
 	}

	apAliveElse_NotRecvCnt = 0;
	apAliveConn_NotRecvCnt = 0;		 
	ap_conn_status = CONNECT;	 

}
