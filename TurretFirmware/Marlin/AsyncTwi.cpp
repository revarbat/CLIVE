/*
 AsyncTwi.cpp - Asyncronous TWI/I2C library for Wiring & Arduino
 Copyright (c) 2010 Garth Minette.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */



#if 0
# define DEBUG_PRINT(x)       Serial.print(x)
# define DEBUG_PRINTLN(x)     Serial.println(x)
# define DEBUG_PRINT_INT(x)   Serial.print(x,HEX)
# define DEBUG_PRINTLN_INT(x) Serial.println(x,HEX)
# define TWI_MAX_SPINS 5000L
#else
# define DEBUG_PRINT(x)       
# define DEBUG_PRINTLN(x)     
# define DEBUG_PRINT_INT(x)   
# define DEBUG_PRINTLN_INT(x) 
# define TWI_MAX_SPINS 100000L
#endif



// #include <math.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <string.h>
#include <HardwareSerial.h>
//#include "WProgram.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "AsyncTwi.h"

namespace asynctwi {


static void (*onSlaveTransmit)(void);
static void (*onSlaveReceive)(uint8_t*, int);

static uint8_t rxBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t rxBufferIndex;

static uint8_t txBuffer[TWI_BUFFER_LENGTH];
static uint8_t txBufferLength;
static uint8_t txBufferIndex;

static volatile uint8_t masterBuffer[128];
static volatile uint8_t masterHeadIdx;
static volatile uint8_t masterTailIdx;
static volatile uint8_t masterPayloadSent;
static volatile uint8_t framesPending;
static volatile uint8_t lastWriteNacked;



/* 
 * Function ::asynctwi::init
 * Desc     readys twi pins and sets twi bitrate
 * Input    none
 * Output   none
 */
void init(void)
{
  DEBUG_PRINTLN("::asynctwi::init");
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__)
  // activate internal pull-ups for twi
  // as per note from atmega8 manual pg167
  sbi(PORTC, 4);
  sbi(PORTC, 5);
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
  // activate internal pull-ups for twi
  sbi(PORTC, 0);
  sbi(PORTC, 1);
#else
  // activate internal pull-ups for twi
  // as per note from atmega128 manual pg204
  sbi(PORTD, 0);
  sbi(PORTD, 1);
#endif

  // initialize twi prescaler and bit rate
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

  /* twi bit rate formula from atmega128 manual pg 204
   SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
   note: TWBR should be 10 or higher for master mode
   It is 72 for a 16mhz Wiring board with 100kHz TWI */

  // enable twi module, acks, and twi interrupt
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);

  rxBufferIndex = 0;
  txBufferLength = 0;
  txBufferIndex = 0;
  masterHeadIdx = 0;
  masterTailIdx = 0;
  masterPayloadSent = 0;
  framesPending = 0;
  lastWriteNacked = 0;
}



static uint8_t appendByte(uint8_t val) {
    if (::asynctwi::available_tx_bytes() < 1) {
        return -1;
    }
    DEBUG_PRINT(" ");
    DEBUG_PRINT_INT(val);
    masterBuffer[masterTailIdx++] = val;
    if (masterTailIdx >= sizeof(masterBuffer)) {
	masterTailIdx -= sizeof(masterBuffer);
    }
    return ::asynctwi::available_tx_bytes();
}


static uint8_t bufferEmpty() {
    if (masterHeadIdx == masterTailIdx) {
    	return 1;
    }
    	return 0;
    }


static uint8_t readFrameByte(uint8_t pos) {
    uint8_t idx = masterHeadIdx + pos;
    if (idx >= sizeof(masterBuffer)) {
	idx -= sizeof(masterBuffer);
    }
    return masterBuffer[idx];
}


static uint8_t addPacket(uint8_t address, uint8_t* data, uint8_t len, callback_t cb)
{
    DEBUG_PRINT(" A");

    ::asynctwi::appendByte(address);
    ::asynctwi::appendByte(((uint16_t)cb) & 0xff);
    ::asynctwi::appendByte((((uint16_t)cb)>>8) & 0xff);
    ::asynctwi::appendByte(len);
    if (data) {
	for (uint8_t i = 0; i < len; i++) {
	    ::asynctwi::appendByte(data[i]);
	}
    }
    DEBUG_PRINTLN(".");
    framesPending++;
    return framesPending;
}



