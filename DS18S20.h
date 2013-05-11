
/******************************************************
 SENSOR LIBRARY:
 
 Written by Enrico Formenti.  
 BSD license, check license.txt for more information
 All text above must be included in any redistribution.
 ******************************************************/



/**
 \file DS18S20.h
 \brief Class definition for DS18S20 component.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
 */

/**
 \class DS18S20 DS18S20.h
 \brief Class for DS18S20 component. This a temperature sensor with fixed resolution (9 bits).
 \details \brief Class for DS18S20 component. This a temperature sensor with fixed resolution (9 bits). 
 The user is allowed to set an alarm which will be triggered whenever the temperature goes below \c _alarm_tmin
 or higher than \c _alarm_tmax. These bounds can be set using \c setAlarm function. Their range
 must be contained in the operating range of the component i.e. between -25°C and +125°C.
 */


#ifndef DS18S20_H
#define DS18S20_H

#include "DB18B20.h"

/* Sensor library
 
 DS18S20.H: class for temperature sensor DS18B20

written by Enrico Formenti
*/

class DS18S20: public DS18B20 {  
public:
	/**
	 \fn DS18B20(uint8_t p) : Sensor(S_DS18B20,ST_TEMPERATURE)  : OWcomponent(p)
	 \brief Constructor
	 @param p The Arduino pin to which the data pin of DS18B20 is connected to.
	 */
	inline DS18S20(uint8_t p) : DS18B20(p) { };
	/**
	 \fn uint8_t getResolution(void)
	 \brief Returns the resolution of the temperature value.
	 \return Number of bits of resolution for the temperature value. For this model it is fixed to 9
	 \remark This number should be multiplied by the basic step value (1/8) to get the actual approximation.
	 @see setResolution
	 */	
	inline uint8_t getResolution(void) { return 9; };
	/**
	 \fn uint8_t setResolution(uint8_t r)
	 \brief Sets the resolution of the temperature value.
	 \return r Number of bits of resolution for the temperature value. This instruction has no effect for this version of
	 the component since resolution bits is fixed to 9
	 @see getResolution
	 */	
	inline setResolution(uint8_t r) { }
	/**
	 \fn float getTemperature(void)
	 \brief Returns the temperature in Celsius degrees.
	 */
	float getTemperature(void) { return (float)((_scratchpad[TEMP_MSB]) << 8) | _scratchpad[TEMP_LSB]) >> 1) - 0.25 +((float)(scratchpad[COUNT_PER_C] - scratchpad[COUNT_REMAIN]) / (float)scratchpad[COUNT_PER_C] );}
};

#endif
