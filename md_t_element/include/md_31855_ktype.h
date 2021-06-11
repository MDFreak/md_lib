/*-----------------------------------------------------
 * Description
 *
 */
#ifndef MD_31855KYPE_HPP
  #define MD_31855KYPE_HPP

  #include "Arduino.h"
  #include <Adafruit_SPIDevice.h>
  #include <md_defines.h>

// class md_31855_ktype
class md_31855_ktype
  {
    private:
      Adafruit_SPIDevice _spi;
      bool               _isInit = false;
      int16_t            _actT   = 0;     // unit 0.25 °C ( /4 )
      int16_t            _refT   = 0;     // unit 0.0625°C ( *5/80 )
      uint8_t            _error  = ISOK;

    public:
      md_31855_ktype(int8_t _cs, int8_t _sclk, int8_t _miso);
      md_31855_ktype(int8_t _cs, SPIClass *_spi = &SPI);

      uint8_t  readErr()   { return _error; }
      uint8_t  begin(void);
      int16_t  refT(void);
      int16_t  actT(void);
      int16_t  getFahrenheit(void);

    //private:
      void     readKType(void);
      uint32_t spiread32(void);
  };

#endif
