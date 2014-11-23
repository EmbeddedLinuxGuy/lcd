/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define leftPin 8
#define rightPin 7
#define buttonPin 6

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

#define FIG_HEIGHT 8
#define FIG_WIDTH  8

#define YMAX 40
#define XMAX 76

static const unsigned char PROGMEM fig_bmp[] =
{ B00110000,
  B00110000,
  B00100000,
  B01111100,
  B10110010,
  B00110000,
  B01001000,
  B11101110
};

void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.clearDisplay();   // clears the screen and buffer
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  int i;
  for (i=1; i < XMAX+8; ++i) {
    display.drawPixel(i, YMAX+8, BLACK);
  }
}

int x = 0;
int y = YMAX-1;
int dx = 1;
int g = 0;
int dy = 0;




void loop() {
 int delay_count = 50;
 int base = YMAX;

 display.clearDisplay();
 display.drawBitmap(x, y,  fig_bmp, FIG_WIDTH, FIG_HEIGHT, 1);
  int i;
  for (i=0; i < XMAX/2; ++i) {
    display.drawPixel(i, YMAX+7, BLACK);
  }
  for (i=XMAX/2; i < XMAX+8; ++i) {
    display.drawPixel(i, YMAX+8-10, BLACK);
  }
 display.display();
 x = (x+dx);
 if (x > XMAX/2) { base = YMAX-10; } else { base = YMAX; } 
 y = (y+dy);
 if (y < 0) { y = 0; dy = 2; }

 if (y > base) { y = base; dy = 0; }
 if (x < 0) { x = 0; }
 else if (x > XMAX) { x = XMAX; }
 if (g > 1) {
    g--; if (g < 5) { dy = 1; } if (g == 0) { dy = 0; }
 } else {
   if (y < base) { dy = 2; }
 }

 int buttonState;
 for (int i=0; i < 100*delay_count; ++i) {
   buttonState = digitalRead(buttonPin);
   if (buttonState == LOW) {
	if (dy == 0) {
	  dy = -2; g = 10;
	  break;
	}
   }
   int leftState = digitalRead(leftPin);
   int rightState = digitalRead(rightPin);
   if (leftState == LOW && rightState == HIGH) {
	dx = 1;
   } else if (leftState == HIGH && rightState == LOW) {
	dx = -1;
   } else {
	dx = 0;
   }
 }
}



