/** The MIT License (MIT)
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

/* If the SPI library has transaction support, these functions
   establish settings and protect from interference from other
   libraries.  Otherwise, they simply do nothing.
  */

// --- includes
  #include <md_ILI9341_SPI.h>
  #include <limits.h>
  #include <md_defines.h>
  //#include <md_SPI.h>

  //#include <pgmspace.h>

  #ifndef ARDUINO_STM32_FEATHER
      #include "pins_arduino.h"
      #include "wiring_private.h"
    #endif

// --- Macros
  //#define FILLARRAY(a,n) a[0]=n, memcpy( ((char*)a)+sizeof(a[0]), a, sizeof(a)-sizeof(a[0]) );

  #if defined (ARDUINO_ARCH_ARC32)
      #define SPISETTINGS   SPISettings(16000000, MSBFIRST, SPI_MODE0)
    // max speed!
    // SPISettings settings_ = {16000000, MSBFIRST, SPI_MODE0};

  #else
      #define SPISETTINGS  SPISettings(40000000, MSBFIRST, SPI_MODE0)
      // max speed!
      //SPISettings settings_ = {40000000, MSBFIRST, SPI_MODE0};
    #endif

//---------------------------------------------
// --- class DisplayDriver
  DisplayDriver::DisplayDriver(int16_t w, int16_t h):
                               WIDTH(w), HEIGHT(h)
    {
      _width    = WIDTH;
      _height   = HEIGHT;
      rotation  = 0;
    }

  uint8_t DisplayDriver::getRotation(void) const
    { return rotation; }

  void DisplayDriver::setRotation(uint8_t x)
    {
      rotation = (x & 3);
      switch(rotation)
        {
          case 0:
          case 2:
            _width  = WIDTH;
            _height = HEIGHT;
            break;
          case 1:
          case 3:
            _width  = HEIGHT;
            _height = WIDTH;
            break;
          default:
            break;
        }
    }

  int16_t DisplayDriver::width(void) const // return display size
    { return _width; }

  int16_t DisplayDriver::height(void) const // return display size
    { return _height; }

