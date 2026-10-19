
#include <Arduino.h>

#include "Common.h"
#include "DF_Main.h"
#include "FileSys.h"
#include "Eeprom.h"
#include "ENow.h"
#include "ServoMotor.h"
#include "Potentiometer.h"
#include "BobbinMotor.h"
#include "Encoder.h"
#include "Led.h"
#include "Diag.h"
#include "Version.h"
#include <Update.h>
#include <FreeRTOSConfig.h>

String strMainBoard[10] =
{
	"",				 
	"MBD_V10",		 
	"MBD_V11",		 
};

typedef struct _ledCont
{
	unsigned int oldcont;			 
	unsigned int cont;			 
	unsigned int colorNo;		 
	unsigned int colorMany;		 
	unsigned int bTime;			 
	unsigned int dTime;			 
	unsigned int bStep;			 
	unsigned int dStep;			 
	unsigned int dColorCnt;		 
	unsigned int dReqCnt;		 
	unsigned int rsv_e;
}stLedCont;
typedef stLedCont *ptrLedCont;

#define LED_CONTROL_10MS	10
#define LED_CONTROL_20MS	20
#define LED_CONTROL_50MS	50

extern const stLedCont initledCont[LED_IDX_MAX] =
{
	{0, },								 

	{ 3, 3, 	3, 		6, 		500,	3000, 	0,	0,	1,	(3000/2/LED_CONTROL_10MS), 0	},	 
	{ 1, 1, 	3, 		1, 		500, 	3000, 	0,	0,	3,	(3000/2/LED_CONTROL_10MS), 0	},	 
	{ 0, 0, 	3, 		1, 		500, 	3000,	0,	0,	0,	(3000/2/LED_CONTROL_10MS), 0	},	 
	{ 0, 0, 	3, 		1, 		500, 	3000, 	0,	0,	0,	(3000/2/LED_CONTROL_10MS), 0	},	 
};

stLedCont ledCont[LED_IDX_MAX];

unsigned int ledLight=100;				 

unsigned int ledDimmHoldTime[LED_IDX_MAX] = 	 
{ 
	10,		 

	10,		 
	10,		 
	10,		 
	10,		 
};

#define COLOR_ORDER	10
unsigned int ledColorOrder[LED_IDX_MAX][COLOR_ORDER] = 
{
	{0,},

	{0,1,2,3,4,5,6,7,8,9},		 
	{0,1,2,3,4,5,6,7,8,9},		 
	{0,1,2,3,4,5,6,7,8,9},		 
	{0,1,2,3,4,5,6,7,8,9},		 
};

typedef struct _diagCode
{
	int catNo;			 
	int itemNo;			 
	int act;			 
	int para;			 
}stDiagCode;
typedef stDiagCode *ptrDiagCode;

ENow eNow;
Potentiometer torqMotor;		 

ServoMotor sMotor;				 

BobbinMotor bbnMotor;					 
Encoder mainEnc;				 

Led extLed;		 

Eeprom eNvm;

FileSys fsInfo;	 

String eulerStr;

String battLvlStr = "-1";			 

int ibatteryLvl = UNKNOWN;			 
int oldibatteryLvl = UNKNOWN;		 
int ibatChargeLvl = UNKNOWN;		 

#define ERR_OK		0
#define ERR_CW		-1
#define ERR_CCW		-2

#define ERR_NG			-1
#define ERR_FAIL			ERR_NG
#define ERR_RETRY_OVER		ERR_NG
#define ERR_WIRE_BROKEN		ERR_NG

#define ERR_RANGE_OVER	-2
#define ERR_ENC_NOT_STABLE	ERR_RANGE_OVER

#define ERR_PARA_NG		-3

#define ERR_TO_OVER		-4
#define ERR_NOT_SIG_CHG	-5
#define ERR_NOT_ROTATE	-6
#define ERR_FS_WRITE	-7
#define ERR_FW_CAL_NG	-8

#define STR_ERR_OK			"00"
#define STR_ERR_NG			"-1"
#define STR_ERR_FAIL		STR_ERR_NG
#define STR_ERR_RANGE_OVER	"-2"
#define STR_ERR_PARA_NG		"-3"

String STX_MAINALIVE       = "$00";

String STX_INFO_REQ        = "$01";
	#define MAIN_IF_VER			"00"
	#define NUM_MAIN_IF_VER		0
	#define ROD_BOARD_TYPE		"01"
	#define NUM_ROD_BOARD_TYPE 	1
	#define ROD_REGIST_TO_SEC	"02"	 
	#define NUM_ROD_REGIST_TO_SEC	2
	#define IMU_INTVAL_TIME		"03"	 
	#define NUM_IMU_INTVAL_TIME	3

	#define MANUF_INFO_READ		"04"
	#define NUM_MANUF_INFO_READ	4
	#define MANUF_INFO_SAVE 	"05"
	#define NUM_MANUF_INFO_SAVE	5
	#define MANUF_INFO_DEL	 	"06"
	#define NUM_MANUF_INFO_DEL	6

	#define INSTALL_INFO_READ 		"07"
	#define NUM_INSTALL_INFO_READ 		7
	#define INSTALL_INFO_SAVE 		"08"
	#define NUM_INSTALL_INFO_SAVE 		8
	#define INSTALL_INFO_DEL		"09"
	#define NUM_INSTALL_INFO_DEL		9

	#define MAINMOT_INFO_READ		"10"			 
	#define NUM_MAINMOT_INFO_READ		10
	#define MAINMOT_INFO_SAVE		"11"			 
	#define NUM_MAINMOT_INFO_SAVE		11

	#define MAIN_BOARD_TYPE		"12"			 
	#define NUM_MAIN_BOARD_TYPE 	12

	#define BLDC_MOT_LIMIT_READ			"13"			 
	#define NUM_BLDC_MOT_LIMIT_READ		13
	#define BLDC_MOT_LIMIT_WRITE		"14"			 
	#define NUM_BLDC_MOT_LIMIT_WRITE	14

String STX_TORQUE_MOTOR   = "$02";
String STX_SERVO_MOTOR    = "$03";

String STX_BBN_MOTOR     = "$04";

String STX_BREAK_MOTOR    = "$05";
String STX_LED_CONT			= "$06";

String STX_IMU_SET			= "$08";

String STX_IMU_DATA_STS		= "$09";

String STX_VER_READ       = "$10";		 
String STX_GAME_STS       = "$11";		 
String OLD_STX_BAT_LVL    = "$0C";		 
String STX_BAT_LVL        = "$12";		 
String STX_BUTT_SWC       = "$13";		 
String STX_HAND_ENC       = "$14";		 

String STX_DEV_CONT_REQ   = "$15";
String STX_ENCODER_CALIBRATION = "$37";
String STX_GET_ADDR       = "$16";
String STX_SET_ADDR       = "$17";
String STX_APSTS_RESP     = "$18";
String STX_MOT_OUT_RATE_SET   = "$19";

String STX_ROD_CONN			="$20";
String STX_IMU_CONN			= "$21";
String STX_PWRON_STS        = "$22";			 
	#define CONT_MAIN_BOARD	1
	#define REEL_MAIN_BOARD 2

String STX_VRT_MOT_CONT      = "$23";			 
String STX_BTN_LED_CONT      = "$24";			 
	#define BTN_LED_ALL_POSI	0
	#define BTN_LED_LEFT_POSI	1
	#define BTN_LED_RIGHT_POSI	2
	#define BTN_LED_LAST_POSI	BTN_LED_RIGHT_POSI		 

