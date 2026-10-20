 
#include "../DF_Rod_Internal.h"
#include "DF_Rod_Registration.h"
void t1ms_Process()
{
	 

}


 

void t10ms_Process()
{
	if(verticalMotorControlFlag) { rodVerticalMotorControl(); }			 
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

 
 
 

static unsigned int rodRegistrationStep = 0;	 

 
void stopRodRegistration()
{
	rodRegistrationStep = 0;	 
}

 
void startRodRegistration()
{
	rodRegistrationStep = 10;	 
}

void rodRegi3secToStart()
{
	rodRegistrationStep = 30;	 
}


#define ROD_REGI_CANCEL_TO	(15*1000)	 
#define ROD_REGI_END_TO		(3500)	 
void processRodRegistration()
{
	static unsigned int oldStep = 0;
	static unsigned long _to = 0;

	switch(rodRegistrationStep)
	{
		case 0:
			break;
			
		case 10:	 
			setTO(_to);
			rodRegistrationStep = 11;
			break;		
		case 11:
			if(checkTO(_to, ROD_REGI_CANCEL_TO))
			{
				 
				cancelRodRegistration();		 
				rodRegistrationStep = 0;		 
			}
			break;

		case 30:	 
			rodRegistrationStep = 31;
			setTO(_to);
			break;
		case 31:
			if(checkTO(_to, ROD_REGI_END_TO))
			{
				cancelRodRegistration();		 
				rodRegistrationStep = 0;		 
				 
				ESP_Soft_Reset();
			}
			break;
			
		default:
			break;
	}
}

 
void cancelRodRegistration()
{
	rodRegistrationMode = 0;
}


 
 
 
unsigned int leftButtonDownCount = 0;
unsigned int AddrWrite_LedBlink = 0;


 


