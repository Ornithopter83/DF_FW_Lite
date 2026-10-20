
#include "../DF_Main_Internal.h"
#include "DF_Main_Diagnostics.h"

void mDiag_entry()
{
	mainMode = DIAG_MODE;
	 

	if( oldmainMode != mainMode)
	{
		oldmainMode = mainMode;

		LogPrintln(" LG] DIAG_ Entry");
		 
		motor_AllOff();
		 
		 
		extLed_controlOff();
		
		reelOut_AllOff();	 
	}

}

 

void mDiag_exit()
{
	diagInput_AllOff();
	diagOutput_AllOff();

	initializeLedControlValues(1);		   
	 
	ledOn_MbBtm();

	mainMode = NORMAL_MODE;

	LogPrintln(" LG] DIAG_ EXIT");

	 
}

void diagInput_AllOff()
{
	diagInput_BoardInAllOff();
	DiagInput_MotInAllOff();
	DiagInput_MechaInAllOff();
	DiagInput_ElecInAllOff();

}

#define DIAG_CHANGE_VAL	3		 

void diagInput_BoardInAllOff()
{

	diagInFlag_ResetSwc = 0; 	
	diagInFlag_BootSwc = 0; 	
	diagInFlag_WdtLbSig = 0; 	
	diagInFlag_SelectBoard = 0;
}
void diagInput_BoardInAllOn()
{

	diagInFlag_BootSwc = 1; 	
	oldsensorBootSwc = sensorBootSwc + DIAG_CHANGE_VAL;

}

void DiagInput_MotInAllOff()
{
	diagInFlag_BobbinEncA = 0;
	diagInFlag_BobbinEncB = 0;
	diagInFlag_BobbinEncDir = 0;
	diagInFlag_AngleHomeSns = 0;
	diagInFlag_BbnBldcFG = 0;	
}

void DiagInput_MotInAllOn()
{

	diagInFlag_BbnBldcFG = 1;
	oldsensorBbnFG = sensorBbnFG + DIAG_CHANGE_VAL;
}

void DiagInput_MainEncoderInAllOn()
{
	diagInFlag_BobbinEncA = 1;
	oldsensorBbnEncA = sensorBbnEncA + DIAG_CHANGE_VAL;
	diagInFlag_BobbinEncB = 1;
	oldsensorBbnEncB = sensorBbnEncB + DIAG_CHANGE_VAL;

	if(AP_IS_TM == apType)
	{
		diagInFlag_BobbinEncDir = 1;
		oldsensorBbnEncCnt= sensorBbnEncCnt + DIAG_CHANGE_VAL;
	}

}

void DiagInput_MechaInAllOff()
{

}

void DiagInput_ElecInAllOff()
{
	diagInFlag_PonSwc = 0;
	diagInFlag_PcUsb5V = 0;

}
void DiagInput_ElecInAllOn()
{
	diagInFlag_PonSwc = 1;
	oldsensorPonSwc = sensorPonSwc + DIAG_CHANGE_VAL;
	diagInFlag_PcUsb5V = 1;
	oldsensorUsb5V = sensorUsb5V + DIAG_CHANGE_VAL;

}

void diagInput_Cat0(String _msg)
{

	int iItemNo = _msg.substring(6,(6+2)).toInt();
	switch(iItemNo)
	{
		case 0:
			diagInput_AllOff();

			LogPrintln(" LG] IO_in All Input mon OFF");
			break;
		 default:

			 LogPrintln(" LG] IO_in Can't Find Item No");
			 break;
	 }

}

