
#include "../DF_Main_Internal.h"
#include "DF_Main_Communication.h"
void rotateChangeCallback(int _dir, int _cnt)
{

  String dir = (_dir)? "CW_":"CCW";
  LogPrintln(" LG] BBENC " +  dir + ", cnt: " + String(_cnt));
}

enum
{
	DF_Main_Communication_ReceiveBufferSize = 128,
	DF_Main_Communication_AddressSize = 6
};

static char DF_Main_Communication_ReceiveBuffer[DF_Main_Communication_ReceiveBufferSize];
static uint8_t DF_Main_Communication_SourceAddress[DF_Main_Communication_AddressSize];
static volatile unsigned int DF_Main_Communication_ReceiveLength = 0;
static volatile unsigned int DF_Main_Communication_ReceivePending = 0;
static volatile unsigned long DF_Main_Communication_OverwrittenCount = 0;
static volatile unsigned long DF_Main_Communication_InvalidCount = 0;
static volatile unsigned int DF_Main_Communication_SendPending = 0;
static volatile int DF_Main_Communication_SendStatus = ESP_NOW_SEND_SUCCESS;
static portMUX_TYPE DF_Main_Communication_Mux = portMUX_INITIALIZER_UNLOCKED;

void sent_cb_esp_now_sts(const uint8_t* mac_addr, esp_now_send_status_t status)
{
	(void)mac_addr;
	portENTER_CRITICAL(&DF_Main_Communication_Mux);
	DF_Main_Communication_SendStatus = (int)status;
	DF_Main_Communication_SendPending = 1;
	portEXIT_CRITICAL(&DF_Main_Communication_Mux);
}

void DF_Main_Communication_ProcessSendResult()
{
	int status = ESP_NOW_SEND_SUCCESS;
	unsigned int pending = 0;

	portENTER_CRITICAL(&DF_Main_Communication_Mux);
	pending = DF_Main_Communication_SendPending;
	if (pending)
	{
		status = DF_Main_Communication_SendStatus;
		DF_Main_Communication_SendPending = 0;
	}
	portEXIT_CRITICAL(&DF_Main_Communication_Mux);

	if (pending && ((ESP_NOW_SEND_SUCCESS != status) || rodRegistMode))
	{
		LogPrintln(String(" LG] nowSent ") + ((ESP_NOW_SEND_SUCCESS == status) ? "Ok" : "Fail"));
	}
}

void recv_cb_esp_now_msg(const uint8_t *mac_info, const uint8_t *data, int data_len)
{
	int i;

	if ((NULL == mac_info) || (NULL == data) || (2 > data_len) || (DF_Main_Communication_ReceiveBufferSize <= data_len))
	{
		portENTER_CRITICAL(&DF_Main_Communication_Mux);
		DF_Main_Communication_InvalidCount++;
		portEXIT_CRITICAL(&DF_Main_Communication_Mux);
		return;
	}

	if (!rodRegistMode)
	{
		for (i = 0; i < DF_Main_Communication_AddressSize; i++)
		{
			if (slave_board_addr[i] != mac_info[i])
			{
				return;
			}
		}
	}

	portENTER_CRITICAL(&DF_Main_Communication_Mux);
	if (DF_Main_Communication_ReceivePending)
	{
		DF_Main_Communication_OverwrittenCount++;
	}
	memcpy(DF_Main_Communication_ReceiveBuffer, data, data_len);
	DF_Main_Communication_ReceiveBuffer[data_len] = '\0';
	memcpy(DF_Main_Communication_SourceAddress, mac_info, DF_Main_Communication_AddressSize);
	DF_Main_Communication_ReceiveLength = (unsigned int)data_len;
	DF_Main_Communication_ReceivePending = 1;
	portEXIT_CRITICAL(&DF_Main_Communication_Mux);
}

