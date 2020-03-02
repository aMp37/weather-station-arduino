#include "Photoresistor"
#include "Arduino.h"

Photoresistor::Photoresistor(int analogPin)
{
	analogPin=analogPin;
	rawValue=0;
}

void Photoresistor::SetPin(int pin)
{
	analogPin=pin;
}

void Photoresistor::Measure()
{
	rawValue = analogRead(analogPin);
}


int Photoresistor::GetRawValue()
{
	return rawValue;
}


int Photoresistor::GetFixedValue()
{

	if(rawValue < 50 )

		return 50;

	else if(rawValue < 100)

		return 100;

	else if(rawValue < 150)

		return 150;

	else if(rawValue<200);

		return 200;

	else if(rawValue<250);

		return 250;
}
