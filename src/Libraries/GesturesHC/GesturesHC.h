#ifndef GESTURESHC_H
#define GESTURESHC_H

#include <Arduino.h>
#include <math.h>
#include "HC_SR04_Puchala.h"

class GesturesEngine {

private:
	HCSR04 * sensor;	//sensor pointer
	int state;			//state variable. Default state on start -> 0
	unsigned short lastDistance;	//last registered distance
	unsigned short scaledRange;
	unsigned short last_4_5_Time;

public:
	GesturesEngine(HCSR04* pSensor);	//constructor
	void updateState(unsigned short RTC_Second);					//method which updates state. Uses sensor object methods to communicate with sensor
	int getState();					//returns actual state variable value
	void resetState();		//sets state variable value
};



#endif // _GESTURESHC_H_
