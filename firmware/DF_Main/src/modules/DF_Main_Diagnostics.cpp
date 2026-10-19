 
#include "../DF_Main_Internal.h"
#include "DF_Main_Diagnostics.h"
void diagInput_AllOff()
{
	diagInput_BoardInAllOff();
	DiagInput_MotInAllOff();
	DiagInput_MechaInAllOff();
	DiagInput_ElecInAllOff();
	
}

 

#define DIAG_CHAGE_VAL	3		 

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
	oldsensorBootSwc = sensorBootSwc + DIAG_CHAGE_VAL;
	 
	 
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
	oldsensorBbnFG = sensorBbnFG + DIAG_CHAGE_VAL;
}

void DiagInput_MainEcoderInAllOn()
{
	diagInFlag_BobbinEncA = 1;
	oldsensorBbnEncA = sensorBbnEncA + DIAG_CHAGE_VAL;
	diagInFlag_BobbinEncB = 1;
	oldsensorBbnEncB = sensorBbnEncB + DIAG_CHAGE_VAL;

	if(AP_IS_TM == apType)
	{
		diagInFlag_BobbinEncDir = 1;
		oldsensorBbnEncCnt= sensorBbnEncCnt + DIAG_CHAGE_VAL;
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
	oldsensorPonSwc = sensorPonSwc + DIAG_CHAGE_VAL;
	diagInFlag_PcUsb5V = 1;
	oldsensorUsb5V = sensorUsb5V + DIAG_CHAGE_VAL;
	 
	 
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

 
void diagIput_Cat1(String _msg)
{
	int iItemNo = _msg.substring(6,(6+2)).toInt();
		    
	int iAct = _msg.substring(9,(9+1)).toInt();
		    
	int iPara = _msg.substring(11,(11+3)).toInt();	 

		   switch(iItemNo)
		   {
			   case 0:
				   if(DAIG_OFF == iAct)
				   {
					   diagInput_BoardInAllOff();
				   }
				   else if(DAIG_ON == iAct)
				   {
					   diagInput_BoardInAllOn();
				   }
				   
				   LogPrintln(" LG] IO_in Board Input All OFF");
				   break;
			   case 1:
				   if(DAIG_OFF == iAct)
				   {
					   diagInFlag_ResetSwc = 0; 		    
					   
					   LogPrintln(" LG] IO_in Reset SWC mon ON");
				   }
				   else if(DAIG_ON == iAct)
				   {
					   diagInFlag_ResetSwc = 1; 		    
					   oldsensorResetSwc = sensorResetSwc + DIAG_CHAGE_VAL;
					   
					   LogPrintln(" LG] IO_in Reset SWC mon ON");
				   }
				   else
				   {
					   
					   LogPrintln(" LG] IO_in Can't Find Act No");
				   }
				   break;
			   case 2:
				   if(DAIG_OFF == iAct)
				   {
					   diagInFlag_BootSwc = 0;			    
					   
					   LogPrintln(" LG] IO_in Boot SWC mon OFF");
				   }
				   else if(DAIG_ON == iAct)
				   {
					   diagInFlag_BootSwc = 1;			    
					   oldsensorBootSwc = sensorBootSwc + DIAG_CHAGE_VAL;
					   
					   LogPrintln(" LG] IO_in Boot SWC mon ON");
				   }
				   else
				   {
				   }
				   break;
			   case 3:
				   if(DAIG_OFF == iAct)
				   {
					   diagInFlag_WdtLbSig = 0; 		    
				   }
				   else if(DAIG_ON == iAct)
				   {
					   diagInFlag_WdtLbSig = 1; 		    
					   oldsensorWdtFbSig = sensorWdtFbSig + DIAG_CHAGE_VAL;
					   
					   LogPrintln(" LG] IO_in WDT LB Signal mon ON");
				   }
				   else
				   {
				   }
				   break;
			   case 4: 
				   if(DAIG_OFF == iAct)
				   {
					   diagInFlag_SelectBoard = 0;			    
				   }
				   else if(DAIG_ON == iAct)
				   {
					   diagInFlag_SelectBoard = 1; 
					   oldsensorSelBoard = sensorSelBoard + DIAG_CHAGE_VAL;
					   
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
			if(DAIG_OFF == iAct)
			{
				DiagInput_MotInAllOff();
			}
			else if(DAIG_ON == iAct)
			{
				DiagInput_MotInAllOn();
				DiagInput_MainEcoderInAllOn();
			}
			break;

		case 1: 
			if(DAIG_OFF == iAct)
			{
				 diagInFlag_BobbinEncA = 0; 		 
				 
				 LogPrintln(" LG] IO_in Main Enc_A mon OFF");
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_BobbinEncA = 1; 
				oldsensorBbnEncA = sensorBbnEncA + DIAG_CHAGE_VAL;
				
				LogPrintln(" LG] IO_in Main Enc_A mon ON");
			}
			break;  

		case 2: 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_BobbinEncB = 0;			 
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_BobbinEncB = 1; 
				oldsensorBbnEncB= sensorBbnEncB+ DIAG_CHAGE_VAL;
				
				LogPrintln(" LG] IO_in Main Enc_B mon ON");
			}
			break;  

		case 3: 					 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_BobbinEncDir = 0;			 
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_BobbinEncDir = 1; 
				oldsensorBbnEncCnt= sensorBbnEncCnt + DIAG_CHAGE_VAL;
				
				LogPrintln(" LG] IO_in Main Enc Dir/Cnt ON");
			}
			break;  

		case 4: 					 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_AngleHomeSns = 0;			 
				LogPrintln(" LG] IO_in Angle HomeSensor mon OFF");
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_AngleHomeSns = 1; 
				oldsensorAngHomeSns = sensorAngHomeSns + DIAG_CHAGE_VAL;
				
				LogPrintln(" LG] IO_in Angle HomeSensor mon ON");
			}
			break;  

		case 5: 					 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_BbnBldcFG = 0;			 
				LogPrintln(" LG] IO_in BBN BLDC FG mon OFF");
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_BbnBldcFG = 1; 
				oldsensorBbnFG = sensorBbnFG + DIAG_CHAGE_VAL;
				
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
			if(DAIG_OFF == iAct)
			{
				DiagInput_ElecInAllOff();
			}
			else if(DAIG_ON == iAct)
			{
				DiagInput_ElecInAllOn();
			}
			break;
		case 1: 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_PonSwc = 0;			 
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_PonSwc = 1; 
				oldsensorPonSwc = sensorPonSwc + DIAG_CHAGE_VAL;
				
				LogPrintln(" LG] IO_in Power On Switch mon ON");
			}
			break;  
		case 2: 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_PcUsb5V = 0; 			 
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_PcUsb5V = 1; 
				oldsensorUsb5V = sensorUsb5V + DIAG_CHAGE_VAL;
				
				LogPrintln(" LG] IO_in Usb 5V mon ON");
			}
			break;  
		case 3: 
			if(DAIG_OFF == iAct)
			{
				diagInFlag_Interlock = 0;			 
			}
			else if(DAIG_ON == iAct)
			{
				diagInFlag_Interlock = 1;
				oldsensorDoorIlk = sensorDoorIlk + DIAG_CHAGE_VAL;
				
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
	   		diagIput_Cat1(_msg);

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

 
 
 
void daigBbnMotor_TokTokEnd()
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
			if(DAIG_OFF == iAct)
			{
				digitalWrite(BD_LED2_PIN, LED_OFF);
				
				LogPrintln(" LG] IOout BD LED2 OFF");
			}
			else if(DAIG_ON == iAct)
			{
				digitalWrite(BD_LED2_PIN, LED_ON);
				
				LogPrintln(" LG] IO_in	BD LED2 ON");
			}
			else
			{
			}
			break;
	
		case 3:
			if(DAIG_OFF == iAct)
			{
				digitalWrite(BD_LED3_PIN, LED_OFF);
				
				LogPrintln(" LG] IOout BD LED3 OFF");
			}
			else if(DAIG_ON == iAct)
			{
				digitalWrite(BD_LED3_PIN, LED_ON);
				
				LogPrintln(" LG] IOout BD LED3 ON");
			}
			else
			{
			}
			break;
		case 4: 
			if(DAIG_OFF == iAct)
			{
				digitalWrite(BD_LED4_PIN, LED_OFF);
				
				LogPrintln(" LG] IOout BD LED4 OFF");
			}
			else if(DAIG_ON == iAct)
			{
				digitalWrite(BD_LED4_PIN, LED_ON);
				
				LogPrintln(" LG] IOout BD LED4 ON");
			}
			else
			{
			}
			break;  
		case 5 :	 
			if(DAIG_OFF == iAct)
			{
				debugPort_Off();
				LogPrintln(" LG] IOout BD DebugPort OFF");
			}
			else if(DAIG_ON == iAct)
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
			if(DAIG_MOT_OFF == iAct)
			{
				torqMotor.setValue(0);
				
				LogPrintln(" LG] IOout Main Mot(Torq) OFF");
			}
			else if(DAIG_MOT_CW == iAct)
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
			 
			 










			
			if(DAIG_MOT_OFF == iAct)
			{
				bbnMotor.offBldc();
				
				LogPrintln(" LG] IOout BBNMT OFF");
			}
			else if(DAIG_MOT_CW == iAct)
			{
				bbnMotor.onBldc(BBN_MOT_CW, iPara);
				diagBbnMotor_TimeoutStart(3000);
				
				LogPrintln(" LG] IOout BBNMT CW ON, Duty : " + String(iPara));
			}
			else if(DAIG_MOT_CCW == iAct)
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
			if(DAIG_MOT_OFF == iAct)
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
			 
			 










			
			if(DAIG_MOT_OFF == iAct)
			{
				 
				daigBbnMotor_TokTokEnd();
				
				LogPrintln(" LG] IOout BBNTK OFF");
			}
			else if(DAIG_MOT_CW == iAct)
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
					if(DAIG_OFF == iAct)
					{
						diagOutFlag_LedCent_R = 0;			 
						extLed.off(LEDPOS_CENT_R);		 
						
						LogPrintln(" LG] IOout diagOutFlag_LedCent_R OFF");
					}
					else if(DAIG_ON == iAct)
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
					if(DAIG_OFF == iAct)
					{
						diagOutFlag_LedCent_G = 0;			 
						extLed.off(LEDPOS_CENT_G);		 
						
						LogPrintln(" LG] IOout diagOutFlag_LedCent_G OFF");
					}
					else if(DAIG_ON == iAct)
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
					if(DAIG_OFF == iAct)
					{
						diagOutFlag_LedCent_B = 0;			 
						extLed.off(LEDPOS_CENT_B);		 
						
						LogPrintln(" LG] IOout diagOutFlag_LedCent_B OFF");
					}
					else if(DAIG_ON == iAct)
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
					if(DAIG_OFF == iAct)
					{
						diagOutFlag_LedCent_RGB = 0; 			 
						extLed.off(LEDPOS_CENT_R);		 
						extLed.off(LEDPOS_CENT_G);		 
						extLed.off(LEDPOS_CENT_B);		 
						
						LogPrintln(" LG] IOout diagOutFlag_LedCent_RGB OFF");
					}
					else if(DAIG_ON == iAct)
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
					if(DAIG_OFF == iAct)
					{
						 
						extLed.off(LEDPOS_BTM);		 
						
						LogPrintln(" LG] IOout diagOutFlag_LedCent_RGB ON");
					}
					else if(DAIG_ON == iAct)
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
					if(DAIG_OFF == iAct)
					{
						 
						subAcOff_WithOtherOff();
					}
					else if(DAIG_ON == iAct)
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

 
void findDiagCode(String msg, stDiagCode code)
{
	code.catNo = msg.substring(3,5).toInt();	 
												 
	code.itemNo = msg.substring(6,8).toInt(); 	 
	
	code.act = msg.substring(8,9).toInt();		 
	
	code.para = msg.substring(9).toInt();		 
}

 
int controlDiagInput(String msg)
{
	stDiagCode diag_code;		 
	findDiagCode(msg, diag_code);

}

 
int controlDiagOutput(String msg)
{
	stDiagCode diag_code;		 
	findDiagCode(msg, diag_code);
	
	 

}



void diagOutAllOff()
{
	torqMotor.setValue(0);
	bbnMotor.offBldc();
}



 





 
void Set_WaveData( String msg)
{
	int val = 0;
	char ch = msg.charAt(3);
	
	if( '0' == ch ) 		 
	{
		 
	}
	else if( '1' == ch ) 		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( 7 < val ) val = 7;
		waveControlCase = val;
		 
		 
	}
	else if( '2' == ch )		 
	{
		val = msg.substring(4,7).toInt();	 
		if( -90 > val ) val = -90;
		if( 90 < val ) val = 90;
		stWaveServoVal.angle1 = val;
	
		val = msg.substring(7,10).toInt();	 
		if( -90 > val ) val = -90;
		if( 90 < val ) val = 90;
		stWaveServoVal.angle2 = val;
	
		val = msg.substring(10).toInt();	 
		if( 500 > val ) val = 500;
		if( 5000 < val ) val = 5000;
		stWaveServoVal.interval = val;
	}
	else if( '3' == ch )		 
	{
		val = msg.substring(4,7).toInt();	 
		if( 20 > val ) val = 20;
		if( 80 < val ) val = 80;
		stWaveTorqVal.rtTorq = val;
	
		val = msg.substring(7,10).toInt();	 
		if( 20 > val ) val = 20;
		if( 99 < val ) val = 99;
		stWaveTorqVal.tgTorq = val;
	
		val = msg.substring(10,(10+4)).toInt();	 
		if( 500 > val ) val = 500;
		if( 5000 < val ) val = 5000;
		stWaveTorqVal.onInterval = val;

		val = msg.substring(14,(14+4)).toInt();	 
		if( 500 > val ) val = 500;
		if( 5000 < val ) val = 5000;
		stWaveTorqVal.offInterval = val;
	}
	
	else if( '5' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( (WAVE_PWR_MAX-1) < val ) val = (WAVE_PWR_MAX-1);
		waveType = val;
		waveType_TorqSet();
	}
	else if( '6' == ch )		 
	{
		val = msg.substring(4,(4+3)).toInt();	 
		if( 5 > val ) val = 5;
		if( 200 < val ) val = 200;
		stWaveTorqVal.pwr[1] = val;
	
		val = msg.substring(7,(7+3)).toInt();	 
		if( 5 > val ) val = 5;
		if( 200 < val ) val = 200;
		stWaveTorqVal.pwr[2]  = val;
	
		val = msg.substring(10,(10+3)).toInt();	 
		if( 5 > val ) val = 5;
		if( 200 < val ) val = 200;
		stWaveTorqVal.pwr[3]  = val;

		val = msg.substring(13,(13+3)).toInt();	 
		if( 5 > val ) val = 5;
		if( 200 < val ) val = 200;
		stWaveTorqVal.pwr[4]  = val;

		val = msg.substring(16,(16+3)).toInt();	 
		if( 5 > val ) val = 5;
		if( 200 < val ) val = 200;
		stWaveTorqVal.pwr[5]  = val;
		
	}
	else if( '7' == ch )		 
	{
		val = msg.substring(4,7).toInt();	 
		if( 5 > val ) val = 5;
		if( 80 < val ) val = 80;
		stWaveBdutyVal.rtTorq = val;
	
		val = msg.substring(7,10).toInt();	 
		if( 5 > val ) val = 5;
		if( 80 < val ) val = 80;
		stWaveBdutyVal.tgTorq = val;
	
		val = msg.substring(10,(10+4)).toInt();	 
		if( 500 > val ) val = 500;
		if( 5000 < val ) val = 5000;
		stWaveBdutyVal.onInterval = val;

		val = msg.substring(14,(14+4)).toInt();	 
		if( 500 > val ) val = 500;
		if( 5000 < val ) val = 5000;
		stWaveBdutyVal.offInterval = val;
	}

	
	
	else if( '9' == ch )		 
	{
		val = msg.substring(4,(4+1)).toInt();	 
		if( 0 == val)			 
		{
			exeWave = 0;
			wave_bbn_step = WAVE_BBN_TORQ_START;
			torqMotor.setValue(DEFAULT_TORQ);
			bbnMotor.offBldc();
		}
		else				 
		{
			waveType_TorqSet();
			wave_bbn_step = WAVE_BBN_TORQ_START;
			exeWave = 1;
		}
	}
	
	if( ('0'==ch) || ('1'==ch)	|| ('2'==ch) || ('3'==ch) || ('4'==ch) || ('5'==ch) || ('6'==ch) || ('7'==ch) || ('9'==ch) )	 
	{
		
		LogPrintln(" LG] Wave_ContCase: " + String(waveControlCase));
		
		LogPrintln(" LG] Angle1: " + String(stWaveServoVal.angle1) + ", Angle2: " + String(stWaveServoVal.angle2) + ", sInterval: " + String(stWaveServoVal.interval));
		
		LogPrintln(" LG] Torq1: " + String(stWaveTorqVal.rtTorq) + ", Torq2: " + String(stWaveTorqVal.tgTorq) + ", onTime: " + String(stWaveTorqVal.onInterval)+ ", offTime: " + String(stWaveTorqVal.offInterval));
		
		LogPrintln(" LG] Wave_Type: " + String(waveType) + ", Type1: "+ String(stWaveTorqVal.pwr[waveType]) );
	}
	else
	{
		
		LogPrintln(" LG] UnCorrect Format!!");
	}

}

 
 
 
void Set_BiteData( String msg)
{
	int val = 0;
	char ch = msg.charAt(3);
	
	if( '0' == ch ) 		 
	{
		biteLogOut = 1;
	}
	else if( '1' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( 3 < val ) val = 3;
		biteTestCase = val;
	}
	else if( '2' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( 3 < val ) val = 3;
		biteTorqCase = val;
		biteLogOut = 1;
	}
	else if( '3' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 1 > val ) val = 1;
		if( 3 < val ) val = 3;
		biteCnt = val;
	}
	else if( '4' == ch )		 
	{
		val = msg.substring(4,7).toInt();	 
		if( 60 > val ) val = 60;
		if( 100 < val ) val = 100;
		stBiteTorqVal.shallowTorq = val;
	
		val = msg.substring(7,11).toInt();	 
		if( 300 > val ) val = 300;
		if( 5000 < val ) val = 5000;
		stBiteTorqVal.shallowInterval = val;
	
		val = msg.substring(11,14).toInt();  
		if( 80 > val ) val = 80;
		if( 150 < val ) val = 150;
		stBiteTorqVal.deepTorq = val;
	
		val = msg.substring(14).toInt();	 
		if( 500 > val ) val = 500;
		if( 5000 < val ) val = 5000;
		stBiteTorqVal.deepInterval = val;
	}
	
	else if( '5' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( 5 < val ) val = 5;
		biteType = val;
	}
	else if( '6' == ch )		 
	{
		val = msg.substring(4,7).toInt();	 
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[1] = val;
	
		val = msg.substring(7,10).toInt();	 
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[2] = val;
	
		val = msg.substring(10,13).toInt();  
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[3] = val;
	
		val = msg.substring(13,16).toInt();  
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[4] = val;
	
		val = msg.substring(16,19).toInt();  
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[5] = val;
	
		val = msg.substring(19,22).toInt();  
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[6] = val;
	
		val = msg.substring(22).toInt();  
		if( 60 > val ) val = 60;
		if( 150 < val ) val = 150;
		stBiteTorqVal.torqType[7] = val;
		
	}
	
	else if( 'A' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( 50 < val ) val = 50;
		

		LogPrintln(" LG] Servo - DEC Angle: " + String(val));
	}
	
	if( ('0'==ch) || ('1'==ch)	|| ('2'==ch) || ('3'==ch) || ('4'==ch) || ('5'==ch) || ('6'==ch) )	 
	{
		
		LogPrintln(" LG] BiteConbi: " + String(biteTestCase) + ", BiteTorqControl: " + String(biteTorqCase) + ", BiteCount: " + String(biteCnt));
		 
		 
		
		LogPrintln(" LG] shallow_Torq: " + String(stBiteTorqVal.shallowTorq) + ", sh_Intval: " + String(stBiteTorqVal.shallowInterval) +", deep_Torq: " + String(stBiteTorqVal.deepTorq) + ", deep_Intval: " + String(stBiteTorqVal.deepInterval));
		
		LogPrintln(" LG] Bite_TorqType: " + String(biteType) + ", Type1: "+ String(stBiteTorqVal.torqType[1]) + ", Type2: "+ String(stBiteTorqVal.torqType[2])  + ", Type3: "+ String(stBiteTorqVal.torqType[3]) + ", Type4: "+ String(stBiteTorqVal.torqType[4])	+ ", Type5: "+ String(stBiteTorqVal.torqType[5])  + ", Type6: "+ String(stBiteTorqVal.torqType[6])	+ ", Type7: "+ String(stBiteTorqVal.torqType[7]) );
	}
	else
	{
		
		LogPrintln(" LG] UnCorrect Format!!");
	}

}


 
void Set_HitData(String msg)
{

			int val = 0;
			char ch = msg.charAt(3);
		
			int i = 0;
		
			if( '0' == ch ) 		 
			{
				 
			}
			else if( '2' == ch )		 
			{
				val = msg.substring(4).toInt();  
				if( 0 > val ) val = 0;
				if( 90 < val ) val = 90;
				hitServoAngle = val;
			}
			 
			else if( '3' == ch )		 
			{
				val = msg.substring(4,7).toInt();	 
				if( 60 > val ) val = 60;
				if( 250 < val ) val = 250;
				stHitTorqVal.targetTorq = val;
				stHitTorqVal.holdonTorq = val;
		
				val = msg.substring(7,11).toInt();	 
				if( 100 > val ) val = 100;
				if( 9000 < val ) val = 9000;
				stHitTorqVal.targetInterval = val;
				stHitTorqVal.holdonInterval = val;
		
			}
			else if( '4' == ch )		 
			{
				val = msg.substring(4,7).toInt();	 
				if( 60 > val ) val = 60;
				if( 250 < val ) val = 250;
				stHitTorqVal.lowTorq = val;
		
				val = msg.substring(7,11).toInt();	 
				if( 100 > val ) val = 100;
				if( 9000 < val ) val = 9000;
				stHitTorqVal.lowInterval = val;
		
				val = msg.substring(11,14).toInt();  
				if( 60 > val ) val = 60;
				if( 250 < val ) val = 250;
				stHitTorqVal.midTorq = val;
		
				val = msg.substring(14).toInt();	 
				if( 100 > val ) val = 100;
				if( 9000 < val ) val = 9000;
				stHitTorqVal.midInterval = val;
			}
			
		  
			else if( '6' == ch )		 
			{
				for(i = 0; i < 12; i++)
				{
					val = msg.substring((3*i)+4,(3*i)+7).toInt();	 
					if( 60 > val ) val = 60;
					if( 250 < val ) val = 250;
					stHitTorqVal.torqType[i+1] = val;
				}
				for(i = 0; i < 12; i++)
				{
					
					LogPrintln(" LG] Hit Tg_Torq " + String(i+1) + " : " + String(stHitTorqVal.torqType[i+1]) );
				}
			}
		
			else if( 'S' == ch )		 
			{
				val = msg.substring(4,6).toInt();  
				if( 0 != val ) val = 1; 	 
				 
				exeResistServoENB = val;
				val = msg.substring(6).toInt();  
				if( 0 != val ) val = 1; 	 
				exeHoldonServoENB = val;
				
				LogPrintln(" LG] Servo - Resist ServoENB: " + String(exeResistServoENB) + ", HoldOn ServoENB: " + String(exeHoldonServoENB) );
			}
			
			else if( 'K' == ch )		 
			{
				val = msg.substring(4,6).toInt();  
				if( 0 != val ) val = 1; 	 
				 
				fishTypeFixENB = val;
				if(fishTypeFixENB)		 
				{
					val = msg.substring(6).toInt();  
					if( 1 > val ) val = 1;
					if( 15 < val ) val = 15;
					fishTypeFix = val;
				}
				
				LogPrintln(" LG] Fish Auto(0)Fix(1): " + String(fishTypeFixENB) + ", FishType(Fix): " + String(fishTypeFix) + ", FishType(Auto): " + String(fishTypeAuto) );
			}
		
			if( ('0'==ch) || ('1'==ch)	|| ('2'==ch) || ('3'==ch) || ('4'==ch) || ('5'==ch) || ('6'==ch) )	 
			{
				
				LogPrintln(" LG] HitAngle: " + String(hitServoAngle) );
				
				
				LogPrintln(" LG] hit_tg_Torq: " + String(stHitTorqVal.targetTorq) + ", hit_tg_Intval: " + String(stHitTorqVal.targetInterval) );
				
				LogPrintln(" LG] hit_low_Torq: " + String(stHitTorqVal.lowTorq) + ", hit_low_Intval: " + String(stHitTorqVal.lowInterval) +  ", hit_mid_Torq: " + String(stHitTorqVal.midTorq) + ", hit_mid_Intval: " + String(stHitTorqVal.midInterval) );
			}
			else
			{
				
				LogPrintln(" LG] UnCorrect Format!!");
			}
}


 
void Set_BreakData( String msg)
{
	int val = 0;
	char ch = msg.charAt(3);
	
	if( '0' == ch ) 		 
	{
		
		LogPrintln(" LG] WaveType(1~3): " + String(waveType));
		
		LogPrintln(" LG] Fish Auto(0)Fix(1): " + String(fishTypeFixENB) + ", FishType(Fix): " + String(fishTypeFix) + ", FishType(Auto): " + String(fishTypeAuto) );
		
		LogPrintln(" LG] Hit Servo - Resist ENB: " + String(exeResistServoENB) + ", HoldOn ENB: " + String(exeHoldonServoENB) + ", Hit_Angle: " + String(hitServoAngle));
		
		LogPrintln(" LG] Hit_Resist_FIX: " + String(torqResistFix) + ", Torq Target: " + String(stHitTorqVal.targetTorq) + ", Resist: " + String(stHitTorqVal.holdonTorq) + ", low: " + String(stHitTorqVal.lowTorq) + ", Mid: " + String(stHitTorqVal.midTorq) );
		
		LogPrintln(" LG] Hit_Break, Break_Type: " + String(breakType) );
	}
	else if( '1' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 1 > val ) val = 1;
		if( 3 < val ) val = 3;
		breakType = val;

		
		LogPrintln(" LG] Hit_Break, Break_Type: " + String(breakType) );
	}

 









}

 
 
 
 
 
 
