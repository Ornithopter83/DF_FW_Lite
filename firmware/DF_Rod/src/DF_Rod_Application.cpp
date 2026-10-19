#include "DF_Rod_Application.h"
#include "DF_Rod_Internal.h"
#include "modules/DF_Rod_SchedulerAndInput.h"

void DF_Rod_Application_Setup()
{

	 
	curr_ms_tick = millis();

	 
	Serial.begin(115200);
	Serial.setTimeout(10);
	 

	DbgSerial.begin(115200, SERIAL_8N1, DEBUG_RX_PIN, DEBUG_TX_PIN);		 
	DbgSerial.setTimeout(10);		   

	 
	settingIOtype();	 

	 
	 
	ioPinSetting();

	 
	setSlaveMode();		 

	 
	 
	int cnt = 0;
	while (cnt < 6)		 
	{
	  cnt++;
	  led1Grn_OutSts = !led1Grn_OutSts;
	  digitalWrite(LED1_GRN_PIN, led1Grn_OutSts);
	  led2Red_OutSts = !led2Red_OutSts;
	  digitalWrite(LED2_RED_PIN, led2Red_OutSts);
	  delay(50);	 
	}

	 
	 
	led1Grn_OutSts = LOW;
	digitalWrite(LED1_GRN_PIN, led1Grn_OutSts);
	led2Red_OutSts = LOW;
	digitalWrite(LED2_RED_PIN, led2Red_OutSts);

	 
	btnLFLedRed_OutSts = BTN_LED_OFF;		 
	digitalWrite(BTN_LED_LF_RED_PIN, btnLFLedRed_OutSts);
	btnRTLedBlue_OutSts = BTN_LED_OFF;		 
	digitalWrite(BTN_LED_RT_BLUE_PIN, btnRTLedBlue_OutSts);


	LogPrintln(" LG] VER:" + String(rodVer));

	 
	checkFileSystem();		 

	 
	eNow.bindTargetAddress();	 

	 
	imuSerial.begin(115200, SERIAL_8N1, IMU_RX, IMU_TX);	 

	 
	if(0 != imu.checkSetupFileVal())	 
	{
		 
			LogPrintln(" LG] IMU OUT AUTO_SET Start");
		imu.autoSetup();
		 
		short len = imu.saveInterval(IMU_INTV_DEFAULT_TIME);	 
		 
		 
	}
	else
	{
	}

	 

	 
	sBtn.init();	 
	imu.init();		 

	 
	if(0) {   }

	 
	 
	if (isSlaveSetupMode())			 
	{
		 
		imu.setStartStop(IMU_CONF_STOP);			 
		flagIMUSTOP = 1;

		 
		led2Red_OutSts = HIGH;
		digitalWrite(LED2_RED_PIN, led2Red_OutSts);		 
	}

	 
	else
	{
		 
		eNow.init(recv_cb_esp_now_msg);
		if( ESP_OK != esp_now_register_send_cb(sent_cb_esp_now_sts))
		{
			LogPrintln(" LG] Error NOW SENT_CB_REGIST NG");
		}

		encoder.init();
		battery.init();
		 

		 
		encoder.setRotateCallback(rotateChangeCallback);
		encoder.setCalibrationCallback(encoderCalibrationChangeCallback);
		sBtn.setSwitchCallback(btnChangeCallback);
		imu.setStateCallback(imuDataCallback);

		imu.setStartStop(IMU_CONF_START);			 

		led1Grn_OutSts = HIGH;
		digitalWrite(LED1_GRN_PIN, led1Grn_OutSts);

	}

	 
	rodPowerOnFlag = 1;		 
	 
	 
	system_delay_cnt = 0;

	LogPrintln(" LG] PWRON_ time:" + String(millis()-curr_ms_tick));

	 
	curr_ms_tick = millis();
	oldcurr_ms_tick = curr_ms_tick;

}

void DF_Rod_Application_Loop()
{

	 
	run_timer();			 
	 

	 
	 
	 
	if (isSlaveSetupMode())
	{

		if(0 < sys_1ms_cnt)
		{
			sys_1ms_cnt--;

			if(0 < sys_10ms_cnt)
			{
				sys_10ms_cnt--;

				 
				 
				uartPcHandler_Setup();			 
				 
				resp_imu = imu.uartRecv_Setup(); 		 
				 
			}
			if(0 < sys_100ms_cnt)	{ sys_100ms_cnt--; }
			if(0 < sys_500ms_cnt)	{ sys_500ms_cnt--; }
			if(0 < sys_1sec_cnt)	{ sys_1sec_cnt--; }
		}

	}

	 
	else
	{
		 
		if(0 < sys_1ms_cnt)
		{
			chk_ms_tick = millis(); 	 

			sys_1ms_cnt--;		 

			 
			 

			 
			nowRecvHandler();			 
			 
			uartPcHandler_NormalMode();

			 
			resp_imu = imu.uartRecv_Normal();			 


			 
			encoder.rotate();		 
			 
			sBtn.toggle();			 

			 
			 
			 

			if(0 < sys_10ms_cnt)
			{
				sys_10ms_cnt--;
				t10ms_Process();	 
			}

			if(0 < sys_100ms_cnt)
			{
				sys_100ms_cnt--;
				t100ms_Process();	 
			}

			if(0 < sys_500ms_cnt)
			{
				sys_500ms_cnt--;
				t500ms_Process();	 
			}

			if(0 < sys_1sec_cnt)
			{
				sys_1sec_cnt--;
				t1sec_Process();
				 
				 
				 
				 
			}

			 
				if(1 < (millis()-chk_ms_tick))
				{
					LogPrintln(" LG] 1msExe Delay:" + String(millis()-chk_ms_tick)+" ms");
				}
		}
		 
		 
	}
}
