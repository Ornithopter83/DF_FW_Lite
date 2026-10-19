 
 
 

#pragma once

#include "arduino.h"

#include "SPIFFS.h"				 

 
#define NO_FILE_MANUF_INFO 		"999999,999999,9999-9999"
#define NO_FILE_INSTALL_INFO	"9999-9999"

 
#define NO_FILE_FS_INFO		"-11"	 
#define NUM_NO_FILE_FS_INFO		-11
#define NO_VAL_FS_INFO		"-12"	 
#define NUM_NO_VAL_FS_INFO		-12
#define PARA_NG_FS_INFO		"-13"	 
#define NUM_PARA_NG_FS_INFO		-13
#define OPEN_FAIL_FS_INFO		"-14"	 
#define NUM_OPEN_FAIL_FS_INFO	-14



class FileSys
{
  public:
    FileSys();
    void init();	 

    String getInfo(short fsKind);
    short saveInfo(short fsKind, String _strManufInfo);
	short deleteInfo(short fsKind);
	 
	bool isNumeric(String str);
	
  private:
     
	 
};


