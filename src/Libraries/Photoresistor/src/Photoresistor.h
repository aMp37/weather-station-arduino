#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#include "Arduino.h"

class Photoresistor{

public:
	
Photoresistor(int analogPin);

void Measure();	//measure method

void SetPin(int pin);	//pin setter
int GetRawValue();	//raw measure read
int GetFixedValue();	//value for brightness controller
private:

int analogPin;
int rawValue;

};


#endif
