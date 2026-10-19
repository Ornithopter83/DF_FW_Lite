 
#include <Arduino.h>
#include "Common.h"
#include "SwitchButton.h"
#include "Encoder.h"
#include "ENow.h"
#include "Potentiometer.h"
#include "EB_IMU.h"
#include "Battery.h"
#include "Version.h"

 
ENow eNow;
Encoder encoder;
SwitchButton sBtn;
Potentiometer brkMotor;
EB_IMU imu;
Battery battery;

 

 
#define STX_ENB_DIS     	"$01"
#define STX_DEV_INFO_REQ    "$01"	 
#define STX_BREAK_MOTOR 	"$05"
#define STX_SWITCH      	"$06"
#define STX_ROTARY      	"$07"
#define DEF_STX_IMU_SETUP   "$08"

String STX_IMU_SETUP =  	"$08";

	String STX_VERSION_READ = "$10";

	String STX_GET_ADDR   = "$16";
	String STX_SET_ADDR   = "$17";
	String STX_IMU_SETSTRING   = "$91";

 
String STX_SETUP_ENTRY 	  = "$9001";		 
String STX_SETUP_EXIT 	  = "$9002";		 

String STX_IMU_INTVAL_CHK = "$9010";		 

String STX_SET_FILE	= "$F0";		 

String STX_SET_DATA	= "$F6";		 

 
#define RESP_STX_IMU_CONN "$05"
#define RESP_STX_IMU_CONN_OK "$0511111111%"
#define RESP_STX_IMU_CONN_NG "$0500000000%"
#define RESP_STX_IMU_CONN_NG_SLEEP "$0500000009%"

#define RESP_STX_SLEEP_ENT "009"


 
#define GAME_ENABLE     "11111111"
#define GAME_DISABLE    "00000000"

 
#define RUN_TIMEOUT_ALIVE_CNT   10 		 
#define RUN_TIMEOUT_SLEEP_CNT   15 		 

unsigned short apType = AP_IS_DF;
unsigned short dfRodAdcDiagnosticEnabled = 0;


 
 
unsigned long curr_ms_tick = 0;
unsigned long oldcurr_ms_tick = 0;

unsigned long curr_us_tick = 0;

 
unsigned long chk_ms_tick = 0;			 

 
unsigned int run_time_1ms = 0;
unsigned int run_time_10ms = 0;
unsigned int run_time_100ms = 0;
 
unsigned int run_time_1sec = 0;

unsigned int sys_1ms_cnt = 0;				 
unsigned int sys_10ms_cnt = 0;				 
unsigned int sys_100ms_cnt = 0;				 
unsigned int sys_500ms_cnt = 0;				 
unsigned int sys_1sec_cnt = 0;				 

 


 
#define RUN_TIME_10MS_TICK		10		 
#define RUN_TIME_100MS_TICK		100		 
#define RUN_TIME_1SEC_TICK		1000		 

 

 
unsigned short led1Grn_OutSts = 0;
unsigned short led2Red_OutSts = 0;

unsigned short btnLFLedRed_OutSts = BTN_LED_OFF;
unsigned short btnRTLedBlue_OutSts = BTN_LED_OFF;

 

 
unsigned int rodPowerOnFlag = 0;		 

 
int rod_Alive_recv_over_cnt = 0;				 
int run_time_sleep_over_cnt = 0;

now_message now_msg;

 
bool isEnable = false;
bool isSetupMode = false;

 
#define MAIN_STS_UNKNOWN	-1
#define MAIN_STS_CONN		0
#define MAIN_STS_DISCONN	1
#define MAIN_STS_PWRON		2	 


int mainStatus = UNKNOWN;
int oldmainStatus = UNKNOWN;
int mainPollingTOcnt = 0;

int rodSelfStatus = UNKNOWN;
int oldrodSelfStatus = UNKNOWN;
int rodPollingTOcnt = 0;

 
#define IMU_UNKNOWN_2  -2
#define IMU_UNKNOWN  -1
#define IMU_CONN      0		 
#define IMU_DISCONN   1		 


