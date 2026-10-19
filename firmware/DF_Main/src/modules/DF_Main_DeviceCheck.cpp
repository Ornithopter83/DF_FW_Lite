 
#include "../DF_Main_Internal.h"
#include "DF_Main_DeviceCheck.h"
void devChkReady_Control()
{
	if(0)	{}	 
	else if(devCheckAllFlag)		{ devchk_bbnAll_Control(); }

	 
	else
	{
		 
		devchk_OkEnd();			 
	}
	 
	 

}


void resp_MainBoard_Ver()
{
	String respMsg;
	 
	respMsg = STX_INFO_REQ + MAIN_BOARD_TYPE + strMainBoard[mainBoardType]+ "%";
	Resp2ApPrintln(respMsg);
}


 
 
void devchk_OkEnd()
{
	int outTorq = 0;

	 
	 
	devReadychkFlag = 0;	 
	devChkMsgSend(MAIN_DEV_CHK_OK);

	 
	resp_MainBoard_Ver();
	
	
	 
	 
	if(AP_IS_TM == apType)		 
	{
		torqOff();
		outTorq = 0;
	}
	 
	else
	{
 










					
	}
	 
	send2resp_FwSelfControlCmd(STX_TORQUE_MOTOR, outTorq);

}

 
void send2resp_FwSelfControlCmd(String Cmd, int torq)
{
	Resp2ApPrintln(Cmd + stringTo3char(torq) + "%");
}

String stringTo3char(int val)
{
	String strRts;

	if(  0 > val)	val = 0;
	if(999 < val)	val = 999;

	if(0)				{}  
	else if(100 < val)	{strRts = String(val); }		 
	else if( 10 < val)	{strRts = "0" + String(val); }	 
	else				{strRts = "00" + String(val); }	 

	return strRts;
}

 
void devChkMsgSend(String msg)
{

	 
	msg = STX_DEV_CONT_REQ + msg;	 
	 
	msg += "%";
	Resp2ApPrintln(msg);
}

 
 
 
void devchk_bbnAllStart()
{
	devCheckAllFlag = 1;
	devBbnAllChkStep = 10;
}


 
void devCheckFlagStep_AllClear()
{
	devCheckAllFlag = 0;
	devBbnAllChkStep = 0;
}

#define DEVCHK_BBN_DIR_CHANGE_TO	50  
#define DEVCHK_BBN_RETRY_OFF_TO		50	 

#define DEVCHK_ENC_SEN_TO	300  

#define DEVCHK_TORQ_MOT_ON_TO	300  
#define DEVCHK_TORQ_MOT_STEP_TO	100	 
#define DEVCHK_TORQ_START_DELAY_TO	150

#define DEVCHK_TORQ_MOT_OFF_TO	50  


 
#define DEVCHK_BBN_DEFAULT_DUTY	30
#define DEVCHK_BBN_STEP_DUTY	10		 

#define DEVCHK_BBN_WIRE_REL_ON_TO	300		 

#define DEVCHK_BLDC24V_REL_ON_TO	150  

#define DEVCHK_BLDC24V_BREAK_TO		50		 

#define DEVCHK_TORQ_TO_ENCA_1ST_ON_CHK_TO	600
#define DEVCHK_TORQ_TO_ENCA_1ST_ON_SPEC_TO	250  

#define DEVCHK_BLDC24V_RETRY_WAIT_TO	500  

#define DEVCHK_BLDC24V_OFF_WAIT_TO		1500	 

#define DEVCHK_BLDC24V_SEN_CNT_TO	1200  

#define DEVCHK_TORQ_STOP_WAIT_TO	500
#define	DEVCHK_TORQ_MOT_CHK_CNT		(9*2)		 

#define DEVCHK_LM_DUTY	35		 
 



#define RETRY_CNT	3	 
#define IN_SIG_CHG_CNT (3*2)	 


