#include <Arduino.h>
#include <md_spi.h>
#include <md_defines.h>

mdSPI_class* pVSPI  = NULL;
mdSPI_class* pHSPI  = NULL;

#if (USE_HSPI > OFF)
  SPIClass     mdHSPI = SPIClass();
  #endif
mdSPI_class::mdSPI_class(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t spi_bus)
  {
    _sck  = sck;
    _miso = miso;
    _mosi = mosi;
    _pSPI = &SPI;  // = VSPI
    #if (USE_HSPI > OFF)
      if (spi_bus == HSPI) _pSPI = &mdHSPI;
      #endif
  }

bool mdSPI_class::mount(void* powner, uint8_t ss, SPISettings settings, uint16_t tout_ms)
  {
    _ss = ss;
    uint64_t _start_ms = millis();
    bool     _ret      = true;
    //SOUT(millis());
    if (!_powner)
      {
        do
          {
            //SOUT(" md_spi.mount  mount SPI "); SOUTHEXLN((uint32_t) _pSPI);
            if (((void*) _pSPI->bus())) // bus occupied
              {
                SOUT(" SPI occupied ");
                if ((millis() - _start_ms) > tout_ms)
                  { _ret = false; }
                usleep(200);
              }
            else
              {
                _pSPI->begin(_sck, _miso, _mosi, _ss);
                _pSPI->beginTransaction(settings);
                _powner = powner;
                //SOUT(" md_spi.mount  SPI mounted ");
              }
          }
          while (!_ret);
      }
    else
      {
        SOUT(" mdSPI occupied ");
      }
    //SOUT(" _ret "); SOUT(_ret); SOUT(" "); SOUT(millis() - _start_ms); SOUTLN(" ms");
    return _ret;
  }

void mdSPI_class::unmount()
  {
    SPI.endTransaction();
    SPI.end();
    _ss     = NU;
    _powner = NULL;
    //SOUTLN("md_spi.unmount  SPI released ");
  }

