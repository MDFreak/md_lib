#ifndef _MD_SPI_H_
  #define _MD_SPI_H_

  #include <SPI.h>
  //#include <md_defines.h>

  #define DEF_TOUT_MS  200
  #define USE_HSPI     0   // TODO: HSPI to be testet

  class mdSPI_class
    {
      public:
        mdSPI_class(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t spi_bus = VSPI);

        bool mount(void* powner, uint8_t cs, SPISettings settings, uint16_t tout_ms = DEF_TOUT_MS);
        void unmount();

      private:
        SPIClass* _pSPI = NULL;
        void*   _powner = NULL;
        uint8_t _sck  = 0;
        uint8_t _miso = 0;
        uint8_t _mosi = 0;
        uint8_t _ss   = 0;
    };

  extern mdSPI_class* pVSPI;
  extern mdSPI_class* pHSPI;
#endif // _MD_SPI_H_