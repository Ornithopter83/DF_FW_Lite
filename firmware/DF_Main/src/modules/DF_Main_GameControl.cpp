#include "../DF_Main_Internal.h"
#include "DF_Main_GameControl.h"

void diagInputMonitor()
{
	String logStr = "";
	int i = 0;

	{

		if(diagInFlag_BootSwc) { sensorBootSwc = (digitalRead(BOOT_MODE_PIN)? 0:1); }	 
		if(diagInFlag_PcUsb5V) { sensorUsb5V = (digitalRead(PC_USB5V_PIN)? 1:0); }

		if(diagInFlag_PonSwc) { sensorPonSwc = (digitalRead(PWROFF_SWC_PIN)? 0:1); }	 

		if(diagInFlag_BbnBldcFG) { sensorBbnFG = (digitalRead(BLDC_FG_PIN)? 1:0); }

		if(diagInFlag_BobbinEncA) { sensorBbnEncA = (digitalRead(BBN_ENC_A_CK_PIN)? 1:0); }
		if(diagInFlag_BobbinEncB) { sensorBbnEncB = (digitalRead(BBN_ENC_B_DT_PIN)? 1:0); }

		if(diagInFlag_BobbinEncDir)		 
		{
			sensorBbnEncCnt = mainEnc.bbnCnt;
			sensorBbnEncDir = mainEnc.bbnDir;
		}

		if(stDiagSen[DIAG_SEN_LMDIR].flag || stDiagSen[DIAG_SEN_LMPULSE].flag)	 
		{
			sensorBbnEncCnt = mainEnc.bbnCnt;
			sensorBbnEncDir = mainEnc.bbnDir;
		}

		if(diagInFlag_BootSwc && (oldsensorBootSwc != sensorBootSwc))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"01-02," + String(sensorBootSwc) + "%");

			LogPrintln(" LG] IO_in 01-02:" + fillZero2StringSign4Char(oldsensorBootSwc) +"->" + fillZero2StringSign4Char(sensorBootSwc));
			oldsensorBootSwc = sensorBootSwc;
		}

		if(diagInFlag_PcUsb5V && (oldsensorUsb5V != sensorUsb5V))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"05-02," + String(sensorUsb5V) + "%");

			LogPrintln(" LG] IO_in 05-02(Usb#5vDect):" + fillZero2StringSign4Char(oldsensorUsb5V) + "->" + fillZero2StringSign4Char(sensorUsb5V));	
			oldsensorUsb5V = sensorUsb5V;
		}

		if(diagInFlag_PonSwc && (oldsensorPonSwc != sensorPonSwc))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"05-01," + String(sensorPonSwc) + "%");

			LogPrintln(" LG] IO_in 05-01(PwrSwc):" + fillZero2StringSign4Char(oldsensorPonSwc) + "->"+fillZero2StringSign4Char(sensorPonSwc));	
			oldsensorPonSwc = sensorPonSwc;
		}

		if(diagInFlag_Interlock && (oldsensorDoorIlk != sensorDoorIlk))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"05-03," + String(sensorDoorIlk) + "%");

			LogPrintln(" LG] IO_in 05-03:" + fillZero2StringSign4Char(oldsensorDoorIlk) + "->" +fillZero2StringSign4Char(sensorDoorIlk));	
			oldsensorDoorIlk = sensorDoorIlk;
		}

		if(diagInFlag_BobbinEncA && (oldsensorBbnEncA != sensorBbnEncA))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"03-01," + String(sensorBbnEncA) + "%");

			LogPrintln(" LG] IO_in 03-01:" + fillZero2StringSign4Char(oldsensorBbnEncA) + "->" + fillZero2StringSign4Char(sensorBbnEncA));
			oldsensorBbnEncA = sensorBbnEncA;
		}
		if(diagInFlag_BobbinEncB && (oldsensorBbnEncB != sensorBbnEncB))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"03-02," + String(sensorBbnEncB) + "%");

			LogPrintln(" LG] IO_in 03-02:" + fillZero2StringSign4Char(oldsensorBbnEncB) + "->" + fillZero2StringSign4Char(sensorBbnEncB));	
			oldsensorBbnEncB = sensorBbnEncB;
		}

		if(diagInFlag_BobbinEncDir && ( (oldsensorBbnEncCnt != sensorBbnEncCnt) || (oldsensorBbnEncDir != sensorBbnEncDir)))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }

			ApPrintln(stxMsg +"03-03," + String(mainEnc.bbnDir)+"," + String(sensorBbnEncCnt) + "%");

			logStr = " LG] IO_in 03-03:";
			logStr += (sensorBbnEncDir? "CW_," : "CCW,");
			LogPrintln(logStr + "bbnCnt:" + stringSign6Char(oldsensorBbnEncCnt) + "->" + stringSign6Char(sensorBbnEncCnt));
			oldsensorBbnEncCnt = sensorBbnEncCnt;
			oldsensorBbnEncDir = sensorBbnEncDir;
		}

		if(diagInFlag_AngleHomeSns && (oldsensorAngHomeSns != sensorAngHomeSns))
		{
			ApPrintln(String(STX_DIAG_INPUT) +"03-04" + fillZero2StringSign4Char(sensorAngHomeSns) + "%");

			LogPrintln(" LG] IO_in 03-04:" + fillZero2StringSign4Char(oldsensorAngHomeSns) + "->" + fillZero2StringSign4Char(sensorAngHomeSns));	
			oldsensorAngHomeSns = sensorAngHomeSns;
		}

		if(diagInFlag_BbnBldcFG && (oldsensorBbnFG != sensorBbnFG))
		{
			String stxMsg;
			if(AP_IS_TM == apType)	{ stxMsg = STX_TM_INPUT_REQ; }
			else					{ stxMsg = STX_DIAG_INPUT; }
			ApPrintln(stxMsg +"03-05," + String(sensorBbnFG) + "%");

			LogPrintln(" LG] IO_in 03-05:" + fillZero2StringSign4Char(oldsensorBbnFG) + "->" +fillZero2StringSign4Char(sensorBbnFG));
			oldsensorBbnFG = sensorBbnFG;
		}

		if(1)
		{
		}

	}

}

void diagInputMotorSensorAllOff()
{
	diagInFlag_BobbinEncA = 0;
	diagInFlag_BobbinEncB = 0;

	diagInFlag_SelectBoard = 0;
}
