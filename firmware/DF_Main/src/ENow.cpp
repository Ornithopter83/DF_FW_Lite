 
 
 
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_pm.h>
#include <esp_sleep.h>

#include "Common.h"
#include "ENow.h"

 
extern void set_slave_status(unsigned int sts);

 

static const char *ADDR_FILE = "/addr.txt";
 

uint8_t slave_board_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	 
 

esp_now_peer_info_t peerInfo;

ENow::ENow()
{

}

 


void ENow::init(esp_now_recv_cb_t cbfunc)
{
	 
	 

  	 
	bindTargetAddress();		 

	 
	if(!isTargetAddressOk())
	{
  		 
  		 
  		LogPrintln(" lg] Error eNOW ADDR NG:" + strRodAddr);
  	}
 
	 
	   




	WiFi.mode(WIFI_AP_STA);

	WiFi.disconnect();

	 
	esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

	 
		 
	if (ESP_OK != esp_now_init())		 
	{
  		 
  		 
    	LogPrintln("(Err) ESP NOW Init Error.");
		 
		 





    	return;
	}

	 
	memcpy(peerInfo.peer_addr, slave_board_addr, 6);
	peerInfo.channel = 0;			 
	peerInfo.encrypt = false;		 
	if (ESP_OK != esp_now_add_peer(&peerInfo))
	{
  		 
  		 
		LogPrintln(" LG] PwrOn (Err) ESP NOW Add Peer Error.");

		 
		 





		return;
	}

	 
	 
	if(ESP_OK != esp_now_register_recv_cb(cbfunc))	 
	{
		LogPrintln(" LG] Error nowRecv CallBack Function");
	}
	LogPrintln(" LG] PwrOn eNOW INIT OK");



  
}

 


void ENow::bindTargetAddress()
{

	String addr = getTargetAddress();
	strRodAddr = addr;

	 
	if( (16 < addr.length()) && (31 > addr.length()) )
	{
		char str[32];
		addr.toCharArray(str, addr.length() + 1);			 
		char* ptr;

		char* stPtr;

		 
		stPtr = strtok(str, ":");		 
		if(NULL != stPtr)
		{
			for ( uint8_t i = 0; i < 6; i++ )
			{
				 
				slave_board_addr[i] = strtol( stPtr, &ptr, HEX );	 
				
				stPtr = strtok( NULL, ":");	 
				if(NULL == stPtr)
				{
					break;	 
				}
				else
				{
					 
				}
			}
  		}
		LogPrintln(" LG] T_ADR rod: " + String(slave_board_addr[0]) + "," + String(slave_board_addr[1]) + "," + String(slave_board_addr[2]) + ","+ String(slave_board_addr[3]) + ","+ String(slave_board_addr[4]) + ","+ String(slave_board_addr[5]) );
	}
	else
	{
		LogPrintln(" LG] T_ADR Load NG, addr= " + addr);
	}

}



 


 
void ENow::write(String pid, String str)
{
	char msg[128];
	String cmd = pid + str;			 
	if (static_cast<unsigned int>(cmd.length()) >= sizeof(msg))
	{
		LogPrintln(" LG] ESP-NOW payload too long");
		return;
	}
	cmd.toCharArray(msg, sizeof(msg));
	int len = cmd.length();

	 
	esp_err_t errRet = esp_now_send(slave_board_addr, (uint8_t *) &msg[0], len);
		if(ESP_OK != errRet)
		{
			LogPrintln(" lg] Error now Send:" + pid +",m:" + str);
		}

	
	 
	sendPid = pid;	 

}

bool ENow::writeRaw(const unsigned char *data, unsigned int length)
{
	if ((NULL == data) || (0 == length) || (250 < length))
	{
		return false;
	}
	return ESP_OK == esp_now_send(slave_board_addr, data, length);
}

void ENow::writeBC(String pid, String str)
{
   
   
   

  char msg[128];
  str = pid + str;			   
  if (static_cast<unsigned int>(str.length()) >= sizeof(msg))
  {
  	LogPrintln(" LG] ESP-NOW payload too long");
  	return;
  }
  str.toCharArray(msg, sizeof(msg));
  int len = str.length();

	esp_err_t errRet = esp_now_send(broad_cast_addr, (uint8_t *) &msg[0], len);
		if(ESP_OK != errRet)
		{
			LogPrintln(" lg] Error now Send:" + pid +",m:" + str);
		}


	 
	sendPid = pid;	 
}


 


String ENow::getMyAddress() {
  return WiFi.macAddress();
}

 


String ENow::getTargetAddress()
{
  String addr = "FF:FF:FF:FF:FF:FF";

  int seq, i;
  int length = 0;

  File file = SPIFFS.open(ADDR_FILE, "r");

   
  if (file && !file.isDirectory())
   
  {

	 
	 
    addr = "";
    while (file.available())
    {
      addr += (char)file.read();
    }
	 


  }

  file.close();
  
  return addr;
}

 


bool ENow::setTargetAddress(String addr)
{
	bool rts = true;
	File file = SPIFFS.open(ADDR_FILE, "w");
	 
	if (!file)
	{
		return false;
	}

	 
	rts = file.print(addr.c_str());		 

	file.close();

	return rts;
}

 


bool ENow::isTargetAddressOk()
{
  bool bRtn = false;			 
  for (int i = 0; i < 6; i++ )
  {
    if ( 0xFF != slave_board_addr[i])
	{
		bRtn = true;	 
		break;
	} 
  }
  return bRtn;
}

int ENow::addPeerAddress(uint8_t * prAdr)
{
	memcpy(peerInfo.peer_addr, prAdr, 6);
	peerInfo.channel = 0;		   
	peerInfo.encrypt = false;	   

	int ret;
	ret = esp_now_add_peer(&peerInfo);
	if (ESP_OK != ret)
	{
		LogPrintln(" LG] Error Func PEER ADD" + String(ret));
	}
	LogPrintln(" LG] NowPeer ADD OK");
	
	 
	esp_now_peer_num_t rodPeer;
	esp_now_get_peer_num(&rodPeer);
	LogPrintln(" LG] nowPR PeerNum:" + String(rodPeer.total_num));
	
	return ret;
}

