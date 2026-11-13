/*
 * DIYables_ST7735_SPI.h - ST7735 / ST7735S / ST7735R SPI TFT driver
 *                         (128x160, 128x128, 80x160, 16-bit RGB565)
 *
 * Many ST7735 panels are smaller than the controller's 132x162 RAM and sit
 * at a column/row offset that also changes with the rotation. The driver
 * sets that offset through the base class's updateOffsets() hook. Same
 * drawing, rotation and backlight API as the other drivers. ST7735 modules
 * have no touch controller, so the touch functions are not used.
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#ifndef DIYABLES_ST7735_SPI_H
#define DIYABLES_ST7735_SPI_H

#include "DIYables_TFT_SPI_Base.h"

class DIYables_ST7735_SPI : public DIYables_TFT_SPI {
public:
  // ST7735 modules differ in where the visible area sits inside the
  // controller RAM and in their red/blue order. The 128x160 names follow
  // Adafruit's "tab" names (the colour of the tab on the screen's
  // protective film). If the screen shows a line of random pixels along
  // one or two edges, or red and blue are swapped, try another type.
  enum PanelType : uint8_t {
    PANEL_128x160_BLACKTAB = 0, // 1.8"  no offset,   RGB (default for 128x160)
    PANEL_128x160_REDTAB,       // 1.8"  no offset,   BGR
    PANEL_128x160_GREENTAB,     // 1.8"  offset 2,1,  BGR
    PANEL_128x128,              // 1.44" offset 2,3,  BGR (default for 128x128)
    PANEL_80x160,               // 0.96" offset 24,0, RGB (default for 80x160)
    PANEL_80x160_PLUGIN         // 0.96" offset 26,1, BGR, inverted
  };

  // width/height: panel resolution in portrait orientation
  //   (128x160, 128x128 or 80x160). The panel type is picked from the size;
  //   call setPanelType() before begin() to choose a different one.
  // Pass &SPI1, &HSPI, etc. for an alternate SPI bus.
  DIYables_ST7735_SPI(uint16_t width, uint16_t height,
                      int8_t cs_pin, int8_t dc_pin,
                      int8_t rst_pin = -1, SPIClass *spi_bus = &SPI);

  void setPanelType(PanelType type); // call before begin()

protected:
  PanelType _panel;

  void     initDriver() override;
  uint8_t  getRotationData(uint8_t r) override;
  uint32_t getDefaultFreq() override;
  void     updateOffsets() override;
  bool     invertedByDefault() const override { return _panel == PANEL_80x160_PLUGIN; }
};

#endif // DIYABLES_ST7735_SPI_H
