/**
 * \file OWromFinder.ino
 * \brief Connect a single 1-Wire component. This program prints its ROM unique
 * id. We assume that the 1-wire pin of the component is connected to Arduino
 * 2. Change \c OWPIN macro if you prefer to connect it to another pin.
 * \author Enrico Formenti
 * \version 0.1
 * \date 2012-2013
 * \warning The author is not responsible for any damage or... 
 * caused by this software. Use it at your own risk.
 * \copyright BSD license. See license.txt for more details. 
 * All text above must be included in any redistribution. 
 */

#include <OWcomponent.h>
#include <Serial.h>

#define OWPIN 2

OWcomponent ow(OWPIN);

void setup()
{
  Serial.begin(9600);
}

void loop(){

  uint8_t ROM[8], i;

  ow.search(ROM, CMD_GENERIC_SEARCH);

  Serial.print("Family id: ");
  Serial.println(ROM[0]);

  Serial.print("Component id:");
  for(i=1;i<7;i++) {
    Serial.print(" ");
    Serial.print(ROM[i]);
  }
  Serial.println();

  Serial.print("CRC: ");
  Serial.println(ROM[7]);
}

