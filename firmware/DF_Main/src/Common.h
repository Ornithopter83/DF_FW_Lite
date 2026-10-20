

#pragma once

#include <DFProtocol.h>
#include "Config.h"

#define BOOT_MODE_PIN		0		 

#define PC_USB5V_PIN	3

#define BLDC_FG_PIN 	4	 
#define BBN_MOT_ON_PIN	5		 
#define BBN_MOT_DIR_PIN	6		 
#define BBN_MOT_PWM_PIN	7		 

#define I2C_SDA_PIN		8
#define I2C_SCL_PIN		9

#define TORQ_MOSI  		10		 
#define TORQ_SCK   		11  		 
#define TORQ_SS    		12		 

#define LED_CENT_R_PIN	13
#define LED_CENT_G_PIN	14
#define LED_CENT_B_PIN	21

#define SUB_ACOFF_PIN	15		 
#define PWROFF_SWC_PIN	16		 

#define BBN_ENC_A_CK_PIN	17
#define BBN_ENC_B_DT_PIN 	18

#define ENC_INTR_PIN	BBN_ENC_A_CK_PIN   

#define USB_DM			19		 
#define USB_DP			20		 

#define BBN_BLDC_24VON_PIN	38		 

#define BD_LED2_PIN		39		 

#define LED_BTM_PIN		42

#define UART1_TX_PIN  	43		 
#define UART1_RX_PIN  	44		 

#define STRAPP_PSRAM_45	45
#define STRAPP_PSRAM_46	46

#define STRAPP_PSRAM_35	35
#define STRAPP_PSRAM_36	36
#define STRAPP_PSRAM_37	37
#define BD_LED3_PIN		40		 
#define BD_LED4_PIN		41		 
#define FW_DEBUG_PIN	BD_LED4_PIN		 

#define WDT_OUT_PIN		47  	 

#define STRAPP_PSRAM_48	48
#define BD_TYPE_PIN		STRAPP_PSRAM_48		 

#define BLDC_HZ 		22000		 
#define BLDC_RESOL10	10			 
#define BLDC_RESOL8		8			 
#define BLDC_RESOL		BLDC_RESOL10

#define PWM_CH0	0
#define PWM_CH1	1
#define PWM_CH2	2
#define PWM_CH3	3
#define PWM_CH4	4
#define PWM_CH5	5
#define PWM_CH6	6
#define PWM_CH7	7

#define BOBBIN_CH	PWM_CH0		 

#define BLDC_DUTY_FULL		(1)
#define BLDC_DUTY_LIMIT192	(0)
#define BLDC_DUTY_LIMIT128	(0)

#define TORQ_MOT_CW		0	 

#define BBN_BLDC_24VON	LOW 	 
#define BBN_BLDC_24VOFF HIGH	 
#define BBN_MOT_ON		HIGH  
#define BBN_MOT_OFF 	LOW  
#define BBN_MOT_CW		HIGH
#define BBN_MOT_CCW		LOW
#define BBN_PWM_INVERT		(1)  

#define BBN_MOT_PULL	BBN_MOT_CW
#define BBN_MOT_REL		BBN_MOT_CCW		LOW

#define SUBAC_OFF_LVL 	HIGH
#define SUBAC_ON_LVL 	LOW

#define LED_PWM_INVERT			(0)		 

#define BBN_ENC_CW 	1
#define BBN_ENC_CCW 2

#define BBN_ENC_DIR_INVERT	(0)

#define ROD_UNKNOWN_2		(-2)   	
#define ROD_UNKNOWN			(-1)   	
#define ROD_CONN			0

#define ROD_DISCONN			10
#define ROD_R_DIS_NOTINIT 	11
#define ROD_R_DIS_NOTPEER	12
#define ROD_R_DISCONN_OTH	13
#define ROD_R_DIS_MAC		19			

#define ROD_R_NOT_RESP		20

#define ROD_R_DIS_SLEEP 	88

#define UNKNOWN_2	-2
#define UNKNOWN		-1
#define CONNECT 	0
#define DISCONNECT	1	 

