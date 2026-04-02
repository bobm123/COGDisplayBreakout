//----------------------------------------------------------------------
// ERC12864_SPIDemo.ino
//
// Port of the ERC12864-11 tutorial demo (ERC12864-11-4SPI.c) to
// Arduino + U8g2. The original code targeted an STC12LE5A60S2 (8051)
// with bit-banged 4-wire SPI. Here, U8g2 handles SPI transport,
// display initialization, fonts, and drawing primitives.
//
// Target board: Adafruit QT Py ESP32-C3
// Display:      ERC12864-11 (ST7567, 128x64, 4-wire SPI)
// Libraries:    U8g2lib, SPI
//----------------------------------------------------------------------

#include <U8g2lib.h>
#include <SPI.h>

// --- Display constructor (matches other sketches in this project) ---
// Pin mapping for QT Py ESP32-C3:
//   clock = GPIO 10 (SCK)
//   data  = GPIO 7  (MO)
//   cs    = GPIO 6  (SCL, repurposed)
//   dc    = GPIO 8  (MI, repurposed as A0/DC)
//   reset = GPIO 5  (SDA, repurposed)
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0,
    /* clock=*/ 10, /* data=*/ 7, /* cs=*/ 6, /* dc=*/ 8, /* reset=*/ 5);

// --- Contrast (from tutorial: default EV level = 24) ---
uint8_t contrast = 24;

// // --- Contrast adjustment via buttons (uncomment and wire buttons) ---
// // BUTTON_UP_PIN:   GPIO 4 (A0) - connect a pushbutton to GND
// // BUTTON_DOWN_PIN: GPIO 9 (BOOT button on QT Py, active LOW)
// #define BUTTON_UP_PIN    4
// #define BUTTON_DOWN_PIN  9
//
// void setupButtons() {
//   pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
//   pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
// }
//
// // Ported from adj_Contrast() in the tutorial C code.
// // The original used raw SPI commands (0x81, value) with auto-repeat;
// // here we use u8g2.setContrast() and simple debounce.
// void adjustContrast() {
//   bool changed = false;
//   if (digitalRead(BUTTON_UP_PIN) == LOW) {
//     if (contrast < 63) contrast++;
//     changed = true;
//   }
//   if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
//     if (contrast > 0) contrast--;
//     changed = true;
//   }
//   if (changed) {
//     u8g2.setContrast(contrast);
//     // Show current contrast value on display
//     u8g2.clearBuffer();
//     u8g2.setFont(u8g2_font_ncenB14_tr);
//     char buf[16];
//     snprintf(buf, sizeof(buf), "Contrast: %d", contrast);
//     u8g2.drawStr(0, 30, buf);
//     u8g2.sendBuffer();
//     delay(150);  // simple debounce
//   }
// }

// --- Sample XBM bitmap (16x16 smiley face) ---
// Demonstrates drawXBMP(), replacing the tutorial's DrawBMP(pic1..4).
#define SMILEY_WIDTH  16
#define SMILEY_HEIGHT 16
static const unsigned char smiley_xbm[] PROGMEM = {
  0xC0, 0x03, 0x30, 0x0C, 0x08, 0x10, 0x04, 0x20,
  0x04, 0x20, 0x22, 0x44, 0x22, 0x44, 0x02, 0x40,
  0x02, 0x40, 0x12, 0x48, 0x24, 0x24, 0xC4, 0x23,
  0x08, 0x10, 0x08, 0x10, 0x30, 0x0C, 0xC0, 0x03
};

//----------------------------------------------------------------------
// Demo 1: Text display
// Ports the tutorial's ShowString() calls with 16px and 8px fonts.
// The original showed Chinese characters on the top rows and ASCII
// strings below. We replace the Chinese text with a title screen.
//----------------------------------------------------------------------
void demoText() {
  u8g2.clearBuffer();

  // Large font — replaces SIZE=16 / ShowString()
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(4, 16, "ERC12864-11");

  // Medium font — replaces SIZE=8 / ShowString()
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(4, 32, "ST7567 SPI Demo");
  u8g2.drawStr(4, 44, "U8g2 + ESP32-C3");
  u8g2.drawStr(4, 56, "Hello World!");

  u8g2.sendBuffer();
  delay(2000);
}

//----------------------------------------------------------------------
// Demo 2: Graphics primitives
// Demonstrates the drawing functions that replace the tutorial's
// direct page/column writes.
//----------------------------------------------------------------------
void demoGraphics() {
  u8g2.clearBuffer();

  // Filled rectangle (drawBox)
  u8g2.drawBox(4, 4, 24, 24);

  // Hollow rectangle (drawFrame)
  u8g2.drawFrame(34, 4, 24, 24);

  // Rounded box
  u8g2.drawRBox(64, 4, 28, 24, 6);

  // Circle (outline)
  u8g2.drawCircle(16, 48, 12, U8G2_DRAW_ALL);

  // Disc (filled circle)
  u8g2.drawDisc(46, 48, 12, U8G2_DRAW_ALL);

  // Triangle
  u8g2.drawTriangle(68, 60, 78, 34, 88, 60);

  // Lines
  u8g2.drawLine(96, 4, 124, 60);
  u8g2.drawLine(96, 60, 124, 4);

  u8g2.sendBuffer();
  delay(2000);
}

