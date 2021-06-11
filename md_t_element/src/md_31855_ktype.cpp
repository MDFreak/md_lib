
#include <md_31855_ktype.h>
  #ifdef __AVR
    #include <avr/pgmspace.h>
  #elif defined(ESP8266)
    #include <pgmspace.h>
  #endif

#include <stdlib.h>

// class md_31855_ktype
  // constructors
    // constructor using software SPI
      /*  _sclk   pin to use for SPI Serial Clock.
       *  _cs     pin to use for SPI Chip Select.
       *  _miso   pin to use for SPI Master In Slave Out.
       */
    md_31855_ktype::md_31855_ktype(int8_t _cs, int8_t _sclk, int8_t _miso)
        : _spi(_cs, _sclk, _miso, -1, 1000000) {}
      //{
      //  _spi = Adafruit_SPIDevice(_cs, _sclk, _miso, -1, 1000000);
      //}

    // constructor using hardware SPI
      /* _cs      pin to use for SPI Chip Select.
       * _spi     spi object
      */
    md_31855_ktype::md_31855_ktype(int8_t _cs, SPIClass *_spi)
        : _spi(_cs, 1000000, SPI_BITORDER_MSBFIRST, SPI_MODE0, _spi) {}
      //{
      //  _spi = SPIAdafruit_SPIDevice(_cs, 1000000,SPI_BITORDER_MSBFIRST, SPI_MODE0, _spi);
      //}

    // implementation
    uint8_t  md_31855_ktype::begin(void)
      {
        if ( _spi.begin() == true)
          { return (uint8_t) ISOK; }
        else
          { return (uint8_t) ISERR; }
      }

    /*- provide reference value unit 1°C - no measurement
     *
     * if error return = NN = -1°C
     */
    int16_t  md_31855_ktype::refT()
      {
        int16_t val = NN;
        if (!_error)
          {
            // convert unit 0.0625°C to unit 1°C
            val = _refT / 16;
          }
        return val;
      }

    /*- read sensor and provide sensor value unit 1°C
     *
     * if error return = NN = -1°C
     */
    int16_t  md_31855_ktype::actT(void)
      {
        int16_t val = NN;
        readKType();
        if (_error == ISOK)
          {
            // convert to unit 1°C
            val = _actT/4;
          }
        return val;
      }

    /*- read value as Fahrennheit */
    int16_t  md_31855_ktype::getFahrenheit(void)
      {
        int16_t val = NN;
        if (_error == ISOK)
          {
            // convert to unit 1°C (/4) and 1° Fahrenheit ( (*9/5) + 32 )
            val = _actT * 9;
            val /= 20;
            val += 32;
          }
        return val;
      }

    /*- readKType read and evaluate sensor ---------------------
     *
     * The raw 32 bit value read.
     * data format
     *   3         2         1
     *  10987654321098765432109876543210
     *  ||||||||||||||*|||||||||||||*||+- 1: error open circuit
     *  ||||||||||||||*|||||||||||||*|+-- 1: error GND
     *  ||||||||||||||*|||||||||||||*+--- 1: error VCC
     *  ||||||||||||||*|||||||||||||+---- not used
     *  ||||||||||||||*||||||||||||+----- ref val 0.0625°C
     *  ||||||||||||||*||+++++++++++----- reference value
     *  ||||||||||||||*|+---------------- ref val 64°C
     *  ||||||||||||||*+----------------- 1: error present
     *  ||||||||||||||+------------------ not used
     *  |||||||||||||+------------------- meas val 0.25°C
     *  ||+++++++++++-------------------- measurement value
     *  |+------------------------------- meas val 1024°C
     *  +-------------------------------- sign 1: <0
     *-----------------------------------------------------
     *------------------------------------------------*/
    void     md_31855_ktype::readKType()
      {
        uint32_t val = spiread32();
                //SOUT(" read KType val "); SOUTHEX(val);

        // extract error
          _error = val % 8;   // all error flags
          val >>= 4;                   // ref is valid
                //SOUT(" err "); SOUTHEX(_error);
                //SOUT(" val neu "); SOUTHEX(val);

        // extract reference value
          if ( (val & 0x00000800) == 0) // ref >= 0
              {
                _refT = val & 0x07FF;
              }
            else  // ref negativ
              {
                _refT = 0xF800 | ( val & 0x07FF);
              }
                //SOUT(" refT*16 "); SOUTHEX(_refT);
                //SOUT(" / "); SOUT(_refT);
          // convert unit 0.0625 °C to unit 1°C
            //_refT = _refT / 16;
          val >>= 14;                  // sensor value valid
                //SOUT(" refT "); SOUT(_refT / 16);
                //SOUT(" val neu "); SOUTHEXLN(val);

        // extract measurement value
          if ( (val & 0x00002000) == 0) // val >= 0
              {
                _actT = val & 0x1FFF;
              }
            else  // ref negativ
              {
                _actT = 0xE000 | ( val & 0x1FFF);
              }
                //SOUT(" actT*4 "); SOUTHEX(_actT);
                //SOUT(" / "); SOUT(_actT);
                //SOUT(" actT "); SOUTLN(_actT / 4);
          // convert unit 0.25 °C to unit 1°C
            //_actT /= 4;
      }

    /*- SPI read32 -----------------------------------
     * Read 4 bytes (32 bits) from breakout over SPI.
     * The raw 32 bit value read.
     *------------------------------------------------*/
    uint32_t md_31855_ktype::spiread32(void)
      {
        uint32_t  val;
        uint8_t   buf[4];
        uint32_t* pbuf = (uint32_t*) &buf[0];

        _spi.read(buf, 4);        // MSB first
        val  = buf[0]; val <<= 8;
        val |= buf[1]; val <<= 8;
        val |= buf[2]; val <<= 8;
        val |= buf[3];
             //SOUT(" spi buf[0-3] "); SOUTHEX(*pbuf);
             //SOUT(" result "); SOUTHEXLN(val);
        return val;
      }