void diagInput_Cat1(String _msg)
{
	int iItemNo = _msg.substring(6,(6+2)).toInt();

	int iAct = _msg.substring(9,(9+1)).toInt();

	int iPara = _msg.substring(11,(11+3)).toInt();	 

		   switch(iItemNo)
		   {
			   case 0:
				   if(DIAG_OFF == iAct)
				   {
					   diagInput_BoardInAllOff();
				   }
				   else if(DIAG_ON == iAct)
				   {
					   diagInput_BoardInAllOn();
				   }

				   LogPrintln(" LG] IO_in Board Input All OFF");
				   break;
			   case 1:
				   if(DIAG_OFF == iAct)
				   {
					   diagInFlag_ResetSwc = 0; 		    

					   LogPrintln(" LG] IO_in Reset SWC mon ON");
				   }
				   else if(DIAG_ON == iAct)
				   {
					   diagInFlag_ResetSwc = 1; 		    
					   oldsensorResetSwc = sensorResetSwc + DIAG_CHANGE_VAL;

					   LogPrintln(" LG] IO_in Reset SWC mon ON");
				   }
				   else
				   {

					   LogPrintln(" LG] IO_in Can't Find Act No");
				   }
				   break;
			   case 2:
				   if(DIAG_OFF == iAct)
				   {
					   diagInFlag_BootSwc = 0;			    

					   LogPrintln(" LG] IO_in Boot SWC mon OFF");
				   }
				   else if(DIAG_ON == iAct)
				   {
					   diagInFlag_BootSwc = 1;			    
					   oldsensorBootSwc = sensorBootSwc + DIAG_CHANGE_VAL;

					   LogPrintln(" LG] IO_in Boot SWC mon ON");
				   }
				   else
				   {
				   }
				   break;
			   case 3:
				   if(DIAG_OFF == iAct)
				   {
					   diagInFlag_WdtLbSig = 0; 		    
				   }
				   else if(DIAG_ON == iAct)
				   {
					   diagInFlag_WdtLbSig = 1; 		    
					   oldsensorWdtFbSig = sensorWdtFbSig + DIAG_CHANGE_VAL;

					   LogPrintln(" LG] IO_in WDT LB Signal mon ON");
				   }
				   else
				   {
				   }
				   break;
			   case 4: 
				   if(DIAG_OFF == iAct)
				   {
					   diagInFlag_SelectBoard = 0;			    
				   }
				   else if(DIAG_ON == iAct)
				   {
					   diagInFlag_SelectBoard = 1; 
					   oldsensorSelBoard = sensorSelBoard + DIAG_CHANGE_VAL;

					   LogPrintln(" LG] IO_in Door Interlock mon ON");
				   }
				   break;  
			   default: 

				   LogPrintln(" LG] IO_in Can't Find Item No");
				   break;
		   	}
}

void diagInput_Cat3(String _msg)
{

	String logmsg;

	int iItemNo = _msg.substring(6,(6+2)).toInt();

	int iAct = _msg.substring(9,(9+1)).toInt();

	int iPara = _msg.substring(11,(11+3)).toInt();	  

	switch(iItemNo)
	{
		case 0:
			if(DIAG_OFF == iAct)
			{
				DiagInput_MotInAllOff();
			}
			else if(DIAG_ON == iAct)
			{
				DiagInput_MotInAllOn();
				DiagInput_MainEncoderInAllOn();
			}
			break;

		case 1: 
			if(DIAG_OFF == iAct)
			{
				 diagInFlag_BobbinEncA = 0; 		 

				 LogPrintln(" LG] IO_in Main Enc_A mon OFF");
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_BobbinEncA = 1; 
				oldsensorBbnEncA = sensorBbnEncA + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Main Enc_A mon ON");
			}
			break;  

		case 2: 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_BobbinEncB = 0;			 
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_BobbinEncB = 1; 
				oldsensorBbnEncB= sensorBbnEncB+ DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Main Enc_B mon ON");
			}
			break;  

		case 3: 					 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_BobbinEncDir = 0;			 
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_BobbinEncDir = 1; 
				oldsensorBbnEncCnt= sensorBbnEncCnt + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Main Enc Dir/Cnt ON");
			}
			break;  

		case 4: 					 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_AngleHomeSns = 0;			 
				LogPrintln(" LG] IO_in Angle HomeSensor mon OFF");
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_AngleHomeSns = 1; 
				oldsensorAngHomeSns = sensorAngHomeSns + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Angle HomeSensor mon ON");
			}
			break;  

		case 5: 					 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_BbnBldcFG = 0;			 
				LogPrintln(" LG] IO_in BBN BLDC FG mon OFF");
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_BbnBldcFG = 1; 
				oldsensorBbnFG = sensorBbnFG + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in BBN BLDC FG mon ON");
			}
			break;  

		default:
			LogPrintln(" LG] IO_in Can't Find Item No");
			break;
	}

}

