#include "serialCom.h"

static HardwareSerial* _SerialInst;

void sCom_init(HardwareSerial* SerialInstance, unsigned long baudrate) {
	_SerialInst = SerialInstance;
	_SerialInst->end();
	_SerialInst->begin(baudrate);
	_SerialInst->setTimeout(500);
}

void sCom_waitForData(uint8_t* buffer, uint16_t size) {
	int i = 0;
    while(i < size) {
        if(_SerialInst->available() > 0) {
            buffer[i] = _SerialInst->read();
            i++;
        }
    }
}