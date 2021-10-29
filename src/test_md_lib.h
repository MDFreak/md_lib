#ifndef _TEST_MD_LIB_
  #define _TEST_MD_LIB_

  // activate testing
  #define TEST_MD_LIB_ACTIVE

  #if defined(TEST_MD_LIB_ACTIVE)
      // select library to test
      //#define TEST_MD_LCD
      //#define TEST_MD_BUZZER
      //#define TEST_MD_KEYPADSHIELD
      //#define TEST_MD_TOUCH
      //#define TEST_MD_WEBSERVER
      #define TEST_MD_LED
      //#define TEST_MD_OLED
      //#define TEST_MD_FRAM
      //#define TEST_MD_WIFI

      #include <SPI.h>
      #include <md_util.h>
      #include <md_defines.h>

      #if defined( TEST_MD_LCD )
          void setup_lcd();
          void lcd_loop();
        #endif
      #if defined( TEST_MD_BUZZER)
          void setup_buzzer();
          void buzzer_loop();
        #endif
      #if defined( TEST_MD_KEYPADSHIELD)
          void setup_keypad();
          void keypad_loop();
        #endif
      #if defined( TEST_MD_TOUCH)
          void setup_touch();
          void touch_loop();
        #endif
      #if defined( TEST_MD_WEBSERVER)
          void setup_webserver();
          void webserver_loop();
        #endif
      #if defined( TEST_MD_LED)
          void setup_led();
          void led_loop();
        #endif
      #if defined( TEST_MD_OLED)
          void setup_oled();
          void oled_loop();
        #endif
      #if defined( TEST_MD_FRAM)
          void setup_fram();
          void fram_loop();
        #endif
      #if defined( TEST_MD_WIFI)
          void setup_wifi();
          void wifi_loop();
        #endif
    #endif

#endif