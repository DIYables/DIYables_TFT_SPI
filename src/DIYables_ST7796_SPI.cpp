/*
 * DIYables_ST7796_SPI.cpp - ST7796 / ST7796S SPI TFT driver (320x480, 16-bit RGB565)
 *
 * Implementation notes:
 * - The ST7796S supports 16-bit RGB565 in 4-wire SPI mode, so the default
 *   RGB565 pixel methods of the base class are used as-is.
 * - CASET (0x2A), RASET (0x2B), RAMWR (0x2C), MADCTL (0x36), INVOFF/INVON
 *   (0x20/0x21) are the standard MIPI DCS commands the base class already
 *   sends, so setAddrWindow(), setRotation() and invertDisplay() work
 *   without any override.
 * - Several panel-tuning registers (0xB6, 0xE8, 0xC1, 0xC2, 0xC5, gamma)
 *   sit in the ST7796S "Command Set 2" bank. That bank is unlocked with
 *   CSCON (0xF0) = 0xC3 + 0x96 before writing them, and locked again with
 *   0x3C + 0x69 afterwards.
 * - Rotation 0 is portrait with MADCTL = MX | BGR (0x48), the same
 *   convention as the ILI9341 / ILI9488 drivers, so sketches behave the
 *   same when switching between these classes.
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#include "DIYables_ST7796_SPI.h"

DIYables_ST7796_SPI::DIYables_ST7796_SPI(uint16_t width, uint16_t height,
                                         int8_t cs_pin, int8_t dc_pin,
                                         int8_t rst_pin, SPIClass *spi_bus)
  : DIYables_TFT_SPI(width, height, cs_pin, dc_pin, rst_pin, spi_bus) {
  // Touch-axis polarity for ST7796S modules with an XPT2046 controller.
  // Left at the base defaults (same as ILI9341). If a press on the left is
  // reported on the right on your module, set _touch_invert_x = true here
  // (as the ILI9488 / ST7789 drivers do), or call setTouchInvertX(true)
  // from the sketch.
  _touch_invert_x = false;
  _touch_invert_y = false;
}

uint32_t DIYables_ST7796_SPI::getDefaultFreq() {
  return 40000000UL; // 40 MHz
}

void DIYables_ST7796_SPI::initDriver() {
  // Software reset. begin() only pulses the hardware reset line when a
  // RST pin is given; this makes init reliable when RST is tied high or
  // the MCU restarts without power-cycling the panel.
  writeCommand(0x01); // SWRESET
  delay(120);

  writeCommand(0x11); // Sleep Out
  delay(120);

  writeCommand(0xF0); // Command Set Control
  writeData8(0xC3);   // Enable Command Set 2, part I
  writeCommand(0xF0); // Command Set Control
  writeData8(0x96);   // Enable Command Set 2, part II

  writeCommand(0x36); // Memory Access Control
  writeData8(0x48);   // MX, BGR (rotation 0 = portrait)

  writeCommand(0x3A); // Interface Pixel Format
  writeData8(0x55);   // 16-bit RGB565

  writeCommand(0xB4); // Display Inversion Control
  writeData8(0x01);   // 1-dot inversion

  writeCommand(0xB6); // Display Function Control
  writeData8(0x80);   // Bypass
  writeData8(0x02);   // Source S1->S960, gate G1->G480, scan cycle = 2
  writeData8(0x3B);   // Drive lines = 8 * (0x3B + 1) = 480

  writeCommand(0xE8); // Display Output Control Adjust
  writeData8(0x40); writeData8(0x8A); writeData8(0x00); writeData8(0x00);
  writeData8(0x29);   // Source equalizing period = 22.5 us
  writeData8(0x19);   // Gate start timing = 25 Tclk
  writeData8(0xA5);   // Gate end timing = 37 Tclk, gate driver EQ on
  writeData8(0x33);

  writeCommand(0xC1); // Power Control 2
  writeData8(0x06);   // VAP(GVDD) = 3.85 + (VCOM + offset)

  writeCommand(0xC2); // Power Control 3
  writeData8(0xA7);   // Source current low, gamma current high

  writeCommand(0xC5); // VCOM Control
  writeData8(0x18);   // VCOM = 0.9 V
  delay(120);

  writeCommand(0xE0); // Positive Gamma Control
  writeData8(0xF0); writeData8(0x09); writeData8(0x0B); writeData8(0x06);
  writeData8(0x04); writeData8(0x15); writeData8(0x2F); writeData8(0x54);
  writeData8(0x42); writeData8(0x3C); writeData8(0x17); writeData8(0x14);
  writeData8(0x18); writeData8(0x1B);

  writeCommand(0xE1); // Negative Gamma Control
  writeData8(0xE0); writeData8(0x09); writeData8(0x0B); writeData8(0x06);
  writeData8(0x04); writeData8(0x03); writeData8(0x2B); writeData8(0x43);
  writeData8(0x42); writeData8(0x3B); writeData8(0x16); writeData8(0x14);
  writeData8(0x17); writeData8(0x1B);
  delay(120);

  writeCommand(0xF0); // Command Set Control
  writeData8(0x3C);   // Disable Command Set 2, part I
  writeCommand(0xF0); // Command Set Control
  writeData8(0x69);   // Disable Command Set 2, part II
  delay(120);

  writeCommand(0x29); // Display ON
  delay(50);
}

uint8_t DIYables_ST7796_SPI::getRotationData(uint8_t r) {
  switch (r & 0x03) {
    case 0:  return 0x48; // MX, BGR          -> portrait
    case 1:  return 0x28; // MV, BGR          -> landscape
    case 2:  return 0x88; // MY, BGR          -> portrait flipped
    default: return 0xE8; // MX, MY, MV, BGR  -> landscape flipped
  }
}
