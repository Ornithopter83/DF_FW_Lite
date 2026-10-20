 
#include "../DF_Rod_Internal.h"
#include "DF_Rod_PowerAndImu.h"
void rodAliveRecvTOCheck()
{
	 
	 
	 
    if (RUN_TIMEOUT_ALIVE_CNT < rod_Alive_recv_over_cnt)		 
	{
		 
		rodAliveRcv_LedBlinkFlag = 0;		 
		if (MAIN_STS_DISCONN != mainStatus)
		{
			rodVrtControl_Stop();
			digitalWrite(VRT_MOT_ON_PIN, VRT_MOT_OFF);
		}
		mainStatus = MAIN_STS_DISCONN;
    }
	else
	{
		 
  		rod_Alive_recv_over_cnt++;
	}
}

 
 
 
 
void sleepEnteranceCheck()
{
		 
	if (RUN_TIMEOUT_SLEEP_CNT < run_time_sleep_over_cnt )	 
	{
	   














	}

}

 



void imuConnectCheck()
{
	String respMsg;

	 
	 
	if(fRecvImuData)		 
	{
		fRecvImuData = 0;		 
		imuPollingTOcnt = 0;			 
		 
		respMsg = "11";
		imuStatus = IMU_CONN;
	}
	else			 
	{
		imuPollingTOcnt++;
		if( IMU_CONN_TIMEOUT_CNT < imuPollingTOcnt)  
		{
			imuPollingTOcnt = IMU_CONN_TIMEOUT_CNT+1;
			 
			respMsg = "00";
			imuStatus = IMU_DISCONN;
		}
	}
	
	 
	if(oldimuStatus != imuStatus)
	{
		eNow.write(DF_Protocol_RodToMain_ImuConnection, respMsg);		 
			LogPrintln(" LG] IMUCON " + respMsg + " o,c:" + String(oldimuStatus)+","+String(imuStatus));		 
		
		oldimuStatus = imuStatus;
	}

}


 