//----------------------------------------------------------------------
// Demo 3: Bitmap display
// Ports the concept of DrawBMP(pic1..4) from the tutorial.
// The original showed 4 full-screen bitmaps; here we show one small
// XBM bitmap tiled/centered to demonstrate drawXBMP().
//----------------------------------------------------------------------
void demoBitmap() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(0, 10, "XBM Bitmap Demo:");

  // Draw the smiley at several positions
  u8g2.drawXBMP(8, 20, SMILEY_WIDTH, SMILEY_HEIGHT, smiley_xbm);
  u8g2.drawXBMP(32, 20, SMILEY_WIDTH, SMILEY_HEIGHT, smiley_xbm);
  u8g2.drawXBMP(56, 20, SMILEY_WIDTH, SMILEY_HEIGHT, smiley_xbm);
  u8g2.drawXBMP(80, 20, SMILEY_WIDTH, SMILEY_HEIGHT, smiley_xbm);
  u8g2.drawXBMP(104, 20, SMILEY_WIDTH, SMILEY_HEIGHT, smiley_xbm);

  // Draw a larger version using the same bitmap at bottom
  u8g2.drawXBMP(56, 44, SMILEY_WIDTH, SMILEY_HEIGHT, smiley_xbm);

  u8g2.sendBuffer();
  delay(2000);
}

//----------------------------------------------------------------------
// Demo 4: Line/dot test patterns
// Ports Show_line_dot() from the tutorial, which displayed alternating
// stripe and checkerboard patterns using Show(0x55,0xaa) etc.
// Here we recreate similar patterns using drawing primitives.
//----------------------------------------------------------------------
void demoPatterns() {
  // Pattern 1: vertical stripes (odd columns)
  // Tutorial: Show(0x55, 0xaa)
  u8g2.clearBuffer();
  for (int x = 0; x < 128; x += 2) {
    u8g2.drawVLine(x, 0, 64);
  }
  u8g2.sendBuffer();
  delay(1000);

  // Pattern 2: vertical stripes (even columns)
  // Tutorial: Show(0xaa, 0x55)
  u8g2.clearBuffer();
  for (int x = 1; x < 128; x += 2) {
    u8g2.drawVLine(x, 0, 64);
  }
  u8g2.sendBuffer();
  delay(1000);

  // Pattern 3: horizontal stripes (odd rows)
  // Tutorial: Show(0x55, 0x55)
  u8g2.clearBuffer();
  for (int y = 0; y < 64; y += 2) {
    u8g2.drawHLine(0, y, 128);
  }
  u8g2.sendBuffer();
  delay(1000);

  // Pattern 4: horizontal stripes (even rows)
  // Tutorial: Show(0xaa, 0xaa)
  u8g2.clearBuffer();
  for (int y = 1; y < 64; y += 2) {
    u8g2.drawHLine(0, y, 128);
  }
  u8g2.sendBuffer();
  delay(1000);

  // Pattern 5: left half filled
  // Tutorial: Show(0xff, 0x00)
  u8g2.clearBuffer();
  u8g2.drawBox(0, 0, 64, 64);
  u8g2.sendBuffer();
  delay(1000);

  // Pattern 6: right half filled
  // Tutorial: Show(0x00, 0xff)
  u8g2.clearBuffer();
  u8g2.drawBox(64, 0, 64, 64);
  u8g2.sendBuffer();
  delay(1000);
}

//----------------------------------------------------------------------
// setup()
// Ports Initial() from the tutorial. U8g2's begin() handles the full
// ST7567 init sequence (soft reset, power control, booster, bias, etc.)
// We just set contrast to match the tutorial's default EV level of 24.
//----------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.setContrast(contrast);

  // // Uncomment to enable contrast buttons:
  // setupButtons();

  Serial.println("ERC12864 SPI Demo started");
}

//----------------------------------------------------------------------
// loop()
// Ports the tutorial's main while(1) demo loop.
//----------------------------------------------------------------------
void loop() {
  demoText();       // text at two font sizes
  demoGraphics();   // boxes, frames, circles, lines, triangle
  demoBitmap();     // XBM bitmap display
  demoPatterns();   // line/dot test patterns

  // // Uncomment to poll contrast buttons between demo cycles:
  // adjustContrast();
}
