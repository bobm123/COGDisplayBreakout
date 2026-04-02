# COG Display Breakout

A breakout board for the **ERC12864FSF-11** COG LCD module (0.96", 128x64, ST7567 controller, 4-wire SPI). The display's 12-pin FFC cable (0.6mm pitch, non-standard) is soldered directly to pads on the breakout PCB, which routes signals to a breadboard-friendly 2.54mm pin header with on-board charge pump capacitors and backlight drive circuit.

Designed to match the Adafruit 0.96" OLED module form factor (~27x27mm, 4 mounting holes) as a drop-in replacement.

COG (Chip On Glass) LCD technology differs from the OLED displays commonly found on breakout boards of this size. COG LCDs are daylight readable like e-paper, but with simpler hardware and software requirements. However, they may need a backlight in low-light conditions, so the breakout includes drive circuitry for the display's built-in LED backlight.

![KiCad 3D render of the breakout board](images/ScreenShot.png)


## Specifications

| Parameter | Value |
|-----------|-------|
| Display | [ERC12864FSF-11](https://www.buydisplay.com/0-96-inch-low-cost-white-128x64-graphic-cog-lcd-display-st7567-spi) (128x64, monochrome) |
| Controller | ST7567 |
| Interface | 4-wire SPI |
| Supply voltage | 3.3V only |
| Board size | ~27 x 27mm |
| Mounting | 4x M2 holes, Adafruit OLED compatible |

## Repository Structure

```
ArduinoCode/                             Example sketches for ESP32-C3
COGDisplayBreakout.pretty/               Custom footprint library
COGDisplayBreakout.3dmodels/             Custom 3D models
docs/                                    Datasheets, reference designs, tutorials
COGDisplayBreakout.kicad_sch             Schematic
COGDisplayBreakout.kicad_pcb             PCB layout
COGDisplayBreakout.kicad_sym             Custom symbol library
README.md
```

---

## Schematic and Board Layout

The KiCad 9 project files are at the repository root.

### Circuit Overview

The schematic connects the ERC12864FSF-11 display to an 8-pin SPI header (J2) with supporting passives:

**Header pinout (J2 - SPI_Header):**

| Pin | Signal | Description |
|-----|--------|-------------|
| 1 | GND | Ground |
| 2 | VDD | +3.3V supply |
| 3 | SCL | SPI clock |
| 4 | SDA | SPI data (MOSI) |
| 5 | /CS | Chip select (active low) |
| 6 | A0 | Data/command select (DC) |
| 7 | /RST | Reset (active low) |
| 8 | 3V0 | 3.3V (unused, reserved for future LED control) |

**Passive components:**

| Ref | Value | Purpose |
|-----|-------|---------|
| R1 | 20R | Backlight LED current-limiting resistor |
| C1 | 10uF | Charge pump capacitor (V0 to XV0) |
| C2 | 100nF | VDD bypass capacitor |
| C3 | 1uF | Charge pump capacitor (V0 to GND) |
| C4 | 1uF | VG decoupling capacitor |

The ST7567's built-in voltage booster generates the LCD bias voltage using external capacitors C1, C3, and C4. No regulator or level shifting is included; the board is 3.3V only.

### PCB

2-layer board, ~27x27mm. The display's 12-pin FFC cable (0.6mm pitch) is soldered directly to SMD pads on the back side; the display module mounts flat against the front. Four M2 mounting holes at the corners match the Adafruit OLED module footprint.

### Board Revisions

- **V0.1** (March 2026) - Initial prototype. Known issues: C1 wiring error, missing VG capacitor. See `AssemblyNotes.txt` for workarounds.
- **V0.2** (March 2026) - Fixed capacitor circuit. Known issue: backlight A/K pads swapped. See `AssemblyNotes.txt`.

Gerber files: V0.1 in `COGDisplayBreakout-Gerbers-20260303.zip`, V0.2 in `Gerbers20260323/`.

---

## Custom ERC12864FSF-11 KiCad Library

The project includes a custom schematic symbol, PCB footprint, and 3D model for the ERC12864FSF-11 display module. These are in project-local libraries referenced via `sym-lib-table` and `fp-lib-table`.

### Schematic Symbol

**File:** `COGDisplayBreakout.kicad_sym`
**Symbol name:** `ERC12864FSF-11`

12-pin FFC cable (0.6mm pitch, direct solder) plus 2 backlight pins:

| Pin | Name | Type | Description |
|-----|------|------|-------------|
| 1 | ~{CS} | Input | Chip select (active low) |
| 2 | ~{RST} | Input | Reset (active low) |
| 3 | A0 | Input | Data/command select |
| 4 | SCL | Input | SPI clock |
| 5 | SDA | Input | SPI data (MOSI) |
| 6 | VDD | Power | +3.3V supply |
| 7 | VSS | Power | Ground |
| 8 | V0 | Passive | Contrast voltage (booster output) |
| 9 | XV0 | Passive | Booster capacitor tap |
| 10 | NC | - | Not connected |
| 11 | NC | - | Not connected |
| 12 | VG | Passive | Negative voltage for LCD bias |
| A | A | Passive | Backlight LED anode |
| K | K | Passive | Backlight LED cathode |

Note: The backlight pins (A, K) are separate solder tabs on the display module, not part of the 12-pin FFC cable.

### PCB Footprint

**File:** `COGDisplayBreakout.pretty/ERC12864FSF-11_COG_LCD.kicad_mod`

- Module outline: 27.30 x 21.50mm (landscape)
- 12 SMD pads at 0.6mm pitch for direct-solder FFC cable connection
- 2 through-hole oval pads for backlight tabs (A and K)
- Display active area marked on silkscreen

### 3D Model

**Directory:** `COGDisplayBreakout.3dmodels/`

| File | Format | Description |
|------|--------|-------------|
| `ERC12864FSF-11.step` | STEP | Solid body model for KiCad 3D viewer |
| `ERC12864FSF-11_COG_LCD.wrl` | VRML | Lightweight preview model |

The STEP model is referenced by the footprint and renders in KiCad's 3D viewer.

---

## Arduino Code

Example sketches are in `ArduinoCode/`, targeting the **Adafruit QT Py ESP32-C3** with the **U8g2** graphics library.

### Dependencies

- [U8g2](https://github.com/olikraus/u8g2) - Monochrome graphics library (handles ST7567 initialization, SPI transport, fonts, and drawing)
- SPI (built-in)

Install U8g2 via the Arduino IDE Library Manager.

### Pin Mapping (QT Py ESP32-C3 to breakout header)

| Display Signal | Header Pin | QT Py Pin | ESP32-C3 GPIO |
|----------------|-----------|-----------|---------------|
| GND | 1 | GND | - |
| VDD | 2 | 3V3 | - |
| SCL (clock) | 3 | SCK | 10 |
| SDA (data) | 4 | MO | 7 |
| /CS | 5 | SCL* | 6 |
| A0 (DC) | 6 | MI* | 8 |
| /RST | 7 | SDA* | 5 |

*These I2C/SPI pins are repurposed as GPIO since the QT Py has limited pins.

### U8g2 Constructor

All sketches use the same constructor:

```cpp
U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0,
    /* clock=*/ 10, /* data=*/ 7, /* cs=*/ 6, /* dc=*/ 8, /* reset=*/ 5);
```

- `ERC12864_ALT` - contrast-improved variant for this display
- `_F_` - full framebuffer mode (~1KB RAM)
- `4W_SW_SPI` - 4-wire software SPI

### Sketches

Here are some Arduino sketches to run on A XIAO style board like Adafruit's QT Py ESP32-C3

![Breadboard with QT Py ESP32-C3 and display running ERC12864_Hello](images/Image1.png)

| Sketch | Description |
|--------|-------------|
| `ERC12864_Hello/` | Minimal hello-world with text and simple graphics. Also blinks the on-board NeoPixel. |
| `ERC12864_SPIDemo/` | Port of the manufacturer's C tutorial. Cycles through text, graphics primitives, XBM bitmap, and test patterns. Includes commented-out contrast adjustment code for future button support. |
| `GraphicsTest/` | U8g2 graphics test suite (from the U8g2 examples, configured for this display). |
| `ScrollingText/` | U8g2 scrolling text demo (from the U8g2 examples, configured for this display). |

---

## Reference Documentation

Datasheets and reference material are in `docs/`:

- `ERC12864-11_Datasheet.pdf` - Display module datasheet
- `ST7567_V1.7.pdf` - Controller IC datasheet
- `ERC12864FSF-11_Display.md` - Display specifications summary
- `ERC12864FSF-11_SPI_Reference_Design.md` - Wiring guide and U8g2 examples
- `ERC12864-11_Tutorial/` - Manufacturer's C code tutorial and interface schematic

## Tools

- **KiCad 9** for schematic and PCB design
- **Arduino IDE** with ESP32 board support for firmware development
- **U8g2** library for display graphics
