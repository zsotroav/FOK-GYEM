#include <Arduino.h>
#include "shiftRegister.h"
#include "driver.h"

// Screen buffer
uint8_t buff[DRV_DATABUFF_SIZE] = {0}; 

// Buffer for incoming small data
byte inbuff[8] = {0}; 
// Expected handshake
byte handshake[8] = {0xAA, 0x55, 0xAA, 0x55, CFG_SERIAL_PROT_VER, CFG_PANEL_COUNT, 0x18, 0x07}; 
// Our reply to the handshake
byte message[6] = {CFG_SERIAL_PROT_VER, CFG_PANEL_COUNT, 0x18, 0x07, 0x00, 0x00}; 

// Connection reference number. Randomized later for each connection 
byte CD = 0xAF;

void setup()
{
    // Initialize driver
    driver_init();

    // Set buffer and empty the display
    memset(buff, 0, DRV_DATABUFF_SIZE);
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_forceWriteScreen();

    // Start serial
    Serial.begin(9600);
    randomSeed(millis());

    // Wait for connection with proper handshake
    bool wait = true;
    while (wait)
    {
        if (Serial.available())
        {
            // Read the handshake
            Serial.readBytes(inbuff, 8);

            CD = (random(0, 256) & 0xFF);

            // If it is what we expect, good.
            if (memcmp(inbuff, handshake, 8) == 0)
            {
                message[4] = 0xFF;
                message[5] = CD;
                wait = false;
            }

            // Write our response back
            for (int x = 0; x < sizeof(message); x = x + 1)
            {
                Serial.write(message[x]);
            }
        }
    }

    // Enter loop()...
}

void (*resetFunc)(void) = 0; // declare reset function at address 0

void loop()
{
    if (Serial.available()){
        Serial.readBytes(inbuff, 4);
        if (inbuff[0] != CD) { return; }        // Incorrect connection reference number
        if (inbuff[1] == 0x0F) { resetFunc(); } // Connection close

        if (inbuff[1] == 0x11)
        { // Write full screen
            // Merge length data
            uint16_t got = ((uint16_t)inbuff[2] << 8) | (uint16_t)inbuff[3];

            // Check if sent length is what we'd expect
            if (got == DRV_DATABUFF_SIZE){
                Serial.readBytes(buff, DRV_DATABUFF_SIZE);
                driver_setBuffer(buff, DRV_DATABUFF_SIZE);
                driver_writeScreen();
            } else {
                Serial.write(CD);
                Serial.write(0x05); // Error for bad data
                return;
            }
        }
        else if (inbuff[1] == 0x12)
        { // Write pixel
            Serial.readBytes(inbuff, 3);

            // We can only set bytes, so 'put' the wanted bit in the current byte
            uint8_t byte = driver_getByteAt(inbuff[0] / 8, inbuff[1]);
            byte ^= (~(-inbuff[2]) ^ byte) & (1UL << (7 - (inbuff[0] % 8)));

            driver_setByteAt(inbuff[0] / 8, inbuff[1], byte);
            driver_writeScreen();
        } 
        else if (inbuff[1] == 0x19)
        { // Force write full screen
            // Merge length data
            uint16_t got = ((uint16_t)inbuff[2] << 8) | (uint16_t)inbuff[3]; 

            // Check if sent length is what we'd expect
            if (got == DRV_DATABUFF_SIZE){
                Serial.readBytes(buff, DRV_DATABUFF_SIZE);
                driver_setBuffer(buff, DRV_DATABUFF_SIZE);
                driver_forceWriteScreen();
            } else {
                Serial.write(CD);
                Serial.write(0x05); // Error for bad data
                return;
            }
        }

        Serial.write(CD);
        Serial.write(0x01); // Success message
    }
}