String STX_GAME_STS_WAT     = "$25";		 
	#define GAME_xxx	0

String STX_DEV_SET_REQ     = "$26";		 
	#define STR_MAINMOT_MINSET_REQ		"01"		 

String STX_TM_INPUT_REQ     = "$27";		 
String STX_TM_OUTPUT_REQ     = "$28";		 

String STX_AP_INFO_SEND     = "$29";		 
	#define DEF_AP_TYPE		1			 

String STX_ROD_REGIST		="$30";			 

	#define ROD_REGI_AP_ENTRY	1
	#define ROD_REGI_AP_CANCEL	2

	#define ROD_REGI_FW_CANCEL		"03%"
	#define ROD_REGI_WRITE_START	"04%"
	#define ROD_REGI_FINISH			"05%"

String STX_DEV_CHK_ERR_LOG	= "$89";		 

String STX_DEV_FUNCTION_REQ	= "$98";		 
	#define STR_WIRE_AGING_TEST		"01"
	#define NUM_WIRE_AGING_TEST		01

String STX_DATA_PARA_SET	= "$99";		 

String STX_DIAG_INPUT		= "$DI";		 
String STX_DIAG_OUTPUT		= "$DO";		 
String STX_DIAG_FUNC		= "$DF";		 
String STX_DIAG_SET			= "$DS";		 
String STX_DIAG_EXIT		= "$DE";		 

String STX_WAVE_SET       = "$F0";		 
String STX_BITE_SET       = "$F1";		 
String STX_HIT_SET        = "$F2";		 
String STX_BREAK_SET      = "$F3";		 

String STX_TOKTOK_SET     = "$F4";		 
String STX_EXTLED_SET     = "$F5";		 

String STX_RANDING_START  = "$F6";		 
String STX_TORQ_ADJ_SET   = "$F8";		 

String STX_TORQ_TUN_SET   = "$F9";		 
String STX_LED_SET		  = "$FA";

String STX_ENB_SET        = "$FF";		 

String STX_TEST_EXEC      = "$TE";		 

String OLD_RESP_STX_BUTTON      = "$06";
String OLD_RESP_STX_ENCODER     = "$07";
String RESP_STX_BUTTON          = "$13";
String RESP_STX_ENCODER         = "$14";
String STS_APSTS_REQ         	= "$18";

String RESP_STX_ROD_CONN		= "$20";

String RESP_STX_IMU_CONNECT     = "$21";
String RESP_STX_BATTERY_DIAG    = "$31";
	#define RESP_IMU_FULLSTR_CONN		"$2101%"
	#define RESP_IMU_FULLSTR_DISCONN	"$2100%"

String STS_PWRON_STS         	= "$22";			 

#define STX_CMD_MAINALIVE			0
#define STX_CMD_INFO_REQ			1
#define STX_CMD_TORQUE_MOTOR		2
#define STX_CMD_SERVO_MOTOR			3
#define STX_CMD_BLDC_MOTOR			4
#define STX_CMD_LED_CONT			6		 
#define STX_CMD_IMU_SET				8
#define STX_STS_IMU_DATA_STS		9	 

#define STX_CMD_VER_READ			10
#define STX_CMD_GAME_STS			11
#define STX_STS_BAT_LVL				12	 
#define STX_STS_BUTT_SWC			13	 
#define STX_STS_HAND_ENC			14	 

#define STX_CMD_DEV_CONT_REQ		15
#define STX_CMD_GET_ADDR			16
#define STX_CMD_SET_ADDR			17
#define STX_CMD_APSTS_RESP			18		 
#define STS_CMD_APSTS_REQ			18		 
#define STX_CMD_MOT_OUT_RATE_SET	19

#define STX_STS_ROD_CONN			20	 
#define STX_STS_IMU_CONN			21	 
#define STX_STS_PWRON_STS			22		 

#define STX_CMD_VRT_MOT_CONT		23
#define STX_CMD_BTN_LED_CONT		24

#define STX_CMD_ROD_REGIST			30		 
#define STX_STS_ROD_REGIST			30		 

#define ROD_REGI_CANCEL_TO		(25*1000)	 
#define ROD_REGI_END_TO_1ST		(1500)	 
#define ROD_REGI_END_TO_2ND		(4000)	 

#define DEF_STX_GET_ADDR     16

String STATE_DISABLE   = "00000000";		 
String STATE_WAITING   = "00000000";		 
String STATE_ENABLE    = "11111111";		 
String STATE_READY     = "22222222";		 

#define STATE_CONN_UNKNOWN     "-1";		 

#define STATE_DISCONN          "00";
#define STATE_CONNECTED        "01";
#define STATE_DISCONN_NOTINIT  "11";
#define STATE_DISCONN_NOTPEER  "12";
#define STATE_DISCONN_SLEEP    "88";
#define STATE_DISCONN_MAC      "99";

#define IMU_CONN_2CHAR_INT 11

bool isGameEnable = false;				 

#define STR_BOOTINFO_DEFAULT	"06028"	 
#define	DEF_PWRON_DELAY_TIME	60
#define	DEF_PWRON_DELAY_TMAX	99
#define	DEF_PWRON_DELAY_TMIN	10
#define	DEF_USB_SAFE_TIME		28

unsigned long	g_RecvCmdCount=0;		 
unsigned long	g_LastCmdTick;			 
int				g_SerialEnable=0;		 
int				g_ResetReason=-1;		 
int				g_AutoReboot=0;			 

unsigned short mainBoardType = MAIN_BOARD_V10;	 
unsigned short mainBoardTypedData = 0;	 

#define ROD_BOARD_V1	0
#define ROD_BOARD_V2	1
unsigned short rodBoardType = ROD_BOARD_V2;	 

String strRodBoardType = "001";		 

int ap_conn_status = UNKNOWN;
int old_ap_conn_status = UNKNOWN;
unsigned int apAliveConn_NotRecvCnt = 0;		 
unsigned int apAliveElse_NotRecvCnt = 0;		 

int rod_conn_status = ROD_UNKNOWN;
int old_rod_conn_status = ROD_UNKNOWN;

String g_RespVersion = "";						 

bool isPowerOnReady = false;

unsigned long curr_ms_tick = 0;			 
unsigned long old_ms_tick = 0;			 

unsigned long chk_ms_tick = 0;			 

unsigned int count_1ms = 0;
unsigned int count_10ms = 0;
unsigned int count_100ms = 0;
unsigned int count_500ms = 0;
unsigned int count_1sec = 0;

unsigned int sys_count_1ms = 0;
unsigned int sys_count_10ms = 0;
unsigned int sys_count_100ms = 0;
unsigned int sys_count_500ms = 0;
unsigned int sys_count_1sec = 0;
unsigned int sys_count_5sec = 0;

#define TO_1MS_TICK 1
#define TO_10MS_TICK 10
#define TO_100MS_TICK 100

#define setTO(var) (var = curr_ms_tick)				 
#define getTO(var) ( (curr_ms_tick < var)? (0xFFFFFFFF - var + curr_ms_tick + 1 ) :  (curr_ms_tick - var) )
#define checkTO(var,TO) ( getTO(var) > TO)	 

#define setLmEnc(var) (var = mainEnc.lmCnt)
#define checkLmEnc(var,TO) (TO < (mainEnc.lmCnt - var))

