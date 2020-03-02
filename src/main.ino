
#include <AM2320_asukiaaa.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <GesturesHC.h>
#include <Buzzer.h>
#include <HC_SR04_Puchala.h>
#include <DS1307RTC.h>
#include "Photoresistor.h"
#include "LCD_Plus.h"

enum eIcons{        //Icons enum class
  GEST,
  TEMP,
  HUM,
  LOW_TEMP,
  HIGH_TEMP,
  GRAD
  };



//	********** objects init ***************

LCDPlus lcd(13, 12, 11, 5, 4, 3, 2,6);		//LCD display
Buzzer systemBuzzer(7);						//Buzzer object for easy buzzer usage

AM2320_asukiaaa tempHumSensor;				//AM2320 sensor
TwoWire tw1;								//TwoWire I2C bus object for AM2320 sensor
HCSR04 usSensor(8, 9);						//Ultrasonic HC-SR04 sensor object

tmElements_t timeObj;						//Time util object for easy RTC usage
GesturesEngine gst(&usSensor);				//Gesture engine object for easy gestures usage
Photoresistor brightnessPhotoresistor(A0);

//	********** variables declare ***************

bool RTC_Read_good;							//RTC read status variable
float minimumTemperature;					//minimal temperature read variable
float maximumTemperature;					//maximal temperature read variable
int backlightValue;							//backlight brightness value
short counterState;						//main menu counter state variable

tmElements_t minimumTime;					//minimal temperature read time util object
tmElements_t maximumTime;					//maximal temperature read time util object

long lastMiliseconds;    //last time read from internal ATMega clock

int lastHour; //last hour from RTC


// ** custom LCD characters arrays 

byte gestureIcon[] = {0x00, 0x1F ,0x11 ,0x0A ,0x04 ,0x04 ,0x04 ,0x00};
byte lowTemp[] = { 0x04, 0x0A,0x0A, 0x0A, 0x0A, 0x11, 0x11, 0x0E };
byte highTemp[] = { 0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x1F, 0x1F, 0x0E };
byte humidityIcon[] = { 0x04, 0x04, 0x0A, 0x0A, 0x11,  0x11, 0x11, 0x0E};
byte temperatureIcon[] = { 0x04, 0x0A, 0x0A, 0x0E, 0x0E, 0x1F, 0x1F, 0x0E};
byte grad[] = {  0x1C, 0x14, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {

  //***************** devices setup
  tw1.begin();          //init I2C bus
  tempHumSensor.setWire(&tw1);  //set AM2320 Bus
  usSensor.setupSensor();    //init HC-SR04 sensor

  //***************** variables init
  backlightValue = 255;

  lastMiliseconds = micros()/1000;
  minimumTemperature = 80;	//init temperature values
  maximumTemperature = -9;
  RTC.read(minimumTime);			//just init time variables
  RTC.read(maximumTime);

  RTC.read(timeObj);          //first time read
  lastHour = timeObj.Hour;
  
  RTC_Read_good = false;


  //***************** LCD init

  //set up LCD for first display 

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  
  //setup custom characters to use with display

  lcd.createChar(eIcons::GEST,gestureIcon);
  lcd.createChar(eIcons::TEMP,temperatureIcon);
  lcd.createChar(eIcons::HUM,humidityIcon);
  lcd.createChar(eIcons::GRAD,grad);
  lcd.createChar(eIcons::LOW_TEMP,lowTemp);
  lcd.createChar(eIcons::HIGH_TEMP,highTemp);
}

// function prints always two digits on LCD
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.print('0');
  }
  lcd.print(number);
}

String mothName(unsigned monthNumber){  //function convert month number to 3 first letter of polish month name
  
  switch(monthNumber){
    case 1: return "Sty";
    case 2: return "Lut"; 
    case 3: return "Mar"; 
    case 4: return "Kwi"; 
    case 5: return "Maj"; 
    case 6: return "Cze"; 
    case 7: return "Lip"; 
    case 8: return "Sie"; 
    case 9: return "Wrz"; 
    case 10: return "Paź"; 
    case 11: return "Lis"; 
    case 12: return "Gru"; 
    
   }
  
}

