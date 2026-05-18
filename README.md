## TFT LCD Touch Display Library for Arduino to work with SPI TFT LCD Displays - DIYables_TFT_SPI
This Arduino library provides an easy-to-use interface for **SPI TFT LCD displays** based on the **ILI9341**, **ILI9488**, and **ST7789** driver ICs, with optional **XPT2046 / ADS7843** SPI touch controller support.
It enables fast and reliable graphics, text, and image rendering on common 1.3" – 3.5" SPI TFT modules.

**Works with both touch and non-touch versions** of the same SPI TFT modules — use the display-only API for non-touch panels, and add `initTouchSPI()` for modules that include the XPT2046 / ADS7843 touch controller.

* Fully extends the Adafruit GFX library, giving you access to all standard graphics and text functions.
* Three driver classes (`DIYables_ILI9341_SPI`, `DIYables_ILI9488_SPI`, `DIYables_ST7789_SPI`) sharing a common API — switch chips by changing one line.
* Built-in support for the **XPT2046 / ADS7843** SPI touch controller commonly found on these modules — no separate touch library required.
* Compatible with a wide range of boards, including Arduino Uno R3, Uno R4 WiFi/Minima, Nano, Mega, Giga R1 WiFi, MKR WiFi 1010, Nano 33 IoT, Nano ESP32, ESP32, and ESP8266.

With this library, you can easily create colorful, high-resolution graphical interfaces — menus, dashboards, image viewers, touch buttons — for your Arduino, ESP32, ESP8266, or SAMD-based projects.