void diagInput_Cat5(String _msg)
{

	   int iItemNo = _msg.substring(6,(6+2)).toInt();

	   int iAct = _msg.substring(9,(9+1)).toInt();

	   int iPara = _msg.substring(11,(11+3)).toInt();	 

	switch(iItemNo)
	{
		case 0:
			if(DIAG_OFF == iAct)
			{
				DiagInput_ElecInAllOff();
			}
			else if(DIAG_ON == iAct)
			{
				DiagInput_ElecInAllOn();
			}
			break;
		case 1: 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_PonSwc = 0;			 
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_PonSwc = 1; 
				oldsensorPonSwc = sensorPonSwc + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Power On Switch mon ON");
			}
			break;  
		case 2: 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_PcUsb5V = 0; 			 
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_PcUsb5V = 1; 
				oldsensorUsb5V = sensorUsb5V + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Usb 5V mon ON");
			}
			break;  
		case 3: 
			if(DIAG_OFF == iAct)
			{
				diagInFlag_Interlock = 0;			 
			}
			else if(DIAG_ON == iAct)
			{
				diagInFlag_Interlock = 1;
				oldsensorDoorIlk = sensorDoorIlk + DIAG_CHANGE_VAL;

				LogPrintln(" LG] IO_in Door Interlock mon ON");
			}
			break;  

		default: 
			LogPrintln(" LG] IO_in Can't Find Item No");
			break;
	}

}

void diagInputTest(String _msg)
{
	   int iCatNo = 0;
	   int iItemNo = 0;
	   int iAct = 0;
	   int iPara = 0;

	   String logmsg = "";

	   iCatNo = _msg.substring(3,(3+2)).toInt();	 

	   iItemNo = _msg.substring(6,(6+2)).toInt();	 

	   iAct = _msg.substring(9,(9+1)).toInt();		 

	   iPara = _msg.substring(11,(11+3)).toInt();	 

	   if( 0 == iCatNo)
	   {
			diagInput_Cat0(_msg);
	   }
	   else if( 1 == iCatNo)	    
	   {
	   		diagInput_Cat1(_msg);

	   }
	   else if( 2 == iCatNo)		    
	   {

		   LogPrintln(" LG] IO_in Can't Find Cat No");
	   }
	   else if( 3 == iCatNo)		    
	   {
	   		diagInput_Cat3(_msg);
	   }

	   else if( 4 == iCatNo)	    
	   {

	   }

	   else if( 5 == iCatNo)	    
	   {
	   		diagInput_Cat5(_msg);

	   }
	   else if( 11 == iCatNo)	    
	   {
	   }
	   else if( 12 == iCatNo)	    
	   {
	   }
	   else if( 14 == iCatNo)	    
	   {
	   }
	   else if( 15 == iCatNo)	    
	   {
	   }
	   else if( 20 == iCatNo)	    
	   {
	   }
	   else
	   {

		   LogPrintln(" LG] IO_in Can't Find Cat No");
	   }

}

void diagOutput_AllOff()
{
	diagOutput_BoardOutAllOff();
}

void diagOutput_BoardOutAllOff()
{
	setRGBcolor(LED_IDX_CENT, COLOR_BLACK);
}

void setRGBcolor(int pos, int Color)
{
	if(COLOR_MAX_NO > Color)
	{
		sRGB[pos].r = sColorTBL[Color].r;	
		sRGB[pos].g = sColorTBL[Color].g;	
		sRGB[pos].b = sColorTBL[Color].b;
	}
	else
	{
		LogPrintln(" LG] LedRg Can't Find Color Number!!!");
	}
}

