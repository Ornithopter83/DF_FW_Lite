 
 
 

#include "Common.h"
#include "EB_IMU.h"
 

extern String rodVer;
extern unsigned short flagIMUSTOP;

extern unsigned short imuIntervalCheckCount;
extern unsigned short apType;		 

extern void setRecvImuData();
extern int isSlaveSetupMode();


 
 
int M_ENABLE = 11111111;
int M_DISABLE = 0;
int M_JYRO_INIT = 10100000;
int M_JYRO_ENABLE = 10101010;

 
char mBuf[MPU_BUF_SIZE];			 
signed int mBuf_cnt = 0;

 
static const char *IMU_FILE = "/imu.txt";			 


 
void (*_imuCallback)(String, String);

EB_IMU::EB_IMU()
{

}

 
typedef struct _respPack
{
	unsigned int ok;
	char str[10];
} stRespPack;
typedef stRespPack *stRespPackPtr;

stRespPack imuRespPack;

stRespPackPtr *stPtr;

#define RESP_SUCESS 0
#define RESP_NG		1

#define RECV_DATA_NO	0
#define RECV_DATA_YES	1
 
 
 
int EB_IMU::respCheck()
{
	
	char chData;
	int i = 0;
	int idx = 0;

	int rts = 0;

 





























	return rts;
}

 




String EB_IMU::reply() {
  String resData = "";
  bool isTimeout = false;
  long timeoutDelay = millis() + MPU_RES_TIMEOUT;	 

   
  while (!imuSerial.available()) {		 
    if (millis() > timeoutDelay) {
      isTimeout = true;				 
      break;
    }
  }

   
  if (!isTimeout) {					 
    while (imuSerial.available()) {		 
      char data = imuSerial.read();
      resData += data;
      delay(1);
    }
  }
  return resData;
}

 
 
void EB_IMU::setStateCallback(void (*imuCallback)(String, String)) {
  _imuCallback = imuCallback;
}

 
void EB_IMU::init()
{
	 

	 
	isMeasure = false;
	imuDataStr = "";

  	 
  	 

 
 













}

void EB_IMU::disable()
{
  isMeasure = false;
}

 
String EB_IMU::convert_oldSetup(String msg)
{
	String rtsMsg ="";
	int val = msg.substring(3).toInt();
	if (M_DISABLE == val)	   
	{
		rtsMsg = "0";
	}
	else if (M_ENABLE == val)		   
	{
		rtsMsg = "1";
	}
	else if (M_JYRO_ENABLE == val)  
	{
		rtsMsg = "3";
	}
	else if (M_JYRO_INIT == val)   
	{
		rtsMsg = "4";
	}
	else	 
	{
		if ( ((IMU_INTV_MIN_TIME) > val) || ((IMU_INTV_MAX_TIME) < val) )		 
		{
			rtsMsg = "20100";	 
		}
		else		 
		{
			rtsMsg = "2" + msg.substring(7,(7+4));	 
		}
	}

	return rtsMsg;

}

 


void EB_IMU::setConfig(String msg)
{
	 
   
  int act = msg.substring(0,1).toInt();
  int val = 100;
  String respMsg= "";

	 
  switch(act)
  {
  	case IMU_DATA_OFF:		 
 	  	isMeasure = false;
		break;

	case IMU_DATA_ON:		 
    	isMeasure = true;
		if (flagIMUSTOP)					 
		{
			flagIMUSTOP = 0;				 
			setStartStop(IMU_CONF_START);		 
		}
		
		if (AP_IS_TM == apType)
		{
		  imuIntervalCheckCount = 1;
		}
		break;

	 
	case IMU_SET_INTEVAL:		 
		 
		val = msg.substring(1).toInt();	 
		if ((IMU_INTV_MIN_TIME) > val || (IMU_INTV_MAX_TIME) < val)		 
		{
			RespPrintln(RESP_IMU_INTV_RNG_OVER);
		}
		else	 
		{
			setInterval(val);		 
			saveInterval(val);		 
		}
		break;

	case IMU_SET_OUT:		 
		 
		 
  		 
		send2Imuln("<sog1>");
		 
    	respMsg = reply().equals("<ok>") ? RESP_IMU_OUTG_OK : RESP_IMU_OUTG_NG;
    	delay(50);		 
    	RespPrintln(respMsg);

		 
		 
		send2Imuln("<sod0>");
 		 
    	respMsg = reply().equals("<ok>") ? RESP_IMU_OUTD_OK : RESP_IMU_OUTD_OK;
    	 
    	RespPrintln(respMsg);
		break;

	case IMU_SET_CALBRATION:		 
		 
		jyro_calibration();
		break;
		
	default:
		break;
		
  }



}

 