#define DEFAULT_TORQ_60 			60
#define DEFAULT_TORQ_70 			70
#define DEFAULT_TORQ_75 			75
#define DEFAULT_TORQ_80 			80

#define DEFAULT_TORQ_10W 			60
#define DEFAULT_TORQ_10W_HUD		70
#define DEFAULT_TORQ_30W_GEAR 		55  

#define DEFAULT_TORQ_30W_BELT 			70  
#define DEFAULT_TORQ_30W_BELTSPRING 	70  

#define AUTOSET_DEFAULT_TORQ	70
#define STR_AUTOSET_DEFAULT_TORQ	"70"		 

#define MAIN_MOT_FS_MIN_DUTY 	60  
#define MAIN_MOT_FS_MAX_DUTY	80  

#define MAIN_MOT_DEFAULT_MIN_DUTY 	60
#define MAIN_MOT_DEFAULT_MAX_DUTY	80

	#define DEFAULT_TORQ	DEFAULT_TORQ_30W_BELTSPRING		 

#define BBNBELT_AP_TORQ_ADDVAL 4

#define DEFAULT_BBN_PWR		13	 
#define WAVE_ADD_BBN_PWR	2

#define PROG_START		1
#define GAME_WAIT		2		 
#define GAME_STBY		3		 
#define CAST_WAIT		4		 
#define CAST_COMP		5
#define GAME_WAVE		6
#define GAME_BITE		7
#define GAME_HIT		8
#define GAME_FIGHT		9
#define GAME_HOLDON		10
#define GAME_RANDING	11
#define GAME_FAIL		12
#define GAME_SUCCESS	GAME_FAIL	 
#define GAME_POINTUP	13
#define GAME_CONTINUE	14
#define GAME_OVER		15

#define PROG_END		16

#define GAME_MAX_ENUM	17

#define MAIN_DEV_CHK_OK		"0000"

#define MAIN_BLDC_MOT_ERR	"6010"
#define MAIN_TORQ_MOT_ERR	"6011"
#define MAIN_WIRE_BROKEN_ERR	"6012"
#define MAIN_WIRE_NOTSTABLE_ERR	"6013"

#define MAIN_BBNENCA_SEN_ERR	"6020"
#define MAIN_BBNENCB_SEN_ERR	"6021"

#define MAIN_ROD_COMM_ERR	"6030"

#define MAIN_BOARD_FLASH_FS_ERR	"6040"		 
#define MAIN_BOARD_BLDC_24V_ERR	"6041"

#define MAIN_MMOT_AUTOSET_ERR	"6050"

#define ROD_BAT_LOW_WARN	"7020"		 
#define ROD_IMU_COMM_ERR	"7030"

#define ApPrint		if (g_SerialEnable) print
#define ApPrintf	if (g_SerialEnable) Serial.printf
#define ApPrintln	if (g_SerialEnable) Serial.println

#define Resp2ApPrintln if (g_SerialEnable) Serial.println		 
#define StsSendPrintln if (g_SerialEnable) Serial.println

#define LogSerial	Serial1
#define LogPrint	Serial1.print
#define LogPrintf	Serial1.printf("%10d",curr_ms_tick);Serial1.printf
#define LogPrintln	Serial1.printf("%10d",curr_ms_tick);Serial1.println

#define LogPrintTime	Serial1.printf("%10d",curr_ms_tick)
#define LogPrintTimeln  Serial1.println

#define ExtSerial	Serial2
#define send2extPrint	Serial2.print
#define send2extPrintf	Serial2.printf		 
#define send2extPrintln	Serial2.println

#define DIAG_OFF	0
#define DIAG_ON		1

#define DIAG_MOT_OFF 0
#define DIAG_MOT_CW  1
#define DIAG_MOT_CCW 2

#define ON	HIGH
#define OFF	LOW

#define LED_ON	HIGH
#define LED_OFF	LOW

