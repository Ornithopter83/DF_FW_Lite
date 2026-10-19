
#include "../DF_Main_Internal.h"
#include "DF_Main_BasicControl.h"
void anaDeviceContReq(String msg)
{
	unsigned int iKind = msg.substring(3).toInt();

	String respMsg ="--";

	switch(iKind)
	{

		case 0:			 
			motor_AllOff();		 

			reelOut_AllOff();	 

			LogPrintln(" LG] DevRq $1500 Device AllOff");
			break;

		case 1:			 

			if(0 == devReadychkFlag)
			{

				if(devChkErrOccure)
				{
					devChkErrOccure = 0;
				}

devReadyCheck();

			}

			if( AP_IS_TM == apType) 		 
			{
				Resp2ApPrintln(STX_INFO_REQ + MAINMOT_INFO_READ + String(defaultTorqueMotor)+"%");	 
 				Resp2ApPrintln(STX_INFO_REQ + BLDC_MOT_LIMIT_READ + String(bldcLimitVal)+"%");		 
			}

			break;

		default:
			break;
	}

}

void anaRestDistance(String msg)
{

	restPowerDist = msg.substring(5,(5+3)).toInt();
	if( 10 > restPowerDist ) { restPowerDist = 10; }
	if(200 < restPowerDist ) { restPowerDist = 200; }

	LogPrintln(" AP] RestD Dist: ," + String(restPowerDist) + " [/M]");

}

void ana_AngleControl( String msg )
{

}

void ana_MainMotControl( String msg )
{
	int recvTorq;
	unsigned int onTime = 0;

	  	recvTorq = msg.substring(3,(3+3)).toInt();		 

		onTime = (unsigned int)msg.substring(6).toInt();
	  	if(0 != onTime)	 
	  	{
	  	}
		else
		{
		}

	int torq = recvTorq;		 

	if(0 < recvTorq)
	{
		reqTorqueMotor = recvTorq;
		recv_TorqMotor_Flag = 1;
	}

	if(defaultTorqueMotor > reqTorqueMotor) 	{ reqTorqueMotor = defaultTorqueMotor; }

			if(devReadychkFlag)
			{

			}
			else	 
			{
				torqMotor.setValue(torq);
				if(onTime && (AP_IS_TM == apType))
				{
					torqMot_OffControlStart(onTime);
				}
			}

		  Resp2ApPrintln(msg+"%");

}

#define AP_BBN_CMD_OFF 0
#define AP_BBN_CMD_CW 1
#define AP_BBN_CMD_CCW 2
#define AP_BBN_CMD_MAX 3

static String reqBobbinMsg;

void ana_BobbinControl_Check(String msg)
{
	reqBobbinMsg = msg;

		if(devReadychkFlag)
		{

		}
		else	 
		{
			ana_BobbinControl(msg);
		}

}

static String reqLmMotMsg;

void ana_BobbinControl(String msg)
{

	String respMsg = "";

	int act = msg.substring(3,(3+1)).toInt();
	if(0 > act ) { act = 4; }		 
	int duty = msg.substring(4,(4+3)).toInt();

	if(0 > duty) { duty = 0; }
	if(255 < duty) { duty = 255; }
	int onTime = msg.substring(7,(7+4)).toInt();
	if(0 > onTime ) { onTime = 0; }		 

	{

	}

	LogPrintln(" AP] BLDCo " + msg);

	if(0) { }	 

	else if(AP_BBN_CMD_OFF == act)
	{
		bbnMotor.offBldc();
	}
	else if(AP_BBN_CMD_CW == act)	 
	{
		bbnMotor.onBldc(BBN_MOT_CW, duty);
		if(onTime) { bbnMotor_OffTime_Start(onTime); }
	}
	else if(AP_BBN_CMD_CCW == act)	 
	{
		bbnMotor.onBldc(BBN_MOT_CCW, duty);
		if(onTime) { bbnMotor_OffTime_Start(onTime); }
	}
	else
	{

	}

	if(AP_BBN_CMD_MAX > act)
	{
		respMsg = msg.substring(0,(0+4)) + fillZero2String3Char(bbnMotor.sbbnOut) + msg.substring(7) + "%";
		Resp2ApPrintln(respMsg);	 
	}

}

#define STEP_OK_END 99
#define STEP_NG_END 98

#define SEN_REPEAT_TIME 300  

#define LEFT_MOVE_PULSE	 46
static uint16_t nvm01_lm_return_stop = 1;
static unsigned int lmLeftPulseCnt = LEFT_MOVE_PULSE;