void EB_IMU::setConfigString(String msg)
{
    String str;
    
	 
    send2Imuln("<stop>");	 
		 
    str = reply();
    RespPrintln(str);

	 
	{
		 
    	send2Imuln(msg);		 
		 
    	str = reply();
    	RespPrintln(str);

		 
    	send2Imuln("<start>");	 
		 
    	str = reply();
    	RespPrintln(str);
	}


}

#define IMU_PARSER_OK	0
#define IMU_PARSER_NG	1
 




int EB_IMU::ebimu_parser(float *item, int number_of_item)
{
  int n, i;
  int rbytes;
  char *addr;
  int result = IMU_PARSER_NG;

	char str[128];


rbytes = imuSerial.available();			 
if(rbytes)
{
  for (n = 0; n < rbytes; n++)
  {
    mBuf[mBuf_cnt] = imuSerial.read();

	 

	 
    if (mBuf[mBuf_cnt] == 0x0A)			 
    {
    	result = IMU_PARSER_OK;
		
		 
    	memcpy(&str[0], &mBuf[0], (mBuf_cnt));		 
		str[mBuf_cnt] = NULL;
		imuDataStr = String(str);

	  
	  mBuf_cnt = -1;
    }
    else if (mBuf[mBuf_cnt] == '*')		 
    {
      mBuf_cnt = -1;
    }
	 
    else if (mBuf[mBuf_cnt] == 0x0D)			 
    {
    	mBuf_cnt--;			 
    }	
	
    mBuf_cnt++;
    if (mBuf_cnt >= MPU_BUF_SIZE)
    {
      mBuf_cnt = 0;
    }
  }

}
return result;
}

#define IMU_RESP_OK		0
#define IMU_RESP_NOT_MS		1
#define IMU_RESP_NOT_RECV	2

 



int EB_IMU::uartRecv_Normal()
{
   
  int rts = IMU_RESP_NOT_RECV;
  
   
   
  float euler[IMU_DATA_SIZE] = { -0.01, -0.01, -0.01, -0.01, -0.01, -0.01 };				 
  if (IMU_PARSER_OK == ebimu_parser(euler, IMU_DATA_SIZE))
  {
    setRecvImuData();			 
	
	rts = IMU_RESP_OK;

    if (true == isMeasure)		 
    {
		 
    	if (_imuCallback != NULL)
    	{
				_imuCallback(DF_Protocol_RodToMain_ImuData, imuDataStr);
    	}
    }
	else
	{
  		rts = IMU_RESP_NOT_MS;
  	}
  }
  
  return rts;
}

 



int EB_IMU::uartRecv_Setup()
{
	int rts = IMU_RESP_NOT_RECV;
	
   
   
   
  float euler[IMU_DATA_SIZE] = { -0.01, -0.01, -0.01, -0.01, -0.01, -0.01 };				 
  if (IMU_PARSER_OK == ebimu_parser(euler, IMU_DATA_SIZE))		 
  {
    setRecvImuData();		 

	rts = IMU_RESP_OK;

    if (isMeasure)			 
    {
		 
		String str = "$09" + imuDataStr + "%";
		
    	StsPrintln(str);
    }
	else
	{
  		rts = IMU_RESP_NOT_MS;
	}

  }
  
  return rts;
}



 


