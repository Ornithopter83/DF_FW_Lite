#include "DF_Main_Application.h"
#include "DF_Main_Internal.h"
#include "modules/DF_Main_Scheduler.h"
void DF_Main_Application_Setup()
{
	curr_ms_tick = millis();
	old_ms_tick = curr_ms_tick;

	Serial1.begin(115200, SERIAL_8N1, UART1_RX_PIN, UART1_TX_PIN);		 
	Serial1.setTimeout(10);		   

	curr_ms_tick = millis();
	LogPrintln(" LG] PWRON FS INIT");

	fsInfo.init();

	String strBootInfo = fsInfo.getInfo(BOOTING_INFO);
	if (strBootInfo.substring(0,1) == "-") {
		strBootInfo = STR_BOOTINFO_DEFAULT;
	}
	g_AutoReboot  = strBootInfo.substring(0,1).toInt();

	curr_ms_tick = millis();
	LogPrintln(" LG] PWRON MAIN Ver:" + mainVer + (g_AutoReboot ? "-R" : "") + ",mode:" + String(mainMode));
	LogPrintln(" LG] BOOT INFO:" + strBootInfo + ",dlytime:"+String(DEF_PWRON_DELAY_TIME) + ",sftime:"+String(DEF_USB_SAFE_TIME) + ",Max_Priority:"+String(configMAX_PRIORITIES-1));

	curr_ms_tick = millis();
	pinMode(SUB_ACOFF_PIN, OUTPUT);
	pinMode(PC_USB5V_PIN, INPUT_PULLUP);
	pinMode(PWROFF_SWC_PIN, INPUT);			 

	int usb5v = digitalRead(PC_USB5V_PIN);
	int pwrSW = digitalRead(PWROFF_SWC_PIN);
	int	ledMd = 1;

	g_ResetReason = esp_reset_reason();		 
	LogPrintln(" LG] SUB_ACOFF_PIN:" + String(digitalRead(SUB_ACOFF_PIN)) + ",PC_USB5V_PIN:" + String(usb5v) + ",PWROFF_SWC_PIN:" + String(pwrSW) + ",esp_reset_reason:" + String(g_ResetReason));
	if ((g_ResetReason == 0) || (g_ResetReason == 3))	{	 
		SerialPortEnable();				 
		g_ProcStatus = PS_RUNNING;
	}
	else {
		if (pwrSW == LOW) {				 
			SerialPortPinInput();		 
			g_ProcStatus = PS_BOOTING;
		}
		else {
			subAC_Off();				 
			ioLedOffSetting();
			SerialPortEnable();			 
			ledMd = 0;
			g_ProcStatus = PS_INITIAL;
		}
	}

	curr_ms_tick = millis();
	LogPrintln(" LG] PWRON IO SET");

	dfConfig = setConfig();

	curr_ms_tick = millis();
	ioPinSetting();

	curr_ms_tick = millis();
	LogPrintln(" LG] PWRON nvm Init");

	eNvm.init();
	delay(50);

	String strMmotVal = fsInfo.getInfo(MAINMOT_INFO);
	short mMotVal = strMmotVal.toInt();

	if( 0 > mMotVal)	 
	{
		defaultTorqueMotor = AUTOSET_DEFAULT_TORQ;
	}
	else if(MAIN_MOT_FS_MIN_DUTY > mMotVal || MAIN_MOT_FS_MAX_DUTY < mMotVal)	 
	{
		fsInfo.saveInfo(MAINMOT_INFO, STR_AUTOSET_DEFAULT_TORQ);
		defaultTorqueMotor = AUTOSET_DEFAULT_TORQ;
	}
	else	 
	{
		defaultTorqueMotor = mMotVal;

	}

	String strBldcVal = fsInfo.getInfo(BLDC_LIMIT_INFO);
	short iBmotVal = strBldcVal.toInt();

	if( 0 > iBmotVal)	 
	{
		bldcLimitVal = BLDC_LIMIT_MIN;
	}
	else if(BLDC_LIMIT_MIN > iBmotVal || BLDC_LIMIT_MAX < iBmotVal)	 
	{

		fsInfo.saveInfo(BLDC_LIMIT_INFO, STR_BLDC_LIMIT_MIN);
		bldcLimitVal = BLDC_LIMIT_MIN;
	}
	else	 
	{
		bldcLimitVal = iBmotVal;

	}

	curr_ms_tick = millis();
	LogPrintln(" LG] PWRON enow INIT");
	eNow.init(recv_cb_esp_now_msg);   

	if(ESP_OK != esp_now_register_send_cb(sent_cb_esp_now_sts))		 
	{
		LogPrintln(" LG] Error nowSent CallBack Function");
	}

	curr_ms_tick = millis();
	LogPrintln(" LG] PWRON MAIN MOT INIT");
	torqMotor.init();				 

	curr_ms_tick = millis();
	bbnMotor.init();				 

	curr_ms_tick = millis();
	mainEnc.init();

	curr_ms_tick = millis();
    mainEnc.setRotateCallback(rotateChangeCallback);

	curr_ms_tick = millis();
	extLed.init();

	curr_ms_tick = millis();
	motor_AllOff();

	curr_ms_tick = millis();
	extLed_AllOff();

	curr_ms_tick = millis();
	ledoff_BoardLedAllOff();

	curr_ms_tick = millis();
	reelOut_AllOff();	 

	curr_ms_tick = millis();
	delay(200);		 
	int cnt = 1;
	while (cnt < 10)		 
	{
	cnt++;
	digitalWrite(BD_LED2_PIN, (cnt%2));		 
	delay(50);
	}

		digitalWrite(BD_LED4_PIN, HIGH);	 
		delay(200);		 
		digitalWrite(BD_LED4_PIN, LOW);	 

	curr_ms_tick = millis();
	sensor1ms[0].old2 = 0;
	sensor1ms[0].old = 0;
	sensor1ms[0].curr = 0;
	sensor1ms[0].lvl = 0;
	sensor1ms[0].le = 0;
	sensor1ms[0].te = 0;
	read_input_1ms();
	delay(1);
	read_input_1ms();
	delay(1);
	read_input_1ms();
	delay(1);
	read_input_1ms();
	delay(1);
	read_input_1ms();
	delay(1);

	curr_ms_tick = millis();

	sensor10ms[0].old2 = 0;
	sensor10ms[0].old = 0;
	sensor10ms[0].curr = 0;
	sensor10ms[0].lvl = 0;
	sensor10ms[0].le = 0;
	sensor10ms[0].te = 0;
	read_input_10ms();
	delay(10);
	read_input_10ms();
	delay(10);
	read_input_10ms();
	delay(10);
	read_input_10ms();
	delay(10);
	read_input_10ms();
	delay(10);

  curr_ms_tick = millis();

  curr_ms_tick = millis();

  initializeLedControlValues(ledMd);		 

	if (ledMd == 0) {
		pwrMode = PWR_OFF;			 
		extLed_AllOff();
	}

  mainMode = NORMAL_MODE;	 
  LogPrintln(" LG] PWRON **MAIN Mode= " + String(mainMode) + ",time:" + String(millis()-old_ms_tick)+"ms");

	for(int i=0; i<ENC_INTV_BUFF_SIZE; i++)
	{
		stEncIntvBuff[i].tick = 9999;
		stEncIntvBuff[i].width = 9999;
		stEncIntvBuff[i].dir = 0;
		stEncIntvBuff[i].enc = 9999;
		stEncIntvBuff[i].dist = 9999;
	}

	curr_ms_tick = lSys1MsTime;
	old_ms_tick = curr_ms_tick;		 

	pwrOn1stCmdSendFlag = 1;

	ApPrintln(STX_PWRON_STS + "11%");		   

	attachInterrupt(digitalPinToInterrupt(ENC_INTR_PIN), ISR_encIntrHandle, FALLING);

	timer = timerBegin(0, 80, true);   
	timerAttachInterrupt(timer, &ISR_onTimerHandler, true);   

	timerAlarmWrite(timer, 100, true);   
	timerAlarmEnable(timer);   

}