void setOutRGBcolor(int pos, int Color)
{
	if(COLOR_MAX_NO > Color)
	{
		outRGB[pos].r = sColorTBL[Color].r;	
		outRGB[pos].g = sColorTBL[Color].g;	
		outRGB[pos].b = sColorTBL[Color].b;
	}
	else
	{
		LogPrintln(" LG] LedRg Can't Find Color Number!!!");
	}
}

void diagBbnMotor_TimeoutStart(unsigned int _time)
{
	bbnTimeout_Val = _time;
	bbnTimeout_step = 0x10;			 
	diagOutflag_bbnTimeOut = 1;		 
}

void diagBbnMotor_TimeoutControl()
{

	switch(bbnTimeout_step)
	{
		case 0x00:	 
			break;
		case 0x10:	 
			setTO(bbnDiagTO);
			bbnTimeout_step = 0x20;
			break;
		case 0x20:	 
			if(checkTO(bbnDiagTO, bbnTimeout_Val))
			{
				bbnTimeout_step = 0x30;
			}
			break;
		case 0x30:	 
			bbnBldcOff();
			bbnTimeout_step = 0x40;
			break;
		case 0x40:	 
			diagOutflag_bbnTimeOut = 0;
			bbnTimeout_step = 0x00;
			break;
		default:

			break;
	}
}

void diagBbnMotor_TokTokEnd()
{
	diagBbnflag_TokTokControl = 0;
	bbnTokTok_step = 0x00;
	bbnMotor.offBldc();
}

void diagBbnMotor_TokTokStart(unsigned int _ackKind, unsigned int _cnt)
{

	bbnActKind = _ackKind;

  	bbnReqCnt[bbnActKind] = _cnt;

	bbnTokTok_step = 0x10;
	diagBbnflag_TokTokControl = 1;		 
}

void diagBbnMotor_TokTokControl(unsigned _opKind)		 
{

	switch(bbnTokTok_step)
	{
		case 0x00:	 
			break;

		case 0x10:	 
			bbnRptCnt = 0;
			setTO(bbnTokTO);
			bbnTokTok_step = 0x20;
			break;

		case 0x20:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnActTbl[bbnActKind].onDuty);	 
			setTO(bbnTokTO);
			bbnTokTok_step = 0x30;
			break;
		case 0x30:
			if(checkTO(bbnTokTO, sBbnActTbl[bbnActKind].onTime))
			{
				bbnMotor.offBldc();				 
				setTO(bbnTokTO);
				bbnTokTok_step = 0x40;
			}
			break;
		case 0x40:	 
			if(checkTO(bbnTokTO, sBbnActTbl[bbnActKind].offTime))
			{
				bbnRptCnt++;
				if( bbnReqCnt[bbnActKind] < bbnRptCnt)
				{
					bbnTokTok_step = 0x70;		 
				}
				else
				{
					bbnTokTok_step = 0x20;			 
				}
			}
			break;

		case 0x50:	 
			break;

		case 0x60:	 
			break;

		case 0x70:	 
			bbnMotor.offBldc(); 			 

			bbnRptCnt = 0;

			bbnTokTok_step = 0x00;
			break;
		default:

			break;
	}
}