void Set_ExtledData(String msg)
{
			int val = 0;
			int para1 = 0;
			char ch = msg.charAt(3);
	
			int i = 0;
		
			if( '0' == ch ) 		 
			{
				 
			}
		 
		 
			else if( '1' == ch )		 
			{
				val = msg.substring(4).toInt();  
				if( 0 > val ) val = 0;		
				 ledControlMode = val;	 
			}
			else if( '2' == ch )		 
			{
				val = msg.substring(4).toInt();  
				if( 1 > val ) val = 1;
				if( 60 < val ) val = 60;
				dimming_time = val;			 
			}
			else if( '3' == ch )		 
			{
				val = msg.substring(4).toInt();  
				if( 100 > val ) val = 100;
				if( 5000 < val ) val = 5000;
				ledoffHoldTimeCnt = (unsigned int)(val/10);
			}
		 





































		
			if( ('0'==ch) || ('1'==ch)	|| ('2'==ch) || ('3'==ch) || ('4'==ch) || ('5'==ch) || ('6'==ch) || ('7'==ch) )  
			{
				
				LogPrintln(" LG] EXTLED_SET ENB : " + String(ledControlMode) + " [sec]");
				LogPrintln(" LG] EXTLED_SET dimm : " + String(dimming_time) + " [sec]");
				LogPrintln(" LG] EXTLED_SET hold : " + String(ledoffHoldTimeCnt*10) + " [ms]");
			 














			}
			else
			{
				
				LogPrintln(" LG] UnCorrect Format!!");
			}
	
}

 
void Set_BbnTokTokData( String msg)
{
		int val = 0;
		int para1 = 0;
		char ch = msg.charAt(3);

		int i = 0;
	
		if( '0' == ch ) 		 
		{
			 
		}
	 
		else if( '5' == ch )		 
		{
			val = msg.substring(4).toInt();  
			if( 0 > val ) val = 0;
			if( BBNACT_TBLMAX <= val ) val = 0;		 
			bbnActKind = (unsigned long)val;
		}

		else if( '6' == ch )		 
		{
			para1 = msg.substring(4,(4+1)).toInt();  
			if((BBNACT_TBLMAX-1) < para1) { para1 = (BBNACT_TBLMAX-1); }
			val = msg.substring(5).toInt();  
			if( BBN_RPT_CNT_MIN > val ) { val = BBN_RPT_CNT_MIN; }
			if( BBN_RPT_CNT_MAX < val ) { val = BBN_RPT_CNT_MAX; }
			bbnReqCnt[para1] = (unsigned long)val;
		}
	 
		else if( '1' == ch )		 
		{
			val = msg.substring(4).toInt();  
			if( BBN_PWM_DUTY_MIN > val ) val = BBN_PWM_DUTY_MIN;
			if( BBN_PWM_DUTY_MAX < val ) val = BBN_PWM_DUTY_MAX;
			sBbnActTbl[bbnActKind].onDuty = val;
		}
		else if( '2' == ch )		 
		{
			val = msg.substring(4).toInt();  
			if( BBN_WAIT_TIME_MIN > val ) val = BBN_WAIT_TIME_MIN;
			if( BBN_WAIT_TIME_MAX < val ) val = BBN_WAIT_TIME_MAX;
			sBbnActTbl[bbnActKind].onTime = (unsigned long)val;
		}
		else if( '3' == ch )		 
		{
			val = msg.substring(4).toInt();  
			if( BBN_PWM_DUTY_MIN > val ) val = BBN_PWM_DUTY_MIN;
			if( BBN_PWM_DUTY_MAX < val ) val = BBN_PWM_DUTY_MAX;
			sBbnActTbl[bbnActKind].offDuty = val;
		}
		else if( '4' == ch )		 
		{
			val = msg.substring(4).toInt();  
			if( BBN_WAIT_TIME_MIN > val ) val = BBN_WAIT_TIME_MIN;
			if( BBN_WAIT_TIME_MAX < val ) val = BBN_WAIT_TIME_MAX;
			sBbnActTbl[bbnActKind].offTime = (unsigned long)val;
		}

		else if( '7' == ch )		 
		{
			val = msg.substring(4).toInt();  
			if(1 > val ) val = 1;
			if( 6 < val ) val = 6;
			torqMotor.torqWattAdj = val;
		}
		else
		{
			 
		}
	
		if( ('0'==ch) || ('1'==ch)	|| ('2'==ch) || ('3'==ch) || ('4'==ch) || ('5'==ch) || ('6'==ch) || ('7'==ch) )	 
		{
			LogPrintln(" LG] TORQ MOT WATT : " + String(torqMotor.torqWattAdj) + " [ 1:10W,2:20W,3:30W]");
			LogPrintln(" LG] BBN TokTok ACT KIND: " + String(bbnActKind) + " [ 0~6 ]");
			LogPrintln(" LG] BBN TokTok Rpt REQ CNT: " + String(bbnReqCnt[bbnActKind]) + " [ 1 ~ 100 ] Times");
			LogPrintln(" LG] BLDC TokTok On Duty: " + String(sBbnActTbl[bbnActKind].onDuty) + " / 255 Max");
			LogPrintln(" LG] BLDC TokTok On Time: " + String(sBbnActTbl[bbnActKind].onTime) + " ms");
			LogPrintln(" LG] BLDC TokTok Off Duty: " + String(sBbnActTbl[bbnActKind].offDuty) + "  / 255 Max");
			LogPrintln(" LG] BLDC TokTok Off Time: " + String(sBbnActTbl[bbnActKind].offTime) + " ms");
		}
		else
		{
			LogPrintln(" LG] UnCorrect Format!!");
		}
}

 


