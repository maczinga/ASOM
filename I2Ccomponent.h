/**
 \file I2CComponent.h
 \brief Definition of a class for dealing with basic I2C communication.
 \details Header file containing the definition of the I2CComponent class.
 \author Enrico Formenti
 \version 0.1
 \date 2012-2013
 \warning This software is provided "as is". The author is 
 not responsible for any damage of any kind caused by this
 software. Use it at your own risk.
 \copyright BSD license. See license.txt for more details. 
 All text above must be included in any redistribution. 
*/


#ifndef I2Ccomponent_H
#define I2Ccomponent_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "/Applications/Arduino.app/Contents/Resources/Java/libraries/Wire/Wire.h"

/**
 \class I2Ccomponent I2Ccomponent.h
 \brief Basic class for dealing with components which use the I2C bus.
 \author Enrico Formenti
 */

class I2Ccomponent {

private:
  /**
	 \var uint8_t _address
	 \brief Address of the component on the I2C bus.
	 */
	uint8_t _address;
protected:
	/**
	 \fn uint8_t readByte(const uint8_t adr)
	 \brief Reads a byte from the component using the I2C bus.
	 @param adr Addresss to be read (ranging from 0x0 to 0xFF)
	 \remark No check is made here to establish if \c adr is a valid address
	 for this component.
	 */ 
	uint8_t readByte(const uint8_t adr);
	/**
	 \fn void writeByte(const uint8_t adr, const uint8_t data)
	 \brief Writes (sends) a byte to the component via the I2C bus.
	 @param adr Addresss to be written to (ranging from 0x0 to 0xFF)
	 @param data Data to be written.
	 \remark No check is made here to establish if \c adr is a valid address
	 for this component.
	 */	
	void writeByte(const uint8_t adr, const uint8_t data);
public:	
	/**
	 \fn I2Ccomponent(const uint8_t a)
	 \brief Constructor
	 @param a Address of this component on the I2C bus.
	 \remark No check is made if there are conflicting addresses on the bus.
	*/
	inline I2Ccomponent(const uint8_t a) { _address = a; Wire.begin(); }
	/**
	 \fn uint8_t getAddress(void)
	 \brief Returns the address of this component on the I2C bus.
	 */ 
	inline uint8_t getAddress(void) { return _address; }
};

#endif
