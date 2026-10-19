 
#include "../DF_Main_Internal.h"
#include "DF_Main_GameControl.h"
void apPatternReq_Bite_Hold()
{
	if(0)	{ }		 
	
	else if(exeBite)
	 
	{
		 
		if(0)	{}	 
		
		else if(exeBitePtn1flag)
		{
			execBite_ptn1_Control();
		}
		else if(exeBitePtn2flag)
		{
			execBite_ptn2_Control();
		}
		else if(exeBitePtn3flag)
		{
			execBite_ptn3_Control();
		}
	}
	else if(holdOnPtnAll_Flag)
	{
		if(0)	{}	 
		
		else if(holdOnPtn1_Flag)
		{
			execHoldon_ptn1_BldcControl();
		}
	}
	
}

 
 
 
void testHand()
{
	if(0) { }	 
	
	 
	else if(exeWave)			 
	{
		switch(waveControlCase)			 
		{
			case 0:
				exeWave = 0;	 
				break;
			case 1:
				break;
			case 2:
				execWave_torque();
				break;
			case 3:
				execWave_bbn();
				break;
			case 4:
				execWave_bbn_Torq();	 
				break;
			case 5:
				execWave_torq_bbn();	 
				break;
			default:
				 
				break;
				
		}
	}

	 
	else if(exeBite)
	{
		 
		 

		if(biteLogOut)
		{
			biteLogOut = 0;
			
			LogPrintln(" LG] BITE_ Control: " + String(biteTorqCase));
		}

		if ( 0 == biteTorqCase)
		{ 
			exeBite = 0;	 
		}
		 
		else if( 1 == biteTorqCase ) { execBite_torque_justTarget(); }
		else if( 2 == biteTorqCase ) { execBite_torque_incTarget(); }
		else if( 3 == biteTorqCase ) { execBite_torquewf_Control(biteCnt); }
    	else
		{
			 
		}
	}
	 
	else if(exeHit)
	{
		execHiting();
	}

	 
	if(exeSuccessFail)
	{
		execSuccessFail();
	}

	else if(exeContinue)
	{

	}
	
	else if(exeGameOver)
	{
		 
		clear_ExecFlag();
		 
	    motor_AllDefault();

		 
		exeGameOver = 0;
	}

	if(exeProgramEnd)
	{
		clear_ExecFlag();
		motor_AllOff();			 
		reelOut_AllOff();		 
		exeProgramEnd = 0;
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

 







 



int randomNum(int min, int max)
{
	int random = (curr_ms_tick % 10);		 
	return (random*(max-min)/9 + min);		 
}



void biteType_TorqSet(int _fishKind)
{
	if( 1 > _fishKind ) { _fishKind = 1; }	 
	if( 8 < _fishKind ) { _fishKind = 8; }
	
	stBiteTorqVal.deepTorq = stBiteTorqVal.torqType[_fishKind];
}


 



int fishKind_autoFromBreak(int breakval)
{
	int fishkind = 1;
	
	if( (222) > breakval )      fishkind = 1;
	else if( (226) > breakval ) fishkind = 2;
	else if( (231) > breakval ) fishkind = 3;
	else if( (235) > breakval ) fishkind = 4;
	else if( (240) > breakval ) fishkind = 5;
	else if( (245) > breakval ) fishkind = 6;
	else if( (250) > breakval ) fishkind = 7;
	else                        fishkind = 8;

	return fishkind;
}

 
int fishKind_FromKg(double fishKg)
{
	int fishkind = 1;
	
	if(      ( 3.0) > fishKg ) fishkind = 1;
	else if( ( 9.0) > fishKg ) fishkind = 2;
	else if( (18.0) > fishKg ) fishkind = 3;
	else if( (28.0) > fishKg ) fishkind = 4;
	else if( (43.0) > fishKg ) fishkind = 5;
	else if( (63.0) > fishKg ) fishkind = 6;
	else if( (83.0) > fishKg ) fishkind = 7;
	else                       fishkind = 8;

	return fishkind;
}


 




void execWave_servo()
{
	static unsigned long wave_s_to = 0;
	static unsigned int wave_s_step = 0x00;
	
	switch(wave_s_step)
	{
		case 0x00:			 
			setTO(wave_s_to);
			wave_s_step = 0x01;
			break;
		case 0x01:			 
			if(checkTO(wave_s_to,3000))	 
			{
				wave_s_step = 0x02;
			}
			break;
		case 0x02:			 
			sMotor.setAngle(stWaveServoVal.angle2);		 
			setTO(wave_s_to);
			wave_s_step = 0x10;
			break;

		case 0x10:			 
			if(checkTO(wave_s_to, stWaveServoVal.interval))	 
			{
				wave_s_step = 0x11;
			}
			break;
			
		case 0x11:			 
			sMotor.setAngle(stWaveServoVal.angle1);		 
			setTO(wave_s_to);
			wave_s_step = 0x20;
			break;

		case 0x20:			 
			if(checkTO(wave_s_to, stWaveServoVal.interval))
			{
				wave_s_step = 0x02;		 
			}
			break;
		default:
			break;
	}
	
	if(exeBite || exeSuccessFail || exeContinue || exeGameOver)		 
	{
		exeWave = 0;		 
		wave_s_step = 0x00;
	}
}


unsigned int wave_tq_step = 0x00;
 
 
 
 
 
void execWave_torque()
{
	static unsigned int oldwave_t_step = 0x00;
	static unsigned long waveto = 0;
	
	
	switch(wave_tq_step)
	{
		case 0x00:			 
			setTO(waveto);
			 
			wave_tq_step = 0x02;		 
			break;
		case 0x01:			 
			if(checkTO(waveto,1000))	 
			{
				wave_tq_step = 0x02;
			}
			break;
			
		case 0x02:			 
			torqMotor.setValue(stWaveTorqVal.tgTorq);		 
			setTO(waveto);
			wave_tq_step = 0x10;
			break;

		case 0x10:			 
			if(checkTO(waveto, stWaveTorqVal.onInterval))		 
			{
				wave_tq_step = 0x11;
			}
			break;
			
		case 0x11:			 
			torqMotor.setValue(stWaveTorqVal.rtTorq);	 
			setTO(waveto);
			wave_tq_step = 0x20;
			break;

		case 0x20:			 
			if(checkTO(waveto, stWaveTorqVal.offInterval))
			{
				wave_tq_step = 0x02;		 
			}
			break;
		default:
			break;
	}

	if(exeBite || exeContinue || exeGameOver || exeProgramEnd)		 
	{
		exeWave = 0;	 
		oldwave_t_step = 0x00;
		wave_tq_step = 0x00;
		waveto = 0;
	}
}

unsigned int wave_bbn_tq_step = 0x01;
 
 
 
 
 
 
 
 
 
void execWave_bbn()
{
	static unsigned long waveto = 0;
	
	switch(wave_bbn_tq_step)
	{
		case 0x00:			 
			break;

		case 0x01:			 
			torqMotor.setValue(00);		 
			 
			setTO(waveto);
			wave_bbn_tq_step = 0x02;
			break;
			
		case 0x02:			 
			if(checkTO(waveto, 100)) 	 
			{
				bbnMotor.onBldc(BBN_MOT_CW, stWaveBdutyVal.tgTorq);		 
				setTO(waveto);
				wave_bbn_tq_step = 0x03;
			}
			break;

		case 0x03:			 
			if(checkTO(waveto, (stWaveBdutyVal.onInterval)/2)) 	 
			{
				wave_bbn_tq_step = 0x20;		 
				 
			}
			break;
			
		case 0x10:			 
			bbnMotor.onBldc(BBN_MOT_CW, stWaveBdutyVal.tgTorq);		 
			setTO(waveto);
			wave_bbn_tq_step = 0x11;
			break;

		case 0x11:			 
			if(checkTO(waveto, stWaveBdutyVal.onInterval)) 	 
			{
				wave_bbn_tq_step = 0x20;		 
				 
			}
			break;
			
		case 0x20:			 
			bbnMotor.onBldc(BBN_MOT_CCW, stWaveBdutyVal.tgTorq);		 
			setTO(waveto);
			wave_bbn_tq_step = 0x21;
			break;

		case 0x21:			 
			if(checkTO(waveto, stWaveBdutyVal.onInterval))
			{
				wave_bbn_tq_step = 0x10; 	 
			}
			break;

		case 0x30:			 
			bbnMotor.offBldc();
			setTO(waveto);
			wave_bbn_tq_step = 0x31;
			break;

		case 0x31:			 
			if(checkTO(waveto, stWaveBdutyVal.offInterval))
			{
				wave_bbn_tq_step = 0x10; 	 
			}
			break;

		default:
			break;
	}

	if(exeBite || exeSuccessFail || exeContinue || exeGameOver)		 
	{
		exeWave = 0;	 
		wave_bbn_tq_step = 0x00;
		torqMotor.setValue(DEFAULT_TORQ); 	 
		bbnMotor.offBldc();
	}
}


unsigned int wave_bbn_step = WAVE_BBN_TORQ_START;

 




void execWave_bbn_Torq()
{
	static unsigned long waveto = 0;
	
	switch(wave_bbn_step)
	{
		case 0x00:			 
			break;

		case 0x01:			 
			torqMotor.setValue(00);		 
			 
			setTO(waveto);
			wave_bbn_step = 0x02;
			break;
			
		case 0x02:			 
			if(checkTO(waveto, 100)) 	 
			{
				wave_bbn_step = 0x03;
			}
			break;

		case WAVE_BBN_TORQ_START:
			torqMotor.setValue(0);		 
			bbnMotor.onBldc(BBN_MOT_CW, stWaveBdutyVal.tgTorq); 	 
			setTO(waveto);
			wave_bbn_step = 0x04;
			break;
		case 0x04:			 
			 
			if(checkTO(waveto, (stWaveBdutyVal.onInterval))) 	 
			{
				wave_bbn_step = 0x20;		 
				 
			}
			break;
			
		case 0x10:			 
			torqMotor.setValue(DEFAULT_TORQ_30W_BELT);
			bbnMotor.onBldc(BBN_MOT_CW, stWaveBdutyVal.tgTorq);		 
			setTO(waveto);
			wave_bbn_step = 0x11;
			break;

		case 0x11:			 
			if(checkTO(waveto, stWaveBdutyVal.onInterval)) 	 
			{
				wave_bbn_step = 0x20;		 
				 
			}
			break;
			
		case 0x20:			 
			torqMotor.setValue(40);
			bbnMotor.onBldc(BBN_MOT_CCW, stWaveBdutyVal.tgTorq);		 
			setTO(waveto);
			wave_bbn_step = 0x21;
			break;

		case 0x21:			 
			if(checkTO(waveto, stWaveBdutyVal.onInterval))
			{
				wave_bbn_step = 0x10; 	 
			}
			break;

		case 0x30:			 
			bbnMotor.offBldc();
			setTO(waveto);
			wave_bbn_step = 0x31;
			break;

		case 0x31:			 
			if(checkTO(waveto, stWaveBdutyVal.offInterval))
			{
				wave_bbn_step = 0x10; 	 
			}
			break;

		default:
			break;
	}

	if(exeBite || exeSuccessFail || exeContinue || exeGameOver)		 
	{
		exeWave = 0;	 
		wave_bbn_step = WAVE_BBN_TORQ_START;
		torqMotor.setValue(DEFAULT_TORQ); 	 
		bbnMotor.offBldc();
	}
}

static unsigned short occureFault = 0;

unsigned int wave_torq_bbn_step = 0;

void execWave_torq_bbn_Start()
{
	wave_torq_bbn_step = 0x10;			 
}
 




void execWave_torq_bbn()
{
	static unsigned int oldwave_t_step = 0x00;
	static unsigned long waveto = 0;
	
	
	switch(wave_torq_bbn_step)
	{
		case 0x00:			 
		   




			break;

		case 0x10:			 
			torqMotor.setValue(0);		 
			bbnMotor.onBldc(BBN_MOT_CCW, stWavePwrBbn.pwr[waveType]);
			setTO(waveto);
			wave_torq_bbn_step = 0x11;
			break;
			
		case 0x11:			 
			if(checkTO(waveto, stWaveTorqVal.offInterval))		 
			{
				wave_torq_bbn_step = 0x20;
			}
			break;

		case 0x20:			 
			torqMotor.setValue(stWaveTorqVal.pwr[waveType]);		 
			bbnMotor.onBldc(BBN_MOT_CW, stWavePwrBbn.pwr[waveType]);
			setTO(waveto);
			wave_torq_bbn_step = 0x21;
			break;

		case 0x21:			 
			if(checkTO(waveto, stWaveTorqVal.onInterval))		 
			{
				wave_torq_bbn_step = 0x10;		 
			}
			break;

		 
		case 0x30:			 
			torqMotor.setValue(stWaveTorqVal.rtTorq);	 
			setTO(waveto);
			wave_torq_bbn_step = 0x31;
			break;

		case 0x31:			 
			if(checkTO(waveto, stWaveTorqVal.offInterval))
			{
				wave_torq_bbn_step = 0x10;		 
			}
			break;
		default:
			break;
	}

	 
	if(exeBite || exeHit || exeFight || exeResist || exeHoldon || exeRanding || exeSuccessFail || exeContinue || exeGameOver || exeProgramEnd)		 
	 
	{
		exeWave = 0;	 
		oldwave_t_step = 0x00;

		wave_torq_bbn_step = 0x00;			 
		torqMotor.setValue(DEFAULT_TORQ);

		bbnMotor.offBldc();
		waveto = 0;

		if(occureFault)
		{
			wave_torq_bbn_step = 0x00;
			torqMotor.setValue(DEFAULT_TORQ);
		}

	}
}


 
 
 

void execBite_servo()
{
}

unsigned int bite_jt_step = 0x00;
 
 
 
void execBite_torque_justTarget()
{

	static unsigned long bite_jt_to = 0;

	int outTorq;

	switch(bite_jt_step)
	{
		 
		case 0x00:			 
			 
			setTO(bite_jt_to);
			bite_jt_step = 0x01;
			break;
		case 0x01:			 
			if(checkTO(bite_jt_to,BITE_WAIT_TIME))		 
			{
				bite_jt_step = 0x20;	 
			}
			break;

		 
		case 0x20:			 
			outTorq = stBiteTorqVal.targetTorq;
			torqMotor.setValue(outTorq);
			setTO(bite_jt_to);
			bite_jt_step = 0x21;
			break;
		case 0x21:			 
			if(checkTO(bite_jt_to, stBiteTorqVal.targetInterval))
			{
				bite_jt_step = 0x30;			 
			}
			break;

		 
		case 0x30:			 
			outTorq -=  stBiteTorqVal.decTorq;		 
			if( END_TORQ > outTorq ) { outTorq = END_TORQ; }	 
			torqMotor.setValue(outTorq);
			setTO(bite_jt_to);
			bite_jt_step = 0x31; 	 
			break;
		case 0x31:			 
			if(checkTO(bite_jt_to, stBiteTorqVal.decInterval))
			{
				if(END_TORQ < outTorq)  { bite_jt_step = 0x30;	}	 
				else                    { bite_jt_step = 0x40;	}	 
			}
			break;

		 
		case 0x40:			 
			torqMotor.setValue(END_TORQ);
			setTO(bite_jt_to);
			bite_jt_step = 0x41;
			break;
		case 0x41:			 
			if(checkTO(bite_jt_to,BITE_WAIT_TIME))
			{
				bite_jt_step = 0x100;
			}
			break;

		 
		case 0x100:			 
			exeBite = 0; 	 
			bite_jt_step = 0x00;		 
			break;

		default:
			break;
			
	}
	
	if(exeHit || exeSuccessFail || exeContinue || exeGameOver)		 
	{
		exeBite = 0;	 
		bite_jt_step = 0x00;
	}

}


unsigned int bite_it_step = 0x00;
 
 
 
void execBite_torque_incTarget()
{

	static unsigned long bite_it_to = 0;

	int outTorq;

	switch(bite_it_to)
	{
		 
		case 0x00:			 
			 
			setTO(bite_it_to);
			bite_it_to = 0x01;
			break;
		case 0x01:			 
			if(checkTO(bite_it_to, BITE_WAIT_TIME))		 
			{
				bite_it_to = 0x10;	 
				outTorq = START_TORQ;
			}
			break;

		 
		case 0x10:			 
			outTorq +=  stBiteTorqVal.incTorq;		 
			if( outTorq > stBiteTorqVal.targetTorq ) { outTorq = stBiteTorqVal.targetTorq; }	 
			torqMotor.setValue(outTorq);
			setTO(bite_it_to);
			bite_it_to = 0x11; 	 
			break;
		case 0x11:			 
			if(checkTO(bite_it_to, stBiteTorqVal.incInterval))
			{
				if(outTorq < stBiteTorqVal.targetTorq)  { bite_it_to = 0x10;	}	 
				else                                    { bite_it_to = 0x20;	}	 
			}
			break;

		 
		case 0x20:			 
			outTorq = stBiteTorqVal.targetTorq;
			torqMotor.setValue(outTorq);
			setTO(bite_it_to);
			bite_it_to = 0x21;
			break;
		case 0x21:			 
			if(checkTO(bite_it_to, stBiteTorqVal.targetInterval))
			{
				bite_it_to = 0x30;			 
			}
			break;

		 
		case 0x30:			 
			outTorq -=  stBiteTorqVal.decTorq;		 
			if( END_TORQ > outTorq ) { outTorq = END_TORQ; }	 
			torqMotor.setValue(outTorq);
			setTO(bite_it_to);
			bite_it_to = 0x31; 	 
			break;
		case 0x31:			 
			if(checkTO(bite_it_to, stBiteTorqVal.decInterval))
			{
				if(END_TORQ < outTorq)  { bite_it_to = 0x30;	}	 
				else                    { bite_it_to = 0x40;	}	 
			}
			break;

		 
		case 0x40:			 
			torqMotor.setValue(END_TORQ);
			setTO(bite_it_to);
			bite_it_to = 0x41;
			break;
		case 0x41:			 
			if(checkTO(bite_it_to, BITE_WAIT_TIME))
			{
				bite_it_to = 0x100;
			}
			break;

		 
		case 0x100:			 
			exeBite = 0; 	 
			bite_it_to = 0x00;		 
			break;

		default:
			break;
	}
	
	if(exeHit || exeSuccessFail || exeContinue || exeGameOver)		 
	{
		exeBite = 0;	 
		bite_it_to = 0x00;
	}

}


 
 
 
unsigned int rdy_incStep = 0x00;

void rdyincTorq_Stop()
{
	rdy_incStep = 0x00;
	rdy_outTorq = DEFAULT_TORQ;  
	rdy_eachTime = 1000;
	rdy_cnt = 0;
}

 
void rdyincTorq_Start()
{
	rdy_incStep = 0x10;
}


 
void setReadyTorque_incTarget(int targetTorq, int incTime, int incTorq)
{
	rdy_outTorq = DEFAULT_TORQ;  
	rdy_eachTime = incTime;
	rdy_incTorq = incTorq;
	rdy_cnt = 0;
	rdy_reqCnt = (targetTorq - DEFAULT_TORQ)/incTorq;
}

 
 
 
void setReadyPosition_Start(int option)
{
	switch(option)
	{
		case 0:		 
			break;
			
		case 1:		 
			if(100 < continueTorq)
			{
				setReadyTorque_incTarget(continueTorq, 1000, 30);  
			}
			else
			{
				setReadyTorque_incTarget(continueTorq, 1000, 20);  
			}
			rdyincTorq_Start();
			execContinue2Ready = 1;			 
			 
			break;
      
		default:
			break;
			
	}
}

 
 

void execReady_TorqControl()
{

	static unsigned int oldrdy_incStep = 0xFF;
	static unsigned long _to = 0;
	static int outTorq;	 

	String rtnMsg = "";
	
	switch(rdy_incStep)
	{
		 
		case 0x00:			 
			break;
			
		case 0x10:			 
			rdy_cnt = 0;
			rdy_outTorq = DEFAULT_TORQ;  
			torqMotor.setValue(rdy_outTorq);
			 
				
			setTO(_to);
			rdy_incStep = 0x11;
			break;

		case 0x11:			 
			if(checkTO(_to,1000))		 
			{
				setTO(_to);
				rdy_incStep = 0x20;
			}
			break;
			
		case 0x20:			 
			rdy_cnt++;
			if(rdy_cnt > rdy_reqCnt)
			{
				rdy_incStep = 0x30;
			}
			else if(checkTO(_to, rdy_eachTime))		 
			{
				rdy_outTorq += rdy_incTorq;			 
				torqMotor.setValue(rdy_outTorq);
			}
			break;


		case 0x30:
			rdy_incStep = 0x90;

		 
			
			break;
			 





		 
		case 0x90:			 
			execContinue2Ready = 0; 	 
			rdy_incStep = 0x00;			 
			oldrdy_incStep = 0x00;		 
			rdy_cnt = 0;
			
			 
			rtnMsg = STX_GAME_STS + "0401%";		 
			 
			Resp2ApPrintln(rtnMsg);			 
			
			LogPrintln(" LG] RDYCN END");
			break;

		default:
			break;
	}

}


unsigned int bite_bldc_step = 0x00;
static unsigned int bite_bldcRptCnt = 0;
static unsigned long bite_bldc_to = 0;

void execBite_BldcStart()
{
	bite_bldc_step = 0x10;
}

#define TO_CHG_DIR_STOP 10
 
 
 
void execBite_BldcControl()
{
	static unsigned int old_bite_wt_step = 0x00;
	
	 

	switch(bite_bldc_step)
	{
		case 0x00:	 
			break;
			
		case 0x10:	 
			bite_bldcRptCnt = 0;
			setTO(bite_bldc_to);
			bite_bldc_step = 0x20;
			break;
			
		case 0x20:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnActTbl[BBNACT_BITE].onDuty);  
			setTO(bite_bldc_to);
			bite_bldc_step = 0x21;
			break;
		case 0x21:
			if(checkTO(bite_bldc_to, sBbnActTbl[BBNACT_BITE].onTime))
			{
					bbnMotor.offBldc(); 			 
					setTO(bite_bldc_to);
					bite_bldc_step = 0x39;	 
			}
			break;

		 
		case 0x30:		 
			bbnMotor.offBldc(); 			 
			setTO(bite_bldc_to);
			bite_bldc_step = 0x31;
			break;

		case 0x31:	 
			if(checkTO(bite_bldc_to, sBbnActTbl[BBNACT_BITE].offTime))
			{
				bite_bldcRptCnt++;
				if( bbnReqCnt[BBNACT_BITE] < bite_bldcRptCnt)
				{
					bite_bldc_step = 0x70;		 
				}
				else
				{
					bite_bldc_step = 0x20;			 
				}
			}
			break;

		 
		case 0x39:		 
			if(checkTO(bite_bldc_to, TO_CHG_DIR_STOP))		 
			{
				bite_bldc_step = 0x40;
			}
			break;
			
		case 0x40:		 
			bbnMotor.onBldc(BBN_MOT_CCW, sBbnActTbl[BBNACT_BITE].onDuty);  
			setTO(bite_bldc_to);
			bite_bldc_step = 0x41;
			break;

		case 0x41:	 
			if(checkTO(bite_bldc_to, sBbnActTbl[BBNACT_BITE].onTime))
			{
				bite_bldcRptCnt++;
				if( bbnReqCnt[BBNACT_BITE] < bite_bldcRptCnt)
				{
					bite_bldc_step = 0x70;		 
				}
				else
				{
					bbnMotor.offBldc(); 			 
					setTO(bite_bldc_to);
					bite_bldc_step = 0x42;			 
				}
			}
			break;

		case 0x42:
			if(checkTO(bite_bldc_to, TO_CHG_DIR_STOP))		 
			{
				bite_bldc_step = 0x20;		 
			}
			break;

		case 0x50:	 
			break;

		case 0x60:	 
			break;
			

		case 0x70:	 
			bbnMotor.offBldc(); 			 
			 
			bite_bldcRptCnt = 0;
			exeFlag_biteBldcTokTokEnd = 1;	 
			bite_bldc_step = 0x00;
			bite_bldc_to = 0;
			break;
		default:
			 
			break;
	}
}


 
 
 
unsigned int bite_wf_step = 0x00;
static int biteReqCnt = 0;					 

void execBite_torquewf_Start(int reqCnt)
{
	biteReqCnt = reqCnt;			 
	bite_wf_step = 0x10;
}

 
 
 
 
void execBite_torquewf_Control(int reqCnt)
{
	static unsigned long _to = 0;
	static unsigned int oldbite_wf_step = 0x00;

	static int _cnt = 0;

	static int outTorq;


	switch(bite_wf_step)
	{
		 
		case 0x00:			 
			break;

		case 0x10:
			 
			setTO(_to);
			_cnt = 0;
			 
			bite_wf_step = 0x11;
			break;
		
		case 0x11:			 
			if(checkTO(_to, BITE_WAIT_TIME))		 
			{
				bite_wf_step = 0x20;	 
			}
			break;
		
		 
		case 0x20:
			_cnt++;
			if(reqCnt < _cnt)	 
			{
				_cnt = 0;
				bite_wf_step = 0x30;	 
			}
			else
			{
				outTorq = stBiteTorqVal.shallowTorq;
				torqMotor.setValue(outTorq);		 
				setTO(_to);
				bite_wf_step = 0x21;
			}
			break;
		case 0x21:			 
			if(checkTO(_to, stBiteTorqVal.shallowInterval))		 
			{
				bite_wf_step = 0x22;		 
			}
			break;
			
		case 0x22:
			torqMotor.setValue(START_TORQ);
			setTO(_to);
			bite_wf_step = 0x23;
			break;

		case 0x23:			 
			if(checkTO(_to, stBiteTorqVal.shallowInterval))
			{
				bite_wf_step = 0x20; 		 
			}
			break;

		 
		case 0x30:			 
			outTorq = stBiteTorqVal.torqType[fishTypeAuto];
			torqMotor.setValue(outTorq);			 
			setTO(_to);
			bite_wf_step = 0x31;
			break;
		case 0x31:			 
			if(checkTO(_to, stBiteTorqVal.deepInterval))
			{
				bite_wf_step = 0x40;			 
			}
			break;

		 
		case 0x40:			 
			outTorq -=  stBiteTorqVal.decTorq;		 
			if( END_TORQ > outTorq )
			{ 	 
				torqMotor.setValue(END_TORQ);
				bite_wf_step = 0x50;			 
			}
			else
			{
				torqMotor.setValue(outTorq);
				setTO(_to);
				bite_wf_step = 0x41; 	 
			}
			break;
		case 0x41:			 
			if(checkTO(_to, stBiteTorqVal.decInterval))
			{
				bite_wf_step = 0x40;	 
			}
			break;

		 
		case 0x50:			 
			torqMotor.setValue(WF_END_TORQ);	 
			if(1)		 
			{
				execBite_BldcStart();
				bite_wf_step = 0x51;
			}	 
			else
			{
				setTO(_to);
				bite_wf_step = 0x52;
			}	 
			break;

		case 0x51:			 
			execBite_BldcControl();			 
			if(exeFlag_biteBldcTokTokEnd)		 
			{
				exeFlag_biteBldcTokTokEnd = 0;
				setTO(_to);
				bite_wf_step = 0x52;
			}
			break;
			
		case 0x52:			 
			if(checkTO(_to, BITE_WAIT_TIME))
			{
				bite_wf_step = 0x90;
			}
			break;

		 
		case 0x90:			 
			exeBite = 0; 	 
			bite_wf_step = 0x00;		 
			oldbite_wf_step = 0x00;
			_cnt = 0;
			break;

		default:
			break;
	}
	
	if(exeHit || exeSuccessFail || exeContinue || exeGameOver || exeProgramEnd)		 
	{
		torqMotor.setValue(END_TORQ);
		bbnMotor.offBldc();
		exeBite = 0;				 

		oldbite_wf_step = 0x00;		 
		bite_wf_step = 0x00;		 

		exeFlag_biteBldcTokTokEnd = 0;
		bite_bldc_step = 0;			 

		_cnt = 0;
		outTorq = 0;
	}

}

 
static int bitePtn1Cnt = 3;			 
static int bitePtn2Cnt = 1;			 
static int bitePtn3Cnt = 4;			 

static unsigned int bite_ptn1_step = 0;
void execBite_ptn1_Start()
{
	 
	bite_ptn1_step = 10;

	LogPrintln(" LG] EXEBT Start PTN_1 cnt=" + String(bitePtn1Cnt));
}

static unsigned int bite_ptn2_step = 0;
void execBite_ptn2_Start()
{
	 
	bite_ptn2_step = 10;
	LogPrintln(" LG] EXEBT Start PTN_2 cnt=" + String(bitePtn2Cnt));
}

static unsigned int bite_ptn3_step = 0;
void execBite_ptn3_Start()
{
	 
	bite_ptn3_step = 10;
	LogPrintln(" LG] EXEBT Start PTN_3 cnt=" + String(bitePtn3Cnt));
}

void execBite_ptn_Flag_Clear()
{
	exeBitePtn1flag = 0;
	bite_ptn1_step = 0;
	exeBitePtn2flag = 0;
	bite_ptn2_step = 0;
	exeBitePtn3flag = 0;
	bite_ptn3_step = 0;
}

void exeBiteFlag_Clr_check()		 
{
	if(0 == exeBitePtn1flag && 0 == exeBitePtn2flag && 0 == exeBitePtn3flag )
	{
		 
		exeBite = 0; 	 
		execBite_ptn_Stop();
	}

}

void execBite_ptn_Stop()
{
	execBite_ptn_Flag_Clear();
	
	 
	bbnMotor.offBldc();
	torqMotor.setValue(reqTorqueMotor);
	send2resp_FwSelfControlCmd(STX_TORQUE_MOTOR, reqTorqueMotor);

	LogPrintln(" LG] EXEBT END PTN_ALL");
}

 
 
 
 
void execBite_ptn1_Control()
{
	static unsigned long _to = 0;
	static unsigned int oldbite_ptn1_step = 0x00;

	static int _cnt = 0;

	static int outTorq;


	switch(bite_ptn1_step)
	{
		case 0x00:			 
			break;

		 
		case 10:
			_cnt = 0;
			bite_ptn1_step = 20;
			break;
			
		 
		case 20:
			_cnt++;
			if(bitePtn1Cnt < _cnt)	 
			{
				bite_ptn1_step = 90;	 
			}
			else
			{
				outTorq = stBiteTorqVal.shallowTorq;
				torqMotor.setValue(outTorq);		 
				setTO(_to);
				bite_ptn1_step = 21;
			}
			break;
		case 21:			 
			if(checkTO(_to, stBiteTorqVal.shallowInterval))		 
			{
				bite_ptn1_step = 30;		 
			}
			break;
			
		case 30:
			 
			torqMotor.setValue(OFF_TORQ);
			
			setTO(_to);
			bite_ptn1_step = 31;
			break;

		case 31:			 
			if(checkTO(_to, stBiteTorqVal.shallowInterval))
			{
				bite_ptn1_step = 20; 		 
			}
			break;

		 
		case 90:			 
			_cnt = 0;
			bite_ptn1_step = 0;		 
			oldbite_ptn1_step = 0;

			exeBitePtn1flag = 0;
			exeBiteFlag_Clr_check();
			break;

		default:
			break;
	}
	
}

 
 
 
 
 
void execBite_ptn2_Control()
{
	static unsigned long _to = 0;
	static unsigned int oldbite_ptn2_step = 0x00;

	static int _cnt = 0;

	static int outTorq;


	switch(bite_ptn2_step)
	{
		case 0x00:			 
			break;

		 
		case 10:
			_cnt = 0;
			bite_ptn2_step = 20;
			break;
			
		 
		case 20:			 
			outTorq = stBiteTorqVal.torqType[fishTypeAuto];
			torqMotor.setValue(outTorq);			 
			setTO(_to);
			bite_ptn2_step = 21;
			break;
		case 21:			 
			if(checkTO(_to, stBiteTorqVal.deepInterval))
			{
				bite_ptn2_step = 30;			 
			}
			break;

		 
		case 30:			 
			outTorq -=  stBiteTorqVal.decTorq;		 
			if( END_TORQ > outTorq )
			{ 	 
				torqMotor.setValue(END_TORQ);
				bite_ptn2_step = 40;			 
			}
			else
			{
				torqMotor.setValue(outTorq);
				setTO(_to);
				bite_ptn2_step = 31; 	 
			}
			break;
		case 31:			 
			if(checkTO(_to, stBiteTorqVal.decInterval))
			{
				bite_ptn2_step = 30;	 
			}
			break;

		 
		case 40:			 
			torqMotor.setValue(WF_END_TORQ);	 
			bite_ptn2_step = 90;	 
			break;

		 
		case 90:			 
			_cnt = 0;
			bite_ptn2_step = 0;		 
			oldbite_ptn2_step = 0;

			exeBitePtn2flag = 0;
			exeBiteFlag_Clr_check();
			break;

		default:
			break;
	}
	
}

 
 
 
 
void execBite_ptn3_Control()
{
	static unsigned long _to = 0;
	static unsigned int oldbite_ptn3_step = 0x00;

	static int _cnt = 0;

	static int outTorq;

		
	switch(bite_ptn3_step)
	{
		case 0:			 
			break;
		
		 
		case 10:	 
			_cnt = 0;
			bite_ptn3_step = 20;
			break;

		 
		case 20:	 
			_cnt++;
			if( bitePtn3Cnt < _cnt)
			{
				bite_ptn3_step = 90;		 
			}
			else
			{
				bbnMotor.onBldc(BBN_MOT_CW, sBbnBiteVal.onDuty);  
				setTO(_to);
				bite_ptn3_step = 21;
			}
			break;
		case 21:
			if(checkTO(_to, sBbnBiteVal.onTime))
			{
				bbnMotor.offBldc(); 			 
				setTO(_to);
				bite_ptn3_step = 22;
			}
			break;

		 
		case 22:		 
			if(checkTO(_to, TO_CHG_DIR_STOP))		 
			{
				bite_ptn3_step = 40;
			}
			break;
			
		case 40:		 
			bbnMotor.onBldc(BBN_MOT_CCW, sBbnBiteVal.onDuty);  
			setTO(_to);
			bite_ptn3_step = 41;
			break;

		case 41:	 
			if(checkTO(_to, sBbnBiteVal.onTime))
			{
				bbnMotor.offBldc(); 			 
				setTO(_to);
				bite_ptn3_step = 42;			 
			}
			break;

		case 42:
			if(checkTO(_to, TO_CHG_DIR_STOP))		 
			{
				bite_ptn3_step = 20;		 
			}
			break;

		case 50:	 
			break;

		case 60:	 
			break;
			

		case 90:	 
			
			_cnt = 0;
			bite_ptn3_step = 0;		 
			oldbite_ptn3_step = 0;

			exeBitePtn3flag = 0;
			exeBiteFlag_Clr_check();
			 
			 
			break;
			
		default:
			 
			break;
	}
}


 
unsigned int hit_BldcStep = 0x00;
static unsigned long hit_bldc_to = 0;
static unsigned int hitBldcRptCnt = 0;

void execHit_BldcStart()
{
	hit_BldcStep = 0x10;	 
}

 
 
 
void execHit_1st_BldcControl()
{
	static unsigned int old_bldc_step = 0x00;
	
	 

	switch(hit_BldcStep)
	{
		case 0x00:	 
			break;
			
		case 0x10:	 
			hitBldcRptCnt = 0;
			setTO(hit_bldc_to);
			hit_BldcStep = 0x20;
			break;
			
		case 0x20:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnActTbl[BBNACT_HIT].onDuty);  
			setTO(hit_bldc_to);
			hit_BldcStep = 0x30;
			break;
		case 0x30:
			if(checkTO(hit_bldc_to, sBbnActTbl[BBNACT_HIT].onTime))
			{
				bbnMotor.offBldc(); 			 
				setTO(hit_bldc_to);
				hit_BldcStep = 0x40;
			}
			break;
		case 0x40:	 
			if(checkTO(hit_bldc_to, sBbnActTbl[BBNACT_HIT].offTime))
			{
				hitBldcRptCnt++;
				if( bbnReqCnt[BBNACT_HIT] < hitBldcRptCnt)
				{
					hit_BldcStep = 0x70;		 
				}
				else
				{
					hit_BldcStep = 0x20;			 
				}
			}
			break;

		case 0x50:	 
			break;

		case 0x60:	 
			break;
			

		case 0x70:	 
			bbnMotor.offBldc(); 			 
			 
			hitBldcRptCnt = 0;
			exeHit_1st = 0;	 
			hit_BldcStep = 0x00;		 
			break;
		default:
			 
			break;
	}
}



 
 
 
void execHiting()
{
	 
	if(exeHit_1st)
	{
		execHit_1st_BldcControl();
	}

	 
	{
		 
	}

	 
	if(!exeResist && !exeResistOnceComp)
	{ 
		execHit_ResistRandom_ToCheck();
	}


	if(0) { }	 

	 
	 
	else if(exeRanding)
	{
		execRanding_Control();		 
	}

	 
	else if(exeHoldon)			 
	{
		 
		if(exeHoldonServoENB)
		{
		}
		 
		execHit_HoldOn_TorqControl();
		 
	}
	 
	else if(exeResist)		 
	{
		if(exeResistServoENB)
		{
		}
		execHit_ResistTorqControl();
	}
	 
	else		 
	{
		 
		execHit_BreakDecControl();
		
		if(exeFight)
		{
			execHit_FightBldcControl();
		}
	}
}

