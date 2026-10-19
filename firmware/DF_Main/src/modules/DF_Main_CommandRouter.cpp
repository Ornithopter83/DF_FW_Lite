 
#include "../DF_Main_Internal.h"
#include "DF_Main_CommandRouter.h"
void ana_TestExecution(String msg)
{
	int act;
	int paraNG = 0;		 
	String respMsg;
	String logMsg;
	static int nEncIdxSave;
	static int nBbnHallCount;
	int i;
	int hasEvent;
	int encoderCount;
	unsigned short encoderDirection;
	unsigned int wireDistance;
	unsigned long encoderInterval;
	encIsrDataBuff encoderEvent;
	unsigned int act2, act3;
	
	act = msg.substring(3,(3+1)).toInt();	 
	switch(act)
	{
		case 0:
			portENTER_CRITICAL(&mux);
			nEncIdxSave = sEncBuffIdx;
			nBbnHallCount = gBbnHallCount;
			portEXIT_CRITICAL(&mux);
			break;
			
		case 1:		 
			for (i=0; ; i++) {
				portENTER_CRITICAL(&mux);
				hasEvent = (nEncIdxSave != sEncBuffIdx);
				if (hasEvent)
				{
					encoderEvent = stEncIntvBuff[nEncIdxSave];
					nEncIdxSave = (nEncIdxSave + 1) & (ENC_INTV_BUFF_SIZE-1);
				}
				portEXIT_CRITICAL(&mux);
				if (!hasEvent)
				{
					break;
				}
				LogPrintln(" lg] EnInt Interval:" +String(i)
					+ ","+ String(encoderEvent.tick)
					+ ","+ String(encoderEvent.width)
					+ ","+ String(encoderEvent.dir)
					+ ","+ String(encoderEvent.enc)
					+ ","+ String(encoderEvent.dist)
					);
			}
		 
			LogPrintln(" LG] HallCnt: (" + String(nBbnHallCount) + ") -> (" + String(nBbnHallCount) + ") = " + String(gBbnHallCount - nBbnHallCount));
			break;
		case 3:		 
			eNow.write(DF_Protocol_MainToRod_Alive, "");
			break;
		case 4:		 
		 
			SerialPortDisable();
			break;
		case 5:		 
			SerialPortEnable();
			break;
		case 6:		 
			subAC_Off();
			break;
		case 7:		 
			subAC_On();
			break;

		case 9:		 
			if (msg.substring(4,4+1) == "-") {
				fsInfo.deleteInfo(BOOTING_INFO);
			}
			else {
				fsInfo.saveInfo(BOOTING_INFO, msg.substring(4, 4+5));
			}
			break;
		case '8':		 
		 
		 
			g_SavePtr = 0;

break;
		default:
			paraNG = 1;
			break;
	}
	if (act < 2) {
		portENTER_CRITICAL(&mux);
		encoderCount = iIsrbbnEncCnt;
		encoderDirection = sIsrBbnEncDir;
		wireDistance = iWireDistance;
		encoderInterval = iEncIntrIntervalTime;
		portEXIT_CRITICAL(&mux);
		LogPrintln(" LG] Encoder: Cnt(" + String(encoderCount) + ") Dir(" + String(encoderDirection) + ") Dist(" + String(wireDistance) + ") Intv(" + String(encoderInterval) + ")");
	}
	
	 
	respMsg = msg.substring(0,(0+5)) + String(paraNG) + "%";
	Resp2ApPrintln(respMsg);
}

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


 
 
 
void anaRanding(String msg)
{

  	randingPtn = msg.substring(5,(5+2)).toInt();
	exeFight = 0;			 
	
	execRanding_SetVal();
	
	exeRanding = 1;
	execRanding_Start();
	gameStatus = GAME_RANDING;
	
	LogPrintln(" LG] GameS RANDING");	 
}

 
 
 
void anaFastHold(String msg)
{
	unsigned int iDir = msg.substring(5,(5+2)).toInt();
	unsigned int iStartStop = msg.substring(7,(7+2)).toInt();
	 

	LogPrintln(" LG] FastH Dir/stop: " + String(iDir) +"," + String(iStartStop));
}

 
 
 
void anaFastComboBtn(String msg)
{
	unsigned int iStartStop = msg.substring(5,(5+2)).toInt();
	 
	
	LogPrintln(" LG] FastB COMBO StartStop: " + String(iStartStop));
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

 
#define GMWAT_MIN	GMWAT_AP_INFO	 
 
#define GMWAT_AP_INFO	1
#define GMWAT_PAY		3
#define GMWAT_WIREHOME	4
#define GMWAT_SELECT	5
#define GMWAT_POINTMOVE	6
#define GMWAT_WAVE		7
#define GMWAT_BITE		8
#define GMWAT_HOOK		9
#define GMWAT_HIT		10
#define GMWAT_FAIL		11
#define GMWAT_FIGHT		12
#define GMWAT_HOLDON	13
#define GMWAT_HARDACT	14
#define GMWAT_RANDING	15
#define GMWAT_SUCCESS	16
#define GMWAT_POINTUP	17
#define GMWAT_TUTORIAL	18
#define GMWAT_BONUS		19
#define GMWAT_STAGECLR	20
#define GMWAT_MALL		21
#define GMWAT_CONTINUE	22
#define GMWAT_OVER		23
 
#define GMWAT_MAX	(1+GMWAT_OVER)	 
 

 
short lmFish_3Level_Set(short pwr)
{
	short level;
	if(0) {}
	else if(66 < pwr)	{level = FISH_LVL_3; }
	else if(33 < pwr)	{level = FISH_LVL_2; }
	else				{level = FISH_LVL_1; }

	return level;
}

 
void lmReturnOrHome_Start()
{
	if(0) {}
	else if(LM_POSI_RIGHT <= lmPosi)	 
	{	
		lmRight_Control_Return();
	}
	else if(LM_POSI_LEFT <= lmPosi)		 
	{
		lmLeft_Control_Return();
	}
	else	 
	{
		lmHome_Control_Start(0, reqTorqueMotor);	 
	}	
}

static volatile short wireHome_Step=0;
static volatile short wireHomeExe_Flag=0;


void wireHomeControl_Start()
{
	wireHomeExe_Flag=1;
	wireHome_Step=STEP_START;
}
void wireHomeControl_Stop()
{
	wireHomeExe_Flag=0;
	wireHome_Step=STEP_IDLE;
}

 
void wireHome_Control()
{
}

 
void setWireHome_EncCnt()
{
	mainEnc.setBbnCnt(WIRE_HOME_BBN_CNT);
	
	portENTER_CRITICAL(&mux);   
		iIsrbbnEncCnt = WIRE_HOME_BBN_CNT;
		iWireDistance = WIRE_HOME_DIST;
	portEXIT_CRITICAL(&mux);    
	
}


 
 
byte nvm_fish_level = FISH_LVL_AUTO;

 


void anaGameStatusWAT(String msg)
{
	 
	short cmdNG = 0;	 
	short paraNG = 0;	 
	
	short act;
	short dir;
	short para;

	short para1 = 0;
	short para2 = 0;
	short para3 = 0;

	short fishType = 0;
	short ptn = 0;	 
	
	short ptn1 = 0;
	short ptn2 = 0;
	short ptn3 = 0;

	String respMsg;
	String logMsg;

	short cmd = msg.substring(3,5).toInt();
	portENTER_CRITICAL(&mux);   
		gmWatStatus = cmd;	 
	portEXIT_CRITICAL(&mux);    

	 
	switch(cmd)
	{

		 
		case GMWAT_STBY:

			 
			if(devReadychkFlag)
			{
				 
				stbyTorqOffControl_Flag = 0;
				stbyTorqOffControl_Stop(0);			 
				stbyTorqOffControl_Req_Flag = 1;	 
			}
			 
			else if(!stbyTorqOffControl_Flag)	 
			{
				stbyTorqOffControl_Start();
			}
			sendSleepEnable(SS_WAIT, TIME_SLEEP_LIGHT, 2);

			break;
		
		 
		case GMWAT_PAY:
			gRodSleepMode = SS_NONE;
			break;
		 
		case GMWAT_WIREHOME:	 
			gRodSleepMode = SS_NONE;
			setWireHome_EncCnt();
			break;
		 
		case GMWAT_SELECT:
			break;
		 
		case GMWAT_POINTMOVE:	 
			gRodSleepMode = SS_NONE;

		  
			break;
		 
		case GMWAT_WAVE:
			break;
			
		 
		case GMWAT_BITE:
			para1 = msg.substring(5).toInt();
			if(100 < para1) { para1 = 100; }
			break;

		 
		case GMWAT_HOOK:
			break;
		 
		case GMWAT_HIT:
			break;
		 
		case GMWAT_FAIL:	 
			 
			break;

		 
		case GMWAT_FIGHT:
			break;
	
		 
		case GMWAT_HOLDON :		 
		 
		case GMWAT_HARDACT:
			break;

		 
		case GMWAT_RANDING:
			break;
		 
		case GMWAT_SUCCESS:
			break;
		 
		case GMWAT_POINTUP:
			break;
		 
		case GMWAT_TUTORIAL:
			break;
		 
		case GMWAT_BONUS:
			break;
		 
		case GMWAT_STAGECLR:
			break;
		 
		case GMWAT_MALL:
			break;
		 
		case GMWAT_CONTINUE:
			break;
		 
		case GMWAT_OVER:
			break;

		default:
			break;
	}

	 
	if(oldGmWatStatus != gmWatStatus)
	{
		if(GMWAT_STBY == oldGmWatStatus)
		{
			 
			stbyTorOffStop_Check();
		}

		 
		oldGmWatStatus = gmWatStatus;
	}

	 
	if(0 < gmWatStatus && GMWAT_MAX > gmWatStatus)
	{
		respMsg = STX_GAME_STS_WAT + msg.substring(3,5) + "00%";	 
	}
	else
	{
		 
		respMsg = STX_GAME_STS_WAT + msg.substring(3,5) + "-1%";	 
	}
	 
	Resp2ApPrintln(respMsg);
	
}


 
 
void stbyTorOffStop_Check()
{
	 
	if(GMWAT_STBY != gmWatStatus)
	{
		 
		stbyTorqOffControl_Req_Flag = 0;
		 
		stbyTorqOffControl_Stop(1);			 
	}

}

 
#define FIGHT_PTN_CW_CCW	1

 


void anaGameStatus(String msg)
{
	 
	int msgKind = msg.substring(3,5).toInt();
	int msgOption = msg.substring(5,7).toInt();
	int para = msg.substring(7,9).toInt();

	int para1 = 0;
	int para2 = 0;
	int para3 = 0;

	int fishType = 0;
	int ptn = 0;	 
	
	int ptn1 = 0;
	int ptn2 = 0;
	int ptn3 = 0;

	switch(msgKind)
	{
		case 1 :		 
			 
			 
			motor_AllOff();								 

			 
			recvMainConn_Pwr1stCnt = 0;

			gameStatus = PROG_START;

			 
			
			 
			 

			 
			eNow.write(DF_Protocol_MainToRod_RodInfoRequest, "");		 

				 
				 
				 
				 
				 
				 
			if (gRodSleepMode == SS_EXIT) {
				gRodSleepMode = SS_WAIT;
			}
			LogPrintln(" LG] GameS PROG Start ");
			
 			break;

		 
		case 99:	 

			isGameEnable = false;				 

			g_RecvCmdCount = 0;					 
 

			execHit_FightBldcStop();
			motor_AllOff();

			 
			 
			reelOut_AllOff();

			 
			recvMainConn_Pwr1stCnt = 0;

			gameStatus = PROG_END;
			exeProgramEnd = 1;  

			sendSleepEnable(SS_EXIT, TIME_SLEEP_DEEP, 3);
			LogPrintln(" LG] GameS ProgEND");    

		 
			 
 			break;

		case 53 :		 
			 

			fishType = msg.substring(5,(5+1)).toInt();	 
			if(1 > fishType ) {fishType = 1; }
			if(6 < fishType ) {fishType = 6; }
		
			ptn = msg.substring(6,(6+3)).toInt();	 
			 
			if(0 == ptn)
			{
				 
			}
			 
			else if(999 == ptn)	 
			{
				exeBite = 0;
				bitePtn_Flag = 0;
				execBite_ptn_Stop();

				 
				 
				sendRodVrtCmd(4, 10, 50, 50);
 			}
			
			 
			else	 
			{
			
				fishTypeAuto = fishType;
		
				ptn1= msg.substring(6,(6+1)).toInt();
				ptn2= msg.substring(7,(7+1)).toInt();
				ptn3= msg.substring(8,(8+1)).toInt();
		
				if(0 != ptn1) { exeBitePtn1flag = 1; execBite_ptn1_Start(); }
				if(0 != ptn2) { exeBitePtn2flag = 1; execBite_ptn2_Start(); }
				if(0 != ptn3) { exeBitePtn3flag = 1; execBite_ptn3_Start(); }
		
				exeBite = 1;
				bitePtn_Flag = 1;

				 
				 
				 
				sendRodVrtCmd(4, 10, 50, 50);
 			}
				

			break;

		case 84 :		 
			 

			fishType = msg.substring(5,(5+1)).toInt();	 
			if(1 > fishType ) {fishType = 1; }
			if(6 < fishType ) {fishType = 6; }
		
			 
			ptn = msg.substring(6).toInt();	 
			if(0 == ptn)
			{
				 
			}
			 
			else if(1 == ptn)
			{
			
				fishTypeAuto = fishType;
		
				if(0 != ptn) { execHoldon_ptn1_Start(); }
				 
			}
				

			break;		
			
 
 
			
	 

		default:
			break;
	}


	if(oldgameStatus != gameStatus)
	{
		 
		 
		LogPrintln(" AP] GameS chg: " + msg);

		oldgameStatus = gameStatus;
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

		 
		case NUM_MANUF_INFO_READ:		 
			 
			{
				respMsg = fsInfo.getInfo(MANUF_INFO);
				respMsg = STX_INFO_REQ + MANUF_INFO_READ + respMsg + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq ManufInfo Read[model,SN,Date]: " + respMsg);
			}
			break;

		case NUM_MANUF_INFO_SAVE:		 
			 
			{
				short result = fsInfo.saveInfo(MANUF_INFO, msg.substring(5));
				respMsg = STX_INFO_REQ + MANUF_INFO_SAVE + String((0 < result)? 1:0) + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq ManufInfo Save[model,SN,Date]:" + respMsg + ", len:" + String(result));
			}
			break;

		case NUM_MANUF_INFO_DEL:		 
			 
			{
				short result = fsInfo.deleteInfo(MANUF_INFO);
				respMsg = STX_INFO_REQ + MANUF_INFO_DEL + String(result) + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq ManufInfo Del[model,SN,Date]: " + respMsg);
			}
			break;

		 
		case NUM_INSTALL_INFO_READ:		 
			 
			{
				respMsg = fsInfo.getInfo(INSTALL_INFO);
				respMsg = STX_INFO_REQ + INSTALL_INFO_READ + respMsg + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq FieldInfo Read[Date]: " + respMsg);
			}
			break;

		case NUM_INSTALL_INFO_SAVE:		 
			 
			{
				short result = fsInfo.saveInfo(INSTALL_INFO, msg.substring(5));
				respMsg = STX_INFO_REQ + INSTALL_INFO_SAVE + String((0 < result)? 1:0) + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq FieldInfo Save[Date]:" + respMsg + ", len:" + String(result));
			}
			break;

		case NUM_INSTALL_INFO_DEL:		 
			 
			{
				short result = fsInfo.deleteInfo(INSTALL_INFO);
				respMsg = STX_INFO_REQ + INSTALL_INFO_DEL + String(result) + "%";	 
				Resp2ApPrintln(respMsg);

				LogPrintln(" LG] InfRq FieldInfo Del[Date]: " + respMsg);
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


 


void anaGameEnbOver(String msg)
{
		  
	 
	    
	   if (msg.endsWith(STATE_WAITING))		 
	   {
		   isGameEnable = false;			    
		    
		    
		   if(DIAG_MODE != mainMode)
		   {
		   		digitalWrite(BD_LED3_PIN, LOW);     
		   }
			gameStatus = GAME_WAIT;
			execGameOver();
			LogPrintln(" AP] GamWT " + msg + ", o/c " + String(oldgameStatus) + "," + String(gameStatus));	   
			if(oldgameStatus != gameStatus)
			{
			   oldgameStatus = gameStatus;
	   		}

	   }
	    
	   else if (msg.endsWith(STATE_ENABLE))	 
	   {
		  
		 LogPrintln(" AP] GamEN " + msg + ", o/c:" + String(oldgameStatus) + "," + String(gameStatus)); 	  
		 
		 isGameEnable = true;				  
		  
		    
		  
		 torqDefault();
	   }
	   else
	   {
	   		 
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
			eNow.write(DF_Protocol_MainToRod_ApInfo, send2rodMsg);

			 
			if(AP_IS_TM == apType)	 
			{
				 
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
	  		

	 
  	if( (2 > recvMainConn_Pwr1stCnt) &&  (PROG_START == gameStatus ))	 
  	{
		recvMainConn_Pwr1stCnt++;
 		sendCurrentStatus();
 	}

	 
	apAliveElse_NotRecvCnt = 0;
	apAliveConn_NotRecvCnt = 0;		 
	ap_conn_status = CONNECT;	 
	
}
 



 
 
 
