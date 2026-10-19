 
#include "../DF_Main_Internal.h"
#include "DF_Main_LedControl.h"
void initSet_LedContVal(int Act)
{
	int i=0;

	 
	memset(ledCont, 0, sizeof(ledCont));
	for(i=0; i<LED_IDX_MAX; i++)
	{
		if (Act) {
			ledCont[i].oldcont = 	initledCont[i].oldcont;
			ledCont[i].cont = 		initledCont[i].cont;
		}
		ledCont[i].oldcont = 	initledCont[i].oldcont;
		ledCont[i].cont = 		initledCont[i].cont;
		ledCont[i].colorNo = 	initledCont[i].colorNo;
		ledCont[i].colorMany = 	initledCont[i].colorMany;
		ledCont[i].bTime = 		initledCont[i].bTime;
		ledCont[i].dTime = 		initledCont[i].dTime;
		ledCont[i].bStep = 		initledCont[i].bStep;
		ledCont[i].dStep = 		initledCont[i].dStep;
		ledCont[i].dColorCnt = 	initledCont[i].dColorCnt;
		ledCont[i].dReqCnt = 	initledCont[i].dReqCnt;
		ledCont[i].rsv_e = 		initledCont[i].rsv_e;
	}

	 
	for(i=LED_IDX_CENT; i<LED_IDX_MAX; i++)
	{
		switch(ledCont[i].cont)
		{
			case 0:  
				ledCont_OffStart(i);
				break;
			case 1:  
				ledCont_OnStart(i);
				break;
			case 2:  
				ledCont_BlinkStart(i);
				break;
			case 3:  
				ledCont_DimmStart(i);
				break;
		}
	}

	 
	ledControlMode = 1;
	 
	ledLight = 100;	 
	
}

 
void ledCont_OffStart(unsigned int pos)
{
	if(0==pos) { return;}
	
	 
	ledCont[pos].bStep = 0;
	ledCont[pos].dStep = 0;
	 
	extLed.off(pos+2);
}

void ledCont_OnStart(unsigned int pos)
{
	if(0==pos) { return;}
	
	 
	ledCont[pos].bStep = 0;
	ledCont[pos].dStep = 0;

	 
	if(LED_IDX_CENT == pos)
	{
		setRGBcolor(pos, (int)ledCont[pos].colorNo);
		extLed.RGBon(LEDPOS_CENT_RGB, sRGB[pos].r, sRGB[pos].g, sRGB[pos].b );	
	}
	else
	{
		extLed.on(pos+2, (int)LED_MAX_DUTY);
	}
	 
}

void ledCont_BlinkStart(unsigned int pos)
{
	if(0==pos) { return;}
	
	 
	ledCont[pos].dStep = 0;

	 
	setRGBcolor((int)pos, (int)ledCont[pos].colorNo );

	 
	ledCont[pos].bStep = 10;	 
}


 
 
 
void execledCont_Blink(int pos)
{
	static unsigned long _to[LED_IDX_MAX] ={0,0,0,0,0};
	static unsigned int _oldStep[LED_IDX_MAX] = {0,0,0,0,0};
	 

	switch(ledCont[pos].bStep)
	{
		case 0:	 
			break;
			
		case 10:	 
			 
			if(LED_IDX_CENT == pos)
			{
				extLed.RGBon((pos+2), sRGB[pos].r, sRGB[pos].g, sRGB[pos].b );
			}
			else
			{
				extLed.on((pos+2), (int)LED_MAX_DUTY);
			}

			setTO(_to[pos]);
			ledCont[pos].bStep = 11;	 
			break;
		case 11:			 
			if(checkTO(_to[pos], ledCont[pos].bTime))
			{
				ledCont[pos].bStep = 20;
			}
			break;
			
		case 20:		 
			extLed.off(pos+2);
			
			setTO(_to[pos]);
			ledCont[pos].bStep = 21;	 
			
			break;
			
		case 21:	 
			if(checkTO(_to[pos], ledCont[pos].bTime))
			{
				ledCont[pos].bStep = 10;
			}
			break;
			

		case 30:
			break;
		case 40:
			break;

		case 90:
			break;
			
		default:
			break;
	}
}

 
void ledCont_DimmStart(unsigned int pos)
{
	if(0==pos) { return;}
	
	 
	ledCont[pos].bStep = 0;

	 
	 
	if(7 > ledCont[pos].colorMany)
	{
		ledCont[pos].dColorCnt = COLOR_RED;				 
	}
	else
	{
		ledCont[pos].dColorCnt = COLOR_WHITE; 			 
	}
	ledCont[pos].dReqCnt = (ledCont[pos].dTime/LED_CONTROL_10MS/2);	 
	setRGBcolor((int)pos, (int)ledCont[pos].dColorCnt );

	 
	ledCont[pos].dStep = 10;	 
}


 
 