unsigned int bbnFight_step = 0x00;
static unsigned int oldbbnFight_step = 0x00;
static unsigned long bbnFightTO = 0;
static unsigned int bbnFightCnt = 0;
static unsigned int bbnFight_ptn = 0;

void execHit_FightBldcStop()
{
	exeFight = 0;			 
	bbnMotor.offBldc(); 		 
	
	oldbbnFight_step = 0;
	bbnFight_step = 0;				
	bbnFightTO = 0;
	bbnFightCnt = 0;
	bbnFight_ptn = 0;
	
}

void execHit_FightBldcStart(unsigned _ptn)		 
{
		bbnFight_step = 0x10;  
		bbnFight_ptn = _ptn;
}

 
 
void execHit_FightBldcControl()		 
{
	 
   







	
	switch(bbnFight_step)
	{
		case 0x00:	 
			break;
			
		case 0x10:	 
			bbnFightCnt++;
			setTO(bbnFightTO);
			bbnFight_step = 0x20;
			break;
			
		case 0x20:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnActTbl[BBNACT_FIGHT].onDuty);	 
			setTO(bbnFightTO);
			bbnFight_step = 0x21;
			break;
		case 0x21:	 
			if(checkTO(bbnFightTO, sBbnActTbl[BBNACT_FIGHT].onTime))
			{
				bbnMotor.offBldc();				 
				setTO(bbnFightTO);
				bbnFight_step = 0x22;
			}
			break;
		case 0x22:	 
			if(checkTO(bbnFightTO,30))
			{
				bbnFight_step = 0x23;
			}
			break;

		case 0x23:
			if(FIGHT_PTN_CW_OFF == bbnFight_ptn)
			{
				bbnFight_step = 0x30;		 
			}
			else
			{
				bbnFight_step = 0x40;		 
			}
			break;

		case 0x30:	 
			if(checkTO(bbnFightTO, sBbnActTbl[BBNACT_FIGHT].offTime))
			{
				if ( 0 == bbnReqCnt[BBNACT_FIGHT] )		 
				{
					bbnFight_step = 0x20;			 
				}
				else
				{
					bbnRptCnt++;
					if( bbnReqCnt[BBNACT_FIGHT] < bbnRptCnt)
					{
						bbnFight_step = 0x70;		 
					}
					else
					{
						bbnFight_step = 0x20;			 
					}
				}
			}
			break;

		case 0x40:	 
			bbnMotor.onBldc(BBN_MOT_CCW, sBbnActTbl[BBNACT_FIGHT].onDuty);	 
			setTO(bbnFightTO);
			bbnFight_step = 0x41;
			break;

		case 0x41:	 
			if(checkTO(bbnFightTO, sBbnActTbl[BBNACT_FIGHT].onTime))
			{
				bbnMotor.offBldc();				 
				setTO(bbnFightTO);
				bbnFight_step = 0x42;		 
			}
			break;
		case 0x42:	 
			if(checkTO(bbnFightTO, 30))
			{
				bbnFight_step = 0x20;		 
			}
			break;
			
		case 0x50:	 
			break;
		case 0x60:	 
			break;

		case 0x70:	 
			bbnMotor.offBldc(); 			 
			 
			bbnFightTO = 0;
			bbnFightCnt = 0;			
			bbnFight_step = 0x00;
			break;
		default:
			 
			break;
	}

	 
	 
	if(exeRanding || exeSuccessFail || exeContinue || exeGameOver || exeProgramEnd )
	{
		execHit_FightBldcStop();
	}
}


 
 
 
unsigned int hitResistRdStep = 0x10;	 