#define IDLE_MODE	0
#define NORMAL_MODE	1
#define DIAG_MODE	3
#define REV04_MODE	4

#define LEVEL_EASY 		1
#define LEVEL_NORMAL	2
#define LEVEL_HARD		3

#define SUBAC_ON_LVL LOW
#define SIPPAGOE 		10  

#define LED_MAX_DUTY	255		 
#define LED_OFF_DUTY	0

#define LED_MAX_COLOR	255			 
#define LED_MIN_COLOR	0

#define LED_CON_OFF		0
#define LED_CON_ON		1
#define LED_CON_BLINK	2
#define LED_CON_DIMM	3

#define LED_IDX_CENT	1
#define LED_IDX_BTM		2
#define LED_IDX_LEFT	3
#define LED_IDX_RIGHT	4

#define LED_IDX_MAX	5	 

#define COLOR_WHITE		0
#define COLOR_RED		1
#define COLOR_GREEN		2
#define COLOR_BLUE		3
#define COLOR_YELLOW	4
#define COLOR_CYAN		5
#define COLOR_MAGENTA	6

#define COLOR_RESV07	7	 
#define COLOR_RESV14	14	 

#define COLOR_BLACK		15

#define IMU_DATA_OFF	0
#define IMU_DATA_ON		1

#define BTN_LED_MAX_SIZE	2

#define BTN_LED_LEFT_IDX	0
#define BTN_LED_RIGHT_IDX	1

#define uint_t	(unsigned int)

#define STEP_IDLE	0
#define STEP_START	10
#define STEP_END	90

#define LM_LEFT_HOME_RETURN		80
#define LM_RIGHT_HOME_RETURN	80

#define	NVM_OFS_MANUFACT	0x0000	 
#define NVM_OFS_PART_LIFE	0x0400	 
#define NVM_OFS_ERROR_LIST	0x0C00	 
#define NVM_OFF_INIT_DONE	0x1400	 
#define NVM_OFF_INIT_DATA	0x1402	 
#define NVM_OFS_SETTING		0x1C00	 
#define NVM_OFS_ADJUST		0x2C00	 
#define NVM_OFS_RSV_END		0x3C00	 

#define NVM_OFS_END			0x8000	 

#define NVM_OFS_FISH_LEVEL	NVM_OFF_ADJUST

#define WIRE_HOME_DIST	(3*1000)	 
#define BBN_ENC_1PULSE_DIST	(10472)	 
#define WIRE_HOME_BBN_CNT	(WIRE_HOME_DIST*1000/BBN_ENC_1PULSE_DIST)	 

#define WRIE_MAX_DIST	(360*10)	 
#define TARGET_SUB_VAL	100		 

#define AP_IS_DF	0
#define AP_IS_TM	1

#define MANUF_INFO		1
#define INSTALL_INFO	2
#define MAINMOT_INFO	3
#define BLDC_LIMIT_INFO	4
#define BOOTING_INFO	5			 

#define BAT_INT_UNKNOWN		-1		 
#define BAT_INT_DISCONN		-2		 
#define BAT_STR_CHAGER_NOCHANGE		"-3%"	 
#define BAT_STR_CHAGER_CHANGING		"-4%"	 
#define BAT_STR_CHAGER_USE			"-5%"	 

#define BAT_LVL_LOW		15		 

#define MAIN_BOARD_V10		1	 
#define MAIN_BOARD_V11		2	 

#define CHECK_TIME_1SEC_TO	1000
#define CHECK_TIME_2SEC_TO	2000
#define CHECK_TIME_3SEC_TO	3000
#define CHECK_TIME_4SEC_TO	4000
#define CHECK_TIME_5SEC_TO	5000

#define CHECK_TIME_10SEC_TO	(10 * 1000)	 
#define CHECK_TIME_20SEC_TO	(20 * 1000)	 

#define BLDC_LIMIT_MAX	255
#define STR_BLDC_LIMIT_MAX	"255"
#define BLDC_LIMIT_MIN	100
#define STR_BLDC_LIMIT_MIN	"100"