void execledCont_Dimm(int pos)
{
	static unsigned long _to[LED_IDX_MAX] ={ 0,0,0,0,0 };
	static unsigned int _oldStep[LED_IDX_MAX] = { 0,0,0,0,0 };
	static unsigned int _cnt[LED_IDX_MAX] = { 0,0,0,0,0 };
	 
	
	switch(ledCont[pos].dStep)
	{
		case 0:  
			break;
				
		case 10:	 
			if(LED_IDX_BTM == pos)
			{
				setRGBcolor(pos, COLOR_BLUE);	 
			}
			else
			{
				setRGBcolor(pos, ledCont[pos].dColorCnt);	 
				 
			}
			 
			_cnt[pos] = 0;
			ledCont[pos].dStep = 20;
			 
			break;
			
		case 20:		 
			_cnt[pos]++;
			if(_cnt[pos] > ledCont[pos].dReqCnt)
			{
				extLed.RGBon((pos+2), sRGB[pos].r, sRGB[pos].g, sRGB[pos].b);	 
				setTO(_to[pos]);
				ledCont[pos].dStep = 21;
			}
			else
			{
				outRGB[pos].r = (sRGB[pos].r) * _cnt[pos] / ledCont[pos].dReqCnt;
				outRGB[pos].g = (sRGB[pos].g) * _cnt[pos] / ledCont[pos].dReqCnt;
				outRGB[pos].b = (sRGB[pos].b) * _cnt[pos] / ledCont[pos].dReqCnt;
				extLed.RGBon((pos+2), outRGB[pos].r, outRGB[pos].g, outRGB[pos].b);
			}
			break;
		case 21:
			if(checkTO(_to[pos], ledDimmHoldTime[pos]))		 
			{
				ledCont[pos].dStep = 30;
			}
			break;

		case 30:		 
			_cnt[pos]--;
			if(_cnt[pos] <= 0)		 
			{
				extLed.RGBon((pos+2), 0,0,0);	 
				setTO(_to[pos]);
				ledCont[pos].dStep = 31;
			}
			else
			{
				outRGB[pos].r=(sRGB[pos].r) * _cnt[pos] / ledCont[pos].dReqCnt;
				outRGB[pos].g=(sRGB[pos].g) * _cnt[pos] / ledCont[pos].dReqCnt;
				outRGB[pos].b=(sRGB[pos].b) * _cnt[pos] / ledCont[pos].dReqCnt;
				extLed.RGBon((pos+2), outRGB[pos].r, outRGB[pos].g, outRGB[pos].b);
			}
			break;
		case 31:
			if(checkTO(_to[pos], ledDimmHoldTime[pos]))		 
			{
				ledCont[pos].dStep = 40;
			}
			break;

		case 40:
			ledCont[pos].dColorCnt++;
			if(ledCont[pos].dColorCnt > ledCont[pos].colorMany)
			{
				if(7 > ledCont[pos].colorMany)
				{
					ledCont[pos].dColorCnt = COLOR_RED;
				}
				else
				{
					ledCont[pos].dColorCnt = COLOR_WHITE;
				}
			}
			else
			{
			}
			ledCont[pos].dStep = 10;
			break;

		case 50:
			break;
			
		default:
			break;
	}

}

 
 

unsigned int ext_led_dim_step = 10;
static unsigned int old_ext_led_dim_step = 0x00;
 
static unsigned long tot_time = 0;
unsigned int cnt = 0;
unsigned int reqCnt = 0;
static unsigned int loopCnt = 0;
static unsigned int outCnt = 0;