void DF_Main_Application_Loop()
{

  system_counter();   

  if(0 < sys_count_1ms)
  {

	portENTER_CRITICAL(&mux);   
	chk_ms_tick = lSys1MsTime;	    
	portEXIT_CRITICAL(&mux);    
	sys_count_1ms--;

	read_input_1ms();
	if(0 < sys_count_10ms)
	{
		read_input_10ms();
	}
	if(0 < sys_count_100ms)
	{
		read_input_100ms();
	}

	uartRecvParsing();	 
	uartRecvHandler();	 
	nowRecvHandler();		 

	logUartRecvParsing();
	logRecvHandler();		 

	t1ms_Process_Exec();

	if(0 < sys_count_10ms)
	{
		sys_count_10ms--;

		t10ms_Process_Exec();

	}

	if(0 < sys_count_100ms)
	{
		sys_count_100ms--;

		t100ms_Process_Exec();

		if(0 == diagOutFlag_WDToff)	 
		{
		}

	}

	if(0 < sys_count_500ms)
	{
		sys_count_500ms--;

		t500ms_Process_Exec();

		if(0 == diagOutFlag_WDToff)	 
		{
		}

	}

	if(0 < sys_count_1sec)
	{
		sys_count_1sec--;

		t1sec_Process_Exec();

	}

	unsigned long end_ms_tick;

		portENTER_CRITICAL(&mux);   
		end_ms_tick = lSys1MsTime;	    
		portEXIT_CRITICAL(&mux);    

	if (1 < (end_ms_tick-chk_ms_tick))	 
	{
		LogPrintln(" LG] 1msExe Delay: " + String(end_ms_tick-chk_ms_tick) + "ms");
	}

  }

}
