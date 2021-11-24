#include "driver.h"
#include "shiftRegister.h"

#define _BIT_AT(COL, ROW) (_buffer[AT((COL)/8,(ROW))] & (0x80 >> ((COL)%8)))

enum _DRV_ROW_CONTROL_MODE {
	_DRV_ROW_CONTROL_MODE_SINK,
	_DRV_ROW_CONTROL_MODE_SOURCE,
};

/**
 * @brief Stores the display data, in a checkboard compensated form.
 * 
 */
static uint8_t _buffer[DRV_DATABUFF_SIZE];

static shiftReg_t _rowCntrlReg = {
	.PIN_DATA			= CFG_ROW_CONTROL_DATA,
	.PIN_CLK			= CFG_ROW_CONTROL_CLOCK,
	.PIN_STROBE			= CFG_ROW_CONTROL_STROBE,
	.PIN_OUTPUT_EN_POL 	= SHIFTREG_PIN_LOW_POSITIVE,
	.PIN_OUTPUT_EN		= CFG_ROW_CONTROL_OUTPUT
};
static shiftReg_t _colCntrlReg = {
	.PIN_DATA			= CFG_COL_CONTROL_DATA,
	.PIN_CLK			= CFG_COL_CONTROL_CLOCK,
	.PIN_STROBE			= CFG_COL_CONTROL_STROBE,
	.PIN_OUTPUT_EN_POL 	= SHIFTREG_PIN_LOW_POSITIVE,
	.PIN_OUTPUT_EN		= CFG_ROW_CONTROL_OUTPUT
};

uint8_t driver_getByteAt(uint8_t x, uint8_t y) {
	if(x > (DRV_COL_COUNT / 8) || y > DRV_ROW_COUNT) {
		return 0;	//Out of range
	}
	return _buffer[AT(x,y)] ^= ((y%2) ? 0xAA : 0x55);	//Checkboard compensation based on the row
}

void driver_setByteAt(uint8_t x, uint8_t y, uint8_t byte) {
	if(x > (DRV_COL_COUNT / 8) || y > DRV_ROW_COUNT) {
		return;	//Out of range
	}
	//Apply checkboard pattern based on the row
	_buffer[AT(x,y)] = (y%2) ? byte ^ 0xAA : byte ^ 0x55;
}

void driver_getBuffer(uint8_t * data) {
	for(uint8_t i = 0; i < DRV_COL_COUNT/8*CFG_PANEL_COUNT; i++) {
		for(uint8_t j = 0; j < DRV_ROW_COUNT; j++) {
			if(j%2 == 0) {
				data[AT(i,j)] = _buffer[AT(i,j)] ^ 0xAA;
			} else {
				data[AT(i,j)] = _buffer[AT(i,j)] ^ 0x55;
			}
		}
	}
}

void driver_setBuffer(const uint8_t * data, uint8_t size) {
	if(size < DRV_DATABUFF_SIZE) {
		return;
	}
	//memcpy(_buffer, data, DRV_DATABUFF_SIZE);
	//Apply the checkboard correction to the user data.
	for(uint8_t i = 0; i < DRV_COL_COUNT/8*CFG_PANEL_COUNT; i++) {
		for(uint8_t j = 0; j < DRV_ROW_COUNT; j++) {
			if(j%2 == 0) {
				_buffer[AT(i,j)] = data[AT(i,j)] ^ 0xAA;
			} else {
				_buffer[AT(i,j)] = data[AT(i,j)] ^ 0x55;
			}
		}
	}
}

void driver_init() {
//Modes
	pinMode(CFG_ROW_CONTROL_DATA, OUTPUT);
	pinMode(CFG_ROW_CONTROL_CLOCK, OUTPUT);
	pinMode(CFG_ROW_CONTROL_STROBE, OUTPUT);
	pinMode(CFG_ROW_CONTROL_OUTPUT, OUTPUT);

	pinMode(CFG_COL_CONTROL_DATA, OUTPUT);
	pinMode(CFG_COL_CONTROL_CLOCK, OUTPUT);
	pinMode(CFG_COL_CONTROL_STROBE, OUTPUT);
	pinMode(CFG_COL_CONTROL_OUTPUT_12V, OUTPUT);
	pinMode(CFG_COL_CONTROL_OUTPUT_GND, OUTPUT);

//Deault values
	//Output enables
	digitalWrite(CFG_ROW_CONTROL_OUTPUT, HIGH);
	digitalWrite(CFG_COL_CONTROL_OUTPUT_12V, CFG_COL_CONTROL_OUTPUT_12V_OFF);
	digitalWrite(CFG_COL_CONTROL_OUTPUT_GND, CFG_COL_CONTROL_OUTPUT_GND_OFF);

	//Others
	digitalWrite(CFG_ROW_CONTROL_DATA, LOW);
	digitalWrite(CFG_ROW_CONTROL_CLOCK, LOW);
	digitalWrite(CFG_ROW_CONTROL_STROBE, LOW);

	digitalWrite(CFG_COL_CONTROL_DATA, LOW);
	digitalWrite(CFG_COL_CONTROL_CLOCK, LOW);
	digitalWrite(CFG_COL_CONTROL_STROBE, LOW);

//Prefill column control registers
	for(uint8_t i = 0; i < DRV_COL_COUNT/8; i++) {
		shiftReg_sendData(&_colCntrlReg, 0x00, 8);
	}
}

