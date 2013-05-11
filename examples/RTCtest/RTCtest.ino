/**
 \file RTCtest.ino
 \brief Basic test file for the RTC class.
 \details This basic examples sets the date and time on the main clock and then
 loops reading the new date and time. Data are printed on the serial monitor.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning The author is not responsible for any damage or... 
 caused by this software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/

#include <RTC.h>
#include <Wire.h>
#include <Serial.h>

#define MPF_PIN 0

const static char *Days[] = {
  "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

uint8_t year, month, date, day, hours, minutes, seconds;
RTC rtc(MPF_PIN);

void setup()
{
	Serial.begin(9600);
	
	rtc.setTime( RTC::RTC_MAIN, "smh", 0, 0, 12 );
	rtc.setDate( RTC::RTC_MAIN, "ymnd", 12, 12, 30, 7 );
	delay(100);  // wait a while
}

void loop(){
  
	rtc.getDate( RTC::RTC_MAIN, "ymnd", &year, &month, &date, &day);
	rtc.getTime( RTC::RTC_MAIN, "hms", &hours, &minutes, &seconds);
	Serial.print(Days[day]);
	Serial.print(",");
	Serial.print(date);
	Serial.print(" ");
	Serial.print(month);
	Serial.print(" ");
	Serial.print("20");    //year beginning with 20xx
	Serial.print(year);
	Serial.print(" at ");
	Serial.print(hours);
	Serial.print(":");
	Serial.print(minutes);
	Serial.print(":");
	Serial.print(seconds);
	Serial.println();
    
	delay(1000); // wait 1 second
}