int imuStatus = UNKNOWN;			 
int oldimuStatus = UNKNOWN;		 
int imuPollingTOcnt = 0;

 
#define IMU_CONN_TIMEOUT_CNT   5  

unsigned short fRecvImuData = 0;			 
unsigned short flagIMUSTOP = 0;			 

 
#define NORMAL_MODE 	0
#define SETUP_MODE  	1		 
int rodMode = NORMAL_MODE;


String L_BTN_UP = "$0600000001%";		 
String L_BTN_DN = "$0600000010%";		 
String R_BTN_UP = "$0600000100%";		 
String R_BTN_DN = "$0600001000%";		 

String ROD_ENC_CNT = "$0700000+";
 
unsigned short recvInfoSeqStep = 0;	 


 
unsigned short imuIntervalCheckCnt = 0;
unsigned long measureCnt = 0;


	extern const int rodBoardType = NEW_BOARD_2;		 

extern const int buttonType = BUTTON_V2;
extern const int breakType = BREAK_NO;
extern const int battType = BATT_800_V2;
extern const int reelEncType = REEL_ENC_V3;

 
unsigned int rodRegistMode = 0;		 

 
unsigned int vrt_cont_flag = 0;
unsigned int vrt_ap_req_cnt = 6;
unsigned int vrt_ap_req_on_time = 100;
unsigned int vrt_ap_req_off_time = 50;
 
#define BTN_LED_MAX_SIZE 2
unsigned int btn_led_flag[BTN_LED_MAX_SIZE] = {0,0};
unsigned int btn_led_ap_req_cnt[BTN_LED_MAX_SIZE] = {6,6};
unsigned int btn_led_ap_req_on_time[BTN_LED_MAX_SIZE] = {50,50};
unsigned int btn_led_ap_req_off_time[BTN_LED_MAX_SIZE] = {50,50};


 
extern uint8_t main_board_addr[];
extern uint8_t broad_cast_addr[];


 
 
 
 
String zeroFill4Char(unsigned int _val);
void btnChangeCallback(String str);
void rotateChangeCallback(String str);
void imuDataCallback(String pid, String str);
void sendInfo_boardType();
void imuConnResp();
void nowRecvHandler();
void anaReelDeviceControl(int act, String msg);
void anaVrtMotMainCmd(String msg);
void anaBtnLedMainCmd(String msg);
void rodVrtControl_Start(int cnt, unsigned int ontime, unsigned int offtime);
void rodVrtControl_Stop();
void rodVrtControl();
void rodBtnLedControl_Start(int idx, unsigned int cnt, unsigned int ontime, unsigned int offtime);
void rodBtnLedControl_Stop(int idx);
void rodLfBtnLedControl();
void rodRtBtnLedControl();
void uartPcHandler_NormalMode();
int targetAddrWrite(String msg);
void uartPcHandler_Setup();
void setRecvImuData();
int isRodNewBoard();
int setBoardType();
int getButtonType();
int getBreakType();
int getEncType();
int getBattType();
void settingIOtype();
void setSlaveMode();
int readSlaveMode();
int isSlaveSetupMode();
void Entry_SetupMode();
void Exit_SetupMode();
void ESP_Soft_Reset();
void battChangeCheckSend();
void rodAliveRecvTOCheck();
void sleepEnteranceCheck();
void imuConnectCheck();
void t1ms_Process();
void t10ms_Process();
void sendRodInfo_forStep();
void rodRegistToStop();
void rodRegistToStart();
void rodRegi3secToStart();
void rodRegistToControl();
void rodRegistCancel();
void t100ms_Process();
void t500ms_Process();
void t1sec_Process();
void rodLed1SecControl();
void run_timer();
void ioPinSetting();
void checkFileSystem();
void DF_Rod_Application_Setup();
void DF_Rod_Application_Loop();




String zeroFill4Char(unsigned int _val)
{
	 
 	if(9999 < _val) { _val = 9999; }
	
	 
  if(0) {}	 
  else if (  10 > _val) { return "000" + String(_val); }
  else if ( 100 > _val) { return "00" + String(_val); }
  else if (1000 > _val) { return "0" + String(_val); }
  else 					{ return String(_val); }

}

 
 



