#include "LCD_Plus.h"



void LCDPlus::SetBrightness(int brightness)
{
	analogWrite(brightnessPin, brightness);
}
