/*=================================================================
ILI9341 TFT Display test code using  ILI9341_spi library for STM32
  
 using Maple Mini hardware (https://stm32duinoforum.com/forum/wiki_subdomain/index_title_Maple_Mini.html) 
 and Arduino STM32 core (https://github.com/rogerclarkmelbourne/Arduino_STM32) 
 
Original library is from https://github.com/rogerclarkmelbourne/Arduino_STM32/tree/master/STM32F1/libraries/Adafruit_ILI9341_STM
Updated by IF 
2022-05-19
- tested support of hardware SPI for Maple Mini board / STM32 (https://github.com/rogerclarkmelbourne/Arduino_STM32/)
- simplified to use a pointer to SPI object in order to reduce potential problems with redefining Wire etc 
- does NOT need Adafruit_BusIO Library.
 
 Status: works ok

 // hardware setup for Maple Mini  
    //Use SPI_1 (5v tolerant)
    // Maple Mini SS   (NCS)  <-->  PA7  D4 pin no 7   -> TFT CS  (Chip Select or  Slave Synchronisation pin. Signal is managed by ILI9341_spi library to allow running several TFT devices with their own SS pins)
    // Maple Mini SCK  (CLK)  <-->  PA5  D6 pin no 6   -> TFT SCK (Clock for spi serial communication)
    // Maple Mini MISO (SPI)  <-->  PA6  D5 pin no 5   -> TFT SDO (Data pin)
    // Maple Mini MOSI (SDO)  <-->  PA7  D4 pin no 4   -> TFT SDI (Data pin)
    // Maple Mini TFT_DC      <-->  PA13 D22 pin no 22 -> TFT DC (Data/Command indicator pin) 
    // Maple Mini TFT_RST     <-->  PA14 D21 pin no 21 -> TFT RESET (ILI9341 Reset pin. If reset is not used, TFT RS pin shall be it shall be HIGH) 


   //Use SPI_2 
    // Maple Mini SS   (NCS)  <-->  PB12 D31 pin no 31 -> TFT CS  (Chip Select or  Slave Synchronisation pin. Signal is managed by ILI9341_spi library to allow running several TFT devices with their own SS pins)
    // Maple Mini SCK  (CLK)  <-->  PB13 D30 pin no 30 -> TFT SCK (Clock for spi serial communication)
    // Maple Mini MISO (SPI)  <-->  PB14 D29 pin no 29 -> TFT SDO (Data pin)
    // Maple Mini MOSI (SDO)  <-->  PB15 D28 pin no 28 -> TFT SDI (Data pin)
    // Maple Mini TFT_DC      <-->  PA13 D22 pin no 22 -> TFT DC (Data/Command indicator pin) 
    // Maple Mini TFT_RST     <-->  PA14 D21 pin no 21 -> TFT RESET (ILI9341 Reset pin. If reset is not used, TFT RS pin shall be it shall be HIGH)    
   
   
Performance with Maple Mini:
 
      Benchmark                Time (microseconds)
      Screen fill              170769
      Text                     27391
      Lines                    137988
      Horiz/Vert Lines         14947
      Rectangles (outline)     10048
      Rectangles (filled)      354859
      Circles (filled)         69469
      Circles (outline)        111383
      Triangles (outline)      31155
      Triangles (filled)       137365
      Rounded rects (outline)  39659
      Rounded rects (filled)   361880

=================================================================*/ 
/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "SPI.h"
#include <Adafruit_GFX.h>
#include <ILI9341_spi_STM32.h>


// DEfine Pins 
#define TFT_CS 7   // Chip Select or Slave Synchronisation pin 
#define TFT_DC 22    // Data/Command pin
#define TFT_RST 21 //  -1  // Reset pin, -1 if not used

//Setup SPI
  //If you use the first SPI port:
  // No need to do anything, there is already an SPIClass object named SPI which is already available

  //If you use the second SPI port:   
  //SPIClass SPI_2 = SPIClass(2); //Create an instance of the SPI Class named SPI_2 that uses the 2nd SPI Port


//Set up TFT device
	//ILI9341_spi_STM32 tft = ILI9341_spi_STM32(TFT_CS, TFT_DC);
  ILI9341_spi_STM32 tft = ILI9341_spi_STM32(TFT_CS, TFT_DC, TFT_RST, 36000000, &SPI);   //  -> SPI1
	//ILI9341_spi_STM32 tft = ILI9341_spi_STM32(TFT_CS, TFT_DC, TFT_RST, 1000000, &SPI_2);   //  ->SPI2
  //ILI9341_spi_STM32 tft = ILI9341_spi_STM32(TFT_CS, TFT_DC);   //  minimal initialisation  - SPI1, 1Mhz, no RESET pin




void setup() {
	
 //==============================
   Serial.begin(9600);
   //Serial.begin(115200);
   //The program will wait for serial to be ready up to 10 sec then it will continue anyway  
     for (int i=1; i<=10; i++){
          delay(1000);
     if (Serial){
         break;
       }
     }
    Serial.println("Setup() started ");
 //===============================
 
 //start SPI interface
  SPI.begin();
  //SPI_2.begin();
	  
 //start TFT device
  tft.begin();

  
 //Connect to the TFT device 
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 


  //Do tests and record their performance
  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(ILI9341_CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(ILI9341_RED, ILI9341_BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(ILI9341_GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, ILI9341_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, ILI9341_WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F("Done!"));

}


void loop(void) {

//Just rotate text   
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(1000);
  }
}


//====================TESTS==================================

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_BLUE);
  tft.fillScreen(ILI9341_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(ILI9341_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9341_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9341_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}
