

#define DEF_NOT_USE	(0)		 

	#define AUTO_TENSION_PID	(0)		 

#define DEV_CHECK_SKIP		(0)		 

#define PWR_SW_OFF_CPU_SOFT_RESET	(0)

	#define SYS_TIMER_INTR_ENB		(1) 	 
	#define GPIO_INTR_ENCA_ENB		(1) 	 

		#define GPIO_INTR_PID_CONT	(0) 	 

#define HAND_TEST_01		(1)			 
#define HAND_TEST_02		(0)			 
#define BBN_BLDC_TEST 		(1)			 
#define HAND_TEST_03		(BBN_BLDC_TEST)			 
#define FW_CONDITION_CHK 	(0)			 
#define CMD_CONTROL_2407E	(0)
#define NEW_IO_DIAG			(1)			 
#define NEW_IF				(1)			 

#define AP_MOT_CONT			(1)		 

	#define COST_10W_CHANGE (0)		 

#define	FW_CONT_BITE_HOLD	(1)		 

#define CONFIG_TORQ_WATT_30W	(1)

	#define CONFIG_TORQ_HUD			(0)		 
	#define BBN_SPRING_AUTOTENSION	(1)

#define MC_CONF_NO_4	(1)		 

#define BBN_MOT_16W		(1)
	#define BBN_MOT_38W	(0)		 

#define IO_TORQ		(1)

#define IO_SERVO	(0)		 

#define IO_BBN	 	(1)

#define IO_BREAK_MOT	(0)		 

#define IO_DLED		(1)			 

#define IO_PC_PWR_OFF 	(1)		 

#define IO_I2C		(1)

#define IO_NVM_TEST		(0)		 

#define IO_EXTIO_UART_PIN		(0)		 

#define CPU_RESET_PIN_SUPPORT	(0)

#define FUNC_IO_EXT_UART		(0)		 

#define FUNC_LOG_UART_RCV_ENB		(1)   

#define FUNC_RF_CERTIFICATION_TEST	(0)		 

#define FUNC_STBY_OFF_CONT	(1)			 

#define FUNC_FET_PROTECT	(1)			 

#define PID_PARA_SET_ENB		(0)		 

#define RESP_IO				(0)  
#define RESP_IO_TEST		(0)		 

#define RESP_IO_MAIN_TORQ	(1)
#define RESP_IO_BBN_BLDC	(1)

#define RESP_LED_CONTROL_COMMAND	(0)  

#define RESP_IMU_DATA		(1)

#define RESP_GMWAT_CMD		(1)		 

#define LOG_BBN_ENC_LOGOUT	(0)	 

#define LOG_AP_CMD			(1)	 

#define LOG_MAIN_CONN		(0)		 
#define LOG_DEV_CONN		(1)		 

#define LOG_IO				(0)  
#define LOG_IO_TORQ			(1)			 

#define LOG_BLDC_AP_CMD 	(1)		 
#define LOG_IO_BBN			(1)		 
#define LOG_IO_LM			(0)  

#define LOG_FW_BAT_CHG		(0)

#define LOG_ROD_BTN			(0)		 
#define LOG_ROD_ENC			(0)		 

#define LOG_FISH_REGIST		(1)		 

#define LOG_FUNC_STEP		(0)
#define LOG_FUNC_STEP_BITE	(0)		 
#define LOG_LED_STEP		(0)

#define LOG_I2C			(0)  

#define LOG_ELEC_IO		(0)  

#define LOG_LE_TE			(0)  
#define LOG_LE_TE_1MS		(0)  
#define LOG_LE_TE_10MS		(0)  

#define LOG_READ_IN_1MS		(0)
#define LOG_READ_IN_10MS	(0)

#define LOG_PWRSWC_OFFON	(1)  
#define LOG_POWEROFF_FUNC	(1)  

#define LOG_CONT_STEP		(1)		 

#define LOG_MKT_TEST_23		(1)		 

#define LOG_BITE_IMPROV		(0)	 
#define LOG_HOLDON_IMPROV	(0)  

#define LOG_DEV_CHECK_OK	(0)  

#define LOG_NOW_GET_INFO	(0)  

#define LOG_TART_ADDR_LOD	(1)		 

#define LOG_AP_RCV_MSG_PARSING		(0)				 

#define LOG_UART1_RCV_MSG_PARSING	(0)				 
#define LOG_UART1_RCV_MSG			(0)

#define LOG_SYSTICK_DELAY		(1)		 
#define LOG_1MS_EXE_DELAY		(1)		 

#define LOG_IMU_RCV_1ST_TIME	(0)		 

#define LOG_ROD_ALV_CHK		(0)  
#define LOG_ROD_ALV_RESP	(0)  

#define LOG_NOW_SEND_ERR		(1)		 
#define LOG_NOW_SEND_SEQNO		(0)		 
#define LOG_NOW_CB_SENT_FAIL	(1)		 
#define LOG_NOW_CB_SENT_REGI	(1)		 

#define LOG_NOW_RCV_OVER_WRITE	(1) 	 
#define LOG_NOW_RCV_CB_SEQNO	(0)
#define LOG_NOW_RCV_ANA_PID		(0)		 

#define LOG_ROD_REGI_TO_STEP			(1)	 
#define LOG_BC_ROD_ADDR_WRITE_STEP		(0)	 
#define LOG_BC_ROD_TART_ADDR_WRITE_TIME	(0)	 
#define LOG_BC_MAIN_ADDR_SEND			(1)	 

#define LOG_VRT_AP_CMD			(0)			 
#define LOG_VRT_AP_CMD_ERR		(1)			 
#define LOG_BTN_LED_AP_CMD		(0)
#define LOG_BTN_LED_AP_CMD_ERR	(0)

#define LOG_IO_LM_MOT		(0)		 

#define LOG_LMMOT_AP_CMD	(0)		 

#define LOG_LM_SET_PARA_OUT	(0)		 

#define LOG_CONFIG			(0)		 

#define LOG_GAME_WAT_CMD	(0)	 

#define LOG_INTR_ENC_A		(0)		 

#define LOG_WAT_FISH_LEVEL	(0)

#define LOG_LM_WIRE_CONTROL	(0)
#define LOG_AP_TYPE			(0)			 

#define LOG_MANUF_INFO		(0)			 

#define LOG_DEVCHK_BBN_MOT	(0)			 
#define LOG_ENC_SEN_ERR		(0)			 
#define LOG_DEVCHK_TORQ_ERR	(0)			 

#define LOG_DEVCHK_BLDC24V_ERR	(0)		 

#define LOG_MAIN_BOARD_TYPE		(0)		 

#define LOG_MAINMOT_AUTOSET_ERR			(0)		 
#define LOG_MAINMOT_AUTOSET_LAST_PULSE	(0)		 

#define LOG_WIRE_AGING_TEST		(0)		 
#define LOG_WIRE_AGING_TEST_ERR	(0)		 

#define WIRE_TEST_DESK	(0)			 

#define LOG_MMOT_AUTOSET_STEP	(0)		 

#define LOG_STBY_TORQ_OFF_STEP	(0)		 
#define LOG_STBY_TORQ_OFF_ERR	(0)		 