int reqServoMotor = 2;
int oldServoMotor = -1;
int absReqServoMotor = 2;
int oldAbsServoMotor = -1;

short recv_TorqMotor_Flag = 0;	 
int reqTorqueMotor = DEFAULT_TORQ;  
int oldTorqueMotor = DEFAULT_TORQ;  
int reqBreakMotor = 0;
int oldBreakMotor = 0;
long reqImuMeasFlag = 0;			 
long oldImuMeasFlag = 0;

short defaultTorqueMotor = DEFAULT_TORQ;
short bldcLimitVal = BLDC_LIMIT_MIN;

int cntRightButtonOn = 0;		 
int cntRightButtonOff = 0;		 
int rightBtnOnCount = 0;		 

int exeFastHoldOn = 0;
int exeFastHoldOff = 0;
int exeButtonCombo = 0;
int exeButtonComboOff = 0;

int execContinue2Ready = 0;

int exeResistServoENB = 0;		 
int exeHoldonServoENB = 1;		 

int biteLogOut = 1;

#define WAVE_NOT_CONT	0
#define WAVE_WEAK		1
#define WAVE_NORMAL		2
#define WAVE_STRONG		3
#define WAVE_MAX		4

typedef struct _stWaveServoSet
{
	int angle1;		 
	int angle2;		 
	int interval;
}stWaveServoSet;

stWaveServoSet stWaveServoVal = { -10, 10, 1500 };

#define WAVE_PWR_MAX	(1+12)		 

typedef struct _stWaveTorqSet
{
	int rtTorq;			 
	int tgTorq;			 
	int offInterval;
	int onInterval;

	unsigned int pwr[WAVE_PWR_MAX];
}stWaveTorqSet;

typedef stWaveTorqSet *stWavePtr;

stWaveTorqSet stWaveTorqVal = { 
	DEFAULT_TORQ-20, DEFAULT_TORQ, 900, 1100,		 

	0,							 
	DEFAULT_TORQ-4, 			 
	DEFAULT_TORQ,
	DEFAULT_TORQ+4, 
	DEFAULT_TORQ+(4*2), 
	DEFAULT_TORQ+(4*3),			 
	76, 
	80, 
	80, 
	80, 
	80,							 
	90, 
	90																		 
	};

#define DEFAULT_BBN		9		 

stWaveTorqSet stWaveBdutyVal = {
	9, 9 , 400, 1700, 				 

	0,							 
	DEFAULT_BBN,			 
	DEFAULT_BBN+(2*1), 
	DEFAULT_BBN+(2*2),
	DEFAULT_BBN+(2*3),
	DEFAULT_BBN+(2*4),		 
	15, 15, 15, 15, 15,		 
	15, 15					 
	};	 

typedef struct _stWavePower
{
	unsigned int pwr[WAVE_PWR_MAX];
}stWavePower;
typedef stWavePower *stWavePwrPtr;
stWavePower stWavePwrBbn = {
	0, 			 

	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*0),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*1),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*2),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*3),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*4),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  

	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  
	DEFAULT_BBN_PWR+(WAVE_ADD_BBN_PWR*5),  
	};

int biteTestCase = 2;		 
int biteTorqCase = 3;	 
int biteCnt = 3;		 
int biteType = 3;		 

typedef struct _biteTorqSet
{
	int targetTorq;
	int targetInterval;
	int incTorq;
	int incInterval;
	int decTorq;
	int decInterval;

	int shallowTorq;
	int shallowInterval;
	int deepTorq;
	int deepInterval;

  	int torqType[1+8];	 
}biteTorqSet;

#define DEEP_ADD_TORQ	25
#define DEEP_STEP_TORQ	6

biteTorqSet stBiteTorqVal = 
{ 

	DEFAULT_TORQ+30, 700,	  
	 15, 200,	 
	 15, 200,	 

	DEFAULT_TORQ+20, 500,	 

	DEFAULT_TORQ+40, 1500,	 

	DEFAULT_TORQ,			 
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*0),	 
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*1), 
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*2), 
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*3),
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*4),
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*5),
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*6),
	DEFAULT_TORQ+DEEP_ADD_TORQ+(DEEP_STEP_TORQ*7)	 
};

#define START_TORQ  DEFAULT_TORQ+10  
#define END_TORQ    DEFAULT_TORQ  

#define OFF_TORQ	(DEFAULT_TORQ-20)

#define WF_END_TORQ    DEFAULT_TORQ+20		 

#define HIT_LOW_TORQ    DEFAULT_TORQ+20		 
#define HIT_END_TORQ    DEFAULT_TORQ+20		 

#define BITE_WAIT_TIME	300  

int hitServoAngle = 10;			 
int hitServoInterval = 200;		 

int torqResistFix = 0;			 

typedef struct _hitTorqSet
{
	int targetTorq;
	int targetInterval;

	int holdonTorq;
	int holdonInterval;

	int lowTorq;
	int lowInterval;

	int midTorq;
	int midInterval;

	int torqType[1+12];		 
} hitTorqSet;

#define HIT_TORQ_ADD_VAL 20
#define HIT_TORQ_STEP_VAL 3

hitTorqSet stHitTorqVal = 
{ 
	 DEFAULT_TORQ+10, 1500,		 

	 DEFAULT_TORQ+20, 1000,		 

	 DEFAULT_TORQ,	  1000,		 
	 DEFAULT_TORQ+15, 1500,		 

	0,				 
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*0),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*1),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*2),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*3),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*4),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*5),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*6),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*7),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*8),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*9),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*10),
	DEFAULT_TORQ + (HIT_TORQ_STEP_VAL*11),		

};

#define HIT_WAIT_TIME	50  

typedef struct _stBreakSet
{
	int val[9];
}stBreakSet;

stBreakSet stBreakVal[1+3] =
{

	{0, },											 
	{0,     215, 220, 225, 230, 235, 240, 245, 245 }, 	 
	{0,		205, 210, 215, 220, 225, 230, 235, 235 },		 
	{0,		195, 200, 205, 210, 215, 220, 225, 225 }		 
};

int breakType = 1;		 
int targetBreakVal = 200;	 
int outBreakVal = 100;		 

int decBreakCnt = 0;

long lpCnt = 0;

int DiagInMonitor = 0;

int diagInFlag_ResetSwc = 0;
int diagInFlag_BootSwc = 0;
int diagInFlag_WdtLbSig = 0;
int diagInFlag_SelectBoard =0;

int diagInFlag_BobbinEncA = 0;
int diagInFlag_BobbinEncB = 0;
int diagInFlag_BobbinEncDir = 0;
int diagInFlag_AngleHomeSns = 0;
int diagInFlag_BbnBldcFG = 0;

int diagInFlag_PonSwc = 0;
int diagInFlag_PcUsb5V = 0;
int diagInFlag_Interlock = 0;

int sensorResetSwc = 0;
int oldsensorResetSwc = 0;
int sensorBootSwc = 1;
int oldsensorBootSwc = 1;
int sensorWdtFbSig = 0;
int oldsensorWdtFbSig = 0;
int sensorSelBoard = 0;
int oldsensorSelBoard = 0;

int sensorBbnEncA = 0;
int oldsensorBbnEncA = 0;
int sensorBbnEncB = 0;
int oldsensorBbnEncB = 0;
unsigned int sensorBbnEncCnt = 0;
unsigned int oldsensorBbnEncCnt = 0;
int sensorBbnEncDir = 0;
int oldsensorBbnEncDir = 0;

