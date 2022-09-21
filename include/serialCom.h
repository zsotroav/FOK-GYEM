#ifndef _SERIALCOM_HEADER_
#define _SERIALCOM_HEADER_

#include <Arduino.h>

/**
 * @brief Initializes/reinitializes the serial communicaton with the given parameters
 * 
 * @param SerialInstance Arduino serial instance
 * @param baudrate Applicable baudrate
 */
void sCom_init(HardwareSerial* SerialInstance, unsigned long baudrate);

/**
 * @brief Waits until size amount of serial data arrives, and immediately loads the bytes into the buffer
 * one by one.
 * 
 * @param buffer Data buffer
 * @param size Amount of bytes to receive
 */
void sCom_waitForData(uint8_t* buffer, uint16_t size);


#endif