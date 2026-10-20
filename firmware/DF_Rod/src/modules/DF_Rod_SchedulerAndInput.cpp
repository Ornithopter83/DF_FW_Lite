
#include "../DF_Rod_Internal.h"
#include "DF_Rod_SchedulerAndInput.h"
void t100ms_Process()
{

	if(rodPowerOnFlag)
	{
		rodPowerOnFlag = 0;	 
		sendInfo_boardType();
	}

	if(recvInfoSeqStep)	 
	{
		sendRodInfo_forStep();
	}

	#define L_BTN_LONG_TIME	2500	 

	if(leftButtonDownCount)
	{
		leftButtonDownCount++;
		if((L_BTN_LONG_TIME/100) < leftButtonDownCount)	 
		{

			rodRegistrationMode = 55;		 

			leftButtonDownCount = 0;

			eNow.addPeerAddress(broad_cast_addr);

			String rodAddr = eNow.getMyAddress();

			eNow.writeBC(DF_Protocol_RodToMain_RodAddress, rodAddr); 	 

			startRodRegistration();

			AddrWrite_LedBlink = 1;	 

		}
	}

	#define ROD_REGI_LED_BLINK_TIME	1000	 

	if(AddrWrite_LedBlink)
	{
		led1GreenOutputStatus = !led1GreenOutputStatus;
		digitalWrite(LED1_GRN_PIN, led1GreenOutputStatus); 	 
		AddrWrite_LedBlink++;
 		if((ROD_REGI_LED_BLINK_TIME/100) < AddrWrite_LedBlink)
		{

			AddrWrite_LedBlink = 0;
		}		 

	}

	if(rodRegistrationMode)
	{
		processRodRegistration();
	}

}

void t500ms_Process()
{

	battery.scanAvgInteg();

}

void t1sec_Process()
{

	rodAliveRecvTOCheck();

	imuConnectCheck();

	battChangeCheckSend();

	batteryDiagnosticSend();

	rodLed1SecControl();

}

void rodLed1SecControl()
{
	if(rodAliveRcv_LedBlinkFlag)
	{
		if(!AddrWrite_LedBlink)
		{
			led1GreenOutputStatus = !led1GreenOutputStatus;
			digitalWrite(LED1_GRN_PIN, led1GreenOutputStatus); 	 
		}
		led2RedOutputStatus = !led2RedOutputStatus;
		digitalWrite(LED2_RED_PIN, led2RedOutputStatus); 	 
	}
	else
	{
		if(!AddrWrite_LedBlink)
		{
			led1GreenOutputStatus = HIGH;
			digitalWrite(LED1_GRN_PIN, led1GreenOutputStatus); 	 
		}
		led2RedOutputStatus = LOW;
		digitalWrite(LED2_RED_PIN, led2RedOutputStatus);	 
	}
}

unsigned int system_delay_cnt = 0;

void run_timer()
{
	unsigned int add_tick = 0;

	curr_ms_tick = millis();
	if(oldcurr_ms_tick != curr_ms_tick)
	{

		if(curr_ms_tick > oldcurr_ms_tick)
		{
			add_tick = (int)(curr_ms_tick - oldcurr_ms_tick);
		}
		else
		{
			add_tick = (int)((0xFFFFFFFF - oldcurr_ms_tick) + 1 + curr_ms_tick);
		}

		sys_1ms_cnt += add_tick;		 

		run_time_1ms += add_tick;

gotoSystemTimer:

		if( 9 < run_time_1ms)	 
		{

			run_time_1ms -= 10;

			sys_10ms_cnt++;

			run_time_10ms++;
			if( 9 < run_time_10ms)	 
			{
				run_time_10ms -= 10;

				sys_100ms_cnt++;

				run_time_100ms++;

				if(!(run_time_100ms % 5))		 
				{
					sys_500ms_cnt++;
				}

				if( 9 < run_time_100ms)	 
				{
					run_time_100ms -= 10;

					sys_1sec_cnt++;		 

				}

			}
		}

		if(9 < run_time_1ms)	 
		{
			goto gotoSystemTimer;
		}

		if(2 < add_tick)
		{

			system_delay_cnt++;
			LogPrintln(" LG] SYSTIK ROD Delay " + String(add_tick) + "ms ,cnt:" + String(system_delay_cnt));
		}

		oldcurr_ms_tick = curr_ms_tick;

	}

}

void ioPinSetting()
{

	pinMode(SETUP_MODE_PIN, INPUT);

	pinMode(LED1_GRN_PIN, OUTPUT);
	pinMode(LED2_RED_PIN, OUTPUT);

	pinMode(BTN_LED_LF_RED_PIN, OUTPUT);
	pinMode(BTN_LED_RT_BLUE_PIN, OUTPUT);

	pinMode(VRT_MOT_ON_PIN, OUTPUT);
	digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);

}

void checkFileSystem()
{

	if (!SPIFFS.begin())		 
	{
			LogPrintln(" LG] FS Format Start");

    	SPIFFS.format();	 

		delay(10);		 

		ESP_Soft_Reset();
	}

}

static int pwrOn1st_loop = 0;
int resp_imu = 0;
