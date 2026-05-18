/*
 * DIYables_ST7789_SPI.cpp - ST7789 SPI TFT driver (240x320, 16-bit RGB565)
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#include "DIYables_ST7789_SPI.h"

DIYables_ST7789_SPI::DIYables_ST7789_SPI(uint16_t width, uint16_t height,
                                         int8_t cs_pin, int8_t dc_pin,
                                         int8_t rst_pin, SPIClass *spi_bus)
  : DIYables_TFT_SPI(width, height, cs_pin, dc_pin, rst_pin, spi_bus) {}

uint32_t DIYables_ST7789_SPI::getDefaultFreq() {
  return 40000000UL; // 40 MHz
}

void DIYables_ST7789_SPI::initDriver() {
  writeCommand(0x01); // Software Reset
  delay(150);

  writeCommand(0x11); // Sleep Out
  delay(120);

  writeCommand(0x3A); // Pixel Format
  writeData8(0x55);   // 16-bit RGB565

  writeCommand(0x36); // Memory Access Control
  writeData8(0x00);

  writeCommand(0xB2); // Porch Setting
  writeData8(0x0C); writeData8(0x0C);
  writeData8(0x00); writeData8(0x33); writeData8(0x33);

  writeCommand(0xB7); // Gate Control
  writeData8(0x35);

  writeCommand(0xBB); // VCOM Setting
  writeData8(0x19);

  writeCommand(0xC0); // LCM Control
  writeData8(0x2C);

  writeCommand(0xC2); // VDV and VRH Command Enable
  writeData8(0x01);

  writeCommand(0xC3); // VRH Set
  writeData8(0x12);

  writeCommand(0xC4); // VDV Set
  writeData8(0x20);

  writeCommand(0xC6); // Frame Rate Control in Normal Mode
  writeData8(0x0F);   // 60 Hz

  writeCommand(0xD0); // Power Control 1
  writeData8(0xA4); writeData8(0xA1);

  writeCommand(0xE0); // Positive Gamma Correction
  writeData8(0xD0); writeData8(0x04); writeData8(0x0D); writeData8(0x11);
  writeData8(0x13); writeData8(0x2B); writeData8(0x3F); writeData8(0x54);
  writeData8(0x4C); writeData8(0x18); writeData8(0x0D); writeData8(0x0B);
  writeData8(0x1F); writeData8(0x23);

  writeCommand(0xE1); // Negative Gamma Correction
  writeData8(0xD0); writeData8(0x04); writeData8(0x0C); writeData8(0x11);
  writeData8(0x13); writeData8(0x2C); writeData8(0x3F); writeData8(0x44);
  writeData8(0x51); writeData8(0x2F); writeData8(0x1F); writeData8(0x1F);
  writeData8(0x20); writeData8(0x23);

  writeCommand(0x21); // Display Inversion On (ST7789 typically needs this)

  writeCommand(0x13); // Normal Display Mode On

  writeCommand(0x29); // Display ON
  delay(50);
}

uint8_t DIYables_ST7789_SPI::getRotationData(uint8_t r) {
  switch (r & 0x03) {
    case 0:  return 0x00; // Portrait
    case 1:  return 0x60; // Landscape (MV, MX)
    case 2:  return 0xC0; // Portrait inverted (MX, MY)
    default: return 0xA0; // Landscape inverted (MV, MY)
  }
}
