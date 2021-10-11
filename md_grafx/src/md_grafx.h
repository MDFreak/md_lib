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

#ifndef _MD_GRAFX_ILI9341_H_
  #define _MD_GRAFX_ILI9341_H_

  // --- includes
    #include <FS.h>
    #if defined(ESP32)
      #include <SPIFFS.h>
    #endif
    #include <md_ILI9341_SPI.h>
    #include <MiniGrafxFonts.h>
    #include <md_defines.h>

  // --- color defines
    #define MD4_BLACK         0
    #define MD4_WHITE         1
    #define MD4_YELLOW        2
    #define MD4_BLUE          3

    #define MD16_BLACK        MD4_BLACK
    #define MD16_WHITE        MD4_WHITE
    #define MD16_YELLOW       MD4_YELLOW
    #define MD16_BLUE         MD4_BLUE
    #define MD16_NAVY         4
    #define MD16_DARKGREEN    5
    #define MD16_ORANGE       6
    #define MD16_DARKCYAN     7
    #define MD16_PURPLE       8
    #define MD16_GREENYELLOW  9
    #define MD16_DARKGREY     10
    #define MD16_LIGHTGRAY    11
    #define MD16_GREEN        12
    #define MD16_CYAN         13
    #define MD16_RED          14
    #define MD16_PINK         15

  // --- Header Values
    #define JUMPTABLE_BYTES 4

    #define JUMPTABLE_LSB   1
    #define JUMPTABLE_SIZE  2
    #define JUMPTABLE_WIDTH 3
    #define JUMPTABLE_START 4

    #define WIDTH_POS       0
    #define HEIGHT_POS      1
    #define FIRST_CHAR_POS  2
    #define CHAR_NUM_POS    3

    #define CUSTOM_BITMAP_DATA_START 6

  // --- structures, macros
    #undef max
    #define max(a,b) ((a)>(b)?(a):(b))

    #undef min
    #define min(a,b) ((a)<(b)?(a):(b))

    #ifndef _swap_int16_t
        #define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
      #endif

    enum TEXT_ALIGNMENT
      {
        TXT_ALIGN_LEFT = 0,
        TXT_ALIGN_RIGHT = 1,
        TXT_ALIGN_CENTER = 2,
        TXT_ALIGN_CENTER_BOTH = 3
      };

    enum BUFFER_COLOR_DEPTH
      {
        BIT_1 = 1,
        BIT_2 = 2,
        BIT_4 = 4,
        BIT_8 = 8,
        BIT_16 = 16
      };

    typedef struct // hearder of BMP file
      {
        int8_t   version  = 1;     //
        int8_t   bitDepth = 2;     // 2 bits/color => 4 colors, 4=>16, 8=>256
        uint16_t width    = 32;    // 0x20
        uint16_t height   = 32;    //
        uint16_t size     = 0;
      } BMPHeader_t;

  class mdGrafx
    {
      public:
        mdGrafx(uint8_t bitsPerPixel, uint16_t *palette);
        mdGrafx(uint8_t bitsPerPixel, uint16_t *palette, uint16_t width, uint16_t height);

        void init(DisplayDriver *driver, uint8_t spi_bus = VSPI);
        void changeBitDepth(uint8_t bitsPerPixel, uint16_t *palette);

        void setRotation(uint8_t r);
        void setMirroredHorizontally(boolean isMirroredHorizontally);
        void setMirroredVertically(boolean isMirroredVertically);
        void setFont(const char *fontData);
        void setFontFile(String fontFile);
        void setTextAlignment(TEXT_ALIGNMENT textAlignment);
        void setColor(uint16_t color);
        void setTransparentColor(uint16_t transparentColor);

        uint16_t getHeight();
        uint16_t getWidth();
        uint16_t getPixel(uint16_t x, uint16_t y);
        uint16_t getStringWidth(const char* text, uint16_t length);
        uint16_t read16(File &f);
        uint32_t read32(File &f);

        void setPixel(uint16_t x, uint16_t y);

        void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
        void drawCircle(int16_t x0, int16_t y0, uint16_t radius);
        void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);
        void fillRect(int16_t xMove, int16_t yMove, int16_t width, int16_t height);
        void fillCircle(int16_t x0, int16_t y0, int16_t radius);
        void drawHorizontalLine(int16_t x, int16_t y, int16_t length);
        void drawVerticalLine(int16_t x, int16_t y, int16_t length);
        void drawStringInternal(int16_t xMove, int16_t yMove, char* text, uint16_t textLength, uint16_t textWidth);
        void fillBottomFlatTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
        void fillTopFlatTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
        void fillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
        void drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

        uint16_t drawString(int16_t xMove, int16_t yMove, String strUser, TEXT_ALIGNMENT alignment = TXT_ALIGN_LEFT);
        void drawStringLineBreak(int16_t x, int16_t y, uint16_t maxLineWidth, String text);
        void drawStringMaxWidth(int16_t x, int16_t y, uint16_t maxLineWidth, String text);
        void drawStatus(String msg, int16_t wifiQual, int16_t y=320,
                        const char* pfont = ArialMTPlain_10,
                        uint16_t col16=ILI9341_PINK, uint16_t backcol16=ILI9341_BLACK);

        void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char *xbm);
        void drawBmpFromFile(String filename, uint8_t x, uint16_t y);
        void drawBmpFromPgm(const char *xbm, uint8_t x, uint16_t y);
        void getPalBitmapHeadFromPgm(BMPHeader_t* head, const char *palBmp);
        void drawPalettedBitmapFromPgm(uint16_t x, uint16_t y, const char *palBmp);
        void drawPalettedBitmapFromFile(uint16_t x, uint16_t y, String fileName);
        void inline drawInternal(int16_t xMove, int16_t yMove,
                                 int16_t width, int16_t height,
                                 const char *data, uint16_t offset, uint16_t bytesInData);

        void commit();
        void commit(uint16_t xPos, uint16_t yPos);
        void commit(uint16_t srcXPos, uint16_t srcYPos,
                    uint16_t srcWidth, uint16_t srcHeight,
                    uint16_t targetXPos, uint16_t targetYPos);

        void clear();
        void freeBuffer();
        void setFastRefresh(boolean isFastRefreshEnabled);
        void fillBuffer(uint8_t pal);
        static char* utf8ascii(String s);
        static byte  utf8ascii(byte ascii);
        void colorSwap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       uint16_t color1, uint16_t color2);

      private:
        uint8_t readFontData(const char * start, uint32_t offset);
        void    initializeBuffer();
        DisplayDriver*  _pdriver;
        File            _fontFile;
        uint16_t        _width = 0, _height = 0;
        uint16_t        _initialWidth = 0, _initialHeight = 0;
        uint16_t        _color;
        uint8_t         _rotation;
        int16_t         _transparentColor = -1;
        uint8_t         _bitsPerPixel = 4;
        uint8_t         _bitShift = 2;
        uint16_t        _bufferSize = 0;
        uint16_t       *_palette = 0;
        uint8_t        *_buffer = 0;
        uint16_t        _bitMask = 0x000F;
        uint8_t         _pixelsPerByte = 2;
        boolean         _isPgmFont = true;
        boolean         _isMirroredHorizontally = false;
        boolean         _isMirroredVertically = false;
        const char     *_fontData = ArialMTPlain_16;
        TEXT_ALIGNMENT  _textAlignment;
    };

#endif
