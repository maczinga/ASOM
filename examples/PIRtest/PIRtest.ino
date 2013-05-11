
/****************************************************************************
   PIRsensor : test program for PIR sensor module

   Author: Enrico Formenti
   Permissions: MIT licence
   
   Remarks:
    - OUT pin is connected to digital pin 2 of Arduino, change this if needed
    - DELAY times depend on the type of module and/or its configuration.
*****************************************************************************/

#include <Serial.h>
#include <PIR.h>

// OUT pin on PIR sensor connected to digital pin 2
// (any other digital pin will do, just change the value below)
#define PIRSensorPin 2

PIR myPIR(PIRSensorPin);

void setup() {
  myPIR.begin();
}

void loop() {

  if(myPIR.getStatus()) {
    Serial.println("Movement detected");
    
    // do something else at least for the delay between two seccessive
    // readings
    delay(myPIR.getDurationDelay());
  }
  else
    Serial.println("Nothing being detected...");
}