int sensorAngHomeSns = 0;
int oldsensorAngHomeSns = 0;
int sensorBbnFG = 0;
int oldsensorBbnFG = 0;

int sensorPonSwc = 0;
int oldsensorPonSwc = 0;
int sensorUsb5V = 0;
int oldsensorUsb5V = 0;
int sensorDoorIlk = 0;
int oldsensorDoorIlk = 0;

#define DIAG_SEN_XXX	0

#define DIAG_SEN_LS01	7
#define DIAG_SEN_LS02	8
#define DIAG_SEN_LS03	9
#define DIAG_SEN_LS04	10
#define DIAG_SEN_LMFG	11
#define DIAG_SEN_LMDIR	12
#define DIAG_SEN_LMPULSE	13
#define DIAG_RSV14		14

#define DIAG_SEN__MAX	15

#define SEN_MAX_SIZE	32
typedef struct _DIAGSEN
{
	short flag;
	short old;
	short curr;
}STDIAGSEN;
STDIAGSEN stDiagSen[SEN_MAX_SIZE];

unsigned int oldDiaglmEncPulse = 0;

int diagOutFlag_WDToff = 0;

int diagOutFlag_LedCent_R = 0;
int diagOutFlag_LedCent_G = 0;
int diagOutFlag_LedCent_B = 0;
int diagOutFlag_LedCent_RGB = 0;

int diagOutflag_bbnTimeOut = 0;		 
unsigned int bbnTimeout_step = 0x00;
unsigned long bbnTimeout_Val = 3000;	 

int diagBbnflag_TokTokControl = 0;
unsigned int bbnTokTok_step = 0x00;
int bbnTokTok_OnDuty = 15;			 
unsigned long bbnTokTok_OnTime = 200;		 
int bbnTokTok_OffDuty = 0;			 
unsigned long bbnTokTok_OffTime = 200;		 

int continueTorq = DEFAULT_TORQ;  
int currTorq = DEFAULT_TORQ;  
int oldTorq = DEFAULT_TORQ;  

int rdy_outTorq		= DEFAULT_TORQ;  
int rdy_eachTime	= 1000;			 
int rdy_cnt			= 0;
int rdy_incTorq		= 10;
int rdy_reqCnt		= 0;

int		gLevel	= 1;
int		gZone	= 1;
int		gStage	= 1;
String	rdyMsg;

int recvMainConn_Pwr1stCnt = 0;

typedef struct _paraCheck
{
	int idx;
	int min;
	int max;
}stParaCheck;

#define GAME_STS_LVL	11
#define GAME_STS_LVL	11
#define MAX_PARA_CNT	20

stParaCheck paraCheck[MAX_PARA_CNT] =
{

	{ 0, 			},	 
   	{ 1,			},	 

	{ GAME_STS_LVL,	1,	3	},	 

};

int mainMode = 0;		 
int oldmainMode = 0;

int ledControlMode = 1;	 

unsigned int bbnActKind = 0; 			 
unsigned int bbnOpKind = 1;				 
unsigned int bbnTokCnt = 0;

int exeFlag_bldcWave = 0;
int exeFlag_bldcBite = 0;
int exeFlag_bldcHit = 0;
int exeFlag_bldcFight = 0;
int exeFlag_bldcHoldOn = 0;
int exeFlag_bldcRanding = 0;
int exeFlag_bldcSuccess = 0;

int exeFlag_bbnMotorStop = 0;	 

int exeFlag_biteBldcTokTokEnd = 0;

int exeFlag_holonBldcTokTokEnd = 0;

int exeFlag_torqMotorStop = 0;	 

int restPowerDist = 0;

unsigned long bbnTokTO = 0;
unsigned int oldbbnTokStep = 0x00;

typedef struct _BBN_ACT
{
	int onDuty;
	unsigned long onTime;
	int offDuty;
	unsigned long offTime;	
}BBN_ACT;

enum
{
	BBNACT_SETVAL = 0,
	BBNACT_WAVE,			 
	BBNACT_BITE,			 
	BBNACT_HIT,				 
	BBNACT_FIGHT,			 
	BBNACT_HOLDON,			 
	BBNACT_RANDING,			 

	BBNACT_TBLMAX		 
}eBBNACTKIND;

#define MUL_BBN_PWR 2		 

BBN_ACT sBbnActTbl[BBNACT_TBLMAX] =	 
{

	{  ( DEFAULT_BBN_PWR+(13*MUL_BBN_PWR)),  1000,   0,   1000 },		 

	{  ( DEFAULT_BBN_PWR+(13*MUL_BBN_PWR)),  2000,   0,   2000 },		 

	{  ( DEFAULT_BBN_PWR+(15*MUL_BBN_PWR)),    70,   0,     70 },		 

	{  ( DEFAULT_BBN_PWR+(40-DEFAULT_BBN_PWR)),   300,   0,    100 },		 

	{  ( DEFAULT_BBN_PWR+(12*MUL_BBN_PWR)),   100,   0,     70 },		 

	{  ( DEFAULT_BBN_PWR+(46-DEFAULT_BBN_PWR)),   150,   0,     50 },		 

	{  ( DEFAULT_BBN_PWR+(14*MUL_BBN_PWR)),   100,   0,     70 },		 

};

int bbnReqCnt[BBNACT_TBLMAX] = {
		10,			 
		10,			 
		4,			 
		3,			 
		0,			 
		4,			 
		5			 
};

int bbnRptCnt = 0;

String strFishKind;
float 	fishKg_flt;
double	fishKg_dbl;

int fishSize = 0;
int fishRare = 0;

int holdDir = 0;
int startEnd = 0;

int randingPtn = 0;

#define LEVEL_MIN 1			 
#define LEVEL_MAX 3			 

BBN_ACT	sBbnBiteVal = 
{
	( DEFAULT_BBN_PWR+(15*MUL_BBN_PWR)),		 
	70,		 
	0,			 
	70		 
};

unsigned long sBbnHoldVal_onTime = 100;
unsigned long sBbnHoldVal_offTime = 50;

#define FISH_MAX (1+6+1)
int sBbnHoldVal[FISH_MAX] =
{
	0,			 
	34,			 
	40,			 
	45,			 
	50,			 
	56,			 
	61,			 
	67			 
};

typedef struct _RGB
{
	int r;
	int g;
	int b;
}sLEDRGB;
typedef sLEDRGB *ptrRGB;

sLEDRGB sRGB[LED_IDX_MAX] = 
{

	{0,},			 

	{255,255,255},	 
	{0,	0,	255},	 
	{0,	0,	255},	 
	{0,	0,	255},	 
};

sLEDRGB outRGB[LED_IDX_MAX] = 
{

	{0,},			 

	{255,255,255},	 
	{0,	0,	255},	 
	{0,	0,	255},	 
	{0,	0,	255},	 
};

#define COLOR_MAX_NO 16
extern const sLEDRGB sColorTBL[COLOR_MAX_NO]
{
	 		{255,	255,	255},	 
	 		{255,	0,	0},			 
	 		{0,	255,	0},			 
	 		{0,	0,	255},			 
	 		{255,	255,	0},		 
	 		{0,	255,	255},		 
	 		{255,	0,	255},		 

	 		{128,	0,	0},			 
	 		{128,	128,	0},		 
	 		{0,	128,	0},			 
	 		{128,	0,	128},		 
	 		{0,	128,	128},		 
	 		{0,	0,	128},			 

	 		{192,	192,	192},	 
	 		{128,	128,	128},	 
	 		{0,	0,	0},				 
};

