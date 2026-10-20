
#include "../DF_Rod_Internal.h"
#include "DF_Rod_Communication.h"

enum { DF_Rod_Communication_ReceiveBufferSize = 128 };
static char DF_Rod_Communication_ReceiveBuffer[DF_Rod_Communication_ReceiveBufferSize];
static unsigned char DF_Rod_Communication_ReceiveSourceAddress[6];
static volatile unsigned int DF_Rod_Communication_ReceiveLength = 0;
static volatile unsigned int DF_Rod_Communication_ReceivePending = 0;
static volatile unsigned long DF_Rod_Communication_OverwrittenCount = 0;
static volatile unsigned long DF_Rod_Communication_InvalidCount = 0;
static volatile unsigned int DF_Rod_Communication_SendPending = 0;
static volatile int DF_Rod_Communication_SendStatus = ESP_NOW_SEND_SUCCESS;
static portMUX_TYPE DF_Rod_Communication_Mux = portMUX_INITIALIZER_UNLOCKED;

void sent_cb_esp_now_sts(const uint8_t* mac_addr, esp_now_send_status_t status)
{
	(void)mac_addr;
	portENTER_CRITICAL(&DF_Rod_Communication_Mux);
	DF_Rod_Communication_SendStatus = (int)status;
	DF_Rod_Communication_SendPending = 1;
	portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
}

void DF_Rod_Communication_ProcessSendResult()
{
	int status = ESP_NOW_SEND_SUCCESS;
	unsigned int pending = 0;
	portENTER_CRITICAL(&DF_Rod_Communication_Mux);
	pending = DF_Rod_Communication_SendPending;
	if (pending)
	{
		status = DF_Rod_Communication_SendStatus;
		DF_Rod_Communication_SendPending = 0;
	}
	portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
	if (pending && (ESP_NOW_SEND_SUCCESS != status))
	{
		LogPrintln(" LG] nowSent Fail");
	}
}

void recv_cb_esp_now_msg(const uint8_t *mac_info, const uint8_t *data, int data_len)
{
	if ((NULL == mac_info) || (NULL == data) || (2 > data_len) || (DF_Rod_Communication_ReceiveBufferSize <= data_len))
	{
		portENTER_CRITICAL(&DF_Rod_Communication_Mux);
		DF_Rod_Communication_InvalidCount++;
		portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
		return;
	}
	portENTER_CRITICAL(&DF_Rod_Communication_Mux);
	if (DF_Rod_Communication_ReceivePending)
	{
		DF_Rod_Communication_OverwrittenCount++;
	}
	memcpy(DF_Rod_Communication_ReceiveBuffer, data, data_len);
	memcpy(DF_Rod_Communication_ReceiveSourceAddress, mac_info, 6);
	DF_Rod_Communication_ReceiveBuffer[data_len] = '\0';
	DF_Rod_Communication_ReceiveLength = (unsigned int)data_len;
	DF_Rod_Communication_ReceivePending = 1;
	portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
}

int DF_Rod_Communication_TakeReceived(char *data, unsigned int capacity, unsigned int *length, unsigned char *sourceAddress, unsigned long *overwrittenCount, unsigned long *invalidCount)
{
	unsigned int receivedLength;
	if ((NULL == data) || (NULL == length) || (NULL == sourceAddress) || (NULL == overwrittenCount) || (NULL == invalidCount))
	{
		return 0;
	}
	portENTER_CRITICAL(&DF_Rod_Communication_Mux);
	if (!DF_Rod_Communication_ReceivePending)
	{
		portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
		return 0;
	}
	receivedLength = DF_Rod_Communication_ReceiveLength;
	if ((receivedLength + 1) > capacity)
	{
		DF_Rod_Communication_InvalidCount++;
		DF_Rod_Communication_ReceivePending = 0;
		portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
		return 0;
	}
	memcpy(data, DF_Rod_Communication_ReceiveBuffer, receivedLength + 1);
	memcpy(sourceAddress, DF_Rod_Communication_ReceiveSourceAddress, 6);
	*length = receivedLength;
	*overwrittenCount = DF_Rod_Communication_OverwrittenCount;
	*invalidCount = DF_Rod_Communication_InvalidCount;
	DF_Rod_Communication_OverwrittenCount = 0;
	DF_Rod_Communication_InvalidCount = 0;
	DF_Rod_Communication_ReceivePending = 0;
	portEXIT_CRITICAL(&DF_Rod_Communication_Mux);
	return 1;
}

