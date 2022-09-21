#include <Arduino.h>
#include "shiftRegister.h"
#include "driver.h"
#include "serialCom.h"

uint8_t buff[DRV_DATABUFF_SIZE] = {0};

void setup() {
    sCom_init(&Serial, 115200);
    driver_init();
    memset(buff, 0, DRV_DATABUFF_SIZE);
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_forceWriteScreen();
}

void loop() {
    sCom_waitForData(buff, DRV_DATABUFF_SIZE);
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_writeScreen();
    while(Serial.available()) { //Remove any remaining or corrupted serial data from the buffer
        Serial.read();
    }
    Serial.println("print_finished");    
}