// unused
  #ifdef XXXX
      int16_t md_31855_ktype::refT(void)
        {
          uint32_t v;

          v = spiread32();

          // ignore bottom 4 bits - they're just thermocouple data
          v >>= 4;

          // pull the bottom 11 bits off
          float internal = v & 0x7FF;
          // check sign bit!
          if (v & 0x800) {
            // Convert to negative value by extending sign and casting to signed type.
            int16_t tmp = 0xF800 | (v & 0x7FF);
            internal = tmp;
          }
          internal *= 0.0625; // LSB = 0.0625 degrees
          // Serial.print("\tInternal Temp: "); Serial.println(internal);
          return internal;
        }

      /**************************************************************************/
      /*!
          @brief  Read the thermocouple temperature.

          @return The thermocouple temperature in degrees Celsius.
      */
      /**************************************************************************/
      double Adafruit_MAX31855::readCelsius(void) {

        int32_t v;

        v = spiread32();

        // Serial.print("0x"); Serial.println(v, HEX);

        /*
        float internal = (v >> 4) & 0x7FF;
        internal *= 0.0625;
        if ((v >> 4) & 0x800)
          internal *= -1;
        Serial.print("\tInternal Temp: "); Serial.println(internal);
        */

        if (v & 0x7) {
          // uh oh, a serious problem!
          return NAN;
        }

        if (v & 0x80000000) {
          // Negative value, drop the lower 18 bits and explicitly extend sign bits.
          v = 0xFFFFC000 | ((v >> 18) & 0x00003FFF);
        } else {
          // Positive value, just drop the lower 18 bits.
          v >>= 18;
        }
        // Serial.println(v, HEX);

        double centigrade = v;

        // LSB = 0.25 degrees C
        centigrade *= 0.25;
        return centigrade;
      }



    #endif