void diagOut_Cat1(String _msg)
{
	int iCatNo = _msg.substring(3,(3+2)).toInt();	 

	int iItemNo = _msg.substring(6,(6+2)).toInt();	 

	int iAct = _msg.substring(9,(9+1)).toInt(); 	 

	int iPara = _msg.substring(11,(11+3)).toInt();	 

	int iPara2 = _msg.substring(15,(15+4)).toInt();  

	int iPara3 = _msg.substring(20,(20+5)).toInt();  

	switch(iItemNo)
	{
		case 0:
			diagOutput_BoardOutAllOff();

			LogPrintln(" LG] IOout Board Output All OFF");
			break;
		case 1:
			diagOutFlag_WDToff = 1; 		 

			LogPrintln(" LG] IOout WDT OFF");
			break;
		case 2:
			if(DIAG_OFF == iAct)
			{
				digitalWrite(BD_LED2_PIN, LED_OFF);

				LogPrintln(" LG] IOout BD LED2 OFF");
			}
			else if(DIAG_ON == iAct)
			{
				digitalWrite(BD_LED2_PIN, LED_ON);

				LogPrintln(" LG] IO_in	BD LED2 ON");
			}
			else
			{
			}
			break;

		case 3:
			if(DIAG_OFF == iAct)
			{
				digitalWrite(BD_LED3_PIN, LED_OFF);

				LogPrintln(" LG] IOout BD LED3 OFF");
			}
			else if(DIAG_ON == iAct)
			{
				digitalWrite(BD_LED3_PIN, LED_ON);

				LogPrintln(" LG] IOout BD LED3 ON");
			}
			else
			{
			}
			break;
		case 4: 
			if(DIAG_OFF == iAct)
			{
				digitalWrite(BD_LED4_PIN, LED_OFF);

				LogPrintln(" LG] IOout BD LED4 OFF");
			}
			else if(DIAG_ON == iAct)
			{
				digitalWrite(BD_LED4_PIN, LED_ON);

				LogPrintln(" LG] IOout BD LED4 ON");
			}
			else
			{
			}
			break;  
		case 5 :	 
			if(DIAG_OFF == iAct)
			{
				debugPort_Off();
				LogPrintln(" LG] IOout BD DebugPort OFF");
			}
			else if(DIAG_ON == iAct)
			{
				debugPort_On();
				LogPrintln(" LG] IOout BD DebugPort ON");
			}

			else
			{

			}
			break;

		default: 
			break;
	}

}

void diagOut_Cat3(String _msg)
{
	int iCatNo = _msg.substring(3,(3+2)).toInt();	 

	int iItemNo = _msg.substring(6,(6+2)).toInt();	 

	int iAct = _msg.substring(9,(9+1)).toInt(); 	 

	int iPara = _msg.substring(11,(11+3)).toInt();	 

	int iPara2 = _msg.substring(15,(15+4)).toInt();  

	int iPara3 = _msg.substring(20,(20+5)).toInt();  

	switch(iItemNo)
	{
		case 0 :	 
			torqMotor.setValue(0);
			bbnMotor.offBldc();

			LogPrintln(" LG] IOout Motot ALL OFF");
			break;

		case 1 :	 
			if(DIAG_MOT_OFF == iAct)
			{
				torqMotor.setValue(0);

				LogPrintln(" LG] IOout Main Mot(Torq) OFF");
			}
			else if(DIAG_MOT_CW == iAct)
			{
				torqMotor.setValue(iPara);

				LogPrintln(" LG] IOout Main Mot(Torq) ON, Duty : " + String(iPara));
			}

			else
			{
			}
			break;

		case 2 :	 
			break;

		case 3 :	 

			if(DIAG_MOT_OFF == iAct)
			{
				bbnMotor.offBldc();

				LogPrintln(" LG] IOout BBNMT OFF");
			}
			else if(DIAG_MOT_CW == iAct)
			{
				bbnMotor.onBldc(BBN_MOT_CW, iPara);
				diagBbnMotor_TimeoutStart(3000);

				LogPrintln(" LG] IOout BBNMT CW ON, Duty : " + String(iPara));
			}
			else if(DIAG_MOT_CCW == iAct)
			{
				bbnMotor.onBldc(BBN_MOT_CCW, iPara);
				diagBbnMotor_TimeoutStart(3000);

				LogPrintln(" LG] IOout BBNMT CCW ON, Duty : " + String(iPara));
			}
			else
			{
			}
			break;

		case 4 :	 
			if(DIAG_MOT_OFF == iAct)
			{
				digitalWrite(BBN_BLDC_24VON_PIN, BBN_BLDC_24VOFF);	 

				LogPrintln(" LG] IOout BBN24 OFF");
			}
			else			 
			{
				digitalWrite(BBN_BLDC_24VON_PIN, BBN_BLDC_24VON);	   

				LogPrintln(" LG] IOout BBN24 ON");
			}
			break;

		case 5 :	 

			if(DIAG_MOT_OFF == iAct)
			{

				diagBbnMotor_TokTokEnd();

				LogPrintln(" LG] IOout BBNTK OFF");
			}
			else if(DIAG_MOT_CW == iAct)
			{

				if ( (BBNACT_TBLMAX) <= iPara)	{ iPara = 0; }		 

				if ( (10000) <= iPara2) { iPara2 = 10000; }

				diagBbnMotor_TokTokStart(iPara, iPara2);			 

				LogPrintln(" LG] IOout BBNTK ON, AckKind : " + String(iPara) + ", opKind: " + String(iPara2) + ", Repeat CNT: " + String(iPara3)+ ", On/Time/Off/Time: " + String(bbnTokTok_OnDuty) + ", " + String(bbnTokTok_OnTime) + ", " + String(bbnTokTok_OffDuty) + ", " + String(bbnTokTok_OffTime));
			}

			else
			{
			}
			break;

		default:
			break;
	}

}

