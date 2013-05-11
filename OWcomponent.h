
/**
 \file OWcomponent.h
 \brief Definition of the OWcomponent class.
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
 \class OWcomponent OWcomponent.h
 \brief Class for dealing with components based on the OneWire bus.
 \author Paul Stoffregen (he wrote the whole class)
 \author Enrico Formenti (added search for alarms feature and improved 
 error management, plus some other (very) minor improvements)
 */


#ifndef OWCOMPONENT_H
#define OWCOMPONENT_H

#include <inttypes.h>

#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif

#include "Error.h"

#define FAM_CODE_DB18S20 0x10
#define FAM_CODE_DB18B20 0x28

#define CMD_ALARM_SEARCH 0xEC
#define CMD_GENERIC_SEARCH 0xF0
#define CMD_READ_ROM 0x33
#define CMD_MATCH_ROM 0x55
#define CMD_SKIP_ROM 0xCC

// You can exclude certain features from OneWire.  In theory, this
// might save some space.  In practice, the compiler automatically
// removes unused code (technically, the linker, using -fdata-sections
// and -ffunction-sections when compiling, and Wl,--gc-sections
// when linking), so most of these will not result in any code size
// reduction.  Well, unless you try to use the missing features
// and redesign your program to not need them!  ONEWIRE_CRC8_TABLE
// is the exception, because it selects a fast but large algorithm
// or a small but slow algorithm.

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

// Select the table-lookup method of computing the 8-bit CRC
// by setting this to 1.  The lookup table enlarges code size by
// about 250 bytes.  It does NOT consume RAM (but did in very
// old versions of OneWire).  If you disable this, a slower
// but very compact algorithm is used.
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 1
#endif

// You can allow 16-bit CRC checks by defining this to 1
// (Note that ONEWIRE_CRC must also be 1.)
#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 1
#endif

#define FALSE 0
#define TRUE  1

// Platform specific I/O definitions

