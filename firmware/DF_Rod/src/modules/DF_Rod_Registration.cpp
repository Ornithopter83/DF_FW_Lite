 
#include "../DF_Rod_Internal.h"
#include "DF_Rod_Registration.h"
void t1ms_Process()
{
	 

}


 

void t10ms_Process()
{
	if(vrt_cont_flag) { rodVrtControl(); }			 
	if(btn_led_flag[0])	{ rodLfBtnLedControl();	}	 
	if(btn_led_flag[1])	{ rodRtBtnLedControl();	}	 
	
}

 
 
 
 
 
 
 
void sendRodInfo_forStep()
{


	switch(recvInfoSeqStep)
	{
		 
		 
		
		case 1:			 
			recvInfoSeqStep ++;
			break;
		
		case 2:		 
			recvInfoSeqStep++;
			imuConnResp();
				LogPrintln(" LG] IMUCON o,c:" + String(oldimuStatus)+","+String(imuStatus));
				 
			break;

		case 3:		 
			recvInfoSeqStep++;
			
			eNow.write(DF_Protocol_RodToMain_Battery, battery.restLvlStr);
				LogPrintln(" LG] BATLVL o,c:" + String(battery.rest_lvl)+","+String(battery.old_restLvl));
				 
			break;
			
		case 4:		 
			recvInfoSeqStep++;
			sendInfo_boardType();
			break;
			
		default:
			recvInfoSeqStep = 0;	 
			break;
	}

}

 
 
 

static unsigned int rodRegistToStep = 0;	 

 
void rodRegistToStop()
{
	rodRegistToStep = 0;	 
}

 
void rodRegistToStart()
{
	rodRegistToStep = 10;	 
}

void rodRegi3secToStart()
{
	rodRegistToStep = 30;	 
}


#define ROD_REGI_CANCEL_TO	(15*1000)	 
#define ROD_REGI_END_TO		(3500)	 
void rodRegistToControl()
{
	static unsigned int oldStep = 0;
	static unsigned long _to = 0;

	switch(rodRegistToStep)
	{
		case 0:
			break;
			
		case 10:	 
			setTO(_to);
			rodRegistToStep = 11;
			break;		
		case 11:
			if(checkTO(_to, ROD_REGI_CANCEL_TO))
			{
				 
				rodRegistCancel();		 
				rodRegistToStep = 0;		 
			}
			break;

		case 30:	 
			rodRegistToStep = 31;
			setTO(_to);
			break;
		case 31:
			if(checkTO(_to, ROD_REGI_END_TO))
			{
				rodRegistCancel();		 
				rodRegistToStep = 0;		 
				 
				ESP_Soft_Reset();
			}
			break;
			
		default:
			break;
	}
}

 
void rodRegistCancel()
{
	rodRegistMode = 0;
}


 
 
 
unsigned int btn_L_DownCnt = 0;
unsigned int AddrWrite_LedBlink = 0;


 


