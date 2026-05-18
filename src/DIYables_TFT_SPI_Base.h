/*
 * DIYables_TFT_SPI_Base.h - Base class for SPI TFT displays
 *
 * Internal header. Do NOT include this directly from sketches.
 * Sketches should include <DIYables_TFT_SPI.h> instead, which exposes
 * this base class and all driver classes (ILI9341, ILI9488, ST7789).
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#ifndef DIYABLES_TFT_SPI_BASE_H
#define DIYABLES_TFT_SPI_BASE_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "DIYables_TouchScreen.h"
#include "DIYables_XPT2046.h"

class DIYables_TFT_SPI : public Adafruit_GFX {
public:
  // Hardware SPI constructor (pass &SPI1, &HSPI, etc. for alternate SPI bus)
  DIYables_TFT_SPI(uint16_t width, uint16_t height,
                   int8_t cs_pin, int8_t dc_pin,
                   int8_t rst_pin = -1, SPIClass *spi_bus = &SPI);

  virtual ~DIYables_TFT_SPI();

  void begin(uint32_t freq = 0);

  // Adafruit_GFX overrides
  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  void writePixel(int16_t x, int16_t y, uint16_t color) override;
  void fillScreen(uint16_t color) override;
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
  void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void invertDisplay(bool i) override;
  void setRotation(uint8_t r) override;

  // Low-level methods (call within startWrite/endWrite block)
  void setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
  void pushColors(uint16_t *data, uint32_t len);

  // Optimized bitmap drawing
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);

  // SPI transaction control (public for advanced use)
  void startWrite();
  void endWrite();

  // Touch screen support.
  // Two backends are supported; pick the one that matches your module:
  //  1) initTouch(...)    - bare 4-wire resistive panel (XP/YP/XM/YM pins).
  //                         Uses analogRead(); no extra controller IC.
  //  2) initTouchSPI(...) - XPT2046 / ADS7843 SPI touch controller
  //                         (modules with T_CS / T_CLK / T_DIN / T_DO / T_IRQ).
  // After init, use setTouchCalibration(), getTouch() and readTouchRaw()
  // identically regardless of which backend is active.
  void initTouch(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx = 300);
  void initTouchSPI(uint8_t cs_pin, int8_t irq_pin = -1, SPIClass *spi_bus = nullptr);
  void setTouchCalibration(int min_x, int max_x, int min_y, int max_y);
  void setADCResolution(uint8_t bits);
  void readTouchRaw(int &x, int &y, int &z);
  bool getTouch(int &screenX, int &screenY);

  // Backlight (LED pin) control.
  // Call initBacklight() once with the GPIO connected to the module's LED/BL
  // pin. Use a PWM-capable pin if you want analog dimming via setBacklight().
  void initBacklight(int8_t led_pin);
  void backlightOn();
  void backlightOff();
  void setBacklight(uint8_t brightness); // 0 = off, 255 = full (PWM)

  static uint16_t colorRGB(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }

protected:
  int8_t  _cs, _dc, _rst;
  uint32_t _freq;
  SPIClass *_spi;

  void spiWrite(uint8_t val);
  void writeCommand(uint8_t cmd);
  void writeData8(uint8_t data);
  void writeData16(uint16_t data);

  // Pure virtual — implemented by each driver
  virtual void    initDriver() = 0;
  virtual uint8_t getRotationData(uint8_t r) = 0;
  virtual uint32_t getDefaultFreq() = 0;

  // Default RGB565 (16-bit, 2 bytes/pixel) — override for other formats (e.g. ILI9488 RGB666)
  virtual void    writeColor(uint16_t color);
  virtual void    fillColor(uint16_t color, uint32_t count);
  virtual void    pushColorData(uint16_t *data, uint32_t len);
  virtual void    pushColorDataPGM(const uint16_t *data, uint32_t len);

private:
  DIYables_TouchScreen *_ts = nullptr;     // 4-wire resistive backend
  DIYables_XPT2046     *_ts_spi = nullptr; // XPT2046 SPI backend
  int _touch_min_x = 150, _touch_max_x = 880;
  int _touch_min_y = 120, _touch_max_y = 900;
  int8_t _led_pin = -1;
};

#endif // DIYABLES_TFT_SPI_BASE_H