static uint8_t currentFrameAddress() {
    if (::asynctwi::bufferEmpty()) {
    	return 0;
    }
    return ::asynctwi::readFrameByte(0);
}



static callback_t getFrameCallback() {
    if (::asynctwi::bufferEmpty()) {
    	return NULL;
    }

    uint8_t  addrlo = ::asynctwi::readFrameByte(1);
    uint16_t addrhi = ::asynctwi::readFrameByte(2) & 0xff;
    uint16_t addr = (addrhi<<8) | addrlo;

    return (::asynctwi::callback_t)addr;
}



static uint8_t currentPayloadSize() {
    if (::asynctwi::bufferEmpty()) {
    	return 0;
    }
    return ::asynctwi::readFrameByte(3);
}


static uint8_t getFrameDataByte(uint8_t pos) {
    if (::asynctwi::bufferEmpty()) {
        return 0;
    }
    return ::asynctwi::readFrameByte(4+pos);
}



static uint8_t packetIsARead() {
    uint8_t addr = ::asynctwi::currentFrameAddress();
    if ((addr & 0x1) == TW_READ) {
        return 1;
    }
    return 0;
}



static void commit() {
    DEBUG_PRINT("Commit ");
    uint8_t len = 4;
    if (!::asynctwi::packetIsARead()) {
        len += ::asynctwi::currentPayloadSize();
    }
    masterHeadIdx += len;
    DEBUG_PRINTLN_INT(len);
    if (masterHeadIdx >= sizeof(masterBuffer)) {
	masterHeadIdx -= sizeof(masterBuffer);
    }
    framesPending--;
}


uint8_t available_tx_bytes() {
    uint8_t tmpsreg = SREG;
    cli();
    int8_t len = masterTailIdx - masterHeadIdx;
    if (len < 0) {
    	len += sizeof(masterBuffer);
    }
    SREG = tmpsreg;
    return sizeof(masterBuffer) - len;
}


uint8_t pending() {
    uint8_t tmpsreg = SREG;
    cli();
    uint8_t pend = framesPending;
    SREG = tmpsreg;
    return pend;
}


void purge() {
    uint8_t tmpsreg = SREG;
    cli();
    while (framesPending) {
        ::asynctwi::commit();
    }
    SREG = tmpsreg;
}


/* 
 * Function ::asynctwi::start
 * Desc     grabs bus master status
 * Input    none
 * Output   none
 */
static void start()
{
    DEBUG_PRINTLN("Start");
    // send start condition
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
}



/* 
 * Function ::asynctwi::stop
 * Desc     relinquishes bus master status
 * Input    none
 * Output   none
 */
static void stop(void)
{
    DEBUG_PRINTLN("STOP\n");
    // send stop condition
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

    // wait for stop condition to be exectued on bus
    // TWINT is not set after a stop condition!
    while(TWCR & _BV(TWSTO)){
	continue;
    }
}



/* 
 * Function ::asynctwi::stop_or_restart
 * Desc     if no more frames are pending, sends STOP condition, otherwise sends RESTART condition.
 * Input    none
 * Output   none
 */
static void stop_or_restart()
{
    if (framesPending > 0) {
	::asynctwi::start();
    } else {
	::asynctwi::stop();
    }
}



/* 
 * Function ::asynctwi::releaseBus
 * Desc     releases bus control without Stop signal.
 * Input    none
 * Output   none
 */
static void releaseBus(void)
{
  // release bus
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
}



/* 
 * Function ::asynctwi::ack
 * Desc     sends ACK/NAK or readys receive line
 * Input    ack: byte indicating to ack or to nack
 * Output   none
 */
static void ack(uint8_t ack)
{
  // transmit master read ready signal, with or without ack
  if(ack){
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
  }
  else{
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
  }
}



/* 
 * Function ::asynctwi::write
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          len: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          cb: callback routine to call when write is complete.
 * Output   number of payload data bytes sent, or 0 if call is used asyncronously.
 *          -1 if not enough buffer space was free.
 *          or -2 if read timed out.
 */