typedef struct _stDevchkBbn
{
	 
	unsigned short duty;
	unsigned short ccw_ontime;	
	unsigned short cw_ontime;	
}stDevchkBbn;

#define DEVCHK_BBNMOT_SIZE	(1 + 3)
stDevchkBbn	stDevchkBbnMot[DEVCHK_BBNMOT_SIZE] =
{
	 
	{0,},	 
	 
	{30, 300, 300},	 
	 
	{40, 250, 250},	 
	 
	{50, 200, 200},	 
};

 
 
 
 
 
 
 
void devchk_bbnAll_Control()
{
	static unsigned long _to;
	static unsigned long _toSenChg;
	static unsigned long pulseWidthMs;
	
	static unsigned short olddevBbnAllChkStep = 0;
	static short errK = ERR_OK;

	static unsigned short _oldSen;
	unsigned short _newSen, _chgSen;

	static unsigned short _chgCnt  = 0;
	static unsigned short _chgCntA = 0;
	static unsigned short _chgCntB = 0;
	static unsigned short _chgCntF = 0;

	static int nDevChkErrCount = 0;
	static int nDevChkTorqDuty = 0;
	static int _nEncCount[4] = {0};
	static String msgErr;
	String msg;

	
	switch(devBbnAllChkStep)
	{
		 
		case 0:
			break;
			
		 
		case 10:
			_oldSen = LVL01_BBNALL;
			torqOff();					 
			bbnMotor.offBldc();

			errK = ERR_OK;
			msgErr = MAIN_DEV_CHK_OK;
			setTO(_to);
			devBbnAllChkStep = 14;
			break;

		case 14:
			if(checkTO(_to, DEVCHK_TORQ_MOT_OFF_TO) )		 
			{
				setTO(_to);
				devBbnAllChkStep = 20;
			}
			break;

		 
		case 20:
			_nEncCount[0] = iIsrbbnEncCnt;
			_nEncCount[1] = _nEncCount[2] = 0;
			bbnMotor.onBldc(BBN_MOT_CCW, DEVCHK_BBN_WIRE_REL_DUTY);
			_oldSen = LVL01_BBNALL;				 
			_chgCntA = 0;						 
			_chgCntB = 0;						 
			_chgCntF = 0;						 
			nDevChkTorqDuty = 0;
			setTO(_to);
			devBbnAllChkStep = 21;
			break;

		case 21:								 
			if(checkTO(_to, DEVCHK_BBN_WIRE_REL_ON_TO) )		 
			{
				bbnMotor.offBldc();
				 
				 
				if ((IN_SIG_CHG_CNT < _chgCntF) && (IN_SIG_CHG_CNT < _chgCntA) && (IN_SIG_CHG_CNT < _chgCntB))		 
				{
					setTO(_to);
					devBbnAllChkStep = 29;	 
				}
				 
				else
				{
					if (IN_SIG_CHG_CNT >= _chgCntF)				 
						msgErr = MAIN_BLDC_MOT_ERR;				 
					else if (IN_SIG_CHG_CNT >= _chgCntA)		 
						msgErr = MAIN_BBNENCA_SEN_ERR;			 
					else if (IN_SIG_CHG_CNT >= _chgCntB)		 
						msgErr = MAIN_BBNENCB_SEN_ERR;			 
					else
						msgErr = MAIN_BLDC_MOT_ERR;
					devBbnAllChkStep = 75;	 
					errK = ERR_CCW;
				}
				_nEncCount[1] = iIsrbbnEncCnt;

			}
			else {
				_newSen = LVL01_BBNALL;						 
				_chgSen = _oldSen ^ _newSen;				 
				if(_chgSen)									 
				{
					_oldSen = _newSen;						 
					if (_chgSen & BIT_SNS_BBNFG)			 
						_chgCntF++;							 
					if (_chgSen & BIT_SNS_BBNENCA)			 
						_chgCntA++;							 
					if (_chgSen & BIT_SNS_BBNENCB)			 
						_chgCntB++;							 
				}
			}
			break;

		 
		case 29:
			if(checkTO(_to, DEVCHK_BLDC24V_OFF_WAIT_TO))	 
			{
				devBbnAllChkStep = 30;	 
			}
			break;

		 
		 
		case 30:
			_oldSen = LVL01_BBNENCA;
			_chgCnt = 0;			 
			_nEncCount[1] = iIsrbbnEncCnt;
 
			nDevChkTorqDuty = defaultTorqueMotor + 3 * ((nDevChkErrCount <= 2) ? nDevChkErrCount : 2);
			if (MAIN_MOT_FS_MAX_DUTY < nDevChkTorqDuty)
				nDevChkTorqDuty = MAIN_MOT_FS_MAX_DUTY;
			torqMotor.setValue(nDevChkTorqDuty); 		 
			setTO(_to);
			devBbnAllChkStep = 34;
			break;

		case 34:
			if(checkTO(_to, DEVCHK_BLDC24V_SEN_CNT_TO) || (DEVCHK_TORQ_MOT_CHK_CNT <= _chgCnt))		 
			{
				torqOff();
				 
				 
				if( DEVCHK_TORQ_MOT_CHK_CNT > _chgCnt)	 
				{
					nDevChkErrCount++;
					msgErr = MAIN_BOARD_BLDC_24V_ERR;
					errK = ERR_CW;
				}
				else
				{
					errK = ERR_OK;
				}
				_nEncCount[2] = iIsrbbnEncCnt;
				devBbnAllChkStep = 75;
				
				 
			}
			else if(LVL01_BBNENCA != _oldSen)	 
			{
				_oldSen = LVL01_BBNENCA;		 
				_chgCnt++;
				pulseWidthMs = getTO(_toSenChg);
				setTO(_toSenChg);
			}
			break;

		 
		case 75:
			bbnMotor.offBldc();
			 
			Resp2ApPrintln( STX_DEV_CHK_ERR_LOG + msgErr + (ERR_OK==errK? "OK":"NG") + ",Err:" + String(nDevChkErrCount)+",Cnt:" + String(_chgCnt)+"("+ String(_chgCntF)+","+ String(_chgCntA)+","+ String(_chgCntB)+"),ms:" + String(getTO(_to)) + ", Enc:" + String(_nEncCount[0]) + "," + String(_nEncCount[1]) + "," + String(_nEncCount[2])+ ",Duty:" + String(nDevChkTorqDuty));
			if(ERR_OK == errK)	 
			{
				devBbnAllChkStep = 90;		 
			}
			else	 
			{
				devBbnAllChkStep = 80;		 
			}
			break;


		 
		case 80:
			devReadychkFlag = 0;	 
			devCheckFlagStep_AllClear();
			
			msg = STX_DEV_CONT_REQ + msgErr + "%";
			Resp2ApPrintln(msg);

			break;
			
		 
		case 90:
			devCheckAllFlag = 0;	 
			devBbnAllChkStep = 0;		 
			if (defaultTorqueMotor != nDevChkTorqDuty) {					 
				fsInfo.saveInfo(MAINMOT_INFO, String(nDevChkTorqDuty));		 
				defaultTorqueMotor = nDevChkTorqDuty;						 
				nDevChkErrCount = 0;										 
			}
			break;

		default:
			break;
	}
}



 
 


 
 



 
 
 

 


 
 
 
 
 
 
 
 
 
 
void devReadyCheck()
{
	 
	devReadychkFlag = 1;

	devChkErrOccure = 0;
	 

	devchk_bbnAllStart();

	 
	 
}

 
void send_Error_Clr()
{
	String respMsg;
	respMsg = STX_DEV_CONT_REQ + "0000";	 
	respMsg += "%";
	Resp2ApPrintln(respMsg);
}

 
 