void execHit_ResistRandomStart()
{
	hitResistRdStep = 0x10;	 
}

 
 
 
void execHit_ResistRandom_ToCheck()
{
	static unsigned long to = 0;
	static int waittime = 0;
	 
	
	switch(hitResistRdStep)
	{
		case 0x00:			 
			break;
		case 0x10:			 
			setTO(to);
			 
			 
			waittime = randomNum(1000,4000);
			hitResistRdStep = 0x20;		
			break;
		case 0x20:			 
			if(checkTO(to,waittime))  
			{
				hitResistRdStep = 0x00;			 

				exeResist = 1;	 
				exeResistOnceComp = 1;
				execHit_ResistTorqStart();

				 
				
				LogPrintln(" RT] RESIT Fix:" + String(torqResistFix) + ",Tgt:" + String(stHitTorqVal.targetTorq) + ",Hold:" + String(stHitTorqVal.holdonTorq) + ",Low:" + String(stHitTorqVal.lowTorq) + ",mid" + String(stHitTorqVal.midTorq) );
			  
			}
			break;
		default:
			break;
	}
}

 
 
 
void execHit_HoldServo()
{
}

unsigned int hit_resist_step = 0x00;

void execHit_ResistTorqStop()
{
	exeResist = 0;
	exeResistOnceComp = 1;		 
	hit_resist_step = 0x00;		 
}