int DF_Main_Communication_TakeReceived(char *data, unsigned int capacity, unsigned int *length, uint8_t *sourceAddress, unsigned long *overwrittenCount, unsigned long *invalidCount)
{
	unsigned int receivedLength;

	if ((NULL == data) || (NULL == length) || (NULL == sourceAddress) || (NULL == overwrittenCount) || (NULL == invalidCount))
	{
		return 0;
	}

	portENTER_CRITICAL(&DF_Main_Communication_Mux);
	if (!DF_Main_Communication_ReceivePending)
	{
		portEXIT_CRITICAL(&DF_Main_Communication_Mux);
		return 0;
	}
	receivedLength = DF_Main_Communication_ReceiveLength;
	if ((receivedLength + 1) > capacity)
	{
		DF_Main_Communication_InvalidCount++;
		DF_Main_Communication_ReceivePending = 0;
		portEXIT_CRITICAL(&DF_Main_Communication_Mux);
		return 0;
	}
	memcpy(data, DF_Main_Communication_ReceiveBuffer, receivedLength + 1);
	memcpy(sourceAddress, DF_Main_Communication_SourceAddress, DF_Main_Communication_AddressSize);
	*length = receivedLength;
	*overwrittenCount = DF_Main_Communication_OverwrittenCount;
	*invalidCount = DF_Main_Communication_InvalidCount;
	DF_Main_Communication_OverwrittenCount = 0;
	DF_Main_Communication_InvalidCount = 0;
	DF_Main_Communication_ReceivePending = 0;
	portEXIT_CRITICAL(&DF_Main_Communication_Mux);
	return 1;
}

void battLevelSet( String msg)
{

	  battLvlStr = msg.substring(1,(1+2)); 	  			 
	  ibatteryLvl = battLvlStr.substring(0, 3).toInt();	 

}

void setRod_Connected()
{
	rodAlive_SendCnt = 0;		 
	rod_conn_status = ROD_CONN;	 
}

void uartRecvHandlerImp()
{
}

bool isParaCheckOk(int idx, int val)
{

}

static unsigned int torqMotorStop_Step = 0x00;
static unsigned long torqMotor_StopTimeOut = 0;

void torqMot_OffControlStart(unsigned int onTime)
{
	torqMotor_StopTimeOut = (unsigned long)onTime;
	torqMotorStop_Step = 0x10;	 
	exeFlag_torqMotorStop = 1;		 
}

void torqMot_OffControlStop()
{
	exeFlag_torqMotorStop = 0;		 
	torqMotorStop_Step = 0;	 
}

void torqMotor_OffTime_Control()
{
	static unsigned long to;

	switch(torqMotorStop_Step)
	{
		case 0x00:
			break;	 

		case 0x10:	 
			setTO(to);
			torqMotorStop_Step = 0x20;
			break;

		case 0x20:	 
			if(checkTO(to, torqMotor_StopTimeOut))
			{
				torqOff();
					Resp2ApPrintln(STX_TORQUE_MOTOR + "000%");	 
				torqMotorStop_Step = 0x90;
			}
			break;

		case 0x30:	 
			break;

		case 0x40:	 
			break;

		case 0x90:	 
			exeFlag_torqMotorStop = 0;
			torqMotorStop_Step = 0x00;
			break;

		default:
			LogPrintln(" LG] TORQM OffTime Control Step NG!!!");
			break;
	}
}

unsigned int bbnMotorStop_Step = 0x00;
static unsigned long bbnMotor_StopTimeOut = 0;
unsigned int gBbnHallCount = 0;							 

void bbnMotor_OffTime_Start(unsigned int time)
{
	bbnMotor_StopTimeOut = (unsigned long)time;
	bbnMotorStop_Step = 0x10;	 
	exeFlag_bbnMotorStop = 1;		 
}

