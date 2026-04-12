#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>

#include <SPI.h>

//QT Py ESP32-C3 pin mappings
// /CS is tied to GND on the breakout board (display always selected).
// If sharing the SPI bus with other devices, connect /CS to a GPIO and replace U8X8_PIN_NONE.
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 7, /* cs=*/ U8X8_PIN_NONE, /* dc=*/ 8, /* reset=*/ 5);

// How many internal neopixels do we have? some boards have more than one!
#define NUMPIXELS        1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(20); // not so bright

  u8g2.begin();
}


// the loop routine runs forever:
void loop() {
  // say something on serial port
  Serial.println("Hello!");
  
  // set color to red and draw some text
  pixels.fill(0xFF0000);  pixels.show();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.firstPage();
  do {
    u8g2.drawStr(0,15,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(500); // wait half a second-

  // set color to green and draw a box
  pixels.fill(0x00FF00);  pixels.show();
  u8g2.firstPage();
  do {
   u8g2.drawStr(0,15,"Hello World!");
   u8g2.drawBox(25,25,32,32);
  } while ( u8g2.nextPage() );
  delay(500); // wait half a second

  // set color to blue and draw an open box
  pixels.fill(0x0000FF);  pixels.show();
  delay(500); // wait half a second
  u8g2.firstPage();
  do {
    u8g2.drawStr(0,15,"Hello World!");
    u8g2.drawBox(25,25,32,32);
    u8g2.drawFrame(60,25,32,32);
  } while ( u8g2.nextPage() );
  delay(5000);
 }