void EB_IMU::jyro_calibration()
{
  String str;
   
  send2Imuln("<cg>");
	 
  str = reply().equals("<ok>") ? RESP_IMU_CALB_OK : RESP_IMU_CALB_NG;
   
  RespPrintln(str);


}

 


void EB_IMU::setInterval(int ms)
{
  String str;
  
	 
  str = "<sor" + String(ms) + ">";
  send2Imuln(str);

  str = reply().equals("<ok>") ? RESP_IMU_INTV_OK : RESP_IMU_INTV_NG;
   
  RespPrintln(str);

	
}

 


String EB_IMU::getversion()
{
  String str;
	
   
  send2Imuln("<ver>");

	 
  str = reply();
  
  str = str.substring(str.indexOf('<'), (str.indexOf('>')+1));
   

  return str;

}

 



void EB_IMU::setStartStop( int set)
{
	 
  String str;
	
  if(0 == set) str = "<stop>";
  else		   str = "<start>";
  send2Imuln(str);

  str = reply().equals("<ok>") ? STX_IMU_RESP_OK : STX_IMU_RESP_NG;

	 
  if(isSlaveSetupMode()) {
    RespPrintln(str);
  }
}


 
 
 
 
 
 
 
short EB_IMU::checkSetupFileVal()
{
	short rts = 0;			 

	 
	String intvalStr = getIntervalFileStr();	 
	if(intvalStr.equals(""))	 
	{
		rts = 1;	 
	}
	else
	{
		int intval = intvalStr.toInt();
		if((IMU_INTV_MIN_TIME) > intval || (IMU_INTV_MAX_TIME) < intval)	 
		{
			rts = 1;	 
		}
	}
	
	return rts;
}

 
String EB_IMU::getIntervalFileStr()
{
	String intvalStr = "";
	File file = SPIFFS.open(IMU_FILE, "r");
	if (!file || file.isDirectory())
	{
		return intvalStr;
	}

	 
    while (file.available())
    {
    	intvalStr += (char)file.read();
    }
	file.close();

	return intvalStr;
}

 
short EB_IMU::saveInterval(int intval)		 
{
	String intvalStr = String(intval);
	return saveInterval(intvalStr);
}

 
short EB_IMU::saveInterval(String intvalStr)		 
{
	File file = SPIFFS.open(IMU_FILE, "w");
	if (!file || file.isDirectory())
	{
		return (0);
	}

	 
	short rts = file.print(intvalStr.c_str());
    file.close();

	return rts;
}

 
 
 
 
void EB_IMU::autoSetup()
{
	String respMsg;
	send2Imuln("<stop>");
	respMsg = "stop: ";
	if(-1 == reply().indexOf("<ok>"))	{ respMsg += RESP_IMU_STOP_NG; }
	else								{ respMsg += RESP_IMU_STOP_OK; }
    delay(5);		 
  
	send2Imuln("<sor100>");
	respMsg = "intval_100: ";
    respMsg += reply().equals("<ok>") ? RESP_IMU_OUTG_OK : RESP_IMU_OUTG_NG;
	delay(5);		 

	 
	send2Imuln("<sog1>");
	respMsg = "gyro: ";
    respMsg += reply().equals("<ok>") ? RESP_IMU_OUTG_OK : RESP_IMU_OUTG_NG;
    delay(5);		 

	 
	send2Imuln("<sod0>");
	respMsg = "Distance: ";
    respMsg += reply().equals("<ok>") ? RESP_IMU_OUTD_OK : RESP_IMU_OUTD_OK;
    delay(5);

	send2Imuln("<start>");
	respMsg = "start: ";
    respMsg += reply().equals("<ok>") ? RESP_IMU_START_OK : RESP_IMU_START_NG;
    delay(5);

}