int8_t write(uint8_t address, uint8_t* data, uint8_t len, uint8_t wait, callback_t cb)
{
    DEBUG_PRINT("Wr");

    address &= 0xfe;
    address |= TW_WRITE;

    uint8_t tmpsreg = SREG;
    cli();

    // ensure frame will fit into buffer
    if(::asynctwi::available_tx_bytes() < len+4){
	SREG = tmpsreg;
	return -1;
    }

    if (::asynctwi::addPacket(address, data, len, cb) <= 1) {
        ::asynctwi::start();
    }
    SREG = tmpsreg;

    if (wait) {
	uint32_t limit = TWI_MAX_SPINS;
	while (::asynctwi::pending() > 0 && --limit>0) {
	    DEBUG_PRINT("~");
	    continue;
	}
	if (!limit) {
	    ::asynctwi::purge();
	    ::asynctwi::releaseBus();
	    return -2;
	}
	return masterPayloadSent;
    }
    DEBUG_PRINTLN("Wend");
    return 0;
}


/* 
 * Function ::asynctwi::write_and_read_async
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          datalen: number of bytes in data array
 *          maxlen: number of bytes to read
 *          cb: callback routine to call for asyncronous reads.
 * Output   number of bytes read, or 0 if call is used asyncronously
 *          or -1 if not enough buffer space was free.
 *          or -2 if read timed out.
 */
int8_t write_and_read_async(uint8_t address, uint8_t* data, uint8_t datalen, uint8_t* readbuf, uint8_t maxlen, callback_t cb)
{
    DEBUG_PRINT("WRA");

    uint8_t tmpsreg = SREG;
    cli();
    // ensure frame will fit into buffer
    if(::asynctwi::available_tx_bytes() < datalen+8){
	SREG = tmpsreg;
	DEBUG_PRINTLN("RWAnoroom");
	return -1;
    }

    if(TWI_BUFFER_LENGTH < maxlen){
	maxlen = TWI_BUFFER_LENGTH;
    }

    address = (address & 0xfe) | TW_WRITE;
    ::asynctwi::addPacket(address, data, datalen, NULL);

    address = (address & 0xfe) | TW_READ;
    if (::asynctwi::addPacket(address, NULL, maxlen, cb) <= 2) {
        ::asynctwi::start();
    }
    SREG = tmpsreg;

    if (!cb) {
	uint32_t limit = TWI_MAX_SPINS;
	while (::asynctwi::pending() > 0 && --limit>0) {
	    DEBUG_PRINT("~");
	    continue;
	}
	if (!limit) {
	    DEBUG_PRINTLN("RWApurge");
	    ::asynctwi::purge();
	    ::asynctwi::releaseBus();
	    DEBUG_PRINTLN("RWAabort");
	    return -2;
	}
	DEBUG_PRINTLN("RWAend");
	return rxBufferIndex;
    } else if (readbuf) {
	uint8_t numbytes = rxBufferIndex;
	memcpy(readbuf, rxBuffer, numbytes);
	DEBUG_PRINTLN("RWAend");
	return numbytes;
    }
    DEBUG_PRINTLN("RWAend");
    return 0;
}



/* 
 * Function ::asynctwi::read_async
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          maxlen: number of bytes to read into array
 *          cb: callback routine to call for asyncronous reads.
 * Output   number of bytes read, or 0 if call is used asyncronously
 *          or -1 if not enough buffer space was free.
 *          or -2 if read timed out.
 */
int8_t read_async(uint8_t address, uint8_t maxlen, callback_t cb)
{
    DEBUG_PRINT("RA");

    uint8_t tmpsreg = SREG;
    cli();
    // ensure frame will fit into buffer
    if(::asynctwi::available_tx_bytes() < 4){
	SREG = tmpsreg;
	return -1;
    }

    if(TWI_BUFFER_LENGTH < maxlen){
	maxlen = TWI_BUFFER_LENGTH;
    }

    address &= 0xfe;
    address |= TW_READ;

    if (::asynctwi::addPacket(address, NULL, maxlen, cb) <= 1) {
        ::asynctwi::start();
    }
    SREG = tmpsreg;

    if (!cb) {
	uint32_t limit = TWI_MAX_SPINS;
	while (::asynctwi::pending() > 0 && --limit>0) {
	    DEBUG_PRINT("~");
	    continue;
	}
	if (!limit) {
	    ::asynctwi::purge();
	    ::asynctwi::releaseBus();
	    return -2;
	}
	return rxBufferIndex;
    }
    DEBUG_PRINTLN("RAend");
    return 0;
}