unsigned long bbnDiagTO = 0;

#define BBN_PWM_DUTY_MIN 1
#define BBN_PWM_DUTY_MAX 255

#define BBN_WAIT_TIME_MIN 1
#define BBN_WAIT_TIME_MAX 30000

#define BBN_RPT_CNT_MIN	1
#define BBN_RPT_CNT_MAX	255

#define ROD_RESP_OVER_CNT 		(5)  
unsigned int rodAlive_SendCnt = 0;

int imu_conn_status = UNKNOWN;
int oldimu_conn_status = UNKNOWN;

int exeResistOnceComp = 0;

int dimming_time = SIPPAGOE;
int olddimming_time = -1;
unsigned int ledoffHoldTimeCnt = (100/10);		 

typedef struct _sensor
{
	unsigned int old2;
	unsigned int old;
	unsigned int curr;
	unsigned int lvl;
	unsigned int le;
	unsigned int te;
}sensor;
typedef sensor* ptrSensor;

#define SENSOR_1	0
#define SENSOR_2	1
#define SENSOR_3	2
#define SENSOR_4	3

#define SENSOR_MAX	4	 

sensor sensor1ms[SENSOR_MAX] = 
	{ 
		{0,0,0,0,0,0 },		 
		{0,0,0,0,0,0 },		 
		{0,0,0,0,0,0 },		 
		{0,0,0,0,0,0 }		 
	};

sensor sensor10ms[SENSOR_MAX] = 
	{ 
		{0,0,0,0,0,0 },		 
		{0,0,0,0,0,0 },		 
		{0,0,0,0,0,0 },		 
		{0,0,0,0,0,0 }		 
	};

#define BIT_0	0
#define BIT_1	1
#define BIT_2	2
#define BIT_3	3
#define BIT_4	4
#define BIT_5	5
#define BIT_6	6
#define BIT_7	7
#define BIT_8	8
#define BIT_9	9
#define BIT_10	10
#define BIT_11	11
#define BIT_12	12
#define BIT_13	13
#define BIT_14	14
#define BIT_15	15

#define SNS_boot		BIT_0  
#define SNS_usb5V		BIT_1  
#define SNS_PwrOffSwc	BIT_2  
#define SNS_AcOff		BIT_3  

#define SNS_RSV04		BIT_4	 

#define SNS_bbnMotFg	BIT_5  
#define SNS_bbnEncA		BIT_6  
#define SNS_bbnEncB		BIT_7  

#define SNS_RSV11		BIT_11	 
#define SNS_RSV12		BIT_12	 
#define SNS_RSV14		BIT_14	 

#define BIT_SNS_BOOT	0x0001	 
#define BIT_SNS_USB5V	0x0002	 
#define BIT_SNS_PWRSWC	0x0004	 
#define BIT_SNS_ACOFF	0x0008	 
#define	BIT_SNS_PWRALL	(BIT_SNS_USB5V|BIT_SNS_PWRSWC|BIT_SNS_ACOFF)		 

#define BIT_SNS_RSV4	0x0010	 

#define BIT_SNS_BBNFG	0x0020	 
#define BIT_SNS_BBNENCA	0x0040	 
#define BIT_SNS_BBNENCB	0x0080	 
#define BIT_SNS_BBNALL	(BIT_SNS_BBNFG|BIT_SNS_BBNENCA|BIT_SNS_BBNENCB)		 
#define BIT_SNS_ENCAB	(BIT_SNS_BBNENCA|BIT_SNS_BBNENCB)					 

#define SNS_ON		1
#define SNS_OFF		0

#define	LVL01_BBNALL	(sensor1ms[SENSOR_1].lvl & (BIT_SNS_BBNALL))		 
#define LVL01_BBNFG		(sensor1ms[SENSOR_1].lvl & (BIT_SNS_BBNFG))
#define LVL01_BBNENCA	(sensor1ms[SENSOR_1].lvl & (BIT_SNS_BBNENCA))
#define LVL01_BBNENCB	(sensor1ms[SENSOR_1].lvl & (BIT_SNS_BBNENCB))

#define LE01_BBNFG		(sensor1ms[SENSOR_1].le & (BIT_SNS_BBNFG))
#define LE01_BBNENCA	(sensor1ms[SENSOR_1].le & (BIT_SNS_BBNENCA))
#define LE01_BBNENCB	(sensor1ms[SENSOR_1].le & (BIT_SNS_BBNENCB))

#define TE01_BBNFG		(sensor1ms[SENSOR_1].te & (BIT_SNS_BBNFG))
#define TE01_BBNENCA	(sensor1ms[SENSOR_1].te & (BIT_SNS_BBNENCA))
#define TE01_BBNENCB	(sensor1ms[SENSOR_1].te & (BIT_SNS_BBNENCB))

#define LVL10_BOOT		(sensor10ms[SENSOR_1].lvl & (BIT_SNS_BOOT))
#define LVL10_USB5V		(sensor10ms[SENSOR_1].lvl & (BIT_SNS_USB5V))
#define LVL10_PWROFF_SWC (sensor10ms[SENSOR_1].lvl & (BIT_SNS_PWRSWC))		 
#define LVL10_ACOFF		(sensor10ms[SENSOR_1].lvl & (BIT_SNS_ACOFF))		 
#define	LVL10_PWRALL	(sensor10ms[SENSOR_1].lvl & (BIT_SNS_PWRALL))		 

#define LVL10_BBNFG		(sensor10ms[SENSOR_1].lvl & (BIT_SNS_BBNFG))
#define LVL10_BBNENCA	(sensor10ms[SENSOR_1].lvl & (BIT_SNS_BBNENCA))
#define LVL10_BBNENCB	(sensor10ms[SENSOR_1].lvl & (BIT_SNS_BBNENCB))
#define	LVL10_ENCAB		(sensor10ms[SENSOR_1].lvl & (BIT_SNS_ENCAB))		 

#define LVL10_LMMOTFG	(sensor10ms[SENSOR_1].lvl & (BIT_SNS_LMMOTFG))
#define LVL10_LMENC		(sensor10ms[SENSOR_1].lvl & (BIT_SNS_LMENC))

#define LE10_BBNFG		(sensor10ms[SENSOR_1].le & (BIT_SNS_BBNFG))

#define LE10_USB5V		(sensor10ms[SENSOR_1].le & (BIT_SNS_USB5V))
#define LE10_PWROFF_SWC	(sensor10ms[SENSOR_1].le & (BIT_SNS_PWRSWC))

#define TE10_BBNFG		(sensor10ms[SENSOR_1].te & (BIT_SNS_BBNFG))

#define TE10_USB5V		(sensor10ms[SENSOR_1].te & (BIT_SNS_USB5V))
#define TE10_PWROFF_SWC	(sensor10ms[SENSOR_1].te & (BIT_SNS_PWRSWC))

int recvPwrOffExec_Resp = 0;

unsigned int	motTestAct = 0;		 
unsigned int	motTestMainIdx = 20;	 
unsigned int	motTestBbnIdx = 20;		 

uint8_t rcv_src_addr_back[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };		 
uint8_t rcv_dest_addr[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };	 

uint8_t broad_cast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };	 

volatile int rodRegistMode = 0;		 

short endTmReqDuty = DEFAULT_TORQ;	 
short leftEndTmReqDuty = DEFAULT_TORQ;	 
short rightEndTmReqDuty = DEFAULT_TORQ;	 

