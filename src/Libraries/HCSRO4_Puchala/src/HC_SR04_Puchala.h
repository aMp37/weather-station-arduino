#ifndef HC_SR04_PUCHALA_H
#define HC_SR04_PUCHALA_H

#include "Arduino.h"

class HCSR04 {
public:
	HCSR04(short echoPin, short trigPin);	//echoPin - pin echo, trigPin - pin Trig


	void setupSensor();	//sensor setup method
	void measureDistance(); //distance measure method
	float getDistance();	//return last measured distance


private:
	short ECHO_PIN;	//PIN-s fields
	short TRIG_PIN;
	
	long duration;	//raw value from sensor
	float distance;	//computed value of distance
};


#endif
