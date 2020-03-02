#ifndef LCD_PLUS_H
#define LCD_PLUS_H

#include <LiquidCrystal.h>
#include <Arduino.h>

class LCDPlus :public LiquidCrystal
{
public:
	LCDPlus(int rs, int rw, int e, int d4, int d5, int d6, int d7, int brightness_ctrl) //modified constructor - extra brightness control pin 
		:LiquidCrystal(rs, rw, e, d4, d5, d6, d7),
		brightnessPin(brightness_ctrl)
	{
		pinMode(brightnessPin, OUTPUT);
	}

	void SetBrightness(int brightness);
private:
	int brightnessPin;        //brightness pin
};


#endif
