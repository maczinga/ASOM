/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/

/**
 \file SensorConstants.h
 \brief Definition of constants for sensor components.
 \details Header file containing the definition of constants for sensor components.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/


#ifndef SENSORCONSTANTS_H
#define SENSORCONSTANTS_H


/* 
 
 SENSOR_CONSTANTS: this files contains all the constants definitions for sensor type, sensors
 ids and errors. All numbers are randomly generated so to avoid numbering conflicts.
*/

/**
 \name Sensor ids
 \brief Macro definitions for sensor ids.
 \remark The codes are defined as 8 bytes sequences. Numbers are randomly generated in order
 to avoid conflicts.
 \remark Some sensors have the same id since they are equivalent.
 */

//@{


// Constants for sensor ids
// NB: some have the same ID since they are equivalent

#define S_DHT11 0x644a2c7b
#define S_DHT21 0xe53fd0d1
#define S_AM2301 0xe53fd0d1
#define S_DHT22 0x79aca58e
#define S_AM2302 0x79aca58e
#define S_RHT03 0x79aca58e
#define S_LM34  0x06884901
#define S_LM35  0x446904b2
#define S_DS18B20 0xfb04f450
#define S_GENERIC_PIR 0x1111

//@}

/**
 \name Sensor types
 \brief Macro definitions for sensor types.
 \remark The codes are defined as 4 bytes sequences. Numbers are randomly generated in order
 to avoid conflicts.
 */

//@{


// Constants for type of sensors

#define ST_HUMIDITY 0x45dd
#define ST_TEMPERATURE_HUMIDITY 0xd130
#define ST_TEMPERATURE 0x44ce
#define ST_PIR 0x7301

//@}

#endif