/* #if defined(__AVR__) */
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define IO_REG_ASM asm("r30")
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*(base+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*(base+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*(base+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*(base+2)) |= (mask))
/*
#elif defined(__PIC32MX__)
#include <plib.h>  // is this necessary?
#define PIN_TO_BASEREG(pin)             (portModeRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_ASM
#define DIRECT_READ(base, mask)         (((*(base+4)) & (mask)) ? 1 : 0)  //PORTX + 0x10
#define DIRECT_MODE_INPUT(base, mask)   ((*(base+2)) = (mask))            //TRISXSET + 0x08
#define DIRECT_MODE_OUTPUT(base, mask)  ((*(base+1)) = (mask))            //TRISXCLR + 0x04
#define DIRECT_WRITE_LOW(base, mask)    ((*(base+8+1)) = (mask))          //LATXCLR  + 0x24
#define DIRECT_WRITE_HIGH(base, mask)   ((*(base+8+2)) = (mask))          //LATXSET + 0x28

#else
#error "Please define I/O register types here"
#endif
*/

class OWcomponent : public Error {

private:
    IO_REG_TYPE bitmask;
    volatile IO_REG_TYPE *baseReg;

#if ONEWIRE_SEARCH
    // global search state
    uint8_t ROM_NO[8];
    uint8_t LastDiscrepancy;
    uint8_t LastFamilyDiscrepancy;
    uint8_t LastDeviceFlag;
#endif
  
protected:

	/**
	 \var uint8_t *_adr
	 \brief Address of this component on the 1-Wire bus. Remark that this address is obtained from the \c search() operation
	 in the constructor. I'm not sure that this operation is thread safe.
	 */
	uint8_t *_adr; // address of this component on the bus
	
public:
	/**
	 \fn OWcomponent(const uint8_t pin)
	 \brief Constructor
	 @param pin Arduino pin to which this component is connected to.
	 */
    OWcomponent(const uint8_t pin);

	/**
	 \fn uint8_t reset(void)
	 \brief Perform a 1-Wire reset cycle. 
	 \return Returns 1 if a device responds with a presence pulse.  Returns 0 if there is no device or 
	 the bus is shorted or otherwise held low for more than 250uS
	 */
    uint8_t reset(void);

	/**
	 \fn void select(uint8_t rom[8])
	 \brief Issue a 1-Wire rom select command, you do the reset first.
	 @param rom Address of the rom to select.
	 */
    void select(uint8_t rom[8]);

	/**
	 \fn void skip(void)
	 \brief Issues a 1-Wire rom skip command, to address all slaves on the bus.
	 */
    void skip(void);
	
	/**
	 \fn void write(uint8_t v, uint8_t power = 0)
	 \brief Writes a byte to the current selected device
	 @param v Byte to write to the current selected device
	 @param power Set to \c 1 if the wire is held high at the end for parasitically 
	 powered devices. You are responsible for eventually depowering it by calling \c depower() or doing
     another read or write.
	 */
    void write(uint8_t v, uint8_t power = 0);
	/**
	 \fn void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0)
	 \brief Writes \c count bytes from \c buf to the current selected device
	 @param buf Buffer containing the data to be written.
	 @param count number of bytes to be written.
	 @param power Set to \c 1 if the wire is held high at the end for parasitically 
	 powered devices. You are responsible for eventually depowering it by calling \c depower() or doing
     another read or write.
	 */
	
    void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);

	/**
	 \fn uint8_t read(void)
	 \brief Reads a byte of data from the currently selected device.
	 \return The byte that has been read
	 */
    uint8_t read(void);

	/**
	 \fn void read_bytes(uint8_t *buf, uint16_t count)
	 \brief Read \c count bytes from the currently selected device and stores them in \c buf.
	 @param buf Buffer for storing the data read.
	 @param count Number of bytes to read.
	 */
    void read_bytes(uint8_t *buf, uint16_t count);
	
	/**
	 \fn void write_bit(uint8_t v)
	 \brief Writes a bit to the current selected device. The bus is always left powered at the end, see
	 note in \c write() about that.
	 @param v The bit to be written.
	 @see write()
	 */
    void write_bit(uint8_t v);
	/**
	 \fn  uint8_t read_bit(void)
	 \brief Reads a bit of data from the currently selected device.
	 \return The bit that has been read
	 */
    uint8_t read_bit(void);

	/**
	 \fn void depower(void)
	 \brief Stop forcing power onto the bus. 
	 \details Stop forcing power onto the bus. You only need to do this if
	 you used the 'power' flag to write() or used a write_bit() call
	 and aren't about to do another read or write. You would rather
	 not leave this powered if you don't have to, just in case
	 someone shorts your bus.
	 @see write, write_bytes, write_bit
	 */
    void depower(void);

#if ONEWIRE_SEARCH
	/**
	 \fn void reset_search(void)
	 \brief Clear the search state so that it will start from the beginning again.
	 */
    void reset_search(void);

	/**
	 \fn bool search(uint8_t *newAddr, uint8_t searchCmd)
	 \brief Look for the next device.
	 @param newAddr Address of the newly found device
	 @param searchCmd Type of search to perform. There are two possible types: \c CMD_GENERIC_SEARCH and \c CMD_ALARM_SEARCH.
	 The first is to find new devices on the bus, the second to detect a device which triggered an alarm.
	 \return Returns \c True if a new address has been returned. A \c False might mean that the bus is shorted, there are
	 no devices, or you have already retrieved all of them.
	 \remark It might be a good idea to check the CRC to make sure you didn't get garbage.  The order is 
	 deterministic. You will always get the same devices in the same order.
	 */
    bool search(uint8_t *newAddr, uint8_t searchCmd);
#endif
	
#if ONEWIRE_CRC
	/**
	 \fn uint8_t crc8( uint8_t *addr, uint8_t len)
	 \brief Compute a Dallas Semiconductor 8 bit CRC, these are used in the ROM and scratchpad registers.
	 @param addr Data buffer
	 @param len Length of the buffer
	 \return The CRC value.
	 */
    static uint8_t crc8( uint8_t *addr, uint8_t len);

#if ONEWIRE_CRC16
	/**
	 \fn bool check_crc16(uint8_t* input, uint16_t len, uint8_t* inverted_crc)
	 \brief Compute the 1-Wire CRC16 and compare it against the received CRC.
	 \details Compute the 1-Wire CRC16 and compare it against the received CRC.
	 \par Example usage (reading a DS2408):
     \par
	 <code>
	   //// Put everything in a buffer so we can compute the CRC easily.<br/>
	   <br/>
        uint8_t buf[13];<br/>
        buf[0] = 0xF0;    // Read PIO Registers<br/>
        buf[1] = 0x88;    // LSB address<br/>
        buf[2] = 0x00;    // MSB address<br/>
        write_bytes(net, buf, 3);    // Write 3 cmd bytes<br/>
        read_bytes(net, buf+3, 10);  // Read 6 data bytes, 2 0xFF, 2 CRC16<br/>
        if (!CheckCRC16(buf, 11, &buf[11])) {<br/>
        &nbsp;&nbsp;    // Handle error.<br/>
        }<br/>
    </code>         
    @param input - Array of bytes to checksum.
    @param len - How many bytes to use.
    @param inverted_crc - The two CRC16 bytes in the received data. This should just point into the received data, *not* at a 16-bit integer.
    @return True, iff the CRC matches.
	 */
    static bool check_crc16(uint8_t* input, uint16_t len, uint8_t* inverted_crc);
	/**
	 \fn static uint16_t crc16(uint8_t* input, uint16_t len)
	 \brief Compute a Dallas Semiconductor 16 bit CRC.  
	 \details Compute a Dallas Semiconductor 16 bit CRC. This is required to check the integrity of data received from many 
	 1-Wire devices.  Note that the CRC computed here is *not* what you'll get from the 1-Wire network, for two reasons:
	 \par
	  1) The CRC is transmitted bitwise inverted.<br>
      2) Depending on the endian-ness of your processor, the binary
     representation of the two-byte return value may have a different
     byte order than the two bytes you get from 1-Wire.
     @param input - Array of bytes to checksum.
	 @param len - How many bytes to use.
	 @return The CRC16, as defined by Dallas Semiconductor.
	 */
    static uint16_t crc16(uint8_t* input, uint16_t len);
#endif
#endif
};

#endif
