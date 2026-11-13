/*
 * DIYables_ST7789_SPI.cpp - ST7789 SPI TFT driver
 *
 * Implementation notes:
 * - The 240x320 settings target the DIYables ST7789 SPI TFT module. It also
 *   works with generic ST7789 panels, since the commands used here
 *   (SWRESET, SLPOUT, COLMOD, MADCTL, CASET, RASET, DISPON) are part
 *   of the common MIPI DCS set supported by both controller families,
 *   and any extra commands are simply ignored by a real ST7789.
 * - The module is landscape-native (320x240 RAM). To keep the public
 *   API consistent with the ILI9341 driver -- where the user passes
 *   portrait dimensions (240x320) and rotation 0 is portrait -- this
 *   class internally adds a 90 degree offset in its MADCTL values so
 *   the application code does not have to know about it.
 * - Smaller panels (the panel table below) sit at an offset inside the
 *   240x320 RAM. They use the full ST7789 init sequence that TFT_eSPI
 *   uses, sent one CS pulse per command, and Adafruit_ST7789's MADCTL
 *   values (portrait-native, rotation 0 = portrait).
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#include "DIYables_ST7789_SPI.h"

// MADCTL bits
#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_BGR 0x08

namespace {

// Init for the DIYables 240x320 module.
const uint8_t ST7789_240x320_INIT[] PROGMEM = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,             // Power control B
  0xED, 4, 0x64, 0x03, 0x12, 0x81,       // Power on sequence control
  0xE8, 3, 0x85, 0x00, 0x78,             // Driver timing control A
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02, // Power control A
  0xF7, 1, 0x20,                         // Pump ratio control
  0xEA, 2, 0x00, 0x00,                   // Driver timing control B
  0xC0, 1, 0x23,                         // Power control 1
  0xC1, 1, 0x10,                         // Power control 2
  0xC5, 2, 0x3E, 0x28,                   // VCOM control 1
  0xC7, 1, 0x86,                         // VCOM control 2
  0x36, 1, 0x20,                         // Memory access control: MV, RGB
                                         // (rotation 0 = portrait on this
                                         // landscape-native panel)
  0x3A, 1, 0x55,                         // Pixel format: 16-bit RGB565
  0xB1, 2, 0x00, 0x18,                   // Frame rate control
  0xB6, 3, 0x08, 0x82, 0x27,             // Display function control
  // Gamma programming intentionally omitted: the ILI9341 reference
  // gamma values dim the red channel on this panel (pure magenta drifts
  // toward violet). Letting the chip use its NVM default gamma gives
  // more accurate primaries.
  0x11, DIYABLES_TFT_INIT_DELAY, 120,    // Sleep out
  0x29, DIYABLES_TFT_INIT_DELAY, 50,     // Display on
  DIYABLES_TFT_INIT_END
};

// ---- Panel table ----
// Panels smaller than the 240x320 RAM. Offsets are for portrait and swap in
// landscape. A panel that is not centred in the RAM (e.g. 240x240, whose
// offset changes when flipped) needs an extra field when it is added.
struct ST7789Panel {
  uint16_t width, height;    // portrait size, as passed to the constructor
  uint8_t  col_off, row_off; // portrait position inside the 240x320 RAM
  bool     bgr;              // red/blue order of the glass
  bool     inverted;         // needs display inversion ON for normal colours
  uint32_t freq;             // default SPI clock
};

const ST7789Panel ST7789_PANELS[] = {
  // 2.25" 76x284 bar display, centred: ((240-76)/2, (320-284)/2).
  // Needs BGR order and inversion OFF (with RGB and inversion on, red shows
  // as yellow). Tested at 20 MHz; pass a higher clock to begin() to try more.
  { 76, 284, 82, 18, true, false, 20000000UL },
};

const size_t ST7789_PANEL_COUNT = sizeof(ST7789_PANELS) / sizeof(ST7789_PANELS[0]);

// Init for the panels in the table (full ST7789 bring-up, as in TFT_eSPI).
// MADCTL goes between the two tables, inversion and display on after them.
const uint8_t ST7789_PANEL_INIT_1[] PROGMEM = {
  0x01, DIYABLES_TFT_INIT_DELAY, 150,      // Software reset
  0x11, DIYABLES_TFT_INIT_DELAY, 120,      // Sleep out
  0x3A, 1 | DIYABLES_TFT_INIT_DELAY, 0x55, 10, // Pixel format: 16-bit RGB565
  DIYABLES_TFT_INIT_END
};

const uint8_t ST7789_PANEL_INIT_2[] PROGMEM = {
  0xB2, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33, // Porch control
  0xB7, 1, 0x35,                         // Gate control (VGH / VGL)
  0xBB, 1, 0x28,                         // VCOM setting
  0xC0, 1, 0x0C,                         // LCM control
  0xC2, 2, 0x01, 0xFF,                   // VDV and VRH command enable
  0xC3, 1, 0x10,                         // VRH set
  0xC4, 1, 0x20,                         // VDV set
  0xC6, 1, 0x0F,                         // Frame rate control (60 Hz)
  0xD0, 2, 0xA4, 0xA1,                   // Power control 1
  0xE0, 14, 0xD0, 0x00, 0x02, 0x07, 0x0A, 0x28, 0x32, // Positive gamma
            0x44, 0x42, 0x06, 0x0E, 0x12, 0x14, 0x17,
  0xE1, 14, 0xD0, 0x00, 0x02, 0x07, 0x0A, 0x28, 0x31, // Negative gamma
            0x54, 0x47, 0x0E, 0x1C, 0x17, 0x1B, 0x1E,
  DIYABLES_TFT_INIT_END
};

} // namespace

DIYables_ST7789_SPI::DIYables_ST7789_SPI(uint16_t width, uint16_t height,
                                         int8_t cs_pin, int8_t dc_pin,
                                         int8_t rst_pin, SPIClass *spi_bus)
  : DIYables_TFT_SPI(width, height, cs_pin, dc_pin, rst_pin, spi_bus),
    _panel(-1) {
  _touch_invert_x = true;
  _touch_invert_y = false;
  for (size_t i = 0; i < ST7789_PANEL_COUNT; i++) {
    if (ST7789_PANELS[i].width == width && ST7789_PANELS[i].height == height) {
      _panel = (int8_t)i;
      break;
    }
  }
  updateOffsets();
}

uint32_t DIYables_ST7789_SPI::getDefaultFreq() {
  if (_panel >= 0) return ST7789_PANELS[_panel].freq;
  return 40000000UL; // 40 MHz
}

void DIYables_ST7789_SPI::updateOffsets() {
  if (_panel < 0) return; // 240x320 module: no offset
  const ST7789Panel &p = ST7789_PANELS[_panel];
  if (getRotation() & 1) { // landscape: controller columns become screen rows
    _x_off = p.row_off;
    _y_off = p.col_off;
  } else {
    _x_off = p.col_off;
    _y_off = p.row_off;
  }
}

bool DIYables_ST7789_SPI::invertedByDefault() const {
  return _panel >= 0 && ST7789_PANELS[_panel].inverted;
}

void DIYables_ST7789_SPI::initPanel() {
  sendInitTable(ST7789_PANEL_INIT_1);
  uint8_t madctl = getRotationData(getRotation());
  sendCommand(0x36, &madctl, 1);                             // Memory access control
  sendInitTable(ST7789_PANEL_INIT_2);
  sendCommand(invertedByDefault() ? 0x21 : 0x20); delay(10); // Inversion on / off
  sendCommand(0x13); delay(10);                              // Normal display mode on
  sendCommand(0x29); delay(120);                             // Display on
}

void DIYables_ST7789_SPI::initDriver() {
  if (_panel >= 0) {
    initPanel();
    return;
  }

  // DIYables 240x320 module: CS stays low for the whole sequence
  sendInitTable(ST7789_240x320_INIT, false);
}

uint8_t DIYables_ST7789_SPI::getRotationData(uint8_t r) {
  if (_panel >= 0) { // panel from the table: portrait-native
    uint8_t m;
    switch (r & 0x03) {
      case 0:  m = ST7789_MADCTL_MX | ST7789_MADCTL_MY; break; // portrait
      case 1:  m = ST7789_MADCTL_MY | ST7789_MADCTL_MV; break; // landscape
      case 2:  m = 0x00;                                break; // portrait flipped
      default: m = ST7789_MADCTL_MX | ST7789_MADCTL_MV; break; // landscape flipped
    }
    return ST7789_PANELS[_panel].bgr ? (uint8_t)(m | ST7789_MADCTL_BGR) : m;
  }

  // DIYables 240x320 module:
  // Table rotated +90 deg vs. the ILI9341 driver so the application
  // sees the same orientation behavior on both panels:
  //   rotation 0 = portrait, 1 = landscape, 2 = portrait flipped,
  //   3 = landscape flipped, regardless of whether the underlying panel
  //   is portrait-native or landscape-native.
  switch (r & 0x03) {
    case 0:  return 0x20; // MV,            RGB -> portrait
    case 1:  return 0x80; // MY,            RGB -> landscape
    case 2:  return 0xE0; // MX, MY, MV,    RGB -> portrait flipped
    default: return 0x40; // MX,            RGB -> landscape flipped
  }
}