/* 
 * Function ::asynctwi::read
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          buf: pointer to byte array to put read data into
 *          maxlen: max number of bytes to read into array
 * Output   number of bytes read, or 0 if call is used asyncronously
 *          or -1 if not enough buffer space was free.
 *          or -2 if read timed out.
 */
int8_t read(uint8_t address, uint8_t* buf, uint8_t maxlen)
{
    DEBUG_PRINT("R");
    uint8_t tmpsreg = SREG;
    cli();

    // ensure frame will fit into buffer
    if(::asynctwi::available_tx_bytes() < 4){
	SREG = tmpsreg;
	return -1;
    }

    if(TWI_BUFFER_LENGTH < maxlen){
	maxlen = TWI_BUFFER_LENGTH;
    }

    address &= 0xfe;
    address |= TW_READ;

    if (::asynctwi::addPacket(address, NULL, maxlen, NULL) <= 1) {
        ::asynctwi::start();
    }
    SREG = tmpsreg;

    uint32_t limit = TWI_MAX_SPINS;
    while (::asynctwi::pending() > 0 && --limit>0) {
	DEBUG_PRINT("~");
	continue;
    }
    if (!limit) {
	::asynctwi::purge();
	::asynctwi::releaseBus();
        return -2;
    }
    uint8_t numbytes = rxBufferIndex;
    memcpy(buf, rxBuffer, numbytes);

    DEBUG_PRINTLN("Rend");
    return numbytes;
}



/* 
 * Function ::asynctwi::transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   -1 length too long for buffer
 *          0 ok
 */
int8_t transmit(uint8_t* data, uint8_t length)
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return -1;
  }

  // set length and copy data into tx buffer
  txBufferLength = length;
  for(i = 0; i < length; ++i){
    txBuffer[i] = data[i];
  }

  return 0;
}



/* 
 * Function ::asynctwi::slaveInit
 * Desc     sets slave address and enables interrupt
 * Input    none
 * Output   none
 */
void setAddress(uint8_t address)
{
  // set twi slave address (skip over TWGCE bit)
  TWAR = address << 1;
}



/* 
 * Function ::asynctwi::attachSlaveRxEvent
 * Desc     sets function called before a slave read operation
 * Input    function: callback function to use
 * Output   none
 */
void attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  ::asynctwi::onSlaveReceive = function;
}



/* 
 * Function ::asynctwi::attachSlaveTxEvent
 * Desc     sets function called before a slave write operation
 * Input    function: callback function to use
 * Output   none
 */
void attachSlaveTxEvent( void (*function)(void) )
{
  ::asynctwi::onSlaveTransmit = function;
}