void execHit_ResistTorqStart()
{
	hit_resist_step = 0x10;		 
}

void execHit_ResistTorqControl()
{
	 
	  static unsigned long _to = 0;
	  static unsigned int old_hit_resist_step = 0x00;
	
	  static int resistCnt = 0;
	
	  static int outTorq;
	  static int outBreak;
	
	
	  switch(hit_resist_step)
	  {
		  case 0x00:		   
			  break;
		  
		   
		  case 0x10:
			  outTorq = stHitTorqVal.holdonTorq;
			  torqMotor.setValue(outTorq);		   
			  outBreak = targetBreakVal + 10;
			  send_breakmot(outBreak);
			  setTO(_to);
			  hit_resist_step = 0x11;
			  break;
		  case 0x11:		   
			  if(checkTO(_to, stHitTorqVal.holdonInterval))	   
			  {
				  hit_resist_step = 0x20;	   
			  }
			  break;
			  
		  case 0x20:
			  torqMotor.setValue(stHitTorqVal.lowTorq);
			  send_breakmot(targetBreakVal);
			  setTO(_to);
			  hit_resist_step = 0x21;
			  break;	
		  case 0x21:		   
			  if(checkTO(_to, stHitTorqVal.lowInterval))
			  {
				  hit_resist_step = 0x30;		   
			  }
			  break;
	
		   
		  case 0x30:		   
			  outTorq = stHitTorqVal.midTorq;
			  torqMotor.setValue(outTorq);			   
			  outBreak = targetBreakVal + 5;
			  send_breakmot(outBreak);
			  setTO(_to);
			  hit_resist_step = 0x31;
			  break;
		  case 0x31:		   
			  if(checkTO(_to, stHitTorqVal.midInterval))
			  {
				  hit_resist_step = 0x40;			   
			  }
			  break;
	
		   
		  case 0x40:		   
			  torqMotor.setValue(reqTorqueMotor);
			  send_breakmot(outBreakVal);
			  setTO(_to);
			  hit_resist_step = 0x41;
			  break;
		  case 0x41:		   
			  if(checkTO(_to, HIT_WAIT_TIME))
			  {
				  hit_resist_step = 0x90;
			  }
			  break;
	
		   
		  case 0x90:		   
			  exeResist = 0;	   
			  hit_resist_step = 0x00;		   
			  old_hit_resist_step = 0x00;
			  resistCnt = 0;
			  break;
	
		  default:
			  break;
	  }
	  
	  if(exeRanding || exeHoldon || exeSuccessFail || exeContinue || exeGameOver)	   
	  {
		  torqMotor.setValue(reqTorqueMotor);
		  send_breakmot(outBreakVal);
		  exeResist = 0;	   
		  exeResistOnceComp = 1;		 
		  hit_resist_step = 0x00;
		  old_hit_resist_step = 0x00;
		  resistCnt = 0;
	  }
	

}

 
 
