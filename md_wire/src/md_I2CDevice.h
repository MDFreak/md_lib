#ifndef _md_I2CDevice_h_
  #define _md_I2CDevice_h_

  #include <Arduino.h>
  #include "md_Wire.h"

  ///< The class which defines how we will talk to this device over I2C
  class md_I2CDevice
    {
      public:
        uint8_t address(void);

        md_I2CDevice(uint8_t addr, TwoWire *theWire = &Wire);

        bool begin(bool addr_detect = true);
        void end(void);
        bool detected(void);

        bool read(uint8_t *buffer, size_t len, bool stop = true);
        bool write(const uint8_t *buffer, size_t len, bool stop = true,
                   const uint8_t *prefix_buffer = nullptr, size_t prefix_len = 0);
        bool write_then_read(const uint8_t *write_buffer, size_t write_len,
                             uint8_t *read_buffer, size_t read_len,
                             bool stop = false);
        bool setSpeed(uint32_t desiredclk);

        /*!   @brief  How many bytes we can read in a transaction
         *    @return The size of the Wire receive/transmit buffer */
        size_t maxBufferSize() { return _maxBufferSize; }

      private:
        uint8_t  _addr;
        TwoWire *_wire;
        bool     _begun;
        size_t   _maxBufferSize;

        bool _read(uint8_t *buffer, size_t len, bool stop);
    };

#endif // _md_I2CDevice_h_
