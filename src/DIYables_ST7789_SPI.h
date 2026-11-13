/*
 * DIYables_ST7789_SPI.h - ST7789 SPI TFT driver (16-bit RGB565)
 *
 * Supported panels (pass the portrait size to the constructor):
 *   240 x 320  DIYables ST7789 module (touch and non-touch versions)
 *    76 x 284  2.25" bar display (no touch)
 * Any other size uses the 240 x 320 module's settings.
 *
 * The ST7789 has 240x320 pixels of RAM. A smaller panel shows only part of
 * it, and each panel also needs its own colour order, inversion and clock,
 * so the driver keeps a small profile per size. To add a size, add a row to
 * the panel table in DIYables_ST7789_SPI.cpp.
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#ifndef DIYABLES_ST7789_SPI_H
#define DIYABLES_ST7789_SPI_H

#include "DIYables_TFT_SPI_Base.h"

class DIYables_ST7789_SPI : public DIYables_TFT_SPI {
public:
  // width/height: panel resolution in portrait orientation, e.g. 240x320
  //   or 76x284 (see the list above).
  // Pass &SPI1, &HSPI, etc. for an alternate SPI bus.
  DIYables_ST7789_SPI(uint16_t width, uint16_t height,
                      int8_t cs_pin, int8_t dc_pin,
                      int8_t rst_pin = -1, SPIClass *spi_bus = &SPI);

protected:
  void     initDriver() override;
  uint8_t  getRotationData(uint8_t r) override;
  uint32_t getDefaultFreq() override;
  void     updateOffsets() override;
  bool     invertedByDefault() const override;

private:
  int8_t _panel; // row in the panel table, or -1 for the 240x320 module

  void initPanel(); // init for the panels in the table
};

#endif // DIYABLES_ST7789_SPI_H
