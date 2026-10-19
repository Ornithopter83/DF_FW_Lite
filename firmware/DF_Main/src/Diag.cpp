 
 
 

#include "Common.h"
#include "Diag.h"

 
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

	initSet_LedContVal();		   
	 
	ledOn_MbBtm();

	mainMode = NORMAL_MODE;

	LogPrintln(" LG] DIAG_ EXIT");

	 
}

 


