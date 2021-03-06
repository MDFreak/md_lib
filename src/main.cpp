#include <Arduino.h>

#include "test_md_lib.h"

#if defined(TEST_MD_LIB_ACTIVE)

  void setup()
  {
    #if defined( TEST_MD_LCD )
        setup_lcd();
      #endif
    #if defined( TEST_MD_BUZZER)
        setup_buzzer();
      #endif
    #if defined( TEST_MD_KEYPADSHIELD)
        setup_keypad();
      #endif
    #if defined( TEST_MD_TOUCH)
        setup_touch();
      #endif
    #if defined( TEST_MD_WEBSERVER)
        setup_webserver();
      #endif
    #if defined( TEST_MD_LED)
        setup_led();
      #endif
    #if defined( TEST_MD_OLED)
        setup_oled();
      #endif
    #if defined( TEST_MD_FRAM)
        setup_fram();
      #endif
    #if defined( TEST_MD_WIFI)
        setup_wifi();
      #endif
  }

  void loop()
  {
    #if defined( TEST_MD_LCD )
        lcd_loop();
      #endif
    #if defined( TEST_MD_BUZZER)
        buzzer_loop();
      #endif
    #if defined( TEST_MD_KEYPADSHIELD)
        keypad_loop();
      #endif
    #if defined( TEST_MD_TOUCH)
        touch_loop();
      #endif
    #if defined( TEST_MD_WEBSERVER)
        webserver_loop();
      #endif
    #if defined( TEST_MD_LED)
        led_loop();
      #endif
    #if defined( TEST_MD_OLED)
        oled_loop();
      #endif
    #if defined( TEST_MD_FRAM)
        fram_loop();
      #endif
    #if defined( TEST_MD_WIFI)
        wifi_loop();
      #endif
  }

#endif