typedef struct _motdutyTbl
{
	short lm;
	short lmTime;
	short bm;
	short bmTime;
	short tm;
	short tmTime;
}STLMOTDUTYTBL;
typedef STLMOTDUTYTBL *ptrSTMOTDUTYTBL;

#define FISH_LVL_AUTO	0	 
#define FISH_LVL_1	1
#define FISH_LVL_2	2
#define FISH_LVL_3	3
#define FISH_LVL_MAX	(1 + FISH_LVL_3)

#define MOT_PWR_LVL_a	1
#define MOT_PWR_LVL_b	2
#define MOT_PWR_LVL_c	3
#define MOT_PWR_LVL_MAX	(1 + MOT_PWR_LVL_c)

STLMOTDUTYTBL stLmWireDutyTbl[2][MOT_PWR_LVL_MAX] =
{

	{
		{0, },
	},

	{
		{0,},	 

		{ 10, 10, 10, 40,  60, 40},	 
		{ 80,100,  5,100,  70,100},	 
		{ 80,200, 20, 50,  80, 50},	 
	},
};

STLMOTDUTYTBL stLmDutyTbl[FISH_LVL_MAX][MOT_PWR_LVL_MAX] =
{

	{
		{0, },
	},

	{
		{0,},	 

		{ 10, 10, 13,100,  67,100},  
		{ 80,100,  5,100,  67,100},  
		{ 80,200, 13,200,  67,200},  
	},

	{
		{0,},	 

		{ 10, 10, 19,100,  67,100},
		{150,100,  5,100,  67,100},
		{150,200, 13,200,  80,200},
	},

	{
		{0,},	 

		{ 10, 10, 25,100,  67,100},
		{200,100,  5,100,  67,100},
		{200,200, 13,200,  90,200},
	},

};

STLMOTDUTYTBL stLmHomeTbl =
{

	40,100, 15,100, 60,100		 
};

STLMOTDUTYTBL stLmReturnTbl =
{

	60,100, 20,100, 60,100		 
};

volatile int nvm01_home_stopWait_time = 1;

#define LM_START_WAIT_TIME 10  

#define LM_MOVE_ERR_TO_TIME		3000	 
#define LM_MOVE_TO_TIME	3000

short fishLevel = FISH_LVL_1;
short rcvlmPwr = MOT_PWR_LVL_b;

short fishPwr = 1;			 
short gmWatStatus = 0;		 
short oldGmWatStatus = 0;		 

#define	TIME_SLEEP_LIGHT		60
#define	TIME_SLEEP_DEEP			120

#define	SS_NONE			0		 
#define	SS_WAIT			1		 
#define	SS_EXIT			2		 
#define	SS_DOWN			3		 

int   gRodSleepStat = 0;		 
int   gRodSleepMode = 0;		 
int   gRodSleepTime = 0;		 

volatile unsigned short wireLongCheck_Flag = 0;		 
volatile unsigned short lmWireControl_Flag = 0;		 
volatile unsigned int targetWirePulse = 0;
volatile unsigned int currWirePulse = 0;
volatile unsigned int oldcurrWirePulse = 0;

#define DF_CONFIG_UNKNOWN		0
#define DF_CONFIG_STAND_ALONE	1

short dfConfig = DF_CONFIG_UNKNOWN;

unsigned short apType = AP_IS_DF;
unsigned short dfMainAdcDiagnosticEnabled = 0;

String strImuInterval = "0000";	 

unsigned short devReadychkFlag = 0;
unsigned short devCheckAllFlag = 0;					 

unsigned short devCheckBbnMotFlag = 0;
unsigned short devCheckBbnEncFlag = 0;

unsigned short devCheckTorqFlag = 0;

unsigned short devCheckBldc24VFlag = 0;

unsigned short devBbnAllChkStep = 0;					 
unsigned short devBbnMotChkStep = 0;
unsigned short devBbnEncChkStep = 0;

unsigned short devTorqChkStep = 0;

unsigned short devBldc24VChkStep = 0;	 

unsigned short mainMotAutoSet_Step = 0;	 

unsigned short devChkErrOccure = 0;

unsigned short stbyTorqOffControl_Req_Flag = 0;

unsigned short stbyTorqOffControlStep = 0;			 

volatile unsigned short sSys100UsCnt = 0;		 
volatile unsigned long lSys100UsTime = 0;		 
volatile unsigned long lSys1MsTime = 0;	 

hw_timer_t *timer = NULL;   
volatile unsigned long lLastTime = 0;   
volatile unsigned long lCurrentTime = 0;   
volatile unsigned long iEncIntrIntervalTime = 0;   

#define ENC_INTV_BUFF_SIZE		256
typedef struct _encIsrDataBuff
{
	unsigned long tick;
	unsigned long width;
	unsigned short dir;	
	unsigned short enc;	
	unsigned short dist;
}encIsrDataBuff;
encIsrDataBuff stEncIntvBuff[ENC_INTV_BUFF_SIZE] =
{
	{0,0,0,0,0},
};

volatile unsigned short sEncBuffIdx=0;

volatile bool bIsrEncInterruptFlag = false;   
volatile unsigned int	iIsrEncInterruptCnt = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;   

double dLastDiff = 0;
double dIntegral = 0;
double dIsrOutDuty = 0;

#define PID_EXEC_ENC_MIN_TIME_1MS	15		 
#define PID_EXEC_ENC_MIN_TIME_100US	(15*10)		 

unsigned int iWireDistance = 0;	 
unsigned int iTargetDist = 0;	 

bool bHookRandingCheckOnce = 0;

unsigned short pidExecFlag = 0;	 
unsigned short pidExecOutLog = 0;	 

double Kp = 0.5;   
double Ki = 0.2;   
double Kd = 0.1;   

short sMotorSpeed = 0;

int iIsrbbnEncCnt = (WIRE_HOME_BBN_CNT);			 
unsigned short sIsrEncBdata;
unsigned short sIsrBbnEncDir;

#define	MAX_SAVE_LEN	4096
int	g_SavePtr = 0;
int	g_ProcPtr = 0;
unsigned char g_readBuf[MAX_SAVE_LEN];

volatile unsigned int g_idleCountCore0 = 0;
#define	MAX_IDLE_CORE	11893125

void esp_soft_reset();
void esp_hard_reset();
void SerialPortEnable();
void SerialPortDisable();
void SerialPortPinInput();
void hex2asc(unsigned long hx, char* buf, int len);
void HexDump(unsigned long toff, unsigned char* buff, int len);
void debugPort_Off();
void debugPort_On();
void subAC_Off();
void subAC_On();
void DC24V_B_off();
void ledoff_BoardLedAllOff();
void ledOff_MbCent();
void ledOff_MbBtm();
void ledOff_OptLeft();
void ledOff_OptRight();
void extLed_AllOff();
void extLed_controlOff();
void ledOn_MbBtm();
void torqOff();
void torqDefault();
int GetDefValue4torqMotor();
void servoDefault();
void bbnBldcOff();
void breakMotorOff();
void motor_AllOff();
void motor_AllDefault();
void led_AllOff();
void output_AllOff();
void reelOut_AllOff();
void reelOut_AllOff_ForceStep();
String fillZero2String5Char(int _int);
String stringSign6Char(long _int);
String fillZero2String3Char(unsigned int uint);
String fillZero2StringSign4Char(int _int);
String fillZero4Char(unsigned int _val);
String fillZero2StringSign6Char(int _int);
String StringFormat(char *fmt, int val);
void rotateChangeCallback(int _dir, int _cnt);
void battLevelSet( String msg);
void setRod_Connected();
void uartRecvHandlerImp();
bool isParaCheckOk(int idx, int val);