void btnChangeCallback(String str) {
  eNow.write(DF_Protocol_RodToMain_Button, str);
}

void rotateChangeCallback(String str) {
  eNow.write(DF_Protocol_RodToMain_Encoder, str);
}

void encoderCalibrationChangeCallback(String str) {
	eNow.write(DF_Protocol_RodToMain_EncoderCalibration, str);
}

void imuDataCallback(String pid, String str) {
  eNow.write(pid, str);
}

int rodAliveRcv_LedBlinkFlag = 0;

#define ROD_STS_UNKNOWN	0
#define ROD_STS_PWRON	1

static unsigned int rodStatus = ROD_STS_UNKNOWN;

unsigned short exeFlag_RodImuOutCycle = 0;		 

void sendInfo_boardType()
{
	String msg = "001";
	eNow.write(DF_Protocol_RodToMain_BoardType, msg);

		LogPrintln(" LG] RodTyp " + String(rodBoardType) + "," + msg);
}

void imuConnResp()
{
	String msg;

	if(IMU_CONN == imuStatus)
	{

		msg = "11";
	}
	else
	{

		msg = "00";
	}
	eNow.write(DF_Protocol_RodToMain_ImuConnection, msg);
}

void nowRecvHandler()
{
	enum { DF_Rod_Communication_HandlerBufferSize = 128 };
	char receiveBuffer[DF_Rod_Communication_HandlerBufferSize];
	unsigned char sourceAddress[6];
	unsigned int receiveLength = 0;
	unsigned long overwrittenCount = 0;
	unsigned long invalidCount = 0;
	int now_rcv_id;

	DF_Rod_Communication_ProcessSendResult();
	if (!DF_Rod_Communication_TakeReceived(receiveBuffer, sizeof(receiveBuffer), &receiveLength, sourceAddress, &overwrittenCount, &invalidCount))
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

	int notDefineCmdFlag = 0;
	String cmd_str_data = now_msg_str.substring(2);

  if(0) {   }

  else if (DF_Protocol_Pid_RodToMain_RodAddress == now_rcv_id)			 
  {
		LogPrintln(" LG] rodRg rcv " + (now_msg_str) + "NOT_EXEC");
  }

  else if (DF_Protocol_Pid_MainToRod_MainAddress == now_rcv_id)			 
  {
  	if(55 == rodRegistMode)	 
  	{

  		rodRegistToStop();		 

		String msgAddr = cmd_str_data;

  		int ret = targetAddrWrite(msgAddr);
	  	if((1000 != ret) && (!isSetupMode) )	 
	  	{
			String logMsg = "";
	  		if(1001 == ret)	logMsg = "Write Fail ";
			else			logMsg = "Len Error ";
			LogPrintln(" LG] Error_ TART ADDR " + logMsg + ",m:" + msgAddr + ",len:" + String(msgAddr.length()));
	  	}

		String wrtAddr = eNow.getTargetAddress();
		LogPrintln(" LG] rodRg Addr:" + wrtAddr);

		rodRegi3secToStart();		 

  	}
	else	 
	{

			LogPrintln(" LG] rodRg rcv " + (now_msg_str) + "NOT_EXEC");
	}
  }

	else if (DF_Protocol_Pid_MainToRod_ApInfo == now_rcv_id)
	{
		short kind = cmd_str_data.substring(0,2).toInt();
		short val = cmd_str_data.substring(2,4).toInt();
		switch(kind)
		{
			case 1:	 
				apType = val;
				dfRodAdcDiagnosticEnabled = 0;
				LogPrintln(" lg] APTYPE " + String(apType) + (apType? ",TM":",DF"));
				break;

			case 2:
				dfRodAdcDiagnosticEnabled = ((AP_IS_TM == apType) && (val == 1)) ? 1 : 0;
				break;

			default:
				break;
		}
	}

  else if (DF_Protocol_Pid_MainToRod_AllOutputsOff == now_rcv_id)
  {
	int act = cmd_str_data.substring(0,(0+2)).toInt();
	if(0 == act)
	{
		anaReelDeviceControl(act, cmd_str_data);
	}
	else
	{

	}
  }

  else if (DF_Protocol_Pid_MainToRod_PowerOn == now_rcv_id)			 
  {

	imu.isMeasure = false;		 

  	mainStatus = MAIN_STS_PWRON;		 
		LogPrintln(" LG] MN_RCV MainPowerOn, PID = 5 ");

  }

  else if (DF_Protocol_Pid_MainToRod_RodInfoRequest == now_rcv_id)			 
  {

 	imu.isMeasure = false;		 

	recvInfoSeqStep = 1; 	 
 		LogPrintln(" LG] RECV__ RODINFOREQ Ap Start, PID = 6 ");

  }

  else if (DF_Protocol_Pid_MainToRod_Alive == now_rcv_id)			 
  {

	rod_Alive_recv_over_cnt = 0;			 
	rodAliveRcv_LedBlinkFlag = 1;			 

	eNow.write(DF_Protocol_RodToMain_Alive, "");

	if(MAIN_STS_PWRON == mainStatus)
	{
		recvInfoSeqStep = 1; 		 

	}
	mainStatus = MAIN_STS_CONN;

  }

  else if (DF_Protocol_Pid_MainToRod_VersionRead == now_rcv_id)
  {

	String msg = rodVer;
  	if(IMU_CONN == imuStatus)
  	{
		msg += ",";
		msg += imu.getversion();
		eNow.write(DF_Protocol_RodToMain_Version, msg);		 
	}
	else
	{
	  	msg += ",Vi99.99.99";
		eNow.write(DF_Protocol_RodToMain_Version, msg);		 
	}
	LogPrintln( " lg] ver: " + msg);
  }

  else if (DF_Protocol_Pid_MainToRod_Break == now_rcv_id)	 
  {

  }

  else if (DF_Protocol_Pid_MainToRod_ImuDataControl == now_rcv_id)	 
  {

  	{
  		int act = cmd_str_data.substring(0,1).toInt();
		if( 0 == act || 1 == act)
		{
			imu.setConfig(cmd_str_data);		 
		}
		else
		{
				LogPrintln( " lg] Error IMU CMD Error m:" + cmd_str_data);

		}
  	}
  }

  else if (DF_Protocol_Pid_MainToRod_VerticalMotor == now_rcv_id)	 
  {
		if(isRodNewBoard())
		{
			anaVrtMotMainCmd(cmd_str_data);

		}
		else
		{
			LogPrintln(" LG] VrtMot Err: NOT_EXIST VRT MOT");
		}
  }

  else if (DF_Protocol_Pid_MainToRod_ButtonLed == now_rcv_id)	 
  {
 		if(isRodNewBoard())
		{
			anaBtnLedMainCmd(cmd_str_data);

		}
		else
		{
			LogPrintln(" LG] BtnLED Err:NOT_EXIST BTN LED");
		}
  } 

	else if (DF_Protocol_Pid_MainToRod_EncoderCalibration == now_rcv_id)
	{
		char action = cmd_str_data.length() ? cmd_str_data.charAt(0) : '\0';
		if (DF_Protocol_EncoderCalibrationAction_Start == action) encoder.beginCalibration();
		else if (DF_Protocol_EncoderCalibrationAction_Complete == action) encoder.completeCalibration();
		else if (DF_Protocol_EncoderCalibrationAction_Apply == action) encoder.applyCalibration();
		else if (DF_Protocol_EncoderCalibrationAction_Cancel == action) encoder.cancelCalibration();
		else if (DF_Protocol_EncoderCalibrationAction_Query == action) encoder.queryCalibration();
		else
		{
			String payload = "6,";
			payload += encoder.calibrationValid ? "1," : "0,";
			payload += String(encoder.calibrationRawPulseCount) + ",";
			payload += String(encoder.calibratedPulsesPerRevolution);
			encoderCalibrationChangeCallback(payload);
		}
	}

  else					 
  {

  		LogPrintln(" LG] NowRcv Fail NotDef Pid:" + String(now_rcv_id) + ", msg:" + (now_msg_str));
  }

}

