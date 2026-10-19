 
 
 
#include "Common.h"
#include "ENow.h"

static const char *ADDR_FILE = "/addr.txt";			 
uint8_t main_board_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broad_cast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;

ENow::ENow()
{

}

void ENow::fsFormat()
{
		
    	SPIFFS.format();	 
		
		
}
	
 


void ENow::bindTargetAddress()
{


	 
	 
	String addr = getTargetAddress();

	 
	 
	if( (16 < addr.length()) && (31 > addr.length()) )
	{
  		 
		char str[32];
		addr.toCharArray(str, addr.length() + 1);
		char* ptr;

		char* stPtr;
		int i = 0;

		 
		stPtr = strtok(str, ":");
		if(NULL != stPtr)
		{
			for (i = 0; i < 6; i++ )
			{
				 
				main_board_addr[i] = strtol( stPtr, &ptr, HEX );
		
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
	}
	else
	{
	}
	
 

}

 


void ENow::init(esp_now_recv_cb_t cb)
{
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
     
    LogPrintln(" LG] Error NOW INIT NG");
     





    return;
  }

   
  memcpy(peerInfo.peer_addr, main_board_addr, 6);
  peerInfo.channel = 0;			 
   
  peerInfo.encrypt = false;

   
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
     
    LogPrintln(" LG] Error NOW PEER NG");
	 





    return;
  }

   
  if( ESP_OK != esp_now_register_recv_cb(cb))
  {
	LogPrintln(" LG] Error NOW RCV_CB_REGIST NG");
  }


}


 


void ENow::write(String pid, String str)
{
   
   
   

  char msg[128];

  str = pid + str;			   
  

  strcpy(msg, str.c_str());
  int len = str.length();

	esp_err_t errRet = esp_now_send(main_board_addr, (uint8_t *) &msg, len);
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
	return ESP_OK == esp_now_send(main_board_addr, data, length);
}

 
 
void ENow::writeBC(String pid, String str)
{

   
   
   

  char msg[128];

  str = pid + str;			   
   

  strcpy(msg, str.c_str());
  int len = str.length();
  
	esp_err_t errRet = esp_now_send(broad_cast_addr, (uint8_t *) &msg, len);		 
		if(ESP_OK != errRet)
		{
			LogPrintln(" lg] Error now Send:" + pid +",m:" + str);
		 }
  
	 
	sendPid = pid;	 

	 
	 
	esp_err_t peerRet;
	esp_now_peer_num_t rodPeer;
	peerRet = esp_now_del_peer(broad_cast_addr);
	if (ESP_OK != peerRet)
	{
	  LogPrintln(" LG] ERROR nowPeer DEL: " + String(peerRet));
	}
	esp_now_get_peer_num(&rodPeer);
	LogPrintln(" LG] nowPR PeerNum:" + String(rodPeer.total_num));

}



 


String ENow::getMyAddress() {
  return WiFi.macAddress();
}

 


String ENow::getTargetAddress() {
	String addr = "FF:FF:FF:FF:FF:FF";
	 
	int seq;
	File file = SPIFFS.open(ADDR_FILE, "r");
	if (!file || file.isDirectory())
	{
		 
		 
  		return	addr;
	}

	 
    addr = "";
    while (file.available())
    {
      addr += (char)file.read();
    }
	file.close();
	return addr;
}

 


bool ENow::setTargetAddress(String addr)
{
	File file = SPIFFS.open(ADDR_FILE, "w");
	if (!file || file.isDirectory())
	{
		 
		 
		return false;
	}

	 
	bool rts = file.print(addr.c_str());
    file.close();

	return rts;
}

int ENow::addPeerAddress(uint8_t * prAdr)
{
	int ret;
	 
	memcpy(peerInfo.peer_addr, prAdr, 6);
	ret = esp_now_add_peer(&peerInfo);
	if (ESP_OK != ret)
	{
		LogPrintln(" LG] Error NOW PEER ADD NG" + String(ret));
	}
	
	 
	esp_now_peer_num_t rodPeer;
	esp_now_get_peer_num(&rodPeer);
	LogPrintln(" LG] nowPR PeerNum:" + String(rodPeer.total_num));
	
	return ret;
}