void Set_TorqAdjValData( String msg)
{
		int val = 0;
		char ch = msg.charAt(3);
		
		if( '0' == ch ) 		 
		{
			
			LogPrintln(" LG] Type: " + String(ch) + ",	");
			
			LogPrintln(" LG] *Torq Adj Curr Val: " + String(torqMotor.setAdjTorq));
		}
		else if( '1' == ch )		 
		{
			val = msg.substring(4,6).toInt();  
			if( 30 > val ) val = 30;
			if( 80 < val ) val = 80;
			torqMotor.defaultVal = val;
			
			LogPrintln(" LG] Set Torq Default Val, *ADJ Curr Val : " + String(torqMotor.setAdjTorq) );
			
			LogPrintln(" LG] *ADJ Set Val : " + String(val) );
		}
	
		else if( '2' == ch )		 
		{
			val = msg.substring(4,6).toInt();  
			if( 1 > val ) val = 1;
			if( 50 < val ) val = 50;
			
			LogPrintln(" LG] Set Torq ADJ Val, *ADJ Curr Val : " + String(torqMotor.setAdjTorq) );
			
			LogPrintln(" LG] *ADJ Set Val : " + String(val) );
			torqMotor.setAdjTorq = val;
		}
	 








		else
		{
			
			LogPrintln(" LG] Set Torq Undefine Val, " + String(msg) );
		}

}


 
 
 
 
 
 
 
void Set_LedValue( String msg)
{
		unsigned int val = 0;
		int pos = 0;
		char ch = msg.charAt(3);
		
		if( '0' == ch ) 		 
		{
			
		}
		else if( '1' == ch )		 
		{
			val = (unsigned int)msg.substring(5,(5+3)).toInt();  
			if( 1 > val ) val = 1;
			if( 100 < val ) val = 100;
			ledLight = val;
		}

		
		else if( '2' == ch )		 
		{
			pos = (unsigned int)msg.substring(4,(4+1)).toInt();  
			if( 0> pos) { pos=0; } if( 4<pos ) { pos =0; }
			
			val = (unsigned int)msg.substring(5,(5+4)).toInt();	 
			if( 10 > val ) val = 10;
			if( 9999 < val ) val = 9999;

			ledDimmHoldTime[pos] = val;
			
		}
	 








		else
		{
			
			LogPrintln(" LG] Set Torq Undefine Val, " + String(msg) );
		}

	if( '0' == ch || '1'==ch || '2'==ch || '3'==ch) 		 
	{
		LogPrintln(" LG] Set LED Light= " + String(ledLight) + "/%" );
		LogPrintln(" LG] Set LED HoldTime 1234: " + String(ledDimmHoldTime[1]) + "," + String(ledDimmHoldTime[2]) + "," + String(ledDimmHoldTime[3]) + "," + String(ledDimmHoldTime[4]) + ","  );
	}
}

 
 
 
 
 
void Set_TorqHitHoldOn( String msg)
{


}