void anaReelDeviceControl(int act, String msg)
{

	switch(act)
	{
		case 0:		 

			imu.setConfig("0");		 

			rodVrtControl_Stop();
			digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);		 

			rodBtnLedControl_Stop(0);
			rodBtnLedControl_Stop(1);
			digitalWrite(BTN_LED_LF_RED_PIN, BTN_LED_OFF);	 
			digitalWrite(BTN_LED_RT_BLUE_PIN, BTN_LED_OFF);	 
			break;

		default:

			break;
	}
}

void anaVrtMotMainCmd(String msg)
{
	int act = msg.substring(0,(0+1)).toInt();
	int cnt = 1;
	int ontime = 100;
	int offtime = 50;

	switch(act)
	{
		case 0:
			digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);		 
			rodVrtControl_Stop();
			break;

		case 1:
			digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_ON);		 
			rodVrtControl_Stop();

			break;
		case 4:  

			digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);		 
			rodVrtControl_Stop();

			cnt = msg.substring(1,(1+2)).toInt();
			if(!cnt) { cnt = 1;}
			vrt_ap_req_cnt = cnt;
			ontime = msg.substring(3,(3+3)).toInt();
			if(20 > ontime || 999 < ontime) { ontime = 100; }
			vrt_ap_req_on_time = ontime;
			offtime = msg.substring(6,(6+3)).toInt();
			if(20 > offtime || 999 < offtime) { offtime = 50; }
			vrt_ap_req_off_time = offtime;

			rodVrtControl_Start(vrt_ap_req_cnt, vrt_ap_req_on_time, vrt_ap_req_off_time);	 

			break;

		case 5:  
			digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);		 
			rodVrtControl_Stop();

			break;

		default:

			break;
	}

}

