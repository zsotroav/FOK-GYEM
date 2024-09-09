#include <Arduino.h>
#include "shiftRegister.h"
#include "driver.h"

/**
 *                                !!! IMPORTANT !!!                               
 * Make sure all settings in config.h are set correctly and according to your setup
 *                      Most important values: CFG_PANEL_COUNT
 * 
 *          Do NOT change anything else unless you know what you are doing
 *            to avoid damaging the Arduino, driver PCB, or the displays
 */

// Screen buffer
uint8_t buff[DRV_DATABUFF_SIZE];

// Set up data to be printed out
uint8_t data[] = { 0x20, 0x09, 0x00, 0x20, 0x09, 0x00, 0x38, 0xC9, 0x18, 0x25, 0x29, 0x24, 0x25, 0xE9, 0x24, 0x25, 0x09, 0x24, 0x24, 0xC4, 0x98 };


void setup() {
    driver_init();
    
    // Anything that runs once here
    
    // Fully clear screen
    for (uint8_t i = 0; i < DRV_DATABUFF_SIZE; i++) { buff[i] = 0x00; }
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_forceWriteScreen();
    
    // Once done, we enter loop()
}

void loop() {
    // Everythign in here is repeaded indefinitely
    driver_setBuffer(data, DRV_DATABUFF_SIZE);  // Set screen buffer
    driver_writeScreen();                       // Update the screen
    delay(2000);                                // Delay next update

    for(uint8_t i = 0; i <DRV_DATABUFF_SIZE; i++) { // Generate transition 
        buff[i] = 0xAA; // 10101010
    }
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_writeScreen();
    
    for(uint8_t i = 0; i <DRV_DATABUFF_SIZE; i++) {
        buff[i] = 0x55; // 01010101
    }
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_writeScreen();
}