//---------------------------------------------
// --- class mdILI9341_SPI
  // Constructor when using software SPI.  All output pins are configurable.
  mdILI9341_SPI::mdILI9341_SPI(int8_t cs, int8_t dc, int8_t mosi,
                               int8_t sclk, int8_t rst, int8_t miso)
                               : DisplayDriver(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT)
    {
      _cs   = cs;
      _dc   = dc;
      _mosi = mosi;
      _miso = miso;
      _sclk = sclk;
      _rst  = rst;
      hwSPI = false;
    }

  // Constructor when using hardware SPI.  Faster, but must use SPI pins
  mdILI9341_SPI::mdILI9341_SPI(int8_t cs, int8_t dc, int8_t rst, int16_t _W, int16_t _H)
                               : DisplayDriver(_W,_H)
    {
      _cs     = cs;
      _dc     = dc;
      _rst    = rst;
      hwSPI   = true;
      _mosi   = _sclk = 0;
    }

  void mdILI9341_SPI::spiwrite(uint8_t c)
    {
      //Serial.print("0x"); Serial.print(c, HEX); Serial.print(", ");
      /* if (hwSPI)
        { */
      #if defined (__AVR__)
        #ifndef SPI_HAS_TRANSACTION
          uint8_t backupSPCR = SPCR;
          SPCR = mySPCR;
        #endif
          SPDR = c;
          while(!(SPSR & _BV(SPIF)));
        #ifndef SPI_HAS_TRANSACTION
          SPCR = backupSPCR;
        #endif
      #else
        if (_isON)
          {
            _pSPI->transfer(c);
          }
      #endif
      /*  }
        else
        {
          #if defined(ESP8266) || defined(ESP32) || defined (ARDUINO_ARCH_ARC32)
              for(uint8_t bit = 0x80; bit; bit >>= 1)
                {
                  if(c & bit)
                      { digitalWrite(_mosi, HIGH); }
                    else
                      { digitalWrite(_mosi, LOW); }
                  digitalWrite(_sclk, HIGH);
                  digitalWrite(_sclk, LOW);
                }
            #else
              // Fast SPI bitbang swiped from LPD8806 library
              for(uint8_t bit = 0x80; bit; bit >>= 1)
                {
                  if(c & bit)
                    {
                      //digitalWrite(_mosi, HIGH);
                      *mosiport |=  mosipinmask;
                    }
                    else
                    {
                      //digitalWrite(_mosi, LOW);
                      *mosiport &= ~mosipinmask;
                    }
                  //digitalWrite(_sclk, HIGH);
                  *clkport |=  clkpinmask;
                  //digitalWrite(_sclk, LOW);
                  *clkport &= ~clkpinmask;
                }
            #endif
        }*/
    }

  void mdILI9341_SPI::setFastRefresh(boolean isFastRefreshEnabled)
    { // Not enabled at the moment
    }

  void mdILI9341_SPI::writecommand(uint8_t c)
    {
      if (_isON)
        {
          #if defined (USE_FAST_PINIO)
            *dcport &= ~dcpinmask;
            *csport &= ~cspinmask;
          #else
            digitalWrite(_dc, LOW);
            digitalWrite(_sclk, LOW);
            digitalWrite(_cs, LOW);
            #endif
          spiwrite(c);
          #if defined (USE_FAST_PINIO)
            *csport |= cspinmask;
          #else
            digitalWrite(_cs, HIGH);
            #endif
        }
    }

  void mdILI9341_SPI::writedata(uint8_t c)
    {
      if (_isON)
        {
          #if defined (USE_FAST_PINIO)
            *dcport |=  dcpinmask;
            *csport &= ~cspinmask;
          #else
            digitalWrite(_dc, HIGH);
            digitalWrite(_cs, LOW);
            #endif
          spiwrite(c);
          #if defined (USE_FAST_PINIO)
            *csport |= cspinmask;
          #else
            digitalWrite(_cs, HIGH);
            #endif
        }
    }

  /* Rather than a bazillion writecommand() and writedata() calls, screen
     initialization commands and arguments are organized in these tables
     stored in PROGMEM.  The table may look bulky, but that's mostly the
     formatting -- storage-wise this is hundreds of bytes more compact
     than the equivalent code.  Companion function follows.
    */
  #define DELAY 0x80
  void mdILI9341_SPI::init(uint8_t spi_bus)
    {
      _pmdSPI = pVSPI;
      _pSPI   = &SPI;
      if (spi_bus == HSPI)
        {
          _pmdSPI = pHSPI;
          //_pSPI   = ?;
        }
      if (_rst > 0)
        {
          pinMode(_rst, OUTPUT);
          digitalWrite(_rst, LOW);
        }
      pinMode(_dc, OUTPUT);
      pinMode(_cs, OUTPUT);
      #if defined (USE_FAST_PINIO)
        csport    = portOutputRegister(digitalPinToPort(_cs));
        cspinmask = digitalPinToBitMask(_cs);
        dcport    = portOutputRegister(digitalPinToPort(_dc));
        dcpinmask = digitalPinToBitMask(_dc);
      #endif

      if (_rst > 0) // toggle RST low to reset
        {
          digitalWrite(_rst, HIGH);
          delay(5);
          digitalWrite(_rst, LOW);
          delay(20);
          digitalWrite(_rst, HIGH);
          delay(150);
        }
      //SOUT(" mdILI9341_SPI::init start_SPI ... ");
      spi_begin();
      // commands
        writecommand(0xEF);
          writedata(0x03);
          writedata(0x80);
          writedata(0x02);

        writecommand(0xCF);
          writedata(0x00);
          writedata(0XC1);
          writedata(0X30);

        writecommand(0xED);
          writedata(0x64);
          writedata(0x03);
          writedata(0X12);
          writedata(0X81);

        writecommand(0xE8);
          writedata(0x85);
          writedata(0x00);
          writedata(0x78);

        writecommand(0xCB);
          writedata(0x39);
          writedata(0x2C);
          writedata(0x00);
          writedata(0x34);
          writedata(0x02);

        writecommand(0xF7);
          writedata(0x20);

        writecommand(0xEA);
          writedata(0x00);
          writedata(0x00);

        writecommand(ILI9341_PWCTR1);    //Power control
          writedata(0x23);   //VRH[5:0]

        writecommand(ILI9341_PWCTR2);    //Power control
          writedata(0x10);   //SAP[2:0];BT[3:0]

        writecommand(ILI9341_VMCTR1);    //VCM control
          writedata(0x3e);
          writedata(0x28);

        writecommand(ILI9341_VMCTR2);    //VCM control2
          writedata(0x86);  //--

        writecommand(ILI9341_MADCTL);    // Memory Access Control
          writedata(0x48);

        writecommand(ILI9341_PIXFMT);
          writedata(0x55);

        writecommand(ILI9341_FRMCTR1);
          writedata(0x00);
          writedata(0x18);

        writecommand(ILI9341_DFUNCTR);    // Display Function Control
          writedata(0x08);
          writedata(0x82);
          writedata(0x27);

        writecommand(0xF2);    // 3Gamma Function Disable
          writedata(0x00);

        writecommand(ILI9341_GAMMASET);    //Gamma curve selected
          writedata(0x01);

        writecommand(ILI9341_GMCTRP1);    //Set Gamma
          writedata(0x0F);
          writedata(0x31);
          writedata(0x2B);
          writedata(0x0C);
          writedata(0x0E);
          writedata(0x08);
          writedata(0x4E);
          writedata(0xF1);
          writedata(0x37);
          writedata(0x07);
          writedata(0x10);
          writedata(0x03);
          writedata(0x0E);
          writedata(0x09);
          writedata(0x00);

        writecommand(ILI9341_GMCTRN1);    //Set Gamma
          writedata(0x00);
          writedata(0x0E);
          writedata(0x14);
          writedata(0x03);
          writedata(0x11);
          writedata(0x07);
          writedata(0x31);
          writedata(0xC1);
          writedata(0x48);
          writedata(0x08);
          writedata(0x0F);
          writedata(0x0C);
          writedata(0x31);
          writedata(0x36);
          writedata(0x0F);

        writecommand(ILI9341_SLPOUT);    //Exit Sleep

      spi_end();
      usleep(120000);
      spi_begin();
      writecommand(ILI9341_DISPON);    //Display on
      spi_end();
      //SOUTLN("ready");
    }

  void mdILI9341_SPI::spi_begin()
    {
      //SOUTLN(" mdILI9341_SPI spi_begin ...");
      if (!_pmdSPI)
        {
          SOUTLN("mdILI9341_SPI::spi_begin _pmdSPI = NULL -> reset!");
          while (1) { sleep(1);}
        }
      _pmdSPI->mount(this, _cs, SPISETTINGS);
      _isON = ON;
      /*
        if (!_pSPI)
          {
            SOUTLN("_pSPI = NULL -> reset!");
            while (1) { sleep(1);}
          }
        SOUT(" mdILI9341_SPI::spi_begin beginTransaction ... ");
        _pSPI->beginTransaction(SPISETTINGS);
        SOUTLN(" ready ");
        */
      /*
        #else
            #define spi_begin()
            #define spi_end()
          #endif
        */
    }

  void mdILI9341_SPI::spi_end()
    {
      _pSPI->endTransaction();
      _pmdSPI->unmount();
      //SOUTLN(" mdILI9341_SPI spi_end");
      _isON = OFF;
      /*
        #else
            #define spi_begin()
            #define spi_end()
          #endif
        */
    }

  void mdILI9341_SPI::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
    {
      writecommand(ILI9341_CASET); // Column addr set
      writedata(x0 >> 8);
      writedata(x0 & 0xFF);     // XSTART
      writedata(x1 >> 8);
      writedata(x1 & 0xFF);     // XEND

      writecommand(ILI9341_PASET); // Row addr set
      writedata(y0>>8);
      writedata(y0);     // YSTART
      writedata(y1>>8);
      writedata(y1);     // YEND

      writecommand(ILI9341_RAMWR); // write to RAM
    }

  #define MADCTL_MY  0x80
  #define MADCTL_MX  0x40
  #define MADCTL_MV  0x20
  #define MADCTL_ML  0x10
  #define MADCTL_RGB 0x00
  #define MADCTL_BGR 0x08
  #define MADCTL_MH  0x04
  void mdILI9341_SPI::setRotation(uint8_t m)
    {
      if (hwSPI) spi_begin();
      writecommand(ILI9341_MADCTL);
      rotation = m % 4; // can't be higher than 3
      switch (rotation)
      {
        case 0:
          writedata(MADCTL_MX | MADCTL_BGR);
          _width  = ILI9341_TFTWIDTH;
          _height = ILI9341_TFTHEIGHT;
          break;
        case 1:
          writedata(MADCTL_MV | MADCTL_BGR);
          _width  = ILI9341_TFTHEIGHT;
          _height = ILI9341_TFTWIDTH;
          break;
        case 2:
          writedata(MADCTL_MY | MADCTL_BGR);
          _width  = ILI9341_TFTWIDTH;
          _height = ILI9341_TFTHEIGHT;
          break;
        case 3:
          writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
          _width  = ILI9341_TFTHEIGHT;
          _height = ILI9341_TFTWIDTH;
          break;
      }
      if (hwSPI) spi_end();
    }

  void mdILI9341_SPI::pushColor(uint16_t color)
    {
      if (hwSPI) spi_begin();
      digitalWrite(_dc, HIGH);
      digitalWrite(_cs, LOW);
      spiwrite(color >> 8);
      spiwrite(color);
      digitalWrite(_cs, HIGH);
      if (hwSPI) spi_end();
    }

  void mdILI9341_SPI::writeBuffer(BufferInfo *bufferInfo)
    {
      //SOUTLN(" ILI..SPI ...");
      spi_begin();
      uint8_t *buffer = bufferInfo->buffer;
      uint16_t bufferWidth = bufferInfo->bufferWidth;
      uint16_t bufferHeight = bufferInfo->bufferHeight;
      uint16_t xPos = bufferInfo->targetX;
      uint16_t yPos = bufferInfo->targetY;
      uint16_t *palette = bufferInfo->palette;
      uint8_t bitsPerPixel = bufferInfo->bitsPerPixel;
      //SOUTLN(" ... begin ok");
      setAddrWindow(xPos, yPos, xPos + bufferWidth - 1, yPos + bufferHeight -1 );
      //Serial.println(" ILI.. before write1");
      digitalWrite(_dc, HIGH);
      //Serial.println(" ILI.. before write2");
      digitalWrite(_cs, LOW);
      if (bitsPerPixel == 16)
        {
          //SOUTLN(" ILI.. before writeBytes");
          _pSPI->writeBytes(buffer, bufferWidth * bufferHeight * 2);
          //SOUTLN(" ILI.. after writeBytes");
            /*for (uint16_t y = 0; y < bufferHeight; y++)
                {
                  for (uint16_t x = 0; x < bufferWidth; x++)
                    {
                    }
                }*/
        }
        else
          {
            // line buffer is in 16bit target format
            uint8_t lineBuffer[_width * 2];
            //Serial
            //uint16_t pos;
            uint8_t bufferByte;
            uint8_t paletteEntry;
            uint16_t color;
            uint8_t shift;
            uint8_t mask = (1 << bitsPerPixel) - 1;
            uint8_t packagesPerBytes = 8 / bitsPerPixel;
            uint16_t bytePos = 0;
            //uint16_t pixelCounter = 0;
            //uint16_t bufferSize = bufferWidth * bufferHeight / packagesPerBytes;
            uint8_t bytesPerLine = bufferWidth / packagesPerBytes;
            uint16_t x = 0;
            for (uint16_t y = 0; y < bufferHeight; y++)
              {
                for (uint16_t b = 0; b < bytesPerLine; b++)
                  {
                    for (uint8_t p = 0; p < packagesPerBytes; p++)
                      {
                        x = b * packagesPerBytes + p;
                        bufferByte = buffer[bytePos];
                        shift = p * bitsPerPixel;
                        paletteEntry = (bufferByte >> shift) & mask;
                        color = palette[paletteEntry];
                        lineBuffer[x * 2] = color >> 8;
                        lineBuffer[x * 2 + 1] = color;
                      }
                    bytePos++;
                  }
                SPI.writeBytes(lineBuffer, bufferWidth * 2);
              }
          }
      digitalWrite(_cs, HIGH);
      spi_end();
    }
