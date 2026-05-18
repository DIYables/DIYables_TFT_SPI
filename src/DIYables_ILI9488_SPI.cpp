/*
 * DIYables_ILI9488_SPI.cpp - ILI9488 SPI TFT driver (320x480, 18-bit RGB666)
 *
 * ILI9488 in SPI mode requires 18-bit color (3 bytes per pixel).
 * This driver converts RGB565 to RGB666 automatically.
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#include "DIYables_ILI9488_SPI.h"

DIYables_ILI9488_SPI::DIYables_ILI9488_SPI(uint16_t width, uint16_t height,
                                           int8_t cs_pin, int8_t dc_pin,
                                           int8_t rst_pin, SPIClass *spi_bus)
  : DIYables_TFT_SPI(width, height, cs_pin, dc_pin, rst_pin, spi_bus) {}

uint32_t DIYables_ILI9488_SPI::getDefaultFreq() {
  return 24000000UL; // 24 MHz
}

void DIYables_ILI9488_SPI::initDriver() {
  writeCommand(0xE0); // Positive Gamma Control
  writeData8(0x00); writeData8(0x03); writeData8(0x09); writeData8(0x08);
  writeData8(0x16); writeData8(0x0A); writeData8(0x3F); writeData8(0x78);
  writeData8(0x4C); writeData8(0x09); writeData8(0x0A); writeData8(0x08);
  writeData8(0x16); writeData8(0x1A); writeData8(0x0F);

  writeCommand(0xE1); // Negative Gamma Control
  writeData8(0x00); writeData8(0x16); writeData8(0x19); writeData8(0x03);
  writeData8(0x0F); writeData8(0x05); writeData8(0x32); writeData8(0x45);
  writeData8(0x46); writeData8(0x04); writeData8(0x0E); writeData8(0x0D);
  writeData8(0x35); writeData8(0x37); writeData8(0x0F);

  writeCommand(0xC0); // Power Control 1
  writeData8(0x17); writeData8(0x15);

  writeCommand(0xC1); // Power Control 2
  writeData8(0x41);

  writeCommand(0xC5); // VCOM Control
  writeData8(0x00); writeData8(0x12); writeData8(0x80);

  writeCommand(0x36); // Memory Access Control
  writeData8(0x48);   // MX, BGR

  writeCommand(0x3A); // Interface Pixel Format
  writeData8(0x66);   // 18-bit RGB666 (required for SPI mode)

  writeCommand(0xB0); // Interface Mode Control
  writeData8(0x00);

  writeCommand(0xB1); // Frame Rate Control
  writeData8(0xA0);

  writeCommand(0xB4); // Display Inversion Control
  writeData8(0x02);

  writeCommand(0xB6); // Display Function Control
  writeData8(0x02); writeData8(0x02); writeData8(0x3B);

  writeCommand(0xB7); // Entry Mode Set
  writeData8(0xC6);

  writeCommand(0xF7); // Adjust Control 3
  writeData8(0xA9); writeData8(0x51); writeData8(0x2C); writeData8(0x82);

  writeCommand(0x11); // Sleep Out
  delay(120);

  writeCommand(0x29); // Display ON
  delay(50);
}

uint8_t DIYables_ILI9488_SPI::getRotationData(uint8_t r) {
  switch (r & 0x03) {
    case 0:  return 0x48; // MX, BGR
    case 1:  return 0x28; // MV, BGR
    case 2:  return 0x88; // MY, BGR
    default: return 0xE8; // MX, MY, MV, BGR
  }
}

// ILI9488 SPI mode uses 18-bit RGB666: 3 bytes per pixel
// Convert from RGB565 to 3 separate R, G, B bytes

static inline void rgb565_to_666(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b) {
  r = (color >> 8) & 0xF8; // R: 5 bits -> top of byte
  g = (color >> 3) & 0xFC; // G: 6 bits -> top of byte
  b = (color << 3) & 0xF8; // B: 5 bits -> top of byte
}

void DIYables_ILI9488_SPI::writeColor(uint16_t color) {
  uint8_t r, g, b;
  rgb565_to_666(color, r, g, b);
  spiWrite(r);
  spiWrite(g);
  spiWrite(b);
}

void DIYables_ILI9488_SPI::fillColor(uint16_t color, uint32_t count) {
  uint8_t r, g, b;
  rgb565_to_666(color, r, g, b);
  while (count--) {
    spiWrite(r);
    spiWrite(g);
    spiWrite(b);
  }
}

void DIYables_ILI9488_SPI::pushColorData(uint16_t *data, uint32_t len) {
  uint8_t r, g, b;
  while (len--) {
    rgb565_to_666(*data++, r, g, b);
    spiWrite(r);
    spiWrite(g);
    spiWrite(b);
  }
}

void DIYables_ILI9488_SPI::pushColorDataPGM(const uint16_t *data, uint32_t len) {
  uint8_t r, g, b;
  while (len--) {
    rgb565_to_666(pgm_read_word(data++), r, g, b);
    spiWrite(r);
    spiWrite(g);
    spiWrite(b);
  }
}