void Set_EnbData( String msg)
{
	int val = 0;
	char ch = msg.charAt(3);
	
	if( '0' == ch ) 		 
	{
		LogPrintln(" LG] WaveType(1~3): " + String(waveType));
		LogPrintln(" LG] Fish Auto(0)Fix(1): " + String(fishTypeFixENB) + ", FishType(Fix): " + String(fishTypeFix) + ", FishType(Auto): " + String(fishTypeAuto) );
		LogPrintln(" LG] Hit Servo - Resist ENB: " + String(exeResistServoENB) + ", HoldOn ENB: " + String(exeHoldonServoENB) + ", Hit_Angle: " + String(hitServoAngle));
		LogPrintln(" LG] Hit_Resist_FIX: " + String(torqResistFix) + ", Torq Target: " + String(stHitTorqVal.targetTorq) + ", Resist: " + String(stHitTorqVal.holdonTorq) + ", low: " + String(stHitTorqVal.lowTorq) + ", Mid: " + String(stHitTorqVal.midTorq) );
 
	}
	else if( '1' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( LEVEL_MIN > val ) val = LEVEL_MIN;
		if( LEVEL_MAX < val ) val = LEVEL_MAX;
		gameLevel = val;
		
		LogPrintln(" LG] game Level, gameLevel: " + String(gameLevel) );
	}

	else if( 'A' == ch )		 
	{
		val = msg.substring(4).toInt();  
		if( 0 > val ) val = 0;
		if( 1 < val ) val = 1;
		torqResistFix = val;	 
	}
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
			initSet_LedContVal(1);					 
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


 
