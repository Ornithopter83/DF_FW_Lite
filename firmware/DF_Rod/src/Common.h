

#pragma once

#include <DFProtocol.h>

#define IO_IMU	(1)

#define BAT_KAL_FILTER		(0)			 
	#define BAT_AD_AVG_INTEG	(0)			 

#define NEW_IF		(1)			 

#define SLEEP_SET	(0)

#define ROD_BDTYPE_DETECT	(0)		 

#define IMU_RX  			1
#define IMU_TX  			2

#define BTN_RED_PIN			5			 

#define BTN_BLUE_PIN_NEW	4		 

#define BTN_LED_LF_RED_PIN		8		 
#define BTN_LED_RT_BLUE_PIN		9		 

#define LED1_GRN_PIN  		6 		 
#define LED2_RED_PIN  		7 		 

#define SPI_SS    			10
#define SPI_SCK   			11
#define SPI_MOSI  			12

#define VRT_MOT_ON_PIN		12		 

#define BATTERY_LVL_PIN		14

#define DC24VON_PIN  		15			 

#define ENC_HALL_SEN_PIN	18	 

#define PWM_PIN   			38		 

#define BOARD_TYPE_PIN		39		 

#define DEBUG_TX_PIN		43		 
#define DEBUG_RX_PIN		44		 
#define SETUP_MODE_PIN  	48	 
#define BOOT_MODE_PIN		0	 

#define BTN_LED_ON		LOW
#define BTN_LED_OFF		HIGH

#define VRT_MOT_ON		HIGH
#define VRT_MOT_OFF		LOW

#define NEW_BOARD_2		1
#define BREAK_NO 	1	 
#define BUTTON_V2	1	 
#define REEL_ENC_V3	2	 
#define BATT_800_V2		1	 

#define PID_NOT_RECVED		0		 

#define IMU_SETUP     11

#define PID_IMU_VERSION   20

#define IMU_DATA_OFF 		0
#define IMU_DATA_ON			1

#define IMU_SET_INTEVAL		2
#define IMU_SET_OUT			3
#define IMU_SET_CALBRATION	4

#define AP_IS_DF	0	 
#define AP_IS_TM	1	 

#define RespPrintln Serial.println
#define StsPrintln  Serial.println

#define LogPrint Serial.print
#define LogPrintln Serial.printf("%10d",curr_ms_tick);Serial.println

#define imuSerial Serial1
#define send2Imuln Serial1.println

#define DbgSerial Serial2
#define DbgPrintf Serial2.printf("%10d",curr_ms_tick);Serial2.printf
#define DbgPrintln Serial2.printf("%10d",curr_ms_tick);Serial2.println
#define send2DBGln Serial2.println

#define setTO(var) (var = curr_ms_tick)				 
#define checkTO(var,TO) ((curr_ms_tick - var) > TO)	 

#define IMU_INTV_MIN_TIME	20	 
#define IMU_INTV_MAX_TIME	1000	 
#define IMU_INTV_DEFAULT_TIME	100	 

#define UNKNOWN		(-1)
#define CONNECTED	(0)
#define DISCONN		(1)
#define CONNELSE	(2)

#define STS_OK		(0)
#define STS_NG		(1)

#define FUNC_EQ_TARGET_ADDR		(1)		 
#define FUNC_NOW_DIS_AT_SETUP	(1)		 
#define FUNC_SLEEP_ENTRY		(0)		 

#define LOG_PWRON_TIME	(0)

#define LOG_BTN			(0)
#define LOG_BAT			(0)

#define LOG_FUNC_STEP	(0)

#define LOG_CONN		(1)  

#define LOG_MAIN_RECV	(0)  

#define LOG_ALIVE_CHK_CNT	(0)  

#define LOG_POWER_ON_VER	(1)
#define LOG_POWER_ON_MODE	(0)

#define LOG_IMU_CONFIG	(0)

#define LOG_TARGET_LOAD		(0)		 
#define LOG_BRK_MOT			(0)		 

#define LOG_BATT_INIT_STR	(0)  

#define LOG_MAIN_PWRON		(1)		 
#define LOG_MAIN_IMU_ONOFF	(0)		 

#define LOG_BOARD_TYPE		(0)		 
#define LOG_IO_TYPE			(0)		 

#define LOG_IMU_RECV_DATA_OUT	(0)	 

#define LOG_ADDR_NG		(1)			 

#define LOG_FS_FORMAT_START	(1)			 
#define LOG_FS_FORMAT	(0)			 
#define LOG_SPIFFS_OK	(0)			 

#define LOG_SYSTICK_DELAY	(1)			 
#define LOG_1MS_EXE_DELAY	(1)			 

#define LOG_NOW_SEND_ERR		(1)	 
#define LOG_NOW_SEND_SEQNO		(0)			 
#define LOG_NOW_CB_SENT_FAIL	(1)
#define LOG_NOW_CB_SENT_REGI	(1)	 

#define LOG_NOW_RCV_OVER_WRITE	(1)		 
#define LOG_NOW_RCV_CB_US_CHK	(0)	 
#define LOG_NOW_CB_RCV_MSG		(0)
#define LOG_NOW_NOT_DEF_CMD		(1)		 

#define LOG_SYS_US_CHK	(0)

#define LOG_NOW_GET_INFO	(0)  

#define LOG_BTN_LONG			(0)			 
#define LOG_MAIN_ADDR_WRITE		(0)		 
#define LOG_ROD_REGI_TO_STEP	(0)	 
#define LOG_OTHER_ROD_NOT_EXEC	(1)		 

#define LOG_RCV_BREAK_EXE		(0)	 

#define LOG_VRT_MOT_MAIN_CMD_ERR	(1)
#define LOG_VRT_MOT_MAIN_CMD		(0)
#define LOG_BTN_LED_MAIN_CMD		(0)
#define LOG_BTN_LED_MAIN_CMD_ERR	(1)

#define LOG_IMU_OUT_MAIN_CMD_ERR	(1)		 

#define LOG_OUT_OFF	(0)  

#define LOG_IMU_AUTOSET_START	(1)		 
#define LOG_IMU_AUTOSET			(0)		 

extern unsigned long curr_ms_tick;
extern unsigned long curr_us_tick;
