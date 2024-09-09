#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_


/**
 * @brief Set to the exact amount of panels in the display. Never set it lower!!!
 */
#define CFG_PANEL_COUNT 1
/**
 * @brief Set the byte polarity of the driver - 0 is normal, 1 is inverted
 */
#define CFG_PIXEL_POLARITY 0

/**
 * @brief Enabling relative mode only updates the pixels that are different compared to the previous state
 */
#define CFG_RELATIVE_MODE 1

/**
 * @brief Configures the energization time for each coil, in microseconds. Default value is 1000 us = 1 ms, from specification.
 * Drastic write speed increase can be achieved using this. The value should determined experimentally, and it depends
 * on the screen position. Eg. using the screen horizontally requires less magnetic energization than using vertically.
 * 
 * Reference values: A good condition vertical screen works well with 400 us write time. For a horizontal one I was able
 * to decrease the value to 300 us. 
 *
 * 								!!! DANGER !!!
 * Increasing this value over 1000 us might cause overcurrent in the driver IC's or overheating the
 * windings which might destroy them.
 */
#define CFG_WRITE_TIME_US 400

//PIN DEFINITIONS

#define CFG_ROW_CONTROL_DATA		11
#define CFG_ROW_CONTROL_CLOCK		13
#define CFG_ROW_CONTROL_STROBE		10
#define CFG_ROW_CONTROL_OUTPUT		8

#define CFG_COL_CONTROL_DATA		5
#define CFG_COL_CONTROL_CLOCK		4
#define CFG_COL_CONTROL_STROBE		3
#define CFG_COL_CONTROL_OUTPUT_12V	6
#define CFG_COL_CONTROL_OUTPUT_GND	7

#define CFG_COL_CONTROL_OUTPUT_12V_ON	LOW
#define CFG_COL_CONTROL_OUTPUT_12V_OFF	HIGH
#define CFG_COL_CONTROL_OUTPUT_GND_ON	LOW
#define CFG_COL_CONTROL_OUTPUT_GND_OFF	HIGH

#endif