unsigned int holdon_bldc_step = 0x00;	 
static unsigned long holdon_bldc_to = 0;
static unsigned int holdon_bldcRptCnt = 0;

void execHit_Holdon_BldcStart()
{
	holdon_bldc_step = 0x10;
}

 
void execHit_Holdon_BldcControl()
{
	static unsigned int old_holdon_bldc_step = 0x00;
	
	 
  
	switch(holdon_bldc_step)
	{
		case 0x00:	 
			break;
			
		case 0x10:	 
			holdon_bldcRptCnt = 0;
			setTO(holdon_bldc_to);
			holdon_bldc_step = 0x20;
			break;
			
		case 0x20:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnActTbl[BBNACT_HOLDON].onDuty);  
			setTO(holdon_bldc_to);
			holdon_bldc_step = 0x30;
			break;
		case 0x30:
			if(checkTO(holdon_bldc_to, sBbnActTbl[BBNACT_HOLDON].onTime))
			{
				bbnMotor.offBldc(); 			 
				setTO(holdon_bldc_to);
				holdon_bldc_step = 0x40;
			}
			break;
		case 0x40:	 
			if(checkTO(holdon_bldc_to, sBbnActTbl[BBNACT_HOLDON].offTime))
			{
				holdon_bldcRptCnt++;
				if( bbnReqCnt[BBNACT_HOLDON] < holdon_bldcRptCnt)
				{
					holdon_bldc_step = 0x70;		 
				}
				else
				{
					holdon_bldc_step = 0x20;			 
				}
			}
			break;

		case 0x50:	 
			break;

		case 0x60:	 
			break;
			

		case 0x70:	 
			bbnMotor.offBldc(); 			 
			 
			holdon_bldcRptCnt = 0;
			exeFlag_holonBldcTokTokEnd = 1;	 
			holdon_bldc_step = 0x00;
			break;
			
		default:
			 
			break;
	}
}

 
 