void bbnMotor_OffTime_Control()
{
	static unsigned long to;
	static unsigned long _oldSen;				 

	switch(bbnMotorStop_Step)
	{
		case 0x00:
			break;	 

		case 0x10:	 
			setTO(to);
			bbnMotorStop_Step = 0x20;
			_oldSen = LVL01_BBNFG;				 
			break;

		case 0x20:	 
			if(checkTO(to, bbnMotor_StopTimeOut))
			{
				bbnMotor.offBldc();
				Resp2ApPrintln(STX_BBN_MOTOR + "00000000%");	 
				bbnMotorStop_Step = 0x90;
			}
			else if(LVL01_BBNFG != _oldSen)		 
			{
				_oldSen = LVL01_BBNFG;			 
				gBbnHallCount++;				 
			}
			break;

		case 0x30:	 
			break;

		case 0x40:	 
			break;

		case 0x90:	 
			exeFlag_bbnMotorStop = 0;
			bbnMotorStop_Step = 0x00;
			break;

		default:
			LogPrintln(" LG] BBNMT OffTime Control Step NG!!!");
			break;
	}
}

void SetIMU_Measure_Out_OnOff(unsigned int _setClr)
{
	String msg;

	if(0 == _setClr)
	{

		msg = "00";
	}
	else
	{

		msg = "01";
	}
	eNow.write(DF_Protocol_MainToRod_ImuDataControl, msg);		 
	reqImuMeasFlag = _setClr;
}

#define GS_PROG_START	1

#define GS_RSV_02		2
#define GS_RSV_03		3
#define GS_WIRE_RDY		4	 

#define GS_LVL_SEL		11
#define GS_ZONE_SEL		21
#define GS_CAST_WAIT	31
#define GS_CAST_COMP	41
#define GS_BITE_1ST		51
#define GS_BITE_PTN		52
#define GS_FAIL			61
#define GS_HIT			71
#define GS_FIGHT		72
#define GS_HOLDON		81
#define GS_RANDING		82
#define GS_SUCCESS		91
#define GS_CONTINUE		92
#define GS_GAME_OVER	98

#define GS_PROG_END		99

void sendSleepEnable(int Mode, int Time, int Id)
{
	String strMsg = StringFormat("%01d", Mode) + StringFormat("%04d", Time);
	gRodSleepMode = Mode;
	gRodSleepTime = Time;
	eNow.write(DF_Protocol_MainToRod_Sleep, strMsg);	 
	LogPrintln(" LG] SLEEP_ENABLE[" + String(Id) + "]: " + strMsg);
}

void sendCurrentStatus()
{
	String respMsg;

		  if(ROD_CONN == rod_conn_status)   
		  {

			  respMsg = RESP_STX_ROD_CONN + STATE_CONNECTED;	   
			  respMsg += "%";
		  }
		  else			   
		  {
			  respMsg = RESP_STX_ROD_CONN + STATE_DISCONN;
			  respMsg += "%"; 	   
		  }
		  Resp2ApPrintln(respMsg);		   

		  LogPrintln(" LG] RODCN "+respMsg);

		  if( CONNECT == imu_conn_status)	   
		  {
			  respMsg = RESP_STX_IMU_CONNECT + STATE_CONNECTED;	   
			  respMsg += "%";
		  }
		  else			   
		  {
			  respMsg = RESP_STX_IMU_CONNECT + STATE_DISCONN;	   
			  respMsg += "%";
		  }
		  Resp2ApPrintln(respMsg);		   

		  LogPrintln(" FW] IMUCN " + respMsg);		 

		respMsg = STX_BAT_LVL + battLvlStr + "%";	 
	  	Resp2ApPrintln(respMsg);		   

		if(ROD_CONN == rod_conn_status)   
		{
			respMsg = STX_BAT_LVL + BAT_STR_CHAGER_NOCHANGE;    
			Resp2ApPrintln(respMsg);			 
		}

		  LogPrintln(" FW] BATLV " + respMsg); 	   

		respMsg = STX_INFO_REQ + ROD_BOARD_TYPE;
		respMsg += strRodBoardType;		 
		respMsg += "%";
		Resp2ApPrintln(respMsg); 	   

			LogPrintln(" FW] INFOR " + respMsg);	   

		respMsg = STX_INFO_REQ + ROD_REGIST_TO_SEC;
		respMsg += String(ROD_REGI_CANCEL_TO/1000);	
		respMsg += "%";
		Resp2ApPrintln(respMsg);

		LogPrintln(" FW] INFOR wt" + respMsg);
}
