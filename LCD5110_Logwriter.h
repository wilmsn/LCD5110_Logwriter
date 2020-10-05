/*********************************************************************
This is a library for the Noika monochrome LCD based on the PCD8544 driver.
It is NOT a complete Library if you are looking for all the features
this display can do! This Library has a very small Memory/RAM footprint (~377Byte)
so you can combine it with other libraries and can still run it on an Arduino UNO.
There is no graphic implemented and only 2 simple fixed fonts (8*8 and 8*6).
Function:
It's just a simple textfield with 8 lines (default). It works like the output 
of the serial monitor. With print/println you will write to the last line.
Example:
	display.print("Test ");
	display.print(56.8);
	display.println("V");
Will produce this output in the last line of the display:
	Test 56.8V
With the next display.print a new line will be written and you will find this text 
on the secondlast line.
**********************************************************************/
#ifndef _LCD5110_LOGWRITER_H_
#define _LCD5110_LOGWRITER_H_

//#define LCD5110_MIN_ROW     0
//#define LCD5110_MAX_ROW     5
//#define LCD5110_MIN_COL     0
//#define LCD5110_MAX_COL     83
//#define LCD5110_ROW         5
//#define LCD5110_COL         83


// Display Modes
#define DISPLAYMODEBLANK      0b00001000
#define DISPLAYMODENORMAL     0b00001100
#define DISPLAYMODEALLSEGON   0b00001001
#define DISPLAYMODEINVERSE    0b00001101

// Instruction Set
#define INSTRUCTIONSET        0b00100000
#define SET_EXT               0b00000001
#define SET_BASIC             0b00000000
#define POWERDOWN             0b00000100
#define POWERUP               0b00000000
#define ADRESS_VERT           0b00000010
#define ADRESS_HORI           0b00000000

#define SET_TEMP              0b00000100
#define SET_BIAS              0b00010000
#define SET_CONTRAST          0b10000000
#define DEFAULT_TEMP          0b00000001
#define DEFAULT_BIAS          0b00000011
#define DEFAULT_CONTRAST      65

// Constants
#define DISPLAY_COL           84
#define BUFFER_ROW            6
// 14*6 = 84
#define BUFFER_COL            14
#define FONT_SMALL_X          5

#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif

#include <stdlib.h>

#include "Arduino.h"
#include "Print.h"
#include <SPI.h>

#include <LCD5110_Logwriter_font.h>
 

class LCD5110 : public Print {
    private:
// Anschlußpinne für das Display
        uint8_t rst;
        uint8_t ce;
        uint8_t dc;
        uint8_t din;
        uint8_t clk;
// ENDE Anschlußpinne für das Display
        char textBuffer[BUFFER_ROW][BUFFER_COL];
        void send(uint8_t data);
		void sendCommand(uint8_t command);
		void sendData(uint8_t data);
        void doReset(void);
		void scrollBuffer(void);
		size_t write(uint8_t c);
		void printBuffer(void);
	
	public:
	    /* Constructor to use the full display */
        LCD5110(uint8_t _rst, uint8_t _ce, uint8_t _dc, uint8_t _din, uint8_t _clk);
#ifdef ESP8266
// ToDo        void begin(uint8_t _sda, uint8_t _scl);
#endif
        /* be sure to call begin() inside your setup block */
		void begin(void);
		/* sets the background to black / inverts all Pixel if it was white before */
		void off(void);
		/* switches the display on */
		void on(void);
		/* Clear the display */
		void clear(void);
};

#endif /* _SSD1306_LOGWRITER_H_ */
