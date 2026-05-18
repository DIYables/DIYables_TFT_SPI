/*
 * DIYables_XPT2046.cpp - Implementation of the XPT2046 SPI touch driver.
 *
 * Copyright (c) DIYables.io
 * Licensed under the MIT License.
 */

#include "DIYables_XPT2046.h"

// XPT2046 control byte format: S A2 A1 A0 MODE SER/DFR PD1 PD0
//   S        = 1 (start bit)
//   A2..A0   = channel select
//   MODE     = 0 -> 12-bit, 1 -> 8-bit
//   SER/DFR  = 0 -> differential, 1 -> single-ended
//   PD1 PD0  = power-down between conversions (00 = lowest power)
// Standard differential 12-bit reads:
#define XPT2046_CMD_READ_X   0xD0
#define XPT2046_CMD_READ_Y   0x90
#define XPT2046_CMD_READ_Z1  0xB0
#define XPT2046_CMD_READ_Z2  0xC0

// XPT2046 max SPI clock: ~2 MHz. Stay safe on slow MCUs and shared buses.
static const uint32_t XPT2046_SPI_FREQ = 2000000UL;

DIYables_XPT2046::DIYables_XPT2046(uint8_t cs, int8_t irq, SPIClass *spi)
  : pressureThreshhold(400), _cs(cs), _irq(irq), _spi(spi) {}

void DIYables_XPT2046::begin() {
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  if (_irq >= 0) pinMode(_irq, INPUT_PULLUP);
}

uint16_t DIYables_XPT2046::readChannel(uint8_t cmd) {
  _spi->transfer(cmd);
  uint8_t hi = _spi->transfer(0x00);
  uint8_t lo = _spi->transfer(0x00);
  // 12-bit result is left-aligned in the 16 read bits.
  return ((uint16_t)hi << 5) | (lo >> 3);
}

void DIYables_XPT2046::readSample(int16_t &x, int16_t &y, int16_t &z) {
  _spi->beginTransaction(SPISettings(XPT2046_SPI_FREQ, MSBFIRST, SPI_MODE0));
  digitalWrite(_cs, LOW);

  // Read Z first (pressure).
  uint16_t z1 = readChannel(XPT2046_CMD_READ_Z1);
  uint16_t z2 = readChannel(XPT2046_CMD_READ_Z2);
  // Approximate pressure indicator (smaller value -> harder press).
  // Most sketches just need a relative number, so use a simple form:
  //   z = z1 + 4095 - z2
  // which grows with press strength and is 0-ish when not touched.
  int32_t z_calc = (int32_t)z1 + 4095 - (int32_t)z2;
  if (z_calc < 0)    z_calc = 0;
  if (z_calc > 4095) z_calc = 4095;
  z = (int16_t)z_calc;

  // Oversample X and Y a few times and average for noise reduction.
  const uint8_t SAMPLES = 3;
  uint32_t sx = 0, sy = 0;
  for (uint8_t i = 0; i < SAMPLES; ++i) {
    sx += readChannel(XPT2046_CMD_READ_X);
    sy += readChannel(XPT2046_CMD_READ_Y);
  }
  x = (int16_t)(sx / SAMPLES);
  y = (int16_t)(sy / SAMPLES);

  // Final read with PD1=PD0=0 to power down between conversions.
  _spi->transfer(0x80); // dummy power-down command
  _spi->transfer(0x00);
  _spi->transfer(0x00);

  digitalWrite(_cs, HIGH);
  _spi->endTransaction();
}

TSPoint DIYables_XPT2046::getPoint() {
  int16_t x, y, z;
  readSample(x, y, z);
  return TSPoint(x, y, z);
}

uint16_t DIYables_XPT2046::pressure() {
  int16_t x, y, z;
  readSample(x, y, z);
  return (uint16_t)z;
}

bool DIYables_XPT2046::isTouching() {
  // Fast path: if IRQ pin is wired, it goes LOW while screen is touched.
  if (_irq >= 0) {
    if (digitalRead(_irq) == HIGH) return false;
  }
  int16_t x, y, z;
  readSample(x, y, z);
  return z > pressureThreshhold;
}
