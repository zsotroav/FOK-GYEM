#ifndef _DRIVER_HEADER_
#define _DRIVER_HEADER_

#include "config.h"
#include "shiftRegister.h"
#include "Arduino.h"

#define DRV_COL_COUNT 24
#define DRV_ROW_COUNT 7
#define DRV_ROW_SHIFT_BASE 0x02

#define DRV_DATABUFF_SIZE (DRV_ROW_COUNT * DRV_COL_COUNT/8 * CFG_PANEL_COUNT)

#define AT(x, y) ((x) + (y)*(DRV_COL_COUNT/8)*CFG_PANEL_COUNT)

/**
 * @brief Initializes the driver & hardware. Must be called first in the program.
 * 
 */
void driver_init();

/**
 * @brief Get the data byte from the buffer at the following byte coordinates:
 * 
 * @param x is the column index. One increase in x means 8 pixels horizontally.
 * @param y is the row index.
 * @return is the value of the byte at the index.
 */
uint8_t driver_getByteAt(uint8_t x, uint8_t y);

/**
 * @brief Set the data byte in the buffer at the following byte coordinates:
 * 
 * @param x is the column index. One increase in x means 8 pixels horizontally.
 * @param y is the row index.
 */
void driver_setByteAt(uint8_t x, uint8_t y, uint8_t byte);

/**
 * @brief Returns the buffer. For indexing the buffer using bytes use the AT() macro.
 * 
 * @param data points to a valid, user defined data field. The size must be at least DRV_DATABUFF_SIZE.
 */
void driver_getBuffer(uint8_t * data);

/**
 * @brief Set the content of the internal driver buffer.
 * 
 * @param data user defined memory area with the data to set the buffer to. To fill be buffer, the size must be DRV_DATABUFF_SIZE.
 * @param size is redundant check of the data size. If the size is smaller than DRV_DATABUFF_SIZE, then no copy will occour.
 */
void driver_setBuffer(const uint8_t * data, uint8_t size);

/**
 * @brief Writes the content of the internal buffer onto the screen.
 * When relative mode is used, stores the previous writeScreen data, and only writes the modified pixels onto the screen.
 * 
 */
void driver_writeScreen();

/**
 * @brief In relative mode overrites the full screen with the given buffer data.
 * 
 */
void driver_forceWriteScreen();

#endif