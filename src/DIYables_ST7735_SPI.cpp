/*
 * DIYables_ST7735_SPI.cpp - ST7735 / ST7735S / ST7735R SPI TFT driver
 *
 * Init sequence, panel offsets and MADCTL values follow the widely used
 * Adafruit_ST7735 "initR" tables (the same values TFT_eSPI uses).
 * Rotation 0 is portrait, matching the other drivers in this library.
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#include "DIYables_ST7735_SPI.h"

// MADCTL bits
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_BGR 0x08

// Start-up commands (Adafruit "initR"), sent one CS pulse per command.
// Inversion and MADCTL depend on the panel type and are sent between the
// two tables, in the same order as the Adafruit sequence.
static const uint8_t ST7735_INIT_1[] PROGMEM = {
  0x01, DIYABLES_TFT_INIT_DELAY, 150,          // Software reset
  0x11, DIYABLES_TFT_INIT_DELAY, 255,          // Sleep out, 500 ms
  0xB1, 3, 0x01, 0x2C, 0x2D,                   // Frame rate - normal mode
  0xB2, 3, 0x01, 0x2C, 0x2D,                   // Frame rate - idle mode
  0xB3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D, // Frame rate - partial mode
  0xB4, 1, 0x07,                               // Display inversion control
  0xC0, 3, 0xA2, 0x02, 0x84,                   // Power control 1
  0xC1, 1, 0xC5,                               // Power control 2
  0xC2, 2, 0x0A, 0x00,                         // Power control 3
  0xC3, 2, 0x8A, 0x2A,                         // Power control 4
  0xC4, 2, 0x8A, 0xEE,                         // Power control 5
  0xC5, 1, 0x0E,                               // VCOM control
  DIYABLES_TFT_INIT_END
};

static const uint8_t ST7735_INIT_2[] PROGMEM = {
  0x3A, 1, 0x05,                               // Pixel format: 16-bit RGB565
  0xE0, 16, 0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, // Positive gamma
            0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10,
  0xE1, 16, 0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, // Negative gamma
            0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10,
  0x13, DIYABLES_TFT_INIT_DELAY, 10,           // Normal display mode on
  0x29, DIYABLES_TFT_INIT_DELAY, 100,          // Display on
  DIYABLES_TFT_INIT_END
};

DIYables_ST7735_SPI::DIYables_ST7735_SPI(uint16_t width, uint16_t height,
                                         int8_t cs_pin, int8_t dc_pin,
                                         int8_t rst_pin, SPIClass *spi_bus)
  : DIYables_TFT_SPI(width, height, cs_pin, dc_pin, rst_pin, spi_bus) {
  if (width == 128 && height == 128) {
    _panel = PANEL_128x128;
  } else if (width == 80 && height == 160) {
    _panel = PANEL_80x160;
  } else {
    _panel = PANEL_128x160_BLACKTAB;
  }
  updateOffsets();
}

void DIYables_ST7735_SPI::setPanelType(PanelType type) {
  _panel = type;
  updateOffsets();
}

uint32_t DIYables_ST7735_SPI::getDefaultFreq() {
  return 27000000UL; // 27 MHz
}

// Position of the visible area inside the 132x162 controller RAM, for the
// current rotation. In landscape (MV set) the controller's columns become
// screen rows, so the two offsets swap.
void DIYables_ST7735_SPI::updateOffsets() {
  uint8_t col = 0, row = 0;
  switch (_panel) {
    case PANEL_128x160_GREENTAB: col = 2;  row = 1; break;
    case PANEL_128x128:          col = 2;  row = (getRotation() < 2) ? 3 : 1; break;
    case PANEL_80x160:           col = 24; row = 0; break;
    case PANEL_80x160_PLUGIN:    col = 26; row = 1; break;
    default: break; // black tab / red tab: panel starts at RAM origin
  }
  if (getRotation() & 1) {
    _x_off = row;
    _y_off = col;
  } else {
    _x_off = col;
    _y_off = row;
  }
}

uint8_t DIYables_ST7735_SPI::getRotationData(uint8_t r) {
  uint8_t order = (_panel == PANEL_128x160_BLACKTAB || _panel == PANEL_80x160)
                    ? 0x00 : ST7735_MADCTL_BGR;
  switch (r & 0x03) {
    case 0:  return ST7735_MADCTL_MX | ST7735_MADCTL_MY | order; // portrait
    case 1:  return ST7735_MADCTL_MY | ST7735_MADCTL_MV | order; // landscape
    case 2:  return order;                                       // portrait flipped
    default: return ST7735_MADCTL_MX | ST7735_MADCTL_MV | order; // landscape flipped
  }
}

void DIYables_ST7735_SPI::initDriver() {
  sendInitTable(ST7735_INIT_1);
  sendCommand(invertedByDefault() ? 0x21 : 0x20); // Inversion on / off
  uint8_t madctl = getRotationData(getRotation());
  sendCommand(0x36, &madctl, 1);                  // Memory access control
  sendInitTable(ST7735_INIT_2);
}