![DIYables TFT SPI Display](https://newbiely.com/images/tutorial/ili9341-ili9488-st7789-tft-lcd-touch-display-spi-interface-pinout.jpg)



Supported Driver ICs
----------------------------
The library supports three popular SPI TFT controllers:

| Driver Class             | Resolution                                              | Color Format            | Max SPI Speed |
|--------------------------|---------------------------------------------------------|-------------------------|---------------|
| `DIYables_ILI9341_SPI`   | 240 × 320                                               | 16-bit RGB565           | 40 MHz        |
| `DIYables_ILI9488_SPI`   | 320 × 480                                               | 18-bit RGB666 (SPI)     | 24 MHz        |
| `DIYables_ST7789_SPI`    | 240 × 320 (also 240×240, 135×240, 170×320, 172×320)     | 16-bit RGB565           | 40 MHz        |

* ILI9488 in SPI mode supports only 18-bit color. The library automatically converts RGB565 → RGB666 during transfer.
* **Recommendation:** for new projects, choose **ST7789** — widely available, runs at full 40 MHz SPI speed, and is the most straightforward to use.



Product Link
----------------------------
* [DIYables SPI TFT LCD Display Modules](https://diyables.io/products/diyables-2.4-inch-tft-lcd-display-module-320x240-spi-st7789-color-screen-no-touch)



Features
----------------------------
* Works with any Arduino-compatible board that has a hardware SPI bus.
* Hardware SPI on any SPI bus (`SPI`, `SPI1`, `HSPI`, or user-defined `SPIClass` instances on ESP32).
* Backlight control with optional **PWM dimming** (`setBacklight(0..255)`).
* **Two touch backends** sharing the same `getTouch()` / `readTouchRaw()` API:
  * **XPT2046 / ADS7843** SPI touch controller — pins labeled `T_CS`, `T_CLK`, `T_DIN`, `T_DO`, `T_IRQ` (touch IC shares the display's SPI bus).
  * **4-wire bare resistive** panel — pins labeled `XP`/`X+`, `YP`/`Y+`, `XM`/`X-`, `YM`/`Y-`.
* SD-card image loading example using the same shared SPI bus.
* Touch calibration utility example.
* Designed for 3.3V displays; works with 5V boards (Uno, Nano, Mega, Uno R4) when a level converter is added on the MCU-driven signal lines.



Available Examples
----------------------------
* **DrawShapes.ino** — Draw circles, rectangles, triangles, diamonds, and round rectangles using the library's graphics functions.
* **ShowTextAndNumber.ino** — Display text strings and numbers in different fonts, sizes, and colors.
* **DrawImage.ino** — Display a bitmap image stored in program memory (flash) directly on the TFT screen.
* **DrawImageSDcard.ino** — Load and display a BMP image from an SD card.
* **UseExternalFont.ino** — Use and display external (custom) Adafruit GFX fonts for enhanced text appearance.
* **CustomSPI.ino** — Use a secondary or user-defined SPI bus for the display (useful on ESP32 / Giga R1 with multiple SPI peripherals).
* **TouchCalibration.ino** — Calibrate the touch screen for accurate touch detection and mapping.
* **TouchGetPoint.ino** — Read and display the raw and mapped touch coordinates from the touch panel.
* **TouchButton.ino** — Create touchable buttons and detect press/release events.
* **TouchDraw.ino** — Draw freehand lines on the screen by dragging your finger or stylus across the touch panel.



Tutorials
----------------------------
* [Arduino — TFT LCD Touch Display (SPI)](https://arduinogetstarted.com/tutorials/arduino-tft-lcd-touch-display-spi)
* [Arduino Nano — TFT LCD Touch Display (SPI)](https://arduinogetstarted.com/tutorials/arduino-nano-tft-lcd-touch-display-spi)
* [Arduino Mega — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-mega/arduino-mega-tft-lcd-touch-display-spi)
* [Arduino Uno R4 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-tft-lcd-touch-display-spi)
* [Arduino Giga R1 WiFi — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-giga-r1-wifi/arduino-giga-r1-wifi-tft-lcd-touch-display-spi)
* [Arduino MKR WiFi 1010 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-mkr-wifi-1010/arduino-mkr-wifi-1010-tft-lcd-touch-display-spi)
* [Arduino Nano 33 IoT — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-nano-33-iot/arduino-nano-33-iot-tft-lcd-touch-display-spi)
* [Arduino Nano ESP32 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-nano-esp32/arduino-nano-esp32-tft-lcd-touch-display-spi)
* [ESP32 — TFT LCD Touch Display (SPI)](https://esp32io.com/tutorials/esp32-tft-lcd-touch-display-spi)
* [ESP8266 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/esp8266/esp8266-tft-lcd-touch-display-spi)



References
----------------------------
* [DIYables_TFT_SPI Library Reference](docs/diyables-tft-spi-library-reference.md)



Supported and Tested Hardware
----------------------------

| Board                                                                                                          | Tested | Logic Level | Notes                              |
|----------------------------------------------------------------------------------------------------------------|:------:|:-----------:|------------------------------------|
| Arduino Uno R3                                                                                                 |   ✅   |     5V      | Requires 5V → 3.3V level converter |
| Arduino Nano                                                                                                   |   ✅   |     5V      | Requires 5V → 3.3V level converter |
| Arduino Mega 2560                                                                                              |   ✅   |     5V      | Requires 5V → 3.3V level converter |
| Arduino Uno R4 Minima                                                                                          |   ✅   |     5V      | Requires 5V → 3.3V level converter |
| Arduino Uno R4 WiFi                                                                                            |   ✅   |     5V      | Requires 5V → 3.3V level converter |
| Arduino Giga R1 WiFi                                                                                           |   ✅   |    3.3V     | Direct wiring                      |
| Arduino MKR WiFi 1010                                                                                          |   ✅   |    3.3V     | Direct wiring                      |
| Arduino Nano 33 IoT                                                                                            |   ✅   |    3.3V     | Direct wiring                      |
| Arduino Nano ESP32                                                                                             |   ✅   |    3.3V     | Direct wiring                      |
| ESP32 (DevKit / WROOM / WROVER)                                                                                |   ✅   |    3.3V     | Direct wiring; multiple SPI buses  |
| ESP8266 (NodeMCU / Wemos D1 mini)                                                                              |   ✅   |    3.3V     | Direct wiring                      |
| Other boards with hardware SPI                                                                                 |   —    |     —       | Should work; not officially tested |



**Note**: This library is designed for **SPI** TFT LCD modules (ILI9341, ILI9488, ST7789 — typically labeled with pins `CS`, `DC`, `RST`, `MOSI`, `SCK`, `MISO`, and optionally `T_CS`/`T_CLK`/`T_DIN`/`T_DO`/`T_IRQ` for the XPT2046 touch controller). If you are using a **parallel-interface 3.5" TFT Touch Shield** (28-pin Arduino shield form factor with ILI9488 or RM68140 driver), please use the [DIYables TFT Touch Shield Library](https://github.com/DIYables/DIYables_TFT_Touch_Shield) instead.
