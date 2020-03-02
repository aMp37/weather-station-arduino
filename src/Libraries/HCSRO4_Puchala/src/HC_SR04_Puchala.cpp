#include "HC_SR04_Puchala.h"
#include "Arduino.h"


HCSR04::HCSR04(short echo, short trig)
{
	this->ECHO_PIN = echo;
	this->TRIG_PIN = trig;

	this->duration = 0L;
	this->distance = 0.0f;
}

void HCSR04::setupSensor()
{
	pinMode(ECHO_PIN, INPUT);
	pinMode(TRIG_PIN, OUTPUT);
}

void HCSR04::measureDistance()
{
	digitalWrite(TRIG_PIN, LOW);			//measurement cycle
	delayMicroseconds(2);

	digitalWrite(TRIG_PIN, HIGH);	
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);		
	duration = pulseIn(ECHO_PIN, HIGH);		//get pulses delta duration
	distance = duration * 0.034 / 2;		//compute value of duration (constants from WEB )
}

float HCSR04::getDistance()
{
	return distance;
}