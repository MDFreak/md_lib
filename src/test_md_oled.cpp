#include <Arduino.h>

#include "test_md_lib.h"

#if defined(TEST_MD_LIB_ACTIVE)
  #if defined(TEST_MD_OLED)

    #include <md_oled.h>

    #define OLED1
    //#define OLED2

    #if defined(OLED1)
        #define I2C_ADDR_OLED1
        #define I2C_SDA_OLED1
        #define I2C_SCL_OLED1
        #define OLEDDISPLAY_GEOMETRY
        #define OLED1_GEO

        md_oled oled1 = md_oled((uint8_t) I2C_ADDR_OLED1, (uint8_t) I2C_SDA_OLED1,
                                (uint8_t) I2C_SCL_OLED1, (OLEDDISPLAY_GEOMETRY) OLED1_GEO);
      #endif

    #if defined(OLED2)
        md_oled oled2 = md_oled((uint8_t) I2C_ADDR_OLED2, (uint8_t) I2C_SDA_OLED2,
                                (uint8_t) I2C_SCL_OLED2, (OLEDDISPLAY_GEOMETRY) OLED2_GEO);
      #endif

    msTimer oledT   = msTimer(DISP_CYCLE);
    uint8_t oledIdx = 0;

    void setup_oled()
      {

      }

    void oled_loop()
      {

      }

  #endif
#endif