static unsigned int bDuty = (LED_MAX_DUTY*10);
static unsigned int updownStep = (1*10);

 
 
 
void old_execCentLed_Dimming_Control(int color, int sec)
{

	 

	switch(ext_led_dim_step)
	{
		case 0:	 
			break;
			
		case 10:	 
			reqCnt = (sec * 1000) / 2 / 10;	 
			loopCnt = (reqCnt+128)/256;  
			if(1 > loopCnt)
			{
				loopCnt = 1;
			}
			
			if(256 > reqCnt)
			{
				updownStep = (256*10)/reqCnt;
			}
			else
			{
				updownStep = (1*10);
			}
			
			bDuty = (0*10);

			 

			ledOut(LED_CENT_R_PIN, 0);
			ledOut(LED_CENT_G_PIN, 0);
			ledOut(LED_CENT_B_PIN, bDuty);	 
			ext_led_dim_step = 20;
			break;
			
		case 20:		 
			bDuty += updownStep ;
			if((LED_MAX_DUTY*10) < bDuty)		 
			{
				outCnt = 0;
				bDuty = 0;
				ext_led_dim_step = 29;
			}
			else
			{
				ext_led_dim_step = 21;
			}
			ledOut(LED_CENT_B_PIN, (bDuty/10));  
			break;
			
		case 21:
			outCnt++;
			if( !(outCnt % loopCnt) )
			{
				if(outCnt > reqCnt)			 
				{
					outCnt = 0;
					if( LED_MAX_DUTY*10 < bDuty) bDuty = (LED_MAX_DUTY*10);
					ledOut(LED_CENT_B_PIN, (bDuty/10));	 
					ext_led_dim_step = 29;
				}
				else
				{
					ext_led_dim_step = 20;
				}
			}
			break;
			
		case 29:
			outCnt++;
			if(outCnt > ledoffHoldTimeCnt)
			{
				if( LED_MAX_DUTY*10 < bDuty) bDuty = (LED_MAX_DUTY*10);
				outCnt = 0;
				ext_led_dim_step = 30;
			}
			break;
			
		case 30:		 
			bDuty -= updownStep;
			if((0*10) > bDuty)		 
			{
				bDuty = (0*10);
				outCnt = 0;
				ext_led_dim_step = 32;		 
			}
			else
			{
				ext_led_dim_step = 31;
			}
			ledOut(LED_CENT_B_PIN, (bDuty/10));	 
			break;
			
		case 31:
			outCnt++;
			if( !(outCnt % loopCnt) )
			{
				if(outCnt > reqCnt)		 
				{
					bDuty = (0*10);
					outCnt = 0;
					ext_led_dim_step = 32;	 
				}
				else
				{
					ext_led_dim_step = 30;
				}
			}
			break;

		case 32:		 
			outCnt++;
			if(outCnt > ledoffHoldTimeCnt)
			{
				if( 0*10 > bDuty) bDuty = (0*10);
				outCnt = 0;
				ext_led_dim_step = 20;
			}
			break;
			
		default:
			break;
	}
}



 
void extLed_Off(unsigned int idx)
{
	switch(idx)
	{
		case LED_IDX_CENT:
		case LED_IDX_BTM:
			extLed.off((idx+2));
			break;

		case LED_IDX_LEFT:
		case LED_IDX_RIGHT:
			break;
		
		default:
			break;
	}
}

 
 
void extLed_On(unsigned int idx)
{
	unsigned int color;
	
	switch(idx)
	{
		case LED_IDX_CENT:
			 
			color = ledCont[idx].colorNo;
			if(COLOR_MAGENTA < color) { color = COLOR_WHITE; }	 
			setRGBcolor((int)idx, (int)color);
			extLed.RGBon((idx+2), sRGB[idx].r, sRGB[idx].g, sRGB[idx].b);
			break;
		
		case LED_IDX_BTM:
			extLed.on((idx+2), (int)LED_MAX_DUTY);	 
			break;
		
		case LED_IDX_LEFT:
		case LED_IDX_RIGHT:
			break;
			
		default:
			break;
	}
}

 













 
 
 
void extLed_Control() 
{
	unsigned int pos=0;
	 
	if(PWR_ON == pwrMode)
	{
		
		 
		if(ledControlMode)
		{

			for(pos=LED_IDX_CENT; pos<LED_IDX_MAX; pos++)
			{
				if(LED_CON_BLINK == ledCont[pos].cont)		 
				{
					 
					execledCont_Blink(pos);
				}
				else if(LED_CON_DIMM == ledCont[pos].cont)	 
				{
					 
					{
						 
						execledCont_Dimm(pos);		 
					}
				}
				else	 
				{
					if(ledCont[pos].oldcont != ledCont[pos].cont)
					{
						if(LED_CON_OFF == ledCont[pos].cont)	 
						{
							extLed_Off(pos);
						}
						else if(LED_CON_ON == ledCont[pos].cont)	 
						{
							extLed_On(pos);
						}
						ledCont[pos].oldcont = ledCont[pos].cont;
					}
					else
					{
						 
					}
				}
			}
		}
	}

}

 
 
 
 

 