static int HoldOnPtn1Cnt = 4;			 

static unsigned int Holdon_ptn1_step = 0;

void execHoldon_ptn1_Start()
{
	holdOnPtnAll_Flag = 1;			 
	
	holdOnPtn1_Flag = 1;
	Holdon_ptn1_step = 10;

	LogPrintln(" LG] EXEHD Start PTN_1 cnt=" + String(HoldOnPtn1Cnt));
}


 
 
void execHoldon_ptn1_BldcControl()
{
	static unsigned long _to = 0;
	static unsigned int oldHoldon_ptn1_step = 0x00;

	static unsigned int _cnt = 0;
	
	 
  
	switch(Holdon_ptn1_step)
	{
		case 0:	 
			break;
			
		case 10:	 
			_cnt = 0;
			Holdon_ptn1_step = 20;
			break;

		 
		case 20:
			_cnt++;
			if(_cnt > HoldOnPtn1Cnt)
			{
				Holdon_ptn1_step = 90;
			}
			else
			{
				setTO(_to);
				Holdon_ptn1_step = 21;
			}
			break;
			
		case 21:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnHoldVal[fishTypeAuto]);  
			setTO(_to);
			Holdon_ptn1_step = 22;
			break;
			
		case 22:
			if(checkTO(_to, sBbnHoldVal_onTime))
			{
				bbnMotor.offBldc(); 			 
				setTO(_to);
				Holdon_ptn1_step = 40;
			}
			break;
			
		case 40:	 
			if(checkTO(_to, sBbnHoldVal_offTime))
			{
				Holdon_ptn1_step = 20;			 
			}
			break;

		case 50:	 
			break;
		case 60:	 
			break;
			

		case 90:	 
			_cnt = 0;
			Holdon_ptn1_step = 0;
			oldHoldon_ptn1_step = 0;

			holdOnPtn1_Flag = 0;
			 
			Check_HoldOnPtn_AllClr();
			break;
			
		default:
			break;
	}
}

 
void Check_HoldOnPtn_AllClr()
{
	 
	if(!holdOnPtn1_Flag && 0 && 0 && 0)
	{
		holdOnPtnAll_Flag = 0;
	}
}

 
unsigned int hit_holdon_step = 0x00;