#define BTN_LED_ALL_POSI	0
#define BTN_LED_LEFT_POSI	1
#define BTN_LED_RIGHT_POSI	2
#define BTN_LED_LAST_POSI	BTN_LED_RIGHT_POSI	 

void anaBtnLedMainCmd(String msg)
{
	int idx;

	int act = msg.substring(1,2).toInt();
	if(5 < act)
	{
		return;
	}	 

	int posi = msg.substring(0,1).toInt();

	if (BTN_LED_ALL_POSI == posi && (0 == act || 1 == act))
	{

	}
	else if (posi < BTN_LED_LEFT_POSI || BTN_LED_LAST_POSI < posi)	 
	{
		return;
	}	 

	idx = (posi-1);		 

	int pin;
	int cnt;
	unsigned int ontime;
	unsigned int offtime;	

	switch(act)
	{
		case 0:	 
			if(BTN_LED_ALL_POSI == posi)	 
			{
				digitalWrite(BTN_LED_LF_RED_PIN, BTN_LED_OFF);		 
				digitalWrite(BTN_LED_RT_BLUE_PIN, BTN_LED_OFF);		 
				rodBtnLedControl_Stop(0);
				rodBtnLedControl_Stop(1);
			}
			else
			{
				if(idx) pin = BTN_LED_RT_BLUE_PIN;
				else	pin = BTN_LED_LF_RED_PIN;
				digitalWrite(pin, BTN_LED_OFF); 	 
				rodBtnLedControl_Stop(idx);
			}

			break;

		case 1:	 
			if(BTN_LED_ALL_POSI == posi)	 
			{
				digitalWrite(BTN_LED_LF_RED_PIN, BTN_LED_ON);		 
				digitalWrite(BTN_LED_RT_BLUE_PIN, BTN_LED_ON);		 
				rodBtnLedControl_Stop(0);
				rodBtnLedControl_Stop(1);
			}
			else
			{
				if(idx) pin = BTN_LED_RT_BLUE_PIN;
				else	pin = BTN_LED_LF_RED_PIN;
				digitalWrite(pin, BTN_LED_ON);		 
				rodBtnLedControl_Stop(idx);
			}
			break;

		case 2:	 
			break;

		case 3:	 
			break;

		case 4:  

			if(idx) pin = BTN_LED_RT_BLUE_PIN;
			else	pin = BTN_LED_LF_RED_PIN;
			digitalWrite(pin, BTN_LED_OFF);		 
			rodBtnLedControl_Stop(idx);

			cnt = msg.substring(2,(2+2)).toInt();
			if(!cnt) { cnt = 1;}
			btn_led_ap_req_cnt[idx] = cnt;
			ontime = msg.substring(4,(4+3)).toInt();
			if(20 > ontime || 999 < ontime) { ontime = 50; }
			btn_led_ap_req_on_time[idx] = ontime;
			offtime = msg.substring(7,(7+3)).toInt();
			if(20 > offtime || 999 < offtime) { offtime = 50; }
			btn_led_ap_req_off_time[idx] = offtime;

			rodBtnLedControl_Start(idx, cnt, ontime, offtime);	 

			break;

		case 5:  
			if(idx) pin = BTN_LED_RT_BLUE_PIN;
			else	pin = BTN_LED_LF_RED_PIN;
			digitalWrite(pin, BTN_LED_OFF);		 
			rodBtnLedControl_Stop(idx);

		default:

			break;
	}

}