#define RIGHT_MOVE_PULSE		46
static uint16_t nvm02_lm_return_stop = 1;
static unsigned int lmRightPulseCnt = RIGHT_MOVE_PULSE;

void checkRodTimer()
{

	String rodConnMsg = "-1";
	String imuConnMsg = "-1";
	String batMsg = "-1";

 	if (isGameEnable)		   
	{
	}
	else
	{
	}

	if (old_rod_conn_status != rod_conn_status)
	{

		if (ROD_CONN == rod_conn_status)		 
		{

			rodConnMsg = RESP_STX_ROD_CONN + STATE_CONNECTED;
			if (AP_IS_TM == apType)
			{
				eNow.write(DF_Protocol_MainToRod_ApInfo, "0101");
				if (dfMainAdcDiagnosticEnabled) eNow.write(DF_Protocol_MainToRod_ApInfo, "0201");
			}

		}
		else	 
		{
			rodConnMsg = RESP_STX_ROD_CONN + STATE_DISCONN;

		}
		rodConnMsg += "%";
		StsSendPrintln(rodConnMsg); 		 
		LogPrintln(" LG] SLCN3 " + rodConnMsg + ", o,c:" + String(old_rod_conn_status) + "," + String(rod_conn_status));	   

		if (ROD_CONN == rod_conn_status)		 
		{
			batMsg = STX_BAT_LVL + battLvlStr + "%";	 
			StsSendPrintln(batMsg); 	 
		}
		else									 
		{
			batMsg = STX_BAT_LVL + "-2%";				 
			StsSendPrintln(batMsg); 	 
		}
		LogPrintln(" LG] RODCN bat:" + batMsg);

		batMsg = STX_BAT_LVL + BAT_STR_CHAGER_NOCHANGE;
		StsSendPrintln(batMsg);  

		old_rod_conn_status = rod_conn_status;	 
	}

	if(oldibatteryLvl != ibatteryLvl)
	{
		String logStr;

		if (ROD_UNKNOWN == rod_conn_status )
		{		
			batMsg = STX_BAT_LVL + "-1%";
			StsSendPrintln(batMsg); 	 
			logStr = " LG] ROD UNKNOWN";

			batMsg = STX_BAT_LVL + BAT_STR_CHAGER_NOCHANGE;
			StsSendPrintln(batMsg);  

		}
		else		 
		{

			batMsg = STX_BAT_LVL + battLvlStr + "%";
			StsSendPrintln(batMsg); 	 
			logStr = " LG] RODCN";

			if(ROD_DISCONN == rod_conn_status)  
			{
				batMsg = STX_BAT_LVL + BAT_STR_CHAGER_NOCHANGE;
				StsSendPrintln(batMsg);  
			}

			else if(ROD_CONN == rod_conn_status)	 
			{

				if((UNKNOWN != ibatChargeLvl) && ((ibatChargeLvl+3) < ibatteryLvl))	 

				{
					ibatChargeLvl = ibatteryLvl;		 

					batMsg = STX_BAT_LVL + BAT_STR_CHAGER_CHANGING;
					StsSendPrintln(batMsg);	 
				}

				else if((UNKNOWN != ibatChargeLvl) && ((ibatChargeLvl-1) > ibatteryLvl))	 

				{
					ibatChargeLvl = ibatteryLvl;		 

					batMsg = STX_BAT_LVL + BAT_STR_CHAGER_USE;
					StsSendPrintln(batMsg);	 
				}
			}	

		}

			LogPrintln(logStr +  ",bat o/c/c:" + String(oldibatteryLvl)+","+String(ibatteryLvl)+","+String(ibatChargeLvl));
		oldibatteryLvl = ibatteryLvl;
		if(UNKNOWN == ibatChargeLvl) { ibatChargeLvl = ibatteryLvl; }		 
	}

	if(oldimu_conn_status != imu_conn_status)
	{
		imuConnMsg = RESP_STX_IMU_CONNECT;
		if(CONNECT == imu_conn_status) { imuConnMsg +=  STATE_CONNECTED; }
		else							{ imuConnMsg +=  STATE_DISCONN;   }
		imuConnMsg += "%";

		StsSendPrintln(imuConnMsg); 		 

		oldimu_conn_status = imu_conn_status;
	}

}

void set_rod_conn_status(unsigned int sts)
{

	rod_conn_status = (sts);
}