void execHit_HoldOn_TorqStop()
{
	torqMotor.setValue(reqTorqueMotor);
	send_breakmot(outBreakVal);
	hit_holdon_step = 0x10;
}

void execHit_HoldOn_TorqStart()
{
	hit_holdon_step = 0x10;
}

void execHit_HoldOn_TorqControl()
{
	 
	  static unsigned int old_hit_holdon_step = 0x00;
	  static unsigned long hit_holdon_to = 0;
	
	  static int holdonCnt = 0;
	
	  static int outTorq;
	  static int outBreak;
	
	
	  switch(hit_holdon_step)
	  {
		  case 0x00:		   
			  break;
		  
		   
		  case 0x10:
			  outTorq = stHitTorqVal.holdonTorq;
			  torqMotor.setValue(outTorq);		   
			  outBreak = targetBreakVal + 10;
			  send_breakmot(outBreak);
			  setTO(hit_holdon_to);
			  hit_holdon_step = 0x11;
			  break;
		  case 0x11:		   
			  if(checkTO(hit_holdon_to, stHitTorqVal.holdonInterval))	   
			  {
				  hit_holdon_step = 0x20;	   
			  }
			  break;
			  
		  case 0x20:
			  torqMotor.setValue(stHitTorqVal.lowTorq);
			  send_breakmot(targetBreakVal);
			  setTO(hit_holdon_to);
			  hit_holdon_step = 0x21;
			  break;
	
		  case 0x21:		   
			  if(checkTO(hit_holdon_to, stHitTorqVal.lowInterval))
			  {
				  hit_holdon_step = 0x30;		   
			  }
			  break;
	
		   
		  case 0x30:		   
			  outTorq = stHitTorqVal.midTorq;
			  torqMotor.setValue(outTorq);			   
			  outBreak = targetBreakVal + 5;
			  send_breakmot(outBreak);
			  setTO(hit_holdon_to);
			  hit_holdon_step = 0x31;
			  break;
		  case 0x31:		   
			  if(checkTO(hit_holdon_to, stHitTorqVal.midInterval))
			  {
			  	   
				   

				   
			  	  execHit_Holdon_BldcStart();
			  	  hit_holdon_step = 0x32;			   
			  }
			  break;

			 
		  case 0x32:
		  	execHit_Holdon_BldcControl();
		  	if(exeFlag_holonBldcTokTokEnd)
		  	{
		  		exeFlag_holonBldcTokTokEnd = 0;
				hit_holdon_step = 0x40;			 
			}
		  	break;
	
		   
		  case 0x40:		   
			  torqMotor.setValue(reqTorqueMotor);
			  send_breakmot(outBreakVal);
			  setTO(hit_holdon_to);
			  hit_holdon_step = 0x41;
			  break;
		  case 0x41:		   
			  if(checkTO(hit_holdon_to, HIT_WAIT_TIME))
			  {
				  hit_holdon_step = 0x90;
			  }
			  break;
	
		   
		  case 0x90:		   
		   
			  exeResist = 0;	   
			  hit_holdon_step = 0x00;		   
			  old_hit_holdon_step = 0x00;
			  holdonCnt = 0;
			  break;
	
		  default:
			  break;
	  }
	  
	  if( exeRanding || exeHoldOff || exeSuccessFail || exeContinue || exeGameOver)	   
	  {
		  bbnMotor.offBldc();
		  torqMotor.setValue(reqTorqueMotor);
		  send2resp_FwSelfControlCmd(STX_TORQUE_MOTOR, reqTorqueMotor);

		  send_breakmot(outBreakVal);
		  
		  exeResist = 0;	   
		  exeHoldon = 0;	   
		  exeHoldOff = 0;
		  exeFlag_holonBldcTokTokEnd = 0;
		  hit_holdon_step = 0x00;
		  old_hit_holdon_step = 0x00;
		  holdonCnt = 0;
	  }
	

}

 