void diagOutputTest(String _msg)
{
		int iCatNo = 0;
		int iItemNo = 0;
		int iAct = 0;
		int iPara = 0;
		int iPara2 = 0;
		int iPara3 = 0;

		int dir = 0;
		int dutyPercent = 0;

		iCatNo = _msg.substring(3,(3+2)).toInt();	 

		iItemNo = _msg.substring(6,(6+2)).toInt();	 

		iAct = _msg.substring(9,(9+1)).toInt();		 

		iPara = _msg.substring(11,(11+3)).toInt();	 

		iPara2 = _msg.substring(15,(15+4)).toInt();	 

		iPara3 = _msg.substring(20,(20+5)).toInt();	 

		dutyPercent = iPara;

		if( 0 == iCatNo && 0 == iItemNo )
		{

			diagOutput_AllOff();

			LogPrintln(" LG] IOout All Output ALL OFF");
		}

		else if( 1 == iCatNo)		 
		{
			diagOut_Cat1(_msg);
		}

		else if( 2 == iCatNo)			 
		{
			switch(iItemNo)
			{
				case 0:	 

					extLed_AllOff();					

					LogPrintln(" LG] IOout Ext LED All OFF");
					break;
				case 1:
					if(DIAG_OFF == iAct)
					{
						diagOutFlag_LedCent_R = 0;			 
						extLed.off(LEDPOS_CENT_R);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_R OFF");
					}
					else if(DIAG_ON == iAct)
					{
						diagOutFlag_LedCent_R = 1;			 
						extLed.on(LEDPOS_CENT_R, iPara);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_R ON");
					}
					else
					{
					}
					break;
				case 2:
					if(DIAG_OFF == iAct)
					{
						diagOutFlag_LedCent_G = 0;			 
						extLed.off(LEDPOS_CENT_G);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_G OFF");
					}
					else if(DIAG_ON == iAct)
					{
						diagOutFlag_LedCent_G = 1;			 
						extLed.on(LEDPOS_CENT_G, iPara);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_G ON");
					}
					else
					{
					}
					break;
				case 3:
					if(DIAG_OFF == iAct)
					{
						diagOutFlag_LedCent_B = 0;			 
						extLed.off(LEDPOS_CENT_B);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_B OFF");
					}
					else if(DIAG_ON == iAct)
					{
						diagOutFlag_LedCent_B = 1;			 
						extLed.on(LEDPOS_CENT_B, iPara);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_B ON");
					}
					else
					{
					}
					break;
				case 4: 
					if(DIAG_OFF == iAct)
					{
						diagOutFlag_LedCent_RGB = 0; 			 
						extLed.off(LEDPOS_CENT_R);		 
						extLed.off(LEDPOS_CENT_G);		 
						extLed.off(LEDPOS_CENT_B);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_RGB OFF");
					}
					else if(DIAG_ON == iAct)
					{
						diagOutFlag_LedCent_RGB = 1;

						setRGBcolor(LEDPOS_CENT_RGB, iPara);				 
						extLed.RGBon(LEDPOS_CENT_RGB, sRGB[LEDPOS_CENT_RGB].r, sRGB[LEDPOS_CENT_RGB].g, sRGB[LEDPOS_CENT_RGB].b);

						LogPrintln(" LG] IOout diagOutFlag_LedCent_RGB ON");
					}
					else
					{
					}
					break;  

				case 5:
					break;

				case 6: 
					if(DIAG_OFF == iAct)
					{

						extLed.off(LEDPOS_BTM);		 

						LogPrintln(" LG] IOout diagOutFlag_LedCent_RGB ON");
					}
					else if(DIAG_ON == iAct)
					{

						extLed.on(LEDPOS_BTM, iPara);

						LogPrintln(" LG] IOout diagOutFlag_LedCent_RGB ON");
					}
					else
					{
					}
					break;  

				default: 
					break;
			}
		}

		else if( 3 == iCatNo )
		{
			diagOut_Cat3(_msg);
		}

		else if( 4 == iCatNo )
		{
		}

		else if( 5 == iCatNo )
		{
			switch(iItemNo)
			{
				case 0 : 	 

					LogPrintln(" LG] IOout Board_1 ALL OFF");
					break;

				case 1 : 	 
					if(DIAG_OFF == iAct)
					{

						subAcOff_WithOtherOff();
					}
					else if(DIAG_ON == iAct)
					{
						subAC_On();
					}

					else
					{

					}
					break;

				default:
					break;
			}
		}

		else
		{

		}
}

