## TFT LCD Touch Display Library for Arduino to work with SPI TFT LCD Displays - DIYables_TFT_SPI
This Arduino library provides an easy-to-use interface for **SPI TFT LCD displays** based on the **ILI9341**, **ILI9488**, **ST7789**, **ST7796**, and **ST7735** driver ICs, with optional **XPT2046 / HR2046 / ADS7843** SPI touch controller support.
It enables fast and reliable graphics, text, and image rendering on common 0.96" – 4.0" SPI TFT modules.

**Works with both touch and non-touch versions** of the same SPI TFT modules — use the display-only API for non-touch panels, and add `initTouchSPI()` for modules that include the XPT2046 / HR2046 / ADS7843 touch controller.

* Fully extends the Adafruit GFX library, giving you access to all standard graphics and text functions.
* Five driver classes (`DIYables_ILI9341_SPI`, `DIYables_ILI9488_SPI`, `DIYables_ST7789_SPI`, `DIYables_ST7796_SPI`, `DIYables_ST7735_SPI`) sharing a common API — switch chips by changing one line.
* Built-in support for the **XPT2046 / HR2046 / ADS7843** SPI touch controller commonly found on these modules — no separate touch library required.
* Compatible with a wide range of boards, including Arduino Uno R3, Uno R4 WiFi/Minima, Uno Q, Nano, Mega, Giga R1 WiFi, MKR WiFi 1010, Nano 33 IoT, Nano ESP32, ESP32, and ESP8266.

With this library, you can easily create colorful, high-resolution graphical interfaces — menus, dashboards, image viewers, touch buttons — for your Arduino, ESP32, ESP8266, or SAMD-based projects.