unsigned int hit_brk_step = 0x00;

void execHit_SetBreakOnceStart()
{
	 
	if (fishTypeFixENB)	targetBreakVal = stBreakVal[breakType].val[fishTypeFix];
	else				targetBreakVal = stBreakVal[breakType].val[fishTypeAuto];
	send_breakmot(targetBreakVal);

	outBreakVal = targetBreakVal;	 
	decBreakCnt = 0;				 

	 
	hit_brk_step = 0x10;			 
}

void send_breakmot(int _val)
{
	String str = "000";

	
	if(ROD_BOARD_V1 == rodBoardType)	 
	{
		if( 255 < _val)	_val = 255;		 
	
		if ( 10 > _val)	      str = "00" + String(_val);
		else if ( 100 > _val) str = "0" + String(_val);
		else                  str = String(_val);
	
		 
	
		eNow.write(DF_Protocol_MainToRod_Break, str);	   
	

	}
  
}


 
 
 
void execHit_BreakStop()
{
	hit_brk_step = 0x00;	 
}

void execHit_BreakStart()
{
	hit_brk_step = 0x10;
}

 
 
 
 
void execHit_BreakControl()
{
	
	static unsigned long _to = 0;

	switch(hit_brk_step)
	{

		case 0x00:
			break;
		 
		case 0x10:
			send_breakmot(targetBreakVal-5);
			setTO(_to);
			hit_brk_step = 0x11;	   
			break;

		case 0x11:		   
			if(checkTO(_to, 4000))	   
			{
				hit_brk_step = 0x20;	 
			}
			break;

		case 0x20:
			send_breakmot(targetBreakVal+5);
			setTO(_to);
			hit_brk_step = 0x21;	   
			break;

		case 0x21:		   
			if(checkTO(_to, 4000))	   
			{
				hit_brk_step = 0x10;	 
			}
			break;

		default:
			break;
	}

}

 
 
 
void execHit_BreakDecStop()
{
	hit_brk_step = 0x00;	 
	send_breakmot(0);
}

void execHit_BreakDecStart()
{
	hit_brk_step = 0x10;	 
}

void execHit_BreakDecControl()
{
	
	static unsigned long _to = 0;

	switch(hit_brk_step)
	{

		case 0x00:		 
			break;

		case 0x10:
			outBreakVal -= 10;				 
			send_breakmot(outBreakVal);
			setTO(_to);
			hit_brk_step = 0x11;	   
			break;

		case 0x11:		   
			if(checkTO(_to, (1000)))	   
			{
				decBreakCnt++;
				if( 19 < decBreakCnt)	 
				{
					hit_brk_step = 0x20;	 
				}
				else
				{
					hit_brk_step = 0x10;	 
				}
			}
			break;

		case 0x20:		 
			send_breakmot(outBreakVal);
			setTO(_to);
			hit_brk_step = 0x21;	   
			break;
			
		case 0x21:		   
			if(checkTO(_to, 10000))	   
			{
				hit_brk_step = 0x20;	 
			}
			break;

		default:
			break;
	}

}

 
 
 
void execFastHold_SetVal()
{

}

unsigned int fastHoldStep =0;
void execFastHold_Start()
{
	fastHoldStep = 10;
}
 
 
 

void execFastHold_Control()
{

	static unsigned int oldfastHoldStep =0;
	static unsigned long _to = 0;

		

	switch(fastHoldStep)
	{

		case 0:		 
			break;

		case 10:	 
			break;
			
		default:	 
			 
			break;
	}
	
	if( exeHoldOff || exeFastHoldOff || exeSuccessFail || exeContinue || exeGameOver)	 
	{
		exeFastHoldOn = 0;		 
		exeFastHoldOff = 0;
		fastHoldStep = 0;	 
		oldfastHoldStep = 0;	 
	}

}

 
 
 
void execButtonCombo_SetVal()
{

}

unsigned int btnComboStep =0;
void execButtonCombo_Start()
{
	btnComboStep = 10;
}
 
 
 

void execButtonCombo_Control()
{

	static unsigned int oldbtnComboStep =0;
	static unsigned long _to = 0;

		

	switch(btnComboStep)
	{

		case 0:		 
			break;

		case 10:	 
			break;
			
		default:	 
			 
			break;
	}
	
	if( exeButtonComboOff || exeSuccessFail || exeContinue || exeGameOver)	 
	{
		exeButtonCombo = 0;		 
		btnComboStep = 0;	 
		oldbtnComboStep = 0;
	}

}


 
 
 
void execRanding_SetVal()
{

}

unsigned int randingStep = 0;
static unsigned int randingCnt = 0;

void execRanding_Start()
{
	randingStep = 10;
}
 
 
 

void execRanding_Control()
{

	static unsigned int oldrandingStep =0;
	static unsigned long _to = 0;

		

	switch(randingStep)
	{

		case 0:		 
			break;

		case 10:	 
			setTO(_to);
			randingStep = 20;
			break;
			
		case 20:	 
			bbnMotor.onBldc(BBN_MOT_CW, sBbnActTbl[BBNACT_RANDING].onDuty);  
			setTO(_to);
			randingStep = 30;
			break;
		case 30:
			if(checkTO(_to, sBbnActTbl[BBNACT_RANDING].onTime))
			{
				bbnMotor.offBldc(); 			 
				setTO(_to);
				randingStep = 40;
			}
			break;
		case 40:	 
			if(checkTO(_to, sBbnActTbl[BBNACT_RANDING].offTime))
			{
				randingCnt++;
				if( bbnReqCnt[BBNACT_RANDING] < randingCnt)
				{
					randingStep = 70;		 
				}
				else
				{
					randingStep = 20;			 
				}
			}
			break;

		case 50:	 
			break;

		case 60:	 
			break;
			

		case 70:	 
			bbnMotor.offBldc(); 			 
			torqDefault();			 
			 
			randingCnt = 0;
			randingStep = 0x00;
			break;
						break;
			
		default:	 
			 
			break;
	}
	
	if( exeSuccessFail || exeContinue || exeGameOver)	 
	{
		exeRanding = 0;		 
		randingCnt = 0;
		randingStep = 0;	 
		oldrandingStep = 0;	 
		torqDefault();
		bbnMotor.offBldc(); 			 
	}

}

unsigned int exeTo1secLog = 0;
static unsigned int setLogToStep = 0;

 
 
void setAfter1sec_LogOut_Start()
{
	exeTo1secLog = 1;		 
	setLogToStep = 10;		 
}

 
 
void setAfterTo_LogOut_Control()
{
	static unsigned int _to;
	
	switch(setLogToStep)
	{
		case 0:			 
			break;

		case 10:	 
			setTO(_to);
			setLogToStep = 11;
			break;
		case 11:
			if(checkTO(_to, 1000))
			{
				Log_Control_Step();
				setLogToStep = 90;
			}
			break;

		case 90:	 
			exeTo1secLog = 0;	 
			setLogToStep = 0;	 
			break;

		default:
			break;
	}
}

 
 
 
void execSuccessFail()
{
	 
	clear_ExecFlag();

	 
	execContinue2Ready = 0;
	motor_AllDefault();		 

	 
	Log_Control_Step();
	setAfter1sec_LogOut_Start();

}

 
 
void execPointUp()
{
	motor_AllDefault();		 
}

void execEnd()
{
}


 
 
 
 