static int vrt_cont_cnt = 0;
static unsigned int vrt_cont_on_time = 100;
static unsigned int vrt_cont_off_time = 50;

static unsigned int vrt_cont_step = 0;

#define VRT_MOT_CONT_START	10
#define VRT_MOT_CONT_STOP	0

void rodVrtControl_Start(int cnt, unsigned int ontime, unsigned int offtime)
{
	vrt_cont_cnt = cnt;
	vrt_cont_on_time = ontime;
	vrt_cont_off_time = offtime;
	vrt_cont_step = VRT_MOT_CONT_START;

	vrt_cont_flag = 1;
}

void rodVrtControl_Stop()
{
	vrt_cont_step = VRT_MOT_CONT_STOP;
	vrt_cont_flag = 0;
}

void rodVrtControl()
{
	static unsigned int oldStep = 0;
	static unsigned long _to = 0;
	static unsigned long _cnt = 0;

	switch(vrt_cont_step)
	{
		case 0:
			break;

		case VRT_MOT_CONT_START:	 
			_cnt = 0;
			vrt_cont_step = 20;
			break;

		case 20:		 
			_cnt++;
			if(_cnt > vrt_cont_cnt)	 
			{
				vrt_cont_step = 90;
				break;
			}
			else
			{
				digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_ON);		 
				setTO(_to);
				vrt_cont_step = 21;
				break;
			}
		case 21:		 
			if(checkTO(_to, vrt_cont_on_time))
			{
				digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);		 
				setTO(_to);
				vrt_cont_step = 22;
			}
			break;
		case 22:		 
			if(checkTO(_to, vrt_cont_off_time))
			{
				vrt_cont_step = 20;		 
			}
			break;

		case 90:
			oldStep = 0;
			_to = 0;
			_cnt = 0;
			rodVrtControl_Stop();
			break;

		default:
			break;
	}
}

