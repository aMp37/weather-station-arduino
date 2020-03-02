#include "GesturesHC.h"

GesturesEngine::GesturesEngine(HCSR04* pSensor)
{
	this->sensor = pSensor;
	state = 0;
	lastDistance = 120;
	scaledRange = 0;
}






void GesturesEngine::updateState(unsigned short RTC_SEC)	//state machine implementation
{
		sensor->measureDistance();
		scaledRange = ((int)sensor->getDistance() / 2)*2;

		if (scaledRange < 100)
		{
			if (state == 0)	//if first good measure
			{
				if (abs(RTC_SEC - last_4_5_Time) > 1)	//wait for idle time pass
				{
					if (scaledRange < 40 && scaledRange>15)
						state++;			//go to state 1
				}			
			}
			else if (state == 1)
			{
				if (scaledRange > lastDistance)
					state += 2;	//go to state 3
				else if (scaledRange < lastDistance)
					state++;	//go to state 2
			}
			else if (state == 2)
			{
				if (scaledRange > lastDistance)
					state = 0;	//back to zero state
				else if (scaledRange < lastDistance)
				{
					state += 2;	//go to state 4
					last_4_5_Time = RTC_SEC;	//save time
				}
					
			}
			else if (state == 3)
			{
				if (scaledRange < lastDistance)
					state = 0;	//back to state 0
				else if (scaledRange > lastDistance)
				{
					state += 2;	//go to state 5
					last_4_5_Time = RTC_SEC;
				}
					

			}

			lastDistance = scaledRange;
		}
}

int GesturesEngine::getState()
{
	return state;
}

void GesturesEngine::resetState()
{
	state = 0;
	lastDistance = 110.0f;
}