#include <Arduino.h>
#include "shiftRegister.h"
#include "driver.h"


/**
 *  IMPORTANT:
 *  REMEMBER TO SET CFG_PANEL_COUNT 
 *  AND OTHER SETTINGS CORRECTLY
 *  IN CONFIG.H BEFORE CONNECTING 
 *  THE SCREEN OR SCREEN 15V
 */

// Screen buffer
uint8_t buff[DRV_DATABUFF_SIZE];

// Set up data to be printed out
uint8_t data[] = {
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xf3, 0xe7, 0x1c,   0x38, 0x70, 0xe1,   0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x01,   0x00, 0x22, 0x22,   0x44, 0x89, 0x13,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x01,   0x00, 0x42, 0x20,   0x04, 0x98, 0x11,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xe0, 0x82, 0x2e,   0x08, 0xa8, 0x21,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x11, 0x02, 0x22,   0x10, 0xc8, 0x41,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x12, 0x02, 0x22,   0x20, 0x88, 0x81,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x01,   0xe3, 0xe7, 0x1e,   0x7c, 0x71, 0xf3,   0x80, 0x00, 0x00
    };

void setup() {
    driver_init();

    // Anything that runs once here
    
    // Fully clear screen
    for (uint8_t i = 0; i < DRV_DATABUFF_SIZE; i++) { buff[i] = 0x00; }
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_writeScreen();
    // Once done, we enter loop()
}

void loop() {
    // Everythign in here is repeaded once done
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