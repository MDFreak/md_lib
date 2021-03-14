#ifndef _TEST_MD_LIB_
  #define _TEST_MD_LIB_

  // activate testing
  #define TEST_MD_LIB_ACTIVE

  #if defined(TEST_MD_LIB_ACTIVE)
      // select library to test
      #define TEST_MD_LCD
      #define TEST_MD_BUZZER
      #define TEST_MD_KEYPADSHIELD
      #define TEST_MD_TOUCH
      #define TEST_MD_WEBSERVER
      #define TEST_MD_LED
      #define TEST_MD_OLED
      #define TEST_MD_FRAM
      #define TEST_MD_WIFI
    #endif

  #if defined( TEST_MD_LCD )
    #endif
  #if defined( TEST_MD_BUZZER)
    #endif
  #if defined( TEST_MD_KEYPADSHIELD)
    #endif
  #if defined( TEST_MD_TOUCH)
    #endif
  #if defined( TEST_MD_WEBSERVER)
    #endif
  #if defined( TEST_MD_LED)
    #endif
  #if defined( TEST_MD_OLED)
    #endif
  #if defined( TEST_MD_FRAM)
    #endif
  #if defined( TEST_MD_WIFI)
    #endif


#endif