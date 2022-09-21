#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_


/**
 * @brief Set to the exact amount of panels in the display. Never set it lower!!!
 */
#define CFG_PANEL_COUNT 4
/**
 * @brief Set the byte polarity of the driver - 0 is normal, 1 is inverted
 */
#define CFG_PIXEL_POLARITY 0

/**
 * @brief Enabling relative mode only updates the pixels that are different compared to the previous state
 */
#define CFG_RELATIVE_MODE 1

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