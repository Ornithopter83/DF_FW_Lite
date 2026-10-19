
#include "../DF_Rod_Internal.h"
#include "DF_Rod_SetupProtocol.h"
void uartPcHandler_NormalMode()
{

  if (Serial.available())	 
  {
    String msg = Serial.readStringUntil('%');	 
    msg.trim();									 
    if (msg == "")
    {
      return;
    }

	if(0) {}	 

	else if (msg.startsWith(STX_IMU_SETUP) )
	{

		String setMsg = imu.convert_oldSetup(msg);
		imu.setConfig(setMsg);		 
	}

    else if (msg.startsWith(STX_GET_ADDR) || msg.startsWith(STX_SETUP_ENTRY) )
	{

		String str = STX_GET_ADDR + "S" + eNow.getMyAddress() + "," + eNow.getTargetAddress() + "%";
		RespPrintln(str);

		imu.setStartStop(IMU_CONF_STOP);
		flagIMUSTOP = 1;

		Entry_SetupMode();		 
    }

	else if (msg.startsWith(STX_SETUP_EXIT) )
  	{

	}

  	else if (msg.startsWith(STX_IMU_INTVAL_CHK))
	{
	  imuIntervalCheckCnt = 1;
    }
	else if (msg.startsWith(STX_VERSION_READ)) 
	{
	 	if(IMU_CONN == imuStatus)
		{
			msg = imu.getversion();
	  	}
	  	else
	  	{
			msg = rodVer + ",Vi99.99.99";
	  	}

		LogPrintln(" LG] Ver__ " + rodVer + "," + msg + "%");
	}

	else if (msg.startsWith(STX_DEV_INFO_REQ))	 
	{
		int reqKind = msg.substring(3,(3+2)).toInt();
		switch(reqKind)
		{
			case 0:		 
				break;
			case 1:		 
				LogPrintln(" LG] INFO__ Board Type: " + String(rodBoardType));
				break;
			default:
				break;
		}
	}

	else
	{
		LogPrintln(" lg] ERROR_ NOT DEFINE CMD");		
	}

  }
}

int targetAddrWrite(String msg)
{
	int rts = 1000;	 

	  int len = msg.length();

      if((DF_Protocol_MacAddressTextLength - 1) < len)

	  {

      	if(false == eNow.setTargetAddress(msg))
      	{
      		rts = 1001;	 
      		LogPrintln(" LG] Error_ T_ADDR Write NG");
      	}
		else
		{
			LogPrintln(" LG] T_ADDR Set Ok Len:" + String(len) + ",m:" + msg);
		}
	  }
	  else
	  {
	  	rts = len;		 
	  	LogPrintln(" LG] T_ADDR Set Address NG, LESS 17 ");
	  }

	  return rts;
}

void uartPcHandler_Setup()
{

  if (Serial.available())	 
  {
    String msg = Serial.readStringUntil('%');	 
    msg.trim();									 
    if (msg == "")
    {
      return;
    }

	String respMsg;

	if(0) {}	 

	else if (msg.startsWith(STX_SETUP_EXIT))
	{
		Exit_SetupMode();
	}

    else if (msg.startsWith(STX_GET_ADDR))
	{
      respMsg = STX_GET_ADDR + "S" + eNow.getMyAddress() + "," + eNow.getTargetAddress() + "%";
      RespPrintln(respMsg);
    }

    else if (msg.startsWith(STX_SET_ADDR))
	{
		msg = msg.substring(3);		 

  		int ret = targetAddrWrite(msg);
	  	if((1000 != ret) && (!isSetupMode) )	 
	  	{
			String logMsg = "";
	  		if(1001 == ret)	logMsg = "Write Fail";
			else			logMsg = "Len Error";
			LogPrintln(" LG] Error_ TART ADDR " + logMsg +",m"+ msg + ",len:" + String(msg.length()));
	  	}

    }

    else if (msg.startsWith(STX_IMU_SETUP))
	{

		respMsg = imu.convert_oldSetup(msg);
      	imu.setConfig(respMsg);	 
    }

  	else if (msg.startsWith(STX_IMU_SETSTRING))
	{
	  respMsg = msg.substring(3, msg.length());

	  imu.setConfigString(respMsg);
    }

  	else if (msg.startsWith(STX_IMU_INTVAL_CHK))
	{
	  imuIntervalCheckCnt = 1;
    }

	else
	{

	}

  }
}

static int pwrOn_1st_imuRecv =0;
static int imuIntervalTime = 0;

void setRecvImuData()
{

	fRecvImuData = 1;		 

  	if(imuIntervalCheckCnt)
  	{
  		imuIntervalCheckCnt++;
		if(0) {}
  		else if(2 == imuIntervalCheckCnt) { measureCnt = curr_ms_tick;	} 
  		else if(3 == imuIntervalCheckCnt) { imuIntervalTime += (int)(curr_ms_tick - measureCnt); measureCnt = curr_ms_tick;	} 
  		else if(4 == imuIntervalCheckCnt) { imuIntervalTime += (int)(curr_ms_tick - measureCnt); measureCnt = curr_ms_tick;	} 
		else						 
		{
			imuIntervalTime += (int)(curr_ms_tick - measureCnt);		 
			imuIntervalTime = imuIntervalTime/3;
			String strImuIntv = zeroFill4Char((unsigned int)imuIntervalTime);

			RespPrintln(STX_IMU_INTVAL_CHK + strImuIntv +"%");	 

			imuIntervalCheckCnt = 0;
			imuIntervalTime = 0;
			measureCnt = 0;
		}
  	}
}

int isRodNewBoard()
{
	return (rodBoardType);		 
}

int setBoardType()
{
	return NEW_BOARD_2;
}

int getButtonType()
{
	return (buttonType);
}

int getBreakType()
{
	return (breakType);
}

int getEncType()
{
	return (reelEncType);
}

int getBattType()
{
	return (battType);
}

void settingIOtype()
{

}

void setSlaveMode()
{
	int i =0;
	unsigned int readData = 0x00;
	for(i=0; i<5; i++)
	{
		readData += ((unsigned int)digitalRead(SETUP_MODE_PIN) ) << i;
		delay(10);
	}	
	if(0x1F == readData)	rodMode = SETUP_MODE;
	else					rodMode = NORMAL_MODE;
}

int readSlaveMode()
{
	return rodMode;
}
int isSlaveSetupMode()
{
	return rodMode;
}

void Entry_SetupMode()
{
	rodMode = SETUP_MODE;
	led1Grn_OutSts = LOW;
	digitalWrite(LED1_GRN_PIN, led1Grn_OutSts);
	led2Red_OutSts = HIGH;
	digitalWrite(LED2_RED_PIN, led2Red_OutSts);
}
void Exit_SetupMode()
{

	ESP_Soft_Reset();
}

void ESP_Soft_Reset()
{

	ESP.restart();

}