static unsigned int btn_led_cnt[BTN_LED_MAX_SIZE] = {0,0};
static unsigned int btn_led_on_time[BTN_LED_MAX_SIZE] = {100,100};
static unsigned int btn_led_off_time[BTN_LED_MAX_SIZE] = {50,50};

static unsigned int btn_led_step[BTN_LED_MAX_SIZE] = {0,0};

#define BTN_LED_CONT_START	10
#define BTN_LED_CONT_STOP	0

void rodBtnLedControl_Start(int idx, unsigned int cnt, unsigned int ontime, unsigned int offtime)
{
	btn_led_cnt[idx] = cnt;
	btn_led_on_time[idx] = ontime;
	btn_led_off_time[idx] = offtime;
	btn_led_step[idx] = BTN_LED_CONT_START;

	btn_led_flag[idx] = 1;

}

void rodBtnLedControl_Stop(int idx)
{
	btn_led_step[idx] = BTN_LED_CONT_STOP;
	btn_led_flag[idx] = 0;

}

#define BTN_LED_LEFT_IDX	0
#define BTN_LED_RIGHT_IDX	1

void rodLfBtnLedControl()
{
	static unsigned int oldStep = 0;
	static unsigned long _to = 0;
	static unsigned long _cnt = 0;
	String sendMsg;

	int idx = BTN_LED_LEFT_IDX;	 

	switch(btn_led_step[idx])
	{
		case 0:
			break;

		case BTN_LED_CONT_START:	 
			_cnt = 0;
			btn_led_step[idx] = 20;
			break;

		case 20:		 
			_cnt++;
			if(_cnt > btn_led_cnt[idx])	 
			{
				btn_led_step[idx] = 90;
				break;
			}
			else
			{

				digitalWrite(BTN_LED_LF_RED_PIN, BTN_LED_ON); 	 
				setTO(_to);
				btn_led_step[idx] = 21;
				break;
			}
		case 21:		 
			if(checkTO(_to, btn_led_on_time[idx]))
			{

				digitalWrite(BTN_LED_LF_RED_PIN, BTN_LED_OFF); 	 
				setTO(_to);
				btn_led_step[idx] = 22;
			}
			break;
		case 22:		 
			if(checkTO(_to, btn_led_off_time[idx]))
			{
				btn_led_step[idx] = 20;		 
			}
			break;

		case 90:
			oldStep = 0;
			_to = 0;
			_cnt = 0;
			rodBtnLedControl_Stop(idx);
			break;

		default:
			break;
	}
}

void rodRtBtnLedControl()
{
	static unsigned int oldStep = 0;
	static unsigned long _to = 0;
	static unsigned long _cnt = 0;
	String sendMsg;

	int idx = BTN_LED_RIGHT_IDX;	 

	switch(btn_led_step[idx])
	{
		case 0:
			break;

		case BTN_LED_CONT_START:	 
			_cnt = 0;
			btn_led_step[idx] = 20;
			break;

		case 20:		 
			_cnt++;
			if(_cnt > btn_led_cnt[idx])	 
			{
				btn_led_step[idx] = 90;
				break;
			}
			else
			{

				digitalWrite(BTN_LED_RT_BLUE_PIN, BTN_LED_ON); 	 
				setTO(_to);
				btn_led_step[idx] = 21;
				break;
			}
		case 21:		 
			if(checkTO(_to, btn_led_on_time[idx]))
			{

				digitalWrite(BTN_LED_RT_BLUE_PIN, BTN_LED_OFF); 	 
				setTO(_to);
				btn_led_step[idx] = 22;
			}
			break;
		case 22:		 
			if(checkTO(_to, btn_led_off_time[idx]))
			{
				btn_led_step[idx] = 20;		 
			}
			break;

		case 90:
			oldStep = 0;
			_to = 0;
			_cnt = 0;
			rodBtnLedControl_Stop(idx);
			break;

		default:
			break;
	}
}