void diagOutAllOff()
{
	torqMotor.setValue(0);
	bbnMotor.offBldc();
}

#define POWER_ON_WAIT		(DEF_PWRON_DELAY_TIME*1000)	 
#define BOOTING_WAIT		(DEF_USB_SAFE_TIME*1000)	 
#define FORCE_OFF_2MIN		(2*60*1000)				 
#define RESP_WAIT_TIME		(500)					 
#define AUTO_REBOOT_TIME	(14*1000)				 
#define APP_CHECK_TIME		(18*1000)				 
#define ACOFF_500MS_AFTER_USB5VOFF	(500)			 
#define TO_EDGE_FILTER_CNT	5						 
#define	PWR_SW_ON		1
#define	PWR_SW_OFF		2

int	g_ProcStatus = 0;					 
static unsigned long	g_TmRTM = 0;
static unsigned long	g_TmNTM = 0;
static unsigned long	g_TmFTM = 0;
static unsigned long	g_TmBTM = 0;
static unsigned int	g_PSON_Count = 0;
static unsigned int	g_PSOF_Count = 0;
static unsigned int	g_USB5V_Count = 0;

void subAcOff_WithOtherOff()
{
	subAC_Off();		 

	ledOff_MbCent();
	ledOff_MbBtm();
	ledOff_OptLeft();
	ledOff_OptRight();
	g_RecvCmdCount = 0;

}

int CheckAppCmd()
{

	{
		if ((g_RecvCmdCount > 2) && !checkTO(g_LastCmdTick, 4000)) {	 

			return 1;													 
		}
	}
	return 0;
}

int CheckPowerSwitch()
{
	if (LVL10_PWROFF_SWC)						 
	{
		g_PSON_Count=0;							 
		g_PSOF_Count++;							 
		if (g_PSOF_Count > TO_EDGE_FILTER_CNT)	 
			return PWR_SW_OFF;					 
	}
	else
	{
		g_PSON_Count++;							 
		g_PSOF_Count=0;							 
		if (g_PSON_Count > TO_EDGE_FILTER_CNT)	 
			return PWR_SW_ON;					 
	}
	return 0;
}

static String StsStr[] = {  "PS_INITIAL", "PS_ON_WAIT", "PS_BOOTING", "PS_RUNNING",
					 "PS_RSPWAIT", "PS_USBWAIT", "PS_OFFWAIT" };
#define	SnsStr(x)	("ACOFF("+String((x >> SNS_AcOff) & 1) + ") USB5V("+String((x >> SNS_usb5V) & 1) + ") PWOFF("+String((x >> SNS_PwrOffSwc) & 1)+")")

