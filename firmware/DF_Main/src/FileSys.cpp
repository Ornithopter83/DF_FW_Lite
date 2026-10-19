 
 
 

#include "Common.h"
#include "FileSys.h"

 
extern void esp_soft_reset();
 
extern unsigned long curr_ms_tick;

 

 
 
 
static char *ptrMANUFAC_FILE = "/manufactInfo.txt";
static char *ptrINSTALL_FILE = "/installInfo.txt";
static char *ptrMAINMOT_FILE = "/mainMotInfo.txt";
static char *ptrBLDC_MOT_FILE = "/bldcMotInfo.txt";
static char *ptrBOOTINFO_FILE = "/bootingInfo.txt";			 


 
FileSys::FileSys()
{

}

 


void FileSys::init()
{

	 
	if (!SPIFFS.begin())
	{
		SPIFFS.format();			 
	
		 
		 
		esp_soft_reset();
	}

  
}


 







String FileSys::getInfo(short fsKind)
{

	String strReturn;
	char *ptrFileName;

	switch(fsKind)
	{
		case MANUF_INFO :
			ptrFileName = ptrMANUFAC_FILE;
			strReturn = NO_FILE_MANUF_INFO;		 
			break;
		case INSTALL_INFO:
			ptrFileName = ptrINSTALL_FILE;
			strReturn = NO_FILE_INSTALL_INFO;		 
			break;

		case MAINMOT_INFO:
			ptrFileName = ptrMAINMOT_FILE;
			strReturn = NO_FILE_FS_INFO;	 
			break;

		case BLDC_LIMIT_INFO:
			ptrFileName = ptrBLDC_MOT_FILE;
			strReturn = NO_FILE_FS_INFO;	 
			break;

		case BOOTING_INFO:									 
			ptrFileName = ptrBOOTINFO_FILE;
			strReturn = NO_FILE_FS_INFO;	 
			break;

		default:
			return PARA_NG_FS_INFO;		 
			break;
	}

	 
	File file = SPIFFS.open(ptrFileName, "r");
	
	 
	if (!file || file.isDirectory())
	{
		return strReturn;	 
	}

	 
	if(file.available())	 
	{
		strReturn = "";
		while (file.available())
		{
  			strReturn += (char)file.read();	 
		}
	}
	else					 
	{
		strReturn = NO_VAL_FS_INFO;	 
	}
	 
	
	if(MAINMOT_INFO == fsKind)		 
	{
		if(false == isNumeric(strReturn))
		{
			strReturn = NO_VAL_FS_INFO;	 
		}
	}
  

  file.close();
  return strReturn;
}

 
 










bool FileSys::isNumeric(String str)
{
  for (size_t i = 0; i < str.length(); i++)
  {
    if (!isDigit(str.charAt(i)))
	{
      return false;
    }
  }
  return true;
}

 


short FileSys::saveInfo(short fsKind, String _strInfo)
{
	short length = 0;
	char *ptrFileName;

	switch(fsKind)
	{
		case MANUF_INFO:
			ptrFileName = ptrMANUFAC_FILE;
			break;

		case INSTALL_INFO:
			ptrFileName = ptrINSTALL_FILE;
			break;

		case MAINMOT_INFO:
			ptrFileName = ptrMAINMOT_FILE;
			break;
		
		case BLDC_LIMIT_INFO:
			ptrFileName = ptrBLDC_MOT_FILE;
			break;
		
		case BOOTING_INFO:									 
			ptrFileName = ptrBOOTINFO_FILE;
			break;
		
		default:
 
			return (NUM_PARA_NG_FS_INFO);		 
			break;
	
	}


	File file = SPIFFS.open(ptrFileName, "w");
	 
	if (!file || file.isDirectory())
	{
 
		  return (NUM_OPEN_FAIL_FS_INFO);			 
	}

	 
	length = file.print(_strInfo.c_str());		 

	file.close();
	return length;
}

 






short FileSys::deleteInfo(short fsKind)
{
	 

	char *ptrFileName;

	switch(fsKind)
	{
		case MANUF_INFO:
			ptrFileName = ptrMANUFAC_FILE;
			break;

		case INSTALL_INFO:
			ptrFileName = ptrINSTALL_FILE;
			break;

		case MAINMOT_INFO:
			ptrFileName = ptrMAINMOT_FILE;
			break;

		case BLDC_LIMIT_INFO:
			ptrFileName = ptrBLDC_MOT_FILE;
			break;

		case BOOTING_INFO:									 
			ptrFileName = ptrBOOTINFO_FILE;
			break;

		default:
			return NUM_PARA_NG_FS_INFO;	 
			break;
				
	}
	return (SPIFFS.remove(ptrFileName)? 0:-1);
}

 
































 
 
 