void torqMot_OffControlStart(unsigned int onTime);
void torqMot_OffControlStop();
void torqMotor_OffTime_Control();
void bbnMotor_OffTime_Start(unsigned int time);
void bbnMotor_OffTime_Control();

void SetIMU_Measure_Out_OnOff(unsigned int _setClr);
void sendSleepEnable(int Mode, int Time, int Id);
void sendCurrentStatus();
void initSet_LedContVal(int Act);
void ledCont_OffStart(unsigned int pos);
void ledCont_OnStart(unsigned int pos);
void ledCont_BlinkStart(unsigned int pos);
void execledCont_Blink(int pos);
void ledCont_DimmStart(unsigned int pos);
void execledCont_Dimm(int pos);
void old_execCentLed_Dimming_Control(int color, int sec);
void extLed_Off(unsigned int idx);
void extLed_On(unsigned int idx);
void extLed_Control();

void analedContCmd(String msg);

void sendRodVrtCmd(int act, unsigned int cnt, unsigned int ontime, unsigned int offtime);
void sendRodBtnLedCmd(int posi, int act, unsigned int cnt, unsigned int ontime, unsigned int offtime);

void anaDeviceInfoReq(String msg);

void apAlive_Check();
void send2RodAlive_CntCheck();
void rodRegistToStop();
void rodRegistToStart();
void rodRegi3secToStart();
void rodRegistToControl();
void rodRegistCancel();
void anaRodRegist(String msg);
void rodRegistExec(String msg);
void tmInput_BoardElecMonitorExec(short _exec);
void tmInput_BbnMotorMonitorExec(short _exec);
void anaTmInputTestReq(String msg);
void anaTmOutputTestReq(String msg);
void anaApInfo(String msg);
void anaMainAlive(String msg);
void devChkReady_Control();
void resp_MainBoard_Ver();
void devchk_OkEnd();
void send2resp_FwSelfControlCmd(String Cmd, int torq);
String stringTo3char(int val);
void devChkMsgSend(String msg);
void devchk_bbnAllStart();
void devCheckFlagStep_AllClear();
void devchk_bbnAll_Control();
void devReadyCheck();
void send_Error_Clr();
void anaDeviceContReq(String msg);
void anaRestDistance(String msg);
void ana_AngleControl( String msg );
void ana_MainMotControl( String msg );
void ana_BobbinControl_Check(String msg);
void ana_BobbinControl(String msg);
void checkRodTimer();
void set_rod_conn_status(unsigned int sts);

void diagInputMonitor();
void diagInputMotorSensorAllOff();

void diagInput_AllOff();
void diagInput_BoardInAllOff();
void diagInput_BoardInAllOn();
void DiagInput_MotInAllOff();
void DiagInput_MotInAllOn();
void DiagInput_MainEcoderInAllOn();
void DiagInput_MechaInAllOff();
void DiagInput_ElecInAllOff();
void DiagInput_ElecInAllOn();
void diagInput_Cat0(String _msg);
void diagIput_Cat1(String _msg);
void diagInput_Cat3(String _msg);
void diagInput_Cat5(String _msg);
void diagInputTest(String _msg);
void diagOutput_AllOff();
void diagOutput_BoardOutAllOff();
void setRGBcolor(int pos, int Color);
void setOutRGBcolor(int pos, int Color);
void diagBbnMotor_TimeoutStart(unsigned int _time);
void diagBbnMotor_TimeoutControl();
void daigBbnMotor_TokTokEnd();
void diagBbnMotor_TokTokStart(unsigned int _ackKind, unsigned int _cnt);
void diagBbnMotor_TokTokControl(unsigned _opKind);
void diagOut_Cat1(String _msg);
void diagOut_Cat3(String _msg);
void diagOutputTest(String _msg);
void findDiagCode(String msg, stDiagCode code);
int controlDiagInput(String msg);
int controlDiagOutput(String msg);
void diagOutAllOff();

void subAcOff_WithOtherOff();
int CheckAppCmd();
int CheckPowerSwitch();
void check_PowerSwitch_Change_10ms();
void imuDataOutCmdSend(int onOff);
void usb5vChange_Check();
int DecodeLZ(unsigned char* src, unsigned char* dst, int opt);
void Exec_Download(String msg);

void reelAllOff_SendControl();
void t1ms_Process_Exec();
void t10ms_Process_Exec();
void t100ms_Process_Exec();
void t500ms_Process_Exec();
void t1sec_Process_Exec();
void sendCmdSts_MainPwrOn();
void logUartRecvParsing();
void logRecvHandler();
void extIOuartRecvHandler();
void imuInterval_Measure();
void nowRecvHandler();
void uartRecvParsing();
void uartRecvHandler();
String anaVrtMotCmd(String msg);

String anaBtnLedCmd(String msg);

void torqMotor_DefaultOut();

void boardLed_Control();
void read_input_1ms();
void read_input_10ms();
void read_input_100ms();

void subAc_Control_atReset();
short setConfig();
void ioLedOffSetting();
void ioPinSetting();

void spi_out(int _pin, int _val);
void system_counter();
int getBoardType();
void DF_Main_Application_Setup();
void DF_Main_Application_Loop();
void esp_soft_reset()
{
	ESP.restart();
}

void esp_hard_reset()
{
		esp_soft_reset();
}

void SerialPortEnable()
{
	Serial.begin(115200);			 
	Serial.setTimeout(10);			 
	int	nBeforeHeap = ESP.getFreeHeap();
	Serial.setRxBufferSize(4096);		 
	int	nAfterHeap = ESP.getFreeHeap();

	g_SerialEnable = 1;
	LogPrintln(" LG] USBCDC SerialPortEnable(): " + String(nBeforeHeap) + " -> " + String(nAfterHeap));
}

void SerialPortDisable()
{
	int	nBeforeHeap = ESP.getFreeHeap();
	Serial.end();					 
	int	nAfterHeap = ESP.getFreeHeap();
	pinMode(USB_DM, INPUT_PULLDOWN);	 
	pinMode(USB_DP, INPUT_PULLDOWN);	 

	g_SerialEnable = 0;
	LogPrintln(" LG] USBCDC SerialPortDisable(): " + String(nBeforeHeap) + " -> " + String(nAfterHeap));
}

void SerialPortPinInput()
{
	pinMode(USB_DM, INPUT_PULLDOWN);	 
	pinMode(USB_DP, INPUT_PULLDOWN);	 

	LogPrintln(" LG] USBCDC SerialPortPinInput()");
}

char	hextbl[] = { "0123456789ABCDEF" };
struct	Lins {
	char	toff[8];
	char	spb1[2];
	char	hxdb[16][3];
	char	spb2[3];
	char	chdb[16];
	char	crlf;
	char	delm;
}	Lins;

void hex2asc(unsigned long hx, char* buf, int len)
{
	int	i;
	for (i = 0; i < len; i++, hx >>= 4)
		buf[len - i - 1] = hextbl[hx & 0x000f];
}

