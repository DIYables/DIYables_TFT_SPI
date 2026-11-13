/*
 * DIYables_ST7796_SPI.h - ST7796 / ST7796S SPI TFT driver (320x480, 16-bit RGB565)
 *
 * Unlike the ILI9488, the ST7796S accepts 16-bit RGB565 over SPI, so this
 * driver uses the base class pixel path unchanged (2 bytes per pixel).
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#ifndef DIYABLES_ST7796_SPI_H
#define DIYABLES_ST7796_SPI_H

#include "DIYables_TFT_SPI_Base.h"

class DIYables_ST7796_SPI : public DIYables_TFT_SPI {
public:
  // width/height: panel resolution in its native orientation.
  //   Common ST7796S modules: 320x480 (portrait) or 480x320 (landscape).
  // Pass &SPI1, &HSPI, etc. for an alternate SPI bus.
  DIYables_ST7796_SPI(uint16_t width, uint16_t height,
                      int8_t cs_pin, int8_t dc_pin,
                      int8_t rst_pin = -1, SPIClass *spi_bus = &SPI);

protected:
  void     initDriver() override;
  uint8_t  getRotationData(uint8_t r) override;
  uint32_t getDefaultFreq() override;
};

#endif // DIYABLES_ST7796_SPI_H
