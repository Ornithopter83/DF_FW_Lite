 

#pragma once

#include "arduino.h"

#define DIVISION_VAL  2
#define MIN_COUNT   -40
#define MAX_COUNT   40

#define ENC_MUL_V2	8	 
#define ENC_DIV_V2	12			 
#define MAX_COUNT_V2  ((MAX_COUNT * ENC_DIV_V2 ) / ENC_MUL_V2)  

 
#define ENC_MUL_V3	8	 
#define ENC_DIV_V3	6			 
#define MAX_COUNT_V3  ((MAX_COUNT * ENC_DIV_V3 ) / ENC_MUL_V3)  
#define ENC_MUL_VX		ENC_MUL_V3
#define ENC_DIV_VX		ENC_DIV_V3
#define MAX_COUNT_VX	MAX_COUNT_V3

enum
{
	DF_Rod_Encoder_CalibrationTargetStepCount = 20,
	DF_Rod_Encoder_CalibrationMaxPulseCount = 100000
};

class Encoder
{
  public:
    Encoder();

    void init();
    void rotate();

    void setRotateCallback(void (*rotateCallback)(String));
	void setCalibrationCallback(void (*calibrationCallback)(String));
	void beginCalibration();
	void completeCalibration();
	void applyCalibration();
	void cancelCalibration();
	void queryCalibration();
	String fillZero2char(int _count);
	int count;
	unsigned long calibrationRawPulseCount;
	unsigned int calibratedPulsesPerRevolution;
	bool calibrationActive;
	bool calibrationAwaitingApply;
	bool calibrationValid;

  private:
    int lastCLK;
    int lastDT;
    int currentCLK;
    int currentDT;
    String oldStr;
	unsigned long pendingCorrectedOutputCount;
	unsigned int correctedOutputStepCount;
	bool calibrationStorageError;

    String sendCount();
	void sendPendingCorrectedStep();
	void sendCalibrationStatus(char state);
	bool saveCalibration(unsigned long pulseCount);
	bool loadCalibration();
};


extern unsigned long curr_ms_tick;
 
extern int getEncType();

