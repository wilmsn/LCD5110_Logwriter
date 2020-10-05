/*********************************************************************
This is a library for our Nokia 5110 Display based on the PCD8544 driver
More details in header file!
*********************************************************************/

#include "LCD5110_Logwriter.h"

byte posInLine = 0;
enum myfont_t { font_8x8, font_8x6 };
myfont_t myfont;

LCD5110::LCD5110(uint8_t _rst, uint8_t _ce, uint8_t _dc, uint8_t _din, uint8_t _clk) {
    rst = _rst;
    ce  = _ce;
    dc  = _dc;
    din = _din;
    clk = _clk;

    pinMode(rst, OUTPUT);
    pinMode(ce,  OUTPUT);
    pinMode(dc,  OUTPUT);
    pinMode(din, OUTPUT);
    pinMode(clk, OUTPUT);
 }

void LCD5110::begin(){
    doReset();
    sendCommand(INSTRUCTIONSET | POWERUP);
    sendCommand(INSTRUCTIONSET | SET_EXT);
    // set temp control
    sendCommand(SET_TEMP | DEFAULT_TEMP);
    // set bias
    sendCommand(SET_BIAS | DEFAULT_BIAS);
    // set contrast
    sendCommand(SET_CONTRAST | DEFAULT_CONTRAST); 
    sendCommand(INSTRUCTIONSET | SET_BASIC );
    // Displaymode normal
    sendCommand(DISPLAYMODENORMAL);     
    on();
    clear();
}

void LCD5110::doReset(){
    digitalWrite(rst, LOW);
    delay(500);
    digitalWrite(rst, HIGH);
}

void LCD5110::sendCommand(uint8_t command){
    digitalWrite(dc, LOW);
    send(command);
}

void LCD5110::sendData(uint8_t data){
    digitalWrite(dc, HIGH);
    send(data);
}

void LCD5110::send(uint8_t data){
    digitalWrite(ce, LOW);
    shiftOut(din, clk, MSBFIRST, data);
    digitalWrite(ce, HIGH);
}

void LCD5110::clear(){
    sendCommand(0b01000000); // First line 
    sendCommand(0b10000000); // First Column
    for (uint8_t row=0; row < BUFFER_ROW; row++) for (uint8_t col=0; col < DISPLAY_COL; col++) sendData(0x00);
    for (uint8_t row=0; row < BUFFER_ROW; row++) for (uint8_t col=0; col < BUFFER_COL; col++) textBuffer[row][col] = ' ';
}

void LCD5110::on(){
    sendCommand(INSTRUCTIONSET | POWERUP);
}

void LCD5110::off(){
    sendCommand(INSTRUCTIONSET | POWERDOWN);
}
 
void LCD5110::printBuffer(void) {
    sendCommand(0b01000000); // line 
    sendCommand(0b10000000); // First Column
    for (byte row=0; row < BUFFER_ROW-1; row++) {
		for (byte col=0; col < BUFFER_COL; col++) {
			sendData(0);
			for(byte i = 0; i < 5; i++) {
				sendData(pgm_read_byte(&myFont8x5[textBuffer[row][col] - 32][i]));
			}
		}
	}
}

void LCD5110::scrollBuffer(void) {
	for(uint8_t row=1; row < BUFFER_ROW; row++) {
		for(uint8_t col=0; col < BUFFER_COL; col++) {
			textBuffer[row-1][col] = textBuffer[row][col];
		}
	}
	for(byte col=0; col < BUFFER_COL; col++) {
		textBuffer[BUFFER_ROW-1][col]=' ';
	}
	printBuffer(); 
}
	
size_t LCD5110::write(uint8_t c) {
    size_t retval=0;
    //Cursor im Display setzen
    sendCommand(0b01000000 + 5); // last line 
    sendCommand(0b10000000 + (posInLine * 6) ); // Column
	// CF und LF sorgen für Zeilenumbruch
	if (c == 10 || c == 13) {
       posInLine=0;	   
    }
	if (c > 31 && c < 127) {
		if ( posInLine == 0 ) {
            for(uint8_t col=0; col < DISPLAY_COL; col++) sendData(0);
			scrollBuffer();
            sendCommand(0b01000000 + 5); // last line 
            sendCommand(0b10000000); // First Column            
		}
		retval = 1;
		if (posInLine < BUFFER_COL) {
			textBuffer[BUFFER_ROW-1][posInLine]=c;
			sendData(0);
			for(byte i = 0; i < 5; i++) {
				sendData(pgm_read_byte(&myFont8x5[c - 32][i]));
			}
		}
		posInLine++;
	}
#ifdef ESP8266
    delay(0);
#endif    
	return retval; 
}

