/**
    The MIT License (MIT)
    Copyright (c) 2019 by Daniel Eichhorn, ThingPulse
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    Please note: We are spending a lot of time to write and maintain open source codes
    Please support us by buying our products from https://thingpulse.com/shop/

    See more at https://thingpulse.com

    Many thanks go to various contributors such as Adafruit, Waveshare.
  */

#ifndef _MDGRAFX_ILI9341H_
  #define _MDGRAFX_ILI9341H_

  // --- includes
    //#include <DisplayDriver.h>
    #include <md_SPI.h>

  // --- uC declarations
     #include "Arduino.h"
     #include "Print.h"
     #include <pgmspace.h>

  // --- display declarations
    #define ILI9341_TFTWIDTH  240
    #define ILI9341_TFTHEIGHT 320

    #define ILI9341_NOP     0x00
    #define ILI9341_SWRESET 0x01
    #define ILI9341_RDDID   0x04
    #define ILI9341_RDDST   0x09

    #define ILI9341_SLPIN   0x10
    #define ILI9341_SLPOUT  0x11
    #define ILI9341_PTLON   0x12
    #define ILI9341_NORON   0x13

    #define ILI9341_RDMODE  0x0A
    #define ILI9341_RDMADCTL  0x0B
    #define ILI9341_RDPIXFMT  0x0C
    #define ILI9341_RDIMGFMT  0x0D
    #define ILI9341_RDSELFDIAG  0x0F

    #define ILI9341_INVOFF  0x20
    #define ILI9341_INVON   0x21
    #define ILI9341_GAMMASET 0x26
    #define ILI9341_DISPOFF 0x28
    #define ILI9341_DISPON  0x29

    #define ILI9341_CASET   0x2A
    #define ILI9341_PASET   0x2B
    #define ILI9341_RAMWR   0x2C
    #define ILI9341_RAMRD   0x2E

    #define ILI9341_PTLAR   0x30
    #define ILI9341_MADCTL  0x36
    #define ILI9341_PIXFMT  0x3A

    #define ILI9341_FRMCTR1 0xB1
    #define ILI9341_FRMCTR2 0xB2
    #define ILI9341_FRMCTR3 0xB3
    #define ILI9341_INVCTR  0xB4
    #define ILI9341_DFUNCTR 0xB6

    #define ILI9341_PWCTR1  0xC0
    #define ILI9341_PWCTR2  0xC1
    #define ILI9341_PWCTR3  0xC2
    #define ILI9341_PWCTR4  0xC3
    #define ILI9341_PWCTR5  0xC4
    #define ILI9341_VMCTR1  0xC5
    #define ILI9341_VMCTR2  0xC7

    #define ILI9341_RDID1   0xDA
    #define ILI9341_RDID2   0xDB
    #define ILI9341_RDID3   0xDC
    #define ILI9341_RDID4   0xDD

    #define ILI9341_GMCTRP1 0xE0
    #define ILI9341_GMCTRN1 0xE1

    //#define ILI9341_PWCTR6  0xFC

  // --- color definitions
    #define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
    #define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
    #define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
    #define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
    #define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
    #define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
    #define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
    #define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
    #define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
    #define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
    #define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
    #define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
    #define ILI9341_RED         0xF800      /* 255,   0,   0 */
    #define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
    #define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
    #define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
    #define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
    #define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
    #define ILI9341_PINK        0xF81F

  // --- class DisplayDriver
    struct BufferInfo
      {
        uint8_t  *buffer;      // the frame buffer
        uint8_t   bitsPerPixel;// number of bits used for one pixel
        uint16_t *palette;     // the palette in case the framebuffer uses a different bit depth than the display
        uint16_t  windowX;     // x origin of the area to copy
        uint16_t  windowY;     // y origin of the area to copy
        uint16_t  windowWidth; // width of the area to be copied
        uint16_t  windowHeight;// height of the area to be copied
        uint16_t  targetX;     // x where to copy the window to
        uint16_t  targetY;     // y where to copy the window to
        uint16_t  bufferWidth; // width of the source buffer
        uint16_t  bufferHeight;// height of the source buffer
      };

    class DisplayDriver
      {
        public:
          DisplayDriver(int16_t w, int16_t h); // Constructor

          virtual void setRotation(uint8_t r);
          virtual void init(uint8_t spi_bus = VSPI) = 0;
          virtual void writeBuffer(BufferInfo *bufferInfo) = 0;
          virtual void setFastRefresh(boolean isEnabled) = 0;

          int16_t height     (void) const;
          int16_t width      (void) const;
          uint8_t getRotation(void) const;

        protected:
          const int16_t
            WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
          int16_t _width = 0, _height = 0;
          uint8_t rotation;
          boolean isFastRefreshEnabled = false;
      };

  // --- class md_ILI9341_SPI
    class mdILI9341_SPI : public DisplayDriver
      {
        public:
          mdILI9341_SPI(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST, int8_t _MISO); // SW-SPI
          mdILI9341_SPI(int8_t _CS, int8_t _DC, int8_t _RST = -1, int16_t = ILI9341_TFTWIDTH, int16_t _H = ILI9341_TFTHEIGHT); // HW-SPI

          void init          (uint8_t spi_bus = VSPI);
          void spi_begin     ();
          void spi_end       ();
          void setAddrWindow (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
          void setRotation   (uint8_t r);

          void writeBuffer   (BufferInfo *bufferInfo);

          void pushColor     (uint16_t color);
          void setFastRefresh(boolean isFastRefreshEnabled);

          void spiwrite      (uint8_t);
          void writecommand  (uint8_t c);
          void writedata     (uint8_t d);

         private:
          mdSPI_class* _pmdSPI;
          SPIClass*    _pSPI;
          boolean      hwSPI;
          uint8_t      _isON = 0;
          int32_t _cs, _dc, _rst, _mosi, _miso, _sclk;
      };

#endif