void loop() {

//AM2320 Temperature & Hudimity sensor read, 2 seconds interval between measurements

//  RTC read

  if(RTC.read(timeObj))                 //RTC read check
    RTC_Read_good = true;
  else
    RTC_Read_good = false;

  if(lastHour!=timeObj.Hour){           //hour change sound
      systemBuzzer.sound(NOTE_G6,100);
      lastHour=timeObj.Hour;
    }
        


  if(micros()/1000 - lastMiliseconds > 2000)  // sensor measure clock setup 
  {
    lastMiliseconds = micros()/1000;
    tempHumSensor.update();

        //Update temperature variables

        if(tempHumSensor.temperatureC < minimumTemperature)
        {
          minimumTemperature = tempHumSensor.temperatureC;
          RTC.read(minimumTime);
        }
        else if(tempHumSensor.temperatureC > maximumTemperature)
        {
          maximumTemperature = tempHumSensor.temperatureC;
          RTC.read(maximumTime);
        }
  }


//Gesture read, and GUI view set

  gst.updateState(timeObj.Second);
  
  if(gst.getState() == 4)               //GUI page counter 
    {
      if(counterState == 0)
        counterState = 2;
      else
        counterState--;

        
        systemBuzzer.sound(NOTE_A6,100);  //play sound if page change
        gst.resetState();

        lcd.clear();  // clear LCD
    }
    else if(gst.getState() == 5)
    {
        if(counterState == 2)
          counterState = 0;
        else
          counterState++;  
          
          systemBuzzer.sound(NOTE_A6,100);  //play sound if page change
          gst.resetState();

          lcd.clear();  //clear LCD
    }
    
  brightnessPhotoresistor.Measure();  //photoresistor value measure

  lcd.SetBrightness(brightnessPhotoresistor.GetFixedValue()); //set brightness


//LCD display

  if(usSensor.getDistance()<100.0f)	//print gesture character 
    {
      lcd.setCursor(15,1);
      lcd.write(byte(eIcons::GEST));
      lcd.setCursor(0,0);
    }
    else
    {
      lcd.setCursor(15,1);
      lcd.print(' ');
      lcd.setCursor(0,0);
    }

   
  if(counterState == 0)				//temperature and Hudimity real time read view
  {
    lcd.setCursor(0,0);
    lcd.write(byte(eIcons::TEMP));lcd.print(" ");lcd.print(tempHumSensor.temperatureC);lcd.print(' ');lcd.write(byte(eIcons::GRAD));lcd.print("C");


    
    lcd.setCursor(0,1);
    lcd.write(byte(eIcons::HUM));lcd.print(" ");lcd.print(tempHumSensor.humidity);lcd.print(" %");
  
  }
  else if(counterState == 1)		//min max temperature view
  {
    lcd.setCursor(0,0);
    lcd.write(byte(eIcons::HIGH_TEMP));lcd.print(" ");lcd.print(maximumTemperature);lcd.print(" ");print2digits(maximumTime.Hour);lcd.print(":");print2digits(maximumTime.Minute);
    lcd.setCursor(0,1);
    lcd.write(byte(eIcons::LOW_TEMP));lcd.print(" ");lcd.print(minimumTemperature);lcd.print(" ");print2digits(minimumTime.Hour);lcd.print(":");print2digits(minimumTime.Minute);
  }
  else							    // date and hour read
  {
    lcd.setCursor(0,0);

     if(RTC_Read_good)
     {
      print2digits(timeObj.Hour);
      lcd.print(':');
      print2digits(timeObj.Minute);
      lcd.print(':');
      print2digits(timeObj.Second);
    
      lcd.setCursor(0,1);
      lcd.print(timeObj.Day);
      lcd.print(' ');
      lcd.print( mothName(timeObj.Month));
      lcd.print(' ');
      lcd.print(tmYearToCalendar(timeObj.Year));
      }
      else 
      {
      lcd.print("RTC read failed! ");
      }    
  }
}