/* Interrupt handler and TWI state engine. */
SIGNAL(SIG_2WIRE_SERIAL)
{
  if (TWCR & _BV(TWINT) == 0) {
      return;
  }

  DEBUG_PRINT("? ");
  DEBUG_PRINT_INT(TW_STATUS);
  DEBUG_PRINTLN("");

  ::asynctwi::callback_t cb = ::asynctwi::getFrameCallback();
  uint8_t sladdr = ::asynctwi::currentFrameAddress();
  uint8_t val;

  switch(TW_STATUS){

  /////////////////////////////////////
  // ALL MASTER MODES
  /////////////////////////////////////
  case TW_START:     // sent start condition
  case TW_REP_START: // sent repeated start condition
    // copy device address and r/w bit to output register and ack
    TWDR = ::asynctwi::currentFrameAddress();
    DEBUG_PRINT("ADDR:");
    DEBUG_PRINTLN_INT(sladdr);
    ::asynctwi::ack(1);
    rxBufferIndex = 0;
    break;

  /////////////////////////////////////
  // MASTER TRANSMITTER
  /////////////////////////////////////
  case TW_MT_SLA_ACK:  // slave receiver acked address
    masterPayloadSent = 0;
  case TW_MT_DATA_ACK: // slave receiver acked data
    // if there is data to send, send it, otherwise stop 
    ::asynctwi::lastWriteNacked = 0;
    if (masterPayloadSent < ::asynctwi::currentPayloadSize()) {
      val = ::asynctwi::getFrameDataByte(masterPayloadSent++);
      DEBUG_PRINT("SEND:");
      DEBUG_PRINTLN_INT(val);
      TWDR = val;
      ::asynctwi::ack(1);
    }
    else{
      ::asynctwi::commit();
      ::asynctwi::stop_or_restart();
      if (cb) {
        DEBUG_PRINTLN("CB");
	cb(sladdr, NULL, masterPayloadSent);
        DEBUG_PRINTLN("CBE");
      }
    }
    break;

  case TW_MT_SLA_NACK:  // address sent, nack received
    // Discard rest of data in frame.
    DEBUG_PRINTLN("NAKADDR");
    masterPayloadSent = 0;
    ::asynctwi::lastWriteNacked = 1;
    ::asynctwi::commit();   // On to the next packet.
    if (cb) {
    // Tell callback we sent no payload data.
      DEBUG_PRINTLN("CB");
      cb(sladdr, NULL, 0);
      DEBUG_PRINTLN("CBE");
    }
    if (::asynctwi::packetIsARead()) {
        DEBUG_PRINTLN("IMMREAD");
        uint8_t rdsladdr = ::asynctwi::currentFrameAddress();
	if ((sladdr&0xfe) == (rdsladdr&0xfe)) {
            DEBUG_PRINTLN("SAMEADDR");
	    sladdr = rdsladdr;
	    cb = ::asynctwi::getFrameCallback();
	    ::asynctwi::lastWriteNacked = 0;
	    rxBufferIndex = 0;
	    ::asynctwi::commit();   // On to the next packet.
	    if (cb) {
	      DEBUG_PRINT("CB:");
	      DEBUG_PRINTLN_INT((uint16_t)cb);
	      cb(sladdr, rxBuffer, rxBufferIndex);
	      DEBUG_PRINTLN("CBE");
	    }
	}
    }
    ::asynctwi::stop_or_restart();

    break;

  case TW_MT_DATA_NACK: // data sent, nack received
    // Discard rest of data in frame.
    DEBUG_PRINTLN("NAKDATA");
    ::asynctwi::lastWriteNacked = 0;
    ::asynctwi::commit();   // On to the next packet.
    if (cb) {
    // Tell callback how many payload bytes we sent.
      DEBUG_PRINTLN("CB");
      cb(sladdr, NULL, masterPayloadSent);
      DEBUG_PRINTLN("CBE");
    }
    if (::asynctwi::packetIsARead()) {
        DEBUG_PRINTLN("IMMREAD");
        uint8_t rdsladdr = ::asynctwi::currentFrameAddress();
	if ((sladdr&0xfe) == (rdsladdr&0xfe)) {
            DEBUG_PRINTLN("SAMEADDR");
	    sladdr = rdsladdr;
	    cb = ::asynctwi::getFrameCallback();
	    ::asynctwi::lastWriteNacked = 0;
	    rxBufferIndex = 0;
	    ::asynctwi::commit();   // On to the next packet.
	    if (cb) {
	      DEBUG_PRINT("CB:");
	      DEBUG_PRINTLN_INT((uint16_t)cb);
	      cb(sladdr, rxBuffer, rxBufferIndex);
	      DEBUG_PRINTLN("CBE");
	    }
	}
    }
    ::asynctwi::stop_or_restart();

    break;

  case TW_MT_ARB_LOST: // We lost bus arbitration
    DEBUG_PRINTLN("ARBLOST");
    // We'll resend this packet later.
    ::asynctwi::releaseBus();    // Let the other master have bus control.
    ::asynctwi::start();         // Try to re-gain bus after other master finishes.
    break;

  /////////////////////////////////////
  // MASTER RECEIVER
  /////////////////////////////////////
  case TW_MR_DATA_ACK: // data received, ack sent
    DEBUG_PRINTLN("READ");
    // put byte into buffer
    rxBuffer[rxBufferIndex++] = TWDR;
    // ack if more bytes are expected, otherwise nack
    if(rxBufferIndex < ::asynctwi::currentPayloadSize()) {
      ::asynctwi::ack(1);
    }
    else{
      ::asynctwi::ack(0);
    }
    break;

  case TW_MR_SLA_ACK:  // address sent, ack received
    // ack if more bytes are expected, otherwise nack
    if(::asynctwi::currentPayloadSize() > 1) {
      DEBUG_PRINTLN("ACKNEXTREAD");
      ::asynctwi::ack(1);
    }
    else{
      DEBUG_PRINTLN("NAKNEXTREAD");
      ::asynctwi::ack(0);
    }
    break;

  case TW_MR_DATA_NACK: // data received, nack sent
    // put final byte into buffer
    rxBuffer[rxBufferIndex++] = TWDR;
  case TW_MR_SLA_NACK: // address sent, nack received
    DEBUG_PRINTLN("READNAK");
    ::asynctwi::commit();
    ::asynctwi::stop_or_restart();
    if (cb) {
      DEBUG_PRINT("CB:");
      DEBUG_PRINTLN_INT((uint16_t)cb);
      cb(sladdr, rxBuffer, rxBufferIndex);
      DEBUG_PRINTLN("CBE");
    }
    break;

    // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case

  /////////////////////////////////////
  // SLAVE RECEIVER
  /////////////////////////////////////
  case TW_SR_SLA_ACK:   // addressed, returned ack
  case TW_SR_GCALL_ACK: // addressed generally, returned ack
  case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
  case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
    // indicate that rx buffer can be overwritten and ack
    rxBufferIndex = 0;
    ::asynctwi::ack(1);
    break;

  case TW_SR_DATA_ACK:       // data received, returned ack
  case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
    // if there is still room in the rx buffer
    if(rxBufferIndex < TWI_BUFFER_LENGTH){
      // put byte in buffer and ack
      rxBuffer[rxBufferIndex++] = TWDR;
      ::asynctwi::ack(1);
    }
    else{
      // otherwise nack
      ::asynctwi::ack(0);
    }
    break;

  case TW_SR_STOP: // stop or repeated start condition received
    // put a null char after data if there's room
    if(rxBufferIndex < TWI_BUFFER_LENGTH){
      rxBuffer[rxBufferIndex] = '\0';
    }
    // callback to user defined callback
    // TODO: Fix this
    ::asynctwi::onSlaveReceive(rxBuffer, rxBufferIndex);
    // ack future responses
    ::asynctwi::ack(1);
    break;

  case TW_SR_DATA_NACK:       // data received, returned nack
  case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
    // nack back at master
    ::asynctwi::ack(0);
    break;

  /////////////////////////////////////
  // SLAVE TRANSMITTER
  /////////////////////////////////////
  case TW_ST_SLA_ACK:          // addressed, returned ack
  case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
    // ready the tx buffer index for iteration
    txBufferIndex = 0;
    // set tx buffer length to be zero, to verify if user changes it
    txBufferLength = 0;
    // request for txBuffer to be filled and length to be set
    // note: user must call ::asynctwi::transmit(bytes, length) to do this
    ::asynctwi::onSlaveTransmit();
    // if they didn't change buffer & length, initialize it
    if(0 == txBufferLength){
      txBufferLength = 1;
      txBuffer[0] = 0x00;
    }
    // transmit first byte from buffer, fall
  case TW_ST_DATA_ACK: // byte sent, ack returned
    // copy data to output register
    TWDR = txBuffer[txBufferIndex++];
    // if there is more to send, ack, otherwise nack
    if(txBufferIndex < txBufferLength){
      ::asynctwi::ack(1);
    }
    else{
      ::asynctwi::ack(0);
    }
    break;
  case TW_ST_DATA_NACK: // received nack, we are done 
  case TW_ST_LAST_DATA: // received ack, but we are done already!
    // ack future responses
    ::asynctwi::ack(1);
    break;

  /////////////////////////////////////
  // WIERD STATES
  /////////////////////////////////////
  case TW_NO_INFO:   // no state information
    break;
  case TW_BUS_ERROR: // bus error, illegal stop/start
    ::asynctwi::stop();
    if (framesPending > 0) {
      ::asynctwi::start();  // Attempt to restart.
    }
    break;
  }
  DEBUG_PRINTLN("*");
}


} // namespace asynctwi