![DIYables TFT SPI Display](https://newbiely.com/images/tutorial/ili9341-ili9488-st7789-tft-lcd-touch-display-spi-interface-pinout.jpg)



Supported Driver ICs
----------------------------
The library supports five popular SPI TFT controllers:

| Driver Class             | Resolution                                              | Color Format            | Max SPI Speed |
|--------------------------|---------------------------------------------------------|-------------------------|---------------|
| `DIYables_ILI9341_SPI`   | 240 × 320                                               | 16-bit RGB565           | 40 MHz        |
| `DIYables_ILI9488_SPI`   | 320 × 480                                               | 18-bit RGB666 (SPI)     | 24 MHz        |
| `DIYables_ST7789_SPI`    | 240 × 320, 76 × 284 (2.25" bar display)                 | 16-bit RGB565           | 40 MHz (76 × 284: 20 MHz) |
| `DIYables_ST7796_SPI`    | 320 × 480                                               | 16-bit RGB565           | 40 MHz        |
| `DIYables_ST7735_SPI`    | 128 × 160 (also 128×128, 80×160)                        | 16-bit RGB565           | 27 MHz        |

* ILI9488 in SPI mode supports only 18-bit color. The library automatically converts RGB565 → RGB666 during transfer.
* ST7796 (ST7796S) has the same 320 × 480 resolution as ILI9488 but accepts 16-bit RGB565 over SPI, so it transfers 2 bytes per pixel instead of 3 and draws faster.
* `DIYables_ST7789_SPI` picks the panel settings from the size you pass. For the 2.25" 76 × 284 bar display, use `DIYables_ST7789_SPI tft(76, 284, CS, DC, RST);`. Its visible area sits at an offset inside the ST7789 memory, and the driver adds that offset for you. Its backlight is active-LOW: wire `BL` to GND (3.3V keeps it off), or wire it to a pin and call `tft.initBacklight(pin, true)`. It has no touch controller. Its example sketches are in `docs/2.25-inch-76x284-code/` (copies with other pins are in the `docs/<board>-code/` folders), and the 2.25" tutorials listed below show the wiring for each board. Any size other than 76 × 284 uses the 240 × 320 module's settings.
* ST7735 modules have no touch controller, so the touch functions are not used with `DIYables_ST7735_SPI`. ST7735 modules differ in panel offset and red/blue order; if you see a line of random pixels along an edge or swapped colors, call `setPanelType()` before `begin()`, e.g. `tft.setPanelType(DIYables_ST7735_SPI::PANEL_128x160_GREENTAB);`. The examples do not include it because these modules use a different pinout (`GND, VCC, SCL, SDA, RST, DC, CS, BL`): SCL = SPI SCK, SDA = SPI MOSI, BL = backlight.
* **Recommendation:** for new projects, choose **ST7789** — widely available, runs at full 40 MHz SPI speed, and is the most straightforward to use.



Product Link
----------------------------
* [DIYables SPI TFT LCD Display Modules](https://diyables.io/tft-spi)



Features
----------------------------
* Works with any Arduino-compatible board that has a hardware SPI bus.
* Hardware SPI on any SPI bus (`SPI`, `SPI1`, `HSPI`, or user-defined `SPIClass` instances on ESP32).
* Backlight control with optional **PWM dimming** (`setBacklight(0..255)`).
* **Two touch backends** sharing the same `getTouch()` / `readTouchRaw()` API:
  * **XPT2046 / HR2046 / ADS7843** SPI touch controller — pins labeled `T_CS`, `T_CLK`, `T_DIN`, `T_DO`, `T_IRQ` (touch IC shares the display's SPI bus).
  * **4-wire bare resistive** panel — pins labeled `XP`/`X+`, `YP`/`Y+`, `XM`/`X-`, `YM`/`Y-`.
* SD-card image loading example using the same shared SPI bus.
* Touch calibration utility example.
* Works with both 3.3V and 5V boards (Uno, Nano, Mega, Uno R4) — the modules have on-board level shifting, so direct wiring is tested and works; a 5V to 3.3V level converter is still the safer choice for permanent builds.



Available Examples
----------------------------
* **DrawShapes.ino** — Draw circles, rectangles, triangles, diamonds, and round rectangles using the library's graphics functions.
* **ShowTextAndNumber.ino** — Display text strings and numbers in different fonts, sizes, and colors.
* **DrawImage.ino** — Display a bitmap image stored in program memory (flash) directly on the TFT screen.
* **DrawImageSDcard.ino** — Load and display a BMP image from an SD card.
* **UseExternalFont.ino** — Use and display external (custom) Adafruit GFX fonts for enhanced text appearance.
* **CustomSPI.ino** — Use a secondary or user-defined SPI bus for the display (useful on ESP32 / Giga R1 with multiple SPI peripherals).
* **ColligateTest.ino** — Comprehensive graphics benchmark: runs 14 drawing tests (shapes, curves, random primitives) and reports the time each one takes, on screen and over Serial.
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
* [Arduino Uno Q — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-uno-q/arduino-uno-q-tft-lcd-touch-display-spi)
* [Arduino Giga R1 WiFi — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-giga/arduino-giga-r1-wifi-tft-lcd-touch-display-spi)
* [Arduino MKR WiFi 1010 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-tft-lcd-touch-display-spi)
* [Arduino Nano 33 IoT — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-tft-lcd-touch-display-spi)
* [Arduino Nano ESP32 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/arduino-nano-esp32/arduino-nano-esp32-tft-lcd-touch-display-spi)
* [ESP32 — TFT LCD Touch Display (SPI)](https://esp32io.com/tutorials/esp32-tft-lcd-touch-display-spi)
* [ESP8266 — TFT LCD Touch Display (SPI)](https://newbiely.com/tutorials/esp8266/esp8266-tft-lcd-touch-display-spi)

2.25" 76 × 284 TFT LCD display (ST7789, no touch):

![2.25 inch IPS TFT LCD Display Screen 76x284 ST7789 SPI](https://diyables.io/images/products/2.25-inch-ips-tft-lcd-display-screen-76x284-st7789-spi-color-module-2.jpg)

* [Arduino — 2.25 inch TFT LCD Display Screen 76x284](https://arduinogetstarted.com/tutorials/arduino-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Nano — 2.25 inch TFT LCD Display Screen 76x284](https://arduinogetstarted.com/tutorials/arduino-nano-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Mega — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-mega/arduino-mega-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Uno R4 — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Uno Q — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-uno-q/arduino-uno-q-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Giga R1 WiFi — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-giga/arduino-giga-r1-wifi-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino MKR WiFi 1010 — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Nano 33 IoT — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-2.25-inch-tft-lcd-display-screen-76x284)
* [Arduino Nano ESP32 — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/arduino-nano-esp32/arduino-nano-esp32-2.25-inch-tft-lcd-display-screen-76x284)
* [ESP32 — 2.25 inch TFT LCD Display Screen 76x284](https://esp32io.com/tutorials/esp32-2.25-inch-tft-lcd-display-screen-76x284)
* [ESP32 Uno — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/esp32-uno/esp32-uno-2.25-inch-tft-lcd-display-screen-76x284)
* [ESP32 S3 Uno — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/esp32-s3-uno/esp32-s3-uno-2.25-inch-tft-lcd-display-screen-76x284)
* [ESP32 S3 — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/esp32-s3/esp32-s3-2.25-inch-tft-lcd-display-screen-76x284)
* [ESP32 C3 Super Mini — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/esp32-c3/esp32-c3-super-mini-2.25-inch-tft-lcd-display-screen-76x284)
* [ESP8266 — 2.25 inch TFT LCD Display Screen 76x284](https://newbiely.com/tutorials/esp8266/esp8266-2.25-inch-tft-lcd-display-screen-76x284)



References
----------------------------
* [DIYables_TFT_SPI Library Reference](https://newbiely.com/library-references/diyables-tft-spi-library-reference.md)



Supported and Tested Hardware
----------------------------

| Board                                                                                                          | Tested | Logic Level | Notes                              |
|----------------------------------------------------------------------------------------------------------------|:------:|:-----------:|------------------------------------|
| Arduino Uno R3                                                                                                 |   ✅   |     5V      | Direct wiring (converter optional)  |
| Arduino Nano                                                                                                   |   ✅   |     5V      | Direct wiring (converter optional)  |
| Arduino Mega 2560                                                                                              |   ✅   |     5V      | Direct wiring (converter optional)  |
| Arduino Uno R4 Minima                                                                                          |   ✅   |     5V      | Direct wiring (converter optional)  |
| Arduino Uno R4 WiFi                                                                                            |   ✅   |     5V      | Direct wiring (converter optional)  |
| Arduino Uno Q                                                                                                  |   ✅   |    3.3V     | Direct wiring                      |
| Arduino Giga R1 WiFi                                                                                           |   ✅   |    3.3V     | Direct wiring                      |
| Arduino MKR WiFi 1010                                                                                          |   ✅   |    3.3V     | Direct wiring                      |
| Arduino Nano 33 IoT                                                                                            |   ✅   |    3.3V     | Direct wiring                      |
| Arduino Nano ESP32                                                                                             |   ✅   |    3.3V     | Direct wiring                      |
| ESP32 (DevKit / WROOM / WROVER)                                                                                |   ✅   |    3.3V     | Direct wiring; multiple SPI buses  |
| ESP8266 (NodeMCU / Wemos D1 mini)                                                                              |   ✅   |    3.3V     | Direct wiring                      |
| Other boards with hardware SPI                                                                                 |   —    |     —       | Should work; not officially tested |



**Note**: This library is designed for **SPI** TFT LCD modules (ILI9341, ILI9488, ST7789, ST7796, ST7735 — typically labeled with pins `CS`, `DC`, `RST`, `MOSI`, `SCK`, `MISO`, and optionally `T_CS`/`T_CLK`/`T_DIN`/`T_DO`/`T_IRQ` for the XPT2046 / HR2046 touch controller). If you are using a **parallel-interface 3.5" TFT Touch Shield** (28-pin Arduino shield form factor with ILI9488 or RM68140 driver), please use the [DIYables TFT Touch Shield Library](https://github.com/DIYables/DIYables_TFT_Touch_Shield) instead.


Version History
----------------------------

**v1.1.0**

* Added **ST7796 / ST7796S** support (320 × 480, 16-bit RGB565) with the new `DIYables_ST7796_SPI` class. It has the same resolution as the ILI9488 but draws faster, because it sends 2 bytes per pixel instead of 3.
* Added **ST7735** support with the new `DIYables_ST7735_SPI` class, for 1.8" 128 × 160, 1.44" 128 × 128 and 0.96" 80 × 160 modules. If a module shows a line of random pixels along an edge or swapped colors, choose its panel type with `setPanelType()` before `begin()`.
* Added support for the **2.25" 76 × 284 ST7789 bar display**: `DIYables_ST7789_SPI tft(76, 284, CS, DC, RST);`. The driver sets the panel offset, color order and SPI clock from the size you pass. Example sketches and a tutorial for each board are listed under Tutorials.
* `initBacklight()` takes an optional second argument, `active_low`, for modules whose backlight turns on when the pin is LOW (such as the 2.25" display): `tft.initBacklight(pin, true);`. `backlightOn()`, `backlightOff()` and `setBacklight()` follow it.
* Fixed a freeze when drawing a line or a filled shape that goes past the left or top edge of the screen. A part lying completely off-screen was sent to the display as billions of pixels.
* Touch examples: added a note for modules where touch does not respond because the display's SDO / MISO pin holds the shared MISO line. Unplug the display's SDO / MISO wire and keep `T_DO` connected.
* Examples: added a commented-out `DIYables_ST7796_SPI` line next to the other driver choices.
* Existing sketches for ILI9341, ILI9488 and 240 × 320 ST7789 work without changes.

**v1.0.2**

* Fixed **Arduino Uno Q** support:
  * Examples now compile — a UTF-8 BOM in six example sketches broke the build.
  * Chip Select works again — D10 is the SPI2 hardware NSS pin on this board and was taken over by the SPI peripheral, so the display never saw its CS.
  * Display no longer stays blank — the automatic SPI clock is capped at 16 MHz on this board; the 24-40 MHz defaults corrupt the panel's init sequence.
  * Much faster drawing — pixel data is sent in blocks instead of one byte per SPI transaction.
* Fixed **Arduino Uno R3**, **Uno R4** (and every other board): the same BOM issue prevented six examples from compiling.
* Added `setTouchInvertScreenX()` and `setTouchInvertScreenY()` for panels that report the touch point mirrored on screen. Unlike `setTouchInvertX()` / `setTouchInvertY()`, these work in screen coordinates, so they behave the same at any `setRotation()`.

**v1.0.1**

* Initial public release.