/**
 * @brief Auxiliary function for driver_writeScreen(). Sends out the row control data to the shift registers.
 * 
 * @param mode specifies the drive mode - sink or source.
 * @param row is the current row index.
 * @param panel is the current panel number.
 */
static void _driver_setRowControl(enum _DRV_ROW_CONTROL_MODE mode, uint8_t row, uint8_t panel) {
	
//Leading zeros
	for(uint8_t i = 0; i < CFG_PANEL_COUNT - 1 - panel; i++) {
		shiftReg_sendData(&_rowCntrlReg, 0x00, 8);
		shiftReg_sendData(&_rowCntrlReg, 0x00, 8);
	}

//Panel data
	if(mode == _DRV_ROW_CONTROL_MODE_SINK) {
		shiftReg_sendData(&_rowCntrlReg, DRV_ROW_SHIFT_BASE << row, 8);
		shiftReg_sendData(&_rowCntrlReg, 0x00, 8);				
	} else {
		shiftReg_sendData(&_rowCntrlReg, 0x00, 8);
		shiftReg_sendData(&_rowCntrlReg, DRV_ROW_SHIFT_BASE << row, 8);
	}

//Trailing
	for(uint8_t i = 0; i < panel; i++) {
		shiftReg_sendData(&_rowCntrlReg, 0x00, 8);
		shiftReg_sendData(&_rowCntrlReg, 0x00, 8);
	}

//Latch the data
	shiftReg_latchData(&_rowCntrlReg);
}

/**
 * @brief sets the pixel data contained in _buffer on the FOK-GYEM display.
 * 
 * The driving circuit uses a bridge-like configuration. The rows and columns of the display are separately controlled.
 * Each row and column could be set as a source (15V) or a sink (GND). A pixel is turned, when the corresponding row and column
 * are activated, with opposite polarity (sink - source or source sink). Changing the polarity changes the on-off state of the pixel.
 * Only one row & column can be active at the same time, otherwise the circuits would be overloaded.
 * 
 * The sink-source modes are controlled by having two types of shift registers: a driver and a sink. For the row control, these
 * registers connected series in an alternating manner: 1.panel (source - sink) - 2.panel (source - sink).
 * For the column control, they are separated into their groups: (source source source) - (sink sink sink). 
 * 
 * The control method
 * 
 * The control principle uses the main advantage of the shift registers. First, the column control shift register chain is loaded
 * with one bit at the beginning. After that we send out the row data into all row registers, which contains exactly one active row.
 * Then we start shifting the column control shift registers, and depending on the current row-column index, we turn on the outputs of
 * the drivers to flick the display pixel. For the reverse flick, we do the same thing, with different row data.
 */
void driver_writeScreen() {

	for(uint8_t row = 0; row < DRV_ROW_COUNT; row++) {

		for(uint8_t panel = 0; panel < CFG_PANEL_COUNT; panel++) {
			//*********** SET ***********	
			_driver_setRowControl(_DRV_ROW_CONTROL_MODE_SINK, row, panel);	
			
			shiftReg_sendData(&_colCntrlReg, 0x01, 1); //Putting the first bit into the first shift register
			//COL data setup SOURCE
			for(uint8_t col = DRV_COL_COUNT; col > 0 ; col--) {
				if(_BIT_AT(col-1+DRV_COL_COUNT*panel, row)) { //Current pin is set
					shiftReg_latchData(&_colCntrlReg);
					shiftReg_outputSet(&_rowCntrlReg, true);
					digitalWrite(CFG_COL_CONTROL_OUTPUT_12V, CFG_COL_CONTROL_OUTPUT_12V_ON);
					delay(1);
					digitalWrite(CFG_COL_CONTROL_OUTPUT_12V, CFG_COL_CONTROL_OUTPUT_12V_OFF);
					shiftReg_outputSet(&_rowCntrlReg, false);
				}
				shiftReg_shiftIn(&_colCntrlReg);
			}
			
			//********** UNSET **********
			_driver_setRowControl(_DRV_ROW_CONTROL_MODE_SOURCE, row, panel);
			//COL data setup SINK
			for(uint8_t col = DRV_COL_COUNT; col > 0; col--) {
				
				if(_BIT_AT(col-1+DRV_COL_COUNT*panel, row) == 0) { //Current pin is not set
					shiftReg_latchData(&_colCntrlReg);
					shiftReg_outputSet(&_rowCntrlReg, true);
					digitalWrite(CFG_COL_CONTROL_OUTPUT_GND, CFG_COL_CONTROL_OUTPUT_GND_ON);
					delay(1);
					digitalWrite(CFG_COL_CONTROL_OUTPUT_GND, CFG_COL_CONTROL_OUTPUT_GND_OFF);
					shiftReg_outputSet(&_rowCntrlReg, false);
				}
				shiftReg_shiftIn(&_colCntrlReg);
			}
		}
	}
}