void HexDump(unsigned long toff, unsigned char* buff, int len)
{
	int	i, j, k;

	for (k = 0; k < len; k += 16, buff += 16)
	{
		memset((char*)&Lins, ' ', sizeof(Lins));
		hex2asc(toff + k, &Lins.toff[4], 4);
		j = ((len - k) < 16) ? (len - k) : 16;

		for (i = 0; i < j; i++)
		{
			if ((i & 0xf) == 8)
				Lins.hxdb[i][0] = '-';
			hex2asc(buff[i], &Lins.hxdb[i][1], 2);
			Lins.chdb[i] = ((buff[i] >= ' ') && (buff[i] <= 0x7e)) ? buff[i] : '.';
		}
		Lins.crlf = '\0';
		Lins.delm = '\0';
		LogPrintln(&Lins.toff[0]);
	}
}

void debugPort_Off()
{
	digitalWrite(FW_DEBUG_PIN, LOW);

}

void debugPort_On()
{
	digitalWrite(FW_DEBUG_PIN, HIGH);

}

#define PWR_ON	1
#define PWR_OFF	0
int pwrMode = PWR_ON;	 

void subAC_Off()
{
	digitalWrite(SUB_ACOFF_PIN, SUBAC_OFF_LVL);
	pwrMode = PWR_OFF;

}

void subAC_On()
{

	digitalWrite(SUB_ACOFF_PIN, SUBAC_ON_LVL);
	pwrMode = PWR_ON;

}

void DC24V_B_off()
{

}

void ledoff_BoardLedAllOff()
{
	digitalWrite(BD_LED2_PIN, LOW);

		digitalWrite(BD_LED3_PIN, LOW);
		digitalWrite(BD_LED4_PIN, LOW);

}

void ledOff_MbCent()
{
	extLed.off(LEDPOS_CENT_RGB);
}

void ledOff_MbBtm()
{
	extLed.off(LEDPOS_BTM);
}

void ledOff_OptLeft()
{
	extLed.off(LEDPOS_LEFT);
}

void ledOff_OptRight()
{
	extLed.off(LEDPOS_RIGHT);
}

void extLed_AllOff()
{
	ledOff_MbCent();
	ledOff_MbBtm();
	ledOff_OptLeft();
	ledOff_OptRight();
}

void extLed_controlOff()
{
	unsigned int i;
	for(i=LED_IDX_CENT; i<LED_IDX_MAX; i++)
	{
		ledCont[i].cont = 0;	 
	}
}

void ledOn_MbBtm()
{
	extLed.on(LEDPOS_BTM, (int)LED_MAX_DUTY);
}

void torqOff()
{
	torqMotor.setValue(0);
}

void torqDefault()
{

	torqMotor.setValue(DEFAULT_TORQ);
}

int GetDefValue4torqMotor()
{
	return (recv_TorqMotor_Flag ? reqTorqueMotor : defaultTorqueMotor);
}

void servoDefault()
{
}

void bbnBldcOff()
{
	bbnMotor.offBldc();
}

#define ROD_BREAK_OFF_STR_DATA	"000"
void breakMotorOff()
{
	if(ROD_BOARD_V1 == rodBoardType)		 
	{

		eNow.write(DF_Protocol_MainToRod_Break, ROD_BREAK_OFF_STR_DATA);	   
	}
}

void motor_AllOff()
{

  	torqOff();
  	servoDefault();
  	bbnBldcOff();

}

void motor_AllDefault()
{
	torqDefault();
	servoDefault();
	bbnBldcOff();

}

void led_AllOff()
{

	extLed_AllOff();

	ledoff_BoardLedAllOff();
}

void output_AllOff()
{

  torqOff();
  servoDefault();
  bbnBldcOff();

  extLed_AllOff();

  ledoff_BoardLedAllOff();
}

volatile int reelOut_AllOff_Flag = 0;		 
volatile int reelAlloffStep = 0;			 
void reelOut_AllOff()
{

	eNow.write(DF_Protocol_MainToRod_AllOutputsOff, "00");		 
}

#define SEND_NOW_STEP_TIME	10		 

void reelOut_AllOff_ForceStep()
{
}

String fillZero2String5Char(int _int)
{
  String str;
  if(99999 < _int) {_int = 99999; }
  if (_int < 10)
  {
    str = "0000" + String(_int);
  }
  else if (_int < 100)
  {
    str = "000" + String(_int);
  }
  else if (_int < 1000)
  {
    str = "00" + String(_int);
  }
  else if (_int < 10000)
  {
    str = "0" + String(_int);
  }
  else
  {
    str = String(_int);
  }
  return str;
}

String stringSign6Char(long _int)
{
	String str;
  if(99999 < _int) {_int = 99999; }
  else if(-99999 > _int) {_int = -99999; }

  if( 0 < _int)  
  {
  	str = "+" + String(_int);
  }
  else if( 0 == _int)	 
  {
  	str = "000000";
  }
  else
  {
  	str = String(_int);
  }

  return str;
}

String fillZero2String3Char(unsigned int uint)
{
  String str;
  if(0)	{ }

  else if (10 > uint)	 
  {
    str = "00" + String(uint);
  }
  else if (100 > uint)	 
  {
    str = "0" + String(uint);
  }
  else					 
  {
    str = String(uint);
  }
  return str;
}

String fillZero2StringSign4Char(int _int)
{
  String str;
  if(-999 > _int) { _int = -999; }
  else if( 999 < _int) { _int =  999; }

  if(-99 >  _int )	 
  {
  	str = "-" + String(_int*-1);
  }
  else if(-9 >  _int )	 
  {
  	str = "-0" + String(_int*-1);
  }
  else if(0 >  _int )	 
  {
  	str = "-00" + String(_int*-1);
  }
  else if(0 ==  _int )	 
  {
  	str = "0000";
  }
  else if (10 > _int)
  {
    str = "+00" + String(_int);
  }
  else if (100 > _int)
  {
    str = "+0" + String(_int);
  }
  else
  {
    str = "+" + String(_int);
  }
  return str;
}

String fillZero4Char(unsigned int _val)
{

 	if(9999 < _val) { _val = 9999; }

  if(0) {}	 
  else if (  10 > _val) { return "000" + String(_val); }
  else if ( 100 > _val) { return "00" + String(_val); }
  else if (1000 > _val) { return "0" + String(_val); }
  else 					{ return String(_val); }

}

String fillZero2StringSign6Char(int _int)
{
  String str;
  if(-99999 > _int) { _int = -99999; }
  else if( 99999 < _int) { _int =  99999; }

  if(-9999 >  _int )	 
  {
  	str = "-" + String(_int*-1);
  }
  else if(-999 >  _int )	 
  {
  	str = "-0" + String(_int*-1);
  }
  else if(-99 >  _int )	 
  {
  	str = "-00" + String(_int*-1);
  }
  else if(-9 >  _int )	 
  {
  	str = "-000" + String(_int*-1);
  }
  else if(0 >  _int )	 
  {
  	str = "-0000" + String(_int*-1);
  }
  else if(0 ==  _int )	 
  {
  	str = "000000";
  }
  else if (10 > _int)	 
  {
    str = "+0000" + String(_int);
  }
  else if (100 > _int)	 
  {
    str = "+000" + String(_int);
  }
  else if (1000 > _int)
  {
    str = "+00" + String(_int);
  }
  else if (10000 > _int)	 
  {
    str = "+0" + String(_int);
  }
  else
  {
    str = "+" + String(_int);
  }
  return str;
}

String StringFormat(char *fmt, int val)
{
	char buf[32];
	snprintf(buf, sizeof(buf), fmt, val);
	return buf;
}
