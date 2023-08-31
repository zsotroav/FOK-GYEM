#include <Arduino.h>
#include "shiftRegister.h"
#include "driver.h"

/**
 *                                !!! IMPORTANT !!!                               
 * Make sure all settings in config.h are set correctly and according to your setup
 *            Most important values: CFG_PANEL_COUNT and CFG_SERIAL_BAUD
 * 
 *          Do NOT change anything else unless you know what you are doing
 *            to avoid damaging the Arduino, driver PCB, or the displays
 */

// Screen buffer
uint8_t buff[DRV_DATABUFF_SIZE] = {0};

// Buffer for incoming small data
byte inbuff[4] = {0};

// Expected handshake
byte config[3] = {CFG_PANEL_COUNT, DRV_COL_COUNT, DRV_ROW_COUNT};
byte magic_header[4] = {0xAA, 0x55, 0xAA, 0x55};

// Our reply to the handshake
byte message[6] = {CFG_SERIAL_PROT_VER, CFG_PANEL_COUNT, DRV_COL_COUNT, DRV_ROW_COUNT, 0x00, 0x00};

// Connection reference number. Randomized later for each connection
byte CD = 0xAF;


// declare reset function at address 0
void (*resetFunc)(void) = 0; 


// Send the message to accept/deny connection
void send(int garbage) 
{
    // Don't leave data unread as that may cause issues
    if (garbage > 0) Serial.readBytes(inbuff, garbage);

    // Write our response back
    for (int x = 0; x < sizeof(message); x = x + 1)
    {
        Serial.write(message[x]);
    }   
}


void setup()
{
    // Initialize driver
    driver_init();

    // Set buffer and empty the display
    memset(buff, 0, DRV_DATABUFF_SIZE);
    driver_setBuffer(buff, DRV_DATABUFF_SIZE);
    driver_forceWriteScreen();


    // Start serial
    Serial.begin(CFG_SERIAL_BAUD);

    // Wait for connection with proper handshake
    bool wait = true;
    while (wait)
    {
        if (!Serial.available()) continue;
        
        // Read the magic header
        Serial.readBytes(inbuff, 4);
        if (memcmp(inbuff, magic_header, 4) != 0) { send(4); continue; }

        // Read protocol version
        Serial.readBytes(inbuff, 1);
        if (inbuff[0] > 1) { send(3); continue; }

        // Read physical details
        Serial.readBytes(inbuff, 3);
        if (memcmp(inbuff, config, 3) != 0) { send(0); continue; }

        randomSeed(millis());
        CD = (random(0, 256) & 0xFF);

        // If it is what we expect, accept connection
        message[4] = 0xFF;
        message[5] = CD;
        wait = false;
        send(0);
    }

    // Enter loop()...
}

void loop()
{
    if (!Serial.available()) {return;}
    
    // Read 4 byte header
    Serial.readBytes(inbuff, 4);
    if (inbuff[0] != CD) { return; }        // Incorrect connection reference number
    if (inbuff[1] == 0x0F) { resetFunc();}  // Connection close

    // Merge length data
    uint16_t length = ((uint16_t)inbuff[2] << 8) | (uint16_t)inbuff[3];

    // Write full screen
    if (inbuff[1] == 0x11)
    { 
        // Check if sent length is what we'd expect
        if (length != DRV_DATABUFF_SIZE)
        {            
            Serial.write(CD);
            Serial.write(0x05); // Error for bad data
            return;
        }
        
        Serial.readBytes(buff, DRV_DATABUFF_SIZE);
        driver_setBuffer(buff, DRV_DATABUFF_SIZE);
        driver_writeScreen();
    }

    // Write pixel
    else if (inbuff[1] == 0x12)
    { 
        // Check if sent length is what we'd expect
        if (length != 3)
        {            
            Serial.write(CD);
            Serial.write(0x05); // Error for bad data
            return;
        }

        Serial.readBytes(inbuff, 3);

        // We can only set bytes, so 'put' the wanted bit in the current byte
        uint8_t byte = driver_getByteAt(inbuff[0] / 8, inbuff[1]);
        byte ^= (~(-inbuff[2]) ^ byte) & (1UL << (7 - (inbuff[0] % 8)));

        driver_setByteAt(inbuff[0] / 8, inbuff[1], byte);
        driver_writeScreen();
    }

    // Force write full screen
    else if (inbuff[1] == 0x19)
    {
        // Check if sent length is what we'd expect
        if (length != DRV_DATABUFF_SIZE)
        {            
            Serial.write(CD);
            Serial.write(0x05); // Error for bad data
            return;
        }

        Serial.readBytes(buff, DRV_DATABUFF_SIZE);
        driver_setBuffer(buff, DRV_DATABUFF_SIZE);
        driver_forceWriteScreen();
    }

    // Unknown command
    else {
        Serial.write(CD);
        Serial.write(0x04); // Error for unknown command
        return;
    }

    Serial.write(CD);
    Serial.write(0x01); // Success message
}