void check_PowerSwitch_Change_10ms()
{
	static int	SaveStatus = -1;
	static	int	oldSns = 0;
	int newSns = LVL10_PWRALL;
	int chgSns = oldSns ^ newSns;
	if (chgSns) {
		LogPrintln(" LG] CHKSNS : " + SnsStr(newSns));
		oldSns = newSns;
	}
	switch (g_ProcStatus)
	{
	case PS_INITIAL :		 
		if (CheckPowerSwitch() == PWR_SW_ON) {		 
			initializeLedControlValues(1);					 
			ledOn_MbBtm();							 
			pwrMode = PWR_ON;						 
			g_ProcStatus = PS_ON_WAIT;				 
		}
		else {
			if ((g_SerialEnable == 0) && 			 
				checkTO(g_TmBTM, BOOTING_WAIT)) {	 
				SerialPortEnable();					 
			}
		}
		break;
	case PS_ON_WAIT :		 
		if (CheckPowerSwitch() == PWR_SW_OFF) {		 

			extLed_AllOff();
			extLed_controlOff();
			pwrMode = PWR_OFF;						 
			g_ProcStatus = PS_INITIAL;				 
		}
		else if (checkTO(g_TmNTM, POWER_ON_WAIT)) {	 
			if (g_SerialEnable) {
				SerialPortDisable();				 
			}
			subAC_On();								 
			setTO(g_TmBTM);							 
			g_ProcStatus = PS_BOOTING;				 
		}
		break;
	case PS_BOOTING :		 
		if (g_SerialEnable || checkTO(g_TmBTM, BOOTING_WAIT)) {		 
			if (g_SerialEnable == 0) {
				SerialPortEnable();					 
			}
			g_ProcStatus = PS_RUNNING;				 
		}
		break;
	case PS_RUNNING :		 
		if ((CheckPowerSwitch() == PWR_SW_OFF) ||	 
			(g_AutoReboot && CheckAppCmd() && checkTO(g_TmFTM, AUTO_REBOOT_TIME))) {	 
			recvPwrOffExec_Resp = 0;				 
			setTO(g_TmFTM);							 
			setTO(g_TmRTM);							 
			g_ProcStatus = PS_RSPWAIT;				 
		}
		break;
	case PS_RSPWAIT :		 
		if(recvPwrOffExec_Resp)						 
		{
			recvPwrOffExec_Resp = 0;				 
			g_ProcStatus = PS_USBWAIT;				 
		}
		else if (checkTO(g_TmFTM, FORCE_OFF_2MIN))	 
		{
			subAcOff_WithOtherOff();				 
			setTO(g_TmNTM);							 
			g_ProcStatus = PS_INITIAL;				 
		}
		else if (checkTO(g_TmRTM, RESP_WAIT_TIME))	 
		{
			if ((CheckPowerSwitch() == PWR_SW_ON)	 
				&& (CheckAppCmd() == 0)) {			 
				g_ProcStatus = PS_RUNNING;			 
			}
			else {
				setTO(g_TmRTM);						 
			}
		}
		break;
	case PS_USBWAIT :		 
		if(LOW == LVL10_USB5V)						 
		{
			setTO(g_TmRTM);							 
			g_ProcStatus = PS_OFFWAIT;				 
		}
		else if (checkTO(g_TmFTM, FORCE_OFF_2MIN))	 
		{
			subAcOff_WithOtherOff();				 
			setTO(g_TmNTM);							 
			g_ProcStatus = PS_INITIAL;				 
		}
		break;
	case PS_OFFWAIT :		 
		if (checkTO(g_TmRTM, RESP_WAIT_TIME))		 
		{
			subAcOff_WithOtherOff();				 
			setTO(g_TmNTM);							 
			g_ProcStatus = PS_INITIAL;				 
		}
		break;
	default :				 
		LogPrintln(" LG] CHKPWR : PS_UNKNOWN("+ String(g_ProcStatus) + ") -> PS_INITIAL");
		g_ProcStatus = PS_INITIAL;					 
		break;
	}
	if (SaveStatus != g_ProcStatus) {
		LogPrintln(" LG] CHKPWR : " + ((SaveStatus==-1) ? "PS_UNKNOWN" : StsStr[SaveStatus]) + " -> " + StsStr[g_ProcStatus]);
		SaveStatus = g_ProcStatus;
	}
}

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
